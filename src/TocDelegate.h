#ifndef TOC_DELEGATE_H
#define TOC_DELEGATE_H

#include <DCCEXProtocol.h>

#include "common.h"

class TocDelegate : public DCCEXProtocolDelegate
{
public:
    void receivedTurnoutAction(int turnoutId, bool thrown) override;
};

#endif