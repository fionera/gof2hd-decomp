#include "class.h"

extern "C" void ImageFactory_reload(ImageFactory *self);
// *g_ctor_flagA / *g_ctor_flagB -> gate bytes (both evaluated). *g_ctor_dst -> int* dest table;
// g_ctor_src -> source int table (PC-relative base, not dereferenced).
extern "C" __attribute__((visibility("hidden"))) char *g_ctor_flagA;
extern "C" __attribute__((visibility("hidden"))) char *g_ctor_flagB;
extern "C" __attribute__((visibility("hidden"))) int *g_ctor_dst;
extern int g_ctor_src[] __attribute__((visibility("hidden")));

// ImageFactory::ImageFactory() -> this. Copies a 13x4x2 int table when gated, then reload().
extern "C" ImageFactory *ImageFactory_ctor(ImageFactory *self)
{
    F<void *>(self, 0x0) = 0;
    if ((*g_ctor_flagA | *g_ctor_flagB) != 0) {
        int *dst = g_ctor_dst;
        int *src = g_ctor_src;
        for (int r = 0; r != 0xd; ++r) {
            int *d = dst;
            int *s = src;
            for (int c = 0; c != 4; ++c) {
                for (int k = 0; k != 2; ++k)
                    d[k] = s[k];
                s += 2;
                d += 2;
            }
            dst += 8;
            src += 8;
        }
    }
    ImageFactory_reload(self);
    return self;
}
