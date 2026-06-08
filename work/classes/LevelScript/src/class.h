#ifndef WORK_CLASSES_LEVELSCRIPT_SRC_CLASS_H
#define WORK_CLASSES_LEVELSCRIPT_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

struct Level;
struct Hud;
struct Radar;
struct TargetFollowCamera;
struct AEGeometry;
struct Explosion;
struct KIPlayer;
struct Player;
struct PlayerFighter;
struct RadioMessage;
struct Route;
struct SolarSystem;
struct StarSystem;
struct Station;
struct Vector;

struct LevelScript {
    // @portable-fields
    int* f_0; // 0x0
    void* f_4; // 0x4
    void* f_8; // 0x8
    int f_c; // 0xc
    int f_10; // 0x10
    TargetFollowCamera* m_pCamera; // 0x14
    MGame* m_pLevel; // 0x18
    int m_nState; // 0x1c
    int f_20; // 0x20
    int f_24; // 0x24
    float m_nStreamTimer; // 0x28
    float m_nStreamOffsetX; // 0x2c
    float m_nStreamOffsetZ; // 0x30
    int f_34; // 0x34
    int f_38; // 0x38
    int f_3c; // 0x3c
    int f_40; // 0x40
    int f_44; // 0x44
    void* f_48; // 0x48
    int m_pMatrix; // 0x4c
    unsigned char _pad_50[60];
    float f_8c; // 0x8c
    int f_90; // 0x90
    int f_94; // 0x94
    int f_98; // 0x98
    int* m_pAlien0; // 0x9c
    int* m_pAlien1; // 0xa0
    int* m_pAlien2; // 0xa4
    int* m_pGeometriesMission; // 0xa8
    int* m_pGeometriesMission2; // 0xac
    int* m_pExplosion; // 0xb0
    int* m_pMission; // 0xb4
    PlayerEgo* m_pPlayerEgo; // 0xb8
    int* m_pAsteroids; // 0xbc
    int* m_pExtra; // 0xc0
    void* m_nParticleSystem0; // 0xc4
    void* m_nParticleSystem1; // 0xc8
    void* f_cc; // 0xcc
    void* f_d0; // 0xd0
    void* f_d4; // 0xd4
    void* f_d8; // 0xd8
    void* f_dc; // 0xdc

    LevelScript(Level *level, Hud *hud, Radar *radar, TargetFollowCamera *camera);
    ~LevelScript();

    uint8_t startSequenceOver();
    uint8_t startSequence();
    void resetStartSequenceOver();
    void skipSequence();
    uint32_t canSkipCutsceneNow();
    void lookBehind();
    void resetCamera(Level *level);
    void render3D();
    void setAutoPilotToProgrammedStation();
    void skipCutscene();
    void process(int delta);
};

static inline char *B(void *self, int offset) { return (char *)self + offset; }
static inline int &I(void *self, int offset) { return *(int *)((char *)self + offset); }
static inline uint32_t &U(void *self, int offset) { return *(uint32_t *)((char *)self + offset); }
static inline float &F(void *self, int offset) { return *(float *)((char *)self + offset); }
static inline uint8_t &UC(void *self, int offset) { return *(uint8_t *)((char *)self + offset); }
static inline uint16_t &US(void *self, int offset) { return *(uint16_t *)((char *)self + offset); }
static inline void *&P(void *self, int offset) { return *(void **)((char *)self + offset); }

#endif
