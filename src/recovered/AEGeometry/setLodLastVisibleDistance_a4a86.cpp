#include "class.h"

// AEGeometry::setLodLastVisibleDistance(unsigned long long)
//   *(u64*)(this+0x70) = d * d   (umull + 2x mla for the high word)
struct AEGeometry { void setLodLastVisibleDistance(uint64_t d); };
void AEGeometry::setLodLastVisibleDistance(uint64_t d)
{
    *(uint64_t *)((char *)this + 0x70) = d * d;
}
