#include "class.h"

// String is a 12-byte trivially-copyable value (ctor/dtor are engine calls).
struct Str { uint32_t a, b, c; };
extern "C" void  Str_ctor_cstr(Str *s, const char *text, bool copy); // 0x6ee18 family
extern "C" void  Str_ctor_copy(Str *s, const void *src, bool copy);  // String(String&)
extern "C" void  Str_dtor(Str *s);

// Layout / drawing callees (resolved blx targets).
extern "C" void  Layout_drawMask(void *layout);                              // 0x7696c
extern "C" void  Layout_drawBox(void *layout, int kind, int x, int y, int w, int color, Str *text); // 0x74de8
extern "C" void  Layout_drawBox8(void *layout, int kind, int x, int y, int w, int color, Str *text, int z); // 8-arg form
extern "C" void  Layout_drawHeader(void *layout, Str *text);
extern "C" void  PaintCanvas_SetColor(uint32_t c);                           // 0x6fac0
extern "C" void  PaintCanvas_FillRectangle(uint32_t c, int x, int y, int w); // 0x... fill
extern "C" int   PaintCanvas_GetTextWidth(uint32_t c, void *str);
extern "C" void  PaintCanvas_DrawImage2D(uint32_t c, int img, int y);
extern "C" void  PaintCanvas_DrawImage2D4(uint32_t c, int img, int x, int y, char flag);
extern "C" void  PaintCanvas_DrawString(uint32_t c, void *str, int x, int y, bool centered);
extern "C" void *GameText_getText(int id);
extern "C" int   ListItem_isShip(void *li);
extern "C" int   ListItem_isItem(void *li);
extern "C" int   ListItem_isBluePrint(void *li);
extern "C" int   ListItem_isPendingProduct(void *li);
extern "C" int   ListItem_getIndex(void *li);
extern "C" int   BluePrint_getIndex(void *bp);
extern "C" int   Item_getIndex(void *it);
extern "C" int   Item_getType(void *it);
extern "C" int   Ship_getIndex(void *ship);
extern "C" void  ImageFactory_drawShip(void *fac, int shipIdx, int x, int y);
extern "C" void  ImageFactory_drawItem(void *fac, int idx, int type, int x, int y);
extern "C" void  ScrollTouchWindow_drawTextBG(void *self);
extern "C" void  ScrollTouchWindow_draw(void *self);
extern "C" int   aeabi_idiv_(int a, int b);

// Hidden PC-relative roots.
__attribute__((visibility("hidden"))) extern char *g_liw_d_maskFlag;     // 0x143118
__attribute__((visibility("hidden"))) extern void **g_liw_d_canvas;      // 0x143140 -> canvas
__attribute__((visibility("hidden"))) extern void **g_liw_d_layout;      // 0x143162 -> Layout
__attribute__((visibility("hidden"))) extern int   *g_liw_d_headerId;    // 0x1431dc
__attribute__((visibility("hidden"))) extern void **g_liw_d_imageFactory;// 0x1437c8 / 0x1432e4
__attribute__((visibility("hidden"))) extern void **g_liw_d_itemDB;      // 0x1432c2
__attribute__((visibility("hidden"))) extern void **g_liw_d_arrowL;      // 0x14350c
__attribute__((visibility("hidden"))) extern void **g_liw_d_arrowR;      // 0x143466
__attribute__((visibility("hidden"))) extern int   *g_liw_d_scrollLimit; // 0x14335c

struct ListItemWindow { void draw(); };

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
        Layout_drawMask(layout);

    PaintCanvas_SetColor(canvas);
    PaintCanvas_FillRectangle(canvas, i32(this, 0x64), i32(this, 0x68), i32(this, 0x6c));

    {
        Str s; Str_ctor_cstr(&s, "", false);
        Layout_drawBox(layout, 2, i32(this, 0x64), i32(this, 0x68), i32(this, 0x6c), i32(this, 0x70), &s);
        Str_dtor(&s);
    }
    if (masked) {
        Str s; Str_ctor_cstr(&s, "", false);
        Layout_drawBox(layout, 7, i32(this, 0x64), i32(this, 0x68), i32(this, 0x6c), i32(this, 0x70), &s);
        Str_dtor(&s);
    }

    {
        Str s; Str_ctor_copy(&s, GameText_getText(*g_liw_d_headerId), false);
        Layout_drawHeader(layout, &s);
        Str_dtor(&s);
    }

    void *li = pp(this, 0x14);
    int isShip = ListItem_isShip(li);
    PaintCanvas_SetColor(canvas);

    int isItemish = ListItem_isItem(li);
    if (isItemish == 0 && ListItem_isBluePrint(li) == 0 && ListItem_isPendingProduct(li) == 0) {
        if (isShip != 0) {
            char *L = (char *)layout;
            int x = i32(this, 0x64), y = i32(this, 0x68), w = i32(this, 0x6c);
            int c0c = i32(L, 0xc), c20 = i32(L, 0x20), c28 = i32(L, 0x28), c2c = i32(L, 0x2c);
            int color = i32(L, 0x5c);
            int textId = *g_liw_d_headerId;
            Ship_getIndex(0);
            Str s; Str_ctor_copy(&s, GameText_getText(textId), false);
            Layout_drawBox8(layout, 1, c28 + x, y + c0c + c20, (w >> 1) - (c2c + c28), color, &s, 2);
            Str_dtor(&s);

            void *fac = *g_liw_d_imageFactory;
            int shipIdx = Ship_getIndex(0);
            ImageFactory_drawShip(fac, shipIdx,
                i32(this, 0x64) + i32(L, 0x28) + i32(L, 0x2c),
                ((i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20) + i32(L, 0x5c) / 2) - i32(L, 0x2c8) / 2) + i32(L, 0x124));
        }
    } else {
        char *L = (char *)layout;
        int x = i32(this, 0x64), y = i32(this, 0x68), w = i32(this, 0x6c);
        int c0c = i32(L, 0xc), c20 = i32(L, 0x20), c28 = i32(L, 0x28), c2c = i32(L, 0x2c);
        int color = i32(L, 0x5c);
        int textId = *g_liw_d_headerId;
        ListItem_getIndex(li);
        Str s; Str_ctor_copy(&s, GameText_getText(textId), false);
        Layout_drawBox8(layout, 1, c28 + x, y + c0c + c20, (w >> 1) - (c2c + c28), color, &s, 2);
        Str_dtor(&s);

        void *itemPtr;
        if (ListItem_isItem(li) == 0) {
            int idx;
            void *db = *g_liw_d_itemDB;
            if (ListItem_isBluePrint(li) == 0)
                idx = i32((void *)(i32(li, 0x18)), 0x14);
            else
                idx = BluePrint_getIndex((void *)i32(li, 0x8));
            itemPtr = *(void **)(i32(db, 0x4) + idx * 4);
        } else {
            itemPtr = *(void **)((char *)li + 0x10);
        }

        void *fac = *g_liw_d_imageFactory;
        int idx = Item_getIndex(itemPtr);
        int type = Item_getType(itemPtr);
        ImageFactory_drawItem(fac, idx, type,
            i32(L, 0x28) + i32(this, 0x64) + i32(L, 0x2c),
            i32(L, 0x124) + ((i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20) + i32(L, 0x5c) / 2) - i32(L, 0x2c8) / 2));
    }

    // Visible value rows.
    char *L = (char *)layout;
    void **rows = (void **)pp(this, 0x0);
    if (rows != 0) {
        uint32_t n = *(uint32_t *)rows;
        int rowH = i32(L, 0x2c);
        int yTop = i32(L, 0x5c) + i32(L, 0xc) + i32(this, 0x68) + i32(L, 0x20) + rowH;
        if ((uint32_t)(*g_liw_d_scrollLimit - i32(L, 0x10)) < (uint32_t)((i32(L, 0x1c) + rowH) * n + yTop))
            rowH = 2;
        int ycur = yTop;
        for (uint32_t i = 0; i < n; i++) {
            PaintCanvas_SetColor(canvas);
            int color = i32(L, 0x1c);
            Str s;
            Str_ctor_copy(&s, *(void **)(*(int *)((char *)rows + 4) + i * 4), false);
            Layout_drawBox(layout, 6, i32(L, 0x28) + i32(this, 0x64), ycur,
                           (i32(this, 0x6c) >> 1) - (i32(L, 0x2c) + i32(L, 0x28)), color, &s);
            Str_dtor(&s);
            PaintCanvas_SetColor(canvas);

            int textX, textY;
            void *valStr;
            bool centered;
            bool drewArrow = false;
            if (isShip != 0) {
                uint32_t cur = *(uint32_t *)pp(this, 0x8);
                if (i < cur) {
                    if (i < cur - 1) {
                        int a = *(int *)(((int *)pp(this, 0x8))[1] + i * 4);
                        int b = *(int *)(*(int *)(i32(this, 0xc) + 4) + i * 4);
                        int img = 0x50;        // equal
                        if (a < b) img = 0x4c; // down
                        if (b < a) img = 0x48; // up
                        PaintCanvas_DrawImage2D(canvas, i32(this, img),
                            ((i32(this, 0x64) + (i32(this, 0x6c) >> 1)) - i32(L, 0x2c)) - i32(this, 0x60));
                    }
                    int sep = i32(this, 0x60);
                    valStr = *(void **)(*(int *)(i32(this, 0x4) + 4) + i * 4);
                    void *arrowStr = *g_liw_d_arrowR;
                    int tw = PaintCanvas_GetTextWidth(canvas, arrowStr);
                    centered = (char)((char)ycur + (char)(i32(L, 0x1c) / 2) - (char)i32(this, 0x1c));
                    textX = (((i32(this, 0x64) + (i32(this, 0x6c) >> 1) + 10) - i32(L, 0x2c)) - sep) - tw;
                    textY = i32(L, 0x1c);
                    drewArrow = true;
                }
            }
            if (!drewArrow) {
                valStr = *(void **)(*(int *)(i32(this, 0x4) + 4) + i * 4);
                void *arrowStr = *g_liw_d_arrowL;
                int tw = PaintCanvas_GetTextWidth(canvas, arrowStr);
                centered = (char)((char)ycur + (char)(i32(L, 0x1c) / 2) - (char)i32(this, 0x1c));
                textX = (i32(this, 0x64) + (i32(this, 0x6c) >> 1) + i32(L, 0x2c) * -2) - tw;
                textY = i32(L, 0x1c);
            }
            PaintCanvas_DrawString(canvas, valStr, textX, textY, centered);
            ycur = ycur + rowH + i32(L, 0x1c);
        }
    }

    // Footer / progress.
    if (i32(this, 0x20) < 1) {
        Str s; Str_ctor_copy(&s, GameText_getText(*g_liw_d_headerId), false);
        Layout_drawBox(layout, 1,
            i32(this, 0x64) + (i32(this, 0x6c) >> 1) + i32(L, 0x2c),
            i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20),
            ((i32(this, 0x6c) >> 1) - i32(L, 0x2c)) - i32(L, 0x28), i32(L, 0x5c), &s);
        Str_dtor(&s);
    } else {
        PaintCanvas_SetColor(canvas);
        {
            Str s; Str_ctor_cstr(&s, "", false);
            Layout_drawBox(layout, 8,
                i32(this, 0x64) + (i32(this, 0x6c) >> 1) + i32(L, 0x2c),
                i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20),
                ((i32(this, 0x6c) >> 1) - i32(L, 0x2c)) - i32(L, 0x28), i32(this, 0x20), &s);
            Str_dtor(&s);
        }
        int prog = i32(this, 0x20);
        int yBox = i32(this, 0x68) + i32(L, 0xc) + i32(L, 0x20);
        if (prog > 0) yBox = yBox + prog + i32(L, 0x2c);
        {
            Str s; Str_ctor_copy(&s, GameText_getText(*g_liw_d_headerId), false);
            Layout_drawBox(layout, 0,
                i32(this, 0x64) + (i32(this, 0x6c) >> 1) + i32(L, 0x2c),
                yBox, ((i32(this, 0x6c) >> 1) - i32(L, 0x2c)) - i32(L, 0x28), i32(L, 0x1c), &s);
            Str_dtor(&s);
        }
        PaintCanvas_SetColor(canvas);
        aeabi_idiv_(i32(this, 0x30), 3);
        PaintCanvas_DrawImage2D(canvas, i32(this, 0x44), i32(this, 0x24) - i32(this, 0x2c));
        int half = aeabi_idiv_(i32(this, 0x30), 3);
        PaintCanvas_DrawImage2D4(canvas, i32(this, 0x44), i32(this, 0x24), i32(this, 0x28) - half, 1);
    }

    ScrollTouchWindow_drawTextBG(pp(this, 0x18));
    ScrollTouchWindow_draw(pp(this, 0x18));
}
