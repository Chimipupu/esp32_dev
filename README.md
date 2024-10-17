# ESP32 評価プログラム by ちみ
ESP32 Firmeware C++ Develop by Chimi\
C++でのESP32 F/W 個人開発リポジトリ🥳

## ✅実装した機能
- CPU x2コア
  - ✅Core0 ... ドライバCPU
    - ドライバ関連
  - ✅Core1 ... アプリCPU
    - ✅WiFiアプリ
      - ✅AP ... HTMLサーバでSTAのSSID、パスワードを取得
      - ✅STA ... NTPから時刻同期、ESP32のRTCとNTPの時刻を同期

## 🛠️TBD
- 🛠️DeepSleep ... DeepSleepでバッテリー消費を押させたい
- 🛠️FreeRTOS ... CPU2コアそれぞれ
- 🛠️WiFi ... FTPの追加
- 🛠️割込み（IRQ） ... ボタンやPIOからIRQがほしい
- 🛠️タイマ⏰ ... インターバル⏰とワンショットタイマ⏰ほしい
- 🛠️アプリ追加 ... I2C、SPI、PIO、スリープ機能
- 🛠️WDT🐶 ... 最後にWDTで番犬わんわんさせる