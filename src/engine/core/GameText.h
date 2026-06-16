#ifndef GOF2_GAMETEXT_H
#define GOF2_GAMETEXT_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/core/String.h"

// Galaxy on Fire 2 -- GameText: the localized UI text table. Loads per-language .lang files,
// resolves text keys to AbyssEngine::String entries, applies an optional substitute key-remap,
// and reshapes Arabic strings into their presentation glyph forms.
class GameText {
public:
    // Substitute key remap table, stored as (from, to) int pairs.
    Array<int> substitutes;
    // Heap-allocated AbyssEngine::String*[textCount] table of resolved entries.
    AbyssEngine::String** textTable;
    // Fallback / region string returned when a key has no entry.
    AbyssEngine::String fallbackText;
    int textCount;

    GameText();
    ~GameText();

    void ReadLangFile(unsigned int file, int count);
    AbyssEngine::String* getText(int key);
    void release();
    void setLanguage_i(int langId);
    void setLanguage_si(int stringCount, int langId);
    void setSubstituteArray(int* pairs, unsigned count);

    // Two-argument language switch the savegame loader invokes (count passed as a short).
    void setLanguage(short stringCount, int langId);
};
extern GameText* gGameText;        // canonical localized-text singleton

#endif
