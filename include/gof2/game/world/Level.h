#ifndef GOF2_LEVEL_H
#define GOF2_LEVEL_H
#include "StarSystem.h"
#include "gof2/common.h"

// Owning headers for the classes Level exposes in its own interface (member pointers,
// method parameters and returns). Types used only inside Level.cpp are included there.
#include "gof2/game/world/Route.h"
#include "gof2/game/world/Waypoint.h"
#include "gof2/game/core/RadioMessage.h"
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/ship/PlayerFixedObject.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/mission/Objective.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/game/weapons/ObjectGun.h"
#include "gof2/engine/render/ParticleSystemManager.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/engine/render/LODManager.h"
#include "gof2/engine/render/LodMeshMerger.h"
#include "gof2/engine/math/BoundingVolume.h"

// Galaxy on Fire 2 — Level: the in-flight game world / mission space. It owns the
// skybox, the actor rosters (player, enemies, asteroids, gas clouds, landmarks),
// the weapon objects, the radio-message queue and the per-orbit mission state.
class Level {
public:
    LODManager* lodManager;         // owning LOD manager (offset 0; Level is non-polymorphic)
    int skyboxMesh;
    int field_08;
    int skyboxTexture;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int killCountA;                 // friendly-fire kill tally
    int killCountB;                 // player kill tally
    Objective* objectivesA;
    Objective* objectivesB;
    int field_30;
    int field_34;
    int field_38;
    int field_3c;
    int field_40;
    int field_44;
    int field_48;
    int field_4c;
    int field_50;
    int field_54;
    int field_58;
    int field_5c;
    int field_60;
    int movingStarsIndex;
    uint8_t field_68;
    uint8_t field_69;
    uint8_t pad_6a[2];
    int field_6c;
    int field_70;
    ParticleSystemManager* field_74;
    ParticleSystemManager* particleEmitBoolPtr;
    ParticleSystemManager* particleSystemMgr;
    ParticleSystemManager* field_80;
    ParticleSystemManager* particleRenderBoolPtr;
    ParticleSystemManager* skybox2Mesh; // engine-trail PSM (legacy name)
    int field_8c;
    ParticleSystemManager* field_90;
    int field_94;
    ParticleSystemManager* field_98;
    ParticleSystemManager* field_9c;
    AbyssEngine::LodMeshMerger* field_a0;
    Array<AEGeometry*>* field_a4;
    Array<int>* field_a8;           // value array; no element dtors
    int miningPlantIndex;
    Array<KIPlayer*>* field_b0;     // sentry guns; elements also owned via enemies
    int field_b4;
    int field_b8;
    int field_bc;
    int missionPtr;                 // ctor argument
    BoundingVolume* collisionVolume;
    int field_c8;
    int field_cc;
    int field_d0;
    int field_d4;
    Waypoint* asteroidWaypoint;
    int field_dc;
    int field_e0;
    Array<ObjectGun*>* playerGuns;
    Array<ObjectGun*>* enemyGuns;
    StarSystem* starSystem;
    PlayerEgo* player;
    Array<KIPlayer*>* gasClouds;
    Array<KIPlayer*>* enemies;
    Array<KIPlayer*>* asteroids;
    Array<KIPlayer*>* landmarks;
    Array<AEGeometry*>* field_104;
    Route* playerRoute;
    Route* friendRoute;
    Route* enemyRoute;
    Array<void*>* messages;         // opaque RadioMessage* elements
    int enemiesLeft;
    int friendsLeft;
    int field_120;
    int field_124;
    int asteroidsLeft;
    int kills;
    int timeLimit;
    int field_134;
    int field_138;
    uint8_t friendCargoStolen;
    uint8_t pad_13d[3];
    Vector flashColor;
    int flashField;
    int flashDurationA;
    int flashDurationB;
    uint8_t flashActive;
    uint8_t pad_159[3];
    int flashType;
    int friendCount;                // active friendly slot count
    int field_164;
    int field_168;
    int hostileCount;               // trailing hostile slot count
    int alienAttackTimer;           // updateAlienAttackers accumulator
    int orbitWaveTimer;             // updateOrbit/updateMissionOrbit wave timer
    int field_178;
    int field_17c;
    Route* field_180;
    int field_184;
    uint16_t field_188;             // low byte friendTurnedEnemy flag, high byte alarm flag
    uint8_t field_18a;
    uint8_t pad_18b;
    int field_18c;
    int field_190;
    int field_194;
    int field_198;
    int field_19c;
    int field_1a0;
    float skyRotX;                  // stored skybox Euler angles (built into sub_20c)
    float skyRotY;
    float skyRotZ;
    uint8_t field_1b0;
    uint8_t pad_1b1[3];
    int field_1b4;
    int field_1b8;
    int field_1bc;
    int field_1c0;
    int supernovaFlareTexture;
    int field_1c8;
    int supernovaFlareMesh;
    uint8_t sub_1d0[0x3c];          // skybox-matrix sub-object
    uint8_t sub_20c[0x3c];          // cloud-layer matrix sub-object
    uint8_t sub_248[0x3c];          // ring matrix sub-object
    int field_284;
    uint8_t field_288;              // boss-present flag
    uint8_t supernovaFlareActive;   // supernova flare visible flag
    uint8_t pad_28a[2];
    int miningPlant;
    int numDeliveredOre;
    int numDeliveredPassengers;
    int field_298;
    uint8_t field_29c;              // attackWanted flag
    uint8_t field_29d;              // killWanted flag
    uint8_t field_29e;
    uint8_t pad_29f;

    Level(int mission);
    ~Level();

    static void setInitStreamOut();

    int init();
    void createSpace();
    void createPlayer();
    void createAsteroids();
    void createGasClouds();
    void createMission();
    void createScene();
    void createCampaignMission();
    void createStaticObjects();
    void createSentryGuns();
    void createFighterTurrets();
    void createWingmen();
    void assignGuns();
    void connectPlayers();
    void enableParticleEffects(bool emit, bool render);
    void setPlayerEngineColor(short color);
    void initParticleSystems();
    StarSystem* getStarSystem();
    Array<KIPlayer*>* getGasClouds();
    // `kind` is the item "sort" (ItemSort); kept as int so the mangled symbol matches the original.
    Gun *createGun(int idx, int owner, int kind, int hp, int dmg, int rate, int cool, int color);
    int createStaticObject(Waypoint *wp, int type, int jitter);
    PlayerFixedObject *createShip(int race, int shipClass, int type, Waypoint *wp, int hostile, int group);
    Route *createRoute(int count);
    void setPlayerRoute(Route *route);
    void createRadioMessages(int set);
    void getBoundingVolume();
    PlayerEgo* getPlayer();
    Array<KIPlayer*>* getEnemies();
    Array<KIPlayer*>* getLandmarks();
    Array<KIPlayer*>* getAsteroids();
    Waypoint* getAsteroidWaypoint();
    Route* getPlayerRoute();
    Route* getEnemyRoute();
    Route* getFriendRoute();
    void flashScreen(int type);
    void enemyDied(int r1, bool r2arg);
    void junkDied();
    void applyKills();
    void friendDied();
    void wingmanDied(int name);
    void asteroidDied();
    int getAsteroidsLeft();
    int getEnemiesLeft();
    int getFriendsLeft();
    Array<void*>* getMessages();
    void *getActiveMessages();
    int getTimeLimit();
    int collide(Vector v);
    void isInAsteroidCenterRange(Vector v);
    int collideStream(Vector v);
    int collideStation(Vector v);
    void renderBG(int t);
    void render(int ctx);
    void render2D();
    void renderPause();
    void updateMissionOrbit(int dt);
    void updateOrbit(int dt);
    void alarmAllFriends(int race, bool message);
    void createRadioMessage(int type, int param);
    void updateAlienAttackers(int dt);
    void updateAsteroidCluster();
    void update(long long time, unsigned dtArg, int stackFlag);
    int checkObjective();
    void stealFriendCargo();
    uint8_t friendCargoWasStolen();
    void removeObjectives();
    Array<ObjectGun*>* getPlayerGuns();
    Array<ObjectGun*>* getEnemyGuns();
    int checkGameOver();
    void pirateStationAction(bool param);
    void uncoverWanted(int index);
    void attackWanted(int index);
    void almostKillWanted(int index);
    void killWanted();
    void friendTurnedEnemy();
    void enableFog(bool enable);
    void enableMovingStars(bool enable);
    void reset();
    void switchSkyboxForIntro();
    void switchSkyboxForSupernovaReversal();
    int getNumDockingTargets();
    int getDockingTarget(int index);
    bool hasMiningPlant();
    int getMiningPlant();
    int getNumDeliveredOre();
    void incNumDeliveredOre(int delta);
    int getNumDeliveredPassengers();
    void incNumDeliveredPassengers(int delta);

    // createScene()'s wingman-roster removal performs a real array-remove loop.
    void wingmanDied_one(String *name, unsigned int *list);

    // createRadioMessage(): hand the finished message queue to the player-ego's
    // comm controller. A null queue clears the channel.
    void crm_dispatch(int egoComm, void *queue);

    // createSpace(): three slices of the skybox/station builder — skybox detail
    // meshes, the star-system backdrop spin, the home station + gates.
    void csp_buildDetail();
    void csp_buildStarSystemScene();
    void csp_buildStationAndGates();

    // init(): place the player at the spawn appropriate for the orbit context.
    void init_placePlayer(int statusA, int stationStack);

    // createMission(): build the per-mission-type actors/objectives.
    void cm_buildMissionScene(Mission *mission);

    // createAsteroids(): reject-sampling distance test + lod-mesh table install.
    float ca_asteroidDistance(unsigned idx, Vector *pos);
    void  ca_installLodMeshes(AEGeometry *geo, short baseMesh, int near);

    // createCampaignMission(): build the scripted campaign scene.
    void ccm_buildCampaignScene(int missionIndex);

    // updateOrbit()/updateMissionOrbit()/updateAlienAttackers(): revive + reposition.
    void uo_spawnFar(int *kiPlayer);
    void umo_spawnAt(int *kiPlayer, int profile);
    void uaa_placeAlien(int *kiPlayer, int alienInOrbit);

    // createStaticObject(): construct the requested static-object type, positioned.
    int cso2_construct(int type, int x, int y, int z);

    // getBoundingVolume(): build one BoundingVolume from a raw collision record.
    BoundingVolume *gbv_makeVolume(int rec, int shape);

    // createShip(): build the class-appropriate bounding-volume array + wreck mesh.
    void *cs_buildBV(int race, int type, int *outWreckMesh);

    // createGasClouds(): pick a far random spawn position for cloud `i`.
    void cgc_randomPos(int rng, int boss, unsigned i, Vector *out);

    // initParticleSystems(): per-asteroid dust, ambient tint, engine-trail systems.
    void ips_buildAsteroidDust(void *arr);
    void ips_applyAmbient();
    int  ips_addPlayerSystem(int kind);

    // createWingmen()/createScene(): position a wingman / static actor.
    void cwm_placeWingman(int *kiSlot, unsigned i);
    void csc_placeActor(int actor, int idx, int profile);

};

#endif
