#include <stdint.h>
#include <string.h>
#include <esp_now.h>
#include <WiFi.h>

// #define ESP_NOW_TX
#define ESP_NOW_RX

esp_now_peer_info_t slave;
static uint8_t s_my_mac_addr[6] = {0};

// 送信コールバック
void tx_proc_cbk(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    Serial.print("TX to MAC : ");
    Serial.println(macStr);
    Serial.print("TX Packet Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// 受信コールバック
void rx_proc_cbk(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    char macStr[18];
    char msg[1];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    Serial.printf("RX from MAC : %s\n", macStr);
    Serial.printf("RX Data(%d): ", data_len);
    for (int i = 0; i < data_len; i++)
    {
        msg[1] = data[i];
        Serial.print(msg[1]);
    }
    Serial.println("");
}

void app_espnow_main(void)
{
#ifdef ESP_NOW_TX
    // ASCII "Hello ESP-NOW"
    uint8_t data[13] = {72, 101, 108, 108, 111, 32, 69, 83, 80, 45, 78, 79, 87};

    esp_err_t result = esp_now_send(slave.peer_addr, data, sizeof(data));
    Serial.print("Send Status: ");
    if (result == ESP_OK) {
        Serial.println("Success");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
        Serial.println("ESPNOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
        Serial.println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
        Serial.println("Peer not found.");
    } else {
        Serial.println("Not sure what happened");
    }
#endif
}

void app_espnow_init(void)
{
#ifdef ESP_NOW_TX
    Serial.println("ESPNow TX");
#else
    Serial.println("ESPNow RX");
#endif

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
        Serial.println("ESPNow Init Success");
    } else {
        Serial.println("ESPNow Init Failed");
        ESP.restart();
    }
    memset(&s_my_mac_addr, 0x00, sizeof(s_my_mac_addr));
    esp_read_mac(s_my_mac_addr ,ESP_MAC_WIFI_STA);
    Serial.printf("WiFi MAC : %s\n", s_my_mac_addr);

    // ブロードキャスト = MACアドレス FF:FF:FF:FF:FF:FF
    memset(&slave, 0, sizeof(slave));
    for (int i = 0; i < 6; ++i) {
        slave.peer_addr[i] = (uint8_t)0xff;
    }
    Serial.printf("Slave MAC : %s\n", slave.peer_addr);

    esp_err_t addStatus = esp_now_add_peer(&slave);
    if (addStatus == ESP_OK) {
        Serial.println("ESPNow Init OK");
    }

    esp_now_register_send_cb(tx_proc_cbk);
    esp_now_register_recv_cb(rx_proc_cbk);
}

void setup()
{
    Serial.begin(115200);
    app_espnow_init();
}

void loop()
{
    app_espnow_main();
    delay(1000);
}