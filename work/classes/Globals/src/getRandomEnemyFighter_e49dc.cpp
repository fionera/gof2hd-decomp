#include "class.h"

extern "C" int Status_dlc1Won(int status);
extern "C" int AERandom_nextIntB(int rng, int bound);
extern void *const gREF_dlc __attribute__((visibility("hidden")));
extern void *const gREF_rng1 __attribute__((visibility("hidden")));
extern void *const gREF_rng2 __attribute__((visibility("hidden")));
extern const int gREF_table __attribute__((visibility("hidden")));

extern "C" unsigned Globals_getRandomEnemyFighter(Globals *self, int kind)
{
    (void)self;
    int t = kind;
    if ((unsigned)(kind - 9) > 1) {
        t = 8;
    }
    if (kind <= 3) {
        t = kind;
    }
    unsigned r;
    if (t == 1) {
        if (Status_dlc1Won(*(int *)gREF_dlc) == 0) {
            r = 9;
        } else {
            int n = AERandom_nextIntB(*(int *)gREF_rng1, 0x64);
            r = 0x27;
            if (n < 0x55) {
                r = 0x29;
            }
            if (n < 0x3c) {
                r = 9;
            }
        }
    } else if (t == 9) {
        r = 8;
    } else if (t == 0xa) {
        r = 0x2c;
    } else {
        const int *table = &gREF_table;
        int *rng = (int *)gREF_rng2;
        do {
            do {
                r = AERandom_nextIntB(*rng, 0x25);
            } while ((unsigned)((r & ~4u) - 9) < 2);
        } while (((r < 0x10) && ((1u << r & 0x8101u) != 0)) ||
                 (table[r] != t));
    }
    return r;
}
