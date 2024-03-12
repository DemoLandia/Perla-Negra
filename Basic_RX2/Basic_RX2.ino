/*

Demonstrates simple RX and TX operation.
Please read the notes in NRFLite.h for a description of all library features.

Radio    Arduino
CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection
VCC   -> No more than 3.6 volts
GND   -> GND

*/

#include <SPI.h>
#include <NRFLite.h>
#include <Servo.h>
#define JOYSTICKX A0 //X axis connected to the A0 
#define JOYSTICKY A1 //y axis connected to the A1
#define JOYSTICKPIN 2 // pin connected to the sw
#define servopin 6 // pin connected to the servo
#define luzpin 3 // pin connected to the luzled pwm

const int MotorPin = 5;//pin connected to the Motor control pwm
const static uint8_t RADIO_ID = 0;    // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;
bool EstadoLuz;
bool EstadoAnteriorLuz;
bool sentido;
struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
    uint32_t FailedTxCount;
    bool sw;
    uint32_t joyx;
    uint32_t joyy;
};
Servo MyServo;
NRFLite _radio;
RadioPacket _radioData;
void setup()
{
    sentido = false;
    EstadoLuz = false;
    EstadoAnteriorLuz = false;
    pinMode(servopin,OUTPUT);
    MyServo.attach(servopin);
    pinMode(MotorPin,OUTPUT);
    pinMode(luzpin,OUTPUT);
    Serial.begin(115200);
    if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    {
        Serial.println("Cannot communicate with radio");
        while (1); // Wait here forever.
    }
    analogWrite(MotorPin,0);
    MyServo.write(90);
}

void loop()
{
  while (_radio.hasData())
    {
        delay(200);
        _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.
        String msg = " Datos x ";
        msg += _radioData.joyx;
        msg += " Datos y ";
        msg += _radioData.joyy;  
        if ((not(_radioData.sw )) and (not(EstadoLuz)) )
        { 
           if (EstadoAnteriorLuz)
           {
             EstadoAnteriorLuz = false;
             digitalWrite(luzpin,LOW);            
           }
           else
           {
            EstadoAnteriorLuz = true;
            digitalWrite(luzpin,HIGH);
           }
        }       
        if (_radioData.joyx >= 0 &&  _radioData.joyx < 480)
            {
                MyServo.write(map(_radioData.joyx,0,520,0,90)); 
            }
        else {  
                if (_radioData.joyx > 520 &&  _radioData.joyx <= 1023)
                    { MyServo.write(map(_radioData.joyx,520,1023,90,180));}
                else {MyServo.write(90);}
            }
        if (_radioData.joyy > 520 &&  _radioData.joyy <= 1023)
            { analogWrite(MotorPin,255); }
        else { 
              analogWrite(MotorPin,0);}
        msg += " Datos Y Convertido ";
        msg += _radioData.joyy;
        Serial.println(msg);
   }
}
