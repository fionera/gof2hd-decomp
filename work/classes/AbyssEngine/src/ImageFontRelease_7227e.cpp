#include "class.h"

void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

// AbyssEngine::ImageFontRelease(AbyssEngine::Engine*, AbyssEngine::ImageFont**)
// Free the codepoint table (@4), release each glyph mesh (count@0, array@0xc), free the glyph
// array, then the font.
namespace AbyssEngine {

void MeshRelease(Engine *engine, Mesh **slot);

void ImageFontRelease(Engine *engine, ImageFont **slot)
{
    if (*slot == 0)
        return;

    void *table = pp(*slot, 0x4);
    if (table != 0)
        operator delete[](table);
    pp(*slot, 0x4) = 0;

    int off = 0;
    unsigned int i = 0;
    void *glyphs;
    for (;;) {
        ImageFont *f = (ImageFont *)*slot;
        glyphs = pp(f, 0xc);
        if (u16(f, 0x0) <= i)
            break;
        MeshRelease(engine, (Mesh **)((char *)glyphs + off));
        off += 4;
        ++i;
    }

    if (glyphs != 0)
        operator delete[](glyphs);
    pp(*slot, 0xc) = 0;

    if (*slot != 0)
        operator delete((void *)*slot);
    *slot = 0;
}

} // namespace AbyssEngine
