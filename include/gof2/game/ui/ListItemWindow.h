#ifndef GOF2_LISTITEMWINDOW_H
#define GOF2_LISTITEMWINDOW_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- ListItemWindow (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace) -- the qualified target name is
// "ListItemWindow::...". Instance layout recovered from the byte-offset accesses
// in the merged source. Members are now real named/typed fields (binary offsets
// shown in "// +0xNN" comments). This is the non-byte-matching native build, so
// the comment offsets are documentation only -- the embedded String/Matrix
// members use the native (wider) layout, not the 12/60-byte binary forms.
//
// Offsets 0x10..0x70 are the embedded window base-region (geometry/colour state
// shared with the window framework); they are modelled inline here as named
// members because the recovered class has no separate base header.

class ListItemWindow {
public:
    // +0x0  label-row String* array (owns its elements).
    Array<AbyssEngine::String *> *labels;
    // +0x4  value-row String* array (owns its elements).
    Array<AbyssEngine::String *> *values;
    // +0x8  current ship-stat int array (ship items only).
    Array<int> *statsCur;
    // +0xc  previous ship-stat int array (ship items only).
    Array<int> *statsPrev;

    // +0x10 rotating 3D ship-preview geometry.
    AEGeometry *previewGeometry;
    // +0x14 the ListItem this window is displaying.
    void *item;
    // +0x18 inner scroll window hosting the value rows (base sub-window).
    void *scrollWindow;
    // +0x1c half text-height (set from canvas text height in the ctor); row Y.
    int textHalfHeight;
    // +0x20 height of the ship-preview / progress region (0 for non-ships).
    int previewHeight;
    // +0x24 scrollbar/footer geometry (computed in the row-fill helper).
    int field_0x24;
    // +0x28 scrollbar/footer geometry.
    int field_0x28;
    // +0x2c scrollbar/footer geometry.
    int field_0x2c;
    // +0x30 scrollbar track length (split into thirds when drawing the thumb).
    int field_0x30;
    // +0x34 mode/colour parameter 2 (from set()).
    uint32_t param2;
    // +0x38 mode/colour parameter 3 (from set()).
    uint32_t param3;
    // +0x3c mode/colour parameter 4 (from set()).
    uint32_t param4;
    // +0x40 mode/colour parameter 5 (from set()).
    uint32_t param5;
    // +0x44 scrollbar thumb image id.
    int scrollThumbImage;
    // +0x48 "trend up" arrow image id.
    int arrowUpImage;
    // +0x4c "trend down" arrow image id.
    int arrowDownImage;
    // +0x50 "trend equal" arrow image id.
    int arrowEqualImage;
    // +0x54 shows-3D-ship flag (true => render the rotating preview).
    uint8_t shows3DShipFlag;
    // +0x60 trend-arrow separator width.
    int arrowSeparator;
    // +0x64 window x.
    int x;
    // +0x68 window y.
    int y;
    // +0x6c window width.
    int width;
    // +0x70 window height.
    int height;
    // +0x74 embedded String (window-scoped scratch string).
    AbyssEngine::String str74;
    // +0x80 embedded String (window-scoped scratch string).
    AbyssEngine::String str80;
    // +0x90 preview state sentinel (-1 => no secondary transform pass).
    int previewSentinel;
    // +0x98 preview world transform matrix (camera-local).
    AbyssEngine::AEMath::Matrix previewTransform;
    // +0xd4 preview secondary transform matrix.
    AbyssEngine::AEMath::Matrix previewTransform2;
    // +0x114 per-ship base preview angle.
    float baseAngle;
    // +0x118 current preview rotation angle (radians).
    float previewAngle;
    // +0x11c accumulated drag angle (integer steps).
    int dragAccum;
    // +0x120 last touch x captured for drag delta.
    int dragLastX;
    // +0x124 settled drag angle.
    int dragSettled;
    // +0x128 current drag delta.
    int dragDelta;
    // +0x12c spin-damping factor.
    float spinDamping;
    // +0x130 spin velocity.
    float spinVelocity;
    // +0x134 drag start x.
    int dragStartX;
    // +0x138 dragging flag (true while the preview is being spun by touch).
    uint8_t dragging;

    ListItemWindow();
    ~ListItemWindow();
    void OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
    uint8_t shows3DShip();
    void render();
    void set(void *item, unsigned p2, unsigned p3, unsigned p4, unsigned p5, bool p6);
    void draw();
    void update(int frameTime);

    // C-ABI destructor helper: runs ~ListItemWindow and returns `this` so the
    // caller can free the storage (matches the ARM deleting-dtor thunk).
    void *dtor();
    // Touch-release entry point used by the owning screen; equivalent to
    // OnTouchEnd, returning an int status the original caller ignores.
    int touch_end(int x, int y);
};

#endif
