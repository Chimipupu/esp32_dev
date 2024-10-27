# ESP32-S3 FreeRTOS F/W by ちみ
ESP32-S3 FreeRTOS F/W by ちみ🥳

## ✅実装した機能
- ✅Core0 ... ドライバコアCPU
  - ✅FreeRTOS
    - ✅loopTask
      - ドライバ関連

- ✅Core1 ... アプリコアCPU
  - ✅割込み
    - ✅ボタンIRQ
      - `ボタン単押し`:ボタン一回押しを検知
      - `ボタン複数押し`:短時間に複数回ボタン押しを検知
      - `ボタン長押し`:ボタン長押しを検知（1000ms = 1s）
      - `ボタン超長押し`:ボタン長押しを検知（3000ms = 2s）
    - ✅FreeRTOS
      - ✅loopTask
        - ✅DeepSleep : TBD
        - TBD
      - ✅WiFiタスク
          - ✅AP
            - ✅HTTP : HTMLでSTAのSSIDとパスワード入力受け
          - ✅STA
            - ✅FTP : FATFSでフラッシュにファイルをR/W
            - ✅NTP : RTCと同期
            - ✅HTTP : HTMLに各種データのI/O
          - ✅RGB LED : WiFiの状態に応じて色表示
            - ✅赤 : オフライン状態（WiFi未接続）
            - ✅緑 : オンライン状態（STAモード）
            - ✅青 : APモード
            - ✅白 : TODO(ESP-NOWで起動中)
            - ✅紫 : TODO(エラー状態)

## 🛠️TBD
- 🛠️タイマ⏰ ... インターバル⏰とワンショットタイマ⏰ほしい
- 🛠️アプリ追加 ... I2C、SPI、PIO、スリープ機能
- 🛠️ESP NOW ... デバイス同士で通信するとき
- 🛠️WDT🐶 ... 最後にWDTで番犬わんわんさせる
- 🛠️PSRAM ... MBの大容量なヒープ領域

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