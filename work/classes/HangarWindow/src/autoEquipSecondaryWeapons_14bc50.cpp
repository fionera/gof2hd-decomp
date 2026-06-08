#include "class.h"

struct String12 { uint32_t a, b, c; };

extern "C" {
void *HangarList_getCurrentItemAt(void *list, int idx);
int ListItem_isItem(void *item);
int Item_getType(void *item);
int Item_getAmount(void *item);
int Item_getIndex(void *item);
void *Item_makeItem(void *item);
void Item_setAmount(void *item, int amount);
int *Ship_getEquipment(void *ship, int slot);
void Ship_setEquipment(void *ship, void *item);
void Ship_removeCargo(void *ship, int index, int amount);
void *Status_getShip();
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b, String12 *c);
void *GameText_getText(int id);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_copy(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void HangarList_initShipTab(void *list, void *ship);
void ChoiceWindow_set(void *win, String12 *msg);
void *HangarWindow_statusShip();
}

__attribute__((visibility("hidden"))) extern int *g_hw_equipTextId;
__attribute__((visibility("hidden"))) extern void **g_hw_globals;
extern "C" extern const char hw_equip_fmt[];

extern "C" void HangarWindow_autoEquipSecondaryWeapons(HangarWindow *self, int row)
{
    void *item = HangarList_getCurrentItemAt(F<void *>(self, 0x14), row);
    if (item == 0)
        return;
    void *itm = G<void *>(item, 0x10);
    if (itm == 0 || Item_getType(itm) != 1)
        return;
    if (ListItem_isItem(item) == 0 || Item_getType(itm) != 1)
        return;
    if (Item_getAmount(itm) <= 0)
        return;

    void *ship = Status_getShip();
    int *equip = Ship_getEquipment(ship, 1);
    if (equip == 0)
        return;

    Array<void *> *arr = (Array<void *> *)equip;
    for (unsigned int i = 0; i < arr->length; i++) {
        void *cur = arr->data[i];
        if (cur == 0)
            continue;
        if (Item_getIndex(cur) != Item_getIndex(itm))
            continue;

        void *made = Item_makeItem(itm);

        Array<void *> *cargo = F<Array<void *> *>(self, 0x10);
        if (cargo != 0) {
            for (unsigned int j = 0; j < cargo->length; j++) {
                if (Item_getIndex(cargo->data[j]) == Item_getIndex(made))
                    Item_setAmount(cargo->data[j], 0);
                cargo = F<Array<void *> *>(self, 0x10);
            }
        }

        Ship_setEquipment(HangarWindow_statusShip(), made);
        Ship_removeCargo(HangarWindow_statusShip(), Item_getIndex(made), Item_getAmount(itm));
        HangarList_initShipTab(F<void *>(self, 0x14), HangarWindow_statusShip());

        String12 msg, msgCopy, name, fmt;
        AEString_ctor(&msg, (const char *)GameText_getText(*g_hw_equipTextId), false);
        AEString_ctor_copy(&msgCopy, &msg, false);
        AEString_ctor(&name, (const char *)GameText_getText(*g_hw_equipTextId), false);
        AEString_ctor(&fmt, hw_equip_fmt, false);
        String12 result;
        Status_replaceHash(&result, *g_hw_globals, &msgCopy, &name, &fmt);
        AEString_assign(&msg, &result);
        AEString_dtor(&result);
        AEString_dtor(&fmt);
        AEString_dtor(&name);
        AEString_dtor(&msgCopy);

        ChoiceWindow_set(F<void *>(self, 0x20), &msg);
        F<uint8_t>(self, 0xad) = 1;
        F<uint8_t>(self, 0x3c) = 1;
        AEString_dtor(&msg);
        break;
    }
}
