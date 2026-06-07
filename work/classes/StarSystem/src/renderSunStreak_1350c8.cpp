#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_StarSystem_sunstreak_canvas;

extern "C" void PaintCanvas_SetTexture(void *canvas, uint32_t texture, int mode);
extern "C" void PaintCanvas_SetBlendMode(void *canvas, int mode);
extern "C" void StarSystem_renderSunStreak_tail(void *geom);

extern "C" void StarSystem_renderSunStreak(StarSystem *self)
{
    void **holder = g_StarSystem_sunstreak_canvas;
    void *canvas = *holder;
    void *slot = (B(self, 0x0c) != 0) ? (void *)((char *)self + 0x10) : P(P(self, 0x14), 4);
    PaintCanvas_SetTexture(canvas, *(uint32_t *)slot, -1);
    PaintCanvas_SetBlendMode(*holder, 2);
    return StarSystem_renderSunStreak_tail(P(self, 0x40));
}
