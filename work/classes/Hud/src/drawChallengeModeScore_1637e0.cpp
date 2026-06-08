#include "class.h"

// Hud::drawChallengeModeScore(int) — renders the challenge-mode HUD numbers (score, bonus
// multiplier, and time) by stamping per-digit sprite frames across the top of the screen.
// r0=this. Strings are right-padded to 7 digits before per-character drawing.
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" int  Sprite_getFrameWidth(void *sprite);
extern "C" int  Sprite_getFrameHeight(void *sprite);
extern "C" void Sprite_setFrame(void *sprite, int frame);
extern "C" void Sprite_setPosition(void *sprite, int x, int y);
extern "C" void Sprite_draw(void *sprite);
extern "C" void PaintCanvas_DrawImage2D2(void *canvas, int img, int anchor);
extern "C" int  PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" void String_ctor_int(void *s, int v);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void String_concat(void *out, void *lhs, void *rhs);
extern "C" void String_op_assign(void *dst, void *src);
extern "C" void String_dtor(void *s);
extern "C" int  String_length(void *s);
extern "C" void String_SubString(void *out, void *s, int idx);
extern "C" int  String_ValueOf(void *s);

__attribute__((visibility("hidden"))) extern void **g_Hud_csCanvas; // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_csLayout; // *holder -> layout (+0x2c row pad)
__attribute__((visibility("hidden"))) extern void **g_Hud_csStatus; // *holder -> status (+0x180 score,+0x184,+0x18c)
__attribute__((visibility("hidden"))) extern void **g_Hud_csScreenW;// *holder -> [0] width
extern const char g_Hud_csZero[] __attribute__((visibility("hidden"))); // "0" pad

static void drawDigits(Hud *self, void *sprite, void *str, int x0, int y, int dw) {
    int len = String_length(str);
    int x = x0;
    for (int i = 1; (unsigned int)(i - 1) < (unsigned int)len; i++) {
        char ch[12];
        String_SubString(ch, str, i - 1);
        int frame = String_ValueOf(ch);
        String_dtor(ch);
        Sprite_setFrame(sprite, frame);
        Sprite_setPosition(sprite, x, y);
        Sprite_draw(sprite);
        x += dw;
    }
}

extern "C" void Hud_drawChallengeModeScore(Hud *self)
{
    void *canvas = *g_Hud_csCanvas;
    int *layout = (int *)*g_Hud_csLayout;
    int *status = (int *)*g_Hud_csStatus;
    int screenW = *(int *)*g_Hud_csScreenW;
    void *sprite = P(self, 0x534);

    PaintCanvas_SetColor(canvas, -1);
    int fw = Sprite_getFrameWidth(sprite);
    int pad = layout[0xb]; // +0x2c
    int fh = Sprite_getFrameHeight(sprite);
    int y = layout[0xb];

    // score string at status+0x184, right-padded to 7 digits
    char score[12];
    String_ctor_int(score, status[0x61] /*+0x184*/);
    int slen = String_length(score);
    if (slen < 7) {
        for (int k = 0; k < 7 - slen; k++) {
            char z[12], acc[12];
            String_ctor_cstr(z, g_Hud_csZero, false);
            String_concat(acc, z, score);
            String_op_assign(score, acc);
            String_dtor(acc);
            String_dtor(z);
        }
    }

    PaintCanvas_SetColor(canvas, -1);
    int dw = fw - pad;
    int half = screenW / 2;
    int span = (dw * 7) / 2;
    int startX = half - span;
    drawDigits(self, sprite, score, startX, y, dw);

    if (status[0x60] /*+0x180*/ > 0 && status[0x63] /*+0x18c*/ > 1) {
        PaintCanvas_SetColor(canvas, -1);
        int yRow = y + fh + pad;
        int scoreVal = status[0x60];
        if (scoreVal < 0xbb9) {
            if (scoreVal % 100 >= 0x32) {
                int mult = status[0x63];
                float bonus = (float)mult;
                float base = (float)(mult * 1000);
                char bonusStr[12];
                String_ctor_int(bonusStr, (int)((bonus * 0.0f + 1.0f) * base));
                int bl = String_length(bonusStr);
                int bx = (screenW / 2 - ((bl * dw) >> 1));
                drawDigits(self, sprite, bonusStr, bx, fh + yRow + pad, dw);
                String_dtor(bonusStr);
            }
        }
        PaintCanvas_DrawImage2D2(canvas, I(self, 0x538), pad + startX);

        char timeStr[12];
        String_ctor_int(timeStr, status[0x63]);
        int tx = (half + pad) - span + PaintCanvas_GetImage2DWidth(canvas);
        drawDigits(self, sprite, timeStr, tx, yRow, dw);
        String_dtor(timeStr);
    }
    PaintCanvas_SetColor(canvas, -1);
    String_dtor(score);
}
