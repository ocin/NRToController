#include <Arduino.h>

#include "common.h"
#include "TurnoutController.h"

TurnoutController::TurnoutController(int id, int closedPort, int thrownPort) {
    _id = id;

    _closedPort = closedPort;
    _thrownPort = thrownPort;

    pinMode(_closedPort, OUTPUT);
    pinMode(_thrownPort, OUTPUT);

    Serial.printf("[Turnout Init] Initialize turnout controller \"%d\"\n", _id);
}

void TurnoutController::setClose() {
#ifdef DEBUG
    Serial.printf("[Turnout Update] Update turnout controller \"%d\" to CLOSE\n", _id);
#endif

    digitalWrite(_closedPort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_closedPort, LOW);
}

void TurnoutController::setThrown() {
#ifdef DEBUG
    Serial.printf("[Turnout Update] Update turnout controller \"%d\" to THROWN\n", _id);
#endif

    digitalWrite(_thrownPort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_thrownPort, LOW);
}
