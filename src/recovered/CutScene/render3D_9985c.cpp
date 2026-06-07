#include "class.h"

extern "C" uint64_t ApplicationManager_GetElapsedTimeMillis(void *self);
__attribute__((visibility("hidden"))) extern void **g_appManager;
extern "C" void Level_update(void *level, long long time, int z);
extern "C" void Level_render(void *level, uint32_t bg);
extern "C" void AEGeometry_render(void *geom);

void CutScene::render3D()
{
    void *level = pp(this, 0x0);
    if (level != 0) {
        uint32_t t = (uint32_t)ApplicationManager_GetElapsedTimeMillis(*g_appManager);
        u32(this, 0x58) = t;
        Level_update(pp(this, 0x0), (long long)(int)t, 0);
        Level_render(pp(this, 0x0), u32(this, 0x58));
    }
    if (pp(this, 0x28) != 0) AEGeometry_render(pp(this, 0x28));
    if (pp(this, 0x2c) != 0) AEGeometry_render(pp(this, 0x2c));
    if (pp(this, 0x30) != 0) AEGeometry_render(pp(this, 0x30));
    if (pp(this, 0x34) != 0) AEGeometry_render(pp(this, 0x34));

    void *arr = pp(this, 0x38);
    if (arr != 0) {
        for (uint32_t i = 0; i < *(uint32_t *)arr; i++) {
            void **data = (void **)pp(arr, 0x4);
            AEGeometry_render(data[i]);
            arr = pp(this, 0x38);
        }
    }
}
