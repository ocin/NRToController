#include <Arduino.h>

#include "common.h"
#include "turnoutController.h"

TurnoutController::TurnoutController(int id, int closedPort, int thrownPort, int state) {
    _id = id;

    _closedPort = closedPort;
    _thrownPort = thrownPort;

    pinMode(_closedPort, OUTPUT);
    pinMode(_thrownPort, OUTPUT);

    _state = state;

    Serial.printf("[Turnout Init] Initialize turnout controller \"%d\"\n", _id);

    if(_state == TU_CLOSE) {
        setClose();
    } else if (_state == TU_THROWN) {
        setThrown();
    }
}

void TurnoutController::setClose() {
    _state = TU_CLOSE;

    Serial.printf("[Turnout Update] Update turnout controller \"%d\" to CLOSE\n", _id);

    digitalWrite(_closedPort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_closedPort, LOW);
}

void TurnoutController::setThrown() {
    _state = TU_THROWN;

    Serial.printf("[Turnout Update] Update turnout controller \"%d\" to THROWN\n", _id);

    digitalWrite(_thrownPort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_thrownPort, LOW);
}

bool TurnoutController::getState() {
    return(_state);
}