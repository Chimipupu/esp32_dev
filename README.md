# ESP32 WiFi 評価F/W開発
ESP32のWiFi評価F/W個人開発リポジトリ

- 📍ESP32-S3
  - 型番 : [ESP32S3N16R8ESP32-S3-WROOM-1-N16R8](https://akizukidenshi.com/catalog/g/g117256/)🔗
    - 📍CPU0 ... Xtensa LX7
    - 📍CPU1 ... Xtensa LX7
    - クロック ... 240MHz
    - 💾Flash ... 16MB
    - 💾RAM
      - SRAM ... 520KB
      - RTC Slow Mem ... 16KB
      - PSRAM ... 8MB
    - 🛜WiFi ... IEEE802.11 b/g/n @2.4GHz
      - 🛜ESP-NOW
    - 🛜Bluetooth ... BLE 5、Mesh

- 📍OS ... FreeRTOS
  - 📍CPU0 ... ✅FreeRTOS(搭載済み)
  - 📍CPU1 ... ✅FreeRTOS(搭載済み)

![](/doc/espnow_req_res.png)

****

## 📍開発環境

- 📍[Arduino IDE v2.36](https://github.com/arduino/arduino-ide/releases/tag/2.3.6)
  - [arduino-esp32 v3.3.2](https://github.com/espressif/arduino-esp32/releases/tag/3.3.2)🔗
- 📍基板
  - ESP32-S3
    - [VCC-GND YD-ESP32-S3](https://github.com/vcc-gnd/YD-ESP32-S3)🔗
    - [M5Stack Atom S3 Lite](https://docs.m5stack.com/en/core/AtomS3%20Lite)🔗

****

## 📍実装機能
**凡例(✅実装＆テスト完了、🆗実装済み、🚩TODO、❌TBD)**

- 📍Core0 ... ドライバコアCPU
  - 📍FreeRTOS
    - ✅loopTask
      - ドライバ関連
- 📍Core1 ... アプリコアCPU
  - 📍FreeRTOS
    - ✅loopTask
      - ✅DeepSleep : TBD
      - TBD
    - ✅ESP-NOWタスク🛜
      - ✅送信、受信🛜
        - ✅ユニキャスト🛜
        - ✅ブロードキャスト🛜
    - ✅WiFiタスク🛜
      - ✅AP🛜
        - ✅HTTP : HTMLでSTAのSSIDとパスワード入力受け
      - ✅STA🛜
        - ✅HTTP : HTMLに各種データのI/O
        - ✅NTP : RTCと同期
        - ✅FTP : FATFSでフラッシュにファイルをR/W

- 📍RGB LED (@GPIO 35)
  - ✅赤🔴 : オフライン状態（WiFi未接続）
  - ✅緑🔵 : オンライン状態（STAモード）
  - ✅青🟢 : APモード
  - ✅白⚪ : ESP-NOW マスター
  - ✅橙🟠 : ESP-NOW スレーブ
  - ✅紫🟣 : エラー状態

- 📍割込み(IRQ)
  - ✅ボタンIRQ (@GPIO 41)
    - ✅`ボタン単押し`:ボタン一回押しを検知
    - ✅`ボタン複数押し`:短時間に複数回ボタン押しを検知
    - ✅`ボタン長押し`:ボタン長押しを検知（1000ms = 1s）
    - ✅`ボタン超長押し`:ボタン長押しを検知（3000ms = 3s）