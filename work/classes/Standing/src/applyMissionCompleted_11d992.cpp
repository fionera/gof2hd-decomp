#include "class.h"

extern "C" void Standing_applyPoints(Standing *self, int race, int delta);

// Standing::applyMissionCompleted(int race): +5 reputation (delta -5 fed to applyPoints).
extern "C" void Standing_applyMissionCompleted(Standing *self, int race) {
    Standing_applyPoints(self, race, -5);
}
