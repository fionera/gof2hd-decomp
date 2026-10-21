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
#include "game/core/Radio.h"
#include "game/world/RadioStageEntry.h"
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
#include "game/ship/PlayerJunk.h"
#include "game/weapons/RocketGun.h"
#include "game/world/Galaxy.h"

#include "game/ship/PlayerFixedObject.h"
#include "game/ship/PlayerStation.h"
#include "game/ship/PlayerJumpgate.h"
#include "game/ship/PlayerWormHole.h"
#include <new>
#include "game/ship/Agent.h"
#include "engine/render/Engine.h"
#include "engine/render/ParticleSettingsRef.h"
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

Matrix *CameraGetLocal(void *canvas, uint32_t index); // lint: void_ptr (free-function signature; retype changes mangling)

// lint: void_ptr (free-function signature; retype changes mangling)

static unsigned char *g_initStreamOut = nullptr;


static const int g_cg_meshTable[233] = {6754, 6755, 6756, 6760, 6761, 6762, 6763, 6764, 6765, -1, -1, -1, 6788, 6789, 6790, 6791, 6792, 6793, 6794, 6795, 6796, 6797, 6798, 6799, 6800, 6801, 6802, 6803, 14236, 14237, 14238, 14247, 14247, 14247, 14247, 14247, 14249, 14249, 14249, 14249, 14249, 14684, 14684, 14684, 14680, 14680, 14682, 6792, 6797, 6789, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14050, 14052, 14054, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6900, 6901, 6902, 14293, 6905, 6906, 6799, 14297, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14239, 14235, -1, -1, 14247, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6788, 6788, 6788, 14247, 14247, 14247, -1, -1, -1, -1, 14247, -1, -1, 6796, -1, -1, -1, -1, 19091, 6803, 19094, 27338};
static const int g_cg_soundTable[233] = {52, 53, 54, 56, 55, 57, 58, 59, 60, 50, 49, 50, 78, 75, 79, 77, 72, 73, 74, 76, 80, 81, 66, 67, 63, 64, 68, 65, 69, 70, 71, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 6, 7, 8, 9, 10, 11, 92, 93, 94, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1098, 1100, 1099, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1111, 1113, 1112, 1117, 1095, 1096, 1097, 1101, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1118, -1, -1, -1, 2254, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2271, 2272, -1, -1, 2263, 2263, 2263, 2265, 2265, 2265, -1, -1, -1, -1, 2254, 2267, 2268, 2252, -1, 2269, -1, 2275, 2276, 2278, 2277, 2280};
















































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
    Level::initStreamOutPosition = 1;
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

int Level::isInAsteroidCenterRange(Vector v) {
    return collisionVolume->collide(v.x, v.y, v.z);
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
    Globals::Canvas->MeshCreate((unsigned short) (0x4591), (unsigned int &) skyboxMesh, false);
    Globals::Canvas->TextureCreate((unsigned short) (0x275a), (unsigned int &) field_198, false);
    if (this->asteroids != nullptr) {
        for (unsigned int i = 0; i < this->asteroids->size(); i = i + 1) {
            (*this->asteroids)[i]->setDead();
        }
    }
}

void Level::switchSkyboxForSupernovaReversal() {
    Globals::Canvas->MeshCreate(
            (unsigned short) (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() + 0x4588),
            (unsigned int &) skyboxMesh, false);
    Globals::Canvas->TextureCreate(
            (unsigned short) (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() + 0x2751),
            (unsigned int &) field_198, false);
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

Array<RadioMessage *> *Level::getMessages() {
    return messages;
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
        if ((int) Globals::status->getStation()->getPirateStationIndex() < 0) {
            return;
        }
        Array<bool> *tbl = Globals::status->field_4c;
        int idx = Globals::status->getStation()->getPirateStationIndex();
        tbl->data_[idx] = 1;

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
            (*this->enemies)[i]->update(ctx);
            (*this->enemies)[i]->render();
        }
    }
    if (this->asteroids != nullptr) {
        for (unsigned int i = 0; i < this->asteroids->size(); i = i + 1) {
            (*this->asteroids)[i]->update(ctx);
            (*this->asteroids)[i]->render();
        }
    }
    if (this->gasClouds != nullptr) {
        for (unsigned int i = 0; i < this->gasClouds->size(); i = i + 1) {
            (*this->gasClouds)[i]->update(ctx);
            (*this->gasClouds)[i]->render();
        }
    }
    if (this->landmarks != nullptr) {
        for (unsigned int i = 0; i < this->landmarks->size(); i = i + 1) {
            if ((*this->landmarks)[i] != nullptr) {
                (*this->landmarks)[i]->update(ctx);
                (*this->landmarks)[i]->render();
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
    }
    delete field_a4;
    field_a4 = nullptr;
    if (field_a8) {
        ArrayRelease(*field_a8);
    }
    delete field_a8;
    field_a8 = nullptr;
    if (playerGuns) {
        ArrayReleaseClasses(*playerGuns);
    }
    delete playerGuns;
    playerGuns = nullptr;
    if (enemyGuns) {
        ArrayReleaseClasses(*enemyGuns);
    }
    delete enemyGuns;
    enemyGuns = nullptr;
    if (enemies) {
        ArrayReleaseClasses(*enemies);
    }
    delete enemies;
    enemies = nullptr;
    if (asteroids) {
        ArrayReleaseClasses(*asteroids);
    }
    delete asteroids;
    asteroids = nullptr;
    if (gasClouds) {
        ArrayReleaseClasses(*gasClouds);
    }
    delete gasClouds;
    gasClouds = nullptr;
    if (landmarks) {
        ArrayReleaseClasses(*landmarks);
    }
    delete landmarks;
    landmarks = nullptr;
    if (messages) {
        ArrayReleaseClasses(*messages);
    }
    delete messages;
    messages = nullptr;
    if (field_104) {
        ArrayReleaseClasses(*field_104);
    }
    delete field_104;
    field_104 = nullptr;
    delete lodManager;
    lodManager = nullptr;
    delete field_a0;
    field_a0 = nullptr;
    delete field_b0;
    field_b0 = nullptr;
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
    for (int i = -1; i + 1 < (int) n; i = i + 3, p = p + 3) {
        int sign = (Globals::rnd->nextInt(2) == 0) ? 1 : -1;
        p[-1] = (Globals::rnd->nextInt(30000) + 50000) * sign;
        p[0] = Globals::rnd->nextInt(20000) - 10000;
        if (i == -1) {
            pts[2] = Globals::rnd->nextInt(30000) + 50000;
        } else {
            int prev = p[-2];
            p[1] = Globals::rnd->nextInt(30000) + prev + 50000;
        }
    }
    return new Route(pts, n);
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
    if (player != nullptr && field_a4 != nullptr) {
        // engine-trail particle sets start at set 29; 0x34 is the set's second colour word
        EngineColorTable *table = reinterpret_cast<EngineColorTable *>(ParticleSettingsRef::cur);
        for (unsigned int i = 0; i < field_a4->size(); i++) {
            table->entries[i].color = color << 0x18 | color << 0x10 | color << 8 | 0xff;
        }
    }
}

Gun *Level::createGun(int idx, int owner, int kind, int hp, int dmg, int rate, int cool, int color) {
    Gun *gun = nullptr;
    AbstractGun *weapon = nullptr;
    switch (kind) {
        case ITEM_SORT_LASER:
        case ITEM_SORT_BLASTER:
        case ITEM_SORT_THERMO: {
            int mesh = g_cg_meshTable[idx];
            if (mesh >= 0) {
                int barrels = ((unsigned) (idx - 9) < 3 || idx == 228) ? 1 : 20;
                if (kind == ITEM_SORT_THERMO) {
                    gun = new Gun(owner, dmg, barrels, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, 300.0f},
                                  Vector{0.0f, 0.0f, 0.0f});
                    gun->setIndex(idx);
                    gun->weaponType = 3;
                    gun->setPlayerGun(true);
                    gun->setErrorMagnitudePercentage(20);
                    weapon = new RocketGun(owner, gun, mesh, 0, 0, 0, true, this);
                } else {
                    gun = new Gun(owner, dmg, barrels, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, 0.0f},
                                  Vector{0.0f, 0.0f, 0.0f});
                    gun->setIndex(idx);
                    gun->weaponType = kind;
                    gun->setPlayerGun(true);
                    weapon = new ObjectGun(owner, gun, mesh, 1000, this);
                }
            } else {
                gun = new Gun(owner, dmg, 1, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, 0.0f},
                              Vector{0.0f, 0.0f, 0.0f});
                gun->setIndex(idx);
                gun->weaponType = kind;
                gun->setPlayerGun(true);
                weapon = new BeamGun(owner, gun, idx, this);
            }
            break;
        }
        case ITEM_SORT_AUTO_CANNON:
        case ITEM_SORT_SCATTER_GUN:
            gun = new Gun(owner, dmg, 25, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, 400.0f},
                          Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = kind;
            gun->setPlayerGun(true);
            weapon = new ObjectGun(owner, gun, g_cg_meshTable[idx], 1000, this);
            gun->setErrorMagnitudePercentage(2);
            break;
        case ITEM_SORT_ROCKET:
        case ITEM_SORT_MISSILE:
        case ITEM_SORT_CLUSTER_MISSILE:
            gun = new Gun(owner, dmg, kind == ITEM_SORT_CLUSTER_MISSILE ? idx - 211 : 5, hp, cool, rate, (float) color,
                          Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = kind;
            gun->setPlayerGun(true);
            weapon = new RocketGun(owner, gun, g_cg_meshTable[idx], 0, 0, kind,
                                   kind == ITEM_SORT_CLUSTER_MISSILE || kind == ITEM_SORT_MISSILE, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
        case ITEM_SORT_EMP_BOMB:
        case ITEM_SORT_NUKE:
        case ITEM_SORT_IONIZING_MISSILE: {
            gun = new Gun(owner, dmg, 1, hp, cool, rate, (float) color, Vector{0.0f, 0.0f, 400.0f},
                          Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = kind;
            gun->setPlayerGun(true);
            int attribute = (*Globals::items)[idx]->getAttribute(15);
            weapon = new BombGun(gun, g_cg_meshTable[idx], 1, kind, attribute == 1, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
        }
        case ITEM_SORT_TURRET: {
            float z;
            float x;
            if (idx == 181) {
                z = -300.0f;
                x = 0.0f;
            } else {
                z = 300.0f;
                x = idx == 48 ? 80.0f : 0.0f;
            }
            gun = new Gun(owner, dmg, 15, hp, cool, rate, (float) color, Vector{x, 0.0f, z}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = 8;
            gun->setPlayerGun(true);
            if (idx == 48 || idx == 224 || idx == 181) {
                gun->field_0xa4_b0 = true;
                if (idx == 224)
                    gun->field_0xa5 = true;
            }
            weapon = new ObjectGun(owner, gun, g_cg_meshTable[idx], 1000, this);
            break;
        }
        case ITEM_SORT_PLASMA_COLLECTOR: {
            float z;
            float x;
            if (idx == 181) {
                z = -300.0f;
                x = 0.0f;
            } else {
                z = 300.0f;
                x = idx == 48 ? 80.0f : 0.0f;
            }
            gun = new Gun(owner, dmg, 15, hp, cool, rate, (float) color, Vector{x, 0.0f, z}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = 35;
            gun->setPlayerGun(true);
            weapon = new ObjectGun(owner, gun, g_cg_meshTable[idx], 1000, this);
            break;
        }
        case ITEM_SORT_MINE:
            gun = new Gun(owner, dmg, 10, hp, cool, rate, 2.0f, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = 11;
            gun->setPlayerGun(true);
            weapon = new MineGun(gun, g_cg_meshTable[idx], 1, 11, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
        case ITEM_SORT_SENTRY_GUN:
            gun = new Gun(owner, 0, 3, hp, cool, rate, 2.0f, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = 39;
            gun->setPlayerGun(true);
            weapon = new SentryGun(gun, g_cg_meshTable[idx], 1, 39, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
        case ITEM_SORT_SHOCK_BLAST:
            gun = new Gun(owner, dmg, 1, hp, 1, rate, (float) color, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setIndex(idx);
            gun->weaponType = 42;
            gun->setPlayerGun(true);
            weapon = new BombGun(gun, g_cg_meshTable[idx], 1, 42, false, this);
            Globals::globals->addSoundResourceToList(g_cg_soundTable[idx]);
            break;
    }
    switch (idx) {
        case 41:
            Globals::globals->addSoundResourceToList(15);
        case 42:
            Globals::globals->addSoundResourceToList(16);
        case 43:
            Globals::globals->addSoundResourceToList(17);
        case 44:
            Globals::globals->addSoundResourceToList(14);
        case 45:
            Globals::globals->addSoundResourceToList(13);
        case 46:
            Globals::globals->addSoundResourceToList(12);
    }
    gun->setLevel(this);
    if (this->playerGuns == nullptr)
        this->playerGuns = new Array<AbstractGun *>();
    ArrayAdd(weapon, *this->playerGuns);
    return gun;
}

void Level::createSpace() {
    using namespace AbyssEngine::AEMath;
    static const unsigned short jumpgateMeshes[12 * 4] = {
            15000, 15002, 15001, 15003, 15004, 15006, 15005, 15007, 15008, 15010, 15009, 15011,
            15012, 15014, 15013, 15015, 14725, 0, 14726, 0, 14723, 0, 14725, 0, 14722, 0, 14725, 0,
            14720, 0, 14721, 0, 2, 0, 2, 0, 2, 0, 4, 0, 3, 0, 3, 0, 2, 0, 3, 0};
    static const unsigned short jumpgateLodMeshes[12 * 2] = {
            15016, 15020, 15017, 15021, 15018, 15022, 15019, 15023, 16900, 0, 16921, 0, 16904, 0,
            18836, 0, 11, 0, 0, 0, 10, 0, 12, 0};

    if (skyboxMesh == -1) {
        if (Globals::status->inAlienOrbit()) {
            Globals::Canvas->MeshCreate(0x45bc, (unsigned int &) field_08, false);
            Globals::Canvas->TextureCreate(0x2768, (unsigned int &) field_19c, false);
            Globals::Canvas->MeshCreate(0x4592, (unsigned int &) skyboxMesh, false);
            Globals::Canvas->TextureCreate(0x275b, (unsigned int &) field_198, false);
        } else {
            Globals::Canvas->MeshCreate(0x45ba + ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() % 3,
                                        (unsigned int &) field_08, false);
            Globals::Canvas->TextureCreate(0x2766 + ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() % 3,
                                           (unsigned int &) field_19c, false);
            if (Globals::status->getCurrentCampaignMission() == 0 && missionPtr == 3) {
                Globals::Canvas->MeshCreate(0x458b, (unsigned int &) skyboxMesh, false);
                Globals::Canvas->TextureCreate(0x2754, (unsigned int &) field_198, false);
            } else {
                if (Globals::status->inSupernovaSystem()) {
                    if (Globals::status->getCurrentCampaignMission() == 0x59) {
                        Globals::Canvas->MeshCreate(0x458d, (unsigned int &) skyboxMesh, false);
                        Globals::Canvas->TextureCreate(0x2756, (unsigned int &) field_198, false);
                    } else {
                        if (Globals::status->getCurrentCampaignMission() <= 0x9d) {
                            // the dying star and its shock wave
                            Globals::Canvas->MeshCreate(0x45a0, (unsigned int &) skyboxTexture, false);
                            Globals::Canvas->TransformCreate((unsigned int &) field_10);
                            Globals::Canvas->TransformAddMeshId(field_10, skyboxTexture);
                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(field_10))->Update(1000, false);
                            Globals::Canvas->MeshCreate(0x45a1, (unsigned int &) field_14, false);
                            Globals::Canvas->TransformCreate((unsigned int &) field_18);
                            Globals::Canvas->TransformAddMeshId(field_18, field_14);
                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(field_18))->Update(1000, false);
                            if (Globals::status->getCurrentCampaignMission() > 0x69) {
                                Globals::Canvas->TextureCreate(0x2765, (unsigned int &) field_1a0, false);
                            } else {
                                Globals::Canvas->TextureCreate(0x2764, (unsigned int &) field_1a0, false);
                            }
                        }
                        Globals::Canvas->MeshCreate(0x4597, (unsigned int &) skyboxMesh, false);
                        Globals::Canvas->TextureCreate(0x2760, (unsigned int &) field_198, false);
                    }
                } else {
                    Globals::Canvas->MeshCreate(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() + 0x4588,
                                                (unsigned int &) skyboxMesh, false);
                    Globals::Canvas->TextureCreate(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() + 0x2751,
                                                   (unsigned int &) field_198, false);
                }
                int systemIndex = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex();
                int systemIndex2 = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex();
                if (systemIndex == 24 || systemIndex2 == 25 || systemIndex == 26) {
                    supernovaFlareActive = 1;
                    Globals::Canvas->MeshCreate(0x3827, (unsigned int &) supernovaFlareMesh, false);
                    Globals::Canvas->TextureCreate(0x8282, (unsigned int &) supernovaFlareTexture, false);
                    Globals::Canvas->TextureCreate(0x8283, (unsigned int &) field_1c8, false);
                }
                if (Globals::status->inPlanetRingOrbit()) {
                    unsigned int mesh;
                    Globals::Canvas->MeshCreate(0x499f, mesh, false);
                    Globals::Canvas->TransformCreate((unsigned int &) field_1b4);
                    Globals::Canvas->TransformAddMeshId(field_1b4, mesh);
                    Globals::Canvas->TextureCreate(0x715a, (unsigned int &) field_1b8, false);
                }
                if (Globals::status->inStormOrbit()) {
                    unsigned int mesh;
                    Globals::Canvas->MeshCreate(0x49a0, mesh, false);
                    Globals::Canvas->TransformCreate((unsigned int &) field_1bc);
                    Globals::Canvas->TransformAddMeshId(field_1bc, mesh);
                    Globals::Canvas->TextureCreate(0x715b, (unsigned int &) field_1c0, false);
                }
            }
            if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() >= 16
                && ((Engine *) Globals::appManager->GetEngine())->IsPostEffectActivated()) {
                reinterpret_cast<float &>(Globals::Canvas->MeshGetPointer(skyboxMesh)->materialId) = 0.22f;
            }
        }

        if (Globals::status->inFogSkyboxOrbit()) {
            skyRotX = 0.0f;
            skyRotY = 0.0f;
            skyRotZ = 0.0f;
        } else {
            Globals::rnd->setSeed(Globals::status->getStation() != nullptr ? Globals::status->getStation()->getIndex() * 2 : -1);
            skyRotX = (float) Globals::rnd->nextInt(0x10000) * (1.0f / 65536.0f) * 6.2831855f;
            skyRotY = (float) Globals::rnd->nextInt(0x10000) * (1.0f / 65536.0f) * 6.2831855f;
            skyRotZ = (float) Globals::rnd->nextInt(0x10000) * (1.0f / 65536.0f) * 6.2831855f;
            Globals::rnd->reset();
        }
    }

    if (missionPtr != 4 && missionPtr != 23) {
        bool aliens = comingFromAlienWorld || Globals::status->getStation()->isAttackedByAliens()
                      || Globals::status->inAlienOrbit();
        bool earlyAliens = aliens & (Globals::status->getCurrentCampaignMission() < 43);
        if (earlyAliens) {
            Globals::globals->addSoundResourceToList(34);
        }
        landmarks = new Array<KIPlayer *>();
        ArraySetLength(4, *landmarks);
        if (!Globals::status->inEmptyOrbit()
            || (!Globals::status->inAlienOrbit()
                && (Globals::status->getStation()->getIndex() == 27 || Globals::status->getStation()->getIndex() == 110
                    || Globals::status->getStation()->getIndex() == 111))) {
            (*landmarks)[0] = new PlayerStation(Globals::status->getStation());
            lodManager->addObject(((PlayerStation *) (*landmarks)[0])->rootGeometry);
            if (Globals::status->dlc1Won()) {
                Station *station = (Station *) (intptr_t) Globals::galaxy->getStation(101);
                (*landmarks)[0]->name = station->getName();
                delete station;
            }
        } else {
            (*landmarks)[0] = nullptr;
        }

        Vector gatePosition = spawnPosition;
        field_138 = 0;
        Globals::rnd->setSeed(Globals::status->getStation() != nullptr
                              ? (long long) (Globals::status->getStation()->getIndex() * 2) : -1);
        for (int i = 1; i < 3; i++) {
            if (i == 1 && (Globals::status->getSystem() == 0
                           || !((SolarSystem *) (intptr_t) Globals::status->getSystem())->currentOrbitHasWarpGate())) {
                delete (*landmarks)[1];
                (*landmarks)[1] = nullptr;
                continue;
            }
            if (Globals::status->getStation() != nullptr) {
                int side = Globals::rnd->nextInt(2);
                int offset = Globals::rnd->nextInt(500) + 250;
                field_138 += (side == 0 ? -offset : offset) * 16;
                float angle = (float) field_138;
                spawnPosition.z = angle * 3.0f + (i == 1 ? 90000.0f : 120000.0f);
                Matrix rotation;
                MatrixSetRotation(rotation, 0.0f, angle * (1.0f / 65536.0f) * 6.2831855f, 0.0f);
                gatePosition = MatrixRotateVector(rotation, spawnPosition);
                gatePosition.y = 0.0f;
            }
            int race = Globals::status->inAlienOrbit() ? 0 : ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
            AEGeometry *geometry = new AEGeometry(jumpgateMeshes[race * 4 + 0], Globals::Canvas, false);
            unsigned int ring = -1;
            Globals::Canvas->TransformCreate(ring);
            Globals::Canvas->TransformAddMesh(ring, jumpgateMeshes[race * 4 + 1], false);
            unsigned int glow = -1;
            Globals::Canvas->TransformCreate(glow);
            Globals::Canvas->TransformAddMesh(glow, jumpgateMeshes[race * 4 + 2], false);
            geometry->addChild(ring);
            geometry->addChild(glow);
            (*landmarks)[i] = new PlayerJumpgate(15, geometry, gatePosition.x, gatePosition.y, gatePosition.z, i != 2);
            if (i == 2) {
                (*landmarks)[2]->player->radius = 0;
            }
            unsigned int jump = -1;
            Globals::Canvas->TransformCreate(jump);
            Globals::Canvas->TransformAddMesh(jump, jumpgateMeshes[race * 4 + 3], false);
            ((PlayerJumpgate *) (*landmarks)[i])->addJumpAnimationHandle(jump);
            unsigned short lodMeshes[2] = {jumpgateLodMeshes[race * 2 + 0], jumpgateLodMeshes[race * 2 + 1]};
            int lodDistances[2] = {40000, 70000};
            (*landmarks)[i]->geometry->setLodMeshes(lodMeshes, lodDistances, 2);
            lodManager->addObject((*landmarks)[i]->geometry);
            if (race == 1) {
                unsigned short childLodMeshes[2] = {0x3ab0, 0x3ab1};
                (*landmarks)[i]->geometry->setLodChildMeshes(childLodMeshes);
            }
        }
        Globals::rnd->reset();

        if (Globals::status->inAlienOrbit()) {
            float y = (float) (Globals::rnd->nextInt(20000) - 10000);
            float z = (float) (Globals::rnd->nextInt(50000) + 170000);
            Vector position = {0.0f, y, z};
            (*landmarks)[2]->setPosition(position);
            (*landmarks)[2]->geometry->setDirection(-position, Vector{0.0f, 1.0f, 0.0f});
            field_138 = 0;
        }
        if (!Globals::status->gameWon()) {
            PlayerWormHole *wormhole = new PlayerWormHole(0x4262, new AEGeometry(0x4262, Globals::Canvas, false),
                                                          (float) (Globals::rnd->nextInt(80000) - 40000),
                                                          (float) (Globals::rnd->nextInt(40000) - 20000),
                                                          (float) (Globals::rnd->nextInt(40000) + 40000), earlyAliens);
            (*landmarks)[3] = wormhole;
            (*landmarks)[3]->setLevel(this);
        }
        field_138 += 0x8000;
        if (!initStreamOutPosition) {
            field_138 = Globals::rnd->nextInt(2) == 0 ? 1600 : -1600;
        }
    }

    starSystem = new StarSystem(missionPtr);
    SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
    if (system != nullptr) {
        r = (float) system->starR;
        g = (float) system->starG;
        b = (float) system->starB;
    } else {
        r = 10.0f;
        g = 136.0f;
        b = 10.0f;
    }
    r_min = (int) (r / 3.0f);
    g_min = (int) (g / 3.0f);
    b_min = (int) (b / 3.0f);
    i_r = r;
    i_g = g;
    i_b = b;
}

void Level::createRadioMessage(int type, int sub) {
    if (this->player == nullptr || this->player->radioRef == nullptr)
        return;
    if (!((Mission *) Globals::status->getMission())->isEmpty())
        return;

    if (this->messages != nullptr) {
        delete this->messages;
        this->messages = nullptr;
    }
    this->messages = new Array<RadioMessage *>();

    int delay = 0;
    int id = 0x1ba;
    int speaker;
    if (sub == 0)
        speaker = 0x40;
    else if (sub == 2)
        speaker = 0x41;
    else if (sub == 3)
        speaker = 0x15;
    else
        speaker = (sub == 8) ? 9 : 0x3f;

    static const RadioStageEntry stageEntries[] = {
        {26, 2453}, {0, 2454}, {26, 2455}, {0, 2456}, {26, 2457}, {0, 2458}, {26, 2459},
        {0, 2460}, {26, 2461}, {0, 2462}, {26, 2463}, {31, 2464}, {26, 2465}, {0, 2466},
        {26, 2467}, {0, 2468}, {26, 2469}, {0, 2470}, {31, 2471}, {26, 2472}, {0, 2473},
    };
    static const int stageCounts[8] = {2, 2, 2, 4, 3, 3, 2, 3};
    static const int bountyCounts[25] = {2, 3};
    static const int bountyEntries[] = {
        10000, 3132, 0, 3133, 10000, 3136, 0, 3137, 10000, 3138,
    };
    static const int wantedCounts[25] = {2, 2};
    static const int wantedEntries[] = {
        0, 3134, 10000, 3135, 0, 3139, 10000, 3140,
    };

    switch (type) {
        case 0:
            if (Globals::status->field_90 != nullptr) {
                for (unsigned k = 0; k < Globals::status->field_90->size(); k++) {
                    if ((*Globals::status->field_90)[k] == ((Station *) Globals::status->getStation())->getIndex()) {
                        delete this->messages;
                        this->messages = nullptr;
                        this->player->radioRef->setMessages(nullptr);
                        return;
                    }
                }
            }
            id = Globals::rnd->nextInt(3) + 0x1aa;
            break;
        case 1:
            if (Globals::status->field_90 != nullptr) {
                for (unsigned k = 0; k < Globals::status->field_90->size(); k++) {
                    if ((*Globals::status->field_90)[k] == ((Station *) Globals::status->getStation())->getIndex()) {
                        delete this->messages;
                        this->messages = nullptr;
                        this->player->radioRef->setMessages(nullptr);
                        return;
                    }
                }
            }
            id = Globals::rnd->nextInt(3) + 0x1ad;
            break;
        case 3:
            this->field_1b0 = 1;
            id = Globals::rnd->nextInt(3) + 0x1b3;
            break;
        case 4:
            this->field_68 = 1;
            id = Globals::rnd->nextInt(3) + 0x1b6;
            break;
        case 5:
            id = 0x1bb;
            speaker = 0;
            break;
        case 6:
            id = 0x1bc;
            speaker = 0;
            break;
        case 7:
            break;
        case 8: {
            int stage = Globals::rnd->nextInt(8);
            int off = 0;
            for (int k = 0; k < stage; k++)
                off += stageCounts[k];
            int count = stageCounts[stage];
            for (int k = 0; k < count; k++) {
                int stageDelay = k == 0 ? 5000 : k - 1;
                int image = stageEntries[off + k].radioStageEntry;
                ArrayAdd(new RadioMessage(stageEntries[off + k].textID, image, k == 0 ? 5 : 6, stageDelay),
                         *this->messages);
            }
            this->player->radioRef->setMessages(this->messages);
            return;
        }
        case 16: {
            int off = 0;
            for (int k = 0; k < sub; k++)
                off += bountyCounts[k];
            int count = bountyCounts[sub];
            for (int k = 0; k < count; k++) {
                int image = bountyEntries[(off + k) * 2];
                if (image == 10000)
                    image += sub;
                ArrayAdd(new RadioMessage(bountyEntries[(off + k) * 2 + 1], image, k == 0 ? 5 : 6, k == 0 ? 5000 : k - 1), *this->messages);
            }
            if (sub != 2 && sub != 6) {
                this->player->radioRef->setMessages(this->messages);
                return;
            }
            id = Globals::rnd->nextInt(5) + 0xc4a;
            speaker = 10000 + sub;
            break;
        }
        case 17:
            id = Globals::rnd->nextInt(5) + 0xc4f;
            speaker = 0;
            delay = (int) this->player->levelScript->m_nScriptTime + 6000;
            break;
        case 18: {
            int off = 0;
            for (int k = 0; k < sub; k++)
                off += wantedCounts[k];
            int count = wantedCounts[sub];
            for (int k = 0; k < count; k++) {
                int image = wantedEntries[(off + k) * 2];
                if (image == 10000)
                    image += sub;
                ArrayAdd(new RadioMessage(wantedEntries[(off + k) * 2 + 1], image, k == 0 ? 5 : 6, k == 0 ? 5000 : k - 1), *this->messages);
            }
            if ((unsigned) sub < 2) {
                this->player->radioRef->setMessages(this->messages);
                return;
            }
            id = Globals::rnd->nextInt(5) + 0xc45;
            speaker = 0;
            break;
        }
        case 9:
            id = Globals::rnd->nextInt(2) + 0x1c1;
            break;
        case 10:
            id = Globals::rnd->nextInt(2) + 0x1c3;
            break;
        case 11:
            id = Globals::rnd->nextInt(2) + 0x1c5;
            break;
        case 12:
            id = 0x1c5;
            break;
        case 13:
            id = Globals::rnd->nextInt(2) + 0x1c7;
            break;
        case 14:
            id = 0x88f;
            if (Globals::status->field_90 != nullptr) {
                for (int k = 0; k != Globals::status->field_90->size(); k++)
                    if ((*Globals::status->field_90)[k] >= 0)
                        id -= 2;
            }
            break;
        case 15:
            id = 0x88e;
            if (Globals::status->field_90 != nullptr) {
                for (int k = 0; k != Globals::status->field_90->size(); k++)
                    if ((*Globals::status->field_90)[k] >= 0)
                        id -= 2;
                if (id >= 0x889 && id <= 0x88d) {
                    speaker = 0;
                    break;
                }
            }
            delete this->messages;
            this->messages = nullptr;
            this->player->radioRef->setMessages(nullptr);
            return;
        case 19:
            ArrayAdd(new RadioMessage(Globals::rnd->nextInt(4) + 0xaf4, 0, 5, 1500), *this->messages);
            ArrayAdd(new RadioMessage(Globals::rnd->nextInt(4) + 0xafa, 0, 6, 0), *this->messages);
            this->player->radioRef->setMessages(this->messages);
            return;
        case 21:
            id = 0xc54;
            speaker = 0;
            break;
        case 22:
            id = 0xc55;
            speaker = 0;
            break;
        case 23:
            id = 0xc56;
            speaker = 0;
            break;
        case 24:
            id = 0xc57;
            speaker = 0;
            break;
        case 25:
            id = 0xc58;
            speaker = 0;
            break;
        case 26:
            id = 0xc59;
            speaker = 0;
            break;
        case 27:
            ArrayAdd(new RadioMessage(sub * 2 + 0xc60, 6, 5, 1500), *this->messages);
            ArrayAdd(new RadioMessage(sub * 2 + 0xc61, 0, 6, 0), *this->messages);
            this->player->radioRef->setMessages(this->messages);
            return;
        case 28:
            speaker = 0x26;
            id = sub;
            break;
        default:
            id = Globals::rnd->nextInt(3) + 0x1bd;
            break;
    }
    ArrayAdd(new RadioMessage(id, speaker, 5, delay), *this->messages);
    this->player->radioRef->setMessages(this->messages);
}

int Level::init() {
    using namespace AbyssEngine::AEMath;
    if (field_134 == 0) {
        field_1b0 = 0;
        field_68 = 0;
        alarmRequested = 0;
        field_18a = 0;
        delete playerRoute;
        playerRoute = nullptr;
        delete enemyRoute;
        enemyRoute = nullptr;
        delete friendRoute;
        friendRoute = nullptr;
        delete objectivesB;
        objectivesB = nullptr;
        delete objectivesA;
        objectivesA = nullptr;
        lodManager = new LODManager();

        particleEmitBoolPtr = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x4e85,
                                                        false, 0xffff, false);
        bool dustSystem = !Globals::status->inAlienOrbit()
                          && ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() == 12;
        particleRenderBoolPtr = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x6a72,
                                                          false, 0xffff, false);
        field_74 = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x4e83, true, 0xffff,
                                             false);
        field_80 = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x4e7a, true, 0x4e7a,
                                             true);
        field_98 = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x5e20, true, 0x5e20,
                                             true);
        particleSystemMgr = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1,
                                                      dustSystem ? 0x4ea9 : 0x4e7f, true, 0, false);
        skybox2Mesh = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x4e7c, false, 0x4e7c,
                                                false);
        field_8c = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x6a7c, true, 0x6a7c,
                                             true);
        field_9c = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x6ab9, true, 0xffff,
                                             false);
        field_94 = new ParticleSystemManager(Globals::Canvas, ParticleSettings::CameraSet_1, 0x6aaf, true, 0xffff,
                                             false);
        createSpace();

        if (missionPtr == 3) {
            createPlayer();
            if (initStreamOutPosition && player->geometry != nullptr) {
                Array<Station *> *stationStack = Globals::status->getStationStack();
                if (stationStack == nullptr || (*stationStack)[1] == nullptr || Globals::status->getSystem() == 0
                    || ((SolarSystem *) (intptr_t) Globals::status->getSystem())->currentOrbitHasWarpGate()) {
                    Vector position = (*landmarks)[2]->getPosition();
                    player->setPosition(position.x, position.y, position.z);
                } else {
                    // leaving a station: start in front of its planet, facing away from it
                    Array<int> *stations = (Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations();
                    int planet;
                    for (planet = 0; (unsigned int) planet < stations->size(); planet++) {
                        if ((*stations)[planet] == (*stationStack)[1]->getIndex()) {
                            break;
                        }
                    }
                    if ((unsigned int) planet >= stations->size()) {
                        planet = -1;
                    }
                    Vector position = {0.0f, 0.0f, 25000.0f};
                    if (planet >= 0) {
                        position = (*(Array<AEGeometry *> *) starSystem->getPlanets())[planet + 1]->getPosition();
                    }
                    position *= 4.0f;
                    player->setPosition(position);
                    position.x = -position.x;
                    position.y = -position.y;
                    position.z = -position.z;
                    position = VectorNormalize(position);
                    player->geometry->setDirection(position, Vector{0.0f, 1.0f, 0.0f});
                }
            } else {
                player->setPosition(10.0f, 10.0f, 10000.0f);
            }
            if (Globals::status->getCurrentCampaignMission() == 1) {
                player->setPosition(0.0f, 0.0f, -110000.0f);
            }
        }
    }

    if (field_134 != 1) {
        field_134++;
        return 0;
    }

    if (missionPtr != 4 && missionPtr != 23) {
        createAsteroids();
        createGasClouds();
    }
    Mission *mission = Globals::status->getMission();
    if (mission == nullptr) {
        Globals::status->setMission(Mission::empty);
    }
    int mode = missionPtr;
    if (mode != 3) {
        createScene();
        missionPtr = mode;
    } else if (mission->isEmpty() || !mission->isCampaignMission()
               || (Globals::status->gameWon() && !(Globals::options[0x35] | Globals::options[0x37]))) {
        createMission();
        bool blackMarket = Globals::status->inBlackMarketSystem();
        if (initStreamOutPosition && blackMarket) {
            player->setPosition(player->getPosition() + Vector{0.0f, 0.0f, 40000.0f});
        }
    } else if (!Globals::status->getMission()->isEmpty() && Globals::status->getMission()->isCampaignMission()) {
        createCampaignMission();
    }

    createStaticObjects();
    if (missionPtr != 23 && missionPtr != 4
        && (missionPtr != 2 || Globals::status->getCurrentCampaignMission() != 0x2b)) {
        createSentryGuns();
        createFighterTurrets();
        createWingmen();
    }
    assignGuns();
    if (missionPtr != 3) {
        missionPtr = 3;
    }
    connectPlayers();
    if (player != nullptr) {
        player->setRoute(playerRoute);
    }

    int count = 0;
    if (enemies != nullptr) {
        for (unsigned int i = 0; i < enemies->size(); i++) {
            if ((*enemies)[i]->deadFlag == 0 && (*enemies)[i]->field_0x71 == 0
                && (*enemies)[i]->field_0x3f_b == 0 && !(*enemies)[i]->isWingMan()
                && (char) (*enemies)[i]->field_0x44 == 0 && (*enemies)[i]->stealFlagByte == 0) {
                count += (*enemies)[i]->countsAsEnemyExcludeFlag ^ 1;
            }
        }
    }
    asteroidsLeft = 0;
    enemiesLeft = enemies != nullptr ? count - field_120 : 0;
    field_184 = 0;
    field_188 = 0;
    field_1c = 0;
    asteroidsLeft = asteroids != nullptr ? (int) asteroids->size() : 0;
    kills = 0;
    friendCargoStolen = 0;
    field_70 = 1;
    return 1;
}

void Level::createFighterTurrets() {
    if (enemies == nullptr) {
        return;
    }
    for (unsigned int i = 0; i < enemies->size(); i++) {
        KIPlayer *ship = (*enemies)[i];
        if (ship == nullptr) {
            continue;
        }
        PlayerTurret *turret;
        switch (ship->shipGroupFlag) {
            case 0x33: {
                turret = (PlayerTurret *) createStaticObject(nullptr, 0x1a74, true);
                turret->player->setVulnerable(false);
                turret->player->setMaxHitpoints(ship->player->getMaxHitpoints());
                Vector hostOffset = {0.0f, 470.0f, -83.0f};
                turret->setHost(ship, hostOffset);
                ship->field_0x10 = (int) (intptr_t) turret;
                turret->shipGroup = 0;
                break;
            }
            case 0x2d:
                turret = (PlayerTurret *) createStaticObject(nullptr, 0x1a74, true);
                turret->player->setVulnerable(false);
                turret->player->setMaxHitpoints(ship->player->getMaxHitpoints());
                turret->setHost(ship, Vector{0.0f, 172.25f, -460.7f});
                ship->field_0x10 = (int) (intptr_t) turret;
                turret->shipGroup = 8;
                break;
            default:
                continue;
        }
        turret->field_0x74 = 1;
        ArrayAdd((KIPlayer *) turret, *enemies);
    }
}

void Level::updateAlienAttackers(int dt) {
    alienAttackTimer += dt;
    Mission *mission = Globals::status->getMission();
    if (mission != nullptr && mission->isCampaignMission() && (Globals::status->getCurrentCampaignMission() == 0x28 || Globals::status->getCurrentCampaignMission() == 0x93
            || Globals::status->getCurrentCampaignMission() == 0x9a)) {
        return;
    }
    if (alienAttackTimer <= (Globals::status->getCurrentCampaignMission() == 0x29 ? 10000 : 45000)) {
        return;
    }
    alienAttackTimer = 0;
    if (enemies == nullptr) {
        return;
    }

    for (unsigned int i = 0; i < enemies->size(); i++) {
        KIPlayer *alien = (*enemies)[i];
        if (alien->shipGroup != 9 || !alien->isDead() || alien->player->isActive()) {
            continue;
        }
        alien->revive();
        if (!Globals::status->inAlienOrbit() && Globals::status->getStation()->isAttackedByAliens()) {
            // the station is under attack: respawn around it
            alien->setPosition((*landmarks)[3]->getPosition().x - 10000.0f + (float) Globals::rnd->nextInt(20000),
                               (*landmarks)[3]->getPosition().y - 10000.0f + (float) Globals::rnd->nextInt(20000),
                               (*landmarks)[3]->getPosition().z - 10000.0f + (float) Globals::rnd->nextInt(20000));
        } else {
            alien->setPosition(player->getPosition().x - 40000.0f + (float) Globals::rnd->nextInt(80000),
                               player->getPosition().y - 30000.0f + (float) Globals::rnd->nextInt(60000),
                               player->getPosition().z + (float) Globals::rnd->nextInt(2) == 0.0f ? 40000.0f : -40000.0f);
        }
    }
}

static int g_tommyFace[5] = {4, 2, 0, 2, 2};

// Station turret hardpoints per race: {race, position.xyz, rotation.xyz}.
static const float kStationTurrets[4][2][7] = {
    {{0.0f, 0.0f, -1097.14f, -4178.23f, 0.0f, 0.0f, 3.1415927f}, {0.0f, 0.0f, 1158.09f, 1180.59f, 0.0f, 0.0f, 0.0f}},
    {{1.0f, 0.0f, -1096.98f, -2691.52f, 0.0f, 0.0f, 3.1415927f}, {1.0f, 0.0f, 1893.48f, 1068.07f, 0.0f, 0.0f, 0.0f}},
    {{2.0f, 0.0f, -484.719f, 1741.22f, 0.0f, 0.0f, 3.1415927f}, {2.0f, 0.0f, 1458.84f, -2304.28f, 0.0f, 0.0f, 0.0f}},
    {{3.0f, 0.0f, -516.08f, -3744.45f, 0.0f, 0.0f, 3.1415927f}, {3.0f, 0.0f, 515.766f, -3744.45f, 0.0f, 0.0f, 0.0f}},
};
// Alien station / outpost gun hardpoints: {position.xyz, rotation.xyz}.
static const float kAlienStationTurrets[8][6] = {
    {-6725.879f, 2458.2627f, -66.07605f, -0.0f, 1.570796f, 0.0f},
    {5824.434f, 2458.2627f, 10243.145f, -0.0f, -1.570796f, -0.0f},
    {-6725.879f, 2458.2627f, 2099.9644f, -0.0f, 1.570796f, 0.0f},
    {5824.434f, 2458.2627f, 8436.65f, -0.0f, -1.570796f, -0.0f},
    {-5548.711f, -2743.1519f, 11954.467f, -3.141593f, 1.570796f, -0.0f},
    {-5548.711f, -2743.1519f, -3710.504f, -3.141593f, 1.570796f, -0.0f},
    {4981.259f, -2743.1519f, 14659.006f, 3.141593f, -2.181661f, 0.0f},
    {0.0f, 2458.2627f, -32811.1f, -0.0f, -0.0f, -0.0f},
};
static const float kAlienOutpostTurrets[7][6] = {
    {5321.74f, 1893.1f, -15570.7f, 0.0f, 0.0f, -1.5707964f},
    {2355.67f, 1115.48f, 7324.1f, 0.0f, 0.0f, -1.5707964f},
    {0.0f, -4261.28f, 4876.28f, 0.0f, 0.0f, 3.1415927f},
    {-5321.74f, 1893.1f, -15570.7f, 0.0f, 0.0f, 1.5707964f},
    {0.0f, 5668.84f, -13871.7f, 0.0f, 0.0f, 0.0f},
    {-2355.67f, 1115.48f, 7324.1f, 0.0f, 0.0f, 1.5707964f},
    {0.000200685f, 2855.3f, 4161.15f, 0.0f, 0.0f, 0.0f},
};
// Pirate base gun hardpoints: {position.xyz, rotation.xyz}.
static const float kBaseTurrets[5][6] = {
    {10283.18f, -1122.7914f, 26039.262f, 0.0f, 0.0f, 0.0f},
    {-10171.174f, -1082.702f, 25906.553f, 0.0f, 0.0f, 0.0f},
    {-15624.387f, -787.1992f, -7569.43f, 0.0f, 0.0f, 0.0f},
    {15624.387f, -787.1992f, -7569.43f, 0.0f, 0.0f, 0.0f},
    {0.0f, 4901.473f, 3083.9644f, 0.0f, 0.0f, 0.0f},
};
static const int kPirateStationCoords[4][3] = {
    {-200000, -100000, 50000}, {50000, 200000, -100000}, {140000, -150000, 7000}, {170000, -170000, 10000}};
static const int kBlueprintRaces[5] = {1, 3, 2, 0, 2};
static const float kBlueprintPositions[5][3] = {
    {-20000.0f, 30000.0f, 80000.0f}, {40000.0f, -30000.0f, 100000.0f}, {-80000.0f, 80000.0f, -90000.0f},
    {40000.0f, 20000.0f, 140000.0f}, {40000.0f, 20000.0f, 140000.0f}};
static const int kBlueprintSpacePoints[5] = {14, 11, 12, 13, 12};

void Level::createMission() {
    Mission *mission = Globals::status->getMission();
    if (mission == nullptr)
        return;

    Vector playerPos = this->player->geometry->getPosition();
    if (Globals::status->inAlienOrbit()) {
        int level = Globals::status->getLevel();
        int count = 2;
        float base = (float) level * 0.5f - 1.0f;
        if (!(base + (float) Globals::rnd->nextInt(2) < 2.0f))
            count = (int) (base + (float) Globals::rnd->nextInt(2));
        if (Globals::status->getCurrentCampaignMission() == 68)
            count = 2;
        if (Globals::status->getCurrentCampaignMission() == 33)
            count = 2;
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(count, *this->enemies);
        for (int i = 0; i < count; i++) {
            (*this->enemies)[i] = this->createShip(9, 0, Globals::globals->getRandomEnemyFighter(9), nullptr, true, false);
            (*this->enemies)[i]->setPosition((float) (Globals::rnd->nextInt(120000) - 60000),
                                             (float) (Globals::rnd->nextInt(80000) - 40000),
                                             (float) (Globals::rnd->nextInt(120000) - 60000));
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
        }
        return;
    }

    if (!mission->isEmpty()) {
        int localRace = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
        int enemyRace = Globals::rnd->nextInt(100) < 75
                                ? 8
                                : ((Standing *) (intptr_t) Globals::status->getStanding())->getEnemyRace(localRace);
        switch (mission->getType()) {
            case 1: {
                int coords[9] = {Globals::rnd->nextInt(100000) - 50000, 0, 50000,
                                 Globals::rnd->nextInt(100000) - 50000, 0, 75000,
                                 Globals::rnd->nextInt(100000) - 50000, 0, 100000};
                this->enemyRoute = new Route(coords, 9);
                float difficulty = (float) Globals::status->getMission()->getDifficulty() / 10.0f * 5.0f;
                int friends = Globals::rnd->nextInt(5);
                float attackers = (float) ((int) difficulty + 3);
                int attackerCount = (int) (attackers + (*(float *) &Globals::options[44] - 0.5f) * attackers);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(attackerCount + friends + 3, *this->enemies);
                for (int i = 0; i < attackerCount; i++) {
                    (*this->enemies)[i] = this->createShip(enemyRace, 0, Globals::globals->getRandomEnemyFighter(enemyRace),
                                                           nullptr, true, false);
                    (*this->enemies)[i]->player->setAlwaysEnemy(true);
                }
                for (unsigned i = attackerCount; i < this->enemies->size(); i++) {
                    (*this->enemies)[i] = this->createShip(
                            localRace, 0, Globals::globals->getRandomEnemyFighter(localRace),
                            this->enemyRoute->getWaypoint(Globals::rnd->nextInt(this->enemyRoute->length())), true, false);
                    (*this->enemies)[i]->player->setAlwaysFriend(true);
                }
                this->objectivesA = new Objective(7, attackerCount, this);
                return;
            }
            case 2: {
                Vector asteroidPos = {0.0f, 0.0f, 0.0f};
                int coords[6] = {(int) (float) (-20000 - Globals::rnd->nextInt(20000)), 0,
                                 (int) (float) (-20000 - Globals::rnd->nextInt(20000)), 0, 0, 0};
                this->enemyRoute = new Route(coords, 6);
                int goods = mission->getProductionGoodAmount();
                int escorts = (int) ((float) mission->getDifficulty() / 10.0f * 4.0f);
                this->enemies = new Array<KIPlayer *>();
                int convoyEnd = escorts + 2;
                int total = convoyEnd + goods;
                ArraySetLength(total, *this->enemies);
                int offset = 0;
                for (int i = 0; i < convoyEnd; i++) {
                    (*this->enemies)[i] = this->createShip(enemyRace, 0, Globals::globals->getRandomEnemyFighter(enemyRace),
                                                           nullptr, true, false);
                    KIPlayer *ship = (*this->enemies)[i];
                    ship->setPosition((float) (offset + this->enemyRoute->getWaypoint(0)->x),
                                      (float) (offset + this->enemyRoute->getWaypoint(0)->y),
                                      (float) (offset + this->enemyRoute->getWaypoint(0)->z));
                    (*this->enemies)[i]->player->setAlwaysEnemy(true);
                    (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
                    (*this->enemies)[i]->getRoute()->reachWaypoint(0);
                    offset += 2000;
                }
                for (int j = 0; (unsigned) (convoyEnd + j) < this->enemies->size(); j++) {
                    (*this->enemies)[convoyEnd + j] = this->createShip(
                            localRace, 0, Globals::globals->getRandomEnemyFighter(localRace), nullptr, true, false);
                    (*this->enemies)[convoyEnd + j]->player->setAlwaysFriend(true);
                    asteroidPos = (*this->asteroids)[j + this->asteroids->size() / 2]->getPosition();
                    (*this->enemies)[convoyEnd + j]->setPosition(asteroidPos.x, asteroidPos.y + 2000.0f, asteroidPos.z);
                    (*this->enemies)[convoyEnd + j]->cargo = nullptr;
                    ((PlayerFighter *) (*this->enemies)[convoyEnd + j])->crateCaptured() = 0;
                    (*this->enemies)[convoyEnd + j]->setSpeed(0.0f);
                    (*this->enemies)[convoyEnd + j]->setRotationSpeed(0.0f);
                    Player *p = (*this->enemies)[convoyEnd + j]->player;
                    p->setHitpoints(p->getMaxHitpoints() * 3);
                }
                this->objectivesA = new Objective(18, 0, convoyEnd, this);
                this->objectivesB = new Objective(18, convoyEnd, total, this);
                return;
            }
            case 3:
            case 5: {
                int coords[3];
                coords[0] = (Globals::rnd->nextInt(80000) + 40000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
                coords[1] = 0;
                coords[2] = (Globals::rnd->nextInt(80000) + 40000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
                this->playerRoute = new Route(coords, 3);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(mission->getProductionGoodAmount(), *this->enemies);
                for (unsigned i = 0; i < this->enemies->size(); i++) {
                    (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                           this->playerRoute->getWaypoint(0), true, false);
                    (*this->enemies)[i]->setToSleep();
                }
                ((PlayerFighter *) (*this->enemies)[this->enemies->size() - 1])->setMissionCrate(true);
                (*this->enemies)[this->enemies->size() - 1]->name = *Globals::gameText->getText(1611);
                this->objectivesA = new Objective(11, this->enemies->size() - 1, this);
                this->objectivesB = new Objective(12, this->enemies->size() - 1, this);
                return;
            }
            case 4: {
                if (Globals::rnd->nextInt(2) == 0) {
                    int coords[3] = {(int) this->stationPosition.x, (int) this->stationPosition.y,
                                     (int) this->stationPosition.z};
                    this->playerRoute = new Route(coords, 3);
                } else {
                    this->playerRoute = this->createRoute(Globals::rnd->nextInt(2) + 2);
                }
                float hunters = (float) ((int) ((float) Globals::status->getMission()->getDifficulty() / 10.0f * 5.0f) + 2);
                int hunterCount = (int) (hunters + (*(float *) &Globals::options[44] - 0.5f) * hunters);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(hunterCount, *this->enemies);
                for (unsigned i = 0; i < this->enemies->size(); i++) {
                    (*this->enemies)[i] = this->createShip(
                            8, 0, Globals::globals->getRandomEnemyFighter(8),
                            this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
                    (*this->enemies)[i]->setToSleep();
                }
                this->objectivesA = new Objective(18, 0, hunterCount, this);
                return;
            }
            case 6: {
                int coords[3];
                coords[0] = (Globals::rnd->nextInt(80000) + 60000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
                coords[1] = 0;
                coords[2] = (Globals::rnd->nextInt(80000) + 60000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
                this->playerRoute = new Route(coords, 3);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(1, *this->enemies);
                (*this->enemies)[0] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                       this->playerRoute->getWaypoint(0), true, false);
                (*this->enemies)[0]->setToSleep();
                Player *p = (*this->enemies)[0]->player;
                p->setMaxHitpoints(p->getMaxHitpoints() * 3);
                (*this->enemies)[0]->setSpeed(3.0f);
                ((PlayerFighter *) (*this->enemies)[0])->setRotate(3);
                this->objectivesA = new Objective(1, 0, this);
                return;
            }
            case 7: {
                int coords[3] = {Globals::rnd->nextInt(40000) - 20000, Globals::rnd->nextInt(20000) - 10000,
                                 Globals::rnd->nextInt(30000) + 40000};
                Route *route = new Route(coords, 3);
                int guards = (int) ((float) Globals::status->getMission()->getDifficulty() / 10.0f * 2.0f);
                int rocks = (int) ((float) Globals::status->getMission()->getDifficulty() / 10.0f * 20.0f);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(rocks + guards + 15, *this->enemies);
                unsigned i = 0;
                for (; i < this->enemies->size() - guards; i++) {
                    (*this->enemies)[i] = this->createStaticObject(route->getWaypoint(0), 16917, true);
                    (*this->enemies)[i]->player->setAlwaysEnemy(true);
                }
                for (i = this->enemies->size() - guards; i < this->enemies->size(); i++)
                    (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8), nullptr, true, false);
                this->objectivesA = new Objective(7, this->enemies->size() - guards, this);
                this->timeLimit = 121000;
                Globals::globals->addSoundResourceToList(22);
                return;
            }
            case 9: {
                int coords[9] = {10000, 0, 100000, 10000, 0, 150000, 10000, 0, 200000};
                this->enemyRoute = new Route(coords, 9);
                float attackers = (float) ((int) ((float) Globals::status->getMission()->getDifficulty() / 10.0f * 5.0f) + 3);
                int attackerCount = (int) (attackers + (*(float *) &Globals::options[44] - 0.5f) * attackers);
                this->enemies = new Array<KIPlayer *>();
                int total = attackerCount + 5;
                ArraySetLength(total, *this->enemies);
                int attackerRace = ((Standing *) (intptr_t) Globals::status->getStanding())->getEnemyRace(mission->getClientRace());
                int clientRace = mission->getClientRace() > 3 ? 0 : mission->getClientRace();
                int stationType = clientRace == 1 ? 13 : 15;
                for (unsigned i = 0; i < this->enemies->size(); i++) {
                    if ((int) i < attackerCount) {
                        (*this->enemies)[i] = this->createShip(
                                attackerRace, 0, Globals::globals->getRandomEnemyFighter(attackerRace),
                                this->enemyRoute->getWaypoint(Globals::rnd->nextInt(this->enemyRoute->length())), true, false);
                        (*this->enemies)[i]->setToSleep();
                        (*this->enemies)[i]->player->setAlwaysEnemy(true);
                    } else {
                        (*this->enemies)[i] = this->createShip(clientRace, 1, stationType, nullptr, true, false);
                        Player *p = (*this->enemies)[i]->player;
                        float hp = (float) ((Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 2) + 150.0f +
                                   (float) ((Globals::status->gameWon() ? 45 : Globals::status->getCurrentCampaignMission()) * 2);
                        p->setMaxHitpoints((int) (hp * (*(float *) &Globals::options[44] > 0.7 ? 1.4f : 1.0f)));
                        ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(true);
                        (*this->enemies)[i]->player->setAlwaysFriend(true);
                        (*this->enemies)[i]->cargo = nullptr;
                        ((PlayerFighter *) (*this->enemies)[i])->crateCaptured() = 0;
                    }
                }
                (*this->enemies)[attackerCount]->setPosition(-2500.0f, -300.0f, 27000.0f);
                (*this->enemies)[attackerCount + 1]->setPosition(6500.0f, 3000.0f, 24000.0f);
                (*this->enemies)[attackerCount + 2]->setPosition(-4000.0f, -2000.0f, 19000.0f);
                (*this->enemies)[attackerCount + 3]->setPosition(9000.0f, -6000.0f, 17000.0f);
                (*this->enemies)[attackerCount + 4]->setPosition(3000.0f, 7000.0f, 15000.0f);
                this->objectivesA = new Objective(18, 0, attackerCount, this);
                this->objectivesB = new Objective(18, attackerCount, total, this);
                return;
            }
            case 10: {
                int coords[6] = {Globals::rnd->nextInt(5000) - 2500, Globals::rnd->nextInt(5000) - 2500,
                                 Globals::rnd->nextInt(30000) + 80000, Globals::rnd->nextInt(5000) - 2500,
                                 Globals::rnd->nextInt(5000) - 2500, Globals::rnd->nextInt(30000) + 120000};
                this->playerRoute = new Route(coords, 6);
                int extra = Globals::rnd->nextInt(2);
                float difficulty = (float) Globals::status->getMission()->getDifficulty() / 10.0f * 5.0f;
                int race = ((Standing *) (intptr_t) Globals::status->getStanding())->getEnemyRace(mission->getClientRace());
                if (race == 8)
                    race = 0;
                float defenders = (float) ((int) difficulty + 3);
                int stations = extra + 2;
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength((int) (defenders + (*(float *) &Globals::options[44] - 0.5f) * defenders) + stations, *this->enemies);
                int stationType = race == 1 ? 13 : 15;
                for (int i = 0; i < stations; i++) {
                    (*this->enemies)[i] = this->createShip(race, 1, stationType, this->playerRoute->getWaypoint(1), true, false);
                    (*this->enemies)[i]->setToSleep();
                    (*this->enemies)[i]->player->setAlwaysEnemy(true);
                    ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                    KIPlayer *station = (*this->enemies)[i];
                    station->setPosition((float) (this->playerRoute->getWaypoint(1)->x + Globals::rnd->nextInt(20000) - 10000),
                                         (float) (this->playerRoute->getWaypoint(1)->y + Globals::rnd->nextInt(20000) - 10000),
                                         (float) (this->playerRoute->getWaypoint(1)->z + Globals::rnd->nextInt(20000) - 10000));
                    Player *p = (*this->enemies)[i]->player;
                    p->setMaxHitpoints((int) ((float) p->getMaxHitpoints() * 0.7f *
                                              (*(float *) &Globals::options[44] > 0.7 ? 1.4f : 1.0f)));
                }
                for (unsigned i = stations; i < this->enemies->size(); i++) {
                    (*this->enemies)[i] = this->createShip(
                            race, 0, Globals::globals->getRandomEnemyFighter(race),
                            this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
                    (*this->enemies)[i]->setToSleep();
                    (*this->enemies)[i]->player->setAlwaysEnemy(true);
                }
                this->objectivesA = new Objective(7, stations, this);
                return;
            }
            case 12: {
                this->playerRoute = this->createRoute(Globals::rnd->nextInt(2) + 3);
                int pirates = (int) ((float) mission->getDifficulty() / 10.0f * 4.0f);
                this->enemies = new Array<KIPlayer *>();
                int count = pirates + 3;
                if (count % 2 == 0)
                    count = pirates + 4;
                int total = count + 1;
                ArraySetLength(total, *this->enemies);
                (*this->enemies)[0] = this->createShip(mission->getAgent()->getRace(), 0,
                                                       Globals::globals->getRandomEnemyFighter(mission->getAgent()->getRace()),
                                                       nullptr, true, false);
                (*this->enemies)[0]->setPosition(playerPos.x + (float) (Globals::rnd->nextInt(1400) - 700),
                                                 playerPos.y + (float) (Globals::rnd->nextInt(1400) - 700),
                                                 playerPos.z + 1000.0f);
                (*this->enemies)[0]->setSpeed(3.0f);
                ((PlayerFighter *) (*this->enemies)[0])->setRotate(3);
                (*this->enemies)[0]->player->setHitpoints(9999999);
                (*this->enemies)[0]->setRoute(this->playerRoute->clone());
                (*this->enemies)[0]->player->setAlwaysFriend(true);
                (*this->enemies)[0]->name = mission->getAgent()->getName();
                (*this->enemies)[0]->cargo = nullptr;
                for (unsigned i = 1; i < this->enemies->size(); i++) {
                    (*this->enemies)[i] = this->createShip(
                            8, 0, Globals::globals->getRandomEnemyFighter(8),
                            this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
                    (*this->enemies)[i]->setToSleep();
                }
                this->objectivesA = new Objective(20, 1, total, this);
                this->objectivesB = new Objective(21, 1, total, this);
                return;
            }
            case 15: {
                int coords[9] = {Globals::rnd->nextInt(140000) - 70000, 0, 70000,
                                 Globals::rnd->nextInt(140000) - 70000, 0, 100000,
                                 Globals::rnd->nextInt(140000) - 70000, 0, 140000};
                this->enemyRoute = new Route(coords, 3);
                float raiders = (float) ((int) ((float) Globals::status->getMission()->getDifficulty() / 10.0f * 2.0f) + 1);
                raiders = raiders + (*(float *) &Globals::options[44] - 0.5f) * raiders;
                int raidRace = ((Standing *) (intptr_t) Globals::status->getStanding())->getEnemyRace(mission->getClientRace());
                int raiderCount = (int) raiders;
                int clientRace = mission->getClientRace();
                this->miningPlant = 1;
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(this->miningPlant + raiderCount + 2, *this->enemies);
                for (int i = 0; i < raiderCount; i++) {
                    (*this->enemies)[i] = this->createShip(
                            raidRace, 0, Globals::globals->getRandomEnemyFighter(raidRace),
                            this->enemyRoute->getWaypoint(Globals::rnd->nextInt(this->enemyRoute->length())), true, false);
                    (*this->enemies)[i]->player->setAlwaysEnemy(true);
                }
                this->miningPlantIndex = raiderCount;
                (*this->enemies)[this->miningPlantIndex] = this->createStaticObject(this->asteroidWaypoint, 19080, true);
                ((PlayerFixedObject *) (*this->enemies)[this->miningPlantIndex])->setDockingType(1);
                ((PlayerFixedObject *) (*this->enemies)[this->miningPlantIndex])->setMoving(false);
                (*this->enemies)[this->miningPlantIndex]->setPosition(this->stationPosition);
                int shuttleCoords[6] = {0, 0, -30000, -1, -1, -1};
                shuttleCoords[0] = (int) (this->stationPosition.x + (float) shuttleCoords[0]);
                shuttleCoords[1] = (int) (this->stationPosition.y + (float) shuttleCoords[1]);
                shuttleCoords[2] = (int) (this->stationPosition.z + (float) shuttleCoords[2]);
                int waitTimes[2] = {0, 6000};
                Array<KIPlayer *> *stops = new Array<KIPlayer *>();
                ArraySetLength(2, *stops);
                (*stops)[1] = (*this->enemies)[this->miningPlantIndex];
                Route *shuttleRoute = new Route(shuttleCoords, stops, waitTimes, 6);
                shuttleRoute->setLoop(true);
                Route *shuttleRoute2 = shuttleRoute->clone();
                for (int i = this->miningPlant + raiderCount; i < this->miningPlant + raiderCount + 2; i++) {
                    int type = Globals::globals->getRandomEnemyFighter(clientRace);
                    if (i == this->miningPlant + raiderCount) {
                        (*this->enemies)[i] = this->createShip(clientRace, 0, type, shuttleRoute->getWaypoint(0), true, false);
                        (*this->enemies)[i]->setRoute(shuttleRoute);
                    } else {
                        (*this->enemies)[i] = this->createShip(clientRace, 0, type, shuttleRoute2->getWaypoint(0), true, false);
                        (*this->enemies)[i]->setRoute(shuttleRoute2);
                    }
                    (*this->enemies)[i]->player->setAlwaysFriend(true);
                    (*this->enemies)[i]->player->setNeverAttack(true);
                }
                this->objectivesA = new Objective(28, mission->getProductionGoodAmount(), mission->getProductionGoodIndex(), this);
                return;
            }
            case 183: {
                int coords[6] = {-55000, 0, -35000, -30000, 0, -50000};
                this->enemyRoute = new Route(coords, 6);
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(14, *this->enemies);
                for (int i = 0; i < 8; i++) {
                    (*this->enemies)[i] = this->createShip(10, 0, 44, this->enemyRoute->getWaypoint(0), true, false);
                    (*this->enemies)[i]->player->setAlwaysEnemy(true);
                    (*this->enemies)[i]->player->setMaxHitpoints(250);
                }
                (*this->enemies)[8] = this->createStaticObject(nullptr, 18781, false);
                ((PlayerFixedObject *) (*this->enemies)[8])->setMoving(false);
                ((PlayerFixedObject *) (*this->enemies)[8])->setDockingType(2);
                ((PlayerFixedObject *) (*this->enemies)[8])->setName(*Globals::gameText->getText(3208));
                (*this->enemies)[8]->player->setAlwaysFriend(true);
                (*this->enemies)[8]->geometry->rotate(Vector{0.0f, -2.3561945f, 0.0f});
                (*this->enemies)[8]->shipGroup = 10;
                (*this->enemies)[8]->field_0x74 = 1;
                delete (*this->enemies)[8]->cargo;
                (*this->enemies)[8]->cargo = nullptr;
                Waypoint *dockWp = new Waypoint(-70000, -5000, -35000, nullptr);
                (*this->enemies)[9] = this->createStaticObject(dockWp, 18804, true);
                (*this->enemies)[9]->setPosition(dockWp->getPosition());
                ((PlayerFixedObject *) (*this->enemies)[9])->setMoving(false);
                (*this->enemies)[9]->field_0x70 = 0;
                ((PlayerFixedObject *) (*this->enemies)[9])->setDockingType(1);
                (*this->enemies)[9]->player->setAlwaysFriend(true);
                (*this->enemies)[9]->player->setHitpoints(9999999);
                (*this->enemies)[9]->shipGroup = 10;
                (*this->enemies)[9]->field_0x74 = 1;
                delete (*this->enemies)[9]->cargo;
                (*this->enemies)[9]->cargo = nullptr;
                int convoyCoords[9] = {-1, -1, -1, 30000, 1000, 40000, -1, -1, -1};
                int waitTimes[3] = {20000, 0, 20000};
                Array<KIPlayer *> *stops = new Array<KIPlayer *>();
                ArraySetLength(3, *stops);
                (*stops)[0] = (*this->enemies)[9];
                (*stops)[2] = (*this->enemies)[8];
                Route *convoyRoute = new Route(convoyCoords, stops, waitTimes, 9);
                convoyRoute->setLoop(true);
                Vector offset;
                for (int i = 10; i < 14; i++) {
                    (*this->enemies)[i] = this->createShip(0, 0, 51, convoyRoute->getWaypoint(0), true, false);
                    (*this->enemies)[i]->setRoute(convoyRoute->clone());
                    (*this->enemies)[i]->player->setAlwaysFriend(true);
                    ((PlayerFighter *) (*this->enemies)[i])->field_0x13d = 0;
                    ((PlayerFighter *) (*this->enemies)[i])->field_0x13e = 0;
                    switch (i - 10) {
                        case 0:
                            offset.y = -3000.0f;
                            offset.x = 20000.0f;
                            offset.z = -50000.0f;
                            break;
                        case 1:
                            offset.y = -7000.0f;
                            offset.x = 10000.0f;
                            offset.z = -30000.0f;
                            break;
                        case 2:
                            offset.y = 0.0f;
                            offset.x = 25000.0f;
                            offset.z = -36000.0f;
                            break;
                        default:
                            offset.y = 6000.0f;
                            offset.x = 12000.0f;
                            offset.z = -45000.0f;
                            break;
                    }
                    (*this->enemies)[i]->setPosition(offset);
                }
                this->player->setPosition(-30000.0f, 5000.0f, -100000.0f);
                this->stationPosition = this->player->getPosition();
                this->timeLimit = 181000;
                this->objectivesA = new Objective(3, 181000, this);
                return;
            }
            case 184: {
                int coords[9] = {Globals::rnd->nextInt(140000) - 70000, 0, 70000,
                                 Globals::rnd->nextInt(140000) - 70000, 0, 100000,
                                 Globals::rnd->nextInt(140000) - 70000, 0, 140000};
                this->enemyRoute = new Route(coords, 3);
                Globals::status->getMission()->getDifficulty();
                ((Standing *) (intptr_t) Globals::status->getStanding())->getEnemyRace(mission->getClientRace());
                int clientRace = mission->getClientRace();
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(4, *this->enemies);
                (*this->enemies)[0] = this->createStaticObject(new Waypoint(-30000, 0, 40000, nullptr), 19080, true);
                ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(false);
                ((PlayerFixedObject *) (*this->enemies)[0])->setDockingType(2);
                ((PlayerFixedObject *) (*this->enemies)[0])->setName(*Globals::gameText->getText(3208));
                (*this->enemies)[1] = this->createStaticObject(new Waypoint(30000, 0, 40000, nullptr), 18830, true);
                ((PlayerFixedObject *) (*this->enemies)[1])->setMoving(false);
                ((PlayerFixedObject *) (*this->enemies)[1])->setDockingType(1);
                ((PlayerFixedObject *) (*this->enemies)[1])->setName(*Globals::gameText->getText(3209));
                int convoyCoords[9] = {-1, -1, -1, 0, 0, 40000, -1, -1, -1};
                int waitTimes[3] = {10000, 0, 10000};
                Array<KIPlayer *> *stops = new Array<KIPlayer *>();
                ArraySetLength(3, *stops);
                (*stops)[0] = (*this->enemies)[0];
                (*stops)[2] = (*this->enemies)[1];
                Route *convoyRoute = new Route(convoyCoords, stops, waitTimes, 9);
                convoyRoute->setLoop(true);
                Route *convoyRoute2 = convoyRoute->clone();
                for (int i = 2; i < 4; i++) {
                    int type = Globals::globals->getRandomEnemyFighter(clientRace);
                    if (i == 2) {
                        (*this->enemies)[i] = this->createShip(clientRace, 0, type, convoyRoute->getWaypoint(0), true, false);
                        (*this->enemies)[i]->setRoute(convoyRoute);
                    } else {
                        (*this->enemies)[i] = this->createShip(clientRace, 0, type, convoyRoute2->getWaypoint(0), true, false);
                        (*this->enemies)[i]->setRoute(convoyRoute2);
                    }
                    (*this->enemies)[i]->player->setAlwaysFriend(true);
                    (*this->enemies)[i]->player->setNeverAttack(true);
                }
                this->objectivesA = new Objective(29, mission->getProductionGoodAmount(), mission->getProductionGoodIndex(), this);
                return;
            }
            default:
                return;
        }
    }

    Globals::rnd->reset();
    this->missionPtr = 0;
    this->createScene();
    this->missionPtr = 3;
    bool introSystem = false;
    if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 15 &&
        Globals::status->getCurrentCampaignMission() < 16)
        introSystem = true;
    int stationIndex = Globals::status->getStation()->getIndex();
    int raidRoll = Globals::rnd->nextInt(100);
    Mission *freelance = Globals::status->getFreelanceMission();
    int pirates = 0;
    if (freelance != nullptr && (freelance->getType() == 0 || freelance->getType() == 11 || freelance->getType() == 15))
        pirates = (int) ((float) freelance->getDifficulty() / 10.0f * 5.0f);
    if (Globals::status->getSystem() != 0 && ((SolarSystem *) (intptr_t) Globals::status->getSystem())->hasPirateBase())
        pirates = Globals::rnd->nextInt(1) + 2;
    int security = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getSecurityLevel();
    if (Globals::status->hardCoreMode() && security > 0)
        security--;
    bool raid;
    if (introSystem && !Globals::status->hardCoreMode())
        raid = false;
    else
        raid = raidRoll < (security == 0 ? 90 : security == 1 ? 65 : security == 2 ? 35 : 10);
    int raidCoords[3] = {Globals::rnd->nextInt(100000) - 50000, 0, Globals::rnd->nextInt(50000) + 50000};
    this->field_180 = new Route(raidCoords, 3);
    int localRace = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
    int raidRace = Globals::rnd->nextInt(100) < 75
                           ? 8
                           : ((Standing *) (intptr_t) Globals::status->getStanding())->getEnemyRace(localRace);
    float raiders = raid ? (float) Globals::rnd->nextInt(4) : 0.0f;
    this->hostileCount = (int) raiders;
    if ((int) raiders >= 1) {
        if (Globals::status->hardCoreMode())
            this->hostileCount = Globals::rnd->nextInt(6) + 2;
        this->hostileCount = (int) ((float) this->hostileCount +
                                    (*(float *) &Globals::options[44] - 0.5f) * (float) this->hostileCount);
        this->hostileCount += Globals::status->getLevel() / 4;
    }
    if (security == 3 && raid && *(float *) &Globals::options[44] < 1.0f)
        this->hostileCount = Globals::rnd->nextInt(2) + 1;
    int extraFriend;
    if (stationIndex == 78) {
        this->field_164 = 0;
        this->field_168 = 0;
        extraFriend = 0;
    } else {
        this->field_164 = Globals::rnd->nextInt(2);
        this->field_168 = Globals::rnd->nextInt(5);
        extraFriend = Globals::rnd->nextInt(2);
    }
    this->friendCount = (introSystem ? Globals::rnd->nextInt(2) : security) + extraFriend + this->field_168 / 4;
    if (raid && Globals::status->getCurrentCampaignMission() >= 32 && Globals::rnd->nextInt(100) <= 7) {
        this->friendCount = 9;
        this->hostileCount = 9;
    }
    unsigned stationPirates = Globals::status->getStation()->stationHasPirateBase();
    int pirateEscorts = (int) (stationPirates ? 5.0f + (*(float *) &Globals::options[44] - 0.5f) * 5.0f : 0.0f);
    if (Globals::status->getSystem() != 0 && ((SolarSystem *) (intptr_t) Globals::status->getSystem())->hasPirateBase()) {
        pirates = Globals::rnd->nextInt(1) + 2;
        if (Globals::status->hardCoreMode())
            pirates = Globals::rnd->nextInt(3) + pirates * 2;
        raid = false;
        this->hostileCount = 0;
    }
    unsigned blueprint = Globals::status->getStation()->stationHasHiddenBlueprint(true);
    if ((Level::comingFromAlienWorld || Globals::status->getStation()->isAttackedByAliens()) &&
        Globals::status->getCurrentCampaignMission() != 42) {
        if (this->field_168 < 2)
            this->field_168 = 2;
        this->hostileCount = Globals::rnd->nextInt(4) + 2;
        if (this->landmarks != nullptr && (*this->landmarks)[3] != nullptr)
            this->field_180->setNewCoords((*this->landmarks)[3]->getPosition());
        raid = true;
        raidRace = 9;
    }
    Wanted *wanted = Globals::status->getWantedInCurrentOrbit();
    bool hasWanted = false;
    if (wanted != nullptr) {
        if (this->friendCount >= 3)
            this->friendCount = 2;
        hasWanted = true;
    }
    float alienChance = 0.0f;
    if (Globals::status->getCurrentCampaignMission() >= 101 && Globals::status->getCurrentCampaignMission() <= 144)
        alienChance = (float) Globals::status->getCurrentCampaignMission() / 144.0f * 15.0f + 5.0f;
    int alienPercent = (int) alienChance << Globals::status->hardCoreMode();
    int extraAliens = 0;
    if (alienPercent >= 1 && Globals::rnd->nextInt(100) < alienPercent)
        extraAliens = (Globals::rnd->nextInt(3) + 2) << Globals::status->hardCoreMode();
    bool nameLeader = false;
    if (freelance != nullptr && freelance->getType() == 13 && Globals::status->field_0xf1 == 0 &&
        freelance->getTargetStation() == Globals::status->getStation()->getIndex()) {
        this->field_164 = 0;
        this->field_168 = 0;
        pirates = 0;
        this->hostileCount = 0;
        raid = false;
        this->friendCount = Globals::status->field_0xf0 == 0 ? 7 : 6;
        nameLeader = Globals::status->field_0xf0 == 0;
    } else {
        if (Globals::status->getStation()->hasAttackedFriends()) {
            if (this->friendCount < 7)
                this->friendCount = 7;
            this->alarmRequested = 1;
            this->field_18a = 1;
        }
        if (this->friendCount + this->field_164 + this->field_168 + this->hostileCount + pirates == 0)
            this->friendCount = 4;
    }
    int currentStation = Globals::status->getStation()->getIndex();
    if (currentStation == 102 || currentStation == 103 || currentStation == 104) {
        this->friendCount = 0;
        this->hostileCount = Globals::rnd->nextInt(5) + 3;
        raid = true;
        raidRace = 8;
        this->field_164 = 0;
        this->field_168 = 0;
    } else if (currentStation == 100 || currentStation == 101 || currentStation == 108 || currentStation == 10) {
        this->friendCount = 0;
        raid = false;
        this->field_164 = 0;
        this->field_168 = 0;
        this->hostileCount = 0;
    }
    if ((Globals::status->getCurrentCampaignMission() == 36 || Globals::status->getCurrentCampaignMission() == 37) &&
        Globals::status->getSystem() != 0 && ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 5) {
        this->hostileCount = 0;
        raid = false;
        this->friendCount = 0;
    }
    if (Globals::status->getCurrentCampaignMission() >= 42 && Globals::status->getCurrentCampaignMission() <= 43 &&
        !Globals::status->inAlienOrbit()) {
        pirates = 0;
        raid = false;
        this->hostileCount = 0;
    }
    if (Globals::status->inBlackMarketSystem()) {
        pirates = 0;
        this->friendCount = 0;
        this->hostileCount = Globals::rnd->nextInt(4) + 6;
        if (Globals::status->hardCoreMode())
            this->hostileCount = Globals::rnd->nextInt(3) + this->hostileCount * 2;
        this->field_168 = 0;
        this->field_164 = 0;
        raid = true;
        raidRace = 8;
    }
    if (Globals::status->inPirateLootOrbit()) {
        pirates = 0;
        this->friendCount = 0;
        this->hostileCount = Globals::rnd->nextInt(4) + 10;
        if (Globals::status->hardCoreMode())
            this->hostileCount = Globals::rnd->nextInt(3) + this->hostileCount * 2;
        this->field_168 = 0;
        this->field_164 = 0;
        raid = true;
        raidRace = 8;
    }
    if (Globals::status->getStation()->getIndex() == 108) {
        if (Globals::status->field_114 == 0) {
            this->friendCount = 0;
            this->field_288 = 1;
            this->hostileCount = Globals::status->hardCoreMode() ? 8 : 6;
            this->field_164 = 0;
            this->field_168 = 0;
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(this->hostileCount + 4, *this->enemies);
            for (int i = 0; i < 4; i++) {
                (*this->enemies)[i] = this->createStaticObject(nullptr, 14243, true);
                (*this->enemies)[i]->name = *Globals::gameText->getText(441);
            }
            (*this->enemies)[0]->setPosition(45000.0f, 5000.0f, 55000.0f);
            (*this->enemies)[1]->setPosition(80000.0f, -30000.0f, -12000.0f);
            (*this->enemies)[2]->setPosition(-15000.0f, 45000.0f, 55000.0f);
            (*this->enemies)[3]->setPosition(-65000.0f, -20000.0f, -45000.0f);
            for (unsigned i = 4; i < this->enemies->size(); i++)
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8), nullptr, true, false);
            Mission *defense = new Mission(4, *Globals::gameText->getText(1601), g_tommyFace, 4, 0, 108, 0);
            Globals::status->setFreelanceMission(defense);
            Globals::status->setMission(defense);
            this->objectivesA = new Objective(18, 0, this->enemies->size(), this);
            return;
        }
        raid = false;
        this->hostileCount = 0;
    }
    bool siegedStation = false;
    if (Globals::status->getCampaignMission() != 0 &&
        ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() == 163 &&
        Globals::status->field_90 != nullptr) {
        for (unsigned i = 0; i < Globals::status->field_90->size(); i++) {
            if ((*Globals::status->field_90)[i] == Globals::status->getStation()->getIndex()) {
                siegedStation = true;
                break;
            }
        }
    }
    int wingCoords[3] = {Globals::rnd->nextInt(20000) - 10000, Globals::rnd->nextInt(20000) - 10000,
                         Globals::rnd->nextInt(30000) + 20000};
    Route *wingRoute = new Route(wingCoords, 3);
    if (siegedStation) {
        this->hostileCount = 0;
        this->field_164 = 0;
        this->enemies = new Array<KIPlayer *>();
        ArraySetLength(this->friendCount + 8, *this->enemies);
        int coords[3];
        coords[0] = (Globals::rnd->nextInt(30000) + 80000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
        coords[1] = Globals::rnd->nextInt(3000) - 6000;
        coords[2] = Globals::rnd->nextInt(50000) + 120000;
        this->playerRoute = new Route(coords, 3);
        this->enemyRoute = this->playerRoute->clone();
        this->enemyRoute->setLoop(true);
        (*this->enemies)[0] = this->createShip(localRace, 1, localRace == 1 ? 13 : 15, this->playerRoute->getWaypoint(),
                                               true, false);
        ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(false);
        (*this->enemies)[0]->setPosition((float) coords[0], (float) coords[1], (float) coords[2]);
        if ((*this->enemies)[0]->cargo != nullptr) {
            int good = Globals::rnd->nextInt(6);
            switch (good) {
                case 3:
                    good = 36;
                    break;
                case 4:
                    good = 22;
                    break;
                case 5:
                    good = 23;
                    break;
            }
            (*(*this->enemies)[0]->cargo)[0] = good;
            (*this->enemies)[0]->name = *Globals::gameText->getText(1664);
        }
        for (int i = 1; i < 6; i++) {
            (*this->enemies)[i] = this->createShip(localRace, 0, Globals::globals->getRandomEnemyFighter(localRace),
                                                   this->playerRoute->getWaypoint(), true, false);
            (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
        }
        Matrix rotation;
        Vector v = {0.0f, 0.0f, 0.0f};
        Vector base = (*this->enemies)[0]->getPosition();
        for (int k = 0; k < 2; k++) {
            (*this->enemies)[6 + k] = this->createStaticObject(nullptr, 14363, true);
            KIPlayer *gun = (*this->enemies)[6 + k];
            gun->player->setMaxHitpoints(1000);
            const float *t = kStationTurrets[localRace][k];
            v = MatrixRotateVector(rotation, Vector{t[4], t[5], t[6]});
            gun->geometry->setRotation(v);
            v = MatrixRotateVector(rotation, Vector{t[1], t[2], t[3]});
            gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
            ((PlayerTurret *) gun)->setScaling(0.3f);
            gun->shipGroup = localRace;
        }
        for (int i = 8; i < this->friendCount + 8; i++)
            (*this->enemies)[i] = this->createShip(localRace, 0, Globals::globals->getRandomEnemyFighter(localRace),
                                                   wingRoute->getWaypoint(), true, false);
        return;
    }
    bool convoy = Globals::status->getCampaignMission() != 0 &&
                  ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() == 167 &&
                  Globals::status->getStation()->getIndex() ==
                          ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation();
    bool alienWave = false;
    bool bigAlienWave = false;
    bool pirateFortress = false;
    if (localRace == 0 && this->field_168 > 0) {
        if (Globals::rnd->nextInt(100) <= 29) {
            if (Globals::rnd->nextInt(100) <= 29)
                bigAlienWave = Globals::status->getCurrentCampaignMission() > 103;
            alienWave = true;
        }
    } else if (localRace == 1 && this->field_168 >= 1 && Globals::rnd->nextInt(100) <= 29) {
        pirateFortress = Globals::status->getCurrentCampaignMission() > 140;
    }
    if (Globals::status->inSupernovaSystem()) {
        this->friendCount = 0;
        this->field_164 = 0;
        alienWave = false;
        this->field_168 = 0;
        this->hostileCount = 0;
        raid = false;
    }
    if (hasWanted)
        this->friendCount += wanted->getNumWingmen() + 1;
    int alienGuns = alienWave ? (bigAlienWave ? 8 : 7) : 0;
    int fortressGuns = pirateFortress ? 5 : 0;
    this->enemies = new Array<KIPlayer *>();
    ArraySetLength(pirateEscorts + stationPirates + blueprint + extraAliens + pirates + convoy + fortressGuns + alienGuns +
                           this->friendCount + this->field_164 + this->field_168 + this->hostileCount,
                   *this->enemies);
    for (int i = 0; i < this->friendCount; i++) {
        if (i == 0 && hasWanted) {
            int race = wanted->getRace() <= 3 ? wanted->getRace() : 8;
            (*this->enemies)[0] = this->createShip(race, 0, wanted->getShip(), wingRoute->getWaypoint(), true, false);
            int hp = wanted->getHitpoints() + (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 15 +
                     (Globals::status->gameWon() ? 45 : Globals::status->getCurrentCampaignMission()) * 4;
            hp = (int) ((float) hp + (*(float *) &Globals::options[44] - 0.5f) * (float) hp);
            (*this->enemies)[0]->player->setMaxHitpoints(hp);
            (*this->enemies)[0]->player->setHitpoints(hp);
            (*this->enemies)[0]->name = wanted->getName();
            delete (*this->enemies)[0]->cargo;
            (*this->enemies)[0]->cargo = nullptr;
            (*this->enemies)[0]->cargo = new Array<int>();
            ArrayAdd(wanted->getLoot(), *(*this->enemies)[0]->cargo);
            ArrayAdd(wanted->getLootAmount(), *(*this->enemies)[0]->cargo);
            (*this->enemies)[0]->field_0x42 = 1;
            (*this->enemies)[0]->field_0x48 = wanted->getIndex();
            (*this->enemies)[0]->setSpeed(4.5f);
            int wingmanHp = hp / 2;
            for (int j = 1; j <= wanted->getNumWingmen(); j++) {
                (*this->enemies)[j] = this->createShip(race, 0, Globals::globals->getRandomEnemyFighter(race),
                                                       wingRoute->getWaypoint(), true, false);
                (*this->enemies)[j]->player->setMaxHitpoints(wingmanHp);
                (*this->enemies)[j]->player->setHitpoints(wingmanHp);
            }
        } else if (!hasWanted || i > wanted->getNumWingmen()) {
            (*this->enemies)[i] = this->createShip(localRace, 0, Globals::globals->getRandomEnemyFighter(localRace),
                                                   wingRoute->getWaypoint(), true, false);
        }
    }
    for (int i = this->friendCount; i < this->friendCount + this->field_164; i++) {
        (*this->enemies)[i] = this->createShip(localRace, 0, Globals::globals->getRandomEnemyFighter(localRace), nullptr,
                                               true, false);
        (*this->enemies)[i]->setDead();
        int jumpCoords[3] = {Globals::rnd->nextInt(400000) - 200000, Globals::rnd->nextInt(200000) - 100000,
                             Globals::rnd->nextInt(100000) + 50000};
        (*this->enemies)[i]->setRoute(new Route(jumpCoords, 3));
        (*this->enemies)[i]->setJumper(true);
    }
    int stationRace = localRace == 2 ? 0 : localRace;
    for (int i = this->friendCount + this->field_164; i < this->friendCount + this->field_164 + this->field_168; i++) {
        if (i == this->friendCount + this->field_164 && alienWave) {
            if (bigAlienWave) {
                Vector pos = {(float) (Globals::rnd->nextInt(80000) - 40000), (float) (Globals::rnd->nextInt(40000) - 20000),
                              (float) (Globals::rnd->nextInt(80000) + 100000)};
                (*this->enemies)[i] =
                        this->createStaticObject(new Waypoint((int) pos.x, (int) pos.y, (int) pos.z, nullptr), 18804, true);
                (*this->enemies)[i]->setSpacePoints(nullptr);
                (*this->enemies)[i]->setPosition(pos);
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                (*this->enemies)[i]->player->setHitpoints(9999999);
                delete (*this->enemies)[i]->cargo;
                (*this->enemies)[i]->cargo = nullptr;
                (*this->enemies)[i]->field_0x70 = 0;
            } else {
                (*this->enemies)[i] = this->createShip(localRace, 1, 14, nullptr, true, false);
            }
            ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
            (*this->enemies)[i]->setPosition((float) (Globals::rnd->nextInt(80000) - 40000),
                                             (float) (Globals::rnd->nextInt(10000) - 5000),
                                             (float) (Globals::rnd->nextInt(80000) + 40000));
        } else if (i == this->friendCount + this->field_164 && pirateFortress) {
            Vector pos = {(float) (Globals::rnd->nextInt(80000) - 40000), (float) (Globals::rnd->nextInt(40000) - 20000),
                          (float) (Globals::rnd->nextInt(80000) + 60000)};
            (*this->enemies)[i] =
                    this->createStaticObject(new Waypoint((int) pos.x, (int) pos.y, (int) pos.z, nullptr), 19051, true);
            (*this->enemies)[i]->setSpacePoints(nullptr);
            (*this->enemies)[i]->setPosition(pos);
            ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
            (*this->enemies)[i]->player->setHitpoints(9999999);
            ((PlayerFixedObject *) (*this->enemies)[i])->setName(*Globals::gameText->getText(1667));
            (*this->enemies)[i]->field_0x70 = 0;
            delete (*this->enemies)[i]->cargo;
            (*this->enemies)[i]->cargo = nullptr;
        } else {
            int race = Globals::rnd->nextInt(100) > 29 ? localRace : (localRace == 0 ? 2 : stationRace);
            (*this->enemies)[i] = this->createShip(race, 1, race == 1 ? 13 : 15, nullptr, true, false);
            ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(true);
            KIPlayer *station = (*this->enemies)[i];
            int x = (Globals::rnd->nextInt(60000) - 80000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
            station->setPosition((float) x, (float) (Globals::rnd->nextInt(40000) - 20000),
                                 (float) (Globals::rnd->nextInt(160000) - 80000));
        }
    }
    if (alienGuns != 0) {
        Matrix rotation;
        Vector v = {0.0f, 0.0f, 0.0f};
        Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
        int k = 0;
        for (int i = this->friendCount + this->field_164 + this->field_168;
             i < this->friendCount + alienGuns + this->field_164 + this->field_168; i++, k++) {
            (*this->enemies)[i] = this->createStaticObject(nullptr, 6772, true);
            KIPlayer *gun = (*this->enemies)[i];
            gun->player->setMaxHitpoints(1000);
            const float *t = bigAlienWave ? kAlienStationTurrets[k] : kAlienOutpostTurrets[k];
            v = MatrixRotateVector(rotation, Vector{t[3], t[4], t[5]});
            gun->geometry->setRotation(v);
            v = MatrixRotateVector(rotation, Vector{t[0], t[1], t[2]});
            gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
            ((PlayerTurret *) gun)->setScaling(6.0f);
            gun->cargo = nullptr;
            gun->shipGroup = localRace;
        }
    }
    if (pirateFortress) {
        Matrix rotation;
        Vector v = {0.0f, 0.0f, 0.0f};
        Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
        int k = 0;
        for (int i = this->friendCount + this->field_164 + this->field_168;
             i < fortressGuns + this->friendCount + this->field_164 + this->field_168; i++, k++) {
            (*this->enemies)[i] = this->createStaticObject(nullptr, 6774, true);
            KIPlayer *gun = (*this->enemies)[i];
            gun->player->setMaxHitpoints(1000);
            v = MatrixRotateVector(rotation, Vector{kBaseTurrets[k][3], kBaseTurrets[k][4], kBaseTurrets[k][5]});
            gun->geometry->setRotation(v);
            v = MatrixRotateVector(rotation, Vector{kBaseTurrets[k][0], kBaseTurrets[k][1], kBaseTurrets[k][2]});
            gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
            ((PlayerTurret *) gun)->setScaling(6.0f);
            gun->cargo = nullptr;
            gun->shipGroup = 1;
        }
    }
    int guns = alienGuns + fortressGuns;
    if (raid) {
        int raidType = Globals::globals->getRandomEnemyFighter(raidRace);
        bool hard = Globals::status->hardCoreMode();
        if (raidRace == 8 && Level::initStreamOutPosition &&
            Globals::rnd->nextInt(100) < (hard ? 40 : 20) + Globals::status->getLevel()) {
            this->spawnPosition = this->player->getPosition() / 3.0f;
            this->field_180->setNewCoords(this->spawnPosition);
        }
        for (int i = guns + this->friendCount + this->field_164 + this->field_168;
             i < guns + this->friendCount + this->field_164 + this->field_168 + this->hostileCount; i++) {
            (*this->enemies)[i] = this->createShip(raidRace, 0, raidType, this->field_180->getWaypoint(), true, false);
            if (Globals::status->inPirateLootOrbit()) {
                Player *p = (*this->enemies)[i]->player;
                p->setHitpoints(p->getMaxHitpoints() * 2);
                (*this->enemies)[i]->setSpeed(3.5f);
                ((PlayerFighter *) (*this->enemies)[i])->setRotate(3);
                this->spawnPosition = this->player->getPosition();
                this->spawnPosition.x += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
                this->spawnPosition.y += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000);
                this->spawnPosition.z += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
                (*this->enemies)[i]->setPosition(this->spawnPosition);
            }
        }
    }
    for (int i = guns + this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
         i < guns + pirates + this->friendCount + this->field_164 + this->field_168 + this->hostileCount; i++) {
        (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8), nullptr, true, false);
        this->spawnPosition = this->player->getPosition();
        (*this->enemies)[i]->setPosition(this->spawnPosition.x - 80000.0f + (float) Globals::rnd->nextInt(160000),
                                         this->spawnPosition.y - 50000.0f + (float) Globals::rnd->nextInt(100000),
                                         this->spawnPosition.z - 80000.0f + (float) Globals::rnd->nextInt(160000));
    }
    if (stationPirates) {
        Vector pirateBase = {0.0f, 0.0f, 0.0f};
        int first = guns + stationPirates;
        for (int i = guns + pirates + this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
             i < first + pirates + this->friendCount + this->field_164 + this->field_168 + this->hostileCount; i++) {
            const int *c = kPirateStationCoords[Globals::status->getStation()->getPirateStationIndex()];
            int coords[3] = {c[0], c[1], c[2]};
            Route *route = new Route(coords, 3);
            (*this->enemies)[i] = this->createStaticObject(route->getWaypoint(), 14243, true);
            pirateBase = route->getWaypoint()->getPosition();
            (*this->enemies)[i]->setToSleep();
            (*this->enemies)[i]->name = *Globals::gameText->getText(441);
        }
        int escortsEnd = pirateEscorts + first + pirates;
        for (int i = first + pirates + this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
             i < escortsEnd + this->friendCount + this->field_164 + this->field_168 + this->hostileCount; i++) {
            (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8), nullptr, true, false);
            int x = Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000;
            int y = Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000;
            int z = Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000;
            (*this->enemies)[i]->setPosition((int) (pirateBase.x + (float) x), (int) (pirateBase.y + (float) y),
                                             (int) (pirateBase.z + (float) z));
            (*this->enemies)[i]->setToSleep();
            ((PlayerFighter *) (*this->enemies)[i])->field_0x12f = 1;
        }
    }
    if (extraAliens >= 1) {
        this->spawnPosition = this->player->getPosition();
        this->spawnPosition.x += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
        this->spawnPosition.y += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000);
        this->spawnPosition.z += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
        int coords[3] = {(int) this->spawnPosition.x, (int) this->spawnPosition.y, (int) this->spawnPosition.z};
        Route *route = new Route(coords, 3);
        int start = stationPirates + guns + pirateEscorts;
        for (int i = pirates + start + this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
             i < start + extraAliens + pirates + this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
             i++) {
            (*this->enemies)[i] = this->createShip(10, 0, 44, route->getWaypoint(), true, false);
            (*this->enemies)[i]->player->setAlwaysEnemy(true);
        }
        delete route;
    }
    if (convoy) {
        this->miningPlantIndex = stationPirates + guns + pirateEscorts + extraAliens + pirates + this->friendCount +
                                 this->field_164 + this->field_168 + this->hostileCount;
        (*this->enemies)[this->miningPlantIndex] =
                this->createStaticObject(new Waypoint(-90000, 0, 20000, nullptr), 19080, true);
        ((PlayerFixedObject *) (*this->enemies)[this->miningPlantIndex])->setDockingType(1);
        ((PlayerFixedObject *) (*this->enemies)[this->miningPlantIndex])->setMoving(false);
        (*this->enemies)[this->miningPlantIndex]->setPosition(this->stationPosition);
        (*this->enemies)[this->miningPlantIndex]->player->setAlwaysFriend(true);
        ((PlayerFixedObject *) (*this->enemies)[this->miningPlantIndex])->setName(*Globals::gameText->getText(3210));
    }
    if (blueprint) {
        int index = Globals::status->getStation()->getHiddenBlueprintIndex();
        int slot = extraAliens + pirateEscorts + stationPirates + pirates + convoy + fortressGuns + alienGuns +
                   this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
        (*this->enemies)[slot] = this->createShip(kBlueprintRaces[index], 1, index == 0 ? 13 : 15, nullptr, true, false);
        PlayerFixedObject *wreck = (PlayerFixedObject *) (*this->enemies)[slot];
        wreck->setDockingType(3);
        wreck->field_0x70 = Globals::status->getStation()->stationHasHiddenBlueprint(false);
        wreck->setPosition(this->stationPosition);
        wreck->setDeadButSelectable();
        (*this->enemies)[slot]->setPosition(kBlueprintPositions[index][0], kBlueprintPositions[index][1],
                                            kBlueprintPositions[index][2]);
        delete wreck->cargo;
        wreck->cargo = nullptr;
        if (Globals::status->getStation()->stationHasHiddenBlueprint(false)) {
            Generator *generator = new Generator();
            wreck->cargo = generator->getLootList(115, 1);
            delete generator;
        }
        FileRead *fileRead = new FileRead();
        Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(kBlueprintSpacePoints[index], -1);
        delete fileRead;
        wreck->setSpacePoints(spacePoints);
    }
    if (nameLeader)
        (*this->enemies)[0]->name = *Globals::gameText->getText(1663);
}

void Level::createAsteroids() {
    using namespace AbyssEngine::AEMath;
    static const int asteroidMeshes[4] = {16900, 16921, 16904, 18836};
    static const int lodDistancesIPad[3] = {100000, 120000, 150000};
    static const int lodDistancesNear[3] = {60000, 100000, 120000};
    static const int lodDistancesFar[3] = {40000, 70000, 90000};

    int systemVariant = Globals::status->inAlienOrbit() ? 0
            : (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 22) << 1;
    asteroids = new Array<KIPlayer *>();
    int *probabilities = Globals::galaxy->getAsteroidProbabilities(Globals::status->getStation());

    // the field layout is seeded by the station so it stays the same between visits
    Globals::rnd->setSeed(Globals::status->getStation()->getIndex());
    ArraySetLength(Globals::rnd->nextInt(40) + 40, *asteroids);
    int randomX = Globals::rnd->nextInt(100000);
    int randomY = Globals::rnd->nextInt(100000);
    int randomZ = Globals::rnd->nextInt(100000);
    bool alienOrbit = Globals::status->inAlienOrbit();
    int mission = Globals::status->getCurrentCampaignMission();
    int x, y, z;
    if (alienOrbit) {
        x = mission == 0x9a ? -70000 : -30000;
        y = 0;
        z = 30000;
    } else if (mission == 0x72 && Globals::status->getStation()->getIndex() == 0x53) {
        x = 30000;
        y = 0;
        z = 80000;
    } else if (Globals::status->getCurrentCampaignMission() == 0x59 && Globals::status->inSupernovaOrbit()) {
        x = -100000;
        y = 0;
        z = -50000;
    } else if (Globals::status->getCurrentCampaignMission() == 0x5b && Globals::status->getStation()->getIndex() == 0x6e) {
        x = 60000;
        y = 0;
        z = 50000;
    } else if (Globals::status->getCurrentCampaignMission() == 0x91 && Globals::status->getStation()->getIndex() == 0x70) {
        x = 50000;
        y = 0;
        z = 70000;
    } else {
        z = randomZ + 20000;
        y = randomY - 50000;
        x = randomX - 50000;
        if (Globals::status->getCurrentCampaignMission() == 0 && missionPtr == 3) {
            x = 0;
            y = 0;
            z = 0;
        }
    }
    Globals::rnd->reset();

    reinterpret_cast<Vector &>(stationPosition.x) = Vector{(float) x, (float) y, (float) z};
    asteroidWaypoint = new Waypoint(x, y, z, nullptr);
    collisionVolume = new BoundingSphere((float) x, (float) y, (float) z, 0.0f, 0.0f, 0.0f, 100000.0f);

    int denseCount = Globals::rnd->nextInt(8) + 2;
    bool alienField = Globals::status->inAlienOrbit();
    Vector position = {0.0f, 0.0f, 0.0f};
    int kind = 0x9a;
    int cursor = 0;
    for (unsigned int i = 0; i < asteroids->size(); i++) {
        if (alienField) {
            kind = 0xa4;
        } else {
            bool found = false;
            while (!found) {
                int roll = Globals::rnd->nextInt(100);
                int next = 0;
                found = false;
                if (roll < probabilities[cursor + 1]) {
                    kind = probabilities[cursor];
                    next = cursor > 8 ? 0 : cursor + 2;
                    found = kind == 0xd9 || kind < 0xa4;
                }
                cursor = next;
            }
        }

        bool dense = (int) i < denseCount;
        int spread = dense ? 60000 : 100000;
        float half = (float) (spread / 2);
        int variant = systemVariant;
        if (Globals::status->inAlienOrbit()) {
            variant = 1;
        }
        if (kind == 0xd9) {
            variant = 3;
        }
        bool checkSpacing = i != 0 && dense;
        int mesh = asteroidMeshes[variant];
        for (;;) {
            position.x = stationPosition.x - half + (float) Globals::rnd->nextInt(spread);
            position.y = stationPosition.y - half + (float) Globals::rnd->nextInt(spread);
            position.z = stationPosition.z - half + (float) Globals::rnd->nextInt(spread);
            if (!checkSpacing) {
                break;
            }
            bool spaced = false;
            for (unsigned int j = 0; j < i; j++) {
                int distance = (int) VectorLength((*asteroids)[j]->getPosition() - position);
                spaced = spaced || distance > 8000;
                if (distance > 8000) {
                    break;
                }
            }
            if (spaced) {
                break;
            }
        }

        AEGeometry *geometry = new AEGeometry(mesh, Globals::Canvas, false);
        unsigned short lodMeshes[3] = {(unsigned short) (mesh + 1), (unsigned short) (mesh + 2),
                                       (unsigned short) (mesh + 3)};
        if (Globals::iPad) {
            int distances[3] = {lodDistancesIPad[0], lodDistancesIPad[1], lodDistancesIPad[2]};
            geometry->setLodMeshes(lodMeshes, distances, 3);
            lodManager->addObject(geometry);
        } else if ((int) i < denseCount) {
            int distances[3] = {lodDistancesNear[0], lodDistancesNear[1], lodDistancesNear[2]};
            geometry->setLodMeshes(lodMeshes, distances, 3);
            lodManager->addObject(geometry);
        } else {
            int distances[3] = {lodDistancesFar[0], lodDistancesFar[1], lodDistancesFar[2]};
            geometry->setLodMeshes(lodMeshes, distances, 3);
            lodManager->addObject(geometry);
        }
        int scale;
        if ((int) i < denseCount) {
            scale = Globals::rnd->nextInt(100) + 120;
        } else {
            scale = Globals::rnd->nextInt(70) + 30;
        }
        float size = (float) scale;
        size *= 0.01f;
        int type;
        if (size < 0.4) {
            type = 4;
        } else if (size < 0.7) {
            type = 5;
        } else if (size < 0.92) {
            type = 6;
        } else if (Globals::rnd->nextInt(2) != 0) {
            type = Globals::rnd->nextInt(3) + 4;
        } else {
            type = 7;
        }
        (*asteroids)[i] = new PlayerAsteroid(mesh, geometry, variant, kind, position, size, type);
        (*asteroids)[i]->setLevel(this);
        ((PlayerAsteroid *) (*asteroids)[i])->setAsteroidCenter(reinterpret_cast<Vector &>(stationPosition.x));
    }
    if (probabilities != nullptr) {
        delete[] probabilities;
    }
}

void Level::createCampaignMission() {
    // Carrier hardpoints: {isLarge, position.xyz, rotation.xyz}.
    static const float kCarrierTurrets[12][7] = {
        {0.0f, -3994.97f, 23359.0f, -7378.1f, 0.0f, 0.0f, 1.5707964f},
        {0.0f, 3994.96f, 23359.0f, -7378.1f, 0.0f, 0.0f, -1.5707964f},
        {1.0f, 1988.03f, -37327.1f, -4511.46f, 0.0f, 0.0f, -1.5707964f},
        {1.0f, -1995.02f, -37327.1f, -4511.46f, 0.0f, 0.0f, 1.5707964f},
        {0.0f, -3264.75f, -22848.6f, 791.524f, 0.0f, 0.0f, 1.5707964f},
        {0.0f, 3273.29f, -22848.6f, 791.524f, 0.0f, 0.0f, -1.5707964f},
        {0.0f, -29726.7f, -10994.8f, -3765.53f, 0.0f, 0.0f, 3.1415927f},
        {0.0f, 29716.2f, -10994.8f, -3765.53f, 0.0f, 0.0f, 3.1415927f},
        {0.0f, 29716.2f, 4854.22f, -3758.77f, 0.0f, 0.0f, 0.0f},
        {1.0f, 17013.0f, -764.391f, -1690.65f, 0.0f, 0.0f, 0.0f},
        {1.0f, -17013.3f, -764.512f, -1690.65f, 0.0f, 0.0f, 0.0f},
        {0.0f, -29726.7f, 4854.22f, -3758.77f, 0.0f, 0.0f, 0.0f},
    };
    Vector playerPos = this->player->geometry->getPosition();

    switch (Globals::status->getCurrentCampaignMission()) {
        case 0:
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(3, *this->enemies);
            for (unsigned i = 0; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(8, 0, i == 1 ? 23 : 2, nullptr, true, false);
                (*this->enemies)[i]->setToSleep();
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                (*this->enemies)[i]->setPosition(50000.0f, 50000.0f, 50000.0f);
                (*this->enemies)[i]->cargo = nullptr;
                ((PlayerFighter *) (*this->enemies)[i])->crateCaptured() = 0;
                (*this->enemies)[i]->player->setHitpoints(150);
                if (i <= 2)
                    ((PlayerFighter *) (*this->enemies)[i])->setExhaustVisible(false);
            }
            this->player->player->setHitpoints(9999999);
            Globals::status->field_64 = 9999999;
            this->player->player->field_5e = 1;
            break;
        case 1: {
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            (*this->enemies)[0] = this->createShip(3, 0, 30, nullptr, true, false);
            (*this->enemies)[0]->setPosition(300.0f, 50.0f, -6000.0f);
            int coords[6] = {0, 0, -5000, 0, 0, 0};
            this->enemyRoute = new Route(coords, 6);
            (*this->enemies)[0]->setRoute(this->enemyRoute);
            (*this->enemies)[0]->setSpeed(0.0f);
            ((PlayerFighter *) (*this->enemies)[0])->setExhaustVisible(false);
            break;
        }
        case 4:
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            (*this->enemies)[0] = this->createShip(8, 0, 2, nullptr, true, false);
            (*this->enemies)[0]->setInitActive(false);
            (*this->enemies)[0]->player->setAlwaysEnemy(true);
            (*this->enemies)[0]->setPosition(0.0f, 0.0f, -200000.0f);
            (*this->enemies)[0]->setToSleep();
            break;
        case 7: {
            int coords[6] = {-4000, -3000, 80000, 10000, 7000, 160000};
            this->playerRoute = new Route(coords, 6);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(4, *this->enemies);
            for (int i = 0; i < 3; i++) {
                (*this->enemies)[i] = this->createShip(8, 0, 2, this->playerRoute->getWaypoint(1), true, false);
                (*this->enemies)[i]->setToSleep();
            }
            (*this->enemies)[3] = this->createShip(3, 0, 30, nullptr, true, false);
            (*this->enemies)[3]->player->setAlwaysFriend(true);
            (*this->enemies)[3]->setPosition(playerPos.x + 700.0f, playerPos.y + 50.0f, playerPos.z + 6000.0f);
            (*this->enemies)[3]->setRoute(this->playerRoute->clone());
            (*this->enemies)[3]->player->setHitpoints(9999999);
            (*this->enemies)[3]->name = *Globals::gameText->getText(1599);
            ((PlayerFighter *) (*this->enemies)[3])->setBoostProb(0);
            this->objectivesA = new Objective(18, 0, 3, this);
            break;
        }
        case 14: {
            int coords[3] = {40000, 5000, 30000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(7, *this->enemies);
            for (int i = 0; i < 3; i++)
                (*this->enemies)[i] = this->createShip(8, 0, 0, this->enemyRoute->getWaypoint(), true, false);
            for (int i = 3; i < 5; i++)
                (*this->enemies)[i] = this->createShip(0, 0, 5, this->enemyRoute->getWaypoint(), true, false);
            for (int i = 5; i < 7; i++)
                (*this->enemies)[i] = this->createShip(0, 1, 14, this->enemyRoute->getWaypoint(), true, false);
            Vector pos = this->enemyRoute->getWaypoint()->getPosition();
            pos.x += 7000.0f;
            (*this->enemies)[this->enemies->size() - 2]->setPosition(pos);
            pos = this->enemyRoute->getWaypoint()->getPosition();
            pos.x -= 9000.0f;
            pos.y += 2000.0f;
            pos.z += 7000.0f;
            (*this->enemies)[this->enemies->size() - 1]->setPosition(pos);
            this->player->setPosition(40000.0f, 0.0f, 120000.0f);
            this->objectivesA = new Objective(31, 0, this);
            break;
        }
        case 16: {
            int coords[3] = {0, 0, 170000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(10, *this->enemies);
            Waypoint *wp = this->enemyRoute->getWaypoint();
            for (int i = 0; i < 3; i++) {
                (*this->enemies)[i] = this->createShip(0, 1, 15, this->enemyRoute->getWaypoint(), true, false);
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
            (*this->enemies)[1]->setPosition((float) (wp->x + 3000), (float) (wp->y + 2000), (float) (wp->z - 3000));
            (*this->enemies)[2]->setPosition((float) (wp->x - 9000), (float) (wp->y - 8000), (float) (wp->z - 7000));
            for (int i = 3; i < 7; i++) {
                (*this->enemies)[i] = this->createShip(9, 0, 8, this->enemyRoute->getWaypoint(), true, false);
                Player *p = (*this->enemies)[i]->player;
                p->setMaxHitpoints(p->getMaxHitpoints() * 10);
                (*this->enemies)[i]->setPosition((float) (wp->x + Globals::rnd->nextInt(20000) - 10000),
                                                 (float) (wp->y + Globals::rnd->nextInt(20000) - 10000),
                                                 (float) (wp->z + 50000));
            }
            for (int i = 7; i < 10; i++) {
                (*this->enemies)[i] = this->createShip(0, 0, 5, nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                (*this->enemies)[i]->player->setHitpoints(600);
                (*this->enemies)[i]->setPosition(playerPos.x + (float) (Globals::rnd->nextInt(4000) - 2000),
                                                 playerPos.y + (float) (Globals::rnd->nextInt(3400) - 1700),
                                                 playerPos.z + 2000.0f + (float) (Globals::rnd->nextInt(4000) - 2000));
            }
            if ((*this->landmarks)[3] != nullptr) {
                (*this->landmarks)[3]->setVisible(true);
                (*this->landmarks)[3]->setPosition((float) wp->x, (float) wp->y, (float) (wp->z + 40000));
            }
            this->objectivesA = new Objective(22, 0, this);
            break;
        }
        case 21: {
            int coords[6] = {40000, -40000, 120000, -10000, 20000, 190000};
            this->playerRoute = new Route(coords, 6);
            int fighterCoords[6] = {40000, -40000, 120000};
            Route *fighterRoute = new Route(fighterCoords, 3);
            fighterRoute->setLoop(true);
            int leaderCoords[6] = {-10000, 20000, 190000};
            Route *leaderRoute = new Route(leaderCoords, 3);
            leaderRoute->setLoop(true);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(4, *this->enemies);
            (*this->enemies)[1] = this->createShip(0, 0, 5, fighterRoute->getWaypoint(), true, false);
            (*this->enemies)[2] = this->createShip(0, 0, 5, fighterRoute->getWaypoint(), true, false);
            (*this->enemies)[3] = this->createShip(0, 0, 5, fighterRoute->getWaypoint(), true, false);
            (*this->enemies)[0] = this->createShip(0, 0, 17, leaderRoute->getWaypoint(), true, false);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1611);
            (*this->enemies)[0]->player->setAlwaysEnemy(true);
            for (unsigned i = 0; i < this->enemies->size(); i++) {
                (*this->enemies)[i]->setToSleep();
                (*this->enemies)[i]->setRoute(i == 0 ? leaderRoute : fighterRoute->clone());
                (*this->enemies)[i]->geometry->setRotation(0.0f, 3.1415927f, 0.0f);
            }
            (*this->enemies)[0]->setPosition((float) (leaderRoute->getWaypoint()->x + 1000),
                                             (float) leaderRoute->getWaypoint()->y,
                                             (float) (leaderRoute->getWaypoint()->z + 2000));
            this->objectivesA = new Objective(22, 0, this);
            this->objectivesB = new Objective(7, 1, this);
            delete fighterRoute;
            break;
        }
        case 24: {
            int coords[6] = {100000, 0, 0, 100000, 0, -30000};
            this->enemyRoute = new Route(coords, 6);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(5, *this->enemies);
            for (int i = 0; i < 3; i++) {
                (*this->enemies)[i] = this->createShip(9, 0, 8, this->enemyRoute->getWaypoint(), true, false);
                (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
            }
            for (int i = 3; i < 5; i++) {
                (*this->enemies)[i] = this->createShip(2, 1, 15, this->enemyRoute->getWaypoint(), true, false);
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                Player *p = (*this->enemies)[i]->player;
                p->setMaxHitpoints(p->getMaxHitpoints() / 3);
                delete (*this->enemies)[i]->cargo;
                (*this->enemies)[i]->cargo = nullptr;
            }
            (*this->landmarks)[3]->setVisible(false);
            this->objectivesA = new Objective(31, 0, this);
            break;
        }
        case 25:
        case 29:
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(3, *this->enemies);
            for (int i = 0; i < 3; i++) {
                (*this->enemies)[i] = this->createShip(9, 0, 8, nullptr, true, false);
                int x = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * (Globals::rnd->nextInt(80000) + 20000);
                int y = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * (Globals::rnd->nextInt(80000) + 20000);
                int z = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * (Globals::rnd->nextInt(80000) + 20000);
                (*this->enemies)[i]->setPosition((float) x, (float) y, (float) z);
            }
            break;
        case 26:
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(2, *this->enemies);
            for (int i = 0; i < 2; i++) {
                (*this->enemies)[i] = this->createShip(9, 0, 8, nullptr, true, false);
                Vector pos = this->player->getPosition();
                pos -= VectorNormalize(this->player->geometry->getDirection()) * 8000.0f;
                pos.x += (float) (Globals::rnd->nextInt(1400) - 700);
                pos.y += (float) (Globals::rnd->nextInt(1400) - 700);
                pos.z += (float) (Globals::rnd->nextInt(1400) - 700);
                (*this->enemies)[i]->setPosition(pos);
            }
            this->objectivesA = new Objective(7, 2, this);
            break;
        case 28: {
            (*this->landmarks)[3]->setPosition(100000.0f, -50000.0f, -40000.0f);
            this->spawnPosition = (*this->landmarks)[3]->getPosition();
            int coords[3] = {(int) this->spawnPosition.x, (int) this->spawnPosition.y, (int) this->spawnPosition.z};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(8, *this->enemies);
            for (int i = 0; i < 5; i++)
                (*this->enemies)[i] = this->createShip(9, 0, 8, this->enemyRoute->getWaypoint(), true, false);
            for (int i = 5; i < 8; i++) {
                (*this->enemies)[i] = this->createShip(0, 1, 15, this->enemyRoute->getWaypoint(), true, false);
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                Player *p = (*this->enemies)[i]->player;
                p->setMaxHitpoints(p->getMaxHitpoints() / 4);
                delete (*this->enemies)[i]->cargo;
                (*this->enemies)[i]->cargo = nullptr;
            }
            break;
        }
        case 36: {
            Globals::status->getMission()->setAgent(
                    new Agent(-1, *Globals::gameText->getText(1604), 29, 5, 1, true, -1, -1, -1, -1));
            int coords[12] = {110000, -10000, -80000, 70000, 0, -100000, -100000, 10000, -80000, -130000, -50000, -150000};
            this->playerRoute = new Route(coords, 12);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(8, *this->enemies);
            (*this->enemies)[0] = this->createShip(1, 0, 9, nullptr, true, false);
            (*this->enemies)[0]->setPosition(playerPos.x + (float) (Globals::rnd->nextInt(1400) - 700),
                                             playerPos.y + (float) (Globals::rnd->nextInt(1400) - 700),
                                             playerPos.z + 1000.0f);
            (*this->enemies)[0]->setSpeed(3.0f);
            ((PlayerFighter *) (*this->enemies)[0])->setRotate(3);
            (*this->enemies)[0]->player->setHitpoints(9999999);
            (*this->enemies)[0]->setRoute(this->playerRoute->clone());
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1604);
            (*this->enemies)[0]->cargo = nullptr;
            for (unsigned i = 1; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(
                        8, 0, Globals::globals->getRandomEnemyFighter(8),
                        this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
                (*this->enemies)[i]->setToSleep();
            }
            this->objectivesA = new Objective(20, 1, 8, this);
            this->objectivesB = new Objective(21, 1, 8, this);
            break;
        }
        case 38: {
            int coords[3] = {90000, 10000, 80000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(7, *this->enemies);
            for (int i = 0; i < 2; i++) {
                (*this->enemies)[i] = this->createShip(2, 1, 15, this->enemyRoute->getWaypoint(), true, false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                (*this->enemies)[i]->setPosition(
                        (float) (this->enemyRoute->getWaypoint()->x + Globals::rnd->nextInt(20000) - 10000),
                        (float) (this->enemyRoute->getWaypoint()->y + Globals::rnd->nextInt(20000) - 10000),
                        (float) (this->enemyRoute->getWaypoint()->z + Globals::rnd->nextInt(20000) - 10000));
            }
            for (unsigned i = 2; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(3, 0, Globals::globals->getRandomEnemyFighter(3),
                                                       this->enemyRoute->getWaypoint(), true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            this->objectivesA = new Objective(18, 2, 7, this);
            this->objectivesB = new Objective(7, 2, this);
            break;
        }
        case 40: {
            int coords[3] = {-20000, -3000, 200000};
            int friendCoords[6] = {-20000, -3000, 35000, -20000, -3000, 200000};
            this->enemyRoute = new Route(coords, 3);
            this->friendRoute = new Route(friendCoords, 6);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(13, *this->enemies);
            (*this->enemies)[0] = this->createShip(0, 1, 13, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1604);
            (*this->enemies)[0]->player->setMaxHitpoints(Globals::status->getLevel() * 5 + 1800);
            (*this->enemies)[0]->setPosition(-9999999.0f, -9999999.0f, -9999999.0f);
            (*this->enemies)[0]->setToSleep();
            (*this->enemies)[0]->setVisible(false);
            for (int i = 1; i < 5; i++) {
                (*this->enemies)[i] = this->createShip(0, 0, Globals::globals->getRandomEnemyFighter(0), nullptr, true, false);
                (*this->enemies)[i]->setRoute(this->friendRoute->clone());
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                if (i == 2)
                    (*this->enemies)[i]->name = *Globals::gameText->getText(1605);
            }
            for (int i = 5; i < 9; i++) {
                (*this->enemies)[i] = this->createShip(9, 0, 8, this->enemyRoute->getWaypoint(), true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            for (int i = 9; i < 13; i++) {
                (*this->enemies)[i] = this->createShip(9, 0, 8, nullptr, true, false);
                (*this->enemies)[i]->setPosition(Vector{-500000.0f, -500000.0f, -500000.0f});
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                (*this->enemies)[i]->setToSleep();
            }
            (*this->landmarks)[3]->setPosition(-20000.0f, -3000.0f, 200000.0f);
            if (Level::initStreamOutPosition) {
                this->player->setPosition(-105000.0f, 0.0f, 80000.0f);
                this->player->geometry->setRotation(0.0f, 1.5707964f, 0.0f);
            }
            this->objectivesB = new Objective(7, 1, this);
            break;
        }
        case 41:
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(8, *this->enemies);
            (*this->enemies)[0] = this->createShip(1, 1, 13, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1604);
            if (Level::lastMissionFreighterHitpoints <= 0)
                Level::lastMissionFreighterHitpoints = (int) ((float) (Globals::status->getLevel() * 5 + 1800) * 0.7f);
            (*this->enemies)[0]->player->setHitpoints(Level::lastMissionFreighterHitpoints);
            ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(true);
            (*this->enemies)[0]->setPosition(0.0f, 0.0f, -300000.0f);
            for (int i = 1; i < 8; i++) {
                (*this->enemies)[i] = this->createShip(9, 0, 8, nullptr, true, false);
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
            break;
        case 48:
        case 49:
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(5, *this->enemies);
            for (unsigned i = 0; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(1, 0, 9, nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
            }
            break;
        case 50:
            this->spawnPosition =
                    (*this->landmarks)[1] != nullptr ? (*this->landmarks)[1]->getPosition() : Vector{0.0f, 0.0f, 40000.0f};
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(4, *this->enemies);
            for (unsigned i = 0; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(1, 0, 9, nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
            }
            break;
        case 51: {
            this->spawnPosition =
                    (*this->landmarks)[1] != nullptr ? (*this->landmarks)[1]->getPosition() : Vector{0.0f, 0.0f, 40000.0f};
            int coords[3] = {(int) this->spawnPosition.x, (int) this->spawnPosition.y, (int) this->spawnPosition.z};
            this->enemyRoute = new Route(coords, 3);
            this->enemyRoute->setLoop(true);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(6, *this->enemies);
            (*this->enemies)[0] = this->createShip(1, 1, 13, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysEnemy(true);
            this->spawnPosition = this->spawnPosition + (this->player->getPosition() - this->spawnPosition) / 3.0f;
            (*this->enemies)[0]->setPosition(this->spawnPosition.x, this->spawnPosition.y, this->spawnPosition.z);
            for (unsigned i = 1; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(1, 0, 9, this->enemyRoute->getWaypoint(), true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
            }
            break;
        }
        case 52: {
            this->spawnPosition =
                    (*this->landmarks)[1] != nullptr ? (*this->landmarks)[1]->getPosition() : Vector{0.0f, 0.0f, 40000.0f};
            int coords[3] = {(int) this->spawnPosition.x, (int) this->spawnPosition.y, (int) this->spawnPosition.z};
            this->enemyRoute = new Route(coords, 3);
            this->enemyRoute->setLoop(true);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(8, *this->enemies);
            (*this->enemies)[0] = this->createShip(1, 1, 13, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysEnemy(true);
            this->spawnPosition = this->spawnPosition + (this->player->getPosition() - this->spawnPosition) / 2.0f;
            (*this->enemies)[0]->setPosition(this->spawnPosition.x, this->spawnPosition.y, this->spawnPosition.z);
            (*this->enemies)[1] = this->createShip(1, 1, 13, nullptr, true, false);
            (*this->enemies)[1]->player->setAlwaysEnemy(true);
            this->spawnPosition.x += 16000.0f;
            this->spawnPosition.y += 13000.0f;
            this->spawnPosition.z += 16000.0f;
            (*this->enemies)[1]->setPosition(this->spawnPosition.x, this->spawnPosition.y, this->spawnPosition.z);
            for (unsigned i = 2; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(1, 0, 9, this->enemyRoute->getWaypoint(), true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
            }
            break;
        }
        case 56: {
            int coords[9] = {0, 0, -60000, -12000, -7000, -110000, 15000, 3000, -160000};
            this->playerRoute = new Route(coords, 9);
            this->friendRoute = this->playerRoute->clone();
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(9, *this->enemies);
            for (int i = 0; i < 3; i++) {
                (*this->enemies)[i] = this->createShip(8, 0, 24, nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                (*this->enemies)[i]->player->setAlwaysEnemy(false);
                (*this->enemies)[i]->setToSleep();
                this->spawnPosition = this->player->getPosition() * 0.8f;
                (*this->enemies)[i]->setPosition(this->spawnPosition.x - 3500.0f + (float) Globals::rnd->nextInt(7000),
                                                 this->spawnPosition.y - 3500.0f + (float) Globals::rnd->nextInt(7000),
                                                 this->spawnPosition.z - 3500.0f + (float) Globals::rnd->nextInt(7000));
                (*this->enemies)[i]->setRoute(this->friendRoute->clone());
                (*this->enemies)[i]->player->setHitpoints(9999999);
            }
            for (unsigned i = 3; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                       this->playerRoute->getWaypoint(i < 5 ? 0 : (i < 10 ? 1 : 2)),
                                                       true, false);
                (*this->enemies)[i]->setToSleep();
            }
            this->objectivesA = new Objective(18, 3, 9, this);
            break;
        }
        case 63: {
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(7, *this->enemies);
            int coords[3] = {0, 0, 0};
            Route *route = new Route(coords, 3);
            (*this->enemies)[0] = this->createStaticObject(route->getWaypoint(), 14243, true);
            Vector center = route->getWaypoint()->getPosition();
            (*this->enemies)[0]->setToSleep();
            (*this->enemies)[0]->name = *Globals::gameText->getText(441);
            for (unsigned i = 1; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8), nullptr, true, false);
                int x = Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000;
                int y = Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000;
                int z = Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000;
                (*this->enemies)[i]->setPosition((int) (center.x + (float) x), (int) (center.y + (float) y),
                                                 (int) (center.z + (float) z));
                (*this->enemies)[i]->setToSleep();
            }
            this->objectivesA = new Objective(27, 4, this);
            break;
        }
        case 64: {
            this->player->setPosition(this->player->getPosition() - this->player->geometry->getDirection() * 40000.0f);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(9, *this->enemies);
            int coords[3] = {100000, 0, 0};
            this->enemyRoute = new Route(coords, 3);
            (*this->enemies)[0] = this->createShip(0, 0, 38, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1617);
            (*this->enemies)[0]->setPosition(0.0f, 0.0f, 0.0f);
            (*this->enemies)[0]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
            (*this->enemies)[0]->setRoute(this->enemyRoute->clone());
            (*this->enemies)[0]->cargo = nullptr;
            for (unsigned i = 1; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8), nullptr, true, false);
                KIPlayer *fighter = (*this->enemies)[i];
                int x = (Globals::rnd->nextInt(1500) + 1000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
                int y = (Globals::rnd->nextInt(1500) + 1000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
                int z = (Globals::rnd->nextInt(1500) + 1000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1);
                fighter->setPosition((float) x, (float) y, (float) z);
                (*this->enemies)[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
                (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
            }
            this->objectivesA = new Objective(22, 0, this);
            this->objectivesB = new Objective(1, 0, this);
            break;
        }
        case 65: {
            int coords[3] = {0, 0, 0};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            (*this->enemies)[0] = this->createShip(0, 0, 38, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1617);
            (*this->enemies)[0]->setRoute(this->enemyRoute->clone());
            (*this->enemies)[0]->player->setHitpoints(9999999);
            break;
        }
        case 67: {
            int coords[3] = {220000, -20000, -10000};
            this->enemyRoute = new Route(coords, 3);
            this->playerRoute = this->enemyRoute->clone();
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(11, *this->enemies);
            (*this->enemies)[0] = this->createStaticObject(nullptr, 14243, true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(441);
            (*this->enemies)[0]->setPosition((float) coords[0], (float) coords[1], (float) coords[2]);
            (*this->enemies)[0]->player->setAlwaysEnemy(false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->setToSleep();
            for (unsigned i = 1; i < this->enemies->size() - 1; i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                       this->enemyRoute->getWaypoint(), true, false);
                (*this->enemies)[i]->setToSleep();
                ((PlayerFighter *) (*this->enemies)[i])->field_0x128 = 150000;
                if (i >= 5)
                    (*this->enemies)[i]->setPosition(800000.0f, 800000.0f, 800000.0f);
            }
            (*this->enemies)[1]->setPosition((float) (coords[0] + 30000), (float) coords[1], (float) coords[2]);
            (*this->enemies)[this->enemies->size() - 1] = this->createShip(0, 0, 27, nullptr, true, false);
            Vector pos = this->player->getPosition();
            (*this->enemies)[this->enemies->size() - 1]->setPosition(pos + Vector{2000.0f, 500.0f, -7000.0f});
            (*this->enemies)[this->enemies->size() - 1]->player->setAlwaysFriend(true);
            (*this->enemies)[this->enemies->size() - 1]->name = *Globals::gameText->getText(1633);
            (*this->enemies)[this->enemies->size() - 1]->player->setMaxHitpoints(9999999);
            (*this->enemies)[this->enemies->size() - 1]->setRoute(this->playerRoute->clone());
            this->objectivesA = new Objective(22, 0, this);
            this->objectivesB = new Objective(1, 0, this);
            break;
        }
        case 69: {
            Vector pos = (*(Array<AEGeometry *> *) this->starSystem->getPlanets())[1]->getPosition();
            pos *= 4.0f;
            int coords[6] = {(int) pos.x, (int) pos.y, (int) pos.z,
                             (int) (pos.x * 10.0f), (int) (pos.y * 10.0f), (int) (pos.z * 10.0f)};
            this->enemyRoute = new Route(coords, 6);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(5, *this->enemies);
            (*this->enemies)[0] = this->createShip(0, 0, 12, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1631);
            (*this->enemies)[0]->setRoute(this->enemyRoute->clone());
            (*this->enemies)[0]->setPosition(pos);
            (*this->enemies)[0]->setSpeed(5.5f);
            pos = VectorNormalize(pos);
            (*this->enemies)[0]->geometry->setDirection(pos, Vector{0.0f, 1.0f, 0.0f});
            int friendCoords[3] = {0, 0, 20000};
            this->friendRoute = new Route(friendCoords, 3);
            for (unsigned i = 1; i < this->enemies->size(); i++)
                (*this->enemies)[i] = this->createShip(0, 0, Globals::globals->getRandomEnemyFighter(0),
                                                       this->friendRoute->getWaypoint(), true, false);
            this->objectivesA = new Objective(22, 0, this);
            break;
        }
        case 70: {
            this->player->setPosition(this->player->getPosition() - this->player->geometry->getDirection() * 120000.0f);
            Vector target = (*this->landmarks)[1]->getPosition();
            Vector start = this->player->getPosition();
            int coords[3] = {(int) target.x, (int) target.y, (int) target.z};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            (*this->enemies)[0] = this->createShip(0, 0, 12, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1631);
            (*this->enemies)[0]->setRoute(this->enemyRoute->clone());
            this->spawnPosition = start + (target - start) / 4.0f;
            (*this->enemies)[0]->setPosition(this->spawnPosition);
            (*this->enemies)[0]->geometry->setDirection(VectorNormalize(target - start), Vector{0.0f, 1.0f, 0.0f});
            Player *p = (*this->enemies)[0]->player;
            p->setHitpoints((int) ((float) p->getMaxHitpoints() * 2.5f));
            this->objectivesA = new Objective(1, 0, this);
            break;
        }
        case 73: {
            int coords[6] = {80000, 0, 60000, 150000, 0, -50000};
            this->enemyRoute = new Route(coords, 6);
            this->player->setPosition(-30000.0f, 0.0f, 150000.0f);
            Vector toTarget = Vector{(float) (coords[3] + 20000), (float) coords[4], (float) coords[5]} -
                              this->player->getPosition();
            this->player->geometry->setDirection(VectorNormalize(toTarget), Vector{0.0f, 1.0f, 0.0f});
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(12, *this->enemies);
            for (unsigned i = 0; i < 8; i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                       this->enemyRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                (*this->enemies)[i]->setToSleep();
                if (i >= 4)
                    (*this->enemies)[i]->setPosition(Vector{-800000.0f, -800000.0f, -800000.0f});
            }
            for (unsigned i = 8; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(0, 1, 15, nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                (*this->enemies)[i]->cargo = nullptr;
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(true);
                (*this->enemies)[i]->setPosition(
                        (float) (this->enemyRoute->getWaypoint(1)->x + Globals::rnd->nextInt(20000) - 10000),
                        (float) (this->enemyRoute->getWaypoint(1)->y + Globals::rnd->nextInt(20000) - 10000),
                        (float) (this->enemyRoute->getWaypoint(1)->z + Globals::rnd->nextInt(20000) - 10000));
            }
            this->objectivesA = new Objective(18, 0, 8, this);
            this->objectivesB = new Objective(18, 8, 12, this);
            break;
        }
        case 78: {
            Matrix rotation;
            Vector v = {0.0f, 0.0f, 0.0f};
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(22, *this->enemies);
            for (int k = 0; k < 2; k++) {
                (*this->enemies)[k] = this->createStaticObject(nullptr, 14363, true);
                v = MatrixRotateVector(rotation, Vector{kCarrierTurrets[k][4], kCarrierTurrets[k][5], kCarrierTurrets[k][6]});
                (*this->enemies)[k]->geometry->setRotation(v);
                v = MatrixRotateVector(rotation, Vector{kCarrierTurrets[k][1], kCarrierTurrets[k][2], kCarrierTurrets[k][3]});
                (*this->enemies)[k]->setPosition(Vector{(k == 0 ? 1200.0f : -1200.0f) + v.x, v.y, v.z});
            }
            for (unsigned i = 2; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8), nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                KIPlayer *fighter = (*this->enemies)[i];
                fighter->setPosition(Vector{(float) (Globals::rnd->nextInt(2000) + (int) i * 2000 - 21000),
                                            (float) (Globals::rnd->nextInt(10000) - 5000),
                                            (float) (Globals::rnd->nextInt(10000) + 155000)});
                (*this->enemies)[i]->geometry->setDirection(Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
                (*this->enemies)[i]->setToSleep();
            }
            break;
        }
        case 79:
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(7, *this->enemies);
            for (unsigned i = 0; i < this->enemies->size(); i++) {
                (*this->enemies)[i] = this->createShip(9, 0, Globals::globals->getRandomEnemyFighter(9), nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            break;
        case 80: {
            this->enemies = new Array<KIPlayer *>();
            Vector carrierPos = {0.0f, 0.0f, 160000.0f};
            Station *station = (Station *) (intptr_t) Globals::galaxy->getStation(101);
            ArrayAdd((KIPlayer *) new PlayerStation(station), *this->enemies);
            (*this->enemies)[0]->name = station->getName();
            (*this->enemies)[0]->shipGroup = 8;
            (*this->enemies)[0]->setPosition(carrierPos);
            ((PlayerStation *) (*this->enemies)[0])->rootGeometry->setRotation(Vector{0.0f, 0.0f, 0.0f});
            delete station;
            Matrix rotation;
            MatrixSetRotation(rotation, 0.0f, 3.1415927f, 0.0f);
            Vector v = {0.0f, 0.0f, 0.0f};
            int turretHitpoints = Globals::status->getLevel() > 20 ? 520 : Globals::status->getLevel() * 15 + 220;
            for (int k = 0; k < 12; k++) {
                ArrayAdd(this->createStaticObject(nullptr, kCarrierTurrets[k][0] == 1.0f ? 14365 : 14363, true),
                         *this->enemies);
                KIPlayer *turret = (*this->enemies)[k + 1];
                turret->player->setMaxHitpoints(turretHitpoints);
                v = MatrixRotateVector(rotation, Vector{kCarrierTurrets[k][4], kCarrierTurrets[k][5], kCarrierTurrets[k][6]});
                turret->geometry->setRotation(v);
                v = MatrixRotateVector(rotation, Vector{kCarrierTurrets[k][1], kCarrierTurrets[k][2], kCarrierTurrets[k][3]});
                turret->setPosition(Vector{carrierPos.x + v.x, carrierPos.y + v.y, carrierPos.z + v.z});
                turret->shipGroup = 8;
                turret->player->setAlwaysEnemy(true);
            }
            int coords[3] = {0, 0, (int) (carrierPos.z * 0.5f)};
            this->enemyRoute = new Route(coords, 3);
            for (int i = 0; i < 6; i++) {
                KIPlayer *fighter = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                     this->enemyRoute->getWaypoint(), true, false);
                fighter->player->setAlwaysEnemy(true);
                fighter->player->setAlwaysFriend(false);
                ArrayAdd(fighter, *this->enemies);
            }
            for (int i = 0; i < 3; i++) {
                KIPlayer *wingman = this->createShip(0, 0, 17, this->enemyRoute->getWaypoint(), true, false);
                wingman->player->setAlwaysFriend(true);
                wingman->player->setAlwaysEnemy(false);
                ArrayAdd(wingman, *this->enemies);
            }
            this->player->setPosition(-70000.0f, 0.0f, -30000.0f);
            this->player->geometry->setDirection(Vector{1.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
            this->objectivesA = new Objective(22, 0, this);
            break;
        }
        case 81: {
            this->enemies = new Array<KIPlayer *>();
            Station *station = (Station *) (intptr_t) Globals::galaxy->getStation(101);
            ArrayAdd((KIPlayer *) new PlayerStation(station), *this->enemies);
            (*this->enemies)[0]->name = station->getName();
            (*this->enemies)[0]->shipGroup = 8;
            delete station;
            for (int i = 0; i < 8; i++)
                ArrayAdd((KIPlayer *) this->createShip(9, 0, 8, nullptr, true, false), *this->enemies);
            break;
        }
        case 87:
            this->objectivesA = new Objective(22, 0, this);
            break;
        case 89: {
            int friendCoords[6] = {-63000, 0, 75000, -60000, 0, 110000};
            this->friendRoute = new Route(friendCoords, 6);
            this->friendRoute->setLoop(true);
            int enemyCoords[6] = {-63000, -5000, 75000, -60000, -5000, 110000};
            this->enemyRoute = new Route(enemyCoords, 6);
            this->enemyRoute->setLoop(true);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(12, *this->enemies);
            (*this->enemies)[0] = this->createStaticObject(nullptr, 16992, false);
            ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(false);

            (*this->enemies)[1] = new PlayerFixedObject(18781, 0, new Player(7500, 9999999, 0, 0, 0), nullptr, 0.0f, 0.0f, 0.0f);
            AEGeometry *gate = new AEGeometry(21076, Globals::Canvas, false);
            AEGeometry *part0 = new AEGeometry(21876, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry(22076, Globals::Canvas, false);
            gate->addChild(part0->transform);
            gate->addChild(part1->transform);
            delete part0;
            delete part1;
            (*this->enemies)[1]->setShipGroup(gate, -1, false);
            (*this->enemies)[1]->setPosition(Vector{-77000.0f, -3000.0f, 90000.0f});
            (*this->enemies)[1]->geometry->setRotation(0.0f, 1.5707964f, 0.0f);

            (*this->enemies)[2] = new PlayerFixedObject(18781, 0, new Player(7500, 9999999, 0, 0, 0), nullptr, 0.0f, 0.0f, 0.0f);
            AEGeometry *brokenGate = new AEGeometry(21076, Globals::Canvas, false);
            AEGeometry *part2 = new AEGeometry(21876, Globals::Canvas, false);
            AEGeometry *part3 = new AEGeometry(18832, Globals::Canvas, false);
            AEGeometry *part4 = new AEGeometry(18833, Globals::Canvas, false);
            brokenGate->addChild(part2->transform);
            brokenGate->addChild(part3->transform);
            brokenGate->addChild(part4->transform);
            delete part2;
            delete part3;
            delete part4;
            (*this->enemies)[2]->setShipGroup(brokenGate, -1, false);
            (*this->enemies)[2]->setVisible(false);
            (*this->enemies)[2]->setPosition(Vector{-77000.0f, -3000.0f, 90000.0f});
            (*this->enemies)[2]->geometry->setRotation(0.0f, 1.5707964f, 0.0f);

            for (int i = 0; i < 8; i++) {
                Waypoint *wp = (i < 4 ? this->friendRoute : this->enemyRoute)
                                       ->getWaypoint(i < 4 ? (i > 1 ? 1 : 0) : (i < 7 ? 1 : 0));
                KIPlayer *ship = this->createShip(3, 0, Globals::globals->getRandomEnemyFighter(3), wp, true, false);
                ship->setRoute(this->friendRoute->clone());
                this->spawnPosition = ship->getPosition();
                if (this->spawnPosition.y > -4000.0f && this->spawnPosition.y < 2000.0f)
                    ship->setPosition(this->spawnPosition.x, this->spawnPosition.y + this->spawnPosition.y,
                                      this->spawnPosition.z);
                ((PlayerFighter *) ship)->field_0x13e = 0;
                (*this->enemies)[i + 3] = ship;
            }
            (*this->enemies)[this->enemies->size() - 1] = this->createShip(3, 1, 15, nullptr, true, false);
            (*this->enemies)[this->enemies->size() - 1]->setPosition(-63000.0f, -3000.0f, 65000.0f);
            break;
        }
        case 91: {
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            Waypoint *wp = new Waypoint(-20000, 0, 60000, nullptr);
            (*this->enemies)[0] = this->createStaticObject(wp, 18766, false);
            ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(false);
            ((PlayerFixedObject *) (*this->enemies)[0])->setDockingType(0);
            (*this->enemies)[0]->setPosition(wp->getPosition());
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            Player *p = (*this->enemies)[0]->player;
            p->setHitpoints(p->getHitpoints() / 20);
            (*this->enemies)[0]->geometry->setRotation(Vector{0.0f, 5.4977875f, 0.0f});
            (*this->enemies)[0]->setActive(false);
            delete (*this->enemies)[0]->cargo;
            (*this->enemies)[0]->cargo = nullptr;
            this->objectivesB = new Objective(1, 0, this);
            break;
        }
        case 92: {
            int coords[9] = {Globals::rnd->nextInt(140000) - 70000, 0, -70000,
                             Globals::rnd->nextInt(140000) - 70000, 0, -100000,
                             Globals::rnd->nextInt(140000) - 70000, 0, -140000};
            this->enemyRoute = new Route(coords, 9);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(4, *this->enemies);
            for (int i = 0; i < 3; i++) {
                (*this->enemies)[i] = this->createShip(
                        10, 0, 44, this->enemyRoute->getWaypoint(Globals::rnd->nextInt(this->enemyRoute->length())), true,
                        false);
                (*this->enemies)[i]->setToSleep();
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                ((PlayerFighter *) (*this->enemies)[i])->setAIDisabled(true);
                ((PlayerFighter *) (*this->enemies)[i])->setCloakingPossible(false);
            }
            Waypoint *wp = new Waypoint(80000, 0, 110000, nullptr);
            (*this->enemies)[3] = this->createStaticObject(wp, 17049, false);
            ((PlayerFixedObject *) (*this->enemies)[3])->setMoving(false);
            (*this->enemies)[3]->player->setAlwaysFriend(true);
            ((PlayerFixedObject *) (*this->enemies)[3])->setDockingType(1);
            ((PlayerFixedObject *) (*this->enemies)[3])->setName(*Globals::gameText->getText(3209));
            delete (*this->enemies)[3]->cargo;
            (*this->enemies)[3]->cargo = nullptr;
            this->objectivesA = new Objective(22, 0, this);
            this->objectivesB = new Objective(1, 3, this);
            break;
        }
        case 94: {
            int coords[3] = {-700000, 0, -700000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(10, *this->enemies);
            for (int i = 0; i < 6; i++) {
                (*this->enemies)[i] = this->createShip(
                        10, 0, 44, this->enemyRoute->getWaypoint(Globals::rnd->nextInt(this->enemyRoute->length())), true,
                        false);
                (*this->enemies)[i]->setToSleep();
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            (*this->enemies)[6] = this->createStaticObject(nullptr, 18781, false);
            ((PlayerFixedObject *) (*this->enemies)[6])->setMoving(false);
            ((PlayerFixedObject *) (*this->enemies)[6])->setDockingType(2);
            ((PlayerFixedObject *) (*this->enemies)[6])->setName(*Globals::gameText->getText(3208));
            (*this->enemies)[6]->player->setAlwaysFriend(true);
            (*this->enemies)[6]->geometry->rotate(Vector{0.0f, -2.3561945f, 0.0f});
            delete (*this->enemies)[6]->cargo;
            (*this->enemies)[6]->cargo = nullptr;
            Waypoint *wp = new Waypoint(30000, -5000, 40000, nullptr);
            (*this->enemies)[7] = this->createStaticObject(wp, 17049, false);
            ((PlayerFixedObject *) (*this->enemies)[7])->setMoving(false);
            ((PlayerFixedObject *) (*this->enemies)[7])->setDockingType(1);
            ((PlayerFixedObject *) (*this->enemies)[7])->setName(*Globals::gameText->getText(3209));
            (*this->enemies)[7]->player->setAlwaysFriend(true);
            delete (*this->enemies)[7]->cargo;
            (*this->enemies)[7]->cargo = nullptr;
            int convoyCoords[9] = {-1, -1, -1, 20000, -3000, 30000, -1, -1, -1};
            int waitTimes[3] = {12000, 0, 12000};
            Array<KIPlayer *> *stops = new Array<KIPlayer *>();
            ArraySetLength(3, *stops);
            (*stops)[0] = (*this->enemies)[6];
            (*stops)[2] = (*this->enemies)[7];
            Route *convoyRoute = new Route(convoyCoords, stops, waitTimes, 9);
            convoyRoute->setLoop(true);
            Route *convoyRoute2 = convoyRoute->clone();
            for (int i = 8; i < 10; i++) {
                int type = Globals::globals->getRandomEnemyFighter(3);
                if (i == 8) {
                    (*this->enemies)[i] = this->createShip(3, 0, type, convoyRoute->getWaypoint(0), true, false);
                    (*this->enemies)[i]->setRoute(convoyRoute);
                } else {
                    (*this->enemies)[i] = this->createShip(3, 0, type, convoyRoute2->getWaypoint(0), true, false);
                    (*this->enemies)[i]->setRoute(convoyRoute2);
                }
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                (*this->enemies)[i]->player->setNeverAttack(true);
            }
            this->objectivesB = new Objective(1, 7, this);
            break;
        }
        case 97: {
            int coords[3] = {0, 0, 50000};
            this->enemyRoute = new Route(coords, 3);
            unsigned char weakDevice = Globals::isRunningHDonWeakDevice;
            unsigned enemyCount = weakDevice ? 8 : 12;
            this->enemies = new Array<KIPlayer *>();
            unsigned total = weakDevice ? 11 : 19;
            ArraySetLength(total, *this->enemies);
            for (unsigned i = 0; i < enemyCount; i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                       this->enemyRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            unsigned friendEnd = weakDevice ? 11 : 16;
            for (unsigned i = enemyCount; i < friendEnd; i++) {
                (*this->enemies)[i] = this->createShip(2, 0, Globals::globals->getRandomEnemyFighter(2),
                                                       this->enemyRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
            }
            for (unsigned i = friendEnd; i < total; i++) {
                (*this->enemies)[i] = this->createShip(2, 1, 15, this->enemyRoute->getWaypoint(0), true, false);
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
            }
            this->objectivesA = new Objective(18, 0, enemyCount, this);
            break;
        }
        case 100: {
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(2, *this->enemies);
            this->spawnPosition = this->player->getPosition();
            this->spawnPosition.x += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
            this->spawnPosition.y += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000);
            this->spawnPosition.z += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
            int coords[3] = {(int) this->spawnPosition.x, (int) this->spawnPosition.y, (int) this->spawnPosition.z};
            Route *route = new Route(coords, 3);
            for (int i = 0; i < 2; i++) {
                (*this->enemies)[i] = this->createShip(10, 0, 44, route->getWaypoint(), true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            this->objectivesA = new Objective(4, 1, this);
            break;
        }
        case 102: {
            int coords[9] = {Globals::rnd->nextInt(140000) - 70000, 0, -70000,
                             Globals::rnd->nextInt(140000) - 70000, 0, -100000,
                             Globals::rnd->nextInt(140000) - 70000, 0, -140000};
            this->enemyRoute = new Route(coords, 9);
            int friendCoords[9] = {Globals::rnd->nextInt(140000) - 70000, 0, -20000,
                                   Globals::rnd->nextInt(140000) - 70000, 0, -500000,
                                   Globals::rnd->nextInt(140000) - 70000, 0, -70000};
            this->friendRoute = new Route(friendCoords, 9);
            Waypoint *dockWp = new Waypoint(-50000, 1000, 70000, nullptr);
            Waypoint *escortWp = new Waypoint(20000, 10000, 10000, nullptr);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(10, *this->enemies);
            (*this->enemies)[0] = this->createStaticObject(dockWp, 18804, true);
            (*this->enemies)[0]->setPosition(dockWp->getPosition());
            ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(false);
            (*this->enemies)[0]->field_0x70 = 0;
            ((PlayerFixedObject *) (*this->enemies)[0])->setDockingType(1);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->player->setHitpoints(9999999);
            (*this->enemies)[0]->field_0x74 = 1;
            delete (*this->enemies)[0]->cargo;
            (*this->enemies)[0]->cargo = nullptr;
            (*this->enemies)[1] = this->createStaticObject(dockWp, 21113, true);
            (*this->enemies)[1]->setPosition(0.0f, 0.0f, 0.0f);
            ((PlayerFixedObject *) (*this->enemies)[1])->setMoving(false);
            ((PlayerFixedObject *) (*this->enemies)[1])->setDockingType(0);
            (*this->enemies)[1]->player->setAlwaysFriend(true);
            (*this->enemies)[1]->player->setHitpoints(9999999);
            (*this->enemies)[1]->geometry->rotate(Vector{0.0f, 3.1415927f, 0.0f});
            (*this->enemies)[1]->setActive(false);
            delete (*this->enemies)[1]->cargo;
            (*this->enemies)[1]->cargo = nullptr;
            for (int i = 2; i < 6; i++) {
                (*this->enemies)[i] = this->createShip(0, 0, 51, escortWp, true, false);
                KIPlayer *escort = (*this->enemies)[i];
                if (i == 2)
                    escort->setPosition(escortWp->getPosition() + Vector{10000.0f, 6000.0f, -20000.0f});
                else
                    escort->setPosition(escortWp->getPosition() +
                                        Vector{(float) (15000 + i * 5000), 8000.0f, (float) (-20000 - i * 5000)});
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                ((PlayerFighter *) (*this->enemies)[i])->field_0x13d = 0;
                ((PlayerFighter *) (*this->enemies)[i])->field_0x13e = 0;
            }
            int convoyCoords[9] = {-1, -1, -1, -30000, 1000, 40000, -1, -1, -1};
            int waitTimes[3] = {20000, 0, 20000};
            Array<KIPlayer *> *stops = new Array<KIPlayer *>();
            ArraySetLength(3, *stops);
            (*stops)[0] = (*this->enemies)[0];
            (*stops)[2] = (*this->enemies)[1];
            Route *convoyRoute = new Route(convoyCoords, stops, waitTimes, 9);
            convoyRoute->setLoop(true);
            for (int i = 2; i < 6; i++)
                (*this->enemies)[i]->setRoute(i == 2 ? convoyRoute : convoyRoute->clone());
            for (int i = 6; i < 10; i++) {
                (*this->enemies)[i] = this->createShip(10, 0, 44, escortWp, true, false);
                (*this->enemies)[i]->setPosition(Vector{1000000.0f, 1000000.0f, 1000000.0f});
                (*this->enemies)[i]->setToSleep();
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            this->objectivesA = new Objective(4, 8, this);
            this->objectivesB = new Objective(18, 2, 6, this);
            break;
        }
        case 105: {
            int coords[6] = {7000000, 7000000, 7000000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(5, *this->enemies);
            for (int i = 0; i < 2; i++) {
                (*this->enemies)[i] = this->createShip(0, 0, 37, this->enemyRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
            }
            for (int i = 2; i < 5; i++) {
                (*this->enemies)[i] = this->createShip(10, 0, 44, this->enemyRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->setToSleep();
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            Vector lightDir = VectorNormalize(this->starSystem->getLightDirection());
            int routeCoords[3] = {(int) (lightDir.x * 650000.0f), 0, (int) (lightDir.z * 650000.0f)};
            Route *route = new Route(routeCoords, 3);
            this->player->geometry->setDirection(lightDir, Vector{0.0f, 1.0f, 0.0f});
            this->player->setRoute(route);
            this->setPlayerRoute(route);
            for (int i = 0; i < 2; i++) {
                (*this->enemies)[i]->setPosition(this->player->getPosition() +
                                                 this->player->geometry->getRightVector() * (i == 0 ? 2100.0f : -2100.0f) +
                                                 this->player->geometry->getDirection() * 2000.0f);
                (*this->enemies)[i]->geometry->setDirection(lightDir, Vector{0.0f, 1.0f, 0.0f});
                (*this->enemies)[i]->setRoute(route->clone());
                (*this->enemies)[i]->setEnemies(nullptr);
                ((PlayerFighter *) (*this->enemies)[i])->field_0x13d = 0;
                ((PlayerFighter *) (*this->enemies)[i])->setAIDisabled(true);
            }
            break;
        }
        case 106: {
            int coords[6] = {-500000, 0, -1700000, -500000, 0, -3700000};
            this->enemyRoute = new Route(coords, 6);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            (*this->enemies)[0] = this->createShip(10, 0, 44, this->enemyRoute->getWaypoint(0), true, false);
            (*this->enemies)[0]->setPosition(this->enemyRoute->getWaypoint(0)->getPosition());
            (*this->enemies)[0]->geometry->setDirection(Vector{-1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
            (*this->enemies)[0]->setRoute(this->enemyRoute->clone());
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->player->setAlwaysEnemy(false);
            (*this->enemies)[0]->player->setHitpoints(1);
            ((PlayerFighter *) (*this->enemies)[0])->setCloakingPossible(false);
            break;
        }
        case 131:
            this->objectivesA = new Objective(4, 0, this);
            break;
        case 135: {
            int coords[3] = {7000000, 7000000, 7000000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(3, *this->enemies);
            (*this->enemies)[0] = this->createStaticObject(nullptr, 19080, true);
            (*this->enemies)[0]->setPosition(0.0f, 0.0f, 0.0f);
            ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(false);
            ((PlayerFixedObject *) (*this->enemies)[0])->setName(*Globals::gameText->getText(3210));
            ((PlayerFixedObject *) (*this->enemies)[0])->setDockingType(1);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            delete (*this->enemies)[0]->cargo;
            (*this->enemies)[0]->cargo = nullptr;
            for (int i = 1; i < 3; i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                       this->enemyRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->setToSleep();
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            for (unsigned i = 0; i < this->asteroids->size(); i++)
                ((PlayerAsteroid *) (*this->asteroids)[i])->setAsteroidIndex(155);
            break;
        }
        case 137:
            this->objectivesA = new Objective(4, 4, this);
            break;
        case 139: {
            Globals::status->field_178 = 0;
            int coords[6] = {-70000, 0, 80000, -30000, 0, 60000};
            this->enemyRoute = new Route(coords, 6);
            this->enemyRoute->setLoop(true);
            int friendCoords[6] = {-220000, 0, 40000, -180000, 0, 20000};
            this->friendRoute = new Route(friendCoords, 6);
            this->friendRoute->setLoop(true);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(22, *this->enemies);
            Waypoint *baseA = new Waypoint(-50000, -1500, 70000, nullptr);
            Waypoint *baseB = new Waypoint(-200000, -1500, 30000, nullptr);
            (*this->enemies)[0] = this->createStaticObject(baseA, 19051, true);
            (*this->enemies)[0]->setPosition(baseA->getPosition());
            ((PlayerFixedObject *) (*this->enemies)[0])->setMoving(false);
            ((PlayerFixedObject *) (*this->enemies)[0])->setDockingType(3);
            (*this->enemies)[0]->player->setHitpoints(9999999);
            ((PlayerFixedObject *) (*this->enemies)[0])->setName(*Globals::gameText->getText(1667) + String(" 1"));
            delete (*this->enemies)[0]->cargo;
            (*this->enemies)[0]->cargo = nullptr;
            (*this->enemies)[1] = this->createStaticObject(baseB, 19051, true);
            (*this->enemies)[1]->setPosition(baseB->getPosition());
            ((PlayerFixedObject *) (*this->enemies)[1])->setMoving(false);
            ((PlayerFixedObject *) (*this->enemies)[1])->setDockingType(3);
            (*this->enemies)[1]->player->setHitpoints(9999999);
            ((PlayerFixedObject *) (*this->enemies)[1])->setName(*Globals::gameText->getText(1667) + String(" 2"));
            delete (*this->enemies)[1]->cargo;
            (*this->enemies)[1]->cargo = nullptr;
            Matrix rotation;
            Vector v = {0.0f, 0.0f, 0.0f};
            int slot = 0;
            for (int i = 2; i < 12; i++) {
                (*this->enemies)[i] = this->createStaticObject(nullptr, 6774, true);
                KIPlayer *turret = (*this->enemies)[i];
                turret->player->setMaxHitpoints(1000);
                v = MatrixRotateVector(rotation, Vector{kBaseTurrets[slot][3], kBaseTurrets[slot][4], kBaseTurrets[slot][5]});
                turret->geometry->setRotation(v);
                v = MatrixRotateVector(rotation, Vector{kBaseTurrets[slot][0], kBaseTurrets[slot][1], kBaseTurrets[slot][2]});
                Waypoint *base = i < 7 ? baseA : baseB;
                turret->setPosition(Vector{base->getPosition().x + v.x, base->getPosition().y + v.y,
                                           base->getPosition().z + v.z});
                ((PlayerTurret *) turret)->setScaling(6.0f);
                turret->cargo = nullptr;
                turret->shipGroup = 1;
                if (++slot == 5)
                    slot -= 5;
            }
            for (int i = 12; i < 22; i++) {
                int type = Globals::globals->getRandomEnemyFighter(1);
                if (i <= 16) {
                    (*this->enemies)[i] = this->createShip(1, 0, type, baseA, true, false);
                    (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
                } else {
                    (*this->enemies)[i] = this->createShip(1, 0, type, baseB, true, false);
                    (*this->enemies)[i]->setRoute(this->friendRoute->clone());
                }
            }
            break;
        }
        case 142: {
            int coords[3] = {90000, 0, 42000};
            this->playerRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            (*this->enemies)[0] = this->createShip(3, 0, 30, nullptr, true, false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->setRoute(this->playerRoute->clone());
            (*this->enemies)[0]->player->setHitpoints(9999999);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1599);
            ((PlayerFighter *) (*this->enemies)[0])->setBoostProb(0);
            (*this->enemies)[0]->setPosition(this->player->getPosition() +
                                             this->player->geometry->getRightVector() * 2100.0f +
                                             this->player->geometry->getDirection() * 2000.0f);
            (*this->enemies)[0]->geometry->setDirection(this->player->GetDirVector(), Vector{0.0f, 1.0f, 0.0f});
            this->objectivesA = new Objective(4, 4, this);
            break;
        }
        case 144: {
            int coords[6] = {60000, 10000, 100000, -50000, 0, 50000};
            this->enemyRoute = new Route(coords, 6);
            this->enemyRoute->getWaypoint(0)->reached();
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(13, *this->enemies);
            Vector dir = this->enemyRoute->getWaypoint(1)->getPosition() - this->enemyRoute->getWaypoint(0)->getPosition();
            dir.y = 0.0f;
            dir = VectorNormalize(dir);
            Vector start = this->enemyRoute->getWaypoint(0)->getPosition();
            (*this->enemies)[0] = this->createShip(2, 0, 49, nullptr, true, false);
            (*this->enemies)[0]->setPosition(this->enemyRoute->getWaypoint(0)->getPosition());
            (*this->enemies)[0]->player->setAlwaysEnemy(true);
            (*this->enemies)[0]->setRoute(this->enemyRoute->getExactClone());
            (*this->enemies)[0]->name = *Globals::gameText->getText(1636);
            (*this->enemies)[0]->geometry->setDirection(dir, Vector{0.0f, 1.0f, 0.0f});
            ((PlayerFighter *) (*this->enemies)[0])->setAIDisabled(true);
            ((PlayerFighter *) (*this->enemies)[0])->setCloakingPossible(false);
            Matrix leaderMatrix = (*this->enemies)[0]->geometry->getMatrix();
            this->spawnPosition = start;
            for (int i = 1; i < 13; i++) {
                (*this->enemies)[i] = this->createShip(10, 0, 44, nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                (*this->enemies)[i]->geometry->setDirection(dir, Vector{0.0f, 1.0f, 0.0f});
                ((PlayerFighter *) (*this->enemies)[i])->setAIDisabled(true);
                ((PlayerFighter *) (*this->enemies)[i])->setCloakingPossible(false);
                (*this->enemies)[i]->setRoute(this->enemyRoute->getExactClone());
                if (i <= 2)
                    this->spawnPosition = Vector{(float) (-5000 + (i - 1) * 1500), 300.0f, 200.0f};
                else if (i <= 4)
                    this->spawnPosition = Vector{(float) (8000 - (i - 1) * 1500), 300.0f, 200.0f};
                else if (i <= 9)
                    this->spawnPosition = Vector{(float) (-19500 + (i - 1) * 3250), i == 7 ? -300.0f : 500.0f, -5000.0f};
                else
                    this->spawnPosition = Vector{(float) (-65000 + (i - 1) * 6500), -400.0f, -5000.0f};
                this->spawnPosition.x += (float) (Globals::rnd->nextInt(60) - 30);
                this->spawnPosition.y += (float) (Globals::rnd->nextInt(60) - 30);
                this->spawnPosition.z += (float) (Globals::rnd->nextInt(60) - 30);
                this->spawnPosition = MatrixTransformVector(leaderMatrix, this->spawnPosition);
                (*this->enemies)[i]->setPosition(this->spawnPosition);
            }
            break;
        }
        case 145: {
            int coords[3] = {-50000, 0, 50000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(14, *this->enemies);
            (*this->enemies)[0] = this->createShip(2, 0, 49, nullptr, true, false);
            this->spawnPosition = Vector{30000.0f, 0.0f, 80000.0f};
            (*this->enemies)[0]->setPosition(this->spawnPosition);
            (*this->enemies)[0]->player->setAlwaysEnemy(true);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1636);
            (*this->enemies)[0]->setRoute(this->enemyRoute->clone());
            ((PlayerFighter *) (*this->enemies)[0])->field_0x13d = 0;
            (*this->enemies)[0]->geometry->setDirection(
                    VectorNormalize(Vector{-50000.0f, 0.0f, 50000.0f} - (*this->enemies)[0]->getPosition()),
                    Vector{0.0f, 1.0f, 0.0f});
            ((PlayerFighter *) (*this->enemies)[0])->setAIDisabled(true);
            for (int i = 1; i < 13; i++) {
                (*this->enemies)[i] = this->createShip(10, 0, 44, this->enemyRoute->getWaypoint(0), true, false);
                KIPlayer *fighter = (*this->enemies)[i];
                int offset = Globals::rnd->nextInt(1000);
                int y = offset + (Globals::rnd->nextInt(1000) - 500) * i;
                int z = Globals::rnd->nextInt(2500) + 72000 + (i - 1) * 2000;
                fighter->setPosition((float) (23000 + (i - 1) * 3000), (float) y, (float) z);
                (*this->enemies)[i]->setRoute(this->enemyRoute->clone());
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                ((PlayerFighter *) (*this->enemies)[i])->setAIDisabled(true);
                ((PlayerFighter *) (*this->enemies)[i])->field_0x13d = 0;
                ((PlayerFighter *) (*this->enemies)[i])->setCloakingPossible(false);
                (*this->enemies)[i]->geometry->setDirection(
                        VectorNormalize(Vector{-50000.0f, 0.0f, 50000.0f} - (*this->enemies)[i]->getPosition()),
                        Vector{0.0f, 1.0f, 0.0f});
            }
            (*this->enemies)[this->enemies->size() - 1] = this->createStaticObject(nullptr, 19050, true);
            (*this->enemies)[this->enemies->size() - 1]->setPosition(-50000.0f, 0.0f, 50000.0f);
            (*this->enemies)[this->enemies->size() - 1]->setVisible(false);
            (*this->enemies)[this->enemies->size() - 1]->setActive(false);
            (*this->enemies)[this->enemies->size() - 1]->name = *Globals::gameText->getText(3207);
            (*this->enemies)[this->enemies->size() - 1]->shipGroup = 3;
            (*this->enemies)[this->enemies->size() - 1]->player->setHitpoints(9999999);
            (*this->enemies)[this->enemies->size() - 1]->field_0x74 = 1;
            this->objectivesA = new Objective(4, 2, this);
            break;
        }
        case 147:
            this->objectivesA = new Objective(4, 1, this);
            break;
        case 154: {
            int coords[3] = {200000, 0, 70000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(22, *this->enemies);
            int friendCoords[3] = {0, 0, 40000};
            this->friendRoute = new Route(friendCoords, 3);
            (*this->enemies)[0] = this->createShip(0, 0, 51, this->friendRoute->getWaypoint(0), true, false);
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            (*this->enemies)[0]->setPosition(this->player->getPosition() +
                                             this->player->geometry->getRightVector() * 2100.0f +
                                             this->player->geometry->getDirection() * 2000.0f);
            (*this->enemies)[0]->geometry->setDirection(this->player->GetDirVector(), Vector{0.0f, 1.0f, 0.0f});
            (*this->enemies)[0]->setEnemies(nullptr);
            ((PlayerFighter *) (*this->enemies)[0])->field_0x13d = 0;
            delete (*this->enemies)[0]->cargo;
            (*this->enemies)[0]->cargo = nullptr;
            (*this->enemies)[1] = this->createStaticObject(nullptr, 16928, false);
            ((PlayerFixedObject *) (*this->enemies)[1])->setMoving(false);
            ((PlayerFixedObject *) (*this->enemies)[1])->setDockingType(3);
            ((PlayerFixedObject *) (*this->enemies)[1])->setName(*Globals::gameText->getText(77));
            (*this->enemies)[1]->player->setAlwaysFriend(true);
            (*this->enemies)[1]->geometry->setRotation(0.0f, 3.1415927f, 0.0f);
            (*this->enemies)[1]->field_0x70 = 0;
            (*this->enemies)[1]->field_0x74 = 1;
            delete (*this->enemies)[1]->cargo;
            (*this->enemies)[1]->cargo = nullptr;
            int dockCoords[9] = {-10000, 1000, 15000, -1, -1, -1, -10000, 1000, 15000};
            int waitTimes[3] = {0, 10000, 0};
            Array<KIPlayer *> *stops = new Array<KIPlayer *>();
            ArraySetLength(3, *stops);
            (*stops)[1] = (*this->enemies)[1];
            Route *dockRoute = new Route(dockCoords, stops, waitTimes, 9);
            (*this->enemies)[0]->setRoute(dockRoute);
            ((PlayerFighter *) (*this->enemies)[0])->field_0x13e = 0;
            (*this->enemies)[0]->player->setVulnerable(false);
            Vector center = {0.0f, 0.0f, 40000.0f};
            for (int i = 2; i < 22; i++) {
                (*this->enemies)[i] = this->createShip(9, 0, Globals::globals->getRandomEnemyFighter(9),
                                                       this->enemyRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->setActive(true);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                KIPlayer *attacker = (*this->enemies)[i];
                if (i <= 10) {
                    int x = (Globals::rnd->nextInt(100) + 900) * (i % 2 == 0 ? 1 : -1);
                    attacker->setPosition(center + (float) i * Vector{(float) x, (float) (Globals::rnd->nextInt(200) - 200),
                                                                      (float) (Globals::rnd->nextInt(400) - 200)});
                } else {
                    int x = (Globals::rnd->nextInt(100) + 1600) * (i % 2 == 0 ? 1 : -1);
                    attacker->setPosition(center + (float) i * Vector{(float) x, (float) (Globals::rnd->nextInt(100) - 200),
                                                                      (float) (Globals::rnd->nextInt(500) + 1000)});
                }
                (*this->enemies)[i]->geometry->setDirection(
                        VectorNormalize(this->player->getPosition() - (*this->enemies)[i]->getPosition()),
                        Vector{0.0f, 1.0f, 0.0f});
            }
            this->objectivesA = new Objective(4, 11, this);
            break;
        }
        case 155:
            this->objectivesA = new Objective(4, 2, this);
            break;
        case 157: {
            int friendCoords[6] = {70000, 0, 20000, 30000, 10000, 60000};
            this->friendRoute = new Route(friendCoords, 6);
            this->friendRoute->getWaypoint(0)->reached();
            int enemyCoords[6] = {-110000, 10000, 170000, -110000, 0, 20000};
            this->enemyRoute = new Route(enemyCoords, 6);
            this->enemyRoute->getWaypoint(0)->reached();
            this->enemies = new Array<KIPlayer *>();
            unsigned char weakDevice = Globals::isRunningHDonWeakDevice;
            int fighters = weakDevice ? 5 : 10;
            int docks = weakDevice ^ 1;
            ArraySetLength(2 * fighters + docks + 3, *this->enemies);
            Vector stationPos = {-120000.0f, 0.0f, 20000.0f};
            Station *station = (Station *) (intptr_t) Globals::galaxy->getStation(101);
            int friendEnd = fighters + docks;
            int enemyEnd = friendEnd + fighters;
            int stationIndex = enemyEnd + 2;
            (*this->enemies)[stationIndex] = new PlayerStation(station);
            (*this->enemies)[stationIndex]->name = station->getName();
            (*this->enemies)[stationIndex]->shipGroup = 3;
            (*this->enemies)[stationIndex]->setPosition(stationPos);
            delete (*this->enemies)[stationIndex]->cargo;
            (*this->enemies)[stationIndex]->cargo = nullptr;
            delete station;
            for (int i = 0; i < fighters; i++) {
                (*this->enemies)[i] = this->createShip(0, 0, Globals::globals->getRandomEnemyFighter(0),
                                                       this->friendRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                (*this->enemies)[i]->setRoute(this->friendRoute->getExactClone());
            }
            for (int i = fighters; i < friendEnd; i++) {
                (*this->enemies)[i] = this->createStaticObject(nullptr, 18804, true);
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                (*this->enemies)[i]->setPosition(30000.0f, -15000.0f, -50000.0f);
                delete (*this->enemies)[i]->cargo;
                (*this->enemies)[i]->cargo = nullptr;
            }
            Vector dir = this->enemyRoute->getWaypoint(1)->getPosition() - this->enemyRoute->getWaypoint(0)->getPosition();
            dir.y = 0.0f;
            dir = VectorNormalize(dir);
            Vector start = this->enemyRoute->getWaypoint(0)->getPosition();
            for (int j = 0; friendEnd + j < enemyEnd; j++) {
                int i = friendEnd + j;
                (*this->enemies)[i] = this->createShip(10, 0, 44, nullptr, true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                (*this->enemies)[i]->setVisible(false);
                (*this->enemies)[i]->setActive(false);
                (*this->enemies)[i]->geometry->setDirection(dir, Vector{0.0f, 1.0f, 0.0f});
                ((PlayerFighter *) (*this->enemies)[i])->setAIDisabled(true);
                ((PlayerFighter *) (*this->enemies)[i])->setCloakingPossible(false);
                (*this->enemies)[i]->setRoute(this->enemyRoute->getExactClone());
                int x;
                float z;
                if (j < 2) {
                    x = -1000 + j * 2000;
                    z = 5000.0f;
                } else if (j < 5) {
                    x = j * 3000 - 9000;
                    z = 7000.0f;
                } else if (j < 9) {
                    x = j * 3000 - 20000;
                    z = 9000.0f;
                } else {
                    x = 0;
                    z = 11000.0f;
                }
                this->spawnPosition = Vector{(float) x, 0.0f, z};
                this->spawnPosition.x += (float) (Globals::rnd->nextInt(200) - 100);
                this->spawnPosition.y += (float) (Globals::rnd->nextInt(200) - 100);
                this->spawnPosition.z += (float) (Globals::rnd->nextInt(200) - 100);
                (*this->enemies)[i]->setPosition(start + this->spawnPosition);
            }
            for (int i = enemyEnd; i <= enemyEnd; i++) {
                (*this->enemies)[i] = this->createShip(2, 0, 49, nullptr, true, false);
                (*this->enemies)[i]->setPosition(start);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                ((PlayerFighter *) (*this->enemies)[i])->setAIDisabled(true);
                (*this->enemies)[i]->geometry->setDirection(dir, Vector{0.0f, 1.0f, 0.0f});
                (*this->enemies)[i]->setActive(false);
                (*this->enemies)[i]->setVisible(false);
                (*this->enemies)[i]->setSpeed(5.0f);
                (*this->enemies)[i]->name = *Globals::gameText->getText(1636);
                delete (*this->enemies)[i]->cargo;
                (*this->enemies)[i]->cargo = nullptr;
            }
            for (int i = enemyEnd + 1; i < stationIndex; i++) {
                (*this->enemies)[i] = this->createShip(3, 0, 20, nullptr, true, false);
                (*this->enemies)[i]->setPosition(50000.0f, 50000.0f, 50000.0f);
                ((PlayerFighter *) (*this->enemies)[i])->setAIDisabled(true);
                (*this->enemies)[i]->setActive(false);
                (*this->enemies)[i]->setVisible(false);
                (*this->enemies)[i]->name = *Globals::gameText->getText(1623);
            }
            this->player->setPosition(100000.0f, 0.0f, 20000.0f);
            this->player->geometry->setDirection(VectorNormalize(Vector{-1.0f, -0.2f, -0.5f}), Vector{0.0f, 1.0f, 0.0f});
            break;
        }
        case 158:
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            (*this->enemies)[0] = this->createShip(10, 0, 49, nullptr, true, false);
            (*this->enemies)[0]->setPosition(9000.0f, 0.0f, -13000.0f);
            (*this->enemies)[0]->geometry->setDirection(VectorNormalize(Vector{-1.0f, 0.0f, 3.0f}), Vector{0.0f, 1.0f, 0.0f});
            (*this->enemies)[0]->player->setAlwaysEnemy(true);
            (*this->enemies)[0]->setSpeed(5.3f);
            {
                Player *p = (*this->enemies)[0]->player;
                p->setMaxEmpPoints(p->getMaxEmpPoints() * 3);
            }
            ((PlayerFighter *) (*this->enemies)[0])->setCloakingPossible(false);
            (*this->enemies)[0]->name = *Globals::gameText->getText(1636);
            ((PlayerFighter *) (*this->enemies)[0])->setAIDisabled(true);
            delete (*this->enemies)[0]->cargo;
            (*this->enemies)[0]->cargo = nullptr;
            for (int i = 0; i < 3; i++) {
                KIPlayer *mine = this->createStaticObject(nullptr, 18882, true);
                mine->player->setRadius(800);
                mine->player->setAlwaysEnemy(true);
                mine->player->setAlwaysFriend(false);
                mine->player->setMaxHitpoints(100);
                mine->setPosition(50000.0f, 50000.0f, 50000.0f);
                mine->setActive(false);
                ArrayAdd(mine, *this->enemies);
            }
            this->objectivesA = new Objective(22, 0, this);
            break;
        case 114: {
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(6, *this->enemies);
            Vector pos = {0.0f, 0.0f, 0.0f};
            int coords[3] = {0, 0, 30000};
            this->enemyRoute = new Route(coords, 3);
            for (int i = 0; i < 6; i++) {
                (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8),
                                                       this->enemyRoute->getWaypoint(0), true, false);
                pos = (*this->asteroids)[i + this->asteroids->size() / 2]->getPosition();
                (*this->enemies)[i]->setPosition(pos.x, pos.y + 2000.0f, pos.z);
                (*this->enemies)[i]->setToSleep();
                ((PlayerFighter *) (*this->enemies)[i])->field_0x128 = 7000;
                Player *p = (*this->enemies)[i]->player;
                p->setMaxHitpoints(p->getMaxHitpoints() * 2);
                p = (*this->enemies)[i]->player;
                p->setHitpoints(p->getMaxHitpoints());
            }
            this->objectivesA = new Objective(18, 0, 6, this);
            break;
        }
        case 120: {
            int coords[3] = {0, 0, -80000};
            this->enemyRoute = new Route(coords, 3);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(2, *this->enemies);
            for (int i = 0; i < 2; i++) {
                (*this->enemies)[i] = this->createShip(10, 0, 44, this->enemyRoute->getWaypoint(0), true, false);
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
                (*this->enemies)[i]->setPosition(this->player->getPosition() +
                                                 this->player->geometry->getRightVector() * 50000.0f +
                                                 this->player->geometry->getDirection() * (i == 0 ? 4000.0f : -4000.0f));
            }
            this->objectivesA = new Objective(4, 0, this);
            break;
        }
        case 123:
            this->objectivesA = new Objective(4, 3, this);
            break;
        case 125: {
            int coords[6] = {0, 0, 0, 7000000, 7000000, 7000000};
            this->enemyRoute = new Route(coords, 6);
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(15, *this->enemies);
            for (int i = 0; i < 8; i++) {
                int type = Globals::globals->getRandomEnemyFighter(8);
                if (i < 2) {
                    (*this->enemies)[i] = this->createShip(8, 0, type, this->enemyRoute->getWaypoint(0), true, false);
                } else {
                    (*this->enemies)[i] = this->createShip(8, 0, type, this->enemyRoute->getWaypoint(1), true, false);
                    (*this->enemies)[i]->setToSleep();
                }
                (*this->enemies)[i]->player->setAlwaysEnemy(true);
            }
            Waypoint *dockA = new Waypoint(-80000, 0, -160000, nullptr);
            Waypoint *dockB = new Waypoint(-72000, 0, -190000, nullptr);
            Waypoint *dockC = new Waypoint(-66000, 53000, -170000, nullptr);
            (*this->enemies)[8] = this->createStaticObject(dockA, 18785, true);
            (*this->enemies)[9] = this->createStaticObject(dockB, 18785, true);
            (*this->enemies)[10] = this->createStaticObject(dockC, 18785, true);
            for (int i = 8; i < 11; i++) {
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                ((PlayerFixedObject *) (*this->enemies)[i])->setDockingType(3);
                (*this->enemies)[i]->player->setAlwaysEnemy(false);
                (*this->enemies)[i]->player->setAlwaysFriend(true);
                (*this->enemies)[i]->field_0x74 = 1;
                delete (*this->enemies)[i]->cargo;
                (*this->enemies)[i]->cargo = nullptr;
            }
            for (int i = 11; i < 15; i++) {
                (*this->enemies)[i] = this->createStaticObject(nullptr, 18786, true);
                ((PlayerFixedObject *) (*this->enemies)[i])->setMoving(false);
                KIPlayer *debris = (*this->enemies)[i];
                debris->setPosition((float) (-72000 + (i - 11) * 4000),
                                    (float) (Globals::rnd->nextInt(1500) - (i - 11) * 200),
                                    (float) (-145000 - (i - 11) * 4000));
                (*this->enemies)[i]->geometry->rotate((float) i + -4.31969f, (float) i + -11.519174f, (float) i + -5.759587f);
                (*this->enemies)[i]->field_0x74 = 1;
                delete (*this->enemies)[i]->cargo;
                (*this->enemies)[i]->cargo = nullptr;
            }
            this->objectivesA = new Objective(4, 7, this);
            break;
        }
        default:
            break;
    }
    this->messages = this->createRadioMessages(Globals::status->getCurrentCampaignMission());
}

void Level::updateOrbit(int dt) {
    orbitWaveTimer += dt;
    field_178 += dt;
    if (field_18a) {
        if (Globals::status->getSystem() != 0 && player->radioRef != nullptr) {
            alarmAllFriends(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace(), false);
            createRadioMessage(2, ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace());
            field_18a = 0;
        }
    }

    // respawn one jumper every ten seconds
    if (field_178 > 10000) {
        field_178 = 0;
        if (enemies != nullptr) {
            for (unsigned int i = 0; i < enemies->size(); i++) {
                KIPlayer *ship = (*enemies)[i];
                if (ship->isJumper() && ship->isDead() && !ship->player->isActive() && !ship->field_0x42) {
                    ship->revive();
                    if ((int) i < friendCount + field_164) {
                        ship->setPosition(0.0f, 0.0f, 0.0f);
                    } else {
                        ship->setPosition(10000.0f, 0.0f, -70000.0f);
                    }
                    break;
                }
            }
        }
    }

    if (orbitWaveTimer > 45000) {
        orbitWaveTimer = 0;
        int deadHostiles = 0;
        if (hostileCount > 0) {
            if (enemies == nullptr) {
                return;
            }
            for (unsigned int i = 0; i < enemies->size(); i++) {
                if (i >= enemies->size() - hostileCount && !(*enemies)[i]->isWingMan() && (*enemies)[i]->isDead()) {
                    deadHostiles += (*enemies)[i]->player->isActive() ^ 1;
                }
            }
        }
        if (enemies == nullptr) {
            return;
        }
        bool spawned = false;
        for (unsigned int i = 0; i < enemies->size(); i++) {
            KIPlayer *ship = (*enemies)[i];
            if (friendCount > 0 && (int) i < friendCount && ship->isDead() && !ship->player->isActive()
                && !ship->field_0x42) {
                ship->revive();
                ship->setPosition(0.0f, 0.0f, 0.0f);
            }
            if (deadHostiles >= 2 && field_184 < 2 && hostileCount > 0 && i >= enemies->size() - hostileCount) {
                int group = ship->shipGroup;
                bool lawless = Globals::status->inAlienOrbit() ? false
                        : ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getSecurityLevel() == 0;
                bool lowSecurity = Globals::status->inAlienOrbit() ? false
                        : ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getSecurityLevel() == 1;
                if (ship->isDead() && !ship->player->isActive() && !ship->field_0x42
                    && (group == 9 || lawless || (lowSecurity && field_17c <= 2))) {
                    field_17c++;
                    ship->revive();
                    Vector position = player->getPosition();
                    position.x += (float) ((Globals::rnd->nextInt(60000) + 60000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                    position.y += (float) ((Globals::rnd->nextInt(5000) + 5000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                    position.z += (float) ((Globals::rnd->nextInt(60000) + 60000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                    ship->setPosition(position.x, position.y, position.z);
                    spawned = true;
                }
            }
        }
        if (spawned) {
            field_184++;
        }
    }
}

void Level::friendTurnedEnemy(int race) {
    if (field_188 == 0) {
        field_188 = 1;
        createRadioMessage(0, race);
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
    int count = 0;
    if (this->enemies != nullptr) {
        for (unsigned int i = 0; i < this->enemies->size(); i++) {
            if ((*this->enemies)[i]->deadFlag == 0 && (*this->enemies)[i]->field_0x71 == 0
                && (*this->enemies)[i]->field_0x3f_b == 0 && !(*this->enemies)[i]->isWingMan()
                && (char) (*this->enemies)[i]->field_0x44 == 0 && (*this->enemies)[i]->stealFlagByte == 0) {
                count += (*this->enemies)[i]->countsAsEnemyExcludeFlag ^ 1;
            }
        }
    }
    count = this->enemies != nullptr ? count - field_120 : 0;
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
        for (unsigned int i = 0; i < field_b0->size(); i = i + 1) {
            KIPlayer *obj = createStaticObject((Waypoint *) (intptr_t) 0, i / 3 + 0x49c0, 1);
            (*field_b0)[i] = obj;
            KIPlayer *k = (*field_b0)[i];
            k->player->setRadius(800);
            k->player->setAlwaysFriend(1);
            k->player->setMaxHitpoints(100);
            k->setPosition(50000.0f, 50000.0f, 50000.0f);
            k->setActive(false);
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

        for (int i = 0;
             i < (*Globals::status->wanted)[index]->getNumWingmen();
             i = i + 1) {
            (*enemies)[i + 1]->player->setAlwaysEnemy(1);
            (*enemies)[i + 1]->player->turnEnemy();
        }
    }
}

void Level::update(long long dt, bool skipLod) {
    if (flashActive) {
        flashDurationA -= (int) dt;
        if (flashDurationA < 0) {
            flashActive = 0;
        }
        float fade = (float) flashDurationA / (float) flashDurationB;
        float red = flashColor.x * fade;
        float green = flashColor.y * fade;
        float blue = flashColor.z * fade;
        float alpha = flashAlpha * fade;
        flashColor.x = i_r > red ? i_r : red;
        flashColor.y = i_g > green ? i_g : green;
        flashColor.z = i_b > blue ? i_b : blue;
        flashAlpha = i_a > alpha ? i_a : alpha;
    }

    if (Globals::status->getMission() == nullptr || Globals::status->getMission()->isEmpty()) {
        updateOrbit((int) dt);
    } else {
        updateMissionOrbit((int) dt);
    }
    if (Globals::status->getStation()->isAttackedByAliens() || Globals::status->inAlienOrbit()) {
        updateAlienAttackers((int) dt);
    }

    if (playerGuns != nullptr) {
        for (unsigned int i = 0; i < playerGuns->size(); i++) {
            (*playerGuns)[i]->update((int) dt);
        }
    }
    if (enemyGuns != nullptr) {
        for (unsigned int i = 0; i < enemyGuns->size(); i++) {
            (*enemyGuns)[i]->update((int) dt);
        }
    }

    float gammaDamage = Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                                    Globals::status->getCurrentCampaignMission());
    if (player != nullptr && gammaDamage > 0.0f) {
        Item *shield = Globals::status->getShip()->getFirstEquipmentOfSort(0x26);
        if (shield != nullptr) {
            int protection = shield->getAttribute(0x34);
            if (protection > 0) {
                gammaDamage *= (100.0f - (float) protection) / 100.0f;
            }
        }
        gammaDamage = gammaDamage * (float) dt / 1000.0f;
        int gammaBefore = player->player->getGammaHP();
        player->player->damageGamma(gammaDamage);
        if (gammaBefore >= 15 && player->player->getGammaHP() <= 14) {
            ((Hud *) player->getHUD())->hudEvent(0x2c, player, 0);
        }
    }

    if (player != nullptr) {
        if (field_80 != nullptr) {
            playerPosition = player->getPosition();
            field_80->update(dt);
        }
        if (field_74 != nullptr) {
            field_74->update(dt);
        }
        if (particleEmitBoolPtr != nullptr) {
            particleEmitBoolPtr->update(dt);
        }
        if (skybox2Mesh != nullptr) {
            playerPosition = player->getPosition();
            skybox2Mesh->update(dt);
        }
        if (particleSystemMgr != nullptr) {
            particleSystemMgr->update(dt);
        }
        if (particleRenderBoolPtr != nullptr) {
            particleRenderBoolPtr->update(dt);
        }
        if (field_8c != nullptr) {
            field_8c->update(dt);
        }
        if (field_98 != nullptr) {
            field_98->update(dt);
        }
        if (field_94 != nullptr) {
            field_94->update(dt);
        }
        if (field_9c != nullptr) {
            field_9c->update(dt);
        }
    }
    if (!skipLod) {
        lodManager->update((int) dt);
    }
}

void Level::connectPlayers() {
    if ((int) (intptr_t) Globals::appManager->GetCurrentApplicationModule() == 5) {
        return;
    }

    if (enemies != nullptr && player != nullptr) {
        Array<Player *> *targets = new Array<Player *>();
        ArraySetLength(enemies->size(), *targets);
        for (unsigned int i = 0; i < targets->size(); i++) {
            (*targets)[i] = (*enemies)[i]->player;
        }
        player->player->setEnemies(targets);
        delete targets;
    }
    if (asteroids != nullptr && player != nullptr) {
        Array<Player *> *targets = new Array<Player *>();
        ArraySetLength(asteroids->size(), *targets);
        for (unsigned int i = 0; i < targets->size(); i++) {
            (*targets)[i] = (*asteroids)[i]->player;
        }
        player->player->addEnemies(targets);
        delete targets;
    }
    if (gasClouds != nullptr && player != nullptr) {
        Array<Player *> *targets = new Array<Player *>();
        ArraySetLength(gasClouds->size(), *targets);
        for (unsigned int i = 0; i < targets->size(); i++) {
            (*targets)[i] = (*gasClouds)[i]->player;
        }
        player->player->addEnemies(targets);
        delete targets;
    }
    if (enemies == nullptr) {
        return;
    }

    int campaignMission = Globals::status->getCurrentCampaignMission();
    for (unsigned int i = 0; i < enemies->size(); i++) {
        KIPlayer *ship = (*enemies)[i];
        int group = ship->shipGroup;
        bool wingman = ship->isWingMan();
        bool firstOfMission24 = campaignMission == 0x24 && i == 0;

        unsigned int count = 0;
        for (unsigned int k = 0; k < enemies->size(); k++) {
            KIPlayer *other = (*enemies)[k];
            if (other == ship || (!wingman & (other->shipGroup == group))) {
                continue;
            }
            if (firstOfMission24) {
                if (other->isWingMan()) {
                    continue;
                }
            } else if (campaignMission == 0x9a) {
                bool alienOrbit = Globals::status->inAlienOrbit();
                if (k == 8 && alienOrbit && ship->shipGroup == 8) {
                    continue;
                }
            } else if (campaignMission == 0x40 && !(i != 0 && ship != nullptr && k != 0)) {
                continue;
            }
            if (ship->field_0x40 && ((PlayerFixedObject *) ship)->getDockingType() == 3
                && Globals::status->getStation()->stationHasHiddenBlueprint(true)) {
                continue;
            }
            count++;
        }

        Array<Player *> *targets = new Array<Player *>();
        if (player != nullptr) {
            count++;
        }
        ArraySetLength(count, *targets);

        Mission *mission = Globals::status->getMission();
        bool friendly = ((i & 1) && mission->getType() == 12) || mission->getType() == 2 || mission->getType() == 9
                        || (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 0x10
                            && ship->shipGroup == 9)
                        || (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 0x18
                            && ship->shipGroup == 9)
                        || (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 0x1c
                            && ship->shipGroup == 9);
        if (!friendly) {
            if (mission->isCampaignMission()) {
                Globals::status->getCurrentCampaignMission();
            }
            if (mission->isCampaignMission()) {
                Globals::status->getCurrentCampaignMission();
            }
            int slot = 0;
            if (player != nullptr) {
                (*targets)[0] = player->player;
                slot = 1;
            }
            for (unsigned int k = 0; k < enemies->size(); k++) {
                KIPlayer *other = (*enemies)[k];
                if (other == ship || (!wingman & (other->shipGroup == group))) {
                    continue;
                }
                if (firstOfMission24) {
                    if (other->isWingMan()) {
                        continue;
                    }
                } else if (campaignMission == 0x9a) {
                    bool alienOrbit = Globals::status->inAlienOrbit();
                    if (k == 8 && alienOrbit && ship->shipGroup == 8) {
                        continue;
                    }
                } else if (campaignMission == 0x40 && !(i != 0 && ship != nullptr && k != 0)) {
                    continue;
                }
                if (ship->field_0x40 && ((PlayerFixedObject *) ship)->getDockingType() == 3
                    && Globals::status->getStation()->stationHasHiddenBlueprint(true)) {
                    continue;
                }
                (*targets)[slot++] = (*enemies)[k]->player;
            }
        } else {
            if (ship->player->isAlwaysFriend()) {
                delete targets;
                targets = new Array<Player *>();
                ArraySetLength(1, *targets);
            } else {
                int slot = 0;
                for (unsigned int k = 0; k < enemies->size(); k++) {
                    KIPlayer *other = (*enemies)[k];
                    if (other == ship || other->stealFlagByte || (!wingman & (other->shipGroup == group))) {
                        continue;
                    }
                    if (firstOfMission24) {
                        if (other->isWingMan()) {
                            continue;
                        }
                    } else if (campaignMission == 0x40) {
                        if (i == 0 || ship == nullptr || k == 0) {
                            continue;
                        }
                    }
                    (*targets)[slot++] = (*enemies)[k]->player;
                }
            }
            (*targets)[targets->size() - 1] = player->player;
        }
        ship->player->addEnemies(targets);
        delete targets;

        if (Globals::status->getMission()->isEmpty() && group == 10) {
            ship->player->setEnemy(player->player);
        }
    }
}

void Level::enemyDied(int /*index*/, bool killedByFriend) {
    enemiesLeft--;
    kills++;
    if (killedByFriend) {
        killCountA++;
        return;
    }

    Globals::status->incKills();
    killCountB++;
    if (player == nullptr) {
        return;
    }

    // medal 40: kills without a scanner
    if (!static_cast<Radar *>(player->field_0x14)->hasScanner()
        && !Globals::achievements->hasMedal(0x28, 1)) {
        int count = Globals::status->field_11c;
        if (!Globals::status->field_120) {
            Globals::status->field_11c = ++count;
        }
        if ((int) ((float) count / (float) Globals::achievements->getValue(0x28, 1) * 100.0f) % 10 == 0) {
            ((Hud *) player->getHUD())->hudEventMedal(0x28,
                    (int) ((float) Globals::status->field_11c
                           / (float) Globals::achievements->getValue(0x28, 1) * 100.0f));
        }
        if (Globals::status->field_11c >= Globals::achievements->getValue(0x28, 1)) {
            Globals::status->field_120 = 1;
        }
    }

    // medal 43: kills while the emergency system is active
    if (player != nullptr && player->emergencySystemActive()
        && !Globals::achievements->hasMedal(0x2b, 1)) {
        int count = ++Globals::status->field_13c;
        if ((int) ((float) count / (float) Globals::achievements->getValue(0x2b, 1)) > 0) {
            ((Hud *) player->getHUD())->hudEventMedal(0x2b,
                    (int) ((float) Globals::status->field_13c
                           / (float) Globals::achievements->getValue(0x2b, 1) * 100.0f));
        }
        if (Globals::status->field_13c >= Globals::achievements->getValue(0x2b, 1)) {
            Globals::status->field_140 = 1;
        }
    }
}

Array<RadioMessage *> *Level::createRadioMessages(int set) {
    this->messages = nullptr;

    switch (set) {
        case 0:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(23, *this->messages);
            (*this->messages)[0] = new RadioMessage(1668, 17, 5, 1500);
            (*this->messages)[1] = new RadioMessage(1669, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(1670, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(1671, 10, 6, 2);
            (*this->messages)[4] = new RadioMessage(1672, 11, 6, 3);
            (*this->messages)[5] = new RadioMessage(1673, 9, 6, 4);
            (*this->messages)[6] = new RadioMessage(1674, 9, 6, 5);
            (*this->messages)[7] = new RadioMessage(1675, 9, 6, 6);
            (*this->messages)[8] = new RadioMessage(1676, 0, 6, 7);
            (*this->messages)[9] = new RadioMessage(1683, 0, 9, 0, 3);
            (*this->messages)[10] = new RadioMessage(1684, 0, 6, 9);
            (*this->messages)[11] = new RadioMessage(1685, 0, 6, 10);
            (*this->messages)[12] = new RadioMessage(1686, 15, 6, 11);
            (*this->messages)[13] = new RadioMessage(1687, 0, 6, 12);
            (*this->messages)[14] = new RadioMessage(1688, 0, 6, 13);
            (*this->messages)[15] = new RadioMessage(1689, 0, 6, 14);
            (*this->messages)[16] = new RadioMessage(1690, 0, 27, 12);
            (*this->messages)[17] = new RadioMessage(1691, 15, 6, 16);
            (*this->messages)[18] = new RadioMessage(1692, 0, 6, 17);
            (*this->messages)[19] = new RadioMessage(1693, 15, 6, 18);
            (*this->messages)[20] = new RadioMessage(1694, 0, 6, 19);
            (*this->messages)[21] = new RadioMessage(1695, 0, 6, 20);
            (*this->messages)[22] = new RadioMessage(1696, 15, 6, 21);
            break;
        case 1:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(3, *this->messages);
            (*this->messages)[0] = new RadioMessage(1697, 2, 5, 10000);
            (*this->messages)[1] = new RadioMessage(1698, 2, 6, 0);
            (*this->messages)[2] = new RadioMessage(1699, 2, 6, 1);
            break;
        case 7:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(2, *this->messages);
            (*this->messages)[0] = new RadioMessage(1756, 2, 16, 0);
            (*this->messages)[1] = new RadioMessage(1757, 0, 6, 0);
            break;
        case 14:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(5, *this->messages);
            (*this->messages)[0] = new RadioMessage(1820, 18, 5, 10000);
            (*this->messages)[1] = new RadioMessage(1821, 18, 20, 1);
            (*this->messages)[2] = new RadioMessage(1822, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(1823, 18, 6, 2);
            (*this->messages)[4] = new RadioMessage(1824, 17, 6, 3);
            break;
        case 16:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(5, *this->messages);
            (*this->messages)[0] = new RadioMessage(1838, 19, 5, 10000);
            (*this->messages)[1] = new RadioMessage(1839, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(1840, 0, 9, 0, 3);
            (*this->messages)[3] = new RadioMessage(1841, 1, 6, 2);
            (*this->messages)[4] = new RadioMessage(1842, 0, 6, 3);
            break;
        case 21:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(5, *this->messages);
            (*this->messages)[0] = new RadioMessage(1881, 10, 16, 0);
            (*this->messages)[1] = new RadioMessage(1882, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(1883, 10, 25, 2);
            (*this->messages)[3] = new RadioMessage(1884, 14, 8, 0);
            (*this->messages)[4] = new RadioMessage(1885, 14, 21, 0);
            break;
        case 24:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(5, *this->messages);
            (*this->messages)[0] = new RadioMessage(1917, 19, 5, 12000);
            (*this->messages)[1] = new RadioMessage(1918, 6, 6, 0);
            (*this->messages)[2] = new RadioMessage(1919, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(1920, 6, 22, 3);
            (*this->messages)[4] = new RadioMessage(1921, 6, 6, 3);
            break;
        case 25:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(3, *this->messages);
            (*this->messages)[0] = new RadioMessage(1925, 0, 5, 20000);
            (*this->messages)[1] = new RadioMessage(1926, 6, 6, 0);
            (*this->messages)[2] = new RadioMessage(1927, 0, 6, 1);
            break;
        case 28:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(3, *this->messages);
            (*this->messages)[0] = new RadioMessage(1944, 0, 5, 20000);
            (*this->messages)[1] = new RadioMessage(1945, 19, 6, 0);
            (*this->messages)[2] = new RadioMessage(1946, 0, 6, 1);
            break;
        case 29:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(6, *this->messages);
            (*this->messages)[0] = new RadioMessage(1948, 0, 23, 0);
            (*this->messages)[1] = new RadioMessage(1949, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(1950, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(1951, 0, 6, 2);
            (*this->messages)[4] = new RadioMessage(1952, 19, 5, 120000);
            (*this->messages)[5] = new RadioMessage(1953, 0, 6, 4);
            break;
        case 38:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2029, 21, 5, 15000);
            break;
        case 40:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(7, *this->messages);
            (*this->messages)[0] = new RadioMessage(2042, 0, 5, 10000);
            (*this->messages)[1] = new RadioMessage(2043, 8, 6, 0);
            (*this->messages)[2] = new RadioMessage(2044, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(2045, 7, 5, 40000);
            (*this->messages)[4] = new RadioMessage(2046, 0, 6, 3);
            (*this->messages)[5] = new RadioMessage(2047, 7, 12, 0);
            (*this->messages)[6] = new RadioMessage(2048, 0, 24, 0);
            break;
        case 41:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(8, *this->messages);
            (*this->messages)[0] = new RadioMessage(2052, 0, 5, 80000);
            (*this->messages)[1] = new RadioMessage(2053, 7, 6, 0);
            (*this->messages)[2] = new RadioMessage(2054, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(2055, 7, 6, 2);
            (*this->messages)[4] = new RadioMessage(2056, 7, 26, 4294867296);
            (*this->messages)[5] = new RadioMessage(2057, 7, 6, 4);
            (*this->messages)[6] = new RadioMessage(2063, 0, 1, 0);
            (*this->messages)[7] = new RadioMessage(2064, 0, 6, 6);
            break;
        case 49:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2120, 0, 5, 8000);
            break;
        case 50:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(4, *this->messages);
            (*this->messages)[0] = new RadioMessage(2121, 63, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2122, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2123, 63, 6, 1);
            (*this->messages)[3] = new RadioMessage(2124, 0, 6, 2);
            break;
        case 51:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(2, *this->messages);
            (*this->messages)[0] = new RadioMessage(2125, 63, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2126, 0, 6, 0);
            break;
        case 52:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(2, *this->messages);
            (*this->messages)[0] = new RadioMessage(2127, 63, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2128, 0, 6, 0);
            break;
        case 53:
        case 54:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2129, 0, 5, 8000);
            break;
        case 55:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(2, *this->messages);
            (*this->messages)[0] = new RadioMessage(2138, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2139, 0, 6, 0);
            break;
        case 56:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(6, *this->messages);
            (*this->messages)[0] = new RadioMessage(2154, 27, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2155, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2156, 28, 16, 0);
            (*this->messages)[3] = new RadioMessage(2157, 0, 20, 3);
            (*this->messages)[4] = new RadioMessage(2158, 0, 6, 3);
            (*this->messages)[5] = new RadioMessage(2159, 27, 28, 0);
            break;
        case 62:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2209, 0, 5, 8000);
            break;
        case 63:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(3, *this->messages);
            (*this->messages)[0] = new RadioMessage(2223, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2224, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2225, 29, 6, 1);
            break;
        case 64:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(7, *this->messages);
            (*this->messages)[0] = new RadioMessage(2228, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2229, 20, 6, 0);
            (*this->messages)[2] = new RadioMessage(2230, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(2231, 30, 6, 2);
            (*this->messages)[4] = new RadioMessage(2232, 0, 6, 3);
            (*this->messages)[5] = new RadioMessage(2233, 30, 20, 5);
            (*this->messages)[6] = new RadioMessage(2234, 0, 6, 5);
            break;
        case 65:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(3, *this->messages);
            (*this->messages)[0] = new RadioMessage(2251, 0, 5, 12000);
            (*this->messages)[1] = new RadioMessage(2252, 20, 6, 0);
            (*this->messages)[2] = new RadioMessage(2253, 0, 6, 1);
            break;
        case 67:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(12, *this->messages);
            (*this->messages)[0] = new RadioMessage(2287, 0, 16, 0);
            (*this->messages)[1] = new RadioMessage(2288, 31, 6, 0);
            (*this->messages)[2] = new RadioMessage(2289, 30, 6, 1);
            (*this->messages)[3] = new RadioMessage(2290, 0, 20, 2);
            (*this->messages)[4] = new RadioMessage(2291, 31, 6, 3);
            (*this->messages)[5] = new RadioMessage(2292, 0, 6, 4);
            (*this->messages)[6] = new RadioMessage(2293, 0, 20, 8);
            (*this->messages)[7] = new RadioMessage(2294, 31, 6, 6);
            (*this->messages)[8] = new RadioMessage(2295, 0, 6, 7);
            (*this->messages)[9] = new RadioMessage(2296, 31, 6, 8);
            (*this->messages)[10] = new RadioMessage(2297, 0, 6, 9);
            (*this->messages)[11] = new RadioMessage(2298, 31, 6, 10);
            break;
        case 69:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(2, *this->messages);
            (*this->messages)[0] = new RadioMessage(2318, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2319, 0, 6, 0);
            break;
        case 70:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(4, *this->messages);
            (*this->messages)[0] = new RadioMessage(2320, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2321, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2322, 34, 6, 1);
            (*this->messages)[3] = new RadioMessage(2323, 0, 6, 2);
            break;
        case 73:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(8, *this->messages);
            (*this->messages)[0] = new RadioMessage(2347, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2348, 0, 16, 0);
            (*this->messages)[2] = new RadioMessage(2349, 11, 6, 1);
            (*this->messages)[3] = new RadioMessage(2350, 0, 6, 2);
            (*this->messages)[4] = new RadioMessage(2351, 0, 27, 1);
            (*this->messages)[5] = new RadioMessage(2352, 33, 27, 2);
            (*this->messages)[6] = new RadioMessage(2353, 0, 27, 3);
            (*this->messages)[7] = new RadioMessage(2354, 0, 6, 6);
            break;
        case 78:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(4, *this->messages);
            (*this->messages)[0] = new RadioMessage(2409, 0, 5, 2000);
            (*this->messages)[1] = new RadioMessage(2410, 0, 27, 9);
            (*this->messages)[2] = new RadioMessage(2411, 0, 27, 11);
            (*this->messages)[3] = new RadioMessage(2412, 11, 6, 2);
            break;
        case 79:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2413, 0, 5, 6000);
            break;
        case 80:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(12, *this->messages);
            (*this->messages)[0] = new RadioMessage(2414, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2415, 6, 5, 25000);
            (*this->messages)[2] = new RadioMessage(2416, 26, 6, 1);
            (*this->messages)[3] = new RadioMessage(2417, 0, 6, 2);
            (*this->messages)[4] = new RadioMessage(2418, 6, 6, 3);
            (*this->messages)[5] = new RadioMessage(2419, 26, 6, 4);
            (*this->messages)[6] = new RadioMessage(2420, 6, 6, 5);
            (*this->messages)[7] = new RadioMessage(2421, 26, 6, 6);
            (*this->messages)[8] = new RadioMessage(2422, 0, 6, 7);
            (*this->messages)[9] = new RadioMessage(2423, 26, 9, 1, 18);
            (*this->messages)[10] = new RadioMessage(2424, 26, 27, 12);
            (*this->messages)[11] = new RadioMessage(2425, 26, 6, 10);
            break;
        case 81:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(4, *this->messages);
            (*this->messages)[0] = new RadioMessage(2432, 26, 5, 16000);
            (*this->messages)[1] = new RadioMessage(2433, 31, 6, 0);
            (*this->messages)[2] = new RadioMessage(2434, 26, 6, 1);
            (*this->messages)[3] = new RadioMessage(2435, 26, 6, 2);
            break;
        case 87:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(6, *this->messages);
            (*this->messages)[0] = new RadioMessage(2480, 6, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2481, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2482, 6, 6, 1);
            (*this->messages)[3] = new RadioMessage(2483, 0, 6, 2);
            (*this->messages)[4] = new RadioMessage(2484, 6, 6, 3);
            (*this->messages)[5] = new RadioMessage(2485, 0, 6, 4);
            break;
        case 89:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2494, 17, 5, 2000);
            break;
        case 91:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(7, *this->messages);
            (*this->messages)[0] = new RadioMessage(2504, 0, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2505, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2506, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(2507, 60, 6, 2);
            (*this->messages)[4] = new RadioMessage(2508, 0, 6, 3);
            (*this->messages)[5] = new RadioMessage(2509, 0, 27, 2);
            (*this->messages)[6] = new RadioMessage(2510, 60, 27, 4);
            break;
        case 92:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(10, *this->messages);
            (*this->messages)[0] = new RadioMessage(2513, 59, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2514, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2515, 59, 6, 1);
            (*this->messages)[3] = new RadioMessage(2516, 0, 6, 2);
            (*this->messages)[4] = new RadioMessage(2517, 0, 27, 2);
            (*this->messages)[5] = new RadioMessage(2518, 0, 6, 4);
            (*this->messages)[6] = new RadioMessage(2519, 0, 27, 6);
            (*this->messages)[7] = new RadioMessage(2520, 0, 6, 6);
            (*this->messages)[8] = new RadioMessage(2521, 0, 9, 0, 3);
            (*this->messages)[9] = new RadioMessage(2522, 59, 27, 10);
            break;
        case 94:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(6, *this->messages);
            (*this->messages)[0] = new RadioMessage(2541, 0, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2542, 59, 6, 0);
            (*this->messages)[2] = new RadioMessage(2543, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(2544, 0, 27, 1);
            (*this->messages)[4] = new RadioMessage(2545, 0, 6, 3);
            (*this->messages)[5] = new RadioMessage(2546, 0, 6, 4);
            break;
        case 95:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2555, 17, 27, 1);
            break;
        case 97:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(3, *this->messages);
            (*this->messages)[0] = new RadioMessage(2582, 0, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2583, 22, 6, 0);
            (*this->messages)[2] = new RadioMessage(2584, 0, 6, 1);
            break;
        case 99:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2600, 17, 27, 1);
            break;
        case 100:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(2, *this->messages);
            (*this->messages)[0] = new RadioMessage(2613, 0, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2614, 0, 6, 0);
            break;
        case 102:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(9, *this->messages);
            (*this->messages)[0] = new RadioMessage(2625, 18, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2626, 18, 6, 0);
            (*this->messages)[2] = new RadioMessage(2627, 18, 6, 1);
            (*this->messages)[3] = new RadioMessage(2628, 18, 27, 3);
            (*this->messages)[4] = new RadioMessage(2629, 0, 30, 1);
            (*this->messages)[5] = new RadioMessage(2630, 0, 30, 2);
            (*this->messages)[6] = new RadioMessage(2631, 0, 30, 3);
            (*this->messages)[7] = new RadioMessage(2632, 18, 27, 5);
            (*this->messages)[8] = new RadioMessage(2633, 0, 27, 7);
            break;
        case 105:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(9, *this->messages);
            (*this->messages)[0] = new RadioMessage(2657, 20, 5, 0);
            (*this->messages)[1] = new RadioMessage(2658, 61, 27, 2);
            (*this->messages)[2] = new RadioMessage(2659, 0, 27, 5);
            (*this->messages)[3] = new RadioMessage(2660, 20, 27, 6);
            (*this->messages)[4] = new RadioMessage(2661, 0, 6, 3);
            (*this->messages)[5] = new RadioMessage(2662, 0, 27, 7);
            (*this->messages)[6] = new RadioMessage(2663, 20, 6, 5);
            (*this->messages)[7] = new RadioMessage(2664, 0, 27, 9);
            (*this->messages)[8] = new RadioMessage(2665, 0, 27, 10);
            break;
        case 106:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(6, *this->messages);
            (*this->messages)[0] = new RadioMessage(2666, 0, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2667, 20, 6, 0);
            (*this->messages)[2] = new RadioMessage(2668, 20, 6, 1);
            (*this->messages)[3] = new RadioMessage(2669, 0, 6, 2);
            (*this->messages)[4] = new RadioMessage(2670, 0, 6, 3);
            (*this->messages)[5] = new RadioMessage(2671, 0, 27, 6);
            break;
        case 109:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2687, 17, 27, 1);
            break;
        case 114:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(3, *this->messages);
            (*this->messages)[0] = new RadioMessage(2708, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2709, 10, 27, 1);
            (*this->messages)[2] = new RadioMessage(2710, 0, 6, 1);
            break;
        case 119:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2745, 17, 27, 1);
            break;
        case 120:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2757, 0, 5, 1500);
            break;
        case 123:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(4, *this->messages);
            (*this->messages)[0] = new RadioMessage(2785, 22, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2786, 38, 6, 0);
            (*this->messages)[2] = new RadioMessage(2787, 22, 6, 1);
            (*this->messages)[3] = new RadioMessage(2788, 0, 6, 2);
            break;
        case 125:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(8, *this->messages);
            (*this->messages)[0] = new RadioMessage(2804, 0, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2814, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2815, 0, 27, 2);
            (*this->messages)[3] = new RadioMessage(2816, 10, 27, 3);
            (*this->messages)[4] = new RadioMessage(2817, 0, 27, 4);
            (*this->messages)[5] = new RadioMessage(2818, 0, 27, 5);
            (*this->messages)[6] = new RadioMessage(2820, 0, 27, 6);
            (*this->messages)[7] = new RadioMessage(2821, 0, 27, 7);
            break;
        case 126:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2828, 17, 27, 1);
            break;
        case 131:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2859, 0, 27, 2);
            break;
        case 133:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(2867, 17, 27, 1);
            break;
        case 135:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(4, *this->messages);
            (*this->messages)[0] = new RadioMessage(2883, 49, 27, 1);
            (*this->messages)[1] = new RadioMessage(2884, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2885, 0, 27, 2);
            (*this->messages)[3] = new RadioMessage(2886, 0, 27, 3);
            break;
        case 137:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(5, *this->messages);
            (*this->messages)[0] = new RadioMessage(2895, 50, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2896, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(2897, 50, 6, 1);
            (*this->messages)[3] = new RadioMessage(2898, 0, 6, 2);
            (*this->messages)[4] = new RadioMessage(2899, 50, 6, 3);
            break;
        case 139:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(6, *this->messages);
            (*this->messages)[0] = new RadioMessage(2924, 0, 5, 8000);
            (*this->messages)[1] = new RadioMessage(2925, 0, 27, 1);
            (*this->messages)[2] = new RadioMessage(2926, 50, 6, 1);
            (*this->messages)[3] = new RadioMessage(2927, 50, 6, 2);
            (*this->messages)[4] = new RadioMessage(2928, 0, 27, 2);
            (*this->messages)[5] = new RadioMessage(2929, 0, 27, 3);
            break;
        case 142:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(5, *this->messages);
            (*this->messages)[0] = new RadioMessage(2952, 2, 5, 1500);
            (*this->messages)[1] = new RadioMessage(2953, 2, 27, 1);
            (*this->messages)[2] = new RadioMessage(2954, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(2955, 2, 6, 2);
            (*this->messages)[4] = new RadioMessage(2956, 2, 27, 3);
            break;
        case 144:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(4, *this->messages);
            (*this->messages)[0] = new RadioMessage(2968, 39, 5, 7000);
            (*this->messages)[1] = new RadioMessage(2969, 39, 6, 0);
            (*this->messages)[2] = new RadioMessage(2970, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(2971, 39, 6, 2);
            break;
        case 145:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(3, *this->messages);
            (*this->messages)[0] = new RadioMessage(2972, 0, 5, 7000);
            (*this->messages)[1] = new RadioMessage(2973, 39, 6, 0);
            (*this->messages)[2] = new RadioMessage(2974, 39, 27, 5);
            break;
        case 147:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(2, *this->messages);
            (*this->messages)[0] = new RadioMessage(2988, 0, 5, 7000);
            (*this->messages)[1] = new RadioMessage(2989, 0, 6, 0);
            break;
        case 154:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(12, *this->messages);
            (*this->messages)[0] = new RadioMessage(3039, 0, 5, 9000);
            (*this->messages)[1] = new RadioMessage(3040, 26, 6, 0);
            (*this->messages)[2] = new RadioMessage(3041, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(3042, 26, 6, 2);
            (*this->messages)[4] = new RadioMessage(3043, 0, 6, 3);
            (*this->messages)[5] = new RadioMessage(3044, 26, 6, 4);
            (*this->messages)[6] = new RadioMessage(3045, 0, 6, 5);
            (*this->messages)[7] = new RadioMessage(3046, 26, 27, 9);
            (*this->messages)[8] = new RadioMessage(3047, 0, 27, 10);
            (*this->messages)[9] = new RadioMessage(3048, 55, 6, 8);
            (*this->messages)[10] = new RadioMessage(3049, 0, 6, 9);
            (*this->messages)[11] = new RadioMessage(3050, 0, 27, 11);
            break;
        case 157:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(17, *this->messages);
            (*this->messages)[0] = new RadioMessage(3077, 0, 5, 7000);
            (*this->messages)[1] = new RadioMessage(3078, 38, 6, 0);
            (*this->messages)[2] = new RadioMessage(3079, 0, 6, 1);
            (*this->messages)[3] = new RadioMessage(3080, 39, 6, 2);
            (*this->messages)[4] = new RadioMessage(3081, 1, 27, 5);
            (*this->messages)[5] = new RadioMessage(3082, 0, 6, 4);
            (*this->messages)[6] = new RadioMessage(3083, 6, 12, 21);
            (*this->messages)[7] = new RadioMessage(3084, 0, 6, 6);
            (*this->messages)[8] = new RadioMessage(3085, 6, 27, 6);
            (*this->messages)[9] = new RadioMessage(3086, 26, 6, 8);
            (*this->messages)[10] = new RadioMessage(3087, 6, 6, 9);
            (*this->messages)[11] = new RadioMessage(3088, 1, 6, 10);
            (*this->messages)[12] = new RadioMessage(3089, 39, 27, 8);
            (*this->messages)[13] = new RadioMessage(3090, 0, 27, 9);
            (*this->messages)[14] = new RadioMessage(3091, 26, 6, 13);
            (*this->messages)[15] = new RadioMessage(3092, 0, 6, 14);
            (*this->messages)[16] = new RadioMessage(3093, 26, 6, 15);
            break;
        case 158:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(11, *this->messages);
            (*this->messages)[0] = new RadioMessage(3094, 0, 5, 10000);
            (*this->messages)[1] = new RadioMessage(3095, 0, 6, 0);
            (*this->messages)[2] = new RadioMessage(3096, 39, 6, 1);
            (*this->messages)[3] = new RadioMessage(3097, 0, 6, 2);
            (*this->messages)[4] = new RadioMessage(3098, 39, 6, 3);
            (*this->messages)[5] = new RadioMessage(3099, 39, 31, 0);
            (*this->messages)[6] = new RadioMessage(3100, 0, 6, 5);
            (*this->messages)[7] = new RadioMessage(3101, 39, 12, 0);
            (*this->messages)[8] = new RadioMessage(3102, 0, 6, 7);
            (*this->messages)[9] = new RadioMessage(3103, 39, 19, 0);
            (*this->messages)[10] = new RadioMessage(3104, 0, 1, 0);
            break;
        case 160:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(3121, 17, 27, 1);
            break;
        case 161:
            this->messages = new Array<RadioMessage *>();
            ArraySetLength(1, *this->messages);
            (*this->messages)[0] = new RadioMessage(3125, 17, 27, 1);
            break;
        default:
            break;
    }
    return this->messages;
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
    flashActive = 1;
    flashType = type;
    int dur = 2000;
    if (type == 1) {
        dur = 7000;
    }
    if (type > 2) {
        dur = 10000;
    }
    flashDurationA = dur;
    flashDurationB = dur;
    switch (flashType) {
        case 3:
            flashColor.x = 0;
            flashColor.y = 0;
            flashColor.z = 0;
            player->hitCamera();
            break;
        case 2:
            flashColor.x = i_r * 1.5f;
            flashColor.y = i_g * 1.5f;
            flashColor.z = i_b * 1.5f;
            break;
        case 4:
            flashColor.x = 0;
            flashColor.y = 0;
            flashColor.z = 255.0f;
            break;
        default: {
            float k = (flashType == 1) ? 8.0f : 5.0f;
            flashColor.x = clampChannel(k * i_r);
            flashColor.y = clampChannel(k * i_g);
            flashColor.z = clampChannel(k * i_b);
            break;
        }
    }
    flashAlpha = 255.0f;
}

void Level::createPlayer() {
    Ship *ship = Globals::status->getShip();
    Array<Item *> *equipment = Globals::status->getShip()->getEquipment();
    int *freeSlots = new int[3];
    int primarySlots = freeSlots[0] = ship->getUsedSlots(0);
    int secondarySlots = freeSlots[1] = ship->getUsedSlots(1);
    int turretSlots = freeSlots[2] = ship->getUsedSlots(2);

    Player *egoPlayer = new Player(1200, Globals::status->getShip()->getMaxHP(), primarySlots, secondarySlots,
                                   turretSlots);
    egoPlayer->setMaxShieldHP(Globals::status->getShip()->getMaxShieldHP());
    egoPlayer->setMaxArmorHP(Globals::status->getShip()->getMaxArmorHP());
    egoPlayer->egoFlag = 1;
    PlayerEgo *ego = new PlayerEgo(egoPlayer);
    player = ego;
    ego->setShip(Globals::status->getShip()->getIndex(), Globals::status->getShip()->getRace());
    player->setLevel(this);
    player->geometry->setRotation(Vector{0.0f, (float) field_138 * (1.0f / 65536.0f) * 6.2831855f, 0.0f});

    Array<Array<Gun *> *> *guns = new Array<Array<Gun *> *>();
    ArraySetLength(3, *guns);
    for (unsigned int i = 0; i < 3; i++) {
        if (freeSlots[i] > 0) {
            (*guns)[i] = new Array<Gun *>();
            ArraySetLength(freeSlots[i], *(*guns)[i]);
        }
    }

    if (equipment != nullptr) {
        FileRead *fileRead = new FileRead();
        Array<Array<Vector *> *> *weaponPositions = fileRead->loadWeaponPositions(Globals::status->getShip()->getIndex());
        delete fileRead;

        if ((*weaponPositions)[3] != nullptr) {
            // engine flare anchors: position/scale pairs
            field_a4 = new Array<AEGeometry *>();
            for (unsigned int k = 0; k < (*weaponPositions)[3]->size(); k += 2) {
                ArrayAdd(new AEGeometry(Globals::Canvas), *field_a4);
                (*field_a4)[k / 2]->setPosition(*(*(*weaponPositions)[3])[k]);
                (*field_a4)[k / 2]->setScaling(*(*(*weaponPositions)[3])[k + 1]);
            }
        }

        for (unsigned int i = 0; i < equipment->size(); i++) {
            if ((*equipment)[i] == nullptr || !(*equipment)[i]->isWeapon()) {
                continue;
            }
            int ammo = (*equipment)[i]->getType() == 1 ? (*equipment)[i]->getAmount() : -1;
            int damage = (*equipment)[i]->getAttribute(9);
            int loadingTime = (*equipment)[i]->getAttribute(11);
            if ((*equipment)[i]->getType() == 0) {
                float damageFactor = Globals::status->getShip()->getDamageFactor();
                if (damage <= 9 && damageFactor < 0.0f) {
                    loadingTime = (int) (Globals::status->getShip()->getFireRateFactor() * 0.7f * (float) loadingTime);
                } else {
                    damage = (int) (damageFactor * (float) damage);
                    loadingTime = (int) (Globals::status->getShip()->getFireRateFactor() * (float) loadingTime);
                }
            }
            Gun *gun = createGun((*equipment)[i]->getIndex(), i, (*equipment)[i]->getSort(), ammo, damage, loadingTime,
                                 (*equipment)[i]->getAttribute(12), (*equipment)[i]->getAttribute(13));
            gun->itemIndex = (*equipment)[i]->getIndex();
            gun->weaponType = static_cast<ItemSort>((*equipment)[i]->getSort());
            gun->setMagnitude((*equipment)[i]->getAttribute(14));
            switch ((*equipment)[i]->getType()) {
                case 0:
                    (*(*guns)[0])[--freeSlots[0]] = gun;
                    gun->setOffset((*(*weaponPositions)[0])[ship->getSlotPos((*equipment)[i])]);
                    break;
                case 1:
                    (*(*guns)[1])[--freeSlots[1]] = gun;
                    gun->setOffset((*(*weaponPositions)[1])[ship->getSlotPos((*equipment)[i])]);
                    break;
                case 2:
                    (*(*guns)[2])[--freeSlots[2]] = gun;
                    player->setTurretPosition(*(*(*weaponPositions)[2])[0]);
                    break;
                default:
                    break;
            }
        }

        if (weaponPositions != nullptr) {
            for (unsigned int k = 0; k < weaponPositions->size(); k++) {
                if ((*weaponPositions)[k] != nullptr) {
                    ArrayReleaseClasses(*(*weaponPositions)[k]);
                    delete (*weaponPositions)[k];
                    (*weaponPositions)[k] = nullptr;
                }
            }
            ArrayReleaseClasses(*weaponPositions);
            delete weaponPositions;
        }
    }

    for (unsigned int i = 0; i < guns->size(); i++) {
        if ((*guns)[i] != nullptr) {
            player->addGun((*guns)[i], i);
        }
    }
    delete[] freeSlots;
}

void Level::wingmanDied(AbyssEngine::String const &name) {
    Array<String *> *wingmen = (Array<String *> *) (intptr_t) Globals::status->getWingmen();
    if (wingmen == nullptr) {
        return;
    }
    if (wingmen->count > 1) {
        for (unsigned int i = 0; i < wingmen->count; i++) {
            if (wingmen->data_[i]->Compare(name) == 0) {
                ArrayRemove(wingmen->data_[i], *wingmen);
                return;
            }
        }
    } else {
        Globals::status->setWingmen(nullptr);
    }
}


Level::Level(int mission)
        : playerPosition(),
          stationPosition(),
          spawnPosition() {
    field_1c = 0;
    killCountA = 0;
    killCountB = 0;
    objectivesA = nullptr;
    objectivesB = nullptr;
    field_30 = 0;
    field_90 = nullptr;
    field_94 = nullptr;
    field_98 = nullptr;
    field_9c = nullptr;
    particleRenderBoolPtr = nullptr;
    skybox2Mesh = nullptr;
    field_8c = nullptr;
    field_74 = nullptr;
    particleEmitBoolPtr = nullptr;
    particleSystemMgr = nullptr;
    field_80 = nullptr;
    skyboxMesh = -1;
    field_08 = -1;
    skyboxTexture = -1;
    field_10 = -1;
    missionPtr = mission;
    collisionVolume = nullptr;
    field_b0 = nullptr;
    flashColor.x = 0;
    flashColor.y = 0;
    flashColor.z = 0;
    flashDurationA = 0;
    flashDurationB = 0;
    flashActive = 0;
    asteroidWaypoint = nullptr;
    field_dc = 0;
    field_e0 = 0;
    playerGuns = nullptr;
    enemyGuns = nullptr;
    starSystem = nullptr;
    player = nullptr;
    gasClouds = nullptr;
    enemies = nullptr;
    asteroids = nullptr;
    landmarks = nullptr;
    field_104 = nullptr;
    playerRoute = nullptr;
    friendRoute = nullptr;
    enemyRoute = nullptr;
    messages = nullptr;
    enemiesLeft = 0;
    friendsLeft = 0;
    field_120 = 0;
    field_124 = 0;
    asteroidsLeft = 0;
    kills = 0;
    timeLimit = 0;
    field_134 = 0;
    field_138 = 0;
    friendCargoStolen = 0;
    hostileCount = 0;
    alienAttackTimer = 0;
    orbitWaveTimer = 0;
    field_178 = 0;
    flashType = 0;
    friendCount = 0;
    field_164 = 0;
    field_168 = 0;
    field_18a = 0;
    field_188 = 0;
    alarmRequested = 0;
    movingStarsIndex = -1;
    field_284 = -1;
    field_34 = -1;
    field_38 = -1;
    field_3c = -1;
    field_48 = -1;
    field_4c = -1;
    field_17c = 0;
    field_180 = nullptr;
    field_184 = 0;
    field_69 = 0;
    field_6c = 0;
    field_288 = 0;
    supernovaFlareActive = 0;
    field_a0 = nullptr;
    field_a4 = nullptr;
    field_a8 = nullptr;
    miningPlantIndex = -1;
    miningPlant = 0;
    numDeliveredOre = 0;
    numDeliveredPassengers = 0;
    field_29c = 0;
    field_29d = 0;
    field_29e = 0;
    field_1b4 = -1;
    field_1b8 = -1;
    field_1bc = -1;
    field_1c0 = -1;
}

void Level::createStaticObjects() {
    // the station-under-construction at station 0x70 grows with the campaign
    if (!Globals::status->inAlienOrbit() && Globals::status->getStation()->getIndex() == 0x70
        && Globals::status->getCurrentCampaignMission() >= 0x80) {
        int type;
        if (Globals::status->getCurrentCampaignMission() < 0x83) {
            type = 0x493e;
        } else if (Globals::status->getCurrentCampaignMission() < 0x87) {
            type = 0x4941;
        } else if (Globals::status->getCurrentCampaignMission() < 0x8a) {
            type = 0x4944;
        } else if (Globals::status->getCurrentCampaignMission() < 0x8e) {
            type = 0x4947;
        } else if (Globals::status->getCurrentCampaignMission() <= 0x91) {
            type = 0x494a;
        } else {
            type = -1;
        }
        if (type != -1) {
            KIPlayer *object = createStaticObject(nullptr, type, false);
            object->setPosition(-50000.0f, 0.0f, 50000.0f);
            ((PlayerFixedObject *) object)->setMoving(false);
            AEGeometry *geometry = object->geometry;
            object->field_0x70 = 0;
            geometry->setDirection(starSystem->getLightDirection(), Vector{0.0f, 1.0f, 0.0f});
            object->name = *Globals::gameText->getText(0xc87);
            object->player->setAlwaysFriend(true);
            if (enemies != nullptr) {
                ArrayAdd(object, *enemies);
            } else {
                enemies = new Array<KIPlayer *>();
                ArrayAdd(object, *enemies);
            }
            delete object->cargo;
            object->cargo = nullptr;
        }
    }

    if (Globals::status->getCurrentCampaignMission() >= 0x55 && Globals::status->getCurrentCampaignMission() != 0x87
        && !Globals::status->inAlienOrbit() && Globals::status->getStation()->getIndex() == 0x67) {
        KIPlayer *object = createStaticObject(nullptr, 0x4a88, false);
        object->setPosition(0.0f, 0.0f, 0.0f);
        ((PlayerFixedObject *) object)->setMoving(false);
        object->field_0x70 = 1;
        ((PlayerFixedObject *) object)->setName(*Globals::gameText->getText(0xc8a));
        ((PlayerFixedObject *) object)->setDockingType(1);
        delete object->cargo;
        object->cargo = nullptr;
        object->player->setAlwaysFriend(true);
        if (enemies != nullptr) {
            ArrayAdd(object, *enemies);
        } else {
            enemies = new Array<KIPlayer *>();
            ArrayAdd(object, *enemies);
        }
    }
}

KIPlayer *Level::createStaticObject(Waypoint *waypoint, int type, bool scatter) {
    int x, y, z;
    if (waypoint != nullptr) {
        x = waypoint->x;
        y = waypoint->y;
        z = waypoint->z;
    } else {
        x = 0;
        y = 0;
        z = 0;
    }
    if (scatter) {
        x = x + Globals::rnd->nextInt(20000) - 10000;
        y = y + Globals::rnd->nextInt(20000) - 10000;
        z = z + Globals::rnd->nextInt(20000) - 10000;
    }

    KIPlayer *object;
    if (type == 0x4215) {
        // space junk: one of three random meshes
        int variant = Globals::rnd->nextInt(3);
        int mesh = variant == 0 ? 0x4215 : (variant == 1 ? 0x4216 : 0x4217);
        AEGeometry *geometry = new AEGeometry(mesh, Globals::Canvas, false);
        Player *player = new Player(1000, 1, 0, 0, 0);
        object = new PlayerJunk(0x4215, player, geometry, x, y, z);
        object->geometry->rotate(x, y, z);
    } else {
        AEGeometry *geometry;
        if (type == 0x1a74 || type == 0x381b || type == 0x1a76) {
            geometry = new AEGeometry(Globals::Canvas);
        } else {
            geometry = new AEGeometry(type, Globals::Canvas, false);
        }

        if ((unsigned int) (type - 0x49c0) < 3 || type == 0x1a74 || type == 0x1a76 || type == 0x381b) {
            Player *player = new Player(1000, 100, 0, 0, 0);
            object = new PlayerTurret(type, player, geometry, x, y, z);
            object->setLevel(this);
            object->name = *Globals::gameText->getText(0x682);
            object->cargo = nullptr;
        } else if (type == 0x37a3) {
            int hitpoints = Globals::status->getLevel() > 20 ? 320 : Globals::status->getLevel() * 15 + 20;
            hitpoints = (hitpoints + (Globals::status->gameWon() ? 180 : Globals::status->getCurrentCampaignMission() * 4)) * 5;
            float scaled = (float) hitpoints;
            scaled = scaled + (*reinterpret_cast<float *>(&Globals::options[0x2c]) - 0.5f) * scaled;
            Player *player = new Player(7500, (int) scaled, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x37a3, 8, player, nullptr, x, y, z);
            geometry = new AEGeometry(0x37a3, Globals::Canvas, false);
            fixed->setShipGroup(geometry, -1, false);
            AEGeometry *part1 = new AEGeometry(0x37a4, Globals::Canvas, false);
            AEGeometry *part2 = new AEGeometry(0x37a5, Globals::Canvas, false);
            fixed->geometry->addChild(part1->transform);
            fixed->geometry->addChild(part2->transform);
            delete part2;
            delete part1;
            fixed->setWreckedMeshId(0x37a6);
            fixed->setBV(getBoundingVolume(1002, geometry));
            fixed->setPosition(x, y, z);
            object = fixed;
        } else if (type == 0x381d) {
            Player *player = new Player(1000, 100, 0, 0, 0);
            object = new PlayerTurret(0x381d, player, geometry, x, y, z);
            object->setLevel(this);
            object->name = *Globals::gameText->getText(0x681);
            object->field_0x25 = 0;
            object->cargo = nullptr;
        } else if (type == 0x4220) {
            Player *player = new Player(1000, 9999999, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x4220, 0, player, nullptr, x, y, z);
            geometry = new AEGeometry(0x4220, Globals::Canvas, false);
            fixed->setShipGroup(geometry, -1, false);
            AEGeometry *part1 = new AEGeometry(0x4221, Globals::Canvas, false);
            AEGeometry *part2 = new AEGeometry(0x4222, Globals::Canvas, false);
            fixed->geometry->addChild(part1->transform);
            fixed->geometry->addChild(part2->transform);
            delete part2;
            delete part1;
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(fixed->geometry->transform))->Update(
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(fixed->geometry->transform))->animationLength,
                    false);
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(fixed->geometry->childTransform))->Update(
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(fixed->geometry->childTransform))->animationLength,
                    false);
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(fixed->geometry->parentTransform))->Update(
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(fixed->geometry->parentTransform))->animationLength,
                    false);
            fixed->field_0x70 = 1;
            fixed->setWreckedMeshId(0x37a6);
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(7, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            fixed->setBV(getBoundingVolume(1003, geometry));
            fixed->setPosition(x, y, z);
            object = fixed;
        } else if (type == 0x4a88) {
            Player *player = new Player(1000, 9999999, 0, 0, 0);
            player->setVulnerable(false);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x4a88, 3, player, nullptr, x, y, z);
            AEGeometry *root = new AEGeometry(0x4a88, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry(0x4a8d, Globals::Canvas, false);
            AEGeometry *part2 = new AEGeometry(0x4a8c, Globals::Canvas, false);
            AEGeometry *part3 = new AEGeometry(0x4a89, Globals::Canvas, false);
            AEGeometry *part4 = new AEGeometry(0x4a8a, Globals::Canvas, false);
            AEGeometry *part5 = new AEGeometry(0x4a8b, Globals::Canvas, false);
            root->addChild(part1->transform);
            root->addChild(part2->transform);
            root->addChild(part3->transform);
            AEGeometry *part6 = new AEGeometry(0x4a8e, Globals::Canvas, false);
            AEGeometry *part7 = new AEGeometry(0x4a90, Globals::Canvas, false);
            AEGeometry *part8 = new AEGeometry(0x4a8f, Globals::Canvas, false);
            root->addChild(part6->transform);
            root->addChild(part7->transform);
            root->addChild(part8->transform);
            delete part6;
            delete part7;
            delete part8;
            root->addChild(part4->transform);
            root->addChild(part5->transform);
            delete part1;
            delete part2;
            delete part3;
            delete part4;
            delete part5;
            fixed->setShipGroup(root, -1, false);
            fixed->field_0x70 = 1;
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x4a88);
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(1, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            fixed->setBV(getBoundingVolume(2000, geometry));
            fixed->player->radius = 0;
            object = fixed;
        } else if (type == 0x4a6a) {
            new Player(1000, 9999999, 0, 0, 0);
            object = new PlayerStatic(0x4a6a, geometry, x, y, z);
        } else if (type == 0x493e || type == 0x4941 || type == 0x4944 || type == 0x4947 || type == 0x494a) {
            Player *player = new Player(1000, 9999999, 0, 0, 0);
            player->setVulnerable(false);
            PlayerFixedObject *fixed = new PlayerFixedObject(type, 3, player, nullptr, x, y, z);
            AEGeometry *root = new AEGeometry(type, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry(type + 2, Globals::Canvas, false);
            AEGeometry *part2 = new AEGeometry(type + 1, Globals::Canvas, false);
            root->addChild(part1->transform);
            root->addChild(part2->transform);
            if (type == 0x494a && Globals::status->getCurrentCampaignMission() == 0x91) {
                AEGeometry *part3 = new AEGeometry(type + 3, Globals::Canvas, false);
                root->addChild(part3->transform);
                delete part3;
            }
            delete part1;
            delete part2;
            fixed->setShipGroup(root, -1, false);
            fixed->field_0x70 = 1;
            fixed->setPosition(x, y, z);
            fixed->setBV(getBoundingVolume(2001, geometry));
            object = fixed;
        } else if (type == 0x494e) {
            int hitpoints = Globals::status->getLevel() > 20 ? 400 : Globals::status->getLevel() * 15 + 100;
            hitpoints = (hitpoints + (Globals::status->gameWon() ? 180 : Globals::status->getCurrentCampaignMission() * 4)) * 5;
            float scaled = (float) hitpoints;
            scaled = scaled + (*reinterpret_cast<float *>(&Globals::options[0x2c]) - 0.5f) * scaled;
            Player *player = new Player(1000, (int) scaled, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x494e, 3, player, nullptr, x, y, z);
            AEGeometry *root = new AEGeometry(0x4a80, Globals::Canvas, false);
            AEGeometry *hull = new AEGeometry(0x494e, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry(0x4a81, Globals::Canvas, false);
            AEGeometry *part2 = new AEGeometry(0x4a82, Globals::Canvas, false);
            AEGeometry *part3 = new AEGeometry(0x4a83, Globals::Canvas, false);
            AEGeometry *part4 = new AEGeometry(0x4a84, Globals::Canvas, false);
            root->addChild(hull->transform);
            hull->addChild(part1->transform);
            hull->addChild(part2->transform);
            hull->addChild(part3->transform);
            hull->addChild(part4->transform);
            delete hull;
            delete part1;
            delete part2;
            delete part3;
            delete part4;
            fixed->setShipGroup(root, -1, false);
            fixed->field_0x40 = 1;
            fixed->field_0x70 = 1;
            Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
            ArraySetLength(2, *volumes);
            (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -199.0f, 4708.0f, 980.0f, 1530.0f, 1240.0f);
            (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, -14.0f, -98.0f, 4500.0f, 1405.0f, 8860.0f);
            fixed->setBV(volumes);
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x477c);
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(3, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            object = fixed;
        } else if (type == 0x495d) {
            Player *player = new Player(1000, 9999999, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x495d, 3, player, nullptr, x, y, z);
            AEGeometry *root = new AEGeometry(0x5254, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry(0x5574, Globals::Canvas, false);
            AEGeometry *part2 = new AEGeometry(0x563c, Globals::Canvas, false);
            AEGeometry *part3 = new AEGeometry(0x495d, Globals::Canvas, false);
            AEGeometry *part4 = new AEGeometry(0x495d, Globals::Canvas, false);
            AEGeometry *part5 = new AEGeometry(0x495e, Globals::Canvas, false);
            AEGeometry *part6 = new AEGeometry(0x495f, Globals::Canvas, false);
            AEGeometry *part7 = new AEGeometry(0x4960, Globals::Canvas, false);
            root->addChild(part7->transform);
            root->addChild(part1->transform);
            root->addChild(part2->transform);
            root->addChild(part3->transform);
            root->addChild(part4->transform);
            root->addChild(part5->transform);
            root->addChild(part6->transform);
            delete part1;
            delete part2;
            delete part3;
            delete part4;
            delete part5;
            delete part6;
            delete part7;
            fixed->setShipGroup(root, -1, false);
            fixed->field_0x70 = 1;
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x477c);
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(6, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            fixed->setBV(getBoundingVolume(111, geometry));
            object = fixed;
        } else if (type == 0x4260) {
            Player *player = new Player(1000, 9999999, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x4260, 0, player, nullptr, x, y, z);
            geometry = new AEGeometry(0x4260, Globals::Canvas, false);
            fixed->setShipGroup(geometry, -1, false);
            fixed->field_0x70 = 1;
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x477c);
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(2, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            object = fixed;
        } else if (type == 0x4299) {
            int hitpoints = Globals::status->getLevel() > 20 ? 400 : Globals::status->getLevel() * 15 + 100;
            hitpoints = (hitpoints + (Globals::status->gameWon() ? 180 : Globals::status->getCurrentCampaignMission() * 4)) * 5;
            float scaled = (float) hitpoints;
            scaled = scaled + (*reinterpret_cast<float *>(&Globals::options[0x2c]) - 0.5f) * scaled;
            Player *player = new Player(1000, (int) scaled, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x4299, 3, player, nullptr, x, y, z);
            AEGeometry *root = new AEGeometry(0x4299, Globals::Canvas, false);
            unsigned int core = -1;
            Globals::Canvas->TransformCreate(core);
            Globals::Canvas->TransformAddMesh(core, 0x429f, false);
            root->addChild(core);
            unsigned int firstRing = -1;
            unsigned int firstLights = -1;
            for (int i = 0; i < 3; i++) {
                unsigned int ring = -1;
                unsigned int lights = -1;
                Globals::Canvas->TransformCreate(ring);
                Globals::Canvas->TransformCreate(lights);
                Globals::Canvas->TransformAddMesh(ring, 0x429c, false);
                Globals::Canvas->TransformAddMesh(lights, 0x429d, false);
                float offset = i == 0 ? -2150.0f : (i == 1 ? 2150.0f : 4300.0f);
                AbyssEngine::AEMath::MatrixSetTranslation(
                        *(Matrix *) Globals::Canvas->TransformGetLocal(ring), 0.0f, 0.0f, offset);
                AbyssEngine::AEMath::MatrixSetTranslation(
                        *(Matrix *) Globals::Canvas->TransformGetLocal(lights), 0.0f, 0.0f, offset);
                if (firstRing == (unsigned int) -1) {
                    firstRing = ring;
                    firstLights = lights;
                } else {
                    Globals::Canvas->TransformAddChild(firstRing, ring);
                    Globals::Canvas->TransformAddChild(firstLights, lights);
                }
            }
            root->addChild(firstRing);
            unsigned int dock = -1;
            Globals::Canvas->TransformCreate(dock);
            Globals::Canvas->TransformAddMesh(dock, 0x429e, false);
            root->addChild(dock);
            unsigned short lodMeshes[1] = {0x429a};
            int lodDistances[1] = {35000};
            root->setLodMeshes(lodMeshes, lodDistances, 1);
            root->setLodChildTransform(firstLights);
            fixed->setShipGroup(root, -1, false);
            fixed->field_0x40 = 1;
            fixed->field_0x70 = 1;
            Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
            ArraySetLength(2, *volumes);
            (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -199.0f, 4708.0f, 980.0f, 1530.0f, 1240.0f);
            (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, -14.0f, -98.0f, 4500.0f, 1405.0f, 8860.0f);
            fixed->setBV(volumes);
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x477c);
            fixed->shipGroup = 3;
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(4, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            object = fixed;
        } else if (type == 0x4961) {
            Player *player = new Player(1000, 9999999, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x4961, 2, player, nullptr, x, y, z);
            geometry = new AEGeometry(0x4961, Globals::Canvas, false);
            AEGeometry *part = new AEGeometry(0x498f, Globals::Canvas, false);
            geometry->addChild(part->transform);
            delete part;
            fixed->setShipGroup(geometry, -1, false);
            fixed->field_0x70 = 1;
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x477c);
            fixed->shipGroup = 2;
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(2, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            fixed->setBV(getBoundingVolume(2003, geometry));
            object = fixed;
        } else if (type == 0x4962) {
            Player *player = new Player(0, 9999999, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x4962, 2, player, nullptr, x, y, z);
            geometry = new AEGeometry(0x4962, Globals::Canvas, false);
            fixed->setShipGroup(geometry, -1, false);
            fixed->field_0x70 = 0;
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x477c);
            object = fixed;
        } else if (type == 0x4974) {
            int hitpoints = Globals::status->getLevel() > 20 ? 400 : Globals::status->getLevel() * 15 + 100;
            hitpoints = (hitpoints + (Globals::status->gameWon() ? 180 : Globals::status->getCurrentCampaignMission() * 4)) * 5;
            float scaled = (float) hitpoints;
            scaled = scaled + (*reinterpret_cast<float *>(&Globals::options[0x2c]) - 0.5f) * scaled;
            Player *player = new Player(0, (int) scaled, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x4974, 0, player, nullptr, x, y, z);
            AEGeometry *root = new AEGeometry(0x4974, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry(0x4975, Globals::Canvas, false);
            AEGeometry *part2 = new AEGeometry(0x4977, Globals::Canvas, false);
            AEGeometry *part3 = new AEGeometry(0x4978, Globals::Canvas, false);
            AEGeometry *part4 = new AEGeometry(0x4979, Globals::Canvas, false);
            AEGeometry *part5 = new AEGeometry(0x497a, Globals::Canvas, false);
            AEGeometry *part6 = new AEGeometry(0x4976, Globals::Canvas, false);
            root->addChild(part1->transform);
            root->addChild(part2->transform);
            root->addChild(part3->transform);
            root->addChild(part4->transform);
            root->addChild(part5->transform);
            root->addChild(part6->transform);
            delete part1;
            delete part2;
            delete part3;
            delete part4;
            delete part5;
            delete part6;
            fixed->setShipGroup(root, -1, false);
            fixed->field_0x70 = 1;
            fixed->setWreckedMeshId(0x494e);
            fixed->setPosition(x, y, z);
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(5, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            fixed->setBV(getBoundingVolume(2005, geometry));
            object = fixed;
        } else if (type == 0x4a6b) {
            Player *player = new Player(0, 9999999, 0, 0, 0);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x4a6b, 1, player, nullptr, x, y, z);
            geometry = new AEGeometry(0x4a6b, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry(0x4a6c, Globals::Canvas, false);
            geometry->addChild(part1->transform);
            delete part1;
            AEGeometry *part2 = new AEGeometry(0x4a6d, Globals::Canvas, false);
            geometry->addChild(part2->transform);
            delete part2;
            fixed->setShipGroup(geometry, -1, false);
            fixed->field_0x70 = 1;
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x477c);
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(8, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            fixed->setBV(getBoundingVolume(2006, geometry));
            object = fixed;
        } else if (type == 0x5279) {
            Player *player = new Player(0, 9999999, 0, 0, 0);
            player->setVulnerable(false);
            PlayerFixedObject *fixed = new PlayerFixedObject(0x41a0, 1, player, nullptr, x, y, z);
            geometry = new AEGeometry(0x41a0, Globals::Canvas, false);
            fixed->setShipGroup(geometry, -1, false);
            fixed->field_0x70 = 1;
            fixed->setPosition(x, y, z);
            fixed->setWreckedMeshId(0x5279);
            FileRead *fileRead = new FileRead();
            Array<SpacePoint *> *spacePoints = fileRead->loadSpacePoints(10, -1);
            delete fileRead;
            fixed->setSpacePoints(spacePoints);
            fixed->setBV(getBoundingVolume(2002, geometry));
            object = fixed;
        } else {
            object = nullptr;
        }
    }
    object->setLevel(this);
    return object;
}

Array<BoundingVolume *> *Level::getBoundingVolume(int id, AEGeometry * /*geometry*/) {
    FileRead *fileRead = new FileRead();
    Array<int> *collision;
    if (id >= 2000) {
        collision = fileRead->loadStaticCollision(id);
    } else {
        collision = fileRead->loadStationCollision(id);
    }
    delete fileRead;

    Array<BoundingVolume *> *volumes = nullptr;
    if (collision != nullptr) {
        int count = (*collision)[0];
        Vector size = {0.0f, 0.0f, 0.0f};
        volumes = new Array<BoundingVolume *>();
        ArraySetLength(count, *volumes);
        for (int i = 0, cursor = 1; i < count; i++) {
            switch ((*collision)[cursor++]) {
                case 0: {
                    Vector pos = {(float) -(*collision)[cursor], (float) (*collision)[cursor + 1],
                                  (float) (*collision)[cursor + 2]};
                    size.x = (float) (*collision)[cursor + 3];
                    cursor += 4;
                    size *= 0.6f;
                    if (size.x < 0.0f) {
                        size *= -1.0f;
                    }
                    Vector center = {-pos.x, pos.z, -pos.y};
                    (*volumes)[i] = new BoundingSphere(0.0f, 0.0f, 0.0f, center.x, center.y, center.z, size.x);
                    break;
                }
                case 1: {
                    Vector pos = {(float) -(*collision)[cursor], (float) (*collision)[cursor + 1],
                                  (float) (*collision)[cursor + 2]};
                    size.x = (float) (*collision)[cursor + 3];
                    size.z = (float) (*collision)[cursor + 4];
                    size.y = (float) (*collision)[cursor + 5];
                    cursor += 6;
                    size *= 1.2f;
                    Vector center = {-pos.x, pos.z, -pos.y};
                    (*volumes)[i] = new BoundingAAB(0.0f, 0.0f, 0.0f, center.x, center.y, center.z,
                                                    size.x * 2.0f, size.y * 2.0f, size.z * 2.0f);
                    break;
                }
                default:
                    break;
            }
        }
        ArrayRelease(*collision);
        delete collision;
    }
    return volumes;
}

PlayerFixedObject *Level::createShip(int race, int shipClass, int type, Waypoint *waypoint, bool hostile, bool group) {
    int mission = Globals::status->getCurrentCampaignMission();
    int wx, wy, wz;
    if (waypoint != nullptr) {
        wx = waypoint->x;
        wy = waypoint->y;
        wz = waypoint->z;
    } else {
        wy = 0;
        wx = 0;
        wz = 0;
    }
    int jitterX = Globals::rnd->nextInt(40000);
    int jitterY = Globals::rnd->nextInt(40000);
    int jitterZ = Globals::rnd->nextInt(40000);

    int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
    int hitpoints = (Globals::status->gameWon() ? 180 : mission * 4) + level * 14 + 20;
    if (type == 0x2c) {
        hitpoints = (int) ((float) hitpoints * 2.25f);
    } else if (type == 0x33) {
        hitpoints = (int) ((float) hitpoints * 1.7f);
    } else if (type == 0x31) {
        hitpoints = (int) ((float) hitpoints * 17.0f);
    }
    if (mission == 49 || mission == 50 || mission == 51 || mission == 52 || mission == 56) {
        hitpoints = 270;
    }
    int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
    int empTime;
    if (shipClass == 1) {
        hitpoints *= type == 14 ? 25 : 5;
        empPoints *= 3;
        empTime = 45000;
    } else {
        empTime = 15000;
    }
    hitpoints = (int) ((float) hitpoints + (*reinterpret_cast<float *>(&Globals::options[0x2c]) - 0.5f) * (float) hitpoints);
    if (mission == 0x9a) {
        hitpoints <<= Globals::status->inAlienOrbit() & (race == 9);
    }
    int armor = Globals::status->hardCoreMode() ? 650 : 1000;
    Player *player = new Player(armor, hitpoints, 1, 1, 0);
    int x = wx + jitterX - 20000;
    int y = wy + jitterY - 20000;
    int z = wz + jitterZ - 20000;
    player->setEmpData(empPoints, empTime);

    KIPlayer *ship = nullptr;
    if (shipClass == 0) {
        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
        fighter->setShipGroup(Globals::globals->getShipGroup(type, race, group), type, hostile);
        if (missionPtr != 1 && missionPtr != 23) {
            KIPlayer *kiFighter = fighter;
            lodManager->addObject(kiFighter->parentGeometry != nullptr ? kiFighter->parentGeometry : kiFighter->geometry);
        }
        if (type == 0x2c) {
            delete fighter->cargo;
            fighter->cargo = nullptr;
        } else if (type == 0x33) {
            fighter->field_0x25 = 0;
        } else if (type == 0x31) {
            delete fighter->cargo;
            fighter->cargo = nullptr;
        }
        ship = fighter;
    } else if (shipClass == 1) {
        // capital ships: hull collision boxes per race
        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
        Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
        switch (race) {
            case 0:
                if (type == 14) {
                    ArraySetLength(11, *volumes);
                    (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -2240.0f, 21608.0f, 760.0f, 640.0f, 2890.0f);
                    (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, -2342.0f, 18392.0f, 1260.0f, 3560.0f, 3670.0f);
                    (*volumes)[2] = new BoundingAAB(x, y, z, 0.0f, -786.0f, 8926.0f, 7410.0f, 7740.0f, 15540.0f);
                    (*volumes)[3] = new BoundingAAB(x, y, z, 0.0f, -4950.0f, 9656.0f, 820.0f, 1040.0f, 7150.0f);
                    (*volumes)[4] = new BoundingAAB(x, y, z, 0.0f, 3420.0f, -452.0f, 1310.0f, 940.0f, 4880.0f);
                    (*volumes)[5] = new BoundingAAB(x, y, z, 0.0f, -270.0f, -9018.0f, 7520.0f, 6870.0f, 20470.0f);
                    (*volumes)[6] = new BoundingAAB(x, y, z, 0.0f, -5088.0f, -7962.0f, 1300.0f, 2900.0f, 13210.0f);
                    (*volumes)[7] = new BoundingAAB(x, y, z, 0.0f, 3526.0f, -11604.0f, 3260.0f, 880.0f, 10640.0f);
                    (*volumes)[8] = new BoundingAAB(x, y, z, 0.0f, 5212.0f, -12332.0f, 3260.0f, 2640.0f, 8410.0f);
                    (*volumes)[9] = new BoundingAAB(x, y, z, 0.0f, 2004.0f, -15852.0f, 15860.0f, 2510.0f, 10910.0f);
                    (*volumes)[10] = new BoundingAAB(x, y, z, 0.0f, -2610.0f, -15852.0f, 15860.0f, 2510.0f, 10910.0f);
                    fixed->setWreckedMeshId(0x4780);
                } else {
                    ArraySetLength(3, *volumes);
                    (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -73.0f, 123.0f, 3000.0f, 2860.0f, 10430.0f);
                    (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, -280.0f, -4257.0f, 3470.0f, 1815.0f, 2200.0f);
                    (*volumes)[2] = new BoundingAAB(x, y, z, 0.0f, -770.0f, -4279.0f, 5220.0f, 680.0f, 2270.0f);
                    fixed->setWreckedMeshId(0x477e);
                }
                break;
            case 1:
                ArraySetLength(5, *volumes);
                (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, 351.0f, 5827.0f, 3930.0f, 1590.0f, 2070.0f);
                (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, 616.0f, 4573.0f, 4440.0f, 2120.0f, 1035.0f);
                (*volumes)[2] = new BoundingAAB(x, y, z, 0.0f, 780.0f, 3303.0f, 4970.0f, 2620.0f, 1895.0f);
                (*volumes)[3] = new BoundingAAB(x, y, z, 0.0f, 456.0f, -309.0f, 2695.0f, 3530.0f, 6600.0f);
                (*volumes)[4] = new BoundingAAB(x, y, z, 0.0f, 248.0f, -5587.0f, 3325.0f, 3020.0f, 4205.0f);
                fixed->setWreckedMeshId(0x477f);
                break;
            case 2:
                ArraySetLength(3, *volumes);
                (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -85.0f, 24.0f, 4335.0f, 1245.0f, 10880.0f);
                (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, 710.0f, 292.0f, 2990.0f, 935.0f, 11450.0f);
                (*volumes)[2] = new BoundingAAB(x, y, z, 0.0f, 1510.0f, -2886.0f, 2750.0f, 1010.0f, 2750.0f);
                fixed->setWreckedMeshId(0x477d);
                break;
            case 3:
                ArraySetLength(2, *volumes);
                (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -199.0f, 4708.0f, 980.0f, 1530.0f, 1240.0f);
                (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, -14.0f, -98.0f, 4500.0f, 1405.0f, 8860.0f);
                fixed->setWreckedMeshId(0x477c);
                break;
            default:
                break;
        }
        fixed->setBV(volumes);
        fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), type, false);
        lodManager->addObject(fixed->geometry);
        fixed->field_0x40 = 1;
        ship = fixed;
    }
    ship->setLevel(this);
    return (PlayerFixedObject *) ship;
}

void Level::almostKillWanted(int index) {
    if (field_29e != 0) {
        return;
    }
    field_29e = 1;
    if (Globals::status->isStorylineWanted(index) == 0) {
        return;
    }
    Mission *mission = new Mission(4, 0, Globals::status->getStation()->getIndex());
    mission->setCampaignMission(true);
    mission->setWon(1);
    Globals::status->setMission(mission);
    Globals::status->setCampaignMission(mission);
    delete objectivesA;
    objectivesA = nullptr;
    objectivesA = new Objective(3, 0, 0, this);
    (*enemies)[0]->player->setAlwaysEnemy(false);
    (*enemies)[0]->player->resetDamageDoneByPlayer();
    (*enemies)[0]->player->enemyFlagsLo = 0;
    (*enemies)[0]->reviveLockFlag = 1;
    (*Globals::status->getWanted())[index]->setActive(false);
}

void Level::assignGuns() {
    if (enemyGuns != nullptr) {
        ArrayReleaseClasses(*enemyGuns);
        delete enemyGuns;
        enemyGuns = nullptr;
    }
    enemyGuns = nullptr;

    float level = ((Globals::status->getLevel() - 2) * 0.9f > 20.0f ? 20.0f
                                                                     : (Globals::status->getLevel() - 2) * 0.9f) < 0.0f
                      ? 0.0f
                      : ((Globals::status->getLevel() - 2) * 0.9f > 20.0f
                             ? 20
                             : (int) ((Globals::status->getLevel() - 2) * 0.9f));
    level += level * (*(float *) &Globals::options[44] - 0.5f);
    int campaignMission = Globals::status->getCurrentCampaignMission();
    Wanted *wanted = Globals::status->getWantedInCurrentOrbit();
    int power = (int) level;
    if (power >= 22) {
        power = 22;
    }
    if (enemies == nullptr) {
        return;
    }

    unsigned int slots = 0;
    for (unsigned int i = 0; i < enemies->size(); i++) {
        if ((*enemies)[i] != nullptr && (*enemies)[i]->field_0x25) {
            slots += (*enemies)[i]->isWingMan() ? 2 : 1;
        }
    }
    enemyGuns = new Array<AbstractGun *>();
    ArraySetLength(slots, *enemyGuns);

    int boostedBase = power == 0 ? 3 : power + 2;
    int baseDamage = boostedBase;
    if (campaignMission == 4) {
        baseDamage = 1;
    }

    unsigned int gunIndex = 0;
    for (unsigned int i = 0; i < enemies->size(); i++) {
        if ((*enemies)[i] == nullptr) {
            continue;
        }
        if ((*enemies)[i]->field_0x25) {
            if (missionPtr == 2) {
                (*enemies)[i]->player->setPlayShootSound(false, 2);
            }
            float color;
            int damage;
            if ((Globals::status->getMission()->getType() == 6 && !(*enemies)[i]->player->isAlwaysFriend())
                || (Globals::status->getMission()->getType() == 12 && (*enemies)[i]->player->isAlwaysFriend())) {
                color = 28.0f;
                damage = Globals::status->getLevel() + boostedBase;
            } else {
                color = 16.0f;
                damage = baseDamage;
            }
            if (!(*enemies)[i]->isWingMan() && !(*enemies)[i]->player->isAlwaysFriend()
                && (*enemies)[i]->shipGroup == 9) {
                if (campaignMission == 16) {
                    damage = (int) (damage * 2.0f);
                } else {
                    damage = (int) (damage * 0.8f);
                }
            }
            if (campaignMission == 49 || campaignMission == 50 || campaignMission == 51 || campaignMission == 52
                || campaignMission == 56) {
                damage = 5;
            }
            if (campaignMission == 80 && (*enemies)[i]->field_0x3e) {
                damage = (int) (damage * 1.7f);
            }
            if (Globals::status->getCurrentCampaignMission() == 70 && !(*enemies)[i]->isWingMan()) {
                damage = (int) (damage * 2.5f);
            }
            if (Globals::status->getMission() != nullptr && Globals::status->getMission()->getType() == 183) {
                damage = 1;
            }

            Gun *gun = new Gun(0, damage, 4, -1, 3000,
                               600 - 2 * (Globals::status->gameWon() ? 45
                                                                     : Globals::status->getCurrentCampaignMission()),
                               color, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setFriendGun(true);
            gun->setLevel(this);
            gun->setIndex(0);
            gun->weaponType = ITEM_SORT_LASER;

            int mesh;
            switch ((*enemies)[i]->shipGroup) {
                case 0:
                    gun->weaponType = ITEM_SORT_LASER;
                    gun->setIndex(0);
                    mesh = 6754;
                    break;
                case 1:
                    gun->setIndex(3);
                    mesh = 6760;
                    break;
                case 2:
                    gun->weaponType = ITEM_SORT_LASER;
                    gun->setIndex(7);
                    mesh = 6764;
                    break;
                case 3:
                    gun->setIndex(25);
                    mesh = 6802;
                    break;
                case 9:
                    gun->setIndex(5);
                    mesh = 6762;
                    break;
                case 10:
                    gun->setIndex(229);
                    gun->damage = (int) (gun->damage * 0.7f);
                    mesh = 19091;
                    break;
                default:
                    gun->weaponType = ITEM_SORT_BLASTER;
                    gun->setIndex(19);
                    mesh = 6795;
                    break;
            }

            int mission = Globals::status->getCurrentCampaignMission();
            if ((*enemies)[i]->field_0x3e) {
                KIPlayer *host = ((PlayerTurret *) (*enemies)[i])->getHost();
                if (host != nullptr && (host->shipGroupFlag == 45 || host->shipGroupFlag == 51)) {
                    gun->weaponType = ITEM_SORT_AUTO_CANNON;
                    gun->setIndex(22);
                    gun->damage = (int) (gun->damage * 0.5);
                    mesh = 6798;
                } else if ((*enemies)[i]->field_0x3f_b) {
                    int type = (*enemies)[i]->getType();
                    int item;
                    switch (type) {
                        case 18880:
                            gun->weaponType = ITEM_SORT_LASER;
                            gun->setIndex(2);
                            gun->offset = Vector{0.0f, 0.0f, 250.0f};
                            item = 211;
                            mesh = 6756;
                            break;
                        case 18881:
                            gun->weaponType = ITEM_SORT_BLASTER;
                            gun->setIndex(20);
                            gun->offset = Vector{0.0f, 0.0f, 250.0f};
                            item = 212;
                            mesh = 6797;
                            break;
                        default:
                            gun->weaponType = ITEM_SORT_BLASTER;
                            gun->setIndex(14);
                            gun->offset = Vector{0.0f, 0.0f, 300.0f};
                            item = 213;
                            mesh = 6790;
                            break;
                    }
                    gun->field_0xa8 = 1;
                    gun->damage = (*Globals::items)[item]->getAttribute(9);
                    gun->fireDelay = (*Globals::items)[item]->getAttribute(11);
                    gun->initialLifetime = (*Globals::items)[item]->getAttribute(12);
                    gun->pitchRate = (float) (*Globals::items)[item]->getAttribute(13);
                    if (mission == 158 && type == 18882 && (*enemies)[i]->player->isAlwaysEnemy()) {
                        gun->pitchRate *= 1.2f;
                        gun->damage = (int) (gun->damage * 1.5f);
                        Player *player = (*enemies)[i]->player;
                        player->setMaxHitpoints((int) (player->getMaxHitpoints() * 5.0f));
                    }
                } else {
                    gun->weaponType = ITEM_SORT_BLASTER;
                    if ((*enemies)[i]->shipGroup == 1) {
                        gun->setIndex(15);
                        mesh = 6791;
                    } else {
                        gun->setIndex(20);
                        mesh = 6796;
                    }
                }
            }
            if (mission == 70) {
                if (!(*enemies)[i]->isWingMan()) {
                    gun->setIndex(183);
                    mesh = 14297;
                }
            } else if (mission == 7 && (*enemies)[i]->shipGroup == 8) {
                gun->damage = (int) (gun->damage * 0.5f);
            }

            if (Globals::status->getMission() != nullptr && Globals::status->getMission()->isCampaignMission()) {
                if (Globals::lastCampaignMissionFailed == Globals::status->getCurrentCampaignMission()
                    && Globals::lastCampaignMissionFailCount >= 3 && (*enemies)[i]->isEnemy()) {
                    gun->damage = (int) (gun->damage * 0.7f);
                }
            } else {
                int group = (*enemies)[i]->shipGroupFlag;
                if (mission == 145 && group == 49) {
                    gun->weaponType = ITEM_SORT_CLUSTER_MISSILE;
                    gun->setIndex(214);
                    gun->damage *= 2;
                    mesh = 14240;
                } else if ((mission == 157 || mission == 158) && group == 49) {
                    gun->weaponType = ITEM_SORT_LASER;
                    gun->setIndex(7);
                    gun->damage *= 3;
                    mesh = 6764;
                } else if (wanted != nullptr && (*enemies)[i]->field_0x42) {
                    int weapon = wanted->getWeapon();
                    gun->setIndex(weapon);
                    int sort = (*Globals::items)[weapon]->getAttribute(2);
                    mesh = g_cg_meshTable[weapon];
                    gun->weaponType = sort;
                    gun->damage *= 4;
                }
            }

            int sort = gun->weaponType;
            if (sort == ITEM_SORT_CLUSTER_MISSILE || sort == ITEM_SORT_MISSILE) {
                RocketGun *rocketGun = new RocketGun(gun->itemIndex, gun, mesh, 0, 0, sort,
                                                     sort == ITEM_SORT_MISSILE, this);
                (*enemyGuns)[gunIndex] = rocketGun;
                gun->pitchRate = 8.0f;
                gun->initialLifetime = 10000;
                gun->fireDelay = 3000;
                gun->damage *= 4;
            } else {
                (*enemyGuns)[gunIndex] = new ObjectGun(0, gun, mesh, 10001, this);
            }
            (*enemies)[i]->addGun(gun, 0);
            Globals::globals->addSoundResourceToList((*enemies)[i]->shipGroup == 9 ? 62 : 61);

            if (wanted != nullptr && (*enemies)[i]->field_0x42
                && (*enemies)[i]->shipGroupFlag >= 45 && (*enemies)[i]->shipGroupFlag <= 48) {
                Gun *rocket = new Gun(0, damage * 4, 4, -1, 10000, 3000, 8.0f,
                                      Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
                rocket->setFriendGun(true);
                rocket->setLevel(this);
                rocket->weaponType = ITEM_SORT_ROCKET;
                rocket->setIndex(31);
                ArrayAdd((AbstractGun *) new RocketGun(rocket->itemIndex, rocket, 14240, 0, 0, rocket->weaponType,
                                                       false, this), *enemyGuns);
                (*enemies)[i]->addGun(rocket, 1);
                Globals::globals->addSoundResourceToList(84);
            }

            if ((mission == 157 || mission == 158) && (*enemies)[i]->shipGroupFlag == 49) {
                Gun *cluster = new Gun(0, damage, 4, -1, 3000,
                                       600 - 2 * (Globals::status->gameWon()
                                                      ? 45
                                                      : Globals::status->getCurrentCampaignMission()),
                                       color, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
                cluster->setFriendGun(true);
                cluster->setLevel(this);
                cluster->weaponType = ITEM_SORT_CLUSTER_MISSILE;
                cluster->setIndex(214);
                ArrayAdd((AbstractGun *) new RocketGun(cluster->itemIndex, cluster, 14240, 0, 0, cluster->weaponType,
                                                       cluster->weaponType == ITEM_SORT_MISSILE, this),
                         *enemyGuns);
                cluster->pitchRate = 8.0f;
                cluster->initialLifetime = 10000;
                cluster->fireDelay = 3000;
                cluster->damage *= 4;
                (*enemies)[i]->addGun(cluster, 1);
                Globals::globals->addSoundResourceToList(84);
            }
            gunIndex++;
        }

        if ((*enemies)[i] != nullptr && (*enemies)[i]->isWingMan() && (*enemies)[i]->field_0x25) {
            Gun *gun = new Gun(18, 0, 4, -1, 3000, 400, 16.0f, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, 0.0f});
            gun->setFriendGun(true);
            gun->setLevel(this);
            gun->itemIndex = 18;
            gun->weaponType = ITEM_SORT_BLASTER;
            (*enemyGuns)[gunIndex] = new ObjectGun(18, gun, 6794, 10001, this);
            gun->setIndex(18);
            gun->empDamage = (*Globals::items)[18]->getAttribute(10);
            (*enemies)[i]->addGun(gun, 1);
            Globals::globals->addSoundResourceToList(74);
            gunIndex++;
        }
    }
}

void Level::createGasClouds() {
    int *plasma = Globals::galaxy->getPlasmaProbabilities(Globals::status->getStation());
    if (Globals::status->getShip()->getFirstEquipmentOfSort(0x21) == nullptr || Globals::status->inAlienOrbit()) {
        return;
    }
    if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() != 10 && plasma[0] == 0xcc
        && ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRoutes() == nullptr) {
        return;
    }

    Vector position = {0.0f, 0.0f, 0.0f};
    gasClouds = new Array<KIPlayer *>();
    bool bossOrbit = Globals::status->getCurrentCampaignMission() == 0x8e
                     && Globals::status->getStation()->getIndex() == 0x4f;
    ArraySetLength((unsigned int) ((bossOrbit ? 3.0f : 0.0f)
                                   + (float) plasma[1] / 100.0f * (float) (Globals::rnd->nextInt(4) + 4)),
                   *gasClouds);
    for (unsigned int i = 0; i < gasClouds->size(); i++) {
        int kind = plasma[0];
        do {
            position.x = (float) (Globals::rnd->nextInt(160000) - 80000);
            position.y = (float) (Globals::rnd->nextInt(160000) - 80000);
            position.z = (float) (Globals::rnd->nextInt(160000) - 80000);
        } while (AbyssEngine::AEMath::VectorLength(position) <= 25000.0f
                 || AbyssEngine::AEMath::VectorLength(position - reinterpret_cast<Vector &>(stationPosition.x)) <= 30000.0f);
        if (i == 0 && bossOrbit) {
            position.y = 0.0f;
            position.x = 92000.0f;
            position.z = 36000.0f;
        }
        AEGeometry *geometry = new AEGeometry(0x37d1, Globals::Canvas, false);
        (*gasClouds)[i] = new PlayerGasCloud(kind, field_94, geometry, position);
        (*gasClouds)[i]->setLevel(this);
    }
}

void Level::updateMissionOrbit(int dt) {
    if (field_288 && !Globals::status->getMission()->isEmpty()) {
        orbitWaveTimer += dt;
        if (orbitWaveTimer > 22500) {
            orbitWaveTimer = 0;
            int coreAlive = 0;
            for (int j = 0; j != 4; j++) {
                coreAlive += (*enemies)[j]->isDead() ^ 1;
            }
            if (coreAlive != 0) {
                for (unsigned int i = 4; i < enemies->size(); i++) {
                    KIPlayer *ship = (*enemies)[i];
                    if (ship->isDead() && !ship->player->isActive()) {
                        ship->revive();
                        Vector position = player->getPosition();
                        position.x += (float) ((Globals::rnd->nextInt(40000) + 40000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                        position.y += (float) ((Globals::rnd->nextInt(5000) + 5000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                        position.z += (float) ((Globals::rnd->nextInt(40000) + 40000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                        ship->setPosition(position.x, position.y, position.z);
                    }
                }
            }
        }
    }

    if (Globals::status->getMission() != nullptr && Globals::status->getMission()->getType() == 0xb7) {
        orbitWaveTimer += dt;
        if (orbitWaveTimer > 7500) {
            orbitWaveTimer = 0;
            for (unsigned int i = 0; i < enemies->size(); i++) {
                KIPlayer *ship = (*enemies)[i];
                if (ship->isDead() && !ship->player->isActive() && ship->shipGroupFlag != 0x33) {
                    ship->revive();
                    Vector position = player->getPosition();
                    position.x += (float) ((Globals::rnd->nextInt(30000) + 50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                    position.y += (float) ((Globals::rnd->nextInt(10000) + 10000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                    position.z += (float) ((Globals::rnd->nextInt(30000) + 50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                    ship->setPosition(position.x, position.y, position.z);
                    ship->cargo = nullptr;
                }
            }
        }
    }

    if (!Globals::status->getMission()->isEmpty() && Globals::status->getMission()->getType() == 15) {
        orbitWaveTimer += dt;
        if (orbitWaveTimer > 50000) {
            orbitWaveTimer = 0;
            bool allDead = true;
            for (unsigned int i = 0; allDead && i < enemies->size() - 1; i++) {
                allDead = (*enemies)[i]->isDead();
            }
            if (allDead) {
                for (unsigned int i = 0; i < enemies->size() - 1; i++) {
                    KIPlayer *ship = (*enemies)[i];
                    if (ship->isDead() && !ship->player->isActive()) {
                        ship->revive();
                        Vector position = player->getPosition();
                        position.x += (float) ((Globals::rnd->nextInt(40000) + 40000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                        position.y += (float) ((Globals::rnd->nextInt(5000) + 5000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                        position.z += (float) ((Globals::rnd->nextInt(40000) + 40000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1));
                        ship->setPosition(position.x, position.y, position.z);
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
        for (int i = 0;
             i < (*Globals::status->wanted)[index]->getNumWingmen();
             i = i + 1) {
            (*enemies)[i + 1]->player->setAlwaysEnemy(1);
            (*enemies)[i + 1]->player->turnEnemy();
        }
    }
}

void Level::initParticleSystems() {
    static const uint32_t g_ips_skyColors[16] = {
        0x00a020ff, 0x8c1402ff, 0x006d7bff, 0x2c9bd8ff,
        0x7a7a7aff, 0xce5a46ff, 0x69a07dff, 0xa7a07eff,
        0x7eb59fff, 0xcedee1ff, 0x9274d4ff, 0xdb6923ff,
        0xffffffff, 0x9ae4ffff, 0xaeb77dff, 0xdb6923ff,
    };
    static const uint32_t g_ips_shipTypeMap[32] = {
        3, 0, 8, 3, 2, 0, 3, 0, 9, 1, 0, 8, 2, 0, 0, 0,
        2, 0, 2, 3, 3, 2, 0, 8, 8, 8, 0, 0, 0, 8, 3, 2,
    };
    static const float g_ips_vel_78[9] = {
        0.376953125f, 0.12890625f, 0.005859375f, 0.251953125f,
        0.251953125f, 0.251953125f, 0.251953125f, 0.501953125f, 0.626953125f,
    };
    static const float g_ips_vel_7c[9] = {
        0.001953125f, 0.005859375f, 0.005859375f, 0.001953125f,
        0.001953125f, 0.001953125f, 0.001953125f, 0.001953125f, 0.001953125f,
    };
    static const float g_ips_vel_80[9] = {
        0.498046875f, 0.24609375f, 0.119140625f, 0.373046875f,
        0.373046875f, 0.373046875f, 0.373046875f, 0.623046875f, 0.748046875f,
    };
    static const float g_ips_vel_84[9] = {
        0.123046875f, 0.12109375f, 0.119140625f, 0.123046875f,
        0.123046875f, 0.123046875f, 0.123046875f, 0.123046875f, 0.123046875f,
    };

    if (this->player != nullptr) {
        if (this->field_a4 != nullptr) {
            this->field_a8 = new Array<int>();
            ArraySetLength(this->field_a4->size(), *(this->field_a8));

            unsigned char *curBase = ParticleSettingsRef::cur;
            unsigned char *entryPtr = curBase + 0x12b4;
            unsigned n = this->field_a4->size();
            for (unsigned i = 0; i < n; i++, entryPtr += 0xa0) {
                const Matrix *playerMat = &this->player->player->transform;
                int slot = this->field_80->addSystem(
                    playerMat,
                    (ParticleSettings::ParticleSet)(i + 0x1d),
                    false);
                (*this->field_a8)[i] = slot;

                AEGeometry *geo = (*this->field_a4)[i];

                float posX = geo->getPosition().x;
                *(int *)(entryPtr - 0x1c) = *(int *)&posX;
                float posY = geo->getPosition().y;
                *(int *)(entryPtr - 0x18) = *(int *)&posY;
                float posZ = geo->getPosition().z;
                *(int *)(entryPtr - 0x14) = *(int *)&posZ;

                float sclX0 = geo->getScaling().x;
                double sclX15 = (double)sclX0 * 1.5;
                int intp;
                if (sclX15 < 1.0) {
                    float sclX1 = geo->getScaling().x;
                    double sv = (double)sclX1 * 1.5 * 80.0;
                    intp = (int)sv;
                } else {
                    intp = 0x50;
                }
                *(int *)(entryPtr - 0x6c) = intp;
                *(int *)(entryPtr - 0x68) = 0x41000000;

                float sclX2 = geo->getScaling().x;
                float sz250 = sclX2 * 250.0f;
                *(float *)(entryPtr - 0x80) = sz250;
                *(int *)(entryPtr - 0x84) = 0x14;

                *(uint32_t *)(entryPtr - 0x60) = 0xddddddff;
                *(uint32_t *)(entryPtr - 0x5c) = 0;

                *(int *)(entryPtr - 0x50) = -1000;

                *(float *)(entryPtr - 0x30) = 0.800000011920929f;

                float sclX3 = geo->getScaling().x;
                double sclX3_15 = (double)sclX3 * 1.5;
                float velResult;
                if (sclX3_15 < 1.0) {
                    float sclX4 = geo->getScaling().x;
                    double sv2 = (double)sclX4 * 1.5 * -4000.0;
                    velResult = (float)sv2;
                } else {
                    velResult = -4000.0f;
                }
                *(float *)(entryPtr - 0x24) = velResult;

                int shipIdx = Globals::status->getShip()->getIndex();
                unsigned midx = (unsigned)(g_ips_shipTypeMap[shipIdx] - 1);
                float v78, v7c, v80, v84;
                if (midx > 8) {
                    v78 = 0.251953125f;
                    v7c = 0.001953125f;
                    v80 = 0.373046875f;
                    v84 = 0.123046875f;
                } else {
                    v78 = g_ips_vel_78[midx];
                    v7c = g_ips_vel_7c[midx];
                    v80 = g_ips_vel_80[midx];
                    v84 = g_ips_vel_84[midx];
                }
                *(float *)(entryPtr - 0xc) = v78;
                *(float *)(entryPtr - 0x8) = v7c;
                *(float *)(entryPtr - 0x4) = v80;
                *(float *)(entryPtr + 0x0) = v84;
            }
        }

        PaintCanvas *canvas = Globals::Canvas;
        canvas->CameraGetCurrent();
        Matrix *local = CameraGetLocal(canvas, canvas->CameraGetCurrent());
        int sys = (this->skybox2Mesh)->addSystem(local, ParticleSettings::ParticleSet_4, false);
        this->movingStarsIndex = sys;

        if (Globals::status->getSystem() != 0) {
            SolarSystem *ss = (SolarSystem *) Globals::status->getSystem();
            if (ss->hasPirateBase() != 0 && this->enemies != nullptr) {
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

        if (Globals::status->inAlienOrbit() != 0) {
            unsigned char *cur = ParticleSettingsRef::cur;
            uint32_t c1 = *(uint32_t *)(cur + 0x494);
            uint32_t c2 = *(uint32_t *)(cur + 0x534);
            c1 = (c1 & 0xff) | (0x9274d4u << 8);
            c2 = (c2 & 0xff) | (0x9274d4u << 8);
            *(uint32_t *)(cur + 0x534) = c2;
            *(uint32_t *)(cur + 0x494) = c1;
        } else {
            unsigned char *cur = ParticleSettingsRef::cur;
            *(uint32_t *)(cur + 0x534) = 0xe2282880u;
            *(uint32_t *)(cur + 0x538) = 0x00ff0080u;
            int texIdx = ((SolarSystem *) Globals::status->getSystem())->getTextureIndex();
            uint32_t col = g_ips_skyColors[texIdx];
            int isSN = Globals::status->inSupernovaSystem();
            float scale = isSN ? 0.5f : 0.6f;
            int alpha = isSN ? 0xff : 0xbb;
            int comp0 = (int)((float)((col >> 24) & 0xff) * scale);
            int comp1 = (int)((float)((col >> 16) & 0xff) * scale);
            int comp2 = (int)((float)((col >>  8) & 0xff) * scale);
            uint32_t result = (uint32_t)(((unsigned)comp0 << 24) | ((unsigned)comp1 << 16) | ((unsigned)comp2 << 8) | (unsigned)alpha);
            *(uint32_t *)(cur + 0x494) = result;
            *(uint32_t *)(cur + 0x498) = result;
        }
    }

    if (Globals::status->inSupernovaSystem() != 0 &&
        Globals::status->getCurrentCampaignMission() != 0x59 &&
        Globals::status->getCurrentCampaignMission() <= 0x9d) {
        Vector ldir = this->starSystem->getLightDirection();
        Vector normalized = AbyssEngine::AEMath::VectorNormalize(ldir);
        Vector negated = AbyssEngine::AEMath::operator-(normalized);
        unsigned char *cur = ParticleSettingsRef::cur;
        *(float *)(cur + 0x4b8) = negated.x * 2000.0f;
        *(float *)(cur + 0x4bc) = negated.y * 2000.0f;
        *(float *)(cur + 0x4c0) = negated.z * 2000.0f;
    } else {
        unsigned char *cur = ParticleSettingsRef::cur;
        *(uint32_t *)(cur + 0x4b8) = 0;
        *(uint32_t *)(cur + 0x4bc) = 0;
        *(uint32_t *)(cur + 0x4c0) = 0;
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
        AbyssEngine::AEMath::Matrix mat_0xa;
        mat_0xa.m[0]  = 1.0f; mat_0xa.m[1]  = 0.0f; mat_0xa.m[2]  = 0.0f;
        mat_0xa.m[3]  = 0.0f; mat_0xa.m[4]  = 0.0f; mat_0xa.m[5]  = 1.0f;
        mat_0xa.m[6]  = 0.0f; mat_0xa.m[7]  = 0.0f; mat_0xa.m[8]  = 0.0f;
        mat_0xa.m[9]  = 0.0f; mat_0xa.m[10] = 1.0f; mat_0xa.m[11] = 0.0f;
        mat_0xa.m[12] = 1.0f; mat_0xa.m[13] = 1.0f; mat_0xa.m[14] = 1.0f;
        AbyssEngine::AEMath::Matrix mat_0xb;
        mat_0xb.m[0]  = 1.0f; mat_0xb.m[1]  = 0.0f; mat_0xb.m[2]  = 0.0f;
        mat_0xb.m[3]  = 0.0f; mat_0xb.m[4]  = 0.0f; mat_0xb.m[5]  = 1.0f;
        mat_0xb.m[6]  = 0.0f; mat_0xb.m[7]  = 0.0f; mat_0xb.m[8]  = 0.0f;
        mat_0xb.m[9]  = 0.0f; mat_0xb.m[10] = 1.0f; mat_0xb.m[11] = 0.0f;
        mat_0xb.m[12] = 1.0f; mat_0xb.m[13] = 1.0f; mat_0xb.m[14] = 1.0f;
        AbyssEngine::AEMath::Matrix mat_14;
        mat_14.m[0]  = 1.0f; mat_14.m[1]  = 0.0f; mat_14.m[2]  = 0.0f;
        mat_14.m[3]  = 0.0f; mat_14.m[4]  = 0.0f; mat_14.m[5]  = 1.0f;
        mat_14.m[6]  = 0.0f; mat_14.m[7]  = 0.0f; mat_14.m[8]  = 0.0f;
        mat_14.m[9]  = 0.0f; mat_14.m[10] = 1.0f; mat_14.m[11] = 0.0f;
        mat_14.m[12] = 1.0f; mat_14.m[13] = 1.0f; mat_14.m[14] = 1.0f;
        AbyssEngine::AEMath::Matrix mat_15;
        mat_15.m[0]  = 1.0f; mat_15.m[1]  = 0.0f; mat_15.m[2]  = 0.0f;
        mat_15.m[3]  = 0.0f; mat_15.m[4]  = 0.0f; mat_15.m[5]  = 1.0f;
        mat_15.m[6]  = 0.0f; mat_15.m[7]  = 0.0f; mat_15.m[8]  = 0.0f;
        mat_15.m[9]  = 0.0f; mat_15.m[10] = 1.0f; mat_15.m[11] = 0.0f;
        mat_15.m[12] = 1.0f; mat_15.m[13] = 1.0f; mat_15.m[14] = 1.0f;
        AbyssEngine::AEMath::Matrix mat_16;
        mat_16.m[0]  = 1.0f; mat_16.m[1]  = 0.0f; mat_16.m[2]  = 0.0f;
        mat_16.m[3]  = 0.0f; mat_16.m[4]  = 0.0f; mat_16.m[5]  = 1.0f;
        mat_16.m[6]  = 0.0f; mat_16.m[7]  = 0.0f; mat_16.m[8]  = 0.0f;
        mat_16.m[9]  = 0.0f; mat_16.m[10] = 1.0f; mat_16.m[11] = 0.0f;
        mat_16.m[12] = 1.0f; mat_16.m[13] = 1.0f; mat_16.m[14] = 1.0f;
        AbyssEngine::AEMath::Matrix mat_17;
        mat_17.m[0]  = 1.0f; mat_17.m[1]  = 0.0f; mat_17.m[2]  = 0.0f;
        mat_17.m[3]  = 0.0f; mat_17.m[4]  = 0.0f; mat_17.m[5]  = 1.0f;
        mat_17.m[6]  = 0.0f; mat_17.m[7]  = 0.0f; mat_17.m[8]  = 0.0f;
        mat_17.m[9]  = 0.0f; mat_17.m[10] = 1.0f; mat_17.m[11] = 0.0f;
        mat_17.m[12] = 1.0f; mat_17.m[13] = 1.0f; mat_17.m[14] = 1.0f;
        AbyssEngine::AEMath::Matrix mat_18a;
        mat_18a.m[0]  = 1.0f; mat_18a.m[1]  = 0.0f; mat_18a.m[2]  = 0.0f;
        mat_18a.m[3]  = 0.0f; mat_18a.m[4]  = 0.0f; mat_18a.m[5]  = 1.0f;
        mat_18a.m[6]  = 0.0f; mat_18a.m[7]  = 0.0f; mat_18a.m[8]  = 0.0f;
        mat_18a.m[9]  = 0.0f; mat_18a.m[10] = 1.0f; mat_18a.m[11] = 0.0f;
        mat_18a.m[12] = 1.0f; mat_18a.m[13] = 1.0f; mat_18a.m[14] = 1.0f;
        AbyssEngine::AEMath::Matrix mat_18b;
        mat_18b.m[0]  = 1.0f; mat_18b.m[1]  = 0.0f; mat_18b.m[2]  = 0.0f;
        mat_18b.m[3]  = 0.0f; mat_18b.m[4]  = 0.0f; mat_18b.m[5]  = 1.0f;
        mat_18b.m[6]  = 0.0f; mat_18b.m[7]  = 0.0f; mat_18b.m[8]  = 0.0f;
        mat_18b.m[9]  = 0.0f; mat_18b.m[10] = 1.0f; mat_18b.m[11] = 0.0f;
        mat_18b.m[12] = 1.0f; mat_18b.m[13] = 1.0f; mat_18b.m[14] = 1.0f;
        this->field_38 = this->field_74->addSystem(&mat_0xa, ParticleSettings::ParticleSet_0xa, false);
        this->field_3c = this->field_74->addSystem(&mat_0xb, ParticleSettings::ParticleSet_0xb, false);
        this->field_48 = this->field_74->addSystem(&mat_14, ParticleSettings::ParticleSet_0x14, false);
        this->field_34 = this->field_74->addSystem(&mat_15, ParticleSettings::ParticleSet_0x15, false);
        this->field_50 = this->field_74->addSystem(&mat_16, ParticleSettings::ParticleSet_0x16, false);
        this->field_54 = this->field_74->addSystem(&mat_17, ParticleSettings::ParticleSet_0x17, false);
        if (Globals::status->getCurrentCampaignMission() == 0x50) {
            this->field_58 = this->field_74->addSystem(&mat_18a, ParticleSettings::ParticleSet_0x18, false);
            this->field_5c = this->field_74->addSystem(&mat_18b, ParticleSettings::ParticleSet_0x18, false);
        }
    }

    (this->field_74)->init();
    (this->field_74)->enableSystemEmit(this->field_50, false);
    (this->field_74)->enableSystemEmit(this->field_54, false);
    if (Globals::status->getCurrentCampaignMission() == 0x50) {
        (this->field_74)->enableSystemEmit(this->field_58, false);
        (this->field_74)->enableSystemEmit(this->field_5c, false);
    }

    this->field_9c->init();
    this->particleEmitBoolPtr->init();
    this->particleRenderBoolPtr->init();
    if (this->field_94 != 0)
        this->field_94->init();
}

void Level::createWingmen() {
    if (Globals::status->inSupernovaSystem() || Globals::status->getCurrentCampaignMission() == 0x9e
        || Globals::status->getWingmen() == 0 || player == nullptr) {
        return;
    }

    Array<KIPlayer *> *wingmen = new Array<KIPlayer *>();
    ArraySetLength(((Array<String *> *) (intptr_t) Globals::status->getWingmen())->count, *wingmen);
    for (unsigned int i = 0; i < wingmen->size(); i++) {
        // each wingman's ship is seeded by its name so it stays the same between visits
        Globals::rnd->setSeed((unsigned int) ((*(Array<String *> *) (intptr_t) Globals::status->getWingmen())[i]->length * 5));
        (*wingmen)[i] = createShip(5, 0, Globals::globals->getRandomEnemyFighter(Globals::status->field_0x2c),
                                   nullptr, true, false);

        Vector position = player->geometry->getPosition();
        Vector right = player->geometry->getRightVector();
        position += right * (i == 0 ? -1000.0f : (i == 1 ? 2000.0f : 0.0f));
        position -= player->geometry->getDirection() * 2000.0f;
        position.y += i == 2 ? 1000.0f : 0.0f;
        (*wingmen)[i]->setPosition(position);
        (*wingmen)[i]->geometry->setDirection(player->geometry->getDirection(), Vector{0.0f, 1.0f, 0.0f});

        (*wingmen)[i]->setWingman(true, i);
        (*wingmen)[i]->player->setAlwaysFriend(true);
        (*wingmen)[i]->player->setHitpoints(600);
        (*wingmen)[i]->name = *(*(Array<String *> *) (intptr_t) Globals::status->getWingmen())[i];
        (*wingmen)[i]->shipGroup = Globals::status->field_0x2c;
        if (((Mission *) Globals::status->getMission())->getType() == 12) {
            (*wingmen)[i]->field_0x25 = 0;
        }
    }

    if (enemies != nullptr) {
        for (unsigned int i = 0; i < wingmen->size(); i++) {
            ArrayAdd((*wingmen)[i], *enemies);
        }
    } else {
        enemies = wingmen;
    }
    Globals::rnd->reset();
}

// Hangar ship spots per scene race ({x, y, z}).
static const int kHangarSpotsTerran[2][3] = {{0, 0, 2891}, {0, 0, 5781}};
static const int kHangarSpotsVossk[9][3] = {{-3715, 0, 661},     {-6983, 36, 2548},   {-9408, 36, 5438},
                                            {-9408, 36, 16301},  {-6983, 36, 19191},  {-3715, 36, 21078},
                                            {-9408, 5359, 16301}, {-6983, 5359, 19191}, {-3715, 5359, 21078}};
static const int kHangarSpotsNivelian[3][3] = {{-4096, 0, 0}, {0, 0, 4096}, {-4096, 0, 4096}};
static const int kHangarSpotsVoid[1][3] = {{4096, 0, 0}};
static const int kHangarSpotsKhador[3][3] = {{-1961, 0, 8512}, {-1961, 0, 5562}, {-1961, 0, 2610}};
static const int kHangarSpotsDefault[2][3] = {{4096, 0, 0}, {4096, 0, 4096}};
static const int *const kHangarSpots[9] = {kHangarSpotsTerran[0], kHangarSpotsVossk[0], kHangarSpotsTerran[0],
                                           kHangarSpotsNivelian[0], kHangarSpotsTerran[0], kHangarSpotsTerran[0],
                                           kHangarSpotsTerran[0], kHangarSpotsTerran[0], kHangarSpotsVoid[0]};

void Level::createScene() {
    static const int kBarChildCounts[4] = {11, 0, 10, 12};
    static const int kBarChildMeshes[4] = {14380, -1, 14403, 14391};
    static const int kBarCounterMeshes[4] = {14281, 14282, 14283, 14284};
    static const int kShipHangarHeights[64] = {
        160, 150, 220, 310, 160, 180, 140, 170, 170, 110, 250, 205, 240, 250, 250, 250,
        170, 170, 190, 460, 170, 210, 140, 170, 160, 190, 170, 240, 220, 280, 200, 200,
        140, 160, 180, 150, 310, 390, 480, 200, 300, 300, 170, 200, 170, 370, 370, 300,
        440, 340, 170, 470, 170, 170, 170, 170, 200, 170, 250, 150, 170, 250, 270, 170};
    static const int kHangarMeshes[10][4] = {
        {14376, 14302, 14301, -1}, {14377, 14304, -1, -1}, {14378, 14308, 14307, -1}, {14379, 14310, 14264, -1},
        {-1, -1, -1, -1},          {-1, -1, -1, -1},       {-1, -1, -1, -1},          {14362, 14358, 14360, 14359},
        {14354, 14355, 14357, 14356}, {-1, -1, -1, -1}};
    static const int kHangarCapacity[10] = {2, 9, 2, 3, 0, 0, 0, 3, 0, 0};
    static const int kBarSeats[4][7][3] = {
        {{-2325, 0, -964}, {-1023, 212, 745}, {-1098, 212, 427}, {-1164, 212, -227}, {-1979, 0, -108},
         {-2556, 0, 646}, {-1439, 212, -860}},
        {{-694, 102, -271}, {914, 102, 453}, {-680, 102, 1734}, {-417, 102, 1454}, {462, 102, -101},
         {285, 102, 913}, {-680, 102, 155}},
        {{-1279, 0, 264}, {-1220, 0, -192}, {-1849, 0, -1214}, {-875, 0, -686}, {48, 0, -911},
         {-457, 0, -2195}, {-794, 0, -1788}},
        {{1542, -10, -642}, {1357, -10, -1986}, {1924, -10, -1377}, {-194, -10, -2542}, {979, -10, -202},
         {1047, -10, 220}, {2319, -10, -498}}};
    static const int kBarMeshes[10][3] = {{14025, 14026, 14027}, {14028, 14029, -1}, {14031, 14032, 14033},
                                          {14034, 14035, 14036}, {-1, -1, -1},       {-1, -1, -1},
                                          {-1, -1, -1},          {-1, -1, -1},       {-1, -1, -1},
                                          {-1, -1, -1}};
    static const int kAgentMeshes[8] = {14725, 14726, 14723, 14725, 14722, 14725, 14720, 14721};

    if (this->enemies != nullptr) {
        ArrayReleaseClasses(*this->enemies);
        delete this->enemies;
    }
    this->enemies = nullptr;
    if (this->missionPtr == 23) {
        {
            int sceneRace;
            if (Globals::status->getStation()->getIndex() == 101)
                sceneRace = 8;
            else if (Globals::status->getStation()->getIndex() == 100)
                sceneRace = 7;
            else
                sceneRace = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
            this->enemies = new Array<KIPlayer *>();
            ArraySetLength(1, *this->enemies);
            int shipIndex = Globals::status->getShip()->getIndex();
            (*this->enemies)[0] = this->createShip(Globals::status->getShip()->getRace(), 0, shipIndex, nullptr,
                                                   this->missionPtr != 23, false);
            (*this->enemies)[0]->setPosition(0.0f, (float) kShipHangarHeights[shipIndex], 0.0f);
            ((PlayerFighter *) (*this->enemies)[0])->removeTrail();
            ((PlayerFighter *) (*this->enemies)[0])->setExhaustVisible(false);
            (*this->enemies)[0]->setToSleep();
            (*this->enemies)[0]->player->setAlwaysFriend(true);
            bool plainHangar = sceneRace == 1 || sceneRace > 3;
            for (int k = 0; k < 4; k++) {
                int mesh = kHangarMeshes[sceneRace][k];
                if (mesh < 0)
                    continue;
                AEGeometry *g = new AEGeometry(mesh, Globals::Canvas, false);
                g->setRotation(Vector{0.0f, 3.1415927f, 0.0f});
                PlayerStatic *hangarPart = new PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
                if (!plainHangar) {
                    for (int j = 0; j < kBarChildCounts[sceneRace]; j++) {
                        AEGeometry *child = new AEGeometry(kBarChildMeshes[sceneRace] + j, Globals::Canvas, false);
                        g->addChild(child->transform);
                        delete child;
                    }
                }
                ArrayAdd((KIPlayer *) hangarPart, *this->enemies);
            }
            bool ownHangar = Globals::status->getStation()->getIndex() == 108 && Globals::status->field_114 == 3;
            int capacity = kHangarCapacity[sceneRace];
            int shipCount = Globals::rnd->nextInt(capacity + 1);
            if (ownHangar) {
                Array<Ship *> *ships = Globals::status->voidStation->getShips();
                shipCount = ships != nullptr ? (capacity <= (int) ships->size() ? capacity : ships->size()) : 0;
            }
            unsigned char *spotUsed = new unsigned char[capacity];
            for (int i = 0; i < capacity; i++)
                spotUsed[i] = 0;
            const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];
            bool customSpots = sceneRace == 0 || sceneRace == 1 || sceneRace == 3 || sceneRace == 8;
            for (int i = 0; i < shipCount; i++) {
                int race = sceneRace;
                if (Globals::rnd->nextInt(100) <= 29) {
                    race = Globals::rnd->nextInt(4);
                    if (Globals::rnd->nextInt(100) < 30)
                        race = 8;
                }
                int type = Globals::globals->getRandomEnemyFighter(race);
                if (Globals::status->getStation()->getIndex() == 100) {
                    int pick = Globals::rnd->nextInt(3);
                    type = pick == 0 ? 37 : pick == 1 ? 38 : 40;
                }
                if (ownHangar)
                    type = (*Globals::status->voidStation->getShips())[i]->getIndex();
                KIPlayer *ship = this->createShip(0, 0, type, nullptr, false, false);
                int spot = Globals::rnd->nextInt(capacity);
                for (int tries = -100; tries != 0 && spotUsed[spot] != 0; tries++)
                    spot = Globals::rnd->nextInt(capacity);
                spotUsed[spot] = 1;
                const int *table = (sceneRace <= 8 && customSpots) ? kHangarSpots[sceneRace] : spots;
                ship->setPosition((float) table[spot * 3], (float) (table[spot * 3 + 1] + kShipHangarHeights[type]),
                                  (float) table[spot * 3 + 2]);
                ship->player->setAlwaysFriend(true);
                ship->setToSleep();
                ship->geometry->setRotation(0.0f, (float) Globals::rnd->nextInt(300) / 100.0f, 0.0f);
                ((PlayerFighter *) ship)->setExhaustVisible(false);
                ArrayAdd(ship, *this->enemies);
            }
            delete[] spotUsed;
            return;
        }
    } else if (this->missionPtr == 4) {
        {
            int race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
            int staticCount = race == 1 ? 2 : 3;
            Array<Agent *> *agents = Globals::status->getStation()->getAgents();
            unsigned char *seatUsed = new unsigned char[7];
            if (agents != nullptr) {
                int agentCount = agents->size();
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(agentCount * 3 + staticCount, *this->enemies);
                for (int i = 0; i < 7; i++)
                    seatUsed[i] = 0;
                for (int i = 0; i < agentCount; i++) {
                    int look;
                    if ((*agents)[i]->getRace() == 3)
                        look = ((*agents)[i]->getImageParts() != nullptr && (*agents)[i]->getImageParts()[0] == 2) ? 2 : 3;
                    else
                        look = (*agents)[i]->getRace();
                    int mesh = kAgentMeshes[look];
                    if ((*agents)[i]->getRace() == 0 && !(*agents)[i]->isMale())
                        mesh = 14724;
                    int seat;
                    do {
                        seat = Globals::rnd->nextInt(7);
                    } while (seatUsed[seat] != 0);
                    float x = (float) kBarSeats[race][seat][0];
                    float y = (float) kBarSeats[race][seat][1];
                    float z = (float) kBarSeats[race][seat][2];
                    seatUsed[seat] = 1;
                    AEGeometry *agentGeometry = new AEGeometry(mesh, Globals::Canvas, false);
                    (*this->enemies)[i] = new PlayerStatic(-1, agentGeometry, x, y, z);
                    AEGeometry *counterGeometry = new AEGeometry(kBarCounterMeshes[race], Globals::Canvas, false);
                    (*this->enemies)[agentCount + i] = new PlayerStatic(-1, counterGeometry, x, y, z);
                    AEGeometry *markerGeometry = new AEGeometry(14348, Globals::Canvas, false);
                    y += 20.0f;
                    (*this->enemies)[agentCount * 2 + i] = new PlayerStatic(-1, markerGeometry, x, y, z);
                }
            } else {
                this->enemies = new Array<KIPlayer *>();
                ArraySetLength(3, *this->enemies);
            }
            for (int k = 0; k < staticCount; k++) {
                AEGeometry *g = new AEGeometry(kBarMeshes[race][k], Globals::Canvas, false);
                (*this->enemies)[this->enemies->size() - staticCount + k] = new PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
            }
            delete[] seatUsed;
            return;
        }
    } else if (this->missionPtr == 2) {
            this->createPlayer();
            Globals::status->setMission(Mission::empty);
            this->createMission();
            if (Globals::status->getCurrentCampaignMission() == 43) {
                AEGeometry *shield = new AEGeometry(14288, Globals::Canvas, false);
                ArrayAdd((KIPlayer *) new PlayerStatic(-1, shield, 0.0f, 0.0f, 0.0f), *this->enemies);
                AEGeometry *beam = new AEGeometry(14289, Globals::Canvas, false);
                ArrayAdd((KIPlayer *) new PlayerStatic(-1, beam, 0.0f, 0.0f, 0.0f), *this->enemies);
            }
            return;
    }
}
void Level::renderBG(int dt) {
    using namespace AbyssEngine::AEMath;
    Globals::Canvas->SetColor(0xffffffff);
    Globals::Canvas->BeginBG();
    skyMatrix = MatrixGetInverse(*(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent()));
    skyMatrix.m[3] = 0;
    skyMatrix.m[7] = 0;
    skyMatrix.m[11] = 0;
    if (!Globals::status->inAlienOrbit() && ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 27) {
        // align the skybox with the star's light direction
        Vector side = {1.0f, 0.0f, 0.0f};
        Vector dir = VectorNormalize(starSystem->getLightDirection());
        Vector right = VectorNormalize(VectorCross(side, dir));
        Vector up = VectorNormalize(VectorCross(right, dir));
        side = -up;
        MatrixSetRotation(cloudMatrix, side, dir, right);
    } else {
        MatrixSetRotation(cloudMatrix, skyRotX, skyRotY, skyRotZ);
    }
    skyMatrix *= cloudMatrix;
    Globals::Canvas->SetWorldViewMatrix(skyMatrix);
    Globals::Canvas->SetTexture(field_19c, 0xffffffff);
    Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_dummy);
    Globals::Canvas->DrawMesh(field_08);
    Globals::Canvas->SetTexture(field_198, 0xffffffff);
    Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_2);
    Globals::Canvas->DrawMesh(skyboxMesh);

    if (field_1b4 != -1) {
        Globals::Canvas->SetTexture(field_1b8, 0xffffffff);
        Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_1);
        skyMatrix = MatrixGetInverse(*(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent()));
        skyMatrix.m[3] = 0;
        skyMatrix.m[7] = 0;
        skyMatrix.m[11] = 0;
        Matrix view = MatrixGetInverse(skyMatrix);
        Globals::Canvas->DrawTransform(field_1b4, &view);
    }

    starSystem->render();

    if (Globals::status->inSupernovaSystem() && skyboxTexture != -1) {
        int mission = Globals::status->getCurrentCampaignMission();
        Globals::Canvas->SetTexture(field_1a0, 0xffffffff);
        Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_2);
        long long time = (long long) ((mission > 0x6a ? 1.5f : 1.0f) * (float) dt);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(field_10))->Update(time, false);
        Matrix view = MatrixGetInverse(skyMatrix);
        Globals::Canvas->DrawTransform(field_10, &view);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(field_18))->Update(time, false);
        Globals::Canvas->DrawTransform(field_18, &view);
    }

    if (field_1bc != -1) {
        Globals::Canvas->SetTexture(field_1c0, 0xffffffff);
        Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_2);
        skyMatrix = MatrixGetInverse(*(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent()));
        AbyssEngine::Transform *transform = (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(field_1bc);
        int before = (int) transform->currentTime;
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(field_1bc))->Update(dt, false);
        if ((int) transform->currentTime < before) {
            // the animation wrapped around: pick a new random orientation
            float x = (float) Globals::rnd->nextInt(0x10000) * (1.0f / 65536.0f) * 6.2831855f;
            float y = (float) Globals::rnd->nextInt(0x10000) * (1.0f / 65536.0f) * 6.2831855f;
            float z = (float) Globals::rnd->nextInt(0x10000) * (1.0f / 65536.0f) * 6.2831855f;
            MatrixSetRotation(reversalMatrix, x, y, z);
        }
        skyMatrix *= reversalMatrix;
        skyMatrix.m[7] = 0;
        skyMatrix.m[3] = 0;
        skyMatrix.m[11] = 0;
        Matrix view = MatrixGetInverse(skyMatrix);
        Globals::Canvas->DrawTransform(field_1bc, &view);
    }

    if (supernovaFlareActive && *reinterpret_cast<float *>(&Globals::options[0x28]) >= 1.0f) {
        skyMatrix = MatrixGetInverse(*(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent()));
        skyMatrix.m[3] = 0;
        skyMatrix.m[7] = 0;
        skyMatrix.m[11] = 0;
        Globals::Canvas->SetWorldViewMatrix(skyMatrix);
        Globals::Canvas->SetColor(0xffffffff);
        Matrix identity;
        Globals::Canvas->engine->SetModelMatrix(identity);
        Globals::Canvas->SetTexture(supernovaFlareTexture, field_1c8);
        Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_8);
        Globals::Canvas->engine->LightSetLight(0x4000);
        Globals::Canvas->engine->GlEnable(0x1100013, true);
        Globals::Canvas->DrawMesh(supernovaFlareMesh);
        Globals::Canvas->engine->GlEnable(0x1100013, false);
        Globals::Canvas->engine->LightEnable(false);
    }

    Globals::Canvas->EndBG();
}

static inline void levelSpawnFar(Level *self, int *kiPlayer) {
    AbyssEngine::AERandom *rng = Globals::rnd;
    Vector p = (self->player == 0 ? Vector{0.0f, 0.0f, 0.0f} : self->player->getPosition());
    float ox = (float) (rng->nextInt() % 120000 - 60000);
    float oy = (float) (rng->nextInt() % 80000 - 40000);
    float oz = (float) (rng->nextInt() % 120000 - 60000);
    ((KIPlayer *) kiPlayer)->setPosition(p.x + ox, p.y + oy, p.z + oz);
}




unsigned char Level::doInstantJump;
Station *Level::programmedStation;
bool Level::comingFromAlienWorld;
unsigned char Level::initStreamOutPosition;
int Level::energyCellsForNextJump;
int Level::lastMissionFreighterHitpoints;
unsigned char Level::initStreamOutPositionAfterCutscene;
float Level::i_a;
float Level::i_b;
float Level::i_g;
float Level::i_r;
int Level::b_min;
int Level::g_min;
int Level::r_min;
