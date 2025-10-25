/**
 * @file app_btn.hpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ESP32 ボタン アプリ処理
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef APP_BTN_HPP
#define APP_BTN_HPP

#include "Arduino.h"
#include "common.hpp"

#define CLICK_TIMEOUT            300  // クリック間の最大許容時間 (ms)
#define RESET_TIMEOUT            800  // 最後のクリックから結果確定までの待ち時間 (ms)
#define LONG_PRESS_TIME          1000 // 長押し判定時間 (ms)
#define VERY_LONG_PRESS_TIME     3000 // 超長押し判定時間 (ms)

// ボタンの状態種別
enum PressType
{
    NONE,
    NORMAL_PRESS,   // 単押し
    MULTI_PRESS,    // 複数押し
    LONG_PRESS,     // 長押し押し
    VERY_LONG_PRESS // 超長押し
};

// ボタンの状態を管理する構造体
struct ButtonState
{
    volatile bool isPressed;                // 現在のボタン状態
    volatile uint32_t clickCount;           // クリック回数
    volatile uint32_t lastclickCount;       // 最後のクリック回数
    volatile uint32_t lastClickTime;        // 最後のクリック時刻
    volatile uint32_t lastReleaseTime;      // 最後のリリース時刻
    volatile bool processingClicks;         // クリック処理中フラグ
    volatile PressType currentPressType;    // ボタンの状態種別
};

extern portMUX_TYPE g_mux;

void app_btn_init(void);
void app_btn_polling(void);

#endif /* APP_BTN_HPP */