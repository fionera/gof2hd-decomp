#ifndef GOF2_LENSFLARE_H
#define GOF2_LENSFLARE_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- LensFlare class.
// Top-level class. Uses PaintCanvas for drawing.
//
//   0x00  float        intensity (alpha; cleared/updated)
//   0x04  PaintCanvas* canvas (param to ctor)
//   0x08  int          width  (PaintCanvas::GetWidth())
//   0x0c  int          height (PaintCanvas::GetHeight())
//   0x10  uint*[3]     images (new[](0xc); three Image2D handles)

void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" {
// AbyssEngine::PaintCanvas::Image2DCreate(canvas, short id, uint* out) -- 0x00071cf8.
void LensFlare_Image2DCreate(void *canvas, short id, uint32_t *out);
// AbyssEngine::PaintCanvas::GetWidth()/GetHeight() -- 0x0006fafc / 0x0006fb08.
int LensFlare_GetWidth(void *canvas);
int LensFlare_GetHeight(void *canvas);

// math + draw helpers used by render2D (resolved from PC-relative blx targets).
float LensFlare_sqrtf(float v);                 // 0x000760e4
int   LensFlare_imgWidth(void *canvas, void *img);  // 0x00072d84
int   LensFlare_imgHandle(void *img);           // GetWidth-ish handle accessor (blx r6)
void  LensFlare_setColor(void *canvas, uint32_t color);          // 0x0006ff28
void  LensFlare_setPos(void *canvas, int x, int y);              // 0x00071d70
void  LensFlare_drawScaled(void *canvas, void *img, int x, int y);   // 0x00073564
void  LensFlare_drawSprite(void *canvas, void *img, int x, int y);   // 0x000709c0-area (blx r5/r6)
void  LensFlare_pushState(void *canvas);        // 0x0006fa90
void  LensFlare_setBlend(void *canvas, uint32_t mode);           // 0x0006fac0
void  LensFlare_drawFinal(void *canvas, void *img, int a, int b); // 0x00074de8
void  LensFlare_restoreState(void *canvas, int saved);           // tail 0x001ac088
}

class LensFlare {
public:
    float intensity;        // +0x0  intensity
    void* canvas;        // +0x4  PaintCanvas* canvas
    int width;          // +0x8  width
    int height;          // +0xc  height
    void* images;       // +0x10 image handle array (uint[3])

    LensFlare(PaintCanvas *canvas);
    ~LensFlare();
    void render2D(float srcX, float srcY, float alpha, int colorIndex);
    void update();
};
#endif
