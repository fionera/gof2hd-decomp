#include "class.h"

__attribute__((visibility("hidden"))) extern Status **g_statusBoard;
extern "C" int SolarSystem_getRace(int system);
extern "C" int Wanted_getBoard(Wanted *w);
extern "C" int Wanted_getRequiredMission(Wanted *w);
extern "C" int Station_getIndex(Station *s);

// True if any wanted entry can post on the current system's bounty board right now.
int Status::wantedBoardAccessible() {
    Status **slot = g_statusBoard;
    Array<Wanted *> *w = (*slot)->wanted;
    for (unsigned i = 0; i < w->size; i = i + 1) {
        int race = SolarSystem_getRace((*slot)->system);
        int board = Wanted_getBoard(((Wanted **)w->data)[i]);
        if (race == board) {
            int ccm = (*slot)->currentCampaignMission;
            if (ccm >= Wanted_getRequiredMission(((Wanted **)w->data)[i]) &&
                (*slot)->inAlienOrbit() == 0 &&
                Station_getIndex((*slot)->station) != 0x6c) {
                return 1;
            }
        }
    }
    return 0;
}
