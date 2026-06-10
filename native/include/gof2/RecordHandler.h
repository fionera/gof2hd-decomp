#ifndef GOF2_RECORDHANDLER_H
#define GOF2_RECORDHANDLER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// RecordHandler — top-level class (NO namespace). Byte-exact decomp scaffold.
// Save/record I/O. Fields accessed by byte offset from each work-item's target disasm.


struct RecordHandler;     // opaque; only ever a RecordHandler* + offset-cast.

// Game types referenced by pointer only — keep opaque.
struct Agent;
struct Mission;
struct Wanted;
struct GameRecord;
struct String;

// ---- tiny offset-cast helpers -------------------------------------------------
static inline char*           B (void* p, int off) { return (char*)p + off; }
static inline int&            I (void* p, int off) { return *(int*)((char*)p + off); }
static inline unsigned int&   U (void* p, int off) { return *(unsigned int*)((char*)p + off); }
static inline float&          F (void* p, int off) { return *(float*)((char*)p + off); }
static inline double&         D (void* p, int off) { return *(double*)((char*)p + off); }
static inline char&           C (void* p, int off) { return *(char*)((char*)p + off); }
static inline unsigned char&  UC(void* p, int off) { return *(unsigned char*)((char*)p + off); }
static inline bool&           BL(void* p, int off) { return *(bool*)((char*)p + off); }
static inline short&          S (void* p, int off) { return *(short*)((char*)p + off); }
static inline unsigned short& US(void* p, int off) { return *(unsigned short*)((char*)p + off); }
static inline void*&          P (void* p, int off) { return *(void**)((char*)p + off); }

struct RecordHandler {
    void* field_0x4;                    // +0x4
};
#endif
