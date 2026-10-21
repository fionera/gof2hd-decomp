#include "game/ship/Player.h"
#include "game/ship/PlayerAsteroid.h"
#include "game/world/Level.h"
#include "game/weapons/AbstractGun.h"
#include "game/world/Station.h"
#include "engine/core/GameText.h"
#include "game/mission/Item.h"
#include "game/ship/Ship.h"
#include "game/ui/Layout.h"
#include "engine/render/Sprite.h"
#include "game/weapons/MineGun.h"
#include "game/weapons/BombGun.h"
#include "game/weapons/RocketGun.h"
#include "game/weapons/Radar.h"
#include "engine/render/PaintCanvas.h"

#include <cmath>

#include "game/ship/KIPlayer.h"
#include "game/core/Globals.h"
#include "game/mission/Status.h"
#include "game/world/SolarSystem.h"

class Mission;

namespace AbyssEngine {
    static PaintCanvas *gPaintCanvas = nullptr;

    static PaintCanvas **gRadarCanvasForDraw = nullptr;
    static PaintCanvas **gRadarCanvasSlot = nullptr;
    static unsigned char **gRadarLayoutSlot = nullptr;
}

using ::AbyssEngine::gPaintCanvas;
using ::AbyssEngine::gRadarCanvasForDraw;
using ::AbyssEngine::gRadarCanvasSlot;
using ::AbyssEngine::gRadarLayoutSlot;

int Radar_GetMissionState(void *mission); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

int Radar_GetMissionType(void *mission); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline int layout_i32(unsigned char *layout, unsigned off) {
    return *reinterpret_cast<int *>(layout + off);
}

static inline uint8_t station_u8(unsigned char *station, unsigned off) {
    return *reinterpret_cast<uint8_t *>(station + off);
}

int Radar::getTurretScopeWidth() {
    return this->turretScopeHalfWidth << 1;
}

Radar::Radar(Level *level) {
    this->field_0x174 = 0;
    this->field_0x178 = 0;
    this->field_0x17c = 0;
    this->field_0x180 = 0;
    this->field_0x164 = 0;
    this->field_0x168 = 0;
    this->field_0x16c = 0;
    this->field_0x170 = 0;
    this->radarPosX = 0;
    this->radarPosY = 0;
    this->radarPosZ = 0;
    this->field_0x160 = 0;

    this->lockedEnemy = nullptr;
    this->dockNavPtr = nullptr;
    this->field_0x14 = 0;
    this->enabled = 1;
    this->field_0x58 = 0;
    this->labelStrings = nullptr;
    this->lockedAsteroid = nullptr;
    this->field_0x10 = 0;
    this->field_0x18 = 0;
    this->lockedGasCloud = nullptr;
    this->field_0x3c = 0;
    this->planetDockIndex = 0;
    this->field_0x20c = 0;
    this->field_0x1b4 = 0;
    this->field_0x1b8 = 0;
    this->field_0x1bc = 0;
    this->field_0x28 = 0;
    this->field_0x2c = 0;
    this->field_0x30 = 0;
    this->players = nullptr;
    this->field_0x1c = nullptr;
    this->field_0x20 = 0;
    this->lockedStation = nullptr;
    this->field_0x144 = 0;
    this->field_0x148 = 0;
    this->field_0x14c = 0;
    this->field_0x150 = 0;
    this->field_0x134 = 0;
    this->field_0x138 = 0;
    this->field_0x13c = 0;
    this->field_0x140 = 0;
    for (int i = 0; i < 16; ++i) {
        this->pad_0x198_to_0x1a8[i] = 0;
    }
    this->field_0x54 = 0;
    this->plasmaInRange = 0;
    this->field_0x120 = 0;
    this->onScreen = 0;
    this->field_0x1a8 = 0;
    this->level = level;

    this->screenWidth = Globals::layout->field_0xac;
    this->halfScreenWidth = this->screenWidth >> 1;
    this->screenHeight = Globals::layout->field_0xa8;
    this->halfScreenHeight = this->screenHeight >> 1;
    this->originX = Globals::layout->field_0xa0;
    this->originY = Globals::layout->field_0xa4;

    Globals::Canvas->Image2DCreate(1223, this->field_0x1c4);
    this->imageWidth = Globals::Canvas->GetImage2DWidth(this->field_0x1c4);
    this->imageHeight = Globals::Canvas->GetImage2DHeight(this->field_0x1c4);
    this->imageWidthSq = this->imageWidth * this->imageWidth;
    this->imageHeightSq = this->imageHeight * this->imageHeight;
    this->centerX = Globals::w / 2;
    this->centerY = Globals::h / 2;
    this->weightX = 1.0f / (float) this->imageWidthSq;
    this->weightY = 1.0f / (float) this->imageHeightSq;

    this->labelStrings = new Array<String *>();
    ArraySetLength(4, *this->labelStrings);
    this->labelStrings->data()[0] = new String(
            (Globals::status->inAlienOrbit() ? String(*Globals::gameText->getText(415), false)
                                             : String(Globals::status->getStation()->getName(), false)) +
            (Globals::status->getStation()->getIndex() == 101
                     ? String("", false)
                     : String(" ", false) + *Globals::gameText->getText(136)));
    if (Globals::status->inAlienOrbit() && Globals::status->dlc1Won() && !Globals::status->inEmptyOrbit()) {
        this->labelStrings->data()[0]->Set((unsigned short *) (*level->getLandmarks())[0]->name);
    }
    this->labelStrings->data()[1] = new String(*Globals::gameText->getText(547), false);
    this->labelStrings->data()[2] = new String("", false);
    this->labelStrings->data()[3] = new String(*Globals::gameText->getText(545), false);

    Array<AbstractGun *> *playerGuns = level->getPlayerGuns();
    if (playerGuns != nullptr) {
        for (unsigned i = 0; i < playerGuns->size(); ++i) {
            if ((*playerGuns)[i]->isRocketGun()) {
                ((RocketGun *) (*playerGuns)[i])->setRadar(this);
            }
            if ((*playerGuns)[i]->isBombGun()) {
                ((BombGun *) (*playerGuns)[i])->setPlayer(level->getPlayer());
            }
            if ((*playerGuns)[i]->isMineGun()) {
                ((MineGun *) (*playerGuns)[i])->setPlayer(level->getPlayer());
            }
        }
    }
    Array<AbstractGun *> *enemyGuns = level->getEnemyGuns();
    if (enemyGuns != nullptr) {
        for (unsigned i = 0; i < enemyGuns->size(); ++i) {
            if ((*enemyGuns)[i]->isRocketGun()) {
                ((RocketGun *) (*enemyGuns)[i])->setRadar(this);
            }
        }
    }

    Globals::Canvas->Image2DCreate(1236, this->image_0xd4);
    Globals::Canvas->Image2DCreate(1241, this->image_0xd0);
    Globals::Canvas->Image2DCreate(1238, this->image_0xd8);
    Globals::Canvas->Image2DCreate(1239, this->image_0xdc);
    Globals::Canvas->Image2DCreate(1235, this->image_0xe4);
    Globals::Canvas->Image2DCreate(1242, this->image_0xe0);
    Globals::Canvas->Image2DCreate(1237, this->image_0xe8);
    Globals::Canvas->Image2DCreate(1240, this->image_0xec);
    this->field_0x1c8 = Globals::Canvas->GetImage2DWidth(this->image_0xd4);
    this->field_0x1cc = Globals::Canvas->GetImage2DHeight(this->image_0xd4);
    Globals::Canvas->Image2DCreate(1108, this->image_0xc8);
    Globals::Canvas->Image2DCreate(1109, this->image_0xc4);
    Globals::Canvas->Image2DCreate(1244, this->image_0x74);
    Globals::Canvas->Image2DCreate(1227, this->image_0x78);
    Globals::Canvas->Image2DCreate(1225, this->image_0x98);
    Globals::Canvas->Image2DCreate(1243, this->image_0x5c);
    Globals::Canvas->Image2DCreate(1228, this->image_0x60);
    Globals::Canvas->Image2DCreate(1224, this->image_0x90);
    Globals::Canvas->Image2DCreate(1234, this->image_0x64);
    Globals::Canvas->Image2DCreate(1229, this->image_0x68);
    Globals::Canvas->Image2DCreate(1226, this->image_0x94);
    Globals::Canvas->Image2DCreate(1264, this->image_0x6c);
    Globals::Canvas->Image2DCreate(1263, reinterpret_cast<unsigned int &>(this->hasReservation));
    Globals::Canvas->Image2DCreate(1266, this->image_0x88);
    Globals::Canvas->Image2DCreate(1265, reinterpret_cast<unsigned int &>(this->reservationDirty));
    Globals::Canvas->Image2DCreate(1224, this->image_0x7c);
    Globals::Canvas->Image2DCreate(1226, this->image_0x80);
    Globals::Canvas->Image2DCreate(1225, this->image_0x84);
    Globals::Canvas->Image2DCreate(1266, this->image_0x9c);
    Globals::Canvas->Image2DCreate(1265, this->image_0xa4);
    this->image_0xa0 = this->image_0xa4;
    Globals::Canvas->Image2DCreate(1105, this->image_0xa8);
    Globals::Canvas->Image2DCreate(1103, this->image_0xb0);
    Globals::Canvas->Image2DCreate(1100, this->image_0xac);
    Globals::Canvas->Image2DCreate(1101, this->image_0xb4);
    Globals::Canvas->Image2DCreate(1104, this->image_0xb8);
    Globals::Canvas->Image2DCreate(1107, this->image_0xbc);
    Globals::Canvas->Image2DCreate(8034, this->image_0xc0);
    Globals::Canvas->Image2DCreate(1220, this->image_0xcc);
    this->lockLabelX = Globals::w / 2 - Globals::Canvas->GetImage2DWidth(this->image_0xcc) / 2;
    this->field_0x214 = reinterpret_cast<int *>(Globals::layout->_pad_0xb0)[0];

    unsigned int *lockFrames = new unsigned int[10];
    Globals::Canvas->Image2DCreate(1185, lockFrames[0]);
    Globals::Canvas->Image2DCreate(1180, lockFrames[1]);
    Globals::Canvas->Image2DCreate(1182, lockFrames[3]);
    Globals::Canvas->Image2DCreate(1183, lockFrames[2]);
    Globals::Canvas->Image2DCreate(1184, lockFrames[8]);
    Globals::Canvas->Image2DCreate(1181, lockFrames[9]);
    this->lockSprite = new Sprite(lockFrames, 10, Globals::Canvas->GetImage2DHeight(lockFrames[0]),
                                  Globals::Canvas->GetImage2DHeight(lockFrames[0]));

    unsigned int blipImage = 0;
    Globals::Canvas->Image2DCreate(1110, blipImage);
    int blipSize = Globals::Canvas->GetImage2DHeight(blipImage);
    this->blipSprite = new Sprite(blipImage, blipSize, blipSize);
    this->blipSprite->defineReferencePixel(blipSize / 2, blipSize / 2);

    unsigned int qualityImage = 0;
    Globals::Canvas->Image2DCreate(1102, qualityImage);
    this->qualitySprite = new Sprite(qualityImage, reinterpret_cast<int *>(Globals::layout->_pad_0xb0)[2],
                                    reinterpret_cast<int *>(Globals::layout->_pad_0xb0)[1]);

    Item *scanner = Globals::status->getShip()->getFirstEquipmentOfSort(19);
    Item *radarItem = Globals::status->getShip()->getFirstEquipmentOfSort(17);
    Item *tractor = Globals::status->getShip()->getFirstEquipmentOfSort(13);
    this->hasScanner19 = scanner != nullptr;
    if (radarItem != nullptr) {
        this->scannerAvailable = 1;
        this->field_0x1a9 = radarItem->getAttribute(31) == 1;
        this->field_0x1aa = radarItem->getAttribute(30) == 1;
        this->field_0x1b8 = radarItem->getAttribute(29);
    } else {
        this->scannerAvailable = 0;
        this->field_0x1a9 = 0;
        this->field_0x1aa = 0;
        this->field_0x1b8 = 8000;
    }
    if (tractor != nullptr) {
        this->hasTractor13 = 1;
        this->tractorAuto = tractor->getAttribute(23) == 1;
        this->field_0x1b4 = tractor->getAttribute(24);
        this->tractorMode2 = tractor->getAttribute(23) == 2;
    } else {
        this->field_0x1b4 = 0;
        this->hasTractor13 = 0;
        this->tractorAuto = 0;
        this->tractorMode2 = 0;
    }
    this->field_0x1bc = 0;
    this->field_0x1b0 = 1;
    this->enabled = 1;

    int *lockSlots = new int[5];
    this->field_0x58 = (int) (intptr_t) lockSlots;
    for (int i = 0; i != 5; ++i) {
        lockSlots[i] = -1;
    }
    this->field_0x124 = Globals::w / 16;
    this->field_0x128 = Globals::w / 6;
    this->turretScopeHalfWidth = Globals::w / 8;
    if (Globals::status->getShip()->getFirstEquipmentOfSort(35) != nullptr &&
        Globals::status->getShip()->getFirstEquipmentOfSort(35)->getAttribute(50) != 0) {
        this->turretScopeHalfWidth = (int) (
                (float) Globals::status->getShip()->getFirstEquipmentOfSort(35)->getAttribute(50) / 100.0f *
                (float) this->turretScopeHalfWidth);
    }
    Item *autoTurret = Globals::status->getShip()->getFirstEquipmentOfSort(37);
    if (autoTurret != nullptr) {
        this->players = new Array<KIPlayer *>();
        ArraySetLength(autoTurret->getAttribute(55), *this->players);
    }
}

Radar::~Radar() {
    Array<KIPlayer *> *players = this->players;
    if (players != nullptr) {
        delete players;
    }
    this->players = nullptr;
}

uint8_t Radar::hasScanner() {
    return this->scannerAvailable;
}

uint8_t Radar::isPlasmaInRange() {
    return this->plasmaInRange;
}

bool Radar::stationLocked() {
    unsigned char *station = reinterpret_cast<unsigned char *>(this->lockedStation);
    if (station != nullptr) {
        return station_u8(station, 0x71) != 0;
    }
    return false;
}

KIPlayer *Radar::getLockedEnemy() {
    return this->lockedEnemy;
}

KIPlayer *Radar::getLockedAsteroid() {
    return this->lockedAsteroid;
}

KIPlayer *Radar::getLockedGasCloud() {
    return this->lockedGasCloud;
}

void Radar::unlockAsteroid() {
    this->lockedAsteroid = nullptr;
}

int Radar::getPlanetDockIndex() {
    SolarSystem *sys = reinterpret_cast<SolarSystem *>(
        static_cast<intptr_t>(Globals::status->getSystem()));
    Array<int> *stations = reinterpret_cast<Array<int> *>(sys->getStations());
    return (*stations)[this->planetDockIndex];
}

void Radar::update(KIPlayer *player) {
    Vector position = player->getPosition();
    update(position);
}

void Radar::update(Vector value) {
    Vector *current = reinterpret_cast<Vector *>(&this->radarPosX);
    *current = AbyssEngine::AEMath::MatrixTransformVector(this->transform, value);

    this->radarPosY = -this->radarPosY;
    this->radarPosZ = -this->radarPosZ;

    int visible = Globals::Canvas->GetScreenPosition(value, value);
    this->onScreen = static_cast<uint8_t>(visible);
    this->screenX = static_cast<int>(value.x);
    this->screenY = static_cast<int>(value.y);

    if (visible == 0) {
        *current = this->elipsoidIntersect(static_cast<int>(value.x), static_cast<int>(value.y), *current);
        this->screenX = static_cast<int>(current->x);
        this->screenY = static_cast<int>(current->y);
    }
}

long long Radar::draw(Player *, Hud *, int mode) {
    if (this->enabled == 0) {
        return 0;
    }

    this->plasmaInRange = 0;

    PaintCanvas *canvas = *gRadarCanvasForDraw;
    canvas->SetColor(static_cast<unsigned int>(-1));

    Mission *mission = Globals::status->getMission();
    int missionState = Radar_GetMissionState(mission);
    if (missionState == 0 && Radar_GetMissionType(mission) == 0) {
        (void) mode;
    }

    return 0;
}

Vector Radar::elipsoidIntersect(int x, int y, Vector value) {
    int centerY = this->centerY;
    float dy = static_cast<float>(centerY - y);
    float dy2 = dy * dy;
    int centerX = this->centerX;
    float dx = static_cast<float>(centerX - x);
    float dx2 = dx * dx;
    float weightY = this->weightY;
    float weightX = this->weightX;
    float distance = weightY * dy2 + weightX * dx2;

    if (distance >= 0.0f) {
        float scale = (distance - Globals::globals->sqrt(distance)) / distance;
        if (scale >= 0.0f && scale <= 1.0f) {
            value.x = static_cast<float>(static_cast<int>(static_cast<float>(x) + scale * dx));
            value.y = static_cast<float>(static_cast<int>(static_cast<float>(y) + scale * dy));
        }
    }

    return value;
}

static inline int layoutField(int offset) {
    return reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(Globals::layout) + offset)[0];
}

void Radar::drawCurrentLock(Hud *) {
    if (this->enabled == 0) {
        return;
    }
    Radar::drawTarget = 1;

    if (this->field_0x14 != 0) {
        Globals::Canvas->DrawImage2D(this->image_0xcc, this->lockLabelX, this->field_0x214);
        String planet(*(*(Array<String *> *) (intptr_t) Globals::status->getPlanetNames())[this->planetDockIndex], false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, planet,
                                    (Globals::w >> 1) -
                                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, planet) / 2,
                                    this->field_0x214 + layoutField(0xbc), false);
        return;
    }

    KIPlayer *target = this->lockedAsteroid;
    if (target == nullptr) {
        target = this->lockedGasCloud;
        if (target == nullptr) {
            target = this->lockedEnemy;
            if (target == nullptr) {
                target = this->lockedStation;
                if (target == nullptr) {
                    Radar::drawTarget = 0;
                    return;
                }
            }
        }
    }

    Globals::Canvas->DrawImage2D(this->image_0xcc, this->lockLabelX, this->field_0x214);
    KIPlayer *asteroid = this->lockedAsteroid;
    KIPlayer *station = this->lockedStation;
    Array<KIPlayer *> *landmarks = (Array<KIPlayer *> *) (intptr_t) this->field_0x138;
    String *titleText;
    if (station == (*landmarks)[0]) {
        titleText = (*this->labelStrings)[0];
    } else {
        titleText = station == (*landmarks)[3] ? (*this->labelStrings)[3] : (*this->labelStrings)[1];
    }
    String title(*titleText, false);

    if (target == this->lockedGasCloud) {
        String text(*Globals::gameText->getText(3236), false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text,
                                    (Globals::w >> 1) -
                                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) / 2,
                                    this->field_0x214 + layoutField(0xc4), false);
        return;
    }
    if (target == asteroid) {
        String text(*Globals::gameText->getText(((PlayerAsteroid *) target)->asteroidIndex + 1274), false);
        int textWidth = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text);
        this->qualitySprite->setFrame(((PlayerAsteroid *) target)->getQualityFrameIndex());
        int x = (Globals::w >> 1) - textWidth / 2;
        this->qualitySprite->setPosition(x - Globals::layout->buttonInsetX - this->qualitySprite->getFrameWidth(),
                                         this->field_0x214 + layoutField(0xc0));
        this->qualitySprite->draw(1.0f, 1.0f);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, x,
                                    this->field_0x214 + layoutField(0xc4), false);
        return;
    }

    String text;
    if (target->name.length != 0 &&
        (target->name.Compare(*Globals::gameText->getText(1611)) == 0 ||
         target->name.Compare(*Globals::gameText->getText(1663)) == 0 || target->field_0x42)) {
        Globals::Canvas->SetColor(0xff2a00ff);
        text = target->name;
        if (target->field_0x42) {
            text += String(String(" ", false) + String(target->player->getDamageRate()) + String("%", false), false);
        }
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text,
                                    (Globals::w >> 1) -
                                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) / 2,
                                    this->field_0x214 + layoutField(0xc4), false);
        Globals::Canvas->SetColor(0xffffffff);
        return;
    }

    int x;
    if (target == station) {
        text = title;
    } else if (target->name.length == 0) {
        text = *Globals::gameText->getText(target->shipGroup + 406);
    } else {
        text = target->name;
    }
    if (target != station) {
        text += String(String(" ", false) + String(target->player->getDamageRate()) + String("%", false), false);
        x = (Globals::w >> 1) - Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) / 2;
    } else {
        if (Globals::status->inAlienOrbit()) {
            if (Globals::status->dlc1Won()) {
                this->lockSprite->setFrame(8);
            } else {
                this->lockSprite->setFrame(9);
            }
        } else {
            this->lockSprite->setFrame(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace());
        }
        int textWidth = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text);
        x = (Globals::w >> 1) - textWidth / 2;
        this->lockSprite->setPosition(x - Globals::layout->buttonInsetX - this->lockSprite->getFrameWidth(),
                                      this->field_0x214 + layoutField(0xc8));
        this->lockSprite->draw(1.0f, 1.0f);
    }
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, x,
                                this->field_0x214 + layoutField(0xc4), false);
    if (target != station && target->shipGroup != 10) {
        this->lockSprite->setFrame(target->shipGroup);
        this->lockSprite->setPosition(x - Globals::layout->buttonInsetX - this->lockSprite->getFrameWidth(),
                                      this->field_0x214 + layoutField(0xc8));
        this->lockSprite->draw(1.0f, 1.0f);
    }
}

String Radar::calcDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    long long dx = static_cast<long long>(x1 * 0.5f - x2 * 0.5f);
    long long dz = static_cast<long long>(z1 * 0.5f - z2 * 0.5f);
    long long dy = static_cast<long long>(y1 * 0.5f - y2 * 0.5f);
    long long squared = dy * dy + dx * dx + dz * dz;
    int distance = static_cast<int>(Globals::globals->sqrt(static_cast<float>(squared) * 0.000244140625f));
    int meters = distance << 3;

    String text = String(meters) + String("m", false);
    if (distance >= 125) {
        int remainder = meters % 1000;
        if (remainder >= 100) {
            text = String(remainder);
        } else {
            text = String("0", false);
        }
        text = text.SubString(0, 1);
        text = String((int) ((unsigned int) distance / 125u)) + String(".", false) + text + String("km", false);
    }
    return text;
}

unsigned char Radar::drawTarget;
