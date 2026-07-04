#include "game/world/Level.h"

float Level::r;
float Level::g;
float Level::b;

#include "game/mission/Mission.h"
#include "game/ship/Ship.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
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
#include "game/mission/Generator.h"
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

#include "game/ship/PlayerFixedObject.h"
#include "game/ship/PlayerStation.h"
#include <new>
#include "game/ship/Agent.h"
#include "engine/render/Engine.h"
#include "game/weapons/Radar.h"
#include "game/weapons/BeamGun.h"
#include "game/weapons/BombGun.h"
#include "game/weapons/MineGun.h"
#include "game/weapons/SentryGun.h"
#include "engine/math/Transform.h"
#include "engine/render/Mesh.h"
#include "game/core/GameSettings.h"
#include "game/mission/DifficultyRecord.h"
#include <cstdint>
#include <cstddef>

namespace {
    struct EngineColorEntry {
        uint32_t color;
        uint8_t pad[0x9c];
    };

    struct EngineColorTable {
        uint8_t field_0x0[0x1254];
        EngineColorEntry entries[1];
    };

    static_assert(sizeof(EngineColorEntry) == 0xa0,
                  "EngineColorEntry stride must be 0xa0");
    static_assert(offsetof(struct EngineColorTable, entries) == 0x1254,
                  "entries must live at table offset 0x1254");
}

Matrix *CameraGetLocal(void *canvas, uint32_t index);

// lint: void_ptr (free-function signature; retype changes mangling)

static unsigned char *g_initStreamOut = nullptr;

static int *g_engineColorBase = nullptr;

static const int g_cg_meshTable[233] = {6754, 6755, 6756, 6760, 6761, 6762, 6763, 6764, 6765, -1, -1, -1, 6788, 6789, 6790, 6791, 6792, 6793, 6794, 6795, 6796, 6797, 6798, 6799, 6800, 6801, 6802, 6803, 14236, 14237, 14238, 14247, 14247, 14247, 14247, 14247, 14249, 14249, 14249, 14249, 14249, 14684, 14684, 14684, 14680, 14680, 14682, 6792, 6797, 6789, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14050, 14052, 14054, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6900, 6901, 6902, 14293, 6905, 6906, 6799, 14297, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14239, 14235, -1, -1, 14247, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6788, 6788, 6788, 14247, 14247, 14247, -1, -1, -1, -1, 14247, -1, -1, 6796, -1, -1, -1, -1, 19091, 6803, 19094, 27338};
static const int g_cg_soundTable[233] = {52, 53, 54, 56, 55, 57, 58, 59, 60, 50, 49, 50, 78, 75, 79, 77, 72, 73, 74, 76, 80, 81, 66, 67, 63, 64, 68, 65, 69, 70, 71, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 6, 7, 8, 9, 10, 11, 92, 93, 94, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1098, 1100, 1099, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1111, 1113, 1112, 1117, 1095, 1096, 1097, 1101, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1118, -1, -1, -1, 2254, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2271, 2272, -1, -1, 2263, 2263, 2263, 2265, 2265, 2265, -1, -1, -1, -1, 2254, 2267, 2268, 2252, -1, 2269, -1, 2275, 2276, 2278, 2277, 2280};

static int g_cg_rocketFx = 0;

static int g_cg_objFx = 0x3E8;







static int g_cg_mineFx = 0;













static int *g_crm_counts8 = nullptr;

static int *g_crm_table8 = nullptr;

static PaintCanvas ***g_init_canvas = nullptr;

static char **g_init_flagStack = nullptr;

static int **g_init_missionDef = nullptr;

static int **g_init_settings = nullptr;

static char **g_init_bmFlag = nullptr;

static char **g_ca_lowDetail = nullptr;

static float g_ccm_pos0 = 50000.0f;

static float g_up_eqMax = 0;

static float *g_up_clampLo = nullptr;

static float *g_up_clampHi = nullptr;

static float *g_up_clampZ = nullptr;

static float *g_up_clampW = nullptr;

static float *g_flash2_a = nullptr;

static float *g_flash2_b = nullptr;

static float *g_flash2_c = nullptr;

static float *g_flashCol_r = nullptr;

static float *g_flashCol_g = nullptr;

static float *g_flashCol_b = nullptr;

static void (**g_levelSubCtor)(Matrix *) = nullptr;

static int **g_cso_textA = nullptr;

static int **g_cso_textB = nullptr;

static float g_cso_posX = 0;

static float g_cso_posZ = 0;

static int *g_cs_diffRec = nullptr;

static int *g_ag_diffRec = nullptr;

static int **g_ag_shipTbl = nullptr;

static int **g_ag_itemTblA = nullptr;

static int *g_ag_weaponDmg = nullptr;

static int **g_ag_statusB = nullptr;

static int **g_ag_alienCnt = nullptr;

static int **g_ag_itemTblB = nullptr;

static float g_ag_perLevel = 0;

static int *g_ips_envA = nullptr;

static int *g_ips_envB = nullptr;

static int **g_cwm_seedSrc = nullptr;

static unsigned int g_level_texOutScratch;

static inline float bitsToFloat(int bits) {
    float f;
    __builtin_memcpy(&f, &bits, sizeof(f));
    return f;
}

struct PlayerEgoPolymorphic;

struct PlayerEgoVtable {
    void (*slot[7])();

    void (*placeAtStation)(PlayerEgoPolymorphic *self, int stationStack);
};

struct PlayerEgoPolymorphic {
    PlayerEgoVtable *vtable;
};

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(PlayerEgoVtable, placeAtStation) == 0x1c,
              "PlayerEgo vtable placeAtStation slot offset");
#endif

static inline void levelSpawnFar(Level *self, int *kiPlayer);

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
    starSystem->render2D();
}

int Level::checkGameOver(int param) {
    Objective *objective = objectivesB;
    if (objective == nullptr) {
        return 0;
    }

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
    Globals::status->field_b0 += 1;
    enemiesLeft -= 1;
}

void Level::enableMovingStars(bool enable) {
    int index = movingStarsIndex;
    if (index < 0) {
        return;
    }

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

    particleSystemMgr->enableSystemRender(field_284, render);
    *(unsigned char *) particleRenderBoolPtr = render;
    *(unsigned char *) particleEmitBoolPtr = render;
}

void Level::switchSkyboxForIntro() {
    unsigned int skyboxMeshHandle;
    Globals::Canvas->MeshCreate((unsigned short) (0x4591), skyboxMeshHandle, false);
    skyboxMesh = skyboxMeshHandle;
    unsigned int skyboxTexHandle;
    Globals::Canvas->TextureCreate((unsigned short) (0x275a), nullptr, nullptr, skyboxTexHandle, false);
    field_198 = skyboxTexHandle;
    if (this->asteroids != nullptr) {
        for (unsigned int i = 0; i < this->asteroids->size(); i = i + 1) {
            (*this->asteroids)[i]->setActive(false);
        }
    }
}

void Level::switchSkyboxForSupernovaReversal() {
    int tex = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex();
    unsigned int skyboxMeshHandle;
    Globals::Canvas->MeshCreate((unsigned short) ((unsigned short) (tex + 0x4588)), skyboxMeshHandle, false);
    skyboxMesh = skyboxMeshHandle;
    int tex2 = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex();
    unsigned int skyboxTexHandle;
    Globals::Canvas->TextureCreate((unsigned short) ((unsigned short) (tex2 + 0x2751)), nullptr, nullptr,
                                   skyboxTexHandle,
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

        createRadioMessage(0x11, 0);
    }
}

Array<AbstractGun *> *Level::getEnemyGuns() {
    return enemyGuns;
}

void Level::stealFriendCargo() {
    friendCargoStolen = 1;
}

Array<KIPlayer *> *Level::getEnemies() {
    return enemies;
}

void Level::applyKills() {
    if (Globals::status->getMission() != 0) {
        Globals::status->addKills(kills);
        kills = 0;
    }
}

uint8_t Level::friendCargoWasStolen() {
    return friendCargoStolen;
}

Array<void *> *Level::getMessages() {
    // lint: void_ptr (exported signature; cross-file use in LevelScript.cpp)
    return (Array<void *> *) messages; // lint: void_ptr (exported return type)
}

Route *Level::getEnemyRoute() {
    return enemyRoute;
}

Array<AbstractGun *> *Level::getPlayerGuns() {
    return playerGuns;
}

void Level::renderPause(long long /*ctx*/) {
    if (this->playerGuns != nullptr) {
        for (unsigned int i = 0; i < this->playerGuns->size(); i = i + 1) {
            ((ObjectGun *) (*this->playerGuns)[i])->render();
        }
    }
    if (this->enemyGuns != nullptr) {
        for (unsigned int i = 0; i < this->enemyGuns->size(); i = i + 1) {
            ((ObjectGun *) (*this->enemyGuns)[i])->render();
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
        if (Globals::status->getStation()->getPirateStationIndex() < 0) {
            return;
        }
        int tbl = (int) (intptr_t) Globals::status->field_4c;
        int idx = Globals::status->getStation()->getPirateStationIndex();
        *(unsigned char *) (*(int *) (tbl + 4) + idx) = 1;

        *((unsigned char *) &Globals::status->field_f8 + 1) = 1;
    }

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
            ((ObjectGun *) (*this->playerGuns)[i])->render();
        }
    }
    if (this->enemyGuns != nullptr) {
        for (unsigned int i = 0; i < this->enemyGuns->size(); i = i + 1) {
            ((ObjectGun *) (*this->enemyGuns)[i])->render();
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
        ArrayReleaseClasses(*field_a4);
        delete field_a4;
        field_a4 = nullptr;
    }
    if (field_a8) {
        delete field_a8;
        field_a8 = nullptr;
    }
    if (playerGuns) {
        ArrayReleaseClasses(*playerGuns);
        delete playerGuns;
        playerGuns = nullptr;
    }
    if (enemyGuns) {
        ArrayReleaseClasses(*enemyGuns);
        delete enemyGuns;
        enemyGuns = nullptr;
    }
    if (enemies) {
        ArrayReleaseClasses(*enemies);
        delete enemies;
        enemies = nullptr;
    }
    if (asteroids) {
        ArrayReleaseClasses(*asteroids);
        delete asteroids;
        asteroids = nullptr;
    }
    if (gasClouds) {
        ArrayReleaseClasses(*gasClouds);
        delete gasClouds;
        gasClouds = nullptr;
    }
    if (landmarks) {
        ArrayReleaseClasses(*landmarks);
        delete landmarks;
        landmarks = nullptr;
    }
    if (messages) {
        ArrayReleaseClasses(*messages);
        delete messages;
        messages = nullptr;
    }
    if (field_104) {
        ArrayReleaseClasses(*field_104);
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
    }
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
        int sign = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
        p[-1] = (Globals::rnd->nextInt(30000) + 50000) * sign;
        p[0] = Globals::rnd->nextInt(20000) - 10000;
        if (i == -1) {
            pts[2] = Globals::rnd->nextInt() + 50000;
        } else {
            int prev = p[-2];
            p[1] = Globals::rnd->nextInt() + prev + 50000;
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

    if (this->alarmRequested == 0 && message) {
        int type = 1;
        this->alarmRequested = (unsigned char) type;
        if (Globals::status->inBlackMarketSystem() != 0) {
            type = 0xc;
        }
        createRadioMessage(type, race);
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == race) {
            Globals::status->getStation()->setAttackedFriends(true);
        }
    }
}

void Level::setPlayerEngineColor(short color) {
    int c = color;
    if (player != nullptr && field_a4 != nullptr) {
        EngineColorTable *table = (EngineColorTable *) g_engineColorBase;
        int count = (int) field_a4->size();
        for (int i = 0; i < count; i = i + 1) {
            table->entries[i].color = c << 0x10 | c << 0x18 | c << 8 | 0xff;
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
            int res = g_cg_meshTable[idx];
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
            new(obj) ObjectGun(owner, gun, g_cg_meshTable[idx], 1000, this);
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
            new(obj) RocketGun(owner, gun, g_cg_meshTable[idx], 0, 0, kind,
                               (kind == ITEM_SORT_CLUSTER_MISSILE || kind == ITEM_SORT_MISSILE) ? 1 : 0, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
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
            int attr = ((Item *) (intptr_t)(*(int *) (*(int *) ((int) (intptr_t) Globals::items + 4) + idx * 4)))->getAttribute(0xf);
            obj = (ObjectGun *) ::operator new(300);
            new(obj) BombGun(gun, g_cg_meshTable[idx], 1, kind, attr == 1 ? 1 : 0,
                             this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
        }
        case ITEM_SORT_TURRET:
        case ITEM_SORT_PLASMA_COLLECTOR: {
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
                reinterpret_cast<int &>(gun->field_0xa4_b0) = 1;
                if (idx == 0xe0) gun->field_0xa5 = 1;
            }
            obj = (ObjectGun *) ::operator new(0xb0);
            const int *tbl = (kind == ITEM_SORT_PLASMA_COLLECTOR) ? g_cg_meshTable : g_cg_meshTable;
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
            new(obj) MineGun(gun, g_cg_meshTable[idx], 1, ITEM_SORT_MINE, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
        }
        case ITEM_SORT_SENTRY_GUN: {
            gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(owner, 0, 3, hp, cool, rate, 2.0f, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = ITEM_SORT_SENTRY_GUN;
            gun->setPlayerGun(1);
            obj = (ObjectGun *) ::operator new(0xb4);
            new(obj) SentryGun(gun, g_cg_meshTable[idx], 1, ITEM_SORT_SENTRY_GUN, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
        }
        case ITEM_SORT_SHOCK_BLAST: {
            gun = (Gun *) ::operator new(0x114);
            new(gun) Gun(owner, dmg, 1, hp, 1, rate, (float) color, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = ITEM_SORT_SHOCK_BLAST;
            gun->setPlayerGun(1);
            obj = (ObjectGun *) ::operator new(300);
            new(obj) BombGun(gun, g_cg_meshTable[idx], 1, ITEM_SORT_SHOCK_BLAST, 0, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
        }
        default:
            gun = 0;
            break;
    }

    switch (idx) {
        case 0x29: Globals::globals->addSoundResourceToList(0xf);
        case 0x2a: Globals::globals->addSoundResourceToList(0x10);
        case 0x2b: Globals::globals->addSoundResourceToList(0x11);
        case 0x2c: Globals::globals->addSoundResourceToList(0xe);
        case 0x2d: Globals::globals->addSoundResourceToList(0xd);
        case 0x2e: Globals::globals->addSoundResourceToList(0xc);
        default: break;
    }

    gun->setLevel(this);
    if (this->playerGuns == nullptr) {
        this->playerGuns = new Array<AbstractGun *>();
    }
    ArrayAdd((AbstractGun *) obj, *(this->playerGuns));
    return gun;
}

void Level::createSpace() {
    if (*(unsigned *) &this->skyboxMesh == 0xffffffff) {
        int alien = Globals::status->inAlienOrbit();

        if (alien == 0) {
            Globals::status->getSystem();
            int sysVariant = (((SolarSystem *) Globals::status->getSystem())->getIndex() % 3);
            unsigned int field08Handle;
            Globals::Canvas->MeshCreate((unsigned short) (sysVariant + 0x45ba), field08Handle, false);
            this->field_08 = field08Handle;
            Globals::status->getSystem();
            sysVariant = (((SolarSystem *) Globals::status->getSystem())->getIndex() % 3);
            Globals::Canvas->TextureCreate((unsigned short) ((sysVariant + 0x2766) & 0xffff), nullptr, nullptr,
                                           g_level_texOutScratch, false);

            Globals::status->getSystem();
            if (0xf < ((SolarSystem *) Globals::status->getSystem())->getTextureIndex()) {
                Engine *eng = (Engine *) Globals::appManager->GetEngine();
                if (eng->IsPostEffectActivated() != false) {
                    AbyssEngine::Mesh *mp = Globals::Canvas->MeshGetPointer(
                        (unsigned int) *(unsigned *) &this->skyboxMesh);
                    mp->materialId = 0;
                }
            }
        } else {
            unsigned int field08Handle;
            Globals::Canvas->MeshCreate((unsigned short) 0x45bc, field08Handle, false);
            this->field_08 = field08Handle;
            Globals::Canvas->TextureCreate((unsigned short) 0x2768, nullptr, nullptr, g_level_texOutScratch, false);
            unsigned int skyboxMeshHandle;
            Globals::Canvas->MeshCreate((unsigned short) 0x4592, skyboxMeshHandle, false);
            this->skyboxMesh = skyboxMeshHandle;
            Globals::Canvas->TextureCreate((unsigned short) 0x275b, nullptr, nullptr, g_level_texOutScratch, false);
        }

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

    this->landmarks = new Array<KIPlayer *>();
    ArraySetLength(4, *(this->landmarks));
}

void Level::createRadioMessage(int type, int sub) {
    unsigned r2 = (unsigned) sub;

    if (this->player == nullptr || this->player->field_0x1c == 0)
        return;

    Globals::status->getMission();
    if (((Mission *) Globals::status->getMission())->isEmpty() == 0)
        return;

    if (this->messages != nullptr) {
        if (this->messages) {
            ArrayReleaseClasses(*this->messages);
            delete this->messages;
            this->messages = nullptr;
        }
    }
    this->messages = new Array<RadioMessage *>();

    int speaker;
    if (r2 == 0) speaker = 0x40;
    else if (r2 == 2) speaker = 0x41;
    else if (r2 == 3) speaker = 0x15;
    else speaker = (r2 == 8) ? 9 : 0x3f;

    int extraDelay = 0;
    unsigned id = 0x1ba;
    bool aborted = false;
    bool builtInline = false;

    switch (type) {
        case 0:
        case 1: {
            int *stations = (int *) Globals::status->field_90;
            bool atStation = false;
            if (stations != 0) {
                for (unsigned k = 0; k < (unsigned) *stations; k = k + 1) {
                    int sidx = *(int *) (((int *) stations[1])[k]);
                    Station *st = (Station *) Globals::status->getStation();
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
            id = Globals::rnd->nextInt() + base;
            break;
        }
        case 3:
            this->field_1b0 = 1;
            id = Globals::rnd->nextInt() + 0x1b3;
            break;
        case 4:
            this->field_68 = 1;
            id = Globals::rnd->nextInt() + 0x1b6;
            break;
        case 5: id = 0x1bb;
            extraDelay = 0;
            break;
        case 6: id = 0x1bc;
            extraDelay = 0;
            break;
        case 9: id = Globals::rnd->nextInt() + 0x1c1;
            break;
        case 10: id = Globals::rnd->nextInt() + 0x1c3;
            break;
        case 0xb: id = Globals::rnd->nextInt() + 0x1c5;
            break;
        case 0xc: id = 0x1c5;
            extraDelay = 0;
            break;
        case 0xd: id = Globals::rnd->nextInt() + 0x1c7;
            break;
        case 0xe: {
            int *st = (int *) Globals::status->field_90;
            id = 0x88f;
            if (st != 0) {
                for (int k = 0; k != *st; k = k + 1)
                    if (-1 < *(int *) (st[1] + k * 4)) id = id - 2;
            }
            break;
        }
        case 0xf: {
            int *st = (int *) Globals::status->field_90;
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
            int stage = Globals::rnd->nextInt();
            int off = 0;
            for (int k = 0; k < stage; k = k + 1)
                off = off + g_crm_counts8[k];
            int cnt = g_crm_counts8[stage];
            RadioStageEntry *tbl = (RadioStageEntry *) g_crm_table8 + off;
            for (int k = 0; k < cnt; k = k + 1) {
                int delay = (k == 0) ? 5000 : (k - 1);
                int arg = tbl[k].radioStageEntry;
                RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                int kind = (k == 0) ? 5 : 6;
                new(m) RadioMessage(tbl[k].textID, arg, kind, delay);
                ArrayAdd(m, *(this->messages));
            }
            builtInline = true;
            break;
        }
        case 0x13: {
            RadioMessage *m = (RadioMessage *) ::operator new(0x28);
            AbyssEngine::AERandom *rng = Globals::rnd;
            new(m) RadioMessage(rng->nextInt() + 0xaf4, 0, 5, 0x5dc);
            ArrayAdd(m, *(this->messages));
            m = (RadioMessage *) ::operator new(0x28);
            new(m) RadioMessage(rng->nextInt() + 0xafa, 0, 6, 0);
            ArrayAdd(m, *(this->messages));
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
            ArrayAdd(m, *(this->messages));
            m = (RadioMessage *) ::operator new(0x28);
            new(m) RadioMessage(r2 * 2 + 0xc61, 0, 6, 0);
            ArrayAdd(m, *(this->messages));
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
            } else {
                id = Globals::rnd->nextInt() + 0x1bd;
            }
            break;
    }

    if (aborted) {
        if (this->messages != nullptr)
            if (this->messages) {
                ArrayReleaseClasses(*this->messages);
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
        ArrayAdd(m, *(this->messages));
    }

    PlayerEgo *ego = this->player;
    this->crm_dispatch(ego->field_0x1c, this->messages);
}

int Level::init() {
    Level *thisptr = this;

    int stage = this->field_134;
    if (stage == 0) {
        this->field_68 = 0;
        this->field_1b0 = 0;
        this->alarmRequested = 0;
        this->field_18a = 0;
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
        if (Globals::status->inAlienOrbit() == 0) {
            Globals::status->getSystem();
            dustVariant = ((SolarSystem *) Globals::status->getSystem())->getTextureIndex() == 0xc;
        }

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
                stk = (int) (intptr_t) Globals::status->getStationStack();
            if (thisptr->player != nullptr) {
                PlayerEgoPolymorphic *ego = (PlayerEgoPolymorphic *) thisptr->player;
                ego->vtable->placeAtStation(ego, stk);
            }
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

    Mission *m = (Mission *) Globals::status->getMission();
    if (m == 0)
        Globals::status->setMission((Mission *) **g_init_missionDef);

    int mode = this->missionPtr;
    if (mode == 3) {
        bool campaign = ((Mission *) Globals::status->getMission())->isEmpty() == 0 && m->isCampaignMission() != 0;
        bool madeScene = false;
        if (campaign) {
            if (this->missionPtr != 3) { madeScene = true; } else {
                int won = Globals::status->gameWon();
                GameSettings *settings = (GameSettings *) *g_init_settings;
                bool skip = won != 0 && settings->settingSkipIntroFlag == 0 &&
                            settings->blackMarketUnlockedFlag == 0;
                if (skip) {
                    thisptr->createMission();
                    if (**g_init_bmFlag != 0 && Globals::status->inBlackMarketSystem() != 0) {
                        if (thisptr->player != nullptr) {
                            PlayerEgoPolymorphic *ego = (PlayerEgoPolymorphic *) thisptr->player;
                            ego->vtable->placeAtStation(ego, 0);
                        }
                    }
                } else if (this->missionPtr != 3) {
                    madeScene = true;
                } else {
                    Globals::status->getMission();
                    if (((Mission *) Globals::status->getMission())->isEmpty() == 0) {
                        Mission *mm = (Mission *) Globals::status->getMission();
                        if (mm->isCampaignMission() != 0)
                            thisptr->createCampaignMission();
                    }
                }
            }
        } else {
            thisptr->createMission();
            if (**g_init_bmFlag != 0 && Globals::status->inBlackMarketSystem() != 0) {
                if (thisptr->player != nullptr) {
                    PlayerEgoPolymorphic *ego = (PlayerEgoPolymorphic *) thisptr->player;
                    ego->vtable->placeAtStation(ego, 0);
                }
            }
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
        (mode != 2 || Globals::status->getCurrentCampaignMission() != 0x2b)) {
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

    int initial = 0;
    int enemies = 0;
    if (this->enemies != nullptr) {
        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            KIPlayer *e = (*this->enemies)[i];
            if (e->deadFlag == 0 && e->lostMissionCrateToEgo == 0 && e->field_0x40 == 0) {
                int wm = e->isWingMan();
                if (wm == 0) {
                    if ((char) e->field_0x44 == 0 && e->stealFlagByte == 0)
                        enemies = enemies + (e->countsAsEnemyExcludeFlag ^ 1);
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
    this->field_188 = 0;
    this->field_1c = 0;
    this->asteroidsLeft = initial;
    this->kills = 0;
    this->friendCargoStolen = 0;
    this->field_70 = 1;
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
                reinterpret_cast<uint8_t &>(t->route) = 1;
                ArrayAdd((KIPlayer *) t, *(this->enemies));
            }
        }
    }
}

void Level::updateAlienAttackers(int dt) {
    Level *thisptr = this;
    this->alienAttackTimer = this->alienAttackTimer + dt;

    Mission *m = (Mission *) Globals::status->getMission();
    bool blocked = (m != 0) && m->isCampaignMission() != 0 &&
                   (Globals::status->getCurrentCampaignMission() == 0x28 ||
                    Globals::status->getCurrentCampaignMission() == 0x93 ||
                    Globals::status->getCurrentCampaignMission() == 0x9a);
    if (blocked)
        return;

    int elapsed = this->alienAttackTimer;
    int period = (Globals::status->getCurrentCampaignMission() == 0x29) ? 10000 : 45000;
    if (elapsed <= period)
        return;

    this->alienAttackTimer = 0;
    if (this->enemies == nullptr)
        return;

    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int *ki = (int *) (*this->enemies)[i];

        if (((KIPlayer *) ki)->shipGroup == 9 && ((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->
            isActive() == 0) {
            ((PlayerFighter *) ki)->revive();
            int inOrbit = (Globals::status->inAlienOrbit() != 0);
            if (!inOrbit) {
                Station *st = (Station *) Globals::status->getStation();
                if (((Station *) st)->isAttackedByAliens() == 0)
                    inOrbit = 1;
            }
            levelPlaceAlien(thisptr, ki, inOrbit);
        }
    }
}

void Level::createMission() {
    Mission *mission = (Mission *) Globals::status->getMission();
    if (mission == 0)
        return;

    if (Globals::status->inAlienOrbit() != 0) {
        int lvl = Globals::status->getLevel();
        AbyssEngine::AERandom *rng = Globals::rnd;
        int roll = rng->nextInt();
        float base = (float) lvl * 0.5f - 1.0f;
        unsigned count = 2;
        if (2.0f <= base + (float) roll) {
            int r2 = rng->nextInt();
            count = (unsigned) (base + (float) r2);
        }
        int campA = Globals::status->getCurrentCampaignMission();
        int campB = Globals::status->getCurrentCampaignMission();
        if (campB == 0x21) count = 2;
        if (campA == 0x44) count = 2;

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(count, *(this->enemies));

        Globals *globals = Globals::globals;
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
    } else {
        Status *S = Globals::status;
        Globals *G = Globals::globals;
        AbyssEngine::AERandom *rnd = Globals::rnd;
        float optDiff = *reinterpret_cast<float *>(&Globals::options[0x2c]);

        if (mission->isEmpty() != 0) {
            // -------- raid / scene branch --------
            this->missionPtr = 0;
            createScene();
            this->missionPtr = 3;

            bool introOrThynome = ((SolarSystem *) S->getSystem())->getIndex() == 15 &&
                                  S->getCurrentCampaignMission() < 16;
            bool varHastra = S->getStation()->getIndex() == 78;

            int pirateCnt = 0;
            Mission *freelance = S->getFreelanceMission();
            if (freelance != 0 && (freelance->getType() == 0 || freelance->getType() == 11 ||
                                   freelance->getType() == 15)) {
                pirateCnt = (int) (5.0f * (float) freelance->getDifficulty() / 10.0f);
            }

            int safety = ((SolarSystem *) S->getSystem())->getSecurityLevel();
            int raidThreshold = safety == 0 ? 90 : safety == 1 ? 65 : safety == 2 ? 35 : 10;
            bool raidActive = !introOrThynome && rnd->nextInt(100) < raidThreshold;

            int raidCoords[3] = {-50000 + rnd->nextInt(100000), 0, 50000 + rnd->nextInt(50000)};
            this->field_180 = new Route(raidCoords, 3);

            int localRace = ((SolarSystem *) S->getSystem())->getRace();
            int attackerRace = (rnd->nextInt(100) < 75) ? 8 : (int) ((Standing *) (intptr_t) S->getStanding())->getEnemyRace(localRace);
            int raiderRace = attackerRace;

            this->hostileCount = raidActive ? rnd->nextInt(4) : 0;
            this->field_164 = varHastra ? 0 : rnd->nextInt(2);
            this->field_168 = (!varHastra && !introOrThynome) ? rnd->nextInt(5) : 0;
            this->friendCount = (varHastra ? 0 : rnd->nextInt(2)) + (introOrThynome ? 0 : safety) +
                                this->field_168 / 4;

            // ---- v2 count-adjustment chain (0xc0680-0xc0a9e) ----
            if (raidActive && S->getCurrentCampaignMission() >= 0x20 && rnd->nextInt(100) <= 7) {
                this->friendCount = 9;
                raiderRace = 9;
            }

            int stationPirateBase = (int) S->getStation()->stationHasPirateBase();
            float pb = stationPirateBase ? 5.0f + (optDiff - 0.5f) * 5.0f : 0.0f;
            pirateCnt = pirateCnt + (int) pb;
            if (S->getSystem() != 0 && ((SolarSystem *) S->getSystem())->hasPirateBase() != 0) {
                pirateCnt = rnd->nextInt(1) + 2;
                if (S->hardCoreMode())
                    pirateCnt = rnd->nextInt(3) + pirateCnt * 2;
                raidActive = false;
                this->hostileCount = 0;
            }

            if (Level::comingFromAlienWorld == 0) {
                if (S->getStation()->isAttackedByAliens() != 0 &&
                    S->getCurrentCampaignMission() != 0x2a) {
                    if (this->field_168 < 2)
                        this->field_168 = 2;
                    this->hostileCount = rnd->nextInt(4) + 2;
                    if (this->landmarks != 0)
                        this->field_180->setNewCoords((*this->landmarks)[0]->getPosition());
                    raidActive = true;
                    raiderRace = 9;
                }
            }

            Wanted *wantedOrbit = S->getWantedInCurrentOrbit();
            if (wantedOrbit != 0) {
                if (this->friendCount >= 3)
                    this->friendCount = 2;
            }

            int extra = 0;
            {
                float f = 0.0f;
                int cm = S->getCurrentCampaignMission();
                if (cm >= 0x65 && cm <= 0x90)
                    f = (float) cm / 144.0f * 15.0f + 5.0f;
                int hc = S->hardCoreMode() ? 1 : 0;
                int t = (int) f << hc;
                if (t >= 1 && rnd->nextInt(100) < t)
                    extra = (rnd->nextInt(3) + 2) << hc;
            }

            bool skipClamp = false;
            Mission *campMission = (Mission *) S->getCampaignMission();
            if (campMission != 0 && campMission->getType() == 0xd && S->field_0xf1 == 0 &&
                campMission->getTargetStation() == S->getStation()->getIndex()) {
                this->field_164 = 0;
                this->field_168 = 0;
                this->hostileCount = 0;
                raidActive = false;
                skipClamp = true;
            }

            if (S->getStation()->hasAttackedFriends() != 0) {
                if (this->friendCount > 7)
                    this->friendCount = 7;
                this->alarmRequested = 1;
                this->field_18a = 1;
            }

            if (!skipClamp) {
                if (this->friendCount + this->field_164 + this->field_168 + this->hostileCount +
                    pirateCnt == 0)
                    this->friendCount = 4;
            }

            int stationIdx = S->getStation()->getIndex();
            if (stationIdx == 100 || stationIdx == 101 || stationIdx == 108 || stationIdx == 10) {
                this->friendCount = 0;
                raidActive = false;
                this->field_164 = 0;
                this->field_168 = 0;
            } else if (stationIdx == 102 || stationIdx == 103 || stationIdx == 104) {
                this->friendCount = 0;
                this->field_164 = 0;
                this->field_168 = 0;
                this->hostileCount = rnd->nextInt(5) + 3;
                raidActive = true;
                raiderRace = 8;
            }

            {
                int cm = S->getCurrentCampaignMission();
                if ((cm == 0x24 || cm == 0x25) && S->getSystem() != 0 &&
                    ((SolarSystem *) S->getSystem())->getIndex() == 5) {
                    this->hostileCount = 0;
                    raidActive = false;
                    this->friendCount = 0;
                }
            }

            {
                int cm = S->getCurrentCampaignMission();
                if ((cm == 42 || cm == 43) && S->inAlienOrbit() == 0) {
                    raidActive = false;
                    this->hostileCount = 0;
                }
            }
            if (S->inBlackMarketSystem()) {
                this->friendCount = 0;
                this->hostileCount = rnd->nextInt(4) + 6;
                if (S->hardCoreMode())
                    this->hostileCount = rnd->nextInt(3) + this->hostileCount * 2;
                this->field_168 = 0;
                this->field_164 = 0;
                raidActive = true;
                raiderRace = 8;
            }
            if (S->inPirateLootOrbit()) {
                this->friendCount = 0;
                this->hostileCount = rnd->nextInt(4) + 10;
                if (S->hardCoreMode())
                    this->hostileCount = rnd->nextInt(3) + this->hostileCount * 2;
                this->field_168 = 0;
                this->field_164 = 0;
                raidActive = true;
                raiderRace = 8;
            }

            if (S->getStation()->getIndex() == 108) {
                if (S->field_114 == 0) {
                    // station108_handler (orig 0xb0f3e): dedicated "defend the station"
                    // campaign event with its own ship array, four static defense
                    // platforms and a freshly-constructed mission object.
                    this->friendCount = 0;
                    this->field_288 = 1;
                    this->field_164 = 0;
                    this->field_168 = 0;
                    this->hostileCount = S->hardCoreMode() ? 8 : 6;
                    this->enemies = new Array<KIPlayer *>();
                    ArraySetLength((unsigned) (this->hostileCount + 4), *(this->enemies));
                    for (int i = 0; i < 4; i = i + 1) {
                        KIPlayer *st =
                            (KIPlayer *) (intptr_t) this->createStaticObject((Waypoint *) 0, 0x37a3, true);
                        (*this->enemies)[i] = st;
                        st->name = *Globals::gameText->getText(441);
                    }
                    (*this->enemies)[0]->setPosition(0.0f, 0.0f, 0.0f);
                    (*this->enemies)[1]->setPosition(0.0f, 0.0f, 0.0f);
                    (*this->enemies)[2]->setPosition(0.0f, 0.0f, 0.0f);
                    (*this->enemies)[3]->setPosition(0.0f, 0.0f, 0.0f);
                    for (int i = 4; i < (int) this->enemies->size(); i = i + 1) {
                        KIPlayer *s =
                            this->createShip(8, 0, (int) G->getRandomEnemyFighter(8), (Waypoint *) 0, 1, 0);
                        (*this->enemies)[i] = s;
                    }
                    String clientStr(*Globals::gameText->getText(1601), false);
                    Mission *ev = new Mission(4, clientStr, (int *) 0, 4, 0, 108, 0);
                    S->setFreelanceMission(ev);
                    S->setMission(ev);
                    this->objectivesA = new Objective(18, 0, (int) this->enemies->size(), this);
                    return;
                }
                raidActive = false;
                this->hostileCount = 0;
            }

            // -------- campaign type-167: wanted-boss showdown at the target station --------
            // Gate matches orig 0xb0e42 (getCampaignMission()->getType()==0xa7 && the current
            // station is the mission's target station) combined with a wanted person present in
            // orbit. Spawn (0xb2a48): boss + wingmen with scaled hitpoints/loot, plus the
            // escape/navigation markers whose space-points are streamed in from disk (0xb4020).
            {
                Mission *campMission167 = (Mission *) S->getCampaignMission();
                if (campMission167 != 0 && campMission167->getType() == 167 &&
                    S->getStation()->getIndex() == campMission167->getTargetStation() &&
                    wantedOrbit != 0) {
                    int numWingmen = wantedOrbit->getNumWingmen();
                    this->friendCount = this->friendCount + numWingmen + 1;

                    this->enemies = new Array<KIPlayer *>();
                    ArraySetLength((unsigned) (numWingmen + 1 + 3), *(this->enemies));

                    int bossRace = wantedOrbit->getRace() <= 3 ? wantedOrbit->getRace() : 8;
                    int dockCoords[3] = {0, 0, 10000};
                    Route *bossRoute = new Route(dockCoords, 3);

                    KIPlayer *boss = this->createShip(bossRace, 0, wantedOrbit->getShip(),
                                                      bossRoute->getWaypoint(), 1, 0);
                    (*this->enemies)[0] = boss;

                    int lvl = S->getLevel();
                    if (lvl > 20)
                        lvl = 20;
                    int bonus = S->gameWon() ? 45 : S->getCurrentCampaignMission();
                    int total = lvl * 15 + wantedOrbit->getHitpoints() + bonus * 4;
                    int maxHp = (int) ((float) total * (optDiff + 0.5f));
                    boss->player->setMaxHitpoints(maxHp);
                    boss->player->setHitpoints(maxHp);
                    boss->name = wantedOrbit->getName();

                    if (boss->cargo != 0)
                        delete boss->cargo;
                    boss->cargo = new Array<int>();
                    ArrayAdd(wantedOrbit->getLoot(), *boss->cargo);
                    ArrayAdd(wantedOrbit->getLootAmount(), *boss->cargo);
                    boss->field_0x42 = 1;
                    boss->field_0x48 = wantedOrbit->getIndex();

                    for (int w = 1; w <= numWingmen; w = w + 1) {
                        KIPlayer *wm = this->createShip(bossRace, 0,
                                                        (int) G->getRandomEnemyFighter(bossRace),
                                                        bossRoute->getWaypoint(), 1, 0);
                        (*this->enemies)[w] = wm;
                        wm->player->setMaxHitpoints(maxHp / 2);
                        wm->player->setHitpoints(maxHp / 2);
                    }

                    // Escape / navigation markers (createStaticObject types 0x4974,0x4a6b,0x1a74
                    // @ orig 0xb2ea6/0xb2fc2/0xb32a4). Each either carries a hidden-blueprint loot
                    // list (Generator) or a set of streamed space-points (FileRead).
                    int escBase = numWingmen + 1;
                    int escTypes[3] = {18804, 19051, 6772};
                    for (int e = 0; e < 3; e = e + 1) {
                        KIPlayer *esc = (KIPlayer *) (intptr_t)
                            this->createStaticObject((Waypoint *) 0, escTypes[e], true);
                        (*this->enemies)[escBase + e] = esc;
                        ((PlayerFixedObject *) esc)->setMoving(false);
                        if (S->getStation()->stationHasHiddenBlueprint(false) != 0) {
                            Generator *gen = new Generator();
                            esc->cargo = gen->getLootList(115, 1);
                            delete gen;
                        } else {
                            FileRead *fr = new FileRead();
                            int spacePointId = 0; // DEFERRED: exact id table lookup @orig 0xb406c
                            Array<SpacePoint *> *pts = fr->loadSpacePoints(spacePointId, -1);
                            delete fr;
                            esc->setSpacePoints(pts);
                        }
                        esc->name = *Globals::gameText->getText(1663);
                    }

                    // Objective type 18 pair (orig 0xb3732/0xb3748). Exact bounds DEFERRED.
                    this->objectivesA = new Objective(18, 0, 1, this);
                    this->objectivesB = new Objective(18, 1, numWingmen + 1, this);
                    return;
                }
            }

            bool campaignStationMatch = false;
            {
                Mission *cm = (Mission *) S->getCampaignMission();
                if (cm != 0 && cm->getType() == 163 && S->field_90 != 0) {
                    Array<int> *arr = S->field_90;
                    for (unsigned i = 0; i < arr->size(); i = i + 1) {
                        if ((*arr)[i] == S->getStation()->getIndex()) {
                            campaignStationMatch = true;
                            break;
                        }
                    }
                }
            }

            if (campaignStationMatch) {
                this->hostileCount = rnd->nextInt(4) + 6;
                raidActive = true;
                raiderRace = 8;
            }

            this->hostileCount = this->hostileCount + extra;

            // ---- ship creation ----
            int total = this->friendCount + this->field_164 + this->field_168 + this->hostileCount +
                        pirateCnt;
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength((unsigned) total, *(this->enemies));

            int dockCoords[3] = {0, 0, 10000};
            Route *dockRoute = new Route(dockCoords, 3);

            int idx = 0;
            for (int i = 0; i < this->friendCount; i = i + 1) {
                KIPlayer *s = this->createShip(localRace, 0, (int) G->getRandomEnemyFighter(localRace),
                                               (Waypoint *) dockRoute->getDockingTarget(), 1, 0);
                (*this->enemies)[idx] = s;
                idx = idx + 1;
            }

            for (int i = 0; i < this->field_164; i = i + 1) {
                KIPlayer *s = this->createShip(localRace, 0, (int) G->getRandomEnemyFighter(attackerRace),
                                               (Waypoint *) 0, 1, 0);
                (*this->enemies)[idx] = s;
                s->setDead();
                int jr[3] = {-200000 + rnd->nextInt(400000), -100000 + rnd->nextInt(200000),
                             50000 + rnd->nextInt(100000)};
                s->setRoute(new Route(jr, 3));
                s->setJumper(true);
                idx = idx + 1;
            }

            bool terranBig = (localRace == 0) && rnd->nextInt(100) < 30;
            for (int i = 0; i < this->field_168; i = i + 1) {
                if (terranBig && i == 0) {
                    KIPlayer *s = this->createShip(localRace, 2, 0x17, (Waypoint *) 0, 1, 0);
                    (*this->enemies)[idx] = s;
                    ((PlayerFixedObject *) s)->setMoving(false);
                    s->setPosition((float) (-40000 + rnd->nextInt(80000)),
                                   (float) (-5000 + rnd->nextInt(10000)),
                                   (float) (40000 + rnd->nextInt(80000)));
                } else {
                    int model = (localRace == 1) ? 13 : 15;
                    KIPlayer *s = this->createShip(localRace, 1, model, (Waypoint *) 0, 1, 0);
                    (*this->enemies)[idx] = s;
                    ((PlayerFixedObject *) s)->setMoving(true);
                    int sign = (rnd->nextInt(2) == 0) ? 1 : -1;
                    s->setPosition((float) ((-80000 + rnd->nextInt(60000)) * sign),
                                   (float) (-20000 + rnd->nextInt(40000)),
                                   (float) (-80000 + rnd->nextInt(160000)));
                }
                idx = idx + 1;
            }

            if (raidActive) {
                int raiderModel = (int) G->getRandomEnemyFighter(raiderRace);
                for (int i = 0; i < this->hostileCount; i = i + 1) {
                    KIPlayer *s = this->createShip(raiderRace, 0, raiderModel,
                                                   (Waypoint *) this->field_180->getDockingTarget(), 1, 0);
                    (*this->enemies)[idx] = s;
                    idx = idx + 1;
                }
            } else {
                idx = idx + this->hostileCount;
            }

            for (int i = 0; i < pirateCnt; i = i + 1) {
                KIPlayer *s = this->createShip(8, 0, (int) G->getRandomEnemyFighter(8), (Waypoint *) 0, 1, 0);
                (*this->enemies)[idx] = s;
                Vec3 p = this->player->getPosition();
                s->setPosition(p.x - 30000.0f + (float) rnd->nextInt(60000),
                               p.y - 30000.0f + (float) rnd->nextInt(60000),
                               p.z - 30000.0f + (float) rnd->nextInt(60000));
                idx = idx + 1;
            }
            return;
        }

        // -------- mission-type dispatch --------
        int localRace = ((SolarSystem *) S->getSystem())->getRace();
        int attackerRace = (rnd->nextInt(100) < 75) ? 8 : (int) ((Standing *) (intptr_t) S->getStanding())->getEnemyRace(localRace);
        bool attackFromBehind = rnd->nextInt(2) == 0;

        switch (mission->getType()) {
            case 1: { // Defense
                int c[9] = {
                    -50000 + rnd->nextInt(100000), 0, attackFromBehind ? -50000 : 50000,
                    -50000 + rnd->nextInt(100000), 0, attackFromBehind ? -75000 : 75000,
                    -50000 + rnd->nextInt(100000), 0, attackFromBehind ? -100000 : 100000
                };
                this->enemyRoute = new Route(c, 9);
                int attackersCnt = 3 + (int) (5.0f * ((float) Globals::status->getMission()->getDifficulty() / 10.0f));
                int supportCnt = 2 + rnd->nextInt(6);
                int n = attackersCnt + supportCnt;
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) n, *(this->enemies));
                for (int i = 0; i < attackersCnt; i = i + 1) {
                    KIPlayer *s = this->createShip(attackerRace, 0,
                                                   (int) G->getRandomEnemyFighter(attackerRace),
                                                   (Waypoint *) 0, 1, 0);
                    (*this->enemies)[i] = s;
                    s->player->setAlwaysEnemy(true);
                }
                for (int i = attackersCnt; i < n; i = i + 1) {
                    Waypoint *w = this->enemyRoute->getWaypoint(rnd->nextInt(this->enemyRoute->length()));
                    KIPlayer *s = this->createShip(localRace, 0, (int) G->getRandomEnemyFighter(localRace),
                                                   w, 1, 0);
                    (*this->enemies)[i] = s;
                    s->player->setAlwaysFriend(true);
                }
                this->objectivesA = new Objective(7, attackersCnt, this);
                return;
            }
            case 2: { // Protection
                int sign = attackFromBehind ? -1 : 1;
                int c[6] = {
                    sign * (20000 + rnd->nextInt(20000)), 0, sign * (20000 + rnd->nextInt(20000)),
                    0, 0, 0
                };
                this->enemyRoute = new Route(c, 6);
                int supportCnt = mission->getProductionGoodAmount();
                int attackersCnt = 2 + (int) (4.0f * ((float) mission->getDifficulty() / 10.0f));
                int n = attackersCnt + supportCnt;
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) n, *(this->enemies));
                for (int i = 0; i < attackersCnt; i = i + 1) {
                    KIPlayer *s = this->createShip(attackerRace, 0,
                                                   (int) G->getRandomEnemyFighter(attackerRace),
                                                   (Waypoint *) 0, 1, 0);
                    (*this->enemies)[i] = s;
                    Waypoint *w0 = this->enemyRoute->getWaypoint(0);
                    ((PlayerFighter *) s)->setPosition((float) (w0->x + i * 2000),
                                                       (float) (w0->y + i * 2000),
                                                       (float) (w0->z + i * 2000));
                    s->player->setAlwaysEnemy(true);
                    s->setRoute(this->enemyRoute->clone());
                    s->getRoute()->reachWaypoint(0);
                }
                int k = 0;
                for (int i = attackersCnt; i < n; i = i + 1) {
                    KIPlayer *s = this->createShip(localRace, 0, (int) G->getRandomEnemyFighter(localRace),
                                                   (Waypoint *) 0, 1, 0);
                    (*this->enemies)[i] = s;
                    s->player->setAlwaysFriend(true);
                    int j = (int) (this->asteroids->size() / 2) + k;
                    k = k + 1;
                    Vec3 ap = (*this->asteroids)[j]->getPosition();
                    s->setPosition(ap.x, ap.y + 2000.0f, ap.z);
                    s->hasCargo = 0;
                    s->cargo = 0;
                    s->setSpeed(0.0f);
                    s->player->setHitpoints(s->player->getMaxHitpoints() * 3);
                }
                this->objectivesA = new Objective(18, 0, attackersCnt, this);
                this->objectivesB = new Objective(18, attackersCnt, attackersCnt + supportCnt, this);
                return;
            }
            case 4: { // Pirate hunting
                if (rnd->nextInt(2) == 0) {
                    int c[3] = {(int) this->field_c8, (int) this->field_cc, (int) this->field_d0};
                    this->playerRoute = new Route(c, 3);
                } else {
                    this->playerRoute = createRoute(2 + rnd->nextInt(2));
                }
                int pirateCnt = 2 + (int) ((float) Globals::status->getMission()->getDifficulty() * 5.0f / 10.0f);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) pirateCnt, *(this->enemies));
                for (int i = 0; i < pirateCnt; i = i + 1) {
                    Waypoint *w = this->playerRoute->getWaypoint(rnd->nextInt(this->playerRoute->length()));
                    KIPlayer *s = this->createShip(8, 0, (int) G->getRandomEnemyFighter(8), w, 1, 0);
                    (*this->enemies)[i] = s;
                    s->setToSleep();
                }
                this->objectivesA = new Objective(18, 0, pirateCnt, this);
                return;
            }
            case 3: // Recovery
            case 5: { // Salvage
                int s1 = (rnd->nextInt(2) == 0) ? 1 : -1;
                int s2 = (rnd->nextInt(2) == 0) ? 1 : -1;
                int c[3] = {(40000 + rnd->nextInt(80000)) * s1, 0, (40000 + rnd->nextInt(80000)) * s2};
                this->playerRoute = new Route(c, 3);
                int n = mission->getProductionGoodAmount();
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) n, *(this->enemies));
                for (int i = 0; i < n; i = i + 1) {
                    KIPlayer *s = this->createShip(8, 0, (int) G->getRandomEnemyFighter(8),
                                                   this->playerRoute->getWaypoint(0), 1, 0);
                    (*this->enemies)[i] = s;
                    s->setToSleep();
                }
                ((PlayerFighter *) (*this->enemies)[n - 1])->setMissionCrate(true);
                {
                    String *txt = Globals::gameText->getText(833);
                    (*this->enemies)[n - 1]->name = *txt;
                }
                this->objectivesA = new Objective(11, n - 1, this);
                this->objectivesB = new Objective(12, n - 1, this);
                return;
            }
            case 6: { // Wanted
                int s1 = (rnd->nextInt(2) == 0) ? 1 : -1;
                int s2 = (rnd->nextInt(2) == 0) ? 1 : -1;
                int c[3] = {(60000 + rnd->nextInt(80000)) * s1, 0, (60000 + rnd->nextInt(80000)) * s2};
                this->playerRoute = new Route(c, 3);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(1, *(this->enemies));
                KIPlayer *s = this->createShip(8, 0, (int) G->getRandomEnemyFighter(8),
                                               this->playerRoute->getWaypoint(0), 1, 0);
                (*this->enemies)[0] = s;
                s->setToSleep();
                int lvl = S->getLevel();
                if (lvl > 20)
                    lvl = 20;
                s->player->setMaxHitpoints(Globals::status->getMission()->getDifficulty() * lvl + 300);
                this->objectivesA = new Objective(1, 0, this);
                return;
            }
            case 7: { // Junk removal
                int c[3] = {-20000 + rnd->nextInt(40000), 0, 20000 + rnd->nextInt(40000)};
                Route *route = new Route(c, 3);
                int pirateCnt = (int) (2.0f * ((float) mission->getDifficulty() / 10.0f));
                int junkCnt = 15 + (int) (35.0f * ((float) mission->getDifficulty() / 10.0f));
                int n = junkCnt + pirateCnt;
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) n, *(this->enemies));
                for (int i = 0; i < n - pirateCnt; i = i + 1) {
                    KIPlayer *s = (KIPlayer *) (intptr_t) this->createStaticObject(route->getWaypoint(0),
                                                                                  0x4215, 1);
                    (*this->enemies)[i] = s;
                    s->player->setAlwaysEnemy(true);
                }
                for (int i = n - pirateCnt; i < n; i = i + 1) {
                    KIPlayer *s = this->createShip(8, 0, (int) G->getRandomEnemyFighter(8),
                                                   (Waypoint *) 0, 1, 0);
                    (*this->enemies)[i] = s;
                }
                this->objectivesA = new Objective(7, n - pirateCnt, this);
                this->timeLimit = 121000;
                G->addSoundResourceToList(22);
                return;
            }
            case 9: { // Escort
                int c[9] = {10000, 0, 100000, 10000, 0, 150000, 10000, 0, 200000};
                this->enemyRoute = new Route(c, 9);
                int attackersCnt = 2 + (int) (6.0f * ((float) Globals::status->getMission()->getDifficulty() / 10.0f));
                int n = attackersCnt + 5;
                int enemyRace = (int) ((Standing *) (intptr_t) S->getStanding())->getEnemyRace(mission->getClientRace());
                int clientRace = mission->getClientRace();
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) n, *(this->enemies));
                for (int i = 0; i < n; i = i + 1) {
                    if (i < attackersCnt) {
                        Waypoint *w = this->enemyRoute->getWaypoint(
                            rnd->nextInt(this->enemyRoute->length()));
                        KIPlayer *s = this->createShip(enemyRace, 0,
                                                       (int) G->getRandomEnemyFighter(enemyRace), w, 1, 0);
                        (*this->enemies)[i] = s;
                        s->setToSleep();
                        s->player->setAlwaysEnemy(true);
                    } else {
                        int model = (clientRace == 1) ? 13 : 15;
                        KIPlayer *s = this->createShip(clientRace, 2, model, (Waypoint *) 0, 1, 0);
                        (*this->enemies)[i] = s;
                        int lvl = S->getLevel();
                        if (lvl > 20)
                            lvl = 20;
                        s->player->setMaxHitpoints(100 + (lvl << 1) +
                                                   (S->getCurrentCampaignMission() << 1));
                        ((PlayerFixedObject *) s)->setMoving(true);
                        s->player->setAlwaysFriend(true);
                        s->hasCargo = 0;
                        s->cargo = 0;
                    }
                }
                ((PlayerFixedObject *) (*this->enemies)[attackersCnt])->setPosition(-2500, -300, 27000);
                ((PlayerFixedObject *) (*this->enemies)[attackersCnt + 1])->setPosition(6500, 3000, 24000);
                ((PlayerFixedObject *) (*this->enemies)[attackersCnt + 2])->setPosition(-4000, -2000, 19000);
                ((PlayerFixedObject *) (*this->enemies)[attackersCnt + 3])->setPosition(9000, -6000, 17000);
                ((PlayerFixedObject *) (*this->enemies)[attackersCnt + 4])->setPosition(3000, 7000, 15000);
                this->objectivesA = new Objective(18, 0, attackersCnt, this);
                this->objectivesB = new Objective(18, attackersCnt, attackersCnt + 5, this);
                return;
            }
            case 10: { // Intercept
                int c[6] = {
                    -2500 + rnd->nextInt(5000), -2500 + rnd->nextInt(5000), 80000 + rnd->nextInt(30000),
                    -2500 + rnd->nextInt(5000), -2500 + rnd->nextInt(5000), 120000 + rnd->nextInt(30000)
                };
                this->playerRoute = new Route(c, 6);
                int cargoShipsCnt = 2 + rnd->nextInt(2);
                int enemyShipCnt = 2 + (int) ((float) Globals::status->getMission()->getDifficulty() * 2.0f / 10.0f);
                int enemyRace = (int) ((Standing *) (intptr_t) S->getStanding())->getEnemyRace(mission->getClientRace());
                int n = cargoShipsCnt + enemyShipCnt;
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) n, *(this->enemies));
                for (int i = 0; i < cargoShipsCnt; i = i + 1) {
                    int model = (enemyRace == 1) ? 13 : 15;
                    KIPlayer *s = this->createShip(enemyRace, 2, model, this->playerRoute->getWaypoint(1),
                                                   1, 0);
                    (*this->enemies)[i] = s;
                    s->setToSleep();
                    s->player->setAlwaysEnemy(true);
                    ((PlayerFixedObject *) s)->setMoving(false);
                    Waypoint *w1 = this->playerRoute->getWaypoint(1);
                    ((PlayerFixedObject *) s)->setPosition(
                        (float) (w1->x - 10000 + rnd->nextInt(20000)),
                        (float) (w1->y - 10000 + rnd->nextInt(20000)),
                        (float) (w1->z - 10000 + rnd->nextInt(20000)));
                }
                for (int i = cargoShipsCnt; i < n; i = i + 1) {
                    Waypoint *w = this->playerRoute->getWaypoint(rnd->nextInt(this->playerRoute->length()));
                    KIPlayer *s = this->createShip(enemyRace, 0, (int) G->getRandomEnemyFighter(enemyRace),
                                                   w, 1, 0);
                    (*this->enemies)[i] = s;
                    s->setToSleep();
                    s->player->setAlwaysEnemy(true);
                }
                this->objectivesA = new Objective(7, cargoShipsCnt, this);
                return;
            }
            case 12: { // Challenge
                this->playerRoute = createRoute(3 + rnd->nextInt(2));
                int pirateCnt = 3 + (int) (4.0f * ((float) Globals::status->getMission()->getDifficulty() / 10.0f));
                if (pirateCnt % 2 == 0)
                    pirateCnt = pirateCnt + 1;
                if (mission->isCampaignMission()) {
                    pirateCnt = 7;
                    int c[12] = {
                        80000, -20000, 80000,
                        70000, 0, -80000,
                        -100000, 10000, -80000,
                        -80000, 20000, 90000
                    };
                    this->playerRoute = new Route(c, 12);
                }
                int n = pirateCnt + 1;
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) n, *(this->enemies));
                int agentRace = mission->getAgent()->getRace();
                KIPlayer *lead = this->createShip(agentRace, 0, (int) G->getRandomEnemyFighter(agentRace),
                                                  (Waypoint *) 0, 1, 0);
                (*this->enemies)[0] = lead;
                Vec3 ep = this->player->getPosition();
                ((PlayerFighter *) lead)->setPosition(ep.x - 700.0f + (float) rnd->nextInt(1400),
                                                      ep.y - 700.0f + (float) rnd->nextInt(1400),
                                                      ep.z + 1000.0f);
                ((PlayerFighter *) lead)->setSpeed(3.0f);
                ((PlayerFighter *) lead)->setRotate(3);
                lead->player->setHitpoints(9999999);
                lead->setRoute(this->playerRoute->clone());
                lead->player->setAlwaysFriend(true);
                lead->name = mission->getAgent()->getName();
                lead->cargo = 0;
                for (int i = 1; i < n; i = i + 1) {
                    Waypoint *w = this->playerRoute->getWaypoint(rnd->nextInt(this->playerRoute->length()));
                    KIPlayer *s = this->createShip(8, 0, (int) G->getRandomEnemyFighter(8), w, 1, 0);
                    (*this->enemies)[i] = s;
                    s->setToSleep();
                }
                this->objectivesA = new Objective(20, 1, pirateCnt + 1, this);
                this->objectivesB = new Objective(21, 1, pirateCnt + 1, this);
                return;
            }
            case 15: { // Mining defense
                int c[9] = {
                    rnd->nextInt(140000) - 70000, 0, 70000,
                    rnd->nextInt(140000) - 70000, 0, 100000,
                    rnd->nextInt(140000) - 70000, 0, 140000
                };
                this->enemyRoute = new Route(c, 9);
                float d = (float) mission->getDifficulty();
                float t = d / 10.0f;
                t = t + t;
                int base = (int) t + 1;
                int enemyShipCnt = (int) ((float) base * (optDiff + 0.5f));
                int enemyRace = (int) ((Standing *) (intptr_t) S->getStanding())->getEnemyRace(mission->getClientRace());
                this->miningPlant = enemyShipCnt;
                int n = enemyShipCnt + 3;
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((unsigned) n, *(this->enemies));
                for (int i = 0; i < enemyShipCnt; i = i + 1) {
                    Waypoint *w = this->enemyRoute->getWaypoint(rnd->nextInt(this->enemyRoute->length()));
                    KIPlayer *s = this->createShip(enemyRace, 0, (int) G->getRandomEnemyFighter(enemyRace),
                                                   w, 1, 0);
                    (*this->enemies)[i] = s;
                    s->player->setAlwaysEnemy(true);
                }
                KIPlayer *plant = (KIPlayer *) (intptr_t) this->createStaticObject(this->asteroidWaypoint,
                                                                                  0x4a88, 1);
                (*this->enemies)[enemyShipCnt] = plant;
                ((PlayerFixedObject *) plant)->setDockingType(1);
                ((PlayerFixedObject *) plant)->setMoving(false);
                plant->setPosition(this->field_c8, this->field_cc, this->field_d0);
                // friendly mining formation: own ship array bound to a follow route (orig 0xb25be)
                int formationCount = n - (enemyShipCnt + 1);
                Array<KIPlayer *> *formation = new Array<KIPlayer *>();
                ArraySetLength((unsigned) formationCount, *formation);
                int fcoords[3] = {0, 0, 0};
                int ftimes[1] = {0};
                Route *formationRoute = new Route(fcoords, formation, ftimes, 1);
                formationRoute->setLoop(true);
                Route *formationClone = formationRoute->clone();
                for (int i = enemyShipCnt + 1; i < n; i = i + 1) {
                    int cr = mission->getClientRace();
                    int fmodel = (int) G->getRandomEnemyFighter(cr);
                    KIPlayer *s;
                    if (i == enemyShipCnt + 1) {
                        s = this->createShip(cr, 0, fmodel, formationRoute->getWaypoint(0), 1, 0);
                        (*this->enemies)[i] = s;
                        (*formation)[i - (enemyShipCnt + 1)] = s;
                        s->setRoute(formationRoute);
                    } else {
                        s = this->createShip(cr, 0, fmodel, formationClone->getWaypoint(0), 1, 0);
                        (*this->enemies)[i] = s;
                        (*formation)[i - (enemyShipCnt + 1)] = s;
                        s->setRoute(formationClone);
                    }
                    s->player->setAlwaysFriend(true);
                    s->player->setNeverAttack(true);
                }
                this->objectivesA = new Objective(28, mission->getProductionGoodAmount(),
                                                  mission->getProductionGoodIndex(), this);
                return;
            }
            case 183: { // Diamond escort
                int c[6] = {-55000, 0, -35000, -30000, 0, -50000};
                this->enemyRoute = new Route(c, 6);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(14, *(this->enemies));
                for (int i = 0; i < 8; i = i + 1) {
                    KIPlayer *s = this->createShip(10, 0, 0x2c, this->enemyRoute->getWaypoint(0), 1, 0);
                    (*this->enemies)[i] = s;
                    s->player->setAlwaysEnemy(true);
                    s->player->setMaxHitpoints(250);
                }
                KIPlayer *st1 = (KIPlayer *) (intptr_t) this->createStaticObject((Waypoint *) 0, 0x495d, 0);
                (*this->enemies)[8] = st1;
                ((PlayerFixedObject *) st1)->setMoving(false);
                ((PlayerFixedObject *) st1)->setDockingType(2);
                {
                    String nm;
                    nm.Set(Globals::gameText->getText(0xc88)->data);
                    ((PlayerFixedObject *) st1)->setName(nm);
                }
                st1->player->setAlwaysFriend(true);
                Waypoint *wpB = new Waypoint(0, 0, 0, 0);
                KIPlayer *st2 = (KIPlayer *) (intptr_t) this->createStaticObject(wpB, 0x4974, 1);
                (*this->enemies)[9] = st2;
                ((PlayerFixedObject *) st2)->setDockingType(1);
                st2->player->setHitpoints(9999999);
                // escort wing: own ship array bound to a follow route (orig 0xb135c)
                Array<KIPlayer *> *formation = new Array<KIPlayer *>();
                ArraySetLength(4, *formation);
                int fcoords[3] = {0, 0, 0};
                int ftimes[1] = {0};
                Route *formationRoute = new Route(fcoords, formation, ftimes, 1);
                formationRoute->setLoop(true);
                for (int i = 10; i < 14; i = i + 1) {
                    KIPlayer *s = this->createShip(0, 0, 0x33, formationRoute->getWaypoint(0), 1, 0);
                    (*this->enemies)[i] = s;
                    (*formation)[i - 10] = s;
                    s->setRoute(formationRoute->clone());
                    s->player->setAlwaysFriend(true);
                }
                (*this->enemies)[10]->setPosition(20000.0f, -3000.0f, -50000.0f);
                (*this->enemies)[11]->setPosition(10000.0f, -7000.0f, -30000.0f);
                (*this->enemies)[12]->setPosition(25000.0f, 0.0f, -36000.0f);
                (*this->enemies)[13]->setPosition(12000.0f, 6000.0f, -45000.0f);
                this->player->setPosition(-30000.0f, 5000.0f, -100000.0f);
                Vec3 pp = this->player->getPosition();
                this->field_c8 = pp.x;
                this->field_cc = pp.y;
                this->field_d0 = pp.z;
                this->timeLimit = 181000;
                this->objectivesA = new Objective(3, 181000, this);
                return;
            }
            case 184: { // Production delivery
                int c[9] = {
                    rnd->nextInt(140000) - 70000, 0, 70000,
                    rnd->nextInt(140000) - 70000, 0, 100000,
                    rnd->nextInt(140000) - 70000, 0, 140000
                };
                this->enemyRoute = new Route(c, 9);
                Globals::status->getMission()->getDifficulty();
                ((Standing *) (intptr_t) S->getStanding())->getEnemyRace(mission->getClientRace());
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(4, *(this->enemies));
                Waypoint *w0 = new Waypoint(-30000, 0, 40000, 0);
                KIPlayer *s0 = (KIPlayer *) (intptr_t) this->createStaticObject(w0, 0x4a88, 1);
                (*this->enemies)[0] = s0;
                ((PlayerFixedObject *) s0)->setMoving(false);
                ((PlayerFixedObject *) s0)->setDockingType(2);
                {
                    String nm;
                    nm.Set(Globals::gameText->getText(0xc88)->data);
                    ((PlayerFixedObject *) s0)->setName(nm);
                }
                Waypoint *w1 = new Waypoint(30000, 0, 40000, 0);
                KIPlayer *s1 = (KIPlayer *) (intptr_t) this->createStaticObject(w1, 0x498e, 1);
                (*this->enemies)[1] = s1;
                ((PlayerFixedObject *) s1)->setMoving(false);
                ((PlayerFixedObject *) s1)->setDockingType(1);
                {
                    String nm;
                    nm.Set(Globals::gameText->getText(0xc89)->data);
                    ((PlayerFixedObject *) s1)->setName(nm);
                }
                int clientRace = mission->getClientRace();
                // escort wing: own ship array bound to a follow route (orig 0xb28be)
                Array<KIPlayer *> *formation = new Array<KIPlayer *>();
                ArraySetLength(2, *formation);
                int fcoords[3] = {0, 0, 0};
                int ftimes[1] = {0};
                Route *formationRoute = new Route(fcoords, formation, ftimes, 1);
                formationRoute->setLoop(true);
                Route *formationClone = formationRoute->clone();
                for (int i = 2; i < 4; i = i + 1) {
                    int fmodel = (int) G->getRandomEnemyFighter(clientRace);
                    KIPlayer *s;
                    if (i == 2) {
                        s = this->createShip(clientRace, 0, fmodel, formationRoute->getWaypoint(0), 1, 0);
                        (*this->enemies)[i] = s;
                        (*formation)[i - 2] = s;
                        s->setRoute(formationRoute);
                    } else {
                        s = this->createShip(clientRace, 0, fmodel, formationClone->getWaypoint(0), 1, 0);
                        (*this->enemies)[i] = s;
                        (*formation)[i - 2] = s;
                        s->setRoute(formationClone);
                    }
                    s->player->setAlwaysFriend(true);
                    s->player->setNeverAttack(true);
                }
                this->objectivesA = new Objective(29, mission->getProductionGoodAmount(),
                                                  mission->getProductionGoodIndex(), this);
                return;
            }
            default:
                return;
        }
    }
}

void Level::createAsteroids() {
    int *rngObj = (int *) &Globals::rnd;

    int colBase;
    if (Globals::status->inAlienOrbit() == 0) {
        Globals::status->getSystem();
        colBase = (((SolarSystem *) Globals::status->getSystem())->getIndex() == 0x16) ? 2 : 0;
    } else {
        colBase = 0;
    }

    this->asteroids = new Array<KIPlayer *>();

    Galaxy *gal = Globals::galaxy;
    Station *st = (Station *) Globals::status->getStation();
    int *prob = (int *) gal->getAsteroidProbabilities(st);

    int seed = *rngObj;
    Station *st2 = (Station *) Globals::status->getStation();
    ((Station *) st2)->getIndex();
    ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->setSeed((long long) seed);

    int countRoll = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(0x28);
    ArraySetLength((unsigned) (countRoll + 0x28), *(this->asteroids));

    int rx = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(100000);
    int ry = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(100000);
    int rz = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt(100000);

    int alien = Globals::status->inAlienOrbit();
    int camp = Globals::status->getCurrentCampaignMission();

    int ox, oy, oz;
    if (alien != 0) {
        oy = 0;
        oz = 30000;
        ox = -30000;
        if (camp == 0x9a)
            ox = -70000;
    } else {
        bool placed = false;
        if (camp == 0x72) {
            Station *s = (Station *) Globals::status->getStation();
            if (((Station *) s)->getIndex() == 0x53) {
                oz = 80000;
                ox = 30000;
                oy = 0;
                placed = true;
            }
        }
        if (!placed && camp == 0x59 && Globals::status->inSupernovaOrbit() != 0) {
            ox = -100000;
            oz = -50000;
            oy = 0;
            placed = true;
        }
        if (!placed && camp == 0x5b) {
            Station *s = (Station *) Globals::status->getStation();
            if (((Station *) s)->getIndex() == 0x6e) {
                oy = 0;
                oz = 50000;
                ox = 60000;
                placed = true;
            }
        }
        if (!placed && camp == 0x91) {
            Station *s = (Station *) Globals::status->getStation();
            if (((Station *) s)->getIndex() == 0x70) {
                oz = 70000;
                ox = 50000;
                oy = 0;
                placed = true;
            }
        }
        if (!placed) {
            ox = rx - 50000;
            oz = rz + 20000;
            oy = ry - 50000;
            if (Globals::status->getCurrentCampaignMission() == 0 && this->missionPtr == 3) {
                oz = 0;
                ox = 0;
                oy = 0;
            }
        }
    }

    ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->reset();

    this->field_c8 = (float) ox;
    this->field_cc = (float) oy;
    this->field_d0 = (float) oz;

    Waypoint *wp = (Waypoint *) ::operator new(0x138);
    new(wp) Waypoint(ox, oy, oz, 0);
    this->asteroidWaypoint = wp;

    BoundingSphere *bs = (BoundingSphere *) ::operator new(0x48);
    new(bs) BoundingSphere(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    *(BoundingSphere **) &this->collisionVolume = bs;

    int density = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt() + 2;
    int alien2 = Globals::status->inAlienOrbit();

    PaintCanvas *canvas = Globals::Canvas;
    int kind = 0x9a;
    int probCursor = 0;

    for (unsigned i = 0; i < this->asteroids->size(); i = i + 1) {
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

        unsigned spread = (int) i < density ? 60000u : 100000u;
        float half = (float) (spread >> 1);

        int colVariant = colBase;
        if (Globals::status->inAlienOrbit() != 0)
            colVariant = 1;
        if (kind == 0xd9)
            colVariant = 3;

        static const int meshTable[4] = {0x37a0, 0x37a4, 0x37a8, 0x37ac};
        int mesh = meshTable[colVariant & 3];

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
        new(geo) AEGeometry((uint16_t) mesh, canvas, 0);

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

        int base = (*g_ca_lowDetail && **g_ca_lowDetail != 0) ? 0x46 : (near ? 0x46 : 0x46);
        (void) base;
        int radius = ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt() + (near ? 0x78 : 0x1e);
        float scale = (float) radius * 0.001f;

        double dscale = (double) scale;
        if (dscale >= 1.0 && dscale >= 1.0 && dscale >= 1.0 &&
            ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt() != 0) {
            ((AbyssEngine::AERandom *) (intptr_t) * rngObj)->nextInt();
        }

        PlayerAsteroid *a = (PlayerAsteroid *) ::operator new(0x170);
        new(a) PlayerAsteroid(kind, geo, colVariant, kind, pos, scale, (int) scale);
        (*this->asteroids)[i] = a;

        (*this->asteroids)[i]->setLevel(this);

        ((PlayerAsteroid *) (*this->asteroids)[i])->setAsteroidCenter(
            Vector{this->field_c8, this->field_cc, this->field_d0});
    }

    if (prob != 0)
        ::operator delete[](prob);
}

void Level::createCampaignMission() {
    int idx = Globals::status->getCurrentCampaignMission();

    if (idx == 0) {
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(3, *(this->enemies));
        float c = g_ccm_pos0;
        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            int type = (i == 1) ? 0x17 : 2;
            (*this->enemies)[i] = (KIPlayer *) this->createShip(8, 0, type, (Waypoint *) (intptr_t) 0, true, false);
            (*this->enemies)[i]->setToSleep();
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
            KIPlayer *kp = (*this->enemies)[i];
            kp->setPosition(c, c, c);
            kp->cargo = nullptr;
            kp->hasCargo = 0;
            kp->player->setHitpoints(150);
            if (i <= 2)
                ((PlayerFighter *) (*this->enemies)[i])->setExhaustVisible(false);
        }
        this->player->player->setHitpoints(9999999);
        // DEFERRED 0xb52cc: str 9999999 -> [[sp#68]+0][0x64]; strb 1 -> [this->player[0]][0x5e]
        //   sp#68 = r6 (Globals::status GOT slot @0x21015c) saved by strd @0xb4bf0;
        //   receiver of the 0x64 store is uncertain (Status vs Player), so not modeled.
        return;
    }

    if (idx == 25 || idx == 29) {
        // case 25 / case 29 (shared body @0xb4cd2)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(3, *(this->enemies));
        AbyssEngine::AERandom *rng = Globals::rnd;
        for (unsigned i = 0; i < 3; i = i + 1) {
            (*this->enemies)[i] =
                (KIPlayer *) this->createShip(9, 0, 8, (Waypoint *) (intptr_t) 0, true, false);
            int signX = (rng->nextInt(2) == 0) ? 1 : -1;
            int magX = rng->nextInt(80000) + 20000;
            int signY = (rng->nextInt(2) == 0) ? 1 : -1;
            int magY = rng->nextInt(80000) + 20000;
            int signZ = (rng->nextInt(2) == 0) ? 1 : -1;
            int magZ = rng->nextInt(80000) + 20000;
            (*this->enemies)[i]->setPosition((float) (signX * magX), (float) (signY * magY),
                                             (float) (signZ * magZ));
        }
        return;
    }

    if (idx == 65) {
        // case 65 (body @0xb7256)
        int coords[3] = {0, 0, 0};
        this->enemyRoute = new Route(coords, 3);
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(1, *(this->enemies));
        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(0, 0, 38, (Waypoint *) (intptr_t) 0, true, false);
        (*this->enemies)[0]->player->setAlwaysFriend(true);
        (*this->enemies)[0]->name = *Globals::gameText->getText(1617);
        (*this->enemies)[0]->setRoute(this->enemyRoute->clone());
        (*this->enemies)[0]->player->setHitpoints(9999999);
        return;
    }

    if (idx == 97) {
        // case 97 (body @0xb8c32)
        int coords[3] = {0, 0, 50000};
        this->enemyRoute = new Route(coords, 3);

        bool hd = Globals::isRunningHDonWeakDevice != 0;
        unsigned enemyCount = hd ? 12 : 8;
        unsigned friendEnd = hd ? 16 : 11;
        unsigned staticEnd = hd ? 19 : 11;

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(hd ? 19 : 11, *(this->enemies));

        for (unsigned i = 0; i < enemyCount; i = i + 1) {
            int type = (int) Globals::globals->getRandomEnemyFighter(8);
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                8, 0, type, this->enemyRoute->getWaypoint(0), true, false);
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
        }

        for (unsigned i = enemyCount; i < friendEnd; i = i + 1) {
            int type = (int) Globals::globals->getRandomEnemyFighter(2);
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                2, 0, type, this->enemyRoute->getWaypoint(0), true, false);
            (*this->enemies)[i]->player->setAlwaysFriend(true);
        }

        for (unsigned i = friendEnd; i < staticEnd; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                2, 1, 15, this->enemyRoute->getWaypoint(0), false, (int) friendEnd);
            ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
            (*this->enemies)[i]->player->setAlwaysFriend(true);
        }

        this->objectivesA = new Objective(18, 0, (int) friendEnd, this);
        return;
    }

    if (idx == 100) {
        // case 100 (body @0xb8ddc)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(2, *(this->enemies));

        Vector *spawn = (Vector *) &this->field_18c;
        *spawn = this->player->getPosition();
        {
            int signX = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
            spawn->x += (float) (signX * Globals::rnd->nextInt(50000) + 20000);
            int signY = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
            spawn->y += (float) (signY * Globals::rnd->nextInt(50000) + 10000);
            int signZ = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
            spawn->z += (float) (signZ * Globals::rnd->nextInt(50000) + 20000);
        }

        int coords[3] = {(int) spawn->x, (int) spawn->y, (int) spawn->z};
        Route *route = new Route(coords, 3);

        for (unsigned i = 0; i < 2; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                10, 0, 44, route->getWaypoint(), true, false);
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
        }

        new Objective(4, 1, this);
        return;
    }

    if (idx == 106) {
        // case 106 (body @0xb9670)
        int coords[6] = {-500000, 0, -1700000, -500000, 0, -3700000};
        this->enemyRoute = new Route(coords, 6);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(1, *(this->enemies));

        (*this->enemies)[0] = (KIPlayer *) this->createShip(
            10, 0, 44, this->enemyRoute->getWaypoint(0), true, false);

        // Position the ship at the route's first waypoint, then aim it.
        KIPlayer *ship = (*this->enemies)[0];
        Waypoint *wp = this->enemyRoute->getWaypoint(0);
        Vector pos = wp->getPosition();
        // DEFERRED 0xb96f8: wp->[vtable+0x28](&out) — return-by-ptr virtual getPosition;
        //   modeled via Waypoint::getPosition(); slot identity not asm-proven.
        ship->setPosition(pos);
        ship->parentGeometry->setDirection(Vector{-1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});

        ship->setRoute(this->enemyRoute->clone());
        ship->player->setAlwaysFriend(true);
        ship->player->setAlwaysEnemy(false);
        ship->player->setHitpoints(1);
        ((PlayerFighter *) ship)->setCloakingPossible(false);
        return;
    }

    if (idx == 1) {
        // case 1 (body @0xb5318)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(1, *(this->enemies));
        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(3, 0, 30, (Waypoint *) (intptr_t) 0, true, false);
        (*this->enemies)[0]->setPosition(300.0f, 50.0f, -6000.0f);

        int coords[6] = {0, 0, -5000, 0, 0, 0};
        this->enemyRoute = new Route(coords, 6);

        KIPlayer *ship = (*this->enemies)[0];
        ship->setRoute(this->enemyRoute);
        // DEFERRED 0xb53ac: ship->[vtable+0x1c](false) — bool-taking virtual slot on KIPlayer;
        //   slot identity not asm-proven, so not modeled.
        ((PlayerFighter *) ship)->setExhaustVisible(false);
        return;
    }

    if (idx == 4) {
        // case 4 (body @0xb53c4)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(1, *(this->enemies));
        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(8, 0, 2, (Waypoint *) (intptr_t) 0, true, false);
        (*this->enemies)[0]->setInitActive(false);
        (*this->enemies)[0]->player->setAlwaysEnemy(true);
        (*this->enemies)[0]->setPosition(0.0f, 0.0f, -200000.0f);
        (*this->enemies)[0]->setToSleep();
        return;
    }

    if (idx == 48 || idx == 49) {
        // case 48 / case 49 (shared body @0xb517a)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(5, *(this->enemies));
        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            (*this->enemies)[i] =
                (KIPlayer *) this->createShip(1, 0, 9, (Waypoint *) (intptr_t) 0, true, false);
            (*this->enemies)[i]->player->setAlwaysEnemy(false);
            (*this->enemies)[i]->player->setAlwaysFriend(true);
        }
        return;
    }

    if (idx == 79) {
        // case 79 (body @0xb7e98)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(7, *(this->enemies));
        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            int type = (int) Globals::globals->getRandomEnemyFighter(9);
            (*this->enemies)[i] =
                (KIPlayer *) this->createShip(9, 0, type, (Waypoint *) (intptr_t) 0, true, false);
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
        }
        return;
    }

    if (idx == 24) {
        // case 24 (body @0xb5da8)
        int coords[6] = {0, 0, 0, 0, 0, 0};
        this->enemyRoute = new Route(coords, 6);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(5, *(this->enemies));

        // enemies[0..2]: escorted friendlies following the route.
        for (unsigned i = 0; i < 3; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                9, 0, 8, this->enemyRoute->getWaypoint(), true, false);
            (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
        }

        // enemies[3..4]: static turrets bound to the same route.
        for (unsigned i = 3; i < 5; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                2, 1, 15, this->enemyRoute->getWaypoint(), true, false);
            ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
            KIPlayer *ship = (*this->enemies)[i];
            ship->player->setMaxHitpoints(ship->player->getMaxHitpoints() / 3);
            delete ship->cargo; // 0xb5ea8: Array<int>::~Array + operator delete
            ship->cargo = nullptr;
        }

        // landmarks[3]->setVisible(false)  (this->landmarks @0x100)
        (*this->landmarks)[3]->setVisible(false);

        new Objective(31, 0, this);
        return;
    }

    if (idx == 87) {
        // case 87 (body @0xb810a)
        this->objectivesA = new Objective(22, 0, this);
        return;
    }

    if (idx == 137) {
        // case 137 (body @0xb991e)
        this->objectivesA = new Objective(4, 4, this);
        return;
    }

    if (idx == 145) {
        // case 145 (body @0xba312; contains the 0xba6a6 finale). id 145 dispatches
        // via the second jump-table (ids 131-158) at tbh 0xb4dcc, index 145-131=14.
        int coords[3] = {-45000, 0, 60000};
        this->enemyRoute = new Route(coords, 3);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(14, *(this->enemies));

        // Leader ship (enemies[0]) @0xba352..0xba43e
        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(2, 0, 49, (Waypoint *) (intptr_t) 0, true, false);
        {
            KIPlayer *lead = (*this->enemies)[0];
            Vector spawn = {30000.0f, 0.0f, 80000.0f};
            *(Vector *) &this->field_18c = spawn;         // this->field_18c = {30000,0,80000}
            lead->setPosition(spawn);                      // vtable+0x44 setPosition(Vector)
            lead->player->setAlwaysEnemy(true);
            lead->name = *Globals::gameText->getText(1636);
            lead->setRoute(this->enemyRoute->clone());

            AEGeometry *geo = lead->parentGeometry;
            ((PlayerFighter *) lead)->field_0x13d = 0;       // strb 0 @[ship+0x13d]
            Vector target = {-50000.0f, 0.0f, 50000.0f};
            Vector dir = AbyssEngine::AEMath::VectorNormalize(target - lead->getPosition());
            Vector up = {0.0f, 1.0f, 0.0f};
            geo->setDirection(dir, up);
            ((PlayerFighter *) lead)->setAIDisabled(true);
        }

        // Wing formation @0xba48c..0xba5da: enemies[1..12], 12 fighters.
        // fp (xPos) starts 23000 (+3000/iter); r9 (zBase) starts 72000 (+2000/iter).
        for (unsigned i = 1; i < 13; i = i + 1) {
            int xPos = 23000 + 3000 * (int) (i - 1);
            int zBase = 72000 + 2000 * (int) (i - 1);

            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                10, 0, 44, this->enemyRoute->getWaypoint(0), true, false);

            AbyssEngine::AERandom *rnd = Globals::rnd;
            int x1 = rnd->nextInt(1000);
            int yPos = (rnd->nextInt(1000) - 500) * (int) i + x1;
            int zPos = rnd->nextInt(1000) + zBase;
            (*this->enemies)[i]->setPosition((float) xPos, (float) yPos, (float) zPos);

            KIPlayer *ship = (*this->enemies)[i];
            ship->setRoute(this->enemyRoute->clone());
            ship->player->setAlwaysEnemy(true);
            ((PlayerFighter *) ship)->setAIDisabled(true);
            ((PlayerFighter *) ship)->field_0x13d = 0;
            ((PlayerFighter *) ship)->setCloakingPossible(false);

            AEGeometry *geo = ship->parentGeometry;
            Vector target = {-50000.0f, 0.0f, 50000.0f};
            Vector dir = AbyssEngine::AEMath::VectorNormalize(target - ship->getPosition());
            Vector up = {0.0f, 1.0f, 0.0f};
            geo->setDirection(dir, up);
        }

        // Static hangar/marker object (enemies[13]) @0xba5de..0xba698
        (*this->enemies)[this->enemies->size() - 1] =
            (KIPlayer *) (intptr_t) this->createStaticObject((Waypoint *) (intptr_t) 0, 19050, true);
        {
            KIPlayer *st = (*this->enemies)[this->enemies->size() - 1];
            st->setVisible(false);
            st->setActive(false);
            st->name = *Globals::gameText->getText(3207);
            st->shipGroup = 3;                               // str 3 @[KIPlayer+0x28]
            st->player->setHitpoints(9999999);
            st->field_0x74 = 1;                               // strb 1 @[KIPlayer+0x74]
        }

        this->objectivesA = new Objective(4, 2, this);        // @0xba69c..0xba6aa
        return;
    }

    if (idx == 26) {
        // case 26 (body @0xb5ef6)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(2, *(this->enemies));

        AbyssEngine::AERandom *rng = Globals::rnd;
        for (unsigned i = 0; i < 2; i = i + 1) {
            (*this->enemies)[i] =
                (KIPlayer *) this->createShip(9, 0, 8, (Waypoint *) (intptr_t) 0, true, false);

            // spawn = playerPos - normalize(playerDirection) * 8000
            Vector spawn = this->player->getPosition();
            Vector dir = this->player->geometry->getDirection();
            spawn -= AbyssEngine::AEMath::VectorNormalize(dir) * 8000.0f;

            // jitter each axis by nextInt(1400) - 700
            spawn.x += (float) (rng->nextInt(1400) - 700);
            spawn.y += (float) (rng->nextInt(1400) - 700);
            spawn.z += (float) (rng->nextInt(1400) - 700);

            (*this->enemies)[i]->setPosition(spawn);
        }

        new Objective(7, 2, this);
        return;
    }

    if (idx == 81) {
        // case 81 (body @0xb805c)
        this->enemies = new Array<KIPlayer *>();

        Station *station = (Station *) (long) Globals::galaxy->getStation(101);
        PlayerStation *ps = new PlayerStation(station);
        ArrayAdd((KIPlayer *) ps, *(this->enemies));
        (*this->enemies)[0]->name = station->getName();
        (*this->enemies)[0]->shipGroup = 8;
        if (station != nullptr)
            delete station;

        for (int n = 0; n < 8; n = n + 1) {
            KIPlayer *s =
                (KIPlayer *) this->createShip(9, 0, 8, (Waypoint *) (intptr_t) 0, true, false);
            ArrayAdd(s, *(this->enemies));
        }
        return;
    }

    if (idx == 7) {
        // case 7 (body @0xb543e)
        int coords[6] = {-4000, -3000, 80000, 10000, 7000, 160000};
        this->playerRoute = new Route(coords, 6);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(4, *(this->enemies));

        for (unsigned i = 0; i < 3; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                8, 0, 2, this->playerRoute->getWaypoint(1), true, false);
            (*this->enemies)[i]->setToSleep();
        }

        (*this->enemies)[3] =
            (KIPlayer *) this->createShip(3, 0, 30, (Waypoint *) (intptr_t) 0, true, false);
        (*this->enemies)[3]->player->setAlwaysFriend(true);

        Vector base = this->player->getPosition();
        (*this->enemies)[3]->setPosition(base.x + 50.0f, base.y - 6000.0f, base.z + 700.0f);

        (*this->enemies)[3]->setRoute(this->playerRoute->clone());
        (*this->enemies)[3]->player->setHitpoints(9999999);
        (*this->enemies)[3]->name = *Globals::gameText->getText(1599);
        ((PlayerFighter *) (*this->enemies)[3])->setBoostProb(0);

        new Objective(18, 0, 3, this);
        return;
    }

    if (idx == 14) {
        // case 14 (body @0xb55a6)
        int coords[3] = {40000, 5000, 30000};
        this->enemyRoute = new Route(coords, 3);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(7, *(this->enemies));

        for (unsigned i = 0; i < 3; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                8, 0, 0, this->enemyRoute->getWaypoint(), true, false);
        }
        for (unsigned i = 3; i < 5; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                0, 0, 5, this->enemyRoute->getWaypoint(), true, false);
        }
        for (unsigned i = 5; i < 7; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                0, 1, 14, this->enemyRoute->getWaypoint(), true, false);
        }

        // enemies[5]: waypoint position nudged forward on X.
        Vector p5 = this->enemyRoute->getWaypoint()->getPosition();
        p5.x += 7000.0f;
        (*this->enemies)[5]->setPosition(p5);

        // enemies[6]: waypoint position offset on all axes.
        Vector p6 = this->enemyRoute->getWaypoint()->getPosition();
        p6.x += -9000.0f;
        p6.y += 2000.0f;
        p6.z += 7000.0f;
        (*this->enemies)[6]->setPosition(p6);

        this->player->setPosition(40000.0f, 0.0f, 120000.0f);

        new Objective(31, 0, this);
        return;
    }

    if (idx == 28) {
        // case 28 (body @0xb603a)
        // Reposition landmark[3], then build a route anchored at its new position.
        (*this->landmarks)[3]->setPosition(100000.0f, -50000.0f, -40000.0f);
        *(Vector *) &this->field_18c = (*this->landmarks)[3]->getPosition();

        int coords[3] = {(int) *(float *) &this->field_18c,
                         (int) *(float *) &this->field_190,
                         (int) *(float *) &this->field_194};
        this->enemyRoute = new Route(coords, 3);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(8, *(this->enemies));

        // enemies[0..4]: escort fighters bound to the route.
        for (unsigned i = 0; i < 5; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                9, 0, 8, this->enemyRoute->getWaypoint(), true, false);
        }

        // enemies[5..7]: static turrets, weakened, cargo cleared.
        for (unsigned i = 5; i < 8; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                0, 1, 15, this->enemyRoute->getWaypoint(), true, false);
            ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
            KIPlayer *ship = (*this->enemies)[i];
            ship->player->setMaxHitpoints(ship->player->getMaxHitpoints() / 4);
            delete ship->cargo; // 0xb6164: Array<int>::~Array + operator delete
            ship->cargo = nullptr;
        }
        return;
    }

    if (idx == 40) {
        // case 40 (body @0xb65ee)
        // enemyRoute (@0x110): 3-point route; friendRoute (@0x10c): 6-point route.
        int enemyCoords[3] = {-20000, -3000, 200000};
        this->enemyRoute = new Route(enemyCoords, 3);

        int friendCoords[6] = {-20000, -3000, 35000, -20000, -3000, 200000};
        this->friendRoute = new Route(friendCoords, 6);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(13, *(this->enemies));

        // enemies[0]: escorted friendly leader following friendRoute.
        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(0, 1, 13, (Waypoint *) (intptr_t) 0, true, false);
        (*this->enemies)[0]->player->setAlwaysFriend(true);
        (*this->enemies)[0]->name = *Globals::gameText->getText(1604);
        {
            KIPlayer *lead = (*this->enemies)[0];
            lead->player->setMaxHitpoints(Globals::status->getLevel() * 5 + 1800);
            lead->setPosition(-9999999.0f, -9999999.0f, -9999999.0f);
            lead->setToSleep();
            lead->setVisible(false);
        }

        // enemies[1..4]: wing fighters bound to friendRoute; enemies[2] named.
        for (unsigned i = 1; i < 5; i = i + 1) {
            int enemyType = Globals::globals->getRandomEnemyFighter(0);
            (*this->enemies)[i] =
                (KIPlayer *) this->createShip(0, 0, enemyType, (Waypoint *) (intptr_t) 0, true, false);
            (*this->enemies)[i]->setRoute(this->friendRoute->clone());
            (*this->enemies)[i]->player->setAlwaysFriend(true);
            if (i == 2)
                (*this->enemies)[i]->name = *Globals::gameText->getText(1605);
        }

        // enemies[5..8]: escort fighters bound to enemyRoute, awake+enemy.
        for (unsigned i = 5; i < 9; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                9, 0, 8, this->enemyRoute->getWaypoint(), true, false);
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
        }

        // enemies[9..12]: sleeping enemy fighters at the origin.
        for (unsigned i = 9; i < 13; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                9, 0, 8, (Waypoint *) (intptr_t) 0, true, false);
            (*this->enemies)[i]->setPosition(0.0f, 0.0f, 0.0f);
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
            (*this->enemies)[i]->setToSleep();
        }

        // Reposition the player and orient it, on non-weak devices.
        this->player->setPosition(-105000.0f, 80000.0f, 200000.0f);
        this->player->geometry->setRotation(0.0f, 1.5707963705062866f, 0.0f);

        new Objective(7, 1, this);
        return;
    }

    if (idx == 56) {
        // case 56 (body @0xb6b9c)
        // playerRoute (@0x108): 9-point route; friendRoute (@0x10c) is its clone.
        int coords[9] = {0, 0, -60000, -12000, -7000, -110000, 15000, 3000, -160000};
        this->playerRoute = new Route(coords, 9);
        this->friendRoute = this->playerRoute->clone();

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(9, *(this->enemies));

        // enemies[0..2]: sleeping friendly cargo ships scattered near player*0.8.
        for (unsigned i = 0; i < 3; i = i + 1) {
            (*this->enemies)[i] =
                (KIPlayer *) this->createShip(8, 0, 24, (Waypoint *) (intptr_t) 0, true, false);
            (*this->enemies)[i]->player->setAlwaysFriend(true);
            (*this->enemies)[i]->player->setAlwaysEnemy(false);
            (*this->enemies)[i]->setToSleep();

            *(Vector *) &this->field_18c = this->player->getPosition() * 0.800000011920929f;

            AbyssEngine::AERandom *rng = Globals::rnd;
            float px = *(float *) &this->field_18c + -3500.0f + (float) rng->nextInt(7000);
            float py = *(float *) &this->field_190 + -3500.0f + (float) rng->nextInt(7000);
            float pz = *(float *) &this->field_194 + -3500.0f + (float) rng->nextInt(7000);
            (*this->enemies)[i]->setPosition(px, py, pz);

            (*this->enemies)[i]->setRoute(this->friendRoute->clone());
            (*this->enemies)[i]->player->setHitpoints(9999999);
        }

        // enemies[3..]: sleeping enemy fighters bound to playerRoute waypoints.
        for (unsigned i = 3; i < this->enemies->size(); i = i + 1) {
            int enemyType = Globals::globals->getRandomEnemyFighter(8);
            int wpIndex = (i < 5) ? 0 : (i < 10 ? 1 : 2);
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                8, 0, enemyType, this->playerRoute->getWaypoint(wpIndex), true, false);
            (*this->enemies)[i]->setToSleep();
        }

        new Objective(18, 3, 9, this);
        return;
    }

    if (idx == 70) {
        // case 70 (body @0xb77c0)
        // Push the player backward along its facing, then anchor enemyRoute at the
        // player's new position.
        Vector spawn =
            this->player->getPosition() - this->player->geometry->getDirection() * 120000.0f;
        this->player->setPosition(spawn);

        Vector landmarkPos = (*this->landmarks)[1]->getPosition();

        Vector playerPos = this->player->getPosition();
        int coords[3] = {(int) landmarkPos.x, (int) landmarkPos.y, (int) landmarkPos.z};
        this->enemyRoute = new Route(coords, 3);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(1, *(this->enemies));

        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(0, 0, 12, (Waypoint *) (intptr_t) 0, true, false);
        (*this->enemies)[0]->player->setAlwaysFriend(true);
        (*this->enemies)[0]->name = *Globals::gameText->getText(1631);
        (*this->enemies)[0]->setRoute(this->enemyRoute->clone());

        // Position the ship a quarter of the way from the player toward the landmark,
        // then orient it to look toward the landmark.
        {
            KIPlayer *ship = (*this->enemies)[0];
            *(Vector *) &this->field_18c = playerPos + (landmarkPos - playerPos) / 4.0f;
            ship->setPosition(*(Vector *) &this->field_18c);

            AEGeometry *geo = ship->parentGeometry;
            Vector look = AbyssEngine::AEMath::VectorNormalize(landmarkPos - playerPos);
            Vector up = {0.0f, 1.0f, 0.0f};
            geo->setDirection(look, up);
            ship->player->setHitpoints((int) ((float) ship->player->getMaxHitpoints() * 2.5f));
        }

        new Objective(1, 0, this);
        return;
    }

    if (idx == 91) {
        // case 91 (body @0xb8560)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(1, *(this->enemies));

        Waypoint *wp = new Waypoint(-20000, 0, 60000, nullptr);
        KIPlayer *ship = (KIPlayer *) (intptr_t) this->createStaticObject(wp, 0x494e, false);
        (*this->enemies)[0] = ship;
        ((PlayerFixedObject *) ship)->setMoving(false);
        ((PlayerFixedObject *) ship)->setDockingType(0);

        // Anchor the object at its waypoint's position.
        Vector pos = wp->getPosition();
        ship->setPosition(pos);

        ship->player->setAlwaysFriend(true);
        ship->player->setHitpoints(ship->player->getHitpoints() / 20);
        ship->geometry->setRotation(Vector{0.0f, 5.4977874755859375f, 0.0f});
        ship->setActive(false);

        delete ship->cargo;
        ship->cargo = nullptr;

        new Objective(1, 0, this);
        return;
    }

    if (idx == 21) {
        // case 21 (body @0xb5b72)
        int playerCoords[6] = {0, 40000, -40000, 120000, -10000, 20000};
        this->playerRoute = new Route(playerCoords, 6);

        int loopCoordsA[3] = {190000, 40000, -40000};
        Route *routeA = new Route(loopCoordsA, 3);
        routeA->setLoop(true);

        int loopCoordsB[3] = {-10000, 20000, 190000};
        Route *routeB = new Route(loopCoordsB, 3);
        routeB->setLoop(true);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(4, *(this->enemies));

        (*this->enemies)[1] =
            (KIPlayer *) this->createShip(0, 0, 5, routeA->getWaypoint(), true, false);
        (*this->enemies)[2] =
            (KIPlayer *) this->createShip(0, 0, 5, routeA->getWaypoint(), true, false);
        (*this->enemies)[3] =
            (KIPlayer *) this->createShip(0, 0, 5, routeA->getWaypoint(), true, false);
        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(0, 0, 17, routeB->getWaypoint(), true, false);
        (*this->enemies)[0]->name = *Globals::gameText->getText(1611);
        (*this->enemies)[0]->player->setAlwaysEnemy(true);

        for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
            (*this->enemies)[i]->setToSleep();
            KIPlayer *ship = (*this->enemies)[i];
            ship->setRoute(i == 0 ? routeB : routeA->clone());
            ship->geometry->setRotation(0.0f, 3.1415927410125732f, 0.0f);
        }

        // Position the named leader at routeB's first waypoint, offset in X and Z.
        {
            KIPlayer *leader = (*this->enemies)[0];
            Waypoint *wp = routeB->getWaypoint();
            leader->setPosition((float) (wp->x + 1000), (float) wp->y, (float) (wp->z + 2000));
        }

        this->objectivesA = new Objective(22, 0, this);
        this->objectivesB = new Objective(7, 1, this);

        delete routeA;
        return;
    }

    if (idx == 38) {
        // case 38 (body @0xb640e)
        int coords[3] = {90000, 10000, 80000};
        this->enemyRoute = new Route(coords, 3);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(7, *(this->enemies));

        // enemies[0..1]: static friendly guards scattered around the waypoint.
        for (unsigned i = 0; i < 2; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                2, 1, 15, this->enemyRoute->getWaypoint(), true, false);
            (*this->enemies)[i]->player->setAlwaysFriend(true);
            ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);

            KIPlayer *guard = (*this->enemies)[i];
            Waypoint *wp = this->enemyRoute->getWaypoint();
            int ox = Globals::rnd->nextInt(20000) - 10000;
            int oy = Globals::rnd->nextInt(20000) - 10000;
            int oz = Globals::rnd->nextInt(20000) - 10000;
            guard->setPosition((float) (wp->x + ox), (float) (wp->y + oy),
                               (float) (wp->z + oz));
        }

        // enemies[2..]: awake enemy fighters bound to the waypoint.
        for (unsigned i = 2; i < this->enemies->size(); i = i + 1) {
            int type = (int) Globals::globals->getRandomEnemyFighter(3);
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                3, 0, type, this->enemyRoute->getWaypoint(), true, false);
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
        }

        this->objectivesA = new Objective(18, 2, 7, this);
        new Objective(7, 2, this);
        return;
    }

    if (idx == 63) {
        // case 63 (body @0xb6db8)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(7, *(this->enemies));

        int coords[3] = {0, 0, 0};
        Route *route = new Route(coords, 3);

        // enemies[0]: sleeping named static object at the (origin) waypoint.
        (*this->enemies)[0] =
            (KIPlayer *) (intptr_t) this->createStaticObject(route->getWaypoint(), 0x37a3, true);
        Waypoint *wp = route->getWaypoint();
        Vector wpPos = wp->getPosition();
        (*this->enemies)[0]->setToSleep();
        (*this->enemies)[0]->name = *Globals::gameText->getText(441);

        // enemies[1..]: sleeping enemy fighters scattered around the waypoint.
        // Each axis draws a magnitude in [0,20000) then a sign, offset by +10000.
        for (unsigned i = 1; i < this->enemies->size(); i = i + 1) {
            int type = (int) Globals::globals->getRandomEnemyFighter(8);
            (*this->enemies)[i] =
                (KIPlayer *) this->createShip(8, 0, type, (Waypoint *) (intptr_t) 0, true, false);

            int magX = Globals::rnd->nextInt(20000);
            int signX = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
            int magY = Globals::rnd->nextInt(20000);
            int signY = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
            int magZ = Globals::rnd->nextInt(20000);
            int signZ = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
            (*this->enemies)[i]->setPosition(
                (float) (int) (wpPos.x + (float) (signX * magX + 10000)),
                (float) (int) (wpPos.y + (float) (signY * magY + 10000)),
                (float) (int) (wpPos.z + (float) (signZ * magZ + 10000)));
            (*this->enemies)[i]->setToSleep();
        }

        new Objective(27, 4, this);
        return;
    }

    if (idx == 16) {
        // case 16 (body @0xb577e)
        int coords[3] = {0, 0, 0};
        this->enemyRoute = new Route(coords, 3);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(10, *(this->enemies));

        Waypoint *wp = this->enemyRoute->getWaypoint();

        // enemies[0..2]: escort platforms parked at the route waypoint.
        for (unsigned i = 0; i < 3; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                0, 1, 15, this->enemyRoute->getWaypoint(), true, false);
            (*this->enemies)[i]->player->setAlwaysFriend(true);
            ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
            Player *p = (*this->enemies)[i]->player;
            p->setMaxHitpoints(p->getMaxHitpoints() / 3);
            delete (*this->enemies)[i]->cargo;
            (*this->enemies)[i]->cargo = nullptr;
        }

        (*this->enemies)[0]->setPosition((float) wp->x, (float) wp->y, (float) wp->z);
        {
            Player *p = (*this->enemies)[0]->player;
            p->setMaxHitpoints(p->getMaxHitpoints() / 6);
        }
        (*this->enemies)[1]->setPosition((float) (wp->x + 3000), (float) (wp->y + 2000),
                                         (float) (wp->z - 3000));
        (*this->enemies)[2]->setPosition((float) (wp->x - 9000), (float) (wp->y - 8000),
                                         (float) (wp->z - 7000));

        // enemies[3..6]: attack fighters scattered around the waypoint (+50000 on Z).
        for (unsigned i = 3; i < 7; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                9, 0, 8, this->enemyRoute->getWaypoint(), true, false);
            Player *p = (*this->enemies)[i]->player;
            p->setMaxHitpoints(p->getMaxHitpoints() * 10);
            int jitterX = Globals::rnd->nextInt(20000) - 10000;
            int jitterY = Globals::rnd->nextInt(20000) - 10000;
            (*this->enemies)[i]->setPosition((float) (wp->x + jitterX),
                                             (float) (wp->y + jitterY),
                                             (float) (wp->z + 50000));
        }

        // enemies[7..9]: friendly interceptors near the player, hitpoints 600.
        Vector base = this->player->getPosition();
        for (unsigned i = 7; i < 10; i = i + 1) {
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                0, 0, 5, (Waypoint *) (intptr_t) 0, true, false);
            (*this->enemies)[i]->player->setAlwaysFriend(true);
            (*this->enemies)[i]->player->setHitpoints(600);
            int jitterX = Globals::rnd->nextInt(4000) - 2000;
            int jitterY = Globals::rnd->nextInt(3400) - 1700;
            int jitterZ = Globals::rnd->nextInt(4000) - 2000;
            (*this->enemies)[i]->setPosition(base.x + (float) jitterX,
                                             base.y + (float) jitterY,
                                             base.z + 0.0f + (float) jitterZ);
        }

        if ((*this->landmarks)[3] != nullptr) {
            (*this->landmarks)[3]->setVisible(true);
            (*this->landmarks)[3]->setPosition((float) wp->x, (float) wp->y,
                                               (float) (wp->z + 40000));
        }

        new Objective(22, 0, this);
        return;
    }

    if (idx == 64) {
        // case 64 (body @0xb6fc2)
        Vector pos = this->player->getPosition();
        Vector dir = this->player->geometry->getDirection();
        this->player->setPosition(pos - dir * 40000.0f);

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(9, *(this->enemies));

        int coords[3] = {100000, 0, 0};
        this->enemyRoute = new Route(coords, 3);

        // enemies[0]: named friendly convoy leader at the origin.
        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(1, 0, 38, (Waypoint *) (intptr_t) 0, true, false);
        (*this->enemies)[0]->player->setAlwaysFriend(true);
        (*this->enemies)[0]->name = *Globals::gameText->getText(1617);
        (*this->enemies)[0]->setPosition(0.0f, 0.0f, 0.0f);
        (*this->enemies)[0]->parentGeometry->setDirection(Vector{1.0f, 0.0f, 0.0f},
                                                          Vector{0.0f, 1.0f, 0.0f});
        (*this->enemies)[0]->setRoute(this->enemyRoute->clone());
        (*this->enemies)[0]->cargo = nullptr;

        // enemies[1..]: enemy fighters jittered around the origin.
        for (unsigned i = 1; i < this->enemies->size(); i = i + 1) {
            int type = (int) Globals::globals->getRandomEnemyFighter(8);
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                8, 0, type, (Waypoint *) (intptr_t) 0, true, false);

            AbyssEngine::AERandom *rnd = Globals::rnd;
            int magX = rnd->nextInt(1500);
            int signX = (rnd->nextInt(2) == 0) ? 1 : -1;
            int magY = rnd->nextInt(1500);
            int signY = (rnd->nextInt(2) == 0) ? 1 : -1;
            int magZ = rnd->nextInt(1500);
            int signZ = (rnd->nextInt(2) == 0) ? 1 : -1;
            (*this->enemies)[i]->setPosition((float) (signX * (magX + 1000)),
                                             (float) (signY * (magY + 1000)),
                                             (float) (signZ * (magZ + 1000)));

            (*this->enemies)[i]->parentGeometry->setDirection(Vector{1.0f, 0.0f, 0.0f},
                                                              Vector{0.0f, 1.0f, 0.0f});
            (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
        }

        this->objectivesA = new Objective(22, 0, this);
        new Objective(1, 0, this);
        return;
    }

    if (idx == 41) {
        // case 41 (body @0xb68be)
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(8, *(this->enemies));

        // enemies[0]: named friendly freighter with level-scaled hitpoints.
        (*this->enemies)[0] =
            (KIPlayer *) this->createShip(1, 1, 13, (Waypoint *) (intptr_t) 0, true, false);
        (*this->enemies)[0]->player->setAlwaysFriend(true);
        (*this->enemies)[0]->name = *Globals::gameText->getText(1604);
        if (Level::lastMissionFreighterHitpoints <= 0)
            Level::lastMissionFreighterHitpoints =
                (int) ((float) (Globals::status->getLevel() * 5 + 1800) * 0.7f);
        (*this->enemies)[0]->player->setHitpoints(Level::lastMissionFreighterHitpoints);
        ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(true);
        (*this->enemies)[0]->setPosition(0.0f, 0.0f, -300000.0f);

        // enemies[1..7]: enemy fighters.
        for (unsigned i = 1; i < 8; i = i + 1) {
            (*this->enemies)[i] =
                (KIPlayer *) this->createShip(9, 0, 8, (Waypoint *) (intptr_t) 0, true, false);
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
        }

        (*this->enemies)[1]->setPosition(0.0f, 0.0f, -260000.0f);
        (*this->enemies)[2]->setPosition(-10000.0f, 10000.0f, -240000.0f);
        (*this->enemies)[3]->setPosition(13000.0f, 2000.0f, -220000.0f);
        (*this->enemies)[4]->setPosition(-72000.0f, -4000.0f, -210000.0f);
        (*this->enemies)[5]->setPosition(60000.0f, -40000.0f, -190000.0f);
        (*this->enemies)[6]->setPosition(-18000.0f, 30000.0f, -160000.0f);
        (*this->enemies)[7]->setPosition(17000.0f, 40000.0f, -140000.0f);
        this->player->setPosition(3000.0f, 2000.0f, -320000.0f);
        this->player->geometry->setRotation(0.0f, 0.0f, 0.0f);

        this->objectivesA = new Objective(25, 0, this);
        this->objectivesB = new Objective(7, 1, this);
        return;
    }

    if (idx == 67) {
        // case 67 (body @0xb7328)
        int coords[3] = {220000, -20000, -10000};
        this->enemyRoute = new Route(coords, 3);
        this->playerRoute = this->enemyRoute->clone();

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(11, *(this->enemies));

        // enemies[0]: named sleeping static freight object at the route start.
        (*this->enemies)[0] =
            (KIPlayer *) (intptr_t) this->createStaticObject((Waypoint *) (intptr_t) 0, 14243, true);
        (*this->enemies)[0]->name = *Globals::gameText->getText(441);
        (*this->enemies)[0]->setPosition((float) coords[0], (float) coords[1], (float) coords[2]);
        (*this->enemies)[0]->player->setAlwaysEnemy(false);
        (*this->enemies)[0]->player->setAlwaysFriend(true);
        (*this->enemies)[0]->setToSleep();

        // enemies[1..length-2]: sleeping enemy fighters on the route.
        for (unsigned i = 1; i < this->enemies->size() - 1; i = i + 1) {
            int type = (int) Globals::globals->getRandomEnemyFighter(8);
            (*this->enemies)[i] = (KIPlayer *) this->createShip(
                8, 0, type, this->enemyRoute->getWaypoint(), true, false);
            (*this->enemies)[i]->setToSleep();
            ((PlayerFighter *) (*this->enemies)[i])->field_0x128 = 150000;
            if (i >= 5)
                (*this->enemies)[i]->setPosition(800000.0f, 800000.0f, 800000.0f);
        }

        (*this->enemies)[1]->setPosition((float) (coords[0] + 30000), (float) coords[1],
                                         (float) coords[2]);

        // Final ship: friendly escort placed relative to the player, following playerRoute.
        (*this->enemies)[this->enemies->size() - 1] =
            (KIPlayer *) this->createShip(0, 0, 27, (Waypoint *) (intptr_t) 0, true, false);
        {
            Vector base = this->player->getPosition();
            KIPlayer *escort = (*this->enemies)[this->enemies->size() - 1];
            Vector offset = {2000.0f, 500.0f, -7000.0f};
            escort->setPosition(base + offset);
            escort->player->setAlwaysFriend(true);
            escort->name = *Globals::gameText->getText(1633);
            escort->player->setMaxHitpoints(9999999);
            escort->setRoute(this->playerRoute->clone());
        }

        this->objectivesA = new Objective(22, 0, this);
        this->objectivesB = new Objective(1, 0, this);
        return;
    }
}

void Level::updateOrbit(int dt) {
    Level *thisptr = this;

    int t178 = this->field_178 + dt;
    this->orbitWaveTimer = this->orbitWaveTimer + dt;
    this->field_178 = t178;

    if (this->field_18a != 0) {
        if (Globals::status->getSystem() != 0 && this->player->field_0x1c != 0) {
            Globals::status->getSystem();
            int race = ((SolarSystem *) Globals::status->getSystem())->getRace();
            thisptr->alarmAllFriends(race, 0 != 0);
            Globals::status->getSystem();
            ((SolarSystem *) Globals::status->getSystem())->getRace();
            thisptr->createRadioMessage(2, 0);
            this->field_18a = 0;
        }
        t178 = this->field_178;
    }

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

                if (0 < this->friendCount && (int) i < this->friendCount &&
                    ((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0 &&
                    (uint8_t)((KIPlayer *) ki)->field_0x42 == 0) {
                    ((PlayerFighter *) ki)->revive();
                    ((KIPlayer *) ki)->setPosition(0, 0, 0);
                }

                if (1 < hostileAlive && this->field_184 < 2 &&
                    0 < this->hostileCount &&
                    (unsigned) ((int) this->enemies->size() - this->hostileCount) <= i) {
                    int race = ((KIPlayer *) ki)->shipGroup;
                    bool secZero = false, secOne = false;
                    if (Globals::status->inAlienOrbit() == 0) {
                        SolarSystem *ss = (SolarSystem *) Globals::status->getSystem();
                        secZero = ((SolarSystem *) ss)->getSecurityLevel() == 0;
                    }
                    if (Globals::status->inAlienOrbit() == 0) {
                        SolarSystem *ss = (SolarSystem *) Globals::status->getSystem();
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

void Level::friendTurnedEnemy(int /*race*/) {
    if (field_188 == 0) {
        field_188 = 1;
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
            if (e->deadFlag == 0 && e->lostMissionCrateToEgo == 0 && e->field_0x40 == 0) {
                int wm = e->isWingMan();
                if (wm == 0) {
                    if ((char) e->field_0x44 == 0 && e->stealFlagByte == 0) {
                        count = count + (e->countsAsEnemyExcludeFlag ^ 1);
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
    int ship = (int) (intptr_t) Globals::status->getShip();
    if (((Ship *) (ship))->getFirstEquipmentOfSort(0x27) != 0) {
        field_b0 = new Array<KIPlayer *>();
        ArraySetLength(9, *field_b0);
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
            ArrayAdd(k, *enemies);
        }
    }
}

int Level::collideStation(Vector v) {
    if (this->landmarks != nullptr &&
        (*this->landmarks)[0] != 0 &&
        Globals::status->inEmptyOrbit() == 0) {
        PlayerFixedObject *obj = (PlayerFixedObject *) (*this->landmarks)[0];
        return obj->collide(v.x, v.y, v.z);
    }
    return 0;
}

void Level::uncoverWanted(int index) {
    if (field_29c == 0) {
        createRadioMessage(0x12, index);
        int **g = (int **) &Globals::status;

        for (int i = 1;
             i - 1 < ((Wanted *) ((int *) (*(int *) (*g) + 4))[index])->getNumWingmen();
             i = i + 1) {
            (*enemies)[i]->player->setAlwaysEnemy(1);
            (*enemies)[i]->player->turnEnemy();
        }
    }
}

void Level::update(long long /*time*/, bool param) {
    Level *thisptr = this;
    unsigned dt = (unsigned) param;

    if (this->flashActive != 0) {
        unsigned remaining = *(unsigned *) &this->flashDurationA - dt;
        *(unsigned *) &this->flashDurationA = remaining;
        if (0x7fffffff < remaining)
            this->flashActive = 0;

        float frac = (float) remaining / (float) this->flashDurationB;
        float *col = &this->flashColor.x;
        float lo = *g_up_clampLo, hi = *g_up_clampHi, z = *g_up_clampZ, w = *g_up_clampW;
        float scaled[4] = {col[0] * frac, col[1] * frac, col[2] * frac, col[3] * frac};
        float bounds[4] = {lo, hi, z, w};
        for (int k = 0; k < 4; k = k + 1)
            col[k] = bounds[k] > scaled[k] ? bounds[k] : scaled[k];
    }

    bool didMission = false;
    if (Globals::status->getMission() != 0) {
        Globals::status->getMission();
        if (((Mission *) Globals::status->getMission())->isEmpty() == 0) {
            thisptr->updateMissionOrbit(dt);
            didMission = true;
        }
    }
    if (!didMission)
        thisptr->updateOrbit(dt);

    Station *st = (Station *) Globals::status->getStation();
    if (((Station *) st)->isAttackedByAliens() != 0 || Globals::status->inAlienOrbit() != 0)
        thisptr->updateAlienAttackers(0);

    if (this->playerGuns != nullptr) {
        for (unsigned i = 0; i < this->playerGuns->size(); i = i + 1) {
            ((ObjectGun *) (*this->playerGuns)[i])->update(dt);
        }
    }
    if (this->enemyGuns != nullptr) {
        for (unsigned i = 0; i < this->enemyGuns->size(); i = i + 1) {
            ((ObjectGun *) (*this->enemyGuns)[i])->update(dt);
        }
    }

    int aPtr = (int) (intptr_t) Globals::status;
    Station *st2 = (Station *) Globals::status->getStation();
    int idx = ((Station *) st2)->getIndex();
    Globals::status->getCurrentCampaignMission();
    int gammaBits = ((Status *) (intptr_t) aPtr)->getGammaRayDamagePerSecond(aPtr, idx);
    float gamma = *(float *) &gammaBits;
    PlayerEgo *ego = this->player;
    if (gamma > 0.0f && ego != nullptr) {
        int ship = (int) (intptr_t) Globals::status->getShip();
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

void Level::connectPlayers() {
    if (Globals::appManager->currentModuleId == 5)
        return;

    if (this->enemies != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        ArraySetLength(this->enemies->size(), arr);
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (*this->enemies)[j]->player;
        ((Player *) this->player->player)->setEnemies(&arr);
    }

    if (this->asteroids != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        ArraySetLength(this->asteroids->size(), arr);
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (*this->asteroids)[j]->player;
        ((Player *) this->player->player)->addEnemies(&arr);
    }
    if (this->gasClouds != nullptr && this->player != nullptr) {
        Array<Player *> arr;
        ArraySetLength(this->gasClouds->size(), arr);
        for (unsigned j = 0; j < arr.size(); j = j + 1)
            arr[j] = (*this->gasClouds)[j]->player;
        ((Player *) this->player->player)->addEnemies(&arr);
    }

    if (this->enemies == nullptr)
        return;

    int camp = Globals::status->getCurrentCampaignMission();
    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int e = (int) (intptr_t)(*this->enemies)[i];
        int eFaction = ((KIPlayer *) (intptr_t) e)->shipGroup;
        int wmAll = ((KIPlayer *) e)->isWingMan();
        unsigned count = 0;
        bool notM24 = camp != 0x24;
        bool notFirst = i != 0;

        for (unsigned k = 0; k < this->enemies->size(); k = k + 1) {
            int o = (int) (intptr_t)(*this->enemies)[k];
            if (o != e && (((~wmAll) & (((KIPlayer *) (intptr_t) o)->shipGroup == eFaction)) == 0)) {
                bool consider;
                if (notM24 || notFirst) {
                    consider = true;
                    if (camp == 0x9a) {
                        int alien = Globals::status->inAlienOrbit();
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
                    if ((char) reinterpret_cast<int &>(((KIPlayer *) (intptr_t) e)->field_0x40) != 0 && ((PlayerFixedObject *) (intptr_t) e)->
                        getDockingType() == 3) {
                        Station *st = (Station *) Globals::status->getStation();
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
        ArraySetLength(count, arr);

        Mission *m = (Mission *) Globals::status->getMission();
        int mtype = ((Mission *) Globals::status->getMission())->getType();
        bool branchA = !(((i & 1) == 0 && mtype == 0xc)) &&
                       ((Mission *) Globals::status->getMission())->getType() != 2 && ((Mission *) Globals::status->
                           getMission())->
                       getType() != 9;

        bool jumpAlwaysFriend = false;
        if (branchA) {
            int tmp;
            if (m->isCampaignMission() != 0) {
                tmp = Globals::status->getCurrentCampaignMission();
                if (tmp == 0x10 && ((KIPlayer *) (intptr_t) e)->shipGroup == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && m->isCampaignMission() != 0) {
                tmp = Globals::status->getCurrentCampaignMission();
                if (tmp == 0x18 && ((KIPlayer *) (intptr_t) e)->shipGroup == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && m->isCampaignMission() != 0) {
                tmp = Globals::status->getCurrentCampaignMission();
                if (tmp == 0x1c && ((KIPlayer *) (intptr_t) e)->shipGroup == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend) {
                if (m->isCampaignMission() != 0) Globals::status->getCurrentCampaignMission();
                if (m->isCampaignMission() != 0) Globals::status->getCurrentCampaignMission();

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
                                int alien = Globals::status->inAlienOrbit();
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
                            if ((char) reinterpret_cast<int &>(((KIPlayer *) (intptr_t) e)->field_0x40) != 0 &&
                                ((PlayerFixedObject *) (intptr_t) e)->getDockingType() == 3) {
                                Station *st = (Station *) Globals::status->getStation();
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
                    if (o != e && ((KIPlayer *) (intptr_t) o)->stealFlagByte == 0 &&
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
                ArraySetLength(1, arr);
            }
            arr[arr.size() - 1] = (Player *) this->player->player;
        }

        ((KIPlayer *) (intptr_t) e)->player->addEnemies(&arr);

        Globals::status->getMission();
        if (eFaction == 10 && ((Mission *) Globals::status->getMission())->isEmpty() != 0)
            ((KIPlayer *) (intptr_t) e)->player->setEnemy((Player *) this->player->player);
    }
}

void Level::enemyDied(int r1, bool r2arg) {
    Level *thisptr = this;
    int r2 = (int) r2arg;
    (void) r1;

    this->enemiesLeft = this->enemiesLeft - 1;
    this->kills = this->kills + 1;

    if (r2 != 0) {
        this->killCountA = this->killCountA + 1;
        return;
    }

    Status **statusHolder = &Globals::status;
    (*statusHolder)->incKills();
    this->killCountB = this->killCountB + 1;
    if (this->player == nullptr)
        return;

    if ((static_cast<Radar *>(this->player->field_0x14))->hasScanner() == 0) {
        Achievements **achA = &Globals::achievements;
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
        Achievements **achB = &Globals::achievements;
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

struct RMSpec {
    int id, speaker, kind, delay;
};

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
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(0x17, *(this->messages));
                for (unsigned i = 0; i < 0x17; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 1: {
            static const RMSpec t[] = {{0x6a1, 2, 5, 10000}, {0x6a2, 2, 6, 0}, {0x6a3, 2, 6, 1}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(3, *(this->messages));
                for (unsigned i = 0; i < 3; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 7: {
            static const RMSpec t[] = {{0x6dc, 2, 0x10, 0}, {0x6dd, 0, 6, 0}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(2, *(this->messages));
                for (unsigned i = 0; i < 2; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x10: {
            static const RMSpec t[] = {
                {0x72e, 0x13, 5, 10000}, {0x72f, 0, 6, 0}, {0x730, 0, 9, 0},
                {0x731, 1, 6, 2}, {0x732, 0, 6, 3}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(5, *(this->messages));
                for (unsigned i = 0; i < 5; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x15: {
            static const RMSpec t[] = {
                {0x759, 10, 0x10, 0}, {0x75a, 0, 6, 0}, {0x75b, 10, 0x19, 2},
                {0x75c, 0xe, 8, 0}, {0x75d, 0xe, 0x15, 0}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(5, *(this->messages));
                for (unsigned i = 0; i < 5; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x18: {
            static const RMSpec t[] = {
                {0x77d, 0x13, 5, 12000}, {0x77e, 6, 6, 0}, {0x77f, 0, 6, 1},
                {0x780, 6, 0x16, 3}, {0x781, 6, 6, 3}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(5, *(this->messages));
                for (unsigned i = 0; i < 5; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x19: {
            static const RMSpec t[] = {{0x785, 0, 5, 20000}, {0x786, 6, 6, 0}, {0x787, 0, 6, 1}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(3, *(this->messages));
                for (unsigned i = 0; i < 3; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x26: {
            static const RMSpec t[] = {{0x7ed, 0x15, 5, 15000}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(1, *(this->messages));
                for (unsigned i = 0; i < 1; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x28: {
            static const RMSpec t[] = {
                {0x7fa, 0, 5, 10000}, {0x7fb, 8, 6, 0}, {0x7fc, 0, 6, 1}, {0x7fd, 7, 5, 40000},
                {0x7fe, 0, 6, 3}, {0x7ff, 7, 0xc, 0}, {0x800, 0, 0x18, 0}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(7, *(this->messages));
                for (unsigned i = 0; i < 7; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x29: {
            RMSpec t[] = {
                {0x804, 0, 5, 80000}, {0x805, 7, 6, 0}, {0x806, 0, 6, 1},
                {0x807, 7, 6, 2}, {0x808, 7, 0x1a, -100000}, {0x809, 7, 6, 4},
                {0x80f, 0, 1, 0}, {0x810, 0, 6, 6}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(8, *(this->messages));
                for (unsigned i = 0; i < 8; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x31: {
            static const RMSpec t[] = {{0x848, 0, 5, 8000}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(1, *(this->messages));
                for (unsigned i = 0; i < 1; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x32: {
            static const RMSpec t[] = {{0x849, 0x3f, 5, 8000}, {0x84a, 0, 6, 0}, {0x84b, 0x3f, 6, 1}, {0x84c, 0, 6, 2}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(4, *(this->messages));
                for (unsigned i = 0; i < 4; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x33: {
            static const RMSpec t[] = {{0x84d, 0x3f, 5, 8000}, {0x84e, 0, 6, 0}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(2, *(this->messages));
                for (unsigned i = 0; i < 2; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x34: {
            static const RMSpec t[] = {{0x84f, 0x3f, 5, 8000}, {0x850, 0, 6, 0}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(2, *(this->messages));
                for (unsigned i = 0; i < 2; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x36: {
            static const RMSpec t[] = {{0x851, 0, 5, 8000}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(1, *(this->messages));
                for (unsigned i = 0; i < 1; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x37: {
            static const RMSpec t[] = {{0x85a, 0, 5, 8000}, {0x85b, 0, 6, 0}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(2, *(this->messages));
                for (unsigned i = 0; i < 2; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x38: {
            static const RMSpec t[] = {
                {0x86a, 0x1b, 5, 8000}, {0x86b, 0, 6, 0}, {0x86c, 0x1c, 0x10, 0},
                {0x86d, 0, 0x14, 3}, {0x86e, 0, 6, 3}, {0x86f, 0x1b, 0x1c, 0}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(6, *(this->messages));
                for (unsigned i = 0; i < 6; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x40: {
            static const RMSpec t[] = {
                {0x8b4, 0, 5, 8000}, {0x8b5, 0x14, 6, 0}, {0x8b6, 0, 6, 1}, {0x8b7, 0x1e, 6, 2},
                {0x8b8, 0, 6, 3}, {0x8b9, 0x1e, 0x14, 5}, {0x8ba, 0, 6, 5}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(7, *(this->messages));
                for (unsigned i = 0; i < 7; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x41: {
            static const RMSpec t[] = {{0x8cb, 0, 5, 12000}, {0x8cc, 0x14, 6, 0}, {0x8cd, 0, 6, 1}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(3, *(this->messages));
                for (unsigned i = 0; i < 3; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x43: {
            static const RMSpec t[] = {
                {0x8ef, 0, 0x10, 0}, {0x8f0, 0x1f, 6, 0}, {0x8f1, 0x1e, 6, 1},
                {0x8f2, 0, 0x14, 2}, {0x8f3, 0x1f, 6, 3}, {0x8f4, 0, 6, 4}, {0x8f5, 0, 0x14, 8},
                {0x8f6, 0x1f, 6, 6}, {0x8f7, 0, 6, 7}, {0x8f8, 0x1f, 6, 8}, {0x8f9, 0, 6, 9},
                {0x8fa, 0x1f, 6, 10}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(0xc, *(this->messages));
                for (unsigned i = 0; i < 0xc; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x45: {
            static const RMSpec t[] = {{0x90e, 0, 5, 8000}, {0x90f, 0, 6, 0}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(2, *(this->messages));
                for (unsigned i = 0; i < 2; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x46: {
            static const RMSpec t[] = {{0x910, 0, 5, 8000}, {0x911, 0, 6, 0}, {0x912, 0x22, 6, 1}, {0x913, 0, 6, 2}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(4, *(this->messages));
                for (unsigned i = 0; i < 4; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x49: {
            static const RMSpec t[] = {
                {0x92b, 0, 5, 8000}, {0x92c, 0, 0x10, 0}, {0x92d, 0xb, 6, 1}, {0x92e, 0, 6, 2},
                {0x92f, 0, 0x1b, 1}, {0x930, 0x21, 0x1b, 2}, {0x931, 0, 0x1b, 3},
                {0x932, 0, 6, 6}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(8, *(this->messages));
                for (unsigned i = 0; i < 8; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x77: {
            static const RMSpec t[] = {{0xab9, 0x11, 0x1b, 1}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(1, *(this->messages));
                for (unsigned i = 0; i < 1; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x78: {
            static const RMSpec t[] = {{0xac5, 0, 5, 0x5dc}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(1, *(this->messages));
                for (unsigned i = 0; i < 1; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x83: {
            static const RMSpec t[] = {{0xb2b, 0, 0x1b, 2}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(1, *(this->messages));
                for (unsigned i = 0; i < 1; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x85: {
            static const RMSpec t[] = {{0xb33, 0x11, 0x1b, 1}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(1, *(this->messages));
                for (unsigned i = 0; i < 1; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x87: {
            static const RMSpec t[] = {
                {0xb43, 0x31, 0x1b, 1}, {0xb44, 0, 6, 0}, {0xb45, 0, 0x1b, 2},
                {0xb46, 0, 0x1b, 3}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(4, *(this->messages));
                for (unsigned i = 0; i < 4; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x89: {
            static const RMSpec t[] = {
                {0xb4f, 0x32, 5, 0x5dc}, {0xb50, 0, 6, 0}, {0xb51, 0x32, 6, 1},
                {0xb52, 0, 6, 2}, {0xb53, 0x32, 6, 3}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(5, *(this->messages));
                for (unsigned i = 0; i < 5; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x90: {
            static const RMSpec t[] = {
                {0xb98, 0x27, 5, 7000}, {0xb99, 0x27, 6, 0}, {0xb9a, 0, 6, 1},
                {0xb9b, 0x27, 6, 2}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(4, *(this->messages));
                for (unsigned i = 0; i < 4; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x91: {
            static const RMSpec t[] = {{0xb9c, 0, 5, 7000}, {0xb9d, 0x27, 6, 0}, {0xb9e, 0x27, 0x1b, 5}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(3, *(this->messages));
                for (unsigned i = 0; i < 3; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x93: {
            static const RMSpec t[] = {{0xbac, 0, 5, 7000}, {0xbad, 0, 6, 0}};
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(2, *(this->messages));
                for (unsigned i = 0; i < 2; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
            break;
        }
        case 0x50: {
            static const RMSpec t[] = {
                {0x96e, 0, 5, 8000}, {0x96f, 6, 5, 25000}, {0x970, 0x1a, 6, 1},
                {0x971, 0, 6, 2}, {0x972, 6, 6, 3}
            };
            {
                this->messages = new Array<RadioMessage *>();
                ArraySetLength(5, *(this->messages));
                for (unsigned i = 0; i < 5; i = i + 1) {
                    RadioMessage *m = (RadioMessage *) ::operator new(0x28);
                    new(m) RadioMessage(t[i].id, t[i].speaker, t[i].kind, t[i].delay);
                    (*this->messages)[i] = m;
                }
            }
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

void Level::createPlayer() {
    Ship *ship = (Ship *) Globals::status->getShip();
    Array<Item *> *equip = ship->getEquipment();

    int slots[3];
    slots[0] = ship->getUsedSlots(0);
    slots[1] = ship->getUsedSlots(1);
    slots[2] = ship->getUsedSlots(2);

    Player *pl = (Player *) ::operator new(0x114);
    new(pl) Player(0x4b0, ship->getMaxHP(), slots[0], slots[1], slots[2]);
    pl->setMaxShieldHP(ship->getMaxShieldHP());
    pl->setMaxArmorHP(ship->getMaxArmorHP());
    *((uint8_t *) pl + 0x69) = 1;

    PlayerEgo *ego = (PlayerEgo *) ::operator new(0x3a0);
    new(ego) PlayerEgo(pl);
    this->player = ego;
    ego->setShip(ship->getIndex(), ship->getRace());
    ego->setLevel(this);

    float yaw = (float) this->field_138 * (1.0f / 65536.0f) * 6.2831855f;
    Vector rot = {0.0f, 0.0f, yaw};
    this->player->geometry->setRotation(rot);

    Array<Array<Gun *> *> *buckets = new Array<Array<Gun *> *>();
    ArraySetLength(3, *buckets);
    for (int t = 0; t < 3; t++) {
        if (slots[t] > 0) {
            Array<Gun *> *b = new Array<Gun *>();
            (*buckets)[t] = b;
            ArraySetLength(slots[t], *b);
        }
    }

    if (equip != nullptr) {
        FileRead *fr = new FileRead();
        Array<Array<Vector *> *> *wpos = fr->loadWeaponPositions(ship->getIndex());
        delete fr;

        if ((*wpos)[3] != nullptr) {
            this->field_a4 = new Array<AEGeometry *>();
            for (unsigned k = 0; k < (*wpos)[3]->size(); k += 2) {
                AEGeometry *g = new AEGeometry(Globals::Canvas);
                ArrayAdd(g, *(this->field_a4));
                g->setPosition(*(*(*wpos)[3])[k]);
                g->setScaling(*(*(*wpos)[3])[k + 1]);
            }
        }

        for (unsigned i = 0; i < equip->size(); i++) {
            Item *it = (*equip)[i];
            if (it == nullptr || !it->isWeapon())
                continue;

            int amount = (it->getType() == 1) ? it->getAmount() : -1;
            int dmg = it->getAttribute(0x9);
            int rate = it->getAttribute(0xb);

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
                (*(*buckets)[2])[--slots[2]] = gun;
                Vector *pos = (*(*wpos)[2])[ship->getSlotPos(it)];
                ego->setTurretPosition(*pos);
            } else if (type == 0 || type == 1) {
                (*(*buckets)[type])[--slots[type]] = gun;
                gun->setOffset((*(*wpos)[type])[ship->getSlotPos(it)]);
            }
        }

        for (unsigned k = 0; k < wpos->size(); k++) {
            if ((*wpos)[k] != nullptr) {
                for (Vector *v: *(*wpos)[k])
                    delete v;
                delete (*wpos)[k];
            }
        }
        delete wpos;
    }

    for (unsigned t = 0; t < buckets->size(); t++) {
        if ((*buckets)[t] != nullptr)
            this->player->addGun((*buckets)[t], (int) t);
    }
}

void Level::wingmanDied(AbyssEngine::String const &name) {
    unsigned int *list = (unsigned int *) (intptr_t) Globals::status->getWingmen();
    if (list == 0) {
        return;
    }
    if (__builtin_expect(*list < 2, 0)) {
        return Globals::status->setWingmen((Array<String *> *) 0);
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

static inline void zero16(unsigned char *p) {
    ((int *) p)[0] = 0;
    ((int *) p)[1] = 0;
    ((int *) p)[2] = 0;
    ((int *) p)[3] = 0;
}

Level::Level(int mission) {
    void(*ctor)(Matrix *) = *g_levelSubCtor;
    field_b4 = 0;
    field_b8 = 0;
    field_bc = 0;
    field_c8 = 0;
    field_cc = 0;
    field_d0 = 0;
    field_18c = 0;
    field_190 = 0;
    field_194 = 0;
    ctor(&this->skyMatrix);
    ctor(&this->cloudMatrix);
    ctor(&this->reversalMatrix);
    zero16((unsigned char *) &field_1c);
    zero16((unsigned char *) &this->field_90);
    zero16((unsigned char *) &this->particleRenderBoolPtr);
    zero16((unsigned char *) &this->field_74);
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

    objectivesA = nullptr;
    objectivesB = nullptr;
    memset(&this->asteroidWaypoint, 0, 0x65);
    zero16((unsigned char *) &this->hostileCount);
    zero16((unsigned char *) &this->flashType);
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

void Level::createStaticObjects() {
    if (Globals::status->inAlienOrbit() == 0) {
        Station *st = (Station *) Globals::status->getStation();
        if (((Station *) st)->getIndex() == 0x70 &&
            0x7f < Globals::status->getCurrentCampaignMission()) {
            int type;
            if (Globals::status->getCurrentCampaignMission() < 0x83) type = 0x493e;
            else if (Globals::status->getCurrentCampaignMission() < 0x87) type = 0x4941;
            else if (Globals::status->getCurrentCampaignMission() < 0x8a) type = 0x4944;
            else if (Globals::status->getCurrentCampaignMission() < 0x8e) type = 0x4947;
            else if (0x91 < Globals::status->getCurrentCampaignMission()) type = -1;
            else type = 0x494a;

            if (type != -1) {
                KIPlayer *o = (KIPlayer *) this->createStaticObject((Waypoint *) (intptr_t) 0, type, 0);
                ((PlayerFixedObject *) o)->setPosition(g_cso_posX, 0, g_cso_posZ);
                ((PlayerFixedObject *) o)->setMoving(0);
                AEGeometry *geo = o->geometry;
                o->field_0x70 = 0;

                Vector dir = this->starSystem->getLightDirection();

                Vector up = {0.0f, 1.0f, 0.0f};
                geo->setDirection(dir, up);
                String *txt = Globals::gameText->getText(**g_cso_textA);
                o->name = *txt;
                o->player->setAlwaysFriend(1);
                if (this->enemies == nullptr) {
                    this->enemies = new Array<KIPlayer *>();
                }
                ArrayAdd(o, *(this->enemies));
                if (o->cargo != 0)
                    delete o->cargo;
                o->cargo = 0;
            }
        }
    }

    if (0x54 < Globals::status->getCurrentCampaignMission() &&
        Globals::status->getCurrentCampaignMission() != 0x87 &&
        Globals::status->inAlienOrbit() == 0) {
        Station *st = (Station *) Globals::status->getStation();
        if (((Station *) st)->getIndex() == 0x67) {
            KIPlayer *o = (KIPlayer *) this->createStaticObject((Waypoint *) (intptr_t) 0, 0x4a88, 0);
            ((PlayerFixedObject *) o)->setPosition(0, 0, 0);
            ((PlayerFixedObject *) o)->setMoving(0);
            o->field_0x70 = 1;
            String *txt = Globals::gameText->getText(**g_cso_textB);
            String name;
            name.Set((txt)->data);
            ((PlayerFixedObject *) o)->setName(name);
            ((PlayerFixedObject *) o)->setDockingType(1);
            if (o->cargo != 0)
                delete o->cargo;
            o->cargo = 0;
            o->player->setAlwaysFriend(1);
            if (this->enemies == nullptr) {
                this->enemies = new Array<KIPlayer *>();
            }
            ArrayAdd(o, *(this->enemies));
        }
    }
}

int Level::createStaticObject(Waypoint *wp, int type, bool jitter) {
    Level * thisptr = this;

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = wp->x;
        y = wp->y;
        z = wp->z;
    }
    if (jitter) {
        AbyssEngine::AERandom *rng = Globals::rnd;
        x = x + rng->nextInt(20000) - 10000;
        y = y + rng->nextInt(20000) - 10000;
        z = z + rng->nextInt(20000) - 10000;
    }

    if (type == 0x4215) {
        int r = Globals::rnd->nextInt();
        type = (r == 0) ? 0x4215 : (r == 1) ? 0x4216 : 0x4217;
    }

    (void) type;
    (void) x;
    (void) y;
    (void) z;
    (void) thisptr;
    return 0;
}

void *Level::getBoundingVolume(int /*unused*/, AEGeometry *kind) {
    // lint: void_ptr (method return type is symbol-baked)
    int index = (int) (intptr_t) kind;
    FileRead *fr = (FileRead *) ::operator new(1);
    new(fr) FileRead();

    int *coll;
    if (index < 2000)
        coll = (int *) fr->loadStationCollision(index);
    else
        coll = (int *) fr->loadStaticCollision(index);
    (fr->~FileRead(), ::operator delete(fr));

    Array<BoundingVolume *> *result = nullptr;
    if (coll != 0) {
        unsigned n = *(unsigned *) coll[1];
        Array<BoundingVolume *> *arr = new Array<BoundingVolume *>();
        ArraySetLength(n, *arr);
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

        ::operator delete((int *) (intptr_t) coll[1]);
        ::operator delete(coll);
    }
    return result;
}

PlayerFixedObject *Level::createShip(int race, int shipClass, int type, Waypoint *wp, bool hostile, bool group) {
    Level * thisptr = this;
    int camp = Globals::status->getCurrentCampaignMission();

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = wp->x;
        y = wp->y;
        z = wp->z;
    }
    AbyssEngine::AERandom *rng = Globals::rnd;
    int jx = rng->nextInt(40000);
    int jy = rng->nextInt(40000);
    int jz = rng->nextInt(40000);

    int lvl = Globals::status->getLevel();
    if (0x15 <= lvl) lvl = 0x14;
    int ramp = (Globals::status->gameWon() != 0) ? 0xb4 : (camp << 2);
    int hp = ramp + lvl * 0xe + 0x14;
    if (type == 0x33) hp = (int) ((float) hp * 1.7f);
    else if (type == 0x31) hp = (int) ((float) hp * 17.0f);
    else if (type == 0x2c) hp = (int) ((float) hp * 2.25f);
    if ((unsigned) (camp - 0x31) < 8 && (0x8fU >> ((unsigned) (camp - 0x31) & 0xff) & 1) != 0)
        hp = 0x10e;

    int dmgLvl = Globals::status->getLevel();
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
    hp = (int) ((float) hp + (((DifficultyRecord *) (intptr_t) * g_cs_diffRec)->hpScale - 0.5f) * (float) hp);
    if (camp == 0x9a) {
        int alien = Globals::status->inAlienOrbit();
        hp = hp << (alien & (race == 9));
    }

    Player *pl = (Player *) ::operator new(0x114);
    int playerHp = (Globals::status->hardCoreMode() != 0) ? 0x28a : 1000;
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
        AEGeometry *gg = Globals::globals->getShipGroup(type, race, group);
        obj->setShipGroup(gg, type, hostile != 0);
        if (this->missionPtr != 1 && this->missionPtr != 0x17) {
            AEGeometry *g = obj->parentGeometry;
            if (g == 0) g = obj->geometry;
            this->lodManager->addObject(g);
        }
        if (type == 0x2c || type == 0x31 || type == 0x33) {
            if (type == 0x33) obj->field_0x25 = 0;
            if (type != 0x33 && obj->cargo != 0) {
                delete obj->cargo;
                obj->cargo = 0;
            }
        }
    } else if (shipClass == 1) {
        obj = (PlayerFixedObject *) ::operator new(0x1bc);
        new(obj) PlayerFixedObject(type, race, pl, 0, fx, fy, fz);

        int wreck = -1;
        BoundingVolume *bv = nullptr;
        obj->setWreckedMeshId(wreck);
        obj->setBV(bv);
        AEGeometry *gg = Globals::globals->getShipGroup(type, race, 0);
        obj->setShipGroup(gg, type, false);
        this->lodManager->addObject(obj->geometry);
        *(unsigned char *) &obj->field_0x40 = 1;
    }

    if (obj != 0)

        obj->setLevel(thisptr);
    return obj;
}

void Level::almostKillWanted(int index) {
    if (field_29e != 0) {
        return;
    }
    field_29e = 1;
    if (Globals::status->isStorylineWanted(index) == 0) {
        return;
    }
    int m = (int) (intptr_t) ::operator new(0x78);
    new((Mission *) (intptr_t) m) Mission(4, 0, Globals::status->getStation()->getIndex());
    ((Mission *) (m))->setCampaignMission(true);
    ((Mission *) (m))->setWon(1);
    Globals::status->setMission((Mission *) (intptr_t) m);
    Globals::status->setCampaignMission((Mission *) (intptr_t) m);
    delete objectivesA;
    objectivesA = nullptr;
    objectivesA = new Objective(3, 0, 0, this);
    KIPlayer *e = (*this->enemies)[0];
    e->player->setAlwaysEnemy(0);
    ((Player *) (int) (intptr_t)(*this->enemies)[1])->resetDamageDoneByPlayer();
    e->player->enemyFlagsLo = 0;
    e->reviveLockFlag = 1;
    Array<Wanted *> *w = Globals::status->getWanted();
    return w->data_[index]->setActive(0 != 0);
}

void Level::assignGuns() {
    if (this->enemyGuns != nullptr) {
        if (this->enemyGuns) {
            ArrayReleaseClasses(*this->enemyGuns);
            delete this->enemyGuns;
            this->enemyGuns = nullptr;
        }
    }
    this->enemyGuns = nullptr;

    float lvlPower = (float) (Globals::status->getLevel() - 2) * g_ag_perLevel;
    if (lvlPower >= 20.0f) lvlPower = 20.0f;
    if (lvlPower < 0.0f) lvlPower = (float) (Globals::status->getLevel() - 2) * g_ag_perLevel;

    float diffScale = ((DifficultyRecord *) (intptr_t) * g_ag_diffRec)->hpScale;
    int camp = Globals::status->getCurrentCampaignMission();
    int basePower = (int) (lvlPower + lvlPower * (diffScale - 0.5f));
    Wanted *wanted = (Wanted *) Globals::status->getWantedInCurrentOrbit();
    if (0x15 < basePower) basePower = 0x16;
    if (this->enemies == nullptr)
        return;

    unsigned slots = 0;
    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
        int e = (int) (intptr_t)(*this->enemies)[i];
        if (e != 0 && (char) ((KIPlayer *) (intptr_t) e)->field_0x25 != 0) {
            int add = (((KIPlayer *) e)->isWingMan() != 0) ? 2 : 1;
            slots = slots + add;
        }
    }

    this->enemyGuns = new Array<AbstractGun *>();
    ArraySetLength(slots, *(this->enemyGuns));

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
            Globals::status->getMission();
            if (((Mission *) Globals::status->getMission())->getType() == 6 && ((KIPlayer *) (intptr_t) e)->player->
                isAlwaysFriend() == 0) {
                color = 0x41e00000;
                dmg = Globals::status->getLevel() + baseDmg;
            } else {
                Globals::status->getMission();
                dmg = baseDmg;
                if (((Mission *) Globals::status->getMission())->getType() == 0xc && ((KIPlayer *) (intptr_t) e)->player
                    ->
                    isAlwaysFriend() != 0) {
                    color = 0x41e00000;
                    dmg = Globals::status->getLevel() + baseDmg;
                }
            }

            if (((KIPlayer *) e)->isWingMan() == 0 && ((KIPlayer *) (intptr_t) e)->player->isAlwaysFriend() == 0 &&
                (*this->enemies)[i] &&
                (*this->enemies)[i]->shipGroup == 9) {
                if (camp != 0x10) dmg = (int) ((float) dmg * 0.8f);
                else dmg = dmg + dmg;
            } else {
                int hard = ((0x8fU >> ((unsigned) (camp - 0x31) & 0xff) & 1) != 0 &&
                            (unsigned) (camp - 0x31) <= 7)
                               ? 5
                               : dmg;
                dmg = hard;
            }

            if (camp == 0x50 && ((PlayerTurret *) (*this->enemies)[i])->field_0x3e != 0)
                dmg = (int) ((float) dmg * 1.7f);

            if (camp == 0x46 && ((KIPlayer *) e)->isWingMan() == 0)
                dmg = (int) ((float) dmg * 2.5f);

            if (Globals::status->getMission() != 0) {
                Globals::status->getMission();
                if (((Mission *) Globals::status->getMission())->getType() == 0xb7) dmg = 1;
            }

            Gun *gun = (Gun *) ::operator new(0x114);
            int won = Globals::status->gameWon();
            int rampMis = (won != 0) ? 0x2d : Globals::status->getCurrentCampaignMission();
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
                    gun->damage = (int) ((float) gun->damage * 0.7f);
                    break;
                default: gun->weaponType = ITEM_SORT_BLASTER;
                    gun->setIndex(0x13);
                    res = 0x1a8b;
                    break;
            }

            int camp2 = Globals::status->getCurrentCampaignMission();
            PlayerTurret *turret = (PlayerTurret *) (*this->enemies)[i];
            if (turret->field_0x3e != 0) {
                int host = (int) (intptr_t) turret->getHost();
                if (host != 0 && (((KIPlayer *) (intptr_t) host)->shipGroupFlag == 0x2d || ((KIPlayer *) (intptr_t)
                                      host)->shipGroupFlag == 0x33)) {
                    gun->weaponType = ITEM_SORT_AUTO_CANNON;
                    gun->setIndex(0x16);
                    res = 0x1a8e;
                    gun->damage = (int) ((double) gun->damage * 0.5);
                } else {
                    KIPlayer *k = (*this->enemies)[i];
                    if ((uint8_t) k->field_0x40 == 0) {
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
                        } else if (kt == 0x49c0) {
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
                        }
                        gun->field_0xa8 = 1;

                        Item *stat = (Item *) (intptr_t)(*(int *) (*(int *) (*g_ag_shipTbl + 4) + statRow * 4));
                        gun->damage = stat->getAttribute(0x9);
                        gun->fireDelay = stat->getAttribute(0xb);
                        gun->initialLifetime = stat->getAttribute(0xc);
                        gun->pitchRate = (float) stat->getAttribute(0xd);
                        if (camp2 == 0x9e && kt == 0x49c2 && ((KIPlayer *) (intptr_t) e)->player->isAlwaysEnemy() !=
                            0) {
                            gun->pitchRate = gun->pitchRate * 1.2f;
                            gun->damage = (int) ((float) gun->damage * 1.5f);
                            Player *pp = (*this->enemies)[i]->player;
                            int mhp = pp->getMaxHitpoints();
                            pp->setMaxHitpoints((int) ((float) mhp * 5.0f));
                        }
                    }
                }
            }

            if (camp2 == 7) {
                if ((*this->enemies)[i]->shipGroup == 8)
                    gun->damage = (int) ((float) gun->damage * 0.5f);
            } else if (camp2 == 0x46 && ((KIPlayer *) e)->isWingMan() == 0) {
                gun->setIndex(0xb7);
                res = 0x37d9;
            }

            if (Globals::status->getMission() != 0 && ((Mission *) Globals::status->getMission())->isCampaignMission()
                != 0) {
                if (**g_ag_statusB == Globals::status->getCurrentCampaignMission() &&
                    2 < **g_ag_alienCnt &&
                    ((*this->enemies)[i])->isEnemy() != 0)
                    gun->damage = (int) ((float) gun->damage * 0.7f);
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

            int sc = gun->weaponType;
            if (sc == ITEM_SORT_CLUSTER_MISSILE || sc == ITEM_SORT_MISSILE) {
                RocketGun *r = (RocketGun *) ::operator new(0xe8);
                new(r) RocketGun(gun->itemIndex, gun, res, 0, 0, sc,
                                 sc == ITEM_SORT_MISSILE ? 1 : 0, this);
                (*this->enemyGuns)[outIdx] = (ObjectGun *) r;
                gun->pitchRate = 8.0f;
                gun->initialLifetime = 10000;
                gun->fireDelay = 3000;
                gun->damage = gun->damage << 2;
            } else {
                ObjectGun *o = (ObjectGun *) ::operator new(0xb0);
                new(o) ObjectGun(0, gun, res, 0x2711, this);
                (*this->enemyGuns)[outIdx] = o;
            }
            ((KIPlayer *) (*this->enemies)[i])->addGun((Gun *) gun, 0);

            Globals::globals
                    ->addSoundResourceToList((*this->enemies)[i]->shipGroup == 9 ? 0x3e : 0x3d);

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
                ArrayAdd((AbstractGun *) r2, *(this->enemyGuns));
                ((KIPlayer *) (*this->enemies)[i])->addGun((Gun *) gun2, 0);
                Globals::globals->addSoundResourceToList(0x54);
            }

            if ((unsigned) (camp2 - 0x9d) < 2 && ((KIPlayer *) (*this->enemies)[i])->shipGroupFlag == 0x31) {
                Gun *gun3 = (Gun *) ::operator new(0x114);
                int won3 = Globals::status->gameWon();
                int rampMis3 = (won3 != 0) ? 0x2d : Globals::status->getCurrentCampaignMission();
                new(gun3) Gun(0, dmg, 4, -1, 3000, rampMis3 * -2 + 600, (float) color,
                              Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
                gun3->setFriendGun(1);
                gun3->setLevel(this);
                gun3->weaponType = ITEM_SORT_CLUSTER_MISSILE;
                gun3->setIndex(0xd6);
                RocketGun *r3 = (RocketGun *) ::operator new(0xe8);
                new(r3) RocketGun(gun3->itemIndex, gun3, 0x37a0, 0, 0, gun3->weaponType,
                                  gun3->weaponType == ITEM_SORT_MISSILE, this);
                ArrayAdd((AbstractGun *) r3, *(this->enemyGuns));
                gun3->pitchRate = 8.0f;
                gun3->initialLifetime = 10000;
                gun3->fireDelay = 3000;
                gun3->damage = gun3->damage << 2;
                ((KIPlayer *) (*this->enemies)[i])->addGun((Gun *) gun3, 0);
                Globals::globals->addSoundResourceToList(0x54);
            }

            outIdx = outIdx + 1;
        }

    wingmanExtra:
        if (((KIPlayer *) e)->isWingMan() != 0 &&
            (char) (*this->enemies)[i]->field_0x25 != 0) {
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
            int attr = ((Item *) ((ItemTable *) (intptr_t)(*(int *) (*g_ag_itemTblB + 4)))->itemTableEntry0x12)->
                    getAttribute(0xa);
            gun->empDamage = attr;
            ((KIPlayer *) (*this->enemies)[i])->addGun((Gun *) gun, 0);

            Globals::globals->addSoundResourceToList(0x4a);
            outIdx = outIdx + 1;
        }
    }
}

void Level::createGasClouds() {
    Galaxy *gal = Globals::galaxy;
    Station *st = (Station *) Globals::status->getStation();
    int *prob = (int *) gal->getPlasmaProbabilities(st);

    int ship = (int) (intptr_t) Globals::status->getShip();
    if ((int) (intptr_t)((Ship *) (intptr_t) ship)->getFirstEquipmentOfSort(0x21) == 0 || Globals::status->
        inAlienOrbit() != 0)
        return;

    Globals::status->getSystem();
    if (((SolarSystem *) Globals::status->getSystem())->getIndex() != 10 && *prob == 0xcc) {
        Globals::status->getSystem();
        if (((SolarSystem *) Globals::status->getSystem())->getRoutes() == 0)
            return;
    }

    this->gasClouds = new Array<KIPlayer *>();

    bool boss = false;
    if (Globals::status->getCurrentCampaignMission() == 0x8e) {
        Station *s2 = (Station *) Globals::status->getStation();
        boss = ((Station *) s2)->getIndex() == 0x4f;
    }

    AbyssEngine::AERandom *rng = Globals::rnd;
    int roll = rng->nextInt();

    float countF = (float) (boss ? 3.0f : 0.0f) + ((float) prob[1] / 1.0f) * (float) (roll + 4);
    int count = (countF > 0.0f) ? (int) countF : 0;
    ArraySetLength((unsigned) count, *(this->gasClouds));

    PaintCanvas *canvas = Globals::Canvas;
    for (unsigned i = 0; i < this->gasClouds->size(); i = i + 1) {
        int kind = *prob;
        Vector pos;
        levelCloudRandomPos(this, (int) (intptr_t) rng, boss, i, &pos);

        AEGeometry *geo = (AEGeometry *) ::operator new(0xc0);
        new(geo) AEGeometry((uint16_t) 0x37d1, canvas, 0);
        PlayerGasCloud *cloud = (PlayerGasCloud *) ::operator new(0x16c);
        new(cloud) PlayerGasCloud(kind, this->field_94, geo, pos);
        (*this->gasClouds)[i] = cloud;

        (*this->gasClouds)[i]->setLevel(this);
    }
}

void Level::updateMissionOrbit(int dt) {
    if (this->field_288 != 0) {
        Globals::status->getMission();
        if (((Mission *) Globals::status->getMission())->isEmpty() == 0) {
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
                        if (((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0) {
                            AbyssEngine::AERandom *rng = Globals::rnd;
                            Vector p = (this->player == 0 ? Vector{0.0f, 0.0f, 0.0f} : this->player->getPosition());
                            int span = 0 ? 40000 : 120000;
                            float ox = (float) (rng->nextInt() % span - span / 2);
                            float oy = (float) (rng->nextInt() % (span * 2 / 3) - span / 3);
                            float oz = (float) (rng->nextInt() % span - span / 2);
                            ((KIPlayer *) ki)->setPosition(p.x + ox, p.y + oy, p.z + oz);
                        }
                    }
                }
            }
        }
    }

    if (Globals::status->getMission() != 0) {
        Globals::status->getMission();
        if (((Mission *) Globals::status->getMission())->getType() == 0xb7) {
            int t = this->orbitWaveTimer;
            this->orbitWaveTimer = t + dt;
            if (0x1d4c < t + dt) {
                this->orbitWaveTimer = 0;
                if (this->enemies != nullptr)
                    for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                        int *ki = (int *) (*this->enemies)[i];
                        if (((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0 &&
                            ((KIPlayer *) ki)->shipGroupFlag != 0x33) {
                            AbyssEngine::AERandom *rng = Globals::rnd;
                            Vector p = (this->player == 0 ? Vector{0.0f, 0.0f, 0.0f} : this->player->getPosition());
                            int span = 1 ? 40000 : 120000;
                            float ox = (float) (rng->nextInt() % span - span / 2);
                            float oy = (float) (rng->nextInt() % (span * 2 / 3) - span / 3);
                            float oz = (float) (rng->nextInt() % span - span / 2);
                            ((KIPlayer *) ki)->setPosition(p.x + ox, p.y + oy, p.z + oz);
                            ((KIPlayer *) ki)->cargo = nullptr;
                        }
                    }
            }
        }
    }

    Globals::status->getMission();
    if (((Mission *) Globals::status->getMission())->isEmpty() == 0) {
        Globals::status->getMission();
        if (((Mission *) Globals::status->getMission())->getType() == 0xf) {
            int t = this->orbitWaveTimer;
            this->orbitWaveTimer = t + dt;
            if (50000 < t + dt && this->enemies != nullptr) {
                this->orbitWaveTimer = 0;
                unsigned count = this->enemies->size();

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
                        if (((KIPlayer *) ki)->isDead() != 0 && ((KIPlayer *) ki)->player->isActive() == 0) {
                            AbyssEngine::AERandom *rng = Globals::rnd;
                            Vector p = (this->player == 0 ? Vector{0.0f, 0.0f, 0.0f} : this->player->getPosition());
                            int span = 0 ? 40000 : 120000;
                            float ox = (float) (rng->nextInt() % span - span / 2);
                            float oy = (float) (rng->nextInt() % (span * 2 / 3) - span / 3);
                            float oz = (float) (rng->nextInt() % span - span / 2);
                            ((KIPlayer *) ki)->setPosition(p.x + ox, p.y + oy, p.z + oz);
                        }
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
        int **slot = (int **) &Globals::status;

        Wanted *wanted = (Wanted *) (intptr_t)(((int *) (*(int *) ((*(int *) *slot) + 4)))[index]);
        int numWingmen = (wanted != nullptr) ? wanted->getNumWingmen() : 0;
        for (int i = 1; i - 1 < numWingmen; i = i + 1) {
            (*enemies)[i]->player->setAlwaysEnemy(1);
            (*enemies)[i]->player->turnEnemy();
        }
    }
}

void Level::initParticleSystems() {
    if (this->player != nullptr) {
        if (this->field_a4 != nullptr) {
            this->field_a8 = new Array<int>();
            ArraySetLength(this->field_a4->size(), *(this->field_a8));
        }

        PaintCanvas *canvas = Globals::Canvas;
        canvas->CameraGetCurrent();
        Matrix *local = CameraGetLocal(canvas, canvas->CameraGetCurrent());
        int sys = (this->skybox2Mesh)->addSystem(local, ParticleSettings::ParticleSet_4, false);
        this->movingStarsIndex = sys;

        if (Globals::status->getSystem() != 0) {
            SolarSystem *ss = (SolarSystem *) Globals::status->getSystem();
            if (((SolarSystem *) ss)->hasPirateBase() != 0 && this->enemies != nullptr) {
                for (unsigned i = 0; i < this->enemies->size(); i = i + 1) {
                    KIPlayer *k = (*this->enemies)[i];
                    if (k != 0 && k->getType() == 0x37a3) {
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
    }

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

    {
        ParticleSystemManager *mgr = this->particleSystemMgr;
        if (mgr == nullptr) {
            this->field_38 = -1;
        } else {
            int sys = mgr->addSystem(0, ParticleSettings::ParticleSet_0xa, true);
            mgr->enableSystemEmit(sys, true);
            this->field_38 = sys;
        }
    }
    {
        ParticleSystemManager *mgr = this->particleSystemMgr;
        if (mgr == nullptr) {
            this->field_3c = -1;
        } else {
            int sys = mgr->addSystem(0, ParticleSettings::ParticleSet_0xb, true);
            mgr->enableSystemEmit(sys, true);
            this->field_3c = sys;
        }
    }
    {
        ParticleSystemManager *mgr = this->particleSystemMgr;
        if (mgr == nullptr) {
            this->field_48 = -1;
        } else {
            int sys = mgr->addSystem(0, ParticleSettings::ParticleSet_0x14, true);
            mgr->enableSystemEmit(sys, true);
            this->field_48 = sys;
        }
    }
    {
        ParticleSystemManager *mgr = this->particleSystemMgr;
        if (mgr == nullptr) {
            this->field_34 = -1;
        } else {
            int sys = mgr->addSystem(0, ParticleSettings::ParticleSet_0x15, true);
            mgr->enableSystemEmit(sys, true);
            this->field_34 = sys;
        }
    }
    {
        ParticleSystemManager *mgr = this->particleSystemMgr;
        if (mgr == nullptr) {
            this->field_50 = -1;
        } else {
            int sys = mgr->addSystem(0, ParticleSettings::ParticleSet_0x16, true);
            mgr->enableSystemEmit(sys, true);
            this->field_50 = sys;
        }
    }
    {
        ParticleSystemManager *mgr = this->particleSystemMgr;
        if (mgr == nullptr) {
            this->field_54 = -1;
        } else {
            int sys = mgr->addSystem(0, ParticleSettings::ParticleSet_0x17, true);
            mgr->enableSystemEmit(sys, true);
            this->field_54 = sys;
        }
    }
    if (Globals::status->getCurrentCampaignMission() == 0x50) {
        {
            ParticleSystemManager *mgr = this->particleSystemMgr;
            if (mgr == nullptr) {
                this->field_58 = -1;
            } else {
                int sys = mgr->addSystem(0, ParticleSettings::ParticleSet_0x18, true);
                mgr->enableSystemEmit(sys, true);
                this->field_58 = sys;
            }
        }
        {
            ParticleSystemManager *mgr = this->particleSystemMgr;
            if (mgr == nullptr) {
                this->field_5c = -1;
            } else {
                int sys = mgr->addSystem(0, ParticleSettings::ParticleSet_0x18, true);
                mgr->enableSystemEmit(sys, true);
                this->field_5c = sys;
            }
        }
    }

    (this->field_74)->init();
    (this->field_74)->enableSystemEmit(this->field_50, true);
    (this->field_74)->enableSystemEmit(this->field_54, true);
    if (Globals::status->getCurrentCampaignMission() == 0x50) {
        (this->field_74)->enableSystemEmit(this->field_58, true);
        (this->field_74)->enableSystemEmit(this->field_5c, true);
    }

    this->field_9c->init();
    this->particleEmitBoolPtr->init();
    this->particleRenderBoolPtr->init();
    if (this->field_94 != 0)
        this->field_94->init();
}

void Level::createWingmen() {
    if (Globals::status->inSupernovaSystem() != 0 ||
        Globals::status->getCurrentCampaignMission() == 0x9e ||
        Globals::status->getWingmen() == 0 ||
        this->player == nullptr)
        return;

    Array<KIPlayer *> *arr = new Array<KIPlayer *>();
    unsigned *wm = (unsigned *) Globals::status->getWingmen();
    ArraySetLength(*wm, *arr);

    unsigned n = arr->size();
    for (unsigned i = 0; i < n; i = i + 1) {
        int seed = **g_cwm_seedSrc;
        Globals::status->getWingmen();
        ((AbyssEngine::AERandom *) (intptr_t) seed)->setSeed((long long) seed);
        int fighter = (int) Globals::globals->getRandomEnemyFighter((int) Globals::status->fadeValue);
        int ship = (int) (intptr_t) createShip(5, 0, fighter, 0, 1, 0);
        (*arr)[i] = (KIPlayer *) (intptr_t) ship;

        int *slot = (int *) &(*arr)[i];
        levelPlaceWingman(this, slot, i);

        ((*arr)[i])->setWingman(true, i);
        (*arr)[i]->player->setAlwaysFriend(1);
        (*arr)[i]->player->setHitpoints(0x258);

        int wmList = Globals::status->getWingmen();
        (*arr)[i]->name =
                **(String **) (*(int *) (wmList + 4) + i * 4);
        (*arr)[i]->shipGroup = (int) Globals::status->fadeValue;

        Globals::status->getMission();
        if (((Mission *) Globals::status->getMission())->getType() == 0xc)
            (*arr)[i]->field_0x25 = 0;
    }

    if (this->enemies == nullptr) {
        this->enemies = arr;
    } else {
        for (unsigned i = 0; i < n; i = i + 1) {
            ArrayAdd((*arr)[i], *(this->enemies));
            n = arr->size();
        }
        delete arr;
    }
    ((AbyssEngine::AERandom *) (intptr_t) * *g_cwm_seedSrc)->reset();
}

void Level::createScene() {
    if (this->enemies != nullptr) {
        if (this->enemies) {
            ArrayReleaseClasses(*this->enemies);
            delete this->enemies;
            this->enemies = nullptr;
        }
    }
    int mode = this->missionPtr;
    this->enemies = nullptr;

    if (mode == 2) {
        createPlayer();
        Globals::status->setMission((Mission *) Globals::status->wanted);
        createMission();
        if (Globals::status->getCurrentCampaignMission() == 0x2b) {
            PaintCanvas *canvas = Globals::Canvas;
            AEGeometry *g = (AEGeometry *) ::operator new(0xc0);
            new(g) AEGeometry((uint16_t) 0x37d0, canvas, 0);
            PlayerStatic *p = (PlayerStatic *) ::operator new(0x130);
            new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            ArrayAdd((KIPlayer *) p, *(this->enemies));
            g = (AEGeometry *) ::operator new(0xc0);
            new(g) AEGeometry((uint16_t) 0x37d1, canvas, 0);
            p = (PlayerStatic *) ::operator new(0x130);
            new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            ArrayAdd((KIPlayer *) p, *(this->enemies));
        }
        return;
    }

    if (mode == 4) {
        Globals::status->getSystem();
        int race = ((SolarSystem *) Globals::status->getSystem())->getRace();
        unsigned crew = (race == 1) ? 2 : 3;
        Station *st = (Station *) Globals::status->getStation();
        int *agents = (int *) ((Station *) st)->getAgents();
        char taken[7];
        PaintCanvas *canvas = Globals::Canvas;

        if (agents == 0) {
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(3, *(this->enemies));
        } else {
            int nAgents = *agents;
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(nAgents * 3 + crew, *(this->enemies));
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
                do { seat = Globals::rnd->nextInt(); } while (taken[seat] != 0);
                taken[seat] = 1;

                AEGeometry *g = (AEGeometry *) ::operator new(0xc0);
                new(g) AEGeometry((uint16_t)(unsigned)part, canvas, 0);
                PlayerStatic *p = (PlayerStatic *) ::operator new(0x130);
                new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
                (*this->enemies)[a] = (KIPlayer *) p;

                g = (AEGeometry *) ::operator new(0xc0);
                new(g) AEGeometry((uint16_t)(unsigned)mode, canvas, 0);
                p = (PlayerStatic *) ::operator new(0x130);
                new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
                (*this->enemies)[nAgents + a] = (KIPlayer *) p;

                g = (AEGeometry *) ::operator new(0xc0);
                new(g) AEGeometry((uint16_t) 0x380c, canvas, 0);
                p = (PlayerStatic *) ::operator new(0x130);
                new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
                (*this->enemies)[nAgents * 2 + a] = (KIPlayer *) p;
            }
        }
        for (unsigned u = 0; u < crew; u = u + 1) {
            AEGeometry *g = (AEGeometry *) ::operator new(0xc0);
            new(g) AEGeometry((uint16_t)(unsigned)mode, canvas, 0);
            PlayerStatic *p = (PlayerStatic *) ::operator new(0x130);
            new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            (*this->enemies)[this->enemies->size() + (u - crew)] = (KIPlayer *) p;
        }
        return;
    }

    if (mode == 0x17) {
        int *host = (int *) Globals::status;
        Station *st = (Station *) Globals::status->getStation();
        unsigned race;
        if (((Station *) st)->getIndex() == 0x65) race = 8;
        else {
            st = (Station *) Globals::status->getStation();
            if (((Station *) st)->getIndex() == 100) race = 7;
            else {
                Globals::status->getSystem();
                race = ((SolarSystem *) Globals::status->getSystem())->getRace();
            }
        }

        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(1, *(this->enemies));

        (int) (intptr_t) Globals::status->getShip();
        int shipIdx = ((Ship *) (int) (intptr_t) Globals::status->getShip())->getIndex();
        Ship *ship = (Ship *) (int) (intptr_t) Globals::status->getShip();
        int shipRace = ship->getRace();
        int actor = (int) (intptr_t) createShip(shipRace, 0, shipIdx, 0,
                                                this->missionPtr != 0x17, 0);
        (*this->enemies)[0] = (KIPlayer *) (intptr_t) actor;

        ((PlayerFighter *) (intptr_t) actor)->removeTrail();
        ((PlayerFighter *) (intptr_t) actor)->setExhaustVisible(false);
        ((KIPlayer *) (intptr_t) actor)->setToSleep();
        ((KIPlayer *) (intptr_t) actor)->player->setAlwaysFriend(1);

        PaintCanvas *canvas = Globals::Canvas;
        for (unsigned u = 0; u < 4; u = u + 1) {
            AEGeometry *g = (AEGeometry *) ::operator new(0xc0);
            new(g) AEGeometry((uint16_t)(unsigned)(0x3800 + u), canvas, 0);
            Vector rot = {0, 0, 0};
            ((AEGeometry *) g)->setRotation(*(const AbyssEngine::AEMath::Vector *) (&rot));
            PlayerStatic *p = (PlayerStatic *) ::operator new(0x130);
            new(p) PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            if ((int) race < 4 && race != 1) {
                AEGeometry *child = (AEGeometry *) ::operator new(0xc0);
                new(child) AEGeometry((uint16_t)(unsigned)u, canvas, 0);
                ((AEGeometry *) g)->addChild(((AEGeometry *) child)->transform);
                [&] {
                    AEGeometry *g_ = (AEGeometry *) (child);
                    if (g_) {
                        g_->~AEGeometry();
                        ::operator delete(g_);
                    }
                }();
            }
            ArrayAdd((KIPlayer *) p, *(this->enemies));
        }

        Station *st2 = (Station *) Globals::status->getStation();
        bool fromStationShips = (((Station *) st2)->getIndex() == 0x6c) &&
                                (Globals::status->field_114 == 3);
        AbyssEngine::AERandom *rng = Globals::rnd;
        int spawnCount = rng->nextInt();
        if (fromStationShips) {
            unsigned *ships = (unsigned *) ((Station *) Globals::status->getStation())->getShips();
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
            int fighter = (int) Globals::globals->getRandomEnemyFighter(spawnRace);
            Station *st3 = (Station *) Globals::status->getStation();
            if (((Station *) st3)->getIndex() == 100) {
                int pick = rng->nextInt();
                fighter = (pick == 1) ? 0x26 : (pick == 0) ? 0x25 : 0x28;
            }
            if (fromStationShips) {
                Array<Ship *> *ships = ((Station *) Globals::status->getStation())->getShips();
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

            k->player->setAlwaysFriend(1);
            k->setToSleep();
            ((PlayerFighter *) k)->setExhaustVisible(false);
            ArrayAdd(k, *(this->enemies));
        }
    }
}

void Level::renderBG(int t) {
    uintptr_t canvas = (uintptr_t) Globals::Canvas;

    ((PaintCanvas *) (long) (canvas))->SetColor(0xffffffffu);
    ((PaintCanvas *) (long) (canvas))->BeginBG();
    ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
    ((PaintCanvas *) (long) (canvas))->CameraGetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent());

    Matrix *sky = &this->skyMatrix;
    (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
    sky->m[7] = 0;
    sky->m[3] = 0;
    sky->m[11] = 0;

    Matrix *cloudMtx = &this->cloudMatrix;
    if (Globals::status->inAlienOrbit() == 0 &&
        ((SolarSystem *) Globals::status->getSystem())->getIndex() == 0x1b) {
        Vector dir = AbyssEngine::AEMath::VectorNormalize(this->starSystem->getLightDirection());
        Vector right = AbyssEngine::AEMath::VectorNormalize(
            AbyssEngine::AEMath::VectorCross(Vector{1.0f, 0.0f, 0.0f}, dir));
        Vector up = AbyssEngine::AEMath::VectorNormalize(
            AbyssEngine::AEMath::VectorCross(right, dir));
        AbyssEngine::AEMath::MatrixSetRotation(*cloudMtx, -up, dir, right);
    } else {
        AbyssEngine::AEMath::MatrixSetRotation(*cloudMtx, this->skyRotX, this->skyRotY, this->skyRotZ);
    }
    *sky *= *cloudMtx;

    ((PaintCanvas *) (long) (canvas))->SetWorldViewMatrix(this->skyMatrix);
    ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_19c), 0);
    ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_dummy);
    ((PaintCanvas *) (long) (canvas))->DrawMesh((unsigned int) (*(unsigned *) &this->field_08));
    ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_198), 0);
    ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_2);
    ((PaintCanvas *) (long) (canvas))->DrawMesh((unsigned int) (*(unsigned *) &this->skyboxMesh));

    if (this->field_1b4 != -1) {
        ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_1b8), 0);
        ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_1);
        ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
        ((PaintCanvas *) (long) (canvas))->CameraGetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        sky->m[7] = 0;
        sky->m[3] = 0;
        sky->m[11] = 0;
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        ((PaintCanvas *) (long) (canvas))->
                DrawTransform((unsigned int) (long) (*(Matrix **) &this->field_1b4), nullptr);
    }

    (*(StarSystem **) &this->starSystem)->render();

    if (Globals::status->inSupernovaSystem() != 0 && this->skyboxTexture != -1) {
        int camp = Globals::status->getCurrentCampaignMission();
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

    if (this->field_1bc != -1) {
        ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (*(unsigned *) &this->field_1c0), 0);
        ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_2);
        ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
        ((PaintCanvas *) (long) (canvas))->CameraGetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        int xf = (int) (long) ((PaintCanvas *) (long) (canvas))->TransformGetTransform(0);
        int before = *(int *) (xf + 0x110);
        int xf2 = (int) (long) ((PaintCanvas *) (long) (canvas))->TransformGetTransform(0);
        ((AbyssEngine::Transform *) (intptr_t) xf2)->Update((int64_t)(int)t, true);
        if (*(int *) (xf + 0x110) < before) {
            AbyssEngine::AERandom *rng = Globals::rnd;
            const float toAngle = (1.0f / 65536.0f) * 6.2831855f;
            float ax = (float) rng->nextInt(0x10000) * toAngle;
            float ay = (float) rng->nextInt(0x10000) * toAngle;
            float az = (float) rng->nextInt(0x10000) * toAngle;
            AbyssEngine::AEMath::MatrixSetRotation(this->reversalMatrix, ax, ay, az);
        }
        (*sky *= this->reversalMatrix);
        sky->m[3] = 0;
        sky->m[7] = 0;
        sky->m[11] = 0;
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        ((PaintCanvas *) (long) (canvas))->
                DrawTransform((unsigned int) (long) (*(Matrix **) &this->field_1bc), nullptr);
    }

    if (this->supernovaFlareActive != 0 &&
        1.0f <= gEngine->explosionTimeline) {
        ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
        ((PaintCanvas *) (long) (canvas))->CameraGetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent());
        (*sky = AbyssEngine::AEMath::MatrixGetInverse(*sky));
        (*sky = *sky);
        sky->m[7] = 0;
        sky->m[3] = 0;
        sky->m[11] = 0;
        ((PaintCanvas *) (long) (canvas))->SetWorldViewMatrix(this->skyMatrix);
        ((PaintCanvas *) (long) (canvas))->SetColor(0xffffffffu);
        const AbyssEngine::AEMath::Matrix *eng =
                (const AbyssEngine::AEMath::Matrix *) Globals::Canvas->engine;
        gEngine->SetModelMatrix(*eng);
        ((PaintCanvas *) (long) (canvas))->SetTexture((unsigned int) (unsigned) this->supernovaFlareTexture, 0);
        ((PaintCanvas *) (long) (canvas))->SetBlendMode(AbyssEngine::BlendMode_8);
        ((Engine *) Globals::Canvas->engine)->LightSetLight(0x4000);
        gEngine->GlEnable((unsigned) (uintptr_t) Globals::Canvas->engine, 0);
        ((PaintCanvas *) (long) (canvas))->DrawMesh((unsigned int) (unsigned) this->supernovaFlareMesh);
        gEngine->GlEnable((unsigned) (uintptr_t) Globals::Canvas->engine, 0);
        ((Engine *) Globals::Canvas->engine)->LightEnable(false);
    }

    ((PaintCanvas *) (long) (canvas))->EndBG();
}

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

static inline void levelSpawnFar(Level *self, int *kiPlayer) {
    AbyssEngine::AERandom *rng = Globals::rnd;
    Vector p = (self->player == 0 ? Vector{0.0f, 0.0f, 0.0f} : self->player->getPosition());
    float ox = (float) (rng->nextInt() % 120000 - 60000);
    float oy = (float) (rng->nextInt() % 80000 - 40000);
    float oz = (float) (rng->nextInt() % 120000 - 60000);
    ((KIPlayer *) kiPlayer)->setPosition(p.x + ox, p.y + oy, p.z + oz);
}

static inline void levelPlaceAlien(Level *self, int *kiPlayer, int alienInOrbit) {
    AbyssEngine::AERandom *rng = Globals::rnd;
    Vector base;
    base.x = base.y = base.z = 0.0f;
    if (alienInOrbit)
        base = (self->player == 0 ? Vector{0.0f, 0.0f, 0.0f} : self->player->getPosition());
    float ox = (float) (rng->nextInt() % 100000 - 50000);
    float oy = (float) (rng->nextInt() % 60000 - 30000);
    float oz = (float) (rng->nextInt() % 100000 - 50000);
    ((KIPlayer *) kiPlayer)->setPosition(base.x + ox, base.y + oy, base.z + oz);
}

static inline void levelCloudRandomPos(Level *self, int rng, int boss, unsigned i, Vector *out) {
    Vector p = (self->player == 0 ? Vector{0.0f, 0.0f, 0.0f} : self->player->getPosition());
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

static inline void levelPlaceWingman(Level *self, int *kiSlot, unsigned i) {
    if (kiSlot == 0)
        return;
    Vector p = (self->player == 0 ? Vector{0.0f, 0.0f, 0.0f} : self->player->getPosition());

    float side = ((i & 1) ? -1.0f : 1.0f) * (float) (2000 + (int) (i / 2) * 1500);
    float back = (float) (2000 + (int) (i / 2) * 2500);
    ((KIPlayer *) kiSlot)->setPosition(p.x + side, p.y, p.z - back);
}

unsigned char Level::doInstantJump;
Station *Level::programmedStation;
unsigned char Level::comingFromAlienWorld;
unsigned char Level::initStreamOutPosition;
int Level::energyCellsForNextJump;
int Level::lastMissionFreighterHitpoints;
unsigned char Level::initStreamOutPositionAfterCutscene;
float Level::i_a;
float Level::i_b;
float Level::i_g;
float Level::i_r;
float Level::b_min;
float Level::g_min;
float Level::r_min;
