#ifndef SETUP_WIFI_H
#define SETUP_WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <WebServer.h>
#include <WiFiManager.h>

#include <RGBLed.h>
#include <colors.h>

#include "config.h"
#include "TocConfig.h"

extern RGBLed networkLed;
extern WiFiClient client;
extern TocConfig tocConfig;

void setup_wifi();

#endif