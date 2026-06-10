#ifndef WORK_CLASSES_PLAYERGASCLOUD_SRC_CLASS_H
#define WORK_CLASSES_PLAYERGASCLOUD_SRC_CLASS_H

// Galaxy on Fire 2 -- PlayerGasCloud (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "PlayerGasCloud::..." (the class is NOT in a
// namespace; only argument types like AbyssEngine::AEMath::Vector are).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;

struct PlayerGasCloud;
struct ParticleSystemManager;
struct AEGeometry;

namespace AbyssEngine {
namespace AEMath {
struct Vector {
    float x, y, z;
};
}
}

// Templated byte-offset field accessor: F<int>(this, 0x138) etc.
template <class T>
static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
