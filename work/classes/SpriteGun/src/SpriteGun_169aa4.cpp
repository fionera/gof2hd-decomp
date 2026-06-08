#include "class.h"

// SpriteGun::SpriteGun(Gun*, int) — store (g_SpriteGun + 8) at +0 and 0 at +4.
// g_SpriteGun is a value-typed global loaded PC-relative (hidden visibility) with a
// single dereference; the two word stores fuse into a single strd.
__attribute__((visibility("hidden"))) extern "C" char *const g_SpriteGun;

extern "C" void SpriteGun_SpriteGun(SpriteGun *this_, Gun *param_1, int param_2)
{
    F<char *>(this_, 0x0) = g_SpriteGun + 8;
    F<int>(this_, 0x4) = 0;
}
