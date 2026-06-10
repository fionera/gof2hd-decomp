#include "gof2/HangarWindow.h"
#include "gof2/BluePrint.h"
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
#include "gof2/RecordHandler.h"
#include "gof2/Station.h"
#include "gof2/TouchButton.h"
#include "gof2/Globals.h"
#include "gof2/Status.h"
#include "gof2/ListItem.h"
// NOTE: gof2/Item.h is intentionally not included - Item is only ever used opaquely
// here (via the Item_* free functions), and fwd.h already forward-declares it.
// NOTE: gof2/Layout.h is intentionally not included - it (re)defines a placement-new
// that clashes with libc++'s <new>. The Layout struct we need is defined in
// gof2/HangarWindow.h instead (completes the fwd.h forward declaration).


extern "C" int *HangarList_getCurrentTabItems(void *list);
extern "C" int HangarList_getCurrentTab(void *list);
extern "C" int BluePrint_getStationIndex(void *bp);
extern "C" int Station_getIndex(void *station);
// Status singleton accessors: some merged sections call these with the singleton
// threaded explicitly, others rely on the implicit singleton. Declared variadic so
// every call arity matches the single extern "C" ABI symbol.
extern "C" void *Status_getStation(...);
extern "C" void *Status_getShip(...);
// Decompiled call sites disagree on the trailing arg type/arity (String12*/void*/int,
// 4 vs 5 args); these are extern "C" ABI symbols, so declare them variadic once.
extern "C" void Status_replaceHash(...);
extern "C" void ChoiceWindow_set(...);
extern "C" void ChoiceWindow_setMsg(...);
extern "C" void Layout_formatCredits(...);
extern "C" int PaintCanvas_GetTextWidth(...);
// Decompiled sections disagree on arity (1 vs 2 args) / return type; extern "C" ABI symbol.
extern "C" int *Ship_getEquipment(...);
extern "C" unsigned int Ship_addEquipment(...);
// Own method forward-declared here; the definition (returns int) lives later in this file.
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
extern "C" void Ship_adjustPrice(void *ship);
// Last arg is void* in some sections, float in others; variadic covers both.
extern "C" void FModSound_play(...);
extern "C" extern const char hw_otb_fmt1[], hw_otb_fmt2[];
extern "C" void *HangarList_getItems(void *list);
extern "C" void *Ship_getCargo(void *ship);
extern "C" void *BluePrint_getIngredientList(void *bp);
extern "C" int Item_getIndex(void *item);
extern "C" extern const char hw_bp_fmt1[], hw_bp_fmt2[], hw_bp_fmt3[];
extern "C" extern const char hw_sel_fmt1[], hw_sel_fmt2[], hw_sel_fmt3[];
extern "C" extern const char hw_tx_fmt[], hw_tx_suffix[];
extern "C" extern const char hw_equip_fmt[];
extern "C" extern const char hw_freeCredits_yes[];
extern "C" extern const char hw_freeCredits_no[];
extern "C" extern const char hw_freeCredits_icon[];

// ---- isInSpecialMode_148a64.cpp ----
bool HangarWindow::isInSpecialMode() {
    HangarWindow *self = this;
    if (self->field_0x89 != 0) return true;
    return self->field_0x3c != 0;
}

// ---- refreshCurrentContentHeight_148a10.cpp ----
// Globals singleton (pc-rel deref -> holder; *holder is the object), field +0x70 is the row height.
__attribute__((visibility("hidden"))) extern void **g_hw_globals;

void HangarWindow::refreshCurrentContentHeight() {
    HangarWindow *self = this;
    int *items = HangarList_getCurrentTabItems(self->field_0x14);
    if (items != 0) {
        int n = *items;
        int rowH = G<int>(*g_hw_globals, 0x70);
        self->field_0xd4 = self->field_0x10c * (n - 1) + n * rowH;
    }
}

// ---- listMode_148a4a.cpp ----
bool HangarWindow::listMode() {
    HangarWindow *self = this;
    if (self->field_0x58 == 0 && self->field_0x89 == 0) {
        return self->field_0x3c == 0;
    }
    return false;
}

// ---- readyToClose_14e4f0.cpp ----
// Status singleton (pc-rel deref -> holder; *holder is the Status object).
__attribute__((visibility("hidden"))) extern void **g_hw_status;

bool HangarWindow::readyToClose() {
    HangarWindow *self = this;
    int tab = HangarList_getCurrentTab(self->field_0x14);
    if (tab == 4 && self->field_0x88 != 0 && self->field_0x94 > 0 &&
        self->field_0x3c == 0 && ((BluePrint *)(self->field_0x80))->isEmpty() == 0) {
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

void HangarWindow::render3D() {
    HangarWindow *self = this;
    if (self->field_0x58 == 1) {
        return HangarWindow_render3D_thunk(self->field_0x18);
    }
}

// ---- currentItemIsHighlighted_14e550.cpp ----
bool HangarWindow::currentItemIsHighlighted() {
    HangarWindow *self = this;
    ListItem *item = (ListItem *)HangarList_getCurrentItem(self->field_0x14);
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
float HangarWindow::getRelativeScrollStartPos() {
    HangarWindow *self = this;
    int range = self->field_0xb4;
    if (range > 0) {
        union { unsigned u; float f; } c; c.u = 0x4650a903u;
        return c.f;
    }
    return -(float)range / (float)self->field_0xd4;
}

// ---- hideMessage_14ecc0.cpp ----
void HangarWindow::hideMessage() {
    HangarWindow *self = this;
    self->field_0x3c = 0;
}

// ---- render_1490d8.cpp ----


extern "C" {
void PaintCanvas_SetColor(unsigned int color);
int PaintCanvas_GetImage2DWidth(void *canvas);
int PaintCanvas_GetImage2DHeight(void *canvas);
int PaintCanvas_GetTextHeight(void *canvas);
void PaintCanvas_DrawImage2D(void *canvas, int img, int x, int y, char anchor);
void PaintCanvas_DrawRegion2D(void *canvas, unsigned int img, int sx, int sy, int sw, int sh,
                              float fw, int a, int b, int c, int y);
void PaintCanvas_DrawString(void *canvas, void *font, int text, int x, char anchor);



void Layout_drawHeader(void *layout, String12 *text);

int BluePrint_getIndex(void *bp);

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

int Status_getCredits();
void *Station_getItems(void *station);


void ImageFactory_drawItem(void *factory, int idx, int type, int x, int y);


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

void HangarWindow::render() {
    HangarWindow *self = this;
    Layout *layout = (Layout *)*g_hw_layout;
    void *canvas = *g_hw_canvas;
    PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);

    void *dlc = AppManager_GetApplicationModule(*g_hw_dlcModuleId);
    bool dlcShown = dlc != 0 && G<uint8_t>(dlc, 0x18) != 0;

    if (dlc == 0 || !dlcShown) {
        int tab2 = self->field_0x58;
        if (tab2 == 0) {
            ((Layout *)(layout))->drawBG();
            unsigned int tab = HangarList_getCurrentTab(self->field_0x14);
            Array<void *> *items = (Array<void *> *)HangarList_getCurrentTabItems(self->field_0x14);
            if (items != 0) {
                float startPos = ((HangarWindow *)(self))->getRelativeScrollStartPos();
                float visH = (float)self->field_0xd8;
                float scrollH = ((HangarWindow *)(self))->getRelativeScrollHeight();
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
                            ((TouchButton *)(btn))->setVisible(false);
                    }
                }

                int boxW = rowGap - 2;

                for (unsigned int i = 0; i < items->size(); i++) {
                    int y = (layout->field_0x70 + self->field_0x10c) * (int)i +
                            self->field_0xb4 + layout->field_0x20 + layout->field_0xc;
                    if (y < 0 || y > *g_hw_screenHeight)
                        continue;

                    void *li = items->data()[i];
                    if (((ListItem *)(li))->isSelectable() == 0)
                        continue;

                    // Row background box (selected vs unselected, by tab).
                    if (self->field_0x68 == li && ((ListItem *)(li))->isTextButton() == 0) {
                        String12 boxText;
                        if (tab == 0 && G<int>(li, 0x3c) >= 0) {
                            ((Layout *)(layout))->drawBox(10, self->field_0xf4 + layout->field_0x28, y, topY, layout->field_0x70, &boxText);
                        } else {
                            ((Layout *)(layout))->drawBox(4, self->field_0xf4 + layout->field_0x28, y, topY, layout->field_0x70, &boxText);
                        }
                    } else if (tab != 0 || G<int>(li, 0x3c) < 0) {
                        String12 boxText;
                        ((Layout *)(layout))->drawBox(3, self->field_0xf4 + layout->field_0x28, y, topY, layout->field_0x70, &boxText);
                    } else {
                        String12 boxText;
                        ((Layout *)(layout))->drawBox(9, self->field_0xf4 + layout->field_0x28, y, topY, layout->field_0x70, &boxText);
                    }

                    PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                    String12 label;

                    if (((ListItem *)(li))->isItem() == 0) {
                        if (((ListItem *)(li))->isShip() != 0) {
                            // Ship row: name + price + ship icon.
                            Ship_getIndex(G<void *>(li, 0xc));
                            PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                            String12 price;
                            Layout_formatCredits(&price, Ship_getPrice(G<void *>(li, 0xc)));
                            PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&price,
                                contentBase + layout->field_0x28 + self->field_0xf4, 1);
                            ((ImageFactory *)(*g_hw_globals))->drawShip(Ship_getIndex(G<void *>(li, 0xc)), self->field_0xf4 + layout->field_0x28 + rowGap, self->field_0x118 + y);
                            PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                        } else if (((ListItem *)(li))->isSlot() != 0) {
                            if (tab == 4 && i == items->size() - 1) {
                                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x5c)))->setPosition(self->field_0xf4 + layout->field_0x28 + topY / 2, layout->field_0x114 + y, 0x14);
                                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x5c)))->setVisible(true);
                                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x5c)))->draw();
                                String12 tmp;
                            }
                        } else if (((ListItem *)(li))->isBluePrint() != 0) {
                            BluePrint_getIndex(G<void *>(li, 0x8));
                            float rate = ((BluePrint *)(G<void *>(li, 0x8)))->getCompletionRate();
                            if (rate > 0.0f) {
                                PaintCanvas_DrawImage2D(canvas, self->field_0x78,
                                    layout->field_0x28 + contentBase + 2 + self->field_0xf4, 0, 0);
                                float dcw = (float)self->field_0xdc;
                                PaintCanvas_DrawRegion2D(canvas, self->field_0x7c, 0, 0,
                                    (int)(rate * dcw), self->field_0xe0, (float)(int)(rate * dcw),
                                    0, 0, 0,
                                    layout->field_0x28 + contentBase + 3 + self->field_0xf4);
                                String12 pct, sfx, sum;
                                AEString_add(&sum, &pct, &sfx);
                                PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&sum,
                                    contentBase + 2 + layout->field_0x28 + self->field_0xf4 +
                                        self->field_0xdc + layout->field_0x2c, 0);
                                PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                            }
                            int bpIdx = BluePrint_getIndex(G<void *>(li, 0x8));
                            int type = Item_getType(G<void *>(G<void *>(*g_hw_globals, 0x4), bpIdx));
                            ImageFactory_drawItem(*g_hw_globals, bpIdx, type,
                                layout->field_0x28 + rowGap + self->field_0xf4,
                                self->field_0x118 + y);
                        } else if (((ListItem *)(li))->isPendingProduct() != 0) {
                            int amt = G<int>(G<void *>(li, 0x18), 0x10);
                            String12 head;
                            if (amt < 2) {
                            } else {
                                String12 num, sfx;
                                AEString_add(&head, &num, &sfx);
                            }
                            String12 full;
                            AEString_add(&full, &head, (String12 *)((GameText *)(*g_hw_itemNameBase))->getText());
                            int pidx = G<int>(G<void *>(li, 0x18), 0x14);
                            int type = Item_getType(G<void *>(G<void *>(*g_hw_globals, 0x4), pidx));
                            ImageFactory_drawItem(*g_hw_globals, pidx, type,
                                rowGap + layout->field_0x28 + self->field_0xf4,
                                self->field_0x118 + y);
                        } else if (((ListItem *)(li))->isMoveToCargoButton() != 0) {
                            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x18)))->setPosition(self->field_0xf4 + layout->field_0x28, y, 0x11);
                            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x18)))->setVisible(true);
                            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x18)))->draw();
                        } else if (((ListItem *)(li))->isSellButton() != 0) {
                            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x14)))->setPosition(self->field_0xf4 + layout->field_0x28, y, 0x11);
                            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x14)))->setVisible(true);
                            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x14)))->draw();
                        } else {
                            String12 txt;
                            AEString_ctor_str(&txt, G<String12 *>(li, 0x1c), false);
                            ((Layout *)(layout))->drawBox(0, self->field_0xf4 + layout->field_0x28, (y + layout->field_0x70) - layout->field_0x1c, topY, layout->field_0x1c, &txt);
                        }
                    } else {
                        // Regular item row: name + amounts + price + icon.
                        Item_getIndex(G<void *>(li, 0x10));
                        PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                        if (self->field_0x11d == 0) {
                            String12 price;
                            Item_getSinglePrice(G<void *>(li, 0x10));
                            Layout_formatCredits(&price, Item_getSinglePrice(G<void *>(li, 0x10)));
                            PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&price,
                                contentBase + layout->field_0x28 + self->field_0xf4, 1);
                        }
                        int iidx = Item_getIndex(G<void *>(li, 0x10));
                        int itype = Item_getType(G<void *>(li, 0x10));
                        ImageFactory_drawItem(*g_hw_globals, iidx, itype,
                            layout->field_0x28 + rowGap + self->field_0xf4,
                            self->field_0x118 + y);
                    }

                    PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&label,
                        self->field_0xf4 + layout->field_0x28 + contentBase, 0);

                }

                // Scroll bar when the content overflows.
                if (scrollPx > 0 || startPx > 0) {
                    ((Layout *)(layout))->drawScrollBar(((*g_hw_screenHeight - layout->field_0x48) - layout->field_0x28) -
                            self->field_0xf4, layout->field_0x20 + layout->field_0xc, self->field_0xd8, startPx, scrollPx);
                }
            }

            String12 header;
            Layout_drawHeader(layout, &header);

            Array<void *> *tabs = (Array<void *> *)self->field_0x4;
            for (unsigned int i = 0; i < tabs->size(); i++)
                ((TouchButton *)(tabs->data()[i]))->draw();
        }

        if (self->field_0x58 == 1) {
            self->field_0x58 = 0;
            ((HangarWindow *)(self))->render();
            self->field_0x58 = 1;
            ListItemWindow_draw(self->field_0x18);
        }
    }

    // --- Footer + credits button (always drawn). ---
    layout = (Layout *)*g_hw_layout;
    ((Layout *)(layout))->drawFooter();
    void *btns = self->field_0x24;
    ((TouchButton *)(G<void *>(G<void *>(btns, 4), 0x2c)))->setVisible(true);
    ((TouchButton *)(G<void *>(G<void *>(btns, 4), 0x2c)))->setAlwaysPressed(g_hw_optionFlags[0x4e] == 0);
    {
        String12 credits;
        Layout_formatCredits(&credits, Status_getCredits());
        ((TouchButton *)(G<void *>(G<void *>(btns, 4), 0x2c)))->setText(&credits);
    }
    ((TouchButton *)(G<void *>(G<void *>(btns, 4), 0x2c)))->draw();

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
                ((TouchButton *)(b))->setVisible(vis);
                ((TouchButton *)(b))->setPosition2(layout->field_0x28 + G<int>(self->field_0x20, 0), layout->field_0x8);
                ((TouchButton *)(b))->draw();
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
                ((TouchButton *)(b))->setVisible(true);
                String12 t;
                ((TouchButton *)(b))->setText(&t);
                ((TouchButton *)(b))->setPosition(*g_hw_screenWidth / 2, 0, 0x14);
                ((TouchButton *)(b))->draw();
            }
        } else {
            // List-mode: per-index labelled buttons with icons.
            for (unsigned int i = 0; i <= 4; i++) {
                void *b = G<void *>(G<void *>(btns, 4), i * 4 + 0x30);
                ((TouchButton *)(b))->setVisible(true);
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
                ((TouchButton *)(b))->setPosition(x, yy, 'D');
                ((TouchButton *)(b))->replaceTextKeepSize(&label);
                ((TouchButton *)(b))->setSplitText(&split);
                ((TouchButton *)(b))->draw();
                PaintCanvas_DrawImage2D(canvas, G<int>(self->field_0x30, i * 4), x,
                                        yy - layout->field_0x2c, 0x11);
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


extern "C" {
void ListItemWindow_OnTouchEnd(void *win, int touch);
int ChoiceWindow_OnTouchEnd(void *win, int touch);
void ChoiceWindow_setChoices(void *win, String12 *msg, void *a, bool flag, void *b, void *c, void *d, int e, int f);

int HangarList_getCurrentLength(void *list);
void HangarList_setCurrentTab(void *list, bool flag);
void HangarList_setCurrentItemIndex(void *list, int idx);
void *HangarList_getCurrentItemAt(void *list, int n);
void HangarList_initShopTab(void *list, void *items, void *ships);
void HangarList_initShipTab(void *list, void *ship);


void *Status_getSystem();
void *Status_getStanding();
int Status_getCredits();
void Status_setCredits(void *globals);
void Status_changeCredits(void *globals);
void Status_setShip(void *ship);

int Station_getIndex(void *station);
void *Station_getShips(void *station);
void *Station_getItems(void *station);

int Ship_getIndex(void *ship);
int Ship_getPrice(void *ship);
int Ship_getRace(void *ship);
void Ship_setRace(void *ship, int race);
void Ship_adjustPrice(void *ship);
void Ship_refreshValue(void *ship);
void *Ship_getCargo(void *ship);
void *Ship_getMods(void *ship);
void *Ship_makeShip(int idx);
void Ship_addCargo(void *ship, void *item);
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


void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);


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
void ArrayReleaseClasses_ItemPtr(void *arr);
void *Array_ItemPtr_dtor(void *arr);
void ArrayRemove_ShipPtr(void *ship, void *arr);
int __aeabi_idiv(int a, int b);
void operator_delete(void *p);

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

void HangarWindow::OnTouchEnd(int touch, int coord) {
    HangarWindow *self = this;
    Globals *globals = (Globals *)*g_hw_globals;
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

        Layout *layout = (Layout *)*g_hw_layout;
        int handled = ((Layout *)(layout))->OnTouchEnd(touch, coord);
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
            for (unsigned int i = 0; i < tabs->size(); i++) {
                if (((TouchButton *)(tabs->data()[i]))->OnTouchEnd(touch) != 0) {
                    ((HangarWindow *)(self))->setSellMode();
                    ((HangarWindow *)(self))->setSellMode();
                    ((HangarWindow *)(self))->setSellMode();
                    self->field_0x68 = 0;
                    HangarList_setCurrentTab(self->field_0x14, i != 0);
                    if (i == 2)
                        ((HangarWindow *)(self))->refreshCargoAvailabilityForBlueprints();
                    ((HangarWindow *)(self))->refreshCurrentContentHeight();
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
                    if (((HangarWindow *)(self))->currentItemIsHighlighted() != 0 &&
                        self->field_0xd2 != 0) {
                        ((HangarWindow *)(self))->setSellMode();
                        ((HangarWindow *)(self))->setSellMode();
                    }
                }
            }
            if (self->field_0xd2 != 0) {
                self->field_0xd2 = 0;
                return;
            }

            // "Auto-complete blueprint" button.
            if (((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x5c)))->OnTouchEnd(touch) != 0) {
                ((BluePrint *)(self->field_0x80))->getAutoCompletionPrice();
                String12 line, priceStr, fmt, msg;
                Layout_formatCredits(&priceStr, 0);
                Status_replaceHash(&msg, globals, &line, &priceStr);
                ChoiceWindow_setMsg(self->field_0x20, &msg, true);
                self->field_0xb1 = 1;
                self->field_0x3c = 1;
            }

            if (((HangarWindow *)(self))->currentItemIsHighlighted() != 0) {
                Array<void *> *btns = (Array<void *> *)self->field_0x24;
                for (unsigned int i = 0; i < btns->size(); i++) {
                    if (((TouchButton *)(btns->data()[i]))->OnTouchEnd(touch) != 0) {
                        // Dispatch on the slot index (original used a jump table).
                        if ((i & 0x7fffffff) < 0xc)
                            return;
                    }
                }
            }

            if (((Layout *)(layout))->helpPressed() != 0) {
                String12 help;
                if (self->field_0x58 == 1) {
                    ((Layout *)(layout))->initHelpWindow(&help);
                } else {
                    unsigned int t = HangarList_getCurrentTab(self->field_0x14);
                    if (t <= 4) {
                        ((Layout *)(layout))->initHelpWindow(&help);
                    }
                }
            }

            // "Buy credits" footer button.
            if (((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x2c)))->OnTouchEnd(touch) != 0) {
                g_hw_optionFlags[0x4e] = 1;
                ((RecordHandler *)(*g_hw_recordHandler))->saveOptions();
                ((HangarWindow *)(self))->showCreditsBuyWindow();
            }
            return;
        }

        if (self->field_0x58 == 1) {
            ((Layout *)(layout))->resetWindowDimensions();
            self->field_0x58 = 0;
            return;
        }

        unsigned int tab = HangarList_getCurrentTab(self->field_0x14);
        if (tab == 4) {
            ((HangarWindow *)(self))->setSellMode();
            self->field_0x68 = 0;
            HangarList_setCurrentTab(self->field_0x14, true);
            ((HangarWindow *)(self))->refreshCargoAvailabilityForBlueprints();
            ((HangarWindow *)(self))->refreshCurrentContentHeight();
            self->field_0xb4 = 0;
            self->field_0xbc = 0;
            HangarList_setCurrentItemIndex(self->field_0x14, -1);
            return;
        }
        if (HangarList_getCurrentTab(self->field_0x14) == 3) {
            HangarList_setCurrentTab(self->field_0x14, false);
            ((HangarWindow *)(self))->refreshCargoAvailabilityForBlueprints();
            ((HangarWindow *)(self))->refreshCurrentContentHeight();
        } else if (((HangarWindow *)(self))->readyToClose() != 0) {
            ((HangarWindow *)(self))->setSellMode();
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
            int price = ((BluePrint *)(self->field_0x80))->getAutoCompletionPrice();
            if (Status_getCredits() < price) {
                String12 line, priceStr, fmt, msg, suffix, combined;
                Layout_formatCredits(&priceStr, Status_getCredits());
                Status_replaceHash(&msg, globals, &line, &priceStr);
                ((GameText *)(*g_hw_notEnoughTextId))->getText();
                AEString_add(&combined, &suffix, &suffix);
                AEString_addAssign(&msg, &combined);
                ChoiceWindow_setMsg(self->field_0x20, &msg, true);
                self->field_0xaf = 1;
            } else {
                self->field_0x3c = 0;
                if (self->field_0x80 != 0) {
                    if (((BluePrint *)(self->field_0x80))->isEmpty() != 0) {
                        G<int>(self->field_0x80, 0x10) = Station_getIndex(Status_getStation());
                        ((Station *)(Status_getStation()))->getName();
                        // String12 nameOut; assign into bp+0x14 (skipped: name handled inline)
                    }
                    ((BluePrint *)(self->field_0x80))->complete();
                    ((HangarWindow *)(self))->highlightItem(HangarList_getCurrentItemAt(self->field_0x14, 1));
                    self->field_0x88 = 1;
                    ((HangarWindow *)(self))->setSellMode();
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
            ((HangarWindow *)(self))->demountItem(self->field_0x2c, -1);
            self->field_0xe4 = self->field_0xb4;
            ((HangarWindow *)(self))->mountItem(self->field_0x28);
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
            ((RecordHandler *)(*g_hw_recordHandler))->saveOptions();
            ((HangarWindow *)(self))->showCreditsBuyWindow();
        }
    } else if (self->field_0xae == 0) {
        if (self->field_0xb0 != 0) {
            // Free-credits action buttons.
            int r = ChoiceWindow_OnTouchEnd(self->field_0x20, touch);
            if (r == 0) {
                for (int i = 0x12; i != 0x17; i++)
                    ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), i * 4)))->setVisible(false);
                self->field_0xb0 = 0;
                ((HangarWindow *)(self))->showCreditsBuyWindow();
            }
            void *appData = AppManager_GetApplicationData();
            void *rh = *g_hw_recordHandler;
            for (unsigned int i = 0; i != 5; i++) {
                if (((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), i * 4 + 0x48)))->OnTouchEnd(touch) != 0) {
                    switch (i) {
                    case 0:
                        ((RecordHandler *)(rh))->recordStoreWrite(0);
                        ((RecordHandler *)(rh))->recordStoreWritePreview(0);
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
                ((HangarWindow *)(self))->setSellMode();
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
                    Layout_formatCredits(&priceStr, Status_getCredits());
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    ChoiceWindow_set(self->field_0x20, 0);
                    self->field_0x3c = 1;
                }
                self->field_0x11e = 0;
            }
            ((HangarWindow *)(self))->refreshCurrentContentHeight();
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
                ((ListItem *)(self->field_0x68))->getPrice();
                Status_changeCredits(globals);
                ((Station *)(Status_getStation()))->removeShip();
                HangarList_initShopTab(self->field_0x14, self->field_0x10,
                                       Station_getShips(Status_getStation()));
                ((HangarWindow *)(self))->refreshCurrentContentHeight();
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
            if (((Station *)((void *)(uintptr_t)idx))->hasShip(Ship_getIndex(Status_getShip())) == 0) {
                int price = Ship_getPrice(G<void *>(self->field_0x68, 0xc));
                if (Status_getCredits() < price) {
                    String12 line, priceStr, fmt, msg, suffix, combined;
                    Ship_getPrice(G<void *>(self->field_0x68, 0xc));
                    Layout_formatCredits(&priceStr, Status_getCredits());
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    ((GameText *)(*g_hw_sellShipTextId))->getText();
                    AEString_add(&combined, &suffix, &suffix);
                    AEString_addAssign(&msg, &combined);
                    ChoiceWindow_setMsg(self->field_0x20, &msg, true);
                    self->field_0x3c = 1;
                    self->field_0xaf = 1;
                    self->field_0x90 = 0;
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
                if (((TouchButton *)(G<void *>(G<void *>(btns, 4), i * 4 + 0x30)))->OnTouchEnd(touch) != 0) {
                    switch (i) {
                    case 0: NFC_iap_buy_credits_300_000(); break;
                    case 1: NFC_iap_buy_credits_1_000_000(); break;
                    case 2: NFC_iap_buy_credits_3_000_000(); break;
                    case 3: NFC_iap_buy_credits_10_000_000(); break;
                    default: NFC_iap_buy_credits_100_000(); break;
                    }
                }
            }
            if (((TouchButton *)(G<void *>(G<void *>(btns, 4), 0x44)))->OnTouchEnd(touch) != 0) {
                bool show = true;
                if (g_hw_optionFlags[0x4a] && g_hw_optionFlags[0x49] &&
                    g_hw_optionFlags[0x4d] && g_hw_optionFlags[0x4c])
                    show = (g_hw_optionFlags[0x4b] == 0);
                void *appData = AppManager_GetApplicationData();
                if (show || G<uint8_t>(appData, 0x15) == 0) {
                    for (int i = 0xc; i != 0x11; i++)
                        ((TouchButton *)(G<void *>(G<void *>(btns, 4), i * 4)))->setVisible(false);
                    ((HangarWindow *)(self))->showFreeCreditsWindow();
                }
            }
            return;
        }
        self->field_0xae = 0;
        self->field_0x3c = 0;
        for (int i = 0xc; i != 0x11; i++)
            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), i * 4)))->setVisible(false);
        ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x44)))->setVisible(false);
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
                    for (unsigned int i = 0; i < tabs->size(); i++)
                        ((TouchButton *)(tabs->data()[i]))->resetTouch();
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
float VectorSignedToFloat(int v, int mode);
void ListItemWindow_update(void *win, int delta);
}

void HangarWindow::update(int delta) {
    HangarWindow *self = this;
    if (self->field_0xc == 0)
        return;
    self->field_0x8 = delta;

    if (self->field_0x58 == 1) {
        ListItemWindow_update(self->field_0x18, delta);
        return;
    }

    unsigned int tab = HangarList_getCurrentTab(self->field_0x14);
    Array<void *> *buttons = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < buttons->size(); i++) {
        bool pressed = true;
        if (i != tab && !(tab == 3 && i == 0))
            pressed = (i == 2 && tab == 4);
        ((TouchButton *)(buttons->data()[i]))->setAlwaysPressed(pressed);
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
        if (((TouchButton *)(btnUp))->isTouched() != 0 || ((TouchButton *)(btnDown))->isTouched() != 0) {
            int t6c = self->field_0x6c + delta;
            int t70 = self->field_0x70 + delta;
            self->field_0x6c = t6c;
            self->field_0x70 = t70;
            int threshold = (t6c > 0x5dc) ? 0x1e : 200;
            if (t70 > threshold && (self->field_0x70 = 0,
                                    self->field_0x88 != 0 || self->field_0x89 != 0)) {
                if (((TouchButton *)(btnDown))->isTouched() != 0 && ((TouchButton *)(btnDown))->isVisible() != 0) {
                    int n = (self->field_0x6c > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        ((HangarWindow *)(self))->transaction(true);
                } else if (((TouchButton *)(btnUp))->isTouched() != 0 &&
                           ((TouchButton *)(btnUp))->isVisible() != 0) {
                    int n = (self->field_0x6c > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        ((HangarWindow *)(self))->transaction(false);
                }
            }
        }
    }
}

// ---- highlightItem_14bbec.cpp ----
// FModSound singleton (pc-rel -> holder; *holder is the FModSound object).
__attribute__((visibility("hidden"))) extern void **g_hw_sound;

int HangarWindow::highlightItem(void *item) {
    HangarWindow *self = this;
    if (item != 0 && ((ListItem *)(item))->isSelectable() != 0) {
        FModSound_play(*g_hw_sound, 0x7c, 0, 0, 0);
        unsigned flag = 0;
        if (self->field_0x68 != item) {
            flag = ((ListItem *)(item))->isTextButton() ^ 1;
        }
        self->field_0x68 = item;
        self->field_0xd2 = flag;
        if (((ListItem *)(item))->isShip() != 0) {
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
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern void **g_hw_sound;

void HangarWindow::demountItem(void *item, int slot) {
    HangarWindow *self = this;
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
    for (unsigned int i = 0; i < cargo->size(); i++) {
        void *cur = cargo->data()[i];
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

    // refreshCurrentContentHeight() returns void (stores into this+0xd4); the target
    // reuses a stale float register as the FModSound pitch argument.
    ((HangarWindow *)(self))->refreshCurrentContentHeight();
    float h = 0.0f;
    self->field_0xb4 = self->field_0xe4;
    FModSound_play(*g_hw_sound, 0x60, 0, 0, h);
}

// ---- OnTouchBegin_14b740.cpp ----


extern "C" {
void ChoiceWindow_OnTouchBegin(void *win, int touch);
void ListItemWindow_OnTouchBegin(void *win, int touch, int coord);
int __aeabi_idiv(int a, int b);
void *HangarList_getCurrentItem(void *list);
int HangarList_getCurrentLength(void *list);
unsigned int HangarList_getCurrentItemIndex(void *list);
void HangarList_setCurrentItemIndex(void *list, int idx);
int Item_getIndex(void *item);
int Item_getBlueprintAmount(void *item);
void *Item_extractItems(void *arr, bool flag);
void Ship_setCargo(void *ship, void *arr);
int Station_getIndex(void *station);
int BluePrint_getStationIndex(void *bp);
void AEString_ctor_empty(String12 *self);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;
__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern int *g_hw_bpTextId;

void HangarWindow::OnTouchBegin(int touch, int coord) {
    HangarWindow *self = this;
    self->field_0x6c = 0;
    self->field_0x70 = 0;
    unsigned char handled = (unsigned char)((Layout *)(*g_hw_layout))->OnTouchBegin(touch);

    if (self->field_0x3c != 0) {
        if (self->field_0xae != 0) {
            for (int i = 0xc; i != 0x11; i++)
                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), i * 4)))->OnTouchBegin(touch);
            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x44)))->OnTouchBegin(touch);
        } else if (self->field_0xb0 != 0) {
            for (int i = 0x12; i != 0x17; i++)
                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), i * 4)))->OnTouchBegin(touch);
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

    Layout *layout = (Layout *)*g_hw_layout;
    unsigned char skip = 1;
    if (layout->field_0xc < coord && coord < *g_hw_screenWidth - layout->field_0x10) {
        int row = __aeabi_idiv(
            ((coord - layout->field_0xc) - layout->field_0x20) - self->field_0x10c -
                self->field_0xb4,
            layout->field_0x70 + self->field_0x10c);
        if (HangarList_getCurrentLength(self->field_0x14) > row) {
            HangarList_setCurrentItemIndex(self->field_0x14, row);
            ((HangarWindow *)(self))->highlightItem(HangarList_getCurrentItem(self->field_0x14));
            skip = 0;
            if (self->field_0x11d != 0) {
                void *ci = HangarList_getCurrentItem(self->field_0x14);
                if (((ListItem *)(ci))->isShip() != 0 &&
                    HangarList_getCurrentTab(self->field_0x14) == 1) {
                    Ship_setCargo(Status_getShip(), Item_extractItems(self->field_0x10, true));
                    ((Station *)(Status_getStation()))->setItems(Item_extractItems(self->field_0x10, false), false);
                }
            }
        }
    }

    if (HangarList_getCurrentTab(self->field_0x14) == 4 && self->field_0x88 != 0 &&
        !(!skip && self->field_0x68 == self->field_0x84) && self->field_0x94 > 0 &&
        self->field_0x3c == 0 && ((BluePrint *)(self->field_0x80))->isEmpty() == 0) {
        if (BluePrint_getStationIndex(self->field_0x80) != Station_getIndex(Status_getStation())) {
            Globals *globals = (Globals *)*g_hw_globals;
            int bpIdx = Item_getIndex(G<void *>(self->field_0x84, 0x10));
            bool localBp = (bpIdx == 0xd1) ||
                           (Item_getIndex(G<void *>(self->field_0x84, 0x10)) == 0xcc);
            self->field_0x11e = localBp;

            String12 msg;
            AEString_ctor_empty(&msg);
            String12 line;

            if (self->field_0x11e == 0) {
                String12 copy, sname, fmt, result;
                AEString_ctor_str(&copy, &msg, false);
                ((BluePrint *)(self->field_0x80))->getStationName();
                Status_replaceHash(&result, globals, &copy, &sname, &fmt);

                String12 copy2, priceStr, fmt2, result2;
                AEString_ctor_str(&copy2, &msg, false);
                Layout_formatCredits(&priceStr,
                                     Item_getBlueprintAmount(G<void *>(self->field_0x84, 0x10)));
                Status_replaceHash(&result2, globals, &copy2, &priceStr, &fmt2);
            }
            bool flag = (self->field_0x11e == 0);
            ChoiceWindow_setMsg(self->field_0x20, &msg, flag);
            self->field_0x3c = 1;
            self->field_0xac = 1;
            self->field_0xd1 = 1;
            return;
        }
    }

    Array<void *> *tabs = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < tabs->size(); i++)
        handled |= (unsigned char)((TouchButton *)(tabs->data()[i]))->OnTouchBegin(touch);

    Array<void *> *buttons = F<Array<void *> *>(self, 0x24);
    for (unsigned int i = 0; i < buttons->size(); i++) {
        void *btn = buttons->data()[i];
        if (btn != 0)
            ((TouchButton *)(btn))->OnTouchBegin(touch);
    }

    if (self->field_0xf8 != 0 && HangarList_getCurrentTab(self->field_0x14) == 1) {
        int idx = self->field_0xfc;
        if (idx >= 0 &&
            !(!handled && (unsigned int)idx == HangarList_getCurrentItemIndex(self->field_0x14))) {
            self->field_0xf8 = 0;
            ((HangarWindow *)(self))->autoEquipSecondaryWeapons(idx);
        }
    }
}

// ---- showCreditsBuyWindow_14e118.cpp ----

extern "C" {
void *ApplicationManager_GetApplicationData();
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
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

void HangarWindow::showCreditsBuyWindow() {
    HangarWindow *self = this;
    void *appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x4c) = 0;
    appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x3d) = 1;

    void *win = self->field_0x20;
    String12 a, b, yes, no;

    if (self->field_0x11f == 0) {
        void *body = ((GameText *)(*g_hw_buyTextId))->getText();
        ChoiceWindow_set(win, &a, &b, false, &yes, &no, body, -1, -1);
    } else {
        void *body = ((GameText *)(*g_hw_buyTextId2))->getText();
        ChoiceWindow_set(win, &a, &b, false, &yes, &no, body, -1, -1);

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
int HangarWindow::getCurrentTab() {
    HangarWindow *self = this;
    return HangarList_getCurrentTab(self->field_0x14);
}

// ---- refreshCargoAvailabilityForBlueprints_14900c.cpp ----
// Status singleton (pc-rel -> holder; *holder is the Status object).
__attribute__((visibility("hidden"))) extern void **g_hw_status2;

void HangarWindow::refreshCargoAvailabilityForBlueprints() {
    HangarWindow *self = this;
    void *items = HangarList_getItems(self->field_0x14);
    Array<void *> *arr = *(Array<void *> **)((char *)G<void *>(items, 0x4) + 0x8);
    if (arr == 0) return;
    for (uint32_t i = 0; i < arr->size(); i++) {
        void *it = arr->data()[i];
        F<uint8_t>((HangarWindow *)it, 0x45) = 0;
        if (it != 0 && ((ListItem *)(it))->isBluePrint() != 0) {
            void *bp = G<void *>(it, 0x8);
            void *cargo = Ship_getCargo(Status_getShip(*g_hw_status2));
            Array<int> *ingr = (Array<int> *)BluePrint_getIngredientList(bp);
            if (cargo != 0) {
                void *base = G<void *>(bp, 0x0);
                for (uint32_t j = 0; j < ingr->size(); j++) {
                    int *amts = *(int **)((char *)base + 0x4);
                    if (amts[j] > 0) {
                        Array<void *> *carr = (Array<void *> *)cargo;
                        for (uint32_t k = 0; k < carr->size(); k++) {
                            if (Item_getIndex(carr->data()[k]) == ingr->data()[j]) {
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


extern "C" {
unsigned int HangarList_getCurrentItemIndex(void *list);
void *HangarList_getCurrentItem(void *list);
void HangarList_initShipTab(void *list, void *ship);
void HangarList_initShopTab(void *list, void *items, void *ships);
void HangarList_initBlueprintTab(void *list, void *bps);
void HangarList_setCurrentTab(void *list, bool flag);
int Item_getType(void *item);
int Item_getAmount(void *item);
int Item_getIndex(void *item);
int Item_getStationAmount(void *item);
int Item_getBlueprintAmount(void *item);
void *Item_makeItem(void *item);
void *Item_extractItems(void *arr, bool flag);
void *Item_mixItems(void *a, void *b);
int Status_getCredits();
void *Status_getSystem();
void *Status_getBluePrints(void *globals);
void Status_addPendingProduct(void *globals, void *bp);
void Ship_setCargo(void *ship, void *arr);
void *Ship_getCargo(void *ship);
void Ship_addCargo(void *ship, void *item);
void Ship_adjustPrice(void *ship);
int Ship_hasEquipment(void *ship, int index);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
int Station_getIndex(void *station);
int SolarSystem_getRoutes(void *system);
int BluePrint_getIndex(void *bp);
int BluePrint_getStationIndex(void *bp);
int BluePrint_getQuantity(void *bp);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void ArrayReleaseClasses_ItemPtr(void *arr);
void *Array_ItemPtr_dtor(void *arr);
void ArrayAdd_ItemPtr(void *item, void *arr);
void operator_delete(void *p);
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern void **g_hw_itemFlags;
__attribute__((visibility("hidden"))) extern int *g_hw_sellTextId1;
__attribute__((visibility("hidden"))) extern int *g_hw_sellTextId2;
__attribute__((visibility("hidden"))) extern int *g_hw_routesTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_bpStations;

void HangarWindow::setSellMode() {
    HangarWindow *self = this;
    ListItem *item = (ListItem *)self->field_0x68;

    if (item == 0 ||
        ((ListItem *)(item))->isShip() != 0 || ((ListItem *)(item))->isSlot() != 0 ||
        ((ListItem *)(item))->isTextButton() != 0 || ((ListItem *)(item))->isSelectable() == 0 ||
        ((ListItem *)(item))->isBluePrint() != 0) {
        self->field_0x88 = 0;
        return;
    }

    self->field_0x88 = 1;

    int tab = HangarList_getCurrentTab(self->field_0x14);
    if (tab == 1) {
        if (self->field_0x88 == 0) {
            if (((ListItem *)(item))->isItem() != 0 && Item_getType(item->field_0x10) != 4) {
                void *flags = *g_hw_itemFlags;
                if (G<uint8_t>(flags, 0x1e) == 0) {
                    ((GameText *)(*g_hw_sellTextId1))->getText();
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
                ((GameText *)(*g_hw_sellTextId2))->getText();
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
        ((Station *)(Status_getStation()))->setItems(Item_extractItems(self->field_0x10, false), false);
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
        if (ci != 0 && ((ListItem *)(ci))->isShip() != 0)
            Ship_adjustPrice(G<void *>(self->field_0x68, 0xc));
        ((HangarWindow *)(self))->refreshCurrentContentHeight();
        return;
    }

    if (HangarList_getCurrentTab(self->field_0x14) != 4)
        return;

    if (self->field_0x88 == 0) {
        if (self->field_0x84 != 0 && self->field_0x80 != 0) {
            void *bpItem = G<void *>(self->field_0x84, 0x10);
            ((BluePrint *)(self->field_0x80))->addItem(bpItem, Item_getBlueprintAmount(bpItem), Station_getIndex(Status_getStation()));
        }

        uint8_t completedFlag = 0;
        if (((BluePrint *)(self->field_0x80))->isCompleted() != 0) {
            Globals *globals = (Globals *)*g_hw_globals;
            if (BluePrint_getStationIndex(self->field_0x80) == Station_getIndex(Status_getStation())) {
                String12 line, copy, name, fmt, result;
                AEString_ctor_str(&copy, &line, false);
                Status_replaceHash(&result, globals, &copy, &name, &fmt);
                ChoiceWindow_set(self->field_0x20, 0);

                int *stations = g_hw_bpStations;
                int idx = stations[BluePrint_getIndex(self->field_0x80)];
                BluePrint_getQuantity(self->field_0x80);
                void *made = Item_makeItem((void *)(uintptr_t)idx);
                Ship_addCargo(Status_getShip(), made);
                ArrayAdd_ItemPtr(made, self->field_0x10);
                HangarList_setCurrentTab(self->field_0x14, true);
                ((HangarWindow *)(self))->refreshCurrentContentHeight();
            } else {
                String12 line, copy, name, fmt, result, line2, sname, fmt2;
                AEString_ctor_str(&copy, &line, false);
                Status_replaceHash(&result, globals, &copy, &name, &fmt);

                AEString_ctor_str(&line2, &line, false);
                ((BluePrint *)(self->field_0x80))->getStationName();
                String12 result2;
                Status_replaceHash(&result2, globals, &line2, &sname, &fmt2);

                ChoiceWindow_set(self->field_0x20, 0);
                Status_addPendingProduct(globals, self->field_0x80);
                HangarList_setCurrentTab(self->field_0x14, true);
                ((HangarWindow *)(self))->refreshCargoAvailabilityForBlueprints();
            }
            ((BluePrint *)(self->field_0x80))->reset();
            completedFlag = 1;
        }

        Globals *globals = (Globals *)*g_hw_globals;
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
            for (unsigned int i = 0; i < items2->size(); i++) {
                void *li = items2->data()[i];
                if (li != 0 && ((ListItem *)(li))->isItem() != 0 &&
                    Item_getIndex(G<void *>(li, 0x10)) == BluePrint_getIndex(self->field_0x80)) {
                    if (Ship_hasEquipment(Status_getShip(), Item_getIndex(G<void *>(li, 0x10))) != 0) {
                        self->field_0xfc = i;
                        self->field_0xf8 = 1;
                        ((HangarWindow *)(self))->autoEquipSecondaryWeapons(i);
                        self->field_0xf8 = 0;
                        break;
                    }
                }
            }
        }
        return;
    }

    self->field_0x94 = 0;
    if (((BluePrint *)(self->field_0x80))->isEmpty() != 0 && Item_getAmount(item->field_0x10) > 0) {
        int idx = BluePrint_getIndex(self->field_0x80);
        bool flag;
        void *text;
        if (idx == 0xd2 || BluePrint_getIndex(self->field_0x80) == 0xdf) {
            if (SolarSystem_getRoutes(Status_getSystem()) != 0) {
                text = ((GameText *)(*g_hw_sellTextId2))->getText();
                flag = true;
            } else {
                self->field_0x130 = 1;
                text = ((GameText *)(*g_hw_routesTextId))->getText();
                flag = false;
            }
        } else {
            text = ((GameText *)(*g_hw_sellTextId2))->getText();
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


extern "C" {
void Ship_adjustPrice(void *ship);
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
int Status_getCredits();
int Status_getCurrentCampaignMission();
int Ship_getMaxPassengers(void *ship);
int Ship_getPrice(void *ship);
int Ship_getIndex(void *ship);
void *Ship_getFirstEquipmentOfSort(void *ship, int sort);
void Ship_setCargo(void *ship, void *arr);
void *Ship_getCargo(void *ship);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);
void ArrayReleaseClasses_ItemPtr(void *arr);
void *Array_ItemPtr_dtor(void *arr);
void operator_delete(void *p);
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
    ((GameText *)(textId))->getText();
    ChoiceWindow_set(self->field_0x20, 0);
    self->field_0x3c = 1;
}

void HangarWindow::selectItem(void *item) {
    HangarWindow *self = this;
    ListItem *li = (ListItem *)item;
    self->field_0x68 = item;
    if (item != 0 && ((ListItem *)(item))->isShip() != 0)
        Ship_adjustPrice(G<void *>(self->field_0x68, 0xc));

    int tab = HangarList_getCurrentTab(self->field_0x14);

    if (tab == 2) {
        if (((ListItem *)(item))->isSelectable() != 0 && ((ListItem *)(item))->isPendingProduct() == 0) {
            void *bp = li->field_0x8;
            self->field_0x80 = bp;
            HangarList_fillIngredientsList(self->field_0x14, bp != 0);
            HangarList_setCurrentTab(self->field_0x14, true);
            ((HangarWindow *)(self))->refreshCurrentContentHeight();
            if (self->field_0x89 != 0)
                self->field_0x89 = 0;
        }
        return;
    }

    if (tab == 1) {
        if (((ListItem *)(item))->isSelectable() == 0)
            return;
        if (((ListItem *)(item))->isShip() == 0) {
            // Buying / selling a regular item.
            if (Item_isUnsaleable(li->field_0x10) != 0)
                return;

            uint8_t was = self->field_0x88;
            self->field_0x88 = (uint8_t)(was ^ 1);
            if (was == 0) {
                void *flags = *g_hw_itemFlags;
                if (G<uint8_t>(flags, 0x1d) == 0) {
                    ((GameText *)(*g_hw_sellMsgTextId1))->getText();
                    ChoiceWindow_set(self->field_0x20, 0);
                    G<uint8_t>(flags, 0x1d) = 1;
                    self->field_0x3c = 1;
                }
                self->field_0x8c = Item_getStationAmount(li->field_0x10);
                self->field_0xa0 = Item_getAmount(li->field_0x10);
                self->field_0x98 = Status_getCredits();
                self->field_0x9c = self->field_0xa8;
            } else {
                if (((ListItem *)(item))->isItem() != 0 && Item_getType(li->field_0x10) != 4) {
                    void *flags = *g_hw_itemFlags;
                    if (G<uint8_t>(flags, 0x1e) == 0) {
                        ((GameText *)(*g_hw_sellMsgTextId2))->getText();
                        ChoiceWindow_set(self->field_0x20, 0);
                        G<uint8_t>(flags, 0x1e) = 1;
                        self->field_0x3c = 1;
                    }
                }
                int li = ((ListItem *)(item))->getIndex();
                if (li > 0x83 && ((ListItem *)(item))->getIndex() < 0x9a) {
                    int base = G<int>(*g_hw_globals, 0xac);
                    *((uint8_t *)G<int>((void *)(uintptr_t)base, 4) + ((ListItem *)(item))->getIndex() - 0x84) = 1;
                }
                self->field_0xf8 = 1;
                self->field_0xfc = HangarList_getCurrentItemIndex(self->field_0x14);

                Ship_setCargo(Status_getShip(), Item_extractItems(self->field_0x10, true));
                ((Station *)(Status_getStation()))->setItems(Item_extractItems(self->field_0x10, false), false);
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
        int price = Ship_getPrice(li->field_0xc);
        Globals *globals = (Globals *)*g_hw_globals;
        int credits = Status_getCredits();
        int oldPrice = Ship_getPrice(Status_getShip());
        if (oldPrice + credits < price && self->field_0x11d == 0) {
            String12 line, priceStr, fmt, msg, suffix, combined;
            Layout_formatCredits(&priceStr,
                                 ((ListItem *)(item))->getPrice() - Status_getCredits() - Ship_getPrice(Status_getShip()),
                                 0);
            AEString_ctor_str(&line, &priceStr, false);
            Status_replaceHash(&msg, globals, &line, &priceStr, &fmt);
            ((GameText *)(*g_hw_notEnoughTextId))->getText();
            AEString_add(&combined, &suffix, &suffix);
            AEString_addAssign(&msg, &combined);
            ChoiceWindow_setMsg(self->field_0x20, &msg, true);
            self->field_0x3c = 1;
            self->field_0xaf = 1;
            return;
        }

        if (globals->field_0x34 < 1) {
            if (Status_getCurrentCampaignMission() == 0x4d &&
                Ship_getIndex(Status_getShip()) == 0x25) {
                showText(self, *g_hw_unsaleableTextId);
                return;
            }
            int a = Ship_getIndex(Status_getShip());
            int b = Ship_getIndex(li->field_0xc);
            if (a != b) {
                self->field_0x90 = 1;
                ((GameText *)(*g_hw_sellMsgTextId2))->getText();
                ChoiceWindow_setMsg(self->field_0x20,
                                    (String12 *)((GameText *)(*g_hw_sellMsgTextId2))->getText(), true);
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
    if (((ListItem *)(item))->isSelectable() == 0)
        return;

    self->field_0xbc = 0;
    self->field_0xe4 = self->field_0xb4;
    self->field_0xb4 = 0;

    void *flags0 = *g_hw_itemFlags;
    if (G<uint8_t>(flags0, 0x1f) == 0 && ((ListItem *)(item))->isSlot() != 0) {
        ((GameText *)(*g_hw_slotMsgTextId))->getText();
        ChoiceWindow_set(self->field_0x20, 0);
        G<uint8_t>(flags0, 0x1f) = 1;
        self->field_0x3c = 1;
    }

    if (((ListItem *)(item))->isSelectable() == 0)
        return;

    void *cur = HangarList_getCurrentItem(self->field_0x14);
    void *curItem = G<void *>(cur, 0x10);
    if (curItem != 0) {
        if (Item_isUnsaleable(curItem) != 0) {
            showText(self, *g_hw_unsaleableTextId);
            return;
        }
        if (Item_getSort(curItem) == 0x14) {
            Globals *globals = (Globals *)*g_hw_globals;
            int passengerCount = globals->field_0x34;
            int chosen = passengerCount;
            if (passengerCount > 0)
                chosen = li->field_0x3c;
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

    if (li->field_0x3c >= 0) {
        void *ci = HangarList_getCurrentItem(self->field_0x14);
        ((HangarWindow *)(self))->demountItem(curItem, G<int>(ci, 0x40));
        return;
    }

    // Mounting: check for an existing equipment of the same sort.
    Globals *globals = (Globals *)*g_hw_globals;
    int sort = Item_getSort(li->field_0x10);
    void *existing = Ship_getFirstEquipmentOfSort(Status_getShip(), sort);
    bool conflict = false;
    if (Item_getSort(li->field_0x10) == 0x15) {
        if (Ship_getIndex(Status_getShip()) != 0x2c &&
            existing != 0 && Ship_getIndex(Status_getShip()) != 0x31)
            conflict = true;
    } else if (existing != 0) {
        conflict = true;
    }

    if (conflict && Item_canBeInstalledMultipleTimes(li->field_0x10) == 0) {
        String12 name, copy, etext, fmt, result, etext2, fmt2, result2;
        AEString_ctor_str(&copy, &name, false);
        Item_getIndex(existing);
        Status_replaceHash(&result, globals, &copy, &etext, &fmt);
        Item_getIndex(li->field_0x10);
        Status_replaceHash(&result2, globals, &result, &etext2, &fmt2);
        ChoiceWindow_setMsg(self->field_0x20, &name, true);
        self->field_0x11c = 1;
        self->field_0x3c = 1;
        self->field_0x28 = li->field_0x10;
        self->field_0x2c = existing;
        return;
    }

    ((HangarWindow *)(self))->mountItem(li->field_0x10);
}

// ---- getRelativeScrollHeight_14b6ec.cpp ----
// Returns float in r0. a=this+0xd4 (denominator), b=this+0xd8.
// NEAR: clang pairs a->r2,b->r1 in the ldrd (target uses r1=a,r2=b), mirroring every
// subsequent op; register pairing is an allocator tie-break, not source-driven.
float HangarWindow::getRelativeScrollHeight() {
    HangarWindow *self = this;
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


extern "C" {
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
void Ship_changeLoad(void *ship, int delta);
void *Ship_getCargo(void *ship);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);
void ChoiceWindow_setText(void *win, void *text);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern int *g_hw_unsaleableTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_notEnoughTextId;

void HangarWindow::transaction(bool buy) {
    HangarWindow *self = this;
    unsigned int tab = HangarList_getCurrentTab(self->field_0x14);
    void *cur = G<void *>(self->field_0x68, 0x10);

    if (tab < 2) {
        if (Item_isUnsaleable(cur) != 0) {
            ((GameText *)(*g_hw_unsaleableTextId))->getText();
            ChoiceWindow_setText(self->field_0x20, 0);
            self->field_0x88 = 0;
            self->field_0x3c = 1;
            return;
        }

        unsigned int result = Item_transaction(cur, buy, self->field_0xa8, self->field_0x11d);
        unsigned int idx = Item_getIndex(cur);
        Globals *globals = (Globals *)*g_hw_globals;
        unsigned int *avail = globals->field_0x54;
        if (idx < avail[0])
            *((uint8_t *)avail[1] + Item_getIndex(cur)) = 1;

        if (result >= 0x80000000 && buy) {
            self->field_0xa8 = self->field_0xa8 + 1;
            Ship_changeLoad(Status_getShip(), 1);
            int li = ((ListItem *)(self->field_0x68))->getIndex();
            if (li >= 0x84 && ((ListItem *)(self->field_0x68))->getIndex() < 0x9a) {
                int base = globals->field_0xac;
                *((uint8_t *)G<int>((void *)(uintptr_t)base, 4) +
                  ((ListItem *)(self->field_0x68))->getIndex() - 0x84) = 1;
            }
        } else if (result == 0 && buy) {
            if (Status_getCredits() < Item_getSinglePrice(cur)) {
                if (self->field_0x11d != 0)
                    return;
                String12 line, priceStr, fmt, msg, suffix, combined;
                Layout_formatCredits(&priceStr, Item_getSinglePrice(cur), Status_getCredits());
                AEString_ctor_str(&line, &priceStr, false);
                Status_replaceHash(&msg, globals, &line, &priceStr, &fmt);
                ((GameText *)(*g_hw_notEnoughTextId))->getText();
                AEString_add(&combined, &suffix, &suffix);
                AEString_addAssign(&msg, &combined);
                ChoiceWindow_setMsg(self->field_0x20, &msg, true);
                self->field_0x3c = 1;
                self->field_0xaf = 1;
                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x20)))->resetTouch();
                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x24)))->resetTouch();
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
            int remaining = ((BluePrint *)(bp))->getRemainingAmount(Item_getIndex(cur));
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
            for (unsigned int i = 0; i < arr->size(); i++) {
                if (Item_getIndex(arr->data()[i]) == Item_getIndex(cur)) {
                    Item_setAmount(arr->data()[i], Item_getAmount(arr->data()[i]));
                    Item_setBlueprintAmount(arr->data()[i], Item_getBlueprintAmount(cur));
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
void Ship_removeCargo(void *ship, int index, int amount);
void Ship_setCargo(void *ship, void *arr);
void *Ship_getCargo(void *ship);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
void ArrayRemove_ItemPtr(void *item, void *arr);
void HangarList_initShipTab(void *list, void *ship);
void HangarList_initShopTab(void *list, void *items, void *ships);
void HangarList_setCurrentTab(void *list, bool flag);
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern void **g_hw_sound;

void HangarWindow::mountItem(void *item) {
    HangarWindow *self = this;
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
        for (unsigned int i = 0; i < cargo->size(); i++) {
            void *cur = cargo->data()[i];
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

    // refreshCurrentContentHeight() returns void (stores into this+0xd4); the target
    // reuses a stale float register as the FModSound pitch argument.
    ((HangarWindow *)(self))->refreshCurrentContentHeight();
    float h = 0.0f;
    self->field_0xb4 = self->field_0xe4;
    FModSound_play(*g_hw_sound, 0x62, 0, 0, h);
}

// ---- OnTouchMove_14be98.cpp ----
extern "C" {
void ListItemWindow_OnTouchMove(void *win, int touch);
void ChoiceWindow_OnTouchMove(void *win, int touch);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;

unsigned int HangarWindow::OnTouchMove(int touch, int coord) {
    HangarWindow *self = this;
    Layout *layout = (Layout *)*g_hw_layout;
    ((Layout *)(layout))->OnTouchMove(touch);

    if (self->field_0x3c != 0) {
        if (self->field_0xae != 0) {
            for (int i = 0xc; i != 0x11; i++)
                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), i * 4)))->OnTouchMove(touch);
            ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x44)))->OnTouchMove(touch);
        } else if (self->field_0xb0 != 0) {
            for (int i = 0x12; i != 0x17; i++)
                ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), i * 4)))->OnTouchMove(touch);
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
        int touched = ((TouchButton *)(btnUp))->isTouched();
        if (touched != 0)
            touched = 1;
        else
            touched = ((TouchButton *)(btnDown))->isTouched();

        int adist = coord - self->field_0xcc;
        if (adist < 0)
            adist = -adist;

        if (touched == 0 && adist > 5) {
            self->field_0x6c = 0;
            self->field_0x70 = 0;
            Array<void *> *buttons = F<Array<void *> *>(self, 0x24);
            for (unsigned int i = 0; i < buttons->size(); i++)
                ((TouchButton *)(buttons->data()[i]))->OnTouchMove(touch);
            ((HangarWindow *)(self))->setSellMode();
            self->field_0xd2 = 0;
            self->field_0x68 = 0;
            ((TouchButton *)(btnUp))->resetTouch();
            ((TouchButton *)(btnDown))->resetTouch();
        }
    }

    Array<void *> *tabs = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < tabs->size(); i++)
        ((TouchButton *)(tabs->data()[i]))->OnTouchMove(touch);
    return 0;
}

// ---- autoEquipSecondaryWeapons_14bc50.cpp ----

extern "C" {
void *HangarList_getCurrentItemAt(void *list, int idx);
int Item_getType(void *item);
int Item_getAmount(void *item);
int Item_getIndex(void *item);
void *Item_makeItem(void *item);
void Item_setAmount(void *item, int amount);
void Ship_setEquipment(void *ship, void *item);
void Ship_removeCargo(void *ship, int index, int amount);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_copy(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void HangarList_initShipTab(void *list, void *ship);
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern int *g_hw_equipTextId;
__attribute__((visibility("hidden"))) extern void **g_hw_globals;

void HangarWindow::autoEquipSecondaryWeapons(int row) {
    HangarWindow *self = this;
    ListItem *item = (ListItem *)HangarList_getCurrentItemAt(self->field_0x14, row);
    if (item == 0)
        return;
    void *itm = item->field_0x10;
    if (itm == 0 || Item_getType(itm) != 1)
        return;
    if (((ListItem *)(item))->isItem() == 0 || Item_getType(itm) != 1)
        return;
    if (Item_getAmount(itm) <= 0)
        return;

    void *ship = Status_getShip();
    int *equip = Ship_getEquipment(ship, 1);
    if (equip == 0)
        return;

    Array<void *> *arr = (Array<void *> *)equip;
    for (unsigned int i = 0; i < arr->size(); i++) {
        void *cur = arr->data()[i];
        if (cur == 0)
            continue;
        if (Item_getIndex(cur) != Item_getIndex(itm))
            continue;

        void *made = Item_makeItem(itm);

        Array<void *> *cargo = F<Array<void *> *>(self, 0x10);
        if (cargo != 0) {
            for (unsigned int j = 0; j < cargo->size(); j++) {
                if (Item_getIndex(cargo->data()[j]) == Item_getIndex(made))
                    Item_setAmount(cargo->data()[j], 0);
                cargo = F<Array<void *> *>(self, 0x10);
            }
        }

        Ship_setEquipment(HangarWindow_statusShip(), made);
        Ship_removeCargo(HangarWindow_statusShip(), Item_getIndex(made), Item_getAmount(itm));
        HangarList_initShipTab(self->field_0x14, HangarWindow_statusShip());

        String12 msg, msgCopy, name, fmt;
        String12 result;
        Status_replaceHash(&result, *g_hw_globals, &msgCopy, &name, &fmt);

        ChoiceWindow_set(self->field_0x20, &msg);
        self->field_0xad = 1;
        self->field_0x3c = 1;
        break;
    }
}

// ---- showFreeCreditsWindow_14e32c.cpp ----

// AbyssEngine::String - 12-byte value type.

extern "C" {
void *ApplicationManager_GetApplicationData();
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
void ChoiceWindow_setWidth(void *win, int w);
void ChoiceWindow_setHeight(void *win, int h);
}

__attribute__((visibility("hidden"))) extern int *g_hw_freeCreditsTextId;
__attribute__((visibility("hidden"))) extern void **g_hw_canvas;
__attribute__((visibility("hidden"))) extern void **g_hw_layout;

void HangarWindow::showFreeCreditsWindow() {
    HangarWindow *self = this;
    void *appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x4c) = 0;
    appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x3d) = 1;

    void *win = self->field_0x20;
    String12 title, title2, yes, no;
    void *body = ((GameText *)(*g_hw_freeCreditsTextId))->getText();
    ChoiceWindow_set(win, &title, &title2, false, &yes, &no, body, -1, -1);

    int rowH = ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x48)))->getHeight();
    ChoiceWindow_setHeight(self->field_0x20, rowH * 5);

    int maxW = 0;
    for (int i = 5; i != 0; i--) {
        ((GameText *)(*g_hw_freeCreditsTextId))->getText();
        int w = PaintCanvas_GetTextWidth((unsigned int)(uintptr_t)*g_hw_canvas, 0);
        if (maxW < w)
            maxW = w;
    }
    int btnW = ((TouchButton *)(0))->getWidth();
    Layout *layout = (Layout *)*g_hw_layout;
    ChoiceWindow_setWidth(self->field_0x20,
                          layout->field_0x2c + btnW + maxW + layout->field_0x28 * 4);

    self->field_0xb0 = 1;
    self->field_0x3c = 1;
    self->field_0xae = 0;
    self->field_0xaf = 0;
}

// ---- initialize_147e24.cpp ----


extern "C" {
int Station_getIndex(void *station);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
void Status_calcCargoPrices(void *status);
void *Status_getSystem();
void *Status_getBluePrints(void *status);
int Status_getCredits();
int Status_getCurrentCampaignMission();
int Status_inBlackMarketSystem();
int SolarSystem_getIndex(void *sys);
void *Ship_getCargo(void *ship);
void Ship_adjustPrice(void *ship);
int Ship_getCurrentLoad(void *ship);
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


// TouchButton constructor overloads (distinct argument arities in the target).
void TouchButton_ctor_text(void *btn, void *text, int a, int b, int c, char k);
void TouchButton_ctor_text2(void *btn, void *text, int a, int b, int c, int d, char k);
void TouchButton_ctor_img(void *btn, void *img, int a, int b, int c, int d, char k, char m);
void TouchButton_ctor_img2(void *btn, void *imgA, void *imgB, int a, int b, int c, char k);
void TouchButton_getPosition(void *btn, float *x, float *y);

void PaintCanvas_Image2DCreate(void *canvas, int id, unsigned int *out);
int PaintCanvas_GetImage2DWidth(void *canvas);
int PaintCanvas_GetImage2DHeight(void *canvas);

void ListItemWindow_ctor(void *win);
void ChoiceWindow_ctor(void *win);

void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);


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

void HangarWindow::initialize() {
    HangarWindow *self = this;
    Globals *status = (Globals *)*g_hw_globals;

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
    Layout *layout = (Layout *)*g_hw_layout;

    void *b0 = operator_new(200);
    TouchButton_ctor_text(b0, ((GameText *)(*g_hw_helpTextId))->getText(), 3,
                          scrW - Layout_getHelpButtonOffset(), 0, 0x12);
    G<void *>(G<void *>(self->field_0x4, 4), 8) = b0;

    void *b1 = operator_new(200);
    int w0 = ((TouchButton *)(b0))->getWidth();
    TouchButton_ctor_text(b1, ((GameText *)(*g_hw_helpTextId))->getText(), 3,
                          (scrW - Layout_getHelpButtonOffset() - w0) + layout->field_0x38, 0, 0x12);
    G<void *>(G<void *>(self->field_0x4, 4), 4) = b1;

    void *b2 = operator_new(200);
    int w0b = ((TouchButton *)(b0))->getWidth();
    int w1b = ((TouchButton *)(b1))->getWidth();
    TouchButton_ctor_text(b2, ((GameText *)(*g_hw_helpTextId))->getText(), 3,
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
    for (unsigned int i = 0; i < tabArr->size(); i++) {
        if (i < 10) {
            float x = 0, y = 0;
            TouchButton_getPosition(tabArr->data()[i], &x, &y);
            posX[i] = (int)x;
            TouchButton_getPosition(tabArr->data()[i], &x, &y);
            posY[i] = (int)y;
        }
    }

    *(unsigned int *)*g_hw_imageCountSlot = tabArr->size();
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
    TouchButton_ctor_text(e1, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 4) = e1;
    void *e2 = operator_new(200);
    TouchButton_ctor_text(e2, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, 0x11);
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
    TouchButton_ctor_text2(e5, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, self->field_0x110, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 0x14) = e5;
    void *e6 = operator_new(200);
    TouchButton_ctor_text2(e6, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, self->field_0x110, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 0x18) = e6;
    void *e7 = operator_new(200);
    TouchButton_ctor_text(e7, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, 0x11);
    G<void *>(G<void *>(self->field_0x24, 4), 0x1c) = e7;

    {
        String12 lbl;
        void *e8 = operator_new(200);
        TouchButton_ctor_img((void *)e8, &lbl, 8, 0, 0, layout->field_0x50, 0x11, 4);
        G<void *>(G<void *>(self->field_0x24, 4), 0x20) = e8;
    }
    {
        String12 lbl;
        void *e9 = operator_new(200);
        TouchButton_ctor_img((void *)e9, &lbl, 9, 0, 0, layout->field_0x50, 0x11, 4);
        G<void *>(G<void *>(self->field_0x24, 4), 0x24) = e9;
    }
    {
        void *e10 = operator_new(200);
        TouchButton_ctor_img((void *)e10, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0,
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
    }

    // Five collapsible "more" rows (indices 0x0c..0x10 in slot terms).
    uint8_t listMode = self->field_0x11f;
    int row = 0;
    for (int slot = 0xc; (unsigned int)(slot - 0xc) < 5; slot++) {
        String12 lbl;
        void *btn = operator_new(200);
        int visIdx;
        if (listMode == 0) {
            TouchButton_ctor_img(btn, &lbl, 0, 0, 0, layout->field_0x264, 0x11, 1);
            visIdx = row + 0xc;
        } else {
            TouchButton_ctor_text(btn, &lbl, 10, 0, 0, 1);
            visIdx = slot;
        }
        G<void *>(G<void *>(self->field_0x24, 4), slot * 4) = btn;
        ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), visIdx * 4)))->setVisible(false);
        row++;
    }
    {
        String12 lbl;
        void *btn = operator_new(200);
        if (listMode == 0)
            TouchButton_ctor_img(btn, &lbl, 0, 0, 0, layout->field_0x264, 0x11, 1);
        else
            TouchButton_ctor_text(btn, &lbl, 10, 0, 0, 1);
        G<void *>(G<void *>(self->field_0x24, 4), 0x44) = btn;
        ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x44)))->setVisible(false);
    }

    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233e, (unsigned int *)((char *)self + 0x34));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233f, (unsigned int *)((char *)self + 0x38));
    {
        String12 lbl;
        void *btn = operator_new(200);
        TouchButton_ctor_text(btn, &lbl, 7, 0, 0, 0x11);
        G<void *>(G<void *>(self->field_0x24, 4), 0x5c) = btn;
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
        ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), i * 4)))->setVisible(false);
    }

    self->field_0x120 = ((TouchButton *)(0))->getWidth();
    int h = ((TouchButton *)(G<void *>(G<void *>(self->field_0x24, 4), 0x30)))->getHeight();
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
        unsigned int n = cargo->size() + (equip ? equip[0] : 0);
        for (unsigned int i = 0; i < n; i++) {
            void *itemPtr;
            if (i < cargo->size())
                itemPtr = cargo->data()[i];
            else
                itemPtr = ((void **)equip[1])[i - cargo->size()];
            if (itemPtr != 0) {
                int price = Item_getSinglePrice(itemPtr);
                int idx = Item_getIndex(itemPtr);
                Globals *globals = (Globals *)*g_hw_globals;
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
    ((HangarWindow *)(self))->refreshCurrentContentHeight();

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
            ((GameText *)(*g_hw_helpTextId))->getText();
            ChoiceWindow_set(self->field_0x20, 0);
            introFlag[0x4e] = 1;
            ((RecordHandler *)(*g_hw_recordHandler))->saveOptions();
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

void HangarWindow::ctor() {
    HangarWindow *self = this;
    int *cnt = g_hw_openCounter;
    void *lay = *g_hw_layout;

    self->field_0x80 = 0;
    self->field_0x84 = 0;
    self->field_0x3c = 0;
    self->field_0xac = 0;
    // zero 4 words at 0x14..0x20 (a zeroed Vector slot) via a 16-byte NEON store
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1c = 0;
    self->field_0x20 = 0;
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
