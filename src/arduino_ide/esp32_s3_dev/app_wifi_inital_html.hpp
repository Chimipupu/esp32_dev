/**
 * @file app_wifi_inital_html.hpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ESP32 初期AP HTML
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef APP_WIFI_INITAL_HTML_HPP
#define APP_WIFI_INITAL_HTML_HPP

#include "common.hpp"

#ifdef __WIFI_ENABLE__
#if 1
const char *p_settingsHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>WiFi設定</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0e6ff;
            margin: 0;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
        }
        .container {
            background-color: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            max-width: 400px;
            width: 100%;
        }
        h1 {
            color: #6a1b9a;
            text-align: center;
        }
        input {
            width: 100%;
            padding: 8px;
            margin: 10px 0;
            border: 1px solid #ddd;
            border-radius: 4px;
            box-sizing: border-box;
        }
        button {
            background-color: #6a1b9a;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            width: 100%;
        }
        button:hover {
            background-color: #4a148c;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>WiFi設定</h1>
        <form action="/save" method="POST">
            <input type="text" name="ssid" placeholder="WiFiのSSID" required><br>
            <input type="password" name="password" placeholder="WiFiのパスワード" required><br>
            <button type="submit">保存</button>
        </form>
    </div>
</body>
</html>
)";
#endif

const char *p_indexHTML = R"(
<!DOCTYPE html>
<html lang="ja">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32評価プログラム by ちみ</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            background-color: #f5e6ff;
            color: #333;
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        h1 {
            color: #8a2be2;
            font-size: 2.5rem;
            margin-bottom: 30px;
        }

        .container {
            background-color: #fff;
            border-radius: 15px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            padding: 20px;
            margin-bottom: 30px;
            width: 90%;
            max-width: 800px;
        }

        .row {
            display: flex;
            justify-content: space-around;
            margin-bottom: 20px;
        }

        .item {
            text-align: center;
        }

        label {
            font-size: 1.5rem;
            color: #6a0dad;
        }

        .data {
            font-size: 1.3rem;
            margin-top: 10px;
            color: #333;
            background-color: #f0e8ff;
            border-radius: 8px;
            padding: 10px;
            min-width: 120px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        .form-container {
            margin-top: 20px;
            text-align: center;
        }

        .form-container input {
            font-size: 1.2rem;
            padding: 8px;
            margin-right: 10px;
            border-radius: 5px;
            border: 1px solid #ccc;
            width: 150px;
            /* 幅を固定 */
        }

        .form-container button {
            font-size: 1.2rem;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            background-color: #8a2be2;
            color: white;
            cursor: pointer;
        }

        .time-container {
            margin-top: 30px;
            font-size: 1.5rem;
            color: #8a2be2;
            background-color: #f0e8ff;
            border-radius: 8px;
            padding: 20px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            display: flex;
            justify-content: space-between;
            /* 横並び */
        }

        .time-input {
            display: flex;
            justify-content: space-around;
            width: 100%;
            /* 幅を100%に設定 */
            margin: 10px 0;
        }
    </style>
</head>

<body>

    <h1>ESP32評価プログラム by ちみ</h1>

    <div class="container">
        <!-- SSID, IPアドレス, CPU温度 -->
        <div class="row">
            <div class="item">
                <label>SSID:</label>
                <div class="data" id="ssid">--</div>
            </div>
            <div class="item">
                <label>IPAddr:</label>
                <div class="data" id="ip-address">--</div>
            </div>
            <div class="item">
                <label>CPU:</label>
                <div class="data" id="cpu-temp">--</div>
            </div>
            <div class="item">
                <label>Temp:</label>
                <div class="data" id="temp">--</div>
            </div>
            <div class="item">
                <label>Him:</label>
                <div class="data" id="him">--</div>
            </div>
        </div>

        <!-- センサ1～5 -->
        <div class="row">
            <div class="item">
                <label>センサ1:</label>
                <div class="data" id="sensor1">--</div>
            </div>
            <div class="item">
                <label>センサ2:</label>
                <div class="data" id="sensor2">--</div>
            </div>
            <div class="item">
                <label>センサ3:</label>
                <div class="data" id="sensor3">--</div>
            </div>
            <div class="item">
                <label>センサ4:</label>
                <div class="data" id="sensor4">--</div>
            </div>
            <div class="item">
                <label>センサ5:</label>
                <div class="data" id="sensor5">--</div>
            </div>
        </div>

        <!-- GPIO1～5 -->
        <div class="row">
            <div class="item">
                <label>GPIO1:</label>
                <div class="data" id="gpio1">--</div>
            </div>
            <div class="item">
                <label>GPIO2:</label>
                <div class="data" id="gpio2">--</div>
            </div>
            <div class="item">
                <label>GPIO3:</label>
                <div class="data" id="gpio3">--</div>
            </div>
            <div class="item">
                <label>GPIO4:</label>
                <div class="data" id="gpio4">--</div>
            </div>
            <div class="item">
                <label>GPIO5:</label>
                <div class="data" id="gpio5">--</div>
            </div>
        </div>
    </div>

    <!-- Deep Sleep 設定 -->
    <div class="container">
        <div class="form-container">
            <label for="deep-sleep">Deep Sleep (μ秒):</label>
            <input type="number" id="deep-sleep" placeholder="μ秒">
            <button>保存</button>
            <button>スリープ</button>
        </div>
    </div>

    <!-- STA WiFi Config -->
    <div class="container">
        <div class="form-container">
            <label for="sta-ssid">SSID:</label>
            <input type="text" id="sta-ssid" placeholder="SSID">
            <label for="sta-pass">パスワード:</label>
            <input type="password" id="sta-pass" placeholder="パスワード">
            <button>保存</button>
            <button>接続</button>
        </div>
    </div>

    <!-- 現在時刻 (NTPとRTC) -->
    <div class="container">
        <div class="time-input">
            <div class="form-container" style="flex: 1; margin-right: 10px;">
                <label for="ntp-time">NTP:</label>
                <div class="data" id="ntp-time">--:--:--</div>
            </div>
            <div class="form-container" style="flex: 1; margin-left: 10px;">
                <label for="rtc-time">RTC:</label>
                <div class="data" id="rtc-time">--:--:--</div>
            </div>
        </div>
    </div>

</body>

</html>
)";
#endif /* __WIFI_ENABLE__ */
#endif /* APP_WIFI_INITAL_HTML_HPP */