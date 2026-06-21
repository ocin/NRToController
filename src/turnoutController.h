
#pragma once

#include "TocLog.h"

// Do not set this high or you will electrocute your turnout!
#define TU_POWER_TIME 50

class TurnoutController
{
    public:
        TurnoutController(int, int, int); // id, closedPort, thrownPort, state
        void setClose();
        void setThrown();
    private:
        int _id;
        int _closePort;
        int _thrownPort;
};
