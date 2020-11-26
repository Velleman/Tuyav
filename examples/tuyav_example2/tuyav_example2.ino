/* 
 *  --- TUYAV example program ---
 *  v1.0 by BN&PSI @ Velleman NV - nov 2020
 *  works with VMA354 Tuya IoT interface
 *  
 *  This example program shows you how to interface with the Tuya IoT interface board & how the timing function is called
 *  for the full manual, please visit https://www.whadda.com/product/VMA354
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

//Global time variables
int Clock_hour;       //hour      (0-23)
int Clock_minute;     //minutes   (0-59)
int Clock_second;     //seconds   (0-59)
int Clock_weekDay;    //day of the week   (1-7)  1=Sunday, 2=Monday, 3=Tuesday, 4=Wednesday, 5=Thursday, 6=Friday, 7=Saturday
int Clock_monthDay;   //day of the month  (0-31)
int Clock_month;      //month             (0-12)
int Clock_year;       //year              (2000-2099)

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

  //set debug mode (shows all TUYA communication on serial port)
  tuyav.setDebug(0);    //Set debug on (on=1, off=0);

  //init the chip
  tuyav.initialize();

  //wait a short while to let the Tuya module connect to the network
  delay(5000);
  //ask the Tuya module to update the time (specify the amount of retries, default: 10)
  getTuyaTime(10);      //getTuyaTime(int amountOfRetries);

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
      tuyav.setUserValue(AV1, "VMA354 Test Application");
      tuyav.setAV2("SW version");
      tuyav.setAV3("nov 2020");
      String AV4msg = "Last update time:";
      tuyav.setUserValue(AV4, AV4msg);
      tuyav.setUserValue(AV5, String(currentTime - previousTime));
      tuyav.setAV6("ms ago"); 
      tuyav.setUserValue(AV7, "upTime & setTime:"); 
      tuyav.setAV8(upTime());
      String AV9msg = String(Clock_hour); AV9msg += ":"; AV9msg += String(Clock_minute); AV9msg += ":"; AV9msg += String(Clock_second); AV9msg += " "; AV9msg += String(Clock_monthDay); AV9msg += "-"; AV9msg += String(Clock_month); AV9msg += "-"; AV9msg += String(Clock_year);
      tuyav.setAV9(AV9msg);

      //check the state of a digital inputs (read only in the app)
      Serial.print("State of DI2: "); Serial.println(tuyav.DIGITAL_IN[1]);
      //check the state of a analog input (read only in the app)
      int analogInCH2 = tuyav.ANALOG_IN[2];
      Serial.print("State of AI3: "); Serial.println(analogInCH2);
      //check the state of all digital outputs (can be set from the app)    Note: in this example DO5 will stay 0, since we did not allocate a PIN
      for(int t = 0; t < 5; t++)
      {
        Serial.print("State of DO"); Serial.print(t+1); Serial.print(": "); Serial.println(tuyav.DIGITAL_OUT[t]);
      }
      //check the state of a analog output (can be set from the app)
      Serial.print("State of AO1: "); Serial.println(tuyav.ANALOG_OUT[0]);
      Serial.println("----");

      //getTuyaTime(10);         //we can request a time update from the Tuya module here, so every cycle gives us the updated time 
      previousTime = millis();   //now that everything has been done, the previous time is the current time (and not the var currentTime, because time has passed during this block of code)
  }

  //other stuff that needs to be done goes here
  // ...
}


//function to return the uptime as a readable string, for example: '4 day(s), 14h 52m 42s'
String upTime()
{
 String upTimeString = "";
 long UPdays= 0;
 long UPhours=0;
 long UPmins= 0;
 unsigned long UPsecs= 0;
 UPsecs = millis()/1000; //convect milliseconds to seconds
 UPmins=UPsecs/60; //convert seconds to minutes
 UPhours=UPmins/60; //convert minutes to hours
 UPdays=UPhours/24; //convert hours to days
 UPsecs=UPsecs-(UPmins*60); //subtract the coverted seconds to minutes in order to display 59 secs max
 UPmins=UPmins-(UPhours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
 UPhours=UPhours-(UPdays*24); //subtract the coverted hours to days in order to display 23 hours max
 //compile results
 if (UPdays>0) // days will displayed only if value is greater than zero
 {
   upTimeString += UPdays;  upTimeString += " day(s), ";
 }
 upTimeString += UPhours;  upTimeString += "h ";
 upTimeString += UPmins;   upTimeString += "m ";
 upTimeString += UPsecs;   upTimeString += "s";
 return upTimeString;
}

//get local time from Tuya module (the time zone you have selected in the Tuya app)
void getTuyaTime(int retryCount)
{
  tuyav.tuyaUpdate();   //update Tuya vars
  Serial.println(F("Time requested from Tuya module"));
  tuyav.getTime();      //request update of global tuyav.newTime[] vars
  tuyav.tuyaUpdate();   //update Tuya vars once more
  delay(200);           //wait a short while for the serial commands to be processed

  //now that we have requested a time update, try the amount of times set in retryCount if we have received an update
  for (int retryCounter = 0; retryCounter < retryCount; retryCounter++) 
  {
    tuyav.tuyaUpdate();   //update Tuya vars once more
    if(tuyav.newTime[0])  //we are certain that the time info has been received (value of tuyav.newTime[0] is the update flag)
    {
      Serial.print(F("Time received from Tuya module: "));
      //update the Clock values so we can use them in our loop, set an RTC module, start counting, ... etc
      Clock_hour = tuyav.newTime[4];        Serial.print(Clock_hour); Serial.print(":");
      Clock_minute = tuyav.newTime[5];      Serial.print(Clock_minute); Serial.print(":");
      Clock_second = tuyav.newTime[6];      Serial.print(Clock_second); Serial.print(" ");
      Clock_weekDay = tuyav.newTime[7];
      Clock_monthDay = tuyav.newTime[3];    Serial.print(Clock_monthDay); Serial.print("-");
      Clock_month = tuyav.newTime[2];       Serial.print(Clock_month); Serial.print("-");
      Clock_year = tuyav.newTime[1];        Serial.println(Clock_year);

      tuyav.newTime[0] = 0; //reset update flag
      retryCounter = retryCount;    //exit for loop
    }
    else
    {
      Serial.print(F("No response yet, waiting... Retry:")); Serial.print(retryCounter+1); Serial.print("/"); Serial.println(retryCount);
      delay(500); //wait a while before trying again
    }
  }
}
