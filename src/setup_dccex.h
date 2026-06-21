#pragma once

#include <Arduino.h>
#include <DCCEXProtocol.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include <RGBLed.h>
#include <colors.h>

#include "config.h"
#include "TocDelegate.h"
#include "TocConfig.h"
#include "TocLog.h"
#include "setup_wifi.h"

extern DCCEXProtocol dccexProtocol;

void setup_dccex();
void check_dccex_connection();
