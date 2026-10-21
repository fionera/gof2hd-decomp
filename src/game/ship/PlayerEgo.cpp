#include "game/ship/PlayerEgo.h"
#include "game/world/Station.h"
#include "game/world/StarSystem.h"
#include "engine/render/ParticleSettings.h"
#include "engine/render/ParticleSettingsRef.h"
#include "engine/render/Camera.h"
#include "engine/render/MovingStars.h"
#include "engine/core/AERandom.h"
#include "game/ship/TargetFollowCamera.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/menu/HackingGame.h"
#include "game/mission/Item.h"
#include "game/mission/OptionsRecord.h"
#include "game/world/Level.h"
#include "game/world/LevelScript.h"
#include "game/menu/MiningGame.h"
#include "engine/render/ParticleSystemManager.h"
#include "game/ship/PlayerAsteroid.h"
#include "game/ship/PlayerWormHole.h"
#include "game/weapons/Gun.h"
#include "engine/render/Mesh.h"
#include "engine/render/Material.h"
#include "game/world/Waypoint.h"
#include "engine/render/Engine.h"

#include "game/mission/Status.h"
#include "engine/math/Transform.h"
#include "engine/math/EaseInOutMatrix.h"
#include "engine/core/ApplicationManager.h"
#include "game/mission/Explosion.h"

#include "game/ship/KIPlayer.h"

#include "game/mission/Mission.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerFixedObject.h"
#include "game/weapons/RepairBeam.h"
#include "game/world/Route.h"
#include "game/world/SpacePoint.h"
#include "game/core/String.h"
#include "game/weapons/TractorBeam.h"
#include "game/ship/Ship.h"
#include "game/core/Globals.h"
#include "game/menu/MiningHostObject.h"

namespace AbyssEngine {
    namespace AEMath {
        Matrix operator*(const Matrix &lhs, const Matrix &rhs);
    }
}

namespace {

    static const float g_PE_shakeDiv = 40000.0f;
    static const float g_PE_strafeLoadK = 0.4f;
    static const float g_PE_strafeLoadB = 0.6f;
    static const float g_PE_strafeAccelK = 0.002f;
    // Pitch ramp delta constants (down/up non-turret paths)
    // Yaw ramp delta constants (right/left non-turret paths)
}

struct PE_AsteroidMineTarget {
    char pad0[0x44];
    int miningFlag;
};
#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(PE_AsteroidMineTarget, miningFlag) == 0x44, "miningFlag");
#endif

// Docking-port capture radius per ship index (rodata 0x1fba48).
static const int kShipDockRadius[64] = {
    160, 150, 220, 310, 160, 180, 140, 170, 170, 110, 250, 205, 240, 250, 250, 250,
    170, 170, 190, 460, 170, 210, 140, 170, 160, 190, 170, 240, 220, 280, 200, 200,
    140, 160, 180, 150, 310, 390, 480, 200, 300, 300, 170, 200, 170, 370, 370, 300,
    440, 340, 170, 470, 170, 170, 170, 170, 200, 170, 250, 150, 170, 250, 270, 170,
};

static inline float PE_minf(float a, float b) { return b < a ? b : a; }
static inline float PE_absf(float v) { return v > 0.0f ? v : -v; }
static inline float PE_maxf(float a, float b) { return a > b ? a : b; }

// Blueprint unlocked by the hidden blueprint of the station (rodata 0x1fba34).
static const int kHiddenBlueprints[5] = {226, 221, 223, 225, 227};

AbyssEngine::AEMath::Vector PlayerEgo::crosshairPos;
AbyssEngine::AEMath::Vector PlayerEgo::crosshairShootPos;
AbyssEngine::AEMath::Vector PlayerEgo::vec_up;

PlayerEgo::PlayerEgo(Player *player)
    : lastCrosshairPos(), lastCrosshairShootPos(), moveDirection(), moveTarget(), dockOffsetVec(), headingVec(),
      miningUpVec(), turretOffsetVec(), emergencyVec(), strafeTargetVec(), facingVec() {
    this->boostSpeed = 5;
    this->boostTime = 5000;
    this->boostDelay = 20000;
    this->field_0x4 = nullptr;
    this->field_0xd8 = -1;
    this->turretBaseGeo = nullptr;
    this->turretGeometry = nullptr;
    this->targetFollowCamera = 0;
    this->explosion = nullptr;
    this->explosion2 = nullptr;
    this->movingStars = 0;
    this->route = 0;
    this->pitchAccumDir = 0;
    this->yawAccumDir = 0;
    this->field_0x14 = nullptr;
    this->radioRef = nullptr;
    this->repairArmorTick = 0;
    this->lastHP = 0;
    this->shakeIntensity = 0;
    this->shieldRegenTick = 0;
    this->repairHPTick = 0;
    this->field_0x108 = 0;
    this->currentSecondaryWeaponIndex = -1;
    this->yawAccumulator = 0;
    this->freeze = 0;
    this->handling = 0;
    this->agility = 0;
    this->autoPilot = 0;
    this->autoPilotTarget = 0;
    this->goingToWaypointFlag = 0;
    this->turretMode = 0;
    this->boostingFlag = 0;
    this->boostTimer = 0;
    this->field_0x146 = 0;
    this->collide = 0;
    this->field_0x145 = 0;
    this->field_0x140 = 0;
    this->turretCamera = 0xffffffff;
    this->dockCameraNode = nullptr;
    this->dockCameraLeaf = nullptr;
    this->dockCameraMid = nullptr;
    this->turretActive = false;
    this->field_0x1a1 = 0;
    this->dockingPointIndex = 0;
    this->field_0x1d4 = 0;
    this->planetDockTimer = 0;
    this->computerControlled = 0;
    this->inWormhole = 0;
    this->field_0x230 = 0;
    this->collidesWithStationFlag = 0;
    this->geometry = nullptr;
    this->shakeAccum = 0;
    this->explosionTimer = 0;
    this->field_0x308 = 0;
    this->turretRoot = nullptr;
    this->turretExtraGeo = nullptr;
    this->shieldVisible = 0;
    this->shieldGeometry = nullptr;
    this->lookPitch = 0;
    this->lookYaw = 0;
    this->cloaked = 0;
    this->chargingCloak = 0;
    this->cloak = nullptr;
    this->tractorBeam = nullptr;
    this->repairBeams = nullptr;
    this->asteroidTarget = 0;
    this->dockingState = 0;
    this->dockScaling = 0;
    this->miningSettleTimer = 0;
    this->dockStation = nullptr;
    this->miningGame = 0;
    this->dockingToStream = 0;
    this->dockedToStream = 0;
    this->dockingToPlanet = 0;
    this->hackingGame = 0;
    this->turretPitch = 0;
    this->driveCharge = 0;
    this->chargingDrive = 0;
    this->driveChargeMax = 0;
    this->cloakDischargeMax = 0;
    this->shieldRegenRate = 0;
    this->field_0x21c = 0;
    this->hud = 0;
    this->cloakCharge = 0;
    this->cloakRechargeTimer = 0;
    this->cloakChargeMax = 0;
    this->smokeSystem = -1;
    this->explosionSmoke = -1;
    this->field_0x309 = 1;
    this->visible = 1;
    this->exhaustVisible = 1;
    this->field_0x32d = 0;
    this->field_0x30a = 0;
    this->aboutToReachAutoTargetFlag = 0;
    this->maneuverType = 0;
    this->maneuverTimer = 0;
    this->rocketControlGun = 0;
    this->autoTurretEquipped = 0;
    this->autoTurretEnabled = 1;
    this->rocketBanking = 0.0f;
    this->rocketPitch = 0;
    this->rocketYaw = 0;
    this->emergencySystemTimer = 0;
    this->emergencyVal1 = 0;
    this->field_0xac = 0;
    this->dockedFlag = 0;
    this->field_0x399 = 0;
    this->field_0xb2 = 0;
    this->throttleStarted = 0;
    this->autoTurretTimer = 0;
    this->autoTurretFireTimer = 0;
    this->autoTurretTarget = nullptr;
    this->autoTurretPrevTarget = nullptr;
    this->throttle = 2000;
    this->field_0xb0 = 0;
    this->switchToStandardCam = 0;
    this->strafeAccel = 0;
    this->strafeFactor = 0.1f;
    this->field_0x384 = 0;
    this->pitchAccumD = 0;
    this->yawAccumF = 0;
    this->rotateX = 0;
    this->rotateY = 0;
    this->rotateZ = 0;
    this->field_0x2f5 = 0;
    this->rollDirection = 0;
    this->autoLevel = 0;
    this->pitchRamp = 0;
    this->yawRamp = 0;
    this->field_0x260 = 0;
    this->field_0x264 = 0;
    this->engineSoundId = -1;
    this->rollAccum = 0;
    this->field_0x26c = 0;
    this->yawAccumD = 0;
    this->field_0x274 = 0;

    this->player = player;
    player->setPlayShootSound(true, 2);
    this->field_0x108 = 1;
    this->movingStars = (int) (intptr_t) new MovingStars();
    this->lastHP = player->getCombinedHP();
    this->speed = 2.0f;
    this->thrust = 1.0f;
    this->boostDelay = Globals::status->getShip()->getBoostDelay();
    this->boostSpeed = (int) ((float) Globals::status->getShip()->getBoostSpeed() / 100.0f * 2.0f) + 2;
    this->boostTime = Globals::status->getShip()->getBoostTime();
    this->field_0x146 = this->boostDelay > 0;

    Item *booster = Globals::status->getShip()->getFirstEquipmentOfSort(14);
    if (booster != nullptr) {
        switch (booster->getIndex()) {
            case 71: this->boostSoundId = 38; break;
            case 72: this->boostSoundId = 39; break;
            case 73: this->boostSoundId = 40; break;
            case 74: this->boostSoundId = 41; break;
            case 195: this->boostSoundId = 1102; break;
        }
        Globals::globals->addSoundResourceToList(this->boostSoundId);
    }

    this->handling = Globals::status->getShip()->getHandling();
    if (Globals::status->getShip()->getIndex() == 42)
        this->field_0x1c = 1104;
    else if (Globals::status->getShip()->getIndex() == 43)
        this->field_0x1c = 1106;
    else if (Globals::status->getShip()->getIndex() == 40)
        this->field_0x1c = 1107;
    else if (this->handling >= 1.4f)
        this->field_0x1c = 45;
    else if (this->handling >= 1.15f)
        this->field_0x1c = 44;
    else if (this->handling < 0.95f)
        this->field_0x1c = 42;
    else
        this->field_0x1c = 43;
    Globals::globals->addSoundResourceToList(this->field_0x1c);

    this->agility = (float) Globals::status->getShip()->getAgility();
    this->handling = (this->handling + this->handling * (this->agility / 100.0f)) * 20.0f;
    this->hardCoreMode = Globals::status->hardCoreMode();
    this->cloakMaterial1 = 0xffffffff;
    this->shieldRegenRate = (float) Globals::status->getShip()->getMaxShieldHP()
                            / ((float) Globals::status->getShip()->getShieldRegen() / 100.0f);

    if (Globals::status->getShip()->hasCloakIntegrated())
        this->cloak = (*Globals::items)[95];
    else
        this->cloak = Globals::status->getShip()->getFirstEquipmentOfSort(21);
    this->cloakCharge = -1;
    this->cloakChargeMax = this->cloak != nullptr ? this->cloak->getAttribute(35) : 0;
    this->cloakDischargeMax = this->cloak != nullptr ? this->cloak->getAttribute(36) : 0;
    if (this->cloak != nullptr)
        Globals::globals->addSoundResourceToList(30);

    Item *jumpDrive = Globals::status->getShip()->getFirstEquipmentOfSort(18);
    if (jumpDrive != nullptr) {
        this->driveChargeMax = jumpDrive->getAttribute(37);
        Globals::globals->addSoundResourceToList(32);
        Globals::globals->addSoundResourceToList(33);
    } else {
        this->driveChargeMax = 0;
        if (Globals::status->getShip()->hasJumpDriveIntegrated())
            this->driveChargeMax = (*Globals::items)[85]->getAttribute(37);
    }
    if (Globals::status->getShip()->getFirstEquipmentOfSort(19) != nullptr) {
        Globals::globals->addSoundResourceToList(1);
        Globals::globals->addSoundResourceToList(3);
        Globals::globals->addSoundResourceToList(2);
    }
    if (Globals::status->getShip()->getFirstEquipmentOfSort(9) != nullptr)
        Globals::globals->addSoundResourceToList(25);
    if (Globals::status->getShip()->getFirstEquipmentOfSort(10) != nullptr)
        Globals::globals->addSoundResourceToList(23);
    this->volatileGoods = Globals::status->getShip()->hasVolatileGoods();
    this->freeCargoSpace = Globals::status->getShip()->getFreeSpace();
    this->field_0x39a = Globals::status->getShip()->getFirstEquipmentOfSort(43) != nullptr;

    Globals::Canvas->Image2DCreate(1216, (unsigned int &) this->dockArrowImage);
    Globals::Canvas->Image2DCreate(1230, (unsigned int &) this->radarBlipImage1);
    Globals::Canvas->Image2DCreate(8030, (unsigned int &) this->radarBlipImage2);
    Globals::Canvas->Image2DCreate(8029, (unsigned int &) this->radarBlipImage3);
    Globals::Canvas->Image2DCreate(1352, (unsigned int &) this->hpGaugeImage);
    this->freeLookMode = 0;
    this->field_0x354 = Globals::status->inBlackMarketSystem();
    this->maxBank = this->handling * 750.0f / 63.0f * 1.2f;
    this->bankFactor = 15.139356f;
    this->field_0x20 = 0;
    for (int i = 0; i < 5; i++)
        this->bankHistory[i] = 0;
    this->bankHistoryIndex = 0;
    this->bankHistoryFilled = 0;
    this->hitShakeTimer = 0;
    this->hitShake = 0;
    this->field_0x84 = 1;
    this->spacePoint = 0;
    this->engineSoundId = -1;
    this->lostMiningGameFlag = 0;
    this->field_0x378 = Globals::mouseCursorActivated == 0;

    float difficulty = reinterpret_cast<const float &>(Globals::options[44]);
    if (difficulty <= 0.0f)
        this->cloakRechargeMax = 5000;
    else if (difficulty <= 0.6f)
        this->cloakRechargeMax = 7000;
    else if (difficulty <= 1.1f)
        this->cloakRechargeMax = 9000;
    else
        this->cloakRechargeMax = 12000;

    if ((Globals::status->inSupernovaSystem() || Globals::status->inSupernovaOrbit())
        && Globals::status->getShip()->getFirstEquipmentOfSort(38) != nullptr) {
        this->engineSoundId = Globals::status->getShip()->getFirstEquipmentOfSort(38)->getIndex() == 205 ? 2261 : 2260;
        this->shieldVisible = 1;
        this->shieldGeometry = new AEGeometry(Globals::Canvas);
        AEGeometry *outer = new AEGeometry((uint16_t) 18803, Globals::Canvas, false);
        AEGeometry *inner = new AEGeometry((uint16_t) 18802, Globals::Canvas, false);
        this->shieldGeometry->addChild(outer->transform);
        delete outer;
        this->shieldGeometry->addChild(inner->transform);
        delete inner;
    }
}

PlayerEgo::~PlayerEgo() noexcept {
    if (this->player) delete (Player *) this->player;
    this->player = 0;
    if (this->field_0x4) delete (AEGeometry *) this->field_0x4;
    this->field_0x4 = 0;
    if (this->geometry) delete (AEGeometry *) this->geometry;
    this->geometry = 0;
    if (this->turretBaseGeo) delete (AEGeometry *) this->turretBaseGeo;
    this->turretBaseGeo = 0;
    if (this->turretGeometry) delete (AEGeometry *) this->turretGeometry;
    this->turretGeometry = 0;
    if (this->route) delete (Route *) this->route;
    this->route = 0;
    if (this->dockCameraNode) delete (AEGeometry *) this->dockCameraNode;
    this->dockCameraNode = 0;
    if (this->dockCameraLeaf) delete (AEGeometry *) this->dockCameraLeaf;
    this->dockCameraLeaf = 0;
    if (this->turretRoot) delete (AEGeometry *) this->turretRoot;
    this->turretRoot = 0;
    if (this->turretExtraGeo) delete (AEGeometry *) this->turretExtraGeo;
    this->turretExtraGeo = 0;
    if (this->shieldGeometry) delete (AEGeometry *) this->shieldGeometry;
    this->shieldGeometry = 0;
    if (this->dockCameraMid) delete (AEGeometry *) this->dockCameraMid;
    this->dockCameraMid = 0;
    if (this->tractorBeam) delete (TractorBeam *) this->tractorBeam;
    this->tractorBeam = 0;
    if (this->miningGame) delete (MiningGame *) (intptr_t) this->miningGame;
    this->miningGame = 0;
    if (this->explosion) delete (Explosion *) (intptr_t) this->explosion;
    this->explosion = 0;
    if (this->explosion2) delete (Explosion *) (intptr_t) this->explosion2;
    this->explosion2 = 0;
    delete this->easeMatrix;
    this->easeMatrix = 0;
    if (this->repairBeams) {
        ArrayReleaseClasses(*this->repairBeams);
        delete this->repairBeams;
    }
    this->repairBeams = 0;
}

void PlayerEgo::setShip(int race, int group) {
    this->field_0x4 = Globals::globals->getShipGroup(race, group, true);
    AbyssEngine::Mesh *mesh = Globals::Canvas->MeshGetPointer(this->field_0x4->meshId);
    this->originalBlendMode = ((AbyssEngine::Material *) mesh->material)->blendMode;

    this->geometry = new AEGeometry(Globals::Canvas);
    this->geometry->addChild(this->field_0x4->transform);

    if (Globals::status->getShip()->getFirstEquipmentOfSort(13) != nullptr) {
        int idx = Globals::status->getShip()->getFirstEquipmentOfSort(13)->getIndex();
        this->tractorBeam = new TractorBeam(this->geometry, idx < 72 ? idx - 68 : 3);
        Globals::globals->addSoundResourceToList(0);
        Globals::globals->addSoundResourceToList(4);
    }

    for (unsigned int i = 0; i < 2; i++) {
        int sort = i == 0 ? 37 : 41;
        Item *item = Globals::status->getShip()->getFirstEquipmentOfSort(sort);
        if (item != nullptr) {
            if (this->repairBeams == nullptr)
                this->repairBeams = new Array<RepairBeam *>();
            RepairBeam *beam = new RepairBeam(item->getIndex(), item->getSort());
            if (item->getIndex() == 222)
                Globals::globals->addSoundResourceToList(2267);
            else if (item->getIndex() == 223)
                Globals::globals->addSoundResourceToList(2268);
            ArrayAdd(beam, *this->repairBeams);
        }
    }

    if (Globals::status->getShip()->getFirstEquipmentOfSort(27) != nullptr
        && Globals::status->getShip()->hasEmergencySystem()) {
        this->field_0xac = (int) (intptr_t) new AEGeometry((uint16_t) 14374, Globals::Canvas, false);
        this->emergencyVal1 = Globals::status->getShip()->getFirstEquipmentOfSort(27)->getAttribute(41);
        this->emergencyVec = ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
            this->field_0x4->transform))->boundingCenter;
        this->emergencyVal2 = ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
            this->field_0x4->transform))->boundingRadius / 500.0f + 0.1f;
    }

    if (Globals::status->inSupernovaSystem() || Globals::status->inSupernovaOrbit()) {
        this->shieldRadius = ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
            this->field_0x4->transform))->boundingRadius * 1.75f;
    }

    if (this->cloak != nullptr)
        Globals::Canvas->MeshCloneMaterial(this->field_0x4->meshId, this->cloakMaterial1);
}

void PlayerEgo::setExhaustVisible(bool param) {
    Level *lvl = this->level;
    this->exhaustVisible = param;
    reinterpret_cast<uint8_t &>(lvl->field_80->flags) = param;
    Array<int> *arr = lvl->field_a8;
    if (arr != nullptr) {
        for (unsigned i = 0; i < arr->size(); i++) {
            ((ParticleSystemManager *) (this->level->field_80))->enableSystemEmit((*arr)[i], param);
        }
    }
}

void PlayerEgo::PlayEngineSound() {
    if (Globals::status->getShip()->getFirstEquipmentOfSort(0x26) != 0
        && !Globals::status->inAlienOrbit()
        && Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                       Globals::status->getCurrentCampaignMission()) > 0.0f
        && this->engineSoundId != -1) {
        Globals::sound->play(this->engineSoundId, nullptr, nullptr, 0.0f);
    }
    this->player->PlayEngineSound(this->field_0x1c, nullptr);
}

void PlayerEgo::PauseEngineSound() {
    ((Player *) this->player)->PauseEngineSound();
}

void PlayerEgo::ResumeEngineSound() {
    ((Player *) this->player)->ResumeEngineSound(false);
}

void PlayerEgo::StopEngineSound() {
    if ((this->dockedFlag == 0 || this->dockingPointIndex != 1)
        && Globals::status->getShip()->getFirstEquipmentOfSort(0x26) != 0
        && !Globals::status->inAlienOrbit()
        && Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                       Globals::status->getCurrentCampaignMission()) > 0.0f
        && this->engineSoundId != -1) {
        Globals::sound->play(this->engineSoundId, nullptr, nullptr, 0.0f);
    }
    this->player->StopEngineSound();
}

bool PlayerEgo::isDockedToDockingPoint() {
    if (this->dockedFlag == 0) return false;
    return this->dockingPointIndex == 1;
}

void PlayerEgo::resetMovement() {
    this->rollAccum = 0;
    this->field_0x26c = 0;
    this->yawAccumD = 0;
    this->field_0x274 = 0;
    this->pitchAccumD = 0;
    this->yawAccumF = 0;
}

void PlayerEgo::resetLastHP() {
    this->lastHP = ((Player *) (this->player))->getCombinedHP();
}

void PlayerEgo::startJumpDrive() {
    if (this->chargingDrive != 0) return;
    Globals::sound->play(0x21, nullptr, nullptr, 0.0f);
    this->hud->hudEvent(0x19, this, 0);
    this->chargingDrive = 1;
    this->driveCharge = 0;
}

bool PlayerEgo::toggleCloaking() {
    if (this->chargingCloak != 0) {
        if (this->cloakCharge >= this->cloakDischargeMax) {
            Globals::sound->play(30, nullptr, nullptr, 0.0f);
            this->player->field_5e = 1;
            this->cloaked = 1;
            this->cloakCharge = 0;
            unsigned int material = 0;
            ((AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(this->cloakMaterial1))->blendMode = 14;
            Globals::Canvas->MeshChangeMaterial(this->field_0x4->meshId, this->cloakMaterial1);
            Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->field_0x4->meshId), 0.0f, 1);
            Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->field_0x4->meshId), 0.0f, 2);
            if (this->turretMode != 0) {
                ((AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(this->cloakMaterial2))->blendMode = 14;
                ((AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(this->cloakMaterial3))->blendMode = 14;
                Globals::Canvas->MeshChangeMaterial(this->turretBaseGeo->meshId, this->cloakMaterial2);
                Globals::Canvas->MeshChangeMaterial(this->turretGeometry->meshId, this->cloakMaterial3);
                Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->turretBaseGeo->meshId),
                                                           0.0f, 1);
                Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->turretBaseGeo->meshId),
                                                           0.0f, 2);
                Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->turretGeometry->meshId),
                                                           0.0f, 1);
                Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->turretGeometry->meshId),
                                                           0.0f, 2);
                if (this->turretMode != 0) {
                    unsigned short resId = 20110;
                    Item *item = Globals::status->getShip()->getFirstEquipmentOfSort(8);
                    if (item != nullptr && item->getIndex() == 224)
                        resId = 24087;
                    item = Globals::status->getShip()->getFirstEquipmentOfSort(35);
                    if (item != nullptr) {
                        int index = item->getIndex();
                        resId = 29037;
                        if (index == 199)
                            resId = 29031;
                        if (index == 198)
                            resId = 29025;
                    }
                    Globals::Canvas->MaterialCreate(resId, material);
                    Globals::Canvas->MeshChangeResourceMaterial(this->turretBaseGeo->meshId, resId);
                    Globals::Canvas->MeshChangeResourceMaterial(this->turretGeometry->meshId, resId);
                }
            }
        }
    } else if (this->cloaked == 0 && this->cloakRechargeTimer <= 0) {
        int energy = this->cloak->getAttribute(38);
        Item *cells = Globals::status->getShip()->getCargo(122);
        if ((cells != nullptr ? cells->getAmount() : 0) < energy)
            return false;
        Globals::status->getShip()->removeCargo(122, energy);
        this->chargingCloak = 1;
        this->hud->hudEvent(30, this, energy);
        this->hud->hudEvent(28, this, 0);
        return true;
    }
    return this->cloaked != 0;
}

bool PlayerEgo::hasCloak() {
    PlayerEgo *self = this;
    return this->cloak != 0;
}

unsigned char PlayerEgo::hasVolatileGoods() {
    return ((uint8_t &) this->volatileGoods);
}

float PlayerEgo::getVolatileForce() {
    float f = ((Player *) this->player)->flShake;
    float r = 0.0f;
    if (!(f < 0.0f)) {
        r = 1.0f;
        if (!(f > 1.0f)) r = f;
    }
    return r;
}

void PlayerEgo::addNukeVolatileForce(float v) {
    Player *p = (Player *) this->player;
    p->flShake = p->flShake + v * 3.0f;
}

unsigned char PlayerEgo::isInTurretMode() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->turretActive);
}

void PlayerEgo::setDockingCamera() {
    if (this->dockCameraNode == nullptr) {
        Globals::Canvas->CameraCreate(this->turretCamera);
        Globals::Canvas->CameraSetPerspective(this->turretCamera, 1.22f, 20.0f,
                                              Globals::status->inAlienOrbit() ? 450000.0f : 300000.0f);
        this->dockCameraNode = new AEGeometry(Globals::Canvas);
        this->dockCameraNode->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
        if (this->turretOffsetVec.x == 0.0f && this->turretOffsetVec.y == 0.0f && this->turretOffsetVec.z == 0.0f) {
            this->turretOffsetVec.y = 150.0f;
            this->turretOffsetVec.z = 500.0f;
        }
        this->dockCameraNode->translate(this->turretOffsetVec);
        this->dockCameraMid = new AEGeometry(Globals::Canvas);
        this->dockCameraMid->translate(0.0f, 200.0f, 500.0f);
        this->dockCameraNode->addChild(this->dockCameraMid->transform);
        this->dockCameraLeaf = new AEGeometry(Globals::Canvas);
        this->dockCameraLeaf->addChild(this->dockCameraNode->transform);
    }
    this->dockCameraLeaf->setPosition(this->dockOffsetVec);
    this->dockCameraLeaf->setMatrix(this->geometry->getMatrix());
    Globals::Canvas->CameraSetCurrent(this->turretCamera);
}

bool PlayerEgo::setTurretMode(bool enable) {
    if (this->turretMode == 0 || this->miningGame != 0 || this->autoTurretEquipped != 0) {
        if (this->rocketControlGun != 0) {
            Globals::Canvas->CameraSetCurrent(this->targetFollowCamera->id);
            this->levelScript->resetCamera(this->level);
        }
        return false;
    }
    this->turretActive = enable;
    if (enable) {
        if (this->rocketControlGun != 0)
            return false;
        if (this->dockCameraNode == nullptr) {
            Globals::Canvas->CameraCreate(this->turretCamera);
            Globals::Canvas->CameraSetPerspective(this->turretCamera, 1.22f, 20.0f,
                                                  Globals::status->inAlienOrbit() ? 450000.0f : 300000.0f);
            this->dockCameraNode = new AEGeometry(Globals::Canvas);
            this->dockCameraNode->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
            this->dockCameraNode->translate(this->turretOffsetVec);
            this->dockCameraMid = new AEGeometry(Globals::Canvas);
            this->dockCameraMid->translate(0.0f, 200.0f, 500.0f);
            this->dockCameraNode->addChild(this->dockCameraMid->transform);
            this->dockCameraLeaf = new AEGeometry(Globals::Canvas);
            this->dockCameraLeaf->addChild(this->dockCameraNode->transform);
            if (Globals::status->getShip()->getFirstEquipmentOfSort(35) != nullptr)
                this->dockCameraNode->rotate(Vector{0.0f, 3.1415927f, 0.0f});
        }
        this->dockCameraLeaf->setPosition(this->dockOffsetVec);
        this->dockCameraLeaf->setMatrix(this->geometry->getMatrix());
        Globals::Canvas->CameraSetCurrent(this->turretCamera);
        this->player->stopShooting(0);
    } else {
        this->stopShooting(2);
        Globals::Canvas->CameraSetCurrent(this->targetFollowCamera->id);
        this->levelScript->resetCamera(this->level);
    }
    if (this->turretExtraGeo != nullptr) {
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->turretExtraGeo->transform))
            ->SetVisible(enable);
        if (enable)
            Globals::sound->play(2255, nullptr, nullptr, 0.0f);
        else
            Globals::sound->stop(2255);
    }
    return true;
}

bool PlayerEgo::isMining() {
    PlayerEgo *self = this;
    return this->miningGame != 0;
}

bool PlayerEgo::isInRocketControl() {
    PlayerEgo *self = this;
    return this->rocketControlGun != 0;
}

void PlayerEgo::stopShooting(int param) {
    if (this->turretActive != 0) {
        ((Player *) this->player)->stopShooting(2);
        return;
    }
    if (((PlayerEgo *) (this))->isDead() != 0) return;
    Player *p = (Player *) this->player;
    if (param == 1) {
        p->stopShooting(1, this->currentSecondaryWeaponIndex);
        return;
    }
    p->stopShooting(param);
}

void PlayerEgo::stopBoost() {
    this->speed = 2.0f;
    this->boostingFlag = 0;
    Globals::sound->stop(0x27);
    Globals::sound->stop(0x26);
    Globals::sound->stop(0x29);
    Globals::sound->stop(0x28);
    Globals::sound->stop(0x44e);
}

int PlayerEgo::isBoostRefreshed() {
    if (this->boostingFlag == 0 && this->field_0x146 != 0 && this->boostTimer >= 0)
        return 1;
    return 0;
}

void PlayerEgo::boost() {
    if (this->boostingFlag != 0) return;
    if (this->field_0x146 == 0) return;
    if (this->rocketControlGun != 0) return;
    if (this->boostTimer < 0) return;
    this->boostingFlag = 1;
    this->boostTimer = 0;
    this->speed = (float) this->boostSpeed;
    Globals::sound->play(this->boostSoundId, nullptr, nullptr, 0.0f);
}

int PlayerEgo::getBoostSpeed() {
    PlayerEgo *self = this;
    return this->boostSpeed;
}

void PlayerEgo::forceBoost() {
    this->boostTimer = 0;
    this->boostingFlag = 1;
    this->speed = 8.0f;
    this->boostTime = 0x2710;
    this->boostDelay = 0;
}

int PlayerEgo::getCurrentMiningAmount() {
    int v = this->miningGame;
    if (v == 0) return 0;
    return ((MiningGame *) v)->getOreAmount();
}

unsigned char PlayerEgo::boosting() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->boostingFlag);
}

bool PlayerEgo::readyToBoost() {
    PlayerEgo *self = this;
    return -1 < this->boostTimer;
}

float PlayerEgo::getBoostPercentage() {
    float den = (float) (this->boostTime / 6);
    float d = (float) this->boostTimer / den;
    float r;
    if (d < 1.0f) {
        r = d;
    } else {
        float sub = 6.0f - d;
        r = 1.0f;
        if (5.0f < d) r = sub;
    }
    return r;
}

float PlayerEgo::getBoostRate() {
    float d = (float) this->boostTimer / (float) this->boostDelay;
    float r = d + 1.0f;
    if (0.0f < d) r = 1.0f;
    return r;
}

float PlayerEgo::getCloakRate() {
    float a = (float) this->cloakCharge;
    float b = (float) (this->cloaked == 0 ? this->cloakDischargeMax : this->cloakChargeMax);
    float d = a / b;
    return d >= 0.0f ? d : 1.0f;
}

float PlayerEgo::getDriveChargeRate() {
    float d = (float) this->driveCharge / (float) this->driveChargeMax;
    return d >= 0.0f ? d : 1.0f;
}

bool PlayerEgo::driveReady() {
    PlayerEgo *self = this;
    return this->driveCharge >= this->driveChargeMax;
}

unsigned char PlayerEgo::isChargingDrive() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->chargingDrive);
}

void PlayerEgo::resetChargingDrive() {
    PlayerEgo *self = this;
    ((uint8_t &) this->chargingDrive) = 0;
}

unsigned char PlayerEgo::isChargingCloak() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->chargingCloak);
}

bool PlayerEgo::isRechargingCloak() {
    PlayerEgo *self = this;
    return 0 < this->cloakRechargeTimer;
}

float PlayerEgo::getCloakRechargeRate() {
    return 1.0f - (float) this->cloakRechargeTimer / (float) this->cloakRechargeMax;
}

bool PlayerEgo::readyForCloak() {
    if (this->chargingCloak == 0) return false;
    return this->cloakCharge >= this->cloakDischargeMax;
}

unsigned char PlayerEgo::isCloaked() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->cloaked);
}

void PlayerEgo::setCollide(bool v) {
    PlayerEgo *self = this;
    this->collide = v;
}

void PlayerEgo::setLevel(Level *level) {
    ((int &) this->level) = (int) (intptr_t) level;
    ParticleSystemManager *src = (ParticleSystemManager *) (intptr_t) level->field_74;
    Matrix *gm = &((AEGeometry *) this->geometry)->getMatrix();
    int sys = ((ParticleSystemManager *) (src))->addSystem(gm, ParticleSettings::ParticleSet_9, 0);
    this->currentSystem = (int) (intptr_t) sys;
    ((ParticleSystemManager *) (this->level->field_74))->enableSystemEmit((int) (intptr_t) sys, 0);
    if (Globals::status->getCurrentCampaignMission() > 1) return;
    ParticleSystemManager *src2 = (ParticleSystemManager *) (intptr_t) this->level->particleEmitBoolPtr;
    Matrix *gm2 = &((AEGeometry *) this->geometry)->getMatrix();
    int sys2 = ((ParticleSystemManager *) (src2))->addSystem(gm2, ParticleSettings::ParticleSet_0xf, 0);
    this->smokeSystem = (int) (intptr_t) sys2;
    ((ParticleSystemManager *) (this->level->particleEmitBoolPtr))->enableSystemEmit((int) (intptr_t) sys2, 0);
    ParticleSystemManager *src3 = (ParticleSystemManager *) (intptr_t) this->level->particleRenderBoolPtr;
    Matrix *gm3 = &((AEGeometry *) this->geometry)->getMatrix();
    int sys3 = ((ParticleSystemManager *) (src3))->addSystem(gm3, ParticleSettings::ParticleSet_0x2a, 0);
    this->explosionSmoke = (int) (intptr_t) sys3;
    ((ParticleSystemManager *) (intptr_t) this->level->particleRenderBoolPtr)->enableSystemEmit(
        (int) (intptr_t) sys3, false);
}

void PlayerEgo::setRoute(Route *v) {
    PlayerEgo *self = this;
    this->route = (int) (intptr_t) v;
}

int PlayerEgo::getRoute() {
    PlayerEgo *self = this;
    return this->route;
}

void PlayerEgo::removeRoute() {
    Route *r = (Route *) ((intptr_t) this->route);
    if (r != 0) delete r;
    this->route = 0;
}

int PlayerEgo::levelCollision() {
    return 0;
}

bool PlayerEgo::isDead() {
    return this->getHitpoints() < 1;
}

int PlayerEgo::getHitpoints() {
    return ((Player *) this->player)->getHitpoints();
}

void PlayerEgo::shoot(int weapon, int type) {
    if (this->turretActive) {
        Matrix turretMatrix = this->geometry->getMatrix() * this->turretGeometry->getMatrix();
        this->player->shoot(2, (long long) weapon, false, turretMatrix);
    } else if (!this->isDead()) {
        if (type == 1) {
            if (!this->player->shoot(1, this->currentSecondaryWeaponIndex, (long long) weapon, false))
                this->currentSecondaryWeaponIndex = -1;
        } else {
            this->player->shoot(type, (long long) weapon, false);
        }
    }
}

unsigned char PlayerEgo::hasAutoTurret() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->autoTurretEquipped);
}

void PlayerEgo::setAutoTurret(bool on) {
    this->autoTurretEnabled = on;
    if (!on) ((Player *) this->player)->stopShooting(2);
}

bool PlayerEgo::autoTurretIsEnabled() {
    return this->autoTurretEnabled;
}

void PlayerEgo::setTurretPosition(Vector v) {
    this->turretOffsetVec = v;
}

Vec3 PlayerEgo::getTurretPosition() {
    Matrix &ship = ((AEGeometry *) (this->geometry))->getMatrix();
    Matrix &turret = ((AEGeometry *) (this->turretGeometry))->getMatrix();
    Matrix world = ship * turret;
    return MatrixGetPosition(world);
}

void PlayerEgo::checkForTurret() {
    if (this->turretMode)
        return;
    this->turretMode = this->player->gunAvailable(2);
    if (!this->turretMode)
        return;

    this->autoTurretEquipped = 0;
    Array<Item *> *equipment = Globals::status->getShip()->getEquipment(2);
    this->turretPitch = (int) ((double) (*equipment)[0]->getAttribute(17) * 1.5);

    unsigned short baseMesh;
    unsigned short barrelMesh;
    int baseExtraMesh = -1;
    int barrelExtraMesh = -1;
    int barrelExtraMesh2 = -1;
    int glowMesh = -1;
    float offsetX = 0.0f;
    float offsetY;
    float offsetZ = 0.0f;
    int turretIndex = (*equipment)[0]->getIndex();
    if (turretIndex == 48) {
        baseMesh = 0x1a74;
        barrelMesh = 0x1a75;
        offsetY = 55.0f;
    } else if (turretIndex == 47) {
        baseMesh = 0x1a72;
        barrelMesh = 0x1a73;
        offsetY = 60.0f;
    } else if (turretIndex == 49) {
        baseMesh = 0x1a76;
        barrelMesh = 0x1a77;
        offsetY = 60.0f;
    } else if (turretIndex == 180) {
        this->autoTurretEquipped = 1;
        baseMesh = 0x1a95;
        barrelMesh = 0x1a96;
        offsetY = 84.0f;
    } else if (turretIndex == 181) {
        this->autoTurretEquipped = 1;
        baseMesh = 0x1a97;
        barrelMesh = 0x1a98;
        offsetY = 88.0f;
        offsetZ = -11.0f;
    } else if (turretIndex == 182) {
        this->autoTurretEquipped = 1;
        baseMesh = 0x1a99;
        barrelMesh = 0x1a9a;
        offsetX = 34.0f;
        offsetY = 88.0f;
        offsetZ = 1.6f;
    } else if (turretIndex == 198) {
        baseMesh = 0x4963;
        barrelMesh = 0x4967;
        baseExtraMesh = 0x4964;
        barrelExtraMesh = 0x4966;
        glowMesh = 0x4a7f;
        offsetY = 80.0f;
    } else if (turretIndex == 199) {
        baseMesh = 0x4968;
        barrelMesh = 0x496b;
        baseExtraMesh = 0x4969;
        barrelExtraMesh = 0x496a;
        glowMesh = 0x4a7f;
        offsetY = 38.0f;
    } else if (turretIndex == 200) {
        baseMesh = 0x496c;
        barrelMesh = 0x496f;
        baseExtraMesh = 0x496d;
        barrelExtraMesh = 0x496e;
        barrelExtraMesh2 = 0x4970;
        glowMesh = 0x4a7f;
        offsetY = 75.0f;
    } else if (turretIndex == 224) {
        baseMesh = 0x499a;
        barrelMesh = 0x499b;
        baseExtraMesh = 0x499c;
        barrelExtraMesh = 0x499d;
        offsetY = 82.0f;
    } else {
        baseMesh = 0xffff;
        barrelMesh = 0xffff;
        offsetY = 0.0f;
    }

    this->turretBaseGeo = new AEGeometry(baseMesh, Globals::Canvas, false);
    this->turretGeometry = new AEGeometry(barrelMesh, Globals::Canvas, false);
    this->turretGeometry->setRotationOrder((AbyssEngine::AEMath::RotationOrder) 2);
    this->turretRoot = new AEGeometry(Globals::Canvas);
    if (baseExtraMesh != -1) {
        AEGeometry *part = new AEGeometry((unsigned short) baseExtraMesh, Globals::Canvas, false);
        this->turretBaseGeo->addChild(part->transform);
        delete part;
    }
    if (barrelExtraMesh != -1) {
        AEGeometry *part = new AEGeometry((unsigned short) barrelExtraMesh, Globals::Canvas, false);
        this->turretGeometry->addChild(part->transform);
        delete part;
    }
    if (barrelExtraMesh2 != -1) {
        AEGeometry *part = new AEGeometry((unsigned short) barrelExtraMesh2, Globals::Canvas, false);
        this->turretGeometry->addChild(part->transform);
        delete part;
    }
    if (glowMesh != -1) {
        this->turretExtraGeo = new AEGeometry((unsigned short) glowMesh, Globals::Canvas, false);
        this->turretGeometry->addChild(this->turretExtraGeo->transform);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->turretExtraGeo->transform))
            ->SetVisible(this->turretActive);
    }
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->turretRoot->transform))->boundingRadius =
        1000000.0f;
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->turretGeometry->transform))
        ->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);
    this->turretBaseGeo->setPosition(this->turretOffsetVec);
    this->turretGeometry->setPosition(this->turretOffsetVec);
    this->turretGeometry->translate(offsetX, offsetY, offsetZ);
    if (!((*equipment)[0]->getIndex() >= 198 && (*equipment)[0]->getIndex() < 201)) {
        this->turretBaseGeo->rotate(0.0f, 3.1415927f, 0.0f);
        this->turretGeometry->rotate(0.0f, 3.1415927f, 0.0f);
    }
    this->turretRoot->addChild(this->turretBaseGeo->transform);
    this->turretRoot->addChild(this->turretGeometry->transform);
    if (this->cloak && this->turretMode) {
        Globals::Canvas->MeshCloneMaterial(this->turretBaseGeo->meshId, this->cloakMaterial2);
        Globals::Canvas->MeshCloneMaterial(this->turretGeometry->meshId, this->cloakMaterial3);
    }
}

void PlayerEgo::addGun(Gun *gun, int x) {
    ((Player *) (this->player))->addGun(gun, x);
    this->checkForTurret();
}

void PlayerEgo::addGun(Array<Gun *> *arr, int x) {
    static_cast<Player *>(this->player)->addGun(arr, x);
    this->checkForTurret();
}

int PlayerEgo::getHullDamageRate() {
    return ((Player *) this->player)->getArmorDamageRate();
}

int PlayerEgo::getShieldDamageRate() {
    return ((Player *) this->player)->getShieldDamageRate();
}

int PlayerEgo::getCurrentSecondaryWeaponIndex() {
    PlayerEgo *self = this;
    return this->currentSecondaryWeaponIndex;
}

void PlayerEgo::setCurrentSecondaryWeaponIndex(int idx) {
    this->currentSecondaryWeaponIndex = idx;
    Globals::status->field_f4 = idx;
}

Vec3 PlayerEgo::GetUpVector() {
    PlayerEgo *self = this;
    return ((AEGeometry *) (self->geometry))->getUpVector();
}

Vec3 PlayerEgo::GetDirVector() {
    PlayerEgo *self = this;
    return ((AEGeometry *) self->geometry)->getDirection();
}

Vec3 PlayerEgo::getPosition() {
    PlayerEgo *self = this;
    return ((AEGeometry *) (self->geometry))->getPosition();
}

void PlayerEgo::setActive(bool value) {
    ((Player *) this->player)->setActive(value);
}

void PlayerEgo::setComputerControlled(bool v) {
    PlayerEgo *self = this;
    this->computerControlled = v;
}

void PlayerEgo::setFreeze(bool v) {
    PlayerEgo *self = this;
    this->freeze = v;
}

void PlayerEgo::setAutoPilot(KIPlayer *kip) {
    this->goingToWaypointFlag = 0;
    int v = (int) (intptr_t) kip;
    unsigned char old = this->autoPilot;
    this->autoPilotTarget = v;
    this->autoPilot = (v != 0) ? 1 : 0;
    if (v == 0) {
        ((KIPlayer *) this->field_0x14)->shipPosition.x = 0;
        if (old != 0) {
            this->bankHistoryIndex = 0;
            this->bankHistoryFilled = 0;
        }
        return;
    }
    if (kip->field_0x72 != 0) this->goingToWaypointFlag = 1;
    AbyssEngine::Engine *eng = (AbyssEngine::Engine *) Globals::appManager->GetEngine();
    ((AbyssEngine::Engine *) eng)->wheelDelta = 0.0f;
    ((int &) this->thrust) = 0x3f800000;
}

void PlayerEgo::setThrust(float v) {
    PlayerEgo *self = this;
    this->thrust = v;
}

int PlayerEgo::getAutoPilotTarget() {
    PlayerEgo *self = this;
    return this->autoPilotTarget;
}

bool PlayerEgo::goingToPlanet() {
    if (this->autoPilot != 0 && ((PlayerEgo *) (this))->goingToStream() == 0 && ((PlayerEgo *) (this))->goingToStation()
        == 0)
        return this->goingToWaypointFlag == 0;
    return false;
}

bool PlayerEgo::isAutoPilot() {
    return this->autoPilot;
}

int PlayerEgo::goingToStream() {
    Level *m = this->level;
    KIPlayer *r4 = (KIPlayer *) (intptr_t) this->autoPilotTarget;
    Array<KIPlayer *> *lm = ((Level *) (m))->getLandmarks();
    return r4 == (*lm)[1];
}

int PlayerEgo::goingToStation() {
    Array<KIPlayer *> *lm = ((Level *) (this->level))->getLandmarks();
    if ((*lm)[0] == 0) return false;
    KIPlayer *r4 = (KIPlayer *) (intptr_t) this->autoPilotTarget;
    lm = ((Level *) (this->level))->getLandmarks();
    return r4 == (*lm)[0];
}

unsigned char PlayerEgo::goingToWaypoint() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->goingToWaypointFlag);
}

bool PlayerEgo::goingToWormhole() {
    Level *m = this->level;
    KIPlayer *r4 = (KIPlayer *) (intptr_t) this->autoPilotTarget;
    Array<KIPlayer *> *lm = ((Level *) (m))->getLandmarks();
    return r4 == (*lm)[3];
}

void PlayerEgo::setPosition(float x, float y, float z) {
    AEGeometry *g = this->geometry;
    char v[12];
    *(float *) (v + 0) = x;
    *(float *) (v + 4) = y;
    *(float *) (v + 8) = z;
    ((AEGeometry *) (g))->setPosition(*(Vector *) v);
}

void PlayerEgo::setPosition(Vector v) {
    setPosition(v.x, v.y, v.z);
}

bool PlayerEgo::isInWormhole() {
    if (((PlayerEgo *) (this))->getHitpoints() > 0) return this->inWormhole != 0;
    return false;
}

void PlayerEgo::handleTurretView(int dt) {
    if ((this->autoPilotTarget == 0 || this->dockedToStream) && !(this->dockedFlag && this->dockingPointIndex == 1))
        this->geometry->moveForward(this->thrust * (float) dt * this->speed);
    Matrix view = this->geometry->getMatrix() * this->dockCameraNode->getMatrix();
    view *= this->dockCameraMid->getMatrix();
    if (this->hitShake || this->boostingFlag) {
        Vector eye = AbyssEngine::AEMath::MatrixGetPosition(view);
        Vector up = AbyssEngine::AEMath::MatrixGetUp(view);
        Vector target = eye - AbyssEngine::AEMath::MatrixGetDir(view);
        if (this->hitShake) {
            this->hitShakeTimer += dt;
            if (this->hitShakeTimer > 1000)
                this->hitShake = 0;
            eye.x += (float) (Globals::rnd->nextInt(12) - 6) * 0.001f;
            eye.y += (float) (Globals::rnd->nextInt(12) - 6) * 0.001f;
            eye.z += (float) (Globals::rnd->nextInt(12) - 6) * 0.001f;
        }
        if (this->boostingFlag) {
            float boost = this->getBoostPercentage();
            int amplitude = this->boostSpeed;
            if (amplitude <= 2)
                amplitude = 2;
            if (amplitude >= 8)
                amplitude = 8;
            target.x += boost * (float) (Globals::rnd->nextInt(amplitude * 2) - amplitude) * 0.0015f;
            target.y += boost * (float) (Globals::rnd->nextInt(amplitude * 2) - amplitude) * 0.0015f;
            target.z += boost * (float) (Globals::rnd->nextInt(amplitude * 2) - amplitude) * 0.0015f;
        }
        view = AbyssEngine::AEMath::MatrixGetLookAt(eye, target, up);
    }
    Globals::Canvas->CameraSetLocal(this->turretCamera, view);
    this->pitchAccumDir = 0;
    this->yawAccumDir = 0;
    this->roll(this->shakeIntensity);
    reinterpret_cast<Matrix &>(this->player->transform[0]) =
        *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
        * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
}

void PlayerEgo::roll(int amount) {
    if (!this->rolling)
        return;

    AbyssEngine::AEMath::Matrix &m = this->geometry->getMatrix();
    float rx = m.m[4];
    float ry = m.m[5];
    float mag = rx > 0.0f ? rx : -rx;
    if (amount >= 60)
        amount = 60;

    if (ry > 0.0f && mag < 0.015f) {
        this->rollMatrix = AbyssEngine::AEMath::MatrixIdentity(this->rollMatrix);
        this->rolling = 0;
        this->rollDirection = 0;
        this->autoLevel = 0;
        return;
    }

    float step;
    if (this->autoLevel) {
        // Level out slowly towards the upright position (left or right half).
        step = -0.0002f;
        if (rx < 0.0f && (ry >= 0.0f || ry < 0.0f))
            step = 0.0002f;
    } else if (rx > 0.0f && this->rollDirection == 1) {
        this->autoLevel = 1;
        step = -0.00035f;
    } else if (rx < 0.0f && this->rollDirection == 2) {
        this->autoLevel = 1;
        step = 0.00035f;
    } else if (rx < 0.0f && ry < 0.0f) {
        step = 0.00075f;
    } else if (rx >= 0.0f && ry < 0.0f) {
        step = -0.00075f;
    } else if (rx > 0.0f && ry > 0.0f) {
        step = rx > 0.3f ? -0.00035f : -0.00025f;
    } else if (rx < 0.0f && ry >= 0.0f) {
        step = rx > -0.3f ? 0.00025f : 0.00035f;
    }

    if (rx < 0.0f || rx > 0.0f)
        this->rollDirection = rx < 0.0f ? 1 : 2;
    this->rolling = 1;
    this->rollMatrix = AbyssEngine::AEMath::MatrixSetRotation(this->rollMatrix, 0.0f, 0.0f, step * (float) amount);
}

void PlayerEgo::handleShip(int dt) {
    Globals::sound->setParamValue(this->player->GetEngineEvent(), 0,
                                  PE_maxf(PE_absf(this->yawAccumD), PE_absf(this->rollAccum)));
    Globals::sound->setParamValue(this->player->GetEngineEvent(), 1, this->rollAccum * 0.2f + 0.5f);

    Matrix local = *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform);
    Matrix rotation;
    float frameTime = (float) dt;
    AbyssEngine::AEMath::MatrixSetRotation(rotation,
                                           this->pitchAccumD * (1.0f / 65536.0f) * 6.2831855f * frameTime * 0.033f,
                                           this->yawAccumF * (1.0f / 65536.0f) * 6.2831855f * frameTime * 0.033f,
                                           0.0f);
    this->field_0x280 = (int &) this->yawAccumF;
    local *= rotation;
    if (this->rolling)
        local *= this->rollMatrix;

    float distance = this->thrust * frameTime * this->speed;
    local.m[3] += local.m[2] * distance;
    local.m[7] += distance * local.m[6];
    local.m[11] += distance * local.m[10];

    Vector direction = {local.m[2], local.m[6], local.m[10]};
    Vector up = {local.m[1], local.m[5], local.m[9]};
    direction = AbyssEngine::AEMath::VectorNormalize(direction);
    up = AbyssEngine::AEMath::VectorNormalize(up);
    Vector right = AbyssEngine::AEMath::VectorCross(up, direction);
    right = AbyssEngine::AEMath::VectorNormalize(right);
    local.m[0] = right.x;
    local.m[4] = right.y;
    local.m[8] = right.z;
    local.m[1] = up.x;
    local.m[5] = up.y;
    local.m[9] = up.z;
    local.m[2] = direction.x;
    local.m[6] = direction.y;
    local.m[10] = direction.z;

    float slide = this->strafeAccel > 0.0f ? this->strafeAccel : -this->strafeAccel;
    if (slide > 0.01f) {
        Vector offset = {this->strafeAccel * (float) this->shakeIntensity, 0.0f, 0.0f};
        offset = AbyssEngine::AEMath::MatrixRotateVector(local, offset);
        local.m[3] = offset.x + local.m[3];
        local.m[7] = offset.y + local.m[7];
        local.m[11] = offset.z + local.m[11];
        this->strafeAccel = this->strafeAccel * 0.7f;
        this->targetFollowCamera->translateNoUpdate(offset.x, offset.y, offset.z);
    } else {
        this->strafeFactor = 0.1f;
    }
    Globals::Canvas->TransformSetLocal(this->geometry->transform, local);
    this->pitchRamp = 0;
    this->yawRamp = 0;
    this->pitchAccumDir = 0;
    this->yawAccumDir = 0;
    this->yawAccumD = 0;
    this->rollAccum = 0;
    reinterpret_cast<Matrix &>(this->player->transform[0]) =
        *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
        * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
}

float PlayerEgo::getCloakingPercentage() {
    float r = 0.0f;
    if (this->cloaked != 0) {
        int v = this->cloakCharge;
        if (v >= 0) {
            if (v < 2000) {
                r = (float) v * 100.0f / 2000.0f;
            } else {
                int lo = this->cloakChargeMax - 2000;
                if (v > lo) {
                    r = (((float) v - (float) lo) / -2000.0f + 1.0f) * 100.0f;
                } else {
                    r = 100.0f;
                }
            }
        }
    }
    return r;
}

int PlayerEgo::updateManeuver() {
    if (this->maneuverType == 1 || this->maneuverType == 2) {
        this->maneuverTimer += this->shakeIntensity;
        Vector side = this->geometry->getRightVector() * (float) this->shakeIntensity * 4.0f
                      * ((float) this->maneuverTimer / -1200.0f + 1.0f);
        side *= this->handling * 0.05f;
        if (this->maneuverType == 2)
            side *= -1.0f;
        this->geometry->moveForward(this->thrust * (float) this->shakeIntensity * this->speed);
        this->geometry->translate(side.x, side.y, side.z);
        Matrix spin;
        AbyssEngine::AEMath::MatrixSetRotation(
            spin, 0.0f, (this->maneuverType == 1 ? -1.0f : 1.0f)
                            * ((1.0f - (float) this->maneuverTimer / 1200.0f) * 0.0062831859f), 0.0f);
        this->geometry->setMatrix(this->geometry->getMatrix() * spin);
        float bank = this->handling * 0.05f * 750.0f * 0.4f;
        this->yawAccumF = AbyssEngine::AEMath::Sinf((float) this->maneuverTimer / 1200.0f * 3.1415927f) * bank
                          * (this->maneuverType == 1 ? -1.0f : 1.0f);
        TargetFollowCamera *camera = (TargetFollowCamera *) (intptr_t) this->targetFollowCamera;
        Vector offset = *camera->getTargetOffset();
        offset.x = this->yawAccumF * -3.0f;
        this->targetFollowCamera->setTargetOffset(offset);
        reinterpret_cast<Matrix &>(this->player->transform[0]) =
            *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
            * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
        this->targetFollowCamera
            ->translateNoUpdate(side.x * -0.9f, side.y * -0.9f, side.z * -0.9f);
        if (this->maneuverTimer >= 1200) {
            this->maneuverType = 0;
            this->levelScript->resetCamera(this->level);
        }
        return 1;
    }
    if (this->maneuverType == 3) {
        this->maneuverTimer += this->shakeIntensity;
        Vector direction = this->geometry->getDirection();
        Vector right = this->geometry->getRightVector();
        Vector up = this->geometry->getUpVector();
        (void) right;
        Vector target = this->getPosition() + 10000.0f * direction + 10000.0f * up;
        if (this->maneuverTimer > 900)
            target = this->strafeTargetVec;
        this->moveToPosition(target, true, 7.0f);
        Matrix spin;
        AbyssEngine::AEMath::MatrixSetRotation(spin, 0.0f, 0.0f,
                                               (float) this->maneuverTimer / 3000.0f * 6.2831855f);
        this->geometry->setMatrix(this->geometry->getMatrix() * spin);
        reinterpret_cast<Matrix &>(this->player->transform[0]) =
            *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
            * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
        this->targetFollowCamera->setLookAtCam(true);
        this->targetFollowCamera->useTargetsUpVector(false);
        Vector shift = this->facingVec * 7.0f * (float) this->shakeIntensity
                       * (1.0f - (float) this->maneuverTimer / 3000.0f);
        this->targetFollowCamera->translate(shift.x, shift.y, shift.z);
        this->geometry->moveForward(this->thrust * (float) this->shakeIntensity * this->speed);
        if (this->maneuverTimer >= 3000) {
            this->maneuverType = 0;
            this->targetFollowCamera->setLookAtCam(false);
            this->targetFollowCamera->useTargetsUpVector(true);
        }
        return 1;
    }
    return 0;
}

float PlayerEgo::moveToPosition(Vector target, bool steer, float speed) {
    this->moveTarget = Vector(target);
    this->moveTarget -= this->geometry->getPosition();
    this->moveTarget = AbyssEngine::AEMath::VectorNormalize(this->moveTarget);
    this->moveDirection = this->geometry->getDirection();
    this->moveDirection = AbyssEngine::AEMath::VectorNormalize(this->moveDirection);
    this->moveTarget -= this->moveDirection;
    this->moveTarget *= (float) (int) ((float) this->shakeIntensity * speed) / 4096.0f;
    this->headingVec = this->moveDirection + this->moveTarget;
    this->headingVec = AbyssEngine::AEMath::VectorNormalize(this->headingVec);

    float bank;
    if (steer) {
        float cosAngle = AbyssEngine::AEMath::VectorDot(AbyssEngine::AEMath::VectorNormalize(this->moveDirection),
                                                        this->headingVec);
        float angle = 0.0f;
        if (cosAngle < 1.0f && cosAngle > -1.0f)
            angle = AbyssEngine::AEMath::ACosf(cosAngle);
        bank = angle > 0.0f ? angle : -angle;
        if (bank != 0.0f) {
            float side = AbyssEngine::AEMath::ACosf(
                AbyssEngine::AEMath::VectorDot(this->geometry->getRightVector(), Vector(this->headingVec)));
            bank = side < 1.5707964f ? -bank : bank;
        }
        this->bankHistory[this->bankHistoryIndex] = bank;
        if (this->bankHistoryFilled) {
            float sum = 0.0f;
            for (int i = 0; i < 5; i++)
                sum += this->bankHistory[i];
            bank = sum / 5.0f;
        } else if (this->bankHistoryIndex != 0) {
            float sum = 0.0f;
            for (int i = 0; i < this->bankHistoryIndex; i++)
                sum += this->bankHistory[i];
            bank = sum / (float) this->bankHistoryIndex;
        }
        this->bankHistoryIndex++;
        if (this->bankHistoryIndex >= 5) {
            this->bankHistoryIndex = 0;
            if (!this->bankHistoryFilled)
                this->bankHistoryFilled = true;
        }
        bank = -(bank * this->maxBank * this->bankFactor);
        if (bank > this->maxBank)
            bank = this->maxBank;
        else if (bank < -this->maxBank)
            bank = -this->maxBank;
    } else {
        bank = 0.0f;
    }

    this->geometry->setDirection(this->headingVec, Vector{0.0f, 1.0f, 0.0f});
    this->geometry->moveForward((float) this->shakeIntensity * this->thrust * this->speed);
    this->roll(this->shakeIntensity);

    float strafe = this->strafeAccel;
    if ((strafe > 0.0f ? strafe : -strafe) > 0.01f) {
        Vector offset = {strafe * (float) this->shakeIntensity, 0.0f, 0.0f};
        Matrix matrix = this->geometry->getMatrix();
        offset = AbyssEngine::AEMath::MatrixRotateVector(matrix, offset);
        matrix.m[3] += offset.x;
        matrix.m[7] += offset.y;
        matrix.m[11] += offset.z;
        this->strafeAccel *= 0.7f;
        this->targetFollowCamera->translateNoUpdate(offset.x, offset.y, offset.z);
        this->geometry->setMatrix(matrix);
    } else {
        this->strafeFactor = 0.1f;
    }

    reinterpret_cast<Matrix &>(this->player->transform[0]) = this->geometry->getMatrix();
    this->dockOffsetVec = this->geometry->getPosition();
    return bank;
}

void PlayerEgo::handleAutoTurret(int dt) {
    this->autoTurretTimer += dt;
    if (this->autoTurretTimer > 3000) {
        this->autoTurretTarget = nullptr;
        this->autoTurretTimer = 0;
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        if (enemies != nullptr) {
            int nearest = 60000;
            for (unsigned int i = 0; i < enemies->size(); i++) {
                KIPlayer *enemy = (*enemies)[i];
                if (enemy->isDead() || enemy->isDying() || !enemy->player->isActive() || !enemy->isEnemy()
                    || enemy->field_0x74)
                    continue;
                int distance = (int) AbyssEngine::AEMath::VectorLength(this->dockOffsetVec - enemy->getPosition());
                if (nearest > distance
                    && (this->autoTurretTarget == nullptr || this->autoTurretTarget != this->autoTurretPrevTarget)) {
                    nearest = distance;
                    this->autoTurretTarget = enemy;
                }
            }
        }
    }

    KIPlayer *target = this->autoTurretTarget;
    if (target == nullptr || target->geometry == nullptr) {
        this->player->stopShooting(2);
        return;
    }
    Matrix aim = reinterpret_cast<Matrix &>(target->player->transform[0]);
    Vector targetPosition = target->getPosition();
    this->moveTarget =
        targetPosition + AbyssEngine::AEMath::VectorNormalize(AbyssEngine::AEMath::MatrixGetDir(aim)) * 1500.0f;
    aim = this->geometry->getMatrix() * this->turretGeometry->getMatrix();
    Vector local = AbyssEngine::AEMath::VectorNormalize(
        AbyssEngine::AEMath::MatrixInverseTransformVector(aim, this->moveTarget));
    bool aligned = false;
    if (local.x > 0.05f) {
        float angle = (float) dt / (200.0f / (float) this->turretPitch) * 0.000244140625f * 6.2831855f;
        this->turretBaseGeo->rotate(0.0f, angle, 0.0f);
        this->turretGeometry->rotate(0.0f, angle, 0.0f);
    } else if (local.x < -0.05f) {
        float angle = (float) -dt / (200.0f / (float) this->turretPitch) * 0.000244140625f * 6.2831855f;
        this->turretBaseGeo->rotate(0.0f, angle, 0.0f);
        this->turretGeometry->rotate(0.0f, angle, 0.0f);
    } else {
        aligned = true;
    }
    if (local.y > 0.05f) {
        if (this->lookYaw > -500.0f) {
            this->lookYaw = this->lookYaw - (float) this->shakeIntensity;
            this->turretGeometry->rotate((float) this->shakeIntensity * 0.000244140625f * -6.2831855f, 0.0f, 0.0f);
            this->autoTurretPrevTarget = nullptr;
        } else {
            this->autoTurretPrevTarget = this->autoTurretTarget;
            this->autoTurretTimer += dt;
        }
    } else if (local.y < -0.05f) {
        if (this->lookYaw < 70.0f) {
            this->lookYaw = this->lookYaw + (float) this->shakeIntensity;
            this->turretGeometry->rotate((float) this->shakeIntensity * 0.000244140625f * 6.2831855f, 0.0f, 0.0f);
            this->autoTurretPrevTarget = nullptr;
        } else {
            this->autoTurretPrevTarget = this->autoTurretTarget;
            this->autoTurretTimer += dt;
        }
    } else if (aligned) {
        this->player->shoot(2, (long long) dt, false, aim);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->turretGeometry->transform))
            ->Update((long long) dt, false);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->turretBaseGeo->transform))
            ->Update((long long) dt, false);
        this->autoTurretFireTimer = 0;
        return;
    }
    this->autoTurretFireTimer += dt;
    if (this->autoTurretFireTimer > 500)
        this->player->stopShooting(2);
}

void PlayerEgo::update(int dt, Radar *radar, Hud *hud, Radio *radio, LevelScript *script, int arg5, bool mouseLocked,
                       int controlMode) {
    (void) arg5;
    this->levelScript = script;
    if (this->hud == 0) {
        this->hud = hud;
        this->field_0x14 = radar;
        this->radioRef = radio;
    }
    if (this->freeze != 0)
        return;

    this->dockOffsetVec = this->geometry->getPosition();
    this->shakeIntensity = dt;
    if (this->boostTimer < 0 && this->boostTimer + dt * 3 > 0)
        this->boostTimer = 0;
    this->boostTimer += dt;

    if (this->field_0x39a && !this->field_0x399 && this->player->getShieldHP() <= 0
        && Globals::status->getShip()->getFirstEquipmentOfSort(43) != nullptr) {
        int cells = Globals::status->getShip()->getFirstEquipmentOfSort(43)->getAttribute(59);
        if (Globals::status->getShip()->hasCargo(202, cells)) {
            this->field_0x399 = 1;
            Globals::sound->play(2258, nullptr, nullptr, 0.0f);
            Globals::sound->play(2257, nullptr, nullptr, 0.0f);
            Vector velocity = {0.0f, 0.0f, 0.0f};
            Vector position = this->getPosition();
            Globals::sound->updateEvent3DAttributes(2258, &position, &velocity, false);
            Globals::status->getShip()->removeCargo(202, cells);
            hud->hudEvent(47, this, cells);
        }
    }
    if (this->field_0x399) {
        if (this->player->getShieldHP() < this->player->getMaxShieldHP()) {
            this->player->setShieldHP((int) ((float) this->player->getShieldHP()
                                             + ((float) dt * 0.15f < 1.0f ? 1.0f : (float) dt * 0.15f)));
            Globals::sound->setParamValue(0, 2257,
                                          (float) this->player->getShieldHP() / (float) this->player->getMaxShieldHP());
            Vector velocity = {0.0f, 0.0f, 0.0f};
            Vector position = this->getPosition();
            Globals::sound->updateEvent3DAttributes(2257, &position, &velocity, false);
        } else {
            Globals::sound->play(2259, nullptr, nullptr, 0.0f);
            Globals::sound->stop(2257);
            Vector velocity = {0.0f, 0.0f, 0.0f};
            Vector position = this->getPosition();
            Globals::sound->updateEvent3DAttributes(2259, &position, &velocity, false);
            this->field_0x399 = 0;
        }
    }

    if (this->cloakRechargeTimer >= 1) {
        this->cloakRechargeTimer -= dt;
        if (this->cloakRechargeTimer <= 0)
            hud->hudEvent(46, this, 0);
    }

    if (this->hackingGame != 0) {
        ((HackingGame *) (intptr_t) this->hackingGame)->update(this->shakeIntensity);
        if (((HackingGame *) (intptr_t) this->hackingGame)->gameWon()) {
            hud->setHackingGameActive(false);
            if (Globals::status->getStation()->stationHasHiddenBlueprint(false)) {
                int blueprint = Globals::status->getStation()->getHiddenBlueprintIndex();
                Globals::status->unlockBluePrint(kHiddenBlueprints[blueprint]);
                if (this->hackingGame != 0)
                    delete (HackingGame *) (intptr_t) this->hackingGame;
                this->hackingGame = 0;
                Array<KIPlayer *> *enemies = this->level->getEnemies();
                if (enemies != nullptr) {
                    for (unsigned int i = 0; i < enemies->size(); i++) {
                        if ((*enemies)[i]->field_0x70) {
                            (*enemies)[i]->field_0x70 = 0;
                            ((PlayerFixedObject *) (*enemies)[i])->setDockingType(0);
                            delete (*enemies)[i]->cargo;
                            (*enemies)[i]->cargo = nullptr;
                            break;
                        }
                    }
                }
                this->level->createRadioMessage(blueprint + 21, 0);
                (*Globals::status->field_58)[blueprint] = true;
            }
        }
    }

    if (Globals::status->getShip()->getFreeSpace() < this->freeCargoSpace) {
        this->freeCargoSpace = Globals::status->getShip()->getFreeSpace();
        this->volatileGoods = Globals::status->getShip()->hasVolatileGoods();
    }

    if (this->boostingFlag) {
        if (this->volatileGoods) {
            float shake = 0.13f;
            Item *booster = Globals::status->getShip()->getFirstEquipmentOfSort(14);
            if (booster != nullptr)
                shake = booster->getIndex() == 195 ? 0.17f : 0.13f;
            this->player->flShake += (float) dt * 0.001f * shake;
        }
        if (this->level->field_a4 != nullptr) {
            ParticleSettings &settings = reinterpret_cast<ParticleSettings &>(ParticleSettingsRef::cur);
            for (unsigned int i = 0; i < this->level->field_a4->size(); i++) {
                float scale = (*this->level->field_a4)[i]->getScaling().x;
                reinterpret_cast<float &>(settings.sets[29 + i].lifeBase) =
                    scale * 250.0f * (this->getBoostPercentage() * 0.5f + 1.0f);
            }
        }
        if (this->boostTimer > this->boostTime) {
            ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->SetPostEffect(0x01400002, false);
            this->speed = 2.0f;
            this->boostingFlag = 0;
            this->boostTimer = -this->boostDelay;
            if (!this->field_0x146) {
                this->boostSpeed = 2;
                this->boostTime = 0;
                this->boostDelay = 0;
            }
        }
    }

    if (this->cloaked) {
        this->cloakCharge += this->shakeIntensity;
        reinterpret_cast<long long &>(Globals::status->field_c0) += this->shakeIntensity;
        if (this->cloakCharge < 2000) {
            int alpha = 255 - (int) ((float) this->cloakCharge / 2000.0f * 255.0f);
            if (alpha <= 50)
                alpha = 50;
            unsigned int color = alpha | 0xffffff00;
            if (this->cloakCharge == 0)
                this->cloakCharge = 1;
            Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->field_0x4->meshId),
                                                       (float) this->cloakCharge / 2000.0f, 1);
            if (this->turretMode) {
                Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->turretBaseGeo->meshId),
                                                           (float) this->cloakCharge / 2000.0f, 1);
                Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->turretGeometry->meshId),
                                                           (float) this->cloakCharge / 2000.0f, 1);
            }
            if ((int) this->field_0x4->parentTransform != -1)
                Globals::Canvas->TransformSetColor(this->field_0x4->parentTransform, color);
            if ((int) this->field_0x4->childTransform != -1)
                Globals::Canvas->TransformSetColor(this->field_0x4->childTransform, color);
            if (this->turretMode) {
                Globals::Canvas->TransformSetColor(this->turretBaseGeo->transform, color);
                Globals::Canvas->TransformSetColor(this->turretGeometry->transform, color);
            }
            if (this->getCloakingPercentage() > 25.0f && !this->field_0x325) {
                this->field_0x325 = 1;
                if (this->field_0x4 != nullptr && (int) this->field_0x4->childTransform != -1)
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->field_0x4->childTransform))
                        ->SetVisible(!this->field_0x325);
            }
        }
        Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->field_0x4->meshId),
                                                   (float) this->cloakCharge * 0.001f, 2);
        if (this->turretMode) {
            Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->turretBaseGeo->meshId),
                                                       (float) this->cloakCharge * 0.001f, 2);
            Globals::Canvas->MeshChangeShaderAnimValue(Globals::Canvas->MeshGetPointer(this->turretGeometry->meshId),
                                                       (float) this->cloakCharge * 0.001f, 2);
        }
        int charge = this->cloakCharge;
        int fadeStart = this->cloakChargeMax - 2000;
        if (charge > fadeStart) {
            Globals::Canvas->MeshChangeShaderAnimValue(
                Globals::Canvas->MeshGetPointer(this->field_0x4->meshId),
                1.0f - ((float) this->cloakCharge - (float) (this->cloakChargeMax - 2000)) / 2000.0f, 1);
            int alpha = (int) (((float) charge - (float) fadeStart) / 2000.0f * 255.0f);
            if (alpha <= 50)
                alpha = 50;
            unsigned int color = alpha | 0xffffff00;
            if (this->turretMode) {
                Globals::Canvas->MeshChangeShaderAnimValue(
                    Globals::Canvas->MeshGetPointer(this->turretBaseGeo->meshId),
                    1.0f - ((float) this->cloakCharge - (float) (this->cloakChargeMax - 2000)) / 2000.0f, 1);
                Globals::Canvas->MeshChangeShaderAnimValue(
                    Globals::Canvas->MeshGetPointer(this->turretGeometry->meshId),
                    1.0f - ((float) this->cloakCharge - (float) (this->cloakChargeMax - 2000)) / 2000.0f, 1);
            }
            if ((int) this->field_0x4->parentTransform != -1)
                Globals::Canvas->TransformSetColor(this->field_0x4->parentTransform, color);
            if ((int) this->field_0x4->childTransform != -1)
                Globals::Canvas->TransformSetColor(this->field_0x4->childTransform, color);
            if (this->turretMode) {
                Globals::Canvas->TransformSetColor(this->turretBaseGeo->transform, color);
                Globals::Canvas->TransformSetColor(this->turretGeometry->transform, color);
            }
            if (this->getCloakingPercentage() < 25.0f && this->field_0x325) {
                this->field_0x325 = 0;
                if (this->field_0x4 != nullptr && (int) this->field_0x4->childTransform != -1)
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->field_0x4->childTransform))
                        ->SetVisible(!this->field_0x325);
            }
        }
        if (this->cloakCharge > this->cloakChargeMax) {
            this->cloaked = 0;
            this->cloakCharge = 0;
            this->cloakRechargeTimer = this->cloakRechargeMax;
            this->player->field_5e = 0;
            Globals::sound->play(30, nullptr, nullptr, 0.0f);
            if (this->field_0x325) {
                this->field_0x325 = 0;
                if (this->field_0x4 != nullptr && (int) this->field_0x4->childTransform != -1)
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->field_0x4->childTransform))
                        ->SetVisible(!this->field_0x325);
            }
            ((AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(this->cloakMaterial1))->blendMode =
                this->originalBlendMode;
            if (this->turretMode) {
                ((AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(this->cloakMaterial2))->blendMode =
                    this->originalBlendMode;
                ((AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(this->cloakMaterial3))->blendMode =
                    this->originalBlendMode;
                if (this->turretMode) {
                    unsigned short resId = 20102;
                    Item *item = Globals::status->getShip()->getFirstEquipmentOfSort(8);
                    if (item != nullptr && item->getIndex() == 224)
                        resId = 24085;
                    item = Globals::status->getShip()->getFirstEquipmentOfSort(35);
                    if (item != nullptr) {
                        int index = item->getIndex();
                        resId = 29035;
                        if (index == 199)
                            resId = 29029;
                        if (index == 198)
                            resId = 29023;
                    }
                    Globals::Canvas->MeshChangeResourceMaterial(this->turretBaseGeo->meshId, resId);
                    Globals::Canvas->MeshChangeResourceMaterial(this->turretGeometry->meshId, resId);
                }
            }
        }
        Level *level = this->level;
        float engineColor = 221.0f + this->getCloakingPercentage() * -201.0f * 0.01f;
        if (engineColor < 221.0f) {
            if (engineColor < 0.0f)
                engineColor = 0.0f;
        } else {
            engineColor = 221.0f;
        }
        level->setPlayerEngineColor((short) engineColor);
    } else if (this->chargingCloak) {
        this->cloakCharge += this->shakeIntensity;
        if (this->cloakCharge > this->cloakDischargeMax) {
            hud->hudEvent(29, this, 0);
            this->toggleCloaking();
            this->chargingCloak = 0;
        }
    }

    if (this->chargingDrive) {
        this->driveCharge += this->shakeIntensity;
        if (this->driveCharge >= this->driveChargeMax) {
            this->shieldVisible = 0;
            hud->hudEvent(26, this, 0);
        }
        if (this->volatileGoods)
            this->player->flShake += (float) dt * 0.001f * 0.5f;
    }
    if (this->tractorBeam != nullptr)
        this->tractorBeam->update(this->shakeIntensity, radar, this->level, hud);
    if (this->repairBeams != nullptr) {
        for (unsigned int i = 0; i < this->repairBeams->size(); i++)
            (*this->repairBeams)[i]->update(this->shakeIntensity, radar, this->level, hud);
    }
    if (this->throttleStarted) {
        this->throttle += dt;
        if (this->throttle >= 2000)
            this->throttleStarted = 0;
    }

    if (this->turretMode && !this->autoTurretEquipped && this->turretActive
        && Globals::status->getShip()->getFirstEquipmentOfSort(35) != nullptr) {
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->turretGeometry->transform))
            ->Update((long long) dt, true);
        if (this->turretExtraGeo != nullptr)
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->turretExtraGeo->transform))
                ->Update((long long) dt, false);
    }
    if (this->shieldGeometry != nullptr && this->shieldVisible) {
        this->shieldGeometry->setScaling(this->shieldRadius, this->shieldRadius, this->shieldRadius);
        this->shieldGeometry->setDirection(
            AbyssEngine::AEMath::VectorNormalize(this->level->getStarSystem()->getLightDirection()),
            Vector{0.0f, 1.0f, 0.0f});
        this->shieldGeometry->setPosition(this->geometry->getPosition());
        if (Globals::status->getShip()->getIndex() == 8)
            this->shieldGeometry->translate(AbyssEngine::AEMath::VectorNormalize(this->geometry->getDirection()) * 300.0f);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->shieldGeometry->transform))
            ->Update((long long) this->shakeIntensity, true);
    }
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->field_0x4->transform))
        ->Update((long long) dt, true);

    bool controlled = this->computerControlled;
    bool toPlanet = this->dockingToPlanet;
    this->aboutToReachAutoTargetFlag = 0;
    this->field_0x145 = 0;
    bool moving = false;
    if (controlled) {
        this->field_0x145 = 1;
        if (toPlanet)
            this->planetDockTimer += this->shakeIntensity;
        moving = true;
    } else if (toPlanet) {
        this->field_0x145 = 1;
        this->planetDockTimer += this->shakeIntensity;
        moving = true;
    } else if (this->dockedToStream) {
        this->field_0x145 = 1;
        moving = true;
    }
    if (moving) {
        this->geometry->moveForward(this->speed * (float) dt);
        reinterpret_cast<Matrix &>(this->player->transform[0]) =
            *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
            * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
    }

    if (this->collide && this->player->isActive()) {
        this->collidesWithStationFlag = 0;
        this->calcCollision(this->level->getLandmarks());
        this->calcCollision(this->level->getEnemies());
        this->calcCollision(this->level->getAsteroids());
        if (this->collidesWithStationFlag && this->turretActive)
            this->handleTurretView(dt);
    }

    if (this->volatileGoods) {
        if (!Globals::sound->isPlaying(35))
            Globals::sound->play(35, nullptr, nullptr, 0.0f);
        float pitchDelta = this->yawAccumD - (float &) this->field_0x274;
        float rollDelta = this->rollAccum - (float &) this->field_0x26c;
        float pitchLevel = pitchDelta > 0.0f ? pitchDelta : -pitchDelta;
        float rollLevel = rollDelta > 0.0f ? rollDelta : -rollDelta;
        this->player->flShake += (float) dt * 0.001f * 0.5f * (pitchLevel > rollLevel ? pitchLevel : rollLevel);
        (float &) this->field_0x26c = this->rollAccum;
        (float &) this->field_0x274 = this->yawAccumD;
        if (this->player->flShake >= 1.0f && !this->dockingToPlanet && !this->dockedToStream) {
            this->player->setHitpoints(0);
            if (this->explosion2 == nullptr) {
                this->explosion2 = new Explosion(12);
                this->explosion2->setScaling(0.7f);
                this->explosion2->start(this->geometry->getPosition(), Vector{0.0f, 0.0f, 0.0f});
            }
        }
        if (this->player->getHitpoints() >= 1)
            Globals::sound->setParamValue(0, 35, this->player->flShake);
        else
            Globals::sound->setParamValue(0, 35, 0.0f);
    } else {
        this->player->flShake = 0;
    }

    if (this->player->getGammaHP() <= 0 && !this->dockingToPlanet && !this->dockingToStream)
        this->player->setHitpoints(0);

    if ((char &) this->dockingState) {
        this->field_0x145 = 1;
        KIPlayer *asteroid = (KIPlayer *) (intptr_t) this->asteroidTarget;
        if (asteroid == nullptr || asteroid->isDying() || ((KIPlayer *) (intptr_t) this->asteroidTarget)->isDead()) {
            this->dockToAsteroid(nullptr, radar);
            return;
        }
        this->approachAsteroid(hud, 0, radar);
    }

    if (this->dockedFlag && this->player->getHitpoints() >= 1) {
        this->field_0x145 = 1;
        KIPlayer *station = (KIPlayer *) (intptr_t) this->asteroidTarget;
        if (station == nullptr || station->isDying() || ((KIPlayer *) (intptr_t) this->asteroidTarget)->isDead()) {
            this->dockToDockingPoint(nullptr, nullptr);
            return;
        }
        if (this->approachDockingPoint(hud, 0, radar)) {
            Mission *mission = Globals::status->getMission();
            Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
            PlayerFixedObject *dock = (PlayerFixedObject *) (intptr_t) this->asteroidTarget;
            if (!mission->isEmpty() && (mission->getType() == 15 || mission->getType() == 184)
                && dock->getDockingType() == 1) {
                int delivered = this->dockTransferedAmount;
                if (delivered != this->dockTotalAmount) {
                    this->dockTransferedAmount =
                        (int) ((float) this->field_0x35c / (mission->getType() == 184 ? 1500.0f : 1000.0f));
                    if (this->dockTotalAmount <= this->dockTransferedAmount) {
                        if (delivered < this->dockTransferedAmount)
                            hud->hudEvent(mission->getType() == 184 ? 38 : 42, this, 0);
                        this->dockTransferedAmount = this->dockTotalAmount;
                    }
                    if (this->dockTransferedAmount > delivered) {
                        if (mission->getType() == 184) {
                            if (Globals::status->field_178 >= 1) {
                                Globals::status->field_178 += delivered - this->dockTransferedAmount;
                                mission->setStatusValue(mission->getStatusValue() + delivered - this->dockTransferedAmount);
                                if (mission->getStatusValue() < 0)
                                    mission->setStatusValue(0);
                                if (Globals::status->field_178 < 0)
                                    Globals::status->field_178 = 0;
                            }
                        } else if (mission->getType() == 15) {
                            Globals::status->getShip()->removeCargo(mission->getProductionGoodIndex(),
                                                                     this->dockTransferedAmount - delivered);
                            this->level->incNumDeliveredOre(this->dockTransferedAmount - delivered);
                        }
                    }
                }
            } else if (!mission->isEmpty() && (mission->getType() == 184 || mission->getType() == 168)
                       && dock->getDockingType() == 2) {
                int delivered = this->dockTransferedAmount;
                if (delivered != this->dockTotalAmount) {
                    this->dockTransferedAmount = (int) ((float) this->field_0x35c / 1500.0f);
                    if (this->dockTotalAmount <= this->dockTransferedAmount) {
                        if (delivered < this->dockTransferedAmount)
                            hud->hudEvent(mission->getType() == 184 || mission->getType() == 168 ? 36 : 40, this, 0);
                        this->dockTransferedAmount = this->dockTotalAmount;
                    }
                    if (this->dockTransferedAmount > delivered)
                        Globals::status->field_178 += this->dockTransferedAmount - delivered;
                }
            } else if (campaign != nullptr && !campaign->isEmpty()
                       && (campaign->getType() == 167 || campaign->getType() == 174) && dock->getDockingType() == 1) {
                int delivered = this->dockTransferedAmount;
                if (delivered != this->dockTotalAmount) {
                    this->dockTransferedAmount = (int) ((float) this->field_0x35c / 1000.0f);
                    if (this->dockTotalAmount <= this->dockTransferedAmount) {
                        if (delivered < this->dockTransferedAmount)
                            hud->hudEvent(42, this, 0);
                        this->dockTransferedAmount = this->dockTotalAmount;
                    }
                    if (this->dockTransferedAmount > delivered) {
                        Globals::status->getShip()->removeCargo(campaign->getProductionGoodIndex(),
                                                                 this->dockTransferedAmount - delivered);
                        campaign->setStatusValue(campaign->getStatusValue() + 1);
                    }
                }
            } else if (dock->getDockingType() == 3 && this->hackingGame == 0) {
                int dockIndex = 0;
                for (int i = 0; i < this->level->getNumDockingTargets(); i++) {
                    if (this->asteroidTarget == this->level->getDockingTarget(i))
                        dockIndex = i;
                }
                int campaignMission = Globals::status->getCurrentCampaignMission();
                this->hackingGame = (int) (intptr_t) new HackingGame(0, campaignMission == 91 ? 1 : 4, -1, -1, dockIndex);
                hud->setHackingGameActive(true);
            }
            this->field_0x35c += this->shakeIntensity;
        }
        if (this->turretActive || this->field_0x1a1)
            this->handleTurretView(dt);
    }

    float bank = 0.0f;
    if (this->autoPilot && this->autoPilotTarget != 0) {
        if (!this->field_0x145 && !this->updateManeuver()) {
            KIPlayer *target = (KIPlayer *) (intptr_t) this->autoPilotTarget;
            if (this->goingToWaypointFlag && ((Waypoint *) target)->route != nullptr) {
                target = (KIPlayer *) (intptr_t) ((Waypoint *) target)->route->getWaypoint();
                this->autoPilotTarget = (int) (intptr_t) target;
            }
            if (target == nullptr || this->dockedToStream) {
                this->setAutoPilot(nullptr);
            } else {
                this->moveTarget = target->getPosition();
                if ((int) AbyssEngine::AEMath::VectorLength(this->moveTarget - this->getPosition()) <= 19999)
                    this->aboutToReachAutoTargetFlag = 1;
                float speed = 4.0f;
                if (Globals::status->getShip()->getHandling() + 2.7f < 4.0f)
                    speed = Globals::status->getShip()->getHandling() + 2.7f;
                bank = this->moveToPosition(this->moveTarget, true, speed);
                if (this->turretActive)
                    this->handleTurretView(dt);
            }
        }
    } else if (!this->field_0x145) {
        if (this->turretActive || this->field_0x1a1) {
            this->handleTurretView(dt);
        } else {
            this->roll(this->shakeIntensity);
            if (!this->updateManeuver())
                this->handleShip(dt);
        }
    }

    if (this->dockedFlag && this->dockingPointIndex != 1 && !this->turretActive
        && (this->dockingPointIndex | 1) != 3)
        this->updateManeuver();

    if (this->autoTurretEquipped && this->autoTurretEnabled
        && (!this->field_0x354 || script->m_nScriptTime >= 10000)) {
        if (this->isDead()) {
            this->autoTurretEnabled = 0;
            this->player->stopShooting(2);
        } else {
            this->handleAutoTurret(dt);
        }
    }

    if (!(char &) this->dockingState && (unsigned int) (this->dockingPointIndex - 1) >= 2) {
        Matrix rotation;
        bool steered;
        if (this->autoPilotTarget == 0 || this->dockedToStream) {
            steered = false;
        } else {
            float step = this->handling * (float) dt * 0.012345679f;
            float yaw = (float &) this->field_0x280;
            if (yaw < bank) {
                yaw += step;
                (float &) this->field_0x280 = yaw;
                if (yaw > bank) {
                    yaw = bank;
                    (float &) this->field_0x280 = bank;
                }
            } else if (yaw > bank) {
                yaw -= step;
                (float &) this->field_0x280 = yaw;
                if (yaw < bank) {
                    yaw = bank;
                    (float &) this->field_0x280 = bank;
                }
            }
            steered = true;
            this->yawAccumF = yaw;
        }
        rotation = AbyssEngine::AEMath::MatrixSetRotation(
            rotation, (float) ((double) this->pitchAccumD * 0.25 * 0.000244140625 * 6.2831854820251465),
            this->yawAccumF * -0.000244140625f * 6.2831855f, 0.0f);
        this->field_0x4->setMatrix(rotation);
        if (steered) {
            reinterpret_cast<Matrix &>(this->player->transform[0]) =
                *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
                * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
        }

        float yaw = this->yawAccumF;
        if (Globals::mouseCursorActivated) {
            if (yaw > 0.0f && this->yawAccumDir == 0) {
                this->yawAccumF = yaw * 0.7f;
                if (this->yawAccumF < 0.0f)
                    this->yawAccumF = 0;
            } else if (yaw < 0.0f && this->yawAccumDir == 0) {
                this->yawAccumF = yaw * 0.7f;
                if (this->yawAccumF > 0.0f)
                    this->yawAccumF = 0;
            }
        } else if (yaw > 0.0f && this->yawAccumDir == 0) {
            this->yawAccumF = yaw + this->handling * (float) dt / -126.0f;
            if (this->yawAccumF < 0.0f)
                this->yawAccumF = 0;
        } else if (yaw < 0.0f && this->yawAccumDir == 0) {
            this->yawAccumF = yaw + this->handling * (float) dt / 126.0f;
            if (this->yawAccumF > 0.0f)
                this->yawAccumF = 0;
        }
        float pitch = this->pitchAccumD;
        if (pitch > 0.0f && this->pitchAccumDir == 0) {
            this->pitchAccumD = pitch + this->handling * (float) dt / -126.0f;
            if (this->pitchAccumD < 0.0f)
                this->pitchAccumD = 0;
        } else if (pitch < 0.0f && this->pitchAccumDir == 0) {
            this->pitchAccumD = pitch + this->handling * (float) dt / 126.0f;
            if (this->pitchAccumD > 0.0f)
                this->pitchAccumD = 0;
        }
        if (this->rocketControlGun != 0)
            this->targetFollowCamera->setRumblePercentage(0.2f, 70);
        this->pitchRamp = 0;
        this->yawRamp = 0;
        this->pitchAccumDir = 0;
        this->yawAccumDir = 0;
        this->yawAccumD = 0;
        this->rollAccum = 0;
    }

    if (Globals::status->getShip()->getShieldRegen() >= 1 && !this->isDead()) {
        this->shieldRegenTick += dt;
        if (this->shieldRegenTick > 100) {
            this->shieldRegenTick = 0;
            this->player->regenerateShield(this->shieldRegenRate);
        }
    }
    int repairType = Globals::status->getShip()->getRepairType();
    if (repairType >= 0 && !this->isDead()) {
        this->repairHPTick += dt;
        this->repairArmorTick += dt;
        if ((repairType == 0 ? 600.0f : 420.0f) < (float) this->repairHPTick) {
            this->repairHPTick = 0;
            if (this->player->getHitpoints() < this->player->getMaxHitpoints())
                this->player->regenerateHull();
        }
        if ((repairType == 0 ? 1000.0f : 700.0f) < (float) this->repairArmorTick) {
            this->repairArmorTick = 0;
            if (this->player->getHitpoints() >= this->player->getMaxHitpoints()
                && this->player->getArmorHP() < this->player->getMaxArmorHP())
                this->player->regenerateArmor();
        }
    }

    if (this->field_0xac != 0 && this->emergencySystemTimer >= 1) {
        AEGeometry *emergency = (AEGeometry *) (intptr_t) this->field_0xac;
        this->player->setVulnerable(false);
        this->emergencySystemTimer -= dt;
        emergency->setMatrix(reinterpret_cast<Matrix &>(this->player->transform[0]));
        ((AEGeometry *) (intptr_t) this->field_0xac)->translate(AbyssEngine::AEMath::MatrixRotateVector(
            reinterpret_cast<Matrix &>(this->player->transform[0]), this->emergencyVec));
        AbyssEngine::Mesh *mesh = Globals::Canvas->MeshGetPointer(((AEGeometry *) (intptr_t) this->field_0xac)->meshId);
        mesh->shaderAnimValue1 += (float) dt * 0.001f;
        float total = (float) this->emergencyVal1;
        float left = (float) this->emergencySystemTimer;
        float scale;
        if (total * 0.95f < left)
            scale = (float) (this->emergencyVal1 - this->emergencySystemTimer) / (total * 0.05f);
        else if (total * 0.05f > left)
            scale = left / (total * 0.05f);
        else
            scale = 1.0f;
        scale *= this->emergencyVal2;
        ((AEGeometry *) (intptr_t) this->field_0xac)->setScaling(scale, scale, scale);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
             ((AEGeometry *) (intptr_t) this->field_0xac)->transform))
            ->Update((long long) dt, false);
        if (this->emergencySystemTimer <= 0) {
            if (this->field_0xac != 0)
                delete (AEGeometry *) (intptr_t) this->field_0xac;
            this->field_0xac = 0;
            this->player->setVulnerable(true);
            Globals::sound->stop(1115);
            Globals::status->field_13c = 0;
        }
    }

    this->player->update(dt, this->isDead() ? false : this->field_0x1c != -1);
    if (this->shieldVisible && Globals::options[15] != 0 && this->engineSoundId != -1) {
        Vector position = this->player->getPosition();
        Globals::sound->updateEvent3DAttributes(this->engineSoundId, &position, nullptr, false);
    }
    vec_up = AbyssEngine::AEMath::MatrixGetUp(*(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform));
    if (this->boostingFlag) {
        ParticleSettings::Interpolate((ParticleSettings::ParticleSet) 4, (ParticleSettings::ParticleSet) 6,
                                      this->getBoostPercentage(), (ParticleSettings::ParticleSet) 4);
    } else {
        ParticleSettings &current = reinterpret_cast<ParticleSettings &>(ParticleSettingsRef::cur);
        uint32_t color = current.sets[4].color0;
        current.sets[4] = reinterpret_cast<ParticleSettings &>(ParticleSettingsRef::init).sets[4];
        current.sets[4].color0 = color;
    }

    if (this->route != 0) {
        int previous = ((Route *) (intptr_t) this->route)->getCurrent();
        ((Route *) (intptr_t) this->route)->update(this->dockOffsetVec);
        if (((Route *) (intptr_t) this->route)->getCurrent() != previous) {
            if (((Route *) (intptr_t) this->route)->loop || ((Route *) (intptr_t) this->route)->getCurrent() != 0)
                hud->hudEvent(23, this, 0);
            else
                hud->hudEvent(24, this, 0);
        }
    }

    this->field_0x20 = 0;
    this->field_0x30a = 0;
    if (Globals::mouseCursorActivated != this->field_0x378)
        this->field_0x84 = 1;
    if (this->lastHP > this->player->getCombinedHP() && !this->computerControlled && !this->freeze) {
        hud->playerHit();
        this->lastHP = this->player->getCombinedHP();
        this->hitCamera();
        if (this->player->shieldHit) {
            this->field_0x30a = 1;
            Globals::sound->play(25, nullptr, nullptr, 0.0f);
            this->player->shieldHit = 0;
        } else if (this->player->armorHit) {
            Globals::sound->play(23, nullptr, nullptr, 0.0f);
            this->player->armorHit = 0;
        } else if (this->player->hullHit) {
            Globals::sound->play(24, nullptr, nullptr, 0.0f);
            this->player->hullHit = 0;
        }
        Vector hitDirection = AbyssEngine::AEMath::VectorNormalize(this->player->getHitVector());
        Vector screen = {0.0f, 0.0f, 0.0f};
        Globals::Canvas->GetScreenPosition(hitDirection * 5000.0f + this->dockOffsetVec, screen);
        int side;
        if (!(screen.x > 0.0f))
            side = 1;
        else
            side = screen.x < (float) Globals::w ? 0 : 2;
        this->field_0x20 = side;
        if (screen.z < 0.0f) {
            this->field_0x20 = side | 16;
        } else {
            this->field_0x20 = side | 32;
            if (screen.x < -50000.0f)
                this->field_0x20 = 1;
            else if (screen.x < -15000.0f)
                this->field_0x20 = side | 33;
            if (screen.x > 50000.0f)
                this->field_0x20 = 2;
            else if (screen.x > 15000.0f)
                this->field_0x20 |= 2;
        }
    }

    Vector aimPosition = AbyssEngine::AEMath::MatrixGetPosition(reinterpret_cast<Matrix &>(this->player->transform[0]));
    Vector aimDirection = AbyssEngine::AEMath::MatrixGetDir(reinterpret_cast<Matrix &>(this->player->transform[0]));
    if (this->turretActive) {
        Matrix turret = this->turretRoot->getMatrix() * this->turretGeometry->getMatrix();
        aimPosition = AbyssEngine::AEMath::MatrixGetPosition(turret);
        aimDirection = AbyssEngine::AEMath::MatrixGetDir(turret);
    }
    if (Globals::mouseCursorActivated && !mouseLocked && controlMode != 3
        && (controlMode == 1 || !this->autoPilot)) {
        float centerX = (float) (Globals::w >> 1);
        float centerY = (float) (Globals::h >> 1);
        float rangeX = (float) ((double) centerX * 0.7);
        float rangeY = (float) ((double) centerY * 0.7);
        float mouseX = (float) Globals::mouseDeltaX;
        float mouseY = (float) Globals::mouseDeltaY;
        float clampedX = rangeX > mouseX ? mouseX : rangeX;
        float clampedY = rangeY > mouseY ? mouseY : rangeY;
        if (clampedX < -rangeX)
            clampedX = -rangeX;
        if (clampedY < -rangeY)
            clampedY = -rangeY;
        int deltaX = (int) clampedX;
        int deltaY = (int) clampedY;
        float maxX = centerX + rangeX;
        float maxY = centerY + rangeY;
        if (maxX > centerX + (float) deltaX)
            maxX = centerX + (float) deltaX;
        if (maxY > centerY + (float) deltaY)
            maxY = centerY + (float) deltaY;
        if (centerX - rangeX > maxX)
            maxX = centerX - rangeX;
        if (centerY - rangeY > maxY)
            maxY = centerY - rangeY;
        crosshairPos.x = maxX;
        Globals::mouseDeltaX = (int) clampedX;
        Globals::mouseDeltaY = (int) clampedY;
        crosshairPos.y = maxY;
        if (deltaX >= 1)
            this->right(dt, (float) deltaX / rangeX);
        else if (deltaX <= -1)
            this->left(dt, -(float) deltaX / rangeX);
        if (deltaY >= 1)
            this->down(dt, (float) deltaY / rangeY);
        else if (deltaY <= -1)
            this->up(dt, -(float) deltaY / rangeY);
        Globals::Canvas->GetScreenPosition(aimPosition + 22000.0f * AbyssEngine::AEMath::VectorNormalize(aimDirection),
                                           crosshairShootPos);
        crosshairShootPos = crosshairShootPos.z > 0.0f ? crosshairShootPos
                                                       : crosshairShootPos * 0.2f + this->lastCrosshairShootPos * 0.8f;
        this->lastCrosshairShootPos = crosshairShootPos;
    } else {
        Globals::Canvas->GetScreenPosition(aimPosition + 22000.0f * AbyssEngine::AEMath::VectorNormalize(aimDirection),
                                           crosshairPos);
        crosshairPos = crosshairPos.z > 0.0f ? crosshairPos
                                             : crosshairPos * 0.2f + this->lastCrosshairPos * 0.8f;
    }
    if (this->field_0x84 && !this->targetFollowCamera->isInFastForwardMode()) {
        if (Globals::mouseCursorActivated)
            this->targetFollowCamera->setShipHandling(this->handling);
        else
            this->targetFollowCamera->resetShipHandling();
        this->field_0x84 = 0;
    }
    this->field_0x378 = Globals::mouseCursorActivated != 0;
    this->lastCrosshairPos = crosshairPos;

    if (this->isDead() && this->explosion != nullptr) {
        this->player->stopShooting(0);
        if (this->tractorBeam != nullptr)
            Globals::sound->stop(0);
        if (this->repairBeams != nullptr)
            Globals::sound->stop(0);
        if (!this->dockedFlag || this->dockingPointIndex != 1)
            this->field_0x4->rotate(0.03f, 0.03f, 0.03f);
        if (this->explosionTimer <= 2999) {
            if (this->explosionTimer + this->shakeIntensity >= 3000) {
                this->explosion->start(this->geometry->getPosition(), Vector{0.0f, 0.0f, 0.0f});
                this->level->field_74->enableSystemEmit(this->currentSystem, false);
                this->level->field_74->enableSystemRender(this->currentSystem, false);
                this->level->field_74->emitManual(this->level->field_3c, aimPosition, 0, aimDirection, -1.0f);
            }
        } else if (this->explosionTimer != 3000) {
            this->explosion->update(this->shakeIntensity, this->geometry->getPosition());
        }
        if (this->explosion2 != nullptr)
            this->explosion2->update(this->shakeIntensity, this->geometry->getPosition());
        this->explosionTimer += this->shakeIntensity;
    }
    if (this->turretMode) {
        this->turretRoot->setMatrix(*(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
                                    * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform));
    }
}

void PlayerEgo::calcCollision(Array<KIPlayer *> *objects) {
    if (objects == nullptr)
        return;
    if (this->dockedFlag && (unsigned int) (this->dockingPointIndex - 1) < 3)
        return;
    for (unsigned int i = 0; i < objects->size(); i++) {
        KIPlayer *object = (*objects)[i];
        if (object == nullptr)
            continue;
        if (i == 0 && !Globals::status->inAlienOrbit()
            && AbyssEngine::AEMath::VectorLength(this->dockOffsetVec) < 16000.0f && object->field_0x71)
            this->collidesWithStationFlag = 1;
        if (!object->outerCollide(this->geometry->getPosition()))
            continue;
        if (object->getType() == 16994 && object->isVisible()) {
            if (((PlayerWormHole *) object)->isShrinking() || this->miningGame != 0)
                continue;
            Vector &pull = this->moveTarget;
            pull = object->getPosition();
            pull -= this->geometry->getPosition();
            int distance = (int) AbyssEngine::AEMath::VectorLength(pull);
            int strength = 40000 - distance;
            bool playing = Globals::sound->isPlaying(34);
            if (strength < 1) {
                if (playing)
                    Globals::sound->stop(34);
                continue;
            }
            if (playing) {
                Vector velocity = {0.0f, 0.0f, 0.0f};
                Vector position = object->getPosition();
                Globals::sound->updateEvent3DAttributes(34, &position, &velocity, false);
            } else {
                Globals::sound->play(34, nullptr, nullptr, 0.0f);
            }
            pull = AbyssEngine::AEMath::VectorNormalize(pull);
            Vector &drift = this->moveDirection;
            drift = Vector(pull);
            drift *= (float) (strength >> 8);
            this->geometry->translate(drift);
            this->hitCamera();
            if (distance < 1000)
                this->inWormhole = 1;
            reinterpret_cast<Matrix &>(this->player->transform[0]) =
                *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
                * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
            continue;
        }
        if (object->stealFlagByte) {
            if (object->isDying() || object->isDead())
                continue;
            if (((char &) this->dockingState || this->dockedFlag) && object == (KIPlayer *) (intptr_t) this->asteroidTarget)
                continue;
            Vector impact = object->getProjectionVector(this->geometry->getPosition());
            if (impact != Vector{0.0f, 0.0f, 0.0f}) {
                object->player->setHitVector(impact.x, impact.y, impact.z);
                object->player->setBombForce(this->boostingFlag ? 0.2f : 0.1f);
                object->player->damage(9999);
                this->player->damage(20);
                if (this->volatileGoods)
                    this->player->flShake += 0.2f;
                this->hitCamera();
            }
            continue;
        }
        if (!object->isVisible())
            continue;
        Vector surface = object->projectCollisionOnSurface(this->geometry->getPosition());
        if (surface != Vector{0.0f, 0.0f, 0.0f}) {
            this->geometry->setPosition(surface);
            reinterpret_cast<Matrix &>(this->player->transform[0]) =
                *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
                * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
            if (i == 0 && object->field_0x71)
                this->collidesWithStationFlag = 1;
        }
        this->hitCamera();
    }
}

unsigned char PlayerEgo::isDockingToPlanet() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->dockingToPlanet);
}

unsigned char PlayerEgo::isDockingToStream() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->dockingToStream);
}

void PlayerEgo::dockToAsteroid(KIPlayer *asteroid, Radar *radar) {
    if (((char &) this->dockingState) != 0) {
        ((PlayerAsteroid *) (((intptr_t) this->asteroidTarget)))->setRotationEnabled(true);
        ((char &) this->dockingState) = 0;
        this->field_0x145 = 0;
        this->asteroidTarget = 0;
        this->miningUpVec = Vector{0.0f, 0.0f, 0.0f};
        this->targetFollowCamera->setActive(true);
        this->player->resetGunDelay(0);
        if (this->miningGame != 0)
            delete (MiningGame *) ((intptr_t) this->miningGame);
        this->miningGame = 0;
        radar->unlockAsteroid();
        this->dockingPointIndex = 0;
        this->setExhaustVisible(true);
    } else if (asteroid != nullptr) {
        this->miningSettleTimer = 0;
        ((char &) this->dockingState) = 1;
        this->asteroidTarget = (int) (intptr_t) asteroid;
        this->dockScaling = (int) (((PlayerAsteroid *) asteroid)->getScaling() * 2500.0f);
        this->field_0x145 = 1;
        this->dockingPointIndex = 0;
    }
}

int PlayerEgo::approachAsteroid(Hud *hud, int /*unused*/, Radar * /*radar*/) {
    if (((KIPlayer *) (intptr_t) this->asteroidTarget)->isDying())
        return 0;
    if (this->dockingPointIndex == 0) {

        if (this->turretActive)
            this->setTurretMode(false);
        this->moveTarget = ((KIPlayer *) (intptr_t) this->asteroidTarget)->getPosition();
        this->moveTarget -= this->geometry->getPosition();
        int distance = (int) AbyssEngine::AEMath::VectorLength(this->moveTarget);
        if (this->dockScaling > distance) {
            if (this->field_0x1d4) {
                PlayerAsteroid *asteroid = (PlayerAsteroid *) (intptr_t) this->asteroidTarget;
                if (asteroid->stealFlagByte)
                    asteroid->setRotationEnabled(false);
                this->dockingPointIndex = 1;
            }
        } else {
            Vector target = ((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getPosition();
            float speed = 4.0f;
            if (Globals::status->getShip()->getHandling() + 2.7f < 4.0f)
                speed = Globals::status->getShip()->getHandling() + 2.7f;
            this->moveToPosition(target, true, speed);
            if ((int) AbyssEngine::AEMath::VectorLength(((KIPlayer *) (intptr_t) this->asteroidTarget)->getPosition()
                                                        - this->getPosition()) <= 19999)
                this->aboutToReachAutoTargetFlag = 1;
        }
        if (this->dockScaling + 2000 > distance) {
            this->thrust = 1.0f;
            if (!this->field_0x2f5) {
                this->setExhaustVisible(false);
                Globals::sound->play(2, nullptr, nullptr, 0.0f);
                this->boostTimer = this->boostTime - this->boostTime / 6;
                this->targetFollowCamera->setActive(false);
                this->miningUpVec = this->field_0x4->getUpVector();
                this->miningUpVec = AbyssEngine::AEMath::VectorNormalize(this->miningUpVec);
                this->field_0x2f5 = 1;
            }
            Vector &heading = this->moveDirection;
            heading = this->field_0x4->getDirection();
            heading = AbyssEngine::AEMath::VectorNormalize(heading);
            if (AbyssEngine::AEMath::ACosf(AbyssEngine::AEMath::VectorDot(heading, this->miningUpVec)) > 0.2f
                && (float &) this->miningSettleTimer > -1024.0f) {
                (float &) this->miningSettleTimer += (float) this->shakeIntensity * -0.35f;
                Matrix pitch;
                AbyssEngine::AEMath::MatrixSetRotation(pitch, (float &) this->miningSettleTimer * (1.0f / 65536.0f) * 6.2831855f,
                                                       0.0f, 0.0f);
                Matrix local = *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
                local *= pitch;
                this->field_0x4->setMatrix(local);
            } else {
                this->field_0x1d4 = 1;
                this->rolling = 0;
                this->field_0x2f5 = 0;
            }
        }
    } else if (this->dockingPointIndex == 1) {
        if ((float &) this->miningSettleTimer > -1024.0f) {
            (float &) this->miningSettleTimer += (float) (-this->shakeIntensity >> 1);
            return 0;
        }
        if (this->miningGame == 0) {
            this->lostMiningGameFlag = 0;
            this->field_0x2f5 = 0;
            Globals::hints[55] = 0;
            this->miningGame = (int) (intptr_t) new MiningGame(((PlayerAsteroid *) (intptr_t) this->asteroidTarget)->getQuality(),
                                                               ((PlayerAsteroid *) (intptr_t) this->asteroidTarget)->asteroidIndex,
                                                               hud);
            ((PlayerAsteroid *) (intptr_t) this->asteroidTarget)->player->radius = 0;
            Globals::sound->play(1, nullptr, nullptr, 0.0f);
            Globals::sound->pause(this->field_0x1c);
            return 0;
        }
        if (((MiningGame *) (intptr_t) this->miningGame)->update(this->shakeIntensity)) {
            if (((KIPlayer *) (intptr_t) this->asteroidTarget)->isDying()
                || ((KIPlayer *) (intptr_t) this->asteroidTarget)->isDead()) {
                Globals::status->field_124 = 0;
                this->stopMining();
                hud->hudEvent(8, this, 0);
                return 1;
            }
            return 0;
        }
        if (!((MiningGame *) (intptr_t) this->miningGame)->gameLost()
            && ((MiningGame *) (intptr_t) this->miningGame)->getOreAmount() >= 1) {
            this->stopMining();
            return 1;
        }
        if (((MiningGame *) (intptr_t) this->miningGame)->gameLost()) {
            this->lostMiningGameFlag = 1;
            Globals::status->field_124 = 0;
            this->stopMining();
            hud->hudEvent(8, this, 0);
        }
        return 1;
    }
    return 0;
}

void PlayerEgo::dockToDockingPoint(KIPlayer *station, Radar *radar) {
    if (this->isDead())
        return;

    if (!this->dockedFlag) {
        if (station) {
            this->asteroidTarget = (int) (intptr_t) station;
            this->dockedFlag = 1;
            this->miningSettleTimer = 0;
            this->dockScaling = 1400;
            this->field_0x145 = 1;
            this->dockingPointIndex = 0;
        }
        return;
    }

    if (station
        && station->getNearestNavigationPoint(this->getPosition(), (SpacePoint *) (intptr_t) this->spacePoint)) {
        if (station->field_0x70)
            station->field_0x8c = 1;
        {
            this->setTurretMode(false);
            this->field_0x1a1 = 0;
            Globals::Canvas->CameraSetCurrent(this->targetFollowCamera->id);
            this->levelScript->resetCamera(this->level);
            this->PlayEngineSound();
            this->dockingPointIndex = 3;
            this->targetFollowCamera->setLookAtCam(true);
            this->targetFollowCamera->useTargetsUpVector(false);
            delete this->easeMatrix;
            this->easeMatrix = nullptr;
            SpacePoint *dockPoint =
                station->getNearestNavigationPoint(this->getPosition(), (SpacePoint *) (intptr_t) this->spacePoint);
            Matrix current = this->geometry->getMatrix();
            AbyssEngine::AEMath::MatrixSetTranslation(current, dockPoint->position.x, dockPoint->position.y,
                                                      dockPoint->position.z);
            this->easeMatrix = new AbyssEngine::EaseInOutMatrix(this->geometry->getMatrix(), current, 3000);
            this->setExhaustVisible(true);
        }
    } else {
        if (station && station->field_0x70)
            station->field_0x8c = 1;
        this->PlayEngineSound();
        this->field_0x1a1 = 0;
        this->dockedFlag = 0;
        this->field_0x145 = 0;
        this->asteroidTarget = 0;
        this->miningUpVec = Vector{0.0f, 0.0f, 0.0f};
        this->targetFollowCamera->setActive(true);
        this->targetFollowCamera->setLookAtCam(false);
        this->targetFollowCamera->useTargetsUpVector(false);
        this->player->resetGunDelay(0);
        this->dockingPointIndex = 0;
        this->setExhaustVisible(true);
        if (this->spacePoint) {
            ((SpacePoint *) (intptr_t) this->spacePoint)->giveFree();
            this->spacePoint = 0;
        }
    }

    if (this->hackingGame) {
        delete (HackingGame *) (intptr_t) this->hackingGame;
        this->hackingGame = 0;
        this->hud->setHackingGameActive(false);
    }
}

bool PlayerEgo::approachDockingPoint(Hud *hud, int /*unused*/, Radar *radar) {
    KIPlayer *station = (KIPlayer *) (intptr_t) this->asteroidTarget;
    if (station->isDying())
        return false;
    switch (this->dockingPointIndex) {
        case 0: {
            SpacePoint *approach = ((KIPlayer *) (intptr_t) this->asteroidTarget)
                                       ->getNearestNavigationPoint(this->getPosition(), (SpacePoint *) (intptr_t) this->spacePoint);
            if (approach == nullptr)
                return false;
            if ((SpacePoint *) (intptr_t) this->spacePoint != approach) {
                if (this->spacePoint != 0)
                    ((SpacePoint *) (intptr_t) this->spacePoint)->giveFree();
                this->spacePoint = (int) (intptr_t) approach;
                approach->take();
            }
            KIPlayer *dockStation = (KIPlayer *) (intptr_t) this->asteroidTarget;
            this->moveTarget = dockStation->getPosition()
                                           + AbyssEngine::AEMath::MatrixRotateVector(
                                               ((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getMatrix(),
                                               approach->position);
            int distance = (int) AbyssEngine::AEMath::VectorLength(this->moveTarget - this->getPosition());
            if (this->dockScaling > distance) {
                KIPlayer *target = (KIPlayer *) (intptr_t) this->asteroidTarget;
                if (target->field_0x70)
                    target->field_0x8c = 0;
                this->dockingPointIndex = 2;
                this->targetFollowCamera->setLookAtCam(true);
                this->targetFollowCamera->useTargetsUpVector(false);
                if (this->easeMatrix != nullptr)
                    delete this->easeMatrix;
                this->easeMatrix = nullptr;
                KIPlayer *dock = (KIPlayer *) (intptr_t) this->asteroidTarget;
                SpacePoint *port = dock->getNearestDockingPoint(
                    dock->getPosition()
                    + AbyssEngine::AEMath::MatrixRotateVector(((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getMatrix(),
                                                              ((SpacePoint *) (intptr_t) this->spacePoint)->position));
                Vector dir = AbyssEngine::AEMath::VectorNormalize(AbyssEngine::AEMath::MatrixRotateVector(
                    ((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getMatrix(), port->direction));
                Vector up = AbyssEngine::AEMath::VectorNormalize(
                    AbyssEngine::AEMath::MatrixRotateVector(((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getMatrix(),
                                                            ((SpacePoint *) (intptr_t) this->spacePoint)->position)
                    - AbyssEngine::AEMath::MatrixRotateVector(
                        ((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getMatrix(), port->position));
                Vector right = AbyssEngine::AEMath::VectorNormalize(AbyssEngine::AEMath::VectorCross(up, dir));
                Matrix target2;
                AbyssEngine::AEMath::MatrixSetRotation(target2, right, up, dir);
                this->easeMatrix = new AbyssEngine::EaseInOutMatrix(this->geometry->getMatrix(), target2, 2000);
                Globals::sound->play(2270, nullptr, nullptr, 0.0f);
                Vector velocity = {0.0f, 0.0f, 0.0f};
                Vector position = this->player->getPosition();
                Globals::sound->updateEvent3DAttributes(2270, &position, &velocity, false);
                this->StopEngineSound();
                return false;
            }
            float speed = 4.0f;
            if (Globals::status->getShip()->getHandling() + 2.7f < 4.0f)
                speed = Globals::status->getShip()->getHandling() + 2.7f;
            this->moveToPosition(this->moveTarget, true, speed);
            if (distance <= 19999)
                this->aboutToReachAutoTargetFlag = 1;
            return false;
        }
        case 1:
            return true;
        case 2: {
            this->aboutToReachAutoTargetFlag = 1;
            if (this->turretActive)
                this->setTurretMode(false);
            KIPlayer *dock = (KIPlayer *) (intptr_t) this->asteroidTarget;
            SpacePoint *port = dock->getNearestDockingPoint(
                dock->getPosition()
                + AbyssEngine::AEMath::MatrixRotateVector(((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getMatrix(),
                                                          ((SpacePoint *) (intptr_t) this->spacePoint)->position));
            this->moveTarget =
                ((KIPlayer *) (intptr_t) this->asteroidTarget)->getPosition()
                + AbyssEngine::AEMath::MatrixRotateVector(((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getMatrix(),
                                                          port->position);
            float distance = AbyssEngine::AEMath::VectorLength(this->moveTarget - this->getPosition());
            if (kShipDockRadius[Globals::status->getShip()->getIndex()] > (int) distance) {
                Mission *mission = Globals::status->getMission();
                Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
                PlayerFixedObject *fixed = (PlayerFixedObject *) (intptr_t) this->asteroidTarget;
                if (!mission->isEmpty() && mission->getType() == 15 && fixed->getDockingType() == 1) {
                    if (Globals::status->getShip()->hasCargo(mission->getProductionGoodIndex(), 1)) {
                        this->dockTotalAmount =
                            Globals::status->getShip()->getCargo(mission->getProductionGoodIndex())->getAmount();
                        int remaining = mission->getProductionGoodAmount() - this->level->getNumDeliveredOre();
                        if (this->dockTotalAmount > remaining)
                            this->dockTotalAmount = remaining;
                        this->dockTransferedAmount = 0;
                        this->field_0x35c = 0;
                        if (this->dockTotalAmount >= 1)
                            hud->hudEvent(41, this, 0);
                    }
                } else if (!mission->isEmpty() && (mission->getType() == 184 || mission->getType() == 168)
                           && fixed->getDockingType() == 2) {
                    if ((Globals::status->getShip()->getMaxPassengers() >= 1
                         && Globals::status->field_178 < Globals::status->getShip()->getMaxPassengers())
                        || mission->getType() == 168) {
                        int capacity = mission->getType() == 168 ? mission->getStatusValue()
                                                                 : Globals::status->getShip()->getMaxPassengers();
                        this->dockTotalAmount = capacity - Globals::status->field_178;
                        if (this->dockTotalAmount > mission->getStatusValue() - Globals::status->field_178)
                            this->dockTotalAmount = mission->getStatusValue() - Globals::status->field_178;
                        this->dockTransferedAmount = 0;
                        this->field_0x35c = 0;
                        if (this->dockTotalAmount >= 1)
                            hud->hudEvent(35, this, 0);
                    } else if (Globals::status->getShip()->getMaxPassengers() == 0) {
                        hud->hudEvent(43, this, 0);
                    }
                } else if (!mission->isEmpty() && mission->getType() == 184 && fixed->getDockingType() == 1) {
                    if (Globals::status->field_178 >= 1) {
                        this->field_0x35c = 0;
                        this->dockTotalAmount = Globals::status->field_178;
                        this->dockTransferedAmount = 0;
                        hud->hudEvent(37, this, 0);
                    }
                } else if (campaign != nullptr && !campaign->isEmpty()
                           && (campaign->getType() == 167 || campaign->getType() == 174) && fixed->getDockingType() == 1
                           && Globals::status->getShip()->hasCargo(campaign->getProductionGoodIndex(), 1)) {
                    this->dockTotalAmount =
                        Globals::status->getShip()->getCargo(campaign->getProductionGoodIndex())->getAmount();
                    int remaining = campaign->getProductionGoodAmount() - campaign->getStatusValue();
                    if (this->dockTotalAmount > remaining)
                        this->dockTotalAmount = remaining;
                    this->dockTransferedAmount = 0;
                    this->field_0x35c = 0;
                    if (this->dockTotalAmount >= 1)
                        hud->hudEvent(41, this, 0);
                }
                this->dockingPointIndex = 1;
                if (!this->field_0xb2)
                    this->switchToStandardCam = 1;
                this->field_0xb2 = 0;
                return true;
            }
            this->easeMatrix->Increase((float) this->shakeIntensity);
            Matrix eased = this->easeMatrix->GetValue();
            AbyssEngine::AEMath::MatrixSetTranslation(
                eased, this->getPosition()
                           + (this->moveTarget - this->getPosition()) * (float) this->shakeIntensity * 0.0006f);
            this->field_0x4->setMatrix(Matrix());
            this->geometry->setMatrix(eased);
            reinterpret_cast<Matrix &>(this->player->transform[0]) = this->geometry->getMatrix();
            this->dockOffsetVec = this->geometry->getPosition();
            return false;
        }
        case 3: {
            SpacePoint *exit = ((KIPlayer *) (intptr_t) this->asteroidTarget)
                                   ->getNearestNavigationPoint(this->getPosition(), (SpacePoint *) (intptr_t) this->spacePoint);
            this->moveTarget =
                ((KIPlayer *) (intptr_t) this->asteroidTarget)->getPosition()
                + AbyssEngine::AEMath::MatrixRotateVector(((KIPlayer *) (intptr_t) this->asteroidTarget)->geometry->getMatrix(),
                                                          exit->position);
            if ((int) AbyssEngine::AEMath::VectorLength(this->moveTarget - this->getPosition()) < 200) {
                this->field_0x145 = 0;
                this->dockedFlag = 0;
                this->asteroidTarget = 0;
                radar->lockedEnemy = nullptr;
                radar->dockNavPtr = nullptr;
                this->miningUpVec = Vector{0.0f, 0.0f, 0.0f};
                this->targetFollowCamera->setActive(true);
                this->targetFollowCamera->setLookAtCam(false);
                this->targetFollowCamera->useTargetsUpVector(false);
                this->levelScript->resetCamera(this->level);
                this->player->resetGunDelay(0);
                this->dockingPointIndex = 0;
                this->field_0xb0 = 1;
                if (this->spacePoint != 0) {
                    ((SpacePoint *) (intptr_t) this->spacePoint)->giveFree();
                    this->spacePoint = 0;
                }
                if (this->hackingGame != 0) {
                    delete (HackingGame *) (intptr_t) this->hackingGame;
                    this->hackingGame = 0;
                    hud->setHackingGameActive(false);
                }
                return false;
            }
            this->easeMatrix->Increase((float) this->shakeIntensity);
            Matrix eased = this->easeMatrix->GetValue();
            AbyssEngine::AEMath::MatrixSetTranslation(
                eased, this->getPosition()
                           + (this->moveTarget - this->getPosition()) * (float) this->shakeIntensity * 0.0012f);
            this->field_0x4->setMatrix(Matrix());
            this->geometry->setMatrix(eased);
            reinterpret_cast<Matrix &>(this->player->transform[0]) = this->geometry->getMatrix();
            this->dockOffsetVec = this->geometry->getPosition();
            return false;
        }
        default:
            return false;
    }
}

bool PlayerEgo::isDockingToDockingPoint() {
    if (this->dockedFlag != 0) return this->dockingPointIndex != 1;
    return false;
}

bool PlayerEgo::isLandingOrTakingOff() {
    if (this->dockedFlag == 0) return false;
    return (((uint32_t &) this->dockingPointIndex) | 1) == 3;
}

void PlayerEgo::hitCamera() {
    this->hitShake = 1;
    TargetFollowCamera *cam = this->targetFollowCamera;
    this->hitShakeTimer = 0;
    cam->hit();
}

void PlayerEgo::right(int frameTime, float delta) {
    if (this->miningGame != 0) {
        ((MiningGame *) (intptr_t) this->miningGame)->right(delta);
        return;
    }
    if (this->turretActive != 0) {
        float angle = (float) frameTime * delta / (200.0f / (float) this->turretPitch) * 0.000244140625f
                      * -6.2831855f;
        this->dockCameraNode->rotate(0.0f, angle, 0.0f);
        this->turretBaseGeo->rotate(0.0f, angle, 0.0f);
        this->turretGeometry->rotate(0.0f, angle, 0.0f);
        return;
    }
    if (this->rocketControlGun != 0) {
        this->rocketYaw = delta * -0.003f * ((Gun *) (intptr_t) this->rocketControlGun)->pitchRate;
        this->rocketBanking += (float) frameTime * delta * 0.01f;
        return;
    }
    if (this->autoPilot != 0)
        return;
    if (this->dockedFlag != 0 && this->dockingPointIndex != 1)
        return;

    this->yawAccumDir = -1;
    float rate;
    if (this->hardCoreMode != 0) {
        float load = (float) Globals::status->getShip()->getCurrentLoad()
                     / (float) Globals::status->getShip()->getMaxLoad();
        rate = this->handling * (1.0f - load) * 0.4f + this->handling * 0.6f;
    } else {
        rate = this->handling;
    }
    float target = (float) ((int) (delta * -750.0f * rate) / 63);
    this->rollAccum = delta;
    this->yawRamp = -this->handling;
    if (this->yawAccumF > target) {
        float step;
        if (Globals::mouseCursorActivated) {
            step = PE_minf(rate * 25.0f / 5.999999f, 280.0f);
        } else {
            OptionsRecord *options = (OptionsRecord *) Globals::options;
            const float &sensitivity = options->flag_0x11 != 0
                                           ? reinterpret_cast<const float &>(options->field_0x14)
                                           : reinterpret_cast<const float &>(options->field_0x18[0]);
            step = rate * (float) frameTime / ((3.3f - sensitivity) * 20.0f);
        }
        float yaw = this->yawAccumF - step;
        if (yaw < target)
            yaw = target;
        this->yawAccumF = yaw;
    }
}

void PlayerEgo::left(int frameTime, float delta) {
    if (this->miningGame != 0) {
        ((MiningGame *) (intptr_t) this->miningGame)->left(-delta);
        return;
    }
    if (this->turretActive != 0) {
        float angle = (float) frameTime * delta / (200.0f / (float) this->turretPitch) * 0.000244140625f
                      * 6.2831855f;
        this->dockCameraNode->rotate(0.0f, angle, 0.0f);
        this->turretBaseGeo->rotate(0.0f, angle, 0.0f);
        this->turretGeometry->rotate(0.0f, angle, 0.0f);
        return;
    }
    if (this->rocketControlGun != 0) {
        this->rocketYaw = delta * 0.003f * ((Gun *) (intptr_t) this->rocketControlGun)->pitchRate;
        this->rocketBanking += (float) frameTime * delta * -0.01f;
        return;
    }
    if (this->autoPilot != 0)
        return;
    if (this->dockedFlag != 0 && this->dockingPointIndex != 1)
        return;

    this->yawAccumDir = 1;
    float rate;
    if (this->hardCoreMode != 0) {
        float load = (float) Globals::status->getShip()->getCurrentLoad()
                     / (float) Globals::status->getShip()->getMaxLoad();
        rate = this->handling * (1.0f - load) * 0.4f + this->handling * 0.6f;
    } else {
        rate = this->handling;
    }
    float target = (float) ((int) (delta * 750.0f * rate) / 63);
    this->yawRamp = rate;
    this->rollAccum = -delta;
    if (this->yawAccumF < target) {
        float step;
        if (Globals::mouseCursorActivated) {
            step = PE_minf(rate * 25.0f / 5.999999f, 280.0f);
        } else {
            OptionsRecord *options = (OptionsRecord *) Globals::options;
            const float &sensitivity = options->flag_0x11 != 0
                                           ? reinterpret_cast<const float &>(options->field_0x14)
                                           : reinterpret_cast<const float &>(options->field_0x18[0]);
            step = rate * (float) frameTime / ((3.3f - sensitivity) * 20.0f);
        }
        float yaw = this->yawAccumF + step;
        if (yaw > target)
            yaw = target;
        this->yawAccumF = yaw;
    }
}

void PlayerEgo::down(int frameTime, float delta) {
    if (this->miningGame != 0) {
        OptionsRecord *options = (OptionsRecord *) Globals::options;
        if (options->field_0x10 != 0)
            ((MiningGame *) (intptr_t) this->miningGame)->down(-delta);
        else
            ((MiningGame *) (intptr_t) this->miningGame)->up(delta);
        return;
    }
    if (this->turretActive != 0) {
        if (this->lookYaw < 70.0f) {
            float step = (float) frameTime * delta;
            this->lookYaw = step + this->lookYaw;
            this->turretGeometry->rotate(step * 0.000244140625f * 6.2831855f, 0.0f, 0.0f);
        }
        if (this->lookPitch < 70.0f) {
            float step = (float) frameTime * delta * 0.5f;
            this->lookPitch = step + this->lookPitch;
            this->dockCameraMid->rotate(step * 0.000244140625f * -6.2831855f, 0.0f, 0.0f);
        }
        return;
    }
    if (this->rocketControlGun != 0) {
        this->rocketPitch = delta * 0.003f * ((Gun *) (intptr_t) this->rocketControlGun)->pitchRate;
        return;
    }
    if (this->autoPilot != 0)
        return;
    if (this->dockedFlag != 0 && this->dockingPointIndex != 1)
        return;

    this->pitchAccumDir = -1;
    float rate;
    if (this->hardCoreMode != 0) {
        float load = (float) Globals::status->getShip()->getCurrentLoad()
                     / (float) Globals::status->getShip()->getMaxLoad();
        rate = this->handling * (1.0f - load) * 0.4f + this->handling * 0.6f;
    } else {
        rate = this->handling;
    }
    float target = (float) ((int) (delta * 750.0f * rate) / 63);
    this->yawAccumD = delta;
    this->pitchRamp = -rate;
    if (this->pitchAccumD < target) {
        float divisor;
        if (Globals::mouseCursorActivated) {
            divisor = 11.999998f;
        } else {
            OptionsRecord *options = (OptionsRecord *) Globals::options;
            float primary = reinterpret_cast<const float &>(options->field_0x14);
            float secondary = reinterpret_cast<const float &>(options->field_0x18[0]) * 1.45f;
            float sensitivity = options->flag_0x11 != 0 ? primary : secondary;
            divisor = (3.3f - sensitivity) * 20.0f;
        }
        float pitch = this->pitchAccumD + rate * (float) frameTime / divisor;
        if (pitch > target)
            pitch = target;
        this->pitchAccumD = pitch;
    }
}

void PlayerEgo::up(int frameTime, float delta) {
    if (this->miningGame != 0) {
        OptionsRecord *options = (OptionsRecord *) Globals::options;
        if (options->field_0x10 != 0)
            ((MiningGame *) (intptr_t) this->miningGame)->up(delta);
        else
            ((MiningGame *) (intptr_t) this->miningGame)->down(-delta);
        return;
    }
    if (this->turretActive != 0) {
        if (this->lookYaw > -500.0f) {
            float step = (float) frameTime * delta;
            this->lookYaw = this->lookYaw - step;
            this->turretGeometry->rotate(step * 0.000244140625f * -6.2831855f, 0.0f, 0.0f);
        }
        if (this->lookPitch > -250.0f) {
            float step = (float) frameTime * delta * 0.5f;
            this->lookPitch = this->lookPitch - step;
            this->dockCameraMid->rotate(step * 0.000244140625f * 6.2831855f, 0.0f, 0.0f);
        }
        return;
    }
    if (this->rocketControlGun != 0) {
        this->rocketPitch = delta * -0.003f * ((Gun *) (intptr_t) this->rocketControlGun)->pitchRate;
        return;
    }
    if (this->autoPilot != 0)
        return;
    if (this->dockedFlag != 0 && this->dockingPointIndex != 1)
        return;

    this->pitchAccumDir = 1;
    float rate;
    if (this->hardCoreMode != 0) {
        float load = (float) Globals::status->getShip()->getCurrentLoad()
                     / (float) Globals::status->getShip()->getMaxLoad();
        rate = this->handling * (1.0f - load) * 0.4f + this->handling * 0.6f;
    } else {
        rate = this->handling;
    }
    float target = (float) ((int) (delta * -750.0f * rate) / 63);
    this->pitchRamp = rate;
    this->yawAccumD = -delta;
    if (this->pitchAccumD > target) {
        float divisor;
        if (Globals::mouseCursorActivated) {
            divisor = 11.999998f;
        } else {
            OptionsRecord *options = (OptionsRecord *) Globals::options;
            float primary = reinterpret_cast<const float &>(options->field_0x14);
            float secondary = reinterpret_cast<const float &>(options->field_0x18[0]) * 1.25f;
            float sensitivity = options->flag_0x11 != 0 ? primary : secondary;
            divisor = (3.3f - sensitivity) * 20.0f;
        }
        float pitch = this->pitchAccumD - rate * (float) frameTime / divisor;
        if (pitch < target)
            pitch = target;
        this->pitchAccumD = pitch;
    }
}

int PlayerEgo::tryToStartEmergencySystem() {
    if (this->field_0xac == 0) return 0;
    if (this->emergencySystemTimer != 0) return 0;
    if (((Player *) (this->player))->getHitpoints() > 1) return 0;
    ((Player *) (this->player))->setHitpoints(1);
    this->emergencySystemTimer = this->emergencyVal1;
    ((Player *) (this->player))->setVulnerable(0);
    Ship *s1 = Globals::status->getShip();
    Item *eq = ((Ship *) (Globals::status->getShip()))->getFirstEquipmentOfSort(0x1b);
    ((Ship *) (s1))->removeEquipment(eq);
    ((FModSound *) (Globals::sound))->play(0x45b, (Vector *) 0, (Vector *) 0, 0.0f);
    return 1;
}

bool PlayerEgo::emergencySystemActive() {
    PlayerEgo *self = this;
    return 0 < this->emergencySystemTimer;
}

unsigned char PlayerEgo::collidesWithStation() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->collidesWithStationFlag);
}

bool PlayerEgo::isHacking() {
    PlayerEgo *self = this;
    return this->hackingGame != 0;
}

int PlayerEgo::hackingWon() {
    int v = this->hackingGame;
    if (v == 0) return 0;
    return ((HackingGame *) v)->gameWon();
}

void PlayerEgo::deleteHackingGame() {
    HackingGame *g = (HackingGame *) ((intptr_t) this->hackingGame);
    if (g != 0) delete g;
    this->hackingGame = 0;
}

int PlayerEgo::getHackingGameDockIndex() {
    int v = this->hackingGame;
    if (v == 0) return -1;
    return ((HackingGame *) v)->getDockingIndex();
}

bool PlayerEgo::isInDockingProcedure() {
    if (((char &) this->dockingState) != 0) return true;
    return this->dockedFlag != 0;
}

void PlayerEgo::turnVertical(int a, float v) {
    if (v < -0.0f) {
        down(a, v);
    } else if (v > 0.0f) {
        up(a, v);
    }
}

void PlayerEgo::turnHorizontal(int a, float v) {
    if (v < 0.0f) {
        left(a, v);
    } else if (v > 0.0f) {
        right(a, v);
    }
}

float PlayerEgo::getRocketBanking() {
    PlayerEgo *self = this;
    return this->rocketBanking;
}

void PlayerEgo::strafe(int /*dir*/, bool positive) {
    if (this->rocketControlGun != 0)
        return;

    float base;
    if (this->hardCoreMode != 0) {
        float cur = (float) ((Ship *) (Globals::status->getShip()))->getCurrentLoad();
        float max = (float) ((Ship *) (Globals::status->getShip()))->getMaxLoad();
        float rate = this->handling;
        base = rate * (1.0f - cur / max) * g_PE_strafeLoadK + rate * g_PE_strafeLoadB;
    } else {
        base = this->handling;
    }

    float accel = (positive ? 1.0f : -1.0f) * PE_minf(base * 30.0f * g_PE_strafeAccelK, 2.0f);
    this->strafeAccel = this->strafeFactor * accel;
    this->strafeFactor = PE_minf(this->strafeFactor * 1.5f, 1.0f);
}

void PlayerEgo::hackingRotateRCW() {
    if (this->hackingGame != 0 && ((HackingGame *) (this->hackingGame))->isRotating() == 0
        && ((HackingGame *) (this->hackingGame))->gameWon() == 0)
        ((HackingGame *) this->hackingGame)->rotateRightCW(true);
}

void PlayerEgo::hackingRotateLCW() {
    if (this->hackingGame != 0 && ((HackingGame *) (this->hackingGame))->isRotating() == 0
        && ((HackingGame *) (this->hackingGame))->gameWon() == 0)
        ((HackingGame *) this->hackingGame)->rotateLeftCW(true);
}

void PlayerEgo::hackingShuffle() {
    int v = this->hackingGame;
    if (v != 0) ((HackingGame *) v)->reInit();
}

void PlayerEgo::alignToHorizon() {
    PlayerEgo *self = this;
    ((uint8_t &) this->rolling) = 1;
}

void PlayerEgo::rotate(float rx, float ry, float rz) {
    this->rotateX = this->rotateX + rx;
    this->rotateY = this->rotateY + ry;
    this->rotateZ = this->rotateZ + rz;
    AbyssEngine::AEMath::Matrix &m =
        *(AbyssEngine::AEMath::Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
    AbyssEngine::AEMath::MatrixSetRotation(m, this->rotateX, this->rotateY, this->rotateZ);
}

void PlayerEgo::setRotation(float rx, float ry, float rz) {
    this->rotateX = rx;
    this->rotateY = ry;
    this->rotateZ = rz;
    AbyssEngine::AEMath::Matrix &m =
        *(AbyssEngine::AEMath::Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
    AbyssEngine::AEMath::MatrixSetRotation(m, this->rotateX, this->rotateY, this->rotateZ);
}

void PlayerEgo::explode() {
    ((ParticleSystemManager *) (this->level->field_74))->enableSystemEmit(this->currentSystem, 1);
    if (((int &) this->explosion) != 0) return;
    this->targetFollowCamera->setActive(0);
    Explosion *e = new Explosion(0);
    ((int &) this->explosion) = (int) (intptr_t) e;
    static_cast<Player *>(this->player)->setActive(false);
    Globals::sound->stop(Globals::sound->currentMusicEvent);
    Globals::sound->stop(this->field_0x1c);
    Globals::sound->stop(0x1b);
    Globals::sound->stop(0x23);
    Globals::sound->stop(0x8d5);
    Globals::sound->stop(0x8d4);
    Globals::sound->stop(0x8cc);
    Globals::sound->stop(0x447);
    Globals::sound->stop(0x448);
    Globals::sound->stop(0x449);
    this->setExhaustVisible(false);
}

void PlayerEgo::revive() {
    this->level->field_74->enableSystemEmit(this->currentSystem, false);
    this->level->field_74->enableSystemRender(this->currentSystem, false);

    if (this->explosion != nullptr)
        delete this->explosion;
    this->explosion = nullptr;

    this->targetFollowCamera->setActive(true);
    this->player->setActive(true);
    Globals::sound->play(Globals::sound->currentMusicEvent, nullptr, nullptr, 0.0f);
    Globals::sound->play(this->field_0x1c, nullptr, nullptr, 0.0f);
    this->setExhaustVisible(true);
    this->player->setHitpoints(this->player->getMaxHitpoints());
    this->player->setArmorHP(this->player->getMaxArmorHP());
    this->setPosition(0.0f, 0.0f, 10000.0f);
    this->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
    this->explosionTimer = 0;
}

void PlayerEgo::startSmokeEmission() {
    int v = this->smokeSystem;
    if (v < 0) return;
    ((ParticleSystemManager *) (this->level->particleEmitBoolPtr))->enableSystemEmit(v, 1);
    ((ParticleSystemManager *) (intptr_t) this->level->particleRenderBoolPtr)->enableSystemEmit(
        this->explosionSmoke, true);
}

void PlayerEgo::endExplosion() {
    int v = ((int &) this->explosion);
    if (v != 0) ((Explosion *) v)->reset();
}

bool PlayerEgo::explosionEnded() {
    if (((int &) this->explosion) == 0) return true;
    return 8000 < this->explosionTimer;
}

unsigned char PlayerEgo::aboutToReachAutoTarget() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->aboutToReachAutoTargetFlag);
}

void PlayerEgo::resetGunDelay() {
    ((Player *) this->player)->resetGunDelay(0);
}

void PlayerEgo::refillGunDelay() {
    ((Player *) this->player)->refillGunDelay(0);
}

bool PlayerEgo::isDockedToAsteroid() {
    if (((char &) this->dockingState) == 0) return false;
    return this->dockingPointIndex == 1;
}

bool PlayerEgo::isDockingToAsteroid() {
    if (((char &) this->dockingState) != 0) return this->dockingPointIndex != 1;
    return false;
}

bool PlayerEgo::isDockedToMiningPlant() {
    if (this->dockedFlag != 0 && this->dockingPointIndex == 1) {
        if (((Mission *) (Globals::status->getMission()))->isEmpty() != 0
            && Globals::status->inAlienOrbit() == 0) {
            return Globals::status->getStation()->getIndex() == 0x67;
        }
    }
    return false;
}

unsigned char PlayerEgo::isDockedToStream() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->dockedToStream);
}

void PlayerEgo::dockToPlanet() {
    this->targetFollowCamera->setLookAtCam(true);
    this->boostTimer = 0;
    this->boostingFlag = 1;
    this->speed = 8.0f;
    this->boostTime = 10000;
    this->boostDelay = 0;
    this->collide = 0;
    this->resetGunDelay();
    this->dockingToPlanet = 1;
    this->shieldVisible = 0;
    this->speed = 8.0f;
    this->planetDockTimer = 0;
    Globals::sound->play(5, nullptr, nullptr, 0.0f);
}

void PlayerEgo::stopPlanetDock() {
    this->targetFollowCamera->setLookAtCam(false);
    this->collide = 1;
    this->dockingToPlanet = 0;
    this->stopBoost();
    this->speed = 2.0f;
}

bool PlayerEgo::isDockedToPlanet() {
    PlayerEgo *self = this;
    return 3000 < this->planetDockTimer;
}

void PlayerEgo::dockToStream(bool param) {
    if (param) {
        this->dockingToStream = 0;
        this->dockedToStream = 1;
        return;
    }
    this->speed = 2.0f;
    AbyssEngine::AEMath::Vector position = {this->moveDirection.x, this->moveDirection.y, this->moveDirection.z};
    this->setPosition(position);
    this->field_0x145 = 0;
    this->freeze = 0;
    this->dockingToStream = 0;
    this->dockedToStream = 0;
    this->setAutoPilot(nullptr);
}

int PlayerEgo::shouldSwitchToStandardCam() {
    if (((char &) this->field_0xb0) == 0) return 0;
    ((char &) this->field_0xb0) = 0;
    return 1;
}

int PlayerEgo::shouldSwitchToFreeLookCam() {
    if (this->switchToStandardCam == 0) return 0;
    this->switchToStandardCam = 0;
    return 1;
}

void PlayerEgo::setDockingState(int s) {
    if (s == 2 && this->dockingPointIndex == 1) this->field_0xb2 = 1;
    this->dockingPointIndex = s;
}

void PlayerEgo::stopMining() {
    PlayerAsteroid *asteroid = (PlayerAsteroid *) (intptr_t) this->asteroidTarget;
    int oreIndex = asteroid->asteroidIndex;
    int freeSpace = Globals::status->getShip()->getFreeSpace();
    int coreIndex = oreIndex == 217 ? 218 : oreIndex + 11;
    int amount = ((MiningGame *) (intptr_t) this->miningGame)->getOreAmount();
    if (Globals::status->hardCoreMode() && !((MiningGame *) (intptr_t) this->miningGame)->gameWon())
        amount = (int) ((float) amount * 0.5f);
    if (freeSpace <= amount)
        amount = freeSpace;
    if (freeSpace >= 1) {
        if (((MiningGame *) (intptr_t) this->miningGame)->gotCore()) {
            Item *core = (*Globals::items)[coreIndex]->makeItem(1);
            Globals::status->getShip()->addCargo(core);
            this->hud->catchCargo(coreIndex, 1, false, false, false, false, false);
            Globals::status->field_a4++;
            if (coreIndex != 218)
                (*Globals::status->field_98)[coreIndex - 165] = true;
            int remaining = Globals::status->getShip()->getFreeSpace();
            if (remaining < amount)
                amount = remaining;
        }
        if (amount > 0) {
            Item *ore = (*Globals::items)[oreIndex]->makeItem(amount);
            Globals::status->getShip()->addCargo(ore);
            this->hud->catchCargo(oreIndex, amount, false, false, true, false, false);
            Globals::status->field_a0 += amount;
            if (oreIndex != 217)
                (*Globals::status->field_94)[oreIndex - 154] = true;
        }
        if (Globals::status->getShip()->getFreeSpace() <= 0)
            this->hud->catchCargo(oreIndex, amount, true, false, true, false, false);
    } else {
        this->hud->catchCargo(oreIndex, 0, true, false, true, false, false);
    }
    this->setExhaustVisible(true);
    ((PlayerAsteroid *) (intptr_t) this->asteroidTarget)->hasCargo = 0;
    ((PlayerAsteroid *) (intptr_t) this->asteroidTarget)->player->setHitpoints(-1);
    Globals::status->field_d8--;
    if (this->miningGame != 0)
        delete (MiningGame *) (intptr_t) this->miningGame;
    this->miningGame = 0;
    this->dockToAsteroid(nullptr, this->field_0x14);
    Globals::sound->stop(1);
    Globals::sound->stop(3);
    Globals::sound->resume(this->field_0x1c);
}

unsigned char PlayerEgo::lostMiningGame() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->lostMiningGameFlag);
}

void PlayerEgo::setRocketControl(Gun *gun, AEGeometry *geo) {
    Level *lvl = this->level;
    this->rocketControlGun = (int) (intptr_t) gun;
    int psm_arg = lvl->movingStarsIndex;
    ParticleSystemManager *psm = (ParticleSystemManager *) (intptr_t) lvl->skybox2Mesh;
    if (gun == 0) {
        ((ParticleSystemManager *) (psm))->systemSetMatrix(psm_arg, &((Player *) this->player)->transform);
        this->rocketBanking = 0.0f;
        return;
    }
    Matrix *m = &geo->getReferenceMatrix();
    ((ParticleSystemManager *) (psm))->systemSetMatrix(psm_arg, m);
    this->setAutoPilot(nullptr);
}

bool PlayerEgo::killLiberator() {
    Array<Array<Gun *> *> *guns = this->player->guns;
    if (guns == nullptr || (*guns)[1] == nullptr || guns->size() == 0 || (*guns)[1]->size() == 0)
        return false;
    if (this->currentSecondaryWeaponIndex != 179)
        return false;
    for (unsigned int i = 0; i < (*guns)[1]->size(); i++) {
        Gun *gun = (*(*guns)[1])[i];
        if (gun->itemIndex == 179) {
            gun->lifetimes[0] = -1;
            *gun->positions.data() = Vector{50000.0f, 50000.0f, 50000.0f};
            (*(*guns)[1])[i]->active = false;
        }
    }
    return true;
}

void PlayerEgo::initManeuver(int type) {
    if ((type == 1 || type == 2) && this->volatileGoods != 0)
        this->player->flShake += 0.17f;

    if (this->maneuverType == 0) {
        this->maneuverTimer = 0;
        this->maneuverType = type;
        if (type == 3) {
            this->strafeTargetVec = this->getPosition() - 10000.0f * this->geometry->getDirection();
            this->facingVec = this->geometry->getDirection();
        }
    }
}

void PlayerEgo::render(bool allowHud) {
    if (this->isDead()) {
        if (this->explosion != nullptr) {
            this->explosion->render();
            if (this->explosionTimer < 3000)
                this->geometry->render();
        }
        if (this->explosion2 != nullptr)
            this->explosion2->render();
        this->level->enableMovingStars(true);
    } else {
        if (this->field_0x309 == 0)
            return;
        if (this->explosion != nullptr)
            this->explosion->render();
        this->geometry->render();
        if (this->field_0xac != 0 && this->emergencySystemTimer >= 1)
            ((AEGeometry *) (intptr_t) this->field_0xac)->render();
        if (this->shieldVisible != 0)
            this->shieldGeometry->render();
        if (this->turretMode != 0) {
            if (this->turretExtraGeo != nullptr)
                this->turretExtraGeo->setVisible(this->turretActive);
            this->turretRoot->render();
        }
        if (this->tractorBeam != nullptr)
            this->tractorBeam->render();
        if (this->repairBeams != nullptr) {
            for (unsigned int i = 0; i < this->repairBeams->size(); i++)
                (*this->repairBeams)[i]->render();
        }
        this->level->enableMovingStars(this->freeze == 0 && allowHud && this->dockingPointIndex != 1);
    }
}

void PlayerEgo::hideShipForFirstPersonCameraView(bool param) {
    unsigned char r1 = param;
    this->field_0x32d = r1;
    unsigned char nr = r1 ^ 1;
    this->field_0x309 = (this->visible != 0) & nr;
    reinterpret_cast<uint8_t &>(this->level->field_80->flags) = nr & (this->exhaustVisible != 0);
}

void PlayerEgo::setVisible(bool v) {
    this->visible = v;
    this->field_0x309 = v;
    this->setExhaustVisible(v);
}

void PlayerEgo::draw(bool allowHud) {
    if (this->rocketControlGun)
        return;
    if (this->hackingGame && !this->turretActive) {
        ((HackingGame *) (intptr_t) this->hackingGame)->render2D();
        return;
    }
    if (this->miningGame) {
        ((MiningGame *) (intptr_t) this->miningGame)->render2D();
        return;
    }
    if (this->computerControlled || this->isDead() || this->freeze || !allowHud || (char &) this->dockingState
        || this->dockingToPlanet || this->dockedToStream || (this->dockedFlag && !this->turretActive)
        || (this->autoPilot && !this->turretActive)) {
        if (this->autoPilot || (this->dockedFlag && (unsigned) (this->dockingPointIndex - 1) >= 3))
            this->drawThrottle();
        return;
    }

    Matrix view;
    if (!this->autoPilot && !this->turretActive)
        view = this->geometry->getMatrix();
    Globals::Canvas->SetColor(0xffffffff);
    if (this->turretActive && Globals::status->getShip()->getFirstEquipmentOfSort(35)) {
        Globals::Canvas->DrawImage2D(this->field_0x14->isPlasmaInRange() ? this->radarBlipImage3 : this->radarBlipImage2,
                                     (int) PlayerEgo::crosshairPos.x, (int) PlayerEgo::crosshairPos.y, 17, 68);
    } else if (this->level->field_30) {
        Globals::Canvas->DrawImage2D(this->radarBlipImage1, (int) PlayerEgo::crosshairPos.x,
                                     (int) PlayerEgo::crosshairPos.y, 17, 68);
        this->shakeAccum += this->shakeIntensity;
        if (this->shakeAccum > 200)
            this->level->field_30 = 0;
    } else {
        Globals::Canvas->DrawImage2D(this->dockArrowImage, (int) PlayerEgo::crosshairPos.x,
                                     (int) PlayerEgo::crosshairPos.y, 17, 68);
        this->shakeAccum = 0;
    }
    this->drawThrottle();
}

void PlayerEgo::drawThrottle() {
    if (this->throttleStarted == 0)
        return;
    int fade = this->throttle;
    if (fade > 500)
        fade = 2000 - fade;
    float alpha = (float) fade / 500.0f;
    alpha = alpha < 1.0f ? alpha : 1.0f;
    Globals::Canvas->SetColor((int) (alpha * 255.0f) + 0xffffff00);

    int width = Globals::Canvas->GetImage2DWidth(this->hpGaugeImage);
    int height = Globals::Canvas->GetImage2DHeight(this->hpGaugeImage);
    int filled = (int) (this->thrust * (float) height);
    Globals::Canvas->DrawRegion2D(this->hpGaugeImage, 0, height - filled, width, (int) (this->thrust * (float) height),
                                  0.0f, 0, 0, (int) (crosshairPos.x - (float) (width / 2)),
                                  (int) (crosshairPos.y + (float) height - (float) filled));

    AbyssEngine::String percent((int) (this->thrust * 100.0f));
    int textHeight = Globals::Canvas->GetImage2DHeight(this->hpGaugeImage);
    Globals::Canvas->DrawString(
        (unsigned int) (uintptr_t) Globals::font, percent,
        (int) (crosshairPos.x - (float) (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, percent) / 2)
               + -1.0f),
        (int) (crosshairPos.y + (float) (int) ((float) textHeight / 1.6f)), false);
    Globals::Canvas->SetColor(0xffffffff);
}

void PlayerEgo::throttleChanged() {
    if (this->throttleStarted == 0) {
        this->throttle = 0;
        this->throttleStarted = 1;
    } else if (this->throttle > 500 && this->throttle < 1500) {
        this->throttle = 500;
    } else if (this->throttle >= 1500) {
        this->throttle = 2000 - this->throttle;
    }
}

float PlayerEgo::getHandling() {
    PlayerEgo *self = this;
    return this->handling;
}

void PlayerEgo::shake(int amount) {
    AEGeometry *cam = this->geometry;
    float a = (float) amount / g_PE_shakeDiv;
    float intensity = (float) (this->shakeIntensity << 1);

    int range = (int) (a * intensity);
    if (range < 2)
        range = 1;
    int span = range << 1;

    float dx = (float) (int) (Globals::rnd->nextInt(span) - range);
    float dy = (float) (int) (Globals::rnd->nextInt(span) - range);
    float dz = (float) (int) (Globals::rnd->nextInt(span) - range);
    ((AEGeometry *) (cam))->translate(dx, dy, dz);
}

Hud *PlayerEgo::getHUD() {
    PlayerEgo *self = this;
    return this->hud;
}

unsigned char PlayerEgo::isInFreeLookMode() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->freeLookMode);
}

void PlayerEgo::setFreeLookMode(bool v) {
    PlayerEgo *self = this;
    this->freeLookMode = v;
}

void PlayerEgo::setTargetFollowCamera(TargetFollowCamera *cam) {
    this->targetFollowCamera = cam;
    cam->setShipHandling(this->handling);
}

TargetFollowCamera *PlayerEgo::getTargetFollowCamera() {
    PlayerEgo *self = this;
    return this->targetFollowCamera;
}

void PlayerEgo::pitchAllPrimaryGuns(float pitch) {
    ((Player *) this->player)->pitchAllPrimaryGuns(pitch);
}

void PlayerEgo::setSpeed(float v) {
    PlayerEgo *self = this;
    this->speed = v;
}

void PlayerEgo::changeThrust(float v) {
    float n = this->thrust + v;
    float p3 = 1.0f;
    if (n < 1.0f) p3 = 0.0f;
    if (n < 0.0f) p3 = 0.0f;
    float p2 = p3;
    if (n < 1.0f) p2 = n;
    if (n < 0.0f) p2 = p3;
    this->thrust = p2;
}

float PlayerEgo::getThrust() {
    return this->thrust;
}

int PlayerEgo::getSpeed() {
    // Callers (LevelScript) consume the raw float bits.
    return reinterpret_cast<int &>(this->speed);
}

int PlayerEgo::getDockTotalAmount() {
    PlayerEgo *self = this;
    return this->dockTotalAmount;
}

int PlayerEgo::getDockTransferedAmount() {
    PlayerEgo *self = this;
    return this->dockTransferedAmount;
}
