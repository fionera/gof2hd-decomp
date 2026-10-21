#ifndef GOF2_PARTICLESETTINGSREF_H
#define GOF2_PARTICLESETTINGSREF_H
#include "engine/render/ParticleSettings.h"

// Live (cur) and pristine (init) particle set tables; storage defined in ParticleSettings.cpp,
// constructed by its static initializer (the original's __GLOBAL__sub_I_ParticleSettings.cpp
// calls ParticleSettings() on both and registers the implicit ~ParticleSettings with atexit).
class ParticleSettingsRef {
public:
    static void initialize() {
        cur.init();
        init.init();
        assertInit = 42;
    }

    // Raw byte view of the live table, for the particle systems' offset-based field reads.
    static unsigned char *curBytes() { return reinterpret_cast<unsigned char *>(&cur); }

    static int assertInit;
    static ParticleSettings cur;
    static ParticleSettings init;
};

#endif
