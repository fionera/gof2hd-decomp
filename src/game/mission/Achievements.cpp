#include "game/mission/Achievements.h"
#include "game/core/Globals.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/ship/Ship.h"

uint8_t Achievements::hasMedal(int index, int value) {
    return this->medals[index] == value;
}

int *Achievements::getMedals() {
    return this->medals;
}

int *Achievements::getNewMedals() {
    return this->newMedals;
}

Achievements::~Achievements() {
    delete[] this->medals;
    this->medals = nullptr;
    delete[] this->newMedals;
    this->newMedals = nullptr;
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
    for (int i = 0; i < 0x2d; ++i)
        this->medals[i] = 0;
    this->medals[0] = 1;
    for (int i = 0; i < 0x2d; ++i)
        this->newMedals[i] = 0;
    this->hasTurretAndWeapon = 0;
    this->credits = 0;
    this->kills = 0;
    this->catches = 0;
    this->pirateKills = 0;
    this->weaponCount = 0;
    return (int) (intptr_t) & this->kills;
}

uint8_t Achievements::isEliteMedal(int index) {
    return index > 0x23;
}

Achievements::Achievements() {
    this->medals = new int[45];
    this->newMedals = new int[45];
    this->gotAllSupernovaMedals_ = 0;
    this->gotAllMedals_ = 0;
    this->gotAllGoldMedals_ = 0;
}

void Achievements::resetNewMedals() {
    for (int i = 0; i < 0x2d; ++i)
        this->newMedals[i] = 0;
    this->hasTurretAndWeapon = 0;
    this->kills = 0;
    this->catches = 0;
    this->pirateKills = 0;
    this->weaponCount = 0;
}

int Achievements::getKills() {
    return this->kills;
}

int Achievements::getCatches() {
    return this->catches;
}

int Achievements::getPirateKills() {
    return this->pirateKills;
}

void Achievements::incCatches() {
    this->catches += 1;
}

void Achievements::incKills() {
    this->kills += 1;
}

static const int gAchievementValues[135] = {0};

int Achievements::getValue(int index, int sub) {
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
    int *r3 = this->medals;
    int total = 0;
    int golds = 0;
    this->medalCount = 0;
    for (int i = 0; i < 36; ++i) {
        int v = r3[i];
        if (v != 0) {
            total += 1;
            if (v == 1) {
                this->medalCount = total;
                golds += 1;
            } else {
                this->medalCount = total;
            }
        }
    }
    r3 += 144 / 4;
    int supers = 0;
    for (int j = 0; j < 9; ++j) {
        if (r3[j] != 0)
            supers += 1;
    }
    uint8_t allSuper = (uint8_t)(supers == 9) & (uint8_t)(golds == 36);
    uint8_t allGold = (uint8_t)(golds == 36);
    this->gotAllSupernovaMedals_ = allSuper;
    this->gotAllGoldMedals_ = allGold;
    this->gotAllMedals_ = (uint8_t)(total == 36);
}

static const int gCFN_req[135] = {
    // medal  0: (never)
    0, -1, -1,
    // medal  1: hull damage rate (<=, lower is better → checked via ble)
    5, 15, 30,
    // medal  2: minedOreTypes bool array count >=
    11, 8, 5,
    // medal  3: minedCoreTypes bool array count >=
    11, 8, 5,
    // medal  4: kills >=
    250, 100, 50,
    // medal  5: missionGoodsCarried >
    200, 100, 25,
    // medal  6: minedOre >
    1000, 500, 100,
    // medal  7: minedCores >
    25, 10, 3,
    // medal  8: boughtBooze >
    1000, 100, 25,
    // medal  9: drinkTypesPossessed bool array count >=
    22, 16, 5,
    // medal 10: destroyedJunk >
    150, 100, 30,
    // medal 11: stationsVisited >=
    100, 50, 25,
    // medal 12: systemsVisited bool array count >=
    22, 10, 5,
    // medal 13: blueprints unlocked >=
    13, 6, 3,
    // medal 14: blueprints produced >0 count >=
    13, 6, 3,
    // medal 15: playingTime > threshold * 3600000
    20, 10, 5,
    // medal 16: missionCount >
    50, 25, 5,
    // medal 17: jumpgatesUsed >=
    100, 50, 10,
    // medal 18: passengersCarried >
    50, 20, 5,
    // medal 19: invisibleTime/60000 >=
    5, 3, 2,
    // medal 20: bombsUsed >
    50, 20, 5,
    // medal 21: alienJunkSalvaged >
    25, 10, 5,
    // medal 22: !hasTurretAndWeapon
    0, -1, -1,
    // medal 23: weaponCount >=
    4, 3, 2,
    // medal 24: capturedCrates >=
    500, 200, 50,
    // medal 25: maxCredits >=
    1000000, 500000, 125000,
    // medal 26: barInteractions >
    100, 50, 20,
    // medal 27: commandedWingmen >
    20, 10, 3,
    // medal 28: isEnemyWithAnyone
    1, -1, -1,
    // medal 29: asteroidsDestroyed >
    250, 150, 50,
    // medal 30: gameWon
    0, -1, -1,
    // medal 31: maxFreeCargo >
    500, 250, 100,
    // medal 32: missionsRejected >
    50, -1, -1,
    // medal 33: askedToRepeat >
    10, -1, -1,
    // medal 34: acceptedNotAskingDifficulty >
    12, -1, -1,
    // medal 35: all other medals collected
    0, -1, -1,
    // medal 36: ship max load >
    3000, -1, -1,
    // medal 37: voidStation ships count >=
    50, -1, -1,
    // medal 38: field_128 (byte) != 0
    10, -1, -1,
    // medal 39: field_118 >=
    20, -1, -1,
    // medal 40: field_120 (byte) != 0
    100, -1, -1,
    // medal 41: field_130 (byte) != 0
    3, -1, -1,
    // medal 42: field_138 (byte) != 0
    15, -1, -1,
    // medal 43: field_140 (byte) != 0
    5, -1, -1,
    // medal 44: field_148 (byte) != 0
    8, -1, -1,
};

void Achievements::checkForNewMedal(PlayerEgo *ego) {
    initCheckEquipmentAndWeapons();

    for (int m = 0; m < 45; ++m) {
        int got = 0;
        if (this->medals[m] == 1) {
            this->newMedals[m] = 0;
            continue;
        }
        for (int tier = 0; tier <= 2; ++tier) {
            int req = gCFN_req[m * 3 + tier];
            if (req < 0)
                continue;
            if (got > 0)
                break;

            // Clear the bic: use m as unsigned for switch
            unsigned um = (unsigned)m;
            if (um - 1 > 43)
                continue;

            Status *st = Globals::status;

            switch (um) {
            case 1:
                if (ego->getHullDamageRate() <= req)
                    got = tier + 1;
                break;
                case 2: {
                    Array<bool> *arr = (Array<bool> *)(st->field_94);
                    int cnt = 0;
                    for (unsigned k = 0; k < arr->size(); ++k)
                        cnt += (int)(*arr)[k];
                    if (cnt >= req)
                        got = tier + 1;
                    break;
                }
                case 3: {
                    Array<bool> *arr = (Array<bool> *)(st->field_98);
                    int cnt = 0;
                    for (unsigned k = 0; k < arr->size(); ++k)
                        cnt += (int)(*arr)[k];
                    if (cnt >= req)
                        got = tier + 1;
                    break;
                }
                case 4:
                    if (st->getKills() >= req)
                        got = tier + 1;
                    break;
                case 5:
                    if (st->field_9c > req)
                        got = tier + 1;
                    break;
                case 6:
                    if (st->field_a0 > req)
                        got = tier + 1;
                    break;
                case 7:
                    if (st->field_a4 > req)
                        got = tier + 1;
                    break;
                case 8:
                    if (st->field_a8 > req)
                        got = tier + 1;
                    break;
                case 9: {
                    Array<bool> *arr = (Array<bool> *)(st->field_ac);
                    int cnt = 0;
                    for (unsigned k = 0; k < arr->size(); ++k)
                        cnt += (int)(*arr)[k];
                    if (cnt >= req)
                        got = tier + 1;
                    break;
                }
                case 10:
                    if (st->field_b0 > req)
                        got = tier + 1;
                    break;
                case 11:
                    if (st->getStationsVisited() >= req)
                        got = tier + 1;
                    break;
                case 12: {
                    bool *p12 = st->field_b4->data_;
                    int cnt = 0, k12 = 0;
                    do {
                        cnt += (int)p12[k12++];
                    } while (k12 < 22);
                    if (cnt >= req)
                        got = tier + 1;
                    break;
                }
                case 13: {
                    int cnt = 0;
                    Array<BluePrint *> *bp = st->bluePrints;
                    for (unsigned k = 0; k < bp->size(); ++k)
                        cnt += (int)(*bp)[k]->isUnlocked();
                    if (cnt >= req)
                        got = tier + 1;
                    break;
                }
                case 14: {
                    int cnt = 0;
                    Array<BluePrint *> *bp = st->bluePrints;
                    for (unsigned k = 0; k < bp->size(); ++k)
                        if ((*bp)[k]->productionCount > 0)
                            cnt += 1;
                    if (cnt >= req)
                        got = tier + 1;
                    break;
                }
                case 15:
                    if (st->getPlayingTime() > (int64_t)((int32_t)req * (int32_t)3600000))
                        got = tier + 1;
                    break;
                case 16:
                    if (st->getMissionCount() > req)
                        got = tier + 1;
                    break;
                case 17:
                    if (st->getJumpgateUsed() >= req)
                        got = tier + 1;
                    break;
                case 18:
                    if (st->field_b8 > req)
                        got = tier + 1;
                    break;
                case 19:
                    if (reinterpret_cast<int64_t &>(st->field_c0) / 60000LL >= (int64_t)req)
                        got = tier + 1;
                    break;
                case 20:
                    if (st->field_c8 > req)
                        got = tier + 1;
                    break;
                case 21:
                    if (st->field_cc > req)
                        got = tier + 1;
                    break;
                case 22:
                    if (this->hasTurretAndWeapon == 0)
                        got = tier + 1;
                    break;
                case 23:
                    if (this->weaponCount >= req)
                        got = tier + 1;
                    break;
                case 24:
                    if (st->getCapturedCrates() >= req)
                        got = tier + 1;
                    break;
                case 25:
                    if (this->credits >= req)
                        got = tier + 1;
                    break;
                case 26:
                    if (st->field_d0 > req)
                        got = tier + 1;
                    break;
                case 27:
                    if (st->field_d4 > req)
                        got = tier + 1;
                    break;
                case 28:
                    if (st->standing->isEnemyWithAnyone())
                        got = tier + 1;
                    break;
                case 29:
                    if (st->field_d8 > req)
                        got = tier + 1;
                    break;
                case 30:
                    if (st->gameWon())
                        got = tier + 1;
                    break;
                case 31:
                    if (st->field_dc > req)
                        got = tier + 1;
                    break;
                case 32:
                    if (st->field_e0 > req)
                        got = tier + 1;
                    break;
                case 33:
                    if (st->field_e8 > req)
                        got = tier + 1;
                    break;
                case 34:
                    if (st->field_ec > req)
                        got = tier + 1;
                    break;
                case 35: {
                    int *md = this->medals;
                    int k = 0;
                    while (k <= 43 && md[k] != 0)
                        k++;
                    if (k > 43)
                        got = tier + 1;
                    break;
                }
                case 36:
                    if (st->getShip()->getMaxLoad() > req)
                        got = tier + 1;
                    break;
                case 37: {
                    Array<Ship *> *ships = st->voidStation->getShips();
                    if (ships != nullptr && ships->count >= (unsigned)req)
                        got = tier + 1;
                    break;
                }
                case 38:
                    if ((uint8_t)st->field_128 != 0)
                        got = tier + 1;
                    break;
                case 39:
                    if (st->field_118 >= req)
                        got = tier + 1;
                    break;
                case 40:
                    if (st->field_120 != 0)
                        got = tier + 1;
                    break;
                case 41:
                    if ((uint8_t)st->field_130 != 0)
                        got = tier + 1;
                    break;
                case 42:
                    if ((uint8_t)st->field_138 != 0)
                        got = tier + 1;
                    break;
                case 43:
                    if ((uint8_t)st->field_140 != 0)
                        got = tier + 1;
                    break;
                case 44:
                    if ((uint8_t)st->field_148 != 0)
                        got = tier + 1;
                    break;
                default:
                    break;
                }
            }

        int cur = this->medals[m];
        if (cur > got || cur == 0) {
            this->newMedals[m] = got;
        }
    }
}

void Achievements::applyNewMedals() {
    for (int i = 0; i < 0x2d; ++i) {
        int nv = this->newMedals[i];
        if (nv > 0) {
            int cur = this->medals[i];
            if (nv < cur || cur == 0)
                this->medals[i] = nv;
        }
    }
    countMedals();
    if (this->medalCount == 0x23) {
        this->newMedals[0x23] = 1;
        this->medals[0x23] = 1;
        countMedals();
    }
}


void Achievements::initCheckEquipmentAndWeapons() {
    int r = Globals::status->getCurrentCampaignMission();
    if (r >= 8) {
        Array<Item *> *eq = Globals::status->getShip()->getEquipment();
        int turrets = 0;
        int weapons = 0;
        if (eq != nullptr) {
            for (int i = 0; i < (int)eq->size(); ++i) {
                if ((*eq)[i] == nullptr) continue;
                if ((*eq)[i]->getType() == 4) continue;
                if ((*eq)[i]->getType() == 0) {
                    this->weaponCount += 1;
                } else if ((*eq)[i]->getType() == 3) {
                    turrets += 1;
                    continue;
                }
                weapons += 1;
            }
        }
        r = (turrets > 0) & (weapons > 0);
    } else {
        r = 1;
    }
    this->hasTurretAndWeapon = (uint8_t)r;
}

void Achievements::setMedals(int *src, int count) {
    for (int i = 0; i < count; ++i) {
        unsigned v = (unsigned) src[i];
        this->medals[i] = (v < 4) ? (int) v : 0;
    }
    for (; count <= 44; ++count)
        this->medals[count] = 0;
}
