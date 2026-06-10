#ifndef GOF2_MESH_H
#define GOF2_MESH_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine::Mesh (Android libgof2hdaa.so, armv7 Thumb).
// (Namespace confirmed from work-item Sig: AbyssEngine::Mesh.)
//
// We do NOT model the full Mesh layout; fields are accessed by byte offset taken from
// each work-item's target disassembly. Known offsets used here:
//   +0x00 (byte) flag           +0x02 (u16)
//   +0x3c..0x50  BSphere (center+radius), with +0x40 swapped with +0x44 on load
//   +0x34  Transform* animation track
//   +0x50  embedded Vector
//   +0x85  (byte) hasAnimation flag


void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

namespace AbyssEngine {

struct Transform;

namespace AEMath {
struct BSphere;
struct Vector;
}

// AbyssEngine::Mesh — renderable mesh with optional animation track.
struct Mesh {
    uint8_t field_0x0;                  // +0x0
    uint16_t field_0x2;                 // +0x2
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    uint32_t field_0xc;                 // +0xc
    void* field_0x10;                   // +0x10
    uint32_t field_0x14;                // +0x14
    uint32_t field_0x18;                // +0x18
    uint32_t field_0x1c;                // +0x1c
    uint32_t field_0x20;                // +0x20
    uint16_t field_0x28;                // +0x28
    uint16_t field_0x2a;                // +0x2a
    void* field_0x2c;                   // +0x2c
    void* field_0x30;                   // +0x30
    Transform* field_0x34;              // +0x34 animation track
    uint8_t field_0x38;                 // +0x38
    float field_0x3c;                   // +0x3c BSphere center x
    float field_0x40;                   // +0x40 BSphere center y
    float field_0x44;                   // +0x44 BSphere center z
    float field_0x48;                   // +0x48 BSphere radius
    float field_0x4c;                   // +0x4c
    float field_0x50;                   // +0x50 embedded Vector x
    float field_0x54;                   // +0x54 embedded Vector y
    uint8_t field_0x5c;                 // +0x5c
    unsigned int field_0x60;            // +0x60
    uint32_t field_0x64;                // +0x64
    unsigned int field_0x68;            // +0x68
    uint32_t field_0x6c;                // +0x6c
    uint32_t field_0x70;                // +0x70
    uint32_t field_0x74;                // +0x74
    uint32_t field_0x78;                // +0x78
    uint32_t field_0x7c;                // +0x7c
    uint32_t field_0x80;                // +0x80
    uint8_t field_0x84;                 // +0x84
    uint8_t field_0x85;                 // +0x85

    // ---- methods (converted from free functions) ----
    int ReadEnhancedDataFromFile(unsigned int file, unsigned int flags);
    Mesh * ctor(Mesh *src);
};

} // namespace AbyssEngine

#endif
