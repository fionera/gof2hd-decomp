#ifndef GOF2_IMAGEFONT_H
#define GOF2_IMAGEFONT_H
#include <cstdint>

namespace AbyssEngine {
    class Mesh;

    struct ImageFont {
        uint16_t glyphCount;
        uint16_t pad0x2;
        uint16_t *codes;
        unsigned char *field_0x8;
        Mesh **glyphMeshes;
        int16_t spacing;
        int16_t yOffset;

        // Inline: the original emits ImageFont::ImageFont() in Image.cpp next to its only caller.
        ImageFont() : glyphCount(0), codes(0), field_0x8(0), glyphMeshes(0), spacing(0), yOffset(0) {
        }
    };
}

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
static_assert(sizeof(AbyssEngine::ImageFont) == 0x14, "ImageFont layout");
static_assert(offsetof(AbyssEngine::ImageFont, codes) == 0x4, "ImageFont.codes");
static_assert(offsetof(AbyssEngine::ImageFont, glyphMeshes) == 0xc, "ImageFont.glyphMeshes");
static_assert(offsetof(AbyssEngine::ImageFont, spacing) == 0x10, "ImageFont.spacing");
static_assert(offsetof(AbyssEngine::ImageFont, yOffset) == 0x12, "ImageFont.yOffset");
#endif

#endif
