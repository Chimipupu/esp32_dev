/**
 * @file app_wifi.cpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESP32 WiFiアプリ
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include"app_wifi.hpp"
#include "app_wifi_inital_html.hpp"

WebServer server(80);

// DeepSleep
#include "esp_sleep.h"
#define DEEPSLEEP_TIME_US    180 * 1000000 // 3分

// FATFS
const char *CONFIG_FILE = "/wifi_config.json";

// 初期APのWiFi設定
const char *AP_SSID = "ESP32 Dev AP";
const char *AP_PASSWORD = "esp32dev";

// NTP
const char *ntpServer = "ntp.nict.jp";
const long gmtOffset_sec = 9 * 3600; // 日本時間 (UTC+9)
const int daylightOffset_sec = 0;

static bool loadWiFiConfig(void);
static void saveWiFiConfig(const String &ssid, const String &password);
static void setupAP(void);
static void handleRoot(void);
static void handleSave(void);
static void set_ntp_to_rtc_time(void);

WiFiConfig config;

static bool loadWiFiConfig(void)
{
    File configFile = FFat.open(CONFIG_FILE, "r");
    if (!configFile)
    {
        Serial.println("FATFSでファイルが見つかりません");
        return false;
    }

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, configFile);
    configFile.close();

    if (error)
    {
        Serial.println("設定の解析に失敗しました");
        return false;
    }

    config.ssid = doc["ssid"].as<String>();
    config.password = doc["password"].as<String>();
    Serial.println("WiFi設定を読み込みました");
    return true;
}

// WiFi設定を保存
static void saveWiFiConfig(const String &ssid, const String &password)
{
    StaticJsonDocument<200> doc;
    doc["ssid"] = ssid;
    doc["password"] = password;

    File configFile = FFat.open(CONFIG_FILE, "w");
    if (!configFile)
    {
        Serial.println("FATFSで設定ファイルのオープンに失敗");
        return;
    }

    serializeJson(doc, configFile);
    configFile.close();
    Serial.println("WiFi設定を保存しました");
}

static void setupAP(void)
{
    WiFi.mode(WIFI_AP);
    Serial.println("APモードを開始します");
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    Serial.print("AP SSID: ");
    Serial.println(AP_SSID);
    Serial.print("AP Password: ");
    Serial.println(AP_PASSWORD);

    Serial.print("AP Web Server IP addr: ");
    Serial.println(WiFi.softAPIP());
}

static void handleRoot(void)
{
    server.send(200, "text/html", settingsHTML);
}

static void handleSave(void)
{
    if (server.hasArg("ssid") && server.hasArg("password"))
    {
        String ssid = server.arg("ssid");
        String password = server.arg("password");

        Serial.print("STA SSID: ");
        Serial.println(ssid);
        Serial.print("STA Password: ");
        Serial.println(password);

        saveWiFiConfig(ssid, password);
        Serial.print("STA SSID＆Password Saved! Now on reboot...");
        server.send(200, "text/plain", "設定を保存しました。デバイスを再起動します。");
        delay(2000);
        ESP.restart();
    }
    else
    {
        server.send(400, "text/plain", "無効なリクエストです");
    }
}

static void set_ntp_to_rtc_time(void)
{
    char timeStr[100], rtcStr[100];
    struct tm timeinfo;
    struct tm rtc_timeinfo;

    Serial.println("NTPとRTCを同期開始");

    if (getLocalTime(&timeinfo))
    {
        Serial.printf("NTP: %04d/%02d/%02d %02d:%02d:%02d\n",
                        timeinfo.tm_year + 1900,
                        timeinfo.tm_mon + 1,
                        timeinfo.tm_mday,
                        timeinfo.tm_hour,
                        timeinfo.tm_min,
                        timeinfo.tm_sec);
    }

    strftime(timeStr, sizeof(timeStr), "NTP: %Y/%m/%d %H:%M:%S", &timeinfo);

    time_t now;
    time(&now);
    struct timeval tv = {.tv_sec = now, .tv_usec = 0};
    settimeofday(&tv, NULL);
    Serial.println("NTPとRTCを同期完了");
}

void app_wifi_init(void)
{
    // Deep Sleep
    esp_sleep_enable_timer_wakeup (DEEPSLEEP_TIME_US);

    Serial.println("FATFS初期化");
    if (!FFat.begin(true))
    {
        Serial.println("工場出荷:reboot");
        ESP.restart();
    }

    Serial.println("WiFi設定を読み込み中...");
    if (loadWiFiConfig() && config.ssid.length() > 0)
    {
        Serial.println("STAモードで接続を試みます");
        WiFi.mode(WIFI_STA);
        Serial.print("STA SSID: ");
        Serial.println(config.ssid.c_str());
        Serial.print("STA Password: ");
        Serial.println(config.password.c_str());
        WiFi.begin(config.ssid.c_str(), config.password.c_str());

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 60)
        {
            delay(500);
            Serial.print(".");
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nWiFi接続完了!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());

            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
            Serial.println("NTPサーバーに接続中...");
            set_ntp_to_rtc_time();
            Serial.println("WiFiから切断");
            WiFi.disconnect();
        }
        else
        {
            Serial.println("\nWiFi接続失敗: APモードに切り替え");
            setupAP();
        }
    }
    else
    {
        setupAP();
    }

    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
}

void app_wifi_main(void)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        server.handleClient();
    }else{
        static unsigned long lastPrint = 0;
        if (millis() - lastPrint >= 1000)
        {
            struct tm rtc_timeinfo;
            getLocalTime(&rtc_timeinfo);
            Serial.printf("RTC: %04d/%02d/%02d %02d:%02d:%02d\n",
                            rtc_timeinfo.tm_year + 1900,
                            rtc_timeinfo.tm_mon + 1,
                            rtc_timeinfo.tm_mday,
                            rtc_timeinfo.tm_hour,
                            rtc_timeinfo.tm_min,
                            rtc_timeinfo.tm_sec);
            lastPrint = millis();
        }

        // DeepSleep @DEEPSLEEP_TIME_US
        Serial.printf("DeepSleep @%d min", (DEEPSLEEP_TIME_US / 60) / 1000000);
        esp_deep_sleep_start();
    }
}