#include "class.h"

struct Sparks {
    void explode(int x, int y, int z);
};

extern "C" void PaintCanvas_SpriteSystemSetPosition(void *canvas, uint32_t sprite,
                                                     uint16_t index, float x, float y, float z);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_explode_rocket;
__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_explode_single;
__attribute__((visibility("hidden"))) extern void **g_Sparks_random_explode;
__attribute__((visibility("hidden"))) extern int (*g_Sparks_nextInt_explode)(void *rng, int bound);

void Sparks::explode(int x, int y, int z)
{
    int x0 = x;
    int y0 = y;
    int z0 = z;

    if (F<uint8_t>(this, 0x10) != 0)
        return;

    if (F<int>(this, 0x14) == 1) {
        uint32_t i = 0;
        void **canvas = g_Sparks_canvas_explode_rocket;
        void **rng = g_Sparks_random_explode;
        int (*nextInt)(void *, int) = g_Sparks_nextInt_explode;
        for (; i < F<uint32_t>(this, 0x18); i++) {
            uint32_t sprite = F<uint32_t>(this, 0x4);
            void *canvasObj = *canvas;
            float px = (float)(nextInt(*rng, 0x190) + x0);
            float py = (float)(nextInt(*rng, 0x190) + y0);
            float pz = (float)(nextInt(*rng, 0x190) + z0);
            PaintCanvas_SpriteSystemSetPosition(canvasObj, sprite, (uint16_t)i,
                                                px, py, pz);
        }
    } else {
        void **canvas = g_Sparks_canvas_explode_single;
        PaintCanvas_SpriteSystemSetPosition(*canvas, F<uint32_t>(this, 0x4), 0,
                                            (float)x0, (float)y0, (float)z0);
    }

    F<uint8_t>(this, 0x10) = 1;
}
