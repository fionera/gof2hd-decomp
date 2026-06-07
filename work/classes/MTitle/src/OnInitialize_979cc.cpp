#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_MTitle_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_sound;

extern "C" void PaintCanvas_Image2DCreate(void *canvas, unsigned short image, unsigned int *out);
extern "C" void FModSound_play(void *self, int sound, int pos, int vel, int extra);

int MTitle::OnInitialize()
{
    void **canvas = g_MTitle_canvas;
    PaintCanvas_Image2DCreate(*canvas, 7000, (unsigned int *)B(this, 0x10));
    PaintCanvas_Image2DCreate(*canvas, 0x1b59, (unsigned int *)B(this, 0x14));

    int z = 0;
    FModSound_play(*g_MTitle_sound, 0x91, z, z, z);

    I(this, 0x18) = z;
    I(this, 0x1c) = z;
    I(this, 0xc) = 100;
    return z;
}
