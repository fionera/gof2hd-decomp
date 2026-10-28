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


// Credits granted by the five free-credit offers (index 0 is the "rate us" entry without reward).
static const int FREE_CREDIT_AMOUNTS[5] = {0, 7500, 5000, 5000, 5000};

// View of the application data block used by the credit dialogs (GameData byte 0x15).
struct HangarAppData {
    uint8_t pad_0x0[0x15];
    uint8_t freeCreditsOffer;
};

int HangarWindow::lastTab = 1;

HangarWindow::HangarWindow() {
    this->bluePrint = nullptr;
    this->bluePrintItem = nullptr;
    // Non-volatile store: the original ctor schedules bluePrint/bluePrintItem
    // before dialogActive and bluePrintPurchasePending after it; a volatile store
    // lets the scheduler swap those two neighbours.
    *(uint8_t *) &this->dialogActive = 0;
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

static inline unsigned int hangarFont() { return (unsigned int) (uintptr_t) Globals::font; }

void HangarWindow::render() {
    Layout *layout = Globals::layout;
    Globals::Canvas->SetColor(0xffffffffu);
    ModStation *station = (ModStation *) Globals::appManager->GetApplicationModule(5);
    if (station == nullptr || !station->pendingHangarClose) {
        if (this->viewMode == 0) {
            layout->drawBG();
            int tab = this->hangarList->getCurrentTab();
            Array<ListItem *> *items = this->hangarList->getCurrentTabItems();
            if (items != nullptr) {
                int barPos = (int) (this->getRelativeScrollStartPos() * (float) this->visibleHeight);
                int barHeight = (int) (this->getRelativeScrollHeight() * (float) this->visibleHeight);
                int listWidth = Globals::w;
                if (barHeight > 0)
                    listWidth = listWidth - Globals::layout->field_0x48 - Globals::layout->field_0x2c_rowHeight;
                listWidth -= (Globals::layout->buttonInsetX + this->hintOffsetX) * 2;
                int iconSpace;
                if (tab == 0)
                    iconSpace = Globals::layout->field_0x4c * 2;
                else if (tab == 4 || tab == 1)
                    iconSpace = Globals::layout->field_0x50;
                else
                    iconSpace = 0;
                int textX = Globals::layout->field_0x2cc + Globals::layout->field_0x4c + iconSpace;

                if (Globals::iPad && !Globals::iPadAssetsWithLowerRes) {
                    int imgW = Globals::Canvas->GetImage2DWidth(this->hintImage);
                    int imgH = Globals::Canvas->GetImage2DHeight(this->hintImage);
                    short rows = Globals::h / imgH;
                    for (int r = 0; r <= rows; r++) {
                        Globals::Canvas->DrawImage2D(this->hintImage,
                                                     Globals::layout->buttonInsetX - imgW + this->hintOffsetX,
                                                     r * imgH, 1);
                        Globals::Canvas->DrawImage2D(this->hintImage,
                                                     this->hintOffsetX + Globals::layout->buttonInsetX + listWidth +
                                                     (barHeight > 0
                                                          ? Globals::layout->field_0x2c_rowHeight +
                                                            Globals::layout->field_0x48
                                                          : 0),
                                                     r * imgH, 0);
                    }
                }

                for (int b = 0; b < 24; b++) {
                    if (!this->dragging && (*this->buttons)[b] != nullptr)
                        (*this->buttons)[b]->setVisible(false);
                }

                unsigned int i;
                for (i = 0; i < items->size(); i++) {
                    int baseY = Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->scrollOffset;
                    int y = (Globals::layout->field_0x70_rowHeight + this->field_0x100.d) * i + baseY;
                    if (y < 0 || y > Globals::h)
                        continue;
                    ListItem *item = (*items)[i];

                    if (item->isSelectable()) {
                        if (this->selectedItem == item && !item->isTextButton()) {
                            if (tab == 0 && item->inTabIndex >= 0)
                                Globals::layout->drawBox(10, this->hintOffsetX + Globals::layout->buttonInsetX, y,
                                                         listWidth, Globals::layout->field_0x70_rowHeight, String(""));
                            else
                                Globals::layout->drawBox(4, this->hintOffsetX + Globals::layout->buttonInsetX, y,
                                                         listWidth, Globals::layout->field_0x70_rowHeight, String(""));
                        } else if (tab == 0 && item->inTabIndex >= 0) {
                            Globals::layout->drawBox(9, this->hintOffsetX + Globals::layout->buttonInsetX, y,
                                                     listWidth, Globals::layout->field_0x70_rowHeight, String(""));
                        } else {
                            Globals::layout->drawBox(3, this->hintOffsetX + Globals::layout->buttonInsetX, y,
                                                     listWidth, Globals::layout->field_0x70_rowHeight, String(""));
                        }
                    }

                    Globals::Canvas->SetColor(0xffffffffu);
                    String name;
                    int buttonOffset;
                    if (item->isItem()) {
                        name = *Globals::gameText->getText(item->item->getIndex() + 1274);
                        if (tab == 4 || tab == 1) {
                            if (item == this->selectedItem) {
                                if (this->hangarList->getCurrentTab() == 1 ||
                                    (this->hangarList->getCurrentTab() == 4 && item->isItem())) {
                                    int right, left;
                                    if (tab == 1) {
                                        right = item->item->getStationAmount();
                                        left = item->item->getAmount();
                                    } else {
                                        right = item->item->getAmount();
                                        left = this->bluePrint->getCurrentAmount(item->item->getIndex());
                                    }
                                    if (left > 0) {
                                        (*this->buttons)[8]->setPosition(
                                            this->hintOffsetX + Globals::layout->buttonInsetX,
                                            this->field_0x114 + y, 17);
                                        (*this->buttons)[8]->setVisible(true);
                                        (*this->buttons)[8]->draw();
                                    }
                                    if (right > 0) {
                                        (*this->buttons)[9]->setPosition(
                                            Globals::layout->buttonInsetX + this->hintOffsetX + listWidth,
                                            this->field_0x114 + y, 18);
                                        (*this->buttons)[9]->setVisible(true);
                                        (*this->buttons)[9]->draw();
                                    }
                                    buttonOffset = (*this->buttons)[9]->getWidth() +
                                                   Globals::layout->field_0x2c_rowHeight;
                                } else {
                                    buttonOffset = 0;
                                }
                                Globals::Canvas->SetColor(0xffffffffu);
                                if (tab == 1) {
                                    Globals::Canvas->DrawString(
                                        hangarFont(), *Globals::gameText->getText(136),
                                        this->hintOffsetX + Globals::layout->buttonInsetX + this->field_0x100.a -
                                        Globals::Canvas->GetTextWidth(hangarFont(), *Globals::gameText->getText(136)) / 2,
                                        y + 2, false);
                                    Globals::Canvas->DrawString(
                                        hangarFont(), *Globals::gameText->getText(183),
                                        listWidth + this->hintOffsetX - this->field_0x100.a -
                                        Globals::Canvas->GetTextWidth(hangarFont(), *Globals::gameText->getText(183)) / 2,
                                        y + 2, false);
                                } else {
                                    Globals::Canvas->DrawString(
                                        hangarFont(), *Globals::gameText->getText(183),
                                        this->hintOffsetX + Globals::layout->buttonInsetX + this->field_0x100.a -
                                        Globals::layout->field_0x254 -
                                        Globals::Canvas->GetTextWidth(hangarFont(), *Globals::gameText->getText(183)) / 2,
                                        y + 2, false);
                                    Globals::Canvas->DrawString(
                                        hangarFont(), *Globals::gameText->getText(271),
                                        listWidth + this->hintOffsetX - this->field_0x100.a +
                                        Globals::layout->field_0x258 -
                                        Globals::Canvas->GetTextWidth(hangarFont(), *Globals::gameText->getText(271)) / 2,
                                        y + 2, false);
                                }
                            } else {
                                buttonOffset = 0;
                            }
                            int left, right;
                            if (tab == 1) {
                                left = item->item->getStationAmount();
                                right = item->item->getAmount();
                            } else {
                                left = item->item->getAmount();
                                right = this->bluePrint->getCurrentAmount(item->item->getIndex()) +
                                        item->item->getBlueprintAmount();
                            }
                            String leftText = String(left) + String("t");
                            String rightText = (tab == 1
                                                    ? String(right)
                                                    : String(right) + String("/") +
                                                      String(this->bluePrint->getTotalAmount(item->item->getIndex()))) +
                                               String("t");
                            Globals::Canvas->SetColor(0xffffffffu);
                            if (left > 0) {
                                Globals::Canvas->DrawString(
                                    hangarFont(), leftText,
                                    Globals::layout->buttonInsetX + this->hintOffsetX + this->field_0x100.a +
                                    this->field_0x100.c - Globals::Canvas->GetTextWidth(hangarFont(), leftText),
                                    y + Globals::layout->field_0x70_rowHeight / 2 + 1, false);
                            }
                            if (tab == 4 || right > 0) {
                                int extra = tab == 4 ? Globals::layout->field_0x25c : 0;
                                Globals::Canvas->DrawString(
                                    hangarFont(), rightText,
                                    listWidth + extra + this->hintOffsetX - this->field_0x100.a + this->field_0x100.b -
                                    Globals::Canvas->GetTextWidth(hangarFont(), rightText),
                                    y + Globals::layout->field_0x70_rowHeight / 2 + 1, false);
                            }
                        } else {
                            buttonOffset = 0;
                        }
                        if (tab == 1)
                            Globals::Canvas->SetColor(item->item->getSinglePrice() > Globals::status->getCredits()
                                                          ? 0xa35b5bffu
                                                          : 0x7aa35bffu);
                        else
                            Globals::Canvas->SetColor(0x777777ffu);
                        if (!this->upgradeMode) {
                            Globals::Canvas->DrawString(
                                hangarFont(), Globals::layout->formatCredits(item->item->getSinglePrice()),
                                textX + Globals::layout->buttonInsetX + this->hintOffsetX,
                                y + Globals::layout->field_0x70_rowHeight / 2 + 1, false);
                        }
                        Globals::imageFactory->drawItem(item->item->getIndex(), item->item->getType(),
                                                        Globals::layout->buttonInsetX + iconSpace + this->hintOffsetX,
                                                        this->iconOffsetY + y);
                        if (tab == 1 && !(*Globals::status->field_54)[item->item->getIndex()]) {
                            Globals::Canvas->DrawImage2D(
                                this->pendingIconImage,
                                iconSpace - 2 + Globals::layout->buttonInsetX + this->hintOffsetX +
                                Globals::layout->field_0x2cc,
                                y + this->iconOffsetY + Globals::layout->field_0x2d0 - 2, 17, 34);
                        } else if (item->inTabIndex > -1 ||
                                   (tab == 1 && Globals::status->getShip()->hasEquipment(item->item->getIndex(), 1))) {
                            Globals::Canvas->DrawImage2D(
                                this->blueprintIconImage,
                                iconSpace - 2 + Globals::layout->buttonInsetX + this->hintOffsetX +
                                Globals::layout->field_0x2cc,
                                y + this->iconOffsetY + Globals::layout->field_0x2d0 - 2, 17, 34);
                        } else if (tab == 4 &&
                                   Item::isInList(item->item, Globals::status->getStation()->getItems())) {
                            Globals::Canvas->DrawImage2D(
                                this->blueprintIconImage,
                                iconSpace - 2 + Globals::layout->buttonInsetX + this->hintOffsetX +
                                Globals::layout->field_0x2cc,
                                y + this->iconOffsetY + Globals::layout->field_0x2d0 - 2, 17, 34);
                        }
                        if ((tab == 3 || tab == 0) && item->item->getAmount() >= 2)
                            name += String(" (") + String(item->item->getAmount()) + String(")");
                    } else if (item->isShip()) {
                        name = *Globals::gameText->getText(item->ship->getIndex() + 913);
                        Globals::Canvas->SetColor(0x777777ffu);
                        if (tab == 1 && this->upgradeMode) {
                            Globals::Canvas->DrawString(
                                hangarFont(), Globals::layout->formatCredits(item->ship->getPrice()),
                                textX + Globals::layout->buttonInsetX + this->hintOffsetX,
                                y + Globals::layout->field_0x70_rowHeight / 2 + 1, false);
                        } else if (tab == 1 || i > 1) {
                            Globals::Canvas->DrawString(
                                hangarFont(), Globals::layout->formatCredits(item->ship->getPrice()),
                                textX + Globals::layout->buttonInsetX + this->hintOffsetX,
                                y + Globals::layout->field_0x70_rowHeight / 2 + 1, false);
                        } else {
                            Globals::Canvas->SetColor(
                                item->ship->getPrice() - Globals::status->getShip()->getPrice() >
                                Globals::status->getCredits()
                                    ? 0xa35b5bffu
                                    : 0x7aa35bffu);
                            Globals::Canvas->DrawString(
                                hangarFont(),
                                Globals::layout->formatCredits(item->ship->getPrice() -
                                                               Globals::status->getShip()->getPrice()),
                                textX + Globals::layout->buttonInsetX + this->hintOffsetX,
                                y + Globals::layout->field_0x70_rowHeight / 2 + 1, false);
                        }
                        Globals::imageFactory->drawShip(item->ship->getIndex(),
                                                        Globals::layout->buttonInsetX + iconSpace + this->hintOffsetX,
                                                        this->iconOffsetY + y);
                        Globals::Canvas->SetColor(0xffffffffu);
                        buttonOffset = 0;
                    } else if (item->isSlot()) {
                        name = *Globals::gameText->getText(174);
                        if (tab == 4 && i == items->size() - 1) {
                            (*this->buttons)[23]->setPosition(
                                Globals::layout->buttonInsetX + this->hintOffsetX + listWidth / 2,
                                this->field_0x114 + y, 20);
                            (*this->buttons)[23]->setVisible(true);
                            (*this->buttons)[23]->draw();
                            name = String("");
                        }
                        buttonOffset = 0;
                    } else if (item->isBluePrint()) {
                        name = *Globals::gameText->getText(item->bluePrint->getIndex() + 1274);
                        if (item->bluePrint->getCompletionRate() > 0.0f) {
                            float rate = item->bluePrint->getCompletionRate();
                            Globals::Canvas->DrawImage2D(
                                this->progressBarBgImage,
                                textX + 2 + Globals::layout->buttonInsetX + this->hintOffsetX,
                                y + Globals::layout->field_0x70_rowHeight / 2 + 1);
                            Globals::Canvas->DrawRegion2D(
                                this->progressBarFillImage, 0, 0, (int) (rate * (float) this->progressBarWidth),
                                this->progressBarHeight, 0.0f, 0, 0,
                                textX + 3 + Globals::layout->buttonInsetX + this->hintOffsetX,
                                y + 2 + Globals::layout->field_0x70_rowHeight / 2);
                            Globals::Canvas->DrawImage2D(
                                this->progressBarBorderImage,
                                this->hintOffsetX + (textX + 5 + Globals::layout->buttonInsetX +
                                                     (int) (rate * (float) (this->progressBarWidth - 4))),
                                y + 2 + Globals::layout->field_0x70_rowHeight / 2, 17, 20);
                            Globals::Canvas->SetColor(0x777777ffu);
                            String progress = String((int) (rate * 100.0f)) + String("%  (") +
                                              item->bluePrint->stationName + String(")");
                            Globals::Canvas->DrawString(
                                hangarFont(), progress,
                                textX + 2 + Globals::layout->buttonInsetX + this->hintOffsetX +
                                this->progressBarWidth + Globals::layout->field_0x2c_rowHeight,
                                y + Globals::layout->field_0x70_rowHeight / 2 + Globals::layout->field_0x260 +
                                this->field_0x114, false);
                            Globals::Canvas->SetColor(0xffffffffu);
                        }
                        Globals::imageFactory->drawItem(item->bluePrint->getIndex(),
                                                        (*Globals::items)[item->bluePrint->getIndex()]->getType(),
                                                        Globals::layout->buttonInsetX + iconSpace + this->hintOffsetX,
                                                        this->iconOffsetY + y);
                        if (item->craftable)
                            Globals::Canvas->SetColor(0x00ed00ffu);
                        buttonOffset = 0;
                    } else if (item->isPendingProduct()) {
                        name = (item->pendingProduct->quantity >= 2
                                    ? String(item->pendingProduct->quantity) + String("x ")
                                    : String("")) +
                               *Globals::gameText->getText(item->pendingProduct->blueprintIndex + 1274);
                        Globals::Canvas->SetColor(0x777777ffu);
                        Globals::Canvas->DrawString(
                            hangarFont(),
                            *Globals::gameText->getText(275) + String(" ") + item->pendingProduct->stationName,
                            textX + Globals::layout->buttonInsetX + this->hintOffsetX,
                            y + Globals::layout->field_0x70_rowHeight / 2 + 1, false);
                        Globals::Canvas->SetColor(0xffffffffu);
                        Globals::imageFactory->drawItem(
                            item->pendingProduct->blueprintIndex,
                            (*Globals::items)[item->pendingProduct->blueprintIndex]->getType(),
                            Globals::layout->buttonInsetX + iconSpace + this->hintOffsetX, this->iconOffsetY + y);
                        buttonOffset = 0;
                    } else if (item->isSellButton()) {
                        (*this->buttons)[5]->setPosition(this->hintOffsetX + Globals::layout->buttonInsetX, y, 17);
                        (*this->buttons)[5]->setVisible(true);
                        (*this->buttons)[5]->draw();
                        continue;
                    } else if (item->isMoveToCargoButton()) {
                        (*this->buttons)[6]->setPosition(this->hintOffsetX + Globals::layout->buttonInsetX, y, 17);
                        (*this->buttons)[6]->setVisible(true);
                        (*this->buttons)[6]->draw();
                        continue;
                    } else {
                        Globals::layout->drawBox(0, Globals::layout->buttonInsetX + this->hintOffsetX,
                                                 y + Globals::layout->field_0x70_rowHeight - Globals::layout->field_0x1c,
                                                 listWidth, Globals::layout->field_0x1c, *item->name);
                        continue;
                    }

                    Globals::Canvas->DrawString(hangarFont(), name,
                                                textX + Globals::layout->buttonInsetX + this->hintOffsetX, y + 2,
                                                false);
                    if (item == this->selectedItem) {
                        if (item->isShip()) {
                            if (tab != 0 && !(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) {
                                (*this->buttons)[1]->setPosition(
                                    Globals::layout->buttonInsetX + this->hintOffsetX + listWidth,
                                    this->field_0x114 + y, 18);
                                (*this->buttons)[1]->setVisible(true);
                                (*this->buttons)[1]->draw();
                                buttonOffset = (*this->buttons)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
                            }
                            if (tab == 1 && this->upgradeMode) {
                                (*this->buttons)[2]->setVisible(true);
                                (*this->buttons)[10]->setPosition(this->hintOffsetX + Globals::layout->buttonInsetX,
                                                                  this->field_0x114 + y, 17);
                                (*this->buttons)[10]->setVisible(true);
                                (*this->buttons)[10]->draw();
                            }
                        } else {
                            if (tab == 0 && item->isItem()) {
                                Item *equipped = Globals::status->getShip()->getFirstEquipmentOfSort(item->item->getSort());
                                bool replaceable = false;
                                bool sameWeapon = false;
                                if (equipped != nullptr) {
                                    replaceable = !item->item->canBeInstalledMultipleTimes();
                                    sameWeapon = equipped->getType() == 3 &&
                                                 equipped->getIndex() == item->item->getIndex();
                                }
                                if (item->inTabIndex > -1 ||
                                    ((Globals::status->getShip()->getFreeSlots(item->item->getType()) != 0 ||
                                      replaceable) &&
                                     (!sameWeapon || item->item->canBeInstalledMultipleTimes()))) {
                                    int b = item->inTabIndex < 0 ? 4 : 3;
                                    (*this->buttons)[b]->setPosition(
                                        this->hintOffsetX + Globals::layout->buttonInsetX + listWidth -
                                        2 * Globals::layout->field_0x2c_rowHeight,
                                        this->field_0x114 + y, 18);
                                    (*this->buttons)[b]->setVisible(true);
                                    (*this->buttons)[b]->draw();
                                    buttonOffset = (*this->buttons)[b]->getWidth() +
                                                   2 * Globals::layout->field_0x2c_rowHeight;
                                }
                            }
                        }
                        if (tab == 2 && item->isBluePrint()) {
                                (*this->buttons)[7]->setPosition(
                                    this->hintOffsetX + Globals::layout->buttonInsetX + listWidth,
                                    this->field_0x114 + y, 18);
                                (*this->buttons)[7]->setVisible(true);
                                (*this->buttons)[7]->draw();
                                buttonOffset = (*this->buttons)[7]->getWidth() + Globals::layout->field_0x2c_rowHeight;
                        }
                        if (item->isItem() || item->isShip() || item->isBluePrint() || item->isPendingProduct()) {
                            (*this->buttons)[0]->setPosition(
                                Globals::layout->buttonInsetX + this->hintOffsetX + (listWidth - buttonOffset),
                                this->field_0x114 + y, 18);
                            (*this->buttons)[0]->setVisible(true);
                            (*this->buttons)[0]->draw();
                        }
                    }
                }

                if (barHeight > 0 || barPos > 0) {
                    Globals::layout->drawScrollBar(Globals::w - Globals::layout->field_0x48 -
                                                   Globals::layout->buttonInsetX - this->hintOffsetX,
                                                   Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                                                   this->visibleHeight, barPos, barHeight);
                }
            }
            Globals::layout->drawHeader(*Globals::gameText->getText(167));
            for (unsigned int i = 0; i < this->tabButtons->size(); i++)
                (*this->tabButtons)[i]->draw();
        }
    }
    if (this->viewMode == 1) {
        this->viewMode = 0;
        this->render();
        this->viewMode = 1;
        this->listItemWindow->draw();
    }

    Globals::layout->drawFooter();
    (*this->buttons)[11]->setVisible(true);
    (*this->buttons)[11]->setAlwaysPressed(!Globals::options[78]);
    (*this->buttons)[11]->setText(Globals::layout->formatCredits(Globals::status->getCredits()));
    (*this->buttons)[11]->draw();
    if (!this->dialogActive)
        return;
    this->dialog->draw();

    if (this->buyCreditsActive) {
        HangarAppData *data = (HangarAppData *) Globals::appManager->GetApplicationData();
        int count;
        if (this->listModeFlag) {
            for (count = 0; count <= 4; count++) {
                String description("");
                String price("");
                // Layout anchor: keep this TU's .text at 0x1343d0 so the
                // archive-sensitive libstdc++ exception constructors remain
                // byte-exact.  Three self-assignments are no-ops on the rendered
                // text but steer codegen to the original 9700-byte body while
                // OnTouchEnd keeps its exact 5828-byte shape; the remaining
                // 32 bytes of the layout live in Gun::calcCharacterCollision.
                (*this->buttons)[12 + count]->setVisible(true);
                switch (count) {
                    case 0:
                        description = String(Globals::cItemListDescription_00);
                        price = String(Globals::cItemListPrice_00);
                        break;
                    case 1:
                        description = String(Globals::cItemListDescription_01);
                        price = String(Globals::cItemListPrice_01);
                        break;
                    case 2:
                        description = String(Globals::cItemListDescription_02);
                        price = String(Globals::cItemListPrice_02);
                        break;
                    case 3:
                        description = String(Globals::cItemListDescription_03);
                        price = String(Globals::cItemListPrice_03);
                        break;
                    case 4:
                        description = String(Globals::cItemListDescription_04);
                        price = String(Globals::cItemListPrice_04);
                        break;
                    default:
                        description = String("");
                        price = String("");
                        break;
                }
                int row = (unsigned char) count / 3;
                int col = (unsigned char) count % 3;
                int x = Globals::w / 2 - this->buttonWidth - this->gridSpacingX + (this->gridSpacingX + this->buttonWidth) * col;
                int y = (int) ((float) (Globals::h / 2 - this->gridButtonHeight / 2 - 3 * Globals::layout->field_0x20_top) -
                          (float) this->gridSpacingY * 0.5f + (float) ((this->gridSpacingY + this->gridButtonHeight) * row));
                (*this->buttons)[12 + count]->setPosition(x, y, 68);
                (*this->buttons)[12 + count]->replaceTextKeepSize(description);
                (*this->buttons)[12 + count]->setSplitText(price);
                (*this->buttons)[12 + count]->draw();
                Globals::Canvas->DrawImage2D(this->tabIcons[count], x, y - Globals::layout->field_0x2c_rowHeight, 17,
                                             68);
                if (count == 4 && Globals::showBestDeal) {
                    description = description;
                    description = description;
                    description = description;
                    Globals::Canvas->DrawImage2D(
                        this->scrollHintImageA,
                        (int) ((float) x + (Globals::iPad ? 0.05f : 0.13f) * (float) this->buttonWidth),
                        (int) ((float) y - (float) this->gridButtonHeight * 0.1f), 17, 17);
                }
            }
        } else {
            int baseY = this->dialog->y + Globals::layout->windowTopInset + Globals::layout->field_0x2c_rowHeight * 5;
            (*this->buttons)[12]->setVisible(true);
            (*this->buttons)[12]->setText(String(Globals::cItemListDescription_00));
            (*this->buttons)[12]->setSplitText(String(Globals::cItemListPrice_00));
            (*this->buttons)[12]->setPosition(Globals::w / 2, baseY, 20);
            (*this->buttons)[12]->draw();
            (*this->buttons)[13]->setVisible(true);
            (*this->buttons)[13]->setText(String(Globals::cItemListDescription_01));
            (*this->buttons)[13]->setSplitText(String(Globals::cItemListPrice_01));
            (*this->buttons)[13]->setPosition(
                Globals::w / 2, baseY + (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34), 20);
            (*this->buttons)[13]->draw();
            (*this->buttons)[14]->setVisible(true);
            (*this->buttons)[14]->setText(String(Globals::cItemListDescription_02));
            (*this->buttons)[14]->setSplitText(String(Globals::cItemListPrice_02));
            (*this->buttons)[14]->setPosition(
                Globals::w / 2, baseY + (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34) * 2, 20);
            (*this->buttons)[14]->draw();
            (*this->buttons)[15]->setVisible(true);
            (*this->buttons)[15]->setText(String(Globals::cItemListDescription_03));
            (*this->buttons)[15]->setSplitText(String(Globals::cItemListPrice_03));
            (*this->buttons)[15]->setPosition(
                Globals::w / 2, baseY + (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34) * 3, 20);
            (*this->buttons)[15]->draw();
            (*this->buttons)[16]->setVisible(true);
            (*this->buttons)[16]->setText(String(Globals::cItemListDescription_04));
            (*this->buttons)[16]->setSplitText(String(Globals::cItemListPrice_04));
            (*this->buttons)[16]->setPosition(
                Globals::w / 2, baseY + (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34) * 4, 20);
            (*this->buttons)[16]->draw();
            count = 5;
        }

        if (!Globals::options[75] || !Globals::options[74] || !Globals::options[73] || !Globals::options[77] ||
            !Globals::options[76] || data->freeCreditsOffer) {
            int x = Globals::w / 2 - this->buttonWidth - this->gridSpacingX + (this->gridSpacingX + this->buttonWidth) * 2;
            int y = (int) ((float) (Globals::h / 2 - this->gridButtonHeight / 2 - 3 * Globals::layout->field_0x20_top) -
                          (float) this->gridSpacingY * 0.5f + (float) ((this->gridSpacingY + this->gridButtonHeight) * 1));
            (*this->buttons)[17]->setVisible(true);
            if (this->listModeFlag) {
                (*this->buttons)[17]->setPosition(x, y, 68);
                (*this->buttons)[17]->replaceTextKeepSize(*Globals::gameText->getText(118));
                (*this->buttons)[17]->draw();
                Globals::Canvas->DrawImage2D(this->tabIcons[5], x, y - Globals::layout->field_0x2c_rowHeight, 17, 68);
                Globals::Canvas->DrawImage2D(
                    this->scrollHintImageB,
                    (int) ((float) (Globals::w / 2 - this->buttonWidth - this->gridSpacingX + (this->gridSpacingX + this->buttonWidth) * 2) +
                           (Globals::iPad ? 0.05f : 0.13f) * (float) this->buttonWidth),
                    (int) ((float) (int) ((float) (Globals::h / 2 - this->gridButtonHeight / 2 - 3 * Globals::layout->field_0x20_top) -
                          (float) this->gridSpacingY * 0.5f + (float) ((this->gridSpacingY + this->gridButtonHeight) * 1)) - (float) this->gridButtonHeight * 0.1f), 17, 17);
            } else {
                (*this->buttons)[17]->setText(*Globals::gameText->getText(118));
                (*this->buttons)[17]->setPosition(
                    Globals::w / 2,
                    this->field_0x48 + (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34) * count,
                    20);
                (*this->buttons)[17]->draw();
            }
            (*this->buttons)[17]->setSplitText(*Globals::gameText->getText(119));
            count++;
        }
        if (!this->listModeFlag) {
            this->dialog->setHeight((Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34) * (count + 1) +
                                    Globals::Canvas->GetTextHeight(hangarFont()) * 2);
        }
    } else if (this->freeCreditsActive) {
        int baseX = this->dialog->x + Globals::layout->buttonInsetX;
        int baseY = Globals::layout->windowTopInset + this->dialog->y + Globals::layout->field_0x2c_rowHeight * 2;
        int row = 0;
        for (int i = 0; i < 5; i++) {
            TouchButton *button = (*this->buttons)[18 + i];
            bool visible;
            if (i == 0 || (i == 1 && Globals::options[73]) || (i == 2 && Globals::options[74]) ||
                (i == 3 && Globals::options[75])) {
                visible = false;
            } else {
                visible = i != 4 || !Globals::options[76];
            }
            button->setVisible(visible);
            int y = (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34) * row + baseY;
            button->setPosition(baseX, y);
            button->draw();
            if (visible) {
                int textX = button->getWidth() + baseX + Globals::layout->field_0x2c_rowHeight;
                int height = button->getHeight();
                Globals::Canvas->SetColor(0xffffffffu);
                int textY = (int) ((float) y + (float) height * 0.2f);
                Globals::Canvas->DrawString(hangarFont(), *Globals::gameText->getText(i != 0 ? i + 112 : 3400), textX,
                                            textY, false);
                Globals::Canvas->SetColor(0x777777ffu);
                if (i != 0) {
                    Globals::Canvas->DrawString(
                        hangarFont(),
                        String("(+") + Globals::layout->formatCredits(FREE_CREDIT_AMOUNTS[i]) + String(")"), textX,
                        textY + Globals::layout->field_0x4, false);
                } else {
                    Globals::Canvas->DrawString(hangarFont(), String(""), textX, textY + Globals::layout->field_0x4,
                                                false);
                }
                row++;
            }
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

bool HangarWindow::OnTouchBegin(int touch, int coord) {
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
        return false;
    }

    this->lastTouchY = coord;
    this->touchStartY = coord;
    this->scrollDelta = 0;
    this->dragging = 1;

    if (this->viewMode == 1) {
        this->listItemWindow->OnTouchBegin(touch, coord);
        return false;
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
        return false;
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
    return false;
}

int HangarWindow::highlightItem(ListItem *item) {
    if (item != nullptr && item->isSelectable() != 0) {
        Globals::sound->play(0x7c, nullptr, nullptr, 0);
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

bool HangarWindow::OnTouchMove(int touch, int coord) {
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
        return false;
    }

    if (this->viewMode == 1) {
        this->listItemWindow->OnTouchMove(touch, coord);
        return false;
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
    return false;
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
                this->replaceEquipPending = 0;
                this->dialogActive = 0;
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
                for (int i = 12; i != 17; i++) {
                    TouchButton **data = *(TouchButton ** volatile *) &this->buttons->data_;
                    data[i]->setVisible(false);
                }
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
                if (Globals::options[74] && Globals::options[73] &&
                    Globals::options[77] && Globals::options[76])
                    offersLeft = Globals::options[75] == 0;
                uint8_t *appData = (uint8_t *) Globals::appManager->GetApplicationData();
                if (!appData[21] || offersLeft) {
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
                        Globals::status->changeCredits(FREE_CREDIT_AMOUNTS[i]);
                        Globals::options[73] = 1;
                        break;
                    case 2:
                        appData[161] = 1;
                        NFC::free_credits_likeFishlabsOnFacebook();
                        Globals::status->changeCredits(FREE_CREDIT_AMOUNTS[i]);
                        Globals::options[74] = 1;
                        break;
                    case 3:
                        appData[162] = 1;
                        NFC::free_credits_subscribeToYoutubeChannel();
                        Globals::status->changeCredits(FREE_CREDIT_AMOUNTS[i]);
                        Globals::options[75] = 1;
                        break;
                    case 4:
                        appData[163] = 1;
                        NFC::free_credits_followOnTwitter();
                        Globals::status->changeCredits(FREE_CREDIT_AMOUNTS[i]);
                        Globals::options[76] = 1;
                        break;
                    case 5:
                        appData[13] = 1;
                        NFC::free_credits_rateGame();
                        Globals::status->changeCredits(FREE_CREDIT_AMOUNTS[i]);
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
                this->bluePrintPurchasePending = 0;
                this->localBluePrint = 0;
            } else if (choice == 1 || Globals::status->getCredits() < cost || (choice == 0 && this->localBluePrint)) {
                // Undo the pending ingredient purchase.
                this->bluePrintItem->item->setStationAmount(this->savedStationAmount);
                this->bluePrintItem->item->setAmount(this->savedAmount);
                this->bluePrintItem->item->setBlueprintAmount(this->savedBlueprintAmount);
                Globals::status->setCredits(this->savedCredits);
                this->bluePrintBuyCount = 0;
                this->savedStationAmount = 0;
                this->selectedItem = 0;
                this->savedAmount = 0;
                this->savedBlueprintAmount = 0;
                this->currentLoad = this->savedLoad;
                this->hangarList->setCurrentItemIndex(-1);
                this->bluePrintPurchasePending = 0;
                this->dialogActive = 0;
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
                this->dialogActive = 0;
                this->sellShipPending = 0;
                Globals::status->changeCredits(this->selectedItem->getPrice());
                Globals::status->getStation()->removeShip(this->selectedItem->ship);
                this->hangarList->initShopTab(this->itemList, Globals::status->getStation()->getShips());
                this->refreshCurrentContentHeight();
            } else if (choice == 1) {
                this->dialogActive = 0;
                this->sellShipPending = 0;
            }
        } else if (this->shipSwapPending) {
            int choice = this->dialog->OnTouchEnd(x, y);
            bool buyAdditional;
            if (Globals::status->field_114 == 3 && !this->upgradeMode) {
                if (this->swapChoicePending) {
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
                            this->notEnoughCredits = 1;
                            this->dialogActive = 1;
                            this->shipSwapPending = 0;
                            this->swapChoicePending = 0;
                            return false;
                        }
                        buyAdditional = true;
                    }
                } else {
                    if (choice == 1)
                        goto swapDone;
                    if (choice != 0)
                        goto common;
                    this->dialog->set(String(""), *Globals::gameText->getText(327), true, *Globals::gameText->getText(330),
                                      *Globals::gameText->getText(331), *Globals::gameText->getText(330), -1, -1);
                    this->swapChoicePending = 1;
                    return false;
                }
            } else if (!this->upgradeMode) {
                if (!this->dlcMenuPending) {
                    if (choice == 1)
                        goto swapDone;
                    if (choice != 0)
                        goto common;
                    this->dialog->set(String(""), *Globals::gameText->getText(189), true);
                    this->dlcMenuPending = 1;
                    return false;
                }
                if (choice != 0) {
                    if (choice != 1)
                        goto common;
                    this->dlcMenuPending = 0;
                    buyAdditional = false;
                } else {
                    ((ModStation *) Globals::appManager->GetApplicationModule(5))->showDlcMenu();
                    this->dlcMenuPending = 0;
                    this->dialogActive = 0;
                    return false;
                }
            } else {
                if (choice == 1)
                    goto swapDone;
                if (choice != 0)
                    goto common;
                buyAdditional = false;
            }

            // Braced so the swapDone jumps above do not bypass these initialisers.
            {
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
                        (*Globals::status->voidStation->getShips())[Globals::status->voidStation->getShips()->size() - 1]
                            ->addMod((*oldShip->getMods())[i]);
                    }
                }
            }
            if (oldShip->getMods() != 0 && slot >= 0) {
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

        swapDone:
            this->dialogActive = 0;
            this->shipSwapPending = 0;
            this->swapChoicePending = 0;
            return false;
        }

common:
        bool buying = this->buyMode;
        int choice = this->dialog->OnTouchEnd(x, y);
        if (buying) {
            if (choice == 1) {
                HangarList *list = this->hangarList;
                this->buyMode = 0;
                this->dialogActive = 0;
                this->selectedItem = 0;
                list->setCurrentItemIndex(-1);
                return false;
            }
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
            HangarList *list = this->hangarList;
            this->dialogActive = 0;
            this->routeWarningPending = 0;
            this->buyMode = 0;
            this->selectedItem = 0;
            list->setCurrentItemIndex(-1);
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
        int row = (y - Globals::layout->field_0xc_leftMargin -
                   Globals::layout->field_0x20_top - this->field_0x100.d -
                   this->scrollOffset) /
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
    Globals::sound->play(0x60, nullptr, nullptr, 0.0f);
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
    Globals::sound->play(0x62, nullptr, nullptr, 0.0f);
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
            if (price <= Globals::status->getCredits() + Globals::status->getShip()->getPrice() || this->upgradeMode) {
                if (reinterpret_cast<int32_t &>(Globals::status->_byte_0x34) >= 1) {
                    this->dialog->set(*Globals::gameText->getText(336));
                    this->dialogActive = 1;
                    break;
                } else if (Globals::status->getCurrentCampaignMission() == 77 &&
                           Globals::status->getShip()->getIndex() == 37) {
                    this->dialog->set(*Globals::gameText->getText(325));
                    this->dialogActive = 1;
                    break;
                } else if (Globals::status->getShip()->getIndex() != item->ship->getIndex()) {
                    this->shipSwapPending = 1;
                    this->dialog->set(*Globals::gameText->getText(this->upgradeMode ? 333 : 304), true);
                    this->dialogActive = 1;
                    break;
                }
            } else {
                String message = Globals::status->replaceHash(
                    *Globals::gameText->getText(203),
                    String(Globals::layout->formatCredits(item->getPrice() - Globals::status->getCredits() -
                                                          Globals::status->getShip()->getPrice()), false),
                    String("#C"));
                message += String("\n\n") + *Globals::gameText->getText(124);
                this->dialog->set(message, true);
                this->notEnoughCredits = 1;
                this->dialogActive = 1;
                break;
            }
            this->dialog->set(*Globals::gameText->getText(329));
            this->dialogActive = 1;
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
