#ifndef GOF2_AEGEOMETRY_H
#define GOF2_AEGEOMETRY_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct AEGeometry {
    uint16_t field_0x8;                 // +0x8
    V4 field_0x30;                      // +0x30
    uint16_t field_0x48;                // +0x48
    V4 field_0x54;                      // +0x54
    unsigned long long field_0x68;      // +0x68
    uint64_t field_0x70;                // +0x70
    Matrix field_0x84;                  // +0x84
};
#endif
