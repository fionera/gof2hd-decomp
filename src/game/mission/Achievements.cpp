#include "gof2/game/mission/Achievements.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/ship/Ship.h"

uint8_t Achievements::hasMedal(int index, int value) {
    return this->medals[index] == value;
}

// ---- getMedals_166ed8 / getNewMedals_166edc ----
// Simple accessors for the two 45-int medal arrays.
int *Achievements::getMedals() {
    return this->medals;
}

int *Achievements::getNewMedals() {
    return this->newMedals;
}

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

uint8_t Achievements::gotAllSupernovaMedals() {
    return this->gotAllSupernovaMedals_;
}

uint8_t Achievements::gotAllMedals() {
    return this->gotAllMedals_;
}

void Achievements::updateCredits(int value) {
    if (this->credits < value)
        this->credits = value;
}

void Achievements::setMedal(int index, int value) {
    this->medals[index] = value;
}

void Achievements::incPirateKills() {
    this->pirateKills += 1;
}

int Achievements::init() {
    int *medals = this->medals;
    int i;
    for (i = 0; i != 0x2d; i = i + 1)
        medals[i] = 0;
    *medals = 1;
    int *newMedals = this->newMedals;
    int j;
    for (j = 0; j != 0x2d; j = j + 1)
        newMedals[j] = 0;
    this->hasTurretAndWeapon = 0;
    this->credits = 0;
    this->kills = 0;
    this->catches = 0;
    this->pirateKills = 0;
    this->field_0x14 = 0;
    return (int)(intptr_t)&this->kills;  // address of member at +0x8 (int cast: pre-existing 64-bit truncation, intentional)
}

uint8_t Achievements::isEliteMedal(int index) {
    return index > 0x23;
}

// Achievements::Achievements() — allocates two 45-int (0xb4) arrays.
Achievements::Achievements()
{
    this->medals = (int *)operator new[](0xb4);
    this->newMedals = (int *)operator new[](0xb4);
    this->gotAllSupernovaMedals_ = 0;
    this->gotAllMedals_ = 0;
}

void Achievements::resetNewMedals() {
    int *newMedals = this->newMedals;
    int i;
    for (i = 0; i != 0x2d; i = i + 1)
        newMedals[i] = 0;
    this->hasTurretAndWeapon = 0;
    this->kills = 0;
    this->catches = 0;
    this->pirateKills = 0;
    this->field_0x14 = 0;
}

void Achievements::incCatches() {
    this->catches += 1;
}

void Achievements::incKills() {
    this->kills += 1;
}

// Static medal-threshold table embedded in the binary, addressed PC-relative.
// getValue returns table[index*3 + sub - 1].
extern const int gAchievementValues[] __attribute__((visibility("hidden")));

int Achievements::getValue(int index, int sub) {
    (void)this;
    const int *row = gAchievementValues + index * 3;
    return row[sub - 1];
}

void Achievements::resetPirateKills() {
    this->pirateKills = 0;
}

uint8_t Achievements::gotAllGoldMedals() {
    return this->gotAllGoldMedals_;
}

void Achievements::countMedals() {
    int *medals = this->medals;
    int total = 0;
    int golds = 0;
    this->medalCount = 0;
    int i;
    for (i = 0; i != 0x24; i = i + 1) {
        int v = medals[i];
        if (v != 0) {
            total = total + 1;
            if (v == 1) {
                this->medalCount = total;
                golds = golds + 1;
            } else {
                this->medalCount = total;
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
    this->gotAllSupernovaMedals_ = (supers == 9) & allGold;
    this->gotAllGoldMedals_ = allGold;
    this->gotAllMedals_ = (total == 0x24);
}

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
    (void)ego;
    ((Achievements *)(this))->initCheckEquipmentAndWeapons();

    for (unsigned m = 0; m != 0x2d; m = m + 1) {
        int got = 0;
        int *medals = this->medals;
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
                this->newMedals[m] = got;
        } else {
            this->newMedals[m] = got;
        }
    }
}

// Trailing tail-call through a global callback (resolved PC-relative in the target).

void Achievements::applyNewMedals() {
    int *newMedals = this->newMedals;
    int i;
    for (i = 0; i != 0x2d; i = i + 1) {
        int nv = newMedals[i];
        if (0 < nv) {
            int *medals = this->medals;
            int cur = medals[i];
            if (nv >= cur) {
                if (cur == 0)
                    medals[i] = nv;
            } else {
                medals[i] = nv;
            }
        }
    }
    ((Achievements *)(this))->countMedals();
    if (this->medalCount == 0x23) {
        this->newMedals[0x23] = 1;
        this->medals[0x23] = 1;
        this->countMedals();   // recount so medalCount includes the "all medals" flag
                               // (the decomp mislabeled this veneer'd tail-call as "onAllMedals")
    }
}

struct Status;
struct Ship;
struct Item;

// Status singleton holder: pc-rel -> holder; *holder is the Status object.
extern void *const gAchStatusHolder __attribute__((visibility("hidden")));

void Achievements::initCheckEquipmentAndWeapons() {
    uint8_t result;
    if (((Status *)(*(Status **)gAchStatusHolder))->getCurrentCampaignMission() < 8) {
        result = 1;
    } else {
        Ship *ship = ((Status *)(*(Status **)gAchStatusHolder))->getShip();
        unsigned *eq = (unsigned *)((Ship*)(ship))->getEquipment();
        int weapons = 0;
        int turrets = 0;
        if (eq != 0) {
            for (unsigned i = 0; i < *eq; i = i + 1) {
                Item *it = ((Item **)eq[1])[i];
                if (it != 0 && ((Item *)(it))->getType() != 4) {
                    if (((Item *)(((Item **)eq[1])[i]))->getType() == 0) {
                        this->field_0x14 += 1;
                    } else if (((Item *)(((Item **)eq[1])[i]))->getType() == 3) {
                        turrets = turrets + 1;
                        continue;
                    }
                    weapons = weapons + 1;
                }
            }
        }
        result = (0 < turrets) & (0 < weapons);
    }
    this->hasTurretAndWeapon = result;
}

void Achievements::setMedals(int *src, int count) {
    int i = 0;
    for (i = 0; i < count; i = i + 1) {
        unsigned v = (unsigned)src[i];
        unsigned *medals = (unsigned *)this->medals;
        if (v < 4)
            medals[i] = v;
        else
            medals[i] = 0;
    }
    for (; count < 0x2d; count = count + 1) {
        this->medals[count] = 0;
    }
}
