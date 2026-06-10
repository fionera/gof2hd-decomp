#ifndef GOF2_PLAYEREGO_H
#define GOF2_PLAYEREGO_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// PlayerEgo — top-level class (NO namespace). Byte-exact decomp scaffold.
// We do NOT model the full layout; fields are accessed by byte offset taken
// from each work-item's target disasm.

struct PlayerEgo;          // opaque; we only ever take a PlayerEgo* and offset-cast.

// Other game types referenced by pointer only — keep opaque.
// (Player/Ship/etc. forward-declared in fwd.h via common.h; Vector comes from math.h.)

// ---- tiny offset-cast helpers -------------------------------------------------
static inline char*           B(void* p, int off) { return (char*)p + off; }
static inline int&            I (void* p, int off) { return *(int*)((char*)p + off); }
static inline unsigned int&   U (void* p, int off) { return *(unsigned int*)((char*)p + off); }
static inline float&          F (void* p, int off) { return *(float*)((char*)p + off); }
static inline double&         D (void* p, int off) { return *(double*)((char*)p + off); }
static inline char&           C (void* p, int off) { return *(char*)((char*)p + off); }
static inline unsigned char&  UC(void* p, int off) { return *(unsigned char*)((char*)p + off); }
static inline bool&           BL(void* p, int off) { return *(bool*)((char*)p + off); }
static inline short&          S (void* p, int off) { return *(short*)((char*)p + off); }
static inline void*&          P (void* p, int off) { return *(void**)((char*)p + off); }

struct PlayerEgo {
    void* field_0x0;                    // +0x0
    AEGeometry* field_0x4;              // +0x4
    AEGeometry* field_0x8;              // +0x8
    Level* field_0xc;                   // +0xc
    LevelScript* field_0x10;            // +0x10
    uint8_t field_0x25;                 // +0x25
    void* field_0x15c;                  // +0x15c
    unsigned short field_0x388;         // +0x388
    unsigned short field_0x38c;         // +0x38c
    unsigned short field_0x390;         // +0x390

    // Real C++ ctor/dtor so the demangled symbols match the binary.
    PlayerEgo(Player* player);
    ~PlayerEgo() noexcept(false);
};
#endif
