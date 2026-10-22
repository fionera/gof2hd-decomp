#include "game/menu/MiningGame.h"

#include <new>

#include "engine/render/PaintCanvas.h"
#include "game/core/String.h"
#include "game/ui/Hud.h"
#include "game/ui/Layout.h"
#include "game/mission/Status.h"
#include "engine/render/MarqueeImage.h"
#include "engine/render/Sprite.h"
#include "game/core/Globals.h"
#include "game/ship/Ship.h"
#include "game/mission/Item.h"
#include "engine/core/GameText.h"
#include "engine/audio/FModSound.h"
#include "game/mission/Achievements.h"
#include "engine/core/AERandom.h"

float LAYER_SPEEDS[7] = {5.0f, 8.0f, 12.0f, 17.0f, 23.0f, 30.0f, 38.0f};
int LAYER_DIAMETERS[49] = {
    250, 210, 170, 140, 110, 80, 50,
    250, 210, 170, 140, 110, 80, 0,
    250, 200, 150, 100, 70, 0, 0,
    250, 170, 120, 80, 0, 0, 0,
    250, 160, 80, 0, 0, 0, 0,
    250, 120, 0, 0, 0, 0, 0,
    250, 0, 0, 0, 0, 0, 0,
};

static inline void MiningGame_FModSound_setParamValue(FModSound *sound, int index, int param, float value) { sound->setParamValue(index, param, value); }

static inline void MiningGame_FModSound_play(FModSound *sound, int id, AbyssEngine::AEMath::Vector *pos,
                               AbyssEngine::AEMath::Vector *vel, float pitch) { sound->play(id, pos, vel, pitch); }

static inline void MiningGame_FModSound_stop(FModSound *sound, int id) { sound->stop(id); }

static inline int MiningGame_Achievements_hasMedal(Achievements *achievements, int medal, int value) { return achievements->hasMedal(medal, value); }

static inline int MiningGame_Achievements_getValue(Achievements *achievements, int medal, int value) { return achievements->getValue(medal, value); }

static inline void MiningGame_Hud_hudEventMedal(Hud *hud, int medal, int value) { hud->hudEventMedal(medal, value); }

Ship *MiningGame_Status_getShip(Status * status);

Item *MiningGame_Ship_getFirstEquipmentOfSort(Ship *ship, int sort);

static inline int MiningGame_Item_getAttribute(Item *item, int attribute) { return item->getAttribute(attribute); }

static inline int MiningGame_Status_getCurrentCampaignMission(Status *status) { return status->getCurrentCampaignMission(); }

float MiningGame_Layout_getPulseValue(Layout *layout, float value);

Ship *MiningGame_Status_getShip_render(Status * status);

static inline int MiningGame_Ship_getFreeSpace(Ship *ship) { return ship->getFreeSpace(); }

int MiningGame_Status_getCurrentCampaignMission_render(Status * status);

static inline float &F(Layout *p, unsigned off) { return *(float *) ((char *) p + off); }

int MiningGame::getOreAmount() {
    return (int) this->oreAmount;
}

int MiningGame::getAsteroidType() {
    return this->station;
}



bool MiningGame::isInCurrentLayer() {
    float dx = this->posX - (float) this->centerX;
    float dy = this->posY - (float) this->centerY;
    Globals **sqrtHolder = &Globals::globals;
    int current = this->currentLayer;
    int layer = this->targetLayer;
    int *row = LAYER_DIAMETERS + (7 - layer) * 7;
    Layout *layout = Globals::layout;
    Globals *globals = *sqrtHolder;
    int size = row[current];
    float scale = layout->ringRadiusScale;
    float distance = globals->sqrt(dx * dx + dy * dy);
    return distance < scale * (float) (size / 2);
}

void MiningGame::up(float amount) {
    this->inputY = amount * 3.0f;
}

uint8_t MiningGame::gameWon() {
    return this->gameWonFlag;
}

void MiningGame::down(float amount) {
    this->inputY = amount * 3.0f;
}

uint8_t MiningGame::gotCore() {
    return this->gotCoreFlag;
}

void MiningGame::left(float amount) {
    this->inputX = amount * 3.0f;
}

uint8_t MiningGame::gameLost() {
    return this->gameLostFlag;
}

void MiningGame::right(float amount) {
    this->inputX = amount * 3.0f;
}

MiningGame::~MiningGame() {
    delete this->drillSprite;
    this->drillSprite = nullptr;
}


static inline int MG_nextInt(AbyssEngine::AERandom *random, int limit) { return random->nextInt(limit); }






int MiningGame::update(int delta) {
    int pulse = this->promptPulseTimer + delta;
    if (pulse >= 2000) {
        pulse = 0;
    }
    this->promptPulseTimer = pulse;

    int wasInLayer = isInCurrentLayer();
    int driftTimer = this->driftTimer + delta;
    this->driftTimer = driftTimer;
    if (driftTimer >= 2501) {
        AbyssEngine::AERandom **randomHolder = &Globals::rnd;
        int(*next)(AbyssEngine::AERandom *, int) = MG_nextInt;
        this->driftTimer = next(*randomHolder, 2000) + 500;

        int value = next(*randomHolder, 7);
        int sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        this->driftX = ((float) ((value + 5) * sign) / 10.0f) / this->controlDivisor;

        value = next(*randomHolder, 7);
        sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        this->driftY = ((float) (sign * (value + 5)) / 10.0f) / this->controlDivisor;

        if (this->isCoreLayer != 0 && this->currentLayer == this->targetLayer - 1) {
            this->driftX *= 0.3f;
            this->driftY *= 0.3f;
        }
    }

    if (this->campaignFlag == 0 && !isInCurrentLayer()) {
        this->driftX = ((float) this->centerX - this->posX) * 0.03f;
        this->driftY = ((float) this->centerY - this->posY) * 0.03f;
    }

    float frameScale = (float) delta;
    this->posX += ((this->inputX + this->driftX) / Globals::layout->driftSpeedDivisor) * frameScale;
    this->posY += ((this->inputY + this->driftY) / Globals::layout->driftSpeedDivisor) * frameScale;
    this->leftMarquee->update(delta);
    this->rightMarquee->update(delta);
    Globals::sound->setParamValue(0, 1, (LAYER_SPEEDS[this->currentLayer] - 5.0f) / 33.0f * 3.0f);

    if (!isInCurrentLayer()) {
        if (wasInLayer) {
            Globals::sound->stop(1);
            Globals::sound->play(3, nullptr, nullptr, 0.0f);
        }
        this->lossTimer += delta;
        if (this->lossTimer > 2500) {
            this->lossTimer = 2500;
            this->oreAmount = 0.0f;
            this->gameLostFlag = 1;
            Globals::status->field_124 = 0;
            return 0;
        }
        this->textAlpha = 1.0f;
    } else {
        if (!wasInLayer) {
            Globals::sound->play(1, nullptr, nullptr, 0.0f);
            Globals::sound->stop(3);
        }
        this->animAccumulator += frameScale / 1000.0f *
                                 (Globals::layout->animSpeedScale * LAYER_SPEEDS[this->currentLayer]) * 3.0f;
        if (this->animAccumulator >= 1.0f) {
            this->drillSprite->nextFrame();
            this->animAccumulator = 0.0f;
        }

        this->oreMarquee->update(delta);
        int oldOre = (int) this->oreAmount;
        this->oreAmount += this->oreRate * (0.15f + (float) (this->currentLayer + 1) / 7.0f * 2.35f) / 1000.0f *
                           frameScale;
        if ((int) this->oreAmount > oldOre)
            this->textAlpha = 0.0f;
        this->layerTimer += delta;
        this->textAlpha += frameScale / 500.0f;
        if (this->textAlpha > 1.0f)
            this->textAlpha = 1.0f;

        if (this->layerTimer > 6000) {
            this->layerTimer = 0;
            this->currentLayer++;
            if (this->currentLayer >= this->targetLayer) {
                this->gameWonFlag = 1;
                this->gotCoreFlag = this->targetLayer == 7;
                if (!Globals::achievements->hasMedal(38, 1)) {
                    Globals::status->field_124++;
                    if ((int) ((float) Globals::status->field_124 / (float) Globals::achievements->getValue(38, 1) *
                               100.0f) % 10 == 0 &&
                        (int) ((float) Globals::status->field_124 / (float) Globals::achievements->getValue(38, 1) *
                               100.0f) >= 30)
                        this->hud->hudEventMedal(38, (int) ((float) Globals::status->field_124 /
                                                            (float) Globals::achievements->getValue(38, 1) * 100.0f));
                    if (Globals::status->field_124 >= Globals::achievements->getValue(38, 1))
                        Globals::status->field_128 = 1;
                }
                return 0;
            }
            this->oreMarquee->setSpeed(Globals::layout->animSpeedScale * LAYER_SPEEDS[this->currentLayer]);
        }
    }
    return 1;
}






MiningGame::MiningGame(int layer, int station, Hud *hud) {
    this->station = station;
    this->layer = layer;
    this->hud = hud;
    this->centerX = Globals::w >> 1;
    this->centerY = Globals::layout->centerYBase + (Globals::h >> 1);
    this->marqueeWidth = Globals::layout->marqueeWidth;
    this->driftTimer = 0;
    this->gameLostFlag = 0;
    this->isCoreLayer = layer == 7;
    this->lossTimer = 0;
    this->oreAmount = 0.0f;
    this->oreRate = 0.0f;
    this->failThreshold = 2500;
    *(long long *) &this->layerTimer = 0;
    this->targetLayer = layer;
    this->gameWonFlag = 0;
    this->gotCoreFlag = 0;
    this->inputY = 0.0f;
    this->posX = (float) this->centerX;
    this->inputX = 0.0f;
    this->driftX = 0.0f;
    this->driftY = 0.0f;
    this->posY = (float) this->centerY;

    Item *drill = Globals::status->getShip()->getFirstEquipmentOfSort(19);
    if (drill != 0) {
        this->controlDivisor = 0.3f + ((float) drill->getAttribute(32) / 100.0f) * 1.5f;
        this->oreRate = (float) drill->getAttribute(33) / 100.0f;
    }

    unsigned int drillImage = (unsigned int) -1;
    Globals::Canvas->Image2DCreate(0x4e6, drillImage);
    int drillHeight = Globals::Canvas->GetImage2DHeight(drillImage);
    this->drillSprite = new Sprite(drillImage, drillHeight, drillHeight);
    this->drillSprite->defineReferencePixel(drillHeight >> 1, drillHeight >> 1);

    this->animAccumulator = 0.0f;
    Globals::Canvas->Image2DCreate(0x4e2, (unsigned int &) this->ringEvenNear);
    Globals::Canvas->Image2DCreate(0x4dd, (unsigned int &) this->ringEvenFar);
    Globals::Canvas->Image2DCreate(0x4de, (unsigned int &) this->ringEvenMid);
    Globals::Canvas->Image2DCreate(0x4e1, (unsigned int &) this->ringOddNear);
    Globals::Canvas->Image2DCreate(0x4df, (unsigned int &) this->ringOddMid);
    Globals::Canvas->Image2DCreate(0x4e0, (unsigned int &) this->ringOddFar);
    Globals::Canvas->Image2DCreate(0x4e5, (unsigned int &) this->oreLabelImageId);
    Globals::Canvas->Image2DCreate(0x4e4, (unsigned int &) this->oreTextImageId);
    Globals::Canvas->Image2DCreate(0x4e7, (unsigned int &) this->oreIconImageId);
    Globals::Canvas->Image2DCreate(0x4e3, (unsigned int &) this->cornerImageId);
    Globals::Canvas->Image2DCreate(0x4e8, (unsigned int &) this->progressBarImageId);
    Globals::Canvas->Image2DCreate(0x4ed, (unsigned int &) this->progressLabelImageId);
    if (this->isCoreLayer)
        Globals::Canvas->Image2DCreate(station == 164 ? 0x522 : 0x523, (unsigned int &) this->coreImageId);

    this->progressBarWidth = Globals::Canvas->GetImage2DWidth(this->progressBarImageId);
    this->progressBarHeight = Globals::Canvas->GetImage2DHeight(this->progressBarImageId);
    this->progressBarX = Globals::w / 2 - this->progressBarWidth / 2;
    this->progressBarY = Globals::layout->progressBarYBase;
    this->leftMarquee = new MarqueeImage(0x4eb, this->marqueeWidth, this->progressBarX,
                                         this->progressBarHeight + this->progressBarY + 5, 20.0f);
    this->rightMarquee = new MarqueeImage(0x4ec, this->marqueeWidth,
                                          (this->progressBarX - this->marqueeWidth) + this->progressBarWidth,
                                          this->progressBarHeight + this->progressBarY + 5, 32.0f);

    this->oreIconOffsetX = Globals::Canvas->GetImage2DWidth(this->oreIconImageId) / 2 + 5;
    this->oreIconOffsetY = Globals::Canvas->GetImage2DWidth(this->oreLabelImageId) >> 1;
    this->oreImageHeight = Globals::Canvas->GetImage2DHeight(this->oreTextImageId);
    float oreMarqueeSpeed = Globals::layout->field_0xdc;
    this->oreMarquee = new MarqueeImage(0x4e4, Globals::Canvas->GetImage2DWidth(this->oreLabelImageId) - 8, 0, 0,
                                        oreMarqueeSpeed);
    this->oreMarquee->setSpeed(Globals::layout->animSpeedScale * LAYER_SPEEDS[this->currentLayer]);

    this->textAlpha = 1.0f;
    this->promptPulseTimer = 0;
    this->campaignFlag = Globals::status->getCurrentCampaignMission() > 4;
}


void MiningGame::render2D() {
    Globals::Canvas->SetColor(0xffffffffu);
    for (int layer = this->currentLayer; layer < this->targetLayer; layer++) {
        int radius = (int) (Globals::layout->ringRadiusScale *
                            (float) LAYER_DIAMETERS[(7 - this->targetLayer) * 7 + layer]);
        int *image;
        if ((layer & 1) == 0) {
            image = &this->ringEvenNear;
            if (Globals::layout->ringNearMidThreshold < radius) {
                image = &this->ringEvenMid;
                if (radius < Globals::layout->ringMidFarThreshold)
                    image = &this->ringEvenFar;
            }
        } else {
            image = &this->ringOddNear;
            if (Globals::layout->ringNearMidThreshold < radius) {
                image = &this->ringOddFar;
                if (radius < Globals::layout->ringMidFarThreshold)
                    image = &this->ringOddMid;
            }
        }
        int ringImage = *image;
        int half = radius / 2;
        // The ring is drawn as four mirrored quarters around the centre.
        Globals::Canvas->DrawImage2D(ringImage, this->centerX, this->centerY, half, half, 0x11, 0x22, 0);
        Globals::Canvas->DrawImage2D(ringImage, this->centerX, this->centerY, half, half, 0x11, 0x21, 1);
        Globals::Canvas->DrawImage2D(ringImage, this->centerX, this->centerY, half, half, 0x11, 0x12, 2);
        Globals::Canvas->DrawImage2D(ringImage, this->centerX, this->centerY, half, half, 0x11, 0x11, 3);
    }

    if (this->isCoreLayer)
        Globals::Canvas->DrawImage2D(this->coreImageId, this->centerX, this->centerY, 0x11, 0x44);
    Globals::Canvas->DrawImage2D(this->oreIconImageId, (int) this->posX, (int) this->posY, 0x11, 0x44);
    this->drillSprite->setRefPixelPosition((int) this->posX, (int) this->posY);
    this->drillSprite->draw(1.0f, 1.0f);

    Globals::Canvas->DrawImage2D(this->cornerImageId, this->progressBarX - Globals::layout->hudInset,
                                 this->progressBarY - Globals::layout->hudInset);
    if (this->lossTimer >= 834) {
        // Flash the progress bar while the drill is about to break off.
        Globals::Canvas->SetColor(255, (unsigned char) (Globals::layout->getPulseValue(10.0f) * 255.0f),
                                  (unsigned char) (Globals::layout->getPulseValue(10.0f) * 255.0f), 255);
    }
    int width = (int) ((2500.0f - (float) this->lossTimer) / 2500.0f * (float) this->progressBarWidth);
    Globals::Canvas->DrawRegion2D(this->progressBarImageId, 0, 0, width, this->progressBarHeight, 0.0f, 0, 0,
                                  this->progressBarX, this->progressBarY);
    Globals::Canvas->SetColor(0xffffffffu);
    Globals::Canvas->DrawImage2D(this->progressLabelImageId, this->centerX, this->progressBarY - 3, 0x11, 0x24);

    this->leftMarquee->draw();
    this->rightMarquee->draw();
    this->oreMarquee->draw((int) (this->posX + (float) this->oreIconOffsetX),
                           (int) (this->posY - (float) this->oreImageHeight));
    Globals::Canvas->DrawImage2D(this->oreLabelImageId,
                                 (int) (this->posX + (float) this->oreIconOffsetX -
                                        (float) Globals::layout->hudInset),
                                 (int) (this->posY - (float) Globals::layout->oreLabelYOffset));

    String oreText = String((int) this->oreAmount) + String("t", false);
    if (Globals::status->getShip()->getFreeSpace() < (int) this->oreAmount)
        Globals::Canvas->SetColor(255, 42, 0, (unsigned char) (this->textAlpha * 255.0f));
    else
        Globals::Canvas->SetColor(255, 255, 255, (unsigned char) (this->textAlpha * 255.0f));
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, oreText,
                                (int) (this->posX + (float) this->oreIconOffsetX + (float) this->oreIconOffsetY -
                                       (float) Globals::layout->hudInset -
                                       (float) (Globals::Canvas->GetTextWidth(
                                               (unsigned int) (uintptr_t) Globals::font, oreText) >> 1)),
                                (int) (this->posY + (float) Globals::layout->oreTextYOffset), false);
    Globals::Canvas->SetColor(0xffffffffu);

    if (Globals::status->getCurrentCampaignMission() < 5) {
        // Tutorial prompt, pulsing in and out.
        int alpha = (int) ((float) this->promptPulseTimer / 1000.0f * 255.0f);
        if (alpha > 255)
            alpha = 255 - alpha;
        Globals::Canvas->SetColor(255, 255, 255, (unsigned char) alpha);
        String prompt(*Globals::gameText->getText(616), false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, prompt,
                                    Globals::w / 2 -
                                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, prompt) / 2,
                                    Globals::layout->field_0x70_rowHeight + this->progressBarY, false);
        Globals::Canvas->SetColor(0xffffffffu);
    }
}
