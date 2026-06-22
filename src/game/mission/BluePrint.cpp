#include "game/mission/BluePrint.h"
#include "game/world/Galaxy.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/world/Station.h"
#include "game/core/String.h"



// The item table (database root). Indexed by item id.
extern Array<Item *> *g_items;

// getAutoCompletionPrice():
//   item 0xd2 (raw materials slot) -> ingredient value plus a fixed sentinel bonus;
//   otherwise -> batch * item max price, scaled by 1.25.
int BluePrint::getAutoCompletionPrice() {
    if (itemIndex == 0xd2)
        return getIngredientsValue() + 0x1e8480;
    int maxPrice = (*g_items)[itemIndex]->getMaxPrice();
    return (int)((float)(batchMultiplier * maxPrice) * 1.25f);
}

// Remaining counter for the ingredient with the given index.
int BluePrint::getRemainingAmount(int item) {
    Array<int> *il = getIngredientList();
    for (uint32_t i = 0; i < ingredientCounters->size(); i++) {
        if ((*il)[i] == item)
            return (*ingredientCounters)[i];
    }
    return 0;
}

bool BluePrint::isEmpty() {
    return spentValue == 0;
}

String BluePrint::getStationName() {
    return stationName;
}

BluePrint::~BluePrint() {
    delete ingredientCounters;
    ingredientCounters = nullptr;
}

// Zero every quantity counter in the ingredient array.
void BluePrint::complete() {
    for (uint32_t i = 0; i < ingredientCounters->size(); i++)
        (*ingredientCounters)[i] = 0;
}

// Already-produced amount = total - remaining.
int BluePrint::getCurrentAmount(int item) {
    return getTotalAmount(item) - getRemainingAmount(item);
}

// True once every ingredient counter has dropped below 1.
bool BluePrint::isCompleted() {
    for (uint32_t i = 0; i < ingredientCounters->size(); i++)
        if ((*ingredientCounters)[i] >= 1)
            return false;
    return true;
}

// The produced item's database index.
int BluePrint::getIndex()        { return itemIndex; }
// Station where the blueprint is being built (-1 if unset).
int BluePrint::getStationIndex() { return stationIndex; }
// Remaining batch count.
int BluePrint::getQuantity()     { return remainingBatch; }
// The unlock flag (set to 1 by unlock()).
bool BluePrint::isUnlocked()     { return locked != 0; }

void BluePrint::unlock() {
    locked = 1;
}

// Bump production count, refill the ingredient counters, clear transient state.
void BluePrint::reset() {
    productionCount += 1;
    gStatus->incGoodsProduced(1);
    Array<int> *ql = getQuantityList();
    for (uint32_t i = 0; i < ingredientCounters->size(); i++)
        (*ingredientCounters)[i] = (*ql)[i];
    stationIndex = -1;
    remainingBatch = batchMultiplier;
    spentValue = 0;
}

void BluePrint::lock() {
    locked = 1;
}

// Required total quantity for the given ingredient index.
int BluePrint::getTotalAmount(int item) {
    Array<int> *il = getIngredientList();
    Array<int> *ql = getQuantityList();
    for (uint32_t i = 0; i < ql->size(); i++) {
        if ((*il)[i] == item)
            return (*ql)[i];
    }
    return 0;
}

// The produced item's ingredient list (Item::getIngredients()).
Array<int> *BluePrint::getIngredientList() {
    return (Array<int> *)(*g_items)[itemIndex]->getIngredients();
}

// The produced item's per-ingredient required quantities (Item::getQuantities()).
Array<int> *BluePrint::getQuantityList() {
    return (Array<int> *)(*g_items)[itemIndex]->getQuantities();
}

// Sum over ingredients of (remaining * single price).
int BluePrint::getIngredientsValue() {
    Array<int> *il = getIngredientList();
    int total = 0;
    if (il != nullptr) {
        for (uint32_t i = 0; i < il->size(); i++) {
            int price = (*g_items)[(*il)[i]]->getSinglePrice();
            total += (*ingredientCounters)[i] * price;
        }
    }
    return total;
}

BluePrint::BluePrint(int item) {
    itemIndex = item;
    Item *it = (*g_items)[item];
    int type = it->getType();
    stationIndex = -1;
    batchMultiplier = (type == 1) ? 10 : 1;
    Array<int> *quantities = (Array<int> *)it->getQuantities();
    ingredientCounters = nullptr;
    if (it->getIngredients() != nullptr) {
        ingredientCounters = new Array<int>();
        ingredientCounters->resize(it->getIngredients()->size());
        for (uint32_t i = 0; i < ingredientCounters->size(); i++)
            (*ingredientCounters)[i] = (*quantities)[i];
    }
    productionCount = 0;
    locked = 0;
    spentValue = 0;
    remainingBatch = batchMultiplier;
}

// Base (unscaled) batch quantity for one production cycle.
int BluePrint::getBaseQuantity() { return batchMultiplier; }
// Accumulated spend on this blueprint.
int BluePrint::getMoneySpent() { return spentValue; }
void BluePrint::setMoneySpent(int value) { spentValue = value; }

// addItem(item, amount, station): apply an ingredient delivery to the blueprint,
// decrementing the matching ingredient counter, accumulating spend and recording
// the delivery station on first contribution.
void BluePrint::addItem(Item *item, int amount, int station) {
    if (amount == 0)
        return;
    item->setBlueprintAmount(0);
    Array<int> *il = getIngredientList();
    if (il == nullptr)
        return;
    for (uint32_t i = 0; i < il->size(); i++) {
        if ((*il)[i] != item->getIndex())
            continue;
        (*ingredientCounters)[i] -= amount;
        spentValue += item->getSinglePrice() * amount;
        if (station >= 0 && stationIndex < 0) {
            stationIndex = station;
            Station *current = gStatus->getStation();
            if (current->getIndex() == station) {
                stationName = current->getName();
            } else {
                Station *st = (Station *)(intptr_t)gGalaxy->getStation(station);
                stationName = st->getName();
                if (st != nullptr)
                    delete st;
            }
        }
        break;
    }
}

// Averaged per-ingredient completion fraction. For each ingredient i:
// produced fraction = (target - remaining) / target, averaged across all
// ingredients (divide each term by the ingredient count).
float BluePrint::getCompletionRate() {
    Array<int> *quantity = getQuantityList();
    float rate = 0.0f;
    for (uint32_t i = 0; i < quantity->size(); i++) {
        int target = (*quantity)[i];
        int remaining = (*ingredientCounters)[i];
        float frac = (float)(target - remaining) / (float)target;
        rate += frac / (float)ingredientCounters->size();
    }
    return rate;
}
