#include "engine/render/AEGeometry.h"
#include "game/ship/Agent.h"
#include "game/mission/Mission.h"
#include "engine/audio/FModSound.h"
#include "game/ui/Hud.h"
#include "game/ship/PlayerGasCloud.h"
#include "game/ship/PlayerEgo.h"
#include "game/world/StarSystem.h"
#include "game/world/Route.h"
#include "game/world/Waypoint.h"
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



static inline int layout_i32(unsigned char *layout, unsigned off) {
    return *reinterpret_cast<int *>(layout + off);
}

static inline uint8_t station_u8(unsigned char *station, unsigned off) {
    return *reinterpret_cast<uint8_t *>(station + off);
}


static const int kRaceMusic[4] = {134, 139, 138, 137};

static inline unsigned int radarFont() {
    return (unsigned int) (uintptr_t) Globals::font;
}


static inline int layoutField(int offset) {
    return reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(Globals::layout) + offset)[0];
}


unsigned char Radar::drawTarget;

Radar::Radar(Level *level) : radarPos(), playerPos(), egoPos(), targetPos() {

    this->lockedEnemy = nullptr;
    this->dockNavPtr = nullptr;
    this->field_0x14 = 0;
    this->field_0x1c = nullptr;
    this->field_0x20 = 0;
    this->lockedStation = nullptr;
    this->field_0x28 = 0;
    this->field_0x2c = 0;
    this->field_0x30 = 0;
    this->players = nullptr;
    this->enabled = 1;
    this->asteroidSlots = nullptr;
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
    this->gasClouds = nullptr;
    this->gasLockTimer = 0;
    this->field_0x14c = 0;
    this->route = nullptr;
    this->enemies = nullptr;
    this->landmarks = nullptr;
    this->asteroids = nullptr;
    this->planetTargets = nullptr;
    this->enemyLockTimer = 0;
    this->asteroidLockTimer = 0;
    this->planetLockTimer = 0;
    this->stationLockTimer = 0;
    this->plasmaInRange = 0;
    this->field_0x54 = 0;
    this->field_0x120 = 0;
    this->onScreen = 0;
    this->field_0x11d = 0;
    this->field_0x11e = 0;
    this->field_0x11f = 0;
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
    this->centerX = Globals::w / 2;
    this->centerY = Globals::h / 2;
    this->imageWidthSq = this->imageWidth * this->imageWidth;
    this->imageHeightSq = this->imageHeight * this->imageHeight;
    this->weightX = 1.0f / (float) this->imageWidthSq;
    this->weightY = 1.0f / (float) this->imageHeightSq;

    this->labelStrings = new Array<String *>();
    ArraySetLength(4, *this->labelStrings);
    {
        this->labelStrings->data()[0] = new String(
                (Globals::status->inAlienOrbit() ? String(*Globals::gameText->getText(415), false)
                                                 : String(Globals::status->getStation()->getName(), false)) +
                (Globals::status->getStation()->getIndex() == 101
                         ? String("", false)
                         : String(" ", false) + *Globals::gameText->getText(136)));
    }
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
    this->field_0x214 = Globals::layout->field_0xb0;

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
    this->qualitySprite = new Sprite(qualityImage, Globals::layout->field_0xb8,
                                    Globals::layout->field_0xb4);

    int tractorMode = 0;
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
        tractorMode = tractor->getAttribute(23) == 2;
    } else {
        this->field_0x1b4 = 0;
        this->hasTractor13 = 0;
        this->tractorAuto = 0;
    }
    this->field_0x1bc = 0;
    this->tractorMode2 = tractorMode;
    this->field_0x1b0 = 1;
    this->enabled = 1;

    this->asteroidSlots = new int[5];
    for (int i = 0; i != 5; ++i) {
        this->asteroidSlots[i] = -1;
    }
    this->field_0x124 = Globals::w / 16;
    this->field_0x128 = Globals::w / 6;
    this->turretScopeHalfWidth = Globals::w / 8;
    Item *turret = Globals::status->getShip()->getFirstEquipmentOfSort(35);
    if (turret != nullptr && turret->getAttribute(50) != 0) {
        this->turretScopeHalfWidth = (int) (
                (float) Globals::status->getShip()->getFirstEquipmentOfSort(35)->getAttribute(50) / 100.0f *
                (float) this->turretScopeHalfWidth);
    }
    Item *autoTurret = Globals::status->getShip()->getFirstEquipmentOfSort(37);
    if (autoTurret != nullptr) {
        this->players = new Array<KIPlayer *>();
        Array<KIPlayer *> *players = this->players;
        (void) players;
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
void Radar::update(KIPlayer *player) {
    Vector position = player->getPosition();
    update(position);
}
void Radar::update(Vector value) {
    Vector *current = &this->radarPos;
    *current = AbyssEngine::AEMath::MatrixTransformVector(this->transform, value);

    this->radarPos.y = -this->radarPos.y;
    this->radarPos.z = -this->radarPos.z;

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
Vector Radar::elipsoidIntersect(int x, int y, Vector value) {
    float dx = static_cast<float>(this->centerX - x);
    float dy = static_cast<float>(this->centerY - y);
    float distance = this->weightX * (dx * dx) + this->weightY * (dy * dy);

    if (distance >= 0.0f) {
        float scale = (distance - Globals::globals->sqrt(distance)) / distance;
        if (scale >= 0.0f && scale <= 1.0f) {
            value.x = static_cast<float>(static_cast<int>(static_cast<float>(x) + scale * dx));
            value.y = static_cast<float>(static_cast<int>(static_cast<float>(y) + scale * dy));
        }
    }

    return value;
}
void Radar::draw(Player *player, Hud *hud, int mode) {
    if (!this->enabled) {
        return;
    }
    this->plasmaInRange = 0;
    this->lockConfirmed = 0;
    Globals::Canvas->SetColor(0xffffffff);

    Mission *mission = Globals::status->getMission();
    bool missionBlocksPlanets = false;
    int alwaysEnemySeen = false;
    if (!mission->isEmpty() && mission->getType() != 11 && mission->getType() != 0 && mission->getType() != 13 &&
        mission->getType() != 171) {
        missionBlocksPlanets = mission->getType() != 172;
    }

    bool turretMode = this->level->getPlayer()->isInTurretMode();
    bool alienOrbit = Globals::status->inAlienOrbit();
    int campaign = Globals::status->getCurrentCampaignMission();
    this->enemies = this->level->getEnemies();
    this->landmarks = this->level->getLandmarks();
    this->route = this->level->getPlayerRoute();
    this->asteroids = this->level->getAsteroids();
    this->gasClouds = this->level->getGasClouds();
    this->planetTargets = (Array<KIPlayer *> *) this->level->getStarSystem()->getPlanetTargets();

    this->transform = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
    this->cameraX = this->transform.m[3];
    this->cameraY = this->transform.m[7];
    this->cameraZ = this->transform.m[11];
    this->transform = AbyssEngine::AEMath::MatrixGetInverse(this->transform);

    if (alienOrbit) {
        this->field_0x1a8 =
                this->level->getPlayer()->goingToPlanet() && !this->level->getPlayer()->isDockingToPlanet();
    } else {
        if (this->level->getPlayer()->goingToPlanet() && !this->level->getPlayer()->isDockingToPlanet()) {
            int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
            Array<int> *targets = (Array<int> *) this->level->getStarSystem()->getPlanetTargets();
            SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
            int stationIndex = system->getStationEnumIndex(
                    ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())
                            ->data()[this->planetDockIndex]);
            this->field_0x1a8 = autoPilotTarget == targets->data()[stationIndex];
        } else {
            this->field_0x1a8 = false;
        }
    }

    Globals::Canvas->DrawImage2D(this->field_0x1c4, this->centerX - this->imageWidth, this->centerY - this->imageHeight);
    Globals::Canvas->DrawImage2D(this->field_0x1c4, this->centerX, this->centerY - this->imageHeight, 1);
    Globals::Canvas->DrawImage2D(this->field_0x1c4, this->centerX - this->imageWidth, this->centerY, 2);
    Globals::Canvas->DrawImage2D(this->field_0x1c4, this->centerX, this->centerY, 3);

    if (this->route != nullptr) {
        Waypoint *waypoint = this->route->getWaypoint();
        if (waypoint != nullptr) {
            this->update(waypoint);
            this->lockLabel = this->calcDistance((float) waypoint->x, (float) waypoint->y, (float) waypoint->z,
                                                 this->cameraX, this->cameraY, this->cameraZ);
            if (this->onScreen) {
                Globals::Canvas->DrawImage2D(this->image_0x6c, this->screenX, this->screenY, 17, 68);
                Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX - this->halfScreenHeight,
                                            this->screenY + this->halfScreenWidth, false);
            } else if (this->scannerAvailable) {
                Globals::Canvas->DrawImage2D(reinterpret_cast<unsigned int &>(this->hasReservation), this->screenX,
                                             this->screenY, 17, 68);
            }
        }
    }

    int otherLock;
    if (this->lockedAsteroid == nullptr && this->lockedGasCloud == nullptr && this->field_0x1c == nullptr) {
        otherLock = this->field_0x14 != 0;
    } else {
        otherLock = 1;
    }
    bool landmarkHit = false;
    int notOtherLock = !otherLock;
    int notTurretMode = !turretMode;
    if (this->landmarks != nullptr &&
        (Globals::status->getStation()->getIndex() != 111 || Globals::status->getCurrentCampaignMission() <= 93) &&
        Globals::status->getStation()->getIndex() != 110 && Globals::status->getStation()->getIndex() != 109) {
        Array<KIPlayer *> *landmarks = this->landmarks;
        for (int i = 0; i < landmarks->size(); ++i) {
            if (i == 2 || this->landmarks->data()[i] == nullptr || !this->landmarks->data()[i]->isVisible()) {
                continue;
            }
            this->update(this->landmarks->data()[i]);
            if (this->onScreen && this->screenX > this->centerX - this->field_0x128 &&
                this->screenX < this->centerX + this->field_0x128 &&
                this->screenY > this->centerY - this->field_0x128 &&
                this->screenY < this->centerY + this->field_0x128) {
                if (!landmarkHit && i != 3 && notTurretMode && notOtherLock) {
                    int size = this->field_0x124;
                    int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
                    int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
                    if (this->screenX > left && this->screenY < top + size * 2 && this->screenY > top &&
                        this->screenX < left + size * 2) {
                        if (this->field_0x28 != this->landmarks->data()[i]) {
                            this->stationLockTimer = 0;
                        }
                        this->field_0x28 = this->landmarks->data()[i];
                        landmarkHit = true;
                    }
                }
                this->egoPos = player->getPosition();
                this->targetPos = this->landmarks->data()[i]->player->getPosition();
                if (i != 3) {
                    Globals::Canvas->DrawImage2D(this->image_0x88, this->screenX, this->screenY, 17, 68);
                }
                int offset = i == 0 ? this->originX : this->originY;
                Globals::Canvas->DrawString(radarFont(), *this->labelStrings->data()[i], this->screenX + offset,
                                            this->screenY, false);
                if (i <= 1) {
                    if (i == 0 && !alienOrbit) {
                        Globals::Canvas->DrawString(
                                radarFont(),
                                *Globals::gameText->getText(133) + String(": ") +
                                        String(String(Globals::status->getStation()->getTecLevel()), false),
                                this->screenX + this->originX, this->screenY + Globals::layout->field_0x4, false);
                        KIPlayer *landmark = this->landmarks->data()[i];
                        this->lockLabel = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                                             this->cameraX, this->cameraY,
                                                             this->cameraZ);
                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + this->originX,
                                                    this->screenY + Globals::layout->field_0x4 * 2, false);
                    } else {
                        KIPlayer *landmark = this->landmarks->data()[i];
                        this->lockLabel = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                                             this->cameraX, this->cameraY,
                                                             this->cameraZ);
                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                                                    this->screenY + Globals::layout->field_0x4, false);
                    }
                }
            } else if (i == 1 || i == 3) {
                Globals::Canvas->DrawImage2D(i == 1 ? this->image_0xbc : this->image_0xb8,
                                             this->screenX, this->screenY, 17, 68);
            }
        }
        if (!turretMode) {
            if (this->lockedStation != nullptr) {
                if (this->lockedStation->isDead()) {
                    this->lockedStation = nullptr;
                }
                if (this->lockedStation == this->field_0x28) {
                    this->lockConfirmed = 1;
                }
                this->lockedStation = nullptr;
            }
            if (landmarkHit && !this->level->getPlayer()->isDockingToAsteroid() &&
                !this->level->getPlayer()->isDockingToDockingPoint() && !this->level->getPlayer()->isAutoPilot()) {
                this->stationLockTimer += mode;
                if (this->stationLockTimer > this->field_0x1b8) {
                    if (!this->lockConfirmed) {
                        Globals::sound->play(26, nullptr, nullptr, 0.0f);
                    }
                    this->lockedStation = this->field_0x28;
                }
                if (this->stationLockTimer > 0) {
                    int frame;
                    if (this->field_0x28 != 0 && this->field_0x28 != this->lockedStation) {
                        frame = (int) ((float) (this->blipSprite->getRawFrameCount() - 1) *
                                       ((float) this->stationLockTimer / (float) this->field_0x1b8));
                    } else {
                        frame = this->blipSprite->getRawFrameCount() - 1;
                    }
                    this->blipSprite->setFrame(frame);
                    this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                          (int) PlayerEgo::crosshairPos.y);
                    this->blipSprite->draw(1.0f, 1.0f);
                }
                landmarkHit = true;
            } else if (this->lockedStation == nullptr) {
                this->stationLockTimer = 0;
                this->field_0x28 = 0;
            } else {
                this->stationLockTimer = 0;
            }
        }
    }

    otherLock = this->lockedAsteroid != nullptr || this->lockedGasCloud != nullptr || this->field_0x1c != nullptr ||
                this->lockedStation != nullptr;
    if (campaign >= 2 && this->planetTargets != nullptr && !alienOrbit &&
        !this->level->getPlayer()->isDockingToPlanet()) {
        Array<int> *stations = (Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations();
        bool planetHit = false;
        for (uint32_t i = 0; i < this->planetTargets->size(); ++i) {
            if (this->planetTargets->data()[i] == nullptr) {
                continue;
            }
            this->update(this->planetTargets->data()[i]);
            if (!this->onScreen) {
                continue;
            }
            int stationId = stations->data()[i];
            int currentStation = Globals::status->getStation()->getIndex();
            int warpGate = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getWarpGateEnumIndex();
            int textOffset = 10;
            if (stationId != currentStation && (int) i == warpGate) {
                Globals::Canvas->DrawImage2D(this->image_0xbc, this->screenX + 10, this->screenY - 10);
                textOffset = 24;
            }
            Mission *campaignMission = (Mission *) (intptr_t) Globals::status->getCampaignMission();
            Mission *freelance = Globals::status->getFreelanceMission();
            int marked = false;
            if (campaignMission != nullptr && !campaignMission->isEmpty() && campaignMission->isVisible() &&
                (Globals::status->getCurrentCampaignMission() < 148 ||
                 Globals::status->getCurrentCampaignMission() >= 152)) {
                bool isTarget = stations->data()[i] == campaignMission->getTargetStation();
                if (stationId != currentStation && isTarget) {
                    if (Globals::status->getCurrentCampaignMission() != 120) {
                        Globals::Canvas->DrawImage2D(this->image_0xc8, this->screenX + textOffset, this->screenY - 10);
                        marked = true;
                    }
                }
                if (Globals::status->getCurrentCampaignMission() == 116 &&
                    (uint32_t) (stations->data()[i] - 90) <= 4 &&
                    ((((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() &
                      (1 << (stations->data()[i] - 90))) == 0)) {
                    Globals::Canvas->DrawImage2D(this->image_0xc8, this->screenX + textOffset, this->screenY - 10);
                    marked = true;
                } else if (Globals::status->getCurrentCampaignMission() == 120 && stations->data()[i] == 93) {
                    Globals::Canvas->DrawImage2D(this->image_0xc8, this->screenX + textOffset,
                                                 this->screenY - 10);
                    marked = true;
                } else if (Globals::status->getCurrentCampaignMission() == 125 &&
                    Globals::status->isFreighterMissionStation(stations->data()[i]) &&
                    ((((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() &
                      (1 << Globals::status->getFreighterMissionStationBit(stations->data()[i])))) == 0) {
                    Globals::Canvas->DrawImage2D(this->image_0xc8, this->screenX + textOffset,
                                                 this->screenY - 10);
                    marked = true;
                }
            }
            if (freelance != nullptr && !freelance->isEmpty() && freelance->isVisible()) {
                bool isTarget = stations->data()[i] == (freelance->getType() == 14 ? freelance->getAgent()->getStation()
                                                                                   : freelance->getTargetStation());
                if (stationId != currentStation && isTarget) {
                    Globals::Canvas->DrawImage2D(this->image_0xc4, this->screenX + textOffset, this->screenY - 10);
                    marked = true;
                }
            }
            if (!otherLock && !planetHit && !turretMode) {
                int size = (int) i == StarSystem::orbitPlanetIndex ? this->field_0x128 : this->field_0x124;
                int left = (int) (PlayerEgo::crosshairPos.x - (float) (size >> 1));
                int top = (int) (PlayerEgo::crosshairPos.y - (float) (size >> 1));
                planetHit = false;
                if (this->screenX > left && this->screenX < left + size && this->screenY > top &&
                    this->screenY < top + size) {
                    if ((int) i == StarSystem::orbitPlanetIndex) {
                        planetHit = false;
                    } else {
                        if (this->field_0x18 != this->planetTargets->data()[i]) {
                            this->planetLockTimer = 0;
                        }
                        this->field_0x18 = this->planetTargets->data()[i];
                        this->planetDockIndex = i;
                        planetHit = true;
                    }
                    if (stationId != currentStation) {
                        Globals::Canvas->DrawString(
                                radarFont(),
                                *((Array<String *> *) (intptr_t) Globals::status->getPlanetNames())->data()[i],
                                this->screenX + textOffset + (marked ? 14 : 0), this->screenY - 10, false);
                    }
                }
            }
        }
        if (!alienOrbit) {
            int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
            SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
            int stationIndex = system->getStationEnumIndex(
                    ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())
                            ->data()[this->planetDockIndex]);
            this->field_0x1a8 = this->level->getPlayer()->goingToPlanet() &&
                                !this->level->getPlayer()->isDockingToPlanet() &&
                                autoPilotTarget ==
                                        ((Array<int> *) this->level->getStarSystem()->getPlanetTargets())
                                                ->data()[stationIndex];
        }

        if (!turretMode && !landmarkHit) {
            if (this->field_0x14 != 0) {
                if (this->field_0x14 == this->field_0x18) {
                    this->lockConfirmed = 1;
                }
                this->field_0x14 = 0;
            }
            if (planetHit && !this->level->getPlayer()->isDockingToAsteroid() &&
                !this->level->getPlayer()->isDockingToDockingPoint()) {
                this->planetLockTimer += mode;
                if (this->planetLockTimer > this->field_0x1b8) {
                    if (missionBlocksPlanets) {
                        hud->hudEvent(21, this->level->getPlayer(), 0);
                        goto enemies;
                    }
                    if (!this->lockConfirmed) {
                        Globals::sound->play(26, nullptr, nullptr, 0.0f);
                    }
                    this->field_0x14 = this->field_0x18;
                    if (this->field_0x1a8) {
                        this->level->getPlayer()->dockToPlanet();
                    }
                }
                if (!missionBlocksPlanets && this->planetLockTimer >= 1) {
                    int frame;
                    if (this->field_0x18 != 0 && this->field_0x18 != this->field_0x14) {
                        frame = (int) ((float) (this->blipSprite->getRawFrameCount() - 1) *
                                       ((float) this->planetLockTimer / (float) this->field_0x1b8));
                    } else {
                        if (this->level->getPlayer()->isDockingToPlanet()) {
                            goto enemies;
                        }
                        frame = this->blipSprite->getRawFrameCount() - 1;
                    }
                    this->blipSprite->setFrame(frame);
                    this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                          (int) PlayerEgo::crosshairPos.y);
                    this->blipSprite->draw(1.0f, 1.0f);
                }
            } else if (this->field_0x14 == 0) {
                this->planetLockTimer = 0;
                this->field_0x18 = 0;
            } else {
                this->planetLockTimer = 0;
            }
        }
    }

enemies:
    if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDying() || this->lockedEnemy->isDead())) {
        this->lockedEnemy = nullptr;
        otherLock = false;
        this->dockNavPtr = nullptr;
    }
    bool bossSeen = false;
    bool enemyHit = false;
    if (this->scannerAvailable && this->enemies != nullptr) {
        bool alwaysEnemy = false;
        for (uint32_t i = 0; i < this->enemies->size(); ++i) {
            KIPlayer *enemy = this->enemies->data()[i];
            if (!enemy->player->isActive() ||
                ((enemy->isDying() && !enemy->hasCargo) || enemy->field_0x74)) {
                if (enemy->shipGroup == 10 && enemy->player->isActive() && !enemy->isDead() && !enemy->isDying()) {
                    this->field_0x1bc++;
                    bossSeen = true;
                }
                continue;
            }
            this->update(enemy);
            bool wreck;
            if (enemy->hasCargo == 0) {
                wreck = false;
            } else if (enemy->isDead()) {
                wreck = true;
            } else {
                wreck = enemy->isDying();
            }
            enemy->field_0x75 = 0;
            if ((enemy->isDead() || enemy->isDying()) && !enemy->hasCargo) {
                continue;
            }
            KIPlayer *locked = this->lockedEnemy;
            if (!enemy->stealFlagByte && !(enemy->countsAsEnemyExcludeFlag != 0 || wreck) &&
                enemy->player->enemyFlagsLo) {
                this->field_0x1bc++;
                if (enemy->shipGroup == 10) {
                    bossSeen = true;
                }
                if (enemy->field_0x42 && enemy->player->isAlwaysEnemy()) {
                    alwaysEnemy = true;
                }
            }
            if (this->onScreen) {
                enemy->field_0x76 = 1;
                this->egoPos = player->getPosition();
                this->targetPos = enemy->player->getPosition();
                if (this->egoPos.x - this->targetPos.x >= -24000.0f && this->egoPos.x - this->targetPos.x <= 24000.0f &&
                    this->egoPos.y - this->targetPos.y >= -24000.0f && this->egoPos.y - this->targetPos.y <= 24000.0f &&
                    this->egoPos.z - this->targetPos.z >= -24000.0f && this->egoPos.z - this->targetPos.z <= 24000.0f &&
                    enemy->getType() != 16917) {
                    if (wreck) {
                        Globals::Canvas->DrawImage2D(this->image_0x9c, this->screenX, this->screenY, 17, 68);
                        enemy->field_0x76 = 1;
                    } else {
                        if (!enemy->isDead()) {
                            int cx = (int) PlayerEgo::crosshairPos.x;
                            int cy = (int) PlayerEgo::crosshairPos.y;
                            enemy->field_0x73 = this->screenX > cx - this->field_0x124 &&
                                                this->screenX < cx + this->field_0x124 &&
                                                this->screenY > cy - this->field_0x124 &&
                                                this->screenY < cy + this->field_0x124;
                        }
                        Player *target = enemy->player;
                        unsigned int frameImage;
                        unsigned int barImage;
                        if (target->enemyFlagsLo) {
                            frameImage = this->image_0xe0;
                            barImage = this->image_0xd0;
                        } else if (target->carriesFriendCargoFlag) {
                            frameImage = this->image_0xe4;
                            barImage = this->image_0xd4;
                        } else {
                            frameImage = this->image_0xe8;
                            barImage = this->image_0xd8;
                        }
                        Globals::Canvas->DrawImage2D(frameImage, this->screenX + 2 - this->halfScreenHeight,
                                                     this->screenY + this->halfScreenWidth + 2);
                        float hull = (float) enemy->player->getDamageRate() / 100.0f * (float) this->field_0x1c8;
                        this->barWidth = (int) hull;
                        Globals::Canvas->DrawRegion2D(barImage, 0, 0, (int) hull, this->field_0x1cc, 0.0f, 0, 0,
                                                      this->screenX + 3 - this->halfScreenHeight,
                                                      this->screenY + this->halfScreenWidth + 3);
                        if (enemy->player->getEmpPoints() < enemy->player->getMaxEmpPoints()) {
                            Globals::Canvas->DrawImage2D(this->image_0xec, this->screenX + 2 - this->halfScreenHeight,
                                                         this->screenY + this->halfScreenWidth + 8);
                            float emp = (float) enemy->player->getEmpDamageRate() / 100.0f * (float) this->field_0x1c8;
                            this->barWidth = (int) emp;
                            Globals::Canvas->DrawRegion2D(this->image_0xdc, 0, 0, (int) emp, this->field_0x1cc, 0.0f,
                                                          0, 0, this->screenX + 3 - this->halfScreenHeight,
                                                          this->screenY + this->halfScreenWidth + 9);
                        }
                        if (enemy == locked) {
                            unsigned int lockImage;
                            if (enemy->player->enemyFlagsLo) {
                                lockImage = this->image_0x5c;
                            } else if (enemy->player->carriesFriendCargoFlag) {
                                lockImage = this->image_0x64;
                            } else {
                                lockImage = this->image_0x74;
                            }
                            Globals::Canvas->DrawImage2D(lockImage, this->screenX, this->screenY, 17, 68);
                        }
                    }
                } else {
                    unsigned int image;
                    if (wreck) {
                        image = this->image_0xa0;
                    } else if (enemy == locked) {
                        if (enemy->player->enemyFlagsLo) {
                            image = this->image_0x7c;
                        } else if (enemy->player->carriesFriendCargoFlag) {
                            image = this->image_0x80;
                        } else {
                            image = this->image_0x84;
                        }
                    } else if (enemy->player->enemyFlagsLo) {
                        image = this->image_0x60;
                    } else if (enemy->player->carriesFriendCargoFlag) {
                        image = this->image_0x68;
                    } else {
                        image = this->image_0x78;
                    }
                    Globals::Canvas->DrawImage2D(image, this->screenX, this->screenY, 17, 68);
                    enemy->field_0x73 = 0;
                    int cx = (int) PlayerEgo::crosshairPos.x;
                    int cy = (int) PlayerEgo::crosshairPos.y;
                    bool inSight = false;
                    if (this->screenX > cx - this->field_0x124 && this->screenY > cy - this->field_0x124 &&
                        this->screenX < cx + this->field_0x124 && this->screenY < cy + this->field_0x124 &&
                        AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) {
                        inSight = true;
                    }
                    enemy->field_0x73 = inSight;
                    if (enemy == locked) {
                        this->lockLabel = this->calcDistance(this->targetPos.x, this->targetPos.y, this->targetPos.z,
                                                             this->cameraX, this->cameraY, this->cameraZ);
                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX - this->halfScreenHeight,
                                                    this->screenY + this->halfScreenWidth, false);
                    }
                }
            } else {
                enemy->field_0x73 = 0;
                enemy->field_0x76 = 0;
                if (wreck) {
                    KIPlayer *e = this->enemies->data()[i];
                    unsigned int image;
                    if (e->countsAsEnemyExcludeFlag) {
                        image = this->image_0xb0;
                    } else if (e->shipGroup == 9) {
                        image = this->image_0xb4;
                    } else {
                        image = this->image_0xa8;
                    }
                    Globals::Canvas->DrawImage2D(image, this->screenX, this->screenY, 17, 68);
                } else {
                    unsigned int image;
                    if (enemy->player->enemyFlagsLo) {
                        image = enemy == locked ? this->image_0x90 : this->image_0x60;
                    } else if (enemy->player->carriesFriendCargoFlag) {
                        image = enemy == locked ? this->image_0x94 : this->image_0x68;
                    } else {
                        image = enemy == locked ? this->image_0x98 : this->image_0x78;
                    }
                    Globals::Canvas->DrawImage2D(image, this->screenX, this->screenY, 17, 68);
                }
                goto tractor;
            }
            if (!this->level->getPlayer()->isAutoPilot() && !this->level->getPlayer()->isDockedToDockingPoint() &&
                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck) && (!enemyHit || enemy->field_0x24) &&
                (!wreck || enemy->isDead())) {
                if (this->field_0x1c == nullptr && this->tractorAuto) {
                    this->dockNavPtr = enemy;
                    this->field_0x1c = enemy;
                    enemyHit = true;
                } else {
                    int size = this->field_0x124;
                    int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
                    int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
                    if (this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
                        this->screenY < top + size * 2) {
                        enemy->field_0x75 = 1;
                        if ((wreck && this->hasTractor13) || (this->field_0x1b0 && !this->field_0x1a8)) {
                            if (this->dockNavPtr != enemy) {
                                this->enemyLockTimer = 0;
                            }
                            this->dockNavPtr = enemy;
                            enemyHit = true;
                        }
                    }
                }
            }
        tractor:
            if (this->tractorMode2 && this->field_0x1c == nullptr && wreck && enemy->isDead()) {
                this->dockNavPtr = enemy;
                this->field_0x1c = enemy;
                enemyHit = true;
            }
        }
        alwaysEnemySeen = alwaysEnemy;

        if (!this->level->getPlayer()->isDockingToAsteroid() &&
            !(this->level->getPlayer()->isDockingToDockingPoint() || turretMode || otherLock)) {
            bool navWreck = this->dockNavPtr != nullptr && this->dockNavPtr->hasCargo &&
                            (this->dockNavPtr->isDead() || this->dockNavPtr->isDying());
            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDying() || this->lockedEnemy->isDead())) {
                this->lockedEnemy = nullptr;
            }
            if (enemyHit) {
                this->enemyLockTimer += mode;
                if (this->enemyLockTimer > ((navWreck || this->dockNavPtr->field_0x24) ? this->field_0x1b4
                                                                                        : this->field_0x1b8)) {
                    if (navWreck || this->dockNavPtr->field_0x24) {
                        this->field_0x1c = this->dockNavPtr;
                        if (!this->hasTractor13) {
                            hud->hudEvent(9, this->level->getPlayer(), 0);
                            this->field_0x1c = nullptr;
                        }
                    } else if (this->lockedEnemy != this->dockNavPtr) {
                        Globals::sound->play(26, nullptr, nullptr, 0.0f);
                        this->lockedEnemy = this->dockNavPtr;
                        if (this->field_0x1a9) {
                            this->egoPos = player->getPosition();
                            this->targetPos = this->lockedEnemy->player->getPosition();
                            if (this->egoPos.x - this->targetPos.x < -24000.0f ||
                                this->egoPos.x - this->targetPos.x > 24000.0f ||
                                this->egoPos.y - this->targetPos.y < -24000.0f ||
                                this->egoPos.y - this->targetPos.y > 24000.0f ||
                                this->egoPos.z - this->targetPos.z < -24000.0f ||
                                this->egoPos.z - this->targetPos.z > 24000.0f) {
                                Array<int> *cargo = this->lockedEnemy->cargo;
                                if (cargo == nullptr) {
                                    hud->hudEvent(22, this->level->getPlayer(), 0);
                                } else {
                                    for (uint32_t c = 0; c < cargo->size(); c += 2) {
                                        if (cargo->data()[c + 1] > 0) {
                                            hud->catchCargo(cargo->data()[0], cargo->data()[1], false, false, false, true,
                                                            false);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    this->enemyLockTimer = 0;
                }
                if (this->enemyLockTimer > ((navWreck || this->dockNavPtr->field_0x24) ? 500 : 0) &&
                    this->dockNavPtr != nullptr) {
                    KIPlayer *current = (navWreck || this->dockNavPtr->field_0x24) ? this->field_0x1c : this->lockedEnemy;
                    if (this->dockNavPtr != current) {
                        Sprite *blip = this->blipSprite;
                        float frames = (float) (blip->getRawFrameCount() - 1);
                        int elapsedInt = this->enemyLockTimer;
                        float elapsed;
                        if (navWreck || this->dockNavPtr->field_0x24) {
                            elapsed = (float) (elapsedInt - 500);
                        } else {
                            elapsed = (float) elapsedInt;
                        }
                        int totalInt;
                        if (navWreck || this->dockNavPtr->field_0x24) {
                            totalInt = this->field_0x1b4 - 500;
                        } else {
                            totalInt = this->field_0x1b8;
                        }
                        float total = (float) totalInt;
                        blip->setFrame((int) (elapsed / total * frames));
                        this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                              (int) PlayerEgo::crosshairPos.y);
                        this->blipSprite->draw(1.0f, 1.0f);
                    }
                }
                enemyHit = true;
            } else {
                if (*(navWreck ? &this->field_0x1c : &this->lockedEnemy) == nullptr) {
                    this->enemyLockTimer = 0;
                    enemyHit = false;
                    this->dockNavPtr = nullptr;
                } else {
                    this->enemyLockTimer = 0;
                    enemyHit = false;
                }
            }
        }
    }

    otherLock = (this->dockNavPtr != nullptr && this->lockedEnemy == nullptr) || this->field_0x1c != nullptr ||
                this->lockedStation != nullptr || this->field_0x14 != 0;
    if (this->gasClouds != nullptr) {
        bool gasHit = false;
        for (uint32_t i = 0; i < this->gasClouds->size(); ++i) {
            PlayerGasCloud *cloud = (PlayerGasCloud *) this->gasClouds->data()[i];
            Array<AEGeometry *> *sparks = (Array<AEGeometry *> *) cloud->getSparks();
            if (sparks != nullptr && Globals::status->getShip()->getFirstEquipmentOfSort(35) != nullptr) {
                for (uint32_t s = 0; s < sparks->size(); ++s) {
                    this->update(sparks->data()[s]->getPosition());
                    bool inSight = false;
                    if (this->onScreen) {
                        int size = this->turretScopeHalfWidth;
                        int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
                        int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
                        inSight = this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
                                  this->screenY < top + size * 2;
                    }
                    if (inSight) {
                        float distance = AbyssEngine::AEMath::VectorLength(
                                sparks->data()[s]->getPosition() - this->level->getPlayer()->getPosition());
                        float range = (float) Globals::status->getShip()->getFirstEquipmentOfSort(35)->getAttribute(51);
                        if (distance <= range && cloud->isSparkAlive(s)) {
                            this->plasmaInRange = 1;
                        }
                        cloud->setSparkInSight(s, turretMode && distance <= range);
                    } else {
                        cloud->setSparkInSight(s, false);
                    }
                }
            }
            if (!cloud->player->isActive() || cloud->isDying() ||
                Globals::status->getShip()->getFirstEquipmentOfSort(33)->getAttribute(57) != 1) {
                continue;
            }
            this->update(cloud);
            cloud->field_0x75 = 0;
            if (cloud->isDead() || cloud->isDying()) {
                continue;
            }
            if (this->onScreen) {
                KIPlayer *locked = this->lockedGasCloud;
                cloud->field_0x76 = 1;
                this->egoPos = player->getPosition();
                this->targetPos = cloud->player->getPosition();
                if (this->egoPos.x - this->targetPos.x > 24000.0f || this->egoPos.x - this->targetPos.x < -24000.0f ||
                    this->egoPos.y - this->targetPos.y > 24000.0f || this->egoPos.y - this->targetPos.y < -24000.0f ||
                    this->egoPos.z - this->targetPos.z > 24000.0f || this->egoPos.z - this->targetPos.z < -24000.0f) {
                    cloud->field_0x73 = 0;
                }
                Globals::Canvas->DrawImage2D(reinterpret_cast<unsigned int &>(this->reservationDirty), this->screenX,
                                             this->screenY, 17, 68);
                cloud->field_0x73 = 0;
                if (cloud == locked) {
                    this->lockLabel = this->calcDistance(this->targetPos.x, this->targetPos.y, this->targetPos.z,
                                                         this->cameraX, this->cameraY, this->cameraZ);
                    Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX - this->halfScreenHeight,
                                                this->screenY + this->halfScreenWidth, false);
                }
                if (!(this->level->getPlayer()->isAutoPilot() || otherLock || enemyHit)) {
                    int size = this->field_0x124;
                    int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
                    int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
                    if (this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
                        this->screenY < top + size * 2) {
                        cloud->field_0x75 = 1;
                        if (this->field_0x1b0 && !this->field_0x1a8) {
                            if (this->field_0x3c != cloud) {
                                this->gasLockTimer = 0;
                            }
                            this->field_0x3c = cloud;
                            gasHit = true;
                        }
                    }
                }
            } else if (Globals::status->getShip()->getFirstEquipmentOfSort(33)->getAttribute(58) == 1) {
                cloud->field_0x76 = 0;
                cloud->field_0x73 = 0;
                Globals::Canvas->DrawImage2D(this->image_0xc0, this->screenX, this->screenY, 17, 68);
            }
        }
        if (!turretMode && !enemyHit) {
            if (this->lockedGasCloud != nullptr) {
                if (this->lockedGasCloud->isDead()) {
                    this->lockedGasCloud = nullptr;
                }
                if (this->lockedGasCloud == this->field_0x3c) {
                    this->lockConfirmed = 1;
                }
                this->lockedGasCloud = nullptr;
            }
            if (gasHit) {
                this->gasLockTimer += mode;
                KIPlayer *lockedGas = nullptr;
                if (this->gasLockTimer > this->field_0x1b8 - 200) {
                    if (!this->lockConfirmed && !Globals::sound->isPlaying(0)) {
                        Globals::sound->play(26, nullptr, nullptr, 0.0f);
                    }
                    lockedGas = this->field_0x3c;
                    this->lockedGasCloud = lockedGas;
                }
                if (this->gasLockTimer > 500) {
                    Sprite *blip = this->blipSprite;
                    KIPlayer *candidate = this->field_0x3c;
                    int last = blip->getRawFrameCount() - 1;
                    if (candidate == nullptr || candidate == lockedGas) {
                        blip->setFrame(last);
                    } else {
                        int frame = (int) ((float) (this->gasLockTimer - 500) / (float) (this->field_0x1b8 - 500) *
                                           (float) last);
                        if (frame >= this->blipSprite->getRawFrameCount() - 1) {
                            goto asteroids;
                        }
                        this->blipSprite->setFrame(frame);
                    }
                    this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                          (int) PlayerEgo::crosshairPos.y);
                    this->blipSprite->draw(1.0f, 1.0f);
                }
            } else {
                this->gasLockTimer = 0;
                this->field_0x3c = 0;
            }
        }
    }

asteroids:
    for (int i = 0; i != 5; ++i) {
        this->asteroidSlots[i] = -1;
    }
    {
        otherLock = (this->dockNavPtr != nullptr && this->lockedEnemy == nullptr) ||
                    this->field_0x1c != nullptr || this->lockedStation != nullptr || this->field_0x14 != 0;
        if (!this->level->getPlayer()->isInDockingProcedure() && this->asteroids != nullptr) {
            int inCenter = 0;
            if (this->field_0x1aa) {
                Level *level = this->level;
                inCenter = level->isInAsteroidCenterRange(level->getPlayer()->getPosition());
            }
            int slotCount = 0;
            bool asteroidHit = false;
            for (uint32_t i = 0; i < this->asteroids->size(); ++i) {
                PlayerAsteroid *asteroid = (PlayerAsteroid *) this->asteroids->data()[i];
                if ((asteroid->isDead() || asteroid->isDying()) && !asteroid->hasCargo) {
                    continue;
                }
                this->update(asteroid);
                bool wreck;
                if (asteroid->hasCargo == 0) {
                    wreck = false;
                } else if (asteroid->isDead()) {
                    wreck = true;
                } else {
                    wreck = asteroid->isDying();
                }
                if (this->onScreen) {
                    if (!(this->level->getPlayer()->isAutoPilot() || otherLock || enemyHit || asteroidHit ||
                          turretMode)) {
                        asteroidHit = false;
                        if (asteroid->isMinable() && !this->field_0x1a8) {
                            if (wreck && this->field_0x1c == nullptr && this->tractorAuto) {
                                this->dockNavPtr = asteroid;
                                this->field_0x1c = asteroid;
                                asteroidHit = true;
                            } else {
                                int size = this->field_0x124;
                                int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
                                int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
                                if (this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
                                    this->screenY < top + size * 2 && !asteroid->asteroidFlag && slotCount <= 3 &&
                                    this->field_0x1b0) {
                                    this->asteroidSlots[slotCount++] = i;
                                }
                            }
                        }
                    }
                    if (inCenter && asteroid->getQualityFrameIndex() == 0) {
                        this->qualitySprite->setFrame(0);
                        this->qualitySprite->setPosition(this->screenX, this->screenY);
                        this->qualitySprite->draw(1.0f, 1.0f);
                    }
                    if (wreck) {
                        Globals::Canvas->DrawImage2D(this->image_0x9c, this->screenX, this->screenY, 17, 68);
                    }
                } else if (wreck) {
                    Globals::Canvas->DrawImage2D(reinterpret_cast<unsigned int &>(this->reservationDirty),
                                                 this->screenX, this->screenY, 17, 68);
                    PaintCanvas *canvas = Globals::Canvas;
                    unsigned int image = this->image_0xac;
                    int x;
                    int y;
                    if (this->field_0x11d && !this->field_0x11f && !this->field_0x120) {
                        x = this->screenX + 11;
                    } else if (this->field_0x11e && !this->field_0x11f) {
                        x = this->screenX;
                        if (!this->field_0x120) {
                            x -= 11;
                        }
                    } else if (this->field_0x11e && this->field_0x11f) {
                        x = this->screenX;
                        y = this->screenY + 11;
                    } else if (this->field_0x11f) {
                        x = this->screenX;
                        y = this->screenY + 11;
                    } else {
                        x = this->screenX;
                    }
                    if (!this->field_0x11f) {
                        y = this->screenY;
                        if (this->field_0x120) {
                            y -= 11;
                        }
                    }
                    canvas->DrawImage2D(image, x, y, 17, 68);
                }
                if (this->tractorMode2 && this->field_0x1c == nullptr && wreck) {
                    this->dockNavPtr = asteroid;
                    this->field_0x1c = asteroid;
                    asteroidHit = true;
                }
            }

            int best = -1;
            int bestDistance = 999999;
            for (int k = 0; k != 5; ++k) {
                int index = this->asteroidSlots[k];
                if (index >= 0) {
                    this->radarPos = this->asteroids->data()[index]->getPosition();
                    this->playerPos = player->getPosition();
                    this->radarPos -= this->playerPos;
                    int distance = (int) AbyssEngine::AEMath::VectorLength(this->radarPos);
                    if (distance < bestDistance) {
                        best = index;
                        bestDistance = distance;
                    }
                }
            }
            if (best > -1) {
                if (this->field_0x10 != this->asteroids->data()[best]) {
                    this->asteroidLockTimer = 0;
                }
                this->field_0x10 = static_cast<PlayerAsteroid *>(this->asteroids->data()[best]);
                asteroidHit = true;
            }

            if (!turretMode && !enemyHit) {
                if (this->lockedAsteroid != nullptr) {
                    if (this->lockedAsteroid->isDead()) {
                        this->lockedAsteroid = nullptr;
                    }
                    if (this->lockedAsteroid == this->field_0x10) {
                        this->lockConfirmed = 1;
                    }
                    this->lockedAsteroid = nullptr;
                }
                KIPlayer *candidate = this->field_0x10;
                bool candidateWreck = candidate != nullptr && candidate->hasCargo &&
                                      (candidate->isDead() || candidate->isDying());
                if (!asteroidHit || this->level->getPlayer()->isDockingToAsteroid()) {
                    if (*(candidateWreck ? &this->field_0x1c : &this->lockedAsteroid) == nullptr) {
                        this->asteroidLockTimer = 0;
                        this->field_0x10 = 0;
                    } else {
                        this->asteroidLockTimer = 0;
                    }
                } else {
                    this->asteroidLockTimer += mode;
                    if (this->asteroidLockTimer > this->field_0x1b8 - 200) {
                        if (!candidateWreck) {
                            if (!this->hasScanner19) {
                                hud->hudEvent(20, this->level->getPlayer(), 0);
                            } else {
                                if (!this->lockConfirmed && !Globals::sound->isPlaying(0)) {
                                    Globals::sound->play(26, nullptr, nullptr, 0.0f);
                                }
                                this->lockedAsteroid = this->field_0x10;
                            }
                        } else {
                            if (!this->hasTractor13) {
                                hud->hudEvent(9, this->level->getPlayer(), 0);
                            } else {
                                this->field_0x1c = this->field_0x10;
                            }
                        }
                    }
                    if (this->asteroidLockTimer > 500) {
                        if (this->field_0x10 != nullptr &&
                            this->field_0x10 != *(candidateWreck ? &this->field_0x1c : &this->lockedAsteroid)) {
                            int frame = (int) ((float) (this->blipSprite->getRawFrameCount() - 1) *
                                               ((float) (this->asteroidLockTimer - 500) /
                                                (float) (this->field_0x1b8 - 500)));
                            if (frame < this->blipSprite->getRawFrameCount() - 1) {
                                this->blipSprite->setFrame(frame);
                                this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                                      (int) PlayerEgo::crosshairPos.y);
                                this->blipSprite->draw(1.0f, 1.0f);
                            }
                        } else {
                            this->blipSprite->setFrame(this->blipSprite->getRawFrameCount() - 1);
                            this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                                  (int) PlayerEgo::crosshairPos.y);
                            this->blipSprite->draw(1.0f, 1.0f);
                        }
                    }
                }
            }
        } else if (this->level->getPlayer()->isDockingToAsteroid() && this->lockedAsteroid != nullptr) {
            this->egoPos = this->lockedAsteroid->getPosition();
            this->lockLabel = this->calcDistance(this->egoPos.x, this->egoPos.y, this->egoPos.z, this->cameraX,
                                                 this->cameraY, this->cameraZ);
            Globals::Canvas->DrawString(radarFont(), this->lockLabel,
                                        (int) (PlayerEgo::crosshairPos.x - (float) this->halfScreenHeight),
                                        (int) (PlayerEgo::crosshairPos.y + (float) this->halfScreenWidth), false);
        }
    }

asteroids_done:
    int current = Globals::sound->currentMusicEvent;
    if (current == 143) {
        return;
    }
    if (this->field_0x1bc >= 1) {
        this->field_0x54 = 1;
        if ((uint32_t) (current - 136) > 15 || ((1 << (current - 136)) & 0xe071) == 0 ||
            (current != 151 && alwaysEnemySeen)) {
            Globals::sound->stop(current);
            if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens() ||
                Globals::status->getCurrentCampaignMission() == 16) {
                Globals::sound->play(136, nullptr, nullptr, 0.0f);
            } else if (this->field_0x1bc < 3) {
                if (alwaysEnemySeen) {
                    Globals::sound->play(151, nullptr, nullptr, 0.0f);
                } else if (bossSeen) {
                    Globals::sound->play(149, nullptr, nullptr, 0.0f);
                } else {
                    Globals::sound->play(140, nullptr, nullptr, 0.0f);
                }
            } else if (this->field_0x1bc < 5) {
                if (alwaysEnemySeen) {
                    Globals::sound->play(151, nullptr, nullptr, 0.0f);
                } else if (bossSeen) {
                    Globals::sound->play(149, nullptr, nullptr, 0.0f);
                } else {
                    Globals::sound->play(141, nullptr, nullptr, 0.0f);
                }
            } else {
                if (alwaysEnemySeen) {
                    Globals::sound->play(151, nullptr, nullptr, 0.0f);
                } else if (bossSeen) {
                    Globals::sound->play(150, nullptr, nullptr, 0.0f);
                } else {
                    Globals::sound->play(142, nullptr, nullptr, 0.0f);
                }
            }
        }
    } else {
        this->field_0x54 = 0;
        if (Globals::status->getCurrentCampaignMission() != 145) {
            current = Globals::sound->currentMusicEvent;
            bool ambient;
            switch (current) {
                case 127: case 128: case 129: case 130: case 134: case 137: case 138: case 139:
                case 145: case 146: case 147: case 148: case 152:
                case 2238: case 2239: case 2240: case 2241: case 2242:
                    ambient = true;
                    break;
                default:
                    ambient = false;
                    break;
            }
            if (!ambient) {
                Globals::sound->stop(current);
                if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens()) {
                    Globals::sound->play(145, nullptr, nullptr, 0.0f);
                } else if (Globals::status->getCurrentCampaignMission() == 1) {
                    Globals::sound->play(143, nullptr, nullptr, 0.0f);
                } else {
                    int race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
                    if (Globals::status->getStation()->getIndex() == 108) {
                        Globals::sound->play(146, nullptr, nullptr, 0.0f);
                    } else if (Globals::status->getStation()->getIndex() == 101) {
                        Globals::sound->play(147, nullptr, nullptr, 0.0f);
                    } else if (Globals::status->inSupernovaSystem()) {
                        if (Globals::status->getMission() != nullptr && !Globals::status->getMission()->isEmpty() &&
                            Globals::status->getMission()->getTargetStation() ==
                                    Globals::status->getStation()->getIndex()) {
                            Globals::sound->play(Globals::status->getCurrentCampaignMission() < 106 ? 2241 : 2242,
                                                 nullptr, nullptr, 0.0f);
                        } else {
                            Globals::sound->play(148, nullptr, nullptr, 0.0f);
                        }
                    } else if (Globals::status->inDeepScienceOrbit()) {
                        Globals::sound->play(152, nullptr, nullptr, 0.0f);
                    } else {
                        Globals::sound->play(kRaceMusic[race], nullptr, nullptr, 0.0f);
                    }
                }
            }
        }
    }
    this->field_0x1bc = 0;

}
String Radar::calcDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    // positions are halved first so the squared 64-bit sum stays in range
    x1 *= 0.5f;
    y1 *= 0.5f;
    z1 *= 0.5f;
    x2 *= 0.5f;
    y2 *= 0.5f;
    z2 *= 0.5f;
    long long dx = static_cast<long long>(x1 - x2);
    long long dx2 = dx * dx;
    long long dz = static_cast<long long>(z1 - z2);
    long long dz2 = dz * dz;
    long long dy = static_cast<long long>(y1 - y2);
    long long squared = dy * dy + dx2 + dz2;
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
int Radar::getPlanetDockIndex() {
    SolarSystem *sys = reinterpret_cast<SolarSystem *>(
        static_cast<intptr_t>(Globals::status->getSystem()));
    Array<int> *stations = reinterpret_cast<Array<int> *>(sys->getStations());
    return (*stations)[this->planetDockIndex];
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
        }
    }
    if (__builtin_expect(target == nullptr, 0)) { // lint: arm_builtin
        target = this->lockedStation;
        if (target == nullptr) {
            Radar::drawTarget = 0;
            return;
        }
    }
    Globals::Canvas->DrawImage2D(this->image_0xcc, this->lockLabelX, this->field_0x214);
    Array<KIPlayer *> *landmarks = this->landmarks;
    KIPlayer *asteroid = this->lockedAsteroid;
    KIPlayer *station = this->lockedStation;
    String *titleText;
    if (station == (*landmarks)[0]) {
        titleText = (*this->labelStrings)[0];
    } else {
        titleText = station == (*landmarks)[3] ? (*this->labelStrings)[3] : (*this->labelStrings)[1];
    }
    String title(*titleText, false);

    if (target == this->lockedGasCloud) {
        String text(*Globals::gameText->getText(3236), false);
        int x = (Globals::w >> 1) -
                (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, x,
                                    this->field_0x214 + layoutField(0xc4), false);
        return;
    }
    if (target == asteroid) {
        String text(*Globals::gameText->getText(((PlayerAsteroid *) target)->asteroidIndex + 1274), false);
        int x = (Globals::w >> 1) -
                (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1);
        this->qualitySprite->setFrame(((PlayerAsteroid *) target)->getQualityFrameIndex());
        this->qualitySprite->setPosition(x - Globals::layout->field_0x2c_rowHeight - this->qualitySprite->getFrameWidth(),
                                         this->field_0x214 + layoutField(0xc0));
        this->qualitySprite->draw(1.0f, 1.0f);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, x,
                                    this->field_0x214 + layoutField(0xc4), false);
        return;
    }

    String text;
    int x;
    const String *label;
    if (target->name.length != 0 &&
        (target->name.Compare(*Globals::gameText->getText(1611)) == 0 ||
         target->name.Compare(*Globals::gameText->getText(1663)) == 0 || target->field_0x42)) {
        Globals::Canvas->SetColor(0xff2a00ff);
        text = target->name;
        if (target->field_0x42) {
            text += String(String(" ", false) + String(target->player->getDamageRate()) + String("%", false), false);
        }
        x = (Globals::w >> 1) -
            (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, x,
                                    this->field_0x214 + layoutField(0xc4), false);
        Globals::Canvas->SetColor(0xffffffff);
    } else {
        if (target == station) {
            label = &title;
        } else if (target->name.length == 0) {
            label = Globals::gameText->getText(target->shipGroup + 406);
        } else {
            label = &target->name;
        }
        text = *label;
        if (target != station) {
            text += String(String(" ", false) + String(target->player->getDamageRate()) + String("%", false), false);
            x = (Globals::w >> 1) - (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1);
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
            x = (Globals::w >> 1) -
                (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1);
            this->lockSprite->setPosition(x - Globals::layout->field_0x2c_rowHeight - this->lockSprite->getFrameWidth(),
                                          this->field_0x214 + layoutField(0xc8));
            this->lockSprite->draw(1.0f, 1.0f);
        }
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, x,
                                    this->field_0x214 + layoutField(0xc4), false);
    }
    if (target != station && target->shipGroup != 10) {
        this->lockSprite->setFrame(target->shipGroup);
        this->lockSprite->setPosition(x - Globals::layout->field_0x2c_rowHeight - this->lockSprite->getFrameWidth(),
                                      this->field_0x214 + layoutField(0xc8));
        this->lockSprite->draw(1.0f, 1.0f);
    }
}
KIPlayer *Radar::getLockedGasCloud() {
    return this->lockedGasCloud;
}
KIPlayer *Radar::getLockedAsteroid() {
    return this->lockedAsteroid;
}
void Radar::unlockAsteroid() {
    this->lockedAsteroid = nullptr;
}
KIPlayer *Radar::getLockedEnemy() {
    return this->lockedEnemy;
}
bool Radar::stationLocked() {
    unsigned char *station = reinterpret_cast<unsigned char *>(this->lockedStation);
    if (station != nullptr) {
        return station_u8(station, 0x71) != 0;
    }
    return false;
}
int Radar::getTurretScopeWidth() {
    return this->turretScopeHalfWidth << 1;
}
uint8_t Radar::isPlasmaInRange() {
    return this->plasmaInRange;
}
uint8_t Radar::hasScanner() {
    return this->scannerAvailable;
}