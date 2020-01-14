/****************************************Copyright (c)*************************
**                           All rights reserved (C), 2015-2020, Tuya
**
**                                 http://www.tuya.com
**
**--------------File information-------------------------------------------------------
**File  name : mcu_api.c
**Description: Deliver/report data processing function
**Instructions for use : The function under this file does not need to be modified by the user.
                         The files that the user needs to call are in the file.
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
#define MCU_API_GLOBAL

#include "wifi.h"

/*****************************************************************************
  Function name          : hex_to_bcd
  Functional description : hex to bcd
  Input parameters       : Value_H:High Byte / Value_L:Low Byte
  Return parameter       : bcd_value:Converted data
*****************************************************************************/
unsigned char hex_to_bcd(unsigned char Value_H, unsigned char Value_L)
{
  unsigned char bcd_value;

  if ((Value_H >= '0') && (Value_H <= '9'))
    Value_H -= '0';
  else if ((Value_H >= 'A') && (Value_H <= 'F'))
    Value_H = Value_H - 'A' + 10;
  else if ((Value_H >= 'a') && (Value_H <= 'f'))
    Value_H = Value_H - 'a' + 10;

  bcd_value = Value_H & 0x0f;

  bcd_value <<= 4;
  if ((Value_L >= '0') && (Value_L <= '9'))
    Value_L -= '0';
  else if ((Value_L >= 'A') && (Value_L <= 'F'))
    Value_L = Value_L - 'a' + 10;
  else if ((Value_L >= 'a') && (Value_L <= 'f'))
    Value_L = Value_L - 'a' + 10;

  bcd_value |= Value_L & 0x0f;

  return bcd_value;
}
/*****************************************************************************
  Function name          : my_strlen
  Functional description : Calculate string length
  Input parameters       : src:source address
  Return parameter       : len:data length
*****************************************************************************/
unsigned long my_strlen(unsigned char *str)
{
  unsigned long len = 0;
  if (str == NULL)
  {
    return 0;
  }

  for (len = 0; *str ++ != '\0'; )
  {
    len ++;
  }

  return len;
}
/*****************************************************************************
  Function name          : my_memset
  Functional description : Set the first count bytes of the memory area pointed to by src to the character c
  Input parameters       : src:source address
                         ch:Set character
                         count:Set the data length
  Return parameter       : src:Source address after data processing
*****************************************************************************/
void *my_memset(void *src, unsigned char ch, unsigned short count)
{
  unsigned char *tmp = (unsigned char *)src;

  if (src == NULL)
  {
    return NULL;
  }

  while (count --)
  {
    *tmp ++ = ch;
  }

  return src;
}
/*****************************************************************************
  Function name          : mymemcpy
  Functional description : Memory copy
  Input parameters       : dest:target address
                         src:source address
                         count:number of data copies
  Return parameter       : src:Source address after data processing
*****************************************************************************/
void *my_memcpy(void *dest, const void *src, unsigned short count)
{
  unsigned char *pdest = (unsigned char *)dest;
  const unsigned char *psrc  = (const unsigned char *)src;
  unsigned short i;

  if (dest == NULL || src == NULL)
  {
    return NULL;
  }

  if ((pdest <= psrc) || (pdest > psrc + count))
  {
    for (i = 0; i < count; i ++)
    {
      pdest[i] = psrc[i];
    }
  }
  else
  {
    for (i = count; i > 0; i --)
    {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}
/*****************************************************************************
  Function name          : my_strcpy
  Functional description : Memory copy
  Input parameters       : s1:target address
                         s2:source address
  Return parameter       : Source address after data processing
*****************************************************************************/
char *my_strcpy(char *dest, const char *src)
{
  char *p = dest;
  while (*src != '\0')
  {
    *dest++ = *src++;
  }
  *dest = '\0';
  return p;
}
/*****************************************************************************
  Function name          : my_strcmp
  Functional description : Memory copy
  Input parameters       : s1:String 1
                         s2:String 2
  Return parameter       : Size comparison value，0:s1=s2; -1:s1<s2; 1:s1>s2
*****************************************************************************/
int my_strcmp(char *s1 , char *s2)
{
  while ( *s1 && *s2 && *s1 == *s2 )
  {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}
/*****************************************************************************
  Function name          : int_to_byte
  Functional description : Split the int type into four bytes
  Input parameters       : number:4 bytes of original data;
                         value:4 bytes of data after processing is completed
  Return parameter       :Null
****************************************************************************/
void int_to_byte(unsigned long number, unsigned char value[4])
{
  value[0] = number >> 24;
  value[1] = number >> 16;
  value[2] = number >> 8;
  value[3] = number & 0xff;
}
/*****************************************************************************
  Function name          : byte_to_int
  Functional description : Combine 4 bytes into 1 32bit variable
  Input parameters       : value:4-byte array
  Return parameter       : number:32bit variable after the merge is completed
****************************************************************************/
unsigned long byte_to_int(const unsigned char value[4])
{
  unsigned long nubmer = 0;

  nubmer = (unsigned long)value[0];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[1];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[2];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[3];

  return nubmer;
}
#ifndef WIFI_CONTROL_SELF_MODE
/*****************************************************************************
  Function name          : mcu_get_reset_wifi_flag
  Functional description : MCU gets reset wifi success flag
  Input parameters       : Null
  Return parameter       : Reset flag:RESET_WIFI_ERROR:failure/RESET_WIFI_SUCCESS:success
  Instructions for use   : 1:The MCU actively calls mcu_reset_wifi() and calls this function to get the reset state.
                         2:If the module is in self-processing mode, the MCU does not need to call this function.
*****************************************************************************/
unsigned char mcu_get_reset_wifi_flag(void)
{
  return reset_wifi_flag;
}
/*****************************************************************************
  Function name          : reset_wifi
  Functional description : MCU actively resets wifi working mode
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   : 1:The MCU actively calls to obtain whether the reset wifi is successful through the mcu_get_reset_wifi_flag() function.
                         2:If the module is in self-processing mode, the MCU does not need to call this function.
*****************************************************************************/
void mcu_reset_wifi(void)
{
  reset_wifi_flag = RESET_WIFI_ERROR;

  wifi_uart_write_frame(WIFI_RESET_CMD, 0);
}
/*****************************************************************************
  Function name          : mcu_get_wifimode_flag
  Functional description : Get set wifi status success flag
  Input parameters       : Null
  Return parameter       : SET_WIFICONFIG_ERROR:failure /SET_WIFICONFIG_SUCCESS:success
  Instructions for use   : 1:The MCU actively calls mcu_set_wifi_mode() and calls this function to get the reset state.
                         2:If the module is in self-processing mode, the MCU does not need to call this function.
*****************************************************************************/
unsigned char mcu_get_wifimode_flag(void)
{
  return set_wifimode_flag;
}
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
void mcu_set_wifi_mode(unsigned char mode)
{
  unsigned char length = 0;

  set_wifimode_flag = SET_WIFICONFIG_ERROR;

  length = set_wifi_uart_byte(length, mode);

  wifi_uart_write_frame(WIFI_MODE_CMD, length);
}
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
unsigned char mcu_get_wifi_work_state(void)
{
  return wifi_work_state;
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
  Function name          : mcu_get_system_time
  Functional description : MCU gets system time for proofreading local clock
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   :The rtc clock is checked in the mcu_write_rtctime function after the MCU is actively called.
*****************************************************************************/
void mcu_get_system_time(void)
{
  wifi_uart_write_frame(GET_LOCAL_TIME_CMD, 0);
}
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
  Return parameter       : mcu_start_wifitest
  Functional description : Mcu initiated wifi function test
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   :The MCU needs to call this function by itself.
*****************************************************************************/
void mcu_start_wifitest(void)
{
  wifi_uart_write_frame(WIFI_TEST_CMD, 0);
}
#endif

#ifdef WIFI_HEARTSTOP_ENABLE
/*****************************************************************************
  Return parameter       : wifi_heart_stop
  Functional description : Notify the WIFI module to turn off the heartbeat
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
void wifi_heart_stop(void)
{
  wifi_uart_write_frame(HEAT_BEAT_STOP, 0);
}
#endif

/*****************************************************************************
  Function name          : mcu_dp_raw_update
  Functional description : raw type dp data upload
  Input parameters       : dpid:id number
                         value:Current dp value pointer
                         len:data length
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_raw_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
  unsigned short length = 0;

  if (stop_update_flag == ENABLE)
    return SUCCESS;
  //
  length = set_wifi_uart_byte(length, dpid);
  length = set_wifi_uart_byte(length, DP_TYPE_RAW);
  //
  length = set_wifi_uart_byte(length, len / 0x100);
  length = set_wifi_uart_byte(length, len % 0x100);
  //
  length = set_wifi_uart_buffer(length, (unsigned char *)value, len);

  wifi_uart_write_frame(STATE_UPLOAD_CMD, length);

  return SUCCESS;
}
/*****************************************************************************
  Function name          : mcu_dp_bool_update
  Functional description : bool type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_bool_update(unsigned char dpid, unsigned char value)
{
  unsigned short length = 0;

  if (stop_update_flag == ENABLE)
    return SUCCESS;

  length = set_wifi_uart_byte(length, dpid);
  length = set_wifi_uart_byte(length, DP_TYPE_BOOL);
  //
  length = set_wifi_uart_byte(length, 0);
  length = set_wifi_uart_byte(length, 1);
  //
  if (value == FALSE)
  {
    length = set_wifi_uart_byte(length, FALSE);
  }
  else
  {
    length = set_wifi_uart_byte(length, 1);
  }

  wifi_uart_write_frame(STATE_UPLOAD_CMD, length);

  return SUCCESS;
}
/*****************************************************************************
  Function name          : mcu_dp_value_update
  Functional description : value type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_value_update(unsigned char dpid, unsigned long value)
{
  unsigned short length = 0;

  if (stop_update_flag == ENABLE)
    return SUCCESS;

  length = set_wifi_uart_byte(length, dpid);
  length = set_wifi_uart_byte(length, DP_TYPE_VALUE);
  //
  length = set_wifi_uart_byte(length, 0);
  length = set_wifi_uart_byte(length, 4);
  //
  length = set_wifi_uart_byte(length, value >> 24);
  length = set_wifi_uart_byte(length, value >> 16);
  length = set_wifi_uart_byte(length, value >> 8);
  length = set_wifi_uart_byte(length, value & 0xff);

  wifi_uart_write_frame(STATE_UPLOAD_CMD, length);

  return SUCCESS;
}
/*****************************************************************************
  Function name          : mcu_dp_string_update
  Functional description : rstring type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value pointer
                         len:data length
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_string_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
  unsigned short length = 0;

  if (stop_update_flag == ENABLE)
    return SUCCESS;
  //
  length = set_wifi_uart_byte(length, dpid);
  length = set_wifi_uart_byte(length, DP_TYPE_STRING);
  //
  length = set_wifi_uart_byte(length, len / 0x100);
  length = set_wifi_uart_byte(length, len % 0x100);
  //
  length = set_wifi_uart_buffer(length, (unsigned char *)value, len);

  wifi_uart_write_frame(STATE_UPLOAD_CMD, length);

  return SUCCESS;
}
/*****************************************************************************
  Function name          : mcu_dp_enum_update
  Functional description : enum type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_enum_update(unsigned char dpid, unsigned char value)
{
  unsigned short length = 0;

  if (stop_update_flag == ENABLE)
    return SUCCESS;

  length = set_wifi_uart_byte(length, dpid);
  length = set_wifi_uart_byte(length, DP_TYPE_ENUM);
  //
  length = set_wifi_uart_byte(length, 0);
  length = set_wifi_uart_byte(length, 1);
  //
  length = set_wifi_uart_byte(length, value);

  wifi_uart_write_frame(STATE_UPLOAD_CMD, length);

  return SUCCESS;
}
/*****************************************************************************
  Function name          : mcu_dp_fault_update
  Functional description : fault type dp data upload
  Input parameters       : dpid:id number
                         value:current dp value
  Return parameter       : Null
*****************************************************************************/
unsigned char mcu_dp_fault_update(unsigned char dpid, unsigned long value)
{
  unsigned short length = 0;

  if (stop_update_flag == ENABLE)
    return SUCCESS;

  length = set_wifi_uart_byte(length, dpid);
  length = set_wifi_uart_byte(length, DP_TYPE_FAULT);
  //
  length = set_wifi_uart_byte(length, 0);

  if ((value | 0xff) == 0xff)
  {
    length = set_wifi_uart_byte(length, 1);
    length = set_wifi_uart_byte(length, value);
  }
  else if ((value | 0xffff) == 0xffff)
  {
    length = set_wifi_uart_byte(length, 2);
    length = set_wifi_uart_byte(length, value >> 8);
    length = set_wifi_uart_byte(length, value & 0xff);
  }
  else
  {
    length = set_wifi_uart_byte(length, 4);
    length = set_wifi_uart_byte(length, value >> 24);
    length = set_wifi_uart_byte(length, value >> 16);
    length = set_wifi_uart_byte(length, value >> 8);
    length = set_wifi_uart_byte(length, value & 0xff);
  }

  wifi_uart_write_frame(STATE_UPLOAD_CMD, length);

  return SUCCESS;
}
/*****************************************************************************
  Function name          : mcu_get_dp_download_bool
  Functional description : mcu gets bool type to send dp value
  Input parameters       : value:dp data buffer address
                         length:dp data length
  Return parameter       : bool:current dp value
*****************************************************************************/
unsigned char mcu_get_dp_download_bool(const unsigned char value[], unsigned short len)
{
  return (value[0]);
}
/*****************************************************************************
  Function name          : mcu_get_dp_download_enum
  Functional description : mcu get enum type to send dp value
  Input parameters       : value:dp data buffer address
                         length:dp data length
  Return parameter       : enum:current dp value
*****************************************************************************/
unsigned char mcu_get_dp_download_enum(const unsigned char value[], unsigned short len)
{
  return (value[0]);
}
/*****************************************************************************
  Function name          : mcu_get_dp_download_value
  Functional description : Mcu gets the value type to send dp value
  Input parameters       : value:dp data buffer address
                         length:dp data length
  Return parameter       : value:current dp value
*****************************************************************************/
unsigned long mcu_get_dp_download_value(const unsigned char value[], unsigned short len)
{
  return (byte_to_int(value));
}
/*****************************************************************************
  Function name          : uart_receive_input
  Functional description : Receive data processing
  Input parameters       : value:Serial port receives byte data
  Return parameter       : Null
  Instructions for use   :Call this function in the MCU serial receive function and pass the received data as a parameter.
*****************************************************************************/
void uart_receive_input(unsigned char value)
{
  //  #error "Please call uart_receive_input(value) in the serial port receive interrupt. The serial port data is processed by MCU_SDK. The user should not process it separately. Delete the line after completion."

  if ((queue_in > queue_out) && ((queue_in - queue_out) >= sizeof(wifi_queue_buf)))
  {
    //Data queue full
  }
  else if ((queue_in < queue_out) && ((queue_out  - queue_in) == 0))
  {
    //Data queue full
  }
  else
  {
    //Queue is not full
    if (queue_in >= (unsigned char *)(wifi_queue_buf + sizeof(wifi_queue_buf)))
    {
      queue_in = (unsigned char *)(wifi_queue_buf);
    }

    *queue_in ++ = value;
  }
}
/*****************************************************************************
  Function name          : wifi_uart_service
  Functional description : Wifi serial port processing service
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   :Call this function in the MCU main function while loop
*****************************************************************************/
void wifi_uart_service(void)
{
  // #error "Please add wifi_uart_service() directly in the while(1){} of the main function, call this function without adding any conditional judgment, delete the line after completion."
  static unsigned short rx_in = 0;
  unsigned short offset = 0;
  unsigned short rx_value_len = 0;             //Data frame length

  while ((rx_in < sizeof(wifi_uart_rx_buf)) && get_queue_total_data() > 0)
  {
    wifi_uart_rx_buf[rx_in ++] = Queue_Read_Byte();
  }

  if (rx_in < PROTOCOL_HEAD)
    return;

  while ((rx_in - offset) >= PROTOCOL_HEAD)
  {
    if (wifi_uart_rx_buf[offset + HEAD_FIRST] != FRAME_FIRST)
    {
      offset ++;
      continue;
    }

    if (wifi_uart_rx_buf[offset + HEAD_SECOND] != FRAME_SECOND)
    {
      offset ++;
      continue;
    }

    if (wifi_uart_rx_buf[offset + PROTOCOL_VERSION] != VERSION)
    {
      offset += 2;
      continue;
    }

    rx_value_len = wifi_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
    rx_value_len += (wifi_uart_rx_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);
    if (rx_value_len > sizeof(wifi_uart_rx_buf) + PROTOCOL_HEAD)
    {
      offset += 3;
      continue;
    }

    if ((rx_in - offset) < rx_value_len)
    {
      break;
    }

    //Data reception completed
    if (get_check_sum((unsigned char *)wifi_uart_rx_buf + offset, rx_value_len - 1) != wifi_uart_rx_buf[offset + rx_value_len - 1])
    {
      //Verification error
      offset += 3;
      continue;
    }

    data_handle(offset);
    offset += rx_value_len;
  }//end while

  rx_in -= offset;
  if (rx_in > 0)
  {
    my_memcpy((char *)wifi_uart_rx_buf, (const char *)wifi_uart_rx_buf + offset, rx_in);
  }
}
/*****************************************************************************
  Function name          : wifi_protocol_init
  Functional description : Protocol serial port initialization function
  Input parameters       : Null
  Return parameter       : Null
  Instructions for use   :This function must be called in the MCU initialization code
*****************************************************************************/
void wifi_protocol_init(void)
{
  // #error " Please add wifi_protocol_init() in the main function to complete the wifi protocol initialization and delete the line."
  queue_in = (unsigned char *)wifi_queue_buf;
  queue_out = (unsigned char *)wifi_queue_buf;
  //
#ifndef WIFI_CONTROL_SELF_MODE
  wifi_work_state = WIFI_SATE_UNKNOW;
#endif
}

#ifdef WIFI_STREAM_ENABLE
/*****************************************************************************
  Function name          : stream_open
  Functional description : Turn on streaming service
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
unsigned char stream_open(void)
{
  stream_status = 0xff;

  if (stop_update_flag == ENABLE)
    return SUCCESS;

  wifi_uart_write_frame(STREAM_OPEN_CMD, 0);
}

/*****************************************************************************
  Function name          : stream_start
  Functional description : Turn on streaming data transmission
  Input parameters       : id:Stream service identifier
  Return parameter       : Null
*****************************************************************************/
unsigned char stream_start(unsigned int id)
{
  unsigned short length = 0;

  stream_status = 0xff;

  if (stop_update_flag == ENABLE)
    return SUCCESS;

  //ID
  length = set_wifi_uart_byte(length, id / 0x100);
  length = set_wifi_uart_byte(length, id % 0x100);

  wifi_uart_write_frame(STREAM_START_CMD, length);
}

/*****************************************************************************
  Function name          : stream_trans
  Functional description : Stream data transmission
  Input parameters       : id:Stream service identifier;
                         offset:Offset;
                         buffer:Data address;
                         buf_len:Data length
  Return parameter       : Null
*****************************************************************************/
unsigned char stream_trans(unsigned int id, unsigned char offset, unsigned char *buffer, unsigned long buf_len)
{
  unsigned short length = 0;

  stream_status = 0xff;

  if (stop_update_flag == ENABLE)
    return SUCCESS;

  //ID
  length = set_wifi_uart_byte(length, id / 0x100);
  length = set_wifi_uart_byte(length, id % 0x100);
  //Offset
  length = set_wifi_uart_byte(length, offset >> 24);
  length = set_wifi_uart_byte(length, offset >> 16);
  length = set_wifi_uart_byte(length, offset >> 8);
  length = set_wifi_uart_byte(length, offset % 256);
  //data
  length = set_wifi_uart_buffer(length, buffer, buf_len);
  wifi_uart_write_frame(STREAM_TRANS_CMD, length);
}

/*****************************************************************************
  Function name          : stream_close
  Functional description : End stream data transfer
  Input parameters       : id:Stream service identifier;offset:Offset
  Return parameter       : Null
*****************************************************************************/
unsigned char stream_stop(unsigned char id, unsigned long offset)
{
  unsigned short length = 0;

  stream_status = 0xff;

  if (stop_update_flag == ENABLE)
    return SUCCESS;

  //ID
  length = set_wifi_uart_byte(length, id / 0x100);
  length = set_wifi_uart_byte(length, id % 0x100);

  //Offset
  length = set_wifi_uart_byte(length, offset >> 24);
  length = set_wifi_uart_byte(length, offset >> 16);
  length = set_wifi_uart_byte(length, offset >> 8);
  length = set_wifi_uart_byte(length, offset % 256);

  //Data
  wifi_uart_write_frame(STREAM_STOP_CMD, length);
}
#endif
