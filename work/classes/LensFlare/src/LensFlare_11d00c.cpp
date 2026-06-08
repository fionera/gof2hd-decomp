#include "class.h"

// PC-relative pointer-to-pointer global: the engine PaintCanvas singleton used to create
// the flare's three Image2D sprites (DAT_0012d02c).
extern void *const gLensFlareCanvas __attribute__((visibility("hidden")));

// LensFlare::LensFlare(AbyssEngine::PaintCanvas* param_1)
struct LensFlare {
    // @portable-fields
    int f_0; // 0x0
    void* f_4; // 0x4
    int f_8; // 0x8
    int f_c; // 0xc
    void* f_10; // 0x10
 LensFlare(PaintCanvas *param_1); };
LensFlare::LensFlare(PaintCanvas *param_1)
{
    uint32_t *images = (uint32_t *)operator new[](0xc);
    this->f_10 = images;
    void *canvas = *(void **)gLensFlareCanvas;
    int off = 0;
    for (int i = 0; i != 3; i = i + 1) {
        LensFlare_Image2DCreate(canvas, (short)(i + 0x508),
                                (uint32_t *)((char *)this->f_10 + off));
        off = off + 4;
    }
    this->f_8 = LensFlare_GetWidth(param_1);
    this->f_c = LensFlare_GetHeight(param_1);
    this->f_4 = param_1;
}
