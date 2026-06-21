#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include "TocConfig.h"

extern WebServer webserver;

// Function to generate the HTML web interface page
void handleRoot();
void handleSave();
void setup_webserver();