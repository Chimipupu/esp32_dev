# ESP32 評価プログラム by ちみ
ESP32 Firmeware C++ Develop by Chimi\
ESP32 C++ F/W個人開発リポジトリ🥳

## ✅実装した機能

- CPU x2コア
  - ✅Core0 ... ドライバCPU
    - ドライバ関連
  - ✅Core1 ... アプリCPU
    - ✅WiFiアプリ
      - ✅AP
        - HTTP : HTMLでSTAのSSIDとパスワード入力受け
      - ✅STA
        - FTP : FATFSでフラッシュにファイルをR/W
        - NTP : RTCと同期
        - HTTP : HTMLに各種データのI/O
    - ✅メインループ
      - ✅DeepSleep

## 🛠️TBD
- 🛠️FreeRTOS ... CPU2コアそれぞれ
- 🛠️割込み（IRQ） ... ボタンやPIOからIRQがほしい
- 🛠️タイマ⏰ ... インターバル⏰とワンショットタイマ⏰ほしい
- 🛠️アプリ追加 ... I2C、SPI、PIO、スリープ機能
- 🛠️WDT🐶 ... 最後にWDTで番犬わんわんさせる