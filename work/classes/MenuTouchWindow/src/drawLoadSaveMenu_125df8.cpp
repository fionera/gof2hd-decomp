#include "class.h"

// MenuTouchWindow::drawLoadSaveMenu(bool). Draws the scrollable list of save slots:
// optional scrollbar image strip, then per-slot boxed rows with name / time / kills / rank
// strings and a ship thumbnail. r0=this, r1 unused (the disasm never reads it).

extern "C" void _mtw_PaintCanvas_SetColor(unsigned int color);
extern "C" int  _mtw_PaintCanvas_GetImage2DWidth(void *pc);
extern "C" int  _mtw_PaintCanvas_GetImage2DHeight(void *pc);
extern "C" void _mtw_PaintCanvas_DrawImage2D(void *pc, unsigned int img, int x, int y, char anchor);
extern "C" void _mtw_PaintCanvas_DrawString(void *pc, void *str, int val, int x, char y);
extern "C" void _mtw_PaintCanvas_DrawStringFull(void *pc, void *str, void *s, int x, int y, int z);
extern "C" int  _mtw_idiv(int a, int b);
extern "C" void _mtw_TouchButton_setPosition(void *btn, int x, int y);
extern "C" void _mtw_TouchButton_draw(void *btn);
extern "C" void _mtw_String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void _mtw_String_dtor(void *s);
extern "C" void _mtw_Layout_drawBox(int layout, int mode, int x, int y, int w, int h, void *str);
extern "C" void _mtw_ImageFactory_drawShip(void *imgF, unsigned int shipId, int x, int y);

// PC-relative holders.
extern void *const gDlsLayout   __attribute__((visibility("hidden")));  // *holder -> layout obj
extern void *const gDlsCanvas   __attribute__((visibility("hidden")));  // *holder -> PaintCanvas
extern void *const gDlsColor    __attribute__((visibility("hidden")));  // *holder -> [0] = color
extern void *const gDlsScreenW  __attribute__((visibility("hidden")));  // *holder -> [0] width-ish bound
extern void *const gDlsScrollOn __attribute__((visibility("hidden")));  // *holder -> [0] byte: show scrollbar
extern void *const gDlsScrollNo __attribute__((visibility("hidden")));  // *holder -> [0] byte: suppress
extern void *const gDlsFlagA    __attribute__((visibility("hidden")));  // *holder -> [0] byte
extern void *const gDlsFlagB    __attribute__((visibility("hidden")));  // *holder -> [0] byte
extern void *const gDlsTileCnt  __attribute__((visibility("hidden")));  // *holder -> [0] tile count divisor src
extern void *const gDlsRowCount __attribute__((visibility("hidden")));  // *holder -> [0] row count
extern void *const gDlsRowMax   __attribute__((visibility("hidden")));  // *holder -> [0] max y
extern const char gDlsBoxStr[]  __attribute__((visibility("hidden")));  // box style id string
extern void *const gDlsFont     __attribute__((visibility("hidden")));  // *holder -> [0] -> font String
extern void *const gDlsImgFact  __attribute__((visibility("hidden")));  // *holder -> ImageFactory

struct MenuTouchWindow { void drawLoadSaveMenu(bool param1); };
void MenuTouchWindow::drawLoadSaveMenu(bool param1)
{
    (void)param1;
    void *self = this;
    int *layout = (int *)*(void **)gDlsLayout;
    void *canvas = *(void **)gDlsCanvas;
    unsigned int color = *(unsigned int *)*(void **)gDlsColor;

    int rowBaseY = layout[0x43];        // +0x10c
    _mtw_PaintCanvas_SetColor(color);

    int scrollOff = i32(self, 0x198);
    int margin = layout[0xa];            // +0x28
    int strip58 = layout[0x44];          // +0x110
    int strip5c = layout[0x45];          // +0x114
    int screenBound = *(int *)*(void **)gDlsScreenW;
    int inner = screenBound + margin * -2 + scrollOff * -2;

    if (*(char *)*(void **)gDlsScrollOn != 0 && *(char *)*(void **)gDlsScrollNo == 0) {
        strip5c = 8;
        if (*(char *)*(void **)gDlsFlagA == 0) {
            strip58 = 0xc;
            if (*(char *)*(void **)gDlsTileCnt == 0) { strip5c = 4; strip58 = 6; }
        } else {
            strip58 = 8; strip5c = 5;
        }
        int iw = _mtw_PaintCanvas_GetImage2DWidth(canvas);
        int ih = _mtw_PaintCanvas_GetImage2DHeight(canvas);
        int count = _mtw_idiv((int)(long)*(void **)gDlsTileCnt, 1);
        int yy = 0;
        for (int k = 0; k <= count; k++) {
            _mtw_PaintCanvas_DrawImage2D(canvas, u32(self, 0x11c),
                (layout[0xa] - iw) + i32(self, 0x198), yy, 1);
            _mtw_PaintCanvas_DrawImage2D(canvas, u32(self, 0x11c),
                layout[0xa] + inner + i32(self, 0x198), yy, 0);
            yy += ih;
        }
        scrollOff = i32(self, 0x198);
        margin = layout[0xa];
    }

    _mtw_TouchButton_setPosition(pp(self, 0xc4),
        (screenBound - scrollOff) - margin,
        (layout[0x1c] + i32(self, 0x1b4)) * i32(self, 0x18c) + i32(self, 0x194)
            + layout[8] + layout[3] + layout[0x42]);

    int rowCount = *(int *)*(void **)gDlsRowCount;
    int rowMax = *(int *)*(void **)gDlsRowMax;

    for (int i = 0; i < rowCount; i++) {
        int rowY = (layout[0x1c] + i32(self, 0x1b4)) * i + i32(self, 0x194) + layout[8] + layout[3];
        if (rowY < 0 || rowY > rowMax) continue;

        _mtw_PaintCanvas_SetColor(color);
        int boxX = layout[0xa] + i32(self, 0x198);
        char box[12]; _mtw_String_ctor_cstr(box, gDlsBoxStr, false);
        int mode = (i == i32(self, 0x18c)) ? 4 : 3;
        _mtw_Layout_drawBox((int)layout, mode, boxX, rowY, inner - 3, layout[0x1c], box);
        _mtw_String_dtor(box);

        void *font = *(void **)*(void **)gDlsFont;
        int yName = strip58 + rowY;
        int *cols = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4);

        _mtw_PaintCanvas_DrawString(canvas, font, *(int *)(((void **)cols)[0]),
            layout[0xa] + i32(self, 0x198) + layout[0xb] /*+0x2c*/, (char)yName);

        int slot = *(int *)(i32(pp(self, 0xbc), 4) + i * 4);
        if (slot != 0) {
            unsigned int shipId = *(unsigned int *)(slot + 0x1a0);
            if (shipId < 0x40) {
                _mtw_ImageFactory_drawShip(*(void **)gDlsImgFact, shipId,
                    layout[0xb] + layout[0xa] + i32(self, 0x198) + i32(self, 0x1bc),
                    rowBaseY + rowY);
            }
        }

        _mtw_PaintCanvas_DrawStringFull(canvas, font, ((void **)cols)[1],
            layout[0xa] + i32(self, 0x198) + layout[0xb] * 2 + i32(self, 0x1bc) + layout[0xb1] /*+0x2c4*/,
            yName, 0);

        _mtw_PaintCanvas_SetColor(color);
        int rowY2 = rowY + strip5c;
        _mtw_PaintCanvas_DrawStringFull(canvas, font, ((void **)cols)[2],
            layout[0xa] + i32(self, 0x198) + layout[0xb],
            rowY2 + layout[0x1c] / 2, 0);

        _mtw_PaintCanvas_DrawStringFull(canvas, font, ((void **)cols)[3],
            layout[0xa] + i32(self, 0x198) + layout[0xb] * 2 + i32(self, 0x1bc) + layout[0xb1],
            rowY2 + layout[0x1c] / 2, 0);

        _mtw_PaintCanvas_DrawStringFull(canvas, font, ((void **)cols)[4],
            layout[0xa] + i32(self, 0x198) + layout[0xb1] + (layout[0xb] + i32(self, 0x1b8)) * 2,
            yName, 0);

        _mtw_PaintCanvas_DrawStringFull(canvas, font, ((void **)cols)[5],
            layout[0xa] + i32(self, 0x198) + layout[0xb1] + (layout[0xb] + i32(self, 0x1b8)) * 2,
            rowY2 + layout[0x1c] / 2, 0);

        if (i == i32(self, 0x18c))
            _mtw_TouchButton_draw(pp(self, 0xc4));
    }
}
