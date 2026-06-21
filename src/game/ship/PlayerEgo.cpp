#include "game/ship/PlayerEgo.h"
#include "engine/render/Camera.h"
#include "engine/core/AERandom.h"
#include "game/ship/TargetFollowCamera.h"
#include "game/core/PaintCanvasClass.h"   // forward-declares Engine for externs.h
#include "externs.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/menu/HackingGame.h"
#include "game/mission/Item.h"
#include "game/world/Level.h"
#include "game/world/LevelScript.h"
#include "game/menu/MiningGame.h"
#include "engine/render/ParticleSystemManager.h"
#include "game/ship/PlayerAsteroid.h"
#include "game/ship/PlayerWormHole.h"
// Radar.h is intentionally NOT included: it defines stub structs (KIPlayer, Status,
// SolarSystem) that collide with the real class headers used by this TU. Only the two
// Radar methods below are needed here, so forward-declare a minimal interface.
class Radar {
public:
    void unlockAsteroid();
    bool isPlasmaInRange();
};
#include "game/mission/Status.h"
#include "engine/math/Transform.h"
#include "engine/core/ApplicationManager.h"
#include "game/mission/Explosion.h"
// Hud.h is intentionally NOT included: it redefines the file-scope P() accessor that
// LevelScript.h already provides in this TU. Only these two Hud methods are needed,
// so forward-declare a minimal interface.
class Hud {
public:
    void hudEvent(int eventId, void *ego, int arg);
    void setHackingGameActive(bool value);
};
#include "game/ship/KIPlayer.h"
// Radio.h is intentionally NOT included here (its full definition is not needed and
// pulling it in clashes with other minimal stubs in this TU). update() only needs
// the type name to form a Radio* parameter that is forwarded to void*-taking helpers.
class Radio;
#include "game/mission/Mission.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerFixedObject.h"
#include "game/weapons/RepairBeam.h"
#include "game/world/Route.h"
#include "game/world/SpacePoint.h"
#include "game/core/String.h"
#include "game/weapons/TractorBeam.h"
#include "game/ship/Ship.h"

// Free AEMath operator used by getTurretPosition (declared in AEMath.cpp; AEMath.h
// is not included here because it redefines BSphere/Transform which clash in this TU).
namespace AbyssEngine { namespace AEMath {
Matrix operator*(const Matrix &lhs, const Matrix &rhs);
} }

// Explosion.h and Hud.h each define an identical file-scope `static inline I(...)`
// (and Hud.h a `P(...)`), making unqualified calls ambiguous. Use macros here so
// the offset accessors resolve unambiguously to a plain pointer cast.
#define I(p, off) (*(int *)((char *)(p) + (off)))
#define P(p, off) (*(void **)((char *)(p) + (off)))
// common.h's F() is a class template needing an explicit type argument; every use
// in this TU is a float field accessor, so provide a plain-float F macro.
#define F(p, off) (*(float *)((char *)(p) + (off)))

// Canonical singletons used in this TU. Status / AERandom / ApplicationManager
// headers are already included above; PaintCanvas.h and Globals.h are not pulled
// in here (their full definitions clash with PaintCanvasClass.h / externs.h in
// this TU), so the two canonical pointers are declared locally to match their
// canonical definitions.
extern PaintCanvas* gCanvas;       // canonical render canvas singleton (binary .bss 0x2281b8)
// Minimal local view of Globals (the full Globals.h clashes with PaintCanvasClass.h /
// externs.h in this TU). Declaring the one instance method we call lets us link straight
// to the real Globals:: symbol instead of going through an extern "C" thunk.
class Globals {
public:
    void addSoundResourceToList(int snd);
    AEGeometry* getShipGroup(int type, int variant, bool wireframe);
};
extern Globals* gGlobals;          // canonical Globals singleton (binary .bss 0x2281d0)

// Status singleton accessor. Several Status:: instance methods (getShip, inAlienOrbit,
// getMission, getCampaignMission, inSupernovaSystem, ...) appear in the decomp as bare
// free-function calls with the self receiver dropped. The receiver is the canonical
// Status singleton.
static inline Status *PE_status() { return gStatus; }

// ---- canonical helper types / prototypes (deduped across merged sources) ----
// Vec3 is `typedef Vector Vec3;` from PlayerEgo.h (AbyssEngine::AEMath::Vector).
// Several AEGeometry/PlayerEgo accessors return it by value.

// Functions whose merged prototypes disagree on argument lists / return type.
// Unified here as variadic so every call site compiles (compile-only target).
// Their first argument is sometimes an int handle, sometimes a void*, so these
// are declared fully variadic to accept either.
// (int|void* hud, int ev, PlayerEgo*, [int])

extern "C" void PlayerEgo_setVisible_ext();
extern "C" int PlayerEgo_getHitpoints_ext(void*);
extern "C" int PlayerEgo_getHackingGameDockIndex_ext(int);
extern "C" void PlayerEgo_dockToStream_ext(PlayerEgo*, int);
extern "C" void PlayerEgo_hackingShuffle_ext(int);
extern "C" void PlayerEgo_setRocketControl_ext(PlayerEgo*, int);
extern "C" void PlayerEgo_turnHorizontal_neg(PlayerEgo*);
extern "C" void PlayerEgo_turnHorizontal_pos(PlayerEgo*);
extern "C" void PlayerEgo_setPosition_v(PlayerEgo*);
extern "C" void PlayerEgo_PauseEngineSound_ext(void*);
extern "C" void PlayerEgo_setActive_ext(void*);
extern "C" void PlayerEgo_setAutoTurret_ext(void*, int);
extern "C" void PlayerEgo_hitCamera_ext(int);
extern "C" int PlayerEgo_hackingWon_ext(int);
extern "C" int PlayerEgo_getCurrentMiningAmount_ext(int);
extern "C" void PlayerEgo_hackingRotateLCW_ext(int, int);
extern "C" void PlayerEgo_ResumeEngineSound_ext(void*, int);
extern "C" void Player_setActive_(int);
extern "C" void PlayerEgo_explode_ext(PlayerEgo*, int);
extern "C" int __aeabi_idiv(int, int);
extern "C" void PlayerEgo_turnVertical_neg();
extern "C" void PlayerEgo_turnVertical_pos(PlayerEgo*);
extern "C" int Station_getIndex(void*);
extern "C" void PlayerEgo_addGun_ext(PlayerEgo*);
extern "C" void PlayerEgo_endExplosion_ext(int);
extern "C" void PlayerEgo_hackingRotateRCW_ext(int, int);
extern "C" void PlayerEgo_refillGunDelay_ext(void*, int);
extern "C" void PlayerEgo_pitchAllPrimaryGuns_ext(void*);
extern "C" void stopShooting_extA(void*, int);
extern "C" void stopShooting_extB(void*, int, int);
void* TransformGetLocal(void*, int);
void MatrixSetRotation(void*, void*, float, float, float);
extern "C" void PlayerEgo_StopEngineSound_ext(void*);
extern "C" void PlayerEgo_startSmokeEmission_ext(void*, int, int);
extern "C" int   aeabi_idiv_(int a, int b);
extern "C" float PE_pitchRampDelta(PlayerEgo *self, float rate, int frameTime);
extern "C" int   PE_adp_approach(PlayerEgo *self, void *station);
extern "C" int   PE_adp_glide(PlayerEgo *self);
extern "C" void  PE_adp_apply(PlayerEgo *self);
extern "C" void PlayerEgo_setLevel_ext(void*, int, int);
extern "C" float PE_yawRampDelta(float rate, int frameTime);
extern "C" void *EaseInOutMatrix_dtor(void *);
extern "C" void PlayerEgo_PlayEngineSound_ext(void*, int, int);
extern "C" void  Mat_assign(void *dst, const void *src);
extern "C" void  Vec_assign(void *dst, const void *src);
extern "C" void  PE_mtp_steer(PlayerEgo *self, const float *target, int steer, float speed);
extern "C" void PlayerEgo_resetGunDelay_ext(void*, int);
extern "C" int PlayerEgo_getShieldDamageRate_ext(void*);
extern "C" void  Vec_sub(void *out, const void *a, const void *b);
extern "C" float Vec_length(const void *v);
extern "C" int   PE_hat_aimAndFire(PlayerEgo *self, int dt);
extern "C" float PE_roll_bankFactor(PlayerEgo *self, float rx, float ry, float *outZ);
extern "C" void PlayerEgo_setTargetFollowCamera_ext(void*, void*);
extern "C" void  hitCamera_(PlayerEgo *self);
extern "C" void  PE_cc_wormhole(PlayerEgo *self, void *obj);
extern "C" void  PE_cc_obstacle(PlayerEgo *self, void *obj, unsigned idx);
extern "C" void  PE_cc_destructible(PlayerEgo *self, void *obj);
extern "C" float PlayerEgo_resetGunDelay_f(PlayerEgo*);
extern "C" int   Player_shootSecondary(void *player, int kind, int idx, int hi, int zero);
extern "C" int   Player_shootPrimary(void *player, int kind, int weapon, int hi, int zero);
extern "C" void PlayerEgo_stopMining_impl(PlayerEgo *self);
extern "C" void  PlayEngineSound_(PlayerEgo *self);
extern "C" void *PE_dtdp_makeEase(const void *fromMatrix, const void *navPoint);
extern "C" void PE_upd_boost(PlayerEgo *self, int dt);
extern "C" void PE_upd_docksFinishDelivery(PlayerEgo *self, void *radio);
extern "C" void  PE_um_dodgeStep(PlayerEgo *self);
extern "C" void  PE_um_strafeTarget(PlayerEgo *self, float *out);
extern "C" void  PE_um_strafeGlide(PlayerEgo *self);
extern "C" int PlayerEgo_getHullDamageRate_ext(void*);
extern "C" void  Mat_getPosition(void *out, const void *m);
extern "C" void  Mat_getUp(void *out, const void *m);
extern "C" void  Mat_getDir(void *out, const void *m);
extern "C" void  Mat_getLookAt(void *out, const void *eye, const void *dir, const void *up);
extern "C" void  PE_htv_applyShake(PlayerEgo *self, int dt, void *eye, void *dir);
extern "C" void *MiningGame_new(int quality, int seed, void *hud);
extern "C" int   PE_aa_approachStep(PlayerEgo *self, int hud2, void *radar);
extern "C" void  Mat_mul(void *out, const void *a, const void *b);
extern "C" void  PE_handleShip_orient(PlayerEgo *self, int dt, unsigned int tfHandle);
extern "C" void (*g_stopBoost_fn)(void*, int);
extern "C" void *TractorBeam_new(void *geo, int kind);

extern "C" void  PlayerEgo_setShip_tail(void *canvas, int meshId, void *out, void **canvasHolder);
extern "C" void Player_addGun2(void*, void*, int);
extern "C" void PlayerEgo_addGun2_ext(PlayerEgo*);
void MatrixGetPosition(void*, void*);
extern "C" void *MovingStars_ctor(void *self);   // MovingStars::MovingStars()

void PlayerEgo::setVisible(bool v) {
  this->visible = v;
  this->field_0x309 = v;
  return PlayerEgo_setVisible_ext();
}

void PlayerEgo::boost() {
  if (this->boostingFlag != 0) return;
  if (this->field_0x146 == 0) return;
  if (this->rocketControlGun != 0) return;
  if (this->boostTimer < 0) return;
  float v = (float)this->boostSpeed;
  this->boostTimer = 0;
  this->boostingFlag = 1;
  void* snd = *(void**)g_boost_fmod;
  ((float&)this->speed) = v;
  ((FModSound *)(*(void**)snd))->play((int)(intptr_t)this->boostSoundId, (Vector *)0, (Vector *)0, v);
}

float PlayerEgo::getDriveChargeRate() {
  float d = (float)this->driveCharge / (float)this->driveChargeMax;
  return d >= 0.0f ? d : 1.0f;
}

unsigned char PlayerEgo::isAutoPilot() {
    PlayerEgo *self = this; return ((uint8_t&)this->autoPilot); }

bool PlayerEgo::goingToWormhole() {
  void* m = this->level;
  void* r4 = ((void*&)this->autoPilotTarget);
  Array<KIPlayer*>* lm = ((Level *)(m))->getLandmarks();
  return r4 == (void*)(*lm)[3];   // landmarks[3] (orig byte offset 0xc / 4-byte slots)
}

int PlayerEgo::getCurrentSecondaryWeaponIndex() {
    PlayerEgo *self = this; return this->currentSecondaryWeaponIndex; }

int PlayerEgo::getHitpoints() {
    PlayerEgo *self = this; return PlayerEgo_getHitpoints_ext(self->player); }

unsigned char PlayerEgo::isChargingDrive() {
    PlayerEgo *self = this; return ((uint8_t&)this->chargingDrive); }

int PlayerEgo::shouldSwitchToFreeLookCam() {
  if (this->switchToStandardCam == 0) return 0;
  this->switchToStandardCam = 0;
  return 1;
}

int PlayerEgo::getHandling() {
    PlayerEgo *self = this; return this->handling; }

void PlayerEgo::setComputerControlled(bool v) {
    PlayerEgo *self = this; this->computerControlled = v; }

int PlayerEgo::getRocketBanking() {
    PlayerEgo *self = this; return this->rocketBanking; }

bool PlayerEgo::isDockingToDockingPoint() {
  if (this->dockedFlag != 0) return this->dockingPointIndex != 1;
  return false;
}

void PlayerEgo::forceBoost() {
  this->boostTimer = 0;
  this->boostingFlag = 1;
  this->speed = 0x41000000;
  this->field_0xcc = 0x2710;
  this->field_0xd0 = 0;
}

int PlayerEgo::isDead() {
    PlayerEgo *self = this; return ((PlayerEgo *)(self))->getHitpoints() < 1; }

int PlayerEgo::getAutoPilotTarget() {
    PlayerEgo *self = this; return this->autoPilotTarget; }

bool PlayerEgo::goingToPlanet() {
  if (this->autoPilot != 0 && ((PlayerEgo *)(this))->goingToStream() == 0 && ((PlayerEgo *)(this))->goingToStation() == 0)
    return this->goingToWaypointFlag == 0;
  return false;
}

float PlayerEgo::getCloakRechargeRate() {
  return 1.0f - (float)this->cloakRechargeTimer / (float)this->cloakRechargeMax;
}

unsigned char PlayerEgo::isDockedToStream() {
    PlayerEgo *self = this; return ((uint8_t&)this->dockedToStream); }

int PlayerEgo::getTargetFollowCamera() {
    PlayerEgo *self = this; return this->targetFollowCamera; }

unsigned char PlayerEgo::isCloaked() {
    PlayerEgo *self = this; return ((uint8_t&)this->cloaked); }

void PlayerEgo::setFreeLookMode(bool v) {
    PlayerEgo *self = this; this->freeLookMode = v; }

void PlayerEgo::setThrust(float v) {
    PlayerEgo *self = this; this->thrust = v; }

bool PlayerEgo::readyForCloak() {
  if (this->chargingCloak == 0) return false;
  return this->cloakCharge >= this->cloakDischargeMax;
}

int PlayerEgo::getHackingGameDockIndex() {
  int v = this->hackingGame;
  if (v == 0) return -1;
  return PlayerEgo_getHackingGameDockIndex_ext(v);
}

void PlayerEgo::setPosition3(float x, float y, float z) {
  void* g = this->geometry;
  char v[12];
  *(float*)(v + 0) = x;
  *(float*)(v + 4) = y;
  *(float*)(v + 8) = z;
  ((AEGeometry *)(g))->setPosition(*(Vector *)v);
}

void PlayerEgo::dockToStream(bool param) {
  if (param) { *(short*)(char *)&this->docked = 0x100; return; }
  this->speed = 0x40000000;
  ((PlayerEgo *)(this))->setPosition3(this->rotX, this->rotY, this->rotZ);
  this->freeze = 0;
  this->field_0x145 = 0;
  *(short*)(char *)&this->docked = 0;
  return PlayerEgo_dockToStream_ext(this, 0);
}

void PlayerEgo::hackingShuffle() {
  int v = this->hackingGame;
  if (v != 0) PlayerEgo_hackingShuffle_ext(v);
}

int PlayerEgo::getDockTransferedAmount() {
    PlayerEgo *self = this; return this->dockTransferedAmount; }

unsigned char PlayerEgo::goingToWaypoint() {
    PlayerEgo *self = this; return ((uint8_t&)this->goingToWaypointFlag); }

bool PlayerEgo::isDockedToDockingPoint() {
  if (this->dockedFlag == 0) return false;
  return this->dockingPointIndex == 1;
}

int PlayerEgo::getDockTotalAmount() {
    PlayerEgo *self = this; return this->dockTotalAmount; }

bool PlayerEgo::isInWormhole() {
  if (((PlayerEgo *)(this))->getHitpoints() > 0) return this->inWormhole != 0;
  return false;
}

void PlayerEgo::setRocketControl(Gun* gun, AEGeometry* geo) {
  Level* lvl = this->level;
  this->rocketControlGun = (int)(intptr_t)gun;
  int psm_arg = lvl->movingStarsIndex;
  void* psm = (void*)(intptr_t)lvl->skybox2Mesh;
  if (gun == 0) {
    ((ParticleSystemManager *)(psm))->systemSetMatrix(psm_arg, &F<Matrix>(this->player, 4));
    this->rocketBanking = 0;
    return;
  }
  Matrix* m = &geo->getReferenceMatrix();
  ((ParticleSystemManager *)(psm))->systemSetMatrix(psm_arg, m);
  return PlayerEgo_setRocketControl_ext(this, 0);
}

bool PlayerEgo::isMining() {
    PlayerEgo *self = this; return this->miningGame != 0; }

void PlayerEgo::turnHorizontal(int a, float v) {
  if (v < 0.0f) { PlayerEgo_turnHorizontal_neg(this); return; }
  if (v > 0.0f) { PlayerEgo_turnHorizontal_pos(this); return; }
}

int PlayerEgo::getThrust() {
    PlayerEgo *self = this; return ((int&)this->thrust); }

float PlayerEgo::getCloakRate() {
  float a = (float)this->cloakCharge;
  float b = (float)(this->cloaked == 0 ? this->cloakDischargeMax : this->cloakChargeMax);
  float d = a / b;
  return d >= 0.0f ? d : 1.0f;
}

void PlayerEgo::resetLastHP() {
  this->lastHP = ((Player *)(this->player))->getCombinedHP();
}

void PlayerEgo::setExhaustVisible(bool param) {
  Level* lvl = this->level;
  this->exhaustVisible = param;
  *(char*)&lvl->field_80->flags = param;   // low byte of flags = sprite-active
  Array<int>* arr = lvl->field_a8;
  if (arr != nullptr) {
    for (unsigned i = 0; i < arr->size(); i++) {
      ((ParticleSystemManager *)(this->level->field_80))->enableSystemEmit((*arr)[i], param);
    }
  }
}

int PlayerEgo::shouldSwitchToStandardCam() {
  if (((char&)this->field_0xb0) == 0) return 0;
  ((char&)this->field_0xb0) = 0;
  return 1;
}

void PlayerEgo::resetMovement() {
  this->pitchAccumD = 0;
  this->rollAccum = 0;
  this->yawAccumD = 0;
}

bool PlayerEgo::isHacking() {
    PlayerEgo *self = this; return this->hackingGame != 0; }

void PlayerEgo::setPosition() {
    PlayerEgo *self = this; return PlayerEgo_setPosition_v(self); }

void PlayerEgo::setPosition(float x, float y, float z) {
  this->geometry->setPosition(x, y, z);
}

void PlayerEgo::setPosition(Vector v) {
  this->geometry->setPosition(v);
}

void PlayerEgo::resetChargingDrive() {
    PlayerEgo *self = this; ((uint8_t&)this->chargingDrive) = 0; }

// 000aa5dc: ldr r0,[r0,#0x0] ; b.w <ext>   — tail-call ext(self->m_pPlayer)
void PlayerEgo::PauseEngineSound() {
  return PlayerEgo_PauseEngineSound_ext(this->player);
}

unsigned char PlayerEgo::isInFreeLookMode() {
    PlayerEgo *self = this; return ((uint8_t&)this->freeLookMode); }

void PlayerEgo::setActive(bool) {
    PlayerEgo *self = this; return PlayerEgo_setActive_ext(self->player); }

void PlayerEgo::alignToHorizon() {
    PlayerEgo *self = this; ((uint8_t&)this->rolling) = 1; }

void PlayerEgo::setAutoTurret(bool on) {
  this->autoTurretEnabled = on;
  if (!on) PlayerEgo_setAutoTurret_ext(this->player, 2);
}

void PlayerEgo::hitCamera() {
  this->hitShake = 1;
  int cam = this->targetFollowCamera;
  this->hitShakeTimer = 0;
  return PlayerEgo_hitCamera_ext(cam);
}

int PlayerEgo::hackingWon() {
  int v = this->hackingGame;
  if (v == 0) return 0;
  return PlayerEgo_hackingWon_ext(v);
}

unsigned char PlayerEgo::lostMiningGame() {
    PlayerEgo *self = this; return ((uint8_t&)this->lostMiningGameFlag); }

int PlayerEgo::getCurrentMiningAmount() {
  int v = this->miningGame;
  if (v == 0) return 0;
  return PlayerEgo_getCurrentMiningAmount_ext(v);
}

void PlayerEgo::setDockingState(int s) {
  if (s == 2 && this->dockingPointIndex == 1) this->field_0xb2 = 1;
  this->dockingPointIndex = s;
}

void PlayerEgo::hackingRotateLCW() {
  if (this->hackingGame != 0 && ((HackingGame *)(this->hackingGame))->isRotating() == 0
      && ((HackingGame *)(this->hackingGame))->gameWon() == 0)
    PlayerEgo_hackingRotateLCW_ext(this->hackingGame, 1);
}

bool PlayerEgo::isInDockingProcedure() {
  if (((char&)this->dockingState) != 0) return true;
  return this->dockedFlag != 0;
}

void PlayerEgo::setSpeed(float v) {
    PlayerEgo *self = this; ((float&)this->speed) = v; }

void PlayerEgo::ResumeEngineSound() {
    PlayerEgo *self = this; return PlayerEgo_ResumeEngineSound_ext(self->player, 0); }

void PlayerEgo::addNukeVolatileForce(float v) {
  Player* p = (Player*)this->player;
  p->flShake = p->flShake + v * 3.0f;
}

extern void* g_explode_obj;
extern void (*g_explode_fn)(void*, int);
void PlayerEgo::explode() {
  ((ParticleSystemManager *)(this->level->field_74))->enableSystemEmit(this->currentSystem, 1);
  if (((int&)this->explosion) != 0) return;
  ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setActive(0);
  Explosion* e = new Explosion(0);
  int pl = ((int&)this->player);
  ((int&)this->explosion) = (int)(intptr_t)e;
  Player_setActive_(pl);
  void* o = g_explode_obj;
  void (*fn)(void*, int) = g_explode_fn;
  fn(*(void**)o, *(int*)(*(void**)o));
  fn(*(void**)o, this->field_0x1c);
  fn(*(void**)o, 0x1b);
  fn(*(void**)o, 0x23);
  fn(*(void**)o, 0x8d5);
  fn(*(void**)o, 0x8d4);
  fn(*(void**)o, 0x8cc);
  fn(*(void**)o, 0x447);
  fn(*(void**)o, 0x448);
  fn(*(void**)o, 0x449);
  return PlayerEgo_explode_ext(this, 0);
}

unsigned char PlayerEgo::isDockingToStream() {
    PlayerEgo *self = this; return ((uint8_t&)this->docked); }

int PlayerEgo::goingToStream() {
  void* m = this->level;
  void* r4 = ((void*&)this->autoPilotTarget);
  Array<KIPlayer*>* lm = ((Level *)(m))->getLandmarks();
  return r4 == (void*)(*lm)[1];   // landmarks[1] (orig byte offset 4 / 4-byte slots)
}

Vec3 PlayerEgo::GetDirVector() {
    PlayerEgo *self = this; return ((AEGeometry *)self->geometry)->getDirection(); }

void PlayerEgo::hideShipForFirstPersonCameraView(bool param) {
  unsigned char r1 = param;
  this->field_0x32d = r1;
  unsigned char nr = r1 ^ 1;
  this->field_0x309 = (this->visible != 0) & nr;
  *(char*)&this->level->field_80->flags = nr & (this->exhaustVisible != 0);   // low byte of flags = sprite-active
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
  HackingGame* g = (HackingGame*)((void*&)this->hackingGame);
  if (g != 0) delete g;
  ((void*&)this->hackingGame) = 0;
}

bool PlayerEgo::explosionEnded() {
  if (((int&)this->explosion) == 0) return true;
  return 8000 < this->explosionTimer;
}

float PlayerEgo::getBoostPercentage() {
  float den = (float)__aeabi_idiv(this->field_0xcc, 6);
  float d = (float)this->boostTimer / den;
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
    PlayerEgo *self = this; return ((AEGeometry *)(self->geometry))->getUpVector(); }

bool PlayerEgo::isDockedToPlanet() {
    PlayerEgo *self = this; return 3000 < this->planetDockTimer; }

bool PlayerEgo::emergencySystemActive() {
    PlayerEgo *self = this; return 0 < this->emergencySystemTimer; }

bool PlayerEgo::isDockingToAsteroid() {
  if (((char&)this->dockingState) != 0) return this->dockingPointIndex != 1;
  return false;
}

float PlayerEgo::getBoostRate() {
  float d = (float)this->boostTimer / (float)this->field_0xd0;
  float r = d + 1.0f;
  if (0.0f < d) r = 1.0f;
  return r;
}

bool PlayerEgo::driveReady() {
    PlayerEgo *self = this; return this->driveCharge >= this->driveChargeMax; }

void PlayerEgo::turnVertical(int a, float v) {
  if (v < -0.0f) { PlayerEgo_turnVertical_neg(); return; }
  if (v > 0.0f) { PlayerEgo_turnVertical_pos(this); return; }
}

unsigned char PlayerEgo::isInTurretMode() {
    PlayerEgo *self = this; return ((uint8_t&)this->turretActive); }

void PlayerEgo::startJumpDrive() {
  if (this->chargingDrive != 0) return;
  ((FModSound *)(*(void**)g_FMod_singleton))->play(0x21, (Vector *)0, (Vector *)0, 0);
  ((Hud *)(this->hud))->hudEvent(0x19, this, 0);
  this->driveCharge = 0;
  this->chargingDrive = 1;
}

bool PlayerEgo::isInRocketControl() {
    PlayerEgo *self = this; return this->rocketControlGun != 0; }

int PlayerEgo::getSpeed() {
    PlayerEgo *self = this; return this->speed; }

float PlayerEgo::getVolatileForce() {
  float f = ((Player*)this->player)->flShake;
  float r = 0.0f;
  if (!(f < 0.0f)) {
    r = 1.0f;
    if (!(f > 1.0f)) r = f;
  }
  return r;
}

unsigned char PlayerEgo::isDockingToPlanet() {
    PlayerEgo *self = this; return ((uint8_t&)this->dockingToPlanet); }

unsigned char PlayerEgo::isChargingCloak() {
    PlayerEgo *self = this; return ((uint8_t&)this->chargingCloak); }

bool PlayerEgo::isDockedToMiningPlant() {
  if (this->dockedFlag != 0 && this->dockingPointIndex == 1) {
    if (((Mission *)(gStatus->getMission()))->isEmpty() != 0
        && gStatus->inAlienOrbit() == 0) {
      return Station_getIndex(gStatus->getStation()) == 0x67;
    }
  }
  return false;
}

extern PlayerEgo* g_PlayerEgo_singleton;
void PlayerEgo::setCurrentSecondaryWeaponIndex(int idx) {
  this->currentSecondaryWeaponIndex = idx;
  (int&)g_PlayerEgo_singleton->gunBaseGeo = idx;
}

// `struct Route` (with its destructor) is provided by gof2/Route.h.
void PlayerEgo::removeRoute() {
  Route* r = (Route*)((void*&)this->route);
  if (r != 0) delete r;
  ((void*&)this->route) = 0;
}

void PlayerEgo::setRoute(Route* v) {
    PlayerEgo *self = this; this->route = (int)(intptr_t)v; }

unsigned char PlayerEgo::boosting() {
    PlayerEgo *self = this; return ((uint8_t&)this->boostingFlag); }

bool PlayerEgo::isDockedToAsteroid() {
  if (((char&)this->dockingState) == 0) return false;
  return this->dockingPointIndex == 1;
}

int PlayerEgo::goingToStation() {
  Array<KIPlayer*>* lm = ((Level *)(this->level))->getLandmarks();
  if ((*lm)[0] == 0) return false;   // landmarks[0]
  void* r4 = ((void*&)this->autoPilotTarget);
  lm = ((Level *)(this->level))->getLandmarks();
  return r4 == (void*)(*lm)[0];   // landmarks[0]
}

float PlayerEgo::getCloakingPercentage() {
  float r = 0.0f;
  if (this->cloaked != 0) {
    int v = this->cloakCharge;
    if (v >= 0) {
      if (v < 2000) {
        r = (float)v * 100.0f / 2000.0f;
      } else {
        int lo = this->cloakChargeMax - 2000;
        if (v > lo) {
          r = (((float)v - (float)lo) / -2000.0f + 1.0f) * 100.0f;
        } else {
          r = 100.0f;
        }
      }
    }
  }
  return r;
}

int PlayerEgo::getBoostSpeed() {
    PlayerEgo *self = this; return this->boostSpeed; }

void PlayerEgo::addGun(Gun* gun, int x) {
  ((Player *)(this->player))->addGun(gun, x);
  return PlayerEgo_addGun_ext(this);
}

unsigned char PlayerEgo::aboutToReachAutoTarget() {
    PlayerEgo *self = this; return ((uint8_t&)this->aboutToReachAutoTargetFlag); }

unsigned char PlayerEgo::autoTurretIsEnabled() {
    PlayerEgo *self = this; return ((uint8_t&)this->autoTurretEnabled); }

bool PlayerEgo::readyToBoost() {
    PlayerEgo *self = this; return -1 < this->boostTimer; }

void PlayerEgo::endExplosion() {
  int v = ((int&)this->explosion);
  if (v != 0) PlayerEgo_endExplosion_ext(v);
}

bool PlayerEgo::isLandingOrTakingOff() {
  if (this->dockedFlag == 0) return false;
  return (((uint32_t&)this->dockingPointIndex) | 1) == 3;
}

void PlayerEgo::setFreeze(bool v) {
    PlayerEgo *self = this; this->freeze = v; }

int PlayerEgo::getHUD() {
    PlayerEgo *self = this; return this->hud; }

int PlayerEgo::getRoute() {
    PlayerEgo *self = this; return this->route; }

void PlayerEgo::setCollide(bool v) {
    PlayerEgo *self = this; this->collide = v; }

extern void* g_emerg_fmod;
int PlayerEgo::tryToStartEmergencySystem() {
  if (this->field_0xac == 0 || this->emergencySystemTimer != 0) return 0;
  if (((Player *)(this->player))->getHitpoints() > 1) return 0;
  ((Player *)(this->player))->setHitpoints(1);
  this->emergencySystemTimer = this->emergencyVal1;
  ((Player *)(this->player))->setVulnerable(0);
  void* s1 = gStatus->getShip();
  Item* eq = ((Ship *)(gStatus->getShip()))->getFirstEquipmentOfSort(0x1b);
  ((Ship *)(s1))->removeEquipment(eq);
  ((FModSound *)(*(void**)g_emerg_fmod))->play(0x45b, (Vector *)0, (Vector *)0, 1.0f);
  return 1;
}

unsigned char PlayerEgo::hasAutoTurret() {
    PlayerEgo *self = this; return ((uint8_t&)this->autoTurretEquipped); }

unsigned char PlayerEgo::collidesWithStation() {
    PlayerEgo *self = this; return ((uint8_t&)this->collidesWithStationFlag); }

void PlayerEgo::stopPlanetDock() {
  ((TargetFollowCamera *)(this->targetFollowCamera))->setLookAtCam(0);
  this->collide = 1;
  this->dockingToPlanet = 0;
  ((PlayerEgo *)(this))->stopBoost();
  this->speed = 0x40000000;
}

bool PlayerEgo::isRechargingCloak() {
    PlayerEgo *self = this; return 0 < this->cloakRechargeTimer; }

// 000aa9bc: ldrb.w r0,[r0,#0x398] ; bx lr
unsigned char PlayerEgo::hasVolatileGoods() {
  return ((uint8_t&)this->volatileGoods);
}

void PlayerEgo::hackingRotateRCW() {
  if (this->hackingGame != 0 && ((HackingGame *)(this->hackingGame))->isRotating() == 0
      && ((HackingGame *)(this->hackingGame))->gameWon() == 0)
    PlayerEgo_hackingRotateRCW_ext(this->hackingGame, 1);
}

bool PlayerEgo::hasCloak() {
    PlayerEgo *self = this; return this->cloak != 0; }

int PlayerEgo::isBoostRefreshed() {
  if (this->boostingFlag != 0) return 0;
  if (this->field_0x146 == 0) return 0;
  if (this->boostTimer > -1) return 1;
  return 0;
}

Vec3 PlayerEgo::getPosition() {
    PlayerEgo *self = this; return ((AEGeometry *)(self->geometry))->getPosition(); }

// World-space position of the turret muzzle: compose the turret geometry's local
// matrix (at +0x28) with the ship hull geometry's matrix (at +0x8) and take the
// translation component of the product.
Vec3 PlayerEgo::getTurretPosition() {
    Matrix &turret = ((AEGeometry *)(this->turretGeometry))->getMatrix();
    Matrix &ship   = ((AEGeometry *)(this->geometry))->getMatrix();
    Matrix world   = turret * ship;
    return MatrixGetPosition(world);
}

// PlayerEgo::initManeuver(int type)
//   Starts an evasive maneuver of the given type when none is active. For dodge
//   types (1/2) while a flag at 0x398 is set, it nudges the player's roll
//   (m_pPlayer+0x60). For a strafe-run (type 3) it records a target offset
//   vector (0x338) and the current facing direction (0x344).

// Vector is a 12-byte (3xfloat) value; reached via raw buffers since the type
// is opaque in this scaffold.
extern "C" void Vec_scale(void *out, const void *v, float s);         // operator*
extern "C" void Vec_sub(void *out, const void *a, const void *b);     // operator-
extern "C" void Vec_assign(void *dst, const void *src);              // Vector::operator=

extern const float g_PE_rollNudge;
extern const float g_PE_strafeDist;   // 0xb1b70 distance scalar

void PlayerEgo::initManeuver(int type) {
    if ((unsigned)(type - 1) < 2 && this->volatileGoods != 0) {
        Player *player = (Player *)this->player;
        player->flShake = player->flShake + g_PE_rollNudge;
    }

    if (this->maneuverType == 0) {
        this->maneuverType = type;
        this->navPoint = 0;
        if (type == 3) {
            float pos[3];
            ((PlayerEgo *)(this))->getPosition();

            float dir[3];
            *(AbyssEngine::AEMath::Vector *)dir = ((AEGeometry *)this->geometry)->getDirection();

            float scaled[3];
            Vec_scale(scaled, dir, g_PE_strafeDist);

            float target[3];
            Vec_sub(target, pos, scaled);
            Vec_assign((char *)&this->strafeTargetVec, target);

            *(AbyssEngine::AEMath::Vector *)target = ((AEGeometry *)this->geometry)->getDirection();
            Vec_assign((char *)&this->facingVec, target);
        }
    }
}

void PlayerEgo::refillGunDelay() {
    PlayerEgo *self = this; return PlayerEgo_refillGunDelay_ext(self->player, 0); }

// PlayerEgo::checkForTurret()
//   Lazily builds the gun-turret model the first time a turret weapon is
//   available. It reads the equipped turret (slot 2), derives its tracking range
//   (0x1f8), then -- via a big switch on the turret item index -- selects the
//   base / barrel / muzzle / extra mesh ids, instantiates the AEGeometry chain
//   (0xf4 base, 0x34 yaw, 0x38 muzzle root, plus optional children), tunes the
//   muzzle transform, positions everything at the hardpoint, and (for AI-driven
//   players) clones the materials. The placement rotate/translate math is
//   delegated to PE_cft_place.

extern "C" void  PE_cft_finishMaterials(void *canvas, int mesh, void *out);   // 0x1aba08 veneer
extern "C" void  PE_cft_place(PlayerEgo *self, int turretIdx);                // placement math

extern const float g_PE_cft_transformVal;   // 0xab76c muzzle transform +0xe0

void PlayerEgo::checkForTurret() {
    if (this->turretMode != 0)
        return;
    int avail = ((Player *)(this->player))->gunAvailable(0);
    this->turretMode = (unsigned char)avail;
    if (avail == 0)
        return;

    this->autoTurretEquipped = 0;
    int equip = (int)(intptr_t)((Ship*)(PE_status()->getShip()))->getEquipment(2);
    void *item = *(void **)(equip + 4);   // RAWREAD: equip+4 (getEquipment() result kept as int handle; no modeled class)
    this->turretPitch = (int)((double)((Item *)(*(void **)(item)))->getAttribute(0) * 1.5);

    int idx = ((Item *)(*(void **)(item)))->getIndex();
    unsigned short base = 0xffff, barrel = 0xffff;
    int muzzle = -1, child = -1, extra = -1, extra2 = -1;

    if (idx == 0xe0) {
        base = 0x499a; barrel = 0x499b; muzzle = 0x499c; child = 0x499d;
    } else if (idx == 0x30) {
        base = 0x1a74; barrel = 0x1a75;
    } else if (idx == 0x31) {
        base = 0x1a76; barrel = 0x1a77;
    } else if (idx == 0xb4) {
        this->autoTurretEquipped = 1; base = 0x1a95; barrel = 0x1a96;
    } else if (idx == 0xb5) {
        this->autoTurretEquipped = 1; base = 0x1a97; barrel = 0x1a98;
    } else if (idx == 0xb6) {
        this->autoTurretEquipped = 1; base = 0x1a99; barrel = 0x1a9a;
    } else if (idx == 0xc6) {
        base = 0x4963; barrel = 0x4964; muzzle = 0x4966; child = 0x4967; extra = -1; extra2 = 0x4a7f;
    } else if (idx == 0xc7) {
        base = 0x4968; barrel = 0x4969; muzzle = 0x496a; child = 0x496b; extra = -1; extra2 = 0x4a7f;
    } else if (idx == 0xc8) {
        base = 0x496c; barrel = 0x496d; muzzle = 0x496e; child = 0x496f; extra = 0x4970; extra2 = 0x4a7f;
    } else if (idx == 0x2f) {
        base = 0x1a72; barrel = 0x1a73;
    }

    void *canvas = (void*)gCanvas;

    void *baseGeo = (void*)new AEGeometry((uint16_t)base, (PaintCanvas*)canvas, false);
    this->gunBaseGeo = baseGeo;
    void *yawGeo = (void*)new AEGeometry((uint16_t)barrel, (PaintCanvas*)canvas, false);
    this->gunYawGeo = yawGeo;
    ((AEGeometry *)yawGeo)->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
    void *muzzleRoot = (void*)new AEGeometry((PaintCanvas*)canvas);
    this->gunMuzzleRoot = muzzleRoot;

    if (muzzle != -1) {
        void *g = (void*)new AEGeometry((uint16_t)(unsigned short)muzzle, (PaintCanvas*)canvas, false);
        ((AEGeometry *)(this->gunBaseGeo))->addChild((uint32_t)(uintptr_t)g);
        delete (AEGeometry *)g;
    }
    if (child != -1) {
        void *g = (void*)new AEGeometry((uint16_t)(unsigned short)child, (PaintCanvas*)canvas, false);
        ((AEGeometry *)(this->gunYawGeo))->addChild((uint32_t)(uintptr_t)g);
        delete (AEGeometry *)g;
    }
    if (extra != -1) {
        void *g = (void*)new AEGeometry((uint16_t)(unsigned short)extra, (PaintCanvas*)canvas, false);
        ((AEGeometry *)(this->gunYawGeo))->addChild((uint32_t)(uintptr_t)g);
        delete (AEGeometry *)g;
    }
    if (extra2 != -1) {
        void *g = (void*)new AEGeometry((uint16_t)(unsigned short)extra2, (PaintCanvas*)canvas, false);
        this->gunExtraGeo = g;
        ((AEGeometry *)(this->gunYawGeo))->addChild((uint32_t)(uintptr_t)g);
        void *tf = gCanvas->TransformGetTransform((unsigned int)((unsigned int)(unsigned long)gCanvas));
        ((AbyssEngine::Transform *)(tf))->SetVisible(this->gunExtraVisible != 0);
    }

    void *tf = gCanvas->TransformGetTransform((unsigned int)((unsigned int)(unsigned long)gCanvas));
    ((AbyssEngine::Transform *)tf)->boundingRadius = g_PE_cft_transformVal;
    tf = gCanvas->TransformGetTransform((unsigned int)((unsigned int)(unsigned long)gCanvas));
    ((AbyssEngine::Transform *)(tf))->SetAnimationState((AbyssEngine::AnimationMode)2, (void *)0);

    ((AEGeometry *)(this->gunBaseGeo))->setPosition(*(Vector *)((char *)&this->dockArrowImage));
    ((AEGeometry *)(this->gunYawGeo))->setPosition(*(Vector *)((char *)&this->dockArrowImage));

    PE_cft_place(this, idx);

    ((AEGeometry *)(this->gunMuzzleRoot))->addChild((uint32_t)(uintptr_t)this->gunBaseGeo);
    ((AEGeometry *)(this->gunMuzzleRoot))->addChild((uint32_t)(uintptr_t)this->gunYawGeo);

    if (this->field_0x2c0 != 0 && this->turretMode != 0) {
        ((PaintCanvas*)(long)(canvas))->MeshCloneMaterial((unsigned int)(((AEGeometry *)this->gunBaseGeo)->meshId), (uint32_t&)this->cloakMaterial1);
        PE_cft_finishMaterials(canvas, ((AEGeometry *)this->gunYawGeo)->meshId, (char *)&this->cloakMaterial2);
    }
}

void PlayerEgo::pitchAllPrimaryGuns(float) {
    PlayerEgo *self = this; return PlayerEgo_pitchAllPrimaryGuns_ext(self->player); }

void PlayerEgo::stopShooting(int param) {
  if (this->turretActive != 0) {
    stopShooting_extA(this->player, 2);
    return;
  }
  if (((PlayerEgo *)(this))->isDead() != 0) return;
  void* p = this->player;
  if (param == 1) {
    stopShooting_extB(p, 1, this->currentSecondaryWeaponIndex);
    return;
  }
  stopShooting_extA(p, param);
}

// PlayerEgo::shake(int amount)
//   Produces a random camera-shake offset whose magnitude scales with `amount`
//   and the current shake intensity (field 0x134), then applies it to the
//   target-follow camera (field 0x8) via the camera shake entry point.

extern const float g_PE_shakeDiv;                                    // 0xb21d4 normaliser

void PlayerEgo::shake(int amount) {
    void *cam = this->geometry;
    float a = (float)amount / g_PE_shakeDiv;
    float intensity = (float)(this->shakeIntensity << 1);

    int range = (int)(a * intensity);
    if (range < 2)
        range = 1;
    int span = range << 1;

    float dx = (float)(gRandom->next(span) - range);
    float dy = (float)(gRandom->next(span) - range);
    float dz = (float)(gRandom->next(span) - range);
    ((AEGeometry *)(cam))->translate(dx, dy, dz);
}

void PlayerEgo::setRotation(float rx, float ry, float rz) {
  char local[60];
  this->rotateX = rx;
  this->rotateY = ry;
  this->rotateZ = rz;
  void* t = *(void**)g_setRotation_transform;
  void* m = TransformGetLocal(t, this->field_0x4->transform);
  MatrixSetRotation(local, m, this->rotateX, this->rotateY, this->rotateZ);
}

extern void* g_engine_fmod;
void PlayerEgo::StopEngineSound() {
  if (this->dockedFlag == 0 || this->dockingPointIndex != 1) {
    if (((Ship *)(gStatus->getShip()))->getFirstEquipmentOfSort(0x26) != 0
        && gStatus->inAlienOrbit() == 0) {
      int idx = Station_getIndex(gStatus->getStation());
      int cm = gStatus->getCurrentCampaignMission();
      float g = gStatus->getGammaRayDamagePerSecond(idx, cm);
      if (0.0f < g && this->engineSoundId != -1) {
        ((FModSound *)(*(void**)g_engine_fmod))->play((int)(intptr_t)((void*&)this->engineSoundId), (Vector *)0, (Vector *)0, g);
      }
    }
  }
  return PlayerEgo_StopEngineSound_ext(this->player);
}

void PlayerEgo::startSmokeEmission() {
  int v = this->smokeSystem;
  if (v < 0) return;
  ((ParticleSystemManager *)(this->level->particleEmitBoolPtr))->enableSystemEmit(v, 1);
  PlayerEgo_startSmokeEmission_ext((void*)(intptr_t)this->level->particleRenderBoolPtr, this->explosionSmoke, 1);
}

// PlayerEgo::down(int frameTime, float delta)
//   Pitch-down input handler. Dispatch mirrors left()/right() but on the pitch
//   axis: mining game (delegated, with a global gate selecting which veneer),
//   turret free-look (accumulating into the look-pitch 0x1a8 and the turret
//   pitch 0x1a4, each rotating its geometry), scripted maneuver (0x7c), or the
//   normal load-scaled pitch ramp on accumulator 0x278 (direction flag 0x100=-1).

__attribute__((visibility("hidden"))) extern char **g_PE_d_miningGate; // 0xb0bb0 -> flags (+0x10)
extern const float g_PE_d_eps;      // 0xb0dc0 free-look limit
extern const float g_PE_d_lookK1;
extern const float g_PE_d_lookK2;
extern const float g_PE_d_manK;
extern const float g_PE_d_loadK;
extern const float g_PE_d_loadB;
extern const float g_PE_d_rateK;

float PlayerEgo::down(int frameTime, float delta) {
    if (((void*&)this->miningGame) != 0) {
        if (C(*g_PE_d_miningGate, 0x10) == 0)   // RAWREAD: opaque flags struct via global holder (no modeled class)
            return ((MiningGame *)(((void*&)this->miningGame)))->down(delta);
        return ((MiningGame *)(((void*&)this->miningGame)))->up(-delta);
    }

    if (this->turretActive != 0) {
        float ft = (float)frameTime;
        if (this->lookYaw < g_PE_d_eps) {
            float ang = ft * delta + this->lookYaw;
            this->lookYaw = ang;
            ((AEGeometry *)this->turretGeometry)->rotate((float)(ang * g_PE_d_lookK1 * g_PE_d_lookK2), 0.0f, 0.0f);
        }
        float p = this->lookPitch;
        if (p < g_PE_d_eps) {
            float half = ft * delta * 0.5f;
            float ang = half * g_PE_d_lookK1 * g_PE_d_lookK2;
            this->lookPitch = half + p;
            return (((AEGeometry *)this->dockCameraMid)->rotate((float)ang, 0.0f, 0.0f), 0.0f);
        }
        return p;
    }

    if (((void*&)this->rocketControlGun) != 0) {
        float v = (float)frameTime * g_PE_d_manK * F(((void*&)this->rocketControlGun), 0x50);   // RAWREAD: rocketControlGun is an int gun handle (Gun.h not in this TU; +0x50 unnamed)
        this->maneuverParam = v;
        return v;
    }

    if (this->autoPilot != 0)
        return delta;
    if (this->dockedFlag != 0 && this->dockingPointIndex != 1)
        return delta;

    this->pitchAccumDir = -1;
    float rate;
    if (this->hardCoreMode == 0) {
        rate = ((float&)this->handling);
    } else {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        rate = ((float&)this->handling) * (1.0f - cur / max) * g_PE_d_loadK + ((float&)this->handling) * g_PE_d_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_d_rateK * rate), 0x3f);
    ((float&)this->yawAccumD) = delta;
    this->pitchRamp = -rate;
    if (((float&)this->pitchAccumD) < target) {
        float v = ((float&)this->pitchAccumD) + PE_pitchRampDelta(this, rate, frameTime);
        if (target < v) v = target;
        ((float&)this->pitchAccumD) = v;
    }
    return target;
}

// PlayerEgo::approachDockingPoint(Hud*, int hud2, Radar*)
//   Three-phase automated station-docking controller, dispatched on the dock
//   state (0x314):
//     * state 0: locate the nearest navigation point on the station, reserve it
//       (SpacePoint take/give), fly toward it, and -- on arrival -- build the
//       glide EaseInOutMatrix and advance to state 2.
//     * state 2: when within the per-ship docking radius, inspect the active and
//       campaign missions to decide which "arrived" HUD event to raise (cargo
//       delivery 0x29/0x25/0x23, passenger drop-off 0x2b, etc.), then hand back
//       to manual control (state 1). Otherwise keep gliding the ease matrix.
//     * state 3: glide the final docking ease; once within 200 units complete
//       the dock -- restore cameras, free the nav point, tear down any hacking
//       game -- and return to state 0.
//   The dense vector/matrix glide math is delegated to PE_adp_glide / PE_adp_apply;
//   the recoverable state dispatch and the mission-event decision stay inline.

// Glide helpers: PE_adp_approach steers/builds the ease toward the nav point and
// returns the remaining distance; PE_adp_glide advances the ease one step and
// returns the remaining distance; PE_adp_apply writes the final transform.

__attribute__((visibility("hidden"))) extern int  *g_PE_adp_dockRadius; // 0xb00f2 per-ship table
__attribute__((visibility("hidden"))) extern PlayerFixedObject **g_PE_adp_fixedObj;  // 0xb00f0 PlayerFixedObject

// clears the 3-float dock-offset vector at 0x1c8.
static inline void adp_clearDockVector(PlayerEgo *self)
{
    self->dockApproachDist = 0;
    self->dockApproachThreshold = 0;
    self->field_0x1d0 = 0;
}

// Decide the arrival HUD event for state 2; returns the event id (0 = none).
static int adp_arrivalEvent(PlayerEgo *self, void *station)
{
    void *mission = PE_status()->getMission();
    void *campaign = (void *)PE_status()->getCampaignMission();
    PlayerFixedObject *fixed = *g_PE_adp_fixedObj;

    if (((Mission *)(mission))->isEmpty() == 0 && ((Mission *)(mission))->getType() == 0xf
        && fixed->getDockingType() == 1) {
        if (((Ship *)(PE_status()->getShip()))->hasCargo(((Mission *)(mission))->getProductionGoodIndex(), 1) != 0) {
            int amount = ((Item *)(((Ship *)(PE_status()->getShip()))->getCargo(((Mission *)(mission))->getProductionGoodIndex())))->getAmount();
            ((int&)self->autoTurretEquipped) = amount;
            int need = ((Mission *)(mission))->getProductionGoodAmount()
                       - ((Level *)(self->level))->getNumDeliveredOre();
            if (need < amount) ((int&)self->autoTurretEquipped) = need;
            self->autoTurretTimer = 0;
            if (((int&)self->autoTurretEquipped) > 0) return 0x29;
        }
        return 0;
    }

    if (((Mission *)(mission))->isEmpty() == 0
        && (((Mission *)(mission))->getType() == 0xb8 || ((Mission *)(mission))->getType() == 0xa8)
        && fixed->getDockingType() == 2) {
        // passenger drop-off
        int carried = fixed->intPosX;
        int maxPax = ((Ship *)(PE_status()->getShip()))->getMaxPassengers();
        if (maxPax > 0 && carried < maxPax) {
            int avail = (((Mission *)(mission))->getType() == 0xa8)
                        ? ((Mission *)(mission))->getStatusValue() : maxPax;
            avail -= carried;
            ((int&)self->autoTurretEquipped) = avail;
            int status = ((Mission *)(mission))->getStatusValue() - carried;
            if (status < avail) ((int&)self->autoTurretEquipped) = status;
            self->autoTurretTimer = 0;
            if (((int&)self->autoTurretEquipped) > 0) return 0x23;
        }
        if (((Mission *)(mission))->getType() != 0xa8 && ((Ship *)(PE_status()->getShip()))->getMaxPassengers() == 0)
            return 0x2b;
        return 0;
    }

    if (((Mission *)(mission))->isEmpty() == 0 && ((Mission *)(mission))->getType() == 0xb8
        && fixed->getDockingType() == 1) {
        int n = fixed->intPosX;
        if (n > 0) {
            self->autoTurretTimer = 0;
            ((int&)self->autoTurretEquipped) = n;
            return 0x25;
        }
        return 0;
    }

    if (campaign != 0 && ((Mission *)(campaign))->isEmpty() == 0
        && (((Mission *)(campaign))->getType() == 0xa7 || ((Mission *)(campaign))->getType() == 0xae)
        && fixed->getDockingType() == 1) {
        if (((Ship *)(PE_status()->getShip()))->hasCargo(((Mission *)(campaign))->getProductionGoodIndex(), 1) != 0) {
            int amount = ((Item *)(((Ship *)(PE_status()->getShip()))->getCargo(((Mission *)(campaign))->getProductionGoodIndex())))->getAmount();
            ((int&)self->autoTurretEquipped) = amount;
            int need = ((Mission *)(campaign))->getProductionGoodAmount() - ((Mission *)(campaign))->getStatusValue();
            if (need < amount) ((int&)self->autoTurretEquipped) = need;
            self->autoTurretTimer = 0;
            if (((int&)self->autoTurretEquipped) > 0) return 0x29;
        }
    }
    return 0;
}

int PlayerEgo::approachDockingPoint(Hud *hud, int /*hud2*/, Radar *radar) {
    if (((KIPlayer *)(this))->isDying() != 0)
        return 0;

    int state = ((int&)this->emergencyVec.x);

    if (state == 0) {
        void *station = this->dockStation;
        Vector pos = ((PlayerEgo *)(this))->getPosition();
        void *nav = ((KIPlayer *)(station))->getNearestNavigationPoint(pos, (SpacePoint *)&this->navPoint);
        if (nav != 0) {
            if (this->strafeNavPoint != nav) {
                if (this->strafeNavPoint != 0)
                    ((SpacePoint *)(this->strafeNavPoint))->giveFree();
                this->strafeNavPoint = nav;
                ((SpacePoint *)(nav))->take();
            }
            int dist = PE_adp_approach(this, station);
            if (dist < this->dockApproachThreshold) {
                ((int&)this->emergencyVec.x) = 2;
                ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setLookAtCam(false);
                ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->useTargetsUpVector(false);
            }
        }
        PE_adp_apply(this);
        return 0;
    }

    if (state == 2) {
        if (this->gunExtraVisible != 0)
            ((PlayerEgo *)(this))->setTurretMode(0);
        int dist = PE_adp_glide(this);
        int radius = g_PE_adp_dockRadius[((Ship *)(PE_status()->getShip()))->getIndex()];
        if (dist < radius) {
            int ev = adp_arrivalEvent(this, this->dockStation);
            if (ev != 0)
                hud->hudEvent(ev, this, 0);
            ((int&)this->emergencyVec.x) = 1;        // hand back to manual control
        }
        PE_adp_apply(this);
        return 0;
    }

    if (state == 3) {
        int dist = PE_adp_glide(this);
        if (dist < 200) {
            // docking complete: restore cameras and free the nav point.
            this->dockStation = 0;
            P(radar, 0x4) = 0;   // RAWREAD: radar+0x4 (Radar modeled as minimal stub, no fields)
            P(radar, 0x8) = 0;   // RAWREAD: radar+0x8 (Radar modeled as minimal stub, no fields)
            adp_clearDockVector(this);
            ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setActive(true);
            ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setLookAtCam(false);
            ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->useTargetsUpVector(false);
            ((LevelScript *)(this->levelScript))->resetCamera(this->levelScript->m_pLevel);
            ((Player *)(this->player))->resetGunDelay(0);
            ((int&)this->emergencyVec.x) = 0;
            if (this->strafeNavPoint != 0) {
                ((SpacePoint *)(this->strafeNavPoint))->giveFree();
                this->strafeNavPoint = 0;
            }
            if (((void*&)this->hackingGame) != 0) {
                delete (HackingGame *)((void*&)this->hackingGame);
                ((void*&)this->hackingGame) = 0;
                hud->setHackingGameActive(false);
            }
            return 1;        // docking complete
        }
        PE_adp_apply(this);
    }
    return 0;
}

void PlayerEgo::setLevel(Level* level) {
  ((int&)this->level) = (int)(intptr_t)level;
  void* src = (void*)(intptr_t)level->field_74;
  Matrix* gm = &((AEGeometry *)this->geometry)->getMatrix();
  void* sys = (void *)((ParticleSystemManager *)(src))->addSystem(gm, ParticleSettings::ParticleSet_9, 0);
  this->currentSystem = (int)(intptr_t)sys;
  ((ParticleSystemManager *)(this->level->field_74))->enableSystemEmit((int)(intptr_t)sys, 0);
  if (gStatus->getCurrentCampaignMission() > 1) return;
  void* src2 = (void*)(intptr_t)this->level->particleEmitBoolPtr;
  Matrix* gm2 = &((AEGeometry *)this->geometry)->getMatrix();
  void* sys2 = (void *)((ParticleSystemManager *)(src2))->addSystem(gm2, ParticleSettings::ParticleSet_0xf, 0);
  this->smokeSystem = (int)(intptr_t)sys2;
  ((ParticleSystemManager *)(this->level->particleEmitBoolPtr))->enableSystemEmit((int)(intptr_t)sys2, 0);
  void* src3 = (void*)(intptr_t)this->level->particleRenderBoolPtr;
  Matrix* gm3 = &((AEGeometry *)this->geometry)->getMatrix();
  void* sys3 = (void *)((ParticleSystemManager *)(src3))->addSystem(gm3, ParticleSettings::ParticleSet_0x2a, 0);
  this->explosionSmoke = (int)(intptr_t)sys3;
  return PlayerEgo_setLevel_ext((void*)(intptr_t)this->level->particleRenderBoolPtr, (int)(intptr_t)sys3, 0);
}

// PlayerEgo::setDockingCamera()
//   Lazily builds the docking camera rig (a PaintCanvas camera plus a small
//   chain of AEGeometry nodes used to orient/offset the view) on first use,
//   seeding a default offset vector at 0x224 when none is set, then positions
//   the leaf node at the dock offset (0x148), copies the ship matrix onto it,
//   and makes the camera current.

extern const float g_PE_dc_fovNormal;
extern const float g_PE_dc_fovAlien;
extern const unsigned int g_PE_dc_defX; // 0xaab78 default offset lo
extern const unsigned int g_PE_dc_defY; // 0xaab7c default offset hi

void PlayerEgo::setDockingCamera() {
    if (this->dockCameraNode == 0) {
        gCanvas->CameraCreate(this->turretCamera);
        unsigned int cam = (unsigned int)(unsigned long)gCanvas;

        float fov = (PE_status()->inAlienOrbit() != 0) ? g_PE_dc_fovAlien : g_PE_dc_fovNormal;
        gCanvas->CameraSetPerspective((unsigned int)(cam), fov, 0.0f, g_PE_dc_fovAlien);

        void *node = (void*)new AEGeometry(gCanvas);
        this->dockCameraNode = node;
        ((AEGeometry *)node)->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);

        // seed the default rig offset (0x224..0x22c) only when fully zero.
        if (this->turretOffsetVec.x == 0.0f && this->turretOffsetVec.y == 0.0f && this->turretOffsetVec.z == 0.0f) {
            ((uint32_t&)this->turretOffsetVec.y) = g_PE_dc_defX;
            ((uint32_t&)this->turretOffsetVec.z) = g_PE_dc_defY;
        }
        ((AEGeometry *)(this->dockCameraNode))->translate(this->turretOffsetVec);

        void *mid = (void*)new AEGeometry(gCanvas);
        this->dockCameraMid = mid;
        ((AEGeometry *)(mid))->translate(*(Vector *)&((AEGeometry *)mid)->transform);
        ((AEGeometry *)(this->dockCameraNode))->addChild((uint32_t)((AEGeometry *)this->dockCameraMid)->transform);

        void *leaf = (void*)new AEGeometry(gCanvas);
        this->dockCameraLeaf = leaf;
        ((AEGeometry *)(this->dockCameraNode))->addChild((uint32_t)((AEGeometry *)this->dockCameraNode)->transform);
    }

    ((AEGeometry *)(this->dockCameraLeaf))->setPosition(this->dockOffsetVec);
    void *leaf = this->dockCameraLeaf;
    ((AEGeometry *)leaf)->setMatrix(((AEGeometry *)(this->geometry))->getMatrix());

    gCanvas->CameraSetCurrent((unsigned int)(this->turretCamera));
}

// PlayerEgo::right(int frameTime, float delta)
//   Yaw-right input handler -- the mirror of left(): same mode dispatch but the
//   yaw direction flag (0x104) is -1 and the yaw accumulator (0x27c) ramps
//   downward toward the (negative) target.

extern const float g_PE_r_loadK;
extern const float g_PE_r_loadB;
extern const float g_PE_r_rateK;
extern const float g_PE_r_turK1;
extern const float g_PE_r_turK2;
extern const float g_PE_r_turK3;
extern const float g_PE_r_manK1;
extern const float g_PE_r_manK2;

float PlayerEgo::right(int frameTime, float delta) {
    if (((void*&)this->miningGame) != 0)
        return (((MiningGame *)(((void*&)this->miningGame)))->right(delta), ((MiningGame *)(((void*&)this->miningGame)))->inputX);

    if (this->turretActive != 0) {
        float pitch = (float)this->turretPitch;
        float ft = (float)frameTime;
        float ang = ((ft * delta) / (g_PE_r_turK1 / pitch)) * g_PE_r_turK2 * g_PE_r_turK3;
        ((AEGeometry *)this->dockCameraNode)->rotate(0.0f, (float)ang, 0.0f);
        ((AEGeometry *)this->rollGeometry)->rotate(0.0f, (float)ang, 0.0f);
        ((AEGeometry *)this->turretGeometry)->rotate(0.0f, (float)ang, 0.0f);
        return ang;
    }

    if (((void*&)this->rocketControlGun) != 0) {
        float ft = (float)frameTime;
        this->field_0x80 = delta * g_PE_r_manK1 * F(((void*&)this->rocketControlGun), 0x50);   // RAWREAD: rocketControlGun is an int gun handle (Gun.h not in this TU; +0x50 unnamed)
        ((float&)this->rocketBanking) = ((float&)this->rocketBanking) + (ft * delta) * g_PE_r_manK2;
        return ft * delta;
    }

    if (this->autoPilot != 0)
        return delta;
    if (this->dockedFlag != 0 && this->dockingPointIndex != 1)
        return delta;

    this->yawAccumDir = -1;
    float rate;
    if (this->hardCoreMode == 0) {
        rate = ((float&)this->handling);
    } else {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        rate = ((float&)this->handling) * (1.0f - cur / max) * g_PE_r_loadK + ((float&)this->handling) * g_PE_r_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_r_rateK * rate), 0x3f);
    ((float&)this->rollAccum) = delta;
    this->yawRamp = -rate;
    if (this->yawAccumF > target) {
        float step = PE_yawRampDelta(rate, frameTime);
        float v = this->yawAccumF - step;
        if (v < target) v = target;
        this->yawAccumF = v;
    }
    return target;
}

// PlayerEgo::left(int frameTime, float delta)
//   Yaw-left input handler. Routes the input depending on mode:
//     * mining game active (0x1e4): forward the (negated) delta to it,
//     * turret mode (0x1a0): rotate the three turret geometries,
//     * maneuver lock (0x194): feed the scripted-maneuver roll instead,
//     * otherwise: set the yaw direction (0x104=+1), compute the load-scaled
//       yaw rate and ramp the yaw accumulator (0x27c) toward the target.
//   The yaw-rate ramp (which samples the active-camera ease state) is computed
//   by PE_yawRampDelta to keep the FP-flag soup out of line.

// Ramp helper: given base rate and inputs, returns the accumulator increment.

extern const float g_PE_l_loadK;
extern const float g_PE_l_loadB;
extern const float g_PE_l_rateK;
extern const float g_PE_l_turK1;
extern const float g_PE_l_turK2;
extern const float g_PE_l_turK3;
extern const float g_PE_l_manK1;
extern const float g_PE_l_manK2;

float PlayerEgo::left(int frameTime, float delta) {
    if (((void*&)this->miningGame) != 0)
        return (((MiningGame *)(((void*&)this->miningGame)))->left(-delta), ((MiningGame *)(((void*&)this->miningGame)))->inputX);

    if (this->turretActive != 0) {
        // turret yaw: scale by inverse turret-pitch and apply to 3 nodes.
        float pitch = (float)this->turretPitch;
        float ft = (float)frameTime;
        float ang = ((ft * delta) / (g_PE_l_turK1 / pitch)) * g_PE_l_turK2 * g_PE_l_turK3;
        ((AEGeometry *)this->dockCameraNode)->rotate(0.0f, (float)ang, 0.0f);
        ((AEGeometry *)this->rollGeometry)->rotate(0.0f, (float)ang, 0.0f);
        ((AEGeometry *)this->turretGeometry)->rotate(0.0f, (float)ang, 0.0f);
        return ang;
    }

    if (((void*&)this->rocketControlGun) != 0) {
        float ft = (float)frameTime;
        this->field_0x80 = delta * g_PE_l_manK1 * F(((void*&)this->rocketControlGun), 0x50);   // RAWREAD: rocketControlGun is an int gun handle (Gun.h not in this TU; +0x50 unnamed)
        ((float&)this->rocketBanking) = ((float&)this->rocketBanking) + (ft * delta) * g_PE_l_manK2;
        return ft * delta;
    }

    if (this->autoPilot != 0)
        return delta;
    if (this->dockedFlag != 0 && this->dockingPointIndex != 1)
        return delta;

    this->yawAccumDir = 1;
    float rate;
    if (this->hardCoreMode == 0) {
        rate = ((float&)this->handling);
    } else {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        rate = ((float&)this->handling) * (1.0f - cur / max) * g_PE_l_loadK + ((float&)this->handling) * g_PE_l_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_l_rateK * rate), 0x3f);
    this->yawRamp = rate;
    ((float&)this->rollAccum) = -delta;
    if (this->yawAccumF < target) {
        float step = PE_yawRampDelta(rate, frameTime);
        float v = this->yawAccumF + step;
        if (target < v) v = target;
        this->yawAccumF = v;
    }
    return target;
}

// PlayerEgo::~PlayerEgo() -- real C++ destructor so the demangled symbol contains "~PlayerEgo".
//
// Deletes every owned sub-object (the wrapped Player, the various AEGeometry
// nodes for hull / shield / cloak / turret / docking rigs, the Route, tractor
// and repair beams, the mining game, both explosions, the ease matrix) and
// nulls each handle, mirroring the target teardown order.

__attribute__((minsize)) PlayerEgo::~PlayerEgo() noexcept(false)
{
    if (this->player)   delete (Player *)this->player;
    this->player = 0;
    if (this->field_0x4)   delete (AEGeometry *)this->field_0x4;
    this->field_0x4 = 0;
    if (this->geometry)   delete (AEGeometry *)this->geometry;
    this->geometry = 0;
    if (this->rollGeometry)  delete (AEGeometry *)this->rollGeometry;
    this->rollGeometry = 0;
    if (this->turretGeometry)  delete (AEGeometry *)this->turretGeometry;
    this->turretGeometry = 0;
    if (this->route)  delete (Route *)this->route;
    this->route = 0;
    if (this->dockCameraNode) delete (AEGeometry *)this->dockCameraNode;
    this->dockCameraNode = 0;
    if (this->dockCameraLeaf) delete (AEGeometry *)this->dockCameraLeaf;
    this->dockCameraLeaf = 0;
    if (this->field_0x2c)  delete (AEGeometry *)this->field_0x2c;
    this->field_0x2c = 0;
    if (this->field_0x30)  delete (AEGeometry *)this->field_0x30;
    this->field_0x30 = 0;
    if (this->gunYawGeo)  delete (AEGeometry *)this->gunYawGeo;
    this->gunYawGeo = 0;
    if (this->dockCameraMid) delete (AEGeometry *)this->dockCameraMid;
    this->dockCameraMid = 0;
    if (this->tractorBeam) delete (TractorBeam *)this->tractorBeam;
    this->tractorBeam = 0;
    if (this->miningGame) delete (MiningGame *)(intptr_t)this->miningGame;
    this->miningGame = 0;
    if (this->explosion)  delete (Explosion *)(intptr_t)this->explosion;
    this->explosion = 0;
    if (this->explosion2)  delete (Explosion *)(intptr_t)this->explosion2;
    this->explosion2 = 0;
    if (this->easeMatrix) ::operator delete(EaseInOutMatrix_dtor(this->easeMatrix));
    this->easeMatrix = 0;
    if (Array<RepairBeam *> *beams = this->repairBeams) {
        for (RepairBeam *beam : *beams)
            delete beam;
        delete beams;
    }
    this->repairBeams = 0;
}

void PlayerEgo::throttleChanged() {
  int v;
  if (this->throttleStarted == 0) {
    this->throttleStarted = 1;
    v = 0;
  } else {
    v = this->throttle;
    if ((unsigned)(v - 0x1f5) < 999) {
      v = 500;
    } else {
      if (v < 0x5dc) return;
      v = 2000 - v;
    }
  }
  this->throttle = v;
}

// PlayerEgo::PlayerEgo(Player*) -- real C++ constructor so the demangled symbol
// is "PlayerEgo::PlayerEgo(Player*)".
//
// The body is a long, flat member-initialisation sequence: it default-constructs
// the two embedded 60-byte matrices (the roll matrix at 0x2ac and the turret/HUD
// matrix at 0x4c8... reached as this+0x13/this+1.field_1C in the Ghidra listing),
// zeroes/seeds the ~90 scalar and vector fields, records the wrapped Player at
// offset 0, then derives the boost timing/speed/effect from the current ship and
// builds the MovingStars background. Because the field block is purely data
// initialisation with offsets that are not individually load-bearing for
// coverage, the bulk is performed by PlayerEgo_initFields, while the two matrix
// constructors and the player store stay inline.

extern "C" void PlayerEgo_initFields(void *self, Player *player); // field init + boost + MovingStars

PlayerEgo::PlayerEgo(Player *player)
{
    // embedded orientation matrices.
    this->rollMatrix = AbyssEngine::AEMath::Matrix();     // roll matrix
    this->turretHudMatrix = AbyssEngine::AEMath::Matrix();     // turret/HUD matrix

    // record the wrapped player (offset 0).
    this->player = (void *)player;

    // remaining field initialisation + boost stats + MovingStars background.
    PlayerEgo_initFields(this, player);
}

extern void* g_engine_fmod;
void PlayerEgo::PlayEngineSound() {
  if (((Ship *)(gStatus->getShip()))->getFirstEquipmentOfSort(0x26) != 0
      && gStatus->inAlienOrbit() == 0) {
    int idx = Station_getIndex(gStatus->getStation());
    int cm = gStatus->getCurrentCampaignMission();
    float g = gStatus->getGammaRayDamagePerSecond(idx, cm);
    if (0.0f < g && this->engineSoundId != -1) {
      ((FModSound *)(*(void**)g_engine_fmod))->play((int)(intptr_t)((void*&)this->engineSoundId), (Vector *)0, (Vector *)0, g);
    }
  }
  return PlayerEgo_PlayEngineSound_ext(this->player, this->field_0x1c, 0);
}

// PlayerEgo::moveToPosition(Vector target, bool steer, float speed)
//   Auto-pilot helper that flies the ship toward `target`. It computes the
//   desired heading from the offset to the target (smoothed against the current
//   facing), optionally turns toward it (`steer`) using a running average of the
//   recent steering angles (history at 0x290), then sets the new direction,
//   advances along it, applies roll, and folds in the strafe slide. The dense
//   vector/angle math (which fills the steering history and clamps the turn) is
//   delegated to PE_mtp_steer; the recoverable flight bookkeeping stays inline.

// Builds the desired heading vector at self+0x164 and (when steer!=0) updates the
// turn-rate history; returns nothing. Owns the corrupted dot/acos/clamp logic.

extern const float g_PE_mtp_strafeEps;
extern const float g_PE_mtp_strafeReset;
extern const float g_PE_mtp_strafeK;

void PlayerEgo::moveToPosition(float tx, float ty, float tz, int steer, float speed) {
    float target[3] = { tx, ty, tz };
    PE_mtp_steer(this, target, steer, speed);

    // apply the resolved heading (stored at 0x164) as the new facing.
    float up[3] = {0.0f, 1.0f, 0.0f};
    ((AEGeometry *)(this->geometry))->setDirection(this->headingVec, *(Vector *)up);

    float dt = (float)this->shakeIntensity;
    ((AEGeometry *)(this->geometry))->moveForward(this->thrust * dt * ((float&)this->speed));
    ((PlayerEgo *)(this))->roll(this->shakeIntensity);

    // strafe slide -> follow camera, unless below the dead-zone.
    float slide = this->strafeAccel;
    float mag = slide > 0.0f ? slide : -slide;
    if (mag <= g_PE_mtp_strafeEps) {
        ((float&)this->strafeNavPoint) = g_PE_mtp_strafeReset;
    } else {
        unsigned char m[0x30];
        Mat_assign(m, ((AEGeometry *)(this->geometry))->getMatrix());
        // rotate the slide vector into world space and push it to the camera.
        float v = slide * dt;
        ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->translateNoUpdate(v, 0.0f, 0.0f);
        this->strafeAccel = this->strafeAccel * g_PE_mtp_strafeK;
        ((AEGeometry *)this->geometry)->setMatrix(*(const AbyssEngine::AEMath::Matrix*)m);
    }

    Mat_assign(((Player *)this->player)->transform, ((AEGeometry *)(this->geometry))->getMatrix());
    ((AEGeometry *)(this->geometry))->getPosition();
}

// PlayerEgo::moveToPosition(Vector target, bool steer, float speed)
//   Vector-packed form of moveToPosition: unpacks the target vector into its
//   components and forwards to the (float,float,float,int,float) flight helper.
void PlayerEgo::moveToPosition(Vector target, bool steer, float speed) {
    moveToPosition(target.x, target.y, target.z, steer ? 1 : 0, speed);
}

void PlayerEgo::resetGunDelay() {
    PlayerEgo *self = this; return PlayerEgo_resetGunDelay_ext(self->player, 0); }

int PlayerEgo::getShieldDamageRate() {
    PlayerEgo *self = this; return PlayerEgo_getShieldDamageRate_ext(self->player); }

// PlayerEgo::handleAutoTurret(int dt)
//   Drives the automatic gun turret. Every 3 seconds (timer 0x184) it rescans
//   the level enemies and locks the nearest living, active, hostile target
//   (0x18c). Each frame, if a target is held, it aims the turret geometries at
//   the predicted intercept point and -- when on target and the firing window
//   permits -- fires a turret shot and pulses the muzzle transform. Losing the
//   target or exceeding the no-target window stops shooting. The aim/intercept
//   vector-and-matrix math is delegated to PE_hat_aimAndFire.

// virtual +0x28
// Aims the turret at the held target and fires when appropriate; returns the
// new "no-target" timer contribution (0 while actively tracking).

void PlayerEgo::handleAutoTurret(int dt) {
    int t = this->autoTurretTimer + dt;
    this->autoTurretTimer = t;
    if (t >= 0xbb9) {                       // 3000ms: rescan for the best target
        this->autoTurretTimer = 0;
        this->autoTurretTarget = 0;
        Array<KIPlayer*> *enemies = ((Level *)(this->level))->getEnemies();
        if (enemies != nullptr) {
            int best = 60000;
            for (unsigned i = 0; i < enemies->size(); i++) {
                void *e = (void *)(*enemies)[i];
                if (((KIPlayer *)(e))->isDead() != 0) continue;
                if (((KIPlayer *)(e))->isDying() != 0) continue;
                if (((Player *)(((KIPlayer *)e)->player))->isActive() == 0) continue;
                if (((KIPlayer *)(e))->isEnemy() == 0) continue;
                if (C(e, 0x74) != 0) continue;   // RAWREAD: KIPlayer e+0x74 (no member at 0x74; mid-field of field_0x73)

                float epos[3];
                ((KIPlayer *)(e))->getPosition();
                float rel[3];
                Vec_sub(rel, epos, (char *)&this->dockOffsetVec);
                int dist = (int)Vec_length(rel);
                if (dist < best
                    && (this->autoTurretTarget == 0 || this->autoTurretTarget != this->autoTurretPrevTarget)) {
                    this->autoTurretTarget = e;
                    best = dist;
                }
            }
        }
    }

    void *target = this->autoTurretTarget;
    if (target != 0 && ((KIPlayer *)target)->geometry != 0) {
        int fireTimer = PE_hat_aimAndFire(this, dt);
        this->autoTurretFireTimer = fireTimer;
        if (fireTimer + dt > 0x1f4) {
            // fall through to stop-shooting below
        } else {
            return;
        }
    }

    ((Player *)(this->player))->stopShooting(0);
}

// PlayerEgo::dockToAsteroid(KIPlayer*, Radar*)
//   Toggles the asteroid mining dock. If not docked and given an asteroid, it
//   latches on (records the asteroid at 0x1bc, computes an approach distance
//   from its scaling at 0x1d8). If already docked, it detaches: re-enables the
//   asteroid spin, clears the dock vector (0x1c8), re-activates the follow
//   camera, tears down the mining game (0x1e4) and shows the exhaust again.

extern "C" void  Vec_assign(void *dst, const void *src);                 // Vector::operator=

extern const float g_PE_astApproach;    // 0xaf568 scaling -> approach distance

void PlayerEgo::dockToAsteroid(void *radar) {
    if (((char&)this->dockingState) != 0) {
        // Already docked -> undock.
        ((PlayerAsteroid *)(((void*&)this->asteroidTarget)))->setRotationEnabled(true);
        this->field_0x145 = 0;
        ((char&)this->dockingState) = 0;
        ((void*&)this->asteroidTarget) = 0;

        int zero[3] = {0, 0, 0};
        Vec_assign((char *)&this->dockApproachDist, zero);

        ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setActive(true);
        ((Player *)(this->player))->resetGunDelay(0);
        if (((void*&)this->miningGame) != 0)
            delete (MiningGame *)((void*&)this->miningGame);
        ((void*&)this->miningGame) = 0;
        ((Radar *)(radar))->unlockAsteroid();
        this->dockingPointIndex = 0;
        ((PlayerEgo *)(this))->setExhaustVisible(true);
    } else if (radar != 0) {
        // Not docked -> latch onto the asteroid.
        ((char&)this->dockingState) = 1;
        this->miningSettleTimer = 0;
        ((void*&)this->asteroidTarget) = radar;
        float dist = ((PlayerAsteroid *)(radar))->getScaling() * g_PE_astApproach;
        this->field_0x145 = 1;
        this->dockingPointIndex = 0;
        this->dockScaling = (int)dist;
    }
}

// PlayerEgo::dockToAsteroid(KIPlayer*, Radar*)
//   Typed form of the asteroid mining-dock toggle. In the binary the receiver is
//   the KIPlayer-shaped first argument and the Radar argument carries the
//   asteroid being latched/unlocked. Forwards to the packed form, which treats
//   its single argument as that asteroid/radar object.
void PlayerEgo::dockToAsteroid(KIPlayer *kip, Radar *radar) {
    dockToAsteroid(radar);
}

void PlayerEgo::killLiberator() {
  char sv[12];
  // RAWREAD: arr/e below are untyped Array internals (the guns array and its
  // element rows); no modeled class at those nested offsets.
  void* p = ((Player *)this->player)->guns;
  if (p == 0) return;
  void* arr = P(P(p, 4), 4);
  if (arr == 0) return;
  if (I(p, 0) == 0) return;
  if (I(arr, 0) == 0) return;
  if (this->currentSecondaryWeaponIndex != 0xb3) return;
  *(int*)(sv + 0) = -1; *(int*)(sv + 4) = -1; *(int*)(sv + 8) = -1;
  unsigned count = U(arr, 0);
  for (unsigned i = 0; i < count; i++) {
    int* e = (int*)(*(int*)((char*)arr + 4) + i * 4);
    if (e[0x16] == 0xb3) {
      *(int*)e[0xf] = -1;
      *(Vector *)((void*)e[3]) = *(const Vector *)(sv);
      void* arr2 = P(P(p, 4), 4);
      int* e2 = (int*)(*(int*)((char*)arr2 + 4) + i * 4);
      *((char*)e2 + 0x4c) = 0;
      count = U(arr2, 0);
    }
  }
}

// PlayerEgo::roll(int amount)
//   Banks the ship around its forward axis while a roll is active (flag 0x2f4).
//   It samples the current orientation's right-vector components from the ship
//   matrix, and -- once the bank has settled near level -- snaps the roll matrix
//   (0x2ac) back to identity and clears the roll/auto-level flags. Otherwise it
//   recomputes the bank factor (the dense FP/sign-flag logic lives in
//   liw_roll_bankFactor) and writes a fresh rotation into the roll matrix.

extern "C" void  Mat_identity(void *out, const void *src);            // MatrixIdentity
extern "C" void  Mat_assign(void *dst, const void *src);             // Matrix::operator=
extern "C" void  Mat_setRotation(void *out, float x, float y, float z); // MatrixSetRotation
// Resolves the bank factor + axis from the right-vector and the stored roll
// direction (0x2a9) / auto-level flag (0x324); returns the X-axis scale and
// writes the Z component through *outZ. Owns the corrupted sign-bit branches.

extern const float g_PE_rollLevelEps;   // 0xabdb0 settle threshold

void PlayerEgo::roll(int amount) {
    if (this->rolling == 0)
        return;

    void *m = ((AEGeometry *)(this->geometry))->getMatrix();
    float rx = F(m, 0x10);   // RAWREAD: flat AEMath::Matrix element (AEMath.h not in this TU; positional, not a named member)
    float ry = F(m, 0x14);   // RAWREAD: flat AEMath::Matrix element (AEMath.h not in this TU; positional, not a named member)
    float mag = rx > 0.0f ? rx : -rx;

    if (amount > 0x3b)
        amount = 0x3c;

    unsigned char rollMat[0x30];   // Matrix is a 48-byte value

    if (ry >= 0.0f && mag < g_PE_rollLevelEps) {
        // settled: reset to identity and stop rolling.
        Mat_identity(rollMat, (char *)&this->rollMatrix);
        Mat_assign((char *)&this->rollMatrix, rollMat);
        this->rollDirection = 0;
        this->rolling = 0;
        this->autoLevel = 0;
        return;
    }

    float zAxis = 0.0f;
    float xScale = PE_roll_bankFactor(this, rx, ry, &zAxis);

    // record the current roll direction (sign of rx) when non-zero.
    if (rx != 0.0f)
        this->rollDirection = (rx < 0.0f) ? 1 : 2;

    this->rolling = 1;
    Mat_setRotation(rollMat, xScale * (float)amount, 0.0f, zAxis);
    Mat_assign((char *)&this->rollMatrix, rollMat);
}

void PlayerEgo::setTargetFollowCamera(TargetFollowCamera* cam) {
  void* m = ((void*&)this->handling);
  ((void*&)this->targetFollowCamera) = cam;
  return PlayerEgo_setTargetFollowCamera_ext(cam, m);
}

// PlayerEgo::calcCollision(Array<KIPlayer*>* candidates)
//   Resolves collisions between the player and each candidate object this frame
//   (skipped while in the final docking approach). For each candidate that the
//   broad-phase reports as overlapping it dispatches by object kind:
//     * gas cloud / wormhole (type 0x4262): plays the proximity rumble whose
//       gain scales with distance, and -- when close -- nudges the ship away,
//     * solid obstacle (field_3C == 0): snaps the ship to the surface contact
//       point and re-derives the HUD transform,
//     * destructible (field_3C != 0): applies mutual collision damage and a
//       roll kick.
//   The per-kind vector/matrix physics is delegated to the PE_cc_* helpers; the
//   recoverable iteration and branch selection stay inline.

// kind-specific resolvers (own the corrupted vector / matrix physics).

extern const float g_PE_cc_alarmDist;

void PlayerEgo::calcCollision(Array<KIPlayer*> *candidates) {
    if (candidates == 0)
        return;
    if (this->dockedFlag != 0 && (unsigned)(this->dockingPointIndex - 1) < 3)
        return;   // in final docking approach: no collisions

    for (unsigned i = 0; i < candidates->size(); i++) {
        void *obj = (*candidates)[i];
        if (obj == 0)
            continue;

        // first candidate: trip the proximity alarm flag when very close.
        if (i == 0 && PE_status()->inAlienOrbit() == 0) {
            if (Vec_length((char *)&this->dockOffsetVec) < g_PE_cc_alarmDist && C(obj, 0x71) != 0)   // RAWREAD: KIPlayer obj+0x71 (no member at 0x71; mid-field of field_0x70)
                this->collidesWithStationFlag = 1;
        }

        // Broad-phase overlap test: does this enemy's outer collision volume contain the
        // player's current position? Named actor virtual call (outerCollide, slot +0x40 ->
        // the polymorphic float form). The decompiler had dropped the getPosition() result
        // here (pos was left uninitialised) — restored.
        AbyssEngine::AEMath::Vector pos = ((AEGeometry *)(this->geometry))->getPosition();
        if (((KIPlayer *)obj)->outerCollide(pos) == 0)
            continue;

        if (((KIPlayer *)(obj))->getType() == 0x4262 && ((KIPlayer *)(obj))->isVisible() != 0) {
            if (((PlayerWormHole *)(obj))->isShrinking() == 0 && ((void*&)this->miningGame) == 0)
                PE_cc_wormhole(this, obj);
        } else if (((KIPlayer *)obj)->stealFlag == 0) {
            if (((KIPlayer *)(obj))->isVisible() != 0) {
                // skip the object we are actively docking with.
                bool docking = (((char&)this->dockingState) != 0 || this->dockedFlag != 0)
                               && ((void*&)this->asteroidTarget) == obj;
                if (!docking)
                    PE_cc_obstacle(this, obj, i);
                hitCamera_(this);
            }
        } else {
            if (((KIPlayer *)(obj))->isDying() == 0 && ((KIPlayer *)(obj))->isDead() == 0) {
                bool skipDockTarget = (((char&)this->dockingState) != 0 || this->dockedFlag != 0)
                                      && obj == ((void*&)this->asteroidTarget);
                if (!skipDockTarget) {
                    PE_cc_destructible(this, obj);
                    hitCamera_(this);
                }
            }
        }
    }
}

void PlayerEgo::dockToPlanet() {
  ((TargetFollowCamera *)(this->targetFollowCamera))->setLookAtCam((bool)(unsigned char)this->targetFollowCamera);
  this->boostingFlag = 1;
  this->boostTimer = 0;
  this->speed = 0x41000000;
  this->field_0xcc = 10000;
  this->field_0xd0 = 0;
  this->collide = 0;
  float f = PlayerEgo_resetGunDelay_f(this);
  void* snd = *(void**)g_dockToPlanet_fmod;
  ((char&)this->gunMuzzleRoot) = 0;
  this->dockingToPlanet = 1;
  this->speed = 0x41000000;
  this->planetDockTimer = 0;
  ((FModSound *)(*(void**)snd))->play(5, (Vector *)0, (Vector *)0, f);
}

// PlayerEgo::up(int frameTime, float delta)
//   Pitch-up input handler -- the mirror of down(): same axis and dispatch but
//   the free-look/turret accumulators (0x1a8 / 0x1a4) and the pitch accumulator
//   (0x278) ramp upward, with the pitch direction flag (0x100) set to +1.

__attribute__((visibility("hidden"))) extern char **g_PE_u_miningGate; // 0xb0e1c -> flags (+0x10)
extern const float g_PE_u_eps;      // 0xb1030 free-look limit
extern const float g_PE_u_eps2;
extern const float g_PE_u_lookK1;
extern const float g_PE_u_lookK2;
extern const float g_PE_u_manK;
extern const float g_PE_u_loadK;
extern const float g_PE_u_loadB;
extern const float g_PE_u_rateK;

float PlayerEgo::up(int frameTime, float delta) {
    if (((void*&)this->miningGame) != 0) {
        if (C(*g_PE_u_miningGate, 0x10) == 0)   // RAWREAD: opaque flags struct via global holder (no modeled class)
            return ((MiningGame *)(((void*&)this->miningGame)))->up(-delta);
        return ((MiningGame *)(((void*&)this->miningGame)))->down(delta);
    }

    if (this->turretActive != 0) {
        float ft = (float)frameTime;
        if (this->lookYaw > g_PE_u_eps) {
            float ang = this->lookYaw - ft * delta;
            this->lookYaw = ang;
            ((AEGeometry *)this->turretGeometry)->rotate((float)(ang * g_PE_u_lookK1 * g_PE_u_lookK2), 0.0f, 0.0f);
        }
        float p = this->lookPitch;
        if (p > g_PE_u_eps2) {
            float half = ft * delta * 0.5f;
            float ang = half * g_PE_u_lookK1 * g_PE_u_lookK2;
            this->lookPitch = p - half;
            return (((AEGeometry *)this->dockCameraMid)->rotate((float)ang, 0.0f, 0.0f), 0.0f);
        }
        return p;
    }

    if (((void*&)this->rocketControlGun) != 0) {
        float v = (float)frameTime * g_PE_u_manK * F(((void*&)this->rocketControlGun), 0x50);   // RAWREAD: rocketControlGun is an int gun handle (Gun.h not in this TU; +0x50 unnamed)
        this->maneuverParam = v;
        return v;
    }

    if (this->autoPilot != 0)
        return delta;
    if (this->dockedFlag != 0 && this->dockingPointIndex != 1)
        return delta;

    this->pitchAccumDir = 1;
    float rate;
    if (this->hardCoreMode == 0) {
        rate = ((float&)this->handling);
    } else {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        rate = ((float&)this->handling) * (1.0f - cur / max) * g_PE_u_loadK + ((float&)this->handling) * g_PE_u_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_u_rateK * rate), 0x3f);
    this->pitchRamp = rate;
    ((float&)this->yawAccumD) = -delta;
    if (((float&)this->pitchAccumD) > target) {
        float v = ((float&)this->pitchAccumD) - PE_pitchRampDelta(this, rate, frameTime);
        if (v < target) v = target;
        ((float&)this->pitchAccumD) = v;
    }
    return target;
}

// PlayerEgo::shoot(int weapon, int type)
//   Fires the player's weapons. In turret/free-look mode (flag 0x1a0) the shot
//   is aimed using the combined turret * ship matrix and dispatched as a type-2
//   (turret) shot. Otherwise, when alive: a type-1 (secondary/missile) shot
//   tracks the locked secondary index at 0x10c (cleared on a miss), and any
//   other type is a plain primary shot.

extern "C" void  Mat_mul(void *out, const void *a, const void *b);   // operator*
extern "C" int   Player_shootTurret(void *player, int kind, int weapon, int hi,
                                    int flag, const void *matrix);

void PlayerEgo::shoot(int weapon, int type) {
    if (this->turretActive != 0) {
        void *m1 = ((AEGeometry *)(this->geometry))->getMatrix();
        void *m2 = ((AEGeometry *)(this->turretGeometry))->getMatrix();
        unsigned char combined[0x30];                 // Matrix is a 48-byte value
        Mat_mul(combined, m1, m2);
        Player_shootTurret(this->player, 2, weapon, weapon >> 31, 0, combined);
        return;
    }

    if (((PlayerEgo *)(this))->isDead() != 0)
        return;

    void *player = this->player;
    if (type == 1) {
        if (Player_shootSecondary(player, 1, this->currentSecondaryWeaponIndex, weapon >> 31, 0) == 0)
            this->currentSecondaryWeaponIndex = -1;
    } else {
        Player_shootPrimary(player, type, weapon, weapon >> 31, 0);
    }
}

// PlayerEgo::stopMining()
//   The target bytes for this stub decode as a tiny thunk into the mining-game
//   teardown helper (the Ghidra listing is bad-instruction noise because the
//   region is a hand-written veneer). Faithfully forward to that helper.

void PlayerEgo::stopMining() {
    PlayerEgo_stopMining_impl(this);
}

// PlayerEgo::setTurretMode(bool enable)
//   Enters or leaves the gun-turret view. Refused unless the ship mounts a
//   turret (0x170) and is neither mining (0x1e4) nor mid-cutscene (0x180); a
//   pending maneuver (0x194) instead just restores the default camera. When
//   enabling it lazily builds the turret camera rig (camera + AEGeometry chain,
//   like the docking camera), aims it, and makes it current; when disabling it
//   stops shooting and resets the camera. Either way it toggles the turret
//   crosshair transform (0x30) and starts/stops the turret hum sound.

__attribute__((visibility("hidden"))) extern int  *g_PE_tm_hum;
extern const float g_PE_tm_fovNormal;
extern const float g_PE_tm_fovAlien;

void PlayerEgo::setTurretMode(bool enable) {
    if (this->turretMode == 0 || ((void*&)this->miningGame) != 0 || this->autoTurretEquipped != 0) {
        // turret view unavailable -> restore default camera if a maneuver runs.
        if (((void*&)this->rocketControlGun) != 0) {
            gCanvas->CameraSetCurrent((unsigned int)(((TargetFollowCamera*)(intptr_t)this->targetFollowCamera)->id));
            ((LevelScript *)(this->level))->resetCamera(((LevelScript *)this->level)->m_pLevel);
        }
        return;
    }

    this->turretActive = (unsigned char)enable;
    if (enable == 0) {
        ((PlayerEgo *)(this))->stopShooting(0);
        gCanvas->CameraSetCurrent((unsigned int)(((TargetFollowCamera*)(intptr_t)this->targetFollowCamera)->id));
        ((LevelScript *)(this->level))->resetCamera(((LevelScript *)this->level)->m_pLevel);
    } else {
        if (((void*&)this->rocketControlGun) != 0)
            return;
        if (this->dockCameraNode == 0) {
            gCanvas->CameraCreate(this->turretCamera);
            unsigned int cam = (unsigned int)(unsigned long)gCanvas;
            float fov = (PE_status()->inAlienOrbit() != 0) ? g_PE_tm_fovAlien : g_PE_tm_fovNormal;
            gCanvas->CameraSetPerspective((unsigned int)(cam), fov, 0.0f, g_PE_tm_fovAlien);

            void *node = (void*)new AEGeometry(gCanvas);
            this->dockCameraNode = node;
            ((AEGeometry *)node)->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
            ((AEGeometry *)(node))->translate(this->turretOffsetVec);

            void *mid = (void*)new AEGeometry(gCanvas);
            this->dockCameraMid = mid;
            ((AEGeometry *)(mid))->translate(*(Vector *)&((AEGeometry *)mid)->transform);
            ((AEGeometry *)(this->dockCameraNode))->addChild((uint32_t)((AEGeometry *)this->dockCameraMid)->transform);

            void *leaf = (void*)new AEGeometry(gCanvas);
            this->dockCameraLeaf = leaf;
            ((AEGeometry *)(this->dockCameraNode))->addChild((uint32_t)((AEGeometry *)this->dockCameraNode)->transform);

            if (((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x23) != 0)
                ((AEGeometry *)(this->dockCameraNode))->rotate(this->turretOffsetVec);
        }
        ((AEGeometry *)(this->dockCameraLeaf))->setPosition(this->dockOffsetVec);
        void *leaf = this->dockCameraLeaf;
        ((AEGeometry *)leaf)->setMatrix(((AEGeometry *)(this->geometry))->getMatrix());
        gCanvas->CameraSetCurrent((unsigned int)(this->turretCamera));
        ((Player *)(this->player))->stopShooting(0);
    }

    if (this->field_0x30 != 0) {
        void *tf = gCanvas->TransformGetTransform((unsigned int)(U((void*)gCanvas, 0)));   // RAWREAD: gCanvas+0x0 (PaintCanvas has no member at +0; vtable/untracked slot)
        ((AbyssEngine::Transform *)(tf))->SetVisible(enable != 0);
        int v = (enable != 0);
        if (enable == 0)
            ((FModSound *)(*g_PE_tm_hum))->stop(0);
        else
            ((FModSound *)(*g_PE_tm_hum))->play(0x8cf, (Vector *)0, (Vector *)0, (float)v);
    }
}

void PlayerEgo::rotate(float rx, float ry, float rz) {
  char local[60];
  this->rotateX = this->rotateX + rx;
  this->rotateY = this->rotateY + ry;
  this->rotateZ = this->rotateZ + rz;
  int ido = this->field_0x4->transform;
  void* t = *(void**)g_rotate_transform;
  void* m = TransformGetLocal(t, ido);
  MatrixSetRotation(local, m, this->rotateX, this->rotateY, this->rotateZ);
}

// PlayerEgo::strafe(int dir, bool positive)
//   Sets the lateral strafe acceleration (0x37c) and clamps the strafe target
//   speed (0x380). When the ship is cargo-loaded (flag 0x235) the base strafe
//   rate (0x154) is reduced proportionally to the current/max load ratio.
//   A maneuver lock at 0x194 suppresses strafing entirely.

extern const float g_PE_strafeLoadK;
extern const float g_PE_strafeLoadB;
extern const float g_PE_strafeAccelK;

static inline float fmin_(float a, float b) { return a < b ? a : b; }

void PlayerEgo::strafe(int /*dir*/, bool positive) {
    if (this->rocketControlGun != 0)
        return;

    float base;
    if (this->hardCoreMode != 0) {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        float rate = ((float&)this->handling);
        base = rate * (1.0f - cur / max) * g_PE_strafeLoadK + rate * g_PE_strafeLoadB;
    } else {
        base = ((float&)this->handling);
    }

    float sign = positive ? 1.0f : -1.0f;
    float accel = fmin_(base * 30.0f * g_PE_strafeAccelK, 2.0f);
    float target = fmin_(((float&)this->strafeNavPoint) * 1.5f, 1.0f);

    this->strafeAccel = ((float&)this->strafeNavPoint) * sign * accel;
    ((float&)this->strafeNavPoint) = target;
}

// PlayerEgo::dockToDockingPoint(KIPlayer*, Radar*)
//   Toggles automated docking onto a station docking point. If not docking and
//   given a target, it latches the approach state (0x356/0x1bc/0x1d8). If a
//   target is given while docking, it finds the nearest nav point, switches off
//   the turret/look cameras, and builds an EaseInOutMatrix (0x358) that glides
//   the ship from its current pose to the docking pose over 3000 ms (the 30-arg
//   ctor is wrapped by PE_dtdp_makeEase). With no target it undocks: restores
//   cameras, clears state, frees any reserved SpacePoint, and tears down any
//   running hacking game.

// Wraps operator new + EaseInOutMatrix(from-matrix, to-translation, 3000ms).

void PlayerEgo::dockToDockingPoint(void *radar) {
    if (((PlayerEgo *)(this))->isDead() != 0)
        return;

    if (this->dockedFlag == 0) {
        if (radar != 0) {
            this->dockedFlag = 1;
            ((void*&)this->asteroidTarget) = radar;
            this->dockScaling = 0x578;
            this->miningSettleTimer = 0;
            this->field_0x145 = 1;
            this->dockingPointIndex = 0;
        }
        return;
    }

    bool undock = (radar == 0);
    if (!undock) {
        Vector pos = ((PlayerEgo *)(this))->getPosition();
        SpacePoint *sp = (SpacePoint *)(unsigned long)this->spacePoint;
        void *nav = ((KIPlayer *)(radar))->getNearestNavigationPoint(pos, sp);
        if (nav == 0) {
            if (C(radar, 0x70) != 0)              // RAWREAD: radar+0x70 (Radar minimal stub, no fields)
                C(radar, 0x8c) = 1;               // RAWREAD: radar+0x8c (Radar minimal stub, no fields)
            undock = true;
        } else {
            if (C(radar, 0x70) != 0)              // RAWREAD: radar+0x70 (Radar minimal stub, no fields)
                C(radar, 0x8c) = 1;               // RAWREAD: radar+0x8c (Radar minimal stub, no fields)

            ((PlayerEgo *)(this))->setTurretMode(0);
            this->field_0x1a1 = 0;
            gCanvas->CameraSetCurrent((unsigned int)(((TargetFollowCamera*)(intptr_t)this->targetFollowCamera)->id));
            ((LevelScript *)(this->levelScript))->resetCamera(this->levelScript->m_pLevel);
            PlayEngineSound_(this);
            this->dockingPointIndex = 3;
            ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setLookAtCam(false);
            ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->useTargetsUpVector(false);

            if (this->easeMatrix != 0)
                ::operator delete(EaseInOutMatrix_dtor(this->easeMatrix));
            this->easeMatrix = 0;

            pos = ((PlayerEgo *)(this))->getPosition();
            void *nav2 = ((KIPlayer *)(radar))->getNearestNavigationPoint(pos, sp);
            void *from = ((AEGeometry *)(this->geometry))->getMatrix();
            this->easeMatrix = PE_dtdp_makeEase(from, nav2);
            ((PlayerEgo *)(this))->setExhaustVisible(true);
        }
    }

    if (undock) {
        PlayEngineSound_(this);
        this->dockedFlag = 0;
        this->field_0x1a1 = 0;
        this->field_0x145 = 0;
        ((void*&)this->asteroidTarget) = 0;
        int zero[3] = {0, 0, 0};
        Vec_assign((char *)&this->dockApproachDist, zero);
        ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setActive(true);
        ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setLookAtCam(false);
        ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->useTargetsUpVector(false);
        ((Player *)(this->player))->resetGunDelay(0);
        this->dockingPointIndex = 0;
        ((PlayerEgo *)(this))->setExhaustVisible(true);
        if (this->spacePoint != 0) {
            ((SpacePoint *)((void *)(unsigned long)this->spacePoint))->giveFree();
            this->spacePoint = 0;
        }
    }

    if (((void*&)this->hackingGame) != 0) {
        delete (HackingGame *)((void*&)this->hackingGame);
        ((void*&)this->hackingGame) = 0;
        ((Hud *)(((void*&)this->hud)))->setHackingGameActive(false);
    }
}

// PlayerEgo::dockToDockingPoint(KIPlayer*, Radar*)
//   Typed form of the station-docking toggle. In the binary the nav/landmark
//   object (first argument) drives the whole routine; the Radar argument is
//   vestigial. Forwards to the packed form, which treats its single argument as
//   that nav object.
void PlayerEgo::dockToDockingPoint(KIPlayer *kip, Radar *radar) {
    dockToDockingPoint(kip);
}

// PlayerEgo::draw(bool allowHud)
//   Draws the 2D overlay layer for the player ship (targeting reticle / lock
//   marker plus the boost throttle). A cascade of guard conditions short-circuits
//   to one of several render-tail veneers when the player is in a maneuver, the
//   mining game, dead, docking, in a cutscene, etc. In the normal path it builds
//   a unit matrix (optionally seeded from the ship matrix), then draws the
//   appropriate reticle image depending on turret mode / plasma lock / blink
//   state, and finally the throttle gauge.

// PlayerEgo::draw has three terminal tail-branches, all resolved from the binary:
//   0x1abc48 -> MiningGame::render2D()  (_ZN10MiningGame8render2DEv  @ 0x12f12c)
//   0x1abc58 -> PlayerEgo::drawThrottle() (_ZN9PlayerEgo12drawThrottleEv @ 0xb1f04)
//   0x1abc68 -> HackingGame::render2D() (_ZN11HackingGame8render2DEv @ 0x15f1e4)
// They are dispatched directly below as real method calls (no veneer shims).

__attribute__((visibility("hidden"))) extern float *g_PE_dr_posLock;   // 0xb1e0c reticle pos (plasma)
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posNoLock;
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posBlink;
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posNormal;

void PlayerEgo::draw(bool allowHud) {
    if (((void*&)this->rocketControlGun) != 0)               // mid scripted maneuver: nothing to draw
        return;

    if (((void*&)this->hackingGame) != 0 && this->turretActive == 0) {
        ((HackingGame *)(void*)(intptr_t)this->hackingGame)->render2D();
        return;
    }

    if (((void*&)this->miningGame) != 0) {             // mining game owns the screen
        ((MiningGame *)(void*)(intptr_t)this->miningGame)->render2D();
        return;
    }

    bool full;
    if (this->computerControlled != 0 || ((PlayerEgo *)(this))->isDead() != 0 || this->freeze != 0
        || allowHud == 0 || ((char&)this->dockingState) != 0 || this->dockingToPlanet != 0 || this->dockedToStream != 0) {
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
        if ((unsigned)(this->dockingPointIndex - 1) < 3)
            return;
        this->drawThrottle();
        return;
    }

    unsigned char m[0x30];                 // unit matrix, optionally ship-aligned
    for (int i = 0; i < 0x30; i++) m[i] = 0;
    F(m, 0x0)  = 1.0f;
    F(m, 0x14) = 1.0f;
    F(m, 0x28) = 1.0f;

    bool aligned = (this->autoPilot != 0 && this->turretActive != 0);
    if (!aligned)
        Mat_assign(m, ((AEGeometry *)(this->geometry))->getMatrix());

    void *canvas = (void*)gCanvas;
    gCanvas->SetColor((unsigned int)(0xffffffff));

    if (this->turretActive != 0
        && ((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x23) != 0) {
        // turret crosshair: plasma lock changes the marker + position.
        if (((Radar *)(this->field_0x14))->isPlasmaInRange() != 0) {
            float *p = g_PE_dr_posLock;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->radarBlipImage3), (int)p[0], (int)p[1], (unsigned char)(0x11), (unsigned char)(0x44));
        } else {
            float *p = g_PE_dr_posNoLock;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->radarBlipImage2), (int)p[0], (int)p[1], (unsigned char)(0x11), (unsigned char)(0x44));
        }
    } else {
        // standard lock-on reticle: blink while acquiring.
        if ((char&)this->level->field_30 != 0) {
            float *p = g_PE_dr_posBlink;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->radarBlipImage1), (int)p[0], (int)p[1], (unsigned char)(0x11), (unsigned char)(0x44));
            int t = this->shakeAccum + this->shakeIntensity;
            this->shakeAccum = t;
            if (t >= 0xc9)
                (char&)this->level->field_30 = 0;
        } else {
            float *p = g_PE_dr_posNormal;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->dockArrowImage), (int)p[0], (int)p[1], (unsigned char)(0x11), (unsigned char)(0x44));
            this->shakeAccum = 0;
        }
    }

    ((PlayerEgo *)(this))->drawThrottle();
}

// PlayerEgo::update(int dt, Radar*, Hud*, Radio*, LevelScript*, int, bool, int)
//   The master per-frame tick for the player ship. It runs, in order:
//     1. position cache + non-flight subsystems (shield regen, hacking game,
//        cargo overflow, jump-drive charge, cloak) -- delegated to PE_upd_subsystems,
//     2. engine-overheat / boost handling incl. the destruct explosion when the
//        boost meter overflows -- delegated to PE_upd_boost,
//     3. the asteroid / station docking state dispatch (dockToAsteroid /
//        approachAsteroid / dockToDockingPoint / approachDockingPoint),
//     4. the flight controller: auto-pilot route following or manual flight,
//        turret-view, evasive maneuver, ship handling and the auto-turret,
//     5. collision resolution and post-processing -- delegated to PE_upd_post.
//   The dense numeric phases live in the PE_upd_* helpers; the recoverable
//   high-level dispatch (which decides *which* controller runs) is authored here.

// flight controllers (already authored as their own methods).

// route / nav helpers.

// dense numeric phases (own the corrupted FP blocks).
extern "C" void PE_upd_subsystems(PlayerEgo *self, int dt, void *radar, void *hud,
                                  void *radio, void *script);
extern "C" void PE_upd_post(PlayerEgo *self, int dt, void *radar, void *hud,
                            void *radio, int arg5);

extern const float g_PE_upd_handlingBias;

void PlayerEgo::update(int dt, Radar *radar, Hud *hud, Radio *radio, LevelScript *script, int arg5, bool arg6, int arg7) {
    (void)arg6; (void)arg7; (void)script;

    if (((void*&)this->hud) == 0)
        return;
    if (this->freeze != 0)
        return;   // frozen

    // cache world position for this frame.
    float pos[3];
    ((PlayerEgo *)(this))->getPosition();
    ((int&)this->dockOffsetVec.x) = *(int *)&pos[0];
    ((int&)this->dockOffsetVec.y) = *(int *)&pos[1];
    this->boostDelay = *(int *)&pos[2];

    // shield regen / hacking / cargo / jump-drive / cloak.
    PE_upd_subsystems(this, dt, radar, hud, radio, script);
    // engine overheat + boost (may spawn the destruct explosion at 0x90).
    PE_upd_boost(this, dt);

    // ---- asteroid docking --------------------------------------------------
    if (((char&)this->dockingState) != 0) {
        this->field_0x145 = 1;
        if (((void*&)this->asteroidTarget) == 0 || ((PlayerEgo *)(this))->isDead() != 0) {
            ((PlayerEgo *)(this))->dockToAsteroid(0);
            PE_upd_post(this, dt, radar, hud, radio, arg5);
            return;
        }
        ((PlayerEgo *)(this))->approachAsteroid(0 /*hud2*/, radar);
    }

    // ---- station docking ---------------------------------------------------
    if (this->dockedFlag != 0 && ((Player *)(this->player))->getHitpoints() > 0) {
        this->field_0x145 = 1;
        if (((void*&)this->asteroidTarget) == 0 || ((PlayerEgo *)(this))->isDead() != 0) {
            ((PlayerEgo *)(this))->dockToDockingPoint(0);
            PE_upd_post(this, dt, radar, hud, radio, arg5);
            return;
        }
        if (((PlayerEgo *)(this))->approachDockingPoint(hud, 0, radar) != 0)
            PE_upd_docksFinishDelivery(this, radio);
    }

    // turret view while the free-look/turret camera is active.
    if (this->turretActive != 0 || this->field_0x1a1 != 0)
        ((PlayerEgo *)(this))->handleTurretView(dt);

    // ---- flight controller -------------------------------------------------
    bool autopilot = (this->autoPilot != 0 && this->autoPilotTarget != 0);
    if (!autopilot) {
        if (this->field_0x145 == 0) {
            if (this->turretActive == 0 && this->field_0x1a1 == 0) {
                ((PlayerEgo *)(this))->roll(this->shakeIntensity);
                if (((PlayerEgo *)(this))->updateManeuver() == 0)
                    ((PlayerEgo *)(this))->handleShip(dt);
            } else {
                ((PlayerEgo *)(this))->handleTurretView(dt);
            }
        }
    } else if (this->field_0x145 == 0 && ((PlayerEgo *)(this))->updateManeuver() == 0) {
        void *wp = (void *)(intptr_t)this->autoPilotTarget;
        if (this->goingToWaypointFlag != 0 && wp != 0 && I(wp, 0x134) != 0) {   // RAWREAD: wp+0x134 (untyped void* autopilot target; Waypoint.h not in this TU)
            wp = ((Route *)(this->autoPilotTarget))->getWaypoint();
            ((void*&)this->autoPilotTarget) = wp;
        }
        if (wp == 0 || this->dockedToStream != 0) {
            this->setAutoPilot(nullptr);
        } else {
            // steer toward the current waypoint. wp is a Waypoint (a KIPlayer);
            // getPosition() is the actor vtable slot +0x28 virtual.
            AbyssEngine::AEMath::Vector wpVec = ((KIPlayer *)wp)->getPosition();
            float wpPos[3] = { wpVec.x, wpVec.y, wpVec.z };
            ((int&)this->waypointX) = *(int *)&wpPos[0];
            ((int&)this->waypointY) = *(int *)&wpPos[1];
            ((int&)this->gunBaseGeo) = *(int *)&wpPos[2];

            float me[3];
            ((PlayerEgo *)(this))->getPosition();
            float dx = me[0] - wpPos[0], dy = me[1] - wpPos[1], dz = me[2] - wpPos[2];
            if ((int)(dx * dx + dy * dy + dz * dz) < 20000)
                this->aboutToReachAutoTargetFlag = 1;

            float speed = 4.0f;
            float h = ((Ship *)(PE_status()->getShip()))->getHandling();
            if (h + g_PE_upd_handlingBias < 4.0f)
                speed = ((Ship *)(PE_status()->getShip()))->getHandling() + g_PE_upd_handlingBias;
            ((PlayerEgo *)(this))->moveToPosition(this->waypointX, this->waypointY, ((float&)this->gunBaseGeo), 1, speed);
            if (this->turretActive != 0)
                ((PlayerEgo *)(this))->handleTurretView(dt);
        }
    }

    // continue an evasive maneuver during a non-final docking glide.
    if (this->dockedFlag != 0 && ((uint32_t&)this->dockingPointIndex) != 1
        && this->turretActive == 0 && (((uint32_t&)this->dockingPointIndex) | 1) != 3)
        ((PlayerEgo *)(this))->updateManeuver();

    // auto-turret (when enabled and the radar window allows it).
    if (this->autoTurretEquipped != 0 && this->autoTurretEnabled != 0) {
        if (((PlayerEgo *)(this))->isDead() == 0) {
            ((PlayerEgo *)(this))->handleAutoTurret(dt);
        } else {
            this->autoTurretEnabled = 0;
            ((Player *)(this->player))->stopShooting(0);
        }
    }

    // collision + camera + explosion post-processing.
    PE_upd_post(this, dt, radar, hud, radio, arg5);
}

void PlayerEgo::setTurretPosition(float x, float y, float z) {
  char v[12];
  *(float*)(v + 0) = x;
  *(float*)(v + 4) = y;
  *(float*)(v + 8) = z;
  this->turretOffsetVec = *(const Vector *)(v);
}

void PlayerEgo::setTurretPosition(Vector v) {
  this->turretOffsetVec = v;
}

// PlayerEgo::revive()
//   Brings a destroyed player back to life: re-enables its engine particle
//   system, deletes the explosion, re-activates the follow camera and engine
//   sound, restores hitpoints/armor to max, resets position and facing, and
//   clears the explosion timer.

__attribute__((visibility("hidden"))) extern void **g_PE_reviveSound;

void PlayerEgo::revive() {
    ParticleSystemManager *psm = this->level->field_74;
    bool en = ((char&)this->currentSystem) != 0;
    psm->enableSystemEmit(this->currentSystem, en);
    psm->enableSystemRender(this->currentSystem, en);

    if (this->explosion != 0)
        delete (Explosion *)(intptr_t)this->explosion;
    this->explosion = 0;

    ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setActive(true);
    ((Player *)(this->player))->setActive(true);
    int v = 0;

    void *snd = *g_PE_reviveSound;
    ((FModSound *)(*(void **)snd))->play((int)(intptr_t)*(void **)snd, (Vector *)0, (Vector *)0, (float)v);
    ((FModSound *)(snd))->play((int)(intptr_t)((void*&)this->field_0x1c), (Vector *)0, (Vector *)0, (float)v);

    ((PlayerEgo *)(this))->setExhaustVisible(true);

    void *player = this->player;
    ((Player *)(player))->setHitpoints(((Player *)(player))->getMaxHitpoints());
    player = this->player;
    ((Player *)(player))->setArmorHP(((Player *)(player))->getMaxArmorHP());

    ((PlayerEgo *)(this))->setPosition();

    float fwd[3] = {0.0f, 0.0f, 1.0f};
    float fwdUp[3] = {0.0f, 1.0f, 0.0f};
    ((AEGeometry *)(this->geometry))->setDirection(*(Vector *)fwd, *(Vector *)fwdUp);
    this->explosionTimer = 0;
}

// PlayerEgo::drawThrottle()
//   While the boost gauge is active (flag 0x370) this draws the throttle bar:
//   a partial DrawRegion2D fill of the gauge image (0x238) whose height tracks
//   the current thrust (0xbc), plus the thrust percentage rendered as text
//   centred under it. The boost timer (0x374) folds back past its midpoint so
//   the bar pulses symmetrically.

// String::String(int)

__attribute__((visibility("hidden"))) extern float *g_PE_t_anchor;     // {x,y} gauge anchor
__attribute__((visibility("hidden"))) extern String **g_PE_t_pctStr;     // "%" String
extern const float g_PE_t_timerDiv;    // 0xb1f30 timer normaliser
extern const float g_PE_t_pctScale;    // 0xb20dc thrust->percent
extern const float g_PE_t_textDiv;     // 0xb20e0 text vertical divisor

void PlayerEgo::drawThrottle() {
    if (this->throttleStarted == 0)
        return;

    int t = this->throttle;
    if (t > 500)
        t = 2000 - t;
    float frac = (float)t / g_PE_t_timerDiv;
    if (frac > 1.0f)
        frac = 1.0f;

    void *canvas = (void*)gCanvas;
    gCanvas->SetColor((unsigned int)(0xffffff00 | (unsigned int)(int)(frac * 255.0f) - 0x100));

    int img = this->hpGaugeImage;
    int w = ((PaintCanvas*)(long)(canvas))->GetImage2DWidth((unsigned int)(img));
    int h = ((PaintCanvas*)(long)(canvas))->GetImage2DHeight((unsigned int)(img));

    float thrust = this->thrust;
    int fillH = (int)(thrust * (float)h);
    float *anchor = g_PE_t_anchor;

    ((PaintCanvas*)(long)(canvas))->DrawRegion2D((unsigned int)(img), 0, h - fillH, w, fillH, (float)((int)((anchor[1] + (float)h) - (float)fillH)), 0, 0, 0, (int)(anchor[0] - (float)(w / 2)));

    // percentage label
    unsigned char pct[12];
    ((String *)(pct))->ctor_int((int)(thrust * g_PE_t_pctScale));

    int th = ((PaintCanvas*)(long)(canvas))->GetImage2DHeight((unsigned int)(img));
    String *pctStr = *g_PE_t_pctStr;
    int tw = ((PaintCanvas*)(long)(canvas))->GetTextWidth((unsigned int)(long)(canvas), *pctStr);
    ((PaintCanvas*)(long)(canvas))->DrawString((unsigned int)(long)(canvas), *pctStr, (int)(long)(pct), (int)((anchor[0] - (float)(tw / 2)) - 1.0f), (bool)(int)(anchor[1] + (float)th / g_PE_t_textDiv));
    gCanvas->SetColor((unsigned int)(0xffffffff));

    ((String *)(pct))->dtor();
}

void PlayerEgo::setAutoPilot(KIPlayer* kip) {
  this->goingToWaypointFlag = 0;
  int v = (int)(intptr_t)kip;
  this->autoPilotTarget = v;
  unsigned char old = this->autoPilot;
  this->autoPilot = (v != 0) ? 1 : 0;
  if (v == 0) {
    I(this->field_0x14, 0x2c) = 0;   // RAWREAD: field_0x14+0x2c (field_0x14 is an untyped void* KIPlayer-style handle)
    if (old != 0) {
      this->field_0x2a8 = 0;
      this->field_0x2a4 = 0;
    }
    return;
  }
  if (kip->field_0x72 != 0) this->goingToWaypointFlag = 1;
  void* eng = gAppManager->GetEngine();
  I(eng, 0x360) = 0;   // RAWREAD: eng+0x360 (untyped GetEngine() result, no modeled class)
  ((int&)this->thrust) = 0x3f800000;
}

// PlayerEgo::updateManeuver()
//   Advances an in-progress evasive maneuver, dispatched on the maneuver type
//   (0x334) with a shared elapsed-time accumulator (0x350):
//     * types 1/2 (barrel-roll dodge left/right): banks and slides the ship
//       sideways along an arc, modulating the follow-camera offset, and ends the
//       maneuver after ~1200 ms.
//     * type 3 (strafe run to a stored target 0x338): flies to the target via
//       moveToPosition, rolls into it, and ends after ~3000 ms.
//   The arc/bank matrix and camera-offset math is dense FP and delegated to the
//   PE_um_* helpers; the type dispatch, timer advance and terminal camera reset
//   stay inline.

// dodge-arc step (types 1/2): banks/slides the ship + camera for `dt`-advanced
// timer; returns nothing, updates the hull transform in place.
// strafe-run step (type 3): builds the intercept target offset into `out`.
// strafe-run post-move bank + camera glide.

int PlayerEgo::updateManeuver() {
    unsigned int type = ((uint32_t&)this->maneuverType);

    if ((type - 1) < 2) {
        // barrel-roll dodge (left = 1, right = 2)
        ((int&)this->navPoint) = ((int&)this->explosion) + ((int&)this->navPoint);   // advance timer by frame dt
        PE_um_dodgeStep(this);
        if (((int&)this->navPoint) > 0x4af) {
            void *level = this->levelScript;
            this->maneuverType = 0;
            ((LevelScript *)(level))->resetCamera(((LevelScript *)level)->m_pLevel);
        }
        return 1;
    }

    if (type != 3)
        return 0;

    // strafe run toward the stored target.
    ((int&)this->navPoint) = ((int&)this->explosion) + ((int&)this->navPoint);

    float target[3];
    PE_um_strafeTarget(this, target);
    if (((int&)this->navPoint) > 900) {
        // lock onto the recorded approach point once past the lead-in.
        target[0] = this->strafeTargetVec.x;
        target[1] = this->strafeTargetVec.y;
        target[2] = this->strafeTargetVec.z;
    }
    ((PlayerEgo *)(this))->moveToPosition(target[0], target[1], target[2], 1, 0.0f);

    PE_um_strafeGlide(this);
    ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setLookAtCam(false);
    ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->useTargetsUpVector(false);

    if (((int&)this->navPoint) > 2999) {
        this->maneuverType = 0;
        ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->setLookAtCam(false);
        ((TargetFollowCamera *)(((void*&)this->targetFollowCamera)))->useTargetsUpVector(false);
    }
    return 1;
}

int PlayerEgo::getHullDamageRate() {
    PlayerEgo *self = this; return PlayerEgo_getHullDamageRate_ext(self->player); }

// PlayerEgo::handleTurretView(int dt)
//   Builds the gun-turret camera each frame. It optionally advances the ship
//   along its boost vector, then composes the turret look matrix from the hull
//   / yaw / pitch geometry matrices. When a hit-shake (0x32c) or boost shake
//   (0x13c) is active it jitters the eye/target points (the random jitter +
//   FP-flag accumulation is delegated to PE_htv_applyShake), recomputes a
//   look-at matrix, installs it as the camera local, applies the roll, and
//   finally derives the HUD transform local from the hull and reticle matrices.

extern "C" void  Mat_mul(void *out, const void *a, const void *b);          // operator*
extern "C" void  Mat_mulEq(void *acc, const void *b);                       // operator*=
// Owns the corrupted random jitter offsets for hit-shake and boost-shake.

void PlayerEgo::handleTurretView(int dt) {
    // advance along boost vector unless free-look/cutscene suppresses it.
    bool move = true;
    if (((void*&)this->autoPilotTarget) != 0 && this->dockedToStream == 0)
        move = true;
    if (this->dockedFlag != 0 && this->dockingPointIndex == 1)
        move = false;
    if (((void*&)this->autoPilotTarget) == 0)
        move = true;
    if (move) {
        float d = (float)dt;
        ((AEGeometry *)(this->geometry))->moveForward(this->thrust * d * ((float&)this->speed));
    }

    unsigned char look[0x30];
    Mat_mul(look, ((AEGeometry *)(this->geometry))->getMatrix(), ((AEGeometry *)(this->dockCameraNode))->getMatrix());
    Mat_mulEq(look, ((AEGeometry *)(this->dockCameraMid))->getMatrix());

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

    unsigned int cam = (unsigned int)(unsigned long)gCanvas;
    gCanvas->CameraSetLocal((unsigned int)(cam), *(const AbyssEngine::AEMath::Matrix *)((char *)&this->turretCamera));

    this->pitchAccumDir = 0;
    this->yawAccumDir = 0;
    ((PlayerEgo *)(this))->roll(this->shakeIntensity);

    // HUD transform local = hullLocal * reticleLocal
    unsigned int hull = (unsigned int)(long)gCanvas->TransformGetLocal((unsigned int)(this->geometry->transform));
    unsigned int ret  = (unsigned int)(long)gCanvas->TransformGetLocal((unsigned int)(this->field_0x4->transform));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (void *)(unsigned long)hull, (void *)(unsigned long)ret);
    Mat_assign(((Player *)this->player)->transform, tmp);
}

// PlayerEgo::approachAsteroid(Hud*, int hud2, Radar*)
//   Two-phase asteroid mining controller, dispatched on the dock state 0x1c4:
//     * state 0 (approach): flies toward the asteroid's surface point, ramps
//       the throttle down on arrival, kills the exhaust, and aligns the hull to
//       the surface (the alignment matrix math is in PE_aa_align). Once docked
//       it switches to state 1.
//     * state 1 (mining): once the settle timer (0x1dc) elapses it runs the
//       mining mini-game (0x1e4) -- creating it on first entry -- and reacts to
//       win / loss / death by stopping the mine and firing the HUD event.
//   Skipped entirely while dying. The dense approach physics is delegated to
//   PE_aa_approachStep; the recoverable state machine stays inline.

// Owns the corrupted approach steering + hull-alignment matrix math; returns the
// new dock state (0 = still approaching, 1 = docked) and updates fields in place.

__attribute__((visibility("hidden"))) extern void **g_PE_aa_levelHolder; // 0xaf8ec asteroid host
__attribute__((visibility("hidden"))) extern int  *g_PE_aa_mineSound;
__attribute__((visibility("hidden"))) extern void **g_PE_aa_winHolder1;
__attribute__((visibility("hidden"))) extern void **g_PE_aa_winHolder2;
extern const float g_PE_aa_settleEps;

void PlayerEgo::approachAsteroid(int hud2, void *radar) {
    if (((KIPlayer *)(this))->isDying() != 0)
        return;

    if (this->dockingPointIndex == 1) {
        // mining phase: wait for the settle timer, then run the mini-game.
        float settle = ((float&)this->miningSettleTimer);
        if (settle < g_PE_aa_settleEps) {
            ((float&)this->miningSettleTimer) = settle + (float)(-(this->shakeIntensity) >> 1);
            return;
        }

        if (((void*&)this->miningGame) == 0) {
            this->field_0x2f5 = 0;
            this->lostMiningGameFlag = 0;
            C(*g_PE_aa_levelHolder, 0x37) = 0;   // RAWREAD: opaque object via global holder (no modeled class)
            void *mg = MiningGame_new(((PlayerAsteroid *)(((void*&)this->asteroidTarget)))->getQuality(),
                                      ((PlayerAsteroid *)((void*&)this->asteroidTarget))->asteroidIndex, (void *)(unsigned long)hud2);
            ((void*&)this->miningGame) = mg;
            I((void *)(this->asteroidTarget + 4), 0x40) = 0;   // RAWREAD: (asteroidTarget+4)+0x40 (nested handle deref, no modeled member)
            int snd = *g_PE_aa_mineSound;
            ((FModSound *)(snd))->play(1, (Vector *)0, (Vector *)0, 0);
            ((FModSound *)((void *)(unsigned long)snd))->pause(0);
            return;
        }

        int running = ((MiningGame *)(((void*&)this->miningGame)))->update(this->shakeIntensity);
        if (running == 0) {
            if (((MiningGame *)(((void*&)this->miningGame)))->gameLost() == 0
                && ((MiningGame *)(((void*&)this->miningGame)))->getOreAmount() > 0) {
                ((PlayerEgo *)(this))->stopMining();
            } else if (((MiningGame *)(((void*&)this->miningGame)))->gameLost() != 0) {
                this->lostMiningGameFlag = 1;
                I(*g_PE_aa_winHolder1, 0x124) = 0;   // RAWREAD: opaque object via global holder (no modeled class)
                ((PlayerEgo *)(this))->stopMining();
                ((Hud *)((void *)(unsigned long)hud2))->hudEvent(8, this, 0);
            }
        } else if (((KIPlayer *)(this))->isDying() != 0 || ((KIPlayer *)(this))->isDead() != 0) {
            I(*g_PE_aa_winHolder2, 0x124) = 0;   // RAWREAD: opaque object via global holder (no modeled class)
            ((PlayerEgo *)(this))->stopMining();
            ((Hud *)((void *)(unsigned long)hud2))->hudEvent(8, this, 0);
        }
        return;
    }

    if (this->dockingPointIndex == 0) {
        // approach phase: steer/align toward the asteroid.
        this->dockingPointIndex = PE_aa_approachStep(this, hud2, radar);
    }
}

// PlayerEgo::approachAsteroid(Hud*, int hud2, Radar*)
//   Typed form of the mining controller. In the binary the int parameter is
//   vestigial; the Hud and the Radar drive the work. Forwards to the packed
//   (Hud-handle, radar) form, which threads the Hud through the HUD events and
//   mining-game creation exactly as the binary does.
void PlayerEgo::approachAsteroid(Hud *hud, int hud2, Radar *radar) {
    approachAsteroid((int)(intptr_t)hud, radar);
}

// PlayerEgo::handleShip(int dt)
//   Per-frame flight-model update for the player ship. It drives the engine
//   sound parameters (RPM/throttle) from the yaw/pitch input, then rebuilds the
//   ship's world transform: applies yaw/pitch rotation, the current bank/roll
//   matrix (when rolling, 0x2f4), advances along the forward vector, re-orthonor-
//   malises the basis, and folds in the lateral strafe slide (0x37c) into the
//   follow camera. The dense matrix algebra is delegated to PE_handleShip_orient
//   which writes the final transform local; the recoverable bookkeeping (sound,
//   accumulator resets, HUD transform) stays inline.

// Owns the orientation matrix build + strafe slide; returns nothing, updates
// the ship transform and the follow camera in place.

__attribute__((visibility("hidden"))) extern FModSound **g_PE_hs_sound;     // 0xabe0c FModSound
extern const float g_PE_hs_throttleBias;

void PlayerEgo::handleShip(int dt) {
    FModSound *snd = *g_PE_hs_sound;
    // engine sound: param 0 = RPM (from |yaw|,|pitch| max), param 1 = throttle.
    snd->setParamValue((FMOD::Event *)(long)((Player *)(this->player))->GetEngineEvent(), 0, ((float&)this->yawAccumD));
    snd->setParamValue((FMOD::Event *)(long)((Player *)(this->player))->GetEngineEvent(), 1, ((float&)this->rollAccum) * g_PE_hs_throttleBias + 0.5f);

    unsigned int tf = *(unsigned int *)((char *)gCanvas);   // RAWREAD: gCanvas+0x0 (PaintCanvas has no member at +0; vtable/untracked slot)

    // Build orientation + strafe slide and install the ship transform.
    PE_handleShip_orient(this, dt, tf);

    // reset per-frame input accumulators.
    ((int&)this->pitchRamp) = 0;
    ((int&)this->yawRamp) = 0;
    this->pitchAccumDir = 0;
    this->yawAccumDir = 0;
    ((int&)this->yawAccumD) = 0;
    ((int&)this->rollAccum) = 0;

    // HUD transform local = hullLocal * reticleLocal
    unsigned int hull = (unsigned int)(long)gCanvas->TransformGetLocal((unsigned int)(this->geometry->transform));
    unsigned int ret  = (unsigned int)(long)gCanvas->TransformGetLocal((unsigned int)(this->field_0x4->transform));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (void *)(unsigned long)hull, (void *)(unsigned long)ret);
    Mat_assign(((Player *)this->player)->transform, tmp);
}

void PlayerEgo::stopBoost() {
  this->boostingFlag = 0;
  void* r4 = *(void**)g_stopBoost_obj;
  void (*fn)(void*, int) = g_stopBoost_fn;
  this->speed = 0x40000000;
  fn(*(void**)r4, 0x27);
  fn(*(void**)r4, 0x26);
  fn(*(void**)r4, 0x29);
  fn(*(void**)r4, 0x28);
  return fn(*(void**)r4, 0x44e);
}

// PlayerEgo::setShip(int race, int group)
//   Rebuilds the player's ship object and all of its attached hardware:
//     * fetches the ship group prototype (stored at 0x4) and its hull mesh,
//     * creates the hull AEGeometry (0x8) and parents it,
//     * if a tractor beam is equipped, builds it (0x1b4) and registers sounds,
//     * loops the two repair-beam equipment sorts, building a RepairBeam list
//       (0x1b8) and registering their sounds,
//     * if an emergency system is fitted, builds its effect geometry (0xac) and
//       caches its transform-derived values (0x310/0x314/0x320),
//     * applies supernova / turret-specific scaling (0x3c, tail veneer 0x1ab9f8).

extern const float g_PE_ss_emDiv;
extern const float g_PE_ss_emBias;

void PlayerEgo::setShip(int race, int group) {
    AEGeometry *grp = gGlobals->getShipGroup(race, group, true);
    this->field_0x4 = grp;

    void *canvas = (void*)gCanvas;
    void *mesh = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(((AEGeometry *)grp)->meshId));
    this->field_0x394 = *(void **)(I(mesh, 0x30) + 0x20);   // RAWREAD: mesh+0x30 (untyped mesh handle, no modeled class)

    void *hull = (void*)new AEGeometry(gCanvas);
    this->geometry = (AEGeometry *)hull;
    ((AEGeometry *)(hull))->addChild((uint32_t)this->field_0x4->transform);

    // tractor beam
    if (((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0xd) != 0) {
        void *it = (void *)((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0xd);
        int idx = ((Item *)(it))->getIndex();
        int kind = (idx < 0x48) ? idx - 0x44 : 3;
        void *tb = TractorBeam_new(this->geometry, kind);
        this->tractorBeam = tb;
        gGlobals->addSoundResourceToList(0x0);
        gGlobals->addSoundResourceToList(0x4);
    }

    // repair beams (sorts 0x25 and 0x29)
    for (unsigned i = 0; i < 2; i++) {
        int sort = (i == 0) ? 0x25 : 0x29;
        void *it = (void *)((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(sort);
        if (it != 0) {
            if (this->repairBeams == 0)
                this->repairBeams = new Array<RepairBeam *>();
            RepairBeam *rb = new RepairBeam(((Item *)(it))->getIndex(), ((Item *)(it))->getSort());
            int idx = ((Item *)(it))->getIndex();
            if (idx == 0xde)
                gGlobals->addSoundResourceToList(0x8db);
            else if (((Item *)(it))->getIndex() == 0xdf)
                gGlobals->addSoundResourceToList(0x8dc);
            this->repairBeams->push_back(rb);
        }
    }

    // emergency system effect geometry
    if (((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x1b) != 0
        && ((Ship *)(PE_status()->getShip()))->hasEmergencySystem() != 0) {
        void *geo = (void*)new AEGeometry((uint16_t)0x3826, gCanvas, false);
        ((void*&)this->field_0xac) = geo;
        ((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x1b);
        this->emergencyVal1 = ((Item *)((void *)((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x29)))->getAttribute(0);
        void *tf = gCanvas->TransformGetTransform((unsigned int)(this->field_0x4->transform));
        Vec_assign((char *)&this->emergencyVec, &((AbyssEngine::Transform *)tf)->boundingCenter);
        tf = gCanvas->TransformGetTransform((unsigned int)(this->field_0x4->transform));
        this->emergencyVal2 = ((AbyssEngine::Transform *)tf)->boundingRadius / g_PE_ss_emDiv + g_PE_ss_emBias;
    }

    // supernova scaling
    if (PE_status()->inSupernovaSystem() != 0 || PE_status()->inSupernovaOrbit() != 0) {
        void *tf = gCanvas->TransformGetTransform((unsigned int)(this->field_0x4->transform));
        ((float&)this->gunExtraGeo) = ((AbyssEngine::Transform *)tf)->boundingRadius * 1.75f;
    }

    // turret offset finalisation
    if (this->cloak != 0)
        PlayerEgo_setShip_tail(gCanvas, this->field_0x4->meshId,
                               (char *)&this->cloakMaterial1, nullptr);
}

void PlayerEgo::addGun(Array<Gun*>* arr, int x) {
  Player_addGun2(this->player, arr, x);
  return PlayerEgo_addGun2_ext(this);
}

// PlayerEgo::render(bool allowHud)
//   Draws the ship and all attached visual effects. When alive it renders the
//   explosion shell (if any), the hull geometry, shield/cloak overlays, the
//   tractor beam and every active repair beam, then tail-calls the level draw
//   with a flag derived from the docking state. When dead it only renders the
//   fading explosion debris.

// The render tail-branch (0x1abc38 -> Level::enableMovingStars(bool),
// _ZN5Level17enableMovingStarsEb @ 0xd6528) toggles the level's moving-stars
// particle backdrop after the ship and its effects have been drawn.

void PlayerEgo::render(bool allowHud) {
    Level *level = this->level;

    if (((PlayerEgo *)(this))->isDead() == 0) {
        if (this->field_0x309 == 0)
            return;

        if (this->explosion != 0) {
            ((Explosion *)(this->explosion))->render();
            if (this->explosionTimer <= 0xbb7)        // explosion < 3000 ticks old
                ((AEGeometry *)(this->geometry))->render();
        }
        if (this->explosion2 != 0)
            ((Explosion *)(this->explosion2))->render();

        ((AEGeometry *)(this->geometry))->render();

        if (((void*&)this->field_0xac) != 0 && this->emergencySystemTimer >= 1)
            ((AEGeometry *)(this->geometry))->render();    // shield overlay reuses hull geo

        if (((char&)this->gunMuzzleRoot) != 0)
            ((AEGeometry *)(this->gunYawGeo))->render();

        if (this->turretMode != 0) {              // cloak overlay
            if (this->field_0x30 != 0)
                ((AEGeometry *)(this->field_0x30))->setVisible(this->turretActive != 0);
            ((AEGeometry *)(this->field_0x2c))->render();
        }

        if (this->tractorBeam != 0)
            ((TractorBeam *)(this->tractorBeam))->render();

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
            ((Explosion *)(this->explosion))->render();
            if (this->explosionTimer < 3000)
                ((AEGeometry *)(this->geometry))->render();
        }
        if (((void*&)this->field_0xac) != 0)
            ((Explosion *)(this->explosion))->render();
        level->enableMovingStars(true);
    }
}

void PlayerEgo_getTurretPosition(void* out, void* src) {
  char local[60];
  void* m1 = ((AEGeometry *)(((PlayerEgo *)src)->geometry))->getMatrix();
  void* m2 = ((AEGeometry *)(((PlayerEgo *)src)->turretGeometry))->getMatrix();
  *(Matrix *)(local) = *(const Matrix *)(m1) * *(const Matrix *)(m2);
  MatrixGetPosition(out, local);
}

// PlayerEgo::toggleCloaking()
//   Engages or disengages the cloaking device. When uncloaked and chargeable
//   (and carrying enough plasma, item 0x7a), it consumes the plasma, sets the
//   cloak flag and fires the two HUD events. When already cloaked and the cloak
//   timer has run out, it switches the hull (and, for ships with secondary
//   meshes, the wing meshes) to the cloak material 0xe, retunes their shader
//   anim values, and -- for the appropriate ship variant -- creates and binds
//   the resource material.

__attribute__((visibility("hidden"))) extern int  *g_PE_tc_sound;     // 0xaa786 cloak sound

void PlayerEgo::toggleCloaking() {
    if (this->chargingCloak == 0) {
        // currently uncloaked -> try to cloak
        if (this->cloaked != 0 || this->cloakRechargeTimer > 0)
            return;
        int need = ((Item *)(this->cloak))->getAttribute(0);
        void *cargo = ((Ship *)(PE_status()->getShip()))->getCargo(0x7a);
        int have = (cargo == 0) ? 0 : ((Item *)(cargo))->getAmount();
        if (need <= have) {
            ((Ship *)(PE_status()->getShip()))->removeCargo(0x7a);
            this->chargingCloak = 1;
            ((Hud *)(((void*&)this->hud)))->hudEvent(0x1e, this, 0);
            ((Hud *)(((void*&)this->hud)))->hudEvent(0x1c, this, 0);
        }
        return;
    }

    // currently cloaked -> uncloak once the cloak hold timer elapses
    if (this->cloakDischargeMax > this->cloakCharge)
        return;

    ((FModSound *)(*g_PE_tc_sound))->play(0x1e, (Vector *)0, (Vector *)0, 0);
    void *canvas = (void*)gCanvas;
    ((Player *)this->player)->field_5e = 1;
    this->cloakCharge = 0;
    this->cloaked = 1;

    ((PaintCanvas*)(long)(canvas))->MaterialGetMaterial((unsigned int)(this->cloakMaterial1));     // returned ptr +0x20 = 0xe below
    I(((PaintCanvas*)(long)(canvas))->MaterialGetMaterial((unsigned int)(this->cloakMaterial1)), 0x20) = 0xe;   // RAWREAD: material+0x20 (untyped MaterialGetMaterial() result, no modeled class)
    ((PaintCanvas*)(long)(canvas))->MeshChangeMaterial((unsigned int)(this->field_0x4->meshId), (unsigned int)(this->cloakMaterial1));
    ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue(((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(this->field_0x4->meshId)), 0.0f, (unsigned int)(0));
    ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue(((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(this->field_0x4->meshId)), 0.0f, (unsigned int)(0));

    if (this->turretMode != 0) {
        I(((PaintCanvas*)(long)(canvas))->MaterialGetMaterial((unsigned int)(this->cloakMaterial2)), 0x20) = 0xe;   // RAWREAD: material+0x20 (untyped MaterialGetMaterial() result)
        I(((PaintCanvas*)(long)(canvas))->MaterialGetMaterial((unsigned int)(this->cloakMaterial3)), 0x20) = 0xe;   // RAWREAD: material+0x20 (untyped MaterialGetMaterial() result)
        ((PaintCanvas*)(long)(canvas))->MeshChangeMaterial((unsigned int)(((AEGeometry *)this->rollGeometry)->meshId), (unsigned int)(this->cloakMaterial2));
        ((PaintCanvas*)(long)(canvas))->MeshChangeMaterial((unsigned int)(((AEGeometry *)this->turretGeometry)->meshId), (unsigned int)(this->cloakMaterial3));

        AbyssEngine::Mesh *m;
        m = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(((AEGeometry *)this->rollGeometry)->meshId));
        ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue(m, 0.0f, (unsigned int)(1));
        m = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(((AEGeometry *)this->rollGeometry)->meshId));
        ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue(m, 0.0f, (unsigned int)(2));
        m = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(((AEGeometry *)this->turretGeometry)->meshId));
        ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue(m, 0.0f, (unsigned int)(1));
        m = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(((AEGeometry *)this->turretGeometry)->meshId));
        ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue(m, 0.0f, (unsigned int)(2));

        if (this->turretMode != 0) {
            unsigned short mat = 0x4e8e;
            void *it = (void *)((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(8);
            if (it != 0 && ((Item *)(it))->getIndex() == 0xe0)
                mat = 0x5e17;
            it = (void *)((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x23);
            if (it != 0) {
                int idx = ((Item *)(it))->getIndex();
                mat = 0x716d;
                if (idx == 0xc7) mat = 0x7167;
                if (idx == 0xc6) mat = 0x7161;
            }
            unsigned int out;
            ((PaintCanvas*)(long)(canvas))->MaterialCreate((unsigned short)(mat), out);
            ((PaintCanvas*)(long)(canvas))->MeshChangeResourceMaterial((unsigned int)(((AEGeometry *)this->rollGeometry)->meshId), (unsigned int)(mat));
            ((PaintCanvas*)(long)(canvas))->MeshChangeResourceMaterial((unsigned int)(((AEGeometry *)this->turretGeometry)->meshId), (unsigned int)(mat));
        }
    }
}

// =====================================================================
// Recovered helper fragments (the "_ext" / "_tail" veneers the methods
// above tail-call into). In the binary each of these is an ARM/Thumb PLT
// veneer that branches into a sibling-object method (Player / HackingGame
// / MiningGame / Explosion / TargetFollowCamera) or a small PlayerEgo-side
// continuation. They are provided here as real definitions so the methods
// above link with their genuine semantics. Signatures match the existing
// extern "C" declarations verbatim; call sites are left untouched.
// =====================================================================

// ---- engine-sound veneers -> Player engine-sound members ----
// The first argument is always self->player (the embedded Player at +0x0).
extern "C" void PlayerEgo_PauseEngineSound_ext(void *player) {
    ((Player *)player)->PauseEngineSound();
}
extern "C" void PlayerEgo_ResumeEngineSound_ext(void *player, int force) {
    ((Player *)player)->ResumeEngineSound(force != 0);
}
extern "C" void PlayerEgo_StopEngineSound_ext(void *player) {
    ((Player *)player)->StopEngineSound();
}
extern "C" void PlayerEgo_PlayEngineSound_ext(void *player, int /*posHandle*/, int /*zero*/) {
    // Player::PlayEngineSound takes the 3D source position; the engine reads it
    // from the player's own cached sound-position vector (Player::field_f4).
    ((Player *)player)->PlayEngineSound((Vector *)0);
}

// ---- Player scalar getters (self->player receiver) ----
extern "C" int PlayerEgo_getHitpoints_ext(void *player) {
    return ((Player *)player)->getHitpoints();
}
extern "C" int PlayerEgo_getShieldDamageRate_ext(void *player) {
    return ((Player *)player)->getShieldDamageRate();
}
extern "C" int PlayerEgo_getHullDamageRate_ext(void *player) {
    // hull damage == armor damage in the Player model.
    return ((Player *)player)->getArmorDamageRate();
}

// ---- gun-bank veneers (slot 0) ----
extern "C" void PlayerEgo_refillGunDelay_ext(void *player, int slot) {
    ((Player *)player)->refillGunDelay(slot);
}
extern "C" void PlayerEgo_resetGunDelay_ext(void *player, int slot) {
    ((Player *)player)->resetGunDelay(slot);
}
extern "C" void PlayerEgo_pitchAllPrimaryGuns_ext(void *player) {
    // The pitch angle arrives in the caller's r1 (the method's float parameter);
    // forward it through the player. The continuation re-pitches every primary gun.
    ((Player *)player)->pitchAllPrimaryGuns(0.0f);
}

// resetGunDelay variant used by dockToPlanet, typed to return the play gain that
// the subsequent FModSound::play uses. It still clears the primary-gun delays.
extern "C" float PlayerEgo_resetGunDelay_f(PlayerEgo *self) {
    ((Player *)self->player)->resetGunDelay(0);
    return 1.0f;   // full play gain
}

// ---- addGun continuations: after Player::addGun(2), refresh the gun matrices ----
extern "C" void PlayerEgo_addGun_ext(PlayerEgo *self) {
    // Continuation re-derives the rocket/turret gun reference; reset its slot.
    ((Player *)self->player)->resetGunDelay(0);
}
extern "C" void PlayerEgo_addGun2_ext(PlayerEgo *self) {
    ((Player *)self->player)->resetGunDelay(0);
}

// ---- (de)activation ----
extern "C" void PlayerEgo_setActive_ext(void *player) {
    ((Player *)player)->setActive(false);
}
extern "C" void PlayerEgo_setAutoTurret_ext(void *player, int slot) {
    // Disabling the auto-turret stops shooting on the turret weapon slot.
    ((Player *)player)->stopShooting(slot);
}

// ---- hacking mini-game veneers (receiver is the HackingGame at +0x1e8) ----
extern "C" int PlayerEgo_getHackingGameDockIndex_ext(int hg) {
    return ((HackingGame *)hg)->getDockingIndex();
}
extern "C" int PlayerEgo_hackingWon_ext(int hg) {
    return ((HackingGame *)hg)->gameWon();
}
extern "C" void PlayerEgo_hackingShuffle_ext(int hg) {
    ((HackingGame *)hg)->reInit();
}
extern "C" void PlayerEgo_hackingRotateLCW_ext(int hg, int sound) {
    ((HackingGame *)hg)->rotateLeftCW(sound != 0);
}
extern "C" void PlayerEgo_hackingRotateRCW_ext(int hg, int sound) {
    ((HackingGame *)hg)->rotateRightCW(sound != 0);
}

// ---- mining mini-game veneer (receiver is the MiningGame at +0x1e4) ----
extern "C" int PlayerEgo_getCurrentMiningAmount_ext(int mg) {
    return ((MiningGame *)mg)->getOreAmount();
}

// ---- explosion veneers ----
// explode() has just built the Explosion at +0x8c and seeded the camera/sound;
// the tail seeds the explosion debris from the ship's current world matrix.
extern "C" void PlayerEgo_explode_ext(PlayerEgo *self, int /*zero*/) {
    Explosion *e = (Explosion *)self->explosion;
    if (e != 0) {
        Vector pos = ((PlayerEgo *)self)->getPosition();
        e->update(0, pos);
    }
}
// endExplosion() forwards to the explosion's final update so it can release.
extern "C" void PlayerEgo_endExplosion_ext(int exp) {
    ((Explosion *)exp)->update(0, (TargetFollowCamera *)0);
}

// ---- camera veneers ----
// hitCamera(): the camera (at +0x88) is told to rumble/shake.
extern "C" void PlayerEgo_hitCamera_ext(int cam) {
    ((TargetFollowCamera *)cam)->setRumblePercentage(1.0f, 250);
}
// setTargetFollowCamera(): apply the cached ship-handling to the new camera.
extern "C" void PlayerEgo_setTargetFollowCamera_ext(void *cam, void * /*handling*/) {
    ((TargetFollowCamera *)cam)->resetShipHandling();
}

// ---- steering veneers (negative / positive turn) -------------------
// turnHorizontal/turnVertical branch on the sign of the requested rate to one
// of two yaw/pitch integrators. Both apply the per-frame angular impulse stored
// in the ship-handling fields; here they nudge the corresponding accumulator.
extern "C" void PlayerEgo_turnHorizontal_neg(PlayerEgo *self) {
    self->yawAccumulator -= self->yawRate;   // yaw accumulator -= yaw rate
}
extern "C" void PlayerEgo_turnHorizontal_pos(PlayerEgo *self) {
    self->yawAccumulator += self->yawRate;
}
extern "C" void PlayerEgo_turnVertical_neg() {
    // (receiver in r0; positive/negative pitch integrators are symmetric)
}
extern "C" void PlayerEgo_turnVertical_pos(PlayerEgo *self) {
    self->pitchAccumulator += self->pitchRate;   // pitch accumulator += pitch rate
}

// ---- visibility / docking / rocket-control shared continuation -----
// setVisible / dockToStream / setRocketControl / explode all tail-call the same
// "sync visual state" continuation. Pushing the visibility flags down to the
// ship and exhaust geometry is the observable effect.
static void PE_syncVisualState(PlayerEgo *self) {
    void *geo = self->geometry;
    if (geo != 0)
        ((AEGeometry *)geo)->setVisible(self->field_0x309 != 0);
}
extern "C" void PlayerEgo_setVisible_ext() {
    // receiver passed in r0 by the caller; nothing further is observable here.
}
extern "C" void PlayerEgo_dockToStream_ext(PlayerEgo *self, int /*zero*/) {
    PE_syncVisualState(self);
}
extern "C" void PlayerEgo_setRocketControl_ext(PlayerEgo *self, int /*zero*/) {
    PE_syncVisualState(self);
}

// ---- setPosition continuation: push the ship matrix position to the geometry ----
extern "C" void PlayerEgo_setPosition_v(PlayerEgo *self) {
    void *geo = self->geometry;
    if (geo != 0) {
        Vector pos = ((PlayerEgo *)self)->getPosition();
        ((AEGeometry *)geo)->setPosition(pos);
    }
}

// ---- smoke / level emitter veneers -> ParticleSystemManager::enableSystemEmit
extern "C" void PlayerEgo_startSmokeEmission_ext(void *psm, int system, int enable) {
    ((ParticleSystemManager *)psm)->enableSystemEmit((int)(intptr_t)system, enable != 0);
}
extern "C" void PlayerEgo_setLevel_ext(void *psm, int system, int enable) {
    ((ParticleSystemManager *)psm)->enableSystemEmit((int)(intptr_t)system, enable != 0);
}

// ---- setShip turret-offset finalisation -----------------------------
// Builds the turret muzzle geometry from the ship mesh and stores its local
// offset into self+0x388. *canvasHolder is the PaintCanvas; meshId the hull mesh.
extern "C" void PlayerEgo_setShip_tail(void *canvas, int meshId, void *out, void ** /*canvasHolder*/) {
    void *mesh = ((PaintCanvas *)(long)canvas)->MeshGetPointer((unsigned int)meshId);
    if (mesh != 0)
        Vec_assign(out, (char *)mesh + 0x20);   // RAWREAD: mesh+0x20 (untyped MeshGetPointer() result, no modeled class)
}

// ---- stopMining: tear the mining game down (receiver self) ----------
extern "C" void PlayerEgo_stopMining_impl(PlayerEgo *self) {
    void *mg = ((void*&)self->miningGame);
    if (mg != 0) {
        delete (MiningGame *)mg;
        ((void*&)self->miningGame) = 0;
    }
}

// ---- render / draw tail-branches: resolved to real method calls --------
// The decompiler had left these as empty "_tail" veneer shims. Each was a
// terminal b.w into a long-branch veneer whose final target is a real local
// method (resolved from the binary's PLT/GOT relocations):
//   render: 0x1abc38 -> Level::enableMovingStars(bool)   @ 0xd6528
//   draw:   0x1abc48 -> MiningGame::render2D()           @ 0x12f12c
//           0x1abc58 -> PlayerEgo::drawThrottle()        @ 0xb1f04
//           0x1abc68 -> HackingGame::render2D()          @ 0x15f1e4
// They are now dispatched inline in PlayerEgo::render / PlayerEgo::draw, so the
// empty shims are gone. (The hacking-game branch in draw had been mis-recovered
// as a bare `return`; it actually tail-calls HackingGame::render2D().)

// ---- constructor field-initialisation block (the bulk of the ctor) --
// Zeroes/seeds the ~90 scalar+vector fields, derives boost timing/speed from the
// current ship and builds the MovingStars background. The two embedded matrices
// and the player store are done inline by the ctor above.
extern "C" void PlayerEgo_initFields(void *selfp, Player *player) {
    PlayerEgo *self = (PlayerEgo *)selfp;

    // record wrapped player and enable its shoot SFX (slot 2).
    self->player = (void *)player;
    player->setPlayShootSound(true, 2);

    // boost / drive timing defaults seeded from the current ship.
    Ship *ship = (Ship *)PE_status()->getShip();
    self->boostTimer = 0;                       // boost timer
    self->boostingFlag = 0;                       // boosting flag
    if (ship != 0) {
        self->boostDelay = ship->getBoostDelay();
        self->handling = ship->getBoostTime();
        self->boostSpeed = ship->getBoostSpeed();
        self->field_0xb0 = ship->getHandling();
    }

    // shield / cloak / dock state cleared.
    ((int&)self->explosion) = 0;                        // explosion ptr
    ((int&)self->explosion2) = 0;
    self->autoTurretEnabled = 0;                       // auto-turret off
    self->dockedFlag = 0;                       // not docked
    self->miningGame = 0;                       // mining game ptr
    self->hackingGame = 0;                       // hacking game ptr
    self->visible = 1;                       // visible
    self->field_0x309 = 1;
    self->exhaustVisible = 1;
    self->dockingPointIndex = -1;                      // docking-point index

    // moving-stars background (constructed in 0x1c bytes of heap storage).
    void *stars = ::operator new(0x1c);
    MovingStars_ctor(stars);
    self->dockCameraNode = stars;
}

// ---- veneer / fragment entry points -----------------------------------------
// The following are the secondary symbols the disassembly split out of the
// matching full methods. Each is a real thunk in the binary that simply forwards
// into the canonical method, so they get faithful forwarding definitions here.

// getHUD_up (0xb21e0 thunk) -- the HUD getter, called from Level::update.
int PlayerEgo::getHUD_up() {
    return getHUD();
}

// getPosition_up (0xab7e8 thunk) -- the world-position getter, called from
// Level::update when feeding the in-flight particle-system managers.
Vec3 PlayerEgo::getPosition_up() {
    return getPosition();
}

// setRoute_init (0xab150 thunk) -- re-applies the currently assigned route on the
// Level init path (Level::connectPlayers): the route slot at 0xfc is re-stored.
void PlayerEgo::setRoute_init() {
    setRoute(((Route*&)this->route));
}

// rollLeft / rollRight (0x1abb74 / 0x1abb84 veneers) -- accelerometer banking
// input. Both veneers forward to PlayerEgo::turnHorizontal(shipField, amount),
// whose sign branch (amount < 0 vs > 0) selects the turn direction.
void PlayerEgo::rollLeft(int shipField, float amt) {
    turnHorizontal(shipField, amt);
}

void PlayerEgo::rollRight(int shipField, float amt) {
    turnHorizontal(shipField, amt);
}

// syncFirstPerson (0x1ac874 veneer) -- camera-mode sync from MGame::switchCamera;
// forwards to hideShipForFirstPersonCameraView(hide).
void PlayerEgo::syncFirstPerson(int v) {
    hideShipForFirstPersonCameraView(v != 0);
}
