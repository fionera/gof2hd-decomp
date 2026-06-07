# HangarWindow::setSellMode
elf 0x14bff0 body 1626
Sig: undefined __stdcall setSellMode(bool param_1)

## decompile
```c

/* HangarWindow::setSellMode(bool) */

void HangarWindow::setSellMode(bool param_1)

{
  HangarWindow HVar1;
  HangarWindow *this;
  int iVar2;
  undefined4 uVar3;
  Ship *pSVar4;
  Array *pAVar5;
  Station *pSVar6;
  void *pvVar7;
  Array *pAVar8;
  ListItem *pLVar9;
  int iVar10;
  Item *pIVar11;
  uint *puVar12;
  undefined4 *puVar13;
  ListItem *in_r1;
  bool bVar14;
  ListItem *this_00;
  BluePrint *this_01;
  int *piVar15;
  code *pcVar16;
  uint uVar17;
  ChoiceWindow *this_02;
  HangarList *pHVar18;
  Item *pIVar19;
  String *pSVar20;
  code *pcVar21;
  undefined4 uVar22;
  code *pcVar23;
  int *piVar24;
  String aSStack_b8 [12];
  String aSStack_ac [12];
  String aSStack_a0 [12];
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  undefined1 auStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  this = (HangarWindow *)(uint)param_1;
  piVar24 = *(int **)(DAT_0015c268 + 0x15c004);
  local_28 = *piVar24;
  this_00 = *(ListItem **)(this + 0x68);
  pLVar9 = this_00;
  if (((((this[0x88] == (HangarWindow)0x0) && (pLVar9 = in_r1, this_00 == (ListItem *)0x0)) ||
       (pLVar9 == (ListItem *)0x0)) ||
      ((iVar2 = ::ListItem::isShip(this_00), iVar2 != 0 ||
       (iVar2 = ::ListItem::isSlot(this_00), iVar2 != 0)))) ||
     ((iVar2 = ::ListItem::isTextButton(this_00), iVar2 != 0 ||
      ((iVar2 = ::ListItem::isSelectable(this_00), iVar2 == 0 ||
       (iVar2 = ::ListItem::isBluePrint(this_00), iVar2 != 0)))))) {
    this[0x88] = (HangarWindow)0x0;
LAB_0015c04a:
    if (*piVar24 == local_28) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  this[0x88] = SUB41(in_r1,0);
  iVar2 = HangarList::getCurrentTab(*(HangarList **)(this + 0x14));
  if (iVar2 == 1) {
    if (this[0x88] == (HangarWindow)0x0) {
      iVar2 = ::ListItem::isItem(this_00);
      if (((iVar2 != 0) && (iVar2 = Item::getType(*(Item **)(this_00 + 0x10)), iVar2 != 4)) &&
         (iVar2 = *(int *)(DAT_0015c280 + 0x15c168), *(char *)(iVar2 + 0x1e) == '\0')) {
        pSVar20 = *(String **)(this + 0x20);
        GameText::getText(**(int **)(DAT_0015c284 + 0x15c178));
        ChoiceWindow::set(pSVar20);
        *(undefined1 *)(iVar2 + 0x1e) = 1;
        this[0x3c] = (HangarWindow)0x1;
      }
      this[0xf8] = (HangarWindow)0x1;
      uVar3 = HangarList::getCurrentItemIndex(*(HangarList **)(this + 0x14));
      *(undefined4 *)(this + 0xfc) = uVar3;
    }
    else {
      iVar2 = *(int *)(DAT_0015c26c + 0x15c07c);
      if (*(char *)(iVar2 + 0x1d) == '\0') {
        pSVar20 = *(String **)(this + 0x20);
        GameText::getText(**(int **)(DAT_0015c270 + 0x15c08e));
        ChoiceWindow::set(pSVar20);
        *(undefined1 *)(iVar2 + 0x1d) = 1;
        this[0x3c] = (HangarWindow)0x1;
      }
      uVar3 = Item::getStationAmount(*(Item **)(this_00 + 0x10));
      *(undefined4 *)(this + 0x8c) = uVar3;
      uVar3 = Item::getAmount();
      *(undefined4 *)(this + 0xa0) = uVar3;
      uVar3 = Status::getCredits();
      *(undefined4 *)(this + 0x98) = uVar3;
      *(undefined4 *)(this + 0x9c) = *(undefined4 *)(this + 0xa8);
    }
    pSVar4 = (Ship *)Status::getShip();
    pAVar5 = (Array *)Item::extractItems(*(Array **)(this + 0x10),true);
    Ship::setCargo(pSVar4,pAVar5);
    pSVar6 = (Station *)Status::getStation();
    pAVar5 = (Array *)Item::extractItems(*(Array **)(this + 0x10),false);
    Station::setItems(pSVar6,pAVar5,false);
    if (*(Array **)(this + 0x10) != (Array *)0x0) {
      ArrayReleaseClasses<Item*>(*(Array **)(this + 0x10));
      if (*(Array<Item*> **)(this + 0x10) != (Array<Item*> *)0x0) {
        pvVar7 = (void *)Array<Item*>::~Array(*(Array<Item*> **)(this + 0x10));
        operator_delete(pvVar7);
      }
    }
    *(undefined4 *)(this + 0x10) = 0;
    Status::getShip();
    pAVar5 = (Array *)Ship::getCargo();
    Status::getStation();
    pAVar8 = (Array *)Station::getItems();
    pAVar5 = (Array *)Item::mixItems(pAVar5,pAVar8);
    pHVar18 = *(HangarList **)(this + 0x14);
    *(Array **)(this + 0x10) = pAVar5;
    Status::getStation();
    pAVar8 = (Array *)Station::getShips();
    HangarList::initShopTab(pHVar18,pAVar5,pAVar8);
    pHVar18 = *(HangarList **)(this + 0x14);
    pSVar4 = (Ship *)Status::getShip();
    HangarList::initShipTab(pHVar18,pSVar4);
    pLVar9 = (ListItem *)HangarList::getCurrentItem(*(HangarList **)(this + 0x14));
    *(ListItem **)(this + 0x68) = pLVar9;
    if ((pLVar9 != (ListItem *)0x0) && (iVar2 = ::ListItem::isShip(pLVar9), iVar2 != 0)) {
      Ship::adjustPrice(*(Ship **)(*(int *)(this + 0x68) + 0xc));
    }
    refreshCurrentContentHeight(this);
    goto LAB_0015c634;
  }
  iVar2 = HangarList::getCurrentTab(*(HangarList **)(this + 0x14));
  if (iVar2 != 4) goto LAB_0015c634;
  if (this[0x88] == (HangarWindow)0x0) {
    if ((*(int *)(this + 0x84) != 0) &&
       (this_01 = *(BluePrint **)(this + 0x80), this_01 != (BluePrint *)0x0)) {
      pIVar19 = *(Item **)(*(int *)(this + 0x84) + 0x10);
      iVar2 = Item::getBlueprintAmount(pIVar19);
      pSVar6 = (Station *)Status::getStation();
      iVar10 = Station::getIndex(pSVar6);
      BluePrint::addItem(this_01,pIVar19,iVar2,iVar10);
    }
    iVar2 = BluePrint::isCompleted(*(BluePrint **)(this + 0x80));
    if (iVar2 == 0) {
      HVar1 = (HangarWindow)0x0;
    }
    else {
      iVar2 = BluePrint::getStationIndex(*(BluePrint **)(this + 0x80));
      puVar13 = *(undefined4 **)(DAT_0015c67c + 0x15c2da);
      pSVar6 = (Station *)Status::getStation();
      iVar10 = Station::getIndex(pSVar6);
      if (iVar2 == iVar10) {
        piVar15 = *(int **)(DAT_0015c680 + 0x15c2f4);
        pSVar20 = (String *)GameText::getText(*piVar15);
        AbyssEngine::String::String(aSStack_34,pSVar20,false);
        uVar22 = *puVar13;
        AbyssEngine::String::String(aSStack_a0,aSStack_34,false);
        iVar2 = *piVar15;
        BluePrint::getIndex(*(BluePrint **)(this + 0x80));
        pSVar20 = (String *)GameText::getText(iVar2);
        AbyssEngine::String::String(aSStack_ac,pSVar20,false);
        uVar3 = AbyssEngine::String::String(aSStack_b8,(char *)(DAT_0015c684 + 0x15c338),false);
        Status::replaceHash(auStack_40,uVar22,aSStack_a0,aSStack_ac,uVar3);
        AbyssEngine::String::operator=(aSStack_34,auStack_40);
        pcVar16 = *(code **)(DAT_0015c710 + 0x15c35a);
        (*pcVar16)(auStack_40);
        (*pcVar16)(aSStack_b8);
        (*pcVar16)(aSStack_ac);
        (*pcVar16)(aSStack_a0);
        ChoiceWindow::set(*(String **)(this + 0x20));
        iVar10 = **(int **)(DAT_0015c714 + 0x15c378);
        iVar2 = BluePrint::getIndex(*(BluePrint **)(this + 0x80));
        iVar2 = *(int *)(*(int *)(iVar10 + 4) + iVar2 * 4);
        BluePrint::getQuantity(*(BluePrint **)(this + 0x80));
        pIVar19 = (Item *)Item::makeItem(iVar2);
        pIVar11 = (Item *)Status::getShip();
        Ship::addCargo(pIVar11);
        ArrayAdd<Item*>(pIVar19,*(Array **)(this + 0x10));
        HangarList::setCurrentTab(*(int *)(this + 0x14),true);
        refreshCurrentContentHeight(this);
      }
      else {
        piVar15 = *(int **)(DAT_0015c728 + 0x15c3ce);
        pSVar20 = (String *)GameText::getText(*piVar15);
        AbyssEngine::String::String(aSStack_34,pSVar20,false);
        uVar22 = *puVar13;
        AbyssEngine::String::String(aSStack_4c,aSStack_34,false);
        iVar2 = *piVar15;
        BluePrint::getIndex(*(BluePrint **)(this + 0x80));
        pSVar20 = (String *)GameText::getText(iVar2);
        AbyssEngine::String::String(aSStack_58,pSVar20,false);
        uVar3 = AbyssEngine::String::String(aSStack_64,(char *)(DAT_0015c72c + 0x15c412),false);
        Status::replaceHash(auStack_40,uVar22,aSStack_4c,aSStack_58,uVar3);
        AbyssEngine::String::operator=(aSStack_34,auStack_40);
        pcVar16 = *(code **)(DAT_0015c730 + 0x15c434);
        (*pcVar16)(auStack_40);
        (*pcVar16)(aSStack_64);
        (*pcVar16)(aSStack_58);
        (*pcVar16)(aSStack_4c);
        uVar22 = *puVar13;
        AbyssEngine::String::String(aSStack_70,aSStack_34,false);
        BluePrint::getStationName();
        AbyssEngine::String::String(aSStack_7c,aSStack_88,false);
        uVar3 = AbyssEngine::String::String(aSStack_94,(char *)(DAT_0015c734 + 0x15c46c),false);
        Status::replaceHash(auStack_40,uVar22,aSStack_70,aSStack_7c,uVar3);
        AbyssEngine::String::operator=(aSStack_34,auStack_40);
        (*pcVar16)(auStack_40);
        (*pcVar16)(aSStack_94);
        (*pcVar16)(aSStack_7c);
        (*pcVar16)(aSStack_88);
        (*pcVar16)(aSStack_70);
        ChoiceWindow::set(*(String **)(this + 0x20));
        Status::addPendingProduct((Status *)*puVar13,*(BluePrint **)(this + 0x80));
        HangarList::setCurrentTab(*(int *)(this + 0x14),true);
        refreshCargoAvailabilityForBlueprints(this);
      }
      AbyssEngine::String::~String(aSStack_34);
      BluePrint::reset(*(BluePrint **)(this + 0x80));
      HVar1 = (HangarWindow)0x1;
    }
    puVar13 = *(undefined4 **)(DAT_0015c718 + 0x15c4da);
    pcVar23 = *(code **)(DAT_0015c71c + 0x15c4dc);
    pSVar4 = (Ship *)(*pcVar23)(*puVar13);
    (*pcVar23)(*puVar13);
    pcVar16 = *(code **)(DAT_0015c720 + 0x15c4f6);
    pAVar5 = (Array *)(*pcVar16)();
    pAVar5 = (Array *)Item::extractItems(pAVar5,true);
    Ship::setCargo(pSVar4,pAVar5);
    pHVar18 = *(HangarList **)(this + 0x14);
    (*pcVar23)(*puVar13);
    pAVar5 = (Array *)(*pcVar16)();
    pcVar21 = *(code **)(DAT_0015c724 + 0x15c51a);
    (*pcVar21)(*puVar13);
    pAVar8 = (Array *)Station::getItems();
    pAVar5 = (Array *)Item::mixItems(pAVar5,pAVar8);
    (*pcVar21)(*puVar13);
    pAVar8 = (Array *)Station::getShips();
    HangarList::initShopTab(pHVar18,pAVar5,pAVar8);
    pHVar18 = *(HangarList **)(this + 0x14);
    pAVar5 = (Array *)Status::getBluePrints((Status *)*puVar13);
    HangarList::initBlueprintTab(pHVar18,pAVar5);
    (*pcVar23)(*puVar13);
    pAVar5 = (Array *)(*pcVar16)();
    (*pcVar21)(*puVar13);
    pAVar8 = (Array *)Station::getItems();
    uVar3 = Item::mixItems(pAVar5,pAVar8);
    *(undefined4 *)(this + 0x10) = uVar3;
    this[0x3c] = HVar1;
    if ((bool)HVar1) {
      puVar12 = (uint *)HangarList::getCurrentTabItems(*(HangarList **)(this + 0x14));
      for (uVar17 = 0; uVar17 < *puVar12; uVar17 = uVar17 + 1) {
        pLVar9 = *(ListItem **)(puVar12[1] + uVar17 * 4);
        if ((pLVar9 != (ListItem *)0x0) && (iVar2 = ::ListItem::isItem(pLVar9), iVar2 != 0)) {
          iVar2 = Item::getIndex(*(Item **)(pLVar9 + 0x10));
          iVar10 = BluePrint::getIndex(*(BluePrint **)(this + 0x80));
          if (iVar2 == iVar10) {
            iVar2 = Status::getShip();
            iVar10 = Item::getIndex(*(Item **)(pLVar9 + 0x10));
            iVar2 = Ship::hasEquipment(iVar2,iVar10);
            if (iVar2 != 0) {
              *(uint *)(this + 0xfc) = uVar17;
              this[0xf8] = (HangarWindow)0x1;
              autoEquipSecondaryWeapons((int)this);
              this[0xf8] = (HangarWindow)0x0;
              break;
            }
          }
        }
      }
    }
    goto LAB_0015c04a;
  }
  *(undefined4 *)(this + 0x94) = 0;
  iVar2 = BluePrint::isEmpty(*(BluePrint **)(this + 0x80));
  if ((iVar2 != 0) && (iVar2 = Item::getAmount(), 0 < iVar2)) {
    iVar2 = BluePrint::getIndex(*(BluePrint **)(this + 0x80));
    if ((iVar2 == 0xd2) ||
       (iVar2 = BluePrint::getIndex(*(BluePrint **)(this + 0x80)), iVar2 == 0xdf)) {
      Status::getSystem();
      iVar2 = SolarSystem::getRoutes();
      if (iVar2 != 0) goto LAB_0015c134;
      this_02 = *(ChoiceWindow **)(this + 0x20);
      puVar13 = (undefined4 *)(DAT_0015c738 + 0x15c5ee);
      this[0x130] = (HangarWindow)0x1;
      pSVar20 = (String *)GameText::getText(*(int *)*puVar13);
      bVar14 = false;
    }
    else {
LAB_0015c134:
      this_02 = *(ChoiceWindow **)(this + 0x20);
      pSVar20 = (String *)GameText::getText(**(int **)(DAT_0015c27c + 0x15c13e));
      bVar14 = true;
    }
    ChoiceWindow::set(this_02,pSVar20,bVar14);
    this[0x3c] = (HangarWindow)0x1;
  }
  uVar3 = Item::getBlueprintAmount(*(Item **)(this_00 + 0x10));
  *(undefined4 *)(this + 0xa4) = uVar3;
  uVar3 = Item::getAmount();
  *(undefined4 *)(this + 0xa0) = uVar3;
  uVar3 = Status::getCredits();
  *(undefined4 *)(this + 0x98) = uVar3;
  *(undefined4 *)(this + 0x9c) = *(undefined4 *)(this + 0xa8);
  *(undefined4 *)(this + 0x84) = *(undefined4 *)(this + 0x68);
LAB_0015c634:
  if (*piVar24 - local_28 == 0) {
    (*(code *)(DAT_001ac4d4 + 0x1ac4d8))();
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(*piVar24 - local_28);
}

```

## target disasm
```
  0015bff0: push {r4,r5,r6,r7,lr}
  0015bff2: add r7,sp,#0xc
  0015bff4: push {r8,r9,r10,r11}
  0015bff8: sub sp,#0xa4
  0015bffa: mov r4,r0
  0015bffc: ldr r0,[0x0015c268]
  0015bffe: mov r6,r1
  0015c000: add r0,pc
  0015c002: ldr.w r10,[r0,#0x0]
  0015c006: ldr.w r0,[r10,#0x0]
  0015c00a: str r0,[sp,#0xa0]
  0015c00c: ldrb.w r0,[r4,#0x88]
  0015c010: ldr r5,[r4,#0x68]
  0015c012: cbz r0,0x0015c018
  0015c014: cbnz r5,0x0015c01c
  0015c016: b 0x0015c044
  0015c018: cbz r5,0x0015c044
  0015c01a: cbz r6,0x0015c044
  0015c01c: mov r0,r5
  0015c01e: blx 0x00076204
  0015c022: cbnz r0,0x0015c044
  0015c024: mov r0,r5
  0015c026: blx 0x00076210
  0015c02a: cbnz r0,0x0015c044
  0015c02c: mov r0,r5
  0015c02e: blx 0x00077008
  0015c032: cbnz r0,0x0015c044
  0015c034: mov r0,r5
  0015c036: blx 0x00076ffc
  0015c03a: cbz r0,0x0015c044
  0015c03c: mov r0,r5
  0015c03e: blx 0x000768f4
  0015c042: cbz r0,0x0015c060
  0015c044: movs r0,#0x0
  0015c046: strb.w r0,[r4,#0x88]
  0015c04a: ldr r0,[sp,#0xa0]
  0015c04c: ldr.w r1,[r10,#0x0]
  0015c050: subs r0,r1,r0
  0015c052: ittt eq
  0015c054: add.eq sp,#0xa4
  0015c056: pop.eq.w {r8,r9,r10,r11}
  0015c05a: pop.eq {r4,r5,r6,r7,pc}
  0015c05c: blx 0x0006e824
  0015c060: ldr r0,[r4,#0x14]
  0015c062: strb.w r6,[r4,#0x88]
  0015c066: blx 0x00076f48
  0015c06a: cmp r0,#0x1
  0015c06c: bne 0x0015c0ce
  0015c06e: ldrb.w r0,[r4,#0x88]
  0015c072: cmp r0,#0x0
  0015c074: beq 0x0015c150
  0015c076: ldr r0,[0x0015c26c]
  0015c078: add r0,pc
  0015c07a: ldr r6,[r0,#0x0]
  0015c07c: ldrb r0,[r6,#0x1d]
  0015c07e: cbnz r0,0x0015c0a4
  0015c080: ldr r0,[0x0015c270]
  0015c082: mov.w r1,#0x24c
  0015c086: ldr.w r8,[r4,#0x20]
  0015c08a: add r0,pc
  0015c08c: ldr r0,[r0,#0x0]
  0015c08e: ldr r0,[r0,#0x0]
  0015c090: blx 0x00072f70
  0015c094: mov r1,r0
  0015c096: mov r0,r8
  0015c098: blx 0x00074794
  0015c09c: movs r0,#0x1
  0015c09e: strb r0,[r6,#0x1d]
  0015c0a0: strb.w r0,[r4,#0x3c]
  0015c0a4: ldr r0,[r5,#0x10]
  0015c0a6: blx 0x00077014
  0015c0aa: str.w r0,[r4,#0x8c]
  0015c0ae: ldr r0,[r5,#0x10]
  0015c0b0: blx 0x0007183c
  0015c0b4: ldr r1,[0x0015c274]
  0015c0b6: str.w r0,[r4,#0xa0]
  0015c0ba: add r1,pc
  0015c0bc: ldr r1,[r1,#0x0]
  0015c0be: ldr r0,[r1,#0x0]
  0015c0c0: blx 0x000733d8
  0015c0c4: ldr.w r1,[r4,#0xa8]
  0015c0c8: strd r0,r1,[r4,#0x98]
  0015c0cc: b 0x0015c19e
  0015c0ce: ldr r0,[r4,#0x14]
  0015c0d0: blx 0x00076f48
  0015c0d4: cmp r0,#0x4
  0015c0d6: bne.w 0x0015c634
  0015c0da: ldrb.w r0,[r4,#0x88]
  0015c0de: cmp r0,#0x0
  0015c0e0: beq.w 0x0015c28c
  0015c0e4: ldr.w r0,[r4,#0x80]
  0015c0e8: movs r1,#0x0
  0015c0ea: str.w r1,[r4,#0x94]
  0015c0ee: blx 0x000738f4
  0015c0f2: cmp r0,#0x0
  0015c0f4: beq.w 0x0015c606
  0015c0f8: ldr r0,[r4,#0x68]
  0015c0fa: ldr r0,[r0,#0x10]
  0015c0fc: blx 0x0007183c
  0015c100: cmp r0,#0x1
  0015c102: blt.w 0x0015c606
  0015c106: ldr.w r0,[r4,#0x80]
  0015c10a: blx 0x000733fc
  0015c10e: cmp r0,#0xd2
  0015c110: beq 0x0015c11e
  0015c112: ldr.w r0,[r4,#0x80]
  0015c116: blx 0x000733fc
  0015c11a: cmp r0,#0xdf
  0015c11c: bne 0x0015c134
  0015c11e: ldr r0,[0x0015c278]
  0015c120: add r0,pc
  0015c122: ldr r0,[r0,#0x0]
  0015c124: ldr r0,[r0,#0x0]
  0015c126: blx 0x00071a10
  0015c12a: blx 0x00071aac
  0015c12e: cmp r0,#0x0
  0015c130: beq.w 0x0015c5de
  0015c134: ldr r0,[0x0015c27c]
  0015c136: movs r1,#0xd4
  0015c138: ldr r6,[r4,#0x20]
  0015c13a: add r0,pc
  0015c13c: ldr r0,[r0,#0x0]
  0015c13e: ldr r0,[r0,#0x0]
  0015c140: blx 0x00072f70
  0015c144: mov r1,r0
  0015c146: mov.w r8,#0x1
  0015c14a: mov r0,r6
  0015c14c: movs r2,#0x1
  0015c14e: b 0x0015c5fe
  0015c150: mov r0,r5
  0015c152: blx 0x000768e8
  0015c156: cbz r0,0x0015c18e
  0015c158: ldr r0,[r5,#0x10]
  0015c15a: blx 0x000718fc
  0015c15e: cmp r0,#0x4
  0015c160: beq 0x0015c18e
  0015c162: ldr r0,[0x0015c280]
  0015c164: add r0,pc
  0015c166: ldr r6,[r0,#0x0]
  0015c168: ldrb r0,[r6,#0x1e]
  0015c16a: cbnz r0,0x0015c18e
  0015c16c: ldr r0,[0x0015c284]
  0015c16e: movw r1,#0x24d
  0015c172: ldr r5,[r4,#0x20]
  0015c174: add r0,pc
  0015c176: ldr r0,[r0,#0x0]
  0015c178: ldr r0,[r0,#0x0]
  0015c17a: blx 0x00072f70
  0015c17e: mov r1,r0
  0015c180: mov r0,r5
  0015c182: blx 0x00074794
  0015c186: movs r0,#0x1
  0015c188: strb r0,[r6,#0x1e]
  0015c18a: strb.w r0,[r4,#0x3c]
  0015c18e: ldr r0,[r4,#0x14]
  0015c190: movs r1,#0x1
  0015c192: strb.w r1,[r4,#0xf8]
  0015c196: blx 0x000770e0
  0015c19a: str.w r0,[r4,#0xfc]
  0015c19e: ldr r0,[0x0015c288]
  0015c1a0: add r0,pc
  0015c1a2: ldr.w r8,[r0,#0x0]
  0015c1a6: ldr.w r0,[r8,#0x0]
  0015c1aa: blx 0x00071a58
  0015c1ae: mov r5,r0
  0015c1b0: ldr r0,[r4,#0x10]
  0015c1b2: movs r1,#0x1
  0015c1b4: blx 0x000770d4
  0015c1b8: mov r1,r0
  0015c1ba: mov r0,r5
  0015c1bc: blx 0x000738b8
  0015c1c0: ldr.w r0,[r8,#0x0]
  0015c1c4: blx 0x00071c14
  0015c1c8: mov r5,r0
  0015c1ca: ldr r0,[r4,#0x10]
  0015c1cc: movs r1,#0x0
  0015c1ce: movs r6,#0x0
  0015c1d0: blx 0x000770d4
  0015c1d4: mov r1,r0
  0015c1d6: mov r0,r5
  0015c1d8: movs r2,#0x0
  0015c1da: blx 0x000736e4
  0015c1de: ldr r0,[r4,#0x10]
  0015c1e0: cbz r0,0x0015c1f2
  0015c1e2: blx 0x00073420
  0015c1e6: ldr r0,[r4,#0x10]
  0015c1e8: cbz r0,0x0015c1f2
  0015c1ea: blx 0x0007342c
  0015c1ee: blx 0x0006eb48
  0015c1f2: str r6,[r4,#0x10]
  0015c1f4: ldr.w r0,[r8,#0x0]
  0015c1f8: blx 0x00071a58
  0015c1fc: blx 0x00073750
  0015c200: mov r5,r0
  0015c202: ldr.w r0,[r8,#0x0]
  0015c206: blx 0x00071c14
  0015c20a: blx 0x00071830
  0015c20e: mov r1,r0
  0015c210: mov r0,r5
  0015c212: blx 0x00076ee8
  0015c216: ldr r6,[r4,#0x14]
  0015c218: mov r5,r0
  0015c21a: str r0,[r4,#0x10]
  0015c21c: ldr.w r0,[r8,#0x0]
  0015c220: blx 0x00071c14
  0015c224: blx 0x00073708
  0015c228: mov r2,r0
  0015c22a: mov r0,r6
  0015c22c: mov r1,r5
  0015c22e: blx 0x000761ec
  0015c232: ldr.w r0,[r8,#0x0]
  0015c236: ldr r5,[r4,#0x14]
  0015c238: blx 0x00071a58
  0015c23c: mov r1,r0
  0015c23e: mov r0,r5
  0015c240: blx 0x000761e0
  0015c244: ldr r0,[r4,#0x14]
  0015c246: blx 0x000770bc
  0015c24a: cmp r0,#0x0
  0015c24c: str r0,[r4,#0x68]
  0015c24e: beq 0x0015c25e
  0015c250: blx 0x00076204
  0015c254: cbz r0,0x0015c25e
  0015c256: ldr r0,[r4,#0x68]
  0015c258: ldr r0,[r0,#0xc]
  0015c25a: blx 0x000719f8
  0015c25e: mov r0,r4
  0015c260: blx 0x00076f30
  0015c264: b 0x0015c634
  0015c28c: ldr.w r0,[r4,#0x84]
  0015c290: cbz r0,0x0015c2be
  0015c292: ldr.w r5,[r4,#0x80]
  0015c296: cbz r5,0x0015c2be
  0015c298: ldr r6,[r0,#0x10]
  0015c29a: mov r0,r6
  0015c29c: blx 0x00076f78
  0015c2a0: mov r8,r0
  0015c2a2: ldr r0,[0x0015c678]
  0015c2a4: add r0,pc
  0015c2a6: ldr r0,[r0,#0x0]
  0015c2a8: ldr r0,[r0,#0x0]
  0015c2aa: blx 0x00071c14
  0015c2ae: blx 0x00071824
  0015c2b2: mov r3,r0
  0015c2b4: mov r0,r5
  0015c2b6: mov r1,r6
  0015c2b8: mov r2,r8
  0015c2ba: blx 0x000738dc
  0015c2be: ldr.w r0,[r4,#0x80]
  0015c2c2: blx 0x00077104
  0015c2c6: cmp r0,#0x0
  0015c2c8: beq 0x0015c3c2
  0015c2ca: ldr.w r0,[r4,#0x80]
  0015c2ce: blx 0x00073624
  0015c2d2: mov r5,r0
  0015c2d4: ldr r0,[0x0015c67c]
  0015c2d6: add r0,pc
  0015c2d8: ldr.w r8,[r0,#0x0]
  0015c2dc: ldr.w r0,[r8,#0x0]
  0015c2e0: blx 0x00071c14
  0015c2e4: blx 0x00071824
  0015c2e8: cmp r5,r0
  0015c2ea: bne 0x0015c3c6
  0015c2ec: ldr r0,[0x0015c680]
  0015c2ee: movs r1,#0xd3
  0015c2f0: add r0,pc
  0015c2f2: ldr r5,[r0,#0x0]
  0015c2f4: ldr r0,[r5,#0x0]
  0015c2f6: blx 0x00072f70
  0015c2fa: add r6,sp,#0x94
  0015c2fc: mov r1,r0
  0015c2fe: movs r2,#0x0
  0015c300: mov r0,r6
  0015c302: blx 0x0006f028
  0015c306: ldr.w r9,[r8,#0x0]
  0015c30a: add r0,sp,#0x28
  0015c30c: mov r1,r6
  0015c30e: movs r2,#0x0
  0015c310: blx 0x0006f028
  0015c314: ldr r6,[r5,#0x0]
  0015c316: ldr.w r0,[r4,#0x80]
  0015c31a: blx 0x000733fc
  0015c31e: addw r1,r0,#0x4fa
  0015c322: mov r0,r6
  0015c324: blx 0x00072f70
  0015c328: mov r1,r0
  0015c32a: add r0,sp,#0x1c
  0015c32c: movs r2,#0x0
  0015c32e: blx 0x0006f028
  0015c332: ldr r1,[0x0015c684]
  0015c334: add r1,pc
  0015c336: add r0,sp,#0x10
  0015c338: movs r2,#0x0
  0015c33a: blx 0x0006ee18
  0015c33e: str r0,[sp,#0x0]
  0015c340: add r0,sp,#0x88
  0015c342: add r2,sp,#0x28
  0015c344: add r3,sp,#0x1c
  0015c346: mov r1,r9
  0015c348: blx 0x000739b4
  0015c34c: add r0,sp,#0x94
  0015c34e: add r1,sp,#0x88
  0015c350: blx 0x0006f2b0
  0015c354: ldr r0,[0x0015c710]
  0015c356: add r0,pc
  0015c358: ldr r5,[r0,#0x0]
  0015c35a: add r0,sp,#0x88
  0015c35c: blx r5
  0015c35e: add r0,sp,#0x10
  0015c360: blx r5
  0015c362: add r0,sp,#0x1c
  0015c364: blx r5
  0015c366: add r0,sp,#0x28
  0015c368: blx r5
  0015c36a: ldr r0,[r4,#0x20]
  0015c36c: add r1,sp,#0x94
  0015c36e: blx 0x00074794
  0015c372: ldr r0,[0x0015c714]
  0015c374: add r0,pc
  0015c376: ldr r0,[r0,#0x0]
  0015c378: ldr r5,[r0,#0x0]
  0015c37a: ldr.w r0,[r4,#0x80]
  0015c37e: blx 0x000733fc
  0015c382: ldr r1,[r5,#0x4]
  0015c384: ldr.w r5,[r1,r0,lsl #0x2]
  0015c388: ldr.w r0,[r4,#0x80]
  0015c38c: blx 0x00073654
  0015c390: mov r1,r0
  0015c392: mov r0,r5
  0015c394: blx 0x000718c0
  0015c398: mov r5,r0
  0015c39a: ldr.w r0,[r8,#0x0]
  0015c39e: blx 0x00071a58
  0015c3a2: mov r1,r5
  0015c3a4: blx 0x00072cdc
  0015c3a8: ldr r1,[r4,#0x10]
  0015c3aa: mov r0,r5
  0015c3ac: blx 0x000718b4
  0015c3b0: ldr r0,[r4,#0x14]
  0015c3b2: movs r1,#0x1
  0015c3b4: movs r2,#0x1
  0015c3b6: blx 0x00076f24
  0015c3ba: mov r0,r4
  0015c3bc: blx 0x00076f30
  0015c3c0: b 0x0015c4c0
  0015c3c2: movs r0,#0x0
  0015c3c4: b 0x0015c4d0
  0015c3c6: ldr r0,[0x0015c728]
  0015c3c8: movs r1,#0xd2
  0015c3ca: add r0,pc
  0015c3cc: ldr r5,[r0,#0x0]
  0015c3ce: ldr r0,[r5,#0x0]
  0015c3d0: blx 0x00072f70
  0015c3d4: add r6,sp,#0x94
  0015c3d6: mov r1,r0
  0015c3d8: movs r2,#0x0
  0015c3da: mov r0,r6
  0015c3dc: blx 0x0006f028
  0015c3e0: ldr.w r9,[r8,#0x0]
  0015c3e4: add r0,sp,#0x7c
  0015c3e6: mov r1,r6
  0015c3e8: movs r2,#0x0
  0015c3ea: blx 0x0006f028
  0015c3ee: ldr r6,[r5,#0x0]
  0015c3f0: ldr.w r0,[r4,#0x80]
  0015c3f4: blx 0x000733fc
  0015c3f8: addw r1,r0,#0x4fa
  0015c3fc: mov r0,r6
  0015c3fe: blx 0x00072f70
  0015c402: mov r1,r0
  0015c404: add r0,sp,#0x70
  0015c406: movs r2,#0x0
  0015c408: blx 0x0006f028
  0015c40c: ldr r1,[0x0015c72c]
  0015c40e: add r1,pc
  0015c410: add r0,sp,#0x64
  0015c412: movs r2,#0x0
  0015c414: blx 0x0006ee18
  0015c418: str r0,[sp,#0x0]
  0015c41a: add r0,sp,#0x88
  0015c41c: add r2,sp,#0x7c
  0015c41e: add r3,sp,#0x70
  0015c420: mov r1,r9
  0015c422: blx 0x000739b4
  0015c426: add r0,sp,#0x94
  0015c428: add r1,sp,#0x88
  0015c42a: blx 0x0006f2b0
  0015c42e: ldr r0,[0x0015c730]
  0015c430: add r0,pc
  0015c432: ldr r6,[r0,#0x0]
  0015c434: add r0,sp,#0x88
  0015c436: blx r6
  0015c438: add r0,sp,#0x64
  0015c43a: blx r6
  0015c43c: add r0,sp,#0x70
  0015c43e: blx r6
  0015c440: add r0,sp,#0x7c
  0015c442: blx r6
  0015c444: ldr.w r5,[r8,#0x0]
  0015c448: add r0,sp,#0x58
  0015c44a: add r1,sp,#0x94
  0015c44c: movs r2,#0x0
  0015c44e: blx 0x0006f028
  0015c452: ldr.w r1,[r4,#0x80]
  0015c456: add r0,sp,#0x40
  0015c458: blx 0x0007672c
  0015c45c: add r0,sp,#0x4c
  0015c45e: add r1,sp,#0x40
  0015c460: movs r2,#0x0
  0015c462: blx 0x0006f028
  0015c466: ldr r1,[0x0015c734]
  0015c468: add r1,pc
  0015c46a: add r0,sp,#0x34
  0015c46c: movs r2,#0x0
  0015c46e: blx 0x0006ee18
  0015c472: str r0,[sp,#0x0]
  0015c474: add r0,sp,#0x88
  0015c476: add r2,sp,#0x58
  0015c478: add r3,sp,#0x4c
  0015c47a: mov r1,r5
  0015c47c: blx 0x000739b4
  0015c480: add r0,sp,#0x94
  0015c482: add r1,sp,#0x88
  0015c484: blx 0x0006f2b0
  0015c488: add r0,sp,#0x88
  0015c48a: blx r6
  0015c48c: add r0,sp,#0x34
  0015c48e: blx r6
  0015c490: add r0,sp,#0x4c
  0015c492: blx r6
  0015c494: add r0,sp,#0x40
  0015c496: blx r6
  0015c498: add r0,sp,#0x58
  0015c49a: blx r6
  0015c49c: ldr r0,[r4,#0x20]
  0015c49e: add r1,sp,#0x94
  0015c4a0: blx 0x00074794
  0015c4a4: ldr.w r1,[r4,#0x80]
  0015c4a8: ldr.w r0,[r8,#0x0]
  0015c4ac: blx 0x00077110
  0015c4b0: ldr r0,[r4,#0x14]
  0015c4b2: movs r1,#0x2
  0015c4b4: movs r2,#0x1
  0015c4b6: blx 0x00076f24
  0015c4ba: mov r0,r4
  0015c4bc: blx 0x0007711c
  0015c4c0: add r0,sp,#0x94
  0015c4c2: blx 0x0006ee30
  0015c4c6: ldr.w r0,[r4,#0x80]
  0015c4ca: blx 0x00073900
  0015c4ce: movs r0,#0x1
  0015c4d0: str r0,[sp,#0xc]
  0015c4d2: ldr r0,[0x0015c718]
  0015c4d4: ldr r1,[0x0015c71c]
  0015c4d6: add r0,pc
  0015c4d8: add r1,pc
  0015c4da: ldr.w r11,[r0,#0x0]
  0015c4de: ldr.w r9,[r1,#0x0]
  0015c4e2: ldr.w r0,[r11,#0x0]
  0015c4e6: blx r9
  0015c4e8: mov r5,r0
  0015c4ea: ldr.w r0,[r11,#0x0]
  0015c4ee: blx r9
  0015c4f0: ldr r1,[0x0015c720]
  0015c4f2: add r1,pc
  0015c4f4: ldr r6,[r1,#0x0]
  0015c4f6: blx r6
  0015c4f8: movs r1,#0x1
  0015c4fa: str r6,[sp,#0x8]
  0015c4fc: blx 0x000770d4
  0015c500: mov r1,r0
  0015c502: mov r0,r5
  0015c504: blx 0x000738b8
  0015c508: ldr.w r0,[r11,#0x0]
  0015c50c: ldr r5,[r4,#0x14]
  0015c50e: blx r9
  0015c510: blx r6
  0015c512: mov r6,r0
  0015c514: ldr r0,[0x0015c724]
  0015c516: add r0,pc
  0015c518: ldr.w r8,[r0,#0x0]
  0015c51c: ldr.w r0,[r11,#0x0]
  0015c520: blx r8
  0015c522: blx 0x00071830
  0015c526: mov r1,r0
  0015c528: mov r0,r6
  0015c52a: blx 0x00076ee8
  0015c52e: mov r6,r0
  0015c530: ldr.w r0,[r11,#0x0]
  0015c534: blx r8
  0015c536: blx 0x00073708
  0015c53a: mov r2,r0
  0015c53c: mov r0,r5
  0015c53e: mov r1,r6
  0015c540: blx 0x000761ec
  0015c544: ldr.w r0,[r11,#0x0]
  0015c548: ldr r5,[r4,#0x14]
  0015c54a: blx 0x000748a8
  0015c54e: mov r1,r0
  0015c550: mov r0,r5
  0015c552: blx 0x000761f8
  0015c556: ldr.w r0,[r11,#0x0]
  0015c55a: blx r9
  0015c55c: ldr r1,[sp,#0x8]
  0015c55e: blx r1
  0015c560: mov r5,r0
  0015c562: ldr.w r0,[r11,#0x0]
  0015c566: blx r8
  0015c568: blx 0x00071830
  0015c56c: mov r1,r0
  0015c56e: mov r0,r5
  0015c570: blx 0x00076ee8
  0015c574: str r0,[r4,#0x10]
  0015c576: ldr r0,[sp,#0xc]
  0015c578: strb.w r0,[r4,#0x3c]
  0015c57c: cmp r0,#0x0
  0015c57e: beq.w 0x0015c04a
  0015c582: ldr r0,[r4,#0x14]
  0015c584: blx 0x00076f3c
  0015c588: mov r8,r0
  0015c58a: movs r5,#0x0
  0015c58c: b 0x0015c5d4
  0015c58e: ldr.w r0,[r8,#0x4]
  0015c592: ldr.w r6,[r0,r5,lsl #0x2]
  0015c596: cbz r6,0x0015c5d2
  0015c598: mov r0,r6
  0015c59a: blx 0x000768e8
  0015c59e: cbz r0,0x0015c5d2
  0015c5a0: ldr r0,[r6,#0x10]
  0015c5a2: blx 0x000718d8
  0015c5a6: mov r9,r0
  0015c5a8: ldr.w r0,[r4,#0x80]
  0015c5ac: blx 0x000733fc
  0015c5b0: cmp r9,r0
  0015c5b2: bne 0x0015c5d2
  0015c5b4: ldr.w r0,[r11,#0x0]
  0015c5b8: blx 0x00071a58
  0015c5bc: mov r9,r0
  0015c5be: ldr r0,[r6,#0x10]
  0015c5c0: blx 0x000718d8
  0015c5c4: mov r1,r0
  0015c5c6: mov r0,r9
  0015c5c8: movs r2,#0x1
  0015c5ca: blx 0x00073978
  0015c5ce: cmp r0,#0x0
  0015c5d0: bne 0x0015c654
  0015c5d2: adds r5,#0x1
  0015c5d4: ldr.w r0,[r8,#0x0]
  0015c5d8: cmp r5,r0
  0015c5da: bcc 0x0015c58e
  0015c5dc: b 0x0015c04a
  0015c5de: ldr r0,[0x0015c738]
  0015c5e0: mov.w r8,#0x1
  0015c5e4: ldr r6,[r4,#0x20]
  0015c5e6: mov.w r1,#0x210
  0015c5ea: add r0,pc
  0015c5ec: strb.w r8,[r4,#0x130]
  0015c5f0: ldr r0,[r0,#0x0]
  0015c5f2: ldr r0,[r0,#0x0]
  0015c5f4: blx 0x00072f70
  0015c5f8: mov r1,r0
  0015c5fa: mov r0,r6
  0015c5fc: movs r2,#0x0
  0015c5fe: blx 0x000746f8
  0015c602: strb.w r8,[r4,#0x3c]
  0015c606: ldr r0,[r5,#0x10]
  0015c608: blx 0x00076f78
  0015c60c: str.w r0,[r4,#0xa4]
  0015c610: ldr r0,[r5,#0x10]
  0015c612: blx 0x0007183c
  0015c616: ldr r1,[0x0015c73c]
  0015c618: str.w r0,[r4,#0xa0]
  0015c61c: add r1,pc
  0015c61e: ldr r1,[r1,#0x0]
  0015c620: ldr r0,[r1,#0x0]
  0015c622: blx 0x000733d8
  0015c626: ldr.w r2,[r4,#0xa8]
  0015c62a: ldr r1,[r4,#0x68]
  0015c62c: strd r0,r2,[r4,#0x98]
  0015c630: str.w r1,[r4,#0x84]
  0015c634: ldr r0,[sp,#0xa0]
  0015c636: ldr.w r1,[r10,#0x0]
  0015c63a: subs r0,r1,r0
  0015c63c: itttt eq
  0015c63e: mov.eq r0,r4
  0015c640: add.eq sp,#0xa4
  0015c642: pop.eq.w {r8,r9,r10,r11}
  0015c646: pop.eq.w {r4,r5,r6,r7,lr}
  0015c64a: it eq
  0015c64c: b.eq.w 0x001ac4c8
  0015c650: blx 0x0006e824
  0015c654: movs r0,#0x1
  0015c656: str.w r5,[r4,#0xfc]
  0015c65a: strb.w r0,[r4,#0xf8]
  0015c65e: mov r0,r4
  0015c660: mov r1,r5
  0015c662: blx 0x000770ec
  0015c666: movs r0,#0x0
  0015c668: strb.w r0,[r4,#0xf8]
  0015c66c: b 0x0015c04a
  001ac4c8: bx pc
```
