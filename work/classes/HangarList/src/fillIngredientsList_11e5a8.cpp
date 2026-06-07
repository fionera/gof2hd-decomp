#include "class.h"

void HangarList::fillIngredientsList(BluePrint *blueprint, bool flag) {
    (void)flag;
    Ship *ship = Status_getShip(g_HangarList_status);
    Array<Item *> *cargo = Ship_getCargo(ship);
    Array<Item *> *allItems = g_HangarList_items;
    Array<int32_t> *ingredients = BluePrint_getIngredientList(blueprint);
    uint32_t count = ingredients->length;

    Array<ListItem *> *list = (Array<ListItem *> *)operator_new(0xc);
    Array_ListItem_ctor(list);
    ArraySetLength_ListItem(count + 1, list);

    ListItem *li = (ListItem *)operator_new(0x48);
    GameText **texts = g_HangarList_gameText;
    ListItem_ctor_String(li,
                         GameText_getText(*texts,
                                          BluePrint_getIndex(blueprint) + 0x4fa));
    list->data[0] = li;

    uint32_t out = 1;
    for (uint32_t i = 0; i < ingredients->length; ++i) {
        Item *shown = 0;
        if (cargo != 0) {
            for (uint32_t j = 0; j < cargo->length; ++j) {
                if (Item_getIndex(cargo->data[j]) == ingredients->data[i]) {
                    li = (ListItem *)operator_new(0x48);
                    ListItem_ctor_Item(li, cargo->data[j]);
                    list->data[out] = li;
                    shown = cargo->data[j];
                    goto got_item;
                }
            }
        }
        shown = allItems->data[ingredients->data[i]];
        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_Item(li, shown);
        list->data[out] = li;
    got_item:
        Item_setBlueprintAmount(shown, 0);
        ++i;
        ++out;
        --i;
    }

    F<Array<Array<ListItem *> *> *>(this, 0x0)->data[4] = list;
    li = (ListItem *)operator_new(0x48);
    ListItem_ctor_Slot(li, 0);
    G<uint8_t>(li, 0x24) = 0;
    ArrayAdd_ListItem(li, F<Array<Array<ListItem *> *> *>(this, 0x0)->data[4]);
}
