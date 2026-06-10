#include "gof2/BluePrint.h"
#include "gof2/Station.h"
#include "gof2/String.h"


extern "C" int Item_getMaxPrice(Item *it);
extern "C" Array<int> *BluePrint_getIngredientList(BluePrint *self);
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void operator_delete(void *p);
extern "C" Array<int> *BluePrint_getQuantityList(BluePrint *self);
extern "C" void Status_incGoodsProduced(void *status, int n);
extern "C" Array<int> *Item_getIngredients(Item *it);
extern "C" int Item_getSinglePrice(Item *it);
extern "C" Array<int> *Item_getQuantities(Item *it);
extern "C" int Item_getType(Item *it);
extern "C" void *operator_new(unsigned int sz);
extern "C" void ArraySetLengthInt(uint32_t n, Array<int> *a);
extern "C" void Item_setBlueprintAmount(Item *it, int amount);
extern "C" int Item_getIndex(Item *it);
extern "C" void *Status_getStation(void);
extern "C" int Station_getIndex(void *station);
extern "C" void *Galaxy_getStation(void *galaxy, int idx);

// ---- getAutoCompletionPrice_1772f0.cpp ----
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getAutoCompletionPrice():
//   item 0xd2 (raw materials slot) -> ingredient value plus a fixed sentinel bonus;
//   otherwise -> batch * item max price, scaled by 1.25.
int BluePrint::getAutoCompletionPrice() {
    BluePrint *self = this;
    int idx = self->field_0x20;
    if (idx == 0xd2)
        return ((BluePrint *)(self))->getIngredientsValue() + 0x1e8480;
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    int maxPrice = Item_getMaxPrice(data[idx]);
    return (int)((float)(self->field_0x24 * maxPrice) * 1.25f);
}

// ---- getRemainingAmount_177208.cpp ----
// BluePrint::getRemainingAmount(int) -> remaining counter for the ingredient with the given index.
int BluePrint::getRemainingAmount(int item) {
    BluePrint *self = this;
    Array<int> *il = BluePrint_getIngredientList(self);
    Array<int> *counters = self->field_0x0;
    for (uint32_t i = 0; i < counters->size(); i++) {
        if (il->data()[i] == item)
            return counters->data()[i];
    }
    return 0;
}

// ---- isEmpty_17736a.cpp ----
bool BluePrint::isEmpty() {
    BluePrint *self = this;
    return self->field_0x4 == 0;
}

// ---- getStationName_177348.cpp ----
// BluePrint::getStationName() -> String by value (sret in r0, this in r1).
AbyssEngine::String12 BluePrint::getStationName() {
    BluePrint *self = this;
    AbyssEngine::String12 r;
    String_copy_ctor(&r, &self->field_0x14, false);
    return r;
}

// ---- _BluePrint_176fdc.cpp ----
extern "C" void ArrayReleaseInt(Array<int> *a);   // ArrayRelease<int>
extern "C" void *ArrayInt_dtor(Array<int> *a);    // Array<int>::~Array
// AbyssEngine::String::~String

// BluePrint::~BluePrint() (D2). Returns this.
extern "C" void *_ZN9BluePrintD2Ev(BluePrint *self)
{
    Array<int> *a = self->field_0x0;
    if (a != 0) {
        ArrayReleaseInt(a);
        Array<int> *a2 = self->field_0x0;
        if (a2 != 0)
            operator_delete(ArrayInt_dtor(a2));
    }
    self->field_0x0 = 0;
    ((String *)(&self->field_0x14))->dtor();
    return self;
}

// ---- complete_17714c.cpp ----
// BluePrint::complete() -> zero every quantity counter in the ingredient array at +0x00.
void BluePrint::complete() {
    BluePrint *self = this;
    Array<int> *a = self->field_0x0;
    for (uint32_t i = 0; i < a->size(); i++)
        a->data()[i] = 0;
}

// ---- getCurrentAmount_1771e6.cpp ----
// BluePrint::getCurrentAmount(int) -> already-produced amount = total - remaining.
int BluePrint::getCurrentAmount(int item) {
    BluePrint *self = this;
    int total = ((BluePrint *)(self))->getTotalAmount(item);
    int remaining = ((BluePrint *)(self))->getRemainingAmount(item);
    return total - remaining;
}

// ---- isCompleted_177164.cpp ----
// BluePrint::isCompleted() -> true once every ingredient counter has dropped below 1.
bool BluePrint::isCompleted() {
    BluePrint *self = this;
    Array<int> *a = self->field_0x0;
    for (uint32_t i = 0; i < a->size(); i++)
        if (a->data()[i] >= 1)
            return false;
    return true;
}

// ---- unlock_17735e.cpp ----
void BluePrint::unlock() {
    BluePrint *self = this;
    self->field_0x8 = 1;
}

// ---- reset_177374.cpp ----
// Hidden PC-relative pointer-to-the-global-Status pointer.
extern void *const gStatusPtr __attribute__((visibility("hidden")));

// BluePrint::reset() -> bump production count, refill the ingredient counters, clear state.
void BluePrint::reset() {
    BluePrint *self = this;
    self->field_0xc += 1;
    Status_incGoodsProduced(*(void **)gStatusPtr, 1);
    Array<int> *ql = BluePrint_getQuantityList(self);
    Array<int> *counters = self->field_0x0;
    for (uint32_t i = 0; i < counters->size(); i++)
        counters->data()[i] = ql->data()[i];
    self->field_0x10 = -1;
    self->field_0x28 = self->field_0x24;
    self->field_0x4 = 0;
}

// ---- lock_177364.cpp ----
void BluePrint::lock() {
    BluePrint *self = this;
    self->field_0x8 = 1;
}

// ---- getTotalAmount_1771a8.cpp ----
// BluePrint::getTotalAmount(int) -> required total quantity for the given ingredient index.
int BluePrint::getTotalAmount(int item) {
    BluePrint *self = this;
    Array<int> *il = BluePrint_getIngredientList(self);
    Array<int> *ql = BluePrint_getQuantityList(self);
    for (uint32_t i = 0; i < ql->size(); i++) {
        if (il->data()[i] == item)
            return ql->data()[i];
    }
    return 0;
}

// ---- getIngredientList_177134.cpp ----
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getIngredientList() -> tail-call Item::getIngredients(itemDB[index]).
extern "C" Array<int> *BluePrint_getIngredientList(BluePrint *self)
{
    int idx = self->field_0x20;
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    return Item_getIngredients(data[idx]);
}

// ---- getIngredientsValue_177298.cpp ----
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getIngredientsValue() -> sum over ingredients of (remaining * single price).
int BluePrint::getIngredientsValue() {
    BluePrint *self = this;
    Array<int> *il = BluePrint_getIngredientList(self);
    int total = 0;
    if (il != 0) {
        for (uint32_t i = 0; i < il->size(); i++) {
            Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
            int price = Item_getSinglePrice(data[il->data()[i]]);
            total += self->field_0x0->data()[i] * price;
        }
    }
    return total;
}

// ---- getQuantityList_177190.cpp ----
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getQuantityList() -> tail-call Item::getQuantities(itemDB[index]).
extern "C" Array<int> *BluePrint_getQuantityList(BluePrint *self)
{
    int idx = self->field_0x20;
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    return Item_getQuantities(data[idx]);
}

// ---- BluePrint_176f20.cpp ----
// AbyssEngine::String::String()
extern "C" void ArrayInt_ctor(void *a);                // Array<int>::Array()
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::BluePrint(int item)
extern "C" BluePrint *_ZN9BluePrintC2Ei(BluePrint *self, int item)
{
    ((String *)(&self->field_0x14))->ctor();
    self->field_0x20 = item;
    Item **db = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    Item *it = db[item];
    int type = Item_getType(it);
    self->field_0x10 = -1;
    self->field_0x24 = (type == 1) ? 10 : 1;
    Array<int> *quantities = Item_getQuantities(it);
    self->field_0x0 = 0;
    if (Item_getIngredients(it) != 0) {
        self->field_0x0 = new Array<int>();
        ArraySetLengthInt(Item_getIngredients(it)->size(), self->field_0x0);
        Array<int> *arr = self->field_0x0;
        for (uint32_t i = 0; i < arr->size(); i++)
            arr->data()[i] = quantities->data()[i];
    }
    self->field_0xc = 0;
    self->field_0x8 = 0;
    self->field_0x4 = 0;
    self->field_0x28 = self->field_0x24;
    return self;
}

// ---- addItem_177008.cpp ----
// AbyssEngine::String::operator=
// Hidden PC-relative pointer-to-the-global-Galaxy pointer.
extern void *const gGalaxyPtr __attribute__((visibility("hidden")));

// BluePrint::addItem(Item *item, int amount, int station)
void BluePrint::addItem(Item *item, int amount, int station) {
    BluePrint *self = this;
    if (amount != 0) {
        Item_setBlueprintAmount(item, 0);
        Array<int> *il = BluePrint_getIngredientList(self);
        if (il != 0) {
            for (uint32_t i = 0; i < il->size(); i++) {
                if (il->data()[i] == Item_getIndex(item)) {
                    self->field_0x0->data()[i] -= amount;
                    self->field_0x4 += Item_getSinglePrice(item) * amount;
                    if (station >= 0 && self->field_0x10 < 0) {
                        self->field_0x10 = station;
                        if (Station_getIndex(Status_getStation()) == station) {
                            char tmp[12];
                            *(RetStr *)tmp = ((Station *)Status_getStation())->getName();
                            ((String *)(&self->field_0x14))->assign((String *)tmp);
                            ((String *)(tmp))->dtor();
                        } else {
                            void *st = Galaxy_getStation(*(void **)gGalaxyPtr, station);
                            char tmp[12];
                            *(RetStr *)tmp = ((Station *)(st))->getName();
                            ((String *)(&self->field_0x14))->assign((String *)tmp);
                            ((String *)(tmp))->dtor();
                            if (st != 0) {
                                ((Station *)(st))->dtor();
                                operator_delete(st);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

// ---- getCompletionRate_177238.cpp ----
// BluePrint::getCompletionRate() -> averaged per-ingredient completion fraction.
// For each ingredient i: produced fraction = (target - remaining) / target,
// then averaged across all ingredients (divide each term by the ingredient count).
float BluePrint::getCompletionRate() {
    BluePrint *self = this;
    Array<int> *quantity = BluePrint_getQuantityList(self);
    Array<int> *counters = self->field_0x0;
    float rate = 0.0f;
    for (uint32_t i = 0; i < quantity->size(); i++) {
        int target = quantity->data()[i];
        int remaining = counters->data()[i];
        float frac = (float)(target - remaining) / (float)target;
        rate += frac / (float)counters->size();
    }
    return rate;
}
