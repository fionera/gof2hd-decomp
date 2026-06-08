#include "class.h"

// Hud::touchedElement(uint x, uint y) — hit-tests a touch against the HUD button rectangles
// and returns the corresponding action bit (0 if none). When the radial quick-menu is open it
// instead forwards to the menu buttons. Rectangle origins live in the ushort fields; the two
// half-extents are at +0x4d8 (most) and +0x4dc (a few). r0=this, r1=x, r2=y.
extern "C" int TouchButton_OnTouchBegin(void *btn, unsigned int x);

__attribute__((visibility("hidden"))) extern void **g_Hud_teCinematic; // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_teScreenW;   // *holder -> [0] width
__attribute__((visibility("hidden"))) extern void **g_Hud_teScreenH;   // *holder -> [0] height

// rect test: origin ushort at `off`, size `w`, against coordinate `v`.
static inline bool span(Hud *self, int off, int w, unsigned int v) {
    unsigned short o = US(self, off);
    return o <= v && v <= (unsigned int)o + w;
}
// centered span: [o-w, o+w]
static inline bool cspan(Hud *self, int off, int w, unsigned int v) {
    unsigned short o = US(self, off);
    return (unsigned int)o - w <= v && v <= (unsigned int)o + w;
}

extern "C" unsigned int Hud_touchedElement(Hud *self, unsigned int x, unsigned int y)
{
    unsigned int *menu = (unsigned int *)P(self, 0x18);
    if (UC(self, 0x282) != 0 && menu != 0) {
        // quick-menu open: delegate to its buttons
        for (unsigned int i = 0; i < menu[0]; i++) {
            if (TouchButton_OnTouchBegin(*(void **)(menu[1] + i * 4), x) != 0)
                return *(unsigned int *)*(void **)(I(P(self, 0x18), 4) + i * 4);
            menu = (unsigned int *)P(self, 0x18);
        }
        return 0;
    }

    int w = I(self, 0x4d8);
    int w2 = I(self, 0x4dc);

    bool cinematic = *(char *)*g_Hud_teCinematic != 0;

    if (cinematic) {
        if (span(self, 0x40a, w, x) && span(self, 0x40c, w, y)) return 1;
        if (UC(self, 0x21e) != 0 && span(self, 0x410, w, x) && span(self, 0x412, w, y)) return 2;
        if (span(self, 0x3f8, w, x) && span(self, 0x3fa, w, y)) return 0x40;
        if (span(self, 0x404, w, x) && span(self, 0x406, w, y)) return 0x100;
        if (cspan(self, 0x424, w, x) && cspan(self, 0x426, w2, y)) return 0x20;
        if (span(self, 0x3f2, w, x) && span(self, 0x3f4, w, y)) { I(self, 0x470) = 1000; return 0x80; }
        if (cspan(self, 0x3ec, w, x) && span(self, 0x3ee, w, y)) return 8;
        if (cspan(self, 0x3e4, w2 >> 1, x) && cspan(self, 0x3e6, w2 >> 1, y)) return 0x10;
        if (UC(self, 0x283) == 0 && span(self, 0x416, US(self, 0x41a), x) &&
            span(self, 0x418, US(self, 0x41c), y)) return 4;
        if (span(self, 0x3fe, w, x) && span(self, 0x400, w, y)) return 0x20000000;
        if (UC(self, 0x528) != 0) {
            if (span(self, 0x454, w, x) && span(self, 0x456, w, y)) return 0x200;
            if (span(self, 0x458, w, x) && span(self, 0x45a, w, y)) return 0x400;
            if (span(self, 0x45e, w, x) && span(self, 0x460, w, y)) return 0x800;
        }
        return 0;
    }

    // normal layout: extra weapon-select buttons first
    if (UC(self, 0x528) != 0) {
        if (span(self, 0x454, w, x) && span(self, 0x456, w, y)) return 0x200;
        if (span(self, 0x458, w, x) && span(self, 0x45a, w, y)) return 0x400;
        if (span(self, 0x45e, w, x) && span(self, 0x460, w, y)) return 0x800;
    }

    int screenW = *(int *)*g_Hud_teScreenW;
    int screenH = *(int *)*g_Hud_teScreenH;

    if (y < (unsigned int)(screenH >> 2)) {
        if (span(self, 0x40a, w, x) && span(self, 0x40c, w, y)) return 1;
    } else if (x < (unsigned int)(screenW >> 1)) {
        if (UC(self, 0x21e) != 0 && cspan(self, 0x410, w, x) && span(self, 0x412, w, y)) return 2;
        if (span(self, 0x3f8, w, x) && span(self, 0x3fa, w, y)) return 0x40;
        if (span(self, 0x404, w, x) && span(self, 0x406, w, y)) return 0x100;
        if (cspan(self, 0x424, w, x) && cspan(self, 0x426, w2, y)) return 0x20;
    } else {
        if (span(self, 0x3f2, w, x) && span(self, 0x3f4, w, y)) { I(self, 0x470) = 1000; return 0x80; }
        if (cspan(self, 0x3ec, w, x) && span(self, 0x3ee, w, y)) return 8;
        if (span(self, 0x3e4, w2, x) && span(self, 0x3e6, w2, y)) return 0x10;
        if (UC(self, 0x283) == 0 && span(self, 0x416, US(self, 0x41a), x) &&
            span(self, 0x418, US(self, 0x41c), y)) return 4;
        if (span(self, 0x3fe, w, x) && span(self, 0x400, w, y)) return 0x20000000;
    }
    return 0;
}
