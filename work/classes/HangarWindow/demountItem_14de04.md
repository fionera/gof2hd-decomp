# HangarWindow::demountItem
elf 0x14de04 body 388
Sig: undefined __thiscall demountItem(HangarWindow * this, Item * param_1, int param_2)

## decompile
```c

/* HangarWindow::demountItem(Item*, int) */

void __thiscall HangarWindow::demountItem(HangarWindow *this,Item *param_1,int param_2)

{
  int iVar1;
  Item *this_00;
  Item *pIVar2;
  Ship *pSVar3;
  int iVar4;
  void *pvVar5;
  Array *pAVar6;
  undefined4 uVar7;
  Array *pAVar8;
  HangarList *pHVar9;
  uint uVar10;
  code *pcVar11;
  code *pcVar12;
  undefined4 *puVar13;
  float fVar14;
  
  iVar1 = Item::getType(param_1);
  if (iVar1 == 1) {
    Item::getAmount();
  }
  this_00 = (Item *)Item::makeItem((int)param_1);
  puVar13 = *(undefined4 **)(DAT_0015df88 + 0x15de36);
  pIVar2 = (Item *)Status::getShip();
  Ship::addCargo(pIVar2);
  pSVar3 = (Ship *)Status::getShip();
  if (param_2 < 0) {
    Ship::freeSlot(pSVar3,param_1);
  }
  else {
    Ship::freeSlot(pSVar3,param_1,param_2);
  }
  uVar10 = 0;
  do {
    pAVar8 = *(Array **)(this + 0x10);
    if (*(uint *)pAVar8 <= uVar10) {
      ArrayAdd<Item*>(this_00,pAVar8);
      goto LAB_0015de9e;
    }
    pIVar2 = *(Item **)(*(int *)(pAVar8 + 4) + uVar10 * 4);
    iVar1 = Item::getIndex(pIVar2);
    iVar4 = Item::getIndex(this_00);
    uVar10 = uVar10 + 1;
  } while (iVar1 != iVar4);
  iVar1 = Item::getAmount();
  Item::changeAmount(pIVar2,iVar1);
LAB_0015de9e:
  pSVar3 = (Ship *)Status::getShip();
  pAVar8 = (Array *)Item::extractItems(*(Array **)(this + 0x10),true);
  Ship::setCargo(pSVar3,pAVar8);
  if (*(Array **)(this + 0x10) != (Array *)0x0) {
    ArrayReleaseClasses<Item*>(*(Array **)(this + 0x10));
    if (*(Array<Item*> **)(this + 0x10) != (Array<Item*> *)0x0) {
      pvVar5 = (void *)Array<Item*>::~Array(*(Array<Item*> **)(this + 0x10));
      operator_delete(pvVar5);
    }
  }
  iVar1 = DAT_0015df8c;
  *(undefined4 *)(this + 0x10) = 0;
  pcVar11 = *(code **)(iVar1 + 0x15dee0);
  (*pcVar11)(*puVar13);
  pAVar8 = (Array *)Ship::getCargo();
  pcVar12 = *(code **)(DAT_0015df90 + 0x15def6);
  (*pcVar12)(*puVar13);
  pAVar6 = (Array *)Station::getItems();
  uVar7 = Item::mixItems(pAVar8,pAVar6);
  pHVar9 = *(HangarList **)(this + 0x14);
  *(undefined4 *)(this + 0x10) = uVar7;
  pSVar3 = (Ship *)(*pcVar11)(*puVar13);
  HangarList::initShipTab(pHVar9,pSVar3);
  pHVar9 = *(HangarList **)(this + 0x14);
  (*pcVar11)(*puVar13);
  pAVar8 = (Array *)Ship::getCargo();
  (*pcVar12)(*puVar13);
  pAVar6 = (Array *)Station::getItems();
  pAVar8 = (Array *)Item::mixItems(pAVar8,pAVar6);
  (*pcVar12)(*puVar13);
  pAVar6 = (Array *)Station::getShips();
  HangarList::initShopTab(pHVar9,pAVar8,pAVar6);
  HangarList::setCurrentTab(*(int *)(this + 0x14),false);
  fVar14 = (float)refreshCurrentContentHeight(this);
  puVar13 = (undefined4 *)(DAT_0015df94 + 0x15df72);
  *(undefined4 *)(this + 0xb4) = *(undefined4 *)(this + 0xe4);
  FModSound::play(*(int *)*puVar13,(Vector *)0x60,(Vector *)0x0,fVar14);
  return;
}

```

## target disasm
```
  0015de04: push {r4,r5,r6,r7,lr}
  0015de06: add r7,sp,#0xc
  0015de08: push {r7,r8,r9,r10,r11}
  0015de0c: mov r9,r0
  0015de0e: mov r0,r1
  0015de10: mov r8,r2
  0015de12: mov r6,r1
  0015de14: blx 0x000718fc
  0015de18: cmp r0,#0x1
  0015de1a: bne 0x0015de26
  0015de1c: mov r0,r6
  0015de1e: blx 0x0007183c
  0015de22: mov r1,r0
  0015de24: b 0x0015de28
  0015de26: movs r1,#0x1
  0015de28: mov r0,r6
  0015de2a: blx 0x000718c0
  0015de2e: mov r11,r0
  0015de30: ldr r0,[0x0015df88]
  0015de32: add r0,pc
  0015de34: ldr.w r10,[r0,#0x0]
  0015de38: ldr.w r0,[r10,#0x0]
  0015de3c: blx 0x00071a58
  0015de40: mov r1,r11
  0015de42: blx 0x00072cdc
  0015de46: ldr.w r0,[r10,#0x0]
  0015de4a: blx 0x00071a58
  0015de4e: mov r1,r6
  0015de50: cmp.w r8,#0x0
  0015de54: blt 0x0015de5e
  0015de56: mov r2,r8
  0015de58: blx 0x00077224
  0015de5c: b 0x0015de62
  0015de5e: blx 0x00077230
  0015de62: movs r5,#0x0
  0015de64: ldr.w r1,[r9,#0x10]
  0015de68: ldr r0,[r1,#0x0]
  0015de6a: cmp r5,r0
  0015de6c: bcs 0x0015de98
  0015de6e: ldr r0,[r1,#0x4]
  0015de70: ldr.w r6,[r0,r5,lsl #0x2]
  0015de74: mov r0,r6
  0015de76: blx 0x000718d8
  0015de7a: mov r4,r0
  0015de7c: mov r0,r11
  0015de7e: blx 0x000718d8
  0015de82: adds r5,#0x1
  0015de84: cmp r4,r0
  0015de86: bne 0x0015de64
  0015de88: mov r0,r11
  0015de8a: blx 0x0007183c
  0015de8e: mov r1,r0
  0015de90: mov r0,r6
  0015de92: blx 0x00071854
  0015de96: b 0x0015de9e
  0015de98: mov r0,r11
  0015de9a: blx 0x000718b4
  0015de9e: ldr.w r0,[r10,#0x0]
  0015dea2: blx 0x00071a58
  0015dea6: mov r4,r0
  0015dea8: ldr.w r0,[r9,#0x10]
  0015deac: movs r1,#0x1
  0015deae: blx 0x000770d4
  0015deb2: mov r1,r0
  0015deb4: mov r0,r4
  0015deb6: blx 0x000738b8
  0015deba: ldr.w r0,[r9,#0x10]
  0015debe: cbz r0,0x0015ded2
  0015dec0: blx 0x00073420
  0015dec4: ldr.w r0,[r9,#0x10]
  0015dec8: cbz r0,0x0015ded2
  0015deca: blx 0x0007342c
  0015dece: blx 0x0006eb48
  0015ded2: ldr r1,[0x0015df8c]
  0015ded4: mov.w r8,#0x0
  0015ded8: str.w r8,[r9,#0x10]
  0015dedc: add r1,pc
  0015dede: ldr.w r0,[r10,#0x0]
  0015dee2: ldr r5,[r1,#0x0]
  0015dee4: blx r5
  0015dee6: blx 0x00073750
  0015deea: ldr r1,[0x0015df90]
  0015deec: mov r4,r0
  0015deee: ldr.w r0,[r10,#0x0]
  0015def2: add r1,pc
  0015def4: ldr r6,[r1,#0x0]
  0015def6: blx r6
  0015def8: blx 0x00071830
  0015defc: mov r1,r0
  0015defe: mov r0,r4
  0015df00: blx 0x00076ee8
  0015df04: ldr.w r4,[r9,#0x14]
  0015df08: str.w r0,[r9,#0x10]
  0015df0c: ldr.w r0,[r10,#0x0]
  0015df10: blx r5
  0015df12: mov r1,r0
  0015df14: mov r0,r4
  0015df16: blx 0x000761e0
  0015df1a: ldr.w r0,[r10,#0x0]
  0015df1e: ldr.w r11,[r9,#0x14]
  0015df22: blx r5
  0015df24: blx 0x00073750
  0015df28: mov r4,r0
  0015df2a: ldr.w r0,[r10,#0x0]
  0015df2e: blx r6
  0015df30: blx 0x00071830
  0015df34: mov r1,r0
  0015df36: mov r0,r4
  0015df38: blx 0x00076ee8
  0015df3c: mov r4,r0
  0015df3e: ldr.w r0,[r10,#0x0]
  0015df42: blx r6
  0015df44: blx 0x00073708
  0015df48: mov r2,r0
  0015df4a: mov r0,r11
  0015df4c: mov r1,r4
  0015df4e: blx 0x000761ec
  0015df52: ldr.w r0,[r9,#0x14]
  0015df56: movs r1,#0x0
  0015df58: movs r2,#0x1
  0015df5a: blx 0x00076f24
  0015df5e: mov r0,r9
  0015df60: blx 0x00076f30
  0015df64: ldr r0,[0x0015df94]
  0015df66: movs r2,#0x0
  0015df68: ldr.w r1,[r9,#0xe4]
  0015df6c: movs r3,#0x0
  0015df6e: add r0,pc
  0015df70: str.w r1,[r9,#0xb4]
  0015df74: movs r1,#0x60
  0015df76: str.w r8,[sp,#0x0]
  0015df7a: ldr r0,[r0,#0x0]
  0015df7c: ldr r0,[r0,#0x0]
  0015df7e: blx 0x00071548
  0015df82: pop.w {r3,r8,r9,r10,r11}
  0015df86: pop {r4,r5,r6,r7,pc}
```
