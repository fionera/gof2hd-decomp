#ifndef GOF2_SPARKS_H
#define GOF2_SPARKS_H
#include "gof2/common.h"
#include "gof2/math.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)

struct Sparks {
    int* field_0x0;                     // +0x0   per-spark lifetime thresholds
    uint32_t field_0x4;                 // +0x4   sprite system handle
    uint32_t field_0x8;                 // +0x8   texture handle
    int field_0xc;                      // +0xc   elapsed time
    uint8_t field_0x10;                 // +0x10  active flag
    int field_0x14;                     // +0x14  kind
    uint32_t field_0x18;                // +0x18  spark count
    int field_0x1c;                     // +0x1c  lifetime
    int field_0x20;                     // +0x20  total threshold

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
