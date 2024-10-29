/**
 * @file common.hpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESP32 共通ヘッダー
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef COMMON_HPP
#define COMMON_HPP

// C,C++ Lib
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// FreeRTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// #define FILESYSTEM_RESET
// #define YD_ESP32_S3

#ifndef ATOM_S3_LITE
#include <M5AtomS3.h>
#define ATOM_S3_LITE
#endif

// GPIO
#ifdef YD_ESP32_S3
#define RGBLED_PIN        48   // YD-ESP32-S3 RGBLED @GPIO 48
#define BUTTON_PIN        46   // YD-ESP32-S3e Button @GPIO 41
#else
#define RGBLED_PIN        35   // Atom S3 Lite RGBLED @GPIO 35
#define BUTTON_PIN        41   // Atom S3 Lite Button @GPIO 41
#endif

// 割込みマスク・許可
#define __DI            vTaskEnterCritical
#define __EI            vTaskExitCritical
#define __DI_ISR_TASK   taskENTER_CRITICAL_FROM_ISR
#define __EI_ISR_TASK   taskEXIT_CRITICAL_FROM_ISR
#define __DI_ISR        portENTER_CRITICAL_ISR
#define __EI_ISR        portEXIT_CRITICAL_ISR

extern SemaphoreHandle_t xSerialMutex;

#define CPU_CORE_0      0
#define CPU_CORE_1      1

// #define ATOM_S3
#define ATOM_S3_LITE
// #define YD_ESP_S3

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

#endif /* COMMON_HPP */