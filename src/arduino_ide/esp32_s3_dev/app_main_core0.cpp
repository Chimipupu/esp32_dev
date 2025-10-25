/**
 * @file app_main_core1.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  Core1 アプリ
 * @version 0.1
 * @date 2025-10-25
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "common.hpp"
#include "app_main_core0.hpp"

void core0RgbLedTask(void *p_parameter)
{
    DEBUG_PRINTF_RTOS("[Core0] ... core0RgbLedTask\n");
    app_neopixel_init();
    app_neopixel_set_color(0x00, 0xFF, 0x00, 0x00); // 赤色

    while (1)
    {
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

void app_main_init_core0(void)
{
#if 1
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
    // DEBUG_PRINTF_RTOS("[Core0] ... loopTask\n");
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
    vTaskSuspend(NULL);
}