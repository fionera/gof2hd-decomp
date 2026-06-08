#include "class.h"

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
        ArrayAdd_ItemPtr(made, self->f_10);

    Ship_setCargo(Status_getShip(), Item_extractItems(self->f_10, true));

    if (self->f_10 != 0) {
        ArrayReleaseClasses_ItemPtr(self->f_10);
        if (self->f_10 != 0)
            operator_delete(Array_ItemPtr_dtor(self->f_10));
    }
    self->f_10 = 0;

    HangarWindow_statusShip();
    void *mixed = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
    self->f_10 = mixed;
    HangarList_initShipTab(self->f_14, HangarWindow_statusShip());

    HangarWindow_statusShip();
    void *items = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
    HangarList_initShopTab(self->f_14, items, Station_getShips(Status_getStation()));
    HangarList_setCurrentTab(self->f_14, false);

    float h = HangarWindow_refreshCurrentContentHeight(self);
    self->f_b4 = self->f_e4;
    FModSound_play(*g_hw_sound, 0x60, 0, 0, h);
}
