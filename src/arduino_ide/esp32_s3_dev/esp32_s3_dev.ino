#include "app_wifi.hpp"

void setup()
{
    Serial.begin(115200);
    Serial.println("UART初期化");

    app_wifi_init();
}

void loop()
{
    app_wifi_main();
}