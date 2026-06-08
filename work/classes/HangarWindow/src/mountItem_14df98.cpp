#include "class.h"

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
                        ArrayRemove_ItemPtr(cur, self->f_10);
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
    Ship_setCargo(ship2, Item_extractItems(self->f_10, true));
    HangarList_initShipTab(self->f_14, HangarWindow_statusShip());

    HangarWindow_statusShip();
    void *items = Item_mixItems(Ship_getCargo(0), Station_getItems(Status_getStation()));
    HangarList_initShopTab(self->f_14, items, Station_getShips(Status_getStation()));
    HangarList_setCurrentTab(self->f_14, false);

    float h = HangarWindow_refreshCurrentContentHeight(self);
    self->f_b4 = self->f_e4;
    FModSound_play(*g_hw_sound, 0x62, 0, 0, h);
}
