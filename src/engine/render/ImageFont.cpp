#include "engine/core/AbyssEngine.h"
#include "engine/file/AEFile.h"
#include "engine/core/GameText.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/FBOContainer.h"
#include "engine/math/Quaternion.h"
#include <GLES2/gl2.h>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace AbyssEngine {
    void ImageFontDrawString(ImageFont *font, const unsigned short *str, unsigned int len, int x,
                             int y, PaintCanvas *canvas, Engine *engine, bool flag);

    void ImageFontDrawString(ImageFont *font, const unsigned short *str, int x, int y,
                             PaintCanvas *canvas, Engine *engine, bool flag) {
        if (font != 0 && str != 0) {
            unsigned short len = 0;
            while (str[len] != 0)
                len++;
            ImageFontDrawString(font, str, len, x, y, canvas, engine, flag);
        }
    }
}

namespace AbyssEngine {
    int ImageFontGetWidth(ImageFont *font, const unsigned short *text, unsigned int len);

    int ImageFontGetHeight(ImageFont * font);
    short ImageFontGetYOffset(ImageFont * font);
    int MeshDraw(Engine * engine, Mesh * mesh);

    void ImageFontDrawString(ImageFont *font, const unsigned short *text, unsigned int len, int x, int y,
                             PaintCanvas *canvas, Engine *engine, bool rtl) {
        bool valid = text != 0 && font != 0;

        if (engine->statsBucketFlag == 0) {
            // Immediate mode: one draw call per glyph mesh, translated by the world-view matrix.
            if (!valid)
                return;
            int top = font->yOffset;
            // `top` is the font's y offset, so the right-hand side is zero; this is the
            // original horizontal clip test.
            if (ImageFontGetWidth(font, text, len) + x < ImageFontGetYOffset(font) - top)
                return;
            int height = ImageFontGetHeight(font);
            top += y;
            if ((int) engine->GetDisplayWidth() >= x && height + top >= 0 &&
                top <= (int) engine->GetDisplayHeight()) {
                AEMath::Matrix placement;
                placement.m[14] = 1.0f;
                placement.m[0] = 1.0f;
                placement.m[5] = 1.0f;
                float glyphY = (float) (top - 2);
                placement.m[3] = (float) x;
                placement.m[7] = glyphY;
                bool forward = (canvas->field_0x1c | rtl) != 0;
                int index = forward ? 0 : len - 1;
                int step = forward ? 1 : -1;

                for (unsigned short i = 0; i < len; i++) {
                    for (unsigned short slot = 0; slot < font->glyphCount; slot++) {
                        if (font->codes[slot] != text[index])
                            continue;
                        placement.m[7] = glyphY;
                        placement.m[3] = (float) x;
                        int advance = (int) ((float *) font->glyphMeshes[slot]->positions)[3];
                        if (x + advance >= 0 && x <= (int) engine->GetDisplayWidth()) {
                            canvas->SetWorldViewMatrix(placement);
                            MeshDraw(engine, font->glyphMeshes[slot]);
                        }
                        int width = font->spacing + advance;
                        int stepX = width;
                        if (text[index] == ' ')
                            stepX = width - 2;
                        if (advance != 11)
                            stepX = width;
                        x += stepX;
                        break;
                    }
                    index += step;
                }
            }
            return;
        }

        // Batched mode: glyph quads are appended to the canvas' 2D quad mesh and flushed per 100.
        if (!valid)
            return;
        int top = font->yOffset;
        // `top` is the font's y offset, so the right-hand side is zero; this is the
        // original horizontal clip test.
        if (ImageFontGetWidth(font, text, len) + x < ImageFontGetYOffset(font) - top)
            return;
        int height = ImageFontGetHeight(font);
        top += y;
        if ((int) engine->GetDisplayWidth() >= x && height + top >= 0 &&
            top <= (int) engine->GetDisplayHeight()) {
            bool forward = (canvas->field_0x1c | rtl) != 0;
            int index = forward ? 0 : len - 1;
            int step = forward ? 1 : -1;
            if (Engine::enableReverseFlag != 0 && GameText::getLanguage() == 9 &&
                GameText::isNonArabicString(text, len) != 0) {
                index = 0;
                step = 1;
            }

            float glyphY = (float) (top - 2);
            for (unsigned short i = 0; i < len; i++) {
                for (unsigned short slot = 0; slot < font->glyphCount; slot++) {
                    if (font->codes[slot] != text[index])
                        continue;
                    int advance = (int) ((float *) font->glyphMeshes[slot]->positions)[3];
                    if (x + advance >= 0 && x <= (int) engine->GetDisplayWidth()) {
                        Mesh *batch = (Mesh *) canvas->quad2dMesh;
                        int n = canvas->field_0xc;
                        Mesh *glyph = font->glyphMeshes[slot];
                        float glyphX = (float) x;
                        float *src = (float *) glyph->positions;
                        float *dst = (float *) batch->positions;
                        dst[n * 12 + 0] = src[0] + glyphX;
                        dst[n * 12 + 1] = src[1] + glyphY;
                        dst[n * 12 + 3] = src[3] + glyphX;
                        dst[n * 12 + 4] = src[4] + glyphY;
                        dst[n * 12 + 6] = src[6] + glyphX;
                        dst[n * 12 + 7] = src[7] + glyphY;
                        dst[n * 12 + 9] = src[9] + glyphX;
                        dst[n * 12 + 10] = src[10] + glyphY;

                        unsigned int *uvSrc = (unsigned int *) glyph->texCoords;
                        unsigned int *uvDst = (unsigned int *) batch->texCoords;
                        uvDst[n * 8 + 0] = uvSrc[0];
                        uvDst[n * 8 + 1] = uvSrc[1];
                        uvDst[n * 8 + 2] = uvSrc[2];
                        uvDst[n * 8 + 3] = uvSrc[3];
                        uvDst[n * 8 + 4] = uvSrc[4];
                        uvDst[n * 8 + 5] = uvSrc[5];
                        uvDst[n * 8 + 6] = uvSrc[6];
                        uvDst[n * 8 + 7] = uvSrc[7];

                        float *colors = (float *) batch->colors;
                        colors[n * 16 + 0] = engine->glColor[0];
                        colors[n * 16 + 1] = engine->glColor[1];
                        colors[n * 16 + 2] = engine->glColor[2];
                        colors[n * 16 + 3] = engine->glColor[3];
                        colors[n * 16 + 4] = engine->glColor[0];
                        colors[n * 16 + 5] = engine->glColor[1];
                        colors[n * 16 + 6] = engine->glColor[2];
                        colors[n * 16 + 7] = engine->glColor[3];
                        colors[n * 16 + 8] = engine->glColor[0];
                        colors[n * 16 + 9] = engine->glColor[1];
                        colors[n * 16 + 10] = engine->glColor[2];
                        colors[n * 16 + 11] = engine->glColor[3];
                        colors[n * 16 + 12] = engine->glColor[0];
                        colors[n * 16 + 13] = engine->glColor[1];
                        colors[n * 16 + 14] = engine->glColor[2];
                        colors[n * 16 + 15] = engine->glColor[3];
                        canvas->field_0xc = n + 1;

                        if (n >= 99) {
                            batch->indexCount = (short) ((n + 1) * 6);
                            AEMath::Matrix identity;
                            canvas->SetWorldViewMatrix(identity);
                            MeshDraw(engine, (Mesh *) canvas->quad2dMesh);
                            canvas->field_0xc = 0;
                        }
                    }
                    int width = font->spacing + advance;
                    int stepX = width;
                    if (text[index] == ' ')
                        stepX = width - 2;
                    if (advance != 11)
                        stepX = width;
                    x += stepX;
                    break;
                }
                index += step;
            }

            int pending = canvas->field_0xc;
            if (pending > 0) {
                ((Mesh *) canvas->quad2dMesh)->indexCount = (short) (pending * 6);
                AEMath::Matrix identity;
                canvas->SetWorldViewMatrix(identity);
                MeshDraw(engine, (Mesh *) canvas->quad2dMesh);
                canvas->field_0xc = 0;
            }
        }
    }
}

namespace AbyssEngine {
    void ImageFontCheckString(ImageFont * /*font*/, const unsigned short * /*str*/, unsigned int count) {
        for (unsigned short i = 0; i < count; ++i) {
        }
    }
}

namespace AbyssEngine {
    int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int count) {
        if (font == 0 || str == 0)
            return 0;

        int total = 0;
        for (unsigned short i = 0; i < count; ++i) {
            for (unsigned short g = 0; g < font->glyphCount; ++g) {
                if (font->codes[g] == str[i]) {
                    int w = (int) ((float *) font->glyphMeshes[g]->positions)[3];
                    int adv = font->spacing + w;
                    // A narrow (11 px) space glyph is drawn two pixels tighter.
                    if (w == 11 && font->codes[g] == ' ')
                        adv -= 2;
                    total += adv;
                    break;
                }
            }
        }
        return total;
    }
}

namespace AbyssEngine {
    int ImageFontGetHeight(ImageFont *font) {
        if (font != 0) {
            Mesh **p = font->glyphMeshes;
            Mesh *q = p[0];
            float *r = (float *) q->positions;
            int v = (int) r[7];
            if (v == 0x18)
                v -= 5; // ASM: original subtracts 5 (subeq) rather than moving 19 (moveq); v==0x18 here
            return v;
        }
        return 0;
    }
}

namespace AbyssEngine {
    short ImageFontGetYOffset(ImageFont *font) {
        if (font == 0)
            return 0;
        return font->yOffset;
    }
}

namespace AbyssEngine {
    int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int start,
                          unsigned int len) {
        if (font == 0 || str == 0)
            return 0;

        int total = 0;
        for (unsigned short i = start; i < start + len; ++i) {
            for (unsigned short g = 0; g < font->glyphCount; ++g) {
                if (font->codes[g] == str[i]) {
                    int w = (int) ((float *) font->glyphMeshes[g]->positions)[3];
                    int adv = font->spacing + w;
                    if (w == 11 && font->codes[g] == ' ')
                        adv -= 2;
                    total += adv;
                    break;
                }
            }
        }
        return total;
    }
}

namespace AbyssEngine {
    void ImageFontSetSpacing(ImageFont *font, short spacing) {
        if (font != 0)
            font->spacing = spacing;
    }
}

namespace AbyssEngine {
    short ImageFontGetSpacing(ImageFont *font) {
        short v = (font == 0) ? (short) 0 : font->spacing;
        return (int) v;
    }
}

namespace AbyssEngine {
    void ImageFontSetYOffset(ImageFont *font, short yOffset) {
        if (font != 0)
            font->yOffset = yOffset;
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void ImageFontRelease(Engine *engine, ImageFont **slot) {
        if (*slot == 0)
            return;

        if ((*slot)->codes != 0)
            delete[] (*slot)->codes;
        (*slot)->codes = 0;

        for (unsigned short i = 0; i < (*slot)->glyphCount; i++)
            MeshRelease(engine, &(*slot)->glyphMeshes[i]);

        if ((*slot)->glyphMeshes != 0)
            delete[] (*slot)->glyphMeshes;
        (*slot)->glyphMeshes = 0;

        if (*slot != 0)
            delete *slot;
        *slot = 0;
    }
}

