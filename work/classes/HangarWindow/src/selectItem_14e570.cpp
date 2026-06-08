#include "class.h"
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
extern "C" extern const char hw_sel_fmt1[], hw_sel_fmt2[], hw_sel_fmt3[];

// Show a simple text-only choice window with the given game-text id, then flag dirty.
static void showText(HangarWindow *self, int textId)
{
    GameText_getText(textId);
    ChoiceWindow_set(self->f_20, 0);
    self->f_3c = 1;
}

extern "C" void HangarWindow_selectItem(HangarWindow *self, void *item)
{
    self->f_68 = item;
    if (item != 0 && ListItem_isShip(item) != 0)
        Ship_adjustPrice(G<void *>(self->f_68, 0xc));

    int tab = HangarList_getCurrentTab(self->f_14);

    if (tab == 2) {
        if (ListItem_isSelectable(item) != 0 && ListItem_isPendingProduct(item) == 0) {
            void *bp = G<void *>(item, 0x8);
            self->f_80 = bp;
            HangarList_fillIngredientsList(self->f_14, bp != 0);
            HangarList_setCurrentTab(self->f_14, true);
            HangarWindow_refreshCurrentContentHeight(self);
            if (F<uint8_t>(self, 0x89) != 0)
                F<uint8_t>(self, 0x89) = 0;
        }
        return;
    }

    if (tab == 1) {
        if (ListItem_isSelectable(item) == 0)
            return;
        if (ListItem_isShip(item) == 0) {
            // Buying / selling a regular item.
            if (Item_isUnsaleable(G<void *>(item, 0x10)) != 0)
                return;

            uint8_t was = self->f_88;
            self->f_88 = (uint8_t)(was ^ 1);
            if (was == 0) {
                void *flags = *g_hw_itemFlags;
                if (G<uint8_t>(flags, 0x1d) == 0) {
                    GameText_getText(*g_hw_sellMsgTextId1);
                    ChoiceWindow_set(self->f_20, 0);
                    G<uint8_t>(flags, 0x1d) = 1;
                    self->f_3c = 1;
                }
                self->f_8c = Item_getStationAmount(G<void *>(item, 0x10));
                self->f_a0 = Item_getAmount(G<void *>(item, 0x10));
                self->f_98 = Status_getCredits();
                self->f_9c = self->f_a8;
            } else {
                if (ListItem_isItem(item) != 0 && Item_getType(G<void *>(item, 0x10)) != 4) {
                    void *flags = *g_hw_itemFlags;
                    if (G<uint8_t>(flags, 0x1e) == 0) {
                        GameText_getText(*g_hw_sellMsgTextId2);
                        ChoiceWindow_set(self->f_20, 0);
                        G<uint8_t>(flags, 0x1e) = 1;
                        self->f_3c = 1;
                    }
                }
                int li = ListItem_getIndex(item);
                if (li > 0x83 && ListItem_getIndex(item) < 0x9a) {
                    int base = G<int>(*g_hw_globals, 0xac);
                    *((uint8_t *)G<int>((void *)(uintptr_t)base, 4) + ListItem_getIndex(item) - 0x84) = 1;
                }
                self->f_f8 = 1;
                self->f_fc = HangarList_getCurrentItemIndex(self->f_14);

                Ship_setCargo(Status_getShip(), Item_extractItems(self->f_10, true));
                Station_setItems(Status_getStation(),
                                 Item_extractItems(self->f_10, false), false);
                if (self->f_10 != 0) {
                    ArrayReleaseClasses_ItemPtr(self->f_10);
                    if (self->f_10 != 0)
                        operator_delete(Array_ItemPtr_dtor(self->f_10));
                }
                self->f_10 = 0;

                void *mixed = Item_mixItems(Ship_getCargo(Status_getShip()),
                                            Station_getItems(Status_getStation()));
                self->f_10 = mixed;
                HangarList_initShopTab(self->f_14, mixed,
                                       Station_getShips(Status_getStation()));
                HangarList_initShipTab(self->f_14, Status_getShip());
            }
            return;
        }

        // Buying a ship.
        int price = Ship_getPrice(G<void *>(item, 0xc));
        void *globals = *g_hw_globals;
        int credits = Status_getCredits();
        int oldPrice = Ship_getPrice(Status_getShip());
        if (oldPrice + credits < price && F<uint8_t>(self, 0x11d) == 0) {
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
            ChoiceWindow_setMsg(self->f_20, &msg, true);
            self->f_3c = 1;
            F<uint8_t>(self, 0xaf) = 1;
            AEString_dtor(&msg);
            return;
        }

        if (G<int>(globals, 0x34) < 1) {
            if (Status_getCurrentCampaignMission() == 0x4d &&
                Ship_getIndex(Status_getShip()) == 0x25) {
                showText(self, *g_hw_unsaleableTextId);
                return;
            }
            int a = Ship_getIndex(Status_getShip());
            int b = Ship_getIndex(G<void *>(item, 0xc));
            if (a != b) {
                self->f_90 = 1;
                GameText_getText(*g_hw_sellMsgTextId2);
                ChoiceWindow_setMsg(self->f_20,
                                    (String12 *)GameText_getText(*g_hw_sellMsgTextId2), true);
                self->f_3c = 1;
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

    self->f_bc = 0;
    self->f_e4 = self->f_b4;
    self->f_b4 = 0;

    void *flags0 = *g_hw_itemFlags;
    if (G<uint8_t>(flags0, 0x1f) == 0 && ListItem_isSlot(item) != 0) {
        GameText_getText(*g_hw_slotMsgTextId);
        ChoiceWindow_set(self->f_20, 0);
        G<uint8_t>(flags0, 0x1f) = 1;
        self->f_3c = 1;
    }

    if (ListItem_isSelectable(item) == 0)
        return;

    void *cur = HangarList_getCurrentItem(self->f_14);
    void *curItem = G<void *>(cur, 0x10);
    if (curItem != 0) {
        if (Item_isUnsaleable(curItem) != 0) {
            showText(self, *g_hw_unsaleableTextId);
            return;
        }
        if (Item_getSort(curItem) == 0x14) {
            void *globals = *g_hw_globals;
            int passengerCount = G<int>(globals, 0x34);
            int chosen = passengerCount;
            if (passengerCount > 0)
                chosen = G<int>(item, 0x3c);
            int adj = passengerCount - 1;
            if (passengerCount >= 1)
                adj = chosen;
            if (adj >= 0 == (passengerCount >= 1)) {
                int maxPax = Ship_getMaxPassengers(Status_getShip());
                int used = Item_getAttribute(curItem);
                if (maxPax - used < G<int>(globals, 0x34)) {
                    showText(self, *g_hw_unsaleableTextId);
                    return;
                }
            }
        }
    }

    if (G<int>(item, 0x3c) >= 0) {
        void *ci = HangarList_getCurrentItem(self->f_14);
        HangarWindow_demountItem(self, curItem, G<int>(ci, 0x40));
        return;
    }

    // Mounting: check for an existing equipment of the same sort.
    void *globals = *g_hw_globals;
    int sort = Item_getSort(G<void *>(item, 0x10));
    void *existing = Ship_getFirstEquipmentOfSort(Status_getShip(), sort);
    bool conflict = false;
    if (Item_getSort(G<void *>(item, 0x10)) == 0x15) {
        if (Ship_getIndex(Status_getShip()) != 0x2c &&
            existing != 0 && Ship_getIndex(Status_getShip()) != 0x31)
            conflict = true;
    } else if (existing != 0) {
        conflict = true;
    }

    if (conflict && Item_canBeInstalledMultipleTimes(G<void *>(item, 0x10)) == 0) {
        String12 name, copy, etext, fmt, result, etext2, fmt2, result2;
        AEString_ctor(&name, (const char *)GameText_getText(*g_hw_unsaleableTextId), false);
        AEString_ctor_str(&copy, &name, false);
        Item_getIndex(existing);
        AEString_ctor(&etext, (const char *)GameText_getText(*g_hw_unsaleableTextId), false);
        AEString_ctor(&fmt, hw_sel_fmt2, false);
        Status_replaceHash(&result, globals, &copy, &etext, &fmt);
        Item_getIndex(G<void *>(item, 0x10));
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
        ChoiceWindow_setMsg(self->f_20, &name, true);
        self->f_11c = 1;
        self->f_3c = 1;
        self->f_28 = G<void *>(item, 0x10);
        self->f_2c = existing;
        AEString_dtor(&name);
        return;
    }

    HangarWindow_mountItem(self, G<void *>(item, 0x10));
}
