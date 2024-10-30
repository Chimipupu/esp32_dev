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

rgbled_state_t g_rgbled_state;

void core0RgbLedTask(void *p_parameter)
{
    Serial.println("[Core0] ... core0RgbLedTask");
    app_neopixel_init();

    // g_rgbled_state.red = 0;
    // g_rgbled_state.green = 0;
    // g_rgbled_state.blue = 0;
    // g_rgbled_state.brightness = 0;
    // g_rgbled_state.onoff = false;
    // g_rgbled_state.autoled = false;

    while (1)
    {
        // app_neopixel_main(&g_rgbled_state);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

void app_main_init_core0(void)
{
#if 0
    // FreeRTOS
    xTaskCreatePinnedToCore(core0RgbLedTask,   // コールバック関数ポインタ
                            "core0RgbLedTask", // タスク名
                            4096,               // スタック
                            NULL,              // パラメータ
                            0,                 // 優先度(0～7、7が最優先)
                            NULL,              // ハンドル
                            CPU_CORE_0);       // Core0 or Core1
#endif
}

void app_main_core0(void)
{
    // Serial.println("[Core0] ... loopTask");
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
    vTaskSuspend(NULL);
}