#include "class.h"

// ImagePart::draw(int x, int y, bool b) — draws the stored image2d at (x, pos_y+y)
// with size (scale_x, scale_y), mode 0x11, flags = field_04|1, last = b.
extern "C" void PaintCanvas_DrawImage2D(void *canvas, unsigned id, int x, int y,
                                        int w, int h, int mode, int flags, bool b);
extern "C" __attribute__((visibility("hidden"))) void **g_ImagePart_draw_canvas;

extern "C" void ImagePart_draw(ImagePart *self, int x, int y, bool b)
{
    void **holder = g_ImagePart_draw_canvas;
    PaintCanvas_DrawImage2D(*holder, F<unsigned>(self, 0x00), x,
                            F<int>(self, 0x08) + y,
                            F<int>(self, 0x0c), F<int>(self, 0x10),
                            0x11, F<int>(self, 0x04) | 1, b);
}
