#include "gof2/GameRecord.h"
#include "gof2/Galaxy.h"
#include "gof2/Item.h"
#include "gof2/Status.h"
#include "gof2/Achievements.h"
#include "gof2/BluePrint.h"
#include "gof2/Station.h"




// ---- _GameRecord_1559d4.cpp ----
// GameRecord::~GameRecord()
GameRecord::~GameRecord()
{
}

// ---- GameRecord_155900.cpp ----
// GameRecord::GameRecord()
GameRecord::GameRecord()
{
    char *t = (char *)this;

    // Two embedded AbyssEngine::String members.

    // Heap buffer at +0x00.
    void *buf = ::operator new[](0x87);
    *(uint32_t *)(t + 0x11c) = 0;
    *(void **)t = buf;

    // Clear the leading block.
    __aeabi_memclr8(t + 0x8, 0x8c);

    // Seven 16-byte vector slots cleared to zero (a {0,0,0,0} init).
    *(uint32_t *)(t + 0xb8) = 0; *(uint32_t *)(t + 0xbc) = 0;
    *(uint32_t *)(t + 0xc0) = 0; *(uint32_t *)(t + 0xc4) = 0;
    *(uint32_t *)(t + 0xa8) = 0; *(uint32_t *)(t + 0xac) = 0;
    *(uint32_t *)(t + 0xb0) = 0; *(uint32_t *)(t + 0xb4) = 0;
    *(uint32_t *)(t + 0x98) = 0; *(uint32_t *)(t + 0x9c) = 0;
    *(uint32_t *)(t + 0xa0) = 0; *(uint32_t *)(t + 0xa4) = 0;
    *(uint32_t *)(t + 0x10b) = 0; *(uint32_t *)(t + 0x10f) = 0;
    *(uint32_t *)(t + 0x113) = 0; *(uint32_t *)(t + 0x117) = 0;
    *(uint32_t *)(t + 0x100) = 0; *(uint32_t *)(t + 0x104) = 0;
    *(uint32_t *)(t + 0x108) = 0; *(uint32_t *)(t + 0x10c) = 0;
    *(uint32_t *)(t + 0xf0) = 0; *(uint32_t *)(t + 0xf4) = 0;
    *(uint32_t *)(t + 0xf8) = 0; *(uint32_t *)(t + 0xfc) = 0;
    *(uint32_t *)(t + 0xe0) = 0; *(uint32_t *)(t + 0xe4) = 0;
    *(uint32_t *)(t + 0xe8) = 0; *(uint32_t *)(t + 0xec) = 0;

    *(uint32_t *)(t + 0x1a0) = 0xffffffff;
    *(uint32_t *)(t + 0xc8) = 0;
    *(uint32_t *)(t + 0xcc) = 0;
    *(uint32_t *)(t + 0xd0) = 0;
    *(uint8_t *)(t + 0xd4) = 0;
    *(uint32_t *)(t + 0xd8) = 0;
    *(uint8_t *)(t + 0xdc) = 0;
    *(uint32_t *)(t + 0x1b8) = 0;
    *(uint8_t *)(t + 0x1c0) = 0;

    __aeabi_memclr8(t + 0x130, 0x58);

    for (int iVar5 = 0x69; iVar5 != 0x6d; iVar5 = iVar5 + 1)
        *(uint32_t *)(t + iVar5 * 4) = 0;

    *(uint32_t *)(t + 0x1b4) = 0;
}

// ---- load_1559f0.cpp ----
// GameRecord::load() — restore game state from the in-memory save buffer.
// COVERAGE MODE: faithful structural translation of the decompiled control flow.
// Cross-class member calls are modelled as extern "C" free functions; opaque
// class/array pointers are uniformly char* for byte-addressed field access.
// Not byte-exact by design.

typedef uint32_t uint;

static inline uint64_t CONCAT44(uint32_t hi, uint32_t lo)
{ return ((uint64_t)hi << 32) | (uint32_t)lo; }
static inline uint8_t SUB41(uint32_t v, int) { return (uint8_t)v; }

extern "C" {
void *AEString_assign(void *self, const void *other);
void AEString_dtor(void *self);
long Array_dtor(void *self);
long BluePrint_getIndex(...);
long BluePrint_getStationIndex(...);
long Mission_ctor(...);
long Mission_getTargetStation(...);
long Mission_isEmpty(...);
void Status_setShip(...);
void Status_setStation(...);
void Status_setMission(...);
long Ship_addMod(...);
long Ship_getCargo(...);
long Ship_getFirstEquipmentOfSort(...);
long Ship_getMods(...);
long SolarSystem_getRoutes(...);
long Station_getIndex(...);
long Station_getShips(...);
long Station_getSystem(...);
extern uint32_t DAT_00165b08;
extern uint32_t DAT_00165b0c;
extern uint32_t DAT_00165b10;
extern uint32_t DAT_0016600c;
extern uint32_t DAT_00166010;
extern uint32_t DAT_00166014;
extern uint32_t DAT_00166018;
extern uint32_t DAT_0016601c;
extern uint32_t DAT_00166020;
extern uint32_t DAT_00166024;
extern uint32_t DAT_0016646c;
extern uint32_t DAT_00166474;
}

void GameRecord::load() {
    GameRecord *self = this;
    uint32_t *in_r0 = (uint32_t *)self;
    char gr_tmpstr[12];  // hidden String return slot (Station::getName)
    bool bVar1;
    int iVar2;
    char *pMVar3;
    char *pvVar4;
    int *piVar5;
    uint32_t *puVar6;
    char *pIVar7;
    char *pThis;
    char *pSVar8;
    int iVar9;
    uint32_t extraout_r1;
    char *ctx;
    char *pEVar10;
    int iVar11;
    uint32_t *puVar12;
    uint32_t uVar13;
    int iVar14;
    int iVar15;
    int *piVar16;
    char *pSVar17;
    uint32_t *puVar18;
    char *this_00;
    uint32_t uVar19;
    uint32_t uVar20;
    int *piVar21;
    int *piVar22;
    uint64_t uVar23;

  piVar21 = *(int **)(DAT_00165b08 + 0x165a02);
  piVar22 = *(int **)(DAT_00165b0c + 0x165a04);
  iVar9 = *piVar21;
  ((Status *)(*piVar22))->resetGame();
  piVar16 = *(int **)(DAT_00165b10 + 0x165a22);
  ((Galaxy *)((char *)*piVar16))->setVisited((bool *)*in_r0, in_r0[1]);
  ((Status *)((char *)*piVar22))->setLastXP(in_r0[9]);
  ((Status *)(*piVar22))->setCredits(in_r0[2]);
  ((Status *)((char *)*piVar22))->setRating(in_r0[3]);
  ((Status *)(*piVar22))->setPlayingTime(*(int64_t *)(in_r0 + 4));
  ((Status *)(*piVar22))->setKills(in_r0[6]);
  ((Status *)((char *)*piVar22))->setMissionCount(in_r0[7]);
  ((Status *)((char *)*piVar22))->setLevel(in_r0[8]);
  ((Status *)((char *)*piVar22))->setLastXP(in_r0[9]);
  ((Status *)((char *)*piVar22))->setGoodsProduced(in_r0[10]);
  ((Status *)((char *)*piVar22))->setPirateKills(in_r0[0xb]);
  ((Status *)((char *)*piVar22))->setJumpgateUsed(in_r0[0xc]);
  ((Status *)((char *)*piVar22))->setCapturedCrates(in_r0[0xd]);
  ((Status *)((char *)*piVar22))->setBoughtEquipment(in_r0[0xe]);
  ((Status *)((char *)*piVar22))->setStationsVisited(in_r0[0xf]);
  iVar2 = *piVar22;
  *(uint32_t *)(iVar2 + 0x80) = in_r0[0x11];
  *(uint32_t *)(iVar2 + 0x7c) = in_r0[0x12];
  *(uint32_t *)(iVar2 + 0x84) = in_r0[0x13];
  *(uint32_t *)(iVar2 + 0x88) = in_r0[0x14];
  if (*(char *)((long)in_r0 + 0x115) == '\0') {
    if (*(char *)((long)in_r0 + 0x117) == '\0') {
      if (0x2d < (long)in_r0[0x10]) {
        in_r0[0x10] = 0x2d;
      }
      goto LAB_00165b22;
    }
LAB_00165b02:
    bVar1 = true;
  }
  else {
    if (*(char *)((long)in_r0 + 0x117) != '\0') goto LAB_00165b02;
LAB_00165b22:
    if (0x54 < (long)in_r0[0x10]) {
      in_r0[0x10] = 0x54;
    }
    bVar1 = false;
  }
  if (((*(char *)((long)in_r0 + 0x115) != '\0' || bVar1) && (in_r0[0x10] == 0x2e)) &&
     (iVar2 = Mission_isEmpty(), iVar2 != 0)) {
    in_r0[0x10] = 0x2d;
  }
  ((Status *)(*piVar22))->setCurrentCampaignMission(in_r0[0x10]);
  *(bool *)((long)in_r0 + 0x102) = 0x2d < (long)in_r0[0x10];
  *(bool *)((long)in_r0 + 0x119) = 0x55 < (long)in_r0[0x10];
  ((Status *)((char *)*piVar22))->setFreelanceMission((Mission *)in_r0[0x15]);
  ((Status *)((char *)*piVar22))->setCampaignMission((Mission *)in_r0[0x16]);
  ((Status *)((char *)*piVar22))->setStationStack((Array<Station *> *)in_r0[0x17]);
  iVar2 = in_r0[0x10];
  if (iVar2 == 0x23) {
    iVar2 = Mission_getTargetStation();
    if (iVar2 != 0x1d) {
      pSVar17 = (char *)*piVar22;
      pMVar3 = (decltype(pMVar3))::operator new(0x78);
      Mission_ctor(pMVar3,0xb,0,0x1d);
      ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
    }
    iVar2 = in_r0[0x10];
  }
  if (iVar2 == 0x1d) {
    in_r0[0x11] = 0x5b;
    in_r0[0x12] = 0x12;
    ((Status *)(*piVar22))->setCurrentCampaignMission(in_r0[0x10]);
    pSVar17 = (char *)*piVar22;
    pMVar3 = (decltype(pMVar3))::operator new(0x78);
    Mission_ctor(pMVar3,4,0,0x5b);
    ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
    iVar2 = in_r0[0x10];
  }
  if (iVar2 == 0x19) {
    in_r0[0x11] = 0x30;
    in_r0[0x12] = 9;
    ((Status *)(*piVar22))->setCurrentCampaignMission(in_r0[0x10]);
    pSVar17 = (char *)*piVar22;
    pMVar3 = (decltype(pMVar3))::operator new(0x78);
    Mission_ctor(pMVar3,4,0,0x30);
    ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
    iVar2 = in_r0[0x10];
  }
  if (iVar2 == 0x29) {
    ((Status *)(*piVar22))->setCurrentCampaignMission(in_r0[0x10]);
    pSVar17 = (char *)*piVar22;
    pMVar3 = (decltype(pMVar3))::operator new(0x78);
    Mission_ctor(pMVar3,0xb,0,0x1e);
    ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
  }
  if (((*(char *)((long)in_r0 + 0x117) != '\0') || (*(char *)(in_r0 + 0x46) != '\0')) &&
     ((in_r0[0x10] == 0x56 && (iVar2 = Mission_getTargetStation(), iVar2 != 100)))) {
    pSVar17 = (char *)*piVar22;
    *(uint8_t *)(*(int *)(DAT_0016600c + 0x165c98) + 0x31) = 1;
    pMVar3 = (decltype(pMVar3))::operator new(0x78);
    Mission_ctor(pMVar3,0xb,0,100);
    ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
  }
  if (((*(char *)((long)in_r0 + 0x117) != '\0') || (*(char *)(in_r0 + 0x46) != '\0')) &&
     ((in_r0[0x10] == 0x57 && (iVar2 = Mission_getTargetStation(), iVar2 != 10)))) {
    pSVar17 = (char *)*piVar22;
    *(uint8_t *)(*(int *)(DAT_00166010 + 0x165ce2) + 0x31) = 1;
    pMVar3 = (decltype(pMVar3))::operator new(0x78);
    Mission_ctor(pMVar3,4,0,10);
    ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
  }
  if ((((*(char *)((long)in_r0 + 0x117) != '\0') || (*(char *)(in_r0 + 0x46) != '\0')) &&
      (in_r0[0x10] == 0x58)) && (iVar2 = Mission_getTargetStation(), iVar2 != 10)) {
    pSVar17 = (char *)*piVar22;
    *(uint8_t *)(*(int *)(DAT_00166014 + 0x165d2c) + 0x31) = 1;
    pMVar3 = (decltype(pMVar3))::operator new(0x78);
    Mission_ctor(pMVar3,0xb,0,10);
    ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
  }
  if (((*(char *)((long)in_r0 + 0x117) != '\0') || (*(char *)(in_r0 + 0x46) != '\0')) &&
     ((in_r0[0x10] == 0x59 && (iVar2 = Mission_getTargetStation(), iVar2 != 10)))) {
    iVar2 = *piVar22;
    *(uint8_t *)(*(int *)(DAT_00166018 + 0x165d72) + 0x31) = 1;
    ((Status *)(iVar2))->setCurrentCampaignMission(in_r0[0x10]);
    pSVar17 = (char *)*piVar22;
    pMVar3 = (decltype(pMVar3))::operator new(0x78);
    Mission_ctor(pMVar3,0xb,0,100);
    ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
  }
  if ((*(char *)((long)in_r0 + 0x117) != '\0') || (*(char *)(in_r0 + 0x46) != '\0')) {
    iVar2 = in_r0[0x10];
    if (iVar2 == 0x5b) {
      iVar2 = Mission_getTargetStation();
      if (iVar2 == 0x6e) {
        iVar2 = in_r0[0x10];
        goto LAB_00165dc2;
      }
    }
    else {
LAB_00165dc2:
      if ((0xc < iVar2 - 0x5bU) || (*(char *)(*(int *)(in_r0[0x58] + 4) + 0x1b) != '\0'))
      goto LAB_00165e0c;
    }
    *(uint8_t *)(*(int *)(DAT_0016601c + 0x165dda) + 0x31) = 1;
    in_r0[0x10] = 0x56;
    ((Status *)(*piVar22))->setCurrentCampaignMission(in_r0[0x10]);
    pSVar17 = (char *)*piVar22;
    pMVar3 = (decltype(pMVar3))::operator new(0x78);
    Mission_ctor(pMVar3,0xb,0,100);
    ((Status *)(pSVar17))->setCampaignMission((Mission *)pMVar3);
  }
LAB_00165e0c:
  ((Station *)(*(char **)(*piVar22 + 0x14c)))->setItems((uint32_t *)in_r0[0x60], true);
  ((Station *)(*(char **)(*piVar22 + 0x14c)))->setShips((uint32_t *)in_r0[0x61], SUB41(in_r0[0x61],0));
  ctx = (char *)*piVar22;
  pEVar10 = ctx + 0x94;
  if (*(char **)pEVar10 != (char *)0x0) {
    pvVar4 = (char *)Array_dtor(*(char **)pEVar10);
    ::operator delete(pvVar4);
    ctx = (char *)*piVar22;
    pEVar10 = ctx + 0x94;
  }
  *(uint32_t *)pEVar10 = in_r0[0x1a];
  pEVar10 = ctx + 0x98;
  if (*(char **)pEVar10 != (char *)0x0) {
    pvVar4 = (char *)Array_dtor(*(char **)pEVar10);
    ::operator delete(pvVar4);
    ctx = (char *)*piVar22;
    pEVar10 = ctx + 0x98;
  }
  *(uint32_t *)pEVar10 = in_r0[0x1b];
  pEVar10 = ctx + 0x90;
  if (*(char **)pEVar10 != (char *)0x0) {
    pvVar4 = (char *)Array_dtor(*(char **)pEVar10);
    ::operator delete(pvVar4);
    ctx = (char *)*piVar22;
    pEVar10 = ctx + 0x90;
  }
  iVar2 = DAT_00166020;
  *(uint32_t *)pEVar10 = in_r0[0x1c];
  *(uint32_t *)(ctx + 0x10c) = in_r0[0x34];
  iVar2 = *(int *)(iVar2 + 0x165e94);
  ctx[0x110] = *(char *)(in_r0 + 0x35);
  ctx[0x111] = *(char *)(in_r0 + 0x37);
  if (*(char *)(iVar2 + 0x36) == '\0') {
    uVar13 = in_r0[0x36];
  }
  else {
    uVar13 = 3;
  }
  pEVar10 = ctx + 0xac;
  *(uint32_t *)(ctx + 0x114) = uVar13;
  uVar23 = *(uint64_t *)(in_r0 + 0x1f);
  *(uint64_t *)(ctx + 0x9c) = *(uint64_t *)(in_r0 + 0x1d);
  *(uint64_t *)(ctx + 0xa4) = uVar23;
  if (*(char **)pEVar10 != (char *)0x0) {
    pvVar4 = (char *)Array_dtor(*(char **)pEVar10);
    ::operator delete(pvVar4);
    ctx = (char *)*piVar22;
    pEVar10 = ctx + 0xac;
  }
  *(uint32_t *)pEVar10 = in_r0[0x21];
  *(uint32_t *)(ctx + 0xb0) = in_r0[0x22];
  piVar5 = (int *)in_r0[0x23];
  iVar11 = *piVar5;
  for (iVar14 = 0; iVar11 != iVar14; iVar14 = iVar14 + 1) {
    *(uint8_t *)(*(int *)(*(int *)(ctx + 0xb4) + 4) + iVar14) =
         *(uint8_t *)(piVar5[1] + iVar14);
  }
  *(uint32_t *)(ctx + 0xb8) = in_r0[0x24];
  iVar11 = DAT_00166024;
  uVar13 = in_r0[0x27];
  *(uint32_t *)(ctx + 0xc0) = in_r0[0x26];
  *(uint32_t *)(ctx + 0xc4) = uVar13;
  uVar23 = *(uint64_t *)(in_r0 + 0x2a);
  *(uint64_t *)(ctx + 200) = *(uint64_t *)(in_r0 + 0x28);
  *(uint64_t *)(ctx + 0xd0) = uVar23;
  uVar23 = *(uint64_t *)(in_r0 + 0x2e);
  *(uint64_t *)(ctx + 0xd8) = *(uint64_t *)(in_r0 + 0x2c);
  *(uint64_t *)(ctx + 0xe0) = uVar23;
  *(uint32_t *)(ctx + 0xe8) = in_r0[0x30];
  puVar18 = *(uint32_t **)(iVar11 + 0x165f34);
  *(uint32_t *)(ctx + 0xec) = in_r0[0x31];
  ((Achievements *)((char *)*puVar18))->init();
  if ((int *)in_r0[0x18] != (int *)0x0) {
    ((Achievements *)((char *)*puVar18))->setMedals((int *)in_r0[0x18], in_r0[0x19]);
  }
  Status_setShip((char *)*piVar22);  // dropped-self call: Ship* argument unrecoverable from local scope (needs Ghidra)
  iVar11 = ((Status *)(*piVar22))->dlc1Won();
  if (iVar11 != 0) {
    iVar11 = (long)((Status *)(*piVar22))->getShip();
    iVar11 = Ship_getFirstEquipmentOfSort(iVar11);
    if (iVar11 == 0) {
      iVar11 = (long)((Status *)(*piVar22))->getShip();
      iVar11 = Ship_getCargo(iVar11);
      if (iVar11 == 0) goto LAB_00165faa;
    }
    ((Item *)(SUB41(iVar11,0)))->setUnsaleable(true);
  }
LAB_00165faa:
  iVar11 = ((Status *)(*piVar22))->gameWon();
  if (iVar11 != 0) {
    ((Status *)(*piVar22))->getShip();
    puVar6 = (uint *)Ship_getCargo();
    if (puVar6 != (uint *)0x0) {
      for (uVar19 = 0; uVar19 < *puVar6; uVar19 = uVar19 + 1) {
        pIVar7 = *(char **)(puVar6[1] + uVar19 * 4);
        if ((pIVar7 != (char *)0x0) && (iVar11 = ((Item *)(pIVar7))->getIndex(), iVar11 == 0x83)) {
          ((Item *)(SUB41(*(uint32_t *)(puVar6[1] + uVar19 * 4),0)))->setUnsaleable(true);
        }
      }
    }
  }
  if (0x79 < (long)in_r0[0x10]) {
    ((Status *)(*piVar22))->getShip();
    puVar6 = (uint *)Ship_getCargo();
    if (puVar6 != (uint *)0x0) {
      for (uVar19 = 0; uVar19 < *puVar6; uVar19 = uVar19 + 1) {
        pIVar7 = *(char **)(puVar6[1] + uVar19 * 4);
        if ((pIVar7 != (char *)0x0) && (iVar11 = ((Item *)(pIVar7))->getIndex(), iVar11 == 0xd1)) {
          ((Item *)(SUB41(*(uint32_t *)(puVar6[1] + uVar19 * 4),0)))->setUnsaleable(true);
        }
      }
    }
  }
  puVar6 = (uint *)in_r0[0x50];
  if (puVar6 != (uint *)0x0) {
    for (uVar19 = 0; uVar19 < *puVar6; uVar19 = uVar19 + 1) {
      pThis = *(char **)(puVar6[1] + uVar19 * 4);
      if ((pThis != (char *)0x0) &&
         (((iVar11 = BluePrint_getIndex(pThis), iVar11 == 0xdf ||
           (iVar11 = BluePrint_getIndex(*(char **)(*(int *)(in_r0[0x50] + 4) + uVar19 * 4)),
           iVar11 == 0xd2)) &&
          (iVar11 = ((BluePrint *)(*(char **)(*(int *)(in_r0[0x50] + 4) + uVar19 * 4)))->isEmpty(),
          iVar11 == 0)))) {
        iVar11 = *piVar16;
        pSVar8 = (char *)((Galaxy *)(iVar11))->getStation((int)BluePrint_getStationIndex(*(char **)(*(int *)(in_r0[0x50] + 4) + uVar19 * 4)));
        if (pSVar8 != (char *)0x0) {
          this_00 = (char *)*piVar16;
          iVar11 = Station_getSystem(pSVar8);
          ((Galaxy *)(this_00))->getSystem(iVar11);
          iVar11 = SolarSystem_getRoutes();
          if (iVar11 == 0) {
            *(uint32_t *)(*(int *)(*(int *)(in_r0[0x50] + 4) + uVar19 * 4) + 0x10) = 10;
            ((Station *)(pSVar8))->dtor();
            pvVar4 = pSVar8;
            ::operator delete(pvVar4);
            pSVar8 = (char *)((Galaxy *)(*piVar16))->getStation((int)BluePrint_getStationIndex(*(char **)(*(int *)(in_r0[0x50] + 4) + uVar19 * 4)));
            ((Station *)(pSVar8))->getName();
            if (pSVar8 == (char *)0x0) goto LAB_00166114;
          }
          ((Station *)(pSVar8))->dtor();
          pvVar4 = pSVar8;
          ::operator delete(pvVar4);
        }
      }
LAB_00166114:
      puVar6 = (uint *)in_r0[0x50];
    }
  }
  pSVar8 = (char *)*piVar22;
  *(uint32_t *)(pSVar8 + 0x8c) = in_r0[0x4d];
  Status_setStation(pSVar8);  // dropped-self call: Station* argument unrecoverable from local scope (needs Ghidra)
  Status_setMission((char *)*piVar22);  // dropped-self call: Mission* argument unrecoverable from local scope (needs Ghidra)
  *(uint32_t *)(*piVar22 + 0x14) = in_r0[0x4f];
  for (uVar19 = 0; uVar19 < *(uint *)in_r0[0x50]; uVar19 = uVar19 + 1) {
    *(uint32_t *)(*(int *)(*(int *)(*piVar22 + 0x18) + 4) + uVar19 * 4) =
         *(uint32_t *)(((uint *)in_r0[0x50])[1] + uVar19 * 4);
  }
  *(uint32_t *)(*piVar22 + 0x1c) = in_r0[0x51];
  for (uVar19 = 0; uVar19 < *(uint *)in_r0[0x52]; uVar19 = uVar19 + 1) {
    *(uint32_t *)(*(int *)(*(int *)(*piVar22 + 0x20) + 4) + uVar19 * 4) =
         *(uint32_t *)(((uint *)in_r0[0x52])[1] + uVar19 * 4);
  }
  iVar11 = *piVar22;
  *(uint32_t *)(iVar11 + 0x24) = in_r0[0x53];
  *(uint32_t *)(iVar11 + 0x2c) = in_r0[0x54];
  *(uint32_t *)(iVar11 + 0x30) = in_r0[0x55];
  *(uint32_t *)(iVar11 + 0x28) = in_r0[0x56];
  *(uint32_t *)(iVar11 + 0x34) = in_r0[0x57];
  piVar16 = (int *)in_r0[0x58];
  iVar14 = *piVar16;
  for (iVar15 = 0; iVar14 != iVar15; iVar15 = iVar15 + 1) {
    *(uint8_t *)(*(int *)(*(int *)(iVar11 + 0x38) + 4) + iVar15) =
         *(uint8_t *)(piVar16[1] + iVar15);
  }
  iVar14 = *(int *)(*(int *)(iVar11 + 0x38) + 4);
  if (*(char *)(iVar2 + 0x35) != '\0') {
    *(uint8_t *)(iVar14 + 0x19) = 1;
  }
  *(uint8_t *)(iVar14 + 0x1a) = 1;
  puVar6 = (uint *)in_r0[0x59];
  for (uVar19 = 0; uVar19 < *puVar6; uVar19 = uVar19 + 1) {
    *(uint32_t *)(*(int *)(*(int *)(iVar11 + 0x3c) + 4) + uVar19 * 4) =
         *(uint32_t *)(puVar6[1] + uVar19 * 4);
  }
  puVar6 = (uint *)in_r0[0x5a];
  for (uVar19 = 0; uVar19 < *puVar6; uVar19 = uVar19 + 1) {
    *(uint32_t *)(*(int *)(*(int *)(iVar11 + 0x40) + 4) + uVar19 * 4) =
         *(uint32_t *)(puVar6[1] + uVar19 * 4);
  }
  puVar6 = (uint *)in_r0[0x5b];
  for (uVar19 = 0; uVar19 < *puVar6; uVar19 = uVar19 + 1) {
    *(uint32_t *)(*(int *)(*(int *)(iVar11 + 0x44) + 4) + uVar19 * 4) =
         *(uint32_t *)(puVar6[1] + uVar19 * 4);
  }
  puVar6 = (uint *)in_r0[0x5c];
  for (uVar19 = 0; uVar19 < *puVar6; uVar19 = uVar19 + 1) {
    *(uint32_t *)(*(int *)(*(int *)(iVar11 + 0x48) + 4) + uVar19 * 4) =
         *(uint32_t *)(puVar6[1] + uVar19 * 4);
  }
  puVar6 = *(uint **)(iVar11 + 0x54);
  *(uint32_t *)(iVar11 + 0x4c) = in_r0[0x5d];
  puVar12 = (uint *)in_r0[0x5e];
  uVar19 = *puVar12;
  if (*puVar6 <= uVar19) {
    for (uVar20 = 0; uVar19 != uVar20; uVar20 = uVar20 + 1) {
      *(uint8_t *)(puVar6[1] + uVar20) = *(uint8_t *)(puVar12[1] + uVar20);
    }
  }
  iVar14 = DAT_0016646c;
  uVar23 = *(uint64_t *)(in_r0 + 0x3b);
  iVar15 = *(int *)(DAT_00166474 + 0x16629c);
  *(uint64_t *)(iVar15 + 8) = *(uint64_t *)(in_r0 + 0x39);
  *(uint64_t *)(iVar15 + 0x10) = uVar23;
  *(uint32_t *)(iVar2 + 0x2c) = in_r0[0x47];
  uVar13 = in_r0[0x33];
  *(uint32_t *)(iVar11 + 0x100) = in_r0[0x32];
  *(uint32_t *)(iVar11 + 0x104) = uVar13;
  uVar23 = *(uint64_t *)(in_r0 + 0x3f);
  *(uint64_t *)(iVar15 + 0x18) = *(uint64_t *)(in_r0 + 0x3d);
  *(uint64_t *)(iVar15 + 0x20) = uVar23;
  if (in_r0[0x6f] == iVar14) {
    *(uint8_t *)(iVar15 + 0x29) = *(uint8_t *)((long)in_r0 + 0x105);
    *(uint8_t *)(iVar15 + 0x28) = *(uint8_t *)(in_r0 + 0x41);
    *(uint8_t *)(iVar15 + 0x2a) = *(uint8_t *)((long)in_r0 + 0x106);
    *(uint8_t *)(iVar15 + 0x2b) = *(uint8_t *)((long)in_r0 + 0x107);
    *(uint8_t *)(iVar15 + 0x2c) = *(uint8_t *)(in_r0 + 0x42);
    *(uint8_t *)(iVar15 + 0x2d) = *(uint8_t *)((long)in_r0 + 0x109);
    *(uint8_t *)(iVar15 + 0x2e) = *(uint8_t *)((long)in_r0 + 0x10a);
    *(uint8_t *)(iVar15 + 0x2f) = *(uint8_t *)((long)in_r0 + 0x10b);
    *(uint8_t *)(iVar15 + 0x30) = *(uint8_t *)(in_r0 + 0x43);
    *(uint32_t *)(iVar11 + 0x178) = in_r0[0x6e];
    *(uint8_t *)(iVar15 + 0x31) = *(uint8_t *)((long)in_r0 + 0x119);
    *(uint8_t *)(iVar15 + 0x32) = *(uint8_t *)((long)in_r0 + 0x11a);
    *(uint8_t *)(iVar15 + 0x33) = *(uint8_t *)((long)in_r0 + 0x10d);
    *(uint8_t *)(iVar15 + 0x34) = *(uint8_t *)((long)in_r0 + 0x10e);
    *(uint8_t *)(iVar15 + 0x35) = *(uint8_t *)((long)in_r0 + 0x10f);
    *(uint8_t *)(iVar15 + 0x36) = *(uint8_t *)(in_r0 + 0x44);
    *(uint8_t *)(iVar15 + 0x37) = *(uint8_t *)((long)in_r0 + 0x111);
    *(uint8_t *)(iVar15 + 0x38) = *(uint8_t *)((long)in_r0 + 0x113);
    *(uint8_t *)(iVar15 + 0x39) = *(uint8_t *)((long)in_r0 + 0x112);
    *(uint8_t *)(iVar15 + 0x3a) = *(uint8_t *)(in_r0 + 0x45);
    *(uint32_t *)(iVar11 + 0x58) = in_r0[0x5f];
    puVar6 = (uint *)in_r0[0x6d];
    if ((puVar6 != (uint *)0x0) && (uVar19 = *puVar6, uVar19 != 0)) {
      for (uVar20 = 0; uVar20 < uVar19; uVar20 = uVar20 + 1) {
        *(uint32_t *)(*(int *)(*(int *)*piVar22 + 4) + uVar20 * 4) =
             *(uint32_t *)(puVar6[1] + uVar20 * 4);
        puVar6 = (uint *)in_r0[0x6d];
        uVar19 = *puVar6;
      }
    }
    iVar2 = *piVar22;
    for (iVar11 = 0; iVar11 != 4; iVar11 = iVar11 + 1) {
      *(uint32_t *)(iVar2 + iVar11 * 4 + 4) = in_r0[iVar11 + 0x69];
    }
    pSVar8 = (char *)((Status *)(*piVar22))->getStation();
    iVar2 = Station_getIndex(pSVar8);
    if (iVar2 == 0x6c) {
      ((Status *)(*piVar22))->getStation();
      puVar6 = (uint *)Station_getShips();
      if (puVar6 != (uint *)0x0) {
        for (uVar19 = 0; uVar19 < *puVar6; uVar19 = uVar19 + 1) {
          puVar12 = (uint *)Ship_getMods(*(char **)(*(int *)(in_r0[0x61] + 4) + uVar19 * 4));
          if (puVar12 != (uint *)0x0) {
            for (uVar20 = 0; uVar20 < *puVar12; uVar20 = uVar20 + 1) {
              Ship_addMod(*(char **)(puVar6[1] + uVar19 * 4),*(int *)(puVar12[1] + uVar20 * 4));
            }
          }
        }
      }
    }
    iVar2 = *piVar22;
    *(uint32_t *)(iVar2 + 0x118) = in_r0[0x38];
    *(uint8_t *)(iVar2 + 0x17c) = *(uint8_t *)(in_r0 + 0x70);
  }
  if (*piVar21 != iVar9) {
                    /* WARNING: Subroutine does not return */
  }
  return;
}
