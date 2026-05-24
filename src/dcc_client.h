#ifndef DCC_CLIENT_H
#define DCC_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>

#include "turnoutController.h"

class DCCClient : public WiFiClient {
    public:
        DCCClient(std::map<int, TurnoutController *> *turnoutControllers);
        void checkMessages();
    private:
        std::map<int, TurnoutController *> *_turnoutControllers;
        String _rxBuffer = "";
        void checkConnected();
        void parseDCCMessage(String msg);
        void handleTurnoutMessage(String msg);
        void handleTrackManagerMessage(String msg);
        void handleUnknownMessage(String msg, String params);
};

#endif