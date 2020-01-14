/****************************************Copyright (c)*************************
**                          All rights reserved (C), 2015-2020, Tuya
**
**                                 http://www.tuya.com
**
**--------------File information-------------------------------------------------------
**File name   : mcu_api.h
**description : Deliver/report data processing function
**Instructions for use : The function under this file does not need to be modified by the user.
                         The files that the user needs to call are in the file
**
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
#ifndef __MCU_API_H_
#define __MCU_API_H_


#ifdef MCU_API_GLOBAL
#define MCU_API_EXTERN
#else
#define MCU_API_EXTERN   extern
#endif

/*****************************************************************************
  Function name          : hex_to_bcd
  Functional description : hex to bcd
  Input parameters       : Value_H:High Byte / Value_L:Low Byte
  Return parameter       : bcd_value:Converted data
*****************************************************************************/
unsigned char hex_to_bcd(unsigned char Value_H, unsigned char Value_L);
/*****************************************************************************
  Function name          : my_strlen
  Functional description : Calculate string length
  Input parameters       : src:source address
  Return parameter       : len:data length
*****************************************************************************/
unsigned long my_strlen(unsigned char *str);

/*****************************************************************************
  Function name          : my_memset
  Functional description : Set the first count bytes of the memory area pointed to by src to the character c
  Input parameters       : src:source address
                         ch:Set character
                         count:Set the data length
  Return parameter       : src:Source address after data processing
*****************************************************************************/
void *my_memset(void *src, unsigned char ch, unsigned short count);

/*****************************************************************************
  Function name          : mymemcpy
  Functional description : Memory copy
  Input parameters       : dest:target address
                         src:source address
                         count:number of data copies
  Return parameter       : src:Source address after data processing
*****************************************************************************/
void *my_memcpy(void *dest, const void *src, unsigned short count);

/*****************************************************************************
  Function name          : my_strcpy
  Functional description : Memory copy
  Input parameters       : s1:target address
                         s2:source address
  Return parameter       : Source address after data processing
*****************************************************************************/
char *my_strcpy(char *dest, const char *src);

/*****************************************************************************
  Function name          : my_strcmp
  Functional description : Memory copy
  Input parameters       : s1:String 1
                         s2:String 2
  Return parameter       : Size comparison value，0:s1=s2; -1:s1<s2; 1:s1>s2
*****************************************************************************/
int my_strcmp(char *s1 , char *s2);

/*****************************************************************************
  Function name          : int_to_byte
  Functional description : Split the int type into four bytes
  Input parameters       : number:4 bytes of original data;
                         value:4 bytes of data after processing is completed
  Return parameter       :Null
****************************************************************************/
void int_to_byte(unsigned long number, unsigned char value[4]);

/*****************************************************************************
  Function name          : byte_to_int
  Functional description : Combine 4 bytes into 1 32bit variable
  Input parameters       : value:4-byte array
  Return parameter       : number:32bit variable after the merge is completed
****************************************************************************/
unsigned long byte_to_int(const unsigned char value[4]);

#ifndef WIFI_CONTROL_SELF_MODE
/*****************************************************************************
  Function name          : mcu_get_reset_wifi_flag
  Functional description : MCU gets reset wifi success flag
  Input parameters       : Null
  Return parameter       : Reset flag:RESET_WIFI_ERROR:failure/RESET_WIFI_SUCCESS:success
  Instructions for use   : 1:The MCU actively calls mcu_reset_wifi() and calls this function to get the reset state.
                         2:If the module is in self-processing mode, the MCU does not need to call this function.
*****************************************************************************/
unsigned char mcu_get_reset_wifi_flag(void);
/*****************************************************************************
  Function name          : reset_wifi
  Functional description : MCU actively resets wifi working mode
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   : 1:The MCU actively calls to obtain whether the reset wifi is successful through the mcu_get_reset_wifi_flag() function.
                         2:If the module is in self-processing mode, the MCU does not need to call this function.
*****************************************************************************/
void mcu_reset_wifi(void);
/*****************************************************************************
  Function name          : mcu_get_wifimode_flag
  Functional description : Get set wifi status success flag
  Input parameters       : Null
  Return parameter       : SET_WIFICONFIG_ERROR:failure /SET_WIFICONFIG_SUCCESS:success
  Instructions for use   : 1:The MCU actively calls mcu_set_wifi_mode() and calls this function to get the reset state.
                         2:If the module is in self-processing mode, the MCU does not need to call this function.
*****************************************************************************/
unsigned char mcu_get_wifimode_flag(void);
/*****************************************************************************
  Function name          : mcu_set_wifi_mode
  Functional description : MCU set wifi working mode
  Input parameters       : mode:
                         SMART_CONFIG:enter smartconfig mode
                         AP_CONFIG:enter AP mode
  Return parameter       : Null
  Instructions for use   : 1:MCU active call
                         2:After success, it can be judged whether set_wifi_config_state is TRUE;
                           TRUE means successful setting of wifi working mode
                         3:If the module is in self-processing mode, the MCU does not need to call this function.
*****************************************************************************/
void mcu_set_wifi_mode(unsigned char mode);
/*****************************************************************************
  Function name          : mcu_get_wifi_work_state
  Functional description : The MCU actively obtains the current wifi working status.
  Input parameters       : Null
  Return parameter       : WIFI_WORK_SATE_E:
                         SMART_CONFIG_STATE:smartconfig config status
                         AP_STATE:AP config status
                         WIFI_NOT_CONNECTED:WIFI configuration succeeded but not connected to the router
                         WIFI_CONNECTED:WIFI configuration is successful and connected to the router
                         WIFI_CONN_CLOUD:WIFI is connected to the cloud server
                         WIFI_LOW_POWER:WIFI is in low power mode
  Instructions for use   : Null
*****************************************************************************/
unsigned char mcu_get_wifi_work_state(void);
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
  Function name          : mcu_get_system_time
  Functional description : MCU gets system time for proofreading local clock
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   :The rtc clock is checked in the mcu_write_rtctime function after the MCU is actively called.
*****************************************************************************/
void mcu_get_system_time(void);
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
  Return parameter       : mcu_start_wifitest
  Functional description : Mcu initiated wifi function test
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   :The MCU needs to call this function by itself.
*****************************************************************************/
void mcu_start_wifitest(void);
#endif

#ifdef WIFI_HEARTSTOP_ENABLE
/*****************************************************************************
  Return parameter       : wifi_heart_stop
  Functional description : Notify the WIFI module to turn off the heartbeat
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
void wifi_heart_stop(void);
#endif

#ifdef WIFI_STREAM_ENABLE
/*****************************************************************************
  Function name          : stream_open
  Functional description : Turn on streaming service
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
unsigned char stream_open(void);

/*****************************************************************************
  Function name          : stream_start
  Functional description : Turn on streaming data transmission
  Input parameters       : id:Stream service identifier
  Return parameter       : Null
*****************************************************************************/
unsigned char stream_start(unsigned int id);

/*****************************************************************************
  Function name          : stream_trans
  Functional description : Stream data transmission
  Input parameters       : id:Stream service identifier;
                         offset:Offset;
                         buffer:Data address;
                         buf_len:Data length
  Return parameter       : Null
*****************************************************************************/

unsigned char stream_trans(unsigned int id, unsigned char offset, unsigned char *buffer, unsigned long buf_len);

/*****************************************************************************
  Function name          : stream_close
  Functional description : End stream data transfer
  Input parameters       : id:Stream service identifier;offset:Offset
  Return parameter       : Null
*****************************************************************************/

unsigned char stream_stop(unsigned char id, unsigned long offset);
#endif

/*****************************************************************************
  Function name          : mcu_dp_raw_update
  Functional description : raw type dp data upload
  Input parameters       : dpid:id number
                         value:Current dp value pointer
                         len:data length
  Return parameter       : Null
*****************************************************************************/

unsigned char mcu_dp_raw_update(unsigned char dpid, const unsigned char value[], unsigned short len);

/*****************************************************************************
  Function name          : mcu_dp_bool_update
  Functional description : bool type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_bool_update(unsigned char dpid, unsigned char value);

/*****************************************************************************
  Function name          : mcu_dp_value_update
  Functional description : value type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_value_update(unsigned char dpid, unsigned long value);

/*****************************************************************************
  Function name          : mcu_dp_string_update
  Functional description : rstring type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value pointer
                         len:data length
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_string_update(unsigned char dpid, const unsigned char value[], unsigned short len);

/*****************************************************************************
  Function name          : mcu_dp_enum_update
  Functional description : enum type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_enum_update(unsigned char dpid, unsigned char value);

/*****************************************************************************
  Function name          : mcu_dp_fault_update
  Functional description : fault type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_fault_update(unsigned char dpid, unsigned long value);

/*****************************************************************************
  Function name          : mcu_get_dp_download_bool
  Functional description : mcu gets bool type to send dp value
  Input parameters       : value:dp data buffer address
                         length:dp data length
  Return parameter       : bool:current dp value
*****************************************************************************/

unsigned char mcu_get_dp_download_bool(const unsigned char value[], unsigned short len);

/*****************************************************************************
  Function name          : mcu_get_dp_download_enum
  Functional description : mcu get enum type to send dp value
  Input parameters       : value:dp data buffer address
                         length:dp data length
  Return parameter       : enum:current dp value
*****************************************************************************/
unsigned long mcu_get_dp_download_value(const unsigned char value[], unsigned short len);

/*****************************************************************************
  Function name          : mcu_get_dp_download_value
  Functional description : Mcu gets the value type to send dp value
  Input parameters       : value:dp data buffer address
                         length:dp data length
  Return parameter       : value:current dp value
*****************************************************************************/

unsigned char mcu_get_dp_download_enum(const unsigned char value[], unsigned short len);

/*****************************************************************************
  Function name          : uart_receive_input
  Functional description : Receive data processing
  Input parameters       : value:Serial port receives byte data
  Return parameter       : Null
  Instructions for use   :Call this function in the MCU serial receive function and pass the received data as a parameter.
*****************************************************************************/

void uart_receive_input(unsigned char value);

/*****************************************************************************
  Function name          : wifi_uart_service
  Functional description : Wifi serial port processing service
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   :Call this function in the MCU main function while loop
*****************************************************************************/

void wifi_uart_service(void);

/*****************************************************************************
  Function name          : wifi_protocol_init
  Functional description : Protocol serial port initialization function
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   :This function must be called in the MCU initialization code
*****************************************************************************/

void wifi_protocol_init(void);
/*****************************************************************************
  Function name          : uart_transmit_output
  Functional description : Send data processing
  Input parameters       : value:Serial port receives byte data
  Return parameter       : Null
  Instructions for use   : Please fill in the MCU serial port send function into the function,
                         and pass the received data as a parameter to the serial port send function.
*****************************************************************************/
void uart_transmit_output(unsigned char value);

#endif
