/**********************************Copyright (c)**********************************
**                     All rights reserved (C), 2015-2020, Tuya
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    system.c
 * @author  Tuya Comprehensive Agreement Development Group
 * @version v2.5.5
 * @date    2020.6.1
 * @brief   UART data processing.The user does not need to care about what the file implements.
 */

#define SYSTEM_GLOBAL

#include "wifi.h"

extern const DOWNLOAD_CMD_S download_cmd[];

/**
 * @brief  Write wifi uart bytes
 * @param[in] {dest} UART send buffer starts writing the address
 * @param[in] {byte} Write byte value
 * @return UART send buffer ends write address
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;

    *obj = byte;
    dest += 1;

    return dest;
}

/**
 * @brief  Write wifi uart buffer
 * @param[in] {dest} UART send buffer starts writing the address
 * @param[in] {src} source address
 * @param[in] {len} Data length
 * @return UART send buffer ends write address
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, const unsigned char *src, unsigned short len)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;

    my_memcpy(obj, src, len);

    dest += len;
    return dest;
}

/**
 * @brief  Calculate checksum
 * @param[in] {pack} Data source pointer
 * @param[in] {pack_len} Need to calculate the length of the checksum data
 * @return checksum
 */
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
    unsigned short i;
    unsigned char check_sum = 0;

    for (i = 0; i < pack_len; i++)
    {
        check_sum += *pack++;
    }

    return check_sum; //265;
}

/**
 * @brief  Write continuous data to wifi uart
 * @param[in] {in} Send buffer pointer
 * @param[in] {len} Data transmission length
 * @return Null
 */
static void wifi_uart_write_data(unsigned char *in, unsigned short len)
{
    if ((NULL == in) || (0 == len))
    {
        return;
    }

    while (len--)
    {
        uart_transmit_output(*in);
        in++;
    }
}

/**
 * @brief  Send a frame of data to the wifi serial port
 * @param[in] {fr_type} Frame type
 * @param[in] {fr_ver} Frame version
 * @param[in] {len} Data length
 * @return Null
 */
void wifi_uart_write_frame(unsigned char fr_type, unsigned char fr_ver, unsigned short len)
{
    unsigned char check_sum = 0;

    wifi_uart_tx_buf[HEAD_FIRST] = 0x55;
    wifi_uart_tx_buf[HEAD_SECOND] = 0xaa;
    wifi_uart_tx_buf[PROTOCOL_VERSION] = fr_ver;
    wifi_uart_tx_buf[FRAME_TYPE] = fr_type;
    wifi_uart_tx_buf[LENGTH_HIGH] = len >> 8;
    wifi_uart_tx_buf[LENGTH_LOW] = len & 0xff;

    len += PROTOCOL_HEAD;
    check_sum = get_check_sum((unsigned char *)wifi_uart_tx_buf, len - 1);
    wifi_uart_tx_buf[len - 1] = check_sum;

    wifi_uart_write_data((unsigned char *)wifi_uart_tx_buf, len);
}

/**
 * @brief  Heartbeat packet detection
 * @param  Null
 * @return Null
 */
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

    wifi_uart_write_frame(HEAT_BEAT_CMD, MCU_TX_VER, length);
}

/**
 * @brief  Product information upload
 * @param  Null
 * @return Null
 */
static void product_info_update(void)
{
    unsigned char length = 0;
    unsigned char str[10] = {0};

    length = set_wifi_uart_buffer(length, "{\"p\":\"", my_strlen("{\"p\":\""));
    length = set_wifi_uart_buffer(length, (unsigned char *)PRODUCT_KEY, my_strlen((unsigned char *)PRODUCT_KEY));
    length = set_wifi_uart_buffer(length, "\",\"v\":\"", my_strlen("\",\"v\":\""));
    length = set_wifi_uart_buffer(length, (unsigned char *)MCU_VER, my_strlen((unsigned char *)MCU_VER));
    length = set_wifi_uart_buffer(length, "\",\"m\":", my_strlen("\",\"m\":"));
    length = set_wifi_uart_buffer(length, (unsigned char *)CONFIG_MODE, my_strlen((unsigned char *)CONFIG_MODE));
#ifdef CONFIG_MODE_DELAY_TIME
    sprintf((char *)str, ",\"mt\":%d", CONFIG_MODE_DELAY_TIME);
    length = set_wifi_uart_buffer(length, str, my_strlen(str));
#endif
#ifdef CONFIG_MODE_CHOOSE
    sprintf((char *)str, ",\"n\":%d", CONFIG_MODE_CHOOSE);
    length = set_wifi_uart_buffer(length, str, my_strlen(str));
#endif
#ifdef ENABLE_MODULE_IR_FUN
    sprintf((char *)str, ",\"ir\":\"%d.%d\"", MODULE_IR_PIN_TX, MODULE_IR_PIN_RX);
    length = set_wifi_uart_buffer(length, str, my_strlen(str));
#endif
#ifdef LONG_CONN_LOWPOWER
    sprintf((char *)str, ",\"low\":%d", LONG_CONN_LOWPOWER);
    length = set_wifi_uart_buffer(length, str, my_strlen(str));
#endif

    length = set_wifi_uart_buffer(length, "}", my_strlen("}"));

    wifi_uart_write_frame(PRODUCT_INFO_CMD, MCU_TX_VER, length);
}

/**
 * @brief  Query the working mode of mcu and wifi
 * @param  Null
 * @return Null
 */
static void get_mcu_wifi_mode(void)
{
    unsigned char length = 0;

#ifdef WIFI_CONTROL_SELF_MODE //Module self-processing
    length = set_wifi_uart_byte(length, WF_STATE_KEY);
    length = set_wifi_uart_byte(length, WF_RESERT_KEY);
#else
    //No need to process data
#endif

    wifi_uart_write_frame(WORK_MODE_CMD, MCU_TX_VER, length);
}

/**
 * @brief  Get the serial number of the DPID in the array
 * @param[in] {dpid} dpid
 * @return dp number
 */
static unsigned char get_dowmload_dpid_index(unsigned char dpid)
{
    unsigned char index;
    unsigned char total = get_download_cmd_total();

    for (index = 0; index < total; index++)
    {
        if (download_cmd[index].dp_id == dpid)
        {
            break;
        }
    }

    return index;
}

/**
 * @brief  Delivery data processing
 * @param[in] {value} Send data source pointer
 * @return Return data processing result
 */
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
/**
 * @brief  Weather data analysis
 * @param[in] {p_data} Receive data pointer
 * @param[in] {data_len} Receive data length
 * @return Null
 */
static void weather_data_raw_handle(const unsigned char p_data[], unsigned short data_len)
{
    int i = 1;
    int param_length = 0;
    char param[15] = {0};
    char day = 0;
    int type1 = 0;
    unsigned char value_string[100] = {0};
    int val_cnt = 0;
    int val_len = 0;

    if (p_data[0] != 1 || data_len < 1)
    {
        //Receive failed
        Serial.println("Received Failed");
    }
    else
    {
        if (data_len < 4)
        {
            //Data is empty
        }

        while (i < data_len)
        {
            param_length = p_data[i];
            my_memset(param, '\0', 15);
            my_memcpy(param, p_data + i + 3, param_length - 2);

            //day = p_data[i + can_len] - '0';

            type1 = p_data[i + 1 + param_length];
            if (type1 != 0 && type1 != 1)
            {
                return;
            }

            my_memset(value_string, '\0', 100);
            val_cnt = i + 1 + param_length + 1;
            val_len = p_data[val_cnt];
            if (type1 == 0)
            { //int32
                weather_data_user_handle(param, type1, p_data + val_cnt + 1, day);
            }
            else if (type1 == 1)
            { //string
                my_memcpy(value_string, p_data + val_cnt + 1, val_len);
                weather_data_user_handle(param, type1, value_string, day);
            }

            i += 1 + param_length + 1 + 1 + val_len;
        }
        tuyav.setWeatherReceived(true);
        wifi_uart_write_frame(WEATHER_DATA_CMD, 0, 0);
    }
}
#endif

#ifdef WIFI_STREAM_ENABLE
/**
 * @brief  Stream data transmission
 * @param[in] {id} Stream service identifier
 * @param[in] {offset} Offset
 * @param[in] {buffer} Data address
 * @param[in] {buf_len} Data length
 * @return Null
 * @note   Null
 */
unsigned char stream_trans(unsigned short id, unsigned int offset, unsigned char *buffer, unsigned short buf_len)
{
    unsigned short send_len = 0;

    stream_status = 0xff;

    if (stop_update_flag == ENABLE)
        return ERROR;

    //ID
    send_len = set_wifi_uart_byte(send_len, id / 0x100);
    send_len = set_wifi_uart_byte(send_len, id % 0x100);
    //Offset
    send_len = set_wifi_uart_byte(send_len, offset >> 24);
    send_len = set_wifi_uart_byte(send_len, offset >> 16);
    send_len = set_wifi_uart_byte(send_len, offset >> 8);
    send_len = set_wifi_uart_byte(send_len, offset % 256);
    //data
    send_len = set_wifi_uart_buffer(send_len, buffer, buf_len);
    wifi_uart_write_frame(STREAM_TRANS_CMD, MCU_TX_VER, send_len);
    return SUCCESS;
}

/**
 * @brief  Stream data transmission
 * @param[in] {pro_ver} Map service agreement version
 * @param[in] {id} Map stream service session id
 * @param[in] {sub_id} submap id
 * @param[in] {sub_id_pro_mode} Sub-map ID data processing method
 * @ref           0x00:Continue to accumulate
 * @ref           0x00:Clear uploaded data
 * @param[in] {offset} Offset
 * @param[in] {buffer} Data address
 * @param[in] {buf_len} Data length
 * @return Null
 * @note   Null
 */
unsigned char maps_stream_trans(unsigned char pro_ver, unsigned short id, unsigned char sub_id, unsigned char sub_id_pro_mode,
                                unsigned int offset, unsigned char *buffer, unsigned short buf_len)
{
    unsigned short send_len = 0;

    maps_stream_status = 0xff;

    if (stop_update_flag == ENABLE)
        return ERROR;

    //Map service agreement version
    send_len = set_wifi_uart_byte(send_len, pro_ver);

    //Map stream service session id
    send_len = set_wifi_uart_byte(send_len, id / 0x100);
    send_len = set_wifi_uart_byte(send_len, id % 0x100);

    //submap id
    send_len = set_wifi_uart_byte(send_len, sub_id);

    //Sub-map ID data processing method
    send_len = set_wifi_uart_byte(send_len, sub_id_pro_mode);

    //Offset
    send_len = set_wifi_uart_byte(send_len, offset >> 24);
    send_len = set_wifi_uart_byte(send_len, offset >> 16);
    send_len = set_wifi_uart_byte(send_len, offset >> 8);
    send_len = set_wifi_uart_byte(send_len, offset % 256);
    //Data
    send_len = set_wifi_uart_buffer(send_len, buffer, buf_len);
    wifi_uart_write_frame(MAPS_STREAM_TRANS_CMD, MCU_TX_VER, send_len);
    return SUCCESS;
}
#endif

/**
 * @brief  Data frame processing
 * @param[in] {offset} Data start position
 * @return Null
 */
void data_handle(unsigned short offset)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
    unsigned char *firmware_addr = NULL;
    static unsigned short firm_size;           //Upgrade package size
    static unsigned long firm_length;          //MCU upgrade file length
    static unsigned char firm_update_flag = 0; //MCU upgrade flag
    unsigned long dp_len;
    unsigned char firm_flag; //Upgrade package size flag
#else
    unsigned short dp_len;
#endif

    unsigned char ret;
    unsigned short i, total_len;
    unsigned char cmd_type = wifi_data_process_buf[offset + FRAME_TYPE];
    unsigned char result;

#ifdef WEATHER_ENABLE
    static unsigned char isWoSend = 0; //Whether the weather data has been opened, 0:no  1:yes
#endif

#ifdef WIFI_TEST_ENABLE
    unsigned char rssi;
#endif

#ifdef FILE_DOWNLOAD_ENABLE
    unsigned char *file_data_addr = NULL;
    static unsigned short file_package_size = 0; //File packet size
    static unsigned char file_download_flag = 0; //File download flag
    unsigned int file_download_size = 0;
#endif
    switch (cmd_type)
    {
    case HEAT_BEAT_CMD: //Heartbeat package
        heat_beat_check();
        break;

    case PRODUCT_INFO_CMD: //Product information
        product_info_update();
        break;

    case WORK_MODE_CMD: //Query the module working mode set by the MCU
        get_mcu_wifi_mode();
        break;
    case WIFI_STATE_CMD:
        result = wifi_data_process_buf[offset + DATA_START];
        get_wifi_status(result);
        /*Serial.println(WIFI_STATUS);
        Serial.println(status);*/
        //tuyav.setNetworkStatus((Network_Status)status);
#ifndef WIFI_CONTROL_SELF_MODE
    case WIFI_STATE_CMD: //Wifi working status
        wifi_work_state = wifi_data_process_buf[offset + DATA_START];
        wifi_uart_write_frame(WIFI_STATE_CMD, MCU_TX_VER, 0);
#ifdef WEATHER_ENABLE
        if (wifi_work_state == WIFI_CONNECTED && isWoSend == 0)
        { //When the WIFI connection is successful, open the weather data only once
            mcu_open_weather();
            isWoSend = 1;
        }
#endif
        break;

    case WIFI_RESET_CMD: //Reset wifi
        reset_wifi_flag = RESET_WIFI_SUCCESS;
        break;

    case WIFI_MODE_CMD: //Select smartconfig/AP mode
        set_wifimode_flag = SET_WIFICONFIG_SUCCESS;
        break;
#endif

    case DATA_QUERT_CMD: //Order send
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];

        for (i = 0; i < total_len;)
        {
            dp_len = wifi_data_process_buf[offset + DATA_START + i + 2] * 0x100;
            dp_len += wifi_data_process_buf[offset + DATA_START + i + 3];
            //
            ret = data_point_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START + i);

            if (SUCCESS == ret)
            {
                //Send success
            }
            else
            {
                //Send fault
            }

            i += (dp_len + 4);
        }
        break;

    case STATE_QUERY_CMD: //Status query
        all_data_update();
        break;

#ifdef SUPPORT_MCU_FIRM_UPDATE
    case UPDATE_START_CMD: //Upgrade start
        //Get global variable of upgrade package size
        firm_flag = PACKAGE_SIZE;
        if (firm_flag == 0)
        {
            firm_size = 256;
        }
        else if (firm_flag == 1)
        {
            firm_size = 512;
        }
        else if (firm_flag == 2)
        {
            firm_size = 1024;
        }

        firm_length = wifi_data_process_buf[offset + DATA_START];
        firm_length <<= 8;
        firm_length |= wifi_data_process_buf[offset + DATA_START + 1];
        firm_length <<= 8;
        firm_length |= wifi_data_process_buf[offset + DATA_START + 2];
        firm_length <<= 8;
        firm_length |= wifi_data_process_buf[offset + DATA_START + 3];

        upgrade_package_choose(PACKAGE_SIZE);
        firm_update_flag = UPDATE_START_CMD;
        break;

    case UPDATE_TRANS_CMD: //Upgrade transfer
        if (firm_update_flag == UPDATE_START_CMD)
        {
            //Stop all data reporting
            stop_update_flag = ENABLE;

            total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];

            dp_len = wifi_data_process_buf[offset + DATA_START];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 1];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 2];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 3];

            firmware_addr = (unsigned char *)wifi_data_process_buf;
            firmware_addr += (offset + DATA_START + 4);

            if ((total_len == 4) && (dp_len == firm_length))
            {
                //Last pack
                ret = mcu_firm_update_handle(firmware_addr, dp_len, 0);
                firm_update_flag = 0;
            }
            else if ((total_len - 4) <= firm_size)
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
                wifi_uart_write_frame(UPDATE_TRANS_CMD, MCU_TX_VER, 0);
            }
            //Restore all data reported
            stop_update_flag = DISABLE;
        }
        break;
#endif

#ifdef SUPPORT_GREEN_TIME
    case GET_ONLINE_TIME_CMD: //Get system time (Greenwich Mean Time)
        mcu_get_greentime((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
    case GET_LOCAL_TIME_CMD: //Get local time
        mcu_write_rtctime((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;
#endif

#ifdef WIFI_TEST_ENABLE
    case WIFI_TEST_CMD: //Wifi function test
        result = wifi_data_process_buf[offset + DATA_START];
        rssi = wifi_data_process_buf[offset + DATA_START + 1];
        wifi_test_result(result, rssi);
        break;
#endif

#ifdef WEATHER_ENABLE
    case WEATHER_OPEN_CMD: //Turn on the weather
        weather_open_return_handle(wifi_data_process_buf[offset + DATA_START], wifi_data_process_buf[offset + DATA_START + 1]);
        break;

    case WEATHER_DATA_CMD: //Weather data
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
        weather_data_raw_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START, total_len);
        break;
#endif

#ifdef WIFI_STREAM_ENABLE
    case STREAM_TRANS_CMD:                                          //Stream data transmission
        stream_status = wifi_data_process_buf[offset + DATA_START]; //Service transmission back to reception
        stream_trans_send_result(stream_status);
        break;

    case MAPS_STREAM_TRANS_CMD:                                          //streams trans(Support for multiple maps)
        maps_stream_status = wifi_data_process_buf[offset + DATA_START]; //Service transmission back to reception
        maps_stream_trans_send_result(maps_stream_status);
        break;
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
    case WIFI_CONNECT_TEST_CMD: //Wifi function test(connection designated route)
        result = wifi_data_process_buf[offset + DATA_START];
        wifi_connect_test_result(result);
        break;
#endif

#ifdef GET_MODULE_MAC_ENABLE
    case GET_MAC_CMD: //Get module mac
        mcu_get_mac((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;
#endif

#ifdef GET_WIFI_STATUS_ENABLE
    case GET_WIFI_STATUS_CMD: //Gets the wifi networking status
        result = wifi_data_process_buf[offset + DATA_START];
        get_wifi_status(result);
        break;
#endif

#ifdef MCU_DP_UPLOAD_SYN
    case STATE_UPLOAD_SYN_RECV_CMD: //Status upload results(synchronization)
        result = wifi_data_process_buf[offset + DATA_START];
        get_upload_syn_result(result);
        break;
#endif

#ifdef GET_IR_STATUS_ENABLE
    case GET_IR_STATUS_CMD: //IR status notification
        result = wifi_data_process_buf[offset + DATA_START];
        get_ir_status(result);
        break;
#endif

#ifdef IR_TX_RX_TEST_ENABLE
    case IR_TX_RX_TEST_CMD: //IR into send-receive test
        result = wifi_data_process_buf[offset + DATA_START];
        ir_tx_rx_test_result(result);
        break;
#endif

#ifdef FILE_DOWNLOAD_ENABLE
    case FILE_DOWNLOAD_START_CMD: //File download startup
        //Get file package size selection
        if (FILE_DOWNLOAD_PACKAGE_SIZE == 0)
        {
            file_package_size = 256;
        }
        else if (FILE_DOWNLOAD_PACKAGE_SIZE == 1)
        {
            file_package_size = 512;
        }
        else if (FILE_DOWNLOAD_PACKAGE_SIZE == 2)
        {
            file_package_size = 1024;
        }

        file_download_size = wifi_data_process_buf[offset + DATA_START];
        file_download_size = (file_download_size << 8) | wifi_data_process_buf[offset + DATA_START + 1];
        file_download_size = (file_download_size << 8) | wifi_data_process_buf[offset + DATA_START + 2];
        file_download_size = (file_download_size << 8) | wifi_data_process_buf[offset + DATA_START + 3];

        file_download_package_choose(FILE_DOWNLOAD_PACKAGE_SIZE);
        file_download_flag = FILE_DOWNLOAD_START_CMD;
        break;

    case FILE_DOWNLOAD_TRANS_CMD: //File download data transfer
        if (file_download_flag == FILE_DOWNLOAD_START_CMD)
        {
            total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];

            dp_len = wifi_data_process_buf[offset + DATA_START];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 1];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 2];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 3];

            file_data_addr = (unsigned char *)wifi_data_process_buf;
            file_data_addr += (offset + DATA_START + 4);

            if ((total_len == 4) && (dp_len == file_download_size))
            {
                //Last pack
                ret = file_download_handle(file_data_addr, dp_len, 0);
                file_download_flag = 0;
            }
            else if ((total_len - 4) <= file_package_size)
            {
                ret = file_download_handle(file_data_addr, dp_len, total_len - 4);
            }
            else
            {
                file_download_flag = 0;
                ret = ERROR;
            }

            if (ret == SUCCESS)
            {
                wifi_uart_write_frame(FILE_DOWNLOAD_TRANS_CMD, MCU_TX_VER, 0);
            }
        }
        break;
#endif

#ifdef MODULE_EXPANDING_SERVICE_ENABLE
    case MODULE_EXTEND_FUN_CMD: //Module expansion service
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
        open_module_time_serve_result((unsigned char *)(wifi_data_process_buf + offset + DATA_START), total_len);
        break;
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
    case BLE_TEST_CMD: //Bluetooth functional test（Scan designated bluetooth beacon）
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
        BLE_test_result((unsigned char *)(wifi_data_process_buf + offset + DATA_START), total_len);
        break;
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
    case GET_VOICE_STATE_CMD: //Gets the voice status code
        result = wifi_data_process_buf[offset + DATA_START];
        get_voice_state_result(result);
        break;
    case MIC_SILENCE_CMD: //MIC mute Settings
        result = wifi_data_process_buf[offset + DATA_START];
        set_voice_MIC_silence_result(result);
        break;
    case SET_SPEAKER_VOLUME_CMD: //speaker volume set
        result = wifi_data_process_buf[offset + DATA_START];
        set_speaker_voice_result(result);
        break;
    case VOICE_TEST_CMD: //Audio production test
        result = wifi_data_process_buf[offset + DATA_START];
        voice_test_result(result);
        break;
    case VOICE_AWAKEN_TEST_CMD: //Wake up production test
        result = wifi_data_process_buf[offset + DATA_START];
        voice_awaken_test_result(result);
        break;
    case VOICE_EXTEND_FUN_CMD: //Voice module extension function
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
        voice_module_extend_fun((unsigned char *)(wifi_data_process_buf + offset + DATA_START), total_len);
        break;
#endif

    default:
        break;
    }
}

/**
 * @brief  Determines whether there is data in the uart receive buffer
 * @param  Null
 * @return Is there data
 */
unsigned char with_data_rxbuff(void)
{
    if (rx_buf_in != rx_buf_out)
        return 1;
    else
        return 0;
}

/**
 * @brief  Read uart receive buffer 1 byte data
 * @param  Null
 * @return Read the data
 */
unsigned char take_byte_rxbuff(void)
{
    unsigned char value;

    if (rx_buf_out != rx_buf_in)
    {
        //With data
        if (rx_buf_out >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf)))
        {
            //The data has reached the end
            rx_buf_out = (unsigned char *)(wifi_uart_rx_buf);
        }

        value = *rx_buf_out++;
    }

    return value;
}
