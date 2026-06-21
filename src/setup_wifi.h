#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <WebServer.h>
#include <WiFiManager.h>

#include <RGBLed.h>
#include <colors.h>

#include "config.h"
#include "TocConfig.h"
#include "TocLog.h"

extern RGBLed networkLed;
extern WiFiClient client;
extern TocConfig tocConfig;

void setup_wifi();