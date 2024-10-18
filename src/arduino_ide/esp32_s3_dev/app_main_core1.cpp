/**
 * @file app_main_core0.cpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief  Core1 アプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "common.hpp"
#include "app_main_core1.hpp"
#include "app_neopixel.hpp"

void core1WiFiTask(void * parameter)
{
    Serial.println("[Core1] ... core1WiFiTask");
    app_wifi_init();

    while (1)
    {
        bool wifi_flag = false;

        // Serial.println("[Core1] ... core1WiFiTask");
        wifi_flag = app_wifi_main();

        if(wifi_flag != true){
            // DeepSleep @DEEPSLEEP_TIME_US
            Serial.printf("DeepSleep : %d min", (DEEPSLEEP_TIME_US / 60) / 1000000);
            esp_deep_sleep_start();
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main_init_core1(void)
{
    Serial.begin(115200);
    Serial.println("UART初期化");

    // Deep Sleep
    esp_sleep_enable_timer_wakeup(DEEPSLEEP_TIME_US);

    // FreeRTOS
    xTaskCreatePinnedToCore(core1WiFiTask,     // コールバック関数ポインタ
                            "core1WiFiTask",   // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            3,                 // 優先度(0～7、7が最優先)
                            NULL,              // ハンドル
                            1);                // Core0 or Core1
}

void app_main_core1(void)
{
    // Serial.println("[Core1] ... loopTask");
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}