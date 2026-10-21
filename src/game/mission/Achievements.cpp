#include "game/mission/Achievements.h"
#include "game/core/Globals.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/ship/Ship.h"

static const int gAchievementValues[135] = {0};

static const int gCFN_req[45][3] = {
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

Achievements::Achievements() {
    this->medals = new int[45];
    this->newMedals = new int[45];
    this->gotAllSupernovaMedals_ = 0;
    this->gotAllMedals_ = false;
    this->gotAllGoldMedals_ = 0;
}

Achievements::~Achievements() {
    delete[] this->medals;
    this->medals = nullptr;
    delete[] this->newMedals;
    this->newMedals = nullptr;
}

void Achievements::checkForNewMedal(PlayerEgo *ego) {
    initCheckEquipmentAndWeapons();

    for (int m = 0; m < 45; ++m) {
        int got = 0;
        if (this->medals[m] == 1) {
            this->newMedals[m] = got;
            continue;
        }
        for (int tier = 0; tier <= 2; ++tier) {
            int req = gCFN_req[m][tier];
            if (req < 0)
                continue;
            if (got > 0)
                break;

            bool reached = false;
            switch (m) {
            case 1:
                reached = ego->getHullDamageRate() <= req;
                break;
                case 2:
                case 3:
                case 9: {
                    // Number of different ores / cores / ... collected.
                    Array<bool> *found = m == 2 ? Globals::status->field_94
                                                : m == 3 ? Globals::status->field_98 : Globals::status->field_ac;
                    unsigned k = 0;
                    int cnt = 0;
                    for (; k < found->size(); ++k)
                        cnt += (int) (*found)[k];
                    reached = cnt >= req;
                    break;
                }
                case 4:
                    reached = Globals::status->getKills() >= req;
                    break;
                case 5:
                    reached = Globals::status->field_9c > req;
                    break;
                case 6:
                    reached = Globals::status->field_a0 > req;
                    break;
                case 7:
                    reached = Globals::status->field_a4 > req;
                    break;
                case 8:
                    reached = Globals::status->field_a8 > req;
                    break;
                case 10:
                    reached = Globals::status->field_b0 > req;
                    break;
                case 11:
                    reached = Globals::status->getStationsVisited() >= req;
                    break;
                case 12: {
                    // 22 visited-system flags.
                    bool *visited = Globals::status->field_b4->data_;
                    int k = 0;
                    int cnt = 0;
                    for (; k < 22; k++)
                        cnt += (int) visited[k];
                    reached = cnt >= req;
                    break;
                }
                case 13: {
                    unsigned k = 0;
                    int cnt = 0;
                    for (; k < Globals::status->bluePrints->size(); ++k)
                        cnt += (int) (*Globals::status->bluePrints)[k]->isUnlocked();
                    reached = cnt >= req;
                    break;
                }
                case 14: {
                    Array<BluePrint *> *bp = Globals::status->bluePrints;
                    unsigned k = 0;
                    int cnt = 0;
                    for (; k < bp->size(); ++k)
                        if ((*bp)[k]->productionCount > 0)
                            cnt += 1;
                    reached = cnt >= req;
                    break;
                }
                case 15:
                    reached = Globals::status->getPlayingTime() > (int64_t)((int32_t)req * (int32_t)3600000);
                    break;
                case 16:
                    reached = Globals::status->getMissionCount() > req;
                    break;
                case 17:
                    reached = Globals::status->getJumpgateUsed() >= req;
                    break;
                case 18:
                    reached = Globals::status->field_b8 > req;
                    break;
                case 19:
                    reached = reinterpret_cast<int64_t &>(Globals::status->field_c0) / 60000LL >= (int64_t)req;
                    break;
                case 20:
                    reached = Globals::status->field_c8 > req;
                    break;
                case 21:
                    reached = Globals::status->field_cc > req;
                    break;
                case 22:
                    reached = this->hasTurretAndWeapon == 0;
                    break;
                case 23:
                    reached = this->weaponCount >= req;
                    break;
                case 24:
                    reached = Globals::status->getCapturedCrates() >= req;
                    break;
                case 25:
                    reached = this->credits >= req;
                    break;
                case 26:
                    reached = Globals::status->field_d0 > req;
                    break;
                case 27:
                    reached = Globals::status->field_d4 > req;
                    break;
                case 28:
                    reached = ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemyWithAnyone();
                    break;
                case 29:
                    reached = Globals::status->field_d8 > req;
                    break;
                case 30:
                    reached = Globals::status->gameWon();
                    break;
                case 31:
                    reached = Globals::status->field_dc > req;
                    break;
                case 32:
                    reached = Globals::status->field_e0 > req;
                    break;
                case 33:
                    reached = Globals::status->field_e8 > req;
                    break;
                case 34:
                    reached = Globals::status->field_ec > req;
                    break;
                case 35: {
                    int *md = this->medals;
                    int k = 0;
                    while (k <= 43 && md[k] != 0)
                        k++;
                    reached = k > 43;
                    break;
                }
                case 36:
                    reached = Globals::status->getShip()->getMaxLoad() > req;
                    break;
                case 37: {
                    Array<Ship *> *ships = Globals::status->voidStation->getShips();
                    reached = ships != nullptr && ships->count >= (unsigned)req;
                    break;
                }
                case 38:
                    reached = (uint8_t)Globals::status->field_128 != 0;
                    break;
                case 39:
                    reached = Globals::status->field_118 >= req;
                    break;
                case 40:
                    reached = Globals::status->field_120 != 0;
                    break;
                case 41:
                    reached = (uint8_t)Globals::status->field_130 != 0;
                    break;
                case 42:
                    reached = (uint8_t)Globals::status->field_138 != 0;
                    break;
                case 43:
                    reached = (uint8_t)Globals::status->field_140 != 0;
                    break;
                case 44:
                    reached = (uint8_t)Globals::status->field_148 != 0;
                    break;
                default:
                    break;
                }
            if (reached)
                got = tier + 1;
            }

        int cur = this->medals[m];
        if (cur > got || cur == 0) {
            this->newMedals[m] = got;
        }
    }
}

void Achievements::initCheckEquipmentAndWeapons() {
    int r = Globals::status->getCurrentCampaignMission();
    if (r >= 8) {
        Array<Item *> *eq = Globals::status->getShip()->getEquipment();
        int weapons = 0;
        int turrets = 0;
        if (eq != nullptr) {
            for (unsigned i = 0; i < eq->size(); ++i) {
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

void Achievements::countMedals() {
    int *medals = this->medals;
    this->medalCount = 0;
    int golds = 0;
    for (int i = 0; i < 36; i++) {
        switch (medals[i]) {
            case 0:
                break;
            case 1:
                this->medalCount++;
                golds++;
                break;
            default:
                this->medalCount++;
                break;
        }
    }
    const int *supernovaMedals = medals + 36;
    int j = 0;
    int supernovas = 0;
    for (; j < 9; j++) {
        if (supernovaMedals[j] != 0)
            supernovas++;
    }
    int allSupernovas = supernovas == 9;
    int allGolds = golds == 36;
    this->gotAllSupernovaMedals_ = (uint8_t) (allSupernovas & allGolds);
    this->gotAllGoldMedals_ = (uint8_t) allGolds;
    this->gotAllMedals_ = this->medalCount == 36;
}

bool Achievements::gotAllMedals() {
    return this->gotAllMedals_;
}

uint8_t Achievements::gotAllGoldMedals() {
    return this->gotAllGoldMedals_;
}

uint8_t Achievements::gotAllSupernovaMedals() {
    return this->gotAllSupernovaMedals_;
}

int *Achievements::getMedals() {
    return this->medals;
}

int *Achievements::getNewMedals() {
    return this->newMedals;
}

void Achievements::incKills() {
    this->kills += 1;
}

int Achievements::getKills() {
    return this->kills;
}

void Achievements::incPirateKills() {
    this->pirateKills += 1;
}

int Achievements::getPirateKills() {
    return this->pirateKills;
}

void Achievements::resetPirateKills() {
    this->pirateKills = 0;
}

void Achievements::incCatches() {
    this->catches += 1;
}

int Achievements::getCatches() {
    return this->catches;
}

void Achievements::updateCredits(int value) {
    if (this->credits < value)
        this->credits = value;
}

void Achievements::setMedals(int *src, int count) {
    for (int i = 0; i < count; ++i) {
        unsigned v = (unsigned) src[i];
        this->medals[i] = (v < 4) ? (int) v : 0;
    }
    for (int i = count; i < 45; ++i)
        this->medals[i] = 0;
}

void Achievements::setMedal(int index, int value) {
    this->medals[index] = value;
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

void Achievements::resetNewMedals() {
    for (int i = 0; i < 0x2d; ++i)
        this->newMedals[i] = 0;
    this->hasTurretAndWeapon = 0;
    this->kills = 0;
    this->catches = 0;
    this->pirateKills = 0;
    this->weaponCount = 0;
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

uint8_t Achievements::hasMedal(int index, int value) {
    return this->medals[index] == value;
}

int Achievements::getValue(int index, int sub) {
    const int *row = gAchievementValues + index * 3;
    return row[sub - 1];
}

uint8_t Achievements::isEliteMedal(int index) {
    return index > 0x23;
}
