#include "gof2/MiningGame.h"
#include "gof2/PaintCanvasClass.h"


extern "C" float MiningGame_sqrt(void *globals, float value);
extern "C" void *MiningGame_Sprite_dtor(void *sprite);
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
extern "C" void MiningGame_Sprite_ctor(void *sprite, int imageId, int width, int height);
extern "C" void MiningGame_Sprite_defineReferencePixel(void *sprite, int x, int y);
extern "C" void MiningGame_MarqueeImage_ctor(void *self, int imageId, int width, int x, int y, float value);
extern "C" void MiningGame_MarqueeImage_setSpeed(void *self, float speed);
extern "C" int MiningGame_Status_getCurrentCampaignMission(void *status);
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
extern "C" int MiningGame_Status_getCurrentCampaignMission_render(void *status);
extern "C" String *MiningGame_GameText_getText(void *gameText, int id);

// helper to read a generic int field at an arbitrary offset from a void* (engine objects)
static inline int &I(void *p, unsigned off) { return *(int *)((char *)p + off); }
static inline float &F(void *p, unsigned off) { return *(float *)((char *)p + off); }
static inline uint8_t &UC(void *p, unsigned off) { return *(uint8_t *)((char *)p + off); }

// ---- getOreAmount_11eb9e.cpp ----
int MiningGame::getOreAmount()
{
    return (int)this->oreAmount;
}

// ---- isInCurrentLayer_11f0a0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MiningGame_sqrt;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layout;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_layerSizes;


bool MiningGame::isInCurrentLayer()
{
    float dx = this->posX - (float)this->centerX;
    float dy = this->posY - (float)this->centerY;
    void **sqrtHolder = g_MiningGame_sqrt;
    int current = this->currentLayer;
    int layer = this->targetLayer;
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
    this->inputY = amount * 3.0f;
}

// ---- gameWon_11eb92.cpp ----
uint8_t MiningGame::gameWon()
{
    return this->gameWonFlag;
}

// ---- down_11ebc8.cpp ----
void MiningGame::down(float amount)
{
    this->inputY = amount * 3.0f;
}

// ---- gotCore_11ebac.cpp ----
uint8_t MiningGame::gotCore()
{
    return this->gotCoreFlag;
}

// ---- left_11ebda.cpp ----
void MiningGame::left(float amount)
{
    this->inputX = amount * 3.0f;
}

// ---- gameLost_11eb98.cpp ----
uint8_t MiningGame::gameLost()
{
    return this->gameLostFlag;
}

// ---- right_11ebec.cpp ----
void MiningGame::right(float amount)
{
    this->inputX = amount * 3.0f;
}

// ---- steer veneers (0x1abaf8..0x1abb38) ----
// PlayerEgo's pitch/yaw input handlers reach the mining game through these
// interworking thunks. Each forwards the steering delta to the corresponding
// axis handler and reports the resulting axis input back to the caller.
float MiningGame::steerXR(float delta)
{
    right(delta);
    return this->inputX;
}

float MiningGame::steerX(float delta)
{
    left(delta);
    return this->inputX;
}

float MiningGame::steerY(float delta)
{
    down(delta);
    return this->inputY;
}

float MiningGame::steerYAlt(float delta)
{
    up(delta);
    return this->inputY;
}

// ---- _MiningGame_11eb74.cpp ----
MiningGame::~MiningGame()
{
    void *sprite = this->drillSprite;
    if (sprite != 0) {
        ::operator delete(MiningGame_Sprite_dtor(sprite));
    }
    this->drillSprite = 0;
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
    int pulse = this->promptPulseTimer + delta;
    if (pulse >= 2000) {
        pulse = 0;
    }
    this->promptPulseTimer = pulse;

    int wasInLayer = isInCurrentLayer();
    int driftTimer = this->driftTimer + delta;
    this->driftTimer = driftTimer;
    if (driftTimer >= 2501) {
        void **randomHolder = g_MiningGame_random;
        int (*next)(void *, int) = g_MiningGame_randomNext;
        this->driftTimer = next(*randomHolder, 2000) + 500;

        int value = next(*randomHolder, 7);
        int sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        this->driftX = ((float)((value + 5) * sign) / 10.0f) / this->controlDivisor;

        value = next(*randomHolder, 7);
        sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        this->driftY = ((float)(sign * (value + 5)) / 10.0f) / this->controlDivisor;

        if (this->isCoreLayer != 0 && this->currentLayer == this->targetLayer - 1) {
            this->driftX *= 0.3f;
            this->driftY *= 0.3f;
        }
    }

    if (this->campaignFlag == 0 && !isInCurrentLayer()) {
        this->driftX = ((float)this->centerX - this->posX) * 0.03f;
        this->driftY = ((float)this->centerY - this->posY) * 0.03f;
    }

    void **layoutHolder = g_MiningGame_layoutUpdate;
    void *layout = *layoutHolder;
    float frameScale = (float)delta;
    this->posX += ((this->inputX + this->driftX) / F(layout, 0xe4)) * frameScale;
    this->posY += ((this->inputY + this->driftY) / F(layout, 0xe4)) * frameScale;

    MiningGame_MarqueeImage_update(this->leftMarquee, delta);
    MiningGame_MarqueeImage_update(this->rightMarquee, delta);

    float *layerSpeed = g_MiningGame_layerSpeedUpdate;
    void **soundHolder = g_MiningGame_sound;
    MiningGame_FModSound_setParamValue(*soundHolder, 0, 1,
                                       ((layerSpeed[this->currentLayer] - 5.0f) / 33.0f) * 3.0f);

    if (isInCurrentLayer()) {
        if (wasInLayer == 0) {
            MiningGame_FModSound_play(*soundHolder, 1, 0, 0, 0);
            MiningGame_FModSound_stop(*soundHolder, 3);
        }

        float anim = this->animAccumulator +
                     (frameScale / 1000.0f) * F(layout, 0xe0) * layerSpeed[this->currentLayer] * 3.0f;
        this->animAccumulator = anim;
        if (!(anim < 1.0f)) {
            MiningGame_Sprite_nextFrame(this->drillSprite);
            this->animAccumulator = 0.0f;
        }

        MiningGame_MarqueeImage_update(this->oreMarquee, delta);
        int nextLayer = this->currentLayer + 1;
        float oldOre = this->oreAmount;
        float layerFactor = 0.15f + ((float)nextLayer / 7.0f) * 2.35f;
        float newOre = oldOre + ((this->oreRate * layerFactor) / 1000.0f) * frameScale;
        this->oreAmount = newOre;
        float alpha = oldOre < newOre ? 0.0f : this->textAlpha;
        int layerTimer = this->layerTimer + delta;
        this->layerTimer = layerTimer;
        alpha += frameScale / 500.0f;
        if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        this->textAlpha = alpha;

        if (layerTimer > 6000) {
            this->layerTimer = 0;
            this->currentLayer = nextLayer;
            if (nextLayer >= this->targetLayer) {
                this->gameWonFlag = 1;
                this->gotCoreFlag = this->targetLayer == 7;
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
                            MiningGame_Hud_hudEventMedal(this->hud, 0x26, shown);
                        }
                    }
                    if (MiningGame_Achievements_getValue(*achHolder, 0x26, 1) <= I(*statusHolder, 0x124)) {
                        UC(*statusHolder, 0x128) = 1;
                    }
                }
                return 0;
            }
            MiningGame_MarqueeImage_setSpeed_update(this->oreMarquee, F(layout, 0xe0) * layerSpeed[nextLayer]);
        }
    } else {
        if (wasInLayer != 0) {
            MiningGame_FModSound_stop(*soundHolder, 1);
            MiningGame_FModSound_play(*soundHolder, 3, 0, 0, 0);
        }
        int lossTimer = this->lossTimer + delta;
        this->lossTimer = lossTimer;
        if (lossTimer >= 2501) {
            this->lossTimer = 0x9c4;
            this->oreAmount = 0.0f;
            this->gameLostFlag = 1;
            I(*g_MiningGame_statusUpdate, 0x124) = 0;
            return 0;
        }
        this->textAlpha = 1.0f;
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

    this->layer = layer;
    this->station = station;
    this->hud = hud;

    void *layout = *layoutHolder;
    int centerX = *screenW >> 1;
    this->centerX = centerX;
    int centerY = I(layout, 0xd0) + (*screenH >> 1);
    this->centerY = centerY;
    this->marqueeWidth = I(layout, 0xd4);
    this->isCoreLayer = layer == 7;
    this->lossTimer = 0;
    this->oreAmount = 0.0f;
    this->oreRate = 0.0f;
    this->driftTimer = 0;
    this->field_0x70 = 0x9c4;
    this->layerTimer = 0;
    this->currentLayer = 0;
    this->targetLayer = layer;
    this->gameWonFlag = 0;
    this->gameLostFlag = 0;
    this->gotCoreFlag = 0;
    this->posX = (float)centerX;
    this->inputX = 0.0f;
    this->inputY = 0.0f;
    this->driftX = 0.0f;
    this->driftY = 0.0f;
    this->posY = (float)centerY;

    void *ship = MiningGame_Status_getShip(*statusHolder);
    void *equipment = MiningGame_Ship_getFirstEquipmentOfSort(ship, 0x13);
    if (equipment != 0) {
        int value = MiningGame_Item_getAttribute(equipment, 0x20);
        this->controlDivisor = 0.3f + ((float)value / 100.0f) * 1.5f;
        value = MiningGame_Item_getAttribute(equipment, 0x21);
        this->oreRate = (float)value / 100.0f;
    }

    void **canvasHolder = g_MiningGame_canvasCtor;
    ((PaintCanvas *)*canvasHolder)->Image2DCreate(0x4e6, (unsigned int *)imageId);
    int imageHeight = ((PaintCanvas *)*canvasHolder)->GetImage2DHeight(imageId[0]);
    void *sprite = ::operator new(0x40);
    MiningGame_Sprite_ctor(sprite, imageId[0], imageHeight, imageHeight);
    this->drillSprite = sprite;
    MiningGame_Sprite_defineReferencePixel(sprite, imageHeight / 2, imageHeight / 2);

    this->animAccumulator = 0.0f;
    void (*imageCreate)(void *, int, int *) = g_MiningGame_imageCreate;
    imageCreate(*canvasHolder, 0x4e2, &this->field_0xac);
    imageCreate(*canvasHolder, 0x4dd, &this->field_0xb0);
    imageCreate(*canvasHolder, 0x4de, &this->field_0xb4);
    imageCreate(*canvasHolder, 0x4e1, &this->field_0xb8);
    imageCreate(*canvasHolder, 0x4df, &this->field_0xbc);
    imageCreate(*canvasHolder, 0x4e0, &this->field_0xc0);
    imageCreate(*canvasHolder, 0x4e5, &this->field_0x9c);
    imageCreate(*canvasHolder, 0x4e4, &this->field_0xa0);
    imageCreate(*canvasHolder, 0x4e7, &this->oreIconImageId);
    imageCreate(*canvasHolder, 0x4e3, &this->field_0xa4);
    imageCreate(*canvasHolder, 0x4e8, &this->progressBarImageId);
    imageCreate(*canvasHolder, 0x4ed, &this->field_0xc4);

    if (this->isCoreLayer != 0) {
        int coreImage = 0x523;
        if (station == 0xa4) {
            coreImage = 0x522;
        }
        ((PaintCanvas *)*canvasHolder)->Image2DCreate(coreImage, (unsigned int *)&this->coreImageId);
    }

    this->progressBarWidth = ((PaintCanvas *)*canvasHolder)->GetImage2DWidth(this->progressBarImageId);
    this->progressBarHeight = ((PaintCanvas *)*canvasHolder)->GetImage2DHeight(this->progressBarImageId);
    int x = *screenW / 2 - this->progressBarWidth / 2;
    this->progressBarX = x;
    int y = I(layout, 0xd8);
    this->progressBarY = y;

    void *leftMarquee = ::operator new(0x24);
    MiningGame_MarqueeImage_ctor(leftMarquee, 0x4eb, this->marqueeWidth, x, this->progressBarHeight + y + 5, 20.0f);
    this->leftMarquee = leftMarquee;

    void *rightMarquee = ::operator new(0x24);
    MiningGame_MarqueeImage_ctor(rightMarquee, 0x4ec, this->marqueeWidth,
                                 (this->progressBarX - this->marqueeWidth) + this->progressBarWidth,
                                 this->progressBarHeight + this->progressBarY + 5, 32.0f);
    this->rightMarquee = rightMarquee;

    int (*imageWidth)(void *, int) = g_MiningGame_imageWidth;
    this->oreIconOffsetX = imageWidth(*canvasHolder, this->oreIconImageId) / 2 + 5;
    this->oreIconOffsetY = imageWidth(*canvasHolder, this->field_0x9c) / 2;
    this->oreImageHeight = ((PaintCanvas *)*canvasHolder)->GetImage2DHeight(this->field_0xa0);

    void *oreMarquee = ::operator new(0x24);
    MiningGame_MarqueeImage_ctor(oreMarquee, 0x4e4, imageWidth(*canvasHolder, this->field_0x9c) - 8, 0, 0,
                                 F(layout, 0xdc));
    this->oreMarquee = oreMarquee;
    MiningGame_MarqueeImage_setSpeed(oreMarquee, F(layout, 0xe0) * g_MiningGame_layerSpeed[this->currentLayer]);

    this->promptPulseTimer = 0;
    this->textAlpha = 1.0f;
    this->campaignFlag = MiningGame_Status_getCurrentCampaignMission(*statusHolder) > 4;
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
    ((PaintCanvas *)canvas)->SetColor((unsigned int)-1);

    int *layerTable = g_MiningGame_layerTableRender;
    void **layoutHolder = g_MiningGame_layoutRender;
    void *layout = *layoutHolder;
    void (*drawLayer)(void *, int, int, int, int, int, int, int, int) = g_MiningGame_drawLayer;

    for (int layerIndex = this->currentLayer; layerIndex < this->targetLayer; layerIndex++) {
        int raw = layerTable[(layerIndex - this->targetLayer * 7) + 0x31];
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
        drawLayer(canvas, image, this->centerX, this->centerY, half, half, 0x11, 0x22, 0);
        drawLayer(canvas, image, this->centerX, this->centerY, half, half, 0x11, 0x21, 1);
        drawLayer(canvas, image, this->centerX, this->centerY, half, half, 0x11, 0x12, 2);
        drawLayer(canvas, image, this->centerX, this->centerY, half, half, 0x11, 0x11, 3);
    }

    if (this->isCoreLayer != 0) {
        ((PaintCanvas *)canvas)->DrawImage2D(this->coreImageId, this->centerX, this->centerY, (unsigned char)0x4411);
    }

    ((PaintCanvas *)canvas)->DrawImage2D(this->oreIconImageId, (int)this->posX, (int)this->posY,
                                         (unsigned char)0x4411);
    MiningGame_Sprite_setRefPixelPosition(this->drillSprite, (int)this->posX, (int)this->posY);
    MiningGame_Sprite_draw(this->drillSprite, 1.0f, 1.0f);

    ((PaintCanvas *)canvas)->DrawImage2D(this->field_0xa4, this->progressBarX - I(layout, 0xfc),
                                         this->progressBarY - I(layout, 0xfc));

    int lossTimer = this->lossTimer;
    if (lossTimer > 0x341) {
        float red = MiningGame_Layout_getPulseValue(layout, 10.0f) * 255.0f;
        float green = MiningGame_Layout_getPulseValue(layout, 10.0f) * 255.0f;
        ((PaintCanvas *)canvas)->SetColor((unsigned char)0xff, (unsigned char)(int)red, (unsigned char)(int)green, (unsigned char)0xff);
    }

    int width = (int)(((2500.0f - (float)lossTimer) / 2500.0f) * (float)this->progressBarWidth);
    ((PaintCanvas *)canvas)->DrawRegion2D(this->progressBarImageId, 0, 0, width, this->progressBarHeight, (float)width, 0, 0, 0,
                                          this->progressBarX);
    ((PaintCanvas *)canvas)->SetColor((unsigned int)-1);
    ((PaintCanvas *)canvas)->DrawImage2D(this->field_0xc4, this->centerX, this->progressBarY - 3, (unsigned char)0x2411);

    MiningGame_MarqueeImage_draw(this->leftMarquee);
    MiningGame_MarqueeImage_draw(this->rightMarquee);
    MiningGame_MarqueeImage_drawAt(this->oreMarquee, (int)(this->posX + (float)this->oreIconOffsetX),
                                   (int)(this->posY - (float)this->oreImageHeight));

    ((PaintCanvas *)canvas)->DrawImage2D(this->field_0x9c,
                                         (int)((this->posX + (float)this->oreIconOffsetX) - (float)I(layout, 0xfc)),
                                         (int)(this->posY - (float)I(layout, 0x100)));

    MiningGame_String_ctor_int(amountText, (int)this->oreAmount);
    MiningGame_String_ctor_char(suffixText, g_MiningGame_oreSuffix, false);
    MiningGame_String_plus(oreText, amountText, suffixText);
    MiningGame_String_dtor(suffixText);
    MiningGame_String_dtor(amountText);

    void *ship = MiningGame_Status_getShip_render(*g_MiningGame_statusRender);
    int freeSpace = MiningGame_Ship_getFreeSpace(ship);
    int alpha = (int)(this->textAlpha * 255.0f);
    if (freeSpace < (int)this->oreAmount) {
        ((PaintCanvas *)canvas)->SetColor((unsigned char)0xff, (unsigned char)0x2a, (unsigned char)0, (unsigned char)alpha);
    } else {
        ((PaintCanvas *)canvas)->SetColor((unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)alpha);
    }

    String *font = (String *)*g_MiningGame_fontString;
    int textWidth = ((PaintCanvas *)canvas)->GetTextWidth((unsigned int)(long)font, oreText);
    int textX = (int)(((this->posX + (float)this->oreIconOffsetX + (float)this->oreIconOffsetY) -
                      (float)I(layout, 0xfc)) -
                     (float)(textWidth / 2));
    int textY = (int)(this->posY + (float)I(layout, 0x104));
    ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, (void *)oreText, textX, textY, false);
    ((PaintCanvas *)canvas)->SetColor((unsigned int)-1);

    if (MiningGame_Status_getCurrentCampaignMission_render(*g_MiningGame_statusRender) < 5) {
        int promptAlpha = (int)(((float)this->promptPulseTimer / 2500.0f) * 255.0f);
        if (promptAlpha > 255) {
            promptAlpha = 255 - promptAlpha;
        }
        ((PaintCanvas *)canvas)->SetColor((unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)(uint8_t)promptAlpha);
        String *prompt = MiningGame_GameText_getText(*g_MiningGame_gameText, 0x268);
        MiningGame_String_ctor_copy(amountText, prompt, false);
        int promptWidth = ((PaintCanvas *)canvas)->GetTextWidth((unsigned int)(long)font, amountText);
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, (void *)amountText, *g_MiningGame_screenWRender / 2 - promptWidth / 2,
                                            I(layout, 0x70) + this->progressBarY, false);
        ((PaintCanvas *)canvas)->SetColor((unsigned int)-1);
        MiningGame_String_dtor(amountText);
    }

    MiningGame_String_dtor(oreText);
}

// ---- C-ABI ctor/dtor/arith wrappers (recovered shims) ----

// MiningGame::create — heap factory. PlayerEgo::approachAsteroid does
// operator_new(0xd4) followed by the constructor; this is the real face of it.
MiningGame *MiningGame::create(int quality, int seed, Hud *hud)
{
    return new MiningGame(quality, seed, hud);
}

// MiningGame_new — C-ABI factory used at the asteroid-docking call site.
extern "C" void *MiningGame_new(int quality, int seed, void *hud)
{
    return MiningGame::create(quality, seed, (Hud *)hud);
}

// MiningGame_dtor — C-ABI destructor. Runs ~MiningGame() and returns the
// storage so the caller can hand it to operator delete.
extern "C" void *MiningGame_dtor(void *mg)
{
    if (mg) ((MiningGame *)mg)->~MiningGame();
    return mg;
}

// MiningGame_idivmod — the original update() emits a single __aeabi_idivmod;
// the quotient (r0) is what the call site consumes. Plain integer divide.
extern "C" int MiningGame_idivmod(int value, int divisor)
{
    return value / divisor;
}
