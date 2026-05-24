#ifndef DCC_CLIENT_H
#define DCC_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>

class DCCClient : public WiFiClient {
    public:
        DCCClient();
        void checkMessages();
    private:
        String _rxBuffer = "";
        void checkConnected();
        void parseDCCMessage(String msg);
        void handleTurnoutMessage(String msg);
        void handleTrackManagerMessage(String msg);
        void handleUnknownMessage(String msg, String params);
};

#endif