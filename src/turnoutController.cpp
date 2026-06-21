#include <Arduino.h>

#include "common.h"
#include "TurnoutController.h"

TurnoutController::TurnoutController(int id, int closePort, int thrownPort) {
    _id = id;

    _closePort = closePort;
    _thrownPort = thrownPort;

    pinMode(_closePort, OUTPUT);
    pinMode(_thrownPort, OUTPUT);

    toclog.printf("[Turnout Init] Initialize turnout controller \"%d\"\n", _id);
}

void TurnoutController::setClose() {
#ifdef DEBUG
    toclog.printf("[Turnout Update] Update turnout controller \"%d\" to CLOSE\n", _id);
#endif

    digitalWrite(_closePort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_closePort, LOW);
}

void TurnoutController::setThrown() {
#ifdef DEBUG
    toclog.printf("[Turnout Update] Update turnout controller \"%d\" to THROWN\n", _id);
#endif

    digitalWrite(_thrownPort, HIGH);
    delay(TU_POWER_TIME);
    digitalWrite(_thrownPort, LOW);
}
