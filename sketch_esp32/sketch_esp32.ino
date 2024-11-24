/*
IoT_ESP32_PROJECT
COPYRIGHT: https://github.com/code1O
LAST_PROJECT: https://github.com/code1O/
*/

#include <iostream>
#include <WiFi.h>
#include "../Headers/Aliases.h"

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pkey, 6);
  WiFi.setTxpower(WIFI_POWER_19_5dBm);
}

void loop() {}