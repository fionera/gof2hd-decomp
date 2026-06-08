#include "class.h"

// AbyssEngine::ImageFontGetWidth(AbyssEngine::ImageFont*, unsigned short const*, unsigned int)
// Sum the advance width of the first `count` glyphs. For each character look it up in the
// font's codepoint table (count@0, table@4); the per-glyph width comes from the glyph's
// metrics plus the font spacing (@0x10), with a -2 tweak for the 0xb-wide space.
namespace AbyssEngine {

int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int count)
{
    int total = 0;
    if (font == 0 || str == 0)
        return 0;

    unsigned short glyphCount = u16(font, 0x0);
    for (unsigned short i = 0; i < count; ++i) {
        unsigned short idx = 0;
        unsigned short code;
        for (;;) {
            unsigned int u = idx;
            if (glyphCount <= u)
                goto next; // character not found -> contributes nothing
            idx = idx + 1;
            code = *(unsigned short *)((char *)pp(font, 0x4) + u * 2);
            if (code == str[i])
                break;
        }
        {
            unsigned int found = (unsigned int)(unsigned short)(idx - 1);
            void *glyph = *(void **)((char *)pp(font, 0xc) + found * 4);
            int w = (int)f32(pp(glyph, 0x4), 0xc);
            int adv = (int)s16(font, 0x10) + w;
            int contrib = adv;
            if (w == 0xb)
                contrib = adv - 2;
            if (code != 0x20)
                contrib = adv;
            total += contrib;
        }
    next:;
    }
    return total;
}

} // namespace AbyssEngine
