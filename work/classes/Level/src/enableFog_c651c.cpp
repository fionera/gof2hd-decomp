#include "class.h"

extern "C" void Level_enableFog_call(int mgr, int sys, bool enable);

void Level::enableFog(bool enable) {
    return Level_enableFog_call(particleSystemMgr, field_284, enable);
}
