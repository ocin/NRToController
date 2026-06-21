#include "setup_dccex.h"

TocDelegate tocDelegate;
DCCEXProtocol dccexProtocol;

void setup_dccex()
{
    char dcc_host[40];
    int dcc_port = tocConfig.dcc_port;
    strncpy(dcc_host, tocConfig.dcc_host, sizeof(dcc_host));

#ifdef DCCEX_HOSTNAME
    dcc_host = DCCEX_HOSTNAME;
    dcc_port = DCCEX_PORT;
#endif

    Serial.printf("Connecting to DCC-EX server %s:%d...\n", dcc_host, dcc_port);
    while (!client.connect(dcc_host, dcc_port))
    {
        Serial.printf("DCC-EX connection failed to %s:%d, retrying...\n", dcc_host, dcc_port);
        delay(1000);
    }

    dccexProtocol.setLogStream(&Serial);

    tocDelegate.initTocs();

    dccexProtocol.setDelegate(&tocDelegate);

    dccexProtocol.connect(&client);
    client.setNoDelay(true);
    dccexProtocol.enableHeartbeat();
    Serial.printf("Connected to the DCC-EX server %s:%d\n", dcc_host, dcc_port);
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