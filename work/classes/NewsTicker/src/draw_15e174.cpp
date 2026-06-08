#include "class.h"

extern "C" void PaintCanvas_SetColor(void *canvas, uint32_t color);
extern "C" void PaintCanvas_FillRectangle(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_EnableClip(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_DrawString(void *canvas, void *font, String *text, int x, int y, bool flag);
extern "C" int GameText_getLanguage();
extern "C" void PaintCanvas_DisableClip(void *canvas);

__attribute__((visibility("hidden"))) extern void **g_NewsTicker_draw_canvas;
__attribute__((visibility("hidden"))) extern int **g_NewsTicker_draw_fontHeight;
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_draw_screenHeight;
__attribute__((visibility("hidden"))) extern void **g_NewsTicker_draw_font;

void NewsTicker::draw()
{
    void **canvasHolder = g_NewsTicker_draw_canvas;
    PaintCanvas_SetColor(*canvasHolder, 0x6f);

    int *fontHeightTable = *g_NewsTicker_draw_fontHeight;
    int *screenHeight = g_NewsTicker_draw_screenHeight;
    {
        int x0 = F<int>(this, 0x4);
        int y0 = F<int>(this, 0x8);
        int clipBottom = *screenHeight;
        int fontHeight = fontHeightTable[4];
        int fillHeight = (2 - y0) + clipBottom - fontHeight;
        PaintCanvas_FillRectangle(*canvasHolder, x0, y0 - 2,
                                  F<int>(this, 0x0c), fillHeight);
    }

    PaintCanvas_EnableClip(*canvasHolder, F<int>(this, 0x4), F<int>(this, 0x8),
                           F<int>(this, 0x0c), *screenHeight);
    PaintCanvas_SetColor(*canvasHolder, 0x777777ff);

    void **fontHolder = g_NewsTicker_draw_font;
    String *text = (String *)((char *)this + 0x14);
    float drawX = F<float>(this, 0x0) + (float)F<int>(this, 0x4);
    PaintCanvas_DrawString(*canvasHolder, *fontHolder, text, (int)drawX,
                           F<int>(this, 0x8), false);

    int language = GameText_getLanguage();
    int textWidth = F<int>(this, 0x10);
    float x = F<float>(this, 0x0);
    if (language == 9) {
        if (x > (float)textWidth) {
            x += (float)F<int>(this, 0x4);
            int drawY = *(volatile int *)((char *)this + 0x8);
            void *font = *fontHolder;
            void *canvas = *canvasHolder;
            PaintCanvas_DrawString(canvas, font, text, (int)x, drawY, false);
        }
    } else {
        if (x < (float)(textWidth - F<int>(this, 0x0c))) {
            x += (float)F<int>(this, 0x4);
            x += (float)textWidth;
            int drawY = *(volatile int *)((char *)this + 0x8);
            void *font = *fontHolder;
            void *canvas = *canvasHolder;
            PaintCanvas_DrawString(canvas, font, text, (int)x, drawY, false);
        }
    }

    return PaintCanvas_DisableClip(*canvasHolder);
}
