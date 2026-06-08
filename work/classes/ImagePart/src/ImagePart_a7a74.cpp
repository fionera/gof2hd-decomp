#include "class.h"

// ImagePart::ImagePart(unsigned, int, int) — stores id/field_04/pos_y, then queries the
// global PaintCanvas for the image's width/height into scale_x/scale_y.
extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, unsigned id);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas, unsigned id);
extern "C" __attribute__((visibility("hidden"))) void **g_ImagePart_canvas;

extern "C" ImagePart *ImagePart_ctor(ImagePart *self, unsigned id, int field04, int posY)
{
    F<unsigned>(self, 0x00) = id;
    F<int>(self, 0x04) = field04;
    F<int>(self, 0x08) = posY;
    void **holder = g_ImagePart_canvas;
    F<int>(self, 0x0c) = PaintCanvas_GetImage2DWidth(*holder, id);
    F<int>(self, 0x10) = PaintCanvas_GetImage2DHeight(*holder, id);
    return self;
}
