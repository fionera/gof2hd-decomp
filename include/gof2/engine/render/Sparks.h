#ifndef GOF2_SPARKS_H
#define GOF2_SPARKS_H
#include "gof2/common.h"
#include "gof2/math.h"

// Particle burst (rocket impacts / weapon hits) backed by a sprite system.
class Sparks {
public:
    int* lifetimeThresholds;    // per-spark lifetime thresholds
    uint32_t spriteSystem;      // sprite system handle
    uint32_t texture;           // texture handle
    int elapsed;                // elapsed time
    uint8_t active;             // active flag
    int kind;                   // burst kind (0 = hit, 1 = rocket)
    uint32_t count;             // spark count
    int lifetime;               // total lifetime
    int totalThreshold;         // sum of per-spark thresholds

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
