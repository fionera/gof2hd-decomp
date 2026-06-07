#include "class.h"
extern "C" int getMaxHP(Ship *self) {
    int *m = (int *)self->mods;
    int bonus;
    if (m == 0) {
        bonus = 0;
    } else {
        bonus = 0;
        int n = *m;
        for (int i = 0; n != i; i = i + 1) {
            if (((int *)m[1])[i] == 0) {
                bonus = bonus + 0x28;
            }
        }
    }
    return self->baseHP + bonus;
}
