#include "class.h"

void HangarList::fillBuyList(ListItem *item) {
    Array<Array<ListItem *> *> *tabs =
        F<Array<Array<ListItem *> *> *>(this, 0x0);
    Array<ListItem *> **slot = tabs->data + 3;
    Array<ListItem *> *old = *slot;
    if (old != 0) {
        if (old->length != 0) {
            ArrayReleaseClasses_ListItem(old);
            slot = F<Array<Array<ListItem *> *> *>(this, 0x0)->data + 3;
            old = *slot;
            if (old == 0) {
                goto clear_old;
            }
        }
        operator_delete(Array_ListItem_dtor(old));
        slot = F<Array<Array<ListItem *> *> *>(this, 0x0)->data + 3;
    }
clear_old:
    *slot = 0;

    uint32_t isShip = ListItem_isShip(item);
    int type = G<int>(item, 0x28);
    int count = 0;
    if (isShip == 0) {
        if (ListItem_isSlot(item) == 0) {
            type = Item_getType(G<Item *>(item, 0x10));
        }
        Ship *ship = Status_getShip(g_HangarList_status);
        Array<Item *> *cargo = Ship_getCargo(ship);
        if (cargo != 0) {
            for (uint32_t i = 0; i < cargo->length; ++i) {
                if (Item_getType(cargo->data[i]) == type) {
                    ++count;
                }
            }
        }
    } else {
        Station *station = Status_getStation(g_HangarList_status);
        Array<Ship *> *ships = Station_getShips(station);
        if (ships != 0) {
            count = ships->length;
        }
    }

    uint32_t isSlot = ListItem_isSlot(item);
    Array<ListItem *> *list = (Array<ListItem *> *)operator_new(0xc);
    Array_ListItem_ctor(list);
    int base = 3;
    uint32_t special = isShip | isSlot;
    if (count == 0) {
        base = 4;
    }
    uint32_t length = base + count + (special ^ 1);
    if (special == 0) {
        length += 2;
    }
    ArraySetLength_ListItem(length, list);

    GameText **texts = g_HangarList_gameText;
    ListItem *li = (ListItem *)operator_new(0x48);
    ListItem_ctor_String(li, GameText_getText(*texts, 0x115));
    list->data[0] = li;

    li = (ListItem *)operator_new(0x48);
    ListItem_ctor_ListItem(li, item);
    list->data[1] = li;

    uint32_t out;
    if (special == 0) {
        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_String(li, GameText_getText(*texts, 0x116));
        list->data[2] = li;

        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_TextButton(li, GameText_getText(*texts, 0x117), true, 0);
        list->data[3] = li;

        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_TextButton(li, GameText_getText(*texts, 0x11a), true, 1);
        list->data[4] = li;
        out = 5;
    } else {
        out = 2;
    }

    li = (ListItem *)operator_new(0x48);
    ListItem_ctor_String(li, GameText_getText(*texts, isShip == 0 ? 0x11c : 0x11d));
    uint32_t next = out + 1;
    list->data[out] = li;

    if (count < 1) {
        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_Slot(li, next);
        list->data[next] = li;
    } else if (isShip == 0) {
        Ship *ship = Status_getShip(g_HangarList_status);
        Array<Item *> *cargo = Ship_getCargo(ship);
        for (uint32_t i = 0; i < cargo->length; ++i) {
            if (Item_getType(cargo->data[i]) == type) {
                li = (ListItem *)operator_new(0x48);
                ListItem_ctor_Item(li, cargo->data[i]);
                list->data[next] = li;
                ++next;
            }
        }
    } else {
        Station *station = Status_getStation(g_HangarList_status);
        Array<Ship *> *ships = Station_getShips(station);
        for (uint32_t i = 0; i < ships->length; ++i) {
            li = (ListItem *)operator_new(0x48);
            ListItem_ctor_Ship(li, ships->data[i]);
            list->data[out + 1 + i] = li;
        }
    }

    F<Array<Array<ListItem *> *> *>(this, 0x0)->data[3] = list;
}
