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
#ifdef ARDUINO_AVR_NANO_EVERY
	digitalWrite(_pinID, _state?HIGH:LOW);
#else
	digitalWrite(_pinID, _state);
#endif
  
}