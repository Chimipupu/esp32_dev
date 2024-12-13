/**
 * @file app_ftp.hpp
 * @author ちみ/Chimi(https://github.com/Chimipupu)
 * @brief ESP32 FTP アプリ処理
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2024 ちみ/Chimi(https://github.com/Chimipupu)
 * 
 */

#ifndef APP_FTP_HPP
#define APP_FTP_HPP

#ifdef __WIFI_ENABLE__
// Lib, Drv
#include "Arduino.h"
#include <SimpleFTPServer.h>

void app_ftp_init(void);
void app_ftp_main(void);
#endif /* __WIFI_ENABLE__ */
#endif /* APP_FTP_HPP */