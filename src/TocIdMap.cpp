#include "TurnoutController.h"
#include "TocIdMap.h"

// Define the global TocIdMapList in one translation unit to avoid multiple definitions
TocIdMap TocIdMapList[MAX_TURNOUTS];

// Simplified Linear Lookup Function
TurnoutController* getTurnoutControllerById(int targetId) {
    for (int i = 0; i < MAX_TURNOUTS; i++) {
        if (TocIdMapList[i].id == targetId) {
            return TocIdMapList[i].controller; // Found
        }
    }
    return nullptr; // Not found
}
