# MenuTouchWindow::startValkyrie
elf 0x12ca80 body 544
Sig: undefined __thiscall startValkyrie(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::startValkyrie() */

void __thiscall MenuTouchWindow::startValkyrie(MenuTouchWindow *this)

{
  Mission *this_00;
  undefined4 uVar1;
  undefined4 uVar2;
  Achievements *this_01;
  int *piVar3;
  Mission *pMVar4;
  Ship *pSVar5;
  Station *pSVar6;
  int iVar7;
  int iVar8;
  code *pcVar9;
  undefined4 *puVar10;
  int *piVar11;
  code *pcVar12;
  code *pcVar13;
  
  piVar3 = *(int **)(DAT_0013ccb0 + 0x13ca90);
  Status::resetGame();
  for (iVar8 = 0x2d; pMVar4 = (Mission *)*piVar3, iVar8 != 0; iVar8 = iVar8 + -1) {
    Status::nextCampaignMission(SUB41(pMVar4,0));
  }
  this_00 = operator_new(0x78);
  Mission::Mission(this_00);
  Status::setMission(pMVar4);
  pSVar5 = (Ship *)*piVar3;
  Ship::makeShip(*(int *)(*(int *)(**(int **)(DAT_0013ccb4 + 0x13caca) + 4) + 0x14));
  Status::setShip(pSVar5);
  pcVar13 = *(code **)(DAT_0013ccb8 + 0x13cae4);
  iVar8 = (*pcVar13)(*piVar3);
  Ship::setRace(iVar8);
  piVar11 = *(int **)(DAT_0013ccbc + 0x13caf4);
  pcVar12 = *(code **)(DAT_0013ccc0 + 0x13cb02);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar11 + 4) + 8));
  uVar2 = (*pcVar13)(*piVar3);
  pcVar9 = *(code **)(DAT_0013ccc4 + 0x13cb16);
  (*pcVar9)(uVar2,uVar1,0);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar11 + 4) + 0x14));
  uVar2 = (*pcVar13)(*piVar3);
  (*pcVar9)(uVar2,uVar1,1);
  uVar1 = Item::makeItem(*(int *)(*(int *)(*piVar11 + 4) + 0x90));
  uVar2 = (*pcVar13)(*piVar3);
  (*pcVar9)(uVar2,uVar1,0);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar11 + 4) + 0x144));
  uVar2 = (*pcVar13)(*piVar3);
  (*pcVar9)(uVar2,uVar1,0);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar11 + 4) + 0xcc));
  uVar2 = (*pcVar13)(*piVar3);
  (*pcVar9)(uVar2,uVar1,1);
  uVar1 = Item::makeItem(*(int *)(*(int *)(*piVar11 + 4) + 0x158));
  uVar2 = (*pcVar13)(*piVar3);
  (*pcVar9)(uVar2,uVar1,2);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar11 + 4) + 0x154));
  uVar2 = (*pcVar13)(*piVar3);
  (*pcVar9)(uVar2,uVar1,3);
  Status::setCredits(*piVar3);
  pSVar6 = (Station *)*piVar3;
  Galaxy::getStation(**(int **)(DAT_0013ccc8 + 0x13cbc6));
  Status::setStation(pSVar6);
  Status::setSystemVisibility((Status *)*piVar3,6,true);
  Status::setSystemVisibility((Status *)*piVar3,0x19,true);
  Status::setCredits(*piVar3);
  iVar8 = *(int *)(DAT_0013cccc + 0x13cbf8);
  puVar10 = *(undefined4 **)(DAT_0013ccd0 + 0x13cbfc);
  iVar7 = *(int *)(DAT_0013ccd4 + 0x13cc00);
  *(undefined4 *)(*piVar3 + 0x84) = 0x1a0a;
  *(undefined2 *)(iVar8 + 8) = 0x101;
  *(undefined2 *)(iVar8 + 0xd) = 0x101;
  *(undefined2 *)(iVar8 + 0x1c) = 0x101;
  *(undefined4 *)(iVar8 + 0x20) = 0x1010101;
  this_01 = (Achievements *)*puVar10;
  *(undefined1 *)(iVar8 + 0x17) = 1;
  *(undefined1 *)(iVar8 + 10) = 1;
  *(undefined1 *)(iVar8 + 0x15) = 1;
  *(undefined1 *)(iVar8 + 0x13) = 1;
  *(undefined1 *)(iVar8 + 0xf) = 1;
  *(undefined1 *)(iVar8 + 0x1e) = 1;
  *(undefined1 *)(iVar8 + 0x24) = 1;
  *(undefined1 *)(iVar8 + 0x36) = 0;
  *(undefined4 *)(iVar8 + 0x32) = 0;
  *(undefined1 *)(iVar8 + 0x38) = 1;
  *(undefined1 *)(iVar7 + 0x34) = 1;
  Achievements::setMedal(this_01,0x17,3);
  Achievements::setMedal((Achievements *)*puVar10,0x1e,1);
  RecordHandler::saveOptions((RecordHandler *)**(undefined4 **)(DAT_0013ccd8 + 0x13cc66));
  Status::setKills(*piVar3);
  FModSound::stop(**(int **)(DAT_0013ccdc + 0x13cc7a));
  puVar10 = *(undefined4 **)(DAT_0013cce0 + 0x13cc8a);
  *(undefined4 *)(iVar7 + 0x2c) = *(undefined4 *)(this + 0x1a4);
  (*(code *)(DAT_001ab904 + 0x1ab908))(*puVar10,5);
  return;
}

```

## target disasm
```
  0013ca80: push {r4,r5,r6,r7,lr}
  0013ca82: add r7,sp,#0xc
  0013ca84: push {r7,r8,r9,r10,r11}
  0013ca88: str r0,[sp,#0x0]
  0013ca8a: ldr r0,[0x0013ccb0]
  0013ca8c: add r0,pc
  0013ca8e: ldr r4,[r0,#0x0]
  0013ca90: ldr r0,[r4,#0x0]
  0013ca92: blx 0x00074d58
  0013ca96: movs r6,#0x2d
  0013ca98: b 0x0013caa4
  0013ca9a: mov r0,r5
  0013ca9c: movs r1,#0x0
  0013ca9e: blx 0x00075070
  0013caa2: subs r6,#0x1
  0013caa4: ldr r5,[r4,#0x0]
  0013caa6: cmp r6,#0x0
  0013caa8: bne 0x0013ca9a
  0013caaa: movs r0,#0x78
  0013caac: blx 0x0006eb24
  0013cab0: mov r6,r0
  0013cab2: blx 0x0007384c
  0013cab6: mov r0,r5
  0013cab8: mov r1,r6
  0013caba: blx 0x00073c3c
  0013cabe: ldr r0,[0x0013ccb4]
  0013cac0: mov.w r1,#0xffffffff
  0013cac4: ldr r5,[r4,#0x0]
  0013cac6: add r0,pc
  0013cac8: ldr r0,[r0,#0x0]
  0013caca: ldr r0,[r0,#0x0]
  0013cacc: ldr r0,[r0,#0x4]
  0013cace: ldr r0,[r0,#0x14]
  0013cad0: blx 0x000719d4
  0013cad4: mov r1,r0
  0013cad6: mov r0,r5
  0013cad8: blx 0x00073870
  0013cadc: ldr r1,[0x0013ccb8]
  0013cade: ldr r0,[r4,#0x0]
  0013cae0: add r1,pc
  0013cae2: ldr.w r11,[r1,#0x0]
  0013cae6: blx r11
  0013cae8: movs r1,#0x0
  0013caea: blx 0x000719ec
  0013caee: ldr r0,[0x0013ccbc]
  0013caf0: add r0,pc
  0013caf2: ldr.w r8,[r0,#0x0]
  0013caf6: ldr.w r0,[r8,#0x0]
  0013cafa: ldr r1,[0x0013ccc0]
  0013cafc: ldr r0,[r0,#0x4]
  0013cafe: add r1,pc
  0013cb00: ldr.w r9,[r1,#0x0]
  0013cb04: ldr r0,[r0,#0x8]
  0013cb06: blx r9
  0013cb08: mov r5,r0
  0013cb0a: ldr r0,[r4,#0x0]
  0013cb0c: blx r11
  0013cb0e: ldr r1,[0x0013ccc4]
  0013cb10: movs r2,#0x0
  0013cb12: add r1,pc
  0013cb14: ldr r6,[r1,#0x0]
  0013cb16: mov r1,r5
  0013cb18: blx r6
  0013cb1a: ldr.w r0,[r8,#0x0]
  0013cb1e: ldr r0,[r0,#0x4]
  0013cb20: ldr r0,[r0,#0x14]
  0013cb22: blx r9
  0013cb24: mov r5,r0
  0013cb26: ldr r0,[r4,#0x0]
  0013cb28: blx r11
  0013cb2a: mov r1,r5
  0013cb2c: movs r2,#0x1
  0013cb2e: mov.w r10,#0x1
  0013cb32: blx r6
  0013cb34: ldr.w r0,[r8,#0x0]
  0013cb38: movs r1,#0xa
  0013cb3a: ldr r0,[r0,#0x4]
  0013cb3c: ldr.w r0,[r0,#0x90]
  0013cb40: blx 0x000718c0
  0013cb44: mov r5,r0
  0013cb46: ldr r0,[r4,#0x0]
  0013cb48: blx r11
  0013cb4a: mov r1,r5
  0013cb4c: movs r2,#0x0
  0013cb4e: blx r6
  0013cb50: ldr.w r0,[r8,#0x0]
  0013cb54: ldr r0,[r0,#0x4]
  0013cb56: ldr.w r0,[r0,#0x144]
  0013cb5a: blx r9
  0013cb5c: mov r5,r0
  0013cb5e: ldr r0,[r4,#0x0]
  0013cb60: blx r11
  0013cb62: mov r1,r5
  0013cb64: movs r2,#0x0
  0013cb66: blx r6
  0013cb68: ldr.w r0,[r8,#0x0]
  0013cb6c: ldr r0,[r0,#0x4]
  0013cb6e: ldr.w r0,[r0,#0xcc]
  0013cb72: blx r9
  0013cb74: mov r5,r0
  0013cb76: ldr r0,[r4,#0x0]
  0013cb78: blx r11
  0013cb7a: mov r1,r5
  0013cb7c: movs r2,#0x1
  0013cb7e: blx r6
  0013cb80: ldr.w r0,[r8,#0x0]
  0013cb84: movs r1,#0xa
  0013cb86: ldr r0,[r0,#0x4]
  0013cb88: ldr.w r0,[r0,#0x158]
  0013cb8c: blx 0x000718c0
  0013cb90: mov r5,r0
  0013cb92: ldr r0,[r4,#0x0]
  0013cb94: blx r11
  0013cb96: mov r1,r5
  0013cb98: movs r2,#0x2
  0013cb9a: blx r6
  0013cb9c: ldr.w r0,[r8,#0x0]
  0013cba0: ldr r0,[r0,#0x4]
  0013cba2: ldr.w r0,[r0,#0x154]
  0013cba6: blx r9
  0013cba8: mov r5,r0
  0013cbaa: ldr r0,[r4,#0x0]
  0013cbac: blx r11
  0013cbae: mov r1,r5
  0013cbb0: movs r2,#0x3
  0013cbb2: blx r6
  0013cbb4: ldr r0,[r4,#0x0]
  0013cbb6: movs r1,#0x0
  0013cbb8: blx 0x000758b0
  0013cbbc: ldr r0,[0x0013ccc8]
  0013cbbe: movs r1,#0x5b
  0013cbc0: ldr r5,[r4,#0x0]
  0013cbc2: add r0,pc
  0013cbc4: ldr r0,[r0,#0x0]
  0013cbc6: ldr r0,[r0,#0x0]
  0013cbc8: blx 0x00071c44
  0013cbcc: mov r1,r0
  0013cbce: mov r0,r5
  0013cbd0: blx 0x0007366c
  0013cbd4: ldr r0,[r4,#0x0]
  0013cbd6: movs r1,#0x6
  0013cbd8: movs r2,#0x1
  0013cbda: blx 0x0007174c
  0013cbde: ldr r0,[r4,#0x0]
  0013cbe0: movs r1,#0x19
  0013cbe2: movs r2,#0x1
  0013cbe4: blx 0x0007174c
  0013cbe8: ldr r0,[r4,#0x0]
  0013cbea: movs r1,#0x0
  0013cbec: blx 0x000758b0
  0013cbf0: ldr r0,[0x0013cccc]
  0013cbf2: ldr r1,[0x0013ccd0]
  0013cbf4: add r0,pc
  0013cbf6: ldr r2,[0x0013ccd4]
  0013cbf8: add r1,pc
  0013cbfa: ldr r3,[r0,#0x0]
  0013cbfc: add r2,pc
  0013cbfe: ldr r0,[r4,#0x0]
  0013cc00: ldr r6,[r1,#0x0]
  0013cc02: movw r1,#0x1a0a
  0013cc06: ldr r5,[r2,#0x0]
  0013cc08: movs r2,#0x3
  0013cc0a: str.w r1,[r0,#0x84]
  0013cc0e: movw r0,#0x101
  0013cc12: strh r0,[r3,#0x8]
  0013cc14: movs r1,#0x0
  0013cc16: strh.w r0,[r3,#0xd]
  0013cc1a: strh r0,[r3,#0x1c]
  0013cc1c: mov.w r0,#0x1010101
  0013cc20: str r0,[r3,#0x20]
  0013cc22: ldr r0,[r6,#0x0]
  0013cc24: strb.w r10,[r3,#0x17]
  0013cc28: strb.w r10,[r3,#0xa]
  0013cc2c: strb.w r10,[r3,#0x15]
  0013cc30: strb.w r10,[r3,#0x13]
  0013cc34: strb.w r10,[r3,#0xf]
  0013cc38: strb.w r10,[r3,#0x1e]
  0013cc3c: strb.w r10,[r3,#0x24]
  0013cc40: strb.w r1,[r3,#0x36]
  0013cc44: str.w r1,[r3,#0x32]
  0013cc48: movs r1,#0x17
  0013cc4a: strb.w r10,[r3,#0x38]
  0013cc4e: strb.w r10,[r5,#0x34]
  0013cc52: blx 0x00076714
  0013cc56: ldr r0,[r6,#0x0]
  0013cc58: movs r1,#0x1e
  0013cc5a: movs r2,#0x1
  0013cc5c: blx 0x00076714
  0013cc60: ldr r0,[0x0013ccd8]
  0013cc62: add r0,pc
  0013cc64: ldr r0,[r0,#0x0]
  0013cc66: ldr r0,[r0,#0x0]
  0013cc68: blx 0x00071788
  0013cc6c: ldr r0,[r4,#0x0]
  0013cc6e: movs r1,#0xc5
  0013cc70: blx 0x00076720
  0013cc74: ldr r0,[0x0013ccdc]
  0013cc76: add r0,pc
  0013cc78: ldr r0,[r0,#0x0]
  0013cc7a: ldr r0,[r0,#0x0]
  0013cc7c: ldr r1,[r0,#0x0]
  0013cc7e: blx 0x000724a8
  0013cc82: ldr r0,[0x0013cce0]
  0013cc84: ldr r1,[sp,#0x0]
  0013cc86: add r0,pc
  0013cc88: ldr r0,[r0,#0x0]
  0013cc8a: ldr.w r1,[r1,#0x1a4]
  0013cc8e: str r1,[r5,#0x2c]
  0013cc90: movs r1,#0x5
  0013cc92: ldr r0,[r0,#0x0]
  0013cc94: pop.w {r3,r8,r9,r10,r11}
  0013cc98: pop.w {r4,r5,r6,r7,lr}
  0013cc9c: b.w 0x001ab8f8
```
