/**
 * @file app_main_core1.hpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  Core1 アプリ
 * @version 0.1
 * @date 2025-10-25
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#ifndef APP_MAIN_CORE1_HPP
#define APP_MAIN_CORE1_HPP

// WiFi
#include "app_wifi.hpp"

// FreeRTOS
#include <task.h>

void app_main_init_core1(void);
void app_main_core1(void);

#endif /* APP_MAIN_CORE1_HPP */