/**
 * @file app_wifi_inital_html.hpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESP32 初期AP HTML
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef APP_WIFI_INITAL_HTML_HPP
#define APP_WIFI_INITAL_HTML_HPP

const char *settingsHTML = R"(
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

#endif /* APP_WIFI_INITAL_HTML_HPP */