#include "class.h"

extern "C" int AERandom_nextInt(int rng, int bound);
// *gCreateChar2Rng1 -> rng (used for the type-3 reroll); gCreateChar2Table -> int[] table base
// (PC-relative address, not dereferenced); *gCreateChar2Rng2 -> rng for the per-part loop.
extern void *const gCreateChar2Rng1 __attribute__((visibility("hidden")));
extern int gCreateChar2Table __attribute__((visibility("hidden")));
extern void *const gCreateChar2Rng2 __attribute__((visibility("hidden")));

// ImageFactory::createChar(bool, int) — builds a 5-int char descriptor (type + 4 random parts).
// r0=param_1(bool, unused on this path), r1=param_2, r2=sel.
extern "C" int *ImageFactory_createChar_bi(int param_1, int param_2, int sel)
{
    (void)param_1;
    if (sel == 3) {
        int t = AERandom_nextInt(*(int *)gCreateChar2Rng1, 4);
        sel = (t != 0) ? 2 : 0;
    }
    int v = sel;
    int *table = &gCreateChar2Table;
    if (sel == 0) v = 10;
    if (param_2 != 0) v = sel;
    if (v == 5) v = 0;
    int *obj = (int *)operator new[](0x14);
    obj[0] = v;
    int *base = (int *)((char *)table + v * 16);
    for (int i = 0; i != 4; ++i)
        obj[i + 1] = AERandom_nextInt(*(int *)gCreateChar2Rng2, base[i]);
    return obj;
}
