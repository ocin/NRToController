#include <DCCEXProtocol.h>
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include "config.h"
#include "TurnoutController.h"
#include "TocDelegate.h"
#include "TocIdMap.h"

WiFiClient client;
DCCEXProtocol dccexProtocol;
TocDelegate tocDelegate;

void setup_wifi()
{
    esp_wifi_set_ps(WIFI_PS_NONE);

    esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11N);
    esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT20);

    Serial.println("\n--- Connecting to Wi-Fi ---");
    // Start the Wi-Fi connection process
    WiFi.begin(ssid, password);

    // Wait until the ESP32 successfully connects
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("."); // Visual loading indicator
    }

    // Connection successful
    Serial.println("\nWi-Fi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void setup_dccex()
{
    int tolistretries = 0;

    Serial.printf("Connecting to DCC-EX server %s:%d...\n", dcc_ip, dcc_port);
    while (!client.connect(dcc_ip, dcc_port))
    {
        Serial.printf("DCC-EX connection failed to %s:%d, retrying...\n", dcc_ip, dcc_port);
        delay(1000);
    }

    dccexProtocol.setLogStream(&Serial);

    dccexProtocol.setDelegate(&tocDelegate);

    dccexProtocol.connect(&client);
    client.setNoDelay(true);
    dccexProtocol.enableHeartbeat();
    Serial.printf("Connected to the DCC-EX server %s:%d\n", dcc_ip, dcc_port);

    dccexProtocol.requestServerVersion();

    dccexProtocol.getLists(false, true, false, false);
}

void setupTurnouts()
{
    TocIdMapList[0] = {101, new TurnoutController(101, 12, 13)};
    TocIdMapList[1] = {102, new TurnoutController(102, 16, 17)};
    TocIdMapList[2] = {103, new TurnoutController(103, 18, 19)};
    TocIdMapList[3] = {104, new TurnoutController(104, 21, 22)};
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32 Serial Initialized!");

    setup_wifi();
    setup_dccex();
    setupTurnouts();
}

void loop()
{
    dccexProtocol.check();
}
