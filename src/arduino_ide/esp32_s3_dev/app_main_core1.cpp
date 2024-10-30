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

static xTaskHandle s_xCore1BtnTask;
static xTaskHandle s_xCore1WiFiTask;
static bool s_wifi_flag = false;
static bool s_wifi_task= true;

void core1BtnTask(void *p_parameter)
{
    ButtonState btnstate;
    Serial.println("[Core1] core1BtnTask");

    while (1)
    {
        app_btn_polling(btnstate);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void core1WiFiTask(void *p_parameter)
{
    Serial.println("[Core1] core1WiFiTask");
    app_wifi_init();

    while (1)
    {
        s_wifi_flag = app_wifi_main();

        if(s_wifi_flag != true){
#ifdef DEEP_SLEEP_ENABLE
            // DeepSleep @DEEPSLEEP_TIME_US
            Serial.printf("DeepSleep : %d min", (DEEPSLEEP_TIME_US / 60) / 1000000);
            esp_deep_sleep_start();
#else
            Serial.println("[Core1] core1WiFiTask Suspend now!");
            s_wifi_task = false;
            vTaskSuspend(NULL);
#endif
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main_init_core1(void)
{
    Serial.begin(115200);
    Serial.println("UART初期化");

    // ボタン
    app_btn_init();

    // RGB LED (NeoPixel)
    app_neopixel_init();
    app_neopixel_main(16, 0, 0, 0,true, false); // red, on

    // Deep Sleep
    esp_sleep_enable_timer_wakeup(DEEPSLEEP_TIME_US);

    // FreeRTOS
    xTaskCreatePinnedToCore(core1BtnTask,     // コールバック関数ポインタ
                            "core1BtnTask",   // タスク名
                            4096,              // スタック
                            NULL,              // パラメータ
                            2,                 // 優先度(0～7、7が最優先)
                            &s_xCore1BtnTask,  // ハンドル
                            CPU_CORE_1);       // Core0 or Core1

    xTaskCreatePinnedToCore(core1WiFiTask,     // コールバック関数ポインタ
                            "core1WiFiTask",   // タスク名
                            8192,              // スタック
                            NULL,              // パラメータ
                            7,                 // 優先度(0～7、7が最優先)
                            &s_xCore1WiFiTask, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
}

void app_main_core1(void)
{
    // Serial.println("[Core1] ... loopTask");
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
    vTaskSuspend(NULL);
}