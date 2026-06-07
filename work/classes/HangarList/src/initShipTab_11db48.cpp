#include "class.h"

void HangarList::initShipTab(Ship *ship) {
    Array<Array<ListItem *> *> *tabs =
        F<Array<Array<ListItem *> *> *>(this, 0x0);
    Array<ListItem *> **slot = tabs->data;
    Array<ListItem *> *old = *slot;
    if (old != 0) {
        if (old->length != 0) {
            ArrayReleaseClasses_ListItem(old);
            slot = F<Array<Array<ListItem *> *> *>(this, 0x0)->data;
            old = *slot;
            if (old == 0) {
                goto clear_old;
            }
        }
        operator_delete(Array_ListItem_dtor(old));
        slot = F<Array<Array<ListItem *> *> *>(this, 0x0)->data;
    }
clear_old:
    *slot = 0;

    Array<ListItem *> *items = (Array<ListItem *> *)operator_new(0xc);
    Array_ListItem_ctor(items);
    Array<Item *> *equipment = Ship_getEquipment(ship);
    Array<Item *> *cargo = Ship_getCargo(ship);
    Array<Item *> *available;
    int baseLength;
    bool cargoEmpty;
    if (cargo == 0) {
        baseLength = 2;
        available = 0;
        cargoEmpty = true;
    } else {
        available = (Array<Item *> *)operator_new(0xc);
        Array_Item_ctor(available);
        for (uint32_t i = 0; i < cargo->length; ++i) {
            Item *item = cargo->data[i];
            int type = Item_getType(item);
            if (type != 4) {
                type = Item_getType(cargo->data[i]);
                if (Ship_getSlots(ship, type) > 0) {
                    ArrayAdd_Item(cargo->data[i], available);
                }
            }
        }
        cargoEmpty = false;
        baseLength = available->length + 2;
    }

    uint32_t equipmentLength = equipment->length;
    int slotTypes = Ship_getSlotTypes(ship);
    ArraySetLength_ListItem(slotTypes + baseLength + equipmentLength, items);

    ListItem *li = (ListItem *)operator_new(0x48);
    GameText **texts = g_HangarList_gameText;
    ListItem_ctor_String(li, GameText_getText(*texts, 0xb7));
    items->data[0] = li;

    li = (ListItem *)operator_new(0x48);
    ListItem_ctor_Ship(li, ship);
    uint32_t out = 2;
    items->data[1] = li;

    for (uint32_t type = 0; type < 4; ++type) {
        if (Ship_getSlots(ship, type) > 0) {
            int textId = 0x10d;
            if (type < 3) {
                textId = type + 0x109;
            }
            li = (ListItem *)operator_new(0x48);
            ListItem_ctor_String_int(li, GameText_getText(*texts, textId), type);
            items->data[out] = li;
            Array<Item *> *slotItems = Ship_getEquipmentByType(ship, type);
            for (uint32_t j = 0; j < slotItems->length; ++j) {
                ++out;
                Item *item = slotItems->data[j];
                li = (ListItem *)operator_new(0x48);
                if (item == 0) {
                    ListItem_ctor_Slot(li, type);
                } else {
                    ListItem_ctor_Item(li, slotItems->data[j]);
                }
                items->data[out] = li;
                uint32_t k = 0;
                for (; k < equipment->length; ++k) {
                    ListItem *entry = items->data[out];
                    if (equipment->data[k] == G<Item *>(entry, 0x10)) {
                        G<uint32_t>(entry, 0x40) = k;
                        break;
                    }
                }
                G<uint32_t>(items->data[out], 0x3c) = j;
            }
            operator_delete(Array_Item_dtor(slotItems));
            if (!cargoEmpty) {
                for (uint32_t j = 0; j < available->length; ++j) {
                    if ((uint32_t)Item_getType(available->data[j]) == type) {
                        li = (ListItem *)operator_new(0x48);
                        ListItem_ctor_Item(li, available->data[j]);
                        items->data[out] = li;
                        ++out;
                    }
                }
            }
        }
    }
    F<Array<Array<ListItem *> *> *>(this, 0x0)->data[0] = items;
}
