#ifndef WORK_CLASSES_PARTICLESETTINGSREF_SRC_CLASS_H
#define WORK_CLASSES_PARTICLESETTINGSREF_SRC_CLASS_H

// ParticleSettingsRef — top-level class (no namespace per Sig).
// initialize() is a static-like routine touching globals only; no `this` fields used.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct ParticleSettingsRef;

template <class T>
static inline T &F(void *self, uint32_t off) { return *(T *)((char *)self + off); }

#endif
