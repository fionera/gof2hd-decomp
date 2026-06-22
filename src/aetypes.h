#ifndef GOF2_AETYPES_H
#define GOF2_AETYPES_H

#include <cstdint>
#include "mathtypes.h"

namespace AEMath = AbyssEngine::AEMath; // so global `AEMath::Vector` resolves
using AbyssEngine::AEMath::Vector; // recovered structs use bare Vector/Matrix
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::Quaternion;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;

struct u32x4 {
    uint32_t v[4];
}; // NEON 4x u32 (best-effort, non-matching)

unsigned int AELabelObject(unsigned int glIdentifier, unsigned int name, const char *label);

#endif // GOF2_AETYPES_H
