#ifndef GOF2_PARTICLESETTINGS_H
#define GOF2_PARTICLESETTINGS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — ParticleSettings class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace per the work-item Sig lines), though
// it embeds AbyssEngine::String members. It is an array of 48 "ParticleSet" entries,
// each 0xa0 bytes, laid out contiguously from offset 0 (total 0x1e00 bytes). Every
// ParticleSet begins with an AbyssEngine::String, so the ctor/dtor walk the array in
// 0xa0 strides constructing / destructing those strings.
// Fields are reached via byte-offset casts from `this`/self (r0 == first arg).




// Field accessor via byte offset.

struct ParticleSettings { void* _opaque; };  // no offset accesses observed
#endif
