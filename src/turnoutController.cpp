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

    Serial.println((String)"Init turnout controller \"" + _id + "\"");

    if(_state == TU_CLOSE) {
        setClose();
    } else if (_state == TU_THROWN) {
        setThrown();
    }
}

void TurnoutController::setClose() {
    _state = TU_CLOSE;

    Serial.println((String)"Update turnout controller \"" + _id + "\" to CLOSE");

    digitalWrite(_closedPort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_closedPort, LOW);
}

void TurnoutController::setThrown() {
    _state = TU_THROWN;

    Serial.println((String)"Update turnout controller \"" + _id + "\" to THROWN");

    digitalWrite(_thrownPort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_thrownPort, LOW);
}

bool TurnoutController::getState() {
    return(_state);
}