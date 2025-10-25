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

#define MAX_BRIGHTNESS          32      // 最大輝度
#define LED_COLOR_ON_TIMER      100     // 1色の表示時間
#define RGBLED_NUM              1       // RGB LEDの数

static uint8_t s_red = 0;        // 赤
static uint8_t s_green = 0;      // 青
static uint8_t s_blue = 0;       // 緑
static uint8_t s_brightness = 0; // 明るさ
static bool s_onoff = false;     // 消灯=false, 点灯=ture
static bool s_autoled = false;   // 手動=false, 自動=ture

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
 * @brief 全カラー回転表示
 * 
 */
static void rgbled_fade(void)
{
    uint8_t cnt, red,green,blue;

    red = MAX_BRIGHTNESS;
    for(green = 0; green < MAX_BRIGHTNESS; green++)
    {
        for(cnt = 0; cnt < RGBLED_NUM; cnt++)
        {
            g_pixel.setPixelColor(cnt, g_pixel.Color(s_red, s_green, s_blue));
            g_pixel.show();
        }
        s_green++;
        s_red--;
        delay(LED_COLOR_ON_TIMER);
    }
    s_red = 0;

    for(blue = 0; blue < MAX_BRIGHTNESS; blue++)
    {
        for(cnt = 0; cnt < RGBLED_NUM; cnt++)
        {
            g_pixel.setPixelColor(cnt, g_pixel.Color(s_red, s_green, s_blue));
            g_pixel.show();
        }
        s_blue++;
        s_green--;
        delay(LED_COLOR_ON_TIMER);
    }
    s_green = 0;

    for(red = 0; red < MAX_BRIGHTNESS; red++)
    {
        for(cnt = 0; cnt < RGBLED_NUM; cnt++)
        {
            g_pixel.setPixelColor(cnt, g_pixel.Color(s_red, s_green, s_blue));
            g_pixel.show();
        }
        s_red++;
        s_blue--;
        delay(LED_COLOR_ON_TIMER);
    }
    s_blue = 0;
}

/**
 * @brief 指定のNeopixelの色を設定
 * 
 * @param led_no Neopixelの指定
 * @param red 赤色
 * @param blue 青色
 * @param green 緑
 */
void app_neopixel_set_color(uint8_t led_no, uint8_t red, uint8_t blue, uint8_t green)
{
    g_pixel.setPixelColor(led_no, g_pixel.Color(red, green, blue));
    g_pixel.show();
}

/**
 * @brief 指定のRGBにLEDを点灯 or 発光
 * 
 * @param red 赤
 * @param green 緑
 * @param blue 青
 * @param brightness 明るさ
 * @param onoff ON/OFF
 * @param autoled 自動で色を遷移し続けるか
 */
void app_neopixel_main(uint8_t red,uint8_t green, uint8_t blue, uint8_t brightness, bool onoff, bool autoled)
{
    s_red = red;
    s_blue = blue;
    s_green = green;
    s_brightness = brightness;
    s_onoff = onoff;
    s_autoled = autoled;

    if(s_autoled != true){
        if (s_onoff != false) {
            g_pixel.setPixelColor(0, g_pixel.Color(s_red, s_green, s_blue));
            g_pixel.show();
        }else{
            g_pixel.clear();
            g_pixel.show();
        }
    }else{
        rgbled_fade();
    }
}