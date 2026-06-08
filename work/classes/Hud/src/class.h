// Hud — top-level class (NO namespace). Byte-exact decomp scaffold.
// We do NOT model the full layout; fields are accessed by byte offset taken
// from each work-item's target disasm.
#ifndef HUD_CLASS_H
#define HUD_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

struct Hud {
    unsigned char _pad_0[24];
    void* f_18; // 0x18
    unsigned char _pad_1c[514];
    uint8_t f_21e; // 0x21e
    unsigned char _pad_21f[2];
    uint8_t f_221; // 0x221
    unsigned char _pad_222[22];
    void* f_238; // 0x238
    unsigned char _pad_23c[28];
    void* f_258; // 0x258
    void* f_25c; // 0x25c
    unsigned char _pad_260[4];
    void* f_264; // 0x264
    unsigned char _pad_268[4];
    uint8_t f_26c; // 0x26c
    unsigned char _pad_26d[11];
    uint8_t f_278; // 0x278
    unsigned char _pad_279[1];
    uint8_t f_27a; // 0x27a
    unsigned char _pad_27b[17];
    void* f_28c; // 0x28c
    void* f_290; // 0x290
    unsigned char _pad_294[668];
    void* f_530; // 0x530
    void* f_534; // 0x534
};            // opaque; we only ever take a Hud* and offset-cast.

// Other game types referenced by pointer only — keep opaque.
struct Item;
struct ListItem;
struct TouchButton;
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

#endif
