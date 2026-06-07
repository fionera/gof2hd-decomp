#include "class.h"

struct MiningGame {
    void render2D();
};

struct String {
    uint32_t a;
    uint32_t b;
    uint32_t c;
};

__attribute__((visibility("hidden"))) extern void **g_MiningGame_canvasRender;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_layerTableRender;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layoutRender;
__attribute__((visibility("hidden"))) extern void (*g_MiningGame_drawLayer)(void *canvas, int image, int x, int y,
                                                                            int w, int h, int anchor, int tile,
                                                                            int frame);
__attribute__((visibility("hidden"))) extern void **g_MiningGame_statusRender;
__attribute__((visibility("hidden"))) extern char g_MiningGame_oreSuffix[];
__attribute__((visibility("hidden"))) extern void **g_MiningGame_fontString;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_gameText;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_screenWRender;

extern "C" void MiningGame_PaintCanvas_SetColor(void *canvas, int color);
extern "C" void MiningGame_PaintCanvas_SetColorRGBA(void *canvas, int a, int r, int g, int b);
extern "C" void MiningGame_PaintCanvas_DrawImage2D_anchor(void *canvas, int image, int x, int y, int anchor);
extern "C" void MiningGame_PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y);
extern "C" void MiningGame_PaintCanvas_DrawRegion2D(void *canvas, int image, int sx, int sy, int w, int h,
                                                     int dx, int dy, int unused0, int unused1, int x);
extern "C" float MiningGame_Layout_getPulseValue(void *layout, float value);
extern "C" void MiningGame_MarqueeImage_draw(void *self);
extern "C" void MiningGame_MarqueeImage_drawAt(void *self, int x, int y);
extern "C" void MiningGame_Sprite_setRefPixelPosition(void *sprite, int x, int y);
extern "C" void MiningGame_Sprite_draw(void *sprite, float sx, float sy);
extern "C" void MiningGame_String_ctor_int(String *self, int value);
extern "C" void MiningGame_String_ctor_char(String *self, const char *text, bool copy);
extern "C" void MiningGame_String_ctor_copy(String *self, const String *other, bool copy);
extern "C" void MiningGame_String_plus(String *out, const String *lhs, const String *rhs);
extern "C" void MiningGame_String_dtor(String *self);
extern "C" void *MiningGame_Status_getShip_render(void *status);
extern "C" int MiningGame_Ship_getFreeSpace(void *ship);
extern "C" int MiningGame_PaintCanvas_GetTextWidth(void *canvas, String *font, String *text);
extern "C" void MiningGame_PaintCanvas_DrawString(void *canvas, String *font, String *text, int x, int y, bool shadow);
extern "C" int MiningGame_Status_getCurrentCampaignMission_render(void *status);
extern "C" String *MiningGame_GameText_getText(void *gameText, int id);

void MiningGame::render2D()
{
    char amountTextBytes[12];
    char suffixTextBytes[12];
    char oreTextBytes[12];
    String *amountText = (String *)amountTextBytes;
    String *suffixText = (String *)suffixTextBytes;
    String *oreText = (String *)oreTextBytes;

    void **canvasHolder = g_MiningGame_canvasRender;
    void *canvas = *canvasHolder;
    MiningGame_PaintCanvas_SetColor(canvas, -1);

    int *layerTable = g_MiningGame_layerTableRender;
    void **layoutHolder = g_MiningGame_layoutRender;
    void *layout = *layoutHolder;
    void (*drawLayer)(void *, int, int, int, int, int, int, int, int) = g_MiningGame_drawLayer;

    for (int layerIndex = I(this, 0x78); layerIndex < I(this, 0x7c); layerIndex++) {
        int raw = layerTable[(layerIndex - I(this, 0x7c) * 7) + 0x31];
        int radius = (int)(F(layout, 0xe8) * (float)raw);
        void *imageSlot;
        if ((layerIndex & 1) == 0) {
            imageSlot = B(this, 0xac);
            if (I(layout, 0xec) < radius) {
                imageSlot = B(this, 0xb4);
                if (radius < I(layout, 0xf4)) {
                    imageSlot = B(this, 0xb0);
                }
            }
        } else {
            imageSlot = B(this, 0xb8);
            if (I(layout, 0xec) < radius) {
                imageSlot = B(this, 0xc0);
                if (radius < I(layout, 0xf4)) {
                    imageSlot = B(this, 0xbc);
                }
            }
        }
        int image = I(imageSlot, 0);
        int half = radius / 2;
        drawLayer(canvas, image, I(this, 0x58), I(this, 0x5c), half, half, 0x11, 0x22, 0);
        drawLayer(canvas, image, I(this, 0x58), I(this, 0x5c), half, half, 0x11, 0x21, 1);
        drawLayer(canvas, image, I(this, 0x58), I(this, 0x5c), half, half, 0x11, 0x12, 2);
        drawLayer(canvas, image, I(this, 0x58), I(this, 0x5c), half, half, 0x11, 0x11, 3);
    }

    if (UC(this, 0x80) != 0) {
        MiningGame_PaintCanvas_DrawImage2D_anchor(canvas, I(this, 0x60), I(this, 0x58), I(this, 0x5c), 0x4411);
    }

    MiningGame_PaintCanvas_DrawImage2D_anchor(canvas, I(this, 0x98), (int)F(this, 0x10), (int)F(this, 0x14),
                                             0x4411);
    MiningGame_Sprite_setRefPixelPosition(P(this, 0x94), (int)F(this, 0x10), (int)F(this, 0x14));
    MiningGame_Sprite_draw(P(this, 0x94), 1.0f, 1.0f);

    MiningGame_PaintCanvas_DrawImage2D(canvas, I(this, 0xa4), I(this, 0x50) - I(layout, 0xfc),
                                       I(this, 0x54) - I(layout, 0xfc));

    int lossTimer = I(this, 0x20);
    if (lossTimer > 0x341) {
        float red = MiningGame_Layout_getPulseValue(layout, 10.0f) * 255.0f;
        float green = MiningGame_Layout_getPulseValue(layout, 10.0f) * 255.0f;
        MiningGame_PaintCanvas_SetColorRGBA(canvas, 0xff, (int)red, (int)green, 0xff);
    }

    int width = (int)(((2500.0f - (float)lossTimer) / 2500.0f) * (float)I(this, 0x48));
    MiningGame_PaintCanvas_DrawRegion2D(canvas, I(this, 0xa8), 0, 0, width, I(this, 0x4c), width, 0, 0, 0,
                                        I(this, 0x50));
    MiningGame_PaintCanvas_SetColor(canvas, -1);
    MiningGame_PaintCanvas_DrawImage2D_anchor(canvas, I(this, 0xc4), I(this, 0x58), I(this, 0x54) - 3, 0x2411);

    MiningGame_MarqueeImage_draw(P(this, 0x8c));
    MiningGame_MarqueeImage_draw(P(this, 0x90));
    MiningGame_MarqueeImage_drawAt(P(this, 0x88), (int)(F(this, 0x10) + (float)I(this, 0x40)),
                                   (int)(F(this, 0x14) - (float)I(this, 0x34)));

    MiningGame_PaintCanvas_DrawImage2D(canvas, I(this, 0x9c),
                                       (int)((F(this, 0x10) + (float)I(this, 0x40)) - (float)I(layout, 0xfc)),
                                       (int)(F(this, 0x14) - (float)I(layout, 0x100)));

    MiningGame_String_ctor_int(amountText, (int)F(this, 0x24));
    MiningGame_String_ctor_char(suffixText, g_MiningGame_oreSuffix, false);
    MiningGame_String_plus(oreText, amountText, suffixText);
    MiningGame_String_dtor(suffixText);
    MiningGame_String_dtor(amountText);

    void *ship = MiningGame_Status_getShip_render(*g_MiningGame_statusRender);
    int freeSpace = MiningGame_Ship_getFreeSpace(ship);
    int alpha = (int)(F(this, 0x64) * 255.0f);
    if (freeSpace < (int)F(this, 0x24)) {
        MiningGame_PaintCanvas_SetColorRGBA(canvas, 0xff, 0x2a, 0, alpha);
    } else {
        MiningGame_PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, alpha);
    }

    String *font = (String *)*g_MiningGame_fontString;
    int textWidth = MiningGame_PaintCanvas_GetTextWidth(canvas, font, oreText);
    int textX = (int)(((F(this, 0x10) + (float)I(this, 0x40) + (float)I(this, 0x44)) -
                      (float)I(layout, 0xfc)) -
                     (float)(textWidth / 2));
    int textY = (int)(F(this, 0x14) + (float)I(layout, 0x104));
    MiningGame_PaintCanvas_DrawString(canvas, font, oreText, textX, textY, false);
    MiningGame_PaintCanvas_SetColor(canvas, -1);

    if (MiningGame_Status_getCurrentCampaignMission_render(*g_MiningGame_statusRender) < 5) {
        int promptAlpha = (int)(((float)I(this, 0xcc) / 2500.0f) * 255.0f);
        if (promptAlpha > 255) {
            promptAlpha = 255 - promptAlpha;
        }
        MiningGame_PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, (uint8_t)promptAlpha);
        String *prompt = MiningGame_GameText_getText(*g_MiningGame_gameText, 0x268);
        MiningGame_String_ctor_copy(amountText, prompt, false);
        int promptWidth = MiningGame_PaintCanvas_GetTextWidth(canvas, font, amountText);
        MiningGame_PaintCanvas_DrawString(canvas, font, amountText, *g_MiningGame_screenWRender / 2 - promptWidth / 2,
                                          I(layout, 0x70) + I(this, 0x54), false);
        MiningGame_PaintCanvas_SetColor(canvas, -1);
        MiningGame_String_dtor(amountText);
    }

    MiningGame_String_dtor(oreText);
}
