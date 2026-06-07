#include "class.h"

// Standing::isFriend(int race).
extern "C" bool Standing_isFriend(Standing *self, int race) {
    int iVar1 = F<int>(self, 4);
    if (-1 < iVar1) {
        if (race == 1) {
            iVar1 = iVar1 + -1;
        } else if (race == 3) {
            iVar1 = iVar1 + -3;
        } else if (race == 2) {
            iVar1 = iVar1 + -2;
        } else if (race != 0) {
            return false;
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
            return 0x46 < iVar1;
        }
        iVar1 = *(int *)(*(int *)self + 4);
    }
    return iVar1 < -0x46;
}
