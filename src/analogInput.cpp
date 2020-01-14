#include "analogInput.h"
#include "mcu_api.h"
//Update analog input values
int AnalogInput::updateAnalogValue()
{
  _currentValue = analogRead(_pinID);
  mcu_dp_value_update(_tuyaPinID, _currentValue);
  return _currentValue;
}