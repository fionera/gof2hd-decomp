#include "class.h"

extern "C" void Array_int_ctor(void *a);
extern "C" void AEString_ctor_default(void *s);
extern "C" void AEString_ctor_cstr(void *s, const char *text, bool copy);
extern "C" void AEString_assign(void *dst, void *src);
extern "C" void AEString_dtor(void *s);
// PC-relative: initial language string; and a short* whose target is reset to 0xffff.
extern const char gInitLangStr[] __attribute__((visibility("hidden")));
extern "C" __attribute__((visibility("hidden"))) unsigned short *g_GameText_langReset;

// GameText::GameText() -- inits substitute Array<int>, region String, default language string.
extern "C" void GameText_ctor(GameText *self)
{
    Array_int_ctor(self);
    AEString_ctor_default((char *)self + 0x10);
    *g_GameText_langReset = 0xffff;
    F<void *>(self, 0xc) = 0;
    i32(self, 0x1c) = 0;
    String tmp;
    AEString_ctor_cstr(&tmp, gInitLangStr, false);
    AEString_assign((char *)self + 0x10, &tmp);
    AEString_dtor(&tmp);
}
