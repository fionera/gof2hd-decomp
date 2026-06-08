#ifndef WORK_CLASSES_SIMPLEMESHMERGER_SRC_CLASS_H
#define WORK_CLASSES_SIMPLEMESHMERGER_SRC_CLASS_H

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

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);

struct SimpleMeshMerger;

namespace AbyssEngine {
namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { float m[15]; };
}
}
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

template <class T>
static inline T &F(void *self, uint32_t off) { return *(T *)((char *)self + off); }

#endif
