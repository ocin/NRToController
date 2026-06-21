#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>

#include "TocConfig.h"
#include "TocLog.h"
#include "setup_wifi.h"

extern WebServer webserver;
extern std::vector<String> webLogBuffer;

// Function to generate the HTML web interface page
void handleRoot();
void handleSave();
void setup_webserver();