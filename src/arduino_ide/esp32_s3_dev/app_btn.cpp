/**
 * @file app_btn.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ESP32 ボタン アプリ処理
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "app_btn.hpp"

ButtonState s_btn_staate = {false, 0, 0, 0, 0, false, NONE};

/**
 * @brief ボタン割り込みハンドラ
 * 
 */
void IRAM_ATTR buttonISR()
{
    __DI_ISR(&g_mux); // 割り込みマスク

    bool btn_state = false;
    unsigned long current_time_ms = 0;

    current_time_ms = millis();
    btn_state = digitalRead(BUTTON_PIN);

    if (btn_state != s_btn_staate.isPressed) {
        s_btn_staate.isPressed = btn_state;
        if (btn_state) {
            if (!s_btn_staate.processingClicks ||
                (current_time_ms - s_btn_staate.lastReleaseTime <= CLICK_TIMEOUT)) {
                s_btn_staate.clickCount = s_btn_staate.clickCount + 1;
                s_btn_staate.processingClicks = true;
            } else {
                s_btn_staate.clickCount = 1;
            }
            s_btn_staate.lastClickTime = current_time_ms;
            s_btn_staate.currentPressType = NORMAL_PRESS;
        } else {
            s_btn_staate.lastReleaseTime = current_time_ms;

            if (s_btn_staate.currentPressType == NORMAL_PRESS) {
                s_btn_staate.processingClicks = true;
            } else {
                s_btn_staate.clickCount = 0;
                s_btn_staate.processingClicks = false;
            }
            s_btn_staate.currentPressType = NONE;
        }
    }

    __EI_ISR(&g_mux); // 割り込み許可
}

/**
 * @brief ボタンポーリング処理
 * 
 */
void app_btn_polling(void)
{
    unsigned long current_time_ms = millis();
    bool isPressed = s_btn_staate.isPressed;
    unsigned long pressTime = current_time_ms - s_btn_staate.lastClickTime;
    PressType pressType = s_btn_staate.currentPressType;

    if (isPressed && pressType != VERY_LONG_PRESS) {
        if (pressTime >= VERY_LONG_PRESS_TIME && pressType != VERY_LONG_PRESS) {
            s_btn_staate.currentPressType = VERY_LONG_PRESS;
            DEBUG_PRINTF_RTOS("超長押し検出\n");
            return;
        } else if (pressTime >= LONG_PRESS_TIME && pressType == NORMAL_PRESS) {
            s_btn_staate.currentPressType = LONG_PRESS;
            DEBUG_PRINTF_RTOS("長押し検出\n");
            return;
        }
    }

    bool shouldProcessClicks = s_btn_staate.processingClicks &&
                                !isPressed &&
                                (current_time_ms - s_btn_staate.lastReleaseTime > RESET_TIMEOUT);

    if (shouldProcessClicks) {
        uint32_t clicks = s_btn_staate.clickCount;
        s_btn_staate.lastclickCount = s_btn_staate.clickCount;
        s_btn_staate.clickCount = 0;
        s_btn_staate.processingClicks = false;
        // s_btn_staate.currentPressType = MULTI_PRESS;

        switch (clicks) {
            case 1:
                DEBUG_PRINTF_RTOS("シングルクリック\n");
                break;
            case 2:
                DEBUG_PRINTF_RTOS("ダブルクリック\n");
                break;
            case 3:
                DEBUG_PRINTF_RTOS("トリプルクリック\n");
                break;
            default:
                if (clicks > 3)
                {
                    DEBUG_PRINTF_RTOS("%d回クリック\n", clicks);
                }
                break;
        }
    }
}

/**
 * @brief ボタン初期化
 * 
 */
void app_btn_init(void)
{
    // GPIO INPUT, プルアップ（アクティブLow）
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // 割込み(割込みピン、割込みハンドラ、エッジ)
    attachInterrupt(BUTTON_PIN, buttonISR, CHANGE);
}