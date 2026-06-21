#include <map>

#include "TocDelegate.h"
#include "TurnoutController.h"
#include "common.h"

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
    TurnoutController *toc = getTocById(turnoutId);
    if (toc != NULL)
    {
        Serial.printf("[receivedTurnoutAction] Received turnout action, setting %d to %s\n", turnoutId, (thrown ? "TROWN" : "CLOSE"));
        if (thrown)
        {
            toc->setThrown();
        }
        else
        {
            toc->setClose();
        }
    }
    else
    {
        Serial.printf("[receivedTurnoutAction] Turnout %d is not defined in this turnout controller, skipping\n", turnoutId);
    }
}

void TocDelegate::initTurnouts()
{
    for (Turnout *turnout = dccexProtocol.turnouts->getFirst(); turnout; turnout = turnout->getNext())
    {
        int turnoutId = turnout->getId();
        TurnoutController *toc = getTocById(turnoutId);
        if (toc != NULL)
        {
            const char *name = turnout->getName();
            bool state = turnout->getThrown();
            Serial.printf("[initTurnouts] Init turnout, setting %d to %s\n", turnoutId, (state ? "TROWN" : "CLOSE"));
            if (state)
            {
                toc->setThrown();
            }
            else
            {
                toc->setClose();
            }
        }
        else
        {
            Serial.printf("[initTurnouts] Turnout %d is not defined in this turnout controller, skipping\n", turnoutId);
        }
    }
}

void TocDelegate::initPorts()
{
    _tocMap.insert(std::make_pair(1, new TurnoutController(1, 12, 13)));
    _tocMap.insert(std::make_pair(2, new TurnoutController(2, 16, 17)));
    _tocMap.insert(std::make_pair(3, new TurnoutController(3, 18, 19)));
    _tocMap.insert(std::make_pair(4, new TurnoutController(4, 21, 22)));

    _toIdToToc.insert(std::make_pair(101, 1));
    _toIdToToc.insert(std::make_pair(102, 2));
    _toIdToToc.insert(std::make_pair(103, 3));
    _toIdToToc.insert(std::make_pair(104, 4));
}

TurnoutController *TocDelegate::getTocById(int turnoutId)
{
    return (_tocMap[_toIdToToc[turnoutId]]);
}