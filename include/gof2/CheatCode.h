#ifndef GOF2_CHEATCODE_H
#define GOF2_CHEATCODE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

namespace AbyssEngine {





} // namespace AbyssEngine

struct CheatCode { void* _opaque; };  // no offset accesses observed
#endif
