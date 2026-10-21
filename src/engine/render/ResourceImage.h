#ifndef GOF2_RESOURCEIMAGE_H
#define GOF2_RESOURCEIMAGE_H

namespace AbyssEngine {
    // Image (sprite / font sheet) resource descriptor: a sub-image `index` of texture `textureId`.
    struct ResourceImage {
        unsigned short textureId;
        unsigned short index;

        ResourceImage(unsigned short texture, unsigned short imageIndex) : textureId(texture), index(imageIndex) {}
    };
}

#endif
