#ifndef SETUP_DCCEX_H
#define SETUP_DCCEX_H

#include <Arduino.h>
#include <DCCEXProtocol.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include <RGBLed.h>
#include <colors.h>

#include "config.h"
#include "TocDelegate.h"
#include "setup_wifi.h"

extern TocDelegate tocDelegate;
extern DCCEXProtocol dccexProtocol;

void setup_dccex();
void check_dccex_connection();

#endif