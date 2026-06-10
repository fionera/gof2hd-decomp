#ifndef GOF2_PLAYERJUNK_H
#define GOF2_PLAYERJUNK_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct PlayerJunk {
    Player* field_0x4;                  // +0x4
    uint8_t field_0x4c;                 // +0x4c
    void* field_0x50;                   // +0x50
    void* field_0x54;                   // +0x54
    V3 field_0x58;                      // +0x58
    void* field_0x78;                   // +0x78
    int field_0x88;                     // +0x88
    int field_0x124;                    // +0x124
};
#endif
