#ifndef GOF2_SPARKS_H
#define GOF2_SPARKS_H
#include "gof2/common.h"
#include "gof2/math.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)

class Sparks {
public:
    int* lifetimeThresholds;                     // +0x0   per-spark lifetime thresholds
    uint32_t spriteSystem;                 // +0x4   sprite system handle
    uint32_t texture;                 // +0x8   texture handle
    int elapsed;                      // +0xc   elapsed time
    uint8_t active;                 // +0x10  active flag
    int kind;                     // +0x14  kind
    uint32_t count;                // +0x18  spark count
    int lifetime;                     // +0x1c  lifetime
    int totalThreshold;                     // +0x20  total threshold

    Sparks(int kind);
    ~Sparks();
    void translate(AbyssEngine::AEMath::Vector const &v);
    void explode(AbyssEngine::AEMath::Vector const &v);
    void explode(int x, int y, int z);
    bool isRocket();
    void update(int step);
    void render();
};
#endif
