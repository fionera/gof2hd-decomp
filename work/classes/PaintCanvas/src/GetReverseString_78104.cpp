#include "class.h"

// AbyssEngine::PaintCanvas::GetReverseString(String out, _, String in, bool reverse)
// When reverse is false, returns a copy of `in`; otherwise concatenates `in`'s characters in
// reverse order (one SubString per char) into a fresh String.
using AbyssEngine::String;

extern "C" void grs_String_ctor_copy(String *self, const String *other, bool copy);
extern "C" void grs_String_ctor_cstr(String *self, const char *text, bool copy);
extern "C" void grs_String_substr(String *out, const String *self, unsigned begin, unsigned end);
extern "C" void grs_String_append(String *self, const String *other);
extern "C" void grs_String_dtor(String *self);
extern "C" void grs___stack_chk_fail();

__attribute__((visibility("hidden"))) extern int **g_grs_canary;
__attribute__((visibility("hidden"))) extern const char g_grs_empty[];

void GetReverseString(AbyssEngine::String *out, unsigned int /*r1*/, AbyssEngine::String *in,
                      int reverse)
{
    int *canary = *g_grs_canary;
    int saved = *canary;

    if (reverse == 0) {
        grs_String_ctor_copy(out, in, false);
    } else {
        grs_String_ctor_cstr(out, g_grs_empty, false);
        int i = *(int *)((char *)in + 8);
        while (--i >= 0) {
            String piece;
            grs_String_substr(&piece, in, (unsigned)i, (unsigned)(i + 1));
            grs_String_append(out, &piece);
            grs_String_dtor(&piece);
        }
    }

    if (*canary != saved)
        grs___stack_chk_fail();
}
