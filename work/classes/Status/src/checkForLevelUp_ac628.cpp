#include "class.h"

extern "C" int __aeabi_idiv(int a, int b);
__attribute__((visibility("hidden"))) extern int g_levelXPTable[];

// Recomputes the player level from the weighted experience sum against the XP threshold table.
void Status::checkForLevelUp() {
    int d4 = __aeabi_idiv(field_d4, 3);
    int a0d = __aeabi_idiv(field_a0, 0x32);
    int sum = a0d + (kills + d4) + field_a4 + missionCount * 2 + currentCampaignMission + stationsVisited;
    for (int i = 0; i != 0x15; i = i + 1) {
        if (sum >= g_levelXPTable[i]) {
            level = i;
        }
    }
}
