#include "setup_dccex.h"

extern TocDelegate tocDelegate;
extern RGBLed networkLed;
extern DCCEXProtocol dccexProtocol;
extern WiFiClient client;

void setup_dccex()
{
    Serial.printf("Connecting to DCC-EX server %s:%d...\n", DCCEX_HOSTNAME, DCCEX_PORT);
    while (!client.connect(DCCEX_HOSTNAME, DCCEX_PORT))
    {
        Serial.printf("DCC-EX connection failed to %s:%d, retrying...\n", DCCEX_HOSTNAME, DCCEX_PORT);
        delay(1000);
    }

    dccexProtocol.setLogStream(&Serial);

    dccexProtocol.setDelegate(&tocDelegate);

    dccexProtocol.connect(&client);
    client.setNoDelay(true);
    dccexProtocol.enableHeartbeat();
    Serial.printf("Connected to the DCC-EX server %s:%d\n", DCCEX_HOSTNAME, DCCEX_PORT);
    networkLed.setColor(BLUE);

    dccexProtocol.requestServerVersion();

    dccexProtocol.getLists(false, true, false, false);
}
