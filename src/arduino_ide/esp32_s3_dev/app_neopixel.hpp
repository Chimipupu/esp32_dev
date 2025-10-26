/**
 * @file app_neopixel.hpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief NeoPixelアプリ
 * @version 0.1
 * @date 2025-10-25
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#ifndef APP_NEOPIXEL_HPP
#define APP_NEOPIXEL_HPP

#include "common.hpp"
#include <Adafruit_NeoPixel.h>

#define MAX_BRIGHTNESS          32      // 最大輝度
#define LED_COLOR_ON_TIMER      100     // 1色の表示時間
#define RGBLED_NUM              1       // RGB LEDの数

// ------------------------------------------------------------------------------
// [色テーブル関連]

#define LED_SCALE8(v)               ((uint8_t)((((uint32_t)(v) * (MAX_BRIGHTNESS)) + 127) / 255))
#define LED_RGB8_SCALED(r,g,b)      { LED_SCALE8(r), LED_SCALE8(g), LED_SCALE8(b) }

#define TBL_IDX_COLOR_RED      0x00    // 赤
#define TBL_IDX_COLOR_GREEN    0x01    // 緑
#define TBL_IDX_COLOR_BLUE     0x02    // 青
#define TBL_IDX_COLOR_WHITE    0x03    // 白
#define TBL_IDX_COLOR_YELLOW   0x04    // 黄
#define TBL_IDX_COLOR_MAGENTA  0x05    // マゼンタ
#define TBL_IDX_COLOR_CYAN     0x06    // シアン
#define TBL_IDX_COLOR_ORANGE   0x07    // オレンジ
#define TBL_IDX_COLOR_PURPLE   0x08    // パープル
#define TBL_IDX_COLOR_OFF      0x09    // オフ（黒）
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color_t;

extern const led_color_t g_led_color_tbl[];
// ------------------------------------------------------------------------------
void app_neopixel_init(void);
void app_neopixel_set_color(uint8_t led_no, const led_color_t *p_color);

#endif /* APP_NEOPIXEL_HPP */