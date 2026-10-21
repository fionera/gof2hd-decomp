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
#include "game/menu/ModStation.h"

static const String g_HangarWindow_emptyDialogText;

void Status_replaceHash(...);

static inline int IDIV(int a, int b) { return a / b; }
static inline unsigned int UDIV(unsigned int a, unsigned int b) { return a / b; }

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

int HangarWindow::lastTab = 1;

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
    this->swapChoicePending = 0;
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
    this->swapChoicePending = 0;
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

void HangarWindow::refreshCurrentContentHeight() {
    Array<ListItem *> *items = ((HangarList *) this->hangarList)->getCurrentTabItems();
    if (items != 0) {
        int n = (int) items->size();

        int rowH = reinterpret_cast<int *>(Globals::layout)[0x70 / 4];
        this->currentContentHeight = this->field_0x100.d * (n - 1) + n * rowH;
    }
}

unsigned char HangarWindow::isInitialized() { return this->active; }

int HangarWindow::getCurrentTab() {
    return this->hangarList->getCurrentTab();
}

bool HangarWindow::listMode() {
    HangarWindow *self = this;
    if (self->viewMode == 0 && self->specialMode == 0) {
        return self->dialogActive == 0;
    }
    return false;
}

bool HangarWindow::isInSpecialMode() {
    if (this->specialMode != 0) return true;
    return this->dialogActive != 0;
}

ListItem *HangarWindow::getCurrentItem() {
    return this->selectedItem;
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
        if (minOffset < 0) {
            if (minOffset > this->scrollOffset) {
                this->damping = 1.0f;
                this->velocity = (float) (minOffset - this->scrollOffset) * 0.5f;
            }
        } else {
            this->scrollOffset = 0;
            this->velocity = 0;
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

void HangarWindow::render3D() {
    if (this->viewMode == 1) {
        this->listItemWindow->render();
    }
}

void HangarWindow::render() {
    Layout *layout = *g_hw_layout;
    PaintCanvas *canvas = Globals::Canvas;
    ((PaintCanvas *) canvas)->SetColor(0xffffffffu);

    unsigned char *dlc = (unsigned char *) Globals::appManager->GetApplicationModule(5);

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
        unsigned char *appData = ApplicationManager_GetApplicationData();
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

float HangarWindow::getRelativeScrollStartPos() {
    int range = this->scrollOffset;
    if (range > 0) {
        return 0.0f;
    }
    return -(float) range / (float) this->currentContentHeight;
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
        this->bluePrintPurchasePending = 1;
        this->dialogActive = 1;
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

void HangarWindow::setSellMode(bool buy) {
    ListItem *item = this->selectedItem;
    if ((!this->buyMode && !buy) || item == 0 || item->isShip() || item->isSlot() || item->isTextButton() ||
        !item->isSelectable() || item->isBluePrint()) {
        this->buyMode = 0;
        return;
    }
    this->buyMode = buy;

    if (this->hangarList->getCurrentTab() == 1) {
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
        } else {
            if (item->isItem() && item->item->getType() != 4 && !Globals::hints[30]) {
                this->dialog->set(*Globals::gameText->getText(589));
                Globals::hints[30] = 1;
                this->dialogActive = 1;
            }
            this->autoEquipPending = 1;
            this->autoEquipIndex = this->hangarList->getCurrentItemIndex();
        }
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
        this->selectedItem = this->hangarList->getCurrentItem();
        if (this->selectedItem != 0 && this->selectedItem->isShip())
            this->selectedItem->ship->adjustPrice();
        this->refreshCurrentContentHeight();
    } else if (this->hangarList->getCurrentTab() == 4) {
        if (!this->buyMode) {
            // Deliver the selected ingredient to the blueprint.
            if (this->bluePrintItem != 0 && this->bluePrint != 0) {
                Item *ingredient = this->bluePrintItem->item;
                this->bluePrint->addItem(ingredient, ingredient->getBlueprintAmount(),
                                         Globals::status->getStation()->getIndex());
            }
            bool completed = false;
            if (this->bluePrint->isCompleted()) {
                if (this->bluePrint->getStationIndex() != Globals::status->getStation()->getIndex()) {
                    String message = *Globals::gameText->getText(210);
                    message = Globals::status->replaceHash(
                        message, *Globals::gameText->getText(1274 + this->bluePrint->getIndex()), String("#N"));
                    message = Globals::status->replaceHash(message, String(this->bluePrint->getStationName(), false),
                                                           String("#S"));
                    this->dialog->set(message);
                    Globals::status->addPendingProduct(this->bluePrint);
                    this->hangarList->setCurrentTab(2, true);
                    this->refreshCargoAvailabilityForBlueprints();
                } else {
                    String message = *Globals::gameText->getText(211);
                    message = Globals::status->replaceHash(
                        message, *Globals::gameText->getText(1274 + this->bluePrint->getIndex()), String("#N"));
                    this->dialog->set(message);
                    Item *product = (*Globals::items)[this->bluePrint->getIndex()]->makeItem(
                        this->bluePrint->getQuantity());
                    Globals::status->getShip()->addCargo(product);
                    ArrayAdd(product, *this->itemList);
                    this->hangarList->setCurrentTab(1, true);
                    this->refreshCurrentContentHeight();
                }
                this->bluePrint->reset();
                completed = true;
            }
            Globals::status->getShip()->setCargo(Item::extractItems(Globals::status->getShip()->getCargo(), true));
            this->hangarList->initShopTab(
                Item::mixItems(Globals::status->getShip()->getCargo(), Globals::status->getStation()->getItems()),
                Globals::status->getStation()->getShips());
            this->hangarList->initBlueprintTab((Array<BluePrint *> *) Globals::status->getBluePrints());
            this->itemList = Item::mixItems(Globals::status->getShip()->getCargo(),
                                            Globals::status->getStation()->getItems());
            this->dialogActive = completed;
            if (completed) {
                // Auto-mount the freshly produced item if the ship already carries one.
                Array<ListItem *> *items = this->hangarList->getCurrentTabItems();
                for (unsigned int i = 0; i < items->size(); i++) {
                    ListItem *listItem = (*items)[i];
                    if (listItem != 0 && listItem->isItem() &&
                        listItem->item->getIndex() == this->bluePrint->getIndex() &&
                        Globals::status->getShip()->hasEquipment(listItem->item->getIndex(), 1)) {
                        this->autoEquipPending = 1;
                        this->autoEquipIndex = i;
                        this->autoEquipSecondaryWeapons(i);
                        this->autoEquipPending = 0;
                        break;
                    }
                }
            }
            return;
        }
        this->bluePrintBuyCount = 0;
        if (this->bluePrint->isEmpty() && this->selectedItem->item->getAmount() >= 1) {
            if ((this->bluePrint->getIndex() == 210 || this->bluePrint->getIndex() == 223) &&
                ((SolarSystem *) Globals::status->getSystem())->getRoutes() == 0) {
                this->routeWarningPending = 1;
                this->dialog->set(*Globals::gameText->getText(528), false);
                this->dialogActive = 1;
            } else {
                this->dialog->set(*Globals::gameText->getText(212), true);
                this->dialogActive = 1;
            }
        }
        this->savedBlueprintAmount = item->item->getBlueprintAmount();
        this->savedAmount = item->item->getAmount();
        this->savedCredits = Globals::status->getCredits();
        this->savedLoad = this->currentLoad;
        this->bluePrintItem = this->selectedItem;
    }
    this->refreshCurrentContentHeight();
}

bool HangarWindow::OnTouchEnd(int x, int y) {
    this->holdTime = 0;
    this->repeatTimer = 0;
    this->dragging = 0;
    if (this->suppressTouchEnd) {
        this->suppressTouchEnd = 0;
        return false;
    }

    if (this->dialogActive) {
        if (this->autoCompletePending) {
            int choice = this->dialog->OnTouchEnd(x, y);
            if (choice == 0) {
                int price = this->bluePrint->getAutoCompletionPrice();
                if (price > Globals::status->getCredits()) {
                    String message = Globals::status->replaceHash(
                        *Globals::gameText->getText(203),
                        String(Globals::layout->formatCredits(price - Globals::status->getCredits()), false),
                        String("#C"));
                    message += String("\n\n") + *Globals::gameText->getText(124);
                    this->dialog->set(message, true);
                    this->notEnoughCredits = 1;
                } else {
                    this->dialogActive = 0;
                    if (this->bluePrint != 0) {
                        if (this->bluePrint->isEmpty()) {
                            this->bluePrint->stationIndex = Globals::status->getStation()->getIndex();
                            this->bluePrint->stationName = Globals::status->getStation()->getName();
                        }
                        this->bluePrint->complete();
                        this->highlightItem(this->hangarList->getCurrentItemAt(1));
                        this->buyMode = 1;
                        this->setSellMode(false);
                        Globals::status->changeCredits(-price);
                    }
                }
            } else if (choice == 1) {
                this->dialogActive = 0;
            }
            this->autoCompletePending = 0;
            return false;
        }

        if (this->replaceEquipPending) {
            int choice = this->dialog->OnTouchEnd(x, y);
            if (choice == 0) {
                if (this->pendingMountItem != 0 && this->pendingDemountItem != 0) {
                    this->demountItem(this->pendingDemountItem, -1);
                    this->savedScrollOffset = this->scrollOffset;
                    this->mountItem(this->pendingMountItem);
                    this->replaceEquipPending = 0;
                    this->dialogActive = 0;
                }
            } else if (choice == 1) {
                this->dialogActive = 0;
                this->replaceEquipPending = 0;
                this->scrollOffset = this->savedScrollOffset;
            }
        } else if (this->notEnoughCredits) {
            int choice = this->dialog->OnTouchEnd(x, y);
            if (choice == 0) {
                Globals::options[78] = 1;
                Globals::recordHandler->saveOptions();
                this->showCreditsBuyWindow();
            } else if (choice == 1) {
                this->dialogActive = 0;
                this->notEnoughCredits = 0;
            }
        } else if (this->buyCreditsActive) {
            if (this->dialog->OnTouchEnd(x, y) == 0) {
                this->dialogActive = 0;
                this->buyCreditsActive = 0;
                for (int i = 12; i != 17; i++)
                    (*this->buttons)[i]->setVisible(false);
                (*this->buttons)[17]->setVisible(false);
                ((uint8_t *) Globals::appManager->GetApplicationData())[64] = 0;
                ModStation *station = (ModStation *) Globals::appManager->GetApplicationModule(5);
                if (station != 0 && station->pendingHangarClose) {
                    station->pendingHangarClose = 0;
                    return true;
                }
            }
            for (int i = 0; i < 5; i++) {
                if ((*this->buttons)[12 + i]->OnTouchEnd(x, y)) {
                    switch (i) {
                    case 0:
                        NFC::iap_buy_credits_100_000();
                        break;
                    case 1:
                        NFC::iap_buy_credits_300_000();
                        break;
                    case 2:
                        NFC::iap_buy_credits_1_000_000();
                        break;
                    case 3:
                        NFC::iap_buy_credits_3_000_000();
                        break;
                    case 4:
                        NFC::iap_buy_credits_10_000_000();
                        break;
                    }
                    break;
                }
            }
            if ((*this->buttons)[17]->OnTouchEnd(x, y)) {
                bool offersLeft = true;
                if (Globals::options[74] && Globals::options[73] && Globals::options[77] && Globals::options[76])
                    offersLeft = !Globals::options[75];
                uint8_t *appData = (uint8_t *) Globals::appManager->GetApplicationData();
                if (offersLeft || !appData[21]) {
                    for (int i = 12; i != 17; i++)
                        (*this->buttons)[i]->setVisible(false);
                    this->showFreeCreditsWindow();
                }
            }
        } else if (this->freeCreditsActive) {
            if (this->dialog->OnTouchEnd(x, y) == 0) {
                for (int i = 18; i != 23; i++)
                    (*this->buttons)[i]->setVisible(false);
                this->freeCreditsActive = 0;
                this->showCreditsBuyWindow();
            }
            static const int freeCreditsReward[6] = {0, 7500, 5000, 5000, 5000, 7500};
            uint8_t *appData = (uint8_t *) Globals::appManager->GetApplicationData();
            for (int i = 0; i != 5; i++) {
                if ((*this->buttons)[18 + i]->OnTouchEnd(x, y)) {
                    switch (i) {
                    case 0:
                        Globals::recordHandler->recordStoreWrite(0);
                        Globals::recordHandler->recordStoreWritePreview(0);
                        break;
                    case 1:
                        appData[160] = 1;
                        NFC::free_credits_likeGOF2OnFacebook();
                        Globals::status->changeCredits(freeCreditsReward[i]);
                        Globals::options[73] = 1;
                        break;
                    case 2:
                        appData[161] = 1;
                        NFC::free_credits_likeFishlabsOnFacebook();
                        Globals::status->changeCredits(freeCreditsReward[i]);
                        Globals::options[74] = 1;
                        break;
                    case 3:
                        appData[162] = 1;
                        NFC::free_credits_subscribeToYoutubeChannel();
                        Globals::status->changeCredits(freeCreditsReward[i]);
                        Globals::options[75] = 1;
                        break;
                    case 4:
                        appData[163] = 1;
                        NFC::free_credits_followOnTwitter();
                        Globals::status->changeCredits(freeCreditsReward[i]);
                        Globals::options[76] = 1;
                        break;
                    case 5:
                        appData[13] = 1;
                        NFC::free_credits_rateGame();
                        Globals::status->changeCredits(freeCreditsReward[i]);
                        Globals::options[77] = 1;
                        break;
                    }
                }
            }
        } else if (this->bluePrintPurchasePending) {
            int choice = this->dialog->OnTouchEnd(x, y);
            int cost = this->bluePrintItem->item->getBlueprintAmount() * 200;
            if (choice == 0 && Globals::status->getCredits() >= cost && !this->localBluePrint) {
                Globals::status->changeCredits(-cost);
                this->setSellMode(false);
                this->selectedItem = 0;
                this->hangarList->setCurrentItemIndex(-1);
                this->localBluePrint = 0;
                this->bluePrintPurchasePending = 0;
            } else if (choice == 1 || Globals::status->getCredits() < cost || (choice == 0 && this->localBluePrint)) {
                // Undo the pending ingredient purchase.
                this->bluePrintItem->item->setStationAmount(this->savedStationAmount);
                this->bluePrintItem->item->setAmount(this->savedAmount);
                this->bluePrintItem->item->setBlueprintAmount(this->savedBlueprintAmount);
                Globals::status->setCredits(this->savedCredits);
                this->savedStationAmount = 0;
                this->bluePrintBuyCount = 0;
                this->selectedItem = 0;
                this->savedAmount = 0;
                this->savedBlueprintAmount = 0;
                this->currentLoad = this->savedLoad;
                this->hangarList->setCurrentItemIndex(-1);
                this->dialogActive = 0;
                this->bluePrintPurchasePending = 0;
                this->buyMode = 0;
                if (Globals::status->getCredits() < cost && !this->localBluePrint) {
                    this->dialog->set(Globals::status->replaceHash(
                        *Globals::gameText->getText(203),
                        String(Globals::layout->formatCredits(cost - Globals::status->getCredits()), false),
                        String("#C")));
                    this->dialogActive = 1;
                }
                this->localBluePrint = 0;
            }
            this->refreshCurrentContentHeight();
            return false;
        } else if (this->sellShipPending) {
            int choice = this->dialog->OnTouchEnd(x, y);
            if (choice == 0) {
                this->sellShipPending = 0;
                this->dialogActive = 0;
                Globals::status->changeCredits(this->selectedItem->getPrice());
                Globals::status->getStation()->removeShip(this->selectedItem->ship);
                this->hangarList->initShopTab(this->itemList, Globals::status->getStation()->getShips());
                this->refreshCurrentContentHeight();
            } else if (choice == 1) {
                this->sellShipPending = 0;
                this->dialogActive = 0;
            }
        } else if (this->shipSwapPending) {
            int choice = this->dialog->OnTouchEnd(x, y);
            bool buyAdditional;
            if (Globals::status->field_114 == 3 && !this->upgradeMode) {
                if (!this->swapChoicePending) {
                    if (choice == 1) {
                        this->dialogActive = 0;
                        this->shipSwapPending = 0;
                        this->swapChoicePending = 0;
                        return false;
                    }
                    if (choice != 0)
                        goto common;
                    this->dialog->set(String(""), *Globals::gameText->getText(327), true, *Globals::gameText->getText(330),
                                      *Globals::gameText->getText(331), *Globals::gameText->getText(330), -1, -1);
                    this->swapChoicePending = 1;
                    return false;
                }
                if (choice == 0) {
                    buyAdditional = false;
                } else {
                    if (choice != 1)
                        goto common;
                    if (Globals::status->voidStation->hasShip(Globals::status->getShip()->getIndex())) {
                        this->dialog->set(*Globals::gameText->getText(328));
                        this->dialogActive = 1;
                        this->shipSwapPending = 0;
                        this->swapChoicePending = 0;
                        return false;
                    }
                    if (this->selectedItem->ship->getPrice() > Globals::status->getCredits()) {
                        String message = Globals::status->replaceHash(
                            *Globals::gameText->getText(203),
                            String(Globals::layout->formatCredits(this->selectedItem->ship->getPrice() -
                                                                  Globals::status->getCredits()), false),
                            String("#C"));
                        message += String("\n\n") + *Globals::gameText->getText(124);
                        this->dialog->set(message, true);
                        this->dialogActive = 1;
                        this->notEnoughCredits = 1;
                        this->shipSwapPending = 0;
                        this->swapChoicePending = 0;
                        return false;
                    }
                    buyAdditional = true;
                }
            } else if (!this->upgradeMode) {
                if (!this->dlcMenuPending) {
                    if (choice == 1) {
                        this->dialogActive = 0;
                        this->shipSwapPending = 0;
                        this->swapChoicePending = 0;
                        return false;
                    }
                    if (choice != 0)
                        goto common;
                    this->dialog->set(String(""), *Globals::gameText->getText(189), true);
                    this->dlcMenuPending = 1;
                    return false;
                }
                if (choice == 0) {
                    ((ModStation *) Globals::appManager->GetApplicationModule(5))->showDlcMenu();
                    this->dialogActive = 0;
                    this->dlcMenuPending = 0;
                    this->shipSwapPending = 0;
                    this->swapChoicePending = 0;
                    return false;
                }
                if (choice != 1)
                    goto common;
                this->dlcMenuPending = 0;
                buyAdditional = false;
            } else {
                if (choice == 1) {
                    this->dialogActive = 0;
                    this->shipSwapPending = 0;
                    this->swapChoicePending = 0;
                    return false;
                }
                if (choice != 0)
                    goto common;
                buyAdditional = false;
            }

            // Swap the current ship for the selected one.
            Globals::status->getShip()->setCargo(Item::extractItems(this->itemList, true));
            if (buyAdditional)
                Globals::status->changeCredits(-this->selectedItem->ship->getPrice());
            else if (!this->upgradeMode)
                Globals::status->changeCredits(Globals::status->getShip()->getPrice() - this->selectedItem->ship->getPrice());

            Ship *oldShip = Globals::status->getShip();
            Array<Item *> *equipment = oldShip->getEquipment();
            Array<Item *> *cargo = oldShip->getCargo();
            Ship *newShip = (*Globals::ships)[this->selectedItem->ship->getIndex()]->makeShip(-1);
            newShip->setRace(this->selectedItem->ship->getRace());
            newShip->adjustPrice();
            if (cargo != 0) {
                for (unsigned int i = 0; i < cargo->size(); i++) {
                    if ((*cargo)[i] != 0)
                        newShip->addCargo((*cargo)[i]->clone());
                }
            }
            if (equipment != 0) {
                for (unsigned int i = 0; i < equipment->size(); i++) {
                    if ((*equipment)[i] != 0) {
                        Item *copy = (*equipment)[i]->clone();
                        if (!newShip->addEquipment(copy))
                            newShip->addCargo(copy);
                    }
                }
            }
            if (this->selectedItem->ship->getMods() != 0) {
                for (unsigned int i = 0; i < this->selectedItem->ship->getMods()->size(); i++)
                    newShip->addMod((*this->selectedItem->ship->getMods())[i]);
            }

            Array<Ship *> *ships = Globals::status->getStation()->getShips();
            int slot = -1;
            for (unsigned int i = 0; i < ships->size(); i++) {
                if ((*ships)[i]->getIndex() == this->selectedItem->ship->getIndex()) {
                    if (buyAdditional) {
                        ArrayRemove((*ships)[i], *ships);
                    } else {
                        // The old ship takes the place of the bought one in the station.
                        delete (*ships)[i];
                        (*ships)[i] = 0;
                        (*ships)[i] = (*Globals::ships)[oldShip->getIndex()]->makeShip(-1);
                        (*ships)[i]->adjustPrice();
                        (*ships)[i]->setRace(oldShip->getRace());
                        slot = i;
                    }
                    break;
                }
            }
            if (buyAdditional) {
                Globals::status->voidStation->addShip((*Globals::ships)[oldShip->getIndex()]->makeShip(-1));
                if (oldShip->getMods() != 0) {
                    for (unsigned int i = 0; i < oldShip->getMods()->size(); i++) {
                        Array<Ship *> *stored = Globals::status->voidStation->getShips();
                        (*stored)[stored->size() - 1]->addMod((*oldShip->getMods())[i]);
                    }
                }
            }
            if (slot >= 0 && oldShip->getMods() != 0) {
                for (unsigned int i = 0; i < oldShip->getMods()->size(); i++) {
                    if (this->upgradeMode)
                        (*Globals::status->voidStation->getShips())[slot]->addMod((*oldShip->getMods())[i]);
                    (*ships)[slot]->addMod((*oldShip->getMods())[i]);
                }
            }
            if (this->upgradeMode)
                Globals::status->voidStation->setShips(Globals::status->getStation()->getShips(), true);
            Globals::status->setShip(newShip);
            Globals::status->getShip()->refreshValue();
            this->hangarList->initShipTab(Globals::status->getShip());
            if (this->itemList != 0) {
                ArrayReleaseClasses(*this->itemList);
                delete this->itemList;
            }
            this->itemList = 0;
            this->itemList = Item::mixItems(Globals::status->getShip()->getCargo(), Globals::status->getStation()->getItems());
            this->hangarList->initShopTab(this->itemList, Globals::status->getStation()->getShips());
            this->hangarList->setCurrentTab(0, true);
            this->refreshCurrentContentHeight();
            this->dialogActive = 0;
            if (!this->upgradeMode) {
                String message = *Globals::gameText->getText(303);
                message = Globals::status->replaceHash(message, *Globals::gameText->getText(913 + newShip->getIndex()),
                                                       String("#N"));
                this->dialog->set(message);
                this->dialogActive = 1;
            }
            this->field_0x0 = 1;
            this->shipSwapPending = 0;
            this->swapChoicePending = 0;
            return false;
        }

common:
        bool buying = this->buyMode;
        int choice = this->dialog->OnTouchEnd(x, y);
        if (buying) {
            if (choice == 1) {
                this->dialogActive = 0;
                this->buyMode = 0;
            } else {
                if (choice != 0)
                    return false;
                if (!this->routeWarningPending) {
                    this->dialogActive = 0;
                    if (this->autoEquipped) {
                        for (unsigned int i = 0; i < this->tabButtons->size(); i++)
                            (*this->tabButtons)[i]->resetTouch();
                    } else {
                        this->buyMode = 1;
                    }
                    return false;
                }
                this->buyMode = 0;
                this->routeWarningPending = 0;
                this->dialogActive = 0;
            }
            this->selectedItem = 0;
            this->hangarList->setCurrentItemIndex(-1);
            return false;
        }
        if (choice != 0)
            return false;
        this->dialogActive = 0;
        ModStation *station = (ModStation *) Globals::appManager->GetApplicationModule(5);
        if (station->pendingHangarClose) {
            station->pendingHangarClose = 0;
            return true;
        }
        return false;
    }

    if (this->viewMode == 1)
        this->listItemWindow->OnTouchEnd(x, y);
    if (Globals::layout->OnTouchEnd(x, y)) {
        // Back button.
        if (this->viewMode == 1) {
            Globals::layout->resetWindowDimensions();
            this->viewMode = 0;
            return false;
        }
        if (this->hangarList->getCurrentTab() == 4) {
            this->setSellMode(false);
            this->selectedItem = 0;
            this->hangarList->setCurrentTab(2, true);
            this->refreshCargoAvailabilityForBlueprints();
            this->refreshCurrentContentHeight();
            this->scrollOffsetBackup = 0;
            this->scrollOffset = 0;
            this->hangarList->setCurrentItemIndex(-1);
            return false;
        }
        if (this->hangarList->getCurrentTab() == 3) {
            this->hangarList->setCurrentTab(0, true);
            this->refreshCargoAvailabilityForBlueprints();
            this->refreshCurrentContentHeight();
            return false;
        }
        if (this->readyToClose()) {
            this->setSellMode(false);
            this->selectedItem = 0;
            this->hangarList->setCurrentItemIndex(-1);
            return true;
        }
        return false;
    }

    this->scrollOffset += this->scrollDelta;
    this->scrollOffsetBackup = this->scrollOffset;
    this->velocity = (this->scrollDelta < 0 ? -this->scrollDelta : this->scrollDelta) > 3
                         ? (float) this->scrollDelta : 0.0f;
    this->damping = 0.9f;

    for (unsigned int i = 0; i < this->tabButtons->size(); i++) {
        if ((*this->tabButtons)[i]->OnTouchEnd(x, y)) {
            this->setSellMode(false);
            this->setSellMode(true);
            this->setSellMode(false);
            this->selectedItem = 0;
            this->hangarList->setCurrentTab(i, true);
            if (i == 2)
                this->refreshCargoAvailabilityForBlueprints();
            this->refreshCurrentContentHeight();
            this->scrollOffsetBackup = 0;
            this->scrollOffset = 0;
            this->hangarList->setCurrentItemIndex(-1);
        }
    }

    if (y > Globals::layout->field_0xc_leftMargin) {
        int row = (y - Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x20_top -
                   this->field_0x100.d - this->scrollOffset) /
                  (Globals::layout->field_0x70_rowHeight + this->field_0x100.d);
        if (row < (int) this->hangarList->getCurrentLength()) {
            this->hangarList->setCurrentItemIndex(row);
            if (this->currentItemIsHighlighted() && this->sellConfirmPending) {
                this->setSellMode(false);
                this->setSellMode(true);
            }
        }
    }
    if (this->sellConfirmPending) {
        this->sellConfirmPending = 0;
        return false;
    }

    if ((*this->buttons)[23]->OnTouchEnd(x, y)) {
        int price = this->bluePrint->getAutoCompletionPrice();
        this->dialog->set(Globals::status->replaceHash(*Globals::gameText->getText(195),
                                                       Globals::layout->formatCredits(price), String("#C")),
                          true);
        this->dialogActive = 1;
        this->autoCompletePending = 1;
    }

    if (this->currentItemIsHighlighted()) {
        for (int i = 0; i < this->buttons->size(); i++) {
            if (!(*this->buttons)[i]->OnTouchEnd(x, y))
                continue;
            int row = (y - Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x20_top -
                       this->field_0x100.d - this->scrollOffset) /
                      (Globals::layout->field_0x70_rowHeight + this->field_0x100.d);
            switch (i) {
            case 0:
                // Info button: open the item details.
                this->hangarList->setCurrentItemIndex(row);
                this->listItemWindow->set(this->hangarList->getCurrentItem(), 0, 0, 0, 0, true);
                this->viewMode = 1;
                Globals::sound->play(97, 0, 0, 0.0f);
                return false;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                this->hangarList->setCurrentItemIndex(row);
                this->selectItem(this->selectedItem);
                break;
            case 8:
                this->hangarList->setCurrentItemIndex(row);
                this->transaction(false);
                Globals::sound->play(100, 0, 0, 0.0f);
                break;
            case 9:
                this->hangarList->setCurrentItemIndex(row);
                this->transaction(true);
                Globals::sound->play(101, 0, 0, 0.0f);
                if (this->hangarList->getCurrentItem()->isItem() &&
                    this->hangarList->getCurrentItem()->item->getType() == 1) {
                    this->autoEquipPending = 1;
                    this->autoEquipIndex = this->hangarList->getCurrentItemIndex();
                }
                break;
            case 10:
                this->dialog->set(*Globals::gameText->getText(334), true);
                this->dialogActive = 1;
                this->sellShipPending = 1;
                break;
            case 11:
                Globals::options[78] = 1;
                Globals::recordHandler->saveOptions();
                this->showCreditsBuyWindow();
                break;
            }
        }
    }

    if (Globals::layout->helpPressed()) {
        if (this->viewMode == 1) {
            Globals::layout->initHelpWindow(*Globals::gameText->getText(643));
        } else {
            switch (this->hangarList->getCurrentTab()) {
            case 0:
                Globals::layout->initHelpWindow(*Globals::gameText->getText(623));
                break;
            case 1:
                Globals::layout->initHelpWindow(*Globals::gameText->getText(622));
                break;
            case 2:
                Globals::layout->initHelpWindow(*Globals::gameText->getText(625));
                break;
            case 3:
                Globals::layout->initHelpWindow(*Globals::gameText->getText(624));
                break;
            case 4:
                Globals::layout->initHelpWindow(*Globals::gameText->getText(626));
                break;
            }
        }
    }
    if ((*this->buttons)[11]->OnTouchEnd(x, y)) {
        Globals::options[78] = 1;
        Globals::recordHandler->saveOptions();
        this->showCreditsBuyWindow();
    }
    return false;

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

bool HangarWindow::currentItemIsHighlighted() {
    ListItem *item = (ListItem *) this->hangarList->getCurrentItem();
    if (item == 0) return false;
    return item == this->selectedItem;
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
        this->dialogActive = 1;
        this->replaceEquipPending = 1;
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

void HangarWindow::hideMessage() {
    this->dialogActive = 0;
}
