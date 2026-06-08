#include "class.h"

// ImagePart::ImagePart(unsigned, int, int) — stores id/field_04/pos_y, then queries the
// global PaintCanvas for the image's width/height into scale_x/scale_y.
extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, unsigned id);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas, unsigned id);
extern "C" __attribute__((visibility("hidden"))) void **g_ImagePart_canvas;

extern "C" ImagePart *ImagePart_ctor(ImagePart *self, unsigned id, int field04, int posY)
{
    self->id = id;
    self->f_4 = field04;
    self->pos_y = posY;
    void **holder = g_ImagePart_canvas;
    self->scale_x = PaintCanvas_GetImage2DWidth(*holder, id);
    self->scale_y = PaintCanvas_GetImage2DHeight(*holder, id);
    return self;
}
