/**********************************Copyright (c)**********************************
**                     All rights reserved (C), 2015-2020, Tuya
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    system.h
 * @author  Tuya Comprehensive Agreement Development Group
 * @version v2.5.5
 * @date    2020.6.1
 * @brief   UART data processing.The user does not need to care about what the file implements.
 */
  

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
#define         UPDATE_START_CMD                0x0a                            //Upgrade start
#define         UPDATE_TRANS_CMD                0x0b                            //Upgrade transfer
#define         GET_ONLINE_TIME_CMD             0x0c                            //Get system time (Greenwich Mean Time)
#define         FACTORY_MODE_CMD                0x0d                            //Enter production test mode    
#define         WIFI_TEST_CMD                   0x0e                            //Wifi function test
#define         GET_LOCAL_TIME_CMD              0x1c                            //Get local time
#define         WEATHER_OPEN_CMD                0x20                            //Turn on the weather          
#define         WEATHER_DATA_CMD                0x21                            //Weather data
#define         STATE_UPLOAD_SYN_CMD            0x22                            //Status upload (synchronization)
#define         STATE_UPLOAD_SYN_RECV_CMD       0x23                            //Status upload results(synchronization)
#define         HEAT_BEAT_STOP                  0x25                            //Turn off the WIFI module heartbeat
#define         STREAM_TRANS_CMD                0x28                            //Stream data transmission
#define         GET_WIFI_STATUS_CMD             0x2b                            //Gets the wifi networking status
#define         WIFI_CONNECT_TEST_CMD           0x2c                            //Wifi function test(connection designated route)
#define         GET_MAC_CMD                     0x2d                            //Get module mac
#define         GET_IR_STATUS_CMD               0x2e                            //IR status notification
#define         IR_TX_RX_TEST_CMD               0x2f                            //IR into send-receive test
#define         MAPS_STREAM_TRANS_CMD           0x30                            //streams trans(Support for multiple maps)
#define         FILE_DOWNLOAD_START_CMD         0x31                            //File download startup
#define         FILE_DOWNLOAD_TRANS_CMD         0x32                            //File download data transfer
#define         MODULE_EXTEND_FUN_CMD           0x34                            //Open the module time service notification
#define         BLE_TEST_CMD                    0x35                            //Bluetooth functional test（Scan designated bluetooth beacon）
#define         GET_VOICE_STATE_CMD             0x60                            //Gets the voice status code
#define         MIC_SILENCE_CMD                 0x61                            //MIC mute Settings
#define         SET_SPEAKER_VOLUME_CMD          0x62                            //speaker volume set
#define         VOICE_TEST_CMD                  0x63                            //Audio production test
#define         VOICE_AWAKEN_TEST_CMD           0x64                            //Wake up production test
#define         VOICE_EXTEND_FUN_CMD            0x65                            //Voice module extension function


//=============================================================================
#define MCU_RX_VER              0x00                                            //Module send frame protocol version number
#define MCU_TX_VER              0x03                                            //MCU send frame protocol version number(default)
#define PROTOCOL_HEAD           0x07                                            //Fixed protocol header length
#define FRAME_FIRST             0x55                                            //Frame header first byte
#define FRAME_SECOND            0xaa                                            //Frame header second byte
//============================================================================= 
SYSTEM_EXTERN volatile unsigned char wifi_data_process_buf[PROTOCOL_HEAD + WIFI_DATA_PROCESS_LMT];     //Serial data processing buffer
SYSTEM_EXTERN volatile unsigned char wifi_uart_rx_buf[PROTOCOL_HEAD + WIFI_UART_RECV_BUF_LMT];         //Serial receive buffer
SYSTEM_EXTERN volatile unsigned char wifi_uart_tx_buf[PROTOCOL_HEAD + WIFIR_UART_SEND_BUF_LMT];        //Serial port send buffer
//
SYSTEM_EXTERN volatile unsigned char *rx_buf_in;
SYSTEM_EXTERN volatile unsigned char *rx_buf_out;

SYSTEM_EXTERN volatile unsigned char stop_update_flag;                                                 //ENABLE:Stop all data uploads   DISABLE:Restore all data uploads

#ifndef WIFI_CONTROL_SELF_MODE
SYSTEM_EXTERN volatile unsigned char reset_wifi_flag;                                                  //Reset wifi flag (TRUE: successful / FALSE: failed)
SYSTEM_EXTERN volatile unsigned char set_wifimode_flag;                                                //Set the WIFI working mode flag (TRUE: Success / FALSE: Failed)
SYSTEM_EXTERN volatile unsigned char wifi_work_state;                                                  //Wifi module current working status
#endif

#ifdef WIFI_STREAM_ENABLE
SYSTEM_EXTERN volatile char stream_status;                                                             //Stream service delivery return status
SYSTEM_EXTERN volatile char maps_stream_status;                                                        //Many maps stream service delivery return status
#endif

/**
 * @brief  Write wifi uart bytes
 * @param[in] {dest} UART send buffer starts writing the address
 * @param[in] {byte} Write byte value
 * @return UART send buffer ends write address
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte);

/**
 * @brief  Write wifi uart buffer
 * @param[in] {dest} UART send buffer starts writing the address
 * @param[in] {src} source address
 * @param[in] {len} Data length
 * @return UART send buffer ends write address
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, const unsigned char *src, unsigned short len);

/**
 * @brief  Calculate checksum
 * @param[in] {pack} Data source pointer
 * @param[in] {pack_len} Need to calculate the length of the checksum data
 * @return checksum
 */
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/**
 * @brief  Send a frame of data to the wifi serial port
 * @param[in] {fr_type} Frame type
 * @param[in] {fr_ver} Frame version
 * @param[in] {len} Data length
 * @return Null
 */
void wifi_uart_write_frame(unsigned char fr_type, unsigned char fr_ver, unsigned short len);

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
unsigned char stream_trans(unsigned short id, unsigned int offset, unsigned char *buffer, unsigned short buf_len);

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
                                unsigned int offset, unsigned char *buffer, unsigned short buf_len);
#endif

/**
 * @brief  Data frame processing
 * @param[in] {offset} Data start position
 * @return Null
 */
void data_handle(unsigned short offset);

/**
 * @brief  Determines whether there is data in the uart receive buffer
 * @param  Null
 * @return Is there data
 */
unsigned char with_data_rxbuff(void);

/**
 * @brief  Read uart receive buffer 1 byte data
 * @param  Null
 * @return Read the data
 */
unsigned char take_byte_rxbuff(void);
#endif
  
  
