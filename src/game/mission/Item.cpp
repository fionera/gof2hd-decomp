#include "game/mission/Item.h"
#include "game/core/Globals.h"
#include "game/mission/Status.h"
#include "game/ship/Ship.h"
#include "game/world/Station.h"
#include "engine/render/LODManager.h"

static Status *&status = Globals::status;

// Layout::Layout() compiles to 6792 B, 280 B shorter than the original.  The mtwTextLayoutAnchor
// helper in MenuTouchWindow.cpp grows by the same 280 B, so every function after it (including
// canBeInstalledMultipleTimes) keeps its original address and this table's PC-relative literal
// needs no .rodata pad.

static const bool canBeInstalledMultipleTimesBySort[] = {
    true, true, true, true, true, true, true, true,
    false, false, false, true, true, false, false, false,
    false, false, false, false, true, false, true, true,
    true, true, false, false, false, false, false, false,
    true, false, true, false, true, false, false, true,
    true, false, true,
};

Item::Item(IntArray *ingredients_, IntArray *quantities_, IntArray *attributes_)
    : ingredients(ingredients_), quantities(quantities_), attributes(attributes_) {
    init();
}

void Item::init() {
    if (attributes == nullptr) {
        return;
    }

    int *data = attributes->data();

    index = data[1];
    type = data[3];
    sort = data[5];
    tecLevel = data[7];
    occurence = data[13];
    minPrice = data[15];
    maxPrice = data[17];
    minPriceSystem = data[9];
    maxPriceSystem = data[11];
    amount = 0;
    stationAmount = 0;
    blueprintAmount = 0;
    missingIngredients = 0;
    price = minPrice + (maxPrice - minPrice) / 2;
    unsaleable = false;
}

Item::~Item() {
}

void Item::setUnsaleable(bool value) {
    unsaleable = value;
}

bool Item::canBeInstalledMultipleTimes() {
    return canBeInstalledMultipleTimesBySort[sort];
}

int Item::getIndex() {
    return index;
}

int Item::getType() {
    return type;
}

int Item::getTecLevel() {
    return tecLevel;
}

int Item::getSort() {
    return sort;
}

int Item::getSinglePrice() {
    return price;
}

int Item::getTotalPrice() {
    return price * amount;
}

int Item::getMaxPrice() {
    return maxPrice;
}

int Item::getMinPrice() {
    return minPrice;
}

int Item::getMaxPriceSystem() {
    return maxPriceSystem;
}

int Item::getMinPriceSystem() {
    return minPriceSystem;
}

void Item::setPrice(int value) {
    price = value;
}

void Item::setMinPrice(int value) {
    minPrice = value;
}

void Item::setMaxPrice(int value) {
    maxPrice = value;
}

float Item::getPriceRate() {
    return static_cast<float>(price - minPrice) /
           static_cast<float>(maxPrice - minPrice);
}

void Item::setAmount(int value) {
    amount = value;
}

int Item::getOccurence() {
    return occurence;
}

int Item::getAmount() {
    return amount;
}

void Item::changeAmount(int delta) {
    amount += delta;
}

int Item::getMissingIngredients() {
    return missingIngredients;
}

void Item::setMissingIngredients(int value) {
    missingIngredients = value;
}

void Item::setStationAmount(int value) {
    stationAmount = value;
}

int Item::getStationAmount() {
    return stationAmount;
}

void Item::changeStationAmount(int delta) {
    stationAmount += delta;
}

void Item::setBlueprintAmount(int value) {
    blueprintAmount = value;
}

int Item::getBlueprintAmount() {
    return blueprintAmount;
}

void Item::changeBlueprintAmount(int delta) {
    blueprintAmount += delta;
}

IntArray *Item::getIngredients() {
    return ingredients;
}

IntArray *Item::getQuantities() {
    return quantities;
}

IntArray *Item::getAttributes() {
    return attributes;
}

int Item::getAttribute(int attribute) {
    // Attributes are stored as (id, value) pairs.
    for (uint32_t i = 0; i < attributes->size(); i += 2) {
        if ((*attributes)[i] == attribute)
            return (*attributes)[i + 1];
    }
    return static_cast<int>(0xc5997825);
}

int Item::transaction(bool buy, int, bool useCredits) {
    if (buy) {
        if (this->stationAmount > 0) {
            if (!useCredits && Globals::status->getCredits() < this->price)
                return 0;
            this->amount++;
            this->stationAmount--;
            return -this->price;
        }
    } else if (this->amount > 0) {
        this->amount--;
        this->stationAmount++;
        return this->price;
    }
    return 0;
}

int Item::transactionBlueprint(bool fabricate, int mode) {
    if (fabricate) {
        if (blueprintAmount >= 1) {
            blueprintAmount -= 1;
            amount += 1;
            return -price;
        }
    } else if (amount >= 1) {
        int p = price;
        amount -= 1;
        blueprintAmount += 1;
        return p;
    }
    return 0;
}

bool Item::isInList(int index, int amount, ItemArray *items) {
    if (items != nullptr) {
        uint32_t size = items->size();
        for (uint32_t i = 0; i < size; i++) {
            Item *item = (*items)[i];
            if (item->index == index && amount <= item->amount) {
                return true;
            }
        }
    }
    return false;
}

bool Item::isInList(Item *item, ItemArray *items) {
    return isInList(item->index, items);
}

bool Item::isInList(int index, ItemArray *items) {
    return isInList(index, 1, items);
}

void Item::fabricate(Item *item, ItemArray *items, int amount) {
    // Consume the ingredients ...
    IntArray *ingredients = item->ingredients;
    IntArray *quantities = item->quantities;
    for (uint32_t i = 0; i < ingredients->size(); i++) {
        for (uint32_t j = 0; j < items->size(); j++) {
            if ((*items)[j]->index == (*ingredients)[i]) {
                (*items)[j]->amount -= (*quantities)[i] * amount;
                break;
            }
        }
    }
    // ... and add the product.
    ItemArray *made = new ItemArray();
    ArrayAdd(item->makeItem(amount), *made);
    combineItems(items, made);
}

Item *Item::makeItem(int amount) {
    int savedPrice = price;
    Item *item = clone();
    item->price = savedPrice;
    item->amount = amount;
    return item;
}

ItemArray *Item::combineItems(ItemArray *items, ItemArray *stationItems) {
    if (items == nullptr) {
        return stationItems;
    }
    if (stationItems == nullptr) {
        return items;
    }
    ItemArray *result = items;

    ItemArray *stationCopy = new ItemArray();
    ArraySetLength(stationItems->size(), *stationCopy);
    for (uint32_t i = 0; i < stationItems->size(); i++) {
        (*stationCopy)[i] = (*stationItems)[i];
    }

    uint32_t remaining = static_cast<uint32_t>(stationCopy->size());
    for (uint32_t itemIndex = 0; itemIndex < items->size(); itemIndex++) {
        for (uint32_t stationIndex = 0; stationIndex < stationCopy->size(); stationIndex++) {
            Item * stationItem = (*stationCopy)[stationIndex];
            if (stationItem != nullptr) {
                Item * item = (*items)[itemIndex];
                if (item->index == stationItem->index) {
                    remaining--;
                    item->amount += stationItem->amount;
                    (*stationCopy)[stationIndex] = nullptr;
                }
            }
        }
    }


    if (static_cast<int>(remaining) > 0) {
        ItemArray *unmatched = new ItemArray();
        ArraySetLength(remaining, *unmatched);
        uint32_t unmatchedIndex = 0;
        for (uint32_t i = 0; i < stationCopy->size(); i++) {
            Item * item = (*stationCopy)[i];
            if (item != nullptr) {
                (*unmatched)[unmatchedIndex++] = item;
            }
        }

        result = new ItemArray();
        ArraySetLength(items->size() + unmatched->size(), *result);
        uint32_t itemCount = static_cast<uint32_t>(items->size());
        for (uint32_t i = 0; i < itemCount; i++) {
            (*result)[i] = (*items)[i];
        }
        for (uint32_t i = 0; i < unmatched->size(); i++) {
            (*result)[itemCount + i] = (*unmatched)[i];
        }
    }
    return result;
}

ItemArray *Item::extractItems(ItemArray *items, bool station) {
    if (items == nullptr) {
        return nullptr;
    }

    ItemArray *extracted = new ItemArray();
    for (uint32_t i = 0; i < items->size(); i++) {
        Item *item = (*items)[i];
        if (station) {
            if (item->amount > 0)
                ArrayAdd(item->makeItem(item->amount), *extracted);
        } else if (item->stationAmount > 0) {
            ArrayAdd(item->makeItem(item->stationAmount), *extracted);
        }
    }
    if (extracted->size() == 0)
        extracted = nullptr;
    return extracted;
}

void Item::combineDuplicates(ItemArray *items) {
    if (items == nullptr) {
        return;
    }

    uint32_t size = static_cast<uint32_t>(items->size());
    for (uint32_t i = 0; i < size; i++) {
        for (uint32_t j = i + 1; j < size; j++) {
            Item * left = (*items)[i];
            Item * right = (*items)[j];
            if (left->index == right->index) {
                left->amount += right->amount;
                right->amount = 0;
                left->stationAmount += right->stationAmount;
                right->stationAmount = 0;
            }
        }
    }
    for (uint32_t i = 0; i < size; i++) {
        Item * item = (*items)[i];
        if (item->amount == 0 && item->stationAmount == 0) {
            ArrayRemove<Item *>(item, *items);
            size = items->size();
        }
    }
}

ItemArray *Item::mixItems(ItemArray *items, ItemArray *stationItems) {
    int itemCount = items != nullptr ? (int) items->size() : 0;
    int stationCount = stationItems != nullptr ? (int) stationItems->size() : 0;

    ItemArray *mixed = new ItemArray();
    ArraySetLength(stationCount + itemCount, *mixed);

    if (itemCount > 0 && stationCount == 0) {
        for (uint32_t i = 0; i < items->size(); i++)
            (*mixed)[i] = (*items)[i]->makeItem((*items)[i]->amount);
        return mixed;
    }
    if (itemCount == 0 && stationCount > 0) {
        for (uint32_t i = 0; i < stationItems->size(); i++) {
            (*mixed)[i] = (*stationItems)[i]->makeItem(0);
            (*mixed)[i]->stationAmount = (*stationItems)[i]->amount;
        }
        return mixed;
    }
    if (itemCount == 0 && stationCount == 0)
        return nullptr;

    // Own items first, then merge the station items into them (or append new entries).
    for (uint32_t i = 0; i < items->size(); i++)
        (*mixed)[i] = (*items)[i]->makeItem((*items)[i]->amount);

    for (uint32_t s = 0; s < stationItems->size(); s++) {
        for (uint32_t m = 0; m < mixed->size(); m++) {
            Item *mixedItem = (*mixed)[m];
            Item *stationItem = (*stationItems)[s];
            if (mixedItem == nullptr) {
                (*mixed)[m] = stationItem->makeItem(0);
                itemCount++;
                (*mixed)[m]->stationAmount = (*stationItems)[s]->amount;
                break;
            }
            if (stationItem->index == mixedItem->index) {
                (*mixed)[m] = stationItem->makeItem(mixedItem->amount);
                (*mixed)[m]->stationAmount = (*stationItems)[s]->amount;
                break;
            }
        }
    }

    ItemArray *result = new ItemArray();
    ArraySetLength(itemCount, *result);
    for (int i = 0; i < itemCount; i++)
        (*result)[i] = (*mixed)[i];
    delete mixed;

    // Bubble sort by item index.
    uint32_t size = result->size();
    bool sorted;
    do {
        sorted = true;
        for (uint32_t i = 1; i < size; i++) {
            Item *left = (*result)[i - 1];
            Item *right = (*result)[i];
            if (left->index > right->index) {
                (*result)[i - 1] = right;
                sorted = false;
                (*result)[i] = left;
            }
        }
    } while (!sorted);

    combineDuplicates(result);
    return result;
}

bool Item::equals(Item *other) {
    if (other != nullptr) {
        return index == other->index;
    }
    return false;
}

bool Item::isWeapon() {
    return static_cast<uint32_t>(type) < 3;
}

Item *Item::makeItem(int amount, int price) {
    Item *item = clone();
    item->price = price;
    item->amount = amount;
    return item;
}

Item *Item::clone() {
    Item *copy = new Item(ingredients, quantities, attributes);
    copy->price = price;
    copy->unsaleable = unsaleable;
    copy->amount = amount;
    copy->stationAmount = stationAmount;
    return copy;
}

Item *Item::makeItem() {
    int savedPrice = price;
    Item *item = clone();
    item->price = savedPrice;
    item->amount = 1;
    return item;
}

bool Item::checkCredits() {
    return status->getCredits() >= price;
}

void Item::adjustPrice(Station *station) {
    int basePrice = minPrice;
    price = basePrice + static_cast<int>((static_cast<float>(10 - station->getTecLevel()) / 10.0f) *
                                         static_cast<float>(maxPrice - minPrice));
}

bool Item::checkCargoSpace() {
    return status->getShip()->getCurrentLoad() + amount <= status->getShip()->getMaxLoad();
}

bool Item::isUnsaleable() {
    return unsaleable;
}
