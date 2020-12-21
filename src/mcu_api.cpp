/**********************************Copyright (c)**********************************
**                     All rights reserved (C), 2015-2020, Tuya
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    mcu_api.c
 * @author  Tuya Comprehensive Agreement Development Group
 * @version v2.5.5
 * @date    2020.6.1
 * @brief   The functions that the user needs to actively call are in this file.
 */


#define MCU_API_GLOBAL

#include "wifi.h"

/**
 * @brief  hex to bcd
 * @param[in] {Value_H} High Byte
 * @param[in] {Value_L} Low Byte
 * @return Converted data
 */
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L)
{
    unsigned char bcd_value;
    
    if((Value_H >= '0') && (Value_H <= '9'))
        Value_H -= '0';
    else if((Value_H >= 'A') && (Value_H <= 'F'))
        Value_H = Value_H - 'A' + 10;
    else if((Value_H >= 'a') && (Value_H <= 'f'))
        Value_H = Value_H - 'a' + 10;
     
    bcd_value = Value_H & 0x0f;
    
    bcd_value <<= 4;
    if((Value_L >= '0') && (Value_L <= '9'))
        Value_L -= '0';
    else if((Value_L >= 'A') && (Value_L <= 'F'))
        Value_L = Value_L - 'a' + 10;
    else if((Value_L >= 'a') && (Value_L <= 'f'))
        Value_L = Value_L - 'a' + 10;
    
    bcd_value |= Value_L & 0x0f;

    return bcd_value;
}

/**
 * @brief  Calculate string length
 * @param[in] {str} String address
 * @return data length
 */
unsigned long my_strlen(unsigned char *str)  
{
    unsigned long len = 0;
    if(str == NULL) { 
        return 0;
    }
    
    for(len = 0; *str ++ != '\0'; ) {
        len ++;
    }
    
    return len;
}

/**
 * @brief  Set the first count bytes of the memory area pointed to by src to the character c
 * @param[out] {src} source address
 * @param[in] {ch} Set character
 * @param[in] {count} Set the data length
 * @return Source address after data processing
 */
void *my_memset(void *src,unsigned char ch,unsigned short count)
{
    unsigned char *tmp = (unsigned char *)src;
    
    if(src == NULL) {
        return NULL;
    }
    
    while(count --) {
        *tmp ++ = ch;
    }
    
    return src;
}

/**
 * @brief  Memory copy
 * @param[out] {dest} target address
 * @param[in] {src} source address
 * @param[in] {count} number of data copies
 * @return Source address after data processing
 */
void *my_memcpy(void *dest, const void *src, unsigned short count)  
{  
    unsigned char *pdest = (unsigned char *)dest;  
    const unsigned char *psrc  = (const unsigned char *)src;  
    unsigned short i;
    
    if(dest == NULL || src == NULL) { 
        return NULL;
    }
    
    if((pdest <= psrc) || (pdest > psrc + count)) {  
        for(i = 0; i < count; i ++) {  
            pdest[i] = psrc[i];  
        }  
    }else {
        for(i = count; i > 0; i --) {  
            pdest[i - 1] = psrc[i - 1];  
        }  
    }  
    
    return dest;  
}

/**
 * @brief  String copy
 * @param[in] {dest} target address
 * @param[in] {src} source address
 * @return Source address after data processing
 */
char *my_strcpy(char *dest, const char *src)  
{
    char *p = dest;
    while(*src!='\0') {
        *dest++ = *src++;
    }
    *dest = '\0';
    return p;
}

/**
 * @brief  String compare
 * @param[in] {s1} String 1
 * @param[in] {s2} String 2
 * @return Size comparison value
 * -             0:s1=s2
 * -             <0:s1<s2
 * -             >0:s1>s2
 */
int my_strcmp(char *s1 , char *s2)
{
    while( *s1 && *s2 && *s1 == *s2 ) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

/**
 * @brief  Split the int type into four bytes
 * @param[in] {number} 4 bytes of original data
 * @param[out] {value} 4 bytes of data after processing is completed
 * @return Null
 */
void int_to_byte(unsigned long number,unsigned char value[4])
{
    value[0] = number >> 24;
    value[1] = number >> 16;
    value[2] = number >> 8;
    value[3] = number & 0xff;
}

/**
 * @brief  Combine 4 bytes into 1 32bit variable
 * @param[in] {value} 4 bytes of data after processing is completed
 * @return 32bit variable after the merge is completed
 */
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

/**
 * @brief  raw type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @param[in] {len} data length
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
      return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_RAW);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  bool type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_BOOL);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    //
    if(value == FALSE) {
        send_len = set_wifi_uart_byte(send_len,FALSE);
    }else {
        send_len = set_wifi_uart_byte(send_len,1);
    }
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);
    
    return SUCCESS;
}

/**
 * @brief  value type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_VALUE);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,4);
    //
    send_len = set_wifi_uart_byte(send_len,value >> 24);
    send_len = set_wifi_uart_byte(send_len,value >> 16);
    send_len = set_wifi_uart_byte(send_len,value >> 8);
    send_len = set_wifi_uart_byte(send_len,value & 0xff);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  string type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @param[in] {len} data length
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_STRING);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  enum type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_ENUM);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    //
    send_len = set_wifi_uart_byte(send_len,value);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  fault type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value)
{
    unsigned short send_len = 0;
     
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_BITMAP);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    
    if((value | 0xff) == 0xff) {
        send_len = set_wifi_uart_byte(send_len,1);
        send_len = set_wifi_uart_byte(send_len,value);
    }else if((value | 0xffff) == 0xffff) {
        send_len = set_wifi_uart_byte(send_len,2);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }else {
        send_len = set_wifi_uart_byte(send_len,4);
        send_len = set_wifi_uart_byte(send_len,value >> 24);
        send_len = set_wifi_uart_byte(send_len,value >> 16);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }    
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}

#ifdef MCU_DP_UPLOAD_SYN
/**
 * @brief  raw type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @param[in] {len} data length
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update_syn(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_RAW);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  bool type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update_syn(unsigned char dpid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_BOOL);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    //
    if(value == FALSE) {
        send_len = set_wifi_uart_byte(send_len,FALSE);
    }else {
        send_len = set_wifi_uart_byte(send_len,1);
    }
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD, MCU_TX_VER, send_len);
    
    return SUCCESS;
}

/**
 * @brief  value type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update_syn(unsigned char dpid,unsigned long value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_VALUE);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,4);
    //
    send_len = set_wifi_uart_byte(send_len,value >> 24);
    send_len = set_wifi_uart_byte(send_len,value >> 16);
    send_len = set_wifi_uart_byte(send_len,value >> 8);
    send_len = set_wifi_uart_byte(send_len,value & 0xff);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD, MCU_TX_VER, send_len);
    
    return SUCCESS;
}

/**
 * @brief  string type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @param[in] {len} data length
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update_syn(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_STRING);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  enum type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update_syn(unsigned char dpid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_ENUM);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    
    send_len = set_wifi_uart_byte(send_len,value);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  fault type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update_syn(unsigned char dpid,unsigned long value)
{
    unsigned short send_len = 0;
     
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_BITMAP);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    
    if((value | 0xff) == 0xff) {
        send_len = set_wifi_uart_byte(send_len,1);
        send_len = set_wifi_uart_byte(send_len,value);
    }else if((value | 0xffff) == 0xffff) {
        send_len = set_wifi_uart_byte(send_len,2);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }else {
        send_len = set_wifi_uart_byte(send_len,4);
        send_len = set_wifi_uart_byte(send_len,value >> 24);
        send_len = set_wifi_uart_byte(send_len,value >> 16);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }    
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,MCU_TX_VER,send_len);

    return SUCCESS;
}
#endif

/**
 * @brief  mcu gets bool type to send dp value
 * @param[in] {value} dp data buffer address
 * @param[in] {len} data length
 * @return current dp value
 * @note   Null
 */
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

/**
 * @brief  mcu gets enum type to send dp value
 * @param[in] {value} dp data buffer address
 * @param[in] {len} data length
 * @return current dp value
 * @note   Null
 */
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

/**
 * @brief  mcu gets value type to send dp value
 * @param[in] {value} dp data buffer address
 * @param[in] {len} data length
 * @return current dp value
 * @note   Null
 */
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len)
{
    return(byte_to_int(value));
}

/**
 * @brief  Receive data processing
 * @param[in] {value} UART receives byte data
 * @return Null
 * @note   Call this function in the MCU serial receive function and pass the received data as a parameter.
 */
void uart_receive_input(unsigned char value)
{
    //#error "Please call uart_receive_input(value) in the serial port receive interrupt. The serial port data is processed by MCU_SDK. The user should not process it separately. Delete the line after completion."
    
    if(1 == rx_buf_out - rx_buf_in) {
        //UART receive buffer is full
    }else if((rx_buf_in > rx_buf_out) && ((rx_buf_in - rx_buf_out) >= sizeof(wifi_uart_rx_buf))) {
        //UART receive buffer is full
    }else {
        //UART receive buffer is not full
        if(rx_buf_in >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf))) {
            rx_buf_in = (unsigned char *)(wifi_uart_rx_buf);
        }
        
        *rx_buf_in ++ = value;
    }
}

/**
 * @brief  Wifi serial port processing service
 * @param  Null
 * @return Null
 * @note   Call this function in the MCU main function while loop
 */
void wifi_uart_service(void)
{
    //#error "Please add wifi_uart_service() directly in the while(1){} of the main function, call this function without adding any conditional judgment, delete the line after completion." 
    static unsigned short rx_in = 0;
    unsigned short offset = 0;
    unsigned short rx_value_len = 0;
    
    while((rx_in < sizeof(wifi_data_process_buf)) && with_data_rxbuff() > 0) {
        wifi_data_process_buf[rx_in ++] = take_byte_rxbuff();
    }
    
    if(rx_in < PROTOCOL_HEAD)
        return;
    
    while((rx_in - offset) >= PROTOCOL_HEAD) {
        if(wifi_data_process_buf[offset + HEAD_FIRST] != FRAME_FIRST) {
            offset ++;
            continue;
        }
        
        if(wifi_data_process_buf[offset + HEAD_SECOND] != FRAME_SECOND) {
            offset ++;
            continue;
        }  
        
        if(wifi_data_process_buf[offset + PROTOCOL_VERSION] != MCU_RX_VER) {
            offset += 2;
            continue;
        }      
        
        rx_value_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
        rx_value_len += (wifi_data_process_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);
        if(rx_value_len > sizeof(wifi_data_process_buf) + PROTOCOL_HEAD) {
            offset += 3;
            continue;
        }
        
        if((rx_in - offset) < rx_value_len) {
            break;
        }
        
        //Data reception completed
        if(get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1) != wifi_data_process_buf[offset + rx_value_len - 1]) {
            //Verification error
            printf("crc error (crc:0x%X  but data:0x%X)\r\n",get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1),wifi_data_process_buf[offset + rx_value_len - 1]);
            offset += 3;
            continue;
        }
        
        data_handle(offset);
        offset += rx_value_len;
    }//end while

    rx_in -= offset;
    if(rx_in > 0) {
        my_memcpy((char *)wifi_data_process_buf, (const char *)wifi_data_process_buf + offset, rx_in);
    }
}

/**
 * @brief  Protocol serial port initialization function
 * @param  Null
 * @return Null
 * @note   This function must be called in the MCU initialization code
 */
void wifi_protocol_init(void)
{
    //#error " Please add wifi_protocol_init() in the main function to complete the wifi protocol initialization and delete the line."
    rx_buf_in = (unsigned char *)wifi_uart_rx_buf;
    rx_buf_out = (unsigned char *)wifi_uart_rx_buf;
    
#ifndef WIFI_CONTROL_SELF_MODE
    wifi_work_state = WIFI_SATE_UNKNOW;
#endif
}



#ifndef WIFI_CONTROL_SELF_MODE
/**
 * @brief  MCU gets reset wifi success flag
 * @param  Null
 * @return Reset flag
 * -           0(RESET_WIFI_ERROR):failure
 * -           1(RESET_WIFI_SUCCESS):success
 * @note   1:The MCU actively calls mcu_reset_wifi() and calls this function to get the reset state.
 *         2:If the module is in self-processing mode, the MCU does not need to call this function.
 */
unsigned char mcu_get_reset_wifi_flag(void)
{
    return reset_wifi_flag;
}

/**
 * @brief  MCU actively resets wifi working mode
 * @param  Null
 * @return Null
 * @note   1:The MCU actively calls to obtain whether the reset wifi is successful through the mcu_get_reset_wifi_flag() function.
 *         2:If the module is in self-processing mode, the MCU does not need to call this function.
 */
void mcu_reset_wifi(void)
{
    reset_wifi_flag = RESET_WIFI_ERROR;
    
    wifi_uart_write_frame(WIFI_RESET_CMD, MCU_TX_VER, 0);
}

/**
 * @brief  Get set wifi status success flag
 * @param  Null
 * @return wifimode flag
 * -           0(SET_WIFICONFIG_ERROR):failure
 * -           1(SET_WIFICONFIG_SUCCESS):success
 * @note   1:The MCU actively calls to obtain whether the reset wifi is successful through the mcu_get_reset_wifi_flag() function.
 *         2:If the module is in self-processing mode, the MCU does not need to call this function.
 */
unsigned char mcu_get_wifimode_flag(void)
{
    return set_wifimode_flag;
}

/**
 * @brief  MCU set wifi working mode
 * @param[in] {mode} enter mode
 * @ref        0(SMART_CONFIG):enter smartconfig mode
 * @ref        1(AP_CONFIG):enter AP mode
 * @return Null
 * @note   1:MCU active call
 *         2:After success, it can be judged whether set_wifi_config_state is TRUE; TRUE means successful setting of wifi working mode
 *         3:If the module is in self-processing mode, the MCU does not need to call this function.
 */
void mcu_set_wifi_mode(unsigned char mode)
{
    unsigned char length = 0;
    
    set_wifimode_flag = SET_WIFICONFIG_ERROR;
    
    length = set_wifi_uart_byte(length, mode);
    
    wifi_uart_write_frame(WIFI_MODE_CMD, MCU_TX_VER, length);
}

/**
 * @brief  The MCU actively obtains the current wifi working status.
 * @param  Null
 * @return wifi work state
 * -          SMART_CONFIG_STATE:smartconfig config status
 * -          AP_STATE:AP config status
 * -          WIFI_NOT_CONNECTED:WIFI configuration succeeded but not connected to the router
 * -          WIFI_CONNECTED:WIFI configuration is successful and connected to the router
 * -          WIFI_CONN_CLOUD:WIFI is connected to the cloud server
 * -          WIFI_LOW_POWER:WIFI is in low power mode
 * -          SMART_AND_AP_STATE: WIFI smartconfig&AP mode
 * @note   1:If the module is in self-processing mode, the MCU does not need to call this function.
 */
unsigned char mcu_get_wifi_work_state(void)
{
    return wifi_work_state;
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU gets green time
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_get_green_time(void)
{
    wifi_uart_write_frame(GET_ONLINE_TIME_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU gets system time for proofreading local clock
 * @param  Null
 * @return Null
 * @note   The rtc clock is checked in the mcu_write_rtctime function after the MCU is actively called.
 */
void mcu_get_system_time(void)
{
    wifi_uart_write_frame(GET_LOCAL_TIME_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  Mcu initiated wifi function test
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_start_wifitest(void)
{
    wifi_uart_write_frame(WIFI_TEST_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef WIFI_HEARTSTOP_ENABLE
/**
 * @brief  Notify the WIFI module to turn off the heartbeat
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void wifi_heart_stop(void)
{
    wifi_uart_write_frame(HEAT_BEAT_STOP, MCU_TX_VER, 0);
}
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  Get the current wifi connection status
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_get_wifi_connect_status(void)
{
    wifi_uart_write_frame(GET_WIFI_STATUS_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef WIFI_STREAM_ENABLE
/**
 * @brief  Stream service delivery
 * @param[in] {id} ID number
 * @param[in] {buffer} The address at which the packet is sent
 * @param[in] {buf_len} Length of packet sent
 * @return Stream_trans result
 * -           0(ERROR): failure
 * -           1(SUCCESS): success
 * @note   MCU needs to implement this function by itself
 */
unsigned char stream_trans_send(unsigned int id, unsigned char *buffer, unsigned long buf_len)
{
    unsigned long map_offset = 0;
    unsigned int pack_num = 0;
    unsigned int rest_length = 0;

    if(stop_update_flag == ENABLE)
        return ERROR;

    pack_num = buf_len / STREM_PACK_LEN;
    rest_length = buf_len - pack_num * STREM_PACK_LEN;
    if (rest_length > 0) {
        pack_num++;
    }

    int this_len = STREM_PACK_LEN;
    for (int cnt = 0; cnt < pack_num; cnt++,map_offset += this_len) {
        if (cnt == pack_num - 1 && rest_length > 0) {
            this_len = rest_length;
        }else {
            this_len = STREM_PACK_LEN;
        }

        if(ERROR == stream_trans(id, map_offset, buffer + map_offset, this_len)) {
            //The MCU is being upgraded and cannot be streamed
            //printf("is upgrade\n");
            return ERROR;
        }
        
        #error "Please complete the Stream service delivery code by yourself and delete the line"
        /*  If the MCU can use multiple processes or threads, you can put this function in a separate process or thread to execute, and open the code shown below  */
        /*
        while(0xff == stream_status); //Wait for the module to reply
        
        if(0x00 != stream_status) {
            //fault!!!  You can check the error code in the stream_trans_send_result function.
            return ERROR;
        }
        */
        
        /*  If the MCU does not support multi-process or multi-thread, only one packet is sent at a time when using this function (data length is not greater than STREM_PACK_LEN),
            After sending, process the result returned by the module in the stream_trans_send_result function. 
            When the transmission is successful, call this function again to send the next packet. Note that the packet offset should be increased.  */
    }
    
    return SUCCESS;
}

/**
 * @brief  Multi-map stream service delivery
 * @param[in] {id} Map ID number
 * @param[in] {sub_id} Submap ID number
 * @param[in] {sub_id_pro_mode} Map ID data processing method
 * @ref       0x00: Continue to accumulate
 * @ref       0x01: Clear the data uploaded by the submap ID number
 * @param[in] {buffer} The address at which the packet is sent
 * @param[in] {buf_len} Length of packet sent
 * @return Stream_trans result
 * -           0(ERROR): failure
 * -           1(SUCCESS): success
 * @note   MCU needs to implement this function by itself
 */
unsigned char maps_stream_trans_send(unsigned int id, unsigned char sub_id, unsigned char sub_id_pro_mode, unsigned char *buffer, unsigned long buf_len)
{
    unsigned long map_offset = 0;
    unsigned int pack_num = 0;
    unsigned int rest_length = 0;
    unsigned char pro_ver = 0; //Map service protocol version, default is 0

    if(stop_update_flag == ENABLE)
        return SUCCESS;

    pack_num = buf_len / STREM_PACK_LEN;
    rest_length = buf_len - pack_num * STREM_PACK_LEN;
    if (rest_length > 0) {
        pack_num++;
    }

    int this_len = STREM_PACK_LEN;
    for (int cnt = 0; cnt < pack_num; cnt++,map_offset += this_len) {
        if (cnt == pack_num - 1 && rest_length > 0) {
            this_len = rest_length;
        }else {
            this_len = STREM_PACK_LEN;
        }

        if(ERROR == maps_stream_trans(pro_ver, id, sub_id, sub_id_pro_mode, map_offset, buffer + map_offset, this_len)) {
            //The MCU is being upgraded and cannot be streamed
            //printf("is upgrade\n");
            return ERROR;
        }
        
        #error "Please complete the Stream service delivery code by yourself and delete the line"
        /*  If the MCU can use multiple processes or threads, you can put this function in a separate process or thread to execute, and open the code shown below  */
        /*
        while(0xff == maps_stream_status); //Wait for the module to reply
        
        if(0x00 != maps_stream_status) {
            //fault!!!  You can check the error code in the maps_stream_trans_send_result function.
            return ERROR;
        }
        */
        
        /*  If the MCU does not support multi-process or multi-thread, only one packet is sent at a time when using this function (data length is not greater than STREM_PACK_LEN),
            After sending, process the result returned by the module in the maps_stream_trans_send_result function. 
            When the transmission is successful, call this function again to send the next packet. Note that the packet offset should be increased.  */
    }
    
    return SUCCESS;
}
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/**
 * @brief  mcu initiate wifi function test (connection specified route)
 * @param[in] {ssid_buf} Address to hold the router name string data (ssid length up to 32 bytes)
 * @param[in] {passwd_buffer} The address where the router name string data is stored (passwd is up to 64 bytes long)
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_start_connect_wifitest(unsigned char *ssid_buf,unsigned char *passwd_buffer)
{
    unsigned short send_len = 0;

    if( my_strlen(ssid_buf) > 32 || my_strlen(passwd_buffer) > 64) {
        //printf("ssid_buf or passwd_buffer is too long!");
        return;
    }
    
    send_len = set_wifi_uart_buffer(send_len, "{\"ssid\":\"", my_strlen("{\"ssid\":\""));
    send_len = set_wifi_uart_buffer(send_len,ssid_buf,my_strlen(ssid_buf));
    send_len = set_wifi_uart_buffer(send_len, "\",\"password\":\"", my_strlen("\",\"password\":\""));
    send_len = set_wifi_uart_buffer(send_len,passwd_buffer,my_strlen(passwd_buffer));
    send_len = set_wifi_uart_buffer(send_len, "\"}", my_strlen("\"}"));

    wifi_uart_write_frame(WIFI_CONNECT_TEST_CMD, MCU_TX_VER, send_len);
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  Get module MAC
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_get_module_mac(void)
{
    wifi_uart_write_frame(GET_MAC_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef IR_TX_RX_TEST_ENABLE
/**
 * @brief  MCU initiated infrared into the transceiver production test
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_start_ir_test(void)
{
    wifi_uart_write_frame(IR_TX_RX_TEST_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef MODULE_EXPANDING_SERVICE_ENABLE
/**
 * @brief  Open the module time service notification
 * @param[in] {time_type} Type of time
 * @ref       0x00: Green time
 * @ref       0x01: Local time
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void open_module_time_serve(unsigned char time_type)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, 0x01);
    send_len = set_wifi_uart_byte(send_len, time_type);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  Actively request weather service data
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void request_weather_serve(void)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, 0x03);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  Open module reset status notification
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void open_module_reset_state_serve(void)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, 0x04);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
}
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
/**
 * @brief  mcu initiates Bluetooth functional test (scanning specified Bluetooth beacons)
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_start_BLE_test(void)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, 0x01);
    wifi_uart_write_frame(BLE_TEST_CMD, MCU_TX_VER, send_len);
}
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
/**
 * @brief  Gets the voice status code
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void get_voice_state(void)
{
    wifi_uart_write_frame(GET_VOICE_STATE_CMD, MCU_TX_VER, 0);
}

/**
 * @brief  MIC mute setting
 * @param[in] {set_val} Mute setting value
 * @ref       0x00: mic on
 * @ref       0x01: mic mute
 * @ref       0xA0: Query the mute status
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void set_voice_MIC_silence(unsigned char set_val)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, set_val);
    wifi_uart_write_frame(MIC_SILENCE_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  Speaker volume setting
 * @param[in] {set_val} Volume set value
 * @ref       0~10: Volume range
 * @ref       0xA0: Query volume value
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void set_speaker_voice(unsigned char set_val)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, set_val);
    wifi_uart_write_frame(SET_SPEAKER_VOLUME_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  Audio production test
 * @param[in] {set_val} Audio production value
 * @ref       0x00: Turn off audio production test
 * @ref       0x01: mic1 audio loop test
 * @ref       0x02: mic2 audio loop test
 * @ref       0xA0: Query the current audio production test status
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void voice_test(unsigned char test_val)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, test_val);
    wifi_uart_write_frame(VOICE_TEST_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  Wake up test
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void voice_awaken_test(void)
{
    wifi_uart_write_frame(VOICE_AWAKEN_TEST_CMD, MCU_TX_VER, 0);
}

/**
 * @brief  Voice module MCU function settings
 * @param[in] {play} Play/pause function  1(play) / 0(pause)
 * @param[in] {bt_play} Bluetooth switch function  1(on) / 0(off)
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void voice_mcu_fun_set(unsigned char play, unsigned char bt_play)
{
    unsigned short send_len = 0;
    unsigned char str[50];
    
    //MCU settings only support "play/pause" "Bluetooth switch"
    
    sprintf((char*)str,"{\"play\":%s,\"bt_play\":%s}", play?"true":"false", bt_play?"true":"false");
    
    send_len = set_wifi_uart_byte(send_len, 0x00);
    send_len = set_wifi_uart_buffer(send_len, str, my_strlen(str));
    wifi_uart_write_frame(VOICE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
}
#endif


