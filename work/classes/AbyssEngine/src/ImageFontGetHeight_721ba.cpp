#include "class.h"

// AbyssEngine::ImageFontGetHeight(AbyssEngine::ImageFont*)
namespace AbyssEngine {

int ImageFontGetHeight(ImageFont *font)
{
    if (font != 0) {
        // ((*(*(font+0xc)))[+4])->[0x1c] as float -> int; clamp 0x18 to 0x13.
        void *p = pp(font, 0xc);
        void *q = pp(p, 0x0);
        void *r = pp(q, 0x4);
        int v = (int)f32(r, 0x1c);
        if (v == 0x18)
            v = 0x13;
        return v;
    }
    return 0;
}

} // namespace AbyssEngine
