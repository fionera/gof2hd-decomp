#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_image;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r2d_time;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r2d_string;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_textId;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_screenW;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_screenH;

extern "C" void PaintCanvas_Begin2d(int canvas);
extern "C" void PaintCanvas_SetColor(int canvas, int color);
extern "C" void PaintCanvas_SetColor4(int canvas, int r, int g, int b, int a);
extern "C" void PaintCanvas_DrawImage2D(
    int canvas, int image, int x, int y, int alignX, int alignY);
extern "C" int PaintCanvas_GetTextWidth(int canvas, void *str, int text);
extern "C" int PaintCanvas_GetImage2DHeight(int canvas, int image);
extern "C" void PaintCanvas_DrawString(
    int canvas, void *str, int text, int x, int y, bool shadow);
extern "C" void CutScene_render2D(void *scene);
extern "C" void MenuTouchWindow_draw(void *window);
extern "C" long long ApplicationManager_GetSystemTimeMillis(void *app);
extern "C" float AEMath_Sinf(float value);
extern "C" int GameText_getText(int text, int id);
extern "C" void ModMainMenu_r2dTail(int canvas);

extern "C" void _ZN11ModMainMenu10OnRender2DEv(ModMainMenu *self)
{
    PaintCanvas_Begin2d(I(self, 0x04));
    PaintCanvas_SetColor(I(self, 0x04), -1);
    CutScene_render2D(P(self, 0x1c));

    if (UC(self, 0x28) == 0) {
        MenuTouchWindow_draw(P(self, 0x18));
    } else {
        int color;
        int time = I(self, 0x24);
        if (time <= 0x0f3b)
            color = (int)(((float)time / 3900.0f) * 255.0f) - 0x100;
        else
            color = -1;

        PaintCanvas_SetColor(I(self, 0x04), color);

        int *imageHolder = g_ModMainMenu_r2d_image;
        PaintCanvas_DrawImage2D(*imageHolder, I(self, 0x20), 0, 0, 'D', 'D');

        if (I(self, 0x24) >= 0x0f3c) {
            int canvas = *imageHolder;
            void **timeHolder = g_ModMainMenu_r2d_time;
            float pulseA =
                AEMath_Sinf((float)ApplicationManager_GetSystemTimeMillis(*timeHolder) *
                            0.003f);
            float pulseB =
                AEMath_Sinf((float)ApplicationManager_GetSystemTimeMillis(*timeHolder) *
                            0.003f);
            float signedPulse = pulseA > 0.0f ? pulseB : -pulseB;
            int alpha = (unsigned int)(signedPulse * 255.0f);
            PaintCanvas_SetColor4(canvas, 0xff, 0xff, 0xff, alpha);

            void **stringHolder = g_ModMainMenu_r2d_string;
            int *textIdHolder = g_ModMainMenu_r2d_textId;
            struct DrawTarget {
                int canvas;
                void *str;
            };
            DrawTarget draw = {*imageHolder, *stringHolder};
            int text = GameText_getText(*textIdHolder, 0xc7);

            int screenW = *g_ModMainMenu_r2d_screenW;
            void *measureStr = *stringHolder;
            int measureCanvas = *imageHolder;
            int measureText = GameText_getText(*textIdHolder, 0xc7);
            int textWidth = PaintCanvas_GetTextWidth(measureCanvas, measureStr, measureText);

            int image = I(self, 0x20);
            int screenH = *g_ModMainMenu_r2d_screenH;
            int imageHeight = PaintCanvas_GetImage2DHeight(*imageHolder, image);
            PaintCanvas_DrawString(draw.canvas, draw.str, text,
                                   (screenW >> 1) - (textWidth >> 1),
                                   (screenH >> 1) + (imageHeight >> 1) + 10, false);
        }
    }

    ModMainMenu_r2dTail(I(self, 0x04));
}
