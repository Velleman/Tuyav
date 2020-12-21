/**********************************Copyright (c)**********************************
**                     All rights reserved (C), 2015-2020, Tuya
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    mcu_api.h
 * @author  Tuya Comprehensive Agreement Development Group
 * @version v2.5.5
 * @date    2020.6.1
 * @brief   The functions that the user needs to actively call are in this file.
 */
 
 
#ifndef __MCU_API_H_
#define __MCU_API_H_


#ifdef MCU_API_GLOBAL
  #define MCU_API_EXTERN
#else
  #define MCU_API_EXTERN   extern
#endif

/**
 * @brief  hex to bcd
 * @param[in] {Value_H} High Byte
 * @param[in] {Value_L} Low Byte
 * @return Converted data
 */
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L);

/**
 * @brief  Calculate string length
 * @param[in] {str} String address
 * @return data length
 */
unsigned long my_strlen(unsigned char *str);

/**
 * @brief  Set the first count bytes of the memory area pointed to by src to the character c
 * @param[out] {src} source address
 * @param[in] {ch} Set character
 * @param[in] {count} Set the data length
 * @return Source address after data processing
 */
void *my_memset(void *src,unsigned char ch,unsigned short count);

/**
 * @brief  Memory copy
 * @param[out] {dest} target address
 * @param[in] {src} source address
 * @param[in] {count} number of data copies
 * @return Source address after data processing
 */
void *my_memcpy(void *dest, const void *src, unsigned short count);

/**
 * @brief  String copy
 * @param[in] {dest} target address
 * @param[in] {src} source address
 * @return Source address after data processing
 */
char *my_strcpy(char *dest, const char *src);

/**
 * @brief  String compare
 * @param[in] {s1} String 1
 * @param[in] {s2} String 2
 * @return Size comparison value
 * -             0:s1=s2
 * -             <0:s1<s2
 * -             >0:s1>s2
 */
int my_strcmp(char *s1 , char *s2);

/**
 * @brief  Split the int type into four bytes
 * @param[in] {number} 4 bytes of original data
 * @param[out] {value} 4 bytes of data after processing is completed
 * @return Null
 */
void int_to_byte(unsigned long number,unsigned char value[4]);

/**
 * @brief  Combine 4 bytes into 1 32bit variable
 * @param[in] {value} 4 bytes of data after processing is completed
 * @return 32bit variable after the merge is completed
 */
unsigned long byte_to_int(const unsigned char value[4]);

/**
 * @brief  raw type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @param[in] {len} data length
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
 * @brief  bool type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value);

/**
 * @brief  value type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value);

/**
 * @brief  string type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @param[in] {len} data length
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
 * @brief  enum type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value);

/**
 * @brief  fault type dp data upload
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value);

#ifdef MCU_DP_UPLOAD_SYN
/**
 * @brief  raw type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @param[in] {len} data length
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update_syn(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
 * @brief  bool type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update_syn(unsigned char dpid,unsigned char value);

/**
 * @brief  value type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update_syn(unsigned char dpid,unsigned long value);

/**
 * @brief  string type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @param[in] {len} data length
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update_syn(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
 * @brief  enum type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update_syn(unsigned char dpid,unsigned char value);

/**
 * @brief  fault type dp data upload syn
 * @param[in] {dpid} id number
 * @param[in] {value} Current dp value pointer
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update_syn(unsigned char dpid,unsigned long value);
#endif

/**
 * @brief  mcu gets bool type to send dp value
 * @param[in] {value} dp data buffer address
 * @param[in] {len} data length
 * @return current dp value
 * @note   Null
 */
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len);

/**
 * @brief  mcu gets enum type to send dp value
 * @param[in] {value} dp data buffer address
 * @param[in] {len} data length
 * @return current dp value
 * @note   Null
 */
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len);

/**
 * @brief  mcu gets value type to send dp value
 * @param[in] {value} dp data buffer address
 * @param[in] {len} data length
 * @return current dp value
 * @note   Null
 */
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len);

/**
 * @brief  Receive data processing
 * @param[in] {value} UART receives byte data
 * @return Null
 * @note   Call this function in the MCU serial receive function and pass the received data as a parameter.
 */
void uart_receive_input(unsigned char value);

/**
 * @brief  Wifi serial port processing service
 * @param  Null
 * @return Null
 * @note   Call this function in the MCU main function while loop
 */
void wifi_uart_service(void);

/**
 * @brief  Protocol serial port initialization function
 * @param  Null
 * @return Null
 * @note   This function must be called in the MCU initialization code
 */
void wifi_protocol_init(void);

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
unsigned char mcu_get_reset_wifi_flag(void);

/**
 * @brief  MCU actively resets wifi working mode
 * @param  Null
 * @return Null
 * @note   1:The MCU actively calls to obtain whether the reset wifi is successful through the mcu_get_reset_wifi_flag() function.
 *         2:If the module is in self-processing mode, the MCU does not need to call this function.
 */
void mcu_reset_wifi(void);

/**
 * @brief  Get set wifi status success flag
 * @param  Null
 * @return wifimode flag
 * -           0(SET_WIFICONFIG_ERROR):failure
 * -           1(SET_WIFICONFIG_SUCCESS):success
 * @note   1:The MCU actively calls to obtain whether the reset wifi is successful through the mcu_get_reset_wifi_flag() function.
 *         2:If the module is in self-processing mode, the MCU does not need to call this function.
 */
unsigned char mcu_get_wifimode_flag(void);

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
void mcu_set_wifi_mode(unsigned char mode);

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
unsigned char mcu_get_wifi_work_state(void);
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU gets green time
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_get_green_time(void);
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU gets system time for proofreading local clock
 * @param  Null
 * @return Null
 * @note   The rtc clock is checked in the mcu_write_rtctime function after the MCU is actively called.
 */
void mcu_get_system_time(void);
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  Mcu initiated wifi function test
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_start_wifitest(void);
#endif

#ifdef WIFI_HEARTSTOP_ENABLE
/**
 * @brief  Notify the WIFI module to turn off the heartbeat
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void wifi_heart_stop(void);
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  Get the current wifi connection status
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_get_wifi_connect_status(void);
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
unsigned char stream_trans_send(unsigned int id, unsigned char *buffer, unsigned long buf_len);

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
unsigned char maps_stream_trans_send(unsigned int id, unsigned char sub_id, unsigned char sub_id_pro_mode, unsigned char *buffer, unsigned long buf_len);
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/**
 * @brief  mcu initiate wifi function test (connection specified route)
 * @param[in] {ssid_buf} Address to hold the router name string data (ssid length up to 32 bytes)
 * @param[in] {passwd_buffer} The address where the router name string data is stored (passwd is up to 64 bytes long)
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_start_connect_wifitest(unsigned char *ssid_buf,unsigned char *passwd_buffer);
#endif

#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  Get module MAC
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_get_module_mac(void);
#endif

#ifdef IR_TX_RX_TEST_ENABLE
/**
 * @brief  MCU initiated infrared into the transceiver production test
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_start_ir_test(void);
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
void open_module_time_serve(unsigned char time_type);

/**
 * @brief  Actively request weather service data
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void request_weather_serve(void);

/**
 * @brief  Open module reset status notification
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void open_module_reset_state_serve(void);
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
/**
 * @brief  mcu initiates Bluetooth functional test (scanning specified Bluetooth beacons)
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void mcu_start_BLE_test(void);
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
/**
 * @brief  Gets the voice status code
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void get_voice_state(void);

/**
 * @brief  MIC mute setting
 * @param[in] {set_val} Mute setting value
 * @ref       0x00: mic on
 * @ref       0x01: mic mute
 * @ref       0xA0: Query the mute status
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void set_voice_MIC_silence(unsigned char set_val);

/**
 * @brief  Speaker volume setting
 * @param[in] {set_val} Volume set value
 * @ref       0~10: Volume range
 * @ref       0xA0: Query volume value
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void set_speaker_voice(unsigned char set_val);

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
void voice_test(unsigned char test_val);

/**
 * @brief  Wake up test
 * @param  Null
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void voice_awaken_test(void);

/**
 * @brief  Voice module MCU function settings
 * @param[in] {play} Play/pause function  1(play) / 0(pause)
 * @param[in] {bt_play} Bluetooth switch function  1(on) / 0(off)
 * @return Null
 * @note   The MCU needs to call this function by itself.
 */
void voice_mcu_fun_set(unsigned char play, unsigned char bt_play);
#endif

#endif
