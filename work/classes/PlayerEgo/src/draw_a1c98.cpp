#include "class.h"

// PlayerEgo::draw(bool allowHud)
//   Draws the 2D overlay layer for the player ship (targeting reticle / lock
//   marker plus the boost throttle). A cascade of guard conditions short-circuits
//   to one of several render-tail veneers when the player is in a maneuver, the
//   mining game, dead, docking, in a cutscene, etc. In the normal path it builds
//   a unit matrix (optionally seeded from the ship matrix), then draws the
//   appropriate reticle image depending on turret mode / plasma lock / blink
//   state, and finally the throttle gauge.

extern "C" int  PlayerEgo_isDead(PlayerEgo *self);
extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void  Mat_assign(void *dst, const void *src);
extern "C" void  PaintCanvas_SetColor(unsigned int c);
extern "C" void  PaintCanvas_DrawImage2DEx(void *canvas, int img, int x, int y,
                                           int anchor, int flags);
extern "C" void *Status_getShip();
extern "C" int   Ship_getFirstEquipmentOfSort(void *ship, int sort);
extern "C" int   Radar_isPlasmaInRange(void *radar);
extern "C" void  PlayerEgo_drawThrottle(PlayerEgo *self);
extern "C" void  PlayerEgo_draw_tailA(void);   // 0x1abc48 veneer
extern "C" void  PlayerEgo_draw_tailB(void);   // 0x1abc58 veneer
extern "C" void  PlayerEgo_draw_tailC(void);   // 0x1abc68 veneer

__attribute__((visibility("hidden"))) extern void **g_PE_dr_canvas;    // 0xb1dd8 PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_PE_dr_status;    // 0xb1de8 Status singleton
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posLock;   // 0xb1e0c reticle pos (plasma)
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posNoLock; // 0xb1ea2
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posBlink;  // 0xb1e24
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posNormal; // 0xb1e6e

extern "C" void PlayerEgo_draw(PlayerEgo *self, int allowHud)
{
    if (P(self, 0x194) != 0)               // mid scripted maneuver: nothing to draw
        return;

    if (P(self, 0x1e8) != 0 && C(self, 0x1a0) == 0)
        return;

    if (P(self, 0x1e4) != 0) {             // mining game owns the screen
        PlayerEgo_draw_tailA();
        return;
    }

    bool full;
    if (C(self, 0x1f4) != 0 || PlayerEgo_isDead(self) != 0 || C(self, 0x24) != 0
        || allowHud == 0 || C(self, 0x1c0) != 0 || C(self, 0x1ee) != 0 || C(self, 0x1ed) != 0) {
        full = false;
    } else {
        full = true;
    }

    if (!full) {
        if (C(self, 0x158) != 0) {
            PlayerEgo_draw_tailB();
            return;
        }
        if (C(self, 0x356) == 0)
            return;
        if ((unsigned)(I(self, 0x1c4) - 1) < 3)
            return;
        PlayerEgo_draw_tailB();
        return;
    }

    unsigned char m[0x30];                 // unit matrix, optionally ship-aligned
    for (int i = 0; i < 0x30; i++) m[i] = 0;
    F(m, 0x0)  = 1.0f;
    F(m, 0x14) = 1.0f;
    F(m, 0x28) = 1.0f;

    bool aligned = (C(self, 0x158) != 0 && C(self, 0x1a0) != 0);
    if (!aligned)
        Mat_assign(m, AEGeometry_getMatrix(P(self, 0x8)));

    void *canvas = *g_PE_dr_canvas;
    PaintCanvas_SetColor(0xffffffff);

    if (C(self, 0x1a0) != 0
        && Ship_getFirstEquipmentOfSort(Status_getShip(), 0x23) != 0) {
        // turret crosshair: plasma lock changes the marker + position.
        if (Radar_isPlasmaInRange(P(self, 0x14)) != 0) {
            float *p = g_PE_dr_posLock;
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x248), (int)p[0], (int)p[1], 0x11, 0x44);
        } else {
            float *p = g_PE_dr_posNoLock;
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x244), (int)p[0], (int)p[1], 0x11, 0x44);
        }
    } else {
        // standard lock-on reticle: blink while acquiring.
        if (C(P(self, 0xc), 0x30) != 0) {
            float *p = g_PE_dr_posBlink;
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x240), (int)p[0], (int)p[1], 0x11, 0x44);
            int t = I(self, 0x24c) + I(self, 0x134);
            I(self, 0x24c) = t;
            if (t >= 0xc9)
                C(P(self, 0xc), 0x30) = 0;
        } else {
            float *p = g_PE_dr_posNormal;
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x23c), (int)p[0], (int)p[1], 0x11, 0x44);
            I(self, 0x24c) = 0;
        }
    }

    PlayerEgo_drawThrottle(self);
}
