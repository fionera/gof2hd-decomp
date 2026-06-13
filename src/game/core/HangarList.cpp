#include "gof2/game/core/HangarList.h"
#include "gof2/game/mission/BluePrint.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/game/ui/ListItem.h"
#include "gof2/game/ship/Ship.h"

HangarList::~HangarList() {
    release();
}

// Selects which tab is active. The hangar/shop UI flips between the normal item
// list (tab 0/2) and the blueprint-ingredients sub-list using this boolean.
void HangarList::setCurrentTab(bool blueprintIngredients) {
    this->currentTab = blueprintIngredients ? 1u : 0u;
}

// The +0x8 slot stores the currently selected row index within the active tab;
// getCurrentItem() later resolves it to a ListItem* via getCurrentItemAt().
void HangarList::setCurrentItemIndex(int index) {
    *reinterpret_cast<int *>(&this->currentItem) = index;
}

uint32_t HangarList::getCurrentItemIndex() {
    return *reinterpret_cast<uint32_t *>(&this->currentItem);
}

uint32_t HangarList::getCurrentTab() {
    return this->currentTab;
}

Array<Array<ListItem *> *> *HangarList::getItems() {
    return this->tabs;
}

uint32_t HangarList::getCurrentLength() {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    uint32_t current = this->currentTab;
    Array<ListItem *> *items = (*tabs)[current];
    return items != 0 ? items->size() : 0;
}

Array<ListItem *> *HangarList::getCurrentTabItems() {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    uint32_t current = this->currentTab;
    return (*tabs)[current];
}

HangarList::HangarList() {
    this->tabs = 0;
    this->currentTab = 0;
    this->currentItem = 0;
}

void HangarList::release() {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    if (tabs != 0) {
        for (uint32_t i = 0; i < tabs->size(); ++i) {
            Array<ListItem *> *items = (*tabs)[i];
            if (items != 0) {
                for (ListItem *e : *items) delete e;
                items->clear();
                delete items;
            }
            (*tabs)[i] = 0;
        }
        for (Array<ListItem *> *e : *tabs) delete e;
        tabs->clear();
        delete tabs;
    }
    this->tabs = 0;
}

extern "C" ListItem *HangarList_getCurrentItem_tail(HangarList *self,
                                                    ListItem *item);

ListItem *HangarList::getCurrentItem() {
    return HangarList_getCurrentItem_tail(this, this->currentItem);
}

// getCurrentItem() tail-calls here with the current-selection index (the +0x8
// field is used as an int index, not a live pointer). It is just the indexed
// lookup, so forward to getCurrentItemAt().
extern "C" ListItem *HangarList_getCurrentItem_tail(HangarList *self,
                                                    ListItem *item) {
    return self->getCurrentItemAt((int)(intptr_t)item);
}

ListItem *HangarList::getCurrentItemAt(int index) {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    if (tabs != 0) {
        uint32_t current = this->currentTab;
        Array<ListItem *> *items = (*tabs)[current];
        if (items != 0) {
            ListItem *result = 0;
            if (-1 < index) {
                int length = items->size();
                if (length > index) {
                    result = (*items)[index];
                }
                return result;
            }
            return 0;
        }
    }
    return 0;
}

void HangarList::initBlueprintTab(Array<BluePrint *> *blueprints) {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    Array<ListItem *> *old = (*tabs)[2];
    if (old != 0) {
        for (ListItem *e : *old) delete e;
        old->clear();
        delete old;
    }
    (*this->tabs)[2] = 0;

    Array<ListItem *> *list = new Array<ListItem *>();

    uint32_t unlocked = 1;
    for (uint32_t i = 0; i < blueprints->size(); ++i) {
        unlocked += ((BluePrint *)((*blueprints)[i]))->isUnlocked();
    }

    Array<PendingProduct *> *pending =
        (Array<PendingProduct *> *)((Status *)(g_HangarList_status))->getPendingProducts();
    uint32_t length = unlocked;
    bool noPending = true;
    if (pending != 0) {
        uint32_t i = 0;
        while (pending->size() != i) {
            if ((*pending)[i] != 0) {
                ++length;
            }
            ++i;
        }
        noPending = length <= unlocked;
        if (unlocked < length) {
            ++length;
        }
    }

    list->resize(length);

    ListItem *li = (ListItem *)::operator new(0x48);
    GameText **texts = g_HangarList_gameText;
    new (li) ListItem(((GameText *)(*texts))->getText(0x111));
    (*list)[0] = li;

    uint32_t out = 1;
    for (uint32_t i = 0; i < blueprints->size(); ++i) {
        if (((BluePrint *)((*blueprints)[i]))->isUnlocked() != 0) {
            li = (ListItem *)::operator new(0x48);
            new (li) ListItem((*blueprints)[i]);
            (*list)[out] = li;
            ++out;
        }
    }

    if (!noPending) {
        li = (ListItem *)::operator new(0x48);
        new (li) ListItem(((GameText *)(*texts))->getText(0x112));
        uint32_t pendingOut = out + 1;
        (*list)[out] = li;
        for (uint32_t i = 0; i < pending->size(); ++i) {
            if ((*pending)[i] != 0) {
                li = (ListItem *)::operator new(0x48);
                new (li) ListItem((*pending)[i]);
                (*list)[pendingOut] = li;
                ++pendingOut;
            }
        }
    }

    (*this->tabs)[2] = list;
}

void HangarList::fillIngredientsList(BluePrint *blueprint, bool flag) {
    (void)flag;
    Ship *ship = ((Status *)(g_HangarList_status))->getShip();
    Array<Item *> *cargo = ((Ship *)(ship))->getCargo();
    Array<Item *> *allItems = g_HangarList_items;
    Array<int32_t> *ingredients = BluePrint_getIngredientList(blueprint);
    uint32_t count = ingredients->size();

    Array<ListItem *> *list = new Array<ListItem *>();
    list->resize(count + 1);

    ListItem *li = (ListItem *)::operator new(0x48);
    GameText **texts = g_HangarList_gameText;
    new (li) ListItem(((GameText *)(*texts))->getText(((BluePrint *)(blueprint))->getIndex() + 0x4fa));
    (*list)[0] = li;

    uint32_t out = 1;
    for (uint32_t i = 0; i < ingredients->size(); ++i) {
        Item *shown = 0;
        if (cargo != 0) {
            for (uint32_t j = 0; j < cargo->size(); ++j) {
                if (((Item *)((*cargo)[j]))->getIndex() == (*ingredients)[i]) {
                    li = (ListItem *)::operator new(0x48);
                    new (li) ListItem((*cargo)[j]);
                    (*list)[out] = li;
                    shown = (*cargo)[j];
                    goto got_item;
                }
            }
        }
        shown = (*allItems)[(*ingredients)[i]];
        li = (ListItem *)::operator new(0x48);
        new (li) ListItem(shown);
        (*list)[out] = li;
    got_item:
        ((Item *)(shown))->setBlueprintAmount(0);
        ++i;
        ++out;
        --i;
    }

    (*this->tabs)[4] = list;
    li = (ListItem *)::operator new(0x48);
    new (li) ListItem(0);
    li->field_0x24 = 0;
    (*this->tabs)[4]->push_back(li);
}

void HangarList::fillBuyList(ListItem *item) {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    Array<ListItem *> *old = (*tabs)[3];
    if (old != 0) {
        for (ListItem *e : *old) delete e;
        old->clear();
        delete old;
    }
    (*this->tabs)[3] = 0;

    uint32_t isShip = ((ListItem *)(item))->isShip();
    int type = item->field_0x28;
    int count = 0;
    if (isShip == 0) {
        if (((ListItem *)(item))->isSlot() == 0) {
            type = ((Item *)(item->field_0x10))->getType();
        }
        Ship *ship = ((Status *)(g_HangarList_status))->getShip();
        Array<Item *> *cargo = ((Ship *)(ship))->getCargo();
        if (cargo != 0) {
            for (uint32_t i = 0; i < cargo->size(); ++i) {
                if (((Item *)((*cargo)[i]))->getType() == type) {
                    ++count;
                }
            }
        }
    } else {
        Station *station = ((Status *)(g_HangarList_status))->getStation();
        Array<Ship *> *ships = Station_getShips(station);
        if (ships != 0) {
            count = ships->size();
        }
    }

    uint32_t isSlot = ((ListItem *)(item))->isSlot();
    Array<ListItem *> *list = new Array<ListItem *>();
    int base = 3;
    uint32_t special = isShip | isSlot;
    if (count == 0) {
        base = 4;
    }
    uint32_t length = base + count + (special ^ 1);
    if (special == 0) {
        length += 2;
    }
    list->resize(length);

    GameText **texts = g_HangarList_gameText;
    ListItem *li = (ListItem *)::operator new(0x48);
    new (li) ListItem(((GameText *)(*texts))->getText(0x115));
    (*list)[0] = li;

    li = (ListItem *)::operator new(0x48);
    new (li) ListItem(*item);
    (*list)[1] = li;

    uint32_t out;
    if (special == 0) {
        li = (ListItem *)::operator new(0x48);
        new (li) ListItem(((GameText *)(*texts))->getText(0x116));
        (*list)[2] = li;

        li = (ListItem *)::operator new(0x48);
        new (li) ListItem(((GameText *)(*texts))->getText(0x117), true, 0);
        (*list)[3] = li;

        li = (ListItem *)::operator new(0x48);
        new (li) ListItem(((GameText *)(*texts))->getText(0x11a), true, 1);
        (*list)[4] = li;
        out = 5;
    } else {
        out = 2;
    }

    li = (ListItem *)::operator new(0x48);
    new (li) ListItem(((GameText *)(*texts))->getText(isShip == 0 ? 0x11c : 0x11d));
    uint32_t next = out + 1;
    (*list)[out] = li;

    if (count < 1) {
        li = (ListItem *)::operator new(0x48);
        new (li) ListItem(next);
        (*list)[next] = li;
    } else if (isShip == 0) {
        Ship *ship = ((Status *)(g_HangarList_status))->getShip();
        Array<Item *> *cargo = ((Ship *)(ship))->getCargo();
        for (uint32_t i = 0; i < cargo->size(); ++i) {
            if (((Item *)((*cargo)[i]))->getType() == type) {
                li = (ListItem *)::operator new(0x48);
                new (li) ListItem((*cargo)[i]);
                (*list)[next] = li;
                ++next;
            }
        }
    } else {
        Station *station = ((Status *)(g_HangarList_status))->getStation();
        Array<Ship *> *ships = Station_getShips(station);
        for (uint32_t i = 0; i < ships->size(); ++i) {
            li = (ListItem *)::operator new(0x48);
            new (li) ListItem((*ships)[i]);
            (*list)[out + 1 + i] = li;
        }
    }

    (*this->tabs)[3] = list;
}

void HangarList::initShipTab(Ship *ship) {
    Array<Array<ListItem *> *> *tabs = this->tabs;
    Array<ListItem *> *old = (*tabs)[0];
    if (old != 0) {
        for (ListItem *e : *old) delete e;
        old->clear();
        delete old;
    }
    (*this->tabs)[0] = 0;

    Array<ListItem *> *items = new Array<ListItem *>();
    Array<Item *> *equipment = ((Ship*)(ship))->getEquipment();
    Array<Item *> *cargo = ((Ship *)(ship))->getCargo();
    Array<Item *> *available;
    int baseLength;
    bool cargoEmpty;
    if (cargo == 0) {
        baseLength = 2;
        available = 0;
        cargoEmpty = true;
    } else {
        available = new Array<Item *>();
        for (uint32_t i = 0; i < cargo->size(); ++i) {
            Item *item = (*cargo)[i];
            int type = ((Item *)(item))->getType();
            if (type != 4) {
                type = ((Item *)((*cargo)[i]))->getType();
                if (((Ship *)(ship))->getSlots(type) > 0) {
                    available->push_back((*cargo)[i]);
                }
            }
        }
        cargoEmpty = false;
        baseLength = available->size() + 2;
    }

    uint32_t equipmentLength = equipment->size();
    int slotTypes = ((Ship *)(ship))->getSlotTypes();
    items->resize(slotTypes + baseLength + equipmentLength);

    ListItem *li = (ListItem *)::operator new(0x48);
    GameText **texts = g_HangarList_gameText;
    new (li) ListItem(((GameText *)(*texts))->getText(0xb7));
    (*items)[0] = li;

    li = (ListItem *)::operator new(0x48);
    new (li) ListItem(ship);
    uint32_t out = 2;
    (*items)[1] = li;

    for (uint32_t type = 0; type < 4; ++type) {
        if (((Ship *)(ship))->getSlots(type) > 0) {
            int textId = 0x10d;
            if (type < 3) {
                textId = type + 0x109;
            }
            li = (ListItem *)::operator new(0x48);
            new (li) ListItem(((GameText *)(*texts))->getText(textId), (int)type);
            (*items)[out] = li;
            Array<Item *> *slotItems = Ship_getEquipmentByType(ship, type);
            for (uint32_t j = 0; j < slotItems->size(); ++j) {
                ++out;
                Item *item = (*slotItems)[j];
                li = (ListItem *)::operator new(0x48);
                if (item == 0) {
                    new (li) ListItem(type);
                } else {
                    new (li) ListItem((*slotItems)[j]);
                }
                (*items)[out] = li;
                uint32_t k = 0;
                for (; k < equipment->size(); ++k) {
                    ListItem *entry = (*items)[out];
                    if ((*equipment)[k] == entry->field_0x10) {
                        entry->field_0x40 = k;
                        break;
                    }
                }
                ((*items)[out])->field_0x3c = j;
            }
            delete slotItems;
            if (!cargoEmpty) {
                for (uint32_t j = 0; j < available->size(); ++j) {
                    if ((uint32_t)((Item *)((*available)[j]))->getType() == type) {
                        li = (ListItem *)::operator new(0x48);
                        new (li) ListItem((*available)[j]);
                        (*items)[out] = li;
                        ++out;
                    }
                }
            }
        }
    }
    (*this->tabs)[0] = items;
}

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

    Array<Array<ListItem *> *> *tabs = this->tabs;
    Array<ListItem *> *old = (*tabs)[1];
    if (old != 0) {
        for (ListItem *e : *old) delete e;
        old->clear();
        delete old;
    }
    (*this->tabs)[1] = 0;

    if ((shopItems == 0 || shopItems->size() == 0) &&
        (ships == 0 || ships->size() == 0)) {
        return;
    }

    Array<ListItem *> *list = new Array<ListItem *>();
    if (shopItems != 0) {
        for (uint32_t i = 0; i < shopItems->size(); ++i) {
            int type = ((Item *)((*shopItems)[i]))->getType();
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
    list->resize(length);

    uint32_t out;
    GameText **texts = g_HangarList_gameText;
    if (ships != 0 && ships->size() != 0) {
        ListItem *li = (ListItem *)::operator new(0x48);
        new (li) ListItem(((GameText *)(*texts))->getText(0xad), -1);
        (*list)[0] = li;
        for (uint32_t i = 0; i < ships->size(); ++i) {
            ((Ship *)((*ships)[i]))->adjustPrice();
            li = (ListItem *)::operator new(0x48);
            new (li) ListItem((*ships)[i]);
            (*list)[i + 1] = li;
        }
        out = ships->size() + 1;
    } else {
        out = 0;
    }

    for (uint32_t type = 0; type < 5; ++type) {
        if (counts[type] > 0) {
            ListItem *li = (ListItem *)::operator new(0x48);
            new (li) ListItem(((GameText *)(*texts))->getText(shopTypeTextId(type)), (int)type);
            (*list)[out] = li;
            ++out;
            if (shopItems != 0) {
                for (uint32_t i = 0; i < shopItems->size(); ++i) {
                    if ((uint32_t)((Item *)((*shopItems)[i]))->getType() == type) {
                        li = (ListItem *)::operator new(0x48);
                        new (li) ListItem((*shopItems)[i]);
                        (*list)[out] = li;
                        ++out;
                    }
                }
            }
        }
    }
    (*this->tabs)[1] = list;
}

extern "C" int HangarList_initBlueprintTab_tail(HangarList *self,
                                                Array<BluePrint *> *blueprints);

int HangarList::init(Ship *ship, Array<Item *> *items, Array<Ship *> *ships,
                     Array<BluePrint *> *blueprints) {
    release();
    Array<BluePrint *> *bp = blueprints;
    Array<Array<ListItem *> *> *tabs = new Array<Array<ListItem *> *>();
    this->tabs = tabs;
    tabs->resize(5);
    initShipTab(ship);
    initShopTab(items, ships);
    return HangarList_initBlueprintTab_tail(this, bp);
}

// init() builds the ship and shop tabs, then tail-calls here to populate the
// blueprint tab from the supplied blueprint array.
extern "C" int HangarList_initBlueprintTab_tail(HangarList *self,
                                                Array<BluePrint *> *blueprints) {
    self->initBlueprintTab(blueprints);
    return 0;
}

// ---- C-ABI dtor / accessor wrappers (recovered shims) ----

// HangarList_dtor — C-ABI destructor. Runs ~HangarList() (which releases the
// tab arrays) and returns the storage for the caller's operator delete.
extern "C" void *HangarList_dtor(void *p) {
    if (p) ((HangarList *)p)->~HangarList();
    return p;
}
