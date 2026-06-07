#ifndef WORK_CLASSES_SPRITE_SRC_CLASS_H
#define WORK_CLASSES_SPRITE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef void *alias_void_ptr __attribute__((may_alias));
typedef uint32_t alias_u32 __attribute__((may_alias));
typedef int32_t alias_i32 __attribute__((may_alias));

struct Sprite {
    Sprite(uint32_t image, int frameWidth, int frameHeight);
    Sprite(uint32_t *frames, int frameCount, int frameWidth, int frameHeight);
    ~Sprite();

    void setFrame(int frame);
    void defineReferencePixel(int x, int y);
    void setRefPixelPosition(int x, int y);
    void setPosition(int x, int y);
    void draw(float scaleX, float scaleY);
    void drawRegion(int x, int y, int w, int h);
    void nextFrame();
    void prevFrame();
};

template <class T>
static inline T &field(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline const T &field(const void *self, uint32_t off)
{
    return *(const T *)((const char *)self + off);
}

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

#endif
