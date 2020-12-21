/**********************************Copyright (c)**********************************
**                     All rights reserved (C), 2015-2020, Tuya
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    wifi.h
 * @author  Tuya Comprehensive Agreement Development Group
 * @version v2.5.5
 * @date    2020.6.1
 * @brief   Users don't need to care about the file implementation content.
 */


#ifndef __WIFI_H_
#define __WIFI_H_

//#include "stm32f1xx.h"  
#include "stdio.h"
#include "string.h"
#include "protocol.h"
#include "system.h"
#include "mcu_api.h"
#include "global.h"

//=============================================================================
//Define constant
//If compilation occurs error:  #40:  expected an identifier    DISABLE = 0, for similar error reporting, you can include a header file in this stm32f1xx.h file
//=============================================================================
#ifndef TRUE
#define         TRUE                1
#endif

#ifndef FALSE
#define         FALSE               0
#endif

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
#define         ENABLE              1
#endif

#ifndef DISABLE
#define         DISABLE             0
#endif
//=============================================================================
//dp data point type
//=============================================================================
#define         DP_TYPE_RAW                     0x00        //RAW type
#define         DP_TYPE_BOOL                    0x01        //bool type
#define         DP_TYPE_VALUE                   0x02        //value type
#define         DP_TYPE_STRING                  0x03        //string type
#define         DP_TYPE_ENUM                    0x04        //enum type
#define         DP_TYPE_BITMAP                  0x05        //fault type

//=============================================================================
//WIFI work status
//=============================================================================
#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_CONN_CLOUD                 0x04
#define         WIFI_LOW_POWER                  0x05
#define         SMART_AND_AP_STATE              0x06
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
//How WIFI module and MCU work
//=============================================================================
#define         UNION_WORK                      0x0                             //MCU and WIFI module processing
#define         WIFI_ALONE                      0x1                             //WIFI module self-processing

//=============================================================================
//System working mode
//=============================================================================
#define         NORMAL_MODE                     0x00                            //Normal working condition
#define         FACTORY_MODE                    0x01                            //Factory mode
#define         UPDATE_MODE                     0x02                            //Upgrade mode

//=============================================================================
//Choose network access mode
//=============================================================================
#define         CONFIG_MODE_DEFAULT             "0"                             //Default mode
#define         CONFIG_MODE_LOWPOWER            "1"                             //low power mode
#define         CONFIG_MODE_SPECIAL             "2"                             //special mode




//=============================================================================
//download command
//=============================================================================
/*typedef struct {
  unsigned char dp_id;                              //dp number
  unsigned char dp_type;                            //dp type
} DOWNLOAD_CMD_S;*/

#endif
