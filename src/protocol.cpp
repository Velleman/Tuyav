/**********************************Copyright (c)**********************************
**                     All rights reserved (C), 2015-2020, Tuya
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    protocol.c
 * @author  Tuya Comprehensive Agreement Development Group
 * @version v2.5.5
 * @date    2020.6.1
 * @brief                
 *                       *******Very important, be sure to watch!!!********
 *          1. The user implements the data delivery/reporting function in this file.
 *          2. DP ID / TYPE and data processing functions require the user to implement according to the actual definition
 *          3. There are #err hints inside the function that needs the user to implement the code after starting some macro definitions. 
 *             Please delete the #err after completing the function.
 */

/******************************************************************************
                                Transplant instructions:
1:The MCU must directly call the wifi_uart_service() function in mcu_api.c in the while.
2:After the normal initialization of the program is completed, 
  it is recommended not to turn off the serial port interrupt. 
  If the interrupt must be turned off, the off interrupt time must be short, 
  and the interrupt will cause the serial port packet to be lost.
3:Do not call the escalation function in the interrupt/timer interrupt
******************************************************************************/

#include "wifi.h"
#include "Arduino.h"
#include "global.h"
#include "Tuyav.h"
#ifdef WEATHER_ENABLE
/**
 * @var    weather_choose
 * @brief  Weather data parameter selection array
 * @note   Users can customize the required parameters, comments or uncomment,
 *         pay attention to the changes
 */
const char *weather_choose[WEATHER_CHOOSE_CNT] = {
    "temp",
    "humidity",
    "condition",
    "pm25",
    "pressure",
    "realFeel",
    "uvi",
    //"tips",
    "windDir",
    "windSpeed",
    "sunRise",
    "sunSet",
    "aqi",
    //"so2 ",
    //"rank",
    "pm10",
    "o3",
    "no2",
    "co",
};
#endif

/******************************************************************************
                              The first step: initialization
1:Include "wifi.h" in files that need to use wifi related files
2:Call the wifi_protocol_init() function in the mcu_api.c file in the MCU initialization
3:Fill the MCU serial single-byte send function into the uart_transmit_output 
   function in the protocol.c file, and delete #error
4:Call the uart_receive_input function in the mcu_api.c file in the MCU serial receive 
   function and pass the received byte as a parameter.
5:The wifi_uart_service() function in the mcu_api.c file is called after the MCU enters the while loop.
******************************************************************************/

/******************************************************************************
                        1:dp data point sequence type comparison table
          **This is the automatic generation of code, such as the relevant changes in
              the development platform, please re-download MCU_SDK**
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
    {
        {DPID_DIN_1, DP_TYPE_BOOL},
        {DPID_DIN_2, DP_TYPE_BOOL},
        {DPID_DIN_3, DP_TYPE_BOOL},
        {DPID_AIN_1, DP_TYPE_VALUE},
        {DPID_AIN_2, DP_TYPE_VALUE},
        {DPID_AIN_3, DP_TYPE_VALUE},
        {DPID_DOUT_1, DP_TYPE_BOOL},
        {DPID_DOUT_2, DP_TYPE_BOOL},
        {DPID_DOUT_3, DP_TYPE_BOOL},
        {DPID_DOUT_4, DP_TYPE_BOOL},
        {DPID_DOUT_5, DP_TYPE_BOOL},
        {DPID_AOUT_1, DP_TYPE_VALUE},
        {DPID_AOUT_2, DP_TYPE_VALUE},
        {DPID_AOUT_3, DP_TYPE_VALUE},
        {DPID_AV1, DP_TYPE_STRING},
        {DPID_AV2, DP_TYPE_STRING},
        {DPID_AV3, DP_TYPE_STRING},
        {DPID_AV4, DP_TYPE_STRING},
        {DPID_AV5, DP_TYPE_STRING},
        {DPID_AV6, DP_TYPE_STRING},
        {DPID_AV7, DP_TYPE_STRING},
        {DPID_AV8, DP_TYPE_STRING},
        {DPID_AV9, DP_TYPE_STRING},
};

/******************************************************************************
                        2:Serial single-byte send function
Please fill in the MCU serial port send function into the function,
and pass the received data as a parameter to the serial port send function.
******************************************************************************/

/**
 * @brief  Send data processing
 * @param[in] {value} Serial port receives byte data
 * @return Null
 */
void uart_transmit_output(unsigned char value)
{
    if (!tuyav.getPasstrough())
    {
        if (tuyav.getDebug())
        {
            Serial.println(value, HEX);
        }
        tuyav.get_tuyaSerial().write(value);
    }
}

/******************************************************************************
                           Step 2: Implement a specific user function
1:APP send data processing
2:Data upload processing
******************************************************************************/

/******************************************************************************
                            1:All data upload processing
The current function handles all data upload (including deliverable/reportable and report only)

  Users need to implement according to the actual situation:
  1:Need to implement the reportable/reportable data point report
  2:Need to report only reported data points
This function must be called internally by the MCU.
Users can also call this function to achieve all data upload.
******************************************************************************/

//Automated generation of data reporting functions

/**
 * @brief  All dp point information of the system is uploaded to realize APP and muc data synchronization
 * @param  Null
 * @return Null
 * @note   This function SDK needs to be called internally;
 *         The MCU must implement the data upload function in the function;
 *         including only reporting and reportable hair style data.
 */
void all_data_update(void)
{
    mcu_dp_bool_update(DPID_DIN_1, 0);      //BOOL;
    mcu_dp_bool_update(DPID_DIN_2, 0);      //BOOL;
    mcu_dp_bool_update(DPID_DIN_3, 0);      //BOOL;
    mcu_dp_value_update(DPID_AIN_1, 0);     //VALUE;
    mcu_dp_value_update(DPID_AIN_2, 0);     //VALUE;
    mcu_dp_value_update(DPID_AIN_3, 0);     //VALUE;
    mcu_dp_bool_update(DPID_DOUT_1, 0);     //BOOL;
    mcu_dp_bool_update(DPID_DOUT_2, 0);     //BOOL;
    mcu_dp_bool_update(DPID_DOUT_3, 0);     //BOOL;
    mcu_dp_bool_update(DPID_DOUT_4, 0);     //BOOL;
    mcu_dp_bool_update(DPID_DOUT_5, 0);     //BOOL;
    mcu_dp_value_update(DPID_AOUT_1, 0);    //VALUE;
    mcu_dp_value_update(DPID_AOUT_2, 0);    //VALUE;
    mcu_dp_value_update(DPID_AOUT_3, 0);    //VALUE;
    mcu_dp_string_update(DPID_AV1, " ", 1); //STRING;
    mcu_dp_string_update(DPID_AV2, " ", 1); //STRING;
    mcu_dp_string_update(DPID_AV3, " ", 1); //STRING;
    mcu_dp_string_update(DPID_AV4, " ", 1); //STRING;
    mcu_dp_string_update(DPID_AV5, " ", 1); //STRING;
    mcu_dp_string_update(DPID_AV6, " ", 1); //STRING;
    mcu_dp_string_update(DPID_AV7, " ", 1); //STRING;
    mcu_dp_string_update(DPID_AV8, " ", 1); //STRING;
    mcu_dp_string_update(DPID_AV9, " ", 1); //STRING;
}

/******************************************************************************
                                WARNING!!!    
                            2:All data upload processing
Automate code template functions, please implement data processing by yourself
******************************************************************************/
/*****************************************************************************
Function name : dp_download_dout_1_handle
Function description : on DPID_DOUT_1 processing function
Input parameter : value:Source data
        : length:Data length
Return parameter : Successful return:SUCCESS/Failed to return:ERROR
Instructions for use : 可下发可上报type,need to report the result to App after data is dealt with
*****************************************************************************/
static unsigned char dp_download_dout_1_handle(const unsigned char value[], unsigned short length)
{
    //Example: The current DP type isBOOL
    unsigned char ret;
    //0:关/1:开
    unsigned char dout_1;

    dout_1 = mcu_get_dp_download_bool(value, length);
    if (tuyav.getDigitalOutput(0).get_PinID() != -1)
    {
        tuyav.getDigitalOutput(0).writeDigital(dout_1);
        

        //处理完DP数据后应有反馈
    }
    tuyav.DIGITAL_OUT[0] = dout_1;
    ret = mcu_dp_bool_update(DPID_DOUT_1, dout_1);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
Function name : dp_download_dout_2_handle
Function description : on DPID_DOUT_2 processing function
Input parameter : value:Source data
        : length:Data length
Return parameter : Successful return:SUCCESS/Failed to return:ERROR
Instructions for use : 可下发可上报type,need to report the result to App after data is dealt with
*****************************************************************************/
static unsigned char dp_download_dout_2_handle(const unsigned char value[], unsigned short length)
{
    //Example: The current DP type isBOOL
    unsigned char ret;
    //0:关/1:开
    unsigned char dout_2;

    dout_2 = mcu_get_dp_download_bool(value, length);
    if (tuyav.getDigitalOutput(1).get_PinID() != -1)
    {
        tuyav.getDigitalOutput(1).writeDigital(dout_2);
        
    }
    tuyav.DIGITAL_OUT[1] = dout_2;
    //处理完DP数据后应有反馈
    ret = mcu_dp_bool_update(DPID_DOUT_2, dout_2);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
Function name : dp_download_dout_3_handle
Function description : on DPID_DOUT_3 processing function
Input parameter : value:Source data
        : length:Data length
Return parameter : Successful return:SUCCESS/Failed to return:ERROR
Instructions for use : 可下发可上报type,need to report the result to App after data is dealt with
*****************************************************************************/
static unsigned char dp_download_dout_3_handle(const unsigned char value[], unsigned short length)
{
    //Example: The current DP type isBOOL
    unsigned char ret;
    //0:关/1:开
    unsigned char dout_3;

    dout_3 = mcu_get_dp_download_bool(value, length);
    if (tuyav.getDigitalOutput(2).get_PinID() != -1)
    {
        tuyav.getDigitalOutput(2).writeDigital(dout_3);
        
    }
    tuyav.DIGITAL_OUT[2] = dout_3;
    //处理完DP数据后应有反馈
    ret = mcu_dp_bool_update(DPID_DOUT_3, dout_3);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
Function name : dp_download_dout_4_handle
Function description : on DPID_DOUT_4 processing function
Input parameter : value:Source data
        : length:Data length
Return parameter : Successful return:SUCCESS/Failed to return:ERROR
Instructions for use : 可下发可上报type,need to report the result to App after data is dealt with
*****************************************************************************/
static unsigned char dp_download_dout_4_handle(const unsigned char value[], unsigned short length)
{
    //Example: The current DP type isBOOL
    unsigned char ret;
    //0:关/1:开
    unsigned char dout_4;

    dout_4 = mcu_get_dp_download_bool(value, length);
    if (tuyav.getDigitalOutput(3).get_PinID() != -1)
    {
        tuyav.getDigitalOutput(3).writeDigital(dout_4);
        
    }
    tuyav.DIGITAL_OUT[3] = dout_4;
    //处理完DP数据后应有反馈
    ret = mcu_dp_bool_update(DPID_DOUT_4, dout_4);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
Function name : dp_download_dout_5_handle
Function description : on DPID_DOUT_5 processing function
Input parameter : value:Source data
        : length:Data length
Return parameter : Successful return:SUCCESS/Failed to return:ERROR
Instructions for use : 可下发可上报type,need to report the result to App after data is dealt with
*****************************************************************************/
static unsigned char dp_download_dout_5_handle(const unsigned char value[], unsigned short length)
{
    //Example: The current DP type isBOOL
    unsigned char ret;
    //0:关/1:开
    unsigned char dout_5;

    dout_5 = mcu_get_dp_download_bool(value, length);
    if (tuyav.getDigitalOutput(4).get_PinID() != -1)
    {
        tuyav.getDigitalOutput(4).writeDigital(dout_5);
        
    }
    tuyav.DIGITAL_OUT[4] = dout_5;
    //处理完DP数据后应有反馈
    ret = mcu_dp_bool_update(DPID_DOUT_5, dout_5);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
Function name : dp_download_aout_1_handle
Function description : on DPID_AOUT_1 processing function
Input parameter : value:Source data
        : length:Data length
Return parameter : Successful return:SUCCESS/Failed to return:ERROR
Instructions for use : 可下发可上报type,need to report the result to App after data is dealt with
*****************************************************************************/
static unsigned char dp_download_aout_1_handle(const unsigned char value[], unsigned short length)
{
    //Example: The current DP type isVALUE
    unsigned char ret;
    long aout_1;

    aout_1 = mcu_get_dp_download_value(value, length);
    if (tuyav.getAnalogOutput(0).get_PinID() != -1)
    {
        tuyav.getAnalogOutput(0).writeAnalog(aout_1);
        
        //Serial.print("AOUT1 ");
        //Serial.println(aout_1);
    }
    tuyav.ANALOG_OUT[0] = aout_1;
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_AOUT_1, aout_1);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
Function name : dp_download_aout_2_handle
Function description : on DPID_AOUT_2 processing function
Input parameter : value:Source data
        : length:Data length
Return parameter : Successful return:SUCCESS/Failed to return:ERROR
Instructions for use : 可下发可上报type,need to report the result to App after data is dealt with
*****************************************************************************/
static unsigned char dp_download_aout_2_handle(const unsigned char value[], unsigned short length)
{
    //Example: The current DP type isVALUE
    unsigned char ret;
    long aout_2;

    aout_2 = mcu_get_dp_download_value(value, length);
    if (tuyav.getAnalogOutput(1).get_PinID() != -1)
    {
        tuyav.getAnalogOutput(1).writeAnalog(aout_2);
        
        //Serial.print("AOUT2 ");
        //Serial.println(aout_2);
    }
    tuyav.ANALOG_OUT[1] = aout_2;
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_AOUT_2, aout_2);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
Function name : dp_download_aout_3_handle
Function description : on DPID_AOUT_3 processing function
Input parameter : value:Source data
        : length:Data length
Return parameter : Successful return:SUCCESS/Failed to return:ERROR
Instructions for use : 可下发可上报type,need to report the result to App after data is dealt with
*****************************************************************************/
static unsigned char dp_download_aout_3_handle(const unsigned char value[], unsigned short length)
{
    //Example: The current DP type isVALUE
    unsigned char ret;
    long aout_3;

    aout_3 = mcu_get_dp_download_value(value, length);
    if (tuyav.getAnalogOutput(2).get_PinID() != -1)
    {
        tuyav.getAnalogOutput(2).writeAnalog(aout_3);
        

        //Serial.print("AOUT3 ");
        //Serial.println(aout_3);
    }
    tuyav.ANALOG_OUT[2] = aout_3;
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_AOUT_3, aout_3);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/******************************************************************************
                                WARNING!!!                     
The following function users do not modify!!
******************************************************************************/

/**
 * @brief  dp delivery processing function
 * @param[in] {dpid} DP number
 * @param[in] {value} dp data buffer address
 * @param[in] {length} dp data length
 * @return Dp processing results
 * -           0(ERROR): failure
 * -           1(SUCCESS): success
 * @note   The function user cannot modify
 */
unsigned char dp_download_handle(unsigned char dpid, const unsigned char value[], unsigned short length)
{
    /*********************************
    Current function processing can issue/report data calls                    
    Need to implement the data processing in the specific function
    The result of the processing needs to be fed back to the APP, otherwise the APP will consider the delivery failure.
    ***********************************/
    unsigned char ret;
    switch (dpid)
    {
    case DPID_DOUT_1:
        //Digital Output 1processing function
        ret = dp_download_dout_1_handle(value, length);
        break;
    case DPID_DOUT_2:
        //Digital Output 2processing function
        ret = dp_download_dout_2_handle(value, length);
        break;
    case DPID_DOUT_3:
        //Digital Output 3processing function
        ret = dp_download_dout_3_handle(value, length);
        break;
    case DPID_DOUT_4:
        //Digital Output 4processing function
        ret = dp_download_dout_4_handle(value, length);
        break;
    case DPID_DOUT_5:
        //Digital Output 5processing function
        ret = dp_download_dout_5_handle(value, length);
        break;
    case DPID_AOUT_1:
        //Analog Output 1processing function
        ret = dp_download_aout_1_handle(value, length);
        break;
    case DPID_AOUT_2:
        //Analog Output 2processing function
        ret = dp_download_aout_2_handle(value, length);
        break;
    case DPID_AOUT_3:
        //Analog Output 3processing function
        ret = dp_download_aout_3_handle(value, length);
        break;

    default:
        break;
    }
    return ret;
}

/**
 * @brief  Get the sum of all dp commands
 * @param[in] Null
 * @return Sent the sum of the commands
 * @note   The function user cannot modify
 */
unsigned char get_download_cmd_total(void)
{
    return (sizeof(download_cmd) / sizeof(download_cmd[0]));
}

/******************************************************************************
                                WARNING!!!                     
This code is called internally by the SDK.
Please implement the internal data of the function according to the actual dp data.
******************************************************************************/

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  Upgrade package size selection
 * @param[in] {package_sz} Upgrade package size
 * @ref           0x00: 256byte (default)
 * @ref           0x01: 512byte
 * @ref           0x02: 1024byte
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void upgrade_package_choose(unsigned char package_sz)
{
#error "Please implement the upgrade package size selection processing code by yourself. Please delete this line after completion"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
    wifi_uart_write_frame(UPDATE_START_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  MCU enters firmware upgrade mode
 * @param[in] {value} Firmware buffer
 * @param[in] {position} The current data packet is in the firmware location
 * @param[in] {length} Current firmware package length (when the firmware package length is 0, it indicates that the firmware package is sent)
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
unsigned char mcu_firm_update_handle(const unsigned char value[], unsigned long position, unsigned short length)
{
#error "Please complete the MCU firmware upgrade processing code yourself. Please delete the line after completion"
    if (length == 0)
    {
        //Firmware data transmission completed
    }
    else
    {
        //Firmware data processing
    }

    return SUCCESS;
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  Gets the green time
 * @param[in] {time} Get the time data
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void mcu_get_greentime(unsigned char time[])
{
#error "Complete the code yourself and delete the line"
    /* 
    time[0] is the flag of success for obtaining the time,0: failure / 1: success
    time[1] is year, 0x00 :2000 year
    time[2] is month, It starts at 1 and ends at 12
    time[3] is date, It starts at 1 and ends at 31
    time[4] is hour, It starts at 0 and ends at 23
    time[5] is minute, It starts at 0 and ends at 59
    time[6] is second, It starts at 0 and ends at 59
    */
    if (time[0] == 1)
    {
        //Receive the green data returned by wifi module correctly
    }
    else
    {
        //There is an error in obtaining the green time. It may be that the current wifi module is not connected to the Internet
    }
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU proofreads local RTC clock
 * @param[in] {time} Get the time data
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void mcu_write_rtctime(unsigned char time[])
{
    if (time[0] == 1)
    {
        //Correctly receive the local clock data returned by the wifi module
        //update Tuyav value with new time
        tuyav.newTime[0] = time[0]; //time updated 							(0=no, 1=yes)
        tuyav.newTime[1] = time[1]; //year 	in yy 	format					(00-99)
        tuyav.newTime[2] = time[2]; //month in mm 	format 					(1-12)
        tuyav.newTime[3] = time[3]; //date 	in dd 	format 					(1-31)
        tuyav.newTime[4] = time[4]; //hour 	in hh	format					(0-23)
        tuyav.newTime[5] = time[5]; //minute in mm	format					(0-59)
        tuyav.newTime[6] = time[6]; //seconds in ss format					(0-59)
        tuyav.newTime[7] = time[7]; //weekday in d  format					(1-7)   1=Monday

        //fixing year & weekDay
        tuyav.newTime[1] = tuyav.newTime[1] + 2000; //add 2000 so year now reports in yyyy format (2000-2099)
        if (tuyav.newTime[7] < 7)
        {
            tuyav.newTime[7] = tuyav.newTime[7] + 1;
        }
        else
        {
            tuyav.newTime[7] = 1;
        } //since 1= monday & most RTC clocks use 1=sunday, increment with 1 (except when 7, then 1);
    }
    else
    {
        //Error getting local clock data, it may be that the current wifi module is not connected
        tuyav.newTime[0] = 0;
        tuyav.newTime[1] = 0;
        tuyav.newTime[2] = 0;
        tuyav.newTime[3] = 0;
        tuyav.newTime[4] = 0;
        tuyav.newTime[5] = 0;
        tuyav.newTime[6] = 0;
        tuyav.newTime[7] = 0;
    }
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  Wifi function test feedback
 * @param[in] {result} Wifi function test
 * @ref       0: failure
 * @ref       1: success
 * @param[in] {rssi} Test success indicates wifi signal strength / test failure indicates error type
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void wifi_test_result(unsigned char result, unsigned char rssi)
{
#error "Please implement the wifi function test success/failure code by yourself. Please delete the line after completion"
    if (result == 0)
    {
        //Test failed
        if (rssi == 0x00)
        {
            //Can't scan to the router named tuya_mdev_test, please check
        }
        else if (rssi == 0x01)
        {
            //Module not authorized
        }
    }
    else
    {
        //Test success
        //rssi is the signal strength (0-100, 0 signal is the worst, 100 signal is the strongest)
    }
}
#endif

#ifdef WEATHER_ENABLE
/**
* @brief  Open weather service
 * @param  Null
 * @return Null
 */
void mcu_open_weather(void)
{
    int i = 0;
    char buffer[13] = {0};
    unsigned char weather_len = 0;
    unsigned short send_len = 0;

    weather_len = sizeof(weather_choose) / sizeof(weather_choose[0]);
    for (i = 0; i < weather_len; i++)
    {
        buffer[0] = sprintf(buffer + 1, "w.%s", weather_choose[i]);
        send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0] + 1);
    }

    //#error "Please read the following instructions,After the processing is completed, delete the line"

    //When the parameters obtained have parameters related to time (such as sunrise and sunset),
    //they need to be used with t.unix or t.local, and whether the parameter data needed to be acquired is based on green time or local time.

    //buffer[0] = sprintf(buffer+1,"t.unix"); //green time   or use  //
    buffer[0] = sprintf(buffer + 1, "t.local"); //local time
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0] + 1);
    buffer[0] = sprintf(buffer+1,"c.%s","city");
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0] + 1);
    //Serial.println(send_len);
    /*buffer[0] = sprintf(buffer+1,"w.date.%d",WEATHER_FORECAST_DAYS_NUM);
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);*/
    wifi_uart_write_frame(WEATHER_OPEN_CMD, MCU_TX_VER, send_len);
    
   //Serial.println();
    wifi_uart_service();
}

/**
 * @brief  Turn on the weather function
 * @param[in] {res} Turn on the weather function to return the result
 * @ref       0: failure
 * @ref       1: success
 * @param[in] {err} Error code
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void weather_open_return_handle(unsigned char res, unsigned char err)
{
    //#error "Please complete the M open weather function to return the data processing code. Please delete the line after completion"
    unsigned char err_num = 0;
    if (res == 1)
    {
        //Open the weather and return successfully
        //tuyav.setDebug(true);
    }
    else if (res == 0)
    {
        //Open weather return failed
        //Get the error code
        err_num = err;
    }
}

/**
 * @brief  Weather data user self-processing function
 * @param[in] {name} parameter name
 * @param[in] {type} parameter type
 * @ref       0: int
 * @ref       1: string
 * @param[in] {data} the address of the parameter value
 * @param[in] {day} What day's weather. range of value: 0~6
 * @ref       0: today
 * @ref       1: tomorrow
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void weather_data_user_handle(char *name, unsigned char type, const unsigned char *data, char day)
{

    /*
    "pm10",
    "o3",
    "no2",
    "co",*/
    int value_int;
    //char value_string[50]; //Since there are many parameters, the default is 50. You can reduce this value appropriately based on the defined parameters.

    //First get the data type

    if (type == 0)
    { //The parameter is INT type
        value_int = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
        tuyav.setWeatherParam(name, value_int);
    }
    else if (type == 1)
    {
        //my_strcpy(value_string, data);
        tuyav.setWeatherParam(name, data);
    }
}
#endif

#ifdef MCU_DP_UPLOAD_SYN
/**
 * @brief  Status reports results synchronously
 * @param[in] {result} Result
 * @ref       0: failure
 * @ref       1: success
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void get_upload_syn_result(unsigned char result)
{
#error "Please complete the status report result processing code by yourself and delete the line"

    if (result == 0)
    {
        //Synchronous report error
    }
    else
    {
        //Report synchronously successfully
    }
}
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  Get the WIFI status result
 * @param[in] {result} Wifi working state
 * @ref       0x00: wifi status 1  smartconfig configuration status
 * @ref       0x01: wifi status 2  AP configuration status
 * @ref       0x02: wifi status 3  WIFI configured but not connected to the router
 * @ref       0x03: wifi status 4  WIFI Configured and connected to the router
 * @ref       0x04: wifi status 5  The router is connected and connected to the cloud
 * @ref       0x05: wifi status 6  The wifi device is in low power mode
 * @ref       0x06: wifi status 7  The wifi device is in smartconfig&AP mode
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void get_wifi_status(unsigned char result)
{
    tuyav.setNetworkStatus(result);
}
#endif

#ifdef WIFI_STREAM_ENABLE
/**
 * @brief  Streaming service sending results
 * @param[in] {result} result
 * @ref       0x00: success
 * @ref       0x01: Streaming service feature is not enabled
 * @ref       0x02: The streaming server is not connected successfully
 * @ref       0x03: Data push timeout
 * @ref       0x04: Wrong data length
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void stream_trans_send_result(unsigned char result)
{
#error "Complete the get the streaming service sending results code yourself and delete the line"
    switch (result)
    {
    case 0x00:
        //success
        break;

    case 0x01:
        //Streaming service feature is not enabled
        break;

    case 0x02:
        //The streaming server is not connected successfully
        break;

    case 0x03:
        //Data push timeout
        break;

    case 0x04:
        //Wrong data length
        break;

    default:
        break;
    }
}

/**
 * @brief  Multi-map streaming service sending results
 * @param[in] {result} result
 * @ref       0x00: success
 * @ref       0x01: fault
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void maps_stream_trans_send_result(unsigned char result)
{
#error "Complete the get the Multi-map streaming service sending results code yourself and delete the line"
    switch (result)
    {
    case 0x00:
        //success
        break;

    case 0x01:
        //fault
        break;

    default:
        break;
    }
}
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/**
 * @brief  Routing information receives notification of results
 * @param[in] {result} Whether the module has successfully received the correct routing information
 * @ref       0x00: failure
 * @ref       0x01: success
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void wifi_connect_test_result(unsigned char result)
{
#error "Please implement the successful/failed wifi function test processing code by yourself, and delete the line after completion"
    if (result == 0)
    {
        //The routing message failed to receive. Please check whether the routing packet sent is a complete JSON packet
    }
    else
    {
        //The routing information was received successfully. Please pay attention to the wifi working state of WIFI_STATE_CMD command
    }
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  Gets the module MAC result
 * @param[in] {mac} Module MAC data
 * @ref       mac[0]: is whether to get the MAC success flag,  0x00 success, 0x01 failure
 * @ref       mac[1]~mac[6]: When obtaining the MAC address flag bit MAC [0] is successful, it represents the valid MAC address of the module
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void mcu_get_mac(unsigned char mac[])
{
#error "Complete the MAC fetch processing code yourself and delete the line"
    /*
    mac[0] is whether to get the MAC success flag,  0x00 success, 0x01 failure
    mac[1]~mac[6]:When obtaining the MAC address flag bit MAC [0] is successful, it represents the valid MAC address of the module
   */

    if (mac[0] == 1)
    {
        //Error getting MAC
    }
    else
    {
        //Receive the MAC address returned by wifi module correctly
    }
}
#endif

#ifdef GET_IR_STATUS_ENABLE
/**
 * @brief  Obtain the infrared state results
 * @param[in] {result} Indicating infrared state
 * @ref       0x00: Infrared state 1 Sending infrared code
 * @ref       0x01: Infrared state 2 Send infrared code to end
 * @ref       0x02: Infrared state 3 Infrared learning begins
 * @ref       0x03: Infrared state 4 End of infrared learning
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void get_ir_status(unsigned char result)
{
#error "Please complete the infrared status processing code by yourself and delete the line"
    switch (result)
    {
    case 0:
        //Infrared state 1
        break;

    case 1:
        //Infrared state 2
        break;

    case 2:
        //Infrared state 3
        break;

    case 3:
        //Infrared state 4
        break;

    default:
        break;
    }

    wifi_uart_write_frame(GET_IR_STATUS_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef IR_TX_RX_TEST_ENABLE
/**
 * @brief  Notification of infrared incoming and outgoing test results
 * @param[in] {result} Whether the module successfully received the correct information
 * @ref       0x00: failure
 * @ref       0x01: success
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void ir_tx_rx_test_result(unsigned char result)
{
#error "Please implement the infrared entry, send, receive, and test functions by yourself. Please delete this line after completion"
    if (result == 0)
    {
        //Infrared into the transceiver production test successful
    }
    else
    {
        //Infrared incoming transceiver production test failed, please check the sent packet
    }
}
#endif

#ifdef FILE_DOWNLOAD_ENABLE
/**
 * @brief  File download package size selection
 * @param[in] {package_sz} File download package size
 * @ref       0x00: 256 byte
 * @ref       0x01: 512 byte
 * @ref       0x02: 1024 byte
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void file_download_package_choose(unsigned char package_sz)
{
#error "Please complete the file download package size selection processing code by yourself and delete the line"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
    wifi_uart_write_frame(FILE_DOWNLOAD_START_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  Package download
 * @param[in] {value} Data buffer
 * @param[in] {position} The current packet is at the file location
 * @param[in] {length} Current file package length (when the length is 0, the file package is sent)
 * @return Data processing results
 * -           0(ERROR): failure
 * -           1(SUCCESS): success
 * @note   MCU needs to implement this function by itself
 */
unsigned char file_download_handle(const unsigned char value[], unsigned long position, unsigned short length)
{
#error "Please complete the file download selection processing code by yourself and delete the line"
    if (length == 0)
    {
        //File package data sending completed
    }
    else
    {
        //File package data processing
    }

    return SUCCESS;
}
#endif

#ifdef MODULE_EXPANDING_SERVICE_ENABLE
/**
 * @brief  Module time service notification results
 * @param[in] {value} Data buffer
 * @param[in] {length} Data buffer length
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void open_module_time_serve_result(const unsigned char value[], unsigned short length)
{
#error "Please complete the Module time service notification results processing code by yourself and delete the line"
    unsigned char sub_cmd = value[0];

    switch (sub_cmd)
    {
    case 0x01:
    { //Subcommand  Turn on module time service notification
        if (0x02 != length)
        {
            //Data length error
            return;
        }

        if (value[1] == 0)
        {
            //Service started successfully
        }
        else
        {
            //Service startup failure
        }
    }
    break;

    case 0x02:
    { //Subcommand  Module time service notification
        if (0x09 != length)
        {
            //Data length error
            return;
        }

        unsigned char time_type = value[1]; //0x00:Green time    0x01:Local time
        unsigned char time_data[7];

        my_memcpy(time_data, value + 2, length - 2);
        /*
            Data[0] is the year and 0x00 is the year 2000.
            Data[1] is the month, starting from 1 to ending at 12
            Data[2] is the date, starting from 1 to 31
            Data[3] is the clock, starting from 0 to ending at 23
            Data[4] is minutes, starting from 0 to ending at 59
            Data[5] is seconds, starting from 0 to ending at 59
            Data[6] is the week, starting from 1 to 7 and 1 is Monday.
            */

        //Add the time data handling code here, with time_type as the time type

        unsigned short send_len = 0;
        send_len = set_wifi_uart_byte(send_len, sub_cmd);
        wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
    }
    break;

    case 0x03:
    { //Subcommand  Actively request weather service data
        if (0x02 != length)
        {
            //Data length error
            return;
        }

        if (value[1] == 0)
        {
            //success
        }
        else
        {
            //fault
        }
    }
    break;

    case 0x04:
    { //Subcommand  Open module reset status notification
        if (0x02 != length)
        {
            //Data length error
            return;
        }

        if (value[1] == 0)
        {
            //success
        }
        else
        {
            //fault
        }
    }
    break;

    case 0x05:
    { //Subcommand  Module reset status notification
        if (0x02 != length)
        {
            //Data length error
            return;
        }

        switch (value[1])
        {
        case 0x00:
            //Module local reset

            break;
        case 0x01:
            //APP remote reset

            break;
        case 0x02:
            //APP factory reset

            break;
        default:
            break;
        }

        unsigned short send_len = 0;
        send_len = set_wifi_uart_byte(send_len, sub_cmd);
        wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
    }
    break;

    default:
        break;
    }
}
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
/**
 * @brief  Bluetooth functional test results
 * @param[in] {value} Data buffer
 * @param[in] {length} Data buffer length
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void BLE_test_result(const unsigned char value[], unsigned short length)
{
#error "Please complete the Bluetooth functional test results processing code by yourself and delete the line"
    unsigned char sub_cmd = value[0];

    if (0x03 != length)
    {
        //Data length error
        return;
    }

    if (0x01 != sub_cmd)
    {
        //Subcommand error
        return;
    }

    unsigned char result = value[1];
    unsigned char rssi = value[2];

    if (result == 0)
    {
        //test failure
        if (rssi == 0x00)
        {
            //Bluetooth beacon named ty_mdev is not scanned, please check
        }
        else if (rssi == 0x01)
        {
            //Module is not authorized
        }
    }
    else if (result == 0x01)
    {
        //Module is not authorized
        //rssiis the signal strength (0-100, 0 signal is the worst, 100 signal is the strongest)
    }
}
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
/**
 * @brief  Get voice status code result
 * @param[in] {result} Voice status code
 * @ref       0x00: idle
 * @ref       0x01: mic mute state
 * @ref       0x02: wake
 * @ref       0x03: recording
 * @ref       0x04: Recognizing
 * @ref       0x05: Identified successfully
 * @ref       0x06: Recognition failed
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void get_voice_state_result(unsigned char result)
{
#error "Please complete the get voice status code result processing code by yourself and delete the line"
    switch (result)
    {
    case 0:
        //idle
        break;

    case 1:
        //mic mute state
        break;

    case 2:
        //wake
        break;

    case 3:
        //recording
        break;

    case 4:
        //Recognizing
        break;

    case 5:
        //Identified successfully
        break;

    case 6:
        //Recognition failed
        break;

    default:
        break;
    }
}

/**
 * @brief  MIC mute setting
 * @param[in] {result} Mute setting value
 * @ref       0x00: mic on
 * @ref       0x01: mic mute
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void set_voice_MIC_silence_result(unsigned char result)
{
#error "Please complete the MIC mute setting processing code by yourself and delete the line"
    if (result == 0)
    {
        //mic on
    }
    else
    {
        //mic mute
    }
}

/**
 * @brief  speaker volume setting result
 * @param[in] {result} Volume value
 * @ref       0~10: Volume range
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void set_speaker_voice_result(unsigned char result)
{
#error "Please complete the speaker volume setting result processing code by yourself and delete the line"
}

/**
 * @brief  Audio production test results
 * @param[in] {result} Audio production test status
 * @ref       0x00: Turn off audio production test
 * @ref       0x01: mic1 audio loop test
 * @ref       0x02: mic2 audio loop test
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void voice_test_result(unsigned char result)
{
#error "Please complete the audio production test results processing code by yourself and delete the line"
    if (result == 0x00)
    {
        //Turn off audio production test
    }
    else if (result == 0x01)
    {
        //mic1 audio loop test
    }
    else if (result == 0x02)
    {
        //mic2 audio loop test
    }
}

/**
 * @brief  Get wake up test result
 * @param[in] {result} Wake-up return value
 * @ref       0x00: Wake up successfully
 * @ref       0x01: Wake up failure (10s timeout failure)
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void voice_awaken_test_result(unsigned char result)
{
#error "Please complete the wake up the test results processing code by yourself and delete the line"
    if (result == 0x00)
    {
        //Wake up successfully
    }
    else if (result == 0x01)
    {
        //Wake up failure
    }
}

/**
 * @brief  Voice module extension function
 * @param[in] {value} Data buffer
 * @param[in] {length} Data length
 * @return Null
 * @note   MCU needs to implement this function by itself
 */
void voice_module_extend_fun(const unsigned char value[], unsigned short length)
{
    unsigned char sub_cmd = value[0];
    unsigned char play;
    unsigned char bt_play;
    unsigned short send_len = 0;

    switch (sub_cmd)
    {
    case 0x00:
    { //Subcommand  MCU function settings
        if (0x02 != length)
        {
            //Data length error
            return;
        }

        if (value[1] == 0)
        {
            //success
        }
        else
        {
            //fault
        }
    }
    break;

    case 0x01:
    { //Subcommand  Status notification
        if (0x02 > length)
        {
            //Data length error
            return;
        }

        unsigned char play = 0xff;
        unsigned char bt_play = 0xff;

        const char *str_buff = (const char *)&value[1];
        const char *str_result = NULL;

        str_result = strstr(str_buff, "play") + my_strlen("play") + 2;
        if (NULL == str_result)
        {
            //data error
            goto ERR_EXTI;
        }

        if (0 == memcmp(str_result, "true", my_strlen("true")))
        {
            play = 1;
        }
        else if (0 == memcmp(str_result, "false", my_strlen("false")))
        {
            play = 0;
        }
        else
        {
            //data error
            goto ERR_EXTI;
        }

        str_result = strstr(str_buff, "bt_play") + my_strlen("bt_play") + 2;
        if (NULL == str_result)
        {
            //data error
            goto ERR_EXTI;
        }

        if (0 == memcmp(str_result, "true", my_strlen("true")))
        {
            bt_play = 1;
        }
        else if (0 == memcmp(str_result, "false", my_strlen("false")))
        {
            bt_play = 0;
        }
        else
        {
            //data error
            goto ERR_EXTI;
        }

#error "Please complete the Voice module extension function code by yourself and delete the line"
        //MCU setting only supports "play/pause" "Bluetooth switch"
                     //play     play/pause function 1 (play) / 0 (pause)
             //bt_play  Bluetooth switch function 1 (on) / 0 (off)

            send_len = 0;
        send_len = set_wifi_uart_byte(send_len, sub_cmd);
        send_len = set_wifi_uart_byte(send_len, 0x00);
        wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
    }
    break;

    default:
        break;
    }

    return;

ERR_EXTI:
    send_len = 0;
    send_len = set_wifi_uart_byte(send_len, sub_cmd);
    send_len = set_wifi_uart_byte(send_len, 0x01);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
    return;
}
#endif
