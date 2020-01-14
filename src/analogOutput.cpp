#include "analogOutput.h"

//Initialize analog output values
void AnalogOutput::analogOutputValueInit()
{
  analogWrite(_pinID, 0);
}

//Write on analog outputs 
void AnalogOutput::writeAnalog(int value)
{
  analogWrite(_pinID, value);
}