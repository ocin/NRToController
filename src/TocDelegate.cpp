#include "TocDelegate.h"

void TocDelegate::receivedServerVersion(int major, int minor, int patch)
{
    toclog.print("Received version: ");
    toclog.print(major);
    toclog.print(".");
    toclog.print(minor);
    toclog.print(".");
    toclog.println(patch);
}

void TocDelegate::receivedTurnoutList()
{
    toclog.println("Received turnout list:");
    initTurnouts();
    toclog.println("End of turnout list:");
}

void TocDelegate::receivedTurnoutAction(int turnoutId, bool thrown)
{
    TurnoutController *toc = getTocById(turnoutId);
    if (toc != NULL)
    {
        toclog.printf("[receivedTurnoutAction] Received turnout action, setting %d to %s\n", turnoutId, (thrown ? "TROWN" : "CLOSE"));
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
        toclog.printf("[receivedTurnoutAction] Turnout %d is not defined in this turnout controller, skipping\n", turnoutId);
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
            toclog.printf("[initTurnouts] Init turnout, setting %d to %s\n", turnoutId, (state ? "TROWN" : "CLOSE"));
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
            toclog.printf("[initTurnouts] Turnout %d is not defined in this turnout controller, skipping\n", turnoutId);
        }
    }
}

void TocDelegate::initTocs()
{
    for(int tocnum=1; tocnum <= TocConfig::NUM_TURNOUTS; tocnum++) {
        int turnoutId = tocConfig.getTurnoutId(tocnum);
        if(turnoutId) {
            toclog.printf("[initTocs] Turnout controller %d mapped to turnout id %d\n", tocnum, turnoutId);
            _toIdToToc.insert(std::make_pair(tocConfig.turnoutid_mappings[tocnum-1], tocnum));
        } else {
            toclog.printf("[initTocs] Turnout controller %d is not defined, skipping\n", tocnum);
        }
    }
    _tocMap.insert(std::make_pair(1, new TurnoutController(1, TOC_1_CLOSE_PORT, TOC_1_THROWN_PORT)));
    _tocMap.insert(std::make_pair(2, new TurnoutController(2, TOC_2_CLOSE_PORT, TOC_2_THROWN_PORT)));
    _tocMap.insert(std::make_pair(3, new TurnoutController(3, TOC_3_CLOSE_PORT, TOC_3_THROWN_PORT)));
    _tocMap.insert(std::make_pair(4, new TurnoutController(4, TOC_4_CLOSE_PORT, TOC_4_THROWN_PORT)));
}

TurnoutController *TocDelegate::getTocById(int turnoutId)
{
    return (_tocMap[_toIdToToc[turnoutId]]);
}