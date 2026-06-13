#include "gof2/game/ui/ListItemWindow.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/game/mission/BluePrint.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/ui/ScrollTouchWindow.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/ui/ListItem.h"
#include "gof2/game/core/PaintCanvasClass.h"   // real PaintCanvas:: methods

extern "C" void _liw_stw_OnTouchBegin(void *stw, int x);
extern "C" void _liw_stw_OnTouchMove(void *stw, int y);
extern "C" void _liw_stw_OnTouchEnd(void *stw, int y);
extern "C" void  ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);
extern "C" void  liw_set_buildShipPreview(void *self, void *item, void *layout);
extern "C" int   aeabi_idiv_(int a, int b);
extern "C" void _liw_Matrix_ctor(void *m);

// Base sub-window at +0x18.
// PC-relative singleton: a pointer to a pointer to the layout/screen object.
__attribute__((visibility("hidden"))) extern int **g_liw_screen;

// ListItemWindow::OnTouchBegin(int, int)
void ListItemWindow::OnTouchBegin(int x, int y)
{
    _liw_stw_OnTouchBegin(pp(this, 0x18), x);
    if (u8(this, 0x54) &&
        i32(this, 0x64) + (i32(this, 0x6c) >> 1) < x) {
        int *obj = *g_liw_screen;
        if (y < i32(this, 0x68) + obj[0xc / 4] + obj[0x20 / 4] + i32(this, 0x20)) {
            i32(this, 0x120) = x;
            i32(this, 0x134) = x;
            u32(this, 0x128) = 0;
            u8(this, 0x138) = 1;
        }
    }
}

// ListItemWindow::shows3DShip() -- raw uint8 getter at +0x54.
uint8_t ListItemWindow::shows3DShip()
{
    return u8(this, 0x54);
}

// Base sub-window at +0x18 (a ScrollTouchWindow*); forward the touch first.

// ListItemWindow::OnTouchMove(int, int)
void ListItemWindow::OnTouchMove(int x, int y)
{
    _liw_stw_OnTouchMove(pp(this, 0x18), x);
    if (u8(this, 0x54) && u8(this, 0x138)) {
        int d = x - i32(this, 0x120);
        i32(this, 0x128) = d;
        u32(this, 0x12c) = 0x3f800000;
        i32(this, 0x11c) = i32(this, 0x11c) + d;
        i32(this, 0x120) = x;
    }
}

// Base sub-window at +0x18; forward the release first.

// ListItemWindow::OnTouchEnd(int, int)
void ListItemWindow::OnTouchEnd(int x, int y)
{
    _liw_stw_OnTouchEnd(pp(this, 0x18), x);
    if (u8(this, 0x54) && u8(this, 0x138)) {
        int dv = i32(this, 0x128);
        int sum = i32(this, 0x11c) + dv;
        float vel = (float)dv;
        int a = dv < 0 ? -dv : dv;
        float v = 0.0f;
        if (a > 3) v = vel;
        f32(this, 0x12c) = 0.9f;
        u8(this, 0x138) = 0;
        i32(this, 0x11c) = sum;
        i32(this, 0x124) = sum;
        f32(this, 0x130) = v;
    }
}

extern "C" void  _liw_Matrix_assign(void *dst, void *src);
extern "C" void  _liw_render_tail(void *c, int a, int h, void *sp);    // 0x1ac428 tail

__attribute__((visibility("hidden"))) extern void **g_liw_r_canvas;
__attribute__((visibility("hidden"))) extern void **g_liw_r_obj;

// ListItemWindow::render()
void ListItemWindow::render()
{
    if (!u8(this, 0x54))
        return;

    PaintCanvas *canvas = (PaintCanvas *)*g_liw_r_canvas;
    canvas->Begin3d();

    int *obj = (int *)*g_liw_r_obj;
    int s = obj[0x128 / 4];
    int h = i32(this, 0x20) - s * 2;
    canvas->EnableClip(
        i32(this, 0x64) + s + (i32(this, 0x6c) >> 1) + obj[0x2c / 4],
        i32(this, 0x68) + s + obj[0xc / 4] + obj[0x20 / 4],
        ((i32(this, 0x6c) >> 1) - (obj[0x2c / 4] + s * 2)) - obj[0x28 / 4],
        h);
    canvas->SetColor((unsigned int)(long)canvas);
    void *m = canvas->CameraGetLocal((unsigned int)(long)canvas);
    _liw_Matrix_assign((char *)this + 0x98, m);
    ((AEGeometry *)pp(this, 0x10))->render();
    canvas->End3d();
    int dummy;
    return _liw_render_tail((void *)canvas, 0, h, &dummy);
}

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

// ---- callees ------------------------------------------------------------------

// Heavy data-driven regions (own the corrupted FP / attribute code).
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
    ((Layout *)(layout))->setWindowDimensions(x, y, h, w);

    // Tear down the label array (+0x0).
    if (this->labels != 0) {
        for (String *s : *this->labels) delete s;
        this->labels->clear();
        delete this->labels;
    }
    this->labels = 0;
    // Tear down the value array (+0x4).
    if (this->values != 0) {
        for (String *s : *this->values) delete s;
        this->values->clear();
        delete this->values;
    }
    this->values = 0;

    this->labels = new Array<String *>();
    this->values = new Array<String *>();

    int isShip = ((ListItem *)(item))->isShip();
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
        this->statsCur  = new Array<int>();
        this->statsPrev = new Array<int>();
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

extern "C" void *_liw_STW_dtor(void *w);

// ListItemWindow::~ListItemWindow()
ListItemWindow::~ListItemWindow()
{
    if (this->labels) {
        for (String *s : *this->labels) delete s;
        this->labels->clear();
        delete this->labels;
        this->labels = 0;
    }
    if (this->values) {
        for (String *s : *this->values) delete s;
        this->values->clear();
        delete this->values;
        this->values = 0;
    }
    if (this->statsCur) {
        this->statsCur->clear();
        delete this->statsCur;
        this->statsCur = 0;
    }
    if (this->statsPrev) {
        this->statsPrev->clear();
        delete this->statsPrev;
        this->statsPrev = 0;
    }
    if (pp(this, 0x18)) operator delete(_liw_STW_dtor(pp(this, 0x18)));
    pp(this, 0x18) = 0;
    ((String *)((char *)this + 0x80))->dtor();
    ((String *)((char *)this + 0x74))->dtor();
}

// Layout / drawing callees (resolved blx targets).
// 8-arg form

// Hidden PC-relative roots.
__attribute__((visibility("hidden"))) extern char *g_liw_d_maskFlag;
__attribute__((visibility("hidden"))) extern void **g_liw_d_canvas;      // 0x143140 -> canvas
__attribute__((visibility("hidden"))) extern void **g_liw_d_layout;      // 0x143162 -> Layout
__attribute__((visibility("hidden"))) extern int   *g_liw_d_headerId;    // 0x1431dc text key
__attribute__((visibility("hidden"))) extern void **g_liw_d_gameText;    // GameText singleton
__attribute__((visibility("hidden"))) extern void **g_liw_d_imageFactory;// 0x1437c8 / 0x1432e4
__attribute__((visibility("hidden"))) extern void **g_liw_d_itemDB;
__attribute__((visibility("hidden"))) extern void **g_liw_d_arrowL;
__attribute__((visibility("hidden"))) extern void **g_liw_d_arrowR;
__attribute__((visibility("hidden"))) extern int   *g_liw_d_scrollLimit;

// ListItemWindow::draw()
//   Renders the list window chrome: background box, header, the ship/item preview
//   (3D ship render or 2D item icon), each visible list row with its value text
//   and up/down trend arrows, and the footer / scrollbar.
void ListItemWindow::draw()
{
    void *layout = *g_liw_d_layout;
    uint32_t canvas = (uint32_t)(unsigned long)*g_liw_d_canvas;
    bool masked = *g_liw_d_maskFlag != 0;

    if (masked)
        ((Layout *)(layout))->drawMask();

    ((PaintCanvas *)(long)canvas)->SetColor(canvas);
    ((PaintCanvas *)(long)canvas)->FillRectangle((int)canvas, i32(this, 0x64), i32(this, 0x68), i32(this, 0x6c));

    {
        String s; s.ctor_char("", false);
        ((Layout *)(layout))->drawBox(2, i32(this, 0x64), i32(this, 0x68), i32(this, 0x6c), i32(this, 0x70), &s, 0);
    }
    if (masked) {
        String s; s.ctor_char("", false);
        ((Layout *)(layout))->drawBox(7, i32(this, 0x64), i32(this, 0x68), i32(this, 0x6c), i32(this, 0x70), &s, 0);
    }

    {
        String s; s.ctor_copy((String *)((GameText *)(*g_liw_d_gameText))->getText(*g_liw_d_headerId), false);
        ((Layout *)(layout))->drawHeader1(&s);
    }

    void *li = pp(this, 0x14);
    int isShip = ((ListItem *)(li))->isShip();
    ((PaintCanvas *)(long)canvas)->SetColor(canvas);

    int isItemish = ((ListItem *)(li))->isItem();
    if (isItemish == 0 && ((ListItem *)(li))->isBluePrint() == 0 && ((ListItem *)(li))->isPendingProduct() == 0) {
        if (isShip != 0) {
            char *L = (char *)layout;
            int x = i32(this, 0x64), y = i32(this, 0x68), w = i32(this, 0x6c);
            int c0c = i32(L, 0xc), c20 = i32(L, 0x20), c28 = i32(L, 0x28), c2c = i32(L, 0x2c);
            int color = i32(L, 0x5c);
            int textId = *g_liw_d_headerId;
            ((Ship *)(0))->getIndex();
            String s; s.ctor_copy((String *)((GameText *)(*g_liw_d_gameText))->getText(textId), false);
            ((Layout *)(layout))->drawBox8(1, c28 + x, y + c0c + c20, (w >> 1) - (c2c + c28), color, &s, 2);

            void *fac = *g_liw_d_imageFactory;
            int shipIdx = ((Ship *)(0))->getIndex();
            ((ImageFactory *)(fac))->drawShip(shipIdx, i32(this, 0x64) + i32(L, 0x28) + i32(L, 0x2c), ((i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20) + i32(L, 0x5c) / 2) - i32(L, 0x2c8) / 2) + i32(L, 0x124));
        }
    } else {
        char *L = (char *)layout;
        int x = i32(this, 0x64), y = i32(this, 0x68), w = i32(this, 0x6c);
        int c0c = i32(L, 0xc), c20 = i32(L, 0x20), c28 = i32(L, 0x28), c2c = i32(L, 0x2c);
        int color = i32(L, 0x5c);
        int textId = *g_liw_d_headerId;
        ((ListItem *)(li))->getIndex();
        String s; s.ctor_copy((String *)((GameText *)(*g_liw_d_gameText))->getText(textId), false);
        ((Layout *)(layout))->drawBox8(1, c28 + x, y + c0c + c20, (w >> 1) - (c2c + c28), color, &s, 2);

        void *itemPtr;
        if (((ListItem *)(li))->isItem() == 0) {
            int idx;
            void *db = *g_liw_d_itemDB;
            if (((ListItem *)(li))->isBluePrint() == 0)
                idx = i32((void *)(i32(li, 0x18)), 0x14);
            else
                idx = ((BluePrint *)((void *)i32(li, 0x8)))->getIndex();
            itemPtr = *(void **)(i32(db, 0x4) + idx * 4);
        } else {
            itemPtr = *(void **)((char *)li + 0x10);
        }

        void *fac = *g_liw_d_imageFactory;
        int idx = ((Item *)(itemPtr))->getIndex();
        int type = ((Item *)(itemPtr))->getType();
        ((ImageFactory *)(fac))->drawItem4(idx, type,
            i32(L, 0x28) + i32(this, 0x64) + i32(L, 0x2c),
            i32(L, 0x124) + ((i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20) + i32(L, 0x5c) / 2) - i32(L, 0x2c8) / 2));
    }

    // Visible value rows.
    char *L = (char *)layout;
    Array<String *> *rows = this->labels;
    if (rows != 0) {
        uint32_t n = rows->size();
        int rowH = i32(L, 0x2c);
        int yTop = i32(L, 0x5c) + i32(L, 0xc) + i32(this, 0x68) + i32(L, 0x20) + rowH;
        if ((uint32_t)(*g_liw_d_scrollLimit - i32(L, 0x10)) < (uint32_t)((i32(L, 0x1c) + rowH) * n + yTop))
            rowH = 2;
        int ycur = yTop;
        for (uint32_t i = 0; i < n; i++) {
            ((PaintCanvas *)(long)canvas)->SetColor(canvas);
            int color = i32(L, 0x1c);
            String s;
            s.ctor_copy((*rows)[i], false);
            ((Layout *)(layout))->drawBox(6, i32(L, 0x28) + i32(this, 0x64), ycur, (i32(this, 0x6c) >> 1) - (i32(L, 0x2c) + i32(L, 0x28)), color, &s, 0);
            ((PaintCanvas *)(long)canvas)->SetColor(canvas);

            int textX, textY;
            void *valStr;
            bool centered;
            bool drewArrow = false;
            if (isShip != 0) {
                uint32_t cur = this->statsCur->size();
                if (i < cur) {
                    if (i < cur - 1) {
                        int a = (*this->statsCur)[i];
                        int b = (*this->statsPrev)[i];
                        int img = 0x50;        // equal
                        if (a < b) img = 0x4c; // down
                        if (b < a) img = 0x48; // up
                        ((PaintCanvas *)(long)canvas)->DrawImage2D(i32(this, img),
                            ((i32(this, 0x64) + (i32(this, 0x6c) >> 1)) - i32(L, 0x2c)) - i32(this, 0x60), 0);
                    }
                    int sep = i32(this, 0x60);
                    valStr = (*this->values)[i];
                    void *arrowStr = *g_liw_d_arrowR;
                    int tw = ((PaintCanvas *)(long)canvas)->GetTextWidth(canvas, arrowStr);
                    centered = (char)((char)ycur + (char)(i32(L, 0x1c) / 2) - (char)i32(this, 0x1c));
                    textX = (((i32(this, 0x64) + (i32(this, 0x6c) >> 1) + 10) - i32(L, 0x2c)) - sep) - tw;
                    textY = i32(L, 0x1c);
                    drewArrow = true;
                }
            }
            if (!drewArrow) {
                valStr = (*this->values)[i];
                void *arrowStr = *g_liw_d_arrowL;
                int tw = ((PaintCanvas *)(long)canvas)->GetTextWidth(canvas, arrowStr);
                centered = (char)((char)ycur + (char)(i32(L, 0x1c) / 2) - (char)i32(this, 0x1c));
                textX = (i32(this, 0x64) + (i32(this, 0x6c) >> 1) + i32(L, 0x2c) * -2) - tw;
                textY = i32(L, 0x1c);
            }
            ((PaintCanvas *)(long)canvas)->DrawString(canvas, valStr, textX, textY, centered);
            ycur = ycur + rowH + i32(L, 0x1c);
        }
    }

    // Footer / progress.
    if (i32(this, 0x20) < 1) {
        String s; s.ctor_copy((String *)((GameText *)(*g_liw_d_gameText))->getText(*g_liw_d_headerId), false);
        ((Layout *)(layout))->drawBox(1, i32(this, 0x64) + (i32(this, 0x6c) >> 1) + i32(L, 0x2c), i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20), ((i32(this, 0x6c) >> 1) - i32(L, 0x2c)) - i32(L, 0x28), i32(L, 0x5c), &s, 0);
    } else {
        ((PaintCanvas *)(long)canvas)->SetColor(canvas);
        {
            String s; s.ctor_char("", false);
            ((Layout *)(layout))->drawBox(8, i32(this, 0x64) + (i32(this, 0x6c) >> 1) + i32(L, 0x2c), i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20), ((i32(this, 0x6c) >> 1) - i32(L, 0x2c)) - i32(L, 0x28), i32(this, 0x20), &s, 0);
        }
        int prog = i32(this, 0x20);
        int yBox = i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20);
        if (prog > 0) yBox = yBox + prog + i32(L, 0x2c);
        {
            String s; s.ctor_copy((String *)((GameText *)(*g_liw_d_gameText))->getText(*g_liw_d_headerId), false);
            ((Layout *)(layout))->drawBox(0, i32(this, 0x64) + (i32(this, 0x6c) >> 1) + i32(L, 0x2c), yBox, ((i32(this, 0x6c) >> 1) - i32(L, 0x2c)) - i32(L, 0x28), i32(L, 0x1c), &s, 0);
        }
        ((PaintCanvas *)(long)canvas)->SetColor(canvas);
        aeabi_idiv_(i32(this, 0x30), 3);
        ((PaintCanvas *)(long)canvas)->DrawImage2D(i32(this, 0x44), i32(this, 0x24) - i32(this, 0x2c), 0);
        int half = aeabi_idiv_(i32(this, 0x30), 3);
        ((PaintCanvas *)(long)canvas)->DrawImage2D(i32(this, 0x44), i32(this, 0x24), i32(this, 0x28) - half, (unsigned char)1);
    }

    ((ScrollTouchWindow *)(pp(this, 0x18)))->drawTextBG();
    ((ScrollTouchWindow *)(pp(this, 0x18)))->draw();
}

// Callees (resolved blx targets).
void  MatrixSetRotation(void *m, float x, float y, float z);
extern "C" void  MatrixSetScaling(void *m, float x, float y, float z);
extern "C" int   VectorSignedToFloat_i(int v);                          // s32->f32 helper

// Hidden PC-relative roots.
__attribute__((visibility("hidden"))) extern uint32_t *g_liw_u_tf;     // 0x143904 (preview transform handle)
__attribute__((visibility("hidden"))) extern const float *g_liw_u_angleTable; // 0x143... per-ship base angle
extern const float g_liw_u_angleScale;

// ListItemWindow::update(int frameTime)
//   Advances the scroll window, applies inertial spin damping to the rotating
//   3D ship preview, and pushes the resulting rotation onto its geometry.
void ListItemWindow::update(int frameTime)
{
    ((ScrollTouchWindow *)(pp(this, 0x18)))->update(frameTime);

    if (u8(this, 0x54) == 0)
        return;

    if (u8(this, 0x138) == 0) {
        float spin = f32(this, 0x12c) * f32(this, 0x130);
        float mag = spin > 0.0f ? spin : -spin;
        f32(this, 0x130) = spin;
        if (mag > 1.0f) {
            float angle = (float)i32(this, 0x11c);
            i32(this, 0x11c) = (int)(spin + angle);
        }
    }

    ((Ship *)(pp(this, 0x14) ? *(void **)((char *)pp(this, 0x14) + 0xc) : 0))->getIndex();
    int idx = ((Ship *)(*(void **)((char *)pp(this, 0x14) + 0xc)))->getIndex();

    float baseAngle = f32(this, 0x114);
    float angle = (float)i32(this, 0x11c) / g_liw_u_angleScale;
    f32(this, 0x118) = angle;

    uint32_t tf = *g_liw_u_tf;
    uint32_t loc = (uint32_t)(long)((PaintCanvas *)(long)tf)->TransformGetLocal(tf);
    MatrixSetRotation((void *)loc, angle, 0.0f, 0.0f);
    loc = (uint32_t)(long)((PaintCanvas *)(long)tf)->TransformGetLocal(tf);
    float tableAngle = g_liw_u_angleTable[idx] + baseAngle;
    MatrixSetScaling((void *)loc, tableAngle, tableAngle, tableAngle);

    if (i32(this, 0x90) != -1) {
        loc = (uint32_t)(long)((PaintCanvas *)(long)tf)->TransformGetLocal(tf);
        MatrixSetRotation((void *)loc, angle, 0.0f, 0.0f);
        loc = (uint32_t)(long)((PaintCanvas *)(long)tf)->TransformGetLocal(tf);
        MatrixSetScaling((void *)loc, tableAngle, tableAngle, tableAngle);
    }

    ((AEGeometry *)(pp(this, 0x10)))->setRotation(tableAngle, tableAngle, tableAngle);
}

// ---- C-ABI fragments ----

// ListItemWindow::dtor(): the C-ABI form of the complete-object destructor. The
// original deleting-destructor thunk runs ~ListItemWindow and returns the object
// pointer so the caller can hand it to operator delete.
void *ListItemWindow::dtor()
{
    this->~ListItemWindow();
    return this;
}

// ListItemWindow::touch_end(int, int): touch-release entry point invoked by the
// owning screen. Semantically identical to OnTouchEnd (release the drag-scroll
// of the 3D ship preview); returns a status int the caller discards.
int ListItemWindow::touch_end(int x, int y)
{
    this->OnTouchEnd(x, y);
    return 0;
}

// Two PC-relative singletons (each holds the address of a pointer).
__attribute__((visibility("hidden"))) extern void ***g_liw_a;
__attribute__((visibility("hidden"))) extern void ***g_liw_b;

struct Vec4 { int a, b, c, d; };

// ListItemWindow::ListItemWindow()
ListItemWindow::ListItemWindow()
{
    ((String *)((char *)this + 0x74))->ctor();
    ((String *)((char *)this + 0x80))->ctor();
    _liw_Matrix_ctor((char *)this + 0x98);
    _liw_Matrix_ctor((char *)this + 0xd4);

    void **a = *g_liw_a;
    void **b = *g_liw_b;
    *(Vec4 *)this = (Vec4){0, 0, 0, 0};
    void *av = *a;
    void *canvas = *b;
    pp(this, 0x18) = 0;
    (void)av;
    int h = ((PaintCanvas *)canvas)->GetTextHeight((unsigned int)(long)canvas);
    i32(this, 0x1c) = h / 2 - 1;
    i32(this, 0x20) = 0;
}
