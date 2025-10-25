/**
 * @file app_main_core0.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  Core1 アプリ
 * @version 0.1
 * @date 2025-10-25
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "common.hpp"
#include "app_main_core1.hpp"
#include "app_btn.hpp"
#include "app_neopixel.hpp"
#include "app_file_system.hpp"

#ifdef __ESP_NOW_ENABLE__
#include "app_esp_now.hpp"
static xTaskHandle s_xTaskCore1EspNow;
static bool s_espnow_flag = true;
#endif /* __ESP_NOW_ENABLE__ */

static xTaskHandle s_xTaskCore1Btn;
static xTaskHandle s_xTaskCore1WiFi;
static xTaskHandle s_xTaskCore1Main;
static bool s_wifi_flag = true;

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

#ifdef __ESP_NOW_ENABLE__
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
            DEBUG_PRINTF_RTOS("[Core1] vTaskCore1EspNow Resume!\n");
        } else {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
#else
        vTaskDelay(100 / portTICK_PERIOD_MS);
#endif /* ESP_NOW_TX */
    }
}
#endif /* __ESP_NOW_ENABLE__ */

#ifdef __WIFI_ENABLE__
void vTaskCore1WiFi(void *p_parameter)
{
#ifdef __ESP_NOW_ENABLE__
    if (s_espnow_flag != false) {
        vTaskSuspend(NULL);
    }
#endif /* __ESP_NOW_ENABLE__ */

    DEBUG_PRINTF_RTOS("[Core1] vTaskCore1WiFi\n");
    app_neopixel_main(16, 0, 0, 0, true, false); // red
    app_wifi_init();

    while (1)
    {
        s_wifi_flag = app_wifi_main();

        if(s_wifi_flag != true){
            DEBUG_PRINTF_RTOS("[Core1] vTaskCore1WiFi Suspend now!\n");
            vTaskSuspend(NULL);
            DEBUG_PRINTF_RTOS("[Core1] vTaskCore1WiFi Resume!\n");
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
#endif /* __WIFI_ENABLE__ */

void vTaskCore1Main(void *p_parameter)
{
    // DEBUG_PRINTF_RTOS("[Core1] vTaskCore1Main\n");

    while (1)
    {
#if 0
        if ((s_wifi_flag != true) && (s_espnow_flag != true))
        {
            // DeepSleep @DEEPSLEEP_TIME_US
            uint32_t dat = (DEEPSLEEP_TIME_US / 60) / 1000000;
            DEBUG_PRINTF_RTOS("[Core1] vTaskCore1Main ... No Proc. DeepSleep Now!\n");
            DEBUG_PRINTF_RTOS("DeepSleep : %d min\n", dat);
            app_neopixel_main(16, 0, 16, 0, true, false); // 紫
            esp_deep_sleep_start();
        } else {
            // TODO: Core1メインタスクの処理実装
            NOP;
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
#else
        DEBUG_PRINTF_RTOS("[Core1] vTaskCore1Main\n");
    #ifdef YD_ESP32_S3
        // NOP
    #endif /* YD_ESP32_S3 */
        vTaskDelay(1000 / portTICK_PERIOD_MS);
#endif
    }
}

void app_main_init_core1(void)
{
    xSerialMutex = xSemaphoreCreateMutex();

    // UART初期化
    Serial.begin(115200);
    while (!Serial) {
        WDT_TOGGLE;
    }

#ifdef YD_ESP32_S3
    // PSRAM初期化
    app_fs_psram_init();
    app_fs_psram_test();
#endif /* YD_ESP32_S3 */

    // NeoPixel初期化
    app_neopixel_init();

    // ボタン初期化
    app_btn_init();

    // Deep Sleep
    // esp_sleep_enable_timer_wakeup(DEEPSLEEP_TIME_US);

#if 0
    // FreeRTOS
    xTaskCreatePinnedToCore(vTaskCore1Btn,     // コールバック関数ポインタ
                            "vTaskCore1Btn",   // タスク名
                            4096,              // スタック
                            NULL,              // パラメータ
                            1,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1Btn,  // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#endif

#ifdef __ESP_NOW_ENABLE__
    xTaskCreatePinnedToCore(vTaskCore1EspNow,   // コールバック関数ポインタ
                            "vTaskCore1EspNow", // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            4,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1EspNow, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#endif /* __ESP_NOW_ENABLE__ */

#ifdef __WIFI_ENABLE__
    xTaskCreatePinnedToCore(vTaskCore1WiFi,     // コールバック関数ポインタ
                            "vTaskCore1WiFi",   // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            3,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1WiFi, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#endif /* __WIFI_ENABLE__ */

    xTaskCreatePinnedToCore(vTaskCore1Main,   // コールバック関数ポインタ
                            "vTaskCore1Main", // タスク名
                            16384,              // スタック
                            NULL,              // パラメータ
                            6,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1Main, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
}

void app_main_core1(void)
{
    // DEBUG_PRINTF_RTOS("[Core1] ... loopTask\n");
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
    vTaskSuspend(NULL);
}