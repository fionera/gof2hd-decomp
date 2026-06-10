#ifndef GOF2_WANTEDWINDOW_H
#define GOF2_WANTEDWINDOW_H
#include "gof2/common.h"
// Galaxy on Fire 2 - WantedWindow (bounty-board UI). Top-level class. Real named struct
// (field_0xNN member names kept for cross-class refs). The recovered bodies still reach into
// sibling engine objects (Layout, the engine Array container) by raw byte offset; F<T>(p,off)
// is the typed byte-offset accessor used for those foreign reads.
//
// 16-byte SIMD lane (the ctor zero-fills field_0x91..0xa0 with one vector store).
typedef int v4si __attribute__((vector_size(16)));

// Typed byte-offset accessor into a foreign (non-WantedWindow) object: F<T>(p, off) == *(T*)(p+off).
template <class T>
static inline T &F(void *p, unsigned off) { return *(T *)((char *)p + off); }

struct WantedWindow;
struct Wanted;
struct Station;
struct Mission;
struct StarMap;
struct ScrollTouchWindow;
struct TouchButton;
struct Layout;
struct PaintCanvas;
struct ImageFactory;






static inline int *IArrayData(void *arr) {
    return F<int *>(arr, 4);

    // ---- methods (converted from free functions) ----
    int OnTouchBegin(int x, int y);
    void OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    void draw();
    float getRelativeScrollHeight();
    float getRelativeScrollStartPos();
    uint32_t getWantedAtPosition(int x, int y);
    int init();
    void render3D();
    void selectWanted(int idx);
    void update(int dt);
}

static inline void *ArrayItem(void *arr, int idx) {
    return *(void **)((char *)F<void *>(arr, 4) + idx * 4);
}

struct Blk16 { int a, b, c, d; };

extern "C" void *operator_new(unsigned size);
extern "C" void operator_delete(void *p);

struct WantedWindow {
    uint32_t field_0x0;                 // +0x0
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    void* field_0xc;                    // +0xc
    int field_0x10;                     // +0x10
    uint8_t field_0x14;                 // +0x14
    uint8_t field_0x15;                 // +0x15
    void* field_0x18;                   // +0x18
    int field_0x1c;                     // +0x1c
    int field_0x20;                     // +0x20
    int field_0x24;                     // +0x24
    int field_0x28;                     // +0x28
    void* field_0x2c;                   // +0x2c
    uint32_t field_0x30;                // +0x30
    uint32_t field_0x34;                // +0x34
    void* field_0x38;                   // +0x38
    int field_0x84;                     // +0x84
    int field_0x88;                     // +0x88
    int field_0x8c;                     // +0x8c
    int field_0x90;                     // +0x90
    v4si field_0x91;                    // +0x91
    float field_0x94;                   // +0x94
    float field_0x98;                   // +0x98
    int field_0x9c;                     // +0x9c
    uint8_t field_0xa0;                 // +0xa0
    int field_0xa4;                     // +0xa4
    int field_0xa8;                     // +0xa8
    int field_0xac;                     // +0xac
    void* field_0xb0;                   // +0xb0
};
#endif
