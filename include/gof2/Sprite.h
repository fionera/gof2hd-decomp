#ifndef GOF2_SPRITE_H
#define GOF2_SPRITE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)

void operator delete(void *ptr) noexcept;

extern "C" int __aeabi_idiv(int numerator, int denominator);
extern "C" int __aeabi_idivmod(int numerator, int denominator);

extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, uint32_t image);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas, uint32_t image);
extern "C" void PaintCanvas_DrawRegion2D(void *canvas, uint32_t image, int srcX, int srcY, int w, int h,
                                         float alpha, int p7, int p8, int x, int y);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, uint32_t image, int x, int y);
extern "C" void PaintCanvas_DrawImage2DScaled(void *canvas, uint32_t image, int x, int y, int w, int h,
                                             int anchorX, int anchorY, int flags);

__attribute__((visibility("hidden"))) extern void **g_Sprite_canvas;

struct Sprite {
    uint32_t *field_0x0;   // frame image handle array (null => single image)
    uint32_t  field_0x4;   // single image handle (-1 when using frame array)
    int32_t   field_0x8;   // reference pixel X
    int32_t   field_0xc;   // reference pixel Y
    int32_t   field_0x10;  // position X
    int32_t   field_0x14;  // position Y
    int32_t   field_0x18;  // frame width
    int32_t   field_0x1c;  // frame height
    int32_t   field_0x20;  // image width
    int32_t   field_0x24;  // image height
    int32_t   field_0x28;  // current frame src X
    int32_t   field_0x2c;  // current frame src Y
    int32_t   field_0x30;  // columns
    int32_t   field_0x34;  // rows
    int32_t   field_0x38;  // current frame index
    int32_t   field_0x3c;  // frame count

    Sprite(uint32_t image, int frameWidth, int frameHeight);
    Sprite(uint32_t *frames, int frameCount, int frameWidth, int frameHeight);
    ~Sprite();
    void setPosition(int x, int y);
    void setRefPixelPosition(int x, int y);
    void defineReferencePixel(int x, int y);
    void setFrame(int frame);
    void nextFrame();
    void prevFrame();
    void draw(float scaleX, float scaleY);
    void drawRegion(int srcX, int srcY, int w, int h);
};
#endif
