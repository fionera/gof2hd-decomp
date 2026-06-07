#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_StarSystem_reversal_canvas;

extern "C" void PaintCanvas_TextureCreate(void *canvas, uint32_t texture, void *slot, int flags);
extern "C" void AEGeometry_setScaling(AEGeometry *geom, const char *scale);

extern "C" void StarSystem_switchSunForSupernovaReversal(StarSystem *self)
{
    void **canvasHolder = g_StarSystem_reversal_canvas;
    char scaleBytes[12];
    PaintCanvas_TextureCreate(*canvasHolder, 0x2734, P(P(self, 0x14), 4), 0);
    AEGeometry *geom = *(AEGeometry **)P(P(self, 0x1c), 4);
    uint32_t *scale = (uint32_t *)scaleBytes;
    scale[0] = 0x68009815;
    scale[1] = 0x68009815;
    scale[2] = 0x68009815;
    AEGeometry_setScaling(geom, scaleBytes);
}
