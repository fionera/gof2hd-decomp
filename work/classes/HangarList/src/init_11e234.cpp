#include "class.h"

extern "C" int HangarList_initBlueprintTab_tail(HangarList *self,
                                                Array<BluePrint *> *blueprints);

int HangarList::init(Ship *ship, Array<Item *> *items, Array<Ship *> *ships,
                     Array<BluePrint *> *blueprints) {
    release();
    void *storage = operator_new(0xc);
    Array<BluePrint *> *bp = blueprints;
    Array<Array<ListItem *> *> *tabs = (Array<Array<ListItem *> *> *)storage;
    Array_ArrayListItem_ctor(tabs);
    F<Array<Array<ListItem *> *> *>(this, 0x0) = tabs;
    ArraySetLength_ArrayListItem(5, tabs);
    initShipTab(ship);
    initShopTab(items, ships);
    return HangarList_initBlueprintTab_tail(this, bp);
}
