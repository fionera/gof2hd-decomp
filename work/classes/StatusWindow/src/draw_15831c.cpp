#include "class.h"

// StatusWindow::draw() -- renders either the player-stats tab or the achievements/medal tab.
// This is a very large UI routine; the control flow, field offsets and the substantive engine
// calls are reproduced faithfully. Repetitive DrawString blocks share the local helpers below.

extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

// String slots are 0xc bytes, driven via engine entry points.
void String_default(void *s);
void String_fromC(void *s, const char *text, bool copy);
void String_fromInt(void *s, int value);
void String_fromText(void *s, void *text, bool copy);
void String_dtor(void *s);
void String_assign(void *dst, void *src);
void String_concatText(void *out, void *lhs);          // operator+(String, text)
void String_concatInt(void *out, void *lhs, int *v);   // operator+(String, int)
void String_subString(void *self, void *other);

void *GameText_getText(int id);
int   GameText_getLanguage();

void PaintCanvas_SetColor(void *canvas);
void PaintCanvas_FillRectangle(void *canvas, int x, int y, int wh);
int  PaintCanvas_GetTextWidth(void *canvas, void *text);
void PaintCanvas_DrawString(void *canvas, void *font, void *text, int x, int y);
void PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y, char anchor);
void PaintCanvas_DrawImage2D_xy(void *canvas, int image, int x, int y);
void PaintCanvas_DrawRegion2D(void *canvas, int image, int w, int sx, int sy, int h,
                              float fy, int a, int b, int c, int x);

void Layout_drawBG(void *layout);
void Layout_drawScrollBar(void *layout, int x, int y, int range, int pos, int height);
void Layout_drawBox(void *layout, int style, int x, int y, int w, int h, void *label);
void Layout_drawHeader(void *layout, void *title);
void Layout_drawFooter(void *layout);
void Layout_formatCredits(void *out, int credits);

void ImageFactory_drawChar(void *factory, void *arr, int x, int y, bool b);
void ImageFactory_drawShip(void *factory, int shipIndex, int x, int y);

void *Status_getShip();
void *Status_getStanding();
int   Status_getCredits();
int   Status_getLevel();
unsigned long long Status_getPlayingTime();
int   Status_getMissionCount(void *st);
int   Status_getKills(void *st);
int   Status_getCapturedCrates(void *st);
int   Status_getStationsVisited(void *st);
int   Status_getJumpgateUsed(void *st);
int   Status_getGoodsProduced(void *st);

int   Ship_getIndex(void *ship);
int   Ship_getFirePower(void *ship);
int   Ship_getCombinedHP(void *ship);

float Standing_getStandingRate(void *standing);

void  Globals_longToTimeStringNoSeconds(void *globals, void *out, unsigned long long t);
void  Globals_drawLines(void *globals, void *font, void *arr, int y, char clip);

void  TouchButton_setPosition(void *btn, int x, int y);
void  TouchButton_draw(void *btn);

int   __aeabi_idiv(int a, int b);
int   __aeabi_uidiv(unsigned a, unsigned b);

extern void *g_swd_screen;     // *(DAT_168464): screen-state base (stack-guard source)
extern void *g_swd_canvas;     // *(DAT_168468): paint canvas
extern int  *g_swd_dimW;       // *(DAT_16846c): screen width
extern int  *g_swd_dimH;       // *(DAT_168470): screen height
extern void *g_swd_layout;     // *(DAT_168474): Layout singleton
extern void *g_swd_imageFactory; // *(DAT_168544): image factory
extern void *g_swd_font;       // *(DAT_1685a4): default font
extern void *g_swd_status;     // *(DAT_16855c): status singleton
extern void *g_swd_globals;    // *(DAT_16868c): globals
extern char *g_swd_landscape;  // *(DAT_168478): landscape-layout flag
extern int  *g_swd_textId;     // *(DAT_1684ac): rolling GameText id cursor
}

extern "C" float StatusWindow_getRelativeScrollStartPos(StatusWindow *self);
extern "C" float StatusWindow_getRelativeScrollHeight(StatusWindow *self);

// StatusWindow::draw()
extern "C" void StatusWindow_draw(StatusWindow *self)
{
    void *volatile cookie = __stack_chk_guard;

    void *canvas = *(void **)g_swd_canvas;
    void *layout = *(void **)g_swd_layout;
    void *font = *(void **)g_swd_font;
    char *land = (char *)*(void **)((char *)&g_swd_landscape);
    int screenW = *g_swd_dimW;
    int screenH = *g_swd_dimH;

    // --- background + scrollbar ---
    PaintCanvas_SetColor(canvas);
    PaintCanvas_FillRectangle(canvas, 0, 0, screenW);
    PaintCanvas_SetColor(canvas);
    Layout_drawBG(layout);

    float relStart = StatusWindow_getRelativeScrollStartPos(self);
    int contentH = i32(self, 0x5c);
    float ch = (float)contentH;
    float relH = StatusWindow_getRelativeScrollHeight(self);
    int barH = (int)(relH * ch);
    if (barH > 0 || (int)(relStart * ch) > 0) {
        Layout_drawScrollBar(layout,
            (screenW - *(int *)((char *)layout + 0x48)) - *(int *)((char *)layout + 0x28),
            *(int *)((char *)layout + 0x20) + *(int *)((char *)layout + 0xc),
            contentH, (int)(relStart * ch), barH);
    }

    int top = *(int *)((char *)layout + 0x20) + *(int *)((char *)layout + 0xc);
    int colW;
    if (*land == 0) {
        colW = screenW;
        top += i32(self, 0x38);
    } else {
        colW = screenW >> 1;
    }
    if (barH > 0)
        colW = (colW - *(int *)((char *)layout + 0x48)) - *(int *)((char *)layout + 0x2c);
    i32(self, 0x6c) = colW + *(int *)((char *)layout + 0x28) * -2;

    char creditStr[0xc];
    String_default(creditStr);
    char sep[0xc];
    if (GameText_getLanguage() == 9)
        String_fromC(sep, "\xa1", false);
    else
        String_fromC(sep, ":", false);

    int tab = i32(self, 0x30);
    char drewStats = 0;

    // ===== player-stats tab (index 0 / landscape) =====
    if (tab == 0 || *land != 0) {
        int boxW = i32(self, 0x6c);
        int x0 = *(int *)((char *)layout + 0x28);
        int pad = *(int *)((char *)layout + 0x2c);
        char lbl[0xc];

        void *t = GameText_getText(*g_swd_textId);
        String_fromText(lbl, t, false);
        Layout_drawBox(layout, 0, x0, top, boxW, *(int *)((char *)layout + 0x1c), lbl);
        String_dtor(lbl);

        int y = *(int *)((char *)layout + 0x1c) + top + pad;

        // Credits panel.
        String_fromC(lbl, "", false);
        Layout_drawBox(layout, 5, x0, y, (boxW >> 1) - pad, *(int *)((char *)layout + 0x2d8), lbl);
        String_dtor(lbl);
        ImageFactory_drawChar(*(void **)g_swd_imageFactory, pp(self, 0xc),
                              *(int *)((char *)layout + 0x4c) + x0, y, false);
        char credTmp[0xc];
        Layout_formatCredits(credTmp, Status_getCredits());
        String_assign(creditStr, credTmp);
        String_dtor(credTmp);
        int tw = PaintCanvas_GetTextWidth(canvas, font);
        PaintCanvas_DrawString(canvas, font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y);

        // Level line.
        char lvlPrefix[0xc], lvlText[0xc], lvlFull[0xc];
        void *lt = GameText_getText(*g_swd_textId);
        String_fromC(lvlPrefix, " ", false);
        String_concatText(lvlText, lt);
        int lvl = Status_getLevel();
        String_concatInt(lvlFull, lvlText, &lvl);
        String_assign(creditStr, lvlFull);
        String_dtor(lvlFull); String_dtor(lvlText); String_dtor(lvlPrefix);
        tw = PaintCanvas_GetTextWidth(canvas, font);
        PaintCanvas_DrawString(canvas, font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y);

        // Playing-time line.
        char timeStr[0xc];
        Globals_longToTimeStringNoSeconds(*(void **)g_swd_globals, timeStr, Status_getPlayingTime());
        tw = PaintCanvas_GetTextWidth(canvas, font);
        PaintCanvas_DrawString(canvas, font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y);

        // Ship picture panel.
        String_fromC(lbl, "", false);
        Layout_drawBox(layout, 5, (boxW >> 1) + x0 + pad, y, (boxW >> 1) - pad,
                       *(int *)((char *)layout + 0x2d8), lbl);
        String_dtor(lbl);
        ImageFactory_drawShip(*(void **)g_swd_imageFactory, Ship_getIndex(Status_getShip()),
                              x0 + (boxW >> 1) + pad * 2, y);
        void *shipNameTxt = GameText_getText(Ship_getIndex(Status_getShip()));
        PaintCanvas_DrawString(canvas, font, shipNameTxt,
                               x0 + (boxW >> 1) + pad * 3 + *(int *)((char *)layout + 0x2cc), y);

        // Fire-power line.
        char fpStr[0xc], fpPre[0xc], fpFull[0xc];
        int firePow = Ship_getFirePower(Status_getShip());
        String_fromInt(fpStr, (int)((float)firePow * 1.0f));
        int fp2 = Ship_getFirePower(Status_getShip());
        String_fromC(fpPre, "%", false);
        String_concatInt(fpFull, fpPre, &fp2);
        String_concatText(fpStr, fpFull);
        String_assign(creditStr, fpStr);
        String_dtor(fpFull); String_dtor(fpPre); String_dtor(fpStr);
        tw = PaintCanvas_GetTextWidth(canvas, font);
        PaintCanvas_DrawString(canvas, font, creditStr, ((y + x0) - pad) - tw, y);

        // Combined-HP line.
        char hpStr[0xc];
        String_fromInt(hpStr, Ship_getCombinedHP(Status_getShip()));
        tw = PaintCanvas_GetTextWidth(canvas, font);
        PaintCanvas_DrawString(canvas, font, hpStr, ((y + x0) - pad) - tw, y);
        String_dtor(hpStr);

        // Standing emblem panel + bars.
        void *standing = Status_getStanding();
        float rate = Standing_getStandingRate(standing);
        PaintCanvas_DrawImage2D(canvas, i32(self, 0x24), x0 + (boxW >> 2), y, '\x11');
        PaintCanvas_DrawRegion2D(canvas, i32(self, 0x28), i32(self, 0x70), 0,
                                 (int)-(rate * (float)i32(self, 0x70)), i32(self, 0x74),
                                 -(rate * (float)i32(self, 0x70)), 0, 0, 0, x0 + (boxW >> 2));
        PaintCanvas_DrawImage2D(canvas, i32(self, 0x2c), x0, y, '\x11');

        // Career-stat rows from the Status singleton.
        void *st = *(void **)g_swd_status;
        char rowStr[0xc];
        int rowX = *(int *)((char *)layout + 0x4c) + x0;
        struct { int (*get)(void *); } rows[] = {
            { Status_getMissionCount }, { Status_getKills }, { Status_getCapturedCrates },
            { Status_getStationsVisited }, { Status_getJumpgateUsed }, { Status_getGoodsProduced },
        };
        for (unsigned r = 0; r < sizeof(rows) / sizeof(rows[0]); r++) {
            void *labelTxt = GameText_getText(*g_swd_textId);
            PaintCanvas_DrawString(canvas, font, labelTxt, rowX, y);
            String_fromInt(rowStr, rows[r].get(st));
            tw = PaintCanvas_GetTextWidth(canvas, font);
            PaintCanvas_DrawString(canvas, font, rowStr, ((y + x0) - pad) - tw, y);
            String_dtor(rowStr);
        }

        drewStats = *land;
        tab = i32(self, 0x30);
    }

    // ===== achievements / medal tab (index 1) =====
    if (drewStats != 0 || tab == 1) {
        int boxW = i32(self, 0x6c);
        int third = __aeabi_idiv(boxW, 3);
        int x0 = *(int *)((char *)layout + 0x28);
        int rowH = i32(self, 0x78);
        int gridX0 = drewStats ? (boxW + (third >> 1) + x0) : (x0 + (third >> 1));
        int gridY0 = *(int *)((char *)layout + 0xc) + (rowH >> 1) + *(int *)((char *)layout + 0x2c);

        if (drewStats != 0) {
            char hdr[0xc];
            void *t = GameText_getText(*g_swd_textId);
            String_fromText(hdr, t, false);
            Layout_drawBox(layout, 0, boxW + x0 * 2, top, x0 + boxW,
                           *(int *)((char *)layout + 0x1c), hdr);
            String_dtor(hdr);
            gridY0 += *(int *)((char *)layout + 0x1c) + *(int *)((char *)layout + 0x2c);
        }

        for (int i = 0; i < i32(self, 0x0); i++) {
            int col = (int)__aeabi_uidiv((unsigned)i, 3);
            int by = col * rowH + gridY0 + i32(self, 0x38);
            TouchButton_setPosition(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i * 4),
                                    (i - col * 3) * third + gridX0, by);
            if (by >= 0 && by <= screenH)
                TouchButton_draw(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i * 4));
        }

        // Selected-medal detail panel.
        if (i32(self, 0x34) >= 0) {
            PaintCanvas_SetColor(canvas);
            int lines = *(int *)pp(self, 0x10);
            int lineH = *(int *)((char *)layout + 0x4);
            char lbl[0xc];
            String_fromC(lbl, "", false);
            Layout_drawBox(layout, 2, *(int *)((char *)layout + 0x28),
                           (((screenH - *(int *)((char *)layout + 0x10)) -
                             *(int *)((char *)layout + 0x24)) - lineH * lines) +
                               *(int *)((char *)layout + 0x4c) * -2,
                           i32(self, 0x6c), *(int *)((char *)layout + 0x4c) * 2 + lineH * lines, lbl);
            String_dtor(lbl);

            String_fromC(lbl, "", false);
            Layout_drawBox(layout, 5, *(int *)((char *)layout + 0x28),
                           (((screenH - *(int *)((char *)layout + 0x10)) -
                             *(int *)((char *)layout + 0x24)) - lineH * lines) +
                               *(int *)((char *)layout + 0x4c) * -2,
                           i32(self, 0x6c), *(int *)((char *)layout + 0x4c) * 2 + lineH * lines, lbl);
            String_dtor(lbl);

            Globals_drawLines(*(void **)g_swd_globals, font, pp(self, 0x10),
                              *(int *)((char *)layout + 0x4c) + *(int *)((char *)layout + 0x28),
                              (char)screenH);
        }
    }

    // --- header / footer / tab buttons ---
    char header[0xc];
    void *ht = GameText_getText(*g_swd_textId);
    String_fromText(header, ht, false);
    Layout_drawHeader(layout, header);
    String_dtor(header);
    Layout_drawFooter(layout);

    if (*land == 0) {
        void **tabs = (void **)pp(self, 0x4);
        for (unsigned int i = 0; i < *(unsigned int *)tabs; i++)
            TouchButton_draw(((void **)tabs[1])[i]);
    }

    String_dtor(sep);
    String_dtor(creditStr);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
