#ifndef Tuyav_h
#define Tuyav_h

#include "Arduino.h"
#include "digitalInput.h"
#include "analogInput.h"
#include "digitalOutput.h"
#include "analogOutput.h"
#include "InputClass.h"
#include "OutputClass.h"
#include "TuyaSerial.h"
#include "mcu_api.h"

#define USER_VALUE_1 119
#define USER_VALUE_2 120
#define USER_VALUE_3 121
#define USER_VALUE_4 122
#define USER_VALUE_5 123

class Tuyav
{
  public:
    Tuyav(HardwareSerial* serial);
    Tuyav(SoftwareSerial* serial);

    void setDigitalInputs(int pin1, int pin2, int pin3, int pin4, int pin5);
    void setAnalogInputs(int pin1, int pin2, int pin3, int pin4, int pin5);
    void setDigitalOutputs(int pin1, int pin2, int pin3, int pin4, int pin5);
    void setAnalogOutputs(int pin1, int pin2, int pin3);
    OutputClass getDigitalOutput(int id);
    OutputClass getAnalogOutput(int id);
    TuyaSerial get_tuyaSerial();

    void sendUserValue(int TuyaPinID, int newValue);
    void sendUserStringValue(int TuyaPinID, unsigned char value[]);
    void setWifiMode(unsigned char mode);
	void resetWifi();
    void setUpInit();
    void tuyaUpdate();
    void digitalUpdate();
    void analogUpdate();
    void digitalInputInit();
    void digitalOutputInit();
    void analogOutputInit();

    int ANALOG_IN[5];
    bool DIGITAL_IN[5];
    int ANALOG_OUT[3];
    bool DIGITAL_OUT[5];

  private:
    DigitalInput _digitalInputs[5];
    AnalogInput _analogInputs[5];
    DigitalOutput _digitalOutputs[5];
    AnalogOutput _analogOutputs[3];
    TuyaSerial _tuyaSerial;

    const unsigned long eventTime1 = 2000;
    const unsigned long eventTime2 = 8000;
    unsigned long previousTime1 = 0;
    unsigned long previousTime2 = 0;

    int _PinID;
    int _TuyaPinID;
    int _value;


};

#endif
