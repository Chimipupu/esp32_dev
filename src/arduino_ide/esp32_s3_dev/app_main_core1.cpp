/**
 * @file app_main_core1.cpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief  Core1 アプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "app_main_core1.hpp"

void app_main_init_core1(void)
{
    Serial.begin(115200);
    Serial.println("UART初期化");

    // WiFi
    app_wifi_init();
}

void app_main_core1(void)
{
    app_wifi_main();
}