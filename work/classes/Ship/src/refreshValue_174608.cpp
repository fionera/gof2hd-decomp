#include "class.h"
extern "C" int Item_getSort(Item *it);
extern "C" int Item_getAttribute(Item *it, int which);
extern "C" int Item_getIndex(Item *it);
extern "C" int Item_getTotalPrice(Item *it);
extern "C" int Status_getStanding(void);
extern "C" void Standing_setPlayerSignatureRace(void *st, int race);
extern "C" int getCargoValue(Ship *self);
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
