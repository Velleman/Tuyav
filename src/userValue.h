#ifndef USERVALUE_H
#define USERVALUE_H
#include "Arduino.h"
class UserValue
{
    public:
    UserValue();
    UserValue(uint8_t tuyaId);
    void setValue(String value);
    String getValue();
    void setID(uint8_t tuyaID);

    private:
        uint8_t _tuyaid;
        String _value;
};
#endif