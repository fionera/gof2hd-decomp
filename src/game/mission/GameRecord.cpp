#include "game/mission/GameRecord.h"
#include "platform/libc.h"
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
    // String members are destroyed last-constructed-first.
}

GameRecord::GameRecord() {
    char *t = (char *) this;

    // Visited-systems bitmap (0x87 entries).
    this->data = new uint8_t[0x87];
    *(uint32_t *) (t + 0x11c) = 0;

    // Zero the record payload, then seed the few non-zero defaults.
    memset(t + 0x8, 0, 0x8c);

    *(uint32_t *) (t + 0xb8) = 0;
    *(uint32_t *) (t + 0xbc) = 0;
    *(uint32_t *) (t + 0xc0) = 0;
    *(uint32_t *) (t + 0xc4) = 0;
    *(uint32_t *) (t + 0xa8) = 0;
    *(uint32_t *) (t + 0xac) = 0;
    *(uint32_t *) (t + 0xb0) = 0;
    *(uint32_t *) (t + 0xb4) = 0;
    *(uint32_t *) (t + 0x98) = 0;
    *(uint32_t *) (t + 0x9c) = 0;
    *(uint32_t *) (t + 0xa0) = 0;
    *(uint32_t *) (t + 0xa4) = 0;
    *(uint32_t *) (t + 0x10b) = 0;
    *(uint32_t *) (t + 0x10f) = 0;
    *(uint32_t *) (t + 0x113) = 0;
    *(uint32_t *) (t + 0x117) = 0;
    *(uint32_t *) (t + 0x100) = 0;
    *(uint32_t *) (t + 0x104) = 0;
    *(uint32_t *) (t + 0x108) = 0;
    *(uint32_t *) (t + 0x10c) = 0;
    *(uint32_t *) (t + 0xf0) = 0;
    *(uint32_t *) (t + 0xf4) = 0;
    *(uint32_t *) (t + 0xf8) = 0;
    *(uint32_t *) (t + 0xfc) = 0;
    *(uint32_t *) (t + 0xe0) = 0;
    *(uint32_t *) (t + 0xe4) = 0;
    *(uint32_t *) (t + 0xe8) = 0;
    *(uint32_t *) (t + 0xec) = 0;

    *(uint32_t *) (t + 0x1a0) = 0xffffffff;
    *(uint32_t *) (t + 0xc8) = 0;
    *(uint32_t *) (t + 0xcc) = 0;
    *(uint32_t *) (t + 0xd0) = 0;
    *(uint8_t *) (t + 0xd4) = 0;
    *(uint32_t *) (t + 0xd8) = 0;
    *(uint8_t *) (t + 0xdc) = 0;
    *(uint32_t *) (t + 0x1b8) = 0;
    *(uint8_t *) (t + 0x1c0) = 0;

    memset(t + 0x130, 0, 0x58);

    for (int i = 0x69; i != 0x6d; i = i + 1)
        *(uint32_t *) (t + i * 4) = 0;

    *(uint32_t *) (t + 0x1b4) = 0;
}

// structural translation of the recovered deserializer: the packed save image is
// read positionally from this record, and the values are written into the live
// Status / Galaxy / Achievements singletons (resolved through the engine's global
// singleton tables). The singleton field writes that have no modelled accessor are
// kept as positional stores against the partially-modelled singleton layouts.

typedef uint32_t uint;

extern "C" {
long Array_dtor(void *self);

long Station_getIndex(...);

long Station_getSystem(...);
}

// The mission-system singletons below are reached through global pointer-to-pointer
// slots in the binary's .bss that this port does not yet model with a typed header.
// load() reads the live pointer out of each slot and pokes a handful of flag bytes;
// the slots are addressed by their resolved absolute .bss addresses (the original
// PC-relative DAT_ loads) and kept as RAWREAD escape-hatches.
//   0x220290 -> active mission/campaign controller (its +0x31 "force-change" flag,
//               +0x35/+0x36 hard-core flags)
//   0x2202bc -> KIPlayer singleton (holds the live Mission pointer)
static char *const *const kMissionCtrlSlot = (char *const *const) 0x220290;
static char *const *const kKiPlayerSlot = (char *const *const) 0x2202bc;

// Save-image format marker stored at record word 0x6f; the extended payload block
// is only applied when the record carries this tag.
static const uint32_t kRecordExtendedTag = 0x6e6a78;

void GameRecord::load() {
    uint32_t *rec = (uint32_t *) this;
    bool campaignLocked;
    int campaignStage;
    void *deadArray;
    int *srcArray;
    uint32_t *srcVec;
    char *blueprint;
    char *station;
    char *statusBytes; // (char*)gStatus, for positional field stores
    char *statusField;
    int targetStation;
    uint32_t *srcVec2;
    uint32_t scalar;
    int srcCount;
    int copyIndex;
    int *srcArray2;
    uint32_t i;
    uint32_t j;
    uint64_t qword;

    gStatus->resetGame();
    gGalaxy->setVisited((bool *) *rec, rec[1]);
    gStatus->setLastXP(rec[9]);
    gStatus->setCredits(rec[2]);
    gStatus->setRating(rec[3]);
    gStatus->setPlayingTime(*(int64_t *) (rec + 4));
    gStatus->setKills(rec[6]);
    gStatus->setMissionCount(rec[7]);
    gStatus->setLevel(rec[8]);
    gStatus->setLastXP(rec[9]);
    gStatus->setGoodsProduced(rec[10]);
    gStatus->setPirateKills(rec[0xb]);
    gStatus->setJumpgateUsed(rec[0xc]);
    gStatus->setCapturedCrates(rec[0xd]);
    gStatus->setBoughtEquipment(rec[0xe]);
    gStatus->setStationsVisited(rec[0xf]);
    statusBytes = (char *) gStatus;
    *(uint32_t *) (statusBytes + 0x80) = rec[0x11];
    *(uint32_t *) (statusBytes + 0x7c) = rec[0x12];
    *(uint32_t *) (statusBytes + 0x84) = rec[0x13];
    *(uint32_t *) (statusBytes + 0x88) = rec[0x14];
    if (*(char *) ((long) rec + 0x115) == '\0') {
        if (*(char *) ((long) rec + 0x117) == '\0') {
            if (0x2d < (long) rec[0x10]) {
                rec[0x10] = 0x2d;
            }
            goto applyMissionCap;
        }
    LAB_dlcUnlocked:
        campaignLocked = true;
    } else {
        if (*(char *) ((long) rec + 0x117) != '\0') goto LAB_dlcUnlocked;
    applyMissionCap:
        if (0x54 < (long) rec[0x10]) {
            rec[0x10] = 0x54;
        }
        campaignLocked = false;
    }
    if (((*(char *) ((long) rec + 0x115) != '\0' || campaignLocked) && (rec[0x10] == 0x2e)) &&
        (campaignStage = ((Mission *) rec[0x16])->isEmpty(), campaignStage != 0)) {
        rec[0x10] = 0x2d;
    }
    gStatus->setCurrentCampaignMission(rec[0x10]);
    *(bool *) ((long) rec + 0x102) = 0x2d < (long) rec[0x10];
    *(bool *) ((long) rec + 0x119) = 0x55 < (long) rec[0x10];
    gStatus->setFreelanceMission((Mission *) rec[0x15]);
    gStatus->setCampaignMission((Mission *) rec[0x16]);
    gStatus->setStationStack((Array<Station *> *) rec[0x17]);
    campaignStage = rec[0x10];
    if (campaignStage == 0x23) {
        targetStation = ((Mission *) rec[0x16])->getTargetStation();
        if (targetStation != 0x1d) {
            Mission *m = new Mission(0xb, 0, 0x1d);
            gStatus->setCampaignMission(m);
        }
        campaignStage = rec[0x10];
    }
    if (campaignStage == 0x1d) {
        rec[0x11] = 0x5b;
        rec[0x12] = 0x12;
        gStatus->setCurrentCampaignMission(0x1c);
        Mission *m = new Mission(4, 0, 0x5b);
        gStatus->setCampaignMission(m);
        campaignStage = rec[0x10];
    }
    if (campaignStage == 0x19) {
        rec[0x11] = 0x30;
        rec[0x12] = 9;
        gStatus->setCurrentCampaignMission(0x18);
        Mission *m = new Mission(4, 0, 0x30);
        gStatus->setCampaignMission(m);
        campaignStage = rec[0x10];
    }
    if (campaignStage == 0x29) {
        gStatus->setCurrentCampaignMission(0x27);
        Mission *m = new Mission(0xb, 0, 0x1e);
        gStatus->setCampaignMission(m);
    }
    if (((*(char *) ((long) rec + 0x117) != '\0') || (*(char *) (rec + 0x46) != '\0')) &&
        ((rec[0x10] == 0x56 && (targetStation = ((Mission *) rec[0x16])->getTargetStation(), targetStation != 100)))) {
        (*kMissionCtrlSlot)[0x31] = 1; // RAWREAD: force mission re-targeting flag
        Mission *m = new Mission(0xb, 0, 100);
        gStatus->setCampaignMission(m);
    }
    if (((*(char *) ((long) rec + 0x117) != '\0') || (*(char *) (rec + 0x46) != '\0')) &&
        ((rec[0x10] == 0x57 && (targetStation = ((Mission *) rec[0x16])->getTargetStation(), targetStation != 10)))) {
        (*kMissionCtrlSlot)[0x31] = 1; // RAWREAD: force mission re-targeting flag
        Mission *m = new Mission(4, 0, 10);
        gStatus->setCampaignMission(m);
    }
    if ((((*(char *) ((long) rec + 0x117) != '\0') || (*(char *) (rec + 0x46) != '\0')) &&
         (rec[0x10] == 0x58)) && (targetStation = ((Mission *) rec[0x16])->getTargetStation(), targetStation != 10)) {
        (*kMissionCtrlSlot)[0x31] = 1; // RAWREAD: force mission re-targeting flag
        Mission *m = new Mission(0xb, 0, 10);
        gStatus->setCampaignMission(m);
    }
    if (((*(char *) ((long) rec + 0x117) != '\0') || (*(char *) (rec + 0x46) != '\0')) &&
        ((rec[0x10] == 0x59 && (targetStation = ((Mission *) rec[0x16])->getTargetStation(), targetStation != 10)))) {
        (*kMissionCtrlSlot)[0x31] = 1; // RAWREAD: force mission re-targeting flag
        gStatus->setCurrentCampaignMission(0x56);
        Mission *m = new Mission(0xb, 0, 100);
        gStatus->setCampaignMission(m);
    }
    if ((*(char *) ((long) rec + 0x117) != '\0') || (*(char *) (rec + 0x46) != '\0')) {
        campaignStage = rec[0x10];
        if (campaignStage == 0x5b) {
            targetStation = ((Mission *) rec[0x16])->getTargetStation();
            if (targetStation == 0x6e) {
                campaignStage = rec[0x10];
                goto LAB_checkStageRange;
            }
        } else {
        LAB_checkStageRange:
            if ((0xc < campaignStage - 0x5bU) || (*(char *) (*(int *) (rec[0x58] + 4) + 0x1b) != '\0'))
                goto stationStackLoaded;
        }
        (*kMissionCtrlSlot)[0x31] = 1; // RAWREAD: force mission re-targeting flag
        rec[0x10] = 0x56;
        gStatus->setCurrentCampaignMission(rec[0x10]);
        Mission *m = new Mission(0xb, 0, 100);
        gStatus->setCampaignMission(m);
    }
stationStackLoaded:
    ((Station *) (*(char **) ((char *) gStatus + 0x14c)))->setItems((Array<Item *> *) rec[0x60], true);
    ((Station *) (*(char **) ((char *) gStatus + 0x14c)))->setShips((Array<Ship *> *) rec[0x61], true);
    statusBytes = (char *) gStatus;
    statusField = statusBytes + 0x94;
    if (*(char **) statusField != (char *) 0x0) {
        deadArray = (void *) Array_dtor(*(char **) statusField);
        ::operator delete(deadArray);
        statusBytes = (char *) gStatus;
        statusField = statusBytes + 0x94;
    }
    *(uint32_t *) statusField = rec[0x1a];
    statusField = statusBytes + 0x98;
    if (*(char **) statusField != (char *) 0x0) {
        deadArray = (void *) Array_dtor(*(char **) statusField);
        ::operator delete(deadArray);
        statusBytes = (char *) gStatus;
        statusField = statusBytes + 0x98;
    }
    *(uint32_t *) statusField = rec[0x1b];
    statusField = statusBytes + 0x90;
    if (*(char **) statusField != (char *) 0x0) {
        deadArray = (void *) Array_dtor(*(char **) statusField);
        ::operator delete(deadArray);
        statusBytes = (char *) gStatus;
        statusField = statusBytes + 0x90;
    }
    *(uint32_t *) statusField = rec[0x1c];
    *(uint32_t *) (statusBytes + 0x10c) = rec[0x34];
    // RAWREAD: second mission-system singleton (resolved .bss slot 0x22016c) whose
    // +0x35/+0x36 bytes are hard-core / valkyrie flags.
    char *missionFlags = *(char **) 0x22016c;
    statusBytes[0x110] = *(char *) (rec + 0x35);
    statusBytes[0x111] = *(char *) (rec + 0x37);
    if (*(char *) (missionFlags + 0x36) == '\0') {
        scalar = rec[0x36];
    } else {
        scalar = 3;
    }
    statusField = statusBytes + 0xac;
    *(uint32_t *) (statusBytes + 0x114) = scalar;
    qword = *(uint64_t *) (rec + 0x1f);
    *(uint64_t *) (statusBytes + 0x9c) = *(uint64_t *) (rec + 0x1d);
    *(uint64_t *) (statusBytes + 0xa4) = qword;
    if (*(char **) statusField != (char *) 0x0) {
        deadArray = (void *) Array_dtor(*(char **) statusField);
        ::operator delete(deadArray);
        statusBytes = (char *) gStatus;
        statusField = statusBytes + 0xac;
    }
    *(uint32_t *) statusField = rec[0x21];
    *(uint32_t *) (statusBytes + 0xb0) = rec[0x22];
    srcArray = (int *) rec[0x23];
    srcCount = *srcArray;
    for (copyIndex = 0; srcCount != copyIndex; copyIndex = copyIndex + 1) {
        *(uint8_t *) (*(int *) (*(int *) (statusBytes + 0xb4) + 4) + copyIndex) =
                *(uint8_t *) (srcArray[1] + copyIndex);
    }
    *(uint32_t *) (statusBytes + 0xb8) = rec[0x24];
    scalar = rec[0x27];
    *(uint32_t *) (statusBytes + 0xc0) = rec[0x26];
    *(uint32_t *) (statusBytes + 0xc4) = scalar;
    qword = *(uint64_t *) (rec + 0x2a);
    *(uint64_t *) (statusBytes + 200) = *(uint64_t *) (rec + 0x28);
    *(uint64_t *) (statusBytes + 0xd0) = qword;
    qword = *(uint64_t *) (rec + 0x2e);
    *(uint64_t *) (statusBytes + 0xd8) = *(uint64_t *) (rec + 0x2c);
    *(uint64_t *) (statusBytes + 0xe0) = qword;
    *(uint32_t *) (statusBytes + 0xe8) = rec[0x30];
    *(uint32_t *) (statusBytes + 0xec) = rec[0x31];
    gAchievements->init();
    if ((int *) rec[0x18] != (int *) 0x0) {
        gAchievements->setMedals((int *) rec[0x18], rec[0x19]);
    }
    gStatus->setShip((Ship *) rec[0x4c]);
    if (gStatus->dlc1Won() != 0) {
        Item *dlcItem = (Item *) gStatus->getShip()->getFirstEquipmentOfSort(0x12);
        if (dlcItem == (Item *) 0x0) {
            dlcItem = (Item *) gStatus->getShip()->getCargo(0x55);
            if (dlcItem == (Item *) 0x0) goto afterDlcUnsaleable;
        }
        dlcItem->setUnsaleable(false);
    }
afterDlcUnsaleable:
    if (gStatus->gameWon() != 0) {
        Array<Item *> *cargo = ((Ship *) (gStatus->getShip()))->getCargo();
        if (cargo != (Array<Item *> *) 0x0) {
            for (i = 0; i < cargo->size(); i = i + 1) {
                Item *item = (*cargo)[i];
                if ((item != (Item *) 0x0) && (item->getIndex() == 0x83)) {
                    item->setUnsaleable(false);
                }
            }
        }
    }
    if (0x79 < (long) rec[0x10]) {
        Array<Item *> *cargo = ((Ship *) (gStatus->getShip()))->getCargo();
        if (cargo != (Array<Item *> *) 0x0) {
            for (i = 0; i < cargo->size(); i = i + 1) {
                Item *item = (*cargo)[i];
                if ((item != (Item *) 0x0) && (item->getIndex() == 0xd1)) {
                    item->setUnsaleable(false);
                }
            }
        }
    }
    srcVec = (uint *) rec[0x50];
    if (srcVec != (uint *) 0x0) {
        for (i = 0; i < *srcVec; i = i + 1) {
            blueprint = *(char **) (srcVec[1] + i * 4);
            if ((blueprint != (char *) 0x0) &&
                (((((BluePrint *) (blueprint))->getIndex() == 0xdf ||
                   (((BluePrint *) (*(char **) (*(int *) (rec[0x50] + 4) + i * 4)))->getIndex() == 0xd2)) &&
                  (((BluePrint *) (*(char **) (*(int *) (rec[0x50] + 4) + i * 4)))->isEmpty() == 0)))) {
                station = (char *) gGalaxy->getStation(
                    (int) ((BluePrint *) (*(char **) (*(int *) (rec[0x50] + 4) + i * 4)))->getStationIndex());
                if (station != (char *) 0x0) {
                    int sysIndex = Station_getSystem(station);
                    SolarSystem *sys = (SolarSystem *) gGalaxy->getSystem(sysIndex);
                    if (sys->getRoutes() == (void *) 0x0) {
                        *(uint32_t *) (*(int *) (*(int *) (rec[0x50] + 4) + i * 4) + 0x10) = 10;
                        delete (Station *) station;
                        station = (char *) gGalaxy->getStation(
                            (int) ((BluePrint *) (*(char **) (*(int *) (rec[0x50] + 4) + i * 4)))->getStationIndex());
                        ((BluePrint *) (*(char **) (*(int *) (rec[0x50] + 4) + i * 4)))->stationName =
                                ((Station *) (station))->getName();
                        if (station == (char *) 0x0) goto LAB_blueprintLoopTail;
                    }
                    delete (Station *) station;
                }
            }
        LAB_blueprintLoopTail:
            srcVec = (uint *) rec[0x50];
        }
    }
    statusBytes = (char *) gStatus;
    *(uint32_t *) (statusBytes + 0x8c) = rec[0x4d];
    ((Status *) (statusBytes))->setStation((Station *) rec[0x4e]);
    gStatus->setMission(*(Mission **) (*kKiPlayerSlot)); // RAWREAD: KIPlayer's live Mission
    *(uint32_t *) ((char *) gStatus + 0x14) = rec[0x4f];
    for (i = 0; i < *(uint *) rec[0x50]; i = i + 1) {
        *(uint32_t *) (*(int *) (*(int *) ((char *) gStatus + 0x18) + 4) + i * 4) =
                *(uint32_t *) (((uint *) rec[0x50])[1] + i * 4);
    }
    *(uint32_t *) ((char *) gStatus + 0x1c) = rec[0x51];
    for (i = 0; i < *(uint *) rec[0x52]; i = i + 1) {
        *(uint32_t *) (*(int *) (*(int *) ((char *) gStatus + 0x20) + 4) + i * 4) =
                *(uint32_t *) (((uint *) rec[0x52])[1] + i * 4);
    }
    statusBytes = (char *) gStatus;
    *(uint32_t *) (statusBytes + 0x24) = rec[0x53];
    *(uint32_t *) (statusBytes + 0x2c) = rec[0x54];
    *(uint32_t *) (statusBytes + 0x30) = rec[0x55];
    *(uint32_t *) (statusBytes + 0x28) = rec[0x56];
    *(uint32_t *) (statusBytes + 0x34) = rec[0x57];
    srcArray2 = (int *) rec[0x58];
    srcCount = *srcArray2;
    for (copyIndex = 0; srcCount != copyIndex; copyIndex = copyIndex + 1) {
        *(uint8_t *) (*(int *) (*(int *) (statusBytes + 0x38) + 4) + copyIndex) =
                *(uint8_t *) (srcArray2[1] + copyIndex);
    }
    char *visBuf = *(char **) (*(int *) (statusBytes + 0x38) + 4);
    if (*(char *) (missionFlags + 0x35) != '\0') {
        *(uint8_t *) (visBuf + 0x19) = 1;
    }
    *(uint8_t *) (visBuf + 0x1a) = 1;
    srcVec = (uint *) rec[0x59];
    for (i = 0; i < *srcVec; i = i + 1) {
        *(uint32_t *) (*(int *) (*(int *) (statusBytes + 0x3c) + 4) + i * 4) =
                *(uint32_t *) (srcVec[1] + i * 4);
    }
    srcVec = (uint *) rec[0x5a];
    for (i = 0; i < *srcVec; i = i + 1) {
        *(uint32_t *) (*(int *) (*(int *) (statusBytes + 0x40) + 4) + i * 4) =
                *(uint32_t *) (srcVec[1] + i * 4);
    }
    srcVec = (uint *) rec[0x5b];
    for (i = 0; i < *srcVec; i = i + 1) {
        *(uint32_t *) (*(int *) (*(int *) (statusBytes + 0x44) + 4) + i * 4) =
                *(uint32_t *) (srcVec[1] + i * 4);
    }
    srcVec = (uint *) rec[0x5c];
    for (i = 0; i < *srcVec; i = i + 1) {
        *(uint32_t *) (*(int *) (*(int *) (statusBytes + 0x48) + 4) + i * 4) =
                *(uint32_t *) (srcVec[1] + i * 4);
    }
    srcVec = *(uint **) (statusBytes + 0x54);
    *(uint32_t *) (statusBytes + 0x4c) = rec[0x5d];
    srcVec2 = (uint *) rec[0x5e];
    i = *srcVec2;
    if (*srcVec <= i) {
        for (j = 0; i != j; j = j + 1) {
            *(uint8_t *) (srcVec[1] + j) = *(uint8_t *) (srcVec2[1] + j);
        }
    }
    // RAWREAD: KIPlayer record fields (resolved .bss slot 0x220290 deref).
    char *kiPlayer = *(char **) 0x220290;
    qword = *(uint64_t *) (rec + 0x3b);
    *(uint64_t *) (kiPlayer + 8) = *(uint64_t *) (rec + 0x39);
    *(uint64_t *) (kiPlayer + 0x10) = qword;
    // The binary writes rec[0x47] to missionFlags+0x2c (str r2,[r12,#0x2c] where
    // r12 is the 0x22016c mission-flags singleton), NOT to gStatus+0x2c.
    *(uint32_t *) (missionFlags + 0x2c) = rec[0x47];
    scalar = rec[0x33];
    *(uint32_t *) (statusBytes + 0x100) = rec[0x32];
    *(uint32_t *) (statusBytes + 0x104) = scalar;
    qword = *(uint64_t *) (rec + 0x3f);
    *(uint64_t *) (kiPlayer + 0x18) = *(uint64_t *) (rec + 0x3d);
    *(uint64_t *) (kiPlayer + 0x20) = qword;
    if (rec[0x6f] == kRecordExtendedTag) {
        *(uint8_t *) (kiPlayer + 0x29) = *(uint8_t *) ((long) rec + 0x105);
        *(uint8_t *) (kiPlayer + 0x28) = *(uint8_t *) (rec + 0x41);
        *(uint8_t *) (kiPlayer + 0x2a) = *(uint8_t *) ((long) rec + 0x106);
        *(uint8_t *) (kiPlayer + 0x2b) = *(uint8_t *) ((long) rec + 0x107);
        *(uint8_t *) (kiPlayer + 0x2c) = *(uint8_t *) (rec + 0x42);
        *(uint8_t *) (kiPlayer + 0x2d) = *(uint8_t *) ((long) rec + 0x109);
        *(uint8_t *) (kiPlayer + 0x2e) = *(uint8_t *) ((long) rec + 0x10a);
        *(uint8_t *) (kiPlayer + 0x2f) = *(uint8_t *) ((long) rec + 0x10b);
        *(uint8_t *) (kiPlayer + 0x30) = *(uint8_t *) (rec + 0x43);
        *(uint32_t *) (statusBytes + 0x178) = rec[0x6e];
        *(uint8_t *) (kiPlayer + 0x31) = *(uint8_t *) ((long) rec + 0x119);
        *(uint8_t *) (kiPlayer + 0x32) = *(uint8_t *) ((long) rec + 0x11a);
        *(uint8_t *) (kiPlayer + 0x33) = *(uint8_t *) ((long) rec + 0x10d);
        *(uint8_t *) (kiPlayer + 0x34) = *(uint8_t *) ((long) rec + 0x10e);
        *(uint8_t *) (kiPlayer + 0x35) = *(uint8_t *) ((long) rec + 0x10f);
        *(uint8_t *) (kiPlayer + 0x36) = *(uint8_t *) (rec + 0x44);
        *(uint8_t *) (kiPlayer + 0x37) = *(uint8_t *) ((long) rec + 0x111);
        *(uint8_t *) (kiPlayer + 0x38) = *(uint8_t *) ((long) rec + 0x113);
        *(uint8_t *) (kiPlayer + 0x39) = *(uint8_t *) ((long) rec + 0x112);
        *(uint8_t *) (kiPlayer + 0x3a) = *(uint8_t *) (rec + 0x45);
        *(uint32_t *) (statusBytes + 0x58) = rec[0x5f];
        srcVec = (uint *) rec[0x6d];
        if ((srcVec != (uint *) 0x0) && (i = *srcVec, i != 0)) {
            for (j = 0; j < i; j = j + 1) {
                *(uint32_t *) (*(int *) (*(int *) gStatus + 4) + j * 4) =
                        *(uint32_t *) (srcVec[1] + j * 4);
                srcVec = (uint *) rec[0x6d];
                i = *srcVec;
            }
        }
        statusBytes = (char *) gStatus;
        for (int k = 0; k != 4; k = k + 1) {
            *(uint32_t *) (statusBytes + k * 4 + 4) = rec[k + 0x69];
        }
        station = (char *) gStatus->getStation();
        if (Station_getIndex(station) == 0x6c) {
            Station *stStation = gStatus->getStation();
            Array<Ship *> *stationShips = (Array<Ship *> *) stStation->getShips();
            if (stationShips != (Array<Ship *> *) 0x0) {
                for (i = 0; i < stationShips->size(); i = i + 1) {
                    Array<int> *mods =
                            ((Ship *) (*(char **) (*(int *) (rec[0x61] + 4) + i * 4)))->getMods();
                    if (mods != (Array<int> *) 0x0) {
                        for (j = 0; j < mods->size(); j = j + 1) {
                            (*stationShips)[i]->addMod((*mods)[j]);
                        }
                    }
                }
            }
        }
        statusBytes = (char *) gStatus;
        *(uint32_t *) (statusBytes + 0x118) = rec[0x38];
        *(uint8_t *) (statusBytes + 0x17c) = *(uint8_t *) (rec + 0x70);
    }
}
