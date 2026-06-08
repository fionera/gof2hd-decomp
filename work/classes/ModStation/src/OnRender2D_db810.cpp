#include "class.h"

struct Layout;

__attribute__((visibility("hidden"))) extern int  *g_r2d_stack;       // [DAT_000eba2c]
__attribute__((visibility("hidden"))) extern int **g_r2d_helpLayout;  // [DAT_000eba30]

extern "C" {
void PaintCanvas_Begin2d_r2d();
void PaintCanvas_End2d_r2d();
void PaintCanvas_SwapBuffer_r2d();
void PaintCanvas_SetColor_r2d(unsigned c);
void HangarWindow_render_r2d(ModStation *self);
void HangarWindow_render3D_r2d();
void SpaceLounge_draw_r2d(ModStation *self);
void SpaceLounge_draw3DShip_r2d();
void MissionsWindow_draw_r2d(void *w);
void StarMap_draw_r2d(ModStation *self);
void StatusWindow_draw_r2d(ModStation *self);
void MenuTouchWindow_draw_r2d(ModStation *self);
void Radio_draw_r2d(ModStation *self);
void CutScene_render2D_r2d();
void ChoiceWindow_draw_r2d(ModStation *self);
void DialogueWindow_draw_r2d(ModStation *self);
void Layout_drawHelpWindow_r2d(Layout *l);
// The full station-HUD pass (header, footer, system box, news ticker, buttons, reward banner) —
// dozens of SIMD string-layout calls the decompiler corrupted.
void ModStation_r2d_drawStationHud(ModStation *self);
}

// ModStation::OnRender2D() — top-level 2D pass; dispatches to whichever station sub-screen is
// currently open, then overlays the choice/dialogue/help windows.
extern "C" void ModStation_OnRender2D(ModStation *self)
{
    PaintCanvas_Begin2d_r2d();
    PaintCanvas_SetColor_r2d(U(self, 4));

    if (C(self, 0x24) == 0) {
        // station screen not active yet — nothing to draw this frame.
        return;
    }

    if (C(self, 0x66) != 0) {            // hangar
        HangarWindow_render_r2d(self);
    } else if (C(self, 0x65) != 0) {     // space lounge
        SpaceLounge_draw_r2d(self);
    } else if (C(self, 0x64) != 0) {     // missions
        MissionsWindow_draw_r2d(P(self, 0x80));
    } else if (C(self, 0x67) != 0) {     // star map
        StarMap_draw_r2d(self);
    } else if (C(self, 0x68) != 0) {     // status
        StatusWindow_draw_r2d(self);
    } else if (C(self, 0x6a) != 0) {     // DLC menu
        MenuTouchWindow_draw_r2d(self);
    } else if (C(self, 0x6c) != 0) {     // radio / cutscene overlay
        Radio_draw_r2d(self);
        CutScene_render2D_r2d();
    } else {
        // default: the station information HUD.
        ModStation_r2d_drawStationHud(self);
    }

    if (C(self, 0x6a) != 0 || C(self, 0x6b) != 0)
        ChoiceWindow_draw_r2d(self);
    if (C(self, 0x69) != 0)
        DialogueWindow_draw_r2d(self);

    Layout **help = g_r2d_helpLayout;
    if (*(char *)*help != 0)
        Layout_drawHelpWindow_r2d((Layout *)*help);

    PaintCanvas_End2d_r2d();

    // 3D hangar/ship pass when no help window is up.
    if (*(char *)*help == 0) {
        if (C(self, 0x66) != 0) {
            HangarWindow_render3D_r2d();
        }
        if (*(char *)*help == 0 && C(self, 0x65) != 0)
            SpaceLounge_draw3DShip_r2d();
    }

    PaintCanvas_SwapBuffer_r2d();
}
