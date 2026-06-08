#include "class.h"

struct Level;
struct PaintCanvas;

extern "C" void PaintCanvas_ClearBuffer(unsigned color);          // 0x71d94
extern "C" void Level_renderBG(int level);                        // 0x72214
extern "C" void PaintCanvas_Begin3d();                            // 0x71da0
extern "C" void Level_render(int level, int arg);                 // 0x7222c
extern "C" void PlayerEgo_render(int flag);                       // 0x78730
extern "C" void AEGeometry_render(Level *g);                      // 0x72238
extern "C" void LevelScript_render3D(int script);                // 0x7873c
extern "C" void MenuTouchWindow_render3D(int w);                  // 0x78748
extern "C" void StarMap_render(int map);                          // 0x7567c
// PaintCanvas End3d tail helper @0x1ab918.
extern "C" void PaintCanvas_End3d(unsigned color);

__attribute__((visibility("hidden"))) extern unsigned *g_r3dCanvas; // @0x190de4 ([0]=canvas)

// MGame::OnRender3D(): render the active 3D view (game world, menu, or star map).
extern "C" void MGame_OnRender3D(MGame *self) {
    if (F<uint8_t>(self, 0x54) == 0) return;
    unsigned canvas = *g_r3dCanvas;
    PaintCanvas_ClearBuffer(canvas);

    uint8_t inMenuLevel = F<uint8_t>(self, 0x5d);
    uint8_t flag15e = F<uint8_t>(self, 0x15e);

    if (inMenuLevel == 0) {
        // World render.
        Level_renderBG(F<int>(self, 0x78));
        PaintCanvas_Begin3d();
        int arg = (flag15e == 0) ? F<int>(self, 0x40) : 0;
        Level_render(F<int>(self, 0x78), arg);
        int egoFlag = (F<uint8_t>(self, 0x5f) != 0) ? 0 : (F<uint8_t>(self, 0xdc) == 0);
        PlayerEgo_render(egoFlag);
        if (F<int>(self, 0x114) != 0)
            AEGeometry_render(F<Level *>(self, 0x58));
        LevelScript_render3D(F<int>(self, 0x7c));
        return PaintCanvas_End3d(canvas);
    }

    if (flag15e != 0) {
        // Free-cam / cinematic world render.
        Level_renderBG(F<int>(self, 0x78));
        PaintCanvas_Begin3d();
        Level_render(F<int>(self, 0x78), F<int>(self, 0x40));
        int egoFlag = (F<uint8_t>(self, 0x5f) != 0) ? 0 : (F<uint8_t>(self, 0xdc) == 0);
        PlayerEgo_render(egoFlag);
        if (F<int>(self, 0x114) != 0)
            AEGeometry_render(F<Level *>(self, 0x58));
        LevelScript_render3D(F<int>(self, 0x7c));
        return PaintCanvas_End3d(canvas);
    }

    if (F<uint8_t>(self, 0xc9) != 0) {
        PaintCanvas_Begin3d();
        MenuTouchWindow_render3D(F<int>(self, 0x88));
        return PaintCanvas_End3d(canvas);
    }

    if (F<uint8_t>(self, 0xc7) != 0) {
        PaintCanvas_Begin3d();
        StarMap_render(F<int>(self, 0x90));
        return PaintCanvas_End3d(canvas);
    }

    // Fallback world render.
    Level_renderBG(F<int>(self, 0x78));
    PaintCanvas_Begin3d();
    Level_render(F<int>(self, 0x78), 0);
    int egoFlag = (F<uint8_t>(self, 0x5f) != 0) ? 0 : (F<uint8_t>(self, 0xdc) == 0);
    PlayerEgo_render(egoFlag);
    if (F<int>(self, 0x114) != 0)
        AEGeometry_render(F<Level *>(self, 0x58));
    LevelScript_render3D(F<int>(self, 0x7c));
    return PaintCanvas_End3d(canvas);
}
