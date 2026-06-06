#include "class.h"

struct ParticleSystemManager {
    static void enableSystemEmit(int mgr, int id);
    static void enableSystemRender(int mgr, int id, bool enable);
};

void Level::enableParticleEffects(bool emit, bool render) {
    ParticleSystemManager::enableSystemEmit(particleSystemMgr, field_284);
    ParticleSystemManager::enableSystemRender(particleSystemMgr, field_284, render);
    *(unsigned char *)particleRenderBoolPtr = render;
    *(unsigned char *)particleEmitBoolPtr = render;
}
