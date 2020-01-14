/****************************************Copyright (c)*************************
**                           All rights reserved (C), 2015-2020, Tuya
**
**                                 http://www.tuya.com
**
**--------------File information-------------------------------------------------------
**File name   : protocol.h
**Description : Deliver/report data processing function
**Instructions for use :

                  *******Very important, be sure to watch！！！********

** 1、The user implements the data delivery/reporting function in this file.
** 2、DP ID / TYPE and data processing functions require the user to implement according to the actual definition
** 3、There are #err hints inside the function that needs the user to implement the code after starting some macro definitions.
**    Please delete the #err after completing the function.
**
**--------------Current revision---------------------------------------------------
** Version : v2.5.1
** Date    : 2018.10.27.
** Description: 1:Turn off streaming service by default
                2:Increase 03 protocol wifi status macro definition
		        3:Update and modify some function comments

** Version : v2.5.0
** Date    : 2018.4.18
** Description: 1:The protocol version is changed to 0x03
                2:Increase WIFI module heartbeat off function
                3:Increase weather function

** Version : v2.3.8
** Date    : 2018.1.17
** Description: 1:Variables added volatile to prevent compiler optimization
                2:Add #error hint

** Version : v2.3.7
** Date    : 2017.4.18
** Description: 1:Optimize serial queue reception processing

** Version : v2.3.6
** Date    : 2016.7.21
** Description: 1:Fix getting local time error
                2:Add hex_to_bcd conversion function

** Version : v2.3.5
** Date    : 2016.6.3
** Description: 1:Modify the return protocol version to 0x01
                2:Firmware upgrade data offset is modified to 4 bytes

** Version : v2.3.4
** Date    : 2016.5.26
** Description: 1:Optimize serial port parsing function
                2:Optimize compiler compatibility, cancel enum type definitions

** Version : v2.3.3
** Date    : 2016.5.24
** Description: 1:Modify mcu to get local time function
                2:Add wifi function test

** Version : v2.3.2
** Date    : 2016.4.23
** Description: 1:Optimize serial port data analysis
                2:Optimize the MCU firmware upgrade process
                3:Optimize the reporting process

** Version : v2.3.1
** Date    : 2016.4.15
** Description: 1:Optimize serial port data analysis

** Version : v2.3
** Date    : 2016.4.14
** Description: 1:Support MCU firmware online upgrade

** Version : v2.2
** Date    : 2016.4.11
** Description: 1:Modify the serial port data receiving mode

** Version : v2.1
** Date    : 2016.4.8
** Description: 1:Adding some compilers does not support function pointer compatibility options

** Version : v2.0
** Data    : 2016.3.29
** Description: 1:Optimize code structure
                2:Save RAM space
**
**-----------------------------------------------------------------------------

******************************************************************************/
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

#include "global.h"

/******************************************************************************
                          User related information configuration
******************************************************************************/
/******************************************************************************
                          1:Modify product information
******************************************************************************/
#define PRODUCT_KEY "bgqmvtsajekilsku"    //The unique product identification generated after product is created on development platform

#define MCU_VER "1.0.0"                                 //User's software version for MCU firmware upgrade, 
//MCU upgrade version needs to be modified

//Distribution mode selection, the default is CONFIG_MODE_DEFAULT, only three choices
#define CONFIG_MODE     CONFIG_MODE_DEFAULT             //Default distribution mode
//#define CONFIG_MODE     CONFIG_MODE_LOWPOWER            //Low power distribution network
//#define CONFIG_MODE     CONFIG_MODE_SPECIAL             //Special distribution network

/******************************************************************************
                          2:Does the MCU require a firmware upgrade?
  If you need to support MCU firmware upgrade, please open this macro
  The MCU can call the mcu_firm_update_query() function in the mcu_api.c file to get the current MCU firmware update.

                        ********WARNING!!!**********
  The current receive buffer is the size to turn off the firmware update function.
  The firmware upgrade package is 256 bytes.
  If you need to enable this function, the serial receive buffer will become larger.

******************************************************************************/
//#define         SUPPORT_MCU_FIRM_UPDATE                 //Enable MCU firmware upgrade function (off by default)
/******************************************************************************
                         3:Define the send and receive buffer:
          If the current RAM of the MCU is not enough, it can be modified to 24
******************************************************************************/
#ifndef SUPPORT_MCU_FIRM_UPDATE
#define WIFI_UART_QUEUE_LMT             128              //The size of the data receiving queue, such as the MCU's RAM is not enough, can be reduced
#define WIFI_UART_RECV_BUF_LMT          100             //According to the user's DP data size, it must be greater than 24
#else
#define WIFI_UART_QUEUE_LMT             128             //The size of the data receiving queue, such as the MCU's RAM is not enough, can be reduced
#define WIFI_UART_RECV_BUF_LMT          300             //Firmware upgrade buffer, need large cache, must be greater than 260
#endif

#define WIFIR_UART_SEND_BUF_LMT         100              //According to the user's DP data size, it must be greater than 24
/******************************************************************************
                        4:Define how the module works
  Module self-processing:
          The wifi indicator and wifi reset button are connected to the wifi module (turn on the WIFI_CONTROL_SELF_MODE macro)
          And correctly define WF_STATE_KEY and WF_RESET_KEY
  MCU self-processing:
          The wifi indicator and wifi reset button are connected to the MCU (turn off the WIFI_CONTROL_SELF_MODE macro)
          The MCU calls the mcu_reset_wifi() function in the mcu_api.c file where it needs to handle the reset wifi, and can call the mcu_get_reset_wifi_flag() function to return the reset wifi result
          or call the mcu_set_wifi_mode(WIFI_CONFIG_E mode) function in the mcu_api.c file in the wifi mode, and call mcu_get_wifi_work_state() to return the setting wifi result.
******************************************************************************/
//#define         WIFI_CONTROL_SELF_MODE                       //Wifi self-processing button and LED indicator; if the MCU external button / LED indicator please turn off the macro
#ifdef          WIFI_CONTROL_SELF_MODE                      //Module self-processing
#define     WF_STATE_KEY            14                    //Wifi module status indication button, please set according to the actual GPIO pin
#define     WF_RESERT_KEY           0                  //Wifi module reset button, please set according to the actual GPIO pin
#endif

/******************************************************************************
                      5: Does the MCU need to support the time function?
  Open this macro if needed and implement the code in mcu_write_rtctime in the Protocol.c file.
  Mcu_write_rtctime has #err hint inside, please delete the #err after completing the function
  Mcu can call the mcu_get_system_time() function to initiate the calibration function after the wifi module is properly networked.

******************************************************************************/
//#define         SUPPORT_MCU_RTC_CHECK                //Turn on time calibration

/******************************************************************************
                      6:Does the MCU need to support the wifi function test?
  Please enable this macro if necessary, and mcu calls mcu_start_wifitest in mcu_api.c file when wifi function test is required.
  And view the test results in the protocol_c file wifi_test_result function.
  There is a #err hint inside wifi_test_result. Please delete the #err after completing the function.

******************************************************************************/
//#define         WIFI_TEST_ENABLE                //Open WIFI production test function

/******************************************************************************
                      7:Whether to turn on the weather function
  Open this macro if needed, and in the protocol.c file weather_open_return_handle and weather_data_u\
  Ser_handle two user handlers to achieve display and other code
  #err hint in these two functions, delete the #err after completing the function

******************************************************************************/
//#define         WEATHER_ENABLE                 //Turn on weather function
#ifdef          WEATHER_ENABLE
#define         WEATHER_CHOOSE_CNT             4
#endif

/******************************************************************************
                      8:Whether to enable the WIFI module heartbeat off function
  If you need to open this macro, call the wifi_heart_stop function in the mcu_api.c file to stop the heartbeat.

******************************************************************************/
//#define         WIFI_HEARTSTOP_ENABLE           //Turn on the heartbeat stop function

/******************************************************************************
                      9:Whether to support streaming service function
******************************************************************************/
//#define         WIFI_STREAM_ENABLE              //Support for streaming service related functions
#ifdef WIFI_STREAM_ENABLE
#define         STREM_PACK_LEN                 256
#endif

/******************************************************************************
                        1:dp data point serial number redefinition
          **This is the automatic generation of code,
            such as the relevant changes in the development platform,
            please re-download MCU_SDK**
******************************************************************************/
//DigitalInput_1(只上报)
//Notes:The DigitalInput_1 reads the state of digital input number 1.
#define DPID_DIN_1 101
//DigitalInput_2(只上报)
//Notes:The DigitalInput_2 reads the state of digital input number 2.
#define DPID_DIN_2 102
//DigitalInput_3(只上报)
//Notes:The DigitalInput_3 reads the state of digital input number 3.
#define DPID_DIN_3 103
//DigitalInput_4(只上报)
//Notes:The DigitalInput_4 reads the state of digital input number 4.
#define DPID_DIN_4 104
//DigitalInput_5(只上报)
//Notes:The DigitalInput_5 reads the state of digital input number 5.
#define DPID_DIN_5 105
//AnalogInput_1(只上报)
//Notes:The AnalogInput_1 reads the value of analog input number 1.
#define DPID_AIN_1 106
//AnalogInput_2(只上报)
//Notes:The AnalogInput_2 reads the value of analog input number 2.
#define DPID_AIN_2 107
//AnalogInput_3(只上报)
//Notes:The AnalogInput_3 reads the value of analog input number 3.
#define DPID_AIN_3 108
//AnalogInput_4(只上报)
//Notes:The AnalogInput_4 reads the value of analog input number 4.
#define DPID_AIN_4 109
//AnalogInput_5(只上报)
//Notes:The AnalogInput_5 reads the value of analog input number 5.
#define DPID_AIN_5 110
//Digital Output 1(可下发可上报)
//Notes:The DigitalOutput_1 reads and writes on digital output number 1.
#define DPID_DOUT_1 111
//DigitalOutput_2(可下发可上报)
//Notes:The DigitalOutput_2 reads and writes on digital output number 2.
#define DPID_DOUT_2 112
//DigitalOutput_3(可下发可上报)
//Notes:The DigitalOutput_3 reads and writes on digital output number 3.
#define DPID_DOUT_3 113
//DigitalOutput_4(可下发可上报)
//Notes:The DigitalOutput_4 reads and writes on digital output number 4.
#define DPID_DOUT_4 114
//DigitalOutput_5(可下发可上报)
//Notes:The DigitalOutput_5 reads and writes on digital output number 5.
#define DPID_DOUT_5 115
//AnalogOutput_1(可下发可上报)
//Notes:The AnalogOutput_1 reads and writes on analog output number 1.
#define DPID_AOUT_1 116
//AnalogOutput_2(可下发可上报)
//Notes:The AnalogOutput_2 reads and writes on analog output number 2.
#define DPID_AOUT_2 117
//AnalogOutput_3(可下发可上报)
//Notes:The AnalogOutput_3 reads and writes on analog output number 3.
#define DPID_AOUT_3 118
//AValue_1(只上报)
//Notes:Value_1 is read by Tuya cloud.
#define DPID_AV1 119
//AValue_2(只上报)
//Notes:Value_2 is read by Tuya cloud.
#define DPID_AV2 120
//AValue_3(只上报)
//Notes:Value_3 is read by Tuya cloud.
#define DPID_AV3 121
//AValue_4(只上报)
//Notes:Value_4 is read by Tuya cloud.
#define DPID_AV4 122
//AValue_5(只上报)
//Notes:Value_5 is read by Tuya cloud.
#define DPID_AV5 123


/*****************************************************************************
  Function name          : all_data_update
  Functional description : All dp point information of the system is uploaded to realize APP and muc data synchronization.
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   : This function SDK needs to be called internally;
           The MCU must implement the data upload function in the function; including only reporting and reportable hair style data.
*****************************************************************************/

void all_data_update(void);

#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
  Function name          : mcu_write_rtctime
  Functional description : MCU proofreads local RTC clock
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   : MCU needs to implement this function by itself
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[]);
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
  Function name          : wifi_test_result
  Functional description : Wifi function test feedback
  Input parameters       : Result:wifi function test » fruit; 0: failure / 1: success
           rssi: test success indicates wifi signal strength / test failure indicates error type
  Return parameter       : Null
  Instructions for use   : MCU needs to implement this function by itself
*****************************************************************************/

void wifi_test_result(unsigned char result, unsigned char rssi);
#endif

#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
  Function name          : mcu_firm_update_handle
  Functional description : MCU enters firmware upgrade mode
  Input parameters       : value:Firmware buffer
                         position:The current data packet is in the firmware location
                         length:Current firmware package length (when the firmware package length is 0,
                                it indicates that the firmware package is sent)
  Return parameter       : Null
  Instructions for use   : MCU needs to implement this function by itself
*****************************************************************************/

unsigned char mcu_firm_update_handle(const unsigned char value[], unsigned long position, unsigned short length);
#endif

/*****************************************************************************
  Function name          : dp_download_handle
  Functional description : dp delivery processing function
  Input parameters       : dpid:DP number
                         value:dp data buffer address
                         length:dp data length
  Return parameter       : successful return: SUCCESS / failed return: ERRO
  Instructions for use   : The function user cannot modify
*****************************************************************************/

unsigned char dp_download_handle(unsigned char dpid, const unsigned char value[], unsigned short length);
/*****************************************************************************
  Function name          : get_download_cmd_total
  Functional description : Get the sum of all dp commands
  Input parameters       : Null
  Return parameter       : Sent the sum of the commands
  Instructions for use   : The function user cannot modify
*****************************************************************************/

unsigned char get_download_cmd_total(void);

#ifdef WEATHER_ENABLE
extern const char weather_choose[WEATHER_CHOOSE_CNT][10];

void weather_open_return_handle(unsigned char res, unsigned char err);
void weather_data_user_handle(char *name, unsigned char type, char *data);
#endif

#endif
