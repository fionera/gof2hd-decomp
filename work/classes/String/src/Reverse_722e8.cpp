#include "class.h"

extern "C" {
int GameText_getLanguage();                             // 0x6f544
String *String_ctor_wchar(String *self, const uint16_t *s, bool rev);
void String_dtor(String *self);
}

// Sentinel code unit returned by operator[] for out-of-range reads (PC-rel static NUL).
__attribute__((visibility("hidden"))) extern uint16_t g_String_nullChar;

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::Reverse() - reverse the code units (only for language id 9 / RTL).
extern "C" void String_Reverse(String *self)
{
    if (F<int>(self, 0x4) != 0 && GameText_getLanguage() == 9) {
        SBuf src;
        String_ctor_wchar((String *)&src, F<uint16_t *>(self, 0x4), false);
        uint16_t *srcData = *(uint16_t **)((char *)&src + 0x4);
        int srcLen = *(int *)((char *)&src + 0x8);

        unsigned int idx = F<int>(self, 0x8) - 1;
        uint16_t *tail = srcData + idx;
        uint16_t *nul = &g_String_nullChar;

        int dst = 0;
        for (; idx != 0xffffffff; idx = idx - 1) {
            uint16_t *pick = nul;
            if ((int)idx < srcLen)
                pick = tail;
            if (idx > 0x7fffffff)
                pick = nul;
            tail = tail - 1;
            *(uint16_t *)((char *)F<void *>(self, 0x4) + dst) = *pick;
            dst = dst + 2;
        }
        String_dtor((String *)&src);
    }
}
