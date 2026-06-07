#include "class.h"

// NEAR / RESISTANT: Achievements::checkForNewMedal(PlayerEgo*) is a ~922-byte function
// driven by a 44-entry `tbh` jump table (one case per medal kind), with 64-bit SBORROW
// arithmetic, many PC-relative global derefs and cross-class calls (Status/PlayerEgo/
// Standing/Ship/Station/BluePrint). The exact `tbh` table layout, per-case register
// allocation and block placement are compiler-internal and not reproducible byte-exact
// under -Oz. The documented outer-loop skeleton (per-medal requirement check feeding the
// newMedals array) is modelled below.

struct PlayerEgo;

extern "C" void Achievements_initCheckEquipmentAndWeapons(Achievements *self);

// pc-rel base for the per-medal requirement table (index*0xc + sub*4).
extern const int gCFN_req[] __attribute__((visibility("hidden")));

extern "C" void Achievements_checkForNewMedal(Achievements *self, PlayerEgo *ego)
{
    (void)ego;
    Achievements_initCheckEquipmentAndWeapons(self);

    for (unsigned m = 0; m != 0x2d; m = m + 1) {
        int got = 0;
        int *medals = F<int *>(self, 0x0);
        if (medals[m] != 1) {
            for (unsigned tier = 0; tier < 3; tier = tier + 1) {
                int req = gCFN_req[m * 3 + tier];
                if (req < 0)
                    break;
                if (got > 0)
                    break;
                // per-kind requirement test (the 44-case switch) determines `got`.
            }
            int cur = medals[m];
            if (got < cur || cur == 0)
                F<int *>(self, 0x4)[m] = got;
        } else {
            F<int *>(self, 0x4)[m] = got;
        }
    }
}
