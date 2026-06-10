#ifndef GOF2_CUTSCENE_H
#define GOF2_CUTSCENE_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- CutScene class (intro / station / docking cinematics).
// Top-level class. Most field names were not individually recovered, so the method
// bodies read/write fields through the byte-offset helpers below (offsets 0x0..0x88).

struct AEGeometry;
struct Level;

// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

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
