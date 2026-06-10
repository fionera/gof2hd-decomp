#include "gof2/Item.h"


extern "C" void Array_Item_ctor(Array *array);
extern "C" void ArraySetLength_Item(uint32_t length, Array *array);
extern "C" void ArrayAdd_Item(Item *item, Array *array);
extern "C" void ArrayAdd_Array(Array *items, Array *array);
extern "C" void ArrayRemove_Item(Item *item, Array *items);
extern "C" void *Array_Item_dtor(Array *array);
extern "C" void ArraySetLength_Item(uint32_t size, Array *array);

// ---- getAttribute_e01b4.cpp ----
__attribute__((minsize))
int Item::getAttribute(int attribute)
{
    volatile Array *v = attributes;
    int result = (int)0xc5997825;
    uint32_t size = v->size;
    for (uint32_t index = 0; index < size; index += 2) {
        int *data = (int *)v->data;
        if (data[index] == attribute) {
            return data[index + 1];
        }
    }
    return result;
}

// ---- getQuantities_e01ac.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

Array *Item::getQuantities() {
    return quantities;
}

// ---- getMissingIngredients_e0180.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getMissingIngredients() {
    return missingIngredients;
}

// ---- adjustPrice_e0848.cpp ----
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

// ---- getTecLevel_e0118.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getTecLevel() {
    return tecLevel;
}

// ---- setUnsaleable_e00fa.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setUnsaleable(bool value)
{
    unsaleable = value;
}

// ---- setMaxPrice_e0144.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setMaxPrice(int value) {
    maxPrice = value;
}

// ---- checkCredits_e0824.cpp ----
struct Status {
    int getCredits();
};

extern Status *status;

__attribute__((minsize)) bool Item::checkCredits()
{
    int credits = status->getCredits();
    return credits >= price;
}

// ---- getOccurence_e0170.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getOccurence() {
    return occurence;
}

// ---- setMissingIngredients_e0184.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setMissingIngredients(int value) {
    missingIngredients = value;
}

// ---- makeItem_e0338.cpp ----
#define wchar_t gof2_wchar_t

Item *Item::makeItem(int amount)
{
    int savedPrice = price;
    Item *item = clone();
    item->price = savedPrice;
    item->amount = amount;
    return item;
}

// ---- canBeInstalledMultipleTimes_e0100.cpp ----
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

// ---- transaction_e01e8.cpp ----
struct Status {
    int getCredits();
};

extern Status *status;

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

// ---- setStationAmount_e0188.cpp ----
void Item::setStationAmount(int value) {
    stationAmount = value;
}

// ---- getIngredients_e01a8.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

Array *Item::getIngredients() {
    return ingredients;
}

// ---- getMaxPrice_e012c.cpp ----
int Item::getMaxPrice() {
    return maxPrice;
}

// ---- getType_e0114.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getType() {
    return type;
}

// ---- setAmount_e016c.cpp ----
void Item::setAmount(int value) {
    amount = value;
}

// ---- isInList_e02a8.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

bool Item::isInList(Item *item, Array *items)
{
    return isInList(item->index, items);
}

// ---- changeBlueprintAmount_e01a0.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::changeBlueprintAmount(int delta) {
    blueprintAmount += delta;
}

// ---- setMinPrice_e0140.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setMinPrice(int value) {
    minPrice = value;
}

// ---- getMinPriceSystem_e0138.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getMinPriceSystem()
{
    return minPriceSystem;
}

// ---- getStationAmount_e018c.cpp ----
int Item::getStationAmount()
{
    return stationAmount;
}

// ---- getTotalPrice_e0124.cpp ----
int Item::getTotalPrice() {
    return price * amount;
}

// ---- changeAmount_e0178.cpp ----
void Item::changeAmount(int delta)
{
    amount += delta;
}

// ---- makeItem_e07c6.cpp ----
Item *Item::makeItem(int amount, int price)
{
    Item *item = clone();
    item->price = price;
    item->amount = amount;
    return item;
}

// ---- _Item_e00f8.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

Item::~Item() {}

// ---- transactionBlueprint_e023c.cpp ----
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

// ---- setPrice_e013c.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setPrice(int value) {
    price = value;
}

// ---- getSort_e011c.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getSort() {
    return sort;
}

// ---- getBlueprintAmount_e019c.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getBlueprintAmount()
{
    return blueprintAmount;
}

// ---- makeItem_e0810.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

Item *Item::makeItem() {
    int savedPrice = price;
    Item *item = clone();
    item->price = savedPrice;
    item->amount = 1;
    return item;
}

// ---- getSinglePrice_e0120.cpp ----
int Item::getSinglePrice() {
    return price;
}

// ---- getIndex_e0110.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getIndex() {
    return index;
}

// ---- getPriceRate_e0148.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

float Item::getPriceRate() {
    return static_cast<float>(price - minPrice) /
           static_cast<float>(maxPrice - minPrice);
}

// ---- isInList_e026e.cpp ----
__attribute__((minsize))
bool Item::isInList(int index, int amount, Array *items)
{
    if (items != 0) {
        volatile Array *v = items;
        uint32_t size = v->size;
        for (uint32_t i = 0; i < size; i++) {
            Item *item = static_cast<Item **>(v->data)[i];
            if (item->index == index && amount <= item->amount) {
                return true;
            }
        }
    }
    return false;
}

// ---- isInList_e02a0.cpp ----
bool Item::isInList(int index, Array *items) {
    return isInList(index, 1, items);
}

// ---- equals_e07a6.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

__attribute__((minsize)) bool Item::equals(Item *other)
{
    if (other != 0) {
        return index == other->index;
    }
    return false;
}

// ---- clone_e07d8.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void *operator new(__SIZE_TYPE__ size);

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

// ---- getAttributes_e01b0.cpp ----
Array *Item::getAttributes() {
    return attributes;
}

// ---- getMinPrice_e0130.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getMinPrice() {
    return minPrice;
}

// ---- init_e00a6.cpp ----
__attribute__((minsize)) void Item::init()
{
    if (attributes == 0) {
        return;
    }

    int *data = static_cast<int *>(attributes->data);

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

// ---- isUnsaleable_e08c4.cpp ----
#define wchar_t gof2_wchar_t

bool Item::isUnsaleable()
{
    return unsaleable;
}

// ---- Item_e0090.cpp ----
__attribute__((minsize)) Item::Item(Array *ingredients_, Array *quantities_, Array *attributes_)
    : ingredients(ingredients_), quantities(quantities_), attributes(attributes_)
{
    init();
}

// ---- changeStationAmount_e0190.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::changeStationAmount(int delta) {
    stationAmount += delta;
}

// ---- getAmount_e0174.cpp ----
int Item::getAmount() {
    return amount;
}

// ---- getMaxPriceSystem_e0134.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

int Item::getMaxPriceSystem() {
    return maxPriceSystem;
}

// ---- setBlueprintAmount_e0198.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

void Item::setBlueprintAmount(int value) {
    blueprintAmount = value;
}

// ---- checkCargoSpace_e088c.cpp ----
struct Status {
    Ship *getShip();
};

struct Ship {
    int getCurrentLoad();
    int getMaxLoad();
};

extern Status *status;

__attribute__((minsize)) bool Item::checkCargoSpace()
{
    int currentLoad = status->getShip()->getCurrentLoad();
    int requiredLoad = amount + currentLoad;
    int maxLoad = status->getShip()->getMaxLoad();
    return requiredLoad <= maxLoad;
}

// ---- isWeapon_e07ba.cpp ----
#define wchar_t gof2_wchar_t
#undef wchar_t

bool Item::isWeapon()
{
    return static_cast<uint32_t>(type) < 3;
}

// ---- combineItems_e034a.cpp ----
Array *Item::combineItems(Array *items, Array *stationItems)
{
    Array *result = stationItems;
    if (items != 0) {
        result = items;
        if (stationItems != 0) {
            Array *stationCopy = static_cast<Array *>(operator new(sizeof(Array)));
            Array_Item_ctor(stationCopy);
            ArraySetLength_Item(stationItems->size, stationCopy);

            uint32_t stationCount = stationItems->size;
            for (uint32_t i = 0; stationCount != i; i++) {
                static_cast<Item **>(stationCopy->data)[i] =
                    static_cast<Item **>(stationItems->data)[i];
            }

            uint32_t copyCount = stationCopy->size;
            uint32_t remaining = copyCount;
            uint32_t itemCount = items->size;
            for (uint32_t itemIndex = 0; itemCount != itemIndex; itemIndex++) {
                for (uint32_t stationIndex = 0; copyCount != stationIndex; stationIndex++) {
                    Item **copyData = static_cast<Item **>(stationCopy->data);
                    Item *stationItem = copyData[stationIndex];
                    if (stationItem != 0) {
                        Item *item = static_cast<Item **>(items->data)[itemIndex];
                        if (item->index == stationItem->index) {
                            remaining--;
                            item->amount = stationItem->amount + item->amount;
                            copyData[stationIndex] = 0;
                        }
                    }
                }
            }

            if (static_cast<int>(remaining) > 0) {
                Array *unmatched = static_cast<Array *>(operator new(sizeof(Array)));
                Array_Item_ctor(unmatched);
                ArraySetLength_Item(remaining, unmatched);

                uint32_t totalCopied = stationCopy->size;
                uint32_t unmatchedIndex = 0;
                for (uint32_t i = 0; totalCopied != i; i++) {
                    Item *item = static_cast<Item **>(stationCopy->data)[i];
                    if (item != 0) {
                        static_cast<Item **>(unmatched->data)[unmatchedIndex] = item;
                        unmatchedIndex++;
                    }
                }

                result = static_cast<Array *>(operator new(sizeof(Array)));
                Array_Item_ctor(result);
                ArraySetLength_Item(items->size + unmatched->size, result);

                uint32_t itemCountForCopy = items->size;
                for (uint32_t i = 0; itemCountForCopy != i; i++) {
                    static_cast<Item **>(result->data)[i] =
                        static_cast<Item **>(items->data)[i];
                }

                uint32_t unmatchedCount = unmatched->size;
                for (uint32_t i = 0; unmatchedCount != i; i++) {
                    static_cast<Item **>(result->data)[itemCountForCopy + i] =
                        static_cast<Item **>(unmatched->data)[i];
                }
            }
        }
    }
    return result;
}

// ---- fabricate_e02ae.cpp ----
void *operator new(__SIZE_TYPE__ size);


__attribute__((minsize)) void Item::fabricate(Item *item, Array *items, int amount)
{
    volatile uint32_t *ingredients = reinterpret_cast<volatile uint32_t *>(item->ingredients);
    Array *quantities = item->quantities;
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

    Array *made = static_cast<Array *>(operator new(sizeof(Array)));
    Array_Item_ctor(made);
    ArrayAdd_Item(item->makeItem(amount), made);
    return ArrayAdd_Array(items, made);
}

// ---- extractItems_e049c.cpp ----
void *operator new(__SIZE_TYPE__ size);

Array *Item::extractItems(Array *items, bool station)
{
    if (items == 0) {
        return 0;
    }

    Array *extracted = static_cast<Array *>(operator new(sizeof(Array)));
    Array_Item_ctor(extracted);

    volatile Array *v = items;
    for (uint32_t i = 0; i < v->size; i++) {
        Item *item = static_cast<Item **>(v->data)[i];
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

    if (extracted->size == 0) {
        return 0;
    }
    return extracted;
}

// ---- combineDuplicates_e0504.cpp ----
__attribute__((minsize))
void Item::combineDuplicates(Array *items)
{
    if (items != 0) {
        volatile Array *v = items;
        uint32_t size = v->size;
        for (uint32_t i = 0; i != size; i++) {
            for (uint32_t j = i + 1; j != size; j++) {
                Item *right = static_cast<Item **>(v->data)[j];
                Item *left = static_cast<Item **>(v->data)[i];
                if (left->index == right->index) {
                    left->amount += right->amount;
                    right->amount = 0;
                    left->stationAmount += right->stationAmount;
                    right->stationAmount = 0;
                }
            }
        }
        for (uint32_t i = 0; i < size; i++) {
            Item *item = static_cast<Item **>(v->data)[i];
            if (item->amount == 0 && item->stationAmount == 0) {
                ArrayRemove_Item(item, items);
                size = v->size;
            }
        }
    }
}

// ---- mixItems_e05ca.cpp ----
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;


Array *Item::mixItems(Array *items, Array *stationItems)
{
    uint32_t itemCount = items ? items->size : 0;
    int stationCount = stationItems ? static_cast<int>(stationItems->size) : 0;

    Array *mixed = static_cast<Array *>(operator new(sizeof(Array)));
    Array_Item_ctor(mixed);
    ArraySetLength_Item(stationCount + itemCount, mixed);

    if (static_cast<int>(itemCount) >= 1 && stationCount == 0) {
        uint32_t i = 0;
        while (i < items->size) {
            Item *item = static_cast<Item **>(items->data)[i];
            static_cast<Item **>(mixed->data)[i] = item->makeItem(item->amount);
            i++;
        }
    } else if (itemCount == 0 && stationCount > 0) {
        uint32_t i = 0;
        while (i < stationItems->size) {
            Item *item = static_cast<Item **>(stationItems->data)[i];
            Item *copy = item->makeItem(0);
            static_cast<Item **>(mixed->data)[i] = copy;
            copy->stationAmount = item->amount;
            i++;
        }
    } else if ((stationCount | static_cast<int>(itemCount)) == 0) {
        mixed = 0;
    } else {
        uint32_t i = 0;
        while (i < items->size) {
            Item *item = static_cast<Item **>(items->data)[i];
            static_cast<Item **>(mixed->data)[i] = item->makeItem(item->amount);
            i++;
        }

        uint32_t stationIndex = 0;
        while (stationIndex < stationItems->size) {
            uint32_t mixedIndex = 0;
            while (mixedIndex < mixed->size) {
                Item *mixedItem = static_cast<Item **>(mixed->data)[mixedIndex];
                Item *stationItem = static_cast<Item **>(stationItems->data)[stationIndex];
                if (mixedItem == 0) {
                    Item *copy = stationItem->makeItem(0);
                    itemCount++;
                    static_cast<Item **>(mixed->data)[mixedIndex] = copy;
                    copy->stationAmount = stationItem->amount;
                    break;
                }
                if (stationItem->index == mixedItem->index) {
                    Item *copy = stationItem->makeItem(mixedItem->amount);
                    static_cast<Item **>(mixed->data)[mixedIndex] = copy;
                    copy->stationAmount = stationItem->amount;
                    break;
                }
                mixedIndex++;
            }
            stationIndex++;
        }

        Array *trimmed = static_cast<Array *>(operator new(sizeof(Array)));
        Array_Item_ctor(trimmed);
        ArraySetLength_Item(itemCount, trimmed);

        int copyIndex = 0;
        while (copyIndex < static_cast<int>(itemCount)) {
            static_cast<Item **>(trimmed->data)[copyIndex] = static_cast<Item **>(mixed->data)[copyIndex];
            copyIndex++;
        }

        operator delete(Array_Item_dtor(mixed));

        uint32_t size = trimmed->size;
        bool done = true;
        uint32_t sortIndex = 1;
        do {
            while (sortIndex < size) {
                Item **data = static_cast<Item **>(trimmed->data);
                Item *right = data[sortIndex];
                Item *left = data[sortIndex - 1];
                if (right->index < left->index) {
                    data[sortIndex - 1] = right;
                    done = false;
                    static_cast<Item **>(trimmed->data)[sortIndex] = left;
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
