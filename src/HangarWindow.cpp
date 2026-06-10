#include "gof2/HangarWindow.h"
#include "gof2/Globals.h"
#include "gof2/Item.h"
#include "gof2/Layout.h"
#include "gof2/Status.h"


extern "C" int *HangarList_getCurrentTabItems(void *list);
extern "C" int HangarList_getCurrentTab(void *list);
extern "C" int BluePrint_isEmpty(void *bp);
extern "C" int BluePrint_getStationIndex(void *bp);
extern "C" int Station_getIndex(void *station);
extern "C" void *Status_getStation(void *status);
extern "C" void HangarWindow_render3D_thunk(void *arg);
extern "C" void *HangarList_getCurrentItem(void *list);
extern "C" void *HangarList_dtor(void *p);
extern "C" void *ListItemWindow_dtor(void *p);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void operator_delete_arr(void *p);
extern "C" extern const char hw_ote_fmt1[], hw_ote_fmt2[], hw_ote_fmt3[], hw_ote_fmt4[];
extern "C" int ListItem_isSelectable(void *item);
extern "C" int ListItem_isTextButton(void *item);
extern "C" int ListItem_isShip(void *item);
extern "C" void Ship_adjustPrice(void *ship);
extern "C" void FModSound_play(void *snd, int id, void *a, void *b, void *c);
extern "C" extern const char hw_otb_fmt1[], hw_otb_fmt2[];
extern "C" void *HangarList_getItems(void *list);
extern "C" int ListItem_isBluePrint(void *item);
extern "C" void *Ship_getCargo(void *ship);
extern "C" void *BluePrint_getIngredientList(void *bp);
extern "C" int Item_getIndex(void *item);
extern "C" void *Status_getShip(void *status);
extern "C" extern const char hw_bp_fmt1[], hw_bp_fmt2[], hw_bp_fmt3[];
extern "C" extern const char hw_sel_fmt1[], hw_sel_fmt2[], hw_sel_fmt3[];
extern "C" extern const char hw_tx_fmt[], hw_tx_suffix[];
extern "C" extern const char hw_equip_fmt[];
extern "C" extern const char hw_freeCredits_yes[];
extern "C" extern const char hw_freeCredits_no[];
extern "C" extern const char hw_freeCredits_icon[];

// ---- isInSpecialMode_148a64.cpp ----
extern "C" bool HangarWindow_isInSpecialMode(HangarWindow *self)
{
    if (self->field_0x89 != 0) return true;
    return self->field_0x3c != 0;
}

// ---- refreshCurrentContentHeight_148a10.cpp ----
// Globals singleton (pc-rel deref -> holder; *holder is the object), field +0x70 is the row height.
__attribute__((visibility("hidden"))) extern void **g_hw_globals;

extern "C" void HangarWindow_refreshCurrentContentHeight(HangarWindow *self)
{
    int *items = HangarList_getCurrentTabItems(self->field_0x14);
    if (items != 0) {
        int n = *items;
        int rowH = G<int>(*g_hw_globals, 0x70);
        self->field_0xd4 = self->field_0x10c * (n - 1) + n * rowH;
    }
}

// ---- listMode_148a4a.cpp ----
extern "C" bool HangarWindow_listMode(HangarWindow *self)
{
    if (self->field_0x58 == 0 && self->field_0x89 == 0) {
        return self->field_0x3c == 0;
    }
    return false;
}

// ---- readyToClose_14e4f0.cpp ----
// Status singleton (pc-rel deref -> holder; *holder is the Status object).
__attribute__((visibility("hidden"))) extern void **g_hw_status;

extern "C" bool HangarWindow_readyToClose(HangarWindow *self)
{
    int tab = HangarList_getCurrentTab(self->field_0x14);
    if (tab == 4 && self->field_0x88 != 0 && self->field_0x94 > 0 &&
        self->field_0x3c == 0 && BluePrint_isEmpty(self->field_0x80) == 0) {
        int si = BluePrint_getStationIndex(self->field_0x80);
        void *st = Status_getStation(*g_hw_status);
        if (si != Station_getIndex(st)) {
            return false;
        }
    }
    return self->field_0x3c == 0;
}

// ---- render3D_1490c8.cpp ----
// Tail-call thunk; arg is this->0x18 when this->0x58 == 1.

extern "C" void HangarWindow_render3D(HangarWindow *self)
{
    if (self->field_0x58 == 1) {
        return HangarWindow_render3D_thunk(self->field_0x18);
    }
}

// ---- currentItemIsHighlighted_14e550.cpp ----
extern "C" bool HangarWindow_currentItemIsHighlighted(HangarWindow *self)
{
    void *item = HangarList_getCurrentItem(self->field_0x14);
    if (item == 0) return false;
    return item == self->field_0x68;
}

// ---- _HangarWindow_147d9c.cpp ----
// HangarWindow::~HangarWindow() -> returns this. Tears down owned windows/lists/arrays.
extern "C" HangarWindow *_ZN12HangarWindowD2Ev(HangarWindow *self)
{
    void *p;
    p = self->field_0x14;
    if (p != 0) operator_delete(HangarList_dtor(p));
    self->field_0x14 = 0;
    p = self->field_0x18;
    if (p != 0) operator_delete(ListItemWindow_dtor(p));
    self->field_0x18 = 0;
    p = self->field_0x1c;
    if (p != 0) operator_delete(ChoiceWindow_dtor(p));
    self->field_0x1c = 0;
    p = self->field_0x20;
    if (p != 0) operator_delete(ChoiceWindow_dtor(p));
    self->field_0x20 = 0;
    p = self->field_0x4;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = self->field_0x4;
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    self->field_0x4 = 0;
    p = self->field_0x24;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = self->field_0x24;
        if (q != 0) operator_delete(Array_TouchButton_dtor(q));
    }
    p = self->field_0x30;
    self->field_0xc = 0;
    self->field_0x24 = 0;
    if (p != 0) operator_delete_arr(p);
    self->field_0x30 = 0;
    return self;
}

// ---- getRelativeScrollStartPos_14b6bc.cpp ----
// Returns float in r0. If scroll range (this+0xb4) >= 1 returns a constant; otherwise
// returns -(float)range / (float)contentHeight(this+0xd4).
extern "C" float HangarWindow_getRelativeScrollStartPos(HangarWindow *self)
{
    int range = self->field_0xb4;
    if (range > 0) {
        union { unsigned u; float f; } c; c.u = 0x4650a903u;
        return c.f;
    }
    return -(float)range / (float)self->field_0xd4;
}

// ---- hideMessage_14ecc0.cpp ----
extern "C" void HangarWindow_hideMessage(HangarWindow *self)
{
    self->field_0x3c = 0;
}

// ---- render_1490d8.cpp ----
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
void PaintCanvas_SetColor(unsigned int color);
int PaintCanvas_GetImage2DWidth(void *canvas);
int PaintCanvas_GetImage2DHeight(void *canvas);
int PaintCanvas_GetTextWidth(void *canvas, void *text);
int PaintCanvas_GetTextHeight(void *canvas);
void PaintCanvas_DrawImage2D(void *canvas, int img, int x, int y, char anchor);
void PaintCanvas_DrawRegion2D(void *canvas, unsigned int img, int sx, int sy, int sw, int sh,
                              float fw, int a, int b, int c, int y);
void PaintCanvas_DrawString(void *canvas, void *font, int text, int x, char anchor);

unsigned int HangarList_getCurrentTab(void *list);
void *HangarList_getCurrentTabItems(void *list);

int ListItem_isSelectable(void *item);
int ListItem_isTextButton(void *item);
int ListItem_isItem(void *item);
int ListItem_isShip(void *item);
int ListItem_isSlot(void *item);
int ListItem_isBluePrint(void *item);
int ListItem_isPendingProduct(void *item);
int ListItem_isMoveToCargoButton(void *item);
int ListItem_isSellButton(void *item);

void Layout_drawBG(void *layout);
void Layout_drawBox(void *layout, int kind, int x, int y, int w, int h, String12 *text);
void Layout_drawScrollBar(void *layout, int a, int b, int c, int d, int e);
void Layout_drawHeader(void *layout, String12 *text);
void Layout_drawFooter(void *layout);
void Layout_formatCredits(String12 *out, int amount);

int BluePrint_getIndex(void *bp);
float BluePrint_getCompletionRate(void *bp);
int BluePrint_getCurrentAmount(void *bp, int idx);
int BluePrint_getTotalAmount(void *bp, int idx);

int Item_getIndex(void *item);
int Item_getType(void *item);
int Item_getSort(void *item);
int Item_getAmount(void *item);
int Item_getStationAmount(void *item);
int Item_getBlueprintAmount(void *item);
int Item_getSinglePrice(void *item);
int Item_canBeInstalledMultipleTimes(void *item);
int Item_isInList(void *item, void *arr);

int Ship_getIndex(void *ship);
int Ship_getPrice(void *ship);
int Ship_hasEquipment(void *ship, int idx);
int Ship_getFreeSlots(void *ship, int type);
void *Ship_getFirstEquipmentOfSort(void *ship, int sort);

void *Status_getShip();
void *Status_getStation();
int Status_getCredits();
void *Station_getItems(void *station);

void *GameText_getText(int id);

void ImageFactory_drawShip(void *factory, int idx, int x, int y);
void ImageFactory_drawItem(void *factory, int idx, int type, int x, int y);

void TouchButton_setVisible(void *btn, bool vis);
void TouchButton_setPosition(void *btn, int x, int y, char anchor);
void TouchButton_setPosition2(void *btn, int x, int y);
void TouchButton_setAlwaysPressed(void *btn, bool flag);
void TouchButton_setText(void *btn, void *text);
void TouchButton_setSplitText(void *btn, void *text);
void TouchButton_replaceTextKeepSize(void *btn, void *text);
int TouchButton_getWidth(void *btn);
int TouchButton_getHeight(void *btn);
void TouchButton_draw(void *btn);

void ChoiceWindow_draw(void *win);
void ChoiceWindow_setHeight(void *win, int h);

void ListItemWindow_draw(void *win);

void *AppManager_GetApplicationData();
void *AppManager_GetApplicationModule(unsigned int id);

void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_int(String12 *self, int v);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);

int __aeabi_idiv(int a, int b);
unsigned int __aeabi_uidiv(unsigned int a, unsigned int b);

float HangarWindow_getRelativeScrollStartPos(HangarWindow *self);
float HangarWindow_getRelativeScrollHeight(HangarWindow *self);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;        // *piVar37
__attribute__((visibility("hidden"))) extern void **g_hw_canvas;        // *puVar19
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;     // *piVar21
__attribute__((visibility("hidden"))) extern int *g_hw_screenHeight;    // *piVar41
__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern void **g_hw_font;
__attribute__((visibility("hidden"))) extern void **g_hw_recordHandler;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_optionFlags;
__attribute__((visibility("hidden"))) extern unsigned int *g_hw_dlcModuleId;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_blackMarketHintFlag;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_introHintFlag;
__attribute__((visibility("hidden"))) extern int *g_hw_headerTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_shipNameBase;
__attribute__((visibility("hidden"))) extern int *g_hw_itemNameBase;
__attribute__((visibility("hidden"))) extern float g_hw_lineScale;
extern "C" extern const char hw_rnd_empty[], hw_rnd_a[], hw_rnd_b[], hw_rnd_c[],
    hw_rnd_d[], hw_rnd_e[], hw_rnd_x[];

extern "C" void HangarWindow_render(HangarWindow *self)
{
    void *layout = *g_hw_layout;
    void *canvas = *g_hw_canvas;
    PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);

    void *dlc = AppManager_GetApplicationModule(*g_hw_dlcModuleId);
    bool dlcShown = dlc != 0 && G<uint8_t>(dlc, 0x18) != 0;

    if (dlc == 0 || !dlcShown) {
        int tab2 = self->field_0x58;
        if (tab2 == 0) {
            Layout_drawBG(layout);
            unsigned int tab = HangarList_getCurrentTab(self->field_0x14);
            Array<void *> *items = (Array<void *> *)HangarList_getCurrentTabItems(self->field_0x14);
            if (items != 0) {
                float startPos = HangarWindow_getRelativeScrollStartPos(self);
                float visH = (float)self->field_0xd8;
                float scrollH = HangarWindow_getRelativeScrollHeight(self);
                int scrollPx = (int)(scrollH * visH);
                int startPx = (int)(startPos * visH);

                int topY = *g_hw_screenWidth;
                if (scrollPx > 0)
                    topY = (topY - layout->field_0x48) - layout->field_0x2c;

                int rowGap;
                if (tab == 0)
                    rowGap = layout->field_0x4c << 1;
                else if (tab == 4 || tab == 1)
                    rowGap = layout->field_0x50;
                else
                    rowGap = 0;

                topY += (layout->field_0x28 + self->field_0xf4) * -2;
                int baseY = layout->field_0x2cc;
                int colW = layout->field_0x4c;

                // Black-market scrolling hint strip.
                if (*g_hw_blackMarketHintFlag != 0 && *g_hw_introHintFlag == 0) {
                    int iw = PaintCanvas_GetImage2DWidth(canvas);
                    int ih = PaintCanvas_GetImage2DHeight(canvas);
                    int rows = __aeabi_idiv(*g_hw_screenHeight, 1);
                    int y = 0;
                    for (int r = 0; r <= rows; r++) {
                        PaintCanvas_DrawImage2D(canvas, self->field_0xf0,
                            (layout->field_0x28 - iw) + self->field_0xf4, y, 1);
                        int off = (scrollPx < 1) ? 0 : (layout->field_0x48 + layout->field_0x2c);
                        PaintCanvas_DrawImage2D(canvas, self->field_0xf0,
                            self->field_0xf4 + layout->field_0x28 + topY + off, y, 0);
                        y += ih;
                    }
                }

                int contentBase = colW + baseY + rowGap;

                // Hide all action buttons before re-laying them out per row.
                Array<void *> *btnArr = (Array<void *> *)self->field_0x24;
                for (int i = 0; i != 0x18; i++) {
                    if (self->field_0xd0 == 0) {
                        void *btn = G<void *>(G<void *>(self->field_0x24, 4), i * 4);
                        if (btn != 0)
                            TouchButton_setVisible(btn, false);
                    }
                }

                int boxW = rowGap - 2;

                for (unsigned int i = 0; i < items->length; i++) {
                    int y = (layout->field_0x70 + self->field_0x10c) * (int)i +
                            self->field_0xb4 + layout->field_0x20 + layout->field_0xc;
                    if (y < 0 || y > *g_hw_screenHeight)
                        continue;

                    void *li = items->data[i];
                    if (ListItem_isSelectable(li) == 0)
                        continue;

                    // Row background box (selected vs unselected, by tab).
                    if (self->field_0x68 == li && ListItem_isTextButton(li) == 0) {
                        String12 boxText;
                        if (tab == 0 && G<int>(li, 0x3c) >= 0) {
                            AEString_ctor(&boxText, hw_rnd_a, false);
                            Layout_drawBox(layout, 10, self->field_0xf4 + layout->field_0x28, y,
                                           topY, layout->field_0x70, &boxText);
                        } else {
                            AEString_ctor(&boxText, hw_rnd_b, false);
                            Layout_drawBox(layout, 4, self->field_0xf4 + layout->field_0x28, y,
                                           topY, layout->field_0x70, &boxText);
                        }
                        AEString_dtor(&boxText);
                    } else if (tab != 0 || G<int>(li, 0x3c) < 0) {
                        String12 boxText;
                        AEString_ctor(&boxText, hw_rnd_c, false);
                        Layout_drawBox(layout, 3, self->field_0xf4 + layout->field_0x28, y, topY,
                                       layout->field_0x70, &boxText);
                        AEString_dtor(&boxText);
                    } else {
                        String12 boxText;
                        AEString_ctor(&boxText, hw_rnd_d, false);
                        Layout_drawBox(layout, 9, self->field_0xf4 + layout->field_0x28, y, topY,
                                       layout->field_0x70, &boxText);
                        AEString_dtor(&boxText);
                    }

                    PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                    String12 label;
                    AEString_ctor(&label, hw_rnd_empty, false);

                    if (ListItem_isItem(li) == 0) {
                        if (ListItem_isShip(li) != 0) {
                            // Ship row: name + price + ship icon.
                            Ship_getIndex(G<void *>(li, 0xc));
                            AEString_assign(&label, (String12 *)GameText_getText(*g_hw_shipNameBase));
                            PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                            String12 price;
                            Layout_formatCredits(&price, Ship_getPrice(G<void *>(li, 0xc)));
                            PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&price,
                                contentBase + layout->field_0x28 + self->field_0xf4, 1);
                            AEString_dtor(&price);
                            ImageFactory_drawShip(*g_hw_globals, Ship_getIndex(G<void *>(li, 0xc)),
                                self->field_0xf4 + layout->field_0x28 + rowGap,
                                self->field_0x118 + y);
                            PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                        } else if (ListItem_isSlot(li) != 0) {
                            AEString_assign(&label, (String12 *)GameText_getText(*g_hw_itemNameBase));
                            if (tab == 4 && i == items->length - 1) {
                                TouchButton_setPosition(
                                    G<void *>(G<void *>(self->field_0x24, 4), 0x5c),
                                    self->field_0xf4 + layout->field_0x28 + topY / 2,
                                    layout->field_0x114 + y, 0x14);
                                TouchButton_setVisible(
                                    G<void *>(G<void *>(self->field_0x24, 4), 0x5c), true);
                                TouchButton_draw(G<void *>(G<void *>(self->field_0x24, 4), 0x5c));
                                String12 tmp;
                                AEString_ctor(&tmp, hw_rnd_x, false);
                                AEString_assign(&label, &tmp);
                                AEString_dtor(&tmp);
                            }
                        } else if (ListItem_isBluePrint(li) != 0) {
                            BluePrint_getIndex(G<void *>(li, 0x8));
                            AEString_assign(&label, (String12 *)GameText_getText(*g_hw_itemNameBase));
                            float rate = BluePrint_getCompletionRate(G<void *>(li, 0x8));
                            if (rate > 0.0f) {
                                PaintCanvas_DrawImage2D(canvas, self->field_0x78,
                                    layout->field_0x28 + contentBase + 2 + self->field_0xf4, 0, 0);
                                float dcw = (float)self->field_0xdc;
                                PaintCanvas_DrawRegion2D(canvas, self->field_0x7c, 0, 0,
                                    (int)(rate * dcw), self->field_0xe0, (float)(int)(rate * dcw),
                                    0, 0, 0,
                                    layout->field_0x28 + contentBase + 3 + self->field_0xf4);
                                String12 pct, sfx, sum;
                                AEString_ctor_int(&pct, (int)(rate * startPos));
                                AEString_ctor(&sfx, hw_rnd_x, false);
                                AEString_add(&sum, &pct, &sfx);
                                PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&sum,
                                    contentBase + 2 + layout->field_0x28 + self->field_0xf4 +
                                        self->field_0xdc + layout->field_0x2c, 0);
                                AEString_dtor(&sum);
                                AEString_dtor(&sfx);
                                AEString_dtor(&pct);
                                PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                            }
                            int bpIdx = BluePrint_getIndex(G<void *>(li, 0x8));
                            int type = Item_getType(G<void *>(G<void *>(*g_hw_globals, 0x4), bpIdx));
                            ImageFactory_drawItem(*g_hw_globals, bpIdx, type,
                                layout->field_0x28 + rowGap + self->field_0xf4,
                                self->field_0x118 + y);
                        } else if (ListItem_isPendingProduct(li) != 0) {
                            int amt = G<int>(G<void *>(li, 0x18), 0x10);
                            String12 head;
                            if (amt < 2) {
                                AEString_ctor(&head, hw_rnd_e, false);
                            } else {
                                String12 num, sfx;
                                AEString_ctor_int(&num, amt);
                                AEString_ctor(&sfx, hw_rnd_e, false);
                                AEString_add(&head, &num, &sfx);
                                AEString_dtor(&sfx);
                                AEString_dtor(&num);
                            }
                            String12 full;
                            AEString_add(&full, &head, (String12 *)GameText_getText(*g_hw_itemNameBase));
                            AEString_assign(&label, &full);
                            AEString_dtor(&full);
                            AEString_dtor(&head);
                            int pidx = G<int>(G<void *>(li, 0x18), 0x14);
                            int type = Item_getType(G<void *>(G<void *>(*g_hw_globals, 0x4), pidx));
                            ImageFactory_drawItem(*g_hw_globals, pidx, type,
                                rowGap + layout->field_0x28 + self->field_0xf4,
                                self->field_0x118 + y);
                        } else if (ListItem_isMoveToCargoButton(li) != 0) {
                            TouchButton_setPosition(
                                G<void *>(G<void *>(self->field_0x24, 4), 0x18),
                                self->field_0xf4 + layout->field_0x28, y, 0x11);
                            TouchButton_setVisible(
                                G<void *>(G<void *>(self->field_0x24, 4), 0x18), true);
                            TouchButton_draw(G<void *>(G<void *>(self->field_0x24, 4), 0x18));
                        } else if (ListItem_isSellButton(li) != 0) {
                            TouchButton_setPosition(
                                G<void *>(G<void *>(self->field_0x24, 4), 0x14),
                                self->field_0xf4 + layout->field_0x28, y, 0x11);
                            TouchButton_setVisible(
                                G<void *>(G<void *>(self->field_0x24, 4), 0x14), true);
                            TouchButton_draw(G<void *>(G<void *>(self->field_0x24, 4), 0x14));
                        } else {
                            String12 txt;
                            AEString_ctor_str(&txt, G<String12 *>(li, 0x1c), false);
                            Layout_drawBox(layout, 0, self->field_0xf4 + layout->field_0x28,
                                           (y + layout->field_0x70) - layout->field_0x1c, topY,
                                           layout->field_0x1c, &txt);
                            AEString_dtor(&txt);
                        }
                    } else {
                        // Regular item row: name + amounts + price + icon.
                        Item_getIndex(G<void *>(li, 0x10));
                        AEString_assign(&label, (String12 *)GameText_getText(*g_hw_itemNameBase));
                        PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                        if (self->field_0x11d == 0) {
                            String12 price;
                            Item_getSinglePrice(G<void *>(li, 0x10));
                            Layout_formatCredits(&price, Item_getSinglePrice(G<void *>(li, 0x10)));
                            PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&price,
                                contentBase + layout->field_0x28 + self->field_0xf4, 1);
                            AEString_dtor(&price);
                        }
                        int iidx = Item_getIndex(G<void *>(li, 0x10));
                        int itype = Item_getType(G<void *>(li, 0x10));
                        ImageFactory_drawItem(*g_hw_globals, iidx, itype,
                            layout->field_0x28 + rowGap + self->field_0xf4,
                            self->field_0x118 + y);
                    }

                    PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&label,
                        self->field_0xf4 + layout->field_0x28 + contentBase, 0);

                    AEString_dtor(&label);
                }

                // Scroll bar when the content overflows.
                if (scrollPx > 0 || startPx > 0) {
                    Layout_drawScrollBar(layout,
                        ((*g_hw_screenHeight - layout->field_0x48) - layout->field_0x28) -
                            self->field_0xf4,
                        layout->field_0x20 + layout->field_0xc, self->field_0xd8, startPx,
                        scrollPx);
                }
            }

            String12 header;
            AEString_ctor(&header, (const char *)GameText_getText(*g_hw_headerTextId), false);
            Layout_drawHeader(layout, &header);
            AEString_dtor(&header);

            Array<void *> *tabs = (Array<void *> *)self->field_0x4;
            for (unsigned int i = 0; i < tabs->length; i++)
                TouchButton_draw(tabs->data[i]);
        }

        if (self->field_0x58 == 1) {
            self->field_0x58 = 0;
            HangarWindow_render(self);
            self->field_0x58 = 1;
            ListItemWindow_draw(self->field_0x18);
        }
    }

    // --- Footer + credits button (always drawn). ---
    layout = *g_hw_layout;
    Layout_drawFooter(layout);
    void *btns = self->field_0x24;
    TouchButton_setVisible(G<void *>(G<void *>(btns, 4), 0x2c), true);
    TouchButton_setAlwaysPressed(G<void *>(G<void *>(btns, 4), 0x2c), g_hw_optionFlags[0x4e] == 0);
    {
        String12 credits;
        Layout_formatCredits(&credits, Status_getCredits());
        TouchButton_setText(G<void *>(G<void *>(btns, 4), 0x2c), &credits);
        AEString_dtor(&credits);
    }
    TouchButton_draw(G<void *>(G<void *>(btns, 4), 0x2c));

    if (self->field_0x3c == 0)
        return;

    ChoiceWindow_draw(self->field_0x20);

    if (self->field_0xae == 0) {
        if (self->field_0xb0 != 0) {
            // Free-credits action button column.
            for (unsigned int i = 0; i < 5; i++) {
                void *b = G<void *>(G<void *>(btns, 4), i * 4 + 0x48);
                bool vis;
                if (i == 0 || (i == 1 && g_hw_optionFlags[0x49] != 0) ||
                    (i == 2 && g_hw_optionFlags[0x4a] != 0) ||
                    (i == 3 && g_hw_optionFlags[0x4b] != 0))
                    vis = false;
                else
                    vis = (i != 4 || g_hw_optionFlags[0x4c] == 0);
                TouchButton_setVisible(b, vis);
                TouchButton_setPosition2(b, layout->field_0x28 + G<int>(self->field_0x20, 0),
                                         layout->field_0x8);
                TouchButton_draw(b);
            }
        }
    } else {
        // Buy-credits / IAP grid.
        void *appData = AppManager_GetApplicationData();
        (void)appData;
        if (self->field_0x11f == 0) {
            // Five fixed-label buttons.
            for (int slot = 0x30; slot <= 0x40; slot += 4) {
                void *b = G<void *>(G<void *>(btns, 4), slot);
                TouchButton_setVisible(b, true);
                String12 t;
                AEString_ctor(&t, hw_rnd_x, false);
                TouchButton_setText(b, &t);
                AEString_dtor(&t);
                TouchButton_setPosition(b, *g_hw_screenWidth / 2, 0, 0x14);
                TouchButton_draw(b);
            }
        } else {
            // List-mode: per-index labelled buttons with icons.
            for (unsigned int i = 0; i <= 4; i++) {
                void *b = G<void *>(G<void *>(btns, 4), i * 4 + 0x30);
                TouchButton_setVisible(b, true);
                String12 label, split;
                switch (i) {
                case 0: AEString_ctor(&label, hw_rnd_a, false); AEString_ctor(&split, hw_rnd_b, false); break;
                case 1: AEString_ctor(&label, hw_rnd_b, false); AEString_ctor(&split, hw_rnd_c, false); break;
                case 2: AEString_ctor(&label, hw_rnd_c, false); AEString_ctor(&split, hw_rnd_d, false); break;
                case 3: AEString_ctor(&label, hw_rnd_d, false); AEString_ctor(&split, hw_rnd_e, false); break;
                default: AEString_ctor(&label, hw_rnd_e, false); AEString_ctor(&split, hw_rnd_x, false); break;
                }
                unsigned int rowOff = __aeabi_uidiv(i & 0xff, 3);
                int x = (self->field_0x128 + self->field_0x120) *
                            ((i + rowOff * -3) & 0xff) +
                        ((*g_hw_screenHeight / 2 - self->field_0x120) - self->field_0x128);
                int yy = (int)((float)((layout->field_0x20 * -3) +
                            (*g_hw_screenWidth / 2 - self->field_0x124 / 2)) +
                            (float)self->field_0x12c * -0.5f +
                            (float)((int)rowOff * (self->field_0x12c + self->field_0x124)));
                TouchButton_setPosition(b, x, yy, 'D');
                TouchButton_replaceTextKeepSize(b, &label);
                TouchButton_setSplitText(b, &split);
                TouchButton_draw(b);
                PaintCanvas_DrawImage2D(canvas, G<int>(self->field_0x30, i * 4), x,
                                        yy - layout->field_0x2c, 0x11);
                AEString_dtor(&split);
                AEString_dtor(&label);
            }
        }

        if (self->field_0x11f == 0) {
            int h17 = layout->field_0x30;
            int h34 = layout->field_0x34;
            int th = PaintCanvas_GetTextHeight(canvas);
            ChoiceWindow_setHeight(self->field_0x20, (h34 + h17) * 6 + th * 2);
        }
    }
}

// ---- OnTouchEnd_14c740.cpp ----
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
int Layout_OnTouchEnd(void *layout, int touch, int coord);
int Layout_helpPressed(void *layout);
void Layout_initHelpWindow(void *layout, String12 *text);
void Layout_resetWindowDimensions(void *layout);
void Layout_formatCredits(String12 *out, int amount);
void ListItemWindow_OnTouchEnd(void *win, int touch);
int TouchButton_OnTouchEnd(void *btn, int touch);
void TouchButton_setVisible(void *btn, bool vis);
void TouchButton_resetTouch(void *btn);
int ChoiceWindow_OnTouchEnd(void *win, int touch);
void ChoiceWindow_set(void *win, void *text);
void ChoiceWindow_setMsg(void *win, String12 *msg, bool flag);
void ChoiceWindow_setChoices(void *win, String12 *msg, void *a, bool flag, void *b, void *c, void *d, int e, int f);

unsigned int HangarList_getCurrentTab(void *list);
int HangarList_getCurrentLength(void *list);
void HangarList_setCurrentTab(void *list, bool flag);
void HangarList_setCurrentItemIndex(void *list, int idx);
void *HangarList_getCurrentItemAt(void *list, int n);
void HangarList_initShopTab(void *list, void *items, void *ships);
void HangarList_initShipTab(void *list, void *ship);

int BluePrint_getAutoCompletionPrice(void *bp);
int BluePrint_isEmpty(void *bp);
void BluePrint_complete(void *bp);

void *Status_getStation();
void *Status_getShip();
void *Status_getSystem();
void *Status_getStanding();
int Status_getCredits();
void Status_setCredits(void *globals);
void Status_changeCredits(void *globals);
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b);
void Status_setShip(void *ship);

int Station_getIndex(void *station);
void *Station_getName(void *station);
void *Station_getShips(void *station);
void *Station_getItems(void *station);
int Station_hasShip(void *station, int idx);
void Station_addShip(void *station, void *ship);
void Station_removeShip(void *station);
void Station_setShips(void *arr, bool flag);

int Ship_getIndex(void *ship);
int Ship_getPrice(void *ship);
int Ship_getRace(void *ship);
void Ship_setRace(void *ship, int race);
void Ship_adjustPrice(void *ship);
void Ship_refreshValue(void *ship);
void *Ship_getEquipment(void *ship);
void *Ship_getCargo(void *ship);
void *Ship_getMods(void *ship);
void *Ship_makeShip(int idx);
void Ship_addCargo(void *ship, void *item);
unsigned int Ship_addEquipment(void *ship, void *item);
void Ship_addMod(void *ship, int mod);
void Ship_setCargo(void *ship, void *arr);
void *Ship_dtor(void *ship);

int Item_getBlueprintAmount(void *item);
void Item_setStationAmount(void *item, int v);
void Item_setAmount(void *item, int v);
void Item_setBlueprintAmount(void *item, int v);
void *Item_extractItems(void *arr, bool flag);
void *Item_mixItems(void *a, void *b);
void *Item_clone(void *item);
int ListItem_getPrice(void *item);

void *GameText_getText(int id);

void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);

void RecordHandler_saveOptions(void *rh);
void RecordHandler_recordStoreWrite(void *rh, int slot);
void RecordHandler_recordStoreWritePreview(void *rh, int slot);

void NFC_free_credits_likeGOF2OnFacebook();
void NFC_free_credits_likeFishlabsOnFacebook();
void NFC_free_credits_subscribeToYoutubeChannel();
void NFC_free_credits_followOnTwitter();
void NFC_free_credits_rateGame();
void NFC_iap_buy_credits_100_000();
void NFC_iap_buy_credits_300_000();
void NFC_iap_buy_credits_1_000_000();
void NFC_iap_buy_credits_3_000_000();
void NFC_iap_buy_credits_10_000_000();

void *AppManager_GetApplicationData();
void *AppManager_GetApplicationModule(unsigned int id);
void ModStation_showDlcMenu(void *mod);
void ArrayReleaseClasses_ItemPtr(void *arr);
void *Array_ItemPtr_dtor(void *arr);
void ArrayRemove_ShipPtr(void *ship, void *arr);
int __aeabi_idiv(int a, int b);
void operator_delete(void *p);

void HangarWindow_refreshCargoAvailabilityForBlueprints(HangarWindow *self);
void HangarWindow_refreshCurrentContentHeight(HangarWindow *self);
int HangarWindow_currentItemIsHighlighted(HangarWindow *self);
void HangarWindow_setSellMode(HangarWindow *self);
void HangarWindow_highlightItem(HangarWindow *self, void *item);
void HangarWindow_showCreditsBuyWindow(HangarWindow *self);
void HangarWindow_showFreeCreditsWindow(HangarWindow *self);
int HangarWindow_readyToClose(HangarWindow *self);
void HangarWindow_mountItem(HangarWindow *self, void *item);
void HangarWindow_demountItem(HangarWindow *self, void *item, int slot);
// Mission-offer subroutine (corrupted in decompile); kept as an extern helper.
void HangarWindow_buildMissionOffer(HangarWindow *self, int touch, int coord);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern void **g_hw_layout;
__attribute__((visibility("hidden"))) extern void **g_hw_recordHandler;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_optionFlags;
__attribute__((visibility("hidden"))) extern unsigned int *g_hw_dlcModuleId;
__attribute__((visibility("hidden"))) extern unsigned int *g_hw_modStationId;
__attribute__((visibility("hidden"))) extern int *g_hw_bpDoneTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_notEnoughTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_helpTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_sellShipTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_baseTextId;

extern "C" void HangarWindow_OnTouchEnd(HangarWindow *self, int touch, int coord)
{
    void *globals = *g_hw_globals;
    self->field_0x6c = 0;
    self->field_0x70 = 0;
    self->field_0xd0 = 0;
    if (self->field_0xd1 != 0) {
        self->field_0xd1 = 0;
        return;
    }

    if (self->field_0x3c == 0) {
        // --- No modal dialog active: normal list / button handling. ---
        if (self->field_0x58 == 1)
            ListItemWindow_OnTouchEnd(self->field_0x18, touch);

        void *layout = *g_hw_layout;
        int handled = Layout_OnTouchEnd(layout, touch, coord);
        if (handled == 0) {
            // Inertial scroll bookkeeping.
            int delta = self->field_0xc0;
            int newScroll = self->field_0xb4 + delta;
            float vel = (float)delta;
            int absd = delta < 0 ? -delta : delta;
            self->field_0xc8 = (absd > 3) ? (int)vel : 0;
            self->field_0xc4 = 0;
            self->field_0xb4 = newScroll;
            self->field_0xbc = newScroll;

            Array<void *> *tabs = (Array<void *> *)self->field_0x4;
            for (unsigned int i = 0; i < tabs->length; i++) {
                if (TouchButton_OnTouchEnd(tabs->data[i], touch) != 0) {
                    HangarWindow_setSellMode(self);
                    HangarWindow_setSellMode(self);
                    HangarWindow_setSellMode(self);
                    self->field_0x68 = 0;
                    HangarList_setCurrentTab(self->field_0x14, i != 0);
                    if (i == 2)
                        HangarWindow_refreshCargoAvailabilityForBlueprints(self);
                    HangarWindow_refreshCurrentContentHeight(self);
                    self->field_0xb4 = 0;
                    self->field_0xbc = 0;
                    HangarList_setCurrentItemIndex(self->field_0x14, -1);
                }
            }

            if (layout->field_0xc < coord) {
                int row = __aeabi_idiv(
                    (((coord - layout->field_0xc) - layout->field_0x20) - self->field_0x10c) -
                        self->field_0xb4,
                    layout->field_0x70 + self->field_0x10c);
                if (row < HangarList_getCurrentLength(self->field_0x14)) {
                    HangarList_setCurrentItemIndex(self->field_0x14, row);
                    if (HangarWindow_currentItemIsHighlighted(self) != 0 &&
                        self->field_0xd2 != 0) {
                        HangarWindow_setSellMode(self);
                        HangarWindow_setSellMode(self);
                    }
                }
            }
            if (self->field_0xd2 != 0) {
                self->field_0xd2 = 0;
                return;
            }

            // "Auto-complete blueprint" button.
            if (TouchButton_OnTouchEnd(G<void *>(G<void *>(self->field_0x24, 4), 0x5c), touch) != 0) {
                BluePrint_getAutoCompletionPrice(self->field_0x80);
                String12 line, priceStr, fmt, msg;
                AEString_ctor(&line, (const char *)GameText_getText(*g_hw_bpDoneTextId), false);
                Layout_formatCredits(&priceStr, 0);
                AEString_ctor(&fmt, hw_ote_fmt1, false);
                Status_replaceHash(&msg, globals, &line, &priceStr);
                ChoiceWindow_setMsg(self->field_0x20, &msg, true);
                AEString_dtor(&msg);
                AEString_dtor(&fmt);
                AEString_dtor(&priceStr);
                AEString_dtor(&line);
                self->field_0xb1 = 1;
                self->field_0x3c = 1;
            }

            if (HangarWindow_currentItemIsHighlighted(self) != 0) {
                Array<void *> *btns = (Array<void *> *)self->field_0x24;
                for (unsigned int i = 0; i < btns->length; i++) {
                    if (TouchButton_OnTouchEnd(btns->data[i], touch) != 0) {
                        // Dispatch on the slot index (original used a jump table).
                        if ((i & 0x7fffffff) < 0xc)
                            return;
                    }
                }
            }

            if (Layout_helpPressed(layout) != 0) {
                String12 help;
                if (self->field_0x58 == 1) {
                    AEString_ctor(&help, (const char *)GameText_getText(*g_hw_helpTextId), false);
                    Layout_initHelpWindow(layout, &help);
                    AEString_dtor(&help);
                } else {
                    unsigned int t = HangarList_getCurrentTab(self->field_0x14);
                    if (t <= 4) {
                        AEString_ctor(&help, (const char *)GameText_getText(*g_hw_helpTextId), false);
                        Layout_initHelpWindow(layout, &help);
                        AEString_dtor(&help);
                    }
                }
            }

            // "Buy credits" footer button.
            if (TouchButton_OnTouchEnd(G<void *>(G<void *>(self->field_0x24, 4), 0x2c), touch) != 0) {
                g_hw_optionFlags[0x4e] = 1;
                RecordHandler_saveOptions(*g_hw_recordHandler);
                HangarWindow_showCreditsBuyWindow(self);
            }
            return;
        }

        if (self->field_0x58 == 1) {
            Layout_resetWindowDimensions(layout);
            self->field_0x58 = 0;
            return;
        }

        unsigned int tab = HangarList_getCurrentTab(self->field_0x14);
        if (tab == 4) {
            HangarWindow_setSellMode(self);
            self->field_0x68 = 0;
            HangarList_setCurrentTab(self->field_0x14, true);
            HangarWindow_refreshCargoAvailabilityForBlueprints(self);
            HangarWindow_refreshCurrentContentHeight(self);
            self->field_0xb4 = 0;
            self->field_0xbc = 0;
            HangarList_setCurrentItemIndex(self->field_0x14, -1);
            return;
        }
        if (HangarList_getCurrentTab(self->field_0x14) == 3) {
            HangarList_setCurrentTab(self->field_0x14, false);
            HangarWindow_refreshCargoAvailabilityForBlueprints(self);
            HangarWindow_refreshCurrentContentHeight(self);
        } else if (HangarWindow_readyToClose(self) != 0) {
            HangarWindow_setSellMode(self);
            self->field_0x68 = 0;
            HangarList_setCurrentItemIndex(self->field_0x14, -1);
        }
        return;
    }

    // --- A modal dialog is active (self+0x3c set). Route to the right handler. ---
    if (self->field_0xb1 != 0) {
        // Blueprint auto-completion confirmation.
        int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
        if (r == 1) {
            self->field_0x3c = 0;
        } else if (r == 0) {
            int price = BluePrint_getAutoCompletionPrice(self->field_0x80);
            if (Status_getCredits() < price) {
                String12 line, priceStr, fmt, msg, suffix, combined;
                AEString_ctor(&line, (const char *)GameText_getText(*g_hw_notEnoughTextId), false);
                Layout_formatCredits(&priceStr, Status_getCredits());
                AEString_ctor(&priceStr, hw_ote_fmt2, false);
                AEString_ctor(&fmt, hw_ote_fmt2, false);
                Status_replaceHash(&msg, globals, &line, &priceStr);
                AEString_dtor(&fmt);
                AEString_dtor(&priceStr);
                AEString_ctor(&suffix, hw_ote_fmt3, false);
                GameText_getText(*g_hw_notEnoughTextId);
                AEString_add(&combined, &suffix, &suffix);
                AEString_addAssign(&msg, &combined);
                AEString_dtor(&combined);
                AEString_dtor(&suffix);
                ChoiceWindow_setMsg(self->field_0x20, &msg, true);
                self->field_0xaf = 1;
                AEString_dtor(&msg);
            } else {
                self->field_0x3c = 0;
                if (self->field_0x80 != 0) {
                    if (BluePrint_isEmpty(self->field_0x80) != 0) {
                        G<int>(self->field_0x80, 0x10) = Station_getIndex(Status_getStation());
                        Station_getName(Status_getStation());
                        // String12 nameOut; assign into bp+0x14 (skipped: name handled inline)
                    }
                    BluePrint_complete(self->field_0x80);
                    HangarWindow_highlightItem(self,
                        HangarList_getCurrentItemAt(self->field_0x14, 1));
                    self->field_0x88 = 1;
                    HangarWindow_setSellMode(self);
                    Status_changeCredits(globals);
                }
            }
        }
        self->field_0xb1 = 0;
        return;
    }

    if (self->field_0x11c != 0) {
        // Equipment-replacement confirmation.
        int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
        if (r == 1) {
            self->field_0x3c = 0;
            self->field_0x11c = 0;
            self->field_0xb4 = self->field_0xe4;
        } else if (r == 0 && self->field_0x28 != 0 && self->field_0x2c != 0) {
            HangarWindow_demountItem(self, self->field_0x2c, -1);
            self->field_0xe4 = self->field_0xb4;
            HangarWindow_mountItem(self, self->field_0x28);
            self->field_0x3c = 0;
            self->field_0x11c = 0;
        }
        // fallthrough to buy/sell confirmation below
    } else if (self->field_0xaf != 0) {
        // "Not enough credits" -> offer to buy credits.
        int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
        if (r == 1) {
            self->field_0xaf = 0;
            self->field_0x3c = 0;
        } else if (r == 0) {
            g_hw_optionFlags[0x4e] = 1;
            RecordHandler_saveOptions(*g_hw_recordHandler);
            HangarWindow_showCreditsBuyWindow(self);
        }
    } else if (self->field_0xae == 0) {
        if (self->field_0xb0 != 0) {
            // Free-credits action buttons.
            int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
            if (r == 0) {
                for (int i = 0x12; i != 0x17; i++)
                    TouchButton_setVisible(G<void *>(G<void *>(self->field_0x24, 4), i * 4), false);
                self->field_0xb0 = 0;
                HangarWindow_showCreditsBuyWindow(self);
            }
            void *appData = AppManager_GetApplicationData();
            void *rh = *g_hw_recordHandler;
            for (unsigned int i = 0; i != 5; i++) {
                if (TouchButton_OnTouchEnd(
                        G<void *>(G<void *>(self->field_0x24, 4), i * 4 + 0x48), touch) != 0) {
                    switch (i) {
                    case 0:
                        RecordHandler_recordStoreWrite(rh, 0);
                        RecordHandler_recordStoreWritePreview(rh, 0);
                        break;
                    case 1:
                        G<uint8_t>(appData, 0xa0) = 1;
                        NFC_free_credits_likeGOF2OnFacebook();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x49] = 1;
                        break;
                    case 2:
                        G<uint8_t>(appData, 0xa1) = 1;
                        NFC_free_credits_likeFishlabsOnFacebook();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x4a] = 1;
                        break;
                    case 3:
                        G<uint8_t>(appData, 0xa2) = 1;
                        NFC_free_credits_subscribeToYoutubeChannel();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x4b] = 1;
                        break;
                    case 4:
                        G<uint8_t>(appData, 0xa3) = 1;
                        NFC_free_credits_followOnTwitter();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x4c] = 1;
                        break;
                    case 5:
                        G<uint8_t>(appData, 0xd) = 1;
                        NFC_free_credits_rateGame();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x4d] = 1;
                        break;
                    }
                }
            }
            return;
        }

        if (self->field_0xac != 0) {
            // Blueprint cargo-purchase confirmation.
            int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
            int cost = Item_getBlueprintAmount(G<void *>(self->field_0x84, 0x10)) * 200;
            bool revert = true;
            if (r == 0 && Status_getCredits() >= cost && self->field_0x11e == 0) {
                Status_changeCredits(globals);
                HangarWindow_setSellMode(self);
                self->field_0x68 = 0;
                HangarList_setCurrentItemIndex(self->field_0x14, -1);
                self->field_0x11e = 0;
                self->field_0xac = 0;
                revert = false;
            } else if (r == 1 && Status_getCredits() >= cost && self->field_0x11e == 0) {
                revert = false;
            }
            if (revert) {
                Item_setStationAmount(G<void *>(self->field_0x84, 0x10), self->field_0x8c);
                Item_setAmount(G<void *>(self->field_0x84, 0x10), self->field_0xa0);
                Item_setBlueprintAmount(G<void *>(self->field_0x84, 0x10), self->field_0xa4);
                Status_setCredits(globals);
                self->field_0x8c = 0;
                self->field_0x94 = 0;
                self->field_0x68 = 0;
                self->field_0xa0 = 0;
                self->field_0xa4 = 0;
                self->field_0xa8 = self->field_0x9c;
                HangarList_setCurrentItemIndex(self->field_0x14, -1);
                self->field_0x3c = 0;
                self->field_0xac = 0;
                self->field_0x88 = 0;
                if (Status_getCredits() < cost && self->field_0x11e == 0) {
                    String12 line, priceStr, fmt, msg;
                    AEString_ctor(&line, (const char *)GameText_getText(*g_hw_notEnoughTextId), false);
                    Layout_formatCredits(&priceStr, Status_getCredits());
                    AEString_ctor(&priceStr, hw_ote_fmt2, false);
                    AEString_ctor(&fmt, hw_ote_fmt4, false);
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    ChoiceWindow_set(self->field_0x20, 0);
                    AEString_dtor(&msg);
                    AEString_dtor(&fmt);
                    AEString_dtor(&priceStr);
                    AEString_dtor(&line);
                    self->field_0x3c = 1;
                }
                self->field_0x11e = 0;
            }
            HangarWindow_refreshCurrentContentHeight(self);
            return;
        }

        if (self->field_0x93 != 0) {
            // Ship sale confirmation.
            int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
            if (r == 1) {
                self->field_0x93 = 0;
                self->field_0x3c = 0;
            } else if (r == 0) {
                self->field_0x93 = 0;
                self->field_0x3c = 0;
                ListItem_getPrice(self->field_0x68);
                Status_changeCredits(globals);
                Station_removeShip(Status_getStation());
                HangarList_initShopTab(self->field_0x14, self->field_0x10,
                                       Station_getShips(Status_getStation()));
                HangarWindow_refreshCurrentContentHeight(self);
            }
            return;
        }

        // Ship purchase confirmation (the large branch). The mission-offer tail
        // is delegated to a helper because its decompile is unrecoverable.
        int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
        bool special = globals->field_0x114 == 3 && self->field_0x11d == 0;
        if (special && self->field_0x91 != 0 && r == 1) {
            int idx = globals->field_0x14c;
            Ship_getIndex(Status_getShip());
            if (Station_hasShip((void *)(uintptr_t)idx, Ship_getIndex(Status_getShip())) == 0) {
                int price = Ship_getPrice(G<void *>(self->field_0x68, 0xc));
                if (Status_getCredits() < price) {
                    String12 line, priceStr, fmt, msg, suffix, combined;
                    AEString_ctor(&line, (const char *)GameText_getText(*g_hw_sellShipTextId), false);
                    Ship_getPrice(G<void *>(self->field_0x68, 0xc));
                    Layout_formatCredits(&priceStr, Status_getCredits());
                    AEString_ctor(&priceStr, hw_ote_fmt2, false);
                    AEString_ctor(&fmt, hw_ote_fmt2, false);
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    AEString_dtor(&fmt);
                    AEString_dtor(&priceStr);
                    AEString_dtor(&line);
                    AEString_ctor(&suffix, hw_ote_fmt3, false);
                    GameText_getText(*g_hw_sellShipTextId);
                    AEString_add(&combined, &suffix, &suffix);
                    AEString_addAssign(&msg, &combined);
                    AEString_dtor(&combined);
                    AEString_dtor(&suffix);
                    ChoiceWindow_setMsg(self->field_0x20, &msg, true);
                    self->field_0x3c = 1;
                    self->field_0xaf = 1;
                    self->field_0x90 = 0;
                    AEString_dtor(&msg);
                    return;
                }
            }
        }
        // The rest of the ship-swap and mission-offer flow lives in the helper.
        HangarWindow_buildMissionOffer(self, touch, coord);
        return;
    } else {
        // self+0xae set: free-credits intro window.
        int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
        if (r != 0) {
            // Buy-credits grid.
            void *btns = self->field_0x24;
            for (unsigned int i = 0; i < 5; i++) {
                if (TouchButton_OnTouchEnd(G<void *>(G<void *>(btns, 4), i * 4 + 0x30), touch) != 0) {
                    switch (i) {
                    case 0: NFC_iap_buy_credits_300_000(); break;
                    case 1: NFC_iap_buy_credits_1_000_000(); break;
                    case 2: NFC_iap_buy_credits_3_000_000(); break;
                    case 3: NFC_iap_buy_credits_10_000_000(); break;
                    default: NFC_iap_buy_credits_100_000(); break;
                    }
                }
            }
            if (TouchButton_OnTouchEnd(G<void *>(G<void *>(btns, 4), 0x44), touch) != 0) {
                bool show = true;
                if (g_hw_optionFlags[0x4a] && g_hw_optionFlags[0x49] &&
                    g_hw_optionFlags[0x4d] && g_hw_optionFlags[0x4c])
                    show = (g_hw_optionFlags[0x4b] == 0);
                void *appData = AppManager_GetApplicationData();
                if (show || G<uint8_t>(appData, 0x15) == 0) {
                    for (int i = 0xc; i != 0x11; i++)
                        TouchButton_setVisible(G<void *>(G<void *>(btns, 4), i * 4), false);
                    HangarWindow_showFreeCreditsWindow(self);
                }
            }
            return;
        }
        self->field_0xae = 0;
        self->field_0x3c = 0;
        for (int i = 0xc; i != 0x11; i++)
            TouchButton_setVisible(G<void *>(G<void *>(self->field_0x24, 4), i * 4), false);
        TouchButton_setVisible(G<void *>(G<void *>(self->field_0x24, 4), 0x44), false);
        void *appData = AppManager_GetApplicationData();
        G<uint8_t>(appData, 0x40) = 0;
        void *mod = AppManager_GetApplicationModule(*g_hw_modStationId);
        if (mod != 0 && G<uint8_t>(mod, 0x18) != 0)
            G<uint8_t>(mod, 0x18) = 0;
        return;
    }

    // --- Shared buy/sell confirmation tail (reached from 0x11c branch). ---
    uint8_t buying = self->field_0x88;
    int r2 = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
    if (buying != 0) {
        if (r2 == 1) {
            self->field_0x88 = 0;
            self->field_0x3c = 0;
        } else if (r2 == 0) {
            if (self->field_0x130 == 0) {
                self->field_0x3c = 0;
                if (self->field_0xad == 0) {
                    self->field_0x88 = 1;
                } else {
                    Array<void *> *tabs = (Array<void *> *)self->field_0x4;
                    for (unsigned int i = 0; i < tabs->length; i++)
                        TouchButton_resetTouch(tabs->data[i]);
                }
                return;
            }
            self->field_0x3c = 0;
            self->field_0x130 = 0;
            self->field_0x88 = 0;
        } else {
            return;
        }
        self->field_0x68 = 0;
        HangarList_setCurrentItemIndex(self->field_0x14, -1);
        return;
    }
    if (r2 != 0)
        return;
    self->field_0x3c = 0;
    void *mod = AppManager_GetApplicationModule(*g_hw_dlcModuleId);
    if (G<uint8_t>(mod, 0x18) != 0)
        G<uint8_t>(mod, 0x18) = 0;
}

// ---- update_148a7e.cpp ----
extern "C" {
unsigned int HangarList_getCurrentTab(void *list);
int HangarList_getCurrentTabItems(void *list);
void TouchButton_setAlwaysPressed(void *btn, bool pressed);
int TouchButton_isTouched(void *btn);
int TouchButton_isVisible(void *btn);
float VectorSignedToFloat(int v, int mode);
void HangarWindow_transaction(HangarWindow *self, bool buy);
void ListItemWindow_update(void *win, int delta);
}

extern "C" void HangarWindow_update(HangarWindow *self, int delta)
{
    if (self->field_0xc == 0)
        return;
    self->field_0x8 = delta;

    if (self->field_0x58 == 1) {
        ListItemWindow_update(self->field_0x18, delta);
        return;
    }

    unsigned int tab = HangarList_getCurrentTab(self->field_0x14);
    Array<void *> *buttons = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < buttons->length; i++) {
        bool pressed = true;
        if (i != tab && !(tab == 3 && i == 0))
            pressed = (i == 2 && tab == 4);
        TouchButton_setAlwaysPressed(buttons->data[i], pressed);
    }

    if (self->field_0xd0 == 0) {
        float v = self->field_0xc4 * self->field_0xc8;
        self->field_0xc8 = v;
        float mag = v > 0.0f ? v : -v;
        if (mag >= 1.0f) {
            float pos = VectorSignedToFloat(self->field_0xb4, 0);
            self->field_0xb4 = (int)(v + pos);
        }
    }

    if (self->field_0xb4 > 0) {
        float f = VectorSignedToFloat(-self->field_0xb4, 0);
        self->field_0xc4 = 1.0f;
        self->field_0xc8 = f * 0.5f;
    }

    if (HangarList_getCurrentTabItems(self->field_0x14) != 0) {
        int diff = self->field_0xd8 - self->field_0xd4;
        if (diff < 0) {
            if (self->field_0xb4 < diff) {
                float f = VectorSignedToFloat(diff - self->field_0xb4, 0);
                self->field_0xc4 = 1.0f;
                self->field_0xc8 = f * 0.5f;
            }
        } else {
            self->field_0xc8 = 0;
            self->field_0xb4 = 0;
        }
    }

    if (self->field_0x88 != 0) {
        void *btnUp = G<void *>(G<void *>(self->field_0x24, 4), 0x20);
        void *btnDown = G<void *>(G<void *>(self->field_0x24, 4), 0x24);
        if (TouchButton_isTouched(btnUp) != 0 || TouchButton_isTouched(btnDown) != 0) {
            int t6c = self->field_0x6c + delta;
            int t70 = self->field_0x70 + delta;
            self->field_0x6c = t6c;
            self->field_0x70 = t70;
            int threshold = (t6c > 0x5dc) ? 0x1e : 200;
            if (t70 > threshold && (self->field_0x70 = 0,
                                    self->field_0x88 != 0 || self->field_0x89 != 0)) {
                if (TouchButton_isTouched(btnDown) != 0 && TouchButton_isVisible(btnDown) != 0) {
                    int n = (self->field_0x6c > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        HangarWindow_transaction(self, true);
                } else if (TouchButton_isTouched(btnUp) != 0 &&
                           TouchButton_isVisible(btnUp) != 0) {
                    int n = (self->field_0x6c > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        HangarWindow_transaction(self, false);
                }
            }
        }
    }
}

// ---- highlightItem_14bbec.cpp ----
// FModSound singleton (pc-rel -> holder; *holder is the FModSound object).
__attribute__((visibility("hidden"))) extern void **g_hw_sound;

extern "C" int HangarWindow_highlightItem(HangarWindow *self, void *item)
{
    if (item != 0 && ListItem_isSelectable(item) != 0) {
        FModSound_play(*g_hw_sound, 0x7c, 0, 0, 0);
        unsigned flag = 0;
        if (self->field_0x68 != item) {
            flag = ListItem_isTextButton(item) ^ 1;
        }
        self->field_0x68 = item;
        self->field_0xd2 = flag;
        if (ListItem_isShip(item) != 0) {
            Ship_adjustPrice(G<void *>(self->field_0x68, 0xc));
        }
    }
    return 0;
}

// ---- demountItem_14de04.cpp ----
extern "C" {
int Item_getType(void *item);
int Item_getAmount(void *item);
void *Item_makeItem(void *item);
int Item_getIndex(void *item);
void Item_changeAmount(void *item, int delta);
void *Item_extractItems(void *arr, bool flag);
void *Item_mixItems(void *a, void *b);
void *Status_getShip();
void *Status_getStation();
void Ship_addCargo(void *ship, void *item);
void Ship_freeSlot(void *ship, void *item);
void Ship_freeSlotAt(void *ship, void *item, int slot);
void Ship_setCargo(void *ship, void *arr);
void *Ship_getCargo(void *ship);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
void ArrayAdd_ItemPtr(void *item, void *arr);
void ArrayReleaseClasses_ItemPtr(void *arr);
void *Array_ItemPtr_dtor(void *arr);
void operator_delete(void *p);
void HangarList_initShipTab(void *list, void *ship);
void HangarList_initShopTab(void *list, void *items, void *ships);
void HangarList_setCurrentTab(void *list, bool flag);
void FModSound_play(void *snd, int id, void *a, void *b, float pitch);
float HangarWindow_refreshCurrentContentHeight(HangarWindow *self);
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern void **g_hw_sound;

extern "C" void HangarWindow_demountItem(HangarWindow *self, void *item, int slot)
{
    int type = Item_getType(item);
    if (type == 1)
        Item_getAmount(item);

    void *made = Item_makeItem(item);
    Ship_addCargo(Status_getShip(), made);

    void *ship = Status_getShip();
    if (slot < 0)
        Ship_freeSlot(ship, item);
    else
        Ship_freeSlotAt(ship, item, slot);

    bool merged = false;
    Array<void *> *cargo = F<Array<void *> *>(self, 0x10);
    for (unsigned int i = 0; i < cargo->length; i++) {
        void *cur = cargo->data[i];
        if (Item_getIndex(cur) == Item_getIndex(made)) {
            Item_changeAmount(cur, Item_getAmount(item));
            merged = true;
            break;
        }
    }
    if (!merged)
        ArrayAdd_ItemPtr(made, self->field_0x10);

    Ship_setCargo(Status_getShip(), Item_extractItems(self->field_0x10, true));

    if (self->field_0x10 != 0) {
        ArrayReleaseClasses_ItemPtr(self->field_0x10);
        if (self->field_0x10 != 0)
            operator_delete(Array_ItemPtr_dtor(self->field_0x10));
    }
    self->field_0x10 = 0;

    HangarWindow_statusShip();
    void *mixed = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
    self->field_0x10 = mixed;
    HangarList_initShipTab(self->field_0x14, HangarWindow_statusShip());

    HangarWindow_statusShip();
    void *items = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
    HangarList_initShopTab(self->field_0x14, items, Station_getShips(Status_getStation()));
    HangarList_setCurrentTab(self->field_0x14, false);

    float h = HangarWindow_refreshCurrentContentHeight(self);
    self->field_0xb4 = self->field_0xe4;
    FModSound_play(*g_hw_sound, 0x60, 0, 0, h);
}

// ---- OnTouchBegin_14b740.cpp ----
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
int Layout_OnTouchBegin(void *layout, int touch);
int TouchButton_OnTouchBegin(void *btn, int touch);
void ChoiceWindow_OnTouchBegin(void *win, int touch);
void ChoiceWindow_setMsg(void *win, String12 *msg, bool flag);
void ListItemWindow_OnTouchBegin(void *win, int touch, int coord);
int __aeabi_idiv(int a, int b);
void *HangarList_getCurrentItem(void *list);
int HangarList_getCurrentLength(void *list);
int HangarList_getCurrentTab(void *list);
unsigned int HangarList_getCurrentItemIndex(void *list);
void HangarList_setCurrentItemIndex(void *list, int idx);
int ListItem_isShip(void *item);
int Item_getIndex(void *item);
int Item_getBlueprintAmount(void *item);
void *Item_extractItems(void *arr, bool flag);
void Ship_setCargo(void *ship, void *arr);
void *Status_getShip();
void *Status_getStation();
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b, String12 *c);
void Station_setItems(void *station, void *arr, bool flag);
int Station_getIndex(void *station);
int BluePrint_isEmpty(void *bp);
int BluePrint_getStationIndex(void *bp);
void *BluePrint_getStationName(void *bp);
void *GameText_getText(int id);
void AEString_ctor_empty(String12 *self);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void Layout_formatCredits(String12 *out, int amount);
void HangarWindow_highlightItem(HangarWindow *self, void *item);
void HangarWindow_autoEquipSecondaryWeapons(HangarWindow *self, int row);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;
__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern int *g_hw_bpTextId;

extern "C" void HangarWindow_OnTouchBegin(HangarWindow *self, int touch, int coord)
{
    self->field_0x6c = 0;
    self->field_0x70 = 0;
    unsigned char handled = (unsigned char)Layout_OnTouchBegin(*g_hw_layout, touch);

    if (self->field_0x3c != 0) {
        if (self->field_0xae != 0) {
            for (int i = 0xc; i != 0x11; i++)
                TouchButton_OnTouchBegin(G<void *>(G<void *>(self->field_0x24, 4), i * 4), touch);
            TouchButton_OnTouchBegin(G<void *>(G<void *>(self->field_0x24, 4), 0x44), touch);
        } else if (self->field_0xb0 != 0) {
            for (int i = 0x12; i != 0x17; i++)
                TouchButton_OnTouchBegin(G<void *>(G<void *>(self->field_0x24, 4), i * 4), touch);
        }
        ChoiceWindow_OnTouchBegin(self->field_0x20, touch);
        return;
    }

    self->field_0xcc = coord;
    self->field_0xb8 = coord;
    self->field_0xc0 = 0;
    self->field_0xd0 = 1;

    if (self->field_0x58 == 1) {
        ListItemWindow_OnTouchBegin(self->field_0x18, touch, coord);
        return;
    }

    void *layout = *g_hw_layout;
    unsigned char skip = 1;
    if (layout->field_0xc < coord && coord < *g_hw_screenWidth - layout->field_0x10) {
        int row = __aeabi_idiv(
            ((coord - layout->field_0xc) - layout->field_0x20) - self->field_0x10c -
                self->field_0xb4,
            layout->field_0x70 + self->field_0x10c);
        if (HangarList_getCurrentLength(self->field_0x14) > row) {
            HangarList_setCurrentItemIndex(self->field_0x14, row);
            HangarWindow_highlightItem(self, HangarList_getCurrentItem(self->field_0x14));
            skip = 0;
            if (self->field_0x11d != 0) {
                void *ci = HangarList_getCurrentItem(self->field_0x14);
                if (ListItem_isShip(ci) != 0 &&
                    HangarList_getCurrentTab(self->field_0x14) == 1) {
                    Ship_setCargo(Status_getShip(), Item_extractItems(self->field_0x10, true));
                    Station_setItems(Status_getStation(),
                                     Item_extractItems(self->field_0x10, false), false);
                }
            }
        }
    }

    if (HangarList_getCurrentTab(self->field_0x14) == 4 && self->field_0x88 != 0 &&
        !(!skip && self->field_0x68 == self->field_0x84) && self->field_0x94 > 0 &&
        self->field_0x3c == 0 && BluePrint_isEmpty(self->field_0x80) == 0) {
        if (BluePrint_getStationIndex(self->field_0x80) != Station_getIndex(Status_getStation())) {
            void *globals = *g_hw_globals;
            int bpIdx = Item_getIndex(G<void *>(self->field_0x84, 0x10));
            bool localBp = (bpIdx == 0xd1) ||
                           (Item_getIndex(G<void *>(self->field_0x84, 0x10)) == 0xcc);
            self->field_0x11e = localBp;

            String12 msg;
            AEString_ctor_empty(&msg);
            String12 line;
            AEString_ctor(&line, (const char *)GameText_getText(*g_hw_bpTextId), false);
            AEString_assign(&msg, &line);
            AEString_dtor(&line);

            if (self->field_0x11e == 0) {
                String12 copy, sname, fmt, result;
                AEString_ctor_str(&copy, &msg, false);
                BluePrint_getStationName(self->field_0x80);
                AEString_ctor(&sname, hw_otb_fmt1, false);
                AEString_ctor(&fmt, hw_otb_fmt1, false);
                Status_replaceHash(&result, globals, &copy, &sname, &fmt);
                AEString_assign(&msg, &result);
                AEString_dtor(&result);
                AEString_dtor(&fmt);
                AEString_dtor(&sname);
                AEString_dtor(&copy);

                String12 copy2, priceStr, fmt2, result2;
                AEString_ctor_str(&copy2, &msg, false);
                Layout_formatCredits(&priceStr,
                                     Item_getBlueprintAmount(G<void *>(self->field_0x84, 0x10)));
                AEString_ctor(&fmt2, hw_otb_fmt2, false);
                Status_replaceHash(&result2, globals, &copy2, &priceStr, &fmt2);
                AEString_assign(&msg, &result2);
                AEString_dtor(&result2);
                AEString_dtor(&fmt2);
                AEString_dtor(&priceStr);
                AEString_dtor(&copy2);
            }
            bool flag = (self->field_0x11e == 0);
            ChoiceWindow_setMsg(self->field_0x20, &msg, flag);
            self->field_0x3c = 1;
            self->field_0xac = 1;
            self->field_0xd1 = 1;
            AEString_dtor(&msg);
            return;
        }
    }

    Array<void *> *tabs = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < tabs->length; i++)
        handled |= (unsigned char)TouchButton_OnTouchBegin(tabs->data[i], touch);

    Array<void *> *buttons = F<Array<void *> *>(self, 0x24);
    for (unsigned int i = 0; i < buttons->length; i++) {
        void *btn = buttons->data[i];
        if (btn != 0)
            TouchButton_OnTouchBegin(btn, touch);
    }

    if (self->field_0xf8 != 0 && HangarList_getCurrentTab(self->field_0x14) == 1) {
        int idx = self->field_0xfc;
        if (idx >= 0 &&
            !(!handled && (unsigned int)idx == HangarList_getCurrentItemIndex(self->field_0x14))) {
            self->field_0xf8 = 0;
            HangarWindow_autoEquipSecondaryWeapons(self, idx);
        }
    }
}

// ---- showCreditsBuyWindow_14e118.cpp ----
struct String12 { uint32_t a, b, c; };

extern "C" {
void *ApplicationManager_GetApplicationData();
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
void *GameText_getText(int id);
void ChoiceWindow_set(void *win, String12 *title, String12 *t2, bool flag,
                      String12 *yes, String12 *no, void *body, int a, int b);
void ChoiceWindow_setWidth(void *win, int w);
void ChoiceWindow_setHeight(void *win, int h);
float VectorSignedToFloat(int v, int mode);
}

__attribute__((visibility("hidden"))) extern int *g_hw_buyTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_buyTextId2;
__attribute__((visibility("hidden"))) extern char *g_hw_buyFlag;
__attribute__((visibility("hidden"))) extern int *g_hw_buyWidth;
__attribute__((visibility("hidden"))) extern int *g_hw_buyHeight;
extern "C" {
extern const char hw_buy_yes[], hw_buy_no[], hw_buy_icon[];
extern const char hw_buy2_yes[], hw_buy2_no[], hw_buy2_icon[];
extern float hw_buy_heightScale;
}

extern "C" void HangarWindow_showCreditsBuyWindow(HangarWindow *self)
{
    void *appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x4c) = 0;
    appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x3d) = 1;

    void *win = self->field_0x20;
    String12 a, b, yes, no;

    if (self->field_0x11f == 0) {
        AEString_ctor(&a, hw_buy_yes, false);
        AEString_ctor(&b, hw_buy_no, false);
        AEString_ctor(&yes, hw_buy_icon, false);
        AEString_ctor(&no, hw_buy2_yes, false);
        void *body = GameText_getText(*g_hw_buyTextId);
        ChoiceWindow_set(win, &a, &b, false, &yes, &no, body, -1, -1);
        AEString_dtor(&no);
        AEString_dtor(&yes);
        AEString_dtor(&b);
        AEString_dtor(&a);
    } else {
        AEString_ctor(&a, hw_buy2_no, false);
        AEString_ctor(&b, hw_buy2_no, false);
        AEString_ctor(&yes, hw_buy2_icon, false);
        AEString_ctor(&no, hw_buy_yes, false);
        void *body = GameText_getText(*g_hw_buyTextId2);
        ChoiceWindow_set(win, &a, &b, false, &yes, &no, body, -1, -1);
        AEString_dtor(&no);
        AEString_dtor(&yes);
        AEString_dtor(&b);
        AEString_dtor(&a);

        int h;
        void *win2;
        if (*g_hw_buyFlag == 0) {
            ChoiceWindow_setWidth(self->field_0x20, *g_hw_buyWidth);
            h = *g_hw_buyHeight;
            win2 = self->field_0x20;
        } else {
            ChoiceWindow_setWidth(self->field_0x20, self->field_0x120 * 3);
            float v = VectorSignedToFloat(self->field_0x124, 0);
            win2 = self->field_0x20;
            h = (int)(v * hw_buy_heightScale);
        }
        ChoiceWindow_setHeight(win2, h);
    }

    self->field_0x3c = 1;
    self->field_0xae = 1;
    self->field_0xaf = 0;
}

// ---- getCurrentTab_148a44.cpp ----
extern "C" int HangarWindow_getCurrentTab(HangarWindow *self)
{
    return HangarList_getCurrentTab(self->field_0x14);
}

// ---- refreshCargoAvailabilityForBlueprints_14900c.cpp ----
// Status singleton (pc-rel -> holder; *holder is the Status object).
__attribute__((visibility("hidden"))) extern void **g_hw_status2;

extern "C" void HangarWindow_refreshCargoAvailabilityForBlueprints(HangarWindow *self)
{
    void *items = HangarList_getItems(self->field_0x14);
    Array<void *> *arr = *(Array<void *> **)((char *)G<void *>(items, 0x4) + 0x8);
    if (arr == 0) return;
    for (uint32_t i = 0; i < arr->length; i++) {
        void *it = arr->data[i];
        F<uint8_t>((HangarWindow *)it, 0x45) = 0;
        if (it != 0 && ListItem_isBluePrint(it) != 0) {
            void *bp = G<void *>(it, 0x8);
            void *cargo = Ship_getCargo(Status_getShip(*g_hw_status2));
            Array<int> *ingr = (Array<int> *)BluePrint_getIngredientList(bp);
            if (cargo != 0) {
                void *base = G<void *>(bp, 0x0);
                for (uint32_t j = 0; j < ingr->length; j++) {
                    int *amts = *(int **)((char *)base + 0x4);
                    if (amts[j] > 0) {
                        Array<void *> *carr = (Array<void *> *)cargo;
                        for (uint32_t k = 0; k < carr->length; k++) {
                            if (Item_getIndex(carr->data[k]) == ingr->data[j]) {
                                F<uint8_t>((HangarWindow *)it, 0x45) = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

// ---- setSellMode_14bff0.cpp ----
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
int HangarList_getCurrentTab(void *list);
unsigned int HangarList_getCurrentItemIndex(void *list);
void *HangarList_getCurrentItem(void *list);
void *HangarList_getCurrentTabItems(void *list);
void HangarList_initShipTab(void *list, void *ship);
void HangarList_initShopTab(void *list, void *items, void *ships);
void HangarList_initBlueprintTab(void *list, void *bps);
void HangarList_setCurrentTab(void *list, bool flag);
int ListItem_isShip(void *item);
int ListItem_isSlot(void *item);
int ListItem_isItem(void *item);
int ListItem_isTextButton(void *item);
int ListItem_isSelectable(void *item);
int ListItem_isBluePrint(void *item);
int Item_getType(void *item);
int Item_getAmount(void *item);
int Item_getIndex(void *item);
int Item_getStationAmount(void *item);
int Item_getBlueprintAmount(void *item);
void *Item_makeItem(void *item);
void *Item_extractItems(void *arr, bool flag);
void *Item_mixItems(void *a, void *b);
int Status_getCredits();
void *Status_getShip();
void *Status_getStation();
void *Status_getSystem();
void *Status_getBluePrints(void *globals);
void Status_addPendingProduct(void *globals, void *bp);
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b, String12 *c);
void Ship_setCargo(void *ship, void *arr);
void *Ship_getCargo(void *ship);
void Ship_addCargo(void *ship, void *item);
void Ship_adjustPrice(void *ship);
int Ship_hasEquipment(void *ship, int index);
void Station_setItems(void *station, void *arr, bool flag);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
int Station_getIndex(void *station);
int SolarSystem_getRoutes(void *system);
int BluePrint_isCompleted(void *bp);
int BluePrint_isEmpty(void *bp);
int BluePrint_getIndex(void *bp);
int BluePrint_getStationIndex(void *bp);
int BluePrint_getQuantity(void *bp);
void *BluePrint_getStationName(void *bp);
void BluePrint_addItem(void *bp, void *item, int amount, int station);
void BluePrint_reset(void *bp);
void *GameText_getText(int id);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void ChoiceWindow_set(void *win, void *text);
void ChoiceWindow_setMsg(void *win, void *msg, bool flag);
void ArrayReleaseClasses_ItemPtr(void *arr);
void *Array_ItemPtr_dtor(void *arr);
void ArrayAdd_ItemPtr(void *item, void *arr);
void operator_delete(void *p);
float HangarWindow_refreshCurrentContentHeight(HangarWindow *self);
void HangarWindow_refreshCargoAvailabilityForBlueprints(HangarWindow *self);
void HangarWindow_autoEquipSecondaryWeapons(HangarWindow *self, int row);
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern void **g_hw_itemFlags;
__attribute__((visibility("hidden"))) extern int *g_hw_sellTextId1;
__attribute__((visibility("hidden"))) extern int *g_hw_sellTextId2;
__attribute__((visibility("hidden"))) extern int *g_hw_routesTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_bpStations;

extern "C" void HangarWindow_setSellMode(HangarWindow *self)
{
    void *item = self->field_0x68;

    if (item == 0 ||
        ListItem_isShip(item) != 0 || ListItem_isSlot(item) != 0 ||
        ListItem_isTextButton(item) != 0 || ListItem_isSelectable(item) == 0 ||
        ListItem_isBluePrint(item) != 0) {
        self->field_0x88 = 0;
        return;
    }

    self->field_0x88 = 1;

    int tab = HangarList_getCurrentTab(self->field_0x14);
    if (tab == 1) {
        if (self->field_0x88 == 0) {
            if (ListItem_isItem(item) != 0 && Item_getType(item->field_0x10) != 4) {
                void *flags = *g_hw_itemFlags;
                if (G<uint8_t>(flags, 0x1e) == 0) {
                    GameText_getText(*g_hw_sellTextId1);
                    ChoiceWindow_set(self->field_0x20, 0);
                    G<uint8_t>(flags, 0x1e) = 1;
                    self->field_0x3c = 1;
                }
            }
            self->field_0xf8 = 1;
            self->field_0xfc = HangarList_getCurrentItemIndex(self->field_0x14);
        } else {
            void *flags = *g_hw_itemFlags;
            if (G<uint8_t>(flags, 0x1d) == 0) {
                GameText_getText(*g_hw_sellTextId2);
                ChoiceWindow_set(self->field_0x20, 0);
                G<uint8_t>(flags, 0x1d) = 1;
                self->field_0x3c = 1;
            }
            self->field_0x8c = Item_getStationAmount(item->field_0x10);
            self->field_0xa0 = Item_getAmount(item->field_0x10);
            self->field_0x98 = Status_getCredits();
            self->field_0x9c = self->field_0xa8;
        }

        Ship_setCargo(Status_getShip(), Item_extractItems(self->field_0x10, true));
        Station_setItems(Status_getStation(), Item_extractItems(self->field_0x10, false), false);
        if (self->field_0x10 != 0) {
            ArrayReleaseClasses_ItemPtr(self->field_0x10);
            if (self->field_0x10 != 0)
                operator_delete(Array_ItemPtr_dtor(self->field_0x10));
        }
        self->field_0x10 = 0;

        void *mixed = Item_mixItems(Ship_getCargo(Status_getShip()), Station_getItems(Status_getStation()));
        self->field_0x10 = mixed;
        HangarList_initShopTab(self->field_0x14, mixed, Station_getShips(Status_getStation()));
        HangarList_initShipTab(self->field_0x14, Status_getShip());

        void *ci = HangarList_getCurrentItem(self->field_0x14);
        self->field_0x68 = ci;
        if (ci != 0 && ListItem_isShip(ci) != 0)
            Ship_adjustPrice(G<void *>(self->field_0x68, 0xc));
        HangarWindow_refreshCurrentContentHeight(self);
        return;
    }

    if (HangarList_getCurrentTab(self->field_0x14) != 4)
        return;

    if (self->field_0x88 == 0) {
        if (self->field_0x84 != 0 && self->field_0x80 != 0) {
            void *bpItem = G<void *>(self->field_0x84, 0x10);
            BluePrint_addItem(self->field_0x80, bpItem, Item_getBlueprintAmount(bpItem),
                              Station_getIndex(Status_getStation()));
        }

        uint8_t completedFlag = 0;
        if (BluePrint_isCompleted(self->field_0x80) != 0) {
            void *globals = *g_hw_globals;
            if (BluePrint_getStationIndex(self->field_0x80) == Station_getIndex(Status_getStation())) {
                String12 line, copy, name, fmt, result;
                AEString_ctor(&line, (const char *)GameText_getText(*g_hw_sellTextId1), false);
                AEString_ctor_str(&copy, &line, false);
                AEString_ctor(&name, (const char *)GameText_getText(*g_hw_sellTextId1), false);
                AEString_ctor(&fmt, hw_bp_fmt1, false);
                Status_replaceHash(&result, globals, &copy, &name, &fmt);
                AEString_assign(&line, &result);
                AEString_dtor(&result);
                AEString_dtor(&fmt);
                AEString_dtor(&name);
                AEString_dtor(&copy);
                ChoiceWindow_set(self->field_0x20, 0);

                int *stations = g_hw_bpStations;
                int idx = stations[BluePrint_getIndex(self->field_0x80)];
                BluePrint_getQuantity(self->field_0x80);
                void *made = Item_makeItem((void *)(uintptr_t)idx);
                Ship_addCargo(Status_getShip(), made);
                ArrayAdd_ItemPtr(made, self->field_0x10);
                HangarList_setCurrentTab(self->field_0x14, true);
                HangarWindow_refreshCurrentContentHeight(self);
                AEString_dtor(&line);
            } else {
                String12 line, copy, name, fmt, result, line2, sname, fmt2;
                AEString_ctor(&line, (const char *)GameText_getText(*g_hw_sellTextId2), false);
                AEString_ctor_str(&copy, &line, false);
                AEString_ctor(&name, (const char *)GameText_getText(*g_hw_sellTextId2), false);
                AEString_ctor(&fmt, hw_bp_fmt2, false);
                Status_replaceHash(&result, globals, &copy, &name, &fmt);
                AEString_assign(&line, &result);
                AEString_dtor(&result);
                AEString_dtor(&fmt);
                AEString_dtor(&name);
                AEString_dtor(&copy);

                AEString_ctor_str(&line2, &line, false);
                BluePrint_getStationName(self->field_0x80);
                AEString_ctor(&sname, hw_bp_fmt3, false);
                AEString_ctor(&fmt2, hw_bp_fmt3, false);
                String12 result2;
                Status_replaceHash(&result2, globals, &line2, &sname, &fmt2);
                AEString_assign(&line, &result2);
                AEString_dtor(&result2);
                AEString_dtor(&fmt2);
                AEString_dtor(&sname);
                AEString_dtor(&line2);

                ChoiceWindow_set(self->field_0x20, 0);
                Status_addPendingProduct(globals, self->field_0x80);
                HangarList_setCurrentTab(self->field_0x14, true);
                HangarWindow_refreshCargoAvailabilityForBlueprints(self);
                AEString_dtor(&line);
            }
            BluePrint_reset(self->field_0x80);
            completedFlag = 1;
        }

        void *globals = *g_hw_globals;
        Ship_setCargo(HangarWindow_statusShip(), Item_extractItems(Ship_getCargo(0), true));
        HangarWindow_statusShip();
        void *items = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
        HangarList_initShopTab(self->field_0x14, items, Station_getShips(Status_getStation()));
        HangarList_initBlueprintTab(self->field_0x14, Status_getBluePrints(globals));
        void *mix = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
        self->field_0x10 = mix;
        self->field_0x3c = completedFlag;

        if (completedFlag) {
            Array<void *> *items2 = (Array<void *> *)HangarList_getCurrentTabItems(self->field_0x14);
            for (unsigned int i = 0; i < items2->length; i++) {
                void *li = items2->data[i];
                if (li != 0 && ListItem_isItem(li) != 0 &&
                    Item_getIndex(G<void *>(li, 0x10)) == BluePrint_getIndex(self->field_0x80)) {
                    if (Ship_hasEquipment(Status_getShip(), Item_getIndex(G<void *>(li, 0x10))) != 0) {
                        self->field_0xfc = i;
                        self->field_0xf8 = 1;
                        HangarWindow_autoEquipSecondaryWeapons(self, i);
                        self->field_0xf8 = 0;
                        break;
                    }
                }
            }
        }
        return;
    }

    self->field_0x94 = 0;
    if (BluePrint_isEmpty(self->field_0x80) != 0 && Item_getAmount(item->field_0x10) > 0) {
        int idx = BluePrint_getIndex(self->field_0x80);
        bool flag;
        void *text;
        if (idx == 0xd2 || BluePrint_getIndex(self->field_0x80) == 0xdf) {
            if (SolarSystem_getRoutes(Status_getSystem()) != 0) {
                text = GameText_getText(*g_hw_sellTextId2);
                flag = true;
            } else {
                self->field_0x130 = 1;
                text = GameText_getText(*g_hw_routesTextId);
                flag = false;
            }
        } else {
            text = GameText_getText(*g_hw_sellTextId2);
            flag = true;
        }
        ChoiceWindow_setMsg(self->field_0x20, text, flag);
        self->field_0x3c = 1;
    }

    self->field_0xa4 = Item_getBlueprintAmount(item->field_0x10);
    self->field_0xa0 = Item_getAmount(item->field_0x10);
    self->field_0x98 = Status_getCredits();
    self->field_0x9c = self->field_0xa8;
    self->field_0x84 = self->field_0x68;
}

// ---- selectItem_14e570.cpp ----
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
int ListItem_isShip(void *item);
int ListItem_isSelectable(void *item);
int ListItem_isPendingProduct(void *item);
int ListItem_isSlot(void *item);
int ListItem_isItem(void *item);
int ListItem_getIndex(void *item);
int ListItem_getPrice(void *item);
void Ship_adjustPrice(void *ship);
int HangarList_getCurrentTab(void *list);
void *HangarList_getCurrentItem(void *list);
unsigned int HangarList_getCurrentItemIndex(void *list);
void HangarList_fillIngredientsList(void *list, bool flag);
void HangarList_setCurrentTab(void *list, bool flag);
void HangarList_initShopTab(void *list, void *items, void *ships);
void HangarList_initShipTab(void *list, void *ship);
int Item_isUnsaleable(void *item);
int Item_getSort(void *item);
int Item_getType(void *item);
int Item_getIndex(void *item);
int Item_getAmount(void *item);
int Item_getStationAmount(void *item);
int Item_getAttribute(void *item);
int Item_canBeInstalledMultipleTimes(void *item);
void *Item_extractItems(void *arr, bool flag);
void *Item_mixItems(void *a, void *b);
void *Status_getShip();
void *Status_getStation();
int Status_getCredits();
int Status_getCurrentCampaignMission();
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b, String12 *c);
int Ship_getMaxPassengers(void *ship);
int Ship_getPrice(void *ship);
int Ship_getIndex(void *ship);
void *Ship_getFirstEquipmentOfSort(void *ship, int sort);
void Ship_setCargo(void *ship, void *arr);
void *Ship_getCargo(void *ship);
void Station_setItems(void *station, void *arr, bool flag);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
void *GameText_getText(int id);
void ChoiceWindow_set(void *win, void *text);
void ChoiceWindow_setMsg(void *win, String12 *msg, bool flag);
void Layout_formatCredits(String12 *out, int price, int credits);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);
void ArrayReleaseClasses_ItemPtr(void *arr);
void *Array_ItemPtr_dtor(void *arr);
void operator_delete(void *p);
void HangarWindow_refreshCurrentContentHeight(HangarWindow *self);
void HangarWindow_demountItem(HangarWindow *self, void *item, int slot);
void HangarWindow_mountItem(HangarWindow *self, void *item);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern void **g_hw_itemFlags;
__attribute__((visibility("hidden"))) extern int *g_hw_unsaleableTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_slotMsgTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_buyBaseTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_notEnoughTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_sellMsgTextId1;
__attribute__((visibility("hidden"))) extern int *g_hw_sellMsgTextId2;

// Show a simple text-only choice window with the given game-text id, then flag dirty.
static void showText(HangarWindow *self, int textId)
{
    GameText_getText(textId);
    ChoiceWindow_set(self->field_0x20, 0);
    self->field_0x3c = 1;
}

extern "C" void HangarWindow_selectItem(HangarWindow *self, void *item)
{
    self->field_0x68 = item;
    if (item != 0 && ListItem_isShip(item) != 0)
        Ship_adjustPrice(G<void *>(self->field_0x68, 0xc));

    int tab = HangarList_getCurrentTab(self->field_0x14);

    if (tab == 2) {
        if (ListItem_isSelectable(item) != 0 && ListItem_isPendingProduct(item) == 0) {
            void *bp = item->field_0x8;
            self->field_0x80 = bp;
            HangarList_fillIngredientsList(self->field_0x14, bp != 0);
            HangarList_setCurrentTab(self->field_0x14, true);
            HangarWindow_refreshCurrentContentHeight(self);
            if (self->field_0x89 != 0)
                self->field_0x89 = 0;
        }
        return;
    }

    if (tab == 1) {
        if (ListItem_isSelectable(item) == 0)
            return;
        if (ListItem_isShip(item) == 0) {
            // Buying / selling a regular item.
            if (Item_isUnsaleable(item->field_0x10) != 0)
                return;

            uint8_t was = self->field_0x88;
            self->field_0x88 = (uint8_t)(was ^ 1);
            if (was == 0) {
                void *flags = *g_hw_itemFlags;
                if (G<uint8_t>(flags, 0x1d) == 0) {
                    GameText_getText(*g_hw_sellMsgTextId1);
                    ChoiceWindow_set(self->field_0x20, 0);
                    G<uint8_t>(flags, 0x1d) = 1;
                    self->field_0x3c = 1;
                }
                self->field_0x8c = Item_getStationAmount(item->field_0x10);
                self->field_0xa0 = Item_getAmount(item->field_0x10);
                self->field_0x98 = Status_getCredits();
                self->field_0x9c = self->field_0xa8;
            } else {
                if (ListItem_isItem(item) != 0 && Item_getType(item->field_0x10) != 4) {
                    void *flags = *g_hw_itemFlags;
                    if (G<uint8_t>(flags, 0x1e) == 0) {
                        GameText_getText(*g_hw_sellMsgTextId2);
                        ChoiceWindow_set(self->field_0x20, 0);
                        G<uint8_t>(flags, 0x1e) = 1;
                        self->field_0x3c = 1;
                    }
                }
                int li = ListItem_getIndex(item);
                if (li > 0x83 && ListItem_getIndex(item) < 0x9a) {
                    int base = G<int>(*g_hw_globals, 0xac);
                    *((uint8_t *)G<int>((void *)(uintptr_t)base, 4) + ListItem_getIndex(item) - 0x84) = 1;
                }
                self->field_0xf8 = 1;
                self->field_0xfc = HangarList_getCurrentItemIndex(self->field_0x14);

                Ship_setCargo(Status_getShip(), Item_extractItems(self->field_0x10, true));
                Station_setItems(Status_getStation(),
                                 Item_extractItems(self->field_0x10, false), false);
                if (self->field_0x10 != 0) {
                    ArrayReleaseClasses_ItemPtr(self->field_0x10);
                    if (self->field_0x10 != 0)
                        operator_delete(Array_ItemPtr_dtor(self->field_0x10));
                }
                self->field_0x10 = 0;

                void *mixed = Item_mixItems(Ship_getCargo(Status_getShip()),
                                            Station_getItems(Status_getStation()));
                self->field_0x10 = mixed;
                HangarList_initShopTab(self->field_0x14, mixed,
                                       Station_getShips(Status_getStation()));
                HangarList_initShipTab(self->field_0x14, Status_getShip());
            }
            return;
        }

        // Buying a ship.
        int price = Ship_getPrice(item->field_0xc);
        void *globals = *g_hw_globals;
        int credits = Status_getCredits();
        int oldPrice = Ship_getPrice(Status_getShip());
        if (oldPrice + credits < price && self->field_0x11d == 0) {
            String12 line, priceStr, fmt, msg, suffix, combined;
            AEString_ctor(&line, (const char *)GameText_getText(*g_hw_notEnoughTextId), false);
            Layout_formatCredits(&priceStr,
                                 ListItem_getPrice(item) - Status_getCredits() - Ship_getPrice(Status_getShip()),
                                 0);
            AEString_ctor_str(&line, &priceStr, false);
            AEString_ctor(&fmt, hw_sel_fmt1, false);
            Status_replaceHash(&msg, globals, &line, &priceStr, &fmt);
            AEString_dtor(&fmt);
            AEString_dtor(&priceStr);
            AEString_ctor(&suffix, hw_sel_fmt2, false);
            GameText_getText(*g_hw_notEnoughTextId);
            AEString_add(&combined, &suffix, &suffix);
            AEString_addAssign(&msg, &combined);
            AEString_dtor(&combined);
            AEString_dtor(&suffix);
            ChoiceWindow_setMsg(self->field_0x20, &msg, true);
            self->field_0x3c = 1;
            self->field_0xaf = 1;
            AEString_dtor(&msg);
            return;
        }

        if (globals->field_0x34 < 1) {
            if (Status_getCurrentCampaignMission() == 0x4d &&
                Ship_getIndex(Status_getShip()) == 0x25) {
                showText(self, *g_hw_unsaleableTextId);
                return;
            }
            int a = Ship_getIndex(Status_getShip());
            int b = Ship_getIndex(item->field_0xc);
            if (a != b) {
                self->field_0x90 = 1;
                GameText_getText(*g_hw_sellMsgTextId2);
                ChoiceWindow_setMsg(self->field_0x20,
                                    (String12 *)GameText_getText(*g_hw_sellMsgTextId2), true);
                self->field_0x3c = 1;
                return;
            }
            showText(self, *g_hw_buyBaseTextId);
        } else {
            showText(self, *g_hw_buyBaseTextId);
        }
        return;
    }

    if (tab != 0) {
        return;
    }

    // Tab 0: cargo / equipment management.
    if (ListItem_isSelectable(item) == 0)
        return;

    self->field_0xbc = 0;
    self->field_0xe4 = self->field_0xb4;
    self->field_0xb4 = 0;

    void *flags0 = *g_hw_itemFlags;
    if (G<uint8_t>(flags0, 0x1f) == 0 && ListItem_isSlot(item) != 0) {
        GameText_getText(*g_hw_slotMsgTextId);
        ChoiceWindow_set(self->field_0x20, 0);
        G<uint8_t>(flags0, 0x1f) = 1;
        self->field_0x3c = 1;
    }

    if (ListItem_isSelectable(item) == 0)
        return;

    void *cur = HangarList_getCurrentItem(self->field_0x14);
    void *curItem = G<void *>(cur, 0x10);
    if (curItem != 0) {
        if (Item_isUnsaleable(curItem) != 0) {
            showText(self, *g_hw_unsaleableTextId);
            return;
        }
        if (Item_getSort(curItem) == 0x14) {
            void *globals = *g_hw_globals;
            int passengerCount = globals->field_0x34;
            int chosen = passengerCount;
            if (passengerCount > 0)
                chosen = item->field_0x3c;
            int adj = passengerCount - 1;
            if (passengerCount >= 1)
                adj = chosen;
            if (adj >= 0 == (passengerCount >= 1)) {
                int maxPax = Ship_getMaxPassengers(Status_getShip());
                int used = Item_getAttribute(curItem);
                if (maxPax - used < globals->field_0x34) {
                    showText(self, *g_hw_unsaleableTextId);
                    return;
                }
            }
        }
    }

    if (item->field_0x3c >= 0) {
        void *ci = HangarList_getCurrentItem(self->field_0x14);
        HangarWindow_demountItem(self, curItem, G<int>(ci, 0x40));
        return;
    }

    // Mounting: check for an existing equipment of the same sort.
    void *globals = *g_hw_globals;
    int sort = Item_getSort(item->field_0x10);
    void *existing = Ship_getFirstEquipmentOfSort(Status_getShip(), sort);
    bool conflict = false;
    if (Item_getSort(item->field_0x10) == 0x15) {
        if (Ship_getIndex(Status_getShip()) != 0x2c &&
            existing != 0 && Ship_getIndex(Status_getShip()) != 0x31)
            conflict = true;
    } else if (existing != 0) {
        conflict = true;
    }

    if (conflict && Item_canBeInstalledMultipleTimes(item->field_0x10) == 0) {
        String12 name, copy, etext, fmt, result, etext2, fmt2, result2;
        AEString_ctor(&name, (const char *)GameText_getText(*g_hw_unsaleableTextId), false);
        AEString_ctor_str(&copy, &name, false);
        Item_getIndex(existing);
        AEString_ctor(&etext, (const char *)GameText_getText(*g_hw_unsaleableTextId), false);
        AEString_ctor(&fmt, hw_sel_fmt2, false);
        Status_replaceHash(&result, globals, &copy, &etext, &fmt);
        Item_getIndex(item->field_0x10);
        AEString_ctor(&etext2, (const char *)GameText_getText(*g_hw_unsaleableTextId), false);
        AEString_ctor(&fmt2, hw_sel_fmt3, false);
        Status_replaceHash(&result2, globals, &result, &etext2, &fmt2);
        AEString_assign(&name, &result2);
        AEString_dtor(&result2);
        AEString_dtor(&fmt2);
        AEString_dtor(&etext2);
        AEString_dtor(&result);
        AEString_dtor(&fmt);
        AEString_dtor(&etext);
        AEString_dtor(&copy);
        ChoiceWindow_setMsg(self->field_0x20, &name, true);
        self->field_0x11c = 1;
        self->field_0x3c = 1;
        self->field_0x28 = item->field_0x10;
        self->field_0x2c = existing;
        AEString_dtor(&name);
        return;
    }

    HangarWindow_mountItem(self, item->field_0x10);
}

// ---- getRelativeScrollHeight_14b6ec.cpp ----
// Returns float in r0. a=this+0xd4 (denominator), b=this+0xd8.
// NEAR: clang pairs a->r2,b->r1 in the ldrd (target uses r1=a,r2=b), mirroring every
// subsequent op; register pairing is an allocator tie-break, not source-driven.
extern "C" float HangarWindow_getRelativeScrollHeight(HangarWindow *self)
{
    int a = self->field_0xd4;
    int b = self->field_0xd8;
    if (a < b) {
        union { unsigned u; float f; } c; c.u = 0x4605e009u;
        return c.f;
    }
    int e = self->field_0xb4;
    int num;
    if (e >= 1) {
        num = b - e;
    } else if (e >= b - a) {
        num = b;
    } else {
        num = e + a;
    }
    return (float)num / (float)a;
}

// ---- transaction_148c68.cpp ----
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
unsigned int HangarList_getCurrentTab(void *list);
int Item_isUnsaleable(void *item);
unsigned int Item_transaction(void *item, bool buy, int amount, bool flag);
int Item_getIndex(void *item);
int Item_getSinglePrice(void *item);
int Item_getAmount(void *item);
void Item_setAmount(void *item, int amount);
int Item_getBlueprintAmount(void *item);
void Item_setBlueprintAmount(void *item, int amount);
int Item_transactionBlueprint(void *item, int z);
void *Item_makeItem(void *item);
int Status_getCredits();
void Status_changeCredits(void *globals);
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b, String12 *c);
void *Status_getShip();
int BluePrint_getRemainingAmount(void *bp, int index);
void Ship_changeLoad(void *ship, int delta);
void *Ship_getCargo(void *ship);
void *GameText_getText(int id);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);
void Layout_formatCredits(String12 *out, int price, int credits);
void ChoiceWindow_setMsg(void *win, String12 *msg, bool flag);
void TouchButton_resetTouch(void *btn);
void ChoiceWindow_setText(void *win, void *text);
int ListItem_getIndex(void *item);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern int *g_hw_unsaleableTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_notEnoughTextId;

extern "C" void HangarWindow_transaction(HangarWindow *self, bool buy)
{
    unsigned int tab = HangarList_getCurrentTab(self->field_0x14);
    void *cur = G<void *>(self->field_0x68, 0x10);

    if (tab < 2) {
        if (Item_isUnsaleable(cur) != 0) {
            GameText_getText(*g_hw_unsaleableTextId);
            ChoiceWindow_setText(self->field_0x20, 0);
            self->field_0x88 = 0;
            self->field_0x3c = 1;
            return;
        }

        unsigned int result = Item_transaction(cur, buy, self->field_0xa8, self->field_0x11d);
        unsigned int idx = Item_getIndex(cur);
        void *globals = *g_hw_globals;
        unsigned int *avail = globals->field_0x54;
        if (idx < avail[0])
            *((uint8_t *)avail[1] + Item_getIndex(cur)) = 1;

        if (result >= 0x80000000 && buy) {
            self->field_0xa8 = self->field_0xa8 + 1;
            Ship_changeLoad(Status_getShip(), 1);
            int li = ListItem_getIndex(self->field_0x68);
            if (li >= 0x84 && ListItem_getIndex(self->field_0x68) < 0x9a) {
                int base = globals->field_0xac;
                *((uint8_t *)G<int>((void *)(uintptr_t)base, 4) +
                  ListItem_getIndex(self->field_0x68) - 0x84) = 1;
            }
        } else if (result == 0 && buy) {
            if (Status_getCredits() < Item_getSinglePrice(cur)) {
                if (self->field_0x11d != 0)
                    return;
                String12 line, priceStr, fmt, msg, suffix, combined;
                AEString_ctor(&line, (const char *)GameText_getText(*g_hw_notEnoughTextId), false);
                Layout_formatCredits(&priceStr, Item_getSinglePrice(cur), Status_getCredits());
                AEString_ctor_str(&line, &priceStr, false);
                AEString_ctor(&fmt, hw_tx_fmt, false);
                Status_replaceHash(&msg, globals, &line, &priceStr, &fmt);
                AEString_dtor(&fmt);
                AEString_dtor(&priceStr);
                AEString_ctor(&suffix, hw_tx_suffix, false);
                GameText_getText(*g_hw_notEnoughTextId);
                AEString_add(&combined, &suffix, &suffix);
                AEString_addAssign(&msg, &combined);
                AEString_dtor(&combined);
                AEString_dtor(&suffix);
                ChoiceWindow_setMsg(self->field_0x20, &msg, true);
                self->field_0x3c = 1;
                self->field_0xaf = 1;
                TouchButton_resetTouch(G<void *>(G<void *>(self->field_0x24, 4), 0x20));
                TouchButton_resetTouch(G<void *>(G<void *>(self->field_0x24, 4), 0x24));
                AEString_dtor(&msg);
            }
        } else if ((int)result > 0 && !buy) {
            self->field_0xa8 = self->field_0xa8 - 1;
            Ship_changeLoad(Status_getShip(), -1);
        }

        if (self->field_0x11d == 0)
            Status_changeCredits(globals);
    } else if (tab == 4) {
        if (buy) {
            int bpAmt = Item_getBlueprintAmount(cur);
            void *bp = self->field_0x80;
            int remaining = BluePrint_getRemainingAmount(bp, Item_getIndex(cur));
            if (bpAmt < remaining) {
                int r = Item_transactionBlueprint(cur, 0);
                if (r < 0) {
                    self->field_0xa8 = self->field_0xa8 + 1;
                } else if (r != 0) {
                    self->field_0x94 = self->field_0x94 + 1;
                    Ship_changeLoad(Status_getShip(), -1);
                }
            }
        }
        void *cargo = Ship_getCargo(Status_getShip());
        if (cargo != 0) {
            Array<void *> *arr = (Array<void *> *)cargo;
            for (unsigned int i = 0; i < arr->length; i++) {
                if (Item_getIndex(arr->data[i]) == Item_getIndex(cur)) {
                    Item_setAmount(arr->data[i], Item_getAmount(arr->data[i]));
                    Item_setBlueprintAmount(arr->data[i], Item_getBlueprintAmount(cur));
                }
            }
        }
    }
}

// ---- mountItem_14df98.cpp ----
extern "C" {
int Item_getType(void *item);
int Item_getAmount(void *item);
void *Item_makeItem(void *item);
int Item_getIndex(void *item);
int Item_getStationAmount(void *item);
void Item_changeAmount(void *item, int delta);
void *Item_extractItems(void *arr, bool flag);
void *Item_mixItems(void *a, void *b);
void *Status_getShip();
void *Status_getStation();
void Ship_addEquipment(void *ship, void *item);
void Ship_removeCargo(void *ship, int index, int amount);
void Ship_setCargo(void *ship, void *arr);
void *Ship_getCargo(void *ship);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
void ArrayRemove_ItemPtr(void *item, void *arr);
void HangarList_initShipTab(void *list, void *ship);
void HangarList_initShopTab(void *list, void *items, void *ships);
void HangarList_setCurrentTab(void *list, bool flag);
void FModSound_play(void *snd, int id, void *a, void *b, float pitch);
float HangarWindow_refreshCurrentContentHeight(HangarWindow *self);
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern void **g_hw_sound;

extern "C" void HangarWindow_mountItem(HangarWindow *self, void *item)
{
    int type = Item_getType(item);
    int amount = 1;
    if (type == 1)
        amount = Item_getAmount(item);

    void *made = Item_makeItem(item);
    void *ship = Status_getShip();
    Ship_addEquipment(ship, made);
    Ship_removeCargo(Status_getShip(), Item_getIndex(made), (type == 1) ? amount : 1);

    Array<void *> *cargo = F<Array<void *> *>(self, 0x10);
    if (cargo != 0) {
        for (unsigned int i = 0; i < cargo->length; i++) {
            void *cur = cargo->data[i];
            if (Item_getIndex(cur) == Item_getIndex(item)) {
                int change;
                if (Item_getStationAmount(cur) == 0) {
                    if (type == 1 || Item_getAmount(item) == 1) {
                        ArrayRemove_ItemPtr(cur, self->field_0x10);
                        break;
                    }
                    change = -1;
                } else if (type == 1) {
                    change = -Item_getAmount(item);
                } else {
                    change = -1;
                }
                Item_changeAmount(cur, change);
                break;
            }
            cargo = F<Array<void *> *>(self, 0x10);
        }
    }

    void *ship2 = HangarWindow_statusShip();
    Ship_setCargo(ship2, Item_extractItems(self->field_0x10, true));
    HangarList_initShipTab(self->field_0x14, HangarWindow_statusShip());

    HangarWindow_statusShip();
    void *items = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
    HangarList_initShopTab(self->field_0x14, items, Station_getShips(Status_getStation()));
    HangarList_setCurrentTab(self->field_0x14, false);

    float h = HangarWindow_refreshCurrentContentHeight(self);
    self->field_0xb4 = self->field_0xe4;
    FModSound_play(*g_hw_sound, 0x62, 0, 0, h);
}

// ---- OnTouchMove_14be98.cpp ----
extern "C" {
void Layout_OnTouchMove(void *layout, int touch);
void ListItemWindow_OnTouchMove(void *win, int touch);
void TouchButton_OnTouchMove(void *btn, int touch);
void TouchButton_resetTouch(void *btn);
int TouchButton_isTouched(void *btn);
void ChoiceWindow_OnTouchMove(void *win, int touch);
void HangarWindow_setSellMode(HangarWindow *self);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;

extern "C" unsigned int HangarWindow_OnTouchMove(HangarWindow *self, int touch, int coord)
{
    void *layout = *g_hw_layout;
    Layout_OnTouchMove(layout, touch);

    if (self->field_0x3c != 0) {
        if (self->field_0xae != 0) {
            for (int i = 0xc; i != 0x11; i++)
                TouchButton_OnTouchMove(G<void *>(G<void *>(self->field_0x24, 4), i * 4), touch);
            TouchButton_OnTouchMove(G<void *>(G<void *>(self->field_0x24, 4), 0x44), touch);
        } else if (self->field_0xb0 != 0) {
            for (int i = 0x12; i != 0x17; i++)
                TouchButton_OnTouchMove(G<void *>(G<void *>(self->field_0x24, 4), i * 4), touch);
        }
        ChoiceWindow_OnTouchMove(self->field_0x20, touch);
        return 0;
    }

    if (self->field_0x58 == 1) {
        ListItemWindow_OnTouchMove(self->field_0x18, touch);
        return 0;
    }

    if (layout->field_0xc < coord && coord < *g_hw_screenWidth - layout->field_0x10) {
        int dy = coord - self->field_0xb8;
        self->field_0xc0 = dy;
        self->field_0xc4 = 1.0f;
        self->field_0xb4 = self->field_0xb4 + dy;
        self->field_0xb8 = coord;

        void *btnUp = G<void *>(G<void *>(self->field_0x24, 4), 0x20);
        void *btnDown = G<void *>(G<void *>(self->field_0x24, 4), 0x24);
        int touched = TouchButton_isTouched(btnUp);
        if (touched != 0)
            touched = 1;
        else
            touched = TouchButton_isTouched(btnDown);

        int adist = coord - self->field_0xcc;
        if (adist < 0)
            adist = -adist;

        if (touched == 0 && adist > 5) {
            self->field_0x6c = 0;
            self->field_0x70 = 0;
            Array<void *> *buttons = F<Array<void *> *>(self, 0x24);
            for (unsigned int i = 0; i < buttons->length; i++)
                TouchButton_OnTouchMove(buttons->data[i], touch);
            HangarWindow_setSellMode(self);
            self->field_0xd2 = 0;
            self->field_0x68 = 0;
            TouchButton_resetTouch(btnUp);
            TouchButton_resetTouch(btnDown);
        }
    }

    Array<void *> *tabs = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < tabs->length; i++)
        TouchButton_OnTouchMove(tabs->data[i], touch);
    return 0;
}

// ---- autoEquipSecondaryWeapons_14bc50.cpp ----
struct String12 { uint32_t a, b, c; };

extern "C" {
void *HangarList_getCurrentItemAt(void *list, int idx);
int ListItem_isItem(void *item);
int Item_getType(void *item);
int Item_getAmount(void *item);
int Item_getIndex(void *item);
void *Item_makeItem(void *item);
void Item_setAmount(void *item, int amount);
int *Ship_getEquipment(void *ship, int slot);
void Ship_setEquipment(void *ship, void *item);
void Ship_removeCargo(void *ship, int index, int amount);
void *Status_getShip();
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b, String12 *c);
void *GameText_getText(int id);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_copy(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void HangarList_initShipTab(void *list, void *ship);
void ChoiceWindow_set(void *win, String12 *msg);
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern int *g_hw_equipTextId;
__attribute__((visibility("hidden"))) extern void **g_hw_globals;

extern "C" void HangarWindow_autoEquipSecondaryWeapons(HangarWindow *self, int row)
{
    void *item = HangarList_getCurrentItemAt(self->field_0x14, row);
    if (item == 0)
        return;
    void *itm = item->field_0x10;
    if (itm == 0 || Item_getType(itm) != 1)
        return;
    if (ListItem_isItem(item) == 0 || Item_getType(itm) != 1)
        return;
    if (Item_getAmount(itm) <= 0)
        return;

    void *ship = Status_getShip();
    int *equip = Ship_getEquipment(ship, 1);
    if (equip == 0)
        return;

    Array<void *> *arr = (Array<void *> *)equip;
    for (unsigned int i = 0; i < arr->length; i++) {
        void *cur = arr->data[i];
        if (cur == 0)
            continue;
        if (Item_getIndex(cur) != Item_getIndex(itm))
            continue;

        void *made = Item_makeItem(itm);

        Array<void *> *cargo = F<Array<void *> *>(self, 0x10);
        if (cargo != 0) {
            for (unsigned int j = 0; j < cargo->length; j++) {
                if (Item_getIndex(cargo->data[j]) == Item_getIndex(made))
                    Item_setAmount(cargo->data[j], 0);
                cargo = F<Array<void *> *>(self, 0x10);
            }
        }

        Ship_setEquipment(HangarWindow_statusShip(), made);
        Ship_removeCargo(HangarWindow_statusShip(), Item_getIndex(made), Item_getAmount(itm));
        HangarList_initShipTab(self->field_0x14, HangarWindow_statusShip());

        String12 msg, msgCopy, name, fmt;
        AEString_ctor(&msg, (const char *)GameText_getText(*g_hw_equipTextId), false);
        AEString_ctor_copy(&msgCopy, &msg, false);
        AEString_ctor(&name, (const char *)GameText_getText(*g_hw_equipTextId), false);
        AEString_ctor(&fmt, hw_equip_fmt, false);
        String12 result;
        Status_replaceHash(&result, *g_hw_globals, &msgCopy, &name, &fmt);
        AEString_assign(&msg, &result);
        AEString_dtor(&result);
        AEString_dtor(&fmt);
        AEString_dtor(&name);
        AEString_dtor(&msgCopy);

        ChoiceWindow_set(self->field_0x20, &msg);
        self->field_0xad = 1;
        self->field_0x3c = 1;
        AEString_dtor(&msg);
        break;
    }
}

// ---- showFreeCreditsWindow_14e32c.cpp ----
typedef unsigned int uintptr_t;

// AbyssEngine::String - 12-byte value type.
struct String12 { uint32_t a, b, c; };

extern "C" {
void *ApplicationManager_GetApplicationData();
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
void *GameText_getText(int id);
void ChoiceWindow_set(void *win, String12 *title, String12 *t2, bool flag,
                      String12 *yes, String12 *no, void *body, int a, int b);
void ChoiceWindow_setWidth(void *win, int w);
void ChoiceWindow_setHeight(void *win, int h);
int TouchButton_getHeight(void *btn);
int TouchButton_getWidth(void *btn);
int PaintCanvas_GetTextWidth(unsigned int canvas, void *str);
}

__attribute__((visibility("hidden"))) extern int *g_hw_freeCreditsTextId;
__attribute__((visibility("hidden"))) extern void **g_hw_canvas;
__attribute__((visibility("hidden"))) extern void **g_hw_layout;

extern "C" void HangarWindow_showFreeCreditsWindow(HangarWindow *self)
{
    void *appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x4c) = 0;
    appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x3d) = 1;

    void *win = self->field_0x20;
    String12 title, title2, yes, no;
    AEString_ctor(&title, hw_freeCredits_yes, false);
    AEString_ctor(&title2, hw_freeCredits_yes, false);
    AEString_ctor(&yes, hw_freeCredits_no, false);
    AEString_ctor(&no, hw_freeCredits_icon, false);
    void *body = GameText_getText(*g_hw_freeCreditsTextId);
    ChoiceWindow_set(win, &title, &title2, false, &yes, &no, body, -1, -1);
    AEString_dtor(&no);
    AEString_dtor(&yes);
    AEString_dtor(&title2);
    AEString_dtor(&title);

    int rowH = TouchButton_getHeight(G<void *>(G<void *>(self->field_0x24, 4), 0x48));
    ChoiceWindow_setHeight(self->field_0x20, rowH * 5);

    int maxW = 0;
    for (int i = 5; i != 0; i--) {
        GameText_getText(*g_hw_freeCreditsTextId);
        int w = PaintCanvas_GetTextWidth((unsigned int)(uintptr_t)*g_hw_canvas, 0);
        if (maxW < w)
            maxW = w;
    }
    int btnW = TouchButton_getWidth(0);
    void *layout = *g_hw_layout;
    ChoiceWindow_setWidth(self->field_0x20,
                          layout->field_0x2c + btnW + maxW + layout->field_0x28 * 4);

    self->field_0xb0 = 1;
    self->field_0x3c = 1;
    self->field_0xae = 0;
    self->field_0xaf = 0;
}

// ---- initialize_147e24.cpp ----
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
void *Status_getStation();
int Station_getIndex(void *station);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
void Status_calcCargoPrices(void *status);
void *Status_getShip();
void *Status_getSystem();
void *Status_getBluePrints(void *status);
int Status_getCredits();
int Status_getCurrentCampaignMission();
int Status_inBlackMarketSystem();
int SolarSystem_getIndex(void *sys);
void *Ship_getCargo(void *ship);
void Ship_adjustPrice(void *ship);
int Ship_getCurrentLoad(void *ship);
void *Ship_getEquipment(void *ship);
void *Item_mixItems(void *a, void *b);
int Item_getSinglePrice(void *item);
int Item_getIndex(void *item);
void *operator_new(unsigned int n);
void *operator_new__(unsigned int n);
int __aeabi_idiv(int a, int b);

void HangarList_ctor(void *list);
void HangarList_init(void *list, void *ship, void *items, void *ships, void *bps);
void HangarList_setCurrentTab(void *list, bool flag);

void Array_TouchButtonPtr_ctor(void *arr);
void ArraySetLength_TouchButtonPtr(int len, void *arr);

void *GameText_getText(int id);
int Layout_getHelpButtonOffset();
int Layout_getFooterTransitionWidth();
void Layout_formatCredits(String12 *out, int credits);

// TouchButton constructor overloads (distinct argument arities in the target).
void TouchButton_ctor_text(void *btn, void *text, int a, int b, int c, char k);
void TouchButton_ctor_text2(void *btn, void *text, int a, int b, int c, int d, char k);
void TouchButton_ctor_img(void *btn, void *img, int a, int b, int c, int d, char k, char m);
void TouchButton_ctor_img2(void *btn, void *imgA, void *imgB, int a, int b, int c, char k);
int TouchButton_getWidth(void *btn);
int TouchButton_getHeight(void *btn);
void TouchButton_getPosition(void *btn, float *x, float *y);
void TouchButton_setVisible(void *btn, bool vis);

void PaintCanvas_Image2DCreate(void *canvas, int id, unsigned int *out);
int PaintCanvas_GetImage2DWidth(void *canvas);
int PaintCanvas_GetImage2DHeight(void *canvas);

void ListItemWindow_ctor(void *win);
void ChoiceWindow_ctor(void *win);
void ChoiceWindow_set(void *win, void *text);

void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);

void RecordHandler_saveOptions(void *rh);

void HangarWindow_refreshCurrentContentHeight(HangarWindow *self);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;          // *piVar27 (Status singleton)
__attribute__((visibility("hidden"))) extern void **g_hw_canvas;           // *puVar30 (PaintCanvas)
__attribute__((visibility("hidden"))) extern void **g_hw_layout;           // *piVar22 (layout config)
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;        // *piVar11
__attribute__((visibility("hidden"))) extern int *g_hw_screenHeight;       // *piVar15
__attribute__((visibility("hidden"))) extern int *g_hw_helpTextId;         // *piVar9
__attribute__((visibility("hidden"))) extern void **g_hw_posXArray;        // DAT for X positions
__attribute__((visibility("hidden"))) extern void **g_hw_posYArray;        // DAT for Y positions
__attribute__((visibility("hidden"))) extern void **g_hw_imageCountSlot;   // mirror of tab count
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_specialModeFlag;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_blackMarketHintFlag;
__attribute__((visibility("hidden"))) extern void **g_hw_recordHandler;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_listModeFlag;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_introHintFlag;
__attribute__((visibility("hidden"))) extern float g_hw_posScale;
extern "C" extern const char hw_init_buy[], hw_init_sell[], hw_init_lbl[],
    hw_init_more[], hw_init_back[], hw_init_help[];

extern "C" void HangarWindow_initialize(HangarWindow *self)
{
    void *status = *g_hw_globals;

    // Special "shipyard upgrade" mode at station 0x6c with campaign stage 3.
    uint8_t special = 0;
    if (Station_getIndex(Status_getStation()) == 0x6c)
        special = (status->field_0x114 == 3);
    self->field_0x11d = special;
    Status_calcCargoPrices(status);

    // Build the hangar item list.
    void *list = operator_new(0xc);
    HangarList_ctor(list);
    self->field_0x14 = list;
    void *mixed = Item_mixItems(Ship_getCargo(Status_getShip()),
                                Station_getItems(Status_getStation()));
    self->field_0x10 = mixed;
    HangarList_init(list, Status_getShip(), 0, Station_getShips(Status_getStation()),
                    Status_getBluePrints(*g_hw_globals));

    // Tab bar: 3 help/info buttons across the top.
    void *tabs = operator_new(0xc);
    Array_TouchButtonPtr_ctor(tabs);
    self->field_0x4 = tabs;
    ArraySetLength_TouchButtonPtr(3, tabs);

    int scrW = *g_hw_screenWidth;
    void *layout = *g_hw_layout;

    void *b0 = operator_new(200);
    TouchButton_ctor_text(b0, GameText_getText(*g_hw_helpTextId), 3,
                          scrW - Layout_getHelpButtonOffset(), 0, 0x12);
    G<void *>(G<void *>(self->field_0x4, 4), 8) = b0;

    void *b1 = operator_new(200);
    int w0 = TouchButton_getWidth(b0);
    TouchButton_ctor_text(b1, GameText_getText(*g_hw_helpTextId), 3,
                          (scrW - Layout_getHelpButtonOffset() - w0) + layout->field_0x38, 0, 0x12);
    G<void *>(G<void *>(self->field_0x4, 4), 4) = b1;

    void *b2 = operator_new(200);
    int w0b = TouchButton_getWidth(b0);
    int w1b = TouchButton_getWidth(b1);
    TouchButton_ctor_text(b2, GameText_getText(*g_hw_helpTextId), 3,
                          (scrW - Layout_getHelpButtonOffset() - w0b - w1b) + layout->field_0x38 * 2,
                          0, 0x12);
    G<void *>(G<void *>(self->field_0x4, 4), 0) = b2;
    self->field_0x11f = *g_hw_listModeFlag;

    // Six tab icons.
    void *icons = operator_new__(0x18);
    self->field_0x30 = icons;
    for (int i = 0; i != 6; i++)
        PaintCanvas_Image2DCreate(*g_hw_canvas, i + 0x232a, (unsigned int *)((char *)icons + i * 4));

    int *posX = (int *)*g_hw_posXArray;
    int *posY = (int *)*g_hw_posYArray;
    Array<void *> *tabArr = (Array<void *> *)self->field_0x4;
    for (unsigned int i = 0; i < tabArr->length; i++) {
        if (i < 10) {
            float x = 0, y = 0;
            TouchButton_getPosition(tabArr->data[i], &x, &y);
            posX[i] = (int)x;
            TouchButton_getPosition(tabArr->data[i], &x, &y);
            posY[i] = (int)y;
        }
    }

    *(unsigned int *)*g_hw_imageCountSlot = tabArr->length;
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x52e, (unsigned int *)((char *)self + 0xe8));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x544, (unsigned int *)((char *)self + 0xec));

    // Action button bank (24 entries).
    void *btns = operator_new(0xc);
    Array_TouchButtonPtr_ctor(btns);
    self->field_0x24 = btns;
    ArraySetLength_TouchButtonPtr(0x18, btns);

    unsigned int img;
    img = 0xffffffff;
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x470, &img);
    void *e0 = operator_new(200);
    TouchButton_ctor_img((void *)e0, (void *)(uintptr_t)img, 7, 0, 0, layout->field_0x60, 0x11, 4);
    G<void *>(G<void *>(self->field_0x24, 4), 0) = e0;

    if (Status_getCurrentCampaignMission() == 0x4d)
        Station_getIndex(Status_getStation());

    void *e1 = operator_new(200);
    TouchButton_ctor_text(e1, GameText_getText(*g_hw_helpTextId), 7, 0, 0, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 4) = e1;
    void *e2 = operator_new(200);
    TouchButton_ctor_text(e2, GameText_getText(*g_hw_helpTextId), 7, 0, 0, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 8) = e2;

    img = 0xffffffff;
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x533, &img);
    void *e3 = operator_new(200);
    TouchButton_ctor_img((void *)e3, (void *)(uintptr_t)img, 7, 0, 0, layout->field_0x64, 0x11, 4);
    G<void *>(G<void *>(self->field_0x24, 4), 0xc) = e3;

    img = 0xffffffff;
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x532, &img);
    void *e4 = operator_new(200);
    TouchButton_ctor_img((void *)e4, (void *)(uintptr_t)img, 7, 0, 0, layout->field_0x64, 0x11, 4);
    G<void *>(G<void *>(self->field_0x24, 4), 0x10) = e4;

    void *e5 = operator_new(200);
    TouchButton_ctor_text2(e5, GameText_getText(*g_hw_helpTextId), 7, 0, 0, self->field_0x110, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 0x14) = e5;
    void *e6 = operator_new(200);
    TouchButton_ctor_text2(e6, GameText_getText(*g_hw_helpTextId), 7, 0, 0, self->field_0x110, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 0x18) = e6;
    void *e7 = operator_new(200);
    TouchButton_ctor_text(e7, GameText_getText(*g_hw_helpTextId), 7, 0, 0, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 0x1c) = e7;

    {
        String12 lbl;
        void *e8 = operator_new(200);
        AEString_ctor(&lbl, hw_init_buy, false);
        TouchButton_ctor_img((void *)e8, &lbl, 8, 0, 0, layout->field_0x50, 0x11, 4);
        G<void *>(G<void *>(self->field_0x24, 4), 0x20) = e8;
        AEString_dtor(&lbl);
    }
    {
        String12 lbl;
        void *e9 = operator_new(200);
        AEString_ctor(&lbl, hw_init_sell, false);
        TouchButton_ctor_img((void *)e9, &lbl, 9, 0, 0, layout->field_0x50, 0x11, 4);
        G<void *>(G<void *>(self->field_0x24, 4), 0x24) = e9;
        AEString_dtor(&lbl);
    }
    {
        void *e10 = operator_new(200);
        TouchButton_ctor_img((void *)e10, GameText_getText(*g_hw_helpTextId), 7, 0, 0,
                             layout->field_0x50, 0x11, 4);
        G<void *>(G<void *>(self->field_0x24, 4), 0x28) = e10;
    }
    {
        String12 credits;
        void *e11 = operator_new(200);
        Layout_formatCredits(&credits, Status_getCredits());
        TouchButton_ctor_img((void *)e11, &credits, 0xb, *g_hw_screenWidth, *g_hw_screenHeight,
                             Layout_getFooterTransitionWidth(), 0x22, 4);
        G<void *>(G<void *>(self->field_0x24, 4), 0x2c) = e11;
        AEString_dtor(&credits);
    }

    // Five collapsible "more" rows (indices 0x0c..0x10 in slot terms).
    uint8_t listMode = self->field_0x11f;
    int row = 0;
    for (int slot = 0xc; (unsigned int)(slot - 0xc) < 5; slot++) {
        String12 lbl;
        void *btn = operator_new(200);
        AEString_ctor(&lbl, hw_init_more, false);
        int visIdx;
        if (listMode == 0) {
            TouchButton_ctor_img(btn, &lbl, 0, 0, 0, layout->field_0x264, 0x11, 1);
            visIdx = row + 0xc;
        } else {
            TouchButton_ctor_text(btn, &lbl, 10, 0, 0, 1);
            visIdx = slot;
        }
        G<void *>(G<void *>(self->field_0x24, 4), slot * 4) = btn;
        AEString_dtor(&lbl);
        TouchButton_setVisible(G<void *>(G<void *>(self->field_0x24, 4), visIdx * 4), false);
        row++;
    }
    {
        String12 lbl;
        void *btn = operator_new(200);
        AEString_ctor(&lbl, hw_init_back, false);
        if (listMode == 0)
            TouchButton_ctor_img(btn, &lbl, 0, 0, 0, layout->field_0x264, 0x11, 1);
        else
            TouchButton_ctor_text(btn, &lbl, 10, 0, 0, 1);
        G<void *>(G<void *>(self->field_0x24, 4), 0x44) = btn;
        AEString_dtor(&lbl);
        TouchButton_setVisible(G<void *>(G<void *>(self->field_0x24, 4), 0x44), false);
    }

    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233e, (unsigned int *)((char *)self + 0x34));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233f, (unsigned int *)((char *)self + 0x38));
    {
        String12 lbl;
        void *btn = operator_new(200);
        AEString_ctor(&lbl, hw_init_help, false);
        TouchButton_ctor_text(btn, &lbl, 7, 0, 0, 0x11);
        G<void *>(G<void *>(self->field_0x24, 4), 0x5c) = btn;
        AEString_dtor(&lbl);
    }

    // Tab selector icons (5 entries with paired up/down images).
    unsigned int imgA, imgB;
    for (int i = 0x12; (unsigned int)(i - 0x12) < 5; i++) {
        imgB = 0xffffffff;
        if (i == 0x12) {
            PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233c, &imgA);
            PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233d, &imgB);
        } else {
            short s = (short)(i - 0x12);
            PaintCanvas_Image2DCreate(*g_hw_canvas, s * 2 + 0x2330, &imgA);
            PaintCanvas_Image2DCreate(*g_hw_canvas, s * 2 + 0x2331, &imgB);
        }
        void *btn = operator_new(200);
        TouchButton_ctor_img2(btn, (void *)(uintptr_t)imgA, (void *)(uintptr_t)imgB, 0x13, 0, 0, 1);
        G<void *>(G<void *>(self->field_0x24, 4), i * 4) = btn;
        TouchButton_setVisible(G<void *>(G<void *>(self->field_0x24, 4), i * 4), false);
    }

    self->field_0x120 = TouchButton_getWidth(0);
    int h = TouchButton_getHeight(G<void *>(G<void *>(self->field_0x24, 4), 0x30));
    self->field_0x124 = h;
    self->field_0x128 = (int)((float)(-self->field_0x120) * g_hw_posScale);
    self->field_0x12c = (int)((float)(-h) * g_hw_posScale);

    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x475, (unsigned int *)((char *)self + 0x78));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x476, (unsigned int *)((char *)self + 0x7c));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x477, (unsigned int *)((char *)self + 0x74));
    self->field_0xdc = PaintCanvas_GetImage2DWidth(*g_hw_canvas);
    self->field_0xe0 = PaintCanvas_GetImage2DHeight(*g_hw_canvas);

    // Recompute best cargo prices from known equipment when not in a black market.
    if (self->field_0x10 != 0 && Status_inBlackMarketSystem() == 0 &&
        self->field_0x11d == 0) {
        unsigned int *equip = (unsigned int *)Ship_getEquipment(Status_getShip());
        Array<void *> *cargo = (Array<void *> *)self->field_0x10;
        unsigned int n = cargo->length + (equip ? equip[0] : 0);
        for (unsigned int i = 0; i < n; i++) {
            void *itemPtr;
            if (i < cargo->length)
                itemPtr = cargo->data[i];
            else
                itemPtr = ((void **)equip[1])[i - cargo->length];
            if (itemPtr != 0) {
                int price = Item_getSinglePrice(itemPtr);
                int idx = Item_getIndex(itemPtr);
                void *globals = *g_hw_globals;
                int *buyTbl = (int *)G<int>(globals->field_0x40, 4);
                if (buyTbl[idx] < price || buyTbl[idx] == 0) {
                    buyTbl[idx] = price;
                    int sysIdx = SolarSystem_getIndex(Status_getSystem());
                    *(int *)(G<int>(globals->field_0x48, 4) + idx * 4) = sysIdx;
                }
                int *sellTbl = (int *)G<int>(globals->field_0x3c, 4);
                if (price < sellTbl[idx] || sellTbl[idx] == 0) {
                    sellTbl[idx] = price;
                    int sysIdx = SolarSystem_getIndex(Status_getSystem());
                    *(int *)(G<int>(G<void *>(*g_hw_globals, 0x44), 4) + idx * 4) = sysIdx;
                }
            }
        }
    }

    Ship_adjustPrice(Status_getShip());

    void *liw = operator_new(0x13c);
    ListItemWindow_ctor(liw);
    self->field_0x18 = liw;
    void *cw1 = operator_new(0x5c);
    ChoiceWindow_ctor(cw1);
    self->field_0x58 = 0;
    self->field_0x1c = cw1;
    void *cw2 = operator_new(0x5c);
    ChoiceWindow_ctor(cw2);
    self->field_0x3c = 0;
    self->field_0x20 = cw2;

    self->field_0x11c = 0;
    self->field_0x6c = 0;
    self->field_0x70 = 0;
    self->field_0x28 = 0;
    self->field_0x2c = 0;
    self->field_0x130 = 0;
    self->field_0xd1 = 0;
    self->field_0xb1 = 0;
    self->field_0xad = 0;

    int scrH = *g_hw_screenHeight;
    int contentW = *g_hw_screenWidth - 10;
    self->field_0x40 = 0x10;
    self->field_0x44 = 5;
    self->field_0x48 = 5;
    self->field_0x4c = contentW;
    self->field_0x50 = scrH - 10;

    int *cols = (int *)operator_new__(0xc);
    self->field_0x54 = cols;
    int third = __aeabi_idiv(contentW, 3) - 2;
    cols[0] = third;
    cols[1] = third;
    cols[2] = (*g_hw_screenWidth - 0xe) + third * -2;

    HangarList_setCurrentTab(self->field_0x14, *g_hw_specialModeFlag != 0);
    HangarWindow_refreshCurrentContentHeight(self);

    self->field_0xa8 = Ship_getCurrentLoad(Status_getShip());
    void *lay2 = *g_hw_layout;
    self->field_0xd8 = ((*g_hw_screenHeight - G<int>(lay2, 0x10) - G<int>(lay2, 0xc)) -
                          G<int>(lay2, 0x20)) - G<int>(lay2, 0x24);

    int extra = 0;
    if (*g_hw_blackMarketHintFlag != 0 && *g_hw_introHintFlag == 0) {
        PaintCanvas_Image2DCreate(*g_hw_canvas, 0x6a4, (unsigned int *)((char *)self + 0xf0));
        extra = (int)((float)(*g_hw_screenWidth) * g_hw_posScale);
    }
    self->field_0x68 = 0;
    self->field_0xf4 = extra;
    self->field_0x88 = 0;
    self->field_0xac = 0;
    self->field_0x90 = 0;
    self->field_0x92 = 0;

    // Zero the 16-byte block at 0xc1 and the touch-drag block at 0xb4.
    self->field_0xc1 = 0;
    self->field_0xc5 = 0;
    self->field_0xc9 = 0;
    self->field_0xcd = 0;
    self->field_0xe4 = 0;
    self->field_0x0 = 0;
    self->field_0xc = 1;
    self->field_0xf8 = 0;
    self->field_0xfc = 0xffffffff;
    self->field_0xb4 = 0;
    self->field_0xb8 = 0;
    self->field_0xbc = 0;
    self->field_0xc0 = 0;

    // First-visit hint at later campaign stages.
    if (Status_getCurrentCampaignMission() > 0xd) {
        uint8_t *introFlag = g_hw_introHintFlag;
        if (introFlag[0x4e] == 0) {
            GameText_getText(*g_hw_helpTextId);
            ChoiceWindow_set(self->field_0x20, 0);
            introFlag[0x4e] = 1;
            RecordHandler_saveOptions(*g_hw_recordHandler);
            self->field_0x3c = 1;
        }
    }
}

// ---- HangarWindow_147d20.cpp ----
// Two PC-rel globals: a window-open counter (set to 1) and a layout/config singleton
// whose fields 0x238..0x250 are copied into this 0x100..0x118.
__attribute__((visibility("hidden"))) extern int *g_hw_openCounter;
__attribute__((visibility("hidden"))) extern void **g_hw_layout;

// NEAR: NEON block copies match (vld1/vst1.32), but clang schedules the many independent
// field-init stores in a different order and picks r12 (no frame) where the target uses r4
// (push). Structurally correct; instruction ordering differs.
struct __attribute__((aligned(4))) Blk16 { int a, b, c, d; };

extern "C" void HangarWindow_ctor(HangarWindow *self)
{
    int *cnt = g_hw_openCounter;
    void *lay = *g_hw_layout;

    self->field_0x80 = 0;
    self->field_0x84 = 0;
    self->field_0x3c = 0;
    self->field_0xac = 0;
    // zero 4 words at 0x14..0x20 (a zeroed Vector slot) via a 16-byte NEON store
    Blk16 zero = {0, 0, 0, 0};
    self->field_0x14 = zero;
    self->field_0x11e = 0;
    *cnt = 1;
    self->field_0x130 = 0;
    self->field_0xd0 = 0;
    self->field_0x4 = 0;
    self->field_0x24 = 0;
    self->field_0x90 = 0;
    self->field_0xae = 0;

    // copy lay[0x238..0x244] (16 bytes) into this[0x100..0x10c]
    self->field_0x100 = *(Blk16 *)((char *)lay + 0x238);
    self->field_0x110 = G<int>(lay, 0x248);
    self->field_0x114 = G<int>(lay, 0x24c);
    self->field_0x118 = G<int>(lay, 0x250);
}
