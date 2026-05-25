#ifndef TOC_DELEGATE_H
#define TOC_DELEGATE_H

#include <DCCEXProtocol.h>

#include "common.h"

class TocDelegate : public DCCEXProtocolDelegate
{
public:
    void receivedServerVersion(int major, int minor, int patch) override;
    void receivedTurnoutList() override;
    void receivedTurnoutAction(int turnoutId, bool thrown) override;
private:
    void initTurnouts();
};

#endif