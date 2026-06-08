#include "class.h"

// AbyssEngine::PaintCanvas::GetReverseString(String in) — convenience overload that forwards to
// the (String,bool) overload, choosing the reverse flag from a runtime layout-direction byte at
// param2+0x1c (RTL when zero).
using AbyssEngine::String;

extern "C" void grs2_String_ctor_copy(String *self, const String *other, bool copy);
extern "C" void grs2_String_dtor(String *self);
extern "C" void grs2_GetReverseString_impl(String *out, unsigned r1, String *tmp, int reverse);
extern "C" void grs2___stack_chk_fail();

__attribute__((visibility("hidden"))) extern int **g_grs2_canary;

void GetReverseString(AbyssEngine::String *out, int param2, AbyssEngine::String *in)
{
    int *canary = *g_grs2_canary;
    int saved = *canary;

    String tmp;
    grs2_String_ctor_copy(&tmp, in, false);
    grs2_GetReverseString_impl(out, 0, &tmp, *(char *)((char *)(unsigned long)param2 + 0x1c) == 0);
    grs2_String_dtor(&tmp);

    if (*canary != saved)
        grs2___stack_chk_fail();
}
