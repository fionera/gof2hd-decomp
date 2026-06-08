#include "class.h"

extern "C" int Agent_getRace(void *agent);
extern "C" int Agent_isMale(void *agent);
extern "C" int *Agent_getImageParts(void *agent);

// Static (dialogueCode, soundId) pair table, 47 entries; linear-searched first.
extern const int gGDS_pairTable[] __attribute__((visibility("hidden")));  // DAT_000f5a6c base
// Per-(category) dialogue-code -> sound-id dispatch. category encodes race/gender bucket
// (the disasm's iVar2 switch value 0..5). Returns -1 when unmapped.
extern "C" int Globals_dialogueDispatch(int category, int code);

// Globals::getDialogueSoundId(int code, Agent* agent)
extern "C" int Globals_getDialogueSoundId(void *self, int code, void *agent)
{
    (void)self;
    // Phase 1: linear search of the static pair table (47 pairs).
    const int *t = gGDS_pairTable;
    for (unsigned i = 0; (i >> 6) < 0x2f; i += 2) {
        if (t[i * 2] == code) {
            return t[i * 2 + 1];
        }
    }

    if (agent == 0) {
        return -1;
    }

    int race = Agent_getRace(agent);
    int male = Agent_isMale(agent);

    int category;
    if (race == 3) {
        int *parts = Agent_getImageParts(agent);
        if (parts != 0) {
            int *p = Agent_getImageParts(agent);
            category = (*p == 2) ? 3 : 0;
            // category 0 dispatch differs by gender; fold gender into the bucket id.
            return Globals_dialogueDispatch(male != 0 ? (category + 10) : category, code);
        }
        // No image parts: race-3 fallback uses the "case 2/3" generic dialogue table.
        return Globals_dialogueDispatch(2, code);
    }

    // Non-Klingon races: bucket by race index, gendered.
    category = race;
    return Globals_dialogueDispatch(male != 0 ? (category + 10) : category, code);
}
