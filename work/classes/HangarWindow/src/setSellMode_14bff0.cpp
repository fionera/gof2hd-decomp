#include "class.h"
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
extern "C" extern const char hw_bp_fmt1[], hw_bp_fmt2[], hw_bp_fmt3[];

extern "C" void HangarWindow_setSellMode(HangarWindow *self)
{
    void *item = F<void *>(self, 0x68);

    if (item == 0 ||
        ListItem_isShip(item) != 0 || ListItem_isSlot(item) != 0 ||
        ListItem_isTextButton(item) != 0 || ListItem_isSelectable(item) == 0 ||
        ListItem_isBluePrint(item) != 0) {
        F<uint8_t>(self, 0x88) = 0;
        return;
    }

    F<uint8_t>(self, 0x88) = 1;

    int tab = HangarList_getCurrentTab(F<void *>(self, 0x14));
    if (tab == 1) {
        if (F<uint8_t>(self, 0x88) == 0) {
            if (ListItem_isItem(item) != 0 && Item_getType(G<void *>(item, 0x10)) != 4) {
                void *flags = *g_hw_itemFlags;
                if (G<uint8_t>(flags, 0x1e) == 0) {
                    GameText_getText(*g_hw_sellTextId1);
                    ChoiceWindow_set(F<void *>(self, 0x20), 0);
                    G<uint8_t>(flags, 0x1e) = 1;
                    F<uint8_t>(self, 0x3c) = 1;
                }
            }
            F<uint8_t>(self, 0xf8) = 1;
            F<unsigned int>(self, 0xfc) = HangarList_getCurrentItemIndex(F<void *>(self, 0x14));
        } else {
            void *flags = *g_hw_itemFlags;
            if (G<uint8_t>(flags, 0x1d) == 0) {
                GameText_getText(*g_hw_sellTextId2);
                ChoiceWindow_set(F<void *>(self, 0x20), 0);
                G<uint8_t>(flags, 0x1d) = 1;
                F<uint8_t>(self, 0x3c) = 1;
            }
            F<int>(self, 0x8c) = Item_getStationAmount(G<void *>(item, 0x10));
            F<int>(self, 0xa0) = Item_getAmount(G<void *>(item, 0x10));
            F<int>(self, 0x98) = Status_getCredits();
            F<int>(self, 0x9c) = F<int>(self, 0xa8);
        }

        Ship_setCargo(Status_getShip(), Item_extractItems(F<void *>(self, 0x10), true));
        Station_setItems(Status_getStation(), Item_extractItems(F<void *>(self, 0x10), false), false);
        if (F<void *>(self, 0x10) != 0) {
            ArrayReleaseClasses_ItemPtr(F<void *>(self, 0x10));
            if (F<void *>(self, 0x10) != 0)
                operator_delete(Array_ItemPtr_dtor(F<void *>(self, 0x10)));
        }
        F<void *>(self, 0x10) = 0;

        void *mixed = Item_mixItems(Ship_getCargo(Status_getShip()), Station_getItems(Status_getStation()));
        F<void *>(self, 0x10) = mixed;
        HangarList_initShopTab(F<void *>(self, 0x14), mixed, Station_getShips(Status_getStation()));
        HangarList_initShipTab(F<void *>(self, 0x14), Status_getShip());

        void *ci = HangarList_getCurrentItem(F<void *>(self, 0x14));
        F<void *>(self, 0x68) = ci;
        if (ci != 0 && ListItem_isShip(ci) != 0)
            Ship_adjustPrice(G<void *>(F<void *>(self, 0x68), 0xc));
        HangarWindow_refreshCurrentContentHeight(self);
        return;
    }

    if (HangarList_getCurrentTab(F<void *>(self, 0x14)) != 4)
        return;

    if (F<uint8_t>(self, 0x88) == 0) {
        if (F<void *>(self, 0x84) != 0 && F<void *>(self, 0x80) != 0) {
            void *bpItem = G<void *>(F<void *>(self, 0x84), 0x10);
            BluePrint_addItem(F<void *>(self, 0x80), bpItem, Item_getBlueprintAmount(bpItem),
                              Station_getIndex(Status_getStation()));
        }

        uint8_t completedFlag = 0;
        if (BluePrint_isCompleted(F<void *>(self, 0x80)) != 0) {
            void *globals = *g_hw_globals;
            if (BluePrint_getStationIndex(F<void *>(self, 0x80)) == Station_getIndex(Status_getStation())) {
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
                ChoiceWindow_set(F<void *>(self, 0x20), 0);

                int *stations = g_hw_bpStations;
                int idx = stations[BluePrint_getIndex(F<void *>(self, 0x80))];
                BluePrint_getQuantity(F<void *>(self, 0x80));
                void *made = Item_makeItem((void *)(uintptr_t)idx);
                Ship_addCargo(Status_getShip(), made);
                ArrayAdd_ItemPtr(made, F<void *>(self, 0x10));
                HangarList_setCurrentTab(F<void *>(self, 0x14), true);
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
                BluePrint_getStationName(F<void *>(self, 0x80));
                AEString_ctor(&sname, hw_bp_fmt3, false);
                AEString_ctor(&fmt2, hw_bp_fmt3, false);
                String12 result2;
                Status_replaceHash(&result2, globals, &line2, &sname, &fmt2);
                AEString_assign(&line, &result2);
                AEString_dtor(&result2);
                AEString_dtor(&fmt2);
                AEString_dtor(&sname);
                AEString_dtor(&line2);

                ChoiceWindow_set(F<void *>(self, 0x20), 0);
                Status_addPendingProduct(globals, F<void *>(self, 0x80));
                HangarList_setCurrentTab(F<void *>(self, 0x14), true);
                HangarWindow_refreshCargoAvailabilityForBlueprints(self);
                AEString_dtor(&line);
            }
            BluePrint_reset(F<void *>(self, 0x80));
            completedFlag = 1;
        }

        void *globals = *g_hw_globals;
        Ship_setCargo(HangarWindow_statusShip(), Item_extractItems(Ship_getCargo(0), true));
        HangarWindow_statusShip();
        void *items = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
        HangarList_initShopTab(F<void *>(self, 0x14), items, Station_getShips(Status_getStation()));
        HangarList_initBlueprintTab(F<void *>(self, 0x14), Status_getBluePrints(globals));
        void *mix = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
        F<void *>(self, 0x10) = mix;
        F<uint8_t>(self, 0x3c) = completedFlag;

        if (completedFlag) {
            Array<void *> *items2 = (Array<void *> *)HangarList_getCurrentTabItems(F<void *>(self, 0x14));
            for (unsigned int i = 0; i < items2->length; i++) {
                void *li = items2->data[i];
                if (li != 0 && ListItem_isItem(li) != 0 &&
                    Item_getIndex(G<void *>(li, 0x10)) == BluePrint_getIndex(F<void *>(self, 0x80))) {
                    if (Ship_hasEquipment(Status_getShip(), Item_getIndex(G<void *>(li, 0x10))) != 0) {
                        F<unsigned int>(self, 0xfc) = i;
                        F<uint8_t>(self, 0xf8) = 1;
                        HangarWindow_autoEquipSecondaryWeapons(self, i);
                        F<uint8_t>(self, 0xf8) = 0;
                        break;
                    }
                }
            }
        }
        return;
    }

    F<int>(self, 0x94) = 0;
    if (BluePrint_isEmpty(F<void *>(self, 0x80)) != 0 && Item_getAmount(G<void *>(item, 0x10)) > 0) {
        int idx = BluePrint_getIndex(F<void *>(self, 0x80));
        bool flag;
        void *text;
        if (idx == 0xd2 || BluePrint_getIndex(F<void *>(self, 0x80)) == 0xdf) {
            if (SolarSystem_getRoutes(Status_getSystem()) != 0) {
                text = GameText_getText(*g_hw_sellTextId2);
                flag = true;
            } else {
                F<uint8_t>(self, 0x130) = 1;
                text = GameText_getText(*g_hw_routesTextId);
                flag = false;
            }
        } else {
            text = GameText_getText(*g_hw_sellTextId2);
            flag = true;
        }
        ChoiceWindow_setMsg(F<void *>(self, 0x20), text, flag);
        F<uint8_t>(self, 0x3c) = 1;
    }

    F<int>(self, 0xa4) = Item_getBlueprintAmount(G<void *>(item, 0x10));
    F<int>(self, 0xa0) = Item_getAmount(G<void *>(item, 0x10));
    F<int>(self, 0x98) = Status_getCredits();
    F<int>(self, 0x9c) = F<int>(self, 0xa8);
    F<void *>(self, 0x84) = F<void *>(self, 0x68);
}
