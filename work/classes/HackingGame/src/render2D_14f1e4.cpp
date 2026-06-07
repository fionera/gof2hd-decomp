#include "class.h"

typedef int (*ImageMeasureFn)(void *canvas, int image);

extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, int image);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas, int image);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y);
extern "C" void PaintCanvas_DrawImage2D_anchor(void *canvas, int image, int x, int y, bool anchor);
extern "C" unsigned __aeabi_uidiv(unsigned value, unsigned divisor);
extern "C" int __aeabi_idiv(int value, int divisor);

__attribute__((visibility("hidden"))) extern void **g_HackingGame_render_canvas;
__attribute__((visibility("hidden"))) extern int *g_HackingGame_render_screen_w_a;
__attribute__((visibility("hidden"))) extern int *g_HackingGame_render_screen_w_b;
__attribute__((visibility("hidden"))) extern int *g_HackingGame_render_screen_h;
__attribute__((visibility("hidden"))) extern ImageMeasureFn g_HackingGame_render_height_fn;
__attribute__((visibility("hidden"))) extern void **g_HackingGame_render_layout;

static inline int half_i(int value)
{
    return value / 2;
}

void HackingGame::render2D()
{
    char deltaBytes[52];
    int *delta = (int *)deltaBytes;

    void **canvasHolder = g_HackingGame_render_canvas;
    void *canvas = *canvasHolder;
    PaintCanvas_SetColor(canvas, -1);

    bool solved = true;
    for (unsigned i = 0; i <= 5; ++i) {
        if (I(this, 0x1c + i * 4) != I(this, 4 + i * 4)) {
            solved = false;
            break;
        }
    }

    int typeOffset = I(this, 0x124) * 0x30;
    int baseImage = I(this, 0x4c + typeOffset);
    int tileW = PaintCanvas_GetImage2DWidth(*canvasHolder, baseImage);
    int tileH = PaintCanvas_GetImage2DHeight(*canvasHolder, baseImage);

    for (int i = 0; i != 6; ++i) {
        delta[i * 2] = 0;
        delta[i * 2 + 1] = 0;
    }

    if (B(this, 0x128) != 0) {
        float amount = (float)I(this, 0x12c) / 300.0f;
        if (amount > 1.0f)
            amount = 1.0f;
        delta[3] = (int)(amount * (float)tileH);
        delta[0] = (int)(amount * (float)tileW);
        delta[7] = (int)(-(amount * (float)tileH));
        delta[8] = (int)(-(amount * (float)tileW));
    } else if (B(this, 0x129) != 0) {
        float amount = (float)I(this, 0x12c) / 300.0f;
        if (amount > 1.0f)
            amount = 1.0f;
        delta[5] = (int)(amount * (float)tileH);
        delta[2] = (int)(amount * (float)tileW);
        delta[9] = (int)(-(amount * (float)tileH));
        delta[10] = (int)(-(amount * (float)tileW));
    }

    if (solved) {
        if (I(this, 0x130) > 0x5dc)
            goto done;
    } else {
        int image = I(this, 0x110);
        void *drawCanvas = *canvasHolder;
        int screenW = *g_HackingGame_render_screen_w_a;
        int width = PaintCanvas_GetImage2DWidth(drawCanvas, image);
        int height = PaintCanvas_GetImage2DHeight(*canvasHolder, image);
        PaintCanvas_DrawImage2D(drawCanvas, image, half_i(screenW) - half_i(width), half_i(*g_HackingGame_render_screen_h) - half_i(height));
    }

    {
        int topImage = I(this, 0x10c);
        int screenW = *g_HackingGame_render_screen_w_b;
        void *drawCanvas = *canvasHolder;
        int topWidth = PaintCanvas_GetImage2DWidth(drawCanvas, topImage);
        ImageMeasureFn measure = g_HackingGame_render_height_fn;
        int screenH = *g_HackingGame_render_screen_h;
        int titleH = measure(*canvasHolder, I(this, 0x110));
        int topH = measure(*canvasHolder, I(this, 0x10c));
        void **layout = g_HackingGame_render_layout;

        PaintCanvas_DrawImage2D(drawCanvas, topImage, half_i(screenW) - topWidth, half_i(screenH) - half_i(titleH) - topH + I((HackingGame *)*layout, 0x30c));
        PaintCanvas_DrawImage2D_anchor(drawCanvas, topImage, half_i(screenW), half_i(screenH) - half_i(titleH) - topH + I((HackingGame *)*layout, 0x30c), true);

        if (!solved) {
            int bottomImage = I(this, 0x114);
            int bottomW = PaintCanvas_GetImage2DWidth(*canvasHolder, bottomImage);
            int bottomH = PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0x110));
            PaintCanvas_DrawImage2D(*canvasHolder, bottomImage, half_i(screenW) - bottomW,
                                    half_i(bottomH) + half_i(screenH) + I((HackingGame *)*layout, 0x314));
            PaintCanvas_DrawImage2D_anchor(*canvasHolder, bottomImage, half_i(screenW),
                                           half_i(bottomH) + half_i(screenH) + I((HackingGame *)*layout, 0x314), true);
        }

        float oneAndHalf = (float)tileW * 1.5f;
        for (unsigned i = 0; i != 6; ++i) {
            int stateIndex = I(this, 0x1c + i * 4);
            bool flashSolved = solved && ((__aeabi_idiv(I(this, 0x130), 200) & 1) == 0);
            int imageOffset = (flashSolved ? 0x64 : 0x4c) + typeOffset + stateIndex * 4;
            int image = I(this, imageOffset);
            unsigned row = __aeabi_uidiv((uint8_t)i, 3);
            unsigned col = (uint8_t)(i - row * 3);
            int y = (int)(((float)half_i(screenW) - oneAndHalf) + (float)(tileW * col) +
                          (float)delta[i * 2]);
            int imageH = PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0x110));
            PaintCanvas_DrawImage2D(*canvasHolder, image, y,
                                    row * tileH + half_i(*g_HackingGame_render_screen_h) - half_i(imageH) +
                                        delta[i * 2 + 1] + I((HackingGame *)*layout, 0x310));
        }

        int leftArrow = B(this, 0x128) != 0 ? I(this, 0x118) : I(this, 0x11c);
        int arrowW = PaintCanvas_GetImage2DWidth(*canvasHolder, leftArrow);
        int arrowTitleH = PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0x110));
        int arrowH = PaintCanvas_GetImage2DHeight(*canvasHolder, leftArrow);
        PaintCanvas_DrawImage2D(*canvasHolder, leftArrow,
                                half_i(screenW) - half_i(tileW) - half_i(arrowW),
                                half_i(*g_HackingGame_render_screen_h) - half_i(tileH) -
                                    half_i(arrowTitleH) - half_i(arrowH) + I((HackingGame *)*layout, 0x310));

        int rightArrow = B(this, 0x129) != 0 ? I(this, 0x118) : I(this, 0x11c);
        arrowW = PaintCanvas_GetImage2DWidth(*canvasHolder, rightArrow);
        arrowTitleH = PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0x110));
        arrowH = PaintCanvas_GetImage2DHeight(*canvasHolder, rightArrow);
        PaintCanvas_DrawImage2D(*canvasHolder, rightArrow,
                                half_i(screenW) + half_i(tileW) - half_i(arrowW),
                                half_i(*g_HackingGame_render_screen_h) - half_i(tileH) -
                                    half_i(arrowTitleH) - half_i(arrowH) + I((HackingGame *)*layout, 0x310));

        if (!solved) {
            for (unsigned i = 0; i != 6; ++i) {
                unsigned row = __aeabi_uidiv((uint8_t)i, 3);
                unsigned col = (uint8_t)(i - row * 3);
                int stateIndex = I(this, 4 + i * 4);
                int image = I(this, 0x4c + typeOffset + stateIndex * 4);
                int imageH = PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0x110));
                int y = (int)(((float)half_i(screenW) - oneAndHalf) + (float)(tileW * col));
                PaintCanvas_DrawImage2D(*canvasHolder, image, y,
                                        row * tileH + half_i(*g_HackingGame_render_screen_h) + half_i(imageH) +
                                            I((HackingGame *)*layout, 0x318));
            }

            int mark = I(this, 0x120);
            int markW = PaintCanvas_GetImage2DWidth(*canvasHolder, mark);
            int titleH2 = measure(*canvasHolder, I(this, 0x110));
            int markH = measure(*canvasHolder, mark);
            PaintCanvas_DrawImage2D(*canvasHolder, mark,
                                    half_i(screenW) - half_i(tileW) - half_i(markW),
                                    half_i(*g_HackingGame_render_screen_h) + half_i(tileH) +
                                        half_i(titleH2) - half_i(markH) + I((HackingGame *)*layout, 0x318));
            PaintCanvas_DrawImage2D(*canvasHolder, mark,
                                    half_i(screenW) + half_i(tileW) - half_i(markW),
                                    half_i(*g_HackingGame_render_screen_h) + half_i(tileH) +
                                        half_i(titleH2) - half_i(markH) + I((HackingGame *)*layout, 0x318));
        }
    }

done:
    return;
}
