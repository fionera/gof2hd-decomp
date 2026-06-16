#ifndef GOF2_VECTOR_H
#define GOF2_VECTOR_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
// Galaxy on Fire 2 -- AbyssEngine::AEMath::Vector (Android libgof2hdaa.so, armv7 Thumb).
// A 3-component float vector. The canonical definition lives in gof2/math.h (struct Vector with
// float x,y,z and the arithmetic operators). The offset-access "field_0xNN" struct that the
// generator emitted here was spurious (it described an unrelated object), so we defer to the real
// math Vector. The operator bodies are provided by native/src/Vector.cpp.

#endif
