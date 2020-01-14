#include "TuyaSerial.h"

//TuyaSerial constructor
TuyaSerial::TuyaSerial()
{
  
}

//TuyaSerial constructor
TuyaSerial::TuyaSerial(HardwareSerial *serial): stream(nullptr)
{
  stream = serial;
  serial->begin(9600);
  _isHWSerial = true;
}

//TuyaSerial constructor
TuyaSerial::TuyaSerial(SoftwareSerial* serial): stream(nullptr)
{
  stream = serial;
  serial->begin(9600);
  _isHWSerial = true;
}

//TuyaSerial Read Function 
char TuyaSerial::read()
{
  return stream->read();
}

//TuyaSerial Write Function
size_t TuyaSerial::write(char value)
{
  return stream->write(value);
}

//TuyaSerial Available Function
int TuyaSerial::available()
{
  return stream->available();
}

//TuyaSerial destructor
TuyaSerial::~TuyaSerial()
  {

  }
