/**
 * @file esp32_c6_dev.ino
 * @author ちみ/Chimi(https://github.com/Chimipupu)
 * @brief ESP32 C6 評価プログラム Arduino IDEファイル
 * @version 0.1
 * @date 2024-12-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "common.hpp"
#include "app_main.hpp"

void setup(void)
{
    app_main_init();
}

void loop()
{
    app_main();
}