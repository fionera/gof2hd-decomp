#include "gof2/game/ship/Ship.h"
#include "gof2/game/world/Standing.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/world/SolarSystem.h"

extern "C" void *ItemDtor(Item *it);
extern "C" void operatorDelete(void *p);
Ship* clone(Ship *self);
extern "C" void *ArrayItemDtor(Array<Item*> *a);
extern "C" void Array_Item_ctor(Array<Item*> *a);
extern "C" int *operatorNewArrayInt(unsigned int n);
void refreshValue(Ship *self);
extern "C" void ArrayReleaseClassesItem(Array<Item*> *a);
extern "C" void *ArrayIntDtor(Array<int> *a);
int getUsedSlots(Ship *self, int type);
int hasCargo(Ship *self, int type, int amount);
extern "C" Array<Item*>* operatorNewArr(unsigned int sz);
extern "C" Ship *operatorNewShip(unsigned int sz);
void addMod(Ship *self, int mod);
extern "C" Array<int>* operatorNewModsArr(unsigned int sz);
extern "C" void Array_int_ctor(Array<int> *a);
extern "C" Item* operatorNewItem(unsigned int sz);
extern "C" void Item_ctor3(Item *it, void *a, void *b, void *c);
int getCargoValue(Ship *self);
void setCargo(Ship *self, Array<Item*> *cargo);
extern "C" Array<Item*>* operatorNewArray(unsigned int sz);
extern "C" void ArrayCtorItem(Array<Item*> *a);

// Ship::removeCargo(int) -> tail-calls Ship::removeCargo(int,int) with amount = 9999999
void removeCargo(Ship *self, int idx) {
    ((Ship *)(self))->removeCargo3(idx, 9999999);
}

int getRace(Ship *self) { return self->race; }

bool hasBooster(Ship *self) { return 0 < self->boostSpeed; }

int getCompression(Ship *self) { return self->compression; }

int getBaseLoad(Ship *self) { return self->baseLoad; }

void setCurrentWeaponSlot(Ship *self, int v) { self->currentWeaponSlot = v; }

int getNumAddedDeviceSlots(Ship *self) { return self->numAddedDeviceSlots; }

int getMaxPassengers(Ship *self) { return self->maxPassengers; }

int getBoostTime(Ship *self) { return self->boostTime; }

void setEquipment(Ship *self, Item *item, int slot) {
    unsigned int idx = slot;
    for (int i = 0; i < ((Item *)(item))->getType(); i = i + 1) {
        idx = idx + self->slots[i];
    }
    if (idx >= self->equipment->size()) {
        return;
    }
    Item **dst;
    Item *old = self->equipment->data()[idx];
    if (old == 0) {
        dst = &self->equipment->data()[idx];
    } else {
        operatorDelete(ItemDtor(old));
        dst = &self->equipment->data()[idx];
    }
    *dst = 0;
    self->equipment->data()[idx] = item;
    return ((Ship *)(self))->recomputeAfterSlots();
}

void makeShip(Ship *self, int v) {
    Ship *s = clone(self);
    if (-1 < v) s->price = v;
}

// Ship::removeCargo(Item*) -> tail-call removeCargo(self, item->getIndex(), item->getAmount())
void removeCargo(Ship *self, Item *item) {
    int idx = ((Item *)(item))->getIndex();
    int amt = ((Item *)(item))->getAmount();
    ((Ship *)(self))->removeCargo3(idx, amt);
}

void removeAllCargo(Ship *self) {
    if (self->cargo != 0) {
        void *p = ArrayItemDtor(self->cargo);
        operatorDelete(p);
    }
    self->cargo = 0;
}

int getMaxLoad(Ship *self) { return self->cargoPlus + self->baseLoad; }

void freeSlot(Ship *self, Item *item) {
    unsigned int i = 0;
    while (i < self->equipment->size()) {
        Item *it = self->equipment->data()[i];
        if (it != 0 && it->equals(item) != 0) {
            self->equipment->data()[i] = 0;
            break;
        }
        i = i + 1;
    }
    return ((Ship *)(self))->recomputeAfterSlots();
}

int getMaxArmorHP(Ship *self) { return self->maxArmorHP; }

int getCurrentWeaponSlot(Ship *self) { return self->currentWeaponSlot; }

void setRace(Ship *self, int v) { self->race = v; }

Ship *_ZN4ShipC2Eiiiiiiiif(Ship *self, int index, int baseHP, int baseLoad,
                                       int value, int s0, int s1, int s2, int s3, float handling) {
    self->index = index;
    self->baseHP = baseHP;
    self->value = value;
    self->baseLoad = baseLoad;
    self->currentLoad = 0;
    self->price = value;
    int *slots = operatorNewArrayInt(0x10);
    self->slots = slots;
    slots[0] = s0;
    slots[1] = s1;
    slots[2] = s2;
    slots[3] = s3;
    self->handling = handling / 1.6f;
    Array<Item*> *eq = (Array<Item*> *)operatorNewArrayInt(0xc);
    Array_Item_ctor(eq);
    self->equipment = eq;
    ArraySetLength<Item*>(s1 + s0 + s2 + s3, *eq);
    self->cargo = 0;
    self->race = 0;
    self->mods = 0;
    self->numAddedDeviceSlots = 0;
    refreshValue(self);
    return self;
}

float getHandling(Ship *self) {
    int *m = (int *)self->mods;
    float h = 1.3f;
    if (m != 0) {
        int n = *m;
        for (int i = 0; n != i; i = i + 1) {
            if (((int *)m[1])[i] == 3) {
                h = h + 0.1f;
            }
        }
    }
    return h + self->handling;
}

int getBaseHP(Ship *self) { return self->baseHP; }

unsigned int hasJumpDriveIntegrated(Ship *self) {
    unsigned int v = (unsigned int)self->index - 0x25u;
    if (v < 4) return 0xbu >> (v & 0xf) & 1;
    return 0;
}

void replaceEquipment(Ship *self, Array<Item*> *eq) {
    if (eq != 0) {
        int *slots = self->slots;
        unsigned int sum = slots[0] + slots[3] + slots[1] + slots[2];
        if (sum < eq->size()) {
            int extra = eq->size() - sum;
            self->numAddedDeviceSlots = extra;
            slots[3] = extra + slots[3];
        }
    }
    self->equipment = eq;
    return ((Ship *)(self))->recomputeAfterSlots();
}

void setEquipment(Ship *self, Array<Item*> *items) {
    for (unsigned int i = 0; i < items->size(); i = i + 1) {
        ((Ship *)(self))->setEquipment1(items->data()[i]);
    }
}

int getEquipmentValue(Ship *self) {
    unsigned int i = 0;
    int total = 0;
    for (; i < self->equipment->size(); i = i + 1) {
        Item *it = self->equipment->data()[i];
        if (it != 0) {
            total += ((Item *)(it))->getTotalPrice();
        }
    }
    return total;
}

int getIndex(Ship *self) { return self->index; }

int getFireRateFactor(Ship *self) { return *(int *)&self->fireRateFactor; }

int getCurrentLoad(Ship *self) { return self->currentLoad; }

Ship *_ZN4ShipD2Ev(Ship *self) {
    Array<Item*> *a = self->equipment;
    if (a != 0) {
        if (a->size() != 0) {
            ArrayReleaseClassesItem(a);
            a = self->equipment;
            if (a == 0) goto skip1;
        }
        operatorDelete(ArrayItemDtor(a));
    }
skip1:
    a = self->cargo;
    self->equipment = 0;
    if (a != 0) {
        if (a->size() != 0) {
            ArrayReleaseClassesItem(a);
            a = self->cargo;
            if (a == 0) goto skip2;
        }
        operatorDelete(ArrayItemDtor(a));
    }
skip2:
    self->cargo = 0;
    if (self->slots != 0) {
        operatorDelete(self->slots);
    }
    self->slots = 0;
    if (self->mods != 0) {
        operatorDelete(ArrayIntDtor(self->mods));
    }
    self->mods = 0;
    return self;
}

unsigned char hasEmergencySystem(Ship *self) { return self->hasEmergency; }

int getRadarType(Ship *self) { return self->radarType; }

int getBoostDelay(Ship *self) { return self->boostDelay; }

int getPrice(Ship *self) { return self->price; }

extern "C" int getSlots(Ship *self, int i) { return self->slots[i]; }

extern int **gShipDataRoot __attribute__((visibility("hidden")));
void priceDecline(Ship *self) {
    int *q = *gShipDataRoot;
    int *table = *(int **)((char *)q + 4);
    int *entry = (int *)table[self->index];
    self->price = (int)((float)entry[0x14 / 4] / 1.25f);
}

int getUsedSlots(Ship *self, int type) {
    unsigned int i = 0;
    int n = 0;
    for (; i < self->equipment->size(); i = i + 1) {
        Item *it = self->equipment->data()[i];
        if (it != 0 && ((Item *)(it))->getType() == type) {
            n = n + 1;
        }
    }
    return n;
}

int getSignatureRace(Ship *self) { return self->signatureRace; }

int getMaxShieldHP(Ship *self) { return self->maxShieldHP; }

void setPrice(Ship *self, int v) { self->price = v; }

int getCombinedHP(Ship *self) {
    int *m = (int *)self->mods;
    int bonus;
    if (m == 0) {
        bonus = 0;
    } else {
        bonus = 0;
        int n = *m;
        for (int i = 0; n != i; i = i + 1) {
            if (((int *)m[1])[i] == 0) {
                bonus = bonus + 0x28;
            }
        }
    }
    return bonus + self->baseHP + self->maxShieldHP + self->maxArmorHP;
}

int getCargoPlus(Ship *self) { return self->cargoPlus; }

int getFreeSlots(Ship *self, int type) {
    int total = self->slots[type];
    int used = getUsedSlots(self, type);
    return total - used;
}

int getValue(Ship *self) { return self->value; }

int hasVolatileGoods(Ship *self) {
    if (hasCargo(self, 0xd1, 1) != 0) return 1;
    return hasCargo(self, 0xcc, 1);
}

int getDamageFactor(Ship *self) { return *(int *)&self->damageFactor; }

int getFirePower(Ship *self) { return self->firePower; }

void freeSlot(Ship *self, Item *item, int slot) {
    unsigned int i = 0;
    while (i < self->equipment->size()) {
        Item *it = self->equipment->data()[i];
        if (it != 0) {
            int r = it->equals(item);
            if ((unsigned int)slot == i && r != 0) {
                self->equipment->data()[slot] = 0;
                break;
            }
        }
        i = i + 1;
    }
    return ((Ship *)(self))->recomputeAfterSlots();
}

// Ship::addCargo(Array<Item*>*) -> combine then tail-call addCargo(self, combined)
void addCargo(Ship *self, Array<Item*> *items) {
    return ((Ship *)(self))->addCargo2(Item::combineItems(self->cargo, items));
}

Array<Item*>* getCargo(Ship *self) { return self->cargo; }

bool hasCloakIntegrated(Ship *self) { return self->index == 0x2c || self->index == 0x31; }

bool spaceAvailable(Ship *self, int n) { return n + self->currentLoad <= self->baseLoad + self->cargoPlus; }

Array<int>* getMods(Ship *self) { return self->mods; }

Array<Item*>* Ship::getEquipment() { Ship *self = this; return self->equipment; }

float getHandlingForShop(Ship *self) {
    int *m = (int *)self->mods;
    float h = 1.3f;
    if (m != 0) {
        int n = *m;
        for (int i = 0; n != i; i = i + 1) {
            if (((int *)m[1])[i] == 3) {
                h = h + 0.1f;
            }
        }
    }
    return h + self->handling;
}

int addEquipment(Ship *self, Item *item) {
    int type = ((Item *)(item))->getType();
    int cap = self->slots[type];
    if (cap < 1) {
        return 0;
    }
    int base = 0;
    for (int k = 0; k < type; k = k + 1) {
        base = base + self->slots[k];
    }
    int end = base + cap;
    while (base < end) {
        Item *cur = self->equipment->data()[base];
        base = base + 1;
        if (cur == 0) {
            self->equipment->data()[base - 1] = item;
            return 1;
        }
    }
    return 0;
}

int getUnmoddedHandling(Ship *self) { return *(int *)&self->handling; }

bool hasCloak(Ship *self) {
    if (__builtin_expect(self->hasCloakFlag != 0, 1)) goto yes;
    if (self->index == 0x31 || self->index == 0x2c) goto yes;
    return false;
yes:
    return true;
}

int getBoostSpeed(Ship *self) { return self->boostSpeed; }

int getFreeSpace(Ship *self) { return (self->cargoPlus + self->baseLoad) - self->currentLoad; }

int getShieldRegen(Ship *self) { return self->shieldRegen; }

int getRepairType(Ship *self) { return self->repairType; }

bool equals(Ship *self, Ship *other) { return self->index == other->index; }

void freeAllSlots(Ship *self) {
    for (unsigned int i = 0; i < self->equipment->size(); i = i + 1) {
        Item **data = self->equipment->data();
        if (data[i] != 0) {
            data[i] = 0;
        }
    }
    return ((Ship *)(self))->recomputeAfterSlots();
}

int getMaxHP(Ship *self) {
    int *m = (int *)self->mods;
    int bonus;
    if (m == 0) {
        bonus = 0;
    } else {
        bonus = 0;
        int n = *m;
        for (int i = 0; n != i; i = i + 1) {
            if (((int *)m[1])[i] == 0) {
                bonus = bonus + 0x28;
            }
        }
    }
    return self->baseHP + bonus;
}

unsigned int hasJumpDrive(Ship *self) {
    if (self->hasJumpDriveFlag != 0) return 1;
    unsigned int v = (unsigned int)self->index - 0x25u;
    if (v < 4) return 0xbu >> (v & 0xf) & 1;
    return 0;
}

int getAgility(Ship *self) { return self->agility; }

unsigned int getSlotPos(Ship *self, Item *item) {
    if (item == 0) {
        return 0xffffffff;
    }
    Array<Item*> *eq = self->equipment;
    unsigned int n = eq->size();
    unsigned int pos = 0xffffffff;
    for (unsigned int i = 0; i < n; i = i + 1) {
        if (eq->data()[i] == item) { pos = i; break; }
    }
    for (int j = 0; j < ((Item *)(item))->getType(); j = j + 1) {
        pos = pos - self->slots[j];
    }
    return pos;
}

int hasCargo(Ship *self, int index, int amount) {
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        for (unsigned int i = 0; i < c->size(); i = i + 1) {
            Item *it = c->data()[i];
            if (it != 0 && ((Item *)(it))->getIndex() == index &&
                ((Item *)(self->cargo->data()[i]))->getAmount() >= amount) {
                return 1;
            }
            c = self->cargo;
        }
    }
    return 0;
}

int hasCargoType(Ship *self, int type) {
    unsigned int *p = (unsigned int *)self->cargo;
    if (p != 0) {
        for (unsigned int i = 0; i < *p; i = i + 1) {
            Item *it = *(Item **)(p[1] + i * 4);
            if (it != 0) {
                int t = ((Item *)(it))->getType();
                if (t == type) return 1;
                p = (unsigned int *)self->cargo;
            }
        }
    }
    return 0;
}

Array<Item*>* Ship::getEquipment(int type) { Ship *self = this;
    if (type >= 4 || self->slots[type] == 0) {
        return 0;
    }
    Array<Item*> *result = operatorNewArr(0xc);
    Array_Item_ctor(result);
    ArraySetLength<Item*>(self->slots[type], *result);
    int base = 0;
    for (int i = 0; i < type; i = i + 1) {
        base = base + self->slots[i];
    }
    unsigned int j = 0;
    for (int i = base; i < self->slots[type] + base; i = i + 1) {
        if (j < result->size()) {
            result->data()[j] = self->equipment->data()[i];
            j = j + 1;
        }
    }
    return result;
}

Ship *clone(Ship *self) {
    Ship *s = operatorNewShip(0x80);
    int *slots = self->slots;
    ((Ship *)(s))->ctor_full(self->index, self->baseHP, self->baseLoad, self->price, slots[0], slots[1], slots[2], slots[3] - self->numAddedDeviceSlots, self->handling * 1.6f);
    Array<int> *m = self->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size(); i = i + 1) {
            addMod(s, m->data()[i]);
            m = self->mods;
        }
    }
    return s;
}

extern int *gShopRoot;
Item* getFirstEquipmentOfSort(Ship *self, int sort) {
    Array<Item*> *e = self->equipment;
    if (e != 0) {
        for (unsigned int i = 0; i < e->size(); i = i + 1) {
            Item *it = e->data()[i];
            if (it != 0) {
                int s = ((Item *)(it))->getSort();
                e = self->equipment;
                if (s == sort) {
                    return e->data()[i];
                }
            }
        }
        if (sort == 0x15 && (self->index == 0x31 || self->index == 0x2c)) {
            int *q = gShopRoot;
            return *(Item **)(*(int *)((char *)q + 4) + 0x17c);
        }
    }
    return 0;
}

extern int *gStatusRoot __attribute__((visibility("hidden")));
extern int **gShipDataRoot __attribute__((visibility("hidden")));
extern int *gRaceTable __attribute__((visibility("hidden")));
extern int *gDifficultyPtr __attribute__((visibility("hidden")));
void adjustPrice(Ship *self) {
    int *status = gStatusRoot;
    if (((Status *)(*(void **)status))->getStation() != 0 && self->price > 0) {
        int *root = (int *)gShipDataRoot;
        int *table = *(int **)(*(int *)root + 4);
        int *entry = *(int **)((char *)table + self->index * 4);
        int cat = *entry;
        SolarSystem *system = (SolarSystem *)(intptr_t)((Status *)(*(void **)status))->getSystem();
        int race = system->getRace();
        int *table2 = *(int **)(*(int *)root + 4);
        int *entry2 = *(int **)((char *)table2 + self->index * 4);
        float base = (float)entry2[0x14 / 4];
        float f6 = 0.1f;
        if (gRaceTable[cat] == race) {
            f6 = base * 0.3f;
        }
        float acc = f6 + base;
        float f5 = 0.1f;
        int dv = *(int *)gDifficultyPtr;
        if (dv != 0) {
            f5 = base * (float)dv * 0.7f;
        }
        self->price = (int)(acc + f5);
    }
}

void removeEquipment(Ship *self, Item *item) {
    Array<Item*> *eq = self->equipment;
    if (eq != 0) {
        for (unsigned int i = 0; i < eq->size(); i = i + 1) {
            Item *it = eq->data()[i];
            if (it != 0) {
                int r = it->equals(item);
                eq = self->equipment;
                if (r != 0) {
                    eq->data()[i] = 0;
                    return;
                }
            }
        }
    }
}

Item* getCargo(Ship *self, int index) {
    Array<Item*> *c = self->cargo;
    if (c == 0) return 0;
    for (unsigned int i = 0; i < c->size(); i = i + 1) {
        Item *it = c->data()[i];
        if (it != 0) {
            int idx = ((Item *)(it))->getIndex();
            c = self->cargo;
            if (idx == index) {
                return c->data()[i];
            }
        }
    }
    return 0;
}

void addMod(Ship *self, int mod) {
    Array<int> *m = self->mods;
    if (m == 0) {
        m = operatorNewModsArr(0xc);
        Array_int_ctor(m);
        self->mods = m;
    }
    unsigned int sz = m->size();
    int found = 0;
    unsigned int i = 0;
    while (!found && i < sz) {
        found = m->data()[i] == mod;
        i = i + 1;
    }
    if (found) {
        return;
    }
    ArrayAdd<int>(mod, *m);
    int dev = 0;
    if (mod == 2) {
        dev = self->numAddedDeviceSlots;
    }
    if (mod == 2 && dev == 0) {
        self->slots[3] = self->slots[3] + 1;
        self->numAddedDeviceSlots = self->numAddedDeviceSlots + 1;
        Item *it = operatorNewItem(0x48);
        Item_ctor3(it, 0, 0, 0);
        ArrayAdd<Item*>(it, *self->equipment);
        Item **slot = &self->equipment->data()[self->equipment->size() - 1];
        if (*slot != 0) {
            operatorDelete(ItemDtor(*slot));
            slot = &self->equipment->data()[self->equipment->size() - 1];
        }
        *slot = 0;
    }
    return ((Ship *)(self))->recomputeAfterSlots();
}

extern int *gStatusRoot2 __attribute__((visibility("hidden")));

void refreshValue(Ship *self) {
    int *status = gStatusRoot2;
    self->repairType = -1;
    self->hasJumpDriveFlag = 0;
    self->hasCloakFlag = 0;
    self->hasEmergency = 0;
    self->fireRateFactor = 1.0f;
    self->damageFactor = 1.0f;
    self->maxShieldHP = 0;
    self->maxArmorHP = 0;
    self->shieldRegen = 0;
    self->cargoPlus = 0;
    self->boostTime = 0;
    self->agility = 0;
    self->maxPassengers = 0;
    self->firePower = 0;
    self->signatureRace = -1;
    self->radarType = 0;
    self->boostSpeed = 0;
    self->boostDelay = 0;
    self->boostTime = 0;
    if (*status != 0 && ((Status *)(*(void **)status))->getStanding() != 0) {
        ((Standing *)((void *)(intptr_t)((Status *)(*(void **)status))->getStanding()))->setPlayerSignatureRace(-1);
    }
    self->value = self->price;

    unsigned int i = 0;
    unsigned int n;
    unsigned int *eq;
    for (;;) {
        eq = (unsigned int *)self->equipment;
        n = *eq;
        if (n <= i) break;
        Item *it = *(Item **)(eq[1] + i * 4);
        if (it != 0) {
            int sort = ((Item *)(it))->getSort();
            switch (sort) {
            case 0: case 1: case 2: case 3: case 8: case 0x19: {
                float a = (float)((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(9);
                float b = (float)((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0xb);
                self->firePower = (int)((float)self->firePower + (a / b) * 1.0f);
                break;
            }
            case 9:
                self->maxShieldHP = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x12);
                self->shieldRegen = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x13);
                break;
            case 10:
                self->maxArmorHP = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x14);
                break;
            case 0xc:
                self->cargoPlus = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x16) + self->cargoPlus;
                break;
            case 0xe:
                self->boostSpeed = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x19);
                self->boostTime = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x1b);
                self->boostDelay = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x1a);
                break;
            case 0xf: {
                int idx = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getIndex();
                self->repairType = idx != 0x4b;
                break;
            }
            case 0x10:
                self->agility = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x1c);
                break;
            case 0x11:
                self->radarType = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x1f);
                break;
            case 0x12:
                self->hasJumpDriveFlag = 1;
                break;
            case 0x14:
                self->firePower = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x22) + self->firePower;
                break;
            case 0x15:
                self->hasCloakFlag = 1;
                break;
            case 0x1b:
                self->hasEmergency = 1;
                break;
            case 0x1c: {
                float r = (float)((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x27);
                r = 1.0f - r / 100.0f;
                if (r == 0.0f) r = 1.0f;
                self->fireRateFactor = r;
                float d = (float)((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0x28);
                d = d / 100.0f + 1.0f;
                self->damageFactor = d;
                if (d == 0.0f) self->damageFactor = 1.0f;
                break;
            }
            case 0x1d: {
                int idx = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getIndex();
                self->signatureRace = idx - 0xbd;
                if (*status != 0 && ((Status *)(*(void **)status))->getStanding() != 0) {
                    ((Standing *)((void *)(intptr_t)((Status *)(*(void **)status))->getStanding()))->setPlayerSignatureRace(self->signatureRace);
                }
                break;
            }
            }
            self->value = ((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getTotalPrice() + self->value;
        }
        i = i + 1;
    }

    self->firePower = 0;
    for (i = 0; i < n; i = i + 1) {
        Item *it = *(Item **)(eq[1] + i * 4);
        unsigned int sort;
        if (it != 0 && (sort = ((Item *)(it))->getSort()) < 0x1a && (1 << (sort & 0xff) & 0x4000) != 0) {
            float dmg = self->damageFactor;
            float a = (float)((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(9);
            float b = (float)((Item *)(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)))->getAttribute(0xb);
            self->firePower = (int)((float)self->firePower + ((dmg * a) / (self->fireRateFactor * b)) * 1.0f);
        }
        eq = (unsigned int *)self->equipment;
        n = *eq;
    }

    int *m = (int *)self->mods;
    int loadBonus;
    if (m == 0) {
        loadBonus = 0;
    } else {
        loadBonus = 0;
        int j = 0;
        while (*m != j) {
            int off = j * 4;
            j = j + 1;
            if (*(int *)(m[1] + off) == 1) {
                loadBonus = loadBonus + 0x1e;
            }
        }
    }
    int cargoPlus = self->cargoPlus;
    float cp = (float)cargoPlus;
    float bl = (float)(self->baseLoad + loadBonus);
    self->cargoPlus = loadBonus + (int)((cp * bl) / 100.0f);
    self->compression = cargoPlus;
    self->value = getCargoValue(self) + self->value;
}

int removeCargo(Ship *self, int index, int amount) {
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        unsigned int i = 0;
        int amt;
        for (;;) {
            if (i >= c->size()) goto done;
            if (((Item *)(c->data()[i]))->getIndex() == index) break;
            c = self->cargo;
            i = i + 1;
        }
        ((Item *)(self->cargo->data()[i]))->changeAmount(-amount);
        amt = ((Item *)(self->cargo->data()[i]))->getAmount();
        c = self->cargo;
        if (amt < 1) {
            c = Item::extractItems(c, true);
            setCargo(self, c);
            return 1;
        }
    done:
        setCargo(self, c);
    }
    return 0;
}

extern int **gWorldSingleton __attribute__((visibility("hidden")));
void setCargo(Ship *self, Array<Item*> *cargo) {
    self->currentLoad = 0;
    self->cargo = cargo;
    if (cargo != 0) {
        for (unsigned int i = 0; i < cargo->size(); i = i + 1) {
            int amt = ((Item *)(cargo->data()[i]))->getAmount();
            cargo = self->cargo;
            self->currentLoad = amt + self->currentLoad;
        }
    }
    refreshValue(self);
    int v = (self->baseLoad + self->cargoPlus) - self->currentLoad;
    int *w = *gWorldSingleton;
    if (w[0xdc / 4] < v) {
        w[0xdc / 4] = v;
    }
}

extern int **gWorldSingleton __attribute__((visibility("hidden")));
void replaceCargo(Ship *self, Array<Item*> *cargo) {
    self->currentLoad = 0;
    self->cargo = cargo;
    for (unsigned int i = 0; i < cargo->size(); i = i + 1) {
        int amt = ((Item *)(cargo->data()[i]))->getAmount();
        cargo = self->cargo;
        self->currentLoad = amt + self->currentLoad;
    }
    refreshValue(self);
    int v = (self->baseLoad + self->cargoPlus) - self->currentLoad;
    int *w = *gWorldSingleton;
    if (w[0xdc / 4] < v) {
        w[0xdc / 4] = v;
    }
}

int getModdedLoad(Ship *self) {
    int load = self->baseLoad;
    int *m = (int *)self->mods;
    if (m == 0) return load;
    for (int i = 0; i < *m; i++)
        if (*(int *)(m[1] + i * 4) == 1) load += 0x1e;
    return load;
}

int getSlotTypes(Ship *self) {
    int n = 0;
    int i = 0;
    while (i != 4) {
        int idx = i;
        i = i + 1;
        if (0 < self->slots[idx]) n = n + 1;
    }
    return n;
}

// Ship::addCargo(Item*) -> wrap the single item in a fresh array, then commit.
void Ship::addCargo(Item *item) {
    Array<Item*> *a = operatorNewArray(0xc);
    ArrayCtorItem(a);
    ArrayAdd<Item*>(item, *a);
    ((Ship *)(this))->addCargo2(a);
}

void setEquipment(Ship *self, Item *item) {
    Array<Item*> *eq = self->equipment;
    unsigned int i = 0;
    Item **data;
    Item *v;
    do {
        if (i >= eq->size()) goto done;
        data = eq->data();
        v = data[i];
        i = i + 1;
    } while (v != 0);
    data[i - 1] = item;
done:
    ;
}

int hasSecondaryWeapons(Ship *self) {
    if (self->slots[1] != 0) {
        unsigned int *p = (unsigned int *)self->equipment;
        if (p != 0) {
            unsigned int n = *p;
            if (n != 0) {
                for (unsigned int i = 0; i < n; i = i + 1) {
                    Item *it = *(Item **)(p[1] + i * 4);
                    if (it != 0) {
                        int t = ((Item *)(it))->getType();
                        if (t == 1) return 1;
                        p = (unsigned int *)self->equipment;
                        n = *p;
                    }
                }
            }
        }
    }
    return 0;
}

int hasEquipment(Ship *self, int index, int amount) {
    Array<Item*> *e = self->equipment;
    if (e != 0) {
        for (unsigned int i = 0; i < e->size(); i = i + 1) {
            Item *it = e->data()[i];
            if (it != 0 && ((Item *)(it))->getIndex() == index &&
                ((Item *)(self->equipment->data()[i]))->getAmount() >= amount) {
                return 1;
            }
            e = self->equipment;
        }
    }
    return 0;
}

extern int **gWorldSingleton __attribute__((visibility("hidden")));
void changeLoad(Ship *self, int delta) {
    int **p = gWorldSingleton;
    int cur = self->currentLoad;
    int newLoad = delta + cur;
    self->currentLoad = newLoad;
    int v = (self->baseLoad - newLoad) + self->cargoPlus;
    int *w = *p;
    if (w[0xdc / 4] < v) {
        w[0xdc / 4] = v;
    }
}

int slotAvailable(Ship *self, int sort) {
    if (sort != 0 && sort != 0xc) {
        for (unsigned int i = 0; i < self->equipment->size(); i = i + 1) {
            Item *it = self->equipment->data()[i];
            if (it != 0 && ((Item *)(it))->getSort() == sort) {
                return 0;
            }
        }
    }
    return 1;
}

unsigned int hasModInstalled(Ship *self, int mod) {
    unsigned int *m = (unsigned int *)self->mods;
    if (m != 0) {
        unsigned int i = 0;
        while (i < *m) {
            int idx = i * 4;
            i = i + 1;
            if (*(int *)(m[1] + idx) == mod) {
                return 1;
            }
        }
    }
    return 0;
}

int getCargoValue(Ship *self) {
    int total = 0;
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        for (unsigned int i = 0; i < c->size(); i = i + 1) {
            Item *it = c->data()[i];
            if (it != 0) {
                int v = ((Item *)(it))->getTotalPrice();
                c = self->cargo;
                total = total + v;
            }
        }
    }
    return total;
}

// ===========================================================================
// Recovered real Ship members.
// ===========================================================================

// ---- Ship::Ship(...) ctor @ 0x18457c ----
// Builds a fresh ship from its base stats: id, hull/load/value, the four
// slot-counts (weapon/secondary/shield/device), and the handling rating
// (stored normalised to 0..1 by dividing the design value by 100).
Ship::Ship(int index, int baseHP, int baseLoad, int value,
           int slot0, int slot1, int slot2, int slot3, float handling) {
    this->index       = index;
    this->baseHP      = baseHP;
    this->value       = value;
    this->baseLoad    = baseLoad;
    this->currentLoad = 0;
    this->price       = value;

    int *slotCounts = operatorNewArrayInt(4);
    this->slots = slotCounts;
    slotCounts[0] = slot0;
    slotCounts[1] = slot1;
    slotCounts[2] = slot2;
    slotCounts[3] = slot3;

    this->handling = handling / 100.0f;

    Array<Item*> *eq = operatorNewArr(0xc);
    Array_Item_ctor(eq);
    this->equipment = eq;
    ArraySetLength<Item*>(slot0 + slot1 + slot2 + slot3, *eq);

    this->race                = 0;
    this->cargo               = 0;
    this->mods                = 0;
    this->numAddedDeviceSlots = 0;

    ::refreshValue(this);
}

// ---- Ship::~Ship() dtor @ 0x1849a4 ----
// Releases the owned equipment / cargo item lists, the slot-count array and
// the installed-mods list.
Ship::~Ship() {
    if (this->equipment != 0) {
        if (this->equipment->size() != 0) {
            ArrayReleaseClassesItem(this->equipment);
        }
        if (this->equipment != 0) {
            operatorDelete(ArrayItemDtor(this->equipment));
        }
    }
    this->equipment = 0;

    if (this->cargo != 0) {
        if (this->cargo->size() != 0) {
            ArrayReleaseClassesItem(this->cargo);
        }
        if (this->cargo != 0) {
            operatorDelete(ArrayItemDtor(this->cargo));
        }
    }
    this->cargo = 0;

    if (this->slots != 0) {
        operatorDelete(this->slots);
    }
    this->slots = 0;

    if (this->mods != 0) {
        operatorDelete(ArrayIntDtor(this->mods));
    }
    this->mods = 0;
}

// ---- Ship::recomputeAfterSlots() @ refreshValue core ----
// Invoked after any equipment-slot mutation to re-derive the ship's stats
// (shield/armor/firepower/boost/etc.) and re-price it.
void Ship::recomputeAfterSlots() {
    ::refreshValue(this);
}

// ---- Ship::addCargo(Array<Item*>*) core @ 0x184fe8 ----
// Merges the supplied items into the current cargo hold (stacking matching
// goods) and commits the combined list as the new cargo.
void Ship::addCargo(Array<Item*> *items) {
    ::setCargo(this, Item::combineItems(this->cargo, items));
}

// ---- Ship::removeCargo(int, int) @ 0x184e64 ----
// Removes up to `amount` units of the good with the given index from the
// cargo hold. Returns 1 if the good was fully depleted (and the hold
// compacted), 0 otherwise.
int Ship::removeCargo(int index, int amount) {
    Array<Item*> *c = this->cargo;
    if (c == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < c->size(); i = i + 1) {
        if (c->data()[i]->getIndex() == index) {
            this->cargo->data()[i]->changeAmount(-amount);
            c = this->cargo;
            if (this->cargo->data()[i]->getAmount() < 1) {
                ::setCargo(this, Item::extractItems(c, true));
                return 1;
            }
            break;
        }
        c = this->cargo;
    }
    ::setCargo(this, c);
    return 0;
}

// ---- Ship::setEquipment(Item*) @ 0x18509c ----
// Places `item` into the first empty equipment slot, then recomputes stats.
void Ship::setEquipment(Item *item) {
    Array<Item*> *eq = this->equipment;
    for (unsigned int i = 0; i < eq->size(); i = i + 1) {
        if (eq->data()[i] == 0) {
            eq->data()[i] = item;
            break;
        }
    }
    recomputeAfterSlots();
}

void setMods(Ship *self, Array<int> *mods) {
    self->mods = mods;
    if (mods != 0) {
        for (unsigned int i = 0; i < mods->size(); i = i + 1) {
            int v = mods->data()[i];
            bool isDev = v == 2;
            if (isDev) {
                v = self->numAddedDeviceSlots;
            }
            if (isDev && v == 0) {
                self->slots[3] = self->slots[3] + 1;
                self->numAddedDeviceSlots = self->numAddedDeviceSlots + 1;
                Item *it = operatorNewItem(0x48);
                Item_ctor3(it, 0, 0, 0);
                ArrayAdd<Item*>(it, *self->equipment);
                Item **slot = &self->equipment->data()[self->equipment->size() - 1];
                if (*slot != 0) {
                    operatorDelete(ItemDtor(*slot));
                    slot = &self->equipment->data()[self->equipment->size() - 1];
                }
                *slot = 0;
            }
        }
    }
    return ((Ship *)(self))->recomputeAfterSlots();
}

// ===========================================================================
// Recovered real Ship members (promoted from the recovered helper bodies).
// These are the canonical class methods; the file-local free helpers above
// still encode the shared implementation that the (not-yet-rewired) call
// sites hit through the extern "C" shims.
// ===========================================================================

// ---- cargo ---------------------------------------------------------------

void Ship::removeCargo(int index) {
    removeCargo(index, 9999999);
}

void Ship::removeCargo(Item *item) {
    removeCargo(item->getIndex(), item->getAmount());
}

void Ship::removeAllCargo() {
    if (this->cargo != 0) {
        operatorDelete(ArrayItemDtor(this->cargo));
    }
    this->cargo = 0;
}

void Ship::setCargo(Array<Item*> *cargo) {
    ::setCargo(this, cargo);
}

void Ship::replaceCargo(Array<Item*> *cargo) {
    ::replaceCargo(this, cargo);
}

Array<Item*>* Ship::getCargo() {
    return this->cargo;
}

Item* Ship::getCargo(int index) {
    Array<Item*> *c = this->cargo;
    if (c == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < c->size(); i = i + 1) {
        Item *it = c->data()[i];
        if (it != 0 && it->getIndex() == index) {
            return this->cargo->data()[i];
        }
    }
    return 0;
}

bool Ship::hasCargo(int index, int amount) {
    Array<Item*> *c = this->cargo;
    if (c == 0) {
        return false;
    }
    for (unsigned int i = 0; i < c->size(); i = i + 1) {
        Item *it = c->data()[i];
        if (it != 0 && it->getIndex() == index && it->getAmount() >= amount) {
            return true;
        }
    }
    return false;
}

bool Ship::hasCargoType(int type) {
    Array<Item*> *c = this->cargo;
    if (c == 0) {
        return false;
    }
    for (unsigned int i = 0; i < c->size(); i = i + 1) {
        Item *it = c->data()[i];
        if (it != 0 && it->getType() == type) {
            return true;
        }
    }
    return false;
}

bool Ship::hasVolatileGoods() {
    return hasCargo(0xd1, 1) || hasCargo(0xcc, 1);
}

int Ship::getCargoValue() {
    int total = 0;
    Array<Item*> *c = this->cargo;
    if (c != 0) {
        for (unsigned int i = 0; i < c->size(); i = i + 1) {
            Item *it = c->data()[i];
            if (it != 0) {
                total += it->getTotalPrice();
            }
        }
    }
    return total;
}

bool Ship::spaceAvailable(int n) {
    return n + this->currentLoad <= this->baseLoad + this->cargoPlus;
}

int Ship::getFreeSpace() {
    return (this->cargoPlus + this->baseLoad) - this->currentLoad;
}

void Ship::changeLoad(int delta) {
    ::changeLoad(this, delta);
}

// ---- equipment / slots ---------------------------------------------------

void Ship::setEquipment(Item *item, int slot) {
    unsigned int idx = (unsigned int)slot;
    for (int i = 0; i < item->getType(); i = i + 1) {
        idx += this->slots[i];
    }
    if (idx >= this->equipment->size()) {
        return;
    }
    Item *old = this->equipment->data()[idx];
    if (old != 0) {
        operatorDelete(ItemDtor(old));
    }
    this->equipment->data()[idx] = item;
    recomputeAfterSlots();
}

void Ship::setEquipment(Array<Item*> *items) {
    for (unsigned int i = 0; i < items->size(); i = i + 1) {
        ((Ship *)(this))->setEquipment1(items->data()[i]);
    }
}

void Ship::replaceEquipment(Array<Item*> *equipment) {
    if (equipment != 0) {
        int *slots = this->slots;
        unsigned int total = slots[0] + slots[1] + slots[2] + slots[3];
        if (total < equipment->size()) {
            int extra = equipment->size() - total;
            this->numAddedDeviceSlots = extra;
            slots[3] += extra;
        }
    }
    this->equipment = equipment;
    recomputeAfterSlots();
}

int Ship::addEquipment(Item *item) {
    int type = item->getType();
    int cap = this->slots[type];
    if (cap < 1) {
        return 0;
    }
    int base = 0;
    for (int k = 0; k < type; k = k + 1) {
        base += this->slots[k];
    }
    for (int i = base; i < base + cap; i = i + 1) {
        if (this->equipment->data()[i] == 0) {
            this->equipment->data()[i] = item;
            return 1;
        }
    }
    return 0;
}

void Ship::removeEquipment(Item *item) {
    Array<Item*> *eq = this->equipment;
    if (eq == 0) {
        return;
    }
    for (unsigned int i = 0; i < eq->size(); i = i + 1) {
        Item *it = eq->data()[i];
        if (it != 0 && it->equals(item)) {
            this->equipment->data()[i] = 0;
            return;
        }
    }
}

Item* Ship::getFirstEquipmentOfSort(int sort) {
    return ::getFirstEquipmentOfSort(this, sort);
}

int Ship::getEquipmentValue() {
    int total = 0;
    for (unsigned int i = 0; i < this->equipment->size(); i = i + 1) {
        Item *it = this->equipment->data()[i];
        if (it != 0) {
            total += it->getTotalPrice();
        }
    }
    return total;
}

int Ship::getUsedSlots(int type) {
    int n = 0;
    for (unsigned int i = 0; i < this->equipment->size(); i = i + 1) {
        Item *it = this->equipment->data()[i];
        if (it != 0 && it->getType() == type) {
            n = n + 1;
        }
    }
    return n;
}

int Ship::getFreeSlots(int type) {
    return this->slots[type] - getUsedSlots(type);
}

int Ship::getSlots(int i) {
    return this->slots[i];
}

int Ship::getSlotTypes() {
    int n = 0;
    for (int i = 0; i < 4; i = i + 1) {
        if (this->slots[i] > 0) {
            n = n + 1;
        }
    }
    return n;
}

unsigned int Ship::getSlotPos(Item *item) {
    if (item == 0) {
        return 0xffffffffu;
    }
    Array<Item*> *eq = this->equipment;
    unsigned int pos = 0xffffffffu;
    for (unsigned int i = 0; i < eq->size(); i = i + 1) {
        if (eq->data()[i] == item) {
            pos = i;
            break;
        }
    }
    for (int j = 0; j < item->getType(); j = j + 1) {
        pos -= this->slots[j];
    }
    return pos;
}

void Ship::freeSlot(Item *item) {
    for (unsigned int i = 0; i < this->equipment->size(); i = i + 1) {
        Item *it = this->equipment->data()[i];
        if (it != 0 && it->equals(item)) {
            this->equipment->data()[i] = 0;
            break;
        }
    }
    recomputeAfterSlots();
}

void Ship::freeSlot(Item *item, int slot) {
    for (unsigned int i = 0; i < this->equipment->size(); i = i + 1) {
        Item *it = this->equipment->data()[i];
        if (it != 0 && (unsigned int)slot == i && it->equals(item)) {
            this->equipment->data()[slot] = 0;
            break;
        }
    }
    recomputeAfterSlots();
}

void Ship::freeAllSlots() {
    for (unsigned int i = 0; i < this->equipment->size(); i = i + 1) {
        Item **data = this->equipment->data();
        if (data[i] != 0) {
            data[i] = 0;
        }
    }
    recomputeAfterSlots();
}

int Ship::slotAvailable(int sort) {
    if (sort != 0 && sort != 0xc) {
        for (unsigned int i = 0; i < this->equipment->size(); i = i + 1) {
            Item *it = this->equipment->data()[i];
            if (it != 0 && it->getSort() == sort) {
                return 0;
            }
        }
    }
    return 1;
}

bool Ship::hasEquipment(int index, int amount) {
    Array<Item*> *e = this->equipment;
    if (e == 0) {
        return false;
    }
    for (unsigned int i = 0; i < e->size(); i = i + 1) {
        Item *it = e->data()[i];
        if (it != 0 && it->getIndex() == index && it->getAmount() >= amount) {
            return true;
        }
    }
    return false;
}

bool Ship::hasSecondaryWeapons() {
    if (this->slots[1] == 0 || this->equipment == 0) {
        return false;
    }
    for (unsigned int i = 0; i < this->equipment->size(); i = i + 1) {
        Item *it = this->equipment->data()[i];
        if (it != 0 && it->getType() == 1) {
            return true;
        }
    }
    return false;
}

// ---- valuation / pricing -------------------------------------------------

Ship* Ship::clone() {
    return ::clone(this);
}

bool Ship::equals(Ship *other) {
    return this->index == other->index;
}

void Ship::refreshValue() {
    ::refreshValue(this);
}

void Ship::adjustPrice() {
    ::adjustPrice(this);
}

void Ship::priceDecline() {
    ::priceDecline(this);
}

Ship* Ship::makeShip(int price) {
    Ship *s = clone();
    if (price >= 0) {
        s->price = price;
    }
    return s;
}

// ---- mods ----------------------------------------------------------------

void Ship::addMod(int mod) {
    ::addMod(this, mod);
}

void Ship::setMods(Array<int> *mods) {
    ::setMods(this, mods);
}

Array<int>* Ship::getMods() {
    return this->mods;
}

bool Ship::hasModInstalled(int mod) {
    Array<int> *m = this->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size(); i = i + 1) {
            if (m->data()[i] == mod) {
                return true;
            }
        }
    }
    return false;
}

int Ship::getModdedLoad() {
    int load = this->baseLoad;
    Array<int> *m = this->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size(); i = i + 1) {
            if (m->data()[i] == 1) {
                load += 0x1e;
            }
        }
    }
    return load;
}

// ---- simple stat accessors ----------------------------------------------

int Ship::getIndex()              { return this->index; }
int Ship::getRace()               { return this->race; }
void Ship::setRace(int race)      { this->race = race; }
int Ship::getSignatureRace()      { return this->signatureRace; }
int Ship::getBaseHP()             { return this->baseHP; }
int Ship::getMaxShieldHP()        { return this->maxShieldHP; }
int Ship::getMaxArmorHP()         { return this->maxArmorHP; }
int Ship::getShieldRegen()        { return this->shieldRegen; }
int Ship::getBaseLoad()           { return this->baseLoad; }
int Ship::getMaxLoad()            { return this->cargoPlus + this->baseLoad; }
int Ship::getCurrentLoad()        { return this->currentLoad; }
int Ship::getCargoPlus()          { return this->cargoPlus; }
int Ship::getCompression()        { return this->compression; }
int Ship::getValue()              { return this->value; }
int Ship::getPrice()              { return this->price; }
void Ship::setPrice(int price)    { this->price = price; }
int Ship::getUnmoddedHandling()   { return *(int *)&this->handling; }
int Ship::getFireRateFactor()     { return *(int *)&this->fireRateFactor; }
int Ship::getDamageFactor()       { return *(int *)&this->damageFactor; }
int Ship::getFirePower()          { return this->firePower; }
int Ship::getAgility()            { return this->agility; }
int Ship::getRadarType()          { return this->radarType; }
int Ship::getRepairType()         { return this->repairType; }
int Ship::getMaxPassengers()      { return this->maxPassengers; }
int Ship::getBoostSpeed()         { return this->boostSpeed; }
int Ship::getBoostDelay()         { return this->boostDelay; }
int Ship::getBoostTime()          { return this->boostTime; }
bool Ship::hasBooster()           { return this->boostSpeed > 0; }
int Ship::getCurrentWeaponSlot()  { return this->currentWeaponSlot; }
void Ship::setCurrentWeaponSlot(int slot) { this->currentWeaponSlot = slot; }
int Ship::getNumAddedDeviceSlots(){ return this->numAddedDeviceSlots; }
unsigned char Ship::hasEmergencySystem() { return this->hasEmergency; }

int Ship::getMaxHP() {
    int bonus = 0;
    Array<int> *m = this->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size(); i = i + 1) {
            if (m->data()[i] == 0) {
                bonus += 0x28;
            }
        }
    }
    return this->baseHP + bonus;
}

int Ship::getCombinedHP() {
    int bonus = 0;
    Array<int> *m = this->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size(); i = i + 1) {
            if (m->data()[i] == 0) {
                bonus += 0x28;
            }
        }
    }
    return bonus + this->baseHP + this->maxShieldHP + this->maxArmorHP;
}

float Ship::getHandling() {
    float h = 1.3f;
    Array<int> *m = this->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size(); i = i + 1) {
            if (m->data()[i] == 3) {
                h += 0.1f;
            }
        }
    }
    return h + this->handling;
}

float Ship::getHandlingForShop() {
    float h = 1.3f;
    Array<int> *m = this->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size(); i = i + 1) {
            if (m->data()[i] == 3) {
                h += 0.1f;
            }
        }
    }
    return h + this->handling;
}

bool Ship::hasCloak() {
    return this->hasCloakFlag != 0 || this->index == 0x31 || this->index == 0x2c;
}

bool Ship::hasCloakIntegrated() {
    return this->index == 0x2c || this->index == 0x31;
}

unsigned int Ship::hasJumpDrive() {
    if (this->hasJumpDriveFlag != 0) {
        return 1;
    }
    unsigned int v = (unsigned int)this->index - 0x25u;
    if (v < 4) {
        return 0xbu >> (v & 0xf) & 1;
    }
    return 0;
}

unsigned int Ship::hasJumpDriveIntegrated() {
    unsigned int v = (unsigned int)this->index - 0x25u;
    if (v < 4) {
        return 0xbu >> (v & 0xf) & 1;
    }
    return 0;
}

// ===========================================================================
// Recovered decompiler-fragment members (canonical bodies, distinct names).
// These mirror the real members above and exist so the extern "C" shims
// (Ship_ctor_full / Ship_addCargo2 / Ship_setEquipment1 / Ship_removeCargo3 /
//  Ship_getEquipmentByType / Ship_hasCloakNeg) have a member to bind to.
// ===========================================================================

// ---- Ship::ctor_full @ 0x18457c (Ship::Ship full constructor) ------------
// In-place initializer used by clone(): the disassembly treats the ctor as a
// free function that takes an already-allocated `self` and returns it.
Ship *Ship::ctor_full(int index, int baseHP, int baseLoad, int value,
                      int slot0, int slot1, int slot2, int slot3, float handling) {
    this->index       = index;
    this->baseHP      = baseHP;
    this->value       = value;
    this->baseLoad    = baseLoad;
    this->currentLoad = 0;
    this->price       = value;

    int *slotCounts = operatorNewArrayInt(4);
    this->slots = slotCounts;
    slotCounts[0] = slot0;
    slotCounts[1] = slot1;
    slotCounts[2] = slot2;
    slotCounts[3] = slot3;

    this->handling = handling / 100.0f;

    Array<Item*> *eq = operatorNewArr(0xc);
    Array_Item_ctor(eq);
    this->equipment = eq;
    ArraySetLength<Item*>(slot0 + slot1 + slot2 + slot3, *eq);

    this->race                = 0;
    this->cargo               = 0;
    this->mods                = 0;
    this->numAddedDeviceSlots = 0;

    ::refreshValue(this);
    return this;
}

// ---- Ship::addCargo2 @ 0x184f00 (commit-cargo / setCargo tail) -----------
// Takes the merged item list and installs it as the new cargo hold, recomputes
// the current load and updates the world's peak-cargo high-water mark.
void Ship::addCargo2(Array<Item*> *items) {
    ::setCargo(this, items);
}

// ---- Ship::setEquipment1 @ 0x18509c --------------------------------------
// Places `item` into the first empty equipment slot, then recomputes stats.
void Ship::setEquipment1(Item *item) {
    Array<Item*> *eq = this->equipment;
    for (unsigned int i = 0; i < eq->size(); i = i + 1) {
        if (eq->data()[i] == 0) {
            eq->data()[i] = item;
            break;
        }
    }
    recomputeAfterSlots();
}

// ---- Ship::removeCargo3 @ 0x184e64 ---------------------------------------
// Removes up to `amount` units of the good with index `index` from the cargo
// hold; returns 1 if the good was fully depleted (and the hold compacted).
int Ship::removeCargo3(int index, int amount) {
    return removeCargo(index, amount);
}

// ---- Ship::getEquipmentByType @ 0x1750cc (getEquipment(int)) -------------
// Returns a freshly built array of the items installed in the slot-type's
// contiguous slot range, or null if the ship has no slots of that type.
Array<Item*>* Ship::getEquipmentByType(unsigned int type) {
    return getEquipment((int)type);
}

// ---- Ship::hasCloakNeg @ 0x184be8 (cloak accessor) -----------------------
// The HUD reads this and XORs the result with 1 to derive its "no-cloak"
// flag, so it returns the plain cloak-present truth value.
unsigned char Ship::hasCloakNeg() {
    return (unsigned char)(hasCloak() ? 1 : 0);
}
