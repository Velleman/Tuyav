#ifndef analoginput_h
#define analoginput_h
#include "InputClass.h"
//analog input class inherited fron input class
class AnalogInput : public InputClass
{
  public:
    AnalogInput(int pin = 0, int TuyaID = 0) : InputClass(pin, TuyaID) {}
    int updateAnalogValue();

};
#endif
