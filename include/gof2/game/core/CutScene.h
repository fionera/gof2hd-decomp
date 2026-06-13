#ifndef GOF2_CUTSCENE_H
#define GOF2_CUTSCENE_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- CutScene class (intro / station / docking cinematics).
// Top-level class. Field names were recovered from the accessor/ctor/dtor bodies
// and Ghidra; offsets in the trailing comments are the original 32-bit layout.

struct AEGeometry;
struct Level;

class CutScene {
public:
    Level                  *level;          // +0x00  embedded cinematic level
    float                   cameraRotX;     // +0x04  camera X-rotation accumulator
    Vector                  vec8;           // +0x08  zero-initialized vector blob
    float                   field_0x14;     // +0x14  (4th word of the zero-init block)
    int                     field_0x18;     // +0x18
    int                     field_0x1c;     // +0x1c
    AEGeometry             *rootGeom;       // +0x20  scene root geometry
    float                   rotationSpeed;  // +0x24  per-frame rotation factor
    AEGeometry             *geom28;         // +0x28  cinematic geometry
    AEGeometry             *geom2c;         // +0x2c  cinematic geometry
    AEGeometry             *geom30;         // +0x30  cinematic geometry
    AEGeometry             *geom34;         // +0x34  cinematic geometry
    Array<AEGeometry *>    *geometries;     // +0x38  owned AEGeometry array
    uint32_t                field_0x40;     // +0x40  current time low word
    uint32_t                field_0x44;     // +0x44  current time high word
    uint32_t                prevTimeLo;     // +0x48  previous-frame time low word
    uint32_t                field_0x4c;     // +0x4c  previous-frame time high word
    uint32_t                accumLo;        // +0x50  64-bit elapsed accumulator low
    uint32_t                accumHi;        // +0x54  64-bit elapsed accumulator high
    uint32_t                frameDelta;     // +0x58  current frame delta (ms)
    uint8_t                 initialized;    // +0x5c  isInitialized() flag
    void                   *player;         // +0x60  PlayerEgo* (level player)
    AEGeometry             *turretGeom;     // +0x64  turret root geometry
    void                   *followCamera;   // +0x68  TargetFollowCamera*
    uint32_t                cameraId6c;     // +0x6c  camera handle (mode 4)
    uint32_t                cameraId70;     // +0x70  camera handle (mode 0x17)
    uint32_t                cameraId74;     // +0x74  camera handle (mode 2)
    uint32_t                transformId78;  // +0x78  transform handle (mode 0x17)
    int                     animTimer7c;    // +0x7c  animation timer
    int                     animTimer80;    // +0x80  animation timer
    int                     fogTimer84;     // +0x84  fog/anim timer
    int                     mode;           // +0x88  cutscene mode (ctor param)

    CutScene(int param);
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
