#include "InputClass.h"
#include "Arduino.h"
#include "mcu_api.h"

//InputClass Constructor
InputClass::InputClass(int pin, int TuyaID)
{
  _pinID = pin;
  _tuyaPinID = TuyaID;

  pinMode(_pinID, INPUT_PULLUP);
}

//To access the private variable _pinID
int InputClass::get_PinID()
{
  return _pinID;
}

//Initialize digital input states
void InputClass::digitalStateInit()
{
  _previousState = digitalRead(_pinID);
  _currentState = _previousState;
}

//Compare current and previous state of digital inputs
bool InputClass::isDifferentState()
{
  _currentState = digitalRead(_pinID);
  return _previousState != _currentState ? true : false;
}

//Update digital input states
bool InputClass::updateDigitalState()
{
  mcu_dp_bool_update(_tuyaPinID, _currentState);
  _previousState = _currentState;
  return _currentState;
}

//Update analog input values
int InputClass::updateAnalogValue()
{
  _currentValue = analogRead(_pinID);
  mcu_dp_value_update(_tuyaPinID, _currentValue);
  return _currentValue;
}
