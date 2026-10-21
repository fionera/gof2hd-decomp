#include "game/ui/HangarWindow.h"
#include <algorithm>
#include "game/world/SolarSystem.h"
#include "engine/render/PaintCanvas.h"
#include "game/ui/ChoiceWindow.h"
#include "engine/audio/FModSound.h"
#include "game/core/HangarList.h"
#include "game/mission/Item.h"
#include "game/ui/ListItemWindow.h"
#include "game/mission/BluePrint.h"
#include "engine/render/ImageFactory.h"
#include "game/mission/RecordHandler.h"
#include "game/world/Station.h"
#include "game/core/Globals.h"
#include "game/mission/Status.h"
#include "game/ui/ListItem.h"
#include "game/mission/PendingProduct.h"
#include "game/ship/Ship.h"
#include "engine/core/NFC.h"
#include "engine/core/GameText.h"

static const String g_HangarWindow_emptyDialogText;

void Status_replaceHash(...);

static inline int IDIV(int a, int b) { return a / b; }
static inline unsigned int UDIV(unsigned int a, unsigned int b) { return a / b; }

unsigned char *AppManager_GetApplicationData();

unsigned char *AppManager_GetApplicationModule(unsigned int id);

static inline unsigned char *ApplicationManager_GetApplicationData() { return (unsigned char *)Globals::appManager->GetApplicationData(); }

void Status_changeCredits(void *globals); // lint: void_ptr cross-file shim, mangled param type

float VectorSignedToFloat(int v, int mode);

void TouchButton_ctor_text(void *btn, void *text, int a, int b, int c, char k); // lint: void_ptr cross-file shim, mangled param type

// lint: void_ptr cross-file shim, mangled param type

void TouchButton_ctor_text2(void *btn, void *text, int a, int b, int c, int d, char k); // lint: void_ptr cross-file shim, mangled param type

// lint: void_ptr cross-file shim, mangled param type

void TouchButton_ctor_img(void *btn, void *img, int a, int b, int c, int d, char k, char m); // lint: void_ptr cross-file shim, mangled param type

// lint: void_ptr cross-file shim, mangled param type

void TouchButton_ctor_img2(void *btn, void *imgA, void *imgB, int a, int b, int c, char k); // lint: void_ptr cross-file shim, mangled param type

// lint: void_ptr cross-file shim, mangled param type

void TouchButton_getPosition(void *btn, float *x, float *y); // lint: void_ptr cross-file shim, mangled param type

static Globals **g_hw_globals;
static Layout **g_hw_layout;
static int *g_hw_screenWidth;
static int *g_hw_screenHeight;
static AbyssEngine::ImageFont **g_hw_font;
static RecordHandler **g_hw_recordHandler;
static uint8_t *g_hw_optionFlags;
static unsigned int *g_hw_dlcModuleId;
static uint8_t *g_hw_blackMarketHintFlag;
static uint8_t *g_hw_introHintFlag;
static int *g_hw_headerTextId;
static int *g_hw_shipNameBase;
static int *g_hw_itemNameBase;
static float g_hw_lineScale;
static unsigned int *g_hw_modStationId;
static int *g_hw_bpDoneTextId;
static int *g_hw_notEnoughTextId;
static int *g_hw_helpTextId;
static int *g_hw_sellShipTextId;
static int *g_hw_baseTextId;
static FModSound **g_hw_sound = &Globals::sound;
static int *g_hw_bpTextId;
static int *g_hw_buyTextId;
static int *g_hw_buyTextId2;
static char *g_hw_buyFlag;
static int *g_hw_buyWidth;
static int *g_hw_buyHeight;
static unsigned char **g_hw_itemFlags;
static int *g_hw_sellTextId1;
static int *g_hw_sellTextId2;
static int *g_hw_routesTextId;
static int *g_hw_bpStations;
static int *g_hw_unsaleableTextId;
static int *g_hw_slotMsgTextId;
static int *g_hw_buyBaseTextId;
static int *g_hw_sellMsgTextId1;
static int *g_hw_sellMsgTextId2;
static int *g_hw_equipTextId;
static int *g_hw_freeCreditsTextId;
static int **g_hw_posXArray;
static int **g_hw_posYArray;
static unsigned int **g_hw_imageCountSlot;
static uint8_t *g_hw_specialModeFlag;
static uint8_t *g_hw_listModeFlag;
static float g_hw_posScale;

static const char hw_rnd_empty[1] = "", hw_rnd_a[1] = "", hw_rnd_b[1] = "",
        hw_rnd_c[1] = "", hw_rnd_d[1] = "", hw_rnd_e[1] = "", hw_rnd_x[1] = "";
static const char hw_buy_yes[1] = "", hw_buy_no[1] = "", hw_buy_icon[1] = "";
static const char hw_buy2_yes[1] = "", hw_buy2_no[1] = "", hw_buy2_icon[1] = "";
static const char hw_init_buy[1] = "", hw_init_sell[1] = "", hw_init_lbl[1] = "",
        hw_init_more[1] = "", hw_init_back[1] = "", hw_init_help[1] = "";

bool HangarWindow::isInSpecialMode() {
    if (this->specialMode != 0) return true;
    return this->dialogActive != 0;
}

void HangarWindow::refreshCurrentContentHeight() {
    Array<ListItem *> *items = ((HangarList *) this->hangarList)->getCurrentTabItems();
    if (items != 0) {
        int n = (int) items->size();

        int rowH = reinterpret_cast<int *>(Globals::layout)[0x70 / 4];
        this->currentContentHeight = this->field_0x100.d * (n - 1) + n * rowH;
    }
}

bool HangarWindow::listMode() {
    HangarWindow *self = this;
    if (self->viewMode == 0 && self->specialMode == 0) {
        return self->dialogActive == 0;
    }
    return false;
}

bool HangarWindow::readyToClose() {
    HangarWindow *self = this;
    int tab = self->hangarList->getCurrentTab();
    if (tab == 4 && self->buyMode != 0 && self->bluePrintBuyCount > 0 &&
        self->dialogActive == 0 && self->bluePrint->isEmpty() == 0) {
        int si = self->bluePrint->getStationIndex();
        Station *st = Globals::status->getStation();
        if (si != st->getIndex()) {
            return false;
        }
    }
    return self->dialogActive == 0;
}

void HangarWindow::render3D() {
    if (this->viewMode == 1) {
        this->listItemWindow->render();
    }
}

bool HangarWindow::currentItemIsHighlighted() {
    ListItem *item = (ListItem *) this->hangarList->getCurrentItem();
    if (item == 0) return false;
    return item == this->selectedItem;
}

HangarWindow::~HangarWindow() {
    delete this->hangarList;
    this->hangarList = nullptr;
    delete this->listItemWindow;
    this->listItemWindow = nullptr;
    delete this->choiceWindow;
    this->choiceWindow = nullptr;
    delete this->dialog;
    this->dialog = nullptr;
    if (this->tabButtons != nullptr) {
        ArrayReleaseClasses(*this->tabButtons);
        delete this->tabButtons;
    }
    this->tabButtons = nullptr;
    if (this->buttons != nullptr) {
        ArrayReleaseClasses(*this->buttons);
        delete this->buttons;
    }
    this->buttons = nullptr;
    this->active = 0;
    delete[] (uint32_t *) this->tabIcons;
    this->tabIcons = nullptr;
}

float HangarWindow::getRelativeScrollStartPos() {
    int range = this->scrollOffset;
    if (range > 0) {
        return 0.0f;
    }
    return -(float) range / (float) this->currentContentHeight;
}

void HangarWindow::hideMessage() {
    this->dialogActive = 0;
}

void HangarWindow::render() {
    Layout *layout = *g_hw_layout;
    PaintCanvas *canvas = Globals::Canvas;
    ((PaintCanvas *) canvas)->SetColor(0xffffffffu);

    unsigned char *dlc = AppManager_GetApplicationModule(*g_hw_dlcModuleId);

    bool dlcShown = dlc != 0 && (*(uint8_t *) ((char *) (dlc) + (0x18))) != 0;

    if (dlc == 0 || !dlcShown) {
        int tab2 = this->viewMode;
        if (tab2 == 0) {
            ((Layout *) (layout))->drawBG();
            unsigned int tab = this->hangarList->getCurrentTab();
            Array<ListItem *> *items = ((HangarList *) this->hangarList)->getCurrentTabItems();
            if (items != 0) {
                float startPos = this->getRelativeScrollStartPos();
                float visH = (float) this->visibleHeight;
                float scrollH = this->getRelativeScrollHeight();
                int scrollPx = (int) (scrollH * visH);
                int startPx = (int) (startPos * visH);

                int topY = *g_hw_screenWidth;
                if (scrollPx > 0)
                    topY = (topY - layout->field_0x48) - layout->field_0x2c_rowHeight;

                int rowGap;
                if (tab == 0)
                    rowGap = layout->field_0x4c << 1;
                else if (tab == 4 || tab == 1)
                    rowGap = layout->field_0x50;
                else
                    rowGap = 0;

                topY += (layout->buttonInsetX + this->hintOffsetX) * -2;
                int baseY = layout->field_0x2cc;
                int colW = layout->field_0x4c;

                if (*g_hw_blackMarketHintFlag != 0 && *g_hw_introHintFlag == 0) {
                    int iw = ((PaintCanvas *) canvas)->GetImage2DWidth(0);
                    int ih = ((PaintCanvas *) canvas)->GetImage2DHeight(0);
                    int rows = IDIV(*g_hw_screenHeight, 1);
                    int y = 0;
                    for (int r = 0; r <= rows; r++) {
                        ((PaintCanvas *) canvas)->DrawImage2D((unsigned) this->hintImage,
                                                              (layout->buttonInsetX - iw) + this->hintOffsetX, y,
                                                              (unsigned char) 1);
                        int off = (scrollPx < 1) ? 0 : (layout->field_0x48 + layout->field_0x2c_rowHeight);
                        ((PaintCanvas *) canvas)->DrawImage2D((unsigned) this->hintImage,
                                                              this->hintOffsetX + layout->buttonInsetX + topY + off, y,
                                                              (unsigned char) 0);
                        y += ih;
                    }
                }

                int contentBase = colW + baseY + rowGap;

                Array<TouchButton *> *btnArr = this->buttons;
                for (int i = 0; i != 0x18; i++) {
                    if (this->dragging == 0) {
                        TouchButton *btn = (*this->buttons)[(i * 4) >> 2];
                        if (btn != 0)
                            btn->setVisible(false);
                    }
                }

                int boxW = rowGap - 2;

                for (unsigned int i = 0; i < items->size(); i++) {
                    int y = (layout->field_0x70_rowHeight + this->field_0x100.d) * (int) i +
                            this->scrollOffset + layout->field_0x20_top + layout->field_0xc_leftMargin;
                    if (y < 0 || y > *g_hw_screenHeight)
                        continue;

                    ListItem *li = items->data()[i];
                    if (((ListItem *) (li))->isSelectable() == 0)
                        continue;

                    if (this->selectedItem == li && ((ListItem *) (li))->isTextButton() == 0) {
                        String boxText;
                        if (tab == 0 && ((ListItem *) li)->inTabIndex >= 0) {
                            ((Layout *) (layout))->drawBox(10, this->hintOffsetX + layout->buttonInsetX, y, topY,
                                                           layout->field_0x70_rowHeight, boxText);
                        } else {
                            ((Layout *) (layout))->drawBox(4, this->hintOffsetX + layout->buttonInsetX, y, topY,
                                                           layout->field_0x70_rowHeight, boxText);
                        }
                    } else if (tab != 0 || ((ListItem *) li)->inTabIndex < 0) {
                        String boxText;
                        ((Layout *) (layout))->drawBox(3, this->hintOffsetX + layout->buttonInsetX, y, topY,
                                                       layout->field_0x70_rowHeight, boxText);
                    } else {
                        String boxText;
                        ((Layout *) (layout))->drawBox(9, this->hintOffsetX + layout->buttonInsetX, y, topY,
                                                       layout->field_0x70_rowHeight, boxText);
                    }

                    ((PaintCanvas *) canvas)->SetColor(0xffffffffu);
                    String label;

                    if (((ListItem *) (li))->isItem() == 0) {
                        if (((ListItem *) (li))->isShip() != 0) {
                            ((ListItem *) li)->ship->getIndex();

                            int shipCredits = Globals::status->getCredits();
                            int shipPrice = ((ListItem *) li)->ship->getPrice();
                            unsigned int shipPriceColor = (shipCredits > shipPrice)
                                                              ? 0xa35b5bffu
                                                              : 0x7aa35bffu;
                            ((PaintCanvas *) canvas)->SetColor(shipPriceColor);
                            String price = Globals::layout->formatCredits(((ListItem *) li)->ship->getPrice());
                            ((PaintCanvas *) canvas)->DrawString((unsigned) (uintptr_t) * g_hw_font, price,
                                                                 contentBase + layout->buttonInsetX + this->hintOffsetX,
                                                                 0, (bool) 1);
                            ((ImageFactory *) (*g_hw_globals))->drawShip(
                                ((ListItem *) li)->ship->getIndex(), this->hintOffsetX + layout->buttonInsetX + rowGap,
                                this->iconOffsetY + y);
                            ((PaintCanvas *) canvas)->SetColor(0xffffffffu);
                        } else if (((ListItem *) (li))->isSlot() != 0) {
                            if (tab == 4 && i == items->size() - 1) {
                                (*this->buttons)[(0x5c) >> 2]->setPosition(
                                    this->hintOffsetX + layout->buttonInsetX + topY / 2, layout->field_0x114_strip5c + y, 0x14);
                                (*this->buttons)[(0x5c) >> 2]->setVisible(true);
                                (*this->buttons)[(0x5c) >> 2]->draw();
                                String tmp;
                            }
                        } else if (((ListItem *) (li))->isBluePrint() != 0) {
                            ((ListItem *) li)->bluePrint->getIndex();
                            float rate = ((ListItem *) li)->bluePrint->getCompletionRate();
                            if (rate > 0.0f) {
                                ((PaintCanvas *) canvas)->DrawImage2D((unsigned) this->progressBarBgImage,
                                                                      layout->buttonInsetX + contentBase + 2 + this->
                                                                      hintOffsetX, 0, (unsigned char) 0);
                                float dcw = (float) this->progressBarWidth;
                                ((PaintCanvas *) canvas)->DrawRegion2D((unsigned) this->progressBarFillImage, 0, 0,
                                                                       (int) (rate * dcw), this->progressBarHeight,
                                                                       (float) (int) (rate * dcw),
                                                                       0, 0, 0,
                                                                       layout->buttonInsetX + contentBase + 3 + this->
                                                                       hintOffsetX);
                                float sliderW = (float) (this->progressBarWidth - 4);
                                ((PaintCanvas *) canvas)->DrawImage2D((unsigned) this->progressBarBorderImage,
                                                                      this->hintOffsetX + layout->buttonInsetX +
                                                                      contentBase + 5 + (int) (rate * sliderW),
                                                                      (layout->field_0x70_rowHeight / 2) + 2 + y,
                                                                      (unsigned char) 0x11);
                                ((PaintCanvas *) canvas)->SetColor(0x777777ffu);

                                String pct, sfx, sum;
                                pct.Set((long long) (int) (rate * 100.0f));
                                sum = pct + sfx;
                                ((PaintCanvas *) canvas)->DrawString((unsigned) (uintptr_t) * g_hw_font, sum,
                                                                     contentBase + 2 + layout->buttonInsetX + this->
                                                                     hintOffsetX +
                                                                     this->progressBarWidth + layout->field_0x2c_rowHeight, 0,
                                                                     (bool) 0);
                                ((PaintCanvas *) canvas)->SetColor(0xffffffffu);
                            }
                            int bpIdx = ((ListItem *) li)->bluePrint->getIndex();

                            int type = ((Item *) ((*(Item * *) (
                                        (char *) ((*(unsigned char * *) ((char *) (*g_hw_globals) + (0x4)))) + (
                                            bpIdx)))))->
                                    getType();
                            ((ImageFactory *) (*g_hw_globals))->drawItem(
                                bpIdx, type, layout->buttonInsetX + rowGap + this->hintOffsetX);

                            if (((ListItem *) li)->craftable != 0)
                                ((PaintCanvas *) canvas)->SetColor(0x00ed00ffu);
                        } else if (((ListItem *) (li))->isPendingProduct() != 0) {
                            int amt = ((ListItem *) li)->pendingProduct->quantity;
                            String head;
                            if (amt < 2) {
                            } else {
                                String num, sfx;
                                head = num + sfx;
                            }
                            String full;
                            full = head + *(String *) Globals::gameText->getText(*g_hw_itemNameBase);
                            int pidx = ((ListItem *) li)->pendingProduct->blueprintIndex;
                            int type = ((Item *) ((*(Item * *) (
                                        (char *) ((*(unsigned char * *) ((char *) (*g_hw_globals) + (0x4)))) + (
                                            pidx)))))->
                                    getType();
                            ((ImageFactory *) (*g_hw_globals))->drawItem(
                                pidx, type, rowGap + layout->buttonInsetX + this->hintOffsetX);
                        } else if (((ListItem *) (li))->isMoveToCargoButton() != 0) {
                            (*this->buttons)[(0x18) >> 2]->setPosition(this->hintOffsetX + layout->buttonInsetX, y, 0x11);
                            (*this->buttons)[(0x18) >> 2]->setVisible(true);
                            (*this->buttons)[(0x18) >> 2]->draw();
                        } else if (((ListItem *) (li))->isSellButton() != 0) {
                            (*this->buttons)[(0x14) >> 2]->setPosition(this->hintOffsetX + layout->buttonInsetX, y, 0x11);
                            (*this->buttons)[(0x14) >> 2]->setVisible(true);
                            (*this->buttons)[(0x14) >> 2]->draw();
                        } else {
                            String txt;
                            ((String *) &txt)->Set(((String *) ((ListItem *) li)->name)->data);
                            ((Layout *) (layout))->drawBox(0, this->hintOffsetX + layout->buttonInsetX,
                                                           (y + layout->field_0x70_rowHeight) - layout->field_0x1c, topY,
                                                           layout->field_0x1c, txt);
                        }
                    } else {
                        ((Item *) ((ListItem *) li)->item)->getIndex();
                        ((PaintCanvas *) canvas)->SetColor(0xffffffffu);

                        if (tab == 1) {
                            int itemCredits = Globals::status->getCredits();
                            int itemPrice = ((Item *) ((ListItem *) li)->item)->getSinglePrice();
                            ((PaintCanvas *) canvas)->SetColor((itemCredits > itemPrice)
                                                                   ? 0xa35b5bffu
                                                                   : 0x7aa35bffu);
                        } else {
                            ((PaintCanvas *) canvas)->SetColor(0x777777ffu);
                        }
                        if (this->upgradeMode == 0) {
                            String price = Globals::layout->formatCredits(((Item *) ((ListItem *) li)->item)->getSinglePrice());
                            ((PaintCanvas *) canvas)->DrawString((unsigned) (uintptr_t) * g_hw_font, price,
                                                                 contentBase + layout->buttonInsetX + this->hintOffsetX,
                                                                 0, (bool) 1);
                        }
                        int iidx = ((Item *) ((ListItem *) li)->item)->getIndex();
                        int itype = ((Item *) ((ListItem *) li)->item)->getType();
                        ((ImageFactory *) (*g_hw_globals))->drawItem(iidx, itype,
                                                                     layout->buttonInsetX + rowGap + this->hintOffsetX);
                    }

                    ((PaintCanvas *) canvas)->DrawString((unsigned) (uintptr_t) * g_hw_font, label,
                                                         this->hintOffsetX + layout->buttonInsetX + contentBase, 0,
                                                         (bool) 0);
                }

                if (scrollPx > 0 || startPx > 0) {
                    ((Layout *) (layout))->drawScrollBar(
                        ((*g_hw_screenHeight - layout->field_0x48) - layout->buttonInsetX) -
                        this->hintOffsetX, layout->field_0x20_top + layout->field_0xc_leftMargin, this->visibleHeight, startPx,
                        scrollPx);
                }
            }

            String header;
            ((Layout *) (layout))->drawHeader(header);

            Array<TouchButton *> *tabs = this->tabButtons;
            for (unsigned int i = 0; i < tabs->size(); i++)
                ((TouchButton *) (tabs->data()[i]))->draw();
        }

        if (this->viewMode == 1) {
            this->viewMode = 0;
            this->render();
            this->viewMode = 1;
            this->listItemWindow->draw();
        }
    }

    layout = *g_hw_layout;
    ((Layout *) (layout))->drawFooter();
    Array<TouchButton *> *btns = this->buttons;
    (*btns)[(0x2c) >> 2]->setVisible(true);
    (*btns)[(0x2c) >> 2]->setAlwaysPressed(g_hw_optionFlags[0x4e] == 0);
    {
        String credits = Globals::layout->formatCredits(Globals::status->getCredits());
        (*btns)[(0x2c) >> 2]->setText(credits);
    }
    (*btns)[(0x2c) >> 2]->draw();

    if (this->dialogActive == 0)
        return;

    this->dialog->draw();

    if (this->buyCreditsActive == 0) {
        if (this->freeCreditsActive != 0) {
            for (unsigned int i = 0; i < 5; i++) {
                TouchButton *b = (*btns)[(i * 4 + 0x48) >> 2];
                bool vis;
                if (i == 0 || (i == 1 && g_hw_optionFlags[0x49] != 0) ||
                    (i == 2 && g_hw_optionFlags[0x4a] != 0) ||
                    (i == 3 && g_hw_optionFlags[0x4b] != 0))
                    vis = false;
                else
                    vis = (i != 4 || g_hw_optionFlags[0x4c] == 0);
                b->setVisible(vis);
                b->setPosition(layout->buttonInsetX + ((ChoiceWindow *) this->dialog)->x,
                               layout->windowTopInset);
                b->draw();
            }
        }
    } else {
        unsigned char *appData = AppManager_GetApplicationData();
        (void) appData;
        if (this->listModeFlag == 0) {
            for (int slot = 0x30; slot <= 0x40; slot += 4) {
                TouchButton *b = (*btns)[(slot) >> 2];
                b->setVisible(true);
                String t;
                b->setText(t);
                b->setPosition(*g_hw_screenWidth / 2, 0, 0x14);
                b->draw();
            }
        } else {
            for (unsigned int i = 0; i <= 4; i++) {
                TouchButton *b = (*btns)[(i * 4 + 0x30) >> 2];
                b->setVisible(true);
                String label, split;
                switch (i) {
                    case 0: ((String *) &label)->ctor_char(hw_rnd_a, false);
                        ((String *) &split)->ctor_char(hw_rnd_b, false);
                        break;
                    case 1: ((String *) &label)->ctor_char(hw_rnd_b, false);
                        ((String *) &split)->ctor_char(hw_rnd_c, false);
                        break;
                    case 2: ((String *) &label)->ctor_char(hw_rnd_c, false);
                        ((String *) &split)->ctor_char(hw_rnd_d, false);
                        break;
                    case 3: ((String *) &label)->ctor_char(hw_rnd_d, false);
                        ((String *) &split)->ctor_char(hw_rnd_e, false);
                        break;
                    default: ((String *) &label)->ctor_char(hw_rnd_e, false);
                        ((String *) &split)->ctor_char(hw_rnd_x, false);
                        break;
                }
                unsigned int rowOff = UDIV(i & 0xff, 3);
                int x = (this->gridSpacingX + this->buttonWidth) *
                        ((i + rowOff * -3) & 0xff) +
                        ((*g_hw_screenHeight / 2 - this->buttonWidth) - this->gridSpacingX);
                int yy = (int) ((float) ((layout->field_0x20_top * -3) +
                                         (*g_hw_screenWidth / 2 - this->gridButtonHeight / 2)) +
                                (float) this->gridSpacingY * -0.5f +
                                (float) ((int) rowOff * (this->gridSpacingY + this->gridButtonHeight)));
                ((TouchButton *) (b))->setPosition(x, yy, 'D');
                ((TouchButton *) (b))->replaceTextKeepSize(label);
                ((TouchButton *) (b))->setSplitText(split);
                ((TouchButton *) (b))->draw();

                ((PaintCanvas *) canvas)->DrawImage2D((unsigned) (*(int *) ((char *) (this->tabIcons) + (i * 4))), x,
                                                      yy - layout->field_0x2c_rowHeight, (unsigned char) 0x11);
            }
        }

        if (this->listModeFlag == 0) {
            int h17 = layout->field_0x30_rowHeight;
            int h34 = layout->field_0x34;
            int th = ((PaintCanvas *) canvas)->GetTextHeight(0);
            this->dialog->setHeight((h34 + h17) * 6 + th * 2);
        }
    }
}

void HangarWindow::OnTouchEnd(int touch, int coord) {
    HangarWindow *self = this;
    Globals *globals = (Globals *) *g_hw_globals;
    self->holdTime = 0;
    self->repeatTimer = 0;
    self->dragging = 0;
    if (self->suppressTouchEnd != 0) {
        self->suppressTouchEnd = 0;
        return;
    }

    if (self->dialogActive == 0) {
        if (self->viewMode == 1)
            self->listItemWindow->OnTouchEnd(touch, coord);

        Layout *layout = *g_hw_layout;
        int handled = ((Layout *) (layout))->OnTouchEnd(touch, coord);
        if (handled == 0) {
            int delta = self->scrollDelta;
            int newScroll = self->scrollOffset + delta;
            float vel = (float) delta;
            int absd = delta < 0 ? -delta : delta;
            self->velocity = (absd > 3) ? (int) vel : 0;
            self->damping = 0;
            self->scrollOffset = newScroll;
            self->scrollOffsetBackup = newScroll;

            Array<TouchButton *> *tabs = self->tabButtons;
            for (unsigned int i = 0; i < tabs->size(); i++) {
                if (((TouchButton *) (tabs->data()[i]))->OnTouchEnd(touch, coord) != 0) {
                    self->setSellMode(true);
                    self->selectedItem = 0;
                    self->hangarList->setCurrentTab(0, i != 0);
                    if (i == 2)
                        self->refreshCargoAvailabilityForBlueprints();
                    self->refreshCurrentContentHeight();
                    self->scrollOffset = 0;
                    self->scrollOffsetBackup = 0;
                    self->hangarList->setCurrentItemIndex(-1);
                }
            }

            if (layout->field_0xc_leftMargin < coord) {
                int row = IDIV(
                    (((coord - layout->field_0xc_leftMargin) - layout->field_0x20_top) - self->field_0x100.d) -
                    self->scrollOffset,
                    layout->field_0x70_rowHeight + self->field_0x100.d);
                if (row < self->hangarList->getCurrentLength()) {
                    self->hangarList->setCurrentItemIndex(row);
                    if (self->currentItemIsHighlighted() != 0 &&
                        self->sellConfirmPending != 0) {
                        self->setSellMode(true);
                    }
                }
            }
            if (self->sellConfirmPending != 0) {
                self->sellConfirmPending = 0;
                return;
            }

            if ((*self->buttons)[(0x5c) >> 2]->OnTouchEnd(touch, coord) != 0) {
                self->bluePrint->getAutoCompletionPrice();
                String line, priceStr, fmt, msg;
                priceStr = Globals::layout->formatCredits(0);
                Status_replaceHash(&msg, globals, &line, &priceStr);
                self->dialog->set(*(String *) &msg, true);
                self->autoCompletePending = 1;
                self->dialogActive = 1;
            }

            if (self->currentItemIsHighlighted() != 0) {
                Array<TouchButton *> *btns = self->buttons;
                for (unsigned int i = 0; i < btns->size(); i++) {
                    if (((TouchButton *) (btns->data()[i]))->OnTouchEnd(touch, coord) != 0) {
                        if ((i & 0x7fffffff) < 0xc)
                            return;
                    }
                }
            }

            if (((Layout *) (layout))->helpPressed() != 0) {
                String help;
                if (self->viewMode == 1) {
                    ((Layout *) (layout))->initHelpWindow(help);
                } else {
                    unsigned int t = self->hangarList->getCurrentTab();
                    if (t <= 4) {
                        ((Layout *) (layout))->initHelpWindow(help);
                    }
                }
            }

            if ((*self->buttons)[(0x2c) >> 2]->OnTouchEnd(touch, coord) != 0) {
                g_hw_optionFlags[0x4e] = 1;
                (*g_hw_recordHandler)->saveOptions();
                self->showCreditsBuyWindow();
            }
            return;
        }

        if (self->viewMode == 1) {
            ((Layout *) (layout))->resetWindowDimensions();
            self->viewMode = 0;
            return;
        }

        unsigned int tab = self->hangarList->getCurrentTab();
        if (tab == 4) {
            self->setSellMode(true);
            self->selectedItem = 0;
            self->hangarList->setCurrentTab(0, true);
            self->refreshCargoAvailabilityForBlueprints();
            self->refreshCurrentContentHeight();
            self->scrollOffset = 0;
            self->scrollOffsetBackup = 0;
            self->hangarList->setCurrentItemIndex(-1);
            return;
        }
        if (self->hangarList->getCurrentTab() == 3) {
            self->hangarList->setCurrentTab(0, false);
            self->refreshCargoAvailabilityForBlueprints();
            self->refreshCurrentContentHeight();
        } else if (self->readyToClose() != 0) {
            self->setSellMode(true);
            self->selectedItem = 0;
            self->hangarList->setCurrentItemIndex(-1);
        }
        return;
    }

    if (self->autoCompletePending != 0) {
        int r = self->dialog->OnTouchEnd(touch, coord);
        if (r == 1) {
            self->dialogActive = 0;
        } else if (r == 0) {
            int price = self->bluePrint->getAutoCompletionPrice();
            if (Globals::status->getCredits() < price) {
                String line, priceStr, fmt, msg, suffix, combined;
                priceStr = Globals::layout->formatCredits(Globals::status->getCredits());
                Status_replaceHash(&msg, globals, &line, &priceStr);
                Globals::gameText->getText(*g_hw_notEnoughTextId);
                combined = suffix + suffix;
                *((String *) &msg) += combined;
                self->dialog->set(*(String *) &msg, true);
                self->notEnoughCredits = 1;
            } else {
                self->dialogActive = 0;
                if (self->bluePrint != 0) {
                    if (self->bluePrint->isEmpty() != 0) {
                        ((BluePrint *) self->bluePrint)->stationIndex = Globals::status->getStation()->getIndex();
                        Globals::status->getStation()->getName();
                    }
                    self->bluePrint->complete();
                    self->highlightItem(self->hangarList->getCurrentItemAt(1));
                    self->buyMode = 1;
                    self->setSellMode(true);
                    Status_changeCredits(globals);
                }
            }
        }
        self->autoCompletePending = 0;
        return;
    }

    if (self->replaceEquipPending != 0) {
        int r = self->dialog->OnTouchEnd(touch, coord);
        if (r == 1) {
            self->dialogActive = 0;
            self->replaceEquipPending = 0;
            self->scrollOffset = self->savedScrollOffset;
        } else if (r == 0 && self->pendingMountItem != 0 && self->pendingDemountItem != 0) {
            self->demountItem(self->pendingDemountItem, -1);
            self->savedScrollOffset = self->scrollOffset;
            self->mountItem(self->pendingMountItem);
            self->dialogActive = 0;
            self->replaceEquipPending = 0;
        }
    } else if (self->notEnoughCredits != 0) {
        int r = self->dialog->OnTouchEnd(touch, coord);
        if (r == 1) {
            self->notEnoughCredits = 0;
            self->dialogActive = 0;
        } else if (r == 0) {
            g_hw_optionFlags[0x4e] = 1;
            (*g_hw_recordHandler)->saveOptions();
            self->showCreditsBuyWindow();
        }
    } else if (self->buyCreditsActive == 0) {
        if (self->freeCreditsActive != 0) {
            int r = self->dialog->OnTouchEnd(touch, coord);
            if (r == 0) {
                for (int i = 0x12; i != 0x17; i++)
                    (*self->buttons)[(i * 4) >> 2]->setVisible(false);
                self->freeCreditsActive = 0;
                self->showCreditsBuyWindow();
            }

            unsigned char *appData = AppManager_GetApplicationData();
            RecordHandler *rh = *g_hw_recordHandler;
            for (unsigned int i = 0; i != 5; i++) {
                if ((*self->buttons)[(i * 4 + 0x48) >> 2]->OnTouchEnd(touch, coord) != 0) {
                    switch (i) {
                        case 0:
                            ((RecordHandler *) (rh))->recordStoreWrite(0);
                            ((RecordHandler *) (rh))->recordStoreWritePreview(0);
                            break;
                        case 1:
                            (*(uint8_t *) ((char *) (appData) + (0xa0))) = 1;
                            NFC().free_credits_likeGOF2OnFacebook();
                            Status_changeCredits(globals);
                            g_hw_optionFlags[0x49] = 1;
                            break;
                        case 2:
                            (*(uint8_t *) ((char *) (appData) + (0xa1))) = 1;
                            NFC().free_credits_likeFishlabsOnFacebook();
                            Status_changeCredits(globals);
                            g_hw_optionFlags[0x4a] = 1;
                            break;
                        case 3:
                            (*(uint8_t *) ((char *) (appData) + (0xa2))) = 1;
                            NFC().free_credits_subscribeToYoutubeChannel();
                            Status_changeCredits(globals);
                            g_hw_optionFlags[0x4b] = 1;
                            break;
                        case 4:
                            (*(uint8_t *) ((char *) (appData) + (0xa3))) = 1;
                            NFC().free_credits_followOnTwitter();
                            Status_changeCredits(globals);
                            g_hw_optionFlags[0x4c] = 1;
                            break;
                        case 5:
                            (*(uint8_t *) ((char *) (appData) + (0xd))) = 1;
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
            int r = self->dialog->OnTouchEnd(touch, coord);
            int cost = ((Item *) ((ListItem *) self->bluePrintItem)->item)->getBlueprintAmount() * 200;
            bool revert = true;
            if (r == 0 && Globals::status->getCredits() >= cost && self->localBluePrint == 0) {
                Status_changeCredits(globals);
                self->setSellMode(true);
                self->selectedItem = 0;
                self->hangarList->setCurrentItemIndex(-1);
                self->localBluePrint = 0;
                self->bluePrintPurchasePending = 0;
                revert = false;
            } else if (r == 1 && Globals::status->getCredits() >= cost && self->localBluePrint == 0) {
                revert = false;
            }
            if (revert) {
                ((Item *) ((ListItem *) self->bluePrintItem)->item)->setStationAmount(self->savedStationAmount);
                ((Item *) ((ListItem *) self->bluePrintItem)->item)->setAmount(self->savedAmount);
                ((Item *) ((ListItem *) self->bluePrintItem)->item)->setBlueprintAmount(self->savedBlueprintAmount);
                ((Status *) (globals))->setCredits(self->savedCredits);
                self->savedStationAmount = 0;
                self->bluePrintBuyCount = 0;
                self->selectedItem = 0;
                self->savedAmount = 0;
                self->savedBlueprintAmount = 0;
                self->currentLoad = self->savedLoad;
                self->hangarList->setCurrentItemIndex(-1);
                self->dialogActive = 0;
                self->bluePrintPurchasePending = 0;
                self->buyMode = 0;
                if (Globals::status->getCredits() < cost && self->localBluePrint == 0) {
                    String line, priceStr, fmt, msg;
                    priceStr = Globals::layout->formatCredits(Globals::status->getCredits());
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    self->dialog->set(g_HangarWindow_emptyDialogText);
                    self->dialogActive = 1;
                }
                self->localBluePrint = 0;
            }
            self->refreshCurrentContentHeight();
            return;
        }

        if (self->sellShipPending != 0) {
            int r = self->dialog->OnTouchEnd(touch, coord);
            if (r == 1) {
                self->sellShipPending = 0;
                self->dialogActive = 0;
            } else if (r == 0) {
                self->sellShipPending = 0;
                self->dialogActive = 0;
                ((ListItem *) (self->selectedItem))->getPrice();
                Status_changeCredits(globals);
                Globals::status->getStation()->removeShip((Ship *) Globals::status->getShip());
                self->hangarList->initShopTab((Array<Item *> *) (self->itemList),
                                              Globals::status->getStation()->getShips());
                self->refreshCurrentContentHeight();
            }
            return;
        }

        int r = self->dialog->OnTouchEnd(touch, coord);
        bool special = globals->field_0x114 == 3 && self->upgradeMode == 0;
        if (special && self->dlcMenuPending != 0 && r == 1) {
            int idx = globals->field_0x14c;
            Globals::status->getShip()->getIndex();
            if (((Station *) (uintptr_t) idx)->hasShip(Globals::status->getShip()->getIndex()) == 0) {
                int price = ((ListItem *) self->selectedItem)->ship->getPrice();
                if (Globals::status->getCredits() < price) {
                    String line, priceStr, fmt, msg, suffix, combined;
                    ((ListItem *) self->selectedItem)->ship->getPrice();
                    priceStr = Globals::layout->formatCredits(Globals::status->getCredits());
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    Globals::gameText->getText(*g_hw_sellShipTextId);
                    combined = suffix + suffix;
                    *((String *) &msg) += combined;
                    self->dialog->set(*(String *) &msg, true);
                    self->dialogActive = 1;
                    self->notEnoughCredits = 1;
                    self->shipSwapPending = 0;
                    return;
                }
            }
        }

        {
            ChoiceWindow *swapDialog = self->dialog;

            int swapResult = swapDialog->OnTouchEnd(touch, coord);
            if (swapResult == 1) {
                self->dialogActive = 0;
                self->replaceEquipPending = 0;
                self->scrollOffset = self->savedScrollOffset;
            } else if (swapResult == 0 && self->pendingMountItem != 0 &&
                       self->pendingDemountItem != 0) {
                self->demountItem(self->pendingDemountItem, -1);
                self->savedScrollOffset = self->scrollOffset;
                self->mountItem(self->pendingMountItem);
                self->dialogActive = 0;
                self->replaceEquipPending = 0;
            }

            uint8_t buying = self->buyMode;
            int buyResult = swapDialog->OnTouchEnd(touch, coord);
            if (buying != 0) {
                if (buyResult == 1) {
                    self->buyMode = 0;
                    self->dialogActive = 0;
                } else {
                    if (buyResult != 0)
                        return;
                    if (self->routeWarningPending == 0) {
                        self->dialogActive = 0;
                        if (self->autoEquipped == 0) {
                            self->buyMode = 1;
                        } else {
                            Array<TouchButton *> *tabs = self->tabButtons;
                            for (unsigned int i = 0; i < tabs->size(); i++)
                                ((TouchButton *) (tabs->data()[i]))->resetTouch();
                        }
                        return;
                    }
                    self->dialogActive = 0;
                    self->routeWarningPending = 0;
                    self->buyMode = 0;
                }
                self->selectedItem = 0;
                self->hangarList->setCurrentItemIndex(-1);
            }
        }
        return;
    } else {
        int r = self->dialog->OnTouchEnd(touch, coord);
        if (r != 0) {
            Array<TouchButton *> *btns = self->buttons;
            for (unsigned int i = 0; i < 5; i++) {
                if ((*btns)[(i * 4 + 0x30) >> 2]->OnTouchEnd(touch, coord) != 0) {
                    switch (i) {
                        case 0: NFC().iap_buy_credits_300_000();
                            break;
                        case 1: NFC().iap_buy_credits_1_000_000();
                            break;
                        case 2: NFC().iap_buy_credits_3_000_000();
                            break;
                        case 3: NFC().iap_buy_credits_10_000_000();
                            break;
                        default: NFC().iap_buy_credits_100_000();
                            break;
                    }
                }
            }
            if ((*btns)[(0x44) >> 2]->OnTouchEnd(touch, coord) != 0) {
                bool show = true;
                if (g_hw_optionFlags[0x4a] && g_hw_optionFlags[0x49] &&
                    g_hw_optionFlags[0x4d] && g_hw_optionFlags[0x4c])
                    show = (g_hw_optionFlags[0x4b] == 0);
                unsigned char *appData = AppManager_GetApplicationData();
                if (show || (*(uint8_t *) ((char *) (appData) + (0x15))) == 0) {
                    for (int i = 0xc; i != 0x11; i++)
                        (*btns)[(i * 4) >> 2]->setVisible(false);
                    self->showFreeCreditsWindow();
                }
            }
            return;
        }
        self->buyCreditsActive = 0;
        self->dialogActive = 0;
        for (int i = 0xc; i != 0x11; i++)
            (*self->buttons)[(i * 4) >> 2]->setVisible(false);
        (*self->buttons)[(0x44) >> 2]->setVisible(false);
        unsigned char *appData = AppManager_GetApplicationData();
        (*(uint8_t *) ((char *) (appData) + (0x40))) = 0;
        unsigned char *mod = AppManager_GetApplicationModule(*g_hw_modStationId);
        if (mod != 0 && (*(uint8_t *) ((char *) (mod) + (0x18))) != 0)
            (*(uint8_t *) ((char *) (mod) + (0x18))) = 0;
        return;
    }

    uint8_t buying = self->buyMode;
    int r2 = self->dialog->OnTouchEnd(touch, coord);
    if (buying != 0) {
        if (r2 == 1) {
            self->buyMode = 0;
            self->dialogActive = 0;
        } else if (r2 == 0) {
            if (self->routeWarningPending == 0) {
                self->dialogActive = 0;
                if (self->autoEquipped == 0) {
                    self->buyMode = 1;
                } else {
                    Array<TouchButton *> *tabs = self->tabButtons;
                    for (unsigned int i = 0; i < tabs->size(); i++)
                        ((TouchButton *) (tabs->data()[i]))->resetTouch();
                }
                return;
            }
            self->dialogActive = 0;
            self->routeWarningPending = 0;
            self->buyMode = 0;
        } else {
            return;
        }
        self->selectedItem = 0;
        self->hangarList->setCurrentItemIndex(-1);
        return;
    }
    if (r2 != 0)
        return;
    self->dialogActive = 0;
    unsigned char *mod = AppManager_GetApplicationModule(*g_hw_dlcModuleId);
    if ((*(uint8_t *) ((char *) (mod) + (0x18))) != 0)
        (*(uint8_t *) ((char *) (mod) + (0x18))) = 0;
}

void HangarWindow::update(int delta) {
    if (this->active == 0)
        return;
    this->lastDelta = delta;

    if (this->viewMode == 1) {
        this->listItemWindow->update(delta);
        return;
    }

    unsigned int tab = this->hangarList->getCurrentTab();
    for (unsigned int i = 0; i < this->tabButtons->size(); i++) {
        (*this->tabButtons)[i]->setAlwaysPressed(i == tab || (tab == 3 && i == 0) || (i == 2 && tab == 4));
    }

    if (this->dragging == 0) {
        this->velocity = this->damping * this->velocity;
        float speed = this->velocity > 0.0f ? this->velocity : -this->velocity;
        if (speed > 1.0f) {
            this->scrollOffset = (int) (this->velocity + (float) this->scrollOffset);
        }
    }

    if (this->scrollOffset > 0) {
        this->damping = 1.0f;
        this->velocity = (float) -this->scrollOffset * 0.5f;
    }

    if (this->hangarList->getCurrentTabItems() != 0) {
        int minOffset = this->visibleHeight - this->currentContentHeight;
        if (minOffset > -1) {
            this->scrollOffset = 0;
            this->velocity = 0;
        } else if (minOffset > this->scrollOffset) {
            this->damping = 1.0f;
            this->velocity = (float) (minOffset - this->scrollOffset) * 0.5f;
        }
    }

    if (this->buyMode != 0 && ((*this->buttons)[8]->isTouched() || (*this->buttons)[9]->isTouched())) {
        this->holdTime += delta;
        this->repeatTimer += delta;
        if (this->repeatTimer > (this->holdTime > 1500 ? 30 : 200)) {
            this->repeatTimer = 0;
            if (this->buyMode != 0 || this->specialMode != 0) {
                if ((*this->buttons)[9]->isTouched() && (*this->buttons)[9]->isVisible()) {
                    for (int n = this->holdTime > 4000 ? 5 : 1; n != 0; n--)
                        this->transaction(true);
                } else if ((*this->buttons)[8]->isTouched() && (*this->buttons)[8]->isVisible()) {
                    for (int n = this->holdTime > 4000 ? 5 : 1; n != 0; n--)
                        this->transaction(false);
                }
            }
        }
    }
}

int HangarWindow::highlightItem(ListItem *item) {
    if (item != nullptr && item->isSelectable() != 0) {
        (*g_hw_sound)->play(0x7c, nullptr, nullptr, 0);
        unsigned flag = 0;
        if (this->selectedItem != item) {
            flag = item->isTextButton() ^ 1;
        }
        this->sellConfirmPending = flag;
        this->selectedItem = item;
        if (item->isShip() != 0) {
            this->selectedItem->ship->adjustPrice();
        }
    }
    return 0;
}

void HangarWindow::demountItem(Item *item, int slot) {
    int type = item->getType();
    int amount = 1;
    if (type == 1)
        amount = item->getAmount();

    Item *made = item->makeItem(amount);
    Globals::status->getShip()->addCargo(made);

    Ship *ship = Globals::status->getShip();
    if (slot >= 0)
        ship->freeSlot(item, slot);
    else
        ship->freeSlot(item);

    {
        unsigned int i = 0;
        while (i < this->itemList->size()) {
            Item *cur = this->itemList->data()[i];
            int curIndex = cur->getIndex();
            int madeIndex = made->getIndex();
            i++;
            if (curIndex == madeIndex) {
                cur->changeAmount(made->getAmount());
                goto demount_found;
            }
        }
        ArrayAdd(made, *(this->itemList));
        demount_found:;
    }

    Globals::status->getShip()->setCargo(Item::extractItems(this->itemList, true));

    if (this->itemList != 0) {
        ArrayReleaseClasses(*this->itemList);
        delete this->itemList;
    }
    this->itemList = 0;

    this->itemList = Item::mixItems(Globals::status->getShip()->getCargo(), Globals::status->getStation()->getItems());
    this->hangarList->initShipTab(Globals::status->getShip());

    HangarList *hangarList = this->hangarList;
    ItemArray *items = Item::mixItems(Globals::status->getShip()->getCargo(),
                                      Globals::status->getStation()->getItems());
    hangarList->initShopTab(items, Globals::status->getStation()->getShips());
    this->hangarList->setCurrentTab(0, true);

    refreshCurrentContentHeight();
    this->scrollOffset = this->savedScrollOffset;
    (*g_hw_sound)->play(0x60, nullptr, nullptr, 0.0f);
}

int HangarWindow::OnTouchBegin(int touch, int coord) {
    this->holdTime = 0;
    this->repeatTimer = 0;
    bool handled = Globals::layout->OnTouchBegin(touch, coord);

    if (this->dialogActive != 0) {
        if (this->buyCreditsActive != 0) {
            for (int i = 0; i < 5; i++) {
                (*this->buttons)[12 + i]->OnTouchBegin(touch, coord);
            }
            (*this->buttons)[17]->OnTouchBegin(touch, coord);
        } else if (this->freeCreditsActive != 0) {
            for (int i = 0; i < 5; i++) {
                (*this->buttons)[18 + i]->OnTouchBegin(touch, coord);
            }
        }
        this->dialog->OnTouchBegin(touch, coord);
        return 0;
    }

    this->lastTouchY = coord;
    this->touchStartY = coord;
    this->scrollDelta = 0;
    this->dragging = 1;

    if (this->viewMode == 1) {
        this->listItemWindow->OnTouchBegin(touch, coord);
        return 0;
    }

    bool outsideList = true;
    Layout *layout = Globals::layout;
    if (layout->field_0xc_leftMargin < coord && coord < Globals::h - layout->field_0x10_rightMargin) {
        int index = (coord - layout->field_0xc_leftMargin - layout->field_0x20_top - this->field_0x100.d -
                     this->scrollOffset) / (layout->field_0x70_rowHeight + this->field_0x100.d);
        if (index < (int) this->hangarList->getCurrentLength()) {
            this->hangarList->setCurrentItemIndex(index);
            this->highlightItem(this->hangarList->getCurrentItem());
            outsideList = false;
            if (this->upgradeMode && this->hangarList->getCurrentItem()->isShip() &&
                this->hangarList->getCurrentTab() == 1) {
                Globals::status->getShip()->setCargo(Item::extractItems(this->itemList, true));
                Globals::status->getStation()->setItems(Item::extractItems(this->itemList, false), false);
            }
        }
    }

    if (this->hangarList->getCurrentTab() == 4 && this->buyMode != 0 &&
        (this->selectedItem != this->bluePrintItem || outsideList) && this->bluePrintBuyCount >= 1 &&
        this->dialogActive == 0 && !this->bluePrint->isEmpty() &&
        this->bluePrint->getStationIndex() != Globals::status->getStation()->getIndex()) {
        this->localBluePrint = this->bluePrintItem->item->getIndex() == 209 ||
                               this->bluePrintItem->item->getIndex() == 204;
        String msg;
        msg = this->localBluePrint ? String(*Globals::gameText->getText(289))
                                   : String(*Globals::gameText->getText(288));
        if (!this->localBluePrint) {
            msg = Globals::status->replaceHash(msg, String(this->bluePrint->getStationName(), false),
                                               String("#S"));
            msg = Globals::status->replaceHash(
                    msg,
                    String(Globals::layout->formatCredits(this->bluePrintItem->item->getBlueprintAmount() * 200),
                           false),
                    String("#C"));
        }
        this->dialog->set(msg, !this->localBluePrint);
        this->dialogActive = 1;
        this->bluePrintPurchasePending = 1;
        this->suppressTouchEnd = 1;
        return 0;
    }

    for (unsigned int i = 0; i < this->tabButtons->size(); i++) {
        handled |= (*this->tabButtons)[i]->OnTouchBegin(touch, coord);
    }
    for (unsigned int i = 0; i < this->buttons->size(); i++) {
        if ((*this->buttons)[i] != 0) {
            (*this->buttons)[i]->OnTouchBegin(touch, coord);
        }
    }

    if (this->autoEquipPending != 0 && this->hangarList->getCurrentTab() == 1 && (int) this->autoEquipIndex >= 0 &&
        !((int) this->autoEquipIndex == (int) this->hangarList->getCurrentItemIndex() && !handled)) {
        this->autoEquipPending = 0;
        this->autoEquipSecondaryWeapons(this->autoEquipIndex);
    }
    return 0;
}

void HangarWindow::showCreditsBuyWindow() {
    ((unsigned char *) Globals::appManager->GetApplicationData())[0x4c] = 0;
    ((unsigned char *) Globals::appManager->GetApplicationData())[0x3d] = 1;

    ChoiceWindow *win = this->dialog;
    if (this->listModeFlag != 0) {
        win->set(String(""), String(""), false, String(""), String(""), *Globals::gameText->getText(170), -1, -1);
        if (Globals::iPad) {
            this->dialog->setWidth(this->buttonWidth * 3);
            int h = (int) ((float) this->gridButtonHeight * 2.3f);
            this->dialog->setHeight(h);
        } else {
            this->dialog->setWidth(Globals::w);
            this->dialog->setHeight(Globals::h);
        }
    } else {
        win->set(String(""), String("\n\n\n\n\n\n\n\n"), false, String(""), String(""),
                 *Globals::gameText->getText(170), -1, -1);
    }

    this->dialogActive = 1;
    this->buyCreditsActive = 1;
    this->notEnoughCredits = 0;
}

int HangarWindow::getCurrentTab() {
    return this->hangarList->getCurrentTab();
}

void HangarWindow::refreshCargoAvailabilityForBlueprints() {
    Array<Array<ListItem *> *> *items = this->hangarList->getItems();
    Array<ListItem *> *arr = (*items)[2];
    if (arr == nullptr) return;
    for (uint32_t i = 0; i < arr->size(); i++) {
        ListItem *it = arr->data()[i];

        it->craftable = 0;
        if (it != nullptr && it->isBluePrint() != 0) {
            BluePrint *bp = it->bluePrint;
            Array<Item *> *cargo = Globals::status->getShip()->getCargo();
            Array<int> *ingr = bp->getIngredientList();
            if (cargo != nullptr) {
                Array<int> *counters = bp->ingredientCounters;
                for (uint32_t j = 0; j < ingr->size(); j++) {
                    if (counters->data()[j] > 0) {
                        for (uint32_t k = 0; k < cargo->size(); k++) {
                            if (cargo->data()[k]->getIndex() == ingr->data()[j]) {
                                it->craftable = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void HangarWindow::setSellMode(bool buy) {
    HangarWindow *self = this;
    ListItem *item = (ListItem *) self->selectedItem;

    if (item == 0 ||
        ((ListItem *) (item))->isShip() != 0 || ((ListItem *) (item))->isSlot() != 0 ||
        ((ListItem *) (item))->isTextButton() != 0 || ((ListItem *) (item))->isSelectable() == 0 ||
        ((ListItem *) (item))->isBluePrint() != 0) {
        self->buyMode = 0;
        return;
    }

    self->buyMode = buy;

    int tab = self->hangarList->getCurrentTab();
    if (tab == 1) {
        if (self->buyMode == 0) {
            if (((ListItem *) (item))->isItem() != 0 && ((Item *) (item->item))->getType() != 4) {
                unsigned char *flags = *g_hw_itemFlags;
                if ((*(uint8_t *) ((char *) (flags) + (0x1e))) == 0) {
                    Globals::gameText->getText(*g_hw_sellTextId1);
                    self->dialog->set(g_HangarWindow_emptyDialogText);
                    (*(uint8_t *) ((char *) (flags) + (0x1e))) = 1;
                    self->dialogActive = 1;
                }
            }
            self->autoEquipPending = 1;
            self->autoEquipIndex = self->hangarList->getCurrentItemIndex();
        } else {
            unsigned char *flags = *g_hw_itemFlags;
            if ((*(uint8_t *) ((char *) (flags) + (0x1d))) == 0) {
                Globals::gameText->getText(*g_hw_sellTextId2);
                self->dialog->set(g_HangarWindow_emptyDialogText);
                (*(uint8_t *) ((char *) (flags) + (0x1d))) = 1;
                self->dialogActive = 1;
            }
            self->savedStationAmount = ((Item *) (item->item))->getStationAmount();
            self->savedAmount = ((Item *) (item->item))->getAmount();
            self->savedCredits = Globals::status->getCredits();
            self->savedLoad = self->currentLoad;
        }

        Globals::status->getShip()->setCargo(Item::extractItems((ItemArray *) (self->itemList), true));
        Globals::status->getStation()->setItems(Item::extractItems(self->itemList, false), false);
        if (self->itemList != 0) {
            ArrayReleaseClasses(*self->itemList);
            delete self->itemList;
        }
        self->itemList = 0;

        ItemArray *mixed = Item::mixItems((ItemArray *) (Globals::status->getShip()->getCargo()),
                                          (ItemArray *) (Globals::status->getStation()->getItems()));
        self->itemList = mixed;
        self->hangarList->initShopTab((Array<Item *> *) (mixed), Globals::status->getStation()->getShips());
        self->hangarList->initShipTab(Globals::status->getShip());

        ListItem *ci = self->hangarList->getCurrentItem();
        self->selectedItem = ci;
        if (ci != nullptr && ci->isShip() != 0)
            self->selectedItem->ship->adjustPrice();
        refreshCurrentContentHeight();
        return;
    }

    if (self->hangarList->getCurrentTab() != 4)
        return;

    if (self->buyMode == 0) {
        if (self->bluePrintItem != 0 && self->bluePrint != 0) {
            Item *bpItem = ((ListItem *) self->bluePrintItem)->item;
            self->bluePrint->addItem(bpItem, bpItem->getBlueprintAmount(),
                                     Globals::status->getStation()->getIndex());
        }

        uint8_t completedFlag = 0;
        if (self->bluePrint->isCompleted() != 0) {
            Globals *globals = (Globals *) *g_hw_globals;
            if (self->bluePrint->getStationIndex() == Globals::status->getStation()->getIndex()) {
                String line, copy, name, fmt, result;
                ((String *) &copy)->Set((line).data);
                Status_replaceHash(&result, globals, &copy, &name, &fmt);
                self->dialog->set(g_HangarWindow_emptyDialogText);

                int *stations = g_hw_bpStations;
                int idx = stations[self->bluePrint->getIndex()];
                self->bluePrint->getQuantity();
                Item *made = ((Item *) (uintptr_t) idx)->makeItem();
                Globals::status->getShip()->addCargo(made);
                ArrayAdd(made, *(self->itemList));
                self->hangarList->setCurrentTab(0, true);
                self->refreshCurrentContentHeight();
            } else {
                String line, copy, name, fmt, result, line2, sname, fmt2;
                ((String *) &copy)->Set((line).data);
                Status_replaceHash(&result, globals, &copy, &name, &fmt);

                ((String *) &line2)->Set((line).data);
                self->bluePrint->getStationName();
                String result2;
                Status_replaceHash(&result2, globals, &line2, &sname, &fmt2);

                self->dialog->set(g_HangarWindow_emptyDialogText);
                ((Status *) (globals))->addPendingProduct((BluePrint *) self->bluePrint);
                self->hangarList->setCurrentTab(0, true);
                self->refreshCargoAvailabilityForBlueprints();
            }
            self->bluePrint->reset();
            completedFlag = 1;
        }

        Globals *globals = (Globals *) *g_hw_globals;
        ((Ship *) (Globals::status->getShip()))->setCargo(
            Item::extractItems((ItemArray *) (((Ship *) (0))->getCargo()), true));
        Globals::status->getShip();
        ItemArray *items = Item::mixItems((ItemArray *) (((Ship *) (0))->getCargo()),
                                          (ItemArray *) (Globals::status->getStation()->getItems()));
        self->hangarList->initShopTab((Array<Item *> *) (items), Globals::status->getStation()->getShips());
        self->hangarList->initBlueprintTab((Array<BluePrint *> *) (long) ((Status *) (globals))->getBluePrints());
        ItemArray *mix = Item::mixItems((ItemArray *) (((Ship *) (0))->getCargo()),
                                        (ItemArray *) (Globals::status->getStation()->getItems()));
        self->itemList = mix;
        self->dialogActive = completedFlag;

        if (completedFlag) {
            Array<ListItem *> *items2 = ((HangarList *) self->hangarList)->getCurrentTabItems();
            for (unsigned int i = 0; i < items2->size(); i++) {
                ListItem *li = items2->data()[i];
                if (li != 0 && ((ListItem *) (li))->isItem() != 0 &&
                    ((Item *) ((ListItem *) li)->item)->getIndex() == self->bluePrint->getIndex()) {
                    if (Globals::status->getShip()->hasEquipment(((Item *) ((ListItem *) li)->item)->getIndex(), 1) !=
                        0) {
                        self->autoEquipIndex = i;
                        self->autoEquipPending = 1;
                        self->autoEquipSecondaryWeapons(i);
                        self->autoEquipPending = 0;
                        break;
                    }
                }
            }
        }
        return;
    }

    self->bluePrintBuyCount = 0;
    if (self->bluePrint->isEmpty() != 0 && ((Item *) (item->item))->getAmount() > 0) {
        int idx = self->bluePrint->getIndex();
        bool flag;
        AbyssEngine::String *text;
        if (idx == 0xd2 || self->bluePrint->getIndex() == 0xdf) {
            if (((SolarSystem *) (long) Globals::status->getSystem())->getRoutes() != 0) {
                text = Globals::gameText->getText(*g_hw_sellTextId2);
                flag = true;
            } else {
                self->routeWarningPending = 1;
                text = Globals::gameText->getText(*g_hw_routesTextId);
                flag = false;
            }
        } else {
            text = Globals::gameText->getText(*g_hw_sellTextId2);
            flag = true;
        }
        self->dialog->set(*(String *) text, flag);
        self->dialogActive = 1;
    }

    self->savedBlueprintAmount = ((Item *) (item->item))->getBlueprintAmount();
    self->savedAmount = ((Item *) (item->item))->getAmount();
    self->savedCredits = Globals::status->getCredits();
    self->savedLoad = self->currentLoad;
    self->bluePrintItem = self->selectedItem;
}

bool HangarWindow::selectItem(ListItem *item) {
    this->selectedItem = item;
    if (item != 0 && item->isShip())
        this->selectedItem->ship->adjustPrice();

    switch (this->hangarList->getCurrentTab()) {
    case 0: {
        if (!item->isSelectable())
            break;
        this->savedScrollOffset = this->scrollOffset;
        this->scrollOffsetBackup = 0;
        this->scrollOffset = 0;
        if (!Globals::hints[31] && item->isSlot()) {
            this->dialog->set(*Globals::gameText->getText(587));
            Globals::hints[31] = 1;
            this->dialogActive = 1;
        }
        if (!item->isSelectable())
            break;
        Item *current = this->hangarList->getCurrentItem()->item;
        if (current != 0) {
            if (current->isUnsaleable()) {
                this->dialog->set(*Globals::gameText->getText(323));
                this->dialogActive = 1;
                break;
            }
            if (current->getSort() == 20) {
                int passengers = reinterpret_cast<int32_t &>(Globals::status->_byte_0x34);
                if (passengers >= 1 && item->inTabIndex >= 0 &&
                    Globals::status->getShip()->getMaxPassengers() - current->getAttribute(34) <
                    reinterpret_cast<int32_t &>(Globals::status->_byte_0x34)) {
                    this->dialog->set(*Globals::gameText->getText(323));
                    this->dialogActive = 1;
                    break;
                }
            }
        }
        if (item->inTabIndex >= 0) {
            this->demountItem(current, this->hangarList->getCurrentItem()->subTabIndex);
            break;
        }
        Item *mounted = Globals::status->getShip()->getFirstEquipmentOfSort(item->item->getSort());
        if (item->item->getSort() == 21) {
            if (Globals::status->getShip()->getIndex() == 44 ||
                Globals::status->getShip()->getIndex() == 49 || mounted == 0) {
                this->mountItem(item->item);
                break;
            }
        } else if (mounted == 0) {
            this->mountItem(item->item);
            break;
        }
        if (item->item->canBeInstalledMultipleTimes()) {
            this->mountItem(item->item);
            break;
        }
        // Replace the already mounted item of the same kind?
        String message = *Globals::gameText->getText(287);
        message = Globals::status->replaceHash(
            Globals::status->replaceHash(message, *Globals::gameText->getText(1274 + mounted->getIndex()),
                                         String("#ITEM1")),
            *Globals::gameText->getText(1274 + item->item->getIndex()), String("#ITEM2"));
        this->dialog->set(message, true);
        this->replaceEquipPending = 1;
        this->dialogActive = 1;
        this->pendingMountItem = item->item;
        this->pendingDemountItem = mounted;
        break;
    }
    case 1: {
        if (!item->isSelectable())
            break;
        if (item->isShip()) {
            int price = item->ship->getPrice();
            if (price > Globals::status->getCredits() + Globals::status->getShip()->getPrice() && !this->upgradeMode) {
                String message = Globals::status->replaceHash(
                    *Globals::gameText->getText(203),
                    String(Globals::layout->formatCredits(item->getPrice() - Globals::status->getCredits() -
                                                          Globals::status->getShip()->getPrice()), false),
                    String("#C"));
                message += String("\n\n") + *Globals::gameText->getText(124);
                this->dialog->set(message, true);
                this->notEnoughCredits = 1;
                this->dialogActive = 1;
            } else if (reinterpret_cast<int32_t &>(Globals::status->_byte_0x34) >= 1) {
                this->dialog->set(*Globals::gameText->getText(336));
                this->dialogActive = 1;
            } else if (Globals::status->getCurrentCampaignMission() == 77 &&
                       Globals::status->getShip()->getIndex() == 37) {
                this->dialog->set(*Globals::gameText->getText(325));
                this->dialogActive = 1;
            } else if (Globals::status->getShip()->getIndex() == item->ship->getIndex()) {
                this->dialog->set(*Globals::gameText->getText(329));
                this->dialogActive = 1;
            } else {
                this->shipSwapPending = 1;
                this->dialog->set(*Globals::gameText->getText(this->upgradeMode ? 333 : 304), true);
                this->dialogActive = 1;
            }
            break;
        }
        if (item->item->isUnsaleable())
            break;
        this->buyMode = !this->buyMode;
        if (this->buyMode) {
            if (!Globals::hints[29]) {
                this->dialog->set(*Globals::gameText->getText(588));
                Globals::hints[29] = 1;
                this->dialogActive = 1;
            }
            this->savedStationAmount = item->item->getStationAmount();
            this->savedAmount = item->item->getAmount();
            this->savedCredits = Globals::status->getCredits();
            this->savedLoad = this->currentLoad;
            break;
        }
        if (item->isItem() && item->item->getType() != 4 && !Globals::hints[30]) {
            this->dialog->set(*Globals::gameText->getText(589));
            Globals::hints[30] = 1;
            this->dialogActive = 1;
        }
        if (item->getIndex() >= 132 && item->getIndex() <= 153) {
            (*Globals::status->field_ac)[item->getIndex() - 132] = true;
        }
        this->autoEquipPending = 1;
        this->autoEquipIndex = this->hangarList->getCurrentItemIndex();
        Globals::status->getShip()->setCargo(Item::extractItems(this->itemList, true));
        Globals::status->getStation()->setItems(Item::extractItems(this->itemList, false), false);
        if (this->itemList != 0) {
            ArrayReleaseClasses(*this->itemList);
            delete this->itemList;
        }
        this->itemList = 0;
        this->itemList = Item::mixItems(Globals::status->getShip()->getCargo(), Globals::status->getStation()->getItems());
        this->hangarList->initShopTab(this->itemList, Globals::status->getStation()->getShips());
        this->hangarList->initShipTab(Globals::status->getShip());
        break;
    }
    case 2:
        if (!item->isSelectable() || item->isPendingProduct())
            break;
        this->bluePrint = item->bluePrint;
        this->hangarList->fillIngredientsList(item->bluePrint, false);
        this->hangarList->setCurrentTab(4, true);
        this->refreshCurrentContentHeight();
        if (this->specialMode)
            this->specialMode = 0;
        break;
    }
    return false;
}

float HangarWindow::getRelativeScrollHeight() {
    int a = this->currentContentHeight;
    int b = this->visibleHeight;
    if (a < b) {
        return 0.0f;
    }
    int e = this->scrollOffset;
    if (e >= 1) {
        return (float)(b - e) / (float)a;
    }
    if (e >= b - a) {
        return (float)b / (float)a;
    }
    return (float)(e + a) / (float)a;
}

void HangarWindow::transaction(bool buy) {
    unsigned int tab = this->hangarList->getCurrentTab();

    if (tab < 2) {
        if (this->selectedItem->item->isUnsaleable()) {
            this->dialog->set(*Globals::gameText->getText(323));
            this->dialogActive = 1;
            this->buyMode = 0;
            return;
        }

        int result = this->selectedItem->item->transaction(buy, this->currentLoad, this->upgradeMode);
        unsigned int itemIndex = this->selectedItem->item->getIndex();
        Array<bool> *seenItems = Globals::status->field_54;
        if (itemIndex < seenItems->size()) {
            (*seenItems)[this->selectedItem->item->getIndex()] = true;
        }

        if (buy && result < 0) {
            this->currentLoad = this->currentLoad + 1;
            Globals::status->getShip()->changeLoad(1);
            if (this->selectedItem->getIndex() >= 132 && this->selectedItem->getIndex() <= 153) {
                (*Globals::status->field_ac)[this->selectedItem->getIndex() - 132] = true;
            }
        } else if (buy && result == 0) {
            if (Globals::status->getCredits() < this->selectedItem->item->getSinglePrice()) {
                if (this->upgradeMode != 0)
                    return;
                String msg = Globals::status->replaceHash(
                        *Globals::gameText->getText(203),
                        String(Globals::layout->formatCredits(this->selectedItem->item->getSinglePrice() -
                                                              Globals::status->getCredits()), false),
                        String("#C"));
                msg += String("\n\n") + *Globals::gameText->getText(124);
                this->dialog->set(msg, true);
                this->notEnoughCredits = 1;
                this->dialogActive = 1;
                (*this->buttons)[8]->resetTouch();
                (*this->buttons)[9]->resetTouch();
            }
        } else if (!buy && result > 0) {
            this->currentLoad = this->currentLoad - 1;
            Globals::status->getShip()->changeLoad(-1);
        }

        if (this->upgradeMode == 0)
            Globals::status->changeCredits(result);
    } else if (tab == 4) {
        if (buy && this->selectedItem->item->getBlueprintAmount() <
                   this->bluePrint->getRemainingAmount(this->selectedItem->item->getIndex())) {
            int r = this->selectedItem->item->transactionBlueprint(false, this->currentLoad);
            if (r < 0) {
                this->currentLoad = this->currentLoad + 1;
            } else if (r != 0) {
                this->bluePrintBuyCount = this->bluePrintBuyCount + 1;
                Globals::status->getShip()->changeLoad(-1);
            }
        }
        Array<Item *> *cargo = Globals::status->getShip()->getCargo();
        if (cargo != 0) {
            for (unsigned int i = 0; i < cargo->size(); i++) {
                if ((*cargo)[i]->getIndex() == this->selectedItem->item->getIndex()) {
                    (*cargo)[i]->setAmount(this->selectedItem->item->getAmount());
                    (*cargo)[i]->setBlueprintAmount(this->selectedItem->item->getBlueprintAmount());
                }
            }
        }
    }
}

void HangarWindow::mountItem(Item *item) {
    int type = item->getType();
    int amount = 1;
    if (type == 1)
        amount = item->getAmount();

    Item *made = item->makeItem(amount);
    Ship *ship = Globals::status->getShip();
    ship->addEquipment(made);
    Globals::status->getShip()->removeCargo(made->getIndex(), (type == 1) ? made->getAmount() : 1);

    Array<Item *> *cargo = this->itemList;
    if (cargo != nullptr) {
        for (unsigned int i = 0; i < cargo->size(); i++) {
            Item *cur = cargo->data()[i];
            if (cur->getIndex() == item->getIndex()) {
                int change;
                if (cur->getStationAmount() == 0) {
                    if (type == 1 || cur->getAmount() == 1) {
                        ArrayRemove(cur, *this->itemList);
                        break;
                    }
                    change = -1;
                } else if (type == 1) {
                    change = -cur->getAmount();
                } else {
                    change = -1;
                }
                cur->changeAmount(change);
                break;
            }
            cargo = this->itemList;
        }
    }

    Globals::status->getShip()->setCargo(Item::extractItems(this->itemList, true));
    this->hangarList->initShipTab(Globals::status->getShip());

    HangarList *hangarList = this->hangarList;
    ItemArray *items = Item::mixItems(Globals::status->getShip()->getCargo(),
                                      Globals::status->getStation()->getItems());
    hangarList->initShopTab(items, Globals::status->getStation()->getShips());
    this->hangarList->setCurrentTab(0, true);

    refreshCurrentContentHeight();
    this->scrollOffset = this->savedScrollOffset;
    (*g_hw_sound)->play(0x62, nullptr, nullptr, 0.0f);
}

unsigned int HangarWindow::OnTouchMove(int touch, int coord) {
    Globals::layout->OnTouchMove(touch, coord);

    if (this->dialogActive != 0) {
        if (this->buyCreditsActive != 0) {
            for (int i = 0; i < 5; i++) {
                (*this->buttons)[12 + i]->OnTouchMove(touch, coord);
            }
            (*this->buttons)[17]->OnTouchMove(touch, coord);
        } else if (this->freeCreditsActive != 0) {
            for (int i = 0; i < 5; i++) {
                (*this->buttons)[18 + i]->OnTouchMove(touch, coord);
            }
        }
        this->dialog->OnTouchMove(touch, coord);
        return 0;
    }

    if (this->viewMode == 1) {
        this->listItemWindow->OnTouchMove(touch, coord);
        return 0;
    }

    Layout *layout = Globals::layout;
    if (layout->field_0xc_leftMargin < coord && coord < Globals::h - layout->field_0x10_rightMargin) {
        int dy = coord - this->lastTouchY;
        this->scrollDelta = dy;
        this->damping = 1.0f;
        this->scrollOffset = this->scrollOffset + dy;
        this->lastTouchY = coord;

        bool touched = (*this->buttons)[8]->isTouched() || (*this->buttons)[9]->isTouched();
        int dist = coord - this->touchStartY;
        if (dist < 0) {
            dist = -dist;
        }
        if (!(touched || dist < 6)) {
            this->holdTime = 0;
            this->repeatTimer = 0;
            for (unsigned int i = 0; i < this->buttons->size(); i++) {
                (*this->buttons)[i]->OnTouchMove(touch, coord);
            }
            this->setSellMode(false);
            this->selectedItem = 0;
            this->sellConfirmPending = 0;
            (*this->buttons)[8]->resetTouch();
            (*this->buttons)[9]->resetTouch();
        }
    }

    for (unsigned int i = 0; i < this->tabButtons->size(); i++) {
        (*this->tabButtons)[i]->OnTouchMove(touch, coord);
    }
    return 0;
}

void HangarWindow::autoEquipSecondaryWeapons(int row) {
    ListItem *item = this->hangarList->getCurrentItemAt(row);
    if (item == 0 || item->item == 0 || item->item->getType() != 1 || !item->isItem() ||
        item->item->getType() != 1 || item->item->getAmount() < 1)
        return;

    Array<Item *> *equipment = Globals::status->getShip()->getEquipment(1);
    if (equipment == 0)
        return;

    for (unsigned int i = 0; i < equipment->size(); i++) {
        Item *mounted = (*equipment)[i];
        if (mounted != 0 && mounted->getIndex() == item->item->getIndex()) {
            Item *merged = item->item->makeItem(item->item->getAmount() + mounted->getAmount());
            if (this->itemList != 0) {
                for (unsigned int j = 0; j < this->itemList->size(); j++) {
                    if ((*this->itemList)[j]->getIndex() == merged->getIndex())
                        (*this->itemList)[j]->setAmount(0);
                }
            }
            Globals::status->getShip()->setEquipment(merged, i);
            Globals::status->getShip()->removeCargo(merged->getIndex(), merged->getAmount());
            this->hangarList->initShipTab(Globals::status->getShip());

            String msg = *Globals::gameText->getText(208);
            msg = Globals::status->replaceHash(msg, *Globals::gameText->getText(merged->getIndex() + 1274),
                                               String("#N"));
            this->dialog->set(msg);
            this->dialogActive = 1;
            this->autoEquipped = 1;
            return;
        }
    }
}

void HangarWindow::showFreeCreditsWindow() {
    ((unsigned char *) Globals::appManager->GetApplicationData())[0x4c] = 0;
    ((unsigned char *) Globals::appManager->GetApplicationData())[0x3d] = 1;

    this->dialog->set(String(""), String(""), false, String(""), String(""), *Globals::gameText->getText(170), -1,
                      -1);
    this->dialog->setHeight((*this->buttons)[18]->getHeight() * 5);

    int maxWidth = 0;
    for (int i = 0; i < 5; i++) {
        int w = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                              *Globals::gameText->getText(i == 0 ? 3400 : 112 + i));
        if (w > maxWidth)
            maxWidth = w;
    }
    int width = (*this->buttons)[18]->getWidth() + maxWidth + Globals::layout->buttonInsetX * 4 +
                Globals::layout->field_0x2c_rowHeight;
    this->dialog->setWidth(width);

    this->dialogActive = 1;
    this->freeCreditsActive = 1;
    this->buyCreditsActive = 0;
    this->notEnoughCredits = 0;
}

void HangarWindow::initialize() {
    this->upgradeMode = Globals::status->getStation()->getIndex() == 108 && Globals::status->field_114 == 3;
    Globals::status->calcCargoPrices();

    this->hangarList = new HangarList();
    this->itemList = Item::mixItems(Globals::status->getShip()->getCargo(), Globals::status->getStation()->getItems());
    this->hangarList->init(Globals::status->getShip(), this->itemList, Globals::status->getStation()->getShips(),
                           (Array<BluePrint *> *) Globals::status->getBluePrints());

    // Tabs (right-aligned next to the help button).
    this->tabButtons = new Array<TouchButton *>();
    ArraySetLength(3, *this->tabButtons);
    (*this->tabButtons)[2] = new TouchButton(*Globals::gameText->getText(272), 3,
                                             Globals::w - Globals::layout->getHelpButtonOffset(), 0, 18);
    (*this->tabButtons)[1] = new TouchButton(*Globals::gameText->getText(this->upgradeMode ? 186 : 185), 3,
                                             Globals::w - Globals::layout->getHelpButtonOffset() -
                                             (*this->tabButtons)[2]->getWidth() + Globals::layout->field_0x38,
                                             0, 18);
    (*this->tabButtons)[0] = new TouchButton(*Globals::gameText->getText(183), 3,
                                             Globals::w - Globals::layout->getHelpButtonOffset() -
                                             (*this->tabButtons)[2]->getWidth() -
                                             (*this->tabButtons)[1]->getWidth() + Globals::layout->field_0x38 * 2,
                                             0, 18);
    this->listModeFlag = Globals::showNewCreditsMenu;
    this->tabIcons = new unsigned int[6];
    for (int i = 0; i != 6; i++) {
        Globals::Canvas->Image2DCreate((uint16_t) (9002 + i), this->tabIcons[i]);
    }
    for (unsigned int i = 0; i < this->tabButtons->size(); i++) {
        if (i < 10) {
            Globals::sub_menu_buttons_x[i] = (int) (*this->tabButtons)[i]->getPosition().x;
            Globals::sub_menu_buttons_y[i] = (int) (*this->tabButtons)[i]->getPosition().y;
        }
    }
    Globals::sub_menu_button_count = this->tabButtons->size();
    Globals::Canvas->Image2DCreate(1326, this->blueprintIconImage);
    Globals::Canvas->Image2DCreate(1348, this->pendingIconImage);

    // Action buttons.
    this->buttons = new Array<TouchButton *>();
    ArraySetLength(24, *this->buttons);
    unsigned int image = 0xffffffffu;
    Globals::Canvas->Image2DCreate(1136, image);
    (*this->buttons)[0] = new TouchButton(image, 7, 0, 0, Globals::layout->field_0x60, 17, 4);
    bool alienStation = Globals::status->getCurrentCampaignMission() == 77 &&
                        Globals::status->getStation()->getIndex() == 100;
    (*this->buttons)[1] = new TouchButton(*Globals::gameText->getText(this->upgradeMode || alienStation ? 332 : 301), 7,
                                          0, 0, 17);
    (*this->buttons)[2] = new TouchButton(*Globals::gameText->getText(302), 7, 0, 0, 17);
    unsigned int upImage = 0xffffffffu;
    Globals::Canvas->Image2DCreate(1331, upImage);
    (*this->buttons)[3] = new TouchButton(upImage, 7, 0, 0, Globals::layout->field_0x64, 17, 4);
    unsigned int downImage = 0xffffffffu;
    Globals::Canvas->Image2DCreate(1330, downImage);
    (*this->buttons)[4] = new TouchButton(downImage, 7, 0, 0, Globals::layout->field_0x64, 17, 4);
    (*this->buttons)[5] = new TouchButton(*Globals::gameText->getText(279), 7, 0, 0, this->buttonHeight, 17, 1);
    (*this->buttons)[6] = new TouchButton(*Globals::gameText->getText(282), 7, 0, 0, this->buttonHeight, 17, 1);
    (*this->buttons)[7] = new TouchButton(*Globals::gameText->getText(283), 7, 0, 0, 17);
    (*this->buttons)[8] = new TouchButton(String(""), 8, 0, 0, Globals::layout->field_0x50, 17, 4);
    (*this->buttons)[9] = new TouchButton(String(""), 9, 0, 0, Globals::layout->field_0x50, 17, 4);
    (*this->buttons)[10] = new TouchButton(*Globals::gameText->getText(330), 7, 0, 0, Globals::layout->field_0x50, 17, 4);
    (*this->buttons)[11] = new TouchButton(Globals::layout->formatCredits(Globals::status->getCredits()), 11, Globals::w,
                                           Globals::h, Globals::layout->getFooterTransitionWidth(), 34, 4);
    for (int i = 0; i < 5; i++) {
        if (this->listModeFlag) {
            (*this->buttons)[12 + i] = new TouchButton(String(""), 10, 0, 0, 1);
        } else {
            (*this->buttons)[12 + i] = new TouchButton(String(""), 0, 0, 0, Globals::layout->field_0x264, 17, 1);
        }
        (*this->buttons)[12 + i]->setVisible(false);
    }
    if (this->listModeFlag) {
        (*this->buttons)[17] = new TouchButton(String(""), 10, 0, 0, 1);
    } else {
        (*this->buttons)[17] = new TouchButton(String(""), 0, 0, 0, Globals::layout->field_0x264, 17, 1);
    }
    (*this->buttons)[17]->setVisible(false);
    Globals::Canvas->Image2DCreate(9022, this->scrollHintImageA);
    Globals::Canvas->Image2DCreate(9023, this->scrollHintImageB);
    (*this->buttons)[23] = new TouchButton(String("Autocomplete"), 7, 0, 0, 17);

    // Credit packages.
    unsigned int packImage = 0xffffffffu;
    unsigned int packImagePressed = 0xffffffffu;
    for (int i = 0; i < 5; i++) {
        if (i + 18 == 18) {
            Globals::Canvas->Image2DCreate(9020, packImage);
            Globals::Canvas->Image2DCreate(9021, packImagePressed);
        } else {
            Globals::Canvas->Image2DCreate((uint16_t) (9008 + i * 2), packImage);
            Globals::Canvas->Image2DCreate((uint16_t) (9009 + i * 2), packImagePressed);
        }
        (*this->buttons)[i + 18] = new TouchButton(packImage, packImagePressed, 19, 0, 0, 1);
        (*this->buttons)[i + 18]->setVisible(false);
    }

    this->buttonWidth = (*this->buttons)[12]->getWidth();
    this->gridButtonHeight = (*this->buttons)[12]->getHeight();
    this->gridSpacingX = (int) ((float) -this->buttonWidth * 0.1f);
    this->gridSpacingY = (int) ((float) -this->gridButtonHeight * 0.1f);
    Globals::Canvas->Image2DCreate(1141, *(unsigned int *) &this->progressBarBgImage);
    Globals::Canvas->Image2DCreate(1142, this->progressBarFillImage);
    Globals::Canvas->Image2DCreate(1143, this->progressBarBorderImage);
    this->progressBarWidth = Globals::Canvas->GetImage2DWidth(this->progressBarFillImage);
    this->progressBarHeight = Globals::Canvas->GetImage2DHeight(this->progressBarFillImage);

    // Remember best buy/sell prices seen for every item.
    if (this->itemList != 0 && !Globals::status->inBlackMarketSystem() && !this->upgradeMode) {
        Array<Item *> *equipment = Globals::status->getShip()->getEquipment();
        for (unsigned int i = 0; i < this->itemList->size() + (equipment != 0 ? equipment->size() : 0); i++) {
            Item *item = i < this->itemList->size() ? (*this->itemList)[i]
                                                    : (*equipment)[i - this->itemList->size()];
            if (item == 0)
                continue;
            int price = item->getSinglePrice();
            int index = item->getIndex();
            if (price > (*Globals::status->field_0x40)[index] || (*Globals::status->field_0x40)[index] == 0) {
                (*Globals::status->field_0x40)[index] = price;
                (*Globals::status->field_0x48)[index] = ((SolarSystem *) Globals::status->getSystem())->getIndex();
            }
            if (price < (*Globals::status->field_0x3c)[index] || (*Globals::status->field_0x3c)[index] == 0) {
                (*Globals::status->field_0x3c)[index] = price;
                (*Globals::status->field_0x44)[index] = ((SolarSystem *) Globals::status->getSystem())->getIndex();
            }
        }
    }
    Globals::status->getShip()->adjustPrice();

    this->listItemWindow = new ListItemWindow();
    this->choiceWindow = new ChoiceWindow();
    this->viewMode = 0;
    this->dialog = new ChoiceWindow();
    this->dialogActive = 0;
    this->replaceEquipPending = 0;
    this->holdTime = 0;
    this->repeatTimer = 0;
    this->pendingMountItem = 0;
    this->pendingDemountItem = 0;
    this->routeWarningPending = 0;
    this->suppressTouchEnd = 0;
    this->sellConfirmPending = 0;
    this->autoCompletePending = 0;
    this->autoEquipped = 0;
    this->buyCreditsActive = 0;
    this->notEnoughCredits = 0;
    this->freeCreditsActive = 0;
    this->field_0x40 = 16;
    this->field_0x44 = 5;
    this->field_0x48 = 5;
    this->contentWidth = Globals::w - 10;
    this->contentHeight = Globals::h - 10;
    this->columnWidths = new int[3];
    int column = (Globals::w - 10) / 3 - 2;
    this->columnWidths[0] = column;
    this->columnWidths[1] = column;
    this->columnWidths[2] = Globals::w - 14 - this->columnWidths[0] - this->columnWidths[1];
    this->hangarList->setCurrentTab(HangarWindow::lastTab, false);
    this->refreshCurrentContentHeight();
    this->currentLoad = Globals::status->getShip()->getCurrentLoad();
    Layout *layout = Globals::layout;
    this->visibleHeight = Globals::h - layout->field_0x10_rightMargin - layout->field_0xc_leftMargin -
                          layout->field_0x20_top - layout->field_0x24;
    int hintX = 0;
    if (Globals::iPad && !Globals::iPadAssetsWithLowerRes) {
        Globals::Canvas->Image2DCreate(1700, *(unsigned int *) &this->hintImage);
        hintX = (int) ((float) Globals::w * 0.2f);
    }
    this->hintOffsetX = hintX;
    this->selectedItem = 0;
    this->buyMode = 0;
    this->specialMode = 0;
    this->bluePrintPurchasePending = 0;
    this->shipSwapPending = 0;
    this->dlcMenuPending = 0;
    this->scrollOffset = 0;
    this->lastTouchY = 0;
    this->scrollOffsetBackup = 0;
    this->scrollDelta = 0;
    this->damping = 0.0f;
    this->velocity = 0.0f;
    this->touchStartY = 0;
    this->dragging = 0;
    this->savedScrollOffset = 0;
    this->field_0x0 = 0;
    this->active = 1;
    this->autoEquipPending = 0;
    this->autoEquipIndex = 0xffffffffu;

    if (Globals::status->getCurrentCampaignMission() >= 14 && Globals::options[78] == 0) {
        // First visit: hangar tutorial message.
        this->dialog->set(*Globals::gameText->getText(109));
        Globals::options[78] = 1;
        Globals::recordHandler->saveOptions();
        this->dialogActive = 1;
    }
}

HangarWindow::HangarWindow() {
    this->bluePrint = nullptr;
    this->bluePrintItem = nullptr;
    this->dialogActive = 0;
    this->bluePrintPurchasePending = 0;
    this->hangarList = nullptr;
    this->listItemWindow = nullptr;
    this->choiceWindow = nullptr;
    this->dialog = nullptr;
    this->localBluePrint = 0;
    HangarWindow::lastTab = 1;
    this->dragging = 0;
    this->routeWarningPending = 0;
    this->tabButtons = nullptr;
    this->buttons = nullptr;
    this->shipSwapPending = 0;
    this->dlcMenuPending = 0;
    this->sellShipPending = 0;
    this->buyCreditsActive = 0;
    this->notEnoughCredits = 0;
    this->freeCreditsActive = 0;
    this->autoCompletePending = 0;

    Layout *lay = Globals::layout;
    this->field_0x100 = lay->field_0x238_blk16;
    this->buttonHeight = lay->field_0x248;
    this->field_0x114 = lay->field_0x24c;
    this->iconOffsetY = lay->field_0x250;
}

unsigned char HangarWindow::isInitialized() { return this->active; }

ListItem *HangarWindow::getCurrentItem() {
    return this->selectedItem;
}

int HangarWindow::lastTab;
