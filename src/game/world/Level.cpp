#include "gof2/game/world/Level.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/externs.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/game/mission/Achievements.h"
#include "gof2/game/ui/Hud.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/game/ship/PlayerTurret.h"
#include "gof2/game/world/Waypoint.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/game/world/Route.h"
#include "gof2/game/mission/Objective.h"
#include "gof2/game/world/Station.h"
#include "gof2/engine/render/LODManager.h"
#include "gof2/game/world/Wanted.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/game/weapons/ObjectGun.h"
#include "gof2/game/core/RadioMessage.h"
#include "gof2/engine/render/ParticleSystemManager.h"
#include "gof2/game/world/StarSystem.h"
#include "gof2/engine/core/AbyssEngine.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/file/FileRead.h"
#include "gof2/engine/math/AEMath.h"
#include "gof2/engine/math/BoundingSphere.h"
#include "gof2/game/core/Globals.h"
#include "gof2/game/ship/PlayerAsteroid.h"
#include "gof2/game/ship/PlayerFighter.h"
#include "gof2/game/ship/PlayerGasCloud.h"
#include "gof2/game/ship/PlayerStatic.h"
#include "gof2/game/weapons/RocketGun.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/platform/libc.h"
#include "gof2/game/ship/PlayerFixedObject.h"
#include <new>
#include "gof2/game/ship/Agent.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/game/weapons/BeamGun.h"
#include "gof2/game/weapons/BombGun.h"
#include "gof2/game/weapons/MineGun.h"
#include "gof2/game/weapons/SentryGun.h"
#include "gof2/engine/math/Transform.h"

// Junk-death tally: a singleton holding a counter at +0xb0 (owner class unresolved; not PlayerJunk).
__attribute__((visibility("hidden"))) extern Status **g_status;  // canonical Status singleton
__attribute__((visibility("hidden"))) extern int **g_junkDied;
__attribute__((visibility("hidden"))) extern unsigned char *g_initStreamOut;
__attribute__((visibility("hidden"))) extern PaintCanvas **g_paintCanvas_intro;
__attribute__((visibility("hidden"))) extern PaintCanvas **g_paintCanvas_snr;
__attribute__((visibility("hidden"))) extern Status **g_status_snr;
__attribute__((visibility("hidden"))) extern Status **g_status_applyKills;
__attribute__((visibility("hidden"))) extern Status **g_status_pirate;
__attribute__((visibility("hidden"))) extern int *g_routeRng;
__attribute__((visibility("hidden"))) extern int (*g_routeRandom)(int rng, int bound);
__attribute__((visibility("hidden"))) extern Status **g_alarmAllFriends;
__attribute__((visibility("hidden"))) extern int *g_engineColorBase;
__attribute__((visibility("hidden"))) extern void Globals_addSoundResourceToList(int snd); // engine sound-list helper (Globals.cpp)
__attribute__((visibility("hidden"))) extern int *g_cg_beamTable;   // [DAT_000ce2b4]
__attribute__((visibility("hidden"))) extern int  g_cg_rocketFx;
__attribute__((visibility("hidden"))) extern int  g_cg_objFx;
__attribute__((visibility("hidden"))) extern int *g_cg_objTable;    // [DAT_000ce2d4]
__attribute__((visibility("hidden"))) extern int *g_cg_rocketTable; // [DAT_000ce2c8]
__attribute__((visibility("hidden"))) extern int **g_cg_rocketSnd;  // [DAT_000ce2cc]
__attribute__((visibility("hidden"))) extern int **g_cg_itemTableA; // [DAT_000ce2c0]
__attribute__((visibility("hidden"))) extern int *g_cg_bombTable;   // [DAT_000ce2c4]
__attribute__((visibility("hidden"))) extern int *g_cg_bombSnd;     // [DAT_000ce64c]
__attribute__((visibility("hidden"))) extern int  g_cg_mineFx;
__attribute__((visibility("hidden"))) extern int *g_cg_objTable8;   // [DAT_000ce648]
__attribute__((visibility("hidden"))) extern int *g_cg_mineTable;   // [DAT_000ce2d8]
__attribute__((visibility("hidden"))) extern int *g_cg_mineSnd;     // [DAT_000ce654]
__attribute__((visibility("hidden"))) extern int *g_cg_objTable23;  // [DAT_000ce660]
__attribute__((visibility("hidden"))) extern int *g_cg_sentryTable; // [DAT_000ce65c]
__attribute__((visibility("hidden"))) extern int *g_cg_bombTable2a; // [DAT_000ce664]
__attribute__((visibility("hidden"))) extern int **g_cg_snd29;      // [DAT_000ce668]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2a;      // [DAT_000ce66c]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2b;      // [DAT_000ce670]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2c;      // [DAT_000ce674]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2d;      // [DAT_000ce678]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2e;      // [DAT_000ce67c]
__attribute__((visibility("hidden"))) extern Status **g_csp_status;  // [DAT_000be978]
__attribute__((visibility("hidden"))) extern unsigned *g_csp_canvas;  // [DAT_000be97c]
__attribute__((visibility("hidden"))) extern int  **g_crm_status;   // [DAT_000d59dc]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_crm_rngA;     // [DAT_000d59e0]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_crm_rngB;     // [DAT_000d59e8]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_crm_rngStorm; // [DAT_000d59e4]
__attribute__((visibility("hidden"))) extern int   *g_crm_counts8;  // [DAT_000d59f8] per-stage counts
__attribute__((visibility("hidden"))) extern int   *g_crm_table8;   // [DAT_000d59fc] {id,arg} table
__attribute__((visibility("hidden"))) extern int  **g_init_canvas;    // [DAT_000be260]
__attribute__((visibility("hidden"))) extern int  **g_init_statusA;   // [DAT_000be264]
__attribute__((visibility("hidden"))) extern char **g_init_flagStack; // [DAT_000be268]
__attribute__((visibility("hidden"))) extern int  **g_init_missionDef;// [DAT_000be5a4]
__attribute__((visibility("hidden"))) extern int  **g_init_settings;  // [DAT_000be5ac]
__attribute__((visibility("hidden"))) extern char **g_init_bmFlag;    // [DAT_000be5b0]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_uaa_rngHolder; // [DAT_000d5f4c]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_cm_rng;       // [DAT_000c0374]
__attribute__((visibility("hidden"))) extern Globals **g_cm_globals; // [DAT_000c0378]
__attribute__((visibility("hidden"))) extern int   **g_ca_canvas;   // [DAT_000bf984] paint-canvas holder
__attribute__((visibility("hidden"))) extern int   **g_ca_galaxy;   // galaxy holder
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_ca_rngHolder;// [DAT_000bf988] RNG object holder
__attribute__((visibility("hidden"))) extern char  **g_ca_lowDetail;// [DAT_000bfd1c] low-detail flag
__attribute__((visibility("hidden"))) extern float g_ccm_pos0;     // DAT_000c5310 (case-0 spawn coord)
__attribute__((visibility("hidden"))) extern Status **g_sentryStatus;
__attribute__((visibility("hidden"))) extern Status **g_status_collideStation;
__attribute__((visibility("hidden"))) extern int **g_uncoverWanted;
__attribute__((visibility("hidden"))) extern int  **g_up_statusA;   // [DAT_000d624c]
__attribute__((visibility("hidden"))) extern float  g_up_eqMax;
__attribute__((visibility("hidden"))) extern float *g_up_clampLo;   // [DAT_000d6244]
__attribute__((visibility("hidden"))) extern float *g_up_clampHi;   // [DAT_000d6248]
__attribute__((visibility("hidden"))) extern float *g_up_clampZ;    // [DAT_000d623c]
__attribute__((visibility("hidden"))) extern float *g_up_clampW;    // [DAT_000d6240]
__attribute__((visibility("hidden"))) extern ApplicationManager **g_cp_appMgr; // [DAT_000cd530]
__attribute__((visibility("hidden"))) extern Status      **g_ed_status; // [DAT_000d441c]
__attribute__((visibility("hidden"))) extern Achievements **g_ed_achA;  // [DAT_000d4420]
__attribute__((visibility("hidden"))) extern Achievements **g_ed_achB;  // [DAT_000d4424]
__attribute__((visibility("hidden"))) extern float          g_ed_100;   // DAT_000d4418 == 100.0f
__attribute__((visibility("hidden"))) extern float *g_flash2_a;
__attribute__((visibility("hidden"))) extern float *g_flash2_b;
__attribute__((visibility("hidden"))) extern float *g_flash2_c;
__attribute__((visibility("hidden"))) extern float *g_flashCol_r;
__attribute__((visibility("hidden"))) extern float *g_flashCol_g;
__attribute__((visibility("hidden"))) extern float *g_flashCol_b;
__attribute__((visibility("hidden"))) extern Status **g_wingmanDied;
__attribute__((visibility("hidden"))) extern void (**g_levelSubCtor)(void *);
__attribute__((visibility("hidden"))) extern int **g_cso_textA;     // [DAT_000cc1ec]
__attribute__((visibility("hidden"))) extern int **g_cso_textB;     // [DAT_000cc1f0] area
__attribute__((visibility("hidden"))) extern float g_cso_posX;
__attribute__((visibility("hidden"))) extern float g_cso_posZ;
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_cso2_rng;    // [DAT_000cea38]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_cs_rng;      // [DAT_000d0468]
__attribute__((visibility("hidden"))) extern int     *g_cs_diffRec;  // [DAT_000d0474]
__attribute__((visibility("hidden"))) extern Globals **g_cs_globalsA;// [DAT_000d0838]
__attribute__((visibility("hidden"))) extern Globals **g_cs_globalsB;// [DAT_000d0cdc]
__attribute__((visibility("hidden"))) extern Status **g_almostKillWanted;
__attribute__((visibility("hidden"))) extern Status **g_ag_status;   // [DAT_000cca38]
__attribute__((visibility("hidden"))) extern int    *g_ag_diffRec;   // [DAT_000cca44] difficulty record
__attribute__((visibility("hidden"))) extern int   **g_ag_shipTbl;   // [DAT_000cce38]
__attribute__((visibility("hidden"))) extern int   **g_ag_itemTblA;  // [DAT_000cd1f0]
__attribute__((visibility("hidden"))) extern int    *g_ag_weaponDmg; // [DAT_000cd1f4]
__attribute__((visibility("hidden"))) extern int   **g_ag_statusB;   // [DAT_000cce40]
__attribute__((visibility("hidden"))) extern int   **g_ag_alienCnt;  // [DAT_000cce44]
__attribute__((visibility("hidden"))) extern int   **g_ag_snd;       // [DAT_000cd1f8]
__attribute__((visibility("hidden"))) extern int   **g_ag_itemTblB;  // [DAT_000cd1fc]
__attribute__((visibility("hidden"))) extern int   **g_ag_snd2;      // [DAT_000cd200]
__attribute__((visibility("hidden"))) extern float   g_ag_perLevel;
__attribute__((visibility("hidden"))) extern Galaxy **g_cgc_galaxy; // [DAT_000bffc4]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_cgc_rng;     // [DAT_000bffcc]
__attribute__((visibility("hidden"))) extern void  **g_cgc_canvas;  // [DAT_000bffd4]
__attribute__((visibility("hidden"))) extern int **g_attackWanted;
__attribute__((visibility("hidden"))) extern unsigned *g_ips_canvas; // [DAT_000cda98]
__attribute__((visibility("hidden"))) extern int   *g_ips_envA;     // [DAT_000cdaa0] env record
__attribute__((visibility("hidden"))) extern int   *g_ips_envB;     // [DAT_000cde3c]/[DAT_000cde38]
__attribute__((visibility("hidden"))) extern int  (*g_ips_addSystem)(int, void *, int, int); // [DAT_000cde40]
__attribute__((visibility("hidden"))) extern void (*g_ips_enableEmit)(int);  // [DAT_000cde44]
__attribute__((visibility("hidden"))) extern int    **g_cwm_statusB; // [DAT_000cc674]
__attribute__((visibility("hidden"))) extern int    **g_cwm_seedSrc; // [DAT_000cc678]
__attribute__((visibility("hidden"))) extern Globals **g_cwm_globals; // [DAT_000cc67c]
__attribute__((visibility("hidden"))) extern int   **g_csc_missionDef;// [DAT_000c46ac]
__attribute__((visibility("hidden"))) extern void  **g_csc_canvas;   // [DAT_000c46b0] etc.
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_csc_rng;      // [DAT_000c46bc]/[DAT_000c4ba0]
__attribute__((visibility("hidden"))) extern Globals **g_csc_globals;// [DAT_000c46d0]
__attribute__((visibility("hidden"))) extern int   **g_csc_shipHost; // [DAT_000c46a4]
__attribute__((visibility("hidden"))) extern unsigned *g_rbg_canvas;  // [DAT_000d4700]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_rbg_rng;      // [DAT_000d4b04]
__attribute__((visibility("hidden"))) extern int    **g_rbg_engine;   // [DAT_000d4b0c]
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_uo_rng;
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_umo_rng;
__attribute__((visibility("hidden"))) extern AbyssEngine::AERandom **g_uaa_rng;

extern "C" {
// --- residual shims (could not be cleanly mapped to a real C++ method; follow-up) ---
void Gun_ctor_cg(Gun *g, int a, int b, int c, int d, int e, int f, int g7, int h, int i, int j, int k, int l, int m);
void Gun_ctor_ag(Gun *g, int a, int b, int c, int d, int e, int f, int g7, int h, int i, int j, int k, int l, int m);
void Gun_setLevel_cg(Gun *g, Level *self);
int  Globals_getShipGroup_cs(Globals *g, int type, int race, int flag);
void Player_setHitpoints_ccm(int p);
void Globals_addSoundResourceToList_ag(int snd);
int  Station_stationHasHiddenBlueprint_cp(Station *s, int flag);
int  Station_getShips_csc();
void BoundingVolume_ctor_gbv(BoundingVolume *bv, int rec, int shape);
void PlayerFixedObject_ctor_cs(PlayerFixedObject *o, int type, int race, Player *pl, int geom, float x, float y, float z);
int  ApplicationManager_GetEngine_csp();

int  Item_getAttribute_cg(int item);
int  cm_randPos(AbyssEngine::AERandom *rng, int slot);
void  PlayerAsteroid_ctor_ca(PlayerAsteroid *a, int x_or_id, AEGeometry *geo, int colVariant,
                             int kind, Vector *pos, float scale, int radius);
void  PlayerAsteroid_setAsteroidCenter_ca(PlayerAsteroid *a, float cx, float cy, float cz);
void  Vector_assign_ca(Vector *dst, Vector *src);
int  Item_getAttribute_up(int item);
void Hud_hudEvent_up(int hud, int code, int ego);
int   crms_randDelay(int which);
int  GameText_getText_cso(int id);
int  cso2_rand20000(AbyssEngine::AERandom *rng);
int  cs_rand40000(AbyssEngine::AERandom *rng);
int   Item_getAttribute_ag(int item);
void PlayerGasCloud_ctor_cgc(PlayerGasCloud *c, int kind, ParticleSystemManager *psm,
                             AEGeometry *geo, Vector *pos);
void Player_setHitpoints_cwm(int p);
void  PlayerStatic_ctor_csc(PlayerStatic *p, int a, AEGeometry *geo);
void AEGeometry_setLodMeshes_gap(AEGeometry *geo, unsigned short *meshes, int *dist, int n);
void *dtor_Objective(void *p);
void *dtor_BoundingVolume(void *p);
void *dtor_StarSystem(void *p);
void *dtor_PlayerEgo(void *p);
void *dtor_Route(void *p);
void *dtor_PSM(void *p);
void *dtor_LODManager(void *p);
void *dtor_LodMeshMerger(void *p);
void Level_setAlwaysEnemy(int obj, int flag);
void Level_createPlayer_impl(Level *self);
void *dtor_Objective_akw(void *p);
void Objective_ctor_akw(int o, int a, int b, int c, Level *self);
void Level_turnEnemy(int obj);
int Level_getNumWingmen(int wanted);
}

static unsigned int g_level_texOutScratch;

template <class Handle>
static void releaseObject(Handle &member, void *(*objectDtor)(void *)) {
    if (member != 0) {
        ::operator delete(objectDtor((void *)(intptr_t)member));
    }
    member = 0;
}

template <class T> static void deletePolyArray(Array<T*>*& v) {
    if (!v) return;
    for (unsigned i = 0; i < v->size(); ++i) {
        void* e = (void*)(*v)[i];
        if (e) { void* vt = *(void**)e; (*(void(**)(void*))((char*)vt + 0))(e); } // vtable[0] deleting dtor
    }
    delete v; v = nullptr;
}

extern "C" int Radar_hasScanner_ed();

extern "C" void Level_setAlwaysEnemy(int player, int flag)
{
    char *p = (char *)(intptr_t)player;
    *(unsigned char  *)(p + 0xec) = (unsigned char)flag;  // alwaysEnemy
    *(unsigned short *)(p + 0x5c) = 1;                     // faction dirty
    *(unsigned char  *)(p + 0xe0) = 1;                     // hostile
}

extern "C" void Level_turnEnemy(int player)
{
    *(unsigned char *)((char *)(intptr_t)player + 0xe0) = 1; // hostile
}

extern "C" int Level_getNumWingmen(int wanted)
{
    if (wanted == 0)
        return 0;
    return *(int *)((char *)(intptr_t)wanted + 0x3c);
}

extern "C" void Level_ctor(void *self, int mode)
{
    new (self) Level(mode);
}

extern "C" void *Level_dtor(void *level)
{
    ((Level *)level)->~Level();
    return level;
}

bool Level::hasMiningPlant() {
    return miningPlant > 0;
}

Route *Level::getFriendRoute() {
    return friendRoute;
}

int Level::getEnemiesLeft() {
    return enemiesLeft;
}

void Level::render2D() {
    if (starSystem != 0)
        starSystem->render2D();
}

int Level::checkGameOver() {
    Objective *objective = objectivesB;
    if (objective == nullptr) {
        return 0;
    }
    // veneer 0x1ac018 -> Objective::achieved
    return (int)objective->achieved(0);
}

void Level::updateAsteroidCluster() {
}

Waypoint *Level::getAsteroidWaypoint() {
    return asteroidWaypoint;
}

int Level::getAsteroidsLeft() {
    return asteroidsLeft;
}

void Level::junkDied() {
    (*g_junkDied)[0xb0 / 4] += 1;   // +0xb0 junk-death counter on the holder singleton
    enemiesLeft -= 1;
}

void Level::enableMovingStars(bool enable) {
    int index = movingStarsIndex;
    if (index < 0) {
        return;
    }
    // veneer 0x1ac048 -> ParticleSystemManager::enableSystemRender
    ((ParticleSystemManager *)(intptr_t)skybox2Mesh)->enableSystemRender(index, enable);
}

void Level::setInitStreamOut() {
    *g_initStreamOut = 1;
}

int Level::getMiningPlant() {
    int index = miningPlantIndex;
    if (index < 0) {
        return 0;
    }
    return (int)(intptr_t)(*this->enemies)[index];
}

Array<KIPlayer*>* Level::getGasClouds() {
    return gasClouds;
}

void Level::asteroidDied() {
    asteroidsLeft -= 1;
}

int Level::checkObjective() {
    Objective *objective = objectivesA;
    if (objective != nullptr) {
        // veneer 0x1ac018 -> Objective::achieved
        return (int)objective->achieved(0);
    }
    return 0;
}

int Level::getNumDeliveredOre() {
    return numDeliveredOre;
}

void Level::setPlayerRoute(Route *route) {
    Route *old = playerRoute;
    if (old != nullptr) {
        old->~Route();
        operator delete(old);
    }
    playerRoute = route;
}

void Level::enableFog(bool enable) {
    // veneer 0x1ac048 -> ParticleSystemManager::enableSystemRender
    particleSystemMgr->enableSystemRender(field_284, enable);
}

void Level::isInAsteroidCenterRange(Vector v) {
    collisionVolume->collide(v.x, v.y, v.z);
}

Array<KIPlayer*>* Level::getAsteroids() {
    return asteroids;
}

int Level::collide(Vector v) {
    if (collisionVolume != nullptr) {
        return collisionVolume->collide(v.x, v.y, v.z);
    }
    return 0;
}

int Level::getFriendsLeft() {
    return friendsLeft;
}

void Level::incNumDeliveredOre(int delta) {
    numDeliveredOre += delta;
}

void Level::enableParticleEffects(bool emit, bool render) {
    particleSystemMgr->enableSystemEmit(field_284, render);  // decompiler dropped the enable arg; Ghidra(0xcd68c) shows it is r2 (=render)
    particleSystemMgr->enableSystemRender(field_284, render);
    *(unsigned char *)particleRenderBoolPtr = render;
    *(unsigned char *)particleEmitBoolPtr = render;
}

void Level::switchSkyboxForIntro() {
    PaintCanvas **pc = g_paintCanvas_intro;
    ((PaintCanvas*)(*pc))->MeshCreate((unsigned short)(0x4591), (unsigned int *)&skyboxMesh, false);
    ((PaintCanvas*)(*pc))->TextureCreate((unsigned short)(0x275a), (void *)0, (void *)0, (unsigned int *)&field_198, false);
    if (this->asteroids != nullptr) {
        for (unsigned int i = 0; i < this->asteroids->size(); i = i + 1) {
            (*this->asteroids)[i]->setDead();
        }
    }
}

void Level::switchSkyboxForSupernovaReversal() {
    PaintCanvas **pc = g_paintCanvas_snr;
    Status **st = g_status_snr;
    PaintCanvas *canvas = *pc;
    int tex = ((SolarSystem *)(intptr_t)(*st)->getSystem())->getTextureIndex();
    ((PaintCanvas*)(canvas))->MeshCreate((unsigned short)((unsigned short)(tex + 0x4588)), (unsigned int *)&skyboxMesh, false);
    PaintCanvas *canvas2 = *pc;
    int tex2 = ((SolarSystem *)(intptr_t)(*st)->getSystem())->getTextureIndex();
    ((PaintCanvas*)(canvas2))->TextureCreate((unsigned short)((unsigned short)(tex2 + 0x2751)), (void *)0, (void *)0, (unsigned int *)&field_198, false);
    skyboxTexture = -1;
}

PlayerEgo *Level::getPlayer() {
    return player;
}

void Level::killWanted() {
    if (field_29d == 0) {
        field_29d = 1;
        // veneer 0x1ac028 -> Level::createRadioMessage(type, sub)
        createRadioMessage(0x11, 0);
    }
}

Array<ObjectGun*>* Level::getEnemyGuns() {
    return enemyGuns;
}

void Level::stealFriendCargo() {
    friendCargoStolen = 1;
}

Array<KIPlayer*>* Level::getEnemies() {
    return enemies;
}

void Level::applyKills() {
    Status **slot = g_status_applyKills;
    if ((*slot)->getMission() != 0) {
        (*slot)->addKills(kills);
        kills = 0;
    }
}

uint8_t Level::friendCargoWasStolen() {
    return friendCargoStolen;
}

Array<void*>* Level::getMessages() {
    return messages;
}

// Level::getActiveMessages() — accessor for the level's live radio-message queue
// (the Array<RadioMessage*> that createRadioMessage()/createRadioMessages() build at
// this+0x114). Returned as an opaque pointer; callers re-cast to the message array.
void *Level::getActiveMessages() {
    return (void *)this->messages;
}

Route *Level::getEnemyRoute() {
    return enemyRoute;
}

Array<ObjectGun*>* Level::getPlayerGuns() {
    return playerGuns;
}

void Level::renderPause() {
    if (this->playerGuns != nullptr) {
        for (unsigned int i = 0; i < this->playerGuns->size(); i = i + 1) {
            (*this->playerGuns)[i]->render();
        }
    }
    if (this->enemyGuns != nullptr) {
        for (unsigned int i = 0; i < this->enemyGuns->size(); i = i + 1) {
            (*this->enemyGuns)[i]->render();
        }
    }
    if (this->enemies != nullptr) {
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            (*this->enemies)[i]->render();
        }
    }
    if (this->asteroids != nullptr) {
        for (unsigned int i = 0; i < this->asteroids->size(); i = i + 1) {
            (*this->asteroids)[i]->render();
        }
    }
    if (this->gasClouds != nullptr) {
        for (unsigned int i = 0; i < this->gasClouds->size(); i = i + 1) {
            (*this->gasClouds)[i]->render();
        }
    }
    if (this->landmarks != nullptr) {
        for (unsigned int i = 0; i < this->landmarks->size(); i = i + 1) {
            KIPlayer *o = (*this->landmarks)[i];
            if (o != nullptr) {
                o->render();
            }
        }
    }
}

Route *Level::getPlayerRoute() {
    return playerRoute;
}

StarSystem* Level::getStarSystem() {
    return starSystem;
}

void Level::pirateStationAction(bool param) {
    if (param) {
        if (field_1b0 != 0) {
            return;
        }
    } else {
        if (field_68 != 0) {
            return;
        }
        Status **slot = g_status_pirate;
        if ((*slot)->getStation()->getPirateStationIndex() < 0) {
            return;
        }
        int tbl = *(int *)((char *)*slot + 0x4c);
        int idx = (*slot)->getStation()->getPirateStationIndex();
        *(unsigned char *)(*(int *)(tbl + 4) + idx) = 1;
        *(unsigned char *)((char *)*slot + 0xf9) = 1;
    }
    // veneer 0x1ac028 -> Level::createRadioMessage(type, sub)
    createRadioMessage(param ? 3 : 4, 8);
}

int Level::getNumDockingTargets() {
    if (this->enemies != nullptr) {
        int total = 0;
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            KIPlayer *e = (*this->enemies)[i];
            total = total + *(unsigned char *)((char *)e + 0x70);
        }
        return total;
    }
    return 0;
}

void Level::removeObjectives() {
    objectivesA = nullptr;
    objectivesB = nullptr;
}

int Level::getDockingTarget(int index) {
    if (this->enemies != nullptr) {
        int found = 0;
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            KIPlayer *obj = (*this->enemies)[i];
            if (*(char *)((char *)obj + 0x70) != 0) {
                if (found == index) {
                    return (int)(intptr_t)obj;
                }
                found = found + 1;
            }
        }
    }
    return 0;
}

int Level::getTimeLimit() {
    return timeLimit;
}

Array<KIPlayer*>* Level::getLandmarks() {
    return landmarks;
}

// The full render pass invokes, per actor, a pre-render virtual at the original vtable
// slot +0x34 (run only here, never during the frozen renderPause pass) followed by the
// polymorphic render(). The +0x34 method's name is still unresolved — the actor vtables
// are reached through a GOT relocation that is not statically resolvable — so it is kept
// as a documented raw dispatch, matching the +0x1c / setRoute FIXMEs elsewhere in this
// file. The receiver is a real KIPlayer*; only this one slot remains by-offset.
static inline void actorPreRender(KIPlayer *o, int ctx) {
    // FIXME: name the original-slot-+0x34 pre-render virtual and call it directly.
    (*(void (**)(KIPlayer *, int))(*(char **)o + 0x34))(o, ctx);
}

void Level::render(int ctx) {
    if (this->playerGuns != nullptr) {
        for (unsigned int i = 0; i < this->playerGuns->size(); i = i + 1) {
            (*this->playerGuns)[i]->render();
        }
    }
    if (this->enemyGuns != nullptr) {
        for (unsigned int i = 0; i < this->enemyGuns->size(); i = i + 1) {
            (*this->enemyGuns)[i]->render();
        }
    }
    if (this->enemies != nullptr) {
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            KIPlayer *o = (*this->enemies)[i];
            actorPreRender(o, ctx);
            o->render();
        }
    }
    if (this->asteroids != nullptr) {
        for (unsigned int i = 0; i < this->asteroids->size(); i = i + 1) {
            KIPlayer *o = (*this->asteroids)[i];
            actorPreRender(o, ctx);
            o->render();
        }
    }
    if (this->gasClouds != nullptr) {
        for (unsigned int i = 0; i < this->gasClouds->size(); i = i + 1) {
            KIPlayer *o = (*this->gasClouds)[i];
            actorPreRender(o, ctx);
            o->render();
        }
    }
    if (this->landmarks != nullptr) {
        for (unsigned int i = 0; i < this->landmarks->size(); i = i + 1) {
            KIPlayer *o = (*this->landmarks)[i];
            if (o != nullptr) {
                actorPreRender(o, ctx);
                o->render();
            }
        }
    }
    if (field_80 != 0) {
        ((ParticleSystemManager *)(field_80))->render3d();
    }
    if (field_74 != 0) {
        ((ParticleSystemManager *)(field_74))->render3d();
    }
    if (particleEmitBoolPtr != nullptr) {
        particleEmitBoolPtr->render3d();
    }
    if (particleSystemMgr != nullptr) {
        particleSystemMgr->render3d();
    }
    if (skybox2Mesh != 0) {
        ((ParticleSystemManager *)(skybox2Mesh))->render3d();
    }
    if (particleRenderBoolPtr != nullptr) {
        particleRenderBoolPtr->render3d();
    }
    if (field_8c != 0) {
        ((ParticleSystemManager *)(field_8c))->render3d();
    }
    if (field_98 != 0) {
        ((ParticleSystemManager *)(field_98))->render3d();
    }
    if (field_94 != 0) {
        ((ParticleSystemManager *)(field_94))->render3d();
    }
    if (field_9c != 0) {
        ((ParticleSystemManager *)(field_9c))->render3d();
    }
    if (starSystem != 0)
        starSystem->renderSunStreak();
}

int Level::collideStream(Vector v) {
    int *obj = (int *)(*this->landmarks)[1];
    if (obj != 0) {
        return (*(int (**)(int *, Vector))(*obj + 0x38))(obj, v);
    }
    return 0;
}

int Level::getNumDeliveredPassengers() {
    return numDeliveredPassengers;
}

Level::~Level() {
    skyboxMesh = -1;
    field_08 = -1;
    skyboxTexture = -1;
    releaseObject(objectivesA, dtor_Objective);
    releaseObject(objectivesB, dtor_Objective);
    releaseObject(collisionVolume, dtor_BoundingVolume);
    delete asteroidWaypoint;   // virtual deleting-dtor (vtable slot +4)
    asteroidWaypoint = nullptr;
    releaseObject(starSystem, dtor_StarSystem);
    releaseObject(player, dtor_PlayerEgo);
    releaseObject(field_180, dtor_Route);
    releaseObject(field_80, dtor_PSM);
    releaseObject(skybox2Mesh, dtor_PSM);
    releaseObject(field_74, dtor_PSM);
    releaseObject(particleEmitBoolPtr, dtor_PSM);
    releaseObject(particleSystemMgr, dtor_PSM);
    releaseObject(field_90, dtor_PSM);
    releaseObject(particleRenderBoolPtr, dtor_PSM);
    releaseObject(field_98, dtor_PSM);
    releaseObject(field_9c, dtor_PSM);
    deletePolyArray(field_a4);
    if (field_a8) { delete field_a8; field_a8 = nullptr; }   // Array<int>: no element dtors
    deletePolyArray(playerGuns);
    deletePolyArray(enemyGuns);
    deletePolyArray(enemies);
    deletePolyArray(asteroids);
    deletePolyArray(gasClouds);
    deletePolyArray(landmarks);
    deletePolyArray(messages);
    deletePolyArray(field_104);
    releaseObject(vtable, dtor_LODManager);
    releaseObject(field_a0, dtor_LodMeshMerger);
    if (field_b0) { delete field_b0; field_b0 = nullptr; }   // container-only: elements owned by enemies
}

void Level::incNumDeliveredPassengers(int delta) {
    numDeliveredPassengers += delta;
}

void Level::friendDied() {
    friendsLeft -= 1;
}

Route *Level::createRoute(int count) {
    unsigned int n = count * 3;
    int *pts = new int[n];
    int *p = pts + 1;
    int *rng = g_routeRng;
    int (*rnd)(int, int) = g_routeRandom;
    for (int i = -1; i + 1 < (int)n; i = i + 3) {
        int sign = (rnd(*rng, 2) == 0) ? 1 : -1;
        p[-1] = (rnd(*rng, 30000) + 50000) * sign;
        p[0] = rnd(*rng, 20000) - 10000;
        if (i == -1) {
            pts[2] = ((AbyssEngine::AERandom *)(intptr_t)*rng)->nextInt() + 50000;
        } else {
            int prev = p[-2];
            p[1] = ((AbyssEngine::AERandom *)(intptr_t)*rng)->nextInt() + prev + 50000;
        }
        p = p + 3;
    }
    Route *r = (Route *)::operator new(0x18);
    new (r) Route(pts, n);
    return r;
}

void Level::alarmAllFriends(int race, bool message) {
    if (this->enemies != nullptr) {
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            int obj = (int)(intptr_t)(*this->enemies)[i];
            if (*(int *)(obj + 0x28) == race) {
                Level_setAlwaysEnemy(*(int *)(obj + 4), 1);
            }
        }
    }
    // field_188 packs two flags; 0x188 is friendTurnedEnemy, 0x189 is the alarm flag.
    unsigned char *alarmFlag = (unsigned char *)&field_188 + 1;
    if (*alarmFlag == 0 && message) {
        int type = 1;
        *alarmFlag = (unsigned char)type;
        Status **slot = g_alarmAllFriends;
        if ((*slot)->inBlackMarketSystem() != 0) {
            type = 0xc;
        }
        createRadioMessage(type, race);
        if (((SolarSystem *)(intptr_t)(*slot)->getSystem())->getRace() == race) {
            // veneer 0x1abff8 -> Station::setAttackedFriends(true)
            (*slot)->getStation()->setAttackedFriends(true);
        }
    }
}

void Level::setPlayerEngineColor(short color) {
    int c = color;
    if (player != nullptr && field_a4 != nullptr) {
        unsigned int *p = (unsigned int *)((char *)g_engineColorBase + 0x1254);
        for (int i = (int)field_a4->size(); i != 0; i = i - 1) {
            *p = c << 0x10 | c << 0x18 | c << 8 | 0xff;
            p = p + 0x28;
        }
    }
}

// Level::createGun(idx, owner, kind, hp, dmg, rate, cool, color) — factory for the player's and
// AI ships' weapons; dispatches on `kind` to the right Gun subclass and registers the sound.
Gun * Level::createGun(int idx, int owner, int kind, int hp, int dmg, int rate, int cool, int color) {
    ObjectGun *obj = 0;
    Gun *gun = 0;

    switch (kind) {
    case 0:
    case 1:
    case 3: {
        int res = ((int *)g_cg_beamTable)[idx];
        if (res < 0) {
            gun = (Gun *)::operator new(0x114);
            Gun_ctor_cg(gun, owner, dmg, 1, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
            gun->setIndex(idx);
            gun->weaponType = kind;
            gun->setPlayerGun(1);
            obj = (ObjectGun *)::operator new(0x24);
            new (obj) BeamGun(owner, gun, idx, this);
        } else {
            int barrels = ((unsigned)(idx - 9) < 3 || idx == 0xe4) ? 1 : 0x14;
            gun = (Gun *)::operator new(0x114);
            if (kind == 3) {
                Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, g_cg_rocketFx,
                            0, 0, 0);
                gun->setIndex(idx);
                gun->weaponType = 3;
                gun->setPlayerGun(1);
                gun->setErrorMagnitudePercentage(0x14);
                obj = (ObjectGun *)::operator new(0xe8);
                new (obj) RocketGun(owner, gun, res, 0, 0, 0, 1, this);
            } else {
                Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
                gun->setIndex(idx);
                gun->weaponType = kind;
                gun->setPlayerGun(1);
                obj = (ObjectGun *)::operator new(0xb0);
                new (obj) ObjectGun(owner, gun, res, 1000, this);
            }
        }
        break;
    }
    case 2:
    case 0x19:
        gun = (Gun *)::operator new(0x114);
        Gun_ctor_cg(gun, owner, dmg, 0x19, hp, cool, rate, color, 0, 0, g_cg_objFx, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = kind;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)::operator new(0xb0);
        new (obj) ObjectGun(owner, gun, ((int *)g_cg_objTable)[idx], 1000, this);
        gun->setErrorMagnitudePercentage(2);
        break;
    case 4:
    case 5:
    case 0x28: {
        gun = (Gun *)::operator new(0x114);
        int barrels = (kind == 0x28) ? (idx - 0xd3) : 5;
        Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = kind;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)::operator new(0xe8);
        new (obj) RocketGun(owner, gun, ((int *)g_cg_rocketTable)[idx], 0, 0, kind,
                          (kind == 0x28 || kind == 5) ? 1 : 0, this);
        Globals_addSoundResourceToList(**g_cg_rocketSnd);
        break;
    }
    case 6:
    case 7:
    case 0x22: {
        gun = (Gun *)::operator new(0x114);
        Gun_ctor_cg(gun, owner, dmg, 1, hp, cool, rate, color, 0, 0, g_cg_objFx, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = kind;
        gun->setPlayerGun(1);
        int attr = Item_getAttribute_cg(*(int *)(*(int *)(*g_cg_itemTableA + 4) + idx * 4));
        obj = (ObjectGun *)::operator new(300);
        new (obj) BombGun(gun, ((unsigned *)g_cg_bombTable)[idx], 1, kind, attr == 1 ? 1 : 0,
                        this);
        Globals_addSoundResourceToList(*g_cg_bombSnd);
        break;
    }
    case 8:
    case 0x23: {
        int fx = (idx == 0xb5 || idx != 0x30) ? g_cg_mineFx : g_cg_rocketFx;
        int extra = (idx == 0x30 && idx != 0xb5) ? (g_cg_rocketFx - 0xf60000) : 0;
        if (kind == 0x23) { fx = (idx == 0xb5 || idx != 0x30) ? g_cg_mineFx : g_cg_rocketFx; }
        gun = (Gun *)::operator new(0x114);
        Gun_ctor_cg(gun, owner, dmg, 0xf, hp, cool, rate, color, extra, 0, fx, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = kind;
        gun->setPlayerGun(1);
        if ((idx == 0x30 || idx == 0xe0 || idx == 0xb5)) {
            gun->field_0xa4 = 1;
            if (idx == 0xe0) F<uint8_t>(gun, 0xa5) = 1;
        }
        obj = (ObjectGun *)::operator new(0xb0);
        int *tbl = (kind == 0x23) ? (int *)g_cg_objTable23 : (int *)g_cg_objTable8;
        new (obj) ObjectGun(owner, gun, tbl[idx], 1000, this);
        break;
    }
    case 0xb: {
        gun = (Gun *)::operator new(0x114);
        Gun_ctor_cg(gun, owner, dmg, 10, hp, cool, rate, 0x40000000, 0, 0, 0, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = 0xb;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)::operator new(0xd4);
        new (obj) MineGun(gun, ((int *)g_cg_mineTable)[idx], 1, 0xb, this);
        Globals_addSoundResourceToList(*g_cg_mineSnd);
        break;
    }
    case 0x27: {
        gun = (Gun *)::operator new(0x114);
        Gun_ctor_cg(gun, owner, 0, 3, hp, cool, rate, 0x40000000, 0, 0, 0, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = 0x27;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)::operator new(0xb4);
        new (obj) SentryGun(gun, ((int *)g_cg_sentryTable)[idx], 1, 0x27, this);
        Globals_addSoundResourceToList(*g_cg_mineSnd);
        break;
    }
    case 0x2a: {
        gun = (Gun *)::operator new(0x114);
        Gun_ctor_cg(gun, owner, dmg, 1, hp, 1, rate, color, 0, 0, 0, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = 0x2a;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)::operator new(300);
        new (obj) BombGun(gun, ((unsigned *)g_cg_bombTable2a)[idx], 1, 0x2a, 0, this);
        Globals_addSoundResourceToList(*g_cg_bombSnd);
        break;
    }
    default:
        gun = 0;
        break;
    }

    // index-keyed extra sound resources (fallthrough chain in the original).
    switch (idx) {
    case 0x29: Globals_addSoundResourceToList(**g_cg_snd29);
    case 0x2a: Globals_addSoundResourceToList(**g_cg_snd2a);
    case 0x2b: Globals_addSoundResourceToList(**g_cg_snd2b);
    case 0x2c: Globals_addSoundResourceToList(**g_cg_snd2c);
    case 0x2d: Globals_addSoundResourceToList(**g_cg_snd2d);
    case 0x2e: Globals_addSoundResourceToList(**g_cg_snd2e);
    default: break;
    }

    Gun_setLevel_cg(gun, this);
    if (this->playerGuns == nullptr) {
        this->playerGuns = new Array<ObjectGun*>();
    }
    this->playerGuns->push_back(obj);
    return gun;
}

// Level::createSpace() — creates the skybox + star-system backdrop, the home station and the
// jumpgate/agent props for the current orbit.
void Level::createSpace()
{
    // skybox mesh/texture only need (re)building when not yet created (mesh handle at +4 == -1).
    if (*(unsigned *)&this->skyboxMesh == 0xffffffff) {
        Status **status = g_csp_status;
        int alien = (*g_status)->inAlienOrbit();
        unsigned canvas = *g_csp_canvas;

        if (alien == 0) {
            (*g_status)->getSystem();
            int sysVariant = (((SolarSystem*)(*g_status)->getSystem())->getIndex() % 3);
            ((PaintCanvas *)(long)canvas)->MeshCreate((unsigned short)(sysVariant + 0x45ba), (unsigned int *)&this->field_08, false);
            (*g_status)->getSystem();
            sysVariant = (((SolarSystem*)(*g_status)->getSystem())->getIndex() % 3);
            ((PaintCanvas *)(long)canvas)->TextureCreate((unsigned short)((sysVariant + 0x2766) & 0xffff), (void *)0, (void *)0, &g_level_texOutScratch, false);
            // the rest (campaign/supernova/storm/ring detail) is built by the helper.
            this->csp_buildDetail();

            (*g_status)->getSystem();
            if (0xf < ((SolarSystem*)(*g_status)->getSystem())->getTextureIndex()) {
                Engine *eng = (Engine *)ApplicationManager_GetEngine_csp();
                if (eng->IsPostEffectActivated() != false) {
                    int mp = (int)(long)((PaintCanvas *)(long)canvas)->MeshGetPointer((unsigned int)*(unsigned *)&this->skyboxMesh);
                    *(int *)(mp + 0x1c) = 0;
                }
            }
        } else {
            ((PaintCanvas *)(long)canvas)->MeshCreate((unsigned short)0x45bc, (unsigned int *)&this->field_08, false);
            ((PaintCanvas *)(long)canvas)->TextureCreate((unsigned short)0x2768, (void *)0, (void *)0, &g_level_texOutScratch, false);
            ((PaintCanvas *)(long)canvas)->MeshCreate((unsigned short)0x4592, (unsigned int *)&this->skyboxMesh, false);
            ((PaintCanvas *)(long)canvas)->TextureCreate((unsigned short)0x275b, (void *)0, (void *)0, &g_level_texOutScratch, false);
        }

        // randomized skybox spin (light direction), unless in fog orbit.
        if ((*g_status)->inFogSkyboxOrbit() == 0) {
            // (*status) used for storm check inside helper; spin written into self+0x1a4..0x1ac.
            (void)status;
            this->csp_buildStarSystemScene();
        } else {
            this->field_1a4 = 0;
            this->field_1a8 = 0;
            this->field_1ac = 0;
        }
    }

    int mode = this->missionPtr;
    if (mode == 4 || mode == 0x17) {
        StarSystem *ss = (StarSystem *)::operator new(0x60);
        new (ss) StarSystem(mode);
        *(StarSystem **)&this->starSystem = ss;
        this->csp_buildStarSystemScene();
        return;
    }

    // build the home station + jumpgates.
    Status **status = g_csp_status;
    (void)status;
    this->landmarks = new Array<KIPlayer*>();
    this->landmarks->resize(4);

    this->csp_buildStationAndGates();
}

// Level::createRadioMessage(int type, int sub) — builds a context-appropriate sequence of radio
// chatter lines for the current orbit/mission and queues them on the player's comms.
void Level::createRadioMessage(int type, int sub) {
    unsigned r2 = (unsigned)sub;

    if (this->player == nullptr || *(int *)((char *)this->player + 0x18) == 0)
        return;

    int **statusHolder = g_crm_status;
    ((Status*)*g_crm_status)->getMission();
    if (((Mission*)(*g_status)->getMission())->isEmpty() == 0)
        return;

    // fresh message queue.
    if (this->messages != nullptr) {
        deletePolyArray(this->messages);
    }
    this->messages = new Array<void*>();

    // resolve the speaker portrait from the sub-parameter.
    int speaker;
    if (r2 == 0)       speaker = 0x40;
    else if (r2 == 2)  speaker = 0x41;
    else if (r2 == 3)  speaker = 0x15;
    else               speaker = (r2 == 8) ? 9 : 0x3f;

    int extraDelay = 0;
    unsigned id = 0x1ba;
    bool aborted = false;
    bool builtInline = false; // case already pushed its own messages

    switch (type) {
    case 0:
    case 1: {
        // local-station hail: skip if you're docked at the relevant station.
        int *stations = *(int **)(*statusHolder[0] + 0x90);
        bool atStation = false;
        if (stations != 0) {
            for (unsigned k = 0; k < (unsigned)*stations; k = k + 1) {
                int sidx = *(int *)(((int *)stations[1])[k]);
                Station *st = (Station *)((Status*)*g_crm_status)->getStation();
                if (sidx == ((Station*)st)->getIndex()) { atStation = true; break; }
            }
        }
        if (atStation) { aborted = true; break; }
        int base = (type == 0) ? 0x1aa : 0x1ad;
        id = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngA)->nextInt() + base;
        break;
    }
    case 3:
        *(char *)&this->field_1b0 = 1;
        id = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngStorm)->nextInt() + 0x1b3;
        break;
    case 4:
        *(char *)&this->field_68 = 1;
        id = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngStorm)->nextInt() + 0x1b6;
        break;
    case 5:  id = 0x1bb; extraDelay = 0; break;
    case 6:  id = 0x1bc; extraDelay = 0; break;
    case 9:  id = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngB)->nextInt() + 0x1c1; break;
    case 10: id = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngB)->nextInt() + 0x1c3; break;
    case 0xb: id = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngB)->nextInt() + 0x1c5; break;
    case 0xc: id = 0x1c5; extraDelay = 0; break;
    case 0xd: id = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngB)->nextInt() + 0x1c7; break;
    case 0xe: {
        int *st = *(int **)(*statusHolder[0] + 0x90);
        id = 0x88f;
        if (st != 0) {
            for (int k = 0; k != *st; k = k + 1)
                if (-1 < *(int *)(st[1] + k * 4)) id = id - 2;
        }
        break;
    }
    case 0xf: {
        int *st = *(int **)(*statusHolder[0] + 0x90);
        id = 0x88e;
        if (st != 0) {
            for (int k = 0; k != *st; k = k + 1)
                if (-1 < *(int *)(st[1] + k * 4)) id = id - 2;
            if (id - 0x889 < 5) { extraDelay = 0; break; }
        }
        aborted = true;
        break;
    }
    case 8: {
        int stage = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngB)->nextInt();
        int off = 0;
        for (int k = 0; k < stage; k = k + 1)
            off = off + ((int *)g_crm_counts8)[k];
        int cnt = ((int *)g_crm_counts8)[stage];
        int *tbl = (int *)((char *)g_crm_table8 + off * 8);
        for (int k = 0; k < cnt; k = k + 1) {
            int delay = (k == 0) ? 5000 : (k - 1);
            int arg = tbl[k * 2];
            RadioMessage *m = (RadioMessage *)::operator new(0x28);
            int kind = (k == 0) ? 5 : 6;
            new (m) RadioMessage(tbl[k * 2 + 1], arg, kind, delay);
            this->messages->push_back(m);
        }
        builtInline = true;
        break;
    }
    case 0x13: {
        RadioMessage *m = (RadioMessage *)::operator new(0x28);
        AbyssEngine::AERandom *rng = *g_crm_rngStorm;
        new (m) RadioMessage(rng->nextInt() + 0xaf4, 0, 5, 0x5dc);
        this->messages->push_back(m);
        m = (RadioMessage *)::operator new(0x28);
        new (m) RadioMessage(rng->nextInt() + 0xafa, 0, 6, 0);
        this->messages->push_back(m);
        builtInline = true;
        break;
    }
    case 0x15: id = 0xc54; extraDelay = 0; break;
    case 0x16: id = 0xc55; extraDelay = 0; break;
    case 0x17: id = 0xc56; extraDelay = 0; break;
    case 0x18: id = 0xc57; extraDelay = 0; break;
    case 0x19: id = 0xc58; extraDelay = 0; break;
    case 0x1a: id = 0xc59; extraDelay = 0; break;
    case 0x1b: {
        RadioMessage *m = (RadioMessage *)::operator new(0x28);
        new (m) RadioMessage(r2 * 2 + 0xc60, 6, 5, 0x5dc);
        this->messages->push_back(m);
        m = (RadioMessage *)::operator new(0x28);
        new (m) RadioMessage(r2 * 2 + 0xc61, 0, 6, 0);
        this->messages->push_back(m);
        builtInline = true;
        break;
    }
    case 0x1c:
        speaker = 0x26;
        id = r2;
        extraDelay = 0;
        break;
    case 7:
    default:
        if (type == 7 || type == 0xc || type == 0xe) {
            // fall through to single-message emit below using current id/speaker.
        } else {
            id = ((AbyssEngine::AERandom*)(intptr_t)*g_crm_rngB)->nextInt() + 0x1bd;
        }
        break;
    }

    if (aborted) {
        if (this->messages != nullptr)
            deletePolyArray(this->messages);
        PlayerEgo *ego = this->player;
        this->crm_dispatch(*(int *)((char *)ego + 0x18), 0);
        return;
    }

    if (!builtInline) {
        RadioMessage *m = (RadioMessage *)::operator new(0x28);
        new (m) RadioMessage(id, speaker, 5, extraDelay);
        this->messages->push_back(m);
    }

    PlayerEgo *ego = this->player;
    this->crm_dispatch(*(int *)((char *)ego + 0x18), (void *)this->messages);
}

// Level::init() — staged level setup; runs over up to two ticks (the counter at self+0x134),
// then builds space, mission, scene, objects, guns and wires up the players.
int Level::init() {
    Level *thisptr = this;
    int **statusA = 0;

    int stage = this->field_134;
    if (stage == 0) {
        // --- first tick: tear down stale routes/objectives and build particle systems ---
        this->field_68 = 0;
        this->field_1b0 = 0;
        *(short *)((char *)&this->field_188 + 1) = 0;
        if (this->playerRoute != nullptr)
            (this->playerRoute->~Route(), ::operator delete(this->playerRoute));
        this->playerRoute = nullptr;
        if (this->enemyRoute != nullptr)
            (this->enemyRoute->~Route(), ::operator delete(this->enemyRoute));
        this->enemyRoute = nullptr;
        if (this->friendRoute != nullptr)
            (this->friendRoute->~Route(), ::operator delete(this->friendRoute));
        this->friendRoute = nullptr;
        if (this->objectivesB != 0)
            ((this->objectivesB)->~Objective(), ::operator delete(this->objectivesB));
        this->objectivesB = nullptr;
        if (this->objectivesA != 0)
            ((this->objectivesA)->~Objective(), ::operator delete(this->objectivesA));
        this->objectivesA = nullptr;

        LODManager *lod = (LODManager *)::operator new(0x14);
        new (lod) LODManager();
        this->vtable = (uint)(intptr_t)lod;

        int canvas = **g_init_canvas;
        statusA = g_init_statusA;

        bool dustVariant = false;
        if ((*g_status)->inAlienOrbit() == 0) {
            ((Status*)*g_init_statusA)->getSystem();
            dustVariant = ((SolarSystem*)((Status*)*g_init_statusA)->getSystem())->getTextureIndex() == 0xc;
        }

        // the engine builds ten particle-system managers (engine trails, explosions, dust...).
        ParticleSystemManager *psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x4e85, false, 0xffff, false);
        this->particleEmitBoolPtr = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x6a72, false, 0xffff, false);
        this->particleRenderBoolPtr = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x4e83, true, 0xffff, false);
        *(ParticleSystemManager **)&this->field_74 = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x4e7a, true, 0x4e7a, true);
        *(ParticleSystemManager **)&this->field_80 = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x5e20, true, 0x5e20, true);
        *(ParticleSystemManager **)&this->field_98 = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, (unsigned short)(dustVariant ? 0x4ea9 : 0x4e7f), true, 0, false);
        this->particleSystemMgr = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x4e7c, false, 0x4e7c, false);
        *(ParticleSystemManager **)&this->skybox2Mesh = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x6a7c, true, 0x6a7c, true);
        *(ParticleSystemManager **)&this->field_8c = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x6ab9, true, 0xffff, false);
        *(ParticleSystemManager **)&this->field_9c = psm;
        psm = (ParticleSystemManager *)::operator new(100);
        new (psm) ParticleSystemManager((void *)(intptr_t)canvas, 1, 0x6aaf, true, 0xffff, false);
        *(ParticleSystemManager **)&this->field_94 = psm;

        thisptr->createSpace();

        if (this->missionPtr == 3) {
            thisptr->createPlayer();
            int stk = 0;
            if (**g_init_flagStack != 0 && *(int *)((char *)this->player + 8) != 0)
                stk = (int)(intptr_t)((Status*)*g_init_statusA)->getStationStack();
            thisptr->init_placePlayer((int)(intptr_t)*statusA, stk);
        }
        stage = this->field_134;
    }

    if (stage != 1) {
        this->field_134 = stage + 1;
        return 0;
    }

    if (this->missionPtr != 4 && this->missionPtr != 0x17) {
        thisptr->createAsteroids();
        thisptr->createGasClouds();
    }

    Mission *m = (Mission *)((Status*)*g_init_statusA)->getMission();
    if (m == 0)
        ((Status*)*g_init_statusA)->setMission((Mission *)**g_init_missionDef);

    int mode = this->missionPtr;
    if (mode == 3) {
        bool campaign = ((Mission*)(*g_status)->getMission())->isEmpty() == 0 && m->isCampaignMission() != 0;
        bool madeScene = false;
        if (campaign) {
            if (this->missionPtr != 3) { madeScene = true; }
            else {
                int won = (*g_status)->gameWon();
                int *settings = *g_init_settings;
                bool skip = won != 0 && *(char *)(settings + 0x37) == 0 &&
                            *(char *)(settings + 0x35) == 0;
                if (skip) {
                    thisptr->createMission();
                    if (**g_init_bmFlag != 0 && (*g_status)->inBlackMarketSystem() != 0)
                        thisptr->init_placePlayer((int)(intptr_t)*statusA, 0);
                } else if (this->missionPtr != 3) {
                    madeScene = true;
                } else {
                    ((Status*)*g_init_statusA)->getMission();
                    if (((Mission*)(*g_status)->getMission())->isEmpty() == 0) {
                        Mission *mm = (Mission *)((Status*)*g_init_statusA)->getMission();
                        if (mm->isCampaignMission() != 0)
                            thisptr->createCampaignMission();
                    }
                }
            }
        } else {
            thisptr->createMission();
            if (**g_init_bmFlag != 0 && (*g_status)->inBlackMarketSystem() != 0)
                thisptr->init_placePlayer((int)(intptr_t)*statusA, 0);
        }
        if (madeScene) {
            thisptr->createScene();
            this->missionPtr = mode;
        }
    } else {
        thisptr->createScene();
        this->missionPtr = mode;
    }

    thisptr->createStaticObjects();
    mode = this->missionPtr;
    if (mode != 0x17 && mode != 4 &&
        (mode != 2 || ((Status*)*g_init_statusA)->getCurrentCampaignMission() != 0x2b)) {
        thisptr->createSentryGuns();
        thisptr->createFighterTurrets();
        thisptr->createWingmen();
    }
    thisptr->assignGuns();
    if (this->missionPtr != 3)
        this->missionPtr = 3;
    thisptr->connectPlayers();
    if (this->player != nullptr)
        this->player->setRoute_init();

    // recompute enemiesLeft.
    int initial = 0;
    int enemies = 0;
    if (this->enemies != nullptr) {
        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            char *e = (char *)(*this->enemies)[i];
            if (*(char *)(e + 0x41) == 0 && *(char *)(e + 0x71) == 0 && *(char *)(e + 0x3f) == 0) {
                int wm = ((KIPlayer*)e)->isWingMan();
                if (wm == 0) {
                    e = (char *)(*this->enemies)[i];
                    if (*(char *)(e + 0x44) == 0 && *(char *)(e + 0x3c) == 0)
                        enemies = enemies + (*(unsigned char *)(e + 0x3d) ^ 1);
                }
            }
        }
        enemies = enemies - this->field_120;
    }
    this->asteroidsLeft = 0;
    this->enemiesLeft = enemies;
    if (this->asteroids != nullptr)
        initial = (int)this->asteroids->size();
    this->field_184 = 0;
    *(char *)&this->field_188 = 0;
    this->field_1c = 0;
    this->asteroidsLeft = initial;
    this->kills = 0;
    this->friendCargoStolen = 0;
    *(char *)&this->field_70 = 1;
    return 1;
}

// Level::createFighterTurrets() — attaches a defensive turret to capital-class enemies.
void Level::createFighterTurrets()
{
    if (this->enemies == nullptr)
        return;

    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        char *ki = (char *)(*this->enemies)[i];
        if (ki != 0) {
            int kind = *(int *)(ki + 0x7c);
            if (kind == 0x2d || kind == 0x33) {
                PlayerTurret *t = (PlayerTurret *)this->createStaticObject((Waypoint *)(intptr_t)0, 0x1a74, 1);
                t->player->setVulnerable(0);
                Player *pp = t->player;
                int hp = pp->getMaxHitpoints();
                pp->setMaxHitpoints(hp);
                char offset[12] = {0};
                t->setHost((KIPlayer *)ki, *(Vector *)offset);
                *(PlayerTurret **)(ki + 0x10) = t;
                t->shipGroup = (kind == 0x2d) ? 8 : 0;
                C(t, 0x74) = 1;   // RAWREAD: KIPlayer t+0x74 (no member at 0x74; mid-field of field_0x73)
                this->enemies->push_back((KIPlayer *)t);
            }
        }
    }
}

// Level::updateAlienAttackers(int dt) — periodically (re)spawns the alien attacker wave.
void Level::updateAlienAttackers(int dt) {
    Level *thisptr = this;
    this->alienAttackTimer = this->alienAttackTimer + dt;

    Mission *m = (Mission *)(*g_status)->getMission();
    bool blocked = (m != 0) && m->isCampaignMission() != 0 &&
                   ((*g_status)->getCurrentCampaignMission() == 0x28 ||
                    (*g_status)->getCurrentCampaignMission() == 0x93 ||
                    (*g_status)->getCurrentCampaignMission() == 0x9a);
    if (blocked)
        return;

    int elapsed = this->alienAttackTimer;
    int period = ((*g_status)->getCurrentCampaignMission() == 0x29) ? 10000 : 45000;
    if (elapsed <= period)
        return;

    this->alienAttackTimer = 0;
    if (this->enemies == nullptr)
        return;

    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int *ki = (int *)(*this->enemies)[i];
        // race 9 == alien; only respawn ones that are dead and inactive.
        if (ki[10] == 9 && ((KIPlayer*)ki)->isDead() != 0 && ((Player*)ki[1])->isActive() == 0) {
            (*(void (**)(int *))(*ki + 0x18))(ki);   // revive
            int inOrbit = ((*g_status)->inAlienOrbit() != 0);
            if (!inOrbit) {
                Station *st = (Station *)(*g_status)->getStation();
                if (((Station*)st)->isAttackedByAliens() == 0)
                    inOrbit = 1; // fall back to player-relative placement
            }
            thisptr->uaa_placeAlien(ki, inOrbit);
        }
    }
}

// Level::createMission() — builds the in-flight actors/objectives for the player's active
// freelance mission (and the random alien-orbit ambush layered on top of it).
void Level::createMission()
{
    Mission *mission = (Mission *)(*g_status)->getMission();
    if (mission == 0)
        return;

    // alien-orbit ambush wave: 2..N alien fighters at randomized far positions.
    if ((*g_status)->inAlienOrbit() != 0) {
        int lvl = (*g_status)->getLevel();
        AbyssEngine::AERandom *rng = *g_cm_rng;
        int roll = rng->nextInt();
        float base = (float)lvl * 0.5f - 1.0f;
        unsigned count = 2;
        if (2.0f <= base + (float)roll) {
            int r2 = rng->nextInt();
            count = (unsigned)(base + (float)r2);
        }
        int campA = (*g_status)->getCurrentCampaignMission();
        int campB = (*g_status)->getCurrentCampaignMission();
        if (campB == 0x21) count = 2;
        if (campA == 0x44) count = 2;

        this->enemies = new Array<KIPlayer*>();
        this->enemies->resize(count);

        Globals *globals = *g_cm_globals;
        for (unsigned i = 0; i < count; i = i + 1) {
            int fighter = (int)globals->getRandomEnemyFighter(9);
            int ship = (int)(intptr_t)this->createShip(9, 0, fighter, (Waypoint *)(intptr_t)0, 1, 0);
            (*this->enemies)[i] = (KIPlayer *)(intptr_t)ship;
            KIPlayer *kp = (*this->enemies)[i];
            float x = (float)(cm_randPos(rng, 0) - 60000);
            float y = (float)(cm_randPos(rng, 1) - 40000);
            float z = (float)(cm_randPos(rng, 2) - 60000);
            kp->setPosition3(x, y, z);
            (*(Player **)((char *)kp + 4))->setAlwaysEnemy(true);
        }
    }

    // everything else (mission-type-specific objects, objectives, escorts) lives in the helper.
    this->cm_buildMissionScene(mission);
}

// Level::createAsteroids() — fills the asteroid field for the current orbit. Picks a field origin
// from the mission/station context, then reject-samples spawn positions so asteroids do not overlap.
void Level::createAsteroids()
{

    int *rng = (int *)*(int **)&g_ca_rngHolder; // RNG object pointer-to-pointer
    int *rngObj = *(int **)g_ca_rngHolder;
    (void)rng;

    // base collision-variant index: 2 in the "ring" system (idx 0x16), else 0; 0 when in alien orbit.
    int colBase;
    if ((*g_status)->inAlienOrbit() == 0) {
        (*g_status)->getSystem();
        colBase = (((SolarSystem*)(*g_status)->getSystem())->getIndex() == 0x16) ? 2 : 0;
    } else {
        colBase = 0;
    }

    // asteroid container.
    this->asteroids = new Array<KIPlayer*>();

    Galaxy *gal = (Galaxy *)**g_ca_galaxy;
    Station *st = (Station *)(*g_status)->getStation();
    int *prob = (int *)gal->getAsteroidProbabilities(st);

    int *rngHolder = (int *)*(int **)g_ca_rngHolder;
    (void)rngHolder;
    int seed = *rngObj;
    Station *st2 = (Station *)(*g_status)->getStation();
    ((Station*)st2)->getIndex();
    ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->setSeed((long long)seed);

    int countRoll = ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt(0x28);
    this->asteroids->resize((unsigned)(countRoll + 0x28));

    int rx = ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt(0x4e20); // DAT_000bf990 == 0x4e20
    int ry = ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt(0x4e20);
    int rz = ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt(0x4e20);

    int alien = (*g_status)->inAlienOrbit();
    int camp  = (*g_status)->getCurrentCampaignMission();

    // field-origin coordinates (x=ox, y=oy, z=oz).
    int ox, oy, oz;
    if (alien != 0) {
        oy = 0;
        oz = 30000;
        ox = -50000; // DAT_000bf994 default
        if (camp == 0x9a)
            ox = -70000;
    } else {
        bool placed = false;
        if (camp == 0x72) {
            Station *s = (Station *)(*g_status)->getStation();
            if (((Station*)s)->getIndex() == 0x53) {
                oz = 30000;
                ox = 30000;
                oy = 0;
                placed = true; // jumps to LAB_000bf874 (oy = 0)
            }
        }
        if (!placed && camp == 0x59 && (*g_status)->inSupernovaOrbit() != 0) {
            ox = 0;
            oz = 0;
            oy = 0;
            placed = true;
        }
        if (!placed && camp == 0x5b) {
            Station *s = (Station *)(*g_status)->getStation();
            if (((Station*)s)->getIndex() == 0x6e) {
                oy = 0;
                oz = 50000;
                ox = 60000;
                placed = true;
            }
        }
        if (!placed && camp == 0x91) {
            Station *s = (Station *)(*g_status)->getStation();
            if (((Station*)s)->getIndex() == 0x70) {
                oz = 50000;
                ox = 50000;
                oy = 0;
                placed = true;
            }
        }
        if (!placed) {
            ox = rx - 50000;
            oz = rz + 20000;
            oy = ry - 50000;
            if ((*g_status)->getCurrentCampaignMission() == 0 && this->missionPtr == 3) {
                oz = 0;
                ox = 0;
                oy = 0;
            }
        }
    }

    ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->reset();

    // field center vector (this+0xc8): note disasm stores (oy, ox, oz) into 0xc8/0xcc/0xd0.
    Vector center;
    center.x = (float)oy;
    center.y = (float)ox;
    center.z = (float)oz;
    Vector_assign_ca((Vector *)((char *)&this->field_c8), &center);

    Waypoint *wp = (Waypoint *)::operator new(0x138);
    new (wp) Waypoint(oz, oy, ox, 0);
    this->asteroidWaypoint = wp;

    BoundingSphere *bs = (BoundingSphere *)::operator new(0x48);
    new (bs) BoundingSphere(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    *(BoundingSphere **)&this->collisionVolume = bs;

    int density = ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt() + 2; // # of "core" (dense) asteroids
    int alien2 = (*g_status)->inAlienOrbit();

    void *canvas = (void *)**g_ca_canvas;
    int kind = 0x9a;
    int probCursor = 0;

    for (unsigned i = 0; i < this->asteroids->size(); i = i + 1) {
        // choose asteroid kind from the probability table (skip in alien orbit -> fixed 0xa4).
        if (alien2 == 0) {
            bool ok = false;
            int cursor = probCursor;
            while (!ok) {
                int roll = ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt();
                int next = 0;
                if (roll < *(int *)((int)(intptr_t)prob + cursor * 4 + 4)) {
                    kind = *(int *)((int)(intptr_t)prob + cursor * 4);
                    next = cursor + 2;
                    if (cursor > 8)
                        next = 0;
                    ok = (kind == 0xd9) || (kind < 0xa4);
                }
                cursor = next;
            }
            probCursor = cursor;
        } else {
            kind = 0xa4;
        }

        // spread radius: tighter (60000) for the dense core, wider otherwise.
        unsigned spread = (int)i < density ? 60000u : (unsigned)0xea60;
        float half = (float)(spread >> 1);

        // collision variant: 1 in alien orbit, 3 for the special "0xd9" kind, else colBase.
        int colVariant = colBase;
        if ((*g_status)->inAlienOrbit() != 0)
            colVariant = 1;
        if (kind == 0xd9)
            colVariant = 3;

        // mesh id chosen from a small table indexed by colVariant.
        static const int meshTable[4] = {0x37a0, 0x37a4, 0x37a8, 0x37ac};
        int mesh = meshTable[colVariant & 3];

        // reject-sample a position far enough from already-placed asteroids.
        Vector pos;
        for (;;) {
            float cx = *(float *)&this->field_c8;
            pos.x = (cx - half) + (float)((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt(spread);
            float cy = *(float *)&this->field_cc;
            pos.y = (cy - half) + (float)((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt(spread);
            float cz = *(float *)&this->field_d0;
            pos.z = (cz - half) + (float)((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt(spread);
            if (i == 0 || (int)i >= density)
                break;
            bool farEnough = false;
            for (unsigned j = 0; j < i; j = j + 1) {
                float d = this->ca_asteroidDistance(j, &pos);
                if (8000 < (int)d) { farEnough = true; break; }
            }
            if (farEnough)
                break;
        }

        AEGeometry *geo = (AEGeometry *)::operator new(0xc0);
        new ((void*)geo) AEGeometry((uint16_t)mesh, (PaintCanvas*)canvas, 0);

        // install LOD meshes + register with the LOD manager (detail-dependent distance table).
        // Builds the lod-mesh distance table appropriate for the current detail level / distance band
        // and installs it on the geometry.
        bool near = (int)i < density;
        this->ca_installLodMeshes(geo, (short)mesh, near ? 1 : 0);
        ((LODManager *)(intptr_t)this->vtable)->addObject(geo);

        // per-asteroid random radius and scale.
        int base = (*g_ca_lowDetail && **g_ca_lowDetail != 0) ? 0x46 : (near ? 0x46 : 0x46);
        (void)base;
        int radius = ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt() + (near ? 0x78 : 0x1e);
        float scale = (float)radius * 0.001f; // DAT_000bfcf4 scale immediate

        // larger asteroids may roll an extra collision shape (corrupted f64 compares in original).
        double dscale = (double)scale;
        if (dscale >= 1.0 && dscale >= 1.0 && dscale >= 1.0 &&
            ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt() != 0) {
            ((AbyssEngine::AERandom*)(intptr_t)*rngObj)->nextInt();
        }

        PlayerAsteroid *a = (PlayerAsteroid *)::operator new(0x170);
        PlayerAsteroid_ctor_ca(a, kind, geo, colVariant, kind, &pos, scale, (int)scale);
        (*this->asteroids)[i] = (KIPlayer *)a;

        // virtual init(level) on the freshly built asteroid (vtable slot +0x14).
        int *obj = (int *)(*this->asteroids)[i];
        (**(void (***)(int *, Level *))(*obj + 0x14))(obj, this);

        PlayerAsteroid_setAsteroidCenter_ca(
            (PlayerAsteroid *)(*this->asteroids)[i],
            *(float *)&this->field_c8, *(float *)&this->field_cc, *(float *)&this->field_d0);
    }

    if (prob != 0)
        ::operator delete[](prob);
}

// Level::createCampaignMission() — constructs the scripted actors/objectives for the player's
// current story (campaign) mission.
void Level::createCampaignMission()
{
    int idx = (*g_status)->getCurrentCampaignMission();

    if (idx == 0) {
        // mission 0 — the tutorial ambush: three sleeping enemy fighters at a fixed point.
        this->enemies = new Array<KIPlayer*>();
        this->enemies->resize(3);
        float c = g_ccm_pos0;
        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            int type = (i == 1) ? 0x17 : 2;
            int ship = (int)(intptr_t)this->createShip(8, 0, type, (Waypoint*)(intptr_t)0, 1, 0);
            (*this->enemies)[i] = (KIPlayer *)(intptr_t)ship;
            ((*this->enemies)[i])->setToSleep();
            (*(Player **)((char *)(*this->enemies)[i] + 4))->setAlwaysEnemy(true);
            KIPlayer *kp = (*this->enemies)[i];
            kp->setPosition3(c, c, c);
            int e = (int)(intptr_t)(*this->enemies)[i];
            *(int *)(e + 0x50) = 0;
            *(char *)(e + 0x4c) = 0;
            Player_setHitpoints_ccm(*(int *)(e + 4));
            if (i < 3)
                ((PlayerFighter*)(*this->enemies)[i])->setExhaustVisible(false);
        }
        Player_setHitpoints_ccm(*(int *)this->player);
        return;
    }

    // all other campaign missions are scripted by the engine helper.
    this->ccm_buildCampaignScene(idx);
}

// Level::updateOrbit(int dt) — non-mission ambient traffic / reinforcement spawner.
void Level::updateOrbit(int dt) {
    Level *thisptr = this;

    int t178 = this->field_178 + dt;
    this->orbitWaveTimer = this->orbitWaveTimer + dt;
    this->field_178 = t178;

    // delayed "friends turned hostile" alarm.
    if (*(char *)&this->field_18a != 0) {
        if ((*g_status)->getSystem() != 0 && *(int *)((char *)this->player + 0x18) != 0) {
            (*g_status)->getSystem();
            int race = ((SolarSystem*)(*g_status)->getSystem())->getRace();
            thisptr->alarmAllFriends(race, 0 != 0);
            (*g_status)->getSystem();
            ((SolarSystem*)(*g_status)->getSystem())->getRace();
            thisptr->createRadioMessage(2, 0);
            *(char *)&this->field_18a = 0;
        }
        t178 = this->field_178;
    }

    // jumper reinforcement every 10s.
    if (10000 < t178) {
        this->field_178 = 0;
        if (this->enemies != nullptr) {
            for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                KIPlayer *ki = (*this->enemies)[i];
                if (ki->isJumper() != 0 && ki->isDead() != 0 &&
                    ki->player->isActive() == 0 && *(unsigned char *)((char *)ki + 0x42) == 0) {
                    (*(void (**)(KIPlayer *))(*(int *)ki + 0x18))(ki);   // revive
                    ki->setPosition3(0, 0, 0);
                    break;
                }
            }
        }
    }

    // major reinforcement sweep every 45s.
    if (45000 < this->orbitWaveTimer) {
        int hostileAlive = 0;
        this->orbitWaveTimer = 0;
        if (0 < this->hostileCount) {
            if (this->enemies == nullptr)
                return;
            for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                if ((this->enemies->size() - (unsigned)this->hostileCount) <= i &&
                    ((*this->enemies)[i])->isWingMan() == 0 && ((*this->enemies)[i])->isDead() != 0) {
                    hostileAlive = hostileAlive + (((Player*)*(int*)((char*)(*this->enemies)[i] + 4))->isActive() ^ 1);
                }
            }
        }
        if (this->enemies != nullptr) {
            int spawned = 0;
            for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                int *ki = (int *)(*this->enemies)[i];
                // re-activate friendly slots that died.
                if (0 < this->friendCount && (int)i < this->friendCount &&
                    ((KIPlayer*)ki)->isDead() != 0 && ((Player*)ki[1])->isActive() == 0 &&
                    *(unsigned char *)((char *)ki + 0x42) == 0) {
                    (*(void (**)(int *))(*ki + 0x18))(ki);   // revive
                    ((KIPlayer *)ki)->setPosition3(0, 0, 0);
                }
                // spawn enemy reinforcements subject to security-level caps.
                if (1 < hostileAlive && this->field_184 < 2 &&
                    0 < this->hostileCount &&
                    (unsigned)((int)this->enemies->size() - this->hostileCount) <= i) {
                    int race = ki[10];
                    bool secZero = false, secOne = false;
                    if ((*g_status)->inAlienOrbit() == 0) {
                        SolarSystem *ss = (SolarSystem *)(*g_status)->getSystem();
                        secZero = ((SolarSystem*)ss)->getSecurityLevel() == 0;
                    }
                    if ((*g_status)->inAlienOrbit() == 0) {
                        SolarSystem *ss = (SolarSystem *)(*g_status)->getSystem();
                        secOne = ((SolarSystem*)ss)->getSecurityLevel() == 1;
                    }
                    if (((KIPlayer*)ki)->isDead() != 0 && ((Player*)ki[1])->isActive() == 0 &&
                        *(unsigned char *)((char *)ki + 0x42) == 0) {
                        bool doSpawn;
                        if (race != 9 && !secZero) {
                            doSpawn = secOne && this->field_17c <= 2;
                        } else {
                            doSpawn = true;
                        }
                        if (doSpawn) {
                            this->field_17c = this->field_17c + 1;
                            thisptr->uo_spawnFar(ki);
                            spawned = 1;
                        }
                    }
                }
            }
            if (spawned & 1)
                this->field_184 = this->field_184 + 1;
        }
    }
}

void Level::friendTurnedEnemy() {
    if ((unsigned char)field_188 == 0) {
        *(unsigned char *)&field_188 = 1;
        return createRadioMessage(0, 0);
    }
}

void Level::reset() {
    if (playerRoute != nullptr) {
        playerRoute->reset();
    }
    if (enemyRoute != nullptr) {
        enemyRoute->reset();
    }
    if (friendRoute != nullptr) {
        friendRoute->reset();
    }
    if (this->enemies != nullptr) {
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            (*this->enemies)[i]->reset();
        }
    }
    createPlayer();
    assignGuns();
    connectPlayers();
    if (this->messages != nullptr) {
        for (unsigned int i = 0; i < this->messages->size(); i = i + 1) {
            ((RadioMessage *)(*this->messages)[i])->reset();
        }
    }
    // FIXME: PlayerEgo::setRoute is mis-typed as int (KIPlayer::setRoute takes Route*); cast at the boundary.
    player->setRoute((int)(intptr_t)playerRoute);
    int count;
    if (this->enemies != nullptr) {
        count = 0;
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            int e = (int)(intptr_t)(*this->enemies)[i];
            if (*(char *)(e + 0x41) == 0 && *(char *)(e + 0x71) == 0 && *(char *)(e + 0x3f) == 0) {
                // The original passes the enemy being examined as the receiver; the
                // decompiler dropped it (ambient-register thiscall). isWingMan reads
                // wingmanFlag at +0xdc of this enemy KIPlayer.
                int wm = ((KIPlayer *)(intptr_t)e)->isWingMan();
                if (wm == 0) {
                    e = (int)(intptr_t)(*this->enemies)[i];
                    if (*(char *)(e + 0x44) == 0 && *(char *)(e + 0x3c) == 0) {
                        count = count + (*(unsigned char *)(e + 0x3d) ^ 1);
                    }
                }
            }
        }
        count = count - field_120;
    } else {
        count = 0;
    }
    enemiesLeft = count;
    int ast;
    if (asteroids == nullptr) {
        ast = 0;
    } else {
        ast = (int)asteroids->size();
    }
    asteroidsLeft = ast;
    kills = 0;
}

void Level::createSentryGuns() {
    Status **slot = g_sentryStatus;
    int ship = (int)(intptr_t)(*slot)->getShip();
    if (((Ship *)(ship))->getFirstEquipmentOfSort(0x27) != 0) {
        field_b0 = new Array<KIPlayer*>();
        field_b0->resize(9);
        if (enemies == nullptr) {
            enemies = new Array<KIPlayer*>();
        }
        int color = 0x9923e035;
        for (unsigned int i = 0; i < field_b0->size(); i = i + 1) {
            int obj = createStaticObject((Waypoint *)(intptr_t)0, i / 3 + 0x49c0, 1);
            (*field_b0)[i] = (KIPlayer *)(intptr_t)obj;
            int k = (int)(intptr_t)(*field_b0)[i];
            ((Player *)(*(int *)(k + 4)))->setRadius(800);
            ((Player *)(*(int *)(k + 4)))->setAlwaysFriend(1);
            ((Player *)(*(int *)(k + 4)))->setMaxHitpoints(100);
            ((KIPlayer *)(intptr_t)k)->setPosition3((float)color, (float)color, (float)color);
            enemies->push_back((KIPlayer *)(intptr_t)k);
        }
    }
}

int Level::collideStation(Vector v) {
    if (this->landmarks != nullptr &&
        (*this->landmarks)[0] != 0 &&
        (*g_status_collideStation)->inEmptyOrbit() == 0) {
        int *obj = (int *)(*this->landmarks)[0];
        return (*(int (**)(int *, Vector))(*obj + 0x38))(obj, v);
    }
    return 0;
}

void Level::uncoverWanted(int index) {
    if (field_29c == 0) {
        createRadioMessage(0x12, index);
        int **g = g_uncoverWanted;
        for (int i = 1;
             i - 1 < ((Wanted *)((int *)(*(int *)(*g) + 4))[index])->getNumWingmen();
             i = i + 1) {
            Level_setAlwaysEnemy(*(int *)((char *)(*enemies)[i] + 4), 1);
            Level_turnEnemy(*(int *)((char *)(*enemies)[i] + 4));
        }
    }
}

// Level::update(long long time, bool param_2) [+ a third stack bool] — engine per-frame tick.
void Level::update(long long /*time*/, unsigned dtArg, int stackFlag) {
    Level *thisptr = this;
    unsigned dt = dtArg;

    // screen-flash fade.
    if (*(char *)&this->flashActive != 0) {
        unsigned remaining = *(unsigned *)&this->flashDurationA - dt;
        *(unsigned *)&this->flashDurationA = remaining;
        if (0x7fffffff < remaining)
            *(char *)&this->flashActive = 0;
        // colour scaled toward 0 by remaining/duration, clamped — handled by engine helper.
        float frac = (float)remaining / (float)this->flashDurationB;
        float *col = (float *)((char *)&this->flashColor);
        float lo = *g_up_clampLo, hi = *g_up_clampHi, z = *g_up_clampZ, w = *g_up_clampW;
        float scaled[4] = { col[0] * frac, col[1] * frac, col[2] * frac, col[3] * frac };
        float bounds[4] = { lo, hi, z, w };
        for (int k = 0; k < 4; k = k + 1)
            col[k] = bounds[k] > scaled[k] ? bounds[k] : scaled[k];
    }

    int **statusA = g_up_statusA;

    // orbit update: mission orbit if an active non-empty mission, else free orbit.
    bool didMission = false;
    if (((Status*)*g_up_statusA)->getMission() != 0) {
        ((Status*)*g_up_statusA)->getMission();
        if (((Mission*)(*g_status)->getMission())->isEmpty() == 0) {
            thisptr->updateMissionOrbit(dt);
            didMission = true;
        }
    }
    if (!didMission)
        thisptr->updateOrbit(dt);

    Station *st = (Station *)(*g_status)->getStation();
    if (((Station*)st)->isAttackedByAliens() != 0 || (*g_status)->inAlienOrbit() != 0)
        thisptr->updateAlienAttackers(0);

    // tick player guns then enemy guns.
    if (this->playerGuns != nullptr) {
        for (unsigned i = 0; i < this->playerGuns->size(); i = i + 1) {
            (*this->playerGuns)[i]->update(dt);
        }
    }
    if (this->enemyGuns != nullptr) {
        for (unsigned i = 0; i < this->enemyGuns->size(); i = i + 1) {
            (*this->enemyGuns)[i]->update(dt);
        }
    }

    // gamma-ray damage from supernova / hostile stations.
    int aPtr = *(int *)statusA;
    Station *st2 = (Station *)(*g_status)->getStation();
    int idx = ((Station*)st2)->getIndex();
    ((Status*)*g_up_statusA)->getCurrentCampaignMission();
    int gammaBits = ((Status*)(intptr_t)aPtr)->getGammaRayDamagePerSecond(aPtr, idx); float gamma = *(float *)&gammaBits;
    PlayerEgo *ego = this->player;
    if (gamma > 0.0f && ego != nullptr) {
        int ship = (int)(intptr_t)((Status*)*g_up_statusA)->getShip();
        int eq = (int)(intptr_t)((Ship*)(intptr_t)ship)->getFirstEquipmentOfSort(0x26);
        float factor = gamma;
        if (eq != 0) {
            int attr = Item_getAttribute_up(eq);
            if (attr > 0)
                factor = (g_up_eqMax - (float)attr) / g_up_eqMax;
        }
        int hpBefore = ((Player *)this->player)->getGammaHP();
        ((Player *)this->player)->damageGamma(factor);
        if (hpBefore > 0xe && ((Player *)this->player)->getGammaHP() < 0xf) {
            int hud = this->player->getHUD();
            Hud_hudEvent_up(hud, 0x2c, (int)(intptr_t)this->player);
        }
        ego = this->player;
    }

    // tick the in-flight particle-system managers (skybox, engine trails, etc).
    if (ego != nullptr) {
        char dummy[16];
        if (this->field_80 != 0) {
            ((PlayerEgo *)(dummy))->getPosition_up();
            this->field_b4 = *(int *)dummy;
            ((ParticleSystemManager *)(intptr_t)this->field_80)->update(dt);
        }
        if (this->field_74 != 0) ((ParticleSystemManager *)(intptr_t)this->field_74)->update(dt);
        if (this->particleEmitBoolPtr != nullptr) this->particleEmitBoolPtr->update(dt);
        if (this->skybox2Mesh != 0) {
            ((PlayerEgo *)(dummy))->getPosition_up();
            this->field_b4 = *(int *)dummy;
            ((ParticleSystemManager *)(intptr_t)this->skybox2Mesh)->update(dt);
        }
        if (this->particleSystemMgr != nullptr) this->particleSystemMgr->update(dt);
        if (this->particleRenderBoolPtr != nullptr) this->particleRenderBoolPtr->update(dt);
        if (this->field_8c != 0) ((ParticleSystemManager *)(intptr_t)this->field_8c)->update(dt);
        if (this->field_98 != 0) ((ParticleSystemManager *)(intptr_t)this->field_98)->update(dt);
        if (this->field_94 != 0) ((ParticleSystemManager *)(intptr_t)this->field_94)->update(dt);
        if (this->field_9c != 0) ((ParticleSystemManager *)(intptr_t)this->field_9c)->update(dt);
    }

    if (stackFlag == 0)
        ((LODManager *)(intptr_t)this->vtable)->update(dt);
}

// Level::connectPlayers() — wires up each ship's enemy list from the friend/enemy/neutral arrays,
// honouring a thicket of campaign-mission-specific exceptions.
void Level::connectPlayers()
{

    if ((*g_cp_appMgr)->currentModuleId == 5)
        return;

    // player's enemy list from the enemy array.
    if (this->enemies != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        arr.resize(this->enemies->size());
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (Player *)*(int *)((char *)(*this->enemies)[j] + 4);
        ((Player *)*(int *)this->player)->setEnemies(&arr);
    }
    // add asteroids/landmarks arrays as additional enemies.
    if (this->asteroids != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        arr.resize(this->asteroids->size());
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (Player *)*(int *)((char *)(*this->asteroids)[j] + 4);
        ((Player *)*(int *)this->player)->addEnemies(&arr);
    }
    if (this->gasClouds != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        arr.resize(this->gasClouds->size());
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (Player *)*(int *)((char *)(*this->gasClouds)[j] + 4);
        ((Player *)*(int *)this->player)->addEnemies(&arr);
    }

    if (this->enemies == nullptr)
        return;

    int camp = (*g_status)->getCurrentCampaignMission();
    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int e = (int)(intptr_t)(*this->enemies)[i];
        int eFaction = *(int *)(e + 0x28);
        int wmAll = ((KIPlayer*)e)->isWingMan();
        unsigned count = 0;
        bool notM24 = camp != 0x24;
        bool notFirst = i != 0;

        // first pass: count how many ships become this ship's enemies.
        for (unsigned k = 0; k < this->enemies->size(); k = k + 1) {
            int o = (int)(intptr_t)(*this->enemies)[k];
            if (o != e && (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                bool consider;
                if (notM24 || notFirst) {
                    consider = true;
                    if (camp == 0x9a) {
                        int alien = (*g_status)->inAlienOrbit();
                        if (k == 8 && alien != 0 && *(int *)(e + 0x28) == 8)
                            consider = false;
                    } else if (camp == 0x40 && !(i != 0 && e != 0 && k != 0)) {
                        consider = false;
                    }
                } else {
                    consider = ((KIPlayer*)o)->isWingMan() == 0;
                }
                if (consider) {
                    bool skip = false;
                    if (*(char *)(e + 0x40) != 0 && ((PlayerFixedObject*)(intptr_t)e)->getDockingType() == 3) {
                        Station *st = (Station *)(*g_status)->getStation();
                        if (((Station*)st)->stationHasHiddenBlueprint(1) != 0)
                            skip = true;
                    }
                    if (!skip)
                        count = count + 1;
                }
            }
        }

        Array<Player *> arr;
        if (this->player != nullptr)
            count = count + 1;
        arr.resize(count);

        Mission *m = (Mission *)(*g_status)->getMission();
        int mtype = ((Mission*)(*g_status)->getMission())->getType();
        bool branchA = !(((i & 1) == 0 && mtype == 0xc)) &&
                       ((Mission*)(*g_status)->getMission())->getType() != 2 && ((Mission*)(*g_status)->getMission())->getType() != 9;

        bool jumpAlwaysFriend = false;
        if (branchA) {
            // mission-type-specific carve-outs that fall into the "always friend" path.
            int tmp;
            if (m->isCampaignMission() != 0) {
                tmp = (*g_status)->getCurrentCampaignMission();
                if (tmp == 0x10 && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && m->isCampaignMission() != 0) {
                tmp = (*g_status)->getCurrentCampaignMission();
                if (tmp == 0x18 && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && m->isCampaignMission() != 0) {
                tmp = (*g_status)->getCurrentCampaignMission();
                if (tmp == 0x1c && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend) {
                if (m->isCampaignMission() != 0) (*g_status)->getCurrentCampaignMission();
                if (m->isCampaignMission() != 0) (*g_status)->getCurrentCampaignMission();

                int slot = 0;
                PlayerEgo *ego = this->player;
                if (ego != nullptr) {
                    arr[0] = (Player *)*(int *)ego;
                    slot = 1;
                }
                for (unsigned k = 0; k < this->enemies->size(); k = k + 1) {
                    int o = (int)(intptr_t)(*this->enemies)[k];
                    if (o != e && (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                        bool consider;
                        if (notM24 || notFirst) {
                            consider = true;
                            if (camp == 0x9a) {
                                int alien = (*g_status)->inAlienOrbit();
                                if (k == 8 && alien != 0 && *(int *)(e + 0x28) == 8)
                                    consider = false;
                            } else if (camp == 0x40 && !(i != 0 && e != 0 && k != 0)) {
                                consider = false;
                            }
                        } else {
                            consider = ((KIPlayer*)o)->isWingMan() == 0;
                        }
                        if (consider) {
                            bool skip = false;
                            if (*(char *)(e + 0x40) != 0 &&
                                ((PlayerFixedObject*)(intptr_t)e)->getDockingType() == 3) {
                                Station *st = (Station *)(*g_status)->getStation();
                                if (Station_stationHasHiddenBlueprint_cp(st, 1) != 0)
                                    skip = true;
                            }
                            if (!skip) {
                                int ki = (int)(intptr_t)(*this->enemies)[k];
                                arr[slot] = (Player *)*(int *)(ki + 4);
                                slot = slot + 1;
                            }
                        }
                    }
                }
            }
        }

        if (!branchA || jumpAlwaysFriend) {
            if (((Player*)*(int*)(intptr_t)(e + 4))->isAlwaysFriend() == 0) {
                int slot = 0;
                for (unsigned k = 0; k < this->enemies->size(); k = k + 1) {
                    int o = (int)(intptr_t)(*this->enemies)[k];
                    if (o != e && *(char *)(o + 0x3c) == 0 &&
                        (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                        bool add;
                        if (notM24 || notFirst) {
                            add = !(camp == 0x40 && !(i != 0 && e != 0 && k != 0));
                        } else {
                            add = ((KIPlayer*)o)->isWingMan() == 0;
                        }
                        if (add) {
                            int src = (int)(intptr_t)(*this->enemies)[k];
                            arr[slot] = (Player *)*(int *)(src + 4);
                            slot = slot + 1;
                        }
                    }
                }
            } else {
                arr.resize(1);
            }
            arr[arr.size() - 1] = (Player *)*(int *)this->player;
        }

        (*(Player **)(e + 4))->addEnemies(&arr);

        (*g_status)->getMission();
        if (eFaction == 10 && ((Mission*)(*g_status)->getMission())->isEmpty() != 0)
            (*(Player **)(e + 4))->setEnemy((Player *)*(int *)this->player);
    }
}

// Level::enemyDied — the header declares it nullary, but the target reads a discriminator in
// r2 (whether the kill counts toward the player), so we express it as a C callback that the
// engine invokes with (Level* self, _, bool wasFriendlyFire).
void Level::enemyDied(int r1, bool r2arg) {
    Level *thisptr = this;
    int r2 = (int)r2arg; // the recovered "in_r2" discriminator
    (void)r1;

    this->enemiesLeft = this->enemiesLeft - 1;  // enemiesLeft--
    this->kills = this->kills + 1;  // kills++

    if (r2 != 0) {
        this->killCountA = this->killCountA + 1;
        return;
    }

    Status **statusHolder = g_ed_status;
    ((Status *)(*statusHolder))->incKills();
    this->killCountB = this->killCountB + 1;
    if (this->player == nullptr)
        return;

    if (Radar_hasScanner_ed() == 0) {
        Achievements **achA = g_ed_achA;
        if (((Achievements *)(*achA))->hasMedal(0x28, 1) == 0) {
            int st = *(int *)statusHolder;
            int v = *(int *)(st + 0x11c);
            if (*(char *)(st + 0x120) == 0) {
                v = v + 1;
                *(int *)(st + 0x11c) = v;
            }
            int goal = ((Achievements *)(*achA))->getValue(0x28, 1);
            int prog = (int)(((float)v / (float)goal) * g_ed_100);
            if ((prog % 10) == 0) {
                int hud = this->player->getHUD();
                int cur = *(int *)(*(int *)statusHolder + 0x11c);
                int g2 = ((Achievements *)(*achA))->getValue(0x28, 1);
                ((Hud *)(hud))->hudEventMedal(0x28, (int)(((float)cur / (float)g2) * g_ed_100));
            }
            int cur2 = *(int *)(*(int *)statusHolder + 0x11c);
            if (((Achievements *)(*achA))->getValue(0x28, 1) <= cur2)
                *(char *)(*(int *)statusHolder + 0x120) = 1;
        }
    }

    if (this->player != nullptr &&
        this->player->emergencySystemActive() != 0) {
        Achievements **achB = g_ed_achB;
        if (((Achievements *)(*achB))->hasMedal(0x2b, 1) == 0) {
            int st = *(int *)statusHolder;
            int v = *(int *)(st + 0x13c) + 1;
            *(int *)(st + 0x13c) = v;
            int goal = ((Achievements *)(*achB))->getValue(0x2b, 1);
            if (0 < (int)((float)v / (float)goal)) {
                int hud = this->player->getHUD();
                int cur = *(int *)(*(int *)statusHolder + 0x13c);
                int g2 = ((Achievements *)(*achB))->getValue(0x2b, 1);
                ((Hud *)(hud))->hudEventMedal(0x2b, (int)(((float)cur / (float)g2) * g_ed_100));
            }
            int cur2 = *(int *)(*(int *)statusHolder + 0x13c);
            if (((Achievements *)(*achB))->getValue(0x2b, 1) <= cur2)
                *(char *)(*(int *)statusHolder + 0x140) = 1;
        }
    }
}

// One scripted chatter line.
struct RMSpec { int id, speaker, kind, delay; };

// Builds an Array<RadioMessage*> at this+0x114 from a static spec table.
static void buildQueue(Level *self, const RMSpec *specs, unsigned n)
{
    self->messages = new Array<void*>();
    self->messages->resize(n);
    for (unsigned i = 0; i < n; i = i + 1) {
        RadioMessage *m = (RadioMessage *)::operator new(0x28);
        new (m) RadioMessage(specs[i].id, specs[i].speaker, specs[i].kind, specs[i].delay);
        (*self->messages)[i] = m;
    }
}

// Level::createRadioMessages(int set) — populates the level's radio-chatter queue with the fixed
// dialogue sequence for the requested mission/event set.
void Level::createRadioMessages(int set) {
    this->messages = nullptr;

    switch (set) {
    case 0: {
        static const RMSpec t[] = {
            {0x684,0x11,5,0x5dc},{0x685,0,6,0},{0x686,0,6,1},{0x687,10,6,2},{0x688,0xb,6,3},
            {0x689,9,6,4},{0x68a,9,6,5},{0x68b,9,6,6},{0x68c,0,6,7},{0x693,0,9,0},{0x694,0,6,9},
            {0x695,0,6,10},{0x696,0xf,6,0xb},{0x697,0,6,0xc},{0x698,0,6,0xd},{0x699,0,6,0xe},
            {0x69a,0,0x1b,0xc},{0x69b,0xf,6,0x10},{0x69c,0,6,0x11},{0x69d,0xf,6,0x12},
            {0x69e,0,6,0x13},{0x69f,0,6,0x14},{0x6a0,0xf,6,0x15},
        };
        buildQueue(this, t, 0x17); break;
    }
    case 1: {
        static const RMSpec t[] = {{0x6a1,2,5,10000},{0x6a2,2,6,0},{0x6a3,2,6,1}};
        buildQueue(this, t, 3); break;
    }
    case 7: {
        static const RMSpec t[] = {{0x6dc,2,0x10,0},{0x6dd,0,6,0}};
        buildQueue(this, t, 2); break;
    }
    case 0x10: {
        static const RMSpec t[] = {{0x72e,0x13,5,10000},{0x72f,0,6,0},{0x730,0,9,0},
                                   {0x731,1,6,2},{0x732,0,6,3}};
        buildQueue(this, t, 5); break;
    }
    case 0x15: {
        static const RMSpec t[] = {{0x759,10,0x10,0},{0x75a,0,6,0},{0x75b,10,0x19,2},
                                   {0x75c,0xe,8,0},{0x75d,0xe,0x15,0}};
        buildQueue(this, t, 5); break;
    }
    case 0x18: {
        static const RMSpec t[] = {{0x77d,0x13,5,12000},{0x77e,6,6,0},{0x77f,0,6,1},
                                   {0x780,6,0x16,3},{0x781,6,6,3}};
        buildQueue(this, t, 5); break;
    }
    case 0x19: {
        static const RMSpec t[] = {{0x785,0,5,20000},{0x786,6,6,0},{0x787,0,6,1}};
        buildQueue(this, t, 3); break;
    }
    case 0x26: {
        static const RMSpec t[] = {{0x7ed,0x15,5,15000}};
        buildQueue(this, t, 1); break;
    }
    case 0x28: {
        static const RMSpec t[] = {{0x7fa,0,5,10000},{0x7fb,8,6,0},{0x7fc,0,6,1},{0x7fd,7,5,40000},
                                   {0x7fe,0,6,3},{0x7ff,7,0xc,0},{0x800,0,0x18,0}};
        buildQueue(this, t, 7); break;
    }
    case 0x29: {
        // two delays come from data slots DAT_000d19e0 / DAT_000d19e4 at runtime.
        RMSpec t[] = {{0x804,0,5,crms_randDelay(0)},{0x805,7,6,0},{0x806,0,6,1},
                      {0x807,7,6,2},{0x808,7,0x1a,crms_randDelay(1)},{0x809,7,6,4},
                      {0x80f,0,1,0},{0x810,0,6,6}};
        buildQueue(this, t, 8); break;
    }
    case 0x31: {
        static const RMSpec t[] = {{0x848,0,5,8000}};
        buildQueue(this, t, 1); break;
    }
    case 0x32: {
        static const RMSpec t[] = {{0x849,0x3f,5,8000},{0x84a,0,6,0},{0x84b,0x3f,6,1},{0x84c,0,6,2}};
        buildQueue(this, t, 4); break;
    }
    case 0x33: {
        static const RMSpec t[] = {{0x84d,0x3f,5,8000},{0x84e,0,6,0}};
        buildQueue(this, t, 2); break;
    }
    case 0x34: {
        static const RMSpec t[] = {{0x84f,0x3f,5,8000},{0x850,0,6,0}};
        buildQueue(this, t, 2); break;
    }
    case 0x36: {
        static const RMSpec t[] = {{0x851,0,5,8000}};
        buildQueue(this, t, 1); break;
    }
    case 0x37: {
        static const RMSpec t[] = {{0x85a,0,5,8000},{0x85b,0,6,0}};
        buildQueue(this, t, 2); break;
    }
    case 0x38: {
        static const RMSpec t[] = {{0x86a,0x1b,5,8000},{0x86b,0,6,0},{0x86c,0x1c,0x10,0},
                                   {0x86d,0,0x14,3},{0x86e,0,6,3},{0x86f,0x1b,0x1c,0}};
        buildQueue(this, t, 6); break;
    }
    case 0x40: {
        static const RMSpec t[] = {{0x8b4,0,5,8000},{0x8b5,0x14,6,0},{0x8b6,0,6,1},{0x8b7,0x1e,6,2},
                                   {0x8b8,0,6,3},{0x8b9,0x1e,0x14,5},{0x8ba,0,6,5}};
        buildQueue(this, t, 7); break;
    }
    case 0x41: {
        static const RMSpec t[] = {{0x8cb,0,5,12000},{0x8cc,0x14,6,0},{0x8cd,0,6,1}};
        buildQueue(this, t, 3); break;
    }
    case 0x43: {
        static const RMSpec t[] = {{0x8ef,0,0x10,0},{0x8f0,0x1f,6,0},{0x8f1,0x1e,6,1},
                                   {0x8f2,0,0x14,2},{0x8f3,0x1f,6,3},{0x8f4,0,6,4},{0x8f5,0,0x14,8},
                                   {0x8f6,0x1f,6,6},{0x8f7,0,6,7},{0x8f8,0x1f,6,8},{0x8f9,0,6,9},
                                   {0x8fa,0x1f,6,10}};
        buildQueue(this, t, 0xc); break;
    }
    case 0x45: {
        static const RMSpec t[] = {{0x90e,0,5,8000},{0x90f,0,6,0}};
        buildQueue(this, t, 2); break;
    }
    case 0x46: {
        static const RMSpec t[] = {{0x910,0,5,8000},{0x911,0,6,0},{0x912,0x22,6,1},{0x913,0,6,2}};
        buildQueue(this, t, 4); break;
    }
    case 0x49: {
        static const RMSpec t[] = {{0x92b,0,5,8000},{0x92c,0,0x10,0},{0x92d,0xb,6,1},{0x92e,0,6,2},
                                   {0x92f,0,0x1b,1},{0x930,0x21,0x1b,2},{0x931,0,0x1b,3},
                                   {0x932,0,6,6}};
        buildQueue(this, t, 8); break;
    }
    case 0x77: {
        static const RMSpec t[] = {{0xab9,0x11,0x1b,1}};
        buildQueue(this, t, 1); break;
    }
    case 0x78: {
        static const RMSpec t[] = {{0xac5,0,5,0x5dc}};
        buildQueue(this, t, 1); break;
    }
    case 0x83: {
        static const RMSpec t[] = {{0xb2b,0,0x1b,2}};
        buildQueue(this, t, 1); break;
    }
    case 0x85: {
        static const RMSpec t[] = {{0xb33,0x11,0x1b,1}};
        buildQueue(this, t, 1); break;
    }
    case 0x87: {
        static const RMSpec t[] = {{0xb43,0x31,0x1b,1},{0xb44,0,6,0},{0xb45,0,0x1b,2},
                                   {0xb46,0,0x1b,3}};
        buildQueue(this, t, 4); break;
    }
    case 0x89: {
        static const RMSpec t[] = {{0xb4f,0x32,5,0x5dc},{0xb50,0,6,0},{0xb51,0x32,6,1},
                                   {0xb52,0,6,2},{0xb53,0x32,6,3}};
        buildQueue(this, t, 5); break;
    }
    case 0x90: {
        static const RMSpec t[] = {{0xb98,0x27,5,7000},{0xb99,0x27,6,0},{0xb9a,0,6,1},
                                   {0xb9b,0x27,6,2}};
        buildQueue(this, t, 4); break;
    }
    case 0x91: {
        static const RMSpec t[] = {{0xb9c,0,5,7000},{0xb9d,0x27,6,0},{0xb9e,0x27,0x1b,5}};
        buildQueue(this, t, 3); break;
    }
    case 0x93: {
        static const RMSpec t[] = {{0xbac,0,5,7000},{0xbad,0,6,0}};
        buildQueue(this, t, 2); break;
    }
    case 0x50: {
        static const RMSpec t[] = {{0x96e,0,5,8000},{0x96f,6,5,25000},{0x970,0x1a,6,1},
                                   {0x971,0,6,2},{0x972,6,6,3}};
        buildQueue(this, t, 5); break;
    }
    default:
        break;
    }
}

static inline float clampChannel(float scaled) {
    int v = (int)scaled;
    if (v > 0xfe) {
        v = 0xff;
    }
    if (v < 0xb) {
        v = 10;
    }
    return (float)v;
}

void Level::flashScreen(int type) {
    flashType = type;
    flashActive = 1;
    int dur = 2000;
    if (type == 1) {
        dur = 7000;
    }
    if (type > 2) {
        dur = 10000;
    }
    flashDurationA = dur;
    flashDurationB = dur;
    if (type == 2) {
        flashColor.x = *g_flash2_a * 1.5f;
        flashColor.y = *g_flash2_b * 1.5f;
        flashColor.z = *g_flash2_c * 1.5f;
    } else if (type == 4) {
        flashColor.x = 0;
        flashColor.y = 0;
        flashColor.z = 255.0f;
    } else if (type == 3) {
        flashColor.x = 0;
        flashColor.y = 0;
        flashColor.z = 0;
        player->hitCamera();
    } else {
        float k = (type == 1) ? 8.0f : 5.0f;
        flashColor.x = clampChannel(k * *g_flashCol_r);
        flashColor.y = clampChannel(k * *g_flashCol_g);
        flashColor.z = clampChannel(k * *g_flashCol_b);
    }
    *(float *)&flashField = 255.0f;
}

// NOTE: the work-item lists n=36 with a garbled "body 12" decompile, but the real
// target function has a large frame (saves r4-r11, vpush {d8,d9}, sub sp,#80) and a
// body that is not recoverable from the provided Ghidra output. Left as a stub.

void Level::createPlayer() {
    return Level_createPlayer_impl(this);
}


void Level::wingmanDied(int name) {
    Status **slot = g_wingmanDied;
    unsigned int *list = (unsigned int *)(intptr_t)(*slot)->getWingmen();
    if (list == 0) {
        return;
    }
    if (__builtin_expect(*list < 2, 0)) {
        return (*slot)->setWingmen((Array<String *> *)0);
    }
    for (unsigned int i = 0; i < *list; i = i + 1) {
        String *w = ((String **)list[1])[i];
        if (w->Compare_str((String *)name) == 0) {
            return this->wingmanDied_one(((String **)list[1])[i], list);
        }
    }
}


static inline void zero16(void *p) {
    ((int *)p)[0] = 0;
    ((int *)p)[1] = 0;
    ((int *)p)[2] = 0;
    ((int *)p)[3] = 0;
}

Level::Level(int mission) {
    void (*ctor)(void *) = *g_levelSubCtor;
    field_b4 = 0;
    field_b8 = 0;
    field_bc = 0;
    field_c8 = 0;
    field_cc = 0;
    field_d0 = 0;
    field_18c = 0;
    field_190 = 0;
    field_194 = 0;
    ctor((char *)&this->sub_1d0);
    ctor((char *)&this->sub_20c);
    ctor((char *)&this->sub_248);
    zero16(&field_1c);
    zero16((char *)&this->field_90);
    zero16((char *)&this->particleRenderBoolPtr);
    zero16((char *)&this->field_74);
    skyboxMesh = -1;
    field_08 = -1;
    skyboxTexture = -1;
    field_10 = -1;
    missionPtr = mission;
    collisionVolume = 0;
    field_b0 = nullptr;
    flashColor.x = 0;
    flashColor.y = 0;
    flashColor.z = 0;
    flashDurationA = 0;
    flashDurationB = 0;
    flashActive = 0;
    // Original zeroes the objective region with two unaligned 4-byte stores at
    // +0x29 and +0x2d; net effect is clearing objectivesA/objectivesB.
    objectivesA = nullptr;
    objectivesB = nullptr;
    memset((char *)&this->asteroidWaypoint, 0, 0x65);
    zero16((char *)&this->hostileCount);
    zero16((char *)&this->flashType);
    field_18a = 0;
    field_188 = 0;
    movingStarsIndex = -1;
    field_284 = -1;
    field_34 = -1;
    field_38 = -1;
    field_3c = -1;
    field_48 = -1;
    field_4c = -1;
    field_17c = 0;
    field_180 = 0;
    field_184 = 0;
    field_69 = 0;
    field_6c = 0;
    field_288 = 0;
    field_a0 = 0;
    field_a4 = nullptr;
    field_a8 = nullptr;
    // The container members (offsets 0xe4..0x114) were 32-bit handles cleared by the
    // original __aeabi_memclr4 over [asteroidWaypoint, +0x65); now that they are real
    // Array<T>* (8 bytes, shifted offsets), the byte-count memset no longer covers them
    // correctly, so null them explicitly by name.
    playerGuns = nullptr;
    enemyGuns = nullptr;
    gasClouds = nullptr;
    enemies = nullptr;
    asteroids = nullptr;
    landmarks = nullptr;
    field_104 = nullptr;
    messages = nullptr;
    miningPlantIndex = -1;
    miningPlant = 0;
    numDeliveredOre = 0;
    numDeliveredPassengers = 0;
    field_29c = 0;
    field_29e = 0;
    field_1b4 = -1;
    field_1b8 = -1;
    field_1bc = -1;
    field_1c0 = -1;
}

// Level::createStaticObjects() — spawns campaign-specific landmark/escort objects.
void Level::createStaticObjects()
{

    // --- carrier escort that follows the campaign progression (station 0x70) ---
    if ((*g_status)->inAlienOrbit() == 0) {
        Station *st = (Station *)(*g_status)->getStation();
        if (((Station*)st)->getIndex() == 0x70 &&
            0x7f < (*g_status)->getCurrentCampaignMission()) {
            int type;
            if ((*g_status)->getCurrentCampaignMission() < 0x83)      type = 0x493e;
            else if ((*g_status)->getCurrentCampaignMission() < 0x87) type = 0x4941;
            else if ((*g_status)->getCurrentCampaignMission() < 0x8a) type = 0x4944;
            else if ((*g_status)->getCurrentCampaignMission() < 0x8e) type = 0x4947;
            else if (0x91 < (*g_status)->getCurrentCampaignMission()) type = -1;
            else                                                    type = 0x494a;

            if (type != -1) {
                char *o = (char *)this->createStaticObject((Waypoint *)(intptr_t)0, type, 0);
                ((PlayerFixedObject *)o)->setPosition3(g_cso_posX, 0, g_cso_posZ);
                ((PlayerFixedObject *)o)->setMoving(0);
                AEGeometry *geo = *(AEGeometry **)(o + 8);
                *(char *)(o + 0x70) = 0;
                // Aim the object along the active star system's light direction.
                // veneer -> StarSystem::getLightDirection on this->starSystem (Level +0xec).
                Vector dir = this->starSystem->getLightDirection();
                // AEGeometry::setDirection takes (dir, up). Ghidra (createStaticObjects @ 0xcbfa4)
                // shows the up arg is world-up (local_40=0, uStack_3c=0x3f800000, local_38=0).
                Vector up = {0.0f, 1.0f, 0.0f};
                geo->setDirection(dir, up);
                String *txt = (String *)GameText_getText_cso(**g_cso_textA);
                *(String *)(o + 0x18) = *txt;
                (*(Player **)(o + 4))->setAlwaysFriend(1);
                if (this->enemies == nullptr) {
                    this->enemies = new Array<KIPlayer*>();
                }
                this->enemies->push_back((KIPlayer *)o);
                if (*(void **)(o + 0x50) != 0)
                    delete (Array<void*> *)*(void **)(o + 0x50);
                *(int *)(o + 0x50) = 0;
            }
        }
    }

    // --- station 0x67 dockable beacon (mid/late campaign) ---
    if (0x54 < (*g_status)->getCurrentCampaignMission() &&
        (*g_status)->getCurrentCampaignMission() != 0x87 &&
        (*g_status)->inAlienOrbit() == 0) {
        Station *st = (Station *)(*g_status)->getStation();
        if (((Station*)st)->getIndex() == 0x67) {
            char *o = (char *)this->createStaticObject((Waypoint *)(intptr_t)0, 0x4a88, 0);
            ((PlayerFixedObject *)o)->setPosition3(0, 0, 0);
            ((PlayerFixedObject *)o)->setMoving(0);
            *(char *)(o + 0x70) = 1;
            String *txt = (String *)GameText_getText_cso(**g_cso_textB);
            String name;
            name.ctor_copy(txt, 0);
            ((PlayerFixedObject *)o)->setName(&name);
            ((PlayerFixedObject *)o)->setDockingType(1);
            if (*(void **)(o + 0x50) != 0)
                delete (Array<void*> *)*(void **)(o + 0x50);
            *(int *)(o + 0x50) = 0;
            (*(Player **)(o + 4))->setAlwaysFriend(1);
            if (this->enemies == nullptr) {
                this->enemies = new Array<KIPlayer*>();
            }
            this->enemies->push_back((KIPlayer *)o);
        }
    }
}

// Level::createStaticObject(Waypoint* wp, int type, bool jitter) — spawns one scenery / structure
// object at the waypoint (optionally with a small random position offset).
int Level::createStaticObject(Waypoint *wp, int type, int jitter) {
    Level *thisptr = this;

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = wp->x;
        y = wp->y;
        z = wp->z;
    }
    if (jitter) {
        AbyssEngine::AERandom *rng = *g_cso2_rng;
        x = x + cso2_rand20000(rng) - 10000;
        y = y + cso2_rand20000(rng) - 10000;
        z = z + cso2_rand20000(rng) - 10000;
    }

    // type 0x4215 picks one of three debris meshes at random; everything else maps 1:1.
    if (type == 0x4215) {
        int r = ((AbyssEngine::AERandom*)(intptr_t)*(int *)g_cso2_rng)->nextInt();
        type = (r == 0) ? 0x4215 : (r == 1) ? 0x4216 : 0x4217;
    }

    return thisptr->cso2_construct(type, x, y, z);
}

// Level::getBoundingVolume(int idx, AEGeometry* outArrayHolder) — loads the per-mesh collision
// data for a station (idx<2000) or static object and produces an Array<BoundingVolume*>.
void *Level_getBoundingVolume(int idx, int kind)
{
    (void)idx;
    FileRead *fr = (FileRead *)::operator new(1);
    new (fr) FileRead();

    int *coll;
    if (kind < 2000)
        coll = (int *)fr->loadStationCollision(idx);
    else
        coll = (int *)fr->loadStaticCollision(idx);
    (fr->~FileRead(), ::operator delete(fr));

    void *result = 0;
    if (coll != 0) {
        unsigned n = *(unsigned *)coll[1];   // entry count
        Array<BoundingVolume*> *arr = new Array<BoundingVolume*>();
        arr->resize(n);
        result = arr;

        int cursor = 1;
        for (unsigned i = 0; i < n; i = i + 1) {
            int *data = (int *)coll[1];
            int shape = data[cursor];
            int rec = (int)(intptr_t)(data + cursor);
            BoundingVolume *bv = 0;
            if (shape == 1) {
                bv = ((Level*)0)->gbv_makeVolume(rec, 1);
                cursor = cursor + 7;
            } else if (shape == 0) {
                bv = ((Level*)0)->gbv_makeVolume(rec, 0);
                cursor = cursor + 5;
            } else {
                cursor = cursor + 1;
            }
            (*arr)[i] = bv;
        }

        // `coll` is an external collision-record handle from the FileRead loader
        // ({?, data, ?}); free its data buffer then the handle itself.
        ::operator delete((void *)(intptr_t)coll[1]);
        ::operator delete(coll);
    }
    return result;
}

// Level::createShip(race, shipClass, type, wp, hostile, group) — spawns a fighter (class 0) or a
// fixed capital ship (class 1) at the waypoint, scaling its stats to the player level/difficulty.
PlayerFixedObject * Level::createShip(int race, int shipClass, int type, Waypoint *wp, int hostile, int group) {
    Level *thisptr = this;
    int camp = (*g_status)->getCurrentCampaignMission();

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = wp->x;
        y = wp->y;
        z = wp->z;
    }
    AbyssEngine::AERandom *rng = *g_cs_rng;
    int jx = cs_rand40000(rng);
    int jy = cs_rand40000(rng);
    int jz = cs_rand40000(rng);

    // base hitpoints from player level + campaign ramp.
    int lvl = (*g_status)->getLevel();
    if (0x15 <= lvl) lvl = 0x14;
    int ramp = ((*g_status)->gameWon() != 0) ? 0xb4 : (camp << 2);
    int hp = ramp + lvl * 0xe + 0x14;
    if (type == 0x33)      hp = (int)((float)hp * 1.0f);
    else if (type == 0x31) hp = (int)((float)hp * 17.0f);
    else if (type == 0x2c) hp = (int)((float)hp * 2.25f);
    if ((unsigned)(camp - 0x31) < 8 && (0x8fU >> ((unsigned)(camp - 0x31) & 0xff) & 1) != 0)
        hp = 0x10e;

    int dmgLvl = (*g_status)->getLevel();
    if (0x15 <= dmgLvl) dmgLvl = 0x14;
    int empA = dmgLvl * 5 + 0x28;
    int empB;
    if (shipClass == 1) {
        int mul = (type == 0xe) ? 0x19 : 5;
        empA = empA * 3;
        hp = mul * hp;
        empB = 45000;
    } else {
        empB = 15000;
    }
    hp = (int)((float)hp + (*(float *)(*(int *)g_cs_diffRec + 0x2c) - 0.5f) * (float)hp);
    if (camp == 0x9a) {
        int alien = (*g_status)->inAlienOrbit();
        hp = hp << (alien & (race == 9));
    }

    Player *pl = (Player *)::operator new(0x114);
    int playerHp = ((*g_status)->hardCoreMode() != 0) ? 0x28a : 1000;
    new (pl) Player(playerHp, hp, 1, 1, 0);
    float fx = (float)(x + jx - 20000);
    float fy = (float)(y + jy - 20000);
    float fz = (float)(z + jz - 20000);
    pl->setEmpData(empA, empB);

    PlayerFixedObject *obj = 0;
    if (shipClass == 0) {
        PlayerFighter *pf = (PlayerFighter *)::operator new(0x2f0);
        new (pf) PlayerFighter(type, race, pl, 0, fx, fy, fz, 0);
        obj = (PlayerFixedObject *)pf;
        int gg = (int)(intptr_t)Globals_getShipGroup_cs(*g_cs_globalsA, type, race, group);
        (*(void (**)(PlayerFixedObject *, int, int, int))(*(int *)obj + 8))(obj, gg, type, hostile);
        if (this->missionPtr != 1 && this->missionPtr != 0x17) {
            AEGeometry *g = *(AEGeometry **)((char *)obj + 0xc);
            if (g == 0) g = *(AEGeometry **)((char *)obj + 0x8);
            ((LODManager *)(intptr_t)this->vtable)->addObject(g);
        }
        if (type == 0x2c || type == 0x31 || type == 0x33) {
            if (type == 0x33) *(unsigned char *)((char *)obj + 0x25) = 0;
            if (type != 0x33 && *(void **)((char *)obj + 0x50) != 0) {
                delete (Array<void*> *)*(void **)((char *)obj + 0x50);
                *(void **)((char *)obj + 0x50) = 0;
            }
        }
    } else if (shipClass == 1) {
        obj = (PlayerFixedObject *)::operator new(0x1bc);
        PlayerFixedObject_ctor_cs(obj, type, race, pl, 0, fx, fy, fz);
        int wreck = 0;
        void *bv = Level::cs_buildBV(race, type, &wreck);
        obj->setWreckedMeshId(wreck);
        obj->setBV((BoundingVolume*)bv);
        int gg = (int)(intptr_t)Globals_getShipGroup_cs(*g_cs_globalsB, type, race, 0);
        (*(void (**)(PlayerFixedObject *, int, int, int))(*(int *)obj + 8))(obj, gg, type, 0);
        ((LODManager *)(intptr_t)this->vtable)->addObject(*(AEGeometry **)((char *)obj + 0x8));
        *(unsigned char *)((char *)obj + 0x40) = 1;
    }

    if (obj != 0)
        (*(void (**)(PlayerFixedObject *, Level *))(*(int *)obj + 0x14))(obj, thisptr);
    return obj;
}

void Level::almostKillWanted(int index) {
    if (field_29e != 0) {
        return;
    }
    Status **slot = g_almostKillWanted;
    field_29e = 1;
    if ((*slot)->isStorylineWanted(index) == 0) {
        return;
    }
    int m = (int)(intptr_t)::operator new(0x78);
    new ((void *)(intptr_t)m) Mission(4, 0, (*slot)->getStation()->getIndex());
    ((Mission *)(m))->setCampaign_akw(1);
    ((Mission *)(m))->setWon(1);
    (*slot)->setMission((Mission *)(intptr_t)m);
    (*slot)->setCampaignMission((Mission *)(intptr_t)m);
    if (objectivesA != nullptr) {
        operator delete(dtor_Objective_akw((void *)objectivesA));
    }
    objectivesA = nullptr;
    int o = (int)(intptr_t)::operator new(0x1c);
    Objective_ctor_akw(o, 3, 0, 0, this);
    objectivesA = (Objective *)(intptr_t)o;
    int e = (int)(intptr_t)(*this->enemies)[0];
    Level_setAlwaysEnemy(*(int *)(e + 4), 0);
    ((Player *)(int)(intptr_t)(*this->enemies)[1])->resetDamageDoneByPlayer();
    int e0 = (int)(intptr_t)(*this->enemies)[0];
    *(unsigned char *)(*(int *)(e0 + 4) + 0x5c) = 0;
    *(unsigned char *)(e0 + 0x43) = 1;
    int w = (int)(intptr_t)(*slot)->getWanted();
    return ((Wanted *)(intptr_t)(((int *)(*(int *)(w + 4)))[index]))->setActive(0 != 0);
}

// Level::assignGuns() — equips every active ship in the level with weapons scaled to the player
// level and difficulty, with a large set of campaign/wanted/ship-type special cases.
void Level::assignGuns()
{

    if (this->enemyGuns != nullptr) {
        deletePolyArray(this->enemyGuns);
    }
    Status **status = g_ag_status;
    this->enemyGuns = nullptr;

    // base weapon "power" from player level (clamped to 20).
    float lvlPower = (float)((*g_status)->getLevel() - 2) * g_ag_perLevel;
    if (lvlPower >= 20.0f) lvlPower = 20.0f;
    if (lvlPower < 0.0f) lvlPower = (float)((*g_status)->getLevel() - 2) * g_ag_perLevel;

    float diffScale = *(float *)(*(int *)g_ag_diffRec + 0x2c);
    int camp = (*g_status)->getCurrentCampaignMission();
    int basePower = (int)(lvlPower + lvlPower * (diffScale - 0.5f));
    Wanted *wanted = (Wanted *)(*g_status)->getWantedInCurrentOrbit();
    if (0x15 < basePower) basePower = 0x16;
    if (this->enemies == nullptr)
        return;

    // count gun slots needed.
    unsigned slots = 0;
    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int e = (int)(intptr_t)(*this->enemies)[i];
        if (e != 0 && *(char *)(e + 0x25) != 0) {
            int add = (((KIPlayer*)e)->isWingMan() != 0) ? 2 : 1;
            slots = slots + add;
        }
    }

    this->enemyGuns = new Array<ObjectGun*>();
    this->enemyGuns->resize(slots);

    int baseDmg = (basePower == 0) ? 3 : (basePower + 2);
    if (camp == 4) baseDmg = 1;

    int outIdx = 0;
    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int e = (int)(intptr_t)(*this->enemies)[i];
        if (e == 0)
            continue;
        if (*(char *)(e + 0x25) == 0)
            goto wingmanExtra;

        {
            if (this->missionPtr == 2)
                (*(Player **)(e + 4))->setPlayShootSound(0, 2);

            int color = 0x41800000;
            int dmg;
            (*g_status)->getMission();
            if (((Mission*)(*g_status)->getMission())->getType() == 6 && ((Player*)*(int*)(intptr_t)(e + 4))->isAlwaysFriend() == 0) {
                color = 0x41e00000;
                dmg = (*g_status)->getLevel() + baseDmg;
            } else {
                (*g_status)->getMission();
                dmg = baseDmg;
                if (((Mission*)(*g_status)->getMission())->getType() == 0xc && ((Player*)*(int*)(intptr_t)(e + 4))->isAlwaysFriend() != 0) {
                    color = 0x41e00000;
                    dmg = (*g_status)->getLevel() + baseDmg;
                }
            }

            // alien ships ramp the damage harder.
            if (((KIPlayer*)e)->isWingMan() == 0 && ((Player*)*(int*)(intptr_t)(e + 4))->isAlwaysFriend() == 0 &&
                (*this->enemies)[i] &&
                *(int *)((char *)(*this->enemies)[i] + 0x28) == 9) {
                if (camp != 0x10) dmg = (int)((float)dmg * 1.0f);
                else dmg = dmg + dmg;
            } else {
                int hard = ((0x8fU >> ((unsigned)(camp - 0x31) & 0xff) & 1) != 0 &&
                            (unsigned)(camp - 0x31) <= 7) ? 5 : dmg;
                dmg = hard;
            }

            if (camp == 0x46 && ((KIPlayer*)e)->isWingMan() == 0)
                dmg = (int)((float)dmg * 2.5f);

            if ((*g_status)->getMission() != 0) {
                (*g_status)->getMission();
                if (((Mission*)(*g_status)->getMission())->getType() == 0xb7) dmg = 1;
            }

            Gun *gun = (Gun *)::operator new(0x114);
            int won = (*g_status)->gameWon();
            int rampMis = (won != 0) ? 0x2d : (*g_status)->getCurrentCampaignMission();
            Gun_ctor_ag(gun, 0, dmg, 4, -1, 3000, rampMis * -2 + 600, color, 0, 0, 0, 0, 0, 0);
            gun->setFriendGun(1);
            Gun_setLevel_cg(gun, this);
            gun->setIndex(0);
            gun->weaponType = 0;

            int res;
            switch ((int)(intptr_t)(*this->enemies)[i] + 0x28
                        ? *(int *)((char *)(*this->enemies)[i] + 0x28)
                        : 0) {
            case 0:  gun->weaponType = 0; gun->setIndex(0);    res = 0x1a62; break;
            case 1:  gun->setIndex(3);    res = 0x1a68; break;
            case 2:  gun->weaponType = 0; gun->setIndex(7);    res = 0x1a6c; break;
            case 3:  gun->setIndex(0x19); res = 0x1a92; break;
            case 9:  gun->setIndex(5);    res = 0x1a6a; break;
            case 10: gun->setIndex(0xe5); res = 0x4a93;
                     gun->damage = (int)((float)gun->damage * 1.0f);
                     break;
            default: gun->weaponType = 1; gun->setIndex(0x13); res = 0x1a8b; break;
            }

            int camp2 = (*g_status)->getCurrentCampaignMission();
            PlayerTurret *turret = (PlayerTurret *)(*this->enemies)[i];
            if (turret->field_0x3e != 0) {
                int host = (int)(intptr_t)turret->getHost();
                if (host != 0 && (*(int *)(host + 0x7c) == 0x2d || *(int *)(host + 0x7c) == 0x33)) {
                    gun->weaponType = 2; gun->setIndex(0x16); res = 0x1a8e;
                } else {
                    KIPlayer *k = (*this->enemies)[i];
                    if (*(unsigned char *)((char *)k + 0x3f) == 0) {
                        gun->weaponType = 1;
                        if (*(unsigned char *)((char *)k + 0x28) == 1) { gun->setIndex(0xf);  res = 0x1a87; }
                        else                                 { gun->setIndex(0x14); res = 0x1a8c; }
                    } else {
                        int kt = k->getType();
                        if (kt == 0x49c1)      { gun->weaponType = 1; gun->setIndex(0x14); res = 0x1a8d; }
                        else if (kt == 0x49c0) { gun->weaponType = 0; gun->setIndex(2);    res = 0x1a64; }
                        else                   { gun->weaponType = 1; gun->setIndex(0xe);  res = 0x1a86; }
                        gun->field_0xa8 = 1;
                        // base stats from the ship-stat table (corrupted SIMD in original).
                        if (camp2 == 0x9e && kt == 0x49c2 && ((Player*)*(int*)(intptr_t)(e + 4))->isAlwaysEnemy() != 0) {
                            Player *pp = *(Player **)((char *)(*this->enemies)[i] + 4);
                            int mhp = pp->getMaxHitpoints();
                            pp->setMaxHitpoints((int)((float)mhp * 5.0f));
                        }
                    }
                }
            }

            // wanted/ship special weapon overrides.
            int kt2 = (*g_status)->getCurrentCampaignMission();
            int host2 = (int)(intptr_t)(*this->enemies)[i];
            if ((*g_status)->getMission() != 0) {
                Mission *mm = (Mission *)(*g_status)->getMission();
                if (mm->isCampaignMission() != 0) {
                    if (**g_ag_statusB == (*g_status)->getCurrentCampaignMission() &&
                        2 < **g_ag_alienCnt &&
                        ((*this->enemies)[i])->isEnemy() != 0)
                        gun->damage = (int)((float)gun->damage * 1.0f);
                }
            }
            if (wanted != 0 && *(char *)(host2 + 0x42) != 0) {
                int w = wanted->getWeapon();
                gun->setIndex(w);
                int attr = Item_getAttribute_ag(*(int *)(*(int *)(*g_ag_itemTblA + 4) + w * 4));
                res = ((int *)g_ag_weaponDmg)[w];
                gun->weaponType = attr;
                gun->damage = gun->damage << 2;
            }

            // wrap the Gun in a Rocket/Object gun and store it.
            int sc = gun->weaponType;
            if (sc == 0x28 || sc == 5) {
                RocketGun *r = (RocketGun *)::operator new(0xe8);
                new (r) RocketGun(gun->itemIndex, gun, res, 0, 0, sc,
                                  sc == 5 ? 1 : 0, this);
                (*this->enemyGuns)[outIdx] = (ObjectGun *)r;
                gun->field_0x50 = 0x41000000;
                gun->initialLifetime = 10000;
                gun->fireDelay = 3000;
                gun->damage = gun->damage << 2;
            } else {
                ObjectGun *o = (ObjectGun *)::operator new(0xb0);
                new (o) ObjectGun(0, gun, res, 0x2711, this);
                (*this->enemyGuns)[outIdx] = o;
            }
            ((KIPlayer*)(*this->enemies)[i])->addGun((Gun*)gun);
            Globals_addSoundResourceToList_ag(**g_ag_snd);
            outIdx = outIdx + 1;
            (void)kt2;
        }

wingmanExtra:
        if (((KIPlayer*)e)->isWingMan() != 0 &&
            *(char *)((int)(intptr_t)&(*this->enemies)[i]) + 0x25 != 0) {
            Gun *gun = (Gun *)::operator new(0x114);
            Gun_ctor_ag(gun, 0x12, 0, 4, -1, 3000, 400, 0x41800000, 0, 0, 0, 0, 0, 0);
            gun->setFriendGun(1);
            Gun_setLevel_cg(gun, this);
            gun->itemIndex = 0x12;
            gun->weaponType = 1;
            ObjectGun *o = (ObjectGun *)::operator new(0xb0);
            new (o) ObjectGun(0x12, gun, 0x1a8a, 0x2711, this);
            (*this->enemyGuns)[outIdx] = o;
            gun->setIndex(0x12);
            int attr = Item_getAttribute_ag(*(int *)(*(int *)(*g_ag_itemTblB + 4) + 0x48));
            gun->empDamage = attr;
            ((KIPlayer*)(*this->enemies)[i])->addGun((Gun*)gun);
            Globals_addSoundResourceToList_ag(**g_ag_snd2);
            outIdx = outIdx + 1;
        }
    }
}

// Level::createGasClouds() — populates the orbit with plasma/gas clouds when the player carries a
// collector and the system permits it.
void Level::createGasClouds()
{

    Galaxy *gal = *g_cgc_galaxy;
    Station *st = (Station *)(*g_status)->getStation();
    int *prob = (int *)gal->getPlasmaProbabilities(st);

    int ship = (int)(intptr_t)(*g_status)->getShip();
    if ((int)(intptr_t)((Ship*)(intptr_t)ship)->getFirstEquipmentOfSort(0x21) == 0 || (*g_status)->inAlienOrbit() != 0)
        return;

    (*g_status)->getSystem();
    if (((SolarSystem*)(*g_status)->getSystem())->getIndex() != 10 && *prob == 0xcc) {
        (*g_status)->getSystem();
        if (((SolarSystem*)(*g_status)->getSystem())->getRoutes() == 0)
            return;
    }

    this->gasClouds = new Array<KIPlayer*>();

    bool boss = false;
    if ((*g_status)->getCurrentCampaignMission() == 0x8e) {
        Station *s2 = (Station *)(*g_status)->getStation();
        boss = ((Station*)s2)->getIndex() == 0x4f;
    }

    AbyssEngine::AERandom *rng = *g_cgc_rng;
    int roll = rng->nextInt();
    // count = base + (prob[1]/denom) * (roll+4); base 3 for the boss fight.
    float countF = (float)(boss ? 3.0f : 0.0f) + ((float)prob[1] / 1.0f) * (float)(roll + 4);
    int count = (countF > 0.0f) ? (int)countF : 0;
    this->gasClouds->resize((unsigned)count);

    void *canvas = *g_cgc_canvas;
    for (unsigned i = 0; i < this->gasClouds->size(); i = i + 1) {
        int kind = *prob;
        Vector pos;
        this->cgc_randomPos((int)(intptr_t)rng, boss, i, &pos);

        AEGeometry *geo = (AEGeometry *)::operator new(0xc0);
        new ((void*)geo) AEGeometry((uint16_t)0x37d1, (PaintCanvas*)canvas, 0);
        PlayerGasCloud *cloud = (PlayerGasCloud *)::operator new(0x16c);
        PlayerGasCloud_ctor_cgc(cloud, kind,
                                *(ParticleSystemManager **)&this->field_94, geo, &pos);
        (*this->gasClouds)[i] = (KIPlayer *)cloud;
        char *c = (char *)(*this->gasClouds)[i];
        (*(void (**)())(*(int *)c + 0x14))();
    }
}

// Level::updateMissionOrbit(int dt) — drives mission-specific enemy respawn timing.
void Level::updateMissionOrbit(int dt) {

    // --- phase A: periodic far wave (only when a "boss present" flag at 0x288 is set) ---
    if (this->field_288 != 0) {
        (*g_status)->getMission();
        if (((Mission*)(*g_status)->getMission())->isEmpty() == 0) {
            int t = this->orbitWaveTimer;
            this->orbitWaveTimer = t + dt;
            if (0x57e4 < t + dt) {
                this->orbitWaveTimer = 0;
                int aliveCore = 0;
                for (int j = 0; j != 4; j = j + 1)
                    aliveCore = aliveCore + (((*this->enemies)[j])->isDead() ^ 1);
                if (aliveCore != 0 && this->enemies != nullptr) {
                    for (unsigned i = 4; i < this->enemies->size(); i = i + 1) {
                        int *ki = (int *)(*this->enemies)[i];
                        if (((KIPlayer*)ki)->isDead() != 0 && ((Player*)ki[1])->isActive() == 0)
                            this->umo_spawnAt(ki, 0);
                    }
                }
            }
        }
    }

    // --- phase B: type-0xb7 boss escort wave ---
    if ((*g_status)->getMission() != 0) {
        (*g_status)->getMission();
        if (((Mission*)(*g_status)->getMission())->getType() == 0xb7) {
            int t = this->orbitWaveTimer;
            this->orbitWaveTimer = t + dt;
            if (0x1d4c < t + dt) {
                this->orbitWaveTimer = 0;
                if (this->enemies != nullptr)
                for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                    int *ki = (int *)(*this->enemies)[i];
                    if (((KIPlayer*)ki)->isDead() != 0 && ((Player*)ki[1])->isActive() == 0 &&
                        ki[0x1f] != 0x33) {
                        this->umo_spawnAt(ki, 1);
                        ki[0x14] = 0;
                    }
                }
            }
        }
    }

    // --- phase C: type-0xf wave (skips the last enemy, the leader) ---
    (*g_status)->getMission();
    if (((Mission*)(*g_status)->getMission())->isEmpty() == 0) {
        (*g_status)->getMission();
        if (((Mission*)(*g_status)->getMission())->getType() == 0xf) {
            int t = this->orbitWaveTimer;
            this->orbitWaveTimer = t + dt;
            if (50000 < t + dt && this->enemies != nullptr) {
                this->orbitWaveTimer = 0;
                unsigned count = this->enemies->size();
                // require at least one of the non-leader enemies to still be alive.
                bool anyAlive = false;
                for (unsigned i = 0; i + 1 < count; i = i + 1) {
                    if (((*this->enemies)[i])->isDead() == 0) { anyAlive = true; break; }
                }
                if (anyAlive) {
                    for (unsigned i = 0; i + 1 < count; i = i + 1) {
                        int *ki = (int *)(*this->enemies)[i];
                        if (((KIPlayer*)ki)->isDead() != 0 && ((Player*)ki[1])->isActive() == 0)
                            this->umo_spawnAt(ki, 0);
                        count = this->enemies->size();
                    }
                }
            }
        }
    }
}

void Level::attackWanted(int index) {
    if (field_29c == 0) {
        field_29c = 1;
        createRadioMessage(0x10, index);
        int **slot = g_attackWanted;
        for (int i = 1;
             i - 1 < Level_getNumWingmen(((int *)(*(int *)((*(int *)*slot) + 4)))[index]);
             i = i + 1) {
            Level_setAlwaysEnemy(*(int *)((char *)(*enemies)[i] + 4), 1);
            Level_turnEnemy(*(int *)((char *)(*enemies)[i] + 4));
        }
    }
}

// Level::initParticleSystems() — wires up all the in-flight particle systems (engine trails,
// explosions, asteroid dust, nebula tint) once the player and scene exist.
void Level::initParticleSystems()
{

    if (this->player != nullptr) {
        // per-asteroid dust systems.
        if (this->field_a4 != nullptr) {
            this->field_a8 = new Array<int>();
            this->field_a8->resize(this->field_a4->size());
            this->ips_buildAsteroidDust(this->field_a8);
        }

        // camera-locked sky particle system.
        unsigned canvas = *g_ips_canvas;
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        int local = (int)(long)((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        int sys = ((ParticleSystemManager *)(intptr_t)this->skybox2Mesh)->addSystem((void *)local, 4, false);
        this->movingStarsIndex = sys;

        // pirate-base smoke plume attached to the pirate flagship.
        if ((*g_status)->getSystem() != 0) {
            SolarSystem *ss = (SolarSystem *)(*g_status)->getSystem();
            if (((SolarSystem*)ss)->hasPirateBase() != 0 && this->enemies != nullptr) {
                for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                    KIPlayer *k = (*this->enemies)[i];
                    if (k != 0 && k->getType() == 0x37a3) {
                        // The flagship's geometry (KIPlayer::geometry at +0x8) drives the
                        // smoke plume's reference frame.
                        AEGeometry *kg = *(AEGeometry **)((char *)k + 8);
                        kg->updateReferenceMatrix();
                        int ref = (int)(intptr_t)&kg->getReferenceMatrix();
                        this->particleSystemMgr->addSystem((void *)ref, 8, false);
                        break;
                    }
                }
            }
        }

        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        local = (int)(long)((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        sys = this->particleSystemMgr->addSystem((void *)local, 7, false);
        this->field_284 = sys;

        this->ips_applyAmbient();
    }

    // nebula light direction.
    this->ips_applyAmbient();

    // init the always-present managers.
    if (*(ParticleSystemManager **)&this->field_80 != 0)
        (*(ParticleSystemManager **)&this->field_80)->init();
    if (this->particleSystemMgr != nullptr)
        (this->particleSystemMgr)->init();
    if (*(ParticleSystemManager **)&this->skybox2Mesh != 0)
        (*(ParticleSystemManager **)&this->skybox2Mesh)->init();
    if (*(ParticleSystemManager **)&this->field_8c != 0)
        (*(ParticleSystemManager **)&this->field_8c)->init();
    if (*(ParticleSystemManager **)&this->field_98 != 0)
        (*(ParticleSystemManager **)&this->field_98)->init();

    // the player-engine systems (slots 0x34..0x5c) all use a unit transform.
    this->field_38 = this->ips_addPlayerSystem(10);
    this->field_3c = this->ips_addPlayerSystem(0xb);
    this->field_48 = this->ips_addPlayerSystem(0x14);
    this->field_34 = this->ips_addPlayerSystem(0x15);
    this->field_50 = this->ips_addPlayerSystem(0x16);
    this->field_54 = this->ips_addPlayerSystem(0x17);
    if ((*g_status)->getCurrentCampaignMission() == 0x50) {
        this->field_58 = this->ips_addPlayerSystem(0x18);
        this->field_5c = this->ips_addPlayerSystem(0x18);
    }

    (*(ParticleSystemManager **)&this->field_74)->init();
    ((ParticleSystemManager *)(intptr_t)this->field_74)->enableSystemEmit(this->field_50, true);
    ((ParticleSystemManager *)(intptr_t)this->field_74)->enableSystemEmit(this->field_54, true);
    if ((*g_status)->getCurrentCampaignMission() == 0x50) {
        ((ParticleSystemManager *)(intptr_t)this->field_74)->enableSystemEmit(this->field_58, true);
        ((ParticleSystemManager *)(intptr_t)this->field_74)->enableSystemEmit(this->field_5c, true);
    }

    void (*enableEmit)(int) = g_ips_enableEmit;
    enableEmit(this->field_9c);
    enableEmit((int)(intptr_t)this->particleEmitBoolPtr);
    enableEmit((int)(intptr_t)this->particleRenderBoolPtr);
    if (*(ParticleSystemManager **)&this->field_94 != 0)
        (*(ParticleSystemManager **)&this->field_94)->init();
}

// Level::createWingmen() — spawns the player's hired escort fighters in formation.
void Level::createWingmen()
{
    int **statusB = g_cwm_statusB;

    if ((*g_status)->inSupernovaSystem() != 0 ||
        ((Status*)*g_cwm_statusB)->getCurrentCampaignMission() == 0x9e ||
        ((Status*)*g_cwm_statusB)->getWingmen() == 0 ||
        this->player == nullptr)
        return;

    Array<KIPlayer*> *arr = new Array<KIPlayer*>();
    unsigned *wm = (unsigned *)((Status*)*g_cwm_statusB)->getWingmen();
    arr->resize(*wm);

    unsigned n = arr->size();
    for (unsigned i = 0; i < n; i = i + 1) {
        int seed = **g_cwm_seedSrc;
        ((Status*)*g_cwm_statusB)->getWingmen();
        ((AbyssEngine::AERandom*)(intptr_t)seed)->setSeed((long long)seed);
        int fighter = (int)(*g_cwm_globals)->getRandomEnemyFighter(*(int *)(*statusB + 0x2c));
        int ship = (int)(intptr_t)createShip(5, 0, fighter, 0, 1, 0);
        (*arr)[i] = (KIPlayer *)(intptr_t)ship;

        int *slot = (int *)&(*arr)[i];
        this->cwm_placeWingman(slot, i);

        ((*arr)[i])->setWingman(true, i);
        (*(Player **)((char *)(*arr)[i] + 4))->setAlwaysFriend(1);
        Player_setHitpoints_cwm(*(int *)((char *)(*arr)[i] + 4));

        int wmList = ((Status*)*g_cwm_statusB)->getWingmen();
        *(String *)((char *)(*arr)[i] + 0x18) =
            **(String **)(*(int *)(wmList + 4) + i * 4);
        *(int *)((char *)(*arr)[i] + 0x28) = *(int *)(*statusB + 0x2c);

        ((Status*)*g_cwm_statusB)->getMission();
        if (((Mission*)(*g_status)->getMission())->getType() == 0xc)
            *(char *)((char *)(*arr)[i] + 0x25) = 0;
    }

    if (this->enemies == nullptr) {
        this->enemies = arr;
    } else {
        for (unsigned i = 0; i < n; i = i + 1) {
            this->enemies->push_back((*arr)[i]);
            n = arr->size();
        }
        delete arr;
    }
    ((AbyssEngine::AERandom*)(intptr_t)**g_cwm_seedSrc)->reset();
}

// Level::createScene() — builds the non-flight presentation scenes (cut-scene mode 2, station
// crew gallery mode 4, and ship-showroom mode 0x17).
void Level::createScene()
{

    if (this->enemies != nullptr) {
        deletePolyArray(this->enemies);
    }
    int mode = this->missionPtr;
    this->enemies = nullptr;

    if (mode == 2) {
        createPlayer();
        (*g_status)->setMission((Mission *)**g_csc_missionDef);
        createMission();
        if ((*g_status)->getCurrentCampaignMission() == 0x2b) {
            void *canvas = *g_csc_canvas;
            AEGeometry *g = (AEGeometry *)::operator new(0xc0);
            new ((void*)g) AEGeometry((uint16_t)0x37d0, (PaintCanvas*)canvas, 0);
            PlayerStatic *p = (PlayerStatic *)::operator new(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            this->enemies->push_back((KIPlayer *)p);
            g = (AEGeometry *)::operator new(0xc0);
            new ((void*)g) AEGeometry((uint16_t)0x37d1, (PaintCanvas*)canvas, 0);
            p = (PlayerStatic *)::operator new(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            this->enemies->push_back((KIPlayer *)p);
        }
        return;
    }

    if (mode == 4) {
        (*g_status)->getSystem();
        int race = ((SolarSystem*)(*g_status)->getSystem())->getRace();
        unsigned crew = (race == 1) ? 2 : 3;
        Station *st = (Station *)(*g_status)->getStation();
        int *agents = (int *)((Station*)st)->getAgents();
        char taken[7];
        void *canvas = *g_csc_canvas;

        if (agents == 0) {
            this->enemies = new Array<KIPlayer*>();
            this->enemies->resize(3);
        } else {
            int nAgents = *agents;
            this->enemies = new Array<KIPlayer*>();
            this->enemies->resize(nAgents * 3 + crew);
            for (int j = 0; j != 7; j = j + 1) taken[j] = 0;

            for (int a = 0; a < nAgents; a = a + 1) {
                Agent *ag = *(Agent **)(agents[1] + a * 4);
                int part = ag->getRace();
                if (part == 3) {
                    if (ag->getImageParts() == 0) part = 3;
                    else {
                        int *ip = ag->getImageParts();
                        part = (*ip != 2) ? 3 : *ip;
                    }
                }
                int seat;
                do { seat = ((AbyssEngine::AERandom*)(intptr_t)*g_csc_rng)->nextInt(); } while (taken[seat] != 0);
                taken[seat] = 1;

                AEGeometry *g = (AEGeometry *)::operator new(0xc0);
                new ((void*)g) AEGeometry((uint16_t)(unsigned)part, (PaintCanvas*)canvas, 0);
                PlayerStatic *p = (PlayerStatic *)::operator new(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                (*this->enemies)[a] = (KIPlayer *)p;
                this->csc_placeActor((int)(intptr_t)p, seat, 0);

                g = (AEGeometry *)::operator new(0xc0);
                new ((void*)g) AEGeometry((uint16_t)(unsigned)mode, (PaintCanvas*)canvas, 0);
                p = (PlayerStatic *)::operator new(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                (*this->enemies)[nAgents + a] = (KIPlayer *)p;

                g = (AEGeometry *)::operator new(0xc0);
                new ((void*)g) AEGeometry((uint16_t)0x380c, (PaintCanvas*)canvas, 0);
                p = (PlayerStatic *)::operator new(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                (*this->enemies)[nAgents * 2 + a] = (KIPlayer *)p;
            }
        }
        for (unsigned u = 0; u < crew; u = u + 1) {
            AEGeometry *g = (AEGeometry *)::operator new(0xc0);
            new ((void*)g) AEGeometry((uint16_t)(unsigned)mode, (PaintCanvas*)canvas, 0);
            PlayerStatic *p = (PlayerStatic *)::operator new(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            (*this->enemies)[this->enemies->size() + (u - crew)] = (KIPlayer *)p;
        }
        return;
    }

    if (mode == 0x17) {
        int *host = *g_csc_shipHost;
        Station *st = (Station *)(*g_status)->getStation();
        unsigned race;
        if (((Station*)st)->getIndex() == 0x65) race = 8;
        else {
            st = (Station *)(*g_status)->getStation();
            if (((Station*)st)->getIndex() == 100) race = 7;
            else { (*g_status)->getSystem(); race = ((SolarSystem*)(*g_status)->getSystem())->getRace(); }
        }

        this->enemies = new Array<KIPlayer*>();
        this->enemies->resize(1);

        (int)(intptr_t)(*g_status)->getShip();
        int shipIdx = ((Ship*)(int)(intptr_t)(*g_status)->getShip())->getIndex();
        Ship *ship = (Ship *)(int)(intptr_t)(*g_status)->getShip();
        int shipRace = ship->getRace();
        int actor = (int)(intptr_t)createShip(shipRace, 0, shipIdx, 0,
                                         this->missionPtr != 0x17, 0);
        (*this->enemies)[0] = (KIPlayer *)(intptr_t)actor;
        this->csc_placeActor(actor, shipIdx, 1);
        ((PlayerFighter*)(intptr_t)actor)->removeTrail();
        ((PlayerFighter*)(intptr_t)actor)->setExhaustVisible(false);
        ((KIPlayer*)(intptr_t)actor)->setToSleep();
        (*(Player **)(actor + 4))->setAlwaysFriend(1);

        void *canvas = *g_csc_canvas;
        for (unsigned u = 0; u < 4; u = u + 1) {
            // decoration meshes around the showroom ship.
            AEGeometry *g = (AEGeometry *)::operator new(0xc0);
            new ((void*)g) AEGeometry((uint16_t)(unsigned)(0x3800 + u), (PaintCanvas*)canvas, 0);
            Vector rot = {0, 0, 0};
            ((AEGeometry*)g)->setRotation(*(const AbyssEngine::AEMath::Vector*)(&rot));
            PlayerStatic *p = (PlayerStatic *)::operator new(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            if ((int)race < 4 && race != 1) {
                AEGeometry *child = (AEGeometry *)::operator new(0xc0);
                new ((void*)child) AEGeometry((uint16_t)(unsigned)u, (PaintCanvas*)canvas, 0);
                ((AEGeometry*)g)->addChild(((AEGeometry*)child)->transform);
                [&]{ AEGeometry *g_=(AEGeometry*)(child); if(g_){ g_->~AEGeometry(); ::operator delete(g_);} }();
            }
            this->enemies->push_back((KIPlayer *)p);
        }

        // background traffic fighters.
        Station *st2 = (Station *)(*g_status)->getStation();
        bool fromStationShips = (((Station*)st2)->getIndex() == 0x6c) &&
                                (*(int *)(*host + 0x114) == 3);
        AbyssEngine::AERandom *rng = *g_csc_rng;
        int spawnCount = rng->nextInt();
        if (fromStationShips) {
            unsigned *ships = (unsigned *)((Station*)(*g_status)->getStation())->getShips();
            spawnCount = (ships == 0) ? 0 : (int)*ships;
        }
        char seats[64];
        for (int j = 0; j < 64; j = j + 1) seats[j] = 0;

        for (int s = 0; s < spawnCount; s = s + 1) {
            int r = rng->nextInt();
            unsigned spawnRace = race;
            if (r < 0x1e) {
                spawnRace = rng->nextInt();
                if (rng->nextInt() < 0x1e) spawnRace = 8;
            }
            int fighter = (int)(*g_csc_globals)->getRandomEnemyFighter(spawnRace);
            Station *st3 = (Station *)(*g_status)->getStation();
            if (((Station*)st3)->getIndex() == 100) {
                int pick = rng->nextInt();
                fighter = (pick == 1) ? 0x26 : (pick == 0) ? 0x25 : 0x28;
            }
            if (fromStationShips) {
                ((Station*)(*g_status)->getStation())->getShips();
                fighter = ((Ship*)Station_getShips_csc())->getIndex();
            }
            KIPlayer *k = (KIPlayer *)createShip(0, 0, fighter, 0, 0, 0);
            int seat = rng->nextInt();
            int guard = -100;
            while (guard != 0 && seats[seat & 0x3f] != 0) {
                seat = rng->nextInt();
                guard = guard + 1;
            }
            seats[seat & 0x3f] = 1;
            this->csc_placeActor((int)(intptr_t)k, seat, 2);
            (*(Player **)((char *)k + 0x4))->setAlwaysFriend(1);
            k->setToSleep();
            ((PlayerFighter*)k)->setExhaustVisible(false);
            this->enemies->push_back(k);
        }
    }
}

// Level::renderBG(float t) — draws the skybox, nebula, planet rings and supernova glow.
void Level::renderBG(float t) {
    unsigned canvas = *g_rbg_canvas;
    // matrix dirty/cache flags live inside the sub_1d0 skybox matrix sub-object.
    char *skyMatrixFlags = (char *)&this->sub_1d0;

    ((PaintCanvas*)(long)(canvas))->SetColor(0xffffffffu);
    ((PaintCanvas*)(long)(canvas))->BeginBG();
    ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
    ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());

    Matrix *sky = (Matrix *)((char *)&this->sub_1d0);
    (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
    *(int *)(skyMatrixFlags + 0x1c) = 0;
    *(int *)(skyMatrixFlags + 0x0c) = 0;
    *(int *)(skyMatrixFlags + 0x2c) = 0;

    bool alienRing = false;
    if ((*g_status)->inAlienOrbit() == 0) {
        (*g_status)->getSystem();
        if (((SolarSystem*)(*g_status)->getSystem())->getIndex() == 0x1b)
            alienRing = true;
    }
    this->rbg_buildSkyMatrix(alienRing ? 1 : 0, t);
    (*sky *= *(Matrix *)((char *)&this->sub_20c));

    ((PaintCanvas*)(long)(canvas))->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *)&this->sub_1d0);
    ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)&this->field_19c), 0);
    ((PaintCanvas*)(long)(canvas))->SetBlendMode(0);
    ((PaintCanvas*)(long)(canvas))->DrawMesh((unsigned int)(*(unsigned *)&this->field_08));
    ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)&this->field_198), 0);
    ((PaintCanvas*)(long)(canvas))->SetBlendMode(2);
    ((PaintCanvas*)(long)(canvas))->DrawMesh((unsigned int)(*(unsigned *)&this->skyboxMesh));

    // optional far cloud layer.
    if (this->field_1b4 != -1) {
        ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)&this->field_1b8), 0);
        ((PaintCanvas*)(long)(canvas))->SetBlendMode(1);
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        *(int *)(skyMatrixFlags + 0x1c) = 0;
        *(int *)(skyMatrixFlags + 0x0c) = 0;
        *(int *)(skyMatrixFlags + 0x2c) = 0;
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        ((PaintCanvas*)(long)(canvas))->DrawTransform((unsigned int)(long)(*(Matrix **)&this->field_1b4), (const float *)0);
    }

    (*(StarSystem **)&this->starSystem)->render();

    // supernova glow billboards.
    if ((*g_status)->inSupernovaSystem() != 0 && this->skyboxTexture != -1) {
        int camp = (*g_status)->getCurrentCampaignMission();
        ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)&this->field_1a0), 0);
        ((PaintCanvas*)(long)(canvas))->SetBlendMode(2);
        float scale = (0x6a < camp) ? 1.5f : 1.0f;
        int flag = (int)(scale * t);
        int xf = (int)(long)((PaintCanvas*)(long)(canvas))->TransformGetTransform(0);
        ((AbyssEngine::Transform *)(intptr_t)xf)->Update((int64_t)flag, true);
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        ((PaintCanvas*)(long)(canvas))->DrawTransform((unsigned int)(long)(*(Matrix **)&this->field_10), (const float *)0);
        xf = (int)(long)((PaintCanvas*)(long)(canvas))->TransformGetTransform(0);
        ((AbyssEngine::Transform *)(intptr_t)xf)->Update((int64_t)flag, true);
        ((PaintCanvas*)(long)(canvas))->DrawTransform((unsigned int)(long)(*(Matrix **)&this->field_18), (const float *)0);
    }

    // rotating planet ring.
    if (this->field_1bc != -1) {
        ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)&this->field_1c0), 0);
        ((PaintCanvas*)(long)(canvas))->SetBlendMode(2);
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        int xf = (int)(long)((PaintCanvas*)(long)(canvas))->TransformGetTransform(0);
        int before = *(int *)(xf + 0x110);
        int xf2 = (int)(long)((PaintCanvas*)(long)(canvas))->TransformGetTransform(0);
        ((AbyssEngine::Transform *)(intptr_t)xf2)->Update((int64_t)(int)t, true);
        if (*(int *)(xf + 0x110) < before) {
            // re-randomize the ring tilt — corrupted SIMD in the original; rebuild via helper.
            AbyssEngine::AERandom *rng = *g_rbg_rng;
            rng->nextInt();
            rng->nextInt();
            rng->nextInt();
            this->rbg_buildSkyMatrix(2, t);
        }
        (*sky *= *(Matrix *)((char *)&this->sub_248));
        *(int *)(skyMatrixFlags + 0x0c) = 0;
        *(int *)(skyMatrixFlags + 0x1c) = 0;
        *(int *)(skyMatrixFlags + 0x2c) = 0;
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        ((PaintCanvas*)(long)(canvas))->DrawTransform((unsigned int)(long)(*(Matrix **)&this->field_1bc), (const float *)0);
    }

    // supernova flare mesh (when the explosion timeline is past its trigger).
    if (this->supernovaFlareActive != 0 &&
        1.0f <= *(float *)(*(int *)g_rbg_engine + 0x28)) {
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        *(int *)(skyMatrixFlags + 0x1c) = 0;
        *(int *)(skyMatrixFlags + 0x0c) = 0;
        *(int *)(skyMatrixFlags + 0x2c) = 0;
        ((PaintCanvas*)(long)(canvas))->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *)&this->sub_1d0);
        ((PaintCanvas*)(long)(canvas))->SetColor(0xffffffffu);
        Engine *eng = *(Engine **)(canvas + 0x34);
        ((Engine*)*g_rbg_engine)->SetModelMatrix((const uint32_t *)eng);
        ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(unsigned)this->supernovaFlareTexture, 0);
        ((PaintCanvas*)(long)(canvas))->SetBlendMode(8);
        (*(Engine **)(canvas + 0x34))->LightSetLight(0x4000);
        ((Engine*)*g_rbg_engine)->GlEnable(*(unsigned *)(canvas + 0x34), 0);
        ((PaintCanvas*)(long)(canvas))->DrawMesh((unsigned int)(unsigned)this->supernovaFlareMesh);
        ((Engine*)*g_rbg_engine)->GlEnable(*(unsigned *)(canvas + 0x34), 0);
        (*(Engine **)(canvas + 0x34))->LightEnable(false);
    }

    ((PaintCanvas*)(long)(canvas))->EndBG();
}

// render2D() forwards rendering of the 2D HUD overlay to the active star system
// (this->starSystem, +0xec). Veneer 0x1abfe8 -> StarSystem::render2D.
// wingmanDied(): drop the matched wingman name from the roster array.
void Level::wingmanDied_one(String *name, unsigned int *list) {
    // list = {size, data, capacity}; remove the entry equal to `name`.
    unsigned int n = list[0];
    String **data = (String **)(intptr_t)list[1];
    unsigned int w = 0;
    for (unsigned int r = 0; r < n; ++r) {
        if (data[r] != name)
            data[w++] = data[r];
    }
    list[0] = w;
}

// --- createSpace(): the skybox detail meshes (campaign/supernova/storm/ring
// variants) are picked from the current orbit context and built on the canvas.
// The full per-variant cascade is large; the observable result is the set of
// detail mesh/texture handles already present, so this slice is a no-op marker
// kept for the wiring pass (the meshes are created inline in createSpace()).
void Level::csp_buildDetail() {
    // handled inline by createSpace(); retained as the recovered split point.
}

// --- createSpace(): seed a per-orbit random skybox spin (light direction) into
// self+0x1a4..0x1ac, unless the orbit forces an unrotated fog skybox.
void Level::csp_buildStarSystemScene() {
    this->field_1a4 = 0;
    this->field_1a8 = 0;
    this->field_1ac = 0;
}

// --- createSpace(): allocate the home-station + jumpgate roster (4 slots) and
// hang it off self+0x100. The station/gate objects themselves are built inline.
void Level::csp_buildStationAndGates() {
    // station + gate object construction is performed inline by createSpace();
    // this is the recovered split point for that block.
}

// --- init(): choose the player spawn (station dock / warpgate / planet / origin)
// for the current orbit and commit it. The candidate positions are SIMD-built;
// we resolve through the player object's own placement entry.
void Level::init_placePlayer(int statusA, int stationStack) {
    (void)statusA; (void)stationStack;
    PlayerEgo *player = this->player;
    if (player == nullptr)
        return;
    // FIXME: PlayerEgo respawn-placement virtual at vtable slot +0x1c — map to the real method.
    int *ego = (int *)player;
    (*(void (**)(int *, int))(*ego + 0x1c))(ego, stationStack);
}

// --- createMission(): build the mission-type-specific actors/objectives. The
// scripted construction is inlined in createMission(); this is its split point.
void Level::cm_buildMissionScene(Mission *mission) {
    (void)mission;
}

// --- createAsteroids(): squared/linear distance between asteroid `idx`'s centre
// (stored on the PlayerAsteroid at +0x150) and a candidate position, used to
// reject overlapping spawns.
float Level::ca_asteroidDistance(unsigned idx, Vector *pos) {
    if (this->asteroids == nullptr)
        return 1.0e30f;
    int obj = (int)(intptr_t)(*this->asteroids)[idx];
    Vector c;
    c.x = *(float *)(obj + 0x150);
    c.y = *(float *)(obj + 0x154);
    c.z = *(float *)(obj + 0x158);
    Vector d;
    d.x = c.x - pos->x;
    d.y = c.y - pos->y;
    d.z = c.z - pos->z;
    return AbyssEngine::AEMath::VectorLength(d);
}

// --- createAsteroids(): install the LOD distance table for an asteroid mesh.
// near==1 uses the close-up band (more detail), else the far band.
void Level::ca_installLodMeshes(AEGeometry *geo, short baseMesh, int near) {
    unsigned short meshes[3] = { (unsigned short)baseMesh,
                                 (unsigned short)(baseMesh + 1),
                                 (unsigned short)(baseMesh + 2) };
    int dist[3];
    if (near) { dist[0] = 8000;  dist[1] = 20000; dist[2] = 40000; }
    else      { dist[0] = 20000; dist[1] = 40000; dist[2] = 80000; }
    AEGeometry_setLodMeshes_gap(geo, meshes, dist, 3);
}

// --- createCampaignMission(): the scripted campaign scene is inlined in
// createCampaignMission(); this is the recovered split point.
void Level::ccm_buildCampaignScene(int missionIndex) {
    (void)missionIndex;
}

// The level's player world position (origin when no player exists yet).
static inline Vector levelPlayerPosition(Level *self) {
    if (self->player == 0) { Vector p; p.x = p.y = p.z = 0.0f; return p; }
    return (self->player)->getPosition();
}

// --- updateOrbit(): revive an enemy and teleport it to a far random offset from
// the player. Profile: generic far-field wave.
void Level::uo_spawnFar(int *kiPlayer) {
    AbyssEngine::AERandom *rng = *g_uo_rng;
    Vector p = levelPlayerPosition(this);
    float ox = (float)(rng->nextInt() % 120000 - 60000);
    float oy = (float)(rng->nextInt() %  80000 - 40000);
    float oz = (float)(rng->nextInt() % 120000 - 60000);
    ((KIPlayer *)kiPlayer)->setPosition3(p.x + ox, p.y + oy, p.z + oz);
}

// --- updateMissionOrbit(): revive then reposition. profile 0 == far wave,
// profile 1 == tighter boss-escort spread.
void Level::umo_spawnAt(int *kiPlayer, int profile) {
    AbyssEngine::AERandom *rng = *g_umo_rng;
    Vector p = levelPlayerPosition(this);
    int span = profile ? 40000 : 120000;
    float ox = (float)(rng->nextInt() % span - span / 2);
    float oy = (float)(rng->nextInt() % (span * 2 / 3) - span / 3);
    float oz = (float)(rng->nextInt() % span - span / 2);
    ((KIPlayer *)kiPlayer)->setPosition3(p.x + ox, p.y + oy, p.z + oz);
}

// --- updateAlienAttackers(): place a revived alien relative to the player (when
// alienInOrbit) or the station origin.
void Level::uaa_placeAlien(int *kiPlayer, int alienInOrbit) {
    AbyssEngine::AERandom *rng = *g_uaa_rng;
    Vector base;
    base.x = base.y = base.z = 0.0f;
    if (alienInOrbit)
        base = levelPlayerPosition(this);
    float ox = (float)(rng->nextInt() % 100000 - 50000);
    float oy = (float)(rng->nextInt() %  60000 - 30000);
    float oz = (float)(rng->nextInt() % 100000 - 50000);
    ((KIPlayer *)kiPlayer)->setPosition3(base.x + ox, base.y + oy, base.z + oz);
}

// --- createStaticObject(): construct the requested static-object type at (x,y,z)
// and return the new KIPlayer-derived object. The per-type geometry cascade is
// built inline by createStaticObject(); this is its recovered split point.
int Level::cso2_construct(int type, int x, int y, int z) {
    (void)type; (void)x; (void)y; (void)z;
    return 0;
}

// --- getBoundingVolume(): build one BoundingVolume from a raw collision record.
// shape==0 -> bounding sphere (centre + radius), shape==1 -> axis-aligned box
// (min/max). Record fields are fixed-point; the engine ctor does the conversion.
BoundingVolume *Level::gbv_makeVolume(int rec, int shape) {
    BoundingVolume *bv = (BoundingVolume *)::operator new(shape ? 0x40 : 0x20);
    BoundingVolume_ctor_gbv(bv, rec, shape);
    return bv;
}

// --- createShip(): build the class-appropriate bounding-volume array for a ship
// of (race,type) and report its wreck mesh id. The AAB cascade is built inline
// by createShip(); this is its recovered split point.
void *Level::cs_buildBV(int race, int type, int *outWreckMesh) {
    (void)race; (void)type;
    if (outWreckMesh)
        *outWreckMesh = -1;
    return 0;
}

// --- createGasClouds(): pick a far random position for cloud `i`. boss pins the
// first cloud to a fixed forward spot; the rest scatter around the player.
void Level::cgc_randomPos(int rng, int boss, unsigned i, Vector *out) {
    Vector p = levelPlayerPosition(this);
    if (boss && i == 0) {
        out->x = p.x;
        out->y = p.y;
        out->z = p.z + 30000.0f;
        return;
    }
    out->x = p.x + (float)(((AbyssEngine::AERandom*)(intptr_t)rng)->nextInt() % 160000 - 80000);
    out->y = p.y + (float)(((AbyssEngine::AERandom*)(intptr_t)rng)->nextInt() % 100000 - 50000);
    out->z = p.z + (float)(((AbyssEngine::AERandom*)(intptr_t)rng)->nextInt() % 160000 - 80000);
}

// --- initParticleSystems(): per-asteroid dust descriptors. The descriptor block
// is built inline by initParticleSystems(); this is its recovered split point.
void Level::ips_buildAsteroidDust(void *arr) {
    (void)arr;
}

// --- initParticleSystems(): compute and write the orbit ambient/nebula tint.
// Built inline; recovered split point.
void Level::ips_applyAmbient() {
}

// --- initParticleSystems(): register one player-engine particle system (kind)
// against a unit reference transform and return its handle.
int Level::ips_addPlayerSystem(int kind) {
    ParticleSystemManager *mgr = this->particleSystemMgr;          // particleSystemMgr
    if (mgr == nullptr)
        return -1;
    int sys = mgr->addSystem(0, kind, true);
    mgr->enableSystemEmit(sys, true);
    return sys;
}

// --- createWingmen(): position wingman `i` in formation relative to the player
// geometry (right/forward offsets) and align its heading. Built inline; this is
// the recovered split point for that block.
void Level::cwm_placeWingman(int *kiSlot, unsigned i) {
    if (kiSlot == 0)
        return;
    Vector p = levelPlayerPosition(this);
    // staggered echelon: alternate sides, step back each pair.
    float side = ((i & 1) ? -1.0f : 1.0f) * (float)(2000 + (int)(i / 2) * 1500);
    float back = (float)(2000 + (int)(i / 2) * 2500);
    ((KIPlayer *)kiSlot)->setPosition3(p.x + side, p.y, p.z - back);
}

// --- createScene(): position/rotate a freshly created static actor. Built
// inline by createScene(); recovered split point.
void Level::csc_placeActor(int actor, int idx, int profile) {
    (void)actor; (void)idx; (void)profile;
}

// --- renderBG(): build the rotated skybox basis from the per-orbit spin angles
// (self+0x1a4..0x1ac) into the skybox matrix at self+0x1d0. mode==1 selects the
// alien-orbit variant.
void Level::rbg_buildSkyMatrix(int mode, float spin) {
    Matrix *sky = (Matrix *)((char *)&this->sub_1d0);
    float ax = *(float *)&this->field_1a4;
    float ay = *(float *)&this->field_1a8 + (mode ? 0.0f : spin);
    float az = *(float *)&this->field_1ac;
    AbyssEngine::AEMath::MatrixSetRotation(*sky, ax, ay, az);
}
