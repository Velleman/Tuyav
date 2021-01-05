/* 
 *  --- TUYAV example program ---
 *  v1.0 by BN&PSI @ Velleman NV - feb 2020
 *  works with VMA354 Tuya IoT interface
 *  
 *  
 *  This program is an UART passthrough, this is intended to work with an Arduino Nano Every  
 *  This program is primaly used for debugging with the Tuya debug assistant
 *  
 *  Connections:
 *  VMA354    Arduino UNO           Arduino MEGA          Arduino Nano Every
 *  VCC       5V                    5V                    5V
 *  GND       GND                   GND                   GND
 *  TX        PIN2  (SWserial RX)   PIN19 (Serial1 RX1)   PIN RX0 (Serial1 RX0)
 *  RX        PIN3  (SWserial TX)   PIN18 (Serial1 TX1)   PIN TX1 (Serial1 TX1)
 */

//libraries
#include "Tuyav.h"
#include "global.h"

//selection of Serial port
#if defined(ARDUINO_AVR_UNO)     //Arduino UNO board: use SoftwareSerial with pins you select, see https://www.arduino.cc/en/Reference/softwareSerial
SoftwareSerial mySWserial(2, 3); //RX,TX (2 and 3 are recommended)
Tuyav tuyav(&mySWserial);
#else //Arduino Mega board: User can choose HardwareSerial: Serial1/Serial2/Serial3
Tuyav tuyav(&Serial1); //Serial1 is pin 18/19 on a Arduino Mega or pin TX1/RX0 on a Arduino Nano Every
#endif

//Initialize Time for updating Arbitrary Values
unsigned long currentTime = 0;
unsigned long previousTime = 0;
int updateDelay = 3000; //3 seconds by default. Min 1 second or you will overflow the serial communication!

void setup()
{
    //start serial for debugging
    Serial.begin(9600);
    Serial1.begin(9600);

    //define the TUYA pins
    // There are 3 digital inputs, 3 analog inputs, 5 digital output and 3 analog outputs available
    // If you do not use a pin, set the pin as PIN_UNUSED
    tuyav.setDigitalInputs(PIN_UNUSED, PIN_UNUSED, PIN_UNUSED);                  //Set DigitalInputs
    tuyav.setAnalogInputs(PIN_UNUSED, PIN_UNUSED, PIN_UNUSED);                   //Set AnalogInputs
    tuyav.setDigitalOutputs(13, PIN_UNUSED, PIN_UNUSED, PIN_UNUSED, PIN_UNUSED); //SetDigitalOutputs
    tuyav.setAnalogOutputs(PIN_UNUSED, PIN_UNUSED, PIN_UNUSED);                  //Set AnalogOutputs (PWM digital pins)
    //tuyav.setDebug(true);
    tuyav.setPasstrough(true);
    //init the chip
    tuyav.initialize();
}

void loop()
{
    //Should be called continuously
    tuyav.tuyaUpdate();
    while(Serial.available())
    {
        tuyav.get_tuyaSerial().write(Serial.read());
    }
}
