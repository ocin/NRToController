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

bool turnoutsSetup = false;

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
    Serial.printf("Connecting to DCC-EX server %s:%d...\n", dcc_ip, dcc_port);
    while (!client.connect(dcc_ip, dcc_port))
    {
        Serial.printf("DCC-EX connection failed to %s:%d, retrying...\n", dcc_ip, dcc_port);
        delay(1000);
    }
    client.setNoDelay(true);

    // Logging on Serial
    dccexProtocol.setLogStream(&Serial);

    // Pass the delegate instance to wiThrottleProtocol
    dccexProtocol.setDelegate(&tocDelegate);

    // Pass the communication to wiThrottleProtocol
    dccexProtocol.connect(&client);
    dccexProtocol.enableHeartbeat();
    Serial.printf("Connected to the DCC-EX server %s:%d\n", dcc_ip, dcc_port);
}

void setupTurnouts()
{
    dccexProtocol.getLists(false, true, false, false);

    TocIdMapList[0] = {101, new TurnoutController(101, 12, 13)};
    TocIdMapList[1] = {102, new TurnoutController(102, 16, 17)};
    TocIdMapList[2] = {103, new TurnoutController(103, 18, 19)};
    TocIdMapList[3] = {104, new TurnoutController(104, 21, 22)};

    for (Turnout *turnout = dccexProtocol.turnouts->getFirst(); turnout; turnout = turnout->getNext())
    {
        int turnoutId = turnout->getId();
        const char *name = turnout->getName();
        bool state = turnout->getThrown();
#ifdef DEBUG
        Serial.print(turnoutId);
        Serial.print(" name: ");
        Serial.print(name);
        Serial.print(" state: ");
        Serial.println(state);
#endif
        if (state)
        {
            getTurnoutControllerById(turnoutId)->setThrown();
        }
        else
        {
            getTurnoutControllerById(turnoutId)->setClose();
        }
    }
    dccexProtocol.clearAllLists();
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

    if (!turnoutsSetup && dccexProtocol.receivedLists())
    {
        setupTurnouts();
        turnoutsSetup = true;
    }
}
