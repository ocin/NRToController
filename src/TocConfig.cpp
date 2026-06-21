#include "TocConfig.h"

// Constructor sets default values
TocConfig::TocConfig()
{
    strncpy(dcc_host, "dcc-ex.local", sizeof(dcc_host));
    dcc_port = 2560;

    for (int i = 0; i < NUM_TURNOUTS; i++)
    {
        turnoutid_mappings[i] = 100 + i;
    }
}

// Initializes storage engines and loads saved data
void TocConfig::begin()
{
    // 1. Open in read-write mode (false) first, so it automatically 
    // creates the namespace "dcc-config" if it doesn't exist yet.
    Preferences preferences;
    preferences.begin("dcc-config", false); 
    
    String savedHost = preferences.getString("dcc_host", "dcc-ex.local");
    savedHost.toCharArray(dcc_host, sizeof(dcc_host));
    dcc_port = preferences.getInt("dcc_port", 2560);
    preferences.end();

    // 2. Initialize LittleFS with formatOnFail set to true
    if (!LittleFS.begin(true)) {
        Serial.println("[Config] LittleFS Mount Failed entirely.");
        return;
    }

    // Explicitly check existence before calling open("r") 
    // to stop the internal virtual file system (vfs) engine from throwing an error.
    if (LittleFS.exists("/turnouts.json")) {
        File configFile = LittleFS.open("/turnouts.json", "r");
        if (configFile) {
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, configFile);
            if (!error) {
                for (int i = 0; i < NUM_TURNOUTS; i++) {
                    turnoutid_mappings[i] = doc["turnoutid"][i] | turnoutid_mappings[i];
                }
                Serial.println("[Config] Settings successfully loaded from LittleFS.");
            }
            configFile.close();
        }
    } else {
        // First boot scenario: Create the file immediately using your defaults
        Serial.println("[Config] turnouts.json not found. Initializing default file...");
        
        // Pass dummy parameters matching current in-memory configurations to initialize the file
        save(dcc_host, dcc_port, turnoutid_mappings); 
    }
}

// Saves all current in-memory parameters to flash storage
void TocConfig::save(const char *new_host, int new_port, int const *new_turnoutid)
{
    // Update in-memory variables
    strncpy(dcc_host, new_host, sizeof(dcc_host));
    dcc_port = new_port;
    for (int i = 0; i < NUM_TURNOUTS; i++)
    {
        turnoutid_mappings[i] = new_turnoutid[i];
    }

    // 1. Save Host and Port to Preferences
    Preferences preferences;
    preferences.begin("dcc-config", false); // Open in write mode
    preferences.putString("dcc_host", String(dcc_host));
    preferences.putInt("dcc_port", dcc_port);
    preferences.end();

    // 2. Save Turnout Array to LittleFS JSON
    JsonDocument doc;
    JsonArray turnoutidNode = doc["turnoutid"].to<JsonArray>();
    for (int i = 0; i < NUM_TURNOUTS; i++)
    {
        turnoutidNode.add(turnoutid_mappings[i]);
    }

    File configFile = LittleFS.open("/turnouts.json", "w");
    if (configFile)
    {
        serializeJson(doc, configFile);
        configFile.close();
        Serial.println("[Config] Settings successfully saved to Flash.");
    }
    else
    {
        Serial.println("[Config] Error opening LittleFS file for writing.");
    }
}

// Debug print utility to view settings via Serial Monitor
void TocConfig::dumpToSerial()
{
    Serial.printf("\n--- Toc Configuration Profile ---\n");
    Serial.printf("DCC-EX Host: %s\n", dcc_host);
    Serial.printf("DCC-EX Port: %d\n", dcc_port);
    for (int i = 0; i < NUM_TURNOUTS; i++)
    {
        Serial.printf("Turnout Controller %d -> Turnout Id: %d\n", i + 1, turnoutid_mappings[i]);
    }
    Serial.println("------------------------------------");
}

/**
 * Gets the DCC Turnout ID based on the controller turnout number.
 * @param tocnum The turnout label number (1 to 4)
 * @return The assigned Turnout ID, or 0 if out of bounds / unconfigured.
 */
int TocConfig::getTurnoutId(int tocnum) {
    // Convert 1-based human label (1-4) to 0-based array index (0-3)
    int arrayIndex = tocnum - 1;

    // Boundary check: Ensure the requested number fits our hardcoded array limit
    if (arrayIndex < 0 || arrayIndex >= NUM_TURNOUTS) {
        Serial.printf("[Config] Warning: Requested turnout number %d is out of bounds!\n", tocnum);
        return 0; 
    }

    int turnoutId = turnoutid_mappings[arrayIndex];

    // Optional: Return 0 if the value is explicitly unconfigured or negative
    if (turnoutId <= 0) {
        return 0;
    }

    return turnoutId;
}