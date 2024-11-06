/**
 * @file app_esp_now.hpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESPNOW アプリヘッダー
 * @version 0.1
 * @date 2024-11-6
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef APP_ESP_NOW_HPP
#define APP_ESP_NOW_HPP

#include <stdint.h>
#include <string.h>
#include <esp_system.h>
#include "esp_mac.h"
#include <esp_wifi.h>
#include <esp_now.h>
#include <WiFi.h>

#include "common.hpp"

#define ESP_NOW_TX
// #define ESP_NOW_RX

typedef enum {
    TX_NONE = 0x00,
    ANY_ONE = 0x10,
    REQ_PEER = 0x20,
    RES_PEER = 0x30,
    TX_DATA = 0x40,
} e_esp_now_tx_type;

extern portMUX_TYPE g_mux;

void app_espnow_init(void);
bool app_espnow_main(void);

#endif /* APP_ESP_NOW_HPP */