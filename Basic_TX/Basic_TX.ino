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
#define JOYSTICKX A1 //X axis connected to the A1 
#define JOYSTICKY A2 //y axis connected to he A2
#define JOYSTICKPIN 2 //

const static uint8_t RADIO_ID = 1;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 0; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;
struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
    uint32_t FailedTxCount;
    bool sw;
    uint32_t joyx;
    uint32_t joyy;
};
NRFLite _radio;
RadioPacket _radioData;
void setup()
{
    Serial.begin(115200);
    pinMode(JOYSTICKPIN,INPUT_PULLUP);    
    if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    {
        Serial.println("Cannot communicate with radio");
        while (1); // Wait here forever.
    }  
   _radioData.FromRadioId = RADIO_ID;
}
void loop()
{
   _radioData.sw = digitalRead(JOYSTICKPIN); // read sw value fron joystick.
   _radioData.joyx = analogRead(JOYSTICKX);//read the x values from joystick.  
   _radioData.joyy = analogRead(JOYSTICKY);//read the y values from joystick. 
   _radioData.OnTimeMillis = millis();
   if (_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData))) // Note how '&' must be placed in front of the variable name.
   {
           String msg = " Datos x ";
           msg += _radioData.joyx;
           msg += " Datos y ";               
           msg += _radioData.joyy;
          Serial.print(msg);
          Serial.println("...Success");
   }
   delay(100);
}
