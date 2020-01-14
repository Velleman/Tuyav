#ifndef OutputClass_h
#define OutputClass_h

#include "Arduino.h"

class OutputClass
{
  public:
    OutputClass(int pinID = 0, int TuyaID = 0);

    int get_PinID();
 
  protected:
    int _pinID;
    int _tuyaPinID;
    bool _state;
};

#endif
