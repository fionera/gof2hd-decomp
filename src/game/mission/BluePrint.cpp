#include "gof2/game/mission/BluePrint.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/world/Station.h"
#include "gof2/game/core/String.h"

Array<int> *BluePrint_getIngredientList(BluePrint *self);
Array<int> *BluePrint_getQuantityList(BluePrint *self);
BluePrint *_ZN9BluePrintC2Ei(BluePrint *self, int item);   // BluePrint::BluePrint(int)
extern "C" int Station_getIndex(void *station);

// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getAutoCompletionPrice():
//   item 0xd2 (raw materials slot) -> ingredient value plus a fixed sentinel bonus;
//   otherwise -> batch * item max price, scaled by 1.25.
int BluePrint::getAutoCompletionPrice() {
    int idx = this->itemIndex;
    if (idx == 0xd2)
        return ((BluePrint *)(this))->getIngredientsValue() + 0x1e8480;
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    int maxPrice = ((Item *)(data[idx]))->getMaxPrice();
    return (int)((float)(this->batchMultiplier * maxPrice) * 1.25f);
}

// BluePrint::getRemainingAmount(int) -> remaining counter for the ingredient with the given index.
int BluePrint::getRemainingAmount(int item) {
    Array<int> *il = BluePrint_getIngredientList(this);
    Array<int> *counters = this->ingredientCounters;
    for (uint32_t i = 0; i < counters->size(); i++) {
        if (il->data()[i] == item)
            return counters->data()[i];
    }
    return 0;
}

bool BluePrint::isEmpty() {
    return this->spentValue == 0;
}

// BluePrint::getStationName() -> String by value (sret in r0, this in r1).
AbyssEngine::String12 BluePrint::getStationName() {
    BluePrint *self = this;
    AbyssEngine::String12 r;
    ((String *)(&r))->ctor_copy((String *)(&self->stationName), false);
    return r;
}

// AbyssEngine::String::~String

// BluePrint::~BluePrint() (D2). Returns this.
void *_ZN9BluePrintD2Ev(BluePrint *self)
{
    delete self->ingredientCounters;
    self->ingredientCounters = 0;
    ((String *)(&self->stationName))->dtor();
    return self;
}

// BluePrint::complete() -> zero every quantity counter in the ingredient array at +0x00.
void BluePrint::complete() {
    Array<int> *a = this->ingredientCounters;
    for (uint32_t i = 0; i < a->size(); i++)
        a->data()[i] = 0;
}

// BluePrint::getCurrentAmount(int) -> already-produced amount = total - remaining.
int BluePrint::getCurrentAmount(int item) {
    int total = ((BluePrint *)(this))->getTotalAmount(item);
    int remaining = ((BluePrint *)(this))->getRemainingAmount(item);
    return total - remaining;
}

// BluePrint::isCompleted() -> true once every ingredient counter has dropped below 1.
bool BluePrint::isCompleted() {
    Array<int> *a = this->ingredientCounters;
    for (uint32_t i = 0; i < a->size(); i++)
        if (a->data()[i] >= 1)
            return false;
    return true;
}

// ---- simple field accessors (187184..18735a) ----
// BluePrint::getIndex() -> the produced item's database index (+0x20).
int BluePrint::getIndex()        { return itemIndex; }
// BluePrint::getStationIndex() -> station where the blueprint is being built (+0x10).
int BluePrint::getStationIndex() { return stationIndex; }
// BluePrint::getQuantity() -> remaining batch count (+0x28).
int BluePrint::getQuantity()     { return remainingBatch; }
// BluePrint::isUnlocked() -> the unlock flag at +0x8 (set to 1 by unlock()).
bool BluePrint::isUnlocked()     { return locked != 0; }

void BluePrint::unlock() {
    this->locked = 1;
}

// Hidden PC-relative pointer-to-the-global-Status pointer.
extern void *const gStatusPtr __attribute__((visibility("hidden")));

// BluePrint::reset() -> bump production count, refill the ingredient counters, clear state.
void BluePrint::reset() {
    this->productionCount += 1;
    ((Status *)(*(void **)gStatusPtr))->incGoodsProduced(1);
    Array<int> *ql = BluePrint_getQuantityList(this);
    Array<int> *counters = this->ingredientCounters;
    for (uint32_t i = 0; i < counters->size(); i++)
        counters->data()[i] = ql->data()[i];
    this->stationIndex = -1;
    this->remainingBatch = this->batchMultiplier;
    this->spentValue = 0;
}

void BluePrint::lock() {
    this->locked = 1;
}

// BluePrint::getTotalAmount(int) -> required total quantity for the given ingredient index.
int BluePrint::getTotalAmount(int item) {
    Array<int> *il = BluePrint_getIngredientList(this);
    Array<int> *ql = BluePrint_getQuantityList(this);
    for (uint32_t i = 0; i < ql->size(); i++) {
        if (il->data()[i] == item)
            return ql->data()[i];
    }
    return 0;
}

// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getIngredientList() -> tail-call Item::getIngredients(itemDB[index]).
Array<int> *BluePrint_getIngredientList(BluePrint *self)
{
    int idx = self->itemIndex;
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    return (Array<int> *)((Item *)(data[idx]))->getIngredients();
}

// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getIngredientsValue() -> sum over ingredients of (remaining * single price).
int BluePrint::getIngredientsValue() {
    Array<int> *il = BluePrint_getIngredientList(this);
    int total = 0;
    if (il != 0) {
        for (uint32_t i = 0; i < il->size(); i++) {
            Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
            int price = ((Item *)(data[il->data()[i]]))->getSinglePrice();
            total += this->ingredientCounters->data()[i] * price;
        }
    }
    return total;
}

// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getQuantityList() -> tail-call Item::getQuantities(itemDB[index]).
Array<int> *BluePrint_getQuantityList(BluePrint *self)
{
    int idx = self->itemIndex;
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    return (Array<int> *)((Item *)(data[idx]))->getQuantities();
}

// AbyssEngine::String::String()
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::BluePrint(int item)
BluePrint *_ZN9BluePrintC2Ei(BluePrint *self, int item)
{
    ((String *)(&self->stationName))->ctor();
    self->itemIndex = item;
    Item **db = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    Item *it = db[item];
    int type = ((Item *)(it))->getType();
    self->stationIndex = -1;
    self->batchMultiplier = (type == 1) ? 10 : 1;
    Array<int> *quantities = (Array<int> *)((Item *)(it))->getQuantities();
    self->ingredientCounters = 0;
    if (((Item *)(it))->getIngredients() != 0) {
        self->ingredientCounters = new Array<int>();
        self->ingredientCounters->resize(((Item *)(it))->getIngredients()->size());
        Array<int> *arr = self->ingredientCounters;
        for (uint32_t i = 0; i < arr->size(); i++)
            arr->data()[i] = quantities->data()[i];
    }
    self->productionCount = 0;
    self->locked = 0;
    self->spentValue = 0;
    self->remainingBatch = self->batchMultiplier;
    return self;
}

// ---- getIndexOf / make ----
// BluePrint::getIndexOf() — index accessor reached via the ListItem/record-handler
// wrapper path; returns the produced item's database index, same as getIndex().
int BluePrint::getIndexOf() { return itemIndex; }

// BluePrint::make(int) — heap factory: operator new + BluePrint(index).
BluePrint *BluePrint::make(int index) {
    return _ZN9BluePrintC2Ei((BluePrint *)::operator new(sizeof(BluePrint)), index);
}

// AbyssEngine::String::operator=
// Hidden PC-relative pointer-to-the-global-Galaxy pointer.
extern void *const gGalaxyPtr __attribute__((visibility("hidden")));

// BluePrint::addItem(Item *item, int amount, int station)
void BluePrint::addItem(Item *item, int amount, int station) {
    BluePrint *self = this;
    if (amount != 0) {
        ((Item *)(item))->setBlueprintAmount(0);
        Array<int> *il = BluePrint_getIngredientList(self);
        if (il != 0) {
            for (uint32_t i = 0; i < il->size(); i++) {
                if (il->data()[i] == ((Item *)(item))->getIndex()) {
                    self->ingredientCounters->data()[i] -= amount;
                    self->spentValue += ((Item *)(item))->getSinglePrice() * amount;
                    if (station >= 0 && self->stationIndex < 0) {
                        self->stationIndex = station;
                        if (Station_getIndex(((Status *)(*(void **)gStatusPtr))->getStation()) == station) {
                            char tmp[12];
                            *(RetStr *)tmp = ((Station *)(((Status *)(*(void **)gStatusPtr))->getStation()))->getName();
                            ((String *)(&self->stationName))->assign((String *)tmp);
                            ((String *)(tmp))->dtor();
                        } else {
                            void *st = (void *)(intptr_t)((Galaxy *)(*(void **)gGalaxyPtr))->getStation(station);
                            char tmp[12];
                            *(RetStr *)tmp = ((Station *)(st))->getName();
                            ((String *)(&self->stationName))->assign((String *)tmp);
                            ((String *)(tmp))->dtor();
                            if (st != 0) {
                                ((Station *)(st))->dtor();
                                ::operator delete(st);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

// BluePrint::getCompletionRate() -> averaged per-ingredient completion fraction.
// For each ingredient i: produced fraction = (target - remaining) / target,
// then averaged across all ingredients (divide each term by the ingredient count).
float BluePrint::getCompletionRate() {
    Array<int> *quantity = BluePrint_getQuantityList(this);
    Array<int> *counters = this->ingredientCounters;
    float rate = 0.0f;
    for (uint32_t i = 0; i < quantity->size(); i++) {
        int target = quantity->data()[i];
        int remaining = counters->data()[i];
        float frac = (float)(target - remaining) / (float)target;
        rate += frac / (float)counters->size();
    }
    return rate;
}
