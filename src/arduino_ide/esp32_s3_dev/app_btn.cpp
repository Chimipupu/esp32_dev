/**
 * @file app_btn.cpp
 * @author ちみ/Chimi(https://github.com/Chimipupu)
 * @brief ESP32 ボタン アプリ処理
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024 ちみ/Chimi(https://github.com/Chimipupu)
 * 
 */

#include "app_btn.hpp"

ButtonState s_buttonState = {false, 0, 0, 0, 0, false, NONE};

/**
 * @brief ボタン割り込みハンドラ
 * 
 */
void IRAM_ATTR buttonISR()
{
    __DI_ISR(&g_mux);
    bool currentState = digitalRead(BUTTON_PIN) == LOW;
    unsigned long currentTime = millis();

    if (currentState != s_buttonState.isPressed) {
        s_buttonState.isPressed = currentState;
        if (currentState) {
            if (!s_buttonState.processingClicks ||
                (currentTime - s_buttonState.lastReleaseTime <= CLICK_TIMEOUT)) {
                s_buttonState.clickCount = s_buttonState.clickCount + 1;
                s_buttonState.processingClicks = true;
            } else {
                s_buttonState.clickCount = 1;
            }
            s_buttonState.lastClickTime = currentTime;
            s_buttonState.currentPressType = NORMAL_PRESS;
        } else {
            s_buttonState.lastReleaseTime = currentTime;

            if (s_buttonState.currentPressType == NORMAL_PRESS) {
                s_buttonState.processingClicks = true;
            } else {
                s_buttonState.clickCount = 0;
                s_buttonState.processingClicks = false;
            }
            s_buttonState.currentPressType = NONE;
        }
    }
    __EI_ISR(&g_mux);
}

/**
 * @brief ボタンポーリング処理
 * 
 */
void app_btn_polling(ButtonState btnstate)
{
    __DI(&g_mux);

    unsigned long currentTime = millis();
    bool isPressed = s_buttonState.isPressed;
    unsigned long pressTime = currentTime - s_buttonState.lastClickTime;
    PressType pressType = s_buttonState.currentPressType;

    if (isPressed && pressType != VERY_LONG_PRESS) {
        if (pressTime >= VERY_LONG_PRESS_TIME && pressType != VERY_LONG_PRESS) {
            s_buttonState.currentPressType = VERY_LONG_PRESS;
            __EI(&g_mux);
            DEBUG_PRINTF_RTOS("超長押し検出\n");
            return;
        } else if (pressTime >= LONG_PRESS_TIME && pressType == NORMAL_PRESS) {
            s_buttonState.currentPressType = LONG_PRESS;
            __EI(&g_mux);
            DEBUG_PRINTF_RTOS("長押し検出\n");
            return;
        }
    }

    bool shouldProcessClicks = s_buttonState.processingClicks &&
                                !isPressed &&
                                (currentTime - s_buttonState.lastReleaseTime > RESET_TIMEOUT);

    if (shouldProcessClicks) {
        uint32_t clicks = s_buttonState.clickCount;
        s_buttonState.lastclickCount = s_buttonState.clickCount;
        s_buttonState.clickCount = 0;
        s_buttonState.processingClicks = false;
        btnstate = s_buttonState;
        // s_buttonState.currentPressType = MULTI_PRESS;
        __EI(&g_mux);

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
    }else{
        btnstate = s_buttonState;
        __EI(&g_mux);
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