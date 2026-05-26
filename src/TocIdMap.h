#ifndef TOCIDMAP_H
#define TOCIDMAP_H

#include "TurnoutController.h"

struct TocIdMap {
    int id;
    TurnoutController* controller;
};

const int MAX_TURNOUTS = 4;
extern TocIdMap TocIdMapList[MAX_TURNOUTS];

// Simplified Linear Lookup Function
TurnoutController* getTurnoutControllerById(int targetId);

#endif