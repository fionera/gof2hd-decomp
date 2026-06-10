#ifndef GOF2_EXPLOSION_H
#define GOF2_EXPLOSION_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- Explosion class (Android libgof2hdaa.so, armv7 Thumb).
// Real named struct recovered from per-method byte-offset access.

struct AEGeometry;
struct PaintCanvas;
struct TargetFollowCamera;

// Explosion: a transient VFX object that drives one or two AEGeometry meshes
// (plus an optional vector of fire-streak meshes) through their transform
// animations. 0x68 bytes in the target; here laid out as a natural struct.
struct Explosion {
    int                    field_0x0;   // +0x00 explosion type
    AEGeometry            *field_0x4;    // +0x04 primary mesh
    AEGeometry            *field_0x8;    // +0x08 secondary mesh (optional)
    Array<AEGeometry *>   *field_0xc;    // +0x0c fire-streak meshes (optional)
    long long              field_0x10;   // +0x10 total duration
    long long              field_0x18;   // +0x18 elapsed time
    byte                   field_0x20;   // +0x20 playing flag
    float                  field_0x24;   // +0x24 scale
    int                    field_0x28;   // +0x28 sound id (-1 == none)
    Matrix                 field_0x2c;   // +0x2c rotation matrix
};

// Byte-offset accessors for EXTERNAL opaque objects (AEGeometry, Transform,
// PaintCanvas) whose layouts are not part of this class.
static inline int      &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline uint32_t &U(void *p, int off) { return *(uint32_t *)((char *)p + off); }
static inline uint8_t  &UC(void *p, int off) { return *(uint8_t *)((char *)p + off); }
static inline float    &F(void *p, int off) { return *(float *)((char *)p + off); }

#endif
