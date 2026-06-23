#include "engine/render/ParticleSettingsRef.h"
#include "engine/render/ParticleSettings.h"

static ParticleSettings *g_PSR_settingsA = nullptr;
static ParticleSettings *g_PSR_settingsB = nullptr;
static int g_PSR_counter = 0;

void ParticleSettingsRef::initialize() {
    g_PSR_settingsA->init();
    g_PSR_settingsB->init();
    g_PSR_counter = 0x2a;
}
