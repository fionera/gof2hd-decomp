#ifndef GOF2_LENSFLARE_H
#define GOF2_LENSFLARE_H
#include "gof2/common.h"

// Galaxy on Fire 2 -- sun/star lens-flare effect.
// Owns three Image2D sprite handles and renders them along the screen-centre ->
// source axis, tinted from a small palette and faded by distance. Drawing is done
// through the engine PaintCanvas.

namespace AbyssEngine { class PaintCanvas; }

// Engine draw/math helpers reached by address in the original binary. They wrap
// PaintCanvas operations (image creation, colour/blend state, sprite blits) and
// are resolved by a later externs pass.
extern "C" {
void  LensFlare_Image2DCreate(AbyssEngine::PaintCanvas *canvas, short id, uint32_t *out);
int   LensFlare_GetWidth(AbyssEngine::PaintCanvas *canvas);
int   LensFlare_GetHeight(AbyssEngine::PaintCanvas *canvas);

float LensFlare_sqrtf(float v);
int   LensFlare_imgWidth(AbyssEngine::PaintCanvas *canvas, void *img);
int   LensFlare_imgHandle(void *img);
void  LensFlare_setColor(AbyssEngine::PaintCanvas *canvas, uint32_t color);
void  LensFlare_drawScaled(AbyssEngine::PaintCanvas *canvas, void *img, int x, int y);
void  LensFlare_pushState(AbyssEngine::PaintCanvas *canvas);
void  LensFlare_setBlend(AbyssEngine::PaintCanvas *canvas, uint32_t mode);
void  LensFlare_drawFinal(AbyssEngine::PaintCanvas *canvas, void *img, int a, int b);
void  LensFlare_restoreState(AbyssEngine::PaintCanvas *canvas, int saved);
}

class LensFlare {
public:
    float intensity;                 // current flare alpha
    AbyssEngine::PaintCanvas *canvas;
    int width;
    int height;
    uint32_t *images;                // three Image2D sprite handles

    explicit LensFlare(AbyssEngine::PaintCanvas *canvas);
    ~LensFlare();

    void render2D(float srcX, float srcY, float alpha, int colorIndex);
    void update();
};

#endif
