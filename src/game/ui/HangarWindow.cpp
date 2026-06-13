#include "gof2/game/ui/HangarWindow.h"
#include <algorithm>
#include "gof2/game/world/SolarSystem.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/externs.h"
#include "gof2/game/ui/ChoiceWindow.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/core/HangarList.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/ui/ListItemWindow.h"
#include "gof2/game/mission/BluePrint.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/mission/RecordHandler.h"
#include "gof2/game/world/Station.h"
#include "gof2/game/core/Globals.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/ui/ListItem.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/engine/core/NFC.h"

// Status singleton holder (Status** at 0xe4c5c). Dropped-self Status_*() calls are
// method calls on this global instance.
__attribute__((visibility("hidden"))) extern Status **gStatus;

// The recovered code addressed the TouchButton arrays through the engine container's
// flattened {count, data} layout: G<void*>(G<void*>(arr,4), byteOff) read the element
// at (data + byteOff) with an implicit 4-byte pointer stride. With the members now a
// real Array<TouchButton*> (== std::vector), the slot offsets stay element-index*4, so
// this helper divides by 4 to index the vector - which also fixes the latent 64-bit
// stride bug (a raw +byteOff would assume 4-byte pointers).
static inline TouchButton *btnAt(Array<TouchButton*> *arr, int byteOff) {
    return (*arr)[byteOff >> 2];
}
static inline TouchButton *&btnSlot(Array<TouchButton*> *arr, int byteOff) {
    return (*arr)[byteOff >> 2];
}

// The decompiler dropped the String argument to several ChoiceWindow::set() calls
// (the dialog body text, built/fetched just above each call and discarded). The text
// itself is unrecoverable without the original key tables, so these pass an empty
// String to keep ChoiceWindow::set(String const&) resolving correctly.
static const String g_HangarWindow_emptyDialogText;
// NOTE: gof2/Item.h is intentionally not included - Item is only ever used opaquely
// here (via the Item_* free functions), and fwd.h already forward-declares it.
// NOTE: gof2/Layout.h, gof2/TouchButton.h and gof2/GameText.h are intentionally NOT
// included. Layout.h redefines the Layout struct and pulls in placement-new; both
// TouchButton.h and Station.h/RecordHandler.h redefine the shared RetStr/B/I/P helper
// structs unconditionally, which collide. Minimal local Layout/TouchButton/GameText
// declarations (matching these call sites) live in gof2/HangarWindow.h instead.

extern "C" int Station_getIndex(void *station);
// Status singleton accessors: some merged sections call these with the singleton
// threaded explicitly, others rely on the implicit singleton. Declared variadic so
// every call arity matches the single extern "C" ABI symbol.
// Decompiled call sites disagree on the trailing arg type/arity (String12*/void*/int,
// 4 vs 5 args); these are extern "C" ABI symbols, so declare them variadic once.
extern "C" void Status_replaceHash(...);
void Layout_formatCredits(...);
// Decompiled sections disagree on arity (1 vs 2 args) / return type; extern "C" ABI symbol.
// Own method forward-declared here; the definition (returns int) lives later in this file.
extern "C" void *HangarList_dtor(void *p);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" extern const char hw_ote_fmt1[], hw_ote_fmt2[], hw_ote_fmt3[], hw_ote_fmt4[];
// Last arg is void* in some sections, float in others; variadic covers both.
extern "C" extern const char hw_otb_fmt1[], hw_otb_fmt2[];
void *BluePrint_getIngredientList(void *bp);
extern "C" extern const char hw_bp_fmt1[], hw_bp_fmt2[], hw_bp_fmt3[];
extern "C" extern const char hw_sel_fmt1[], hw_sel_fmt2[], hw_sel_fmt3[];
extern "C" extern const char hw_tx_fmt[], hw_tx_suffix[];
extern "C" extern const char hw_equip_fmt[];
extern "C" extern const char hw_freeCredits_yes[];
extern "C" extern const char hw_freeCredits_no[];
extern "C" extern const char hw_freeCredits_icon[];

bool HangarWindow::isInSpecialMode() {
    if (this->specialMode != 0) return true;
    return this->dialogActive != 0;
}

// Globals singleton (pc-rel deref -> holder; *holder is the object), field +0x70 is the row height.
__attribute__((visibility("hidden"))) extern void **g_hw_globals;

void HangarWindow::refreshCurrentContentHeight() {
    Array<ListItem*> *items = ((HangarList *)this->hangarList)->getCurrentTabItems();
    if (items != 0) {
        int n = (int)items->size();
        int rowH = G<int>(*g_hw_globals, 0x70);
        this->currentContentHeight = this->rowSpacing * (n - 1) + n * rowH;
    }
}

bool HangarWindow::listMode() {
    HangarWindow *self = this;
    if (self->viewMode == 0 && self->specialMode == 0) {
        return self->dialogActive == 0;
    }
    return false;
}

// Status singleton (pc-rel deref -> holder; *holder is the Status object).
__attribute__((visibility("hidden"))) extern void **g_hw_status;

bool HangarWindow::readyToClose() {
    HangarWindow *self = this;
    int tab = ((HangarList *)(self->hangarList))->getCurrentTab();
    if (tab == 4 && self->buyMode != 0 && self->bluePrintBuyCount > 0 &&
        self->dialogActive == 0 && ((BluePrint *)(self->bluePrint))->isEmpty() == 0) {
        int si = ((BluePrint *)(self->bluePrint))->getStationIndex();
        void *st = ((Status *)(*g_hw_status))->getStation();
        if (si != Station_getIndex(st)) {
            return false;
        }
    }
    return self->dialogActive == 0;
}

// Tail-call thunk; arg is this->0x18 when this->0x58 == 1.

void HangarWindow::render3D() {
    if (this->viewMode == 1) {
        return this->render3D_thunk(this->listItemWindow);
    }
}

// render3D()'s tail-call thunk (resolved from the PLT veneer at 0x1ac4b8): in the
// embedded item-list view mode the hangar has no 3D content of its own, so it hands
// the 3D pass off to the item-list sub-window.
void HangarWindow::render3D_thunk(void *listItemWindow) {
    ((ListItemWindow *)listItemWindow)->render();
}

bool HangarWindow::currentItemIsHighlighted() {
    ListItem *item = (ListItem *)((HangarList *)(this->hangarList))->getCurrentItem();
    if (item == 0) return false;
    return item == this->selectedItem;
}

// HangarWindow::~HangarWindow() -> returns this. Tears down owned windows/lists/arrays.
HangarWindow *_ZN12HangarWindowD2Ev(HangarWindow *self)
{
    void *p;
    p = self->hangarList;
    if (p != 0) ::operator delete(HangarList_dtor(p));
    self->hangarList = 0;
    p = self->listItemWindow;
    if (p != 0) ::operator delete(((ListItemWindow *)(p))->dtor());
    self->listItemWindow = 0;
    p = self->choiceWindow;
    if (p != 0) ::operator delete(ChoiceWindow_dtor(p));
    self->choiceWindow = 0;
    p = self->dialog;
    if (p != 0) ::operator delete(ChoiceWindow_dtor(p));
    self->dialog = 0;
    if (self->tabButtons != 0) {
        for (TouchButton *b : *self->tabButtons) delete b;
        self->tabButtons->clear();
        delete self->tabButtons;
    }
    self->tabButtons = 0;
    if (self->buttons != 0) {
        for (TouchButton *b : *self->buttons) delete b;
        self->buttons->clear();
        delete self->buttons;
    }
    p = self->tabIcons;
    self->active = 0;
    self->buttons = 0;
    if (p != 0) ::operator delete[](p);
    self->tabIcons = 0;
    return self;
}

// Returns float in r0. If scroll range (this+0xb4) >= 1 returns a constant; otherwise
// returns -(float)range / (float)contentHeight(this+0xd4).
float HangarWindow::getRelativeScrollStartPos() {
    int range = this->scrollOffset;
    if (range > 0) {
        union { unsigned u; float f; } c; c.u = 0x4650a903u;
        return c.f;
    }
    return -(float)range / (float)this->currentContentHeight;
}

void HangarWindow::hideMessage() {
    this->dialogActive = 0;
}

extern "C" {

void Layout_drawHeader(void *layout, String12 *text);

int Item_canBeInstalledMultipleTimes(void *item);

void *Station_getItems(void *station);

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
    Layout *layout = (Layout *)*g_hw_layout;
    void *canvas = *g_hw_canvas;
    ((PaintCanvas *)canvas)->SetColor(0u);

    void *dlc = AppManager_GetApplicationModule(*g_hw_dlcModuleId);
    bool dlcShown = dlc != 0 && G<uint8_t>(dlc, 0x18) != 0;

    if (dlc == 0 || !dlcShown) {
        int tab2 = this->viewMode;
        if (tab2 == 0) {
            ((Layout *)(layout))->drawBG();
            unsigned int tab = ((HangarList *)(this->hangarList))->getCurrentTab();
            Array<ListItem*> *items = ((HangarList *)this->hangarList)->getCurrentTabItems();
            if (items != 0) {
                float startPos = ((HangarWindow *)(this))->getRelativeScrollStartPos();
                float visH = (float)this->visibleHeight;
                float scrollH = ((HangarWindow *)(this))->getRelativeScrollHeight();
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

                topY += (layout->field_0x28 + this->hintOffsetX) * -2;
                int baseY = layout->field_0x2cc;
                int colW = layout->field_0x4c;

                // Black-market scrolling hint strip.
                if (*g_hw_blackMarketHintFlag != 0 && *g_hw_introHintFlag == 0) {
                    int iw = ((PaintCanvas *)canvas)->GetImage2DWidth(0);
                    int ih = ((PaintCanvas *)canvas)->GetImage2DHeight(0);
                    int rows = __aeabi_idiv(*g_hw_screenHeight, 1);
                    int y = 0;
                    for (int r = 0; r <= rows; r++) {
                        ((PaintCanvas *)canvas)->DrawImage2D((unsigned)this->field_0xf0,
                            (layout->field_0x28 - iw) + this->hintOffsetX, y, (unsigned char)1);
                        int off = (scrollPx < 1) ? 0 : (layout->field_0x48 + layout->field_0x2c);
                        ((PaintCanvas *)canvas)->DrawImage2D((unsigned)this->field_0xf0,
                            this->hintOffsetX + layout->field_0x28 + topY + off, y, (unsigned char)0);
                        y += ih;
                    }
                }

                int contentBase = colW + baseY + rowGap;

                // Hide all action buttons before re-laying them out per row.
                Array<TouchButton*> *btnArr = this->buttons;
                for (int i = 0; i != 0x18; i++) {
                    if (this->dragging == 0) {
                        void *btn = btnAt(this->buttons, i * 4);
                        if (btn != 0)
                            ((TouchButton *)(btn))->setVisible(false);
                    }
                }

                int boxW = rowGap - 2;

                for (unsigned int i = 0; i < items->size(); i++) {
                    int y = (layout->field_0x70 + this->rowSpacing) * (int)i +
                            this->scrollOffset + layout->field_0x20 + layout->field_0xc;
                    if (y < 0 || y > *g_hw_screenHeight)
                        continue;

                    void *li = items->data()[i];
                    if (((ListItem *)(li))->isSelectable() == 0)
                        continue;

                    // Row background box (selected vs unselected, by tab).
                    if (this->selectedItem == li && ((ListItem *)(li))->isTextButton() == 0) {
                        String12 boxText;
                        if (tab == 0 && G<int>(li, 0x3c) >= 0) {
                            ((Layout *)(layout))->drawBox(10, this->hintOffsetX + layout->field_0x28, y, topY, layout->field_0x70, &boxText);
                        } else {
                            ((Layout *)(layout))->drawBox(4, this->hintOffsetX + layout->field_0x28, y, topY, layout->field_0x70, &boxText);
                        }
                    } else if (tab != 0 || G<int>(li, 0x3c) < 0) {
                        String12 boxText;
                        ((Layout *)(layout))->drawBox(3, this->hintOffsetX + layout->field_0x28, y, topY, layout->field_0x70, &boxText);
                    } else {
                        String12 boxText;
                        ((Layout *)(layout))->drawBox(9, this->hintOffsetX + layout->field_0x28, y, topY, layout->field_0x70, &boxText);
                    }

                    ((PaintCanvas *)canvas)->SetColor(0u);
                    String12 label;

                    if (((ListItem *)(li))->isItem() == 0) {
                        if (((ListItem *)(li))->isShip() != 0) {
                            // Ship row: name + price + ship icon.
                            ((Ship *)(G<void *>(li, 0xc)))->getIndex();
                            ((PaintCanvas *)canvas)->SetColor(0u);
                            String12 price;
                            Layout_formatCredits(&price, ((Ship *)(G<void *>(li, 0xc)))->getPrice());
                            ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)*g_hw_font, (void *)(uintptr_t)(int)(uintptr_t)&price,
                                contentBase + layout->field_0x28 + this->hintOffsetX, 0, (bool)1);
                            ((ImageFactory *)(*g_hw_globals))->drawShip(((Ship *)(G<void *>(li, 0xc)))->getIndex(), this->hintOffsetX + layout->field_0x28 + rowGap, this->iconOffsetY + y);
                            ((PaintCanvas *)canvas)->SetColor(0u);
                        } else if (((ListItem *)(li))->isSlot() != 0) {
                            if (tab == 4 && i == items->size() - 1) {
                                btnAt(this->buttons, 0x5c)->setPosition(this->hintOffsetX + layout->field_0x28 + topY / 2, layout->field_0x114 + y, 0x14);
                                btnAt(this->buttons, 0x5c)->setVisible(true);
                                btnAt(this->buttons, 0x5c)->draw();
                                String12 tmp;
                            }
                        } else if (((ListItem *)(li))->isBluePrint() != 0) {
                            ((BluePrint *)((BluePrint *)(G<void *>(li, 0x8))))->getIndex();
                            float rate = ((BluePrint *)(G<void *>(li, 0x8)))->getCompletionRate();
                            if (rate > 0.0f) {
                                ((PaintCanvas *)canvas)->DrawImage2D((unsigned)this->field_0x78,
                                    layout->field_0x28 + contentBase + 2 + this->hintOffsetX, 0, (unsigned char)0);
                                float dcw = (float)this->progressBarWidth;
                                ((PaintCanvas *)canvas)->DrawRegion2D((unsigned)this->field_0x7c, 0, 0,
                                    (int)(rate * dcw), this->progressBarHeight, (float)(int)(rate * dcw),
                                    0, 0, 0,
                                    layout->field_0x28 + contentBase + 3 + this->hintOffsetX);
                                String12 pct, sfx, sum;
                                AEString_add(&sum, &pct, &sfx);
                                ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)*g_hw_font, (void *)(uintptr_t)(int)(uintptr_t)&sum,
                                    contentBase + 2 + layout->field_0x28 + this->hintOffsetX +
                                        this->progressBarWidth + layout->field_0x2c, 0, (bool)0);
                                ((PaintCanvas *)canvas)->SetColor(0u);
                            }
                            int bpIdx = ((BluePrint *)((BluePrint *)(G<void *>(li, 0x8))))->getIndex();
                            int type = ((Item *)(G<void *>(G<void *>(*g_hw_globals, 0x4), bpIdx)))->getType();
                            ((ImageFactory *)(*g_hw_globals))->drawItem(bpIdx, type, layout->field_0x28 + rowGap + this->hintOffsetX);
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
                            int type = ((Item *)(G<void *>(G<void *>(*g_hw_globals, 0x4), pidx)))->getType();
                            ((ImageFactory *)(*g_hw_globals))->drawItem(pidx, type, rowGap + layout->field_0x28 + this->hintOffsetX);
                        } else if (((ListItem *)(li))->isMoveToCargoButton() != 0) {
                            btnAt(this->buttons, 0x18)->setPosition(this->hintOffsetX + layout->field_0x28, y, 0x11);
                            btnAt(this->buttons, 0x18)->setVisible(true);
                            btnAt(this->buttons, 0x18)->draw();
                        } else if (((ListItem *)(li))->isSellButton() != 0) {
                            btnAt(this->buttons, 0x14)->setPosition(this->hintOffsetX + layout->field_0x28, y, 0x11);
                            btnAt(this->buttons, 0x14)->setVisible(true);
                            btnAt(this->buttons, 0x14)->draw();
                        } else {
                            String12 txt;
                            AEString_ctor_str(&txt, G<String12 *>(li, 0x1c), false);
                            ((Layout *)(layout))->drawBox(0, this->hintOffsetX + layout->field_0x28, (y + layout->field_0x70) - layout->field_0x1c, topY, layout->field_0x1c, &txt);
                        }
                    } else {
                        // Regular item row: name + amounts + price + icon.
                        ((Item *)(G<void *>(li, 0x10)))->getIndex();
                        ((PaintCanvas *)canvas)->SetColor(0u);
                        if (this->upgradeMode == 0) {
                            String12 price;
                            ((Item *)(G<void *>(li, 0x10)))->getSinglePrice();
                            Layout_formatCredits(&price, ((Item *)(G<void *>(li, 0x10)))->getSinglePrice());
                            ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)*g_hw_font, (void *)(uintptr_t)(int)(uintptr_t)&price,
                                contentBase + layout->field_0x28 + this->hintOffsetX, 0, (bool)1);
                        }
                        int iidx = ((Item *)(G<void *>(li, 0x10)))->getIndex();
                        int itype = ((Item *)(G<void *>(li, 0x10)))->getType();
                        ((ImageFactory *)(*g_hw_globals))->drawItem(iidx, itype, layout->field_0x28 + rowGap + this->hintOffsetX);
                    }

                    ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)*g_hw_font, (void *)(uintptr_t)(int)(uintptr_t)&label,
                        this->hintOffsetX + layout->field_0x28 + contentBase, 0, (bool)0);

                }

                // Scroll bar when the content overflows.
                if (scrollPx > 0 || startPx > 0) {
                    ((Layout *)(layout))->drawScrollBar(((*g_hw_screenHeight - layout->field_0x48) - layout->field_0x28) -
                            this->hintOffsetX, layout->field_0x20 + layout->field_0xc, this->visibleHeight, startPx, scrollPx);
                }
            }

            String12 header;
            Layout_drawHeader(layout, &header);

            Array<TouchButton*> *tabs = this->tabButtons;
            for (unsigned int i = 0; i < tabs->size(); i++)
                ((TouchButton *)(tabs->data()[i]))->draw();
        }

        if (this->viewMode == 1) {
            this->viewMode = 0;
            ((HangarWindow *)(this))->render();
            this->viewMode = 1;
            ((ListItemWindow *)(this->listItemWindow))->draw();
        }
    }

    // --- Footer + credits button (always drawn). ---
    layout = (Layout *)*g_hw_layout;
    ((Layout *)(layout))->drawFooter();
    Array<TouchButton*> *btns = this->buttons;
    btnAt(btns, 0x2c)->setVisible(true);
    btnAt(btns, 0x2c)->setAlwaysPressed(g_hw_optionFlags[0x4e] == 0);
    {
        String12 credits;
        Layout_formatCredits(&credits, ((Status *)(*gStatus))->getCredits());
        btnAt(btns, 0x2c)->setText(&credits);
    }
    btnAt(btns, 0x2c)->draw();

    if (this->dialogActive == 0)
        return;

    ((ChoiceWindow *)(this->dialog))->draw();

    if (this->buyCreditsActive == 0) {
        if (this->freeCreditsActive != 0) {
            // Free-credits action button column.
            for (unsigned int i = 0; i < 5; i++) {
                void *b = btnAt(btns, i * 4 + 0x48);
                bool vis;
                if (i == 0 || (i == 1 && g_hw_optionFlags[0x49] != 0) ||
                    (i == 2 && g_hw_optionFlags[0x4a] != 0) ||
                    (i == 3 && g_hw_optionFlags[0x4b] != 0))
                    vis = false;
                else
                    vis = (i != 4 || g_hw_optionFlags[0x4c] == 0);
                ((TouchButton *)(b))->setVisible(vis);
                ((TouchButton *)(b))->setPosition2(layout->field_0x28 + G<int>(this->dialog, 0), layout->field_0x8);
                ((TouchButton *)(b))->draw();
            }
        }
    } else {
        // Buy-credits / IAP grid.
        void *appData = AppManager_GetApplicationData();
        (void)appData;
        if (this->listModeFlag == 0) {
            // Five fixed-label buttons.
            for (int slot = 0x30; slot <= 0x40; slot += 4) {
                void *b = btnAt(btns, slot);
                ((TouchButton *)(b))->setVisible(true);
                String12 t;
                ((TouchButton *)(b))->setText(&t);
                ((TouchButton *)(b))->setPosition(*g_hw_screenWidth / 2, 0, 0x14);
                ((TouchButton *)(b))->draw();
            }
        } else {
            // List-mode: per-index labelled buttons with icons.
            for (unsigned int i = 0; i <= 4; i++) {
                void *b = btnAt(btns, i * 4 + 0x30);
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
                int x = (this->gridSpacingX + this->buttonWidth) *
                            ((i + rowOff * -3) & 0xff) +
                        ((*g_hw_screenHeight / 2 - this->buttonWidth) - this->gridSpacingX);
                int yy = (int)((float)((layout->field_0x20 * -3) +
                            (*g_hw_screenWidth / 2 - this->gridButtonHeight / 2)) +
                            (float)this->gridSpacingY * -0.5f +
                            (float)((int)rowOff * (this->gridSpacingY + this->gridButtonHeight)));
                ((TouchButton *)(b))->setPosition(x, yy, 'D');
                ((TouchButton *)(b))->replaceTextKeepSize(&label);
                ((TouchButton *)(b))->setSplitText(&split);
                ((TouchButton *)(b))->draw();
                ((PaintCanvas *)canvas)->DrawImage2D((unsigned)G<int>(this->tabIcons, i * 4), x,
                                        yy - layout->field_0x2c, (unsigned char)0x11);
            }
        }

        if (this->listModeFlag == 0) {
            int h17 = layout->field_0x30;
            int h34 = layout->field_0x34;
            int th = ((PaintCanvas *)canvas)->GetTextHeight(0);
            ((ChoiceWindow *)(this->dialog))->setHeight((h34 + h17) * 6 + th * 2);
        }
    }
}

extern "C" {
void ChoiceWindow_setChoices(void *win, String12 *msg, void *a, bool flag, void *b, void *c, void *d, int e, int f);

void Status_changeCredits(void *globals);

int Station_getIndex(void *station);
void *Station_getItems(void *station);

void *Ship_dtor(void *ship);

void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);

void *AppManager_GetApplicationData();
void *AppManager_GetApplicationModule(unsigned int id);
int __aeabi_idiv(int a, int b);

// Mission-offer subroutine (corrupted in decompile); kept as an extern helper.
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
    self->holdTime = 0;
    self->repeatTimer = 0;
    self->dragging = 0;
    if (self->suppressTouchEnd != 0) {
        self->suppressTouchEnd = 0;
        return;
    }

    if (self->dialogActive == 0) {
        // --- No modal dialog active: normal list / button handling. ---
        if (self->viewMode == 1)
            ((ListItemWindow *)(self->listItemWindow))->OnTouchEnd(touch, coord);

        Layout *layout = (Layout *)*g_hw_layout;
        int handled = ((Layout *)(layout))->OnTouchEnd(touch, coord);
        if (handled == 0) {
            // Inertial scroll bookkeeping.
            int delta = self->scrollDelta;
            int newScroll = self->scrollOffset + delta;
            float vel = (float)delta;
            int absd = delta < 0 ? -delta : delta;
            self->velocity = (absd > 3) ? (int)vel : 0;
            self->damping = 0;
            self->scrollOffset = newScroll;
            self->scrollOffsetBackup = newScroll;

            Array<TouchButton*> *tabs = self->tabButtons;
            for (unsigned int i = 0; i < tabs->size(); i++) {
                if (((TouchButton *)(tabs->data()[i]))->OnTouchEnd(touch) != 0) {
                    ((HangarWindow *)(self))->setSellMode();
                    ((HangarWindow *)(self))->setSellMode();
                    ((HangarWindow *)(self))->setSellMode();
                    self->selectedItem = 0;
                    ((HangarList *)(self->hangarList))->setCurrentTab(i != 0);
                    if (i == 2)
                        ((HangarWindow *)(self))->refreshCargoAvailabilityForBlueprints();
                    ((HangarWindow *)(self))->refreshCurrentContentHeight();
                    self->scrollOffset = 0;
                    self->scrollOffsetBackup = 0;
                    ((HangarList *)(self->hangarList))->setCurrentItemIndex(-1);
                }
            }

            if (layout->field_0xc < coord) {
                int row = __aeabi_idiv(
                    (((coord - layout->field_0xc) - layout->field_0x20) - self->rowSpacing) -
                        self->scrollOffset,
                    layout->field_0x70 + self->rowSpacing);
                if (row < ((HangarList *)(self->hangarList))->getCurrentLength()) {
                    ((HangarList *)(self->hangarList))->setCurrentItemIndex(row);
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
            if (btnAt(self->buttons, 0x5c)->OnTouchEnd(touch) != 0) {
                ((BluePrint *)(self->bluePrint))->getAutoCompletionPrice();
                String12 line, priceStr, fmt, msg;
                Layout_formatCredits(&priceStr, 0);
                Status_replaceHash(&msg, globals, &line, &priceStr);
                ((ChoiceWindow *)(self->dialog))->setMsg(*(String *)&msg, true);
                self->autoCompletePending = 1;
                self->dialogActive = 1;
            }

            if (((HangarWindow *)(self))->currentItemIsHighlighted() != 0) {
                Array<TouchButton*> *btns = self->buttons;
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
                if (self->viewMode == 1) {
                    ((Layout *)(layout))->initHelpWindow(&help);
                } else {
                    unsigned int t = ((HangarList *)(self->hangarList))->getCurrentTab();
                    if (t <= 4) {
                        ((Layout *)(layout))->initHelpWindow(&help);
                    }
                }
            }

            // "Buy credits" footer button.
            if (btnAt(self->buttons, 0x2c)->OnTouchEnd(touch) != 0) {
                g_hw_optionFlags[0x4e] = 1;
                ((RecordHandler *)(*g_hw_recordHandler))->saveOptions();
                ((HangarWindow *)(self))->showCreditsBuyWindow();
            }
            return;
        }

        if (self->viewMode == 1) {
            ((Layout *)(layout))->resetWindowDimensions();
            self->viewMode = 0;
            return;
        }

        unsigned int tab = ((HangarList *)(self->hangarList))->getCurrentTab();
        if (tab == 4) {
            ((HangarWindow *)(self))->setSellMode();
            self->selectedItem = 0;
            ((HangarList *)(self->hangarList))->setCurrentTab(true);
            ((HangarWindow *)(self))->refreshCargoAvailabilityForBlueprints();
            ((HangarWindow *)(self))->refreshCurrentContentHeight();
            self->scrollOffset = 0;
            self->scrollOffsetBackup = 0;
            ((HangarList *)(self->hangarList))->setCurrentItemIndex(-1);
            return;
        }
        if (((HangarList *)(self->hangarList))->getCurrentTab() == 3) {
            ((HangarList *)(self->hangarList))->setCurrentTab(false);
            ((HangarWindow *)(self))->refreshCargoAvailabilityForBlueprints();
            ((HangarWindow *)(self))->refreshCurrentContentHeight();
        } else if (((HangarWindow *)(self))->readyToClose() != 0) {
            ((HangarWindow *)(self))->setSellMode();
            self->selectedItem = 0;
            ((HangarList *)(self->hangarList))->setCurrentItemIndex(-1);
        }
        return;
    }

    // --- A modal dialog is active (self+0x3c set). Route to the right handler. ---
    if (self->autoCompletePending != 0) {
        // Blueprint auto-completion confirmation.
        int r = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
        if (r == 1) {
            self->dialogActive = 0;
        } else if (r == 0) {
            int price = ((BluePrint *)(self->bluePrint))->getAutoCompletionPrice();
            if (((Status *)(*gStatus))->getCredits() < price) {
                String12 line, priceStr, fmt, msg, suffix, combined;
                Layout_formatCredits(&priceStr, ((Status *)(*gStatus))->getCredits());
                Status_replaceHash(&msg, globals, &line, &priceStr);
                ((GameText *)(*g_hw_notEnoughTextId))->getText();
                AEString_add(&combined, &suffix, &suffix);
                AEString_addAssign(&msg, &combined);
                ((ChoiceWindow *)(self->dialog))->setMsg(*(String *)&msg, true);
                self->notEnoughCredits = 1;
            } else {
                self->dialogActive = 0;
                if (self->bluePrint != 0) {
                    if (((BluePrint *)(self->bluePrint))->isEmpty() != 0) {
                        G<int>(self->bluePrint, 0x10) = Station_getIndex(((Status *)(*gStatus))->getStation());
                        ((Station *)(((Status *)(*gStatus))->getStation()))->getName();
                        // String12 nameOut; assign into bp+0x14 (skipped: name handled inline)
                    }
                    ((BluePrint *)(self->bluePrint))->complete();
                    ((HangarWindow *)(self))->highlightItem(((HangarList *)(self->hangarList))->getCurrentItemAt(1));
                    self->buyMode = 1;
                    ((HangarWindow *)(self))->setSellMode();
                    Status_changeCredits(globals);
                }
            }
        }
        self->autoCompletePending = 0;
        return;
    }

    if (self->replaceEquipPending != 0) {
        // Equipment-replacement confirmation.
        int r = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
        if (r == 1) {
            self->dialogActive = 0;
            self->replaceEquipPending = 0;
            self->scrollOffset = self->savedScrollOffset;
        } else if (r == 0 && self->pendingMountItem != 0 && self->pendingDemountItem != 0) {
            ((HangarWindow *)(self))->demountItem(self->pendingDemountItem, -1);
            self->savedScrollOffset = self->scrollOffset;
            ((HangarWindow *)(self))->mountItem(self->pendingMountItem);
            self->dialogActive = 0;
            self->replaceEquipPending = 0;
        }
        // fallthrough to buy/sell confirmation below
    } else if (self->notEnoughCredits != 0) {
        // "Not enough credits" -> offer to buy credits.
        int r = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
        if (r == 1) {
            self->notEnoughCredits = 0;
            self->dialogActive = 0;
        } else if (r == 0) {
            g_hw_optionFlags[0x4e] = 1;
            ((RecordHandler *)(*g_hw_recordHandler))->saveOptions();
            ((HangarWindow *)(self))->showCreditsBuyWindow();
        }
    } else if (self->buyCreditsActive == 0) {
        if (self->freeCreditsActive != 0) {
            // Free-credits action buttons.
            int r = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
            if (r == 0) {
                for (int i = 0x12; i != 0x17; i++)
                    btnAt(self->buttons, i * 4)->setVisible(false);
                self->freeCreditsActive = 0;
                ((HangarWindow *)(self))->showCreditsBuyWindow();
            }
            void *appData = AppManager_GetApplicationData();
            void *rh = *g_hw_recordHandler;
            for (unsigned int i = 0; i != 5; i++) {
                if (btnAt(self->buttons, i * 4 + 0x48)->OnTouchEnd(touch) != 0) {
                    switch (i) {
                    case 0:
                        ((RecordHandler *)(rh))->recordStoreWrite(0);
                        ((RecordHandler *)(rh))->recordStoreWritePreview_int(0);
                        break;
                    case 1:
                        G<uint8_t>(appData, 0xa0) = 1;
                        NFC().free_credits_likeGOF2OnFacebook();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x49] = 1;
                        break;
                    case 2:
                        G<uint8_t>(appData, 0xa1) = 1;
                        NFC().free_credits_likeFishlabsOnFacebook();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x4a] = 1;
                        break;
                    case 3:
                        G<uint8_t>(appData, 0xa2) = 1;
                        NFC().free_credits_subscribeToYoutubeChannel();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x4b] = 1;
                        break;
                    case 4:
                        G<uint8_t>(appData, 0xa3) = 1;
                        NFC().free_credits_followOnTwitter();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x4c] = 1;
                        break;
                    case 5:
                        G<uint8_t>(appData, 0xd) = 1;
                        NFC().free_credits_rateGame();
                        Status_changeCredits(globals);
                        g_hw_optionFlags[0x4d] = 1;
                        break;
                    }
                }
            }
            return;
        }

        if (self->bluePrintPurchasePending != 0) {
            // Blueprint cargo-purchase confirmation.
            int r = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
            int cost = ((Item *)(G<void *>(self->bluePrintItem, 0x10)))->getBlueprintAmount() * 200;
            bool revert = true;
            if (r == 0 && ((Status *)(*gStatus))->getCredits() >= cost && self->localBluePrint == 0) {
                Status_changeCredits(globals);
                ((HangarWindow *)(self))->setSellMode();
                self->selectedItem = 0;
                ((HangarList *)(self->hangarList))->setCurrentItemIndex(-1);
                self->localBluePrint = 0;
                self->bluePrintPurchasePending = 0;
                revert = false;
            } else if (r == 1 && ((Status *)(*gStatus))->getCredits() >= cost && self->localBluePrint == 0) {
                revert = false;
            }
            if (revert) {
                ((Item *)(G<void *>(self->bluePrintItem, 0x10)))->setStationAmount(self->savedStationAmount);
                ((Item *)(G<void *>(self->bluePrintItem, 0x10)))->setAmount(self->savedAmount);
                ((Item *)(G<void *>(self->bluePrintItem, 0x10)))->setBlueprintAmount(self->savedBlueprintAmount);
                ((Status *)(globals))->setCredits(self->savedCredits);
                self->savedStationAmount = 0;
                self->bluePrintBuyCount = 0;
                self->selectedItem = 0;
                self->savedAmount = 0;
                self->savedBlueprintAmount = 0;
                self->currentLoad = self->savedLoad;
                ((HangarList *)(self->hangarList))->setCurrentItemIndex(-1);
                self->dialogActive = 0;
                self->bluePrintPurchasePending = 0;
                self->buyMode = 0;
                if (((Status *)(*gStatus))->getCredits() < cost && self->localBluePrint == 0) {
                    String12 line, priceStr, fmt, msg;
                    Layout_formatCredits(&priceStr, ((Status *)(*gStatus))->getCredits());
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
                    self->dialogActive = 1;
                }
                self->localBluePrint = 0;
            }
            ((HangarWindow *)(self))->refreshCurrentContentHeight();
            return;
        }

        if (self->sellShipPending != 0) {
            // Ship sale confirmation.
            int r = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
            if (r == 1) {
                self->sellShipPending = 0;
                self->dialogActive = 0;
            } else if (r == 0) {
                self->sellShipPending = 0;
                self->dialogActive = 0;
                ((ListItem *)(self->selectedItem))->getPrice();
                Status_changeCredits(globals);
                ((Station *)(((Status *)(*gStatus))->getStation()))->removeShip((Ship *)((Status *)(*gStatus))->getShip());
                ((HangarList *)(self->hangarList))->initShopTab((Array<Item *> *)(self->itemList), Station_getShips(((Status *)(*gStatus))->getStation()));
                ((HangarWindow *)(self))->refreshCurrentContentHeight();
            }
            return;
        }

        // Ship purchase confirmation (the large branch). The mission-offer tail
        // is delegated to a helper because its decompile is unrecoverable.
        int r = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
        bool special = globals->field_0x114 == 3 && self->upgradeMode == 0;
        if (special && self->field_0x91 != 0 && r == 1) {
            int idx = globals->field_0x14c;
            ((Ship *)(((Status *)(*gStatus))->getShip()))->getIndex();
            if (((Station *)((void *)(uintptr_t)idx))->hasShip(((Ship *)(((Status *)(*gStatus))->getShip()))->getIndex()) == 0) {
                int price = ((Ship *)(G<void *>(self->selectedItem, 0xc)))->getPrice();
                if (((Status *)(*gStatus))->getCredits() < price) {
                    String12 line, priceStr, fmt, msg, suffix, combined;
                    ((Ship *)(G<void *>(self->selectedItem, 0xc)))->getPrice();
                    Layout_formatCredits(&priceStr, ((Status *)(*gStatus))->getCredits());
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    ((GameText *)(*g_hw_sellShipTextId))->getText();
                    AEString_add(&combined, &suffix, &suffix);
                    AEString_addAssign(&msg, &combined);
                    ((ChoiceWindow *)(self->dialog))->setMsg(*(String *)&msg, true);
                    self->dialogActive = 1;
                    self->notEnoughCredits = 1;
                    self->shipSwapPending = 0;
                    return;
                }
            }
        }
        // The rest of the ship-swap and mission-offer flow lives in the helper.
        ((HangarWindow *)(self))->buildMissionOffer(touch, coord);
        return;
    } else {
        // self+0xae set: free-credits intro window.
        int r = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
        if (r != 0) {
            // Buy-credits grid.
            Array<TouchButton*> *btns = self->buttons;
            for (unsigned int i = 0; i < 5; i++) {
                if (btnAt(btns, i * 4 + 0x30)->OnTouchEnd(touch) != 0) {
                    switch (i) {
                    case 0: NFC().iap_buy_credits_300_000(); break;
                    case 1: NFC().iap_buy_credits_1_000_000(); break;
                    case 2: NFC().iap_buy_credits_3_000_000(); break;
                    case 3: NFC().iap_buy_credits_10_000_000(); break;
                    default: NFC().iap_buy_credits_100_000(); break;
                    }
                }
            }
            if (btnAt(btns, 0x44)->OnTouchEnd(touch) != 0) {
                bool show = true;
                if (g_hw_optionFlags[0x4a] && g_hw_optionFlags[0x49] &&
                    g_hw_optionFlags[0x4d] && g_hw_optionFlags[0x4c])
                    show = (g_hw_optionFlags[0x4b] == 0);
                void *appData = AppManager_GetApplicationData();
                if (show || G<uint8_t>(appData, 0x15) == 0) {
                    for (int i = 0xc; i != 0x11; i++)
                        btnAt(btns, i * 4)->setVisible(false);
                    ((HangarWindow *)(self))->showFreeCreditsWindow();
                }
            }
            return;
        }
        self->buyCreditsActive = 0;
        self->dialogActive = 0;
        for (int i = 0xc; i != 0x11; i++)
            btnAt(self->buttons, i * 4)->setVisible(false);
        btnAt(self->buttons, 0x44)->setVisible(false);
        void *appData = AppManager_GetApplicationData();
        G<uint8_t>(appData, 0x40) = 0;
        void *mod = AppManager_GetApplicationModule(*g_hw_modStationId);
        if (mod != 0 && G<uint8_t>(mod, 0x18) != 0)
            G<uint8_t>(mod, 0x18) = 0;
        return;
    }

    // --- Shared buy/sell confirmation tail (reached from 0x11c branch). ---
    uint8_t buying = self->buyMode;
    int r2 = ((ChoiceWindow *)(self->dialog))->OnTouchEnd(touch, coord);
    if (buying != 0) {
        if (r2 == 1) {
            self->buyMode = 0;
            self->dialogActive = 0;
        } else if (r2 == 0) {
            if (self->field_0x130 == 0) {
                self->dialogActive = 0;
                if (self->autoEquipped == 0) {
                    self->buyMode = 1;
                } else {
                    Array<TouchButton*> *tabs = self->tabButtons;
                    for (unsigned int i = 0; i < tabs->size(); i++)
                        ((TouchButton *)(tabs->data()[i]))->resetTouch();
                }
                return;
            }
            self->dialogActive = 0;
            self->field_0x130 = 0;
            self->buyMode = 0;
        } else {
            return;
        }
        self->selectedItem = 0;
        ((HangarList *)(self->hangarList))->setCurrentItemIndex(-1);
        return;
    }
    if (r2 != 0)
        return;
    self->dialogActive = 0;
    void *mod = AppManager_GetApplicationModule(*g_hw_dlcModuleId);
    if (G<uint8_t>(mod, 0x18) != 0)
        G<uint8_t>(mod, 0x18) = 0;
}

extern "C" {
float VectorSignedToFloat(int v, int mode);
}

void HangarWindow::update(int delta) {
    if (this->active == 0)
        return;
    this->lastDelta = delta;

    if (this->viewMode == 1) {
        ((ListItemWindow *)(this->listItemWindow))->update(delta);
        return;
    }

    unsigned int tab = ((HangarList *)(this->hangarList))->getCurrentTab();
    Array<TouchButton*> *buttons = this->tabButtons;
    for (unsigned int i = 0; i < buttons->size(); i++) {
        bool pressed = true;
        if (i != tab && !(tab == 3 && i == 0))
            pressed = (i == 2 && tab == 4);
        ((TouchButton *)(buttons->data()[i]))->setAlwaysPressed(pressed);
    }

    if (this->dragging == 0) {
        float v = this->damping * this->velocity;
        this->velocity = v;
        float mag = v > 0.0f ? v : -v;
        if (mag >= 1.0f) {
            float pos = VectorSignedToFloat(this->scrollOffset, 0);
            this->scrollOffset = (int)(v + pos);
        }
    }

    if (this->scrollOffset > 0) {
        float f = VectorSignedToFloat(-this->scrollOffset, 0);
        this->damping = 1.0f;
        this->velocity = f * 0.5f;
    }

    if (((HangarList *)this->hangarList)->getCurrentTabItems() != 0) {
        int diff = this->visibleHeight - this->currentContentHeight;
        if (diff < 0) {
            if (this->scrollOffset < diff) {
                float f = VectorSignedToFloat(diff - this->scrollOffset, 0);
                this->damping = 1.0f;
                this->velocity = f * 0.5f;
            }
        } else {
            this->velocity = 0;
            this->scrollOffset = 0;
        }
    }

    if (this->buyMode != 0) {
        void *btnUp = btnAt(this->buttons, 0x20);
        void *btnDown = btnAt(this->buttons, 0x24);
        if (((TouchButton *)(btnUp))->isTouched() != 0 || ((TouchButton *)(btnDown))->isTouched() != 0) {
            int t6c = this->holdTime + delta;
            int t70 = this->repeatTimer + delta;
            this->holdTime = t6c;
            this->repeatTimer = t70;
            int threshold = (t6c > 0x5dc) ? 0x1e : 200;
            if (t70 > threshold && (this->repeatTimer = 0,
                                    this->buyMode != 0 || this->specialMode != 0)) {
                if (((TouchButton *)(btnDown))->isTouched() != 0 && ((TouchButton *)(btnDown))->isVisible() != 0) {
                    int n = (this->holdTime > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        ((HangarWindow *)(this))->transaction(true);
                } else if (((TouchButton *)(btnUp))->isTouched() != 0 &&
                           ((TouchButton *)(btnUp))->isVisible() != 0) {
                    int n = (this->holdTime > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        ((HangarWindow *)(this))->transaction(false);
                }
            }
        }
    }
}

// FModSound singleton (pc-rel -> holder; *holder is the FModSound object).
__attribute__((visibility("hidden"))) extern void **g_hw_sound;

int HangarWindow::highlightItem(void *item) {
    if (item != 0 && ((ListItem *)(item))->isSelectable() != 0) {
        ((FModSound *)(*g_hw_sound))->play(0x7c, 0, 0, 0);
        unsigned flag = 0;
        if (this->selectedItem != item) {
            flag = ((ListItem *)(item))->isTextButton() ^ 1;
        }
        this->selectedItem = item;
        this->field_0xd2 = flag;
        if (((ListItem *)(item))->isShip() != 0) {
            ((Ship *)((Ship *)(G<void *>(this->selectedItem, 0xc))))->adjustPrice();
        }
    }
    return 0;
}

// ---- buildMissionOffer (tail of OnTouchEnd_14c740) ---------------------------
// The decompiler peeled the trailing equipment swap-confirmation / buy-mode dialog
// flow out of OnTouchEnd() into this helper. With the equipment-swap ChoiceWindow
// (this->dialog) showing, route the touch result:
//   * OK on the swap dialog       -> keep the new equipment, restore the scroll.
//   * Cancel on the swap dialog   -> undo: re-mount the demounted item / drop the
//                                    pending mount, and snapshot the scroll offset.
//   * buy-mode confirmation       -> commit or roll back the buy, resetting the
//                                    HangarList selection.
void HangarWindow::buildMissionOffer(int touch, int coord)
{
    HangarWindow *self = this;
    ChoiceWindow *dialog = (ChoiceWindow *)self->dialog;

    // Equipment swap confirmation.
    int swapResult = dialog->OnTouchEnd(touch, coord);
    if (swapResult == 1) {
        // Confirmed: keep the swapped-in equipment, restore the saved scroll position.
        self->dialogActive = 0;
        self->replaceEquipPending = 0;
        self->scrollOffset = self->savedScrollOffset;
    } else if (swapResult == 0 && self->pendingMountItem != 0 && self->pendingDemountItem != 0) {
        // Cancelled: undo the swap. Put the demounted item back and re-apply the mount.
        self->demountItem(self->pendingDemountItem, -1);
        self->savedScrollOffset = self->scrollOffset;
        self->mountItem(self->pendingMountItem);
        self->dialogActive = 0;
        self->replaceEquipPending = 0;
    }

    // Buy-mode confirmation (shares the same ChoiceWindow).
    uint8_t buyMode = self->buyMode;
    int buyResult = dialog->OnTouchEnd(touch, coord);
    if (buyMode != 0) {
        HangarList *list = (HangarList *)self->hangarList;
        if (buyResult == 1) {
            self->buyMode = 0;
            self->dialogActive = 0;
        } else {
            if (buyResult != 0)
                return;
            if (self->field_0x130 == 0) {
                self->dialogActive = 0;
                if (self->autoEquipped == 0) {
                    self->buyMode = 1;
                } else {
                    // Auto-equipped: clear any in-flight tab-button touches.
                    Array<TouchButton*> *tabs = self->tabButtons;
                    for (unsigned int i = 0; i < tabs->size(); i++)
                        ((TouchButton *)(tabs->data()[i]))->resetTouch();
                }
                return;
            }
            self->dialogActive = 0;
            self->field_0x130 = 0;
            self->buyMode = 0;
        }
        self->selectedItem = 0;
        list->setCurrentItemIndex(-1);
    }
}

extern "C" {
void Ship_freeSlotAt(void *ship, void *item, int slot);
void *Station_getItems(void *station);
}

__attribute__((visibility("hidden"))) extern void **g_hw_sound;

void HangarWindow::demountItem(void *item, int slot) {
    int type = ((Item *)(item))->getType();
    if (type == 1)
        ((Item *)(item))->getAmount();

    void *made = ((Item *)(item))->makeItem();
    ((Ship *)(((Status *)(*gStatus))->getShip()))->addCargo((Item *)made);

    void *ship = ((Status *)(*gStatus))->getShip();
    if (slot < 0)
        ((Ship *)(ship))->freeSlot((Item *)item);
    else
        Ship_freeSlotAt(ship, item, slot);

    bool merged = false;
    Array<Item*> *cargo = this->itemList;
    for (unsigned int i = 0; i < cargo->size(); i++) {
        void *cur = cargo->data()[i];
        if (((Item *)(cur))->getIndex() == ((Item *)(made))->getIndex()) {
            ((Item *)(cur))->changeAmount(((Item *)(item))->getAmount());
            merged = true;
            break;
        }
    }
    if (!merged)
        this->itemList->push_back((Item*)made);

    ((Ship *)(((Status *)(*gStatus))->getShip()))->setCargo(Item::extractItems((ItemArray *)(this->itemList), true));

    if (this->itemList != 0) {
        for (Item *it : *this->itemList) delete it;
            this->itemList->clear();
            delete this->itemList;
    }
    this->itemList = 0;

    this->statusShip();
    ItemArray *mixed = Item::mixItems((ItemArray *)(((Ship *)(0))->getCargo()), (ItemArray *)(Station_getItems(((Status *)(*gStatus))->getStation())));
    this->itemList = mixed;
    ((HangarList *)(this->hangarList))->initShipTab((Ship *)this->statusShip());

    this->statusShip();
    ItemArray *items = Item::mixItems((ItemArray *)(((Ship *)(0))->getCargo()), (ItemArray *)(Station_getItems(((Status *)(*gStatus))->getStation())));
    ((HangarList *)(this->hangarList))->initShopTab((Array<Item *> *)(items), Station_getShips(((Status *)(*gStatus))->getStation()));
    ((HangarList *)(this->hangarList))->setCurrentTab(false);

    // refreshCurrentContentHeight() returns void (stores into this+0xd4); the target
    // reuses a stale float register as the FModSound pitch argument.
    ((HangarWindow *)(this))->refreshCurrentContentHeight();
    float h = 0.0f;
    this->scrollOffset = this->savedScrollOffset;
    ((FModSound *)(*g_hw_sound))->play(0x60, 0, 0, h);
}

extern "C" {
int __aeabi_idiv(int a, int b);
int Station_getIndex(void *station);
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
    self->holdTime = 0;
    self->repeatTimer = 0;
    unsigned char handled = (unsigned char)((Layout *)(*g_hw_layout))->OnTouchBegin(touch);

    if (self->dialogActive != 0) {
        if (self->buyCreditsActive != 0) {
            for (int i = 0xc; i != 0x11; i++)
                btnAt(self->buttons, i * 4)->OnTouchBegin(touch);
            btnAt(self->buttons, 0x44)->OnTouchBegin(touch);
        } else if (self->freeCreditsActive != 0) {
            for (int i = 0x12; i != 0x17; i++)
                btnAt(self->buttons, i * 4)->OnTouchBegin(touch);
        }
        ((ChoiceWindow *)(self->dialog))->OnTouchBegin(touch, coord);
        return;
    }

    self->touchStartY = coord;
    self->lastTouchY = coord;
    self->scrollDelta = 0;
    self->dragging = 1;

    if (self->viewMode == 1) {
        ((ListItemWindow *)(self->listItemWindow))->OnTouchBegin(touch, coord);
        return;
    }

    Layout *layout = (Layout *)*g_hw_layout;
    unsigned char skip = 1;
    if (layout->field_0xc < coord && coord < *g_hw_screenWidth - layout->field_0x10) {
        int row = __aeabi_idiv(
            ((coord - layout->field_0xc) - layout->field_0x20) - self->rowSpacing -
                self->scrollOffset,
            layout->field_0x70 + self->rowSpacing);
        if (((HangarList *)(self->hangarList))->getCurrentLength() > row) {
            ((HangarList *)(self->hangarList))->setCurrentItemIndex(row);
            ((HangarWindow *)(self))->highlightItem(((HangarList *)(self->hangarList))->getCurrentItem());
            skip = 0;
            if (self->upgradeMode != 0) {
                void *ci = ((HangarList *)(self->hangarList))->getCurrentItem();
                if (((ListItem *)(ci))->isShip() != 0 &&
                    ((HangarList *)(self->hangarList))->getCurrentTab() == 1) {
                    ((Ship *)(((Status *)(*gStatus))->getShip()))->setCargo(Item::extractItems((ItemArray *)(self->itemList), true));
                    ((Station *)(((Status *)(*gStatus))->getStation()))->setItems((uint32_t *)Item::extractItems((ItemArray *)(self->itemList), false), false);
                }
            }
        }
    }

    if (((HangarList *)(self->hangarList))->getCurrentTab() == 4 && self->buyMode != 0 &&
        !(!skip && self->selectedItem == self->bluePrintItem) && self->bluePrintBuyCount > 0 &&
        self->dialogActive == 0 && ((BluePrint *)(self->bluePrint))->isEmpty() == 0) {
        if (((BluePrint *)(self->bluePrint))->getStationIndex() != Station_getIndex(((Status *)(*gStatus))->getStation())) {
            Globals *globals = (Globals *)*g_hw_globals;
            int bpIdx = ((Item *)(G<void *>(self->bluePrintItem, 0x10)))->getIndex();
            bool localBp = (bpIdx == 0xd1) ||
                           (((Item *)(G<void *>(self->bluePrintItem, 0x10)))->getIndex() == 0xcc);
            self->localBluePrint = localBp;

            String12 msg;
            AEString_ctor_empty(&msg);
            String12 line;

            if (self->localBluePrint == 0) {
                String12 copy, sname, fmt, result;
                AEString_ctor_str(&copy, &msg, false);
                ((BluePrint *)(self->bluePrint))->getStationName();
                Status_replaceHash(&result, globals, &copy, &sname, &fmt);

                String12 copy2, priceStr, fmt2, result2;
                AEString_ctor_str(&copy2, &msg, false);
                Layout_formatCredits(&priceStr,
                                     ((Item *)(G<void *>(self->bluePrintItem, 0x10)))->getBlueprintAmount());
                Status_replaceHash(&result2, globals, &copy2, &priceStr, &fmt2);
            }
            bool flag = (self->localBluePrint == 0);
            ((ChoiceWindow *)(self->dialog))->setMsg(*(String *)&msg, flag);
            self->dialogActive = 1;
            self->bluePrintPurchasePending = 1;
            self->suppressTouchEnd = 1;
            return;
        }
    }

    Array<TouchButton*> *tabs = self->tabButtons;
    for (unsigned int i = 0; i < tabs->size(); i++)
        handled |= (unsigned char)((TouchButton *)(tabs->data()[i]))->OnTouchBegin(touch);

    Array<void *> *buttons = F<Array<void *> *>(self, 0x24);
    for (unsigned int i = 0; i < buttons->size(); i++) {
        void *btn = buttons->data()[i];
        if (btn != 0)
            ((TouchButton *)(btn))->OnTouchBegin(touch);
    }

    if (self->autoEquipPending != 0 && ((HangarList *)(self->hangarList))->getCurrentTab() == 1) {
        int idx = self->autoEquipIndex;
        if (idx >= 0 &&
            !(!handled && (unsigned int)idx == ((HangarList *)(self->hangarList))->getCurrentItemIndex())) {
            self->autoEquipPending = 0;
            ((HangarWindow *)(self))->autoEquipSecondaryWeapons(idx);
        }
    }
}

extern "C" {
void *ApplicationManager_GetApplicationData();
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
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
    void *appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x4c) = 0;
    appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x3d) = 1;

    void *win = this->dialog;
    String12 a, b, yes, no;

    if (this->listModeFlag == 0) {
        void *body = ((GameText *)(*g_hw_buyTextId))->getText();
        ((ChoiceWindow *)(win))->set(*(String const *)&a, *(String const *)&b);
    } else {
        void *body = ((GameText *)(*g_hw_buyTextId2))->getText();
        ((ChoiceWindow *)(win))->set(*(String const *)&a, *(String const *)&b);

        int h;
        void *win2;
        if (*g_hw_buyFlag == 0) {
            ((ChoiceWindow *)(this->dialog))->setWidth(*g_hw_buyWidth);
            h = *g_hw_buyHeight;
            win2 = this->dialog;
        } else {
            ((ChoiceWindow *)(this->dialog))->setWidth(this->buttonWidth * 3);
            float v = VectorSignedToFloat(this->gridButtonHeight, 0);
            win2 = this->dialog;
            h = (int)(v * hw_buy_heightScale);
        }
        ((ChoiceWindow *)(win2))->setHeight(h);
    }

    this->dialogActive = 1;
    this->buyCreditsActive = 1;
    this->notEnoughCredits = 0;
}

int HangarWindow::getCurrentTab() {
    return ((HangarList *)(this->hangarList))->getCurrentTab();
}

// Status singleton (pc-rel -> holder; *holder is the Status object).
__attribute__((visibility("hidden"))) extern void **g_hw_status2;

void HangarWindow::refreshCargoAvailabilityForBlueprints() {
    Array<Array<ListItem*>*> *items = ((HangarList *)(this->hangarList))->getItems();
    // Original navigated the old {count,data} layout: data buffer at +0x4, element [2]
    // at byte offset +0x8 (4-byte pointer stride). With a real Array<...> that is (*items)[2].
    Array<ListItem*> *arr = (*items)[2];
    if (arr == 0) return;
    for (uint32_t i = 0; i < arr->size(); i++) {
        void *it = arr->data()[i];
        F<uint8_t>((HangarWindow *)it, 0x45) = 0;
        if (it != 0 && ((ListItem *)(it))->isBluePrint() != 0) {
            void *bp = G<void *>(it, 0x8);
            void *cargo = ((Ship *)(((Status *)(*g_hw_status2))->getShip()))->getCargo();
            Array<int> *ingr = (Array<int> *)BluePrint_getIngredientList(bp);
            if (cargo != 0) {
                void *base = G<void *>(bp, 0x0);
                for (uint32_t j = 0; j < ingr->size(); j++) {
                    int *amts = *(int **)((char *)base + 0x4);
                    if (amts[j] > 0) {
                        Array<void *> *carr = (Array<void *> *)cargo;
                        for (uint32_t k = 0; k < carr->size(); k++) {
                            if (((Item *)(carr->data()[k]))->getIndex() == ingr->data()[j]) {
                                F<uint8_t>((HangarWindow *)it, 0x45) = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

extern "C" {
void *Station_getItems(void *station);
int Station_getIndex(void *station);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern void **g_hw_itemFlags;
__attribute__((visibility("hidden"))) extern int *g_hw_sellTextId1;
__attribute__((visibility("hidden"))) extern int *g_hw_sellTextId2;
__attribute__((visibility("hidden"))) extern int *g_hw_routesTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_bpStations;

void HangarWindow::setSellMode() {
    HangarWindow *self = this;
    ListItem *item = (ListItem *)self->selectedItem;

    if (item == 0 ||
        ((ListItem *)(item))->isShip() != 0 || ((ListItem *)(item))->isSlot() != 0 ||
        ((ListItem *)(item))->isTextButton() != 0 || ((ListItem *)(item))->isSelectable() == 0 ||
        ((ListItem *)(item))->isBluePrint() != 0) {
        self->buyMode = 0;
        return;
    }

    self->buyMode = 1;

    int tab = ((HangarList *)(self->hangarList))->getCurrentTab();
    if (tab == 1) {
        if (self->buyMode == 0) {
            if (((ListItem *)(item))->isItem() != 0 && ((Item *)(item->field_0x10))->getType() != 4) {
                void *flags = *g_hw_itemFlags;
                if (G<uint8_t>(flags, 0x1e) == 0) {
                    ((GameText *)(*g_hw_sellTextId1))->getText();
                    ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
                    G<uint8_t>(flags, 0x1e) = 1;
                    self->dialogActive = 1;
                }
            }
            self->autoEquipPending = 1;
            self->autoEquipIndex = ((HangarList *)(self->hangarList))->getCurrentItemIndex();
        } else {
            void *flags = *g_hw_itemFlags;
            if (G<uint8_t>(flags, 0x1d) == 0) {
                ((GameText *)(*g_hw_sellTextId2))->getText();
                ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
                G<uint8_t>(flags, 0x1d) = 1;
                self->dialogActive = 1;
            }
            self->savedStationAmount = ((Item *)(item->field_0x10))->getStationAmount();
            self->savedAmount = ((Item *)(item->field_0x10))->getAmount();
            self->savedCredits = ((Status *)(*gStatus))->getCredits();
            self->savedLoad = self->currentLoad;
        }

        ((Ship *)(((Status *)(*gStatus))->getShip()))->setCargo(Item::extractItems((ItemArray *)(self->itemList), true));
        ((Station *)(((Status *)(*gStatus))->getStation()))->setItems((uint32_t *)Item::extractItems((ItemArray *)(self->itemList), false), false);
        if (self->itemList != 0) {
            for (Item *it : *self->itemList) delete it;
            self->itemList->clear();
            delete self->itemList;
        }
        self->itemList = 0;

        ItemArray *mixed = Item::mixItems((ItemArray *)(((Ship *)(((Status *)(*gStatus))->getShip()))->getCargo()), (ItemArray *)(Station_getItems(((Status *)(*gStatus))->getStation())));
        self->itemList = mixed;
        ((HangarList *)(self->hangarList))->initShopTab((Array<Item *> *)(mixed), Station_getShips(((Status *)(*gStatus))->getStation()));
        ((HangarList *)(self->hangarList))->initShipTab(((Status *)(*gStatus))->getShip());

        void *ci = ((HangarList *)(self->hangarList))->getCurrentItem();
        self->selectedItem = ci;
        if (ci != 0 && ((ListItem *)(ci))->isShip() != 0)
            ((Ship *)((Ship *)(G<void *>(self->selectedItem, 0xc))))->adjustPrice();
        ((HangarWindow *)(self))->refreshCurrentContentHeight();
        return;
    }

    if (((HangarList *)(self->hangarList))->getCurrentTab() != 4)
        return;

    if (self->buyMode == 0) {
        if (self->bluePrintItem != 0 && self->bluePrint != 0) {
            void *bpItem = G<void *>(self->bluePrintItem, 0x10);
            ((BluePrint *)(self->bluePrint))->addItem((Item *)bpItem, ((Item *)(bpItem))->getBlueprintAmount(), Station_getIndex(((Status *)(*gStatus))->getStation()));
        }

        uint8_t completedFlag = 0;
        if (((BluePrint *)(self->bluePrint))->isCompleted() != 0) {
            Globals *globals = (Globals *)*g_hw_globals;
            if (((BluePrint *)(self->bluePrint))->getStationIndex() == Station_getIndex(((Status *)(*gStatus))->getStation())) {
                String12 line, copy, name, fmt, result;
                AEString_ctor_str(&copy, &line, false);
                Status_replaceHash(&result, globals, &copy, &name, &fmt);
                ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);

                int *stations = g_hw_bpStations;
                int idx = stations[((BluePrint *)((BluePrint *)self->bluePrint))->getIndex()];
                ((BluePrint *)(self->bluePrint))->getQuantity();
                void *made = ((Item *)((void *)(uintptr_t)idx))->makeItem();
                ((Ship *)(((Status *)(*gStatus))->getShip()))->addCargo((Item *)made);
                self->itemList->push_back((Item*)made);
                ((HangarList *)(self->hangarList))->setCurrentTab(true);
                ((HangarWindow *)(self))->refreshCurrentContentHeight();
            } else {
                String12 line, copy, name, fmt, result, line2, sname, fmt2;
                AEString_ctor_str(&copy, &line, false);
                Status_replaceHash(&result, globals, &copy, &name, &fmt);

                AEString_ctor_str(&line2, &line, false);
                ((BluePrint *)(self->bluePrint))->getStationName();
                String12 result2;
                Status_replaceHash(&result2, globals, &line2, &sname, &fmt2);

                ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
                ((Status *)(globals))->addPendingProduct((BluePrint *)self->bluePrint);
                ((HangarList *)(self->hangarList))->setCurrentTab(true);
                ((HangarWindow *)(self))->refreshCargoAvailabilityForBlueprints();
            }
            ((BluePrint *)(self->bluePrint))->reset();
            completedFlag = 1;
        }

        Globals *globals = (Globals *)*g_hw_globals;
        ((Ship *)(this->statusShip()))->setCargo(Item::extractItems((ItemArray *)(((Ship *)(0))->getCargo()), true));
        this->statusShip();
        ItemArray *items = Item::mixItems((ItemArray *)(((Ship *)(0))->getCargo()), (ItemArray *)(Station_getItems(((Status *)(*gStatus))->getStation())));
        ((HangarList *)(self->hangarList))->initShopTab((Array<Item *> *)(items), Station_getShips(((Status *)(*gStatus))->getStation()));
        ((HangarList *)(self->hangarList))->initBlueprintTab((Array<BluePrint *> *)(long)((Status *)(globals))->getBluePrints());
        ItemArray *mix = Item::mixItems((ItemArray *)(((Ship *)(0))->getCargo()), (ItemArray *)(Station_getItems(((Status *)(*gStatus))->getStation())));
        self->itemList = mix;
        self->dialogActive = completedFlag;

        if (completedFlag) {
            Array<ListItem*> *items2 = ((HangarList *)self->hangarList)->getCurrentTabItems();
            for (unsigned int i = 0; i < items2->size(); i++) {
                void *li = items2->data()[i];
                if (li != 0 && ((ListItem *)(li))->isItem() != 0 &&
                    ((Item *)(G<void *>(li, 0x10)))->getIndex() == ((BluePrint *)((BluePrint *)(self->bluePrint)))->getIndex()) {
                    if (((Ship *)(((Status *)(*gStatus))->getShip()))->hasEquipment(((Item *)(G<void *>(li, 0x10)))->getIndex(), 1) != 0) {
                        self->autoEquipIndex = i;
                        self->autoEquipPending = 1;
                        ((HangarWindow *)(self))->autoEquipSecondaryWeapons(i);
                        self->autoEquipPending = 0;
                        break;
                    }
                }
            }
        }
        return;
    }

    self->bluePrintBuyCount = 0;
    if (((BluePrint *)(self->bluePrint))->isEmpty() != 0 && ((Item *)(item->field_0x10))->getAmount() > 0) {
        int idx = ((BluePrint *)((BluePrint *)(self->bluePrint)))->getIndex();
        bool flag;
        void *text;
        if (idx == 0xd2 || ((BluePrint *)((BluePrint *)(self->bluePrint)))->getIndex() == 0xdf) {
            if (((SolarSystem *)((void *)(long)((Status *)(*gStatus))->getSystem()))->getRoutes() != 0) {
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
        ((ChoiceWindow *)(self->dialog))->setMsg(*(String *)text, flag);
        self->dialogActive = 1;
    }

    self->savedBlueprintAmount = ((Item *)(item->field_0x10))->getBlueprintAmount();
    self->savedAmount = ((Item *)(item->field_0x10))->getAmount();
    self->savedCredits = ((Status *)(*gStatus))->getCredits();
    self->savedLoad = self->currentLoad;
    self->bluePrintItem = self->selectedItem;
}

extern "C" {
int Item_canBeInstalledMultipleTimes(void *item);
void *Station_getItems(void *station);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);
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
    ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
    self->dialogActive = 1;
}

void HangarWindow::selectItem(void *item) {
    HangarWindow *self = this;
    ListItem *li = (ListItem *)item;
    self->selectedItem = item;
    if (item != 0 && ((ListItem *)(item))->isShip() != 0)
        ((Ship *)((Ship *)(G<void *>(self->selectedItem, 0xc))))->adjustPrice();

    int tab = ((HangarList *)(self->hangarList))->getCurrentTab();

    if (tab == 2) {
        if (((ListItem *)(item))->isSelectable() != 0 && ((ListItem *)(item))->isPendingProduct() == 0) {
            void *bp = li->field_0x8;
            self->bluePrint = bp;
            ((HangarList *)(self->hangarList))->fillIngredientsList((BluePrint *)bp, bp != 0);
            ((HangarList *)(self->hangarList))->setCurrentTab(true);
            ((HangarWindow *)(self))->refreshCurrentContentHeight();
            if (self->specialMode != 0)
                self->specialMode = 0;
        }
        return;
    }

    if (tab == 1) {
        if (((ListItem *)(item))->isSelectable() == 0)
            return;
        if (((ListItem *)(item))->isShip() == 0) {
            // Buying / selling a regular item.
            if (((Item *)(li->field_0x10))->isUnsaleable() != 0)
                return;

            uint8_t was = self->buyMode;
            self->buyMode = (uint8_t)(was ^ 1);
            if (was == 0) {
                void *flags = *g_hw_itemFlags;
                if (G<uint8_t>(flags, 0x1d) == 0) {
                    ((GameText *)(*g_hw_sellMsgTextId1))->getText();
                    ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
                    G<uint8_t>(flags, 0x1d) = 1;
                    self->dialogActive = 1;
                }
                self->savedStationAmount = ((Item *)(li->field_0x10))->getStationAmount();
                self->savedAmount = ((Item *)(li->field_0x10))->getAmount();
                self->savedCredits = ((Status *)(*gStatus))->getCredits();
                self->savedLoad = self->currentLoad;
            } else {
                if (((ListItem *)(item))->isItem() != 0 && ((Item *)(li->field_0x10))->getType() != 4) {
                    void *flags = *g_hw_itemFlags;
                    if (G<uint8_t>(flags, 0x1e) == 0) {
                        ((GameText *)(*g_hw_sellMsgTextId2))->getText();
                        ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
                        G<uint8_t>(flags, 0x1e) = 1;
                        self->dialogActive = 1;
                    }
                }
                int li = ((ListItem *)(item))->getIndex();
                if (li > 0x83 && ((ListItem *)(item))->getIndex() < 0x9a) {
                    int base = G<int>(*g_hw_globals, 0xac);
                    *((uint8_t *)G<int>((void *)(uintptr_t)base, 4) + ((ListItem *)(item))->getIndex() - 0x84) = 1;
                }
                self->autoEquipPending = 1;
                self->autoEquipIndex = ((HangarList *)(self->hangarList))->getCurrentItemIndex();

                ((Ship *)(((Status *)(*gStatus))->getShip()))->setCargo(Item::extractItems((ItemArray *)(self->itemList), true));
                ((Station *)(((Status *)(*gStatus))->getStation()))->setItems((uint32_t *)Item::extractItems((ItemArray *)(self->itemList), false), false);
                if (self->itemList != 0) {
                    for (Item *it : *self->itemList) delete it;
            self->itemList->clear();
            delete self->itemList;
                }
                self->itemList = 0;

                ItemArray *mixed = Item::mixItems((ItemArray *)(((Ship *)(((Status *)(*gStatus))->getShip()))->getCargo()), (ItemArray *)(Station_getItems(((Status *)(*gStatus))->getStation())));
                self->itemList = mixed;
                ((HangarList *)(self->hangarList))->initShopTab((Array<Item *> *)(mixed), Station_getShips(((Status *)(*gStatus))->getStation()));
                ((HangarList *)(self->hangarList))->initShipTab(((Status *)(*gStatus))->getShip());
            }
            return;
        }

        // Buying a ship.
        int price = ((Ship *)(li->field_0xc))->getPrice();
        Globals *globals = (Globals *)*g_hw_globals;
        int credits = ((Status *)(*gStatus))->getCredits();
        int oldPrice = ((Ship *)(((Status *)(*gStatus))->getShip()))->getPrice();
        if (oldPrice + credits < price && self->upgradeMode == 0) {
            String12 line, priceStr, fmt, msg, suffix, combined;
            Layout_formatCredits(&priceStr,
                                 ((ListItem *)(item))->getPrice() - ((Status *)(*gStatus))->getCredits() - ((Ship *)(((Status *)(*gStatus))->getShip()))->getPrice(),
                                 0);
            AEString_ctor_str(&line, &priceStr, false);
            Status_replaceHash(&msg, globals, &line, &priceStr, &fmt);
            ((GameText *)(*g_hw_notEnoughTextId))->getText();
            AEString_add(&combined, &suffix, &suffix);
            AEString_addAssign(&msg, &combined);
            ((ChoiceWindow *)(self->dialog))->setMsg(*(String *)&msg, true);
            self->dialogActive = 1;
            self->notEnoughCredits = 1;
            return;
        }

        if (globals->field_0x34 < 1) {
            if (((Status *)(*gStatus))->getCurrentCampaignMission() == 0x4d &&
                ((Ship *)(((Status *)(*gStatus))->getShip()))->getIndex() == 0x25) {
                showText(self, *g_hw_unsaleableTextId);
                return;
            }
            int a = ((Ship *)(((Status *)(*gStatus))->getShip()))->getIndex();
            int b = ((Ship *)(li->field_0xc))->getIndex();
            if (a != b) {
                self->shipSwapPending = 1;
                ((GameText *)(*g_hw_sellMsgTextId2))->getText();
                ((ChoiceWindow *)(self->dialog))->setMsg(*(String *)((GameText *)(*g_hw_sellMsgTextId2))->getText(), true);
                self->dialogActive = 1;
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

    self->scrollOffsetBackup = 0;
    self->savedScrollOffset = self->scrollOffset;
    self->scrollOffset = 0;

    void *flags0 = *g_hw_itemFlags;
    if (G<uint8_t>(flags0, 0x1f) == 0 && ((ListItem *)(item))->isSlot() != 0) {
        ((GameText *)(*g_hw_slotMsgTextId))->getText();
        ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
        G<uint8_t>(flags0, 0x1f) = 1;
        self->dialogActive = 1;
    }

    if (((ListItem *)(item))->isSelectable() == 0)
        return;

    void *cur = ((HangarList *)(self->hangarList))->getCurrentItem();
    void *curItem = G<void *>(cur, 0x10);
    if (curItem != 0) {
        if (((Item *)(curItem))->isUnsaleable() != 0) {
            showText(self, *g_hw_unsaleableTextId);
            return;
        }
        if (((Item *)(curItem))->getSort() == 0x14) {
            Globals *globals = (Globals *)*g_hw_globals;
            int passengerCount = globals->field_0x34;
            int chosen = passengerCount;
            if (passengerCount > 0)
                chosen = li->field_0x3c;
            int adj = passengerCount - 1;
            if (passengerCount >= 1)
                adj = chosen;
            if (adj >= 0 == (passengerCount >= 1)) {
                int maxPax = ((Ship *)(((Status *)(*gStatus))->getShip()))->getMaxPassengers();
                int used = ((Item *)(curItem))->getAttribute(0);
                if (maxPax - used < globals->field_0x34) {
                    showText(self, *g_hw_unsaleableTextId);
                    return;
                }
            }
        }
    }

    if (li->field_0x3c >= 0) {
        void *ci = ((HangarList *)(self->hangarList))->getCurrentItem();
        ((HangarWindow *)(self))->demountItem(curItem, G<int>(ci, 0x40));
        return;
    }

    // Mounting: check for an existing equipment of the same sort.
    Globals *globals = (Globals *)*g_hw_globals;
    int sort = ((Item *)(li->field_0x10))->getSort();
    void *existing = ((Ship *)(((Status *)(*gStatus))->getShip()))->getFirstEquipmentOfSort(sort);
    bool conflict = false;
    if (((Item *)(li->field_0x10))->getSort() == 0x15) {
        if (((Ship *)(((Status *)(*gStatus))->getShip()))->getIndex() != 0x2c &&
            existing != 0 && ((Ship *)(((Status *)(*gStatus))->getShip()))->getIndex() != 0x31)
            conflict = true;
    } else if (existing != 0) {
        conflict = true;
    }

    if (conflict && Item_canBeInstalledMultipleTimes(li->field_0x10) == 0) {
        String12 name, copy, etext, fmt, result, etext2, fmt2, result2;
        AEString_ctor_str(&copy, &name, false);
        ((Item *)(existing))->getIndex();
        Status_replaceHash(&result, globals, &copy, &etext, &fmt);
        ((Item *)(li->field_0x10))->getIndex();
        Status_replaceHash(&result2, globals, &result, &etext2, &fmt2);
        ((ChoiceWindow *)(self->dialog))->setMsg(*(String *)&name, true);
        self->replaceEquipPending = 1;
        self->dialogActive = 1;
        self->pendingMountItem = li->field_0x10;
        self->pendingDemountItem = existing;
        return;
    }

    ((HangarWindow *)(self))->mountItem(li->field_0x10);
}

// Returns float in r0. a=this+0xd4 (denominator), b=this+0xd8.
// NEAR: clang pairs a->r2,b->r1 in the ldrd (target uses r1=a,r2=b), mirroring every
// subsequent op; register pairing is an allocator tie-break, not source-driven.
float HangarWindow::getRelativeScrollHeight() {
    int a = this->currentContentHeight;
    int b = this->visibleHeight;
    if (a < b) {
        union { unsigned u; float f; } c; c.u = 0x4605e009u;
        return c.f;
    }
    int e = this->scrollOffset;
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

extern "C" {
unsigned int Item_transaction(void *item, bool buy, int amount, bool flag);
void Status_changeCredits(void *globals);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern int *g_hw_unsaleableTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_notEnoughTextId;

void HangarWindow::transaction(bool buy) {
    unsigned int tab = ((HangarList *)(this->hangarList))->getCurrentTab();
    void *cur = G<void *>(this->selectedItem, 0x10);

    if (tab < 2) {
        if (((Item *)(cur))->isUnsaleable() != 0) {
            ((ChoiceWindow *)(this->dialog))->set(*(String *)((GameText *)(*g_hw_unsaleableTextId))->getText());
            this->buyMode = 0;
            this->dialogActive = 1;
            return;
        }

        unsigned int result = Item_transaction(cur, buy, this->currentLoad, this->upgradeMode);
        unsigned int idx = ((Item *)(cur))->getIndex();
        Globals *globals = (Globals *)*g_hw_globals;
        unsigned int *avail = globals->field_0x54;
        if (idx < avail[0])
            *((uint8_t *)avail[1] + ((Item *)(cur))->getIndex()) = 1;

        if (result >= 0x80000000 && buy) {
            this->currentLoad = this->currentLoad + 1;
            ((Ship *)(((Status *)(*gStatus))->getShip()))->changeLoad(1);
            int li = ((ListItem *)(this->selectedItem))->getIndex();
            if (li >= 0x84 && ((ListItem *)(this->selectedItem))->getIndex() < 0x9a) {
                int base = globals->field_0xac;
                *((uint8_t *)G<int>((void *)(uintptr_t)base, 4) +
                  ((ListItem *)(this->selectedItem))->getIndex() - 0x84) = 1;
            }
        } else if (result == 0 && buy) {
            if (((Status *)(*gStatus))->getCredits() < ((Item *)(cur))->getSinglePrice()) {
                if (this->upgradeMode != 0)
                    return;
                String12 line, priceStr, fmt, msg, suffix, combined;
                Layout_formatCredits(&priceStr, ((Item *)(cur))->getSinglePrice(), ((Status *)(*gStatus))->getCredits());
                AEString_ctor_str(&line, &priceStr, false);
                Status_replaceHash(&msg, globals, &line, &priceStr, &fmt);
                ((GameText *)(*g_hw_notEnoughTextId))->getText();
                AEString_add(&combined, &suffix, &suffix);
                AEString_addAssign(&msg, &combined);
                ((ChoiceWindow *)(this->dialog))->setMsg(*(String *)&msg, true);
                this->dialogActive = 1;
                this->notEnoughCredits = 1;
                btnAt(this->buttons, 0x20)->resetTouch();
                btnAt(this->buttons, 0x24)->resetTouch();
            }
        } else if ((int)result > 0 && !buy) {
            this->currentLoad = this->currentLoad - 1;
            ((Ship *)(((Status *)(*gStatus))->getShip()))->changeLoad(-1);
        }

        if (this->upgradeMode == 0)
            Status_changeCredits(globals);
    } else if (tab == 4) {
        if (buy) {
            int bpAmt = ((Item *)(cur))->getBlueprintAmount();
            void *bp = this->bluePrint;
            int remaining = ((BluePrint *)(bp))->getRemainingAmount(((Item *)(cur))->getIndex());
            if (bpAmt < remaining) {
                int r = ((Item *)(cur))->transactionBlueprint(0);
                if (r < 0) {
                    this->currentLoad = this->currentLoad + 1;
                } else if (r != 0) {
                    this->bluePrintBuyCount = this->bluePrintBuyCount + 1;
                    ((Ship *)(((Status *)(*gStatus))->getShip()))->changeLoad(-1);
                }
            }
        }
        void *cargo = ((Ship *)(((Status *)(*gStatus))->getShip()))->getCargo();
        if (cargo != 0) {
            Array<void *> *arr = (Array<void *> *)cargo;
            for (unsigned int i = 0; i < arr->size(); i++) {
                if (((Item *)(arr->data()[i]))->getIndex() == ((Item *)(cur))->getIndex()) {
                    ((Item *)(arr->data()[i]))->setAmount(((Item *)(arr->data()[i]))->getAmount());
                    ((Item *)(arr->data()[i]))->setBlueprintAmount(((Item *)(cur))->getBlueprintAmount());
                }
            }
        }
    }
}

extern "C" {
void *Station_getItems(void *station);
}

__attribute__((visibility("hidden"))) extern void **g_hw_sound;

void HangarWindow::mountItem(void *item) {
    int type = ((Item *)(item))->getType();
    int amount = 1;
    if (type == 1)
        amount = ((Item *)(item))->getAmount();

    void *made = ((Item *)(item))->makeItem();
    void *ship = ((Status *)(*gStatus))->getShip();
    ((Ship *)(ship))->addEquipment((Item *)made);
    ((Ship *)(((Status *)(*gStatus))->getShip()))->removeCargo(((Item *)(made))->getIndex(), (type == 1) ? amount : 1);

    Array<Item*> *cargo = this->itemList;
    if (cargo != 0) {
        for (unsigned int i = 0; i < cargo->size(); i++) {
            void *cur = cargo->data()[i];
            if (((Item *)(cur))->getIndex() == ((Item *)(item))->getIndex()) {
                int change;
                if (((Item *)(cur))->getStationAmount() == 0) {
                    if (type == 1 || ((Item *)(item))->getAmount() == 1) {
                        this->itemList->erase(std::find(this->itemList->begin(), this->itemList->end(), (Item*)cur));
                        break;
                    }
                    change = -1;
                } else if (type == 1) {
                    change = -((Item *)(item))->getAmount();
                } else {
                    change = -1;
                }
                ((Item *)(cur))->changeAmount(change);
                break;
            }
            cargo = this->itemList;
        }
    }

    void *ship2 = this->statusShip();
    ((Ship *)(ship2))->setCargo(Item::extractItems((ItemArray *)(this->itemList), true));
    ((HangarList *)(this->hangarList))->initShipTab((Ship *)this->statusShip());

    this->statusShip();
    ItemArray *items = Item::mixItems((ItemArray *)(((Ship *)(0))->getCargo()), (ItemArray *)(Station_getItems(((Status *)(*gStatus))->getStation())));
    ((HangarList *)(this->hangarList))->initShopTab((Array<Item *> *)(items), Station_getShips(((Status *)(*gStatus))->getStation()));
    ((HangarList *)(this->hangarList))->setCurrentTab(false);

    // refreshCurrentContentHeight() returns void (stores into this+0xd4); the target
    // reuses a stale float register as the FModSound pitch argument.
    ((HangarWindow *)(this))->refreshCurrentContentHeight();
    float h = 0.0f;
    this->scrollOffset = this->savedScrollOffset;
    ((FModSound *)(*g_hw_sound))->play(0x62, 0, 0, h);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;

unsigned int HangarWindow::OnTouchMove(int touch, int coord) {
    Layout *layout = (Layout *)*g_hw_layout;
    ((Layout *)(layout))->OnTouchMove(touch);

    if (this->dialogActive != 0) {
        if (this->buyCreditsActive != 0) {
            for (int i = 0xc; i != 0x11; i++)
                btnAt(this->buttons, i * 4)->OnTouchMove(touch);
            btnAt(this->buttons, 0x44)->OnTouchMove(touch);
        } else if (this->freeCreditsActive != 0) {
            for (int i = 0x12; i != 0x17; i++)
                btnAt(this->buttons, i * 4)->OnTouchMove(touch);
        }
        ((ChoiceWindow *)(this->dialog))->OnTouchMove(touch, coord);
        return 0;
    }

    if (this->viewMode == 1) {
        ((ListItemWindow *)(this->listItemWindow))->OnTouchMove(touch, coord);
        return 0;
    }

    if (layout->field_0xc < coord && coord < *g_hw_screenWidth - layout->field_0x10) {
        int dy = coord - this->lastTouchY;
        this->scrollDelta = dy;
        this->damping = 1.0f;
        this->scrollOffset = this->scrollOffset + dy;
        this->lastTouchY = coord;

        void *btnUp = btnAt(this->buttons, 0x20);
        void *btnDown = btnAt(this->buttons, 0x24);
        int touched = ((TouchButton *)(btnUp))->isTouched();
        if (touched != 0)
            touched = 1;
        else
            touched = ((TouchButton *)(btnDown))->isTouched();

        int adist = coord - this->touchStartY;
        if (adist < 0)
            adist = -adist;

        if (touched == 0 && adist > 5) {
            this->holdTime = 0;
            this->repeatTimer = 0;
            Array<void *> *buttons = F<Array<void *> *>(this, 0x24);
            for (unsigned int i = 0; i < buttons->size(); i++)
                ((TouchButton *)(buttons->data()[i]))->OnTouchMove(touch);
            ((HangarWindow *)(this))->setSellMode();
            this->field_0xd2 = 0;
            this->selectedItem = 0;
            ((TouchButton *)(btnUp))->resetTouch();
            ((TouchButton *)(btnDown))->resetTouch();
        }
    }

    Array<TouchButton*> *tabs = this->tabButtons;
    for (unsigned int i = 0; i < tabs->size(); i++)
        ((TouchButton *)(tabs->data()[i]))->OnTouchMove(touch);
    return 0;
}

extern "C" {
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_copy(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
}

__attribute__((visibility("hidden"))) extern int *g_hw_equipTextId;
__attribute__((visibility("hidden"))) extern void **g_hw_globals;

void HangarWindow::autoEquipSecondaryWeapons(int row) {
    ListItem *item = (ListItem *)((HangarList *)(this->hangarList))->getCurrentItemAt(row);
    if (item == 0)
        return;
    void *itm = item->field_0x10;
    if (itm == 0 || ((Item *)(itm))->getType() != 1)
        return;
    if (((ListItem *)(item))->isItem() == 0 || ((Item *)(itm))->getType() != 1)
        return;
    if (((Item *)(itm))->getAmount() <= 0)
        return;

    void *ship = ((Status *)(*gStatus))->getShip();
    int *equip = (int *)Ship_getEquipmentByType((Ship *)ship, 1);
    if (equip == 0)
        return;

    Array<void *> *arr = (Array<void *> *)equip;
    for (unsigned int i = 0; i < arr->size(); i++) {
        void *cur = arr->data()[i];
        if (cur == 0)
            continue;
        if (((Item *)(cur))->getIndex() != ((Item *)(itm))->getIndex())
            continue;

        void *made = ((Item *)(itm))->makeItem();

        Array<Item*> *cargo = this->itemList;
        if (cargo != 0) {
            for (unsigned int j = 0; j < cargo->size(); j++) {
                if (((Item *)(cargo->data()[j]))->getIndex() == ((Item *)(made))->getIndex())
                    ((Item *)(cargo->data()[j]))->setAmount(0);
                cargo = this->itemList;
            }
        }

        ((Ship *)(this->statusShip()))->setEquipment((Item *)made);
        ((Ship *)(this->statusShip()))->removeCargo(((Item *)(made))->getIndex(), ((Item *)(itm))->getAmount());
        ((HangarList *)(this->hangarList))->initShipTab((Ship *)this->statusShip());

        String12 msg, msgCopy, name, fmt;
        String12 result;
        Status_replaceHash(&result, *g_hw_globals, &msgCopy, &name, &fmt);

        ((ChoiceWindow *)(this->dialog))->set(*(String const *)&msg);
        this->autoEquipped = 1;
        this->dialogActive = 1;
        break;
    }
}

// AbyssEngine::String - 12-byte value type.

extern "C" {
void *ApplicationManager_GetApplicationData();
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
}

__attribute__((visibility("hidden"))) extern int *g_hw_freeCreditsTextId;
__attribute__((visibility("hidden"))) extern void **g_hw_canvas;
__attribute__((visibility("hidden"))) extern void **g_hw_layout;

void HangarWindow::showFreeCreditsWindow() {
    void *appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x4c) = 0;
    appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x3d) = 1;

    void *win = this->dialog;
    String12 title, title2, yes, no;
    void *body = ((GameText *)(*g_hw_freeCreditsTextId))->getText();
    ((ChoiceWindow *)(win))->set(*(String const *)&title, *(String const *)&title2);

    int rowH = btnAt(this->buttons, 0x48)->getHeight();
    ((ChoiceWindow *)(this->dialog))->setHeight(rowH * 5);

    int maxW = 0;
    for (int i = 5; i != 0; i--) {
        ((GameText *)(*g_hw_freeCreditsTextId))->getText();
        int w = ((PaintCanvas *)*g_hw_canvas)->GetTextWidth(0, (void *)0);
        if (maxW < w)
            maxW = w;
    }
    int btnW = ((TouchButton *)(0))->getWidth();
    Layout *layout = (Layout *)*g_hw_layout;
    ((ChoiceWindow *)(this->dialog))->setWidth(layout->field_0x2c + btnW + maxW + layout->field_0x28 * 4);

    this->freeCreditsActive = 1;
    this->dialogActive = 1;
    this->buyCreditsActive = 0;
    this->notEnoughCredits = 0;
}

extern "C" {
int Station_getIndex(void *station);
void *Station_getItems(void *station);
int __aeabi_idiv(int a, int b);

void HangarList_ctor(void *list);


// TouchButton constructor overloads (distinct argument arities in the target).
void TouchButton_ctor_text(void *btn, void *text, int a, int b, int c, char k);
void TouchButton_ctor_text2(void *btn, void *text, int a, int b, int c, int d, char k);
void TouchButton_ctor_img(void *btn, void *img, int a, int b, int c, int d, char k, char m);
void TouchButton_ctor_img2(void *btn, void *imgA, void *imgB, int a, int b, int c, char k);
void TouchButton_getPosition(void *btn, float *x, float *y);

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
    if (Station_getIndex(((Status *)(*gStatus))->getStation()) == 0x6c)
        special = (status->field_0x114 == 3);
    self->upgradeMode = special;
    ((Status *)(status))->calcCargoPrices();

    // Build the hangar item list.
    void *list = ::operator new(0xc);
    HangarList_ctor(list);
    self->hangarList = list;
    ItemArray *mixed = Item::mixItems((ItemArray *)(((Ship *)(((Status *)(*gStatus))->getShip()))->getCargo()), (ItemArray *)(Station_getItems(((Status *)(*gStatus))->getStation())));
    self->itemList = mixed;
    ((HangarList *)(list))->init(((Status *)(*gStatus))->getShip(), 0, Station_getShips(((Status *)(*gStatus))->getStation()), (Array<BluePrint *> *)(long)((Status *)(*g_hw_globals))->getBluePrints());

    // Tab bar: 3 help/info buttons across the top.
    self->tabButtons = new Array<TouchButton*>();
    self->tabButtons->resize(3);

    int scrW = *g_hw_screenWidth;
    Layout *layout = (Layout *)*g_hw_layout;

    void *b0 = ::operator new(200);
    TouchButton_ctor_text(b0, ((GameText *)(*g_hw_helpTextId))->getText(), 3,
                          scrW - Layout_getHelpButtonOffset(), 0, 0x12);
    btnSlot(self->tabButtons, 8) = (TouchButton*)(b0);

    void *b1 = ::operator new(200);
    int w0 = ((TouchButton *)(b0))->getWidth();
    TouchButton_ctor_text(b1, ((GameText *)(*g_hw_helpTextId))->getText(), 3,
                          (scrW - Layout_getHelpButtonOffset() - w0) + layout->field_0x38, 0, 0x12);
    btnSlot(self->tabButtons, 4) = (TouchButton*)(b1);

    void *b2 = ::operator new(200);
    int w0b = ((TouchButton *)(b0))->getWidth();
    int w1b = ((TouchButton *)(b1))->getWidth();
    TouchButton_ctor_text(b2, ((GameText *)(*g_hw_helpTextId))->getText(), 3,
                          (scrW - Layout_getHelpButtonOffset() - w0b - w1b) + layout->field_0x38 * 2,
                          0, 0x12);
    btnSlot(self->tabButtons, 0) = (TouchButton*)(b2);
    self->listModeFlag = *g_hw_listModeFlag;

    // Six tab icons.
    void *icons = ::operator new[](0x18);
    self->tabIcons = icons;
    for (int i = 0; i != 6; i++)
        ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(i + 0x232a), (unsigned int *)((unsigned int *)((char *)icons + i * 4)));

    int *posX = (int *)*g_hw_posXArray;
    int *posY = (int *)*g_hw_posYArray;
    Array<TouchButton*> *tabArr = self->tabButtons;
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
    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x52e), (unsigned int *)((unsigned int *)((char *)self + 0xe8)));
    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x544), (unsigned int *)((unsigned int *)((char *)self + 0xec)));

    // Action button bank (24 entries).
    self->buttons = new Array<TouchButton*>();
    self->buttons->resize(0x18);

    unsigned int img;
    img = 0xffffffff;
    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x470), (unsigned int *)(&img));
    void *e0 = ::operator new(200);
    TouchButton_ctor_img((void *)e0, (void *)(uintptr_t)img, 7, 0, 0, layout->field_0x60, 0x11, 4);
    btnSlot(self->buttons, 0) = (TouchButton*)(e0);

    if (((Status *)(*gStatus))->getCurrentCampaignMission() == 0x4d)
        Station_getIndex(((Status *)(*gStatus))->getStation());

    void *e1 = ::operator new(200);
    TouchButton_ctor_text(e1, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, 0x11);
    btnSlot(self->buttons, 4) = (TouchButton*)(e1);
    void *e2 = ::operator new(200);
    TouchButton_ctor_text(e2, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, 0x11);
    btnSlot(self->buttons, 8) = (TouchButton*)(e2);

    img = 0xffffffff;
    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x533), (unsigned int *)(&img));
    void *e3 = ::operator new(200);
    TouchButton_ctor_img((void *)e3, (void *)(uintptr_t)img, 7, 0, 0, layout->field_0x64, 0x11, 4);
    btnSlot(self->buttons, 0xc) = (TouchButton*)(e3);

    img = 0xffffffff;
    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x532), (unsigned int *)(&img));
    void *e4 = ::operator new(200);
    TouchButton_ctor_img((void *)e4, (void *)(uintptr_t)img, 7, 0, 0, layout->field_0x64, 0x11, 4);
    btnSlot(self->buttons, 0x10) = (TouchButton*)(e4);

    void *e5 = ::operator new(200);
    TouchButton_ctor_text2(e5, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, self->buttonHeight, 0x11);
    btnSlot(self->buttons, 0x14) = (TouchButton*)(e5);
    void *e6 = ::operator new(200);
    TouchButton_ctor_text2(e6, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, self->buttonHeight, 0x11);
    btnSlot(self->buttons, 0x18) = (TouchButton*)(e6);
    void *e7 = ::operator new(200);
    TouchButton_ctor_text(e7, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0, 0x11);
    btnSlot(self->buttons, 0x1c) = (TouchButton*)(e7);

    {
        String12 lbl;
        void *e8 = ::operator new(200);
        TouchButton_ctor_img((void *)e8, &lbl, 8, 0, 0, layout->field_0x50, 0x11, 4);
        btnSlot(self->buttons, 0x20) = (TouchButton*)(e8);
    }
    {
        String12 lbl;
        void *e9 = ::operator new(200);
        TouchButton_ctor_img((void *)e9, &lbl, 9, 0, 0, layout->field_0x50, 0x11, 4);
        btnSlot(self->buttons, 0x24) = (TouchButton*)(e9);
    }
    {
        void *e10 = ::operator new(200);
        TouchButton_ctor_img((void *)e10, ((GameText *)(*g_hw_helpTextId))->getText(), 7, 0, 0,
                             layout->field_0x50, 0x11, 4);
        btnSlot(self->buttons, 0x28) = (TouchButton*)(e10);
    }
    {
        String12 credits;
        void *e11 = ::operator new(200);
        Layout_formatCredits(&credits, ((Status *)(*gStatus))->getCredits());
        TouchButton_ctor_img((void *)e11, &credits, 0xb, *g_hw_screenWidth, *g_hw_screenHeight,
                             Layout_getFooterTransitionWidth(), 0x22, 4);
        btnSlot(self->buttons, 0x2c) = (TouchButton*)(e11);
    }

    // Five collapsible "more" rows (indices 0x0c..0x10 in slot terms).
    uint8_t listMode = self->listModeFlag;
    int row = 0;
    for (int slot = 0xc; (unsigned int)(slot - 0xc) < 5; slot++) {
        String12 lbl;
        void *btn = ::operator new(200);
        int visIdx;
        if (listMode == 0) {
            TouchButton_ctor_img(btn, &lbl, 0, 0, 0, layout->field_0x264, 0x11, 1);
            visIdx = row + 0xc;
        } else {
            TouchButton_ctor_text(btn, &lbl, 10, 0, 0, 1);
            visIdx = slot;
        }
        btnSlot(self->buttons, slot * 4) = (TouchButton*)(btn);
        btnAt(self->buttons, visIdx * 4)->setVisible(false);
        row++;
    }
    {
        String12 lbl;
        void *btn = ::operator new(200);
        if (listMode == 0)
            TouchButton_ctor_img(btn, &lbl, 0, 0, 0, layout->field_0x264, 0x11, 1);
        else
            TouchButton_ctor_text(btn, &lbl, 10, 0, 0, 1);
        btnSlot(self->buttons, 0x44) = (TouchButton*)(btn);
        btnAt(self->buttons, 0x44)->setVisible(false);
    }

    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x233e), (unsigned int *)((unsigned int *)((char *)self + 0x34)));
    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x233f), (unsigned int *)((unsigned int *)((char *)self + 0x38)));
    {
        String12 lbl;
        void *btn = ::operator new(200);
        TouchButton_ctor_text(btn, &lbl, 7, 0, 0, 0x11);
        btnSlot(self->buttons, 0x5c) = (TouchButton*)(btn);
    }

    // Tab selector icons (5 entries with paired up/down images).
    unsigned int imgA, imgB;
    for (int i = 0x12; (unsigned int)(i - 0x12) < 5; i++) {
        imgB = 0xffffffff;
        if (i == 0x12) {
            ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x233c), (unsigned int *)(&imgA));
            ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x233d), (unsigned int *)(&imgB));
        } else {
            short s = (short)(i - 0x12);
            ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(s * 2 + 0x2330), (unsigned int *)(&imgA));
            ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(s * 2 + 0x2331), (unsigned int *)(&imgB));
        }
        void *btn = ::operator new(200);
        TouchButton_ctor_img2(btn, (void *)(uintptr_t)imgA, (void *)(uintptr_t)imgB, 0x13, 0, 0, 1);
        btnSlot(self->buttons, i * 4) = (TouchButton*)(btn);
        btnAt(self->buttons, i * 4)->setVisible(false);
    }

    self->buttonWidth = ((TouchButton *)(0))->getWidth();
    int h = btnAt(self->buttons, 0x30)->getHeight();
    self->gridButtonHeight = h;
    self->gridSpacingX = (int)((float)(-self->buttonWidth) * g_hw_posScale);
    self->gridSpacingY = (int)((float)(-h) * g_hw_posScale);

    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x475), (unsigned int *)((unsigned int *)((char *)self + 0x78)));
    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x476), (unsigned int *)((unsigned int *)((char *)self + 0x7c)));
    ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x477), (unsigned int *)((unsigned int *)((char *)self + 0x74)));
    self->progressBarWidth = ((PaintCanvas *)*g_hw_canvas)->GetImage2DWidth(0);
    self->progressBarHeight = ((PaintCanvas *)*g_hw_canvas)->GetImage2DHeight(0);

    // Recompute best cargo prices from known equipment when not in a black market.
    if (self->itemList != 0 && ((Status *)(*gStatus))->inBlackMarketSystem() == 0 &&
        self->upgradeMode == 0) {
        Array<Item*> *equip = ((Ship*)(((Status *)(*gStatus))->getShip()))->getEquipment();
        Array<Item*> *cargo = self->itemList;
        unsigned int n = cargo->size() + (equip ? equip->size() : 0);
        for (unsigned int i = 0; i < n; i++) {
            void *itemPtr;
            if (i < cargo->size())
                itemPtr = cargo->data()[i];
            else
                itemPtr = equip->data()[i - cargo->size()];
            if (itemPtr != 0) {
                int price = ((Item *)(itemPtr))->getSinglePrice();
                int idx = ((Item *)(itemPtr))->getIndex();
                Globals *globals = (Globals *)*g_hw_globals;
                int *buyTbl = (int *)G<int>(globals->field_0x40, 4);
                if (buyTbl[idx] < price || buyTbl[idx] == 0) {
                    buyTbl[idx] = price;
                    int sysIdx = ((SolarSystem *)((void *)(long)((Status *)(*gStatus))->getSystem()))->getIndex();
                    *(int *)(G<int>(globals->field_0x48, 4) + idx * 4) = sysIdx;
                }
                int *sellTbl = (int *)G<int>(globals->field_0x3c, 4);
                if (price < sellTbl[idx] || sellTbl[idx] == 0) {
                    sellTbl[idx] = price;
                    int sysIdx = ((SolarSystem *)((void *)(long)((Status *)(*gStatus))->getSystem()))->getIndex();
                    *(int *)(G<int>(G<void *>(*g_hw_globals, 0x44), 4) + idx * 4) = sysIdx;
                }
            }
        }
    }

    ((Ship *)((Ship *)(((Status *)(*gStatus))->getShip())))->adjustPrice();

    void *liw = ::operator new(0x13c);
    ListItemWindow_ctor(liw);
    self->listItemWindow = liw;
    void *cw1 = ::operator new(0x5c);
    ChoiceWindow_ctor(cw1);
    self->viewMode = 0;
    self->choiceWindow = cw1;
    void *cw2 = ::operator new(0x5c);
    ChoiceWindow_ctor(cw2);
    self->dialogActive = 0;
    self->dialog = cw2;

    self->replaceEquipPending = 0;
    self->holdTime = 0;
    self->repeatTimer = 0;
    self->pendingMountItem = 0;
    self->pendingDemountItem = 0;
    self->field_0x130 = 0;
    self->suppressTouchEnd = 0;
    self->autoCompletePending = 0;
    self->autoEquipped = 0;

    int scrH = *g_hw_screenHeight;
    int contentW = *g_hw_screenWidth - 10;
    self->field_0x40 = 0x10;
    self->field_0x44 = 5;
    self->field_0x48 = 5;
    self->contentWidth = contentW;
    self->contentHeight = scrH - 10;

    int *cols = (int *)::operator new[](0xc);
    self->columnWidths = cols;
    int third = __aeabi_idiv(contentW, 3) - 2;
    cols[0] = third;
    cols[1] = third;
    cols[2] = (*g_hw_screenWidth - 0xe) + third * -2;

    ((HangarList *)(self->hangarList))->setCurrentTab(*g_hw_specialModeFlag != 0);
    ((HangarWindow *)(self))->refreshCurrentContentHeight();

    self->currentLoad = ((Ship *)(((Status *)(*gStatus))->getShip()))->getCurrentLoad();
    void *lay2 = *g_hw_layout;
    self->visibleHeight = ((*g_hw_screenHeight - G<int>(lay2, 0x10) - G<int>(lay2, 0xc)) -
                          G<int>(lay2, 0x20)) - G<int>(lay2, 0x24);

    int extra = 0;
    if (*g_hw_blackMarketHintFlag != 0 && *g_hw_introHintFlag == 0) {
        ((PaintCanvas *)*g_hw_canvas)->Image2DCreate((unsigned short)(0x6a4), (unsigned int *)((unsigned int *)((char *)self + 0xf0)));
        extra = (int)((float)(*g_hw_screenWidth) * g_hw_posScale);
    }
    self->selectedItem = 0;
    self->hintOffsetX = extra;
    self->buyMode = 0;
    self->bluePrintPurchasePending = 0;
    self->shipSwapPending = 0;
    self->field_0x92 = 0;

    // Zero the 16-byte block at 0xc1 and the touch-drag block at 0xb4.
    self->field_0xc1 = 0;
    self->field_0xc5 = 0;
    self->field_0xc9 = 0;
    self->field_0xcd = 0;
    self->savedScrollOffset = 0;
    self->field_0x0 = 0;
    self->active = 1;
    self->autoEquipPending = 0;
    self->autoEquipIndex = 0xffffffff;
    self->scrollOffset = 0;
    self->lastTouchY = 0;
    self->scrollOffsetBackup = 0;
    self->scrollDelta = 0;

    // First-visit hint at later campaign stages.
    if (((Status *)(*gStatus))->getCurrentCampaignMission() > 0xd) {
        uint8_t *introFlag = g_hw_introHintFlag;
        if (introFlag[0x4e] == 0) {
            ((GameText *)(*g_hw_helpTextId))->getText();
            ((ChoiceWindow *)(self->dialog))->set(g_HangarWindow_emptyDialogText);
            introFlag[0x4e] = 1;
            ((RecordHandler *)(*g_hw_recordHandler))->saveOptions();
            self->dialogActive = 1;
        }
    }
}

// Two PC-rel globals: a window-open counter (set to 1) and a layout/config singleton
// whose fields 0x238..0x250 are copied into this 0x100..0x118.
__attribute__((visibility("hidden"))) extern int *g_hw_openCounter;
__attribute__((visibility("hidden"))) extern void **g_hw_layout;

// NEAR: NEON block copies match (vld1/vst1.32), but clang schedules the many independent
// field-init stores in a different order and picks r12 (no frame) where the target uses r4
// (push). Structurally correct; instruction ordering differs.

void HangarWindow::ctor() {
    int *cnt = g_hw_openCounter;
    void *lay = *g_hw_layout;

    this->bluePrint = 0;
    this->bluePrintItem = 0;
    this->dialogActive = 0;
    this->bluePrintPurchasePending = 0;
    // zero 4 words at 0x14..0x20 (a zeroed Vector slot) via a 16-byte NEON store
    this->hangarList = 0;
    this->listItemWindow = 0;
    this->choiceWindow = 0;
    this->dialog = 0;
    this->localBluePrint = 0;
    *cnt = 1;
    this->field_0x130 = 0;
    this->dragging = 0;
    this->tabButtons = 0;
    this->buttons = 0;
    this->shipSwapPending = 0;
    this->buyCreditsActive = 0;

    // copy lay[0x238..0x244] (16 bytes) into this[0x100..0x10c]
    this->field_0x100 = *(Blk16 *)((char *)lay + 0x238);
    this->buttonHeight = G<int>(lay, 0x248);
    this->field_0x114 = G<int>(lay, 0x24c);
    this->iconOffsetY = G<int>(lay, 0x250);
}

// ---- statusShip ----
// The hangar always edits the player's current ship, which the global Status owns. The
// decompiler inlined this everywhere as a bare call returning the ship in r0.
void *HangarWindow::statusShip() {
    return ((Status *)(*gStatus))->getShip();
}
