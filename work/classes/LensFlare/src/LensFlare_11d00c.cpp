#include "class.h"

// PC-relative pointer-to-pointer global: the engine PaintCanvas singleton used to create
// the flare's three Image2D sprites (DAT_0012d02c).
extern void *const gLensFlareCanvas __attribute__((visibility("hidden")));

// LensFlare::LensFlare(AbyssEngine::PaintCanvas* param_1)
struct LensFlare { LensFlare(PaintCanvas *param_1); };
LensFlare::LensFlare(PaintCanvas *param_1)
{
    uint32_t *images = (uint32_t *)operator new[](0xc);
    pp(this, 0x10) = images;
    void *canvas = *(void **)gLensFlareCanvas;
    int off = 0;
    for (int i = 0; i != 3; i = i + 1) {
        LensFlare_Image2DCreate(canvas, (short)(i + 0x508),
                                (uint32_t *)((char *)pp(this, 0x10) + off));
        off = off + 4;
    }
    i32(this, 0x08) = LensFlare_GetWidth(param_1);
    i32(this, 0x0c) = LensFlare_GetHeight(param_1);
    pp(this, 0x04) = param_1;
}
