/**
 * @file app_wifi.hpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESP32 WiFiアプリ
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef APP_WIFI_HPP
#define APP_WIFI_HPP

// Lib, Drv
#include <WiFi.h>
#include "FS.h"
#include "FFat.h"
#include <WebServer.h>
#include <time.h>
#include <ArduinoJson.h>
#include "soc/rtc.h"

// APP
// #include "app_wifi_inital_html.hpp"

struct WiFiConfig
{
    String ssid;
    String password;
};

void app_wifi_init(void);
void app_wifi_main(void);

#endif /* APP_WIFI_HPP */