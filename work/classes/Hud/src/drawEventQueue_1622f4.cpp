#include "class.h"

// Hud::drawEventQueue() — draws the sliding event-banner background image and, if the queue
// front holds a ListItem, its colored label (color keyed by the item's kind at +0x30). The
// vertical offset animates from the queue timer at +0x268 / display +0x1e0. r0=this.
extern "C" void PaintCanvas_SetColor4(void *canvas, int b1, int b2, int b3, int b4);
extern "C" void PaintCanvas_DrawImage2D2(void *canvas, int img, unsigned int frame);
extern "C" int  PaintCanvas_GetTextWidth(void *font, void *str);
extern "C" void PaintCanvas_DrawString2(void *canvas, void *font, void *str, int x, char y);
extern "C" void Hud_eventQueueFinish(void *canvas, unsigned int color); // DAT_001ac094 tail thunk

__attribute__((visibility("hidden"))) extern void **g_Hud_eqLetterbox; // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_eqSelf;      // *holder -> a Hud-like obj (+0x1e0/+0x1e4)
__attribute__((visibility("hidden"))) extern void **g_Hud_eqCanvas;    // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_eqScreenW;   // *holder -> [0] width
__attribute__((visibility("hidden"))) extern void **g_Hud_eqFont;      // *holder -> font String

extern "C" void Hud_drawEventQueue(Hud *self)
{
    char letterbox = *(char *)*g_Hud_eqLetterbox;
    char cinematicY = (letterbox == 0) ? 0 : (char)US(self, 0x3e2);

    void *src = *g_Hud_eqSelf;
    void *canvas = *g_Hud_eqCanvas;
    int dispBase = I(src, 0x1e4);
    float dispScale = F(src, 0x1e0);

    PaintCanvas_SetColor4(canvas, 0xff, 0xff, 0xff, 0); // alpha derived below replaced inline
    float mul = (letterbox == 0) ? -2.0f : -1.0f;
    int yOff = (int)(mul * dispScale);

    PaintCanvas_DrawImage2D2(canvas, I(self, 0x354), US(self, 0x3e0));

    int item = *(int *)(I(P(self, 0x264), 4) + 4);
    if (item != 0) {
        int kind = *(int *)(item + 0x30);
        int b2, b3, b4;
        if (kind == 2)      { b2 = 0;    b3 = 0xed; b4 = 0; }
        else if (kind == 1) { b2 = 0xff; b3 = 0x2a; b4 = 0; }
        else if (kind == 3) { b2 = 0xff; b3 = 0x80; b4 = 0; }
        else                { b2 = 0xff; b3 = 0xff; b4 = 0xff; }
        PaintCanvas_SetColor4(canvas, 0xff, b2, b3, b4);

        int strVal = *(int *)(item + 0x1c);
        void *font = *g_Hud_eqFont;
        int screenW = *(int *)*g_Hud_eqScreenW;
        int w = PaintCanvas_GetTextWidth(canvas, font);
        char y = (char)((char)yOff + (char)dispBase + cinematicY);
        PaintCanvas_DrawString2(canvas, font, (void *)(long)strVal, (screenW >> 1) - w / 2, y);
    }
    Hud_eventQueueFinish(canvas, 0xffffffff);
}
