/*
IoT_ESP32_PROJECT
COPYRIGHT: https://github.com/code1O
LAST_PROJECT: https://github.com/code1O/
*/

#include <iostream>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../Headers/Aliases.h"

WiFi espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.begin(esp32_STA_SSID, esp32_STA_KEY);
  WiFi.softAP(esp32_AP_SSID, esp32_AP_KEY, 6);
  WiFi.setTxpower(WIFI_POWER_19_5dBm);
}

void loop() {}