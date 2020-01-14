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

  //For an unused pin: Set the pin as PIN_UNUSED
  tuyav.setDigitalInputs(9, 10, 11, 12, 13);
  tuyav.setAnalogInputs(A0, A1, A2, A3, A4);
  tuyav.setDigitalOutputs(4, 5, 6, 7, 8);
  tuyav.setAnalogOutputs(22, 24, 26);

  tuyav.setUpInit();

}

void loop() {

  tuyav.tuyaUpdate();

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
