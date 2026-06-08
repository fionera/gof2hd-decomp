#include "class.h"

struct PaintCanvas;
struct TouchButton;
struct Status;
struct Ship;

extern "C" void PaintCanvas_SetColor(unsigned color);                      // 0x6fac0
extern "C" int PaintCanvas_GetImage2DWidth(int img);                       // 0x72d84
extern "C" void PaintCanvas_DrawImage2D5(PaintCanvas *pc, unsigned img, int x,
                                         int y, int anchor);                // 0x71d70 / 0x74e3c
extern "C" void PaintCanvas_DrawImage2D3(PaintCanvas *pc, unsigned img, int x, int y); // 0x72dc0
extern "C" void Layout_drawBGPattern(Layout *self, unsigned img, int p3, int p4,
                                     int p5, int p6);                       // 0x74e00
extern "C" void TouchButton_setVisible(TouchButton *b, int v);             // 0x74e48
extern "C" void TouchButton_draw(TouchButton *b);                          // 0x746e0
extern "C" Ship *Status_getShip();                                         // 0x71a58
extern "C" int Ship_getCurrentLoad(Ship *s);                               // 0x72bf8
extern "C" int Ship_getMaxLoad(Ship *s);                                   // 0x72c04
extern "C" int Status_getCredits();                                        // 0x733d8
extern "C" void String_from_uint(void *out, unsigned v);                   // 0x6f658
extern "C" void String_cstr_ctor(void *out, const char *s, bool);          // 0x6ee18
extern "C" void String_concat(void *ret, void *a, void *b);                // 0x6ef98
extern "C" void String_dtor(void *s);                                      // 0x6ee30
extern "C" int PaintCanvas_GetTextWidth(unsigned pc, void *font);          // 0x6faa8
extern "C" void PaintCanvas_DrawString(unsigned pc, void *font, void *str,
                                       int x, int y);                       // 0x6fe14
extern "C" void Layout_formatCredits(void *out, int n);                    // 0x74e54

// Hidden globals from drawFooter disasm.
__attribute__((visibility("hidden"))) extern int *g_dfGuard;       // @0xe3c54 (stack guard [0])
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvas; // @0xe3c56
__attribute__((visibility("hidden"))) extern int **g_dfMetric;     // @0xe3cae ([0][0x10],[0][0x74])
__attribute__((visibility("hidden"))) extern int g_dfWarnColor;    // @0xe3f90 ([0])
__attribute__((visibility("hidden"))) extern const char g_dfSep[];     // @0xe3dc8
__attribute__((visibility("hidden"))) extern const char g_dfTail[];    // @0xe3dfa
__attribute__((visibility("hidden"))) extern void **g_dfFont;      // @0xe3e34

// Layout::drawFooter(bool stationMode, bool showBack)
extern "C" void Layout_drawFooter(Layout *self, int stationMode, int showBack) {
    int *guard = g_dfGuard;
    int g0 = *guard;
    PaintCanvas *pc = *g_dfCanvas;
    PaintCanvas_SetColor(F<unsigned>(self, 0x3b0));
    int wRight = PaintCanvas_GetImage2DWidth(F<int>(self, 0x33c));
    int wLeft = PaintCanvas_GetImage2DWidth(F<int>(self, 0x334));

    PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x334), F<int>(self, 0x2dc),
                             F<int>(self, 0x2e0) + F<int>(self, 0x2e8), 0x11);
    int *m = *g_dfMetric;
    int footerH = m[0x10 / 4];
    PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x33c), F<int>(self, 0x2dc) + wLeft,
                             (F<int>(self, 0x2e0) + F<int>(self, 0x2e8)) - footerH);
    int both = wLeft + wRight;
    Layout_drawBGPattern(self, F<unsigned>(self, 0x338), both + F<int>(self, 0x2dc),
                         (F<int>(self, 0x2e0) + F<int>(self, 0x2e8)) - footerH,
                         F<int>(self, 0x2e4) + both * -2, footerH);
    PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x33c),
                             (F<int>(self, 0x2dc) - both) + F<int>(self, 0x2e4),
                             (F<int>(self, 0x2e8) + F<int>(self, 0x2e0)) - footerH, 0x01);
    PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x334),
                             (F<int>(self, 0x2dc) - wRight) + F<int>(self, 0x2e4),
                             (F<int>(self, 0x2e8) + F<int>(self, 0x2e0)) - footerH, 0x01);

    int backVis = (!stationMode) && showBack;
    TouchButton_setVisible(F<TouchButton *>(self, 0x3b4), backVis);
    if (!backVis) {
        TouchButton_draw(F<TouchButton *>(self, 0x3b8));
    } else if (showBack) {
        TouchButton_draw(F<TouchButton *>(self, 0x3b4));
    }

    // Cargo load text, in warning color if over capacity.
    Status_getShip();
    int load = Ship_getCurrentLoad(Status_getShip());
    Status_getShip();
    int maxLoad = Ship_getMaxLoad(Status_getShip());
    if (maxLoad < load)
        PaintCanvas_SetColor(*(unsigned *)&g_dfWarnColor);

    Status_getShip();
    int cur = Ship_getCurrentLoad(Status_getShip());
    unsigned char sLoad[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_58
    String_from_uint(sLoad, cur);
    unsigned char sSep[sizeof(String12)] __attribute__((aligned(4)));    // aSStack_64
    String_cstr_ctor(sSep, g_dfSep, false);
    unsigned char s1[sizeof(String12)] __attribute__((aligned(4)));      // aSStack_4c
    String_concat(s1, sLoad, sSep);

    Status_getShip();
    int mx = Ship_getMaxLoad(Status_getShip());
    unsigned char sMax[sizeof(String12)] __attribute__((aligned(4)));    // aSStack_70
    String_from_uint(sMax, mx);
    unsigned char s2[sizeof(String12)] __attribute__((aligned(4)));      // aSStack_40
    String_concat(s2, s1, sMax);
    unsigned char sTail[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_7c
    String_cstr_ctor(sTail, g_dfTail, false);
    unsigned char loadStr[sizeof(String12)] __attribute__((aligned(4))); // aSStack_34
    String_concat(loadStr, s2, sTail);

    String_dtor(sTail);
    String_dtor(s2);
    String_dtor(sMax);
    String_dtor(s1);
    String_dtor(sSep);
    String_dtor(sLoad);

    {
        int x = F<int>(self, 0x2dc);
        int w = F<int>(self, 0x2e4);
        void *font = *g_dfFont;
        unsigned cv = *(unsigned *)g_dfCanvas;
        int tw = PaintCanvas_GetTextWidth(cv, font);
        PaintCanvas_DrawString(cv, font, loadStr, (x + w / 2) - tw / 2,
                               (F<int>(self, 0x2e8) + F<int>(self, 0x2e0)) - F<int>(self, 0x14));
    }
    PaintCanvas_SetColor(F<unsigned>(self, 0x3b0));

    int credits = Status_getCredits();
    unsigned char credStr[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_40
    Layout_formatCredits(credStr, credits);

    {
        int x = F<int>(self, 0x2dc);
        int w = F<int>(self, 0x2e4);
        void *font = *g_dfFont;
        unsigned cv = *(unsigned *)g_dfCanvas;
        if (stationMode) {
            int rightInset = m[0x74 / 4];
            int tw = PaintCanvas_GetTextWidth(cv, font);
            PaintCanvas_DrawString(cv, font, credStr, ((w + x) - rightInset) - tw / 2,
                                   (F<int>(self, 0x2e8) + F<int>(self, 0x2e0)) - F<int>(self, 0x14));
        } else {
            int tw = PaintCanvas_GetTextWidth(cv, font);
            PaintCanvas_DrawString(cv, font, credStr, (w + x - 10) - tw,
                                   (F<int>(self, 0x2e8) + F<int>(self, 0x2e0)) - F<int>(self, 0x14));
        }
    }

    String_dtor(credStr);
    String_dtor(loadStr);
    if (*guard != g0)
        __stack_chk_fail();
}
