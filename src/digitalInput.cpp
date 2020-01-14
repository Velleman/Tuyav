#include "digitalInput.h"
#include "mcu_api.h"
//Update digital input states
bool DigitalInput::updateDigitalState()
{
  mcu_dp_bool_update(_tuyaPinID, _currentState);
  _previousState = _currentState;
  return _currentState;
}