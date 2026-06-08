#include "class.h"

// GameText::getText(int key)
// Resolves a text key to a String*. Two reserved keys return guarded function-local statics
// built from the active language/region code (5000 = language name, 5001 = region name).
// Any other key is first remapped through the substitute pair-table (+0x04, count at +0x00),
// then looked up in the text table (ptr at +0x0c, count at +0x1c); a missing entry falls back
// to the embedded fallback String at +0x10.
//
// NB: signature is getText(int) on `this` in r0; we model it as (self, key).

extern "C" void AEString_ctor_cstr(void *s, const char *text, bool copy);

// Active language code (1 => use the primary string, otherwise the fallback variant).
__attribute__((visibility("hidden"))) extern short *g_langCode;
// Reserved-key string literals (primary + fallback) for the two guarded statics.
__attribute__((visibility("hidden"))) extern const char gLang5000Primary[];
__attribute__((visibility("hidden"))) extern const char gLang5000Fallback[];
__attribute__((visibility("hidden"))) extern const char gLang5001Primary[];
__attribute__((visibility("hidden"))) extern const char gLang5001Fallback[];

extern "C" void *GameText_getText(void *self, int key)
{
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
    int *pairs = (int *)F<void *>(self, 0x04);
    for (uint32_t i = 0; i < pairCount; i += 2) {
        if (pairs[i] == key) {
            key = pairs[i + 1];
            break;
        }
    }

    if (key >= 0 && key < i32(self, 0x1c) && F<void *>(self, 0x0c) != 0) {
        String **table = (String **)F<void *>(self, 0x0c);
        String *s = table[key];
        if (s != 0)
            return s;
    }
    return (String *)((char *)self + 0x10);
}
