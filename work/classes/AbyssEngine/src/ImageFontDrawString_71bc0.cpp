#include "class.h"

// AbyssEngine::ImageFontDrawString(ImageFont*, unsigned short const* text, unsigned int len,
//                                  int x, int y, PaintCanvas*, Engine*, bool rtl)
// Draw a UTF-16 string with a bitmap font. Two rendering paths are selected by the engine
// shader-mode flag at engine+0xfc:
//   - immediate path: per-glyph world matrix + MeshDraw
//   - batched path: glyph quads are accumulated into a shared sprite buffer (PaintCanvas+8)
//     and flushed (MeshDraw) when full or at the end.
// Characters are looked up in the font codepoint table (font+0x04); advance widths come from
// each glyph mesh. RTL ordering (or the canvas batch flag) reverses the scan direction.
namespace AbyssEngine {

extern "C" char *g_GameText_arabicEnabledFlag; // **(DAT + 0x81c5c)

extern "C" {
int  AE_Engine_GetDisplayWidth();
int  AE_Engine_GetDisplayHeight();
int  AE_GameText_getLanguage();
int  AE_GameText_isNonArabicString(const unsigned short *text, unsigned int len);
void AE_PaintCanvas_SetWorldViewMatrix(void *canvas);
float AE_VectorSignedToFloat(int v, unsigned char mode);
}

int  ImageFontGetWidth(ImageFont *font, unsigned short *text, unsigned int len);
int  ImageFontGetHeight(ImageFont *font);
int  MeshDraw(Engine *engine, Mesh *mesh);

int ImageFontDrawString(ImageFont *font, unsigned short *text, unsigned int len, int x, int y,
                        PaintCanvas *canvas, Engine *engine, bool rtl)
{
    if (text == 0 || font == 0)
        return 0;

    char *f = (char *)font;
    char *pc = (char *)canvas;
    char *en = (char *)engine;
    unsigned char mode = 0;

    int w = ImageFontGetWidth(font, text, len);
    if (w + x < 0)
        return 0;

    int top = (int)(short)s16(f, 0x12);
    int h = ImageFontGetHeight(font);
    int dispW = AE_Engine_GetDisplayWidth();
    int bottom = dispW;
    if (x <= dispW) {
        top = top + y;
        bottom = h + top;
    }
    int side = (x <= dispW) ? bottom : (dispW - x);
    // Off-screen clip on X.
    if ((side < 0) != (dispW < x))
        return 0;
    if (top > AE_Engine_GetDisplayHeight())
        return 0;

    // Scan direction: forward by default; reversed for RTL or when batching is requested.
    int step = -1;
    int idx = (int)len - 1;
    bool batched = (u8(pc, 0x1c) != 0) || rtl;
    if (batched) {
        step = 1;
        idx = 0;
    }

    bool shaderMode = (u8(en, 0xfc) != 0);
    if (shaderMode) {
        // Arabic special-case: force forward scan.
        if (*g_GameText_arabicEnabledFlag != 0 && AE_GameText_getLanguage() == 9 &&
            AE_GameText_isNonArabicString(text, len) != 0) {
            idx = 0;
            step = 1;
        }
    }

    float baseY = AE_VectorSignedToFloat(top - 2, mode);

    for (unsigned int i = 0; i < len; ++i) {
        // Find glyph slot for codepoint text[idx].
        unsigned int slot = 0;
        unsigned short glyphCount = u16(f, 0x0);
        bool found = false;
        while (slot < glyphCount) {
            if (*(unsigned short *)(*(int *)(f + 4) + slot * 2) == text[idx]) {
                found = true;
                break;
            }
            ++slot;
        }

        if (found) {
            int glyphMesh = *(int *)(*(int *)(f + 0xc) + slot * 4);
            int advance = (int)*(float *)(*(int *)(glyphMesh + 4) + 0xc);

            if (x + advance >= 0 && x <= AE_Engine_GetDisplayWidth()) {
                if (!shaderMode) {
                    AE_PaintCanvas_SetWorldViewMatrix(canvas);
                    MeshDraw(engine, *(Mesh **)(*(int *)(f + 0xc) + slot * 4));
                } else {
                    // Batched path: append this glyph's quad into the sprite buffer at
                    // canvas+8 (vertex/color/matrix arrays), flush when the slot count
                    // reaches the buffer capacity.
                    int spr = *(int *)(pc + 8);
                    int n = *(int *)(pc + 0xc);
                    float fx = AE_VectorSignedToFloat(x, mode);
                    float *vsrc = *(float **)(glyphMesh + 4);
                    float *vdst = (float *)(*(int *)(spr + 4) + n * 0x30);
                    vdst[0] = vsrc[0] + fx;     vdst[1] = vsrc[1] + baseY;
                    vdst[3] = vsrc[3] + fx;     vdst[4] = vsrc[4] + baseY;
                    vdst[6] = vsrc[6] + fx;     vdst[7] = vsrc[7] + baseY;
                    vdst[9] = vsrc[9] + fx;     vdst[10] = vsrc[10] + baseY;

                    unsigned int *csrc = *(unsigned int **)(glyphMesh + 8);
                    unsigned int *cdst = (unsigned int *)(*(int *)(spr + 8) + (n << 5));
                    for (int k = 0; k < 8; ++k) cdst[k] = csrc[k];

                    unsigned int *mdst = (unsigned int *)(*(int *)(spr + 0xc) + (n << 6));
                    for (int r = 0; r < 4; ++r) {
                        mdst[r * 4 + 0] = u32(en, 0xd0);
                        mdst[r * 4 + 1] = u32(en, 0xd4);
                        mdst[r * 4 + 2] = u32(en, 0xd8);
                        mdst[r * 4 + 3] = u32(en, 0xdc);
                    }
                    *(int *)(pc + 0xc) = n + 1;

                    if (n > 0x62) {
                        s16(*(void **)(pc + 8), 0x28) = (short)((n + 1) * 6);
                        AE_PaintCanvas_SetWorldViewMatrix(canvas);
                        MeshDraw(engine, *(Mesh **)(pc + 8));
                        *(int *)(pc + 0xc) = 0;
                    }
                }
            }

            int adv = (int)(short)s16(f, 0x10) + advance;
            int eff = adv;
            if (text[idx] == 0x20)
                eff = adv - 2;
            if (advance != 0xb)
                eff = adv;
            x += eff;
        }

        idx += step;
    }

    // Flush any remaining batched glyphs.
    if (shaderMode) {
        int n = *(int *)(pc + 0xc);
        if (n > 0) {
            s16(*(void **)(pc + 8), 0x28) = (short)((n + (n << 1)) * 2);
            AE_PaintCanvas_SetWorldViewMatrix(canvas);
            MeshDraw(engine, *(Mesh **)(pc + 8));
            *(int *)(pc + 0xc) = 0;
        }
    }

    return 1;
}

} // namespace AbyssEngine
