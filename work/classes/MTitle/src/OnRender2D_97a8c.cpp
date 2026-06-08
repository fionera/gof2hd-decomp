#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_layout;
__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_screen;

extern "C" void PaintCanvas_Begin2d(void *canvas);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y, int ax, int ay);
extern "C" void PaintCanvas_End2d(void *canvas);
extern "C" void Layout_drawBG(void *layout);
extern "C" void Layout_drawHeader(void *layout);
extern "C" void Layout_drawEmptyFooter(void *layout, int flag);
extern "C" int ApplicationManager_GetElapsedTimeMillis(void *arg);
extern "C" void MTitle_r2dDone(void *screen, int arg);
extern "C" void MTitle_r2dTail(void *canvas);

void MTitle::OnRender2D()
{
    PaintCanvas_Begin2d(P(this, 0x4));

    void **canvas = g_MTitle_r2d_canvas;
    PaintCanvas_SetColor(*canvas, -1);

    void **layout = g_MTitle_r2d_layout;
    Layout_drawBG(*layout);
    Layout_drawHeader(*layout);
    Layout_drawEmptyFooter(*layout, 0);

    int t = ApplicationManager_GetElapsedTimeMillis(P(this, 0x8));
    if (0x32 < t)
        t = 0x32;
    else
        t = ApplicationManager_GetElapsedTimeMillis(P(this, 0x8));

    t += I(this, 0x1c);
    I(this, 0x1c) = t;

    int image;
    float fade;
    if (t > 0xfa0) {
        int step = *(volatile int *)B(this, 0x18) + 1;
        I(this, 0x18) = step;
        I(this, 0x1c) = 0;
        if (step == 2) {
            MTitle_r2dDone(*g_MTitle_r2d_screen, 1);
            return;
        }
        image = I(this, step == 0 ? 0x14 : 0x10);
        t = 0;
        goto pathC;
    }

    image = I(this, I(this, 0x18) == 0 ? 0x14 : 0x10);
    if (t >= 0x3e8) {
        if (t >= 0xbb9) {
            fade = (float)(t - 0xbb8) / -1000.0f + 1.0f;
            goto common;
        }
        fade = 1.0f;
        goto common;
    }
pathC:
    fade = (float)t / 1000.0f;
common:
    int color = (int)(fade * 255.0f) - 0x100;
    PaintCanvas_SetColor(P(this, 0x4), color);
    PaintCanvas_DrawImage2D(*canvas, image, 0, 0, 0x44, 0x44);
    PaintCanvas_End2d(P(this, 0x4));
    MTitle_r2dTail(P(this, 0x4));
}
