/****************************************Copyright (c)*************************
**                           All rights reserved (C), 2015-2020, Tuya
**
**                                 http://www.tuya.com
**
**--------------File information-------------------------------------------------------
**File name   : system.c
**Description : Wifi data processing function
**Instructions for use : Users don't need to care about the file implementation content
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
#define SYSTEM_GLOBAL

#include "wifi.h"
#include "protocol.h"
//
//
extern const DOWNLOAD_CMD_S download_cmd[];

/*****************************************************************************
  Function name          : set_wifi_uart_byte
  Functional description : Write wifi_uart byte
  Input parameters       : dest: the actual address of the buffer area;
                         byte: write byte value
  Return parameter       : Total length after writing is completed
*****************************************************************************/
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte)
{
  unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;

  *obj = byte;
  dest += 1;

  return dest;
}
/*****************************************************************************
  Function name          : set_wifi_uart_buffer
  Functional description : write wifi_uart_buffer
  Input parameters       : dest:target address
                         src:source address
                         len:Data length
  Return parameter       : Null
*****************************************************************************/
unsigned short set_wifi_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len)
{
  unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;

  my_memcpy(obj, src, len);

  dest += len;
  return dest;
}
/*****************************************************************************
  Function name          : wifi_uart_write_data
  Functional description : Write continuous data to wifi uart
  Input parameters       : in:Send buffer pointer
                         len:Data transmission length
  Return parameter       : Null
*****************************************************************************/
static void wifi_uart_write_data(unsigned char *in, unsigned short len)
{
  if ((NULL == in) || (0 == len))
  {
    return;
  }

  while (len --)
  {
    uart_transmit_output(*in);
    in ++;
  }
}
/*****************************************************************************
  Function name          : get_check_sum
  Functional description : Calculate checksum
  Input parameters       : pack:Data source pointer
                         pack_len:Need to calculate the length of the checksum data
  Return parameter       : checksum
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
  unsigned short i;
  unsigned char check_sum = 0;

  for (i = 0; i < pack_len; i ++)
  {
    check_sum += *pack ++;
  }

  return check_sum;
}
/*****************************************************************************
  Function name          : wifi_uart_write_frame
  Functional description : Send a frame of data to the wifi serial port
  Input parameters       : fr_type:Frame type
                         len:Data length
  Return parameter       : Null
*****************************************************************************/
void wifi_uart_write_frame(unsigned char fr_type, unsigned short len)
{
  unsigned char check_sum = 0;

  wifi_uart_tx_buf[HEAD_FIRST] = 0x55;
  wifi_uart_tx_buf[HEAD_SECOND] = 0xaa;
  wifi_uart_tx_buf[PROTOCOL_VERSION] = 0x03;
  wifi_uart_tx_buf[FRAME_TYPE] = fr_type;
  wifi_uart_tx_buf[LENGTH_HIGH] = len >> 8;
  wifi_uart_tx_buf[LENGTH_LOW] = len & 0xff;

  len += PROTOCOL_HEAD;
  check_sum = get_check_sum((unsigned char *)wifi_uart_tx_buf, len - 1);
  wifi_uart_tx_buf[len - 1] = check_sum;
  //
  wifi_uart_write_data((unsigned char *)wifi_uart_tx_buf, len);
}
/*****************************************************************************
  Function name          : heat_beat_check
  Functional description : Heartbeat packet detection
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
static void heat_beat_check(void)
{
  unsigned char length = 0;
  static unsigned char mcu_reset_state = FALSE;

  if (FALSE == mcu_reset_state)
  {
    length = set_wifi_uart_byte(length, FALSE);
    mcu_reset_state = TRUE;
  }
  else
  {
    length = set_wifi_uart_byte(length, TRUE);
  }

  wifi_uart_write_frame(HEAT_BEAT_CMD, length);
}
/*****************************************************************************
  Function name          : product_info_update
  Functional description : Product information upload
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
static void product_info_update(void)
{
  unsigned char length = 0;

  length = set_wifi_uart_buffer(length, "{\"p\":\"", my_strlen("{\"p\":\""));
  length = set_wifi_uart_buffer(length, (unsigned char *)PRODUCT_KEY, my_strlen((unsigned char *)PRODUCT_KEY));
  length = set_wifi_uart_buffer(length, "\",\"v\":\"", my_strlen("\",\"v\":\""));
  length = set_wifi_uart_buffer(length, (unsigned char *)MCU_VER, my_strlen((unsigned char *)MCU_VER));
  length = set_wifi_uart_buffer(length, "\",\"m\":", my_strlen("\",\"m\":"));
  length = set_wifi_uart_buffer(length, (unsigned char *)CONFIG_MODE, my_strlen((unsigned char *)CONFIG_MODE));
  length = set_wifi_uart_buffer(length, "}", my_strlen("}"));

  wifi_uart_write_frame(PRODUCT_INFO_CMD, length);
}
/*****************************************************************************
  Function name          : get_mcu_wifi_mode
  Functional description : Query the working mode of mcu and wifi
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
static void get_mcu_wifi_mode(void)
{
  unsigned char length = 0;

#ifdef WIFI_CONTROL_SELF_MODE                                   //Module self-processing
  length = set_wifi_uart_byte(length, WF_STATE_KEY);
  length = set_wifi_uart_byte(length, WF_RESERT_KEY);
#else
  //No need to process data
#endif

  wifi_uart_write_frame(WORK_MODE_CMD, length);
}
/*****************************************************************************
  Function name          : get_update_dpid_index
  Functional description : Get the serial number of the DPID in the array
  Input parameters       : dpid:dpid
  Return parameter       : index:dp number
*****************************************************************************/
static unsigned char get_dowmload_dpid_index(unsigned char dpid)
{
  unsigned char index;
  unsigned char total = get_download_cmd_total();

  for (index = 0; index < total; index ++)
  {
    if (download_cmd[index].dp_id == dpid)
    {
      break;
    }
  }

  return index;
}
/*****************************************************************************
  Function name          : data_point_handle
  Functional description : Delivery data processing
  Input parameters       : value:Send data source pointer
  Return parameter       : ret:Return data processing result
*****************************************************************************/
static unsigned char data_point_handle(const unsigned char value[])
{
  unsigned char dp_id, index;
  unsigned char dp_type;
  unsigned char ret;
  unsigned short dp_len;

  dp_id = value[0];
  dp_type = value[1];
  dp_len = value[2] * 0x100;
  dp_len += value[3];

  index = get_dowmload_dpid_index(dp_id);

  if (dp_type != download_cmd[index].dp_type)
  {
    //Error message
    return FALSE;
  }
  else
  {
    ret = dp_download_handle(dp_id, value + 4, dp_len);
  }

  return ret;
}

#ifdef WEATHER_ENABLE
/*****************************************************************************
  Function name          : data_point_handle
  Functional description : Delivery data processing
  Input parameters       : value:Send data source pointer
  Return parameter       : ret:Return data processing result
*****************************************************************************/
void mcu_open_weather(void)
{
  int i = 0;
  char temp[10], buffer[13] = "xw.xxxxxxxxxx";
  char result[WEATHER_CHOOSE_CNT * 13 + 8];
  int buf_len = 0, now_buf_len = 0, last_buf_len = 0;
  int weather_len = 0;
  unsigned char check_sum = 0;

  weather_len = sizeof(weather_choose);
  weather_len = sizeof(weather_choose) / 10;

  for (i = 0; i < weather_len; i++)
  {
    my_memcpy(temp, weather_choose[i], strlen(weather_choose[i]));
    my_memcpy(buffer + 3, temp, strlen(weather_choose[i]));
    buffer[0] = strlen(weather_choose[i]) + 2;
    buf_len = strlen(weather_choose[i]) + 3;
    now_buf_len += buf_len;
    my_memcpy(result + DATA_START + last_buf_len, buffer, buf_len);
    last_buf_len = now_buf_len;
  }

  result[HEAD_FIRST] = 0x55;
  result[HEAD_SECOND] = 0xaa;
  result[PROTOCOL_VERSION] = 0x03;
  result[FRAME_TYPE] = 0x20;
  result[LENGTH_HIGH] = now_buf_len >> 8;
  result[LENGTH_LOW] = now_buf_len & 0xff;

  now_buf_len += PROTOCOL_HEAD;
  check_sum = get_check_sum((unsigned char *)result, now_buf_len - 1);
  result[now_buf_len - 1] = check_sum;

  wifi_uart_write_data((unsigned char *)result, now_buf_len);
}

/*****************************************************************************
  Function name          : weather_data_raw_handle
  Functional description : Weather data analysis
  Input parameters       : buffer:Receive data pointer
  Return parameter       : Null
*****************************************************************************/
void weather_data_raw_handle(char *buffer)
{
  char return_send_Weather[7] = { 0x55, 0xaa, 0x00, 0x21, 0x00, 0x00, 0x20 };
  int length = buffer[LENGTH_HIGH] * 256 + buffer[LENGTH_LOW];
  int i = 7;
  int can_len = 0;
  char can[15] = "";
  int type1 = 0;
  char value_string[100] = "";
  int val_cnt = 0;
  int val_len = 0;

  if (buffer[DATA_START] != 1 || length < 1)
  {
    //Receive failure
  }
  else
  {
    if (length < 4)
    {
      //The data is empty
    }

    while (i < length + 7 - 1)
    {
      can_len = buffer[i];
      my_memset(can, '\0', 15);
      my_memcpy(can, buffer + i + 1, can_len);

      type1 = buffer[i + 1 + can_len];
      if (type1 != 0 && type1 != 1)
      {
        return;
      }

      my_memset(value_string, '\0', 100);
      val_cnt = i + 1 + can_len + 1 + 1 - 1;
      val_len = buffer[val_cnt];
      if (type1 == 0)
      { //int32
        weather_data_user_handle(can + 2, type1, buffer + val_cnt + 1);
      }
      else if (type1 == 1)
      { //string
        my_memcpy(value_string, buffer + val_cnt + 1, val_len);
        weather_data_user_handle(can + 2, type1, value_string);
      }

      i += 1 + can_len + 1 + val_len + 1;
    }

    my_memcpy((unsigned char *)wifi_uart_tx_buf, return_send_Weather, 7);
    wifi_uart_write_data((unsigned char *)wifi_uart_tx_buf, 7);
  }
}
#endif

/*****************************************************************************
  Function name          : data_handle
  Functional description : Data frame processing
  Input parameters       : offset:Data start position
  Return parameter       : Null
*****************************************************************************/
void data_handle(unsigned short offset)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
  unsigned char *firmware_addr;
  static unsigned long firm_length;                                             //MCU upgrade file length
  static unsigned char firm_update_flag;                                        //MCU upgrade flag
  unsigned long dp_len;
#else
  unsigned short dp_len;
#endif

  unsigned char ret;
  unsigned short i, total_len;
  unsigned char cmd_type = wifi_uart_rx_buf[offset + FRAME_TYPE];

#ifdef WEATHER_ENABLE
  static unsigned char isWoSend = 0;                    //Whether the weather data has been turned on, 0: yes; 1: no
#endif

#ifdef WIFI_TEST_ENABLE
  unsigned char result;
  unsigned char rssi;
#endif

  switch (cmd_type)
  {
    case HEAT_BEAT_CMD:                                   //Heartbeat package
      heat_beat_check();
      break;

    case PRODUCT_INFO_CMD:                                //product information
      product_info_update();
      break;

    case WORK_MODE_CMD:                                   //Query the module working mode set by the MCU
      get_mcu_wifi_mode();
      break;

#ifndef WIFI_CONTROL_SELF_MODE
    case WIFI_STATE_CMD:                                  //Wifi working status
      wifi_work_state = wifi_uart_rx_buf[offset + DATA_START];
      wifi_uart_write_frame(WIFI_STATE_CMD, 0);
#ifdef WEATHER_ENABLE
      if (wifi_work_state == WIFI_CONNECTED && isWoSend == 0)  //When the WIFI connection is successful, open the weather data and only once
      {
        mcu_open_weather();
        isWoSend = 1;
      }
#endif
      break;

    case WIFI_RESET_CMD:                                  //Reset wifi (wifi returns successfully)
      reset_wifi_flag = RESET_WIFI_SUCCESS;
      break;

    case WIFI_MODE_CMD:                                   //Select smartconfig/AP mode (wifi returns successfully)
      set_wifimode_flag = SET_WIFICONFIG_SUCCESS;
      break;
#endif

    case DATA_QUERT_CMD:                                  //Order send
      total_len = wifi_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
      total_len += wifi_uart_rx_buf[offset + LENGTH_LOW];

      for (i = 0; i < total_len;)
      {
        dp_len = wifi_uart_rx_buf[offset + DATA_START + i + 2] * 0x100;
        dp_len += wifi_uart_rx_buf[offset + DATA_START + i + 3];
        //
        ret = data_point_handle((unsigned char *)wifi_uart_rx_buf + offset + DATA_START + i);

        if (SUCCESS == ret)
        {
          //Success message
        }
        else
        {
          //Error message
        }

        i += (dp_len + 4);
      }

      break;

    case STATE_QUERY_CMD:                                 //Status query
      all_data_update();
      break;

#ifdef SUPPORT_MCU_FIRM_UPDATE
    case UPDATE_START_CMD:                                //Upgrade start
      firm_length = wifi_uart_rx_buf[offset + DATA_START];
      firm_length <<= 8;
      firm_length |= wifi_uart_rx_buf[offset + DATA_START + 1];
      firm_length <<= 8;
      firm_length |= wifi_uart_rx_buf[offset + DATA_START + 2];
      firm_length <<= 8;
      firm_length |= wifi_uart_rx_buf[offset + DATA_START + 3];
      //
      wifi_uart_write_frame(UPDATE_START_CMD, 0);
      firm_update_flag = UPDATE_START_CMD;
      break;

    case UPDATE_TRANS_CMD:                                //Upgrade transfer
      if (firm_update_flag == UPDATE_START_CMD)
      {
        //Stop all data reporting
        stop_update_flag = ENABLE;

        total_len = wifi_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
        total_len += wifi_uart_rx_buf[offset + LENGTH_LOW];

        dp_len = wifi_uart_rx_buf[offset + DATA_START];
        dp_len <<= 8;
        dp_len |= wifi_uart_rx_buf[offset + DATA_START + 1];
        dp_len <<= 8;
        dp_len |= wifi_uart_rx_buf[offset + DATA_START + 2];
        dp_len <<= 8;
        dp_len |= wifi_uart_rx_buf[offset + DATA_START + 3];

        firmware_addr = (unsigned char *)wifi_uart_rx_buf;
        firmware_addr += (offset + DATA_START + 4);
        if ((total_len == 4) && (dp_len == firm_length))
        {
          //Last pack
          ret = mcu_firm_update_handle(firmware_addr, dp_len, 0);

          firm_update_flag = 0;
        }
        else if ((total_len - 4) <= FIRM_UPDATA_SIZE)
        {
          ret = mcu_firm_update_handle(firmware_addr, dp_len, total_len - 4);
        }
        else
        {
          firm_update_flag = 0;
          ret = ERROR;
        }

        if (ret == SUCCESS)
        {
          wifi_uart_write_frame(UPDATE_TRANS_CMD, 0);
        }
        //Resume all data reporting
        stop_update_flag = DISABLE;
      }
      break;
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
    case GET_LOCAL_TIME_CMD:                             //Get local time
      mcu_write_rtctime((unsigned char *)(wifi_uart_rx_buf + offset + DATA_START));
      break;
#endif

#ifdef WIFI_TEST_ENABLE
    case WIFI_TEST_CMD:                                   //Wifi function test
      result = wifi_uart_rx_buf[offset + DATA_START];
      rssi = wifi_uart_rx_buf[offset + DATA_START + 1];
      wifi_test_result(result, rssi);
      break;
#endif

#ifdef WEATHER_ENABLE
    case WEATHER_OPEN_CMD:
      weather_open_return_handle(wifi_uart_rx_buf[offset + DATA_START], wifi_uart_rx_buf[offset + DATA_START + 1]);

      break;

    case WEATHER_DATA_CMD:
      weather_data_raw_handle((char*)wifi_uart_rx_buf + offset);
      break;
#endif

#ifdef WIFI_STREAM_ENABLE
    case STREAM_OPEN_CMD:
      stream_status = wifi_uart_rx_buf[offset + DATA_START];//Whether the stream data is successfully opened
      break;

    case STREAM_START_CMD:
      stream_status = wifi_uart_rx_buf[offset + DATA_START];//Whether the stream data is successfully opened
      break;

    case STREAM_TRANS_CMD:
      stream_status = wifi_uart_rx_buf[offset + DATA_START];//Stream service transmission return reception
      break;

    case STREAM_STOP_CMD:
      stream_status = wifi_uart_rx_buf[offset + DATA_START];//Whether the stream data ends successfully
      break;
#endif

    default:
      break;
  }
}
/*****************************************************************************
  Function name          : get_queue_total_data
  Functional description : Read data in the queue
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
unsigned char get_queue_total_data(void)
{
  if (queue_in != queue_out)
    return 1;
  else
    return 0;
}
/*****************************************************************************
  Function name          : Queue_Read_Byte
  Functional description : Read queue 1 byte data
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
unsigned char Queue_Read_Byte(void)
{
  unsigned char value;

  if (queue_out != queue_in)
  {
    //Have data
    if (queue_out >= (unsigned char *)(wifi_queue_buf + sizeof(wifi_queue_buf)))
    {
      //The data has reached the end
      queue_out = (unsigned char *)(wifi_queue_buf);
    }

    value = *queue_out ++;
  }

  return value;
}
