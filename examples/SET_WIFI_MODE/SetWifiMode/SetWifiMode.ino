#include "Tuyav.h"
#include "global.h"


//User can choose SoftwareSerial
/*
  SoftwareSerial mySerial(Rx,Tx);
  Tuyav tuyav(&mySerial);
*/
//User can choose HardwareSerial: Serial1/Serial2/Serial3
Tuyav tuyav(&Serial1);


void setup() {

  //For an unused pin: Set the pin as PIN_UNUSED
  tuyav.setDigitalInputs(9, 10, 11, 12, 13);
  tuyav.setAnalogInputs(A0, A1, A2, A3, A4);
  tuyav.setDigitalOutputs(4, 5, 6, 7, 8);
  tuyav.setAnalogOutputs(22, 24, 26);



  //User can choose Wi-Fi mode calling "tuyav.setWifiMode(Mode)" function
  //Mode: SMART_CONFIG_STATE/AP_STATE

  //example: If first digital input is equal to one the wifi mode is set to "AP_STATE"
  if (digitalRead(9) == 1)
  {
    tuyav.setWifiMode(AP_STATE);
  }


  tuyav.setUpInit();
}

void loop() {

  tuyav.tuyaUpdate();

}
