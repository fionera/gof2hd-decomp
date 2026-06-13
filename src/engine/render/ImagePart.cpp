#include "gof2/engine/render/ImagePart.h"
#include "gof2/engine/render/PaintCanvas.h"

using AbyssEngine::PaintCanvas;

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
    self->scale_x = ((PaintCanvas*)*holder)->GetImage2DWidth(id);
    self->scale_y = ((PaintCanvas*)*holder)->GetImage2DHeight(id);
    return self;
}

// ---- draw_a7aac.cpp ----
// ImagePart::draw(int x, int y, bool b) — draws the stored image2d at (x, pos_y+y)
// with size (scale_x, scale_y), mode 0x11, flags = field_04|1, last = b.
void ImagePart::draw(int x, int y, bool b) {
    ImagePart *self = this;
    void **holder = g_ImagePart_draw_canvas;
    ((PaintCanvas*)*holder)->DrawImage2D(self->id, x,
                            self->pos_y + y,
                            self->scale_x, self->scale_y,
                            (unsigned char)0x11, (unsigned char)(self->f_4 | 1), (unsigned char)b);
}
