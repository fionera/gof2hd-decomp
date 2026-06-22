#ifndef GOF2_GAMETEXT_H
#define GOF2_GAMETEXT_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/core/String.h"

class GameText {
public:
    // Substitute key remap table, stored as (from, to) int pairs.
    Array<int> substitutes;
    // Heap-allocated AbyssEngine::String*[textCount] table of resolved entries.
    AbyssEngine::String **textTable;
    // Fallback / region string returned when a key has no entry.
    AbyssEngine::String fallbackText;
    int textCount;

    GameText();

    ~GameText();

    static int getLanguage();

    // Region code returned as an AbyssEngine::String by value.
    static AbyssEngine::String getRegionCode();

    // Returns 1 if no character of `str` appears in the Arabic codepoint table.
    static int isNonArabicString(const unsigned short *str, unsigned int count);

    // Reshapes a logical-order Arabic string into the presentation glyph forms the font expects.
    AbyssEngine::String convertStringFromArabic(AbyssEngine::String in);

    void ReadLangFile(unsigned int file, int count);

    AbyssEngine::String *getText(int key);

    void release();

    void setLanguage(int langId);

    void setSubstituteArray(int *pairs, unsigned count);

    // Two-argument language switch the savegame loader invokes (count passed as a short).
    void setLanguage(short stringCount, int langId);

    // Legacy alias retained for callers that used the disambiguated name.
    inline void setLanguage_i(int langId) { setLanguage(langId); }
};

extern GameText *gGameText; // canonical localized-text singleton

#endif
