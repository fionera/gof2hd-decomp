#ifndef GOF2_SIMPLEMESHMERGER_H
#define GOF2_SIMPLEMESHMERGER_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// SimpleMeshMerger — top-level class (only arg types are namespaced: AbyssEngine::Mesh,
// AEMath::Matrix/Vector, PaintCanvas).
// Layout recovered from the constructor disassembly:
//   +0x00  int    (first Array<Matrix> word copied from param)
//   +0x04  short  mergeFactor (param_5 truncated)
//   +0x06  uint8_t valid flag (set to 1 at end)
//   +0x08  Array<Mesh*> header { size@+0x08, data@+0x0c, size2@+0x10 }
//   +0x14  PaintCanvas* canvas
//   +0x18  uint   mergedMeshId
//   +0x1c  uint   transformId


void *operator new(__SIZE_TYPE__ size);

struct SimpleMeshMerger {
    // @portable-fields
    int f_0; // 0x0
    short f_4; // 0x4  mergeFactor
    unsigned char field_0x6; // 0x6  valid flag
    unsigned char _pad_7;    // 0x7
    unsigned char _pad_8[4];
    void* f_c; // 0xc
    unsigned char _pad_10[4];
    void* f_14; // 0x14
    int f_18; // 0x18
    int f_1c; // 0x1c
};

namespace AbyssEngine {
namespace AEMath {


}
}
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;
#endif
