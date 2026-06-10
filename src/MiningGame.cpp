#include "gof2/MiningGame.h"


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

// helper to read a generic int field at an arbitrary offset from a void* (engine objects)
static inline int &I(void *p, unsigned off) { return *(int *)((char *)p + off); }
static inline float &F(void *p, unsigned off) { return *(float *)((char *)p + off); }
static inline uint8_t &UC(void *p, unsigned off) { return *(uint8_t *)((char *)p + off); }

// ---- getOreAmount_11eb9e.cpp ----
int MiningGame::getOreAmount()
{
    return (int)this->field_0x24;
}

// ---- isInCurrentLayer_11f0a0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MiningGame_sqrt;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layout;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_layerSizes;


bool MiningGame::isInCurrentLayer()
{
    float dx = this->field_0x10 - (float)this->field_0x58;
    float dy = this->field_0x14 - (float)this->field_0x5c;
    void **sqrtHolder = g_MiningGame_sqrt;
    int current = this->field_0x78;
    int layer = this->field_0x7c;
    int *row = g_MiningGame_layerSizes + (7 - layer) * 7;
    void **layoutHolder = g_MiningGame_layout;
    void *globals = *sqrtHolder;
    int size = row[current];
    float scale = F(*layoutHolder, 0xe8);
    float distance = MiningGame_sqrt(globals, dx * dx + dy * dy);
    return distance < scale * (float)(size / 2);
}

// ---- up_11ebb6.cpp ----
void MiningGame::up(float amount)
{
    this->field_0x4 = amount * 3.0f;
}

// ---- gameWon_11eb92.cpp ----
uint8_t MiningGame::gameWon()
{
    return this->field_0x81;
}

// ---- down_11ebc8.cpp ----
void MiningGame::down(float amount)
{
    this->field_0x4 = amount * 3.0f;
}

// ---- gotCore_11ebac.cpp ----
uint8_t MiningGame::gotCore()
{
    return this->field_0x83;
}

// ---- left_11ebda.cpp ----
void MiningGame::left(float amount)
{
    this->field_0x0 = amount * 3.0f;
}

// ---- gameLost_11eb98.cpp ----
uint8_t MiningGame::gameLost()
{
    return this->field_0x82;
}

// ---- right_11ebec.cpp ----
void MiningGame::right(float amount)
{
    this->field_0x0 = amount * 3.0f;
}

// ---- _MiningGame_11eb74.cpp ----
MiningGame::~MiningGame()
{
    void *sprite = this->field_0x94;
    if (sprite != 0) {
        MiningGame_operator_delete(MiningGame_Sprite_dtor(sprite));
    }
    this->field_0x94 = 0;
}

// ---- update_11ec00.cpp ----
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
    int pulse = this->field_0xcc + delta;
    if (pulse >= 2000) {
        pulse = 0;
    }
    this->field_0xcc = pulse;

    int wasInLayer = isInCurrentLayer();
    int driftTimer = this->field_0x6c + delta;
    this->field_0x6c = driftTimer;
    if (driftTimer >= 2501) {
        void **randomHolder = g_MiningGame_random;
        int (*next)(void *, int) = g_MiningGame_randomNext;
        this->field_0x6c = next(*randomHolder, 2000) + 500;

        int value = next(*randomHolder, 7);
        int sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        this->field_0x8 = ((float)((value + 5) * sign) / 10.0f) / this->field_0x2c;

        value = next(*randomHolder, 7);
        sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        this->field_0xc = ((float)(sign * (value + 5)) / 10.0f) / this->field_0x2c;

        if (this->field_0x80 != 0 && this->field_0x78 == this->field_0x7c - 1) {
            this->field_0x8 *= 0.3f;
            this->field_0xc *= 0.3f;
        }
    }

    if (this->field_0x84 == 0 && !isInCurrentLayer()) {
        this->field_0x8 = ((float)this->field_0x58 - this->field_0x10) * 0.03f;
        this->field_0xc = ((float)this->field_0x5c - this->field_0x14) * 0.03f;
    }

    void **layoutHolder = g_MiningGame_layoutUpdate;
    void *layout = *layoutHolder;
    float frameScale = (float)delta;
    this->field_0x10 += ((this->field_0x0 + this->field_0x8) / F(layout, 0xe4)) * frameScale;
    this->field_0x14 += ((this->field_0x4 + this->field_0xc) / F(layout, 0xe4)) * frameScale;

    MiningGame_MarqueeImage_update(this->field_0x8c, delta);
    MiningGame_MarqueeImage_update(this->field_0x90, delta);

    float *layerSpeed = g_MiningGame_layerSpeedUpdate;
    void **soundHolder = g_MiningGame_sound;
    MiningGame_FModSound_setParamValue(*soundHolder, 0, 1,
                                       ((layerSpeed[this->field_0x78] - 5.0f) / 33.0f) * 3.0f);

    if (isInCurrentLayer()) {
        if (wasInLayer == 0) {
            MiningGame_FModSound_play(*soundHolder, 1, 0, 0, 0);
            MiningGame_FModSound_stop(*soundHolder, 3);
        }

        float anim = this->field_0x68 +
                     (frameScale / 1000.0f) * F(layout, 0xe0) * layerSpeed[this->field_0x78] * 3.0f;
        this->field_0x68 = anim;
        if (!(anim < 1.0f)) {
            MiningGame_Sprite_nextFrame(this->field_0x94);
            this->field_0x68 = 0.0f;
        }

        MiningGame_MarqueeImage_update(this->field_0x88, delta);
        int nextLayer = this->field_0x78 + 1;
        float oldOre = this->field_0x24;
        float layerFactor = 0.15f + ((float)nextLayer / 7.0f) * 2.35f;
        float newOre = oldOre + ((this->field_0x28 * layerFactor) / 1000.0f) * frameScale;
        this->field_0x24 = newOre;
        float alpha = oldOre < newOre ? 0.0f : this->field_0x64;
        int layerTimer = this->field_0x74 + delta;
        this->field_0x74 = layerTimer;
        alpha += frameScale / 500.0f;
        if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        this->field_0x64 = alpha;

        if (layerTimer > 6000) {
            this->field_0x74 = 0;
            this->field_0x78 = nextLayer;
            if (nextLayer >= this->field_0x7c) {
                this->field_0x81 = 1;
                this->field_0x83 = this->field_0x7c == 7;
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
                            MiningGame_Hud_hudEventMedal(this->field_0xd0, 0x26, shown);
                        }
                    }
                    if (MiningGame_Achievements_getValue(*achHolder, 0x26, 1) <= I(*statusHolder, 0x124)) {
                        UC(*statusHolder, 0x128) = 1;
                    }
                }
                return 0;
            }
            MiningGame_MarqueeImage_setSpeed_update(this->field_0x88, F(layout, 0xe0) * layerSpeed[nextLayer]);
        }
    } else {
        if (wasInLayer != 0) {
            MiningGame_FModSound_stop(*soundHolder, 1);
            MiningGame_FModSound_play(*soundHolder, 3, 0, 0, 0);
        }
        int lossTimer = this->field_0x20 + delta;
        this->field_0x20 = lossTimer;
        if (lossTimer >= 2501) {
            this->field_0x20 = 0x9c4;
            this->field_0x24 = 0.0f;
            this->field_0x82 = 1;
            I(*g_MiningGame_statusUpdate, 0x124) = 0;
            return 0;
        }
        this->field_0x64 = 1.0f;
    }

    return 1;
}

// ---- MiningGame_11e7a8.cpp ----
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

    this->field_0x18 = layer;
    this->field_0x1c = station;
    this->field_0xd0 = hud;

    void *layout = *layoutHolder;
    int centerX = *screenW >> 1;
    this->field_0x58 = centerX;
    int centerY = I(layout, 0xd0) + (*screenH >> 1);
    this->field_0x5c = centerY;
    this->field_0xc8 = I(layout, 0xd4);
    this->field_0x80 = layer == 7;
    this->field_0x20 = 0;
    this->field_0x24 = 0.0f;
    this->field_0x28 = 0.0f;
    this->field_0x6c = 0;
    this->field_0x70 = 0x9c4;
    this->field_0x74 = 0;
    this->field_0x78 = 0;
    this->field_0x7c = layer;
    this->field_0x81 = 0;
    this->field_0x82 = 0;
    this->field_0x83 = 0;
    this->field_0x10 = (float)centerX;
    this->field_0x0 = 0.0f;
    this->field_0x4 = 0.0f;
    this->field_0x8 = 0.0f;
    this->field_0xc = 0.0f;
    this->field_0x14 = (float)centerY;

    void *ship = MiningGame_Status_getShip(*statusHolder);
    void *equipment = MiningGame_Ship_getFirstEquipmentOfSort(ship, 0x13);
    if (equipment != 0) {
        int value = MiningGame_Item_getAttribute(equipment, 0x20);
        this->field_0x2c = 0.3f + ((float)value / 100.0f) * 1.5f;
        value = MiningGame_Item_getAttribute(equipment, 0x21);
        this->field_0x28 = (float)value / 100.0f;
    }

    void **canvasHolder = g_MiningGame_canvasCtor;
    MiningGame_PaintCanvas_Image2DCreate(*canvasHolder, 0x4e6, imageId);
    int imageHeight = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, imageId[0]);
    void *sprite = MiningGame_operator_new(0x40);
    MiningGame_Sprite_ctor(sprite, imageId[0], imageHeight, imageHeight);
    this->field_0x94 = sprite;
    MiningGame_Sprite_defineReferencePixel(sprite, imageHeight / 2, imageHeight / 2);

    this->field_0x68 = 0.0f;
    void (*imageCreate)(void *, int, int *) = g_MiningGame_imageCreate;
    imageCreate(*canvasHolder, 0x4e2, &this->field_0xac);
    imageCreate(*canvasHolder, 0x4dd, &this->field_0xb0);
    imageCreate(*canvasHolder, 0x4de, &this->field_0xb4);
    imageCreate(*canvasHolder, 0x4e1, &this->field_0xb8);
    imageCreate(*canvasHolder, 0x4df, &this->field_0xbc);
    imageCreate(*canvasHolder, 0x4e0, &this->field_0xc0);
    imageCreate(*canvasHolder, 0x4e5, &this->field_0x9c);
    imageCreate(*canvasHolder, 0x4e4, &this->field_0xa0);
    imageCreate(*canvasHolder, 0x4e7, &this->field_0x98);
    imageCreate(*canvasHolder, 0x4e3, &this->field_0xa4);
    imageCreate(*canvasHolder, 0x4e8, &this->field_0xa8);
    imageCreate(*canvasHolder, 0x4ed, &this->field_0xc4);

    if (this->field_0x80 != 0) {
        int coreImage = 0x523;
        if (station == 0xa4) {
            coreImage = 0x522;
        }
        MiningGame_PaintCanvas_Image2DCreate(*canvasHolder, coreImage, &this->field_0x60);
    }

    this->field_0x48 = MiningGame_PaintCanvas_GetImage2DWidth(*canvasHolder, this->field_0xa8);
    this->field_0x4c = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, this->field_0xa8);
    int x = *screenW / 2 - this->field_0x48 / 2;
    this->field_0x50 = x;
    int y = I(layout, 0xd8);
    this->field_0x54 = y;

    void *leftMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(leftMarquee, 0x4eb, this->field_0xc8, x, this->field_0x4c + y + 5, 20.0f);
    this->field_0x8c = leftMarquee;

    void *rightMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(rightMarquee, 0x4ec, this->field_0xc8,
                                 (this->field_0x50 - this->field_0xc8) + this->field_0x48,
                                 this->field_0x4c + this->field_0x54 + 5, 32.0f);
    this->field_0x90 = rightMarquee;

    int (*imageWidth)(void *, int) = g_MiningGame_imageWidth;
    this->field_0x40 = imageWidth(*canvasHolder, this->field_0x98) / 2 + 5;
    this->field_0x44 = imageWidth(*canvasHolder, this->field_0x9c) / 2;
    this->field_0x34 = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, this->field_0xa0);

    void *oreMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(oreMarquee, 0x4e4, imageWidth(*canvasHolder, this->field_0x9c) - 8, 0, 0,
                                 F(layout, 0xdc));
    this->field_0x88 = oreMarquee;
    MiningGame_MarqueeImage_setSpeed(oreMarquee, F(layout, 0xe0) * g_MiningGame_layerSpeed[this->field_0x78]);

    this->field_0xcc = 0;
    this->field_0x64 = 1.0f;
    this->field_0x84 = MiningGame_Status_getCurrentCampaignMission(*statusHolder) > 4;
}

// ---- render2D_11f12c.cpp ----
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
    String amountStorage;
    String suffixStorage;
    String oreStorage;
    String *amountText = &amountStorage;
    String *suffixText = &suffixStorage;
    String *oreText = &oreStorage;

    void **canvasHolder = g_MiningGame_canvasRender;
    void *canvas = *canvasHolder;
    MiningGame_PaintCanvas_SetColor(canvas, -1);

    int *layerTable = g_MiningGame_layerTableRender;
    void **layoutHolder = g_MiningGame_layoutRender;
    void *layout = *layoutHolder;
    void (*drawLayer)(void *, int, int, int, int, int, int, int, int) = g_MiningGame_drawLayer;

    for (int layerIndex = this->field_0x78; layerIndex < this->field_0x7c; layerIndex++) {
        int raw = layerTable[(layerIndex - this->field_0x7c * 7) + 0x31];
        int radius = (int)(F(layout, 0xe8) * (float)raw);
        int *imageSlot;
        if ((layerIndex & 1) == 0) {
            imageSlot = &this->field_0xac;
            if (I(layout, 0xec) < radius) {
                imageSlot = &this->field_0xb4;
                if (radius < I(layout, 0xf4)) {
                    imageSlot = &this->field_0xb0;
                }
            }
        } else {
            imageSlot = &this->field_0xb8;
            if (I(layout, 0xec) < radius) {
                imageSlot = &this->field_0xc0;
                if (radius < I(layout, 0xf4)) {
                    imageSlot = &this->field_0xbc;
                }
            }
        }
        int image = *imageSlot;
        int half = radius / 2;
        drawLayer(canvas, image, this->field_0x58, this->field_0x5c, half, half, 0x11, 0x22, 0);
        drawLayer(canvas, image, this->field_0x58, this->field_0x5c, half, half, 0x11, 0x21, 1);
        drawLayer(canvas, image, this->field_0x58, this->field_0x5c, half, half, 0x11, 0x12, 2);
        drawLayer(canvas, image, this->field_0x58, this->field_0x5c, half, half, 0x11, 0x11, 3);
    }

    if (this->field_0x80 != 0) {
        MiningGame_PaintCanvas_DrawImage2D_anchor(canvas, this->field_0x60, this->field_0x58, this->field_0x5c, 0x4411);
    }

    MiningGame_PaintCanvas_DrawImage2D_anchor(canvas, this->field_0x98, (int)this->field_0x10, (int)this->field_0x14,
                                             0x4411);
    MiningGame_Sprite_setRefPixelPosition(this->field_0x94, (int)this->field_0x10, (int)this->field_0x14);
    MiningGame_Sprite_draw(this->field_0x94, 1.0f, 1.0f);

    MiningGame_PaintCanvas_DrawImage2D(canvas, this->field_0xa4, this->field_0x50 - I(layout, 0xfc),
                                       this->field_0x54 - I(layout, 0xfc));

    int lossTimer = this->field_0x20;
    if (lossTimer > 0x341) {
        float red = MiningGame_Layout_getPulseValue(layout, 10.0f) * 255.0f;
        float green = MiningGame_Layout_getPulseValue(layout, 10.0f) * 255.0f;
        MiningGame_PaintCanvas_SetColorRGBA(canvas, 0xff, (int)red, (int)green, 0xff);
    }

    int width = (int)(((2500.0f - (float)lossTimer) / 2500.0f) * (float)this->field_0x48);
    MiningGame_PaintCanvas_DrawRegion2D(canvas, this->field_0xa8, 0, 0, width, this->field_0x4c, width, 0, 0, 0,
                                        this->field_0x50);
    MiningGame_PaintCanvas_SetColor(canvas, -1);
    MiningGame_PaintCanvas_DrawImage2D_anchor(canvas, this->field_0xc4, this->field_0x58, this->field_0x54 - 3, 0x2411);

    MiningGame_MarqueeImage_draw(this->field_0x8c);
    MiningGame_MarqueeImage_draw(this->field_0x90);
    MiningGame_MarqueeImage_drawAt(this->field_0x88, (int)(this->field_0x10 + (float)this->field_0x40),
                                   (int)(this->field_0x14 - (float)this->field_0x34));

    MiningGame_PaintCanvas_DrawImage2D(canvas, this->field_0x9c,
                                       (int)((this->field_0x10 + (float)this->field_0x40) - (float)I(layout, 0xfc)),
                                       (int)(this->field_0x14 - (float)I(layout, 0x100)));

    MiningGame_String_ctor_int(amountText, (int)this->field_0x24);
    MiningGame_String_ctor_char(suffixText, g_MiningGame_oreSuffix, false);
    MiningGame_String_plus(oreText, amountText, suffixText);
    MiningGame_String_dtor(suffixText);
    MiningGame_String_dtor(amountText);

    void *ship = MiningGame_Status_getShip_render(*g_MiningGame_statusRender);
    int freeSpace = MiningGame_Ship_getFreeSpace(ship);
    int alpha = (int)(this->field_0x64 * 255.0f);
    if (freeSpace < (int)this->field_0x24) {
        MiningGame_PaintCanvas_SetColorRGBA(canvas, 0xff, 0x2a, 0, alpha);
    } else {
        MiningGame_PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, alpha);
    }

    String *font = (String *)*g_MiningGame_fontString;
    int textWidth = MiningGame_PaintCanvas_GetTextWidth(canvas, font, oreText);
    int textX = (int)(((this->field_0x10 + (float)this->field_0x40 + (float)this->field_0x44) -
                      (float)I(layout, 0xfc)) -
                     (float)(textWidth / 2));
    int textY = (int)(this->field_0x14 + (float)I(layout, 0x104));
    MiningGame_PaintCanvas_DrawString(canvas, font, oreText, textX, textY, false);
    MiningGame_PaintCanvas_SetColor(canvas, -1);

    if (MiningGame_Status_getCurrentCampaignMission_render(*g_MiningGame_statusRender) < 5) {
        int promptAlpha = (int)(((float)this->field_0xcc / 2500.0f) * 255.0f);
        if (promptAlpha > 255) {
            promptAlpha = 255 - promptAlpha;
        }
        MiningGame_PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, (uint8_t)promptAlpha);
        String *prompt = MiningGame_GameText_getText(*g_MiningGame_gameText, 0x268);
        MiningGame_String_ctor_copy(amountText, prompt, false);
        int promptWidth = MiningGame_PaintCanvas_GetTextWidth(canvas, font, amountText);
        MiningGame_PaintCanvas_DrawString(canvas, font, amountText, *g_MiningGame_screenWRender / 2 - promptWidth / 2,
                                          I(layout, 0x70) + this->field_0x54, false);
        MiningGame_PaintCanvas_SetColor(canvas, -1);
        MiningGame_String_dtor(amountText);
    }

    MiningGame_String_dtor(oreText);
}
