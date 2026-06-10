#include "gof2/Ship.h"


extern "C" void Ship_removeCargo3(Ship *self, int idx, int amount);
extern "C" int Item_getType(Item *it);
extern "C" void *ItemDtor(Item *it);
extern "C" void operatorDelete(void *p);
extern "C" void Ship_recomputeAfterSlots(Ship *self);
extern "C" Ship* clone(Ship *self);
extern "C" int Item_getIndex(Item *it);
extern "C" int Item_getAmount(Item *it);
extern "C" void *ArrayItemDtor(Array<Item*> *a);
extern "C" int Item_equals(Item *it, Item *other);
extern "C" void Array_Item_ctor(Array<Item*> *a);
extern "C" int *operatorNewArrayInt(unsigned int n);
extern "C" void refreshValue(Ship *self);
extern "C" void Ship_setEquipment1(Ship *self, Item *item);
extern "C" int Item_getTotalPrice(Item *it);
extern "C" void ArrayReleaseClassesItem(Array<Item*> *a);
extern "C" void *ArrayIntDtor(Array<int> *a);
extern "C" int getUsedSlots(Ship *self, int type);
extern "C" int hasCargo(Ship *self, int type, int amount);
extern "C" Array<Item*>* Item_combineItems(Array<Item*> *a, Array<Item*> *b);
extern "C" void Ship_addCargo2(Ship *self, Array<Item*> *items);
extern "C" Array<Item*>* operatorNewArr(unsigned int sz);
extern "C" Ship *operatorNewShip(unsigned int sz);
extern "C" void addMod(Ship *self, int mod);
extern "C" int Item_getSort(Item *it);
extern "C" int Status_getStation(void *s);
extern "C" void Status_getSystem(void *s);
extern "C" int SolarSystem_getRace(void);
extern "C" Array<int>* operatorNewModsArr(unsigned int sz);
extern "C" void Array_int_ctor(Array<int> *a);
extern "C" Item* operatorNewItem(unsigned int sz);
extern "C" void Item_ctor3(Item *it, void *a, void *b, void *c);
extern "C" int Item_getAttribute(Item *it, int which);
extern "C" int Status_getStanding(void);
extern "C" void Standing_setPlayerSignatureRace(void *st, int race);
extern "C" int getCargoValue(Ship *self);
extern "C" void Item_changeAmount(Item *it, int delta);
extern "C" Array<Item*>* Item_extractItems(Array<Item*> *a, bool flag);
extern "C" void setCargo(Ship *self, Array<Item*> *cargo);
extern "C" Array<Item*>* operatorNewArray(unsigned int sz);
extern "C" void ArrayCtorItem(Array<Item*> *a);

// ---- removeCargo_174ef4.cpp ----
// Ship::removeCargo(int) -> tail-calls Ship::removeCargo(int,int) with amount = 9999999
void removeCargo(Ship *self, int idx) {
    return Ship_removeCargo3(self, idx, 9999999);
}

// ---- getRace_174a06.cpp ----
extern "C" int getRace(Ship *self) { return self->race; }

// ---- hasBooster_174b9a.cpp ----
extern "C" bool hasBooster(Ship *self) { return 0 < self->boostSpeed; }

// ---- getCompression_174c3c.cpp ----
extern "C" int getCompression(Ship *self) { return self->compression; }

// ---- getBaseLoad_174c34.cpp ----
extern "C" int getBaseLoad(Ship *self) { return self->baseLoad; }

// ---- setCurrentWeaponSlot_174a12.cpp ----
extern "C" void setCurrentWeaponSlot(Ship *self, int v) { self->currentWeaponSlot = v; }

// ---- getNumAddedDeviceSlots_175694.cpp ----
extern "C" int getNumAddedDeviceSlots(Ship *self) { return self->numAddedDeviceSlots; }

// ---- getMaxPassengers_1752d4.cpp ----
extern "C" int getMaxPassengers(Ship *self) { return self->maxPassengers; }

// ---- getBoostTime_174b4c.cpp ----
extern "C" int getBoostTime(Ship *self) { return self->boostTime; }

// ---- setEquipment_175204.cpp ----
void setEquipment(Ship *self, Item *item, int slot) {
    unsigned int idx = slot;
    for (int i = 0; i < Item_getType(item); i = i + 1) {
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
    return Ship_recomputeAfterSlots(self);
}

// ---- makeShip_175430.cpp ----
extern "C" void makeShip(Ship *self, int v) {
    Ship *s = clone(self);
    if (-1 < v) s->price = v;
}

// ---- removeCargo_174e38.cpp ----
// Ship::removeCargo(Item*) -> tail-call removeCargo(self, item->getIndex(), item->getAmount())
void removeCargo(Ship *self, Item *item) {
    int idx = Item_getIndex(item);
    int amt = Item_getAmount(item);
    Ship_removeCargo3(self, idx, amt);
}

// ---- removeAllCargo_174eda.cpp ----
extern "C" void removeAllCargo(Ship *self) {
    if (self->cargo != 0) {
        void *p = ArrayItemDtor(self->cargo);
        operatorDelete(p);
    }
    self->cargo = 0;
}

// ---- getMaxLoad_174c40.cpp ----
extern "C" int getMaxLoad(Ship *self) { return self->cargoPlus + self->baseLoad; }

// ---- freeSlot_175344.cpp ----
void freeSlot(Ship *self, Item *item) {
    unsigned int i = 0;
    while (i < self->equipment->size()) {
        Item *it = self->equipment->data()[i];
        if (it != 0 && Item_equals(it, item) != 0) {
            self->equipment->data()[i] = 0;
            break;
        }
        i = i + 1;
    }
    return Ship_recomputeAfterSlots(self);
}

// ---- getMaxArmorHP_174afa.cpp ----
extern "C" int getMaxArmorHP(Ship *self) { return self->maxArmorHP; }

// ---- getCurrentWeaponSlot_174a16.cpp ----
extern "C" int getCurrentWeaponSlot(Ship *self) { return self->currentWeaponSlot; }

// ---- setRace_174a02.cpp ----
extern "C" void setRace(Ship *self, int v) { self->race = v; }

// ---- Ship_17457c.cpp ----
extern "C" Ship *_ZN4ShipC2Eiiiiiiiif(Ship *self, int index, int baseHP, int baseLoad,
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

// ---- getHandling_174a68.cpp ----
extern "C" float getHandling(Ship *self) {
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

// ---- getBaseHP_174ab4.cpp ----
extern "C" int getBaseHP(Ship *self) { return self->baseHP; }

// ---- hasJumpDriveIntegrated_174bcc.cpp ----
extern "C" unsigned int hasJumpDriveIntegrated(Ship *self) {
    unsigned int v = (unsigned int)self->index - 0x25u;
    if (v < 4) return 0xbu >> (v & 0xf) & 1;
    return 0;
}

// ---- replaceEquipment_175000.cpp ----
extern "C" void replaceEquipment(Ship *self, Array<Item*> *eq) {
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
    return Ship_recomputeAfterSlots(self);
}

// ---- setEquipment_175072.cpp ----
void setEquipment(Ship *self, Array<Item*> *items) {
    for (unsigned int i = 0; i < items->size(); i = i + 1) {
        Ship_setEquipment1(self, items->data()[i]);
    }
}

// ---- getEquipmentValue_1752a4.cpp ----
extern "C" int getEquipmentValue(Ship *self) {
    unsigned int i = 0;
    int total = 0;
    for (; i < self->equipment->size(); i = i + 1) {
        Item *it = self->equipment->data()[i];
        if (it != 0) {
            total += Item_getTotalPrice(it);
        }
    }
    return total;
}

// ---- getIndex_174a0a.cpp ----
extern "C" int getIndex(Ship *self) { return self->index; }

// ---- getFireRateFactor_174b58.cpp ----
extern "C" int getFireRateFactor(Ship *self) { return *(int *)&self->fireRateFactor; }

// ---- getCurrentLoad_174c48.cpp ----
extern "C" int getCurrentLoad(Ship *self) { return self->currentLoad; }

// ---- _Ship_1749a4.cpp ----
extern "C" Ship *_ZN4ShipD2Ev(Ship *self) {
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

// ---- hasEmergencySystem_174c22.cpp ----
extern "C" unsigned char hasEmergencySystem(Ship *self) { return self->hasEmergency; }

// ---- getRadarType_174b44.cpp ----
extern "C" int getRadarType(Ship *self) { return self->radarType; }

// ---- getBoostDelay_174b50.cpp ----
extern "C" int getBoostDelay(Ship *self) { return self->boostDelay; }

// ---- getPrice_174a0e.cpp ----
extern "C" int getPrice(Ship *self) { return self->price; }

// ---- getSlots_175388.cpp ----
extern "C" int getSlots(Ship *self, int i) { return self->slots[i]; }

// ---- priceDecline_1754c8.cpp ----
extern int **gShipDataRoot __attribute__((visibility("hidden")));
extern "C" void priceDecline(Ship *self) {
    int *q = *gShipDataRoot;
    int *table = *(int **)((char *)q + 4);
    int *entry = (int *)table[self->index];
    self->price = (int)((float)entry[0x14 / 4] / 1.25f);
}

// ---- getUsedSlots_1753aa.cpp ----
extern "C" int getUsedSlots(Ship *self, int type) {
    unsigned int i = 0;
    int n = 0;
    for (; i < self->equipment->size(); i = i + 1) {
        Item *it = self->equipment->data()[i];
        if (it != 0 && Item_getType(it) == type) {
            n = n + 1;
        }
    }
    return n;
}

// ---- getSignatureRace_174c2c.cpp ----
extern "C" int getSignatureRace(Ship *self) { return self->signatureRace; }

// ---- getMaxShieldHP_174b38.cpp ----
extern "C" int getMaxShieldHP(Ship *self) { return self->maxShieldHP; }

// ---- setPrice_1755b8.cpp ----
extern "C" void setPrice(Ship *self, int v) { self->price = v; }

// ---- getCombinedHP_174ab8.cpp ----
extern "C" int getCombinedHP(Ship *self) {
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

// ---- getCargoPlus_174c38.cpp ----
extern "C" int getCargoPlus(Ship *self) { return self->cargoPlus; }

// ---- getFreeSlots_17541e.cpp ----
extern "C" int getFreeSlots(Ship *self, int type) {
    int total = self->slots[type];
    int used = getUsedSlots(self, type);
    return total - used;
}

// ---- getValue_174b40.cpp ----
extern "C" int getValue(Ship *self) { return self->value; }

// ---- hasVolatileGoods_174d90.cpp ----
extern "C" int hasVolatileGoods(Ship *self) {
    if (hasCargo(self, 0xd1, 1) != 0) return 1;
    return hasCargo(self, 0xcc, 1);
}

// ---- getDamageFactor_174b5c.cpp ----
extern "C" int getDamageFactor(Ship *self) { return *(int *)&self->damageFactor; }

// ---- getFirePower_174c30.cpp ----
extern "C" int getFirePower(Ship *self) { return self->firePower; }

// ---- freeSlot_1752fa.cpp ----
void freeSlot(Ship *self, Item *item, int slot) {
    unsigned int i = 0;
    while (i < self->equipment->size()) {
        Item *it = self->equipment->data()[i];
        if (it != 0) {
            int r = Item_equals(it, item);
            if ((unsigned int)slot == i && r != 0) {
                self->equipment->data()[slot] = 0;
                break;
            }
        }
        i = i + 1;
    }
    return Ship_recomputeAfterSlots(self);
}

// ---- addCargo_174fe8.cpp ----
// Ship::addCargo(Array<Item*>*) -> combine then tail-call addCargo(self, combined)
void addCargo(Ship *self, Array<Item*> *items) {
    return Ship_addCargo2(self, Item_combineItems(self->cargo, items));
}

// ---- getCargo_174c8c.cpp ----
Array<Item*>* getCargo(Ship *self) { return self->cargo; }

// ---- hasCloakIntegrated_174c0a.cpp ----
extern "C" bool hasCloakIntegrated(Ship *self) { return self->index == 0x2c || self->index == 0x31; }

// ---- spaceAvailable_174f96.cpp ----
extern "C" bool spaceAvailable(Ship *self, int n) { return n + self->currentLoad <= self->baseLoad + self->cargoPlus; }

// ---- getMods_1756ca.cpp ----
extern "C" Array<int>* getMods(Ship *self) { return self->mods; }

// ---- getEquipment_174c88.cpp ----
Array<Item*>* getEquipment(Ship *self) { return self->equipment; }

// ---- getHandlingForShop_174a1c.cpp ----
extern "C" float getHandlingForShop(Ship *self) {
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

// ---- addEquipment_1751ae.cpp ----
extern "C" int addEquipment(Ship *self, Item *item) {
    int type = Item_getType(item);
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

// ---- getUnmoddedHandling_175698.cpp ----
extern "C" int getUnmoddedHandling(Ship *self) { return *(int *)&self->handling; }

// ---- hasCloak_174be8.cpp ----
extern "C" bool hasCloak(Ship *self) {
    if (__builtin_expect(self->hasCloakFlag != 0, 1)) goto yes;
    if (self->index == 0x31 || self->index == 0x2c) goto yes;
    return false;
yes:
    return true;
}

// ---- getBoostSpeed_174b48.cpp ----
extern "C" int getBoostSpeed(Ship *self) { return self->boostSpeed; }

// ---- getFreeSpace_174c7c.cpp ----
extern "C" int getFreeSpace(Ship *self) { return (self->cargoPlus + self->baseLoad) - self->currentLoad; }

// ---- getShieldRegen_174b3c.cpp ----
extern "C" int getShieldRegen(Ship *self) { return self->shieldRegen; }

// ---- getRepairType_174c28.cpp ----
extern "C" int getRepairType(Ship *self) { return self->repairType; }

// ---- equals_175686.cpp ----
extern "C" bool equals(Ship *self, Ship *other) { return self->index == other->index; }

// ---- freeAllSlots_1752d8.cpp ----
extern "C" void freeAllSlots(Ship *self) {
    for (unsigned int i = 0; i < self->equipment->size(); i = i + 1) {
        Item **data = self->equipment->data();
        if (data[i] != 0) {
            data[i] = 0;
        }
    }
    return Ship_recomputeAfterSlots(self);
}

// ---- getMaxHP_174afe.cpp ----
extern "C" int getMaxHP(Ship *self) {
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

// ---- hasJumpDrive_174ba6.cpp ----
extern "C" unsigned int hasJumpDrive(Ship *self) {
    if (self->hasJumpDriveFlag != 0) return 1;
    unsigned int v = (unsigned int)self->index - 0x25u;
    if (v < 4) return 0xbu >> (v & 0xf) & 1;
    return 0;
}

// ---- getAgility_174b54.cpp ----
extern "C" int getAgility(Ship *self) { return self->agility; }

// ---- getSlotPos_17515a.cpp ----
extern "C" unsigned int getSlotPos(Ship *self, Item *item) {
    if (item == 0) {
        return 0xffffffff;
    }
    Array<Item*> *eq = self->equipment;
    unsigned int n = eq->size();
    unsigned int pos = 0xffffffff;
    for (unsigned int i = 0; i < n; i = i + 1) {
        if (eq->data()[i] == item) { pos = i; break; }
    }
    for (int j = 0; j < Item_getType(item); j = j + 1) {
        pos = pos - self->slots[j];
    }
    return pos;
}

// ---- hasCargo_174db2.cpp ----
extern "C" int hasCargo(Ship *self, int index, int amount) {
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        for (unsigned int i = 0; i < c->size(); i = i + 1) {
            Item *it = c->data()[i];
            if (it != 0 && Item_getIndex(it) == index &&
                Item_getAmount(self->cargo->data()[i]) >= amount) {
                return 1;
            }
            c = self->cargo;
        }
    }
    return 0;
}

// ---- hasCargoType_174dfe.cpp ----
extern "C" int hasCargoType(Ship *self, int type) {
    unsigned int *p = (unsigned int *)self->cargo;
    if (p != 0) {
        for (unsigned int i = 0; i < *p; i = i + 1) {
            Item *it = *(Item **)(p[1] + i * 4);
            if (it != 0) {
                int t = Item_getType(it);
                if (t == type) return 1;
                p = (unsigned int *)self->cargo;
            }
        }
    }
    return 0;
}

// ---- getEquipment_1750cc.cpp ----
Array<Item*>* getEquipment(Ship *self, int type) {
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

// ---- clone_175444.cpp ----
extern "C" Ship *Ship_ctor_full(Ship *self, int index, int baseHP, int baseLoad, int value,
                                int s0, int s1, int s2, int s3, float handling);
extern "C" Ship *clone(Ship *self) {
    Ship *s = operatorNewShip(0x80);
    int *slots = self->slots;
    Ship_ctor_full(s, self->index, self->baseHP, self->baseLoad, self->price,
                   slots[0], slots[1], slots[2], slots[3] - self->numAddedDeviceSlots,
                   self->handling * 1.6f);
    Array<int> *m = self->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size(); i = i + 1) {
            addMod(s, m->data()[i]);
            m = self->mods;
        }
    }
    return s;
}

// ---- getFirstEquipmentOfSort_174ce4.cpp ----
extern int *gShopRoot;
extern "C" Item* getFirstEquipmentOfSort(Ship *self, int sort) {
    Array<Item*> *e = self->equipment;
    if (e != 0) {
        for (unsigned int i = 0; i < e->size(); i = i + 1) {
            Item *it = e->data()[i];
            if (it != 0) {
                int s = Item_getSort(it);
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

// ---- adjustPrice_1754f8.cpp ----
extern int *gStatusRoot __attribute__((visibility("hidden")));
extern int **gShipDataRoot __attribute__((visibility("hidden")));
extern int *gRaceTable __attribute__((visibility("hidden")));
extern int *gDifficultyPtr __attribute__((visibility("hidden")));
extern "C" void adjustPrice(Ship *self) {
    int *status = gStatusRoot;
    if (Status_getStation(*(void **)status) != 0 && self->price > 0) {
        int *root = (int *)gShipDataRoot;
        int *table = *(int **)(*(int *)root + 4);
        int *entry = *(int **)((char *)table + self->index * 4);
        int cat = *entry;
        Status_getSystem(*(void **)status);
        int race = SolarSystem_getRace();
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

// ---- removeEquipment_175034.cpp ----
extern "C" void removeEquipment(Ship *self, Item *item) {
    Array<Item*> *eq = self->equipment;
    if (eq != 0) {
        for (unsigned int i = 0; i < eq->size(); i = i + 1) {
            Item *it = eq->data()[i];
            if (it != 0) {
                int r = Item_equals(it, item);
                eq = self->equipment;
                if (r != 0) {
                    eq->data()[i] = 0;
                    return;
                }
            }
        }
    }
}

// ---- getCargo_174f58.cpp ----
Item* getCargo(Ship *self, int index) {
    Array<Item*> *c = self->cargo;
    if (c == 0) return 0;
    for (unsigned int i = 0; i < c->size(); i = i + 1) {
        Item *it = c->data()[i];
        if (it != 0) {
            int idx = Item_getIndex(it);
            c = self->cargo;
            if (idx == index) {
                return c->data()[i];
            }
        }
    }
    return 0;
}

// ---- addMod_1755bc.cpp ----
extern "C" void addMod(Ship *self, int mod) {
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
    return Ship_recomputeAfterSlots(self);
}

// ---- refreshValue_174608.cpp ----
extern int *gStatusRoot2 __attribute__((visibility("hidden")));

extern "C" void refreshValue(Ship *self) {
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
    if (*status != 0 && Status_getStanding() != 0) {
        Standing_setPlayerSignatureRace((void *)Status_getStanding(), -1);
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
            int sort = Item_getSort(it);
            switch (sort) {
            case 0: case 1: case 2: case 3: case 8: case 0x19: {
                float a = (float)Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 9);
                float b = (float)Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0xb);
                self->firePower = (int)((float)self->firePower + (a / b) * 1.0f);
                break;
            }
            case 9:
                self->maxShieldHP = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x12);
                self->shieldRegen = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x13);
                break;
            case 10:
                self->maxArmorHP = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x14);
                break;
            case 0xc:
                self->cargoPlus = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x16) + self->cargoPlus;
                break;
            case 0xe:
                self->boostSpeed = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x19);
                self->boostTime = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x1b);
                self->boostDelay = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x1a);
                break;
            case 0xf: {
                int idx = Item_getIndex(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4));
                self->repairType = idx != 0x4b;
                break;
            }
            case 0x10:
                self->agility = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x1c);
                break;
            case 0x11:
                self->radarType = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x1f);
                break;
            case 0x12:
                self->hasJumpDriveFlag = 1;
                break;
            case 0x14:
                self->firePower = Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x22) + self->firePower;
                break;
            case 0x15:
                self->hasCloakFlag = 1;
                break;
            case 0x1b:
                self->hasEmergency = 1;
                break;
            case 0x1c: {
                float r = (float)Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x27);
                r = 1.0f - r / 100.0f;
                if (r == 0.0f) r = 1.0f;
                self->fireRateFactor = r;
                float d = (float)Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0x28);
                d = d / 100.0f + 1.0f;
                self->damageFactor = d;
                if (d == 0.0f) self->damageFactor = 1.0f;
                break;
            }
            case 0x1d: {
                int idx = Item_getIndex(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4));
                self->signatureRace = idx - 0xbd;
                if (*status != 0 && Status_getStanding() != 0) {
                    Standing_setPlayerSignatureRace((void *)Status_getStanding(), self->signatureRace);
                }
                break;
            }
            }
            self->value = Item_getTotalPrice(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4)) + self->value;
        }
        i = i + 1;
    }

    self->firePower = 0;
    for (i = 0; i < n; i = i + 1) {
        Item *it = *(Item **)(eq[1] + i * 4);
        unsigned int sort;
        if (it != 0 && (sort = Item_getSort(it)) < 0x1a && (1 << (sort & 0xff) & 0x4000) != 0) {
            float dmg = self->damageFactor;
            float a = (float)Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 9);
            float b = (float)Item_getAttribute(*(Item **)(((unsigned int *)self->equipment)[1] + i * 4), 0xb);
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

// ---- removeCargo_174e64.cpp ----
int removeCargo(Ship *self, int index, int amount) {
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        unsigned int i = 0;
        int amt;
        for (;;) {
            if (i >= c->size()) goto done;
            if (Item_getIndex(c->data()[i]) == index) break;
            c = self->cargo;
            i = i + 1;
        }
        Item_changeAmount(self->cargo->data()[i], -amount);
        amt = Item_getAmount(self->cargo->data()[i]);
        c = self->cargo;
        if (amt < 1) {
            c = Item_extractItems(c, true);
            setCargo(self, c);
            return 1;
        }
    done:
        setCargo(self, c);
    }
    return 0;
}

// ---- setCargo_174f00.cpp ----
extern int **gWorldSingleton __attribute__((visibility("hidden")));
extern "C" void setCargo(Ship *self, Array<Item*> *cargo) {
    self->currentLoad = 0;
    self->cargo = cargo;
    if (cargo != 0) {
        for (unsigned int i = 0; i < cargo->size(); i = i + 1) {
            int amt = Item_getAmount(cargo->data()[i]);
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

// ---- replaceCargo_174c90.cpp ----
extern int **gWorldSingleton __attribute__((visibility("hidden")));
extern "C" void replaceCargo(Ship *self, Array<Item*> *cargo) {
    self->currentLoad = 0;
    self->cargo = cargo;
    for (unsigned int i = 0; i < cargo->size(); i = i + 1) {
        int amt = Item_getAmount(cargo->data()[i]);
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

// ---- getModdedLoad_17569c.cpp ----
extern "C" int getModdedLoad(Ship *self) {
    int load = self->baseLoad;
    int *m = (int *)self->mods;
    if (m == 0) return load;
    for (int i = 0; i < *m; i++)
        if (*(int *)(m[1] + i * 4) == 1) load += 0x1e;
    return load;
}

// ---- getSlotTypes_175390.cpp ----
extern "C" int getSlotTypes(Ship *self) {
    int n = 0;
    int i = 0;
    while (i != 4) {
        int idx = i;
        i = i + 1;
        if (0 < self->slots[idx]) n = n + 1;
    }
    return n;
}

// ---- addCargo_174faa.cpp ----
void addCargo(Ship *self, Item *item) {
    Array<Item*> *a = operatorNewArray(0xc);
    ArrayCtorItem(a);
    ArrayAdd<Item*>(item, *a);
    Ship_addCargo2(self, a);
}

// ---- setEquipment_17509c.cpp ----
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
    return Ship_recomputeAfterSlots(self);
}

// ---- hasSecondaryWeapons_174b60.cpp ----
extern "C" int hasSecondaryWeapons(Ship *self) {
    if (self->slots[1] != 0) {
        unsigned int *p = (unsigned int *)self->equipment;
        if (p != 0) {
            unsigned int n = *p;
            if (n != 0) {
                for (unsigned int i = 0; i < n; i = i + 1) {
                    Item *it = *(Item **)(p[1] + i * 4);
                    if (it != 0) {
                        int t = Item_getType(it);
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

// ---- hasEquipment_174d44.cpp ----
extern "C" int hasEquipment(Ship *self, int index, int amount) {
    Array<Item*> *e = self->equipment;
    if (e != 0) {
        for (unsigned int i = 0; i < e->size(); i = i + 1) {
            Item *it = e->data()[i];
            if (it != 0 && Item_getIndex(it) == index &&
                Item_getAmount(self->equipment->data()[i]) >= amount) {
                return 1;
            }
            e = self->equipment;
        }
    }
    return 0;
}

// ---- changeLoad_174c4c.cpp ----
extern int **gWorldSingleton __attribute__((visibility("hidden")));
extern "C" void changeLoad(Ship *self, int delta) {
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

// ---- slotAvailable_1753e0.cpp ----
extern "C" int slotAvailable(Ship *self, int sort) {
    if (sort != 0 && sort != 0xc) {
        for (unsigned int i = 0; i < self->equipment->size(); i = i + 1) {
            Item *it = self->equipment->data()[i];
            if (it != 0 && Item_getSort(it) == sort) {
                return 0;
            }
        }
    }
    return 1;
}

// ---- hasModInstalled_17577c.cpp ----
extern "C" unsigned int hasModInstalled(Ship *self, int mod) {
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

// ---- getCargoValue_175270.cpp ----
extern "C" int getCargoValue(Ship *self) {
    int total = 0;
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        for (unsigned int i = 0; i < c->size(); i = i + 1) {
            Item *it = c->data()[i];
            if (it != 0) {
                int v = Item_getTotalPrice(it);
                c = self->cargo;
                total = total + v;
            }
        }
    }
    return total;
}

// ---- setMods_1756ce.cpp ----
extern "C" void setMods(Ship *self, Array<int> *mods) {
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
    return Ship_recomputeAfterSlots(self);
}
