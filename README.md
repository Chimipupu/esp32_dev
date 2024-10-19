# ESP32 FreeRTOS F/W
ESP32 FreeRTOS F/W🥳

## ✅実装した機能
- ✅Core0 ... ドライバコアCPU
  - ✅loopTask
    - ドライバ関連

- ✅Core1 ... アプリコアCPU
    - ✅WiFiアプリ
        - ✅RGB LED : WiFiの状態に応じて色表示
          - 赤 : オフライン状態（WiFi未接続）
          - 青 : APモード
          - 緑 : オンライン(STAモード)
        - ✅AP
          - HTTP : HTMLでSTAのSSIDとパスワード入力受け
        - ✅STA
          - FTP : FATFSでフラッシュにファイルをR/W
          - NTP : RTCと同期
          - HTTP : HTMLに各種データのI/O
      - ✅DeepSleep : TBD
    - ✅loopTask
      - TBD

## 🛠️TBD
- 🛠️PSRAM ... MBの大容量なヒープ領域
- 🛠️割込み（IRQ） ... ボタンやPIOからIRQがほしい
- 🛠️タイマ⏰ ... インターバル⏰とワンショットタイマ⏰ほしい
- 🛠️アプリ追加 ... I2C、SPI、PIO、スリープ機能
- 🛠️ESP NOW ... デバイス同士で通信するとき
- 🛠️WDT🐶 ... 最後にWDTで番犬わんわんさせる

## 🤖開発基板
[VCC-GND YD-ESP32-S3]

https://github.com/vcc-gnd/YD-ESP32-S3

- 機能
  - `N16R8`
    - フラッシュ : 16 MB
    - PSRAM : 8 MB
    - SRAM : 512 KB
  - `WS2812` ... GPIO 48
  - `CH343` ... UART to USB
  - GPIO ... x45
  - SPI ... x4ch
  - UART ... x3ch
  - I2C ... x2ch
  - I2C ... x2ch
  - CAN(TWAI)  ... x2ch

![](https://github.com/vcc-gnd/YD-ESP32-S3/raw/main/IMG/img11.jpg)