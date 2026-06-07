#include "class.h"

// Lookup table of "enemy race" per faction index (4 entries), in .data.rel.ro.
__attribute__((visibility("hidden"))) extern const uint32_t Standing_enemyRaceTable[4];

// Standing::getEnemyRace(int idx): idx<4 ? table[idx] : 8.
extern "C" uint32_t Standing_getEnemyRace(Standing *self, unsigned idx) {
    (void)self;
    if (idx < 4)
        return Standing_enemyRaceTable[idx];
    return 8;
}
