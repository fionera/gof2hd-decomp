#include "MiningGame.h"


extern "C" float MiningGame_sqrt(void *globals, float value);
extern "C" void *MiningGame_Sprite_dtor(void *sprite);
extern "C" void MiningGame_operator_delete(void *ptr);
extern "C" void MiningGame_MarqueeImage_update(void *self, int delta);
extern "C" void MiningGame_MarqueeImage_setSpeed_update(void *self, float speed);
extern "C" void MiningGame_FModSound_setParamValue(void *sound, int index, int param, float value);
extern "C" void MiningGame_FModSound_play(void *sound, int id, void *a, void *b, void *c);
extern "C" void MiningGame_FModSound_stop(void *sound, int id);
extern "C" void MiningGame_Sprite_nextFrame(void *sprite);
extern "C" int MiningGame_Achievements_hasMedal(void *achievements, int medal, int value);
extern "C" int MiningGame_Achievements_getValue(void *achievements, int medal, int value);
extern "C" int MiningGame_idivmod(int value, int divisor);
extern "C" void MiningGame_Hud_hudEventMedal(void *hud, int medal, int value);
extern "C" void *MiningGame_Status_getShip(void *status);
extern "C" void *MiningGame_Ship_getFirstEquipmentOfSort(void *ship, int sort);
extern "C" int MiningGame_Item_getAttribute(void *item, int attribute);
extern "C" void MiningGame_PaintCanvas_Image2DCreate(void *canvas, int imageId, int *outId);
extern "C" int MiningGame_PaintCanvas_GetImage2DHeight(void *canvas, int imageId);
extern "C" int MiningGame_PaintCanvas_GetImage2DWidth(void *canvas, int imageId);
extern "C" void *MiningGame_operator_new(uint32_t size);
extern "C" void MiningGame_Sprite_ctor(void *sprite, int imageId, int width, int height);
extern "C" void MiningGame_Sprite_defineReferencePixel(void *sprite, int x, int y);
extern "C" void MiningGame_MarqueeImage_ctor(void *self, int imageId, int width, int x, int y, float value);
extern "C" void MiningGame_MarqueeImage_setSpeed(void *self, float speed);
extern "C" int MiningGame_Status_getCurrentCampaignMission(void *status);
extern "C" void MiningGame_PaintCanvas_SetColor(void *canvas, int color);
extern "C" void MiningGame_PaintCanvas_SetColorRGBA(void *canvas, int a, int r, int g, int b);
extern "C" void MiningGame_PaintCanvas_DrawImage2D_anchor(void *canvas, int image, int x, int y, int anchor);
extern "C" void MiningGame_PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y);
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

// ---- getOreAmount_11eb9e.cpp ----
struct MiningGame {
    int getOreAmount();
};

int MiningGame::getOreAmount()
{
    return (int)F(this, 0x24);
}

// ---- isInCurrentLayer_11f0a0.cpp ----
struct MiningGame {
    bool isInCurrentLayer();
};

__attribute__((visibility("hidden"))) extern void **g_MiningGame_sqrt;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layout;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_layerSizes;


bool MiningGame::isInCurrentLayer()
{
    float dx = F(this, 0x10) - (float)I(this, 0x58);
    float dy = F(this, 0x14) - (float)I(this, 0x5c);
    void **sqrtHolder = g_MiningGame_sqrt;
    int current = I(this, 0x78);
    int layer = I(this, 0x7c);
    int *row = g_MiningGame_layerSizes + (7 - layer) * 7;
    void **layoutHolder = g_MiningGame_layout;
    void *globals = *sqrtHolder;
    int size = row[current];
    float scale = F(*layoutHolder, 0xe8);
    float distance = MiningGame_sqrt(globals, dx * dx + dy * dy);
    return distance < scale * (float)(size / 2);
}

// ---- up_11ebb6.cpp ----
struct MiningGame {
    void up(float amount);
};

void MiningGame::up(float amount)
{
    F(this, 0x04) = amount * 3.0f;
}

// ---- gameWon_11eb92.cpp ----
struct MiningGame {
    uint8_t gameWon();
};

uint8_t MiningGame::gameWon()
{
    return UC(this, 0x81);
}

// ---- down_11ebc8.cpp ----
struct MiningGame {
    void down(float amount);
};

void MiningGame::down(float amount)
{
    F(this, 0x04) = amount * 3.0f;
}

// ---- gotCore_11ebac.cpp ----
struct MiningGame {
    uint8_t gotCore();
};

uint8_t MiningGame::gotCore()
{
    return UC(this, 0x83);
}

// ---- left_11ebda.cpp ----
struct MiningGame {
    void left(float amount);
};

void MiningGame::left(float amount)
{
    F(this, 0x00) = amount * 3.0f;
}

// ---- gameLost_11eb98.cpp ----
struct MiningGame {
    uint8_t gameLost();
};

uint8_t MiningGame::gameLost()
{
    return UC(this, 0x82);
}

// ---- right_11ebec.cpp ----
struct MiningGame {
    void right(float amount);
};

void MiningGame::right(float amount)
{
    F(this, 0x00) = amount * 3.0f;
}

// ---- _MiningGame_11eb74.cpp ----
struct MiningGame {
    ~MiningGame();
};


MiningGame::~MiningGame()
{
    void *sprite = P(this, 0x94);
    if (sprite != 0) {
        MiningGame_operator_delete(MiningGame_Sprite_dtor(sprite));
    }
    P(this, 0x94) = 0;
}

// ---- update_11ec00.cpp ----
struct MiningGame {
    // @portable-fields
    unsigned char _pad_0[136];
    void* f_88; // 0x88
    void* f_8c; // 0x8c
    void* f_90; // 0x90
    void* f_94; // 0x94
    unsigned char _pad_98[56];
    void* f_d0; // 0xd0

    bool isInCurrentLayer();
    int update(int delta);
};

__attribute__((visibility("hidden"))) extern void **g_MiningGame_random;
__attribute__((visibility("hidden"))) extern int (*g_MiningGame_randomNext)(void *random, int limit);
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layoutUpdate;
__attribute__((visibility("hidden"))) extern float *g_MiningGame_layerSpeedUpdate;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_sound;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_statusUpdate;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_achievements;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_statusMedal;


int MiningGame::update(int delta)
{
    int pulse = I(this, 0xcc) + delta;
    if (pulse >= 2000) {
        pulse = 0;
    }
    I(this, 0xcc) = pulse;

    int wasInLayer = isInCurrentLayer();
    int driftTimer = I(this, 0x6c) + delta;
    I(this, 0x6c) = driftTimer;
    if (driftTimer >= 2501) {
        void **randomHolder = g_MiningGame_random;
        int (*next)(void *, int) = g_MiningGame_randomNext;
        I(this, 0x6c) = next(*randomHolder, 2000) + 500;

        int value = next(*randomHolder, 7);
        int sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        F(this, 0x08) = ((float)((value + 5) * sign) / 10.0f) / F(this, 0x2c);

        value = next(*randomHolder, 7);
        sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        F(this, 0x0c) = ((float)(sign * (value + 5)) / 10.0f) / F(this, 0x2c);

        if (UC(this, 0x80) != 0 && I(this, 0x78) == I(this, 0x7c) - 1) {
            F(this, 0x08) *= 0.3f;
            F(this, 0x0c) *= 0.3f;
        }
    }

    if (UC(this, 0x84) == 0 && !isInCurrentLayer()) {
        F(this, 0x08) = ((float)I(this, 0x58) - F(this, 0x10)) * 0.03f;
        F(this, 0x0c) = ((float)I(this, 0x5c) - F(this, 0x14)) * 0.03f;
    }

    void **layoutHolder = g_MiningGame_layoutUpdate;
    void *layout = *layoutHolder;
    float frameScale = (float)delta;
    F(this, 0x10) += ((F(this, 0x00) + F(this, 0x08)) / F(layout, 0xe4)) * frameScale;
    F(this, 0x14) += ((F(this, 0x04) + F(this, 0x0c)) / F(layout, 0xe4)) * frameScale;

    MiningGame_MarqueeImage_update(this->f_8c, delta);
    MiningGame_MarqueeImage_update(this->f_90, delta);

    float *layerSpeed = g_MiningGame_layerSpeedUpdate;
    void **soundHolder = g_MiningGame_sound;
    MiningGame_FModSound_setParamValue(*soundHolder, 0, 1,
                                       ((layerSpeed[I(this, 0x78)] - 5.0f) / 33.0f) * 3.0f);

    if (isInCurrentLayer()) {
        if (wasInLayer == 0) {
            MiningGame_FModSound_play(*soundHolder, 1, 0, 0, 0);
            MiningGame_FModSound_stop(*soundHolder, 3);
        }

        float anim = F(this, 0x68) +
                     (frameScale / 1000.0f) * F(layout, 0xe0) * layerSpeed[I(this, 0x78)] * 3.0f;
        F(this, 0x68) = anim;
        if (!(anim < 1.0f)) {
            MiningGame_Sprite_nextFrame(this->f_94);
            F(this, 0x68) = 0.0f;
        }

        MiningGame_MarqueeImage_update(this->f_88, delta);
        int nextLayer = I(this, 0x78) + 1;
        float oldOre = F(this, 0x24);
        float layerFactor = 0.15f + ((float)nextLayer / 7.0f) * 2.35f;
        float newOre = oldOre + ((F(this, 0x28) * layerFactor) / 1000.0f) * frameScale;
        F(this, 0x24) = newOre;
        float alpha = oldOre < newOre ? 0.0f : F(this, 0x64);
        int layerTimer = I(this, 0x74) + delta;
        I(this, 0x74) = layerTimer;
        alpha += frameScale / 500.0f;
        if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        F(this, 0x64) = alpha;

        if (layerTimer > 6000) {
            I(this, 0x74) = 0;
            I(this, 0x78) = nextLayer;
            if (nextLayer >= I(this, 0x7c)) {
                UC(this, 0x81) = 1;
                UC(this, 0x83) = I(this, 0x7c) == 7;
                void **achHolder = g_MiningGame_achievements;
                if (MiningGame_Achievements_hasMedal(*achHolder, 0x26, 1) == 0) {
                    void **statusHolder = g_MiningGame_statusMedal;
                    int count = I(*statusHolder, 0x124) + 1;
                    I(*statusHolder, 0x124) = count;
                    int goal = MiningGame_Achievements_getValue(*achHolder, 0x26, 1);
                    int percent = (int)(((float)count / (float)goal) * 100.0f);
                    MiningGame_idivmod(percent, 10);
                    if (percent % 10 == 0) {
                        int shown = (int)(((float)I(*statusHolder, 0x124) /
                                           (float)MiningGame_Achievements_getValue(*achHolder, 0x26, 1)) *
                                          100.0f);
                        if (shown > 29) {
                            MiningGame_Hud_hudEventMedal(this->f_d0, 0x26, shown);
                        }
                    }
                    if (MiningGame_Achievements_getValue(*achHolder, 0x26, 1) <= I(*statusHolder, 0x124)) {
                        UC(*statusHolder, 0x128) = 1;
                    }
                }
                return 0;
            }
            MiningGame_MarqueeImage_setSpeed_update(this->f_88, F(layout, 0xe0) * layerSpeed[nextLayer]);
        }
    } else {
        if (wasInLayer != 0) {
            MiningGame_FModSound_stop(*soundHolder, 1);
            MiningGame_FModSound_play(*soundHolder, 3, 0, 0, 0);
        }
        int lossTimer = I(this, 0x20) + delta;
        I(this, 0x20) = lossTimer;
        if (lossTimer >= 2501) {
            I(this, 0x20) = 0x9c4;
            I(this, 0x24) = 0;
            UC(this, 0x82) = 1;
            I(*g_MiningGame_statusUpdate, 0x124) = 0;
            return 0;
        }
        F(this, 0x64) = 1.0f;
    }

    return 1;
}

// ---- MiningGame_11e7a8.cpp ----
struct MiningGame {
    // @portable-fields
    unsigned char _pad_0[136];
    void* f_88; // 0x88
    void* f_8c; // 0x8c
    void* f_90; // 0x90
    void* f_94; // 0x94
    unsigned char _pad_98[56];
    void* f_d0; // 0xd0

    MiningGame(int layer, int station, Hud *hud);
};

__attribute__((visibility("hidden"))) extern int *g_MiningGame_screenW;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_screenH;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layoutCtor;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_statusCtor;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_canvasCtor;
__attribute__((visibility("hidden"))) extern void (*g_MiningGame_imageCreate)(void *canvas, int imageId, int *outId);
__attribute__((visibility("hidden"))) extern int (*g_MiningGame_imageWidth)(void *canvas, int imageId);
__attribute__((visibility("hidden"))) extern float *g_MiningGame_layerSpeed;


MiningGame::MiningGame(int layer, int station, Hud *hud)
{
    int imageId[2];
    imageId[0] = -1;

    int *screenW = g_MiningGame_screenW;
    int *screenH = g_MiningGame_screenH;
    void **layoutHolder = g_MiningGame_layoutCtor;
    void **statusHolder = g_MiningGame_statusCtor;

    I(this, 0x18) = layer;
    I(this, 0x1c) = station;
    this->f_d0 = hud;

    void *layout = *layoutHolder;
    int centerX = *screenW >> 1;
    I(this, 0x58) = centerX;
    int centerY = I(layout, 0xd0) + (*screenH >> 1);
    I(this, 0x5c) = centerY;
    I(this, 0xc8) = I(layout, 0xd4);
    UC(this, 0x80) = layer == 7;
    I(this, 0x20) = 0;
    I(this, 0x24) = 0;
    I(this, 0x28) = 0;
    I(this, 0x6c) = 0;
    I(this, 0x70) = 0x9c4;
    I(this, 0x74) = 0;
    I(this, 0x78) = 0;
    I(this, 0x7c) = layer;
    *(uint16_t *)B(this, 0x81) = 0;
    UC(this, 0x83) = 0;
    F(this, 0x10) = (float)centerX;
    F(this, 0x00) = 0.0f;
    F(this, 0x04) = 0.0f;
    F(this, 0x08) = 0.0f;
    F(this, 0x0c) = 0.0f;
    F(this, 0x14) = (float)centerY;

    void *ship = MiningGame_Status_getShip(*statusHolder);
    void *equipment = MiningGame_Ship_getFirstEquipmentOfSort(ship, 0x13);
    if (equipment != 0) {
        int value = MiningGame_Item_getAttribute(equipment, 0x20);
        F(this, 0x2c) = 0.3f + ((float)value / 100.0f) * 1.5f;
        value = MiningGame_Item_getAttribute(equipment, 0x21);
        F(this, 0x28) = (float)value / 100.0f;
    }

    void **canvasHolder = g_MiningGame_canvasCtor;
    MiningGame_PaintCanvas_Image2DCreate(*canvasHolder, 0x4e6, imageId);
    int imageHeight = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, imageId[0]);
    void *sprite = MiningGame_operator_new(0x40);
    MiningGame_Sprite_ctor(sprite, imageId[0], imageHeight, imageHeight);
    this->f_94 = sprite;
    MiningGame_Sprite_defineReferencePixel(sprite, imageHeight / 2, imageHeight / 2);

    I(this, 0x68) = 0;
    void (*imageCreate)(void *, int, int *) = g_MiningGame_imageCreate;
    imageCreate(*canvasHolder, 0x4e2, (int *)B(this, 0xac));
    imageCreate(*canvasHolder, 0x4dd, (int *)B(this, 0xb0));
    imageCreate(*canvasHolder, 0x4de, (int *)B(this, 0xb4));
    imageCreate(*canvasHolder, 0x4e1, (int *)B(this, 0xb8));
    imageCreate(*canvasHolder, 0x4df, (int *)B(this, 0xbc));
    imageCreate(*canvasHolder, 0x4e0, (int *)B(this, 0xc0));
    imageCreate(*canvasHolder, 0x4e5, (int *)B(this, 0x9c));
    imageCreate(*canvasHolder, 0x4e4, (int *)B(this, 0xa0));
    imageCreate(*canvasHolder, 0x4e7, (int *)B(this, 0x98));
    imageCreate(*canvasHolder, 0x4e3, (int *)B(this, 0xa4));
    imageCreate(*canvasHolder, 0x4e8, (int *)B(this, 0xa8));
    imageCreate(*canvasHolder, 0x4ed, (int *)B(this, 0xc4));

    if (UC(this, 0x80) != 0) {
        int coreImage = 0x523;
        if (station == 0xa4) {
            coreImage = 0x522;
        }
        MiningGame_PaintCanvas_Image2DCreate(*canvasHolder, coreImage, (int *)B(this, 0x60));
    }

    I(this, 0x48) = MiningGame_PaintCanvas_GetImage2DWidth(*canvasHolder, I(this, 0xa8));
    I(this, 0x4c) = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0xa8));
    int x = *screenW / 2 - I(this, 0x48) / 2;
    I(this, 0x50) = x;
    int y = I(layout, 0xd8);
    I(this, 0x54) = y;

    void *leftMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(leftMarquee, 0x4eb, I(this, 0xc8), x, I(this, 0x4c) + y + 5, 20.0f);
    this->f_8c = leftMarquee;

    void *rightMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(rightMarquee, 0x4ec, I(this, 0xc8),
                                 (I(this, 0x50) - I(this, 0xc8)) + I(this, 0x48),
                                 I(this, 0x4c) + I(this, 0x54) + 5, 32.0f);
    this->f_90 = rightMarquee;

    int (*imageWidth)(void *, int) = g_MiningGame_imageWidth;
    I(this, 0x40) = imageWidth(*canvasHolder, I(this, 0x98)) / 2 + 5;
    I(this, 0x44) = imageWidth(*canvasHolder, I(this, 0x9c)) / 2;
    I(this, 0x34) = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0xa0));

    void *oreMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(oreMarquee, 0x4e4, imageWidth(*canvasHolder, I(this, 0x9c)) - 8, 0, 0,
                                 F(layout, 0xdc));
    this->f_88 = oreMarquee;
    MiningGame_MarqueeImage_setSpeed(oreMarquee, F(layout, 0xe0) * g_MiningGame_layerSpeed[I(this, 0x78)]);

    I(this, 0xcc) = 0;
    F(this, 0x64) = 1.0f;
    UC(this, 0x84) = MiningGame_Status_getCurrentCampaignMission(*statusHolder) > 4;
}

// ---- render2D_11f12c.cpp ----
struct MiningGame {
    // @portable-fields
    unsigned char _pad_0[136];
    void* f_88; // 0x88
    void* f_8c; // 0x8c
    void* f_90; // 0x90
    void* f_94; // 0x94
    unsigned char _pad_98[56];
    void* f_d0; // 0xd0

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

extern "C" void MiningGame_PaintCanvas_DrawRegion2D(void *canvas, int image, int sx, int sy, int w, int h,
                                                     int dx, int dy, int unused0, int unused1, int x);

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
    MiningGame_Sprite_setRefPixelPosition(this->f_94, (int)F(this, 0x10), (int)F(this, 0x14));
    MiningGame_Sprite_draw(this->f_94, 1.0f, 1.0f);

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

    MiningGame_MarqueeImage_draw(this->f_8c);
    MiningGame_MarqueeImage_draw(this->f_90);
    MiningGame_MarqueeImage_drawAt(this->f_88, (int)(F(this, 0x10) + (float)I(this, 0x40)),
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
