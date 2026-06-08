#include "class.h"

extern "C" {
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
void String_Set_wchar(String *self, const uint16_t *s);
void String_dtor(String *self);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::Trim() - strip leading/trailing spaces and tabs.
extern "C" void String_Trim(String *self)
{
    int data = F<int>(self, 0x4);
    unsigned int len;
    if (data != 0 && (len = F<uint32_t>(self, 0x8)) != 0) {
        short *base = (short *)(unsigned int)data;
        unsigned int start = 0;
        while (start < len) {
            short c = base[start];
            if (c != 0x20 && c != 9)
                break;
            start = start + 1;
        }
        // Walk back over trailing whitespace.
        unsigned int end = len;
        short c;
        do {
            do {
                c = base[end - 1];
                end = end - 1;
            } while (c == 9);
        } while (c == 0x20);

        SBuf tmp;
        String_SubString((String *)&tmp, self, start, end);
        String_Set_wchar(self, *(uint16_t **)((char *)&tmp + 0x4));
        String_dtor((String *)&tmp);
    }
}
