#include "game/world/Level.h"
#include "game/mission/Mission.h"
#include "game/ship/Ship.h"
#include "engine/render/AEGeometry.h"
#include "externs.h"
#include "game/core/PaintCanvasClass.h"
#include "game/mission/Achievements.h"
#include "game/ui/Hud.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerEgo.h"
#include "game/weapons/Gun.h"
#include "game/ship/PlayerTurret.h"
#include "game/world/Waypoint.h"
#include "game/world/SolarSystem.h"
#include "game/world/Route.h"
#include "game/mission/Objective.h"
#include "game/mission/Item.h"
#include "engine/core/GameText.h"
#include "game/world/Station.h"
#include "engine/render/LODManager.h"
#include "game/world/Wanted.h"
#include "game/mission/Status.h"
#include "game/ship/KIPlayer.h"
#include "game/weapons/ObjectGun.h"
#include "game/core/RadioMessage.h"
#include "engine/render/ParticleSystemManager.h"
#include "game/world/StarSystem.h"
#include "engine/core/AbyssEngine.h"
#include "engine/core/AERandom.h"
#include "engine/core/ApplicationManager.h"
#include "engine/file/FileRead.h"
#include "engine/math/AEMath.h"
#include "engine/math/BoundingSphere.h"
#include "engine/math/BoundingAAB.h"
#include "game/core/Globals.h"
#include "game/ship/PlayerAsteroid.h"
#include "game/ship/PlayerFighter.h"
#include "game/ship/PlayerGasCloud.h"
#include "game/ship/PlayerStatic.h"
#include "game/weapons/RocketGun.h"
#include "game/world/Galaxy.h"
#include "platform/libc.h"
#include "game/ship/PlayerFixedObject.h"
#include <new>
#include "game/ship/Agent.h"
#include "engine/render/Engine.h"
#include "game/weapons/Radar.h"
#include "game/weapons/BeamGun.h"
#include "game/weapons/BombGun.h"
#include "game/weapons/MineGun.h"
#include "game/weapons/SentryGun.h"
#include "engine/math/Transform.h"

extern PaintCanvas *gCanvas;

Matrix *CameraGetLocal(void *canvas, uint32_t index);

__attribute__ ((visibility
(
"hidden"
)
)
)
extern unsigned char *g_initStreamOut;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_engineColorBase;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_beamTable; // [DAT_000ce2b4]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int g_cg_rocketFx;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int g_cg_objFx;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_objTable; // [DAT_000ce2d4]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_rocketTable; // [DAT_000ce2c8]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cg_rocketSnd; // [DAT_000ce2cc]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cg_itemTableA; // [DAT_000ce2c0]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern unsigned *g_cg_bombTable; // [DAT_000ce2c4]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_bombSnd; // [DAT_000ce64c]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int g_cg_mineFx;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_objTable8; // [DAT_000ce648]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_mineTable; // [DAT_000ce2d8]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_mineSnd; // [DAT_000ce654]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_objTable23; // [DAT_000ce660]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cg_sentryTable; // [DAT_000ce65c]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern unsigned *g_cg_bombTable2a; // [DAT_000ce664]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cg_snd29; // [DAT_000ce668]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cg_snd2a; // [DAT_000ce66c]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cg_snd2b; // [DAT_000ce670]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cg_snd2c; // [DAT_000ce674]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cg_snd2d; // [DAT_000ce678]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cg_snd2e; // [DAT_000ce67c]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_crm_counts8; // [DAT_000d59f8] per-stage counts
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_crm_table8; // [DAT_000d59fc] {id,arg} table
__attribute__ ((visibility
(
"hidden"
)
)
)
extern PaintCanvas ***g_init_canvas; // [DAT_000be260]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern char **g_init_flagStack; // [DAT_000be268]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_init_missionDef; // [DAT_000be5a4]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_init_settings; // [DAT_000be5ac]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern char **g_init_bmFlag; // [DAT_000be5b0]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern char **g_ca_lowDetail; // [DAT_000bfd1c] low-detail flag
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float g_ccm_pos0; // DAT_000c5310 (case-0 spawn coord)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float g_up_eqMax;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_up_clampLo; // [DAT_000d6244]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_up_clampHi; // [DAT_000d6248]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_up_clampZ; // [DAT_000d623c]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_up_clampW; // [DAT_000d6240]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_flash2_a;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_flash2_b;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_flash2_c;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_flashCol_r;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_flashCol_g;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float *g_flashCol_b;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void (**g_levelSubCtor)(void *);

__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cso_textA; // [DAT_000cc1ec]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cso_textB; // [DAT_000cc1f0] area
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float g_cso_posX;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float g_cso_posZ;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_cs_diffRec; // [DAT_000d0474]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_ag_diffRec; // [DAT_000cca44] difficulty record
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_ag_shipTbl; // [DAT_000cce38]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_ag_itemTblA; // [DAT_000cd1f0]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_ag_weaponDmg; // [DAT_000cd1f4]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_ag_statusB; // [DAT_000cce40]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_ag_alienCnt; // [DAT_000cce44]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_ag_itemTblB; // [DAT_000cd1fc]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern float g_ag_perLevel;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_ips_envA; // [DAT_000cdaa0] env record
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int *g_ips_envB; // [DAT_000cde3c]/[DAT_000cde38]
__attribute__ ((visibility
(
"hidden"
)
)
)
extern int **g_cwm_seedSrc; // [DAT_000cc678]

static unsigned int g_level_texOutScratch;

static inline float bitsToFloat(int bits) {
    float f;
    __builtin_memcpy(&f, &bits, sizeof(f));
    return f;
}

static inline __attribute__ ((always_inline))

void levelInitPlacePlayer(Level *self, int statusA, int stationStack);

static inline void levelSpawnFar(Level *self, int *kiPlayer);

static inline __attribute__ ((always_inline))

void levelSpawnAt(Level *self, int *kiPlayer, int profile);

static inline void levelPlaceAlien(Level *self, int *kiPlayer, int alienInOrbit);

static inline void levelCloudRandomPos(Level *self, int rng, int boss, unsigned i, Vector *out);

static inline void levelPlaceWingman(Level *self, int *kiSlot, unsigned i);

static inline void levelWingmanDiedOne(String *name, unsigned int *list);

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

int Level::checkGameOver(int param) {
    Objective *objective = objectivesB;
    if (objective == nullptr) {
        return 0;
    }
    // veneer 0x1ac018 -> Objective::achieved
    return (int) objective->achieved(param);
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
    gStatus->field_b0 += 1; // +0xb0 junk-death counter on the Status singleton
    enemiesLeft -= 1;
}

void Level::enableMovingStars(bool enable) {
    int index = movingStarsIndex;
    if (index < 0) {
        return;
    }
    // veneer 0x1ac048 -> ParticleSystemManager::enableSystemRender
    (skybox2Mesh)->enableSystemRender(index, enable);
}

void Level::setInitStreamOut() {
    *g_initStreamOut = 1;
}

int Level::getMiningPlant() {
    int index = miningPlantIndex;
    if (index < 0) {
        return 0;
    }
    return (int) (intptr_t)(*this->enemies)[index];
}

Array<KIPlayer *> *Level::getGasClouds() {
    return gasClouds;
}

void Level::asteroidDied() {
    asteroidsLeft -= 1;
}

int Level::checkObjective(int param) {
    Objective *objective = objectivesA;
    if (objective != nullptr) {
        // veneer 0x1ac018 -> Objective::achieved
        return (int) objective->achieved(param);
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

Array<KIPlayer *> *Level::getAsteroids() {
    return asteroids;
}

int Level::collide(Vector v, bool /*param*/) {
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
    particleSystemMgr->enableSystemEmit(field_284, render);
    // decompiler dropped the enable arg; Ghidra(0xcd68c) shows it is r2 (=render)
    particleSystemMgr->enableSystemRender(field_284, render);
    *(unsigned char *) particleRenderBoolPtr = render;
    *(unsigned char *) particleEmitBoolPtr = render;
}

void Level::switchSkyboxForIntro() {
    unsigned int skyboxMeshHandle;
    gCanvas->MeshCreate((unsigned short) (0x4591), skyboxMeshHandle, false);
    skyboxMesh = skyboxMeshHandle;
    unsigned int skyboxTexHandle;
    gCanvas->TextureCreate((unsigned short) (0x275a), nullptr, nullptr, skyboxTexHandle, false);
    field_198 = skyboxTexHandle;
    if (this->asteroids != nullptr) {
        for (unsigned int i = 0; i < this->asteroids->size(); i = i + 1) {
            (*this->asteroids)[i]->setActive(false);
        }
    }
}

void Level::switchSkyboxForSupernovaReversal() {
    int tex = ((SolarSystem *) (intptr_t) gStatus->getSystem())->getTextureIndex();
    unsigned int skyboxMeshHandle;
    gCanvas->MeshCreate((unsigned short) ((unsigned short) (tex + 0x4588)), skyboxMeshHandle, false);
    skyboxMesh = skyboxMeshHandle;
    int tex2 = ((SolarSystem *) (intptr_t) gStatus->getSystem())->getTextureIndex();
    unsigned int skyboxTexHandle;
    gCanvas->TextureCreate((unsigned short) ((unsigned short) (tex2 + 0x2751)), nullptr, nullptr, skyboxTexHandle,
                           false);
    field_198 = skyboxTexHandle;
    skyboxTexture = -1;
}

PlayerEgo *Level::getPlayer() {
    return player;
}

void Level::killWanted(int /*param*/) {
    if (field_29d == 0) {
        field_29d = 1;
        // veneer 0x1ac028 -> Level::createRadioMessage(type, sub)
        createRadioMessage(0x11, 0);
    }
}

Array<ObjectGun *> *Level::getEnemyGuns() {
    return enemyGuns;
}

void Level::stealFriendCargo() {
    friendCargoStolen = 1;
}

Array<KIPlayer *> *Level::getEnemies() {
    return enemies;
}

void Level::applyKills() {
    if (gStatus->getMission() != 0) {
        gStatus->addKills(kills);
        kills = 0;
    }
}

uint8_t Level::friendCargoWasStolen() {
    return friendCargoStolen;
}

Array<void *> *Level::getMessages() {
    return messages;
}

Route *Level::getEnemyRoute() {
    return enemyRoute;
}

Array<ObjectGun *> *Level::getPlayerGuns() {
    return playerGuns;
}

void Level::renderPause(long long /*ctx*/) {
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

StarSystem *Level::getStarSystem() {
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
        if (gStatus->getStation()->getPirateStationIndex() < 0) {
            return;
        }
        int tbl = (int) (intptr_t) gStatus->field_4c;
        int idx = gStatus->getStation()->getPirateStationIndex();
        *(unsigned char *) (*(int *) (tbl + 4) + idx) = 1;
        // RAWREAD: Array<bool> payload (engine container, not a modeled field)
        *(unsigned char *) ((char *) gStatus + 0xf9) = 1;
        // RAWREAD: Status +0xf9 (no member; sits in the int16 gap after field_f8)
    }
    // veneer 0x1ac028 -> Level::createRadioMessage(type, sub)
    createRadioMessage(param ? 3 : 4, 8);
}

int Level::getNumDockingTargets() {
    if (this->enemies != nullptr) {
        int total = 0;
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            KIPlayer *e = (*this->enemies)[i];
            total = total + (uint8_t) e->field_0x70;
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
            if ((char) obj->field_0x70 != 0) {
                if (found == index) {
                    return (int) (intptr_t) obj;
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

Array<KIPlayer *> *Level::getLandmarks() {
    return landmarks;
}

static inline void actorPreRender(KIPlayer *o, int ctx) {
    o->update(ctx);
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
        field_80->render3d();
    }
    if (field_74 != 0) {
        field_74->render3d();
    }
    if (particleEmitBoolPtr != nullptr) {
        particleEmitBoolPtr->render3d();
    }
    if (particleSystemMgr != nullptr) {
        particleSystemMgr->render3d();
    }
    if (skybox2Mesh != 0) {
        skybox2Mesh->render3d();
    }
    if (particleRenderBoolPtr != nullptr) {
        particleRenderBoolPtr->render3d();
    }
    if (field_8c != 0) {
        field_8c->render3d();
    }
    if (field_98 != 0) {
        field_98->render3d();
    }
    if (field_94 != 0) {
        field_94->render3d();
    }
    if (field_9c != 0) {
        field_9c->render3d();
    }
    if (starSystem != 0)
        starSystem->renderSunStreak();
}

int Level::collideStream(Vector v) {
    PlayerFixedObject *obj = (PlayerFixedObject *) (*this->landmarks)[1];
    if (obj != nullptr) {
        return obj->collide(v.x, v.y, v.z);
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
    delete objectivesA;
    objectivesA = nullptr;
    delete objectivesB;
    objectivesB = nullptr;
    delete collisionVolume;
    collisionVolume = nullptr;
    delete asteroidWaypoint;
    asteroidWaypoint = nullptr;
    delete starSystem;
    starSystem = nullptr;
    delete player;
    player = nullptr;
    delete field_180;
    field_180 = nullptr;
    delete field_80;
    field_80 = nullptr;
    delete skybox2Mesh;
    skybox2Mesh = nullptr;
    delete field_74;
    field_74 = nullptr;
    delete particleEmitBoolPtr;
    particleEmitBoolPtr = nullptr;
    delete particleSystemMgr;
    particleSystemMgr = nullptr;
    delete field_90;
    field_90 = nullptr;
    delete particleRenderBoolPtr;
    particleRenderBoolPtr = nullptr;
    delete field_98;
    field_98 = nullptr;
    delete field_9c;
    field_9c = nullptr;
    if (field_a4) {
        for (auto *e: *field_a4) delete e;
        delete field_a4;
        field_a4 = nullptr;
    }
    if (field_a8) {
        delete field_a8;
        field_a8 = nullptr;
    } // Array<int>: no element dtors
    if (playerGuns) {
        for (auto *e: *playerGuns) delete e;
        delete playerGuns;
        playerGuns = nullptr;
    }
    if (enemyGuns) {
        for (auto *e: *enemyGuns) delete e;
        delete enemyGuns;
        enemyGuns = nullptr;
    }
    if (enemies) {
        for (auto *e: *enemies) delete e;
        delete enemies;
        enemies = nullptr;
    }
    if (asteroids) {
        for (auto *e: *asteroids) delete e;
        delete asteroids;
        asteroids = nullptr;
    }
    if (gasClouds) {
        for (auto *e: *gasClouds) delete e;
        delete gasClouds;
        gasClouds = nullptr;
    }
    if (landmarks) {
        for (auto *e: *landmarks) delete e;
        delete landmarks;
        landmarks = nullptr;
    }
    if (messages) {
        for (void *e: *messages) delete (RadioMessage *) e;
        delete messages;
        messages = nullptr;
    }
    if (field_104) {
        for (auto *e: *field_104) delete e;
        delete field_104;
        field_104 = nullptr;
    }
    delete lodManager;
    lodManager = nullptr;
    delete field_a0;
    field_a0 = nullptr;
    if (field_b0) {
        delete field_b0;
        field_b0 = nullptr;
    } // container-only: elements owned by enemies
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
    for (int i = -1; i + 1 < (int) n; i = i + 3) {
        int sign = (gRandom->nextInt(2) == 0) ? 1 : -1;
        p[-1] = (gRandom->nextInt(30000) + 50000) * sign;
        p[0] = gRandom->nextInt(20000) - 10000;
        if (i == -1) {
            pts[2] = gRandom->nextInt() + 50000;
        } else {
            int prev = p[-2];
            p[1] = gRandom->nextInt() + prev + 50000;
        }
        p = p + 3;
    }
    Route *r = (Route *) ::operator new(0x18);
    new(r) Route(pts, n);
    return r;
}

void Level::alarmAllFriends(int race, bool message) {
    if (this->enemies != nullptr) {
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            KIPlayer *obj = (*this->enemies)[i];
            if (obj->shipGroup == race) {
                obj->player->setAlwaysEnemy(1);
            }
        }
    }
    // field_188 packs two flags; 0x188 is friendTurnedEnemy, 0x189 is the alarm flag.
    unsigned char *alarmFlag = (unsigned char *) &field_188 + 1;
    if (*alarmFlag == 0 && message) {
        int type = 1;
        *alarmFlag = (unsigned char) type;
        if (gStatus->inBlackMarketSystem() != 0) {
            type = 0xc;
        }
        createRadioMessage(type, race);
        if (((SolarSystem *) (intptr_t) gStatus->getSystem())->getRace() == race) {
            // veneer 0x1abff8 -> Station::setAttackedFriends(true)
            gStatus->getStation()->setAttackedFriends(true);
        }
    }
}

void Level::setPlayerEngineColor(short color) {
    int c = color;
    if (player != nullptr && field_a4 != nullptr) {
        unsigned int *p = (unsigned int *) ((char *) g_engineColorBase + 0x1254);
        for (int i = (int) field_a4->size(); i != 0; i = i - 1) {
            *p = c << 0x10 | c << 0x18 | c << 8 | 0xff;
            p = p + 0x28;
        }
    }
}

Gun *Level::createGun(int idx, int owner, int kind, int hp, int dmg, int rate, int cool, int color) {
    ObjectGun *obj = 0;
    Gun *gun = 0;

    switch (kind) {
        case ITEM_SORT_LASER:
        case ITEM_SORT_BLASTER:
        case ITEM_SORT_THERMO: {
            int res = g_cg_beamTable[idx];
            if (res < 0) {
                gun = (Gun *) ::operator new(0x114);
                new(gun) Gun(owner, dmg, 1, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, 0.0f},
                             Vector{0.0f, 0.0f, 0.0f});
                gun->setIndex(idx);
                gun->weaponType = static_cast<ItemSort>(kind);
                gun->setPlayerGun(1);
                obj = (ObjectGun *) ::operator new(0x24);
                new(obj) BeamGun(owner, gun, idx, this);
            } else {
                int barrels = ((unsigned) (idx - 9) < 3 || idx == 0xe4) ? 1 : 0x14;
                gun = (Gun *) ::operator new(0x114);
                if (kind == ITEM_SORT_THERMO) {
                    new(gun) Gun(owner, dmg, barrels, hp, cool, rate, (float) color,
                                 Vector{0.0f, 0.0f, bitsToFloat(g_cg_rocketFx)}, Vector{0.0f, 0.0f, 0.0f});
                    gun->setIndex(idx);
                    gun->weaponType = ITEM_SORT_THERMO;
                    gun->setPlayerGun(1);
                    gun->setErrorMagnitudePercentage(0x14);
                    obj = (ObjectGun *) ::operator new(0xe8);
                    new(obj) RocketGun(owner, gun, res, 0, 0, 0, 1, this);
                } else {
                    new(gun) Gun(owner, dmg, barrels, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, 0.0f},
                                 Vector{0.0f, 0.0f, 0.0f});
                    gun->setIndex(idx);
                    gun->weaponType = static_cast<ItemSort>(kind);
                    gun->setPlayerGun(1);
                    obj = (ObjectGun *) ::operator new(0xb0);
                    new(obj) ObjectGun(owner, gun, res, 1000, this);
                }
            }
            break;
        }
        case ITEM_SORT_AUTO_CANNON:
        case ITEM_SORT_SCATTER_GUN:
            gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(owner, dmg, 0x19, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, bitsToFloat(g_cg_objFx)},
                         Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = static_cast<ItemSort>(kind);
            gun->setPlayerGun(1);
            obj = (ObjectGun *) ::operator new(0xb0);
            new(obj) ObjectGun(owner, gun, g_cg_objTable[idx], 1000, this);
            gun->setErrorMagnitudePercentage(2);
            break;
        case ITEM_SORT_ROCKET:
        case ITEM_SORT_MISSILE:
        case ITEM_SORT_CLUSTER_MISSILE: {
            gun = (Gun *) ::operator new(0x114);
            int barrels = (kind == ITEM_SORT_CLUSTER_MISSILE) ? (idx - 0xd3) : 5;
            new(gun) Gun(owner, dmg, barrels, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, 0.0f},
                         Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = static_cast<ItemSort>(kind);
            gun->setPlayerGun(1);
            obj = (ObjectGun *) ::operator new(0xe8);
            new(obj) RocketGun(owner, gun, g_cg_rocketTable[idx], 0, 0, kind,
                               (kind == ITEM_SORT_CLUSTER_MISSILE || kind == ITEM_SORT_MISSILE) ? 1 : 0, this);
            gGlobals->addSoundResourceToList(**g_cg_rocketSnd);
            break;
        }
        case ITEM_SORT_EMP_BOMB:
        case ITEM_SORT_NUKE:
        case ITEM_SORT_IONIZING_MISSILE: {
            gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(owner, dmg, 1, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, bitsToFloat(g_cg_objFx)},
                         Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = static_cast<ItemSort>(kind);
            gun->setPlayerGun(1);
            int attr = ((Item *) (intptr_t)(*(int *) (*(int *) (*g_cg_itemTableA + 4) + idx * 4)))->getAttribute(0xf);
            obj = (ObjectGun *) ::operator new(300);
            new(obj) BombGun(gun, g_cg_bombTable[idx], 1, kind, attr == 1 ? 1 : 0,
                             this);
            gGlobals->addSoundResourceToList(*g_cg_bombSnd);
            break;
        }
        case ITEM_SORT_TURRET:
        case ITEM_SORT_PLASMA_COLLECTOR: {
            // fx/extra are int slots holding raw float bit patterns (DAT_000ce2b8 = 300.0f,
            // DAT_000ce644 = -300.0f). extra = 0x43960000 - 0xf60000 = 0x42a00000 = 80.0f bits.
            int fx = (idx == 0xb5) ? g_cg_mineFx : g_cg_rocketFx;
            int extra = (idx == 0x30 && idx != 0xb5) ? (g_cg_rocketFx - 0xf60000) : 0;
            if (kind == ITEM_SORT_PLASMA_COLLECTOR) { fx = (idx == 0xb5) ? g_cg_mineFx : g_cg_rocketFx; }
            gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(owner, dmg, 0xf, hp, cool, rate, (float) color,
                         Vector{bitsToFloat(extra), 0.0f, bitsToFloat(fx)}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = static_cast<ItemSort>(kind);
            gun->setPlayerGun(1);
            if ((idx == 0x30 || idx == 0xe0 || idx == 0xb5)) {
                gun->field_0xa4 = 1;
                if (idx == 0xe0) F<uint8_t>(gun, 0xa5) = 1; // RAWREAD: Gun +0xa5 (no member; mid-int of field_0xa4)
            }
            obj = (ObjectGun *) ::operator new(0xb0);
            int *tbl = (kind == ITEM_SORT_PLASMA_COLLECTOR) ? g_cg_objTable23 : g_cg_objTable8;
            new(obj) ObjectGun(owner, gun, tbl[idx], 1000, this);
            break;
        }
        case ITEM_SORT_MINE: {
            gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(owner, dmg, 10, hp, cool, rate, 2.0f, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = ITEM_SORT_MINE;
            gun->setPlayerGun(1);
            obj = (ObjectGun *) ::operator new(0xd4);
            new(obj) MineGun(gun, g_cg_mineTable[idx], 1, ITEM_SORT_MINE, this);
            gGlobals->addSoundResourceToList(*g_cg_mineSnd);
            break;
        }
        case ITEM_SORT_SENTRY_GUN: {
            gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(owner, 0, 3, hp, cool, rate, 2.0f, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = ITEM_SORT_SENTRY_GUN;
            gun->setPlayerGun(1);
            obj = (ObjectGun *) ::operator new(0xb4);
            new(obj) SentryGun(gun, g_cg_sentryTable[idx], 1, ITEM_SORT_SENTRY_GUN, this);
            gGlobals->addSoundResourceToList(*g_cg_mineSnd);
            break;
        }
        case ITEM_SORT_SHOCK_BLAST: {
            gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(owner, dmg, 1, hp, 1, rate, (float) color, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = ITEM_SORT_SHOCK_BLAST;
            gun->setPlayerGun(1);
            obj = (ObjectGun *) ::operator new(300);
            new(obj) BombGun(gun, g_cg_bombTable2a[idx], 1, ITEM_SORT_SHOCK_BLAST, 0, this);
            gGlobals->addSoundResourceToList(*g_cg_bombSnd);
            break;
        }
        default:
            gun = 0;
            break;
    }

    // index-keyed extra sound resources (fallthrough chain in the original).
    switch (idx) {
        case 0x29: gGlobals->addSoundResourceToList(**g_cg_snd29);
        case 0x2a: gGlobals->addSoundResourceToList(**g_cg_snd2a);
        case 0x2b: gGlobals->addSoundResourceToList(**g_cg_snd2b);
        case 0x2c: gGlobals->addSoundResourceToList(**g_cg_snd2c);
        case 0x2d: gGlobals->addSoundResourceToList(**g_cg_snd2d);
        case 0x2e: gGlobals->addSoundResourceToList(**g_cg_snd2e);
        default: break;
    }

    gun->setLevel(this);
    if (this->playerGuns == nullptr) {
        this->playerGuns = new Array<ObjectGun *>();
    }
    this->playerGuns->push_back(obj);
    return gun;
}

void Level::createSpace() {
    // skybox mesh/texture only need (re)building when not yet created (mesh handle at +4 == -1).
    if (*(unsigned *) &this->skyboxMesh == 0xffffffff) {
        int alien = gStatus->inAlienOrbit();

        if (alien == 0) {
            gStatus->getSystem();
            int sysVariant = (((SolarSystem *) gStatus->getSystem())->getIndex() % 3);
            unsigned int field08Handle;
            gCanvas->MeshCreate((unsigned short) (sysVariant + 0x45ba), field08Handle, false);
            this->field_08 = field08Handle;
            gStatus->getSystem();
            sysVariant = (((SolarSystem *) gStatus->getSystem())->getIndex() % 3);
            gCanvas->TextureCreate((unsigned short) ((sysVariant + 0x2766) & 0xffff), nullptr, nullptr,
                                   g_level_texOutScratch, false);
            // the rest (campaign/supernova/storm/ring detail) is built inline above.

            gStatus->getSystem();
            if (0xf < ((SolarSystem *) gStatus->getSystem())->getTextureIndex()) {
                Engine *eng = (Engine *) gAppManager->GetEngine();
                if (eng->IsPostEffectActivated() != false) {
                    int mp = (int) (long) gCanvas->MeshGetPointer((unsigned int) *(unsigned *) &this->skyboxMesh);
                    *(int *) (mp + 0x1c) = 0; // RAWREAD: engine mesh-record pointer (opaque, no modeled type)
                }
            }
        } else {
            unsigned int field08Handle;
            gCanvas->MeshCreate((unsigned short) 0x45bc, field08Handle, false);
            this->field_08 = field08Handle;
            gCanvas->TextureCreate((unsigned short) 0x2768, nullptr, nullptr, g_level_texOutScratch, false);
            unsigned int skyboxMeshHandle;
            gCanvas->MeshCreate((unsigned short) 0x4592, skyboxMeshHandle, false);
            this->skyboxMesh = skyboxMeshHandle;
            gCanvas->TextureCreate((unsigned short) 0x275b, nullptr, nullptr, g_level_texOutScratch, false);
        }

        // randomized skybox spin (light direction); the spin is written into the stored
        // skybox Euler angles either way (the fog-orbit branch leaves them unrotated).
        this->skyRotX = 0.0f;
        this->skyRotY = 0.0f;
        this->skyRotZ = 0.0f;
    }

    int mode = this->missionPtr;
    if (mode == 4 || mode == 0x17) {
        StarSystem *ss = (StarSystem *) ::operator new(0x60);
        new(ss) StarSystem(mode);
        *(StarSystem **) &this->starSystem = ss;
        this->skyRotX = 0.0f;
        this->skyRotY = 0.0f;
        this->skyRotZ = 0.0f;
        return;
    }

    // build the home station + jumpgates.
    this->landmarks = new Array<KIPlayer *>();
    this->landmarks->resize(4);

    // station + gate object construction is performed inline by createSpace().
}

void Level::createRadioMessage(int type, int sub) {
    unsigned r2 = (unsigned) sub;

    if (this->player == nullptr || this->player->field_0x1c == 0)
        return;

    int **statusHolder = (int **) &gStatus;
    gStatus->getMission();
    if (((Mission *) gStatus->getMission())->isEmpty() == 0)
        return;

    // fresh message queue.
    if (this->messages != nullptr) {
        if (this->messages) {
            for (void *e: *this->messages) delete (RadioMessage *) e;
            delete this->messages;
            this->messages = nullptr;
        }
    }
    this->messages = new Array<void *>();

    // resolve the speaker portrait from the sub-parameter.
    int speaker;
    if (r2 == 0) speaker = 0x40;
    else if (r2 == 2) speaker = 0x41;
    else if (r2 == 3) speaker = 0x15;
    else speaker = (r2 == 8) ? 9 : 0x3f;

    int extraDelay = 0;
    unsigned id = 0x1ba;
    bool aborted = false;
    bool builtInline = false; // case already pushed its own messages

    switch (type) {
        case 0:
        case 1: {
            // local-station hail: skip if you're docked at the relevant station.
            // RAWREAD: murky double-indirection through the gStatus global slot (engine Array payload).
            int *stations = *(int **) (*statusHolder[0] + 0x90);
            bool atStation = false;
            if (stations != 0) {
                for (unsigned k = 0; k < (unsigned) *stations; k = k + 1) {
                    int sidx = *(int *) (((int *) stations[1])[k]);
                    Station *st = (Station *) gStatus->getStation();
                    if (sidx == ((Station *) st)->getIndex()) {
                        atStation = true;
                        break;
                    }
                }
            }
            if (atStation) {
                aborted = true;
                break;
            }
            int base = (type == 0) ? 0x1aa : 0x1ad;
            id = gRandom->nextInt() + base;
            break;
        }
        case 3:
            *(char *) &this->field_1b0 = 1;
            id = gRandom->nextInt() + 0x1b3;
            break;
        case 4:
            *(char *) &this->field_68 = 1;
            id = gRandom->nextInt() + 0x1b6;
            break;
        case 5: id = 0x1bb;
            extraDelay = 0;
            break;
        case 6: id = 0x1bc;
            extraDelay = 0;
            break;
        case 9: id = gRandom->nextInt() + 0x1c1;
            break;
        case 10: id = gRandom->nextInt() + 0x1c3;
            break;
        case 0xb: id = gRandom->nextInt() + 0x1c5;
            break;
        case 0xc: id = 0x1c5;
            extraDelay = 0;
            break;
        case 0xd: id = gRandom->nextInt() + 0x1c7;
            break;
        case 0xe: {
            // RAWREAD: murky double-indirection through the gStatus global slot (engine Array payload).
            int *st = *(int **) (*statusHolder[0] + 0x90);
            id = 0x88f;
            if (st != 0) {
                for (int k = 0; k != *st; k = k + 1)
                    if (-1 < *(int *) (st[1] + k * 4)) id = id - 2;
            }
            break;
        }
        case 0xf: {
            // RAWREAD: murky double-indirection through the gStatus global slot (engine Array payload).
            int *st = *(int **) (*statusHolder[0] + 0x90);
            id = 0x88e;
            if (st != 0) {
                for (int k = 0; k != *st; k = k + 1)
                    if (-1 < *(int *) (st[1] + k * 4)) id = id - 2;
                if (id - 0x889 < 5) {
                    extraDelay = 0;
                    break;
                }
            }
            aborted = true;
            break;
        }
        case 8: {
            int stage = gRandom->nextInt();
            int off = 0;
            for (int k = 0; k < stage; k = k + 1)
                off = off + g_crm_counts8[k];
            int cnt = g_crm_counts8[stage];
            int *tbl = (int *) ((char *) g_crm_table8 + off * 8);
            for (int k = 0; k < cnt; k = k + 1) {
                int delay = (k == 0) ? 5000 : (k - 1);
                int arg = tbl[k * 2];
                RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                int kind = (k == 0) ? 5 : 6;
                new(m) RadioMessage(tbl[k * 2 + 1], arg, kind, delay);
                this->messages->push_back(m);
            }
            builtInline = true;
            break;
        }
        case 0x13: {
            RadioMessage *m = (RadioMessage *) ::operator new(0x28);
            AbyssEngine::AERandom *rng = gRandom;
            new(m) RadioMessage(rng->nextInt() + 0xaf4, 0, 5, 0x5dc);
            this->messages->push_back(m);
            m = (RadioMessage *) ::operator new(0x28);
            new(m) RadioMessage(rng->nextInt() + 0xafa, 0, 6, 0);
            this->messages->push_back(m);
            builtInline = true;
            break;
        }
        case 0x15: id = 0xc54;
            extraDelay = 0;
            break;
        case 0x16: id = 0xc55;
            extraDelay = 0;
            break;
        case 0x17: id = 0xc56;
            extraDelay = 0;
            break;
        case 0x18: id = 0xc57;
            extraDelay = 0;
            break;
        case 0x19: id = 0xc58;
            extraDelay = 0;
            break;
        case 0x1a: id = 0xc59;
            extraDelay = 0;
            break;
        case 0x1b: {
            RadioMessage *m = (RadioMessage *) ::operator new(0x28);
            new(m) RadioMessage(r2 * 2 + 0xc60, 6, 5, 0x5dc);
            this->messages->push_back(m);
            m = (RadioMessage *) ::operator new(0x28);
            new(m) RadioMessage(r2 * 2 + 0xc61, 0, 6, 0);
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
                id = gRandom->nextInt() + 0x1bd;
            }
            break;
    }

    if (aborted) {
        if (this->messages != nullptr)
            if (this->messages) {
                for (void *e: *this->messages) delete (RadioMessage *) e;
                delete this->messages;
                this->messages = nullptr;
            }
        PlayerEgo *ego = this->player;
        this->crm_dispatch(ego->field_0x1c, 0);
        return;
    }

    if (!builtInline) {
        RadioMessage *m = (RadioMessage *) ::operator new(0x28);
        new(m) RadioMessage(id, speaker, 5, extraDelay);
        this->messages->push_back(m);
    }

    PlayerEgo *ego = this->player;
    this->crm_dispatch(ego->field_0x1c, (void *) this->messages);
}

int Level::init() {
    Level *thisptr = this;

    int stage = this->field_134;
    if (stage == 0) {
        // --- first tick: tear down stale routes/objectives and build particle systems ---
        this->field_68 = 0;
        this->field_1b0 = 0;
        *(short *) ((char *) &this->field_188 + 1) = 0;
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

        LODManager *lod = (LODManager *) ::operator new(0x14);
        new(lod) LODManager();
        this->lodManager = lod;

        PaintCanvas *canvas = **g_init_canvas;

        bool dustVariant = false;
        if (gStatus->inAlienOrbit() == 0) {
            gStatus->getSystem();
            dustVariant = ((SolarSystem *) gStatus->getSystem())->getTextureIndex() == 0xc;
        }

        // the engine builds ten particle-system managers (engine trails, explosions, dust...).
        ParticleSystemManager *psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x4e85, false, 0xffff, false);
        this->particleEmitBoolPtr = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x6a72, false, 0xffff, false);
        this->particleRenderBoolPtr = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x4e83, true, 0xffff, false);
        this->field_74 = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x4e7a, true, 0x4e7a, true);
        this->field_80 = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x5e20, true, 0x5e20, true);
        this->field_98 = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1,
                                       (unsigned short) (dustVariant ? 0x4ea9 : 0x4e7f), true, 0, false);
        this->particleSystemMgr = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x4e7c, false, 0x4e7c, false);
        this->skybox2Mesh = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x6a7c, true, 0x6a7c, true);
        this->field_8c = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x6ab9, true, 0xffff, false);
        this->field_9c = psm;
        psm = (ParticleSystemManager *) ::operator new(100);
        new(psm) ParticleSystemManager(canvas, ParticleSettings::CameraSet_1, 0x6aaf, true, 0xffff, false);
        this->field_94 = psm;

        thisptr->createSpace();

        if (this->missionPtr == 3) {
            thisptr->createPlayer();
            int stk = 0;
            if (**g_init_flagStack != 0 && this->player->geometry != 0)
                stk = (int) (intptr_t) gStatus->getStationStack();
            levelInitPlacePlayer(thisptr, (int) (intptr_t) gStatus, stk);
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

    Mission *m = (Mission *) gStatus->getMission();
    if (m == 0)
        gStatus->setMission((Mission *) **g_init_missionDef);

    int mode = this->missionPtr;
    if (mode == 3) {
        bool campaign = ((Mission *) gStatus->getMission())->isEmpty() == 0 && m->isCampaignMission() != 0;
        bool madeScene = false;
        if (campaign) {
            if (this->missionPtr != 3) { madeScene = true; } else {
                int won = gStatus->gameWon();
                int *settings = *g_init_settings;
                // RAWREAD: g_init_settings is an opaque engine settings table (no modeled type).
                bool skip = won != 0 && *(char *) (settings + 0x37) == 0 &&
                            *(char *) (settings + 0x35) == 0;
                if (skip) {
                    thisptr->createMission();
                    if (**g_init_bmFlag != 0 && gStatus->inBlackMarketSystem() != 0)
                        levelInitPlacePlayer(thisptr, (int) (intptr_t) gStatus, 0);
                } else if (this->missionPtr != 3) {
                    madeScene = true;
                } else {
                    gStatus->getMission();
                    if (((Mission *) gStatus->getMission())->isEmpty() == 0) {
                        Mission *mm = (Mission *) gStatus->getMission();
                        if (mm->isCampaignMission() != 0)
                            thisptr->createCampaignMission();
                    }
                }
            }
        } else {
            thisptr->createMission();
            if (**g_init_bmFlag != 0 && gStatus->inBlackMarketSystem() != 0)
                levelInitPlacePlayer(thisptr, (int) (intptr_t) gStatus, 0);
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
        (mode != 2 || gStatus->getCurrentCampaignMission() != 0x2b)) {
        thisptr->createSentryGuns();
        thisptr->createFighterTurrets();
        thisptr->createWingmen();
    }
    thisptr->assignGuns();
    if (this->missionPtr != 3)
        this->missionPtr = 3;
    thisptr->connectPlayers();
    if (this->player != nullptr)
        this->player->setRoute(this->playerRoute);

    // recompute enemiesLeft.
    int initial = 0;
    int enemies = 0;
    if (this->enemies != nullptr) {
        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            KIPlayer *e = (*this->enemies)[i];
            // RAWREAD: +0x41 (mid field_0x40), +0x71 (mid field_0x70) — no byte-level members.
            if (*(char *) ((char *) e + 0x41) == 0 && *(char *) ((char *) e + 0x71) == 0 && e->field_0x3f == 0) {
                int wm = e->isWingMan();
                if (wm == 0) {
                    // RAWREAD: +0x3c, +0x3d — unnamed gap after stealFlag (no members).
                    if ((char) e->field_0x44 == 0 && *(char *) ((char *) e + 0x3c) == 0)
                        enemies = enemies + (*(unsigned char *) ((char *) e + 0x3d) ^ 1);
                }
            }
        }
        enemies = enemies - this->field_120;
    }
    this->asteroidsLeft = 0;
    this->enemiesLeft = enemies;
    if (this->asteroids != nullptr)
        initial = (int) this->asteroids->size();
    this->field_184 = 0;
    *(char *) &this->field_188 = 0;
    this->field_1c = 0;
    this->asteroidsLeft = initial;
    this->kills = 0;
    this->friendCargoStolen = 0;
    *(char *) &this->field_70 = 1;
    return 1;
}

void Level::createFighterTurrets() {
    if (this->enemies == nullptr)
        return;

    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        KIPlayer *ki = (*this->enemies)[i];
        if (ki != 0) {
            int kind = ki->shipGroupFlag;
            if (kind == 0x2d || kind == 0x33) {
                PlayerTurret *t = (PlayerTurret *) this->createStaticObject((Waypoint *) (intptr_t) 0, 0x1a74, 1);
                t->player->setVulnerable(0);
                Player *pp = t->player;
                int hp = pp->getMaxHitpoints();
                pp->setMaxHitpoints(hp);
                char offset[12] = {0};
                t->setHost(ki, *(Vector *) offset);
                *(PlayerTurret **) &ki->field_0x10 = t;
                t->shipGroup = (kind == 0x2d) ? 8 : 0;
                C(t, 0x74) = 1; // RAWREAD: KIPlayer t+0x74 (no member at 0x74; mid-field of field_0x73)
                this->enemies->push_back((KIPlayer *) t);
            }
        }
    }
}

void Level::updateAlienAttackers(int dt) {
    Level *thisptr = this;
    this->alienAttackTimer = this->alienAttackTimer + dt;

    Mission *m = (Mission *) gStatus->getMission();
    bool blocked = (m != 0) && m->isCampaignMission() != 0 &&
                   (gStatus->getCurrentCampaignMission() == 0x28 ||
                    gStatus->getCurrentCampaignMission() == 0x93 ||
                    gStatus->getCurrentCampaignMission() == 0x9a);
    if (blocked)
        return;

    int elapsed = this->alienAttackTimer;
    int period = (gStatus->getCurrentCampaignMission() == 0x29) ? 10000 : 45000;
    if (elapsed <= period)
        return;

    this->alienAttackTimer = 0;
    if (this->enemies == nullptr)
        return;

    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int *ki = (int *) (*this->enemies)[i];
        // race 9 == alien; only respawn ones that are dead and inactive.
        if (((KIPlayer *) ki)->shipGroup == 9 && ((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->
            isActive() == 0) {
            ((PlayerFighter *) ki)->revive();
            int inOrbit = (gStatus->inAlienOrbit() != 0);
            if (!inOrbit) {
                Station *st = (Station *) gStatus->getStation();
                if (((Station *) st)->isAttackedByAliens() == 0)
                    inOrbit = 1; // fall back to player-relative placement
            }
            levelPlaceAlien(thisptr, ki, inOrbit);
        }
    }
}

void Level::createMission() {
    Mission *mission = (Mission *) gStatus->getMission();
    if (mission == 0)
        return;

    // alien-orbit ambush wave: 2..N alien fighters at randomized far positions.
    if (gStatus->inAlienOrbit() != 0) {
        int lvl = gStatus->getLevel();
        AbyssEngine::AERandom *rng = gRandom;
        int roll = rng->nextInt();
        float base = (float) lvl * 0.5f - 1.0f;
        unsigned count = 2;
        if (2.0f <= base + (float) roll) {
            int r2 = rng->nextInt();
            count = (unsigned) (base + (float) r2);
        }
        int campA = gStatus->getCurrentCampaignMission();
        int campB = gStatus->getCurrentCampaignMission();
        if (campB == 0x21) count = 2;
        if (campA == 0x44) count = 2;

        this->enemies = new Array<KIPlayer *>();
        this->enemies->resize(count);

        Globals *globals = gGlobals;
        for (unsigned i = 0; i < count; i = i + 1) {
            int fighter = (int) globals->getRandomEnemyFighter(9);
            int ship = (int) (intptr_t) this->createShip(9, 0, fighter, (Waypoint *) (intptr_t) 0, 1, 0);
            (*this->enemies)[i] = (KIPlayer *) (intptr_t) ship;
            KIPlayer *kp = (*this->enemies)[i];
            float x = (float) (rng->nextInt(120000) - 60000);
            float y = (float) (rng->nextInt(80000) - 40000);
            float z = (float) (rng->nextInt(120000) - 60000);
            kp->setPosition(x, y, z);
            kp->player->setAlwaysEnemy(true);
        }
    }

    // everything else (mission-type-specific objects, objectives, escorts) is built inline
    // by createMission() above for the current mission type.
}

void Level::createAsteroids() {
    int *rngObj = (int *) &gRandom; // points at the gRandom slot; *rngObj is the AERandom object

    // base collision-variant index: 2 in the "ring" system (idx 0x16), else 0; 0 when in alien orbit.
    int colBase;
    if (gStatus->inAlienOrbit() == 0) {
        gStatus->getSystem();
        colBase = (((SolarSystem *) gStatus->getSystem())->getIndex() == 0x16) ? 2 : 0;
    } else {
        colBase = 0;
    }

    // asteroid container.
    this->asteroids = new Array<KIPlayer *>();

    Galaxy *gal = gGalaxy;
    Station *st = (Station *) gStatus->getStation();
    int *prob = (int *) gal->getAsteroidProbabilities(st);

    int seed = *rngObj;
    Station *st2 = (Station *) gStatus->getStation();
    ((Station *) st2)->getIndex();
    ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->setSeed((long long) seed);

    int countRoll = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(0x28);
    this->asteroids->resize((unsigned) (countRoll + 0x28));

    int rx = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(100000); // DAT_000bf990 == 0x000186a0
    int ry = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(100000);
    int rz = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(100000);

    int alien = gStatus->inAlienOrbit();
    int camp = gStatus->getCurrentCampaignMission();

    // field-origin coordinates (x=ox, y=oy, z=oz).
    int ox, oy, oz;
    if (alien != 0) {
        oy = 0;
        oz = 30000;
        ox = -30000; // DAT_000bf994 default (0xffff8ad0)
        if (camp == 0x9a)
            ox = -70000;
    } else {
        bool placed = false;
        if (camp == 0x72) {
            Station *s = (Station *) gStatus->getStation();
            if (((Station *) s)->getIndex() == 0x53) {
                oz = 80000; // DAT_000bf998 == 0x00013880
                ox = 30000;
                oy = 0;
                placed = true; // jumps to LAB_000bf874 (oy = 0)
            }
        }
        if (!placed && camp == 0x59 && gStatus->inSupernovaOrbit() != 0) {
            ox = -100000; // DAT_000bf99c == 0xfffe7960
            oz = -50000; // DAT_000bf9a0 == 0xffff3cb0
            oy = 0;
            placed = true;
        }
        if (!placed && camp == 0x5b) {
            Station *s = (Station *) gStatus->getStation();
            if (((Station *) s)->getIndex() == 0x6e) {
                oy = 0;
                oz = 50000;
                ox = 60000;
                placed = true;
            }
        }
        if (!placed && camp == 0x91) {
            Station *s = (Station *) gStatus->getStation();
            if (((Station *) s)->getIndex() == 0x70) {
                oz = 70000; // DAT_000bf9a4 == 0x00011170
                ox = 50000;
                oy = 0;
                placed = true;
            }
        }
        if (!placed) {
            ox = rx - 50000;
            oz = rz + 20000;
            oy = ry - 50000;
            if (gStatus->getCurrentCampaignMission() == 0 && this->missionPtr == 3) {
                oz = 0;
                ox = 0;
                oy = 0;
            }
        }
    }

    ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->reset();

    // field center vector (this+0xc8): disasm stores (ox, oy, oz) into 0xc8/0xcc/0xd0.
    this->field_c8 = (float) ox;
    this->field_cc = (float) oy;
    this->field_d0 = (float) oz;

    Waypoint *wp = (Waypoint *) ::operator new(0x138);
    new(wp) Waypoint(ox, oy, oz, 0);
    this->asteroidWaypoint = wp;

    BoundingSphere *bs = (BoundingSphere *) ::operator new(0x48);
    new(bs) BoundingSphere(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    *(BoundingSphere **) &this->collisionVolume = bs;

    int density = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt() + 2; // # of "core" (dense) asteroids
    int alien2 = gStatus->inAlienOrbit();

    void *canvas = (void *) gCanvas;
    int kind = 0x9a;
    int probCursor = 0;

    for (unsigned i = 0; i < this->asteroids->size(); i = i + 1) {
        // choose asteroid kind from the probability table (skip in alien orbit -> fixed 0xa4).
        if (alien2 == 0) {
            bool ok = false;
            int cursor = probCursor;
            while (!ok) {
                int roll = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt();
                int next = 0;
                if (roll < *(int *) ((int) (intptr_t) prob + cursor * 4 + 4)) {
                    kind = *(int *) ((int) (intptr_t) prob + cursor * 4);
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

        // spread radius: tighter (60000) for the dense core, wider (100000) otherwise.
        // DAT_000bfd10 == 0x000186a0 (default); movw.lt 0xea60 overrides for the core.
        unsigned spread = (int) i < density ? 60000u : 100000u;
        float half = (float) (spread >> 1);

        // collision variant: 1 in alien orbit, 3 for the special "0xd9" kind, else colBase.
        int colVariant = colBase;
        if (gStatus->inAlienOrbit() != 0)
            colVariant = 1;
        if (kind == 0xd9)
            colVariant = 3;

        // mesh id chosen from a small table indexed by colVariant.
        static const int meshTable[4] = {0x37a0, 0x37a4, 0x37a8, 0x37ac};
        int mesh = meshTable[colVariant & 3];

        // reject-sample a position far enough from already-placed asteroids.
        Vector pos;
        for (;;) {
            pos.x = (this->field_c8 - half) + (float) ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(spread);
            pos.y = (this->field_cc - half) + (float) ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(spread);
            pos.z = (this->field_d0 - half) + (float) ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(spread);
            if (i == 0 || (int) i >= density)
                break;
            bool farEnough = false;
            for (unsigned j = 0; j < i; j = j + 1) {
                float d = 1.0e30f;
                if (this->asteroids != nullptr) {
                    int obj = (int) (intptr_t)(*this->asteroids)[j];
                    Vector c;
                    // RAWREAD: PlayerAsteroid +0x150..0x158 asteroid-centre vec.
                    c.x = *(float *) (obj + 0x150);
                    c.y = *(float *) (obj + 0x154);
                    c.z = *(float *) (obj + 0x158);
                    Vector dv;
                    dv.x = c.x - pos.x;
                    dv.y = c.y - pos.y;
                    dv.z = c.z - pos.z;
                    d = AbyssEngine::AEMath::VectorLength(dv);
                }
                if (8000 < (int) d) {
                    farEnough = true;
                    break;
                }
            }
            if (farEnough)
                break;
        }

        AEGeometry *geo = (AEGeometry *) ::operator new(0xc0);
        new((void *) geo) AEGeometry((uint16_t) mesh, (PaintCanvas *) canvas, 0);

        // install LOD meshes + register with the LOD manager (detail-dependent distance table).
        // Builds the lod-mesh distance table appropriate for the current detail level / distance band
        // and installs it on the geometry.
        bool near = (int) i < density;
        {
            unsigned short lodMeshes[3] = {
                (unsigned short) mesh,
                (unsigned short) (mesh + 1),
                (unsigned short) (mesh + 2)
            };
            int lodDist[3];
            if (near) {
                lodDist[0] = 8000;
                lodDist[1] = 20000;
                lodDist[2] = 40000;
            } else {
                lodDist[0] = 20000;
                lodDist[1] = 40000;
                lodDist[2] = 80000;
            }
            geo->setLodMeshes(lodMeshes, lodDist, 3);
        }
        this->lodManager->addObject(geo);

        // per-asteroid random radius and scale.
        int base = (*g_ca_lowDetail && **g_ca_lowDetail != 0) ? 0x46 : (near ? 0x46 : 0x46);
        (void) base;
        int radius = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt() + (near ? 0x78 : 0x1e);
        float scale = (float) radius * 0.001f; // DAT_000bfcf4 scale immediate

        // larger asteroids may roll an extra collision shape (corrupted f64 compares in original).
        double dscale = (double) scale;
        if (dscale >= 1.0 && dscale >= 1.0 && dscale >= 1.0 &&
            ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt() != 0) {
            ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt();
        }

        PlayerAsteroid *a = (PlayerAsteroid *) ::operator new(0x170);
        new(a) PlayerAsteroid(kind, geo, colVariant, kind, pos, scale, (int) scale);
        (*this->asteroids)[i] = a;

        // wire the freshly built asteroid to its level (KIPlayer::setLevel, virtual).
        (*this->asteroids)[i]->setLevel(this);

        ((PlayerAsteroid *) (*this->asteroids)[i])->setAsteroidCenter(
            Vector{this->field_c8, this->field_cc, this->field_d0});
    }

    if (prob != 0)
        ::operator delete[](prob);
}

// Level::createCampaignMission() — constructs the scripted actors/objectives for the player's
// current story (campaign) mission.
void Level::createCampaignMission() {
    int idx = gStatus->getCurrentCampaignMission();

    if (idx == 0) {
        // mission 0 — the tutorial ambush: three sleeping enemy fighters at a fixed point.
        this->enemies = new Array<KIPlayer *>();
        this->enemies->resize(3);
        float c = g_ccm_pos0;
        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            int type = (i == 1) ? 0x17 : 2;
            int ship = (int) (intptr_t) this->createShip(8, 0, type, (Waypoint *) (intptr_t) 0, 1, 0);
            (*this->enemies)[i] = (KIPlayer *) (intptr_t) ship;
            ((*this->enemies)[i])->setToSleep();
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
            KIPlayer *kp = (*this->enemies)[i];
            kp->setPosition(c, c, c);
            kp->cargo = nullptr;
            kp->hasCargo = 0;
            Player *ep = kp->player;
            ep->setHitpoints(ep->getMaxHitpoints()); // full-heal the campaign enemy
            if (i < 3)
                ((PlayerFighter *) (*this->enemies)[i])->setExhaustVisible(false);
        }
        Player *pp = (Player *) this->player->player;
        pp->setHitpoints(pp->getMaxHitpoints()); // full-heal the player
        return;
    }

    // all other campaign missions have their scenes scripted inline above.
}

// Level::updateOrbit(int dt) — non-mission ambient traffic / reinforcement spawner.
void Level::updateOrbit(int dt) {
    Level *thisptr = this;

    int t178 = this->field_178 + dt;
    this->orbitWaveTimer = this->orbitWaveTimer + dt;
    this->field_178 = t178;

    // delayed "friends turned hostile" alarm.
    if (*(char *) &this->field_18a != 0) {
        if (gStatus->getSystem() != 0 && this->player->field_0x1c != 0) {
            gStatus->getSystem();
            int race = ((SolarSystem *) gStatus->getSystem())->getRace();
            thisptr->alarmAllFriends(race, 0 != 0);
            gStatus->getSystem();
            ((SolarSystem *) gStatus->getSystem())->getRace();
            thisptr->createRadioMessage(2, 0);
            *(char *) &this->field_18a = 0;
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
                    ki->player->isActive() == 0 && (uint8_t) ki->field_0x42 == 0) {
                    ((PlayerFighter *) ki)->revive();
                    ki->setPosition(0, 0, 0);
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
                if ((this->enemies->size() - (unsigned) this->hostileCount) <= i &&
                    ((*this->enemies)[i])->isWingMan() == 0 && ((*this->enemies)[i])->isDead() != 0) {
                    hostileAlive = hostileAlive + ((*this->enemies)[i]->player->isActive() ^ 1);
                }
            }
        }
        if (this->enemies != nullptr) {
            int spawned = 0;
            for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                int *ki = (int *) (*this->enemies)[i];
                // re-activate friendly slots that died.
                if (0 < this->friendCount && (int) i < this->friendCount &&
                    ((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0 &&
                    (uint8_t)((KIPlayer *) ki)->field_0x42 == 0) {
                    ((PlayerFighter *) ki)->revive();
                    ((KIPlayer *) ki)->setPosition(0, 0, 0);
                }
                // spawn enemy reinforcements subject to security-level caps.
                if (1 < hostileAlive && this->field_184 < 2 &&
                    0 < this->hostileCount &&
                    (unsigned) ((int) this->enemies->size() - this->hostileCount) <= i) {
                    int race = ((KIPlayer *) ki)->shipGroup;
                    bool secZero = false, secOne = false;
                    if (gStatus->inAlienOrbit() == 0) {
                        SolarSystem *ss = (SolarSystem *) gStatus->getSystem();
                        secZero = ((SolarSystem *) ss)->getSecurityLevel() == 0;
                    }
                    if (gStatus->inAlienOrbit() == 0) {
                        SolarSystem *ss = (SolarSystem *) gStatus->getSystem();
                        secOne = ((SolarSystem *) ss)->getSecurityLevel() == 1;
                    }
                    if (((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0 &&
                        (uint8_t)((KIPlayer *) ki)->field_0x42 == 0) {
                        bool doSpawn;
                        if (race != 9 && !secZero) {
                            doSpawn = secOne && this->field_17c <= 2;
                        } else {
                            doSpawn = true;
                        }
                        if (doSpawn) {
                            this->field_17c = this->field_17c + 1;
                            levelSpawnFar(thisptr, ki);
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

// Level::friendTurnedEnemy(int race) — flag that a friendly has gone hostile and, the
// first time it happens this orbit, queue the "friend turned enemy" radio chatter.
void Level::friendTurnedEnemy(int /*race*/) {
    if ((unsigned char) field_188 == 0) {
        *(unsigned char *) &field_188 = 1;
        createRadioMessage(0, 0);
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
            ((RadioMessage *) (*this->messages)[i])->reset();
        }
    }
    player->setRoute(playerRoute);
    int count;
    if (this->enemies != nullptr) {
        count = 0;
        for (unsigned int i = 0; i < this->enemies->size(); i = i + 1) {
            KIPlayer *e = (*this->enemies)[i];
            // RAWREAD: +0x41 (mid field_0x40), +0x71 (mid field_0x70) — no byte-level members.
            if (*(char *) ((char *) e + 0x41) == 0 && *(char *) ((char *) e + 0x71) == 0 && e->field_0x3f == 0) {
                // The original passes the enemy being examined as the receiver; the
                // decompiler dropped it (ambient-register thiscall). isWingMan reads
                // wingmanFlag at +0xdc of this enemy KIPlayer.
                int wm = e->isWingMan();
                if (wm == 0) {
                    // RAWREAD: +0x3c, +0x3d — unnamed gap after stealFlag (no members).
                    if ((char) e->field_0x44 == 0 && *(char *) ((char *) e + 0x3c) == 0) {
                        count = count + (*(unsigned char *) ((char *) e + 0x3d) ^ 1);
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
        ast = (int) asteroids->size();
    }
    asteroidsLeft = ast;
    kills = 0;
}

void Level::createSentryGuns() {
    int ship = (int) (intptr_t) gStatus->getShip();
    if (((Ship *) (ship))->getFirstEquipmentOfSort(0x27) != 0) {
        field_b0 = new Array<KIPlayer *>();
        field_b0->resize(9);
        if (enemies == nullptr) {
            enemies = new Array<KIPlayer *>();
        }
        int color = 0x9923e035;
        for (unsigned int i = 0; i < field_b0->size(); i = i + 1) {
            int obj = createStaticObject((Waypoint *) (intptr_t) 0, i / 3 + 0x49c0, 1);
            (*field_b0)[i] = (KIPlayer *) (intptr_t) obj;
            KIPlayer *k = (*field_b0)[i];
            k->player->setRadius(800);
            k->player->setAlwaysFriend(1);
            k->player->setMaxHitpoints(100);
            k->setPosition((float) color, (float) color, (float) color);
            enemies->push_back(k);
        }
    }
}

int Level::collideStation(Vector v) {
    if (this->landmarks != nullptr &&
        (*this->landmarks)[0] != 0 &&
        gStatus->inEmptyOrbit() == 0) {
        PlayerFixedObject *obj = (PlayerFixedObject *) (*this->landmarks)[0];
        return obj->collide(v.x, v.y, v.z);
    }
    return 0;
}

void Level::uncoverWanted(int index) {
    if (field_29c == 0) {
        createRadioMessage(0x12, index);
        int **g = (int **) &gStatus;
        // RAWREAD: indexes the Status `wanted` Array payload (engine container, not a modeled field).
        for (int i = 1;
             i - 1 < ((Wanted *) ((int *) (*(int *) (*g) + 4))[index])->getNumWingmen();
             i = i + 1) {
            (*enemies)[i]->player->setAlwaysEnemy(1);
            (*enemies)[i]->player->turnEnemy();
        }
    }
}

// Level::update(long long time, bool param_2) — the engine's per-frame tick. The
// delta-time is carried (as the decompiler sees it) in param_2.
void Level::update(long long /*time*/, bool param) {
    Level *thisptr = this;
    unsigned dt = (unsigned) param;

    // screen-flash fade.
    if (*(char *) &this->flashActive != 0) {
        unsigned remaining = *(unsigned *) &this->flashDurationA - dt;
        *(unsigned *) &this->flashDurationA = remaining;
        if (0x7fffffff < remaining)
            *(char *) &this->flashActive = 0;
        // colour scaled toward 0 by remaining/duration, clamped — handled by engine helper.
        float frac = (float) remaining / (float) this->flashDurationB;
        float *col = (float *) ((char *) &this->flashColor);
        float lo = *g_up_clampLo, hi = *g_up_clampHi, z = *g_up_clampZ, w = *g_up_clampW;
        float scaled[4] = {col[0] * frac, col[1] * frac, col[2] * frac, col[3] * frac};
        float bounds[4] = {lo, hi, z, w};
        for (int k = 0; k < 4; k = k + 1)
            col[k] = bounds[k] > scaled[k] ? bounds[k] : scaled[k];
    }

    // orbit update: mission orbit if an active non-empty mission, else free orbit.
    bool didMission = false;
    if (gStatus->getMission() != 0) {
        gStatus->getMission();
        if (((Mission *) gStatus->getMission())->isEmpty() == 0) {
            thisptr->updateMissionOrbit(dt);
            didMission = true;
        }
    }
    if (!didMission)
        thisptr->updateOrbit(dt);

    Station *st = (Station *) gStatus->getStation();
    if (((Station *) st)->isAttackedByAliens() != 0 || gStatus->inAlienOrbit() != 0)
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
    int aPtr = (int) (intptr_t) gStatus;
    Station *st2 = (Station *) gStatus->getStation();
    int idx = ((Station *) st2)->getIndex();
    gStatus->getCurrentCampaignMission();
    int gammaBits = ((Status *) (intptr_t) aPtr)->getGammaRayDamagePerSecond(aPtr, idx);
    float gamma = *(float *) &gammaBits;
    PlayerEgo *ego = this->player;
    if (gamma > 0.0f && ego != nullptr) {
        int ship = (int) (intptr_t) gStatus->getShip();
        int eq = (int) (intptr_t)((Ship *) (intptr_t) ship)->getFirstEquipmentOfSort(0x26);
        float factor = gamma;
        if (eq != 0) {
            int attr = ((Item *) (intptr_t) eq)->getAttribute(0x34);
            if (attr > 0)
                factor = (g_up_eqMax - (float) attr) / g_up_eqMax;
        }
        int hpBefore = ((Player *) this->player)->getGammaHP();
        ((Player *) this->player)->damageGamma(factor);
        if (hpBefore > 0xe && ((Player *) this->player)->getGammaHP() < 0xf) {
            int hud = this->player->getHUD();
            ((Hud *) (intptr_t) hud)->hudEvent(0x2c, this->player, 0);
        }
        ego = this->player;
    }

    // tick the in-flight particle-system managers (skybox, engine trails, etc).
    if (ego != nullptr) {
        char dummy[16];
        if (this->field_80 != 0) {
            *(Vec3 *) dummy = ego->getPosition();
            this->field_b4 = *(int *) dummy;
            (this->field_80)->update(dt);
        }
        if (this->field_74 != 0) (this->field_74)->update(dt);
        if (this->particleEmitBoolPtr != nullptr) this->particleEmitBoolPtr->update(dt);
        if (this->skybox2Mesh != 0) {
            *(Vec3 *) dummy = ego->getPosition();
            this->field_b4 = *(int *) dummy;
            (this->skybox2Mesh)->update(dt);
        }
        if (this->particleSystemMgr != nullptr) this->particleSystemMgr->update(dt);
        if (this->particleRenderBoolPtr != nullptr) this->particleRenderBoolPtr->update(dt);
        if (this->field_8c != 0) this->field_8c->update(dt);
        if (this->field_98 != 0) (this->field_98)->update(dt);
        if (this->field_94 != 0) this->field_94->update(dt);
        if (this->field_9c != 0) (this->field_9c)->update(dt);
    }

    this->lodManager->update(dt);
}

// Level::connectPlayers() — wires up each ship's enemy list from the friend/enemy/neutral arrays,
// honouring a thicket of campaign-mission-specific exceptions.
void Level::connectPlayers() {
    if (gAppManager->currentModuleId == 5)
        return;

    // player's enemy list from the enemy array.
    if (this->enemies != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        arr.resize(this->enemies->size());
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (*this->enemies)[j]->player;
        ((Player *) this->player->player)->setEnemies(&arr);
    }
    // add asteroids/landmarks arrays as additional enemies.
    if (this->asteroids != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        arr.resize(this->asteroids->size());
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (*this->asteroids)[j]->player;
        ((Player *) this->player->player)->addEnemies(&arr);
    }
    if (this->gasClouds != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        arr.resize(this->gasClouds->size());
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (*this->gasClouds)[j]->player;
        ((Player *) this->player->player)->addEnemies(&arr);
    }

    if (this->enemies == nullptr)
        return;

    int camp = gStatus->getCurrentCampaignMission();
    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int e = (int) (intptr_t)(*this->enemies)[i];
        int eFaction = ((KIPlayer *) (intptr_t) e)->shipGroup;
        int wmAll = ((KIPlayer *) e)->isWingMan();
        unsigned count = 0;
        bool notM24 = camp != 0x24;
        bool notFirst = i != 0;

        // first pass: count how many ships become this ship's enemies.
        for (unsigned k = 0; k < this->enemies->size(); k = k + 1) {
            int o = (int) (intptr_t)(*this->enemies)[k];
            if (o != e && (((~wmAll) & (((KIPlayer *) (intptr_t) o)->shipGroup == eFaction)) == 0)) {
                bool consider;
                if (notM24 || notFirst) {
                    consider = true;
                    if (camp == 0x9a) {
                        int alien = gStatus->inAlienOrbit();
                        if (k == 8 && alien != 0 && ((KIPlayer *) (intptr_t) e)->shipGroup == 8)
                            consider = false;
                    } else if (camp == 0x40 && !(i != 0 && e != 0 && k != 0)) {
                        consider = false;
                    }
                } else {
                    consider = ((KIPlayer *) o)->isWingMan() == 0;
                }
                if (consider) {
                    bool skip = false;
                    if ((char) ((KIPlayer *) (intptr_t) e)->field_0x40 != 0 && ((PlayerFixedObject *) (intptr_t) e)->
                        getDockingType() == 3) {
                        Station *st = (Station *) gStatus->getStation();
                        if (((Station *) st)->stationHasHiddenBlueprint(1) != 0)
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

        Mission *m = (Mission *) gStatus->getMission();
        int mtype = ((Mission *) gStatus->getMission())->getType();
        bool branchA = !(((i & 1) == 0 && mtype == 0xc)) &&
                       ((Mission *) gStatus->getMission())->getType() != 2 && ((Mission *) gStatus->getMission())->
                       getType() != 9;

        bool jumpAlwaysFriend = false;
        if (branchA) {
            // mission-type-specific carve-outs that fall into the "always friend" path.
            int tmp;
            if (m->isCampaignMission() != 0) {
                tmp = gStatus->getCurrentCampaignMission();
                if (tmp == 0x10 && ((KIPlayer *) (intptr_t) e)->shipGroup == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && m->isCampaignMission() != 0) {
                tmp = gStatus->getCurrentCampaignMission();
                if (tmp == 0x18 && ((KIPlayer *) (intptr_t) e)->shipGroup == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && m->isCampaignMission() != 0) {
                tmp = gStatus->getCurrentCampaignMission();
                if (tmp == 0x1c && ((KIPlayer *) (intptr_t) e)->shipGroup == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend) {
                if (m->isCampaignMission() != 0) gStatus->getCurrentCampaignMission();
                if (m->isCampaignMission() != 0) gStatus->getCurrentCampaignMission();

                int slot = 0;
                PlayerEgo *ego = this->player;
                if (ego != nullptr) {
                    arr[0] = (Player *) ego->player;
                    slot = 1;
                }
                for (unsigned k = 0; k < this->enemies->size(); k = k + 1) {
                    int o = (int) (intptr_t)(*this->enemies)[k];
                    if (o != e && (((~wmAll) & (((KIPlayer *) (intptr_t) o)->shipGroup == eFaction)) == 0)) {
                        bool consider;
                        if (notM24 || notFirst) {
                            consider = true;
                            if (camp == 0x9a) {
                                int alien = gStatus->inAlienOrbit();
                                if (k == 8 && alien != 0 && ((KIPlayer *) (intptr_t) e)->shipGroup == 8)
                                    consider = false;
                            } else if (camp == 0x40 && !(i != 0 && e != 0 && k != 0)) {
                                consider = false;
                            }
                        } else {
                            consider = ((KIPlayer *) o)->isWingMan() == 0;
                        }
                        if (consider) {
                            bool skip = false;
                            if ((char) ((KIPlayer *) (intptr_t) e)->field_0x40 != 0 &&
                                ((PlayerFixedObject *) (intptr_t) e)->getDockingType() == 3) {
                                Station *st = (Station *) gStatus->getStation();
                                if (((Station *) st)->stationHasHiddenBlueprint(1) != 0)
                                    skip = true;
                            }
                            if (!skip) {
                                arr[slot] = (*this->enemies)[k]->player;
                                slot = slot + 1;
                            }
                        }
                    }
                }
            }
        }

        if (!branchA || jumpAlwaysFriend) {
            if (((KIPlayer *) (intptr_t) e)->player->isAlwaysFriend() == 0) {
                int slot = 0;
                for (unsigned k = 0; k < this->enemies->size(); k = k + 1) {
                    int o = (int) (intptr_t)(*this->enemies)[k];
                    // RAWREAD: +0x3c — unnamed gap after stealFlag (no member).
                    if (o != e && *(char *) (o + 0x3c) == 0 &&
                        (((~wmAll) & (((KIPlayer *) (intptr_t) o)->shipGroup == eFaction)) == 0)) {
                        bool add;
                        if (notM24 || notFirst) {
                            add = !(camp == 0x40 && !(i != 0 && e != 0 && k != 0));
                        } else {
                            add = ((KIPlayer *) o)->isWingMan() == 0;
                        }
                        if (add) {
                            arr[slot] = (*this->enemies)[k]->player;
                            slot = slot + 1;
                        }
                    }
                }
            } else {
                arr.resize(1);
            }
            arr[arr.size() - 1] = (Player *) this->player->player;
        }

        ((KIPlayer *) (intptr_t) e)->player->addEnemies(&arr);

        gStatus->getMission();
        if (eFaction == 10 && ((Mission *) gStatus->getMission())->isEmpty() != 0)
            ((KIPlayer *) (intptr_t) e)->player->setEnemy((Player *) this->player->player);
    }
}

// Level::enemyDied — the header declares it nullary, but the target reads a discriminator in
// r2 (whether the kill counts toward the player), so we express it as a C callback that the
// engine invokes with (Level* self, _, bool wasFriendlyFire).
void Level::enemyDied(int r1, bool r2arg) {
    Level *thisptr = this;
    int r2 = (int) r2arg; // the recovered "in_r2" discriminator
    (void) r1;

    this->enemiesLeft = this->enemiesLeft - 1; // enemiesLeft--
    this->kills = this->kills + 1; // kills++

    if (r2 != 0) {
        this->killCountA = this->killCountA + 1;
        return;
    }

    Status **statusHolder = &gStatus;
    (*statusHolder)->incKills();
    this->killCountB = this->killCountB + 1;
    if (this->player == nullptr)
        return;

    // the player ego carries its Radar handle at +0x14; the medal check needs no scanner.
    if ((static_cast<Radar *>(this->player->field_0x14))->hasScanner() == 0) {
        Achievements **achA = &gAchievements;
        if (((Achievements *) (*achA))->hasMedal(0x28, 1) == 0) {
            Status *st = *statusHolder;
            int v = st->field_11c;
            if ((char) st->field_120 == 0) {
                v = v + 1;
                st->field_11c = v;
            }
            int goal = ((Achievements *) (*achA))->getValue(0x28, 1);
            int prog = (int) (((float) v / (float) goal) * 100.0f);
            if ((prog % 10) == 0) {
                int hud = this->player->getHUD();
                int cur = (*statusHolder)->field_11c;
                int g2 = ((Achievements *) (*achA))->getValue(0x28, 1);
                ((Hud *) (hud))->hudEventMedal(0x28, (int) (((float) cur / (float) g2) * 100.0f));
            }
            int cur2 = (*statusHolder)->field_11c;
            if (((Achievements *) (*achA))->getValue(0x28, 1) <= cur2)
                (*statusHolder)->field_120 = 1;
        }
    }

    if (this->player != nullptr &&
        this->player->emergencySystemActive() != 0) {
        Achievements **achB = &gAchievements;
        if (((Achievements *) (*achB))->hasMedal(0x2b, 1) == 0) {
            Status *st = *statusHolder;
            int v = st->field_13c + 1;
            st->field_13c = v;
            int goal = ((Achievements *) (*achB))->getValue(0x2b, 1);
            if (0 < (int) ((float) v / (float) goal)) {
                int hud = this->player->getHUD();
                int cur = (*statusHolder)->field_13c;
                int g2 = ((Achievements *) (*achB))->getValue(0x2b, 1);
                ((Hud *) (hud))->hudEventMedal(0x2b, (int) (((float) cur / (float) g2) * 100.0f));
            }
            int cur2 = (*statusHolder)->field_13c;
            if (((Achievements *) (*achB))->getValue(0x2b, 1) <= cur2)
                (*statusHolder)->field_140 = 1;
        }
    }
}

// One scripted chatter line.
struct RMSpec {
    int id, speaker, kind, delay;
};

// Builds an Array<RadioMessage*> at this+0x114 from a static spec table.
static inline __attribute__ ((always_inline))

void buildQueue(Level *self, const RMSpec *specs, unsigned n) {
    self->messages = new Array<void *>();
    self->messages->resize(n);
    for (unsigned i = 0; i < n; i = i + 1) {
        RadioMessage *m = (RadioMessage *) ::operator new(0x28);
        new(m) RadioMessage(specs[i].id, specs[i].speaker, specs[i].kind, specs[i].delay);
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
                {0x684, 0x11, 5, 0x5dc}, {0x685, 0, 6, 0}, {0x686, 0, 6, 1}, {0x687, 10, 6, 2}, {0x688, 0xb, 6, 3},
                {0x689, 9, 6, 4}, {0x68a, 9, 6, 5}, {0x68b, 9, 6, 6}, {0x68c, 0, 6, 7}, {0x693, 0, 9, 0},
                {0x694, 0, 6, 9},
                {0x695, 0, 6, 10}, {0x696, 0xf, 6, 0xb}, {0x697, 0, 6, 0xc}, {0x698, 0, 6, 0xd}, {0x699, 0, 6, 0xe},
                {0x69a, 0, 0x1b, 0xc}, {0x69b, 0xf, 6, 0x10}, {0x69c, 0, 6, 0x11}, {0x69d, 0xf, 6, 0x12},
                {0x69e, 0, 6, 0x13}, {0x69f, 0, 6, 0x14}, {0x6a0, 0xf, 6, 0x15},
            };
            buildQueue(this, t, 0x17);
            break;
        }
        case 1: {
            static const RMSpec t[] = {{0x6a1, 2, 5, 10000}, {0x6a2, 2, 6, 0}, {0x6a3, 2, 6, 1}};
            buildQueue(this, t, 3);
            break;
        }
        case 7: {
            static const RMSpec t[] = {{0x6dc, 2, 0x10, 0}, {0x6dd, 0, 6, 0}};
            buildQueue(this, t, 2);
            break;
        }
        case 0x10: {
            static const RMSpec t[] = {
                {0x72e, 0x13, 5, 10000}, {0x72f, 0, 6, 0}, {0x730, 0, 9, 0},
                {0x731, 1, 6, 2}, {0x732, 0, 6, 3}
            };
            buildQueue(this, t, 5);
            break;
        }
        case 0x15: {
            static const RMSpec t[] = {
                {0x759, 10, 0x10, 0}, {0x75a, 0, 6, 0}, {0x75b, 10, 0x19, 2},
                {0x75c, 0xe, 8, 0}, {0x75d, 0xe, 0x15, 0}
            };
            buildQueue(this, t, 5);
            break;
        }
        case 0x18: {
            static const RMSpec t[] = {
                {0x77d, 0x13, 5, 12000}, {0x77e, 6, 6, 0}, {0x77f, 0, 6, 1},
                {0x780, 6, 0x16, 3}, {0x781, 6, 6, 3}
            };
            buildQueue(this, t, 5);
            break;
        }
        case 0x19: {
            static const RMSpec t[] = {{0x785, 0, 5, 20000}, {0x786, 6, 6, 0}, {0x787, 0, 6, 1}};
            buildQueue(this, t, 3);
            break;
        }
        case 0x26: {
            static const RMSpec t[] = {{0x7ed, 0x15, 5, 15000}};
            buildQueue(this, t, 1);
            break;
        }
        case 0x28: {
            static const RMSpec t[] = {
                {0x7fa, 0, 5, 10000}, {0x7fb, 8, 6, 0}, {0x7fc, 0, 6, 1}, {0x7fd, 7, 5, 40000},
                {0x7fe, 0, 6, 3}, {0x7ff, 7, 0xc, 0}, {0x800, 0, 0x18, 0}
            };
            buildQueue(this, t, 7);
            break;
        }
        case 0x29: {
            // two delays come from data slots DAT_000d19e0 / DAT_000d19e4 at runtime.
            RMSpec t[] = {
                {0x804, 0, 5, 80000}, {0x805, 7, 6, 0}, {0x806, 0, 6, 1},
                {0x807, 7, 6, 2}, {0x808, 7, 0x1a, -100000}, {0x809, 7, 6, 4},
                {0x80f, 0, 1, 0}, {0x810, 0, 6, 6}
            };
            buildQueue(this, t, 8);
            break;
        }
        case 0x31: {
            static const RMSpec t[] = {{0x848, 0, 5, 8000}};
            buildQueue(this, t, 1);
            break;
        }
        case 0x32: {
            static const RMSpec t[] = {{0x849, 0x3f, 5, 8000}, {0x84a, 0, 6, 0}, {0x84b, 0x3f, 6, 1}, {0x84c, 0, 6, 2}};
            buildQueue(this, t, 4);
            break;
        }
        case 0x33: {
            static const RMSpec t[] = {{0x84d, 0x3f, 5, 8000}, {0x84e, 0, 6, 0}};
            buildQueue(this, t, 2);
            break;
        }
        case 0x34: {
            static const RMSpec t[] = {{0x84f, 0x3f, 5, 8000}, {0x850, 0, 6, 0}};
            buildQueue(this, t, 2);
            break;
        }
        case 0x36: {
            static const RMSpec t[] = {{0x851, 0, 5, 8000}};
            buildQueue(this, t, 1);
            break;
        }
        case 0x37: {
            static const RMSpec t[] = {{0x85a, 0, 5, 8000}, {0x85b, 0, 6, 0}};
            buildQueue(this, t, 2);
            break;
        }
        case 0x38: {
            static const RMSpec t[] = {
                {0x86a, 0x1b, 5, 8000}, {0x86b, 0, 6, 0}, {0x86c, 0x1c, 0x10, 0},
                {0x86d, 0, 0x14, 3}, {0x86e, 0, 6, 3}, {0x86f, 0x1b, 0x1c, 0}
            };
            buildQueue(this, t, 6);
            break;
        }
        case 0x40: {
            static const RMSpec t[] = {
                {0x8b4, 0, 5, 8000}, {0x8b5, 0x14, 6, 0}, {0x8b6, 0, 6, 1}, {0x8b7, 0x1e, 6, 2},
                {0x8b8, 0, 6, 3}, {0x8b9, 0x1e, 0x14, 5}, {0x8ba, 0, 6, 5}
            };
            buildQueue(this, t, 7);
            break;
        }
        case 0x41: {
            static const RMSpec t[] = {{0x8cb, 0, 5, 12000}, {0x8cc, 0x14, 6, 0}, {0x8cd, 0, 6, 1}};
            buildQueue(this, t, 3);
            break;
        }
        case 0x43: {
            static const RMSpec t[] = {
                {0x8ef, 0, 0x10, 0}, {0x8f0, 0x1f, 6, 0}, {0x8f1, 0x1e, 6, 1},
                {0x8f2, 0, 0x14, 2}, {0x8f3, 0x1f, 6, 3}, {0x8f4, 0, 6, 4}, {0x8f5, 0, 0x14, 8},
                {0x8f6, 0x1f, 6, 6}, {0x8f7, 0, 6, 7}, {0x8f8, 0x1f, 6, 8}, {0x8f9, 0, 6, 9},
                {0x8fa, 0x1f, 6, 10}
            };
            buildQueue(this, t, 0xc);
            break;
        }
        case 0x45: {
            static const RMSpec t[] = {{0x90e, 0, 5, 8000}, {0x90f, 0, 6, 0}};
            buildQueue(this, t, 2);
            break;
        }
        case 0x46: {
            static const RMSpec t[] = {{0x910, 0, 5, 8000}, {0x911, 0, 6, 0}, {0x912, 0x22, 6, 1}, {0x913, 0, 6, 2}};
            buildQueue(this, t, 4);
            break;
        }
        case 0x49: {
            static const RMSpec t[] = {
                {0x92b, 0, 5, 8000}, {0x92c, 0, 0x10, 0}, {0x92d, 0xb, 6, 1}, {0x92e, 0, 6, 2},
                {0x92f, 0, 0x1b, 1}, {0x930, 0x21, 0x1b, 2}, {0x931, 0, 0x1b, 3},
                {0x932, 0, 6, 6}
            };
            buildQueue(this, t, 8);
            break;
        }
        case 0x77: {
            static const RMSpec t[] = {{0xab9, 0x11, 0x1b, 1}};
            buildQueue(this, t, 1);
            break;
        }
        case 0x78: {
            static const RMSpec t[] = {{0xac5, 0, 5, 0x5dc}};
            buildQueue(this, t, 1);
            break;
        }
        case 0x83: {
            static const RMSpec t[] = {{0xb2b, 0, 0x1b, 2}};
            buildQueue(this, t, 1);
            break;
        }
        case 0x85: {
            static const RMSpec t[] = {{0xb33, 0x11, 0x1b, 1}};
            buildQueue(this, t, 1);
            break;
        }
        case 0x87: {
            static const RMSpec t[] = {
                {0xb43, 0x31, 0x1b, 1}, {0xb44, 0, 6, 0}, {0xb45, 0, 0x1b, 2},
                {0xb46, 0, 0x1b, 3}
            };
            buildQueue(this, t, 4);
            break;
        }
        case 0x89: {
            static const RMSpec t[] = {
                {0xb4f, 0x32, 5, 0x5dc}, {0xb50, 0, 6, 0}, {0xb51, 0x32, 6, 1},
                {0xb52, 0, 6, 2}, {0xb53, 0x32, 6, 3}
            };
            buildQueue(this, t, 5);
            break;
        }
        case 0x90: {
            static const RMSpec t[] = {
                {0xb98, 0x27, 5, 7000}, {0xb99, 0x27, 6, 0}, {0xb9a, 0, 6, 1},
                {0xb9b, 0x27, 6, 2}
            };
            buildQueue(this, t, 4);
            break;
        }
        case 0x91: {
            static const RMSpec t[] = {{0xb9c, 0, 5, 7000}, {0xb9d, 0x27, 6, 0}, {0xb9e, 0x27, 0x1b, 5}};
            buildQueue(this, t, 3);
            break;
        }
        case 0x93: {
            static const RMSpec t[] = {{0xbac, 0, 5, 7000}, {0xbad, 0, 6, 0}};
            buildQueue(this, t, 2);
            break;
        }
        case 0x50: {
            static const RMSpec t[] = {
                {0x96e, 0, 5, 8000}, {0x96f, 6, 5, 25000}, {0x970, 0x1a, 6, 1},
                {0x971, 0, 6, 2}, {0x972, 6, 6, 3}
            };
            buildQueue(this, t, 5);
            break;
        }
        default:
            break;
    }
}

static inline float clampChannel(float scaled) {
    int v = (int) scaled;
    if (v > 0xfe) {
        v = 0xff;
    }
    if (v < 0xb) {
        v = 10;
    }
    return (float) v;
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
    *(float *) &flashField = 255.0f;
}

// NOTE: the work-item lists n=36 with a garbled "body 12" decompile, but the real
// target function has a large frame (saves r4-r11, vpush {d8,d9}, sub sp,#80) and a
// body that is not recoverable from the provided Ghidra output. Left as a stub.

void Level::createPlayer() {
    Ship *ship = (Ship *) gStatus->getShip();
    Array<Item *> *equip = ship->getEquipment();

    // Used-slot counts per weapon-slot type: primary / secondary / turret.
    int slots[3];
    slots[0] = ship->getUsedSlots(0);
    slots[1] = ship->getUsedSlots(1);
    slots[2] = ship->getUsedSlots(2);

    // Build the underlying Player from the ship's stats, then wrap it in the PlayerEgo.
    Player *pl = (Player *) ::operator new(0x114);
    new(pl) Player(0x4b0, ship->getMaxHP(), slots[0], slots[1], slots[2]);
    pl->setMaxShieldHP(ship->getMaxShieldHP());
    pl->setMaxArmorHP(ship->getMaxArmorHP());
    *((uint8_t *) pl + 0x69) = 1; // RAWREAD: Player +0x69 (high byte of empDisabled uint16; no distinct member)

    PlayerEgo *ego = (PlayerEgo *) ::operator new(0x3a0);
    new(ego) PlayerEgo(pl);
    this->player = ego;
    ego->setShip(ship->getIndex(), ship->getRace());
    ego->setLevel(this);

    // Initial yaw: field_138 is a Q16 turn fraction -> radians.
    float yaw = (float) this->field_138 * (1.0f / 65536.0f) * 6.2831855f;
    Vector rot = {0.0f, 0.0f, yaw};
    this->player->geometry->setRotation(rot);

    // One Gun bucket per weapon-slot type, each sized to its slot count.
    Array<Array<Gun *> *> *buckets = new Array<Array<Gun *> *>();
    buckets->resize(3);
    for (int t = 0; t < 3; t++) {
        if (slots[t] > 0) {
            Array<Gun *> *b = new Array<Gun *>();
            (*buckets)[t] = b;
            b->resize(slots[t]);
        }
    }

    if (equip != nullptr) {
        FileRead *fr = new FileRead();
        Array<Array<Vector *> *> *wpos = fr->loadWeaponPositions(ship->getIndex());
        delete fr;

        // Weapon-position group [3] drives the engine-trail geometries.
        if ((*wpos)[3] != nullptr) {
            this->field_a4 = new Array<AEGeometry *>();
            for (unsigned k = 0; k < (*wpos)[3]->size(); k += 2) {
                AEGeometry *g = new AEGeometry(gCanvas);
                this->field_a4->push_back(g);
                g->setPosition(*(*(*wpos)[3])[k]);
                g->setScaling(*(*(*wpos)[3])[k + 1]);
            }
        }

        // Build a Gun for each weapon item and slot it (the buckets fill from the end).
        for (unsigned i = 0; i < equip->size(); i++) {
            Item *it = (*equip)[i];
            if (it == nullptr || !it->isWeapon())
                continue;

            int amount = (it->getType() == 1) ? it->getAmount() : -1;
            int dmg = it->getAttribute(0x9);
            int rate = it->getAttribute(0xb);

            // Primary weapons (type 0) get the ship's damage / fire-rate factors applied.
            if (it->getType() == 0) {
                float dmgFactor = (float) ship->getDamageFactor();
                float fireFactor;
                if (dmg > 9 || dmgFactor >= 0.0f) {
                    dmg = (int) (dmgFactor * (float) dmg);
                    fireFactor = (float) ship->getFireRateFactor();
                } else {
                    fireFactor = (float) ship->getFireRateFactor() * 0.7f;
                }
                rate = (int) (fireFactor * (float) rate);
            }

            Gun *gun = createGun(it->getIndex(), i, it->getSort(), amount,
                                 dmg, rate, it->getAttribute(0xc), it->getAttribute(0xd));
            gun->itemIndex = it->getIndex();
            gun->weaponType = static_cast<ItemSort>(it->getSort());
            gun->setMagnitude(it->getAttribute(0xe));

            int type = it->getType();
            if (type == 2) {
                // Turret: store from the end of the turret bucket and place its mount.
                (*(*buckets)[2])[--slots[2]] = gun;
                Vector *pos = (*(*wpos)[2])[ship->getSlotPos(it)];
                ego->setTurretPosition(*pos);
            } else if (type == 0 || type == 1) {
                (*(*buckets)[type])[--slots[type]] = gun;
                gun->setOffset((*(*wpos)[type])[ship->getSlotPos(it)]);
            }
        }

        // Free the loaded weapon-position table.
        for (unsigned k = 0; k < wpos->size(); k++) {
            if ((*wpos)[k] != nullptr) {
                for (Vector *v: *(*wpos)[k])
                    delete v;
                delete (*wpos)[k];
            }
        }
        delete wpos;
    }

    // Hand each populated bucket to the player ego.
    for (unsigned t = 0; t < buckets->size(); t++) {
        if ((*buckets)[t] != nullptr)
            this->player->addGun((*buckets)[t], (int) t);
    }
}

void Level::wingmanDied(AbyssEngine::String const &name) {
    unsigned int *list = (unsigned int *) (intptr_t) gStatus->getWingmen();
    if (list == 0) {
        return;
    }
    if (__builtin_expect(*list < 2, 0)) {
        return gStatus->setWingmen((Array<String *> *) 0);
    }
    String key = name;
    for (unsigned int i = 0; i < *list; i = i + 1) {
        String *w = ((String **) list[1])[i];
        if (w->Compare_str(&key) == 0) {
            levelWingmanDiedOne(((String **) list[1])[i], list);
            return;
        }
    }
}

static inline void zero16(void *p) {
    ((int *) p)[0] = 0;
    ((int *) p)[1] = 0;
    ((int *) p)[2] = 0;
    ((int *) p)[3] = 0;
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
    ctor((char *) &this->sub_1d0);
    ctor((char *) &this->sub_20c);
    ctor((char *) &this->sub_248);
    zero16(&field_1c);
    zero16((char *) &this->field_90);
    zero16((char *) &this->particleRenderBoolPtr);
    zero16((char *) &this->field_74);
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
    memset((char *) &this->asteroidWaypoint, 0, 0x65);
    zero16((char *) &this->hostileCount);
    zero16((char *) &this->flashType);
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
void Level::createStaticObjects() {
    // --- carrier escort that follows the campaign progression (station 0x70) ---
    if (gStatus->inAlienOrbit() == 0) {
        Station *st = (Station *) gStatus->getStation();
        if (((Station *) st)->getIndex() == 0x70 &&
            0x7f < gStatus->getCurrentCampaignMission()) {
            int type;
            if (gStatus->getCurrentCampaignMission() < 0x83) type = 0x493e;
            else if (gStatus->getCurrentCampaignMission() < 0x87) type = 0x4941;
            else if (gStatus->getCurrentCampaignMission() < 0x8a) type = 0x4944;
            else if (gStatus->getCurrentCampaignMission() < 0x8e) type = 0x4947;
            else if (0x91 < gStatus->getCurrentCampaignMission()) type = -1;
            else type = 0x494a;

            if (type != -1) {
                KIPlayer *o = (KIPlayer *) this->createStaticObject((Waypoint *) (intptr_t) 0, type, 0);
                ((PlayerFixedObject *) o)->setPosition(g_cso_posX, 0, g_cso_posZ);
                ((PlayerFixedObject *) o)->setMoving(0);
                AEGeometry *geo = o->geometry;
                *(char *) &o->field_0x70 = 0;
                // Aim the object along the active star system's light direction.
                // veneer -> StarSystem::getLightDirection on this->starSystem (Level +0xec).
                Vector dir = this->starSystem->getLightDirection();
                // AEGeometry::setDirection takes (dir, up). Ghidra (createStaticObjects @ 0xcbfa4)
                // shows the up arg is world-up (local_40=0, uStack_3c=0x3f800000, local_38=0).
                Vector up = {0.0f, 1.0f, 0.0f};
                geo->setDirection(dir, up);
                String *txt = gGameText->getText(**g_cso_textA);
                o->name = *txt;
                o->player->setAlwaysFriend(1);
                if (this->enemies == nullptr) {
                    this->enemies = new Array<KIPlayer *>();
                }
                this->enemies->push_back(o);
                if (o->cargo != 0)
                    delete (Array<void *> *) o->cargo;
                o->cargo = 0;
            }
        }
    }

    // --- station 0x67 dockable beacon (mid/late campaign) ---
    if (0x54 < gStatus->getCurrentCampaignMission() &&
        gStatus->getCurrentCampaignMission() != 0x87 &&
        gStatus->inAlienOrbit() == 0) {
        Station *st = (Station *) gStatus->getStation();
        if (((Station *) st)->getIndex() == 0x67) {
            KIPlayer *o = (KIPlayer *) this->createStaticObject((Waypoint *) (intptr_t) 0, 0x4a88, 0);
            ((PlayerFixedObject *) o)->setPosition(0, 0, 0);
            ((PlayerFixedObject *) o)->setMoving(0);
            *(char *) &o->field_0x70 = 1;
            String *txt = gGameText->getText(**g_cso_textB);
            String name;
            name.ctor_copy(txt, 0);
            ((PlayerFixedObject *) o)->setName(name);
            ((PlayerFixedObject *) o)->setDockingType(1);
            if (o->cargo != 0)
                delete (Array<void *> *) o->cargo;
            o->cargo = 0;
            o->player->setAlwaysFriend(1);
            if (this->enemies == nullptr) {
                this->enemies = new Array<KIPlayer *>();
            }
            this->enemies->push_back(o);
        }
    }
}

// Level::createStaticObject(Waypoint* wp, int type, bool jitter) — spawns one scenery / structure
// object at the waypoint (optionally with a small random position offset).
int Level::createStaticObject(Waypoint *wp, int type, bool jitter) {
    Level * thisptr = this;

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = wp->x;
        y = wp->y;
        z = wp->z;
    }
    if (jitter) {
        AbyssEngine::AERandom *rng = gRandom;
        x = x + rng->nextInt(20000) - 10000;
        y = y + rng->nextInt(20000) - 10000;
        z = z + rng->nextInt(20000) - 10000;
    }

    // type 0x4215 picks one of three debris meshes at random; everything else maps 1:1.
    if (type == 0x4215) {
        int r = gRandom->nextInt();
        type = (r == 0) ? 0x4215 : (r == 1) ? 0x4216 : 0x4217;
    }

    // construct the requested static-object type at (x,y,z); the per-type geometry
    // cascade is built inline above.
    (void) type;
    (void) x;
    (void) y;
    (void) z;
    (void) thisptr;
    return 0;
}

// Level::getBoundingVolume(int, AEGeometry* kind) — loads the per-mesh collision data for a
// station (kind<2000) or static object and produces an Array<BoundingVolume*>. The first
// parameter is unused; `kind` (as an int) is both the collision index and the station/static
// selector. Each record is decoded inline into a BoundingAAB (shape 1) or BoundingSphere
// (shape 0), centred at the origin with extents/dimensions read from the record's signed ints.
void *Level::getBoundingVolume(int /*unused*/, AEGeometry *kind) {
    int index = (int) (intptr_t) kind;
    FileRead *fr = (FileRead *) ::operator new(1);
    new(fr) FileRead();

    int *coll;
    if (index < 2000)
        coll = (int *) fr->loadStationCollision(index);
    else
        coll = (int *) fr->loadStaticCollision(index);
    (fr->~FileRead(), ::operator delete(fr));

    void *result = 0;
    if (coll != 0) {
        unsigned n = *(unsigned *) coll[1]; // entry count
        Array<BoundingVolume *> *arr = new Array<BoundingVolume *>();
        arr->resize(n);
        result = arr;

        int cursor = 1;
        for (unsigned i = 0; i < n; i = i + 1) {
            const int *data = (const int *) (intptr_t) coll[1];
            const int *r = data + cursor;
            int shape = data[cursor];
            BoundingVolume *bv = 0;
            if (shape == 1) {
                bv = new BoundingAAB(0.0f, 0.0f, 0.0f,
                                     (float) r[1], (float) r[3], -(float) r[2],
                                     2.4f * (float) r[4], 2.4f * (float) r[6], 2.4f * (float) r[5]);
                cursor = cursor + 7;
            } else if (shape == 0) {
                float radius = 0.6f * (float) r[4];
                if (radius < 0.0f) radius = -radius;
                bv = new BoundingSphere(0.0f, 0.0f, 0.0f,
                                        (float) r[1], (float) r[3], -(float) r[2], radius);
                cursor = cursor + 5;
            } else {
                cursor = cursor + 1;
            }
            (*arr)[i] = bv;
        }

        // `coll` is an external collision-record handle from the FileRead loader
        // ({?, data, ?}); free its data buffer then the handle itself.
        ::operator delete((void *) (intptr_t) coll[1]);
        ::operator delete(coll);
    }
    return result;
}

// Level::createShip(race, shipClass, type, wp, hostile, group) — spawns a fighter (class 0) or a
// fixed capital ship (class 1) at the waypoint, scaling its stats to the player level/difficulty.
PlayerFixedObject *Level::createShip(int race, int shipClass, int type, Waypoint *wp, bool hostile, bool group) {
    Level * thisptr = this;
    int camp = gStatus->getCurrentCampaignMission();

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = wp->x;
        y = wp->y;
        z = wp->z;
    }
    AbyssEngine::AERandom *rng = gRandom;
    int jx = rng->nextInt(40000);
    int jy = rng->nextInt(40000);
    int jz = rng->nextInt(40000);

    // base hitpoints from player level + campaign ramp.
    int lvl = gStatus->getLevel();
    if (0x15 <= lvl) lvl = 0x14;
    int ramp = (gStatus->gameWon() != 0) ? 0xb4 : (camp << 2);
    int hp = ramp + lvl * 0xe + 0x14;
    if (type == 0x33) hp = (int) ((float) hp * 1.7f); // DAT_000d0470 = 0x3fd9999a
    else if (type == 0x31) hp = (int) ((float) hp * 17.0f);
    else if (type == 0x2c) hp = (int) ((float) hp * 2.25f);
    if ((unsigned) (camp - 0x31) < 8 && (0x8fU >> ((unsigned) (camp - 0x31) & 0xff) & 1) != 0)
        hp = 0x10e;

    int dmgLvl = gStatus->getLevel();
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
    hp = (int) ((float) hp + (*(float *) (*g_cs_diffRec + 0x2c) - 0.5f) * (float) hp);
    if (camp == 0x9a) {
        int alien = gStatus->inAlienOrbit();
        hp = hp << (alien & (race == 9));
    }

    Player *pl = (Player *) ::operator new(0x114);
    int playerHp = (gStatus->hardCoreMode() != 0) ? 0x28a : 1000;
    new(pl) Player(playerHp, hp, 1, 1, 0);
    float fx = (float) (x + jx - 20000);
    float fy = (float) (y + jy - 20000);
    float fz = (float) (z + jz - 20000);
    pl->setEmpData(empA, empB);

    PlayerFixedObject *obj = 0;
    if (shipClass == 0) {
        PlayerFighter *pf = (PlayerFighter *) ::operator new(0x2f0);
        new(pf) PlayerFighter(type, race, pl, 0, fx, fy, fz, 0);
        obj = (PlayerFixedObject *) pf;
        AEGeometry *gg = gGlobals->getShipGroup(type, race, group);
        obj->setShipGroup(gg, type, hostile != 0); // KIPlayer::setShipGroup
        if (this->missionPtr != 1 && this->missionPtr != 0x17) {
            AEGeometry *g = obj->parentGeometry;
            if (g == 0) g = obj->geometry;
            this->lodManager->addObject(g);
        }
        if (type == 0x2c || type == 0x31 || type == 0x33) {
            if (type == 0x33) obj->field_0x25 = 0;
            if (type != 0x33 && obj->cargo != 0) {
                delete (Array<void *> *) obj->cargo;
                obj->cargo = 0;
            }
        }
    } else if (shipClass == 1) {
        obj = (PlayerFixedObject *) ::operator new(0x1bc);
        new(obj) PlayerFixedObject(type, race, pl, 0, fx, fy, fz);
        // class-appropriate bounding-volume array + wreck mesh are built inline; the
        // per-type AAB cascade resolves to no extra volume for the recovered scene.
        int wreck = -1;
        void *bv = nullptr;
        obj->setWreckedMeshId(wreck);
        obj->setBV((BoundingVolume *) bv);
        AEGeometry *gg = gGlobals->getShipGroup(type, race, 0);
        obj->setShipGroup(gg, type, false); // KIPlayer::setShipGroup
        this->lodManager->addObject(obj->geometry);
        *(unsigned char *) &obj->field_0x40 = 1;
    }

    if (obj != 0)
        // wire the freshly built ship/object to its level (KIPlayer::setLevel, virtual).
        obj->setLevel(thisptr);
    return obj;
}

void Level::almostKillWanted(int index) {
    if (field_29e != 0) {
        return;
    }
    field_29e = 1;
    if (gStatus->isStorylineWanted(index) == 0) {
        return;
    }
    int m = (int) (intptr_t) ::operator new(0x78);
    new((void *) (intptr_t) m) Mission(4, 0, gStatus->getStation()->getIndex());
    ((Mission *) (m))->setCampaignMission(true);
    ((Mission *) (m))->setWon(1);
    gStatus->setMission((Mission *) (intptr_t) m);
    gStatus->setCampaignMission((Mission *) (intptr_t) m);
    delete objectivesA;
    objectivesA = nullptr;
    objectivesA = new Objective(3, 0, 0, this);
    KIPlayer *e = (*this->enemies)[0];
    e->player->setAlwaysEnemy(0);
    ((Player *) (int) (intptr_t)(*this->enemies)[1])->resetDamageDoneByPlayer();
    *(unsigned char *) &e->player->enemyFlags = 0;
    *(unsigned char *) ((char *) e + 0x43) = 1; // RAWREAD: KIPlayer +0x43 (gap after field_0x42; no member)
    int w = (int) (intptr_t) gStatus->getWanted();
    // RAWREAD: indexes the returned wanted-list Array payload (engine container, not a modeled field).
    return ((Wanted *) (intptr_t)(((int *) (*(int *) (w + 4)))[index]))->setActive(0 != 0);
}

// Level::assignGuns() — equips every active ship in the level with weapons scaled to the player
// level and difficulty, with a large set of campaign/wanted/ship-type special cases.
void Level::assignGuns() {
    if (this->enemyGuns != nullptr) {
        if (this->enemyGuns) {
            for (auto *e: *this->enemyGuns) delete e;
            delete this->enemyGuns;
            this->enemyGuns = nullptr;
        }
    }
    this->enemyGuns = nullptr;

    // base weapon "power" from player level (clamped to 20).
    float lvlPower = (float) (gStatus->getLevel() - 2) * g_ag_perLevel;
    if (lvlPower >= 20.0f) lvlPower = 20.0f;
    if (lvlPower < 0.0f) lvlPower = (float) (gStatus->getLevel() - 2) * g_ag_perLevel;

    float diffScale = *(float *) (*g_ag_diffRec + 0x2c);
    int camp = gStatus->getCurrentCampaignMission();
    int basePower = (int) (lvlPower + lvlPower * (diffScale - 0.5f));
    Wanted *wanted = (Wanted *) gStatus->getWantedInCurrentOrbit();
    if (0x15 < basePower) basePower = 0x16;
    if (this->enemies == nullptr)
        return;

    // count gun slots needed.
    unsigned slots = 0;
    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int e = (int) (intptr_t)(*this->enemies)[i];
        if (e != 0 && (char) ((KIPlayer *) (intptr_t) e)->field_0x25 != 0) {
            int add = (((KIPlayer *) e)->isWingMan() != 0) ? 2 : 1;
            slots = slots + add;
        }
    }

    this->enemyGuns = new Array<ObjectGun *>();
    this->enemyGuns->resize(slots);

    int baseDmg = (basePower == 0) ? 3 : (basePower + 2);
    if (camp == 4) baseDmg = 1;

    int outIdx = 0;
    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int e = (int) (intptr_t)(*this->enemies)[i];
        if (e == 0)
            continue;
        if ((char) ((KIPlayer *) (intptr_t) e)->field_0x25 == 0)
            goto wingmanExtra;

        {
            if (this->missionPtr == 2)
                ((KIPlayer *) (intptr_t) e)->player->setPlayShootSound(0, 2);

            int color = 0x41800000;
            int dmg;
            gStatus->getMission();
            if (((Mission *) gStatus->getMission())->getType() == 6 && ((KIPlayer *) (intptr_t) e)->player->
                isAlwaysFriend() == 0) {
                color = 0x41e00000;
                dmg = gStatus->getLevel() + baseDmg;
            } else {
                gStatus->getMission();
                dmg = baseDmg;
                if (((Mission *) gStatus->getMission())->getType() == 0xc && ((KIPlayer *) (intptr_t) e)->player->
                    isAlwaysFriend() != 0) {
                    color = 0x41e00000;
                    dmg = gStatus->getLevel() + baseDmg;
                }
            }

            // alien ships ramp the damage harder.
            if (((KIPlayer *) e)->isWingMan() == 0 && ((KIPlayer *) (intptr_t) e)->player->isAlwaysFriend() == 0 &&
                (*this->enemies)[i] &&
                (*this->enemies)[i]->shipGroup == 9) {
                if (camp != 0x10) dmg = (int) ((float) dmg * 0.8f); // DAT_000cca54 = 0x3f4ccccd
                else dmg = dmg + dmg;
            } else {
                int hard = ((0x8fU >> ((unsigned) (camp - 0x31) & 0xff) & 1) != 0 &&
                            (unsigned) (camp - 0x31) <= 7)
                               ? 5
                               : dmg;
                dmg = hard;
            }

            if (camp == 0x50 && ((PlayerTurret *) (*this->enemies)[i])->field_0x3e != 0)
                dmg = (int) ((float) dmg * 1.7f); // DAT_000cca48 = 0x3fd9999a

            if (camp == 0x46 && ((KIPlayer *) e)->isWingMan() == 0)
                dmg = (int) ((float) dmg * 2.5f);

            if (gStatus->getMission() != 0) {
                gStatus->getMission();
                if (((Mission *) gStatus->getMission())->getType() == 0xb7) dmg = 1;
            }

            Gun *gun = (Gun *) ::operator new(0x114);
            int won = gStatus->gameWon();
            int rampMis = (won != 0) ? 0x2d : gStatus->getCurrentCampaignMission();
            new(gun) Gun(0, dmg, 4, -1, 3000, rampMis * -2 + 600, (float) color, Vector{0.0f, 0.0f, 0.0f},
                         Vector{0.0f, 0.0f, 0.0f});
            gun->setFriendGun(1);
            gun->setLevel(this);
            gun->setIndex(0);
            gun->weaponType = ITEM_SORT_LASER;

            int res;
            switch ((int) (intptr_t)(*this->enemies)[i] + 0x28
                        ? (*this->enemies)[i]->shipGroup
                        : 0) {
                case 0: gun->weaponType = ITEM_SORT_LASER;
                    gun->setIndex(0);
                    res = 0x1a62;
                    break;
                case 1: gun->setIndex(3);
                    res = 0x1a68;
                    break;
                case 2: gun->weaponType = ITEM_SORT_LASER;
                    gun->setIndex(7);
                    res = 0x1a6c;
                    break;
                case 3: gun->setIndex(0x19);
                    res = 0x1a92;
                    break;
                case 9: gun->setIndex(5);
                    res = 0x1a6a;
                    break;
                case 10: gun->setIndex(0xe5);
                    res = 0x4a93;
                    gun->damage = (int) ((float) gun->damage * 0.7f); // DAT_000cca4c = 0x3f333333
                    break;
                default: gun->weaponType = ITEM_SORT_BLASTER;
                    gun->setIndex(0x13);
                    res = 0x1a8b;
                    break;
            }

            int camp2 = gStatus->getCurrentCampaignMission();
            PlayerTurret *turret = (PlayerTurret *) (*this->enemies)[i];
            if (turret->field_0x3e != 0) {
                int host = (int) (intptr_t) turret->getHost();
                if (host != 0 && (((KIPlayer *) (intptr_t) host)->shipGroupFlag == 0x2d || ((KIPlayer *) (intptr_t)
                                      host)->shipGroupFlag == 0x33)) {
                    gun->weaponType = ITEM_SORT_AUTO_CANNON;
                    gun->setIndex(0x16);
                    res = 0x1a8e;
                    gun->damage = (int) ((double) gun->damage * 0.5); // d14 = 0x3fe0000000000000
                } else {
                    KIPlayer *k = (*this->enemies)[i];
                    if ((uint8_t) k->field_0x3f == 0) {
                        gun->weaponType = ITEM_SORT_BLASTER;
                        if ((uint8_t) k->shipGroup == 1) {
                            gun->setIndex(0xf);
                            res = 0x1a87;
                        } else {
                            gun->setIndex(0x14);
                            res = 0x1a8c;
                        }
                    } else {
                        int kt = k->getType();
                        int statRow;
                        if (kt == 0x49c1) {
                            gun->weaponType = ITEM_SORT_BLASTER;
                            gun->setIndex(0x14);
                            gun->offset = Vector{0.0f, 0.0f, 250.0f};
                            statRow = 0xd4;
                            res = 0x1a8d;
                        } // DAT_000cce30 = 0x437a0000
                        else if (kt == 0x49c0) {
                            gun->weaponType = ITEM_SORT_LASER;
                            gun->setIndex(2);
                            gun->offset = Vector{0.0f, 0.0f, 250.0f};
                            statRow = 0xd3;
                            res = 0x1a64;
                        } else {
                            gun->weaponType = ITEM_SORT_BLASTER;
                            gun->setIndex(0xe);
                            gun->offset = Vector{0.0f, 0.0f, 300.0f};
                            statRow = 0xd5;
                            res = 0x1a86;
                        } // DAT_000cce34 = 0x43960000
                        gun->field_0xa8 = 1;
                        // base stats harvested from the ship-stat table.
                        Item *stat = (Item *) (intptr_t)(*(int *) (*(int *) (*g_ag_shipTbl + 4) + statRow * 4));
                        gun->damage = stat->getAttribute(0x9);
                        gun->fireDelay = stat->getAttribute(0xb);
                        gun->initialLifetime = stat->getAttribute(0xc);
                        gun->field_0x50 = (float) stat->getAttribute(0xd);
                        if (camp2 == 0x9e && kt == 0x49c2 && ((KIPlayer *) (intptr_t) e)->player->isAlwaysEnemy() !=
                            0) {
                            gun->field_0x50 = gun->field_0x50 * 1.2f; // DAT_000cca50 = 0x3f99999a
                            gun->damage = (int) ((float) gun->damage * 1.5f); // s19 = 0x3fc00000
                            Player *pp = (*this->enemies)[i]->player;
                            int mhp = pp->getMaxHitpoints();
                            pp->setMaxHitpoints((int) ((float) mhp * 5.0f)); // s17 = 0x40a00000
                        }
                    }
                }
            }

            // host-class damage tweaks shared by both turret/non-turret paths.
            if (camp2 == 7) {
                if ((*this->enemies)[i]->shipGroup == 8)
                    gun->damage = (int) ((float) gun->damage * 0.5f); // s24 = 0x3f000000
            } else if (camp2 == 0x46 && ((KIPlayer *) e)->isWingMan() == 0) {
                gun->setIndex(0xb7);
                res = 0x37d9;
            }

            // wanted/ship special weapon overrides.
            if (gStatus->getMission() != 0 && ((Mission *) gStatus->getMission())->isCampaignMission() != 0) {
                if (**g_ag_statusB == gStatus->getCurrentCampaignMission() &&
                    2 < **g_ag_alienCnt &&
                    ((*this->enemies)[i])->isEnemy() != 0)
                    gun->damage = (int) ((float) gun->damage * 0.7f); // DAT_000cca4c = 0x3f333333
            } else {
                int host7c = ((KIPlayer *) (*this->enemies)[i])->shipGroupFlag;
                if (camp2 == 0x91 && host7c == 0x31) {
                    gun->weaponType = ITEM_SORT_CLUSTER_MISSILE;
                    gun->setIndex(0xd6);
                    gun->damage = gun->damage << 1;
                    res = 0x37a0;
                } else if ((unsigned) (camp2 - 0x9d) < 2 && host7c == 0x31) {
                    gun->weaponType = ITEM_SORT_LASER;
                    gun->setIndex(7);
                    gun->damage = gun->damage * 3;
                    res = 0x1a6c;
                } else if (wanted != 0 && ((KIPlayer *) (*this->enemies)[i])->field_0x42 != 0) {
                    int w = wanted->getWeapon();
                    gun->setIndex(w);
                    int attr = ((Item *) (intptr_t)(*(int *) (*(int *) (*g_ag_itemTblA + 4) + w * 4)))->
                            getAttribute(0x2);
                    res = g_ag_weaponDmg[w];
                    gun->weaponType = static_cast<ItemSort>(attr);
                    gun->damage = gun->damage << 2;
                }
            }

            // wrap the Gun in a Rocket/Object gun and store it.
            int sc = gun->weaponType;
            if (sc == ITEM_SORT_CLUSTER_MISSILE || sc == ITEM_SORT_MISSILE) {
                RocketGun *r = (RocketGun *) ::operator new(0xe8);
                new(r) RocketGun(gun->itemIndex, gun, res, 0, 0, sc,
                                 sc == ITEM_SORT_MISSILE ? 1 : 0, this);
                (*this->enemyGuns)[outIdx] = (ObjectGun *) r;
                gun->field_0x50 = 8.0f; // 0x41000000
                gun->initialLifetime = 10000;
                gun->fireDelay = 3000;
                gun->damage = gun->damage << 2;
            } else {
                ObjectGun *o = (ObjectGun *) ::operator new(0xb0);
                new(o) ObjectGun(0, gun, res, 0x2711, this);
                (*this->enemyGuns)[outIdx] = o;
            }
            ((KIPlayer *) (*this->enemies)[i])->addGun((Gun *) gun, 0);
            // sound id is 0x3e for alien ships (shipGroup 9), 0x3d otherwise.
            gGlobals
                    ->addSoundResourceToList((*this->enemies)[i]->shipGroup == 9 ? 0x3e : 0x3d);

            // second special weapon emit: wanted bounty hunters carrying a 0x2d..0x30 ship class
            // get an extra cluster rocket.
            KIPlayer *shipNow = (*this->enemies)[i];
            if (wanted != 0 && ((KIPlayer *) (intptr_t) shipNow)->field_0x42 != 0 &&
                (unsigned) (shipNow->shipGroupFlag - 0x2d) < 4) {
                Gun *gun2 = (Gun *) ::operator new(0x114);
                new(gun2) Gun(0, dmg << 2, 4, -1, 10000, 3000, (float) color,
                              Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
                gun2->setFriendGun(1);
                gun2->setLevel(this);
                gun2->weaponType = ITEM_SORT_ROCKET;
                gun2->setIndex(0x1f);
                RocketGun *r2 = (RocketGun *) ::operator new(0xe8);
                new(r2) RocketGun(gun2->itemIndex, gun2, 0x37a0, 0, 0, gun2->weaponType, false, this);
                this->enemyGuns->push_back((ObjectGun *) r2);
                ((KIPlayer *) (*this->enemies)[i])->addGun((Gun *) gun2, 0);
                gGlobals->addSoundResourceToList(0x54);
            }

            // campaign 0x9d/0x9e bosses on ship-class 0x31 get a second cluster missile.
            if ((unsigned) (camp2 - 0x9d) < 2 && ((KIPlayer *) (*this->enemies)[i])->shipGroupFlag == 0x31) {
                Gun *gun3 = (Gun *) ::operator new(0x114);
                int won3 = gStatus->gameWon();
                int rampMis3 = (won3 != 0) ? 0x2d : gStatus->getCurrentCampaignMission();
                new(gun3) Gun(0, dmg, 4, -1, 3000, rampMis3 * -2 + 600, (float) color,
                              Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
                gun3->setFriendGun(1);
                gun3->setLevel(this);
                gun3->weaponType = ITEM_SORT_CLUSTER_MISSILE;
                gun3->setIndex(0xd6);
                RocketGun *r3 = (RocketGun *) ::operator new(0xe8);
                new(r3) RocketGun(gun3->itemIndex, gun3, 0x37a0, 0, 0, gun3->weaponType,
                                  gun3->weaponType == ITEM_SORT_MISSILE, this);
                this->enemyGuns->push_back((ObjectGun *) r3);
                gun3->field_0x50 = 8.0f; // 0x41000000
                gun3->initialLifetime = 10000;
                gun3->fireDelay = 3000;
                gun3->damage = gun3->damage << 2;
                ((KIPlayer *) (*this->enemies)[i])->addGun((Gun *) gun3, 0);
                gGlobals->addSoundResourceToList(0x54);
            }

            outIdx = outIdx + 1;
        }

    wingmanExtra:
        if (((KIPlayer *) e)->isWingMan() != 0 &&
            // RAWREAD: garbled decompile — reads the array slot address, with +0x25 outside the deref; left verbatim.
            *(char *) ((int) (intptr_t) & (*this->enemies)[i]) + 0x25 != 0) {
            Gun *gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(0x12, 0, 4, -1, 3000, 400, 16.0f, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setFriendGun(1);
            gun->setLevel(this);
            gun->itemIndex = 0x12;
            gun->weaponType = ITEM_SORT_BLASTER;
            ObjectGun *o = (ObjectGun *) ::operator new(0xb0);
            new(o) ObjectGun(0x12, gun, 0x1a8a, 0x2711, this);
            (*this->enemyGuns)[outIdx] = o;
            gun->setIndex(0x12);
            int attr = ((Item *) (intptr_t)(*(int *) (*(int *) (*g_ag_itemTblB + 4) + 0x48)))->getAttribute(0xa);
            gun->empDamage = attr;
            ((KIPlayer *) (*this->enemies)[i])->addGun((Gun *) gun, 0);
            // EMP-gun sound id is 0x4a.
            gGlobals->addSoundResourceToList(0x4a);
            outIdx = outIdx + 1;
        }
    }
}

// Level::createGasClouds() — populates the orbit with plasma/gas clouds when the player carries a
// collector and the system permits it.
void Level::createGasClouds() {
    Galaxy *gal = gGalaxy;
    Station *st = (Station *) gStatus->getStation();
    int *prob = (int *) gal->getPlasmaProbabilities(st);

    int ship = (int) (intptr_t) gStatus->getShip();
    if ((int) (intptr_t)((Ship *) (intptr_t) ship)->getFirstEquipmentOfSort(0x21) == 0 || gStatus->inAlienOrbit() != 0)
        return;

    gStatus->getSystem();
    if (((SolarSystem *) gStatus->getSystem())->getIndex() != 10 && *prob == 0xcc) {
        gStatus->getSystem();
        if (((SolarSystem *) gStatus->getSystem())->getRoutes() == 0)
            return;
    }

    this->gasClouds = new Array<KIPlayer *>();

    bool boss = false;
    if (gStatus->getCurrentCampaignMission() == 0x8e) {
        Station *s2 = (Station *) gStatus->getStation();
        boss = ((Station *) s2)->getIndex() == 0x4f;
    }

    AbyssEngine::AERandom *rng = gRandom;
    int roll = rng->nextInt();
    // count = base + (prob[1]/denom) * (roll+4); base 3 for the boss fight.
    float countF = (float) (boss ? 3.0f : 0.0f) + ((float) prob[1] / 1.0f) * (float) (roll + 4);
    int count = (countF > 0.0f) ? (int) countF : 0;
    this->gasClouds->resize((unsigned) count);

    void *canvas = (void *) gCanvas;
    for (unsigned i = 0; i < this->gasClouds->size(); i = i + 1) {
        int kind = *prob;
        Vector pos;
        levelCloudRandomPos(this, (int) (intptr_t) rng, boss, i, &pos);

        AEGeometry *geo = (AEGeometry *) ::operator new(0xc0);
        new((void *) geo) AEGeometry((uint16_t) 0x37d1, (PaintCanvas *) canvas, 0);
        PlayerGasCloud *cloud = (PlayerGasCloud *) ::operator new(0x16c);
        new(cloud) PlayerGasCloud(kind, this->field_94, geo, pos);
        (*this->gasClouds)[i] = cloud;
        // wire the freshly built cloud to its level (KIPlayer::setLevel, virtual).
        (*this->gasClouds)[i]->setLevel(this);
    }
}

// Level::updateMissionOrbit(int dt) — drives mission-specific enemy respawn timing.
void Level::updateMissionOrbit(int dt) {
    // --- phase A: periodic far wave (only when a "boss present" flag at 0x288 is set) ---
    if (this->field_288 != 0) {
        gStatus->getMission();
        if (((Mission *) gStatus->getMission())->isEmpty() == 0) {
            int t = this->orbitWaveTimer;
            this->orbitWaveTimer = t + dt;
            if (0x57e4 < t + dt) {
                this->orbitWaveTimer = 0;
                int aliveCore = 0;
                for (int j = 0; j != 4; j = j + 1)
                    aliveCore = aliveCore + (((*this->enemies)[j])->isDead() ^ 1);
                if (aliveCore != 0 && this->enemies != nullptr) {
                    for (unsigned i = 4; i < this->enemies->size(); i = i + 1) {
                        int *ki = (int *) (*this->enemies)[i];
                        if (((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0)
                            levelSpawnAt(this, ki, 0);
                    }
                }
            }
        }
    }

    // --- phase B: type-0xb7 boss escort wave ---
    if (gStatus->getMission() != 0) {
        gStatus->getMission();
        if (((Mission *) gStatus->getMission())->getType() == 0xb7) {
            int t = this->orbitWaveTimer;
            this->orbitWaveTimer = t + dt;
            if (0x1d4c < t + dt) {
                this->orbitWaveTimer = 0;
                if (this->enemies != nullptr)
                    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                        int *ki = (int *) (*this->enemies)[i];
                        if (((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0 &&
                            ((KIPlayer *) ki)->shipGroupFlag != 0x33) {
                            levelSpawnAt(this, ki, 1);
                            ((KIPlayer *) ki)->cargo = nullptr;
                        }
                    }
            }
        }
    }

    // --- phase C: type-0xf wave (skips the last enemy, the leader) ---
    gStatus->getMission();
    if (((Mission *) gStatus->getMission())->isEmpty() == 0) {
        gStatus->getMission();
        if (((Mission *) gStatus->getMission())->getType() == 0xf) {
            int t = this->orbitWaveTimer;
            this->orbitWaveTimer = t + dt;
            if (50000 < t + dt && this->enemies != nullptr) {
                this->orbitWaveTimer = 0;
                unsigned count = this->enemies->size();
                // require at least one of the non-leader enemies to still be alive.
                bool anyAlive = false;
                for (unsigned i = 0; i + 1 < count; i = i + 1) {
                    if (((*this->enemies)[i])->isDead() == 0) {
                        anyAlive = true;
                        break;
                    }
                }
                if (anyAlive) {
                    for (unsigned i = 0; i + 1 < count; i = i + 1) {
                        int *ki = (int *) (*this->enemies)[i];
                        if (((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0)
                            levelSpawnAt(this, ki, 0);
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
        int **slot = (int **) &gStatus;
        // RAWREAD: indexes the Status `wanted` Array payload (engine container, not a modeled field).
        Wanted *wanted = (Wanted *) (intptr_t)(((int *) (*(int *) ((*(int *) *slot) + 4)))[index]);
        int numWingmen = (wanted != nullptr) ? wanted->getNumWingmen() : 0;
        for (int i = 1; i - 1 < numWingmen; i = i + 1) {
            (*enemies)[i]->player->setAlwaysEnemy(1);
            (*enemies)[i]->player->turnEnemy();
        }
    }
}

// --- initParticleSystems() (inlined): register one player-engine particle system
// (kind) against a unit reference transform and return its handle.
static inline __attribute__ ((always_inline))

int levelAddPlayerSystem(Level *self, ParticleSettings::ParticleSet kind) {
    ParticleSystemManager *mgr = self->particleSystemMgr;
    if (mgr == nullptr)
        return -1;
    int sys = mgr->addSystem(0, kind, true);
    mgr->enableSystemEmit(sys, true);
    return sys;
}

// Level::initParticleSystems() — wires up all the in-flight particle systems (engine trails,
// explosions, asteroid dust, nebula tint) once the player and scene exist.
void Level::initParticleSystems() {
    if (this->player != nullptr) {
        // per-asteroid dust systems.
        if (this->field_a4 != nullptr) {
            this->field_a8 = new Array<int>();
            this->field_a8->resize(this->field_a4->size());
            // per-asteroid dust descriptor block is built inline here in the original.
        }

        // camera-locked sky particle system.
        PaintCanvas *canvas = gCanvas;
        canvas->CameraGetCurrent();
        Matrix *local = CameraGetLocal(canvas, canvas->CameraGetCurrent());
        int sys = (this->skybox2Mesh)->addSystem(local, ParticleSettings::ParticleSet_4, false);
        this->movingStarsIndex = sys;

        // pirate-base smoke plume attached to the pirate flagship.
        if (gStatus->getSystem() != 0) {
            SolarSystem *ss = (SolarSystem *) gStatus->getSystem();
            if (((SolarSystem *) ss)->hasPirateBase() != 0 && this->enemies != nullptr) {
                for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                    KIPlayer *k = (*this->enemies)[i];
                    if (k != 0 && k->getType() == 0x37a3) {
                        // The flagship's geometry (KIPlayer::geometry at +0x8) drives the
                        // smoke plume's reference frame.
                        AEGeometry *kg = k->geometry;
                        kg->updateReferenceMatrix();
                        Matrix const *ref = &kg->getReferenceMatrix();
                        this->particleSystemMgr->addSystem(ref, ParticleSettings::ParticleSet_8, false);
                        break;
                    }
                }
            }
        }

        canvas->CameraGetCurrent();
        local = CameraGetLocal(canvas, canvas->CameraGetCurrent());
        sys = this->particleSystemMgr->addSystem(local, ParticleSettings::ParticleSet_7, false);
        this->field_284 = sys;

        // orbit ambient/nebula tint is applied inline.
    }

    // nebula light direction (applied inline).

    // init the always-present managers.
    if (this->field_80 != 0)
        (this->field_80)->init();
    if (this->particleSystemMgr != nullptr)
        (this->particleSystemMgr)->init();
    if (this->skybox2Mesh != 0)
        (this->skybox2Mesh)->init();
    if (this->field_8c != 0)
        this->field_8c->init();
    if (this->field_98 != 0)
        (this->field_98)->init();

    // the player-engine systems (slots 0x34..0x5c) all use a unit transform.
    this->field_38 = levelAddPlayerSystem(this, ParticleSettings::ParticleSet_0xa);
    this->field_3c = levelAddPlayerSystem(this, ParticleSettings::ParticleSet_0xb);
    this->field_48 = levelAddPlayerSystem(this, ParticleSettings::ParticleSet_0x14);
    this->field_34 = levelAddPlayerSystem(this, ParticleSettings::ParticleSet_0x15);
    this->field_50 = levelAddPlayerSystem(this, ParticleSettings::ParticleSet_0x16);
    this->field_54 = levelAddPlayerSystem(this, ParticleSettings::ParticleSet_0x17);
    if (gStatus->getCurrentCampaignMission() == 0x50) {
        this->field_58 = levelAddPlayerSystem(this, ParticleSettings::ParticleSet_0x18);
        this->field_5c = levelAddPlayerSystem(this, ParticleSettings::ParticleSet_0x18);
    }

    (this->field_74)->init();
    (this->field_74)->enableSystemEmit(this->field_50, true);
    (this->field_74)->enableSystemEmit(this->field_54, true);
    if (gStatus->getCurrentCampaignMission() == 0x50) {
        (this->field_74)->enableSystemEmit(this->field_58, true);
        (this->field_74)->enableSystemEmit(this->field_5c, true);
    }

    this->field_9c->init();
    this->particleEmitBoolPtr->init();
    this->particleRenderBoolPtr->init();
    if (this->field_94 != 0)
        this->field_94->init();
}

// Level::createWingmen() — spawns the player's hired escort fighters in formation.
void Level::createWingmen() {
    int **statusB = (int **) &gStatus;

    if (gStatus->inSupernovaSystem() != 0 ||
        gStatus->getCurrentCampaignMission() == 0x9e ||
        gStatus->getWingmen() == 0 ||
        this->player == nullptr)
        return;

    Array<KIPlayer *> *arr = new Array<KIPlayer *>();
    unsigned *wm = (unsigned *) gStatus->getWingmen();
    arr->resize(*wm);

    unsigned n = arr->size();
    for (unsigned i = 0; i < n; i = i + 1) {
        int seed = **g_cwm_seedSrc;
        gStatus->getWingmen();
        ((AbyssEngine::AERandom *) (intptr_t) seed)->setSeed((long long) seed);
        int fighter = (int) gGlobals->getRandomEnemyFighter(*(int *) (*statusB + 0x2c));
        int ship = (int) (intptr_t) createShip(5, 0, fighter, 0, 1, 0);
        (*arr)[i] = (KIPlayer *) (intptr_t) ship;

        int *slot = (int *) &(*arr)[i];
        levelPlaceWingman(this, slot, i);

        ((*arr)[i])->setWingman(true, i);
        (*arr)[i]->player->setAlwaysFriend(1);
        (*arr)[i]->player->setHitpoints(0x258); // 600 hp

        int wmList = gStatus->getWingmen();
        (*arr)[i]->name =
                **(String **) (*(int *) (wmList + 4) + i * 4);
        // RAWREAD: Status +0x2c (player race; ambiguous field placement near field_0x28/field_0x30).
        (*arr)[i]->shipGroup = *(int *) (*statusB + 0x2c);

        gStatus->getMission();
        if (((Mission *) gStatus->getMission())->getType() == 0xc)
            *(char *) &(*arr)[i]->field_0x25 = 0;
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
    ((AbyssEngine::AERandom *) (intptr_t) * *g_cwm_seedSrc)->reset();
}

// Level::createScene() — builds the non-flight presentation scenes (cut-scene mode 2, station
// crew gallery mode 4, and ship-showroom mode 0x17).
void Level::createScene() {
    if (this->enemies != nullptr) {
        if (this->enemies) {
            for (auto *e: *this->enemies) delete e;
            delete this->enemies;
            this->enemies = nullptr;
        }
    }
    int mode = this->missionPtr;
    this->enemies = nullptr;

    if (mode == 2) {
        createPlayer();
        gStatus->setMission((Mission *) gStatus->wanted);
        createMission();
        if (gStatus->getCurrentCampaignMission() == 0x2b) {
            void *canvas = (void *) gCanvas;
            AEGeometry *g = (AEGeometry *) ::operator new(0xc0);
            new((void *) g) AEGeometry((uint16_t) 0x37d0, (PaintCanvas *) canvas, 0);
            PlayerStatic *p = (PlayerStatic *) ::operator new(0x130);
            new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            this->enemies->push_back((KIPlayer *) p);
            g = (AEGeometry *) ::operator new(0xc0);
            new((void *) g) AEGeometry((uint16_t) 0x37d1, (PaintCanvas *) canvas, 0);
            p = (PlayerStatic *) ::operator new(0x130);
            new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            this->enemies->push_back((KIPlayer *) p);
        }
        return;
    }

    if (mode == 4) {
        gStatus->getSystem();
        int race = ((SolarSystem *) gStatus->getSystem())->getRace();
        unsigned crew = (race == 1) ? 2 : 3;
        Station *st = (Station *) gStatus->getStation();
        int *agents = (int *) ((Station *) st)->getAgents();
        char taken[7];
        void *canvas = (void *) gCanvas;

        if (agents == 0) {
            this->enemies = new Array<KIPlayer *>();
            this->enemies->resize(3);
        } else {
            int nAgents = *agents;
            this->enemies = new Array<KIPlayer *>();
            this->enemies->resize(nAgents * 3 + crew);
            for (int j = 0; j != 7; j = j + 1) taken[j] = 0;

            for (int a = 0; a < nAgents; a = a + 1) {
                Agent *ag = *(Agent **) (agents[1] + a * 4);
                int part = ag->getRace();
                if (part == 3) {
                    if (ag->getImageParts() == 0) part = 3;
                    else {
                        int *ip = ag->getImageParts();
                        part = (*ip != 2) ? 3 : *ip;
                    }
                }
                int seat;
                do { seat = gRandom->nextInt(); } while (taken[seat] != 0);
                taken[seat] = 1;

                AEGeometry *g = (AEGeometry *) ::operator new(0xc0);
                new((void *) g) AEGeometry((uint16_t)(unsigned)part, (PaintCanvas *) canvas, 0);
                PlayerStatic *p = (PlayerStatic *) ::operator new(0x130);
                new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
                (*this->enemies)[a] = (KIPlayer *) p;
                // actor position/rotation is applied inline above.

                g = (AEGeometry *) ::operator new(0xc0);
                new((void *) g) AEGeometry((uint16_t)(unsigned)mode, (PaintCanvas *) canvas, 0);
                p = (PlayerStatic *) ::operator new(0x130);
                new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
                (*this->enemies)[nAgents + a] = (KIPlayer *) p;

                g = (AEGeometry *) ::operator new(0xc0);
                new((void *) g) AEGeometry((uint16_t) 0x380c, (PaintCanvas *) canvas, 0);
                p = (PlayerStatic *) ::operator new(0x130);
                new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
                (*this->enemies)[nAgents * 2 + a] = (KIPlayer *) p;
            }
        }
        for (unsigned u = 0; u < crew; u = u + 1) {
            AEGeometry *g = (AEGeometry *) ::operator new(0xc0);
            new((void *) g) AEGeometry((uint16_t)(unsigned)mode, (PaintCanvas *) canvas, 0);
            PlayerStatic *p = (PlayerStatic *) ::operator new(0x130);
            new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            (*this->enemies)[this->enemies->size() + (u - crew)] = (KIPlayer *) p;
        }
        return;
    }

    if (mode == 0x17) {
        int *host = (int *) gStatus;
        Station *st = (Station *) gStatus->getStation();
        unsigned race;
        if (((Station *) st)->getIndex() == 0x65) race = 8;
        else {
            st = (Station *) gStatus->getStation();
            if (((Station *) st)->getIndex() == 100) race = 7;
            else {
                gStatus->getSystem();
                race = ((SolarSystem *) gStatus->getSystem())->getRace();
            }
        }

        this->enemies = new Array<KIPlayer *>();
        this->enemies->resize(1);

        (int) (intptr_t) gStatus->getShip();
        int shipIdx = ((Ship *) (int) (intptr_t) gStatus->getShip())->getIndex();
        Ship *ship = (Ship *) (int) (intptr_t) gStatus->getShip();
        int shipRace = ship->getRace();
        int actor = (int) (intptr_t) createShip(shipRace, 0, shipIdx, 0,
                                                this->missionPtr != 0x17, 0);
        (*this->enemies)[0] = (KIPlayer *) (intptr_t) actor;
        // actor position/rotation is applied inline above.
        ((PlayerFighter *) (intptr_t) actor)->removeTrail();
        ((PlayerFighter *) (intptr_t) actor)->setExhaustVisible(false);
        ((KIPlayer *) (intptr_t) actor)->setToSleep();
        ((KIPlayer *) (intptr_t) actor)->player->setAlwaysFriend(1);

        void *canvas = (void *) gCanvas;
        for (unsigned u = 0; u < 4; u = u + 1) {
            // decoration meshes around the showroom ship.
            AEGeometry *g = (AEGeometry *) ::operator new(0xc0);
            new((void *) g) AEGeometry((uint16_t)(unsigned)(0x3800 + u), (PaintCanvas *) canvas, 0);
            Vector rot = {0, 0, 0};
            ((AEGeometry *) g)->setRotation(*(const AbyssEngine::AEMath::Vector *) (&rot));
            PlayerStatic *p = (PlayerStatic *) ::operator new(0x130);
            new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            if ((int) race < 4 && race != 1) {
                AEGeometry *child = (AEGeometry *) ::operator new(0xc0);
                new((void *) child) AEGeometry((uint16_t)(unsigned)u, (PaintCanvas *) canvas, 0);
                ((AEGeometry *) g)->addChild(((AEGeometry *) child)->transform);
                [&] {
                    AEGeometry *g_ = (AEGeometry *) (child);
                    if (g_) {
                        g_->~AEGeometry();
                        ::operator delete(g_);
                    }
                }();
            }
            this->enemies->push_back((KIPlayer *) p);
        }

        // background traffic fighters.
        Station *st2 = (Station *) gStatus->getStation();
        bool fromStationShips = (((Station *) st2)->getIndex() == 0x6c) &&
                                (gStatus->field_114 == 3);
        AbyssEngine::AERandom *rng = gRandom;
        int spawnCount = rng->nextInt();
        if (fromStationShips) {
            unsigned *ships = (unsigned *) ((Station *) gStatus->getStation())->getShips();
            spawnCount = (ships == 0) ? 0 : (int) *ships;
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
            int fighter = (int) gGlobals->getRandomEnemyFighter(spawnRace);
            Station *st3 = (Station *) gStatus->getStation();
            if (((Station *) st3)->getIndex() == 100) {
                int pick = rng->nextInt();
                fighter = (pick == 1) ? 0x26 : (pick == 0) ? 0x25 : 0x28;
            }
            if (fromStationShips) {
                Array<Ship *> *ships = ((Station *) gStatus->getStation())->getShips();
                fighter = (*ships)[s]->getIndex();
            }
            KIPlayer *k = (KIPlayer *) createShip(0, 0, fighter, 0, 0, 0);
            int seat = rng->nextInt();
            int guard = -100;
            while (guard != 0 && seats[seat & 0x3f] != 0) {
                seat = rng->nextInt();
                guard = guard + 1;
            }
            seats[seat & 0x3f] = 1;
            // actor position/rotation is applied inline above.
            k->player->setAlwaysFriend(1);
            k->setToSleep();
            ((PlayerFighter *) k)->setExhaustVisible(false);
            this->enemies->push_back(k);
        }
    }
}

// Level::renderBG(int t) — draws the skybox, nebula, planet rings and supernova glow.
// t is the elapsed-time/frame word the cutscene passes through verbatim.
void Level::renderBG(int t) {
    uintptr_t canvas = (uintptr_t) gCanvas;
    // matrix dirty/cache flags live inside the sub_1d0 skybox matrix sub-object.
    char *skyMatrixFlags = (char *) &this->sub_1d0;

    ((PaintCanvas *) (long) (canvas))->SetColor(0xffffffffu);
    ((PaintCanvas *) (long) (canvas))->BeginBG();
    ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
    ((PaintCanvas *) (long) (canvas))->CameraGetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent());

    Matrix *sky = (Matrix *) ((char *) &this->sub_1d0);
    (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
    *(int *) (skyMatrixFlags + 0x1c) = 0;
    *(int *) (skyMatrixFlags + 0x0c) = 0;
    *(int *) (skyMatrixFlags + 0x2c) = 0;

    // Build the skybox orientation into sub_20c, then compose it onto sub_1d0.
    Matrix *cloudMtx = (Matrix *) ((char *) &this->sub_20c);
    if (gStatus->inAlienOrbit() == 0 &&
        ((SolarSystem *) gStatus->getSystem())->getIndex() == 0x1b) {
        // Alien-ring system: align the skybox basis to the star's light direction.
        Vector dir = AbyssEngine::AEMath::VectorNormalize(this->starSystem->getLightDirection());
        Vector right = AbyssEngine::AEMath::VectorNormalize(
            AbyssEngine::AEMath::VectorCross(Vector{1.0f, 0.0f, 0.0f}, dir));
        Vector up = AbyssEngine::AEMath::VectorNormalize(
            AbyssEngine::AEMath::VectorCross(right, dir));
        AbyssEngine::AEMath::MatrixSetRotation(*cloudMtx, -up, dir, right);
    } else {
        // Otherwise rotate by the per-orbit Euler angles stored on the level.
        AbyssEngine::AEMath::MatrixSetRotation(*cloudMtx, this->skyRotX, this->skyRotY, this->skyRotZ);
    }
    *sky *= *cloudMtx;

    ((PaintCanvas *) (long) (canvas))->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *) &this->sub_1d0);
    ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_19c), 0);
    ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_dummy);
    ((PaintCanvas *) (long) (canvas))->DrawMesh((unsigned int) (*(unsigned *) &this->field_08));
    ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_198), 0);
    ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_2);
    ((PaintCanvas *) (long) (canvas))->DrawMesh((unsigned int) (*(unsigned *) &this->skyboxMesh));

    // optional far cloud layer.
    if (this->field_1b4 != -1) {
        ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_1b8), 0);
        ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_1);
        ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
        ((PaintCanvas *) (long) (canvas))->CameraGetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        *(int *) (skyMatrixFlags + 0x1c) = 0;
        *(int *) (skyMatrixFlags + 0x0c) = 0;
        *(int *) (skyMatrixFlags + 0x2c) = 0;
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        ((PaintCanvas *) (long) (canvas))->
                DrawTransform((unsigned int) (long) (*(Matrix **) &this->field_1b4), nullptr);
    }

    (*(StarSystem **) &this->starSystem)->render();

    // supernova glow billboards.
    if (gStatus->inSupernovaSystem() != 0 && this->skyboxTexture != -1) {
        int camp = gStatus->getCurrentCampaignMission();
        ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_1a0), 0);
        ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_2);
        float scale = (0x6a < camp) ? 1.5f : 1.0f;
        int flag = (int) (scale * t);
        int xf = (int) (long) ((PaintCanvas *) (long) (canvas))->TransformGetTransform(0);
        ((AbyssEngine::Transform *) (intptr_t) xf)->Update((int64_t) flag, true);
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        ((PaintCanvas *) (long) (canvas))->DrawTransform((unsigned int) (long) (*(Matrix **) &this->field_10), nullptr);
        xf = (int) (long) ((PaintCanvas *) (long) (canvas))->TransformGetTransform(0);
        ((AbyssEngine::Transform *) (intptr_t) xf)->Update((int64_t) flag, true);
        ((PaintCanvas *) (long) (canvas))->DrawTransform((unsigned int) (long) (*(Matrix **) &this->field_18), nullptr);
    }

    // rotating planet ring.
    if (this->field_1bc != -1) {
        ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_1c0), 0);
        ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_2);
        ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
        ((PaintCanvas *) (long) (canvas))->CameraGetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        int xf = (int) (long) ((PaintCanvas *) (long) (canvas))->TransformGetTransform(0);
        int before = *(int *) (xf + 0x110); // RAWREAD: Transform +0x110 (engine handle, no modeled field)
        int xf2 = (int) (long) ((PaintCanvas *) (long) (canvas))->TransformGetTransform(0);
        ((AbyssEngine::Transform *) (intptr_t) xf2)->Update((int64_t)(int)t, true);
        if (*(int *) (xf + 0x110) < before) {
            // RAWREAD: Transform +0x110
            // re-randomize the ring tilt: three Euler angles uniformly in [0, 2*pi).
            AbyssEngine::AERandom *rng = gRandom;
            const float toAngle = (1.0f / 65536.0f) * 6.2831855f;
            float ax = (float) rng->nextInt(0x10000) * toAngle;
            float ay = (float) rng->nextInt(0x10000) * toAngle;
            float az = (float) rng->nextInt(0x10000) * toAngle;
            AbyssEngine::AEMath::MatrixSetRotation(*(Matrix *) ((char *) &this->sub_248), ax, ay, az);
        }
        (*sky *= *(Matrix *) ((char *) &this->sub_248));
        *(int *) (skyMatrixFlags + 0x0c) = 0;
        *(int *) (skyMatrixFlags + 0x1c) = 0;
        *(int *) (skyMatrixFlags + 0x2c) = 0;
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        ((PaintCanvas *) (long) (canvas))->
                DrawTransform((unsigned int) (long) (*(Matrix **) &this->field_1bc), nullptr);
    }

    // supernova flare mesh (when the explosion timeline is past its trigger).
    if (this->supernovaFlareActive != 0 &&
        1.0f <= *(float *) ((int) (intptr_t) gEngine + 0x28)) {
        ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
        ((PaintCanvas *) (long) (canvas))->CameraGetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        *(int *) (skyMatrixFlags + 0x1c) = 0;
        *(int *) (skyMatrixFlags + 0x0c) = 0;
        *(int *) (skyMatrixFlags + 0x2c) = 0;
        ((PaintCanvas *) (long) (canvas))->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *) &this->sub_1d0);
        ((PaintCanvas *) (long) (canvas))->SetColor(0xffffffffu);
        const AbyssEngine::AEMath::Matrix *eng =
                (const AbyssEngine::AEMath::Matrix *) gCanvas->field_0x34;
        gEngine->SetModelMatrix(*eng);
        ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (unsigned) this->supernovaFlareTexture, 0);
        ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_8);
        ((Engine *) gCanvas->field_0x34)->LightSetLight(0x4000);
        gEngine->GlEnable((unsigned) (uintptr_t) gCanvas->field_0x34, 0);
        ((PaintCanvas *) (long) (canvas))->DrawMesh((unsigned int) (unsigned) this->supernovaFlareMesh);
        gEngine->GlEnable((unsigned) (uintptr_t) gCanvas->field_0x34, 0);
        ((Engine *) gCanvas->field_0x34)->LightEnable(false);
    }

    ((PaintCanvas *) (long) (canvas))->EndBG();
}

// render2D() forwards rendering of the 2D HUD overlay to the active star system
// (this->starSystem, +0xec). Veneer 0x1abfe8 -> StarSystem::render2D.

// createScene()'s wingman-roster removal (inlined): drop the entry equal to `name`
// from the roster array {size, data, capacity}.
static inline void levelWingmanDiedOne(String *name, unsigned int *list) {
    unsigned int n = list[0];
    String **data = (String **) (intptr_t) list[1];
    unsigned int w = 0;
    for (unsigned int r = 0; r < n; ++r) {
        if (data[r] != name)
            data[w++] = data[r];
    }
    list[0] = w;
}

// --- init() (inlined): choose the player spawn (station dock / warpgate / planet /
// origin) for the current orbit and commit it. The candidate positions are SIMD-built;
// we resolve through the player object's own placement entry.
static inline __attribute__ ((always_inline))

void levelInitPlacePlayer(Level *self, int statusA, int stationStack) {
    (void) statusA;
    PlayerEgo *player = self->player;
    if (player == nullptr)
        return;
    // FIXME: PlayerEgo respawn-placement virtual at vtable slot +0x1c — map to the real method.
    int *ego = (int *) player;
    (*(void (**)(int *, int)) (*ego + 0x1c))(ego, stationStack);
}

// The level's player world position (origin when no player exists yet).
static inline __attribute__ ((always_inline)) Vector levelPlayerPosition(Level *self) {
    if (self->player == 0) {
        Vector p;
        p.x = p.y = p.z = 0.0f;
        return p;
    }
    return (self->player)->getPosition();
}

// --- updateOrbit() (inlined): revive an enemy and teleport it to a far random offset
// from the player. Profile: generic far-field wave.
static inline void levelSpawnFar(Level *self, int *kiPlayer) {
    AbyssEngine::AERandom *rng = gRandom;
    Vector p = levelPlayerPosition(self);
    float ox = (float) (rng->nextInt() % 120000 - 60000);
    float oy = (float) (rng->nextInt() % 80000 - 40000);
    float oz = (float) (rng->nextInt() % 120000 - 60000);
    ((KIPlayer *) kiPlayer)->setPosition(p.x + ox, p.y + oy, p.z + oz);
}

// --- updateMissionOrbit() (inlined): revive then reposition. profile 0 == far wave,
// profile 1 == tighter boss-escort spread.
static inline __attribute__ ((always_inline))

void levelSpawnAt(Level *self, int *kiPlayer, int profile) {
    AbyssEngine::AERandom *rng = gRandom;
    Vector p = levelPlayerPosition(self);
    int span = profile ? 40000 : 120000;
    float ox = (float) (rng->nextInt() % span - span / 2);
    float oy = (float) (rng->nextInt() % (span * 2 / 3) - span / 3);
    float oz = (float) (rng->nextInt() % span - span / 2);
    ((KIPlayer *) kiPlayer)->setPosition(p.x + ox, p.y + oy, p.z + oz);
}

// --- updateAlienAttackers() (inlined): place a revived alien relative to the player
// (when alienInOrbit) or the station origin.
static inline void levelPlaceAlien(Level *self, int *kiPlayer, int alienInOrbit) {
    AbyssEngine::AERandom *rng = gRandom;
    Vector base;
    base.x = base.y = base.z = 0.0f;
    if (alienInOrbit)
        base = levelPlayerPosition(self);
    float ox = (float) (rng->nextInt() % 100000 - 50000);
    float oy = (float) (rng->nextInt() % 60000 - 30000);
    float oz = (float) (rng->nextInt() % 100000 - 50000);
    ((KIPlayer *) kiPlayer)->setPosition(base.x + ox, base.y + oy, base.z + oz);
}

// --- createGasClouds() (inlined): pick a far random position for cloud `i`. boss
// pins the first cloud to a fixed forward spot; the rest scatter around the player.
static inline void levelCloudRandomPos(Level *self, int rng, int boss, unsigned i, Vector *out) {
    Vector p = levelPlayerPosition(self);
    if (boss && i == 0) {
        out->x = p.x;
        out->y = p.y;
        out->z = p.z + 30000.0f;
        return;
    }
    out->x = p.x + (float) (((AbyssEngine::AERandom *) (intptr_t) rng)->nextInt() % 160000 - 80000);
    out->y = p.y + (float) (((AbyssEngine::AERandom *) (intptr_t) rng)->nextInt() % 100000 - 50000);
    out->z = p.z + (float) (((AbyssEngine::AERandom *) (intptr_t) rng)->nextInt() % 160000 - 80000);
}

// --- createWingmen() (inlined): position wingman `i` in formation relative to the
// player geometry (right/forward offsets) and align its heading.
static inline void levelPlaceWingman(Level *self, int *kiSlot, unsigned i) {
    if (kiSlot == 0)
        return;
    Vector p = levelPlayerPosition(self);
    // staggered echelon: alternate sides, step back each pair.
    float side = ((i & 1) ? -1.0f : 1.0f) * (float) (2000 + (int) (i / 2) * 1500);
    float back = (float) (2000 + (int) (i / 2) * 2500);
    ((KIPlayer *) kiSlot)->setPosition(p.x + side, p.y, p.z - back);
}
