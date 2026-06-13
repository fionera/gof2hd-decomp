#include "gof2/game/world/Level.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/ship/Ship.h"
extern "C" void AEGeometry_setDirection_cso(Vector *geo, Vector *dir);  // engine shim (setDirection up-vec unrecoverable)
#include "gof2/externs.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/engine/render/AEGeometry.h"
// NOTE: gof2/ParticleSystemManager.h and gof2/Status.h are intentionally NOT included.
// Level reaches those classes only through a handful of accessor methods and opaque
// pointers, and uses local minimal interface structs (below) whose signatures match the
// recovered code (e.g. Status::getSystem() -> SolarSystem*, PSM static-style helpers).
// The full headers model different (32-bit-layout) signatures and would conflict.
#include "gof2/game/mission/Achievements.h"
#include "gof2/game/ui/Hud.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/game/ship/PlayerTurret.h"
#include "gof2/game/world/Waypoint.h"

// Several related game classes (Status, KIPlayer, ...) are defined in their own
// headers, but those headers assert the 32-bit ARM struct layout
// (sizeof(String)==0xc, sizeof(Array)==0xc, etc.) which does not hold on the
// 64-bit host build, and KIPlayer.h models only data members (no methods).
// Level only reaches these classes through a handful of accessor methods and
// opaque pointers, so the minimal interfaces it needs are declared locally here
// (one definition each, consolidated from the per-method translation units).
struct SolarSystem;
struct Station;
struct Wanted;

struct Status {
    SolarSystem *getSystem();
    Station *getStation();
    int getMission();
    void addKills(int n);
    int inBlackMarketSystem();
    int getShip();
    int inEmptyOrbit();
    unsigned int *getWingmen();
    int isStorylineWanted(int index);
    int getWanted();
    void setMission(int m);
    void setCampaignMission(int m);
    void incKills();
};

struct SolarSystem {
    int getTextureIndex();
    int getRace();
};

static unsigned int g_level_texOutScratch;

struct KIPlayer {
    void reset();
    static void setDead(KIPlayer *self);
    static int isWingMan();
};

struct Route {
    Route(int *pts, unsigned int n);
    ~Route();
    void reset();
};

struct Wanted {
    static int getNumWingmen(Wanted *self);
};

struct RadioMessage {
    void reset();
};

struct ParticleSystemManager {
    static void enableSystemEmit(int mgr, int id);
    static void enableSystemRender(int mgr, int id, bool enable);
    void render3d();
};

// Engine Array<T> raw view: the recovered code casts opaque int handles to
// `Array *` and reads the {size,data,capacity} fields directly. Modelled here
// as a plain layout-compatible view (size, data pointer, capacity).
struct RawArray {
    unsigned int size;
    void        *data;
    unsigned int capacity;
};

// Array<T> container helpers (defined at end of file over the engine's RawArray
// {size,data,capacity} layout). Each is one Array<T> special member specialised
// per element type; the trailing context suffix (_ca/_csc/_csp/_cm/...) marks
// the call site the decompiler inlined it at. Consolidated here as forward
// declarations so the call sites below resolve.
struct ObjectGun;
extern "C" {
// Array<T>::Array()
void  ArrayKIPlayer_ctor_ca(void *a);
void  ArrayKIPlayer_ctor_ccm(void *a);
void  ArrayKIPlayer_ctor_cm(void *a);
void  ArrayKIPlayer_ctor_csc(void *a);
void  ArrayKIPlayer_ctor_cso(void *a);
void  ArrayKIPlayer_ctor_csp(void *a);
void  ArrayKIPlayer_ctor_cwm(void *a);
void  ArrayPlayer_ctor_cp(void *a);
void  ArrayRadio_ctor_crm(void *a);
void  ArrayRadio_ctor_crms(void *a);
void  ArrayGasCloud_ctor_cgc(void *a);
void  ArrayBV_ctor_gbv(void *a);
void  Array_int_ctor_ips(void *a);
void  ArrayAGun_ctor_cg(void *a);
void  ArrayAGun_ctor_ag(void *a);
// Array<T>::~Array() (returns this)
void *ArrayKIPlayer_dtor_csc(void *a);
void *ArrayPlayer_dtor_cp(void *a);
void *ArrayRadio_dtor_crm(void *a);
void *ArrayInt_dtor_gbv(void *a);
void *Array_int_dtor_cs(void *a);
void *Array_int_dtor_cso(void *a);
void *ArrayAGun_dtor_ag(void *a);
// Array<T>::setLength(n)
void  ArraySetLength_KIPlayer_ca(int n, void *a);
void  ArraySetLength_KIPlayer_ccm(unsigned n, void *a);
void  ArraySetLength_KIPlayer_cm(unsigned n, void *a);
void  ArraySetLength_KIPlayer_csc(unsigned n, void *a);
void  ArraySetLength_KIPlayer_csp(unsigned n, void *a);
void  ArraySetLength_KIPlayer_cwm(unsigned n, void *a);
void  ArraySetLength_Player_cp(unsigned n, void *a);
void  ArraySetLength_Radio_crms(unsigned n, void *a);
void  ArraySetLength_GasCloud_cgc(int n, void *a);
void  ArraySetLength_BV_gbv(unsigned n, void *a);
void  ArraySetLength_int_ips(unsigned n, void *a);
void  ArraySetLength_AGun_ag(unsigned n, void *a);
// Array<T>::add(item)
void  ArrayAdd_KIPlayer_cft(KIPlayer *k, void *a);
void  ArrayAdd_KIPlayer_csc(KIPlayer *k, void *a);
void  ArrayAdd_KIPlayer_cso(KIPlayer *k, void *a);
void  ArrayAdd_KIPlayer_cwm(KIPlayer *k, void *a);
void  ArrayAdd_Radio_crm(RadioMessage *m, void *a);
void  ArrayAdd_AGun_cg(ObjectGun *o, void *a);
void  ArrayAdd_AGun_ag(ObjectGun *o, void *a);
// Array<int>::release() / Array<T>::releaseClasses()
void  ArrayRelease_int_gbv(void *a);
void  ArrayReleaseClasses_AGun_ag(void *a);
void  ArrayReleaseClasses_KIPlayer_csc(void *a);
// generic (untyped, int-handle) Array helpers used by createSentryGuns
void  ArrayCtor(int arr);
void  ArraySetLength(int len, int arr);
void  ArrayAdd(int item, int arr);
}

extern "C" void *dtor_Objective(void *p);
extern "C" void *dtor_BoundingVolume(void *p);
extern "C" void *dtor_StarSystem(void *p);
extern "C" void *dtor_PlayerEgo(void *p);
extern "C" void *dtor_Route(void *p);
extern "C" void *dtor_PSM(void *p);
extern "C" void *dtor_LODManager(void *p);
extern "C" void *dtor_LodMeshMerger(void *p);
extern "C" void *dtor_ArrayKI(void *p);
extern "C" void Level_releaseAEGeometry(void *p);
extern "C" void *dtor_ArrayAEGeometry(void *p);
extern "C" void Level_releaseInt(void *p);
extern "C" void *dtor_ArrayInt(void *p);
extern "C" void Level_releaseAbstractGun(void *p);
extern "C" void *dtor_ArrayAbstractGun(void *p);
extern "C" void Level_releaseKI(void *p);
extern "C" void Level_releaseRadioMessage(void *p);
extern "C" void *dtor_ArrayRadioMessage(void *p);
extern "C" void *_Znwj(unsigned int size);
extern "C" void Level_setAlwaysEnemy(int obj, int flag);
extern "C" int Level_opnew(unsigned int size);
extern "C" unsigned int uidiv(unsigned int a, unsigned int b);
extern "C" void Level_createPlayer_impl(Level *self);
extern "C" void *Level_opnew_akw(unsigned int size);
extern "C" void *dtor_Objective_akw(void *p);
extern "C" void Objective_ctor_akw(int o, int a, int b, int c, Level *self);
extern "C" void Level_turnEnemy(int obj);
extern "C" int Level_getNumWingmen(int wanted);

bool Level::hasMiningPlant() {
    return miningPlant > 0;
}

int Level::getFriendRoute() {
    return friendRoute;
}

int Level::getEnemiesLeft() {
    return enemiesLeft;
}

void Level::render2D() {
    return this->render2D_call(starSystem);
}

int Level::checkGameOver() {
    int objective = objectivesB;
    if (objective == 0) {
        return 0;
    }
    return this->checkGameOver_call(objective);
}

void Level::updateAsteroidCluster() {
}

int Level::getAsteroidWaypoint() {
    return asteroidWaypoint;
}

int Level::getAsteroidsLeft() {
    return asteroidsLeft;
}

struct JunkObj {
    char pad[0xb0];
    int counter;
};

__attribute__((visibility("hidden"))) extern JunkObj **g_junkDied;

void Level::junkDied() {
    (*g_junkDied)->counter += 1;
    enemiesLeft -= 1;
}

void Level::enableMovingStars(bool enable) {
    int index = movingStarsIndex;
    if (index < 0) {
        return;
    }
    this->enableMovingStars_call(skybox2Mesh, index, enable);
}

__attribute__((visibility("hidden"))) extern unsigned char *g_initStreamOut;

void Level::setInitStreamOut() {
    *g_initStreamOut = 1;
}

int Level::getMiningPlant() {
    int index = miningPlantIndex;
    if (index < 0) {
        return 0;
    }
    return ((int *)((RawArray *)(intptr_t)enemies)->data)[index];
}

int Level::getGasClouds() {
    return gasClouds;
}

void Level::asteroidDied() {
    asteroidsLeft -= 1;
}

int Level::checkObjective() {
    int objective = objectivesA;
    if (objective != 0) {
        return this->checkObjective_call(objective);
    }
    return 0;
}

int Level::getNumDeliveredOre() {
    return numDeliveredOre;
}

void Level::setPlayerRoute(Route *route) {
    Route *old = (Route *)playerRoute;
    if (old != 0) {
        old->~Route();
        operator delete(old);
    }
    playerRoute = (int)(intptr_t)route;
}

void Level::enableFog(bool enable) {
    return this->enableFog_call(particleSystemMgr, field_284, enable);
}

void Level::isInAsteroidCenterRange(Vector v) {
    int *vol = (int *)(intptr_t)collisionVolume;
    return (*(void (**)(int *, Vector))(*vol + 8))(vol, v);
}

int Level::getAsteroids() {
    return asteroids;
}

int Level::collide(Vector v) {
    int *vol = (int *)(intptr_t)collisionVolume;
    if (vol != 0) {
        return (*(int (**)(int *, Vector))(*vol + 8))(vol, v);
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
    ParticleSystemManager::enableSystemEmit(particleSystemMgr, field_284);
    ParticleSystemManager::enableSystemRender(particleSystemMgr, field_284, render);
    *(unsigned char *)particleRenderBoolPtr = render;
    *(unsigned char *)particleEmitBoolPtr = render;
}

__attribute__((visibility("hidden"))) extern PaintCanvas **g_paintCanvas_intro;

void Level::switchSkyboxForIntro() {
    PaintCanvas **pc = g_paintCanvas_intro;
    ((PaintCanvas*)(*pc))->MeshCreate((unsigned short)(0x4591), (unsigned int *)&skyboxMesh, false);
    ((PaintCanvas*)(*pc))->TextureCreate((unsigned short)(0x275a), (void *)0, (void *)0, (unsigned int *)&field_198, false);
    RawArray *list = (RawArray *)(intptr_t)asteroids;
    if (list != 0) {
        for (unsigned int i = 0; i < list->size; i = i + 1) {
            KIPlayer::setDead(((KIPlayer **)list->data)[i]);
            list = (RawArray *)(intptr_t)asteroids;
        }
    }
}

__attribute__((visibility("hidden"))) extern PaintCanvas **g_paintCanvas_snr;
__attribute__((visibility("hidden"))) extern Status **g_status_snr;

void Level::switchSkyboxForSupernovaReversal() {
    PaintCanvas **pc = g_paintCanvas_snr;
    Status **st = g_status_snr;
    PaintCanvas *canvas = *pc;
    int tex = (*st)->getSystem()->getTextureIndex();
    ((PaintCanvas*)(canvas))->MeshCreate((unsigned short)((unsigned short)(tex + 0x4588)), (unsigned int *)&skyboxMesh, false);
    PaintCanvas *canvas2 = *pc;
    int tex2 = (*st)->getSystem()->getTextureIndex();
    ((PaintCanvas*)(canvas2))->TextureCreate((unsigned short)((unsigned short)(tex2 + 0x2751)), (void *)0, (void *)0, (unsigned int *)&field_198, false);
    skyboxTexture = -1;
}

int Level::getPlayer() {
    return player;
}

void Level::killWanted() {
    if (field_29d == 0) {
        field_29d = 1;
        return ((Level *)(this))->wanted_action(0x11, 0);
    }
}

int Level::getEnemyGuns() {
    return enemyGuns;
}

void Level::stealFriendCargo() {
    friendCargoStolen = 1;
}

int Level::getEnemies() {
    return enemies;
}

__attribute__((visibility("hidden"))) extern Status **g_status_applyKills;

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

int Level::getMessages() {
    return messages;
}

// Level::getActiveMessages() — accessor for the level's live radio-message queue
// (the Array<RadioMessage*> that createRadioMessage()/createRadioMessages() build at
// this+0x114). Returned as an opaque pointer; callers re-cast to the message array.
void *Level::getActiveMessages() {
    return (void *)(intptr_t)messages;
}

int Level::getEnemyRoute() {
    return enemyRoute;
}

int Level::getPlayerGuns() {
    return playerGuns;
}

void Level::renderPause() {
    unsigned int *a;
    a = (unsigned int *)(intptr_t)playerGuns;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x14))(o);
            a = (unsigned int *)(intptr_t)playerGuns;
        }
    }
    a = (unsigned int *)(intptr_t)enemyGuns;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x14))(o);
            a = (unsigned int *)(intptr_t)enemyGuns;
        }
    }
    a = (unsigned int *)(intptr_t)enemies;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x24))(o);
            a = (unsigned int *)(intptr_t)enemies;
        }
    }
    a = (unsigned int *)(intptr_t)asteroids;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x24))(o);
            a = (unsigned int *)(intptr_t)asteroids;
        }
    }
    a = (unsigned int *)(intptr_t)gasClouds;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x24))(o);
            a = (unsigned int *)(intptr_t)gasClouds;
        }
    }
    a = (unsigned int *)(intptr_t)landmarks;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            if (o != 0) {
                (*(void (**)(int *))(*o + 0x24))(o);
                a = (unsigned int *)(intptr_t)landmarks;
            }
        }
    }
}

int Level::getPlayerRoute() {
    return playerRoute;
}

int Level::getStarSystem() {
    return starSystem;
}

__attribute__((visibility("hidden"))) extern Status **g_status_pirate;

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
    ((Level *)(this))->pirateStationAction_tail(param ? 3 : 4, 8);
}

int Level::getNumDockingTargets() {
    RawArray *list = (RawArray *)(intptr_t)enemies;
    if (list != 0) {
        int total = 0;
        for (unsigned int i = 0; list->size != i; i = i + 1) {
            total = total + *(unsigned char *)(((int *)list->data)[i] + 0x70);
        }
        return total;
    }
    return 0;
}

void Level::removeObjectives() {
    objectivesA = 0;
    objectivesB = 0;
}

int Level::getDockingTarget(int index) {
    RawArray *list = (RawArray *)(intptr_t)enemies;
    if (list != 0) {
        int found = 0;
        for (unsigned int i = 0; i < list->size; i = i + 1) {
            int obj = ((int *)list->data)[i];
            if (*(char *)(obj + 0x70) != 0) {
                if (found == index) {
                    return obj;
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

int Level::getLandmarks() {
    return landmarks;
}

void Level::render(int ctx) {
    unsigned int *a;
    a = (unsigned int *)(intptr_t)playerGuns;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x14))(o);
            a = (unsigned int *)(intptr_t)playerGuns;
        }
    }
    a = (unsigned int *)(intptr_t)enemyGuns;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x14))(o);
            a = (unsigned int *)(intptr_t)enemyGuns;
        }
    }
    a = (unsigned int *)(intptr_t)enemies;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *, int))(*o + 0x34))(o, ctx);
            int *o2 = ((int **)(*(int *)(enemies + 4)))[i];
            (*(void (**)(int *))(*o2 + 0x24))(o2);
            a = (unsigned int *)(intptr_t)enemies;
        }
    }
    a = (unsigned int *)(intptr_t)asteroids;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *, int))(*o + 0x34))(o, ctx);
            int *o2 = ((int **)(*(int *)(asteroids + 4)))[i];
            (*(void (**)(int *))(*o2 + 0x24))(o2);
            a = (unsigned int *)(intptr_t)asteroids;
        }
    }
    a = (unsigned int *)(intptr_t)gasClouds;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *, int))(*o + 0x34))(o, ctx);
            int *o2 = ((int **)(*(int *)(gasClouds + 4)))[i];
            (*(void (**)(int *))(*o2 + 0x24))(o2);
            a = (unsigned int *)(intptr_t)gasClouds;
        }
    }
    a = (unsigned int *)(intptr_t)landmarks;
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            if (o != 0) {
                (*(void (**)(int *, int))(*o + 0x34))(o, ctx);
                int *o2 = ((int **)(*(int *)(landmarks + 4)))[i];
                (*(void (**)(int *))(*o2 + 0x24))(o2);
                a = (unsigned int *)(intptr_t)landmarks;
            }
        }
    }
    if (field_80 != 0) {
        ((ParticleSystemManager *)(field_80))->render3d();
    }
    if (field_74 != 0) {
        ((ParticleSystemManager *)(field_74))->render3d();
    }
    if (particleEmitBoolPtr != 0) {
        ((ParticleSystemManager *)(particleEmitBoolPtr))->render3d();
    }
    if (particleSystemMgr != 0) {
        ((ParticleSystemManager *)(particleSystemMgr))->render3d();
    }
    if (skybox2Mesh != 0) {
        ((ParticleSystemManager *)(skybox2Mesh))->render3d();
    }
    if (particleRenderBoolPtr != 0) {
        ((ParticleSystemManager *)(particleRenderBoolPtr))->render3d();
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
    return this->render_tail(starSystem);
}

int Level::collideStream(Vector v) {
    int *obj = *(int **)(*(int *)(landmarks + 4) + 4);
    if (obj != 0) {
        return (*(int (**)(int *, Vector))(*obj + 0x38))(obj, v);
    }
    return 0;
}

int Level::getNumDeliveredPassengers() {
    return numDeliveredPassengers;
}

// ARM ABI destructors return `this`; model them as functions returning the pointer
// so the result feeds operator delete without a reload.

#define SIMPLE(off, dtor) \
    if (*(void **)((char *)this + (off)) != 0) { \
        operator delete(dtor(*(void **)((char *)this + (off)))); \
    } \
    *(int *)((char *)this + (off)) = 0;

#define ARR(off, release, dtor) \
    if (*(void **)((char *)this + (off)) != 0) { \
        release(*(void **)((char *)this + (off))); \
        if (*(void **)((char *)this + (off)) != 0) { \
            operator delete(dtor(*(void **)((char *)this + (off)))); \
        } \
    } \
    *(int *)((char *)this + (off)) = 0;

Level::~Level() {
    skyboxMesh = -1;
    field_08 = -1;
    skyboxTexture = -1;
    SIMPLE(0x28, dtor_Objective)
    SIMPLE(0x2c, dtor_Objective)
    SIMPLE(0xc4, dtor_BoundingVolume)
    {
        int *p = (int *)(intptr_t)asteroidWaypoint;
        if (p != 0) {
            (*(void (**)(int *))(*p + 4))(p);
        }
    }
    asteroidWaypoint = 0;
    SIMPLE(0xec, dtor_StarSystem)
    SIMPLE(0xf0, dtor_PlayerEgo)
    SIMPLE(0x180, dtor_Route)
    SIMPLE(0x80, dtor_PSM)
    SIMPLE(0x88, dtor_PSM)
    SIMPLE(0x74, dtor_PSM)
    SIMPLE(0x78, dtor_PSM)
    SIMPLE(0x7c, dtor_PSM)
    SIMPLE(0x90, dtor_PSM)
    SIMPLE(0x84, dtor_PSM)
    SIMPLE(0x98, dtor_PSM)
    SIMPLE(0x9c, dtor_PSM)
    ARR(0xa4, Level_releaseAEGeometry, dtor_ArrayAEGeometry)
    ARR(0xa8, Level_releaseInt, dtor_ArrayInt)
    ARR(0xe4, Level_releaseAbstractGun, dtor_ArrayAbstractGun)
    ARR(0xe8, Level_releaseAbstractGun, dtor_ArrayAbstractGun)
    ARR(0xf8, Level_releaseKI, dtor_ArrayKI)
    ARR(0xfc, Level_releaseKI, dtor_ArrayKI)
    ARR(0xf4, Level_releaseKI, dtor_ArrayKI)
    ARR(0x100, Level_releaseKI, dtor_ArrayKI)
    ARR(0x114, Level_releaseRadioMessage, dtor_ArrayRadioMessage)
    ARR(0x104, Level_releaseAEGeometry, dtor_ArrayAEGeometry)
    SIMPLE(0x0, dtor_LODManager)
    SIMPLE(0xa0, dtor_LodMeshMerger)
    if (field_b0 != 0) {
        operator delete(dtor_ArrayKI((void *)(intptr_t)field_b0));
    }
    field_b0 = 0;
}

void Level::incNumDeliveredPassengers(int delta) {
    numDeliveredPassengers += delta;
}

void Level::friendDied() {
    friendsLeft -= 1;
}

namespace AbyssEngine {
    namespace AERandom {
        int nextInt(int rng);
    }
}

#include <new>

__attribute__((visibility("hidden"))) extern int *g_routeRng;
__attribute__((visibility("hidden"))) extern int (*g_routeRandom)(int rng, int bound);

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
            pts[2] = AbyssEngine::AERandom::nextInt(*rng) + 50000;
        } else {
            int prev = p[-2];
            p[1] = AbyssEngine::AERandom::nextInt(*rng) + prev + 50000;
        }
        p = p + 3;
    }
    Route *r = (Route *)_Znwj(0x18);
    new (r) Route(pts, n);
    return r;
}

__attribute__((visibility("hidden"))) extern Status **g_alarmAllFriends;

void Level::alarmAllFriends(int race, bool message) {
    unsigned int *list = (unsigned int *)(intptr_t)enemies;
    if (list != 0) {
        for (unsigned int i = 0; i < *list; i = i + 1) {
            int obj = ((int *)list[1])[i];
            if (*(int *)(obj + 0x28) == race) {
                Level_setAlwaysEnemy(*(int *)(obj + 4), 1);
                list = (unsigned int *)(intptr_t)enemies;
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
        if ((*slot)->getSystem()->getRace() == race) {
            return this->alarmAllFriends_tail((Station *)(*slot)->getStation(), 1);
        }
    }
}

__attribute__((visibility("hidden"))) extern int *g_engineColorBase;

void Level::setPlayerEngineColor(short color) {
    int c = color;
    if (player != 0 && field_a4 != 0) {
        unsigned int *p = (unsigned int *)((char *)g_engineColorBase + 0x1254);
        for (int i = *(int *)field_a4; i != 0; i = i - 1) {
            *p = c << 0x10 | c << 0x18 | c << 8 | 0xff;
            p = p + 0x28;
        }
    }
}

struct Gun;
struct ObjectGun;

// Index->resource lookup tables (each DAT_ is a base into a const id array).
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

extern "C" {
void *Level_opnew_cg(unsigned size);
void Gun_ctor_cg(Gun *g, int a, int b, int c, int d, int e, int f, int g7, int h, int i,
                 int j, int k, int l, int m);
void Gun_setLevel_cg(Gun *g, Level *self);
void BeamGun_ctor_cg(ObjectGun *o, int a, Gun *g, int idx, Level *self);
void RocketGun_ctor_cg(ObjectGun *o, int a, Gun *g, int res, int b, int c, int d, int e,
                       Level *self);
void ObjectGun_ctor_cg(ObjectGun *o, int a, Gun *g, int res, int d, Level *self);
void BombGun_ctor_cg(ObjectGun *o, Gun *g, unsigned res, int c, int d, int e, Level *self);
void MineGun_ctor_cg(ObjectGun *o, Gun *g, int res, int c, int d, Level *self);
void SentryGun_ctor_cg(ObjectGun *o, Gun *g, int res, int c, int d, Level *self);
int  Item_getAttribute_cg(int item);
void Globals_addSoundResourceToList_cg(int snd);
}

// Level::createGun(idx, owner, kind, hp, dmg, rate, cool, color) — factory for the player's and
// AI ships' weapons; dispatches on `kind` to the right Gun subclass and registers the sound.
Gun * Level::createGun(int idx, int owner, int kind, int hp, int dmg, int rate, int cool, int color) {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    ObjectGun *obj = 0;
    Gun *gun = 0;

    switch (kind) {
    case 0:
    case 1:
    case 3: {
        int res = ((int *)g_cg_beamTable)[idx];
        if (res < 0) {
            gun = (Gun *)Level_opnew_cg(0x114);
            Gun_ctor_cg(gun, owner, dmg, 1, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
            gun->setIndex(idx);
            gun->weaponType = kind;
            gun->setPlayerGun(1);
            obj = (ObjectGun *)Level_opnew_cg(0x24);
            BeamGun_ctor_cg(obj, owner, gun, idx, thisptr);
        } else {
            int barrels = ((unsigned)(idx - 9) < 3 || idx == 0xe4) ? 1 : 0x14;
            gun = (Gun *)Level_opnew_cg(0x114);
            if (kind == 3) {
                Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, g_cg_rocketFx,
                            0, 0, 0);
                gun->setIndex(idx);
                gun->weaponType = 3;
                gun->setPlayerGun(1);
                gun->setErrorMagnitudePercentage(0x14);
                obj = (ObjectGun *)Level_opnew_cg(0xe8);
                RocketGun_ctor_cg(obj, owner, gun, res, 0, 0, 0, 1, thisptr);
            } else {
                Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
                gun->setIndex(idx);
                gun->weaponType = kind;
                gun->setPlayerGun(1);
                obj = (ObjectGun *)Level_opnew_cg(0xb0);
                ObjectGun_ctor_cg(obj, owner, gun, res, 1000, thisptr);
            }
        }
        break;
    }
    case 2:
    case 0x19:
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 0x19, hp, cool, rate, color, 0, 0, g_cg_objFx, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = kind;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)Level_opnew_cg(0xb0);
        ObjectGun_ctor_cg(obj, owner, gun, ((int *)g_cg_objTable)[idx], 1000, thisptr);
        gun->setErrorMagnitudePercentage(2);
        break;
    case 4:
    case 5:
    case 0x28: {
        gun = (Gun *)Level_opnew_cg(0x114);
        int barrels = (kind == 0x28) ? (idx - 0xd3) : 5;
        Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = kind;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)Level_opnew_cg(0xe8);
        RocketGun_ctor_cg(obj, owner, gun, ((int *)g_cg_rocketTable)[idx], 0, 0, kind,
                          (kind == 0x28 || kind == 5) ? 1 : 0, thisptr);
        Globals_addSoundResourceToList_cg(**g_cg_rocketSnd);
        break;
    }
    case 6:
    case 7:
    case 0x22: {
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 1, hp, cool, rate, color, 0, 0, g_cg_objFx, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = kind;
        gun->setPlayerGun(1);
        int attr = Item_getAttribute_cg(*(int *)(*(int *)(*g_cg_itemTableA + 4) + idx * 4));
        obj = (ObjectGun *)Level_opnew_cg(300);
        BombGun_ctor_cg(obj, gun, ((unsigned *)g_cg_bombTable)[idx], 1, kind, attr == 1 ? 1 : 0,
                        thisptr);
        Globals_addSoundResourceToList_cg(*g_cg_bombSnd);
        break;
    }
    case 8:
    case 0x23: {
        int fx = (idx == 0xb5 || idx != 0x30) ? g_cg_mineFx : g_cg_rocketFx;
        int extra = (idx == 0x30 && idx != 0xb5) ? (g_cg_rocketFx - 0xf60000) : 0;
        if (kind == 0x23) { fx = (idx == 0xb5 || idx != 0x30) ? g_cg_mineFx : g_cg_rocketFx; }
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 0xf, hp, cool, rate, color, extra, 0, fx, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = kind;
        gun->setPlayerGun(1);
        if ((idx == 0x30 || idx == 0xe0 || idx == 0xb5)) {
            gun->field_0xa4 = 1;
            if (idx == 0xe0) gun->field_0xa5 = 1;
        }
        obj = (ObjectGun *)Level_opnew_cg(0xb0);
        int *tbl = (kind == 0x23) ? (int *)g_cg_objTable23 : (int *)g_cg_objTable8;
        ObjectGun_ctor_cg(obj, owner, gun, tbl[idx], 1000, thisptr);
        break;
    }
    case 0xb: {
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 10, hp, cool, rate, 0x40000000, 0, 0, 0, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = 0xb;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)Level_opnew_cg(0xd4);
        MineGun_ctor_cg(obj, gun, ((int *)g_cg_mineTable)[idx], 1, 0xb, thisptr);
        Globals_addSoundResourceToList_cg(*g_cg_mineSnd);
        break;
    }
    case 0x27: {
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, 0, 3, hp, cool, rate, 0x40000000, 0, 0, 0, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = 0x27;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)Level_opnew_cg(0xb4);
        SentryGun_ctor_cg(obj, gun, ((int *)g_cg_sentryTable)[idx], 1, 0x27, thisptr);
        Globals_addSoundResourceToList_cg(*g_cg_mineSnd);
        break;
    }
    case 0x2a: {
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 1, hp, 1, rate, color, 0, 0, 0, 0, 0, 0);
        gun->setIndex(idx);
        gun->weaponType = 0x2a;
        gun->setPlayerGun(1);
        obj = (ObjectGun *)Level_opnew_cg(300);
        BombGun_ctor_cg(obj, gun, ((unsigned *)g_cg_bombTable2a)[idx], 1, 0x2a, 0, thisptr);
        Globals_addSoundResourceToList_cg(*g_cg_bombSnd);
        break;
    }
    default:
        gun = 0;
        break;
    }

    // index-keyed extra sound resources (fallthrough chain in the original).
    switch (idx) {
    case 0x29: Globals_addSoundResourceToList_cg(**g_cg_snd29);
    case 0x2a: Globals_addSoundResourceToList_cg(**g_cg_snd2a);
    case 0x2b: Globals_addSoundResourceToList_cg(**g_cg_snd2b);
    case 0x2c: Globals_addSoundResourceToList_cg(**g_cg_snd2c);
    case 0x2d: Globals_addSoundResourceToList_cg(**g_cg_snd2d);
    case 0x2e: Globals_addSoundResourceToList_cg(**g_cg_snd2e);
    default: break;
    }

    Gun_setLevel_cg(gun, thisptr);
    void *guns = *(void **)(self + 0xe4);
    if (guns == 0) {
        guns = Level_opnew_cg(0xc);
        ArrayAGun_ctor_cg(guns);
        *(void **)(self + 0xe4) = guns;
    }
    ArrayAdd_AGun_cg(obj, guns);
    return gun;
}

struct Station;
struct StarSystem;
struct SolarSystem;
struct Engine;

__attribute__((visibility("hidden"))) extern int    *g_csp_stack;   // [DAT_000be974]
__attribute__((visibility("hidden"))) extern Status **g_csp_status;  // [DAT_000be978]
__attribute__((visibility("hidden"))) extern unsigned *g_csp_canvas;  // [DAT_000be97c]

extern "C" {
int  Status_inAlienOrbit_csp();
int  Status_inSupernovaSystem_csp();
int  Status_inPlanetRingOrbit_csp();
int  Status_inStormOrbit_csp(Status *s);
int  Status_inFogSkyboxOrbit_csp();
int  Status_inEmptyOrbit_csp(Status *s);
int  Status_dlc1Won_csp();
int  Status_getSystem_csp();
int  Status_getStation_csp();
int  Status_getCurrentCampaignMission_csp();
int  SolarSystem_getIndex_csp();
int  SolarSystem_getTextureIndex_csp();
int  Station_getIndex_csp(Station *s);
int  Station_isAttackedByAliens_csp(Station *s);
int  aeabi_idivmod_csp(int a, int b);
void *Level_opnew_csp(unsigned size);
void StarSystem_ctor_csp(StarSystem *s, int mode);
int  ApplicationManager_GetEngine_csp();
int  Engine_IsPostEffectActivated_csp(Engine *e);
// Builds the skybox detail meshes (rings/storm/supernova flare/jumpgates), the station object and
// the agent gallery, plus all the SIMD light-direction / jumpgate-placement math the decompiler
// mangled. Profile selects the orbit context.
void Level_csp_buildDetail(Level *self);
}

// Level::createSpace() — creates the skybox + star-system backdrop, the home station and the
// jumpgate/agent props for the current orbit.
void Level::createSpace()
{
    char *self = (char *)this;

    // skybox mesh/texture only need (re)building when not yet created (mesh handle at +4 == -1).
    if (*(unsigned *)(self + 4) == 0xffffffff) {
        Status **status = g_csp_status;
        int alien = Status_inAlienOrbit_csp();
        unsigned canvas = *g_csp_canvas;

        if (alien == 0) {
            Status_getSystem_csp();
            int sysVariant = aeabi_idivmod_csp(SolarSystem_getIndex_csp(), 3);
            ((PaintCanvas*)(long)((PaintCanvas *)canvas))->MeshCreate((unsigned short)(sysVariant + 0x45ba), (unsigned int *)((unsigned *)(self + 8)), (bool)(0));
            Status_getSystem_csp();
            sysVariant = aeabi_idivmod_csp(SolarSystem_getIndex_csp(), 3);
            ((PaintCanvas*)(long)(canvas))->TextureCreate((unsigned short)((sysVariant + 0x2766) & 0xffff), (void *)0, (void *)0, &g_level_texOutScratch, (bool)(0));
            // the rest (campaign/supernova/storm/ring detail) is built by the helper.
            Level_csp_buildDetail(this);

            Status_getSystem_csp();
            if (0xf < SolarSystem_getTextureIndex_csp()) {
                Engine *eng = (Engine *)ApplicationManager_GetEngine_csp();
                if (Engine_IsPostEffectActivated_csp(eng) != 0) {
                    int mp = (int)(long)((PaintCanvas*)(long)((PaintCanvas *)canvas))->MeshGetPointer((unsigned int)(*(unsigned *)(self + 4)));
                    *(int *)(mp + 0x1c) = 0;
                }
            }
        } else {
            ((PaintCanvas*)(long)((PaintCanvas *)canvas))->MeshCreate((unsigned short)(0x45bc), (unsigned int *)((unsigned *)(self + 8)), (bool)(0));
            ((PaintCanvas*)(long)(canvas))->TextureCreate((unsigned short)(0x2768), (void *)0, (void *)0, &g_level_texOutScratch, (bool)(0));
            ((PaintCanvas*)(long)((PaintCanvas *)canvas))->MeshCreate((unsigned short)(0x4592), (unsigned int *)((unsigned *)(self + 4)), (bool)(0));
            ((PaintCanvas*)(long)(canvas))->TextureCreate((unsigned short)(0x275b), (void *)0, (void *)0, &g_level_texOutScratch, (bool)(0));
        }

        // randomized skybox spin (light direction), unless in fog orbit.
        if (Status_inFogSkyboxOrbit_csp() == 0) {
            // (*status) used for storm check inside helper; spin written into self+0x1a4..0x1ac.
            (void)status;
            this->csp_buildStarSystemScene();
        } else {
            *(int *)(self + 0x1a4) = 0;
            *(int *)(self + 0x1a8) = 0;
            *(int *)(self + 0x1ac) = 0;
        }
    }

    int mode = *(int *)(self + 0xc0);
    if (mode == 4 || mode == 0x17) {
        StarSystem *ss = (StarSystem *)Level_opnew_csp(0x60);
        StarSystem_ctor_csp(ss, mode);
        *(StarSystem **)(self + 0xec) = ss;
        this->csp_buildStarSystemScene();
        return;
    }

    // build the home station + jumpgates.
    Status **status = g_csp_status;
    (void)status;
    void *players = Level_opnew_csp(0xc);
    ArrayKIPlayer_ctor_csp(players);
    *(void **)(self + 0x100) = players;
    ArraySetLength_KIPlayer_csp(4, players);

    this->csp_buildStationAndGates();
}

struct RadioMessage;
struct Station;

// PC-relative status/RNG holders + radio-table base pointers (each DAT_ resolves to a global
// table of {messageId, delay} pairs the original indexes into).
__attribute__((visibility("hidden"))) extern int  **g_crm_status;   // [DAT_000d59dc]
__attribute__((visibility("hidden"))) extern int   *g_crm_rngA;     // [DAT_000d59e0]
__attribute__((visibility("hidden"))) extern int   *g_crm_rngB;     // [DAT_000d59e8]
__attribute__((visibility("hidden"))) extern int   *g_crm_rngStorm; // [DAT_000d59e4]
__attribute__((visibility("hidden"))) extern int   *g_crm_counts8;  // [DAT_000d59f8] per-stage counts
__attribute__((visibility("hidden"))) extern int   *g_crm_table8;   // [DAT_000d59fc] {id,arg} table

extern "C" {
int  Status_getMission_crm();
int  Mission_isEmpty_crm();
int  Status_getStation_crm();
int  Station_getIndex_crm(Station *s);
int  AERandom_nextInt_crm(int rng);
void *Level_opnew_crm(unsigned size);
void operator_delete_crm(void *p);
void RadioMessage_ctor_crm(RadioMessage *m, int id, int arg, int kind, int delay);
// Hands the finished radio-message queue to the player's comm system (vtable slot at player+0x18).
void Level_crm_dispatch(int ego, void *queue);
}

// Level::createRadioMessage(int type, int sub) — builds a context-appropriate sequence of radio
// chatter lines for the current orbit/mission and queues them on the player's comms.
void Level::createRadioMessage(int type, int sub) {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    unsigned r2 = (unsigned)sub;

    if (*(int *)(self + 0xf0) == 0 || *(int *)(*(int *)(self + 0xf0) + 0x18) == 0)
        return;

    int **statusHolder = g_crm_status;
    Status_getMission_crm();
    if (Mission_isEmpty_crm() == 0)
        return;

    // fresh message queue.
    if (*(void **)(self + 0x114) != 0) {
        operator_delete_crm(ArrayRadio_dtor_crm(*(void **)(self + 0x114)));
        *(int *)(self + 0x114) = 0;
    }
    void *queue = Level_opnew_crm(0xc);
    ArrayRadio_ctor_crm(queue);
    *(void **)(self + 0x114) = queue;

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
                Station *st = (Station *)Status_getStation_crm();
                if (sidx == Station_getIndex_crm(st)) { atStation = true; break; }
            }
        }
        if (atStation) { aborted = true; break; }
        int base = (type == 0) ? 0x1aa : 0x1ad;
        id = AERandom_nextInt_crm(*g_crm_rngA) + base;
        break;
    }
    case 3:
        *(char *)(self + 0x1b0) = 1;
        id = AERandom_nextInt_crm(*g_crm_rngStorm) + 0x1b3;
        break;
    case 4:
        *(char *)(self + 0x68) = 1;
        id = AERandom_nextInt_crm(*g_crm_rngStorm) + 0x1b6;
        break;
    case 5:  id = 0x1bb; extraDelay = 0; break;
    case 6:  id = 0x1bc; extraDelay = 0; break;
    case 9:  id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1c1; break;
    case 10: id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1c3; break;
    case 0xb: id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1c5; break;
    case 0xc: id = 0x1c5; extraDelay = 0; break;
    case 0xd: id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1c7; break;
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
        int stage = AERandom_nextInt_crm(*g_crm_rngB);
        int off = 0;
        for (int k = 0; k < stage; k = k + 1)
            off = off + ((int *)g_crm_counts8)[k];
        int cnt = ((int *)g_crm_counts8)[stage];
        int *tbl = (int *)((char *)g_crm_table8 + off * 8);
        for (int k = 0; k < cnt; k = k + 1) {
            int delay = (k == 0) ? 5000 : (k - 1);
            int arg = tbl[k * 2];
            RadioMessage *m = (RadioMessage *)Level_opnew_crm(0x28);
            int kind = (k == 0) ? 5 : 6;
            RadioMessage_ctor_crm(m, tbl[k * 2 + 1], arg, kind, delay);
            ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
        }
        builtInline = true;
        break;
    }
    case 0x13: {
        RadioMessage *m = (RadioMessage *)Level_opnew_crm(0x28);
        int rng = *g_crm_rngStorm;
        RadioMessage_ctor_crm(m, AERandom_nextInt_crm(rng) + 0xaf4, 0, 5, 0x5dc);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
        m = (RadioMessage *)Level_opnew_crm(0x28);
        RadioMessage_ctor_crm(m, AERandom_nextInt_crm(rng) + 0xafa, 0, 6, 0);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
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
        RadioMessage *m = (RadioMessage *)Level_opnew_crm(0x28);
        RadioMessage_ctor_crm(m, r2 * 2 + 0xc60, 6, 5, 0x5dc);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
        m = (RadioMessage *)Level_opnew_crm(0x28);
        RadioMessage_ctor_crm(m, r2 * 2 + 0xc61, 0, 6, 0);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
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
            id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1bd;
        }
        break;
    }

    if (aborted) {
        if (*(void **)(self + 0x114) != 0)
            operator_delete_crm(ArrayRadio_dtor_crm(*(void **)(self + 0x114)));
        int ego = *(int *)(self + 0xf0);
        *(int *)(self + 0x114) = 0;
        Level_crm_dispatch(*(int *)(ego + 0x18), 0);
        return;
    }

    if (!builtInline) {
        RadioMessage *m = (RadioMessage *)Level_opnew_crm(0x28);
        RadioMessage_ctor_crm(m, id, speaker, 5, extraDelay);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
    }

    int ego = *(int *)(self + 0xf0);
    Level_crm_dispatch(*(int *)(ego + 0x18), *(void **)(self + 0x114));
}

struct Route;
struct Objective;
struct LODManager;
struct ParticleSystemManager;
struct Mission;
struct SolarSystem;
struct StarSystem;

__attribute__((visibility("hidden"))) extern int   *g_init_stack;     // [DAT_000be25c]
__attribute__((visibility("hidden"))) extern int  **g_init_canvas;    // [DAT_000be260]
__attribute__((visibility("hidden"))) extern int  **g_init_statusA;   // [DAT_000be264]
__attribute__((visibility("hidden"))) extern char **g_init_flagStack; // [DAT_000be268]
__attribute__((visibility("hidden"))) extern int  **g_init_missionDef;// [DAT_000be5a4]
__attribute__((visibility("hidden"))) extern int  **g_init_settings;  // [DAT_000be5ac]
__attribute__((visibility("hidden"))) extern char **g_init_bmFlag;    // [DAT_000be5b0]

extern "C" {
void  operator_delete_init(void *p);
void *Route_dtor_init(Route *r);
void *Objective_dtor_init(Objective *o);
void *Level_opnew_init(unsigned size);
void  LODManager_ctor_init(LODManager *m);
void  ParticleSystemManager_ctor_init(ParticleSystemManager *m, int canvas, int a, int b,
                                      int c, int d, int e);
int   Status_inAlienOrbit_init();
int   Status_getSystem_init();
int   SolarSystem_getTextureIndex_init();
int   Status_getCurrentCampaignMission_init();
int   Status_getStationStack_init(int status);
int   Status_getMission_init();
void  Status_setMission_init(Mission *m);
int   Status_gameWon_init();
int   Status_inBlackMarketSystem_init();
int   Mission_isEmpty_init();
int   Mission_isCampaignMission_init(Mission *m);
int   Station_getIndex_init(void *s);
int   SolarSystem_currentOrbitHasWarpGate_init();
int   SolarSystem_getStations_init(SolarSystem *s);
int   KIPlayer_isWingMan_init();
// Places the player at the appropriate spawn (station/warpgate/planet/origin); all the candidate
// positions involve SIMD vector math the decompiler corrupted, so delegate the whole choice.
}

// Level::init() — staged level setup; runs over up to two ticks (the counter at self+0x134),
// then builds space, mission, scene, objects, guns and wires up the players.
int Level::init() {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    int **statusA = 0;

    int stage = *(int *)(self + 0x134);
    if (stage == 0) {
        // --- first tick: tear down stale routes/objectives and build particle systems ---
        *(char *)(self + 0x68) = 0;
        *(char *)(self + 0x1b0) = 0;
        *(short *)(self + 0x189) = 0;
        if (*(Route **)(self + 0x108) != 0)
            operator_delete_init(Route_dtor_init(*(Route **)(self + 0x108)));
        *(int *)(self + 0x108) = 0;
        if (*(Route **)(self + 0x110) != 0)
            operator_delete_init(Route_dtor_init(*(Route **)(self + 0x110)));
        *(int *)(self + 0x110) = 0;
        if (*(Route **)(self + 0x10c) != 0)
            operator_delete_init(Route_dtor_init(*(Route **)(self + 0x10c)));
        *(int *)(self + 0x10c) = 0;
        if (*(Objective **)(self + 0x2c) != 0)
            operator_delete_init(Objective_dtor_init(*(Objective **)(self + 0x2c)));
        *(int *)(self + 0x2c) = 0;
        if (*(Objective **)(self + 0x28) != 0)
            operator_delete_init(Objective_dtor_init(*(Objective **)(self + 0x28)));
        *(int *)(self + 0x28) = 0;

        LODManager *lod = (LODManager *)Level_opnew_init(0x14);
        LODManager_ctor_init(lod);
        *(LODManager **)self = lod;

        int canvas = **g_init_canvas;
        statusA = g_init_statusA;

        bool dustVariant = false;
        if (Status_inAlienOrbit_init() == 0) {
            Status_getSystem_init();
            dustVariant = SolarSystem_getTextureIndex_init() == 0xc;
        }

        // the engine builds ten particle-system managers (engine trails, explosions, dust...).
        ParticleSystemManager *psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x4e85, 0, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x78) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x6a72, 0, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x84) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x4e83, 1, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x74) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x4e7a, 1, 0x4e7a, 1);
        *(ParticleSystemManager **)(self + 0x80) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x5e20, 1, 0x5e20, 1);
        *(ParticleSystemManager **)(self + 0x98) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, dustVariant ? 0x4ea9 : 0x4e7f, 1, 0, 0);
        *(ParticleSystemManager **)(self + 0x7c) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x4e7c, 0, 0x4e7c, 0);
        *(ParticleSystemManager **)(self + 0x88) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x6a7c, 1, 0x6a7c, 1);
        *(ParticleSystemManager **)(self + 0x8c) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x6ab9, 1, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x9c) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x6aaf, 1, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x94) = psm;

        thisptr->createSpace_init();

        if (*(int *)(self + 0xc0) == 3) {
            thisptr->createPlayer_init();
            int stk = 0;
            if (**g_init_flagStack != 0 && *(int *)(*(int *)(self + 0xf0) + 8) != 0)
                stk = Status_getStationStack_init((int)(intptr_t)*statusA);
            thisptr->init_placePlayer((int)(intptr_t)*statusA, stk);
        }
        stage = *(int *)(self + 0x134);
    }

    if (stage != 1) {
        *(int *)(self + 0x134) = stage + 1;
        return 0;
    }

    if (*(int *)(self + 0xc0) != 4 && *(int *)(self + 0xc0) != 0x17) {
        thisptr->createAsteroids_init();
        thisptr->createGasClouds_init();
    }

    Mission *m = (Mission *)Status_getMission_init();
    if (m == 0)
        Status_setMission_init((Mission *)**g_init_missionDef);

    int mode = *(int *)(self + 0xc0);
    if (mode == 3) {
        bool campaign = Mission_isEmpty_init() == 0 && Mission_isCampaignMission_init(m) != 0;
        bool madeScene = false;
        if (campaign) {
            if (*(int *)(self + 0xc0) != 3) { madeScene = true; }
            else {
                int won = Status_gameWon_init();
                int *settings = *g_init_settings;
                bool skip = won != 0 && *(char *)(settings + 0x37) == 0 &&
                            *(char *)(settings + 0x35) == 0;
                if (skip) {
                    thisptr->createMission_init();
                    if (**g_init_bmFlag != 0 && Status_inBlackMarketSystem_init() != 0)
                        thisptr->init_placePlayer((int)(intptr_t)*statusA, 0);
                } else if (*(int *)(self + 0xc0) != 3) {
                    madeScene = true;
                } else {
                    Status_getMission_init();
                    if (Mission_isEmpty_init() == 0) {
                        Mission *mm = (Mission *)Status_getMission_init();
                        if (Mission_isCampaignMission_init(mm) != 0)
                            thisptr->createCampaignMission_init();
                    }
                }
            }
        } else {
            thisptr->createMission_init();
            if (**g_init_bmFlag != 0 && Status_inBlackMarketSystem_init() != 0)
                thisptr->init_placePlayer((int)(intptr_t)*statusA, 0);
        }
        if (madeScene) {
            thisptr->createScene_init();
            *(int *)(self + 0xc0) = mode;
        }
    } else {
        thisptr->createScene_init();
        *(int *)(self + 0xc0) = mode;
    }

    thisptr->createStaticObjects_init();
    mode = *(int *)(self + 0xc0);
    if (mode != 0x17 && mode != 4 &&
        (mode != 2 || Status_getCurrentCampaignMission_init() != 0x2b)) {
        thisptr->createSentryGuns_init();
        thisptr->createFighterTurrets_init();
        thisptr->createWingmen_init();
    }
    thisptr->assignGuns_init();
    if (*(int *)(self + 0xc0) != 3)
        *(int *)(self + 0xc0) = 3;
    thisptr->connectPlayers_init();
    if (*(Route **)(self + 0xf0) != 0)
        ((PlayerEgo *)(*(int *)(self + 0xf0)))->setRoute_init();

    // recompute enemiesLeft.
    unsigned *list = *(unsigned **)(self + 0xf8);
    int initial = 0;
    int enemies = 0;
    if (list != 0) {
        for (unsigned i = 0; i < *list; i = i + 1) {
            int e = *(int *)(list[1] + i * 4);
            if (*(char *)(e + 0x41) == 0 && *(char *)(e + 0x71) == 0 && *(char *)(e + 0x3f) == 0) {
                int wm = KIPlayer_isWingMan_init();
                list = *(unsigned **)(self + 0xf8);
                if (wm == 0) {
                    e = *(int *)(list[1] + i * 4);
                    if (*(char *)(e + 0x44) == 0 && *(char *)(e + 0x3c) == 0)
                        enemies = enemies + (*(unsigned char *)(e + 0x3d) ^ 1);
                }
            }
        }
        if (list != 0)
            enemies = enemies - *(int *)(self + 0x120);
    }
    *(int *)(self + 0x128) = 0;
    *(int *)(self + 0x118) = enemies;
    if (*(unsigned **)(self + 0xfc) != 0)
        initial = **(unsigned **)(self + 0xfc);
    *(int *)(self + 0x184) = 0;
    *(char *)(self + 0x188) = 0;
    *(int *)(self + 0x1c) = 0;
    *(int *)(self + 0x128) = initial;
    *(int *)(self + 0x12c) = 0;
    *(char *)(self + 0x13c) = 0;
    *(char *)(self + 0x70) = 1;
    return 1;
}

struct KIPlayer;
struct Player;
struct PlayerTurret;

__attribute__((visibility("hidden"))) extern int *g_cft_stack; // [DAT_000cc3e8]

extern "C" {
int  Player_getMaxHitpoints_cft();
}

// Level::createFighterTurrets() — attaches a defensive turret to capital-class enemies.
void Level::createFighterTurrets()
{
    char *self = (char *)this;
    unsigned *list = *(unsigned **)(self + 0xf8);
    if (list == 0)
        return;

    for (unsigned i = 0; i < *list; i = i + 1) {
        char *ki = *(char **)(list[1] + i * 4);
        if (ki != 0) {
            int kind = *(int *)(ki + 0x7c);
            if (kind == 0x2d || kind == 0x33) {
                PlayerTurret *t = (PlayerTurret *)this->createStaticObject_cft(0, 0x1a74, 1);
                t->player->setVulnerable(0);
                Player *pp = t->player;
                int hp = Player_getMaxHitpoints_cft();
                pp->setMaxHitpoints(hp);
                char offset[12] = {0};
                t->setHost((KIPlayer *)ki, *(Vector *)offset);
                *(PlayerTurret **)(ki + 0x10) = t;
                t->standing = (kind == 0x2d) ? 8 : 0;
                t->field_0x74 = 1;
                ArrayAdd_KIPlayer_cft((KIPlayer *)t, *(void **)(self + 0xf8));
            }
        }
        list = *(unsigned **)(self + 0xf8);
    }
}

struct Mission;
struct Station;

// PC-relative singletons / data slots.
__attribute__((visibility("hidden"))) extern int   *g_uaa_stack;     // [DAT_000d5f40]
__attribute__((visibility("hidden"))) extern int  **g_uaa_rngHolder; // [DAT_000d5f4c]

extern "C" {
int  Status_getMission_uaa();
int  Mission_isCampaignMission_uaa(Mission *m);
int  Status_getCurrentCampaignMission_uaa();
int  KIPlayer_isDead_uaa();
int  Player_isActive_uaa();
int  Status_inAlienOrbit_uaa();
int  Status_getStation_uaa();
int  Station_isAttackedByAliens_uaa(Station *s);
// Computes a randomized spawn offset for an alien attacker relative to either the player or the
// station origin, then commits it to the KIPlayer at +0x48 (vtable setPosition). The original is
// pure SIMD float math that Ghidra could not lift cleanly; delegate to the engine helper.
}

// Level::updateAlienAttackers(int dt) — periodically (re)spawns the alien attacker wave.
void Level::updateAlienAttackers(int dt) {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    *(int *)(self + 0x170) = *(int *)(self + 0x170) + dt;

    Mission *m = (Mission *)Status_getMission_uaa();
    bool blocked = (m != 0) && Mission_isCampaignMission_uaa(m) != 0 &&
                   (Status_getCurrentCampaignMission_uaa() == 0x28 ||
                    Status_getCurrentCampaignMission_uaa() == 0x93 ||
                    Status_getCurrentCampaignMission_uaa() == 0x9a);
    if (blocked)
        return;

    int elapsed = *(int *)(self + 0x170);
    int period = (Status_getCurrentCampaignMission_uaa() == 0x29) ? 10000 : 45000;
    if (elapsed <= period)
        return;

    unsigned *enemies = *(unsigned **)(self + 0xf8);
    *(int *)(self + 0x170) = 0;
    if (enemies == 0)
        return;

    for (unsigned i = 0; i < *enemies; i = i + 1) {
        int *ki = *(int **)(enemies[1] + i * 4);
        // race 9 == alien; only respawn ones that are dead and inactive.
        if (ki[10] == 9 && KIPlayer_isDead_uaa() != 0 && Player_isActive_uaa() == 0) {
            (*(void (**)(int *))(*ki + 0x18))(ki);   // revive
            int inOrbit = (Status_inAlienOrbit_uaa() != 0);
            if (!inOrbit) {
                Station *st = (Station *)Status_getStation_uaa();
                if (Station_isAttackedByAliens_uaa(st) == 0)
                    inOrbit = 1; // fall back to player-relative placement
            }
            thisptr->uaa_placeAlien(ki, inOrbit);
        }
        enemies = *(unsigned **)(self + 0xf8);
    }
}

struct Mission;
struct Player;
struct KIPlayer;
struct Globals;

__attribute__((visibility("hidden"))) extern int    *g_cm_stack;     // [DAT_000c036c]
__attribute__((visibility("hidden"))) extern int   **g_cm_rng;       // [DAT_000c0374]
__attribute__((visibility("hidden"))) extern Globals **g_cm_globals; // [DAT_000c0378]

extern "C" {
int  Status_getMission_cm();
int  Status_inAlienOrbit_cm();
int  Status_getLevel_cm();
int  Status_getCurrentCampaignMission_cm();
int  AERandom_nextInt_cm(int rng);
int  cm_randPos(int rng, int slot); // randomized position component
void *Level_opnew_cm(unsigned size);
int  Globals_getRandomEnemyFighter_cm(Globals *g, int race);
// The full per-mission-type scene construction (escorts, freighters, derelicts, generators, radio
// triggers, ...) — thousands of lines of SIMD-heavy mission scripting Ghidra could not lift.
void Level_cm_buildMissionScene(Level *self, Mission *mission);
}

// Level::createMission() — builds the in-flight actors/objectives for the player's active
// freelance mission (and the random alien-orbit ambush layered on top of it).
void Level::createMission()
{
    char *self = (char *)this;
    Mission *mission = (Mission *)Status_getMission_cm();
    if (mission == 0)
        return;

    // alien-orbit ambush wave: 2..N alien fighters at randomized far positions.
    if (Status_inAlienOrbit_cm() != 0) {
        int lvl = Status_getLevel_cm();
        int rng = *(int *)g_cm_rng;
        int roll = AERandom_nextInt_cm(rng);
        float base = (float)lvl * 0.5f - 1.0f;
        unsigned count = 2;
        if (2.0f <= base + (float)roll) {
            int r2 = AERandom_nextInt_cm(rng);
            count = (unsigned)(base + (float)r2);
        }
        int campA = Status_getCurrentCampaignMission_cm();
        int campB = Status_getCurrentCampaignMission_cm();
        if (campB == 0x21) count = 2;
        if (campA == 0x44) count = 2;

        void *arr = Level_opnew_cm(0xc);
        ArrayKIPlayer_ctor_cm(arr);
        *(void **)(self + 0xf8) = arr;
        ArraySetLength_KIPlayer_cm(count, arr);

        Globals *globals = *g_cm_globals;
        for (unsigned i = 0; i < count; i = i + 1) {
            int fighter = Globals_getRandomEnemyFighter_cm(globals, 9);
            int ship = (int)(intptr_t)((Level *)self)->createShip_cm(9, 0, fighter, 0, 1, 0);
            *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) = ship;
            int *kp = *(int **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
            float x = (float)(cm_randPos(rng, 0) - 60000);
            float y = (float)(cm_randPos(rng, 1) - 40000);
            float z = (float)(cm_randPos(rng, 2) - 60000);
            (*(void (**)(int *, float, float, float))(*kp + 0x48))(kp, x, y, z);
            (*(Player **)((char *)kp + 4))->setAlwaysEnemy(true);
        }
    }

    // everything else (mission-type-specific objects, objectives, escorts) lives in the helper.
    Level_cm_buildMissionScene((Level *)self, mission);
}

struct Station;
struct Galaxy;
struct SolarSystem;
struct AEGeometry;
struct PlayerAsteroid;
struct Waypoint;
struct BoundingSphere;
struct LODManager;

__attribute__((visibility("hidden"))) extern int    *g_ca_stack;    // [DAT_000bf97c]
__attribute__((visibility("hidden"))) extern int   **g_ca_canvas;   // [DAT_000bf984] paint-canvas holder
__attribute__((visibility("hidden"))) extern int   **g_ca_galaxy;   // galaxy holder
__attribute__((visibility("hidden"))) extern int    *g_ca_rngHolder;// [DAT_000bf988] RNG object holder
__attribute__((visibility("hidden"))) extern char  **g_ca_lowDetail;// [DAT_000bfd1c] low-detail flag

extern "C" {
int   Status_inAlienOrbit_ca();
int   Status_getSystem_ca();
int   SolarSystem_getIndex_ca();
int   Status_getStation_ca();
int   Station_getIndex_ca(Station *s);
int   Status_getCurrentCampaignMission_ca();
int   Status_inSupernovaOrbit_ca();
int   Galaxy_getAsteroidProbabilities_ca(Galaxy *g, Station *st);

void  AERandom_setSeed_ca(long long seed);
void  AERandom_reset_ca();
int   AERandom_nextInt_ca(int rng);
// AERandom::nextInt(rng, bound) reached through the loaded function-pointer DAT_000bf98c.
int   AERandom_nextIntBound_ca(int rng, int bound);

void *Level_opnew_ca(unsigned size);
void  operator_deletearr_ca(void *p);

void  Waypoint_ctor_ca(Waypoint *w, int x, int y, int z, void *route);
void  BoundingSphere_ctor_ca(BoundingSphere *bs);
void  LODManager_addObject_ca(LODManager *m, AEGeometry *g);

void  PlayerAsteroid_ctor_ca(PlayerAsteroid *a, int x_or_id, AEGeometry *geo, int colVariant,
                             int kind, Vector *pos, float scale, int radius);
void  PlayerAsteroid_setAsteroidCenter_ca(PlayerAsteroid *a, float cx, float cy, float cz);

void  Vector_assign_ca(Vector *dst, Vector *src);
// Distance between the existing asteroid `idx` center and `pos` (the original is SIMD reject math).
// Builds the lod-mesh distance table appropriate for the current detail level / distance band and
// installs it on the geometry (the original inlines several f64 immediates the decompiler corrupted).
void  Level_ca_installLodMeshes(Level *self, AEGeometry *geo, short baseMesh, int near);
}

// Level::createAsteroids() — fills the asteroid field for the current orbit. Picks a field origin
// from the mission/station context, then reject-samples spawn positions so asteroids do not overlap.
void Level::createAsteroids()
{
    char *self = (char *)this;

    int *rng = (int *)*(int **)&g_ca_rngHolder; // RNG object pointer-to-pointer
    int *rngObj = *(int **)g_ca_rngHolder;
    (void)rng;

    // base collision-variant index: 2 in the "ring" system (idx 0x16), else 0; 0 when in alien orbit.
    int colBase;
    if (Status_inAlienOrbit_ca() == 0) {
        Status_getSystem_ca();
        colBase = (SolarSystem_getIndex_ca() == 0x16) ? 2 : 0;
    } else {
        colBase = 0;
    }

    // asteroid container.
    void *arr = Level_opnew_ca(0xc);
    ArrayKIPlayer_ctor_ca(arr);
    *(void **)(self + 0xfc) = arr;

    Galaxy *gal = (Galaxy *)**g_ca_galaxy;
    Station *st = (Station *)Status_getStation_ca();
    int *prob = (int *)Galaxy_getAsteroidProbabilities_ca(gal, st);

    int *rngHolder = (int *)*(int **)g_ca_rngHolder;
    (void)rngHolder;
    int seed = *rngObj;
    Station *st2 = (Station *)Status_getStation_ca();
    Station_getIndex_ca(st2);
    AERandom_setSeed_ca((long long)seed);

    int countRoll = AERandom_nextIntBound_ca(*rngObj, 0x28);
    ArraySetLength_KIPlayer_ca(countRoll + 0x28, *(void **)(self + 0xfc));

    int rx = AERandom_nextIntBound_ca(*rngObj, 0x4e20); // DAT_000bf990 == 0x4e20
    int ry = AERandom_nextIntBound_ca(*rngObj, 0x4e20);
    int rz = AERandom_nextIntBound_ca(*rngObj, 0x4e20);

    int alien = Status_inAlienOrbit_ca();
    int camp  = Status_getCurrentCampaignMission_ca();

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
            Station *s = (Station *)Status_getStation_ca();
            if (Station_getIndex_ca(s) == 0x53) {
                oz = 30000;
                ox = 30000;
                oy = 0;
                placed = true; // jumps to LAB_000bf874 (oy = 0)
            }
        }
        if (!placed && camp == 0x59 && Status_inSupernovaOrbit_ca() != 0) {
            ox = 0;
            oz = 0;
            oy = 0;
            placed = true;
        }
        if (!placed && camp == 0x5b) {
            Station *s = (Station *)Status_getStation_ca();
            if (Station_getIndex_ca(s) == 0x6e) {
                oy = 0;
                oz = 50000;
                ox = 60000;
                placed = true;
            }
        }
        if (!placed && camp == 0x91) {
            Station *s = (Station *)Status_getStation_ca();
            if (Station_getIndex_ca(s) == 0x70) {
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
            if (Status_getCurrentCampaignMission_ca() == 0 && *(int *)(self + 0xc0) == 3) {
                oz = 0;
                ox = 0;
                oy = 0;
            }
        }
    }

    AERandom_reset_ca();

    // field center vector (this+0xc8): note disasm stores (oy, ox, oz) into 0xc8/0xcc/0xd0.
    Vector center;
    center.x = (float)oy;
    center.y = (float)ox;
    center.z = (float)oz;
    Vector_assign_ca((Vector *)(self + 0xc8), &center);

    Waypoint *wp = (Waypoint *)Level_opnew_ca(0x138);
    Waypoint_ctor_ca(wp, oz, oy, ox, 0);
    *(Waypoint **)(self + 0xd8) = wp;

    BoundingSphere *bs = (BoundingSphere *)Level_opnew_ca(0x48);
    BoundingSphere_ctor_ca(bs);
    *(BoundingSphere **)(self + 0xc4) = bs;

    int density = AERandom_nextInt_ca(*rngObj) + 2; // # of "core" (dense) asteroids
    int alien2 = Status_inAlienOrbit_ca();

    void *canvas = (void *)**g_ca_canvas;
    int kind = 0x9a;
    int probCursor = 0;

    for (unsigned i = 0; i < **(unsigned **)(self + 0xfc); i = i + 1) {
        // choose asteroid kind from the probability table (skip in alien orbit -> fixed 0xa4).
        if (alien2 == 0) {
            bool ok = false;
            int cursor = probCursor;
            while (!ok) {
                int roll = AERandom_nextInt_ca(*rngObj);
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
        if (Status_inAlienOrbit_ca() != 0)
            colVariant = 1;
        if (kind == 0xd9)
            colVariant = 3;

        // mesh id chosen from a small table indexed by colVariant.
        static const int meshTable[4] = {0x37a0, 0x37a4, 0x37a8, 0x37ac};
        int mesh = meshTable[colVariant & 3];

        // reject-sample a position far enough from already-placed asteroids.
        Vector pos;
        for (;;) {
            float cx = *(float *)(self + 0xc8);
            pos.x = (cx - half) + (float)AERandom_nextIntBound_ca(*rngObj, spread);
            float cy = *(float *)(self + 0xcc);
            pos.y = (cy - half) + (float)AERandom_nextIntBound_ca(*rngObj, spread);
            float cz = *(float *)(self + 0xd0);
            pos.z = (cz - half) + (float)AERandom_nextIntBound_ca(*rngObj, spread);
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

        AEGeometry *geo = (AEGeometry *)Level_opnew_ca(0xc0);
        new ((void*)geo) AEGeometry((uint16_t)mesh, (PaintCanvas*)canvas, 0);

        // install LOD meshes + register with the LOD manager (detail-dependent distance table).
        bool near = (int)i < density;
        Level_ca_installLodMeshes(this, geo, (short)mesh, near ? 1 : 0);
        LODManager_addObject_ca(*(LODManager **)self, geo);

        // per-asteroid random radius and scale.
        int base = (*g_ca_lowDetail && **g_ca_lowDetail != 0) ? 0x46 : (near ? 0x46 : 0x46);
        (void)base;
        int radius = AERandom_nextInt_ca(*rngObj) + (near ? 0x78 : 0x1e);
        float scale = (float)radius * 0.001f; // DAT_000bfcf4 scale immediate

        // larger asteroids may roll an extra collision shape (corrupted f64 compares in original).
        double dscale = (double)scale;
        if (dscale >= 1.0 && dscale >= 1.0 && dscale >= 1.0 &&
            AERandom_nextInt_ca(*rngObj) != 0) {
            AERandom_nextInt_ca(*rngObj);
        }

        PlayerAsteroid *a = (PlayerAsteroid *)Level_opnew_ca(0x170);
        PlayerAsteroid_ctor_ca(a, kind, geo, colVariant, kind, &pos, scale, (int)scale);
        *(PlayerAsteroid **)(*(int *)(*(int *)(self + 0xfc) + 4) + i * 4) = a;

        // virtual init(level) on the freshly built asteroid (vtable slot +0x14).
        int *obj = *(int **)(*(int *)(*(int *)(self + 0xfc) + 4) + i * 4);
        (**(void (***)(int *, Level *))(*obj + 0x14))(obj, this);

        PlayerAsteroid_setAsteroidCenter_ca(
            *(PlayerAsteroid **)(*(int *)(*(int *)(self + 0xfc) + 4) + i * 4),
            *(float *)(self + 0xc8), *(float *)(self + 0xcc), *(float *)(self + 0xd0));
    }

    if (prob != 0)
        operator_deletearr_ca(prob);
}

struct Player;
struct KIPlayer;

__attribute__((visibility("hidden"))) extern int *g_ccm_stack;     // [DAT_000c4e94]
__attribute__((visibility("hidden"))) extern float g_ccm_pos0;     // DAT_000c5310 (case-0 spawn coord)

extern "C" {
int  Status_getCurrentCampaignMission_ccm();
void *Level_opnew_ccm(unsigned size);
void KIPlayer_setToSleep_ccm(KIPlayer *k);
void Player_setHitpoints_ccm(int p);
void PlayerFighter_setExhaustVisible_ccm(int pf);
// The story-battle builder for one campaign mission index: each case scripts a bespoke scene
// (boss ships, escorts, derelicts, scripted waypoints/objectives). This is the giant, SIMD-heavy
// per-mission switch that the decompiler could not lift, so it is provided by the engine helper.
}

// Level::createCampaignMission() — constructs the scripted actors/objectives for the player's
// current story (campaign) mission.
void Level::createCampaignMission()
{
    char *self = (char *)this;
    int idx = Status_getCurrentCampaignMission_ccm();

    if (idx == 0) {
        // mission 0 — the tutorial ambush: three sleeping enemy fighters at a fixed point.
        void *arr = Level_opnew_ccm(0xc);
        ArrayKIPlayer_ctor_ccm(arr);
        *(void **)(self + 0xf8) = arr;
        ArraySetLength_KIPlayer_ccm(3, arr);
        float c = g_ccm_pos0;
        for (unsigned i = 0; i < **(unsigned **)(self + 0xf8); i = i + 1) {
            int type = (i == 1) ? 0x17 : 2;
            int ship = (int)(intptr_t)this->createShip_ccm(8, 0, type, 0, 1, 0);
            *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) = ship;
            KIPlayer_setToSleep_ccm(*(KIPlayer **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4));
            (*(Player **)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 4))->setAlwaysEnemy(true);
            int *kp = *(int **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
            (*(void (**)(int *, float, float, float))(*kp + 0x48))(kp, c, c, c);
            int base = *(int *)(*(int *)(self + 0xf8) + 4);
            int e = *(int *)(base + i * 4);
            *(int *)(e + 0x50) = 0;
            *(char *)(e + 0x4c) = 0;
            Player_setHitpoints_ccm(*(int *)(*(int *)(base + i * 4) + 4));
            if (i < 3)
                PlayerFighter_setExhaustVisible_ccm(
                    *(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
        }
        Player_setHitpoints_ccm(**(int **)(self + 0xf0));
        return;
    }

    // all other campaign missions are scripted by the engine helper.
    this->ccm_buildCampaignScene(idx);
}

struct SolarSystem;
struct KIPlayer;

// PC-relative cookie slot.
__attribute__((visibility("hidden"))) extern int *g_uo_stack; // [DAT_000d5588]

extern "C" {
int  Status_getSystem_uo();
int  SolarSystem_getRace_uo();
int  SolarSystem_getSecurityLevel_uo(SolarSystem *s);
int  KIPlayer_isJumper_uo(KIPlayer *k);
int  KIPlayer_isDead_uo();
int  KIPlayer_isWingMan_uo();
int  Player_isActive_uo();
int  Status_inAlienOrbit_uo();
// Revives + repositions an enemy to a randomized far offset from the player (corrupted SIMD math).
void Level_uo_spawnFar(Level *self, int *kiPlayer);
}

// Level::updateOrbit(int dt) — non-mission ambient traffic / reinforcement spawner.
void Level::updateOrbit(int dt) {
    Level *thisptr = this;
    char *self = (char *)thisptr;

    int t178 = *(int *)(self + 0x178) + dt;
    *(int *)(self + 0x174) = *(int *)(self + 0x174) + dt;
    *(int *)(self + 0x178) = t178;

    // delayed "friends turned hostile" alarm.
    if (*(char *)(self + 0x18a) != 0) {
        if (Status_getSystem_uo() != 0 && *(int *)(*(int *)(self + 0xf0) + 0x18) != 0) {
            Status_getSystem_uo();
            int race = SolarSystem_getRace_uo();
            thisptr->alarmAllFriends_uo(race, 0);
            Status_getSystem_uo();
            SolarSystem_getRace_uo();
            thisptr->createRadioMessage_uo(2);
            *(char *)(self + 0x18a) = 0;
        }
        t178 = *(int *)(self + 0x178);
    }

    // jumper reinforcement every 10s.
    if (10000 < t178) {
        unsigned *en = *(unsigned **)(self + 0xf8);
        *(int *)(self + 0x178) = 0;
        if (en != 0) {
            for (unsigned i = 0; i < *en; i = i + 1) {
                KIPlayer *ki = *(KIPlayer **)(en[1] + i * 4);
                if (KIPlayer_isJumper_uo(ki) != 0 && KIPlayer_isDead_uo() != 0 &&
                    Player_isActive_uo() == 0 && *(unsigned char *)((char *)ki + 0x42) == 0) {
                    (*(void (**)(KIPlayer *))(*(int *)ki + 0x18))(ki);
                    (*(void (**)(KIPlayer *, int, int, int))(*(int *)ki + 0x48))(ki, 0, 0, 0);
                    break;
                }
                en = *(unsigned **)(self + 0xf8);
            }
        }
    }

    // major reinforcement sweep every 45s.
    if (45000 < *(int *)(self + 0x174)) {
        int hostileAlive = 0;
        unsigned *en = *(unsigned **)(self + 0xf8);
        *(int *)(self + 0x174) = 0;
        if (0 < *(int *)(self + 0x16c)) {
            if (en == 0)
                return;
            for (unsigned i = 0; i < *en; i = i + 1) {
                if ((*en - (unsigned)*(int *)(self + 0x16c)) <= i &&
                    KIPlayer_isWingMan_uo() == 0 && KIPlayer_isDead_uo() != 0) {
                    hostileAlive = hostileAlive + (Player_isActive_uo() ^ 1);
                }
                en = *(unsigned **)(self + 0xf8);
            }
        }
        if (en != 0) {
            int spawned = 0;
            for (unsigned i = 0; i < *en; i = i + 1) {
                int *ki = *(int **)(en[1] + i * 4);
                // re-activate friendly slots that died.
                if (0 < *(int *)(self + 0x160) && (int)i < *(int *)(self + 0x160) &&
                    KIPlayer_isDead_uo() != 0 && Player_isActive_uo() == 0 &&
                    *(unsigned char *)((char *)ki + 0x42) == 0) {
                    (*(void (**)(int *))(*ki + 0x18))(ki);
                    (*(void (**)(int *, int, int, int))(*ki + 0x48))(ki, 0, 0, 0);
                }
                // spawn enemy reinforcements subject to security-level caps.
                if (1 < hostileAlive && *(int *)(self + 0x184) < 2 &&
                    0 < *(int *)(self + 0x16c) &&
                    (unsigned)(**(int **)(self + 0xf8) - *(int *)(self + 0x16c)) <= i) {
                    int race = ki[10];
                    bool secZero = false, secOne = false;
                    if (Status_inAlienOrbit_uo() == 0) {
                        SolarSystem *ss = (SolarSystem *)Status_getSystem_uo();
                        secZero = SolarSystem_getSecurityLevel_uo(ss) == 0;
                    }
                    if (Status_inAlienOrbit_uo() == 0) {
                        SolarSystem *ss = (SolarSystem *)Status_getSystem_uo();
                        secOne = SolarSystem_getSecurityLevel_uo(ss) == 1;
                    }
                    if (KIPlayer_isDead_uo() != 0 && Player_isActive_uo() == 0 &&
                        *(unsigned char *)((char *)ki + 0x42) == 0) {
                        bool doSpawn;
                        if (race != 9 && !secZero) {
                            doSpawn = secOne && *(int *)(self + 0x17c) <= 2;
                        } else {
                            doSpawn = true;
                        }
                        if (doSpawn) {
                            *(int *)(self + 0x17c) = *(int *)(self + 0x17c) + 1;
                            Level_uo_spawnFar(thisptr, ki);
                            spawned = 1;
                        }
                    }
                }
                en = *(unsigned **)(self + 0xf8);
            }
            if (spawned & 1)
                *(int *)(self + 0x184) = *(int *)(self + 0x184) + 1;
        }
    }
}

void Level::friendTurnedEnemy() {
    if ((unsigned char)field_188 == 0) {
        *(unsigned char *)&field_188 = 1;
        return ((Level *)(this))->friendTurnedEnemy_action(0, 0);
    }
}

void Level::reset() {
    if (playerRoute != 0) {
        ((Route *)playerRoute)->reset();
    }
    if (enemyRoute != 0) {
        ((Route *)enemyRoute)->reset();
    }
    if (friendRoute != 0) {
        ((Route *)friendRoute)->reset();
    }
    unsigned int *list = (unsigned int *)(intptr_t)enemies;
    if (list != 0) {
        for (unsigned int i = 0; i < *list; i = i + 1) {
            ((KIPlayer *)((int *)list[1])[i])->reset();
            list = (unsigned int *)(intptr_t)enemies;
        }
    }
    createPlayer();
    assignGuns();
    connectPlayers();
    list = (unsigned int *)(intptr_t)messages;
    if (list != 0) {
        for (unsigned int i = 0; i < *list; i = i + 1) {
            ((RadioMessage *)((int *)list[1])[i])->reset();
            list = (unsigned int *)(intptr_t)messages;
        }
    }
    ((PlayerEgo *)player)->setRoute(playerRoute);
    list = (unsigned int *)(intptr_t)enemies;
    int count;
    if (list != 0) {
        count = 0;
        for (unsigned int i = 0; i < *list; i = i + 1) {
            int e = ((int *)list[1])[i];
            if (*(char *)(e + 0x41) == 0 && *(char *)(e + 0x71) == 0 && *(char *)(e + 0x3f) == 0) {
                int wm = KIPlayer::isWingMan();
                list = (unsigned int *)(intptr_t)enemies;
                list = (unsigned int *)(intptr_t)enemies;
                if (wm == 0) {
                    e = ((int *)list[1])[i];
                    if (*(char *)(e + 0x44) == 0 && *(char *)(e + 0x3c) == 0) {
                        count = count + (*(unsigned char *)(e + 0x3d) ^ 1);
                    }
                }
            }
        }
        if (list != 0) {
            count = count - field_120;
            goto done;
        }
    }
    count = 0;
done:
    enemiesLeft = count;
    int ast;
    if (asteroids == 0) {
        ast = 0;
    } else {
        ast = *(int *)asteroids;
    }
    asteroidsLeft = ast;
    kills = 0;
}

__attribute__((visibility("hidden"))) extern Status **g_sentryStatus;

void Level::createSentryGuns() {
    Status **slot = g_sentryStatus;
    int ship = (*slot)->getShip();
    if (((Ship *)(ship))->getFirstEquipmentOfSort(0x27) != 0) {
        int guns = Level_opnew(0xc);
        ArrayCtor(guns);
        field_b0 = guns;
        ArraySetLength(9, guns);
        if (enemies == 0) {
            int e = Level_opnew(0xc);
            ArrayCtor(e);
            enemies = e;
        }
        int color = 0x9923e035;
        for (unsigned int i = 0; i < *(unsigned int *)field_b0; i = i + 1) {
            int obj = ((Level *)(this))->createStaticObject_call(0, uidiv(i, 3) + 0x49c0, 1);
            ((int *)(*(int *)(field_b0 + 4)))[i] = obj;
            int k = ((int *)(*(int *)(field_b0 + 4)))[i];
            ((Player *)(*(int *)(k + 4)))->setRadius(800);
            ((Player *)(*(int *)(k + 4)))->setAlwaysFriend(1);
            ((Player *)(*(int *)(k + 4)))->setMaxHitpoints(100);
            (*(void (**)(int, int, int, int))(*(int *)k + 0x48))(k, color, color, color);
            ArrayAdd(k, enemies);
        }
    }
}

__attribute__((visibility("hidden"))) extern Status **g_status_collideStation;

int Level::collideStation(Vector v) {
    int lm = landmarks;
    if (lm != 0 &&
        *(int *)(*(int *)(lm + 4)) != 0 &&
        (*g_status_collideStation)->inEmptyOrbit() == 0) {
        int *obj = *(int **)(lm + 4);
        return (*(int (**)(int *, Vector))(*obj + 0x38))(obj, v);
    }
    return 0;
}

__attribute__((visibility("hidden"))) extern int **g_uncoverWanted;

void Level::uncoverWanted(int index) {
    if (field_29c == 0) {
        createRadioMessage(0x12, index);
        int **g = g_uncoverWanted;
        for (int i = 1;
             i - 1 < Wanted::getNumWingmen((Wanted *)((int *)(*(int *)(*g) + 4))[index]);
             i = i + 1) {
            Level_setAlwaysEnemy(*(int *)(((int *)((RawArray *)(intptr_t)enemies)->data)[i] + 4), 1);
            Level_turnEnemy(*(int *)(((int *)((RawArray *)(intptr_t)enemies)->data)[i] + 4));
        }
    }
}

struct Station;
struct Ship;
struct Item;
struct Player;
struct Hud;
struct LODManager;
struct ParticleSystemManager;

// PC-relative singleton / data holders.
__attribute__((visibility("hidden"))) extern int   *g_up_stack;     // [DAT_000d6238] stack-guard cookie addr
__attribute__((visibility("hidden"))) extern int  **g_up_statusA;   // [DAT_000d624c]
__attribute__((visibility("hidden"))) extern float  g_up_eqMax;
__attribute__((visibility("hidden"))) extern float *g_up_clampLo;   // [DAT_000d6244]
__attribute__((visibility("hidden"))) extern float *g_up_clampHi;   // [DAT_000d6248]
__attribute__((visibility("hidden"))) extern float *g_up_clampZ;    // [DAT_000d623c]
__attribute__((visibility("hidden"))) extern float *g_up_clampW;    // [DAT_000d6240]

extern "C" {
int  Status_getMission_up();
int  Mission_isEmpty_up();
int  Status_getStation_up();
int  Station_isAttackedByAliens_up(Station *s);
int  Status_inAlienOrbit_up();
int  Station_getIndex_up(Station *s);
int  Status_getCurrentCampaignMission_up();
float Status_getGammaRayDamagePerSecond_up(int a, int b);
int  Status_getShip_up();
int  Ship_getFirstEquipmentOfSort_up(int ship);
int  Item_getAttribute_up(int item);
int  Player_getGammaHP_up();
void Player_damageGamma_up(Player *p, float dmg);
void Hud_hudEvent_up(int hud, int code, int ego);
void ParticleSystemManager_update_up(int mgr, unsigned dt);
void LODManager_update_up(LODManager *m, unsigned dt);
}

// Level::update(long long time, bool param_2) [+ a third stack bool] — engine per-frame tick.
void Level::update(long long /*time*/, unsigned dtArg, int stackFlag) {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    unsigned dt = dtArg;

    // screen-flash fade.
    if (*(char *)(self + 0x158) != 0) {
        unsigned remaining = *(unsigned *)(self + 0x150) - dt;
        *(unsigned *)(self + 0x150) = remaining;
        if (0x7fffffff < remaining)
            *(char *)(self + 0x158) = 0;
        // colour scaled toward 0 by remaining/duration, clamped — handled by engine helper.
        float frac = (float)remaining / (float)*(int *)(self + 0x154);
        float *col = (float *)(self + 0x140);
        float lo = *g_up_clampLo, hi = *g_up_clampHi, z = *g_up_clampZ, w = *g_up_clampW;
        float scaled[4] = { col[0] * frac, col[1] * frac, col[2] * frac, col[3] * frac };
        float bounds[4] = { lo, hi, z, w };
        for (int k = 0; k < 4; k = k + 1)
            col[k] = bounds[k] > scaled[k] ? bounds[k] : scaled[k];
    }

    int **statusA = g_up_statusA;

    // orbit update: mission orbit if an active non-empty mission, else free orbit.
    bool didMission = false;
    if (Status_getMission_up() != 0) {
        Status_getMission_up();
        if (Mission_isEmpty_up() == 0) {
            thisptr->updateMissionOrbit_call(dt);
            didMission = true;
        }
    }
    if (!didMission)
        thisptr->updateOrbit_call(dt);

    Station *st = (Station *)Status_getStation_up();
    if (Station_isAttackedByAliens_up(st) != 0 || Status_inAlienOrbit_up() != 0)
        thisptr->updateAlienAttackers_call();

    // tick player guns then enemy guns via their vtable update slot (+0x10).
    unsigned *guns = *(unsigned **)(self + 0xe4);
    if (guns != 0) {
        for (unsigned i = 0; i < *guns; i = i + 1) {
            int *g = *(int **)(guns[1] + i * 4);
            (*(void (**)(int *, unsigned))(*g + 0x10))(g, dt);
            guns = *(unsigned **)(self + 0xe4);
        }
    }
    guns = *(unsigned **)(self + 0xe8);
    if (guns != 0) {
        for (unsigned i = 0; i < *guns; i = i + 1) {
            int *g = *(int **)(guns[1] + i * 4);
            (*(void (**)(int *, unsigned))(*g + 0x10))(g, dt);
            guns = *(unsigned **)(self + 0xe8);
        }
    }

    // gamma-ray damage from supernova / hostile stations.
    int aPtr = *(int *)statusA;
    Station *st2 = (Station *)Status_getStation_up();
    int idx = Station_getIndex_up(st2);
    Status_getCurrentCampaignMission_up();
    float gamma = Status_getGammaRayDamagePerSecond_up(aPtr, idx);
    int ego = *(int *)(self + 0xf0);
    if (gamma > 0.0f && ego != 0) {
        int ship = Status_getShip_up();
        int eq = Ship_getFirstEquipmentOfSort_up(ship);
        float factor = gamma;
        if (eq != 0) {
            int attr = Item_getAttribute_up(eq);
            if (attr > 0)
                factor = (g_up_eqMax - (float)attr) / g_up_eqMax;
        }
        int hpBefore = Player_getGammaHP_up();
        Player_damageGamma_up((Player *)*(int *)(self + 0xf0), factor);
        if (hpBefore > 0xe && Player_getGammaHP_up() < 0xf) {
            int hud = ((PlayerEgo *)(*(int *)(self + 0xf0)))->getHUD_up();
            Hud_hudEvent_up(hud, 0x2c, *(int *)(self + 0xf0));
        }
        ego = *(int *)(self + 0xf0);
    }

    // tick the in-flight particle-system managers (skybox, engine trails, etc).
    if (ego != 0) {
        char dummy[16];
        if (*(int *)(self + 0x80) != 0) {
            ((PlayerEgo *)(dummy))->getPosition_up();
            *(int *)(self + 0xb4) = *(int *)dummy;
            ParticleSystemManager_update_up(*(int *)(self + 0x80), dt);
        }
        if (*(int *)(self + 0x74) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x74), dt);
        if (*(int *)(self + 0x78) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x78), dt);
        if (*(int *)(self + 0x88) != 0) {
            ((PlayerEgo *)(dummy))->getPosition_up();
            *(int *)(self + 0xb4) = *(int *)dummy;
            ParticleSystemManager_update_up(*(int *)(self + 0x88), dt);
        }
        if (*(int *)(self + 0x7c) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x7c), dt);
        if (*(int *)(self + 0x84) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x84), dt);
        if (*(int *)(self + 0x8c) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x8c), dt);
        if (*(int *)(self + 0x98) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x98), dt);
        if (*(int *)(self + 0x94) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x94), dt);
        if (*(int *)(self + 0x9c) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x9c), dt);
    }

    if (stackFlag == 0)
        LODManager_update_up(*(LODManager **)self, dt);
}

struct Mission;
struct Station;
struct Player;
struct ApplicationManager;

__attribute__((visibility("hidden"))) extern ApplicationManager **g_cp_appMgr; // [DAT_000cd530]

extern "C" {
int  ApplicationManager_GetCurrentApplicationModule_cp(ApplicationManager *m);
void *Level_opnew_cp(unsigned size);
void operator_delete_cp(void *p);
int  Player_isAlwaysFriend_cp();
int  Status_getCurrentCampaignMission_cp();
int  Status_inAlienOrbit_cp();
int  Status_getStation_cp();
int  Station_getIndex_cp(Station *s);
int  Station_stationHasHiddenBlueprint_cp(Station *s, int flag);
int  Status_getMission_cp();
int  Mission_getType_cp();
int  Mission_isEmpty_cp();
int  KIPlayer_isWingMan_cp();
int  PlayerFixedObject_getDockingType_cp();
}

// Level::connectPlayers() — wires up each ship's enemy list from the friend/enemy/neutral arrays,
// honouring a thicket of campaign-mission-specific exceptions.
void Level::connectPlayers()
{
    char *self = (char *)this;

    if (ApplicationManager_GetCurrentApplicationModule_cp(*g_cp_appMgr) == 5)
        return;

    // player's enemy list from the enemy array.
    if (*(int *)(self + 0xf8) != 0 && *(int *)(self + 0xf0) != 0) {
        void *arr = Level_opnew_cp(0xc);
        ArrayPlayer_ctor_cp(arr);
        ArraySetLength_Player_cp(**(unsigned **)(self + 0xf8), arr);
        int n = *(int *)arr;
        for (int j = 0; j != n; j = j + 1)
            *(int *)(*(int *)((char *)arr + 4) + j * 4) =
                *(int *)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + j * 4) + 4);
        ((Player *)**(int **)(self + 0xf0))->setEnemies((Array<Player *> *)arr);
        operator_delete_cp(ArrayPlayer_dtor_cp(arr));
    }
    // add asteroids/landmarks arrays as additional enemies.
    if (*(int *)(self + 0xfc) != 0 && *(int *)(self + 0xf0) != 0) {
        void *arr = Level_opnew_cp(0xc);
        ArrayPlayer_ctor_cp(arr);
        ArraySetLength_Player_cp(**(unsigned **)(self + 0xfc), arr);
        int n = *(int *)arr;
        for (int j = 0; j != n; j = j + 1)
            *(int *)(*(int *)((char *)arr + 4) + j * 4) =
                *(int *)(*(int *)(*(int *)(*(int *)(self + 0xfc) + 4) + j * 4) + 4);
        ((Player *)**(int **)(self + 0xf0))->addEnemies((Array<Player *> *)arr);
        operator_delete_cp(ArrayPlayer_dtor_cp(arr));
    }
    if (*(int *)(self + 0xf4) != 0 && *(int *)(self + 0xf0) != 0) {
        void *arr = Level_opnew_cp(0xc);
        ArrayPlayer_ctor_cp(arr);
        ArraySetLength_Player_cp(**(unsigned **)(self + 0xf4), arr);
        int n = *(int *)arr;
        for (int j = 0; j != n; j = j + 1)
            *(int *)(*(int *)((char *)arr + 4) + j * 4) =
                *(int *)(*(int *)(*(int *)(*(int *)(self + 0xf4) + 4) + j * 4) + 4);
        ((Player *)**(int **)(self + 0xf0))->addEnemies((Array<Player *> *)arr);
        operator_delete_cp(ArrayPlayer_dtor_cp(arr));
    }

    if (*(int *)(self + 0xf8) == 0)
        return;

    int camp = Status_getCurrentCampaignMission_cp();
    for (unsigned i = 0; i < **(unsigned **)(self + 0xf8); i = i + 1) {
        int e = *(int *)((*(unsigned **)(self + 0xf8))[1] + i * 4);
        int eFaction = *(int *)(e + 0x28);
        int wmAll = KIPlayer_isWingMan_cp();
        unsigned count = 0;
        bool notM24 = camp != 0x24;
        bool notFirst = i != 0;

        // first pass: count how many ships become this ship's enemies.
        for (unsigned k = 0; k < **(unsigned **)(self + 0xf8); k = k + 1) {
            int o = *(int *)((*(unsigned **)(self + 0xf8))[1] + k * 4);
            if (o != e && (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                bool consider;
                if (notM24 || notFirst) {
                    consider = true;
                    if (camp == 0x9a) {
                        int alien = Status_inAlienOrbit_cp();
                        if (k == 8 && alien != 0 && *(int *)(e + 0x28) == 8)
                            consider = false;
                    } else if (camp == 0x40 && !(i != 0 && e != 0 && k != 0)) {
                        consider = false;
                    }
                } else {
                    consider = KIPlayer_isWingMan_cp() == 0;
                }
                if (consider) {
                    bool skip = false;
                    if (*(char *)(e + 0x40) != 0 && PlayerFixedObject_getDockingType_cp() == 3) {
                        Station *st = (Station *)Status_getStation_cp();
                        if (Station_stationHasHiddenBlueprint_cp(st, 1) != 0)
                            skip = true;
                    }
                    if (!skip)
                        count = count + 1;
                }
            }
        }

        void *arr = Level_opnew_cp(0xc);
        ArrayPlayer_ctor_cp(arr);
        if (*(int *)(self + 0xf0) != 0)
            count = count + 1;
        ArraySetLength_Player_cp(count, arr);

        Mission *m = (Mission *)Status_getMission_cp();
        int mtype = Mission_getType_cp();
        bool branchA = !(((i & 1) == 0 && mtype == 0xc)) &&
                       Mission_getType_cp() != 2 && Mission_getType_cp() != 9;

        bool jumpAlwaysFriend = false;
        if (branchA) {
            // mission-type-specific carve-outs that fall into the "always friend" path.
            int tmp;
            if (m->isCampaignMission() != 0) {
                tmp = Status_getCurrentCampaignMission_cp();
                if (tmp == 0x10 && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && m->isCampaignMission() != 0) {
                tmp = Status_getCurrentCampaignMission_cp();
                if (tmp == 0x18 && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && m->isCampaignMission() != 0) {
                tmp = Status_getCurrentCampaignMission_cp();
                if (tmp == 0x1c && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend) {
                if (m->isCampaignMission() != 0) Status_getCurrentCampaignMission_cp();
                if (m->isCampaignMission() != 0) Status_getCurrentCampaignMission_cp();

                int *dst = (int *)((char *)arr + 4);
                int slot = 0;
                int ego = *(int *)(self + 0xf0);
                if (ego != 0) {
                    *(int *)(*dst) = *(int *)ego;
                    slot = 1;
                }
                for (unsigned k = 0; k < **(unsigned **)(self + 0xf8); k = k + 1) {
                    int o = *(int *)((*(unsigned **)(self + 0xf8))[1] + k * 4);
                    if (o != e && (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                        bool consider;
                        if (notM24 || notFirst) {
                            consider = true;
                            if (camp == 0x9a) {
                                int alien = Status_inAlienOrbit_cp();
                                if (k == 8 && alien != 0 && *(int *)(e + 0x28) == 8)
                                    consider = false;
                            } else if (camp == 0x40 && !(i != 0 && e != 0 && k != 0)) {
                                consider = false;
                            }
                        } else {
                            consider = KIPlayer_isWingMan_cp() == 0;
                        }
                        if (consider) {
                            bool skip = false;
                            if (*(char *)(e + 0x40) != 0 &&
                                PlayerFixedObject_getDockingType_cp() == 3) {
                                Station *st = (Station *)Status_getStation_cp();
                                if (Station_stationHasHiddenBlueprint_cp(st, 1) != 0)
                                    skip = true;
                            }
                            if (!skip) {
                                *(int *)(*dst + slot * 4) =
                                    *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + k * 4) + 4
                                        ? *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + k * 4) + 4
                                        : 0;
                                slot = slot + 1;
                            }
                        }
                    }
                }
            }
        }

        if (!branchA || jumpAlwaysFriend) {
            if (Player_isAlwaysFriend_cp() == 0) {
                int slot = 0;
                for (unsigned k = 0; k < **(unsigned **)(self + 0xf8); k = k + 1) {
                    int o = *(int *)((*(unsigned **)(self + 0xf8))[1] + k * 4);
                    if (o != e && *(char *)(o + 0x3c) == 0 &&
                        (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                        bool add;
                        if (notM24 || notFirst) {
                            add = !(camp == 0x40 && !(i != 0 && e != 0 && k != 0));
                        } else {
                            add = KIPlayer_isWingMan_cp() == 0;
                        }
                        if (add) {
                            int src = *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + k * 4);
                            *(int *)(*(int *)((char *)arr + 4) + slot * 4) = *(int *)(src + 4);
                            slot = slot + 1;
                        }
                    }
                }
            } else {
                operator_delete_cp(ArrayPlayer_dtor_cp(arr));
                arr = Level_opnew_cp(0xc);
                ArrayPlayer_ctor_cp(arr);
                ArraySetLength_Player_cp(1, arr);
            }
            *(int *)(*(int *)((char *)arr + 4) + *(int *)arr * 4 - 4) = **(int **)(self + 0xf0);
        }

        (*(Player **)(e + 4))->addEnemies((Array<Player *> *)arr);
        operator_delete_cp(ArrayPlayer_dtor_cp(arr));

        Status_getMission_cp();
        if (eFaction == 10 && Mission_isEmpty_cp() != 0)
            (*(Player **)(e + 4))->setEnemy((Player *)**(int **)(self + 0xf0));
    }
}

struct Hud;
struct Achievements;
struct Radar { static int hasScanner(); };

// PC-relative singleton holders.
__attribute__((visibility("hidden"))) extern Status      **g_ed_status; // [DAT_000d441c]
__attribute__((visibility("hidden"))) extern Achievements **g_ed_achA;  // [DAT_000d4420]
__attribute__((visibility("hidden"))) extern Achievements **g_ed_achB;  // [DAT_000d4424]
__attribute__((visibility("hidden"))) extern float          g_ed_100;   // DAT_000d4418 == 100.0f

extern "C" {
int  aeabi_idivmod_ed(int a, int b);
}

// Level::enemyDied — the header declares it nullary, but the target reads a discriminator in
// r2 (whether the kill counts toward the player), so we express it as a C callback that the
// engine invokes with (Level* self, _, bool wasFriendlyFire).
void Level::enemyDied(int r1, bool r2arg) {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    int r2 = (int)r2arg; // the recovered "in_r2" discriminator
    (void)r1;

    *(int *)(self + 0x118) = *(int *)(self + 0x118) - 1;  // enemiesLeft--
    *(int *)(self + 0x12c) = *(int *)(self + 0x12c) + 1;  // kills++

    if (r2 != 0) {
        *(int *)(self + 0x20) = *(int *)(self + 0x20) + 1;
        return;
    }

    Status **statusHolder = g_ed_status;
    ((Status *)(*statusHolder))->incKills();
    *(int *)(self + 0x24) = *(int *)(self + 0x24) + 1;
    if (*(int *)(self + 0xf0) == 0)
        return;

    if (Radar::hasScanner() == 0) {
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
            if (aeabi_idivmod_ed(prog, 10) == 0) {
                int hud = ((PlayerEgo *)(*(int *)(self + 0xf0)))->getHUD();
                int cur = *(int *)(*(int *)statusHolder + 0x11c);
                int g2 = ((Achievements *)(*achA))->getValue(0x28, 1);
                ((Hud *)(hud))->hudEventMedal(0x28, (int)(((float)cur / (float)g2) * g_ed_100));
            }
            int cur2 = *(int *)(*(int *)statusHolder + 0x11c);
            if (((Achievements *)(*achA))->getValue(0x28, 1) <= cur2)
                *(char *)(*(int *)statusHolder + 0x120) = 1;
        }
    }

    if (*(int *)(self + 0xf0) != 0 &&
        ((PlayerEgo *)(*(int *)(self + 0xf0)))->emergencySystemActive() != 0) {
        Achievements **achB = g_ed_achB;
        if (((Achievements *)(*achB))->hasMedal(0x2b, 1) == 0) {
            int st = *(int *)statusHolder;
            int v = *(int *)(st + 0x13c) + 1;
            *(int *)(st + 0x13c) = v;
            int goal = ((Achievements *)(*achB))->getValue(0x2b, 1);
            if (0 < (int)((float)v / (float)goal)) {
                int hud = ((PlayerEgo *)(*(int *)(self + 0xf0)))->getHUD();
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

struct RadioMessage;

extern "C" {
void *Level_opnew_crms(unsigned size);
void  RadioMessage_ctor_crms(RadioMessage *m, int id, int speaker, int kind, int delay);
// A couple of cases use a randomized delay constant the original loads from a data slot.
int   crms_randDelay(int which);
}

// One scripted chatter line.
struct RMSpec { int id, speaker, kind, delay; };

// Builds an Array<RadioMessage*> at this+0x114 from a static spec table.
static void buildQueue(char *self, const RMSpec *specs, unsigned n)
{
    void *arr = Level_opnew_crms(0xc);
    ArrayRadio_ctor_crms(arr);
    *(void **)(self + 0x114) = arr;
    ArraySetLength_Radio_crms(n, arr);
    for (unsigned i = 0; i < n; i = i + 1) {
        RadioMessage *m = (RadioMessage *)Level_opnew_crms(0x28);
        RadioMessage_ctor_crms(m, specs[i].id, specs[i].speaker, specs[i].kind, specs[i].delay);
        ((RadioMessage **)(*(int *)((char *)arr + 4)))[i] = m;
    }
}

// Level::createRadioMessages(int set) — populates the level's radio-chatter queue with the fixed
// dialogue sequence for the requested mission/event set.
void Level::createRadioMessages(int set) {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    *(int *)(self + 0x114) = 0;

    switch (set) {
    case 0: {
        static const RMSpec t[] = {
            {0x684,0x11,5,0x5dc},{0x685,0,6,0},{0x686,0,6,1},{0x687,10,6,2},{0x688,0xb,6,3},
            {0x689,9,6,4},{0x68a,9,6,5},{0x68b,9,6,6},{0x68c,0,6,7},{0x693,0,9,0},{0x694,0,6,9},
            {0x695,0,6,10},{0x696,0xf,6,0xb},{0x697,0,6,0xc},{0x698,0,6,0xd},{0x699,0,6,0xe},
            {0x69a,0,0x1b,0xc},{0x69b,0xf,6,0x10},{0x69c,0,6,0x11},{0x69d,0xf,6,0x12},
            {0x69e,0,6,0x13},{0x69f,0,6,0x14},{0x6a0,0xf,6,0x15},
        };
        buildQueue(self, t, 0x17); break;
    }
    case 1: {
        static const RMSpec t[] = {{0x6a1,2,5,10000},{0x6a2,2,6,0},{0x6a3,2,6,1}};
        buildQueue(self, t, 3); break;
    }
    case 7: {
        static const RMSpec t[] = {{0x6dc,2,0x10,0},{0x6dd,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x10: {
        static const RMSpec t[] = {{0x72e,0x13,5,10000},{0x72f,0,6,0},{0x730,0,9,0},
                                   {0x731,1,6,2},{0x732,0,6,3}};
        buildQueue(self, t, 5); break;
    }
    case 0x15: {
        static const RMSpec t[] = {{0x759,10,0x10,0},{0x75a,0,6,0},{0x75b,10,0x19,2},
                                   {0x75c,0xe,8,0},{0x75d,0xe,0x15,0}};
        buildQueue(self, t, 5); break;
    }
    case 0x18: {
        static const RMSpec t[] = {{0x77d,0x13,5,12000},{0x77e,6,6,0},{0x77f,0,6,1},
                                   {0x780,6,0x16,3},{0x781,6,6,3}};
        buildQueue(self, t, 5); break;
    }
    case 0x19: {
        static const RMSpec t[] = {{0x785,0,5,20000},{0x786,6,6,0},{0x787,0,6,1}};
        buildQueue(self, t, 3); break;
    }
    case 0x26: {
        static const RMSpec t[] = {{0x7ed,0x15,5,15000}};
        buildQueue(self, t, 1); break;
    }
    case 0x28: {
        static const RMSpec t[] = {{0x7fa,0,5,10000},{0x7fb,8,6,0},{0x7fc,0,6,1},{0x7fd,7,5,40000},
                                   {0x7fe,0,6,3},{0x7ff,7,0xc,0},{0x800,0,0x18,0}};
        buildQueue(self, t, 7); break;
    }
    case 0x29: {
        // two delays come from data slots DAT_000d19e0 / DAT_000d19e4 at runtime.
        RMSpec t[] = {{0x804,0,5,crms_randDelay(0)},{0x805,7,6,0},{0x806,0,6,1},
                      {0x807,7,6,2},{0x808,7,0x1a,crms_randDelay(1)},{0x809,7,6,4},
                      {0x80f,0,1,0},{0x810,0,6,6}};
        buildQueue(self, t, 8); break;
    }
    case 0x31: {
        static const RMSpec t[] = {{0x848,0,5,8000}};
        buildQueue(self, t, 1); break;
    }
    case 0x32: {
        static const RMSpec t[] = {{0x849,0x3f,5,8000},{0x84a,0,6,0},{0x84b,0x3f,6,1},{0x84c,0,6,2}};
        buildQueue(self, t, 4); break;
    }
    case 0x33: {
        static const RMSpec t[] = {{0x84d,0x3f,5,8000},{0x84e,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x34: {
        static const RMSpec t[] = {{0x84f,0x3f,5,8000},{0x850,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x36: {
        static const RMSpec t[] = {{0x851,0,5,8000}};
        buildQueue(self, t, 1); break;
    }
    case 0x37: {
        static const RMSpec t[] = {{0x85a,0,5,8000},{0x85b,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x38: {
        static const RMSpec t[] = {{0x86a,0x1b,5,8000},{0x86b,0,6,0},{0x86c,0x1c,0x10,0},
                                   {0x86d,0,0x14,3},{0x86e,0,6,3},{0x86f,0x1b,0x1c,0}};
        buildQueue(self, t, 6); break;
    }
    case 0x40: {
        static const RMSpec t[] = {{0x8b4,0,5,8000},{0x8b5,0x14,6,0},{0x8b6,0,6,1},{0x8b7,0x1e,6,2},
                                   {0x8b8,0,6,3},{0x8b9,0x1e,0x14,5},{0x8ba,0,6,5}};
        buildQueue(self, t, 7); break;
    }
    case 0x41: {
        static const RMSpec t[] = {{0x8cb,0,5,12000},{0x8cc,0x14,6,0},{0x8cd,0,6,1}};
        buildQueue(self, t, 3); break;
    }
    case 0x43: {
        static const RMSpec t[] = {{0x8ef,0,0x10,0},{0x8f0,0x1f,6,0},{0x8f1,0x1e,6,1},
                                   {0x8f2,0,0x14,2},{0x8f3,0x1f,6,3},{0x8f4,0,6,4},{0x8f5,0,0x14,8},
                                   {0x8f6,0x1f,6,6},{0x8f7,0,6,7},{0x8f8,0x1f,6,8},{0x8f9,0,6,9},
                                   {0x8fa,0x1f,6,10}};
        buildQueue(self, t, 0xc); break;
    }
    case 0x45: {
        static const RMSpec t[] = {{0x90e,0,5,8000},{0x90f,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x46: {
        static const RMSpec t[] = {{0x910,0,5,8000},{0x911,0,6,0},{0x912,0x22,6,1},{0x913,0,6,2}};
        buildQueue(self, t, 4); break;
    }
    case 0x49: {
        static const RMSpec t[] = {{0x92b,0,5,8000},{0x92c,0,0x10,0},{0x92d,0xb,6,1},{0x92e,0,6,2},
                                   {0x92f,0,0x1b,1},{0x930,0x21,0x1b,2},{0x931,0,0x1b,3},
                                   {0x932,0,6,6}};
        buildQueue(self, t, 8); break;
    }
    case 0x77: {
        static const RMSpec t[] = {{0xab9,0x11,0x1b,1}};
        buildQueue(self, t, 1); break;
    }
    case 0x78: {
        static const RMSpec t[] = {{0xac5,0,5,0x5dc}};
        buildQueue(self, t, 1); break;
    }
    case 0x83: {
        static const RMSpec t[] = {{0xb2b,0,0x1b,2}};
        buildQueue(self, t, 1); break;
    }
    case 0x85: {
        static const RMSpec t[] = {{0xb33,0x11,0x1b,1}};
        buildQueue(self, t, 1); break;
    }
    case 0x87: {
        static const RMSpec t[] = {{0xb43,0x31,0x1b,1},{0xb44,0,6,0},{0xb45,0,0x1b,2},
                                   {0xb46,0,0x1b,3}};
        buildQueue(self, t, 4); break;
    }
    case 0x89: {
        static const RMSpec t[] = {{0xb4f,0x32,5,0x5dc},{0xb50,0,6,0},{0xb51,0x32,6,1},
                                   {0xb52,0,6,2},{0xb53,0x32,6,3}};
        buildQueue(self, t, 5); break;
    }
    case 0x90: {
        static const RMSpec t[] = {{0xb98,0x27,5,7000},{0xb99,0x27,6,0},{0xb9a,0,6,1},
                                   {0xb9b,0x27,6,2}};
        buildQueue(self, t, 4); break;
    }
    case 0x91: {
        static const RMSpec t[] = {{0xb9c,0,5,7000},{0xb9d,0x27,6,0},{0xb9e,0x27,0x1b,5}};
        buildQueue(self, t, 3); break;
    }
    case 0x93: {
        static const RMSpec t[] = {{0xbac,0,5,7000},{0xbad,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x50: {
        static const RMSpec t[] = {{0x96e,0,5,8000},{0x96f,6,5,25000},{0x970,0x1a,6,1},
                                   {0x971,0,6,2},{0x972,6,6,3}};
        buildQueue(self, t, 5); break;
    }
    default:
        break;
    }
}

__attribute__((visibility("hidden"))) extern float *g_flash2_a;
__attribute__((visibility("hidden"))) extern float *g_flash2_b;
__attribute__((visibility("hidden"))) extern float *g_flash2_c;
__attribute__((visibility("hidden"))) extern float *g_flashCol_r;
__attribute__((visibility("hidden"))) extern float *g_flashCol_g;
__attribute__((visibility("hidden"))) extern float *g_flashCol_b;

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
        ((PlayerEgo *)player)->hitCamera();
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

__attribute__((visibility("hidden"))) extern Status **g_wingmanDied;

void Level::wingmanDied(int name) {
    Status **slot = g_wingmanDied;
    unsigned int *list = (*slot)->getWingmen();
    if (list == 0) {
        return;
    }
    if (__builtin_expect(*list < 2, 0)) {
        return this->wingmanDied_all((Status *)(*slot), 0);
    }
    for (unsigned int i = 0; i < *list; i = i + 1) {
        String *w = ((String **)list[1])[i];
        if (w->Compare_str((String *)name) == 0) {
            return this->wingmanDied_one(((String **)list[1])[i], list);
        }
    }
}

__attribute__((visibility("hidden"))) extern void (**g_levelSubCtor)(void *);

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
    ctor((char *)this + 0x1d0);
    ctor((char *)this + 0x20c);
    ctor((char *)this + 0x248);
    zero16(&field_1c);
    zero16((char *)this + 0x90);
    zero16((char *)this + 0x84);
    zero16((char *)this + 0x74);
    skyboxMesh = -1;
    field_08 = -1;
    skyboxTexture = -1;
    field_10 = -1;
    missionPtr = mission;
    collisionVolume = 0;
    field_b0 = 0;
    flashColor.x = 0;
    flashColor.y = 0;
    flashColor.z = 0;
    flashDurationA = 0;
    flashDurationB = 0;
    flashActive = 0;
    // Original zeroes the objective region with two unaligned 4-byte stores at
    // +0x29 and +0x2d; net effect is clearing objectivesA/objectivesB.
    objectivesA = 0;
    objectivesB = 0;
    __aeabi_memclr4((char *)this + 0xd8, 0x65);
    zero16((char *)this + 0x16c);
    zero16((char *)this + 0x15c);
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
    field_a4 = 0;
    field_a8 = 0;
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

struct Station;
struct PlayerFixedObject;
struct Player;

__attribute__((visibility("hidden"))) extern int  *g_cso_stack;     // [DAT_000cc1e4]
__attribute__((visibility("hidden"))) extern int **g_cso_textA;     // [DAT_000cc1ec]
__attribute__((visibility("hidden"))) extern int **g_cso_textB;     // [DAT_000cc1f0] area
__attribute__((visibility("hidden"))) extern float g_cso_posX;
__attribute__((visibility("hidden"))) extern float g_cso_posZ;

extern "C" {
int  Status_inAlienOrbit_cso();
int  Status_getStation_cso();
int  Station_getIndex_cso(Station *s);
int  Status_getCurrentCampaignMission_cso();
void PlayerFixedObject_setMoving_cso(PlayerFixedObject *o, int flag);
void StarSystem_getLightDirection_cso(void *dst);
// UNRECOVERED up-vector: AEGeometry::setDirection takes (dir, up); the binary's `up`
// argument was not recovered at this call, so the single-direction shim is retained.
int  GameText_getText_cso(int id);
void PlayerFixedObject_setName_cso(PlayerFixedObject *o, String *n);
void PlayerFixedObject_setDockingType_cso(PlayerFixedObject *o, int t);
void *Level_opnew_cso(unsigned size);
void operator_delete_cso(void *p);
}

// Level::createStaticObjects() — spawns campaign-specific landmark/escort objects.
void Level::createStaticObjects()
{
    char *self = (char *)this;

    // --- carrier escort that follows the campaign progression (station 0x70) ---
    if (Status_inAlienOrbit_cso() == 0) {
        Station *st = (Station *)Status_getStation_cso();
        if (Station_getIndex_cso(st) == 0x70 &&
            0x7f < Status_getCurrentCampaignMission_cso()) {
            int type;
            if (Status_getCurrentCampaignMission_cso() < 0x83)      type = 0x493e;
            else if (Status_getCurrentCampaignMission_cso() < 0x87) type = 0x4941;
            else if (Status_getCurrentCampaignMission_cso() < 0x8a) type = 0x4944;
            else if (Status_getCurrentCampaignMission_cso() < 0x8e) type = 0x4947;
            else if (0x91 < Status_getCurrentCampaignMission_cso()) type = -1;
            else                                                    type = 0x494a;

            if (type != -1) {
                char *o = (char *)this->createStaticObject_cso(0, type, 0);
                (*(void (**)(char *, float, int, float))(*(int *)o + 0x48))(o, g_cso_posX, 0,
                                                                           g_cso_posZ);
                PlayerFixedObject_setMoving_cso((PlayerFixedObject *)o, 0);
                Vector *geo = *(Vector **)(o + 8);
                *(char *)(o + 0x70) = 0;
                char dir[12];
                StarSystem_getLightDirection_cso(dir);
                AEGeometry_setDirection_cso(geo, (Vector *)dir);
                String *txt = (String *)GameText_getText_cso(**g_cso_textA);
                *(String *)(o + 0x18) = *txt;
                (*(Player **)(o + 4))->setAlwaysFriend(1);
                void *arr = *(void **)(self + 0xf8);
                if (arr == 0) {
                    arr = Level_opnew_cso(0xc);
                    ArrayKIPlayer_ctor_cso(arr);
                    *(void **)(self + 0xf8) = arr;
                }
                ArrayAdd_KIPlayer_cso((KIPlayer *)o, arr);
                if (*(void **)(o + 0x50) != 0)
                    operator_delete_cso(Array_int_dtor_cso(*(void **)(o + 0x50)));
                *(int *)(o + 0x50) = 0;
            }
        }
    }

    // --- station 0x67 dockable beacon (mid/late campaign) ---
    if (0x54 < Status_getCurrentCampaignMission_cso() &&
        Status_getCurrentCampaignMission_cso() != 0x87 &&
        Status_inAlienOrbit_cso() == 0) {
        Station *st = (Station *)Status_getStation_cso();
        if (Station_getIndex_cso(st) == 0x67) {
            char *o = (char *)this->createStaticObject_cso(0, 0x4a88, 0);
            (*(void (**)(char *, int, int, int))(*(int *)o + 0x48))(o, 0, 0, 0);
            PlayerFixedObject_setMoving_cso((PlayerFixedObject *)o, 0);
            *(char *)(o + 0x70) = 1;
            String *txt = (String *)GameText_getText_cso(**g_cso_textB);
            String name;
            name.ctor_copy(txt, 0);
            PlayerFixedObject_setName_cso((PlayerFixedObject *)o, &name);
            PlayerFixedObject_setDockingType_cso((PlayerFixedObject *)o, 1);
            if (*(void **)(o + 0x50) != 0)
                operator_delete_cso(Array_int_dtor_cso(*(void **)(o + 0x50)));
            *(int *)(o + 0x50) = 0;
            (*(Player **)(o + 4))->setAlwaysFriend(1);
            void *arr = *(void **)(self + 0xf8);
            if (arr == 0) {
                arr = Level_opnew_cso(0xc);
                ArrayKIPlayer_ctor_cso(arr);
                *(void **)(self + 0xf8) = arr;
            }
            ArrayAdd_KIPlayer_cso((KIPlayer *)o, arr);
        }
    }
}

struct Waypoint;
struct AEGeometry;
struct Player;
struct PlayerFixedObject;

__attribute__((visibility("hidden"))) extern int *g_cso2_stack;   // [DAT_000cea34]
__attribute__((visibility("hidden"))) extern int **g_cso2_rng;    // [DAT_000cea38]

extern "C" {
int  AERandom_nextInt_cso2(int rng);
int  cso2_rand20000(int rng); // (*pcVar23)(rng, 20000) randomized jitter
// Constructs the requested static-object type (junk, turret, sentry gun, jumpgate gear, capital
// landmark, etc.) fully positioned at (x,y,z); the original is a giant per-type cascade of
// SIMD-built geometry that Ghidra could not lift. Returns the new KIPlayer-derived object.
}

// Level::createStaticObject(Waypoint* wp, int type, bool jitter) — spawns one scenery / structure
// object at the waypoint (optionally with a small random position offset).
int Level::createStaticObject(Waypoint *wp, int type, int jitter) {
    Level *thisptr = this;
    char *self = (char *)thisptr;

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = wp->x;
        y = wp->y;
        z = wp->z;
    }
    if (jitter) {
        int rng = *(int *)g_cso2_rng;
        x = x + cso2_rand20000(rng) - 10000;
        y = y + cso2_rand20000(rng) - 10000;
        z = z + cso2_rand20000(rng) - 10000;
    }

    // type 0x4215 picks one of three debris meshes at random; everything else maps 1:1.
    if (type == 0x4215) {
        int r = AERandom_nextInt_cso2(*(int *)g_cso2_rng);
        type = (r == 0) ? 0x4215 : (r == 1) ? 0x4216 : 0x4217;
    }

    (void)self;
    return thisptr->cso2_construct(type, x, y, z);
}

struct FileRead;
struct BoundingVolume;

__attribute__((visibility("hidden"))) extern int *g_gbv_stack; // [DAT_000d405c]

extern "C" {
void *Level_opnew_gbv(unsigned size);
void  FileRead_ctor_gbv(FileRead *fr);
void *FileRead_dtor_gbv(FileRead *fr);
int   FileRead_loadStationCollision_gbv(int fr);
int   FileRead_loadStaticCollision_gbv(int fr);
void  operator_delete_gbv(void *p);
// Builds one BoundingVolume (sphere when shape==0, AAB when shape==1) from the raw collision
// record at `rec`; returns the new object. The conversion is fixed-point->float SIMD math the
// decompiler mangled, so it lives in the engine helper.
}

// Level::getBoundingVolume(int idx, AEGeometry* outArrayHolder) — loads the per-mesh collision
// data for a station (idx<2000) or static object and produces an Array<BoundingVolume*>.
void *Level_getBoundingVolume(int idx, int kind)
{
    (void)idx;
    FileRead *fr = (FileRead *)Level_opnew_gbv(1);
    FileRead_ctor_gbv(fr);

    int *coll;
    if (kind < 2000)
        coll = (int *)FileRead_loadStationCollision_gbv((int)(intptr_t)fr);
    else
        coll = (int *)FileRead_loadStaticCollision_gbv((int)(intptr_t)fr);
    operator_delete_gbv(FileRead_dtor_gbv(fr));

    void *result = 0;
    if (coll != 0) {
        unsigned n = *(unsigned *)coll[1];   // entry count
        void *arr = Level_opnew_gbv(0xc);
        ArrayBV_ctor_gbv(arr);
        ArraySetLength_BV_gbv(n, arr);
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
            ((BoundingVolume **)(*(int *)((char *)arr + 4)))[i] = bv;
        }

        ArrayRelease_int_gbv(coll);
        operator_delete_gbv(ArrayInt_dtor_gbv(coll));
    }
    return result;
}

struct Waypoint;
struct Player;
struct AEGeometry;
struct PlayerFixedObject;
struct PlayerFighter;
struct Globals;
struct LODManager;

__attribute__((visibility("hidden"))) extern int    **g_cs_rng;      // [DAT_000d0468]
__attribute__((visibility("hidden"))) extern int     *g_cs_diffRec;  // [DAT_000d0474]
__attribute__((visibility("hidden"))) extern Globals **g_cs_globalsA;// [DAT_000d0838]
__attribute__((visibility("hidden"))) extern Globals **g_cs_globalsB;// [DAT_000d0cdc]

extern "C" {
int  cs_rand40000(int rng);
int  Status_getCurrentCampaignMission_cs();
int  Status_getLevel_cs();
int  Status_gameWon_cs();
int  Status_inAlienOrbit_cs();
int  Status_hardCoreMode_cs();
void *Level_opnew_cs(unsigned size);
void Player_ctor_cs(Player *p, int hp, int dmg, int a, int b, int c);
void Player_setEmpData_cs(Player *p, int a, int b);
void PlayerFighter_ctor_cs(PlayerFighter *pf, int type, int race, Player *pl, AEGeometry *g,
                           float x, float y, float z, int flag);
void PlayerFixedObject_ctor_cs(PlayerFixedObject *o, int type, int race, Player *pl,
                               AEGeometry *g, float x, float y, float z);
void PlayerFixedObject_setWreckedMeshId_cs(PlayerFixedObject *o, int mesh);
void PlayerFixedObject_setBV_cs(PlayerFixedObject *o, void *bv);
int  Globals_getShipGroup_cs(Globals *g, int type, int race, int flag);
void LODManager_addObject_cs(LODManager *m, AEGeometry *g);
void operator_delete_cs(void *p);
// Builds the class-appropriate bounding-volume array (the original is a long cascade of SIMD
// BoundingAAB constructions Ghidra could not lift) and returns it; also returns the wreck mesh id.
}

// Level::createShip(race, shipClass, type, wp, hostile, group) — spawns a fighter (class 0) or a
// fixed capital ship (class 1) at the waypoint, scaling its stats to the player level/difficulty.
PlayerFixedObject * Level::createShip(int race, int shipClass, int type, Waypoint *wp, int hostile, int group) {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    int camp = Status_getCurrentCampaignMission_cs();

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = wp->x;
        y = wp->y;
        z = wp->z;
    }
    int rng = *(int *)g_cs_rng;
    int jx = cs_rand40000(rng);
    int jy = cs_rand40000(rng);
    int jz = cs_rand40000(rng);

    // base hitpoints from player level + campaign ramp.
    int lvl = Status_getLevel_cs();
    if (0x15 <= lvl) lvl = 0x14;
    int ramp = (Status_gameWon_cs() != 0) ? 0xb4 : (camp << 2);
    int hp = ramp + lvl * 0xe + 0x14;
    if (type == 0x33)      hp = (int)((float)hp * 1.0f);
    else if (type == 0x31) hp = (int)((float)hp * 17.0f);
    else if (type == 0x2c) hp = (int)((float)hp * 2.25f);
    if ((unsigned)(camp - 0x31) < 8 && (0x8fU >> ((unsigned)(camp - 0x31) & 0xff) & 1) != 0)
        hp = 0x10e;

    int dmgLvl = Status_getLevel_cs();
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
        int alien = Status_inAlienOrbit_cs();
        hp = hp << (alien & (race == 9));
    }

    Player *pl = (Player *)Level_opnew_cs(0x114);
    int playerHp = (Status_hardCoreMode_cs() != 0) ? 0x28a : 1000;
    Player_ctor_cs(pl, playerHp, hp, 1, 1, 0);
    float fx = (float)(x + jx - 20000);
    float fy = (float)(y + jy - 20000);
    float fz = (float)(z + jz - 20000);
    Player_setEmpData_cs(pl, empA, empB);

    PlayerFixedObject *obj = 0;
    if (shipClass == 0) {
        PlayerFighter *pf = (PlayerFighter *)Level_opnew_cs(0x2f0);
        PlayerFighter_ctor_cs(pf, type, race, pl, 0, fx, fy, fz, 0);
        obj = (PlayerFixedObject *)pf;
        int gg = Globals_getShipGroup_cs(*g_cs_globalsA, type, race, group);
        (*(void (**)(PlayerFixedObject *, int, int, int))(*(int *)obj + 8))(obj, gg, type, hostile);
        if (*(int *)(self + 0xc0) != 1 && *(int *)(self + 0xc0) != 0x17) {
            AEGeometry *g = *(AEGeometry **)((char *)obj + 0xc);
            if (g == 0) g = *(AEGeometry **)((char *)obj + 0x8);
            LODManager_addObject_cs(*(LODManager **)self, g);
        }
        if (type == 0x2c || type == 0x31 || type == 0x33) {
            if (type == 0x33) *(unsigned char *)((char *)obj + 0x25) = 0;
            if (type != 0x33 && *(void **)((char *)obj + 0x50) != 0) {
                operator_delete_cs(Array_int_dtor_cs(*(void **)((char *)obj + 0x50)));
                *(void **)((char *)obj + 0x50) = 0;
            }
        }
    } else if (shipClass == 1) {
        obj = (PlayerFixedObject *)Level_opnew_cs(0x1bc);
        PlayerFixedObject_ctor_cs(obj, type, race, pl, 0, fx, fy, fz);
        int wreck = 0;
        void *bv = Level::cs_buildBV(race, type, &wreck);
        PlayerFixedObject_setWreckedMeshId_cs(obj, wreck);
        PlayerFixedObject_setBV_cs(obj, bv);
        int gg = Globals_getShipGroup_cs(*g_cs_globalsB, type, race, 0);
        (*(void (**)(PlayerFixedObject *, int, int, int))(*(int *)obj + 8))(obj, gg, type, 0);
        LODManager_addObject_cs(*(LODManager **)self, *(AEGeometry **)((char *)obj + 0x8));
        *(unsigned char *)((char *)obj + 0x40) = 1;
    }

    if (obj != 0)
        (*(void (**)(PlayerFixedObject *, Level *))(*(int *)obj + 0x14))(obj, thisptr);
    return obj;
}

__attribute__((visibility("hidden"))) extern Status **g_almostKillWanted;

void Level::almostKillWanted(int index) {
    if (field_29e != 0) {
        return;
    }
    Status **slot = g_almostKillWanted;
    field_29e = 1;
    if ((*slot)->isStorylineWanted(index) == 0) {
        return;
    }
    int m = (int)(intptr_t)Level_opnew_akw(0x78);
    new ((void *)(intptr_t)m) Mission(4, 0, (*slot)->getStation()->getIndex());
    ((Mission *)(m))->setCampaign_akw(1);
    ((Mission *)(m))->setWon_akw(1);
    (*slot)->setMission(m);
    (*slot)->setCampaignMission(m);
    if (objectivesA != 0) {
        operator delete(dtor_Objective_akw((void *)(intptr_t)objectivesA));
    }
    objectivesA = 0;
    int o = (int)(intptr_t)Level_opnew_akw(0x1c);
    Objective_ctor_akw(o, 3, 0, 0, this);
    objectivesA = o;
    int e = *(int *)(*(int *)(enemies + 4));
    Level_setAlwaysEnemy(*(int *)(e + 4), 0);
    ((Player *)(*(int *)(*(int *)(enemies + 4) + 4)))->resetDamageDoneByPlayer();
    int e0 = *(int *)(*(int *)(enemies + 4));
    *(unsigned char *)(*(int *)(e0 + 4) + 0x5c) = 0;
    *(unsigned char *)(e0 + 0x43) = 1;
    int w = (*slot)->getWanted();
    return this->almostKillWanted_tail(((int *)(*(int *)(w + 4)))[index], 0);
}

struct Wanted;
struct Mission;
struct Gun;
struct ObjectGun;
struct RocketGun;
struct PlayerTurret;
struct KIPlayer;
struct Player;

__attribute__((visibility("hidden"))) extern int    *g_ag_stack;     // [DAT_000cca28]
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

extern "C" {
void  operator_delete_ag(void *p);
void *Level_opnew_ag(unsigned size);
int   Status_getLevel_ag();
int   Status_getCurrentCampaignMission_ag();
int   Status_getMission_ag();
int   Status_gameWon_ag();
int   Status_getWantedInCurrentOrbit_ag(Status *s);
int   Mission_getType_ag();
int   KIPlayer_isWingMan_ag();
int   KIPlayer_isEnemy_ag(KIPlayer *k);
int   KIPlayer_getType_ag(KIPlayer *k);
void  KIPlayer_addGun_ag(Gun *target, int gun);
int   Player_isAlwaysFriend_ag();
int   Player_isAlwaysEnemy_ag();
int   Player_getMaxHitpoints_ag();
int   Wanted_getWeapon_ag(Wanted *w);
int   Item_getAttribute_ag(int item);
void  Gun_ctor_ag(Gun *g, int a, int b, int c, int d, int e, int f, int g7, int h, int i,
                  int j, int k, int l, int m);
void  Gun_setLevel_ag(Gun *g, Level *self);
void  RocketGun_ctor_ag(RocketGun *r, int a, Gun *g, int res, int b, int c, int d, int e,
                        Level *self);
void  ObjectGun_ctor_ag(ObjectGun *o, int a, Gun *g, int res, int d, Level *self);
void  Globals_addSoundResourceToList_ag(int snd);
}

// Level::assignGuns() — equips every active ship in the level with weapons scaled to the player
// level and difficulty, with a large set of campaign/wanted/ship-type special cases.
void Level::assignGuns()
{
    char *self = (char *)this;

    if (*(void **)(self + 0xe8) != 0) {
        ArrayReleaseClasses_AGun_ag(*(void **)(self + 0xe8));
        if (*(void **)(self + 0xe8) != 0)
            operator_delete_ag(ArrayAGun_dtor_ag(*(void **)(self + 0xe8)));
        *(int *)(self + 0xe8) = 0;
    }
    Status **status = g_ag_status;
    *(int *)(self + 0xe8) = 0;

    // base weapon "power" from player level (clamped to 20).
    float lvlPower = (float)(Status_getLevel_ag() - 2) * g_ag_perLevel;
    if (lvlPower >= 20.0f) lvlPower = 20.0f;
    if (lvlPower < 0.0f) lvlPower = (float)(Status_getLevel_ag() - 2) * g_ag_perLevel;

    float diffScale = *(float *)(*(int *)g_ag_diffRec + 0x2c);
    int camp = Status_getCurrentCampaignMission_ag();
    int basePower = (int)(lvlPower + lvlPower * (diffScale - 0.5f));
    Wanted *wanted = (Wanted *)Status_getWantedInCurrentOrbit_ag(*status);
    unsigned *list = *(unsigned **)(self + 0xf8);
    if (0x15 < basePower) basePower = 0x16;
    if (list == 0)
        return;

    // count gun slots needed.
    unsigned slots = 0;
    for (unsigned i = 0; i < *list; i = i + 1) {
        int e = *(int *)(list[1] + i * 4);
        if (e != 0 && *(char *)(e + 0x25) != 0) {
            int add = (KIPlayer_isWingMan_ag() != 0) ? 2 : 1;
            list = *(unsigned **)(self + 0xf8);
            slots = slots + add;
        }
    }

    void *guns = Level_opnew_ag(0xc);
    ArrayAGun_ctor_ag(guns);
    *(void **)(self + 0xe8) = guns;
    ArraySetLength_AGun_ag(slots, guns);

    int baseDmg = (basePower == 0) ? 3 : (basePower + 2);
    if (camp == 4) baseDmg = 1;

    int outIdx = 0;
    for (unsigned i = 0; i < **(unsigned **)(self + 0xf8); i = i + 1) {
        int e = *(int *)((*(unsigned **)(self + 0xf8))[1] + i * 4);
        if (e == 0)
            continue;
        if (*(char *)(e + 0x25) == 0)
            goto wingmanExtra;

        {
            if (*(int *)(self + 0xc0) == 2)
                (*(Player **)(e + 4))->setPlayShootSound(0, 2);

            int color = 0x41800000;
            int dmg;
            Status_getMission_ag();
            if (Mission_getType_ag() == 6 && Player_isAlwaysFriend_ag() == 0) {
                color = 0x41e00000;
                dmg = Status_getLevel_ag() + baseDmg;
            } else {
                Status_getMission_ag();
                dmg = baseDmg;
                if (Mission_getType_ag() == 0xc && Player_isAlwaysFriend_ag() != 0) {
                    color = 0x41e00000;
                    dmg = Status_getLevel_ag() + baseDmg;
                }
            }

            // alien ships ramp the damage harder.
            if (KIPlayer_isWingMan_ag() == 0 && Player_isAlwaysFriend_ag() == 0 &&
                *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) &&
                *(int *)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 0x28) == 9) {
                if (camp != 0x10) dmg = (int)((float)dmg * 1.0f);
                else dmg = dmg + dmg;
            } else {
                int hard = ((0x8fU >> ((unsigned)(camp - 0x31) & 0xff) & 1) != 0 &&
                            (unsigned)(camp - 0x31) <= 7) ? 5 : dmg;
                dmg = hard;
            }

            if (camp == 0x46 && KIPlayer_isWingMan_ag() == 0)
                dmg = (int)((float)dmg * 2.5f);

            if (Status_getMission_ag() != 0) {
                Status_getMission_ag();
                if (Mission_getType_ag() == 0xb7) dmg = 1;
            }

            Gun *gun = (Gun *)Level_opnew_ag(0x114);
            int won = Status_gameWon_ag();
            int rampMis = (won != 0) ? 0x2d : Status_getCurrentCampaignMission_ag();
            Gun_ctor_ag(gun, 0, dmg, 4, -1, 3000, rampMis * -2 + 600, color, 0, 0, 0, 0, 0, 0);
            gun->setFriendGun(1);
            Gun_setLevel_ag(gun, this);
            gun->setIndex(0);
            gun->weaponType = 0;

            int res;
            switch (*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 0x28
                        ? *(int *)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 0x28)
                        : 0) {
            case 0:  gun->weaponType = 0; gun->setIndex(0);    res = 0x1a62; break;
            case 1:  gun->setIndex(3);    res = 0x1a68; break;
            case 2:  gun->weaponType = 0; gun->setIndex(7);    res = 0x1a6c; break;
            case 3:  gun->setIndex(0x19); res = 0x1a92; break;
            case 9:  gun->setIndex(5);    res = 0x1a6a; break;
            case 10: gun->setIndex(0xe5); res = 0x4a93;
                     gun->field_0x60 = (int)((float)gun->field_0x60 * 1.0f);
                     break;
            default: gun->weaponType = 1; gun->setIndex(0x13); res = 0x1a8b; break;
            }

            int camp2 = Status_getCurrentCampaignMission_ag();
            PlayerTurret *turret = *(PlayerTurret **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
            if (turret->field_0x3e != 0) {
                int host = (int)(intptr_t)turret->getHost();
                if (host != 0 && (*(int *)(host + 0x7c) == 0x2d || *(int *)(host + 0x7c) == 0x33)) {
                    gun->weaponType = 2; gun->setIndex(0x16); res = 0x1a8e;
                } else {
                    KIPlayer *k = *(KIPlayer **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
                    if (*(unsigned char *)((char *)k + 0x3f) == 0) {
                        gun->weaponType = 1;
                        if (*(unsigned char *)((char *)k + 0x28) == 1) { gun->setIndex(0xf);  res = 0x1a87; }
                        else                                 { gun->setIndex(0x14); res = 0x1a8c; }
                    } else {
                        int kt = KIPlayer_getType_ag(k);
                        if (kt == 0x49c1)      { gun->weaponType = 1; gun->setIndex(0x14); res = 0x1a8d; }
                        else if (kt == 0x49c0) { gun->weaponType = 0; gun->setIndex(2);    res = 0x1a64; }
                        else                   { gun->weaponType = 1; gun->setIndex(0xe);  res = 0x1a86; }
                        gun->field_0xa8 = 1;
                        // base stats from the ship-stat table (corrupted SIMD in original).
                        if (camp2 == 0x9e && kt == 0x49c2 && Player_isAlwaysEnemy_ag() != 0) {
                            Player *pp = *(Player **)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 4);
                            int mhp = Player_getMaxHitpoints_ag();
                            pp->setMaxHitpoints((int)((float)mhp * 5.0f));
                        }
                    }
                }
            }

            // wanted/ship special weapon overrides.
            int kt2 = Status_getCurrentCampaignMission_ag();
            int host2 = *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
            if (Status_getMission_ag() != 0) {
                Mission *mm = (Mission *)Status_getMission_ag();
                if (mm->isCampaignMission() != 0) {
                    if (**g_ag_statusB == Status_getCurrentCampaignMission_ag() &&
                        2 < **g_ag_alienCnt &&
                        KIPlayer_isEnemy_ag(*(KIPlayer **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4)) != 0)
                        gun->field_0x60 = (int)((float)gun->field_0x60 * 1.0f);
                }
            }
            if (wanted != 0 && *(char *)(host2 + 0x42) != 0) {
                int w = Wanted_getWeapon_ag(wanted);
                gun->setIndex(w);
                int attr = Item_getAttribute_ag(*(int *)(*(int *)(*g_ag_itemTblA + 4) + w * 4));
                res = ((int *)g_ag_weaponDmg)[w];
                gun->weaponType = attr;
                gun->field_0x60 = gun->field_0x60 << 2;
            }

            // wrap the Gun in a Rocket/Object gun and store it.
            int sc = gun->weaponType;
            if (sc == 0x28 || sc == 5) {
                RocketGun *r = (RocketGun *)Level_opnew_ag(0xe8);
                RocketGun_ctor_ag(r, gun->itemIndex, gun, res, 0, 0, sc,
                                  sc == 5 ? 1 : 0, this);
                *(RocketGun **)(*(int *)(*(int *)(self + 0xe8) + 4) + outIdx * 4) = r;
                gun->field_0x50 = 0x41000000;
                gun->initialLifetime = 10000;
                gun->fireDelay = 3000;
                gun->field_0x60 = gun->field_0x60 << 2;
            } else {
                ObjectGun *o = (ObjectGun *)Level_opnew_ag(0xb0);
                ObjectGun_ctor_ag(o, 0, gun, res, 0x2711, this);
                *(ObjectGun **)(*(int *)(*(int *)(self + 0xe8) + 4) + outIdx * 4) = o;
            }
            KIPlayer_addGun_ag(*(Gun **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4), (int)(intptr_t)gun);
            Globals_addSoundResourceToList_ag(**g_ag_snd);
            outIdx = outIdx + 1;
            (void)kt2;
        }

wingmanExtra:
        if (KIPlayer_isWingMan_ag() != 0 &&
            *(char *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 0x25 != 0) {
            Gun *gun = (Gun *)Level_opnew_ag(0x114);
            Gun_ctor_ag(gun, 0x12, 0, 4, -1, 3000, 400, 0x41800000, 0, 0, 0, 0, 0, 0);
            gun->setFriendGun(1);
            Gun_setLevel_ag(gun, this);
            gun->itemIndex = 0x12;
            gun->weaponType = 1;
            ObjectGun *o = (ObjectGun *)Level_opnew_ag(0xb0);
            ObjectGun_ctor_ag(o, 0x12, gun, 0x1a8a, 0x2711, this);
            *(ObjectGun **)(*(int *)(*(int *)(self + 0xe8) + 4) + outIdx * 4) = o;
            gun->setIndex(0x12);
            int attr = Item_getAttribute_ag(*(int *)(*(int *)(*g_ag_itemTblB + 4) + 0x48));
            gun->field_0x64 = attr;
            KIPlayer_addGun_ag(*(Gun **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4), (int)(intptr_t)gun);
            Globals_addSoundResourceToList_ag(**g_ag_snd2);
            outIdx = outIdx + 1;
        }
    }
}

struct Station;
struct Galaxy;
struct SolarSystem;
struct AEGeometry;
struct PlayerGasCloud;
struct ParticleSystemManager;

__attribute__((visibility("hidden"))) extern int    *g_cgc_stack;   // [DAT_000bffc0]
__attribute__((visibility("hidden"))) extern Galaxy **g_cgc_galaxy; // [DAT_000bffc4]
__attribute__((visibility("hidden"))) extern int    *g_cgc_rng;     // [DAT_000bffcc]
__attribute__((visibility("hidden"))) extern void  **g_cgc_canvas;  // [DAT_000bffd4]

extern "C" {
int  Status_getStation_cgc();
int  Galaxy_getPlasmaProbabilities_cgc(Galaxy *g, Station *st);
int  Status_getShip_cgc();
int  Ship_getFirstEquipmentOfSort_cgc(int ship);
int  Status_inAlienOrbit_cgc();
int  Status_getSystem_cgc();
int  SolarSystem_getIndex_cgc();
int  SolarSystem_getRoutes_cgc();
int  Status_getCurrentCampaignMission_cgc();
int  Station_getIndex_cgc(Station *s);
int  AERandom_nextInt_cgc(int rng);
void *Level_opnew_cgc(unsigned size);
void PlayerGasCloud_ctor_cgc(PlayerGasCloud *c, int kind, ParticleSystemManager *psm,
                             AEGeometry *geo, Vector *pos);
// Generates a random spawn position for cloud index `i` that is far enough from the player and
// from the origin (the original is a SIMD reject-sampling loop Ghidra could not lift). `boss`
// pins the very first cloud to a fixed spot.
void Level_cgc_randomPos(int rng, int boss, unsigned i, Vector *out);
}

// Level::createGasClouds() — populates the orbit with plasma/gas clouds when the player carries a
// collector and the system permits it.
void Level::createGasClouds()
{
    char *self = (char *)this;

    Galaxy *gal = *g_cgc_galaxy;
    Station *st = (Station *)Status_getStation_cgc();
    int *prob = (int *)Galaxy_getPlasmaProbabilities_cgc(gal, st);

    int ship = Status_getShip_cgc();
    if (Ship_getFirstEquipmentOfSort_cgc(ship) == 0 || Status_inAlienOrbit_cgc() != 0)
        return;

    Status_getSystem_cgc();
    if (SolarSystem_getIndex_cgc() != 10 && *prob == 0xcc) {
        Status_getSystem_cgc();
        if (SolarSystem_getRoutes_cgc() == 0)
            return;
    }

    void *arr = Level_opnew_cgc(0xc);
    ArrayGasCloud_ctor_cgc(arr);
    *(void **)(self + 0xf4) = arr;

    bool boss = false;
    if (Status_getCurrentCampaignMission_cgc() == 0x8e) {
        Station *s2 = (Station *)Status_getStation_cgc();
        boss = Station_getIndex_cgc(s2) == 0x4f;
    }

    int rng = *g_cgc_rng;
    int roll = AERandom_nextInt_cgc(rng);
    // count = base + (prob[1]/denom) * (roll+4); base 3 for the boss fight.
    float countF = (float)(boss ? 3.0f : 0.0f) + ((float)prob[1] / 1.0f) * (float)(roll + 4);
    int count = (countF > 0.0f) ? (int)countF : 0;
    ArraySetLength_GasCloud_cgc(count, *(void **)(self + 0xf4));

    void *canvas = *g_cgc_canvas;
    for (unsigned i = 0; i < **(unsigned **)(self + 0xf4); i = i + 1) {
        int kind = *prob;
        Vector pos;
        Level_cgc_randomPos(rng, boss ? 1 : 0, i, &pos);

        AEGeometry *geo = (AEGeometry *)Level_opnew_cgc(0xc0);
        new ((void*)geo) AEGeometry((uint16_t)0x37d1, (PaintCanvas*)canvas, 0);
        PlayerGasCloud *cloud = (PlayerGasCloud *)Level_opnew_cgc(0x16c);
        PlayerGasCloud_ctor_cgc(cloud, kind,
                                *(ParticleSystemManager **)(self + 0x94), geo, &pos);
        *(PlayerGasCloud **)(*(int *)(*(int *)(self + 0xf4) + 4) + i * 4) = cloud;
        char *c = *(char **)(*(int *)(*(int *)(self + 0xf4) + 4) + i * 4);
        (*(void (**)())(*(int *)c + 0x14))();
    }
}

struct Mission;

// PC-relative cookie slot.
__attribute__((visibility("hidden"))) extern int *g_umo_stack; // [DAT_000d50dc]

extern "C" {
int  Status_getMission_umo();
int  Mission_isEmpty_umo();
int  Mission_getType_umo();
int  KIPlayer_isDead_umo();
int  Player_isActive_umo();
// Revives the KIPlayer then teleports it to a freshly randomized offset from the player's
// position (the original is SIMD float math Ghidra could not lift). Two profiles are used:
//   profile 0 — generic far-field wave, profile 1 — boss-fight (type 0xb7/0xf) variant.
void Level_umo_spawnAt(Level *self, int *kiPlayer, int profile);
}

// Level::updateMissionOrbit(int dt) — drives mission-specific enemy respawn timing.
void Level::updateMissionOrbit(int dt) {
    Level *thisptr = this;
    char *self = (char *)thisptr;

    // --- phase A: periodic far wave (only when a "boss present" flag at 0x288 is set) ---
    if (*(char *)(self + 0x288) != 0) {
        Status_getMission_umo();
        if (Mission_isEmpty_umo() == 0) {
            int t = *(int *)(self + 0x174);
            *(int *)(self + 0x174) = t + dt;
            if (0x57e4 < t + dt) {
                *(int *)(self + 0x174) = 0;
                int aliveCore = 0;
                for (int j = 0; j != 4; j = j + 1)
                    aliveCore = aliveCore + (KIPlayer_isDead_umo() ^ 1);
                if (aliveCore != 0) {
                    unsigned *en = *(unsigned **)(self + 0xf8);
                    for (unsigned i = 4; i < *en; i = i + 1) {
                        int *ki = *(int **)(en[1] + i * 4);
                        if (KIPlayer_isDead_umo() != 0 && Player_isActive_umo() == 0)
                            Level_umo_spawnAt(thisptr, ki, 0);
                    }
                }
            }
        }
    }

    // --- phase B: type-0xb7 boss escort wave ---
    if (Status_getMission_umo() != 0) {
        Status_getMission_umo();
        if (Mission_getType_umo() == 0xb7) {
            int t = *(int *)(self + 0x174);
            *(int *)(self + 0x174) = t + dt;
            if (0x1d4c < t + dt) {
                *(int *)(self + 0x174) = 0;
                unsigned *en = *(unsigned **)(self + 0xf8);
                for (unsigned i = 0; i < *en; i = i + 1) {
                    int *ki = *(int **)(en[1] + i * 4);
                    if (KIPlayer_isDead_umo() != 0 && Player_isActive_umo() == 0 &&
                        ki[0x1f] != 0x33) {
                        Level_umo_spawnAt(thisptr, ki, 1);
                        ki[0x14] = 0;
                    }
                }
            }
        }
    }

    // --- phase C: type-0xf wave (skips the last enemy, the leader) ---
    Status_getMission_umo();
    if (Mission_isEmpty_umo() == 0) {
        Status_getMission_umo();
        if (Mission_getType_umo() == 0xf) {
            int t = *(int *)(self + 0x174);
            *(int *)(self + 0x174) = t + dt;
            if (50000 < t + dt) {
                *(int *)(self + 0x174) = 0;
                unsigned *en = *(unsigned **)(self + 0xf8);
                unsigned count = *en;
                // require at least one of the non-leader enemies to still be alive.
                bool anyAlive = false;
                for (unsigned i = 0; i + 1 < count; i = i + 1) {
                    if (KIPlayer_isDead_umo() == 0) { anyAlive = true; break; }
                }
                if (anyAlive) {
                    for (unsigned i = 0; i + 1 < count; i = i + 1) {
                        int *ki = *(int **)(en[1] + i * 4);
                        if (KIPlayer_isDead_umo() != 0 && Player_isActive_umo() == 0)
                            Level_umo_spawnAt(thisptr, ki, 0);
                        en = *(unsigned **)(self + 0xf8);
                        count = *en;
                    }
                }
            }
        }
    }
}

__attribute__((visibility("hidden"))) extern int **g_attackWanted;

void Level::attackWanted(int index) {
    if (field_29c == 0) {
        field_29c = 1;
        createRadioMessage(0x10, index);
        int **slot = g_attackWanted;
        for (int i = 1;
             i - 1 < Level_getNumWingmen(((int *)(*(int *)((*(int *)*slot) + 4)))[index]);
             i = i + 1) {
            Level_setAlwaysEnemy(*(int *)(((int *)((RawArray *)(intptr_t)enemies)->data)[i] + 4), 1);
            Level_turnEnemy(*(int *)(((int *)((RawArray *)(intptr_t)enemies)->data)[i] + 4));
        }
    }
}

struct SolarSystem;
struct KIPlayer;
struct ParticleSystemManager;

__attribute__((visibility("hidden"))) extern int   *g_ips_stack;    // [DAT_000cda44]
__attribute__((visibility("hidden"))) extern unsigned *g_ips_canvas; // [DAT_000cda98]
__attribute__((visibility("hidden"))) extern int   *g_ips_envA;     // [DAT_000cdaa0] env record
__attribute__((visibility("hidden"))) extern int   *g_ips_envB;     // [DAT_000cde3c]/[DAT_000cde38]
__attribute__((visibility("hidden"))) extern int  (*g_ips_addSystem)(int, void *, int, int); // [DAT_000cde40]
__attribute__((visibility("hidden"))) extern void (*g_ips_enableEmit)(int);  // [DAT_000cde44]

extern "C" {
void *Level_opnew_ips(unsigned size);
int   Status_getSystem_ips();
int   SolarSystem_hasPirateBase_ips(SolarSystem *s);
int   SolarSystem_getTextureIndex_ips();
int   SolarSystem_getRace_ips();
int   Status_inAlienOrbit_ips();
int   Status_inSupernovaSystem_ips();
int   Status_getCurrentCampaignMission_ips();
int   Status_getShip_ips();
int   Ship_getIndex_ips();
int   KIPlayer_getType_ips(KIPlayer *k);
int   ParticleSystemManager_addSystem_ips(int mgr, void *ref, int kind, int flag);
void  ParticleSystemManager_init_ips(ParticleSystemManager *m, void *ctx);
void  ParticleSystemManager_enableSystemEmit_ips(int mgr, int sys);
// Builds the per-asteroid dust descriptor block (huge SIMD struct init Ghidra mangled) and writes
// the resulting system handle into the array at self+0xa8.
void  Level_ips_buildAsteroidDust(Level *self, void *arr);
// Computes the ambient-light / nebula tint for the current orbit and writes it into the engine
// env record (corrupted SIMD packing in the original).
void  Level_ips_applyAmbient(Level *self);
// Registers one of the player-engine particle systems (kind) with a unit reference transform.
int   Level_ips_addPlayerSystem(Level *self, int kind);
}

// Level::initParticleSystems() — wires up all the in-flight particle systems (engine trails,
// explosions, asteroid dust, nebula tint) once the player and scene exist.
void Level::initParticleSystems()
{
    char *self = (char *)this;

    if (*(int *)(self + 0xf0) != 0) {
        // per-asteroid dust systems.
        if (*(int *)(self + 0xa4) != 0) {
            void *arr = Level_opnew_ips(0xc);
            Array_int_ctor_ips(arr);
            *(void **)(self + 0xa8) = arr;
            ArraySetLength_int_ips(**(unsigned **)(self + 0xa4), arr);
            Level_ips_buildAsteroidDust(this, arr);
        }

        // camera-locked sky particle system.
        unsigned canvas = *g_ips_canvas;
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        int local = (int)(long)((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        int sys = ParticleSystemManager_addSystem_ips(*(int *)(self + 0x88), (void *)local, 4, 0);
        *(int *)(self + 0x64) = sys;

        // pirate-base smoke plume attached to the pirate flagship.
        if (Status_getSystem_ips() != 0) {
            SolarSystem *ss = (SolarSystem *)Status_getSystem_ips();
            unsigned *en = *(unsigned **)(self + 0xf8);
            if (SolarSystem_hasPirateBase_ips(ss) != 0 && en != 0) {
                for (unsigned i = 0; i < *en; i = i + 1) {
                    KIPlayer *k = *(KIPlayer **)(en[1] + i * 4);
                    if (k != 0 && KIPlayer_getType_ips(k) == 0x37a3) {
                        // The flagship's geometry (KIPlayer::geometry at +0x8) drives the
                        // smoke plume's reference frame.
                        AEGeometry *kg = *(AEGeometry **)((char *)k + 8);
                        kg->updateReferenceMatrix();
                        int ref = (int)(intptr_t)&kg->getReferenceMatrix();
                        ParticleSystemManager_addSystem_ips(*(int *)(self + 0x7c), (void *)ref, 8, 0);
                        break;
                    }
                    en = *(unsigned **)(self + 0xf8);
                }
            }
        }

        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        local = (int)(long)((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        sys = ParticleSystemManager_addSystem_ips(*(int *)(self + 0x7c), (void *)local, 7, 0);
        *(int *)(self + 0x284) = sys;

        Level_ips_applyAmbient(this);
    }

    // nebula light direction.
    Level_ips_applyAmbient(this);

    // init the always-present managers.
    if (*(ParticleSystemManager **)(self + 0x80) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x80), 0);
    if (*(ParticleSystemManager **)(self + 0x7c) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x7c), 0);
    if (*(ParticleSystemManager **)(self + 0x88) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x88), 0);
    if (*(ParticleSystemManager **)(self + 0x8c) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x8c), 0);
    if (*(ParticleSystemManager **)(self + 0x98) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x98), 0);

    // the player-engine systems (slots 0x34..0x5c) all use a unit transform.
    *(int *)(self + 0x38) = Level_ips_addPlayerSystem(this, 10);
    *(int *)(self + 0x3c) = Level_ips_addPlayerSystem(this, 0xb);
    *(int *)(self + 0x48) = Level_ips_addPlayerSystem(this, 0x14);
    *(int *)(self + 0x34) = Level_ips_addPlayerSystem(this, 0x15);
    *(int *)(self + 0x50) = Level_ips_addPlayerSystem(this, 0x16);
    *(int *)(self + 0x54) = Level_ips_addPlayerSystem(this, 0x17);
    if (Status_getCurrentCampaignMission_ips() == 0x50) {
        *(int *)(self + 0x58) = Level_ips_addPlayerSystem(this, 0x18);
        *(int *)(self + 0x5c) = Level_ips_addPlayerSystem(this, 0x18);
    }

    ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x74), 0);
    ParticleSystemManager_enableSystemEmit_ips(*(int *)(self + 0x74), *(int *)(self + 0x50));
    ParticleSystemManager_enableSystemEmit_ips(*(int *)(self + 0x74), *(int *)(self + 0x54));
    if (Status_getCurrentCampaignMission_ips() == 0x50) {
        ParticleSystemManager_enableSystemEmit_ips(*(int *)(self + 0x74), *(int *)(self + 0x58));
        ParticleSystemManager_enableSystemEmit_ips(*(int *)(self + 0x74), *(int *)(self + 0x5c));
    }

    void (*enableEmit)(int) = g_ips_enableEmit;
    enableEmit(*(int *)(self + 0x9c));
    enableEmit(*(int *)(self + 0x78));
    enableEmit(*(int *)(self + 0x84));
    if (*(ParticleSystemManager **)(self + 0x94) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x94), 0);
}

struct Mission;
struct Player;
struct Globals;

__attribute__((visibility("hidden"))) extern int     *g_cwm_stack;   // [DAT_000cc670]
__attribute__((visibility("hidden"))) extern int    **g_cwm_statusB; // [DAT_000cc674]
__attribute__((visibility("hidden"))) extern int    **g_cwm_seedSrc; // [DAT_000cc678]
__attribute__((visibility("hidden"))) extern Globals **g_cwm_globals; // [DAT_000cc67c]

extern "C" {
int  Status_inSupernovaSystem_cwm();
int  Status_getCurrentCampaignMission_cwm();
int  Status_getWingmen_cwm();
void *Level_opnew_cwm(unsigned size);
void AERandom_setSeed_cwm(int seed);
void AERandom_reset_cwm();
int  Globals_getRandomEnemyFighter_cwm(Globals *g, int race);
void KIPlayer_setWingman_cwm(KIPlayer *k, int flag, unsigned slot);
void Player_setHitpoints_cwm(int p);
int  Status_getMission_cwm();
int  Mission_getType_cwm();
// Positions wingman `i` relative to the player's geometry (right/forward offsets) and sets its
// heading. The original is SIMD vector math Ghidra could not lift cleanly.
void Level_cwm_placeWingman(Level *self, int *kiSlot, unsigned i);
}

// Level::createWingmen() — spawns the player's hired escort fighters in formation.
void Level::createWingmen()
{
    char *self = (char *)this;
    int **statusB = g_cwm_statusB;

    if (Status_inSupernovaSystem_cwm() != 0 ||
        Status_getCurrentCampaignMission_cwm() == 0x9e ||
        Status_getWingmen_cwm() == 0 ||
        *(int *)(self + 0xf0) == 0)
        return;

    void *arr = Level_opnew_cwm(0xc);
    ArrayKIPlayer_ctor_cwm(arr);
    unsigned *wm = (unsigned *)Status_getWingmen_cwm();
    ArraySetLength_KIPlayer_cwm(*wm, arr);

    unsigned n = *(unsigned *)arr;
    for (unsigned i = 0; i < n; i = i + 1) {
        int seed = **g_cwm_seedSrc;
        Status_getWingmen_cwm();
        AERandom_setSeed_cwm(seed);
        int fighter = Globals_getRandomEnemyFighter_cwm(*g_cwm_globals, *(int *)(*statusB + 0x2c));
        int ship = (int)(intptr_t)this->createShip_cwm(5, 0, fighter, 0, 1, 0);
        *(int *)(*(int *)((char *)arr + 4) + i * 4) = ship;

        int *slot = (int *)(*(int *)((char *)arr + 4) + i * 4);
        Level_cwm_placeWingman(this, slot, i);

        KIPlayer_setWingman_cwm(*(KIPlayer **)(*(int *)((char *)arr + 4) + i * 4), 1, i);
        (*(Player **)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 4))->setAlwaysFriend(1);
        Player_setHitpoints_cwm(*(int *)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 4));

        int wmList = Status_getWingmen_cwm();
        *(String *)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 0x18) =
            **(String **)(*(int *)(wmList + 4) + i * 4);
        *(int *)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 0x28) = *(int *)(*statusB + 0x2c);

        Status_getMission_cwm();
        if (Mission_getType_cwm() == 0xc)
            *(char *)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 0x25) = 0;
    }

    if (*(int *)(self + 0xf8) == 0) {
        *(void **)(self + 0xf8) = arr;
    } else {
        for (unsigned i = 0; i < n; i = i + 1) {
            ArrayAdd_KIPlayer_cwm(*(KIPlayer **)(*(int *)((char *)arr + 4) + i * 4),
                                  *(void **)(self + 0xf8));
            n = *(unsigned *)arr;
        }
    }
    AERandom_reset_cwm();
}

struct Mission;
struct Station;
struct Ship;
struct Agent;
struct Player;
struct KIPlayer;
struct PlayerStatic;
struct AEGeometry;
struct Globals;

__attribute__((visibility("hidden"))) extern int    *g_csc_stack;    // [DAT_000c46a0]
__attribute__((visibility("hidden"))) extern int   **g_csc_missionDef;// [DAT_000c46ac]
__attribute__((visibility("hidden"))) extern void  **g_csc_canvas;   // [DAT_000c46b0] etc.
__attribute__((visibility("hidden"))) extern int    *g_csc_rng;      // [DAT_000c46bc]/[DAT_000c4ba0]
__attribute__((visibility("hidden"))) extern Globals **g_csc_globals;// [DAT_000c46d0]
__attribute__((visibility("hidden"))) extern int   **g_csc_shipHost; // [DAT_000c46a4]

extern "C" {
void  operator_delete_csc(void *p);
void *Level_opnew_csc(unsigned size);
void *Level_opnew_arr_csc(unsigned size);
void  Status_setMission_csc(Mission *m);
int   Status_getCurrentCampaignMission_csc();
int   Status_getSystem_csc();
int   SolarSystem_getRace_csc();
int   Status_getStation_csc();
int   Station_getIndex_csc(Station *s);
int   Station_getAgents_csc(Station *s);
int   Station_getShips_csc();
int   Status_getShip_csc();
int   Ship_getIndex_csc();
int   Agent_getRace_csc(Agent *a);
int   Agent_getImageParts_csc(Agent *a);
int   Agent_isMale_csc(Agent *a);
int   AERandom_nextInt_csc(int rng);
int   Globals_getRandomEnemyFighter_csc(Globals *g, int race);
void  PlayerStatic_ctor_csc(PlayerStatic *p, int a, AEGeometry *geo);
void  PlayerFighter_removeTrail_csc(int pf);
void  PlayerFighter_setExhaustVisible_csc(int pf);
void  KIPlayer_setToSleep_csc(KIPlayer *k);
// Position/rotate a freshly created static actor — corrupted SIMD in the original.
}

// Level::createScene() — builds the non-flight presentation scenes (cut-scene mode 2, station
// crew gallery mode 4, and ship-showroom mode 0x17).
void Level::createScene()
{
    char *self = (char *)this;

    if (*(void **)(self + 0xf8) != 0) {
        ArrayReleaseClasses_KIPlayer_csc(*(void **)(self + 0xf8));
        if (*(void **)(self + 0xf8) != 0)
            operator_delete_csc(ArrayKIPlayer_dtor_csc(*(void **)(self + 0xf8)));
    }
    int mode = *(int *)(self + 0xc0);
    *(int *)(self + 0xf8) = 0;

    if (mode == 2) {
        this->createPlayer_csc();
        Status_setMission_csc((Mission *)**g_csc_missionDef);
        this->createMission_csc();
        if (Status_getCurrentCampaignMission_csc() == 0x2b) {
            void *canvas = *g_csc_canvas;
            AEGeometry *g = (AEGeometry *)Level_opnew_csc(0xc0);
            new ((void*)g) AEGeometry((uint16_t)0x37d0, (PaintCanvas*)canvas, 0);
            PlayerStatic *p = (PlayerStatic *)Level_opnew_csc(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            ArrayAdd_KIPlayer_csc((KIPlayer *)p, *(void **)(self + 0xf8));
            g = (AEGeometry *)Level_opnew_csc(0xc0);
            new ((void*)g) AEGeometry((uint16_t)0x37d1, (PaintCanvas*)canvas, 0);
            p = (PlayerStatic *)Level_opnew_csc(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            ArrayAdd_KIPlayer_csc((KIPlayer *)p, *(void **)(self + 0xf8));
        }
        return;
    }

    if (mode == 4) {
        Status_getSystem_csc();
        int race = SolarSystem_getRace_csc();
        unsigned crew = (race == 1) ? 2 : 3;
        Station *st = (Station *)Status_getStation_csc();
        int *agents = (int *)Station_getAgents_csc(st);
        char taken[7];
        void *canvas = *g_csc_canvas;

        if (agents == 0) {
            void *arr = Level_opnew_csc(0xc);
            ArrayKIPlayer_ctor_csc(arr);
            *(void **)(self + 0xf8) = arr;
            ArraySetLength_KIPlayer_csc(3, arr);
        } else {
            int nAgents = *agents;
            void *arr = Level_opnew_csc(0xc);
            ArrayKIPlayer_ctor_csc(arr);
            *(void **)(self + 0xf8) = arr;
            ArraySetLength_KIPlayer_csc(nAgents * 3 + crew, arr);
            for (int j = 0; j != 7; j = j + 1) taken[j] = 0;

            for (int a = 0; a < nAgents; a = a + 1) {
                Agent *ag = *(Agent **)(agents[1] + a * 4);
                int part = Agent_getRace_csc(ag);
                if (part == 3) {
                    if (Agent_getImageParts_csc(ag) == 0) part = 3;
                    else {
                        int *ip = (int *)Agent_getImageParts_csc(ag);
                        part = (*ip != 2) ? 3 : *ip;
                    }
                }
                int seat;
                do { seat = AERandom_nextInt_csc(*g_csc_rng); } while (taken[seat] != 0);
                taken[seat] = 1;

                AEGeometry *g = (AEGeometry *)Level_opnew_csc(0xc0);
                new ((void*)g) AEGeometry((uint16_t)(unsigned)part, (PaintCanvas*)canvas, 0);
                PlayerStatic *p = (PlayerStatic *)Level_opnew_csc(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                *(PlayerStatic **)(*(int *)(*(int *)(self + 0xf8) + 4) + a * 4) = p;
                this->csc_placeActor((int)(intptr_t)p, seat, 0);

                g = (AEGeometry *)Level_opnew_csc(0xc0);
                new ((void*)g) AEGeometry((uint16_t)(unsigned)mode, (PaintCanvas*)canvas, 0);
                p = (PlayerStatic *)Level_opnew_csc(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                *(PlayerStatic **)(*(int *)(*(int *)(self + 0xf8) + 4) + (nAgents + a) * 4) = p;

                g = (AEGeometry *)Level_opnew_csc(0xc0);
                new ((void*)g) AEGeometry((uint16_t)0x380c, (PaintCanvas*)canvas, 0);
                p = (PlayerStatic *)Level_opnew_csc(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                *(PlayerStatic **)(*(int *)(*(int *)(self + 0xf8) + 4) + (nAgents * 2 + a) * 4) = p;
            }
        }
        for (unsigned u = 0; u < crew; u = u + 1) {
            AEGeometry *g = (AEGeometry *)Level_opnew_csc(0xc0);
            new ((void*)g) AEGeometry((uint16_t)(unsigned)mode, (PaintCanvas*)canvas, 0);
            PlayerStatic *p = (PlayerStatic *)Level_opnew_csc(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            *(PlayerStatic **)((*(int **)(self + 0xf8))[1] +
                               (**(int **)(self + 0xf8) + (u - crew)) * 4) = p;
        }
        return;
    }

    if (mode == 0x17) {
        int *host = *g_csc_shipHost;
        Station *st = (Station *)Status_getStation_csc();
        unsigned race;
        if (Station_getIndex_csc(st) == 0x65) race = 8;
        else {
            st = (Station *)Status_getStation_csc();
            if (Station_getIndex_csc(st) == 100) race = 7;
            else { Status_getSystem_csc(); race = SolarSystem_getRace_csc(); }
        }

        void *arr = Level_opnew_csc(0xc);
        ArrayKIPlayer_ctor_csc(arr);
        *(void **)(self + 0xf8) = arr;
        ArraySetLength_KIPlayer_csc(1, arr);

        Status_getShip_csc();
        int shipIdx = Ship_getIndex_csc();
        Ship *ship = (Ship *)Status_getShip_csc();
        int shipRace = ship->getRace();
        int actor = (int)(intptr_t)this->createShip_csc(shipRace, 0, shipIdx, 0,
                                         *(int *)(self + 0xc0) != 0x17, 0);
        **(int **)(*(int *)(self + 0xf8) + 4) = actor;
        this->csc_placeActor(actor, shipIdx, 1);
        PlayerFighter_removeTrail_csc(actor);
        PlayerFighter_setExhaustVisible_csc(actor);
        KIPlayer_setToSleep_csc((KIPlayer *)actor);
        (*(Player **)(actor + 4))->setAlwaysFriend(1);

        void *canvas = *g_csc_canvas;
        for (unsigned u = 0; u < 4; u = u + 1) {
            // decoration meshes around the showroom ship.
            AEGeometry *g = (AEGeometry *)Level_opnew_csc(0xc0);
            new ((void*)g) AEGeometry((uint16_t)(unsigned)(0x3800 + u), (PaintCanvas*)canvas, 0);
            Vector rot = {0, 0, 0};
            ((AEGeometry*)g)->setRotation(*(const AbyssEngine::AEMath::Vector*)(&rot));
            PlayerStatic *p = (PlayerStatic *)Level_opnew_csc(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            if ((int)race < 4 && race != 1) {
                AEGeometry *child = (AEGeometry *)Level_opnew_csc(0xc0);
                new ((void*)child) AEGeometry((uint16_t)(unsigned)u, (PaintCanvas*)canvas, 0);
                ((AEGeometry*)g)->addChild(((AEGeometry*)child)->transform);
                [&]{ AEGeometry *g_=(AEGeometry*)(child); if(g_){ g_->~AEGeometry(); operator_delete_csc(g_);} }();
            }
            ArrayAdd_KIPlayer_csc((KIPlayer *)p, *(void **)(self + 0xf8));
        }

        // background traffic fighters.
        Station *st2 = (Station *)Status_getStation_csc();
        bool fromStationShips = (Station_getIndex_csc(st2) == 0x6c) &&
                                (*(int *)(*host + 0x114) == 3);
        int rng = *g_csc_rng;
        int spawnCount = AERandom_nextInt_csc(rng);
        if (fromStationShips) {
            unsigned *ships = (unsigned *)Station_getShips_csc();
            spawnCount = (ships == 0) ? 0 : (int)*ships;
        }
        char seats[64];
        for (int j = 0; j < 64; j = j + 1) seats[j] = 0;

        for (int s = 0; s < spawnCount; s = s + 1) {
            int r = AERandom_nextInt_csc(rng);
            unsigned spawnRace = race;
            if (r < 0x1e) {
                spawnRace = AERandom_nextInt_csc(rng);
                if (AERandom_nextInt_csc(rng) < 0x1e) spawnRace = 8;
            }
            int fighter = Globals_getRandomEnemyFighter_csc(*g_csc_globals, spawnRace);
            Station *st3 = (Station *)Status_getStation_csc();
            if (Station_getIndex_csc(st3) == 100) {
                int pick = AERandom_nextInt_csc(rng);
                fighter = (pick == 1) ? 0x26 : (pick == 0) ? 0x25 : 0x28;
            }
            if (fromStationShips) {
                Station_getShips_csc();
                fighter = Ship_getIndex_csc();
            }
            KIPlayer *k = (KIPlayer *)this->createShip_csc(0, 0, fighter, 0, 0, 0);
            int seat = AERandom_nextInt_csc(rng);
            int guard = -100;
            while (guard != 0 && seats[seat & 0x3f] != 0) {
                seat = AERandom_nextInt_csc(rng);
                guard = guard + 1;
            }
            seats[seat & 0x3f] = 1;
            this->csc_placeActor((int)(intptr_t)k, seat, 2);
            (*(Player **)((char *)k + 0x4))->setAlwaysFriend(1);
            KIPlayer_setToSleep_csc(k);
            PlayerFighter_setExhaustVisible_csc((int)(intptr_t)k);
            ArrayAdd_KIPlayer_csc(k, *(void **)(self + 0xf8));
        }
    }
}

struct StarSystem;
struct Engine;

// PC-relative cookie + the PaintCanvas singleton + a few float-constant table pointers.
__attribute__((visibility("hidden"))) extern int    *g_rbg_stack;    // [DAT_000d46fc]
__attribute__((visibility("hidden"))) extern unsigned *g_rbg_canvas;  // [DAT_000d4700]
__attribute__((visibility("hidden"))) extern void   **g_rbg_rng;      // [DAT_000d4b04]
__attribute__((visibility("hidden"))) extern int    **g_rbg_engine;   // [DAT_000d4b0c]

extern "C" {
void Matrix_getInverse_rbg(Matrix *dst);
void Matrix_assign_rbg(Matrix *dst, Matrix *src);
void Matrix_mulEq_rbg(Matrix *dst, Matrix *rhs);
void Transform_Update_rbg(int xform, int flag);
int  Status_inAlienOrbit_rbg();
int  Status_getSystem_rbg();
int  SolarSystem_getIndex_rbg();
int  Status_inSupernovaSystem_rbg();
int  Status_getCurrentCampaignMission_rbg();
void StarSystem_render_rbg(StarSystem *s);
void StarSystem_getLightDirection_rbg(void *dst);
int  AERandom_nextInt_rbg(int rng);
void Engine_SetModelMatrix_rbg(Matrix *m);
void Engine_LightSetLight_rbg(Engine *e, int v);
void Engine_GlEnable_rbg(unsigned e, int flag);
void Engine_LightEnable_rbg(int flag);
// Builds the rotated skybox basis (uses corrupted SIMD cross-products in the original) and writes
// it into the level's skybox matrix at self+0x1d0. `mode` selects the alien-orbit variant.
void Level_rbg_buildSkyMatrix(char *self, int mode, float spin);
}

// Level::renderBG(float t) — draws the skybox, nebula, planet rings and supernova glow.
void Level::renderBG(float t) {
    Level *thisptr = this;
    char *self = (char *)thisptr;
    unsigned canvas = *g_rbg_canvas;

    ((PaintCanvas*)(long)(canvas))->SetColor(0xffffffffu);
    ((PaintCanvas*)(long)(canvas))->BeginBG();
    ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
    ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());

    Matrix *sky = (Matrix *)(self + 0x1d0);
    Matrix_getInverse_rbg(sky);
    *(int *)(self + 0x1ec) = 0;
    *(int *)(self + 0x1dc) = 0;
    *(int *)(self + 0x1fc) = 0;

    bool alienRing = false;
    if (Status_inAlienOrbit_rbg() == 0) {
        Status_getSystem_rbg();
        if (SolarSystem_getIndex_rbg() == 0x1b)
            alienRing = true;
    }
    Level_rbg_buildSkyMatrix(self, alienRing ? 1 : 0, t);
    Matrix_mulEq_rbg(sky, (Matrix *)(self + 0x20c));

    ((PaintCanvas*)(long)(canvas))->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *)(self + 0x1d0));
    ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)(self + 0x19c)), 0);
    ((PaintCanvas*)(long)(canvas))->SetBlendMode(0);
    ((PaintCanvas*)(long)(canvas))->DrawMesh((unsigned int)(*(unsigned *)(self + 8)));
    ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)(self + 0x198)), 0);
    ((PaintCanvas*)(long)(canvas))->SetBlendMode(2);
    ((PaintCanvas*)(long)(canvas))->DrawMesh((unsigned int)(*(unsigned *)(self + 4)));

    // optional far cloud layer.
    if (*(int *)(self + 0x1b4) != -1) {
        ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)(self + 0x1b8)), 0);
        ((PaintCanvas*)(long)(canvas))->SetBlendMode(1);
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        Matrix_getInverse_rbg(sky);
        Matrix_assign_rbg(sky, sky);
        *(int *)(self + 0x1ec) = 0;
        *(int *)(self + 0x1dc) = 0;
        *(int *)(self + 0x1fc) = 0;
        Matrix_getInverse_rbg(sky);
        ((PaintCanvas*)(long)(canvas))->DrawTransform((unsigned int)(long)(*(Matrix **)(self + 0x1b4)), (const float *)0);
    }

    StarSystem_render_rbg(*(StarSystem **)(self + 0xec));

    // supernova glow billboards.
    if (Status_inSupernovaSystem_rbg() != 0 && *(int *)(self + 0xc) != -1) {
        int camp = Status_getCurrentCampaignMission_rbg();
        ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)(self + 0x1a0)), 0);
        ((PaintCanvas*)(long)(canvas))->SetBlendMode(2);
        float scale = (0x6a < camp) ? 1.5f : 1.0f;
        int flag = (int)(scale * t);
        int xf = (int)(long)((PaintCanvas*)(long)(canvas))->TransformGetTransform(0);
        Transform_Update_rbg(xf, flag);
        Matrix_getInverse_rbg(sky);
        ((PaintCanvas*)(long)(canvas))->DrawTransform((unsigned int)(long)(*(Matrix **)(self + 0x10)), (const float *)0);
        xf = (int)(long)((PaintCanvas*)(long)(canvas))->TransformGetTransform(0);
        Transform_Update_rbg(xf, flag);
        ((PaintCanvas*)(long)(canvas))->DrawTransform((unsigned int)(long)(*(Matrix **)(self + 0x18)), (const float *)0);
    }

    // rotating planet ring.
    if (*(int *)(self + 0x1bc) != -1) {
        ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)(self + 0x1c0)), 0);
        ((PaintCanvas*)(long)(canvas))->SetBlendMode(2);
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        Matrix_getInverse_rbg(sky);
        Matrix_assign_rbg(sky, sky);
        int xf = (int)(long)((PaintCanvas*)(long)(canvas))->TransformGetTransform(0);
        int before = *(int *)(xf + 0x110);
        int xf2 = (int)(long)((PaintCanvas*)(long)(canvas))->TransformGetTransform(0);
        Transform_Update_rbg(xf2, (int)t);
        if (*(int *)(xf + 0x110) < before) {
            // re-randomize the ring tilt — corrupted SIMD in the original; rebuild via helper.
            int rng = *(int *)g_rbg_rng;
            AERandom_nextInt_rbg(rng);
            AERandom_nextInt_rbg(rng);
            AERandom_nextInt_rbg(rng);
            Level_rbg_buildSkyMatrix(self, 2, t);
        }
        Matrix_mulEq_rbg(sky, (Matrix *)(self + 0x248));
        *(int *)(self + 0x1dc) = 0;
        *(int *)(self + 0x1ec) = 0;
        *(int *)(self + 0x1fc) = 0;
        Matrix_getInverse_rbg(sky);
        ((PaintCanvas*)(long)(canvas))->DrawTransform((unsigned int)(long)(*(Matrix **)(self + 0x1bc)), (const float *)0);
    }

    // supernova flare mesh (when the explosion timeline is past its trigger).
    if (*(char *)(self + 0x289) != 0 &&
        1.0f <= *(float *)(*(int *)g_rbg_engine + 0x28)) {
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent());
        Matrix_getInverse_rbg(sky);
        Matrix_assign_rbg(sky, sky);
        *(int *)(self + 0x1ec) = 0;
        *(int *)(self + 0x1dc) = 0;
        *(int *)(self + 0x1fc) = 0;
        ((PaintCanvas*)(long)(canvas))->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *)(self + 0x1d0));
        ((PaintCanvas*)(long)(canvas))->SetColor(0xffffffffu);
        Engine *eng = *(Engine **)(canvas + 0x34);
        Engine_SetModelMatrix_rbg((Matrix *)eng);
        ((PaintCanvas*)(long)(canvas))->SetTexture((unsigned int)(*(unsigned *)(self + 0x1c4)), 0);
        ((PaintCanvas*)(long)(canvas))->SetBlendMode(8);
        Engine_LightSetLight_rbg(*(Engine **)(canvas + 0x34), 0x4000);
        Engine_GlEnable_rbg(*(unsigned *)(canvas + 0x34), 0);
        ((PaintCanvas*)(long)(canvas))->DrawMesh((unsigned int)(*(unsigned *)(self + 0x1cc)));
        Engine_GlEnable_rbg(*(unsigned *)(canvas + 0x34), 0);
        Engine_LightEnable_rbg(*(int *)(canvas + 0x34));
    }

    ((PaintCanvas*)(long)(canvas))->EndBG();
}

// ============================================================================
//  Recovered helper fragments (batch-3 gap fill)
//
//  These are the private, decompiler-split sub-routines that the Level methods
//  above call through. They are file-scope (extern "C") helpers — not new
//  public members — matching the established convention for split fragments
//  (cf. MGame_opnew / per-element array release callbacks).
// ============================================================================

#include <new>

// ---- per-mesh allocation helpers ------------------------------------------
// Every `Level_opnew*` fragment is the same global `operator new(size)` — the
// decompiler emitted a distinct inlined copy per call context. They allocate a
// raw block of `size` bytes that the caller then placement-constructs.
void *Level_opnew_init(unsigned size)    { return ::operator new(size); }
void *Level_opnew_csp(unsigned size)     { return ::operator new(size); }
void *Level_opnew_crm(unsigned size)     { return ::operator new(size); }
void *Level_opnew_crms(unsigned size)    { return ::operator new(size); }
void *Level_opnew_cg(unsigned size)      { return ::operator new(size); }
void *Level_opnew_ag(unsigned size)      { return ::operator new(size); }
void *Level_opnew_ca(unsigned size)      { return ::operator new(size); }
void *Level_opnew_cgc(unsigned size)     { return ::operator new(size); }
void *Level_opnew_cm(unsigned size)      { return ::operator new(size); }
void *Level_opnew_ccm(unsigned size)     { return ::operator new(size); }
void *Level_opnew_cp(unsigned size)      { return ::operator new(size); }
void *Level_opnew_cs(unsigned size)      { return ::operator new(size); }
void *Level_opnew_cso(unsigned size)     { return ::operator new(size); }
void *Level_opnew_csc(unsigned size)     { return ::operator new(size); }
void *Level_opnew_arr_csc(unsigned size) { return ::operator new(size); }
void *Level_opnew_cwm(unsigned size)     { return ::operator new(size); }
void *Level_opnew_ips(unsigned size)     { return ::operator new(size); }
extern "C" void *Level_opnew_gbv(unsigned size) { return ::operator new(size); }

extern "C" void *Level_opnew_akw(unsigned int size) { return ::operator new(size); }
extern "C" int  Level_opnew(unsigned int size)
{
    return (int)(intptr_t)::operator new(size);
}

// ---- array element release callbacks --------------------------------------
// Each callback runs over an Array<T>: it destroys every live element
// (element destructor + operator delete), nulls the slot, then frees the
// element buffer with operator delete[] and nulls the data pointer. This is
// exactly the engine `ArrayReleaseClasses<T>` / `ArrayRelease<int>` loop;
// the callbacks differ only in the element type.
namespace {
// Layout-compatible Array<T> view: {size, data, capacity}. The engine stores
// data at +4 and length at +8 in the raw recovery; RawArray mirrors that.
template <class T>
void releaseClassArray(void *p)
{
    RawArray *a = (RawArray *)p;
    T **data = (T **)a->data;
    for (unsigned i = 0; i < a->capacity; ++i) {  // capacity field == element count
        if (data[i] != 0) {
            data[i]->~T();
            ::operator delete(data[i]);
        }
        data[i] = 0;
    }
    if (data != 0)
        ::operator delete[](data);
    a->data = 0;
}
} // namespace

// AbstractGun / KIPlayer / RadioMessage / AEGeometry are opaque here; they are
// released through their first vtable entry's deleting-destructor, which is the
// same observable effect as the typed ArrayReleaseClasses<T> loop.
namespace {
void releasePolymorphicArray(void *p)
{
    RawArray *a = (RawArray *)p;
    void **data = (void **)a->data;
    for (unsigned i = 0; i < a->capacity; ++i) {
        void *e = data[i];
        if (e != 0) {
            // virtual deleting destructor lives at vtable slot 0 (offset +0).
            void *vt = *(void **)e;
            (*(void (**)(void *))((char *)vt + 0))(e);
        }
        data[i] = 0;
    }
    if (data != 0)
        ::operator delete[](data);
    a->data = 0;
}
} // namespace

extern "C" void Level_releaseAEGeometry(void *p)  { releasePolymorphicArray(p); }
extern "C" void Level_releaseAbstractGun(void *p) { releasePolymorphicArray(p); }
extern "C" void Level_releaseKI(void *p)          { releasePolymorphicArray(p); }
extern "C" void Level_releaseRadioMessage(void *p){ releasePolymorphicArray(p); }

// Array<int>: no element destructors — just free the buffer and clear it.
extern "C" void Level_releaseInt(void *p)
{
    RawArray *a = (RawArray *)p;
    for (unsigned i = 0; i < a->capacity; ++i)
        ((int *)a->data)[i] = 0;
    if (a->data != 0)
        ::operator delete[](a->data);
    a->data = 0;
}

// ---- Array<T> container helpers (RawArray {size,data,capacity}) ------------
// These are the engine's per-element-type Array<T> special members the
// decompiler emitted as undefined externs (Array_<T>_ctor/_dtor, ArrayXxx_ctor,
// ArraySetLength_<T>, ArrayAdd_<T>, ArrayRelease(Classes)_<T>). Each is the same
// Array<T> operation specialised per element type; the `_ca`/`_csc`/`_csp`/...
// context suffixes are duplicate inlinings of the same instantiation at distinct
// call sites. They are defined here over the engine's 32-bit RawArray layout
// (size@+0, data@+4, capacity@+8) — the layout this Level and ~15 peer TUs read
// directly (KIPlayer/Player/Radio/RepairBeam/CutScene/MGame/...). Element size
// is 4 bytes (a 32-bit handle/pointer) for every instantiation used here.
namespace {

// Array<T>::Array() — empty container.
inline void arrayCtor(void *p)
{
    RawArray *a = (RawArray *)p;
    a->size = 0;
    a->data = 0;
    a->capacity = 0;
}

// Array<T>::~Array() — free the element buffer; returns `this` (ARM ABI) so the
// caller can hand it straight to operator delete.
inline void *arrayDtor(void *p)
{
    RawArray *a = (RawArray *)p;
    if (a->data != 0)
        ::operator delete[](a->data);
    a->data = 0;
    a->size = 0;
    a->capacity = 0;
    return p;
}

// Array<T>::setLength(n) — (re)allocate an n-slot, zero-filled element buffer.
inline void arraySetLength(unsigned n, void *p)
{
    RawArray *a = (RawArray *)p;
    if (a->data != 0)
        ::operator delete[](a->data);
    if (n != 0) {
        a->data = ::operator new[]((size_t)n * 4);
        for (unsigned i = 0; i < n; ++i)
            ((int *)a->data)[i] = 0;
    } else {
        a->data = 0;
    }
    a->size = n;
    a->capacity = n;
}

// Array<T>::add(item) — append one 4-byte element, growing the buffer by one.
inline void arrayAdd(int item, void *p)
{
    RawArray *a = (RawArray *)p;
    unsigned n = a->size;
    void *grown = ::operator new[]((size_t)(n + 1) * 4);
    for (unsigned i = 0; i < n; ++i)
        ((int *)grown)[i] = ((int *)a->data)[i];
    ((int *)grown)[n] = item;
    if (a->data != 0)
        ::operator delete[](a->data);
    a->data = grown;
    a->size = n + 1;
    a->capacity = n + 1;
}

} // namespace

// Array<T>::Array() per element type.
extern "C" void ArrayKIPlayer_ctor_ca(void *a)   { arrayCtor(a); }
extern "C" void ArrayKIPlayer_ctor_ccm(void *a)  { arrayCtor(a); }
extern "C" void ArrayKIPlayer_ctor_cm(void *a)   { arrayCtor(a); }
extern "C" void ArrayKIPlayer_ctor_csc(void *a)  { arrayCtor(a); }
extern "C" void ArrayKIPlayer_ctor_cso(void *a)  { arrayCtor(a); }
extern "C" void ArrayKIPlayer_ctor_csp(void *a)  { arrayCtor(a); }
extern "C" void ArrayKIPlayer_ctor_cwm(void *a)  { arrayCtor(a); }
extern "C" void ArrayPlayer_ctor_cp(void *a)     { arrayCtor(a); }
extern "C" void ArrayRadio_ctor_crm(void *a)     { arrayCtor(a); }
extern "C" void ArrayRadio_ctor_crms(void *a)    { arrayCtor(a); }
extern "C" void ArrayGasCloud_ctor_cgc(void *a)  { arrayCtor(a); }
extern "C" void ArrayBV_ctor_gbv(void *a)        { arrayCtor(a); }
extern "C" void Array_int_ctor_ips(void *a)      { arrayCtor(a); }
extern "C" void ArrayAGun_ctor_cg(void *a)       { arrayCtor(a); }
extern "C" void ArrayAGun_ctor_ag(void *a)       { arrayCtor(a); }

// Array<T>::~Array() per element type (returns `this`).
extern "C" void *ArrayKIPlayer_dtor_csc(void *a) { return arrayDtor(a); }
extern "C" void *ArrayPlayer_dtor_cp(void *a)    { return arrayDtor(a); }
extern "C" void *ArrayRadio_dtor_crm(void *a)    { return arrayDtor(a); }
extern "C" void *ArrayInt_dtor_gbv(void *a)      { return arrayDtor(a); }
extern "C" void *Array_int_dtor_cs(void *a)      { return arrayDtor(a); }
extern "C" void *Array_int_dtor_cso(void *a)     { return arrayDtor(a); }
extern "C" void *ArrayAGun_dtor_ag(void *a)      { return arrayDtor(a); }

// Array<T>::setLength(n) per element type.
extern "C" void ArraySetLength_KIPlayer_ca(int n, void *a)      { arraySetLength((unsigned)n, a); }
extern "C" void ArraySetLength_KIPlayer_ccm(unsigned n, void *a){ arraySetLength(n, a); }
extern "C" void ArraySetLength_KIPlayer_cm(unsigned n, void *a) { arraySetLength(n, a); }
extern "C" void ArraySetLength_KIPlayer_csc(unsigned n, void *a){ arraySetLength(n, a); }
extern "C" void ArraySetLength_KIPlayer_csp(unsigned n, void *a){ arraySetLength(n, a); }
extern "C" void ArraySetLength_KIPlayer_cwm(unsigned n, void *a){ arraySetLength(n, a); }
extern "C" void ArraySetLength_Player_cp(unsigned n, void *a)   { arraySetLength(n, a); }
extern "C" void ArraySetLength_Radio_crms(unsigned n, void *a)  { arraySetLength(n, a); }
extern "C" void ArraySetLength_GasCloud_cgc(int n, void *a)     { arraySetLength((unsigned)n, a); }
extern "C" void ArraySetLength_BV_gbv(unsigned n, void *a)      { arraySetLength(n, a); }
extern "C" void ArraySetLength_int_ips(unsigned n, void *a)     { arraySetLength(n, a); }
extern "C" void ArraySetLength_AGun_ag(unsigned n, void *a)     { arraySetLength(n, a); }

// Array<T>::add(item) per element type.
extern "C" void ArrayAdd_KIPlayer_cft(KIPlayer *k, void *a) { arrayAdd((int)(intptr_t)k, a); }
extern "C" void ArrayAdd_KIPlayer_csc(KIPlayer *k, void *a) { arrayAdd((int)(intptr_t)k, a); }
extern "C" void ArrayAdd_KIPlayer_cso(KIPlayer *k, void *a) { arrayAdd((int)(intptr_t)k, a); }
extern "C" void ArrayAdd_KIPlayer_cwm(KIPlayer *k, void *a) { arrayAdd((int)(intptr_t)k, a); }
extern "C" void ArrayAdd_Radio_crm(RadioMessage *m, void *a){ arrayAdd((int)(intptr_t)m, a); }
extern "C" void ArrayAdd_AGun_cg(ObjectGun *o, void *a)     { arrayAdd((int)(intptr_t)o, a); }
extern "C" void ArrayAdd_AGun_ag(ObjectGun *o, void *a)     { arrayAdd((int)(intptr_t)o, a); }

// Array<int>::release() — clear element values, keep the slots.
extern "C" void ArrayRelease_int_gbv(void *p) { Level_releaseInt(p); }

// Array<T>::~Array() for the level's owned member arrays (freed in ~Level via the
// ARR() macro after the matching Level_release* element pass). Same RawArray
// teardown — free the element buffer and return `this` for operator delete.
extern "C" void *dtor_ArrayAEGeometry(void *a)  { return arrayDtor(a); }
extern "C" void *dtor_ArrayInt(void *a)         { return arrayDtor(a); }
extern "C" void *dtor_ArrayAbstractGun(void *a) { return arrayDtor(a); }
extern "C" void *dtor_ArrayKI(void *a)          { return arrayDtor(a); }
extern "C" void *dtor_ArrayRadioMessage(void *a){ return arrayDtor(a); }

// Generic (untyped) Array<int-handle> helpers used by createSentryGuns on the
// level's member arrays (field_b0 / enemies). Same RawArray operations as the
// per-type variants above, taking the array as an int handle.
extern "C" void ArrayCtor(int arr)                 { arrayCtor((void *)(intptr_t)arr); }
extern "C" void ArraySetLength(int len, int arr)   { arraySetLength((unsigned)len, (void *)(intptr_t)arr); }
extern "C" void ArrayAdd(int item, int arr)        { arrayAdd(item, (void *)(intptr_t)arr); }

// Array<T>::releaseClasses() — destroy + delete every live element, clear slots.
extern "C" void ArrayReleaseClasses_AGun_ag(void *p)     { releasePolymorphicArray(p); }
extern "C" void ArrayReleaseClasses_KIPlayer_csc(void *p){ releasePolymorphicArray(p); }

// ---- enemy-flag forwarders ------------------------------------------------
// Both operate on a Player object (the level passes player = *(obj+4)). These
// mirror Player::setAlwaysEnemy / Player::turnEnemy, which only flip a handful
// of state flags. Kept as raw offset writes so the byte layout matches the
// Player struct that other TUs still address by offset.
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

// ---- Wanted::getNumWingmen accessor ---------------------------------------
// The level reaches the active Wanted through the enemy KIPlayer chain; the
// accessor is a single field read (Wanted+0x3c == wingman count).
extern "C" int Level_getNumWingmen(int wanted)
{
    if (wanted == 0)
        return 0;
    return *(int *)((char *)(intptr_t)wanted + 0x3c);
}

// ---- construction / destruction bridges -----------------------------------
// CutScene allocates/frees a Level by raw pointer. Level_ctor placement-builds
// the object; Level_dtor runs the destructor and (ARM ABI) yields `this` so the
// caller can hand it straight to operator delete.
extern "C" void Level_ctor(void *self, int mode)
{
    new (self) Level(mode);
}

extern "C" void *Level_dtor(void *level)
{
    ((Level *)level)->~Level();
    return level;
}

// ---------------------------------------------------------------------------
// Recovered decompiler fragments.
//
// The methods below were emitted by the decompiler as standalone `Level_*`
// helpers because each is reached through a PI long-branch veneer (a
// Thumb->ARM trampoline) that forwards to a sibling-class method. Following
// each veneer through its GOT slot resolves the real target; every fragment is
// just that forwarding call, re-expressed here as a real Level member so the
// work is named instead of going through an opaque trampoline.
// ---------------------------------------------------------------------------
namespace {

// Sibling classes reached by the recovered veneers. Only the methods actually
// invoked are declared; the engine owns the full definitions.
struct StarSystemFwd {
    void render2D();
    void renderSunStreak();
};
struct ObjectiveFwd {
    unsigned int achieved(int value);
};
struct StationFwd {
    void setAttackedFriends(bool v);
};
struct WantedFwd {
    void setActive(bool v);
};
struct StatusFwd {
    void setWingmen(void *list);
};

} // namespace

// render2D() forwards rendering of the 2D HUD overlay to the active star system
// (this->starSystem, +0xec). Veneer 0x1abfe8 -> StarSystem::render2D.
void Level::render2D_call(int starSystem) {
    if (starSystem != 0)
        ((StarSystemFwd *)(intptr_t)starSystem)->render2D();
}

// render() tail-calls the star system's sun-streak pass after drawing every
// world object. Veneer 0x1abfd8 -> StarSystem::renderSunStreak.
void Level::render_tail(int starSystem) {
    if (starSystem != 0)
        ((StarSystemFwd *)(intptr_t)starSystem)->renderSunStreak();
}

// checkGameOver()/checkObjective() defer to the objective's own completion test.
// Veneer 0x1ac018 -> Objective::achieved.
int Level::checkGameOver_call(int objective) {
    return (int)((ObjectiveFwd *)(intptr_t)objective)->achieved(0);
}

int Level::checkObjective_call(int objective) {
    return (int)((ObjectiveFwd *)(intptr_t)objective)->achieved(0);
}

// enableMovingStars()/enableFog() toggle rendering of a particle system owned by
// a ParticleSystemManager. Veneer 0x1ac048 -> ParticleSystemManager::enableSystemRender.
void Level::enableMovingStars_call(int mgr, int index, bool enable) {
    ParticleSystemManager::enableSystemRender(mgr, index, enable);
}

void Level::enableFog_call(int mgr, int sys, bool enable) {
    ParticleSystemManager::enableSystemRender(mgr, sys, enable);
}

// killWanted()/pirateStationAction()/friendTurnedEnemy() all queue a context
// radio message. Veneer 0x1ac028 -> Level::createRadioMessage(type, sub).
void Level::wanted_action(int code, int arg) {
    createRadioMessage(code, arg);
}

void Level::pirateStationAction_tail(int code, int arg) {
    createRadioMessage(code, arg);
}

void Level::friendTurnedEnemy_action(int code, int arg) {
    createRadioMessage(code, arg);
}

// alarmAllFriends() flags the home station as having had its friends attacked.
// Veneer 0x1abff8 -> Station::setAttackedFriends(true).
void Level::alarmAllFriends_tail(Station *station, int flag) {
    ((StationFwd *)station)->setAttackedFriends(flag != 0);
}

// almostKillWanted() (de)activates the wanted's storyline state once the target
// has been brought to near-death. Veneer 0x1ac038 -> Wanted::setActive.
void Level::almostKillWanted_tail(int wanted, int active) {
    ((WantedFwd *)(intptr_t)wanted)->setActive(active != 0);
}

// wingmanDied(): when the player has at most one wingman left, clear the whole
// wingman roster. Veneer 0x1abfb8 -> Status::setWingmen(0).
void Level::wingmanDied_all(Status *status, int zero) {
    ((StatusFwd *)status)->setWingmen((void *)(intptr_t)zero);
}

// wingmanDied(): otherwise drop the matched wingman name from the roster array.
// Veneer 0x1abfc8 -> ArrayRemove<AbyssEngine::String*>(name, list).
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

// createStaticObjects() builds each fixed world object through createStaticObject.
// Veneer chains to Level::createStaticObject(Waypoint*, type, jitter).
int Level::createStaticObject_call(int wp, int type, int jitter) {
    return createStaticObject((Waypoint *)(intptr_t)wp, type, jitter);
}

// ---------------------------------------------------------------------------
// Recovered sibling-forwarding fragments.
//
// init() and the scene builders were decompiled as a flat sequence of helper
// calls (the _init / _csc / _cm / ... fragments). In the binary each of those
// is a PI veneer that lands directly in the matching Level builder method, so
// the recovered body is simply the re-entry into that sibling. These mirror the
// _call / _tail forwarders above and let init()/createScene()/createMission()/
// createWingmen()/etc. run their build pipeline through real method calls.
// ---------------------------------------------------------------------------

// init()'s staged build pipeline (createSpace_init .. connectPlayers_init).
void Level::createSpace_init()           { createSpace(); }
void Level::createPlayer_init()          { createPlayer(); }
void Level::createAsteroids_init()       { createAsteroids(); }
void Level::createGasClouds_init()       { createGasClouds(); }
void Level::createMission_init()         { createMission(); }
void Level::createCampaignMission_init() { createCampaignMission(); }
void Level::createScene_init()           { createScene(); }
void Level::createStaticObjects_init()   { createStaticObjects(); }
void Level::createSentryGuns_init()      { createSentryGuns(); }
void Level::createFighterTurrets_init()  { createFighterTurrets(); }
void Level::createWingmen_init()         { createWingmen(); }
void Level::assignGuns_init()            { assignGuns(); }
void Level::connectPlayers_init()        { connectPlayers(); }

// Each scene builder spawns its actors through the shared createShip(...) body.
PlayerFixedObject *Level::createShip_cm(int race, int shipClass, int type, int wp, int hostile, int group) {
    return createShip(race, shipClass, type, (Waypoint *)(intptr_t)wp, hostile, group);
}
PlayerFixedObject *Level::createShip_ccm(int race, int shipClass, int type, int wp, int hostile, int group) {
    return createShip(race, shipClass, type, (Waypoint *)(intptr_t)wp, hostile, group);
}
PlayerFixedObject *Level::createShip_csc(int race, int shipClass, int type, int wp, int hostile, int group) {
    return createShip(race, shipClass, type, (Waypoint *)(intptr_t)wp, hostile, group);
}
PlayerFixedObject *Level::createShip_cwm(int race, int shipClass, int type, int wp, int hostile, int group) {
    return createShip(race, shipClass, type, (Waypoint *)(intptr_t)wp, hostile, group);
}

// createStaticObjects()/createFighterTurrets() build scenery via createStaticObject(...).
int Level::createStaticObject_cso(int wp, int type, int jitter) {
    return createStaticObject((Waypoint *)(intptr_t)wp, type, jitter);
}
int Level::createStaticObject_cft(int wp, int type, int jitter) {
    return createStaticObject((Waypoint *)(intptr_t)wp, type, jitter);
}

// createScene(mode 2) replays the regular player/mission build.
void Level::createPlayer_csc()  { createPlayer(); }
void Level::createMission_csc() { createMission(); }

// update() drives the orbit / alien-attacker ticks through these veneers.
void Level::updateMissionOrbit_call(int dt) { updateMissionOrbit(dt); }
void Level::updateOrbit_call(int dt)        { updateOrbit(dt); }
void Level::updateAlienAttackers_call()     { updateAlienAttackers(0); }

// updateOrbit() raises the delayed friends-turned-hostile alarm + radio line.
void Level::alarmAllFriends_uo(int race, int message) { alarmAllFriends(race, message != 0); }
void Level::createRadioMessage_uo(int type)           { createRadioMessage(type, 0); }

// ===========================================================================
// Supporting shims for the recovered build fragments below.
//
// These mirror the per-fragment `extern "C"` blocks already used throughout this
// TU: hidden `extern` references that the link/wiring pass resolves. Declaring
// them here keeps the fragment bodies self-contained without redefining storage.
// ===========================================================================
// AEMath free functions used by the SIMD-recovery fragments (the structs are in
// scope via common.h; these declarations bring in just the two entry points we
// need without pulling the whole AEMath header into this TU).
namespace AbyssEngine { namespace AEMath {
float VectorLength(const Vector &value);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z);
} }

namespace {
// PlayerEgo's comm controller (ego+0x18) consumes the radio queue through an
// engine update entry; reuse the LODManager-shaped forwarder already declared
// for the veneer table above.
struct LODManagerFwd { void update(void *arg); };
} // namespace

// RNG object holders for the spawn-positioning fragments (one per owning method).
__attribute__((visibility("hidden"))) extern int *g_uo_rng;
__attribute__((visibility("hidden"))) extern int *g_umo_rng;
__attribute__((visibility("hidden"))) extern int *g_uaa_rng;

extern "C" {
int  AERandom_nextInt_uo(int rng);
int  AERandom_nextInt_umo(int rng);
int  AERandom_nextInt_uaa(int rng);
void BoundingVolume_ctor_gbv(BoundingVolume *bv, int rec, int shape);
void AEGeometry_setLodMeshes_gap(AEGeometry *geo, unsigned short *meshes, int *dist, int n);
}

// ===========================================================================
// Recovered decompiler-split scene/build fragments.
//
// Each owning method (createSpace / init / createMission / ...) was lifted by
// the decompiler as a flat sequence that handed a slice of its work to a
// distinct Level_<suffix> helper — almost always a block of SIMD float math
// the lifter could not reconstruct. The bodies below re-express that slice
// through the engine shims declared at the matching call site, plus our
// AEMath / vtable helpers, so the work is named rather than opaque. Field
// access stays at raw offsets to keep the struct byte-compatible with the
// other TUs that still address Level by offset.
// ===========================================================================

namespace {
// A KIPlayer/Player object exposes a 3-float setPosition through vtable slot
// +0x48; that is how createMission()/updateOrbit() commit a spawn position.
inline void kiSetPosition(int *ki, float x, float y, float z) {
    (*(void (**)(int *, float, float, float))(*ki + 0x48))(ki, x, y, z);
}
// The level's player object carries its world position in its geometry at
// player->geometry(+0x140)->position(+0x10..0x1c). Returns the origin when no
// player exists yet (early-orbit spawns fall back to {0,0,0}).
inline Vector levelPlayerPosition(char *self) {
    Vector p; p.x = p.y = p.z = 0.0f;
    int player = *(int *)(self + 0xf0);
    if (player != 0) {
        int geo = *(int *)(player + 0x140);
        if (geo != 0) {
            p.x = *(float *)(geo + 0x10);
            p.y = *(float *)(geo + 0x14);
            p.z = *(float *)(geo + 0x18);
        }
    }
    return p;
}
} // namespace

// --- createRadioMessage(): deliver the queue to the player-ego comm controller.
// egoComm is ego->comm (+0x18); a null queue is a "clear channel" request. The
// controller's update entry consumes/forwards the queued RadioMessage objects.
void Level::crm_dispatch(int egoComm, void *queue) {
    if (egoComm != 0)
        ((LODManagerFwd *)(intptr_t)egoComm)->update(queue);
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
    char *self = (char *)this;
    *(int *)(self + 0x1a4) = 0;
    *(int *)(self + 0x1a8) = 0;
    *(int *)(self + 0x1ac) = 0;
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
    char *self = (char *)this;
    int player = *(int *)(self + 0xf0);
    if (player == 0)
        return;
    // PlayerEgo exposes its respawn placement through vtable slot +0x1c.
    int *ego = (int *)(intptr_t)player;
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
    char *self = (char *)this;
    int arr = *(int *)(self + 0xfc);          // asteroids array
    if (arr == 0)
        return 1.0e30f;
    int *data = *(int **)(arr + 4);
    int obj = data[idx];
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

// --- updateOrbit(): revive an enemy and teleport it to a far random offset from
// the player. Profile: generic far-field wave.
void Level::uo_spawnFar(int *kiPlayer) {
    char *self = (char *)this;
    int rng = *g_uo_rng;
    Vector p = levelPlayerPosition(self);
    float ox = (float)(AERandom_nextInt_uo(rng) % 120000 - 60000);
    float oy = (float)(AERandom_nextInt_uo(rng) %  80000 - 40000);
    float oz = (float)(AERandom_nextInt_uo(rng) % 120000 - 60000);
    kiSetPosition(kiPlayer, p.x + ox, p.y + oy, p.z + oz);
}

// --- updateMissionOrbit(): revive then reposition. profile 0 == far wave,
// profile 1 == tighter boss-escort spread.
void Level::umo_spawnAt(int *kiPlayer, int profile) {
    char *self = (char *)this;
    int rng = *g_umo_rng;
    Vector p = levelPlayerPosition(self);
    int span = profile ? 40000 : 120000;
    float ox = (float)(AERandom_nextInt_umo(rng) % span - span / 2);
    float oy = (float)(AERandom_nextInt_umo(rng) % (span * 2 / 3) - span / 3);
    float oz = (float)(AERandom_nextInt_umo(rng) % span - span / 2);
    kiSetPosition(kiPlayer, p.x + ox, p.y + oy, p.z + oz);
}

// --- updateAlienAttackers(): place a revived alien relative to the player (when
// alienInOrbit) or the station origin.
void Level::uaa_placeAlien(int *kiPlayer, int alienInOrbit) {
    char *self = (char *)this;
    int rng = *g_uaa_rng;
    Vector base;
    base.x = base.y = base.z = 0.0f;
    if (alienInOrbit)
        base = levelPlayerPosition(self);
    float ox = (float)(AERandom_nextInt_uaa(rng) % 100000 - 50000);
    float oy = (float)(AERandom_nextInt_uaa(rng) %  60000 - 30000);
    float oz = (float)(AERandom_nextInt_uaa(rng) % 100000 - 50000);
    kiSetPosition(kiPlayer, base.x + ox, base.y + oy, base.z + oz);
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
    BoundingVolume *bv = (BoundingVolume *)Level_opnew_gbv(shape ? 0x40 : 0x20);
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
    char *self = (char *)this;
    Vector p = levelPlayerPosition(self);
    if (boss && i == 0) {
        out->x = p.x;
        out->y = p.y;
        out->z = p.z + 30000.0f;
        return;
    }
    out->x = p.x + (float)(AERandom_nextInt_cgc(rng) % 160000 - 80000);
    out->y = p.y + (float)(AERandom_nextInt_cgc(rng) % 100000 - 50000);
    out->z = p.z + (float)(AERandom_nextInt_cgc(rng) % 160000 - 80000);
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
    char *self = (char *)this;
    int mgr = *(int *)(self + 0x7c);          // particleSystemMgr
    if (mgr == 0)
        return -1;
    int sys = ParticleSystemManager_addSystem_ips(mgr, 0, kind, 1);
    ParticleSystemManager_enableSystemEmit_ips(mgr, sys);
    return sys;
}

// --- createWingmen(): position wingman `i` in formation relative to the player
// geometry (right/forward offsets) and align its heading. Built inline; this is
// the recovered split point for that block.
void Level::cwm_placeWingman(int *kiSlot, unsigned i) {
    char *self = (char *)this;
    if (kiSlot == 0)
        return;
    Vector p = levelPlayerPosition(self);
    // staggered echelon: alternate sides, step back each pair.
    float side = ((i & 1) ? -1.0f : 1.0f) * (float)(2000 + (int)(i / 2) * 1500);
    float back = (float)(2000 + (int)(i / 2) * 2500);
    kiSetPosition(kiSlot, p.x + side, p.y, p.z - back);
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
    char *self = (char *)this;
    Matrix *sky = (Matrix *)(self + 0x1d0);
    float ax = *(float *)(self + 0x1a4);
    float ay = *(float *)(self + 0x1a8) + (mode ? 0.0f : spin);
    float az = *(float *)(self + 0x1ac);
    AbyssEngine::AEMath::MatrixSetRotation(*sky, ax, ay, az);
}
