#include "class.h"

struct Sparks {
    Sparks(int kind);
};

extern "C" void *operator_new(uint32_t size);
extern "C" void PaintCanvas_SpriteSystemCreate(void *canvas, uint16_t count, bool dynamic,
                                                uint32_t *outSprite);
extern "C" void PaintCanvas_SpriteSystemSetAllUv(void *canvas, uint32_t sprite, float u0,
                                                  float v0, float u1, float v1);
extern "C" void PaintCanvas_SpriteSystemSetSize(void *canvas, uint32_t sprite, uint16_t index,
                                                 int16_t size);
extern "C" int AERandom_nextInt(void *rng, int bound);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_ctor;
__attribute__((visibility("hidden"))) extern void **g_Sparks_random_ctor;

Sparks::Sparks(int kind)
{
    uint32_t count = 5;
    if (kind == 0)
        count = 1;
    uint32_t *outSprite = &F<uint32_t>(this, 0x4);
    void **canvas = g_Sparks_canvas_ctor;
    uint32_t lifetime = 0x514;
    if (kind == 0)
        lifetime = 0x1f4;

    F<int>(this, 0x14) = kind;
    F<uint32_t>(this, 0x18) = count;
    F<uint32_t>(this, 0x1c) = lifetime;

    PaintCanvas_SpriteSystemCreate(*canvas, (uint16_t)count, false, outSprite);
    PaintCanvas_SpriteSystemSetAllUv(*canvas, F<uint32_t>(this, 0x4), 0.626953125f,
                                     0.001953125f, 0.748046875f, 0.123046875f);

    uint32_t n = F<uint32_t>(this, 0x18);
    F<uint8_t>(this, 0x10) = 0;
    F<int *>(this, 0x0) = new int[n];
    F<int>(this, 0x20) = 0;

    if (F<int>(this, 0x14) == 1) {
        void **rng = g_Sparks_random_ctor;
        for (uint32_t i = 0; i < n; i++) {
            PaintCanvas_SpriteSystemSetSize(*canvas, F<uint32_t>(this, 0x4), (uint16_t)i, 1);
            int value = AERandom_nextInt(*rng, 0x1f4);
            F<int *>(this, 0x0)[i] = value;
            n = F<uint32_t>(this, 0x18);
            F<int>(this, 0x20) = value + F<int>(this, 0x20);
        }
    } else {
        F<int *>(this, 0x0)[0] = 0;
        F<int>(this, 0x20) = 0;
    }

    F<int>(this, 0x0c) = 0;
}
