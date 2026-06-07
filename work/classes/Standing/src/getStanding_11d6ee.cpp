#include "class.h"

// Standing::getStanding(int race) [static-style: this in r0, race in r1].
// In "derived" mode (currentRace >= 0) factions 0/1 map to fixed standings based
// on currentRace; otherwise the raw standings[race] value is returned.
extern "C" int Standing_getStanding(Standing *self, int race) {
    int cr = F<int>(self, 4);
    if (cr >= 0) {
        if (race == 0) {
            int v = 0x46;
            if (cr == 0) v = 100;
            if (cr == 1) v = -100;
            return v;
        }
        if (race == 1) {
            int v = 0x46;
            if (cr == 2) v = 100;
            if (cr == 3) v = -100;
            return v;
        }
    }
    return F<int *>(self, 0)[race];
}
