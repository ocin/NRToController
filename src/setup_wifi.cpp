#include "setup_dccex.h"
#include "setup_webserver.h"

RGBLed networkLed(25, 26, 27);
WiFiClient client;

TocConfig tocConfig; // Create instance of your configuration class
bool shouldSaveConfig = false;

void saveConfigCallback() {
  shouldSaveConfig = true;
}

void setup_vm() {
      // 1. Initialize configuration and load saved variables
  tocConfig.begin();

  WiFiManager wm;
  wm.setSaveConfigCallback(saveConfigCallback);

  // 2. Setup standard host and port HTML input elements
  char port_str[6];
  sprintf(port_str, "%d", tocConfig.dcc_port);
  
  WiFiManagerParameter custom_dcc_host("host", "DCC-EX Host/IP", tocConfig.dcc_host, 40);
  WiFiManagerParameter custom_dcc_port("port", "DCC-EX Port", port_str, 6);
  wm.addParameter(&custom_dcc_host);
  wm.addParameter(&custom_dcc_port);

  // 3. Create dynamic storage strings for UI generation
  char id_buffers[TocConfig::NUM_TURNOUTS][10];
  char label_buffers[TocConfig::NUM_TURNOUTS][40];
  char val_buffers[TocConfig::NUM_TURNOUTS][10];
  WiFiManagerParameter* custom_turnoutid[TocConfig::NUM_TURNOUTS];

  // 4. Generate the 4 UI fields (Turnout 1 to 4)
  for (int i = 0; i < TocConfig::NUM_TURNOUTS; i++) {
    sprintf(id_buffers[i], "turnoutid_%d", i);
    sprintf(label_buffers[i], "Turnout Controller %d Turnout ID", i + 1);
    sprintf(val_buffers[i], "%d", tocConfig.turnoutid_mappings[i]);
    
    custom_turnoutid[i] = new WiFiManagerParameter(id_buffers[i], label_buffers[i], val_buffers[i], 6);
    wm.addParameter(custom_turnoutid[i]);
  }

  // 5. Run the interface portal
  if (!wm.autoConnect("Toc_Manager")) {
    Serial.println("Portal timed out. Restarting...");
    delay(3000);
    ESP.restart();
  }

  // 6. If user submitted changes, save them using the class method
  if (shouldSaveConfig) {
    int temp_turnoutid[TocConfig::NUM_TURNOUTS];
    for (int i = 0; i < TocConfig::NUM_TURNOUTS; i++) {
        temp_turnoutid[i] = atoi(custom_turnoutid[i]->getValue());
    }
    
    // Pass user data directly into the class handler method
    tocConfig.save(
        custom_dcc_host.getValue(), 
        atoi(custom_dcc_port.getValue()), 
        temp_turnoutid
    );
  }

  // Clean up UI allocation heap memory space
  for (int i = 0; i < TocConfig::NUM_TURNOUTS; i++) {
    delete custom_turnoutid[i];
  }

  // Print summary verifying successful config extraction
  tocConfig.dumpToSerial();
}

void setup_wifi()
{
    networkLed.setColor(RED);

    Serial.println("\n--- Connecting to Wi-Fi ---");
    // Start the Wi-Fi connection process
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Wifi configuration for better performance and reliability
    esp_wifi_set_ps(WIFI_PS_NONE);
    esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11N);
    esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT20);

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
    networkLed.setColor(YELLOW);

    setup_vm();
    setup_webserver();
}
