#include "class.h"

// GameText::setLanguage(short stringCount, int langId)
// Selects the active UI language. If the language is unchanged it does nothing; otherwise it
// releases the previous text table, allocates a fresh String*[stringCount] (nulled), resolves
// the per-language .lang filename via a switch, records the language code (Arabic-shaping
// languages keep their id, the rest map to 0), falls back to the English file when the chosen
// one is missing, then opens it and streams the records in through ReadLangFile.
//
// Exported as GameText_setLanguage_si to match the forwarder in setLanguage_7f2b8.cpp.

extern "C" void  AEString_ctor_cstr(void *s, const char *text, bool copy);
extern "C" void  AEString_assign(void *dst, void *src);
extern "C" void  AEString_dtor(void *s);
extern "C" char *AEString_GetAEChar(void *s);
extern "C" int   _ZN11AbyssEngine6AEFile9FileExistERKNS_6StringE(void *path);
extern "C" void  _ZN11AbyssEngine6AEFile8OpenReadERKNS_6StringEPj(void *path, unsigned int *fileOut);
extern "C" void  GameText_release(void *self);
extern "C" void  GameText_ReadLangFile(void *self, unsigned int file, int count);

void *operator new[](__SIZE_TYPE__);
void operator delete[](void *) noexcept;

// Active language code slot (compared against the requested id to detect a no-op).
__attribute__((visibility("hidden"))) extern unsigned short *g_langCode;
// Per-language file paths; index 0..15 plus a default and an English fallback.
__attribute__((visibility("hidden"))) extern const char *gLangPaths[17];
__attribute__((visibility("hidden"))) extern const char gLangPathDefault[];
__attribute__((visibility("hidden"))) extern const char gLangPathEnglish[];

extern "C" void GameText_setLanguage_si(void *self, int stringCount, int langId)
{
    if ((unsigned int)*g_langCode == ((unsigned int)langId & 0xffff))
        return;

    GameText_release(self);
    i32(self, 0x1c) = stringCount;

    String **table = (String **)operator new[]((uint32_t)((unsigned long long)stringCount * 4));
    F<void *>(self, 0xc) = table;
    for (int i = 0; i < stringCount; ++i)
        table[i] = 0;

    String path;
    AEString_ctor_cstr(&path, gLangPathDefault, false);

    // Arabic-class languages (1..9, 12..15) retain their id for shaping; others map to 0.
    unsigned short code;
    const char *p;
    switch (langId) {
    case 0:  p = gLangPaths[0];  code = 0;  break;
    case 1:  p = gLangPaths[1];  code = 1;  break;
    case 2:  p = gLangPaths[2];  code = 2;  break;
    case 3:  p = gLangPaths[3];  code = 3;  break;
    case 4:  p = gLangPaths[4];  code = 4;  break;
    case 5:  p = gLangPaths[5];  code = 5;  break;
    case 6:  p = gLangPaths[6];  code = 6;  break;
    case 7:  p = gLangPaths[7];  code = 7;  break;
    case 8:  p = gLangPaths[8];  code = 8;  break;
    case 9:  p = gLangPaths[9];  code = 9;  break;
    case 10: p = gLangPaths[10]; code = 0;  break;
    case 11: p = gLangPaths[11]; code = 0;  break;
    case 12: p = gLangPaths[12]; code = 12; break;
    case 13: p = gLangPaths[13]; code = 13; break;
    case 14: p = gLangPaths[14]; code = 14; break;
    case 15: p = gLangPaths[15]; code = 15; break;
    default: p = gLangPaths[16]; code = 0;  break;
    }
    {
        String tmp;
        AEString_ctor_cstr(&tmp, p, false);
        AEString_assign(&path, &tmp);
        AEString_dtor(&tmp);
    }
    *g_langCode = code;

    // Fall back to English when the selected file is absent.
    if (_ZN11AbyssEngine6AEFile9FileExistERKNS_6StringE(&path) == 0) {
        operator delete[](AEString_GetAEChar(&path));
        String eng;
        AEString_ctor_cstr(&eng, gLangPathEnglish, false);
        AEString_assign(&path, &eng);
        AEString_dtor(&eng);
        *g_langCode = 0;
    }

    unsigned int file = 0;
    _ZN11AbyssEngine6AEFile8OpenReadERKNS_6StringEPj(&path, &file);
    GameText_ReadLangFile(self, 0, stringCount);
    AEString_dtor(&path);
}
