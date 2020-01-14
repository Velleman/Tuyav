#ifndef analogonput_h
#define analogonput_h
#include "OutputClass.h"
//analog output class inherited fron output class
class AnalogOutput : public OutputClass
{
  public:
    AnalogOutput(int pin = 0, int TuyaID = 0) : OutputClass(pin, TuyaID) {}
    void analogOutputValueInit();
    void writeAnalog(int value);


};
#endif
