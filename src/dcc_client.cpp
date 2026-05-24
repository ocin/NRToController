#include <map>

#include "config.h"
#include "common.h"
#include "dcc_client.h"
#include "turnoutController.h"

DCCClient::DCCClient(std::map<int, TurnoutController *> *turnoutControllers) : WiFiClient()
{
    _turnoutControllers = turnoutControllers;

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

    // Connect to DCC-EX Command Station TCP Server
    Serial.printf("Attempting link to DCC-EX at %s:%d...\n", dcc_ip, dcc_port);
    if (connect(dcc_ip, dcc_port))
    {
        setNoDelay(true); 
        Serial.println("[Connected to DCC-EX Command Station!]");

        this->println("<=>");
        Serial.println("Sent Command: <=> (Request Track Manager Configuration)");
    }
    else
    {
        Serial.println("[Connection to DCC-EX failed! Retrying in loop...]");
    }
}

void DCCClient::handleTurnoutMessage(char* params)
{
    // Check if the pointer is null or points to an empty string
    if (params == NULL || *params == '\0')
    {
        Serial.println("[Turnout Error] params string is empty.");
        return;
    }

    int turnoutID = -1;
    int state = -1;

    // sscanf scans the string for two integers separated by a space.
    // It returns the number of successfully matched arguments.
    if (sscanf(params, "%d %d", &turnoutID, &state) != 2)
    {
        Serial.println("[Turnout Error] Malformed parameters: missing ID or state value.");
        return;
    }

    // Use the parsed integers directly with your map
    if (_turnoutControllers->find(turnoutID) == _turnoutControllers->end()) {
        Serial.printf("[Turnout Error] No turnout controller found for ID: %d\n", turnoutID);
        return;
    }

    if (_turnoutControllers->at(turnoutID)->getState() != state) {
        if (state == TU_CLOSE)
        {
            _turnoutControllers->at(turnoutID)->setClose();
        }
        else if (state == TU_THROWN)
        {
            _turnoutControllers->at(turnoutID)->setThrown();
        }
        else
        {
            Serial.printf("[Turnout Error] Invalid state value for turnout ID %d: %d\n", turnoutID, state);
            return;
        }
    }
}

void DCCClient::handleTrackManagerMessage(char* params)
{
    Serial.print("[Track Manager Event] Status: ");
    Serial.println(params ? params : "NONE");
}

void DCCClient::handleUnknownMessage(const char* cmd, char* params)
{
    // %s natively accepts raw character pointers (char*)
#ifdef DEBUG
    Serial.printf("[Unhandled DCC Msg] Cmd: %s | Params: %s\n", 
                  cmd ? cmd : "NONE", 
                  params ? params : "NONE");
#endif
}

void DCCClient::parseDCCMessage(char* msg)
{
    // 1. Debug Print using the raw character array (No String allocation)
#ifdef DEBUG
    Serial.print("Received from DCC-EX: <");
    Serial.print(msg);
    Serial.println(">");
#endif

    // 2. Trim leading spaces manually if necessary (Usually not needed if using the previous fixed buffer)
    while (*msg == ' ') {
        msg++;
    }

    size_t len = strlen(msg);
    if (len == 0) return;

    // Trim trailing spaces in place
    while (len > 0 && msg[len - 1] == ' ') {
        msg[len - 1] = '\0';
        len--;
    }

    char* cmd = msg;      // The command starts at the beginning of the string
    char* params = NULL;  // Initialize parameters as empty/NULL

    // 3. Find the first space separator using a fast pointer lookup
    char* firstSpace = strchr(msg, ' ');

    if (firstSpace != NULL)
    {
        *firstSpace = '\0';        // Split string in place by replacing space with null terminator
        params = firstSpace + 1;   // Parameters start right after the old space character
        
        // Trim any extra leading spaces in the parameters block
        while (*params == ' ') {
            params++;
        }
    }

    // 4. Route commands using fast pointer comparisons (strcmp)
    if (strcmp(cmd, "H") == 0)
    {
        // If params is NULL, pass an empty string safely
        handleTurnoutMessage(params ? params : (char*)"");
    }
    else if (strcmp(cmd, "=") == 0)
    { 
        handleTrackManagerMessage(params ? params : (char*)"");
    }
    else
    {
        handleUnknownMessage(cmd, params ? params : (char*)"");
    }
}

void DCCClient::checkConnected()
{
    if (!connected())
    {
        Serial.println("\n[DCC-EX Disconnected. Reconnecting...]");
        stop();
        delay(3000);
        if (this->connect(dcc_ip, dcc_port))
        {
            Serial.println("[Reconnected to DCC-EX!]");
            this->println("<=>");
            Serial.println("Sent Command: <=> (Request Track Manager Configuration)");
        }
    }
}

void DCCClient::checkMessages()
{
    checkConnected();

    // Aggressively pull all waiting data out of the Wi-Fi chip buffer
    while (available() > 0)
    {
        char c = read();

        if (c == '<')
        {
            // Reset index instantly without touching heap memory
            _rxIndex = 0; 
            _rxBuffer[_rxIndex] = '\0';
        }
        else if (c == '>')
        {
            // We found the end! Cap the string array and parse
            if (_rxIndex > 0)
            {
                _rxBuffer[_rxIndex] = '\0'; // Properly terminate the C-string
                parseDCCMessage(_rxBuffer);  // Update your parser to accept (char*)
                _rxIndex = 0;               // Reset for next packet
            }
        }
        else
        {
            // Filter out carriage returns, newlines, or spaces at the start
            if (c != '\r' && c != '\n') 
            {
                // Safely append while guaranteeing we never overflow the array bounds
                if (_rxIndex < MAX_MSG_LEN - 1)
                {
                    _rxBuffer[_rxIndex++] = c;
                }
                else
                {
                    // Buffer safety net: packet was too long/corrupt, dump it
                    _rxIndex = 0;
                }
            }
        }
    }
}
