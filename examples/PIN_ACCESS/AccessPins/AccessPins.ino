#include "Tuyav.h"
#include "global.h"

//User can choose SoftwareSerial:
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

}
