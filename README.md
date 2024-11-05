# 📍ESP32評価F/W by ちみ
RP2040評価F/W 個人開発リポジトリ🥳
- 📍CPU ... ESP32-S3
  - 📍CPU0 ... Xtensa LX7
  - 📍CPU1 ... Xtensa LX7
- 📍OS ... FreeRTOS
  - 📍CPU0 ... ✅FreeRTOS(搭載済み)
  - 📍CPU1 ... ✅FreeRTOS(搭載済み)
****

# 開発環境
[開発環境の詳細(esp32_dev.md)🔗](/doc/esp32_dev.md)

- 📍[Arduino IDE v2.33](https://github.com/arduino/arduino-ide/releases/tag/2.3.3)
  - [Arduino core for the esp32 v3.0.7🔗](https://github.com/espressif/arduino-esp32/releases/tag/3.0.7)
- 📍基板
  - [VCC-GND YD-ESP32-S3🔗](https://github.com/vcc-gnd/YD-ESP32-S3)
  - [M5Stack Atom S3 Lite🔗](https://docs.m5stack.com/en/core/AtomS3%20Lite)

****

## 📍F/W実装済み
- ✅Core0 ... ドライバコアCPU
  - ✅FreeRTOS
    - ✅loopTask
      - ドライバ関連
- ✅Core1 ... アプリコアCPU
  - ✅FreeRTOS
    - ✅loopTask
      - ✅DeepSleep : TBD
      - TBD
    - ✅ESPNowタスク🛜
      - ✅送信、受信🛜
        - ✅ユニキャスト🛜
        - ✅ブロードキャスト🛜
    - ✅WiFiタスク🛜
      - ✅AP🛜
        - ✅HTTP : HTMLでSTAのSSIDとパスワード入力受け
      - ✅STA🛜
        - ✅FTP : FATFSでフラッシュにファイルをR/W
        - ✅NTP : RTCと同期
        - ✅HTTP : HTMLに各種データのI/O

- ✅RGB LED (@GPIO 35)
  - ✅`赤` : オフライン状態（WiFi未接続）
  - ✅`緑` : オンライン状態（STAモード）
  - ✅`青` : APモード
  - ✅`白` : ESP-NOW マスター
  - ✅`橙` : ESP-NOW スレーブ
  - ✅`紫` : エラー状態

- ✅割込み(IRQ)
  - ✅ボタンIRQ (@GPIO 41)
    - `ボタン単押し`:ボタン一回押しを検知
    - `ボタン複数押し`:短時間に複数回ボタン押しを検知
    - `ボタン長押し`:ボタン長押しを検知（1000ms = 1s）
    - `ボタン超長押し`:ボタン長押しを検知（3000ms = 2s）