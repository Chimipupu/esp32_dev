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
#include "app_btn.hpp"
#include "app_neopixel.hpp"
#include "app_esp_now.hpp"

static xTaskHandle s_xCore1BtnTask;
static xTaskHandle s_xCore1WiFiTask;
static xTaskHandle s_xCore1EspNowTask;
static bool s_wifi_flag = false;
static bool s_wifi_task= true;

void core1BtnTask(void *p_parameter)
{
    ButtonState btnstate;
    DEBUG_PRINTF_RTOS("[Core1] core1BtnTask\n");

    while (1)
    {
        app_btn_polling(btnstate);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

#ifdef USE_ESPNOW_MODE
void core1EspNowTask(void *p_parameter)
{
    DEBUG_PRINTF_RTOS("[Core1] core1EspNowTask\n");
    app_espnow_init();

    while (1)
    {
        app_espnow_main();
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
#else
void core1WiFiTask(void *p_parameter)
{
    DEBUG_PRINTF_RTOS("[Core1] core1WiFiTask\n");
    app_wifi_init();

    while (1)
    {
        s_wifi_flag = app_wifi_main();

        if(s_wifi_flag != true){
#ifdef DEEP_SLEEP_ENABLE
            // DeepSleep @DEEPSLEEP_TIME_US
            DEBUG_PRINTF_RTOS("DeepSleep : %d min\n", (DEEPSLEEP_TIME_US / 60) / 1000000);
            esp_deep_sleep_start();
#else
            DEBUG_PRINTF_RTOS("[Core1] core1WiFiTask Suspend now!\n");
            s_wifi_task = false;
            vTaskSuspend(NULL);
#endif
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
#endif /* USE_ESPNOW_MODE */

void app_main_init_core1(void)
{
    Serial.begin(115200);

    // ボタン
    app_btn_init();

    // RGB LED (NeoPixel)
    app_neopixel_init();

#ifdef USE_ESPNOW_MODE
#ifdef ESP_NOW_TX
    app_neopixel_main(16, 16, 16, 0, true, false); // 白 = ESPNOW 送信側
#else
    app_neopixel_main(16, 6, 0, 0, true, false); // オレンジ = ESPNOW 受信側
#endif /* ESP_NOW_TX */
#else
    app_neopixel_main(16, 0, 0, 0,true, false); // red, on
#endif /* USE_ESPNOW_MODE */

    // Deep Sleep
    esp_sleep_enable_timer_wakeup(DEEPSLEEP_TIME_US);

    // FreeRTOS
    // xSerialMutex = xSemaphoreCreateBinary();
    xSerialMutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(core1BtnTask,     // コールバック関数ポインタ
                            "core1BtnTask",   // タスク名
                            4096,              // スタック
                            NULL,              // パラメータ
                            2,                 // 優先度(0～7、7が最優先)
                            &s_xCore1BtnTask,  // ハンドル
                            CPU_CORE_1);       // Core0 or Core1

#ifdef USE_ESPNOW_MODE
    xTaskCreatePinnedToCore(core1EspNowTask,   // コールバック関数ポインタ
                            "core1EspNowTask", // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            7,                 // 優先度(0～7、7が最優先)
                            &s_xCore1EspNowTask, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#else
    xTaskCreatePinnedToCore(core1WiFiTask,     // コールバック関数ポインタ
                            "core1WiFiTask",   // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            7,                 // 優先度(0～7、7が最優先)
                            &s_xCore1WiFiTask, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#endif
}

void app_main_core1(void)
{
    // DEBUG_PRINTF_RTOS("[Core1] ... loopTask\n");
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
    vTaskSuspend(NULL);
}