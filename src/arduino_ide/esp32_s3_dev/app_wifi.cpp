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
#include "app_ftp.hpp"
#include "app_file_system.hpp"

WebServer server(80);

// ファイルシステム
const char *CONFIG_FILE = "/wifi_config.json";

// 初期APのWiFi設定
const char *AP_SSID = "ESP32 Dev AP";
const char *AP_PASSWORD = "esp32dev";

// NTP
const char *ntpServer = "ntp.nict.jp";
const long gmtOffset_sec = 9 * 3600; // 日本時間 (UTC+9)
const int daylightOffset_sec = 0;

typedef enum {
    INDEX = 0,
    FACTOCY_CONFIG = 0x20,
    STA_WIFI_CONFIG = 0xFF
} e_html_type;

static e_html_type s_html_type = STA_WIFI_CONFIG;
static bool g_wifi_flag = false;
static tm s_ntp_timeinfo_t;
static tm s_rtc_timeinfo_t;

static bool loadWiFiConfig(void);
static void saveWiFiConfig(const String &ssid, const String &password);
static void setupAP(void);
static void handleRoot(void);
static void handleSave(void);
static void time_show(bool type);
static void set_ntp_to_rtc_time(void);
static void wifi_online_proc(void);
static void wifi_off_online_proc(void);
static void uap_mode(void);

WiFiConfig config;

static bool loadWiFiConfig(void)
{
    File configFile = FILE_SYSTEM.open(CONFIG_FILE, "r");
    if (!configFile)
    {
        Serial.println("ファイルシステムでファイルが見つかりません");
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

    File configFile = FILE_SYSTEM.open(CONFIG_FILE, "w");
    if (!configFile)
    {
        Serial.println("ファイルシステムで設定ファイルのオープンに失敗");
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
    if(s_html_type == INDEX){
        File html = FILE_SYSTEM.open("/index.html");
        server.streamFile(html, "text/html");
    } else if(s_html_type == FACTOCY_CONFIG){
        File html = FILE_SYSTEM.open("/factocy_config.html");
        server.streamFile(html, "text/html");
    } else if(s_html_type == STA_WIFI_CONFIG){
        server.send(200, "text/html", settingsHTML);
    }
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
        Serial.print("STA SSID & Password Saved! Now on reboot...");
        server.send(200, "text/plain", "設定を保存しました。デバイスを再起動します。");
        delay(2000);
        ESP.restart();
    }
    else
    {
        server.send(400, "text/plain", "無効なリクエストです");
    }
}

static void time_show(bool type)
{
    switch (type)
    {
        case NTP_TIME:
            getLocalTime(&s_ntp_timeinfo_t);
            Serial.printf("NTP: %04d/%02d/%02d %02d:%02d:%02d\n",
                            s_ntp_timeinfo_t.tm_year + 1900,
                            s_ntp_timeinfo_t.tm_mon + 1,
                            s_ntp_timeinfo_t.tm_mday,
                            s_ntp_timeinfo_t.tm_hour,
                            s_ntp_timeinfo_t.tm_min,
                            s_ntp_timeinfo_t.tm_sec);
            break;

        case RTC_TIME:
            getLocalTime(&s_rtc_timeinfo_t);
            Serial.printf("RTC: %04d/%02d/%02d %02d:%02d:%02d\n",
                            s_rtc_timeinfo_t.tm_year + 1900,
                            s_rtc_timeinfo_t.tm_mon + 1,
                            s_rtc_timeinfo_t.tm_mday,
                            s_rtc_timeinfo_t.tm_hour,
                            s_rtc_timeinfo_t.tm_min,
                            s_rtc_timeinfo_t.tm_sec);
            break;

        default:
            break;
    }
}

static void set_ntp_to_rtc_time(void)
{
    char timeStr[100], rtcStr[100];

    Serial.println("NTPとRTCを同期開始");
    time_show(NTP_TIME);
    strftime(timeStr, sizeof(timeStr), "NTP: %Y/%m/%d %H:%M:%S", &s_ntp_timeinfo_t);

    time_t now;
    time(&now);
    struct timeval tv = {.tv_sec = now, .tv_usec = 0};
    settimeofday(&tv, NULL);

    Serial.println("NTPとRTCを同期完了");
    time_show(RTC_TIME);
}

static void wifi_online_proc(void)
{
    app_ftp_main();
}

static void wifi_off_online_proc(void)
{
    g_wifi_flag = true;
}

static void uap_mode(void)
{
    setupAP();
    Serial.println("Web鯖 begin...");
    s_html_type = STA_WIFI_CONFIG;
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
    server.handleClient();
}

void app_wifi_init(void)
{
    time_show(RTC_TIME);

    Serial.println("ファイルシステム初期化");

    if (!FILE_SYSTEM.begin())
    {
        Serial.println("ファイルシステム マウント失敗");
#if 0
        Serial.println("ファイルシステムをフォーマット");
        FILE_SYSTEM.format();

        Serial.println("ファイルシステムのフォーマット完了、rebootします!");
#endif
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
        while (WiFi.status() != WL_CONNECTED && attempts < 30)
        {
            delay(500);
            Serial.print(".");
            attempts++;
        }

        Serial.print("\n");

        if (WiFi.status() == WL_CONNECTED)
        {
            g_wifi_flag = true;
            Serial.println("\nWiFi接続完了!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());

            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
            Serial.println("NTPサーバーに接続中...");

            set_ntp_to_rtc_time();

            // WEB Server
            s_html_type = INDEX;
            server.on("/", handleRoot);
            server.begin();
            server.handleClient();

            time_show(NTP_TIME);
            time_show(RTC_TIME);

            // FTP Server
            app_ftp_init();

            // Serial.println("WiFiから切断");
            // WiFi.disconnect();
            // g_wifi_flag = false;
        }
        else
        {
            g_wifi_flag = true;
            uap_mode();
        }
    }
    else
    {
        g_wifi_flag = true;
        uap_mode();
    }
}

bool app_wifi_main(void)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        wifi_online_proc();
    }else{
        wifi_off_online_proc();
    }

    server.handleClient();

    return g_wifi_flag;
}