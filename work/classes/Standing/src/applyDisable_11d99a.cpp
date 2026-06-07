#include "class.h"

extern "C" void Standing_applyPoints(Standing *self, int race, int delta);

// Standing::applyDisable(int race).
extern "C" void Standing_applyDisable(Standing *self, int race) {
    Standing_applyPoints(self, race, 2);
}
