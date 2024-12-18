/**
 * @file app_main_core0.hpp
 * @author ちみ/Chimi(https://github.com/Chimipupu)
 * @brief  Core0 アプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024 ちみ/Chimi(https://github.com/Chimipupu)
 * 
 */
#ifndef APP_MAIN_CORE0_HPP
#define APP_MAIN_CORE0_HPP

// FreeRTOS
#include <task.h>

// RGBLED
#include "app_neopixel.hpp"

void app_main_init_core0(void);
void app_main_core0(void);

#endif /* APP_MAIN_CORE0_HPP */