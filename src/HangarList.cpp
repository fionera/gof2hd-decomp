#include "gof2/HangarList.h"
#include "gof2/GameText.h"
#include "gof2/ListItem.h"




// ---- _HangarList_11d9fe.cpp ----
HangarList::~HangarList() {
    release();
}

// ---- getCurrentLength_11e786.cpp ----
uint32_t HangarList::getCurrentLength() {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    uint32_t current = this->currentTab;
    Array<ListItem *> *items = tabs->data()[current];
    return items != 0 ? items->size() : 0;
}

// ---- getCurrentTabItems_11e772.cpp ----
Array<ListItem *> *HangarList::getCurrentTabItems() {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    uint32_t current = this->currentTab;
    return tabs->data()[current];
}

// ---- HangarList_11d9f4.cpp ----
HangarList::HangarList() {
    this->tabs = 0;
    this->currentTab = 0;
    this->currentItem = 0;
}

// ---- release_11da10.cpp ----
void HangarList::release() {
    Array<Array<ListItem *> *> *tabs =
        this->tabs;
    if (tabs != 0) {
        uint32_t off = 0;
        uint32_t i = 0;
        while (i < tabs->size()) {
            Array<ListItem *> **data = tabs->data();
            Array<ListItem *> *items =
                *(Array<ListItem *> **)((char *)data + off);
            Array<ListItem *> **slot;
            if (items == 0) {
                slot = data + i;
            } else {
                if (items->size() != 0) {
                    ArrayReleaseClasses_ListItem(items);
                    tabs = this->tabs;
                    data = tabs->data();
                    items = *(Array<ListItem *> **)((char *)data + off);
                    if (items == 0) {
                        slot = data + i;
                        goto clear_slot;
                    }
                }
                operator_delete(Array_ListItem_dtor(items));
                tabs = this->tabs;
                slot = (Array<ListItem *> **)((char *)tabs->data() + off);
            }
        clear_slot:
            *slot = 0;
            uint32_t next = i + 1;
            off += 4;
            tabs = this->tabs;
            i = next;
        }
        ArrayReleaseClasses_ArrayListItem(tabs);
        tabs = this->tabs;
        if (tabs != 0) {
            operator_delete(Array_ArrayListItem_dtor(tabs));
        }
    }
    this->tabs = 0;
}

// ---- getCurrentItem_11e744.cpp ----
extern "C" ListItem *HangarList_getCurrentItem_tail(HangarList *self,
                                                    ListItem *item);

ListItem *HangarList::getCurrentItem() {
    return HangarList_getCurrentItem_tail(this, this->currentItem);
}

// ---- getCurrentItemAt_11e74a.cpp ----
ListItem *HangarList::getCurrentItemAt(int index) {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    if (tabs != 0) {
        uint32_t current = ((volatile uint32_t *)this)[1];
        uint32_t dataWord = ((volatile uint32_t *)tabs)[1];
        Array<ListItem *> **data = (Array<ListItem *> **)dataWord;
        uint32_t itemsWord = ((volatile uint32_t *)data)[current];
        Array<ListItem *> *items = (Array<ListItem *> *)itemsWord;
        if (items != 0) {
            ListItem *result = 0;
            if (-1 < index) {
                int length = items->size();
                if (length > index) {
                    result = items->data()[index];
                }
                return result;
            }
            return 0;
        }
    }
    return 0;
}

// ---- initBlueprintTab_11e088.cpp ----
void HangarList::initBlueprintTab(Array<BluePrint *> *blueprints) {
    Array<Array<ListItem *> *> *tabs =
        this->tabs;
    Array<ListItem *> **slot = tabs->data() + 2;
    Array<ListItem *> *old = *slot;
    if (old != 0) {
        if (old->size() != 0) {
            ArrayReleaseClasses_ListItem(old);
            slot = this->tabs->data() + 2;
            old = *slot;
            if (old == 0) {
                goto clear_old;
            }
        }
        operator_delete(Array_ListItem_dtor(old));
        slot = this->tabs->data() + 2;
    }
clear_old:
    *slot = 0;

    Array<ListItem *> *list = (Array<ListItem *> *)operator_new(0xc);
    Array_ListItem_ctor(list);

    uint32_t unlocked = 1;
    for (uint32_t i = 0; i < blueprints->size(); ++i) {
        unlocked += BluePrint_isUnlocked(blueprints->data()[i]);
    }

    Array<PendingProduct *> *pending =
        Status_getPendingProducts(g_HangarList_status);
    uint32_t length = unlocked;
    bool noPending = true;
    if (pending != 0) {
        uint32_t i = 0;
        while (pending->size() != i) {
            if (pending->data()[i] != 0) {
                ++length;
            }
            ++i;
        }
        noPending = length <= unlocked;
        if (unlocked < length) {
            ++length;
        }
    }

    ArraySetLength_ListItem(length, list);

    ListItem *li = (ListItem *)operator_new(0x48);
    GameText **texts = g_HangarList_gameText;
    ((ListItem *)(li))->ctor_String(((GameText *)(*texts))->getText(0x111));
    list->data()[0] = li;

    uint32_t out = 1;
    for (uint32_t i = 0; i < blueprints->size(); ++i) {
        if (BluePrint_isUnlocked(blueprints->data()[i]) != 0) {
            li = (ListItem *)operator_new(0x48);
            ((ListItem *)(li))->ctor_BluePrint(blueprints->data()[i]);
            list->data()[out] = li;
            ++out;
        }
    }

    if (!noPending) {
        li = (ListItem *)operator_new(0x48);
        ((ListItem *)(li))->ctor_String(((GameText *)(*texts))->getText(0x112));
        uint32_t pendingOut = out + 1;
        list->data()[out] = li;
        for (uint32_t i = 0; i < pending->size(); ++i) {
            if (pending->data()[i] != 0) {
                li = (ListItem *)operator_new(0x48);
                ((ListItem *)(li))->ctor_PendingProduct(pending->data()[i]);
                list->data()[pendingOut] = li;
                ++pendingOut;
            }
        }
    }

    this->tabs->data()[2] = list;
}

// ---- fillIngredientsList_11e5a8.cpp ----
void HangarList::fillIngredientsList(BluePrint *blueprint, bool flag) {
    (void)flag;
    Ship *ship = Status_getShip(g_HangarList_status);
    Array<Item *> *cargo = Ship_getCargo(ship);
    Array<Item *> *allItems = g_HangarList_items;
    Array<int32_t> *ingredients = BluePrint_getIngredientList(blueprint);
    uint32_t count = ingredients->size();

    Array<ListItem *> *list = (Array<ListItem *> *)operator_new(0xc);
    Array_ListItem_ctor(list);
    ArraySetLength_ListItem(count + 1, list);

    ListItem *li = (ListItem *)operator_new(0x48);
    GameText **texts = g_HangarList_gameText;
    ((ListItem *)(li))->ctor_String(((GameText *)(*texts))->getText(BluePrint_getIndex(blueprint) + 0x4fa));
    list->data()[0] = li;

    uint32_t out = 1;
    for (uint32_t i = 0; i < ingredients->size(); ++i) {
        Item *shown = 0;
        if (cargo != 0) {
            for (uint32_t j = 0; j < cargo->size(); ++j) {
                if (Item_getIndex(cargo->data()[j]) == ingredients->data()[i]) {
                    li = (ListItem *)operator_new(0x48);
                    ((ListItem *)(li))->ctor_Item(cargo->data()[j]);
                    list->data()[out] = li;
                    shown = cargo->data()[j];
                    goto got_item;
                }
            }
        }
        shown = allItems->data()[ingredients->data()[i]];
        li = (ListItem *)operator_new(0x48);
        ((ListItem *)(li))->ctor_Item(shown);
        list->data()[out] = li;
    got_item:
        Item_setBlueprintAmount(shown, 0);
        ++i;
        ++out;
        --i;
    }

    this->tabs->data()[4] = list;
    li = (ListItem *)operator_new(0x48);
    ListItem_ctor_Slot(li, 0);
    li->field_0x24 = 0;
    ArrayAdd_ListItem(li, this->tabs->data()[4]);
}

// ---- fillBuyList_11e2e0.cpp ----
void HangarList::fillBuyList(ListItem *item) {
    Array<Array<ListItem *> *> *tabs =
        this->tabs;
    Array<ListItem *> **slot = tabs->data() + 3;
    Array<ListItem *> *old = *slot;
    if (old != 0) {
        if (old->size() != 0) {
            ArrayReleaseClasses_ListItem(old);
            slot = this->tabs->data() + 3;
            old = *slot;
            if (old == 0) {
                goto clear_old;
            }
        }
        operator_delete(Array_ListItem_dtor(old));
        slot = this->tabs->data() + 3;
    }
clear_old:
    *slot = 0;

    uint32_t isShip = ((ListItem *)(item))->isShip();
    int type = item->field_0x28;
    int count = 0;
    if (isShip == 0) {
        if (((ListItem *)(item))->isSlot() == 0) {
            type = Item_getType(item->field_0x10);
        }
        Ship *ship = Status_getShip(g_HangarList_status);
        Array<Item *> *cargo = Ship_getCargo(ship);
        if (cargo != 0) {
            for (uint32_t i = 0; i < cargo->size(); ++i) {
                if (Item_getType(cargo->data()[i]) == type) {
                    ++count;
                }
            }
        }
    } else {
        Station *station = Status_getStation(g_HangarList_status);
        Array<Ship *> *ships = Station_getShips(station);
        if (ships != 0) {
            count = ships->size();
        }
    }

    uint32_t isSlot = ((ListItem *)(item))->isSlot();
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
    ((ListItem *)(li))->ctor_String(((GameText *)(*texts))->getText(0x115));
    list->data()[0] = li;

    li = (ListItem *)operator_new(0x48);
    ListItem_ctor_ListItem(li, item);
    list->data()[1] = li;

    uint32_t out;
    if (special == 0) {
        li = (ListItem *)operator_new(0x48);
        ((ListItem *)(li))->ctor_String(((GameText *)(*texts))->getText(0x116));
        list->data()[2] = li;

        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_TextButton(li, ((GameText *)(*texts))->getText(0x117), true, 0);
        list->data()[3] = li;

        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_TextButton(li, ((GameText *)(*texts))->getText(0x11a), true, 1);
        list->data()[4] = li;
        out = 5;
    } else {
        out = 2;
    }

    li = (ListItem *)operator_new(0x48);
    ((ListItem *)(li))->ctor_String(((GameText *)(*texts))->getText(isShip == 0 ? 0x11c : 0x11d));
    uint32_t next = out + 1;
    list->data()[out] = li;

    if (count < 1) {
        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_Slot(li, next);
        list->data()[next] = li;
    } else if (isShip == 0) {
        Ship *ship = Status_getShip(g_HangarList_status);
        Array<Item *> *cargo = Ship_getCargo(ship);
        for (uint32_t i = 0; i < cargo->size(); ++i) {
            if (Item_getType(cargo->data()[i]) == type) {
                li = (ListItem *)operator_new(0x48);
                ((ListItem *)(li))->ctor_Item(cargo->data()[i]);
                list->data()[next] = li;
                ++next;
            }
        }
    } else {
        Station *station = Status_getStation(g_HangarList_status);
        Array<Ship *> *ships = Station_getShips(station);
        for (uint32_t i = 0; i < ships->size(); ++i) {
            li = (ListItem *)operator_new(0x48);
            ((ListItem *)(li))->ctor_Ship(ships->data()[i]);
            list->data()[out + 1 + i] = li;
        }
    }

    this->tabs->data()[3] = list;
}

// ---- initShipTab_11db48.cpp ----
void HangarList::initShipTab(Ship *ship) {
    Array<Array<ListItem *> *> *tabs =
        this->tabs;
    Array<ListItem *> **slot = tabs->data();
    Array<ListItem *> *old = *slot;
    if (old != 0) {
        if (old->size() != 0) {
            ArrayReleaseClasses_ListItem(old);
            slot = this->tabs->data();
            old = *slot;
            if (old == 0) {
                goto clear_old;
            }
        }
        operator_delete(Array_ListItem_dtor(old));
        slot = this->tabs->data();
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
        for (uint32_t i = 0; i < cargo->size(); ++i) {
            Item *item = cargo->data()[i];
            int type = Item_getType(item);
            if (type != 4) {
                type = Item_getType(cargo->data()[i]);
                if (Ship_getSlots(ship, type) > 0) {
                    ArrayAdd_Item(cargo->data()[i], available);
                }
            }
        }
        cargoEmpty = false;
        baseLength = available->size() + 2;
    }

    uint32_t equipmentLength = equipment->size();
    int slotTypes = Ship_getSlotTypes(ship);
    ArraySetLength_ListItem(slotTypes + baseLength + equipmentLength, items);

    ListItem *li = (ListItem *)operator_new(0x48);
    GameText **texts = g_HangarList_gameText;
    ((ListItem *)(li))->ctor_String(((GameText *)(*texts))->getText(0xb7));
    items->data()[0] = li;

    li = (ListItem *)operator_new(0x48);
    ((ListItem *)(li))->ctor_Ship(ship);
    uint32_t out = 2;
    items->data()[1] = li;

    for (uint32_t type = 0; type < 4; ++type) {
        if (Ship_getSlots(ship, type) > 0) {
            int textId = 0x10d;
            if (type < 3) {
                textId = type + 0x109;
            }
            li = (ListItem *)operator_new(0x48);
            ((ListItem *)(li))->ctor_String_int(((GameText *)(*texts))->getText(textId), type);
            items->data()[out] = li;
            Array<Item *> *slotItems = Ship_getEquipmentByType(ship, type);
            for (uint32_t j = 0; j < slotItems->size(); ++j) {
                ++out;
                Item *item = slotItems->data()[j];
                li = (ListItem *)operator_new(0x48);
                if (item == 0) {
                    ListItem_ctor_Slot(li, type);
                } else {
                    ((ListItem *)(li))->ctor_Item(slotItems->data()[j]);
                }
                items->data()[out] = li;
                uint32_t k = 0;
                for (; k < equipment->size(); ++k) {
                    ListItem *entry = items->data()[out];
                    if (equipment->data()[k] == entry->field_0x10) {
                        entry->field_0x40 = k;
                        break;
                    }
                }
                (items->data()[out])->field_0x3c = j;
            }
            operator_delete(Array_Item_dtor(slotItems));
            if (!cargoEmpty) {
                for (uint32_t j = 0; j < available->size(); ++j) {
                    if ((uint32_t)Item_getType(available->data()[j]) == type) {
                        li = (ListItem *)operator_new(0x48);
                        ((ListItem *)(li))->ctor_Item(available->data()[j]);
                        items->data()[out] = li;
                        ++out;
                    }
                }
            }
        }
    }
    this->tabs->data()[0] = items;
}

// ---- initShopTab_11de38.cpp ----
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
        this->tabs;
    Array<ListItem *> **slot = tabs->data() + 1;
    Array<ListItem *> *old = *slot;
    if (old != 0) {
        if (old->size() != 0) {
            ArrayReleaseClasses_ListItem(old);
            slot = this->tabs->data() + 1;
            old = *slot;
            if (old == 0) {
                goto clear_old;
            }
        }
        operator_delete(Array_ListItem_dtor(old));
        slot = this->tabs->data() + 1;
    }
clear_old:
    *slot = 0;

    if ((shopItems == 0 || shopItems->size() == 0) &&
        (ships == 0 || ships->size() == 0)) {
        return;
    }

    Array<ListItem *> *list = (Array<ListItem *> *)operator_new(0xc);
    Array_ListItem_ctor(list);
    if (shopItems != 0) {
        for (uint32_t i = 0; i < shopItems->size(); ++i) {
            int type = Item_getType(shopItems->data()[i]);
            counts[type] = counts[type] + 1;
        }
    }

    int length = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        if (counts[i] > 0) {
            ++length;
        }
    }
    if (ships != 0 && ships->size() != 0) {
        length += ships->size() + 1;
    }
    length += shopItems == 0 ? 0 : shopItems->size();
    ArraySetLength_ListItem(length, list);

    uint32_t out;
    GameText **texts = g_HangarList_gameText;
    if (ships != 0 && ships->size() != 0) {
        ListItem *li = (ListItem *)operator_new(0x48);
        ((ListItem *)(li))->ctor_String_int(((GameText *)(*texts))->getText(0xad), -1);
        list->data()[0] = li;
        for (uint32_t i = 0; i < ships->size(); ++i) {
            Ship_adjustPrice(ships->data()[i]);
            li = (ListItem *)operator_new(0x48);
            ((ListItem *)(li))->ctor_Ship(ships->data()[i]);
            list->data()[i + 1] = li;
        }
        out = ships->size() + 1;
    } else {
        out = 0;
    }

    for (uint32_t type = 0; type < 5; ++type) {
        if (counts[type] > 0) {
            ListItem *li = (ListItem *)operator_new(0x48);
            ((ListItem *)(li))->ctor_String_int(((GameText *)(*texts))->getText(shopTypeTextId(type)), type);
            list->data()[out] = li;
            ++out;
            if (shopItems != 0) {
                for (uint32_t i = 0; i < shopItems->size(); ++i) {
                    if ((uint32_t)Item_getType(shopItems->data()[i]) == type) {
                        li = (ListItem *)operator_new(0x48);
                        ((ListItem *)(li))->ctor_Item(shopItems->data()[i]);
                        list->data()[out] = li;
                        ++out;
                    }
                }
            }
        }
    }
    this->tabs->data()[1] = list;
}

// ---- init_11e234.cpp ----
extern "C" int HangarList_initBlueprintTab_tail(HangarList *self,
                                                Array<BluePrint *> *blueprints);

int HangarList::init(Ship *ship, Array<Item *> *items, Array<Ship *> *ships,
                     Array<BluePrint *> *blueprints) {
    release();
    void *storage = operator_new(0xc);
    Array<BluePrint *> *bp = blueprints;
    Array<Array<ListItem *> *> *tabs = (Array<Array<ListItem *> *> *)storage;
    Array_ArrayListItem_ctor(tabs);
    this->tabs = tabs;
    ArraySetLength_ArrayListItem(5, tabs);
    initShipTab(ship);
    initShopTab(items, ships);
    return HangarList_initBlueprintTab_tail(this, bp);
}
