#include "class.h"

// ImagePart::draw(int x, int y, bool b) — draws the stored image2d at (x, pos_y+y)
// with size (scale_x, scale_y), mode 0x11, flags = field_04|1, last = b.
extern "C" void PaintCanvas_DrawImage2D(void *canvas, unsigned id, int x, int y,
                                        int w, int h, int mode, int flags, bool b);
extern "C" __attribute__((visibility("hidden"))) void **g_ImagePart_draw_canvas;

extern "C" void ImagePart_draw(ImagePart *self, int x, int y, bool b)
{
    void **holder = g_ImagePart_draw_canvas;
    PaintCanvas_DrawImage2D(*holder, self->id, x,
                            self->pos_y + y,
                            self->scale_x, self->scale_y,
                            0x11, self->f_4 | 1, b);
}
