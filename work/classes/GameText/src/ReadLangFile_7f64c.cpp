#include "class.h"

// GameText::ReadLangFile(unsigned int file, int count)
// Reads `count` records from an opened .lang file. Each record is a big-endian u16 byte
// length followed by that many UTF-8 bytes; the bytes are decoded to wide chars, wrapped in
// a heap String (run through convertStringFromArabic when the active language is Arabic == 9),
// and stored into the text table at +0x0c. On a short read the table is released and the file
// is closed. file == 0 is a no-op early exit.

extern "C" int   _ZN11AbyssEngine6AEFile4ReadEjPvj(int size, void *dst, unsigned int file);
extern "C" void  _ZN11AbyssEngine6AEFile5CloseEj(unsigned int file);
extern "C" unsigned short *_ZN11AbyssEngine6String15getWCharFromUtf8EPcj(char *utf8, unsigned int len);
extern "C" void  AEString_ctor_cstr(void *s, const char *text, bool copy);     // wchar ctor (text*, copy)
extern "C" void  AEString_ctor_wide(void *s, const unsigned short *text, bool copy);
extern "C" void  AEString_dtor(void *s);
extern "C" void  GameText_release(void *self);
extern "C" void  GameText_convertStringFromArabic(void *out, int pad, void *in);

void *operator new(uint32_t);
void operator delete[](void *) noexcept;
void *operator new[](uint32_t);

// Active language code; 9 == Arabic.
__attribute__((visibility("hidden"))) extern short *g_langCode;

extern "C" void GameText_ReadLangFile(void *self, unsigned int file, int count)
{
    if (file == 0)
        return;

    short lang = *g_langCode;
    for (int i = 0; i < count; ++i) {
        unsigned short len;
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &len, file) == 0) {
            GameText_release(self);
            break;
        }
        // Length field is stored big-endian; byte-swap to host order.
        unsigned int byteLen = (unsigned int)((len >> 8) | (len << 8)) & 0xffff;

        char *utf8 = (char *)operator new[](byteLen + 1);
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj((int)byteLen, utf8, file) == 0) {
            operator delete[](utf8);
            GameText_release(self);
            break;
        }
        utf8[byteLen] = '\0';

        unsigned short *wide = _ZN11AbyssEngine6String15getWCharFromUtf8EPcj(utf8, byteLen);

        String *s = (String *)operator new(0xc);
        String **table = (String **)F<void *>(self, 0x0c);
        if (lang == 9) {
            String tmp;
            AEString_ctor_wide(&tmp, wide, false);
            GameText_convertStringFromArabic(s, 0, &tmp);
            table[i] = s;
            AEString_dtor(&tmp);
        } else {
            AEString_ctor_wide(s, wide, false);
            table[i] = s;
        }

        if (wide != 0)
            operator delete[](wide);
        operator delete[](utf8);
    }

    _ZN11AbyssEngine6AEFile5CloseEj(file);
}
