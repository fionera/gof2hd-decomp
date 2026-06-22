#ifndef GOF2_DIFFICULTYRECORD_H
#define GOF2_DIFFICULTYRECORD_H
#include "aetypes.h"

// The global "difficulty record" the gameplay code consults when scaling enemy
// hitpoints and weapon power to the selected difficulty. Held behind a global
// pointer (g_cs_diffRec / g_ag_diffRec in Level.cpp, g_hardCoreHolder in
// Status.cpp) and only ever accessed through its difficulty scalar at +0x2c.
//
// The record body (0x00..0x2b) is not yet recovered; it is left as opaque
// padding so the one known field lands at its exact offset/width. Status.cpp
// reads the same float as `difficulty` (1.5f == hard-core); Level.cpp reads it
// as the enemy hitpoint / gun-power multiplier, hence the name here.
struct DifficultyRecord {
    uint8_t field_0x0[0x2c]; // opaque record body (unrecovered)
    float hpScale; // +0x2c difficulty scalar (HP/gun-power multiplier)
};

#endif // GOF2_DIFFICULTYRECORD_H
