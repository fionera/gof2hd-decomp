#ifndef GOF2_MISSIONSWINDOW_H
#define GOF2_MISSIONSWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MissionsWindow (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace) -- the qualified target name is
// "MissionsWindow::...". Field offsets are recovered per-method from the target
// disassembly; we do NOT model a full layout -- fields are reached via byte-offset casts.


void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

// Field accessors via byte offset.

// 16-byte aggregate cleared in the ctor (4 ints).
#ifndef GOF2_BLK16_DEFINED
#define GOF2_BLK16_DEFINED
struct __attribute__((aligned(4))) Blk16 { int a, b, c, d; };
#endif

class MissionsWindow {
public:
    Blk16 field_0xc;                    // +0xc
    MissionsWindow();
    ~MissionsWindow();
    int OnTouchMove(int, int);
    int OnTouchBegin(int, int);
    void OnTouchEnd(int y, int z);
    void setHangarUpdate(bool);
    uint8_t hangarNeedsUpdate();
    void render3D();
    int init();
    void draw();
    void update(int dt);

    // Mode-specific sub-window dispatchers, reached from draw()/update() when an
    // overlay is active. drawWanted/acceptAction drive the wanted-board window
    // (m_pWantedWindow @ +0x10, active when m_mode @ +0x40 == 1); drawStarMap/
    // cancelAction drive the star-map overlay (m_pStarMap @ +0x8, active when the
    // star-map flag @ +0x22 is set).
    void drawWanted();      // paint the active wanted-board window
    void drawStarMap();     // paint the active star-map overlay
    void acceptAction();    // advance the wanted-board window one frame
    void cancelAction();    // advance the star-map overlay one frame
};
#endif
