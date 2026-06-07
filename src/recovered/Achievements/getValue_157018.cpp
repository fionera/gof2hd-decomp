#include "class.h"

// Static medal-threshold table embedded in the binary, addressed PC-relative.
// getValue returns table[index*3 + sub - 1].
extern const int gAchievementValues[] __attribute__((visibility("hidden")));

extern "C" int Achievements_getValue(Achievements *self, int index, int sub)
{
    (void)self;
    const int *row = gAchievementValues + index * 3;
    return row[sub - 1];
}
