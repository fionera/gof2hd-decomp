#ifndef GOF2_PARTICLESETTINGS_H
#define GOF2_PARTICLESETTINGS_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 - particle-emitter settings table.
// ParticleSettings is an array of 48 ParticleSet parameter blocks (0xa0 bytes each,
// 0x1e00 bytes total). Every ParticleSet begins with an AbyssEngine::String name, so the
// embedded String members construct/destruct automatically with the array.

// One particle-emitter parameter block (0xa0 bytes). Most slots hold either an int or a
// float bit pattern depending on the field, so the generic slots stay uint32_t and are
// reinterpreted where used as floats.
struct ParticleSet {
    String   name;          // emitter/texture name
    uint32_t flags;
    int32_t  count;
    uint32_t field_0x14;
    uint32_t field_0x18;
    uint32_t field_0x1c;
    uint32_t field_0x20;
    uint32_t field_0x24;
    int32_t  lifetime;
    uint32_t flLifetime;
    uint32_t field_0x30;
    uint32_t field_0x34;
    uint32_t field_0x38;
    uint32_t field_0x3c;
    uint32_t field_0x40;
    uint32_t field_0x44;
    uint32_t field_0x48;
    uint32_t field_0x4c;
    uint32_t field_0x50;
    uint32_t field_0x54;
    uint32_t field_0x58;
    uint32_t field_0x5c;
    uint32_t field_0x60;
    uint32_t field_0x64;
    uint32_t field_0x68;
    uint32_t field_0x6c;
    uint32_t field_0x70;
    uint32_t field_0x74;
    uint32_t field_0x78;
    uint32_t field_0x7c;
    uint32_t field_0x80;
    uint32_t field_0x84;
    uint32_t field_0x88;
    uint32_t field_0x8c;
    uint32_t field_0x90;
    uint32_t field_0x94;
    uint32_t field_0x98;
    uint32_t field_0x9c;
};

class ParticleSettings {
public:
    ParticleSet sets[48];

    ParticleSettings();
    ~ParticleSettings();

    // Populate the 48 ParticleSet entries with the built-in emitter presets.
    int init();

    // Scale lifetimes and colour components of every ParticleSet by `scale` (or its
    // reciprocal, depending on per-set flag bits).
    void multiplyAll(float scale);

    // out = b*t + (1-t)*a, field by field, over the lifetime/colour/scale slots.
    void Interpolate(int a, int b, float t, int out);
};
#endif
