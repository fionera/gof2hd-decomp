#include "class.h"

struct Arr { uint32_t size; void **data; uint32_t size2; };

extern "C" void PaintCanvas_SetColor(unsigned canvas, unsigned color);
extern "C" void PaintCanvas_DrawImage2D(unsigned canvas, int image, int x, int y);
extern "C" void ImagePart_draw(void *part, int x, int y, int flag);
// Tail veneer (function-pointer global): draws the foreground glyph layer.
extern "C" void ImageFactory_drawChar_tail(unsigned canvas, int handle, int x, int y);
extern "C" __attribute__((visibility("hidden"))) unsigned *g_drawChar_canvas;

// ImageFactory::drawChar(Array<ImagePart*>*, int, int, bool)
extern "C" void ImageFactory_drawChar(ImageFactory *self, Arr *parts, int x, int y, int flag)
{
    unsigned *holder = g_drawChar_canvas;
    PaintCanvas_SetColor(*holder, 0xffffffffu);
    PaintCanvas_DrawImage2D(*holder, i32(self, 0x4), x, y);
    for (unsigned i = 0; i < parts->size; ++i) {
        void *part = parts->data[i];
        if (part != 0) ImagePart_draw(part, x, y, flag);
    }
    return ImageFactory_drawChar_tail(*holder, i32(self, 0x8), x, y);
}
