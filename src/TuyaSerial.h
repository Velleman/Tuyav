#ifndef TuyaSerial_h
#define TuyaSerial_h

#include "Arduino.h"
#include "Stream.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

class TuyaSerial
{

  public:
    TuyaSerial();
    ~TuyaSerial();
    TuyaSerial(HardwareSerial *serial);
    TuyaSerial(SoftwareSerial *serial);
    char read();
    size_t write(char value);
    int available();
    bool _isHWSerial = true;

  private:
    Stream *stream;
};

#endif
