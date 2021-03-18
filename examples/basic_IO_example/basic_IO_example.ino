/* 
 *  --- TUYAV example program ---
 *  v1.0 by BN&PSI @ Velleman NV - feb 2020
 *  works with VMA354 Tuya IoT interface
 *  
 *  This example program shows you how to interface with the Tuya IoT interface board
 *  for the full product info, please visit https://whadda.com/product/tuya-iot-interface-wpi354/
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
#if defined(ARDUINO_AVR_UNO)    //Arduino UNO board: use SoftwareSerial with pins you select, see https://www.arduino.cc/en/Reference/softwareSerial
SoftwareSerial mySWserial(2,3); //RX,TX (2 and 3 are recommended)
Tuyav tuyav(&mySWserial);         
#else                           //Arduino Mega board: User can choose HardwareSerial: Serial1/Serial2/Serial3
  Tuyav tuyav(&Serial1);        //Serial1 is pin 18/19 on a Arduino Mega or pin TX1/RX0 on a Arduino Nano Every
#endif

//Initialize Time for updating Arbitrary Values
unsigned long currentTime = 0;
unsigned long previousTime = 0;
int updateDelay = 3000;    //3 seconds by default. Min 1 second or you will overflow the serial communication!


void setup() 
{
  //start serial for debugging
  Serial.begin(9600);
  Serial.println("Tuya Demo program");

  //if ArduinoMega or ArduinoNanoEvery, start Serial1
  #if defined(ARDUINO_AVR_UNO)
  #else 
    Serial1.begin(9600);
  #endif
  
  //define the TUYA pins
  // There are 3 digital inputs, 3 analog inputs, 5 digital output and 3 analog outputs available
  // If you do not use a pin, set the pin as PIN_UNUSED
  tuyav.setDigitalInputs(4, 5, 6);                    //Set DigitalInputs
  tuyav.setAnalogInputs(A0, A1, A2);                  //Set AnalogInputs
  tuyav.setDigitalOutputs(13, 12, 8, 7, PIN_UNUSED);  //SetDigitalOutputs
  tuyav.setAnalogOutputs(9, 10, 11);                  //Set AnalogOutputs (PWM digital pins)

  //init the chip
  tuyav.initialize();
}

void loop() 
{
  //Should be called continuously 
  tuyav.tuyaUpdate();
  
  //check time
  currentTime = millis();

  //timer: run the code below every time the updateDelay has passed
  if(currentTime - previousTime > updateDelay)
  {
      //set arbitrary values (9 are available - read only in the app)
      tuyav.setUserValue(AV1, "Test Application");
      tuyav.setAV2("SW version");
      tuyav.setAV3("V1.0");
      String AV4msg = "Last update time:";
      tuyav.setUserValue(AV4, AV4msg);
      tuyav.setUserValue(AV5, String(currentTime - previousTime));
      tuyav.setAV6("ms ago"); 
      tuyav.setUserValue(AV7, "Time since boot:"); 
      tuyav.setAV8(String(currentTime));
      tuyav.setAV9("ms");

      //check the state of a digital inputs (read only in the app)
      Serial.print("State of DI2: "); Serial.println(tuyav.DIGITAL_IN[1]);
      //check the state of a analog input (read only in the app)
      int analogInCH2 = tuyav.ANALOG_IN[2];
      Serial.print("State of AI3: "); Serial.println(analogInCH2);
      //check the state of all digital outputs (can be set from the app)
      for(int t = 0; t < 5; t++)
      {
        Serial.print("State of DO"); Serial.print(t+1); Serial.print(": "); Serial.println(tuyav.DIGITAL_OUT[t]);
      }
      //check the state of a analog output (can be set from the app)
      Serial.print("State of AO1: "); Serial.println(tuyav.ANALOG_OUT[0]);
      Serial.println("----");

      previousTime = millis();   //now that everything has been done, the previous time is the current time (and not the var currentTime, because time has passed during this block of code)
  }
}
