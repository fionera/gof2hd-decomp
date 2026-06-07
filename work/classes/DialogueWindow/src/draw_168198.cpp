#include "class.h"

struct Vec2 {
    float x;
    float y;
};

typedef void (*ButtonDraw)(void *);

extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void Layout_drawMask(void *layout);
extern "C" void String_ctor_copy(StringSlot *self, String *text, bool copy);
extern "C" void String_dtor(StringSlot *self);
extern "C" void Layout_drawBox(void *layout, int style, int x, int y, int w, int h, StringSlot *title);
extern "C" void ScrollTouchWindow_draw(void *self);
extern "C" void ImageFactory_drawChar(void *self, void *parts, int x, int y, bool flipped);
extern "C" void ChoiceWindow_draw(void *self);
extern "C" void TouchButton_getPosition(Vec2 *out, void *self);

__attribute__((visibility("hidden"))) extern int *g_dw_drawGuard;
__attribute__((visibility("hidden"))) extern void **g_dw_paintCanvas;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutDraw;
__attribute__((visibility("hidden"))) extern void **g_dw_imageFactoryDraw;
__attribute__((visibility("hidden"))) extern ButtonDraw g_dw_touchButtonDraw;
__attribute__((visibility("hidden"))) extern int *g_dw_drawPositionsReady;
__attribute__((visibility("hidden"))) extern int *g_dw_moreButtonX;
__attribute__((visibility("hidden"))) extern int *g_dw_moreButtonY;
__attribute__((visibility("hidden"))) extern int *g_dw_nextButtonX;
__attribute__((visibility("hidden"))) extern int *g_dw_nextButtonY;
__attribute__((visibility("hidden"))) extern int *g_dw_drawReadyFlag;

extern "C" void DialogueWindow_draw(DialogueWindow *self)
{
    StringSlot title;
    Vec2 pos;

    PaintCanvas_SetColor(*g_dw_paintCanvas, -1);
    void *layout = *g_dw_layoutDraw;
    Layout_drawMask(layout);
    String_ctor_copy(&title, (String *)((char *)self + 0x34), false);
    Layout_drawBox(layout, 7, F<int>(self, 0x14), F<int>(self, 0x18),
                   F<int>(self, 0x1c), F<int>(self, 0x20), &title);
    String_dtor(&title);

    ScrollTouchWindow_draw(F<void *>(self, 0x40));

    layout = *g_dw_layoutDraw;
    int margin = F<int>(layout, 0x4c);
    ImageFactory_drawChar(*g_dw_imageFactoryDraw, F<void *>(self, 0x0c),
                          F<int>(self, 0x14) + margin,
                          F<int>(self, 0x18) + margin + F<int>(layout, 0x08),
                          F<uint8_t>(self, 0x70));

    ButtonDraw drawButton = g_dw_touchButtonDraw;
    drawButton(F<void *>(self, 0x00));
    drawButton(F<void *>(self, 0x04));
    drawButton(F<void *>(self, 0x08));

    if (F<uint8_t>(self, 0x54) != 0) {
        ChoiceWindow_draw(F<void *>(self, 0x50));
    }

    if (*g_dw_drawPositionsReady == 0) {
        if (F<void *>(self, 0x08) != 0) {
            TouchButton_getPosition(&pos, F<void *>(self, 0x08));
            F<int>(g_dw_moreButtonX, 0x08) = (int)pos.x;
            TouchButton_getPosition(&pos, F<void *>(self, 0x08));
            F<int>(g_dw_moreButtonY, 0x08) = (int)pos.y;
        }
        if (F<void *>(self, 0x04) != 0) {
            TouchButton_getPosition(&pos, F<void *>(self, 0x04));
            F<int>(g_dw_nextButtonX, 0x0c) = (int)pos.x;
            TouchButton_getPosition(&pos, F<void *>(self, 0x04));
            F<int>(g_dw_nextButtonY, 0x0c) = (int)pos.y;
        }
        *g_dw_drawReadyFlag = 1;
    }
}
