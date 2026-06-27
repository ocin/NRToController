#include <Arduino.h>

#include "common.h"
#include "TurnoutController.h"

TurnoutController::TurnoutController(int id, int enablePort, int closePort, int thrownPort) {
    _id = id;

    _enablePort = enablePort;
    _closePort = closePort;
    _thrownPort = thrownPort;

    pinMode(_enablePort, OUTPUT);
    digitalWrite(_enablePort, LOW);

    pinMode(_closePort, OUTPUT);
    pinMode(_thrownPort, OUTPUT);
    digitalWrite(_thrownPort, LOW);
    digitalWrite(_closePort, LOW);

    toclog.printf("[Turnout Init] Initialize turnout controller \"%d\"\n", _id);
}

void TurnoutController::setClose() {
#ifdef DEBUG
    toclog.printf("[Turnout Update] Update turnout controller \"%d\" to CLOSE\n", _id);
#endif

    digitalWrite(_closePort, HIGH);
    digitalWrite(_thrownPort, LOW);

    digitalWrite(_enablePort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_enablePort, LOW);

    digitalWrite(_closePort, LOW);
    digitalWrite(_thrownPort, LOW);
}

void TurnoutController::setThrown() {
#ifdef DEBUG
    toclog.printf("[Turnout Update] Update turnout controller \"%d\" to THROWN\n", _id);
#endif

    digitalWrite(_thrownPort, HIGH);
    digitalWrite(_closePort, LOW);

    digitalWrite(_enablePort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_enablePort, LOW);

    digitalWrite(_thrownPort, LOW);
    digitalWrite(_closePort, LOW);
}
