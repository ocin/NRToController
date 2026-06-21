#ifndef SETUP_WIFI_H
#define SETUP_WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include <RGBLed.h>
#include <colors.h>

#include "config.h"

extern RGBLed networkLed;
extern WiFiClient client;

void setup_wifi();

#endif