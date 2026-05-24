#include <Arduino.h>

#include "dcc_client.h"

DCCClient *dcc_client;

void setup() {
  Serial.begin(115200);
  delay(1000); 
  Serial.println("ESP32 Serial Initialized!");

  dcc_client = new DCCClient();
}

void loop() {
  dcc_client->checkMessages();
}
