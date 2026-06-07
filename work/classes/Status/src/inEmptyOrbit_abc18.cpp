#include "class.h"

extern "C" int Station_getIndex(Station *s);
__attribute__((visibility("hidden"))) extern int g_emptyOrbitMask;

// True when the current orbit has no station/special content for the player to interact with.
bool Status::inEmptyOrbit() {
    int idx = Station_getIndex(station);
    if (idx == 0x4e && currentCampaignMission < 2) {
        return true;
    }
    if (inAlienOrbit() != 0 && (unsigned)(currentCampaignMission - 0x2b) < 0x29) {
        return true;
    }
    if (inAlienOrbit() != 0 && currentCampaignMission > 0x99) {
        return true;
    }
    unsigned d = idx - 0x66;
    if (d < 0x20) {
        if ((1 << (d & 0xff) & g_emptyOrbitMask) != 0) {
            return true;
        }
        if (d == 9) {
            if (currentCampaignMission > 0x5d) {
                return true;
            }
            return idx == 0x86;
        }
    }
    if (idx != 0x65 || currentCampaignMission < 0x54) {
        return idx == 0x86;
    }
    return true;
}
