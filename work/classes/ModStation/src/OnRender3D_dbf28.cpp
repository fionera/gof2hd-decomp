#include "class.h"

struct SpaceLounge;
struct StarMap;
struct CutScene;
struct MissionsWindow;

// PaintCanvas singleton holder: single pc-rel deref -> holder; object is *holder.
__attribute__((visibility("hidden"))) extern void **g_ModStation_r3d_canvas;

extern "C" void PaintCanvas_ClearBuffer(void *c);
extern "C" void PaintCanvas_Begin3d(void *c);
extern "C" void StarMap_renderBG(void *p);
extern "C" void SpaceLounge_OnRenderBG(void *sl);
extern "C" void CutScene_renderBG();
extern "C" void SpaceLounge_OnRender3D(void *sl);
extern "C" void StarMap_render(void *p);
extern "C" void MissionsWindow_render3D(void *p);
extern "C" void CutScene_render3D(void *p);
// Tail veneer at 0x1ab918.
extern "C" void ModStation_r3d_endTail(void *c);

// ModStation::OnRender3D()
extern "C" void ModStation_OnRender3D(ModStation *self)
{
    if (C(self, 0x24) == 0)
        return;
    void **holder = g_ModStation_r3d_canvas;
    PaintCanvas_ClearBuffer(*holder);

    char *p65 = (char *)self + 0x65;
    if (P(self, 0x14) == 0 || C(self, 0x66) != 0 || C(self, 0x64) != 0 ||
        C(self, 0x68) != 0 || C(self, 0x67) != 0 || *p65 != 0 ||
        C(self, 0x62) != 0) {
        if (C(self, 0x67) != 0) {
            StarMap_renderBG(P(self, 0x10));
        } else if (*p65 != 0) {
            SpaceLounge_OnRenderBG(P(self, 0x74));
        }
    } else {
        CutScene_renderBG();
    }
    PaintCanvas_Begin3d(*holder);
    if (*p65 != 0)
        SpaceLounge_OnRender3D(P(self, 0x74));
    else if (C(self, 0x67) != 0)
        StarMap_render(P(self, 0x10));
    else if (C(self, 0x64) != 0)
        MissionsWindow_render3D(P(self, 0x80));
    else if (C(self, 0x66) == 0 && P(self, 0x14) != 0)
        CutScene_render3D(P(self, 0x14));
    ModStation_r3d_endTail(*holder);
}
