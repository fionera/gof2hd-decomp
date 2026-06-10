#include "gof2/ImagePart.h"


extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, unsigned id);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas, unsigned id);
extern "C" __attribute__((visibility("hidden"))) void **g_ImagePart_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_ImagePart_draw_canvas;

// ---- ImagePart_a7a74.cpp ----
// ImagePart::ImagePart(unsigned, int, int) — stores id/field_04/pos_y, then queries the
// global PaintCanvas for the image's width/height into scale_x/scale_y.

ImagePart * ImagePart::ctor(unsigned id, int field04, int posY) {
    ImagePart *self = this;
    self->id = id;
    self->f_4 = field04;
    self->pos_y = posY;
    void **holder = g_ImagePart_canvas;
    self->scale_x = PaintCanvas_GetImage2DWidth(*holder, id);
    self->scale_y = PaintCanvas_GetImage2DHeight(*holder, id);
    return self;
}

// ---- draw_a7aac.cpp ----
// ImagePart::draw(int x, int y, bool b) — draws the stored image2d at (x, pos_y+y)
// with size (scale_x, scale_y), mode 0x11, flags = field_04|1, last = b.
extern "C" void PaintCanvas_DrawImage2D(void *canvas, unsigned id, int x, int y,
                                        int w, int h, int mode, int flags, bool b);

void ImagePart::draw(int x, int y, bool b) {
    ImagePart *self = this;
    void **holder = g_ImagePart_draw_canvas;
    PaintCanvas_DrawImage2D(*holder, self->id, x,
                            self->pos_y + y,
                            self->scale_x, self->scale_y,
                            0x11, self->f_4 | 1, b);
}
