#include "gof2/Achievements.h"


extern "C" void Achievements_onAllMedals(Achievements *self);
extern "C" int Status_getCurrentCampaignMission(Status *s);
extern "C" Ship *Status_getShip(Status *s);
extern "C" unsigned *Ship_getEquipment(Ship *ship);
extern "C" int Item_getType(Item *it);

// ---- hasMedal_157008.cpp ----
uint8_t Achievements::hasMedal(int index, int value) {
    Achievements *self = this;
    return self->medals[index] == value;
}

// ---- _Achievements_1568f2.cpp ----

// Achievements::~Achievements() — delete[] the two arrays (with null checks),
// zeroing each pointer.
Achievements::~Achievements()
{
    if (this->medals != 0)
        operator delete[](this->medals);
    this->medals = 0;
    if (this->newMedals != 0)
        operator delete[](this->newMedals);
    this->newMedals = 0;
}

// ---- gotAllSupernovaMedals_156ed2.cpp ----
uint8_t Achievements::gotAllSupernovaMedals() {
    Achievements *self = this;
    return self->field_0x22;
}

// ---- gotAllMedals_156ec6.cpp ----
uint8_t Achievements::gotAllMedals() {
    Achievements *self = this;
    return self->field_0x20;
}

// ---- updateCredits_156f0a.cpp ----
void Achievements::updateCredits(int value) {
    Achievements *self = this;
    if (self->credits < value)
        self->credits = value;
}

// ---- setMedal_156f4c.cpp ----
void Achievements::setMedal(int index, int value) {
    Achievements *self = this;
    self->medals[index] = value;
}

// ---- incPirateKills_156eec.cpp ----
void Achievements::incPirateKills() {
    Achievements *self = this;
    self->pirateKills += 1;
}

// ---- init_156f54.cpp ----
int Achievements::init() {
    Achievements *self = this;
    int *medals = self->medals;
    int i;
    for (i = 0; i != 0x2d; i = i + 1)
        medals[i] = 0;
    *medals = 1;
    int *newMedals = self->newMedals;
    int j;
    for (j = 0; j != 0x2d; j = j + 1)
        newMedals[j] = 0;
    self->hasTurretAndWeapon = 0;
    self->credits = 0;
    self->kills = 0;
    self->catches = 0;
    self->pirateKills = 0;
    self->field_0x14 = 0;
    return (int)(intptr_t)((char *)self + 0x8);
}

// ---- isEliteMedal_157034.cpp ----
uint8_t Achievements::isEliteMedal(int index) {
    Achievements *self = this;
    return index > 0x23;
}

// ---- Achievements_1568d0.cpp ----

// Achievements::Achievements() — allocates two 45-int (0xb4) arrays.
Achievements::Achievements()
{
    this->medals = (int *)operator new[](0xb4);
    this->newMedals = (int *)operator new[](0xb4);
    this->field_0x22 = 0;
    this->field_0x20 = 0;
}

// ---- resetNewMedals_156f8e.cpp ----
void Achievements::resetNewMedals() {
    Achievements *self = this;
    int *newMedals = self->newMedals;
    int i;
    for (i = 0; i != 0x2d; i = i + 1)
        newMedals[i] = 0;
    self->hasTurretAndWeapon = 0;
    self->kills = 0;
    self->catches = 0;
    self->pirateKills = 0;
    self->field_0x14 = 0;
}

// ---- incCatches_156efe.cpp ----
void Achievements::incCatches() {
    Achievements *self = this;
    self->catches += 1;
}

// ---- incKills_156ee0.cpp ----
void Achievements::incKills() {
    Achievements *self = this;
    self->kills += 1;
}

// ---- getValue_157018.cpp ----
// Static medal-threshold table embedded in the binary, addressed PC-relative.
// getValue returns table[index*3 + sub - 1].
extern const int gAchievementValues[] __attribute__((visibility("hidden")));

int Achievements::getValue(int index, int sub) {
    Achievements *self = this;
    (void)self;
    const int *row = gAchievementValues + index * 3;
    return row[sub - 1];
}

// ---- resetPirateKills_156ef8.cpp ----
void Achievements::resetPirateKills() {
    Achievements *self = this;
    self->pirateKills = 0;
}

// ---- gotAllGoldMedals_156ecc.cpp ----
uint8_t Achievements::gotAllGoldMedals() {
    Achievements *self = this;
    return self->field_0x21;
}

// ---- countMedals_156e48.cpp ----
void Achievements::countMedals() {
    Achievements *self = this;
    int *medals = self->medals;
    int total = 0;
    int golds = 0;
    self->medalCount = 0;
    int i;
    for (i = 0; i != 0x24; i = i + 1) {
        int v = medals[i];
        if (v != 0) {
            total = total + 1;
            if (v == 1) {
                self->medalCount = total;
                golds = golds + 1;
            } else {
                self->medalCount = total;
            }
        }
    }
    int j;
    int supers = 0;
    for (j = 0; j != 9; j = j + 1) {
        if (medals[0x24 + j] != 0)
            supers = supers + 1;
    }
    uint8_t allGold = (golds == 0x24);
    self->field_0x22 = (supers == 9) & allGold;
    self->field_0x21 = allGold;
    self->field_0x20 = (total == 0x24);
}

// ---- checkForNewMedal_156914.cpp ----
// NEAR / RESISTANT: Achievements::checkForNewMedal(PlayerEgo*) is a ~922-byte function
// driven by a 44-entry `tbh` jump table (one case per medal kind), with 64-bit SBORROW
// arithmetic, many PC-relative global derefs and cross-class calls (Status/PlayerEgo/
// Standing/Ship/Station/BluePrint). The exact `tbh` table layout, per-case register
// allocation and block placement are compiler-internal and not reproducible byte-exact
// under -Oz. The documented outer-loop skeleton (per-medal requirement check feeding the
// newMedals array) is modelled below.

struct PlayerEgo;


// pc-rel base for the per-medal requirement table (index*0xc + sub*4).
extern const int gCFN_req[] __attribute__((visibility("hidden")));

void Achievements::checkForNewMedal(PlayerEgo *ego) {
    Achievements *self = this;
    (void)ego;
    ((Achievements *)(self))->initCheckEquipmentAndWeapons();

    for (unsigned m = 0; m != 0x2d; m = m + 1) {
        int got = 0;
        int *medals = self->medals;
        if (medals[m] != 1) {
            for (unsigned tier = 0; tier < 3; tier = tier + 1) {
                int req = gCFN_req[m * 3 + tier];
                if (req < 0)
                    break;
                if (got > 0)
                    break;
                // per-kind requirement test (the 44-case switch) determines `got`.
            }
            int cur = medals[m];
            if (got < cur || cur == 0)
                self->newMedals[m] = got;
        } else {
            self->newMedals[m] = got;
        }
    }
}

// ---- applyNewMedals_156fb0.cpp ----
// Trailing tail-call through a global callback (resolved PC-relative in the target).

void Achievements::applyNewMedals() {
    Achievements *self = this;
    int *newMedals = self->newMedals;
    int i;
    for (i = 0; i != 0x2d; i = i + 1) {
        int nv = newMedals[i];
        if (0 < nv) {
            int *medals = self->medals;
            int cur = medals[i];
            if (nv >= cur) {
                if (cur == 0)
                    medals[i] = nv;
            } else {
                medals[i] = nv;
            }
        }
    }
    ((Achievements *)(self))->countMedals();
    if (self->medalCount == 0x23) {
        self->newMedals[0x23] = 1;
        self->medals[0x23] = 1;
        Achievements_onAllMedals(self);
    }
}

// ---- initCheckEquipmentAndWeapons_156da4.cpp ----
struct Status;
struct Ship;
struct Item;


// Status singleton holder: pc-rel -> holder; *holder is the Status object.
extern void *const gAchStatusHolder __attribute__((visibility("hidden")));

void Achievements::initCheckEquipmentAndWeapons() {
    Achievements *self = this;
    uint8_t result;
    if (Status_getCurrentCampaignMission(*(Status **)gAchStatusHolder) < 8) {
        result = 1;
    } else {
        Ship *ship = Status_getShip(*(Status **)gAchStatusHolder);
        unsigned *eq = Ship_getEquipment(ship);
        int weapons = 0;
        int turrets = 0;
        if (eq != 0) {
            for (unsigned i = 0; i < *eq; i = i + 1) {
                Item *it = ((Item **)eq[1])[i];
                if (it != 0 && Item_getType(it) != 4) {
                    if (Item_getType(((Item **)eq[1])[i]) == 0) {
                        self->field_0x14 += 1;
                    } else if (Item_getType(((Item **)eq[1])[i]) == 3) {
                        turrets = turrets + 1;
                        continue;
                    }
                    weapons = weapons + 1;
                }
            }
        }
        result = (0 < turrets) & (0 < weapons);
    }
    self->hasTurretAndWeapon = result;
}

// ---- setMedals_156f14.cpp ----
void Achievements::setMedals(int *src, int count) {
    Achievements *self = this;
    int i = 0;
    for (i = 0; i < count; i = i + 1) {
        unsigned v = (unsigned)src[i];
        unsigned *medals = (unsigned *)self->medals;
        if (v < 4)
            medals[i] = v;
        else
            medals[i] = 0;
    }
    for (; count < 0x2d; count = count + 1) {
        self->medals[count] = 0;
    }
}
