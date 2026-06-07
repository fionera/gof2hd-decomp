#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r3d_canvas;

extern "C" void PaintCanvas_ClearBuffer(void *canvas, int value);
extern "C" void PaintCanvas_Begin3d(void *canvas);
extern "C" void CutScene_renderBG(void *scene);
extern "C" void CutScene_render3D(void *scene);
extern "C" void ModMainMenu_r3dTail(void *canvas);

extern "C" void _ZN11ModMainMenu10OnRender3DEv(ModMainMenu *self)
{
    void **canvas = g_ModMainMenu_r3d_canvas;
    PaintCanvas_ClearBuffer(*canvas, 0);
    CutScene_renderBG(P(self, 0x1c));
    PaintCanvas_Begin3d(*canvas);
    CutScene_render3D(P(self, 0x1c));
    ModMainMenu_r3dTail(*canvas);
}
