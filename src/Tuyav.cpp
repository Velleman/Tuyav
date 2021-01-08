#include "global.h"
#include "Tuyav.h"
#include "protocol.h"
#include "mcu_api.h"

//Tuyav constructor: Set hardwareSerial
Tuyav::Tuyav(HardwareSerial *serial)
{
  _tuyaSerial = TuyaSerial(serial);
}

//Tuyav constructor: Set SoftwareSerial
Tuyav::Tuyav(SoftwareSerial *serial)
{
  _tuyaSerial = TuyaSerial(serial);
}

//Set digital input pins
void Tuyav::setDigitalInputs(int pin1, int pin2, int pin3)
{
  _digitalInputs[0] = DigitalInput(pin1, 101);
  _digitalInputs[1] = DigitalInput(pin2, 102);
  _digitalInputs[2] = DigitalInput(pin3, 103);
}

//Set analog input pins
void Tuyav::setAnalogInputs(int pin1, int pin2, int pin3)
{
  _analogInputs[0] = AnalogInput(pin1, 106);
  _analogInputs[1] = AnalogInput(pin2, 107);
  _analogInputs[2] = AnalogInput(pin3, 108);
}

//Set digital output pins
void Tuyav::setDigitalOutputs(int pin1, int pin2, int pin3, int pin4, int pin5)
{
  _digitalOutputs[0] = DigitalOutput(pin1, 111);
  _digitalOutputs[1] = DigitalOutput(pin2, 112);
  _digitalOutputs[2] = DigitalOutput(pin3, 113);
  _digitalOutputs[3] = DigitalOutput(pin4, 114);
  _digitalOutputs[4] = DigitalOutput(pin5, 115);
}

//Set analog output pins
void Tuyav::setAnalogOutputs(int pin1, int pin2, int pin3)
{
  _analogOutputs[0] = AnalogOutput(pin1, 116);
  _analogOutputs[1] = AnalogOutput(pin2, 117);
  _analogOutputs[2] = AnalogOutput(pin3, 118);
}

//To access digital output pin ids
DigitalOutput Tuyav::getDigitalOutput(int id)
{
  return _digitalOutputs[id].get_PinID();
}

//To access analog output pin ids
AnalogOutput Tuyav::getAnalogOutput(int id)
{
  return _analogOutputs[id].get_PinID();
}

//To access private object "_tuyaSerial"
TuyaSerial Tuyav::get_tuyaSerial()
{
  return _tuyaSerial;
}

bool Tuyav::getDebug()
{
  return _debug;
}

//Tuya setup Function
void Tuyav::initialize()
{
  if (_debug)
  {
    Serial.begin(9600);
  }

  //" Please add wifi_protocol_init() in the main function to complete the wifi protocol initialization."
  wifi_protocol_init();

  digitalInputInit();  //Call digitalInputInit() Function
  digitalOutputInit(); //Call digitalOutputInit() Function
  analogOutputInit();  //Call analogOutputInit() Function
  userValueInit();
}

void Tuyav::userValueInit()
{
  for (int i = 0; i < 9; i++)
  {
    _userValues[i].setID(AV1 + i);
  }
}

//Tuya Update Function
void Tuyav::tuyaUpdate()
{

  while (_tuyaSerial.available())
  {
    char c = _tuyaSerial.read();
    if (_debug)
    {
      Serial.println(c, HEX);
    }
    if (_passthrough)
    {
      Serial.print(c);
    }
    else
    {
      //"Please call uart_receive_input(value) in the serial port receive interrupt. The serial port data is processed by MCU_SDK. The user should not process it separately."
      uart_receive_input(c);
    }
  }
  wifi_uart_service();
  unsigned long currentTime = millis();

  //check if the last time is more than 2000ms ago
  if (currentTime - previousTime1 >= eventTime1)
  {
    digitalUpdate();
    wifi_uart_service();
    analogUpdate();
    wifi_uart_service();
    userValueUpdate();
    wifi_uart_service();
    previousTime1 = millis();
  }
}

//Initialize each digital inputs
void Tuyav::digitalInputInit()
{
  for (int i = 0; i < 3; i++)
  {
    _digitalInputs[i].digitalStateInit();
  }
}

//Initialize each digital outputs
void Tuyav::digitalOutputInit()
{
  for (int i = 0; i < 5; i++)
  {
    _digitalOutputs[i].digitalOutputStateInit();
  }
}

//Initialize each analog outputs
void Tuyav::analogOutputInit()
{

  for (int i = 0; i < 3; i++)
  {
    _analogOutputs[i].analogOutputValueInit();
  }
}

//Update each digital inputs
void Tuyav::digitalUpdate()
{

  //Compare if different send update
  for (int i = 0; i < 3; i++)
  {
    if (_digitalInputs[i].isDifferentState() && _digitalInputs[i].get_PinID() != PIN_UNUSED)
    {
      DIGITAL_IN[i] = _digitalInputs[i].updateDigitalState();
    }
  }
}

void Tuyav::userValueUpdate()
{
  for (int i = 0; i < 9; i++)
  {
    mcu_dp_string_update(AV1 + i, reinterpret_cast<const unsigned char *>(_userValues[i].getValue().c_str()), _userValues[i].getValue().length());
  }
}

//Update each analog inputs
void Tuyav::analogUpdate()
{
  for (int i = 0; i < 3; i++)
  {
    if (_analogInputs[i].get_PinID() != PIN_UNUSED)
    {
      ANALOG_IN[i] = _analogInputs[i].updateAnalogValue();
    }
  }
}

//Send user's arbitrary String value to the cloud
void Tuyav::setUserValue(int TuyaPinID, String value)
{
  uint8_t index = TuyaPinID - AV1;
  _userValues[index].setValue(value);
}

void Tuyav::setUpdateRateMs(unsigned long updateRate)
{
  if (updateRate < 2000)
    updateRate = 2000;
  eventTime1 = updateRate;
}

void Tuyav::setAV1(String value)
{
  _userValues[0].setValue(value);
}

void Tuyav::setAV2(String value)
{
  _userValues[1].setValue(value);
}

void Tuyav::setAV3(String value)
{
  _userValues[2].setValue(value);
}

void Tuyav::setAV4(String value)
{
  _userValues[3].setValue(value);
}

void Tuyav::setAV5(String value)
{
  _userValues[4].setValue(value);
}

void Tuyav::setAV6(String value)
{
  _userValues[5].setValue(value);
}

void Tuyav::setAV7(String value)
{
  _userValues[6].setValue(value);
}

void Tuyav::setAV8(String value)
{
  _userValues[7].setValue(value);
}

void Tuyav::setAV9(String value)
{
  _userValues[8].setValue(value);
}

void Tuyav::setDebug(bool enabled)
{
  _debug = enabled;
}

void Tuyav::setPasstrough(bool pass)
{
  _passthrough = pass;
}

bool Tuyav::getPasstrough()
{
  return _passthrough;
}

void Tuyav::getTime()
{
  wifi_uart_write_frame(GET_LOCAL_TIME_CMD, 0, 1);
}

void Tuyav::setNetworkStatus(Network_Status status)
{
  _networkStatus = status;
}

Network_Status Tuyav::getNetworkStatus()
{
  //mcu_get_wifi_work_state();
  //mcu_get_wifi_connect_status();
  wifi_uart_write_frame(GET_WIFI_STATUS_CMD, MCU_TX_VER, 0);
  wifi_uart_service();
  return _networkStatus;
}

void Tuyav::startWeather()
{
  mcu_open_weather();
  wifi_uart_service();
}

void Tuyav::setWeatherParam(char *name, int param)
{
  if(my_strcmp(name,"temp") == 0)
  {
    _weather.Temperature = param;
  }
  else if(my_strcmp(name,"humidity") == 0)
  {
    _weather.Humidity = param;
  }
  else if(my_strcmp(name,"pm25") == 0)
  {
    _weather.PM25 = param;
  }
  else if(my_strcmp(name,"windSpeed") == 0)
  {
    _weather.WindSpeed = param;
  }
  else if(my_strcmp(name,"uvi") == 0)
  {
    _weather.UVIndex = param;
  }
  else if(my_strcmp(name,"pressure") == 0)
  {
    _weather.Pressure = param;
  }
  else if(my_strcmp(name,"aqi") == 0)
  {
    _weather.AirQuality = param;
  }
  else if(my_strcmp(name,"realFeel") == 0)
  {
    _weather.RealFeel = param;
  }
  else if(my_strcmp(name,"pm10") == 0)
  {
    _weather.PM10 = param;
  }
  else if(my_strcmp(name,"o3") == 0)
  {
    _weather.O3 = param;
  }
  else if(my_strcmp(name,"no2") == 0)
  {
    _weather.NO2 = param;
  }
  else if(my_strcmp(name,"co") == 0)
  {
    _weather.CO = param;
  }
}
void Tuyav::setWeatherParam(char *name, const unsigned char* param)
{
  if (my_strcmp(name, "condition") == 0)
  {
    my_strcpy(_weather.Condition,param);
  }
  if(my_strcmp(name, "sunRise") == 0)
  {
    my_strcpy(_weather.SunRise,param);
  }
  if(my_strcmp(name, "sunSet") == 0)
  {
    my_strcpy(_weather.SunSet,param);
  }
  if(my_strcmp(name,"city") == 0)
  {
    my_strcpy(_weather.City,param);
  }
}
void Tuyav::setWeatherReceived(bool received)
{
  _weatherReceived = received;
}
weather_info Tuyav::getWeatherInfo()
{
  return _weather;
}
bool Tuyav::WeatherReceived()
{
  return _weatherReceived;
}