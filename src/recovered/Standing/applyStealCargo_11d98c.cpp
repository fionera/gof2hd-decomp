#include "class.h"

extern "C" void Standing_applyPoints(Standing *self, int race, int delta);

// Standing::applyStealCargo(int race): -2 reputation toward that race.
extern "C" void Standing_applyStealCargo(Standing *self, int race) {
    Standing_applyPoints(self, race, 2);
}
