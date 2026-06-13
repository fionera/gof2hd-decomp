#include "gof2/game/mission/Item.h"

extern "C" void Array_Item_ctor(ItemArray *array);
extern "C" void ArraySetLength_Item(uint32_t length, ItemArray *array);
extern "C" void ArrayAdd_Item(Item *item, ItemArray *array);
extern "C" void ArrayAdd_Array(ItemArray *items, ItemArray *array);
extern "C" void ArrayRemove_Item(Item *item, ItemArray *items);
extern "C" void *Array_Item_dtor(ItemArray *array);
extern "C" void ArraySetLength_Item(uint32_t size, ItemArray *array);

// Player status singleton (defined elsewhere); only the accessors used here.
struct Ship {
    int getCurrentLoad();
    int getMaxLoad();
};
struct Status {
    int getCredits();
    Ship *getShip();
};
extern Status *status;

__attribute__((minsize))
int Item::getAttribute(int attribute)
{
    ItemArray *v = attributes;
    int result = (int)0xc5997825;
    uint32_t size = v->size();
    for (uint32_t index = 0; index < size; index += 2) {
        int *data = (int *)v->data();
        if (data[index] == attribute) {
            return data[index + 1];
        }
    }
    return result;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

ItemArray *Item::getQuantities() {
    return quantities;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getMissingIngredients() {
    return missingIngredients;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

struct Station {
    int getTecLevel();
};

void Item::adjustPrice(Station *station)
{
    int basePrice = minPrice;
    price = basePrice + static_cast<int>((static_cast<float>(10 - station->getTecLevel()) / 10.0f) *
                                         static_cast<float>(maxPrice - minPrice));
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getTecLevel() {
    return tecLevel;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setUnsaleable(bool value)
{
    unsaleable = value;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setMaxPrice(int value) {
    maxPrice = value;
}

__attribute__((minsize)) bool Item::checkCredits()
{
    int credits = status->getCredits();
    return credits >= price;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getOccurence() {
    return occurence;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setMissingIngredients(int value) {
    missingIngredients = value;
}

#define wchar_t gof2_wchar_t

Item *Item::makeItem(int amount)
{
    int savedPrice = price;
    Item *item = clone();
    item->price = savedPrice;
    item->amount = amount;
    return item;
}

static const bool canBeInstalledMultipleTimesBySort[] = {
    true,  true,  true,  true,  true,  true,  true,  true,
    false, false, false, true,  true,  false, false, false,
    false, false, false, false, true,  false, true,  true,
    true,  true,  false, false, false, false, false, false,
    true,  false, true,  false, true,  false, false, true,
    true,  false, true,
};

__attribute__((minsize)) bool Item::canBeInstalledMultipleTimes()
{
    return canBeInstalledMultipleTimesBySort[sort];
}

__attribute__((minsize)) int Item::transaction(bool buy, int, bool useCredits)
{
    if (buy) {
        int currentStationAmount = stationAmount;
        if (currentStationAmount > 0) {
            int currentPrice;
            if (useCredits) {
                currentPrice = price;
            } else {
                int credits = status->getCredits();
                currentPrice = price;
                if (credits < currentPrice) {
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

void Item::setStationAmount(int value) {
    stationAmount = value;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

ItemArray *Item::getIngredients() {
    return ingredients;
}

int Item::getMaxPrice() {
    return maxPrice;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getType() {
    return type;
}

void Item::setAmount(int value) {
    amount = value;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

bool Item::isInList(Item *item, ItemArray *items)
{
    return isInList(item->index, items);
}

#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::changeBlueprintAmount(int delta) {
    blueprintAmount += delta;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setMinPrice(int value) {
    minPrice = value;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getMinPriceSystem()
{
    return minPriceSystem;
}

int Item::getStationAmount()
{
    return stationAmount;
}

int Item::getTotalPrice() {
    return price * amount;
}

void Item::changeAmount(int delta)
{
    amount += delta;
}

// ---- addAmount (Station::addItem veneer): accumulate amount onto an existing item ----
void Item::addAmount(int delta)
{
    amount += delta;
}

Item *Item::makeItem(int amount, int price)
{
    Item *item = clone();
    item->price = price;
    item->amount = amount;
    return item;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

Item::~Item() {}

__attribute__((minsize))
int Item::transactionBlueprint(bool fabricate)
{
    if (fabricate) {
        int blueprints = blueprintAmount;
        if (blueprints > 0) {
            int currentAmount = amount;
            int currentPrice = price;
            blueprintAmount = blueprints - 1;
            amount = currentAmount + 1;
            return -currentPrice;
        }
    } else {
        int currentAmount = amount;
        if (currentAmount > 0) {
            int currentPrice = price;
            int blueprints = blueprintAmount;
            amount = currentAmount - 1;
            blueprintAmount = blueprints + 1;
            return currentPrice;
        }
    }

    return 0;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setPrice(int value) {
    price = value;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getSort() {
    return sort;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getBlueprintAmount()
{
    return blueprintAmount;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

Item *Item::makeItem() {
    int savedPrice = price;
    Item *item = clone();
    item->price = savedPrice;
    item->amount = 1;
    return item;
}

int Item::getSinglePrice() {
    return price;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getIndex() {
    return index;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

float Item::getPriceRate() {
    return static_cast<float>(price - minPrice) /
           static_cast<float>(maxPrice - minPrice);
}

__attribute__((minsize))
bool Item::isInList(int index, int amount, ItemArray *items)
{
    if (items != 0) {
        ItemArray *v = items;
        uint32_t size = v->size();
        for (uint32_t i = 0; i < size; i++) {
            Item *item = static_cast<Item **>(v->data())[i];
            if (item->index == index && amount <= item->amount) {
                return true;
            }
        }
    }
    return false;
}

bool Item::isInList(int index, ItemArray *items) {
    return isInList(index, 1, items);
}

#define wchar_t gof2_wchar_t
#undef wchar_t

__attribute__((minsize)) bool Item::equals(Item *other)
{
    if (other != 0) {
        return index == other->index;
    }
    return false;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

Item *Item::clone()
{
    Item *copy = static_cast<Item *>(operator new(sizeof(Item)));
    copy->ingredients = ingredients;
    copy->quantities = quantities;
    copy->attributes = attributes;
    copy->init();
    copy->price = price;
    copy->unsaleable = unsaleable;
    copy->amount = amount;
    copy->stationAmount = stationAmount;
    return copy;
}

ItemArray *Item::getAttributes() {
    return attributes;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getMinPrice() {
    return minPrice;
}

__attribute__((minsize)) void Item::init()
{
    if (attributes == 0) {
        return;
    }

    int *data = reinterpret_cast<int *>(attributes->data());

    index = data[1];
    type = data[3];
    sort = data[5];
    tecLevel = data[7];
    occurence = data[13];
    minPrice = data[15];
    maxPrice = data[17];
    minPriceSystem = data[9];
    int maxSystem = data[11];
    amount = 0;
    stationAmount = 0;
    blueprintAmount = 0;
    missingIngredients = 0;
    maxPriceSystem = maxSystem;
    unsaleable = false;
    price = minPrice + (maxPrice - minPrice) / 2;

}

#define wchar_t gof2_wchar_t

bool Item::isUnsaleable()
{
    return unsaleable;
}

__attribute__((minsize)) Item::Item(ItemArray *ingredients_, ItemArray *quantities_, ItemArray *attributes_)
    : ingredients(ingredients_), quantities(quantities_), attributes(attributes_)
{
    init();
}

#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::changeStationAmount(int delta) {
    stationAmount += delta;
}

int Item::getAmount() {
    return amount;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getMaxPriceSystem() {
    return maxPriceSystem;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setBlueprintAmount(int value) {
    blueprintAmount = value;
}

__attribute__((minsize)) bool Item::checkCargoSpace()
{
    int currentLoad = status->getShip()->getCurrentLoad();
    int requiredLoad = amount + currentLoad;
    int maxLoad = status->getShip()->getMaxLoad();
    return requiredLoad <= maxLoad;
}

#define wchar_t gof2_wchar_t
#undef wchar_t

bool Item::isWeapon()
{
    return static_cast<uint32_t>(type) < 3;
}

ItemArray *Item::combineItems(ItemArray *items, ItemArray *stationItems)
{
    ItemArray *result = stationItems;
    if (items != 0) {
        result = items;
        if (stationItems != 0) {
            ItemArray *stationCopy = static_cast<ItemArray *>(operator new(sizeof(ItemArray)));
            Array_Item_ctor(stationCopy);
            ArraySetLength_Item(stationItems->size(), stationCopy);

            uint32_t stationCount = stationItems->size();
            for (uint32_t i = 0; stationCount != i; i++) {
                static_cast<Item **>(stationCopy->data())[i] =
                    static_cast<Item **>(stationItems->data())[i];
            }

            uint32_t copyCount = stationCopy->size();
            uint32_t remaining = copyCount;
            uint32_t itemCount = items->size();
            for (uint32_t itemIndex = 0; itemCount != itemIndex; itemIndex++) {
                for (uint32_t stationIndex = 0; copyCount != stationIndex; stationIndex++) {
                    Item **copyData = static_cast<Item **>(stationCopy->data());
                    Item *stationItem = copyData[stationIndex];
                    if (stationItem != 0) {
                        Item *item = static_cast<Item **>(items->data())[itemIndex];
                        if (item->index == stationItem->index) {
                            remaining--;
                            item->amount = stationItem->amount + item->amount;
                            copyData[stationIndex] = 0;
                        }
                    }
                }
            }

            if (static_cast<int>(remaining) > 0) {
                ItemArray *unmatched = static_cast<ItemArray *>(operator new(sizeof(ItemArray)));
                Array_Item_ctor(unmatched);
                ArraySetLength_Item(remaining, unmatched);

                uint32_t totalCopied = stationCopy->size();
                uint32_t unmatchedIndex = 0;
                for (uint32_t i = 0; totalCopied != i; i++) {
                    Item *item = static_cast<Item **>(stationCopy->data())[i];
                    if (item != 0) {
                        static_cast<Item **>(unmatched->data())[unmatchedIndex] = item;
                        unmatchedIndex++;
                    }
                }

                result = static_cast<ItemArray *>(operator new(sizeof(ItemArray)));
                Array_Item_ctor(result);
                ArraySetLength_Item(items->size() + unmatched->size(), result);

                uint32_t itemCountForCopy = items->size();
                for (uint32_t i = 0; itemCountForCopy != i; i++) {
                    static_cast<Item **>(result->data())[i] =
                        static_cast<Item **>(items->data())[i];
                }

                uint32_t unmatchedCount = unmatched->size();
                for (uint32_t i = 0; unmatchedCount != i; i++) {
                    static_cast<Item **>(result->data())[itemCountForCopy + i] =
                        static_cast<Item **>(unmatched->data())[i];
                }
            }
        }
    }
    return result;
}

__attribute__((minsize)) void Item::fabricate(Item *item, ItemArray *items, int amount)
{
    volatile uint32_t *ingredients = reinterpret_cast<volatile uint32_t *>(item->ingredients);
    ItemArray *quantities = item->quantities;
    volatile uint32_t *itemsRaw = reinterpret_cast<volatile uint32_t *>(items);
    uint32_t i = 0;
    uint32_t count = ingredients[0];

    goto check;
loop:
    {
        uint32_t itemCount = itemsRaw[0];
        uint32_t j = 0;
    inner:
        if (j < itemCount) {
            int *candidate = reinterpret_cast<int **>(itemsRaw[1])[j];
            j++;
            if (candidate[0] != reinterpret_cast<volatile int *>(ingredients[1])[i]) {
                goto inner;
            }
            candidate[13] -= reinterpret_cast<int *>(reinterpret_cast<uint32_t *>(quantities)[1])[i] * amount;
        }
    }
    i++;
check:
    if (i < count) {
        goto loop;
    }

    ItemArray *made = static_cast<ItemArray *>(operator new(sizeof(ItemArray)));
    Array_Item_ctor(made);
    ArrayAdd_Item(item->makeItem(amount), made);
    return ArrayAdd_Array(items, made);
}

ItemArray *Item::extractItems(ItemArray *items, bool station)
{
    if (items == 0) {
        return 0;
    }

    ItemArray *extracted = static_cast<ItemArray *>(operator new(sizeof(ItemArray)));
    Array_Item_ctor(extracted);

    ItemArray *v = items;
    for (uint32_t i = 0; i < v->size(); i++) {
        Item *item = static_cast<Item **>(v->data())[i];
        int amount;
        if (station) {
            amount = item->amount;
        } else {
            amount = item->stationAmount;
        }
        if (amount > 0) {
            ArrayAdd_Item(item->makeItem(), extracted);
        }
    }

    if (extracted->size() == 0) {
        return 0;
    }
    return extracted;
}

__attribute__((minsize))
void Item::combineDuplicates(ItemArray *items)
{
    if (items != 0) {
        ItemArray *v = items;
        uint32_t size = v->size();
        for (uint32_t i = 0; i != size; i++) {
            for (uint32_t j = i + 1; j != size; j++) {
                Item *right = static_cast<Item **>(v->data())[j];
                Item *left = static_cast<Item **>(v->data())[i];
                if (left->index == right->index) {
                    left->amount += right->amount;
                    right->amount = 0;
                    left->stationAmount += right->stationAmount;
                    right->stationAmount = 0;
                }
            }
        }
        for (uint32_t i = 0; i < size; i++) {
            Item *item = static_cast<Item **>(v->data())[i];
            if (item->amount == 0 && item->stationAmount == 0) {
                ArrayRemove_Item(item, items);
                size = v->size();
            }
        }
    }
}

ItemArray *Item::mixItems(ItemArray *items, ItemArray *stationItems)
{
    uint32_t itemCount = items ? items->size() : 0;
    int stationCount = stationItems ? static_cast<int>(stationItems->size()) : 0;

    ItemArray *mixed = static_cast<ItemArray *>(operator new(sizeof(ItemArray)));
    Array_Item_ctor(mixed);
    ArraySetLength_Item(stationCount + itemCount, mixed);

    if (static_cast<int>(itemCount) >= 1 && stationCount == 0) {
        uint32_t i = 0;
        while (i < items->size()) {
            Item *item = static_cast<Item **>(items->data())[i];
            static_cast<Item **>(mixed->data())[i] = item->makeItem(item->amount);
            i++;
        }
    } else if (itemCount == 0 && stationCount > 0) {
        uint32_t i = 0;
        while (i < stationItems->size()) {
            Item *item = static_cast<Item **>(stationItems->data())[i];
            Item *copy = item->makeItem(0);
            static_cast<Item **>(mixed->data())[i] = copy;
            copy->stationAmount = item->amount;
            i++;
        }
    } else if ((stationCount | static_cast<int>(itemCount)) == 0) {
        mixed = 0;
    } else {
        uint32_t i = 0;
        while (i < items->size()) {
            Item *item = static_cast<Item **>(items->data())[i];
            static_cast<Item **>(mixed->data())[i] = item->makeItem(item->amount);
            i++;
        }

        uint32_t stationIndex = 0;
        while (stationIndex < stationItems->size()) {
            uint32_t mixedIndex = 0;
            while (mixedIndex < mixed->size()) {
                Item *mixedItem = static_cast<Item **>(mixed->data())[mixedIndex];
                Item *stationItem = static_cast<Item **>(stationItems->data())[stationIndex];
                if (mixedItem == 0) {
                    Item *copy = stationItem->makeItem(0);
                    itemCount++;
                    static_cast<Item **>(mixed->data())[mixedIndex] = copy;
                    copy->stationAmount = stationItem->amount;
                    break;
                }
                if (stationItem->index == mixedItem->index) {
                    Item *copy = stationItem->makeItem(mixedItem->amount);
                    static_cast<Item **>(mixed->data())[mixedIndex] = copy;
                    copy->stationAmount = stationItem->amount;
                    break;
                }
                mixedIndex++;
            }
            stationIndex++;
        }

        ItemArray *trimmed = static_cast<ItemArray *>(operator new(sizeof(ItemArray)));
        Array_Item_ctor(trimmed);
        ArraySetLength_Item(itemCount, trimmed);

        int copyIndex = 0;
        while (copyIndex < static_cast<int>(itemCount)) {
            static_cast<Item **>(trimmed->data())[copyIndex] = static_cast<Item **>(mixed->data())[copyIndex];
            copyIndex++;
        }

        operator delete(Array_Item_dtor(mixed));

        uint32_t size = trimmed->size();
        bool done = true;
        uint32_t sortIndex = 1;
        do {
            while (sortIndex < size) {
                Item **data = static_cast<Item **>(trimmed->data());
                Item *right = data[sortIndex];
                Item *left = data[sortIndex - 1];
                if (right->index < left->index) {
                    data[sortIndex - 1] = right;
                    done = false;
                    static_cast<Item **>(trimmed->data())[sortIndex] = left;
                }
                sortIndex++;
            }
            bool again = !done;
            sortIndex = 1;
            done = true;
            if (!again) {
                break;
            }
        } while (true);

        Item::combineDuplicates(trimmed);
        mixed = trimmed;
    }

    return mixed;
}
