# HangarWindow::OnTouchBegin
elf 0x14b740 body 1036
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* HangarWindow::OnTouchBegin(int, int) */

void HangarWindow::OnTouchBegin(int param_1,int param_2)

{
  byte bVar1;
  byte bVar2;
  bool bVar3;
  int iVar4;
  ListItem *pLVar5;
  Ship *this;
  Array *pAVar6;
  Station *pSVar7;
  undefined4 *puVar8;
  String *pSVar9;
  undefined4 uVar10;
  int in_r2;
  HangarList *this_00;
  int iVar11;
  uint uVar12;
  undefined4 uVar13;
  code *pcVar14;
  int *piVar15;
  int *piVar16;
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar16 = *(int **)(DAT_0015ba58 + 0x15b756);
  piVar15 = *(int **)(DAT_0015ba5c + 0x15b75a);
  local_28 = *piVar16;
  *(undefined4 *)(param_1 + 0x6c) = 0;
  *(undefined4 *)(param_1 + 0x70) = 0;
  bVar1 = Layout::OnTouchBegin(*piVar15,param_2);
  if (*(char *)(param_1 + 0x3c) != '\0') {
    if (*(char *)(param_1 + 0xae) == '\0') {
      if (*(char *)(param_1 + 0xb0) != '\0') {
        for (iVar11 = 0x12; iVar11 != 0x17; iVar11 = iVar11 + 1) {
          TouchButton::OnTouchBegin
                    (*(int *)(*(int *)(*(int *)(param_1 + 0x24) + 4) + iVar11 * 4),param_2);
        }
      }
    }
    else {
      for (iVar11 = 0xc; iVar11 != 0x11; iVar11 = iVar11 + 1) {
        TouchButton::OnTouchBegin
                  (*(int *)(*(int *)(*(int *)(param_1 + 0x24) + 4) + iVar11 * 4),param_2);
      }
      TouchButton::OnTouchBegin(*(int *)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x44),param_2);
    }
    ChoiceWindow::OnTouchBegin(*(int *)(param_1 + 0x20),param_2);
    goto LAB_0015b804;
  }
  *(int *)(param_1 + 0xcc) = in_r2;
  *(int *)(param_1 + 0xb8) = in_r2;
  *(undefined4 *)(param_1 + 0xc0) = 0;
  *(undefined1 *)(param_1 + 0xd0) = 1;
  if (*(int *)(param_1 + 0x58) == 1) {
    ListItemWindow::OnTouchBegin(*(ListItemWindow **)(param_1 + 0x18),param_2,in_r2);
    goto LAB_0015b804;
  }
  iVar11 = *piVar15;
  if ((*(int *)(iVar11 + 0xc) < in_r2) &&
     (in_r2 < **(int **)(DAT_0015ba60 + 0x15b82e) - *(int *)(iVar11 + 0x10))) {
    this_00 = *(HangarList **)(param_1 + 0x14);
    iVar11 = __aeabi_idiv((((in_r2 - *(int *)(iVar11 + 0xc)) - *(int *)(iVar11 + 0x20)) -
                          *(int *)(param_1 + 0x10c)) - *(int *)(param_1 + 0xb4),
                          *(int *)(iVar11 + 0x70) + *(int *)(param_1 + 0x10c));
    iVar4 = HangarList::getCurrentLength(this_00);
    if (iVar4 <= iVar11) goto LAB_0015b8e6;
    HangarList::setCurrentItemIndex(*(HangarList **)(param_1 + 0x14),iVar11);
    pLVar5 = (ListItem *)HangarList::getCurrentItem(*(HangarList **)(param_1 + 0x14));
    highlightItem((HangarWindow *)param_1,pLVar5);
    if (*(char *)(param_1 + 0x11d) == '\0') {
LAB_0015b9d8:
      bVar2 = 0;
    }
    else {
      pLVar5 = (ListItem *)HangarList::getCurrentItem(*(HangarList **)(param_1 + 0x14));
      iVar11 = ::ListItem::isShip(pLVar5);
      if ((iVar11 == 0) ||
         (iVar11 = HangarList::getCurrentTab(*(HangarList **)(param_1 + 0x14)), iVar11 != 1))
      goto LAB_0015b9d8;
      this = (Ship *)Status::getShip();
      pAVar6 = (Array *)Item::extractItems(*(Array **)(param_1 + 0x10),true);
      Ship::setCargo(this,pAVar6);
      pSVar7 = (Station *)Status::getStation();
      bVar2 = 0;
      pAVar6 = (Array *)Item::extractItems(*(Array **)(param_1 + 0x10),false);
      Station::setItems(pSVar7,pAVar6,false);
    }
  }
  else {
LAB_0015b8e6:
    bVar2 = 1;
  }
  iVar11 = HangarList::getCurrentTab(*(HangarList **)(param_1 + 0x14));
  if (((((iVar11 == 4) && (*(char *)(param_1 + 0x88) != '\0')) &&
       (!(bool)(~bVar2 & *(int *)(param_1 + 0x68) == *(int *)(param_1 + 0x84)))) &&
      ((0 < *(int *)(param_1 + 0x94) && (*(char *)(param_1 + 0x3c) == '\0')))) &&
     (iVar11 = BluePrint::isEmpty(*(BluePrint **)(param_1 + 0x80)), iVar11 == 0)) {
    iVar11 = BluePrint::getStationIndex(*(BluePrint **)(param_1 + 0x80));
    puVar8 = *(undefined4 **)(DAT_0015bbd8 + 0x15b938);
    pSVar7 = (Station *)Status::getStation();
    iVar4 = Station::getIndex(pSVar7);
    if (iVar11 != iVar4) {
      iVar11 = Item::getIndex(*(Item **)(*(int *)(param_1 + 0x84) + 0x10));
      if (iVar11 == 0xd1) {
        bVar3 = true;
      }
      else {
        iVar11 = Item::getIndex(*(Item **)(*(int *)(param_1 + 0x84) + 0x10));
        bVar3 = iVar11 == 0xcc;
      }
      *(bool *)(param_1 + 0x11e) = bVar3;
      AbyssEngine::String::String(aSStack_34);
      if (*(char *)(param_1 + 0x11e) == '\0') {
        pSVar9 = (String *)GameText::getText(**(int **)(DAT_0015bbdc + 0x15ba14));
        AbyssEngine::String::String(aSStack_40,pSVar9,false);
      }
      else {
        pSVar9 = (String *)GameText::getText(**(int **)(DAT_0015bbdc + 0x15ba14));
        AbyssEngine::String::String(aSStack_40,pSVar9,false);
      }
      AbyssEngine::String::operator=(aSStack_34,aSStack_40);
      AbyssEngine::String::~String(aSStack_40);
      if (*(char *)(param_1 + 0x11e) == '\0') {
        uVar13 = *puVar8;
        AbyssEngine::String::String(aSStack_4c,aSStack_34,false);
        BluePrint::getStationName();
        AbyssEngine::String::String(aSStack_58,aSStack_64,false);
        uVar10 = AbyssEngine::String::String(aSStack_70,(char *)(DAT_0015bbe0 + 0x15ba8c),false);
        Status::replaceHash(aSStack_40,uVar13,aSStack_4c,aSStack_58,uVar10);
        AbyssEngine::String::operator=(aSStack_34,aSStack_40);
        pcVar14 = *(code **)(DAT_0015bbe4 + 0x15baae);
        (*pcVar14)(aSStack_40);
        (*pcVar14)(aSStack_70);
        (*pcVar14)(aSStack_58);
        (*pcVar14)(aSStack_64);
        (*pcVar14)(aSStack_4c);
        uVar13 = *puVar8;
        AbyssEngine::String::String(aSStack_7c,aSStack_34,false);
        Item::getBlueprintAmount(*(Item **)(*(int *)(param_1 + 0x84) + 0x10));
        Layout::formatCredits((int)aSStack_64);
        AbyssEngine::String::String(aSStack_88,aSStack_64,false);
        uVar10 = AbyssEngine::String::String(aSStack_94,(char *)(DAT_0015bbe8 + 0x15bafe),false);
        Status::replaceHash(aSStack_40,uVar13,aSStack_7c,aSStack_88,uVar10);
        AbyssEngine::String::operator=(aSStack_34,aSStack_40);
        (*pcVar14)(aSStack_40);
        (*pcVar14)(aSStack_94);
        (*pcVar14)(aSStack_88);
        (*pcVar14)(aSStack_64);
        (*pcVar14)(aSStack_7c);
        bVar3 = *(char *)(param_1 + 0x11e) == '\0';
      }
      else {
        bVar3 = false;
      }
      ChoiceWindow::set(*(ChoiceWindow **)(param_1 + 0x20),aSStack_34,bVar3);
      *(undefined1 *)(param_1 + 0x3c) = 1;
      *(undefined1 *)(param_1 + 0xac) = 1;
      *(undefined1 *)(param_1 + 0xd1) = 1;
      AbyssEngine::String::~String(aSStack_34);
      goto LAB_0015b804;
    }
  }
  for (uVar12 = 0; uVar12 < **(uint **)(param_1 + 4); uVar12 = uVar12 + 1) {
    bVar2 = TouchButton::OnTouchBegin(*(int *)((*(uint **)(param_1 + 4))[1] + uVar12 * 4),param_2);
    bVar1 = bVar1 | bVar2;
  }
  for (uVar12 = 0; uVar12 < **(uint **)(param_1 + 0x24); uVar12 = uVar12 + 1) {
    iVar11 = *(int *)((*(uint **)(param_1 + 0x24))[1] + uVar12 * 4);
    if (iVar11 != 0) {
      TouchButton::OnTouchBegin(iVar11,param_2);
    }
  }
  if (((*(char *)(param_1 + 0xf8) != '\0') &&
      (iVar11 = HangarList::getCurrentTab(*(HangarList **)(param_1 + 0x14)), iVar11 == 1)) &&
     ((iVar11 = *(int *)(param_1 + 0xfc), -1 < iVar11 &&
      (iVar4 = HangarList::getCurrentItemIndex(*(HangarList **)(param_1 + 0x14)),
      (~bVar1 & iVar11 == iVar4) == 0)))) {
    *(undefined1 *)(param_1 + 0xf8) = 0;
    autoEquipSecondaryWeapons(param_1);
  }
LAB_0015b804:
  iVar11 = *piVar16 - local_28;
  if (iVar11 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar11);
  }
  return;
}

```

## target disasm
```
  0015b740: push {r4,r5,r6,r7,lr}
  0015b742: add r7,sp,#0xc
  0015b744: push {r8,r9,r10,r11}
  0015b748: sub sp,#0x7c
  0015b74a: mov r9,r0
  0015b74c: ldr r0,[0x0015ba58]
  0015b74e: mov r6,r1
  0015b750: ldr r1,[0x0015ba5c]
  0015b752: add r0,pc
  0015b754: movs r4,#0x0
  0015b756: add r1,pc
  0015b758: mov r5,r2
  0015b75a: ldr.w r11,[r0,#0x0]
  0015b75e: ldr.w r10,[r1,#0x0]
  0015b762: mov r1,r6
  0015b764: ldr.w r0,[r11,#0x0]
  0015b768: str r0,[sp,#0x78]
  0015b76a: strd r4,r4,[r9,#0x6c]
  0015b76e: ldr.w r0,[r10,#0x0]
  0015b772: blx 0x00074728
  0015b776: mov r8,r0
  0015b778: ldrb.w r0,[r9,#0x3c]
  0015b77c: cbz r0,0x0015b7ae
  0015b77e: ldrb.w r0,[r9,#0xae]
  0015b782: cbz r0,0x0015b7d6
  0015b784: movs r4,#0xc
  0015b786: b 0x0015b798
  0015b788: ldr r0,[r0,#0x4]
  0015b78a: mov r1,r6
  0015b78c: mov r2,r5
  0015b78e: ldr.w r0,[r0,r4,lsl #0x2]
  0015b792: blx 0x00074734
  0015b796: adds r4,#0x1
  0015b798: ldr.w r0,[r9,#0x24]
  0015b79c: cmp r4,#0x11
  0015b79e: bne 0x0015b788
  0015b7a0: ldr r0,[r0,#0x4]
  0015b7a2: mov r1,r6
  0015b7a4: mov r2,r5
  0015b7a6: ldr r0,[r0,#0x44]
  0015b7a8: blx 0x00074734
  0015b7ac: b 0x0015b7f8
  0015b7ae: str.w r5,[r9,#0xcc]
  0015b7b2: movs r1,#0x1
  0015b7b4: str.w r5,[r9,#0xb8]
  0015b7b8: str.w r4,[r9,#0xc0]
  0015b7bc: ldr.w r0,[r9,#0x58]
  0015b7c0: strb.w r1,[r9,#0xd0]
  0015b7c4: cmp r0,#0x1
  0015b7c6: bne 0x0015b81c
  0015b7c8: ldr.w r0,[r9,#0x18]
  0015b7cc: mov r1,r6
  0015b7ce: mov r2,r5
  0015b7d0: blx 0x00077098
  0015b7d4: b 0x0015b804
  0015b7d6: ldrb.w r0,[r9,#0xb0]
  0015b7da: cbz r0,0x0015b7f8
  0015b7dc: movs r4,#0x12
  0015b7de: b 0x0015b7f4
  0015b7e0: ldr.w r0,[r9,#0x24]
  0015b7e4: mov r1,r6
  0015b7e6: mov r2,r5
  0015b7e8: ldr r0,[r0,#0x4]
  0015b7ea: ldr.w r0,[r0,r4,lsl #0x2]
  0015b7ee: blx 0x00074734
  0015b7f2: adds r4,#0x1
  0015b7f4: cmp r4,#0x17
  0015b7f6: bne 0x0015b7e0
  0015b7f8: ldr.w r0,[r9,#0x20]
  0015b7fc: mov r1,r6
  0015b7fe: mov r2,r5
  0015b800: blx 0x0007471c
  0015b804: ldr r0,[sp,#0x78]
  0015b806: ldr.w r1,[r11,#0x0]
  0015b80a: subs r0,r1,r0
  0015b80c: itttt eq
  0015b80e: mov.eq r0,#0x0
  0015b810: add.eq sp,#0x7c
  0015b812: pop.eq.w {r8,r9,r10,r11}
  0015b816: pop.eq {r4,r5,r6,r7,pc}
  0015b818: blx 0x0006e824
  0015b81c: ldr.w r1,[r10,#0x0]
  0015b820: ldr r0,[r1,#0xc]
  0015b822: cmp r0,r5
  0015b824: bge 0x0015b8e6
  0015b826: ldr r2,[0x0015ba60]
  0015b828: ldr r3,[r1,#0x10]
  0015b82a: add r2,pc
  0015b82c: ldr r2,[r2,#0x0]
  0015b82e: ldr r2,[r2,#0x0]
  0015b830: subs r2,r2,r3
  0015b832: cmp r2,r5
  0015b834: ble 0x0015b8e6
  0015b836: ldr r4,[r1,#0x20]
  0015b838: subs r0,r5,r0
  0015b83a: ldr.w r2,[r9,#0x14]
  0015b83e: ldr.w r3,[r9,#0x10c]
  0015b842: subs r0,r0,r4
  0015b844: ldr r1,[r1,#0x70]
  0015b846: str r2,[sp,#0x8]
  0015b848: subs r0,r0,r3
  0015b84a: ldr.w r2,[r9,#0xb4]
  0015b84e: add r1,r3
  0015b850: subs r0,r0,r2
  0015b852: blx 0x0007198c
  0015b856: mov r4,r0
  0015b858: ldr r0,[sp,#0x8]
  0015b85a: blx 0x000770a4
  0015b85e: cmp r4,r0
  0015b860: bge 0x0015b8e6
  0015b862: ldr.w r0,[r9,#0x14]
  0015b866: mov r1,r4
  0015b868: blx 0x000770b0
  0015b86c: ldr.w r0,[r9,#0x14]
  0015b870: blx 0x000770bc
  0015b874: mov r1,r0
  0015b876: mov r0,r9
  0015b878: blx 0x000770c8
  0015b87c: ldrb.w r0,[r9,#0x11d]
  0015b880: cmp r0,#0x0
  0015b882: beq.w 0x0015b9d8
  0015b886: ldr.w r0,[r9,#0x14]
  0015b88a: blx 0x000770bc
  0015b88e: blx 0x00076204
  0015b892: cmp r0,#0x0
  0015b894: beq.w 0x0015b9d8
  0015b898: ldr.w r0,[r9,#0x14]
  0015b89c: blx 0x00076f48
  0015b8a0: cmp r0,#0x1
  0015b8a2: bne.w 0x0015b9d8
  0015b8a6: ldr r0,[0x0015bbd4]
  0015b8a8: add r0,pc
  0015b8aa: ldr r4,[r0,#0x0]
  0015b8ac: ldr r0,[r4,#0x0]
  0015b8ae: blx 0x00071a58
  0015b8b2: str r0,[sp,#0x8]
  0015b8b4: movs r1,#0x1
  0015b8b6: ldr.w r0,[r9,#0x10]
  0015b8ba: blx 0x000770d4
  0015b8be: mov r1,r0
  0015b8c0: ldr r0,[sp,#0x8]
  0015b8c2: blx 0x000738b8
  0015b8c6: ldr r0,[r4,#0x0]
  0015b8c8: blx 0x00071c14
  0015b8cc: str r0,[sp,#0x8]
  0015b8ce: movs r1,#0x0
  0015b8d0: ldr.w r0,[r9,#0x10]
  0015b8d4: movs r4,#0x0
  0015b8d6: blx 0x000770d4
  0015b8da: mov r1,r0
  0015b8dc: ldr r0,[sp,#0x8]
  0015b8de: movs r2,#0x0
  0015b8e0: blx 0x000736e4
  0015b8e4: b 0x0015b8e8
  0015b8e6: movs r4,#0x1
  0015b8e8: ldr.w r0,[r9,#0x14]
  0015b8ec: blx 0x00076f48
  0015b8f0: cmp r0,#0x4
  0015b8f2: bne 0x0015b948
  0015b8f4: ldrb.w r0,[r9,#0x88]
  0015b8f8: cbz r0,0x0015b948
  0015b8fa: ldr.w r0,[r9,#0x68]
  0015b8fe: ldr.w r1,[r9,#0x84]
  0015b902: subs r0,r0,r1
  0015b904: mvns r1,r4
  0015b906: clz r0,r0
  0015b90a: lsrs r0,r0,#0x5
  0015b90c: tst r1,r0
  0015b90e: bne 0x0015b948
  0015b910: ldr.w r0,[r9,#0x94]
  0015b914: cmp r0,#0x1
  0015b916: blt 0x0015b948
  0015b918: ldrb.w r0,[r9,#0x3c]
  0015b91c: cbnz r0,0x0015b948
  0015b91e: ldr.w r0,[r9,#0x80]
  0015b922: blx 0x000738f4
  0015b926: cbnz r0,0x0015b948
  0015b928: ldr.w r0,[r9,#0x80]
  0015b92c: blx 0x00073624
  0015b930: mov r4,r0
  0015b932: ldr r0,[0x0015bbd8]
  0015b934: add r0,pc
  0015b936: ldr r0,[r0,#0x0]
  0015b938: str r0,[sp,#0x8]
  0015b93a: ldr r0,[r0,#0x0]
  0015b93c: blx 0x00071c14
  0015b940: blx 0x00071824
  0015b944: cmp r4,r0
  0015b946: bne 0x0015b9dc
  0015b948: movs r4,#0x0
  0015b94a: b 0x0015b960
  0015b94c: ldr r0,[r0,#0x4]
  0015b94e: mov r1,r6
  0015b950: mov r2,r5
  0015b952: ldr.w r0,[r0,r4,lsl #0x2]
  0015b956: blx 0x00074734
  0015b95a: orr.w r8,r8,r0
  0015b95e: adds r4,#0x1
  0015b960: ldr.w r0,[r9,#0x4]
  0015b964: ldr r1,[r0,#0x0]
  0015b966: cmp r4,r1
  0015b968: bcc 0x0015b94c
  0015b96a: movs r4,#0x0
  0015b96c: b 0x0015b980
  0015b96e: ldr r0,[r0,#0x4]
  0015b970: ldr.w r0,[r0,r4,lsl #0x2]
  0015b974: cbz r0,0x0015b97e
  0015b976: mov r1,r6
  0015b978: mov r2,r5
  0015b97a: blx 0x00074734
  0015b97e: adds r4,#0x1
  0015b980: ldr.w r0,[r9,#0x24]
  0015b984: ldr r1,[r0,#0x0]
  0015b986: cmp r4,r1
  0015b988: bcc 0x0015b96e
  0015b98a: ldrb.w r0,[r9,#0xf8]
  0015b98e: cmp r0,#0x0
  0015b990: beq.w 0x0015b804
  0015b994: ldr.w r0,[r9,#0x14]
  0015b998: blx 0x00076f48
  0015b99c: cmp r0,#0x1
  0015b99e: bne.w 0x0015b804
  0015b9a2: ldr.w r4,[r9,#0xfc]
  0015b9a6: cmp r4,#0x0
  0015b9a8: blt.w 0x0015b804
  0015b9ac: ldr.w r0,[r9,#0x14]
  0015b9b0: blx 0x000770e0
  0015b9b4: subs r0,r4,r0
  0015b9b6: mvn.w r1,r8
  0015b9ba: clz r0,r0
  0015b9be: lsrs r0,r0,#0x5
  0015b9c0: tst r1,r0
  0015b9c2: bne.w 0x0015b804
  0015b9c6: ldr.w r1,[r9,#0xfc]
  0015b9ca: movs r0,#0x0
  0015b9cc: strb.w r0,[r9,#0xf8]
  0015b9d0: mov r0,r9
  0015b9d2: blx 0x000770ec
  0015b9d6: b 0x0015b804
  0015b9d8: movs r4,#0x0
  0015b9da: b 0x0015b8e8
  0015b9dc: ldr.w r0,[r9,#0x84]
  0015b9e0: ldr r0,[r0,#0x10]
  0015b9e2: blx 0x000718d8
  0015b9e6: cmp r0,#0xd1
  0015b9e8: bne 0x0015b9ee
  0015b9ea: movs r0,#0x1
  0015b9ec: b 0x0015ba00
  0015b9ee: ldr.w r0,[r9,#0x84]
  0015b9f2: ldr r0,[r0,#0x10]
  0015b9f4: blx 0x000718d8
  0015b9f8: subs r0,#0xcc
  0015b9fa: clz r0,r0
  0015b9fe: lsrs r0,r0,#0x5
  0015ba00: strb.w r0,[r9,#0x11e]
  0015ba04: add r0,sp,#0x6c
  0015ba06: blx 0x0006efbc
  0015ba0a: ldr r0,[0x0015bbdc]
  0015ba0c: ldrb.w r1,[r9,#0x11e]
  0015ba10: add r0,pc
  0015ba12: cmp r1,#0x0
  0015ba14: ldr r0,[r0,#0x0]
  0015ba16: ldr r0,[r0,#0x0]
  0015ba18: beq 0x0015ba2e
  0015ba1a: movw r1,#0x121
  0015ba1e: blx 0x00072f70
  0015ba22: mov r1,r0
  0015ba24: add r0,sp,#0x60
  0015ba26: movs r2,#0x0
  0015ba28: blx 0x0006f028
  0015ba2c: b 0x0015ba40
  0015ba2e: mov.w r1,#0x120
  0015ba32: blx 0x00072f70
  0015ba36: mov r1,r0
  0015ba38: add r0,sp,#0x60
  0015ba3a: movs r2,#0x0
  0015ba3c: blx 0x0006f028
  0015ba40: add r0,sp,#0x6c
  0015ba42: add r1,sp,#0x60
  0015ba44: blx 0x0006f2b0
  0015ba48: add r0,sp,#0x60
  0015ba4a: blx 0x0006ee30
  0015ba4e: ldrb.w r0,[r9,#0x11e]
  0015ba52: cbz r0,0x0015ba64
  0015ba54: movs r2,#0x0
  0015ba56: b 0x0015bb38
  0015ba64: ldr r0,[sp,#0x8]
  0015ba66: ldr r5,[r0,#0x0]
  0015ba68: add r0,sp,#0x54
  0015ba6a: add r1,sp,#0x6c
  0015ba6c: movs r2,#0x0
  0015ba6e: blx 0x0006f028
  0015ba72: ldr.w r1,[r9,#0x80]
  0015ba76: add r0,sp,#0x3c
  0015ba78: blx 0x0007672c
  0015ba7c: add r0,sp,#0x48
  0015ba7e: add r1,sp,#0x3c
  0015ba80: movs r2,#0x0
  0015ba82: blx 0x0006f028
  0015ba86: ldr r1,[0x0015bbe0]
  0015ba88: add r1,pc
  0015ba8a: add r0,sp,#0x30
  0015ba8c: movs r2,#0x0
  0015ba8e: blx 0x0006ee18
  0015ba92: str r0,[sp,#0x0]
  0015ba94: add r0,sp,#0x60
  0015ba96: add r2,sp,#0x54
  0015ba98: add r3,sp,#0x48
  0015ba9a: mov r1,r5
  0015ba9c: blx 0x000739b4
  0015baa0: add r0,sp,#0x6c
  0015baa2: add r1,sp,#0x60
  0015baa4: blx 0x0006f2b0
  0015baa8: ldr r0,[0x0015bbe4]
  0015baaa: add r0,pc
  0015baac: ldr r5,[r0,#0x0]
  0015baae: add r0,sp,#0x60
  0015bab0: blx r5
  0015bab2: add r0,sp,#0x30
  0015bab4: blx r5
  0015bab6: add r0,sp,#0x48
  0015bab8: blx r5
  0015baba: add r0,sp,#0x3c
  0015babc: blx r5
  0015babe: add r0,sp,#0x54
  0015bac0: blx r5
  0015bac2: ldr r0,[sp,#0x8]
  0015bac4: ldr.w r8,[r0,#0x0]
  0015bac8: add r0,sp,#0x24
  0015baca: add r1,sp,#0x6c
  0015bacc: movs r2,#0x0
  0015bace: blx 0x0006f028
  0015bad2: ldr.w r0,[r9,#0x84]
  0015bad6: ldr.w r6,[r10,#0x0]
  0015bada: ldr r0,[r0,#0x10]
  0015badc: blx 0x00076f78
  0015bae0: movs r1,#0xc8
  0015bae2: mul r2,r0,r1
  0015bae6: add r0,sp,#0x3c
  0015bae8: mov r1,r6
  0015baea: blx 0x00074e54
  0015baee: add r0,sp,#0x18
  0015baf0: add r1,sp,#0x3c
  0015baf2: movs r2,#0x0
  0015baf4: blx 0x0006f028
  0015baf8: ldr r1,[0x0015bbe8]
  0015bafa: add r1,pc
  0015bafc: add r0,sp,#0xc
  0015bafe: movs r2,#0x0
  0015bb00: blx 0x0006ee18
  0015bb04: str r0,[sp,#0x0]
  0015bb06: add r0,sp,#0x60
  0015bb08: add r2,sp,#0x24
  0015bb0a: add r3,sp,#0x18
  0015bb0c: mov r1,r8
  0015bb0e: blx 0x000739b4
  0015bb12: add r0,sp,#0x6c
  0015bb14: add r1,sp,#0x60
  0015bb16: blx 0x0006f2b0
  0015bb1a: add r0,sp,#0x60
  0015bb1c: blx r5
  0015bb1e: add r0,sp,#0xc
  0015bb20: blx r5
  0015bb22: add r0,sp,#0x18
  0015bb24: blx r5
  0015bb26: add r0,sp,#0x3c
  0015bb28: blx r5
  0015bb2a: add r0,sp,#0x24
  0015bb2c: blx r5
  0015bb2e: ldrb.w r0,[r9,#0x11e]
  0015bb32: clz r0,r0
  0015bb36: lsrs r2,r0,#0x5
  0015bb38: ldr.w r0,[r9,#0x20]
  0015bb3c: add r1,sp,#0x6c
  0015bb3e: blx 0x000746f8
  0015bb42: movs r0,#0x1
  0015bb44: strb.w r0,[r9,#0x3c]
  0015bb48: strb.w r0,[r9,#0xac]
  0015bb4c: strb.w r0,[r9,#0xd1]
  0015bb50: add r0,sp,#0x6c
  0015bb52: blx 0x0006ee30
  0015bb56: b 0x0015b804
```
