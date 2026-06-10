#ifndef GOF2_BSPHERE_H
#define GOF2_BSPHERE_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct BSphere {
    float cx, cy, cz;   // 0x00, 0x04, 0x08
    float radius;       // 0x0c
    float maxRadius;    // 0x10

    BSphere &operator=(const BSphere &other);
    void Merge(const Transform &t);
    void Merge(const BSphere &other);
};
#endif
