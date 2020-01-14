#include "Tuyav.h"
#include "global.h"


//User can choose SoftwareSerial
//SoftwareSerial mySerial(Rx,Tx);

//User can choose HardwareSerial: Serial1/Serial2/Serial3
Tuyav tuyav(&Serial1);

void setup() {

  //If user does not use a pin: set the pin as PIN_UNUSED
  tuyav.setDigitalInputs(9, 10, 11, 12, 13);//Set DigitalInputs
  tuyav.setAnalogInputs(A0, A1, A2, A3, A4);//Set AnalogInputs
  tuyav.setDigitalOutputs(PIN_UNUSED, 5, 6, 7, 8);//SetDigitalOutputs
  tuyav.setAnalogOutputs(22, 24, 26);//Set AnalogOutputs

  tuyav.setUpInit();
}

void loop() {

  tuyav.tuyaUpdate();

}
