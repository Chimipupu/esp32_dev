/**
 * @file app_esp_now.cpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESPNOW アプリ
 * @version 0.1
 * @date 2024-11-6
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "app_esp_now.hpp"

esp_now_peer_info_t s_peer_info;
esp_now_send_status_t s_tx_status;

e_esp_now_tx_type s_tx_data_type = TX_NONE;

static bool s_is_broadcat_data = false;
static bool s_is_rx_data = false;

static bool s_is_init_broadcat = false;

static bool s_is_unicast_peer = false;
static bool s_is_unicast_data = false;
static bool s_is_unicast_res = false;

static uint8_t s_rx_data_len = 0;
static uint8_t s_rx_data_buf[250] = {0};

static uint8_t s_tx_data_len = 0;
static uint8_t s_tx_data_buf[250] = {0};

static uint8_t s_my_mac_addr[6] = {0};
static uint8_t s_slave_mac_addr[6] = {0};
static uint8_t s_src_mac_addr[6] = {0};
static uint8_t s_des_mac_addr[6] = {0};

static char s_my_mac_str_buf[18] = {0};
static char s_slave_mac_str_buf[18] = {0};
static char s_src_mac_str_buf[18] = {0};
static char s_dst_mac_str_buf[18] = {0};

const uint8_t g_init_tx_str[] = "CQCQCQ";
const uint8_t g_req_peer_str[] = "REQ PEER";
const uint8_t g_res_peer_req_str[] = "RES PEER REQ OK";
const uint8_t g_tx_dmy_str[] = "Dummy Data";

static void printMacAddr(esp_mac_type_t type);
static void wifi_scan(void);
static void slave_addr_update(uint8_t *p_macaddr);
static void mac_addr_print(void);
static void tx_data_create(uint8_t *p_data, uint8_t data_len);
static void tx_data_print(void);
static void tx_proc_main(void);
static void rx_data_analyze(uint8_t *p_data);
static void rx_proc_main(void);

void tx_proc_cbk(const uint8_t *p_mac_addr, esp_now_send_status_t status)
{
    __DI(&g_mux);
    snprintf(s_src_mac_str_buf, sizeof(s_src_mac_str_buf), "%02x:%02x:%02x:%02x:%02x:%02x",
            s_my_mac_addr[0], s_my_mac_addr[1], s_my_mac_addr[2],
            s_my_mac_addr[3], s_my_mac_addr[4], s_my_mac_addr[5]);

    snprintf(s_dst_mac_str_buf, sizeof(s_dst_mac_str_buf), "%02X:%02X:%02X:%02X:%02X:%02X",
            p_mac_addr[0], p_mac_addr[1], p_mac_addr[2], p_mac_addr[3], p_mac_addr[4], p_mac_addr[5]);

    s_tx_status = status;
    __EI(&g_mux);
}

void rx_proc_cbk(const esp_now_recv_info_t *p_info, const uint8_t *p_data, int data_len)
{
    __DI(&g_mux);
    memcpy(s_src_mac_addr, p_info->src_addr, 6);
    memcpy(s_des_mac_addr, p_info->des_addr, 6);

    snprintf(s_src_mac_str_buf, sizeof(s_src_mac_str_buf), "%02x:%02x:%02x:%02x:%02x:%02x",
            p_info->src_addr[0], p_info->src_addr[1], p_info->src_addr[2],
            p_info->src_addr[3], p_info->src_addr[4], p_info->src_addr[5]);

    snprintf(s_dst_mac_str_buf, sizeof(s_dst_mac_str_buf), "%02x:%02x:%02x:%02x:%02x:%02x",
            p_info->des_addr[0], p_info->des_addr[1], p_info->des_addr[2],
            p_info->des_addr[3], p_info->des_addr[4], p_info->des_addr[5]);

    snprintf(s_my_mac_str_buf, sizeof(s_my_mac_str_buf), "%02x:%02x:%02x:%02x:%02x:%02x",
            s_my_mac_addr[0], s_my_mac_addr[1], s_my_mac_addr[2],
            s_my_mac_addr[3], s_my_mac_addr[4], s_my_mac_addr[5]);

    s_rx_data_len = data_len;
    memset(&s_rx_data_buf, 0x00, sizeof(s_rx_data_buf));
    for (int i = 0; i < data_len; i++)
    {
        s_rx_data_buf[i] = *p_data;
        p_data++;
    }
    s_is_rx_data = true;

    __EI(&g_mux);
}

static void wifi_scan(void)
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

static void slave_addr_update(uint8_t *p_macaddr)
{
    memcpy(s_peer_info.peer_addr, p_macaddr, 6);
    snprintf(s_slave_mac_str_buf, sizeof(s_slave_mac_str_buf), "%02x:%02x:%02x:%02x:%02x:%02x",
            p_macaddr[0], p_macaddr[1], p_macaddr[2],
            p_macaddr[3], p_macaddr[4], p_macaddr[5]);
    esp_now_add_peer(&s_peer_info);
}

static void mac_addr_print(void)
{
    DEBUG_PRINTF_RTOS("My MAC : %s\n", s_my_mac_str_buf);
    DEBUG_PRINTF_RTOS("Slave MAC : %s\n", s_slave_mac_str_buf);
    DEBUG_PRINTF_RTOS("Src MAC : %s\n", s_src_mac_str_buf);
    DEBUG_PRINTF_RTOS("Dest MAC : %s\n", s_dst_mac_str_buf);
}

static void rx_data_analyze(uint8_t *p_data)
{
    // ユニキャスト
    if (s_is_unicast_data != false) {
        if (strcmp((const char*)p_data, "REQ PEER") == 0) {
            s_tx_data_type = RES_PEER;
        } else {
            s_tx_data_type = TX_NONE;
            s_is_unicast_data = false;
        }
    }

    // ブロードキャスト
    if (s_is_broadcat_data != false) {
        if (strcmp((const char*)p_data, "CQCQCQ") == 0) {
            s_tx_data_type = REQ_PEER;
        } else {
            s_tx_data_type = TX_NONE;
            s_is_broadcat_data = false;
        }
    }
}

static void rx_proc_main(void)
{
    if (s_is_rx_data != false) {
        DEBUG_PRINTF_RTOS("\n");
        // ユニキャスト
        int result = memcmp(s_my_mac_addr, s_des_mac_addr, sizeof(s_my_mac_addr));
        if (result == 0) {
            DEBUG_PRINTF_RTOS("ESPNow Unicast Data!\n");
            s_is_unicast_data = true;
        }

        // ブロードキャスト
        result = strcmp("ff:ff:ff:ff:ff:ff", s_dst_mac_str_buf);
        if (result == 0) {
            DEBUG_PRINTF_RTOS("ESPNow Broadcast Data!\n");
            s_is_broadcat_data = true;
        }

        rx_data_analyze(&s_rx_data_buf[0]);

        mac_addr_print();
        DEBUG_PRINTF_RTOS("RX Data (%d Byte) : ", s_rx_data_len);
        for (int i = 0; i < s_rx_data_len; i++)
        {
            DEBUG_PRINTF_RTOS("%c", s_rx_data_buf[i]);
        }
        DEBUG_PRINTF_RTOS("\n");

        s_is_rx_data = false;
    }
}

static void tx_data_create(uint8_t *p_data, uint8_t data_len)
{
    __DI(&g_mux);
    s_tx_data_len = 0;
    memset(&s_tx_data_buf, 0x00, sizeof(s_tx_data_buf));
    for (uint8_t i = 0; i < data_len; i++)
    {
        s_tx_data_buf[i] = *p_data;
        p_data++;
        s_tx_data_len++;
    }
    __EI(&g_mux);
}

static void tx_data_print(void)
{
    DEBUG_PRINTF_RTOS("TX Data (%d Byte) : ", s_tx_data_len);
    for (int i = 0; i < s_tx_data_len; i++)
    {
        DEBUG_PRINTF_RTOS("%c", s_tx_data_buf[i]);
    }
    DEBUG_PRINTF_RTOS("\n");
}

static void tx_proc_main(void)
{
    switch (s_tx_data_type)
    {
        case ANY_ONE:
            tx_data_create((uint8_t*)&g_init_tx_str[0], sizeof(g_init_tx_str));
            break;

        case REQ_PEER:
            tx_data_create((uint8_t*)&g_req_peer_str[0], sizeof(g_req_peer_str));
            slave_addr_update(&s_src_mac_addr[0]);
            break;

        case RES_PEER:
            tx_data_create((uint8_t*)&g_res_peer_req_str[0], sizeof(g_res_peer_req_str));
            slave_addr_update(&s_src_mac_addr[0]);
            break;

        case TX_DATA:
            // TODO:一旦、ダミーデータ送信
            tx_data_create((uint8_t*)&g_tx_dmy_str[0], sizeof(g_tx_dmy_str));
            break;

        default:
            break;
    }

    if(s_tx_data_type != TX_NONE)
    {
        DEBUG_PRINTF_RTOS("\n");
        esp_err_t result = esp_now_send(s_peer_info.peer_addr, s_tx_data_buf, s_tx_data_len);
        if (result == ESP_OK) {
            mac_addr_print();
            DEBUG_PRINTF_RTOS("TX Status : %s\n", s_tx_status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
        }

        tx_data_print();
    }

    s_tx_data_type = TX_NONE;
}

void app_espnow_main(void)
{
    rx_proc_main();
    tx_proc_main();
}

static void printMacAddr(esp_mac_type_t type)
{
    uint8_t mac[6];

    esp_read_mac(mac, type);

    DEBUG_PRINTF_RTOS("%s MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                    type == ESP_MAC_WIFI_STA ? "WiFi STA" :
                    type == ESP_MAC_WIFI_SOFTAP ? "WiFi SoftAP" :
                    type == ESP_MAC_BT ? "Bluetooth" :
                    type == ESP_MAC_ETH ? "Ethernet" : "Unknown",
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void app_espnow_init(void)
{
    memset(&s_rx_data_buf, 0x00, sizeof(s_rx_data_buf));
    memset(&s_tx_data_buf, 0x00, sizeof(s_tx_data_buf));
    memset(&s_my_mac_addr, 0x00, sizeof(s_my_mac_addr));
    memset(&s_src_mac_addr, 0x00, sizeof(s_src_mac_addr));
    memset(&s_des_mac_addr, 0x00, sizeof(s_des_mac_addr));
    memset(&s_my_mac_str_buf, 0x00, sizeof(s_my_mac_str_buf));
    memset(&s_slave_mac_str_buf, 0x00, sizeof(s_slave_mac_str_buf));
    memset(&s_src_mac_str_buf, 0x00, sizeof(s_src_mac_str_buf));
    memset(&s_dst_mac_str_buf, 0x00, sizeof(s_dst_mac_str_buf));

#ifdef ESP_NOW_TX
    DEBUG_PRINTF_RTOS("ESPNow TX\n");
    s_tx_data_type = ANY_ONE;
#else
    DEBUG_PRINTF_RTOS("ESPNow RX\n");
#endif /* ESP_NOW_TX */

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
        DEBUG_PRINTF_RTOS("ESPNow Init Success\n");
    } else {
        DEBUG_PRINTF_RTOS("ESPNow Init Failed\n");
        ESP.restart();
    }
    // printMacAddr(ESP_MAC_WIFI_STA);
    esp_read_mac(s_my_mac_addr, ESP_MAC_WIFI_STA);
    snprintf(s_my_mac_str_buf, sizeof(s_my_mac_str_buf), "%02x:%02x:%02x:%02x:%02x:%02x",
            s_my_mac_addr[0], s_my_mac_addr[1], s_my_mac_addr[2],
            s_my_mac_addr[3], s_my_mac_addr[4], s_my_mac_addr[5]);
    wifi_scan();

    // ブロードキャスト = MACアドレス FF:FF:FF:FF:FF:FF
    __DI(&g_mux);
    memset(&s_peer_info, 0, sizeof(s_peer_info));
    memset(&s_slave_mac_addr, 0xff, sizeof(s_slave_mac_addr));
    memcpy(s_peer_info.peer_addr, s_slave_mac_addr, 6);
    s_peer_info.channel = 0;
    s_peer_info.encrypt = false;
    snprintf(s_slave_mac_str_buf, sizeof(s_slave_mac_str_buf), "%02x:%02x:%02x:%02x:%02x:%02x",
            s_slave_mac_addr[0], s_slave_mac_addr[1], s_slave_mac_addr[2],
            s_slave_mac_addr[3], s_slave_mac_addr[4], s_slave_mac_addr[5]);
    __EI(&g_mux);

    esp_err_t addStatus = esp_now_add_peer(&s_peer_info);
    if (addStatus == ESP_OK) {
        DEBUG_PRINTF_RTOS("ESPNow Init OK\n");
    }

    esp_now_register_send_cb(tx_proc_cbk);
    esp_now_register_recv_cb(rx_proc_cbk);
}