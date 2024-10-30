/**
 * @file app_btn.cpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESP32 ボタン アプリ処理
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "app_btn.hpp"

ButtonState s_buttonState = {false, 0, 0, 0, false, NONE};
portMUX_TYPE s_mux = portMUX_INITIALIZER_UNLOCKED;

/**
 * @brief ボタン割り込みハンドラ
 * 
 */
void IRAM_ATTR buttonISR()
{
    __DI_ISR(&s_mux);
    bool currentState = digitalRead(BUTTON_PIN) == LOW;
    unsigned long currentTime = millis();

    if (currentState != s_buttonState.isPressed) {
        s_buttonState.isPressed = currentState;
        if (currentState) {
            if (!s_buttonState.processingClicks ||
                (currentTime - s_buttonState.lastReleaseTime <= CLICK_TIMEOUT)) {
                s_buttonState.clickCount++;
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
    __EI_ISR(&s_mux);
}

/**
 * @brief ボタンポーリング処理
 * 
 */
void app_btn_polling(ButtonState btnstate)
{
    bool result = false;
    bool isPressed = s_buttonState.isPressed;
    PressType pressType = s_buttonState.currentPressType;

    __DI_ISR(&s_mux);
    unsigned long currentTime = millis();
    unsigned long pressTime = currentTime - s_buttonState.lastClickTime;

    if (isPressed && pressType != VERY_LONG_PRESS) {
        if (pressTime >= VERY_LONG_PRESS_TIME && pressType != VERY_LONG_PRESS) {
            s_buttonState.currentPressType = VERY_LONG_PRESS;
        } else if (pressTime >= LONG_PRESS_TIME && pressType == NORMAL_PRESS) {
            s_buttonState.currentPressType = LONG_PRESS;
        }
    }

    bool shouldProcessClicks = s_buttonState.processingClicks &&
                                !isPressed &&
                                (currentTime - s_buttonState.lastReleaseTime > RESET_TIMEOUT);

    if (shouldProcessClicks) {
        s_buttonState.processingClicks = false;
        s_buttonState.currentPressType = MULTI_PRESS;
    }

    btnstate = s_buttonState;
    __EI_ISR(&s_mux);
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