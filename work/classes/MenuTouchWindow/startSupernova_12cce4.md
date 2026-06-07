# MenuTouchWindow::startSupernova
elf 0x12cce4 body 612
Sig: undefined __thiscall startSupernova(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::startSupernova() */

void __thiscall MenuTouchWindow::startSupernova(MenuTouchWindow *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  Item *pIVar3;
  int iVar4;
  Achievements *this_00;
  int iVar5;
  int *piVar6;
  Ship *pSVar7;
  Station *pSVar8;
  undefined4 *puVar9;
  int *piVar10;
  code *pcVar11;
  code *pcVar12;
  code *pcVar13;
  code *pcVar14;
  
  piVar10 = *(int **)(DAT_0013cf48 + 0x13ccf4);
  Status::resetGame();
  for (iVar5 = 0x54; iVar5 != 0; iVar5 = iVar5 + -1) {
    Status::nextCampaignMission(SUB41((Mission *)*piVar10,0));
  }
  Status::setMission((Mission *)*piVar10);
  pSVar7 = (Ship *)*piVar10;
  Ship::makeShip(*(int *)(*(int *)(**(int **)(DAT_0013cf50 + 0x13cd24) + 4) + 0x78));
  Status::setShip(pSVar7);
  pcVar14 = *(code **)(DAT_0013cf54 + 0x13cd3e);
  iVar5 = (*pcVar14)(*piVar10);
  Ship::setRace(iVar5);
  piVar6 = *(int **)(DAT_0013cf58 + 0x13cd4e);
  pcVar12 = *(code **)(DAT_0013cf5c + 0x13cd58);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0x2c0));
  uVar2 = (*pcVar14)(*piVar10);
  pcVar11 = *(code **)(DAT_0013cf60 + 0x13cd6e);
  (*pcVar11)(uVar2,uVar1,0);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0x50));
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,1);
  pcVar13 = *(code **)(DAT_0013cf64 + 0x13cd96);
  uVar1 = (*pcVar13)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0x90),0x14);
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,0);
  uVar1 = (*pcVar13)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0xb0),0x14);
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,1);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0x144));
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,0);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0xcc));
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,1);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0x110));
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,2);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0x158));
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,3);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0x154));
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,4);
  uVar1 = (*pcVar12)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0xe0));
  uVar2 = (*pcVar14)(*piVar10);
  (*pcVar11)(uVar2,uVar1,5);
  (*pcVar13)(*(undefined4 *)(*(int *)(*piVar6 + 4) + 0x1e8),8);
  pIVar3 = (Item *)(*pcVar14)(*piVar10);
  Ship::addCargo(pIVar3);
  Status::setCredits(*piVar10);
  pSVar8 = (Station *)*piVar10;
  Galaxy::getStation(**(int **)(DAT_0013cf68 + 0x13ce6e));
  Status::setStation(pSVar8);
  Status::setSystemVisibility((Status *)*piVar10,6,true);
  Status::setSystemVisibility((Status *)*piVar10,0x19,true);
  Status::setCredits(*piVar10);
  iVar4 = *(int *)(DAT_0013cf6c + 0x13cea4);
  puVar9 = *(undefined4 **)(DAT_0013cf70 + 0x13cea6);
  *(undefined4 *)(*piVar10 + 0x84) = 0x1a0a;
  *(undefined4 *)(iVar4 + 0x20) = 0x1010101;
  iVar5 = DAT_0013cf74;
  *(undefined1 *)(iVar4 + 0x17) = 1;
  iVar5 = *(int *)(iVar5 + 0x13cebe);
  *(undefined1 *)(iVar4 + 0x36) = 0;
  *(undefined4 *)(iVar4 + 0x32) = 0;
  *(undefined2 *)(iVar4 + 8) = 0x101;
  *(undefined1 *)(iVar4 + 10) = 1;
  *(undefined1 *)(iVar4 + 0x15) = 1;
  *(undefined1 *)(iVar4 + 0x13) = 1;
  *(undefined2 *)(iVar4 + 0xd) = 0x101;
  *(undefined1 *)(iVar4 + 0xf) = 1;
  *(undefined1 *)(iVar4 + 0x26) = 1;
  *(undefined1 *)(iVar4 + 0x31) = 1;
  *(undefined1 *)(iVar4 + 0x24) = 1;
  *(undefined2 *)(iVar4 + 0x1c) = 0x101;
  *(undefined1 *)(iVar4 + 0x1e) = 1;
  *(undefined2 *)(iVar4 + 0x38) = 0x101;
  this_00 = (Achievements *)*puVar9;
  *(undefined1 *)(iVar5 + 0x34) = 1;
  Achievements::setMedal(this_00,0x17,3);
  Achievements::setMedal((Achievements *)*puVar9,0x1e,1);
  Status::setKills(*piVar10);
  RecordHandler::saveOptions((RecordHandler *)**(undefined4 **)(DAT_0013cf78 + 0x13cf16));
  FModSound::stop(**(int **)(DAT_0013cf7c + 0x13cf22));
  puVar9 = *(undefined4 **)(DAT_0013cf80 + 0x13cf32);
  *(undefined4 *)(iVar5 + 0x2c) = *(undefined4 *)(this + 0x1a4);
  (*(code *)(DAT_001ab904 + 0x1ab908))(*puVar9,5);
  return;
}

```

## target disasm
```
  0013cce4: push {r4,r5,r6,r7,lr}
  0013cce6: add r7,sp,#0xc
  0013cce8: push {r7,r8,r9,r10,r11}
  0013ccec: str r0,[sp,#0x0]
  0013ccee: ldr r0,[0x0013cf48]
  0013ccf0: add r0,pc
  0013ccf2: ldr r6,[r0,#0x0]
  0013ccf4: ldr r0,[r6,#0x0]
  0013ccf6: blx 0x00074d58
  0013ccfa: movs r4,#0x54
  0013ccfc: b 0x0013cd06
  0013ccfe: movs r1,#0x0
  0013cd00: blx 0x00075070
  0013cd04: subs r4,#0x1
  0013cd06: ldr r0,[r6,#0x0]
  0013cd08: cmp r4,#0x0
  0013cd0a: bne 0x0013ccfe
  0013cd0c: ldr r1,[0x0013cf4c]
  0013cd0e: add r1,pc
  0013cd10: ldr r1,[r1,#0x0]
  0013cd12: ldr r1,[r1,#0x0]
  0013cd14: blx 0x00073c3c
  0013cd18: ldr r0,[0x0013cf50]
  0013cd1a: mov.w r1,#0xffffffff
  0013cd1e: ldr r5,[r6,#0x0]
  0013cd20: add r0,pc
  0013cd22: ldr r0,[r0,#0x0]
  0013cd24: ldr r0,[r0,#0x0]
  0013cd26: ldr r0,[r0,#0x4]
  0013cd28: ldr r0,[r0,#0x78]
  0013cd2a: blx 0x000719d4
  0013cd2e: mov r1,r0
  0013cd30: mov r0,r5
  0013cd32: blx 0x00073870
  0013cd36: ldr r1,[0x0013cf54]
  0013cd38: ldr r0,[r6,#0x0]
  0013cd3a: add r1,pc
  0013cd3c: ldr.w r11,[r1,#0x0]
  0013cd40: blx r11
  0013cd42: movs r1,#0x3
  0013cd44: blx 0x000719ec
  0013cd48: ldr r0,[0x0013cf58]
  0013cd4a: add r0,pc
  0013cd4c: ldr r4,[r0,#0x0]
  0013cd4e: ldr r0,[r4,#0x0]
  0013cd50: ldr r1,[0x0013cf5c]
  0013cd52: ldr r0,[r0,#0x4]
  0013cd54: add r1,pc
  0013cd56: ldr.w r9,[r1,#0x0]
  0013cd5a: ldr.w r0,[r0,#0x2c0]
  0013cd5e: blx r9
  0013cd60: mov r5,r0
  0013cd62: ldr r0,[r6,#0x0]
  0013cd64: blx r11
  0013cd66: ldr r1,[0x0013cf60]
  0013cd68: movs r2,#0x0
  0013cd6a: add r1,pc
  0013cd6c: ldr.w r8,[r1,#0x0]
  0013cd70: mov r1,r5
  0013cd72: blx r8
  0013cd74: ldr r0,[r4,#0x0]
  0013cd76: ldr r0,[r0,#0x4]
  0013cd78: ldr r0,[r0,#0x50]
  0013cd7a: blx r9
  0013cd7c: mov r5,r0
  0013cd7e: ldr r0,[r6,#0x0]
  0013cd80: blx r11
  0013cd82: mov r1,r5
  0013cd84: movs r2,#0x1
  0013cd86: blx r8
  0013cd88: ldr r0,[r4,#0x0]
  0013cd8a: ldr r0,[r0,#0x4]
  0013cd8c: ldr.w r0,[r0,#0x90]
  0013cd90: ldr r1,[0x0013cf64]
  0013cd92: add r1,pc
  0013cd94: ldr.w r10,[r1,#0x0]
  0013cd98: movs r1,#0x14
  0013cd9a: blx r10
  0013cd9c: mov r5,r0
  0013cd9e: ldr r0,[r6,#0x0]
  0013cda0: blx r11
  0013cda2: mov r1,r5
  0013cda4: movs r2,#0x0
  0013cda6: blx r8
  0013cda8: ldr r0,[r4,#0x0]
  0013cdaa: movs r1,#0x14
  0013cdac: ldr r0,[r0,#0x4]
  0013cdae: ldr.w r0,[r0,#0xb0]
  0013cdb2: blx r10
  0013cdb4: mov r5,r0
  0013cdb6: ldr r0,[r6,#0x0]
  0013cdb8: blx r11
  0013cdba: mov r1,r5
  0013cdbc: movs r2,#0x1
  0013cdbe: blx r8
  0013cdc0: ldr r0,[r4,#0x0]
  0013cdc2: ldr r0,[r0,#0x4]
  0013cdc4: ldr.w r0,[r0,#0x144]
  0013cdc8: blx r9
  0013cdca: mov r5,r0
  0013cdcc: ldr r0,[r6,#0x0]
  0013cdce: blx r11
  0013cdd0: mov r1,r5
  0013cdd2: movs r2,#0x0
  0013cdd4: blx r8
  0013cdd6: ldr r0,[r4,#0x0]
  0013cdd8: ldr r0,[r0,#0x4]
  0013cdda: ldr.w r0,[r0,#0xcc]
  0013cdde: blx r9
  0013cde0: mov r5,r0
  0013cde2: ldr r0,[r6,#0x0]
  0013cde4: blx r11
  0013cde6: mov r1,r5
  0013cde8: movs r2,#0x1
  0013cdea: blx r8
  0013cdec: ldr r0,[r4,#0x0]
  0013cdee: ldr r0,[r0,#0x4]
  0013cdf0: ldr.w r0,[r0,#0x110]
  0013cdf4: blx r9
  0013cdf6: mov r5,r0
  0013cdf8: ldr r0,[r6,#0x0]
  0013cdfa: blx r11
  0013cdfc: mov r1,r5
  0013cdfe: movs r2,#0x2
  0013ce00: blx r8
  0013ce02: ldr r0,[r4,#0x0]
  0013ce04: ldr r0,[r0,#0x4]
  0013ce06: ldr.w r0,[r0,#0x158]
  0013ce0a: blx r9
  0013ce0c: mov r5,r0
  0013ce0e: ldr r0,[r6,#0x0]
  0013ce10: blx r11
  0013ce12: mov r1,r5
  0013ce14: movs r2,#0x3
  0013ce16: blx r8
  0013ce18: ldr r0,[r4,#0x0]
  0013ce1a: ldr r0,[r0,#0x4]
  0013ce1c: ldr.w r0,[r0,#0x154]
  0013ce20: blx r9
  0013ce22: mov r5,r0
  0013ce24: ldr r0,[r6,#0x0]
  0013ce26: blx r11
  0013ce28: mov r1,r5
  0013ce2a: movs r2,#0x4
  0013ce2c: blx r8
  0013ce2e: ldr r0,[r4,#0x0]
  0013ce30: ldr r0,[r0,#0x4]
  0013ce32: ldr.w r0,[r0,#0xe0]
  0013ce36: blx r9
  0013ce38: mov r5,r0
  0013ce3a: ldr r0,[r6,#0x0]
  0013ce3c: blx r11
  0013ce3e: mov r1,r5
  0013ce40: movs r2,#0x5
  0013ce42: blx r8
  0013ce44: ldr r0,[r4,#0x0]
  0013ce46: movs r1,#0x8
  0013ce48: ldr r0,[r0,#0x4]
  0013ce4a: ldr.w r0,[r0,#0x1e8]
  0013ce4e: blx r10
  0013ce50: mov r5,r0
  0013ce52: ldr r0,[r6,#0x0]
  0013ce54: blx r11
  0013ce56: mov r1,r5
  0013ce58: blx 0x00072cdc
  0013ce5c: ldr r0,[r6,#0x0]
  0013ce5e: movs r1,#0x0
  0013ce60: blx 0x000758b0
  0013ce64: ldr r0,[0x0013cf68]
  0013ce66: movs r1,#0x46
  0013ce68: ldr r5,[r6,#0x0]
  0013ce6a: add r0,pc
  0013ce6c: ldr r0,[r0,#0x0]
  0013ce6e: ldr r0,[r0,#0x0]
  0013ce70: blx 0x00071c44
  0013ce74: mov r1,r0
  0013ce76: mov r0,r5
  0013ce78: blx 0x0007366c
  0013ce7c: ldr r0,[r6,#0x0]
  0013ce7e: movs r1,#0x6
  0013ce80: movs r2,#0x1
  0013ce82: blx 0x0007174c
  0013ce86: ldr r0,[r6,#0x0]
  0013ce88: movs r1,#0x19
  0013ce8a: movs r2,#0x1
  0013ce8c: blx 0x0007174c
  0013ce90: ldr r0,[r6,#0x0]
  0013ce92: movs r1,#0x0
  0013ce94: blx 0x000758b0
  0013ce98: ldr r0,[0x0013cf6c]
  0013ce9a: movw r2,#0x1a0a
  0013ce9e: ldr r1,[0x0013cf70]
  0013cea0: add r0,pc
  0013cea2: add r1,pc
  0013cea4: ldr r0,[r0,#0x0]
  0013cea6: ldr r5,[r1,#0x0]
  0013cea8: ldr r1,[r6,#0x0]
  0013ceaa: str.w r2,[r1,#0x84]
  0013ceae: mov.w r1,#0x1010101
  0013ceb2: str r1,[r0,#0x20]
  0013ceb4: movs r2,#0x1
  0013ceb6: ldr r1,[0x0013cf74]
  0013ceb8: strb r2,[r0,#0x17]
  0013ceba: add r1,pc
  0013cebc: ldr r4,[r1,#0x0]
  0013cebe: movs r1,#0x0
  0013cec0: strb.w r1,[r0,#0x36]
  0013cec4: str.w r1,[r0,#0x32]
  0013cec8: movw r1,#0x101
  0013cecc: strh r1,[r0,#0x8]
  0013cece: strb r2,[r0,#0xa]
  0013ced0: strb r2,[r0,#0x15]
  0013ced2: strb r2,[r0,#0x13]
  0013ced4: strh.w r1,[r0,#0xd]
  0013ced8: strb r2,[r0,#0xf]
  0013ceda: strb.w r2,[r0,#0x26]
  0013cede: strb.w r2,[r0,#0x31]
  0013cee2: strb.w r2,[r0,#0x24]
  0013cee6: strh r1,[r0,#0x1c]
  0013cee8: strb r2,[r0,#0x1e]
  0013ceea: movs r2,#0x1
  0013ceec: strh r1,[r0,#0x38]
  0013ceee: movs r1,#0x17
  0013cef0: ldr r0,[r5,#0x0]
  0013cef2: strb.w r2,[r4,#0x34]
  0013cef6: movs r2,#0x3
  0013cef8: blx 0x00076714
  0013cefc: ldr r0,[r5,#0x0]
  0013cefe: movs r1,#0x1e
  0013cf00: movs r2,#0x1
  0013cf02: blx 0x00076714
  0013cf06: ldr r0,[r6,#0x0]
  0013cf08: mov.w r1,#0x182
  0013cf0c: blx 0x00076720
  0013cf10: ldr r0,[0x0013cf78]
  0013cf12: add r0,pc
  0013cf14: ldr r0,[r0,#0x0]
  0013cf16: ldr r0,[r0,#0x0]
  0013cf18: blx 0x00071788
  0013cf1c: ldr r0,[0x0013cf7c]
  0013cf1e: add r0,pc
  0013cf20: ldr r0,[r0,#0x0]
  0013cf22: ldr r0,[r0,#0x0]
  0013cf24: ldr r1,[r0,#0x0]
  0013cf26: blx 0x000724a8
  0013cf2a: ldr r0,[0x0013cf80]
  0013cf2c: ldr r1,[sp,#0x0]
  0013cf2e: add r0,pc
  0013cf30: ldr r0,[r0,#0x0]
  0013cf32: ldr.w r1,[r1,#0x1a4]
  0013cf36: str r1,[r4,#0x2c]
  0013cf38: movs r1,#0x5
  0013cf3a: ldr r0,[r0,#0x0]
  0013cf3c: pop.w {r3,r8,r9,r10,r11}
  0013cf40: pop.w {r4,r5,r6,r7,lr}
  0013cf44: b.w 0x001ab8f8
```
