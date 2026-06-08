#include "class.h"

// ParticleSettingsRef::initialize() — initialize the two referenced ParticleSettings
// singletons and seed a global counter to 0x2a.
extern "C" void ParticleSettings_init(void *self);
extern "C" __attribute__((visibility("hidden"))) void **g_PSR_settingsA;
extern "C" __attribute__((visibility("hidden"))) void **g_PSR_settingsB;
extern "C" __attribute__((visibility("hidden"))) int *g_PSR_counter;

extern "C" void ParticleSettingsRef_initialize()
{
    ParticleSettings_init(*g_PSR_settingsA);
    ParticleSettings_init(*g_PSR_settingsB);
    *g_PSR_counter = 0x2a;
}
