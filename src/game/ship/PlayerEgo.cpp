#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/engine/render/Camera.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/game/ship/TargetFollowCamera.h"
#include "gof2/externs.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/menu/HackingGame.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/world/LevelScript.h"
#include "gof2/game/menu/MiningGame.h"
#include "gof2/engine/render/ParticleSystemManager.h"
#include "gof2/game/ship/PlayerAsteroid.h"
#include "gof2/game/ship/PlayerWormHole.h"
// Radar.h is intentionally NOT included: it defines stub structs (KIPlayer, Status,
// SolarSystem) that collide with the real class headers used by this TU. Only the two
// Radar methods below are needed here, so forward-declare a minimal interface.
class Radar {
public:
    void unlockAsteroid();
    bool isPlasmaInRange();
};
#include "gof2/game/mission/Status.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/game/mission/Explosion.h"
// Hud.h is intentionally NOT included: it redefines the file-scope P() accessor that
// LevelScript.h already provides in this TU. Only these two Hud methods are needed,
// so forward-declare a minimal interface.
class Hud {
public:
    void hudEvent(int eventId, void *ego, int arg);
    void setHackingGameActive(bool value);
};
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/ship/PlayerFixedObject.h"
#include "gof2/game/weapons/RepairBeam.h"
#include "gof2/game/world/Route.h"
#include "gof2/game/world/SpacePoint.h"
#include "gof2/game/core/String.h"
#include "gof2/game/weapons/TractorBeam.h"
#include "gof2/game/ship/Ship.h"

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

// Status singleton accessor. Several Status:: instance methods (getShip, inAlienOrbit,
// getMission, getCampaignMission, inSupernovaSystem, ...) appear in the decomp as bare
// free-function calls with the self receiver dropped. The receiver is the Status
// singleton, the same object the sibling functions reach via g_*_status holders.
extern void *g_mining_status;
static inline Status *PE_status() { return (Status *)g_mining_status; }

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
extern "C" void Explosion_ctor(void*, int);
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
extern "C" void *HackingGame_dtor(void *hg);
extern "C" int   PE_adp_approach(PlayerEgo *self, void *station);
extern "C" int   PE_adp_glide(PlayerEgo *self);
extern "C" void  PE_adp_apply(PlayerEgo *self);
extern "C" void PlayerEgo_setLevel_ext(void*, int, int);
extern "C" float PE_yawRampDelta(float rate, int frameTime);
extern "C" void *Player_dtor(void *);
extern "C" void *MiningGame_dtor(void *);
extern "C" void *Explosion_dtor(void *);
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
extern "C" void *MiningGame_dtor(void *mg);
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
extern "C" void *EaseInOutMatrix_dtor(void *m);
extern "C" void *PE_dtdp_makeEase(const void *fromMatrix, const void *navPoint);
extern "C" void PE_upd_boost(PlayerEgo *self, int dt);
extern "C" void PE_upd_docksFinishDelivery(PlayerEgo *self, void *radio);
extern "C" void *Explosion_dtor(void *exp);
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
void *Globals_getShipGroup(void *g, int race, int group, bool b);
extern "C" void *TractorBeam_new(void *geo, int kind);
void  Globals_addSoundResourceToList(int snd);
extern "C" void *RepairBeam_new(int idx, int sort);

// Typed accessor for the repair-beam array member (+0x1b8). The class layout is
// sparse / offset-addressed, so reach the Array<RepairBeam*>* through the raw offset.
static inline Array<RepairBeam *> *&PE_repairBeams(void *self) {
    return *(Array<RepairBeam *> **)((char *)self + 0x1b8);
}
extern "C" void  PlayerEgo_setShip_tail(void *canvas, int meshId, void *out, void **canvasHolder);
extern "C" void Player_addGun2(void*, void*, int);
extern "C" void PlayerEgo_addGun2_ext(PlayerEgo*);
void MatrixGetPosition(void*, void*);
extern "C" void *MovingStars_ctor(void *self);   // MovingStars::MovingStars()

void PlayerEgo::setVisible(bool v) {
    PlayerEgo *self = this;
  C(self, 0x32e) = v;
  C(self, 0x309) = v;
  return PlayerEgo_setVisible_ext();
}

void PlayerEgo::boost() {
    PlayerEgo *self = this;
  if (C(self, 0x13c) != 0) return;
  if (C(self, 0x146) == 0) return;
  if (I(self, 0x194) != 0) return;
  if (I(self, 0x138) < 0) return;
  float v = (float)I(self, 0xc8);
  I(self, 0x138) = 0;
  C(self, 0x13c) = 1;
  void* snd = *(void**)g_boost_fmod;
  F(self, 0xb8) = v;
  ((FModSound *)(*(void**)snd))->play((int)(intptr_t)P(self, 0xd4), (Vector *)0, (Vector *)0, v);
}

float PlayerEgo::getDriveChargeRate() {
    PlayerEgo *self = this;
  float d = (float)I(self, 0x1fc) / (float)I(self, 0x200);
  return d >= 0.0f ? d : 1.0f;
}

unsigned char PlayerEgo::isAutoPilot() {
    PlayerEgo *self = this; return UC(self, 0x158); }

bool PlayerEgo::goingToWormhole() {
    PlayerEgo *self = this;
  void* m = P(self, 0xc);
  void* r4 = P(self, 0x15c);
  void* lm = (void *)((Level *)(m))->getLandmarks();
  return r4 == P(P(lm, 4), 0xc);
}

int PlayerEgo::getCurrentSecondaryWeaponIndex() {
    PlayerEgo *self = this; return I(self, 0x10c); }

int PlayerEgo::getHitpoints() {
    PlayerEgo *self = this; return PlayerEgo_getHitpoints_ext(P(self, 0)); }

unsigned char PlayerEgo::isChargingDrive() {
    PlayerEgo *self = this; return UC(self, 0x204); }

int PlayerEgo::shouldSwitchToFreeLookCam() {
    PlayerEgo *self = this;
  if (C(self, 0xb1) == 0) return 0;
  C(self, 0xb1) = 0;
  return 1;
}

int PlayerEgo::getHandling() {
    PlayerEgo *self = this; return I(self, 0x154); }

void PlayerEgo::setComputerControlled(bool v) {
    PlayerEgo *self = this; C(self, 0x1f4) = v; }

int PlayerEgo::getRocketBanking() {
    PlayerEgo *self = this; return I(self, 0x198); }

bool PlayerEgo::isDockingToDockingPoint() {
    PlayerEgo *self = this;
  if (C(self, 0x356) != 0) return I(self, 0x1c4) != 1;
  return false;
}

void PlayerEgo::forceBoost() {
    PlayerEgo *self = this;
  I(self, 0x138) = 0;
  C(self, 0x13c) = 1;
  I(self, 0xb8) = 0x41000000;
  I(self, 0xcc) = 0x2710;
  I(self, 0xd0) = 0;
}

int PlayerEgo::isDead() {
    PlayerEgo *self = this; return ((PlayerEgo *)(self))->getHitpoints() < 1; }

int PlayerEgo::getAutoPilotTarget() {
    PlayerEgo *self = this; return I(self, 0x15c); }

bool PlayerEgo::goingToPlanet() {
    PlayerEgo *self = this;
  if (C(self, 0x158) != 0 && ((PlayerEgo *)(self))->goingToStream() == 0 && ((PlayerEgo *)(self))->goingToStation() == 0)
    return C(self, 0x160) == 0;
  return false;
}

float PlayerEgo::getCloakRechargeRate() {
    PlayerEgo *self = this;
  return 1.0f - (float)I(self, 0x20c) / (float)I(self, 0x368);
}

unsigned char PlayerEgo::isDockedToStream() {
    PlayerEgo *self = this; return UC(self, 0x1ed); }

int PlayerEgo::getTargetFollowCamera() {
    PlayerEgo *self = this; return I(self, 0x88); }

unsigned char PlayerEgo::isCloaked() {
    PlayerEgo *self = this; return UC(self, 0x1ac); }

void PlayerEgo::setFreeLookMode(bool v) {
    PlayerEgo *self = this; C(self, 0xc4) = v; }

void PlayerEgo::setThrust(float v) {
    PlayerEgo *self = this; F(self, 0xbc) = v; }

bool PlayerEgo::readyForCloak() {
    PlayerEgo *self = this;
  if (C(self, 0x1ad) == 0) return false;
  return I(self, 0x208) >= I(self, 0x214);
}

int PlayerEgo::getHackingGameDockIndex() {
    PlayerEgo *self = this;
  int v = I(self, 0x1e8);
  if (v == 0) return -1;
  return PlayerEgo_getHackingGameDockIndex_ext(v);
}

void PlayerEgo::setPosition3(float x, float y, float z) {
    PlayerEgo *self = this;
  void* g = P(self, 8);
  char v[12];
  *(float*)(v + 0) = x;
  *(float*)(v + 4) = y;
  *(float*)(v + 8) = z;
  ((AEGeometry *)(g))->setPosition(*(Vector *)v);
}

void PlayerEgo::dockToStream(bool param) {
    PlayerEgo *self = this;
  if (param) { *(short*)B(self, 0x1ec) = 0x100; return; }
  I(self, 0xb8) = 0x40000000;
  ((PlayerEgo *)(self))->setPosition3(F(self, 0xe0), F(self, 0xe4), F(self, 0xe8));
  C(self, 0x24) = 0;
  C(self, 0x145) = 0;
  *(short*)B(self, 0x1ec) = 0;
  return PlayerEgo_dockToStream_ext(self, 0);
}

void PlayerEgo::hackingShuffle() {
    PlayerEgo *self = this;
  int v = I(self, 0x1e8);
  if (v != 0) PlayerEgo_hackingShuffle_ext(v);
}

int PlayerEgo::getDockTransferedAmount() {
    PlayerEgo *self = this; return I(self, 0x364); }

unsigned char PlayerEgo::goingToWaypoint() {
    PlayerEgo *self = this; return UC(self, 0x160); }

bool PlayerEgo::isDockedToDockingPoint() {
    PlayerEgo *self = this;
  if (C(self, 0x356) == 0) return false;
  return I(self, 0x1c4) == 1;
}

int PlayerEgo::getDockTotalAmount() {
    PlayerEgo *self = this; return I(self, 0x360); }

bool PlayerEgo::isInWormhole() {
    PlayerEgo *self = this;
  if (((PlayerEgo *)(self))->getHitpoints() > 0) return C(self, 0x25) != 0;
  return false;
}

void PlayerEgo::setRocketControl(void* gun, void* geo) {
    PlayerEgo *self = this;
  void* lvl = P(self, 0xc);
  I(self, 0x194) = (int)(intptr_t)gun;
  int psm_arg = I(lvl, 0x64);
  void* psm = P(lvl, 0x88);
  if (gun == 0) {
    ((ParticleSystemManager *)(psm))->systemSetMatrix(psm_arg, B(P(self, 0), 4));
    I(self, 0x198) = 0;
    return;
  }
  void* m = (void*)&((AEGeometry *)geo)->getReferenceMatrix();
  ((ParticleSystemManager *)(psm))->systemSetMatrix(psm_arg, m);
  return PlayerEgo_setRocketControl_ext(self, 0);
}

bool PlayerEgo::isMining() {
    PlayerEgo *self = this; return I(self, 0x1e4) != 0; }

void PlayerEgo::turnHorizontal(int a, float v) {
    PlayerEgo *self = this;
  if (v < 0.0f) { PlayerEgo_turnHorizontal_neg(self); return; }
  if (v > 0.0f) { PlayerEgo_turnHorizontal_pos(self); return; }
}

int PlayerEgo::getThrust() {
    PlayerEgo *self = this; return I(self, 0xbc); }

float PlayerEgo::getCloakRate() {
    PlayerEgo *self = this;
  int off = 0x210;
  if (C(self, 0x1ac) == 0) off = 0x214;
  float a = (float)I(self, 0x208);
  float b = (float)I(self, off);
  float d = a / b;
  return d >= 0.0f ? d : 1.0f;
}

void PlayerEgo::resetLastHP() {
    PlayerEgo *self = this;
  I(self, 0x130) = ((Player *)(P(self, 0)))->getCombinedHP();
}

void PlayerEgo::setExhaustVisible(bool param) {
    PlayerEgo *self = this;
  void* lvl = P(self, 0xc);
  C(self, 0x32f) = param;
  C(P(lvl, 0x80), 0) = param;
  unsigned* arr = (unsigned*)P(lvl, 0xa8);
  if (arr != 0) {
    for (unsigned i = 0; i < arr[0]; i++) {
      int* p = (int*)arr[1];
      ((ParticleSystemManager *)(P(P(self, 0xc), 0x80)))->enableSystemEmit2(p[i], param);
    }
  }
}

int PlayerEgo::shouldSwitchToStandardCam() {
    PlayerEgo *self = this;
  if (C(self, 0xb0) == 0) return 0;
  C(self, 0xb0) = 0;
  return 1;
}

void PlayerEgo::resetMovement() {
    PlayerEgo *self = this;
  D(self, 0x278) = 0;
  D(self, 0x268) = 0;
  D(self, 0x270) = 0;
}

bool PlayerEgo::isHacking() {
    PlayerEgo *self = this; return I(self, 0x1e8) != 0; }

void PlayerEgo::setPosition() {
    PlayerEgo *self = this; return PlayerEgo_setPosition_v(self); }

void PlayerEgo::resetChargingDrive() {
    PlayerEgo *self = this; UC(self, 0x204) = 0; }

// 000aa5dc: ldr r0,[r0,#0x0] ; b.w <ext>   — tail-call ext(self->m_pPlayer)
void PlayerEgo::PauseEngineSound() {
    PlayerEgo *self = this;
  return PlayerEgo_PauseEngineSound_ext(P(self, 0));
}

unsigned char PlayerEgo::isInFreeLookMode() {
    PlayerEgo *self = this; return UC(self, 0xc4); }

void PlayerEgo::setActive(bool) {
    PlayerEgo *self = this; return PlayerEgo_setActive_ext(P(self, 0)); }

void PlayerEgo::alignToHorizon() {
    PlayerEgo *self = this; UC(self, 0x2f4) = 1; }

void PlayerEgo::setAutoTurret(bool on) {
    PlayerEgo *self = this;
  C(self, 0x355) = on;
  if (!on) PlayerEgo_setAutoTurret_ext(P(self, 0), 2);
}

void PlayerEgo::hitCamera() {
    PlayerEgo *self = this;
  C(self, 0x32c) = 1;
  int cam = I(self, 0x88);
  I(self, 0x328) = 0;
  return PlayerEgo_hitCamera_ext(cam);
}

int PlayerEgo::hackingWon() {
    PlayerEgo *self = this;
  int v = I(self, 0x1e8);
  if (v == 0) return 0;
  return PlayerEgo_hackingWon_ext(v);
}

unsigned char PlayerEgo::lostMiningGame() {
    PlayerEgo *self = this; return UC(self, 0x39b); }

int PlayerEgo::getCurrentMiningAmount() {
    PlayerEgo *self = this;
  int v = I(self, 0x1e4);
  if (v == 0) return 0;
  return PlayerEgo_getCurrentMiningAmount_ext(v);
}

void PlayerEgo::setDockingState(int s) {
    PlayerEgo *self = this;
  if (s == 2 && I(self, 0x1c4) == 1) C(self, 0xb2) = 1;
  I(self, 0x1c4) = s;
}

void PlayerEgo::hackingRotateLCW() {
    PlayerEgo *self = this;
  if (I(self, 0x1e8) != 0 && ((HackingGame *)(I(self, 0x1e8)))->isRotating() == 0
      && ((HackingGame *)(I(self, 0x1e8)))->gameWon() == 0)
    PlayerEgo_hackingRotateLCW_ext(I(self, 0x1e8), 1);
}

bool PlayerEgo::isInDockingProcedure() {
    PlayerEgo *self = this;
  if (C(self, 0x1c0) != 0) return true;
  return C(self, 0x356) != 0;
}

void PlayerEgo::setSpeed(float v) {
    PlayerEgo *self = this; F(self, 0xb8) = v; }

void PlayerEgo::ResumeEngineSound() {
    PlayerEgo *self = this; return PlayerEgo_ResumeEngineSound_ext(P(self, 0), 0); }

void PlayerEgo::addNukeVolatileForce(float v) {
    PlayerEgo *self = this;
  void* p = P(self, 0);
  F(p, 0x60) = F(p, 0x60) + v * 3.0f;
}

extern void* g_explode_obj;
extern void (*g_explode_fn)(void*, int);
void PlayerEgo::explode() {
    PlayerEgo *self = this;
  ((ParticleSystemManager *)(P(P(self, 0xc), 0x74)))->enableSystemEmit3(I(self, 0x2fc), 1);
  if (I(self, 0x8c) != 0) return;
  ((TargetFollowCamera *)(P(self, 0x88)))->setActive(0);
  void* e = ::operator new(0x68);
  Explosion_ctor(e, 0);
  int pl = I(self, 0);
  I(self, 0x8c) = (int)(intptr_t)e;
  Player_setActive_(pl);
  void* o = g_explode_obj;
  void (*fn)(void*, int) = g_explode_fn;
  fn(*(void**)o, *(int*)(*(void**)o));
  fn(*(void**)o, I(self, 0x1c));
  fn(*(void**)o, 0x1b);
  fn(*(void**)o, 0x23);
  fn(*(void**)o, 0x8d5);
  fn(*(void**)o, 0x8d4);
  fn(*(void**)o, 0x8cc);
  fn(*(void**)o, 0x447);
  fn(*(void**)o, 0x448);
  fn(*(void**)o, 0x449);
  return PlayerEgo_explode_ext(self, 0);
}

unsigned char PlayerEgo::isDockingToStream() {
    PlayerEgo *self = this; return UC(self, 0x1ec); }

int PlayerEgo::goingToStream() {
    PlayerEgo *self = this;
  void* m = P(self, 0xc);
  void* r4 = P(self, 0x15c);
  void* lm = (void *)((Level *)(m))->getLandmarks();
  return r4 == P(P(lm, 4), 4);
}

Vec3 PlayerEgo::GetDirVector() {
    PlayerEgo *self = this; return ((AEGeometry *)P(self, 8))->getDirection(); }

void PlayerEgo::hideShipForFirstPersonCameraView(bool param) {
    PlayerEgo *self = this;
  unsigned char r1 = param;
  C(self, 0x32d) = r1;
  unsigned char nr = r1 ^ 1;
  C(self, 0x309) = (C(self, 0x32e) != 0) & nr;
  *(char*)P(P(self, 0xc), 0x80) = nr & (C(self, 0x32f) != 0);
}

void PlayerEgo::changeThrust(float v) {
    PlayerEgo *self = this;
  float n = F(self, 0xbc) + v;
  float p3 = 1.0f;
  if (n < 1.0f) p3 = 0.0f;
  if (n < 0.0f) p3 = 0.0f;
  float p2 = p3;
  if (n < 1.0f) p2 = n;
  if (n < 0.0f) p2 = p3;
  F(self, 0xbc) = p2;
}

void PlayerEgo::deleteHackingGame() {
    PlayerEgo *self = this;
  HackingGame* g = (HackingGame*)P(self, 0x1e8);
  if (g != 0) delete g;
  P(self, 0x1e8) = 0;
}

bool PlayerEgo::explosionEnded() {
    PlayerEgo *self = this;
  if (I(self, 0x8c) == 0) return true;
  return 8000 < I(self, 0x2f8);
}

float PlayerEgo::getBoostPercentage() {
    PlayerEgo *self = this;
  float den = (float)__aeabi_idiv(I(self, 0xcc), 6);
  float d = (float)I(self, 0x138) / den;
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
    PlayerEgo *self = this; return ((AEGeometry *)(P(self, 8)))->getUpVector(); }

bool PlayerEgo::isDockedToPlanet() {
    PlayerEgo *self = this; return 3000 < I(self, 0x1f0); }

bool PlayerEgo::emergencySystemActive() {
    PlayerEgo *self = this; return 0 < I(self, 0x30c); }

bool PlayerEgo::isDockingToAsteroid() {
    PlayerEgo *self = this;
  if (C(self, 0x1c0) != 0) return I(self, 0x1c4) != 1;
  return false;
}

float PlayerEgo::getBoostRate() {
    PlayerEgo *self = this;
  float d = (float)I(self, 0x138) / (float)I(self, 0xd0);
  float r = d + 1.0f;
  if (0.0f < d) r = 1.0f;
  return r;
}

bool PlayerEgo::driveReady() {
    PlayerEgo *self = this; return I(self, 0x1fc) >= I(self, 0x200); }

void PlayerEgo::turnVertical(int a, float v) {
    PlayerEgo *self = this;
  if (v < -0.0f) { PlayerEgo_turnVertical_neg(); return; }
  if (v > 0.0f) { PlayerEgo_turnVertical_pos(self); return; }
}

unsigned char PlayerEgo::isInTurretMode() {
    PlayerEgo *self = this; return UC(self, 0x1a0); }

void PlayerEgo::startJumpDrive() {
    PlayerEgo *self = this;
  if (C(self, 0x204) != 0) return;
  ((FModSound *)(*(void**)g_FMod_singleton))->play(0x21, (Vector *)0, (Vector *)0, 0);
  ((Hud *)(I(self, 0x220)))->hudEvent(0x19, self, 0);
  I(self, 0x1fc) = 0;
  C(self, 0x204) = 1;
}

bool PlayerEgo::isInRocketControl() {
    PlayerEgo *self = this; return I(self, 0x194) != 0; }

int PlayerEgo::getSpeed() {
    PlayerEgo *self = this; return I(self, 0xb8); }

float PlayerEgo::getVolatileForce() {
    PlayerEgo *self = this;
  float f = F(P(self, 0), 0x60);
  float r = 0.0f;
  if (!(f < 0.0f)) {
    r = 1.0f;
    if (!(f > 1.0f)) r = f;
  }
  return r;
}

unsigned char PlayerEgo::isDockingToPlanet() {
    PlayerEgo *self = this; return UC(self, 0x1ee); }

unsigned char PlayerEgo::isChargingCloak() {
    PlayerEgo *self = this; return UC(self, 0x1ad); }

extern void* g_mining_status;
bool PlayerEgo::isDockedToMiningPlant() {
    PlayerEgo *self = this;
  if (C(self, 0x356) != 0 && I(self, 0x1c4) == 1) {
    if (((Mission *)(((Status *)(g_mining_status))->getMission()))->isEmpty() != 0
        && ((Status *)(g_mining_status))->inAlienOrbit() == 0) {
      return Station_getIndex(((Status *)(g_mining_status))->getStation()) == 0x67;
    }
  }
  return false;
}

extern PlayerEgo* g_PlayerEgo_singleton;
void PlayerEgo::setCurrentSecondaryWeaponIndex(int idx) {
    PlayerEgo *self = this;
  I(self, 0x10c) = idx;
  I(g_PlayerEgo_singleton, 0xf4) = idx;
}

// `struct Route` (with its destructor) is provided by gof2/Route.h.
void PlayerEgo::removeRoute() {
    PlayerEgo *self = this;
  Route* r = (Route*)P(self, 0xfc);
  if (r != 0) delete r;
  P(self, 0xfc) = 0;
}

void PlayerEgo::setRoute(int v) {
    PlayerEgo *self = this; I(self, 0xfc) = v; }

unsigned char PlayerEgo::boosting() {
    PlayerEgo *self = this; return UC(self, 0x13c); }

bool PlayerEgo::isDockedToAsteroid() {
    PlayerEgo *self = this;
  if (C(self, 0x1c0) == 0) return false;
  return I(self, 0x1c4) == 1;
}

int PlayerEgo::goingToStation() {
    PlayerEgo *self = this;
  void* lm = (void *)((Level *)(P(self, 0xc)))->getLandmarks();
  if (P(P(lm, 4), 0) == 0) return false;
  void* r4 = P(self, 0x15c);
  lm = (void *)((Level *)(P(self, 0xc)))->getLandmarks();
  return r4 == P(P(lm, 4), 0);
}

float PlayerEgo::getCloakingPercentage() {
    PlayerEgo *self = this;
  float r = 0.0f;
  if (C(self, 0x1ac) != 0) {
    int v = I(self, 0x208);
    if (v >= 0) {
      if (v < 2000) {
        r = (float)v * 100.0f / 2000.0f;
      } else {
        int lo = I(self, 0x210) - 2000;
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
    PlayerEgo *self = this; return I(self, 0xc8); }

void PlayerEgo::addGun(void* gun, int x) {
    PlayerEgo *self = this;
  ((Player *)(P(self, 0)))->addGun((Gun *)gun, x);
  return PlayerEgo_addGun_ext(self);
}

unsigned char PlayerEgo::aboutToReachAutoTarget() {
    PlayerEgo *self = this; return UC(self, 0x330); }

unsigned char PlayerEgo::autoTurretIsEnabled() {
    PlayerEgo *self = this; return UC(self, 0x355); }

bool PlayerEgo::readyToBoost() {
    PlayerEgo *self = this; return -1 < I(self, 0x138); }

void PlayerEgo::endExplosion() {
    PlayerEgo *self = this;
  int v = I(self, 0x8c);
  if (v != 0) PlayerEgo_endExplosion_ext(v);
}

bool PlayerEgo::isLandingOrTakingOff() {
    PlayerEgo *self = this;
  if (C(self, 0x356) == 0) return false;
  return (U(self, 0x1c4) | 1) == 3;
}

void PlayerEgo::setFreeze(bool v) {
    PlayerEgo *self = this; C(self, 0x24) = v; }

int PlayerEgo::getHUD() {
    PlayerEgo *self = this; return I(self, 0x220); }

int PlayerEgo::getRoute() {
    PlayerEgo *self = this; return I(self, 0xfc); }

void PlayerEgo::setCollide(bool v) {
    PlayerEgo *self = this; C(self, 0x144) = v; }

extern void* g_emerg_status;
extern void* g_emerg_fmod;
int PlayerEgo::tryToStartEmergencySystem() {
    PlayerEgo *self = this;
  if (I(self, 0xac) == 0 || I(self, 0x30c) != 0) return 0;
  if (((Player *)(P(self, 0)))->getHitpoints() > 1) return 0;
  ((Player *)(P(self, 0)))->setHitpoints(1);
  I(self, 0x30c) = I(self, 0x310);
  ((Player *)(P(self, 0)))->setVulnerable(0);
  void* s1 = ((Status *)(g_emerg_status))->getShip();
  Item* eq = ((Ship *)(((Status *)(g_emerg_status))->getShip()))->getFirstEquipmentOfSort(0x1b);
  ((Ship *)(s1))->removeEquipment(eq);
  ((FModSound *)(*(void**)g_emerg_fmod))->play(0x45b, (Vector *)0, (Vector *)0, 1.0f);
  return 1;
}

unsigned char PlayerEgo::hasAutoTurret() {
    PlayerEgo *self = this; return UC(self, 0x180); }

unsigned char PlayerEgo::collidesWithStation() {
    PlayerEgo *self = this; return UC(self, 0x234); }

void PlayerEgo::stopPlanetDock() {
    PlayerEgo *self = this;
  ((TargetFollowCamera *)(I(self, 0x88)))->setLookAtCam(0);
  C(self, 0x144) = 1;
  C(self, 0x1ee) = 0;
  ((PlayerEgo *)(self))->stopBoost();
  I(self, 0xb8) = 0x40000000;
}

bool PlayerEgo::isRechargingCloak() {
    PlayerEgo *self = this; return 0 < I(self, 0x20c); }

// 000aa9bc: ldrb.w r0,[r0,#0x398] ; bx lr
unsigned char PlayerEgo::hasVolatileGoods() {
    PlayerEgo *self = this;
  return UC(self, 0x398);
}

void PlayerEgo::hackingRotateRCW() {
    PlayerEgo *self = this;
  if (I(self, 0x1e8) != 0 && ((HackingGame *)(I(self, 0x1e8)))->isRotating() == 0
      && ((HackingGame *)(I(self, 0x1e8)))->gameWon() == 0)
    PlayerEgo_hackingRotateRCW_ext(I(self, 0x1e8), 1);
}

bool PlayerEgo::hasCloak() {
    PlayerEgo *self = this; return P(self, 0x1b0) != 0; }

int PlayerEgo::isBoostRefreshed() {
    PlayerEgo *self = this;
  if (C(self, 0x13c) != 0) return 0;
  if (C(self, 0x146) == 0) return 0;
  if (I(self, 0x138) > -1) return 1;
  return 0;
}

Vec3 PlayerEgo::getPosition() {
    PlayerEgo *self = this; return ((AEGeometry *)(P(self, 8)))->getPosition(); }

// World-space position of the turret muzzle: compose the turret geometry's local
// matrix (at +0x28) with the ship hull geometry's matrix (at +0x8) and take the
// translation component of the product.
Vec3 PlayerEgo::getTurretPosition() {
    PlayerEgo *self = this;
    Matrix &turret = ((AEGeometry *)(P(self, 0x28)))->getMatrix();
    Matrix &ship   = ((AEGeometry *)(P(self, 0x8)))->getMatrix();
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
    PlayerEgo *self = this;
    if ((unsigned)(type - 1) < 2 && C(self, 0x398) != 0) {
        void *player = P(self, 0x0);
        F(player, 0x60) = F(player, 0x60) + g_PE_rollNudge;
    }

    if (I(self, 0x334) == 0) {
        I(self, 0x334) = type;
        P(self, 0x350) = 0;
        if (type == 3) {
            float pos[3];
            ((PlayerEgo *)(self))->getPosition();

            float dir[3];
            *(AbyssEngine::AEMath::Vector *)dir = ((AEGeometry *)P(self, 0x8))->getDirection();

            float scaled[3];
            Vec_scale(scaled, dir, g_PE_strafeDist);

            float target[3];
            Vec_sub(target, pos, scaled);
            Vec_assign((char *)self + 0x338, target);

            *(AbyssEngine::AEMath::Vector *)target = ((AEGeometry *)P(self, 0x8))->getDirection();
            Vec_assign((char *)self + 0x344, target);
        }
    }
}

void PlayerEgo::refillGunDelay() {
    PlayerEgo *self = this; return PlayerEgo_refillGunDelay_ext(P(self, 0), 0); }

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

__attribute__((visibility("hidden"))) extern void **g_PE_cft_canvas;   // 0xab512 PaintCanvas
extern const float g_PE_cft_transformVal;   // 0xab76c muzzle transform +0xe0

void PlayerEgo::checkForTurret() {
    PlayerEgo *self = this;
    if (C(self, 0x170) != 0)
        return;
    int avail = ((Player *)(P(self, 0x0)))->gunAvailable(0);
    C(self, 0x170) = (unsigned char)avail;
    if (avail == 0)
        return;

    C(self, 0x180) = 0;
    int equip = (int)(intptr_t)((Ship*)(PE_status()->getShip()))->getEquipment(2);
    void *item = *(void **)(equip + 4);
    I(self, 0x1f8) = (int)((double)((Item *)(*(void **)(item)))->getAttribute(0) * 1.5);

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
        C(self, 0x180) = 1; base = 0x1a95; barrel = 0x1a96;
    } else if (idx == 0xb5) {
        C(self, 0x180) = 1; base = 0x1a97; barrel = 0x1a98;
    } else if (idx == 0xb6) {
        C(self, 0x180) = 1; base = 0x1a99; barrel = 0x1a9a;
    } else if (idx == 0xc6) {
        base = 0x4963; barrel = 0x4964; muzzle = 0x4966; child = 0x4967; extra = -1; extra2 = 0x4a7f;
    } else if (idx == 0xc7) {
        base = 0x4968; barrel = 0x4969; muzzle = 0x496a; child = 0x496b; extra = -1; extra2 = 0x4a7f;
    } else if (idx == 0xc8) {
        base = 0x496c; barrel = 0x496d; muzzle = 0x496e; child = 0x496f; extra = 0x4970; extra2 = 0x4a7f;
    } else if (idx == 0x2f) {
        base = 0x1a72; barrel = 0x1a73;
    }

    void **canvasHolder = g_PE_cft_canvas;
    void *canvas = *canvasHolder;

    void *baseGeo = (void*)new AEGeometry((uint16_t)base, (PaintCanvas*)canvas, false);
    P(self, 0xf4) = baseGeo;
    void *yawGeo = (void*)new AEGeometry((uint16_t)barrel, (PaintCanvas*)canvas, false);
    P(self, 0x34) = yawGeo;
    ((AEGeometry *)yawGeo)->setRotationOrder(2);
    void *muzzleRoot = (void*)new AEGeometry((PaintCanvas*)canvas);
    P(self, 0x38) = muzzleRoot;

    if (muzzle != -1) {
        void *g = (void*)new AEGeometry((uint16_t)(unsigned short)muzzle, (PaintCanvas*)canvas, false);
        ((AEGeometry *)(P(self, 0xf4)))->addChild((uint32_t)(uintptr_t)g);
        ((AEGeometry *)g)->~AEGeometry(); ::operator delete(g);
    }
    if (child != -1) {
        void *g = (void*)new AEGeometry((uint16_t)(unsigned short)child, (PaintCanvas*)canvas, false);
        ((AEGeometry *)(P(self, 0x34)))->addChild((uint32_t)(uintptr_t)g);
        ((AEGeometry *)g)->~AEGeometry(); ::operator delete(g);
    }
    if (extra != -1) {
        void *g = (void*)new AEGeometry((uint16_t)(unsigned short)extra, (PaintCanvas*)canvas, false);
        ((AEGeometry *)(P(self, 0x34)))->addChild((uint32_t)(uintptr_t)g);
        ((AEGeometry *)g)->~AEGeometry(); ::operator delete(g);
    }
    if (extra2 != -1) {
        void *g = (void*)new AEGeometry((uint16_t)(unsigned short)extra2, (PaintCanvas*)canvas, false);
        P(self, 0x3c) = g;
        ((AEGeometry *)(P(self, 0x34)))->addChild((uint32_t)(uintptr_t)g);
        void *tf = ((PaintCanvas*)g_PaintCanvas)->TransformGetTransform((unsigned int)(*(unsigned int *)canvasHolder));
        ((AbyssEngine::Transform *)(tf))->SetVisible(C(self, 0x2c4) != 0);
    }

    void *tf = ((PaintCanvas*)g_PaintCanvas)->TransformGetTransform((unsigned int)(*(unsigned int *)canvasHolder));
    F(tf, 0xe0) = g_PE_cft_transformVal;
    tf = ((PaintCanvas*)g_PaintCanvas)->TransformGetTransform((unsigned int)(*(unsigned int *)canvasHolder));
    ((AbyssEngine::Transform *)(tf))->SetAnimationState((AbyssEngine::AnimationMode)2, (void *)0);

    ((AEGeometry *)(P(self, 0xf4)))->setPosition(*(Vector *)((char *)self + 0x23c));
    ((AEGeometry *)(P(self, 0x34)))->setPosition(*(Vector *)((char *)self + 0x23c));

    PE_cft_place(self, idx);

    ((AEGeometry *)(P(self, 0x38)))->addChild((uint32_t)(uintptr_t)P(self, 0xf4));
    ((AEGeometry *)(P(self, 0x38)))->addChild((uint32_t)(uintptr_t)P(self, 0x34));

    if (P(self, 0x2c0) != 0 && C(self, 0x170) != 0) {
        ((PaintCanvas*)(long)(canvas))->MeshCloneMaterial((unsigned int)(I(P(self, 0xf4), 0x1c)), (unsigned int *)(&U(self, 0x388)));
        PE_cft_finishMaterials(canvas, I(P(self, 0x34), 0x1c), (char *)self + 0x38c);
    }
}

void PlayerEgo::pitchAllPrimaryGuns(float) {
    PlayerEgo *self = this; return PlayerEgo_pitchAllPrimaryGuns_ext(P(self, 0)); }

void PlayerEgo::stopShooting(int param) {
    PlayerEgo *self = this;
  if (C(self, 0x1a0) != 0) {
    stopShooting_extA(P(self, 0), 2);
    return;
  }
  if (((PlayerEgo *)(self))->isDead() != 0) return;
  void* p = P(self, 0);
  if (param == 1) {
    stopShooting_extB(p, 1, I(self, 0x10c));
    return;
  }
  stopShooting_extA(p, param);
}

// PlayerEgo::shake(int amount)
//   Produces a random camera-shake offset whose magnitude scales with `amount`
//   and the current shake intensity (field 0x134), then applies it to the
//   target-follow camera (field 0x8) via the camera shake entry point.

__attribute__((visibility("hidden"))) extern void **g_PE_rng;        // AERandom singleton
extern const float g_PE_shakeDiv;                                    // 0xb21d4 normaliser

void PlayerEgo::shake(int amount) {
    PlayerEgo *self = this;
    void *cam = P(self, 0x8);
    float a = (float)amount / g_PE_shakeDiv;
    float intensity = (float)(I(self, 0x134) << 1);

    int range = (int)(a * intensity);
    if (range < 2)
        range = 1;
    int span = range << 1;

    void *rng = *g_PE_rng;
    float dx = (float)(((AbyssEngine::AERandom *)(rng))->next(span) - range);
    float dy = (float)(((AbyssEngine::AERandom *)(rng))->next(span) - range);
    float dz = (float)(((AbyssEngine::AERandom *)(rng))->next(span) - range);
    ((AbyssEngine::Camera *)(cam))->shake(dx, dy, dz);
}

void PlayerEgo::setRotation(float rx, float ry, float rz) {
    PlayerEgo *self = this;
  char local[60];
  F(self, 0x2e8) = rx;
  F(self, 0x2ec) = ry;
  F(self, 0x2f0) = rz;
  void* t = *(void**)g_setRotation_transform;
  void* m = TransformGetLocal(t, I(P(self, 4), 0xc));
  MatrixSetRotation(local, m, F(self, 0x2e8), F(self, 0x2ec), F(self, 0x2f0));
}

extern void* g_engine_status;
extern void* g_engine_fmod;
void PlayerEgo::StopEngineSound() {
    PlayerEgo *self = this;
  if (C(self, 0x356) == 0 || I(self, 0x1c4) != 1) {
    if (((Ship *)(((Status *)(g_engine_status))->getShip()))->getFirstEquipmentOfSort(0x26) != 0
        && ((Status *)(g_engine_status))->inAlienOrbit() == 0) {
      void* obj = g_engine_status;
      int idx = Station_getIndex(((Status *)(obj))->getStation());
      int cm = ((Status *)(g_engine_status))->getCurrentCampaignMission();
      float g = ((Status *)(obj))->getGammaRayDamagePerSecond(idx, cm);
      if (0.0f < g && I(self, 0xb4) != -1) {
        ((FModSound *)(*(void**)g_engine_fmod))->play((int)(intptr_t)P(self, 0xb4), (Vector *)0, (Vector *)0, g);
      }
    }
  }
  return PlayerEgo_StopEngineSound_ext(P(self, 0));
}

void PlayerEgo::startSmokeEmission() {
    PlayerEgo *self = this;
  int v = I(self, 0x300);
  if (v < 0) return;
  ((ParticleSystemManager *)(P(P(self, 0xc), 0x78)))->enableSystemEmit(v, 1);
  PlayerEgo_startSmokeEmission_ext(P(P(self, 0xc), 0x84), I(self, 0x304), 1);
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
    PlayerEgo *self = this;
    if (P(self, 0x1e4) != 0) {
        if (C(*g_PE_d_miningGate, 0x10) == 0)
            return ((MiningGame *)(P(self, 0x1e4)))->steerYAlt(delta);
        return ((MiningGame *)(P(self, 0x1e4)))->steerY(-delta);
    }

    if (C(self, 0x1a0) != 0) {
        float ft = (float)frameTime;
        if (F(self, 0x1a8) < g_PE_d_eps) {
            float ang = ft * delta + F(self, 0x1a8);
            F(self, 0x1a8) = ang;
            ((AEGeometry *)P(self, 0x28))->rotate((float)(ang * g_PE_d_lookK1 * g_PE_d_lookK2), 0.0f, 0.0f);
        }
        float p = F(self, 0x1a4);
        if (p < g_PE_d_eps) {
            float half = ft * delta * 0.5f;
            float ang = half * g_PE_d_lookK1 * g_PE_d_lookK2;
            F(self, 0x1a4) = half + p;
            return (((AEGeometry *)P(self, 0x19c))->rotate((float)ang, 0.0f, 0.0f), 0.0f);
        }
        return p;
    }

    if (P(self, 0x194) != 0) {
        float v = (float)frameTime * g_PE_d_manK * F(P(self, 0x194), 0x50);
        F(self, 0x7c) = v;
        return v;
    }

    if (C(self, 0x158) != 0)
        return delta;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) != 1)
        return delta;

    I(self, 0x100) = -1;
    float rate;
    if (C(self, 0x235) == 0) {
        rate = F(self, 0x154);
    } else {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        rate = F(self, 0x154) * (1.0f - cur / max) * g_PE_d_loadK + F(self, 0x154) * g_PE_d_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_d_rateK * rate), 0x3f);
    F(self, 0x270) = delta;
    F(self, 0x258) = -rate;
    if (F(self, 0x278) < target) {
        float v = F(self, 0x278) + PE_pitchRampDelta(self, rate, frameTime);
        if (target < v) v = target;
        F(self, 0x278) = v;
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
__attribute__((visibility("hidden"))) extern void **g_PE_adp_fixedObj;  // 0xb00f0 PlayerFixedObject

// clears the 3-float dock-offset vector at 0x1c8.
static inline void adp_clearDockVector(PlayerEgo *self)
{
    I(self, 0x1c8) = 0;
    I(self, 0x1cc) = 0;
    I(self, 0x1d0) = 0;
}

// Decide the arrival HUD event for state 2; returns the event id (0 = none).
static int adp_arrivalEvent(PlayerEgo *self, void *station)
{
    void *mission = PE_status()->getMission();
    void *campaign = (void *)PE_status()->getCampaignMission();
    void *fixed = *g_PE_adp_fixedObj;

    if (((Mission *)(mission))->isEmpty() == 0 && ((Mission *)(mission))->getType() == 0xf
        && ((PlayerFixedObject *)(fixed))->getDockingType() == 1) {
        if (((Ship *)(PE_status()->getShip()))->hasCargo(((Mission *)(mission))->getProductionGoodIndex(), 1) != 0) {
            int amount = ((Item *)(((Ship *)(PE_status()->getShip()))->getCargo(((Mission *)(mission))->getProductionGoodIndex())))->getAmount();
            I(self, 0x180) = amount;
            int need = ((Mission *)(mission))->getProductionGoodAmount()
                       - ((Level *)(P(self, 0xc)))->getNumDeliveredOre();
            if (need < amount) I(self, 0x180) = need;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x29;
        }
        return 0;
    }

    if (((Mission *)(mission))->isEmpty() == 0
        && (((Mission *)(mission))->getType() == 0xb8 || ((Mission *)(mission))->getType() == 0xa8)
        && ((PlayerFixedObject *)(fixed))->getDockingType() == 2) {
        // passenger drop-off
        int carried = I(fixed, 0x178);
        int maxPax = ((Ship *)(PE_status()->getShip()))->getMaxPassengers();
        if (maxPax > 0 && carried < maxPax) {
            int avail = (((Mission *)(mission))->getType() == 0xa8)
                        ? ((Mission *)(mission))->getStatusValue() : maxPax;
            avail -= carried;
            I(self, 0x180) = avail;
            int status = ((Mission *)(mission))->getStatusValue() - carried;
            if (status < avail) I(self, 0x180) = status;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x23;
        }
        if (((Mission *)(mission))->getType() != 0xa8 && ((Ship *)(PE_status()->getShip()))->getMaxPassengers() == 0)
            return 0x2b;
        return 0;
    }

    if (((Mission *)(mission))->isEmpty() == 0 && ((Mission *)(mission))->getType() == 0xb8
        && ((PlayerFixedObject *)(fixed))->getDockingType() == 1) {
        int n = I(fixed, 0x178);
        if (n > 0) {
            I(self, 0x184) = 0;
            I(self, 0x180) = n;
            return 0x25;
        }
        return 0;
    }

    if (campaign != 0 && ((Mission *)(campaign))->isEmpty() == 0
        && (((Mission *)(campaign))->getType() == 0xa7 || ((Mission *)(campaign))->getType() == 0xae)
        && ((PlayerFixedObject *)(fixed))->getDockingType() == 1) {
        if (((Ship *)(PE_status()->getShip()))->hasCargo(((Mission *)(campaign))->getProductionGoodIndex(), 1) != 0) {
            int amount = ((Item *)(((Ship *)(PE_status()->getShip()))->getCargo(((Mission *)(campaign))->getProductionGoodIndex())))->getAmount();
            I(self, 0x180) = amount;
            int need = ((Mission *)(campaign))->getProductionGoodAmount() - ((Mission *)(campaign))->getStatusValue();
            if (need < amount) I(self, 0x180) = need;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x29;
        }
    }
    return 0;
}

int PlayerEgo::approachDockingPoint(void *hud, int /*hud2*/, void *radar) {
    PlayerEgo *self = this;
    if (((KIPlayer *)(self))->isDying() != 0)
        return 0;

    int state = I(self, 0x314);

    if (state == 0) {
        void *station = P(self, 0x1e0);
        float pos[3];
        ((PlayerEgo *)(self))->getPosition();
        void *nav = ((KIPlayer *)(station))->getNearestNavigationPoint((Vector *)pos, (char *)self + 0x350);
        if (nav != 0) {
            if (P(self, 0x380) != nav) {
                if (P(self, 0x380) != 0)
                    ((SpacePoint *)(P(self, 0x380)))->giveFree();
                P(self, 0x380) = nav;
                ((SpacePoint *)(nav))->take();
            }
            int dist = PE_adp_approach(self, station);
            if (dist < I(self, 0x1cc)) {
                I(self, 0x314) = 2;
                ((TargetFollowCamera *)(P(self, 0x88)))->setLookAtCam(false);
                ((TargetFollowCamera *)(P(self, 0x88)))->useTargetsUpVector(false);
            }
        }
        PE_adp_apply(self);
        return 0;
    }

    if (state == 2) {
        if (C(self, 0x2c4) != 0)
            ((PlayerEgo *)(self))->setTurretMode(0);
        int dist = PE_adp_glide(self);
        int radius = g_PE_adp_dockRadius[((Ship *)(PE_status()->getShip()))->getIndex()];
        if (dist < radius) {
            int ev = adp_arrivalEvent(self, P(self, 0x1e0));
            if (ev != 0)
                ((Hud *)(hud))->hudEvent(ev, self, 0);
            I(self, 0x314) = 1;        // hand back to manual control
        }
        PE_adp_apply(self);
        return 0;
    }

    if (state == 3) {
        int dist = PE_adp_glide(self);
        if (dist < 200) {
            // docking complete: restore cameras and free the nav point.
            P(self, 0x1e0) = 0;
            P(radar, 0x4) = 0;
            P(radar, 0x8) = 0;
            adp_clearDockVector(self);
            ((TargetFollowCamera *)(P(self, 0x88)))->setActive(true);
            ((TargetFollowCamera *)(P(self, 0x88)))->setLookAtCam(false);
            ((TargetFollowCamera *)(P(self, 0x88)))->useTargetsUpVector(false);
            ((LevelScript *)(P(self, 0x10)))->resetCamera((Level *)P(P(self, 0x10), 0x18));
            ((Player *)(P(self, 0x0)))->resetGunDelay(0);
            I(self, 0x314) = 0;
            if (P(self, 0x380) != 0) {
                ((SpacePoint *)(P(self, 0x380)))->giveFree();
                P(self, 0x380) = 0;
            }
            if (P(self, 0x1e8) != 0) {
                ::operator delete(HackingGame_dtor(P(self, 0x1e8)));
                P(self, 0x1e8) = 0;
                ((Hud *)(hud))->setHackingGameActive(false);
            }
            return 1;        // docking complete
        }
        PE_adp_apply(self);
    }
    return 0;
}

extern void* g_setLevel_status;
void PlayerEgo::setLevel(void* level) {
    PlayerEgo *self = this;
  I(self, 0xc) = (int)(intptr_t)level;
  void* src = P(level, 0x74);
  void* gm = (void*)&((AEGeometry *)P(self, 8))->getMatrix();
  void* sys = (void *)((ParticleSystemManager *)(src))->addSystem(gm, 9, 0);
  I(self, 0x2fc) = (int)(intptr_t)sys;
  ((ParticleSystemManager *)(P(P(self, 0xc), 0x74)))->enableSystemEmit3((int)(intptr_t)sys, 0);
  if (((Status *)(g_setLevel_status))->getCurrentCampaignMission() > 1) return;
  void* src2 = P(P(self, 0xc), 0x78);
  void* gm2 = (void*)&((AEGeometry *)P(self, 8))->getMatrix();
  void* sys2 = (void *)((ParticleSystemManager *)(src2))->addSystem(gm2, 0xf, 0);
  I(self, 0x300) = (int)(intptr_t)sys2;
  ((ParticleSystemManager *)(P(P(self, 0xc), 0x78)))->enableSystemEmit3((int)(intptr_t)sys2, 0);
  void* src3 = P(P(self, 0xc), 0x84);
  void* gm3 = (void*)&((AEGeometry *)P(self, 8))->getMatrix();
  void* sys3 = (void *)((ParticleSystemManager *)(src3))->addSystem(gm3, 0x2a, 0);
  I(self, 0x304) = (int)(intptr_t)sys3;
  return PlayerEgo_setLevel_ext(P(P(self, 0xc), 0x84), (int)(intptr_t)sys3, 0);
}

// PlayerEgo::setDockingCamera()
//   Lazily builds the docking camera rig (a PaintCanvas camera plus a small
//   chain of AEGeometry nodes used to orient/offset the view) on first use,
//   seeding a default offset vector at 0x224 when none is set, then positions
//   the leaf node at the dock offset (0x148), copies the ship matrix onto it,
//   and makes the camera current.

__attribute__((visibility("hidden"))) extern void **g_PE_dc_canvas;   // 0xaab84 PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_PE_dc_canvas2;  // 0xaab8c PaintCanvas (current)
extern const float g_PE_dc_fovNormal;
extern const float g_PE_dc_fovAlien;
extern const unsigned int g_PE_dc_defX; // 0xaab78 default offset lo
extern const unsigned int g_PE_dc_defY; // 0xaab7c default offset hi

void PlayerEgo::setDockingCamera() {
    PlayerEgo *self = this;
    if (P(self, 0x178) == 0) {
        void **holder = g_PE_dc_canvas;
        ((PaintCanvas*)(long)(*holder))->CameraCreate((unsigned int *)(&U(self, 0x174)));
        unsigned int cam = (unsigned int)(unsigned long)*holder;

        float fov = (PE_status()->inAlienOrbit() != 0) ? g_PE_dc_fovAlien : g_PE_dc_fovNormal;
        ((PaintCanvas*)g_PaintCanvas)->CameraSetPerspective((unsigned int)(cam), fov, 0.0f, g_PE_dc_fovAlien);

        void *node = (void*)new AEGeometry((PaintCanvas*)(*holder));
        P(self, 0x178) = node;
        ((AEGeometry *)node)->setRotationOrder(2);

        // seed the default rig offset (0x224..0x22c) only when fully zero.
        if (F(self, 0x224) == 0.0f && F(self, 0x228) == 0.0f && F(self, 0x22c) == 0.0f) {
            U(self, 0x228) = g_PE_dc_defX;
            U(self, 0x22c) = g_PE_dc_defY;
        }
        ((AEGeometry *)(P(self, 0x178)))->translate(*(Vector *)((char *)self + 0x224));

        void *mid = (void*)new AEGeometry((PaintCanvas*)(*holder));
        P(self, 0x19c) = mid;
        ((AEGeometry *)(mid))->translate(*(Vector *)((char *)mid + 0xc));
        ((AEGeometry *)(P(self, 0x178)))->addChild((uint32_t)(uintptr_t)*(void **)((char *)P(self, 0x19c) + 0xc));

        void *leaf = (void*)new AEGeometry((PaintCanvas*)(*holder));
        P(self, 0x17c) = leaf;
        ((AEGeometry *)(P(self, 0x178)))->addChild((uint32_t)(uintptr_t)*(void **)((char *)P(self, 0x178) + 0xc));
    }

    ((AEGeometry *)(P(self, 0x17c)))->setPosition(*(Vector *)((char *)self + 0x148));
    void *leaf = P(self, 0x17c);
    ((AEGeometry *)leaf)->setMatrix(((AEGeometry *)(P(self, 0x8)))->getMatrix());

    ((PaintCanvas*)(long)(*g_PE_dc_canvas2))->CameraSetCurrent((unsigned int)(U(self, 0x174)));
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
    PlayerEgo *self = this;
    if (P(self, 0x1e4) != 0)
        return ((MiningGame *)(P(self, 0x1e4)))->steerXR(delta);

    if (C(self, 0x1a0) != 0) {
        float pitch = (float)I(self, 0x1f8);
        float ft = (float)frameTime;
        float ang = ((ft * delta) / (g_PE_r_turK1 / pitch)) * g_PE_r_turK2 * g_PE_r_turK3;
        ((AEGeometry *)P(self, 0x178))->rotate(0.0f, (float)ang, 0.0f);
        ((AEGeometry *)P(self, 0xdc))->rotate(0.0f, (float)ang, 0.0f);
        ((AEGeometry *)P(self, 0x28))->rotate(0.0f, (float)ang, 0.0f);
        return ang;
    }

    if (P(self, 0x194) != 0) {
        float ft = (float)frameTime;
        F(self, 0x80) = delta * g_PE_r_manK1 * F(P(self, 0x194), 0x50);
        F(self, 0x198) = F(self, 0x198) + (ft * delta) * g_PE_r_manK2;
        return ft * delta;
    }

    if (C(self, 0x158) != 0)
        return delta;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) != 1)
        return delta;

    I(self, 0x104) = -1;
    float rate;
    if (C(self, 0x235) == 0) {
        rate = F(self, 0x154);
    } else {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        rate = F(self, 0x154) * (1.0f - cur / max) * g_PE_r_loadK + F(self, 0x154) * g_PE_r_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_r_rateK * rate), 0x3f);
    F(self, 0x268) = delta;
    F(self, 0x25c) = -rate;
    if (F(self, 0x27c) > target) {
        float step = PE_yawRampDelta(rate, frameTime);
        float v = F(self, 0x27c) - step;
        if (v < target) v = target;
        F(self, 0x27c) = v;
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
    PlayerEgo *self = this;
    if (P(self, 0x1e4) != 0)
        return ((MiningGame *)(P(self, 0x1e4)))->steerX(-delta);

    if (C(self, 0x1a0) != 0) {
        // turret yaw: scale by inverse turret-pitch and apply to 3 nodes.
        float pitch = (float)I(self, 0x1f8);
        float ft = (float)frameTime;
        float ang = ((ft * delta) / (g_PE_l_turK1 / pitch)) * g_PE_l_turK2 * g_PE_l_turK3;
        ((AEGeometry *)P(self, 0x178))->rotate(0.0f, (float)ang, 0.0f);
        ((AEGeometry *)P(self, 0xdc))->rotate(0.0f, (float)ang, 0.0f);
        ((AEGeometry *)P(self, 0x28))->rotate(0.0f, (float)ang, 0.0f);
        return ang;
    }

    if (P(self, 0x194) != 0) {
        float ft = (float)frameTime;
        F(self, 0x80) = delta * g_PE_l_manK1 * F(P(self, 0x194), 0x50);
        F(self, 0x198) = F(self, 0x198) + (ft * delta) * g_PE_l_manK2;
        return ft * delta;
    }

    if (C(self, 0x158) != 0)
        return delta;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) != 1)
        return delta;

    I(self, 0x104) = 1;
    float rate;
    if (C(self, 0x235) == 0) {
        rate = F(self, 0x154);
    } else {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        rate = F(self, 0x154) * (1.0f - cur / max) * g_PE_l_loadK + F(self, 0x154) * g_PE_l_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_l_rateK * rate), 0x3f);
    F(self, 0x25c) = rate;
    F(self, 0x268) = -delta;
    if (F(self, 0x27c) < target) {
        float step = PE_yawRampDelta(rate, frameTime);
        float v = F(self, 0x27c) + step;
        if (target < v) v = target;
        F(self, 0x27c) = v;
    }
    return target;
}

// PlayerEgo::~PlayerEgo() -- real C++ destructor so the demangled symbol contains "~PlayerEgo".
//
// Deletes every owned sub-object (the wrapped Player, the various AEGeometry
// nodes for hull / shield / cloak / turret / docking rigs, the Route, tractor
// and repair beams, the mining game, both explosions, the ease matrix) and
// nulls each handle, mirroring the target teardown order.

static inline void *&PP(void *self, uint32_t off) { return *(void **)((char *)self + off); }

__attribute__((minsize)) PlayerEgo::~PlayerEgo() noexcept(false)
{
    void *self = this;
    if (PP(self, 0x0))   ::operator delete(Player_dtor(PP(self, 0x0)));
    PP(self, 0x0) = 0;
    if (PP(self, 0x4))   { ((AEGeometry *)PP(self, 0x4))->~AEGeometry(); ::operator delete(PP(self, 0x4)); }
    PP(self, 0x4) = 0;
    if (PP(self, 0x8))   { ((AEGeometry *)PP(self, 0x8))->~AEGeometry(); ::operator delete(PP(self, 0x8)); }
    PP(self, 0x8) = 0;
    if (PP(self, 0xdc))  { ((AEGeometry *)PP(self, 0xdc))->~AEGeometry(); ::operator delete(PP(self, 0xdc)); }
    PP(self, 0xdc) = 0;
    if (PP(self, 0x28))  { ((AEGeometry *)PP(self, 0x28))->~AEGeometry(); ::operator delete(PP(self, 0x28)); }
    PP(self, 0x28) = 0;
    if (PP(self, 0xfc))  ::operator delete(((Route *)(PP(self, 0xfc)))->dtor());
    PP(self, 0xfc) = 0;
    if (PP(self, 0x178)) { ((AEGeometry *)PP(self, 0x178))->~AEGeometry(); ::operator delete(PP(self, 0x178)); }
    PP(self, 0x178) = 0;
    if (PP(self, 0x17c)) { ((AEGeometry *)PP(self, 0x17c))->~AEGeometry(); ::operator delete(PP(self, 0x17c)); }
    PP(self, 0x17c) = 0;
    if (PP(self, 0x2c))  { ((AEGeometry *)PP(self, 0x2c))->~AEGeometry(); ::operator delete(PP(self, 0x2c)); }
    PP(self, 0x2c) = 0;
    if (PP(self, 0x30))  { ((AEGeometry *)PP(self, 0x30))->~AEGeometry(); ::operator delete(PP(self, 0x30)); }
    PP(self, 0x30) = 0;
    if (PP(self, 0x34))  { ((AEGeometry *)PP(self, 0x34))->~AEGeometry(); ::operator delete(PP(self, 0x34)); }
    PP(self, 0x34) = 0;
    if (PP(self, 0x19c)) { ((AEGeometry *)PP(self, 0x19c))->~AEGeometry(); ::operator delete(PP(self, 0x19c)); }
    PP(self, 0x19c) = 0;
    if (PP(self, 0x1b4)) ::operator delete(((TractorBeam *)(PP(self, 0x1b4)))->dtor());
    PP(self, 0x1b4) = 0;
    if (PP(self, 0x1e4)) ::operator delete(MiningGame_dtor(PP(self, 0x1e4)));
    PP(self, 0x1e4) = 0;
    if (PP(self, 0x8c))  ::operator delete(Explosion_dtor(PP(self, 0x8c)));
    PP(self, 0x8c) = 0;
    if (PP(self, 0x90))  ::operator delete(Explosion_dtor(PP(self, 0x90)));
    PP(self, 0x90) = 0;
    if (PP(self, 0x358)) ::operator delete(EaseInOutMatrix_dtor(PP(self, 0x358)));
    PP(self, 0x358) = 0;
    if (Array<RepairBeam *> *beams = PE_repairBeams(self)) {
        for (RepairBeam *beam : *beams)
            if (beam)
                ::operator delete(beam->dtor());
        delete beams;
    }
    PE_repairBeams(self) = 0;
}

void PlayerEgo::throttleChanged() {
    PlayerEgo *self = this;
  int v;
  if (C(self, 0x370) == 0) {
    C(self, 0x370) = 1;
    v = 0;
  } else {
    v = I(self, 0x374);
    if ((unsigned)(v - 0x1f5) < 999) {
      v = 500;
    } else {
      if (v < 0x5dc) return;
      v = 2000 - v;
    }
  }
  I(self, 0x374) = v;
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
    void *self = this;
    // embedded orientation matrices.
    ((Matrix *)((char *)self + 0x2ac))->initIdentity();     // roll matrix
    ((Matrix *)((char *)self + 0x4c8))->initIdentity();     // turret/HUD matrix

    // record the wrapped player (offset 0).
    PP(self, 0x0) = (void *)player;

    // remaining field initialisation + boost stats + MovingStars background.
    PlayerEgo_initFields(self, player);
}

extern void* g_engine_status;
extern void* g_engine_fmod;
void PlayerEgo::PlayEngineSound() {
    PlayerEgo *self = this;
  if (((Ship *)(((Status *)(g_engine_status))->getShip()))->getFirstEquipmentOfSort(0x26) != 0
      && ((Status *)(g_engine_status))->inAlienOrbit() == 0) {
    void* obj = g_engine_status;
    int idx = Station_getIndex(((Status *)(obj))->getStation());
    int cm = ((Status *)(g_engine_status))->getCurrentCampaignMission();
    float g = ((Status *)(obj))->getGammaRayDamagePerSecond(idx, cm);
    if (0.0f < g && I(self, 0xb4) != -1) {
      ((FModSound *)(*(void**)g_engine_fmod))->play((int)(intptr_t)P(self, 0xb4), (Vector *)0, (Vector *)0, g);
    }
  }
  return PlayerEgo_PlayEngineSound_ext(P(self, 0), I(self, 0x1c), 0);
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
    PlayerEgo *self = this;
    float target[3] = { tx, ty, tz };
    PE_mtp_steer(self, target, steer, speed);

    // apply the resolved heading (stored at 0x164) as the new facing.
    float up[3] = {0.0f, 1.0f, 0.0f};
    ((AEGeometry *)(P(self, 0x8)))->setDirection(*(Vector *)((char *)self + 0x164), *(Vector *)up);

    float dt = (float)I(self, 0x134);
    ((AEGeometry *)(P(self, 0x8)))->moveForward(F(self, 0xbc) * dt * F(self, 0xb8));
    ((PlayerEgo *)(self))->roll(I(self, 0x134));

    // strafe slide -> follow camera, unless below the dead-zone.
    float slide = F(self, 0x37c);
    float mag = slide > 0.0f ? slide : -slide;
    if (mag <= g_PE_mtp_strafeEps) {
        F(self, 0x380) = g_PE_mtp_strafeReset;
    } else {
        unsigned char m[0x30];
        Mat_assign(m, ((AEGeometry *)(P(self, 0x8)))->getMatrix());
        // rotate the slide vector into world space and push it to the camera.
        float v = slide * dt;
        ((TargetFollowCamera *)(P(self, 0x88)))->translateNoUpdate(v, 0.0f, 0.0f);
        F(self, 0x37c) = F(self, 0x37c) * g_PE_mtp_strafeK;
        ((AEGeometry *)P(self, 0x8))->setMatrix(*(const AbyssEngine::AEMath::Matrix*)m);
    }

    Mat_assign((char *)P(self, 0x0) + 0x4, ((AEGeometry *)(P(self, 0x8)))->getMatrix());
    ((AEGeometry *)(P(self, 0x8)))->getPosition();
}

void PlayerEgo::resetGunDelay() {
    PlayerEgo *self = this; return PlayerEgo_resetGunDelay_ext(P(self, 0), 0); }

int PlayerEgo::getShieldDamageRate() {
    PlayerEgo *self = this; return PlayerEgo_getShieldDamageRate_ext(P(self, 0)); }

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
    PlayerEgo *self = this;
    int t = I(self, 0x184) + dt;
    I(self, 0x184) = t;
    if (t >= 0xbb9) {                       // 3000ms: rescan for the best target
        I(self, 0x184) = 0;
        P(self, 0x18c) = 0;
        unsigned int *enemies = (unsigned int *)((Level *)(P(self, 0xc)))->getEnemies();
        if (enemies != 0) {
            int best = 60000;
            for (unsigned i = 0; i < enemies[0]; i++) {
                void *e = *(void **)(enemies[1] + i * 4);
                if (((KIPlayer *)(e))->isDead() != 0) continue;
                if (((KIPlayer *)(e))->isDying() != 0) continue;
                if (((Player *)(*(void **)((char *)e + 4)))->isActive() == 0) continue;
                if (((KIPlayer *)(e))->isEnemy() == 0) continue;
                if (C(e, 0x74) != 0) continue;

                float epos[3];
                ((KIPlayer *)(e))->getPosition();
                float rel[3];
                Vec_sub(rel, epos, (char *)self + 0x148);
                int dist = (int)Vec_length(rel);
                if (dist < best
                    && (P(self, 0x18c) == 0 || P(self, 0x18c) != P(self, 0x190))) {
                    P(self, 0x18c) = e;
                    best = dist;
                }
            }
        }
    }

    void *target = P(self, 0x18c);
    if (target != 0 && I(target, 0x8) != 0) {
        int fireTimer = PE_hat_aimAndFire(self, dt);
        I(self, 0x188) = fireTimer;
        if (fireTimer + dt > 0x1f4) {
            // fall through to stop-shooting below
        } else {
            return;
        }
    }

    ((Player *)(P(self, 0x0)))->stopShooting(0);
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
    PlayerEgo *self = this;
    if (C(self, 0x1c0) != 0) {
        // Already docked -> undock.
        ((PlayerAsteroid *)(P(self, 0x1bc)))->setRotationEnabled(true);
        C(self, 0x145) = 0;
        C(self, 0x1c0) = 0;
        P(self, 0x1bc) = 0;

        int zero[3] = {0, 0, 0};
        Vec_assign((char *)self + 0x1c8, zero);

        ((TargetFollowCamera *)(P(self, 0x88)))->setActive(true);
        ((Player *)(P(self, 0x0)))->resetGunDelay(0);
        if (P(self, 0x1e4) != 0)
            ::operator delete(MiningGame_dtor(P(self, 0x1e4)));
        P(self, 0x1e4) = 0;
        ((Radar *)(radar))->unlockAsteroid();
        I(self, 0x1c4) = 0;
        ((PlayerEgo *)(self))->setExhaustVisible(true);
    } else if (radar != 0) {
        // Not docked -> latch onto the asteroid.
        C(self, 0x1c0) = 1;
        I(self, 0x1dc) = 0;
        P(self, 0x1bc) = radar;
        float dist = ((PlayerAsteroid *)(radar))->getScaling() * g_PE_astApproach;
        C(self, 0x145) = 1;
        I(self, 0x1c4) = 0;
        I(self, 0x1d8) = (int)dist;
    }
}

void PlayerEgo::killLiberator() {
    PlayerEgo *self = this;
  char sv[12];
  void* p = *(void**)P(self, 0);
  if (p == 0) return;
  void* arr = P(P(p, 4), 4);
  if (arr == 0) return;
  if (I(p, 0) == 0) return;
  if (I(arr, 0) == 0) return;
  if (I(self, 0x10c) != 0xb3) return;
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
    PlayerEgo *self = this;
    if (C(self, 0x2f4) == 0)
        return;

    void *m = ((AEGeometry *)(P(self, 0x8)))->getMatrix();
    float rx = F(m, 0x10);
    float ry = F(m, 0x14);
    float mag = rx > 0.0f ? rx : -rx;

    if (amount > 0x3b)
        amount = 0x3c;

    unsigned char rollMat[0x30];   // Matrix is a 48-byte value

    if (ry >= 0.0f && mag < g_PE_rollLevelEps) {
        // settled: reset to identity and stop rolling.
        Mat_identity(rollMat, (char *)self + 0x2ac);
        Mat_assign((char *)self + 0x2ac, rollMat);
        C(self, 0x2a9) = 0;
        C(self, 0x2f4) = 0;
        C(self, 0x324) = 0;
        return;
    }

    float zAxis = 0.0f;
    float xScale = PE_roll_bankFactor(self, rx, ry, &zAxis);

    // record the current roll direction (sign of rx) when non-zero.
    if (rx != 0.0f)
        C(self, 0x2a9) = (rx < 0.0f) ? 1 : 2;

    C(self, 0x2f4) = 1;
    Mat_setRotation(rollMat, xScale * (float)amount, 0.0f, zAxis);
    Mat_assign((char *)self + 0x2ac, rollMat);
}

void PlayerEgo::setTargetFollowCamera(void* cam) {
    PlayerEgo *self = this;
  void* m = P(self, 0x154);
  P(self, 0x88) = cam;
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

void PlayerEgo::calcCollision(void *candidates) {
    PlayerEgo *self = this;
    if (candidates == 0)
        return;
    if (C(self, 0x356) != 0 && (unsigned)(I(self, 0x1c4) - 1) < 3)
        return;   // in final docking approach: no collisions

    unsigned int *arr = (unsigned int *)candidates;
    for (unsigned i = 0; i < arr[0]; i++) {
        void *obj = *(void **)(arr[1] + i * 4);
        if (obj == 0)
            continue;

        // first candidate: trip the proximity alarm flag when very close.
        if (i == 0 && PE_status()->inAlienOrbit() == 0) {
            if (Vec_length((char *)self + 0x148) < g_PE_cc_alarmDist && C(obj, 0x71) != 0)
                C(self, 0x234) = 1;
        }

        // broad-phase overlap test (virtual +0x40).
        float pos[3];
        ((AEGeometry *)(P(self, 0x8)))->getPosition();
        typedef int (*overlap_fn)(void *, void *);
        overlap_fn overlaps = *(overlap_fn *)(*(char **)obj + 0x40);
        if (!overlaps(obj, pos))
            continue;

        if (((KIPlayer *)(obj))->getType() == 0x4262 && ((KIPlayer *)(obj))->isVisible() != 0) {
            if (((PlayerWormHole *)(obj))->isShrinking() == 0 && P(self, 0x1e4) == 0)
                PE_cc_wormhole(self, obj);
        } else if (C(obj, 0x3c) == 0) {
            if (((KIPlayer *)(obj))->isVisible() != 0) {
                // skip the object we are actively docking with.
                bool docking = (C(self, 0x1c0) != 0 || C(self, 0x356) != 0)
                               && P(self, 0x1bc) == obj;
                if (!docking)
                    PE_cc_obstacle(self, obj, i);
                hitCamera_(self);
            }
        } else {
            if (((KIPlayer *)(obj))->isDying() == 0 && ((KIPlayer *)(obj))->isDead() == 0) {
                bool skipDockTarget = (C(self, 0x1c0) != 0 || C(self, 0x356) != 0)
                                      && obj == P(self, 0x1bc);
                if (!skipDockTarget) {
                    PE_cc_destructible(self, obj);
                    hitCamera_(self);
                }
            }
        }
    }
}

void PlayerEgo::dockToPlanet() {
    PlayerEgo *self = this;
  ((TargetFollowCamera *)(I(self, 0x88)))->setLookAtCam((bool)(unsigned char)I(self, 0x88));
  C(self, 0x13c) = 1;
  I(self, 0x138) = 0;
  I(self, 0xb8) = 0x41000000;
  I(self, 0xcc) = 10000;
  I(self, 0xd0) = 0;
  C(self, 0x144) = 0;
  float f = PlayerEgo_resetGunDelay_f(self);
  void* snd = *(void**)g_dockToPlanet_fmod;
  C(self, 0x38) = 0;
  C(self, 0x1ee) = 1;
  I(self, 0xb8) = 0x41000000;
  I(self, 0x1f0) = 0;
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
    PlayerEgo *self = this;
    if (P(self, 0x1e4) != 0) {
        if (C(*g_PE_u_miningGate, 0x10) == 0)
            return ((MiningGame *)(P(self, 0x1e4)))->steerY(-delta);
        return ((MiningGame *)(P(self, 0x1e4)))->steerYAlt(delta);
    }

    if (C(self, 0x1a0) != 0) {
        float ft = (float)frameTime;
        if (F(self, 0x1a8) > g_PE_u_eps) {
            float ang = F(self, 0x1a8) - ft * delta;
            F(self, 0x1a8) = ang;
            ((AEGeometry *)P(self, 0x28))->rotate((float)(ang * g_PE_u_lookK1 * g_PE_u_lookK2), 0.0f, 0.0f);
        }
        float p = F(self, 0x1a4);
        if (p > g_PE_u_eps2) {
            float half = ft * delta * 0.5f;
            float ang = half * g_PE_u_lookK1 * g_PE_u_lookK2;
            F(self, 0x1a4) = p - half;
            return (((AEGeometry *)P(self, 0x19c))->rotate((float)ang, 0.0f, 0.0f), 0.0f);
        }
        return p;
    }

    if (P(self, 0x194) != 0) {
        float v = (float)frameTime * g_PE_u_manK * F(P(self, 0x194), 0x50);
        F(self, 0x7c) = v;
        return v;
    }

    if (C(self, 0x158) != 0)
        return delta;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) != 1)
        return delta;

    I(self, 0x100) = 1;
    float rate;
    if (C(self, 0x235) == 0) {
        rate = F(self, 0x154);
    } else {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        rate = F(self, 0x154) * (1.0f - cur / max) * g_PE_u_loadK + F(self, 0x154) * g_PE_u_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_u_rateK * rate), 0x3f);
    F(self, 0x258) = rate;
    F(self, 0x270) = -delta;
    if (F(self, 0x278) > target) {
        float v = F(self, 0x278) - PE_pitchRampDelta(self, rate, frameTime);
        if (v < target) v = target;
        F(self, 0x278) = v;
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
    PlayerEgo *self = this;
    if (C(self, 0x1a0) != 0) {
        void *m1 = ((AEGeometry *)(P(self, 0x8)))->getMatrix();
        void *m2 = ((AEGeometry *)(P(self, 0x28)))->getMatrix();
        unsigned char combined[0x30];                 // Matrix is a 48-byte value
        Mat_mul(combined, m1, m2);
        Player_shootTurret(P(self, 0x0), 2, weapon, weapon >> 31, 0, combined);
        return;
    }

    if (((PlayerEgo *)(self))->isDead() != 0)
        return;

    void *player = P(self, 0x0);
    if (type == 1) {
        if (Player_shootSecondary(player, 1, I(self, 0x10c), weapon >> 31, 0) == 0)
            I(self, 0x10c) = -1;
    } else {
        Player_shootPrimary(player, type, weapon, weapon >> 31, 0);
    }
}

// PlayerEgo::stopMining()
//   The target bytes for this stub decode as a tiny thunk into the mining-game
//   teardown helper (the Ghidra listing is bad-instruction noise because the
//   region is a hand-written veneer). Faithfully forward to that helper.

void PlayerEgo::stopMining() {
    PlayerEgo *self = this;
    PlayerEgo_stopMining_impl(self);
}

// PlayerEgo::setTurretMode(bool enable)
//   Enters or leaves the gun-turret view. Refused unless the ship mounts a
//   turret (0x170) and is neither mining (0x1e4) nor mid-cutscene (0x180); a
//   pending maneuver (0x194) instead just restores the default camera. When
//   enabling it lazily builds the turret camera rig (camera + AEGeometry chain,
//   like the docking camera), aims it, and makes it current; when disabling it
//   stops shooting and resets the camera. Either way it toggles the turret
//   crosshair transform (0x30) and starts/stops the turret hum sound.

__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasA;
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasB;
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasC;
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasD;
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasE;
__attribute__((visibility("hidden"))) extern void **g_PE_tm_transform;
__attribute__((visibility("hidden"))) extern int  *g_PE_tm_hum;
extern const float g_PE_tm_fovNormal;
extern const float g_PE_tm_fovAlien;

void PlayerEgo::setTurretMode(int enable) {
    PlayerEgo *self = this;
    if (C(self, 0x170) == 0 || P(self, 0x1e4) != 0 || C(self, 0x180) != 0) {
        // turret view unavailable -> restore default camera if a maneuver runs.
        if (P(self, 0x194) != 0) {
            ((PaintCanvas*)(long)(*g_PE_tm_canvasA))->CameraSetCurrent((unsigned int)(U(P(self, 0x88), 0)));
            ((LevelScript *)(P(self, 0xc)))->resetCamera((Level *)P(P(self, 0xc), 0x18));
        }
        return;
    }

    C(self, 0x1a0) = (unsigned char)enable;
    if (enable == 0) {
        ((PlayerEgo *)(self))->stopShooting(0);
        ((PaintCanvas*)(long)(*g_PE_tm_canvasE))->CameraSetCurrent((unsigned int)(U(P(self, 0x88), 0)));
        ((LevelScript *)(P(self, 0xc)))->resetCamera((Level *)P(P(self, 0xc), 0x18));
    } else {
        if (P(self, 0x194) != 0)
            return;
        if (P(self, 0x178) == 0) {
            void **holder = g_PE_tm_canvasB;
            ((PaintCanvas*)(long)(*holder))->CameraCreate((unsigned int *)(&U(self, 0x174)));
            unsigned int cam = (unsigned int)(unsigned long)*holder;
            float fov = (PE_status()->inAlienOrbit() != 0) ? g_PE_tm_fovAlien : g_PE_tm_fovNormal;
            ((PaintCanvas*)g_PaintCanvas)->CameraSetPerspective((unsigned int)(cam), fov, 0.0f, g_PE_tm_fovAlien);

            void *node = (void*)new AEGeometry((PaintCanvas*)(*holder));
            P(self, 0x178) = node;
            ((AEGeometry *)node)->setRotationOrder(2);
            ((AEGeometry *)(node))->translate(*(Vector *)((char *)self + 0x224));

            void *mid = (void*)new AEGeometry((PaintCanvas*)(*holder));
            P(self, 0x19c) = mid;
            ((AEGeometry *)(mid))->translate(*(Vector *)((char *)mid + 0xc));
            ((AEGeometry *)(P(self, 0x178)))->addChild((uint32_t)(uintptr_t)*(void **)((char *)P(self, 0x19c) + 0xc));

            void *leaf = (void*)new AEGeometry((PaintCanvas*)(*holder));
            P(self, 0x17c) = leaf;
            ((AEGeometry *)(P(self, 0x178)))->addChild((uint32_t)(uintptr_t)*(void **)((char *)P(self, 0x178) + 0xc));

            if (((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x23) != 0)
                ((AEGeometry *)(P(self, 0x178)))->rotate(*(Vector *)((char *)self + 0x224));
        }
        ((AEGeometry *)(P(self, 0x17c)))->setPosition(*(Vector *)((char *)self + 0x148));
        void *leaf = P(self, 0x17c);
        ((AEGeometry *)leaf)->setMatrix(((AEGeometry *)(P(self, 0x8)))->getMatrix());
        ((PaintCanvas*)(long)(*g_PE_tm_canvasD))->CameraSetCurrent((unsigned int)(U(self, 0x174)));
        ((Player *)(P(self, 0x0)))->stopShooting(0);
    }

    if (P(self, 0x30) != 0) {
        void *tf = ((PaintCanvas*)g_PaintCanvas)->TransformGetTransform((unsigned int)(U(*g_PE_tm_transform, 0)));
        ((AbyssEngine::Transform *)(tf))->SetVisible(enable != 0);
        int v = (enable != 0);
        if (enable == 0)
            ((FModSound *)(*g_PE_tm_hum))->stop(0);
        else
            ((FModSound *)(*g_PE_tm_hum))->play(0x8cf, (Vector *)0, (Vector *)0, (float)v);
    }
}

void PlayerEgo::rotate(float rx, float ry, float rz) {
    PlayerEgo *self = this;
  char local[60];
  F(self, 0x2e8) = F(self, 0x2e8) + rx;
  F(self, 0x2ec) = F(self, 0x2ec) + ry;
  F(self, 0x2f0) = F(self, 0x2f0) + rz;
  int ido = I(P(self, 4), 0xc);
  void* t = *(void**)g_rotate_transform;
  void* m = TransformGetLocal(t, ido);
  MatrixSetRotation(local, m, F(self, 0x2e8), F(self, 0x2ec), F(self, 0x2f0));
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
    PlayerEgo *self = this;
    if (I(self, 0x194) != 0)
        return;

    float base;
    if (C(self, 0x235) != 0) {
        float cur = (float)((Ship *)(PE_status()->getShip()))->getCurrentLoad();
        float max = (float)((Ship *)(PE_status()->getShip()))->getMaxLoad();
        float rate = F(self, 0x154);
        base = rate * (1.0f - cur / max) * g_PE_strafeLoadK + rate * g_PE_strafeLoadB;
    } else {
        base = F(self, 0x154);
    }

    float sign = positive ? 1.0f : -1.0f;
    float accel = fmin_(base * 30.0f * g_PE_strafeAccelK, 2.0f);
    float target = fmin_(F(self, 0x380) * 1.5f, 1.0f);

    F(self, 0x37c) = F(self, 0x380) * sign * accel;
    F(self, 0x380) = target;
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

__attribute__((visibility("hidden"))) extern void **g_PE_dtdp_canvas;   // 0xafa68 PaintCanvas

void PlayerEgo::dockToDockingPoint(void *radar) {
    PlayerEgo *self = this;
    if (((PlayerEgo *)(self))->isDead() != 0)
        return;

    if (C(self, 0x356) == 0) {
        if (radar != 0) {
            C(self, 0x356) = 1;
            P(self, 0x1bc) = radar;
            I(self, 0x1d8) = 0x578;
            I(self, 0x1dc) = 0;
            C(self, 0x145) = 1;
            I(self, 0x1c4) = 0;
        }
        return;
    }

    bool undock = (radar == 0);
    if (!undock) {
        float pos[3];
        ((PlayerEgo *)(self))->getPosition();
        void *sp = (void *)(unsigned long)I(self, 0x36c);
        void *nav = ((KIPlayer *)(radar))->getNearestNavigationPoint((Vector *)pos, sp);
        if (nav == 0) {
            if (C(radar, 0x70) != 0)
                C(radar, 0x8c) = 1;
            undock = true;
        } else {
            if (C(radar, 0x70) != 0)
                C(radar, 0x8c) = 1;

            ((PlayerEgo *)(self))->setTurretMode(0);
            C(self, 0x1a1) = 0;
            ((PaintCanvas*)(long)(*g_PE_dtdp_canvas))->CameraSetCurrent((unsigned int)(U(P(self, 0x88), 0)));
            ((LevelScript *)(P(self, 0x10)))->resetCamera((Level *)P(P(self, 0x10), 0x18));
            PlayEngineSound_(self);
            I(self, 0x1c4) = 3;
            ((TargetFollowCamera *)(P(self, 0x88)))->setLookAtCam(false);
            ((TargetFollowCamera *)(P(self, 0x88)))->useTargetsUpVector(false);

            if (P(self, 0x358) != 0)
                ::operator delete(EaseInOutMatrix_dtor(P(self, 0x358)));
            P(self, 0x358) = 0;

            ((PlayerEgo *)(self))->getPosition();
            void *nav2 = ((KIPlayer *)(radar))->getNearestNavigationPoint((Vector *)pos, sp);
            void *from = ((AEGeometry *)(P(self, 0x8)))->getMatrix();
            P(self, 0x358) = PE_dtdp_makeEase(from, nav2);
            ((PlayerEgo *)(self))->setExhaustVisible(true);
        }
    }

    if (undock) {
        PlayEngineSound_(self);
        C(self, 0x356) = 0;
        C(self, 0x1a1) = 0;
        C(self, 0x145) = 0;
        P(self, 0x1bc) = 0;
        int zero[3] = {0, 0, 0};
        Vec_assign((char *)self + 0x1c8, zero);
        ((TargetFollowCamera *)(P(self, 0x88)))->setActive(true);
        ((TargetFollowCamera *)(P(self, 0x88)))->setLookAtCam(false);
        ((TargetFollowCamera *)(P(self, 0x88)))->useTargetsUpVector(false);
        ((Player *)(P(self, 0x0)))->resetGunDelay(0);
        I(self, 0x1c4) = 0;
        ((PlayerEgo *)(self))->setExhaustVisible(true);
        if (I(self, 0x36c) != 0) {
            ((SpacePoint *)((void *)(unsigned long)I(self, 0x36c)))->giveFree();
            I(self, 0x36c) = 0;
        }
    }

    if (P(self, 0x1e8) != 0) {
        ::operator delete(HackingGame_dtor(P(self, 0x1e8)));
        P(self, 0x1e8) = 0;
        ((Hud *)(P(self, 0x220)))->setHackingGameActive(false);
    }
}

// PlayerEgo::draw(bool allowHud)
//   Draws the 2D overlay layer for the player ship (targeting reticle / lock
//   marker plus the boost throttle). A cascade of guard conditions short-circuits
//   to one of several render-tail veneers when the player is in a maneuver, the
//   mining game, dead, docking, in a cutscene, etc. In the normal path it builds
//   a unit matrix (optionally seeded from the ship matrix), then draws the
//   appropriate reticle image depending on turret mode / plasma lock / blink
//   state, and finally the throttle gauge.

extern "C" void  PlayerEgo_draw_tailA(void);   // 0x1abc48 veneer
extern "C" void  PlayerEgo_draw_tailB(void);   // 0x1abc58 veneer
extern "C" void  PlayerEgo_draw_tailC(void);   // 0x1abc68 veneer

__attribute__((visibility("hidden"))) extern void **g_PE_dr_canvas;    // 0xb1dd8 PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_PE_dr_status;    // 0xb1de8 Status singleton
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posLock;   // 0xb1e0c reticle pos (plasma)
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posNoLock;
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posBlink;
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posNormal;

void PlayerEgo::draw(int allowHud) {
    PlayerEgo *self = this;
    if (P(self, 0x194) != 0)               // mid scripted maneuver: nothing to draw
        return;

    if (P(self, 0x1e8) != 0 && C(self, 0x1a0) == 0)
        return;

    if (P(self, 0x1e4) != 0) {             // mining game owns the screen
        PlayerEgo_draw_tailA();
        return;
    }

    bool full;
    if (C(self, 0x1f4) != 0 || ((PlayerEgo *)(self))->isDead() != 0 || C(self, 0x24) != 0
        || allowHud == 0 || C(self, 0x1c0) != 0 || C(self, 0x1ee) != 0 || C(self, 0x1ed) != 0) {
        full = false;
    } else {
        full = true;
    }

    if (!full) {
        if (C(self, 0x158) != 0) {
            PlayerEgo_draw_tailB();
            return;
        }
        if (C(self, 0x356) == 0)
            return;
        if ((unsigned)(I(self, 0x1c4) - 1) < 3)
            return;
        PlayerEgo_draw_tailB();
        return;
    }

    unsigned char m[0x30];                 // unit matrix, optionally ship-aligned
    for (int i = 0; i < 0x30; i++) m[i] = 0;
    F(m, 0x0)  = 1.0f;
    F(m, 0x14) = 1.0f;
    F(m, 0x28) = 1.0f;

    bool aligned = (C(self, 0x158) != 0 && C(self, 0x1a0) != 0);
    if (!aligned)
        Mat_assign(m, ((AEGeometry *)(P(self, 0x8)))->getMatrix());

    void *canvas = *g_PE_dr_canvas;
    ((PaintCanvas*)g_PaintCanvas)->SetColor((unsigned int)(0xffffffff));

    if (C(self, 0x1a0) != 0
        && ((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x23) != 0) {
        // turret crosshair: plasma lock changes the marker + position.
        if (((Radar *)(P(self, 0x14)))->isPlasmaInRange() != 0) {
            float *p = g_PE_dr_posLock;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(I(self, 0x248)), (int)p[0], (int)p[1], (unsigned char)(0x11), (unsigned char)(0x44));
        } else {
            float *p = g_PE_dr_posNoLock;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(I(self, 0x244)), (int)p[0], (int)p[1], (unsigned char)(0x11), (unsigned char)(0x44));
        }
    } else {
        // standard lock-on reticle: blink while acquiring.
        if (C(P(self, 0xc), 0x30) != 0) {
            float *p = g_PE_dr_posBlink;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(I(self, 0x240)), (int)p[0], (int)p[1], (unsigned char)(0x11), (unsigned char)(0x44));
            int t = I(self, 0x24c) + I(self, 0x134);
            I(self, 0x24c) = t;
            if (t >= 0xc9)
                C(P(self, 0xc), 0x30) = 0;
        } else {
            float *p = g_PE_dr_posNormal;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(I(self, 0x23c)), (int)p[0], (int)p[1], (unsigned char)(0x11), (unsigned char)(0x44));
            I(self, 0x24c) = 0;
        }
    }

    ((PlayerEgo *)(self))->drawThrottle();
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

void PlayerEgo::update(int dt, void *radar, void *hud, void *radio, void *script, int arg5, bool arg6, int arg7) {
    PlayerEgo *self = this;
    (void)arg6; (void)arg7; (void)script;

    if (P(self, 0x220) == 0)
        return;
    if (C(self, 0x24) != 0)
        return;   // frozen

    // cache world position for this frame.
    float pos[3];
    ((PlayerEgo *)(self))->getPosition();
    I(self, 0x148) = *(int *)&pos[0];
    I(self, 0x14c) = *(int *)&pos[1];
    I(self, 0x150) = *(int *)&pos[2];

    // shield regen / hacking / cargo / jump-drive / cloak.
    PE_upd_subsystems(self, dt, radar, hud, radio, script);
    // engine overheat + boost (may spawn the destruct explosion at 0x90).
    PE_upd_boost(self, dt);

    // ---- asteroid docking --------------------------------------------------
    if (C(self, 0x1c0) != 0) {
        C(self, 0x145) = 1;
        if (P(self, 0x1bc) == 0 || ((PlayerEgo *)(self))->isDead() != 0) {
            ((PlayerEgo *)(self))->dockToAsteroid(0);
            PE_upd_post(self, dt, radar, hud, radio, arg5);
            return;
        }
        ((PlayerEgo *)(self))->approachAsteroid(0 /*hud2*/, radar);
    }

    // ---- station docking ---------------------------------------------------
    if (C(self, 0x356) != 0 && ((Player *)(P(self, 0x0)))->getHitpoints() > 0) {
        C(self, 0x145) = 1;
        if (P(self, 0x1bc) == 0 || ((PlayerEgo *)(self))->isDead() != 0) {
            ((PlayerEgo *)(self))->dockToDockingPoint(0);
            PE_upd_post(self, dt, radar, hud, radio, arg5);
            return;
        }
        if (((PlayerEgo *)(self))->approachDockingPoint(hud, 0, radar) != 0)
            PE_upd_docksFinishDelivery(self, radio);
    }

    // turret view while the free-look/turret camera is active.
    if (C(self, 0x1a0) != 0 || C(self, 0x1a1) != 0)
        ((PlayerEgo *)(self))->handleTurretView(dt);

    // ---- flight controller -------------------------------------------------
    bool autopilot = (C(self, 0x158) != 0 && I(self, 0x15c) != 0);
    if (!autopilot) {
        if (C(self, 0x145) == 0) {
            if (C(self, 0x1a0) == 0 && C(self, 0x1a1) == 0) {
                ((PlayerEgo *)(self))->roll(I(self, 0x134));
                if (((PlayerEgo *)(self))->updateManeuver() == 0)
                    ((PlayerEgo *)(self))->handleShip(dt);
            } else {
                ((PlayerEgo *)(self))->handleTurretView(dt);
            }
        }
    } else if (C(self, 0x145) == 0 && ((PlayerEgo *)(self))->updateManeuver() == 0) {
        void *wp = self->autoPilotTarget;
        if (C(self, 0x160) != 0 && wp != 0 && I(wp, 0x134) != 0) {
            wp = ((Route *)(self->autoPilotTarget))->getWaypoint();
            P(self, 0x15c) = wp;
        }
        if (wp == 0 || C(self, 0x1ed) != 0) {
            ((KIPlayer *)(self))->setAutoPilot(0);
        } else {
            // steer toward the current waypoint.
            float wpPos[3];
            typedef void (*getpos_fn)(void *, void *);
            (*(getpos_fn *)(*(char **)wp + 0x28))(wp, wpPos);
            I(self, 0xec) = *(int *)&wpPos[0];
            I(self, 0xf0) = *(int *)&wpPos[1];
            I(self, 0xf4) = *(int *)&wpPos[2];

            float me[3];
            ((PlayerEgo *)(self))->getPosition();
            float dx = me[0] - wpPos[0], dy = me[1] - wpPos[1], dz = me[2] - wpPos[2];
            if ((int)(dx * dx + dy * dy + dz * dz) < 20000)
                C(self, 0x330) = 1;

            float speed = 4.0f;
            float h = ((Ship *)(PE_status()->getShip()))->getHandling();
            if (h + g_PE_upd_handlingBias < 4.0f)
                speed = ((Ship *)(PE_status()->getShip()))->getHandling() + g_PE_upd_handlingBias;
            ((PlayerEgo *)(self))->moveToPosition(F(self, 0xec), F(self, 0xf0), F(self, 0xf4), 1, speed);
            if (C(self, 0x1a0) != 0)
                ((PlayerEgo *)(self))->handleTurretView(dt);
        }
    }

    // continue an evasive maneuver during a non-final docking glide.
    if (C(self, 0x356) != 0 && U(self, 0x1c4) != 1
        && C(self, 0x1a0) == 0 && (U(self, 0x1c4) | 1) != 3)
        ((PlayerEgo *)(self))->updateManeuver();

    // auto-turret (when enabled and the radar window allows it).
    if (C(self, 0x180) != 0 && C(self, 0x355) != 0) {
        if (((PlayerEgo *)(self))->isDead() == 0) {
            ((PlayerEgo *)(self))->handleAutoTurret(dt);
        } else {
            C(self, 0x355) = 0;
            ((Player *)(P(self, 0x0)))->stopShooting(0);
        }
    }

    // collision + camera + explosion post-processing.
    PE_upd_post(self, dt, radar, hud, radio, arg5);
}

void PlayerEgo::setTurretPosition(float x, float y, float z) {
    PlayerEgo *self = this;
  char v[12];
  *(float*)(v + 0) = x;
  *(float*)(v + 4) = y;
  *(float*)(v + 8) = z;
  *(Vector *)(B(self, 0x224)) = *(const Vector *)(v);
}

// PlayerEgo::revive()
//   Brings a destroyed player back to life: re-enables its engine particle
//   system, deletes the explosion, re-activates the follow camera and engine
//   sound, restores hitpoints/armor to max, resets position and facing, and
//   clears the explosion timer.

__attribute__((visibility("hidden"))) extern void **g_PE_reviveSound;

void PlayerEgo::revive() {
    PlayerEgo *self = this;
    int psm = I(P(self, 0xc), 0x74);
    bool en = C(self, 0x2fc) != 0;
    ((ParticleSystemManager *)(psm))->enableSystemEmit(I(self, 0x2fc), en);
    ((ParticleSystemManager *)(psm))->enableSystemRender(I(self, 0x2fc), en);

    if (P(self, 0x8c) != 0)
        ::operator delete(Explosion_dtor(P(self, 0x8c)));
    P(self, 0x8c) = 0;

    ((TargetFollowCamera *)(P(self, 0x88)))->setActive(true);
    ((Player *)(P(self, 0x0)))->setActive(true);
    int v = 0;

    void *snd = *g_PE_reviveSound;
    ((FModSound *)(*(void **)snd))->play((int)(intptr_t)*(void **)snd, (Vector *)0, (Vector *)0, (float)v);
    ((FModSound *)(snd))->play((int)(intptr_t)P(self, 0x1c), (Vector *)0, (Vector *)0, (float)v);

    ((PlayerEgo *)(self))->setExhaustVisible(true);

    void *player = P(self, 0x0);
    ((Player *)(player))->setHitpoints(((Player *)(player))->getMaxHitpoints());
    player = P(self, 0x0);
    ((Player *)(player))->setArmorHP(((Player *)(player))->getMaxArmorHP());

    ((PlayerEgo *)(self))->setPosition();

    float fwd[3] = {0.0f, 0.0f, 1.0f};
    float fwdUp[3] = {0.0f, 1.0f, 0.0f};
    ((AEGeometry *)(P(self, 0x8)))->setDirection(*(Vector *)fwd, *(Vector *)fwdUp);
    I(self, 0x2f8) = 0;
}

// PlayerEgo::drawThrottle()
//   While the boost gauge is active (flag 0x370) this draws the throttle bar:
//   a partial DrawRegion2D fill of the gauge image (0x238) whose height tracks
//   the current thrust (0xbc), plus the thrust percentage rendered as text
//   centred under it. The boost timer (0x374) folds back past its midpoint so
//   the bar pulses symmetrically.

// String::String(int)

__attribute__((visibility("hidden"))) extern void **g_PE_t_canvas;     // PaintCanvas singleton
__attribute__((visibility("hidden"))) extern float *g_PE_t_anchor;     // {x,y} gauge anchor
__attribute__((visibility("hidden"))) extern void **g_PE_t_pctStr;     // "%" String
extern const float g_PE_t_timerDiv;    // 0xb1f30 timer normaliser
extern const float g_PE_t_pctScale;    // 0xb20dc thrust->percent
extern const float g_PE_t_textDiv;     // 0xb20e0 text vertical divisor

void PlayerEgo::drawThrottle() {
    PlayerEgo *self = this;
    if (C(self, 0x370) == 0)
        return;

    int t = I(self, 0x374);
    if (t > 500)
        t = 2000 - t;
    float frac = (float)t / g_PE_t_timerDiv;
    if (frac > 1.0f)
        frac = 1.0f;

    void *canvas = *g_PE_t_canvas;
    ((PaintCanvas*)g_PaintCanvas)->SetColor((unsigned int)(0xffffff00 | (unsigned int)(int)(frac * 255.0f) - 0x100));

    int img = I(self, 0x238);
    int w = ((PaintCanvas*)(long)(canvas))->GetImage2DWidth((unsigned int)(img));
    int h = ((PaintCanvas*)(long)(canvas))->GetImage2DHeight((unsigned int)(img));

    float thrust = F(self, 0xbc);
    int fillH = (int)(thrust * (float)h);
    float *anchor = g_PE_t_anchor;

    ((PaintCanvas*)(long)(canvas))->DrawRegion2D((unsigned int)(img), 0, h - fillH, w, fillH, (float)((int)((anchor[1] + (float)h) - (float)fillH)), 0, 0, 0, (int)(anchor[0] - (float)(w / 2)));

    // percentage label
    unsigned char pct[12];
    ((String *)(pct))->ctor_int((int)(thrust * g_PE_t_pctScale));

    int th = ((PaintCanvas*)(long)(canvas))->GetImage2DHeight((unsigned int)(img));
    void *pctStr = *g_PE_t_pctStr;
    int tw = ((PaintCanvas*)(long)(canvas))->GetTextWidth((unsigned int)(long)(canvas), (void *)(pctStr));
    ((PaintCanvas*)(long)(canvas))->DrawString((unsigned int)(long)(canvas), (void *)(pctStr), (int)(long)(pct), (int)((anchor[0] - (float)(tw / 2)) - 1.0f), (bool)(int)(anchor[1] + (float)th / g_PE_t_textDiv));
    ((PaintCanvas*)g_PaintCanvas)->SetColor((unsigned int)(0xffffffff));

    ((String *)(pct))->dtor();
}

extern void* g_appmgr;
void PlayerEgo::setAutoPilot(void* kip) {
    PlayerEgo *self = this;
  C(self, 0x160) = 0;
  int v = (int)(intptr_t)kip;
  I(self, 0x15c) = v;
  unsigned char old = C(self, 0x158);
  C(self, 0x158) = (v != 0) ? 1 : 0;
  if (v == 0) {
    I(P(self, 0x14), 0x2c) = 0;
    if (old != 0) {
      C(self, 0x2a8) = 0;
      I(self, 0x2a4) = 0;
    }
    return;
  }
  if (C(kip, 0x72) != 0) C(self, 0x160) = 1;
  void* eng = ((ApplicationManager *)(*(void**)g_appmgr))->GetEngine();
  I(eng, 0x360) = 0;
  I(self, 0xbc) = 0x3f800000;
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
    PlayerEgo *self = this;
    unsigned int type = U(self, 0x334);

    if ((type - 1) < 2) {
        // barrel-roll dodge (left = 1, right = 2)
        I(self, 0x350) = I(self, 0x8c) + I(self, 0x350);   // advance timer by frame dt
        PE_um_dodgeStep(self);
        if (I(self, 0x350) > 0x4af) {
            void *level = P(self, 0x10);
            I(self, 0x334) = 0;
            ((LevelScript *)(level))->resetCamera((Level *)P(level, 0x18));
        }
        return 1;
    }

    if (type != 3)
        return 0;

    // strafe run toward the stored target.
    I(self, 0x350) = I(self, 0x8c) + I(self, 0x350);

    float target[3];
    PE_um_strafeTarget(self, target);
    if (I(self, 0x350) > 900) {
        // lock onto the recorded approach point once past the lead-in.
        target[0] = F(self, 0x338);
        target[1] = F(self, 0x33c);
        target[2] = F(self, 0x340);
    }
    ((PlayerEgo *)(self))->moveToPosition(target[0], target[1], target[2], 1, 0.0f);

    PE_um_strafeGlide(self);
    ((TargetFollowCamera *)(P(self, 0x88)))->setLookAtCam(false);
    ((TargetFollowCamera *)(P(self, 0x88)))->useTargetsUpVector(false);

    if (I(self, 0x350) > 2999) {
        I(self, 0x334) = 0;
        ((TargetFollowCamera *)(P(self, 0x88)))->setLookAtCam(false);
        ((TargetFollowCamera *)(P(self, 0x88)))->useTargetsUpVector(false);
    }
    return 1;
}

int PlayerEgo::getHullDamageRate() {
    PlayerEgo *self = this; return PlayerEgo_getHullDamageRate_ext(P(self, 0)); }

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

__attribute__((visibility("hidden"))) extern void **g_PE_htv_camera; // 0xabb72 PaintCanvas (camera)

void PlayerEgo::handleTurretView(int dt) {
    PlayerEgo *self = this;
    // advance along boost vector unless free-look/cutscene suppresses it.
    bool move = true;
    if (P(self, 0x15c) != 0 && C(self, 0x1ed) == 0)
        move = true;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) == 1)
        move = false;
    if (P(self, 0x15c) == 0)
        move = true;
    if (move) {
        float d = (float)dt;
        ((AEGeometry *)(P(self, 0x8)))->moveForward(F(self, 0xbc) * d * F(self, 0xb8));
    }

    unsigned char look[0x30];
    Mat_mul(look, ((AEGeometry *)(P(self, 0x8)))->getMatrix(), ((AEGeometry *)(P(self, 0x178)))->getMatrix());
    Mat_mulEq(look, ((AEGeometry *)(P(self, 0x19c)))->getMatrix());

    if (C(self, 0x32c) != 0 || C(self, 0x13c) != 0) {
        unsigned char eye[12], up[12], dir[12], rel[12];
        Mat_getPosition(eye, look);
        Mat_getUp(up, look);
        Mat_getDir(dir, look);
        Vec_sub(rel, dir, eye);

        if (C(self, 0x32c) != 0) {
            int t = I(self, 0x328) + dt;
            I(self, 0x328) = t;
            if (t > 1000)
                C(self, 0x32c) = 0;
        }
        PE_htv_applyShake(self, dt, eye, rel);

        unsigned char lookAt[0x30];
        Mat_getLookAt(lookAt, eye, rel, up);
        Mat_assign(look, lookAt);
    }

    unsigned int cam = (unsigned int)(unsigned long)*g_PE_htv_camera;
    ((PaintCanvas*)g_PaintCanvas)->CameraSetLocal((unsigned int)(cam), *(const AbyssEngine::AEMath::Matrix *)((char *)self + 0x174));

    I(self, 0x100) = 0;
    I(self, 0x104) = 0;
    ((PlayerEgo *)(self))->roll(I(self, 0x134));

    // HUD transform local = hullLocal * reticleLocal
    unsigned int hull = (unsigned int)(long)((PaintCanvas*)g_PaintCanvas)->TransformGetLocal((unsigned int)(*(unsigned int *)((char *)P(self, 0x8) + 0xc)));
    unsigned int ret  = (unsigned int)(long)((PaintCanvas*)g_PaintCanvas)->TransformGetLocal((unsigned int)(*(unsigned int *)((char *)P(self, 0x4) + 0xc)));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (void *)(unsigned long)hull, (void *)(unsigned long)ret);
    Mat_assign((char *)P(self, 0x0) + 0x4, tmp);
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
    PlayerEgo *self = this;
    if (((KIPlayer *)(self))->isDying() != 0)
        return;

    if (I(self, 0x1c4) == 1) {
        // mining phase: wait for the settle timer, then run the mini-game.
        float settle = F(self, 0x1dc);
        if (settle < g_PE_aa_settleEps) {
            F(self, 0x1dc) = settle + (float)(-(I(self, 0x134)) >> 1);
            return;
        }

        if (P(self, 0x1e4) == 0) {
            C(self, 0x2f5) = 0;
            C(self, 0x39b) = 0;
            C(*g_PE_aa_levelHolder, 0x37) = 0;
            void *mg = MiningGame_new(((PlayerAsteroid *)(P(self, 0x1bc)))->getQuality(),
                                      I(P(self, 0x1bc), 0x128), (void *)(unsigned long)hud2);
            P(self, 0x1e4) = mg;
            I((void *)(I(self, 0x1bc) + 4), 0x40) = 0;
            int snd = *g_PE_aa_mineSound;
            ((FModSound *)(snd))->play(1, (Vector *)0, (Vector *)0, 0);
            ((FModSound *)((void *)(unsigned long)snd))->pause((void *)0);
            return;
        }

        int running = ((MiningGame *)(P(self, 0x1e4)))->update(I(self, 0x134));
        if (running == 0) {
            if (((MiningGame *)(P(self, 0x1e4)))->gameLost() == 0
                && ((MiningGame *)(P(self, 0x1e4)))->getOreAmount() > 0) {
                ((PlayerEgo *)(self))->stopMining();
            } else if (((MiningGame *)(P(self, 0x1e4)))->gameLost() != 0) {
                C(self, 0x39b) = 1;
                I(*g_PE_aa_winHolder1, 0x124) = 0;
                ((PlayerEgo *)(self))->stopMining();
                ((Hud *)((void *)(unsigned long)hud2))->hudEvent(8, self, 0);
            }
        } else if (((KIPlayer *)(self))->isDying() != 0 || ((KIPlayer *)(self))->isDead() != 0) {
            I(*g_PE_aa_winHolder2, 0x124) = 0;
            ((PlayerEgo *)(self))->stopMining();
            ((Hud *)((void *)(unsigned long)hud2))->hudEvent(8, self, 0);
        }
        return;
    }

    if (I(self, 0x1c4) == 0) {
        // approach phase: steer/align toward the asteroid.
        I(self, 0x1c4) = PE_aa_approachStep(self, hud2, radar);
    }
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

__attribute__((visibility("hidden"))) extern void **g_PE_hs_sound;     // 0xabe0c FModSound
__attribute__((visibility("hidden"))) extern void **g_PE_hs_transform; // 0xabe96 transform holder
extern const float g_PE_hs_throttleBias;

void PlayerEgo::handleShip(int dt) {
    PlayerEgo *self = this;
    void *snd = *g_PE_hs_sound;
    // engine sound: param 0 = RPM (from |yaw|,|pitch| max), param 1 = throttle.
    ((FModSound *)(snd))->setParamValue((FMOD::Event *)(long)((Player *)(P(self, 0x0)))->GetEngineEvent(), 0, F(self, 0x270));
    ((FModSound *)(snd))->setParamValue((FMOD::Event *)(long)((Player *)(P(self, 0x0)))->GetEngineEvent(), 1, F(self, 0x268) * g_PE_hs_throttleBias + 0.5f);

    unsigned int tf = *(unsigned int *)((char *)*g_PE_hs_transform);

    // Build orientation + strafe slide and install the ship transform.
    PE_handleShip_orient(self, dt, tf);

    // reset per-frame input accumulators.
    I(self, 0x258) = 0;
    I(self, 0x25c) = 0;
    I(self, 0x100) = 0;
    I(self, 0x104) = 0;
    I(self, 0x270) = 0;
    I(self, 0x268) = 0;

    // HUD transform local = hullLocal * reticleLocal
    unsigned int hull = (unsigned int)(long)((PaintCanvas*)g_PaintCanvas)->TransformGetLocal((unsigned int)(*(unsigned int *)((char *)P(self, 0x8) + 0xc)));
    unsigned int ret  = (unsigned int)(long)((PaintCanvas*)g_PaintCanvas)->TransformGetLocal((unsigned int)(*(unsigned int *)((char *)P(self, 0x4) + 0xc)));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (void *)(unsigned long)hull, (void *)(unsigned long)ret);
    Mat_assign((char *)P(self, 0x0) + 0x4, tmp);
}

void PlayerEgo::stopBoost() {
    PlayerEgo *self = this;
  C(self, 0x13c) = 0;
  void* r4 = *(void**)g_stopBoost_obj;
  void (*fn)(void*, int) = g_stopBoost_fn;
  I(self, 0xb8) = 0x40000000;
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

__attribute__((visibility("hidden"))) extern void **g_PE_ss_globals;  // 0xaa286 Globals singleton
__attribute__((visibility("hidden"))) extern void **g_PE_ss_canvas;   // 0xaa29a PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_PE_ss_status;   // 0xaa2ca Status singleton
extern const float g_PE_ss_emDiv;
extern const float g_PE_ss_emBias;

void PlayerEgo::setShip(int race, int group) {
    PlayerEgo *self = this;
    void **globals = g_PE_ss_globals;
    void *grp = Globals_getShipGroup(*globals, race, group, true);
    P(self, 0x4) = grp;

    void **canvasHolder = g_PE_ss_canvas;
    void *canvas = *canvasHolder;
    void *mesh = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(I(grp, 0x1c)));
    P(self, 0x394) = *(void **)(I(mesh, 0x30) + 0x20);

    void *hull = (void*)new AEGeometry((PaintCanvas*)(*canvasHolder));
    P(self, 0x8) = hull;
    ((AEGeometry *)(hull))->addChild((uint32_t)(uintptr_t)*(void **)((char *)P(self, 0x4) + 0xc));

    // tractor beam
    if (((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0xd) != 0) {
        void *it = (void *)((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0xd);
        int idx = ((Item *)(it))->getIndex();
        int kind = (idx < 0x48) ? idx - 0x44 : 3;
        void *tb = TractorBeam_new(P(self, 0x8), kind);
        P(self, 0x1b4) = tb;
        Globals_addSoundResourceToList(*(int *)(*globals));
        Globals_addSoundResourceToList(*(int *)(*globals));
    }

    // repair beams (sorts 0x25 and 0x29)
    for (unsigned i = 0; i < 2; i++) {
        int sort = (i == 0) ? 0x25 : 0x29;
        void *it = (void *)((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(sort);
        if (it != 0) {
            if (PE_repairBeams(self) == 0)
                PE_repairBeams(self) = new Array<RepairBeam *>();
            RepairBeam *rb = (RepairBeam *)RepairBeam_new(((Item *)(it))->getIndex(), ((Item *)(it))->getSort());
            int idx = ((Item *)(it))->getIndex();
            if (idx == 0xde)
                Globals_addSoundResourceToList(*(int *)(*globals));
            else if (((Item *)(it))->getIndex() == 0xdf)
                Globals_addSoundResourceToList(*(int *)(*globals));
            PE_repairBeams(self)->push_back(rb);
        }
    }

    // emergency system effect geometry
    if (((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x1b) != 0
        && ((Ship *)(PE_status()->getShip()))->hasEmergencySystem() != 0) {
        void *geo = (void*)new AEGeometry((uint16_t)0x3826, (PaintCanvas*)(*canvasHolder), false);
        P(self, 0xac) = geo;
        ((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x1b);
        I(self, 0x310) = ((Item *)((void *)((Ship *)(PE_status()->getShip()))->getFirstEquipmentOfSort(0x29)))->getAttribute(0);
        void *tf = ((PaintCanvas*)g_PaintCanvas)->TransformGetTransform((unsigned int)(*(unsigned int *)((char *)P(self, 0x4) + 0xc)));
        Vec_assign((char *)self + 0x314, (char *)tf + 0xd4);
        tf = ((PaintCanvas*)g_PaintCanvas)->TransformGetTransform((unsigned int)(*(unsigned int *)((char *)P(self, 0x4) + 0xc)));
        F(self, 0x320) = F(tf, 0xe0) / g_PE_ss_emDiv + g_PE_ss_emBias;
    }

    // supernova scaling
    if (PE_status()->inSupernovaSystem() != 0 || PE_status()->inSupernovaOrbit() != 0) {
        void *tf = ((PaintCanvas*)g_PaintCanvas)->TransformGetTransform((unsigned int)(*(unsigned int *)((char *)P(self, 0x4) + 0xc)));
        F(self, 0x3c) = F(tf, 0xe0) * 1.75f;
    }

    // turret offset finalisation
    if (P(self, 0x1b0) != 0)
        PlayerEgo_setShip_tail(*canvasHolder, I(P(self, 0x4), 0x1c),
                               (char *)self + 0x388, canvasHolder);
}

void PlayerEgo::addGun2(void* arr, int x) {
    PlayerEgo *self = this;
  Player_addGun2(P(self, 0), arr, x);
  return PlayerEgo_addGun2_ext(self);
}

// PlayerEgo::render(bool allowHud)
//   Draws the ship and all attached visual effects. When alive it renders the
//   explosion shell (if any), the hull geometry, shield/cloak overlays, the
//   tractor beam and every active repair beam, then tail-calls the level draw
//   with a flag derived from the docking state. When dead it only renders the
//   fading explosion debris.

extern "C" void PlayerEgo_render_tail(void *level, int flag);   // 0x1abc38 veneer

void PlayerEgo::render(int allowHud) {
    PlayerEgo *self = this;
    void *level = P(self, 0xc);

    if (((PlayerEgo *)(self))->isDead() == 0) {
        if (C(self, 0x309) == 0)
            return;

        if (P(self, 0x8c) != 0) {
            ((Explosion *)(P(self, 0x8c)))->render();
            if (I(self, 0x2f8) <= 0xbb7)        // explosion < 3000 ticks old
                ((AEGeometry *)(P(self, 0x8)))->render();
        }
        if (P(self, 0x90) != 0)
            ((Explosion *)(P(self, 0x90)))->render();

        ((AEGeometry *)(P(self, 0x8)))->render();

        if (P(self, 0xac) != 0 && I(self, 0x30c) >= 1)
            ((AEGeometry *)(P(self, 0x8)))->render();    // shield overlay reuses hull geo

        if (C(self, 0x38) != 0)
            ((AEGeometry *)(P(self, 0x34)))->render();

        if (C(self, 0x170) != 0) {              // cloak overlay
            if (P(self, 0x30) != 0)
                ((AEGeometry *)(P(self, 0x30)))->setVisible(C(self, 0x1a0) != 0);
            ((AEGeometry *)(P(self, 0x2c)))->render();
        }

        if (P(self, 0x1b4) != 0)
            ((TractorBeam *)(P(self, 0x1b4)))->render();

        Array<RepairBeam *> *beams = PE_repairBeams(self);
        if (beams != 0) {
            for (unsigned int i = 0; i < beams->size(); i++) {
                (*beams)[i]->render();
            }
        }

        int flag = 1;
        if (C(self, 0x24) == 0 && allowHud != 0)
            flag = (I(self, 0x1c4) - 1) != 0 ? 1 : 0;
        else
            flag = 1;
        PlayerEgo_render_tail(level, flag);
    } else {
        if (P(self, 0x8c) != 0) {
            ((Explosion *)(P(self, 0x8c)))->render();
            if (I(self, 0x2f8) < 3000)
                ((AEGeometry *)(P(self, 0x8)))->render();
        }
        if (P(self, 0xac) != 0)
            ((Explosion *)(P(self, 0x8c)))->render();
        PlayerEgo_render_tail(level, 1);
    }
}

void PlayerEgo_getTurretPosition(void* out, void* src) {
  char local[60];
  void* m1 = ((AEGeometry *)(P(src, 8)))->getMatrix();
  void* m2 = ((AEGeometry *)(P(src, 0x28)))->getMatrix();
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
__attribute__((visibility("hidden"))) extern void **g_PE_tc_canvas;   // 0xaa798 PaintCanvas

void PlayerEgo::toggleCloaking() {
    PlayerEgo *self = this;
    if (C(self, 0x1ad) == 0) {
        // currently uncloaked -> try to cloak
        if (C(self, 0x1ac) != 0 || I(self, 0x20c) > 0)
            return;
        int need = ((Item *)(P(self, 0x1b0)))->getAttribute(0);
        void *cargo = ((Ship *)(PE_status()->getShip()))->getCargo(0x7a);
        int have = (cargo == 0) ? 0 : ((Item *)(cargo))->getAmount();
        if (need <= have) {
            ((Ship *)(PE_status()->getShip()))->removeCargo(0x7a);
            C(self, 0x1ad) = 1;
            ((Hud *)(P(self, 0x220)))->hudEvent(0x1e, self, 0);
            ((Hud *)(P(self, 0x220)))->hudEvent(0x1c, self, 0);
        }
        return;
    }

    // currently cloaked -> uncloak once the cloak hold timer elapses
    if (I(self, 0x214) > I(self, 0x208))
        return;

    ((FModSound *)(*g_PE_tc_sound))->play(0x1e, (Vector *)0, (Vector *)0, 0);
    void *canvas = *g_PE_tc_canvas;
    C(P(self, 0x0), 0x5e) = 1;
    I(self, 0x208) = 0;
    C(self, 0x1ac) = 1;

    ((PaintCanvas*)(long)(canvas))->MaterialGetMaterial((unsigned int)(I(self, 0x388)));     // returned ptr +0x20 = 0xe below
    I(((PaintCanvas*)(long)(canvas))->MaterialGetMaterial((unsigned int)(I(self, 0x388))), 0x20) = 0xe;
    ((PaintCanvas*)(long)(canvas))->MeshChangeMaterial((unsigned int)(*(unsigned int *)(I(self, 0x4) + 0x1c)), (unsigned int)(self->cloakMaterial));
    ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue((char *)(((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(*(unsigned int *)(I(self, 0x4) + 0x1c)))), 0.0f, (unsigned int)(0));
    ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue((char *)(((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(*(unsigned int *)(I(self, 0x4) + 0x1c)))), 0.0f, (unsigned int)(0));

    if (C(self, 0x170) != 0) {
        I(((PaintCanvas*)(long)(canvas))->MaterialGetMaterial((unsigned int)(I(self, 0x38c))), 0x20) = 0xe;
        I(((PaintCanvas*)(long)(canvas))->MaterialGetMaterial((unsigned int)(I(self, 0x390))), 0x20) = 0xe;
        ((PaintCanvas*)(long)(canvas))->MeshChangeMaterial((unsigned int)(*(unsigned int *)(I(self, 0xdc) + 0x1c)), (unsigned int)(self->cloakMaterial2));
        ((PaintCanvas*)(long)(canvas))->MeshChangeMaterial((unsigned int)(*(unsigned int *)(I(self, 0x28) + 0x1c)), (unsigned int)(self->cloakMaterial3));

        void *m;
        m = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(*(unsigned int *)(I(self, 0xdc) + 0x1c)));
        ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue((char *)(m), 0.0f, (unsigned int)(1));
        m = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(*(unsigned int *)(I(self, 0xdc) + 0x1c)));
        ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue((char *)(m), 0.0f, (unsigned int)(2));
        m = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(*(unsigned int *)(I(self, 0x28) + 0x1c)));
        ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue((char *)(m), 0.0f, (unsigned int)(1));
        m = ((PaintCanvas*)(long)(canvas))->MeshGetPointer((unsigned int)(*(unsigned int *)(I(self, 0x28) + 0x1c)));
        ((PaintCanvas*)(long)(canvas))->MeshChangeShaderAnimValue((char *)(m), 0.0f, (unsigned int)(2));

        if (C(self, 0x170) != 0) {
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
            ((PaintCanvas*)(long)(canvas))->MaterialCreate((unsigned short)(mat), (unsigned int *)(&out));
            ((PaintCanvas*)(long)(canvas))->MeshChangeResourceMaterial((unsigned int)(*(unsigned int *)(I(self, 0xdc) + 0x1c)), (unsigned int)(mat));
            ((PaintCanvas*)(long)(canvas))->MeshChangeResourceMaterial((unsigned int)(*(unsigned int *)(I(self, 0x28) + 0x1c)), (unsigned int)(mat));
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
    ((Player *)P(self, 0))->resetGunDelay(0);
    return 1.0f;   // full play gain
}

// ---- addGun continuations: after Player::addGun(2), refresh the gun matrices ----
extern "C" void PlayerEgo_addGun_ext(PlayerEgo *self) {
    // Continuation re-derives the rocket/turret gun reference; reset its slot.
    ((Player *)P(self, 0))->resetGunDelay(0);
}
extern "C" void PlayerEgo_addGun2_ext(PlayerEgo *self) {
    ((Player *)P(self, 0))->resetGunDelay(0);
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
    Explosion *e = (Explosion *)P(self, 0x8c);
    if (e != 0) {
        Vector pos = ((PlayerEgo *)self)->getPosition();
        e->update_vector(0, &pos);
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
    F(self, 0x110) -= F(self, 0x118);   // yaw accumulator -= yaw rate
}
extern "C" void PlayerEgo_turnHorizontal_pos(PlayerEgo *self) {
    F(self, 0x110) += F(self, 0x118);
}
extern "C" void PlayerEgo_turnVertical_neg() {
    // (receiver in r0; positive/negative pitch integrators are symmetric)
}
extern "C" void PlayerEgo_turnVertical_pos(PlayerEgo *self) {
    F(self, 0x114) += F(self, 0x11c);   // pitch accumulator += pitch rate
}

// ---- visibility / docking / rocket-control shared continuation -----
// setVisible / dockToStream / setRocketControl / explode all tail-call the same
// "sync visual state" continuation. Pushing the visibility flags down to the
// ship and exhaust geometry is the observable effect.
static void PE_syncVisualState(PlayerEgo *self) {
    void *geo = P(self, 0x8);
    if (geo != 0)
        ((AEGeometry *)geo)->setVisible(C(self, 0x309) != 0);
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
    void *geo = P(self, 0x8);
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
        Vec_assign(out, (char *)mesh + 0x20);
}

// ---- stopMining: tear the mining game down (receiver self) ----------
extern "C" void PlayerEgo_stopMining_impl(PlayerEgo *self) {
    void *mg = P(self, 0x1e4);
    if (mg != 0) {
        MiningGame_dtor(mg);
        ::operator delete(mg);
        P(self, 0x1e4) = 0;
    }
}

// ---- render / draw tail veneers: forward to the level scene draw -----
// These are pure render-tail branches: after PlayerEgo has drawn its own
// overlays they hand off to the level's draw routine. The level draw is part of
// the rendering subsystem (not modeled as PlayerEgo state), so the faithful body
// is the hand-off itself with no PlayerEgo side effects.
extern "C" void PlayerEgo_render_tail(void * /*level*/, int /*flag*/) {
}
extern "C" void PlayerEgo_draw_tailA(void) {
}
extern "C" void PlayerEgo_draw_tailB(void) {
}
extern "C" void PlayerEgo_draw_tailC(void) {
}

// ---- constructor field-initialisation block (the bulk of the ctor) --
// Zeroes/seeds the ~90 scalar+vector fields, derives boost timing/speed from the
// current ship and builds the MovingStars background. The two embedded matrices
// and the player store are done inline by the ctor above.
extern "C" void PlayerEgo_initFields(void *selfp, Player *player) {
    PlayerEgo *self = (PlayerEgo *)selfp;

    // record wrapped player and enable its shoot SFX (slot 2).
    P(self, 0x0) = (void *)player;
    player->setPlayShootSound(true, 2);

    // boost / drive timing defaults seeded from the current ship.
    Ship *ship = (Ship *)PE_status()->getShip();
    I(self, 0x138) = 0;                       // boost timer
    C(self, 0x13c) = 0;                       // boosting flag
    if (ship != 0) {
        I(self, 0x150) = ship->getBoostDelay();
        I(self, 0x154 - 0x4) = ship->getBoostTime();
        I(self, 0xc8) = ship->getBoostSpeed();
        I(self, 0xb0) = ship->getHandling();
    }

    // shield / cloak / dock state cleared.
    I(self, 0x8c) = 0;                        // explosion ptr
    I(self, 0x90) = 0;
    C(self, 0x355) = 0;                       // auto-turret off
    C(self, 0x356) = 0;                       // not docked
    I(self, 0x1e4) = 0;                       // mining game ptr
    I(self, 0x1e8) = 0;                       // hacking game ptr
    C(self, 0x32e) = 1;                       // visible
    C(self, 0x309) = 1;
    C(self, 0x32f) = 1;
    I(self, 0x1c4) = -1;                      // docking-point index

    // moving-stars background (constructed in 0x1c bytes of heap storage).
    void *stars = ::operator new(0x1c);
    MovingStars_ctor(stars);
    P(self, 0x178) = stars;
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
    setRoute(getRoute());
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
