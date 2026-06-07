#include "class.h"

extern "C" bool Standing_isEnemy(Standing *self, int race);
extern "C" unsigned Standing_isFriend(Standing *self, int race);

// Standing::isNeutral(int race): neither enemy nor friend.
extern "C" unsigned Standing_isNeutral(Standing *self, int race) {
    if (Standing_isEnemy(self, race)) return 0;
    return Standing_isFriend(self, race) ^ 1;
}
