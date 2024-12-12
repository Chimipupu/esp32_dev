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

#ifdef __WIFI_ENABLE__
// Lib, Drv
#include <WiFi.h>
#include <WebServer.h>
#include <time.h>
#include <ArduinoJson.h>
#include "soc/rtc.h"

// APP
// #include "app_wifi_inital_html.hpp"

#define RTC_TIME    false
#define NTP_TIME    true

struct WiFiConfig
{
    String ssid;
    String password;
};

extern portMUX_TYPE g_mux;

void app_wifi_scan(void);
void app_wifi_init(void);
bool app_wifi_main(void);
#endif /* __WIFI_ENABLE__ */

#endif /* APP_WIFI_HPP */