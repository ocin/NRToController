#include <Arduino.h>
#include <map>

#include "common.h"
#include "dcc_client.h"
#include "turnoutController.h"

DCCClient *dcc_client;

std::map<int, TurnoutController *> turnoutControllers;

void setup() {
  Serial.begin(115200);
  delay(1000); 
  Serial.println("ESP32 Serial Initialized!");

  turnoutControllers.insert(std::make_pair(101, new TurnoutController(101, 12, 13, TU_CLOSE)));
  turnoutControllers.insert(std::make_pair(102, new TurnoutController(102, 16, 17, TU_THROWN)));
  turnoutControllers.insert(std::make_pair(103, new TurnoutController(103, 18, 19, TU_THROWN)));
  turnoutControllers.insert(std::make_pair(104, new TurnoutController(104, 21, 22, TU_CLOSE)));

  dcc_client = new DCCClient(&turnoutControllers);
}

void loop() {
  dcc_client->checkMessages();
}
