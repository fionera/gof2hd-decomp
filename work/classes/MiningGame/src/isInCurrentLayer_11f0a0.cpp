#include "class.h"

struct MiningGame {
    // @portable-fields
    unsigned char _pad_0[136];
    void* f_88; // 0x88
    void* f_8c; // 0x8c
    void* f_90; // 0x90
    void* f_94; // 0x94
    unsigned char _pad_98[56];
    void* f_d0; // 0xd0

    bool isInCurrentLayer();
};

__attribute__((visibility("hidden"))) extern void **g_MiningGame_sqrt;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layout;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_layerSizes;

extern "C" float MiningGame_sqrt(void *globals, float value);

bool MiningGame::isInCurrentLayer()
{
    float dx = F(this, 0x10) - (float)I(this, 0x58);
    float dy = F(this, 0x14) - (float)I(this, 0x5c);
    void **sqrtHolder = g_MiningGame_sqrt;
    int current = I(this, 0x78);
    int layer = I(this, 0x7c);
    int *row = g_MiningGame_layerSizes + (7 - layer) * 7;
    void **layoutHolder = g_MiningGame_layout;
    void *globals = *sqrtHolder;
    int size = row[current];
    float scale = F(*layoutHolder, 0xe8);
    float distance = MiningGame_sqrt(globals, dx * dx + dy * dy);
    return distance < scale * (float)(size / 2);
}
