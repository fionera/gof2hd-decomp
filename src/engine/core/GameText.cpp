#include "engine/core/GameText.h"
GameText* gGameText = nullptr;        // canonical localized-text singleton
#include "engine/file/AEFile.h"
#include "platform/libc.h"



extern short *g_GameText_language;
extern unsigned short *g_GameText_langReset;

// Destroys each owned text-table entry, then nulls the slot.
void GameText::release() {
    if (this->textTable == nullptr)
        return;
    for (int i = 0; i < this->textCount; ++i) {
        delete this->textTable[i];
        this->textTable[i] = nullptr;
    }
}

// Current active language id (signed short).
int GameText::getLanguage()
{
    return *g_GameText_language;
}

// PC-relative region-code C string.
extern const char gRegionCodeStr[];

// Returns the region code as an AbyssEngine::String by value.
String GameText::getRegionCode()
{
    return String(gRegionCodeStr);
}

// Replaces the substitute key-remap table with `count` int values (interpreted as from/to pairs).
void GameText::setSubstituteArray(int* pairs, unsigned count) {
    if (count != 0) {
        if ((count & 1) != 0) return;
        ArrayRemoveAll<int>(this->substitutes);
    }
    for (; count != 0; --count) {
        int v = *pairs++;
        this->substitutes.push_back(v);
    }
}

// Active language code slot (compared against the requested id to detect a no-op).
extern unsigned short *g_langCode;
// Per-language file paths; index 0..15 plus a default and an English fallback.
extern const char *gLangPaths[17];
extern const char gLangPathDefault[];
extern const char gLangPathEnglish[];

// Selects the active UI language (the two-argument overload the savegame loader invokes; the
// string-table count is passed as a short). If the language is unchanged it does nothing; otherwise
// it releases the previous text table, allocates a fresh String*[stringCount] (nulled), resolves the
// per-language .lang filename via a switch, records the language code (Arabic-shaping languages
// keep their id, the rest map to 0), falls back to the English file when the chosen one is missing,
// then opens it and streams the records in through ReadLangFile.
void GameText::setLanguage(short stringCount, int langId) {
    if ((unsigned int)*g_langCode == ((unsigned int)langId & 0xffff))
        return;

    this->release();
    this->textCount = stringCount;
    this->textTable = new String*[stringCount]();

    String path(gLangPathDefault);

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
    path = String(p);
    *g_langCode = code;

    // Fall back to English when the selected file is absent.
    if (AEFile::FileExist(path) == 0) {
        path = String(gLangPathEnglish);
        *g_langCode = 0;
    }

    unsigned int file = 0;
    AEFile::OpenRead(path, &file);
    this->ReadLangFile(0, stringCount);
}

// Releases the text table and frees it; the embedded Array<int> and fallback String are destroyed
// by the implicit member teardown.
GameText::~GameText()
{
    this->release();
    delete[] this->textTable;
    this->textTable = nullptr;
}

// Arabic codepoint table: 0x29 rows x 5 u32 columns.
extern unsigned gArabicTable[];

// Returns 1 if no character of `str` appears in the Arabic codepoint table.
int GameText::isNonArabicString(const unsigned short *str, unsigned int count)
{
    unsigned short i = 0;
    bool keep;
    do {
        if (count <= i) return 1;
        keep = true;
        unsigned short ch = str[i];
        unsigned *row = gArabicTable;
        for (int r = 0; r != 0x29; row += 5, ++r) {
            for (int c = 0; c != 5; ++c)
                keep = keep & (row[c] != ch);
        }
        ++i;
    } while (keep);
    return 0;
}

// PC-relative: initial language string.
extern const char gInitLangStr[];

// Initializes the substitute table, fallback String, and default language string.
GameText::GameText() {
    this->fallbackText.ctor();
    *g_GameText_langReset = 0xffff;
    this->textTable = nullptr;
    this->textCount = 0;
    String tmp(gInitLangStr);
}

// Substitution tables (see body for row layout).
extern const unsigned int gArabForms[];      // 41 rows * 5 {base, isolated, final, initial, medial}
extern const unsigned int gLamAlef[];        // first-letter map (10)
extern const unsigned int gLamAlefForms[];   // 0x29 rows * 5

// True for code points that participate in Arabic joining as a "letter".
static inline bool isJoiner(unsigned short c) {
    return c >= 0x600 && c != 0x60c && c != 0x61f;
}

// Reshapes a logical-order Arabic string into the presentation forms the bitmap font expects.
// Walks the code points from the end toward the start; for each Arabic letter it picks the
// isolated / initial / medial / final glyph based on the joining behaviour of its neighbours, and
// collapses the LAM (U+0644) + ALEF family into the appropriate ligature, splicing the string
// around the merged pair. The input string is taken by value and the reshaped result is returned.
String GameText::convertStringFromArabic(String in)
{
    // Working copy of the wide buffer (length + NUL).
    String work;
    work.ctor_wchar(in.index(0), false);
    unsigned int len = work.size();            // character count

    unsigned short *buf = new unsigned short[len + 1];
    memcpy(buf, work.index(0), (unsigned long)(len * 2 + 2));

    int inLen = (int)in.size();
    unsigned int hi = (unsigned int)(inLen - 1);
    unsigned int i = hi;

    while (true) {
        if (i > 0x7fffffff) {
            // Underflowed past 0: finished. Emit the reshaped buffer.
            String result;
            result.ctor_wchar(buf, false);
            delete[] buf;
            return result;
        }

        if (i != 0) {
            unsigned short prev = *in.index(i - 1);
            unsigned short cur  = *in.index(i);
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

                // Splice: keep [0,i-1) and [i+1,end), join via SubString + addAssign.
                String merged;
                merged.ctor_wchar(buf, false);
                String head, tail;
                head.SubString(&merged, 0, i);
                tail.SubString(&merged, i + 1, merged.size());
                head.addAssign_str(&tail);

                delete[] buf;

                unsigned int mLen = head.size();
                buf = new unsigned short[mLen + 1];
                memcpy(buf, head.index(0), (unsigned long)(mLen * 2 + 2));
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

// Single-argument language switch: forwards to the two-argument overload with a zero count.
void GameText::setLanguage(int langId) {
    this->setLanguage(static_cast<short>(0), langId);
}

// Active language code (1 => use the primary string, otherwise the fallback variant).
// Reserved-key string literals (primary + fallback) for the two guarded statics.
extern const char gLang5000Primary[];
extern const char gLang5000Fallback[];
extern const char gLang5001Primary[];
extern const char gLang5001Fallback[];

// Resolves a text key to a String*. Two reserved keys return guarded function-local statics built
// from the active language/region code (5000 = language name, 5001 = region name). Any other key
// is first remapped through the substitute pair-table, then looked up in the text table; a missing
// entry falls back to the embedded fallback String.
AbyssEngine::String* GameText::getText(int key) {
    if (key == 5000) {
        static String s5000;
        const char *txt = (*g_langCode != 1) ? gLang5000Fallback : gLang5000Primary;
        s5000.ctor_char(txt, false);
        return &s5000;
    }
    if (key == 5001) {
        static String s5001;
        const char *txt = (*g_langCode != 1) ? gLang5001Fallback : gLang5001Primary;
        s5001.ctor_char(txt, false);
        return &s5001;
    }

    // Remap via the substitute pair-table: entries are (from, to) int pairs.
    uint32_t pairCount = (uint32_t)this->substitutes.size();
    int *pairs = this->substitutes.data();
    for (uint32_t i = 0; i < pairCount; i += 2) {
        if (pairs[i] == key) {
            key = pairs[i + 1];
            break;
        }
    }

    if (key >= 0 && key < this->textCount && this->textTable != nullptr) {
        String *s = this->textTable[key];
        if (s != nullptr)
            return s;
    }
    return &this->fallbackText;
}

// Active language code; 9 == Arabic.
// Reads `count` records from an opened .lang file. Each record is a big-endian u16 byte length
// followed by that many UTF-8 bytes; the bytes are decoded to wide chars, wrapped in a heap String
// (run through convertStringFromArabic when the active language is Arabic == 9), and stored into
// the text table. On a short read the table is released and the file is closed. file == 0 is a
// no-op early exit.
void GameText::ReadLangFile(unsigned int file, int count) {
    if (file == 0)
        return;

    short lang = *g_langCode;
    for (int i = 0; i < count; ++i) {
        unsigned short len;
        if (AEFile::Read(2, &len, file) == 0) {
            this->release();
            break;
        }
        // Length field is stored big-endian; byte-swap to host order.
        unsigned int byteLen = (unsigned int)((len >> 8) | (len << 8)) & 0xffff;

        char *utf8 = new char[byteLen + 1];
        if (AEFile::Read(byteLen, utf8, file) == 0) {
            delete[] utf8;
            this->release();
            break;
        }
        utf8[byteLen] = '\0';

        unsigned short *wide = String::getWCharFromUtf8(utf8, (int)byteLen);

        String *s = new String;
        if (lang == 9) {
            String tmp;
            tmp.ctor_wchar(wide, false);
            *s = this->convertStringFromArabic(tmp);
            this->textTable[i] = s;
        } else {
            s->ctor_wchar(wide, false);
            this->textTable[i] = s;
        }

        delete[] wide;
        delete[] utf8;
    }

    AEFile::Close(file);
}
