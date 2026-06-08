#include "class.h"

struct PaintCanvas;

extern "C" unsigned PaintCanvas_GetColor(PaintCanvas *pc);                  // 0x6fa90
extern "C" void PaintCanvas_SetColor(unsigned color);                      // 0x6fac0
extern "C" void String_cstr_ctor(void *out, const char *s, bool);          // 0x6ee18
extern "C" void String_copy_ctor(void *out, const void *src, bool);        // 0x6f028
extern "C" void String_dtor(void *s);                                      // 0x6ee30
extern "C" void Layout_drawBox(PaintCanvas *pc, int p2, int x, int y,
                               int w, int h, void *str);                   // 0x7462c
extern "C" void PaintCanvas_DrawImage2D5(PaintCanvas *pc, int img, int x,
                                         int y, int anchor);                // 0x71d70
extern "C" void *GameText_getText(int id);                                 // 0x72f70
extern "C" int PaintCanvas_GetTextWidth(unsigned pc, void *font);          // 0x6faa8
extern "C" void PaintCanvas_DrawString(unsigned pc, void *font, void *str,
                                       int x, int y);                       // 0x6fe14
extern "C" void Layout_formatCredits(void *out, int n);                    // 0x74e54
extern "C" void String_concat(void *ret, void *a, void *b);                // 0x6ef98
extern "C" void String_assign(void *dst, const void *src);                 // 0x6f2b0

// Hidden globals from drawMissionRewardMessage disasm.
__attribute__((visibility("hidden"))) extern int *g_mrGuard;       // @0xe4d88 (stack guard [0])
__attribute__((visibility("hidden"))) extern PaintCanvas **g_mrCanvas; // @0xe4d9e
__attribute__((visibility("hidden"))) extern int *g_mrDimA;        // @0xe4e2a ([0]=screen W)
__attribute__((visibility("hidden"))) extern const char g_mrLit0[];    // @0xe4e38
__attribute__((visibility("hidden"))) extern const char g_mrLit1[];    // @0xe4e6a
__attribute__((visibility("hidden"))) extern int *g_mrDimB;        // @0xe4e94 ([0]=screen H)
__attribute__((visibility("hidden"))) extern int *g_mrTextId;      // @0xe4eba ([0]=text id)
__attribute__((visibility("hidden"))) extern void **g_mrFont;      // @0xe4ede ([0]=font)
__attribute__((visibility("hidden"))) extern const char g_mrLit2[];    // @0xe4f14

// Layout::drawMissionRewardMessage(bool transition)
extern "C" void Layout_drawMissionRewardMessage(Layout *self, int transition) {
    int *guard = g_mrGuard;
    int g0 = *guard;
    if (F<uint8_t>(self, 0x2ec) != 0) {
        PaintCanvas *pc = *g_mrCanvas;
        unsigned saved = PaintCanvas_GetColor(pc);
        unsigned origColor = F<unsigned>(self, 0x3b0);
        PaintCanvas_SetColor(0xffffffff);

        // Pulse alpha based on field 0x3d0 (animation timer).
        int t = F<int>(self, 0x3d0);
        float a;
        if (t < 2000) {
            a = (float)t / 6000.0f;
        } else if (t > 5000) {
            a = (float)(7000 - t) / 6000.0f;
        } else {
            a = 1.0f;
        }
        unsigned col = (unsigned)((int)(a * 256.0f)) - 0x100;
        PaintCanvas_SetColor(col);

        unsigned newColor = PaintCanvas_GetColor(pc);
        int boxW = F<int>(self, 0x3e8);
        int boxH = F<int>(self, 0x3ec);
        int boxX = F<int>(self, 0x3f0);
        int boxY = F<int>(self, 0x3f4);
        F<unsigned>(self, 0x3b0) = newColor;

        if (transition != 0) {
            int sw = *g_mrDimA;
            unsigned char s0[sizeof(String12)] __attribute__((aligned(4)));
            String_cstr_ctor(s0, g_mrLit0, false);
            Layout_drawBox(pc, 2, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, s0);
            String_dtor(s0);

            sw = *g_mrDimA;
            unsigned char s1[sizeof(String12)] __attribute__((aligned(4)));
            String_cstr_ctor(s1, g_mrLit1, false);
            Layout_drawBox(pc, 8, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, s1);
            String_dtor(s1);
        }

        int sh = *g_mrDimB;
        PaintCanvas_DrawImage2D5(*g_mrCanvas, F<int>(self, 0x3a4), sh >> 1,
                                 (char)boxX, 0x11);

        void *txt = GameText_getText(*g_mrTextId);
        unsigned char line[sizeof(String12)] __attribute__((aligned(4)));
        String_copy_ctor(line, txt, false);

        sh = *g_mrDimB;
        void *font = *g_mrFont;
        unsigned cv = *(unsigned *)g_mrCanvas;
        int tw = PaintCanvas_GetTextWidth(cv, font);
        PaintCanvas_DrawString(cv, font, line, (sh >> 1) - (tw >> 1), boxX + boxY);

        unsigned char suffix[sizeof(String12)] __attribute__((aligned(4)));
        String_cstr_ctor(suffix, g_mrLit2, false);
        unsigned char credits[sizeof(String12)] __attribute__((aligned(4)));
        Layout_formatCredits(credits, F<int>(self, 0x3d4));
        unsigned char joined[sizeof(String12)] __attribute__((aligned(4)));
        String_concat(joined, suffix, credits);
        String_assign(line, joined);
        String_dtor(joined);
        String_dtor(credits);
        String_dtor(suffix);

        sh = *g_mrDimB;
        cv = *(unsigned *)g_mrCanvas;
        tw = PaintCanvas_GetTextWidth(cv, font);
        PaintCanvas_DrawString(cv, font, line, (sh >> 1) - (tw >> 1),
                               F<int>(self, 0x3f8) + boxX);
        PaintCanvas_SetColor(saved);
        F<unsigned>(self, 0x3b0) = origColor;
        String_dtor(line);
    }
    if (*guard != g0)
        __stack_chk_fail();
}
