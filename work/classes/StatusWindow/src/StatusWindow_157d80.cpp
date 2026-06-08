#include "class.h"

extern "C" {
void *operator_new(uint32_t size);
void *operator_new_array(uint32_t size);

void ArrayTB_ctor(void *self);                       // Array<TouchButton*>::Array()
void ArrayTB_setLength(int n, void *self);           // ArraySetLength<TouchButton*>

void *GameText_getText(int id);

void TouchButton_ctor_tab(void *self, void *text, int kind, int x, int y, char flags);
void TouchButton_ctor_medal(void *self, int index, int medal, void *text, int x, int y, char flags);
int  TouchButton_getWidth(void *self);
void TouchButton_setAlwaysPressed(void *self, bool pressed);

int  Layout_getHelpButtonOffset();

int  *Achievements_getMedals(void *ach);

void PaintCanvas_setImage2D(void *canvas, int imageId, void *out);
int  PaintCanvas_GetImage2DWidth(void *canvas);
int  PaintCanvas_GetImage2DHeight(void *canvas);

int  __aeabi_idiv(int a, int b);

extern void *g_sw_gameTextDef;   // *(DAT_168024): GameText id source
extern void *g_sw_layout;        // *(DAT_16802c): Layout singleton
extern void *g_sw_layoutW;       // *(DAT_168028): layout width source
extern unsigned char *g_sw_tabIndex; // *(DAT_168030): default tab index byte
extern void *g_sw_achievements;  // *(DAT_168034): achievements root
extern void *g_sw_canvas;        // *(DAT_168038): paint canvas
extern int   g_sw_screenH;       // *(DAT_168040): screen height source
}

extern "C" void StatusWindow_reInit(StatusWindow *self);

// StatusWindow::StatusWindow() -- build the tab bar and the medal grid, then lay out scrolling.
extern "C" StatusWindow *StatusWindow_ctor(StatusWindow *self)
{
    // --- two-tab button bar at +0x04 ---
    void *tabs = operator_new(0xc);
    ArrayTB_ctor(tabs);
    pp(self, 0x4) = tabs;
    ArrayTB_setLength(2, tabs);

    void *layout = *(void **)g_sw_layout;
    int layoutW = *(int *)*(void **)g_sw_layoutW;
    int textId = *(int *)*(void **)g_sw_gameTextDef;

    void *b0 = operator_new(200);
    void *t0 = GameText_getText(textId);
    int helpOff = Layout_getHelpButtonOffset();
    TouchButton_ctor_tab(b0, t0, 3, layoutW - helpOff, 0, 0x12);
    *(void **)(*(int *)(*(int *)((char *)self + 0x4) + 4) + 4) = b0;

    void *b1 = operator_new(200);
    void *t1 = GameText_getText(textId);
    int helpOff2 = Layout_getHelpButtonOffset();
    int w1 = TouchButton_getWidth(b1);
    TouchButton_ctor_tab(b1, t1, 3,
                         ((layoutW - helpOff2) - w1) + *(int *)((char *)layout + 0x38), 0, 0x12);
    *(void **)(*(int *)((char *)self + 0x4) + 4) = b1;

    unsigned int defTab = *g_sw_tabIndex;
    u32(self, 0x30) = defTab;
    TouchButton_setAlwaysPressed(
        *(void **)(*(int *)(*(int *)((char *)self + 0x4) + 4) + defTab * 4), true);

    i32(self, 0x10) = 0;
    i32(self, 0x78) = *(int *)((char *)layout + 0x84);
    i32(self, 0x34) = -1;

    // --- 45 medal buttons at +0x08 ---
    void *medals = operator_new(0xc);
    ArrayTB_ctor(medals);
    i32(self, 0x0) = 0x2d;
    pp(self, 0x8) = medals;
    ArrayTB_setLength(0x2d, medals);

    int *medalIds = Achievements_getMedals(*(void **)g_sw_achievements);
    for (int i = 0; i < i32(self, 0x0); i++) {
        void *btn = operator_new(200);
        int medal = medalIds[i];
        void *txt = GameText_getText(textId);
        TouchButton_ctor_medal(btn, i, medal, txt, 0, 0, 'D');
        *(void **)(*(int *)(*(int *)((char *)self + 0x8) + 4) + i * 4) = btn;
    }

    StatusWindow_reInit(self);

    void *canvas = *(void **)g_sw_canvas;
    PaintCanvas_setImage2D(canvas, 0x48e, (char *)self + 0x24);
    PaintCanvas_setImage2D(canvas, 0x48f, (char *)self + 0x28);
    PaintCanvas_setImage2D(canvas, 0x48d, (char *)self + 0x2c);
    i32(self, 0x70) = PaintCanvas_GetImage2DWidth(canvas) / 2;
    int img3h = PaintCanvas_GetImage2DHeight(canvas);

    // Zero the inertia/colour state blocks (+0x45..+0x54, +0x38..+0x44).
    for (int o = 0x45; o < 0x55; o++) F<unsigned char>(self, o) = 0;
    for (int o = 0x38; o < 0x48; o += 4) i32(self, o) = 0;
    i32(self, 0x74) = img3h;

    // Precompute per-tab scroll content heights.
    int *heights = (int *)operator_new_array(0xc);
    pp(self, 0x68) = heights;
    int row = ((*(int *)((char *)layout + 0x1c) * 3 + *(int *)((char *)layout + 0x2d8)) +
               *(int *)((char *)layout + 0x2c) * 8) + i32(self, 0x64) +
               *(int *)((char *)layout + 0x4) * 7;
    heights[0] = row;

    int lineH;
    if (*g_sw_tabIndex == 0) {
        lineH = i32(self, 0x78) * __aeabi_idiv(i32(self, 0x0), 3);
    } else {
        lineH = __aeabi_idiv(i32(self, 0x0), 3) * i32(self, 0x78) +
                *(int *)((char *)layout + 0x1c) + *(int *)((char *)layout + 0x2c);
    }
    heights[1] = lineH + 10;

    i32(self, 0x58) = heights[u32(self, 0x30)];
    i32(self, 0x5c) =
        (((g_sw_screenH - *(int *)((char *)layout + 0x10)) - *(int *)((char *)layout + 0xc)) -
         *(int *)((char *)layout + 0x20)) - *(int *)((char *)layout + 0x24);
    return self;
}
