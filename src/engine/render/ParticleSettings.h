#ifndef GOF2_PARTICLESETTINGS_H
#define GOF2_PARTICLESETTINGS_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include <cstddef>
#include <type_traits>

class ParticleSettings {
public:
    enum ParticleSet {
        ParticleSet_dummy = 0,
        ParticleSet_4 = 4, ParticleSet_7 = 7, ParticleSet_8 = 8, ParticleSet_9 = 9,
        ParticleSet_0xa = 0xa, ParticleSet_0xb = 0xb, ParticleSet_0xc = 0xc, ParticleSet_0xf = 0xf,
        ParticleSet_0x14 = 0x14, ParticleSet_0x15 = 0x15, ParticleSet_0x16 = 0x16,
        ParticleSet_0x17 = 0x17, ParticleSet_0x18 = 0x18, ParticleSet_0x19 = 0x19,
        ParticleSet_0x1a = 0x1a, ParticleSet_0x1b = 0x1b, ParticleSet_0x1c = 0x1c,
        ParticleSet_0x27 = 0x27, ParticleSet_0x2a = 0x2a, ParticleSet_0x2f = 0x2f
    };

    enum CameraSet { CameraSet_dummy = 0, CameraSet_1 = 1 };

    struct {
        String name;
        uint32_t flags;
        int32_t count;
        float lifeBase;
        int32_t lifeRandom;
        float startSize;
        float endSize;
        float velocityFromSlot;
        int32_t lifetime;
        float flLifetime;
        uint32_t oneShot;
        uint32_t color0;
        uint32_t color1;
        int32_t fadeFrames;
        float colorFlag;
        int32_t posBase;
        int32_t posSpread;
        int32_t ySpread;
        int32_t velSpread;
        uint32_t field_0x54;
        float velBaseX;
        float velBaseY;
        float velBaseZ;
        float drag;
        float velRight;
        float velUp;
        float velDir;
        uint32_t field_0x74;
        float posRight;
        float posUp;
        float posDir;
        float posDirRandom;
        float uvU0;
        float uvV0;
        float uvU1;
        float uvV1;
        int32_t speedThreshold;
        int32_t frames;
    } sets[48];

    using SetDefinition = std::remove_reference<decltype(sets[0])>::type;

    // Authoritative layout: recovered from the original ParticleSettings::init() store
    // offsets (0x184020) and the field reads in IParticleSystem / ParticleSystemMesh /
    // ParticleSystemSprite / Level::setPlayerEngineColor.  Each set is 0xa0 bytes and the
    // 48-entry table is 0x1e00 bytes, matching ParticleSettingsRef::cur/init in the
    // original .bss (nm: 0x1e00 each).
    static_assert(sizeof(SetDefinition) == 0xa0, "ParticleSettings set stride must be 0xa0");
    static_assert(offsetof(SetDefinition, name) == 0x00, "name");
    static_assert(offsetof(SetDefinition, flags) == 0x0c, "flags");
    static_assert(offsetof(SetDefinition, count) == 0x10, "count");
    static_assert(offsetof(SetDefinition, lifeBase) == 0x14, "lifeBase");
    static_assert(offsetof(SetDefinition, lifeRandom) == 0x18, "lifeRandom");
    static_assert(offsetof(SetDefinition, startSize) == 0x1c, "startSize");
    static_assert(offsetof(SetDefinition, endSize) == 0x20, "endSize");
    static_assert(offsetof(SetDefinition, velocityFromSlot) == 0x24, "velocityFromSlot");
    static_assert(offsetof(SetDefinition, lifetime) == 0x28, "lifetime");
    static_assert(offsetof(SetDefinition, flLifetime) == 0x2c, "flLifetime");
    static_assert(offsetof(SetDefinition, oneShot) == 0x30, "oneShot");
    static_assert(offsetof(SetDefinition, color0) == 0x34, "color0");
    static_assert(offsetof(SetDefinition, color1) == 0x38, "color1");
    static_assert(offsetof(SetDefinition, fadeFrames) == 0x3c, "fadeFrames");
    static_assert(offsetof(SetDefinition, colorFlag) == 0x40, "colorFlag");
    static_assert(offsetof(SetDefinition, posBase) == 0x44, "posBase");
    static_assert(offsetof(SetDefinition, posSpread) == 0x48, "posSpread");
    static_assert(offsetof(SetDefinition, ySpread) == 0x4c, "ySpread");
    static_assert(offsetof(SetDefinition, velSpread) == 0x50, "velSpread");
    static_assert(offsetof(SetDefinition, field_0x54) == 0x54, "field_0x54");
    static_assert(offsetof(SetDefinition, velBaseX) == 0x58, "velBaseX");
    static_assert(offsetof(SetDefinition, velBaseY) == 0x5c, "velBaseY");
    static_assert(offsetof(SetDefinition, velBaseZ) == 0x60, "velBaseZ");
    static_assert(offsetof(SetDefinition, drag) == 0x64, "drag");
    static_assert(offsetof(SetDefinition, velRight) == 0x68, "velRight");
    static_assert(offsetof(SetDefinition, velUp) == 0x6c, "velUp");
    static_assert(offsetof(SetDefinition, velDir) == 0x70, "velDir");
    static_assert(offsetof(SetDefinition, field_0x74) == 0x74, "field_0x74");
    static_assert(offsetof(SetDefinition, posRight) == 0x78, "posRight");
    static_assert(offsetof(SetDefinition, posUp) == 0x7c, "posUp");
    static_assert(offsetof(SetDefinition, posDir) == 0x80, "posDir");
    static_assert(offsetof(SetDefinition, posDirRandom) == 0x84, "posDirRandom");
    static_assert(offsetof(SetDefinition, uvU0) == 0x88, "uvU0");
    static_assert(offsetof(SetDefinition, uvV0) == 0x8c, "uvV0");
    static_assert(offsetof(SetDefinition, uvU1) == 0x90, "uvU1");
    static_assert(offsetof(SetDefinition, uvV1) == 0x94, "uvV1");
    static_assert(offsetof(SetDefinition, speedThreshold) == 0x98, "speedThreshold");
    static_assert(offsetof(SetDefinition, frames) == 0x9c, "frames");

    ParticleSettings();

    void init();

    static void multiplyAll(float scale);

    static void Interpolate(ParticleSet a, ParticleSet b, float t, ParticleSet out);

    static float particleMultiply;
    static int pCounter;
};
static_assert(sizeof(ParticleSettings) == 0x1e00, "ParticleSettings table must be 0x1e00");

#endif
