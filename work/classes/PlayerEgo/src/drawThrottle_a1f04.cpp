#include "class.h"

// PlayerEgo::drawThrottle()
//   While the boost gauge is active (flag 0x370) this draws the throttle bar:
//   a partial DrawRegion2D fill of the gauge image (0x238) whose height tracks
//   the current thrust (0xbc), plus the thrust percentage rendered as text
//   centred under it. The boost timer (0x374) folds back past its midpoint so
//   the bar pulses symmetrically.

extern "C" void  PaintCanvas_SetColor(unsigned int c);
extern "C" int   PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" int   PaintCanvas_GetImage2DHeight(void *canvas);
extern "C" void  PaintCanvas_DrawRegion2D(void *canvas, int img, int sx, int sy, int w,
                                          int h, int dh, int a, int b, int c, int dx);
extern "C" int   PaintCanvas_GetTextWidth(void *canvas, void *str);
extern "C" void  PaintCanvas_DrawString(void *canvas, void *str, void *posStr, int x, bool centered);
extern "C" void  String_ctor_int(void *s, int value);                  // String::String(int)
extern "C" void  String_dtor(void *s);

__attribute__((visibility("hidden"))) extern void **g_PE_t_canvas;     // PaintCanvas singleton
__attribute__((visibility("hidden"))) extern float *g_PE_t_anchor;     // {x,y} gauge anchor
__attribute__((visibility("hidden"))) extern void **g_PE_t_pctStr;     // "%" String
extern const float g_PE_t_timerDiv;    // 0xb1f30 timer normaliser
extern const float g_PE_t_pctScale;    // 0xb20dc thrust->percent
extern const float g_PE_t_textDiv;     // 0xb20e0 text vertical divisor

extern "C" void PlayerEgo_drawThrottle(PlayerEgo *self)
{
    if (C(self, 0x370) == 0)
        return;

    int t = I(self, 0x374);
    if (t > 500)
        t = 2000 - t;
    float frac = (float)t / g_PE_t_timerDiv;
    if (frac > 1.0f)
        frac = 1.0f;

    void *canvas = *g_PE_t_canvas;
    PaintCanvas_SetColor(0xffffff00 | (unsigned int)(int)(frac * 255.0f) - 0x100);

    int img = I(self, 0x238);
    int w = PaintCanvas_GetImage2DWidth(canvas);
    int h = PaintCanvas_GetImage2DHeight(canvas);

    float thrust = F(self, 0xbc);
    int fillH = (int)(thrust * (float)h);
    float *anchor = g_PE_t_anchor;

    PaintCanvas_DrawRegion2D(canvas, img, 0, h - fillH, w, fillH,
        (int)((anchor[1] + (float)h) - (float)fillH), 0, 0, 0,
        (int)(anchor[0] - (float)(w / 2)));

    // percentage label
    unsigned char pct[12];
    String_ctor_int(pct, (int)(thrust * g_PE_t_pctScale));

    int th = PaintCanvas_GetImage2DHeight(canvas);
    void *pctStr = *g_PE_t_pctStr;
    int tw = PaintCanvas_GetTextWidth(canvas, pctStr);
    PaintCanvas_DrawString(canvas, pctStr, pct,
        (int)((anchor[0] - (float)(tw / 2)) - 1.0f),
        (bool)(int)(anchor[1] + (float)th / g_PE_t_textDiv));
    PaintCanvas_SetColor(0xffffffff);

    String_dtor(pct);
}
