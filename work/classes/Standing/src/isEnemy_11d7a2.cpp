#include "class.h"

// Standing::isEnemy(int race) [static-style: this in r0, race in r1].
// NOTE: best-effort. Logic matches the target exactly, but under -Oz clang keeps
// `this` in r0 / currentRace in r2, whereas the original swaps them (this->r2,
// cr->r0) so the "== 0" test lowers to clz/lsr in r0 — a register-allocation
// choice not reachable from portable C. Left as the faithful decompiled form.
extern "C" bool Standing_isEnemy(Standing *self, int race) {
    int iVar1 = F<int>(self, 4);
    if (-1 < iVar1) {
        if (race != 1) {
            if (race == 3) {
                iVar1 = iVar1 + -2;
            } else if (race == 2) {
                iVar1 = iVar1 + -3;
            } else {
                if (race != 0) return false;
                iVar1 = iVar1 + -1;
            }
        }
        return iVar1 == 0;
    }
    if (race == 1) {
        iVar1 = **(int **)self;
    } else {
        if (race != 3) {
            if (race == 2) {
                iVar1 = *(int *)(*(int *)self + 4);
            } else {
                if (race != 0) return false;
                iVar1 = **(int **)self;
            }
            return iVar1 < -0x46;
        }
        iVar1 = *(int *)(*(int *)self + 4);
    }
    return 0x46 < iVar1;
}
