#ifndef WORK_CLASSES_BSPHERE_SRC_CLASS_H
#define WORK_CLASSES_BSPHERE_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine::AEMath::BSphere (Android libgof2hdaa.so, armv7 Thumb).
// (Namespace confirmed from work-item Sig: AbyssEngine::AEMath::BSphere.)
//
// Bounding sphere, 0x14 bytes:
//   +0x00 float cx          +0x04 float cy        +0x08 float cz   (center)
//   +0x0c float radius      +0x10 float maxRadius (a second, looser radius bound)
// Field offsets recovered from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {

struct Transform;   // opaque; only ever taken by pointer

namespace AEMath {

struct BSphere {
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
