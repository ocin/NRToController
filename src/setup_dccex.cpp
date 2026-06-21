#include "setup_dccex.h"

TocDelegate tocDelegate;
DCCEXProtocol dccexProtocol;

void setup_dccex()
{
    Serial.printf("Connecting to DCC-EX server %s:%d...\n", DCCEX_HOSTNAME, DCCEX_PORT);
    while (!client.connect(DCCEX_HOSTNAME, DCCEX_PORT))
    {
        Serial.printf("DCC-EX connection failed to %s:%d, retrying...\n", DCCEX_HOSTNAME, DCCEX_PORT);
        delay(1000);
    }

    dccexProtocol.setLogStream(&Serial);

    tocDelegate.initTocs();

    dccexProtocol.setDelegate(&tocDelegate);

    dccexProtocol.connect(&client);
    client.setNoDelay(true);
    dccexProtocol.enableHeartbeat();
    Serial.printf("Connected to the DCC-EX server %s:%d\n", DCCEX_HOSTNAME, DCCEX_PORT);
    networkLed.setColor(BLUE);

    dccexProtocol.requestServerVersion();

    dccexProtocol.getLists(false, true, false, false);
}

void check_dccex_connection()
{
    if (!client.connected())
    {
        Serial.println("DCC-EX server connection lost, attempting to reconnect...");
        networkLed.setColor(YELLOW);
        setup_dccex();
    }
}