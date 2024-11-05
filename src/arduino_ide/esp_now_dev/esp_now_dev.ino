#include <stdint.h>
#include <string.h>
#include <esp_system.h>
#include "esp_mac.h"
#include <esp_wifi.h>
#include <esp_now.h>
#include <WiFi.h>

// #define ESP_NOW_TX
#define ESP_NOW_RX

esp_now_peer_info_t slave;
static uint8_t s_my_mac_addr[6] = {0};
static uint8_t s_slave_mac_addr[6] = {0};

// 送信コールバック
void tx_proc_cbk(const uint8_t *p_mac_addr, esp_now_send_status_t status)
{
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            p_mac_addr[0], p_mac_addr[1], p_mac_addr[2], p_mac_addr[3], p_mac_addr[4], p_mac_addr[5]);

    Serial.print("TX to MAC : ");
    Serial.println(macStr);
    Serial.print("TX Packet Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// 受信コールバック
void rx_proc_cbk(const esp_now_recv_info_t *p_info, const uint8_t *p_data, int data_len)
{
    char data;
    char src_macStr[18] = {0};
    char dest_macStr[18] = {0};

    snprintf(src_macStr, sizeof(src_macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            p_info->src_addr[0], p_info->src_addr[1], p_info->src_addr[2],
            p_info->src_addr[3], p_info->src_addr[4], p_info->src_addr[5]);

    snprintf(dest_macStr, sizeof(dest_macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            p_info->des_addr[0], p_info->des_addr[1], p_info->des_addr[2],
            p_info->des_addr[3], p_info->des_addr[4], p_info->des_addr[5]);

    Serial.printf("Src MAC : %s\r\n", src_macStr);
    Serial.printf("Dest MAC : %s\r\n", dest_macStr);
    Serial.printf("RX Data (%d Byte): ", data_len);
    for (int i = 0; i < data_len; i++)
    {
        data = *p_data;
        p_data++;
        Serial.print(data);
    }
    Serial.printf("\r\n");
}

void app_espnow_main(void)
{
#ifdef ESP_NOW_TX
    // ASCII "Hello ESP-NOW"
    uint8_t data[13] = {72, 101, 108, 108, 111, 32, 69, 83, 80, 45, 78, 79, 87};

    Serial.printf("TX data : %s\r\n", data);
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

void printMacAddr(esp_mac_type_t type)
{
    uint8_t mac[6];
    esp_read_mac(mac, type);
    Serial.printf("%s MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                    type == ESP_MAC_WIFI_STA ? "WiFi STA" :
                    type == ESP_MAC_WIFI_SOFTAP ? "WiFi SoftAP" :
                    type == ESP_MAC_BT ? "Bluetooth" :
                    type == ESP_MAC_ETH ? "Ethernet" : "Unknown",
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void app_espnow_init(void)
{
    memset(&s_my_mac_addr, 0x00, sizeof(s_my_mac_addr));
    memset(&s_slave_mac_addr, 0x00, sizeof(s_slave_mac_addr));

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
    printMacAddr(ESP_MAC_WIFI_STA);
    // Serial.printf("WiFi MAC : %s\n", s_my_mac_addr);

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
    delay(3000);
}