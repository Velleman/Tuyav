#ifndef OutputClass_h
#define OutputClass_h

#include "Arduino.h"

class OutputClass
{
  public:
    OutputClass(int pinID = 0, int TuyaID = 0);

    int get_PinID();
    void writeAnalog(int value);
    void writeDigital(bool State);
    void analogOutputValueInit();
    void digitalOutputStateInit();
 
  private:
    int _pinID;
    int _tuyaPinID;
    bool _state;
};

#endif
