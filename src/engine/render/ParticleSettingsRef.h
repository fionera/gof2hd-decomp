#ifndef GOF2_PARTICLESETTINGSREF_H
#define GOF2_PARTICLESETTINGSREF_H
#include "engine/render/ParticleSettings.h"

// Live (cur) and pristine (init) particle set tables; storage defined in ParticleSettings.cpp.
class ParticleSettingsRef {
public:
    static void initialize() {
        reinterpret_cast<ParticleSettings &>(cur).init();
        reinterpret_cast<ParticleSettings &>(init).init();
        assertInit = 42;
    }

    static int assertInit;
    static unsigned char cur[7680];
    static unsigned char init[7680];
};

#endif
