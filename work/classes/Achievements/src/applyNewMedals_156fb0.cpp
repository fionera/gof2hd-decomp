#include "class.h"

extern "C" void Achievements_countMedals(Achievements *self);
// Trailing tail-call through a global callback (resolved PC-relative in the target).
extern "C" void Achievements_onAllMedals(Achievements *self);

extern "C" void Achievements_applyNewMedals(Achievements *self)
{
    int *newMedals = F<int *>(self, 0x4);
    int i;
    for (i = 0; i != 0x2d; i = i + 1) {
        int nv = newMedals[i];
        if (0 < nv) {
            int *medals = F<int *>(self, 0x0);
            int cur = medals[i];
            if (nv >= cur) {
                if (cur == 0)
                    medals[i] = nv;
            } else {
                medals[i] = nv;
            }
        }
    }
    Achievements_countMedals(self);
    if (F<int>(self, 0x24) == 0x23) {
        F<int *>(self, 0x4)[0x23] = 1;
        F<int *>(self, 0x0)[0x23] = 1;
        Achievements_onAllMedals(self);
    }
}
