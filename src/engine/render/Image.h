#ifndef GOF2_IMAGE_H
#define GOF2_IMAGE_H
#include <cstdint>

namespace AbyssEngine {
    struct Image {
        uint16_t width;
        uint16_t height;
        uint32_t format;
        uint8_t hasMipmaps;
        unsigned char *data;
        uint32_t dataLen;

        // Inline: the original emits Image::Image() in Image.cpp; dataLen is left unset.
        Image() : width(0), height(0), format(0), hasMipmaps(0), data(0) {
        }
    };
}

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
static_assert(offsetof(AbyssEngine::Image, format) == 0x4, "Image.format");
static_assert(offsetof(AbyssEngine::Image, hasMipmaps) == 0x8, "Image.hasMipmaps");
static_assert(offsetof(AbyssEngine::Image, data) == 0xc, "Image.data");
static_assert(offsetof(AbyssEngine::Image, dataLen) == 0x10, "Image.dataLen");
#endif

#endif
