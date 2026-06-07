# HangarWindow::mountItem
elf 0x14df98 body 372
Sig: undefined __thiscall mountItem(HangarWindow * this, Item * param_1)

## decompile
```c

/* HangarWindow::mountItem(Item*) */

void __thiscall HangarWindow::mountItem(HangarWindow *this,Item *param_1)

{
  int iVar1;
  Item *pIVar2;
  Ship *pSVar3;
  int iVar4;
  int iVar5;
  uint *puVar6;
  Array *pAVar7;
  Array *pAVar8;
  undefined4 *puVar9;
  HangarList *pHVar10;
  uint uVar11;
  code *pcVar12;
  float fVar13;
  
  iVar1 = Item::getType(param_1);
  if (iVar1 == 1) {
    Item::getAmount();
  }
  pIVar2 = (Item *)Item::makeItem((int)param_1);
  puVar9 = *(undefined4 **)(DAT_0015e10c + 0x15dfca);
  pSVar3 = (Ship *)Status::getShip();
  Ship::addEquipment(pSVar3,pIVar2);
  iVar4 = Status::getShip();
  iVar5 = Item::getIndex(pIVar2);
  if (iVar1 == 1) {
    Item::getAmount();
  }
  Ship::removeCargo(iVar4,iVar5);
  puVar6 = *(uint **)(this + 0x10);
  if (puVar6 != (uint *)0x0) {
    for (uVar11 = 0; uVar11 < *puVar6; uVar11 = uVar11 + 1) {
      pIVar2 = *(Item **)(puVar6[1] + uVar11 * 4);
      iVar4 = Item::getIndex(pIVar2);
      iVar5 = Item::getIndex(param_1);
      if (iVar4 == iVar5) {
        iVar4 = Item::getStationAmount(pIVar2);
        if (iVar4 == 0) {
          if ((iVar1 == 1) || (iVar1 = Item::getAmount(), iVar1 == 1)) {
            ArrayRemove<Item*>(pIVar2,*(Array **)(this + 0x10));
            break;
          }
LAB_0015e066:
          iVar1 = -1;
        }
        else {
          if (iVar1 != 1) goto LAB_0015e066;
          iVar1 = Item::getAmount();
          iVar1 = -iVar1;
        }
        Item::changeAmount(pIVar2,iVar1);
        break;
      }
      puVar6 = *(uint **)(this + 0x10);
    }
  }
  pcVar12 = *(code **)(DAT_0015e110 + 0x15e078);
  pSVar3 = (Ship *)(*pcVar12)(*puVar9);
  pAVar7 = (Array *)Item::extractItems(*(Array **)(this + 0x10),true);
  Ship::setCargo(pSVar3,pAVar7);
  pHVar10 = *(HangarList **)(this + 0x14);
  pSVar3 = (Ship *)(*pcVar12)(*puVar9);
  HangarList::initShipTab(pHVar10,pSVar3);
  pHVar10 = *(HangarList **)(this + 0x14);
  (*pcVar12)(*puVar9);
  pAVar7 = (Array *)Ship::getCargo();
  Status::getStation();
  pAVar8 = (Array *)Station::getItems();
  pAVar7 = (Array *)Item::mixItems(pAVar7,pAVar8);
  Status::getStation();
  pAVar8 = (Array *)Station::getShips();
  HangarList::initShopTab(pHVar10,pAVar7,pAVar8);
  HangarList::setCurrentTab(*(int *)(this + 0x14),false);
  fVar13 = (float)refreshCurrentContentHeight(this);
  puVar9 = (undefined4 *)(DAT_0015e114 + 0x15e0f8);
  *(undefined4 *)(this + 0xb4) = *(undefined4 *)(this + 0xe4);
  FModSound::play(*(int *)*puVar9,(Vector *)0x62,(Vector *)0x0,fVar13);
  return;
}

```

## target disasm
```
  0015df98: push {r4,r5,r6,r7,lr}
  0015df9a: add r7,sp,#0xc
  0015df9c: push {r5,r6,r7,r8,r9,r10,r11}
  0015dfa0: mov r11,r0
  0015dfa2: mov r0,r1
  0015dfa4: mov r8,r1
  0015dfa6: blx 0x000718fc
  0015dfaa: mov r6,r0
  0015dfac: cmp r0,#0x1
  0015dfae: bne 0x0015dfba
  0015dfb0: mov r0,r8
  0015dfb2: blx 0x0007183c
  0015dfb6: mov r1,r0
  0015dfb8: b 0x0015dfbc
  0015dfba: movs r1,#0x1
  0015dfbc: mov r0,r8
  0015dfbe: blx 0x000718c0
  0015dfc2: mov r5,r0
  0015dfc4: ldr r0,[0x0015e10c]
  0015dfc6: add r0,pc
  0015dfc8: ldr r4,[r0,#0x0]
  0015dfca: ldr r0,[r4,#0x0]
  0015dfcc: blx 0x00071a58
  0015dfd0: mov r1,r5
  0015dfd2: blx 0x000738c4
  0015dfd6: ldr r0,[r4,#0x0]
  0015dfd8: str r4,[sp,#0x8]
  0015dfda: blx 0x00071a58
  0015dfde: mov r9,r0
  0015dfe0: mov r0,r5
  0015dfe2: blx 0x000718d8
  0015dfe6: mov r10,r0
  0015dfe8: cmp r6,#0x1
  0015dfea: bne 0x0015dff6
  0015dfec: mov r0,r5
  0015dfee: blx 0x0007183c
  0015dff2: mov r2,r0
  0015dff4: b 0x0015dff8
  0015dff6: movs r2,#0x1
  0015dff8: mov r0,r9
  0015dffa: mov r1,r10
  0015dffc: str r6,[sp,#0x4]
  0015dffe: blx 0x00072454
  0015e002: ldr.w r0,[r11,#0x10]
  0015e006: cbz r0,0x0015e070
  0015e008: movs r6,#0x0
  0015e00a: b 0x0015e02a
  0015e00c: ldr r0,[r0,#0x4]
  0015e00e: ldr.w r5,[r0,r6,lsl #0x2]
  0015e012: mov r0,r5
  0015e014: blx 0x000718d8
  0015e018: mov r4,r0
  0015e01a: mov r0,r8
  0015e01c: blx 0x000718d8
  0015e020: cmp r4,r0
  0015e022: beq 0x0015e032
  0015e024: ldr.w r0,[r11,#0x10]
  0015e028: adds r6,#0x1
  0015e02a: ldr r1,[r0,#0x0]
  0015e02c: cmp r6,r1
  0015e02e: bcc 0x0015e00c
  0015e030: b 0x0015e070
  0015e032: mov r0,r5
  0015e034: blx 0x00077014
  0015e038: cbz r0,0x0015e04a
  0015e03a: ldr r0,[sp,#0x4]
  0015e03c: cmp r0,#0x1
  0015e03e: bne 0x0015e066
  0015e040: mov r0,r5
  0015e042: blx 0x0007183c
  0015e046: rsbs r1,r0
  0015e048: b 0x0015e06a
  0015e04a: ldr r0,[sp,#0x4]
  0015e04c: cmp r0,#0x1
  0015e04e: beq 0x0015e05a
  0015e050: mov r0,r5
  0015e052: blx 0x0007183c
  0015e056: cmp r0,#0x1
  0015e058: bne 0x0015e066
  0015e05a: ldr.w r1,[r11,#0x10]
  0015e05e: mov r0,r5
  0015e060: blx 0x00075a3c
  0015e064: b 0x0015e070
  0015e066: mov.w r1,#0xffffffff
  0015e06a: mov r0,r5
  0015e06c: blx 0x00071854
  0015e070: ldr r1,[0x0015e110]
  0015e072: ldr r5,[sp,#0x8]
  0015e074: add r1,pc
  0015e076: ldr r0,[r5,#0x0]
  0015e078: ldr r6,[r1,#0x0]
  0015e07a: blx r6
  0015e07c: mov r4,r0
  0015e07e: ldr.w r0,[r11,#0x10]
  0015e082: movs r1,#0x1
  0015e084: blx 0x000770d4
  0015e088: mov r1,r0
  0015e08a: mov r0,r4
  0015e08c: blx 0x000738b8
  0015e090: ldr r0,[r5,#0x0]
  0015e092: ldr.w r4,[r11,#0x14]
  0015e096: blx r6
  0015e098: mov r1,r0
  0015e09a: mov r0,r4
  0015e09c: blx 0x000761e0
  0015e0a0: ldr r0,[r5,#0x0]
  0015e0a2: ldr.w r8,[r11,#0x14]
  0015e0a6: blx r6
  0015e0a8: blx 0x00073750
  0015e0ac: mov r4,r0
  0015e0ae: ldr r0,[r5,#0x0]
  0015e0b0: blx 0x00071c14
  0015e0b4: blx 0x00071830
  0015e0b8: mov r1,r0
  0015e0ba: mov r0,r4
  0015e0bc: blx 0x00076ee8
  0015e0c0: mov r4,r0
  0015e0c2: ldr r0,[r5,#0x0]
  0015e0c4: blx 0x00071c14
  0015e0c8: blx 0x00073708
  0015e0cc: mov r2,r0
  0015e0ce: mov r0,r8
  0015e0d0: mov r1,r4
  0015e0d2: blx 0x000761ec
  0015e0d6: ldr.w r0,[r11,#0x14]
  0015e0da: movs r1,#0x0
  0015e0dc: movs r2,#0x1
  0015e0de: movs r4,#0x0
  0015e0e0: blx 0x00076f24
  0015e0e4: mov r0,r11
  0015e0e6: blx 0x00076f30
  0015e0ea: ldr r0,[0x0015e114]
  0015e0ec: movs r2,#0x0
  0015e0ee: ldr.w r1,[r11,#0xe4]
  0015e0f2: movs r3,#0x0
  0015e0f4: add r0,pc
  0015e0f6: str.w r1,[r11,#0xb4]
  0015e0fa: movs r1,#0x62
  0015e0fc: str r4,[sp,#0x0]
  0015e0fe: ldr r0,[r0,#0x0]
  0015e100: ldr r0,[r0,#0x0]
  0015e102: blx 0x00071548
  0015e106: pop.w {r1,r2,r3,r8,r9,r10,r11}
  0015e10a: pop {r4,r5,r6,r7,pc}
```
