
#ifndef TURNOUT_CONTROLLER_H
#define TURNOUT_CONTROLLER_H

// Do not set this high or you will electrocute your turnout!
#define TU_POWER_TIME 30

class TurnoutController
{
    public:
        TurnoutController(int, int, int, int); // id, closedPort, thrownPort, state
        void setClose();
        void setThrown();
        bool getState();
    private:
        int _id;
        int _closedPort;
        int _thrownPort;
        int _state; 
};

#endif