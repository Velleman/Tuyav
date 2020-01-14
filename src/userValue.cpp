#include "userValue.h"

UserValue::UserValue()
{
    _value = "";
}

UserValue::UserValue(uint8_t tuyaID)
{
    _tuyaid = tuyaID;
    _value = "";
}

void UserValue::setValue(String value)
{
    _value = value;
}

String UserValue::getValue()
{
    return _value;
}

void UserValue::setID(uint8_t tuyaID)
{
    _tuyaid = tuyaID;
}