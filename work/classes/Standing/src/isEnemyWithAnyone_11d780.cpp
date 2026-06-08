#include "class.h"

// Standing::isEnemyWithAnyone(): true if either home-race standing is outside the
// non-hostile band, i.e. (standing + 0x46) does not fit in [0, 0x8c] as unsigned.
// Both comparisons are always evaluated (bitwise |) to mirror the branchless target.
extern "C" bool Standing_isEnemyWithAnyone(Standing *self) {
    int *p = F<int *>(self, 0);
    int a = p[0];
    int b = p[1];
    return (bool)(((unsigned)(b + 0x46) > 0x8c) | ((unsigned)(a + 0x46) > 0x8c));
}
