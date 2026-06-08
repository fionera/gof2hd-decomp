#include "class.h"

// TouchButton::draw()
//   Renders the button for its current state. Saves the canvas colour and font
//   spacing, draws the appropriate skin (single icon, 9-patch frame + label, a
//   progress fill, or a plain image) depending on `kind` (offset 0x70) and the
//   pressed/highlight flags, then restores spacing and colour.

extern "C" unsigned int PaintCanvas_GetColor(void *canvas);
extern "C" void  PaintCanvas_SetColor(unsigned int c);
extern "C" void  PaintCanvas_SetColorARGB(unsigned char a, unsigned char r,
                                          unsigned char g, unsigned char b);
extern "C" void  Layout_setDrawColor(void *layout, int color);
extern "C" int   PaintCanvas_FontGetSpacing(void *canvas);
extern "C" void  PaintCanvas_FontSetSpacing(void *canvas, unsigned int spacing, short kerning);
extern "C" void  PaintCanvas_DrawImage2D(void *canvas, int img, int x, int y);
extern "C" void  PaintCanvas_DrawImage2DEx(void *canvas, int img, int x, int y,
                                           int anchor, int flags);
extern "C" void  PaintCanvas_DrawString(void *canvas, String *text, void *posStr,
                                         int x, bool centered);
extern "C" int   PaintCanvas_GetTextWidth(void *canvas, String *text);
extern "C" int   PaintCanvas_GetTextHeight(void *canvas);
extern "C" int   PaintCanvas_DrawRegion2D(void *canvas, unsigned int img, int sx, int sy,
                                          int w, int h, int dw, int a, int b, int c, int y);
extern "C" int   Layout_drawBGPattern(void *layout, unsigned int img, int x, int y, int w, int h);

// Singletons (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_d_canvas;
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutA;   // pressed/disabled tint layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutBG;  // background-pattern layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutC;   // progress-fill layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutEnd; // final restore layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_unitStr;   // "%"-style unit String
__attribute__((visibility("hidden"))) extern unsigned int g_TB_d_frameMask; // kinds that get a frame

extern "C" void TouchButton_draw(TouchButton *self)
{
    void *canvas = *g_TB_d_canvas;
    unsigned int savedColor = PaintCanvas_GetColor(canvas);

    if (C(self, 0xb2) == 0)
        return;

    if (C(self, 0xb3) != 0) {
        PaintCanvas_SetColor(0xffffff2f);   // mvn #0xd0 == 0xffffff2f
        Layout_setDrawColor(*g_TB_d_layoutA, -0xd1);
    } else {
        PaintCanvas_SetColor(0xffffffff);
    }

    short savedSpacing = (short)PaintCanvas_FontGetSpacing(canvas);
    PaintCanvas_FontSetSpacing(canvas, U(self, 0x8), (short)I(self, 0xc4));

    unsigned int kind = U(self, 0x70);
    int icon = -1;       // image to draw at the tail (offset 0x78 position)
    int iconY = 0;
    bool tailIcon = false;

    if (kind == 0x10) {
        PaintCanvas_DrawImage2D(canvas, I(self, 0x48), I(self, 0x78), I(self, 0x7c));
        if (C(self, 0xb0) != 0 || C(self, 0xb1) != 0) {
            icon = I(self, 0x3c);
            iconY = I(self, 0x78);
            tailIcon = true;
        }
    } else if (kind == 4) {
        PaintCanvas_DrawImage2D(canvas, I(self, 0x60), I(self, 0x78), I(self, 0x7c));
        if (I(self, 0x68) != -1) {
            PaintCanvas_SetColor(0xffffffff);
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x68),
                I(self, 0x78) + (I(self, 0x90) >> 1),
                (I(self, 0x7c) + (I(self, 0x88) >> 1)) - 1, 0x11, 0x44);
            PaintCanvas_SetColor(0xffffffff);
            if (C(self, 0xb0) != 0 || C(self, 0xb1) != 0)
                PaintCanvas_DrawImage2D(canvas, I(self, 0x64), I(self, 0x78), I(self, 0x7c));
        }
        PaintCanvas_SetColor(0xffffffff);
        PaintCanvas_DrawString(canvas, (String *)((char *)self + 8), (char *)self + 0xc,
            I(self, 0x78) + I(self, 0xa4), (bool)(I(self, 0x7c) + I(self, 0xa8)));
    } else {
        // generic frame / label kinds.
        int base;
        if (C(self, 0xb0) != 0)
            base = I(self, 0x3c);
        else if (C(self, 0xb1) != 0)
            base = I(self, 0x54);
        else
            base = I(self, 0x48);

        if (kind <= 0x14 && ((1u << (kind & 0xff)) & g_TB_d_frameMask) != 0) {
            // draw the 9-patch background frame + middle stretch.
            unsigned int frameLeft;
            int frameMid;
            if (C(self, 0xb0) != 0) {
                frameLeft = U(self, 0x40);
                frameMid  = I(self, 0x44);
            } else if (C(self, 0xb1) != 0) {
                frameLeft = U(self, 0x58);
                frameMid  = I(self, 0x5c);
            } else {
                frameLeft = U(self, 0x4c);
                frameMid  = I(self, 0x50);
            }
            Layout_drawBGPattern(*g_TB_d_layoutBG, frameLeft,
                I(self, 0x94) + I(self, 0x78), I(self, 0x7c), I(self, 0xa0), I(self, 0x88));
            PaintCanvas_DrawImage2D(canvas, frameMid,
                I(self, 0x78) + I(self, 0x94) + I(self, 0xa0), I(self, 0x7c));
        }
        PaintCanvas_DrawImage2D(canvas, base, I(self, 0x78), I(self, 0x7c));

        void *layoutC = *g_TB_d_layoutC;
        Layout_setDrawColor(layoutC, -1);

        // optional progress fill (when 0xbc > 0).
        float prog = F(self, 0xbc);
        if (prog > 0.0f) {
            PaintCanvas_SetColor(0xffffffff);
            Layout_setDrawColor(layoutC, -0x80);
            int span = I(self, 0x94);
            int total = I(self, 0x9c) + I(self, 0xa0) + span;
            int filled = (int)(prog * (float)total);
            int leftImg = (C(self, 0xb8) == 0) ? I(self, 0x48) : I(self, 0x3c);
            int drawW = (filled < span) ? filled : span;
            PaintCanvas_DrawRegion2D(canvas, (unsigned int)leftImg, 0, 0, drawW,
                I(self, 0x88), filled, 0, 0, 0, I(self, 0x78));

            int mid = I(self, 0x94);
            if (mid < filled) {
                int midImg = (C(self, 0xb8) == 0) ? I(self, 0x4c) : I(self, 0x40);
                int patW = I(self, 0xa0);
                if (filled - mid < patW) patW = filled - mid;
                Layout_drawBGPattern(layoutC, (unsigned int)midImg,
                    mid + I(self, 0x78), I(self, 0x7c), patW, I(self, 0x88));
                mid = I(self, 0x94);
            }
            int rstart = I(self, 0xa0) + mid;
            if (rstart < filled) {
                int rImg = (C(self, 0xb8) == 0) ? I(self, 0x50) : I(self, 0x44);
                int rW = (filled - mid) - I(self, 0xa0);
                if (I(self, 0x9c) < rW) rW = I(self, 0x9c);
                PaintCanvas_DrawRegion2D(canvas, (unsigned int)rImg, 0, 0, rW,
                    I(self, 0x88), filled, 0, 0, 0, rstart + I(self, 0x78));
            }
            Layout_setDrawColor(layoutC, -1);
        }

        // label colour: tinted when disabled (0xb3).
        unsigned int lblColor = U(self, 0xac);
        if (C(self, 0xb3) != 0)
            PaintCanvas_SetColorARGB((unsigned char)(lblColor >> 24),
                (unsigned char)(lblColor >> 16), (unsigned char)(lblColor >> 8),
                (unsigned char)lblColor);
        else
            PaintCanvas_SetColor(0xffffffff);

        if (I(self, 0x24) == -1) {
            // primary label
            PaintCanvas_DrawString(canvas, (String *)((char *)self + 8), (char *)self + 0xc,
                I(self, 0x78) + I(self, 0xa4), (bool)(I(self, 0x7c) + I(self, 0xa8)));

            // secondary / value label (offset 0x18) when 0x20 set.
            if (I(self, 0x20) != 0) {
                String *t = (String *)((char *)self + 8);
                int px = I(self, 0x78);
                int w  = I(self, 0x90);
                int tx, ty;
                if (I(self, 0x70) == 10) {
                    int tw = PaintCanvas_GetTextWidth(canvas, t);
                    int th = PaintCanvas_GetTextHeight(canvas);
                    ty = I(self, 0x7c) + I(self, 0x88) + th * -2;
                    tx = (px + w / 2) - tw / 2;
                } else {
                    int off = I(self, 0xa4);
                    int tw  = PaintCanvas_GetTextWidth(canvas, t);
                    ty = I(self, 0x7c) + I(self, 0xa8);
                    tx = ((w + px) - off) - tw;
                }
                PaintCanvas_DrawString(canvas, t, (char *)self + 0x18, tx, (bool)ty);
            }

            // shortcut / corner label (offset 0x2c) when 0x34 set.
            if (I(self, 0x34) != 0) {
                PaintCanvas_SetColor(0xffffffff);
                String *u = (String *)(*g_TB_d_unitStr);
                int tw = PaintCanvas_GetTextWidth(canvas, u);
                PaintCanvas_DrawString(canvas, u, (char *)self + 0x2c,
                    (I(self, 0x94) + I(self, 0x78)) - tw, (bool)(I(self, 0x7c) + I(self, 0xa8)));
                PaintCanvas_SetColor(0xffffffff);
            }

            // small adornment image (offset 0x38) when set.
            if (I(self, 0x38) != -1) {
                PaintCanvas_SetColor(0xffffffff);
                PaintCanvas_DrawImage2DEx(canvas, I(self, 0x38),
                    (I(self, 0x78) + I(self, 0x90) + 6) - I(self, 0x94),
                    I(self, 0x7c) + 1, 0x11, 0x14);
                PaintCanvas_SetColor(0xffffffff);
            }
        } else if (I(self, 0x70) != 0x13) {
            icon = base;
            iconY = I(self, 0xa4) + I(self, 0x78);
            tailIcon = true;
        }
    }

    if (tailIcon)
        PaintCanvas_DrawImage2D(canvas, icon, I(self, 0x78) + I(self, 0xa4) + 0, iconY);

    Layout_setDrawColor(*g_TB_d_layoutEnd, -1);
    PaintCanvas_FontSetSpacing(canvas, U(self, 0x8), savedSpacing);
    PaintCanvas_SetColor(savedColor);
}
