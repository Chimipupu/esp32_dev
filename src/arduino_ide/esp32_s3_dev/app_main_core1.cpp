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

#include "esp_system.h"

#include "common.hpp"
#include "app_main_core1.hpp"
#include "app_btn.hpp"
#include "app_neopixel.hpp"
#include "app_file_system.hpp"

// --------------------------------------------------
// [グローバル変数]

// RTC Slow Memの変数(リセットでも消えない領域)
RTC_DATA_ATTR uint32_t g_boot_cnt = 0;

// --------------------------------------------------
// [Static]
#ifdef __ESP_NOW_ENABLE__
#include "app_esp_now.hpp"
static xTaskHandle s_xTaskCore1EspNow;
static bool s_espnow_flag = true;
#endif /* __ESP_NOW_ENABLE__ */

static xTaskHandle s_xTaskCore1Btn;
static xTaskHandle s_xTaskCore1WiFi;
// static xTaskHandle s_xTaskCore1Main;
static bool s_wifi_flag = true;
static void rst_reson_check(void);
// --------------------------------------------------

static void rst_reson_check(void)
{
    esp_reset_reason_t reason = esp_reset_reason();

    switch (reason) {
        case ESP_RST_UNKNOWN:
            Serial.println("(DEBUG)Reset reason: UNKNOWN");
            break;

        case ESP_RST_POWERON:
            Serial.println("(DEBUG)Reset reason: POWER ON");
            break;

        case ESP_RST_EXT:
            Serial.println("(DEBUG)Reset reason: EXTERNAL RESET");
            break;

        case ESP_RST_SW:
            Serial.println("(DEBUG)Reset reason: SOFTWARE RESET");
            break;

        case ESP_RST_PANIC:
            Serial.println("(DEBUG)Reset reason: PANIC RESET");
            break;

        case ESP_RST_INT_WDT:
            Serial.println("(DEBUG)Reset reason: INTERRUPT WATCHDOG");
            break;

        case ESP_RST_TASK_WDT:
            Serial.println("(DEBUG)Reset reason: TASK WATCHDOG");
            break;

        case ESP_RST_WDT:
            Serial.println("(DEBUG)Reset reason: OTHER WATCHDOG");
            break;

        case ESP_RST_DEEPSLEEP:
            Serial.println("(DEBUG)Reset reason: DEEP SLEEP WAKEUP");
            break;

        case ESP_RST_BROWNOUT:
            Serial.println("(DEBUG)Reset reason: BROWNOUT RESET");
            break;

        case ESP_RST_SDIO:
            Serial.println("(DEBUG)Reset reason: SDIO RESET");
            break;

        default:
            Serial.println("(DEBUG)Reset reason: UNKNOWN VALUE");
            break;
    }
}

void vTaskCore1Btn(void *p_parameter)
{
    DEBUG_PRINTF_RTOS("[Core1] vTaskCore1Btn\n");

    while (1)
    {
        app_btn_polling();
        vTaskDelay(800 / portTICK_PERIOD_MS);
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
    app_neopixel_set_color(0, &g_led_color_tbl[TBL_IDX_COLOR_RED]);
    app_wifi_init();

    while (1)
    {
        s_wifi_flag = app_wifi_main();

        if(s_wifi_flag != true){
            DEBUG_PRINTF_RTOS("[Core1] vTaskCore1WiFi Suspend now!\n");
            vTaskSuspend(NULL);
            DEBUG_PRINTF_RTOS("[Core1] vTaskCore1WiFi Resume!\n");
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
#endif /* __WIFI_ENABLE__ */

#if 0
void vTaskCore1Main(void *p_parameter)
{
    // DEBUG_PRINTF_RTOS("[Core1] vTaskCore1Main\n");

    while (1)
    {
        // TODO
    }
}
#endif

void app_main_init_core1(void)
{
    xSerialMutex = xSemaphoreCreateMutex();

    // UART初期化
    Serial.begin(115200);
    while (!Serial) {
        WDT_TOGGLE;
    }
    DEBUG_PRINTF_RTOS("[DEBUG] Boot Cnt: %d\r\n", g_boot_cnt);

    // Deep Sleep
    rst_reson_check();
    esp_sleep_enable_timer_wakeup(DEEPSLEEP_TIME_US);

    // PSRAM初期化
    app_fs_psram_init();
    app_fs_psram_test();

    // NeoPixel初期化
    app_neopixel_init();

    // ボタン初期化
    app_btn_init();

    // FreeRTOS
#if 1
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
                            5,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1EspNow, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#endif /* __ESP_NOW_ENABLE__ */

#ifdef __WIFI_ENABLE__
    xTaskCreatePinnedToCore(vTaskCore1WiFi,    // コールバック関数ポインタ
                            "vTaskCore1WiFi",  // タスク名
                            16384,             // スタック
                            NULL,              // パラメータ
                            6,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1WiFi, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#endif /* __WIFI_ENABLE__ */

#if 0
    xTaskCreatePinnedToCore(vTaskCore1Main,   // コールバック関数ポインタ
                            "vTaskCore1Main", // タスク名
                            4096,              // スタック
                            NULL,              // パラメータ
                            3,                 // 優先度(0～7、7が最優先)
                            &s_xTaskCore1Main, // ハンドル
                            CPU_CORE_1);       // Core0 or Core1
#endif
}

void app_main_core1(void)
{
    // DEBUG_PRINTF_RTOS("[Core1] ... loopTask\n");
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
    vTaskSuspend(NULL);
}