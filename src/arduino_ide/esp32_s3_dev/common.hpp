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

#define __DI        vTaskEnterCritical
#define __EI        vTaskExitCritical
#define __DI_ISR    taskENTER_CRITICAL_FROM_ISR
#define __EI_ISR    taskEXIT_CRITICAL_FROM_ISR

extern SemaphoreHandle_t xSerialMutex;

#define CPU_CORE_0      0
#define CPU_CORE_1      1
#define APP_CORE        CPU_CORE_0
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

// GPIO
#define RGBLED_PIN      48   // NEOPIXEL @GPIO 48

#endif /* COMMON_HPP */