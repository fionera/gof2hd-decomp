#ifndef GOF2_BSPHERE_H
#define GOF2_BSPHERE_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// Galaxy on Fire 2 -- AbyssEngine::AEMath::BSphere (Android libgof2hdaa.so, armv7 Thumb).
// (Namespace confirmed from work-item Sig: AbyssEngine::AEMath::BSphere.)
//
// Bounding sphere, 0x14 bytes:
//   +0x00 float cx          +0x04 float cy        +0x08 float cz   (center)
//   +0x0c float radius      +0x10 float maxRadius (a second, looser radius bound)
// Field offsets recovered from the target disassembly; accessed via byte-offset casts.


namespace AbyssEngine {

struct Transform;   // opaque; only ever taken by pointer

namespace AEMath {

class BSphere {
public:
    float cx, cy, cz;   // 0x00, 0x04, 0x08
    float radius;       // 0x0c
    float maxRadius;    // 0x10

    BSphere &operator=(const BSphere &other);
    void Merge(const Transform &t);
    void Merge(const BSphere &other);
};

} // namespace AEMath
} // namespace AbyssEngine
#endif
