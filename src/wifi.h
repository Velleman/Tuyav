/****************************************Copyright (c)*************************
**                           All rights reserved (C), 2015-2020, Tuya
**
**                                 http://www.tuya.com
**
**--------------File information-------------------------------------------------------
**File name   : wifi.h
**Description : Wifi file header definition
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
#ifndef __WIFI_H_
#define __WIFI_H_


//=============================================================================
/*Define constant*/
//=============================================================================
#ifndef TRUE
#define         TRUE                1
#endif
//
#ifndef FALSE
#define         FALSE               0
#endif
//
#ifndef NULL
#define         NULL                ((void *) 0)
#endif

#ifndef SUCCESS
#define         SUCCESS             1
#endif

#ifndef ERROR
#define         ERROR               0
#endif

#ifndef INVALID
#define         INVALID             0xFF
#endif

#ifndef ENABLE
#define         ENABLE                1
#endif
//
#ifndef DISABLE
#define         DISABLE               0
#endif
//=============================================================================
//dp data point type
//=============================================================================
#define         DP_TYPE_RAW                     0x00				//RAW type
#define         DP_TYPE_BOOL                    0x01	            //bool type
#define         DP_TYPE_VALUE                   0x02	            //value type
#define         DP_TYPE_STRING                  0x03				//string type
#define         DP_TYPE_ENUM                    0x04				//enum type
#define         DP_TYPE_FAULT                   0x05				//fault type

//=============================================================================
//WIFI work status
//=============================================================================
#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define			WIFI_CONN_CLOUD					0x04
#define			WIFI_LOW_POWER					0x05
#define         WIFI_SATE_UNKNOW                0xff
//=============================================================================
//wifi distribution network
//=============================================================================
#define         SMART_CONFIG                    0x0
#define         AP_CONFIG                       0x1

//=============================================================================
//wifi reset status
//=============================================================================
#define         RESET_WIFI_ERROR                0
#define         RESET_WIFI_SUCCESS              1

//=============================================================================
//wifi reset result
//=============================================================================
#define         SET_WIFICONFIG_ERROR            0
#define         SET_WIFICONFIG_SUCCESS          1

//=============================================================================
//MCU firmware upgrade status
//=============================================================================
#define         FIRM_STATE_UN_SUPPORT           0x00                            //MCU upgrade is not supported
#define         FIRM_STATE_WIFI_UN_READY        0x01                            //Module not ready
#define         FIRM_STATE_GET_ERROR            0x02                            //Cloud upgrade information query failed.
#define         FIRM_STATE_NO                   0x03                            //No upgrade required (no update in the cloud)
#define         FIRM_STATE_START                0x04                            //Need to upgrade, wait for the module to initiate an upgrade operation

//=============================================================================
//How WIFI and mcu work
//=============================================================================
#define         UNION_WORK                      0x0                             //Mcu module and wifi processing
#define         WIFI_ALONE                      0x1                             //Wifi module self-processing

//=============================================================================
//System working mode
//=============================================================================
#define         NORMAL_MODE             0x00                                    //Normal working condition
#define         FACTORY_MODE            0x01                                    //Factory mode
#define         UPDATE_MODE             0X02                                    //Upgrade mode
//=============================================================================
//Choose network access mode
//=============================================================================
#define         CONFIG_MODE_DEFAULT     "0"                                    //Default mode
#define         CONFIG_MODE_LOWPOWER    "1"                                    //low power mode
#define         CONFIG_MODE_SPECIAL     "2"                                    //special mode    

#include "protocol.h"
#include "system.h"
#include "mcu_api.h"


//=============================================================================
//send command
//=============================================================================


#endif
