/* 
 *  --- TUYAV example program ---
 *  v1.0 by BN&PSI @ Velleman NV - feb 2020
 *  works with VMA354 Tuya IoT interface
 *  
 *  This example retrieves the weather Info from the Tuya module
 *  The Tuya module will send weather info to the arduino when it's connected to the cloud on start-up.
 *  After the startup it will send the weather info with a 30 min interval
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
char printBuffer[100];
void setup()
{
    //start serial for debugging
    Serial.begin(9600);
    Serial.println("Tuya Weather program");

//if ArduinoMega or ArduinoNanoEvery, start Serial1
#if defined(ARDUINO_AVR_UNO)
#else
    Serial1.begin(9600);
#endif

    //define the TUYA pins
    // There are 3 digital inputs, 3 analog inputs, 5 digital output and 3 analog outputs available
    // If you do not use a pin, set the pin as PIN_UNUSED
    tuyav.setDigitalInputs(PIN_UNUSED, PIN_UNUSED, PIN_UNUSED);                  //Set DigitalInputs
    tuyav.setAnalogInputs(PIN_UNUSED, PIN_UNUSED, PIN_UNUSED);                   //Set AnalogInputs
    tuyav.setDigitalOutputs(PIN_UNUSED, PIN_UNUSED, PIN_UNUSED, PIN_UNUSED, PIN_UNUSED); //SetDigitalOutputs
    tuyav.setAnalogOutputs(PIN_UNUSED, PIN_UNUSED, PIN_UNUSED);                  //Set AnalogOutputs (PWM digital pins)

    //init the chip
    tuyav.initialize();
    //Wait for tuya module to connect to the cloud
    Serial.println("Waiting for Tuya Module to be connected to the cloud");
    while(tuyav.getNetworkStatus() != 4)
    {
        tuyav.tuyaUpdate();
    }
    Serial.println("Activating the weather service");
    tuyav.startWeather();
}

void loop()
{
    //Should be called continuously
    tuyav.tuyaUpdate();

    if(tuyav.WeatherReceived()) //Check if new weather info is received, On startup and after that a 30min interval
    {
        //To force the Tuya module to send the weather immediately power cycle the Tuya module
        /*
        Available Weather Info
        int Temperature
        int Humidity
        int PM25
        char* Condition
        int Pressure
        int RealFeel
        int UVIndex
        char* WindDirection
        int WindSpeed
        char* SunSet
        char* SunRise
        int AirQuality
        int PM10
        int O3
        int NO2
        int CO
        */
        weather_info weather = tuyav.getWeatherInfo();
        //City
        String City = String(weather.City);
        Serial.print("This is the weather info for ");
        Serial.println(City);        
        //Temperature
        String temperature = "The Temperature is ";
        temperature += weather.Temperature;
        temperature += " degrees Celsius";
        Serial.println(temperature);
        //Condition
        sprintf(printBuffer,"The condition is %s",weather.Condition);
        Serial.println(printBuffer);
        //Sunset
        sprintf(printBuffer,"The Sun will set at %s",weather.SunSet);
        Serial.println(printBuffer);
        //Sunrise
        sprintf(printBuffer,"The Sun will rise at %s",weather.SunRise);
        Serial.println(printBuffer);
        //UV Index
        sprintf(printBuffer,"The UVIndex is %d",weather.UVIndex);
        Serial.println(printBuffer);
        
        
        tuyav.setWeatherReceived(false); //Reset flag
    }
}
