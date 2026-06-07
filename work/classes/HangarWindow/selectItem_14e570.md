# HangarWindow::selectItem
elf 0x14e570 body 1620
Sig: undefined __thiscall selectItem(HangarWindow * this, ListItem * param_1)

## decompile
```c

/* HangarWindow::selectItem(ListItem*) */

void __thiscall HangarWindow::selectItem(HangarWindow *this,ListItem *param_1)

{
  HangarWindow HVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  Ship *pSVar5;
  int iVar6;
  Array *pAVar7;
  Station *this_00;
  void *pvVar8;
  Array *pAVar9;
  undefined4 uVar10;
  ChoiceWindow *this_01;
  Item *pIVar11;
  HangarList *pHVar12;
  code *pcVar13;
  String *pSVar14;
  int *piVar15;
  undefined4 uVar16;
  int *piVar17;
  int *piVar18;
  String aSStack_b8 [12];
  String aSStack_ac [12];
  String aSStack_a0 [12];
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  undefined1 auStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar18 = *(int **)(DAT_0015e924 + 0x15e586);
  local_28 = *piVar18;
  *(ListItem **)(this + 0x68) = param_1;
  if ((param_1 != (ListItem *)0x0) && (iVar2 = ::ListItem::isShip(param_1), iVar2 != 0)) {
    Ship::adjustPrice(*(Ship **)(*(int *)(this + 0x68) + 0xc));
  }
  iVar2 = HangarList::getCurrentTab(*(HangarList **)(this + 0x14));
  if (iVar2 == 2) {
    iVar2 = ::ListItem::isSelectable(param_1);
    if ((iVar2 != 0) && (iVar2 = ::ListItem::isPendingProduct(param_1), iVar2 == 0)) {
      uVar10 = *(undefined4 *)(param_1 + 8);
      *(undefined4 *)(this + 0x80) = uVar10;
      HangarList::fillIngredientsList(*(BluePrint **)(this + 0x14),SUB41(uVar10,0));
      HangarList::setCurrentTab(*(int *)(this + 0x14),true);
      refreshCurrentContentHeight(this);
      if (this[0x89] != (HangarWindow)0x0) {
        this[0x89] = (HangarWindow)0x0;
      }
    }
    goto LAB_0015ebbe;
  }
  if (iVar2 != 1) {
    if ((iVar2 != 0) || (iVar2 = ::ListItem::isSelectable(param_1), iVar2 == 0)) goto LAB_0015ebbe;
    iVar2 = *(int *)(DAT_0015e928 + 0x15e5cc);
    *(undefined4 *)(this + 0xbc) = 0;
    *(undefined4 *)(this + 0xe4) = *(undefined4 *)(this + 0xb4);
    *(undefined4 *)(this + 0xb4) = 0;
    if ((*(char *)(iVar2 + 0x1f) == '\0') && (iVar3 = ::ListItem::isSlot(param_1), iVar3 != 0)) {
      pSVar14 = *(String **)(this + 0x20);
      GameText::getText(**(int **)(DAT_0015e92c + 0x15e5f4));
      ChoiceWindow::set(pSVar14);
      *(undefined1 *)(iVar2 + 0x1f) = 1;
      this[0x3c] = (HangarWindow)0x1;
    }
    iVar2 = ::ListItem::isSelectable(param_1);
    if (iVar2 == 0) goto LAB_0015ebbe;
    iVar2 = HangarList::getCurrentItem(*(HangarList **)(this + 0x14));
    pIVar11 = *(Item **)(iVar2 + 0x10);
    if (pIVar11 != (Item *)0x0) {
      iVar2 = Item::isUnsaleable(pIVar11);
      if (iVar2 != 0) {
LAB_0015e630:
        pSVar14 = *(String **)(this + 0x20);
        puVar4 = (undefined4 *)(DAT_0015e930 + 0x15e63c);
        goto LAB_0015e6e4;
      }
      iVar2 = Item::getSort(pIVar11);
      if (iVar2 == 0x14) {
        piVar17 = *(int **)(DAT_0015e94c + 0x15e852);
        iVar3 = *(int *)(*piVar17 + 0x34);
        iVar2 = iVar3;
        if (0 < iVar3) {
          iVar2 = *(int *)(param_1 + 0x3c);
        }
        iVar6 = iVar3 + -1;
        if (iVar3 >= 1) {
          iVar6 = iVar2;
        }
        if (iVar6 < 0 == (iVar3 < 1 && SBORROW4(iVar3,1))) {
          Status::getShip();
          iVar2 = Ship::getMaxPassengers();
          iVar3 = Item::getAttribute((int)pIVar11);
          if (iVar2 - iVar3 < *(int *)(*piVar17 + 0x34)) goto LAB_0015e630;
        }
      }
    }
    if (-1 < *(int *)(param_1 + 0x3c)) {
      iVar2 = HangarList::getCurrentItem(*(HangarList **)(this + 0x14));
      demountItem(this,pIVar11,*(int *)(iVar2 + 0x40));
      goto LAB_0015ebbe;
    }
    puVar4 = *(undefined4 **)(DAT_0015e954 + 0x15e8a8);
    iVar2 = Status::getShip();
    Item::getSort(*(Item **)(param_1 + 0x10));
    pIVar11 = (Item *)Ship::getFirstEquipmentOfSort(iVar2);
    iVar2 = Item::getSort(*(Item **)(param_1 + 0x10));
    if (iVar2 == 0x15) {
      Status::getShip();
      iVar2 = Ship::getIndex();
      if (iVar2 != 0x2c) {
        Status::getShip();
        iVar2 = Ship::getIndex();
        if (pIVar11 != (Item *)0x0 && iVar2 != 0x31) goto LAB_0015ea90;
      }
    }
    else if (pIVar11 != (Item *)0x0) {
LAB_0015ea90:
      iVar2 = Item::canBeInstalledMultipleTimes(*(Item **)(param_1 + 0x10));
      if (iVar2 == 0) {
        piVar17 = *(int **)(DAT_0015ec88 + 0x15eae0);
        pSVar14 = (String *)GameText::getText(*piVar17);
        AbyssEngine::String::String(aSStack_34,pSVar14,false);
        uVar16 = *puVar4;
        AbyssEngine::String::String(aSStack_58,aSStack_34,false);
        iVar2 = *piVar17;
        Item::getIndex(pIVar11);
        pSVar14 = (String *)GameText::getText(iVar2);
        AbyssEngine::String::String(aSStack_64,pSVar14,false);
        uVar10 = AbyssEngine::String::String(aSStack_70,(char *)(DAT_0015ec8c + 0x15eb28),false);
        Status::replaceHash(auStack_4c,uVar16,aSStack_58,aSStack_64,uVar10);
        iVar2 = *piVar17;
        Item::getIndex(*(Item **)(param_1 + 0x10));
        pSVar14 = (String *)GameText::getText(iVar2);
        AbyssEngine::String::String(aSStack_7c,pSVar14,false);
        uVar10 = AbyssEngine::String::String(aSStack_88,(char *)(DAT_0015ec90 + 0x15eb60),false);
        Status::replaceHash(aSStack_40,uVar16,auStack_4c,aSStack_7c,uVar10);
        AbyssEngine::String::operator=(aSStack_34,aSStack_40);
        pcVar13 = *(code **)(DAT_0015ec94 + 0x15eb82);
        (*pcVar13)(aSStack_40);
        (*pcVar13)(aSStack_88);
        (*pcVar13)(aSStack_7c);
        (*pcVar13)(auStack_4c);
        (*pcVar13)(aSStack_70);
        (*pcVar13)(aSStack_64);
        (*pcVar13)(aSStack_58);
        ChoiceWindow::set(*(ChoiceWindow **)(this + 0x20),aSStack_34,true);
        this[0x11c] = (HangarWindow)0x1;
        this[0x3c] = (HangarWindow)0x1;
        *(undefined4 *)(this + 0x28) = *(undefined4 *)(param_1 + 0x10);
        *(Item **)(this + 0x2c) = pIVar11;
        goto LAB_0015ebb8;
      }
    }
    mountItem(this,*(Item **)(param_1 + 0x10));
    goto LAB_0015ebbe;
  }
  iVar2 = ::ListItem::isSelectable(param_1);
  if (iVar2 == 0) goto LAB_0015ebbe;
  iVar2 = ::ListItem::isShip(param_1);
  if (iVar2 == 0) {
    iVar2 = Item::isUnsaleable(*(Item **)(param_1 + 0x10));
    if (iVar2 == 0) {
      HVar1 = this[0x88];
      this[0x88] = (HangarWindow)((byte)HVar1 ^ 1);
      if (HVar1 == (HangarWindow)0x0) {
        iVar2 = *(int *)(DAT_0015ecb4 + 0x15ea38);
        if (*(char *)(iVar2 + 0x1d) == '\0') {
          pSVar14 = *(String **)(this + 0x20);
          GameText::getText(**(int **)(DAT_0015ecb8 + 0x15ea4a));
          ChoiceWindow::set(pSVar14);
          *(undefined1 *)(iVar2 + 0x1d) = 1;
          this[0x3c] = (HangarWindow)0x1;
        }
        uVar10 = Item::getStationAmount(*(Item **)(param_1 + 0x10));
        *(undefined4 *)(this + 0x8c) = uVar10;
        uVar10 = Item::getAmount();
        *(undefined4 *)(this + 0xa0) = uVar10;
        uVar10 = Status::getCredits();
        *(undefined4 *)(this + 0x98) = uVar10;
        *(undefined4 *)(this + 0x9c) = *(undefined4 *)(this + 0xa8);
      }
      else {
        iVar2 = ::ListItem::isItem(param_1);
        if (((iVar2 != 0) && (iVar2 = Item::getType(*(Item **)(param_1 + 0x10)), iVar2 != 4)) &&
           (iVar2 = *(int *)(DAT_0015e93c + 0x15e732), *(char *)(iVar2 + 0x1e) == '\0')) {
          pSVar14 = *(String **)(this + 0x20);
          GameText::getText(**(int **)(DAT_0015e940 + 0x15e744));
          ChoiceWindow::set(pSVar14);
          *(undefined1 *)(iVar2 + 0x1e) = 1;
          this[0x3c] = (HangarWindow)0x1;
        }
        iVar2 = ::ListItem::getIndex(param_1);
        if ((0x83 < iVar2) && (iVar2 = ::ListItem::getIndex(param_1), iVar2 < 0x9a)) {
          iVar3 = *(int *)(**(int **)(DAT_0015e944 + 0x15e774) + 0xac);
          iVar2 = ::ListItem::getIndex(param_1);
          *(undefined1 *)(iVar2 + *(int *)(iVar3 + 4) + -0x84) = 1;
        }
        this[0xf8] = (HangarWindow)0x1;
        uVar10 = HangarList::getCurrentItemIndex(*(HangarList **)(this + 0x14));
        *(undefined4 *)(this + 0xfc) = uVar10;
        pSVar5 = (Ship *)Status::getShip();
        pAVar7 = (Array *)Item::extractItems(*(Array **)(this + 0x10),true);
        Ship::setCargo(pSVar5,pAVar7);
        this_00 = (Station *)Status::getStation();
        pAVar7 = (Array *)Item::extractItems(*(Array **)(this + 0x10),false);
        Station::setItems(this_00,pAVar7,false);
        if (*(Array **)(this + 0x10) != (Array *)0x0) {
          ArrayReleaseClasses<Item*>(*(Array **)(this + 0x10));
          if (*(Array<Item*> **)(this + 0x10) != (Array<Item*> *)0x0) {
            pvVar8 = (void *)Array<Item*>::~Array(*(Array<Item*> **)(this + 0x10));
            operator_delete(pvVar8);
          }
        }
        *(undefined4 *)(this + 0x10) = 0;
        Status::getShip();
        pAVar7 = (Array *)Ship::getCargo();
        Status::getStation();
        pAVar9 = (Array *)Station::getItems();
        pAVar7 = (Array *)Item::mixItems(pAVar7,pAVar9);
        pHVar12 = *(HangarList **)(this + 0x14);
        *(Array **)(this + 0x10) = pAVar7;
        Status::getStation();
        pAVar9 = (Array *)Station::getShips();
        HangarList::initShopTab(pHVar12,pAVar7,pAVar9);
        pHVar12 = *(HangarList **)(this + 0x14);
        pSVar5 = (Ship *)Status::getShip();
        HangarList::initShipTab(pHVar12,pSVar5);
      }
    }
    goto LAB_0015ebbe;
  }
  iVar2 = Ship::getPrice(*(Ship **)(param_1 + 0xc));
  piVar17 = *(int **)(DAT_0015e934 + 0x15e6a6);
  iVar3 = Status::getCredits();
  pSVar5 = (Ship *)Status::getShip();
  iVar6 = Ship::getPrice(pSVar5);
  if ((iVar6 + iVar3 < iVar2) && (this[0x11d] == (HangarWindow)0x0)) {
    piVar15 = *(int **)(DAT_0015ec9c + 0x15e960);
    iVar2 = *piVar17;
    pSVar14 = (String *)GameText::getText(*piVar15);
    AbyssEngine::String::String(aSStack_94,pSVar14,false);
    ::ListItem::getPrice(param_1);
    Status::getCredits();
    pSVar5 = (Ship *)Status::getShip();
    Ship::getPrice(pSVar5);
    Layout::formatCredits((int)aSStack_40);
    AbyssEngine::String::String(aSStack_a0,aSStack_40,false);
    uVar10 = AbyssEngine::String::String(aSStack_ac,(char *)(DAT_0015eca4 + 0x15e9be),false);
    Status::replaceHash(aSStack_34,iVar2,aSStack_94,aSStack_a0,uVar10);
    pcVar13 = *(code **)(DAT_0015eca8 + 0x15e9d8);
    (*pcVar13)(aSStack_ac);
    (*pcVar13)(aSStack_a0);
    (*pcVar13)(aSStack_40);
    (*pcVar13)(aSStack_94);
    AbyssEngine::String::String(aSStack_b8,(char *)(DAT_0015ecac + 0x15e9ee),false);
    GameText::getText(*piVar15);
    AbyssEngine::operator+(aSStack_40,aSStack_b8);
    AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
    AbyssEngine::String::~String(aSStack_40);
    AbyssEngine::String::~String(aSStack_b8);
    ChoiceWindow::set(*(ChoiceWindow **)(this + 0x20),aSStack_34,true);
    this[0x3c] = (HangarWindow)0x1;
    this[0xaf] = (HangarWindow)0x1;
LAB_0015ebb8:
    AbyssEngine::String::~String(aSStack_34);
    goto LAB_0015ebbe;
  }
  if (*(int *)(*piVar17 + 0x34) < 1) {
    iVar2 = Status::getCurrentCampaignMission();
    if (iVar2 == 0x4d) {
      Status::getShip();
      iVar2 = Ship::getIndex();
      if (iVar2 == 0x25) {
        pSVar14 = *(String **)(this + 0x20);
        puVar4 = (undefined4 *)(DAT_0015e950 + 0x15e8a2);
        goto LAB_0015e6e4;
      }
    }
    Status::getShip();
    iVar2 = Ship::getIndex();
    iVar3 = Ship::getIndex();
    if (iVar2 != iVar3) {
      this_01 = *(ChoiceWindow **)(this + 0x20);
      puVar4 = (undefined4 *)(DAT_0015ec98 + 0x15eab0);
      this[0x90] = (HangarWindow)0x1;
      pSVar14 = (String *)GameText::getText(*(int *)*puVar4);
      ChoiceWindow::set(this_01,pSVar14,true);
      this[0x3c] = (HangarWindow)0x1;
      goto LAB_0015ebbe;
    }
    pSVar14 = *(String **)(this + 0x20);
    puVar4 = (undefined4 *)(&UNK_0015e922 + DAT_0015ecb0);
  }
  else {
    pSVar14 = *(String **)(this + 0x20);
    puVar4 = (undefined4 *)(DAT_0015e938 + 0x15e6e6);
  }
LAB_0015e6e4:
  GameText::getText(*(int *)*puVar4);
  ChoiceWindow::set(pSVar14);
  this[0x3c] = (HangarWindow)0x1;
LAB_0015ebbe:
  iVar2 = *piVar18 - local_28;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  0015e570: push {r4,r5,r6,r7,lr}
  0015e572: add r7,sp,#0xc
  0015e574: push {r8,r9,r10,r11}
  0015e578: sub sp,#0x9c
  0015e57a: mov r4,r0
  0015e57c: ldr r0,[0x0015e924]
  0015e57e: mov r5,r1
  0015e580: cmp r1,#0x0
  0015e582: add r0,pc
  0015e584: ldr.w r10,[r0,#0x0]
  0015e588: ldr.w r0,[r10,#0x0]
  0015e58c: str r0,[sp,#0x98]
  0015e58e: str r1,[r4,#0x68]
  0015e590: beq 0x0015e5a2
  0015e592: mov r0,r5
  0015e594: blx 0x00076204
  0015e598: cbz r0,0x0015e5a2
  0015e59a: ldr r0,[r4,#0x68]
  0015e59c: ldr r0,[r0,#0xc]
  0015e59e: blx 0x000719f8
  0015e5a2: ldr r0,[r4,#0x14]
  0015e5a4: blx 0x00076f48
  0015e5a8: cmp r0,#0x2
  0015e5aa: beq 0x0015e63c
  0015e5ac: cmp r0,#0x1
  0015e5ae: beq 0x0015e684
  0015e5b0: cmp r0,#0x0
  0015e5b2: bne.w 0x0015ebbe
  0015e5b6: mov r0,r5
  0015e5b8: blx 0x00076ffc
  0015e5bc: cmp r0,#0x0
  0015e5be: beq.w 0x0015ebbe
  0015e5c2: ldr r0,[0x0015e928]
  0015e5c4: ldr.w r1,[r4,#0xb4]
  0015e5c8: add r0,pc
  0015e5ca: ldr r6,[r0,#0x0]
  0015e5cc: movs r0,#0x0
  0015e5ce: str.w r0,[r4,#0xbc]
  0015e5d2: str.w r1,[r4,#0xe4]
  0015e5d6: str.w r0,[r4,#0xb4]
  0015e5da: ldrb r0,[r6,#0x1f]
  0015e5dc: cbnz r0,0x0015e60a
  0015e5de: mov r0,r5
  0015e5e0: blx 0x00076210
  0015e5e4: cbz r0,0x0015e60a
  0015e5e6: ldr r0,[0x0015e92c]
  0015e5e8: movw r1,#0x24b
  0015e5ec: ldr.w r8,[r4,#0x20]
  0015e5f0: add r0,pc
  0015e5f2: ldr r0,[r0,#0x0]
  0015e5f4: ldr r0,[r0,#0x0]
  0015e5f6: blx 0x00072f70
  0015e5fa: mov r1,r0
  0015e5fc: mov r0,r8
  0015e5fe: blx 0x00074794
  0015e602: movs r0,#0x1
  0015e604: strb r0,[r6,#0x1f]
  0015e606: strb.w r0,[r4,#0x3c]
  0015e60a: mov r0,r5
  0015e60c: blx 0x00076ffc
  0015e610: cmp r0,#0x0
  0015e612: beq.w 0x0015ebbe
  0015e616: ldr r0,[r4,#0x14]
  0015e618: blx 0x000770bc
  0015e61c: ldr r6,[r0,#0x10]
  0015e61e: cmp r6,#0x0
  0015e620: beq.w 0x0015e866
  0015e624: mov r0,r6
  0015e626: blx 0x00074ad0
  0015e62a: cmp r0,#0x0
  0015e62c: beq.w 0x0015e842
  0015e630: ldr r0,[0x0015e930]
  0015e632: movw r1,#0x143
  0015e636: ldr r5,[r4,#0x20]
  0015e638: add r0,pc
  0015e63a: b 0x0015e6e4
  0015e63c: mov r0,r5
  0015e63e: blx 0x00076ffc
  0015e642: cmp r0,#0x0
  0015e644: beq.w 0x0015ebbe
  0015e648: mov r0,r5
  0015e64a: blx 0x00076900
  0015e64e: cmp r0,#0x0
  0015e650: bne.w 0x0015ebbe
  0015e654: ldr r1,[r5,#0x8]
  0015e656: movs r2,#0x0
  0015e658: ldr r0,[r4,#0x14]
  0015e65a: movs r5,#0x0
  0015e65c: str.w r1,[r4,#0x80]
  0015e660: blx 0x00077248
  0015e664: ldr r0,[r4,#0x14]
  0015e666: movs r1,#0x4
  0015e668: movs r2,#0x1
  0015e66a: blx 0x00076f24
  0015e66e: mov r0,r4
  0015e670: blx 0x00076f30
  0015e674: ldrb.w r0,[r4,#0x89]
  0015e678: cmp r0,#0x0
  0015e67a: beq.w 0x0015ebbe
  0015e67e: strb.w r5,[r4,#0x89]
  0015e682: b 0x0015ebbe
  0015e684: mov r0,r5
  0015e686: blx 0x00076ffc
  0015e68a: cmp r0,#0x0
  0015e68c: beq.w 0x0015ebbe
  0015e690: mov r0,r5
  0015e692: blx 0x00076204
  0015e696: cbz r0,0x0015e6fc
  0015e698: ldr r0,[r5,#0xc]
  0015e69a: blx 0x00071b00
  0015e69e: mov r8,r0
  0015e6a0: ldr r0,[0x0015e934]
  0015e6a2: add r0,pc
  0015e6a4: ldr.w r11,[r0,#0x0]
  0015e6a8: ldr.w r0,[r11,#0x0]
  0015e6ac: blx 0x000733d8
  0015e6b0: mov r6,r0
  0015e6b2: ldr.w r0,[r11,#0x0]
  0015e6b6: blx 0x00071a58
  0015e6ba: blx 0x00071b00
  0015e6be: add r0,r6
  0015e6c0: cmp r8,r0
  0015e6c2: ble 0x0015e6ce
  0015e6c4: ldrb.w r0,[r4,#0x11d]
  0015e6c8: cmp r0,#0x0
  0015e6ca: beq.w 0x0015e958
  0015e6ce: ldr.w r0,[r11,#0x0]
  0015e6d2: ldr r1,[r0,#0x34]
  0015e6d4: cmp r1,#0x1
  0015e6d6: blt.w 0x0015e87e
  0015e6da: ldr r0,[0x0015e938]
  0015e6dc: mov.w r1,#0x150
  0015e6e0: ldr r5,[r4,#0x20]
  0015e6e2: add r0,pc
  0015e6e4: ldr r0,[r0,#0x0]
  0015e6e6: ldr r0,[r0,#0x0]
  0015e6e8: blx 0x00072f70
  0015e6ec: mov r1,r0
  0015e6ee: mov r0,r5
  0015e6f0: blx 0x00074794
  0015e6f4: movs r0,#0x1
  0015e6f6: strb.w r0,[r4,#0x3c]
  0015e6fa: b 0x0015ebbe
  0015e6fc: ldr r0,[r5,#0x10]
  0015e6fe: blx 0x00074ad0
  0015e702: cmp r0,#0x0
  0015e704: bne.w 0x0015ebbe
  0015e708: ldrb.w r0,[r4,#0x88]
  0015e70c: cmp r0,#0x0
  0015e70e: eor r1,r0,#0x1
  0015e712: strb.w r1,[r4,#0x88]
  0015e716: beq.w 0x0015ea32
  0015e71a: mov r0,r5
  0015e71c: blx 0x000768e8
  0015e720: cbz r0,0x0015e75a
  0015e722: ldr r0,[r5,#0x10]
  0015e724: blx 0x000718fc
  0015e728: cmp r0,#0x4
  0015e72a: beq 0x0015e75a
  0015e72c: ldr r0,[0x0015e93c]
  0015e72e: add r0,pc
  0015e730: ldr r6,[r0,#0x0]
  0015e732: ldrb r0,[r6,#0x1e]
  0015e734: cbnz r0,0x0015e75a
  0015e736: ldr r0,[0x0015e940]
  0015e738: movw r1,#0x24d
  0015e73c: ldr.w r8,[r4,#0x20]
  0015e740: add r0,pc
  0015e742: ldr r0,[r0,#0x0]
  0015e744: ldr r0,[r0,#0x0]
  0015e746: blx 0x00072f70
  0015e74a: mov r1,r0
  0015e74c: mov r0,r8
  0015e74e: blx 0x00074794
  0015e752: movs r0,#0x1
  0015e754: strb r0,[r6,#0x1e]
  0015e756: strb.w r0,[r4,#0x3c]
  0015e75a: mov r0,r5
  0015e75c: blx 0x000768dc
  0015e760: cmp r0,#0x84
  0015e762: blt 0x0015e78a
  0015e764: mov r0,r5
  0015e766: blx 0x000768dc
  0015e76a: cmp r0,#0x99
  0015e76c: bgt 0x0015e78a
  0015e76e: ldr r0,[0x0015e944]
  0015e770: add r0,pc
  0015e772: ldr r0,[r0,#0x0]
  0015e774: ldr r0,[r0,#0x0]
  0015e776: ldr.w r6,[r0,#0xac]
  0015e77a: mov r0,r5
  0015e77c: blx 0x000768dc
  0015e780: ldr r1,[r6,#0x4]
  0015e782: add r0,r1
  0015e784: movs r1,#0x1
  0015e786: strb.w r1,[r0,#-0x84]
  0015e78a: ldr r0,[r4,#0x14]
  0015e78c: movs r1,#0x1
  0015e78e: strb.w r1,[r4,#0xf8]
  0015e792: blx 0x000770e0
  0015e796: ldr r1,[0x0015e948]
  0015e798: str.w r0,[r4,#0xfc]
  0015e79c: add r1,pc
  0015e79e: ldr.w r8,[r1,#0x0]
  0015e7a2: ldr.w r0,[r8,#0x0]
  0015e7a6: blx 0x00071a58
  0015e7aa: mov r5,r0
  0015e7ac: ldr r0,[r4,#0x10]
  0015e7ae: movs r1,#0x1
  0015e7b0: blx 0x000770d4
  0015e7b4: mov r1,r0
  0015e7b6: mov r0,r5
  0015e7b8: blx 0x000738b8
  0015e7bc: ldr.w r0,[r8,#0x0]
  0015e7c0: blx 0x00071c14
  0015e7c4: mov r5,r0
  0015e7c6: ldr r0,[r4,#0x10]
  0015e7c8: movs r1,#0x0
  0015e7ca: movs r6,#0x0
  0015e7cc: blx 0x000770d4
  0015e7d0: mov r1,r0
  0015e7d2: mov r0,r5
  0015e7d4: movs r2,#0x0
  0015e7d6: blx 0x000736e4
  0015e7da: ldr r0,[r4,#0x10]
  0015e7dc: cbz r0,0x0015e7ee
  0015e7de: blx 0x00073420
  0015e7e2: ldr r0,[r4,#0x10]
  0015e7e4: cbz r0,0x0015e7ee
  0015e7e6: blx 0x0007342c
  0015e7ea: blx 0x0006eb48
  0015e7ee: str r6,[r4,#0x10]
  0015e7f0: ldr.w r0,[r8,#0x0]
  0015e7f4: blx 0x00071a58
  0015e7f8: blx 0x00073750
  0015e7fc: mov r5,r0
  0015e7fe: ldr.w r0,[r8,#0x0]
  0015e802: blx 0x00071c14
  0015e806: blx 0x00071830
  0015e80a: mov r1,r0
  0015e80c: mov r0,r5
  0015e80e: blx 0x00076ee8
  0015e812: ldr r6,[r4,#0x14]
  0015e814: mov r5,r0
  0015e816: str r0,[r4,#0x10]
  0015e818: ldr.w r0,[r8,#0x0]
  0015e81c: blx 0x00071c14
  0015e820: blx 0x00073708
  0015e824: mov r2,r0
  0015e826: mov r0,r6
  0015e828: mov r1,r5
  0015e82a: blx 0x000761ec
  0015e82e: ldr.w r0,[r8,#0x0]
  0015e832: ldr r4,[r4,#0x14]
  0015e834: blx 0x00071a58
  0015e838: mov r1,r0
  0015e83a: mov r0,r4
  0015e83c: blx 0x000761e0
  0015e840: b 0x0015ebbe
  0015e842: mov r0,r6
  0015e844: blx 0x00071908
  0015e848: cmp r0,#0x14
  0015e84a: bne 0x0015e866
  0015e84c: ldr r0,[0x0015e94c]
  0015e84e: add r0,pc
  0015e850: ldr.w r9,[r0,#0x0]
  0015e854: ldr.w r0,[r9,#0x0]
  0015e858: ldr r1,[r0,#0x34]
  0015e85a: cmp r1,#0x1
  0015e85c: itt ge
  0015e85e: ldr.ge r1,[r5,#0x3c]
  0015e860: cmp.ge r1,#0x0
  0015e862: bge.w 0x0015ebd6
  0015e866: ldr r0,[r5,#0x3c]
  0015e868: cmp r0,#0x0
  0015e86a: blt 0x0015e8a2
  0015e86c: ldr r0,[r4,#0x14]
  0015e86e: blx 0x000770bc
  0015e872: ldr r2,[r0,#0x40]
  0015e874: mov r0,r4
  0015e876: mov r1,r6
  0015e878: blx 0x00077140
  0015e87c: b 0x0015ebbe
  0015e87e: blx 0x00071770
  0015e882: cmp r0,#0x4d
  0015e884: bne 0x0015e8fc
  0015e886: ldr.w r0,[r11,#0x0]
  0015e88a: blx 0x00071a58
  0015e88e: blx 0x000719c8
  0015e892: cmp r0,#0x25
  0015e894: bne 0x0015e8fc
  0015e896: ldr r0,[0x0015e950]
  0015e898: movw r1,#0x145
  0015e89c: ldr r5,[r4,#0x20]
  0015e89e: add r0,pc
  0015e8a0: b 0x0015e6e4
  0015e8a2: ldr r0,[0x0015e954]
  0015e8a4: add r0,pc
  0015e8a6: ldr.w r8,[r0,#0x0]
  0015e8aa: ldr.w r0,[r8,#0x0]
  0015e8ae: blx 0x00071a58
  0015e8b2: mov r6,r0
  0015e8b4: ldr r0,[r5,#0x10]
  0015e8b6: blx 0x00071908
  0015e8ba: mov r1,r0
  0015e8bc: mov r0,r6
  0015e8be: blx 0x0007228c
  0015e8c2: mov r9,r0
  0015e8c4: ldr r0,[r5,#0x10]
  0015e8c6: blx 0x00071908
  0015e8ca: cmp r0,#0x15
  0015e8cc: bne.w 0x0015ea8a
  0015e8d0: ldr.w r0,[r8,#0x0]
  0015e8d4: blx 0x00071a58
  0015e8d8: blx 0x000719c8
  0015e8dc: cmp r0,#0x2c
  0015e8de: beq.w 0x0015ea98
  0015e8e2: ldr.w r0,[r8,#0x0]
  0015e8e6: blx 0x00071a58
  0015e8ea: blx 0x000719c8
  0015e8ee: cmp.w r9,#0x0
  0015e8f2: it ne
  0015e8f4: cmp.ne r0,#0x31
  0015e8f6: bne.w 0x0015ea90
  0015e8fa: b 0x0015ea98
  0015e8fc: ldr.w r0,[r11,#0x0]
  0015e900: blx 0x00071a58
  0015e904: blx 0x000719c8
  0015e908: mov r6,r0
  0015e90a: ldr r0,[r5,#0xc]
  0015e90c: blx 0x000719c8
  0015e910: cmp r6,r0
  0015e912: bne.w 0x0015eaa2
  0015e916: ldr r0,[0x0015ecb0]
  0015e918: movw r1,#0x149
  0015e91c: ldr r5,[r4,#0x20]
  0015e91e: add r0,pc
  0015e920: b 0x0015e6e4
  0015e958: ldr r0,[0x0015ec9c]
  0015e95a: movs r1,#0xcb
  0015e95c: add r0,pc
  0015e95e: ldr.w r8,[r0,#0x0]
  0015e962: ldr.w r0,[r11,#0x0]
  0015e966: str r0,[sp,#0x4]
  0015e968: ldr.w r0,[r8,#0x0]
  0015e96c: blx 0x00072f70
  0015e970: mov r1,r0
  0015e972: add r0,sp,#0x2c
  0015e974: movs r2,#0x0
  0015e976: blx 0x0006f028
  0015e97a: ldr r0,[0x0015eca0]
  0015e97c: add r0,pc
  0015e97e: ldr r0,[r0,#0x0]
  0015e980: ldr.w r9,[r0,#0x0]
  0015e984: mov r0,r5
  0015e986: blx 0x00076960
  0015e98a: mov r5,r0
  0015e98c: ldr.w r0,[r11,#0x0]
  0015e990: blx 0x000733d8
  0015e994: mov r6,r0
  0015e996: ldr.w r0,[r11,#0x0]
  0015e99a: blx 0x00071a58
  0015e99e: blx 0x00071b00
  0015e9a2: subs r1,r5,r6
  0015e9a4: subs r2,r1,r0
  0015e9a6: add r0,sp,#0x80
  0015e9a8: mov r1,r9
  0015e9aa: blx 0x00074e54
  0015e9ae: add r0,sp,#0x20
  0015e9b0: add r1,sp,#0x80
  0015e9b2: movs r2,#0x0
  0015e9b4: blx 0x0006f028
  0015e9b8: ldr r1,[0x0015eca4]
  0015e9ba: add r1,pc
  0015e9bc: add r0,sp,#0x14
  0015e9be: movs r2,#0x0
  0015e9c0: blx 0x0006ee18
  0015e9c4: ldr r1,[sp,#0x4]
  0015e9c6: add r2,sp,#0x2c
  0015e9c8: str r0,[sp,#0x0]
  0015e9ca: add r0,sp,#0x8c
  0015e9cc: add r3,sp,#0x20
  0015e9ce: blx 0x000739b4
  0015e9d2: ldr r0,[0x0015eca8]
  0015e9d4: add r0,pc
  0015e9d6: ldr r5,[r0,#0x0]
  0015e9d8: add r0,sp,#0x14
  0015e9da: blx r5
  0015e9dc: add r0,sp,#0x20
  0015e9de: blx r5
  0015e9e0: add r0,sp,#0x80
  0015e9e2: blx r5
  0015e9e4: add r0,sp,#0x2c
  0015e9e6: blx r5
  0015e9e8: ldr r1,[0x0015ecac]
  0015e9ea: add r1,pc
  0015e9ec: add r0,sp,#0x8
  0015e9ee: movs r2,#0x0
  0015e9f0: blx 0x0006ee18
  0015e9f4: ldr.w r0,[r8,#0x0]
  0015e9f8: movs r1,#0x7c
  0015e9fa: blx 0x00072f70
  0015e9fe: mov r2,r0
  0015ea00: add r0,sp,#0x80
  0015ea02: add r1,sp,#0x8
  0015ea04: blx 0x0006ef98
  0015ea08: add r0,sp,#0x8c
  0015ea0a: add r1,sp,#0x80
  0015ea0c: blx 0x0006ef5c
  0015ea10: add r0,sp,#0x80
  0015ea12: blx 0x0006ee30
  0015ea16: add r0,sp,#0x8
  0015ea18: blx 0x0006ee30
  0015ea1c: ldr r0,[r4,#0x20]
  0015ea1e: add r1,sp,#0x8c
  0015ea20: movs r2,#0x1
  0015ea22: movs r5,#0x1
  0015ea24: blx 0x000746f8
  0015ea28: strb.w r5,[r4,#0x3c]
  0015ea2c: strb.w r5,[r4,#0xaf]
  0015ea30: b 0x0015ebb8
  0015ea32: ldr r0,[0x0015ecb4]
  0015ea34: add r0,pc
  0015ea36: ldr r6,[r0,#0x0]
  0015ea38: ldrb r0,[r6,#0x1d]
  0015ea3a: cbnz r0,0x0015ea60
  0015ea3c: ldr r0,[0x0015ecb8]
  0015ea3e: mov.w r1,#0x24c
  0015ea42: ldr.w r8,[r4,#0x20]
  0015ea46: add r0,pc
  0015ea48: ldr r0,[r0,#0x0]
  0015ea4a: ldr r0,[r0,#0x0]
  0015ea4c: blx 0x00072f70
  0015ea50: mov r1,r0
  0015ea52: mov r0,r8
  0015ea54: blx 0x00074794
  0015ea58: movs r0,#0x1
  0015ea5a: strb r0,[r6,#0x1d]
  0015ea5c: strb.w r0,[r4,#0x3c]
  0015ea60: ldr r0,[r5,#0x10]
  0015ea62: blx 0x00077014
  0015ea66: str.w r0,[r4,#0x8c]
  0015ea6a: ldr r0,[r5,#0x10]
  0015ea6c: blx 0x0007183c
  0015ea70: ldr r1,[0x0015ecbc]
  0015ea72: str.w r0,[r4,#0xa0]
  0015ea76: add r1,pc
  0015ea78: ldr r1,[r1,#0x0]
  0015ea7a: ldr r0,[r1,#0x0]
  0015ea7c: blx 0x000733d8
  0015ea80: ldr.w r1,[r4,#0xa8]
  0015ea84: strd r0,r1,[r4,#0x98]
  0015ea88: b 0x0015ebbe
  0015ea8a: cmp.w r9,#0x0
  0015ea8e: beq 0x0015ea98
  0015ea90: ldr r0,[r5,#0x10]
  0015ea92: blx 0x0007702c
  0015ea96: cbz r0,0x0015ead6
  0015ea98: ldr r1,[r5,#0x10]
  0015ea9a: mov r0,r4
  0015ea9c: blx 0x0007714c
  0015eaa0: b 0x0015ebbe
  0015eaa2: ldr r0,[0x0015ec98]
  0015eaa4: movs r6,#0x1
  0015eaa6: ldr r5,[r4,#0x20]
  0015eaa8: movw r1,#0x14d
  0015eaac: add r0,pc
  0015eaae: strb.w r6,[r4,#0x90]
  0015eab2: ldrb.w r2,[r4,#0x11d]
  0015eab6: ldr r0,[r0,#0x0]
  0015eab8: cmp r2,#0x0
  0015eaba: ldr r0,[r0,#0x0]
  0015eabc: it eq
  0015eabe: mov.eq.w r1,#0x130
  0015eac2: blx 0x00072f70
  0015eac6: mov r1,r0
  0015eac8: mov r0,r5
  0015eaca: movs r2,#0x1
  0015eacc: blx 0x000746f8
  0015ead0: strb.w r6,[r4,#0x3c]
  0015ead4: b 0x0015ebbe
  0015ead6: ldr r0,[0x0015ec88]
  0015ead8: movw r1,#0x11f
  0015eadc: add r0,pc
  0015eade: ldr.w r11,[r0,#0x0]
  0015eae2: ldr.w r0,[r11,#0x0]
  0015eae6: blx 0x00072f70
  0015eaea: add r6,sp,#0x8c
  0015eaec: mov r1,r0
  0015eaee: movs r2,#0x0
  0015eaf0: mov r0,r6
  0015eaf2: blx 0x0006f028
  0015eaf6: ldr.w r8,[r8,#0x0]
  0015eafa: add r0,sp,#0x68
  0015eafc: mov r1,r6
  0015eafe: movs r2,#0x0
  0015eb00: blx 0x0006f028
  0015eb04: ldr.w r6,[r11,#0x0]
  0015eb08: mov r0,r9
  0015eb0a: blx 0x000718d8
  0015eb0e: addw r1,r0,#0x4fa
  0015eb12: mov r0,r6
  0015eb14: blx 0x00072f70
  0015eb18: mov r1,r0
  0015eb1a: add r0,sp,#0x5c
  0015eb1c: movs r2,#0x0
  0015eb1e: blx 0x0006f028
  0015eb22: ldr r1,[0x0015ec8c]
  0015eb24: add r1,pc
  0015eb26: add r0,sp,#0x50
  0015eb28: movs r2,#0x0
  0015eb2a: blx 0x0006ee18
  0015eb2e: str r0,[sp,#0x0]
  0015eb30: add r0,sp,#0x74
  0015eb32: add r2,sp,#0x68
  0015eb34: add r3,sp,#0x5c
  0015eb36: mov r1,r8
  0015eb38: blx 0x000739b4
  0015eb3c: ldr.w r6,[r11,#0x0]
  0015eb40: ldr r0,[r5,#0x10]
  0015eb42: blx 0x000718d8
  0015eb46: addw r1,r0,#0x4fa
  0015eb4a: mov r0,r6
  0015eb4c: blx 0x00072f70
  0015eb50: mov r1,r0
  0015eb52: add r0,sp,#0x44
  0015eb54: movs r2,#0x0
  0015eb56: blx 0x0006f028
  0015eb5a: ldr r1,[0x0015ec90]
  0015eb5c: add r1,pc
  0015eb5e: add r0,sp,#0x38
  0015eb60: movs r2,#0x0
  0015eb62: blx 0x0006ee18
  0015eb66: str r0,[sp,#0x0]
  0015eb68: add r0,sp,#0x80
  0015eb6a: add r2,sp,#0x74
  0015eb6c: add r3,sp,#0x44
  0015eb6e: mov r1,r8
  0015eb70: blx 0x000739b4
  0015eb74: add r0,sp,#0x8c
  0015eb76: add r1,sp,#0x80
  0015eb78: blx 0x0006f2b0
  0015eb7c: ldr r0,[0x0015ec94]
  0015eb7e: add r0,pc
  0015eb80: ldr r6,[r0,#0x0]
  0015eb82: add r0,sp,#0x80
  0015eb84: blx r6
  0015eb86: add r0,sp,#0x38
  0015eb88: blx r6
  0015eb8a: add r0,sp,#0x44
  0015eb8c: blx r6
  0015eb8e: add r0,sp,#0x74
  0015eb90: blx r6
  0015eb92: add r0,sp,#0x50
  0015eb94: blx r6
  0015eb96: add r0,sp,#0x5c
  0015eb98: blx r6
  0015eb9a: add r0,sp,#0x68
  0015eb9c: blx r6
  0015eb9e: ldr r0,[r4,#0x20]
  0015eba0: add r1,sp,#0x8c
  0015eba2: movs r2,#0x1
  0015eba4: movs r6,#0x1
  0015eba6: blx 0x000746f8
  0015ebaa: strb.w r6,[r4,#0x11c]
  0015ebae: strb.w r6,[r4,#0x3c]
  0015ebb2: ldr r0,[r5,#0x10]
  0015ebb4: strd r0,r9,[r4,#0x28]
  0015ebb8: add r0,sp,#0x8c
  0015ebba: blx 0x0006ee30
  0015ebbe: ldr r0,[sp,#0x98]
  0015ebc0: ldr.w r1,[r10,#0x0]
  0015ebc4: subs r0,r1,r0
  0015ebc6: itttt eq
  0015ebc8: mov.eq r0,#0x0
  0015ebca: add.eq sp,#0x9c
  0015ebcc: pop.eq.w {r8,r9,r10,r11}
  0015ebd0: pop.eq {r4,r5,r6,r7,pc}
  0015ebd2: blx 0x0006e824
  0015ebd6: blx 0x00071a58
  0015ebda: blx 0x00072bd4
  0015ebde: mov r8,r0
  0015ebe0: mov r0,r6
  0015ebe2: movs r1,#0x22
  0015ebe4: blx 0x000718e4
  0015ebe8: ldr.w r1,[r9,#0x0]
  0015ebec: sub.w r0,r8,r0
  0015ebf0: ldr r1,[r1,#0x34]
  0015ebf2: cmp r0,r1
  0015ebf4: blt.w 0x0015e630
  0015ebf8: b 0x0015e866
```
