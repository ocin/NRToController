#ifndef TOC_DELEGATE_H
#define TOC_DELEGATE_H

#include <DCCEXProtocol.h>
#include <map>

#include "common.h"
#include "setup_dccex.h"
#include "TurnoutController.h"

class TocDelegate : public DCCEXProtocolDelegate
{
public:
    void receivedServerVersion(int major, int minor, int patch) override;
    void receivedTurnoutList() override;
    void receivedTurnoutAction(int turnoutId, bool thrown) override;
    void initPorts();
private:
    void initTurnouts();
    TurnoutController *getTocById(int);
    std::map<int, TurnoutController*> _tocMap;
    std::map<int, int> _toIdToToc;
};

#endif