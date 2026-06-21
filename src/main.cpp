#include <Arduino.h>
#include <DCCEXProtocol.h>

#include <RGBLed.h>

#include "config.h"
#include "setup_wifi.h"
#include "setup_dccex.h"
#include "setup_webserver.h"
#include "TocLog.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);
    toclog.println("ESP32 toclog Initialized!");

    setup_wifi();
    setup_dccex();
}

void loop()
{
    dccexProtocol.check();

    check_dccex_connection();

    webserver.handleClient();
}
