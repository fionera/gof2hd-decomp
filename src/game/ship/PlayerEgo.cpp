#include "game/ship/PlayerEgo.h"
#include "engine/render/Camera.h"
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

    static const float g_PE_cft_transformVal = 0.0f;
    static const float g_PE_shakeDiv = 40000.0f;
    static int *g_PE_adp_dockRadius = nullptr;
    static PlayerFixedObject **g_PE_adp_fixedObj = nullptr;
    static const unsigned int g_PE_dc_defX = 0u;
    static const unsigned int g_PE_dc_defY = 0u;
    static const float g_PE_mtp_strafeEps = 0.0f;
    static const float g_PE_mtp_strafeReset = 0.0f;
    static const float g_PE_mtp_strafeK = 0.0f;
    static const float g_PE_rollLevelEps = 0.0f;
    static const float g_PE_cc_alarmDist = 0.0f;
    static const float g_PE_strafeLoadK = 0.4f;
    static const float g_PE_strafeLoadB = 0.6f;
    static const float g_PE_strafeAccelK = 0.002f;
    static float *g_PE_dr_posLock = nullptr;
    static float *g_PE_dr_posNoLock = nullptr;
    static float *g_PE_dr_posBlink = nullptr;
    static float *g_PE_dr_posNormal = nullptr;
    static const float g_PE_upd_handlingBias = 0.0f;
        static float *g_PE_t_anchor = nullptr;
    static String **g_PE_t_pctStr = nullptr;
    static const float g_PE_t_timerDiv = 0.0f;
    static const float g_PE_t_pctScale = 0.0f;
    static const float g_PE_t_textDiv = 0.0f;
    static MiningHostObject **g_PE_aa_levelHolder = nullptr;
    static int *g_PE_aa_mineSound = nullptr;
    static MiningHostObject **g_PE_aa_winHolder1 = nullptr;
    static MiningHostObject **g_PE_aa_winHolder2 = nullptr;
    static const float g_PE_aa_settleEps = 0.0f;
        static const float g_PE_hs_throttleBias = 0.0f;
    // Pitch ramp delta constants (down/up non-turret paths)
    // Yaw ramp delta constants (right/left non-turret paths)
}

struct PE_MatrixBuf {
    float m00;
    float pad0[4];
    float m11;
    float pad1[4];
    float m22;
    float pad2[1];
};
#if __SIZEOF_POINTER__ == 4
static_assert(sizeof(PE_MatrixBuf) == 0x30, "PE_MatrixBuf size");
static_assert(__builtin_offsetof(PE_MatrixBuf, m00) == 0x00, "m00");
static_assert(__builtin_offsetof(PE_MatrixBuf, m11) == 0x14, "m11");
static_assert(__builtin_offsetof(PE_MatrixBuf, m22) == 0x28, "m22");
#endif

struct PE_MaterialBlock {
    char pad0[0x20];
    unsigned char *materialList;
};
#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(PE_MaterialBlock, materialList) == 0x20, "materialList");
#endif

struct PE_AsteroidMineTarget {
    char pad0[0x44];
    int miningFlag;
};
#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(PE_AsteroidMineTarget, miningFlag) == 0x44, "miningFlag");
#endif

static inline Status *PE_status() { return Globals::status; }

void *TransformGetLocal(void *, int); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void MatrixSetRotation(void *, void *, float, float, float); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

int aeabi_idiv_(int a, int b);

int PE_adp_approach(PlayerEgo *self, void *station); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

int PE_adp_glide(PlayerEgo * self);
void PE_adp_apply(PlayerEgo * self);


static inline void Mat_assign(void *dst, const void *src) { for (int _i=0;_i<15;_i++) ((float*)dst)[_i]=((const float*)src)[_i]; } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline void Vec_assign(void *dst, const void *src) { for(int _i=0;_i<3;_i++) ((float*)dst)[_i]=((const float*)src)[_i]; } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PE_mtp_steer(PlayerEgo *self, const float *target, int steer, float speed);

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline void Vec_sub(void *out, const void *a, const void *b) { for(int _i=0;_i<3;_i++) ((float*)out)[_i]=((const float*)a)[_i]-((const float*)b)[_i]; } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline float Vec_length(const void *v) { const float*_p=(const float*)v; return __builtin_sqrtf(_p[0]*_p[0]+_p[1]*_p[1]+_p[2]*_p[2]); } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

int PE_hat_aimAndFire(PlayerEgo *self, int dt);

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

float PE_roll_bankFactor(PlayerEgo *self, float rx, float ry, float *outZ);

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline void hitCamera_(PlayerEgo *self) { self->hitCamera(); }

void PE_cc_wormhole(PlayerEgo *self, void *obj); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PE_cc_obstacle(PlayerEgo *self, void *obj, unsigned idx); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PE_cc_destructible(PlayerEgo *self, void *obj); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void *PE_dtdp_makeEase(const void *fromMatrix, const void *navPoint); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PE_upd_boost(PlayerEgo *self, int dt);

void PE_upd_docksFinishDelivery(PlayerEgo *self, void *radio); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PE_um_dodgeStep(PlayerEgo * self);

void PE_um_strafeTarget(PlayerEgo *self, float *out);

void PE_um_strafeGlide(PlayerEgo * self);

static inline void Mat_getPosition(void *out, const void *m) { *(AbyssEngine::AEMath::Vector*)out = AbyssEngine::AEMath::MatrixGetPosition(*(const AbyssEngine::AEMath::Matrix*)m); } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline void Mat_getUp(void *out, const void *m) { *(AbyssEngine::AEMath::Vector*)out = AbyssEngine::AEMath::MatrixGetUp(*(const AbyssEngine::AEMath::Matrix*)m); } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline void Mat_getDir(void *out, const void *m) { *(AbyssEngine::AEMath::Vector*)out = AbyssEngine::AEMath::MatrixGetDir(*(const AbyssEngine::AEMath::Matrix*)m); } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void Mat_getLookAt(void *out, const void *eye, const void *dir, const void *up); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PE_htv_applyShake(PlayerEgo *self, int dt, void *eye, void *dir); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

int PE_aa_approachStep(PlayerEgo *self, int hud2, void *radar); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void Mat_mul(void *out, const void *a, const void *b); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PE_handleShip_orient(PlayerEgo *self, int dt, unsigned int tfHandle);

void *TractorBeam_new(void *geo, int kind); // lint: void_ptr (fn-ptr slot; param type load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline void MatrixGetPosition(void *out, void *m) { *(AbyssEngine::AEMath::Vector*)out = AbyssEngine::AEMath::MatrixGetPosition(*(const AbyssEngine::AEMath::Matrix*)m); } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void *MovingStars_ctor(void *self); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PlayerEgo::setVisible(bool v) {
    this->visible = v;
    this->field_0x309 = v;
    this->setExhaustVisible(v);
}

void PlayerEgo::boost() {
    if (this->boostingFlag != 0) return;
    if (this->field_0x146 == 0) return;
    if (this->rocketControlGun != 0) return;
    if (this->boostTimer < 0) return;
    this->boostingFlag = 1;
    this->boostTimer = 0;
    ((float &) this->speed) = (float) this->boostSpeed;
    Globals::sound->play(this->boostSoundId, nullptr, nullptr, 0.0f);
}

float PlayerEgo::getDriveChargeRate() {
    float d = (float) this->driveCharge / (float) this->driveChargeMax;
    return d >= 0.0f ? d : 1.0f;
}

unsigned char PlayerEgo::isAutoPilot() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->autoPilot);
}

bool PlayerEgo::goingToWormhole() {
    Level *m = this->level;
    KIPlayer *r4 = (KIPlayer *) (intptr_t) this->autoPilotTarget;
    Array<KIPlayer *> *lm = ((Level *) (m))->getLandmarks();
    return r4 == (*lm)[3];
}

int PlayerEgo::getCurrentSecondaryWeaponIndex() {
    PlayerEgo *self = this;
    return this->currentSecondaryWeaponIndex;
}

int PlayerEgo::getHitpoints() {
    return ((Player *) this->player)->getHitpoints();
}

unsigned char PlayerEgo::isChargingDrive() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->chargingDrive);
}

int PlayerEgo::shouldSwitchToFreeLookCam() {
    if (this->switchToStandardCam == 0) return 0;
    this->switchToStandardCam = 0;
    return 1;
}

float PlayerEgo::getHandling() {
    PlayerEgo *self = this;
    return this->handling;
}

void PlayerEgo::setComputerControlled(bool v) {
    PlayerEgo *self = this;
    this->computerControlled = v;
}

int PlayerEgo::getRocketBanking() {
    PlayerEgo *self = this;
    return this->rocketBanking;
}

bool PlayerEgo::isDockingToDockingPoint() {
    if (this->dockedFlag != 0) return this->dockingPointIndex != 1;
    return false;
}

void PlayerEgo::forceBoost() {
    this->boostTimer = 0;
    this->boostingFlag = 1;
    this->speed = 0x41000000;
    this->boostTime = 0x2710;
    this->boostDelay = 0;
}

int PlayerEgo::isDead() {
    PlayerEgo *self = this;
    return ((PlayerEgo *) (self))->getHitpoints() < 1;
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

float PlayerEgo::getCloakRechargeRate() {
    return 1.0f - (float) this->cloakRechargeTimer / (float) this->cloakRechargeMax;
}

unsigned char PlayerEgo::isDockedToStream() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->dockedToStream);
}

int PlayerEgo::getTargetFollowCamera() {
    PlayerEgo *self = this;
    return this->targetFollowCamera;
}

unsigned char PlayerEgo::isCloaked() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->cloaked);
}

void PlayerEgo::setFreeLookMode(bool v) {
    PlayerEgo *self = this;
    this->freeLookMode = v;
}

void PlayerEgo::setThrust(float v) {
    PlayerEgo *self = this;
    this->thrust = v;
}

bool PlayerEgo::readyForCloak() {
    if (this->chargingCloak == 0) return false;
    return this->cloakCharge >= this->cloakDischargeMax;
}

int PlayerEgo::getHackingGameDockIndex() {
    int v = this->hackingGame;
    if (v == 0) return -1;
    return ((HackingGame *) v)->getDockingIndex();
}

void PlayerEgo::setPosition(float x, float y, float z) {
    AEGeometry *g = this->geometry;
    char v[12];
    *(float *) (v + 0) = x;
    *(float *) (v + 4) = y;
    *(float *) (v + 8) = z;
    ((AEGeometry *) (g))->setPosition(*(Vector *) v);
}

void PlayerEgo::dockToStream(bool param) {
    if (param) {
        this->dockingToStream = 0;
        this->dockedToStream = 1;
        return;
    }
    this->speed = 0x40000000;
    AbyssEngine::AEMath::Vector position = {this->rotX, this->rotY, this->rotZ};
    this->setPosition(position);
    this->field_0x145 = 0;
    this->freeze = 0;
    this->dockingToStream = 0;
    this->dockedToStream = 0;
    this->setAutoPilot(nullptr);
}

void PlayerEgo::hackingShuffle() {
    int v = this->hackingGame;
    if (v != 0) ((HackingGame *) v)->reInit();
}

int PlayerEgo::getDockTransferedAmount() {
    PlayerEgo *self = this;
    return this->dockTransferedAmount;
}

unsigned char PlayerEgo::goingToWaypoint() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->goingToWaypointFlag);
}

bool PlayerEgo::isDockedToDockingPoint() {
    if (this->dockedFlag == 0) return false;
    return this->dockingPointIndex == 1;
}

int PlayerEgo::getDockTotalAmount() {
    PlayerEgo *self = this;
    return this->dockTotalAmount;
}

bool PlayerEgo::isInWormhole() {
    if (((PlayerEgo *) (this))->getHitpoints() > 0) return this->inWormhole != 0;
    return false;
}

void PlayerEgo::setRocketControl(Gun *gun, AEGeometry *geo) {
    Level *lvl = this->level;
    this->rocketControlGun = (int) (intptr_t) gun;
    int psm_arg = lvl->movingStarsIndex;
    ParticleSystemManager *psm = (ParticleSystemManager *) (intptr_t) lvl->skybox2Mesh;
    if (gun == 0) {
        ((ParticleSystemManager *) (psm))->systemSetMatrix(psm_arg, (Matrix *) ((Player *) this->player)->transform);
        this->rocketBanking = 0;
        return;
    }
    Matrix *m = &geo->getReferenceMatrix();
    ((ParticleSystemManager *) (psm))->systemSetMatrix(psm_arg, m);
    this->setAutoPilot(nullptr);
}

bool PlayerEgo::isMining() {
    PlayerEgo *self = this;
    return this->miningGame != 0;
}

void PlayerEgo::turnHorizontal(int a, float v) {
    if (v < 0.0f) {
        left(a, v);
    } else if (v > 0.0f) {
        right(a, v);
    }
}

int PlayerEgo::getThrust() {
    PlayerEgo *self = this;
    return ((int &) this->thrust);
}

float PlayerEgo::getCloakRate() {
    float a = (float) this->cloakCharge;
    float b = (float) (this->cloaked == 0 ? this->cloakDischargeMax : this->cloakChargeMax);
    float d = a / b;
    return d >= 0.0f ? d : 1.0f;
}

void PlayerEgo::resetLastHP() {
    this->lastHP = ((Player *) (this->player))->getCombinedHP();
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

int PlayerEgo::shouldSwitchToStandardCam() {
    if (((char &) this->field_0xb0) == 0) return 0;
    ((char &) this->field_0xb0) = 0;
    return 1;
}

void PlayerEgo::resetMovement() {
    this->rollAccum = 0;
    this->field_0x26c = 0;
    this->yawAccumD = 0;
    this->field_0x274 = 0;
    this->pitchAccumD = 0;
    this->yawAccumF = 0;
}

bool PlayerEgo::isHacking() {
    PlayerEgo *self = this;
    return this->hackingGame != 0;
}

void PlayerEgo::setPosition(Vector v) {
    setPosition(v.x, v.y, v.z);
}

void PlayerEgo::resetChargingDrive() {
    PlayerEgo *self = this;
    ((uint8_t &) this->chargingDrive) = 0;
}

void PlayerEgo::PauseEngineSound() {
    ((Player *) this->player)->PauseEngineSound();
}

unsigned char PlayerEgo::isInFreeLookMode() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->freeLookMode);
}

void PlayerEgo::setActive(bool value) {
    ((Player *) this->player)->setActive(value);
}

void PlayerEgo::alignToHorizon() {
    PlayerEgo *self = this;
    ((uint8_t &) this->rolling) = 1;
}

void PlayerEgo::setAutoTurret(bool on) {
    this->autoTurretEnabled = on;
    if (!on) ((Player *) this->player)->stopShooting(2);
}

void PlayerEgo::hitCamera() {
    this->hitShake = 1;
    int cam = this->targetFollowCamera;
    this->hitShakeTimer = 0;
    ((TargetFollowCamera *) cam)->hit();
}

int PlayerEgo::hackingWon() {
    int v = this->hackingGame;
    if (v == 0) return 0;
    return ((HackingGame *) v)->gameWon();
}

unsigned char PlayerEgo::lostMiningGame() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->lostMiningGameFlag);
}

int PlayerEgo::getCurrentMiningAmount() {
    int v = this->miningGame;
    if (v == 0) return 0;
    return ((MiningGame *) v)->getOreAmount();
}

void PlayerEgo::setDockingState(int s) {
    if (s == 2 && this->dockingPointIndex == 1) this->field_0xb2 = 1;
    this->dockingPointIndex = s;
}

void PlayerEgo::hackingRotateLCW() {
    if (this->hackingGame != 0 && ((HackingGame *) (this->hackingGame))->isRotating() == 0
        && ((HackingGame *) (this->hackingGame))->gameWon() == 0)
        ((HackingGame *) this->hackingGame)->rotateLeftCW(true);
}

bool PlayerEgo::isInDockingProcedure() {
    if (((char &) this->dockingState) != 0) return true;
    return this->dockedFlag != 0;
}

void PlayerEgo::setSpeed(float v) {
    PlayerEgo *self = this;
    ((float &) this->speed) = v;
}

void PlayerEgo::ResumeEngineSound() {
    ((Player *) this->player)->ResumeEngineSound(false);
}

void PlayerEgo::addNukeVolatileForce(float v) {
    Player *p = (Player *) this->player;
    p->flShake = p->flShake + v * 3.0f;
}

void PlayerEgo::explode() {
    ((ParticleSystemManager *) (this->level->field_74))->enableSystemEmit(this->currentSystem, 1);
    if (((int &) this->explosion) != 0) return;
    ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setActive(0);
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

unsigned char PlayerEgo::isDockingToStream() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->dockingToStream);
}

int PlayerEgo::goingToStream() {
    Level *m = this->level;
    KIPlayer *r4 = (KIPlayer *) (intptr_t) this->autoPilotTarget;
    Array<KIPlayer *> *lm = ((Level *) (m))->getLandmarks();
    return r4 == (*lm)[1];
}

Vec3 PlayerEgo::GetDirVector() {
    PlayerEgo *self = this;
    return ((AEGeometry *) self->geometry)->getDirection();
}

void PlayerEgo::hideShipForFirstPersonCameraView(bool param) {
    unsigned char r1 = param;
    this->field_0x32d = r1;
    unsigned char nr = r1 ^ 1;
    this->field_0x309 = (this->visible != 0) & nr;
    reinterpret_cast<uint8_t &>(this->level->field_80->flags) = nr & (this->exhaustVisible != 0);
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

void PlayerEgo::deleteHackingGame() {
    HackingGame *g = (HackingGame *) ((intptr_t) this->hackingGame);
    if (g != 0) delete g;
    this->hackingGame = 0;
}

bool PlayerEgo::explosionEnded() {
    if (((int &) this->explosion) == 0) return true;
    return 8000 < this->explosionTimer;
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

Vec3 PlayerEgo::GetUpVector() {
    PlayerEgo *self = this;
    return ((AEGeometry *) (self->geometry))->getUpVector();
}

bool PlayerEgo::isDockedToPlanet() {
    PlayerEgo *self = this;
    return 3000 < this->planetDockTimer;
}

bool PlayerEgo::emergencySystemActive() {
    PlayerEgo *self = this;
    return 0 < this->emergencySystemTimer;
}

bool PlayerEgo::isDockingToAsteroid() {
    if (((char &) this->dockingState) != 0) return this->dockingPointIndex != 1;
    return false;
}

float PlayerEgo::getBoostRate() {
    float d = (float) this->boostTimer / (float) this->boostDelay;
    float r = d + 1.0f;
    if (0.0f < d) r = 1.0f;
    return r;
}

bool PlayerEgo::driveReady() {
    PlayerEgo *self = this;
    return this->driveCharge >= this->driveChargeMax;
}

void PlayerEgo::turnVertical(int a, float v) {
    if (v < -0.0f) {
        down(a, v);
    } else if (v > 0.0f) {
        up(a, v);
    }
}

unsigned char PlayerEgo::isInTurretMode() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->turretActive);
}

void PlayerEgo::startJumpDrive() {
    if (this->chargingDrive != 0) return;
    Globals::sound->play(0x21, nullptr, nullptr, 0.0f);
    ((Hud *) (this->hud))->hudEvent(0x19, this, 0);
    this->chargingDrive = 1;
    this->driveCharge = 0;
}

bool PlayerEgo::isInRocketControl() {
    PlayerEgo *self = this;
    return this->rocketControlGun != 0;
}

int PlayerEgo::getSpeed() {
    PlayerEgo *self = this;
    return this->speed;
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

unsigned char PlayerEgo::isDockingToPlanet() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->dockingToPlanet);
}

unsigned char PlayerEgo::isChargingCloak() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->chargingCloak);
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

void PlayerEgo::setCurrentSecondaryWeaponIndex(int idx) {
    this->currentSecondaryWeaponIndex = idx;
    Globals::status->field_f4 = idx;
}

void PlayerEgo::removeRoute() {
    Route *r = (Route *) ((intptr_t) this->route);
    if (r != 0) delete r;
    this->route = 0;
}

void PlayerEgo::setRoute(Route *v) {
    PlayerEgo *self = this;
    this->route = (int) (intptr_t) v;
}

unsigned char PlayerEgo::boosting() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->boostingFlag);
}

bool PlayerEgo::isDockedToAsteroid() {
    if (((char &) this->dockingState) == 0) return false;
    return this->dockingPointIndex == 1;
}

int PlayerEgo::goingToStation() {
    Array<KIPlayer *> *lm = ((Level *) (this->level))->getLandmarks();
    if ((*lm)[0] == 0) return false;
    KIPlayer *r4 = (KIPlayer *) (intptr_t) this->autoPilotTarget;
    lm = ((Level *) (this->level))->getLandmarks();
    return r4 == (*lm)[0];
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

int PlayerEgo::getBoostSpeed() {
    PlayerEgo *self = this;
    return this->boostSpeed;
}

void PlayerEgo::addGun(Gun *gun, int x) {
    ((Player *) (this->player))->addGun(gun, x);
    this->checkForTurret();
}

unsigned char PlayerEgo::aboutToReachAutoTarget() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->aboutToReachAutoTargetFlag);
}

unsigned char PlayerEgo::autoTurretIsEnabled() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->autoTurretEnabled);
}

bool PlayerEgo::readyToBoost() {
    PlayerEgo *self = this;
    return -1 < this->boostTimer;
}

void PlayerEgo::endExplosion() {
    int v = ((int &) this->explosion);
    if (v != 0) ((Explosion *) v)->reset();
}

bool PlayerEgo::isLandingOrTakingOff() {
    if (this->dockedFlag == 0) return false;
    return (((uint32_t &) this->dockingPointIndex) | 1) == 3;
}

void PlayerEgo::setFreeze(bool v) {
    PlayerEgo *self = this;
    this->freeze = v;
}

int PlayerEgo::getHUD() {
    PlayerEgo *self = this;
    return this->hud;
}

int PlayerEgo::getRoute() {
    PlayerEgo *self = this;
    return this->route;
}

void PlayerEgo::setCollide(bool v) {
    PlayerEgo *self = this;
    this->collide = v;
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

unsigned char PlayerEgo::hasAutoTurret() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->autoTurretEquipped);
}

unsigned char PlayerEgo::collidesWithStation() {
    PlayerEgo *self = this;
    return ((uint8_t &) this->collidesWithStationFlag);
}

void PlayerEgo::stopPlanetDock() {
    ((TargetFollowCamera *) (this->targetFollowCamera))->setLookAtCam(false);
    this->collide = 1;
    this->dockingToPlanet = 0;
    this->stopBoost();
    ((float &) this->speed) = 2.0f;
}

bool PlayerEgo::isRechargingCloak() {
    PlayerEgo *self = this;
    return 0 < this->cloakRechargeTimer;
}

unsigned char PlayerEgo::hasVolatileGoods() {
    return ((uint8_t &) this->volatileGoods);
}

void PlayerEgo::hackingRotateRCW() {
    if (this->hackingGame != 0 && ((HackingGame *) (this->hackingGame))->isRotating() == 0
        && ((HackingGame *) (this->hackingGame))->gameWon() == 0)
        ((HackingGame *) this->hackingGame)->rotateRightCW(true);
}

bool PlayerEgo::hasCloak() {
    PlayerEgo *self = this;
    return this->cloak != 0;
}

int PlayerEgo::isBoostRefreshed() {
    if (this->boostingFlag == 0 && this->field_0x146 != 0 && this->boostTimer >= 0)
        return 1;
    return 0;
}

Vec3 PlayerEgo::getPosition() {
    PlayerEgo *self = this;
    return ((AEGeometry *) (self->geometry))->getPosition();
}

Vec3 PlayerEgo::getTurretPosition() {
    Matrix &ship = ((AEGeometry *) (this->geometry))->getMatrix();
    Matrix &turret = ((AEGeometry *) (this->turretGeometry))->getMatrix();
    Matrix world = ship * turret;
    return MatrixGetPosition(world);
}

static inline void Vec_scale(void *out, const void *v, float s) { for(int _i=0;_i<3;_i++) ((float*)out)[_i]=((const float*)v)[_i]*s; } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)



// lint: void_ptr (external symbol; param/return types mangling-load-bearing)



// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PlayerEgo::initManeuver(int type) {
    if ((type == 1 || type == 2) && this->volatileGoods != 0)
        this->player->flShake += 0.17f;

    if (this->maneuverType == 0) {
        this->navPoint = nullptr;
        this->maneuverType = type;
        if (type == 3) {
            this->strafeTargetVec = this->getPosition() - 10000.0f * this->geometry->getDirection();
            this->facingVec = this->geometry->getDirection();
        }
    }
}

void PlayerEgo::refillGunDelay() {
    ((Player *) this->player)->refillGunDelay(0);
}

void PE_cft_place(PlayerEgo *self, int turretIdx);

void PlayerEgo::checkForTurret() {
    if (this->turretMode != 0)
        return;
    int avail = ((Player *) (this->player))->gunAvailable(0);
    this->turretMode = (unsigned char) avail;
    if (avail == 0)
        return;

    this->autoTurretEquipped = 0;
    Array<Item *> *equip = ((Ship *) (PE_status()->getShip()))->getEquipment(2);
    Item **item = equip->data_;
    this->turretPitch = (int) ((double) item[0]->getAttribute(0) * 1.5);

    int idx = item[0]->getIndex();
    unsigned short base = 0xffff, barrel = 0xffff;
    int muzzle = -1, child = -1, extra = -1, extra2 = -1;

    if (idx == 0xe0) {
        base = 0x499a;
        barrel = 0x499b;
        muzzle = 0x499c;
        child = 0x499d;
    } else if (idx == 0x30) {
        base = 0x1a74;
        barrel = 0x1a75;
    } else if (idx == 0x31) {
        base = 0x1a76;
        barrel = 0x1a77;
    } else if (idx == 0xb4) {
        this->autoTurretEquipped = 1;
        base = 0x1a95;
        barrel = 0x1a96;
    } else if (idx == 0xb5) {
        this->autoTurretEquipped = 1;
        base = 0x1a97;
        barrel = 0x1a98;
    } else if (idx == 0xb6) {
        this->autoTurretEquipped = 1;
        base = 0x1a99;
        barrel = 0x1a9a;
    } else if (idx == 0xc6) {
        base = 0x4963;
        barrel = 0x4964;
        muzzle = 0x4966;
        child = 0x4967;
        extra = -1;
        extra2 = 0x4a7f;
    } else if (idx == 0xc7) {
        base = 0x4968;
        barrel = 0x4969;
        muzzle = 0x496a;
        child = 0x496b;
        extra = -1;
        extra2 = 0x4a7f;
    } else if (idx == 0xc8) {
        base = 0x496c;
        barrel = 0x496d;
        muzzle = 0x496e;
        child = 0x496f;
        extra = 0x4970;
        extra2 = 0x4a7f;
    } else if (idx == 0x2f) {
        base = 0x1a72;
        barrel = 0x1a73;
    }

    PaintCanvas *canvas = Globals::Canvas;

    AEGeometry *baseGeo = new AEGeometry((uint16_t) base, (PaintCanvas *) canvas, false);
    this->turretBaseGeo = baseGeo;
    AEGeometry *yawGeo = new AEGeometry((uint16_t) barrel, (PaintCanvas *) canvas, false);
    this->turretGeometry = yawGeo;
    ((AEGeometry *) yawGeo)->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
    AEGeometry *muzzleRoot = new AEGeometry((PaintCanvas *) canvas);
    this->turretRoot = muzzleRoot;

    if (muzzle != -1) {
        AEGeometry *g = new AEGeometry((uint16_t)(unsigned short)muzzle, (PaintCanvas *) canvas, false);
        ((AEGeometry *) (this->turretBaseGeo))->addChild((uint32_t)(uintptr_t)g);
        delete (AEGeometry *) g;
    }
    if (child != -1) {
        AEGeometry *g = new AEGeometry((uint16_t)(unsigned short)child, (PaintCanvas *) canvas, false);
        ((AEGeometry *) (this->turretGeometry))->addChild((uint32_t)(uintptr_t)g);
        delete (AEGeometry *) g;
    }
    if (extra != -1) {
        AEGeometry *g = new AEGeometry((uint16_t)(unsigned short)extra, (PaintCanvas *) canvas, false);
        ((AEGeometry *) (this->turretGeometry))->addChild((uint32_t)(uintptr_t)g);
        delete (AEGeometry *) g;
    }
    if (extra2 != -1) {
        AEGeometry *g = new AEGeometry((uint16_t)(unsigned short)extra2, (PaintCanvas *) canvas, false);
        this->turretExtraGeo = g;
        ((AEGeometry *) (this->turretGeometry))->addChild((uint32_t)(uintptr_t)g);
        AbyssEngine::Transform *tf = (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
            (unsigned int) ((unsigned int) (unsigned long) Globals::Canvas));
        ((AbyssEngine::Transform *) (tf))->SetVisible(this->turretActive != 0);
    }

    AbyssEngine::Transform *tf = (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
        (unsigned int) ((unsigned int) (unsigned long) Globals::Canvas));
    ((AbyssEngine::Transform *) tf)->boundingRadius = g_PE_cft_transformVal;
    tf = (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
        (unsigned int) ((unsigned int) (unsigned long) Globals::Canvas));
    ((AbyssEngine::Transform *) (tf))->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);

    ((AEGeometry *) (this->turretBaseGeo))->setPosition(*(Vector *) &this->dockArrowImage);
    ((AEGeometry *) (this->turretGeometry))->setPosition(*(Vector *) &this->dockArrowImage);

    PE_cft_place(this, idx);

    ((AEGeometry *) (this->turretRoot))->addChild((uint32_t)(uintptr_t)this->turretBaseGeo);
    ((AEGeometry *) (this->turretRoot))->addChild((uint32_t)(uintptr_t)this->turretGeometry);

    if (this->cloak != 0 && this->turretMode != 0) {
        ((PaintCanvas *) (long) (canvas))->MeshCloneMaterial(
            ((AEGeometry *) this->turretBaseGeo)->meshId, this->cloakMaterial2);
        ((PaintCanvas *) (long) (canvas))->MeshCloneMaterial(
            ((AEGeometry *) this->turretGeometry)->meshId, this->cloakMaterial3);
    }
}

void PlayerEgo::pitchAllPrimaryGuns(float pitch) {
    ((Player *) this->player)->pitchAllPrimaryGuns(pitch);
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

void PlayerEgo::shake(int amount) {
    AEGeometry *cam = this->geometry;
    float a = (float) amount / g_PE_shakeDiv;
    float intensity = (float) (this->shakeIntensity << 1);

    int range = (int) (a * intensity);
    if (range < 2)
        range = 1;
    int span = range << 1;

    float dx = (float) (int) (Globals::rnd->next(span) - range);
    float dy = (float) (int) (Globals::rnd->next(span) - range);
    float dz = (float) (int) (Globals::rnd->next(span) - range);
    ((AEGeometry *) (cam))->translate(dx, dy, dz);
}

void PlayerEgo::setRotation(float rx, float ry, float rz) {
    this->rotateX = rx;
    this->rotateY = ry;
    this->rotateZ = rz;
    AbyssEngine::AEMath::Matrix &m =
        *(AbyssEngine::AEMath::Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
    AbyssEngine::AEMath::MatrixSetRotation(m, this->rotateX, this->rotateY, this->rotateZ);
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

void PlayerEgo::startSmokeEmission() {
    int v = this->smokeSystem;
    if (v < 0) return;
    ((ParticleSystemManager *) (this->level->particleEmitBoolPtr))->enableSystemEmit(v, 1);
    ((ParticleSystemManager *) (intptr_t) this->level->particleRenderBoolPtr)->enableSystemEmit(
        this->explosionSmoke, true);
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

static inline void adp_clearDockVector(PlayerEgo *self) {
    self->miningUpVec = Vector{0.0f, 0.0f, 0.0f};
}

int PlayerEgo::approachDockingPoint(Hud *hud, int /*hud2*/, Radar *radar) {
    if (((KIPlayer *) (this))->isDying() != 0)
        return 0;

    int state = ((int &) this->emergencyVec.x);

    if (state == 0) {
        KIPlayer *station = (KIPlayer *) this->dockStation;
        Vector pos = ((PlayerEgo *) (this))->getPosition();
        SpacePoint *nav = ((KIPlayer *) (station))->getNearestNavigationPoint(pos, (SpacePoint *) &this->navPoint);
        if (nav != 0) {
            if (this->strafeNavPoint != nav) {
                if (this->strafeNavPoint != 0)
                    ((SpacePoint *) (this->strafeNavPoint))->giveFree();
                this->strafeNavPoint = nav;
                ((SpacePoint *) (nav))->take();
            }
            int dist = PE_adp_approach(this, station);
            if (dist < (int) this->miningUpVec.y) {
                ((int &) this->emergencyVec.x) = 2;
                ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setLookAtCam(false);
                ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->useTargetsUpVector(false);
            }
        }
        PE_adp_apply(this);
        return 0;
    }

    if (state == 2) {
        if (this->turretActive != 0)
            ((PlayerEgo *) (this))->setTurretMode(0);
        int dist = PE_adp_glide(this);
        int radius = g_PE_adp_dockRadius[((Ship *) (PE_status()->getShip()))->getIndex()];
        if (dist < radius) {
            int ev = 0;
            {
                Mission *mission = PE_status()->getMission();
                Mission *campaign = (Mission *) (intptr_t) PE_status()->getCampaignMission();
                PlayerFixedObject *fixed = *g_PE_adp_fixedObj;

                if (((Mission *) (mission))->isEmpty() == 0 && ((Mission *) (mission))->getType() == 0xf
                    && fixed->getDockingType() == 1) {
                    if (((Ship *) (PE_status()->getShip()))->hasCargo(((Mission *) (mission))->getProductionGoodIndex(),
                                                                      1) != 0) {
                        int amount = ((Item *) (((Ship *) (PE_status()->getShip()))->getCargo(
                            ((Mission *) (mission))->getProductionGoodIndex())))->getAmount();
                        ((int &) this->autoTurretEquipped) = amount;
                        int need = ((Mission *) (mission))->getProductionGoodAmount()
                                   - ((Level *) (this->level))->getNumDeliveredOre();
                        if (need < amount) ((int &) this->autoTurretEquipped) = need;
                        this->autoTurretTimer = 0;
                        if (((int &) this->autoTurretEquipped) > 0) ev = 0x29;
                    }
                } else if (((Mission *) (mission))->isEmpty() == 0
                           && (((Mission *) (mission))->getType() == 0xb8 || ((Mission *) (mission))->getType() == 0xa8)
                           && fixed->getDockingType() == 2) {
                    int carried = fixed->intPosX;
                    int maxPax = ((Ship *) (PE_status()->getShip()))->getMaxPassengers();
                    bool handled = false;
                    if (maxPax > 0 && carried < maxPax) {
                        int avail = (((Mission *) (mission))->getType() == 0xa8)
                                        ? ((Mission *) (mission))->getStatusValue()
                                        : maxPax;
                        avail -= carried;
                        ((int &) this->autoTurretEquipped) = avail;
                        int status = ((Mission *) (mission))->getStatusValue() - carried;
                        if (status < avail) ((int &) this->autoTurretEquipped) = status;
                        this->autoTurretTimer = 0;
                        if (((int &) this->autoTurretEquipped) > 0) {
                            ev = 0x23;
                            handled = true;
                        }
                    }
                    if (!handled && ((Mission *) (mission))->getType() != 0xa8
                        && ((Ship *) (PE_status()->getShip()))->getMaxPassengers() == 0)
                        ev = 0x2b;
                } else if (((Mission *) (mission))->isEmpty() == 0 && ((Mission *) (mission))->getType() == 0xb8
                           && fixed->getDockingType() == 1) {
                    int n = fixed->intPosX;
                    if (n > 0) {
                        this->autoTurretTimer = 0;
                        ((int &) this->autoTurretEquipped) = n;
                        ev = 0x25;
                    }
                } else if (campaign != 0 && ((Mission *) (campaign))->isEmpty() == 0
                           && (((Mission *) (campaign))->getType() == 0xa7 || ((Mission *) (campaign))->getType() ==
                               0xae)
                           && fixed->getDockingType() == 1) {
                    if (((Ship *) (PE_status()->getShip()))->hasCargo(
                            ((Mission *) (campaign))->getProductionGoodIndex(), 1) != 0) {
                        int amount = ((Item *) (((Ship *) (PE_status()->getShip()))->getCargo(
                            ((Mission *) (campaign))->getProductionGoodIndex())))->getAmount();
                        ((int &) this->autoTurretEquipped) = amount;
                        int need = ((Mission *) (campaign))->getProductionGoodAmount() - ((Mission *) (campaign))->
                                   getStatusValue();
                        if (need < amount) ((int &) this->autoTurretEquipped) = need;
                        this->autoTurretTimer = 0;
                        if (((int &) this->autoTurretEquipped) > 0) ev = 0x29;
                    }
                }
            }
            if (ev != 0)
                hud->hudEvent(ev, this, 0);
            ((int &) this->emergencyVec.x) = 1;
        }
        PE_adp_apply(this);
        return 0;
    }

    if (state == 3) {
        int dist = PE_adp_glide(this);
        if (dist < 200) {
            this->dockStation = 0;
            radar->lockedEnemy = 0;
            radar->dockNavPtr = 0;
            adp_clearDockVector(this);
            ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setActive(true);
            ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setLookAtCam(false);
            ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->useTargetsUpVector(false);
            ((LevelScript *) (this->levelScript))->resetCamera(this->levelScript->m_pLevel);
            ((Player *) (this->player))->resetGunDelay(0);
            ((int &) this->emergencyVec.x) = 0;
            if (this->strafeNavPoint != 0) {
                ((SpacePoint *) (this->strafeNavPoint))->giveFree();
                this->strafeNavPoint = 0;
            }
            if (((intptr_t) this->hackingGame) != 0) {
                delete (HackingGame *) ((intptr_t) this->hackingGame);
                this->hackingGame = 0;
                hud->setHackingGameActive(false);
            }
            return 1;
        }
        PE_adp_apply(this);
    }
    return 0;
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

static inline float PE_minf(float a, float b) { return b < a ? b : a; }

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
        ((float &) this->rocketBanking) += (float) frameTime * delta * 0.01f;
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
        ((float &) this->rocketBanking) += (float) frameTime * delta * -0.01f;
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

PlayerEgo::PlayerEgo(Player *player) {
    this->rollMatrix = AbyssEngine::AEMath::Matrix();
    this->turretHudMatrix = AbyssEngine::AEMath::Matrix();

    this->player = player;
    player->setPlayShootSound(true, 2);

    Ship *ship = (Ship *) PE_status()->getShip();
    this->boostTimer = 0;
    this->boostingFlag = 0;
    if (ship != 0) {
        this->boostDelay = ship->getBoostDelay();
        this->handling = ship->getBoostTime();
        this->boostSpeed = ship->getBoostSpeed();
        this->field_0xb0 = ship->getHandling();
    }

    ((int &) this->explosion) = 0;
    ((int &) this->explosion2) = 0;
    this->autoTurretEnabled = 0;
    this->dockedFlag = 0;
    this->miningGame = 0;
    this->hackingGame = 0;
    this->visible = 1;
    this->field_0x309 = 1;
    this->exhaustVisible = 1;
    this->dockingPointIndex = -1;

    unsigned char *stars = (unsigned char *) ::operator new(0x1c);
    MovingStars_ctor(stars);
    this->dockCameraNode = (AEGeometry *) stars;
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

void PlayerEgo::moveToPosition(Vector target, bool steer, float speed) {
    float t[3] = {target.x, target.y, target.z};
    PE_mtp_steer(this, t, steer ? 1 : 0, speed);

    float up[3] = {0.0f, 1.0f, 0.0f};
    ((AEGeometry *) (this->geometry))->setDirection(this->headingVec, *(Vector *) up);

    float dt = (float) this->shakeIntensity;
    ((AEGeometry *) (this->geometry))->moveForward(this->thrust * dt * ((float &) this->speed));
    ((PlayerEgo *) (this))->roll(this->shakeIntensity);

    float slide = this->strafeAccel;
    float mag = slide > 0.0f ? slide : -slide;
    if (mag <= g_PE_mtp_strafeEps) {
        ((float &) this->strafeNavPoint) = g_PE_mtp_strafeReset;
    } else {
        unsigned char m[0x30];
        Mat_assign(m, ((AEGeometry *) (this->geometry))->getMatrix());

        float v = slide * dt;
        ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->translateNoUpdate(v, 0.0f, 0.0f);
        this->strafeAccel = this->strafeAccel * g_PE_mtp_strafeK;
        ((AEGeometry *) this->geometry)->setMatrix(*(const AbyssEngine::AEMath::Matrix *) m);
    }

    Mat_assign(((Player *) this->player)->transform, ((AEGeometry *) (this->geometry))->getMatrix());
    ((AEGeometry *) (this->geometry))->getPosition();
}

void PlayerEgo::resetGunDelay() {
    ((Player *) this->player)->resetGunDelay(0);
}

int PlayerEgo::getShieldDamageRate() {
    return ((Player *) this->player)->getShieldDamageRate();
}

void PlayerEgo::handleAutoTurret(int dt) {
    int t = this->autoTurretTimer + dt;
    this->autoTurretTimer = t;
    if (t >= 0xbb9) {
        this->autoTurretTimer = 0;
        this->autoTurretTarget = 0;
        Array<KIPlayer *> *enemies = ((Level *) (this->level))->getEnemies();
        if (enemies != nullptr) {
            int best = 60000;
            for (unsigned i = 0; i < enemies->size(); i++) {
                KIPlayer *e = (*enemies)[i];
                if (((KIPlayer *) (e))->isDead() != 0) continue;
                if (((KIPlayer *) (e))->isDying() != 0) continue;
                if (((Player *) (((KIPlayer *) e)->player))->isActive() == 0) continue;
                if (((KIPlayer *) (e))->isEnemy() == 0) continue;
                if (reinterpret_cast<uint8_t &>(((KIPlayer *) e)->route) != 0) continue;

                float epos[3];
                ((KIPlayer *) (e))->getPosition();
                float rel[3];
                Vec_sub(rel, epos, &this->dockOffsetVec);
                int dist = (int) Vec_length(rel);
                if (dist < best
                    && (this->autoTurretTarget == 0 || this->autoTurretTarget != this->autoTurretPrevTarget)) {
                    this->autoTurretTarget = e;
                    best = dist;
                }
            }
        }
    }

    KIPlayer *target = (KIPlayer *) this->autoTurretTarget;
    if (target != 0 && ((KIPlayer *) target)->geometry != 0) {
        int fireTimer = PE_hat_aimAndFire(this, dt);
        this->autoTurretFireTimer = fireTimer;
        if (fireTimer + dt > 0x1f4) {
        } else {
            return;
        }
    }

    ((Player *) (this->player))->stopShooting(0);
}



// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PlayerEgo::dockToAsteroid(KIPlayer *asteroid, Radar *radar) {
    if (((char &) this->dockingState) != 0) {
        ((PlayerAsteroid *) (((intptr_t) this->asteroidTarget)))->setRotationEnabled(true);
        ((char &) this->dockingState) = 0;
        this->field_0x145 = 0;
        this->asteroidTarget = 0;
        this->miningUpVec = Vector{0.0f, 0.0f, 0.0f};
        ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setActive(true);
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

int PlayerEgo::levelCollision() {
    return 0;
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
            *(Vector *) gun->positions = Vector{50000.0f, 50000.0f, 50000.0f};
            (*(*guns)[1])[i]->active = false;
        }
    }
    return true;
}

void Mat_identity(void *out, const void *src); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)


// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

static inline void Mat_setRotation(void *out, float x, float y, float z) { AbyssEngine::AEMath::MatrixSetRotation(*(AbyssEngine::AEMath::Matrix*)out, x, y, z); } // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PlayerEgo::roll(int amount) {
    if (this->rolling == 0)
        return;

    AbyssEngine::AEMath::Matrix &m = ((AEGeometry *) (this->geometry))->getMatrix();
    float rx = m.m[4];
    float ry = m.m[5];
    float mag = rx > 0.0f ? rx : -rx;

    if (amount > 0x3b)
        amount = 0x3c;

    unsigned char rollMat[0x30];

    if (ry >= 0.0f && mag < g_PE_rollLevelEps) {
        Mat_identity(rollMat, &this->rollMatrix);
        Mat_assign(&this->rollMatrix, rollMat);
        this->rollDirection = 0;
        this->rolling = 0;
        this->autoLevel = 0;
        return;
    }

    float zAxis = 0.0f;
    float xScale = PE_roll_bankFactor(this, rx, ry, &zAxis);

    if (rx != 0.0f)
        this->rollDirection = (rx < 0.0f) ? 1 : 2;

    this->rolling = 1;
    Mat_setRotation(rollMat, xScale * (float) amount, 0.0f, zAxis);
    Mat_assign(&this->rollMatrix, rollMat);
}

void PlayerEgo::setTargetFollowCamera(TargetFollowCamera *cam) {
    this->targetFollowCamera = (int) (intptr_t) cam;
    cam->setShipHandling(this->handling);
}

void PlayerEgo::calcCollision(Array<KIPlayer *> *candidates) {
    if (candidates == 0)
        return;
    if (this->dockedFlag != 0 && (unsigned) (this->dockingPointIndex - 1) < 3)
        return;

    for (unsigned i = 0; i < candidates->size(); i++) {
        KIPlayer *obj = (*candidates)[i];
        if (obj == 0)
            continue;

        if (i == 0 && PE_status()->inAlienOrbit() == 0) {
            if (Vec_length(&this->dockOffsetVec) < g_PE_cc_alarmDist
                && ((KIPlayer *) obj)->lostMissionCrateToEgo != 0)
                this->collidesWithStationFlag = 1;
        }

        AbyssEngine::AEMath::Vector pos = ((AEGeometry *) (this->geometry))->getPosition();
        if (((KIPlayer *) obj)->outerCollide(pos) == 0)
            continue;

        if (((KIPlayer *) (obj))->getType() == 0x4262 && ((KIPlayer *) (obj))->isVisible() != 0) {
            if (((PlayerWormHole *) (obj))->isShrinking() == 0 && ((intptr_t) this->miningGame) == 0)
                PE_cc_wormhole(this, obj);
        } else if (reinterpret_cast<int &>(((KIPlayer *) obj)->stealFlagByte) == 0) {
            if (((KIPlayer *) (obj))->isVisible() != 0) {
                bool docking = (((char &) this->dockingState) != 0 || this->dockedFlag != 0)
                               && (KIPlayer *) (intptr_t) this->asteroidTarget == obj;
                if (!docking)
                    PE_cc_obstacle(this, obj, i);
                hitCamera_(this);
            }
        } else {
            if (((KIPlayer *) (obj))->isDying() == 0 && ((KIPlayer *) (obj))->isDead() == 0) {
                bool skipDockTarget = (((char &) this->dockingState) != 0 || this->dockedFlag != 0)
                                      && obj == (KIPlayer *) (intptr_t) this->asteroidTarget;
                if (!skipDockTarget) {
                    PE_cc_destructible(this, obj);
                    hitCamera_(this);
                }
            }
        }
    }
}

void PlayerEgo::dockToPlanet() {
    ((TargetFollowCamera *) (this->targetFollowCamera))->setLookAtCam(true);
    this->boostTimer = 0;
    this->boostingFlag = 1;
    ((float &) this->speed) = 8.0f;
    this->boostTime = 10000;
    this->boostDelay = 0;
    this->collide = 0;
    this->resetGunDelay();
    this->dockingToPlanet = 1;
    this->shieldVisible = 0;
    ((float &) this->speed) = 8.0f;
    this->planetDockTimer = 0;
    Globals::sound->play(5, nullptr, nullptr, 0.0f);
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

void Mat_mul(void *out, const void *a, const void *b); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PlayerEgo::shoot(int weapon, int type) {
    int hi = weapon >> 31;
    if (this->turretActive != 0) {
        float *m1 = ((AEGeometry *) (this->geometry))->getMatrix();
        float *m2 = ((AEGeometry *) (this->turretGeometry))->getMatrix();
        float combined[0x30 / sizeof(float)];
        Mat_mul(combined, m1, m2);
        Matrix mat;
        for (int k = 0; k < 15; ++k) mat.m[k] = combined[k];
        mat.m[15] = 1.0f;
        long long pos = ((long long) hi << 32) | (unsigned int) weapon;
        static_cast<Player *>(this->player)->shoot(2, pos, hi < 0, mat);
        return;
    }

    if (((PlayerEgo *) (this))->isDead() != 0)
        return;

    Player *player = static_cast<Player *>(this->player);
    if (type == 1) {
        int idx = this->currentSecondaryWeaponIndex;
        long long pos = ((long long) hi << 32) | (unsigned int) idx;
        player->shoot(1, idx, pos, hi < 0);
    } else {
        long long pos = ((long long) hi << 32) | (unsigned int) weapon;
        player->shoot(type, weapon, pos, hi < 0);
    }
}

void PlayerEgo::stopMining() {
    MiningGame *mg = (MiningGame *) (intptr_t) this->miningGame;
    if (mg != 0) {
        delete (MiningGame *) mg;
        this->miningGame = 0;
    }
}

bool PlayerEgo::setTurretMode(bool enable) {
    if (this->turretMode == 0 || this->miningGame != 0 || this->autoTurretEquipped != 0) {
        if (this->rocketControlGun != 0) {
            Globals::Canvas->CameraSetCurrent(((TargetFollowCamera *) (intptr_t) this->targetFollowCamera)->id);
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
        Globals::Canvas->CameraSetCurrent(((TargetFollowCamera *) (intptr_t) this->targetFollowCamera)->id);
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

void PlayerEgo::rotate(float rx, float ry, float rz) {
    this->rotateX = this->rotateX + rx;
    this->rotateY = this->rotateY + ry;
    this->rotateZ = this->rotateZ + rz;
    AbyssEngine::AEMath::Matrix &m =
        *(AbyssEngine::AEMath::Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
    AbyssEngine::AEMath::MatrixSetRotation(m, this->rotateX, this->rotateY, this->rotateZ);
}


void PlayerEgo::strafe(int /*dir*/, bool positive) {
    if (this->rocketControlGun != 0)
        return;

    float base;
    if (this->hardCoreMode != 0) {
        float cur = (float) ((Ship *) (PE_status()->getShip()))->getCurrentLoad();
        float max = (float) ((Ship *) (PE_status()->getShip()))->getMaxLoad();
        float rate = this->handling;
        base = rate * (1.0f - cur / max) * g_PE_strafeLoadK + rate * g_PE_strafeLoadB;
    } else {
        base = this->handling;
    }

    float accel = (positive ? 1.0f : -1.0f) * PE_minf(base * 30.0f * g_PE_strafeAccelK, 2.0f);
    this->strafeAccel = ((float &) this->strafeNavPoint) * accel;
    ((float &) this->strafeNavPoint) = PE_minf(((float &) this->strafeNavPoint) * 1.5f, 1.0f);
}

void PlayerEgo::dockToDockingPoint(KIPlayer *kip, Radar *radar) {
    (void) kip;
    if (((PlayerEgo *) (this))->isDead() != 0)
        return;

    if (this->dockedFlag == 0) {
        if (radar != 0) {
            this->dockedFlag = 1;
            this->asteroidTarget = (int) (intptr_t) radar;
            this->dockScaling = 0x578;
            this->miningSettleTimer = 0;
            this->field_0x145 = 1;
            this->dockingPointIndex = 0;
        }
        return;
    }

    bool undock = (radar == 0);
    if (!undock) {
        Vector pos = ((PlayerEgo *) (this))->getPosition();
        SpacePoint *sp = (SpacePoint *) (unsigned long) this->spacePoint;
        SpacePoint *nav = ((KIPlayer *) (radar))->getNearestNavigationPoint(pos, sp);
        if (nav == 0) {
            if (radar->hasReservation != 0)
                radar->reservationDirty = 1;
            undock = true;
        } else {
            if (radar->hasReservation != 0)
                radar->reservationDirty = 1;

            ((PlayerEgo *) (this))->setTurretMode(0);
            this->field_0x1a1 = 0;
            Globals::Canvas->CameraSetCurrent(
                (unsigned int) (((TargetFollowCamera *) (intptr_t) this->targetFollowCamera)->id));
            ((LevelScript *) (this->levelScript))->resetCamera(this->levelScript->m_pLevel);
            this->PlayEngineSound();
            this->dockingPointIndex = 3;
            ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setLookAtCam(false);
            ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->useTargetsUpVector(false);

            if (this->easeMatrix != 0) {
                ((AbyssEngine::EaseInOutMatrix *) this->easeMatrix)->~EaseInOutMatrix();
                ::operator delete(this->easeMatrix);
            }
            this->easeMatrix = 0;

            pos = ((PlayerEgo *) (this))->getPosition();
            SpacePoint *nav2 = ((KIPlayer *) (radar))->getNearestNavigationPoint(pos, sp);
            float *from = ((AEGeometry *) (this->geometry))->getMatrix();
            this->easeMatrix = (AbyssEngine::EaseInOutMatrix *) PE_dtdp_makeEase(from, nav2);
            ((PlayerEgo *) (this))->setExhaustVisible(true);
        }
    }

    if (undock) {
        this->PlayEngineSound();
        this->dockedFlag = 0;
        this->field_0x1a1 = 0;
        this->field_0x145 = 0;
        this->asteroidTarget = 0;
        this->miningUpVec = Vector{0.0f, 0.0f, 0.0f};
        ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setActive(true);
        ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setLookAtCam(false);
        ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->useTargetsUpVector(false);
        ((Player *) (this->player))->resetGunDelay(0);
        this->dockingPointIndex = 0;
        ((PlayerEgo *) (this))->setExhaustVisible(true);
        if (this->spacePoint != 0) {
            ((SpacePoint *) ((unsigned long) this->spacePoint))->giveFree();
            this->spacePoint = 0;
        }
    }

    if (((intptr_t) this->hackingGame) != 0) {
        delete (HackingGame *) ((intptr_t) this->hackingGame);
        this->hackingGame = 0;
        ((Hud *) (((intptr_t) this->hud)))->setHackingGameActive(false);
    }
}

void PlayerEgo::draw(bool allowHud) {
    if (((intptr_t) this->rocketControlGun) != 0)
        return;

    if (((intptr_t) this->hackingGame) != 0 && this->turretActive == 0) {
        ((HackingGame *) (intptr_t) this->hackingGame)->render2D();
        return;
    }

    if (((intptr_t) this->miningGame) != 0) {
        ((MiningGame *) (intptr_t) this->miningGame)->render2D();
        return;
    }

    bool full;
    if (this->computerControlled != 0 || ((PlayerEgo *) (this))->isDead() != 0 || this->freeze != 0
        || allowHud == 0 || ((char &) this->dockingState) != 0 || this->dockingToPlanet != 0 || this->dockedToStream !=
        0) {
        full = false;
    } else {
        full = true;
    }

    if (!full) {
        if (this->autoPilot != 0) {
            this->drawThrottle();
            return;
        }
        if (this->dockedFlag == 0)
            return;
        if ((unsigned) (this->dockingPointIndex - 1) < 3)
            return;
        this->drawThrottle();
        return;
    }

    PE_MatrixBuf m;
    for (unsigned i = 0; i < sizeof(m); i++) ((unsigned char *) &m)[i] = 0;
    m.m00 = 1.0f;
    m.m11 = 1.0f;
    m.m22 = 1.0f;

    bool aligned = (this->autoPilot != 0 && this->turretActive != 0);
    if (!aligned)
        Mat_assign(&m, ((AEGeometry *) (this->geometry))->getMatrix());

    PaintCanvas *canvas = Globals::Canvas;
    Globals::Canvas->SetColor((unsigned int) (0xffffffff));

    if (this->turretActive != 0
        && ((Ship *) (PE_status()->getShip()))->getFirstEquipmentOfSort(0x23) != 0) {
        if (((Radar *) (this->field_0x14))->isPlasmaInRange() != 0) {
            float *p = g_PE_dr_posLock;
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->radarBlipImage3), (int) p[0],
                                                           (int) p[1], (unsigned char) (0x11), (unsigned char) (0x44));
        } else {
            float *p = g_PE_dr_posNoLock;
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->radarBlipImage2), (int) p[0],
                                                           (int) p[1], (unsigned char) (0x11), (unsigned char) (0x44));
        }
    } else {
        if ((char &) this->level->field_30 != 0) {
            float *p = g_PE_dr_posBlink;
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->radarBlipImage1), (int) p[0],
                                                           (int) p[1], (unsigned char) (0x11), (unsigned char) (0x44));
            int t = this->shakeAccum + this->shakeIntensity;
            this->shakeAccum = t;
            if (t >= 0xc9)
                (char &) this->level->field_30 = 0;
        } else {
            float *p = g_PE_dr_posNormal;
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->dockArrowImage), (int) p[0],
                                                           (int) p[1], (unsigned char) (0x11), (unsigned char) (0x44));
            this->shakeAccum = 0;
        }
    }

    ((PlayerEgo *) (this))->drawThrottle();
}

void PE_upd_subsystems(PlayerEgo *self, int dt, void *radar, void *hud, // lint: void_ptr (external symbol; param/return types mangling-load-bearing)
                       // lint: void_ptr (external symbol; param/return types mangling-load-bearing)
                       void *radio, // lint: void_ptr (external symbol; param/return types mangling-load-bearing)
                       void *script); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PE_upd_post(PlayerEgo *self, int dt, void *radar, void *hud, // lint: void_ptr (external symbol; param/return types mangling-load-bearing)
                 // lint: void_ptr (external symbol; param/return types mangling-load-bearing)
                 void *radio, int arg5); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PlayerEgo::update(int dt, Radar *radar, Hud *hud, Radio *radio, LevelScript *script, int arg5, bool arg6,
                       int arg7) {
    (void) arg6;
    (void) arg7;
    (void) script;

    if (((intptr_t) this->hud) == 0)
        return;
    if (this->freeze != 0)
        return;

    float pos[3];
    ((PlayerEgo *) (this))->getPosition();
    ((int &) this->dockOffsetVec.x) = *(int *) &pos[0];
    ((int &) this->dockOffsetVec.y) = *(int *) &pos[1];
    this->boostSoundId = *(int *) &pos[2];

    PE_upd_subsystems(this, dt, radar, hud, radio, script);

    PE_upd_boost(this, dt);

    if (((char &) this->dockingState) != 0) {
        this->field_0x145 = 1;
        if (((intptr_t) this->asteroidTarget) == 0 || ((PlayerEgo *) (this))->isDead() != 0) {
            ((PlayerEgo *) (this))->dockToAsteroid(nullptr, nullptr);
            PE_upd_post(this, dt, radar, hud, radio, arg5);
            return;
        }
        ((PlayerEgo *) (this))->approachAsteroid(hud, (int) (intptr_t) hud, radar);
    }

    if (this->dockedFlag != 0 && ((Player *) (this->player))->getHitpoints() > 0) {
        this->field_0x145 = 1;
        if (((intptr_t) this->asteroidTarget) == 0 || ((PlayerEgo *) (this))->isDead() != 0) {
            ((PlayerEgo *) (this))->dockToDockingPoint(nullptr, nullptr);
            PE_upd_post(this, dt, radar, hud, radio, arg5);
            return;
        }
        if (((PlayerEgo *) (this))->approachDockingPoint(hud, 0, radar) != 0)
            PE_upd_docksFinishDelivery(this, radio);
    }

    if (this->turretActive != 0 || this->field_0x1a1 != 0)
        ((PlayerEgo *) (this))->handleTurretView(dt);

    bool autopilot = (this->autoPilot != 0 && this->autoPilotTarget != 0);
    if (!autopilot) {
        if (this->field_0x145 == 0) {
            if (this->turretActive == 0 && this->field_0x1a1 == 0) {
                ((PlayerEgo *) (this))->roll(this->shakeIntensity);
                if (((PlayerEgo *) (this))->updateManeuver() == 0)
                    ((PlayerEgo *) (this))->handleShip(dt);
            } else {
                ((PlayerEgo *) (this))->handleTurretView(dt);
            }
        }
    } else if (this->field_0x145 == 0 && ((PlayerEgo *) (this))->updateManeuver() == 0) {
        Waypoint *wp = (Waypoint *) (intptr_t) this->autoPilotTarget;
        if (this->goingToWaypointFlag != 0 && wp != 0 && ((Waypoint *) wp)->route != 0) {
            wp = (Waypoint *) ((Route *) (this->autoPilotTarget))->getWaypoint();
            this->autoPilotTarget = (int) (intptr_t) wp;
        }
        if (wp == 0 || this->dockedToStream != 0) {
            this->setAutoPilot(nullptr);
        } else {
            AbyssEngine::AEMath::Vector wpVec = ((KIPlayer *) wp)->getPosition();
            float wpPos[3] = {wpVec.x, wpVec.y, wpVec.z};
            ((int &) this->waypointX) = *(int *) &wpPos[0];
            ((int &) this->waypointY) = *(int *) &wpPos[1];
            ((int &) this->waypointZ) = *(int *) &wpPos[2];

            float me[3];
            ((PlayerEgo *) (this))->getPosition();
            float dx = me[0] - wpPos[0], dy = me[1] - wpPos[1], dz = me[2] - wpPos[2];
            if ((int) (dx * dx + dy * dy + dz * dz) < 20000)
                this->aboutToReachAutoTargetFlag = 1;

            float speed = 4.0f;
            float h = ((Ship *) (PE_status()->getShip()))->getHandling();
            if (h + g_PE_upd_handlingBias < 4.0f)
                speed = ((Ship *) (PE_status()->getShip()))->getHandling() + g_PE_upd_handlingBias;
            ((PlayerEgo *) (this))->moveToPosition(Vector{
                                                       this->waypointX, this->waypointY, this->waypointZ
                                                   }, true, speed);
            if (this->turretActive != 0)
                ((PlayerEgo *) (this))->handleTurretView(dt);
        }
    }

    if (this->dockedFlag != 0 && ((uint32_t &) this->dockingPointIndex) != 1
        && this->turretActive == 0 && (((uint32_t &) this->dockingPointIndex) | 1) != 3)
        ((PlayerEgo *) (this))->updateManeuver();

    if (this->autoTurretEquipped != 0 && this->autoTurretEnabled != 0) {
        if (((PlayerEgo *) (this))->isDead() == 0) {
            ((PlayerEgo *) (this))->handleAutoTurret(dt);
        } else {
            this->autoTurretEnabled = 0;
            ((Player *) (this->player))->stopShooting(0);
        }
    }

    PE_upd_post(this, dt, radar, hud, radio, arg5);
}

void PlayerEgo::setTurretPosition(Vector v) {
    this->turretOffsetVec = v;
}

void PlayerEgo::revive() {
    this->level->field_74->enableSystemEmit(this->currentSystem, false);
    this->level->field_74->enableSystemRender(this->currentSystem, false);

    if (this->explosion != nullptr)
        delete this->explosion;
    this->explosion = nullptr;

    ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setActive(true);
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

void PlayerEgo::drawThrottle() {
    if (this->throttleStarted == 0)
        return;

    int t = this->throttle;
    if (t > 500)
        t = 2000 - t;
    float frac = (float) t / g_PE_t_timerDiv;
    if (frac > 1.0f)
        frac = 1.0f;

    PaintCanvas *canvas = Globals::Canvas;
    Globals::Canvas->SetColor((unsigned int) (0xffffff00 | (unsigned int) (int) (frac * 255.0f) - 0x100));

    int img = this->hpGaugeImage;
    int w = ((PaintCanvas *) (long) (canvas))->GetImage2DWidth((unsigned int) (img));
    int h = ((PaintCanvas *) (long) (canvas))->GetImage2DHeight((unsigned int) (img));

    float thrust = this->thrust;
    int fillH = (int) (thrust * (float) h);
    float *anchor = g_PE_t_anchor;

    ((PaintCanvas *) (long) (canvas))->DrawRegion2D((unsigned int) (img), 0, h - fillH, w, fillH,
                                                    (float) ((int) ((anchor[1] + (float) h) - (float) fillH)), 0, 0, 0,
                                                    (int) (anchor[0] - (float) (w / 2)));

    unsigned char pct[12];
    ((String *) (pct))->Set((long long) (int) (thrust * g_PE_t_pctScale));

    int th = ((PaintCanvas *) (long) (canvas))->GetImage2DHeight((unsigned int) (img));
    String *pctStr = *g_PE_t_pctStr;
    int tw = ((PaintCanvas *) (long) (canvas))->GetTextWidth((unsigned int) (long) (canvas), *pctStr);
    ((PaintCanvas *) (long) (canvas))->DrawString((unsigned int) (long) (canvas), *pctStr, (int) (long) (pct),
                                                  (int) ((anchor[0] - (float) (tw / 2)) - 1.0f),
                                                  (bool) (int) (anchor[1] + (float) th / g_PE_t_textDiv));
    Globals::Canvas->SetColor((unsigned int) (0xffffffff));

    {
        String *_s = ((String *) (pct));
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
}

void PlayerEgo::setAutoPilot(KIPlayer *kip) {
    this->goingToWaypointFlag = 0;
    int v = (int) (intptr_t) kip;
    unsigned char old = this->autoPilot;
    this->autoPilotTarget = v;
    this->autoPilot = (v != 0) ? 1 : 0;
    if (v == 0) {
        ((KIPlayer *) this->field_0x14)->autoPilotState = 0;
        if (old != 0) {
            this->field_0x2a4 = 0;
            this->field_0x2a8 = 0;
        }
        return;
    }
    if (kip->field_0x72 != 0) this->goingToWaypointFlag = 1;
    AbyssEngine::Engine *eng = (AbyssEngine::Engine *) Globals::appManager->GetEngine();
    ((AbyssEngine::Engine *) eng)->autoPilotEngaged = 0;
    ((int &) this->thrust) = 0x3f800000;
}

int PlayerEgo::updateManeuver() {
    unsigned int type = ((uint32_t &) this->maneuverType);

    if ((type - 1) < 2) {
        ((int &) this->navPoint) = ((int &) this->explosion) + ((int &) this->navPoint);
        PE_um_dodgeStep(this);
        if (((int &) this->navPoint) > 0x4af) {
            LevelScript *level = this->levelScript;
            this->maneuverType = 0;
            ((LevelScript *) (level))->resetCamera(((LevelScript *) level)->m_pLevel);
        }
        return 1;
    }

    if (type != 3)
        return 0;

    ((int &) this->navPoint) = ((int &) this->explosion) + ((int &) this->navPoint);

    float target[3];
    PE_um_strafeTarget(this, target);
    if (((int &) this->navPoint) > 900) {
        target[0] = this->strafeTargetVec.x;
        target[1] = this->strafeTargetVec.y;
        target[2] = this->strafeTargetVec.z;
    }
    ((PlayerEgo *) (this))->moveToPosition(Vector{target[0], target[1], target[2]}, true, 0.0f);

    PE_um_strafeGlide(this);
    ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setLookAtCam(false);
    ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->useTargetsUpVector(false);

    if (((int &) this->navPoint) > 2999) {
        this->maneuverType = 0;
        ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->setLookAtCam(false);
        ((TargetFollowCamera *) (((intptr_t) this->targetFollowCamera)))->useTargetsUpVector(false);
    }
    return 1;
}

int PlayerEgo::getHullDamageRate() {
    return ((Player *) this->player)->getArmorDamageRate();
}

void Mat_mul(void *out, const void *a, const void *b); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

// lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void Mat_mulEq(void *acc, const void *b); // lint: void_ptr (external symbol; param/return types mangling-load-bearing)

void PlayerEgo::handleTurretView(int dt) {
    bool move = true;
    if (((intptr_t) this->autoPilotTarget) != 0 && this->dockedToStream == 0)
        move = true;
    if (this->dockedFlag != 0 && this->dockingPointIndex == 1)
        move = false;
    if (((intptr_t) this->autoPilotTarget) == 0)
        move = true;
    if (move) {
        float d = (float) dt;
        ((AEGeometry *) (this->geometry))->moveForward(this->thrust * d * ((float &) this->speed));
    }

    unsigned char look[0x30];
    Mat_mul(look, ((AEGeometry *) (this->geometry))->getMatrix(), ((AEGeometry *) (this->dockCameraNode))->getMatrix());
    Mat_mulEq(look, ((AEGeometry *) (this->dockCameraMid))->getMatrix());

    if (this->hitShake != 0 || this->boostingFlag != 0) {
        unsigned char eye[12], up[12], dir[12], rel[12];
        Mat_getPosition(eye, look);
        Mat_getUp(up, look);
        Mat_getDir(dir, look);
        Vec_sub(rel, dir, eye);

        if (this->hitShake != 0) {
            int t = this->hitShakeTimer + dt;
            this->hitShakeTimer = t;
            if (t > 1000)
                this->hitShake = 0;
        }
        PE_htv_applyShake(this, dt, eye, rel);

        unsigned char lookAt[0x30];
        Mat_getLookAt(lookAt, eye, rel, up);
        Mat_assign(look, lookAt);
    }

    unsigned int cam = (unsigned int) (unsigned long) Globals::Canvas;
    Globals::Canvas->CameraSetLocal((unsigned int) (cam),
                                    *(const AbyssEngine::AEMath::Matrix *) &this->turretCamera);

    this->pitchAccumDir = 0;
    this->yawAccumDir = 0;
    ((PlayerEgo *) (this))->roll(this->shakeIntensity);

    unsigned int hull = (unsigned int) (long) Globals::Canvas->TransformGetLocal(
        (unsigned int) (this->geometry->transform));
    unsigned int ret = (unsigned int) (long) Globals::Canvas->TransformGetLocal(
        (unsigned int) (this->field_0x4->transform));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (AbyssEngine::AEMath::Matrix *) (unsigned long) hull,
            (AbyssEngine::AEMath::Matrix *) (unsigned long) ret);
    Mat_assign(((Player *) this->player)->transform, tmp);
}

void PlayerEgo::approachAsteroid(Hud *hud, int hud2, Radar *radar) {
    (void) hud;
    if (((KIPlayer *) (this))->isDying() != 0)
        return;

    if (this->dockingPointIndex == 1) {
        float settle = ((float &) this->miningSettleTimer);
        if (settle < g_PE_aa_settleEps) {
            ((float &) this->miningSettleTimer) = settle + (float) (-(this->shakeIntensity) >> 1);
            return;
        }

        if (((intptr_t) this->miningGame) == 0) {
            this->field_0x2f5 = 0;
            this->lostMiningGameFlag = 0;
            ((MiningHostObject *) *g_PE_aa_levelHolder)->miningActiveFlag = 0;
            MiningGame *mg = new MiningGame(((PlayerAsteroid *) (((intptr_t) this->asteroidTarget)))->getQuality(),
                                            ((PlayerAsteroid *) ((intptr_t) this->asteroidTarget))->asteroidIndex,
                                            (Hud *) (unsigned long) hud2);
            this->miningGame = (int) (intptr_t) mg;
            ((PE_AsteroidMineTarget *) (intptr_t) this->asteroidTarget)->miningFlag = 0;

            int snd = *g_PE_aa_mineSound;
            ((FModSound *) (snd))->play(1, (Vector *) 0, (Vector *) 0, 0);
            ((FModSound *) ((unsigned long) snd))->pause(0);
            return;
        }

        int running = ((MiningGame *) (((intptr_t) this->miningGame)))->update(this->shakeIntensity);
        if (running == 0) {
            if (((MiningGame *) (((intptr_t) this->miningGame)))->gameLost() == 0
                && ((MiningGame *) (((intptr_t) this->miningGame)))->getOreAmount() > 0) {
                ((PlayerEgo *) (this))->stopMining();
            } else if (((MiningGame *) (((intptr_t) this->miningGame)))->gameLost() != 0) {
                this->lostMiningGameFlag = 1;
                ((MiningHostObject *) *g_PE_aa_winHolder1)->miningResultSlot = 0;
                ((PlayerEgo *) (this))->stopMining();
                ((Hud *) ((unsigned long) hud2))->hudEvent(8, this, 0);
            }
        } else if (((KIPlayer *) (this))->isDying() != 0 || ((KIPlayer *) (this))->isDead() != 0) {
            ((MiningHostObject *) *g_PE_aa_winHolder2)->miningResultSlot = 0;
            ((PlayerEgo *) (this))->stopMining();
            ((Hud *) ((unsigned long) hud2))->hudEvent(8, this, 0);
        }
        return;
    }

    if (this->dockingPointIndex == 0) {
        this->dockingPointIndex = PE_aa_approachStep(this, hud2, radar);
    }
}

void PlayerEgo::handleShip(int dt) {
    FModSound *snd = Globals::sound;

    snd->setParamValue((FMOD::Event *) (long) ((Player *) (this->player))->GetEngineEvent(), 0,
                       ((float &) this->yawAccumD));
    snd->setParamValue((FMOD::Event *) (long) ((Player *) (this->player))->GetEngineEvent(), 1,
                       ((float &) this->rollAccum) * g_PE_hs_throttleBias + 0.5f);

    unsigned int tf = reinterpret_cast<unsigned int &>(Globals::Canvas->initialized);

    PE_handleShip_orient(this, dt, tf);

    ((int &) this->pitchRamp) = 0;
    ((int &) this->yawRamp) = 0;
    this->pitchAccumDir = 0;
    this->yawAccumDir = 0;
    ((int &) this->yawAccumD) = 0;
    ((int &) this->rollAccum) = 0;

    unsigned int hull = (unsigned int) (long) Globals::Canvas->TransformGetLocal(
        (unsigned int) (this->geometry->transform));
    unsigned int ret = (unsigned int) (long) Globals::Canvas->TransformGetLocal(
        (unsigned int) (this->field_0x4->transform));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (AbyssEngine::AEMath::Matrix *) (unsigned long) hull,
            (AbyssEngine::AEMath::Matrix *) (unsigned long) ret);
    Mat_assign(((Player *) this->player)->transform, tmp);
}

void PlayerEgo::stopBoost() {
    ((float &) this->speed) = 2.0f;
    this->boostingFlag = 0;
    Globals::sound->stop(0x27);
    Globals::sound->stop(0x26);
    Globals::sound->stop(0x29);
    Globals::sound->stop(0x28);
    Globals::sound->stop(0x44e);
}

void PlayerEgo::setShip(int race, int group) {
    this->field_0x4 = Globals::globals->getShipGroup(race, group, true);
    AbyssEngine::Mesh *mesh = Globals::Canvas->MeshGetPointer(this->field_0x4->meshId);
    this->field_0x394 = ((PE_MaterialBlock *) mesh->material)->materialList;

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

void PlayerEgo::addGun(Array<Gun *> *arr, int x) {
    static_cast<Player *>(this->player)->addGun(arr, x);
    this->checkForTurret();
}

void PlayerEgo::render(bool allowHud) {
    Level *level = this->level;

    if (((PlayerEgo *) (this))->isDead() == 0) {
        if (this->field_0x309 == 0)
            return;

        if (this->explosion != 0) {
            ((Explosion *) (this->explosion))->render();
            if (this->explosionTimer <= 0xbb7)
                ((AEGeometry *) (this->geometry))->render();
        }
        if (this->explosion2 != 0)
            ((Explosion *) (this->explosion2))->render();

        ((AEGeometry *) (this->geometry))->render();

        if (((intptr_t) this->field_0xac) != 0 && this->emergencySystemTimer >= 1)
            ((AEGeometry *) (this->geometry))->render();

        if (this->shieldVisible != 0)
            ((AEGeometry *) (this->shieldGeometry))->render();

        if (this->turretMode != 0) {
            if (this->turretExtraGeo != 0)
                ((AEGeometry *) (this->turretExtraGeo))->setVisible(this->turretActive != 0);
            ((AEGeometry *) (this->turretRoot))->render();
        }

        if (this->tractorBeam != 0)
            ((TractorBeam *) (this->tractorBeam))->render();

        Array<RepairBeam *> *beams = this->repairBeams;
        if (beams != 0) {
            for (unsigned int i = 0; i < beams->size(); i++) {
                (*beams)[i]->render();
            }
        }

        int flag = 1;
        if (this->freeze == 0 && allowHud != 0)
            flag = (this->dockingPointIndex - 1) != 0 ? 1 : 0;
        else
            flag = 1;
        level->enableMovingStars(flag != 0);
    } else {
        if (this->explosion != 0) {
            ((Explosion *) (this->explosion))->render();
            if (this->explosionTimer < 3000)
                ((AEGeometry *) (this->geometry))->render();
        }
        if (((intptr_t) this->field_0xac) != 0)
            ((Explosion *) (this->explosion))->render();
        level->enableMovingStars(true);
    }
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
        ((Hud *) (intptr_t) this->hud)->hudEvent(30, this, energy);
        ((Hud *) (intptr_t) this->hud)->hudEvent(28, this, 0);
        return true;
    }
    return this->cloaked != 0;
}

AbyssEngine::AEMath::Vector PlayerEgo::crosshairPos;
AbyssEngine::AEMath::Vector PlayerEgo::crosshairShootPos;
AbyssEngine::AEMath::Vector PlayerEgo::vec_up;
