#include "engine/render/ImagePart.h"
#include "engine/render/PaintCanvas.h"

using AbyssEngine::PaintCanvas;

extern "C" __attribute__((visibility("hidden"))) void **g_ImagePart_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_ImagePart_draw_canvas;

// ImagePart::ImagePart(unsigned, int, int) — stores id/field_04/pos_y, then queries the
// global PaintCanvas for the image's width/height into scale_x/scale_y.

ImagePart::ImagePart(unsigned id, int field04, int posY) {
    this->id = id;
    this->f_4 = field04;
    this->pos_y = posY;
    void **holder = g_ImagePart_canvas;
    this->scale_x = ((PaintCanvas*)*holder)->GetImage2DWidth(id);
    this->scale_y = ((PaintCanvas*)*holder)->GetImage2DHeight(id);
}

// ImagePart::~ImagePart() — trivial; the binary emits a standalone empty dtor.
ImagePart::~ImagePart() {}

// ImagePart::draw(int x, int y, bool b) — draws the stored image2d at (x, pos_y+y)
// with size (scale_x, scale_y), mode 0x11, flags = field_04|1, last = b.
void ImagePart::draw(int x, int y, bool b) {
    void **holder = g_ImagePart_draw_canvas;
    ((PaintCanvas*)*holder)->DrawImage2D(this->id, x,
                            this->pos_y + y,
                            this->scale_x, this->scale_y,
                            (unsigned char)0x11, (unsigned char)(this->f_4 | 1), (unsigned char)b);
}
