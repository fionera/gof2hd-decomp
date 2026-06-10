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

// Galaxy on Fire 2 — Level (the in-flight game world / mission space).
// Layout deduced from getter/setter disassembly. Total size 0x2a0.
struct Level {
    // --- header / skybox + objective state (0x00..0xb0) ---
    uint vtable;             // 0x00 (4-byte vtable slot; ARM32 32-bit pointer layout)
    int skyboxMesh;          // 0x04
    int field_08;            // 0x08
    int skyboxTexture;       // 0x0c
    int field_10;            // 0x10
    int field_14;            // 0x14
    int field_18;            // 0x18
    Vector field_1c;         // 0x1c (0x1c..0x28)
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
    int landmarks;           // 0x100
    int field_104;           // 0x104
    int playerRoute;         // 0x108
    int friendRoute;         // 0x10c
    int enemyRoute;          // 0x110
    int messages;            // 0x114
    int enemiesLeft;         // 0x118
    int friendsLeft;         // 0x11c
    int field_120;           // 0x120
    int field_124;           // 0x124
    int asteroidsLeft;       // 0x128
    int kills;               // 0x12c
    int timeLimit;           // 0x130
    int field_134;           // 0x134
    int field_138;           // 0x138
    uint8_t friendCargoStolen; // 0x13c
    uint8_t pad_13d[3];      // 0x13d
    Vector flashColor;       // 0x140 (0x140..0x14c)
    int flashField;          // 0x14c
    int flashDurationA;      // 0x150
    int flashDurationB;      // 0x154
    uint8_t flashActive;     // 0x158
    uint8_t pad_159[3];      // 0x159
    int flashType;           // 0x15c
    Vector field_160;        // 0x160 (0x160..0x16c)
    Vector field_16c;        // 0x16c (0x16c..0x178)
    int field_178;           // 0x178
    int field_17c;           // 0x17c
    int field_180;           // 0x180
    int field_184;           // 0x184
    uint16_t field_188;      // 0x188 (0x188 friendTurnedEnemy flag, 0x189 alarm flag)
    uint8_t field_18a;       // 0x18a
    uint8_t pad_18b;         // 0x18b
    int field_18c;           // 0x18c
    int field_190;           // 0x190
    int field_194;           // 0x194
    int field_198;           // 0x198 (skybox2 mesh region; this+0x198)
    int field_19c;           // 0x19c
    int field_1a0;           // 0x1a0
    int field_1a4;           // 0x1a4
    int field_1a8;           // 0x1a8
    int field_1ac;           // 0x1ac
    uint8_t field_1b0;       // 0x1b0
    uint8_t pad_1b1[3];      // 0x1b1
    int field_1b4;           // 0x1b4
    int field_1b8;           // 0x1b8
    int field_1bc;           // 0x1bc
    int field_1c0;           // 0x1c0
    uint8_t pad_1c4[12];     // 0x1c4
    uint8_t sub_1d0[0x3c];   // 0x1d0 (constructed sub-object)
    uint8_t sub_20c[0x3c];   // 0x20c (constructed sub-object)
    uint8_t sub_248[0x3c];   // 0x248 (constructed sub-object)
    int field_284;           // 0x284
    uint16_t field_288;      // 0x288
    uint8_t pad_28a[2];      // 0x28a
    int miningPlant;         // 0x28c
    int numDeliveredOre;     // 0x290
    int numDeliveredPassengers; // 0x294
    int field_298;           // 0x298
    uint8_t field_29c;       // 0x29c (attackWanted flag)
    uint8_t field_29d;       // 0x29d (killWanted flag)
    uint8_t field_29e;       // 0x29e
    uint8_t pad_29f;         // 0x29f

    // --- methods ---
    Level(int mission);
    ~Level();

    static void setInitStreamOut();

    void init();
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
    int createGun();
    void createStaticObject();
    void createShip();
    Route *createRoute(int count);
    void setPlayerRoute(Route *route);
    void createRadioMessages();
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
    void enemyDied();
    void junkDied();
    void applyKills();
    void friendDied();
    void wingmanDied(int name);
    void asteroidDied();
    int getAsteroidsLeft();
    int getEnemiesLeft();
    int getFriendsLeft();
    int getMessages();
    int getTimeLimit();
    int collide(Vector v);
    void isInAsteroidCenterRange(Vector v);
    int collideStream(Vector v);
    int collideStation(Vector v);
    void renderBG();
    void render(int ctx);
    void render2D();
    void renderPause();
    void updateMissionOrbit();
    void updateOrbit();
    void alarmAllFriends(int race, bool message);
    void createRadioMessage(int type, int param);
    void updateAlienAttackers();
    void updateAsteroidCluster();
    void update();
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
