#include "class.h"

extern "C" void PaintCanvas_FontCreate(unsigned short canvas, unsigned *glyph, int fontGlobal);
extern "C" void PaintCanvas_FontSetSpacing(void *canvas, unsigned font, short spacing);
extern "C" void Globals_loadFont_tail(int canvas, int font, int zero);  // veneer at 0x1ac2c8

// Per-font canvas+font globals (PC-relative). canvasP/fontP each *(void**) -> object pointer.
extern void *const gLF_canvas9 __attribute__((visibility("hidden")));   // DAT_000f4980
extern void *const gLF_font9 __attribute__((visibility("hidden")));     // DAT_000f4984
extern void *const gLF_canvas10 __attribute__((visibility("hidden")));  // DAT_000f4990
extern void *const gLF_font10 __attribute__((visibility("hidden")));    // DAT_000f4994
extern void *const gLF_canvas11 __attribute__((visibility("hidden")));  // DAT_000f499c
extern void *const gLF_font11 __attribute__((visibility("hidden")));    // DAT_000f49a0
extern void *const gLF_canvas14 __attribute__((visibility("hidden")));  // DAT_000f49a4
extern void *const gLF_font14 __attribute__((visibility("hidden")));    // DAT_000f49a8
extern void *const gLF_canvasD __attribute__((visibility("hidden")));   // DAT_000f49b0 default
extern void *const gLF_font15 __attribute__((visibility("hidden")));    // DAT_000f49b4
extern void *const gLF_fontDef __attribute__((visibility("hidden")));   // DAT_000f49c0
extern void *const gLF_canvasMain __attribute__((visibility("hidden"))); // DAT_000f49d0
extern void *const gLF_fontMain __attribute__((visibility("hidden")));  // DAT_000f49d4
extern void *const gLF_fontExtra __attribute__((visibility("hidden"))); // DAT_000f49d8

// Locale-related boolean flag globals: each *(char**) -> a byte.
extern void *const gLF_flagA __attribute__((visibility("hidden")));     // DAT_000f4988 / shared
extern void *const gLF_flagB __attribute__((visibility("hidden")));     // DAT_000f498c
extern void *const gLF_flagC __attribute__((visibility("hidden")));     // DAT_000f49b8
extern void *const gLF_flagD __attribute__((visibility("hidden")));     // DAT_000f49bc
extern void *const gLF_flagE __attribute__((visibility("hidden")));     // DAT_000f49c4
extern void *const gLF_flagF __attribute__((visibility("hidden")));     // DAT_000f49c8
extern void *const gLF_flagG __attribute__((visibility("hidden")));     // DAT_000f49cc

static inline char flag(void *const g) { return **(char **)&g; }

// Globals::loadFont(int kind) — kind in r1.
extern "C" void Globals_loadFont(void *self, int kind)
{
    (void)self;
    void **canvasP;
    void **fontP;
    unsigned glyph;
    short spacing;
    unsigned char isMainFontPersian = 1;

    switch (kind) {
    case 9: {
        canvasP = *(void ***)gLF_canvas9;
        fontP = *(void ***)gLF_font9;
        PaintCanvas_FontCreate((unsigned short)(unsigned long)*canvasP, (unsigned *)0x2d74,
                               (int)*fontP);
        if (flag(gLF_flagA) != 0) {
            spacing = -6;
        } else {
            spacing = flag(gLF_flagB) != 0 ? -8 : -4;
        }
        PaintCanvas_FontSetSpacing(*canvasP, *(unsigned *)fontP, spacing);
        isMainFontPersian = 0;
        goto epilogue;
    }
    case 10:
        canvasP = *(void ***)gLF_canvas10;
        fontP = *(void ***)gLF_font10;
        glyph = 0x2d78;
        break;
    case 0xb:
        canvasP = *(void ***)gLF_canvas11;
        fontP = *(void ***)gLF_font11;
        glyph = 0x2d76;
        break;
    case 0xe:
        canvasP = *(void ***)gLF_canvas14;
        fontP = *(void ***)gLF_font14;
        glyph = 0x2d7c;
        break;
    default: {
        canvasP = *(void ***)gLF_canvasD;
        if (kind == 0xf) {
            fontP = *(void ***)gLF_font15;
            PaintCanvas_FontCreate((unsigned short)(unsigned long)*canvasP, (unsigned *)0x2d7e,
                                   (int)*fontP);
            if (flag(gLF_flagC) != 0) {
                spacing = -7;
            } else {
                spacing = flag(gLF_flagD) != 0 ? -10 : -5;
            }
        } else {
            fontP = *(void ***)gLF_fontDef;
            PaintCanvas_FontCreate((unsigned short)(unsigned long)*canvasP, (unsigned *)0x457,
                                   (int)*fontP);
            if (flag(gLF_flagE) != 0) {
                spacing = -5;
            } else if (flag(gLF_flagF) != 0) {
                spacing = -4;
            } else if (flag(gLF_flagG) != 0) {
                spacing = -4;
            } else {
                spacing = -2;
            }
        }
        PaintCanvas_FontSetSpacing(*canvasP, *(unsigned *)fontP, spacing);
        goto setMain;
    }
    }

    // cases 10/11/14: common creation tail
    PaintCanvas_FontCreate((unsigned short)(unsigned long)*canvasP, (unsigned *)(unsigned long)glyph,
                           (int)*fontP);
    if (flag(gLF_flagA) != 0) {
        spacing = -6;
    } else {
        spacing = flag(gLF_flagB) != 0 ? -8 : -4;
    }
    PaintCanvas_FontSetSpacing(*canvasP, *(unsigned *)fontP, spacing);

setMain:
    isMainFontPersian = 1;
epilogue: {
    int *mainCanvas = *(int **)gLF_canvasMain;
    unsigned *mainFont = *(unsigned **)gLF_fontMain;
    int cv = *mainCanvas;
    *(unsigned char *)(cv + 0x1c) = isMainFontPersian;
    PaintCanvas_FontCreate((unsigned short)cv, (unsigned *)0x51e, (int)mainFont);
    PaintCanvas_FontSetSpacing((void *)(long)*mainCanvas, *mainFont, 0);
    unsigned *extra = *(unsigned **)gLF_fontExtra;
    PaintCanvas_FontCreate((unsigned short)*mainCanvas, (unsigned *)0x2d7a, (int)extra);
    Globals_loadFont_tail(*mainCanvas, (int)*extra, 0);
}
}
