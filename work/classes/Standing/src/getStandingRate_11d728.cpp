#include "class.h"

extern "C" int Standing_getStanding(Standing *self, int race);

// Standing::getStandingRate(int race): getStanding(race) normalized to [-1, 1].
extern "C" float Standing_getStandingRate(Standing *self, int race) {
    return (float)Standing_getStanding(self, race) / 100.0f;
}
