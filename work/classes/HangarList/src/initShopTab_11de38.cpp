#include "class.h"

static int shopTypeTextId(uint32_t type) {
    int textId = 0x10e;
    if (type == 3) {
        textId = 0x10d;
    }
    if (type < 3) {
        textId = type + 0x109;
    }
    return textId;
}

void HangarList::initShopTab(Array<Item *> *shopItems, Array<Ship *> *ships) {
    int counts[5] = {};

    Array<Array<ListItem *> *> *tabs =
        F<Array<Array<ListItem *> *> *>(this, 0x0);
    Array<ListItem *> **slot = tabs->data + 1;
    Array<ListItem *> *old = *slot;
    if (old != 0) {
        if (old->length != 0) {
            ArrayReleaseClasses_ListItem(old);
            slot = F<Array<Array<ListItem *> *> *>(this, 0x0)->data + 1;
            old = *slot;
            if (old == 0) {
                goto clear_old;
            }
        }
        operator_delete(Array_ListItem_dtor(old));
        slot = F<Array<Array<ListItem *> *> *>(this, 0x0)->data + 1;
    }
clear_old:
    *slot = 0;

    if ((shopItems == 0 || shopItems->length == 0) &&
        (ships == 0 || ships->length == 0)) {
        return;
    }

    Array<ListItem *> *list = (Array<ListItem *> *)operator_new(0xc);
    Array_ListItem_ctor(list);
    if (shopItems != 0) {
        for (uint32_t i = 0; i < shopItems->length; ++i) {
            int type = Item_getType(shopItems->data[i]);
            counts[type] = counts[type] + 1;
        }
    }

    int length = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        if (counts[i] > 0) {
            ++length;
        }
    }
    if (ships != 0 && ships->length != 0) {
        length += ships->length + 1;
    }
    length += shopItems == 0 ? 0 : shopItems->length;
    ArraySetLength_ListItem(length, list);

    uint32_t out;
    GameText **texts = g_HangarList_gameText;
    if (ships != 0 && ships->length != 0) {
        ListItem *li = (ListItem *)operator_new(0x48);
        ListItem_ctor_String_int(li, GameText_getText(*texts, 0xad), -1);
        list->data[0] = li;
        for (uint32_t i = 0; i < ships->length; ++i) {
            Ship_adjustPrice(ships->data[i]);
            li = (ListItem *)operator_new(0x48);
            ListItem_ctor_Ship(li, ships->data[i]);
            list->data[i + 1] = li;
        }
        out = ships->length + 1;
    } else {
        out = 0;
    }

    for (uint32_t type = 0; type < 5; ++type) {
        if (counts[type] > 0) {
            ListItem *li = (ListItem *)operator_new(0x48);
            ListItem_ctor_String_int(li, GameText_getText(*texts, shopTypeTextId(type)),
                                     type);
            list->data[out] = li;
            ++out;
            if (shopItems != 0) {
                for (uint32_t i = 0; i < shopItems->length; ++i) {
                    if ((uint32_t)Item_getType(shopItems->data[i]) == type) {
                        li = (ListItem *)operator_new(0x48);
                        ListItem_ctor_Item(li, shopItems->data[i]);
                        list->data[out] = li;
                        ++out;
                    }
                }
            }
        }
    }
    F<Array<Array<ListItem *> *> *>(this, 0x0)->data[1] = list;
}
