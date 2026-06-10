#ifndef GOF2_WAYPOINT_H
#define GOF2_WAYPOINT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Waypoint {
    void* field_0x0;                    // +0x0
    Player* field_0x4;                  // +0x4
    uint8_t field_0x4c;                 // +0x4c
    float field_0x58;                   // +0x58
    float field_0x5c;                   // +0x5c
    float field_0x60;                   // +0x60
    uint8_t field_0x72;                 // +0x72
    int32_t field_0x124;                // +0x124
    int32_t field_0x128;                // +0x128
    int32_t field_0x12c;                // +0x12c
    uint16_t field_0x130;               // +0x130
    Route* field_0x134;                 // +0x134
};
#endif
