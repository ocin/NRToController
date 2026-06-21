#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <map>
#include <DCCEXProtocol.h>

#include <RGBLed.h>

#include "config.h"
#include "setup_wifi.h"
#include "setup_dccex.h"
#include "TurnoutController.h"
#include "TocDelegate.h"
#include "colors.h"

WiFiClient client;
DCCEXProtocol dccexProtocol;
TocDelegate tocDelegate;

RGBLed networkLed(25, 26, 27);

std::map<int, TurnoutController*> tocIdMap;

void setupTurnouts()
{
    tocIdMap.insert(std::make_pair(101, new TurnoutController(101, 12, 13)));
    tocIdMap.insert(std::make_pair(102, new TurnoutController(102, 16, 17)));
    tocIdMap.insert(std::make_pair(103, new TurnoutController(103, 18, 19)));
    tocIdMap.insert(std::make_pair(104, new TurnoutController(104, 21, 22)));
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32 Serial Initialized!");

    networkLed.setColor(RED);

    setup_wifi();
    setup_dccex();
    setupTurnouts();
}

void loop()
{
    dccexProtocol.check();

    if(!client.connected()) {
        Serial.println("DCC-EX server connection lost, attempting to reconnect...");
        networkLed.setColor(YELLOW);
        setup_dccex();
    }
}
