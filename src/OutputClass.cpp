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
