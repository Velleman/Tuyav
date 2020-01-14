/****************************************Copyright (c)*************************
**                            All rights reserved (C), 2015-2020, Tuya
**
**                                 http://www.tuya.com
**
**--------------File information-------------------------------------------------------
**File name   : system.h
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
#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#ifdef SYSTEM_GLOBAL
#define SYSTEM_EXTERN
#else
#define SYSTEM_EXTERN   extern
#endif

//=============================================================================
//Byte order of the frame
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//Data frame type
//=============================================================================
#define         HEAT_BEAT_CMD                   0                               //Heartbeat package
#define         PRODUCT_INFO_CMD                1                               //Product information
#define         WORK_MODE_CMD                   2                               //Query the module working mode set by the MCU
#define         WIFI_STATE_CMD                  3                               //Wifi working status
#define         WIFI_RESET_CMD                  4                               //Reset wifi
#define         WIFI_MODE_CMD                   5                               //Select smartconfig/AP mode
#define         DATA_QUERT_CMD                  6                               //Order send
#define         STATE_UPLOAD_CMD                7                               //Status upload	 
#define         STATE_QUERY_CMD                 8                               //Status query   
#define         UPDATE_QUERY_CMD                9                               //Upgrade query
#define         UPDATE_START_CMD                0x0a                            //Upgrade start
#define         UPDATE_TRANS_CMD                0x0b                            //Upgrade transfer
#define         GET_ONLINE_TIME_CMD             0x0c                            //Get system time (Greenwich Mean Time)
#define         FACTORY_MODE_CMD                0x0d                            //Enter production test mode    
#define         WIFI_TEST_CMD                   0x0e                            //Wifi function test
#define         GET_LOCAL_TIME_CMD              0x1c                            //Get local time
#define         WEATHER_OPEN_CMD                0x20                            //Turn on the weather          
#define         WEATHER_DATA_CMD                0x21                            //Weather data
#define         HEAT_BEAT_STOP                  0x25                            //Turn off the WIFI module heartbeat
#define         STREAM_OPEN_CMD                 0x26                            //Turn on streaming service
#define         STREAM_START_CMD                0x27                            //Turn on streaming data transmission
#define         STREAM_TRANS_CMD                0x28                            //Stream data transmission
#define         STREAM_STOP_CMD                 0x29                            //End stream data transfer


//=============================================================================
#define VERSION                 0x00                                            //Protocol version number
#define PROTOCOL_HEAD           0x07                                            //Fixed protocol header length
#define FIRM_UPDATA_SIZE        256                                            //Upgrade package size
#define FRAME_FIRST             0x55
#define FRAME_SECOND            0xaa
//=============================================================================
SYSTEM_EXTERN volatile unsigned char wifi_queue_buf[PROTOCOL_HEAD + WIFI_UART_QUEUE_LMT];  //Serial queue buffer
SYSTEM_EXTERN volatile unsigned char wifi_uart_rx_buf[PROTOCOL_HEAD + WIFI_UART_RECV_BUF_LMT];         //Serial receive buffer
SYSTEM_EXTERN volatile unsigned char wifi_uart_tx_buf[PROTOCOL_HEAD + WIFIR_UART_SEND_BUF_LMT];        //Serial port send buffer
//
SYSTEM_EXTERN volatile unsigned char *queue_in;
SYSTEM_EXTERN volatile unsigned char *queue_out;

SYSTEM_EXTERN volatile unsigned char stop_update_flag;

#ifndef WIFI_CONTROL_SELF_MODE
SYSTEM_EXTERN volatile unsigned char reset_wifi_flag;                                                  //Reset wifi flag (TRUE: successful / FALSE: failed)
SYSTEM_EXTERN volatile unsigned char set_wifimode_flag;                                                //Set the WIFI working mode flag (TRUE: Success / FALSE: Failed)
SYSTEM_EXTERN volatile unsigned char wifi_work_state;                                                  //Wifi module current working status
#endif

#ifdef WIFI_STREAM_ENABLE
SYSTEM_EXTERN volatile char stream_status;                                                             //Stream service delivery return status
#endif

/*****************************************************************************
  Function name          : set_wifi_uart_byte
  Functional description : Write wifi_uart byte
  Input parameters       : dest: the actual address of the buffer area;
                         byte: write byte value
  Return parameter       : Total length after writing is completed
*****************************************************************************/
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte);

/*****************************************************************************
  Function name          : set_wifi_uart_buffer
  Functional description : 写wifi_uart_buffer
  Input parameters       : dest:target address
           src:source address
           len:Data length
  Return parameter       : Null
*****************************************************************************/
unsigned short set_wifi_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len);

/*****************************************************************************
  Function name          : wifi_uart_write_frame
  Functional description : Send a frame of data to the wifi serial port
  Input parameters       : fr_type:Frame type
           len:Data length
  Return parameter       : Null
*****************************************************************************/
void wifi_uart_write_frame(unsigned char fr_type, unsigned short len);

/*****************************************************************************
  Function name          : get_check_sum
  Functional description : Calculate checksum
  Input parameters       : pack:Data source pointer
                         pack_len:Need to calculate the length of the checksum data
  Return parameter       : checksum
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/*****************************************************************************
  Function name          : data_handle
  Functional description : Data frame processing
  Input parameters       : offset:Data start position
  Return parameter       : Null
*****************************************************************************/
void data_handle(unsigned short offset);

/*****************************************************************************
  Function name          : get_queue_total_data
  Functional description : Read data in the queue
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
unsigned char get_queue_total_data(void);

/*****************************************************************************
  Function name          : Queue_Read_Byte
  Functional description : Read queue 1 byte data
  Input parameters       : Null
  Return parameter       : Null
*****************************************************************************/
unsigned char Queue_Read_Byte(void);

#endif
