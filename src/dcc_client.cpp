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

// Params string here contains everything after "H ", like "101 1"
void DCCClient::handleTurnoutMessage(String params)
{
    params.trim();
    if (params.length() == 0)
    {
        Serial.println("[Turnout Error] params string is empty.");
        return;
    }

    // Find the space separating the turnout ID and the state
    int spaceIndex = params.indexOf(' ');
    if (spaceIndex == -1)
    {
        Serial.println("[Turnout Error] Malformed parameters: missing state value.");
        return;
    }

    // Extract id and state from the params string
    int turnoutID = params.substring(0, spaceIndex).toInt();
    int state = params.substring(spaceIndex + 1).toInt();

    if(_turnoutControllers->find(turnoutID) == _turnoutControllers->end()) {
        Serial.printf("[Turnout Error] No turnout controller found for ID: %d\n", turnoutID);
        return;
    }

    if(_turnoutControllers->at(turnoutID)->getState() != state) {
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

void DCCClient::handleTrackManagerMessage(String params)
{
    Serial.print("[Track Manager Event] Status: ");
    Serial.println(params);
}

void DCCClient::handleUnknownMessage(String cmd, String params)
{
    Serial.printf("[Unhandled DCC Msg] Cmd: %s | Params: %s\n", cmd.c_str(), params.c_str());
}

void DCCClient::parseDCCMessage(String msg)
{
    // Print the raw message to VS Code Serial Monitor for debugging
    Serial.print("Received from DCC-EX: <");
    Serial.print(msg);
    Serial.println(">");

    msg.trim(); // Clean up trailing spaces or newlines
    if (msg.length() == 0)
        return;

    String cmd = "";
    String params = "";

    // Find the first space separator
    int firstSpace = msg.indexOf(' ');

    if (firstSpace != -1)
    {
        // Space found: split into command key and subsequent parameters
        cmd = msg.substring(0, firstSpace);
        params = msg.substring(firstSpace + 1);
    }
    else
    {
        // No space found: the entire message is the command (e.g., "p1")
        cmd = msg;
    }

    // Route based on the extracted multi-character command token
    if (cmd == "H")
    {
        handleTurnoutMessage(params);
    }
    else if (cmd == "=")
    { // Works for multi-character variations
        handleTrackManagerMessage(params);
    }
    else
    {
        handleUnknownMessage(cmd, params);
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

    // Process characters only if they are actively waiting in the Wi-Fi buffer
    while (available())
    {
        char c = read();

        if (c == '<')
        {
            // Clear the buffer to prepare for a brand new message
            _rxBuffer = "";
        }
        else if (c == '>')
        {
            // We found the end! Only now do we pass it to the parser
            if (_rxBuffer.length() > 0)
            {
                parseDCCMessage(_rxBuffer);
                _rxBuffer = ""; // Reset for the next packet
            }
        }
        else
        {
            // Content character: safely append it to our ongoing buffer
            _rxBuffer += c;
        }
    }
}