#include "Tuyav.h"
#include "global.h"


//User can choose SoftwareSerial
//SoftwareSerial mySerial(Rx,Tx);

//User can choose HardwareSerial: Serial1/Serial2/Serial3
Tuyav tuyav(&Serial1);

//Initialize Time for updating Arbitrary Values
const unsigned long eventTime = 2000;
unsigned long previousTime = 0;


void setup() {

  //If user does not use a pin: set the pin as PIN_UNUSED
  tuyav.setDigitalInputs(9, 10, 11, 12, 13);        //Set DigitalInputs
  tuyav.setAnalogInputs(A0, A1, A2, A3, A4);        //Set AnalogInputs
  tuyav.setDigitalOutputs(PIN_UNUSED, 5, 6, 7, 8);  //SetDigitalOutputs
  tuyav.setAnalogOutputs(22, 24, 26);               //Set AnalogOutputs

  /*
    //User can choose Wi-Fi mode calling "tuyav.setWifiMode(Mode)" function
    //Mode: SMART_CONFIG_STATE/AP_STATE

    //example: If first digital input is equal to one the wifi mode is set to "AP_STATE"
    if (digitalRead(9) == 1)
    {
    tuyav.setWifiMode(AP_STATE);
    }

  */

  tuyav.setUpInit();

  //Pin 4 is set as an output
  pinMode(4, OUTPUT);
}

void loop() {

  tuyav.tuyaUpdate();

  //example: user can access the inputs values and states
  int D_IN_1 = tuyav.DIGITAL_IN[0];     // "tuyav.DIGITAL_IN[0]" returns the state of the first digital input
  int A_IN_1 = tuyav.ANALOG_IN[0];      // "tuyav.ANALOG_IN[0]" returns the value of the first analog input

  //example: user can access the outputs values and states
  int D_OUT_1 = tuyav.DIGITAL_OUT[0];    // "tuyav.DIGITAL_OUT[0]" returns the state of the first digital output
  int A_OUT_1 = tuyav.ANALOG_OUT[0];     // "tuyav.ANALOG_OUT[0]" returns the value of the first analog output


  //example
  if (A_OUT_1 >= 10)
  {
    digitalWrite(4, HIGH);
  }
  else
  {
    digitalWrite(4, LOW);
  }


  //User can send an Arbitrary value to the cloud calling "tuyav.sendUserStringValue(USER_VALUE_1/USER_VALUE_2/USER_VALUE_3, "UserStringValue")" and "tuyav.sendUserValue(USER_VALUE_4/, UserValue)"

  int sensorValue1 = 10; //User define a value for USER_VALUE_4
  int sensorValue2 = 200; //User can define a value for USER_VALUE_5

  unsigned long currentTime = millis();
  if (currentTime - previousTime >= eventTime)
  {
    tuyav.sendUserStringValue(USER_VALUE_1, "Kitchen"); //User send string value "Kitchen" to the cloud
    tuyav.sendUserStringValue(USER_VALUE_2, "Living Room"); //User send string value "Living Room" to the cloud
    tuyav.sendUserStringValue(USER_VALUE_3, "Bedroom"); //User send string value "Bedroom" to the cloud

    tuyav.sendUserValue(USER_VALUE_4, sensorValue1); //User send string value sensorValue1 to the cloud
    tuyav.sendUserValue(USER_VALUE_5, sensorValue2); //User send string value sensorValue2 to the cloud

    previousTime = currentTime;
  }
}
