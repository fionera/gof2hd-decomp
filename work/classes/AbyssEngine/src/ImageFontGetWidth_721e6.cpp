#include "class.h"

// AbyssEngine::ImageFontGetWidth(AbyssEngine::ImageFont*, unsigned short const*, unsigned int
//                                start, unsigned int len)
// Same as the 3-arg form, but measures the substring [start, start+len).
namespace AbyssEngine {

int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int start,
                      unsigned int len)
{
    if (font == 0 || str == 0)
        return 0;

    unsigned int end = len + start;
    unsigned short glyphCount = u16(font, 0x0);
    int total = 0;

    for (; (start & 0xffff) < end; ++start) {
        unsigned short idx = 0;
        unsigned short code;
        bool found = false;
        for (;;) {
            unsigned int u = idx;
            if (glyphCount <= u)
                break;
            idx = idx + 1;
            code = *(unsigned short *)((char *)pp(font, 0x4) + u * 2);
            if (code == str[start & 0xffff]) {
                found = true;
                break;
            }
        }
        if (!found)
            continue;

        unsigned int gi = (unsigned int)(unsigned short)(idx - 1);
        void *glyph = *(void **)((char *)pp(font, 0xc) + gi * 4);
        int w = (int)f32(pp(glyph, 0x4), 0xc);
        int adv = (int)s16(font, 0x10) + w;
        int contrib = adv;
        if (w == 0xb)
            contrib = adv - 2;
        if (code != 0x20)
            contrib = adv;
        total += contrib;
    }
    return total;
}

} // namespace AbyssEngine
