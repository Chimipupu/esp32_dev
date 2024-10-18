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

#include "common.hpp"
#include "app_main_core0.hpp"

void core0RgbLedTask(void * parameter)
{
    Serial.println("[Core0] ... core0RgbLedTask");
    app_neopixel_init();

    while (1)
    {
        app_neopixel_main();
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

void app_main_init_core0(void)
{
    // FreeRTOS
    xTaskCreatePinnedToCore(core0RgbLedTask,   // コールバック関数ポインタ
                            "core0RgbLedTask", // タスク名
                            4096,               // スタック
                            NULL,              // パラメータ
                            0,                 // 優先度(0～7、7が最優先)
                            NULL,              // ハンドル
                            0);                // Core0 or Core1
}

void app_main_core0(void)
{
    // Serial.println("[Core0] ... loopTask");
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}