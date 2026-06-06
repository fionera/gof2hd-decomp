# Status::nextCampaignMission
elf 0xa9a8c body 7626
Sig: undefined __stdcall nextCampaignMission(bool param_1)

## decompile
```c

/* Status::nextCampaignMission(bool) */

void Status::nextCampaignMission(bool param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  Status *this;
  void *pvVar18;
  Array *pAVar19;
  uint *puVar20;
  undefined4 *puVar21;
  Mission *pMVar22;
  Item *pIVar23;
  undefined4 *puVar24;
  uint uVar25;
  int iVar26;
  undefined4 uVar27;
  int iVar28;
  int iVar29;
  int iVar30;
  code *UNRECOVERED_JUMPTABLE;
  Ship *this_00;
  BluePrint *pBVar31;
  code *pcVar32;
  Status *pSVar33;
  Status *pSVar34;
  int *piVar35;
  int *piVar36;
  undefined4 uVar37;
  undefined4 uVar38;
  
  this = (Status *)(uint)param_1;
  uVar37 = *(undefined4 *)(this + 0x1b8);
  uVar38 = *(undefined4 *)(this + 0x1bc);
  pSVar33 = this + 0x100;
  pSVar34 = this + 0x1e8;
  iVar30 = DAT_000b9d14 + 0xb9aae;
  iVar29 = *(int *)(this + 0x1e8);
switchD_000b9ac6_caseD_34:
  iVar28 = iVar29;
  iVar29 = iVar28 + 1;
  iVar26 = 0;
  while (iVar17 = DAT_000bb908, iVar16 = DAT_000bb540, iVar15 = DAT_000bb4f8, iVar14 = DAT_000baecc,
        iVar13 = DAT_000baec4, iVar12 = DAT_000baec0, iVar11 = DAT_000baeb8, iVar10 = DAT_000baa34,
        iVar9 = DAT_000baa2c, iVar8 = DAT_000baa24, iVar7 = DAT_000baa20, iVar6 = DAT_000baa1c,
        iVar5 = DAT_000baa0c, iVar4 = DAT_000ba5d4, iVar3 = DAT_000ba5d0, iVar2 = DAT_000ba12c,
        iVar1 = DAT_000ba11c, iVar26 != 3) {
    iVar1 = iVar26 * 4;
    iVar26 = iVar26 + 1;
    if (iVar29 == *(int *)(iVar30 + iVar1)) {
      this[0x17c] = (Status)0x1;
    }
  }
  switch(iVar28) {
  case 0:
    goto switchD_000b9ac6_caseD_0;
  case 1:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9a,0,0x4e);
    setCampaignMission(this,pMVar22);
    goto LAB_000baaac;
  case 2:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x4e);
    goto LAB_000b9c30;
  case 3:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Ship::setCargo(*(Ship **)(this + 400),(Array *)0x0);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9a,0,0x4e);
    setCampaignMission(this,pMVar22);
    iVar29 = *(int *)(this + 0x198);
    uVar25 = 0x19;
    goto LAB_000bba20;
  case 4:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x4e);
    goto LAB_000b9c30;
  case 5:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Ship::removeAllCargo(*(Ship **)(this + 400));
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9e,0,0x4e);
    goto LAB_000b9c30;
  case 6:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x4e);
    goto LAB_000b9c30;
  case 7:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    puVar20 = (uint *)Ship::getEquipment(*(Ship **)(this + 400));
    if (puVar20 != (uint *)0x0) {
      piVar35 = *(int **)(DAT_000b9d18 + 0xb9cc0);
      for (uVar25 = 0; uVar25 < *puVar20; uVar25 = uVar25 + 1) {
        iVar29 = *(int *)(puVar20[1] + uVar25 * 4);
        if (iVar29 != 0) {
          Item::setUnsaleable(SUB41(iVar29,0));
          iVar29 = *(int *)(puVar20[1] + uVar25 * 4);
          Item::getSinglePrice(*(Item **)(*(int *)(*piVar35 + 4) + uVar25 * 4));
          Item::setPrice(iVar29);
        }
      }
    }
    puVar20 = (uint *)Ship::getCargo();
    if (puVar20 != (uint *)0x0) {
      piVar35 = *(int **)(DAT_000b9d1c + 0xb9d10);
      for (uVar25 = 0; uVar25 < *puVar20; uVar25 = uVar25 + 1) {
        iVar29 = *(int *)(puVar20[1] + uVar25 * 4);
        if (iVar29 != 0) {
          Item::setUnsaleable(SUB41(iVar29,0));
          iVar29 = *(int *)(puVar20[1] + uVar25 * 4);
          Item::getSinglePrice(*(Item **)(*(int *)(*piVar35 + 4) + uVar25 * 4));
          Item::setPrice(iVar29);
        }
      }
    }
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x4e);
    goto LAB_000b9c30;
  case 8:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x4e);
    goto LAB_000b9c30;
  case 9:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pIVar23 = (Item *)Ship::getFirstEquipmentOfSort(*(int *)(this + 400));
    Ship::removeEquipment(*(Ship **)(this + 400),pIVar23);
    this_00 = *(Ship **)(this + 400);
    pIVar23 = (Item *)Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000bb544 + 0xbb3e6) + 4) +
                                             0x158));
    Ship::addEquipment(this_00,pIVar23);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x4f);
    goto LAB_000b9c30;
  case 10:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x4c);
    goto LAB_000b9c30;
  case 0xb:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x4f);
    goto LAB_000b9c30;
  case 0xc:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x96,0,0);
    setCampaignMission(this,pMVar22);
    Mission::setStatusValue(**(int **)(*(int *)(this + 0x198) + 4));
    goto LAB_000bb522;
  case 0xd:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x4f);
    goto LAB_000b9c30;
  case 0xe:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x62);
    goto LAB_000b9c30;
  case 0xf:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x62);
    goto LAB_000b9c30;
  case 0x10:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x62);
    goto LAB_000b9c30;
  case 0x11:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Ship::setRace(*(int *)(this + 400));
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9c,0,0x38);
    goto LAB_000b9c30;
  case 0x12:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9c,0,0x37);
    goto LAB_000b9c30;
  case 0x13:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xbd,0,0x37);
    setCampaignMission(this,pMVar22);
    iVar29 = *(int *)(this + 0x198);
    uVar25 = 6;
    goto LAB_000bba20;
  case 0x14:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x37);
    goto LAB_000b9c30;
  case 0x15:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x37);
    goto LAB_000b9c30;
  case 0x16:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined1 *)(*(int *)(*(int *)(this + 0x38) + 4) + 6) = 1;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,20000,10);
    goto LAB_000b9c30;
  case 0x17:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined4 *)(this + 0x7c) = 9;
    *(undefined4 *)(this + 0x80) = 0x30;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x30);
    goto LAB_000b9c30;
  case 0x18:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    puVar20 = (uint *)Ship::getCargo();
    if (puVar20 == (uint *)0x0) goto LAB_000bb9dc;
    uVar25 = 0;
    goto LAB_000b9e86;
  case 0x19:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined4 *)(this + 0x7c) = 0xffffffff;
    *(undefined4 *)(this + 0x80) = 0xffffffff;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x30);
    goto LAB_000b9c30;
  case 0x1a:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x1b:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined4 *)(this + 0x7c) = 0x12;
    *(undefined4 *)(this + 0x80) = 0x5b;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x5b);
    goto LAB_000b9c30;
  case 0x1c:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,-1);
    goto LAB_000b9c30;
  case 0x1d:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9c,0,0x5b);
    goto LAB_000b9c30;
  case 0x1e:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,30000,0x62);
    goto LAB_000b9c30;
  case 0x1f:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x20:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,8,0,10);
    setCampaignMission(this,pMVar22);
    uVar37 = 0xa4;
    uVar27 = 0x32;
    uVar38 = **(undefined4 **)(*(int *)(this + 0x198) + 4);
    goto LAB_000bb84c;
  case 0x21:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Ship::removeCargo(*(int *)(this + 400),0xa4);
    uVar25 = 0;
    goto LAB_000b9fdc;
  case 0x22:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x1d);
    goto LAB_000b9c30;
  case 0x23:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xc,0,0x1b);
    goto LAB_000b9c30;
  case 0x24:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa0,0,0x1b);
    break;
  case 0x25:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x16);
    goto LAB_000b9c30;
  case 0x26:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x1e);
    goto LAB_000b9c30;
  case 0x27:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa1,0,-1);
    goto LAB_000b9c30;
  case 0x28:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,-1);
    goto LAB_000b9c30;
  case 0x29:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined4 *)(this + 0x7c) = 0xfffffff6;
    *(undefined4 *)(this + 0x80) = 0xfffffff6;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa0,0,-1);
    goto LAB_000b9c30;
  case 0x2a:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x2b:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x2c:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined4 *)(this + 0x7c) = 0xfffffff6;
    *(undefined4 *)(this + 0x80) = 0xfffffff6;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22);
    setCampaignMission(this,pMVar22);
    Mission::setVisible((Mission *)**(undefined4 **)(*(int *)(this + 0x198) + 4),false);
    *(uint *)(this + 0x1b0) =
         *(int *)(this + 0x1b0) + 40000U & ~((int)(*(int *)(this + 0x1b0) + 40000U) >> 0x1f);
    return;
  default:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    return;
  case 0x2e:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x4a);
    goto LAB_000b9c30;
  case 0x2f:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    piVar35 = *(int **)(iVar1 + 0xb9dae);
    *(undefined4 *)(this + 0x8c) = *(undefined4 *)(this + 400);
    *(undefined4 *)(this + 400) = 0;
    Ship::makeShip(*(int *)(*(int *)(*piVar35 + 4) + 0x24));
    setShip((Ship *)this);
    Ship::setRace(*(int *)(this + 400));
    iVar29 = Item::makeItem();
    piVar35 = *(int **)(DAT_000ba124 + 0xb9dfe);
    Ship::setEquipment(*(Item **)(*piVar35 + 400),iVar29);
    iVar29 = Item::makeItem();
    Ship::setEquipment(*(Item **)(*piVar35 + 400),iVar29);
    *(undefined4 *)(*piVar35 + 0x30) = 0;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x3a);
    goto LAB_000b9c30;
  case 0x30:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Ship::makeShip(*(int *)(*(int *)(**(int **)(iVar17 + 0xbb556) + 4) + 0xa4));
    setShip((Ship *)this);
    Ship::setRace(*(int *)(this + 400));
    piVar35 = *(int **)(DAT_000bb90c + 0xbb580);
    piVar36 = *(int **)(DAT_000bb910 + 0xbb586);
    for (iVar29 = 0; iVar29 != 3; iVar29 = iVar29 + 1) {
      iVar30 = Item::makeItem();
      Ship::setEquipment(*(Item **)(*piVar36 + 400),iVar30);
    }
    UNRECOVERED_JUMPTABLE = *(code **)(DAT_000bb914 + 0xbb5b6);
    uVar37 = (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(*(int *)(*piVar35 + 4) + 0xe8));
    pcVar32 = *(code **)(DAT_000bb918 + 0xbb5c4);
    (*pcVar32)(*(undefined4 *)(*piVar36 + 400),uVar37,0);
    uVar37 = (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(*(int *)(*piVar35 + 4) + 0x14c));
    (*pcVar32)(*(undefined4 *)(*piVar36 + 400),uVar37,1);
    uVar37 = (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(*(int *)(*piVar35 + 4) + 0xd0));
    (*pcVar32)(*(undefined4 *)(*piVar36 + 400),uVar37,2);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9c,0,0x3a);
    goto LAB_000b9c30;
  case 0x31:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9c,0,0x3e);
    goto LAB_000b9c30;
  case 0x32:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0x9c,0,0x19);
    goto LAB_000b9c30;
  case 0x33:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa0,0,0x19);
    break;
  case 0x34:
  case 0x80:
    goto switchD_000b9ac6_caseD_34;
  case 0x35:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,DAT_000ba128,0x4a);
    goto LAB_000b9c30;
  case 0x36:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined1 *)(*(int *)(*(int *)(this + 0x38) + 4) + 0x17) = 1;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x65);
    setCampaignMission(this,pMVar22);
    setShip((Ship *)this);
    *(undefined4 *)(this + 0x8c) = 0;
    return;
  case 0x37:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    piVar35 = *(int **)(iVar4 + 0xba3d4);
    *(undefined4 *)(this + 0x8c) = *(undefined4 *)(this + 400);
    *(undefined4 *)(this + 400) = 0;
    Ship::makeShip(*(int *)(*(int *)(*piVar35 + 4) + 0x9c));
    setShip((Ship *)this);
    Ship::setRace(*(int *)(this + 400));
    piVar36 = *(int **)(DAT_000ba5d8 + 0xba40e);
    pcVar32 = *(code **)(DAT_000ba5dc + 0xba41c);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0x2d4));
    piVar35 = *(int **)(DAT_000ba5e0 + 0xba42c);
    UNRECOVERED_JUMPTABLE = *(code **)(DAT_000ba5e4 + 0xba438);
    (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(*piVar35 + 400),uVar37,0);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0xd0));
    (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(*piVar35 + 400),uVar37,0);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0xe8));
    (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(*piVar35 + 400),uVar37,1);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0x14c));
    (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(*piVar35 + 400),uVar37,2);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x66);
    goto LAB_000b9c30;
  case 0x38:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,DAT_000ba5e8,0x65);
    goto LAB_000b9c30;
  case 0x39:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa6,0,0x65);
    setCampaignMission(this,pMVar22);
    Mission::setProductionGoods((Mission *)**(undefined4 **)(*(int *)(this + 0x198) + 4),0xb3,10);
    uVar25 = 0;
    goto LAB_000ba500;
  case 0x3a:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    piVar35 = *(int **)(iVar2 + 0xba0ca);
    puVar21 = (undefined4 *)(*piVar35 + 0x90);
    if ((Array<int> *)*puVar21 != (Array<int> *)0x0) {
      pvVar18 = (void *)Array<int>::~Array((Array<int> *)*puVar21);
      operator_delete(pvVar18);
      puVar21 = (undefined4 *)(*piVar35 + 0x90);
    }
    *puVar21 = 0;
    pAVar19 = operator_new(0xc);
    Array<int>::Array();
    *(Array **)(*piVar35 + 0x90) = pAVar19;
    ArraySetLength<int>(3,pAVar19);
    puVar21 = *(undefined4 **)(*(int *)(*piVar35 + 0x90) + 4);
    *puVar21 = 0x38;
    puVar21[1] = 0x2d;
    puVar21[2] = 0x16;
    for (uVar25 = 0; uVar25 < **(uint **)(this + 0x18); uVar25 = uVar25 + 1) {
      iVar29 = BluePrint::getIndex(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar25 * 4));
      if (iVar29 == 0xb3) {
        BluePrint::lock(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4));
      }
    }
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa3,0,0x65);
    setCampaignMission(this,pMVar22);
    Mission::setStatusValue(**(int **)(*(int *)(this + 0x198) + 4));
    *(undefined4 *)(*piVar35 + 0x10c) = 0;
    return;
  case 0x3b:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    iVar29 = Mission::getStatusValue();
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,iVar29 * 50000 + 50000,0x65);
    setCampaignMission(this,pMVar22);
    uVar37 = **(undefined4 **)(*(int *)(this + 0x198) + 4);
    uVar25 = (uint)(0 < iVar29);
    goto LAB_000bba24;
  case 0x3c:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa4,0,0x65);
    goto LAB_000b9c30;
  case 0x3d:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined1 *)(*(int *)(*(int *)(this + 0x38) + 4) + 0x16) = 1;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,100);
    goto LAB_000b9c30;
  case 0x3e:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined1 *)(*(int *)(*(int *)(this + 0x38) + 4) + 0x18) = 1;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x67);
    goto LAB_000b9c30;
  case 0x3f:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x68);
    goto LAB_000b9c30;
  case 0x40:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,100);
    goto LAB_000b9c30;
  case 0x41:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x65);
    goto LAB_000b9c30;
  case 0x42:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x68);
    setCampaignMission(this,pMVar22);
    Mission::setStatusValue(**(int **)(*(int *)(this + 0x198) + 4));
    piVar36 = *(int **)(DAT_000ba9fc + 0xba624);
    piVar35 = *(int **)(DAT_000baa00 + 0xba626);
    uVar38 = *(undefined4 *)(*piVar35 + 0x19c);
    pcVar32 = *(code **)(DAT_000baa04 + 0xba634);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0xf0),5,0);
    UNRECOVERED_JUMPTABLE = *(code **)(DAT_000baa08 + 0xba64c);
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37);
    uVar38 = *(undefined4 *)(*piVar35 + 0x19c);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0xf4),5,0);
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37);
    uVar38 = *(undefined4 *)(*piVar35 + 0x19c);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0xf8),5,0);
                    /* WARNING: Could not recover jumptable at 0x000ba68c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37);
    return;
  case 0x43:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,8,0,0x42);
    setCampaignMission(this,pMVar22);
    iVar29 = *(int *)(this + 0x198);
    uVar37 = 0xaf;
    goto LAB_000bb846;
  case 0x44:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Ship::removeCargo(*(int *)(**(int **)(iVar5 + 0xba6c6) + 400));
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,6,0,0x42);
    goto LAB_000b9c30;
  case 0x45:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,6,0,0x41);
    goto LAB_000b9c30;
  case 0x46:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x42);
    goto LAB_000b9c30;
  case 0x47:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    for (uVar25 = 0; uVar25 < **(uint **)(this + 0x18); uVar25 = uVar25 + 1) {
      iVar29 = BluePrint::getIndex(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar25 * 4));
      if (iVar29 == 0xb7) {
        BluePrint::unlock(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4));
      }
    }
    pIVar23 = *(Item **)(**(int **)(DAT_000baa14 + 0xba760) + 400);
    Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000baa10 + 0xba75e) + 4) + 700));
    Ship::addCargo(pIVar23);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa4,DAT_000baa18,0x65);
    setCampaignMission(this,pMVar22);
    *(undefined4 *)(this + 0x10c) = 0;
    return;
  case 0x48:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,10,0,0x51);
    goto LAB_000b9c30;
  case 0x49:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,100);
    goto LAB_000b9c30;
  case 0x4a:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Station::removeShips(*(Station **)(**(int **)(iVar6 + 0xba7ca) + 0x19c));
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,100);
    goto LAB_000b9c30;
  case 0x4b:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,100);
    goto LAB_000b9c30;
  case 0x4c:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    iVar29 = Ship::getFirstEquipmentOfSort(*(int *)(**(int **)(iVar7 + 0xba818) + 400));
    if (iVar29 != 0) {
      Item::setUnsaleable(SUB41(iVar29,0));
    }
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x65);
    goto LAB_000b9c30;
  case 0x4d:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    piVar35 = *(int **)(iVar3 + 0xba1fe);
    pIVar23 = (Item *)Ship::getFirstEquipmentOfSort(*(int *)(*piVar35 + 400));
    if (pIVar23 == (Item *)0x0) {
      iVar29 = Ship::getCargo((int)*(Ship **)(*piVar35 + 400));
      if (iVar29 != 0) {
        Ship::removeCargo(*(Item **)(*piVar35 + 400));
      }
    }
    else {
      Ship::removeEquipment(*(Ship **)(*piVar35 + 400),pIVar23);
    }
    for (uVar25 = 0; uVar25 < **(uint **)(this + 0x18); uVar25 = uVar25 + 1) {
      iVar29 = BluePrint::isEmpty(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar25 * 4));
      if ((iVar29 == 0) &&
         (iVar29 = BluePrint::getStationIndex
                             (*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4)),
         iVar29 == 0x65)) {
        BluePrint::reset(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4));
      }
    }
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x65);
    goto LAB_000b9c30;
  case 0x4e:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa5,0,-1);
    goto LAB_000b9c30;
  case 0x4f:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,1,0,100);
    goto LAB_000b9c30;
  case 0x50:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,-1);
    goto LAB_000b9c30;
  case 0x51:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,100);
    goto LAB_000b9c30;
  case 0x52:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,100);
    goto LAB_000b9c30;
  case 0x53:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    piVar35 = *(int **)(iVar8 + 0xba8ca);
    iVar29 = Ship::getFirstEquipmentOfSort(*(int *)(*piVar35 + 400));
    if ((iVar29 != 0) || (iVar29 = Ship::getCargo(*(int *)(*piVar35 + 400)), iVar29 != 0)) {
      Item::setUnsaleable(SUB41(iVar29,0));
    }
    pIVar23 = *(Item **)(*piVar35 + 400);
    Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000baa28 + 0xba8f6) + 4) + 0x154));
    Ship::addCargo(pIVar23);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22);
    break;
  case 0x54:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa4,0,0);
    goto LAB_000b9c30;
  case 0x55:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,100);
    goto LAB_000b9c30;
  case 0x56:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,10);
    goto LAB_000b9c30;
  case 0x57:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x58:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    piVar35 = *(int **)(iVar9 + 0xba988);
    Ship::removeCargo(*(int *)(*piVar35 + 400),0x68);
    *(undefined2 *)(*(int *)(*(int *)(*piVar35 + 0x38) + 4) + 0x1b) = 0x101;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x6d);
    goto LAB_000b9c30;
  case 0x59:
    puVar21 = (undefined4 *)(DAT_000baa30 + 0xba9c2);
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    puVar24 = *(undefined4 **)(iVar10 + 0xba9cc);
    *(int *)(*(int *)*puVar21 + 0x1d0) = *(int *)(*(int *)*puVar21 + 0x1d0) + -1;
    iVar29 = Galaxy::getVisited((Galaxy *)*puVar24);
    *(undefined1 *)(iVar29 + 0x6d) = 0;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x5a:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined2 *)(*(int *)(*(int *)(this + 0x38) + 4) + 0x1b) = 0x101;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb8,0,0x6e);
    setCampaignMission(this,pMVar22);
    Mission::setProductionGoods((Mission *)**(undefined4 **)(*(int *)(this + 0x198) + 4),0,10);
    goto LAB_000bb0e2;
  case 0x5b:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb8,0,0x71);
    setCampaignMission(this,pMVar22);
    Mission::setProductionGoods((Mission *)**(undefined4 **)(*(int *)(this + 0x198) + 4),0,10);
LAB_000baaac:
    iVar29 = *(int *)(this + 0x198);
    uVar25 = 10;
    goto LAB_000bba20;
  case 0x5c:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x72);
    goto LAB_000b9c30;
  case 0x5d:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Item::makeItem(*(int *)(*(int *)(**(int **)(iVar11 + 0xbaae0) + 4) + 0x334));
    Ship::addCargo(*(Item **)(**(int **)(DAT_000baebc + 0xbaaf8) + 400));
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb8,0,0x6f);
    setCampaignMission(this,pMVar22);
    Mission::setProductionGoods((Mission *)**(undefined4 **)(*(int *)(this + 0x198) + 4),0,0x53);
    iVar29 = *(int *)(this + 0x198);
    goto LAB_000bb0e8;
  case 0x5e:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,10);
    goto LAB_000bba12;
  case 0x5f:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x62);
    goto LAB_000b9c30;
  case 0x60:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x55);
    goto LAB_000b9c30;
  case 0x61:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined1 *)(*(int *)(*(int *)(**(int **)(iVar12 + 0xbab9c) + 0x38) + 4) + 0x1d) = 1;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x78);
    goto LAB_000b9c30;
  case 0x62:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,10);
    goto LAB_000bba12;
  case 99:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x62);
    goto LAB_000b9c30;
  case 100:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x62);
    goto LAB_000b9c30;
  case 0x65:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Item::makeItem(*(int *)(*(int *)(**(int **)(iVar13 + 0xbac06) + 4) + 0x33c));
    Ship::addCargo(*(Item **)(**(int **)(DAT_000baec8 + 0xbac1e) + 400));
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb8,0,0x71);
    setCampaignMission(this,pMVar22);
    Mission::setProductionGoods((Mission *)**(undefined4 **)(*(int *)(this + 0x198) + 4),0,0x6a4);
    iVar29 = *(int *)(this + 0x198);
    uVar25 = 0x6a4;
    goto LAB_000bba20;
  case 0x66:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x67:
    uVar25 = 0;
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    goto LAB_000bac9c;
  case 0x68:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x6d);
    goto LAB_000b9c30;
  case 0x69:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x6f);
    goto LAB_000b9c30;
  case 0x6b:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x6c:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,0x72);
    goto LAB_000bba12;
  case 0x6d:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xab,0,10);
    goto LAB_000b9c30;
  case 0x6e:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xab,0,0x26);
    goto LAB_000b9c30;
  case 0x6f:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xac,0,0x26);
    setCampaignMission(this,pMVar22);
    iVar29 = *(int *)(this + 0x198);
    uVar37 = 0x92;
    goto LAB_000bb846;
  case 0x70:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Ship::removeCargo(*(int *)(**(int **)(iVar14 + 0xbad78) + 400),0x92);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xab,0,0x52);
    goto LAB_000b9c30;
  case 0x71:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x53);
    goto LAB_000b9c30;
  case 0x72:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xab,0,0x52);
    goto LAB_000b9c30;
  case 0x73:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xab,0,0x5d);
    goto LAB_000bba12;
  case 0x74:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined1 *)(*(int *)(*(int *)(this + 0x38) + 4) + 0x1e) = 1;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x7e);
    goto LAB_000b9c30;
  case 0x75:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,8,0,0x7e);
    goto LAB_000bae8a;
  case 0x76:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    puVar20 = (uint *)Ship::getCargo();
    if (puVar20 == (uint *)0x0) goto LAB_000bba00;
    uVar25 = 0;
    goto LAB_000bae48;
  case 0x77:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x28);
    goto LAB_000b9c30;
  case 0x78:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,8,0,0x5d);
LAB_000bae8a:
    setCampaignMission(this,pMVar22);
    iVar29 = *(int *)(this + 0x198);
    uVar37 = 0xd1;
    goto LAB_000bb846;
  case 0x79:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    puVar20 = (uint *)Ship::getCargo();
    if (puVar20 == (uint *)0x0) goto LAB_000bba3c;
    uVar25 = 0;
    goto LAB_000baee2;
  case 0x7a:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x79);
    goto LAB_000b9c30;
  case 0x7b:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x79);
    goto LAB_000b9c30;
  case 0x7c:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x37);
    goto LAB_000b9c30;
  case 0x7d:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,0x78);
    goto LAB_000bba12;
  case 0x7e:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x62);
    goto LAB_000b9c30;
  case 0x7f:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22);
    break;
  case 0x81:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22);
    break;
  case 0x82:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x70);
    goto LAB_000b9c30;
  case 0x83:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x70);
    goto LAB_000b9c30;
  case 0x84:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,0x78);
    goto LAB_000bba12;
  case 0x85:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x16);
    goto LAB_000bba12;
  case 0x86:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xae,0,0x67);
    setCampaignMission(this,pMVar22);
    Mission::setProductionGoods((Mission *)**(undefined4 **)(*(int *)(this + 0x198) + 4),0x9b,0x8c);
    goto LAB_000bba1a;
  case 0x87:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x70);
    goto LAB_000b9c30;
  case 0x88:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x3a);
    goto LAB_000b9c30;
  case 0x89:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x3a);
    goto LAB_000b9c30;
  case 0x8a:
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    *(undefined1 *)(*(int *)(*(int *)(this + 0x38) + 4) + 0x1f) = 1;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa8,0,0x83);
    setCampaignMission(this,pMVar22);
LAB_000bb0e2:
    iVar29 = *(int *)(this + 0x198);
LAB_000bb0e8:
    Mission::setStatusValue(**(int **)(iVar29 + 4));
    *(undefined4 *)(this + 0x178) = 0;
    return;
  case 0x8b:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x70);
    goto LAB_000b9c30;
  case 0x8c:
    uVar25 = 0;
    *(int *)(this + 0x1e8) = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    goto LAB_000bb134;
  case 0x8d:
    puVar21 = (undefined4 *)(DAT_000bb4fc + 0xbb146);
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    piVar35 = (int *)*puVar21;
    piVar36 = *(int **)(iVar15 + 0xbb14c);
    uVar38 = *(undefined4 *)(*piVar35 + 400);
    pcVar32 = *(code **)(DAT_000bb538 + 0xbb16c);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0x314),0xf);
    UNRECOVERED_JUMPTABLE = *(code **)(DAT_000bb53c + 0xbb17a);
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37);
    uVar38 = *(undefined4 *)(*piVar35 + 400);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0x310),1);
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37);
    uVar38 = *(undefined4 *)(*piVar35 + 400);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar36 + 4) + 0x318),1);
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x4f);
    goto LAB_000b9c30;
  case 0x8e:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,8,0,0x70);
    setCampaignMission(this,pMVar22);
    iVar29 = *(int *)(this + 0x198);
    uVar37 = 0xd2;
    goto LAB_000bb846;
  case 0x8f:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    Ship::removeCargo(*(int *)(**(int **)(iVar16 + 0xbb204) + 400),0xd2);
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,0x70);
    goto LAB_000bba12;
  case 0x90:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x70);
    goto LAB_000b9c30;
  case 0x91:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x70);
    goto LAB_000b9c30;
  case 0x92:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,-1);
    goto LAB_000b9c30;
  case 0x93:
  case 0x94:
  case 0x95:
  case 0x96:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xab,0,0x60);
    goto LAB_000b9c30;
  case 0x97:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa5,0,-1);
    goto LAB_000b9c30;
  case 0x98:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,0x62);
    goto LAB_000b9c30;
  case 0x99:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,-1);
    goto LAB_000b9c30;
  case 0x9a:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa4,0,0);
    goto LAB_000b9c30;
  case 0x9b:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,99);
    goto LAB_000b9c30;
  case 0x9c:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x70);
    goto LAB_000b9c30;
  case 0x9d:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,4,0,0x6f);
    goto LAB_000b9c30;
  case 0x9e:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xb,0,10);
    goto LAB_000b9c30;
  case 0x9f:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,10);
    goto LAB_000b9c30;
  case 0xa0:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,0x5d);
    goto LAB_000bba12;
  case 0xa1:
    *(int *)pSVar34 = iVar29;
    *(undefined4 *)pSVar33 = uVar37;
    *(undefined4 *)(this + 0x104) = uVar38;
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22);
  }
  setCampaignMission(this,pMVar22);
LAB_000bb522:
  (*(code *)(DAT_001abf44 + 0x1abf48))(**(undefined4 **)(*(int *)(this + 0x198) + 4),0);
  return;
LAB_000bb134:
  if (**(uint **)(this + 0x18) <= uVar25) goto LAB_000bb8f2;
  iVar29 = BluePrint::getIndex(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar25 * 4));
  if (iVar29 == 0xd2) {
    BluePrint::unlock(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4));
    piVar35 = *(int **)(DAT_000bb924 + 0xbb86a);
    uVar38 = *(undefined4 *)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4);
    pcVar32 = *(code **)(DAT_000bb928 + 0xbb878);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar35 + 4) + 0x324));
    UNRECOVERED_JUMPTABLE = *(code **)(DAT_000bb92c + 0xbb892);
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37,0x34f,0x70);
    uVar38 = *(undefined4 *)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar35 + 4) + 0x328));
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37,0x342,0x70);
    uVar38 = *(undefined4 *)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar35 + 4) + 0x32c));
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37,0x35d,0x70);
    uVar38 = *(undefined4 *)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4);
    uVar37 = (*pcVar32)(*(undefined4 *)(*(int *)(*piVar35 + 4) + 0x330));
    (*UNRECOVERED_JUMPTABLE)(uVar38,uVar37,0x37c,0x70);
    goto LAB_000bb8f2;
  }
  uVar25 = uVar25 + 1;
  goto LAB_000bb134;
LAB_000bb8f2:
  pMVar22 = operator_new(0x78);
  Mission::Mission(pMVar22,0xb,0,0x4e);
  goto LAB_000b9c30;
LAB_000baee2:
  if (*puVar20 <= uVar25) goto LAB_000bba3c;
  iVar29 = Item::getIndex(*(Item **)(puVar20[1] + uVar25 * 4));
  if (iVar29 == 0xd1) {
    Item::setUnsaleable(SUB41(*(undefined4 *)(puVar20[1] + uVar25 * 4),0));
    goto LAB_000bba3c;
  }
  uVar25 = uVar25 + 1;
  goto LAB_000baee2;
LAB_000bba3c:
  Ship::removeCargo(*(int *)(**(int **)(DAT_000bbbb4 + 0xbba46) + 400),0xd1);
  pMVar22 = operator_new(0x78);
  Mission::Mission(pMVar22,0xb,0,10);
  goto LAB_000b9c30;
LAB_000bae48:
  if (*puVar20 <= uVar25) {
LAB_000bba00:
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xaa,0,10);
LAB_000bba12:
    setCampaignMission(this,pMVar22);
LAB_000bba1a:
    iVar29 = *(int *)(this + 0x198);
    uVar25 = 0;
LAB_000bba20:
    uVar37 = **(undefined4 **)(iVar29 + 4);
LAB_000bba24:
    (*(code *)(DAT_001abf64 + 0x1abf68))(uVar37,uVar25);
    return;
  }
  iVar29 = Item::getIndex(*(Item **)(puVar20[1] + uVar25 * 4));
  if (iVar29 == 0xd1) {
    Item::setUnsaleable(SUB41(*(undefined4 *)(puVar20[1] + uVar25 * 4),0));
    goto LAB_000bba00;
  }
  uVar25 = uVar25 + 1;
  goto LAB_000bae48;
LAB_000bac9c:
  if (**(uint **)(this + 0x18) <= uVar25) {
LAB_000bb826:
    pMVar22 = operator_new(0x78);
    Mission::Mission(pMVar22,0xa6,0,10);
    setCampaignMission(this,pMVar22);
    iVar29 = *(int *)(this + 0x198);
    uVar37 = 0xce;
LAB_000bb846:
    uVar38 = **(undefined4 **)(iVar29 + 4);
    uVar27 = 1;
LAB_000bb84c:
    (*(code *)(DAT_001abf54 + 0x1abf58))(uVar38,uVar37,uVar27);
    return;
  }
  iVar29 = BluePrint::getIndex(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar25 * 4));
  if (iVar29 == 0xce) {
    BluePrint::unlock(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4));
    pBVar31 = *(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4);
    pIVar23 = (Item *)Item::makeItem();
    BluePrint::addItem(pBVar31,pIVar23,10,10);
    goto LAB_000bb826;
  }
  uVar25 = uVar25 + 1;
  goto LAB_000bac9c;
LAB_000ba500:
  if (**(uint **)(this + 0x18) <= uVar25) {
LAB_000bb95a:
    setShip((Ship *)this);
    *(undefined4 *)(this + 0x8c) = 0;
    return;
  }
  iVar29 = BluePrint::getIndex(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar25 * 4));
  if (iVar29 == 0xb3) {
    BluePrint::unlock(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4));
    BluePrint::addItem(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4),
                       *(Item **)(*(int *)(**(int **)(DAT_000bbbb0 + 0xbb946) + 4) + 0x1fc),5,0x65);
    goto LAB_000bb95a;
  }
  uVar25 = uVar25 + 1;
  goto LAB_000ba500;
LAB_000b9fdc:
  if (**(uint **)(this + 0x18) <= uVar25) goto LAB_000bb7de;
  iVar29 = BluePrint::getIndex(*(BluePrint **)((*(uint **)(this + 0x18))[1] + uVar25 * 4));
  if (iVar29 == 0x55) {
    BluePrint::unlock(*(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4));
    pBVar31 = *(BluePrint **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar25 * 4);
    pIVar23 = (Item *)Item::makeItem();
    BluePrint::addItem(pBVar31,pIVar23,0x32,10);
    goto LAB_000bb7de;
  }
  uVar25 = uVar25 + 1;
  goto LAB_000b9fdc;
LAB_000bb7de:
  pMVar22 = operator_new(0x78);
  Mission::Mission(pMVar22,0xb,0,0x1e);
  goto LAB_000b9c30;
LAB_000b9e86:
  if (*puVar20 <= uVar25) goto LAB_000bb9dc;
  iVar29 = Item::getIndex(*(Item **)(puVar20[1] + uVar25 * 4));
  if (iVar29 == 0x83) {
    Item::setUnsaleable(SUB41(*(undefined4 *)(puVar20[1] + uVar25 * 4),0));
    goto LAB_000bb9dc;
  }
  uVar25 = uVar25 + 1;
  goto LAB_000b9e86;
switchD_000b9ac6_caseD_0:
  *(int *)pSVar34 = iVar29;
  *(undefined4 *)pSVar33 = uVar37;
  *(undefined4 *)(this + 0x104) = uVar38;
  pMVar22 = operator_new(0x78);
  Mission::Mission(pMVar22,0xb,0,0x4e);
  setCampaignMission(this,pMVar22);
  *(undefined4 *)(this + 0x194) = **(undefined4 **)(*(int *)(this + 0x198) + 4);
  return;
LAB_000bb9dc:
  pMVar22 = operator_new(0x78);
  Mission::Mission(pMVar22,0x9c,0,-1);
LAB_000b9c30:
  (*(code *)(DAT_001abf34 + 0x1abf38))(this,pMVar22);
  return;
}

```
## target disasm
```
  000b9a8c: push {r4,r5,r6,r7,lr}
  000b9a8e: add r7,sp,#0xc
  000b9a90: push {r8,r9,r10}
  000b9a94: ldrd r12,lr,[r0,#0x1b8]
  000b9a98: add.w r8,r0,#0x100
  000b9a9c: ldr.w r3,[r0,#0x1e8]
  000b9aa0: add.w r9,r0,#0x1e8
  000b9aa4: ldr r5,[0x000b9d14]
  000b9aa6: mov r4,r0
  000b9aa8: movs r0,#0x1
  000b9aaa: add r5,pc
  000b9aac: b 0x000b9c0e
  000b9aae: ldr.w r6,[r5,r2,lsl #0x2]
  000b9ab2: adds r2,#0x1
  000b9ab4: cmp r3,r6
  000b9ab6: it eq
  000b9ab8: strb.eq.w r0,[r4,#0x17c]
  000b9abc: cmp r2,#0x3
  000b9abe: bne 0x000b9aae
  000b9ac0: cmp r1,#0xa1
  000b9ac2: bhi.w 0x000b9ee4
  000b9ac6: tbh [pc,r1]
  000b9c0e: mov r1,r3
  000b9c10: adds r3,#0x1
  000b9c12: movs r2,#0x0
  000b9c14: b 0x000b9abc
  000b9c16: movs r0,#0x78
  000b9c18: str.w r3,[r9,#0x0]
  000b9c1c: strd r12,lr,[r8,#0x0]
  000b9c20: blx 0x0006eb24
  000b9c24: mov r5,r0
  000b9c26: movs r1,#0xab
  000b9c28: movs r2,#0x0
  000b9c2a: movs r3,#0x60
  000b9c2c: blx 0x00073834
  000b9c30: mov r0,r4
  000b9c32: mov r1,r5
  000b9c34: pop.w {r8,r9,r10}
  000b9c38: pop.w {r4,r5,r6,r7,lr}
  000b9c3c: b.w 0x001abf28
  000b9c40: mvn r0,#0x9
  000b9c44: str.w r3,[r4,#0x1e8]
  000b9c48: strd r12,lr,[r8,#0x0]
  000b9c4c: strd r0,r0,[r4,#0x7c]
  000b9c50: movs r0,#0x78
  000b9c52: blx 0x0006eb24
  000b9c56: mov r5,r0
  000b9c58: blx 0x0007384c
  000b9c5c: mov r0,r4
  000b9c5e: mov r1,r5
  000b9c60: blx 0x00073840
  000b9c64: ldr.w r0,[r4,#0x198]
  000b9c68: movs r1,#0x0
  000b9c6a: ldr r0,[r0,#0x4]
  000b9c6c: ldr r0,[r0,#0x0]
  000b9c6e: blx 0x00073858
  000b9c72: ldr.w r0,[r4,#0x1b0]
  000b9c76: movw r1,#0x9c40
  000b9c7a: add r0,r1
  000b9c7c: bic.w r0,r0,r0, asr #0x1f
  000b9c80: str.w r0,[r4,#0x1b0]
  000b9c84: b.w 0x000bb96a
  000b9c88: movs r0,#0x78
  000b9c8a: str.w r3,[r9,#0x0]
  000b9c8e: strd r12,lr,[r8,#0x0]
  000b9c92: blx 0x0006eb24
  000b9c96: mov r5,r0
  000b9c98: movs r1,#0xb
  000b9c9a: movs r2,#0x0
  000b9c9c: movs r3,#0x37
  000b9c9e: blx 0x00073834
  000b9ca2: b 0x000b9c30
  000b9ca4: str.w r3,[r4,#0x1e8]
  000b9ca8: strd r12,lr,[r8,#0x0]
  000b9cac: ldr.w r0,[r4,#0x190]
  000b9cb0: blx 0x0007333c
  000b9cb4: mov r8,r0
  000b9cb6: cbz r0,0x000b9cfc
  000b9cb8: ldr r0,[0x000b9d18]
  000b9cba: movs r5,#0x0
  000b9cbc: add r0,pc
  000b9cbe: ldr.w r9,[r0,#0x0]
  000b9cc2: b 0x000b9cf4
  000b9cc4: ldr.w r0,[r8,#0x4]
  000b9cc8: ldr.w r0,[r0,r5,lsl #0x2]
  000b9ccc: cbz r0,0x000b9cf2
  000b9cce: movs r1,#0x0
  000b9cd0: blx 0x00073330
  000b9cd4: ldr.w r0,[r9,#0x0]
  000b9cd8: ldr.w r1,[r8,#0x4]
  000b9cdc: ldr r0,[r0,#0x4]
  000b9cde: ldr.w r6,[r1,r5,lsl #0x2]
  000b9ce2: ldr.w r0,[r0,r5,lsl #0x2]
  000b9ce6: blx 0x00071944
  000b9cea: mov r1,r0
  000b9cec: mov r0,r6
  000b9cee: blx 0x00073864
  000b9cf2: adds r5,#0x1
  000b9cf4: ldr.w r0,[r8,#0x0]
  000b9cf8: cmp r5,r0
  000b9cfa: bcc 0x000b9cc4
  000b9cfc: ldr.w r0,[r4,#0x190]
  000b9d00: blx 0x00073750
  000b9d04: mov r8,r0
  000b9d06: cbz r0,0x000b9d58
  000b9d08: ldr r0,[0x000b9d1c]
  000b9d0a: movs r5,#0x0
  000b9d0c: add r0,pc
  000b9d0e: ldr.w r9,[r0,#0x0]
  000b9d12: b 0x000b9d50
  000b9d20: ldr.w r0,[r8,#0x4]
  000b9d24: ldr.w r0,[r0,r5,lsl #0x2]
  000b9d28: cbz r0,0x000b9d4e
  000b9d2a: movs r1,#0x0
  000b9d2c: blx 0x00073330
  000b9d30: ldr.w r0,[r9,#0x0]
  000b9d34: ldr.w r1,[r8,#0x4]
  000b9d38: ldr r0,[r0,#0x4]
  000b9d3a: ldr.w r6,[r1,r5,lsl #0x2]
  000b9d3e: ldr.w r0,[r0,r5,lsl #0x2]
  000b9d42: blx 0x00071944
  000b9d46: mov r1,r0
  000b9d48: mov r0,r6
  000b9d4a: blx 0x00073864
  000b9d4e: adds r5,#0x1
  000b9d50: ldr.w r0,[r8,#0x0]
  000b9d54: cmp r5,r0
  000b9d56: bcc 0x000b9d20
  000b9d58: movs r0,#0x78
  000b9d5a: blx 0x0006eb24
  000b9d5e: mov r5,r0
  000b9d60: movs r1,#0xb
  000b9d62: movs r2,#0x0
  000b9d64: movs r3,#0x4e
  000b9d66: blx 0x00073834
  000b9d6a: b 0x000b9c30
  000b9d6c: movs r0,#0x78
  000b9d6e: str.w r3,[r9,#0x0]
  000b9d72: strd r12,lr,[r8,#0x0]
  000b9d76: blx 0x0006eb24
  000b9d7a: mov r5,r0
  000b9d7c: movs r1,#0xb
  000b9d7e: movs r2,#0x0
  000b9d80: movs r3,#0x1e
  000b9d82: blx 0x00073834
  000b9d86: b 0x000b9c30
  000b9d88: movs r0,#0x78
  000b9d8a: str.w r3,[r9,#0x0]
  000b9d8e: strd r12,lr,[r8,#0x0]
  000b9d92: blx 0x0006eb24
  000b9d96: mov r5,r0
  000b9d98: movs r1,#0xb
  000b9d9a: movs r2,#0x0
  000b9d9c: movs r3,#0x4f
  000b9d9e: blx 0x00073834
  000b9da2: b 0x000b9c30
  000b9da4: ldr r0,[0x000ba11c]
  000b9da6: str.w r3,[r4,#0x1e8]
  000b9daa: add r0,pc
  000b9dac: strd r12,lr,[r8,#0x0]
  000b9db0: ldr.w r1,[r4,#0x190]
  000b9db4: mov.w r8,#0x0
  000b9db8: ldr r0,[r0,#0x0]
  000b9dba: str.w r1,[r4,#0x8c]
  000b9dbe: mov.w r1,#0xffffffff
  000b9dc2: str.w r8,[r4,#0x190]
  000b9dc6: ldr r0,[r0,#0x0]
  000b9dc8: ldr r0,[r0,#0x4]
  000b9dca: ldr r0,[r0,#0x24]
  000b9dcc: blx 0x000719d4
  000b9dd0: mov r1,r0
  000b9dd2: mov r0,r4
  000b9dd4: blx 0x00073870
  000b9dd8: ldr.w r0,[r4,#0x190]
  000b9ddc: movs r1,#0x1
  000b9dde: blx 0x000719ec
  000b9de2: ldr r0,[0x000ba120]
  000b9de4: add r0,pc
  000b9de6: ldr r6,[r0,#0x0]
  000b9de8: ldr r0,[r6,#0x0]
  000b9dea: ldr r0,[r0,#0x4]
  000b9dec: ldr.w r0,[r0,#0xe8]
  000b9df0: blx 0x000718a8
  000b9df4: mov r1,r0
  000b9df6: ldr r0,[0x000ba124]
  000b9df8: movs r2,#0x0
  000b9dfa: add r0,pc
  000b9dfc: ldr r5,[r0,#0x0]
  000b9dfe: ldr r0,[r5,#0x0]
  000b9e00: ldr.w r0,[r0,#0x190]
  000b9e04: blx 0x0007387c
  000b9e08: ldr r0,[r6,#0x0]
  000b9e0a: ldr r0,[r0,#0x4]
  000b9e0c: ldr.w r0,[r0,#0x14c]
  000b9e10: blx 0x000718a8
  000b9e14: mov r1,r0
  000b9e16: ldr r0,[r5,#0x0]
  000b9e18: movs r2,#0x1
  000b9e1a: ldr.w r0,[r0,#0x190]
  000b9e1e: blx 0x0007387c
  000b9e22: ldr r0,[r5,#0x0]
  000b9e24: str.w r8,[r0,#0x30]
  000b9e28: movs r0,#0x78
  000b9e2a: blx 0x0006eb24
  000b9e2e: mov r5,r0
  000b9e30: movs r1,#0xb
  000b9e32: movs r2,#0x0
  000b9e34: movs r3,#0x3a
  000b9e36: blx 0x00073834
  000b9e3a: b 0x000b9c30
  000b9e3c: movs r0,#0x78
  000b9e3e: str.w r3,[r9,#0x0]
  000b9e42: strd r12,lr,[r8,#0x0]
  000b9e46: blx 0x0006eb24
  000b9e4a: mov r5,r0
  000b9e4c: movs r1,#0xb
  000b9e4e: movs r2,#0x0
  000b9e50: movs r3,#0x62
  000b9e52: blx 0x00073834
  000b9e56: b 0x000b9c30
  000b9e58: str.w r3,[r4,#0x1e8]
  000b9e5c: strd r12,lr,[r8,#0x0]
  000b9e60: ldr.w r0,[r4,#0x190]
  000b9e64: blx 0x00073750
  000b9e68: mov r5,r0
  000b9e6a: cmp r0,#0x0
  000b9e6c: beq.w 0x000bb9dc
  000b9e70: movs r6,#0x0
  000b9e72: b 0x000b9e86
  000b9e74: ldr r0,[r5,#0x4]
  000b9e76: ldr.w r0,[r0,r6,lsl #0x2]
  000b9e7a: blx 0x000718d8
  000b9e7e: cmp r0,#0x83
  000b9e80: beq.w 0x000bb9d0
  000b9e84: adds r6,#0x1
  000b9e86: ldr r0,[r5,#0x0]
  000b9e88: cmp r6,r0
  000b9e8a: bcc 0x000b9e74
  000b9e8c: b.w 0x000bb9dc
  000b9e90: movs r0,#0x78
  000b9e92: str.w r3,[r9,#0x0]
  000b9e96: strd r12,lr,[r8,#0x0]
  000b9e9a: blx 0x0006eb24
  000b9e9e: mov r5,r0
  000b9ea0: movs r1,#0xb
  000b9ea2: movs r2,#0x0
  000b9ea4: movs r3,#0x62
  000b9ea6: blx 0x00073834
  000b9eaa: b 0x000b9c30
  000b9eac: movs r0,#0x78
  000b9eae: str.w r3,[r9,#0x0]
  000b9eb2: strd r12,lr,[r8,#0x0]
  000b9eb6: blx 0x0006eb24
  000b9eba: mov r5,r0
  000b9ebc: movs r1,#0x9c
  000b9ebe: movs r2,#0x0
  000b9ec0: movs r3,#0x37
  000b9ec2: blx 0x00073834
  000b9ec6: b 0x000b9c30
  000b9ec8: movs r0,#0x78
  000b9eca: str.w r3,[r9,#0x0]
  000b9ece: strd r12,lr,[r8,#0x0]
  000b9ed2: blx 0x0006eb24
  000b9ed6: mov r5,r0
  000b9ed8: movs r1,#0xb
  000b9eda: movs r2,#0x0
  000b9edc: movs r3,#0xa
  000b9ede: blx 0x00073834
  000b9ee2: b 0x000b9c30
  000b9ee4: str.w r3,[r9,#0x0]
  000b9ee8: strd r12,lr,[r8,#0x0]
  000b9eec: b.w 0x000bb96a
  000b9ef0: movs r0,#0x78
  000b9ef2: str.w r3,[r9,#0x0]
  000b9ef6: strd r12,lr,[r8,#0x0]
  000b9efa: blx 0x0006eb24
  000b9efe: mov r5,r0
  000b9f00: movs r1,#0x9c
  000b9f02: movs r2,#0x0
  000b9f04: movs r3,#0x3e
  000b9f06: blx 0x00073834
  000b9f0a: b 0x000b9c30
  000b9f0c: movs r0,#0x78
  000b9f0e: str.w r3,[r9,#0x0]
  000b9f12: strd r12,lr,[r8,#0x0]
  000b9f16: blx 0x0006eb24
  000b9f1a: mov r5,r0
  000b9f1c: movs r1,#0xb
  000b9f1e: movs r2,#0x0
  000b9f20: movs r3,#0x4e
  000b9f22: blx 0x00073834
  000b9f26: b 0x000b9c30
  000b9f28: movs r0,#0x78
  000b9f2a: str.w r3,[r9,#0x0]
  000b9f2e: strd r12,lr,[r8,#0x0]
  000b9f32: blx 0x0006eb24
  000b9f36: mov r5,r0
  000b9f38: movs r1,#0xb
  000b9f3a: movs r2,#0x0
  000b9f3c: movs r3,#0xa
  000b9f3e: blx 0x00073834
  000b9f42: b 0x000b9c30
  000b9f44: movs r0,#0x78
  000b9f46: str.w r3,[r9,#0x0]
  000b9f4a: strd r12,lr,[r8,#0x0]
  000b9f4e: blx 0x0006eb24
  000b9f52: mov r5,r0
  000b9f54: movs r1,#0xa1
  000b9f56: movs r2,#0x0
  000b9f58: mov.w r3,#0xffffffff
  000b9f5c: blx 0x00073834
  000b9f60: b 0x000b9c30
  000b9f62: movs r0,#0x78
  000b9f64: str.w r3,[r9,#0x0]
  000b9f68: strd r12,lr,[r8,#0x0]
  000b9f6c: blx 0x0006eb24
  000b9f70: mov r5,r0
  000b9f72: movs r1,#0xb
  000b9f74: movw r2,#0x7530
  000b9f78: movs r3,#0x62
  000b9f7a: blx 0x00073834
  000b9f7e: b 0x000b9c30
  000b9f80: movs r0,#0x78
  000b9f82: str.w r3,[r9,#0x0]
  000b9f86: strd r12,lr,[r8,#0x0]
  000b9f8a: blx 0x0006eb24
  000b9f8e: mov r5,r0
  000b9f90: movs r1,#0x8
  000b9f92: movs r2,#0x0
  000b9f94: movs r3,#0xa
  000b9f96: blx 0x00073834
  000b9f9a: mov r0,r4
  000b9f9c: mov r1,r5
  000b9f9e: blx 0x00073840
  000b9fa2: ldr.w r0,[r4,#0x198]
  000b9fa6: movs r1,#0xa4
  000b9fa8: movs r2,#0x32
  000b9faa: ldr r0,[r0,#0x4]
  000b9fac: ldr r0,[r0,#0x0]
  000b9fae: b.w 0x000bb84c
  000b9fb2: str.w r3,[r4,#0x1e8]
  000b9fb6: movs r1,#0xa4
  000b9fb8: strd r12,lr,[r8,#0x0]
  000b9fbc: movs r2,#0x32
  000b9fbe: ldr.w r0,[r4,#0x190]
  000b9fc2: blx 0x00072454
  000b9fc6: movs r5,#0x0
  000b9fc8: b 0x000b9fdc
  000b9fca: ldr r0,[r0,#0x4]
  000b9fcc: ldr.w r0,[r0,r5,lsl #0x2]
  000b9fd0: blx 0x000733fc
  000b9fd4: cmp r0,#0x55
  000b9fd6: beq.w 0x000bb7ac
  000b9fda: adds r5,#0x1
  000b9fdc: ldr r0,[r4,#0x18]
  000b9fde: ldr r1,[r0,#0x0]
  000b9fe0: cmp r5,r1
  000b9fe2: bcc 0x000b9fca
  000b9fe4: b.w 0x000bb7de
  000b9fe8: movs r0,#0x78
  000b9fea: str.w r3,[r9,#0x0]
  000b9fee: strd r12,lr,[r8,#0x0]
  000b9ff2: blx 0x0006eb24
  000b9ff6: mov r5,r0
  000b9ff8: movs r1,#0x9c
  000b9ffa: movs r2,#0x0
  000b9ffc: movs r3,#0x5b
  000b9ffe: blx 0x00073834
  000ba002: b 0x000b9c30
  000ba004: movs r0,#0x78
  000ba006: str.w r3,[r9,#0x0]
  000ba00a: strd r12,lr,[r8,#0x0]
  000ba00e: blx 0x0006eb24
  000ba012: mov r5,r0
  000ba014: movs r1,#0x9a
  000ba016: movs r2,#0x0
  000ba018: movs r3,#0x4e
  000ba01a: blx 0x00073834
  000ba01e: mov r0,r4
  000ba020: mov r1,r5
  000ba022: blx 0x00073840
  000ba026: b.w 0x000baaac
  000ba02a: movs r0,#0x78
  000ba02c: str.w r3,[r9,#0x0]
  000ba030: strd r12,lr,[r8,#0x0]
  000ba034: blx 0x0006eb24
  000ba038: mov r5,r0
  000ba03a: movs r1,#0x4
  000ba03c: movs r2,#0x0
  000ba03e: movs r3,#0x37
  000ba040: blx 0x00073834
  000ba044: b 0x000b9c30
  000ba046: movs r0,#0x9
  000ba048: movs r1,#0x30
  000ba04a: str.w r3,[r4,#0x1e8]
  000ba04e: strd r12,lr,[r8,#0x0]
  000ba052: strd r0,r1,[r4,#0x7c]
  000ba056: movs r0,#0x78
  000ba058: blx 0x0006eb24
  000ba05c: mov r5,r0
  000ba05e: movs r1,#0x4
  000ba060: movs r2,#0x0
  000ba062: movs r3,#0x30
  000ba064: blx 0x00073834
  000ba068: b 0x000b9c30
  000ba06a: movs r0,#0x78
  000ba06c: str.w r3,[r9,#0x0]
  000ba070: strd r12,lr,[r8,#0x0]
  000ba074: blx 0x0006eb24
  000ba078: mov r5,r0
  000ba07a: movs r1,#0xb
  000ba07c: movs r2,#0x0
  000ba07e: movs r3,#0x1d
  000ba080: blx 0x00073834
  000ba084: b 0x000b9c30
  000ba086: movs r0,#0x78
  000ba088: str.w r3,[r9,#0x0]
  000ba08c: strd r12,lr,[r8,#0x0]
  000ba090: blx 0x0006eb24
  000ba094: mov r5,r0
  000ba096: movs r1,#0xa0
  000ba098: movs r2,#0x0
  000ba09a: movs r3,#0x19
  000ba09c: blx 0x00073834
  000ba0a0: b.w 0x000bb51a
  000ba0a4: movs r0,#0x78
  000ba0a6: str.w r3,[r9,#0x0]
  000ba0aa: strd r12,lr,[r8,#0x0]
  000ba0ae: blx 0x0006eb24
  000ba0b2: mov r5,r0
  000ba0b4: ldr r2,[0x000ba128]
  000ba0b6: movs r1,#0xb
  000ba0b8: movs r3,#0x4a
  000ba0ba: blx 0x00073834
  000ba0be: b 0x000b9c30
  000ba0c0: ldr r0,[0x000ba12c]
  000ba0c2: str.w r3,[r9,#0x0]
  000ba0c6: add r0,pc
  000ba0c8: strd r12,lr,[r8,#0x0]
  000ba0cc: ldr r6,[r0,#0x0]
  000ba0ce: ldr r1,[r6,#0x0]
  000ba0d0: ldr.w r0,[r1,#0x90]!
  000ba0d4: cbz r0,0x000ba0e4
  000ba0d6: blx 0x00070204
  000ba0da: blx 0x0006eb48
  000ba0de: ldr r0,[r6,#0x0]
  000ba0e0: add.w r1,r0,#0x90
  000ba0e4: movs r0,#0x0
  000ba0e6: str r0,[r1,#0x0]
  000ba0e8: movs r0,#0xc
  000ba0ea: blx 0x0006eb24
  000ba0ee: mov r5,r0
  000ba0f0: blx 0x000701f8
  000ba0f4: ldr r0,[r6,#0x0]
  000ba0f6: mov r1,r5
  000ba0f8: str.w r5,[r0,#0x90]
  000ba0fc: movs r0,#0x3
  000ba0fe: blx 0x00071a4c
  000ba102: ldr r0,[r6,#0x0]
  000ba104: movs r5,#0x0
  000ba106: movs r1,#0x2d
  000ba108: movs r2,#0x38
  000ba10a: movs r3,#0x16
  000ba10c: ldr.w r0,[r0,#0x90]
  000ba110: ldr r0,[r0,#0x4]
  000ba112: strd r2,r1,[r0,#0x0]
  000ba116: str r3,[r0,#0x8]
  000ba118: b 0x000ba14c
  000ba130: ldr r0,[r0,#0x4]
  000ba132: ldr.w r0,[r0,r5,lsl #0x2]
  000ba136: blx 0x000733fc
  000ba13a: cmp r0,#0xb3
  000ba13c: bne 0x000ba14a
  000ba13e: ldr r0,[r4,#0x18]
  000ba140: ldr r0,[r0,#0x4]
  000ba142: ldr.w r0,[r0,r5,lsl #0x2]
  000ba146: blx 0x00073888
  000ba14a: adds r5,#0x1
  000ba14c: ldr r0,[r4,#0x18]
  000ba14e: ldr r1,[r0,#0x0]
  000ba150: cmp r5,r1
  000ba152: bcc 0x000ba130
  000ba154: movs r0,#0x78
  000ba156: blx 0x0006eb24
  000ba15a: mov r5,r0
  000ba15c: movs r1,#0xa3
  000ba15e: movs r2,#0x0
  000ba160: movs r3,#0x65
  000ba162: mov.w r8,#0x0
  000ba166: blx 0x00073834
  000ba16a: mov r0,r4
  000ba16c: mov r1,r5
  000ba16e: blx 0x00073840
  000ba172: ldr.w r0,[r4,#0x198]
  000ba176: movs r1,#0x0
  000ba178: ldr r0,[r0,#0x4]
  000ba17a: ldr r0,[r0,#0x0]
  000ba17c: blx 0x00072898
  000ba180: ldr r0,[r6,#0x0]
  000ba182: str.w r8,[r0,#0x10c]
  000ba186: b.w 0x000bb96a
  000ba18a: str.w r3,[r4,#0x1e8]
  000ba18e: strd r12,lr,[r8,#0x0]
  000ba192: ldr.w r0,[r4,#0x198]
  000ba196: ldr r0,[r0,#0x4]
  000ba198: ldr r0,[r0,#0x0]
  000ba19a: blx 0x0007288c
  000ba19e: mov r5,r0
  000ba1a0: movs r0,#0x78
  000ba1a2: blx 0x0006eb24
  000ba1a6: mov r6,r0
  000ba1a8: movw r0,#0xc350
  000ba1ac: mla r2,r5,r0,r0
  000ba1b0: mov r0,r6
  000ba1b2: movs r1,#0xb
  000ba1b4: movs r3,#0x65
  000ba1b6: blx 0x00073834
  000ba1ba: mov r0,r4
  000ba1bc: mov r1,r6
  000ba1be: blx 0x00073840
  000ba1c2: ldr.w r0,[r4,#0x198]
  000ba1c6: movs r1,#0x0
  000ba1c8: cmp r5,#0x0
  000ba1ca: ldr r0,[r0,#0x4]
  000ba1cc: ldr r0,[r0,#0x0]
  000ba1ce: it gt
  000ba1d0: mov.gt r1,#0x1
  000ba1d2: b.w 0x000bba24
  000ba1d6: movs r0,#0x78
  000ba1d8: str.w r3,[r9,#0x0]
  000ba1dc: strd r12,lr,[r8,#0x0]
  000ba1e0: blx 0x0006eb24
  000ba1e4: mov r5,r0
  000ba1e6: movs r1,#0xb
  000ba1e8: movs r2,#0x0
  000ba1ea: movs r3,#0x64
  000ba1ec: blx 0x00073834
  000ba1f0: b 0x000b9c30
  000ba1f2: ldr r0,[0x000ba5d0]
  000ba1f4: movs r1,#0x12
  000ba1f6: str.w r3,[r9,#0x0]
  000ba1fa: add r0,pc
  000ba1fc: strd r12,lr,[r8,#0x0]
  000ba200: ldr r5,[r0,#0x0]
  000ba202: ldr r0,[r5,#0x0]
  000ba204: ldr.w r0,[r0,#0x190]
  000ba208: blx 0x0007228c
  000ba20c: mov r1,r0
  000ba20e: ldr r0,[r5,#0x0]
  000ba210: cmp r1,#0x0
  000ba212: ldr.w r0,[r0,#0x190]
  000ba216: beq.w 0x000bb970
  000ba21a: blx 0x00072c34
  000ba21e: b.w 0x000bb984
  000ba222: movs r0,#0x78
  000ba224: str.w r3,[r9,#0x0]
  000ba228: strd r12,lr,[r8,#0x0]
  000ba22c: blx 0x0006eb24
  000ba230: mov r5,r0
  000ba232: movs r1,#0x4
  000ba234: movs r2,#0x0
  000ba236: mov.w r3,#0xffffffff
  000ba23a: blx 0x00073834
  000ba23e: b 0x000b9c30
  000ba240: movs r0,#0x78
  000ba242: str.w r3,[r9,#0x0]
  000ba246: strd r12,lr,[r8,#0x0]
  000ba24a: blx 0x0006eb24
  000ba24e: mov r5,r0
  000ba250: movs r1,#0xb
  000ba252: movs r2,#0x0
  000ba254: movs r3,#0xa
  000ba256: blx 0x00073834
  000ba25a: b 0x000b9c30
  000ba25c: movs r0,#0x78
  000ba25e: str.w r3,[r9,#0x0]
  000ba262: strd r12,lr,[r8,#0x0]
  000ba266: blx 0x0006eb24
  000ba26a: mov r5,r0
  000ba26c: movs r1,#0x4
  000ba26e: movs r2,#0x0
  000ba270: movs r3,#0x62
  000ba272: blx 0x00073834
  000ba276: b 0x000b9c30
  000ba278: movs r0,#0x78
  000ba27a: str.w r3,[r9,#0x0]
  000ba27e: strd r12,lr,[r8,#0x0]
  000ba282: blx 0x0006eb24
  000ba286: mov r5,r0
  000ba288: movs r1,#0xab
  000ba28a: movs r2,#0x0
  000ba28c: movs r3,#0xa
  000ba28e: blx 0x00073834
  000ba292: b 0x000b9c30
  000ba294: movs r0,#0x78
  000ba296: str.w r3,[r9,#0x0]
  000ba29a: strd r12,lr,[r8,#0x0]
  000ba29e: blx 0x0006eb24
  000ba2a2: mov r5,r0
  000ba2a4: movs r1,#0x4
  000ba2a6: movs r2,#0x0
  000ba2a8: movs r3,#0x53
  000ba2aa: blx 0x00073834
  000ba2ae: b 0x000b9c30
  000ba2b0: movs r0,#0x78
  000ba2b2: str.w r3,[r9,#0x0]
  000ba2b6: strd r12,lr,[r8,#0x0]
  000ba2ba: blx 0x0006eb24
  000ba2be: mov r5,r0
  000ba2c0: movs r1,#0x4
  000ba2c2: movs r2,#0x0
  000ba2c4: movs r3,#0x6f
  000ba2c6: blx 0x00073834
  000ba2ca: b 0x000b9c30
  000ba2cc: movs r0,#0x78
  000ba2ce: str.w r3,[r9,#0x0]
  000ba2d2: strd r12,lr,[r8,#0x0]
  000ba2d6: blx 0x0006eb24
  000ba2da: mov r5,r0
  000ba2dc: movs r1,#0xaa
  000ba2de: movs r2,#0x0
  000ba2e0: movs r3,#0xa
  000ba2e2: blx 0x00073834
  000ba2e6: b 0x000b9c30
  000ba2e8: movs r0,#0x78
  000ba2ea: str.w r3,[r9,#0x0]
  000ba2ee: strd r12,lr,[r8,#0x0]
  000ba2f2: blx 0x0006eb24
  000ba2f6: mov r5,r0
  000ba2f8: movs r1,#0xb
  000ba2fa: movs r2,#0x0
  000ba2fc: movs r3,#0x4e
  000ba2fe: blx 0x00073834
  000ba302: mov r0,r4
  000ba304: mov r1,r5
  000ba306: blx 0x00073840
  000ba30a: ldr.w r0,[r4,#0x198]
  000ba30e: ldr r0,[r0,#0x4]
  000ba310: ldr r0,[r0,#0x0]
  000ba312: str.w r0,[r4,#0x194]
  000ba316: b.w 0x000bb96a
  000ba31a: movs r0,#0x78
  000ba31c: str.w r3,[r9,#0x0]
  000ba320: strd r12,lr,[r8,#0x0]
  000ba324: blx 0x0006eb24
  000ba328: mov r5,r0
  000ba32a: movs r1,#0xb
  000ba32c: movs r2,#0x0
  000ba32e: movs r3,#0x4e
  000ba330: blx 0x00073834
  000ba334: b 0x000b9c30
  000ba336: movs r0,#0x78
  000ba338: str.w r3,[r9,#0x0]
  000ba33c: strd r12,lr,[r8,#0x0]
  000ba340: blx 0x0006eb24
  000ba344: mov r5,r0
  000ba346: movs r1,#0x96
  000ba348: movs r2,#0x0
  000ba34a: movs r3,#0x0
  000ba34c: blx 0x00073834
  000ba350: mov r0,r4
  000ba352: mov r1,r5
  000ba354: blx 0x00073840
  000ba358: ldr.w r0,[r4,#0x198]
  000ba35c: ldr.w r1,[r4,#0x1c4]
  000ba360: ldr r0,[r0,#0x4]
  000ba362: adds r1,#0x1
  000ba364: ldr r0,[r0,#0x0]
  000ba366: blx 0x00072898
  000ba36a: b.w 0x000bb522
  000ba36e: movs r0,#0x78
  000ba370: str.w r3,[r9,#0x0]
  000ba374: strd r12,lr,[r8,#0x0]
  000ba378: blx 0x0006eb24
  000ba37c: mov r5,r0
  000ba37e: movs r1,#0x4
  000ba380: movs r2,#0x0
  000ba382: mov.w r3,#0xffffffff
  000ba386: blx 0x00073834
  000ba38a: b 0x000b9c30
  000ba38c: str.w r3,[r4,#0x1e8]
  000ba390: movs r1,#0x1
  000ba392: strd r12,lr,[r8,#0x0]
  000ba396: ldr r0,[r4,#0x38]
  000ba398: ldr r0,[r0,#0x4]
  000ba39a: strb r1,[r0,#0x17]
  000ba39c: movs r0,#0x78
  000ba39e: blx 0x0006eb24
  000ba3a2: mov r5,r0
  000ba3a4: movs r1,#0xb
  000ba3a6: movs r2,#0x0
  000ba3a8: movs r3,#0x65
  000ba3aa: movs r6,#0x0
  000ba3ac: blx 0x00073834
  000ba3b0: mov r0,r4
  000ba3b2: mov r1,r5
  000ba3b4: blx 0x00073840
  000ba3b8: ldr.w r1,[r4,#0x8c]
  000ba3bc: mov r0,r4
  000ba3be: blx 0x00073870
  000ba3c2: str.w r6,[r4,#0x8c]
  000ba3c6: b.w 0x000bb96a
  000ba3ca: ldr r0,[0x000ba5d4]
  000ba3cc: str.w r3,[r4,#0x1e8]
  000ba3d0: add r0,pc
  000ba3d2: strd r12,lr,[r8,#0x0]
  000ba3d6: ldr.w r1,[r4,#0x190]
  000ba3da: ldr r0,[r0,#0x0]
  000ba3dc: str.w r1,[r4,#0x8c]
  000ba3e0: movs r1,#0x0
  000ba3e2: str.w r1,[r4,#0x190]
  000ba3e6: mov.w r1,#0xffffffff
  000ba3ea: ldr r0,[r0,#0x0]
  000ba3ec: ldr r0,[r0,#0x4]
  000ba3ee: ldr.w r0,[r0,#0x9c]
  000ba3f2: blx 0x000719d4
  000ba3f6: mov r1,r0
  000ba3f8: mov r0,r4
  000ba3fa: blx 0x00073870
  000ba3fe: ldr.w r0,[r4,#0x190]
  000ba402: movs r1,#0x1
  000ba404: blx 0x000719ec
  000ba408: ldr r0,[0x000ba5d8]
  000ba40a: add r0,pc
  000ba40c: ldr.w r8,[r0,#0x0]
  000ba410: ldr.w r0,[r8,#0x0]
  000ba414: ldr r1,[0x000ba5dc]
  000ba416: ldr r0,[r0,#0x4]
  000ba418: add r1,pc
  000ba41a: ldr.w r9,[r1,#0x0]
  000ba41e: ldr.w r0,[r0,#0x2d4]
  000ba422: blx r9
  000ba424: mov r1,r0
  000ba426: ldr r0,[0x000ba5e0]
  000ba428: add r0,pc
  000ba42a: ldr r6,[r0,#0x0]
  000ba42c: ldr r0,[r6,#0x0]
  000ba42e: ldr.w r0,[r0,#0x190]
  000ba432: ldr r2,[0x000ba5e4]
  000ba434: add r2,pc
  000ba436: ldr r5,[r2,#0x0]
  000ba438: movs r2,#0x0
  000ba43a: blx r5
  000ba43c: ldr.w r0,[r8,#0x0]
  000ba440: ldr r0,[r0,#0x4]
  000ba442: ldr.w r0,[r0,#0xd0]
  000ba446: blx r9
  000ba448: mov r1,r0
  000ba44a: ldr r0,[r6,#0x0]
  000ba44c: movs r2,#0x0
  000ba44e: ldr.w r0,[r0,#0x190]
  000ba452: blx r5
  000ba454: ldr.w r0,[r8,#0x0]
  000ba458: ldr r0,[r0,#0x4]
  000ba45a: ldr.w r0,[r0,#0xe8]
  000ba45e: blx r9
  000ba460: mov r1,r0
  000ba462: ldr r0,[r6,#0x0]
  000ba464: movs r2,#0x1
  000ba466: ldr.w r0,[r0,#0x190]
  000ba46a: blx r5
  000ba46c: ldr.w r0,[r8,#0x0]
  000ba470: ldr r0,[r0,#0x4]
  000ba472: ldr.w r0,[r0,#0x14c]
  000ba476: blx r9
  000ba478: mov r1,r0
  000ba47a: ldr r0,[r6,#0x0]
  000ba47c: movs r2,#0x2
  000ba47e: ldr.w r0,[r0,#0x190]
  000ba482: blx r5
  000ba484: movs r0,#0x78
  000ba486: blx 0x0006eb24
  000ba48a: mov r5,r0
  000ba48c: movs r1,#0x4
  000ba48e: movs r2,#0x0
  000ba490: movs r3,#0x66
  000ba492: blx 0x00073834
  000ba496: b.w 0x000b9c30
  000ba49a: movs r0,#0x78
  000ba49c: str.w r3,[r9,#0x0]
  000ba4a0: strd r12,lr,[r8,#0x0]
  000ba4a4: blx 0x0006eb24
  000ba4a8: mov r5,r0
  000ba4aa: ldr r2,[0x000ba5e8]
  000ba4ac: movs r1,#0xb
  000ba4ae: movs r3,#0x65
  000ba4b0: blx 0x00073834
  000ba4b4: b.w 0x000b9c30
  000ba4b8: movs r0,#0x78
  000ba4ba: str.w r3,[r9,#0x0]
  000ba4be: strd r12,lr,[r8,#0x0]
  000ba4c2: blx 0x0006eb24
  000ba4c6: mov r5,r0
  000ba4c8: movs r1,#0xa6
  000ba4ca: movs r2,#0x0
  000ba4cc: movs r3,#0x65
  000ba4ce: blx 0x00073834
  000ba4d2: mov r0,r4
  000ba4d4: mov r1,r5
  000ba4d6: blx 0x00073840
  000ba4da: ldr.w r0,[r4,#0x198]
  000ba4de: movs r1,#0xb3
  000ba4e0: movs r2,#0xa
  000ba4e2: ldr r0,[r0,#0x4]
  000ba4e4: ldr r0,[r0,#0x0]
  000ba4e6: blx 0x00071ca4
  000ba4ea: movs r5,#0x0
  000ba4ec: b 0x000ba500
  000ba4ee: ldr r0,[r0,#0x4]
  000ba4f0: ldr.w r0,[r0,r5,lsl #0x2]
  000ba4f4: blx 0x000733fc
  000ba4f8: cmp r0,#0xb3
  000ba4fa: beq.w 0x000bb930
  000ba4fe: adds r5,#0x1
  000ba500: ldr r0,[r4,#0x18]
  000ba502: ldr r1,[r0,#0x0]
  000ba504: cmp r5,r1
  000ba506: bcc 0x000ba4ee
  000ba508: b.w 0x000bb95a
  000ba50c: movs r0,#0x78
  000ba50e: str.w r3,[r9,#0x0]
  000ba512: strd r12,lr,[r8,#0x0]
  000ba516: blx 0x0006eb24
  000ba51a: mov r5,r0
  000ba51c: movs r1,#0xa4
  000ba51e: movs r2,#0x0
  000ba520: movs r3,#0x65
  000ba522: blx 0x00073834
  000ba526: b.w 0x000b9c30
  000ba52a: str.w r3,[r4,#0x1e8]
  000ba52e: movs r1,#0x1
  000ba530: strd r12,lr,[r8,#0x0]
  000ba534: ldr r0,[r4,#0x38]
  000ba536: ldr r0,[r0,#0x4]
  000ba538: strb r1,[r0,#0x16]
  000ba53a: movs r0,#0x78
  000ba53c: blx 0x0006eb24
  000ba540: mov r5,r0
  000ba542: movs r1,#0xb
  000ba544: movs r2,#0x0
  000ba546: movs r3,#0x64
  000ba548: blx 0x00073834
  000ba54c: b.w 0x000b9c30
  000ba550: str.w r3,[r4,#0x1e8]
  000ba554: movs r1,#0x1
  000ba556: strd r12,lr,[r8,#0x0]
  000ba55a: ldr r0,[r4,#0x38]
  000ba55c: ldr r0,[r0,#0x4]
  000ba55e: strb r1,[r0,#0x18]
  000ba560: movs r0,#0x78
  000ba562: blx 0x0006eb24
  000ba566: mov r5,r0
  000ba568: movs r1,#0x4
  000ba56a: movs r2,#0x0
  000ba56c: movs r3,#0x67
  000ba56e: blx 0x00073834
  000ba572: b.w 0x000b9c30
  000ba576: movs r0,#0x78
  000ba578: str.w r3,[r9,#0x0]
  000ba57c: strd r12,lr,[r8,#0x0]
  000ba580: blx 0x0006eb24
  000ba584: mov r5,r0
  000ba586: movs r1,#0x4
  000ba588: movs r2,#0x0
  000ba58a: movs r3,#0x68
  000ba58c: blx 0x00073834
  000ba590: b.w 0x000b9c30
  000ba594: movs r0,#0x78
  000ba596: str.w r3,[r9,#0x0]
  000ba59a: strd r12,lr,[r8,#0x0]
  000ba59e: blx 0x0006eb24
  000ba5a2: mov r5,r0
  000ba5a4: movs r1,#0xb
  000ba5a6: movs r2,#0x0
  000ba5a8: movs r3,#0x64
  000ba5aa: blx 0x00073834
  000ba5ae: b.w 0x000b9c30
  000ba5b2: movs r0,#0x78
  000ba5b4: str.w r3,[r9,#0x0]
  000ba5b8: strd r12,lr,[r8,#0x0]
  000ba5bc: blx 0x0006eb24
  000ba5c0: mov r5,r0
  000ba5c2: movs r1,#0xb
  000ba5c4: movs r2,#0x0
  000ba5c6: movs r3,#0x65
  000ba5c8: blx 0x00073834
  000ba5cc: b.w 0x000b9c30
  000ba5ec: movs r0,#0x78
  000ba5ee: str.w r3,[r9,#0x0]
  000ba5f2: strd r12,lr,[r8,#0x0]
  000ba5f6: blx 0x0006eb24
  000ba5fa: mov r5,r0
  000ba5fc: movs r1,#0x4
  000ba5fe: movs r2,#0x0
  000ba600: movs r3,#0x68
  000ba602: blx 0x00073834
  000ba606: mov r0,r4
  000ba608: mov r1,r5
  000ba60a: blx 0x00073840
  000ba60e: ldr.w r0,[r4,#0x198]
  000ba612: movs r1,#0x0
  000ba614: ldr r0,[r0,#0x4]
  000ba616: ldr r0,[r0,#0x0]
  000ba618: blx 0x00072898
  000ba61c: ldr r0,[0x000ba9fc]
  000ba61e: ldr r1,[0x000baa00]
  000ba620: add r0,pc
  000ba622: add r1,pc
  000ba624: ldr r6,[r0,#0x0]
  000ba626: ldr r4,[r1,#0x0]
  000ba628: ldr r0,[r6,#0x0]
  000ba62a: ldr r1,[r4,#0x0]
  000ba62c: ldr r2,[0x000baa04]
  000ba62e: ldr r0,[r0,#0x4]
  000ba630: add r2,pc
  000ba632: ldr.w r8,[r1,#0x19c]
  000ba636: movs r1,#0x5
  000ba638: ldr.w r9,[r2,#0x0]
  000ba63c: movs r2,#0x0
  000ba63e: ldr.w r0,[r0,#0xf0]
  000ba642: blx r9
  000ba644: mov r1,r0
  000ba646: ldr r0,[0x000baa08]
  000ba648: add r0,pc
  000ba64a: ldr r5,[r0,#0x0]
  000ba64c: mov r0,r8
  000ba64e: blx r5
  000ba650: ldr r0,[r4,#0x0]
  000ba652: movs r1,#0x5
  000ba654: movs r2,#0x0
  000ba656: ldr.w r8,[r0,#0x19c]
  000ba65a: ldr r0,[r6,#0x0]
  000ba65c: ldr r0,[r0,#0x4]
  000ba65e: ldr.w r0,[r0,#0xf4]
  000ba662: blx r9
  000ba664: mov r1,r0
  000ba666: mov r0,r8
  000ba668: blx r5
  000ba66a: ldr r0,[r6,#0x0]
  000ba66c: movs r2,#0x0
  000ba66e: ldr r1,[r4,#0x0]
  000ba670: ldr r0,[r0,#0x4]
  000ba672: ldr.w r4,[r1,#0x19c]
  000ba676: movs r1,#0x5
  000ba678: ldr.w r0,[r0,#0xf8]
  000ba67c: blx r9
  000ba67e: mov r1,r0
  000ba680: mov r0,r4
  000ba682: mov r2,r5
  000ba684: pop.w {r8,r9,r10}
  000ba688: pop.w {r4,r5,r6,r7,lr}
  000ba68c: bx r2
  000ba68e: movs r0,#0x78
  000ba690: str.w r3,[r9,#0x0]
  000ba694: strd r12,lr,[r8,#0x0]
  000ba698: blx 0x0006eb24
  000ba69c: mov r5,r0
  000ba69e: movs r1,#0x8
  000ba6a0: movs r2,#0x0
  000ba6a2: movs r3,#0x42
  000ba6a4: blx 0x00073834
  000ba6a8: mov r0,r4
  000ba6aa: mov r1,r5
  000ba6ac: blx 0x00073840
  000ba6b0: ldr.w r0,[r4,#0x198]
  000ba6b4: movs r1,#0xaf
  000ba6b6: b.w 0x000bb846
  000ba6ba: ldr r0,[0x000baa0c]
  000ba6bc: movs r1,#0xaf
  000ba6be: str.w r3,[r9,#0x0]
  000ba6c2: add r0,pc
  000ba6c4: strd r12,lr,[r8,#0x0]
  000ba6c8: ldr r0,[r0,#0x0]
  000ba6ca: ldr r0,[r0,#0x0]
  000ba6cc: ldr.w r0,[r0,#0x190]
  000ba6d0: blx 0x00073894
  000ba6d4: movs r0,#0x78
  000ba6d6: blx 0x0006eb24
  000ba6da: mov r5,r0
  000ba6dc: movs r1,#0x6
  000ba6de: movs r2,#0x0
  000ba6e0: movs r3,#0x42
  000ba6e2: blx 0x00073834
  000ba6e6: b.w 0x000b9c30
  000ba6ea: movs r0,#0x78
  000ba6ec: str.w r3,[r9,#0x0]
  000ba6f0: strd r12,lr,[r8,#0x0]
  000ba6f4: blx 0x0006eb24
  000ba6f8: mov r5,r0
  000ba6fa: movs r1,#0x6
  000ba6fc: movs r2,#0x0
  000ba6fe: movs r3,#0x41
  000ba700: blx 0x00073834
  000ba704: b.w 0x000b9c30
  000ba708: movs r0,#0x78
  000ba70a: str.w r3,[r9,#0x0]
  000ba70e: strd r12,lr,[r8,#0x0]
  000ba712: blx 0x0006eb24
  000ba716: mov r5,r0
  000ba718: movs r1,#0xb
  000ba71a: movs r2,#0x0
  000ba71c: movs r3,#0x42
  000ba71e: blx 0x00073834
  000ba722: b.w 0x000b9c30
  000ba726: movs r5,#0x0
  000ba728: str.w r3,[r4,#0x1e8]
  000ba72c: strd r12,lr,[r8,#0x0]
  000ba730: b 0x000ba74e
  000ba732: ldr r0,[r0,#0x4]
  000ba734: ldr.w r0,[r0,r5,lsl #0x2]
  000ba738: blx 0x000733fc
  000ba73c: cmp r0,#0xb7
  000ba73e: bne 0x000ba74c
  000ba740: ldr r0,[r4,#0x18]
  000ba742: ldr r0,[r0,#0x4]
  000ba744: ldr.w r0,[r0,r5,lsl #0x2]
  000ba748: blx 0x000738a0
  000ba74c: adds r5,#0x1
  000ba74e: ldr r0,[r4,#0x18]
  000ba750: ldr r1,[r0,#0x0]
  000ba752: cmp r5,r1
  000ba754: bcc 0x000ba732
  000ba756: ldr r0,[0x000baa10]
  000ba758: ldr r1,[0x000baa14]
  000ba75a: add r0,pc
  000ba75c: add r1,pc
  000ba75e: ldr r0,[r0,#0x0]
  000ba760: ldr r1,[r1,#0x0]
  000ba762: ldr r0,[r0,#0x0]
  000ba764: ldr r1,[r1,#0x0]
  000ba766: ldr r0,[r0,#0x4]
  000ba768: ldr.w r5,[r1,#0x190]
  000ba76c: movs r1,#0x1
  000ba76e: ldr.w r0,[r0,#0x2bc]
  000ba772: blx 0x000718c0
  000ba776: mov r1,r0
  000ba778: mov r0,r5
  000ba77a: blx 0x00072cdc
  000ba77e: movs r0,#0x78
  000ba780: blx 0x0006eb24
  000ba784: mov r5,r0
  000ba786: ldr r2,[0x000baa18]
  000ba788: movs r1,#0xa4
  000ba78a: movs r3,#0x65
  000ba78c: blx 0x00073834
  000ba790: mov r0,r4
  000ba792: mov r1,r5
  000ba794: blx 0x00073840
  000ba798: movs r0,#0x0
  000ba79a: str.w r0,[r4,#0x10c]
  000ba79e: b.w 0x000bb96a
  000ba7a2: movs r0,#0x78
  000ba7a4: str.w r3,[r9,#0x0]
  000ba7a8: strd r12,lr,[r8,#0x0]
  000ba7ac: blx 0x0006eb24
  000ba7b0: mov r5,r0
  000ba7b2: movs r1,#0xa
  000ba7b4: movs r2,#0x0
  000ba7b6: movs r3,#0x51
  000ba7b8: blx 0x00073834
  000ba7bc: b.w 0x000b9c30
  000ba7c0: ldr r0,[0x000baa1c]
  000ba7c2: str.w r3,[r9,#0x0]
  000ba7c6: add r0,pc
  000ba7c8: strd r12,lr,[r8,#0x0]
  000ba7cc: ldr r0,[r0,#0x0]
  000ba7ce: ldr r0,[r0,#0x0]
  000ba7d0: ldr.w r0,[r0,#0x19c]
  000ba7d4: blx 0x000738ac
  000ba7d8: movs r0,#0x78
  000ba7da: blx 0x0006eb24
  000ba7de: mov r5,r0
  000ba7e0: movs r1,#0xb
  000ba7e2: movs r2,#0x0
  000ba7e4: movs r3,#0x64
  000ba7e6: blx 0x00073834
  000ba7ea: b.w 0x000b9c30
  000ba7ee: movs r0,#0x78
  000ba7f0: str.w r3,[r9,#0x0]
  000ba7f4: strd r12,lr,[r8,#0x0]
  000ba7f8: blx 0x0006eb24
  000ba7fc: mov r5,r0
  000ba7fe: movs r1,#0xb
  000ba800: movs r2,#0x0
  000ba802: movs r3,#0x64
  000ba804: blx 0x00073834
  000ba808: b.w 0x000b9c30
  000ba80c: ldr r0,[0x000baa20]
  000ba80e: movs r1,#0x12
  000ba810: str.w r3,[r9,#0x0]
  000ba814: add r0,pc
  000ba816: strd r12,lr,[r8,#0x0]
  000ba81a: ldr r0,[r0,#0x0]
  000ba81c: ldr r0,[r0,#0x0]
  000ba81e: ldr.w r0,[r0,#0x190]
  000ba822: blx 0x0007228c
  000ba826: cbz r0,0x000ba82e
  000ba828: movs r1,#0x1
  000ba82a: blx 0x00073330
  000ba82e: movs r0,#0x78
  000ba830: blx 0x0006eb24
  000ba834: mov r5,r0
  000ba836: movs r1,#0xb
  000ba838: movs r2,#0x0
  000ba83a: movs r3,#0x65
  000ba83c: blx 0x00073834
  000ba840: b.w 0x000b9c30
  000ba844: movs r0,#0x78
  000ba846: str.w r3,[r9,#0x0]
  000ba84a: strd r12,lr,[r8,#0x0]
  000ba84e: blx 0x0006eb24
  000ba852: mov r5,r0
  000ba854: movs r1,#0xa5
  000ba856: movs r2,#0x0
  000ba858: mov.w r3,#0xffffffff
  000ba85c: blx 0x00073834
  000ba860: b.w 0x000b9c30
  000ba864: movs r0,#0x78
  000ba866: str.w r3,[r9,#0x0]
  000ba86a: strd r12,lr,[r8,#0x0]
  000ba86e: blx 0x0006eb24
  000ba872: mov r5,r0
  000ba874: movs r1,#0x1
  000ba876: movs r2,#0x0
  000ba878: movs r3,#0x64
  000ba87a: blx 0x00073834
  000ba87e: b.w 0x000b9c30
  000ba882: movs r0,#0x78
  000ba884: str.w r3,[r9,#0x0]
  000ba888: strd r12,lr,[r8,#0x0]
  000ba88c: blx 0x0006eb24
  000ba890: mov r5,r0
  000ba892: movs r1,#0xb
  000ba894: movs r2,#0x0
  000ba896: movs r3,#0x64
  000ba898: blx 0x00073834
  000ba89c: b.w 0x000b9c30
  000ba8a0: movs r0,#0x78
  000ba8a2: str.w r3,[r9,#0x0]
  000ba8a6: strd r12,lr,[r8,#0x0]
  000ba8aa: blx 0x0006eb24
  000ba8ae: mov r5,r0
  000ba8b0: movs r1,#0xb
  000ba8b2: movs r2,#0x0
  000ba8b4: movs r3,#0x64
  000ba8b6: blx 0x00073834
  000ba8ba: b.w 0x000b9c30
  000ba8be: ldr r0,[0x000baa24]
  000ba8c0: movs r1,#0x12
  000ba8c2: str.w r3,[r9,#0x0]
  000ba8c6: add r0,pc
  000ba8c8: strd r12,lr,[r8,#0x0]
  000ba8cc: ldr r5,[r0,#0x0]
  000ba8ce: ldr r0,[r5,#0x0]
  000ba8d0: ldr.w r0,[r0,#0x190]
  000ba8d4: blx 0x0007228c
  000ba8d8: cbnz r0,0x000ba8e8
  000ba8da: ldr r0,[r5,#0x0]
  000ba8dc: movs r1,#0x55
  000ba8de: ldr.w r0,[r0,#0x190]
  000ba8e2: blx 0x00072424
  000ba8e6: cbz r0,0x000ba8ee
  000ba8e8: movs r1,#0x0
  000ba8ea: blx 0x00073330
  000ba8ee: ldr r0,[0x000baa28]
  000ba8f0: ldr r1,[r5,#0x0]
  000ba8f2: add r0,pc
  000ba8f4: ldr r0,[r0,#0x0]
  000ba8f6: ldr.w r5,[r1,#0x190]
  000ba8fa: movs r1,#0x1
  000ba8fc: ldr r0,[r0,#0x0]
  000ba8fe: ldr r0,[r0,#0x4]
  000ba900: ldr.w r0,[r0,#0x154]
  000ba904: blx 0x000718c0
  000ba908: mov r1,r0
  000ba90a: mov r0,r5
  000ba90c: blx 0x00072cdc
  000ba910: movs r0,#0x78
  000ba912: blx 0x0006eb24
  000ba916: mov r5,r0
  000ba918: blx 0x0007384c
  000ba91c: b.w 0x000bb51a
  000ba920: movs r0,#0x78
  000ba922: str.w r3,[r9,#0x0]
  000ba926: strd r12,lr,[r8,#0x0]
  000ba92a: blx 0x0006eb24
  000ba92e: mov r5,r0
  000ba930: movs r1,#0xa4
  000ba932: movs r2,#0x0
  000ba934: movs r3,#0x0
  000ba936: blx 0x00073834
  000ba93a: b.w 0x000b9c30
  000ba93e: movs r0,#0x78
  000ba940: str.w r3,[r9,#0x0]
  000ba944: strd r12,lr,[r8,#0x0]
  000ba948: blx 0x0006eb24
  000ba94c: mov r5,r0
  000ba94e: movs r1,#0xb
  000ba950: movs r2,#0x0
  000ba952: movs r3,#0x64
  000ba954: blx 0x00073834
  000ba958: b.w 0x000b9c30
  000ba95c: movs r0,#0x78
  000ba95e: str.w r3,[r9,#0x0]
  000ba962: strd r12,lr,[r8,#0x0]
  000ba966: blx 0x0006eb24
  000ba96a: mov r5,r0
  000ba96c: movs r1,#0x4
  000ba96e: movs r2,#0x0
  000ba970: movs r3,#0xa
  000ba972: blx 0x00073834
  000ba976: b.w 0x000b9c30
  000ba97a: ldr r0,[0x000baa2c]
  000ba97c: movs r1,#0x68
  000ba97e: str.w r3,[r9,#0x0]
  000ba982: movs r2,#0xa
  000ba984: add r0,pc
  000ba986: strd r12,lr,[r8,#0x0]
  000ba98a: ldr r5,[r0,#0x0]
  000ba98c: ldr r0,[r5,#0x0]
  000ba98e: ldr.w r0,[r0,#0x190]
  000ba992: blx 0x00072454
  000ba996: ldr r0,[r5,#0x0]
  000ba998: movw r1,#0x101
  000ba99c: ldr r0,[r0,#0x38]
  000ba99e: ldr r0,[r0,#0x4]
  000ba9a0: strh.w r1,[r0,#0x1b]
  000ba9a4: movs r0,#0x78
  000ba9a6: blx 0x0006eb24
  000ba9aa: mov r5,r0
  000ba9ac: movs r1,#0x4
  000ba9ae: movs r2,#0x0
  000ba9b0: movs r3,#0x6d
  000ba9b2: blx 0x00073834
  000ba9b6: b.w 0x000b9c30
  000ba9ba: ldr r0,[0x000baa30]
  000ba9bc: ldr r1,[0x000baa34]
  000ba9be: add r0,pc
  000ba9c0: str.w r3,[r9,#0x0]
  000ba9c4: strd r12,lr,[r8,#0x0]
  000ba9c8: add r1,pc
  000ba9ca: ldr r0,[r0,#0x0]
  000ba9cc: ldr r1,[r1,#0x0]
  000ba9ce: ldr r0,[r0,#0x0]
  000ba9d0: ldr.w r2,[r0,#0x1d0]
  000ba9d4: subs r2,#0x1
  000ba9d6: str.w r2,[r0,#0x1d0]
  000ba9da: ldr r0,[r1,#0x0]
  000ba9dc: blx 0x00073480
  000ba9e0: movs r1,#0x0
  000ba9e2: strb.w r1,[r0,#0x6d]
  000ba9e6: movs r0,#0x78
  000ba9e8: blx 0x0006eb24
  000ba9ec: mov r5,r0
  000ba9ee: movs r1,#0xb
  000ba9f0: movs r2,#0x0
  000ba9f2: movs r3,#0xa
  000ba9f4: blx 0x00073834
  000ba9f8: b.w 0x000b9c30
  000baa38: str.w r3,[r4,#0x1e8]
  000baa3c: movw r1,#0x101
  000baa40: strd r12,lr,[r8,#0x0]
  000baa44: ldr r0,[r4,#0x38]
  000baa46: ldr r0,[r0,#0x4]
  000baa48: strh.w r1,[r0,#0x1b]
  000baa4c: movs r0,#0x78
  000baa4e: blx 0x0006eb24
  000baa52: mov r5,r0
  000baa54: movs r1,#0xb8
  000baa56: movs r2,#0x0
  000baa58: movs r3,#0x6e
  000baa5a: movs r6,#0x0
  000baa5c: blx 0x00073834
  000baa60: mov r0,r4
  000baa62: mov r1,r5
  000baa64: blx 0x00073840
  000baa68: ldr.w r0,[r4,#0x198]
  000baa6c: movs r1,#0x0
  000baa6e: movs r2,#0xa
  000baa70: ldr r0,[r0,#0x4]
  000baa72: ldr r0,[r0,#0x0]
  000baa74: blx 0x00071ca4
  000baa78: b 0x000bb0e2
  000baa7a: movs r0,#0x78
  000baa7c: str.w r3,[r9,#0x0]
  000baa80: strd r12,lr,[r8,#0x0]
  000baa84: blx 0x0006eb24
  000baa88: mov r5,r0
  000baa8a: movs r1,#0xb8
  000baa8c: movs r2,#0x0
  000baa8e: movs r3,#0x71
  000baa90: blx 0x00073834
  000baa94: mov r0,r4
  000baa96: mov r1,r5
  000baa98: blx 0x00073840
  000baa9c: ldr.w r0,[r4,#0x198]
  000baaa0: movs r1,#0x0
  000baaa2: movs r2,#0xa
  000baaa4: ldr r0,[r0,#0x4]
  000baaa6: ldr r0,[r0,#0x0]
  000baaa8: blx 0x00071ca4
  000baaac: ldr.w r0,[r4,#0x198]
  000baab0: movs r1,#0xa
  000baab2: b.w 0x000bba20
  000baab6: movs r0,#0x78
  000baab8: str.w r3,[r9,#0x0]
  000baabc: strd r12,lr,[r8,#0x0]
  000baac0: blx 0x0006eb24
  000baac4: mov r5,r0
  000baac6: movs r1,#0xb
  000baac8: movs r2,#0x0
  000baaca: movs r3,#0x72
  000baacc: blx 0x00073834
  000baad0: b.w 0x000b9c30
  000baad4: ldr r0,[0x000baeb8]
  000baad6: movs r1,#0x1
  000baad8: str.w r3,[r9,#0x0]
  000baadc: add r0,pc
  000baade: strd r12,lr,[r8,#0x0]
  000baae2: ldr r0,[r0,#0x0]
  000baae4: ldr r0,[r0,#0x0]
  000baae6: ldr r0,[r0,#0x4]
  000baae8: ldr.w r0,[r0,#0x334]
  000baaec: blx 0x000718c0
  000baaf0: mov r1,r0
  000baaf2: ldr r0,[0x000baebc]
  000baaf4: add r0,pc
  000baaf6: ldr r0,[r0,#0x0]
  000baaf8: ldr r0,[r0,#0x0]
  000baafa: ldr.w r0,[r0,#0x190]
  000baafe: blx 0x00072cdc
  000bab02: movs r0,#0x78
  000bab04: blx 0x0006eb24
  000bab08: mov r5,r0
  000bab0a: movs r1,#0xb8
  000bab0c: movs r2,#0x0
  000bab0e: movs r3,#0x6f
  000bab10: movs r6,#0x0
  000bab12: blx 0x00073834
  000bab16: mov r0,r4
  000bab18: mov r1,r5
  000bab1a: blx 0x00073840
  000bab1e: ldr.w r0,[r4,#0x198]
  000bab22: movs r1,#0x0
  000bab24: movs r2,#0x53
  000bab26: ldr r0,[r0,#0x4]
  000bab28: ldr r0,[r0,#0x0]
  000bab2a: blx 0x00071ca4
  000bab2e: ldr.w r0,[r4,#0x198]
  000bab32: movs r1,#0x53
  000bab34: b 0x000bb0e8
  000bab36: movs r0,#0x78
  000bab38: str.w r3,[r9,#0x0]
  000bab3c: strd r12,lr,[r8,#0x0]
  000bab40: blx 0x0006eb24
  000bab44: mov r5,r0
  000bab46: movs r1,#0xaa
  000bab48: movs r2,#0x0
  000bab4a: movs r3,#0xa
  000bab4c: blx 0x00073834
  000bab50: b.w 0x000bba12
  000bab54: movs r0,#0x78
  000bab56: str.w r3,[r9,#0x0]
  000bab5a: strd r12,lr,[r8,#0x0]
  000bab5e: blx 0x0006eb24
  000bab62: mov r5,r0
  000bab64: movs r1,#0xb
  000bab66: movs r2,#0x0
  000bab68: movs r3,#0x62
  000bab6a: blx 0x00073834
  000bab6e: b.w 0x000b9c30
  000bab72: movs r0,#0x78
  000bab74: str.w r3,[r9,#0x0]
  000bab78: strd r12,lr,[r8,#0x0]
  000bab7c: blx 0x0006eb24
  000bab80: mov r5,r0
  000bab82: movs r1,#0x4
  000bab84: movs r2,#0x0
  000bab86: movs r3,#0x55
  000bab88: blx 0x00073834
  000bab8c: b.w 0x000b9c30
  000bab90: ldr r0,[0x000baec0]
  000bab92: movs r1,#0x1
  000bab94: str.w r3,[r9,#0x0]
  000bab98: add r0,pc
  000bab9a: strd r12,lr,[r8,#0x0]
  000bab9e: ldr r0,[r0,#0x0]
  000baba0: ldr r0,[r0,#0x0]
  000baba2: ldr r0,[r0,#0x38]
  000baba4: ldr r0,[r0,#0x4]
  000baba6: strb r1,[r0,#0x1d]
  000baba8: movs r0,#0x78
  000babaa: blx 0x0006eb24
  000babae: mov r5,r0
  000babb0: movs r1,#0xb
  000babb2: movs r2,#0x0
  000babb4: movs r3,#0x78
  000babb6: blx 0x00073834
  000babba: b.w 0x000b9c30
  000babbe: movs r0,#0x78
  000babc0: str.w r3,[r9,#0x0]
  000babc4: strd r12,lr,[r8,#0x0]
  000babc8: blx 0x0006eb24
  000babcc: mov r5,r0
  000babce: movs r1,#0xaa
  000babd0: movs r2,#0x0
  000babd2: movs r3,#0xa
  000babd4: blx 0x00073834
  000babd8: b.w 0x000bba12
  000babdc: movs r0,#0x78
  000babde: str.w r3,[r9,#0x0]
  000babe2: strd r12,lr,[r8,#0x0]
  000babe6: blx 0x0006eb24
  000babea: mov r5,r0
  000babec: movs r1,#0xb
  000babee: movs r2,#0x0
  000babf0: movs r3,#0x62
  000babf2: blx 0x00073834
  000babf6: b.w 0x000b9c30
  000babfa: ldr r0,[0x000baec4]
  000babfc: movs r1,#0x1
  000babfe: str.w r3,[r9,#0x0]
  000bac02: add r0,pc
  000bac04: strd r12,lr,[r8,#0x0]
  000bac08: ldr r0,[r0,#0x0]
  000bac0a: ldr r0,[r0,#0x0]
  000bac0c: ldr r0,[r0,#0x4]
  000bac0e: ldr.w r0,[r0,#0x33c]
  000bac12: blx 0x000718c0
  000bac16: mov r1,r0
  000bac18: ldr r0,[0x000baec8]
  000bac1a: add r0,pc
  000bac1c: ldr r0,[r0,#0x0]
  000bac1e: ldr r0,[r0,#0x0]
  000bac20: ldr.w r0,[r0,#0x190]
  000bac24: blx 0x00072cdc
  000bac28: movs r0,#0x78
  000bac2a: blx 0x0006eb24
  000bac2e: mov r5,r0
  000bac30: movs r1,#0xb8
  000bac32: movs r2,#0x0
  000bac34: movs r3,#0x71
  000bac36: blx 0x00073834
  000bac3a: mov r0,r4
  000bac3c: mov r1,r5
  000bac3e: blx 0x00073840
  000bac42: ldr.w r0,[r4,#0x198]
  000bac46: movs r1,#0x0
  000bac48: movw r2,#0x6a4
  000bac4c: ldr r0,[r0,#0x4]
  000bac4e: ldr r0,[r0,#0x0]
  000bac50: blx 0x00071ca4
  000bac54: ldr.w r0,[r4,#0x198]
  000bac58: movw r1,#0x6a4
  000bac5c: b.w 0x000bba20
  000bac60: movs r0,#0x78
  000bac62: str.w r3,[r9,#0x0]
  000bac66: strd r12,lr,[r8,#0x0]
  000bac6a: blx 0x0006eb24
  000bac6e: mov r5,r0
  000bac70: movs r1,#0xb
  000bac72: movs r2,#0x0
  000bac74: movs r3,#0xa
  000bac76: blx 0x00073834
  000bac7a: b.w 0x000b9c30
  000bac7e: movs r5,#0x0
  000bac80: str.w r3,[r4,#0x1e8]
  000bac84: strd r12,lr,[r8,#0x0]
  000bac88: b 0x000bac9c
  000bac8a: ldr r0,[r0,#0x4]
  000bac8c: ldr.w r0,[r0,r5,lsl #0x2]
  000bac90: blx 0x000733fc
  000bac94: cmp r0,#0xce
  000bac96: beq.w 0x000bb7f4
  000bac9a: adds r5,#0x1
  000bac9c: ldr r0,[r4,#0x18]
  000bac9e: ldr r1,[r0,#0x0]
  000baca0: cmp r5,r1
  000baca2: bcc 0x000bac8a
  000baca4: b.w 0x000bb826
  000baca8: movs r0,#0x78
  000bacaa: str.w r3,[r9,#0x0]
  000bacae: strd r12,lr,[r8,#0x0]
  000bacb2: blx 0x0006eb24
  000bacb6: mov r5,r0
  000bacb8: movs r1,#0x4
  000bacba: movs r2,#0x0
  000bacbc: movs r3,#0x6d
  000bacbe: blx 0x00073834
  000bacc2: b.w 0x000b9c30
  000bacc6: movs r0,#0x78
  000bacc8: str.w r3,[r9,#0x0]
  000baccc: strd r12,lr,[r8,#0x0]
  000bacd0: blx 0x0006eb24
  000bacd4: mov r5,r0
  000bacd6: movs r1,#0x4
  000bacd8: movs r2,#0x0
  000bacda: movs r3,#0x6f
  000bacdc: blx 0x00073834
  000bace0: b.w 0x000b9c30
  000bace4: movs r0,#0x78
  000bace6: str.w r3,[r9,#0x0]
  000bacea: strd r12,lr,[r8,#0x0]
  000bacee: blx 0x0006eb24
  000bacf2: mov r5,r0
  000bacf4: movs r1,#0xb
  000bacf6: movs r2,#0x0
  000bacf8: movs r3,#0xa
  000bacfa: blx 0x00073834
  000bacfe: b.w 0x000b9c30
  000bad02: movs r0,#0x78
  000bad04: str.w r3,[r9,#0x0]
  000bad08: strd r12,lr,[r8,#0x0]
  000bad0c: blx 0x0006eb24
  000bad10: mov r5,r0
  000bad12: movs r1,#0xaa
  000bad14: movs r2,#0x0
  000bad16: movs r3,#0x72
  000bad18: blx 0x00073834
  000bad1c: b.w 0x000bba12
  000bad20: movs r0,#0x78
  000bad22: str.w r3,[r9,#0x0]
  000bad26: strd r12,lr,[r8,#0x0]
  000bad2a: blx 0x0006eb24
  000bad2e: mov r5,r0
  000bad30: movs r1,#0xab
  000bad32: movs r2,#0x0
  000bad34: movs r3,#0x26
  000bad36: blx 0x00073834
  000bad3a: b.w 0x000b9c30
  000bad3e: movs r0,#0x78
  000bad40: str.w r3,[r9,#0x0]
  000bad44: strd r12,lr,[r8,#0x0]
  000bad48: blx 0x0006eb24
  000bad4c: mov r5,r0
  000bad4e: movs r1,#0xac
  000bad50: movs r2,#0x0
  000bad52: movs r3,#0x26
  000bad54: blx 0x00073834
  000bad58: mov r0,r4
  000bad5a: mov r1,r5
  000bad5c: blx 0x00073840
  000bad60: ldr.w r0,[r4,#0x198]
  000bad64: movs r1,#0x92
  000bad66: b.w 0x000bb846
  000bad6a: ldr r0,[0x000baecc]
  000bad6c: movs r1,#0x92
  000bad6e: str.w r3,[r9,#0x0]
  000bad72: movs r2,#0x1
  000bad74: add r0,pc
  000bad76: strd r12,lr,[r8,#0x0]
  000bad7a: ldr r0,[r0,#0x0]
  000bad7c: ldr r0,[r0,#0x0]
  000bad7e: ldr.w r0,[r0,#0x190]
  000bad82: blx 0x00072454
  000bad86: movs r0,#0x78
  000bad88: blx 0x0006eb24
  000bad8c: mov r5,r0
  000bad8e: movs r1,#0xab
  000bad90: movs r2,#0x0
  000bad92: movs r3,#0x52
  000bad94: blx 0x00073834
  000bad98: b.w 0x000b9c30
  000bad9c: movs r0,#0x78
  000bad9e: str.w r3,[r9,#0x0]
  000bada2: strd r12,lr,[r8,#0x0]
  000bada6: blx 0x0006eb24
  000badaa: mov r5,r0
  000badac: movs r1,#0xab
  000badae: movs r2,#0x0
  000badb0: movs r3,#0x52
  000badb2: blx 0x00073834
  000badb6: b.w 0x000b9c30
  000badba: movs r0,#0x78
  000badbc: str.w r3,[r9,#0x0]
  000badc0: strd r12,lr,[r8,#0x0]
  000badc4: blx 0x0006eb24
  000badc8: mov r5,r0
  000badca: movs r1,#0xab
  000badcc: movs r2,#0x0
  000badce: movs r3,#0x5d
  000badd0: blx 0x00073834
  000badd4: b.w 0x000bba12
  000badd8: str.w r3,[r4,#0x1e8]
  000baddc: movs r1,#0x1
  000badde: strd r12,lr,[r8,#0x0]
  000bade2: ldr r0,[r4,#0x38]
  000bade4: ldr r0,[r0,#0x4]
  000bade6: strb r1,[r0,#0x1e]
  000bade8: movs r0,#0x78
  000badea: blx 0x0006eb24
  000badee: mov r5,r0
  000badf0: movs r1,#0xb
  000badf2: movs r2,#0x0
  000badf4: movs r3,#0x7e
  000badf6: blx 0x00073834
  000badfa: b.w 0x000b9c30
  000badfe: movs r0,#0x78
  000bae00: str.w r3,[r9,#0x0]
  000bae04: strd r12,lr,[r8,#0x0]
  000bae08: blx 0x0006eb24
  000bae0c: mov r5,r0
  000bae0e: movs r1,#0x8
  000bae10: movs r2,#0x0
  000bae12: movs r3,#0x7e
  000bae14: blx 0x00073834
  000bae18: b 0x000bae8a
  000bae1a: str.w r3,[r4,#0x1e8]
  000bae1e: strd r12,lr,[r8,#0x0]
  000bae22: ldr.w r0,[r4,#0x190]
  000bae26: blx 0x00073750
  000bae2a: mov r5,r0
  000bae2c: cmp r0,#0x0
  000bae2e: beq.w 0x000bba00
  000bae32: movs r6,#0x0
  000bae34: b 0x000bae48
  000bae36: ldr r0,[r5,#0x4]
  000bae38: ldr.w r0,[r0,r6,lsl #0x2]
  000bae3c: blx 0x000718d8
  000bae40: cmp r0,#0xd1
  000bae42: beq.w 0x000bb9f4
  000bae46: adds r6,#0x1
  000bae48: ldr r0,[r5,#0x0]
  000bae4a: cmp r6,r0
  000bae4c: bcc 0x000bae36
  000bae4e: b.w 0x000bba00
  000bae52: movs r0,#0x78
  000bae54: str.w r3,[r9,#0x0]
  000bae58: strd r12,lr,[r8,#0x0]
  000bae5c: blx 0x0006eb24
  000bae60: mov r5,r0
  000bae62: movs r1,#0x4
  000bae64: movs r2,#0x0
  000bae66: movs r3,#0x28
  000bae68: blx 0x00073834
  000bae6c: b.w 0x000b9c30
  000bae70: movs r0,#0x78
  000bae72: str.w r3,[r9,#0x0]
  000bae76: strd r12,lr,[r8,#0x0]
  000bae7a: blx 0x0006eb24
  000bae7e: mov r5,r0
  000bae80: movs r1,#0x8
  000bae82: movs r2,#0x0
  000bae84: movs r3,#0x5d
  000bae86: blx 0x00073834
  000bae8a: mov r0,r4
  000bae8c: mov r1,r5
  000bae8e: blx 0x00073840
  000bae92: ldr.w r0,[r4,#0x198]
  000bae96: movs r1,#0xd1
  000bae98: b.w 0x000bb846
  000bae9c: str.w r3,[r4,#0x1e8]
  000baea0: strd r12,lr,[r8,#0x0]
  000baea4: ldr.w r0,[r4,#0x190]
  000baea8: blx 0x00073750
  000baeac: mov r5,r0
  000baeae: cmp r0,#0x0
  000baeb0: beq.w 0x000bba3c
  000baeb4: movs r6,#0x0
  000baeb6: b 0x000baee2
  000baed0: ldr r0,[r5,#0x4]
  000baed2: ldr.w r0,[r0,r6,lsl #0x2]
  000baed6: blx 0x000718d8
  000baeda: cmp r0,#0xd1
  000baedc: beq.w 0x000bba30
  000baee0: adds r6,#0x1
  000baee2: ldr r0,[r5,#0x0]
  000baee4: cmp r6,r0
  000baee6: bcc 0x000baed0
  000baee8: b.w 0x000bba3c
  000baeec: movs r0,#0x78
  000baeee: str.w r3,[r9,#0x0]
  000baef2: strd r12,lr,[r8,#0x0]
  000baef6: blx 0x0006eb24
  000baefa: mov r5,r0
  000baefc: movs r1,#0x4
  000baefe: movs r2,#0x0
  000baf00: movs r3,#0x79
  000baf02: blx 0x00073834
  000baf06: b.w 0x000b9c30
  000baf0a: movs r0,#0x78
  000baf0c: str.w r3,[r9,#0x0]
  000baf10: strd r12,lr,[r8,#0x0]
  000baf14: blx 0x0006eb24
  000baf18: mov r5,r0
  000baf1a: movs r1,#0xb
  000baf1c: movs r2,#0x0
  000baf1e: movs r3,#0x79
  000baf20: blx 0x00073834
  000baf24: b.w 0x000b9c30
  000baf28: movs r0,#0x78
  000baf2a: str.w r3,[r9,#0x0]
  000baf2e: strd r12,lr,[r8,#0x0]
  000baf32: blx 0x0006eb24
  000baf36: mov r5,r0
  000baf38: movs r1,#0x4
  000baf3a: movs r2,#0x0
  000baf3c: movs r3,#0x37
  000baf3e: blx 0x00073834
  000baf42: b.w 0x000b9c30
  000baf46: movs r0,#0x78
  000baf48: str.w r3,[r9,#0x0]
  000baf4c: strd r12,lr,[r8,#0x0]
  000baf50: blx 0x0006eb24
  000baf54: mov r5,r0
  000baf56: movs r1,#0xaa
  000baf58: movs r2,#0x0
  000baf5a: movs r3,#0x78
  000baf5c: blx 0x00073834
  000baf60: b.w 0x000bba12
  000baf64: movs r0,#0x78
  000baf66: str.w r3,[r9,#0x0]
  000baf6a: strd r12,lr,[r8,#0x0]
  000baf6e: blx 0x0006eb24
  000baf72: mov r5,r0
  000baf74: movs r1,#0xb
  000baf76: movs r2,#0x0
  000baf78: movs r3,#0x62
  000baf7a: blx 0x00073834
  000baf7e: b.w 0x000b9c30
  000baf82: movs r0,#0x78
  000baf84: str.w r3,[r9,#0x0]
  000baf88: strd r12,lr,[r8,#0x0]
  000baf8c: blx 0x0006eb24
  000baf90: mov r5,r0
  000baf92: blx 0x0007384c
  000baf96: b 0x000bb51a
  000baf98: movs r0,#0x78
  000baf9a: str.w r3,[r9,#0x0]
  000baf9e: strd r12,lr,[r8,#0x0]
  000bafa2: blx 0x0006eb24
  000bafa6: mov r5,r0
  000bafa8: blx 0x0007384c
  000bafac: b 0x000bb51a
  000bafae: movs r0,#0x78
  000bafb0: str.w r3,[r9,#0x0]
  000bafb4: strd r12,lr,[r8,#0x0]
  000bafb8: blx 0x0006eb24
  000bafbc: mov r5,r0
  000bafbe: movs r1,#0x4
  000bafc0: movs r2,#0x0
  000bafc2: movs r3,#0x70
  000bafc4: blx 0x00073834
  000bafc8: b.w 0x000b9c30
  000bafcc: movs r0,#0x78
  000bafce: str.w r3,[r9,#0x0]
  000bafd2: strd r12,lr,[r8,#0x0]
  000bafd6: blx 0x0006eb24
  000bafda: mov r5,r0
  000bafdc: movs r1,#0xb
  000bafde: movs r2,#0x0
  000bafe0: movs r3,#0x70
  000bafe2: blx 0x00073834
  000bafe6: b.w 0x000b9c30
  000bafea: movs r0,#0x78
  000bafec: str.w r3,[r9,#0x0]
  000baff0: strd r12,lr,[r8,#0x0]
  000baff4: blx 0x0006eb24
  000baff8: mov r5,r0
  000baffa: movs r1,#0xaa
  000baffc: movs r2,#0x0
  000baffe: movs r3,#0x78
  000bb000: blx 0x00073834
  000bb004: b.w 0x000bba12
  000bb008: movs r0,#0x78
  000bb00a: str.w r3,[r9,#0x0]
  000bb00e: strd r12,lr,[r8,#0x0]
  000bb012: blx 0x0006eb24
  000bb016: mov r5,r0
  000bb018: movs r1,#0xb
  000bb01a: movs r2,#0x0
  000bb01c: movs r3,#0x16
  000bb01e: blx 0x00073834
  000bb022: b.w 0x000bba12
  000bb026: movs r0,#0x78
  000bb028: str.w r3,[r9,#0x0]
  000bb02c: strd r12,lr,[r8,#0x0]
  000bb030: blx 0x0006eb24
  000bb034: mov r5,r0
  000bb036: movs r1,#0xae
  000bb038: movs r2,#0x0
  000bb03a: movs r3,#0x67
  000bb03c: blx 0x00073834
  000bb040: mov r0,r4
  000bb042: mov r1,r5
  000bb044: blx 0x00073840
  000bb048: ldr.w r0,[r4,#0x198]
  000bb04c: movs r1,#0x9b
  000bb04e: movs r2,#0x8c
  000bb050: ldr r0,[r0,#0x4]
  000bb052: ldr r0,[r0,#0x0]
  000bb054: blx 0x00071ca4
  000bb058: b.w 0x000bba1a
  000bb05c: movs r0,#0x78
  000bb05e: str.w r3,[r9,#0x0]
  000bb062: strd r12,lr,[r8,#0x0]
  000bb066: blx 0x0006eb24
  000bb06a: mov r5,r0
  000bb06c: movs r1,#0xb
  000bb06e: movs r2,#0x0
  000bb070: movs r3,#0x70
  000bb072: blx 0x00073834
  000bb076: b.w 0x000b9c30
  000bb07a: movs r0,#0x78
  000bb07c: str.w r3,[r9,#0x0]
  000bb080: strd r12,lr,[r8,#0x0]
  000bb084: blx 0x0006eb24
  000bb088: mov r5,r0
  000bb08a: movs r1,#0x4
  000bb08c: movs r2,#0x0
  000bb08e: movs r3,#0x3a
  000bb090: blx 0x00073834
  000bb094: b.w 0x000b9c30
  000bb098: movs r0,#0x78
  000bb09a: str.w r3,[r9,#0x0]
  000bb09e: strd r12,lr,[r8,#0x0]
  000bb0a2: blx 0x0006eb24
  000bb0a6: mov r5,r0
  000bb0a8: movs r1,#0xb
  000bb0aa: movs r2,#0x0
  000bb0ac: movs r3,#0x3a
  000bb0ae: blx 0x00073834
  000bb0b2: b.w 0x000b9c30
  000bb0b6: str.w r3,[r4,#0x1e8]
  000bb0ba: movs r1,#0x1
  000bb0bc: strd r12,lr,[r8,#0x0]
  000bb0c0: ldr r0,[r4,#0x38]
  000bb0c2: ldr r0,[r0,#0x4]
  000bb0c4: strb r1,[r0,#0x1f]
  000bb0c6: movs r0,#0x78
  000bb0c8: blx 0x0006eb24
  000bb0cc: mov r5,r0
  000bb0ce: movs r1,#0xa8
  000bb0d0: movs r2,#0x0
  000bb0d2: movs r3,#0x83
  000bb0d4: movs r6,#0x0
  000bb0d6: blx 0x00073834
  000bb0da: mov r0,r4
  000bb0dc: mov r1,r5
  000bb0de: blx 0x00073840
  000bb0e2: ldr.w r0,[r4,#0x198]
  000bb0e6: movs r1,#0xa
  000bb0e8: ldr r0,[r0,#0x4]
  000bb0ea: ldr r0,[r0,#0x0]
  000bb0ec: blx 0x00072898
  000bb0f0: str.w r6,[r4,#0x178]
  000bb0f4: b.w 0x000bb96a
  000bb0f8: movs r0,#0x78
  000bb0fa: str.w r3,[r9,#0x0]
  000bb0fe: strd r12,lr,[r8,#0x0]
  000bb102: blx 0x0006eb24
  000bb106: mov r5,r0
  000bb108: movs r1,#0xb
  000bb10a: movs r2,#0x0
  000bb10c: movs r3,#0x70
  000bb10e: blx 0x00073834
  000bb112: b.w 0x000b9c30
  000bb116: movs r6,#0x0
  000bb118: str.w r3,[r4,#0x1e8]
  000bb11c: strd r12,lr,[r8,#0x0]
  000bb120: b 0x000bb134
  000bb122: ldr r0,[r0,#0x4]
  000bb124: ldr.w r0,[r0,r6,lsl #0x2]
  000bb128: blx 0x000733fc
  000bb12c: cmp r0,#0xd2
  000bb12e: beq.w 0x000bb858
  000bb132: adds r6,#0x1
  000bb134: ldr r0,[r4,#0x18]
  000bb136: ldr r1,[r0,#0x0]
  000bb138: cmp r6,r1
  000bb13a: bcc 0x000bb122
  000bb13c: b 0x000bb8f2
  000bb13e: ldr r0,[0x000bb4fc]
  000bb140: ldr r1,[0x000bb4f8]
  000bb142: add r0,pc
  000bb144: str.w r3,[r9,#0x0]
  000bb148: add r1,pc
  000bb14a: strd r12,lr,[r8,#0x0]
  000bb14e: ldr.w r9,[r0,#0x0]
  000bb152: ldr.w r10,[r1,#0x0]
  000bb156: ldr.w r1,[r9,#0x0]
  000bb15a: ldr.w r0,[r10,#0x0]
  000bb15e: ldr r2,[0x000bb538]
  000bb160: ldr.w r8,[r1,#0x190]
  000bb164: movs r1,#0xf
  000bb166: ldr r0,[r0,#0x4]
  000bb168: add r2,pc
  000bb16a: ldr r6,[r2,#0x0]
  000bb16c: ldr.w r0,[r0,#0x314]
  000bb170: blx r6
  000bb172: mov r1,r0
  000bb174: ldr r0,[0x000bb53c]
  000bb176: add r0,pc
  000bb178: ldr r5,[r0,#0x0]
  000bb17a: mov r0,r8
  000bb17c: blx r5
  000bb17e: ldr.w r0,[r10,#0x0]
  000bb182: ldr.w r1,[r9,#0x0]
  000bb186: ldr r0,[r0,#0x4]
  000bb188: ldr.w r8,[r1,#0x190]
  000bb18c: movs r1,#0x1
  000bb18e: ldr.w r0,[r0,#0x310]
  000bb192: blx r6
  000bb194: mov r1,r0
  000bb196: mov r0,r8
  000bb198: blx r5
  000bb19a: ldr.w r0,[r10,#0x0]
  000bb19e: ldr.w r1,[r9,#0x0]
  000bb1a2: ldr r0,[r0,#0x4]
  000bb1a4: ldr.w r8,[r1,#0x190]
  000bb1a8: movs r1,#0x1
  000bb1aa: ldr.w r0,[r0,#0x318]
  000bb1ae: blx r6
  000bb1b0: mov r1,r0
  000bb1b2: mov r0,r8
  000bb1b4: blx r5
  000bb1b6: movs r0,#0x78
  000bb1b8: blx 0x0006eb24
  000bb1bc: mov r5,r0
  000bb1be: movs r1,#0x4
  000bb1c0: movs r2,#0x0
  000bb1c2: movs r3,#0x4f
  000bb1c4: blx 0x00073834
  000bb1c8: b.w 0x000b9c30
  000bb1cc: movs r0,#0x78
  000bb1ce: str.w r3,[r9,#0x0]
  000bb1d2: strd r12,lr,[r8,#0x0]
  000bb1d6: blx 0x0006eb24
  000bb1da: mov r5,r0
  000bb1dc: movs r1,#0x8
  000bb1de: movs r2,#0x0
  000bb1e0: movs r3,#0x70
  000bb1e2: blx 0x00073834
  000bb1e6: mov r0,r4
  000bb1e8: mov r1,r5
  000bb1ea: blx 0x00073840
  000bb1ee: ldr.w r0,[r4,#0x198]
  000bb1f2: movs r1,#0xd2
  000bb1f4: b 0x000bb846
  000bb1f6: ldr r0,[0x000bb540]
  000bb1f8: movs r1,#0xd2
  000bb1fa: str.w r3,[r9,#0x0]
  000bb1fe: movs r2,#0x1
  000bb200: add r0,pc
  000bb202: strd r12,lr,[r8,#0x0]
  000bb206: ldr r0,[r0,#0x0]
  000bb208: ldr r0,[r0,#0x0]
  000bb20a: ldr.w r0,[r0,#0x190]
  000bb20e: blx 0x00072454
  000bb212: movs r0,#0x78
  000bb214: blx 0x0006eb24
  000bb218: mov r5,r0
  000bb21a: movs r1,#0xaa
  000bb21c: movs r2,#0x0
  000bb21e: movs r3,#0x70
  000bb220: blx 0x00073834
  000bb224: b 0x000bba12
  000bb226: movs r0,#0x78
  000bb228: str.w r3,[r9,#0x0]
  000bb22c: strd r12,lr,[r8,#0x0]
  000bb230: blx 0x0006eb24
  000bb234: mov r5,r0
  000bb236: movs r1,#0x4
  000bb238: movs r2,#0x0
  000bb23a: movs r3,#0x70
  000bb23c: blx 0x00073834
  000bb240: b.w 0x000b9c30
  000bb244: movs r0,#0x78
  000bb246: str.w r3,[r9,#0x0]
  000bb24a: strd r12,lr,[r8,#0x0]
  000bb24e: blx 0x0006eb24
  000bb252: mov r5,r0
  000bb254: movs r1,#0xb
  000bb256: movs r2,#0x0
  000bb258: movs r3,#0x70
  000bb25a: blx 0x00073834
  000bb25e: b.w 0x000b9c30
  000bb262: movs r0,#0x78
  000bb264: str.w r3,[r9,#0x0]
  000bb268: strd r12,lr,[r8,#0x0]
  000bb26c: blx 0x0006eb24
  000bb270: mov r5,r0
  000bb272: movs r1,#0x4
  000bb274: movs r2,#0x0
  000bb276: mov.w r3,#0xffffffff
  000bb27a: blx 0x00073834
  000bb27e: b.w 0x000b9c30
  000bb282: movs r0,#0x78
  000bb284: str.w r3,[r9,#0x0]
  000bb288: strd r12,lr,[r8,#0x0]
  000bb28c: blx 0x0006eb24
  000bb290: mov r5,r0
  000bb292: movs r1,#0xa5
  000bb294: movs r2,#0x0
  000bb296: mov.w r3,#0xffffffff
  000bb29a: blx 0x00073834
  000bb29e: b.w 0x000b9c30
  000bb2a2: movs r0,#0x78
  000bb2a4: str.w r3,[r9,#0x0]
  000bb2a8: strd r12,lr,[r8,#0x0]
  000bb2ac: blx 0x0006eb24
  000bb2b0: mov r5,r0
  000bb2b2: movs r1,#0xb
  000bb2b4: movs r2,#0x0
  000bb2b6: movs r3,#0x62
  000bb2b8: blx 0x00073834
  000bb2bc: b.w 0x000b9c30
  000bb2c0: movs r0,#0x78
  000bb2c2: str.w r3,[r9,#0x0]
  000bb2c6: strd r12,lr,[r8,#0x0]
  000bb2ca: blx 0x0006eb24
  000bb2ce: mov r5,r0
  000bb2d0: movs r1,#0x4
  000bb2d2: movs r2,#0x0
  000bb2d4: mov.w r3,#0xffffffff
  000bb2d8: blx 0x00073834
  000bb2dc: b.w 0x000b9c30
  000bb2e0: movs r0,#0x78
  000bb2e2: str.w r3,[r9,#0x0]
  000bb2e6: strd r12,lr,[r8,#0x0]
  000bb2ea: blx 0x0006eb24
  000bb2ee: mov r5,r0
  000bb2f0: movs r1,#0xa4
  000bb2f2: movs r2,#0x0
  000bb2f4: movs r3,#0x0
  000bb2f6: blx 0x00073834
  000bb2fa: b.w 0x000b9c30
  000bb2fe: movs r0,#0x78
  000bb300: str.w r3,[r9,#0x0]
  000bb304: strd r12,lr,[r8,#0x0]
  000bb308: blx 0x0006eb24
  000bb30c: mov r5,r0
  000bb30e: movs r1,#0xb
  000bb310: movs r2,#0x0
  000bb312: movs r3,#0x63
  000bb314: blx 0x00073834
  000bb318: b.w 0x000b9c30
  000bb31c: movs r0,#0x78
  000bb31e: str.w r3,[r9,#0x0]
  000bb322: strd r12,lr,[r8,#0x0]
  000bb326: blx 0x0006eb24
  000bb32a: mov r5,r0
  000bb32c: movs r1,#0x4
  000bb32e: movs r2,#0x0
  000bb330: movs r3,#0x70
  000bb332: blx 0x00073834
  000bb336: b.w 0x000b9c30
  000bb33a: movs r0,#0x78
  000bb33c: str.w r3,[r9,#0x0]
  000bb340: strd r12,lr,[r8,#0x0]
  000bb344: blx 0x0006eb24
  000bb348: mov r5,r0
  000bb34a: movs r1,#0xb
  000bb34c: movs r2,#0x0
  000bb34e: movs r3,#0xa
  000bb350: blx 0x00073834
  000bb354: b.w 0x000b9c30
  000bb358: movs r0,#0x78
  000bb35a: str.w r3,[r9,#0x0]
  000bb35e: strd r12,lr,[r8,#0x0]
  000bb362: blx 0x0006eb24
  000bb366: mov r5,r0
  000bb368: movs r1,#0xaa
  000bb36a: movs r2,#0x0
  000bb36c: movs r3,#0x5d
  000bb36e: blx 0x00073834
  000bb372: b 0x000bba12
  000bb374: movs r0,#0x78
  000bb376: str.w r3,[r9,#0x0]
  000bb37a: strd r12,lr,[r8,#0x0]
  000bb37e: blx 0x0006eb24
  000bb382: mov r5,r0
  000bb384: blx 0x0007384c
  000bb388: b 0x000bb51a
  000bb38a: str.w r3,[r4,#0x1e8]
  000bb38e: movs r1,#0x0
  000bb390: strd r12,lr,[r8,#0x0]
  000bb394: ldr.w r0,[r4,#0x190]
  000bb398: blx 0x000738b8
  000bb39c: movs r0,#0x78
  000bb39e: blx 0x0006eb24
  000bb3a2: mov r5,r0
  000bb3a4: movs r1,#0x9a
  000bb3a6: movs r2,#0x0
  000bb3a8: movs r3,#0x4e
  000bb3aa: blx 0x00073834
  000bb3ae: mov r0,r4
  000bb3b0: mov r1,r5
  000bb3b2: blx 0x00073840
  000bb3b6: ldr.w r0,[r4,#0x198]
  000bb3ba: movs r1,#0x19
  000bb3bc: b 0x000bba20
  000bb3be: str.w r3,[r4,#0x1e8]
  000bb3c2: movs r1,#0x13
  000bb3c4: strd r12,lr,[r8,#0x0]
  000bb3c8: ldr.w r0,[r4,#0x190]
  000bb3cc: blx 0x0007228c
  000bb3d0: mov r1,r0
  000bb3d2: ldr.w r0,[r4,#0x190]
  000bb3d6: blx 0x00072c34
  000bb3da: ldr r0,[0x000bb544]
  000bb3dc: movs r1,#0x1
  000bb3de: ldr.w r5,[r4,#0x190]
  000bb3e2: add r0,pc
  000bb3e4: ldr r0,[r0,#0x0]
  000bb3e6: ldr r0,[r0,#0x0]
  000bb3e8: ldr r0,[r0,#0x4]
  000bb3ea: ldr.w r0,[r0,#0x158]
  000bb3ee: blx 0x000718c0
  000bb3f2: mov r1,r0
  000bb3f4: mov r0,r5
  000bb3f6: blx 0x000738c4
  000bb3fa: movs r0,#0x78
  000bb3fc: blx 0x0006eb24
  000bb400: mov r5,r0
  000bb402: movs r1,#0xb
  000bb404: movs r2,#0x0
  000bb406: movs r3,#0x4f
  000bb408: blx 0x00073834
  000bb40c: b.w 0x000b9c30
  000bb410: movs r0,#0x78
  000bb412: str.w r3,[r9,#0x0]
  000bb416: strd r12,lr,[r8,#0x0]
  000bb41a: blx 0x0006eb24
  000bb41e: mov r5,r0
  000bb420: movs r1,#0xc
  000bb422: movs r2,#0x0
  000bb424: movs r3,#0x1b
  000bb426: blx 0x00073834
  000bb42a: b.w 0x000b9c30
  000bb42e: movs r0,#0x78
  000bb430: str.w r3,[r9,#0x0]
  000bb434: strd r12,lr,[r8,#0x0]
  000bb438: blx 0x0006eb24
  000bb43c: mov r5,r0
  000bb43e: movs r1,#0x4
  000bb440: movs r2,#0x0
  000bb442: mov.w r3,#0xffffffff
  000bb446: blx 0x00073834
  000bb44a: b.w 0x000b9c30
  000bb44e: movs r0,#0x78
  000bb450: str.w r3,[r9,#0x0]
  000bb454: strd r12,lr,[r8,#0x0]
  000bb458: blx 0x0006eb24
  000bb45c: mov r5,r0
  000bb45e: movs r1,#0xb
  000bb460: movs r2,#0x0
  000bb462: movs r3,#0xa
  000bb464: blx 0x00073834
  000bb468: b.w 0x000b9c30
  000bb46c: movs r0,#0x78
  000bb46e: str.w r3,[r9,#0x0]
  000bb472: strd r12,lr,[r8,#0x0]
  000bb476: blx 0x0006eb24
  000bb47a: mov r5,r0
  000bb47c: movs r1,#0xb
  000bb47e: movs r2,#0x0
  000bb480: movs r3,#0x4e
  000bb482: blx 0x00073834
  000bb486: b.w 0x000b9c30
  000bb48a: movs r0,#0x12
  000bb48c: movs r1,#0x5b
  000bb48e: str.w r3,[r4,#0x1e8]
  000bb492: strd r12,lr,[r8,#0x0]
  000bb496: strd r0,r1,[r4,#0x7c]
  000bb49a: movs r0,#0x78
  000bb49c: blx 0x0006eb24
  000bb4a0: mov r5,r0
  000bb4a2: movs r1,#0x4
  000bb4a4: movs r2,#0x0
  000bb4a6: movs r3,#0x5b
  000bb4a8: blx 0x00073834
  000bb4ac: b.w 0x000b9c30
  000bb4b0: movs r0,#0x78
  000bb4b2: str.w r3,[r9,#0x0]
  000bb4b6: strd r12,lr,[r8,#0x0]
  000bb4ba: blx 0x0006eb24
  000bb4be: mov r5,r0
  000bb4c0: movs r1,#0xb
  000bb4c2: movs r2,#0x0
  000bb4c4: movs r3,#0x4a
  000bb4c6: blx 0x00073834
  000bb4ca: b.w 0x000b9c30
  000bb4ce: str.w r3,[r4,#0x1e8]
  000bb4d2: movs r1,#0x0
  000bb4d4: strd r12,lr,[r8,#0x0]
  000bb4d8: ldr.w r0,[r4,#0x190]
  000bb4dc: blx 0x000719ec
  000bb4e0: movs r0,#0x78
  000bb4e2: blx 0x0006eb24
  000bb4e6: mov r5,r0
  000bb4e8: movs r1,#0x9c
  000bb4ea: movs r2,#0x0
  000bb4ec: movs r3,#0x38
  000bb4ee: blx 0x00073834
  000bb4f2: b.w 0x000b9c30
  000bb500: movs r0,#0x78
  000bb502: str.w r3,[r9,#0x0]
  000bb506: strd r12,lr,[r8,#0x0]
  000bb50a: blx 0x0006eb24
  000bb50e: mov r5,r0
  000bb510: movs r1,#0xa0
  000bb512: movs r2,#0x0
  000bb514: movs r3,#0x1b
  000bb516: blx 0x00073834
  000bb51a: mov r0,r4
  000bb51c: mov r1,r5
  000bb51e: blx 0x00073840
  000bb522: ldr.w r0,[r4,#0x198]
  000bb526: movs r1,#0x0
  000bb528: ldr r0,[r0,#0x4]
  000bb52a: ldr r0,[r0,#0x0]
  000bb52c: pop.w {r8,r9,r10}
  000bb530: pop.w {r4,r5,r6,r7,lr}
  000bb534: b.w 0x001abf38
  000bb548: ldr r0,[0x000bb908]
  000bb54a: mov.w r1,#0xffffffff
  000bb54e: str.w r3,[r4,#0x1e8]
  000bb552: add r0,pc
  000bb554: strd r12,lr,[r8,#0x0]
  000bb558: ldr r0,[r0,#0x0]
  000bb55a: ldr r0,[r0,#0x0]
  000bb55c: ldr r0,[r0,#0x4]
  000bb55e: ldr.w r0,[r0,#0xa4]
  000bb562: blx 0x000719d4
  000bb566: mov r1,r0
  000bb568: mov r0,r4
  000bb56a: blx 0x00073870
  000bb56e: ldr.w r0,[r4,#0x190]
  000bb572: movs r1,#0x1
  000bb574: blx 0x000719ec
  000bb578: ldr r0,[0x000bb90c]
  000bb57a: movs r5,#0x0
  000bb57c: add r0,pc
  000bb57e: ldr r6,[r0,#0x0]
  000bb580: ldr r0,[0x000bb910]
  000bb582: add r0,pc
  000bb584: ldr.w r8,[r0,#0x0]
  000bb588: b 0x000bb5a4
  000bb58a: ldr.w r0,[r0,#0x2c4]
  000bb58e: blx 0x000718a8
  000bb592: mov r1,r0
  000bb594: ldr.w r0,[r8,#0x0]
  000bb598: mov r2,r5
  000bb59a: ldr.w r0,[r0,#0x190]
  000bb59e: blx 0x0007387c
  000bb5a2: adds r5,#0x1
  000bb5a4: ldr r0,[r6,#0x0]
  000bb5a6: cmp r5,#0x3
  000bb5a8: ldr r0,[r0,#0x4]
  000bb5aa: bne 0x000bb58a
  000bb5ac: ldr r1,[0x000bb914]
  000bb5ae: ldr.w r0,[r0,#0xe8]
  000bb5b2: add r1,pc
  000bb5b4: ldr r5,[r1,#0x0]
  000bb5b6: blx r5
  000bb5b8: ldr r2,[0x000bb918]
  000bb5ba: mov r1,r0
  000bb5bc: ldr.w r0,[r8,#0x0]
  000bb5c0: add r2,pc
  000bb5c2: ldr.w r9,[r2,#0x0]
  000bb5c6: movs r2,#0x0
  000bb5c8: ldr.w r0,[r0,#0x190]
  000bb5cc: blx r9
  000bb5ce: ldr r0,[r6,#0x0]
  000bb5d0: ldr r0,[r0,#0x4]
  000bb5d2: ldr.w r0,[r0,#0x14c]
  000bb5d6: blx r5
  000bb5d8: mov r1,r0
  000bb5da: ldr.w r0,[r8,#0x0]
  000bb5de: movs r2,#0x1
  000bb5e0: ldr.w r0,[r0,#0x190]
  000bb5e4: blx r9
  000bb5e6: ldr r0,[r6,#0x0]
  000bb5e8: ldr r0,[r0,#0x4]
  000bb5ea: ldr.w r0,[r0,#0xd0]
  000bb5ee: blx r5
  000bb5f0: mov r1,r0
  000bb5f2: ldr.w r0,[r8,#0x0]
  000bb5f6: movs r2,#0x2
  000bb5f8: ldr.w r0,[r0,#0x190]
  000bb5fc: blx r9
  000bb5fe: movs r0,#0x78
  000bb600: blx 0x0006eb24
  000bb604: mov r5,r0
  000bb606: movs r1,#0x9c
  000bb608: movs r2,#0x0
  000bb60a: movs r3,#0x3a
  000bb60c: blx 0x00073834
  000bb610: b.w 0x000b9c30
  000bb614: str.w r3,[r4,#0x1e8]
  000bb618: strd r12,lr,[r8,#0x0]
  000bb61c: ldr.w r0,[r4,#0x190]
  000bb620: blx 0x000738d0
  000bb624: movs r0,#0x78
  000bb626: blx 0x0006eb24
  000bb62a: mov r5,r0
  000bb62c: movs r1,#0x9e
  000bb62e: movs r2,#0x0
  000bb630: movs r3,#0x4e
  000bb632: blx 0x00073834
  000bb636: b.w 0x000b9c30
  000bb63a: movs r0,#0x78
  000bb63c: str.w r3,[r9,#0x0]
  000bb640: strd r12,lr,[r8,#0x0]
  000bb644: blx 0x0006eb24
  000bb648: mov r5,r0
  000bb64a: movs r1,#0x4
  000bb64c: movs r2,#0x0
  000bb64e: movs r3,#0x62
  000bb650: blx 0x00073834
  000bb654: b.w 0x000b9c30
  000bb658: str.w r3,[r4,#0x1e8]
  000bb65c: movs r1,#0x1
  000bb65e: strd r12,lr,[r8,#0x0]
  000bb662: ldr r0,[r4,#0x38]
  000bb664: ldr r0,[r0,#0x4]
  000bb666: strb r1,[r0,#0x6]
  000bb668: movs r0,#0x78
  000bb66a: blx 0x0006eb24
  000bb66e: mov r5,r0
  000bb670: movs r1,#0xb
  000bb672: movw r2,#0x4e20
  000bb676: movs r3,#0xa
  000bb678: blx 0x00073834
  000bb67c: b.w 0x000b9c30
  000bb680: movs r0,#0x78
  000bb682: str.w r3,[r9,#0x0]
  000bb686: strd r12,lr,[r8,#0x0]
  000bb68a: blx 0x0006eb24
  000bb68e: mov r5,r0
  000bb690: movs r1,#0xb
  000bb692: movs r2,#0x0
  000bb694: movs r3,#0x4c
  000bb696: blx 0x00073834
  000bb69a: b.w 0x000b9c30
  000bb69e: movs r0,#0x78
  000bb6a0: str.w r3,[r9,#0x0]
  000bb6a4: strd r12,lr,[r8,#0x0]
  000bb6a8: blx 0x0006eb24
  000bb6ac: mov r5,r0
  000bb6ae: movs r1,#0xbd
  000bb6b0: movs r2,#0x0
  000bb6b2: movs r3,#0x37
  000bb6b4: blx 0x00073834
  000bb6b8: mov r0,r4
  000bb6ba: mov r1,r5
  000bb6bc: blx 0x00073840
  000bb6c0: ldr.w r0,[r4,#0x198]
  000bb6c4: movs r1,#0x6
  000bb6c6: b 0x000bba20
  000bb6c8: movs r0,#0x78
  000bb6ca: str.w r3,[r9,#0x0]
  000bb6ce: strd r12,lr,[r8,#0x0]
  000bb6d2: blx 0x0006eb24
  000bb6d6: mov r5,r0
  000bb6d8: movs r1,#0xb
  000bb6da: movs r2,#0x0
  000bb6dc: movs r3,#0xa
  000bb6de: blx 0x00073834
  000bb6e2: b.w 0x000b9c30
  000bb6e6: movs r0,#0x78
  000bb6e8: str.w r3,[r9,#0x0]
  000bb6ec: strd r12,lr,[r8,#0x0]
  000bb6f0: blx 0x0006eb24
  000bb6f4: mov r5,r0
  000bb6f6: movs r1,#0x4
  000bb6f8: movs r2,#0x0
  000bb6fa: movs r3,#0x4f
  000bb6fc: blx 0x00073834
  000bb700: b.w 0x000b9c30
  000bb704: movs r0,#0x78
  000bb706: str.w r3,[r9,#0x0]
  000bb70a: strd r12,lr,[r8,#0x0]
  000bb70e: blx 0x0006eb24
  000bb712: mov r5,r0
  000bb714: movs r1,#0x4
  000bb716: movs r2,#0x0
  000bb718: movs r3,#0x16
  000bb71a: blx 0x00073834
  000bb71e: b.w 0x000b9c30
  000bb722: movs r0,#0x78
  000bb724: str.w r3,[r9,#0x0]
  000bb728: strd r12,lr,[r8,#0x0]
  000bb72c: blx 0x0006eb24
  000bb730: mov r5,r0
  000bb732: movs r1,#0x9c
  000bb734: movs r2,#0x0
  000bb736: movs r3,#0x19
  000bb738: blx 0x00073834
  000bb73c: b.w 0x000b9c30
  000bb740: movs r0,#0x78
  000bb742: str.w r3,[r9,#0x0]
  000bb746: strd r12,lr,[r8,#0x0]
  000bb74a: blx 0x0006eb24
  000bb74e: mov r5,r0
  000bb750: movs r1,#0x4
  000bb752: movs r2,#0x0
  000bb754: movs r3,#0x4e
  000bb756: blx 0x00073834
  000bb75a: b.w 0x000b9c30
  000bb75e: mvn r0,#0x9
  000bb762: str.w r3,[r4,#0x1e8]
  000bb766: strd r12,lr,[r8,#0x0]
  000bb76a: strd r0,r0,[r4,#0x7c]
  000bb76e: movs r0,#0x78
  000bb770: blx 0x0006eb24
  000bb774: mov r5,r0
  000bb776: movs r1,#0xa0
  000bb778: movs r2,#0x0
  000bb77a: mov.w r3,#0xffffffff
  000bb77e: blx 0x00073834
  000bb782: b.w 0x000b9c30
  000bb786: mov.w r0,#0xffffffff
  000bb78a: str.w r3,[r4,#0x1e8]
  000bb78e: strd r12,lr,[r8,#0x0]
  000bb792: strd r0,r0,[r4,#0x7c]
  000bb796: movs r0,#0x78
  000bb798: blx 0x0006eb24
  000bb79c: mov r5,r0
  000bb79e: movs r1,#0x4
  000bb7a0: movs r2,#0x0
  000bb7a2: movs r3,#0x30
  000bb7a4: blx 0x00073834
  000bb7a8: b.w 0x000b9c30
  000bb7ac: ldr r0,[r4,#0x18]
  000bb7ae: ldr r0,[r0,#0x4]
  000bb7b0: ldr.w r0,[r0,r5,lsl #0x2]
  000bb7b4: blx 0x000738a0
  000bb7b8: ldr r0,[0x000bb91c]
  000bb7ba: ldr r1,[r4,#0x18]
  000bb7bc: add r0,pc
  000bb7be: ldr r0,[r0,#0x0]
  000bb7c0: ldr r1,[r1,#0x4]
  000bb7c2: ldr r0,[r0,#0x0]
  000bb7c4: ldr.w r5,[r1,r5,lsl #0x2]
  000bb7c8: ldr r0,[r0,#0x4]
  000bb7ca: ldr.w r0,[r0,#0x290]
  000bb7ce: blx 0x000718a8
  000bb7d2: mov r1,r0
  000bb7d4: mov r0,r5
  000bb7d6: movs r2,#0x32
  000bb7d8: movs r3,#0xa
  000bb7da: blx 0x000738dc
  000bb7de: movs r0,#0x78
  000bb7e0: blx 0x0006eb24
  000bb7e4: mov r5,r0
  000bb7e6: movs r1,#0xb
  000bb7e8: movs r2,#0x0
  000bb7ea: movs r3,#0x1e
  000bb7ec: blx 0x00073834
  000bb7f0: b.w 0x000b9c30
  000bb7f4: ldr r0,[r4,#0x18]
  000bb7f6: ldr r0,[r0,#0x4]
  000bb7f8: ldr.w r0,[r0,r5,lsl #0x2]
  000bb7fc: blx 0x000738a0
  000bb800: ldr r0,[0x000bb920]
  000bb802: ldr r1,[r4,#0x18]
  000bb804: add r0,pc
  000bb806: ldr r0,[r0,#0x0]
  000bb808: ldr r1,[r1,#0x4]
  000bb80a: ldr r0,[r0,#0x0]
  000bb80c: ldr.w r5,[r1,r5,lsl #0x2]
  000bb810: ldr r0,[r0,#0x4]
  000bb812: ldr.w r0,[r0,#0x28c]
  000bb816: blx 0x000718a8
  000bb81a: mov r1,r0
  000bb81c: mov r0,r5
  000bb81e: movs r2,#0xa
  000bb820: movs r3,#0xa
  000bb822: blx 0x000738dc
  000bb826: movs r0,#0x78
  000bb828: blx 0x0006eb24
  000bb82c: mov r5,r0
  000bb82e: movs r1,#0xa6
  000bb830: movs r2,#0x0
  000bb832: movs r3,#0xa
  000bb834: blx 0x00073834
  000bb838: mov r0,r4
  000bb83a: mov r1,r5
  000bb83c: blx 0x00073840
  000bb840: ldr.w r0,[r4,#0x198]
  000bb844: movs r1,#0xce
  000bb846: ldr r0,[r0,#0x4]
  000bb848: ldr r0,[r0,#0x0]
  000bb84a: movs r2,#0x1
  000bb84c: pop.w {r8,r9,r10}
  000bb850: pop.w {r4,r5,r6,r7,lr}
  000bb854: b.w 0x001abf48
  000bb858: ldr r0,[r4,#0x18]
  000bb85a: ldr r0,[r0,#0x4]
  000bb85c: ldr.w r0,[r0,r6,lsl #0x2]
  000bb860: blx 0x000738a0
  000bb864: ldr r0,[0x000bb924]
  000bb866: add r0,pc
  000bb868: ldr r5,[r0,#0x0]
  000bb86a: ldr r0,[r4,#0x18]
  000bb86c: ldr r1,[r5,#0x0]
  000bb86e: ldr r0,[r0,#0x4]
  000bb870: ldr r2,[0x000bb928]
  000bb872: ldr r1,[r1,#0x4]
  000bb874: add r2,pc
  000bb876: ldr.w r9,[r0,r6,lsl #0x2]
  000bb87a: ldr.w r0,[r1,#0x324]
  000bb87e: ldr.w r10,[r2,#0x0]
  000bb882: blx r10
  000bb884: mov r1,r0
  000bb886: ldr r0,[0x000bb92c]
  000bb888: movw r2,#0x34f
  000bb88c: movs r3,#0x70
  000bb88e: add r0,pc
  000bb890: ldr.w r8,[r0,#0x0]
  000bb894: mov r0,r9
  000bb896: blx r8
  000bb898: ldr r0,[r4,#0x18]
  000bb89a: ldr r0,[r0,#0x4]
  000bb89c: ldr.w r9,[r0,r6,lsl #0x2]
  000bb8a0: ldr r0,[r5,#0x0]
  000bb8a2: ldr r0,[r0,#0x4]
  000bb8a4: ldr.w r0,[r0,#0x328]
  000bb8a8: blx r10
  000bb8aa: mov r1,r0
  000bb8ac: mov r0,r9
  000bb8ae: movw r2,#0x342
  000bb8b2: movs r3,#0x70
  000bb8b4: blx r8
  000bb8b6: ldr r0,[r4,#0x18]
  000bb8b8: ldr r0,[r0,#0x4]
  000bb8ba: ldr.w r9,[r0,r6,lsl #0x2]
  000bb8be: ldr r0,[r5,#0x0]
  000bb8c0: ldr r0,[r0,#0x4]
  000bb8c2: ldr.w r0,[r0,#0x32c]
  000bb8c6: blx r10
  000bb8c8: mov r1,r0
  000bb8ca: mov r0,r9
  000bb8cc: movw r2,#0x35d
  000bb8d0: movs r3,#0x70
  000bb8d2: blx r8
  000bb8d4: ldr r0,[r4,#0x18]
  000bb8d6: ldr r1,[r5,#0x0]
  000bb8d8: ldr r0,[r0,#0x4]
  000bb8da: ldr r1,[r1,#0x4]
  000bb8dc: ldr.w r5,[r0,r6,lsl #0x2]
  000bb8e0: ldr.w r0,[r1,#0x330]
  000bb8e4: blx r10
  000bb8e6: mov r1,r0
  000bb8e8: mov r0,r5
  000bb8ea: mov.w r2,#0x37c
  000bb8ee: movs r3,#0x70
  000bb8f0: blx r8
  000bb8f2: movs r0,#0x78
  000bb8f4: blx 0x0006eb24
  000bb8f8: mov r5,r0
  000bb8fa: movs r1,#0xb
  000bb8fc: movs r2,#0x0
  000bb8fe: movs r3,#0x4e
  000bb900: blx 0x00073834
  000bb904: b.w 0x000b9c30
  000bb930: ldr r0,[r4,#0x18]
  000bb932: ldr r0,[r0,#0x4]
  000bb934: ldr.w r0,[r0,r5,lsl #0x2]
  000bb938: blx 0x000738a0
  000bb93c: ldr r0,[0x000bbbb0]
  000bb93e: movs r3,#0x65
  000bb940: ldr r1,[r4,#0x18]
  000bb942: add r0,pc
  000bb944: ldr r0,[r0,#0x0]
  000bb946: ldr r1,[r1,#0x4]
  000bb948: ldr r0,[r0,#0x0]
  000bb94a: ldr r2,[r0,#0x4]
  000bb94c: ldr.w r0,[r1,r5,lsl #0x2]
  000bb950: ldr.w r1,[r2,#0x1fc]
  000bb954: movs r2,#0x5
  000bb956: blx 0x000738dc
  000bb95a: ldr.w r1,[r4,#0x8c]
  000bb95e: mov r0,r4
  000bb960: blx 0x00073870
  000bb964: movs r0,#0x0
  000bb966: str.w r0,[r4,#0x8c]
  000bb96a: pop.w {r8,r9,r10}
  000bb96e: pop {r4,r5,r6,r7,pc}
  000bb970: movs r1,#0x55
  000bb972: blx 0x00072424
  000bb976: mov r1,r0
  000bb978: cbz r0,0x000bb984
  000bb97a: ldr r0,[r5,#0x0]
  000bb97c: ldr.w r0,[r0,#0x190]
  000bb980: blx 0x000738e8
  000bb984: movs r5,#0x0
  000bb986: b 0x000bb9b2
  000bb988: ldr r0,[r0,#0x4]
  000bb98a: ldr.w r0,[r0,r5,lsl #0x2]
  000bb98e: blx 0x000738f4
  000bb992: cbnz r0,0x000bb9b0
  000bb994: ldr r0,[r4,#0x18]
  000bb996: ldr r0,[r0,#0x4]
  000bb998: ldr.w r0,[r0,r5,lsl #0x2]
  000bb99c: blx 0x00073624
  000bb9a0: cmp r0,#0x65
  000bb9a2: bne 0x000bb9b0
  000bb9a4: ldr r0,[r4,#0x18]
  000bb9a6: ldr r0,[r0,#0x4]
  000bb9a8: ldr.w r0,[r0,r5,lsl #0x2]
  000bb9ac: blx 0x00073900
  000bb9b0: adds r5,#0x1
  000bb9b2: ldr r0,[r4,#0x18]
  000bb9b4: ldr r1,[r0,#0x0]
  000bb9b6: cmp r5,r1
  000bb9b8: bcc 0x000bb988
  000bb9ba: movs r0,#0x78
  000bb9bc: blx 0x0006eb24
  000bb9c0: mov r5,r0
  000bb9c2: movs r1,#0x4
  000bb9c4: movs r2,#0x0
  000bb9c6: movs r3,#0x65
  000bb9c8: blx 0x00073834
  000bb9cc: b.w 0x000b9c30
  000bb9d0: ldr r0,[r5,#0x4]
  000bb9d2: movs r1,#0x1
  000bb9d4: ldr.w r0,[r0,r6,lsl #0x2]
  000bb9d8: blx 0x00073330
  000bb9dc: movs r0,#0x78
  000bb9de: blx 0x0006eb24
  000bb9e2: mov r5,r0
  000bb9e4: movs r1,#0x9c
  000bb9e6: movs r2,#0x0
  000bb9e8: mov.w r3,#0xffffffff
  000bb9ec: blx 0x00073834
  000bb9f0: b.w 0x000b9c30
  000bb9f4: ldr r0,[r5,#0x4]
  000bb9f6: movs r1,#0x1
  000bb9f8: ldr.w r0,[r0,r6,lsl #0x2]
  000bb9fc: blx 0x00073330
  000bba00: movs r0,#0x78
  000bba02: blx 0x0006eb24
  000bba06: mov r5,r0
  000bba08: movs r1,#0xaa
  000bba0a: movs r2,#0x0
  000bba0c: movs r3,#0xa
  000bba0e: blx 0x00073834
  000bba12: mov r0,r4
  000bba14: mov r1,r5
  000bba16: blx 0x00073840
  000bba1a: ldr.w r0,[r4,#0x198]
  000bba1e: movs r1,#0x0
  000bba20: ldr r0,[r0,#0x4]
  000bba22: ldr r0,[r0,#0x0]
  000bba24: pop.w {r8,r9,r10}
  000bba28: pop.w {r4,r5,r6,r7,lr}
  000bba2c: b.w 0x001abf58
  000bba30: ldr r0,[r5,#0x4]
  000bba32: movs r1,#0x0
  000bba34: ldr.w r0,[r0,r6,lsl #0x2]
  000bba38: blx 0x00073330
  000bba3c: ldr r0,[0x000bbbb4]
  000bba3e: movs r1,#0xd1
  000bba40: movs r2,#0x1
  000bba42: add r0,pc
  000bba44: ldr r0,[r0,#0x0]
  000bba46: ldr r0,[r0,#0x0]
  000bba48: ldr.w r0,[r0,#0x190]
  000bba4c: blx 0x00072454
  000bba50: movs r0,#0x78
  000bba52: blx 0x0006eb24
  000bba56: mov r5,r0
  000bba58: movs r1,#0xb
  000bba5a: movs r2,#0x0
  000bba5c: movs r3,#0xa
  000bba5e: blx 0x00073834
  000bba62: b.w 0x000b9c30
  001abf28: bx pc
  001abf38: bx pc
  001abf48: bx pc
  001abf58: bx pc
```
