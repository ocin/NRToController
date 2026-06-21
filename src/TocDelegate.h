#pragma once

#include <DCCEXProtocol.h>
#include <map>

#include "common.h"
#include "setup_dccex.h"
#include "TurnoutController.h"
#include "TocLog.h"

class TocDelegate : public DCCEXProtocolDelegate
{
public:
    void receivedServerVersion(int major, int minor, int patch) override;
    void receivedTurnoutList() override;
    void receivedTurnoutAction(int turnoutId, bool thrown) override;
    void initTocs();
private:
    void initTurnouts();
    TurnoutController *getTocById(int);
    std::map<int, TurnoutController*> _tocMap;
    std::map<int, int> _toIdToToc;
};