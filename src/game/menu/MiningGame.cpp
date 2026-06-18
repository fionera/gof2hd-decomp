#include "game/menu/MiningGame.h"

#include "game/core/PaintCanvasClass.h"
#include "game/core/String.h"
#include "game/ui/Hud.h"
#include "engine/render/MarqueeImage.h"
#include "engine/render/Sprite.h"

using AbyssEngine::String;

// Engine-internal entry points reached through opaque global singletons. These
// operate on engine objects (status/ship/item/achievements/sound/layout/text)
// that have no clean header here, so they stay as thin C-ABI forwarders.
extern "C" float MiningGame_sqrt(void *globals, float value);
extern "C" void MiningGame_FModSound_setParamValue(void *sound, int index, int param, float value);
extern "C" void MiningGame_FModSound_play(void *sound, int id, void *a, void *b, void *c);
extern "C" void MiningGame_FModSound_stop(void *sound, int id);
extern "C" int MiningGame_Achievements_hasMedal(void *achievements, int medal, int value);
extern "C" int MiningGame_Achievements_getValue(void *achievements, int medal, int value);
extern "C" void MiningGame_Hud_hudEventMedal(void *hud, int medal, int value);
extern "C" void *MiningGame_Status_getShip(void *status);
extern "C" void *MiningGame_Ship_getFirstEquipmentOfSort(void *ship, int sort);
extern "C" int MiningGame_Item_getAttribute(void *item, int attribute);
extern "C" int MiningGame_Status_getCurrentCampaignMission(void *status);
extern "C" float MiningGame_Layout_getPulseValue(void *layout, float value);
extern "C" void *MiningGame_Status_getShip_render(void *status);
extern "C" int MiningGame_Ship_getFreeSpace(void *ship);
extern "C" int MiningGame_Status_getCurrentCampaignMission_render(void *status);
extern "C" String *MiningGame_GameText_getText(void *gameText, int id);

// Generic typed field accessors over engine singletons reached only as void*.
// RAWREAD: every I()/F()/UC() target here is an opaque engine singleton
// (layout/status) handed back through a void** global, not a byte-faithful typed
// object. The Layout/Status headers are sparse (no members at these offsets), so
// these stay as raw offset reads rather than invented named members.
static inline int &I(void *p, unsigned off) { return *(int *)((char *)p + off); }
static inline float &F(void *p, unsigned off) { return *(float *)((char *)p + off); }
static inline uint8_t &UC(void *p, unsigned off) { return *(uint8_t *)((char *)p + off); }

int MiningGame::getOreAmount()
{
    return (int)this->oreAmount;
}

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

void MiningGame::up(float amount)
{
    this->inputY = amount * 3.0f;
}

uint8_t MiningGame::gameWon()
{
    return this->gameWonFlag;
}

void MiningGame::down(float amount)
{
    this->inputY = amount * 3.0f;
}

uint8_t MiningGame::gotCore()
{
    return this->gotCoreFlag;
}

void MiningGame::left(float amount)
{
    this->inputX = amount * 3.0f;
}

uint8_t MiningGame::gameLost()
{
    return this->gameLostFlag;
}

void MiningGame::right(float amount)
{
    this->inputX = amount * 3.0f;
}

// PlayerEgo's pitch/yaw input handlers reach the mining game through these
// veneers. Each forwards the steering delta to the corresponding axis handler
// and reports the resulting axis input back to the caller.
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

MiningGame::~MiningGame()
{
    delete this->drillSprite;
    this->drillSprite = nullptr;
}

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

    this->leftMarquee->update(delta);
    this->rightMarquee->update(delta);

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
            this->drillSprite->nextFrame();
            this->animAccumulator = 0.0f;
        }

        this->oreMarquee->update(delta);
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
            this->oreMarquee->speed = F(layout, 0xe0) * layerSpeed[nextLayer];
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

__attribute__((visibility("hidden"))) extern void **g_MiningGame_layoutCtor;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_statusCtor;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_canvasCtor;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_screenW;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_screenH;
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
    this->failThreshold = 0x9c4;
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

    PaintCanvas *canvas = (PaintCanvas *)*g_MiningGame_canvasCtor;
    unsigned int drillImageId;
    canvas->Image2DCreate(0x4e6, drillImageId);
    imageId[0] = drillImageId;
    int imageHeight = canvas->GetImage2DHeight(imageId[0]);
    Sprite *sprite = new Sprite((uint32_t)imageId[0], imageHeight, imageHeight);
    this->drillSprite = sprite;
    sprite->defineReferencePixel(imageHeight / 2, imageHeight / 2);

    this->animAccumulator = 0.0f;
    unsigned int createdImageId;
    canvas->Image2DCreate(0x4e2, createdImageId);
    this->ringEvenNear = createdImageId;
    canvas->Image2DCreate(0x4dd, createdImageId);
    this->ringEvenFar = createdImageId;
    canvas->Image2DCreate(0x4de, createdImageId);
    this->ringEvenMid = createdImageId;
    canvas->Image2DCreate(0x4e1, createdImageId);
    this->ringOddNear = createdImageId;
    canvas->Image2DCreate(0x4df, createdImageId);
    this->ringOddMid = createdImageId;
    canvas->Image2DCreate(0x4e0, createdImageId);
    this->ringOddFar = createdImageId;
    canvas->Image2DCreate(0x4e5, createdImageId);
    this->oreLabelImageId = createdImageId;
    canvas->Image2DCreate(0x4e4, createdImageId);
    this->oreTextImageId = createdImageId;
    canvas->Image2DCreate(0x4e7, createdImageId);
    this->oreIconImageId = createdImageId;
    canvas->Image2DCreate(0x4e3, createdImageId);
    this->cornerImageId = createdImageId;
    canvas->Image2DCreate(0x4e8, createdImageId);
    this->progressBarImageId = createdImageId;
    canvas->Image2DCreate(0x4ed, createdImageId);
    this->progressLabelImageId = createdImageId;

    if (this->isCoreLayer != 0) {
        int coreImage = 0x523;
        if (station == 0xa4) {
            coreImage = 0x522;
        }
        canvas->Image2DCreate(coreImage, createdImageId);
        this->coreImageId = createdImageId;
    }

    this->progressBarWidth = canvas->GetImage2DWidth(this->progressBarImageId);
    this->progressBarHeight = canvas->GetImage2DHeight(this->progressBarImageId);
    int x = *screenW / 2 - this->progressBarWidth / 2;
    this->progressBarX = x;
    int y = I(layout, 0xd8);
    this->progressBarY = y;

    this->leftMarquee = new MarqueeImage(0x4eb, this->marqueeWidth, x, this->progressBarHeight + y + 5, 20.0f);

    this->rightMarquee = new MarqueeImage(0x4ec, this->marqueeWidth,
                                          (this->progressBarX - this->marqueeWidth) + this->progressBarWidth,
                                          this->progressBarHeight + this->progressBarY + 5, 32.0f);

    this->oreIconOffsetX = canvas->GetImage2DWidth(this->oreIconImageId) / 2 + 5;
    this->oreIconOffsetY = canvas->GetImage2DWidth(this->oreLabelImageId) / 2;
    this->oreImageHeight = canvas->GetImage2DHeight(this->oreTextImageId);

    MarqueeImage *oreMarquee =
        new MarqueeImage(0x4e4, canvas->GetImage2DWidth(this->oreLabelImageId) - 8, 0, 0, F(layout, 0xdc));
    this->oreMarquee = oreMarquee;
    oreMarquee->speed = F(layout, 0xe0) * g_MiningGame_layerSpeed[this->currentLayer];

    this->promptPulseTimer = 0;
    this->textAlpha = 1.0f;
    this->campaignFlag = MiningGame_Status_getCurrentCampaignMission(*statusHolder) > 4;
}

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

    PaintCanvas *canvas = (PaintCanvas *)*g_MiningGame_canvasRender;
    canvas->SetColor((unsigned int)-1);

    int *layerTable = g_MiningGame_layerTableRender;
    void **layoutHolder = g_MiningGame_layoutRender;
    void *layout = *layoutHolder;
    void (*drawLayer)(void *, int, int, int, int, int, int, int, int) = g_MiningGame_drawLayer;

    for (int layerIndex = this->currentLayer; layerIndex < this->targetLayer; layerIndex++) {
        int raw = layerTable[(layerIndex - this->targetLayer * 7) + 0x31];
        int radius = (int)(F(layout, 0xe8) * (float)raw);
        int *imageSlot;
        if ((layerIndex & 1) == 0) {
            imageSlot = &this->ringEvenNear;
            if (I(layout, 0xec) < radius) {
                imageSlot = &this->ringEvenMid;
                if (radius < I(layout, 0xf4)) {
                    imageSlot = &this->ringEvenFar;
                }
            }
        } else {
            imageSlot = &this->ringOddNear;
            if (I(layout, 0xec) < radius) {
                imageSlot = &this->ringOddFar;
                if (radius < I(layout, 0xf4)) {
                    imageSlot = &this->ringOddMid;
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
        canvas->DrawImage2D(this->coreImageId, this->centerX, this->centerY, (unsigned char)0x4411);
    }

    canvas->DrawImage2D(this->oreIconImageId, (int)this->posX, (int)this->posY, (unsigned char)0x4411);
    this->drillSprite->setRefPixelPosition((int)this->posX, (int)this->posY);
    this->drillSprite->draw(1.0f, 1.0f);

    canvas->DrawImage2D(this->cornerImageId, this->progressBarX - I(layout, 0xfc),
                        this->progressBarY - I(layout, 0xfc));

    int lossTimer = this->lossTimer;
    if (lossTimer > 0x341) {
        float red = MiningGame_Layout_getPulseValue(layout, 10.0f) * 255.0f;
        float green = MiningGame_Layout_getPulseValue(layout, 10.0f) * 255.0f;
        canvas->SetColor((unsigned char)0xff, (unsigned char)(int)red, (unsigned char)(int)green, (unsigned char)0xff);
    }

    int width = (int)(((2500.0f - (float)lossTimer) / 2500.0f) * (float)this->progressBarWidth);
    canvas->DrawRegion2D(this->progressBarImageId, 0, 0, width, this->progressBarHeight, (float)width, 0, 0, 0,
                         this->progressBarX);
    canvas->SetColor((unsigned int)-1);
    canvas->DrawImage2D(this->progressLabelImageId, this->centerX, this->progressBarY - 3, (unsigned char)0x2411);

    this->leftMarquee->draw();
    this->rightMarquee->draw();
    this->oreMarquee->draw((int)(this->posX + (float)this->oreIconOffsetX),
                           (int)(this->posY - (float)this->oreImageHeight));

    canvas->DrawImage2D(this->oreLabelImageId,
                        (int)((this->posX + (float)this->oreIconOffsetX) - (float)I(layout, 0xfc)),
                        (int)(this->posY - (float)I(layout, 0x100)));

    amountText->ctor_int((int)this->oreAmount);
    suffixText->ctor_char(g_MiningGame_oreSuffix, false);
    *oreText = *amountText + *suffixText;
    suffixText->dtor();
    amountText->dtor();

    void *ship = MiningGame_Status_getShip_render(*g_MiningGame_statusRender);
    int freeSpace = MiningGame_Ship_getFreeSpace(ship);
    int alpha = (int)(this->textAlpha * 255.0f);
    if (freeSpace < (int)this->oreAmount) {
        canvas->SetColor((unsigned char)0xff, (unsigned char)0x2a, (unsigned char)0, (unsigned char)alpha);
    } else {
        canvas->SetColor((unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)alpha);
    }

    String *font = (String *)*g_MiningGame_fontString;
    int textWidth = canvas->GetTextWidth((unsigned int)(long)font, *oreText);
    int textX = (int)(((this->posX + (float)this->oreIconOffsetX + (float)this->oreIconOffsetY) -
                       (float)I(layout, 0xfc)) -
                      (float)(textWidth / 2));
    int textY = (int)(this->posY + (float)I(layout, 0x104));
    canvas->DrawString((unsigned int)(long)font, *oreText, textX, textY, false);
    canvas->SetColor((unsigned int)-1);

    if (MiningGame_Status_getCurrentCampaignMission_render(*g_MiningGame_statusRender) < 5) {
        int promptAlpha = (int)(((float)this->promptPulseTimer / 2500.0f) * 255.0f);
        if (promptAlpha > 255) {
            promptAlpha = 255 - promptAlpha;
        }
        canvas->SetColor((unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)(uint8_t)promptAlpha);
        String *prompt = MiningGame_GameText_getText(*g_MiningGame_gameText, 0x268);
        amountText->ctor_copy(prompt, false);
        int promptWidth = canvas->GetTextWidth((unsigned int)(long)font, *amountText);
        canvas->DrawString((unsigned int)(long)font, *amountText,
                           *g_MiningGame_screenWRender / 2 - promptWidth / 2,
                           I(layout, 0x70) + this->progressBarY, false);
        canvas->SetColor((unsigned int)-1);
        amountText->dtor();
    }

    oreText->dtor();
}

// Heap factory. PlayerEgo::approachAsteroid constructs the instance with the
// asteroid quality/seed.
MiningGame *MiningGame::create(int quality, int seed, Hud *hud)
{
    return new MiningGame(quality, seed, hud);
}

// C-ABI factory used at the asteroid-docking call site.
extern "C" void *MiningGame_new(int quality, int seed, void *hud)
{
    return MiningGame::create(quality, seed, (Hud *)hud);
}

// C-ABI destructor: runs ~MiningGame() and returns the storage so the caller can
// hand it to operator delete.
extern "C" void *MiningGame_dtor(void *mg)
{
    if (mg) ((MiningGame *)mg)->~MiningGame();
    return mg;
}
