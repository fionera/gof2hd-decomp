#ifndef GOF2_STATUSWINDOW_H
#define GOF2_STATUSWINDOW_H
#include "gof2/common.h"
// ImageFactory.h provides the ImageFactory class and forward-declares ImagePart, which the
// imageParts member (Array<ImagePart*>*) and drawChar() path need.
#include "gof2/engine/render/ImageFactory.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- StatusWindow (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "StatusWindow::..." (class not in a namespace;
// argument types like AbyssEngine::String / Array<...> are namespaced/templated).
// Field offsets recovered per-method from the target disassembly. All members declared
// in offset order; semantics derived from ctor init / usage / Ghidra. StatusWindow shares
// its leading layout region with ScrollTouchWindow (see ScrollTouchWindow.h) but is modeled
// as a flat class here, matching the rest of the offset-access campaign.

class TouchButton;

typedef AbyssEngine::String String;

class StatusWindow {
public:
    int medalCount;                      // +0x00  number of medal buttons (== 0x2d)
    Array<TouchButton*>* tabButtons;     // +0x04
    Array<TouchButton*>* medalButtons;   // +0x08
    Array<ImagePart*>* imageParts;       // +0x0c  (glyph image tiles for drawChar)
    Array<String*>* detailLines;         // +0x10
    unsigned rankImage0;                 // +0x14  rank/medal tile image handle
    unsigned rankImage1;                 // +0x18  rank/medal tile image handle
    unsigned rankImage2;                 // +0x1c  rank/medal tile image handle
    unsigned rankImage3;                 // +0x20  rank/medal tile image handle
    unsigned standingEmblemImage;        // +0x24  standing emblem image handle
    unsigned standingBarImage;           // +0x28  standing-bar fill image handle
    unsigned standingFrameImage;         // +0x2c  standing-bar frame image handle
    unsigned activeTab;                  // +0x30  active tab index
    int selectedMedal;                   // +0x34  selected medal index (-1 == none)
    int scrollOffset;                    // +0x38  current scroll offset
    int lastTouchY;                      // +0x3c  last drag touch Y
    int scrollTarget;                    // +0x40  scroll offset target
    int scrollVelocity;                  // +0x44  integer scroll velocity (Y)
    float scrollDamping;                 // +0x48  inertia damping factor
    float scrollVelocityF;               // +0x4c  float scroll velocity
    int touchStartY;                     // +0x50  touch-begin Y
    unsigned char isDragging;            // +0x54  drag-in-progress flag
    int contentHeight;                   // +0x58  current-tab content height
    int viewportHeight;                  // +0x5c  scrollable viewport height
    int charImageWidth;                  // +0x60  credit-glyph image width
    int charImageHeight;                 // +0x64  credit-glyph image height
    int* tabHeights;                     // +0x68  per-tab scroll content heights
    int boxWidth;                        // +0x6c  content column / box width
    int standingBarWidth;                // +0x70  standing bar half-width
    int standingBarHeight;               // +0x74  standing bar height
    int medalRowHeight;                  // +0x78  medal grid row height

    // ---- methods (converted from free functions) ----
    ~StatusWindow();
    int OnTouchBegin(int param_1, int param_2);
    void OnTouchEnd(int x, int y);
    int OnTouchMove(int param_1, int param_2);
    StatusWindow * ctor();
    void draw();
    float getRelativeScrollHeight();
    float getRelativeScrollStartPos();
    void reInit();
    void update();
};
#endif
