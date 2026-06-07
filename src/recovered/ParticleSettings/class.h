#ifndef WORK_CLASSES_PARTICLESETTINGS_SRC_CLASS_H
#define WORK_CLASSES_PARTICLESETTINGS_SRC_CLASS_H

// Galaxy on Fire 2 — ParticleSettings class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace per the work-item Sig lines), though
// it embeds AbyssEngine::String members. It is an array of 48 "ParticleSet" entries,
// each 0xa0 bytes, laid out contiguously from offset 0 (total 0x1e00 bytes). Every
// ParticleSet begins with an AbyssEngine::String, so the ctor/dtor walk the array in
// 0xa0 strides constructing / destructing those strings.
// Fields are reached via byte-offset casts from `this`/self (r0 == first arg).

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct ParticleSettings {
    ParticleSettings();
    ~ParticleSettings();
};

// Field accessor via byte offset.
template <class T>
static inline T &F(void *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
