#ifndef WORK_CLASSES_REPAIRBEAM_SRC_CLASS_H
#define WORK_CLASSES_REPAIRBEAM_SRC_CLASS_H

// RepairBeam — top-level class (only arg types are namespaced).
// Layout recovered from ctor/dtor/render/update decompiles:
//   +0x00  int    shipIndex      (used to index a sound-event global array)
//   +0x04  Vector beamPosition   (x@+0x4, y@+0x8, z@+0xc)
//   +0x10  Array<AEGeometry*>* geoms
//   +0x14  Array<int>*           targetIds   (size@[0], data@[1])
//   +0x18  Array<float>*         charges     (size@[0], data@[1])
//   +0x1c  int    sort           (0x25 heal / 0x29 shield)
//   +0x20  int    timer

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);

struct RepairBeam;

namespace AbyssEngine {
namespace AEMath {
struct Vector { float x, y, z; };
}
}
using Vector = AbyssEngine::AEMath::Vector;

template <class T>
static inline T &F(void *self, uint32_t off) { return *(T *)((char *)self + off); }

#endif
