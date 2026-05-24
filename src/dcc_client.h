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
        static const size_t MAX_MSG_LEN = 64; // DCC-EX commands rarely exceed 30 chars
        char _rxBuffer[MAX_MSG_LEN];
        size_t _rxIndex = 0;
        void checkConnected();
        void parseDCCMessage(char* msg);
        void handleTurnoutMessage(char* params);
        void handleTrackManagerMessage(char* params);
        void handleUnknownMessage(const char* cmd, char* params);
};

#endif