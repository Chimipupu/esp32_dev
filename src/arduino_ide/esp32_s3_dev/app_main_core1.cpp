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

static xTaskHandle s_xTaskCore1Btn;
static xTaskHandle s_xTaskCore1WiFi;
static xTaskHandle s_xTaskCore1EspNow;
static bool s_wifi_flag = false;
static bool s_espnow_flag = true;
static bool s_wifi_task = false;

void vTaskCore1Btn(void *p_parameter)
{
    ButtonState btnstate;
    DEBUG_PRINTF_RTOS("[Core1] vTaskCore1Btn\n");

    while (1)
    {
        app_btn_polling(btnstate);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void vTaskCore1EspNow(void *p_parameter)
{
    DEBUG_PRINTF_RTOS("[Core1] vTaskCore1EspNow\n");
    app_espnow_init();

    while (1)
    {
        s_espnow_flag = app_espnow_main();
#ifdef ESP_NOW_TX
        if (s_espnow_flag != true) {
            DEBUG_PRINTF_RTOS("[Core1] vTaskCore1EspNow Suspend now!\n");
            vTaskResume(s_xTaskCore1WiFi);
            vTaskSuspend(NULL);
        } else {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
#else
        vTaskDelay(100 / portTICK_PERIOD_MS);
#endif /* ESP_NOW_TX */
    }
}

void vTaskCore1WiFi(void *p_parameter)
{
    if (s_espnow_flag != false) {
        vTaskSuspend(NULL);
    }

    DEBUG_PRINTF_RTOS("[Core1] vTaskCore1WiFi\n");
    app_neopixel_main(16, 0, 0, 0,true, false); // red
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
            DEBUG_PRINTF_RTOS("[Core1] vTaskCore1WiFi Suspend now!\n");
            vTaskSuspend(NULL);
#endif
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main_init_core1(void)
{
    Serial.begin(115200);

    // ボタン
    app_btn_init();

    // RGB LED (NeoPixel)
    app_neopixel_init();

    // Deep Sleep
    esp_sleep_enable_timer_wakeup(DEEPSLEEP_TIME_US);

    // FreeRTOS
    // xSerialMutex = xSemaphoreCreateBinary();
    xSerialMutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(vTaskCore1Btn,     // コールバック関数ポインタ
                            "vTaskCore1Btn",   // タスク名
                            4096,              // スタック
                            NULL,              // パラメータ
                            2,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1Btn,  // ハンドル
                            CPU_CORE_1);       // Core0 or Core1

    xTaskCreatePinnedToCore(vTaskCore1EspNow,   // コールバック関数ポインタ
                            "vTaskCore1EspNow", // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            7,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1EspNow, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1

#ifdef ESP_NOW_TX
    xTaskCreatePinnedToCore(vTaskCore1WiFi,     // コールバック関数ポインタ
                            "vTaskCore1WiFi",   // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            5,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1WiFi, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#endif /* ESP_NOW_TX */
}

void app_main_core1(void)
{
    // DEBUG_PRINTF_RTOS("[Core1] ... loopTask\n");
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
    vTaskSuspend(NULL);
}