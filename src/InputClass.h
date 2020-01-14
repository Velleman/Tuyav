#ifndef InputClass_h
#define InputClass_h

#include "Arduino.h"

class InputClass
{
  public:
    InputClass(int pin = 0, int TuyaID = 0);
    void digitalStateInit();
    bool isDifferentState();
    
    
    int get_PinID();

  protected:
    int _pinID;
    int _tuyaPinID;
    bool _currentState = false;
    bool _previousState;
    int _currentValue = 0;
};

#endif
