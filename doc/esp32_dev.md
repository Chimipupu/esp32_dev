# 開発環境
- 📍[Arduino IDE v2.33🔗](https://github.com/arduino/arduino-ide/releases/tag/2.3.3)
  - [Arduino core for the esp32 v3.0.7🔗](https://github.com/espressif/arduino-esp32/releases/tag/3.0.7)
- 📍基板
  - [VCC-GND YD-ESP32-S3🔗](https://github.com/vcc-gnd/YD-ESP32-S3)
  - [M5Stack Atom S3 Lite🔗](https://docs.m5stack.com/en/core/AtomS3%20Lite)
****

## Arduino IDE
[Arduino core for the esp32🔗](https://github.com/espressif/arduino-esp32)

👇をArduino IDEの`基本設定`->`追加のボードマネージャのURL`に追記

```shell
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

****

### ビルド構成
👇AtomS3 Liteのビルド構成

![AtomS3 Liteのビルド構成](/doc/atoms3lite_build_info.png)

****

## 📍基板
  - [VCC-GND YD-ESP32-S3🔗](https://github.com/vcc-gnd/YD-ESP32-S3)
  - [M5Stack Atom S3 Lite🔗](https://docs.m5stack.com/en/core/AtomS3%20Lite)

### 📍VCC-GND YD-ESP32-S3

https://github.com/vcc-gnd/YD-ESP32-S3

- 機能
  - WS2812(RGBLED) ... GPIO 48
  - CH343 ... UART to USB
  - MCU ... ESP32-S3FN16R8
    - Flash : 16 MB
    - PSRAM : 8 MB

![](https://github.com/vcc-gnd/YD-ESP32-S3/raw/main/IMG/img11.jpg)

### 📍M5Stack Atom S3 Lite
https://docs.m5stack.com/en/core/AtomS3%20Lite

- WS2812(RGBLED) ... GPIO 35
- ボタン ... GPIO 41
- 赤外線LED ... GPIO 4
- MCU ... ESP32-S3FN8
  - Flash : 8 MB
  - PSRAM : N/A

![](/doc/atoms3lite_pin.png)

# 📍評価対象
凡例(✅:対象,❌:対象外)

- ✅ESP32-S3
  - ✅SPIFS,FATFS
  - ✅PSRAM
  - ✅USB
  - ✅WiFi
  - ✅Bluetooth
  - ❌Ethernet
  - ✅GPIO
  - ✅PWM
  - ✅RMT(赤外線)
  - ✅UART
  - ✅SPI
  - ✅I2C
  - ❌I2S
  - ❌CAN(TWAI)
  - ✅ADC
  - ❌DAC
  - ❌LCD I/F
  - ❌Camera I/F
  - ✅Timer
  - ✅RTC
  - ✅WDT
  - ✅DeepSleep
  - 暗号化
    - ✅TRNG（真性乱数生成器）
    - ❌RSA
    - ✅AES
    - ✅SHA
    - ❌HMAC
    - ❌Digital signature