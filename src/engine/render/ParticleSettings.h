#ifndef GOF2_PARTICLESETTINGS_H
#define GOF2_PARTICLESETTINGS_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
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

    ParticleSettings();

    void init();

    static void multiplyAll(float scale);

    static void Interpolate(ParticleSet a, ParticleSet b, float t, ParticleSet out);

    static float particleMultiply;
    static int pCounter;
};
#endif
