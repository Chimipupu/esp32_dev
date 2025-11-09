/**
 * @file common.hpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ESP32 共通ヘッダー
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef COMMON_HPP
#define COMMON_HPP

// --------------------------------------------------
// [C/C++ Lib]
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// --------------------------------------------------
// [Arduino IDE Lib]
#include <Arduino.h>

// --------------------------------------------------
// [FreeRTOS Lib]
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// --------------------------------------------------
// DeepSleep
#include "esp_sleep.h"
#define DEEPSLEEP_TIME_US_30SEC     (   30ULL * 1000000ULL)   // DeepSleep = 30秒
#define DEEPSLEEP_TIME_US_1MIN      (   60ULL * 1000000ULL)   // DeepSleep = 1分
#define DEEPSLEEP_TIME_US_2MIN      (  120ULL * 1000000ULL)   // DeepSleep = 2分
#define DEEPSLEEP_TIME_US_3MIN      (  180ULL * 1000000ULL)   // DeepSleep = 3分
#define DEEPSLEEP_TIME_US_5MIN      (  300ULL * 1000000ULL)   // DeepSleep = 5分
#define DEEPSLEEP_TIME_US_10MIN     (  600ULL * 1000000ULL)   // DeepSleep = 10分
#define DEEPSLEEP_TIME_US_15MIN     (  900ULL * 1000000ULL)   // DeepSleep = 15分
#define DEEPSLEEP_TIME_US_20MIN     ( 1200ULL * 1000000ULL)   // DeepSleep = 20分
#define DEEPSLEEP_TIME_US_30MIN     ( 1800ULL * 1000000ULL)   // DeepSleep = 30分
#define DEEPSLEEP_TIME_US_1HOUR     ( 3600ULL * 1000000ULL)   // DeepSleep = 1時間
#define DEEPSLEEP_TIME_US_2HOUR     ( 7200ULL * 1000000ULL)   // DeepSleep = 2時間
#define DEEPSLEEP_TIME_US_3HOUR     (10800ULL * 1000000ULL)   // DeepSleep = 3時間
#define DEEPSLEEP_TIME_US_5HOUR     (18000ULL * 1000000ULL)   // DeepSleep = 5時間
#define DEEPSLEEP_TIME_US_8HOUR     (28800ULL * 1000000ULL)   // DeepSleep = 8時間
#define DEEPSLEEP_TIME_US_12HOUR    (43200ULL * 1000000ULL)   // DeepSleep = 12時間
#define DEEPSLEEP_TIME_US_24HOUR    (86400ULL * 1000000ULL)   // DeepSleep = 24時間

#define DEEPSLEEP_TIME_US           DEEPSLEEP_TIME_US_1MIN    // DeepSleep = 1分間

// --------------------------------------------------

#define __WIFI_ENABLE__
// #define __FTP_ENABLE__
// #define __ESP_NOW_ENABLE__

#if defined(__WIFI_ENABLE__) && defined(__ESP_NOW_ENABLE__)
    #define ESP_NOW_TX
    // #define ESP_NOW_RX
#endif /* __ESP_NOW_ENABLE__ */

// #define FILESYSTEM_RESET
// #define DEEP_SLEEP_ENABLE

// GPIO
#define RGBLED_PIN        48   // YD-ESP32-S3 RGBLED @GPIO 48
#define BUTTON_PIN        46   // YD-ESP32-S3e Button @GPIO 41

// 割込みマスク・許可
#define __DI            portENTER_CRITICAL
#define __EI            portEXIT_CRITICAL
#define __DI_ISR        portENTER_CRITICAL_ISR
#define __EI_ISR        portEXIT_CRITICAL_ISR

extern portMUX_TYPE g_mux;
extern SemaphoreHandle_t xSerialMutex;

#define CPU_CORE_0      0
#define CPU_CORE_1      1

#ifndef APP_CORE
// #define APP_CORE        CPU_CORE_0
#define APP_CORE        CPU_CORE_1

#if APPLE_CORE == CPU_CORE_0
#define core0_init    setup
#define core0_main    loop
#else
#define core1_init    setup
#define core1_main    loop
#endif

#define CPU_CORE_0      0
#define CPU_CORE_1      1

// #define APP_CORE        CPU_CORE_0
#define APP_CORE        CPU_CORE_1

#if APP_CORE == CPU_CORE_0
    #define core0_init    setup
    #define core0_main    loop
#else
    #define core1_init    setup
    #define core1_main    loop
#endif
#endif

#ifndef NOP
static inline void NOP(void)
{
    asm volatile("nop");
}
#endif

static inline void WDT_TOGGLE(void)
{
#ifdef __WDT_ENABLE__
    watchdog_update();
#else
    asm volatile("nop");
#endif /* __WDT_ENABLE__ */
}

#define DEBUG_PRINTF_RTOS   safeSerialPrintf
#ifdef DEBUG_PRINTF_RTOS
static inline void safeSerialPrintf(const char *p_format, ...)
{
    if (xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdTRUE) {
        va_list args;
        va_start(args, p_format);

        for(const char *p = p_format; *p != '\0'; p++)
        {
            if (*p == '%') {
                p++;  // '%'の次の文字を見る
                int width = 0;
                int precision = -1;

                // 幅の取得
                while(*p >= '0' && *p <= '9')
                {
                    width = width * 10 + (*p - '0');
                    p++;
                }

                // 精度の取得
                if (*p == '.') {
                    p++;
                    precision = 0;
                    while (*p >= '0' && *p <= '9') {
                        precision = precision * 10 + (*p - '0');
                        p++;
                    }
                }

                // フォーマット指定子に基づいて処理
                switch(*p)
                {
                    case 'd':  // 整数
                        Serial.print(va_arg(args, int));
                        break;
                    case 'f':  // 浮動小数点数（float）
                    case 'l':  // 'l' は無視
                    case 'F':  // 浮動小数点数（float、大文字）
                        Serial.print(va_arg(args, double), (precision >= 0) ? precision : 2);
                        break;
                    case 's':  // 文字列
                        Serial.print(va_arg(args, char*));
                        break;
                    case 'c':  // 文字
                        Serial.print((char)va_arg(args, int));
                        break;
                    case 'x':  // 16進数（小文字）
                    case 'X':  // 16進数（大文字）
                        Serial.print(va_arg(args, int), HEX);
                        break;
                    default:  // 他のフォーマット指定子はそのまま表示
                        Serial.print(*p);
                        break;
                }
            } else if (*p == '\r' || *p == '\n') {
                // 改行シーケンスの処理
                Serial.println(); // 改行
            } else {
                // '%'でない文字はそのまま表示
                Serial.print(*p);
            }
        }
        va_end(args);
        xSemaphoreGive(xSerialMutex);
    } else {
        Serial.println("Failed to acquire mutex!");
    }
}
#endif /* DEBUG_PRINTF_RTOS */

#endif /* COMMON_HPP */