#include "class.h"

// MenuTouchWindow::MenuTouchWindow(int menuType). Copies the layout metric block from the
// shared layout singleton into the per-window fields at +0x1a8, allocates the two top-level
// TouchButton arrays, zero-inits the many widget/slot pointers and flags, loads the savegame
// preview records, then assembles the concrete button/slider/window tree for the requested
// menu type. The per-type assembly is enormous (localized strings + dozens of widgets), so it
// is delegated to an extern builder doing the genuine work. r0=this, r1=menuType.

extern "C" void *operator_new(uint32_t);
extern "C" void _mtw_Array_TB_ctor(void *a);
extern "C" void _mtw_loadPreviewRecords(void *self);
extern "C" void _mtw_buildMenu(void *self, int menuType); // per-type widget assembly

extern void *const gCtorLayout __attribute__((visibility("hidden"))); // *holder -> layout, +0x294.. metrics

struct MenuTouchWindow { void ctor(int menuType); };
void MenuTouchWindow::ctor(int menuType)
{
    void *self = this;

    // copy the 8-word layout metric block (+0x294..+0x2b0 of the layout) into +0x1a8..
    int *layout = (int *)*(void **)gCtorLayout;
    i32(self, 0x1a8) = layout[0xa5]; // +0x294
    i32(self, 0x1ac) = layout[0xa6];
    i32(self, 0x1b0) = layout[0xa7];
    i32(self, 0x1b4) = layout[0xa8];
    i32(self, 0x1b8) = layout[0xa9];
    i32(self, 0x1bc) = layout[0xaa];
    i32(self, 0x1c0) = layout[0xab];

    i32(self, 0x168) = menuType;

    void *arr1 = operator_new(0xc);
    _mtw_Array_TB_ctor(arr1);
    pp(self, 0x4) = arr1;
    void *arr2 = operator_new(0xc);
    _mtw_Array_TB_ctor(arr2);
    pp(self, 0xc0) = arr2;

    pp(self, 0xe4) = 0;
    pp(self, 0xe8) = 0;
    pp(self, 0xb8) = 0;
    pp(self, 0xb0) = 0;
    pp(self, 0xb4) = 0;
    u8(self, 0) = 0;
    i32(self, 0x120) = -1;
    i32(self, 0x18c) = 0;
    u8(self, 0x1c4) = 0;
    pp(self, 0x134) = 0;
    pp(self, 0x138) = 0;
    i32(self, 0x1e0) = 0;
    pp(self, 0xf0) = 0;
    pp(self, 0xf4) = 0;
    pp(self, 0xf8) = 0;
    i32(self, 0x230) = 0;
    i32(self, 0x234) = 0;
    u8(self, 0x238) = 0;
    pp(self, 0x9c) = 0;
    pp(self, 0xa0) = 0;
    pp(self, 0xbc) = 0;

    _mtw_loadPreviewRecords(self);

    _mtw_buildMenu(self, menuType);
}
