/**
 * @file app_wifi.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ESP32 WiFiアプリ
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "common.hpp"

#ifdef __WIFI_ENABLE__
#include"app_wifi.hpp"
#include "app_wifi_inital_html.hpp"
#include "app_ftp.hpp"
#include "app_file_system.hpp"
#include "app_neopixel.hpp"

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

// IP/MAC Addr、SSID、パスワード
String g_ip_addr_str;
String g_mac_addr_str;
String g_wifi_ssid_str;
String g_wifi_password_str;
uint8_t *p_ip_addr = NULL;
uint8_t *p_mac_addr = NULL;
uint8_t *p_wifi_ssid = NULL;
uint8_t *p_wifi_password = NULL;

typedef enum {
    INDEX = 0,
    FACTOCY_CONFIG = 0x20,
    STA_WIFI_CONFIG = 0xFF
} e_html_type;

static e_html_type s_html_type = STA_WIFI_CONFIG;
// static rgbled_state_t s_rgbled_state;
static tm s_ntp_timeinfo_t;
static tm s_rtc_timeinfo_t;
static bool s_wifi_flag = false;
static bool s_ap_flg = false;
static bool s_ftp_flg = false;

static bool fs_read_wifi_config(void);
static void saveWiFiConfig(const String &ssid, const String &password);
static void setupAP(void);
static void handleRoot(void);
static void handleSave(void);
static void time_show(bool type);
static void set_ntp_to_rtc_time(void);
static void wifi_online_proc(void);
static void wifi_off_online_proc(void);
static void ap_mode_main(void);
static void sta_mode_main(void);
static void wifi_connect_info_print(void);

WiFiConfig g_wifi_config;
IPAddress g_ip_addr;

/**
 * @brief WiFi接続情報表示（IPアドレス、MACアドレス、SSID、パスワード）
 * 
 */
static void wifi_connect_info_print(void)
{
    g_ip_addr           = WiFi.localIP();

    // String型を保存
    g_ip_addr_str       = g_ip_addr.toString();
    g_mac_addr_str      = WiFi.macAddress();
    g_wifi_ssid_str     = g_wifi_config.ssid;
    g_wifi_password_str = g_wifi_config.password;

    // String型 -> uint8_tのポインタに変換
    p_ip_addr           = (uint8_t *)g_ip_addr_str.c_str();
    p_mac_addr          = (uint8_t *)g_mac_addr_str.c_str();
    p_wifi_ssid         = (uint8_t *)g_wifi_ssid_str.c_str();
    p_wifi_password     = (uint8_t *)g_wifi_password_str.c_str();

    // printf()
    DEBUG_PRINTF_RTOS("IP Address : %s\n", p_ip_addr);
    DEBUG_PRINTF_RTOS("WiFi MAC Address : %s\n", p_mac_addr);
    DEBUG_PRINTF_RTOS("STA SSID : %s\n", p_wifi_ssid);
    DEBUG_PRINTF_RTOS("STA Password : %s\n", p_wifi_password);
}

static bool fs_read_wifi_config(void)
{
    File configFile = FILE_SYSTEM.open(CONFIG_FILE, "r");
    if (!configFile)
    {
        DEBUG_PRINTF_RTOS("ファイルシステムでファイルが見つかりません\n");
        return false;
    }

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, configFile);
    configFile.close();

    if (error)
    {
        DEBUG_PRINTF_RTOS("設定の解析に失敗しました\n");
        return false;
    }

    g_wifi_config.ssid = doc["ssid"].as<String>();
    g_wifi_config.password = doc["password"].as<String>();
    DEBUG_PRINTF_RTOS("WiFi設定を読み込みました\n");
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
        DEBUG_PRINTF_RTOS("ファイルシステムで設定ファイルのオープンに失敗\n");
        return;
    }

    serializeJson(doc, configFile);
    configFile.close();
    DEBUG_PRINTF_RTOS("WiFi設定を保存しました\n");
}

static void setupAP(void)
{
    WiFi.mode(WIFI_AP);
    DEBUG_PRINTF_RTOS("APモードを開始します\n");
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    DEBUG_PRINTF_RTOS("AP SSID : %s\n", AP_SSID);
    DEBUG_PRINTF_RTOS("AP Password : %s\n", AP_PASSWORD);

    DEBUG_PRINTF_RTOS("AP Web Server IP addr : %s\n", WiFi.softAPIP().toString().c_str());
    String str = WiFi.macAddress();
    DEBUG_PRINTF_RTOS("WiFi MAC addr : %s\n", str.c_str());
}

static void handleRoot(void)
{
    if(s_html_type == INDEX){
        // File html = FILE_SYSTEM.open("/index.html");
        // server.streamFile(html, "text/html");
        server.send(200, "text/html", p_indexHTML);
    } else if(s_html_type == FACTOCY_CONFIG){
        File html = FILE_SYSTEM.open("/factocy_config.html");
        server.streamFile(html, "text/html");
    } else if(s_html_type == STA_WIFI_CONFIG){
        server.send(200, "text/html", p_settingsHTML);
    }
}

static void handleSave(void)
{
    if (server.hasArg("ssid") && server.hasArg("password"))
    {
        String ssid = server.arg("ssid");
        String password = server.arg("password");

        DEBUG_PRINTF_RTOS("STA SSID : %s\n", ssid.c_str());
        DEBUG_PRINTF_RTOS("STA Password : %s\n", password.c_str());

        saveWiFiConfig(ssid, password);
        DEBUG_PRINTF_RTOS("STA SSID & Password Saved! Now on reboot...\n");
        server.send(200, "text/plain", "設定を保存しました。デバイスを再起動します。");
        delay(2000);

        // reboot
        ESP.restart();
    } else {
        server.send(400, "text/plain", "無効なリクエストです");
    }
}

static void time_show(bool type)
{
    switch (type)
    {
        case NTP_TIME:
            getLocalTime(&s_ntp_timeinfo_t);
            DEBUG_PRINTF_RTOS("NTP: %04d/%02d/%02d %02d:%02d:%02d\n",
                            s_ntp_timeinfo_t.tm_year + 1900,
                            s_ntp_timeinfo_t.tm_mon + 1,
                            s_ntp_timeinfo_t.tm_mday,
                            s_ntp_timeinfo_t.tm_hour,
                            s_ntp_timeinfo_t.tm_min,
                            s_ntp_timeinfo_t.tm_sec);
            break;

        case RTC_TIME:
            getLocalTime(&s_rtc_timeinfo_t);
            DEBUG_PRINTF_RTOS("RTC: %04d/%02d/%02d %02d:%02d:%02d\n",
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

    DEBUG_PRINTF_RTOS("NTPとRTCを同期開始\n");

    strftime(timeStr, sizeof(timeStr), "NTP: %Y/%m/%d %H:%M:%S", &s_ntp_timeinfo_t);
    time_t now;
    time(&now);
    struct timeval tv = {.tv_sec = now, .tv_usec = 0};
    settimeofday(&tv, NULL);

    DEBUG_PRINTF_RTOS("NTPとRTCを同期完了\n");
    time_show(NTP_TIME);
    time_show(RTC_TIME);
}

static void wifi_online_proc(void)
{
    if(s_ftp_flg != false)
    {
        app_neopixel_set_color(0, &g_led_color_tbl[TBL_IDX_COLOR_BLUE]);
#ifdef __FTP_ENABLE__
        app_ftp_main();
#endif
    } else {
        wifi_connect_info_print();
        app_neopixel_set_color(0, &g_led_color_tbl[TBL_IDX_COLOR_WHITE]);
    }
}

static void wifi_off_online_proc(void)
{
    if(s_ap_flg != false){
        app_neopixel_set_color(0, &g_led_color_tbl[TBL_IDX_COLOR_BLUE]);
    }else{
        app_neopixel_set_color(0, &g_led_color_tbl[TBL_IDX_COLOR_RED]);
    }
}

static void sta_mode_main(void)
{
    DEBUG_PRINTF_RTOS("STAモードで接続を試みます\n");
    WiFi.mode(WIFI_STA);
    DEBUG_PRINTF_RTOS("STA SSID : %s\n", g_wifi_config.ssid.c_str());
    DEBUG_PRINTF_RTOS("STA Password : %s\n", g_wifi_config.password.c_str());
    WiFi.begin(g_wifi_config.ssid.c_str(), g_wifi_config.password.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30)
    {
        delay(500);
        DEBUG_PRINTF_RTOS(".");
        attempts++;
    }

    DEBUG_PRINTF_RTOS("\n");

    if (WiFi.status() == WL_CONNECTED)
    {
        s_wifi_flag = true;
        app_neopixel_set_color(0, &g_led_color_tbl[TBL_IDX_COLOR_GREEN]);

        DEBUG_PRINTF_RTOS("\nWiFi接続完了!\n");
        wifi_connect_info_print();

        DEBUG_PRINTF_RTOS("NTPサーバーに接続...\n");
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

        set_ntp_to_rtc_time();

        // WEB Server
        s_html_type = INDEX;
        server.on("/", handleRoot);
        server.begin();
        server.handleClient();

#ifdef __FTP_ENABLE__
        // FTP Server
        app_ftp_init();
        s_ftp_flg = true;
#endif // __FTP_ENABLE__

#if 0
        // WiFi切断
        DEBUG_PRINTF_RTOS("WiFiから切断\n");
        WiFi.disconnect();
        s_wifi_flag = false;
#else
        // WiFiの接続を継続
        s_wifi_flag = true;
#endif
    } else {
        s_wifi_flag = true;
        ap_mode_main();
    }
}

static void ap_mode_main(void)
{
    s_ap_flg = true;
    app_neopixel_set_color(0, &g_led_color_tbl[TBL_IDX_COLOR_BLUE]);

    setupAP();

    DEBUG_PRINTF_RTOS("Web鯖 begin...\n");
    s_html_type = STA_WIFI_CONFIG;
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
    server.handleClient();
}

void app_wifi_scan(void)
{
    DEBUG_PRINTF_RTOS("WiFi scan...\n");
    uint8_t network_cnt = WiFi.scanNetworks();

    if (network_cnt == 0) {
        DEBUG_PRINTF_RTOS("no WiFi network\n");
    } else {
        DEBUG_PRINTF_RTOS("found %d WiFi network\n", network_cnt);
        for (uint8_t i = 0; i < network_cnt; ++i) {
            DEBUG_PRINTF_RTOS("[WiFi Network No.%d]\n", i);
            String ssid = WiFi.SSID(i);
            DEBUG_PRINTF_RTOS("[SSID:%s] [RSSI:%d dBm] [Ch:%d]\n", ssid.c_str(), WiFi.RSSI(i), WiFi.channel(i));
        }
    }

    DEBUG_PRINTF_RTOS("");
}

/**
 * @brief WiFi アプリ初期化
 * 
 */
void app_wifi_init(void)
{
    time_show(RTC_TIME);

    DEBUG_PRINTF_RTOS("ファイルシステム初期化\n");

    if (!FILE_SYSTEM.begin())
    {
        DEBUG_PRINTF_RTOS("ファイルシステム マウント失敗\n");
#ifdef FILESYSTEM_RESET
        DEBUG_PRINTF_RTOS("ファイルシステムをフォーマット\n");
        FILE_SYSTEM.format();

        DEBUG_PRINTF_RTOS("ファイルシステムのフォーマット完了、rebootします!\n");
#endif /* FILESYSTEM_RESET */
        ESP.restart();
    }

    DEBUG_PRINTF_RTOS("WiFiをスキャン中...\n");
    app_wifi_scan();

    DEBUG_PRINTF_RTOS("WiFi設定をファイルシステムから読み込み中...\n");
    if (fs_read_wifi_config() && g_wifi_config.ssid.length() > 0) {
        sta_mode_main();
    } else {
        s_wifi_flag = true;
        ap_mode_main();
    }
}

/**
 * @brief WiFi アプリメイン
 * 
 * @return true WiFiオンライン
 * @return false WiFiオフライン
 */
bool app_wifi_main(void)
{
    if ((s_wifi_flag != true) || (WiFi.status() != WL_CONNECTED))
    {
        wifi_off_online_proc();
    } else {
        wifi_online_proc();
    }

    server.handleClient();

    return s_wifi_flag;
}
#endif /* __WIFI_ENABLE__ */