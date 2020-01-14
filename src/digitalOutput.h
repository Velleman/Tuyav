#ifndef digitaloutput_h
#define digitaloutput_h
#include "OutputClass.h"

class OutputClass;
//digital output class inherited fron output class
class DigitalOutput : public OutputClass
{
  public:
    DigitalOutput(int pin = 0, int TuyaID = 0): OutputClass(pin, TuyaID) {}
    void digitalOutputStateInit();
    void writeDigital(bool state);
};
#endif
