#include "class.h"

// ListItemWindow::set(ListItem*, unsigned, unsigned, unsigned, unsigned, bool)
//
//   Re-targets the window onto a new ListItem. The method:
//     * stores the item pointer and the four mode/colour parameters,
//     * picks the window width/height from the current language + fullscreen
//       state and applies them via Layout::setWindowDimensions,
//     * tears down and rebuilds the two String arrays (labels at +0x0, values
//       at +0x4) plus, for ships, the int arrays (+0x8 / +0xc),
//     * for a ship item, builds the rotating 3D preview (transform, mesh,
//       texture, camera, lighting) -- this block is dense FP/transform setup,
//       delegated to liw_set_buildShipPreview,
//     * constructs the inner ScrollTouchWindow,
//     * fills the label/value rows: ship stats, or the per-attribute item stat
//       table, or a blueprint/product price block -- delegated to
//       liw_set_fillRows, which owns the large attribute switch,
//     * resets the preview spin state.
//
// The recoverable scaffolding is authored directly; the two heavy data-driven
// regions are reached through helpers that take the window, the item and the
// already-resolved layout pointer.

// ---- String / Array runtime ---------------------------------------------------
extern "C" void  Array_releaseClasses(void *arr);     // ArrayReleaseClasses<String*>
extern "C" void *Array_dtor(void *arr);               // Array<String*>::~Array
extern "C" void  Array_ctor(void *arr);               // Array<T>::Array()
extern "C" void  operator_delete_(void *p);

// ---- callees ------------------------------------------------------------------
extern "C" void  Layout_setWindowDimensions(void *layout, int x, int y, int w, int h);
extern "C" int   ListItem_isShip(void *li);
extern "C" int   ListItem_isItem(void *li);
extern "C" int   ListItem_isBluePrint(void *li);
extern "C" int   ListItem_isPendingProduct(void *li);
extern "C" void  ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);

// Heavy data-driven regions (own the corrupted FP / attribute code).
extern "C" void  liw_set_buildShipPreview(void *self, void *item, void *layout);
extern "C" void  liw_set_fillRows(void *self, void *item, void *layout,
                                  int isShip, bool param6);

// ---- hidden PC-relative roots -------------------------------------------------
__attribute__((visibility("hidden"))) extern int   *g_liw_s_stackGuard;  // 0x1419b8 stack canary src
__attribute__((visibility("hidden"))) extern char **g_liw_s_fullscreen;  // 0x1419bc -> bool flag
__attribute__((visibility("hidden"))) extern char **g_liw_s_modeFlag;    // 0x1419c0 -> bool (HUD layout)
__attribute__((visibility("hidden"))) extern char **g_liw_s_altFlag;     // 0x1419d4 -> bool
__attribute__((visibility("hidden"))) extern int   *g_liw_s_screenW;     // 0x1419e0 -> screen width
__attribute__((visibility("hidden"))) extern int   *g_liw_s_screenH;     // 0x1419e4 -> screen height
__attribute__((visibility("hidden"))) extern void **g_liw_s_layoutHolder;// 0x1419f8 -> Layout*

// width/height presets (read-only floats/ints in the binary).
extern const float g_liw_s_wFull;      // fullscreen-off width source
extern const float g_liw_s_wAlt;
extern const float g_liw_s_wMode;      // mode-flag width
extern const unsigned int g_liw_s_baseAngle;   // 0x1419fc default preview angle

struct ListItemWindow { void set(void *item, unsigned p2, unsigned p3,
                                 unsigned p4, unsigned p5, bool p6); };

void ListItemWindow::set(void *item, unsigned p2, unsigned p3,
                         unsigned p4, unsigned p5, bool p6)
{
    pp(this, 0x14) = item;
    u32(this, 0x34) = p2;
    u32(this, 0x38) = p3;
    u32(this, 0x3c) = p4;
    u32(this, 0x40) = p5;

    void *layout = *g_liw_s_layoutHolder;

    int w, h, x, y;
    if (*g_liw_s_fullscreen == 0) {
        // borderless: window spans the whole screen origin.
        i32(this, 0x64) = 0;
        i32(this, 0x68) = 0;
        w = *g_liw_s_screenW;   // wired from the two screen-dim globals below
        h = *g_liw_s_screenH;
        i32(this, 0x6c) = w;
        i32(this, 0x70) = h;
        x = 0;
        y = 0;
    } else {
        // pick width/height from the HUD-mode + language flags.
        float wf;
        if (**g_liw_s_modeFlag != 0) {
            h = 0x392;
            wf = g_liw_s_wMode;
        } else {
            bool alt = **g_liw_s_altFlag == 0;
            wf = alt ? g_liw_s_wAlt : g_liw_s_wFull;
            h = alt ? 0x28a : 0x514;
        }
        w = (int)wf;
        i32(this, 0x70) = w;
        i32(this, 0x6c) = h;
        x = (*g_liw_s_screenW >> 1) - (h >> 1);
        y = (*g_liw_s_screenH >> 1) - (w >> 1);
        i32(this, 0x64) = x;
        i32(this, 0x68) = y;
    }
    u32(this, 0x114) = g_liw_s_baseAngle;
    Layout_setWindowDimensions(layout, x, y, h, w);

    // Tear down the label array (+0x0).
    if (pp(this, 0x0) != 0) {
        Array_releaseClasses(pp(this, 0x0));
        if (pp(this, 0x0) != 0)
            operator_delete_(Array_dtor(pp(this, 0x0)));
    }
    pp(this, 0x0) = 0;
    // Tear down the value array (+0x4).
    if (pp(this, 0x4) != 0) {
        Array_releaseClasses(pp(this, 0x4));
        if (pp(this, 0x4) != 0)
            operator_delete_(Array_dtor(pp(this, 0x4)));
    }
    pp(this, 0x4) = 0;

    void *a0 = operator new(0xc); Array_ctor(a0); pp(this, 0x0) = a0;
    void *a1 = operator new(0xc); Array_ctor(a1); pp(this, 0x4) = a1;

    int isShip = ListItem_isShip(item);
    if (isShip == 0) {
        i32(this, 0x20) = 0;
        u8(this, 0x54) = 0;
    } else {
        // progress bar height + 3D ship preview (transform/camera/light setup).
        char *L = (char *)layout;
        i32(this, 0x20) =
            ((((i32(this, 0x70) - i32(L, 0xc)) - i32(L, 0x10)) - i32(L, 0x20)) - i32(L, 0x24)) / 2
            - i32(L, 0x2c);
        u32(this, 0x118) = g_liw_s_baseAngle;
        u8(this, 0x54) = 1;
        liw_set_buildShipPreview(this, item, layout);

        // ship stat rows need the two int arrays too.
        void *i0 = operator new(0xc); Array_ctor(i0); pp(this, 0x8) = i0;
        void *i1 = operator new(0xc); Array_ctor(i1); pp(this, 0xc) = i1;
    }

    // Inner scroll window, positioned below the preview / header.
    {
        char *L = (char *)layout;
        int progH = i32(this, 0x20);
        int sel = (progH > 0) ? 0x1c : 0x5c;
        int rowH = i32(L, 0x2c);
        void *stw = operator new(0x24);
        int sx = i32(this, 0x64) + rowH + (i32(this, 0x6c) >> 1);
        int sy = i32(L, 0x20) + i32(this, 0x68) + rowH + i32(L, 0xc) + progH + i32(L, sel);
        int sw = (i32(this, 0x6c) >> 1) - i32(L, 0x28);
        int sh = ((i32(this, 0x70) -
                  (i32(L, 0xc) + rowH * 2 + i32(L, 0x20) + progH + i32(L, sel)))
                  - i32(L, 0x10)) - i32(L, 0x24);
        ScrollTouchWindow_ctor(stw, sx, sy, sw, sh, false);
        pp(this, 0x18) = stw;
    }

    // Populate the rows (ship stats / item attribute table / blueprint price).
    liw_set_fillRows(this, item, layout, isShip, p6);

    // Reset the rotating-preview spin state.
    u8(this, 0x138)  = 0;
    u32(this, 0x118) = 0;
    u32(this, 0x11c) = 0x104;
    u32(this, 0x120) = 0;
    u32(this, 0x124) = 0;
    u32(this, 0x128) = 0;
    u32(this, 0x12c) = 0;
    u32(this, 0x130) = 0;
    u32(this, 0x134) = 0;
}
