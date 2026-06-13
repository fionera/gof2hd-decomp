#include "gof2/engine/render/ParticleSettingsRef.h"
#include "gof2/engine/render/ParticleSettings.h"

extern "C" __attribute__((visibility("hidden"))) void **g_PSR_settingsA;
extern "C" __attribute__((visibility("hidden"))) void **g_PSR_settingsB;
extern "C" __attribute__((visibility("hidden"))) int *g_PSR_counter;

// ParticleSettingsRef::initialize() — initialize the two referenced ParticleSettings
// singletons and seed a global counter to 0x2a.

void ParticleSettingsRef_initialize()
{
    ((ParticleSettings *)(*g_PSR_settingsA))->init();
    ((ParticleSettings *)(*g_PSR_settingsB))->init();
    *g_PSR_counter = 0x2a;
}
