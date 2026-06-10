#include "gof2/PlayerEgo.h"

// ---- canonical helper types / prototypes (deduped across merged sources) ----
// 3-float vector returned by value by several AEGeometry/PlayerEgo accessors.
struct Vec3 { float x, y, z; };

// These accessors are used in the decomp both as value-returning (Vec3 r = f(geo))
// and as out-param (f(geo, out)) forms — the same sret ABI function viewed two ways.
// Declared variadic so both call shapes type-check (the trailing out-ptr, when
// present, is passed as a variadic argument and the returned value discarded).
extern "C" Vec3 AEGeometry_getDirection(void*, ...);
extern "C" Vec3 AEGeometry_getUpVector(void*, ...);
extern "C" Vec3 AEGeometry_getPosition(void*, ...);
extern "C" Vec3 PlayerEgo_getPosition(PlayerEgo* self, ...);

// Functions whose merged prototypes disagree on argument lists / return type.
// Unified here as variadic so every call site compiles (compile-only target).
// Their first argument is sometimes an int handle, sometimes a void*, so these
// are declared fully variadic to accept either.
extern "C" int  FModSound_play(...);
extern "C" void* Status_getShip(...);
extern "C" void* Status_getMission(...);
extern "C" int  Status_inAlienOrbit(...);
extern "C" void TargetFollowCamera_setLookAtCam(...);
extern "C" void ParticleSystemManager_enableSystemEmit(...);
extern "C" int  Transform_SetVisible(void* tf, bool vis);
extern "C" void* PaintCanvas_MeshGetPointer(void* canvas, unsigned int mesh);
extern "C" void  AEGeometry_setPosition(void* geo, const void* vec);
extern "C" void  Hud_hudEvent(...);                 // (int|void* hud, int ev, PlayerEgo*, [int])
extern "C" void  TargetFollowCamera_setActive(void* cam, int active);
extern "C" void* AEGeometry_newMesh(int meshId, void* canvas, bool b);
extern "C" void  operator_delete_(void*) noexcept;

extern "C" void PlayerEgo_setVisible_ext();
extern "C" void* g_boost_fmod;
extern "C" void* Level_getLandmarks(void*);
extern "C" int PlayerEgo_getHitpoints_ext(void*);
extern "C" int PlayerEgo_getHitpoints(PlayerEgo*);
extern "C" int PlayerEgo_goingToStream(PlayerEgo*);
extern "C" int PlayerEgo_goingToStation(PlayerEgo*);
extern "C" int PlayerEgo_getHackingGameDockIndex_ext(int);
extern "C" void PlayerEgo_setPosition3(PlayerEgo*, float, float, float);
extern "C" void PlayerEgo_dockToStream_ext(PlayerEgo*, int);
extern "C" void PlayerEgo_hackingShuffle_ext(int);
extern "C" void* AEGeometry_getReferenceMatrix(void*);
extern "C" void ParticleSystemManager_systemSetMatrix(void*, int, void*);
extern "C" void PlayerEgo_setRocketControl_ext(PlayerEgo*, int);
extern "C" void PlayerEgo_turnHorizontal_neg(PlayerEgo*);
extern "C" void PlayerEgo_turnHorizontal_pos(PlayerEgo*);
extern "C" int Player_getCombinedHP(void*);
extern "C" void ParticleSystemManager_enableSystemEmit2(void*, int, int);
extern "C" void PlayerEgo_setPosition_v(PlayerEgo*);
extern "C" void PlayerEgo_PauseEngineSound_ext(void*);
extern "C" void PlayerEgo_setActive_ext(void*);
extern "C" void PlayerEgo_setAutoTurret_ext(void*, int);
extern "C" void PlayerEgo_hitCamera_ext(int);
extern "C" int PlayerEgo_hackingWon_ext(int);
extern "C" int PlayerEgo_getCurrentMiningAmount_ext(int);
extern "C" int HackingGame_isRotating(int);
extern "C" int HackingGame_gameWon(int);
extern "C" void PlayerEgo_hackingRotateLCW_ext(int, int);
extern "C" void PlayerEgo_ResumeEngineSound_ext(void*, int);
extern "C" void ParticleSystemManager_enableSystemEmit3(void*, int, int);
extern "C" void* operator_new_(unsigned);
extern "C" void Explosion_ctor(void*, int);
extern "C" void Player_setActive_(int);
extern "C" void PlayerEgo_explode_ext(PlayerEgo*, int);
extern "C" int __aeabi_idiv(int, int);
extern "C" void PlayerEgo_turnVertical_neg();
extern "C" void PlayerEgo_turnVertical_pos(PlayerEgo*);
extern "C" void* g_FMod_singleton;
extern "C" int Mission_isEmpty(void*);
extern "C" void* Status_getStation(void*);
extern "C" int Station_getIndex(void*);
extern "C" void Player_addGun(void*, void*, int);
extern "C" void PlayerEgo_addGun_ext(PlayerEgo*);
extern "C" void PlayerEgo_endExplosion_ext(int);
extern "C" int Player_getHitpoints(void*);
extern "C" void Player_setHitpoints(void*, int);
extern "C" void Player_setVulnerable(void*, int);
extern "C" int Ship_getFirstEquipmentOfSort(void*, int);
extern "C" float Ship_removeEquipment(void*, int);
extern "C" void PlayerEgo_stopBoost(PlayerEgo*);
extern "C" void PlayerEgo_hackingRotateRCW_ext(int, int);
extern "C" void PlayerEgo_refillGunDelay_ext(void*, int);
extern "C" int   Player_gunAvailable(void *player);
extern "C" int   Ship_getEquipment(void *ship, int slot);
extern "C" int   Item_getAttribute(void *item);
extern "C" int   Item_getIndex(void *item);
extern "C" void *AEGeometry_new(void *canvas);
extern "C" void  AEGeometry_setRotationOrder(void *geo, int order);
extern "C" void  AEGeometry_addChild(void *parent, void *child);
extern "C" void *AEGeometry_dtor(void *geo);
extern "C" void *PaintCanvas_TransformGetTransform(unsigned int handle);
extern "C" void  Transform_SetAnimationState(void *tf, int a, int b);
extern "C" void  PaintCanvas_MeshCloneMaterial(void *canvas, int mesh, unsigned int *out);
extern "C" void PlayerEgo_pitchAllPrimaryGuns_ext(void*);
extern "C" int PlayerEgo_isDead(PlayerEgo*);
extern "C" void stopShooting_extA(void*, int);
extern "C" void stopShooting_extB(void*, int, int);
extern "C" int  AERandom_next(void *rng, int bound);
extern "C" void Camera_shake(void *cam, float dx, float dy, float dz);
extern "C" void* g_setRotation_transform;
extern "C" void* TransformGetLocal(void*, int);
extern "C" void MatrixSetRotation(void*, void*, float, float, float);
extern "C" int Status_getCurrentCampaignMission(void*);
extern "C" float Status_getGammaRayDamagePerSecond(void*, int, int);
extern "C" void PlayerEgo_StopEngineSound_ext(void*);
extern "C" void PlayerEgo_startSmokeEmission_ext(void*, int, int);
extern "C" int   Ship_getCurrentLoad(void *ship);
extern "C" int   Ship_getMaxLoad(void *ship);
extern "C" int   aeabi_idiv_(int a, int b);
extern "C" float PE_pitchRampDelta(PlayerEgo *self, float rate, int frameTime);
extern "C" int   KIPlayer_isDying(void *self);
extern "C" void *KIPlayer_getNearestNavigationPoint(void *station, void *posOut, void *spOut);
extern "C" void  SpacePoint_take(void *sp);
extern "C" void  SpacePoint_giveFree(void *sp);
extern "C" void  PlayerEgo_setTurretMode(PlayerEgo *self, int on);
extern "C" int   Ship_getIndex(void *ship);
extern "C" int   Ship_hasCargo(void *ship, int idx);
extern "C" void *Ship_getCargo(void *ship, int idx);
extern "C" int   Ship_getMaxPassengers(void *ship);
extern "C" int   Item_getAmount(void *item);
extern "C" void *Status_getCampaignMission();
extern "C" int   Mission_isEmpty(void *m);
extern "C" int   Mission_getType(void *m);
extern "C" int   Mission_getProductionGoodIndex(void *m);
extern "C" int   Mission_getProductionGoodAmount(void *m);
extern "C" int   Mission_getStatusValue(void *m);
extern "C" int   PlayerFixedObject_getDockingType(void *o);
extern "C" int   Level_getNumDeliveredOre(void *level);
extern "C" void  TargetFollowCamera_useTargetsUpVector(void *cam, bool on);
extern "C" void  LevelScript_resetCamera(void *level);
extern "C" void  Player_resetGunDelay(void *player, int idx);
extern "C" void *HackingGame_dtor(void *hg);
extern "C" void  Hud_setHackingGameActive(void *hud, bool active);
extern "C" int   PE_adp_approach(PlayerEgo *self, void *station);
extern "C" int   PE_adp_glide(PlayerEgo *self);
extern "C" void  PE_adp_apply(PlayerEgo *self);
extern "C" void* AEGeometry_getMatrix2(void*);
extern "C" void* ParticleSystemManager_addSystem(void*, void*, int, int);
extern "C" void PlayerEgo_setLevel_ext(void*, int, int);
extern "C" void  PaintCanvas_CameraCreate(void *canvas, unsigned int *outHandle);
extern "C" void  PaintCanvas_CameraSetPerspective(unsigned int cam, float fov, float a, float b);
extern "C" void  AEGeometry_translate(void *geo, const void *vec);
extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void  AEGeometry_setMatrix(void *geo, const void *m);
extern "C" void  AEGeometry_rotateY(void *geo, int a, float ang, int b);
extern "C" float PE_yawRampDelta(float rate, int frameTime);
extern "C" void *Player_dtor(void *);
extern "C" void *AEGeometry_dtor(void *);
extern "C" void *Route_dtor(void *);
extern "C" void *TractorBeam_dtor(void *);
extern "C" void *MiningGame_dtor(void *);
extern "C" void *Explosion_dtor(void *);
extern "C" void *EaseInOutMatrix_dtor(void *);
extern "C" void PlayerEgo_PlayEngineSound_ext(void*, int, int);
extern "C" void  AEGeometry_setDirection(void *geo, const void *dir);
extern "C" void  AEGeometry_moveForward(void *geo, float dist);
extern "C" void  Mat_assign(void *dst, const void *src);
extern "C" void  Vec_assign(void *dst, const void *src);
extern "C" void  PlayerEgo_roll(PlayerEgo *self, int amount);
extern "C" void  TargetFollowCamera_translateNoUpdate(void *cam, float x, float y, float z);
extern "C" void  PE_mtp_steer(PlayerEgo *self, const float *target, int steer, float speed);
extern "C" void PlayerEgo_resetGunDelay_ext(void*, int);
extern "C" int PlayerEgo_getShieldDamageRate_ext(void*);
extern "C" void *Level_getEnemies(void *level);
extern "C" int   KIPlayer_isDead(void *e);
extern "C" int   KIPlayer_isDying(void *e);
extern "C" int   Player_isActive(void *p);
extern "C" int   KIPlayer_isEnemy(void *e);
extern "C" void  Vec_sub(void *out, const void *a, const void *b);
extern "C" float Vec_length(const void *v);
extern "C" void  Player_stopShooting(void *player);
extern "C" int   PE_hat_aimAndFire(PlayerEgo *self, int dt);
extern "C" void  PlayerAsteroid_setRotationEnabled(void *ast, bool en);
extern "C" float PlayerAsteroid_getScaling(void *ast);
extern "C" void *MiningGame_dtor(void *mg);
extern "C" void  Radar_unlockAsteroid(void *radar);
extern "C" void  PlayerEgo_setExhaustVisible(PlayerEgo *self, bool visible);
extern "C" void Vector_assign(void*, void*);
extern "C" float PE_roll_bankFactor(PlayerEgo *self, float rx, float ry, float *outZ);
extern "C" void PlayerEgo_setTargetFollowCamera_ext(void*, void*);
extern "C" int   KIPlayer_getType(void *o);
extern "C" int   KIPlayer_isVisible(void *o);
extern "C" int   KIPlayer_isDying(void *o);
extern "C" int   KIPlayer_isDead(void *o);
extern "C" int   PlayerWormHole_isShrinking(void *o);
extern "C" void  hitCamera_(PlayerEgo *self);
extern "C" void  PE_cc_wormhole(PlayerEgo *self, void *obj);
extern "C" void  PE_cc_obstacle(PlayerEgo *self, void *obj, unsigned idx);
extern "C" void  PE_cc_destructible(PlayerEgo *self, void *obj);
extern "C" float PlayerEgo_resetGunDelay_f(PlayerEgo*);
extern "C" void* g_dockToPlanet_fmod;
extern "C" int   PlayerEgo_isDead(PlayerEgo *self);
extern "C" int   Player_shootSecondary(void *player, int kind, int idx, int hi, int zero);
extern "C" int   Player_shootPrimary(void *player, int kind, int weapon, int hi, int zero);
extern "C" void PlayerEgo_stopMining_impl(PlayerEgo *self);
extern "C" void  PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam);
extern "C" void  PlayerEgo_stopShooting(PlayerEgo *self);
extern "C" void  PaintCanvas_CameraCreate(void *canvas, unsigned int *out);
extern "C" void  AEGeometry_rotate(void *geo, const void *vec);
extern "C" int   Ship_getFirstEquipmentOfSort(void *ship, int sort);
extern "C" void  FModSound_stop(int snd);
extern "C" void* g_rotate_transform;
extern "C" void *KIPlayer_getNearestNavigationPoint(void *radar, void *posOut, void *spOut);
extern "C" void  PlayEngineSound_(PlayerEgo *self);
extern "C" void  PlayerEgo_setExhaustVisible(PlayerEgo *self, bool vis);
extern "C" void *EaseInOutMatrix_dtor(void *m);
extern "C" void *PE_dtdp_makeEase(const void *fromMatrix, const void *navPoint);
extern "C" int  PlayerEgo_isDead(PlayerEgo *self);
extern "C" void  PaintCanvas_SetColor(unsigned int c);
extern "C" int   Radar_isPlasmaInRange(void *radar);
extern "C" void  PlayerEgo_drawThrottle(PlayerEgo *self);
extern "C" int  Player_isActive(void *player);
extern "C" int  Player_getHitpoints(void *player);
extern "C" void Player_stopShooting(void *player);
extern "C" void PlayerEgo_roll(PlayerEgo *self, int amount);
extern "C" int  PlayerEgo_updateManeuver(PlayerEgo *self);
extern "C" void PlayerEgo_handleShip(PlayerEgo *self, int dt);
extern "C" void PlayerEgo_handleTurretView(PlayerEgo *self, int dt);
extern "C" void PlayerEgo_handleAutoTurret(PlayerEgo *self, int dt);
extern "C" void PlayerEgo_dockToAsteroid(PlayerEgo *self, void *radar);
extern "C" void PlayerEgo_dockToDockingPoint(PlayerEgo *self, void *radar);
extern "C" void PlayerEgo_approachAsteroid(PlayerEgo *self, int hud2, void *radar);
extern "C" int  PlayerEgo_approachDockingPoint(PlayerEgo *self, void *hud, int hud2, void *radar);
extern "C" void KIPlayer_setAutoPilot(PlayerEgo *self);
extern "C" void *Route_getWaypoint(void *route);
extern "C" float Ship_getHandling(void *ship);
extern "C" void PE_upd_boost(PlayerEgo *self, int dt);
extern "C" void PE_upd_docksFinishDelivery(PlayerEgo *self, void *radio);
extern "C" void  ParticleSystemManager_enableSystemRender(int mgr, bool en);
extern "C" void *Explosion_dtor(void *exp);
extern "C" int   Player_setActive(void *player, bool active);
extern "C" int   Player_getMaxHitpoints(void *player);
extern "C" void  Player_setHitpoints(void *player, int hp);
extern "C" int   Player_getMaxArmorHP(void *player);
extern "C" void  Player_setArmorHP(void *player, int hp);
extern "C" void  PlayerEgo_setPosition(PlayerEgo *self, int a, int b, int c);
extern "C" int   PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" int   PaintCanvas_GetImage2DHeight(void *canvas);
extern "C" int   PaintCanvas_GetTextWidth(void *canvas, void *str);
extern "C" void  PaintCanvas_DrawString(void *canvas, void *str, void *posStr, int x, bool centered);
extern "C" void  String_dtor(void *s);
extern "C" void* ApplicationManager_GetEngine(void*);
extern "C" void  AEGeometry_getRightVector(void *geo, void *out);
extern "C" void  PE_um_dodgeStep(PlayerEgo *self);
extern "C" void  PE_um_strafeTarget(PlayerEgo *self, float *out);
extern "C" void  PE_um_strafeGlide(PlayerEgo *self);
extern "C" int PlayerEgo_getHullDamageRate_ext(void*);
extern "C" void  Mat_getPosition(void *out, const void *m);
extern "C" void  Mat_getUp(void *out, const void *m);
extern "C" void  Mat_getDir(void *out, const void *m);
extern "C" void  Mat_getLookAt(void *out, const void *eye, const void *dir, const void *up);
extern "C" void  PaintCanvas_CameraSetLocal(unsigned int cam, const void *m);
extern "C" unsigned int PaintCanvas_TransformGetLocal(unsigned int tf);
extern "C" void  PE_htv_applyShake(PlayerEgo *self, int dt, void *eye, void *dir);
extern "C" int   KIPlayer_isDead(void *self);
extern "C" void *MiningGame_new(int quality, int seed, void *hud);
extern "C" int   MiningGame_update(void *mg, int dt);
extern "C" int   MiningGame_gameLost(void *mg);
extern "C" int   MiningGame_getOreAmount(void *mg);
extern "C" int   PlayerAsteroid_getQuality(void *ast);
extern "C" void  FModSound_pause(void *snd);
extern "C" void  PlayerEgo_stopMining(PlayerEgo *self);
extern "C" int   PE_aa_approachStep(PlayerEgo *self, int hud2, void *radar);
extern "C" void *Player_GetEngineEvent(void *player);
extern "C" void  FModSound_setParamValue(void *snd, void *ev, int idx, float v);
extern "C" void  Mat_mul(void *out, const void *a, const void *b);
extern "C" void  PaintCanvas_TransformSetLocal(unsigned int tf, const void *m);
extern "C" void  PE_handleShip_orient(PlayerEgo *self, int dt, unsigned int tfHandle);
extern "C" void* g_stopBoost_obj;
extern "C" void (*g_stopBoost_fn)(void*, int);
extern "C" void *Globals_getShipGroup(void *g, int race, int group, bool b);
extern "C" int   Ship_hasEmergencySystem(void *ship);
extern "C" int   Item_getSort(void *item);
extern "C" void *TractorBeam_new(void *geo, int kind);
extern "C" void  Globals_addSoundResourceToList(int snd);
extern "C" void *RepairBeam_new(int idx, int sort);
extern "C" void  ArrayAdd_RepairBeam(void *rb, void *arr);
extern "C" void *Array_RepairBeam_new();
extern "C" int   Status_inSupernovaSystem();
extern "C" int   Status_inSupernovaOrbit();
extern "C" void  PlayerEgo_setShip_tail(void *canvas, int meshId, void *out, void **canvasHolder);
extern "C" void Player_addGun2(void*, void*, int);
extern "C" void PlayerEgo_addGun2_ext(PlayerEgo*);
extern "C" void Explosion_render(void *exp);
extern "C" void AEGeometry_render(void *geo);
extern "C" void AEGeometry_setVisible(void *geo, bool vis);
extern "C" void TractorBeam_render(void *tb);
extern "C" void RepairBeam_render(void *rb);
extern "C" void* AEGeometry_getMatrix(void*);
extern "C" void Matrix_mul(void*, void*, void*);
extern "C" void MatrixGetPosition(void*, void*);
extern "C" void *Ship_getCargo(void *ship, int item);
extern "C" void  Ship_removeCargo(void *ship, int item);
extern "C" void *PaintCanvas_MaterialGetMaterial(void *canvas, int handle);
extern "C" void  PaintCanvas_MeshChangeMaterial(void *canvas, unsigned int mesh, unsigned short mat);
extern "C" void  PaintCanvas_MeshChangeShaderAnimValue(void *canvas, void *mesh, float v, int i);
extern "C" void  PaintCanvas_MaterialCreate(void *canvas, unsigned short id, unsigned int *out);

// ---- setVisible_a1c88.cpp ----
void PlayerEgo_setVisible(PlayerEgo* self, bool v) {
  C(self, 0x32e) = v;
  C(self, 0x309) = v;
  return PlayerEgo_setVisible_ext();
}

// ---- boost_9aea4.cpp ----
void PlayerEgo_boost(PlayerEgo* self) {
  if (C(self, 0x13c) != 0) return;
  if (C(self, 0x146) == 0) return;
  if (I(self, 0x194) != 0) return;
  if (I(self, 0x138) < 0) return;
  float v = (float)I(self, 0xc8);
  I(self, 0x138) = 0;
  C(self, 0x13c) = 1;
  void* snd = *(void**)g_boost_fmod;
  F(self, 0xb8) = v;
  FModSound_play(*(void**)snd, P(self, 0xd4), 0, v);
}

// ---- getDriveChargeRate_9b00e.cpp ----
float PlayerEgo_getDriveChargeRate(PlayerEgo* self) {
  float d = (float)I(self, 0x1fc) / (float)I(self, 0x200);
  return d >= 0.0f ? d : 1.0f;
}

// ---- isAutoPilot_9b8a4.cpp ----
unsigned char PlayerEgo_isAutoPilot(PlayerEgo* self) { return UC(self, 0x158); }

// ---- goingToWormhole_9b8fc.cpp ----
bool PlayerEgo_goingToWormhole(PlayerEgo* self) {
  void* m = P(self, 0xc);
  void* r4 = P(self, 0x15c);
  void* lm = Level_getLandmarks(m);
  return r4 == P(P(lm, 4), 0xc);
}

// ---- getCurrentSecondaryWeaponIndex_9b7b0.cpp ----
int PlayerEgo_getCurrentSecondaryWeaponIndex(PlayerEgo* self) { return I(self, 0x10c); }

// ---- getHitpoints_9b190.cpp ----
int PlayerEgo_getHitpoints(PlayerEgo* self) { return PlayerEgo_getHitpoints_ext(P(self, 0)); }

// ---- isChargingDrive_9b048.cpp ----
unsigned char PlayerEgo_isChargingDrive(PlayerEgo* self) { return UC(self, 0x204); }

// ---- shouldSwitchToFreeLookCam_a1788.cpp ----
int PlayerEgo_shouldSwitchToFreeLookCam(PlayerEgo* self) {
  if (C(self, 0xb1) == 0) return 0;
  C(self, 0xb1) = 0;
  return 1;
}

// ---- getHandling_a212e.cpp ----
int PlayerEgo_getHandling(PlayerEgo* self) { return I(self, 0x154); }

// ---- setComputerControlled_9b7fa.cpp ----
void PlayerEgo_setComputerControlled(PlayerEgo* self, bool v) { C(self, 0x1f4) = v; }

// ---- getRocketBanking_a11ae.cpp ----
int PlayerEgo_getRocketBanking(PlayerEgo* self) { return I(self, 0x198); }

// ---- isDockingToDockingPoint_a069c.cpp ----
bool PlayerEgo_isDockingToDockingPoint(PlayerEgo* self) {
  if (C(self, 0x356) != 0) return I(self, 0x1c4) != 1;
  return false;
}

// ---- forceBoost_9af06.cpp ----
void PlayerEgo_forceBoost(PlayerEgo* self) {
  I(self, 0x138) = 0;
  C(self, 0x13c) = 1;
  I(self, 0xb8) = 0x41000000;
  I(self, 0xcc) = 0x2710;
  I(self, 0xd0) = 0;
}

// ---- isDead_9b17c.cpp ----
int PlayerEgo_isDead(PlayerEgo* self) { return PlayerEgo_getHitpoints(self) < 1; }

// ---- getAutoPilotTarget_9b872.cpp ----
int PlayerEgo_getAutoPilotTarget(PlayerEgo* self) { return I(self, 0x15c); }

// ---- goingToPlanet_9b878.cpp ----
bool PlayerEgo_goingToPlanet(PlayerEgo* self) {
  if (C(self, 0x158) != 0 && PlayerEgo_goingToStream(self) == 0 && PlayerEgo_goingToStation(self) == 0)
    return C(self, 0x160) == 0;
  return false;
}

// ---- getCloakRechargeRate_9b06a.cpp ----
float PlayerEgo_getCloakRechargeRate(PlayerEgo* self) {
  return 1.0f - (float)I(self, 0x20c) / (float)I(self, 0x368);
}

// ---- isDockedToStream_a167c.cpp ----
unsigned char PlayerEgo_isDockedToStream(PlayerEgo* self) { return UC(self, 0x1ed); }

// ---- getTargetFollowCamera_a2202.cpp ----
int PlayerEgo_getTargetFollowCamera(PlayerEgo* self) { return I(self, 0x88); }

// ---- isCloaked_9b0aa.cpp ----
unsigned char PlayerEgo_isCloaked(PlayerEgo* self) { return UC(self, 0x1ac); }

// ---- setFreeLookMode_a21ec.cpp ----
void PlayerEgo_setFreeLookMode(PlayerEgo* self, bool v) { C(self, 0xc4) = v; }

// ---- setThrust_9b86c.cpp ----
void PlayerEgo_setThrust(PlayerEgo* self, float v) { F(self, 0xbc) = v; }

// ---- readyForCloak_9b08c.cpp ----
bool PlayerEgo_readyForCloak(PlayerEgo* self) {
  if (C(self, 0x1ad) == 0) return false;
  return I(self, 0x208) >= I(self, 0x214);
}

// ---- getHackingGameDockIndex_a1142.cpp ----
int PlayerEgo_getHackingGameDockIndex(PlayerEgo* self) {
  int v = I(self, 0x1e8);
  if (v == 0) return -1;
  return PlayerEgo_getHackingGameDockIndex_ext(v);
}

// ---- setPosition_9b91c.cpp ----
void PlayerEgo_setPosition3(PlayerEgo* self, float x, float y, float z) {
  void* g = P(self, 8);
  char v[12];
  *(float*)(v + 0) = x;
  *(float*)(v + 4) = y;
  *(float*)(v + 8) = z;
  AEGeometry_setPosition(g, v);
}

// ---- dockToStream_a1732.cpp ----
void PlayerEgo_dockToStream(PlayerEgo* self, bool param) {
  if (param) { *(short*)B(self, 0x1ec) = 0x100; return; }
  I(self, 0xb8) = 0x40000000;
  PlayerEgo_setPosition3(self, F(self, 0xe0), F(self, 0xe4), F(self, 0xe8));
  C(self, 0x24) = 0;
  C(self, 0x145) = 0;
  *(short*)B(self, 0x1ec) = 0;
  return PlayerEgo_dockToStream_ext(self, 0);
}

// ---- hackingShuffle_a12e4.cpp ----
void PlayerEgo_hackingShuffle(PlayerEgo* self) {
  int v = I(self, 0x1e8);
  if (v != 0) PlayerEgo_hackingShuffle_ext(v);
}

// ---- getDockTransferedAmount_a2286.cpp ----
int PlayerEgo_getDockTransferedAmount(PlayerEgo* self) { return I(self, 0x364); }

// ---- goingToWaypoint_9b8f6.cpp ----
unsigned char PlayerEgo_goingToWaypoint(PlayerEgo* self) { return UC(self, 0x160); }

// ---- isDockedToDockingPoint_9a684.cpp ----
bool PlayerEgo_isDockedToDockingPoint(PlayerEgo* self) {
  if (C(self, 0x356) == 0) return false;
  return I(self, 0x1c4) == 1;
}

// ---- getDockTotalAmount_a2280.cpp ----
int PlayerEgo_getDockTotalAmount(PlayerEgo* self) { return I(self, 0x360); }

// ---- isInWormhole_9b94c.cpp ----
bool PlayerEgo_isInWormhole(PlayerEgo* self) {
  if (PlayerEgo_getHitpoints(self) > 0) return C(self, 0x25) != 0;
  return false;
}

// ---- setRocketControl_a19c6.cpp ----
void PlayerEgo_setRocketControl(PlayerEgo* self, void* gun, void* geo) {
  void* lvl = P(self, 0xc);
  I(self, 0x194) = (int)(intptr_t)gun;
  int psm_arg = I(lvl, 0x64);
  void* psm = P(lvl, 0x88);
  if (gun == 0) {
    ParticleSystemManager_systemSetMatrix(psm, psm_arg, B(P(self, 0), 4));
    I(self, 0x198) = 0;
    return;
  }
  void* m = AEGeometry_getReferenceMatrix(geo);
  ParticleSystemManager_systemSetMatrix(psm, psm_arg, m);
  return PlayerEgo_setRocketControl_ext(self, 0);
}

// ---- isMining_9add8.cpp ----
bool PlayerEgo_isMining(PlayerEgo* self) { return I(self, 0x1e4) != 0; }

// ---- turnHorizontal_a1194.cpp ----
void PlayerEgo_turnHorizontal(PlayerEgo* self, int a, float v) {
  if (v < 0.0f) { PlayerEgo_turnHorizontal_neg(self); return; }
  if (v > 0.0f) { PlayerEgo_turnHorizontal_pos(self); return; }
}

// ---- getThrust_a2274.cpp ----
int PlayerEgo_getThrust(PlayerEgo* self) { return I(self, 0xbc); }

// ---- getCloakRate_9afd0.cpp ----
float PlayerEgo_getCloakRate(PlayerEgo* self) {
  int off = 0x210;
  if (C(self, 0x1ac) == 0) off = 0x214;
  float a = (float)I(self, 0x208);
  float b = (float)I(self, off);
  float d = a / b;
  return d >= 0.0f ? d : 1.0f;
}

// ---- resetLastHP_9a6b2.cpp ----
void PlayerEgo_resetLastHP(PlayerEgo* self) {
  I(self, 0x130) = Player_getCombinedHP(P(self, 0));
}

// ---- setExhaustVisible_9a50c.cpp ----
void PlayerEgo_setExhaustVisible(PlayerEgo* self, bool param) {
  void* lvl = P(self, 0xc);
  C(self, 0x32f) = param;
  C(P(lvl, 0x80), 0) = param;
  unsigned* arr = (unsigned*)P(lvl, 0xa8);
  if (arr != 0) {
    for (unsigned i = 0; i < arr[0]; i++) {
      int* p = (int*)arr[1];
      ParticleSystemManager_enableSystemEmit2(P(P(self, 0xc), 0x80), p[i], param);
    }
  }
}

// ---- shouldSwitchToStandardCam_a1772.cpp ----
int PlayerEgo_shouldSwitchToStandardCam(PlayerEgo* self) {
  if (C(self, 0xb0) == 0) return 0;
  C(self, 0xb0) = 0;
  return 1;
}

// ---- resetMovement_9a69e.cpp ----
void PlayerEgo_resetMovement(PlayerEgo* self) {
  D(self, 0x278) = 0;
  D(self, 0x268) = 0;
  D(self, 0x270) = 0;
}

// ---- isHacking_a110c.cpp ----
bool PlayerEgo_isHacking(PlayerEgo* self) { return I(self, 0x1e8) != 0; }

// ---- setPosition_9b948.cpp ----
void PlayerEgo_setPosition(PlayerEgo* self) { return PlayerEgo_setPosition_v(self); }

// ---- resetChargingDrive_9b04e.cpp ----
void PlayerEgo_resetChargingDrive(PlayerEgo* self) { UC(self, 0x204) = 0; }

// ---- PauseEngineSound_9a5dc.cpp ----
// 000aa5dc: ldr r0,[r0,#0x0] ; b.w <ext>   — tail-call ext(self->m_pPlayer)
void PlayerEgo_PauseEngineSound(PlayerEgo* self) {
  return PlayerEgo_PauseEngineSound_ext(P(self, 0));
}

// ---- isInFreeLookMode_a21e6.cpp ----
unsigned char PlayerEgo_isInFreeLookMode(PlayerEgo* self) { return UC(self, 0xc4); }

// ---- setActive_9b7f4.cpp ----
void PlayerEgo_setActive(PlayerEgo* self, bool) { return PlayerEgo_setActive_ext(P(self, 0)); }

// ---- alignToHorizon_a12f2.cpp ----
void PlayerEgo_alignToHorizon(PlayerEgo* self) { UC(self, 0x2f4) = 1; }

// ---- setAutoTurret_9b2a2.cpp ----
void PlayerEgo_setAutoTurret(PlayerEgo* self, bool on) {
  C(self, 0x355) = on;
  if (!on) PlayerEgo_setAutoTurret_ext(P(self, 0), 2);
}

// ---- hitCamera_a06ec.cpp ----
void PlayerEgo_hitCamera(PlayerEgo* self) {
  C(self, 0x32c) = 1;
  int cam = I(self, 0x88);
  I(self, 0x328) = 0;
  return PlayerEgo_hitCamera_ext(cam);
}

// ---- hackingWon_a1118.cpp ----
int PlayerEgo_hackingWon(PlayerEgo* self) {
  int v = I(self, 0x1e8);
  if (v == 0) return 0;
  return PlayerEgo_hackingWon_ext(v);
}

// ---- lostMiningGame_a19c0.cpp ----
unsigned char PlayerEgo_lostMiningGame(PlayerEgo* self) { return UC(self, 0x39b); }

// ---- getCurrentMiningAmount_9af24.cpp ----
int PlayerEgo_getCurrentMiningAmount(PlayerEgo* self) {
  int v = I(self, 0x1e4);
  if (v == 0) return 0;
  return PlayerEgo_getCurrentMiningAmount_ext(v);
}

// ---- setDockingState_a179e.cpp ----
void PlayerEgo_setDockingState(PlayerEgo* self, int s) {
  if (s == 2 && I(self, 0x1c4) == 1) C(self, 0xb2) = 1;
  I(self, 0x1c4) = s;
}

// ---- hackingRotateLCW_a12b8.cpp ----
void PlayerEgo_hackingRotateLCW(PlayerEgo* self) {
  if (I(self, 0x1e8) != 0 && HackingGame_isRotating(I(self, 0x1e8)) == 0
      && HackingGame_gameWon(I(self, 0x1e8)) == 0)
    PlayerEgo_hackingRotateLCW_ext(I(self, 0x1e8), 1);
}

// ---- isInDockingProcedure_a1152.cpp ----
bool PlayerEgo_isInDockingProcedure(PlayerEgo* self) {
  if (C(self, 0x1c0) != 0) return true;
  return C(self, 0x356) != 0;
}

// ---- setSpeed_a220e.cpp ----
void PlayerEgo_setSpeed(PlayerEgo* self, float v) { F(self, 0xb8) = v; }

// ---- ResumeEngineSound_9a5e2.cpp ----
void PlayerEgo_ResumeEngineSound(PlayerEgo* self) { return PlayerEgo_ResumeEngineSound_ext(P(self, 0), 0); }

// ---- addNukeVolatileForce_9a9f8.cpp ----
void PlayerEgo_addNukeVolatileForce(PlayerEgo* self, float v) {
  void* p = P(self, 0);
  F(p, 0x60) = F(p, 0x60) + v * 3.0f;
}

// ---- explode_a13e0.cpp ----
extern void* g_explode_obj;
extern void (*g_explode_fn)(void*, int);
void PlayerEgo_explode(PlayerEgo* self) {
  ParticleSystemManager_enableSystemEmit3(P(P(self, 0xc), 0x74), I(self, 0x2fc), 1);
  if (I(self, 0x8c) != 0) return;
  TargetFollowCamera_setActive(P(self, 0x88), 0);
  void* e = operator_new_(0x68);
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

// ---- isDockingToStream_9f49a.cpp ----
unsigned char PlayerEgo_isDockingToStream(PlayerEgo* self) { return UC(self, 0x1ec); }

// ---- goingToStream_9b8aa.cpp ----
int PlayerEgo_goingToStream(PlayerEgo* self) {
  void* m = P(self, 0xc);
  void* r4 = P(self, 0x15c);
  void* lm = Level_getLandmarks(m);
  return r4 == P(P(lm, 4), 4);
}

// ---- GetDirVector_9b7dc.cpp ----
Vec3 PlayerEgo_GetDirVector(PlayerEgo* self) { return AEGeometry_getDirection(P(self, 8)); }

// ---- hideShipForFirstPersonCameraView_a1c56.cpp ----
void PlayerEgo_hideShipForFirstPersonCameraView(PlayerEgo* self, bool param) {
  unsigned char r1 = param;
  C(self, 0x32d) = r1;
  unsigned char nr = r1 ^ 1;
  C(self, 0x309) = (C(self, 0x32e) != 0) & nr;
  *(char*)P(P(self, 0xc), 0x80) = nr & (C(self, 0x32f) != 0);
}

// ---- changeThrust_a2214.cpp ----
void PlayerEgo_changeThrust(PlayerEgo* self, float v) {
  float n = F(self, 0xbc) + v;
  float p3 = 1.0f;
  if (n < 1.0f) p3 = 0.0f;
  if (n < 0.0f) p3 = 0.0f;
  float p2 = p3;
  if (n < 1.0f) p2 = n;
  if (n < 0.0f) p2 = p3;
  F(self, 0xbc) = p2;
}

// ---- deleteHackingGame_a1126.cpp ----
struct HackingGame { ~HackingGame(); };
void PlayerEgo_deleteHackingGame(PlayerEgo* self) {
  HackingGame* g = (HackingGame*)P(self, 0x1e8);
  if (g != 0) delete g;
  P(self, 0x1e8) = 0;
}

// ---- explosionEnded_a15d4.cpp ----
bool PlayerEgo_explosionEnded(PlayerEgo* self) {
  if (I(self, 0x8c) == 0) return true;
  return 8000 < I(self, 0x2f8);
}

// ---- getBoostPercentage_9af48.cpp ----
float PlayerEgo_getBoostPercentage(PlayerEgo* self) {
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

// ---- GetUpVector_9b7d0.cpp ----
Vec3 PlayerEgo_GetUpVector(PlayerEgo* self) { return AEGeometry_getUpVector(P(self, 8)); }

// ---- isDockedToPlanet_a1720.cpp ----
bool PlayerEgo_isDockedToPlanet(PlayerEgo* self) { return 3000 < I(self, 0x1f0); }

// ---- emergencySystemActive_a10f8.cpp ----
bool PlayerEgo_emergencySystemActive(PlayerEgo* self) { return 0 < I(self, 0x30c); }

// ---- isDockingToAsteroid_a161e.cpp ----
bool PlayerEgo_isDockingToAsteroid(PlayerEgo* self) {
  if (C(self, 0x1c0) != 0) return I(self, 0x1c4) != 1;
  return false;
}

// ---- getBoostRate_9afa0.cpp ----
float PlayerEgo_getBoostRate(PlayerEgo* self) {
  float d = (float)I(self, 0x138) / (float)I(self, 0xd0);
  float r = d + 1.0f;
  if (0.0f < d) r = 1.0f;
  return r;
}

// ---- driveReady_9b03a.cpp ----
bool PlayerEgo_driveReady(PlayerEgo* self) { return I(self, 0x1fc) >= I(self, 0x200); }

// ---- turnVertical_a1168.cpp ----
void PlayerEgo_turnVertical(PlayerEgo* self, int a, float v) {
  if (v < -0.0f) { PlayerEgo_turnVertical_neg(); return; }
  if (v > 0.0f) { PlayerEgo_turnVertical_pos(self); return; }
}

// ---- isInTurretMode_9aa10.cpp ----
unsigned char PlayerEgo_isInTurretMode(PlayerEgo* self) { return UC(self, 0x1a0); }

// ---- startJumpDrive_9a6c4.cpp ----
void PlayerEgo_startJumpDrive(PlayerEgo* self) {
  if (C(self, 0x204) != 0) return;
  FModSound_play(*(void**)g_FMod_singleton, 0x21, 0, 0, 0);
  Hud_hudEvent(I(self, 0x220), 0x19, self, 0);
  I(self, 0x1fc) = 0;
  C(self, 0x204) = 1;
}

// ---- isInRocketControl_9ade4.cpp ----
bool PlayerEgo_isInRocketControl(PlayerEgo* self) { return I(self, 0x194) != 0; }

// ---- getSpeed_a227a.cpp ----
int PlayerEgo_getSpeed(PlayerEgo* self) { return I(self, 0xb8); }

// ---- getVolatileForce_9a9c4.cpp ----
float PlayerEgo_getVolatileForce(PlayerEgo* self) {
  float f = F(P(self, 0), 0x60);
  float r = 0.0f;
  if (!(f < 0.0f)) {
    r = 1.0f;
    if (!(f > 1.0f)) r = f;
  }
  return r;
}

// ---- isDockingToPlanet_9f494.cpp ----
unsigned char PlayerEgo_isDockingToPlanet(PlayerEgo* self) { return UC(self, 0x1ee); }

// ---- isChargingCloak_9b056.cpp ----
unsigned char PlayerEgo_isChargingCloak(PlayerEgo* self) { return UC(self, 0x1ad); }

// ---- isDockedToMiningPlant_a1634.cpp ----
extern void* g_mining_status;
bool PlayerEgo_isDockedToMiningPlant(PlayerEgo* self) {
  if (C(self, 0x356) != 0 && I(self, 0x1c4) == 1) {
    if (Mission_isEmpty(Status_getMission(g_mining_status)) != 0
        && Status_inAlienOrbit(g_mining_status) == 0) {
      return Station_getIndex(Status_getStation(g_mining_status)) == 0x67;
    }
  }
  return false;
}

// ---- setCurrentSecondaryWeaponIndex_9b7b8.cpp ----
extern PlayerEgo* g_PlayerEgo_singleton;
void PlayerEgo_setCurrentSecondaryWeaponIndex(PlayerEgo* self, int idx) {
  I(self, 0x10c) = idx;
  I(g_PlayerEgo_singleton, 0xf4) = idx;
}

// ---- removeRoute_9b15c.cpp ----
struct Route { ~Route(); };
void PlayerEgo_removeRoute(PlayerEgo* self) {
  Route* r = (Route*)P(self, 0xfc);
  if (r != 0) delete r;
  P(self, 0xfc) = 0;
}

// ---- setRoute_9b150.cpp ----
void PlayerEgo_setRoute(PlayerEgo* self, int v) { I(self, 0xfc) = v; }

// ---- boosting_9af32.cpp ----
unsigned char PlayerEgo_boosting(PlayerEgo* self) { return UC(self, 0x13c); }

// ---- isDockedToAsteroid_a1604.cpp ----
bool PlayerEgo_isDockedToAsteroid(PlayerEgo* self) {
  if (C(self, 0x1c0) == 0) return false;
  return I(self, 0x1c4) == 1;
}

// ---- goingToStation_9b8c8.cpp ----
int PlayerEgo_goingToStation(PlayerEgo* self) {
  void* lm = Level_getLandmarks(P(self, 0xc));
  if (P(P(lm, 4), 0) == 0) return false;
  void* r4 = P(self, 0x15c);
  lm = Level_getLandmarks(P(self, 0xc));
  return r4 == P(P(lm, 4), 0);
}

// ---- getCloakingPercentage_9c170.cpp ----
float PlayerEgo_getCloakingPercentage(PlayerEgo* self) {
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

// ---- getBoostSpeed_9af00.cpp ----
int PlayerEgo_getBoostSpeed(PlayerEgo* self) { return I(self, 0xc8); }

// ---- addGun_9b778.cpp ----
void PlayerEgo_addGun(PlayerEgo* self, void* gun, int x) {
  Player_addGun(P(self, 0), gun, x);
  return PlayerEgo_addGun_ext(self);
}

// ---- aboutToReachAutoTarget_a15ee.cpp ----
unsigned char PlayerEgo_aboutToReachAutoTarget(PlayerEgo* self) { return UC(self, 0x330); }

// ---- autoTurretIsEnabled_9b2b4.cpp ----
unsigned char PlayerEgo_autoTurretIsEnabled(PlayerEgo* self) { return UC(self, 0x355); }

// ---- readyToBoost_9af38.cpp ----
bool PlayerEgo_readyToBoost(PlayerEgo* self) { return -1 < I(self, 0x138); }

// ---- endExplosion_a15c6.cpp ----
void PlayerEgo_endExplosion(PlayerEgo* self) {
  int v = I(self, 0x8c);
  if (v != 0) PlayerEgo_endExplosion_ext(v);
}

// ---- isLandingOrTakingOff_a06b2.cpp ----
bool PlayerEgo_isLandingOrTakingOff(PlayerEgo* self) {
  if (C(self, 0x356) == 0) return false;
  return (U(self, 0x1c4) | 1) == 3;
}

// ---- setFreeze_9b800.cpp ----
void PlayerEgo_setFreeze(PlayerEgo* self, bool v) { C(self, 0x24) = v; }

// ---- getHUD_a21e0.cpp ----
int PlayerEgo_getHUD(PlayerEgo* self) { return I(self, 0x220); }

// ---- getRoute_9b156.cpp ----
int PlayerEgo_getRoute(PlayerEgo* self) { return I(self, 0xfc); }

// ---- setCollide_9b0b0.cpp ----
void PlayerEgo_setCollide(PlayerEgo* self, bool v) { C(self, 0x144) = v; }

// ---- tryToStartEmergencySystem_a106c.cpp ----
extern void* g_emerg_status;
extern void* g_emerg_fmod;
int PlayerEgo_tryToStartEmergencySystem(PlayerEgo* self) {
  if (I(self, 0xac) == 0 || I(self, 0x30c) != 0) return 0;
  if (Player_getHitpoints(P(self, 0)) > 1) return 0;
  Player_setHitpoints(P(self, 0), 1);
  I(self, 0x30c) = I(self, 0x310);
  Player_setVulnerable(P(self, 0), 0);
  void* s1 = Status_getShip(g_emerg_status);
  int eq = Ship_getFirstEquipmentOfSort(Status_getShip(g_emerg_status), 0x1b);
  float f = Ship_removeEquipment(s1, eq);
  FModSound_play(*(void**)g_emerg_fmod, (void*)0x45b, 0, f, 0);
  return 1;
}

// ---- hasAutoTurret_9b29c.cpp ----
unsigned char PlayerEgo_hasAutoTurret(PlayerEgo* self) { return UC(self, 0x180); }

// ---- collidesWithStation_a1106.cpp ----
unsigned char PlayerEgo_collidesWithStation(PlayerEgo* self) { return UC(self, 0x234); }

// ---- stopPlanetDock_a16f4.cpp ----
void PlayerEgo_stopPlanetDock(PlayerEgo* self) {
  TargetFollowCamera_setLookAtCam(I(self, 0x88), 0);
  C(self, 0x144) = 1;
  C(self, 0x1ee) = 0;
  PlayerEgo_stopBoost(self);
  I(self, 0xb8) = 0x40000000;
}

// ---- isRechargingCloak_9b05c.cpp ----
bool PlayerEgo_isRechargingCloak(PlayerEgo* self) { return 0 < I(self, 0x20c); }

// ---- hasVolatileGoods_9a9bc.cpp ----
// 000aa9bc: ldrb.w r0,[r0,#0x398] ; bx lr
unsigned char PlayerEgo_hasVolatileGoods(PlayerEgo* self) {
  return UC(self, 0x398);
}

// ---- hackingRotateRCW_a128c.cpp ----
void PlayerEgo_hackingRotateRCW(PlayerEgo* self) {
  if (I(self, 0x1e8) != 0 && HackingGame_isRotating(I(self, 0x1e8)) == 0
      && HackingGame_gameWon(I(self, 0x1e8)) == 0)
    PlayerEgo_hackingRotateRCW_ext(I(self, 0x1e8), 1);
}

// ---- hasCloak_9a9b0.cpp ----
bool PlayerEgo_hasCloak(PlayerEgo* self) { return P(self, 0x1b0) != 0; }

// ---- isBoostRefreshed_9ae80.cpp ----
int PlayerEgo_isBoostRefreshed(PlayerEgo* self) {
  if (C(self, 0x13c) != 0) return 0;
  if (C(self, 0x146) == 0) return 0;
  if (I(self, 0x138) > -1) return 1;
  return 0;
}

// ---- getPosition_9b7e8.cpp ----
Vec3 PlayerEgo_getPosition(PlayerEgo* self) { return AEGeometry_getPosition(P(self, 8)); }

// ---- initManeuver_a1abc.cpp ----
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

extern const float g_PE_rollNudge;    // 0xb1b6c
extern const float g_PE_strafeDist;   // 0xb1b70 distance scalar

extern "C" void PlayerEgo_initManeuver(PlayerEgo *self, int type)
{
    if ((unsigned)(type - 1) < 2 && C(self, 0x398) != 0) {
        void *player = P(self, 0x0);
        F(player, 0x60) = F(player, 0x60) + g_PE_rollNudge;
    }

    if (I(self, 0x334) == 0) {
        I(self, 0x334) = type;
        P(self, 0x350) = 0;
        if (type == 3) {
            float pos[3];
            PlayerEgo_getPosition(self, pos);

            float dir[3];
            AEGeometry_getDirection(P(self, 0x8), dir);

            float scaled[3];
            Vec_scale(scaled, dir, g_PE_strafeDist);

            float target[3];
            Vec_sub(target, pos, scaled);
            Vec_assign((char *)self + 0x338, target);

            AEGeometry_getDirection(P(self, 0x8), target);
            Vec_assign((char *)self + 0x344, target);
        }
    }
}

// ---- refillGunDelay_a15fc.cpp ----
void PlayerEgo_refillGunDelay(PlayerEgo* self) { return PlayerEgo_refillGunDelay_ext(P(self, 0), 0); }

// ---- checkForTurret_9b340.cpp ----
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

extern "C" void PlayerEgo_checkForTurret(PlayerEgo *self)
{
    if (C(self, 0x170) != 0)
        return;
    int avail = Player_gunAvailable(P(self, 0x0));
    C(self, 0x170) = (unsigned char)avail;
    if (avail == 0)
        return;

    C(self, 0x180) = 0;
    int equip = Ship_getEquipment(Status_getShip(), 2);
    void *item = *(void **)(equip + 4);
    I(self, 0x1f8) = (int)((double)Item_getAttribute(*(void **)(item)) * 1.5);

    int idx = Item_getIndex(*(void **)(item));
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

    void *baseGeo = AEGeometry_newMesh(base, canvas, false);
    P(self, 0xf4) = baseGeo;
    void *yawGeo = AEGeometry_newMesh(barrel, canvas, false);
    P(self, 0x34) = yawGeo;
    AEGeometry_setRotationOrder(yawGeo, 2);
    void *muzzleRoot = AEGeometry_new(canvas);
    P(self, 0x38) = muzzleRoot;

    if (muzzle != -1) {
        void *g = AEGeometry_newMesh((unsigned short)muzzle, canvas, false);
        AEGeometry_addChild(P(self, 0xf4), g);
        operator_delete_(AEGeometry_dtor(g));
    }
    if (child != -1) {
        void *g = AEGeometry_newMesh((unsigned short)child, canvas, false);
        AEGeometry_addChild(P(self, 0x34), g);
        operator_delete_(AEGeometry_dtor(g));
    }
    if (extra != -1) {
        void *g = AEGeometry_newMesh((unsigned short)extra, canvas, false);
        AEGeometry_addChild(P(self, 0x34), g);
        operator_delete_(AEGeometry_dtor(g));
    }
    if (extra2 != -1) {
        void *g = AEGeometry_newMesh((unsigned short)extra2, canvas, false);
        P(self, 0x3c) = g;
        AEGeometry_addChild(P(self, 0x34), g);
        void *tf = PaintCanvas_TransformGetTransform(*(unsigned int *)canvasHolder);
        Transform_SetVisible(tf, C(self, 0x2c4) != 0);
    }

    void *tf = PaintCanvas_TransformGetTransform(*(unsigned int *)canvasHolder);
    F(tf, 0xe0) = g_PE_cft_transformVal;
    tf = PaintCanvas_TransformGetTransform(*(unsigned int *)canvasHolder);
    Transform_SetAnimationState(tf, 2, 0);

    AEGeometry_setPosition(P(self, 0xf4), (char *)self + 0x23c);
    AEGeometry_setPosition(P(self, 0x34), (char *)self + 0x23c);

    PE_cft_place(self, idx);

    AEGeometry_addChild(P(self, 0x38), P(self, 0xf4));
    AEGeometry_addChild(P(self, 0x38), P(self, 0x34));

    if (P(self, 0x2c0) != 0 && C(self, 0x170) != 0) {
        PaintCanvas_MeshCloneMaterial(canvas, I(P(self, 0xf4), 0x1c), &U(self, 0x388));
        PE_cft_finishMaterials(canvas, I(P(self, 0x34), 0x1c), (char *)self + 0x38c);
    }
}

// ---- pitchAllPrimaryGuns_a2208.cpp ----
void PlayerEgo_pitchAllPrimaryGuns(PlayerEgo* self, float) { return PlayerEgo_pitchAllPrimaryGuns_ext(P(self, 0)); }

// ---- stopShooting_9adf0.cpp ----
void PlayerEgo_stopShooting(PlayerEgo* self, int param) {
  if (C(self, 0x1a0) != 0) {
    stopShooting_extA(P(self, 0), 2);
    return;
  }
  if (PlayerEgo_isDead(self) != 0) return;
  void* p = P(self, 0);
  if (param == 1) {
    stopShooting_extB(p, 1, I(self, 0x10c));
    return;
  }
  stopShooting_extA(p, param);
}

// ---- shake_a2134.cpp ----
// PlayerEgo::shake(int amount)
//   Produces a random camera-shake offset whose magnitude scales with `amount`
//   and the current shake intensity (field 0x134), then applies it to the
//   target-follow camera (field 0x8) via the camera shake entry point.


__attribute__((visibility("hidden"))) extern void **g_PE_rng;        // AERandom singleton
extern const float g_PE_shakeDiv;                                    // 0xb21d4 normaliser

extern "C" void PlayerEgo_shake(PlayerEgo *self, int amount)
{
    void *cam = P(self, 0x8);
    float a = (float)amount / g_PE_shakeDiv;
    float intensity = (float)(I(self, 0x134) << 1);

    int range = (int)(a * intensity);
    if (range < 2)
        range = 1;
    int span = range << 1;

    void *rng = *g_PE_rng;
    float dx = (float)(AERandom_next(rng, span) - range);
    float dy = (float)(AERandom_next(rng, span) - range);
    float dz = (float)(AERandom_next(rng, span) - range);
    Camera_shake(cam, dx, dy, dz);
}

// ---- setRotation_a137c.cpp ----
void PlayerEgo_setRotation(PlayerEgo* self, float rx, float ry, float rz) {
  char local[60];
  F(self, 0x2e8) = rx;
  F(self, 0x2ec) = ry;
  F(self, 0x2f0) = rz;
  void* t = *(void**)g_setRotation_transform;
  void* m = TransformGetLocal(t, I(P(self, 4), 0xc));
  MatrixSetRotation(local, m, F(self, 0x2e8), F(self, 0x2ec), F(self, 0x2f0));
}

// ---- StopEngineSound_9a5ec.cpp ----
extern void* g_engine_status;
extern void* g_engine_fmod;
void PlayerEgo_StopEngineSound(PlayerEgo* self) {
  if (C(self, 0x356) == 0 || I(self, 0x1c4) != 1) {
    if (Ship_getFirstEquipmentOfSort(Status_getShip(g_engine_status), 0x26) != 0
        && Status_inAlienOrbit(g_engine_status) == 0) {
      void* obj = g_engine_status;
      int idx = Station_getIndex(Status_getStation(obj));
      int cm = Status_getCurrentCampaignMission(g_engine_status);
      float g = Status_getGammaRayDamagePerSecond(obj, idx, cm);
      if (0.0f < g && I(self, 0xb4) != -1) {
        FModSound_play(*(void**)g_engine_fmod, P(self, 0xb4), 0, g, 0);
      }
    }
  }
  return PlayerEgo_StopEngineSound_ext(P(self, 0));
}

// ---- startSmokeEmission_a1598.cpp ----
void PlayerEgo_startSmokeEmission(PlayerEgo* self) {
  int v = I(self, 0x300);
  if (v < 0) return;
  ParticleSystemManager_enableSystemEmit(P(P(self, 0xc), 0x78), v, 1);
  PlayerEgo_startSmokeEmission_ext(P(P(self, 0xc), 0x84), I(self, 0x304), 1);
}

// ---- down_a0b90.cpp ----
// PlayerEgo::down(int frameTime, float delta)
//   Pitch-down input handler. Dispatch mirrors left()/right() but on the pitch
//   axis: mining game (delegated, with a global gate selecting which veneer),
//   turret free-look (accumulating into the look-pitch 0x1a8 and the turret
//   pitch 0x1a4, each rotating its geometry), scripted maneuver (0x7c), or the
//   normal load-scaled pitch ramp on accumulator 0x278 (direction flag 0x100=-1).

extern "C" float MiningGame_steerY(void *mg, float d);                 // 0x1abb18 veneer
extern "C" float MiningGame_steerYAlt(void);                          // 0x1abb38 veneer
extern "C" void  AEGeometry_rotateX(void *geo, float ang, int a, float b); // 0x72160
extern "C" float AEGeometry_rotateXr(void *geo, float ang, int a, int b);  // 0x1abb28 veneer

__attribute__((visibility("hidden"))) extern char **g_PE_d_miningGate; // 0xb0bb0 -> flags (+0x10)
extern const float g_PE_d_eps;      // 0xb0dc0 free-look limit
extern const float g_PE_d_lookK1;   // 0xb0dc4
extern const float g_PE_d_lookK2;   // 0xb0dcc
extern const float g_PE_d_manK;     // 0xb0dd0
extern const float g_PE_d_loadK;    // 0xb0dd4
extern const float g_PE_d_loadB;    // 0xb0dd8
extern const float g_PE_d_rateK;    // 0xb0ddc

extern "C" float PlayerEgo_down(PlayerEgo *self, int frameTime, float delta)
{
    if (P(self, 0x1e4) != 0) {
        if (C(*g_PE_d_miningGate, 0x10) == 0)
            return MiningGame_steerYAlt();
        return MiningGame_steerY(P(self, 0x1e4), -delta);
    }

    if (C(self, 0x1a0) != 0) {
        float ft = (float)frameTime;
        if (F(self, 0x1a8) < g_PE_d_eps) {
            float ang = ft * delta + F(self, 0x1a8);
            F(self, 0x1a8) = ang;
            AEGeometry_rotateX(P(self, 0x28), ang * g_PE_d_lookK1 * g_PE_d_lookK2, 0, 0);
        }
        float p = F(self, 0x1a4);
        if (p < g_PE_d_eps) {
            float half = ft * delta * 0.5f;
            float ang = half * g_PE_d_lookK1 * g_PE_d_lookK2;
            F(self, 0x1a4) = half + p;
            return AEGeometry_rotateXr(P(self, 0x19c), ang, 0, 0);
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
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
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

// ---- approachDockingPoint_9fcb8.cpp ----
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
    void *mission = Status_getMission();
    void *campaign = Status_getCampaignMission();
    void *fixed = *g_PE_adp_fixedObj;

    if (Mission_isEmpty(mission) == 0 && Mission_getType(mission) == 0xf
        && PlayerFixedObject_getDockingType(fixed) == 1) {
        if (Ship_hasCargo(Status_getShip(), Mission_getProductionGoodIndex(mission)) != 0) {
            int amount = Item_getAmount(Ship_getCargo(Status_getShip(),
                                        Mission_getProductionGoodIndex(mission)));
            I(self, 0x180) = amount;
            int need = Mission_getProductionGoodAmount(mission)
                       - Level_getNumDeliveredOre(P(self, 0xc));
            if (need < amount) I(self, 0x180) = need;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x29;
        }
        return 0;
    }

    if (Mission_isEmpty(mission) == 0
        && (Mission_getType(mission) == 0xb8 || Mission_getType(mission) == 0xa8)
        && PlayerFixedObject_getDockingType(fixed) == 2) {
        // passenger drop-off
        int carried = I(fixed, 0x178);
        int maxPax = Ship_getMaxPassengers(Status_getShip());
        if (maxPax > 0 && carried < maxPax) {
            int avail = (Mission_getType(mission) == 0xa8)
                        ? Mission_getStatusValue(mission) : maxPax;
            avail -= carried;
            I(self, 0x180) = avail;
            int status = Mission_getStatusValue(mission) - carried;
            if (status < avail) I(self, 0x180) = status;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x23;
        }
        if (Mission_getType(mission) != 0xa8 && Ship_getMaxPassengers(Status_getShip()) == 0)
            return 0x2b;
        return 0;
    }

    if (Mission_isEmpty(mission) == 0 && Mission_getType(mission) == 0xb8
        && PlayerFixedObject_getDockingType(fixed) == 1) {
        int n = I(fixed, 0x178);
        if (n > 0) {
            I(self, 0x184) = 0;
            I(self, 0x180) = n;
            return 0x25;
        }
        return 0;
    }

    if (campaign != 0 && Mission_isEmpty(campaign) == 0
        && (Mission_getType(campaign) == 0xa7 || Mission_getType(campaign) == 0xae)
        && PlayerFixedObject_getDockingType(fixed) == 1) {
        if (Ship_hasCargo(Status_getShip(), Mission_getProductionGoodIndex(campaign)) != 0) {
            int amount = Item_getAmount(Ship_getCargo(Status_getShip(),
                                        Mission_getProductionGoodIndex(campaign)));
            I(self, 0x180) = amount;
            int need = Mission_getProductionGoodAmount(campaign) - Mission_getStatusValue(campaign);
            if (need < amount) I(self, 0x180) = need;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x29;
        }
    }
    return 0;
}

extern "C" int PlayerEgo_approachDockingPoint(PlayerEgo *self, void *hud, int /*hud2*/, void *radar)
{
    if (KIPlayer_isDying(self) != 0)
        return 0;

    int state = I(self, 0x314);

    if (state == 0) {
        void *station = P(self, 0x1e0);
        float pos[3];
        PlayerEgo_getPosition(self, pos);
        void *nav = KIPlayer_getNearestNavigationPoint(station, pos, (char *)self + 0x350);
        if (nav != 0) {
            if (P(self, 0x380) != nav) {
                if (P(self, 0x380) != 0)
                    SpacePoint_giveFree(P(self, 0x380));
                P(self, 0x380) = nav;
                SpacePoint_take(nav);
            }
            int dist = PE_adp_approach(self, station);
            if (dist < I(self, 0x1cc)) {
                I(self, 0x314) = 2;
                TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
                TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);
            }
        }
        PE_adp_apply(self);
        return 0;
    }

    if (state == 2) {
        if (C(self, 0x2c4) != 0)
            PlayerEgo_setTurretMode(self, 0);
        int dist = PE_adp_glide(self);
        int radius = g_PE_adp_dockRadius[Ship_getIndex(Status_getShip())];
        if (dist < radius) {
            int ev = adp_arrivalEvent(self, P(self, 0x1e0));
            if (ev != 0)
                Hud_hudEvent(hud, ev, self);
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
            TargetFollowCamera_setActive(P(self, 0x88), true);
            TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
            TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);
            LevelScript_resetCamera(P(self, 0x10));
            Player_resetGunDelay(P(self, 0x0), 0);
            I(self, 0x314) = 0;
            if (P(self, 0x380) != 0) {
                SpacePoint_giveFree(P(self, 0x380));
                P(self, 0x380) = 0;
            }
            if (P(self, 0x1e8) != 0) {
                operator_delete_(HackingGame_dtor(P(self, 0x1e8)));
                P(self, 0x1e8) = 0;
                Hud_setHackingGameActive(hud, false);
            }
            return 1;        // docking complete
        }
        PE_adp_apply(self);
    }
    return 0;
}

// ---- setLevel_9b0b8.cpp ----
extern void* g_setLevel_status;
void PlayerEgo_setLevel(PlayerEgo* self, void* level) {
  I(self, 0xc) = (int)(intptr_t)level;
  void* src = P(level, 0x74);
  void* gm = AEGeometry_getMatrix2(P(self, 8));
  void* sys = ParticleSystemManager_addSystem(src, gm, 9, 0);
  I(self, 0x2fc) = (int)(intptr_t)sys;
  ParticleSystemManager_enableSystemEmit3(P(P(self, 0xc), 0x74), (int)(intptr_t)sys, 0);
  if (Status_getCurrentCampaignMission(g_setLevel_status) > 1) return;
  void* src2 = P(P(self, 0xc), 0x78);
  void* gm2 = AEGeometry_getMatrix2(P(self, 8));
  void* sys2 = ParticleSystemManager_addSystem(src2, gm2, 0xf, 0);
  I(self, 0x300) = (int)(intptr_t)sys2;
  ParticleSystemManager_enableSystemEmit3(P(P(self, 0xc), 0x78), (int)(intptr_t)sys2, 0);
  void* src3 = P(P(self, 0xc), 0x84);
  void* gm3 = AEGeometry_getMatrix2(P(self, 8));
  void* sys3 = ParticleSystemManager_addSystem(src3, gm3, 0x2a, 0);
  I(self, 0x304) = (int)(intptr_t)sys3;
  return PlayerEgo_setLevel_ext(P(P(self, 0xc), 0x84), (int)(intptr_t)sys3, 0);
}

// ---- setDockingCamera_9aa18.cpp ----
// PlayerEgo::setDockingCamera()
//   Lazily builds the docking camera rig (a PaintCanvas camera plus a small
//   chain of AEGeometry nodes used to orient/offset the view) on first use,
//   seeding a default offset vector at 0x224 when none is set, then positions
//   the leaf node at the dock offset (0x148), copies the ship matrix onto it,
//   and makes the camera current.

extern "C" void *AEGeometry_new(void *canvas);            // operator new + AEGeometry::AEGeometry
extern "C" void  PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam); // 0x1ab2d8 veneer

__attribute__((visibility("hidden"))) extern void **g_PE_dc_canvas;   // 0xaab84 PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_PE_dc_canvas2;  // 0xaab8c PaintCanvas (current)
extern const float g_PE_dc_fovNormal;   // 0xaab68
extern const float g_PE_dc_fovAlien;    // 0xaab6c
extern const unsigned int g_PE_dc_defX; // 0xaab78 default offset lo
extern const unsigned int g_PE_dc_defY; // 0xaab7c default offset hi

extern "C" void PlayerEgo_setDockingCamera(PlayerEgo *self)
{
    if (P(self, 0x178) == 0) {
        void **holder = g_PE_dc_canvas;
        PaintCanvas_CameraCreate(*holder, &U(self, 0x174));
        unsigned int cam = (unsigned int)(unsigned long)*holder;

        float fov = (Status_inAlienOrbit() != 0) ? g_PE_dc_fovAlien : g_PE_dc_fovNormal;
        PaintCanvas_CameraSetPerspective(cam, fov, 0.0f, g_PE_dc_fovAlien);

        void *node = AEGeometry_new(*holder);
        P(self, 0x178) = node;
        AEGeometry_setRotationOrder(node, 2);

        // seed the default rig offset (0x224..0x22c) only when fully zero.
        if (F(self, 0x224) == 0.0f && F(self, 0x228) == 0.0f && F(self, 0x22c) == 0.0f) {
            U(self, 0x228) = g_PE_dc_defX;
            U(self, 0x22c) = g_PE_dc_defY;
        }
        AEGeometry_translate(P(self, 0x178), (char *)self + 0x224);

        void *mid = AEGeometry_new(*holder);
        P(self, 0x19c) = mid;
        AEGeometry_translate(mid, (char *)mid + 0xc);
        AEGeometry_addChild(P(self, 0x178), *(void **)((char *)P(self, 0x19c) + 0xc));

        void *leaf = AEGeometry_new(*holder);
        P(self, 0x17c) = leaf;
        AEGeometry_addChild(P(self, 0x178), *(void **)((char *)P(self, 0x178) + 0xc));
    }

    AEGeometry_setPosition(P(self, 0x17c), (char *)self + 0x148);
    void *leaf = P(self, 0x17c);
    AEGeometry_setMatrix(leaf, AEGeometry_getMatrix(P(self, 0x8)));

    PaintCanvas_CameraSetCurrent(*g_PE_dc_canvas2, U(self, 0x174));
}

// ---- right_a0704.cpp ----
// PlayerEgo::right(int frameTime, float delta)
//   Yaw-right input handler -- the mirror of left(): same mode dispatch but the
//   yaw direction flag (0x104) is -1 and the yaw accumulator (0x27c) ramps
//   downward toward the (negative) target.

extern "C" float MiningGame_steerXR(void *mg, float d);                // 0x1abaf8 veneer

extern const float g_PE_r_loadK;    // 0xb0924
extern const float g_PE_r_loadB;    // 0xb0928
extern const float g_PE_r_rateK;    // 0xb092c
extern const float g_PE_r_turK1;    // 0xb0910
extern const float g_PE_r_turK2;    // 0xb0914
extern const float g_PE_r_turK3;    // 0xb0918
extern const float g_PE_r_manK1;    // 0xb091c
extern const float g_PE_r_manK2;    // 0xb0920

extern "C" float PlayerEgo_right(PlayerEgo *self, int frameTime, float delta)
{
    if (P(self, 0x1e4) != 0)
        return MiningGame_steerXR(P(self, 0x1e4), delta);

    if (C(self, 0x1a0) != 0) {
        float pitch = (float)I(self, 0x1f8);
        float ft = (float)frameTime;
        float ang = ((ft * delta) / (g_PE_r_turK1 / pitch)) * g_PE_r_turK2 * g_PE_r_turK3;
        AEGeometry_rotateY(P(self, 0x178), 0, ang, 0);
        AEGeometry_rotateY(P(self, 0xdc), 0, ang, 0);
        AEGeometry_rotateY(P(self, 0x28), 0, ang, 0);
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
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
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

// ---- left_a094c.cpp ----
// PlayerEgo::left(int frameTime, float delta)
//   Yaw-left input handler. Routes the input depending on mode:
//     * mining game active (0x1e4): forward the (negated) delta to it,
//     * turret mode (0x1a0): rotate the three turret geometries,
//     * maneuver lock (0x194): feed the scripted-maneuver roll instead,
//     * otherwise: set the yaw direction (0x104=+1), compute the load-scaled
//       yaw rate and ramp the yaw accumulator (0x27c) toward the target.
//   The yaw-rate ramp (which samples the active-camera ease state) is computed
//   by PE_yawRampDelta to keep the FP-flag soup out of line.

extern "C" float MiningGame_steerX(void *mg, float d);                 // 0x1abb08 veneer
extern "C" void  AEGeometry_rotateY(void *geo, int a, float ang, int b);// turret yaw veneer
// Ramp helper: given base rate and inputs, returns the accumulator increment.

extern const float g_PE_l_loadK;    // 0xb0b68
extern const float g_PE_l_loadB;    // 0xb0b6c
extern const float g_PE_l_rateK;    // 0xb0b70
extern const float g_PE_l_turK1;    // 0xb0b54
extern const float g_PE_l_turK2;    // 0xb0b58
extern const float g_PE_l_turK3;    // 0xb0b5c
extern const float g_PE_l_manK1;    // 0xb0b60
extern const float g_PE_l_manK2;    // 0xb0b64

extern "C" float PlayerEgo_left(PlayerEgo *self, int frameTime, float delta)
{
    if (P(self, 0x1e4) != 0)
        return MiningGame_steerX(P(self, 0x1e4), -delta);

    if (C(self, 0x1a0) != 0) {
        // turret yaw: scale by inverse turret-pitch and apply to 3 nodes.
        float pitch = (float)I(self, 0x1f8);
        float ft = (float)frameTime;
        float ang = ((ft * delta) / (g_PE_l_turK1 / pitch)) * g_PE_l_turK2 * g_PE_l_turK3;
        AEGeometry_rotateY(P(self, 0x178), 0, ang, 0);
        AEGeometry_rotateY(P(self, 0xdc), 0, ang, 0);
        AEGeometry_rotateY(P(self, 0x28), 0, ang, 0);
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
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
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

// ---- _PlayerEgo_9a0a4.cpp ----
// PlayerEgo::~PlayerEgo() -- real C++ destructor so the demangled symbol contains "~PlayerEgo".
//
// Deletes every owned sub-object (the wrapped Player, the various AEGeometry
// nodes for hull / shield / cloak / turret / docking rigs, the Route, tractor
// and repair beams, the mining game, both explosions, the ease matrix) and
// nulls each handle, mirroring the target teardown order.

extern "C" void  Array_releaseRepairBeams(void *);     // ArrayReleaseClasses<RepairBeam*>
extern "C" void *ArrayRB_dtor(void *);                 // Array<RepairBeam*>::~Array

static inline void *&PP(void *self, uint32_t off) { return *(void **)((char *)self + off); }

__attribute__((minsize)) PlayerEgo::~PlayerEgo() noexcept(false)
{
    void *self = this;
    if (PP(self, 0x0))   operator_delete_(Player_dtor(PP(self, 0x0)));
    PP(self, 0x0) = 0;
    if (PP(self, 0x4))   operator_delete_(AEGeometry_dtor(PP(self, 0x4)));
    PP(self, 0x4) = 0;
    if (PP(self, 0x8))   operator_delete_(AEGeometry_dtor(PP(self, 0x8)));
    PP(self, 0x8) = 0;
    if (PP(self, 0xdc))  operator_delete_(AEGeometry_dtor(PP(self, 0xdc)));
    PP(self, 0xdc) = 0;
    if (PP(self, 0x28))  operator_delete_(AEGeometry_dtor(PP(self, 0x28)));
    PP(self, 0x28) = 0;
    if (PP(self, 0xfc))  operator_delete_(Route_dtor(PP(self, 0xfc)));
    PP(self, 0xfc) = 0;
    if (PP(self, 0x178)) operator_delete_(AEGeometry_dtor(PP(self, 0x178)));
    PP(self, 0x178) = 0;
    if (PP(self, 0x17c)) operator_delete_(AEGeometry_dtor(PP(self, 0x17c)));
    PP(self, 0x17c) = 0;
    if (PP(self, 0x2c))  operator_delete_(AEGeometry_dtor(PP(self, 0x2c)));
    PP(self, 0x2c) = 0;
    if (PP(self, 0x30))  operator_delete_(AEGeometry_dtor(PP(self, 0x30)));
    PP(self, 0x30) = 0;
    if (PP(self, 0x34))  operator_delete_(AEGeometry_dtor(PP(self, 0x34)));
    PP(self, 0x34) = 0;
    if (PP(self, 0x19c)) operator_delete_(AEGeometry_dtor(PP(self, 0x19c)));
    PP(self, 0x19c) = 0;
    if (PP(self, 0x1b4)) operator_delete_(TractorBeam_dtor(PP(self, 0x1b4)));
    PP(self, 0x1b4) = 0;
    if (PP(self, 0x1e4)) operator_delete_(MiningGame_dtor(PP(self, 0x1e4)));
    PP(self, 0x1e4) = 0;
    if (PP(self, 0x8c))  operator_delete_(Explosion_dtor(PP(self, 0x8c)));
    PP(self, 0x8c) = 0;
    if (PP(self, 0x90))  operator_delete_(Explosion_dtor(PP(self, 0x90)));
    PP(self, 0x90) = 0;
    if (PP(self, 0x358)) operator_delete_(EaseInOutMatrix_dtor(PP(self, 0x358)));
    PP(self, 0x358) = 0;
    if (PP(self, 0x1b8)) {
        Array_releaseRepairBeams(PP(self, 0x1b8));
        if (PP(self, 0x1b8))
            operator_delete_(ArrayRB_dtor(PP(self, 0x1b8)));
    }
    PP(self, 0x1b8) = 0;
}

// ---- throttleChanged_a20f4.cpp ----
void PlayerEgo_throttleChanged(PlayerEgo* self) {
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

// ---- PlayerEgo_998cc.cpp ----
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

extern "C" void Matrix_ctor(void *m);                       // AEMath::Matrix::Matrix()
extern "C" void PlayerEgo_initFields(void *self, Player *player); // field init + boost + MovingStars

PlayerEgo::PlayerEgo(Player *player)
{
    void *self = this;
    // embedded orientation matrices.
    Matrix_ctor((char *)self + 0x2ac);     // roll matrix
    Matrix_ctor((char *)self + 0x4c8);     // turret/HUD matrix

    // record the wrapped player (offset 0).
    PP(self, 0x0) = (void *)player;

    // remaining field initialisation + boost stats + MovingStars background.
    PlayerEgo_initFields(self, player);
}

// ---- PlayEngineSound_9a550.cpp ----
extern void* g_engine_status;
extern void* g_engine_fmod;
void PlayerEgo_PlayEngineSound(PlayerEgo* self) {
  if (Ship_getFirstEquipmentOfSort(Status_getShip(g_engine_status), 0x26) != 0
      && Status_inAlienOrbit(g_engine_status) == 0) {
    void* obj = g_engine_status;
    int idx = Station_getIndex(Status_getStation(obj));
    int cm = Status_getCurrentCampaignMission(g_engine_status);
    float g = Status_getGammaRayDamagePerSecond(obj, idx, cm);
    if (0.0f < g && I(self, 0xb4) != -1) {
      FModSound_play(*(void**)g_engine_fmod, P(self, 0xb4), 0, g, 0);
    }
  }
  return PlayerEgo_PlayEngineSound_ext(P(self, 0), I(self, 0x1c), 0);
}

// ---- moveToPosition_9c6d4.cpp ----
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

extern const float g_PE_mtp_strafeEps;   // 0xaca60
extern const float g_PE_mtp_strafeReset; // 0xaca64
extern const float g_PE_mtp_strafeK;     // 0xaca68

extern "C" void PlayerEgo_moveToPosition(PlayerEgo *self, float tx, float ty, float tz,
                                         int steer, float speed)
{
    float target[3] = { tx, ty, tz };
    PE_mtp_steer(self, target, steer, speed);

    // apply the resolved heading (stored at 0x164) as the new facing.
    float up[3] = {0.0f, 1.0f, 0.0f};
    (void)up;
    AEGeometry_setDirection(P(self, 0x8), (char *)self + 0x164);

    float dt = (float)I(self, 0x134);
    AEGeometry_moveForward(P(self, 0x8), F(self, 0xbc) * dt * F(self, 0xb8));
    PlayerEgo_roll(self, I(self, 0x134));

    // strafe slide -> follow camera, unless below the dead-zone.
    float slide = F(self, 0x37c);
    float mag = slide > 0.0f ? slide : -slide;
    if (mag <= g_PE_mtp_strafeEps) {
        F(self, 0x380) = g_PE_mtp_strafeReset;
    } else {
        unsigned char m[0x30];
        Mat_assign(m, AEGeometry_getMatrix(P(self, 0x8)));
        // rotate the slide vector into world space and push it to the camera.
        float v = slide * dt;
        TargetFollowCamera_translateNoUpdate(P(self, 0x88), v, 0.0f, 0.0f);
        F(self, 0x37c) = F(self, 0x37c) * g_PE_mtp_strafeK;
        AEGeometry_setMatrix(P(self, 0x8), m);
    }

    Mat_assign((char *)P(self, 0x0) + 0x4, AEGeometry_getMatrix(P(self, 0x8)));
    AEGeometry_getPosition(P(self, 0x8), (char *)self + 0x148);
}

// ---- resetGunDelay_a15f4.cpp ----
void PlayerEgo_resetGunDelay(PlayerEgo* self) { return PlayerEgo_resetGunDelay_ext(P(self, 0), 0); }

// ---- getShieldDamageRate_9b7aa.cpp ----
int PlayerEgo_getShieldDamageRate(PlayerEgo* self) { return PlayerEgo_getShieldDamageRate_ext(P(self, 0)); }

// ---- handleAutoTurret_9ca78.cpp ----
// PlayerEgo::handleAutoTurret(int dt)
//   Drives the automatic gun turret. Every 3 seconds (timer 0x184) it rescans
//   the level enemies and locks the nearest living, active, hostile target
//   (0x18c). Each frame, if a target is held, it aims the turret geometries at
//   the predicted intercept point and -- when on target and the firing window
//   permits -- fires a turret shot and pulses the muzzle transform. Losing the
//   target or exceeding the no-target window stops shooting. The aim/intercept
//   vector-and-matrix math is delegated to PE_hat_aimAndFire.

extern "C" void  KIPlayer_getPosition(void *e, void *out);   // virtual +0x28
// Aims the turret at the held target and fires when appropriate; returns the
// new "no-target" timer contribution (0 while actively tracking).

extern "C" void PlayerEgo_handleAutoTurret(PlayerEgo *self, int dt)
{
    int t = I(self, 0x184) + dt;
    I(self, 0x184) = t;
    if (t >= 0xbb9) {                       // 3000ms: rescan for the best target
        I(self, 0x184) = 0;
        P(self, 0x18c) = 0;
        unsigned int *enemies = (unsigned int *)Level_getEnemies(P(self, 0xc));
        if (enemies != 0) {
            int best = 60000;
            for (unsigned i = 0; i < enemies[0]; i++) {
                void *e = *(void **)(enemies[1] + i * 4);
                if (KIPlayer_isDead(e) != 0) continue;
                if (KIPlayer_isDying(e) != 0) continue;
                if (Player_isActive(*(void **)((char *)e + 4)) == 0) continue;
                if (KIPlayer_isEnemy(e) == 0) continue;
                if (C(e, 0x74) != 0) continue;

                float epos[3];
                KIPlayer_getPosition(e, epos);
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

    Player_stopShooting(P(self, 0x0));
}

// ---- dockToAsteroid_9f4a0.cpp ----
// PlayerEgo::dockToAsteroid(KIPlayer*, Radar*)
//   Toggles the asteroid mining dock. If not docked and given an asteroid, it
//   latches on (records the asteroid at 0x1bc, computes an approach distance
//   from its scaling at 0x1d8). If already docked, it detaches: re-enables the
//   asteroid spin, clears the dock vector (0x1c8), re-activates the follow
//   camera, tears down the mining game (0x1e4) and shows the exhaust again.

extern "C" void  Vec_assign(void *dst, const void *src);                 // Vector::operator=

extern const float g_PE_astApproach;    // 0xaf568 scaling -> approach distance

extern "C" void PlayerEgo_dockToAsteroid(PlayerEgo *self, void *radar)
{
    if (C(self, 0x1c0) != 0) {
        // Already docked -> undock.
        PlayerAsteroid_setRotationEnabled(P(self, 0x1bc), true);
        C(self, 0x145) = 0;
        C(self, 0x1c0) = 0;
        P(self, 0x1bc) = 0;

        int zero[3] = {0, 0, 0};
        Vec_assign((char *)self + 0x1c8, zero);

        TargetFollowCamera_setActive(P(self, 0x88), true);
        Player_resetGunDelay(P(self, 0x0), 0);
        if (P(self, 0x1e4) != 0)
            operator_delete_(MiningGame_dtor(P(self, 0x1e4)));
        P(self, 0x1e4) = 0;
        Radar_unlockAsteroid(radar);
        I(self, 0x1c4) = 0;
        PlayerEgo_setExhaustVisible(self, true);
    } else if (radar != 0) {
        // Not docked -> latch onto the asteroid.
        C(self, 0x1c0) = 1;
        I(self, 0x1dc) = 0;
        P(self, 0x1bc) = radar;
        float dist = PlayerAsteroid_getScaling(radar) * g_PE_astApproach;
        C(self, 0x145) = 1;
        I(self, 0x1c4) = 0;
        I(self, 0x1d8) = (int)dist;
    }
}

// ---- killLiberator_a1a18.cpp ----
void PlayerEgo_killLiberator(PlayerEgo* self) {
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
      Vector_assign((void*)e[3], sv);
      void* arr2 = P(P(p, 4), 4);
      int* e2 = (int*)(*(int*)((char*)arr2 + 4) + i * 4);
      *((char*)e2 + 0x4c) = 0;
      count = U(arr2, 0);
    }
  }
}

// ---- roll_9bbf8.cpp ----
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

extern "C" void PlayerEgo_roll(PlayerEgo *self, int amount)
{
    if (C(self, 0x2f4) == 0)
        return;

    void *m = AEGeometry_getMatrix(P(self, 0x8));
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

// ---- setTargetFollowCamera_a21f2.cpp ----
void PlayerEgo_setTargetFollowCamera(PlayerEgo* self, void* cam) {
  void* m = P(self, 0x154);
  P(self, 0x88) = cam;
  return PlayerEgo_setTargetFollowCamera_ext(cam, m);
}

// ---- calcCollision_9f0cc.cpp ----
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

extern const float g_PE_cc_alarmDist;   // 0xaf470

extern "C" void PlayerEgo_calcCollision(PlayerEgo *self, void *candidates)
{
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
        if (i == 0 && Status_inAlienOrbit() == 0) {
            if (Vec_length((char *)self + 0x148) < g_PE_cc_alarmDist && C(obj, 0x71) != 0)
                C(self, 0x234) = 1;
        }

        // broad-phase overlap test (virtual +0x40).
        float pos[3];
        AEGeometry_getPosition(P(self, 0x8), pos);
        typedef int (*overlap_fn)(void *, void *);
        overlap_fn overlaps = *(overlap_fn *)(*(char **)obj + 0x40);
        if (!overlaps(obj, pos))
            continue;

        if (KIPlayer_getType(obj) == 0x4262 && KIPlayer_isVisible(obj) != 0) {
            if (PlayerWormHole_isShrinking(obj) == 0 && P(self, 0x1e4) == 0)
                PE_cc_wormhole(self, obj);
        } else if (C(obj, 0x3c) == 0) {
            if (KIPlayer_isVisible(obj) != 0) {
                // skip the object we are actively docking with.
                bool docking = (C(self, 0x1c0) != 0 || C(self, 0x356) != 0)
                               && P(self, 0x1bc) == obj;
                if (!docking)
                    PE_cc_obstacle(self, obj, i);
                hitCamera_(self);
            }
        } else {
            if (KIPlayer_isDying(obj) == 0 && KIPlayer_isDead(obj) == 0) {
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

// ---- dockToPlanet_a1684.cpp ----
void PlayerEgo_dockToPlanet(PlayerEgo* self) {
  TargetFollowCamera_setLookAtCam(I(self, 0x88));
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
  FModSound_play(*(void**)snd, (void*)5, 0, f, 0);
}

// ---- up_a0dfc.cpp ----
// PlayerEgo::up(int frameTime, float delta)
//   Pitch-up input handler -- the mirror of down(): same axis and dispatch but
//   the free-look/turret accumulators (0x1a8 / 0x1a4) and the pitch accumulator
//   (0x278) ramp upward, with the pitch direction flag (0x100) set to +1.

extern "C" float MiningGame_steerY(void *mg, float d);                 // 0x1abb18 veneer
extern "C" float MiningGame_steerYAlt(void);                          // 0x1abb38 veneer
extern "C" void  AEGeometry_rotateX(void *geo, float ang, int a, float b); // 0x72160
extern "C" float AEGeometry_rotateXr(void *geo, float ang, int a, int b);  // 0x1abb28 veneer

__attribute__((visibility("hidden"))) extern char **g_PE_u_miningGate; // 0xb0e1c -> flags (+0x10)
extern const float g_PE_u_eps;      // 0xb1030 free-look limit
extern const float g_PE_u_eps2;     // 0xb103c
extern const float g_PE_u_lookK1;   // 0xb1034
extern const float g_PE_u_lookK2;   // 0xb1040
extern const float g_PE_u_manK;     // 0xb1044
extern const float g_PE_u_loadK;    // 0xb1048
extern const float g_PE_u_loadB;    // 0xb104c
extern const float g_PE_u_rateK;    // 0xb1050

extern "C" float PlayerEgo_up(PlayerEgo *self, int frameTime, float delta)
{
    if (P(self, 0x1e4) != 0) {
        if (C(*g_PE_u_miningGate, 0x10) == 0)
            return MiningGame_steerY(P(self, 0x1e4), -delta);
        return MiningGame_steerYAlt();
    }

    if (C(self, 0x1a0) != 0) {
        float ft = (float)frameTime;
        if (F(self, 0x1a8) > g_PE_u_eps) {
            float ang = F(self, 0x1a8) - ft * delta;
            F(self, 0x1a8) = ang;
            AEGeometry_rotateX(P(self, 0x28), ang * g_PE_u_lookK1 * g_PE_u_lookK2, 0, 0);
        }
        float p = F(self, 0x1a4);
        if (p > g_PE_u_eps2) {
            float half = ft * delta * 0.5f;
            float ang = half * g_PE_u_lookK1 * g_PE_u_lookK2;
            F(self, 0x1a4) = p - half;
            return AEGeometry_rotateXr(P(self, 0x19c), ang, 0, 0);
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
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
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

// ---- shoot_9b198.cpp ----
// PlayerEgo::shoot(int weapon, int type)
//   Fires the player's weapons. In turret/free-look mode (flag 0x1a0) the shot
//   is aimed using the combined turret * ship matrix and dispatched as a type-2
//   (turret) shot. Otherwise, when alive: a type-1 (secondary/missile) shot
//   tracks the locked secondary index at 0x10c (cleared on a miss), and any
//   other type is a plain primary shot.

extern "C" void  Mat_mul(void *out, const void *a, const void *b);   // operator*
extern "C" int   Player_shootTurret(void *player, int kind, int weapon, int hi,
                                    int flag, const void *matrix);

extern "C" void PlayerEgo_shoot(PlayerEgo *self, int weapon, int type)
{
    if (C(self, 0x1a0) != 0) {
        void *m1 = AEGeometry_getMatrix(P(self, 0x8));
        void *m2 = AEGeometry_getMatrix(P(self, 0x28));
        unsigned char combined[0x30];                 // Matrix is a 48-byte value
        Mat_mul(combined, m1, m2);
        Player_shootTurret(P(self, 0x0), 2, weapon, weapon >> 31, 0, combined);
        return;
    }

    if (PlayerEgo_isDead(self) != 0)
        return;

    void *player = P(self, 0x0);
    if (type == 1) {
        if (Player_shootSecondary(player, 1, I(self, 0x10c), weapon >> 31, 0) == 0)
            I(self, 0x10c) = -1;
    } else {
        Player_shootPrimary(player, type, weapon, weapon >> 31, 0);
    }
}

// ---- stopMining_a17b4.cpp ----
// PlayerEgo::stopMining()
//   The target bytes for this stub decode as a tiny thunk into the mining-game
//   teardown helper (the Ghidra listing is bad-instruction noise because the
//   region is a hand-written veneer). Faithfully forward to that helper.

extern "C" void PlayerEgo_stopMining(PlayerEgo *self)
{
    PlayerEgo_stopMining_impl(self);
}

// ---- setTurretMode_9ab90.cpp ----
// PlayerEgo::setTurretMode(bool enable)
//   Enters or leaves the gun-turret view. Refused unless the ship mounts a
//   turret (0x170) and is neither mining (0x1e4) nor mid-cutscene (0x180); a
//   pending maneuver (0x194) instead just restores the default camera. When
//   enabling it lazily builds the turret camera rig (camera + AEGeometry chain,
//   like the docking camera), aims it, and makes it current; when disabling it
//   stops shooting and resets the camera. Either way it toggles the turret
//   crosshair transform (0x30) and starts/stops the turret hum sound.


__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasA;   // 0xaadbc
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasB;   // 0xaadc0
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasC;   // 0xaadc4
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasD;   // 0xaadc8
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasE;   // 0xaadcc
__attribute__((visibility("hidden"))) extern void **g_PE_tm_transform; // 0xaadd0
__attribute__((visibility("hidden"))) extern int  *g_PE_tm_hum;        // 0xaadd4
extern const float g_PE_tm_fovNormal;  // 0xaad9c
extern const float g_PE_tm_fovAlien;   // 0xaada0

extern "C" void PlayerEgo_setTurretMode(PlayerEgo *self, int enable)
{
    if (C(self, 0x170) == 0 || P(self, 0x1e4) != 0 || C(self, 0x180) != 0) {
        // turret view unavailable -> restore default camera if a maneuver runs.
        if (P(self, 0x194) != 0) {
            PaintCanvas_CameraSetCurrent(*g_PE_tm_canvasA, U(P(self, 0x88), 0));
            LevelScript_resetCamera(P(self, 0xc));
        }
        return;
    }

    C(self, 0x1a0) = (unsigned char)enable;
    if (enable == 0) {
        PlayerEgo_stopShooting(self);
        PaintCanvas_CameraSetCurrent(*g_PE_tm_canvasE, U(P(self, 0x88), 0));
        LevelScript_resetCamera(P(self, 0xc));
    } else {
        if (P(self, 0x194) != 0)
            return;
        if (P(self, 0x178) == 0) {
            void **holder = g_PE_tm_canvasB;
            PaintCanvas_CameraCreate(*holder, &U(self, 0x174));
            unsigned int cam = (unsigned int)(unsigned long)*holder;
            float fov = (Status_inAlienOrbit() != 0) ? g_PE_tm_fovAlien : g_PE_tm_fovNormal;
            PaintCanvas_CameraSetPerspective(cam, fov, 0.0f, g_PE_tm_fovAlien);

            void *node = AEGeometry_new(*holder);
            P(self, 0x178) = node;
            AEGeometry_setRotationOrder(node, 2);
            AEGeometry_translate(node, (char *)self + 0x224);

            void *mid = AEGeometry_new(*holder);
            P(self, 0x19c) = mid;
            AEGeometry_translate(mid, (char *)mid + 0xc);
            AEGeometry_addChild(P(self, 0x178), *(void **)((char *)P(self, 0x19c) + 0xc));

            void *leaf = AEGeometry_new(*holder);
            P(self, 0x17c) = leaf;
            AEGeometry_addChild(P(self, 0x178), *(void **)((char *)P(self, 0x178) + 0xc));

            if (Ship_getFirstEquipmentOfSort(Status_getShip(), 0x23) != 0)
                AEGeometry_rotate(P(self, 0x178), (char *)self + 0x224);
        }
        AEGeometry_setPosition(P(self, 0x17c), (char *)self + 0x148);
        void *leaf = P(self, 0x17c);
        AEGeometry_setMatrix(leaf, AEGeometry_getMatrix(P(self, 0x8)));
        PaintCanvas_CameraSetCurrent(*g_PE_tm_canvasD, U(self, 0x174));
        Player_stopShooting(P(self, 0x0));
    }

    if (P(self, 0x30) != 0) {
        void *tf = PaintCanvas_TransformGetTransform(U(*g_PE_tm_transform, 0));
        int v = Transform_SetVisible(tf, enable != 0);
        if (enable == 0)
            FModSound_stop(*g_PE_tm_hum);
        else
            FModSound_play(*g_PE_tm_hum, 0x8cf, 0, v);
    }
}

// ---- rotate_a12fc.cpp ----
void PlayerEgo_rotate(PlayerEgo* self, float rx, float ry, float rz) {
  char local[60];
  F(self, 0x2e8) = F(self, 0x2e8) + rx;
  F(self, 0x2ec) = F(self, 0x2ec) + ry;
  F(self, 0x2f0) = F(self, 0x2f0) + rz;
  int ido = I(P(self, 4), 0xc);
  void* t = *(void**)g_rotate_transform;
  void* m = TransformGetLocal(t, ido);
  MatrixSetRotation(local, m, F(self, 0x2e8), F(self, 0x2ec), F(self, 0x2f0));
}

// ---- strafe_a11b4.cpp ----
// PlayerEgo::strafe(int dir, bool positive)
//   Sets the lateral strafe acceleration (0x37c) and clamps the strafe target
//   speed (0x380). When the ship is cargo-loaded (flag 0x235) the base strafe
//   rate (0x154) is reduced proportionally to the current/max load ratio.
//   A maneuver lock at 0x194 suppresses strafing entirely.


extern const float g_PE_strafeLoadK;   // 0xb127c
extern const float g_PE_strafeLoadB;   // 0xb1280
extern const float g_PE_strafeAccelK;  // 0xb1284

static inline float fmin_(float a, float b) { return a < b ? a : b; }

extern "C" void PlayerEgo_strafe(PlayerEgo *self, int /*dir*/, bool positive)
{
    if (I(self, 0x194) != 0)
        return;

    float base;
    if (C(self, 0x235) != 0) {
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
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

// ---- dockToDockingPoint_9f9f4.cpp ----
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

extern "C" void PlayerEgo_dockToDockingPoint(PlayerEgo *self, void *radar)
{
    if (PlayerEgo_isDead(self) != 0)
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
        PlayerEgo_getPosition(self, pos);
        void *sp = (void *)(unsigned long)I(self, 0x36c);
        void *nav = KIPlayer_getNearestNavigationPoint(radar, pos, sp);
        if (nav == 0) {
            if (C(radar, 0x70) != 0)
                C(radar, 0x8c) = 1;
            undock = true;
        } else {
            if (C(radar, 0x70) != 0)
                C(radar, 0x8c) = 1;

            PlayerEgo_setTurretMode(self, 0);
            C(self, 0x1a1) = 0;
            PaintCanvas_CameraSetCurrent(*g_PE_dtdp_canvas, U(P(self, 0x88), 0));
            LevelScript_resetCamera(P(self, 0x10));
            PlayEngineSound_(self);
            I(self, 0x1c4) = 3;
            TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
            TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);

            if (P(self, 0x358) != 0)
                operator_delete_(EaseInOutMatrix_dtor(P(self, 0x358)));
            P(self, 0x358) = 0;

            PlayerEgo_getPosition(self, pos);
            void *nav2 = KIPlayer_getNearestNavigationPoint(radar, pos, sp);
            void *from = AEGeometry_getMatrix(P(self, 0x8));
            P(self, 0x358) = PE_dtdp_makeEase(from, nav2);
            PlayerEgo_setExhaustVisible(self, true);
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
        TargetFollowCamera_setActive(P(self, 0x88), true);
        TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
        TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);
        Player_resetGunDelay(P(self, 0x0), 0);
        I(self, 0x1c4) = 0;
        PlayerEgo_setExhaustVisible(self, true);
        if (I(self, 0x36c) != 0) {
            SpacePoint_giveFree((void *)(unsigned long)I(self, 0x36c));
            I(self, 0x36c) = 0;
        }
    }

    if (P(self, 0x1e8) != 0) {
        operator_delete_(HackingGame_dtor(P(self, 0x1e8)));
        P(self, 0x1e8) = 0;
        Hud_setHackingGameActive(P(self, 0x220), false);
    }
}

// ---- draw_a1c98.cpp ----
// PlayerEgo::draw(bool allowHud)
//   Draws the 2D overlay layer for the player ship (targeting reticle / lock
//   marker plus the boost throttle). A cascade of guard conditions short-circuits
//   to one of several render-tail veneers when the player is in a maneuver, the
//   mining game, dead, docking, in a cutscene, etc. In the normal path it builds
//   a unit matrix (optionally seeded from the ship matrix), then draws the
//   appropriate reticle image depending on turret mode / plasma lock / blink
//   state, and finally the throttle gauge.

extern "C" void  PaintCanvas_DrawImage2DEx(void *canvas, int img, int x, int y,
                                           int anchor, int flags);
extern "C" void  PlayerEgo_draw_tailA(void);   // 0x1abc48 veneer
extern "C" void  PlayerEgo_draw_tailB(void);   // 0x1abc58 veneer
extern "C" void  PlayerEgo_draw_tailC(void);   // 0x1abc68 veneer

__attribute__((visibility("hidden"))) extern void **g_PE_dr_canvas;    // 0xb1dd8 PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_PE_dr_status;    // 0xb1de8 Status singleton
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posLock;   // 0xb1e0c reticle pos (plasma)
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posNoLock; // 0xb1ea2
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posBlink;  // 0xb1e24
__attribute__((visibility("hidden"))) extern float *g_PE_dr_posNormal; // 0xb1e6e

extern "C" void PlayerEgo_draw(PlayerEgo *self, int allowHud)
{
    if (P(self, 0x194) != 0)               // mid scripted maneuver: nothing to draw
        return;

    if (P(self, 0x1e8) != 0 && C(self, 0x1a0) == 0)
        return;

    if (P(self, 0x1e4) != 0) {             // mining game owns the screen
        PlayerEgo_draw_tailA();
        return;
    }

    bool full;
    if (C(self, 0x1f4) != 0 || PlayerEgo_isDead(self) != 0 || C(self, 0x24) != 0
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
        Mat_assign(m, AEGeometry_getMatrix(P(self, 0x8)));

    void *canvas = *g_PE_dr_canvas;
    PaintCanvas_SetColor(0xffffffff);

    if (C(self, 0x1a0) != 0
        && Ship_getFirstEquipmentOfSort(Status_getShip(), 0x23) != 0) {
        // turret crosshair: plasma lock changes the marker + position.
        if (Radar_isPlasmaInRange(P(self, 0x14)) != 0) {
            float *p = g_PE_dr_posLock;
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x248), (int)p[0], (int)p[1], 0x11, 0x44);
        } else {
            float *p = g_PE_dr_posNoLock;
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x244), (int)p[0], (int)p[1], 0x11, 0x44);
        }
    } else {
        // standard lock-on reticle: blink while acquiring.
        if (C(P(self, 0xc), 0x30) != 0) {
            float *p = g_PE_dr_posBlink;
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x240), (int)p[0], (int)p[1], 0x11, 0x44);
            int t = I(self, 0x24c) + I(self, 0x134);
            I(self, 0x24c) = t;
            if (t >= 0xc9)
                C(P(self, 0xc), 0x30) = 0;
        } else {
            float *p = g_PE_dr_posNormal;
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x23c), (int)p[0], (int)p[1], 0x11, 0x44);
            I(self, 0x24c) = 0;
        }
    }

    PlayerEgo_drawThrottle(self);
}

// ---- update_9ce38.cpp ----
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
extern "C" void PlayerEgo_moveToPosition(PlayerEgo *self, float x, float y, float z,
                                         int steer, float speed);

// route / nav helpers.

// dense numeric phases (own the corrupted FP blocks).
extern "C" void PE_upd_subsystems(PlayerEgo *self, int dt, void *radar, void *hud,
                                  void *radio, void *script);
extern "C" void PE_upd_post(PlayerEgo *self, int dt, void *radar, void *hud,
                            void *radio, int arg5);

extern const float g_PE_upd_handlingBias;   // 0xae484

extern "C" void PlayerEgo_update(PlayerEgo *self, int dt, void *radar, void *hud,
                                 void *radio, void *script, int arg5, bool arg6, int arg7)
{
    (void)arg6; (void)arg7; (void)script;

    if (P(self, 0x220) == 0)
        return;
    if (C(self, 0x24) != 0)
        return;   // frozen

    // cache world position for this frame.
    float pos[3];
    PlayerEgo_getPosition(self, pos);
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
        if (P(self, 0x1bc) == 0 || PlayerEgo_isDead(self) != 0) {
            PlayerEgo_dockToAsteroid(self, 0);
            PE_upd_post(self, dt, radar, hud, radio, arg5);
            return;
        }
        PlayerEgo_approachAsteroid(self, 0 /*hud2*/, radar);
    }

    // ---- station docking ---------------------------------------------------
    if (C(self, 0x356) != 0 && Player_getHitpoints(P(self, 0x0)) > 0) {
        C(self, 0x145) = 1;
        if (P(self, 0x1bc) == 0 || PlayerEgo_isDead(self) != 0) {
            PlayerEgo_dockToDockingPoint(self, 0);
            PE_upd_post(self, dt, radar, hud, radio, arg5);
            return;
        }
        if (PlayerEgo_approachDockingPoint(self, hud, 0, radar) != 0)
            PE_upd_docksFinishDelivery(self, radio);
    }

    // turret view while the free-look/turret camera is active.
    if (C(self, 0x1a0) != 0 || C(self, 0x1a1) != 0)
        PlayerEgo_handleTurretView(self, dt);

    // ---- flight controller -------------------------------------------------
    bool autopilot = (C(self, 0x158) != 0 && I(self, 0x15c) != 0);
    if (!autopilot) {
        if (C(self, 0x145) == 0) {
            if (C(self, 0x1a0) == 0 && C(self, 0x1a1) == 0) {
                PlayerEgo_roll(self, I(self, 0x134));
                if (PlayerEgo_updateManeuver(self) == 0)
                    PlayerEgo_handleShip(self, dt);
            } else {
                PlayerEgo_handleTurretView(self, dt);
            }
        }
    } else if (C(self, 0x145) == 0 && PlayerEgo_updateManeuver(self) == 0) {
        void *wp = self->field_0x15c;
        if (C(self, 0x160) != 0 && wp != 0 && I(wp, 0x134) != 0) {
            wp = Route_getWaypoint(self->field_0x15c);
            P(self, 0x15c) = wp;
        }
        if (wp == 0 || C(self, 0x1ed) != 0) {
            KIPlayer_setAutoPilot(self);
        } else {
            // steer toward the current waypoint.
            float wpPos[3];
            typedef void (*getpos_fn)(void *, void *);
            (*(getpos_fn *)(*(char **)wp + 0x28))(wp, wpPos);
            I(self, 0xec) = *(int *)&wpPos[0];
            I(self, 0xf0) = *(int *)&wpPos[1];
            I(self, 0xf4) = *(int *)&wpPos[2];

            float me[3];
            PlayerEgo_getPosition(self, me);
            float dx = me[0] - wpPos[0], dy = me[1] - wpPos[1], dz = me[2] - wpPos[2];
            if ((int)(dx * dx + dy * dy + dz * dz) < 20000)
                C(self, 0x330) = 1;

            float speed = 4.0f;
            float h = Ship_getHandling(Status_getShip());
            if (h + g_PE_upd_handlingBias < 4.0f)
                speed = Ship_getHandling(Status_getShip()) + g_PE_upd_handlingBias;
            PlayerEgo_moveToPosition(self, F(self, 0xec), F(self, 0xf0), F(self, 0xf4), 1, speed);
            if (C(self, 0x1a0) != 0)
                PlayerEgo_handleTurretView(self, dt);
        }
    }

    // continue an evasive maneuver during a non-final docking glide.
    if (C(self, 0x356) != 0 && U(self, 0x1c4) != 1
        && C(self, 0x1a0) == 0 && (U(self, 0x1c4) | 1) != 3)
        PlayerEgo_updateManeuver(self);

    // auto-turret (when enabled and the radar window allows it).
    if (C(self, 0x180) != 0 && C(self, 0x355) != 0) {
        if (PlayerEgo_isDead(self) == 0) {
            PlayerEgo_handleAutoTurret(self, dt);
        } else {
            C(self, 0x355) = 0;
            Player_stopShooting(P(self, 0x0));
        }
    }

    // collision + camera + explosion post-processing.
    PE_upd_post(self, dt, radar, hud, radio, arg5);
}

// ---- setTurretPosition_9b2bc.cpp ----
void PlayerEgo_setTurretPosition(PlayerEgo* self, float x, float y, float z) {
  char v[12];
  *(float*)(v + 0) = x;
  *(float*)(v + 4) = y;
  *(float*)(v + 8) = z;
  Vector_assign(B(self, 0x224), v);
}

// ---- revive_a14a4.cpp ----
// PlayerEgo::revive()
//   Brings a destroyed player back to life: re-enables its engine particle
//   system, deletes the explosion, re-activates the follow camera and engine
//   sound, restores hitpoints/armor to max, resets position and facing, and
//   clears the explosion timer.


__attribute__((visibility("hidden"))) extern void **g_PE_reviveSound;

extern "C" void PlayerEgo_revive(PlayerEgo *self)
{
    int psm = I(P(self, 0xc), 0x74);
    bool en = C(self, 0x2fc) != 0;
    ParticleSystemManager_enableSystemEmit(psm, en);
    ParticleSystemManager_enableSystemRender(psm, en);

    if (P(self, 0x8c) != 0)
        operator_delete_(Explosion_dtor(P(self, 0x8c)));
    P(self, 0x8c) = 0;

    TargetFollowCamera_setActive(P(self, 0x88), true);
    int v = Player_setActive(P(self, 0x0), true);

    void *snd = *g_PE_reviveSound;
    v = FModSound_play(*(void **)snd, *(void **)snd, 0, v);
    FModSound_play(snd, P(self, 0x1c), 0, v);

    PlayerEgo_setExhaustVisible(self, true);

    void *player = P(self, 0x0);
    Player_setHitpoints(player, Player_getMaxHitpoints(player));
    player = P(self, 0x0);
    Player_setArmorHP(player, Player_getMaxArmorHP(player));

    PlayerEgo_setPosition(self, 0, 0, 0);

    float fwd[3] = {0.0f, 0.0f, 1.0f};
    AEGeometry_setDirection(P(self, 0x8), fwd);
    I(self, 0x2f8) = 0;
}

// ---- drawThrottle_a1f04.cpp ----
// PlayerEgo::drawThrottle()
//   While the boost gauge is active (flag 0x370) this draws the throttle bar:
//   a partial DrawRegion2D fill of the gauge image (0x238) whose height tracks
//   the current thrust (0xbc), plus the thrust percentage rendered as text
//   centred under it. The boost timer (0x374) folds back past its midpoint so
//   the bar pulses symmetrically.

extern "C" void  PaintCanvas_DrawRegion2D(void *canvas, int img, int sx, int sy, int w,
                                          int h, int dh, int a, int b, int c, int dx);
extern "C" void  String_ctor_int(void *s, int value);                  // String::String(int)

__attribute__((visibility("hidden"))) extern void **g_PE_t_canvas;     // PaintCanvas singleton
__attribute__((visibility("hidden"))) extern float *g_PE_t_anchor;     // {x,y} gauge anchor
__attribute__((visibility("hidden"))) extern void **g_PE_t_pctStr;     // "%" String
extern const float g_PE_t_timerDiv;    // 0xb1f30 timer normaliser
extern const float g_PE_t_pctScale;    // 0xb20dc thrust->percent
extern const float g_PE_t_textDiv;     // 0xb20e0 text vertical divisor

extern "C" void PlayerEgo_drawThrottle(PlayerEgo *self)
{
    if (C(self, 0x370) == 0)
        return;

    int t = I(self, 0x374);
    if (t > 500)
        t = 2000 - t;
    float frac = (float)t / g_PE_t_timerDiv;
    if (frac > 1.0f)
        frac = 1.0f;

    void *canvas = *g_PE_t_canvas;
    PaintCanvas_SetColor(0xffffff00 | (unsigned int)(int)(frac * 255.0f) - 0x100);

    int img = I(self, 0x238);
    int w = PaintCanvas_GetImage2DWidth(canvas);
    int h = PaintCanvas_GetImage2DHeight(canvas);

    float thrust = F(self, 0xbc);
    int fillH = (int)(thrust * (float)h);
    float *anchor = g_PE_t_anchor;

    PaintCanvas_DrawRegion2D(canvas, img, 0, h - fillH, w, fillH,
        (int)((anchor[1] + (float)h) - (float)fillH), 0, 0, 0,
        (int)(anchor[0] - (float)(w / 2)));

    // percentage label
    unsigned char pct[12];
    String_ctor_int(pct, (int)(thrust * g_PE_t_pctScale));

    int th = PaintCanvas_GetImage2DHeight(canvas);
    void *pctStr = *g_PE_t_pctStr;
    int tw = PaintCanvas_GetTextWidth(canvas, pctStr);
    PaintCanvas_DrawString(canvas, pctStr, pct,
        (int)((anchor[0] - (float)(tw / 2)) - 1.0f),
        (bool)(int)(anchor[1] + (float)th / g_PE_t_textDiv));
    PaintCanvas_SetColor(0xffffffff);

    String_dtor(pct);
}

// ---- setAutoPilot_9b808.cpp ----
extern void* g_appmgr;
void PlayerEgo_setAutoPilot(PlayerEgo* self, void* kip) {
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
  void* eng = ApplicationManager_GetEngine(*(void**)g_appmgr);
  I(eng, 0x360) = 0;
  I(self, 0xbc) = 0x3f800000;
}

// ---- updateManeuver_9c1f8.cpp ----
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

extern "C" void  PlayerEgo_moveToPosition(PlayerEgo *self, float x, float y, float z,
                                          int steer, float speed);
// dodge-arc step (types 1/2): banks/slides the ship + camera for `dt`-advanced
// timer; returns nothing, updates the hull transform in place.
// strafe-run step (type 3): builds the intercept target offset into `out`.
// strafe-run post-move bank + camera glide.

extern "C" int PlayerEgo_updateManeuver(PlayerEgo *self)
{
    unsigned int type = U(self, 0x334);

    if ((type - 1) < 2) {
        // barrel-roll dodge (left = 1, right = 2)
        I(self, 0x350) = I(self, 0x8c) + I(self, 0x350);   // advance timer by frame dt
        PE_um_dodgeStep(self);
        if (I(self, 0x350) > 0x4af) {
            void *level = P(self, 0x10);
            I(self, 0x334) = 0;
            LevelScript_resetCamera(level);
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
    PlayerEgo_moveToPosition(self, target[0], target[1], target[2], 1, 0.0f);

    PE_um_strafeGlide(self);
    TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
    TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);

    if (I(self, 0x350) > 2999) {
        I(self, 0x334) = 0;
        TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
        TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);
    }
    return 1;
}

// ---- getHullDamageRate_9b7a4.cpp ----
int PlayerEgo_getHullDamageRate(PlayerEgo* self) { return PlayerEgo_getHullDamageRate_ext(P(self, 0)); }

// ---- handleTurretView_9b96c.cpp ----
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

extern "C" void PlayerEgo_handleTurretView(PlayerEgo *self, int dt)
{
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
        AEGeometry_moveForward(P(self, 0x8), F(self, 0xbc) * d * F(self, 0xb8));
    }

    unsigned char look[0x30];
    Mat_mul(look, AEGeometry_getMatrix(P(self, 0x8)), AEGeometry_getMatrix(P(self, 0x178)));
    Mat_mulEq(look, AEGeometry_getMatrix(P(self, 0x19c)));

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
    PaintCanvas_CameraSetLocal(cam, (char *)self + 0x174);

    I(self, 0x100) = 0;
    I(self, 0x104) = 0;
    PlayerEgo_roll(self, I(self, 0x134));

    // HUD transform local = hullLocal * reticleLocal
    unsigned int hull = PaintCanvas_TransformGetLocal(*(unsigned int *)((char *)P(self, 0x8) + 0xc));
    unsigned int ret  = PaintCanvas_TransformGetLocal(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (void *)(unsigned long)hull, (void *)(unsigned long)ret);
    Mat_assign((char *)P(self, 0x0) + 0x4, tmp);
}

// ---- approachAsteroid_9f570.cpp ----
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
__attribute__((visibility("hidden"))) extern int  *g_PE_aa_mineSound;    // 0xaf928
__attribute__((visibility("hidden"))) extern void **g_PE_aa_winHolder1;  // 0xaf984
__attribute__((visibility("hidden"))) extern void **g_PE_aa_winHolder2;  // 0xaf742
extern const float g_PE_aa_settleEps;   // 0xaf9c0

extern "C" void PlayerEgo_approachAsteroid(PlayerEgo *self, int hud2, void *radar)
{
    if (KIPlayer_isDying(self) != 0)
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
            void *mg = MiningGame_new(PlayerAsteroid_getQuality(P(self, 0x1bc)),
                                      I(P(self, 0x1bc), 0x128), (void *)(unsigned long)hud2);
            P(self, 0x1e4) = mg;
            I((void *)(I(self, 0x1bc) + 4), 0x40) = 0;
            int snd = *g_PE_aa_mineSound;
            FModSound_play(snd, 1, 0, 0);
            FModSound_pause((void *)(unsigned long)snd);
            return;
        }

        int running = MiningGame_update(P(self, 0x1e4), I(self, 0x134));
        if (running == 0) {
            if (MiningGame_gameLost(P(self, 0x1e4)) == 0
                && MiningGame_getOreAmount(P(self, 0x1e4)) > 0) {
                PlayerEgo_stopMining(self);
            } else if (MiningGame_gameLost(P(self, 0x1e4)) != 0) {
                C(self, 0x39b) = 1;
                I(*g_PE_aa_winHolder1, 0x124) = 0;
                PlayerEgo_stopMining(self);
                Hud_hudEvent((void *)(unsigned long)hud2, 8, self);
            }
        } else if (KIPlayer_isDying(self) != 0 || KIPlayer_isDead(self) != 0) {
            I(*g_PE_aa_winHolder2, 0x124) = 0;
            PlayerEgo_stopMining(self);
            Hud_hudEvent((void *)(unsigned long)hud2, 8, self);
        }
        return;
    }

    if (I(self, 0x1c4) == 0) {
        // approach phase: steer/align toward the asteroid.
        I(self, 0x1c4) = PE_aa_approachStep(self, hud2, radar);
    }
}

// ---- handleShip_9bdf0.cpp ----
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
extern const float g_PE_hs_throttleBias;   // 0xac140

extern "C" void PlayerEgo_handleShip(PlayerEgo *self, int dt)
{
    void *snd = *g_PE_hs_sound;
    // engine sound: param 0 = RPM (from |yaw|,|pitch| max), param 1 = throttle.
    FModSound_setParamValue(snd, Player_GetEngineEvent(P(self, 0x0)), 0, F(self, 0x270));
    FModSound_setParamValue(snd, Player_GetEngineEvent(P(self, 0x0)), 1,
                            F(self, 0x268) * g_PE_hs_throttleBias + 0.5f);

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
    unsigned int hull = PaintCanvas_TransformGetLocal(*(unsigned int *)((char *)P(self, 0x8) + 0xc));
    unsigned int ret  = PaintCanvas_TransformGetLocal(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
    unsigned char tmp[0x30];
    Mat_mul(tmp, (void *)(unsigned long)hull, (void *)(unsigned long)ret);
    Mat_assign((char *)P(self, 0x0) + 0x4, tmp);
}

// ---- stopBoost_9ae34.cpp ----
void PlayerEgo_stopBoost(PlayerEgo* self) {
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

// ---- setShip_9a274.cpp ----
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
extern const float g_PE_ss_emDiv;     // 0xaa4b8
extern const float g_PE_ss_emBias;    // 0xaa4bc

extern "C" void PlayerEgo_setShip(PlayerEgo *self, int race, int group)
{
    void **globals = g_PE_ss_globals;
    void *grp = Globals_getShipGroup(*globals, race, group, true);
    P(self, 0x4) = grp;

    void **canvasHolder = g_PE_ss_canvas;
    void *canvas = *canvasHolder;
    void *mesh = PaintCanvas_MeshGetPointer(canvas, I(grp, 0x1c));
    P(self, 0x394) = *(void **)(I(mesh, 0x30) + 0x20);

    void *hull = AEGeometry_new(*canvasHolder);
    P(self, 0x8) = hull;
    AEGeometry_addChild(hull, *(void **)((char *)P(self, 0x4) + 0xc));

    // tractor beam
    if (Ship_getFirstEquipmentOfSort(Status_getShip(), 0xd) != 0) {
        void *it = (void *)Ship_getFirstEquipmentOfSort(Status_getShip(), 0xd);
        int idx = Item_getIndex(it);
        int kind = (idx < 0x48) ? idx - 0x44 : 3;
        void *tb = TractorBeam_new(P(self, 0x8), kind);
        P(self, 0x1b4) = tb;
        Globals_addSoundResourceToList(*(int *)(*globals));
        Globals_addSoundResourceToList(*(int *)(*globals));
    }

    // repair beams (sorts 0x25 and 0x29)
    for (unsigned i = 0; i < 2; i++) {
        int sort = (i == 0) ? 0x25 : 0x29;
        void *it = (void *)Ship_getFirstEquipmentOfSort(Status_getShip(), sort);
        if (it != 0) {
            if (P(self, 0x1b8) == 0)
                P(self, 0x1b8) = Array_RepairBeam_new();
            void *rb = RepairBeam_new(Item_getIndex(it), Item_getSort(it));
            int idx = Item_getIndex(it);
            if (idx == 0xde)
                Globals_addSoundResourceToList(*(int *)(*globals));
            else if (Item_getIndex(it) == 0xdf)
                Globals_addSoundResourceToList(*(int *)(*globals));
            ArrayAdd_RepairBeam(rb, P(self, 0x1b8));
        }
    }

    // emergency system effect geometry
    if (Ship_getFirstEquipmentOfSort(Status_getShip(), 0x1b) != 0
        && Ship_hasEmergencySystem(Status_getShip()) != 0) {
        void *geo = AEGeometry_newMesh(0x3826, *canvasHolder, false);
        P(self, 0xac) = geo;
        Ship_getFirstEquipmentOfSort(Status_getShip(), 0x1b);
        I(self, 0x310) = Item_getAttribute((void *)Ship_getFirstEquipmentOfSort(Status_getShip(), 0x29));
        void *tf = PaintCanvas_TransformGetTransform(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
        Vec_assign((char *)self + 0x314, (char *)tf + 0xd4);
        tf = PaintCanvas_TransformGetTransform(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
        F(self, 0x320) = F(tf, 0xe0) / g_PE_ss_emDiv + g_PE_ss_emBias;
    }

    // supernova scaling
    if (Status_inSupernovaSystem() != 0 || Status_inSupernovaOrbit() != 0) {
        void *tf = PaintCanvas_TransformGetTransform(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
        F(self, 0x3c) = F(tf, 0xe0) * 1.75f;
    }

    // turret offset finalisation
    if (P(self, 0x1b0) != 0)
        PlayerEgo_setShip_tail(*canvasHolder, I(P(self, 0x4), 0x1c),
                               (char *)self + 0x388, canvasHolder);
}

// ---- addGun_9b78e.cpp ----
void PlayerEgo_addGun2(PlayerEgo* self, void* arr, int x) {
  Player_addGun2(P(self, 0), arr, x);
  return PlayerEgo_addGun2_ext(self);
}

// ---- render_a1b78.cpp ----
// PlayerEgo::render(bool allowHud)
//   Draws the ship and all attached visual effects. When alive it renders the
//   explosion shell (if any), the hull geometry, shield/cloak overlays, the
//   tractor beam and every active repair beam, then tail-calls the level draw
//   with a flag derived from the docking state. When dead it only renders the
//   fading explosion debris.

extern "C" void PlayerEgo_render_tail(void *level, int flag);   // 0x1abc38 veneer

extern "C" void PlayerEgo_render(PlayerEgo *self, int allowHud)
{
    void *level = P(self, 0xc);

    if (PlayerEgo_isDead(self) == 0) {
        if (C(self, 0x309) == 0)
            return;

        if (P(self, 0x8c) != 0) {
            Explosion_render(P(self, 0x8c));
            if (I(self, 0x2f8) <= 0xbb7)        // explosion < 3000 ticks old
                AEGeometry_render(P(self, 0x8));
        }
        if (P(self, 0x90) != 0)
            Explosion_render(P(self, 0x90));

        AEGeometry_render(P(self, 0x8));

        if (P(self, 0xac) != 0 && I(self, 0x30c) >= 1)
            AEGeometry_render(P(self, 0x8));    // shield overlay reuses hull geo

        if (C(self, 0x38) != 0)
            AEGeometry_render(P(self, 0x34));

        if (C(self, 0x170) != 0) {              // cloak overlay
            if (P(self, 0x30) != 0)
                AEGeometry_setVisible(P(self, 0x30), C(self, 0x1a0) != 0);
            AEGeometry_render(P(self, 0x2c));
        }

        if (P(self, 0x1b4) != 0)
            TractorBeam_render(P(self, 0x1b4));

        unsigned int *beams = (unsigned int *)P(self, 0x1b8);
        if (beams != 0) {
            for (unsigned int i = 0; i < beams[0]; i++) {
                RepairBeam_render(*(void **)(beams[1] + i * 4));
                beams = (unsigned int *)P(self, 0x1b8);
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
            Explosion_render(P(self, 0x8c));
            if (I(self, 0x2f8) < 3000)
                AEGeometry_render(P(self, 0x8));
        }
        if (P(self, 0xac) != 0)
            Explosion_render(P(self, 0x8c));
        PlayerEgo_render_tail(level, 1);
    }
}

// ---- getTurretPosition_9b2ec.cpp ----
void PlayerEgo_getTurretPosition(void* out, void* src) {
  char local[60];
  void* m1 = AEGeometry_getMatrix(P(src, 8));
  void* m2 = AEGeometry_getMatrix(P(src, 0x28));
  Matrix_mul(local, m1, m2);
  MatrixGetPosition(out, local);
}

// ---- toggleCloaking_9a708.cpp ----
// PlayerEgo::toggleCloaking()
//   Engages or disengages the cloaking device. When uncloaked and chargeable
//   (and carrying enough plasma, item 0x7a), it consumes the plasma, sets the
//   cloak flag and fires the two HUD events. When already cloaked and the cloak
//   timer has run out, it switches the hull (and, for ships with secondary
//   meshes, the wing meshes) to the cloak material 0xe, retunes their shader
//   anim values, and -- for the appropriate ship variant -- creates and binds
//   the resource material.

extern "C" void  PaintCanvas_MeshChangeResourceMaterial(void *canvas, unsigned int mesh,
                                                        unsigned short mat);

__attribute__((visibility("hidden"))) extern int  *g_PE_tc_sound;     // 0xaa786 cloak sound
__attribute__((visibility("hidden"))) extern void **g_PE_tc_canvas;   // 0xaa798 PaintCanvas

extern "C" void PlayerEgo_toggleCloaking(PlayerEgo *self)
{
    if (C(self, 0x1ad) == 0) {
        // currently uncloaked -> try to cloak
        if (C(self, 0x1ac) != 0 || I(self, 0x20c) > 0)
            return;
        int need = Item_getAttribute(P(self, 0x1b0));
        void *cargo = Ship_getCargo(Status_getShip(), 0x7a);
        int have = (cargo == 0) ? 0 : Item_getAmount(cargo);
        if (need <= have) {
            Ship_removeCargo(Status_getShip(), 0x7a);
            C(self, 0x1ad) = 1;
            Hud_hudEvent(P(self, 0x220), 0x1e, self);
            Hud_hudEvent(P(self, 0x220), 0x1c, self);
        }
        return;
    }

    // currently cloaked -> uncloak once the cloak hold timer elapses
    if (I(self, 0x214) > I(self, 0x208))
        return;

    FModSound_play(*g_PE_tc_sound, 0x1e, 0, 0);
    void *canvas = *g_PE_tc_canvas;
    C(P(self, 0x0), 0x5e) = 1;
    I(self, 0x208) = 0;
    C(self, 0x1ac) = 1;

    PaintCanvas_MaterialGetMaterial(canvas, I(self, 0x388));     // returned ptr +0x20 = 0xe below
    I(PaintCanvas_MaterialGetMaterial(canvas, I(self, 0x388)), 0x20) = 0xe;
    PaintCanvas_MeshChangeMaterial(canvas, *(unsigned int *)(I(self, 0x4) + 0x1c), self->field_0x388);
    PaintCanvas_MeshChangeShaderAnimValue(canvas,
        PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0x4) + 0x1c)), 0.0f, 0);
    PaintCanvas_MeshChangeShaderAnimValue(canvas,
        PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0x4) + 0x1c)), 0.0f, 0);

    if (C(self, 0x170) != 0) {
        I(PaintCanvas_MaterialGetMaterial(canvas, I(self, 0x38c)), 0x20) = 0xe;
        I(PaintCanvas_MaterialGetMaterial(canvas, I(self, 0x390)), 0x20) = 0xe;
        PaintCanvas_MeshChangeMaterial(canvas, *(unsigned int *)(I(self, 0xdc) + 0x1c), self->field_0x38c);
        PaintCanvas_MeshChangeMaterial(canvas, *(unsigned int *)(I(self, 0x28) + 0x1c), self->field_0x390);

        void *m;
        m = PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0xdc) + 0x1c));
        PaintCanvas_MeshChangeShaderAnimValue(canvas, m, 0.0f, 1);
        m = PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0xdc) + 0x1c));
        PaintCanvas_MeshChangeShaderAnimValue(canvas, m, 0.0f, 2);
        m = PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0x28) + 0x1c));
        PaintCanvas_MeshChangeShaderAnimValue(canvas, m, 0.0f, 1);
        m = PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0x28) + 0x1c));
        PaintCanvas_MeshChangeShaderAnimValue(canvas, m, 0.0f, 2);

        if (C(self, 0x170) != 0) {
            unsigned short mat = 0x4e8e;
            void *it = (void *)Ship_getFirstEquipmentOfSort(Status_getShip(), 8);
            if (it != 0 && Item_getIndex(it) == 0xe0)
                mat = 0x5e17;
            it = (void *)Ship_getFirstEquipmentOfSort(Status_getShip(), 0x23);
            if (it != 0) {
                int idx = Item_getIndex(it);
                mat = 0x716d;
                if (idx == 0xc7) mat = 0x7167;
                if (idx == 0xc6) mat = 0x7161;
            }
            unsigned int out;
            PaintCanvas_MaterialCreate(canvas, mat, &out);
            PaintCanvas_MeshChangeResourceMaterial(canvas, *(unsigned int *)(I(self, 0xdc) + 0x1c), mat);
            PaintCanvas_MeshChangeResourceMaterial(canvas, *(unsigned int *)(I(self, 0x28) + 0x1c), mat);
        }
    }
}
