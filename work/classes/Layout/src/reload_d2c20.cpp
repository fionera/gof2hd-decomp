#include "class.h"

struct PaintCanvas;
struct TouchButton;

extern "C" void __aeabi_memset4(void *dst, int n, int v);                // 0x74d94
extern "C" void *operator_new(unsigned sz);
extern "C" void *GameText_getText(int id);                               // 0x72f70
extern "C" int PaintCanvas_GetTextHeight(unsigned pc);
extern "C" void PaintCanvas_Image2DCreate(PaintCanvas *pc, int id, unsigned *out);
extern "C" void Layout_resetWindowDimensions(Layout *self);              // 0xd3048
extern "C" void TouchButton_ctorStr(TouchButton *b, void *str, int a, int x,
                                    int y, int anchor);                  // 0xd... TouchButton(String,...)
extern "C" void TouchButton_ctorImg(TouchButton *b, unsigned img, int a, int x,
                                    int y, int anchor);
extern "C" void TouchButton_ctorImg2(TouchButton *b, unsigned img, int a, int x,
                                     int y, int z, int anchor, int extra);
extern "C" int TouchButton_getWidth(TouchButton *b);
// Image2DCreate-into-field loader (fn ptr @0xe2c64): (canvas, imgId, &field).
extern "C" void Layout_loadImage(unsigned canvas, int id, void *field);

__attribute__((visibility("hidden"))) extern int *g_rlGuard;    // @0xe2c38 (stack guard [0])
__attribute__((visibility("hidden"))) extern unsigned *g_rlCanvas; // @0xe2c62 (*puVar8)
__attribute__((visibility("hidden"))) extern char *g_rlHdFlag;  // @0xe2da8 (HD variant)
__attribute__((visibility("hidden"))) extern int *g_rlBackText; // @0xe2ec0 (*piVar10)
__attribute__((visibility("hidden"))) extern int *g_rlScreenH;  // @0xe2ed6 (*piVar9)
__attribute__((visibility("hidden"))) extern int *g_rlMenuY;    // @0xe2f8a

// Layout::reload(): (re)load all layout images/strings and rebuild the buttons.
extern "C" void Layout_reload(Layout *self) {
    int *guard = g_rlGuard;
    int g0 = *guard;

    F<int>(self, 0x324) = -1;
    F<int>(self, 0x328) = -1;
    F<int>(self, 0x32c) = -1;
    F<int>(self, 0x3a8) = -1;
    __aeabi_memset4((char *)self + 0x334, 0x70, 0xff);

    unsigned canvas = *g_rlCanvas;
    Layout_loadImage(canvas, 0x503, (char *)self + 0x398);
    Layout_loadImage(canvas, 0x47e, (char *)self + 0x324);
    Layout_loadImage(canvas, 0x4ff, (char *)self + 0x328);
    Layout_loadImage(canvas, 0x500, (char *)self + 0x330);
    Layout_loadImage(canvas, 0x474, (char *)self + 0x32c);
    Layout_loadImage(canvas, 0x502, (char *)self + 0x334);
    Layout_loadImage(canvas, 0x506, (char *)self + 0x340);
    Layout_loadImage(canvas, 0x501, (char *)self + 0x338);
    Layout_loadImage(canvas, 0x507, (char *)self + 0x344);
    Layout_loadImage(canvas, 0x4fe, (char *)self + 0x33c);
    Layout_loadImage(canvas, 0x482, (char *)self + 0x348);
    Layout_loadImage(canvas, 0x481, (char *)self + 0x34c);
    Layout_loadImage(canvas, 0x486, (char *)self + 0x378);
    Layout_loadImage(canvas, 0x487, (char *)self + 0x374);
    Layout_loadImage(canvas, 0x48b, (char *)self + 0x37c);
    Layout_loadImage(canvas, 0x52d, (char *)self + 0x3a4);

    if (*g_rlHdFlag == 0) {
        Layout_loadImage(canvas, 0x480, (char *)self + 0x350);
        Layout_loadImage(canvas, 0x47f, (char *)self + 0x354);
        Layout_loadImage(canvas, 0x479, (char *)self + 0x358);
        Layout_loadImage(canvas, 0x478, (char *)self + 0x35c);
        Layout_loadImage(canvas, 0x489, (char *)self + 0x36c);
        Layout_loadImage(*g_rlCanvas, 0x488, (char *)self + 0x370);
    } else {
        Layout_loadImage(canvas, 0x6bb, (char *)self + 0x350);
        Layout_loadImage(canvas, 0x6ba, (char *)self + 0x354);
        Layout_loadImage(canvas, 0x6b9, (char *)self + 0x358);
        Layout_loadImage(canvas, 0x6b8, (char *)self + 0x35c);
        Layout_loadImage(canvas, 0x6b7, (char *)self + 0x36c);
        Layout_loadImage(*g_rlCanvas, 0x6bc, (char *)self + 0x370);
    }

    Layout_loadImage(canvas, 0x530, (char *)self + 0x360);
    Layout_loadImage(canvas, 0x531, (char *)self + 0x364);
    Layout_loadImage(canvas, 0x52f, (char *)self + 0x368);
    Layout_loadImage(canvas, 0x47c, (char *)self + 900);
    Layout_loadImage(canvas, 0x47d, (char *)self + 0x380);
    Layout_loadImage(canvas, 0x47b, (char *)self + 0x388);
    Layout_loadImage(canvas, 0x47a, (char *)self + 0x38c);
    Layout_loadImage(canvas, 0x484, (char *)self + 0x390);
    Layout_loadImage(canvas, 0x483, (char *)self + 0x394);
    Layout_loadImage(canvas, 0x50c, (char *)self + 0x3a0);
    Layout_loadImage(canvas, 0x50d, (char *)self + 0x39c);

    // Back button (string-labelled).
    TouchButton *bBack = (TouchButton *)operator_new(200);
    void *txt = GameText_getText(*g_rlBackText);
    int sh = *g_rlScreenH;
    TouchButton_ctorStr(bBack, txt, 2, F<int>(self, 0x28), sh - 3, '!');
    F<TouchButton *>(self, 0x3b4) = bBack;
    F<int>(self, 0x2f0) = TouchButton_getWidth(bBack);

    // Secondary button (image if available, else string fallback).
    unsigned img535 = 0xffffffff;
    PaintCanvas_Image2DCreate((PaintCanvas *)*g_rlCanvas, 0x535, &img535);
    TouchButton *b2 = (TouchButton *)operator_new(200);
    if (img535 == 0xffffffff) {
        void *t = GameText_getText(*g_rlBackText);
        TouchButton_ctorStr(b2, t, 2, F<int>(self, 0x28),
                            *g_rlScreenH - F<int>(self, 0x3fc), '!');
    } else {
        TouchButton_ctorImg(b2, img535, 2, F<int>(self, 0x28),
                            *g_rlScreenH - F<int>(self, 0x3fc), '!');
    }
    F<TouchButton *>(self, 0x3b8) = b2;

    // Help button (image).
    unsigned img471 = 0xffffffff;
    PaintCanvas_Image2DCreate((PaintCanvas *)*g_rlCanvas, 0x471, &img471);
    TouchButton *bHelp = (TouchButton *)operator_new(200);
    TouchButton_ctorImg2(bHelp, img471, 1, *g_rlMenuY, 0, F<int>(self, 0x3c), 0x12, 0x04);
    F<TouchButton *>(self, 0x3bc) = bHelp;

    int th = PaintCanvas_GetTextHeight(*g_rlCanvas);
    F<uint8_t>(self, 0x0) = 0;
    F<int>(self, 0x3c4) = 0;
    F<int>(self, 0x3c8) = 0;
    F<int>(self, 0x3ac) = th / 2 - 1;
    F<int>(self, 0x3b0) = -1;
    Layout_resetWindowDimensions(self);
    F<int>(self, 0x3d0) = 0;
    F<uint8_t>(self, 0x3cc) = 0;
    F<int>(self, 0x3d8) = 0;
    F<uint16_t>(self, 0x400) = 0;
    F<uint8_t>(self, 0x3dc) = 0;
    F<int>(self, 0x404) = 0;
    F<int>(self, 0x408) = 0;
    F<uint16_t>(self, 0x2ec) = 0;
    F<int>(self, 0x40d) = 0;
    F<int>(self, 0x409) = 0;

    if (*guard != g0)
        __stack_chk_fail();
}
