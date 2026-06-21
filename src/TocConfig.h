#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

class TocConfig
{
public:
    char dcc_host[40];
    int dcc_port;

    static const int NUM_TURNOUTS = 4;
    int turnoutid_mappings[NUM_TURNOUTS];
    
    TocConfig();
    void begin();
    void save(const char *new_host, int new_port, int const *new_turnoutid);
    void dumpToSerial();
    int getTurnoutId(int tocnum);
};
