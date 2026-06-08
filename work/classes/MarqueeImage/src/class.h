#ifndef WORK_CLASSES_MARQUEEIMAGE_SRC_CLASS_H
#define WORK_CLASSES_MARQUEEIMAGE_SRC_CLASS_H

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct MarqueeImage {
    MarqueeImage(uint16_t image, int width, int x, int y, float speed);

    void setPosition(int x, int y);
    void update(int dt);
    void draw();
    void draw(int x, int y);
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

extern "C" void PaintCanvas_Image2DCreate(void *canvas, uint16_t image, uint32_t *out);
extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, uint32_t image);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas, uint32_t image);
extern "C" void PaintCanvas_DrawRegion2D(void *canvas, uint32_t image, int srcX, int srcY, int w, int h,
                                         long long zeroPair, int flags, int x, int y);

__attribute__((visibility("hidden"))) extern void **g_MarqueeImage_canvas;

#endif
