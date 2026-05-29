#include <map>

#include "TocDelegate.h"
#include "TurnoutController.h"
#include "common.h"

extern std::map<int, TurnoutController*> tocIdMap;

void TocDelegate::receivedServerVersion(int major, int minor, int patch)
{
    Serial.print("Received version: ");
    Serial.print(major);
    Serial.print(".");
    Serial.print(minor);
    Serial.print(".");
    Serial.println(patch);
}

void TocDelegate::receivedTurnoutList()
{
    Serial.println("Received turnout list:");
    initTurnouts();
    Serial.println("End of turnout list:");
}

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
        tocIdMap[turnoutId]->setThrown();
    }
    else
    {
        tocIdMap[turnoutId]->setClose();
    }
}

void TocDelegate::initTurnouts()
{
    for (Turnout *turnout = dccexProtocol.turnouts->getFirst(); turnout; turnout = turnout->getNext())
    {
        int turnoutId = turnout->getId();
        const char *name = turnout->getName();
        bool state = turnout->getThrown();
#ifdef DEBUG
        Serial.print(turnoutId);
        Serial.print(" name: ");
        Serial.print(name);
        Serial.print(" state: ");
        Serial.println(state);
#endif
        if (state)
        {
            tocIdMap[turnoutId]->setThrown();
        }
        else
        {
            tocIdMap[turnoutId]->setClose();
        }
    }
}