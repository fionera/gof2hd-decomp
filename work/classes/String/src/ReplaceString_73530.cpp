#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle);
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
void String_concat(String *out, String *a, String *b);
void String_addAssign_str(String *self, String *other);
void String_Set_wchar(String *self, const uint16_t *s);
void String_dtor(String *self);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::ReplaceString(AbyssEngine::String find, AbyssEngine::String repl)
// Replace each occurrence of `find` with `repl` (here named param2=repl) in this string.
extern "C" void String_ReplaceString(String *self, String *find, String *repl)
{
    if (F<int>(self, 0x4) == 0 || F<int>(self, 0x8) == 0 || F<int>(find, 0x8) == 0)
        return;

    SBuf acc;
    acc.a = (uint32_t)(unsigned long)g_String_vtable;
    acc.b = 0;
    acc.c = 0;

    unsigned int pos = 0;
    int idx;
    while ((idx = (int)String_IndexOf_from(self, pos, find)) != -1) {
        SBuf head, piece;
        String_SubString((String *)&head, self, pos, (unsigned int)idx);
        String_concat((String *)&piece, (String *)&head, repl);
        String_addAssign_str((String *)&acc, (String *)&piece);
        String_dtor((String *)&piece);
        String_dtor((String *)&head);
        pos = F<int>(find, 0x8) + idx;
    }

    if (pos != 0 && pos < F<uint32_t>(self, 0x8)) {
        SBuf tail;
        String_SubString((String *)&tail, self, pos, F<uint32_t>(self, 0x8));
        String_addAssign_str((String *)&acc, (String *)&tail);
        String_dtor((String *)&tail);
    }

    if (acc.c != 0)   // accumulator has a non-zero length field
        String_Set_wchar(self, (uint16_t *)(unsigned long)acc.b);
    String_dtor((String *)&acc);
}
