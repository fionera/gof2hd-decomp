#include "game/mission/Item.h"
#include "game/core/Globals.h"
#include "game/mission/Status.h"
#include "game/ship/Ship.h"
#include "game/world/Station.h"
#include "engine/render/LODManager.h"

static Status *&status = Globals::status;

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
    minPriceSystem = data[9];
    maxPriceSystem = data[11];
    occurence = data[13];
    minPrice = data[15];
    maxPrice = data[17];
    amount = 0;
    stationAmount = 0;
    blueprintAmount = 0;
    missingIngredients = 0;
    unsaleable = false;
    price = minPrice + (maxPrice - minPrice) / 2;
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
    int *data = attributes->data();
    uint32_t size = attributes->size();
    for (uint32_t index = 0; index < size; index += 2) {
        if (data[index] == attribute) {
            return data[index + 1];
        }
    }
    return static_cast<int>(0xc5997825);
}

int Item::transaction(bool buy, int, bool useCredits) {
    if (buy) {
        int currentStationAmount = stationAmount;
        if (currentStationAmount > 0) {
            int currentPrice;
            if (useCredits) {
                currentPrice = price;
            } else {
                currentPrice = price;
                if (status->getCredits() < currentPrice) {
                    return 0;
                }
                currentStationAmount = stationAmount;
            }
            amount += 1;
            stationAmount = currentStationAmount - 1;
            return -currentPrice;
        }
    } else if (amount > 0) {
        amount -= 1;
        stationAmount += 1;
        return price;
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
    int *ingredientIds = item->ingredients->data();
    int *ingredientQuantities = item->quantities->data();
    uint32_t ingredientCount = ingredientIds[0];

    for (uint32_t i = 0; i < ingredientCount; i++) {
        int wantedId = ingredientIds[i + 1];
        for (uint32_t j = 0; j < items->size(); j++) {
            Item * candidate = (*items)[j];
            if (candidate->index == wantedId) {
                candidate->amount -= ingredientQuantities[i + 1] * amount;
                break;
            }
        }
    }

    ItemArray *made = new ItemArray();
    ArrayAdd(item->makeItem(amount), *made);
    for (Item * element


    :
    *items
    )
    {
        ArrayAdd(element, *made);
    }
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
        Item * item = (*items)[i];
        int amount = station ? item->amount : item->stationAmount;
        if (amount > 0) {
            ArrayAdd(item->makeItem(), *extracted);
        }
    }

    if (extracted->size() == 0) {
        return nullptr;
    }
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
    uint32_t itemCount = items ? static_cast<uint32_t>(items->size()) : 0;
    int stationCount = stationItems ? static_cast<int>(stationItems->size()) : 0;

    ItemArray *mixed = new ItemArray();
    ArraySetLength(stationCount + itemCount, *mixed);

    if (static_cast<int>(itemCount) >= 1 && stationCount == 0) {
        for (uint32_t i = 0; i < items->size(); i++) {
            Item * item = (*items)[i];
            (*mixed)[i] = item->makeItem(item->amount);
        }
    } else if (itemCount == 0 && stationCount > 0) {
        for (uint32_t i = 0; i < stationItems->size(); i++) {
            Item * item = (*stationItems)[i];
            Item * copy = item->makeItem(0);
            (*mixed)[i] = copy;
            copy->stationAmount = item->amount;
        }
    } else if ((stationCount | static_cast<int>(itemCount)) == 0) {
        mixed = nullptr;
    } else {
        for (uint32_t i = 0; i < items->size(); i++) {
            Item * item = (*items)[i];
            (*mixed)[i] = item->makeItem(item->amount);
        }

        for (uint32_t stationIndex = 0; stationIndex < stationItems->size(); stationIndex++) {
            for (uint32_t mixedIndex = 0; mixedIndex < mixed->size(); mixedIndex++) {
                Item * mixedItem = (*mixed)[mixedIndex];
                Item * stationItem = (*stationItems)[stationIndex];
                if (mixedItem == nullptr) {
                    Item * copy = stationItem->makeItem(0);
                    itemCount++;
                    (*mixed)[mixedIndex] = copy;
                    copy->stationAmount = stationItem->amount;
                    break;
                }
                if (stationItem->index == mixedItem->index) {
                    Item * copy = stationItem->makeItem(mixedItem->amount);
                    (*mixed)[mixedIndex] = copy;
                    copy->stationAmount = stationItem->amount;
                    break;
                }
            }
        }

        ItemArray *trimmed = new ItemArray();
        ArraySetLength(itemCount, *trimmed);
        for (uint32_t i = 0; i < itemCount; i++) {
            (*trimmed)[i] = (*mixed)[i];
        }

        delete mixed;

        uint32_t size = static_cast<uint32_t>(trimmed->size());
        bool swapped;
        do {
            swapped = false;
            for (uint32_t i = 1; i < size; i++) {
                Item * right = (*trimmed)[i];
                Item * left = (*trimmed)[i - 1];
                if (right->index < left->index) {
                    (*trimmed)[i - 1] = right;
                    (*trimmed)[i] = left;
                    swapped = true;
                }
            }
        } while (swapped);

        Item::combineDuplicates(trimmed);
        mixed = trimmed;
    }

    return mixed;
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
    int requiredLoad = amount + status->getShip()->getCurrentLoad();
    return requiredLoad <= status->getShip()->getMaxLoad();
}

bool Item::isUnsaleable() {
    return unsaleable;
}
