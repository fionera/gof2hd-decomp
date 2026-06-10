#ifndef GOF2_VECTOR_H
#define GOF2_VECTOR_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine::AEMath::Vector (Android libgof2hdaa.so, armv7 Thumb).
// A 3-component float vector. The recovered disassembly for these operators sits in a
// corrupted/overlapping bad-instruction region; per coverage mode we author the faithful
// component-wise semantics implied by each operator's signature and the s0/s2/s4 + [0,4,8]
// VFP load/store pattern (three contiguous floats).


namespace AbyssEngine {
namespace AEMath {



} // namespace AEMath
} // namespace AbyssEngine

struct Vector { void* _opaque; };  // no offset accesses observed
#endif
