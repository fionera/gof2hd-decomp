#include "gof2/GameText.h"


struct Arr { uint32_t size; int *data; uint32_t size2; };

extern "C" __attribute__((visibility("hidden"))) short *g_GameText_language;
extern "C" void AEString_ctor_cstr(void *self, const char *text, bool copy);
extern "C" void ArrayRemoveAll_int(Arr *a);
extern "C" void ArrayAdd_int(int v, Arr *a);
extern "C" void  AEString_ctor_cstr(void *s, const char *text, bool copy);
extern "C" void  AEString_assign(void *dst, void *src);
extern "C" void  AEString_dtor(void *s);
extern "C" char *AEString_GetAEChar(void *s);
extern "C" int   _ZN11AbyssEngine6AEFile9FileExistERKNS_6StringE(void *path);
extern "C" void  _ZN11AbyssEngine6AEFile8OpenReadERKNS_6StringEPj(void *path, unsigned int *fileOut);
extern "C" void operator_delete_arr(void *p);
extern "C" void AEString_dtor(void *s);
extern "C" GameText *GameText_dtor_tail(GameText *self);
extern "C" void Array_int_ctor(void *a);
extern "C" void AEString_ctor_default(void *s);
extern "C" void AEString_ctor_cstr(void *s, const char *text, bool copy);
extern "C" void AEString_assign(void *dst, void *src);
extern "C" __attribute__((visibility("hidden"))) unsigned short *g_GameText_langReset;
extern "C" void  AEString_ctor_wide(void *s, const unsigned short *text, bool copy);
extern "C" void  AEString_SubString(void *out, void *s, unsigned int a, unsigned int b);
extern "C" void *memcpy(void *, const void *, unsigned long);
extern "C" int   _ZN11AbyssEngine6AEFile4ReadEjPvj(int size, void *dst, unsigned int file);
extern "C" void  _ZN11AbyssEngine6AEFile5CloseEj(unsigned int file);
extern "C" unsigned short *_ZN11AbyssEngine6String15getWCharFromUtf8EPcj(char *utf8, unsigned int len);
void  GameText_convertStringFromArabic(void *out, int pad, void *in);

// ---- release_8174e.cpp ----
// GameText::release() -- destroys each owned text-table entry via its vtable[1], then nulls it.
void GameText::release() {
    GameText *self = this;
    void **data = (void **)self->textTable;
    if (data == 0) return;
    int i = 0;
    for (int byteoff = 0; i < i32(self, 0x1c); byteoff += 4) {
        void *obj = *(void **)((char *)self->textTable + byteoff);
        void **slot;
        if (obj == 0) {
            slot = (void **)((char *)self->textTable + i * 4);
        } else {
            void (**vt)(void *) = *(void (***)(void *))obj;
            vt[1](obj);
            slot = (void **)((char *)self->textTable + byteoff);
        }
        *slot = 0;
        ++i;
    }
}

// ---- getLanguage_81790.cpp ----
// *g_GameText_language -> current language id (signed short).

// GameText::getLanguage() -> (int)current language id.
int GameText_getLanguage()
{
    return *g_GameText_language;
}

// ---- getRegionCode_7efcc.cpp ----
// PC-relative region-code C string.
extern const char gRegionCodeStr[] __attribute__((visibility("hidden")));


// GameText::getRegionCode() -> returns an AbyssEngine::String by value (sret).
RetStr GameText_getRegionCode()
{
    RetStr r;
    AEString_ctor_cstr(&r, gRegionCodeStr, false);
    return r;
}

// ---- setSubstituteArray_817a0.cpp ----
// GameText::setSubstituteArray(int*, unsigned) -- replaces the substitute Array<int> at this+0.
void GameText::setSubstituteArray(int *param_1, unsigned param_2) {
    GameText *self = this;
    Arr *a = (Arr *)self;
    if (param_2 != 0) {
        if ((param_2 & 1) != 0) return;
        ArrayRemoveAll_int(a);
    }
    for (; param_2 != 0; --param_2) {
        int v = *param_1++;
        ArrayAdd_int(v, a);
    }
}

// ---- setLanguage_7f2c0.cpp ----
// GameText::setLanguage(short stringCount, int langId)
// Selects the active UI language. If the language is unchanged it does nothing; otherwise it
// releases the previous text table, allocates a fresh String*[stringCount] (nulled), resolves
// the per-language .lang filename via a switch, records the language code (Arabic-shaping
// languages keep their id, the rest map to 0), falls back to the English file when the chosen
// one is missing, then opens it and streams the records in through ReadLangFile.
//
// Exported as GameText_setLanguage_si to match the forwarder in setLanguage_7f2b8.cpp.



// Active language code slot (compared against the requested id to detect a no-op).
__attribute__((visibility("hidden"))) extern unsigned short *g_langCode;
// Per-language file paths; index 0..15 plus a default and an English fallback.
__attribute__((visibility("hidden"))) extern const char *gLangPaths[17];
__attribute__((visibility("hidden"))) extern const char gLangPathDefault[];
__attribute__((visibility("hidden"))) extern const char gLangPathEnglish[];

void GameText::setLanguage_si(int stringCount, int langId) {
    GameText *self = this;
    if ((unsigned int)*g_langCode == ((unsigned int)langId & 0xffff))
        return;

    ((GameText *)(self))->release();
    i32(self, 0x1c) = stringCount;

    String **table = (String **)operator new[]((uint32_t)((unsigned long long)stringCount * 4));
    self->textTable = table;
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
    }
    *g_langCode = code;

    // Fall back to English when the selected file is absent.
    if (_ZN11AbyssEngine6AEFile9FileExistERKNS_6StringE(&path) == 0) {
        operator delete[](AEString_GetAEChar(&path));
        String eng;
        AEString_ctor_cstr(&eng, gLangPathEnglish, false);
        *g_langCode = 0;
    }

    unsigned int file = 0;
    _ZN11AbyssEngine6AEFile8OpenReadERKNS_6StringEPj(&path, &file);
    ((GameText *)(self))->ReadLangFile(0, stringCount);
}

// ---- _GameText_81726.cpp ----
// Tail veneer to the base/Array<int> destructor; takes and returns this.

// GameText::~GameText() -> releases owned text table + string, tail-calls base dtor.
GameText *_ZN8GameTextD2Ev(GameText *self)
{
    ((GameText *)(self))->release();
    void *p = self->textTable;
    if (p != 0) operator_delete_arr(p);
    self->textTable = 0;
    return GameText_dtor_tail(self);
}

// ---- isNonArabicString_7f798.cpp ----
// Arabic codepoint table: 0x29 rows x 5 u32 columns (PC-relative base address).
extern unsigned gArabicTable[] __attribute__((visibility("hidden")));

// GameText::isNonArabicString(unsigned short const*, unsigned) -> 1 if no char is in the table.
int GameText_isNonArabicString(const unsigned short *param_1, unsigned param_2)
{
    unsigned short i = 0;
    bool keep;
    do {
        if (param_2 <= i) return 1;
        keep = true;
        unsigned short ch = param_1[i];
        unsigned *row = gArabicTable;
        for (int r = 0; r != 0x29; row += 5, ++r) {
            for (int c = 0; c != 5; ++c)
                keep = keep & (row[c] != ch);
        }
        ++i;
    } while (keep);
    return 0;
}

// ---- GameText_8165c.cpp ----
// PC-relative: initial language string; and a short* whose target is reset to 0xffff.
extern const char gInitLangStr[] __attribute__((visibility("hidden")));

// GameText::GameText() -- inits substitute Array<int>, region String, default language string.
void GameText::ctor() {
    GameText *self = this;
    Array_int_ctor(self);
    AEString_ctor_default((char *)self + 0x10);
    *g_GameText_langReset = 0xffff;
    self->textTable = 0;
    i32(self, 0x1c) = 0;
    String tmp;
    AEString_ctor_cstr(&tmp, gInitLangStr, false);
}

// ---- convertStringFromArabic_7efe0.cpp ----
// GameText::convertStringFromArabic(String in) -> String (sret)
// Reshapes a logical-order Arabic string into the presentation forms the bitmap font expects.
// Walks the code points from the end toward the start; for each Arabic letter it picks the
// isolated / initial / medial / final glyph based on the joining behaviour of its neighbours,
// and collapses the LAM (U+0644) + ALEF family into the appropriate ligature, splicing the
// string around the merged pair. Substitution data lives in two extern tables:
//   gArabForms : rows of 5 u32 {base, isolated, final, initial, medial}, terminated implicitly
//                by the 0x334-byte span (41 rows).
//   gLamAlef   : rows of 5 u32 {base, ...} for the 10 LAM-ALEF combinations.
//
// out is sret; `pad` is the hidden alignment argument from the {String} by-value ABI.

extern "C" unsigned short *AEString_data(void *s);                 // operator cast to ushort*
extern "C" unsigned short *AEString_index(void *s, unsigned int i);// operator[](i)
extern "C" void  AEString_append(void *dst, void *src);           // operator+=


// Substitution tables (see header comment for row layout).
__attribute__((visibility("hidden"))) extern const unsigned int gArabForms[];  // 41 rows * 5
__attribute__((visibility("hidden"))) extern const unsigned int gLamAlef[];     // first-letter map (10)
__attribute__((visibility("hidden"))) extern const unsigned int gLamAlefForms[];// 0x29 rows * 5

// True for code points that participate in Arabic joining as a "letter".
static inline bool isJoiner(unsigned short c) {
    return c >= 0x600 && c != 0x60c && c != 0x61f;
}

void GameText_convertStringFromArabic(void *out, int pad, void *in)
{
    (void)pad;

    // Working copy of the wide buffer (length + NUL).
    String work;
    AEString_ctor_wide(&work, AEString_data(in), false);
    unsigned int len = u32(&work, 8);          // character count

    unsigned int cap = (len + 1) * 2;
    unsigned short *buf = (unsigned short *)operator new[](cap);
    memcpy(buf, AEString_data(&work), (unsigned long)(len * 2 + 2));

    int inLen = i32(in, 8);
    unsigned int hi = (unsigned int)(inLen - 1);
    unsigned int i = hi;

    while (true) {
        if (i > 0x7fffffff) {
            // Underflowed past 0: finished. Emit the reshaped buffer.
            AEString_ctor_wide(out, buf, false);
            if (buf != 0)
                operator delete[](buf);
            return;
        }

        if (i != 0) {
            unsigned short prev = *AEString_index(in, i - 1);
            unsigned short cur  = *AEString_index(in, i);
            if (prev == 0x644) {
                // LAM + ALEF-family ligature.
                unsigned short form = 2;
                switch (cur) {
                case 0x622: form = 5; break;
                case 0x623: form = 3; break;
                case 0x627: form = 2; break;
                default: break;
                }
                buf[i] = form;

                // Splice: keep [0,i-1) and [i+1,end), join via SubString + operator+=.
                String merged;
                AEString_ctor_wide(&merged, buf, false);
                String head, tail;
                AEString_append(&head, &tail);

                if (buf != 0)
                    operator delete[](buf);

                unsigned int mLen = u32(&head, 8);
                unsigned int mCap = (mLen + 1) * 2;
                buf = (unsigned short *)operator new[](mCap);
                memcpy(buf, AEString_data(&head), (unsigned long)(mLen * 2 + 2));
                --i;
            }

            // Map the LAM-ALEF result through the secondary form table.
            unsigned int k = 0;
            for (; k < 10; ++k)
                if (gLamAlef[k * 4] == (unsigned int)prev)
                    break;
            if (k < 10) {
                for (unsigned int r = 0; r < 0x29; ++r) {
                    const unsigned int *row = &gLamAlefForms[r * 5];
                    if (row[0] == (unsigned int)buf[i]) {
                        int form = 1;
                        if ((int)i < (int)hi && isJoiner(buf[i + 1]))
                            form = isJoiner(buf[i + 1]) ? 4 : 1;
                        buf[i] = (unsigned short)row[form];
                        break;
                    }
                }
            }
        } else {
            // i == 0: standalone first letter contextual form.
            for (unsigned int r = 0; r < 0x334 / 20; ++r) {
                const unsigned int *row = &gArabForms[r * 5];
                if (row[0] == (unsigned int)buf[0]) {
                    int form;
                    if (inLen == 1 || buf[1] < 0x600) {
                        form = 1;
                    } else {
                        unsigned short nxt = buf[1];
                        form = (nxt == 0x60c || nxt == 0x61f) ? 1 : 4;
                    }
                    buf[0] = (unsigned short)row[form];
                    break;
                }
            }
        }

        --i;
    }
}

// ---- setLanguage_7f2b8.cpp ----
// Tail veneer to GameText::setLanguage(short, int).

// GameText::setLanguage(int) -> forwards to setLanguage(0, param_1).
void GameText::setLanguage_i(int param_1) {
    GameText *self = this;
    return ((GameText *)(self))->setLanguage_si(0, param_1);
}

// ---- getText_81818.cpp ----
// GameText::getText(int key)
// Resolves a text key to a String*. Two reserved keys return guarded function-local statics
// built from the active language/region code (5000 = language name, 5001 = region name).
// Any other key is first remapped through the substitute pair-table (+0x04, count at +0x00),
// then looked up in the text table (ptr at +0x0c, count at +0x1c); a missing entry falls back
// to the embedded fallback String at +0x10.
//
// NB: signature is getText(int) on `this` in r0; we model it as (self, key).


// Active language code (1 => use the primary string, otherwise the fallback variant).
__attribute__((visibility("hidden"))) extern unsigned short *g_langCode;
// Reserved-key string literals (primary + fallback) for the two guarded statics.
__attribute__((visibility("hidden"))) extern const char gLang5000Primary[];
__attribute__((visibility("hidden"))) extern const char gLang5000Fallback[];
__attribute__((visibility("hidden"))) extern const char gLang5001Primary[];
__attribute__((visibility("hidden"))) extern const char gLang5001Fallback[];

void * GameText::getText(int key) {
    GameText *self = this;
    if (key == 5000) {
        static String s5000;   // function-local guarded static
        const char *txt = (*g_langCode != 1) ? gLang5000Fallback : gLang5000Primary;
        AEString_ctor_cstr(&s5000, txt, false);
        return &s5000;
    }
    if (key == 5001) {
        static String s5001;
        const char *txt = (*g_langCode != 1) ? gLang5001Fallback : gLang5001Primary;
        AEString_ctor_cstr(&s5001, txt, false);
        return &s5001;
    }

    // Remap via the substitute pair-table: entries are (from,to) int pairs.
    uint32_t pairCount = u32(self, 0x00);
    int *pairs = (int *)self->substituteData;
    for (uint32_t i = 0; i < pairCount; i += 2) {
        if (pairs[i] == key) {
            key = pairs[i + 1];
            break;
        }
    }

    if (key >= 0 && key < i32(self, 0x1c) && self->textTable != 0) {
        String **table = (String **)self->textTable;
        String *s = table[key];
        if (s != 0)
            return s;
    }
    return (String *)((char *)self + 0x10);
}

// ---- ReadLangFile_7f64c.cpp ----
// GameText::ReadLangFile(unsigned int file, int count)
// Reads `count` records from an opened .lang file. Each record is a big-endian u16 byte
// length followed by that many UTF-8 bytes; the bytes are decoded to wide chars, wrapped in
// a heap String (run through convertStringFromArabic when the active language is Arabic == 9),
// and stored into the text table at +0x0c. On a short read the table is released and the file
// is closed. file == 0 is a no-op early exit.

extern "C" void  AEString_ctor_cstr(void *s, const char *text, bool copy);     // wchar ctor (text*, copy)


// Active language code; 9 == Arabic.
__attribute__((visibility("hidden"))) extern unsigned short *g_langCode;

void GameText::ReadLangFile(unsigned int file, int count) {
    GameText *self = this;
    if (file == 0)
        return;

    short lang = *g_langCode;
    for (int i = 0; i < count; ++i) {
        unsigned short len;
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &len, file) == 0) {
            ((GameText *)(self))->release();
            break;
        }
        // Length field is stored big-endian; byte-swap to host order.
        unsigned int byteLen = (unsigned int)((len >> 8) | (len << 8)) & 0xffff;

        char *utf8 = (char *)operator new[](byteLen + 1);
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj((int)byteLen, utf8, file) == 0) {
            operator delete[](utf8);
            ((GameText *)(self))->release();
            break;
        }
        utf8[byteLen] = '\0';

        unsigned short *wide = _ZN11AbyssEngine6String15getWCharFromUtf8EPcj(utf8, byteLen);

        String *s = (String *)operator new(0xc);
        String **table = (String **)self->textTable;
        if (lang == 9) {
            String tmp;
            AEString_ctor_wide(&tmp, wide, false);
            GameText_convertStringFromArabic(s, 0, &tmp);
            table[i] = s;
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
