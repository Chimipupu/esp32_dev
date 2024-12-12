/**
 * @file esp32_s3_dev_freertos.ino
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESP32評価プログラム Arduino IDEファイル
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "common.hpp"
#include "app_main_core0.hpp"
#include "app_main_core1.hpp"

SemaphoreHandle_t xSerialMutex;
portMUX_TYPE g_mux = portMUX_INITIALIZER_UNLOCKED;

// CPU Core0
void vTaskCore0Main(void *p_parameter)
{
    app_main_init_core0();

    while (1)
    {
        app_main_core0();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// CPU Core1
void core1_init(void)
{
    app_main_init_core1();

#if 0
    xTaskCreatePinnedToCore(vTaskCore0Main,    // コールバック関数ポインタ
                            "vTaskCore0Main",  // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            2,                 // 優先度(0～7、7が最優先)
                            NULL,              // ハンドル
                            CPU_CORE_0);       // Core0 or Core1
#endif
}

void core1_main(void)
{
    app_main_core1();
}