/**
 * @file app_neopixel.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief NeoPixelアプリ
 * @version 0.1
 * @date 2025-10-25
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#include "app_neopixel.hpp"

// LEDカラーテーブル
const led_color_t g_led_color_tbl[] = {
//   | R | G | B |
    {255,   0,   0}, // 赤
    {  0, 255,   0}, // 緑
    {  0,   0, 255}, // 青
    {255, 255, 255}, // 白
    {255, 255,   0}, // 黄
    {255,   0, 255}, // マゼンタ
    {  0, 255, 255}, // シアン
    {255, 165,   0}, // オレンジ
    {128,   0, 128}, // パープル
    {  0,   0,   0}  // オフ（黒）
};

Adafruit_NeoPixel g_pixel(RGBLED_NUM,
                        RGBLED_PIN,
                        NEO_GRB + NEO_KHZ800);

/**
 * @brief RGB LED 初期化
 * 
 */
void app_neopixel_init(void)
{
    g_pixel.begin();
    g_pixel.clear();
    g_pixel.show();
}

/**
 * @brief 指定のNeopixelの色を設定
 * 
 * @param led_no Neopixelの指定
 * @param p_color LED色構造体へのポインタ
 */
void app_neopixel_set_color(uint8_t led_no, const led_color_t *p_color)
{
    g_pixel.clear();
    // 最大輝度のMAX_BRIGHTNESSに色をスケーリングしてから光らす
    g_pixel.setPixelColor(led_no, g_pixel.Color(
        LED_SCALE8(p_color->red),
        LED_SCALE8(p_color->green),
        LED_SCALE8(p_color->blue)));
    g_pixel.show();
}