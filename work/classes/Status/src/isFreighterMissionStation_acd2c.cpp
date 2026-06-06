#include "class.h"

// True when `station` hosts a freighter mission (two bit-masks plus the special id 0xf).
__attribute__((visibility("hidden"))) extern int DAT_freighterA;
__attribute__((visibility("hidden"))) extern int DAT_freighterB;

int Status::isFreighterMissionStation(int station) {
    if ((unsigned)(station - 0x1e) < 0x1f && ((1 << ((station - 0x1e) & 0xff)) & DAT_freighterA) != 0)
        return 1;
    if ((unsigned)(station - 0x46) < 0x1a && ((1 << ((station - 0x46) & 0xff)) & DAT_freighterB) != 0)
        return 1;
    if (station == 0xf) return 1;
    return 0;
}
