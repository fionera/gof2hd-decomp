#include "class.h"
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
extern "C" extern const char hw_ote_fmt1[], hw_ote_fmt2[], hw_ote_fmt3[], hw_ote_fmt4[];

extern "C" void HangarWindow_OnTouchEnd(HangarWindow *self, int touch, int coord)
{
    void *globals = *g_hw_globals;
    self->f_6c = 0;
    self->f_70 = 0;
    self->f_d0 = 0;
    if (F<uint8_t>(self, 0xd1) != 0) {
        F<uint8_t>(self, 0xd1) = 0;
        return;
    }

    if (self->f_3c == 0) {
        // --- No modal dialog active: normal list / button handling. ---
        if (self->f_58 == 1)
            ListItemWindow_OnTouchEnd(self->f_18, touch);

        void *layout = *g_hw_layout;
        int handled = Layout_OnTouchEnd(layout, touch, coord);
        if (handled == 0) {
            // Inertial scroll bookkeeping.
            int delta = self->f_c0;
            int newScroll = self->f_b4 + delta;
            float vel = (float)delta;
            int absd = delta < 0 ? -delta : delta;
            self->f_c8 = (absd > 3) ? (int)vel : 0;
            self->f_c4 = 0;
            self->f_b4 = newScroll;
            self->f_bc = newScroll;

            Array<void *> *tabs = (Array<void *> *)self->f_4;
            for (unsigned int i = 0; i < tabs->length; i++) {
                if (TouchButton_OnTouchEnd(tabs->data[i], touch) != 0) {
                    HangarWindow_setSellMode(self);
                    HangarWindow_setSellMode(self);
                    HangarWindow_setSellMode(self);
                    self->f_68 = 0;
                    HangarList_setCurrentTab(self->f_14, i != 0);
                    if (i == 2)
                        HangarWindow_refreshCargoAvailabilityForBlueprints(self);
                    HangarWindow_refreshCurrentContentHeight(self);
                    self->f_b4 = 0;
                    self->f_bc = 0;
                    HangarList_setCurrentItemIndex(self->f_14, -1);
                }
            }

            if (G<int>(layout, 0xc) < coord) {
                int row = __aeabi_idiv(
                    (((coord - G<int>(layout, 0xc)) - G<int>(layout, 0x20)) - self->f_10c) -
                        self->f_b4,
                    G<int>(layout, 0x70) + self->f_10c);
                if (row < HangarList_getCurrentLength(self->f_14)) {
                    HangarList_setCurrentItemIndex(self->f_14, row);
                    if (HangarWindow_currentItemIsHighlighted(self) != 0 &&
                        F<uint8_t>(self, 0xd2) != 0) {
                        HangarWindow_setSellMode(self);
                        HangarWindow_setSellMode(self);
                    }
                }
            }
            if (F<uint8_t>(self, 0xd2) != 0) {
                F<uint8_t>(self, 0xd2) = 0;
                return;
            }

            // "Auto-complete blueprint" button.
            if (TouchButton_OnTouchEnd(G<void *>(G<void *>(self->f_24, 4), 0x5c), touch) != 0) {
                BluePrint_getAutoCompletionPrice(self->f_80);
                String12 line, priceStr, fmt, msg;
                AEString_ctor(&line, (const char *)GameText_getText(*g_hw_bpDoneTextId), false);
                Layout_formatCredits(&priceStr, 0);
                AEString_ctor(&fmt, hw_ote_fmt1, false);
                Status_replaceHash(&msg, globals, &line, &priceStr);
                ChoiceWindow_setMsg(self->f_20, &msg, true);
                AEString_dtor(&msg);
                AEString_dtor(&fmt);
                AEString_dtor(&priceStr);
                AEString_dtor(&line);
                F<uint8_t>(self, 0xb1) = 1;
                self->f_3c = 1;
            }

            if (HangarWindow_currentItemIsHighlighted(self) != 0) {
                Array<void *> *btns = (Array<void *> *)self->f_24;
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
                if (self->f_58 == 1) {
                    AEString_ctor(&help, (const char *)GameText_getText(*g_hw_helpTextId), false);
                    Layout_initHelpWindow(layout, &help);
                    AEString_dtor(&help);
                } else {
                    unsigned int t = HangarList_getCurrentTab(self->f_14);
                    if (t <= 4) {
                        AEString_ctor(&help, (const char *)GameText_getText(*g_hw_helpTextId), false);
                        Layout_initHelpWindow(layout, &help);
                        AEString_dtor(&help);
                    }
                }
            }

            // "Buy credits" footer button.
            if (TouchButton_OnTouchEnd(G<void *>(G<void *>(self->f_24, 4), 0x2c), touch) != 0) {
                g_hw_optionFlags[0x4e] = 1;
                RecordHandler_saveOptions(*g_hw_recordHandler);
                HangarWindow_showCreditsBuyWindow(self);
            }
            return;
        }

        if (self->f_58 == 1) {
            Layout_resetWindowDimensions(layout);
            self->f_58 = 0;
            return;
        }

        unsigned int tab = HangarList_getCurrentTab(self->f_14);
        if (tab == 4) {
            HangarWindow_setSellMode(self);
            self->f_68 = 0;
            HangarList_setCurrentTab(self->f_14, true);
            HangarWindow_refreshCargoAvailabilityForBlueprints(self);
            HangarWindow_refreshCurrentContentHeight(self);
            self->f_b4 = 0;
            self->f_bc = 0;
            HangarList_setCurrentItemIndex(self->f_14, -1);
            return;
        }
        if (HangarList_getCurrentTab(self->f_14) == 3) {
            HangarList_setCurrentTab(self->f_14, false);
            HangarWindow_refreshCargoAvailabilityForBlueprints(self);
            HangarWindow_refreshCurrentContentHeight(self);
        } else if (HangarWindow_readyToClose(self) != 0) {
            HangarWindow_setSellMode(self);
            self->f_68 = 0;
            HangarList_setCurrentItemIndex(self->f_14, -1);
        }
        return;
    }

    // --- A modal dialog is active (self+0x3c set). Route to the right handler. ---
    if (F<uint8_t>(self, 0xb1) != 0) {
        // Blueprint auto-completion confirmation.
        int r = ChoiceWindow_OnTouchEnd(self->f_20, touch);
        if (r == 1) {
            self->f_3c = 0;
        } else if (r == 0) {
            int price = BluePrint_getAutoCompletionPrice(self->f_80);
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
                ChoiceWindow_setMsg(self->f_20, &msg, true);
                F<uint8_t>(self, 0xaf) = 1;
                AEString_dtor(&msg);
            } else {
                self->f_3c = 0;
                if (self->f_80 != 0) {
                    if (BluePrint_isEmpty(self->f_80) != 0) {
                        G<int>(self->f_80, 0x10) = Station_getIndex(Status_getStation());
                        Station_getName(Status_getStation());
                        // String12 nameOut; assign into bp+0x14 (skipped: name handled inline)
                    }
                    BluePrint_complete(self->f_80);
                    HangarWindow_highlightItem(self,
                        HangarList_getCurrentItemAt(self->f_14, 1));
                    self->f_88 = 1;
                    HangarWindow_setSellMode(self);
                    Status_changeCredits(globals);
                }
            }
        }
        F<uint8_t>(self, 0xb1) = 0;
        return;
    }

    if (self->f_11c != 0) {
        // Equipment-replacement confirmation.
        int r = ChoiceWindow_OnTouchEnd(self->f_20, touch);
        if (r == 1) {
            self->f_3c = 0;
            self->f_11c = 0;
            self->f_b4 = self->f_e4;
        } else if (r == 0 && self->f_28 != 0 && self->f_2c != 0) {
            HangarWindow_demountItem(self, self->f_2c, -1);
            self->f_e4 = self->f_b4;
            HangarWindow_mountItem(self, self->f_28);
            self->f_3c = 0;
            self->f_11c = 0;
        }
        // fallthrough to buy/sell confirmation below
    } else if (F<uint8_t>(self, 0xaf) != 0) {
        // "Not enough credits" -> offer to buy credits.
        int r = ChoiceWindow_OnTouchEnd(self->f_20, touch);
        if (r == 1) {
            F<uint8_t>(self, 0xaf) = 0;
            self->f_3c = 0;
        } else if (r == 0) {
            g_hw_optionFlags[0x4e] = 1;
            RecordHandler_saveOptions(*g_hw_recordHandler);
            HangarWindow_showCreditsBuyWindow(self);
        }
    } else if (F<uint8_t>(self, 0xae) == 0) {
        if (self->f_b0 != 0) {
            // Free-credits action buttons.
            int r = ChoiceWindow_OnTouchEnd(self->f_20, touch);
            if (r == 0) {
                for (int i = 0x12; i != 0x17; i++)
                    TouchButton_setVisible(G<void *>(G<void *>(self->f_24, 4), i * 4), false);
                self->f_b0 = 0;
                HangarWindow_showCreditsBuyWindow(self);
            }
            void *appData = AppManager_GetApplicationData();
            void *rh = *g_hw_recordHandler;
            for (unsigned int i = 0; i != 5; i++) {
                if (TouchButton_OnTouchEnd(
                        G<void *>(G<void *>(self->f_24, 4), i * 4 + 0x48), touch) != 0) {
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

        if (self->f_ac != 0) {
            // Blueprint cargo-purchase confirmation.
            int r = ChoiceWindow_OnTouchEnd(self->f_20, touch);
            int cost = Item_getBlueprintAmount(G<void *>(self->f_84, 0x10)) * 200;
            bool revert = true;
            if (r == 0 && Status_getCredits() >= cost && F<uint8_t>(self, 0x11e) == 0) {
                Status_changeCredits(globals);
                HangarWindow_setSellMode(self);
                self->f_68 = 0;
                HangarList_setCurrentItemIndex(self->f_14, -1);
                F<uint8_t>(self, 0x11e) = 0;
                self->f_ac = 0;
                revert = false;
            } else if (r == 1 && Status_getCredits() >= cost && F<uint8_t>(self, 0x11e) == 0) {
                revert = false;
            }
            if (revert) {
                Item_setStationAmount(G<void *>(self->f_84, 0x10), self->f_8c);
                Item_setAmount(G<void *>(self->f_84, 0x10), self->f_a0);
                Item_setBlueprintAmount(G<void *>(self->f_84, 0x10), self->f_a4);
                Status_setCredits(globals);
                self->f_8c = 0;
                self->f_94 = 0;
                self->f_68 = 0;
                self->f_a0 = 0;
                self->f_a4 = 0;
                self->f_a8 = self->f_9c;
                HangarList_setCurrentItemIndex(self->f_14, -1);
                self->f_3c = 0;
                self->f_ac = 0;
                self->f_88 = 0;
                if (Status_getCredits() < cost && F<uint8_t>(self, 0x11e) == 0) {
                    String12 line, priceStr, fmt, msg;
                    AEString_ctor(&line, (const char *)GameText_getText(*g_hw_notEnoughTextId), false);
                    Layout_formatCredits(&priceStr, Status_getCredits());
                    AEString_ctor(&priceStr, hw_ote_fmt2, false);
                    AEString_ctor(&fmt, hw_ote_fmt4, false);
                    Status_replaceHash(&msg, globals, &line, &priceStr);
                    ChoiceWindow_set(self->f_20, 0);
                    AEString_dtor(&msg);
                    AEString_dtor(&fmt);
                    AEString_dtor(&priceStr);
                    AEString_dtor(&line);
                    self->f_3c = 1;
                }
                F<uint8_t>(self, 0x11e) = 0;
            }
            HangarWindow_refreshCurrentContentHeight(self);
            return;
        }

        if (F<uint8_t>(self, 0x93) != 0) {
            // Ship sale confirmation.
            int r = ChoiceWindow_OnTouchEnd(self->f_20, touch);
            if (r == 1) {
                F<uint8_t>(self, 0x93) = 0;
                self->f_3c = 0;
            } else if (r == 0) {
                F<uint8_t>(self, 0x93) = 0;
                self->f_3c = 0;
                ListItem_getPrice(self->f_68);
                Status_changeCredits(globals);
                Station_removeShip(Status_getStation());
                HangarList_initShopTab(self->f_14, self->f_10,
                                       Station_getShips(Status_getStation()));
                HangarWindow_refreshCurrentContentHeight(self);
            }
            return;
        }

        // Ship purchase confirmation (the large branch). The mission-offer tail
        // is delegated to a helper because its decompile is unrecoverable.
        int r = ChoiceWindow_OnTouchEnd(self->f_20, touch);
        bool special = G<int>(globals, 0x114) == 3 && F<uint8_t>(self, 0x11d) == 0;
        if (special && F<uint8_t>(self, 0x91) != 0 && r == 1) {
            int idx = G<int>(globals, 0x14c);
            Ship_getIndex(Status_getShip());
            if (Station_hasShip((void *)(uintptr_t)idx, Ship_getIndex(Status_getShip())) == 0) {
                int price = Ship_getPrice(G<void *>(self->f_68, 0xc));
                if (Status_getCredits() < price) {
                    String12 line, priceStr, fmt, msg, suffix, combined;
                    AEString_ctor(&line, (const char *)GameText_getText(*g_hw_sellShipTextId), false);
                    Ship_getPrice(G<void *>(self->f_68, 0xc));
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
                    ChoiceWindow_setMsg(self->f_20, &msg, true);
                    self->f_3c = 1;
                    F<uint8_t>(self, 0xaf) = 1;
                    self->f_90 = 0;
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
        int r = ChoiceWindow_OnTouchEnd(self->f_20, touch);
        if (r != 0) {
            // Buy-credits grid.
            void *btns = self->f_24;
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
        F<uint8_t>(self, 0xae) = 0;
        self->f_3c = 0;
        for (int i = 0xc; i != 0x11; i++)
            TouchButton_setVisible(G<void *>(G<void *>(self->f_24, 4), i * 4), false);
        TouchButton_setVisible(G<void *>(G<void *>(self->f_24, 4), 0x44), false);
        void *appData = AppManager_GetApplicationData();
        G<uint8_t>(appData, 0x40) = 0;
        void *mod = AppManager_GetApplicationModule(*g_hw_modStationId);
        if (mod != 0 && G<uint8_t>(mod, 0x18) != 0)
            G<uint8_t>(mod, 0x18) = 0;
        return;
    }

    // --- Shared buy/sell confirmation tail (reached from 0x11c branch). ---
    uint8_t buying = self->f_88;
    int r2 = ChoiceWindow_OnTouchEnd(self->f_20, touch);
    if (buying != 0) {
        if (r2 == 1) {
            self->f_88 = 0;
            self->f_3c = 0;
        } else if (r2 == 0) {
            if (self->f_130 == 0) {
                self->f_3c = 0;
                if (F<uint8_t>(self, 0xad) == 0) {
                    self->f_88 = 1;
                } else {
                    Array<void *> *tabs = (Array<void *> *)self->f_4;
                    for (unsigned int i = 0; i < tabs->length; i++)
                        TouchButton_resetTouch(tabs->data[i]);
                }
                return;
            }
            self->f_3c = 0;
            self->f_130 = 0;
            self->f_88 = 0;
        } else {
            return;
        }
        self->f_68 = 0;
        HangarList_setCurrentItemIndex(self->f_14, -1);
        return;
    }
    if (r2 != 0)
        return;
    self->f_3c = 0;
    void *mod = AppManager_GetApplicationModule(*g_hw_dlcModuleId);
    if (G<uint8_t>(mod, 0x18) != 0)
        G<uint8_t>(mod, 0x18) = 0;
}
