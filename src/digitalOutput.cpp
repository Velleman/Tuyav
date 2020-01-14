#include "digitalOutput.h"

//Initialize digital output states
void DigitalOutput::digitalOutputStateInit()
{
  writeDigital(LOW);
}

//Write on digital outputs
void DigitalOutput::writeDigital(bool state)
{
  _state = state;
  digitalWrite(_pinID, _state);
}