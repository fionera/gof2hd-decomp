#ifndef WORK_CLASSES_MESH_SRC_CLASS_H
#define WORK_CLASSES_MESH_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine::Mesh (Android libgof2hdaa.so, armv7 Thumb).
// (Namespace confirmed from work-item Sig: AbyssEngine::Mesh.)
//
// We do NOT model the full Mesh layout; fields are accessed by byte offset taken from
// each work-item's target disassembly. Known offsets used here:
//   +0x00 (byte) flag           +0x02 (u16)
//   +0x3c..0x50  BSphere (center+radius), with +0x40 swapped with +0x44 on load
//   +0x34  Transform* animation track
//   +0x50  embedded Vector
//   +0x85  (byte) hasAnimation flag

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;

namespace AbyssEngine {

struct Mesh;
struct Transform;

namespace AEMath {
struct BSphere;
struct Vector;
}

} // namespace AbyssEngine

// Templated byte-offset field accessor: F<int>(this, 0x34) etc.
template <class T>
static inline T &F(void *p, unsigned off) { return *(T *)((char *)p + off); }

#endif
