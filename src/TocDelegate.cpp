#include "TocDelegate.h"
#include "TurnoutController.h"
#include "TocIdMap.h"
#include "common.h"

void TocDelegate::receivedTurnoutAction(int turnoutId, bool thrown)
{
#ifdef DEBUG
    Serial.print("Received turnout action id: ");
    Serial.print(turnoutId);
    Serial.print(" state:");
    Serial.println(thrown);
#endif
    if (thrown)
    {
        getTurnoutControllerById(turnoutId)->setThrown();
    }
    else
    {
        getTurnoutControllerById(turnoutId)->setClose();
    }
}