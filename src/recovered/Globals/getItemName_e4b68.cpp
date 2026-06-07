#include "class.h"

extern "C" String *GameText_getText(void *gt, int id);
extern "C" void AEString_ctor_copy(void *dst, void *src, bool flag);
// hidden PC-relative pointer-to-pointer global (deref'd twice).
extern void *const gItemNameGameText __attribute__((visibility("hidden")));

struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

// r0=sret, r1=unused, r2=item id.
extern "C" RetStr Globals_getItemName(void *unused, int item)
{
    (void)unused;
    String *src = GameText_getText(*(void **)gItemNameGameText, item + 0x4fa);
    RetStr r;
    AEString_ctor_copy(&r, src, false);
    return r;
}
