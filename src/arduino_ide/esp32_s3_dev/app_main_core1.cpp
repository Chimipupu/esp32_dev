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

    // Deep Sleep
    esp_sleep_enable_timer_wakeup(DEEPSLEEP_TIME_US);

    // WiFi
    app_wifi_init();
}

void app_main_core1(void)
{
    bool wifi_flag = false;

    wifi_flag = app_wifi_main();

#if 0
    if(wifi_flag != true){
        // DeepSleep @DEEPSLEEP_TIME_US
        Serial.printf("DeepSleep : %d min", (DEEPSLEEP_TIME_US / 60) / 1000000);
        esp_deep_sleep_start();
    }
#endif
}