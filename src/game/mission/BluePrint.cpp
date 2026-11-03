#include "game/mission/BluePrint.h"
#include "game/core/Globals.h"
#include "game/world/Galaxy.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/world/Station.h"
#include "game/core/String.h"

BluePrint::BluePrint(int item) {
    itemIndex = item;
    Item *it = (*Globals::items)[item];
    int type = it->getType();
    batchMultiplier = (type == 1) ? 10 : 1;
    stationIndex = -1;
    Array<int> *quantities = (Array<int> *) it->getQuantities();
    ingredientCounters = nullptr;
    if (it->getIngredients() != nullptr) {
        ingredientCounters = new Array<int>();
        ArraySetLength(it->getIngredients()->size(), *ingredientCounters);
        for (uint32_t i = 0; i < ingredientCounters->size(); i++)
            (*ingredientCounters)[i] = (*quantities)[i];
    }
    locked = 0;
    productionCount = 0;
    spentValue = 0;
    remainingBatch = batchMultiplier;
}

BluePrint::~BluePrint() {
    if (ingredientCounters != nullptr) {
        ArrayRelease(*ingredientCounters);
        delete ingredientCounters;
    }
    ingredientCounters = nullptr;
}

void BluePrint::addItem(Item *item, int amount, int station) {
    if (amount == 0)
        return;
    item->setBlueprintAmount(0);
    Array<int> *il = getIngredientList();
    if (il == nullptr)
        return;
    for (uint32_t i = 0; i < il->size(); i++) {
        if ((*il)[i] == item->getIndex()) {
            (*ingredientCounters)[i] -= amount;
            spentValue += item->getSinglePrice() * amount;
            if (stationIndex < 0 && station >= 0)
                goto set_station;
            return;
        }
    }
    return;
set_station:
    stationIndex = station;
    if (Globals::status->getStation()->getIndex() == station) {
        stationName = Globals::status->getStation()->getName();
    } else {
        Station *st = (Station *) (intptr_t) Globals::galaxy->getStation(station);
        stationName = st->getName();
        if (st != nullptr)
            delete st;
    }
    return;
}

Array<int> *BluePrint::getIngredientList() {
    return (Array<int> *) (*Globals::items)[itemIndex]->getIngredients();
}

void BluePrint::complete() {
    for (uint32_t i = 0; i < ingredientCounters->size(); i++)
        (*ingredientCounters)[i] = 0;
}

bool BluePrint::isCompleted() {
    for (uint32_t i = 0; i < ingredientCounters->size(); i++)
        if ((*ingredientCounters)[i] >= 1)
            return false;
    return true;
}

int BluePrint::getIndex() { return itemIndex; }

int BluePrint::getMoneySpent() { return spentValue; }
void BluePrint::setMoneySpent(int value) { spentValue = value; }

Array<int> *BluePrint::getQuantityList() {
    return (Array<int> *) (*Globals::items)[itemIndex]->getQuantities();
}

int BluePrint::getTotalAmount(int item) {
    Array<int> *il = getIngredientList();
    Array<int> *ql = getQuantityList();
    for (uint32_t i = 0; i < ql->size(); i++) {
        if ((*il)[i] == item)
            return (*ql)[i];
    }
    return 0;
}

int BluePrint::getCurrentAmount(int item) {
    return getTotalAmount(item) - getRemainingAmount(item);
}

int BluePrint::getRemainingAmount(int item) {
    Array<int> *il = getIngredientList();
    for (uint32_t i = 0; i < ingredientCounters->size(); i++) {
        if ((*il)[i] == item)
            return (*ingredientCounters)[i];
    }
    return 0;
}

float BluePrint::getCompletionRate() {
    Array<int> *quantity = getQuantityList();
    float rate = 0.0f;
    for (uint32_t i = 0; i < quantity->size(); i++) {
        int target = (*quantity)[i];
        int remaining = (*ingredientCounters)[i];
        float frac = (float) (target - remaining) / (float) target;
        rate += frac / (float) ingredientCounters->size();
    }
    return rate;
}

int BluePrint::getIngredientsValue() {
    Array<int> *il = getIngredientList();
    if (il == nullptr)
        return 0;
    int total = 0;
    for (uint32_t i = 0; i < il->size(); i++) {
        int price = (*Globals::items)[(*il)[i]]->getSinglePrice();
        total += (*ingredientCounters)[i] * price;
    }
    return total;
}

int BluePrint::getAutoCompletionPrice() {
    if (itemIndex == 0xd2)
        return getIngredientsValue() + 0x1e8480;
    int maxPrice = (*Globals::items)[itemIndex]->getMaxPrice();
    return (int) ((float) (batchMultiplier * maxPrice) * 1.25f);
}

int BluePrint::getBaseQuantity() { return batchMultiplier; }

int BluePrint::getStationIndex() { return stationIndex; }

AbyssEngine::String BluePrint::getStationName() {
    return AbyssEngine::String(stationName, false);
}

int BluePrint::getQuantity() { return remainingBatch; }

bool BluePrint::isUnlocked() { return locked; }

void BluePrint::unlock() {
    locked = 1;
}

void BluePrint::lock() {
    locked = 1;
}

bool BluePrint::isEmpty() {
    return spentValue == 0;
}

void BluePrint::reset() {
    productionCount += 1;
    Globals::status->incGoodsProduced(1);
    Array<int> *ql = getQuantityList();
    for (uint32_t i = 0; i < ingredientCounters->size(); i++)
        (*ingredientCounters)[i] = (*ql)[i];
    stationIndex = -1;
    remainingBatch = batchMultiplier;
    spentValue = 0;
}
