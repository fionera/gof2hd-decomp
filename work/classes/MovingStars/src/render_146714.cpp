#include "class.h"

using AbyssEngine::AEMath::Matrix;

struct MovingStars {
    void render();
};

extern "C" void PaintCanvas_SetTexture(void *canvas, uint32_t texture, uint32_t mode);
extern "C" void PaintCanvas_SetBlendMode(void *canvas, int mode);
extern "C" void PaintCanvas_DrawTransform(void *canvas, uint32_t tf, int z);

__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas_render;

void MovingStars::render()
{
    void **canvas = g_MovingStars_canvas_render;
    PaintCanvas_SetTexture(*canvas, F<uint32_t>(this, 0x8), 0xffffffff);
    PaintCanvas_SetBlendMode(*canvas, 1);
    for (int i = 0; i != 0x32; i = i + 1) {
        PaintCanvas_DrawTransform(*canvas, F<uint32_t *>(this, 0x4)[i], 0);
    }
}
