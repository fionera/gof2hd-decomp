#ifndef GOF2_CUTSCENE_H
#define GOF2_CUTSCENE_H
#include "common.h"

// Galaxy on Fire 2 -- CutScene: intro / station / docking cinematics.
// Owns a self-contained cinematic Level plus the cameras, transforms and
// decorative geometry that frame it.

class AEGeometry;            // engine/render/AEGeometry.h; used by pointer only
class TargetFollowCamera;    // game/ship/TargetFollowCamera.h; used by pointer only
class PlayerEgo;             // game/ship/PlayerEgo.h; used by pointer only

struct Level;

class CutScene {
public:
    Level                  *level;          // self-contained cinematic level
    float                   cameraRotX;     // camera X-rotation accumulator
    Vector                  vec8;           // zero-initialized vector blob
    float                   field_0x14;     // fourth word of the zero-init block
    int                     field_0x18;
    int                     field_0x1c;
    AEGeometry             *rootGeom;       // scene root geometry
    float                   rotationSpeed;  // per-frame rotation factor
    AEGeometry             *geom28;         // cinematic geometry
    AEGeometry             *geom2c;         // cinematic geometry
    AEGeometry             *geom30;         // cinematic geometry
    AEGeometry             *geom34;         // cinematic geometry
    Array<AEGeometry *>    *geometries;     // owned AEGeometry array
    uint32_t                field_0x40;     // current time low word
    uint32_t                field_0x44;     // current time high word
    uint32_t                prevTimeLo;     // previous-frame time low word
    uint32_t                field_0x4c;     // previous-frame time high word
    uint32_t                accumLo;        // 64-bit elapsed accumulator low
    uint32_t                accumHi;        // 64-bit elapsed accumulator high
    uint32_t                frameDelta;     // current frame delta (ms)
    uint8_t                 initialized;    // isInitialized() flag
    PlayerEgo              *player;         // level player
    AEGeometry             *turretGeom;     // turret root geometry
    TargetFollowCamera     *followCamera;
    uint32_t                cameraId6c;     // camera handle (mode 4)
    uint32_t                cameraId70;     // camera handle (mode 0x17)
    uint32_t                cameraId74;     // camera handle (mode 2)
    uint32_t                transformId78;  // transform handle (mode 0x17)
    int                     animTimer7c;    // animation timer
    int                     animTimer80;    // animation timer
    int                     fogTimer84;     // fog/anim timer
    int                     mode;           // cutscene mode (ctor param)

    CutScene(int mode);
    ~CutScene();

    uint8_t isInitialized();
    void update();
    void update_tail();
    void render3D();
    void render2D();
    void render2D_tail(Level *level);
    void renderBG();
    void renderBG_tail(Level *level, uint32_t t);
    void process(int delta);
    void replacePlayerShip(int a, int b);
    void initialize();
    void resetCamera();
    void checkForTurret();
    void processAux();
    void turretFinalize(void *positions);
};

#endif
