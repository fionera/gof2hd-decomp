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
