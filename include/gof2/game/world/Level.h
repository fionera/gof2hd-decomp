#ifndef GOF2_LEVEL_H
#define GOF2_LEVEL_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// Engine array container (size, data, capacity).

// Forward declarations for related game classes referenced by Level.
struct Route;
struct RadioMessage;
struct KIPlayer;
struct PlayerEgo;
struct Ship;
struct SolarSystem;
struct Station;
struct Wanted;
struct Status;
struct Player;
struct ParticleSystemManager;
struct PaintCanvas;
struct Mission;
struct Gun;
struct Waypoint;
struct PlayerFixedObject;
struct BoundingVolume;
struct AEGeometry;

// Galaxy on Fire 2 — Level (the in-flight game world / mission space).
// Layout deduced from getter/setter disassembly. Total size 0x2a0.
class Level {
public:
    // --- header / skybox + objective state (0x00..0xb0) ---
    uint vtable;             // 0x00 (4-byte vtable slot; ARM32 32-bit pointer layout)
    int skyboxMesh;          // 0x04
    int field_08;            // 0x08
    int skyboxTexture;       // 0x0c
    int field_10;            // 0x10
    int field_14;            // 0x14
    int field_18;            // 0x18
    int field_1c;            // 0x1c (scalar counters, not a vector)
    int killCountA;          // 0x20 (friendly-fire kill tally)
    int killCountB;          // 0x24 (player kill tally)
    int objectivesA;         // 0x28
    int objectivesB;         // 0x2c
    int field_30;            // 0x30
    int field_34;            // 0x34
    int field_38;            // 0x38
    int field_3c;            // 0x3c
    int field_40;            // 0x40
    int field_44;            // 0x44
    int field_48;            // 0x48
    int field_4c;            // 0x4c
    int field_50;            // 0x50
    int field_54;            // 0x54
    int field_58;            // 0x58
    int field_5c;            // 0x5c
    int field_60;            // 0x60
    int movingStarsIndex;    // 0x64
    uint8_t field_68;        // 0x68
    uint8_t field_69;        // 0x69
    uint8_t pad_6a[2];       // 0x6a
    int field_6c;            // 0x6c
    int field_70;            // 0x70
    int field_74;            // 0x74
    int particleEmitBoolPtr; // 0x78
    int particleSystemMgr;   // 0x7c
    int field_80;            // 0x80
    int particleRenderBoolPtr; // 0x84
    int skybox2Mesh;         // 0x88 (movingStars target)
    int field_8c;            // 0x8c
    int field_90;            // 0x90
    int field_94;            // 0x94
    int field_98;            // 0x98
    int field_9c;            // 0x9c
    int field_a0;            // 0xa0
    int field_a4;            // 0xa4
    int field_a8;            // 0xa8
    int miningPlantIndex;    // 0xac
    int field_b0;            // 0xb0
    int field_b4;            // 0xb4
    int field_b8;            // 0xb8
    int field_bc;            // 0xbc
    int missionPtr;          // 0xc0 (ctor param_1)
    int collisionVolume;     // 0xc4
    int field_c8;            // 0xc8
    int field_cc;            // 0xcc
    int field_d0;            // 0xd0
    int field_d4;            // 0xd4
    int asteroidWaypoint;    // 0xd8
    int field_dc;            // 0xdc
    int field_e0;            // 0xe0
    int playerGuns;          // 0xe4
    int enemyGuns;           // 0xe8
    int starSystem;          // 0xec
    int player;              // 0xf0
    int gasClouds;           // 0xf4
    int enemies;             // 0xf8
    int asteroids;           // 0xfc
    int landmarks;
    int field_104;
    int playerRoute;
    int friendRoute;
    int enemyRoute;
    int messages;
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
    Vector flashColor;       // 0x140 (0x140..0x14c)
    int flashField;
    int flashDurationA;
    int flashDurationB;
    uint8_t flashActive;
    uint8_t pad_159[3];
    int flashType;
    int friendCount;         // 0x160 (active friendly slot count)
    int field_164;           // 0x164
    int field_168;           // 0x168
    int hostileCount;        // 0x16c (trailing hostile slot count)
    int alienAttackTimer;    // 0x170 (updateAlienAttackers accumulator)
    int orbitWaveTimer;      // 0x174 (updateOrbit/updateMissionOrbit wave timer)
    int field_178;
    int field_17c;
    int field_180;
    int field_184;
    uint16_t field_188;      // 0x188 (0x188 friendTurnedEnemy flag, 0x189 alarm flag)
    uint8_t field_18a;
    uint8_t pad_18b;
    int field_18c;
    int field_190;
    int field_194;
    int field_198;           // 0x198 (skybox2 mesh region; this+0x198)
    int field_19c;
    int field_1a0;
    int field_1a4;
    int field_1a8;
    int field_1ac;
    uint8_t field_1b0;
    uint8_t pad_1b1[3];
    int field_1b4;
    int field_1b8;
    int field_1bc;
    int field_1c0;
    int supernovaFlareTexture; // +0x1c4 (supernova flare mesh texture handle)
    int field_1c8;           // +0x1c8
    int supernovaFlareMesh;  // +0x1cc (supernova flare mesh handle)
    uint8_t sub_1d0[0x3c];   // 0x1d0 (constructed sub-object)
    uint8_t sub_20c[0x3c];   // 0x20c (constructed sub-object)
    uint8_t sub_248[0x3c];   // 0x248 (constructed sub-object)
    int field_284;
    uint8_t field_288;       // +0x288 (boss-present flag)
    uint8_t supernovaFlareActive; // +0x289 (supernova flare visible flag)
    uint8_t pad_28a[2];
    int miningPlant;
    int numDeliveredOre;
    int numDeliveredPassengers;
    int field_298;
    uint8_t field_29c;       // 0x29c (attackWanted flag)
    uint8_t field_29d;       // 0x29d (killWanted flag)
    uint8_t field_29e;
    uint8_t pad_29f;

    // --- methods ---
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
    int getStarSystem();
    int getGasClouds();
    Gun *createGun(int idx, int owner, int kind, int hp, int dmg, int rate, int cool, int color);
    int createStaticObject(Waypoint *wp, int type, int jitter);
    PlayerFixedObject *createShip(int race, int shipClass, int type, Waypoint *wp, int hostile, int group);
    Route *createRoute(int count);
    void setPlayerRoute(Route *route);
    void createRadioMessages(int set);
    void getBoundingVolume();
    int getPlayer();
    int getEnemies();
    int getLandmarks();
    int getAsteroids();
    int getAsteroidWaypoint();
    int getPlayerRoute();
    int getEnemyRoute();
    int getFriendRoute();
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
    int getMessages();
    void *getActiveMessages();
    int getTimeLimit();
    int collide(Vector v);
    void isInAsteroidCenterRange(Vector v);
    int collideStream(Vector v);
    int collideStation(Vector v);
    void renderBG(float t);
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
    int getPlayerGuns();
    int getEnemyGuns();
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

    // Recovered decompiler fragments: each forwards through a PI veneer to the
    // resolved sibling-class method (see src/Level.cpp for the resolution notes).
    void render2D_call(int starSystem);
    void render_tail(int starSystem);
    int checkGameOver_call(int objective);
    int checkObjective_call(int objective);
    void enableMovingStars_call(int mgr, int index, bool enable);
    void enableFog_call(int mgr, int sys, bool enable);
    void wanted_action(int code, int arg);
    void pirateStationAction_tail(int code, int arg);
    void friendTurnedEnemy_action(int code, int arg);
    void alarmAllFriends_tail(Station *station, int flag);
    void almostKillWanted_tail(int wanted, int active);
    void wingmanDied_all(Status *status, int zero);
    void wingmanDied_one(String *name, unsigned int *list);
    int createStaticObject_call(int wp, int type, int jitter);

    // init() runs its build pipeline by chaining to each of these sibling
    // builders (the decompiler split them out as separate _init fragments);
    // every one simply re-enters the matching no-argument Level method.
    void createSpace_init();
    void createPlayer_init();
    void createAsteroids_init();
    void createGasClouds_init();
    void createMission_init();
    void createCampaignMission_init();
    void createScene_init();
    void createStaticObjects_init();
    void createSentryGuns_init();
    void createFighterTurrets_init();
    void createWingmen_init();
    void assignGuns_init();
    void connectPlayers_init();

    // Per-context spawn helpers all funnel into the single createShip(...)
    // body; the suffix marks the calling scene builder (mission / campaign /
    // cut-scene / wingmen).
    PlayerFixedObject *createShip_cm(int race, int shipClass, int type, int wp, int hostile, int group);
    PlayerFixedObject *createShip_ccm(int race, int shipClass, int type, int wp, int hostile, int group);
    PlayerFixedObject *createShip_csc(int race, int shipClass, int type, int wp, int hostile, int group);
    PlayerFixedObject *createShip_cwm(int race, int shipClass, int type, int wp, int hostile, int group);

    // Static-object spawners (createStaticObjects / createFighterTurrets) both
    // re-enter createStaticObject(wp, type, jitter).
    int createStaticObject_cso(int wp, int type, int jitter);
    int createStaticObject_cft(int wp, int type, int jitter);

    // createScene(mode 2) replays the in-flight player/mission build.
    void createPlayer_csc();
    void createMission_csc();

    // update() dispatches the orbit/attacker ticks through these veneers.
    void updateMissionOrbit_call(int dt);
    void updateOrbit_call(int dt);
    void updateAlienAttackers_call();

    // updateOrbit() raises the delayed "friends turned hostile" alarm and the
    // accompanying radio message through these veneers.
    void alarmAllFriends_uo(int race, int message);
    void createRadioMessage_uo(int type);

    // ------------------------------------------------------------------------
    // Decompiler-split scene/build fragments.
    //
    // Each of these was emitted as a standalone Level_<suffix> helper because the
    // owning method (createSpace / init / createMission / ...) was too large for
    // the decompiler to lift in one piece — typically the cut fell on a block of
    // SIMD float math. They are recovered here as real members performing that
    // block's work through the engine shims declared at each call site.
    // ------------------------------------------------------------------------

    // createRadioMessage(): hand the finished message queue to the player-ego's
    // comm controller (ego->comm at +0x18). A null queue clears the channel.
    void crm_dispatch(int egoComm, void *queue);

    // createSpace(): three slices of the monolithic skybox/station builder —
    // skybox detail meshes, the star-system backdrop spin, the home station + gates.
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

    // renderBG(): build the rotated skybox basis into self+0x1d0.
    void rbg_buildSkyMatrix(int mode, float spin);
};

static_assert(sizeof(Level) == 0x2a0, "Level size");
static_assert(__builtin_offsetof(Level, objectivesA) == 0x28, "objectivesA");
static_assert(__builtin_offsetof(Level, particleSystemMgr) == 0x7c, "particleSystemMgr");
static_assert(__builtin_offsetof(Level, miningPlantIndex) == 0xac, "miningPlantIndex");
static_assert(__builtin_offsetof(Level, player) == 0xf0, "player");
static_assert(__builtin_offsetof(Level, enemies) == 0xf8, "enemies");
static_assert(__builtin_offsetof(Level, asteroids) == 0xfc, "asteroids");
static_assert(__builtin_offsetof(Level, landmarks) == 0x100, "landmarks");
static_assert(__builtin_offsetof(Level, playerRoute) == 0x108, "playerRoute");
static_assert(__builtin_offsetof(Level, messages) == 0x114, "messages");
static_assert(__builtin_offsetof(Level, enemiesLeft) == 0x118, "enemiesLeft");
static_assert(__builtin_offsetof(Level, asteroidsLeft) == 0x128, "asteroidsLeft");
static_assert(__builtin_offsetof(Level, kills) == 0x12c, "kills");
static_assert(__builtin_offsetof(Level, timeLimit) == 0x130, "timeLimit");
static_assert(__builtin_offsetof(Level, friendCargoStolen) == 0x13c, "friendCargoStolen");
static_assert(__builtin_offsetof(Level, flashColor) == 0x140, "flashColor");
static_assert(__builtin_offsetof(Level, flashActive) == 0x158, "flashActive");
static_assert(__builtin_offsetof(Level, flashType) == 0x15c, "flashType");
static_assert(__builtin_offsetof(Level, sub_1d0) == 0x1d0, "sub_1d0");
static_assert(__builtin_offsetof(Level, field_284) == 0x284, "field_284");
static_assert(__builtin_offsetof(Level, miningPlant) == 0x28c, "miningPlant");
static_assert(__builtin_offsetof(Level, numDeliveredOre) == 0x290, "numDeliveredOre");
static_assert(__builtin_offsetof(Level, field_29c) == 0x29c, "field_29c");
#endif
