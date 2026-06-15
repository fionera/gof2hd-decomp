#ifndef GOF2_KEYFRAME_H
#define GOF2_KEYFRAME_H

#include "gof2/common.h"
#include "gof2/math.h"

namespace AbyssEngine {

// A single animation key: two transform vectors plus a blend weight.
class KeyFrame {
public:
    unsigned char _pad_00[12];   // 0x00..0x0c
    AEMath::Vector scaleA;       // 0x0c  scale-ish vector A (default 1,1,1)
    unsigned char _pad_18[24];   // 0x18..0x30
    AEMath::Vector scaleB;       // 0x30  scale-ish vector B (default 1,1,1)
    unsigned char _pad_3c[12];   // 0x3c..0x48
    float flWeight;              // 0x48  blend weight (default 1.0)
    unsigned char _pad_4c[4];    // 0x4c..0x50
    uint32_t field_0x50;
    uint32_t field_0x54;
    uint32_t field_0x58;
    uint32_t field_0x5c;

    KeyFrame();
};

} // namespace AbyssEngine

#endif
