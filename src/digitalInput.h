#ifndef digitalinput_h
#define digitalinput_h
#include "InputClass.h"

class InputClass;
//digital input class inherited fron input class
class DigitalInput : public InputClass
{
  public:
    DigitalInput(int pin = 0, int TuyaID = 0): InputClass(pin, TuyaID) {}
    bool updateDigitalState();
};
#endif
