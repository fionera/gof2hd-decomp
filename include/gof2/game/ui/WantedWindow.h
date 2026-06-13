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
}

static inline void *ArrayItem(void *arr, int idx) {
    return *(void **)((char *)F<void *>(arr, 4) + idx * 4);
}

#ifndef GOF2_BLK16_DEFINED
#define GOF2_BLK16_DEFINED
#ifndef GOF2_BLK16_DEFINED
#define GOF2_BLK16_DEFINED
struct Blk16 { int a, b, c, d; };
#endif
#endif

extern "C" void *operator_new(unsigned size);
extern "C" void operator_delete(void *p);

class WantedWindow {
public:
    uint32_t field_0x0;                 // +0x0
    void* starMap;                    // +0x4
    void* imageParts;                    // +0x8
    void* buttons;                    // +0xc
    int halfTextHeight;                     // +0x10
    uint8_t showingMap;                 // +0x14
    uint8_t field_0x15;                 // +0x15
    void* detailButton;                   // +0x18
    int windowX;                     // +0x1c
    int windowY;                     // +0x20
    int windowWidth;                     // +0x24
    int windowHeight;                     // +0x28
    void* scrollWindow;                   // +0x2c
    uint32_t selectedWanted;                // +0x30
    uint32_t highlightedWanted;                // +0x34
    void* wantedList;                   // +0x38
    int scrollOffset;                     // +0x84
    int lastDragY;                     // +0x88
    int scrollOffsetSnapshot;                     // +0x8c
    int dragDelta;                     // +0x90
    v4si field_0x91;                    // +0x91
    float scrollDamping;                   // +0x94
    float scrollVelocity;                   // +0x98
    int touchStartY;                     // +0x9c
    uint8_t dragging;                 // +0xa0
    int contentHeight;                     // +0xa4
    int visibleHeight;                     // +0xa8
    int bgImage;                     // +0xac
    void* mission;                   // +0xb0

    // ---- methods (converted from free functions) ----
    WantedWindow();
    ~WantedWindow();
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

    // Tail-call forwarders used when the bounty board is showing the star map:
    // each defers to the corresponding StarMap method on the embedded map object.
    static void update_tail(void *starMap, int dt);
    static void render3D_tail(void *starMap);
    static void draw_tail(void *starMap);
};
#endif
