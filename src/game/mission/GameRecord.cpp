#include "game/mission/GameRecord.h"
#include "game/core/Globals.h"

#include "game/world/Galaxy.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/mission/Achievements.h"
#include "game/mission/BluePrint.h"
#include "game/world/Station.h"
#include "game/mission/Mission.h"
#include "game/world/SolarSystem.h"
#include "game/ship/Ship.h"

GameRecord::~GameRecord() {
}

GameRecord::GameRecord() {
    this->data = new uint8_t[0x87];
    this->rank = 0;

    this->field_0x08 = 0;
    this->field_0x0c = 0;
    this->playTime = 0;
    this->field_0x18 = 0;
    this->field_0x1c = 0;
    this->killsText = 0;
    this->field_0x24 = 0;
    this->field_0x28 = 0;
    this->field_0x2c = 0;
    this->field_0x30 = 0;
    this->field_0x34 = 0;
    this->field_0x38 = 0;
    this->field_0x3c = 0;
    this->field_0x40 = 0;
    this->field_0x44 = 0;
    this->field_0x48 = 0;
    this->field_0x4c = 0;
    this->field_0x50 = 0;
    this->field_0x54 = 0;
    this->field_0x58 = 0;
    this->field_0x5c = 0;
    this->field_0x60 = 0;
    this->field_0x64 = 0;
    this->field_0x68 = 0;
    this->field_0x6c = 0;
    this->field_0x70 = 0;
    this->field_0x74 = 0;
    this->field_0x78 = 0;
    this->field_0x7c = 0;
    this->field_0x80 = 0;
    this->field_0x84 = 0;
    this->field_0x88 = 0;
    this->field_0x8c = 0;
    this->field_0x90 = 0;

    this->field_0xb8 = 0;
    this->field_0xbc = 0;
    this->field_0xc0 = 0;
    this->field_0xc4 = 0;
    this->field_0xa8 = 0;
    this->field_0xac = 0;
    this->field_0xb0 = 0;
    this->field_0xb4 = 0;
    this->field_0x98 = 0;
    this->field_0xa0 = 0;
    this->field_0xa4 = 0;
    this->field_0x10b = 0;
    this->field_0x10c = 0;
    this->field_0x10d = 0;
    this->field_0x10e = 0;
    this->field_0x10f = 0;
    this->field_0x110 = 0;
    this->field_0x111 = 0;
    this->field_0x112 = 0;
    this->field_0x113 = 0;
    this->field_0x114 = 0;
    this->dlcRequiredFlag = 0;
    this->field_0x116 = 0;
    this->versionMismatchFlag = 0;
    this->field_0x118 = 0;
    this->field_0x119 = 0;
    this->field_0x11a = 0;
    this->hintFlags[28] = 0;
    this->hintFlags[29] = 0;
    this->hintFlags[30] = 0;
    this->hintFlags[31] = 0;
    this->field_0x104 = 0;
    this->field_0x105 = 0;
    this->field_0x106 = 0;
    this->field_0x107 = 0;
    this->field_0x108 = 0;
    this->field_0x109 = 0;
    this->field_0x10a = 0;
    this->hintFlags[12] = 0;
    this->hintFlags[13] = 0;
    this->hintFlags[14] = 0;
    this->hintFlags[15] = 0;
    this->hintFlags[16] = 0;
    this->hintFlags[17] = 0;
    this->hintFlags[18] = 0;
    this->hintFlags[19] = 0;
    this->hintFlags[20] = 0;
    this->hintFlags[21] = 0;
    this->hintFlags[22] = 0;
    this->hintFlags[23] = 0;
    this->hintFlags[24] = 0;
    this->hintFlags[25] = 0;
    this->hintFlags[26] = 0;
    this->hintFlags[27] = 0;
    this->field_0xe0 = 0;
    this->hintFlags[0] = 0;
    this->hintFlags[1] = 0;
    this->hintFlags[2] = 0;
    this->hintFlags[3] = 0;
    this->hintFlags[4] = 0;
    this->hintFlags[5] = 0;
    this->hintFlags[6] = 0;
    this->hintFlags[7] = 0;
    this->hintFlags[8] = 0;
    this->hintFlags[9] = 0;
    this->hintFlags[10] = 0;
    this->hintFlags[11] = 0;

    this->field_0xc8 = 0;
    this->shipId = 0xffffffff;
    this->field_0xcc = 0;
    this->field_0xd0 = 0;
    this->field_0xd4 = 0;
    this->field_0xd8 = 0;
    this->field_0xdc = 0;
    this->field_0x1b8 = 0;
    this->field_0x1c0 = 0;

    this->field_0x130 = 0;
    this->field_0x134 = 0;
    this->field_0x138 = 0;
    this->field_0x13c = 0;
    this->field_0x140 = nullptr;
    this->field_0x144 = 0;
    this->field_0x148 = nullptr;
    this->field_0x14c = 0;
    this->field_0x150 = 0;
    this->field_0x154 = 0;
    this->field_0x158 = 0;
    this->field_0x15c = 0;
    this->field_0x160 = 0;
    this->field_0x164 = 0;
    this->field_0x168 = 0;
    this->field_0x16c = 0;
    this->field_0x170 = 0;
    this->field_0x174 = 0;
    this->field_0x178 = 0;
    this->field_0x17c = 0;
    this->field_0x180 = 0;
    this->field_0x184 = 0;

    for (int i = 0; i < 4; i++)
        this->collectedBounties[i] = 0;

    this->field_0x1b4 = 0;
}

typedef uint32_t uint;





struct MissionFlags {
    uint8_t _pad_0x00[0x2c];
    uint32_t field_0x2c;
    uint8_t _pad_0x30[0x5];
    uint8_t dlcSkyboxFlag;
    uint8_t versionGuardFlag;
};


static const uint32_t kRecordExtendedTag = 0x6e6a78;

void GameRecord::load() {
    bool campaignLocked;
    int campaignStage;
    unsigned char *deadArray;
    Array<bool> *srcByteArray;
    Array<uint32_t> *srcVec;
    BluePrint *blueprint;
    Station *station;
    int targetStation;
    uint32_t scalar;
    int srcCount;
    int copyIndex;
    Array<uint8_t> *srcByteArray2;
    uint32_t i;
    uint32_t j;
    Status *st;

    Globals::status->resetGame();
    Globals::galaxy->setVisited((bool *) this->data, this->field_0x04);
    Globals::status->setLastXP(this->field_0x24);
    Globals::status->setCredits(this->field_0x08);
    Globals::status->setRating(this->field_0x0c);
    Globals::status->setPlayingTime(this->playTime);
    Globals::status->setKills(this->field_0x18);
    Globals::status->setMissionCount(this->field_0x1c);
    Globals::status->setLevel(this->killsText);
    Globals::status->setLastXP(this->field_0x24);
    Globals::status->setGoodsProduced(this->field_0x28);
    Globals::status->setPirateKills(this->field_0x2c);
    Globals::status->setJumpgateUsed(this->field_0x30);
    Globals::status->setCapturedCrates(this->field_0x34);
    Globals::status->setBoughtEquipment(this->field_0x38);
    Globals::status->setStationsVisited(this->field_0x3c);
    st = Globals::status;
    st->field_80 = this->field_0x44;
    st->field_7c = this->field_0x48;
    st->field_84 = this->field_0x4c;
    st->field_88 = this->field_0x50;
    // Saves from older versions are capped to the campaign part they contain.
    if (!this->dlcRequiredFlag && !this->versionMismatchFlag && this->field_0x40 > 45)
        this->field_0x40 = 45;
    if (!this->versionMismatchFlag && this->field_0x40 > 84)
        this->field_0x40 = 84;
    if ((this->dlcRequiredFlag || this->versionMismatchFlag) && this->field_0x40 == 46 &&
        ((Mission *) this->field_0x58)->isEmpty())
        this->field_0x40 = 45;
    Globals::status->setCurrentCampaignMission(this->field_0x40);
    this->hintFlags[30] = this->field_0x40 > 45;
    this->field_0x119 = this->field_0x40 > 85;
    Globals::status->setFreelanceMission((Mission *) this->field_0x54);
    Globals::status->setCampaignMission((Mission *) this->field_0x58);
    Globals::status->setStationStack((Array<Station *> *) this->field_0x5c);

    // Repair campaign states broken by earlier versions.
    if (this->field_0x40 == 35 && ((Mission *) this->field_0x58)->getTargetStation() != 29)
        Globals::status->setCampaignMission(new Mission(11, 0, 29));
    if (this->field_0x40 == 29) {
        this->field_0x44 = 91;
        this->field_0x48 = 18;
        Globals::status->setCurrentCampaignMission(28);
        Globals::status->setCampaignMission(new Mission(4, 0, 91));
    }
    if (this->field_0x40 == 25) {
        this->field_0x44 = 48;
        this->field_0x48 = 9;
        Globals::status->setCurrentCampaignMission(24);
        Globals::status->setCampaignMission(new Mission(4, 0, 48));
    }
    if (this->field_0x40 == 41) {
        Globals::status->setCurrentCampaignMission(39);
        Globals::status->setCampaignMission(new Mission(11, 0, 30));
    }
    if ((this->versionMismatchFlag || this->field_0x118) && this->field_0x40 == 86 &&
        ((Mission *) this->field_0x58)->getTargetStation() != 100) {
        Globals::hints[49] = 1;
        Globals::status->setCampaignMission(new Mission(11, 0, 100));
    }
    if ((this->versionMismatchFlag || this->field_0x118) && this->field_0x40 == 87 &&
        ((Mission *) this->field_0x58)->getTargetStation() != 10) {
        Globals::hints[49] = 1;
        Globals::status->setCampaignMission(new Mission(4, 0, 10));
    }
    if ((this->versionMismatchFlag || this->field_0x118) && this->field_0x40 == 88 &&
        ((Mission *) this->field_0x58)->getTargetStation() != 10) {
        Globals::hints[49] = 1;
        Globals::status->setCampaignMission(new Mission(11, 0, 10));
    }
    if ((this->versionMismatchFlag || this->field_0x118) && this->field_0x40 == 89 &&
        ((Mission *) this->field_0x58)->getTargetStation() != 10) {
        Globals::hints[49] = 1;
        Globals::status->setCurrentCampaignMission(86);
        Globals::status->setCampaignMission(new Mission(11, 0, 100));
    }
    if ((this->versionMismatchFlag || this->field_0x118) &&
        ((this->field_0x40 == 91 && ((Mission *) this->field_0x58)->getTargetStation() != 110) ||
         ((unsigned int) (this->field_0x40 - 91) <= 12 && !((Array<uint8_t> *) this->field_0x160)->data_[27]))) {
        Globals::hints[49] = 1;
        this->field_0x40 = 86;
        Globals::status->setCurrentCampaignMission(this->field_0x40);
        Globals::status->setCampaignMission(new Mission(11, 0, 100));
    }
    ((Station *) Globals::status->voidStation)->setItems((Array<Item *> *) this->field_0x180, true);
    ((Station *) Globals::status->voidStation)->setShips((Array<Ship *> *) this->field_0x184, true);
    st = Globals::status;
    delete st->field_94;
    st = Globals::status;
    st->field_94 = (Array<bool> *) this->field_0x68;
    delete st->field_98;
    st = Globals::status;
    st->field_98 = (Array<bool> *) this->field_0x6c;
    delete st->field_90;
    st = Globals::status;
    st->field_90 = (Array<int> *) this->field_0x70;
    st->field_10c = this->field_0xd0;

    MissionFlags *missionFlags = (MissionFlags *) Globals::options;
    st->field_110 = this->field_0xd4;
    st->field_0x111 = this->field_0xdc;
    if (missionFlags->versionGuardFlag == '\0') {
        scalar = this->field_0xd8;
    } else {
        scalar = 3;
    }
    st->field_114 = scalar;

    st->field_9c = this->field_0x74;
    st->field_a0 = this->field_0x78;
    st->field_a4 = this->field_0x7c;
    st->field_a8 = this->field_0x80;
    delete st->field_ac;
    st = Globals::status;
    st->field_ac = (Array<bool> *) this->field_0x84;
    st->field_b0 = this->field_0x88;
    srcByteArray = (Array<bool> *) this->field_0x8c;
    srcCount = srcByteArray->count;
    for (copyIndex = 0; srcCount != copyIndex; copyIndex = copyIndex + 1) {
        st->field_b4->data_[copyIndex] = srcByteArray->data_[copyIndex];
    }
    st->field_b8 = this->field_0x90;
    reinterpret_cast<int64_t &>(st->field_c0) = this->field_0x98;

    st->field_c8 = this->field_0xa0;
    st->field_cc = this->field_0xa4;
    st->field_d0 = this->field_0xa8;
    st->field_d4 = this->field_0xac;
    st->field_d8 = this->field_0xb0;
    st->field_dc = this->field_0xb4;
    st->field_e0 = this->field_0xb8;
    st->field_e4 = this->field_0xbc;
    st->field_e8 = this->field_0xc0;
    st->field_ec = this->field_0xc4;
    Globals::achievements->init();
    if ((int *) this->field_0x60 != (int *) 0x0) {
        Globals::achievements->setMedals((int *) this->field_0x60, this->field_0x64);
    }
    Globals::status->setShip((Ship *) this->field_0x130);
    if (Globals::status->dlc1Won()) {
        Item *dlcItem = Globals::status->getShip()->getFirstEquipmentOfSort(18);
        if (dlcItem == nullptr)
            dlcItem = Globals::status->getShip()->getCargo(85);
        if (dlcItem != nullptr)
            dlcItem->setUnsaleable(false);
    }
    if (Globals::status->gameWon()) {
        Array<Item *> *cargo = Globals::status->getShip()->getCargo();
        if (cargo != nullptr) {
            for (i = 0; i < cargo->size(); i++) {
                if ((*cargo)[i] != nullptr && (*cargo)[i]->getIndex() == 131)
                    (*cargo)[i]->setUnsaleable(false);
            }
        }
    }
    if (this->field_0x40 > 121) {
        Array<Item *> *cargo = Globals::status->getShip()->getCargo();
        if (cargo != nullptr) {
            for (i = 0; i < cargo->size(); i++) {
                if ((*cargo)[i] != nullptr && (*cargo)[i]->getIndex() == 209)
                    (*cargo)[i]->setUnsaleable(false);
            }
        }
    }
    // Blueprints whose station lies in a system without routes are moved to station 10.
    if (this->field_0x140 != nullptr) {
        for (unsigned int k = 0; k < (*this->field_0x140).size(); k++) {
            if ((*this->field_0x140)[k] == nullptr ||
                ((*this->field_0x140)[k]->getIndex() != 223 && (*this->field_0x140)[k]->getIndex() != 210) ||
                (*this->field_0x140)[k]->isEmpty())
                continue;
            Station *bpStation = (Station *) (intptr_t) Globals::galaxy->getStation((*this->field_0x140)[k]->getStationIndex());
            if (bpStation == nullptr)
                continue;
            if (((SolarSystem *) (intptr_t) Globals::galaxy->getSystem(bpStation->getSystem()))->getRoutes() == nullptr) {
                (*this->field_0x140)[k]->stationIndex = 10;
                delete bpStation;
                bpStation = (Station *) (intptr_t) Globals::galaxy->getStation(10);
                (*this->field_0x140)[k]->stationName = bpStation->getName();
            }
            delete bpStation;
        }
    }
    st = Globals::status;
    st->field_8c = this->field_0x134;
    st->setStation((Station *) this->field_0x138);
    Globals::status->setMission(Mission::empty);
    Globals::status->standing = (Standing *) this->field_0x13c;
    for (i = 0; i < this->field_0x140->size(); i++)
        (*Globals::status->bluePrints)[i] = (*this->field_0x140)[i];
    Globals::status->pendingProducts = (Array<PendingProduct *> *) this->field_0x144;
    for (i = 0; i < this->field_0x148->size(); i++)
        (*Globals::status->agents)[i] = (*this->field_0x148)[i];
    st = Globals::status;
    st->wingmen = (Array<String *> *) this->field_0x14c;
    st->field_0x2c = this->field_0x150;
    st->fadeValue = this->field_0x154;
    reinterpret_cast<int32_t &>(st->_byte_0x28) = this->field_0x158;
    reinterpret_cast<int32_t &>(st->_byte_0x34) = this->field_0x15c;
    srcByteArray2 = (Array<uint8_t> *) this->field_0x160;
    srcCount = srcByteArray2->count;
    for (copyIndex = 0; srcCount != copyIndex; copyIndex = copyIndex + 1) {
        ((Array<uint8_t> *) st->systemVisibilities)->data_[copyIndex] =
                srcByteArray2->data_[copyIndex];
    }
    uint8_t *visBuf = ((Array<uint8_t> *) st->systemVisibilities)->data_;
    if (missionFlags->dlcSkyboxFlag != '\0') {
        visBuf[0x19] = 1;
    }
    visBuf[0x1a] = 1;
    srcVec = (Array<uint32_t> *) this->field_0x164;
    for (i = 0; i < srcVec->count; i = i + 1) {
        ((Array<uint32_t> *) st->field_0x3c)->data_[i] = srcVec->data_[i];
    }
    srcVec = (Array<uint32_t> *) this->field_0x168;
    for (i = 0; i < srcVec->count; i = i + 1) {
        ((Array<uint32_t> *) st->field_0x40)->data_[i] = srcVec->data_[i];
    }
    srcVec = (Array<uint32_t> *) this->field_0x16c;
    for (i = 0; i < srcVec->count; i = i + 1) {
        ((Array<uint32_t> *) st->field_0x44)->data_[i] = srcVec->data_[i];
    }
    srcVec = (Array<uint32_t> *) this->field_0x170;
    for (i = 0; i < srcVec->count; i = i + 1) {
        ((Array<uint32_t> *) st->field_0x48)->data_[i] = srcVec->data_[i];
    }
    {
        Array<uint8_t> *dstByteVec = (Array<uint8_t> *) st->field_54;
        st->field_4c = (Array<bool> *) this->field_0x174;
        srcByteArray2 = (Array<uint8_t> *) this->field_0x178;
        i = srcByteArray2->count;
        if (i >= dstByteVec->count) {
            for (j = 0; i != j; j = j + 1) {
                dstByteVec->data_[j] = srcByteArray2->data_[j];
            }
        }
    }

    Globals::hints[8] = this->hintFlags[0];
    Globals::hints[9] = this->hintFlags[1];
    Globals::hints[10] = this->hintFlags[2];
    Globals::hints[11] = this->hintFlags[3];
    Globals::hints[12] = this->hintFlags[4];
    Globals::hints[13] = this->hintFlags[5];
    Globals::hints[14] = this->hintFlags[6];
    Globals::hints[15] = this->hintFlags[7];
    Globals::hints[16] = this->hintFlags[8];
    Globals::hints[17] = this->hintFlags[9];
    Globals::hints[18] = this->hintFlags[10];
    Globals::hints[19] = this->hintFlags[11];
    Globals::hints[20] = this->hintFlags[12];
    Globals::hints[21] = this->hintFlags[13];
    Globals::hints[22] = this->hintFlags[14];
    Globals::hints[23] = this->hintFlags[15];
    missionFlags->field_0x2c = reinterpret_cast<uint32_t &>(this->rank);
    scalar = this->field_0xcc;
    st->field_0x100 = this->field_0xc8;
    st->field_0x104 = scalar;
    Globals::hints[24] = this->hintFlags[16];
    Globals::hints[25] = this->hintFlags[17];
    Globals::hints[26] = this->hintFlags[18];
    Globals::hints[27] = this->hintFlags[19];
    Globals::hints[28] = this->hintFlags[20];
    Globals::hints[29] = this->hintFlags[21];
    Globals::hints[30] = this->hintFlags[22];
    Globals::hints[31] = this->hintFlags[23];
    Globals::hints[32] = this->hintFlags[24];
    Globals::hints[33] = this->hintFlags[25];
    Globals::hints[34] = this->hintFlags[26];
    Globals::hints[35] = this->hintFlags[27];
    Globals::hints[36] = this->hintFlags[28];
    Globals::hints[37] = this->hintFlags[29];
    Globals::hints[38] = this->hintFlags[30];
    Globals::hints[39] = this->hintFlags[31];
    if (this->field_0x1bc == kRecordExtendedTag) {
        Globals::hints[41] = this->field_0x105;
        Globals::hints[40] = this->field_0x104;
        Globals::hints[42] = this->field_0x106;
        Globals::hints[43] = this->field_0x107;
        Globals::hints[44] = this->field_0x108;
        Globals::hints[45] = this->field_0x109;
        Globals::hints[46] = this->field_0x10a;
        Globals::hints[47] = this->field_0x10b;
        Globals::hints[48] = this->field_0x10c;
        st->field_178 = this->field_0x1b8;
        Globals::hints[49] = this->field_0x119;
        Globals::hints[50] = this->field_0x11a;
        Globals::hints[51] = this->field_0x10d;
        Globals::hints[52] = this->field_0x10e;
        Globals::hints[53] = this->field_0x10f;
        Globals::hints[54] = this->field_0x110;
        Globals::hints[55] = this->field_0x111;
        Globals::hints[56] = this->field_0x113;
        Globals::hints[57] = this->field_0x112;
        Globals::hints[58] = this->field_0x114;
        st->field_58 = (Array<bool> *) this->field_0x17c;
        srcVec = (Array<uint32_t> *) this->field_0x1b4;
        if ((srcVec != (Array<uint32_t> *) 0x0) && (i = srcVec->count, i != 0)) {
            for (j = 0; j < i; j = j + 1) {
                ((Array<uint32_t> *) Globals::status->wanted)->data_[j] = srcVec->data_[j];
                srcVec = (Array<uint32_t> *) this->field_0x1b4;
                i = srcVec->count;
            }
        }
        st = Globals::status;
        st->collectedBounties[0] = this->collectedBounties[0];
        st->collectedBounties[1] = this->collectedBounties[1];
        st->collectedBounties[2] = this->collectedBounties[2];
        st->collectedBounties[3] = this->collectedBounties[3];
        station = Globals::status->getStation();
        if (station->getIndex() == 0x6c) {
            Station *stStation = Globals::status->getStation();
            Array<Ship *> *stationShips = (Array<Ship *> *) stStation->getShips();
            if (stationShips != (Array<Ship *> *) 0x0) {
                Array<Ship *> *recShips = (Array<Ship *> *) this->field_0x184;
                for (i = 0; i < stationShips->size(); i = i + 1) {
                    Array<int> *mods = recShips->data_[i]->getMods();
                    if (mods != (Array<int> *) 0x0) {
                        for (j = 0; j < mods->size(); j = j + 1) {
                            (*stationShips)[i]->addMod((*mods)[j]);
                        }
                    }
                }
            }
        }
        st = Globals::status;
        st->field_118 = this->field_0xe0;
        st->field_0x17c = this->field_0x1c0;
    }
}
