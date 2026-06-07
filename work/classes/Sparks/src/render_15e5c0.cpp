#include "class.h"

using AbyssEngine::AEMath::Matrix;

struct Sparks {
    void render();
};

extern "C" void PaintCanvas_SetTexture(void *canvas, uint32_t texture, uint32_t mode);
extern "C" void PaintCanvas_SetBlendMode(void *canvas, int mode);
extern "C" void PaintCanvas_SetWorldViewMatrix(void *canvas, Matrix const &m);
extern "C" void PaintCanvas_DrawSpriteSystem(void *canvas, uint32_t sprite);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_render;

void Sparks::render()
{
    Matrix matrix;

    if (F<uint8_t>(this, 0x10) != 0) {
        void **canvas = g_Sparks_canvas_render;
        PaintCanvas_SetTexture(*canvas, F<uint32_t>(this, 0x8), 0xffffffff);
        PaintCanvas_SetBlendMode(*canvas, 2);

        void *canvasObj = *canvas;
        uint32_t *m = (uint32_t *)&matrix;
        m[0] = 0x3f800000;
        m[1] = 0;
        m[2] = 0;
        m[3] = 0;
        m[4] = 0;
        m[5] = 0x3f800000;
        m[6] = 0;
        m[7] = 0;
        m[8] = 0;
        m[9] = 0;
        m[10] = 0x3f800000;
        m[11] = 0;
        m[12] = 0x3f800000;
        m[13] = 0x3f800000;
        m[14] = 0x3f800000;

        PaintCanvas_SetWorldViewMatrix(canvasObj, matrix);
        PaintCanvas_DrawSpriteSystem(*canvas, F<uint32_t>(this, 0x4));
    }
}
