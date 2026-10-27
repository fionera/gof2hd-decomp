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

// The original exports both tables as 0x1e00-byte .bss objects (nm:
// ParticleSettingsRef::cur / ::init), immediately followed by pCounter and assertInit.
static_assert(sizeof(ParticleSettingsRef::cur) == 0x1e00, "cur table must be 0x1e00");
static_assert(sizeof(ParticleSettingsRef::init) == 0x1e00, "init table must be 0x1e00");

#endif
