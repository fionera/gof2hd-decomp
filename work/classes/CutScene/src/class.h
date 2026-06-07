#ifndef WORK_CLASSES_CUTSCENE_SRC_CLASS_H
#define WORK_CLASSES_CUTSCENE_SRC_CLASS_H

// Galaxy on Fire 2 -- CutScene class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Field offsets recovered per-method from the target
// disassembly; accessed via byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef int int32_t;

struct AEGeometry;
struct Level;

struct CutScene {
    CutScene(int param);
    ~CutScene();

    uint8_t isInitialized();
    void render2D();
    void renderBG();
    void render3D();
    void update();
    void initialize();
    void resetCamera();
    void checkForTurret();
    void replacePlayerShip(int a, int b);
    void process(int delta);
};

// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
