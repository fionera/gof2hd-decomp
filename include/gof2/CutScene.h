#ifndef GOF2_CUTSCENE_H
#define GOF2_CUTSCENE_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- CutScene class (intro / station / docking cinematics).
// Top-level class. Most field names were not individually recovered, so the method
// bodies read/write fields through the byte-offset helpers below (offsets 0x0..0x88).

struct AEGeometry;
struct Level;

// Field accessors via byte offset.

struct CutScene {
    // raw field storage (offsets referenced through the i32/u32/u8/f32/pp helpers above)
    char field_storage[0x90];

    CutScene(int param);
    ~CutScene();

    uint8_t isInitialized();
    void update();
    void render3D();
    void render2D();
    void renderBG();
    void process(int delta);
    void replacePlayerShip(int a, int b);
    void initialize();
    void resetCamera();
    void checkForTurret();
};

#endif
