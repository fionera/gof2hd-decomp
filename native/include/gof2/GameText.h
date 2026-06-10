#ifndef GOF2_GAMETEXT_H
#define GOF2_GAMETEXT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- GameText (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "GameText::..." (class not in a namespace;
// argument types like AbyssEngine::String are namespaced).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.
//
// Layout (from ctor/release decompiles): GameText embeds an Array<int> substitute table at +0x00
//   +0x00  uint32_t   substitute Array size
//   +0x04  int*       substitute Array data
//   +0x08  uint32_t   substitute Array cap
//   +0x0c  String**   text table (operator new[]; size at +0x1c)
//   +0x10  String     fallback / region string (12 bytes)
//   +0x1c  int        text table count


void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

struct GameText;

// AbyssEngine::String -- 12-byte value type (text*, size, ...). Pass-by-value uses the
// >8-byte struct ABI (sret / on stack), matching the target.
namespace AbyssEngine {

}
typedef AbyssEngine::String String;

// Field accessors via byte offset.

static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct GameText {
    void* field_0x4;                    // +0x4
    void* field_0xc;                    // +0xc
};
#endif
