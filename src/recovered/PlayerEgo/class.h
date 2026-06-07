// PlayerEgo — top-level class (NO namespace). Byte-exact decomp scaffold.
// We do NOT model the full layout; fields are accessed by byte offset taken
// from each work-item's target disasm.
#ifndef PLAYEREGO_CLASS_H
#define PLAYEREGO_CLASS_H

struct PlayerEgo;          // opaque; we only ever take a PlayerEgo* and offset-cast.

// Other game types referenced by pointer only — keep opaque.
struct Player;
struct Ship;
struct Item;
struct Route;
struct HUD;
struct Camera;
struct Vector;

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

#endif
