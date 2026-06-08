#include "class.h"

// AbyssEngine::ImageFontDrawString(ImageFont*, unsigned short const*, int x, int y,
//                                  PaintCanvas*, Engine*, bool)
// Convenience overload: compute the NUL-terminated length and forward to the explicit-length
// renderer.
namespace AbyssEngine {

void ImageFontDrawString(ImageFont *font, const unsigned short *str, unsigned int len, int x,
                         int y, PaintCanvas *canvas, Engine *engine, bool flag);

void ImageFontDrawString(ImageFont *font, const unsigned short *str, int x, int y,
                         PaintCanvas *canvas, Engine *engine, bool flag)
{
    if (font != 0 && str != 0) {
        unsigned short i = 0;
        unsigned int len;
        do {
            len = (unsigned int)i;
            i = i + 1;
        } while (str[len] != 0);
        ImageFontDrawString(font, str, len, x, y, canvas, engine, flag);
    }
}

} // namespace AbyssEngine
