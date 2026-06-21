#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>

#include "TocConfig.h"
#include "setup_wifi.h"

WebServer webserver(80); // Start a standard web server on port 80

// Function to generate the HTML web interface page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:sans-serif;margin:20px;padding:20px;background:#f0f2f5;}";
  html += ".card{background:white;padding:20px;border-radius:8px;max-width:400px;margin:0 auto;box-shadow:0 4px 6px rgba(0,0,0,0.1);}";
  html += "input[type=text],input[type=number]{width:100%;padding:10px;margin:8px 0;box-sizing:border-box;}";
  html += "input[type=submit]{width:100%;background:#007bff;color:white;padding:12px;border:none;border-radius:4px;cursor:pointer;}";
  html += "</style><title>DCC-EX Config</title></head><body>";
  
  html += "<div class='card'><h2>Turnout Controller Config</h2>";
  html += "<form action='/save' method='POST'>";
  
  // Host and Port Fields
  html += "<label>DCC-EX Host/IP:</label>";
  html += "<input type='text' name='host' value='" + String(tocConfig.dcc_host) + "'>";
  html += "<label>DCC-EX Port:</label>";
  html += "<input type='text' name='port' value='" + String(tocConfig.dcc_port) + "'>";
  
  // 4 Turnout VPin Fields
  for (int i = 0; i < TocConfig::NUM_TURNOUTS; i++) {
    html += "<label>Turnout Controller " + String(i + 1) + " Turnout Id:</label>";
    html += "<input type='text' name='turnoutid_" + String(i) + "' value='" + String(tocConfig.turnoutid_mappings[i]) + "'>";
  }
  
  html += "<input type='submit' value='Save Settings'>";
  html += "</form></div></body></html>";
  
  webserver.send(200, "text/html", html);
}

// Function to handle incoming form submissions
void handleSave() {
  if (webserver.method() != HTTP_POST) {
    webserver.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  // Extract Host and Port from post parameters
  String new_host = webserver.arg("host");
  int new_port = webserver.arg("port").toInt();
  
  // Extract Turnout maps from post parameters
  int temp_vpins[TocConfig::NUM_TURNOUTS];
  for (int i = 0; i < TocConfig::NUM_TURNOUTS; i++) {
    String param_name = "vpin_" + String(i);
    temp_vpins[i] = webserver.arg(param_name).toInt();
  }

  // Save everything directly through your C++ configuration class
  tocConfig.save(new_host.c_str(), new_port, temp_vpins);

  // Send a confirmation page back to user browser screen
  String resp = "<html><body><h2>Settings Saved Successfully!</h2><p>Rebooting device...</p>";
  resp += "<script>setTimeout(function(){window.location.href='/';}, 3000);</script></body></html>";
  webserver.send(200, "text/html", resp);
  
  delay(1000);
  ESP.restart(); // Restart device to apply your modifications
}

void setup_webserver() {
  // 3. Display the Local IP Address assigned by your home network router
  Serial.println("\n-------------------------------------");
  Serial.print("Connected! Open this address in your browser: http://");
  Serial.println(WiFi.localIP());
  Serial.println("-------------------------------------\n");

  // 4. Bind the web webserver pathways 
  webserver.on("/", handleRoot);
  webserver.on("/save", handleSave);
  webserver.begin(); // Start the background local web service
}
