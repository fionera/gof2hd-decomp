#include "engine/render/ParticleSettingsRef.h"
#include "engine/render/ParticleSettings.h"

extern ParticleSettings *g_PSR_settingsA;
extern ParticleSettings *g_PSR_settingsB;
extern int g_PSR_counter;

void ParticleSettingsRef::initialize() {
    g_PSR_settingsA->init();
    g_PSR_settingsB->init();
    g_PSR_counter = 0x2a;
}
