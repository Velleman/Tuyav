#include "OutputClass.h"
#include "Arduino.h"

//OutputClass Constructor
OutputClass::OutputClass(int pinID, int tuyaID)
{
  _pinID = pinID;
  _tuyaPinID = tuyaID;

  pinMode(_pinID, OUTPUT);
}

//To access the private variable _pinID
int OutputClass::get_PinID()
{
  return _pinID;
}

//Write on digital outputs
void OutputClass::writeDigital(bool state)
{
  _state = state;
  digitalWrite(_pinID, _state);
}

//Write on analog outputs 
void OutputClass::writeAnalog(int value)
{
  analogWrite(_pinID, value);
}

//Initialize digital output states
void OutputClass::digitalOutputStateInit()
{
  digitalWrite(_pinID, LOW);
}

//Initialize analog output values
void OutputClass::analogOutputValueInit()
{
  analogWrite(_pinID, 0);
}
