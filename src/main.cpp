#include <DCCEXProtocol.h>
#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "TurnoutController.h"
#include "TocDelegate.h"
#include "TocIdMap.h"

WiFiClient client;
DCCEXProtocol dccexProtocol;
TocDelegate tocDelegate;

void setup_wifi()
{
    WiFi.setSleep(false);

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

    Serial.println("Connecting to the DCC-EX server...");
    if (!client.connect(dcc_ip, dcc_port))
    {
        Serial.println("connection failed");
        while (1)
            delay(1000);
    }
    Serial.println("Connected to the DCC-EX server");

    // Logging on Serial
    dccexProtocol.setLogStream(&Serial);

    // Pass the delegate instance to wiThrottleProtocol
    dccexProtocol.setDelegate(&tocDelegate);

    // Pass the communication to wiThrottleProtocol
    dccexProtocol.connect(&client);
    client.setNoDelay(true);
    dccexProtocol.enableHeartbeat();
    Serial.println("DCC-EX connected");

    dccexProtocol.requestServerVersion();

    dccexProtocol.getLists(false, true, false, false);
}

void setupTurnouts() {
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
