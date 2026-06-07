# Generator::getShipBuyList
elf 0x95bcc body 2122
Sig: undefined __stdcall getShipBuyList(Station * param_1)

## decompile
```c

/* Generator::getShipBuyList(Station*) */

Array<Ship*> * Generator::getShipBuyList(Station *param_1)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  Array<Ship*> *pAVar4;
  int iVar5;
  Ship *pSVar6;
  int *piVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  undefined4 uVar12;
  Station *in_r1;
  int iVar13;
  int iVar14;
  undefined4 *puVar15;
  int iVar16;
  uint uVar17;
  uint uVar18;
  int *piVar19;
  uint uVar20;
  uint uVar21;
  
  iVar3 = Station::getSystem(in_r1);
  if ((((iVar3 == 0xf) && (iVar3 = Status::getCurrentCampaignMission(), iVar3 < 0x10)) ||
      (iVar3 = Station::getIndex(in_r1), iVar3 == 0x65)) ||
     (iVar3 = Station::getIndex(in_r1), iVar3 == 0x6c)) {
    return (Array<Ship*> *)0x0;
  }
  puVar15 = *(undefined4 **)(DAT_000a5f7c + 0xa5c0c);
  iVar3 = Status::inSupernovaSystem();
  if (iVar3 != 0) {
    return (Array<Ship*> *)0x0;
  }
  iVar3 = Station::getIndex(in_r1);
  if ((iVar3 == 100) && (iVar3 = Status::dlc1Won(), iVar3 != 0)) {
    pAVar4 = operator_new(0xc);
    Array<Ship*>::Array(pAVar4);
    piVar19 = *(int **)(DAT_000a5f80 + 0xa5c4a);
    for (iVar3 = 0; iVar3 != 0x40; iVar3 = iVar3 + 1) {
      iVar16 = *(int *)(*(int *)(*piVar19 + 4) + iVar3 * 4);
      iVar5 = Ship::hasJumpDriveIntegrated();
      if ((iVar5 != 0) && (iVar5 = Ship::getIndex(), iVar5 != 0x25)) {
        pSVar6 = (Ship *)Ship::makeShip(iVar16);
        ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
        Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
        Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      }
    }
    return pAVar4;
  }
  iVar3 = Station::getIndex(in_r1);
  if (iVar3 == 0x6b) {
    pAVar4 = operator_new(0xc);
    Array<Ship*>::Array(pAVar4);
    iVar3 = DAT_000a5f88 + 0xa5ccc;
    piVar19 = *(int **)(DAT_000a5f8c + 0xa5cce);
    for (iVar5 = 0; iVar5 != 0x32; iVar5 = iVar5 + 1) {
      if (*(int *)(iVar3 + iVar5 * 4) == 8) {
        pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + iVar5 * 4));
        ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
        Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
        Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      }
    }
    if (*(char *)(*(int *)(DAT_000a5f90 + 0xa5d20) + 0x37) == '\0') {
      return pAVar4;
    }
    piVar7 = *(int **)*puVar15;
    if (piVar7 == (int *)0x0) {
      return pAVar4;
    }
    if (*piVar7 == 0) {
      return pAVar4;
    }
    iVar3 = Wanted::isTerminated(*(Wanted **)(piVar7[1] + 0x18));
    if (iVar3 != 0) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xb4));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    iVar3 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)*puVar15 + 4) + 0x30));
    if (iVar3 != 0) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xb8));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    iVar3 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)*puVar15 + 4) + 0x48));
    if (iVar3 != 0) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xbc));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    iVar3 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)*puVar15 + 4) + 0x60));
    if (iVar3 == 0) {
      return pAVar4;
    }
    pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xc0));
    ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
    iVar3 = *(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4;
    goto LAB_000a63f8;
  }
  Status::getSystem();
  iVar3 = SolarSystem::getRace();
  iVar5 = Station::getIndex(in_r1);
  if (iVar5 == 10) {
    uVar8 = Achievements::gotAllGoldMedals();
  }
  else {
    uVar8 = 0;
  }
  iVar5 = Station::getIndex(in_r1);
  iVar16 = Station::getIndex(in_r1);
  if (uVar8 == 0) {
    uVar17 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a5f98 + 0xa5e96));
    if (iVar5 == 0x29) {
      uVar17 = uVar17 + 1;
    }
    if (uVar17 == 0) {
      return (Array<Ship*> *)0x0;
    }
  }
  else {
    uVar17 = 1;
  }
  pAVar4 = operator_new(0xc);
  Array<Ship*>::Array(pAVar4);
  ArraySetLength<Ship*>(uVar17,(Array *)pAVar4);
  uVar9 = 10;
  if (uVar8 != 0) {
    uVar9 = 8;
  }
  piVar19 = *(int **)(DAT_000a5f9c + 0xa5ede);
  for (iVar14 = 0; iVar14 < (int)uVar17; iVar14 = iVar14 + 1) {
    bVar1 = iVar14 == 0;
    uVar18 = 0;
    uVar20 = uVar8 | (bVar1 && iVar5 == 0x29);
    uVar21 = uVar20;
    if (uVar20 != 0) {
      uVar21 = uVar9;
    }
LAB_000a5f0e:
    bVar2 = false;
    while (!bVar2) {
      uVar18 = uVar21;
      if (uVar20 == 0 && (iVar16 != 0x4e || !bVar1)) {
        uVar18 = Globals::getRandomEnemyFighter
                           ((Globals *)**(undefined4 **)(DAT_000a5fa0 + 0xa5f22),iVar3);
      }
      if (1 < (int)uVar17) {
        piVar7 = *(int **)(DAT_000a5fa4 + 0xa5f36);
        iVar10 = AbyssEngine::AERandom::nextInt(*piVar7);
        if ((iVar10 < 0x16) &&
           (iVar10 = AbyssEngine::AERandom::nextInt(*piVar7), uVar18 = uVar21,
           uVar20 == 0 && (iVar16 != 0x4e || !bVar1))) {
          iVar13 = iVar10;
          if (iVar10 == iVar3) {
            iVar13 = 8;
          }
          if (iVar10 == 4) {
            iVar13 = 8;
          }
          uVar18 = Globals::getRandomEnemyFighter
                             ((Globals *)**(undefined4 **)(DAT_000a5fa8 + 0xa5f56),iVar13);
        }
      }
      for (iVar10 = 0; iVar10 < (int)uVar17; iVar10 = iVar10 + 1) {
        if ((*(int *)(*(int *)(pAVar4 + 4) + iVar10 * 4) != 0) &&
           (uVar11 = Ship::getIndex(), uVar11 == uVar18)) goto LAB_000a5f0e;
      }
      bVar2 = true;
    }
    uVar12 = Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + uVar18 * 4));
    *(undefined4 *)(*(int *)(pAVar4 + 4) + iVar14 * 4) = uVar12;
    Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + iVar14 * 4));
    Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + iVar14 * 4));
  }
  if (iVar3 == 0) {
    iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a60d8 + 0xa6082));
    if (iVar3 == 0) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xf8));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    bVar1 = true;
    uVar8 = 0;
  }
  else {
    if (iVar3 == 1) {
      iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a646c + 0xa60e4));
      if (iVar3 != 0) {
        bVar1 = false;
        uVar8 = 1;
        goto LAB_000a60f8;
      }
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xfc));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      uVar8 = 1;
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    else {
      if ((iVar3 == 2) &&
         (iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a60d4 + 0xa602e)), iVar3 == 0)) {
        pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xf4));
        ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
        Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
        Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      }
      uVar8 = 0;
    }
    bVar1 = false;
  }
LAB_000a60f8:
  iVar3 = *(int *)(DAT_000a6470 + 0xa60fe);
  if ((*(char *)(iVar3 + 0x35) != '\0') && (uVar17 = Status::dlc1Won(), (uVar17 & uVar8) == 1)) {
    piVar7 = *(int **)(DAT_000a6474 + 0xa611e);
    iVar5 = AbyssEngine::AERandom::nextInt(*piVar7);
    if (iVar5 == 0) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0x9c));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    iVar5 = AbyssEngine::AERandom::nextInt(*piVar7);
    if (iVar5 == 0) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xa4));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
  }
  if (*(char *)(iVar3 + 0x37) != '\0') {
    if ((uVar8 != 0) &&
       (iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a6478 + 0xa61c2)), iVar3 == 0)) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xd8));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    iVar3 = Station::getIndex(in_r1);
    if (((iVar3 == 0x78) && (iVar3 = Status::getCurrentCampaignMission(), 0x9e < iVar3)) &&
       ((iVar3 = Status::hardCoreMode(), iVar3 != 0 ||
        ((iVar3 = Status::hardCoreMode(), iVar3 == 0 &&
         (iVar3 = Achievements::gotAllSupernovaMedals
                            ((Achievements *)**(undefined4 **)(DAT_000a647c + 0xa622e)), iVar3 != 0)
         ))))) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xb0));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    iVar3 = Station::getIndex(in_r1);
    if ((iVar3 == 0x78) && (iVar3 = Status::getCurrentCampaignMission(), 0x9e < iVar3)) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xc4));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
    if ((bVar1) &&
       (iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a6480 + 0xa62cc)), iVar3 == 0)) {
      pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xcc));
      ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
      Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    }
  }
  iVar3 = Station::getSystem(in_r1);
  if ((iVar3 == 0x11) &&
     (iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a6484 + 0xa6320)), iVar3 == 0)) {
    pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xa8));
    ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
    Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
  }
  iVar3 = Station::getSystem(in_r1);
  if ((iVar3 == 0x11) &&
     (iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a6488 + 0xa6374)), iVar3 == 0)) {
    pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xac));
    ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
    Ship::setRace(*(int *)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
    Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
  }
  iVar3 = Station::getSystem(in_r1);
  if (iVar3 != 0x11) {
    return pAVar4;
  }
  iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000a648c + 0xa63ca));
  if (iVar3 != 0) {
    return pAVar4;
  }
  pSVar6 = (Ship *)Ship::makeShip(*(int *)(*(int *)(*piVar19 + 4) + 0xd0));
  ArrayAdd<Ship*>(pSVar6,(Array *)pAVar4);
  iVar3 = *(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4;
LAB_000a63f8:
  Ship::setRace(*(int *)(iVar3 + -4));
  Ship::adjustPrice(*(Ship **)(*(int *)(pAVar4 + 4) + *(int *)pAVar4 * 4 + -4));
  return pAVar4;
}

```

## target disasm
```
  000a5bcc: push {r4,r5,r6,r7,lr}
  000a5bce: add r7,sp,#0xc
  000a5bd0: push {r8,r9,r10,r11}
  000a5bd4: sub sp,#0x24
  000a5bd6: mov r0,r1
  000a5bd8: mov r4,r1
  000a5bda: blx 0x0007189c
  000a5bde: cmp r0,#0xf
  000a5be0: bne 0x000a5bf2
  000a5be2: ldr r0,[0x000a5f78]
  000a5be4: add r0,pc
  000a5be6: ldr r0,[r0,#0x0]
  000a5be8: ldr r0,[r0,#0x0]
  000a5bea: blx 0x00071770
  000a5bee: cmp r0,#0x10
  000a5bf0: blt 0x000a5c14
  000a5bf2: mov r0,r4
  000a5bf4: blx 0x00071824
  000a5bf8: cmp r0,#0x65
  000a5bfa: beq 0x000a5c14
  000a5bfc: mov r0,r4
  000a5bfe: blx 0x00071824
  000a5c02: cmp r0,#0x6c
  000a5c04: beq 0x000a5c14
  000a5c06: ldr r0,[0x000a5f7c]
  000a5c08: add r0,pc
  000a5c0a: ldr r5,[r0,#0x0]
  000a5c0c: ldr r0,[r5,#0x0]
  000a5c0e: blx 0x00071878
  000a5c12: cbz r0,0x000a5c22
  000a5c14: mov.w r11,#0x0
  000a5c18: mov r0,r11
  000a5c1a: add sp,#0x24
  000a5c1c: pop.w {r8,r9,r10,r11}
  000a5c20: pop {r4,r5,r6,r7,pc}
  000a5c22: mov r0,r4
  000a5c24: blx 0x00071824
  000a5c28: cmp r0,#0x64
  000a5c2a: bne 0x000a5ca8
  000a5c2c: ldr r0,[r5,#0x0]
  000a5c2e: blx 0x000719a4
  000a5c32: cbz r0,0x000a5ca8
  000a5c34: movs r0,#0xc
  000a5c36: blx 0x0006eb24
  000a5c3a: mov r11,r0
  000a5c3c: blx 0x000719b0
  000a5c40: ldr r0,[0x000a5f80]
  000a5c42: movs r6,#0x0
  000a5c44: ldr r4,[0x000a5f84]
  000a5c46: add r0,pc
  000a5c48: add r4,pc
  000a5c4a: ldr.w r8,[r0,#0x0]
  000a5c4e: b 0x000a5ca2
  000a5c50: ldr.w r0,[r8,#0x0]
  000a5c54: ldr r0,[r0,#0x4]
  000a5c56: ldr.w r5,[r0,r6,lsl #0x2]
  000a5c5a: mov r0,r5
  000a5c5c: blx 0x000719bc
  000a5c60: cbz r0,0x000a5ca0
  000a5c62: mov r0,r5
  000a5c64: blx 0x000719c8
  000a5c68: cmp r0,#0x25
  000a5c6a: beq 0x000a5ca0
  000a5c6c: mov r0,r5
  000a5c6e: mov.w r1,#0xffffffff
  000a5c72: blx 0x000719d4
  000a5c76: mov r1,r11
  000a5c78: blx 0x000719e0
  000a5c7c: ldrd r0,r2,[r11,#0x0]
  000a5c80: ldr.w r1,[r4,r6,lsl #0x2]
  000a5c84: add.w r0,r2,r0, lsl #0x2
  000a5c88: ldr.w r0,[r0,#-0x4]
  000a5c8c: blx 0x000719ec
  000a5c90: ldrd r0,r1,[r11,#0x0]
  000a5c94: add.w r0,r1,r0, lsl #0x2
  000a5c98: ldr.w r0,[r0,#-0x4]
  000a5c9c: blx 0x000719f8
  000a5ca0: adds r6,#0x1
  000a5ca2: cmp r6,#0x40
  000a5ca4: bne 0x000a5c50
  000a5ca6: b 0x000a5c18
  000a5ca8: mov r0,r4
  000a5caa: blx 0x00071824
  000a5cae: cmp r0,#0x6b
  000a5cb0: bne.w 0x000a5e46
  000a5cb4: movs r0,#0xc
  000a5cb6: mov r4,r5
  000a5cb8: blx 0x0006eb24
  000a5cbc: mov r11,r0
  000a5cbe: blx 0x000719b0
  000a5cc2: ldr r5,[0x000a5f88]
  000a5cc4: movs r6,#0x0
  000a5cc6: ldr r0,[0x000a5f8c]
  000a5cc8: add r5,pc
  000a5cca: add r0,pc
  000a5ccc: ldr.w r8,[r0,#0x0]
  000a5cd0: b 0x000a5d16
  000a5cd2: ldr.w r0,[r5,r6,lsl #0x2]
  000a5cd6: cmp r0,#0x8
  000a5cd8: bne 0x000a5d14
  000a5cda: ldr.w r0,[r8,#0x0]
  000a5cde: mov.w r1,#0xffffffff
  000a5ce2: ldr r0,[r0,#0x4]
  000a5ce4: ldr.w r0,[r0,r6,lsl #0x2]
  000a5ce8: blx 0x000719d4
  000a5cec: mov r1,r11
  000a5cee: blx 0x000719e0
  000a5cf2: ldrd r0,r1,[r11,#0x0]
  000a5cf6: add.w r0,r1,r0, lsl #0x2
  000a5cfa: movs r1,#0x8
  000a5cfc: ldr.w r0,[r0,#-0x4]
  000a5d00: blx 0x000719ec
  000a5d04: ldrd r0,r1,[r11,#0x0]
  000a5d08: add.w r0,r1,r0, lsl #0x2
  000a5d0c: ldr.w r0,[r0,#-0x4]
  000a5d10: blx 0x000719f8
  000a5d14: adds r6,#0x1
  000a5d16: cmp r6,#0x32
  000a5d18: bne 0x000a5cd2
  000a5d1a: ldr r0,[0x000a5f90]
  000a5d1c: add r0,pc
  000a5d1e: ldr r0,[r0,#0x0]
  000a5d20: ldrb.w r0,[r0,#0x37]
  000a5d24: cmp r0,#0x0
  000a5d26: beq.w 0x000a5c18
  000a5d2a: ldr r0,[r4,#0x0]
  000a5d2c: ldr r0,[r0,#0x0]
  000a5d2e: cmp r0,#0x0
  000a5d30: beq.w 0x000a5c18
  000a5d34: ldr r1,[r0,#0x0]
  000a5d36: cmp r1,#0x0
  000a5d38: beq.w 0x000a5c18
  000a5d3c: ldr r0,[r0,#0x4]
  000a5d3e: ldr r0,[r0,#0x18]
  000a5d40: blx 0x00071a04
  000a5d44: cbz r0,0x000a5d80
  000a5d46: ldr.w r0,[r8,#0x0]
  000a5d4a: mov.w r1,#0xffffffff
  000a5d4e: ldr r0,[r0,#0x4]
  000a5d50: ldr.w r0,[r0,#0xb4]
  000a5d54: blx 0x000719d4
  000a5d58: mov r1,r11
  000a5d5a: blx 0x000719e0
  000a5d5e: ldrd r0,r1,[r11,#0x0]
  000a5d62: add.w r0,r1,r0, lsl #0x2
  000a5d66: movs r1,#0x8
  000a5d68: ldr.w r0,[r0,#-0x4]
  000a5d6c: blx 0x000719ec
  000a5d70: ldrd r0,r1,[r11,#0x0]
  000a5d74: add.w r0,r1,r0, lsl #0x2
  000a5d78: ldr.w r0,[r0,#-0x4]
  000a5d7c: blx 0x000719f8
  000a5d80: ldr r0,[r4,#0x0]
  000a5d82: ldr r0,[r0,#0x0]
  000a5d84: ldr r0,[r0,#0x4]
  000a5d86: ldr r0,[r0,#0x30]
  000a5d88: blx 0x00071a04
  000a5d8c: cbz r0,0x000a5dc8
  000a5d8e: ldr.w r0,[r8,#0x0]
  000a5d92: mov.w r1,#0xffffffff
  000a5d96: ldr r0,[r0,#0x4]
  000a5d98: ldr.w r0,[r0,#0xb8]
  000a5d9c: blx 0x000719d4
  000a5da0: mov r1,r11
  000a5da2: blx 0x000719e0
  000a5da6: ldrd r0,r1,[r11,#0x0]
  000a5daa: add.w r0,r1,r0, lsl #0x2
  000a5dae: movs r1,#0x8
  000a5db0: ldr.w r0,[r0,#-0x4]
  000a5db4: blx 0x000719ec
  000a5db8: ldrd r0,r1,[r11,#0x0]
  000a5dbc: add.w r0,r1,r0, lsl #0x2
  000a5dc0: ldr.w r0,[r0,#-0x4]
  000a5dc4: blx 0x000719f8
  000a5dc8: ldr r0,[r4,#0x0]
  000a5dca: ldr r0,[r0,#0x0]
  000a5dcc: ldr r0,[r0,#0x4]
  000a5dce: ldr r0,[r0,#0x48]
  000a5dd0: blx 0x00071a04
  000a5dd4: cbz r0,0x000a5e10
  000a5dd6: ldr.w r0,[r8,#0x0]
  000a5dda: mov.w r1,#0xffffffff
  000a5dde: ldr r0,[r0,#0x4]
  000a5de0: ldr.w r0,[r0,#0xbc]
  000a5de4: blx 0x000719d4
  000a5de8: mov r1,r11
  000a5dea: blx 0x000719e0
  000a5dee: ldrd r0,r1,[r11,#0x0]
  000a5df2: add.w r0,r1,r0, lsl #0x2
  000a5df6: movs r1,#0x8
  000a5df8: ldr.w r0,[r0,#-0x4]
  000a5dfc: blx 0x000719ec
  000a5e00: ldrd r0,r1,[r11,#0x0]
  000a5e04: add.w r0,r1,r0, lsl #0x2
  000a5e08: ldr.w r0,[r0,#-0x4]
  000a5e0c: blx 0x000719f8
  000a5e10: ldr r0,[r4,#0x0]
  000a5e12: ldr r0,[r0,#0x0]
  000a5e14: ldr r0,[r0,#0x4]
  000a5e16: ldr r0,[r0,#0x60]
  000a5e18: blx 0x00071a04
  000a5e1c: cmp r0,#0x0
  000a5e1e: beq.w 0x000a5c18
  000a5e22: ldr.w r0,[r8,#0x0]
  000a5e26: mov.w r1,#0xffffffff
  000a5e2a: ldr r0,[r0,#0x4]
  000a5e2c: ldr.w r0,[r0,#0xc0]
  000a5e30: blx 0x000719d4
  000a5e34: mov r1,r11
  000a5e36: blx 0x000719e0
  000a5e3a: ldrd r0,r1,[r11,#0x0]
  000a5e3e: add.w r0,r1,r0, lsl #0x2
  000a5e42: movs r1,#0x8
  000a5e44: b 0x000a63f8
  000a5e46: ldr r0,[r5,#0x0]
  000a5e48: blx 0x00071a10
  000a5e4c: blx 0x00071950
  000a5e50: str r0,[sp,#0x20]
  000a5e52: mov r0,r4
  000a5e54: blx 0x00071824
  000a5e58: cmp r0,#0xa
  000a5e5a: str r5,[sp,#0x0]
  000a5e5c: bne 0x000a5e6e
  000a5e5e: ldr r0,[0x000a5f94]
  000a5e60: add r0,pc
  000a5e62: ldr r0,[r0,#0x0]
  000a5e64: ldr r0,[r0,#0x0]
  000a5e66: blx 0x00071a1c
  000a5e6a: mov r8,r0
  000a5e6c: b 0x000a5e72
  000a5e6e: mov.w r8,#0x0
  000a5e72: mov r0,r4
  000a5e74: blx 0x00071824
  000a5e78: mov r6,r0
  000a5e7a: mov r0,r4
  000a5e7c: str r4,[sp,#0x4]
  000a5e7e: blx 0x00071824
  000a5e82: mov r9,r0
  000a5e84: cmp.w r8,#0x0
  000a5e88: beq 0x000a5e8e
  000a5e8a: movs r5,#0x1
  000a5e8c: b 0x000a5eaa
  000a5e8e: ldr r0,[0x000a5f98]
  000a5e90: movs r1,#0x6
  000a5e92: add r0,pc
  000a5e94: ldr r0,[r0,#0x0]
  000a5e96: ldr r0,[r0,#0x0]
  000a5e98: blx 0x00071848
  000a5e9c: mov r5,r0
  000a5e9e: cmp r6,#0x29
  000a5ea0: it eq
  000a5ea2: add.eq r5,#0x1
  000a5ea4: cmp r5,#0x0
  000a5ea6: beq.w 0x000a5c14
  000a5eaa: movs r0,#0xc
  000a5eac: blx 0x0006eb24
  000a5eb0: mov r11,r0
  000a5eb2: blx 0x000719b0
  000a5eb6: mov r0,r5
  000a5eb8: mov r1,r11
  000a5eba: blx 0x00071a28
  000a5ebe: movs r0,#0xa
  000a5ec0: cmp.w r8,#0x0
  000a5ec4: it ne
  000a5ec6: mov.ne r0,#0x8
  000a5ec8: movs r4,#0x0
  000a5eca: str r0,[sp,#0xc]
  000a5ecc: sub.w r0,r9,#0x4e
  000a5ed0: clz r0,r0
  000a5ed4: lsrs r0,r0,#0x5
  000a5ed6: str r0,[sp,#0x8]
  000a5ed8: ldr r0,[0x000a5f9c]
  000a5eda: add r0,pc
  000a5edc: ldr r0,[r0,#0x0]
  000a5ede: str r0,[sp,#0x18]
  000a5ee0: strd r6,r8,[sp,#0x10]
  000a5ee4: b 0x000a6012
  000a5ee6: sub.w r2,r6,#0x29
  000a5eea: clz r0,r4
  000a5eee: clz r2,r2
  000a5ef2: ldr r1,[sp,#0x8]
  000a5ef4: lsrs r0,r0,#0x5
  000a5ef6: str r4,[sp,#0x1c]
  000a5ef8: ands r1,r0
  000a5efa: lsrs r2,r2,#0x5
  000a5efc: ands r0,r2
  000a5efe: movs r6,#0x0
  000a5f00: orrs.w r10,r8,r0
  000a5f04: ldr r0,[sp,#0xc]
  000a5f06: orr.w r8,r10,r1
  000a5f0a: it ne
  000a5f0c: mov.ne r10,r0
  000a5f0e: movs r0,#0x0
  000a5f10: b 0x000a5fcc
  000a5f12: cmp.w r8,#0x0
  000a5f16: mov r6,r10
  000a5f18: bne 0x000a5f2a
  000a5f1a: ldr r0,[0x000a5fa0]
  000a5f1c: ldr r1,[sp,#0x20]
  000a5f1e: add r0,pc
  000a5f20: ldr r0,[r0,#0x0]
  000a5f22: ldr r0,[r0,#0x0]
  000a5f24: blx 0x00071a34
  000a5f28: mov r6,r0
  000a5f2a: cmp r5,#0x2
  000a5f2c: blt 0x000a5f70
  000a5f2e: ldr r0,[0x000a5fa4]
  000a5f30: movs r1,#0x64
  000a5f32: add r0,pc
  000a5f34: ldr r4,[r0,#0x0]
  000a5f36: ldr r0,[r4,#0x0]
  000a5f38: blx 0x00071848
  000a5f3c: cmp r0,#0x15
  000a5f3e: bgt 0x000a5f70
  000a5f40: ldr r0,[r4,#0x0]
  000a5f42: movs r1,#0x5
  000a5f44: blx 0x00071848
  000a5f48: cmp.w r8,#0x0
  000a5f4c: mov r6,r10
  000a5f4e: bne 0x000a5f70
  000a5f50: ldr r1,[0x000a5fa8]
  000a5f52: add r1,pc
  000a5f54: ldr r2,[r1,#0x0]
  000a5f56: ldr r1,[sp,#0x20]
  000a5f58: cmp r0,r1
  000a5f5a: mov r1,r0
  000a5f5c: it eq
  000a5f5e: mov.eq r1,#0x8
  000a5f60: ldr r2,[r2,#0x0]
  000a5f62: cmp r0,#0x4
  000a5f64: it eq
  000a5f66: mov.eq r1,#0x8
  000a5f68: mov r0,r2
  000a5f6a: blx 0x00071a34
  000a5f6e: mov r6,r0
  000a5f70: mov.w r9,#0x0
  000a5f74: b 0x000a5fb0
  000a5fac: add.w r9,r9,#0x1
  000a5fb0: cmp r9,r5
  000a5fb2: bge 0x000a5fca
  000a5fb4: ldr.w r0,[r11,#0x4]
  000a5fb8: ldr.w r0,[r0,r9,lsl #0x2]
  000a5fbc: cmp r0,#0x0
  000a5fbe: beq 0x000a5fac
  000a5fc0: blx 0x000719c8
  000a5fc4: cmp r0,r6
  000a5fc6: bne 0x000a5fac
  000a5fc8: b 0x000a5f0e
  000a5fca: movs r0,#0x1
  000a5fcc: lsls r0,r0,#0x1f
  000a5fce: beq 0x000a5f12
  000a5fd0: ldr r0,[sp,#0x18]
  000a5fd2: mov.w r1,#0xffffffff
  000a5fd6: ldr r0,[r0,#0x0]
  000a5fd8: ldr r0,[r0,#0x4]
  000a5fda: ldr.w r0,[r0,r6,lsl #0x2]
  000a5fde: blx 0x000719d4
  000a5fe2: ldr.w r1,[r11,#0x4]
  000a5fe6: ldr r2,[0x000a60d0]
  000a5fe8: ldr r4,[sp,#0x1c]
  000a5fea: add r2,pc
  000a5fec: str.w r0,[r1,r4,lsl #0x2]
  000a5ff0: ldr.w r0,[r11,#0x4]
  000a5ff4: ldr.w r1,[r2,r6,lsl #0x2]
  000a5ff8: ldr.w r0,[r0,r4,lsl #0x2]
  000a5ffc: blx 0x000719ec
  000a6000: ldr.w r0,[r11,#0x4]
  000a6004: ldr.w r0,[r0,r4,lsl #0x2]
  000a6008: blx 0x000719f8
  000a600c: adds r4,#0x1
  000a600e: ldrd r6,r8,[sp,#0x10]
  000a6012: cmp r4,r5
  000a6014: blt.w 0x000a5ee6
  000a6018: ldr r0,[sp,#0x20]
  000a601a: cbz r0,0x000a607a
  000a601c: ldr r6,[sp,#0x0]
  000a601e: cmp r0,#0x1
  000a6020: beq 0x000a60dc
  000a6022: cmp r0,#0x2
  000a6024: bne 0x000a6070
  000a6026: ldr r0,[0x000a60d4]
  000a6028: movs r1,#0x8
  000a602a: add r0,pc
  000a602c: ldr r0,[r0,#0x0]
  000a602e: ldr r0,[r0,#0x0]
  000a6030: blx 0x00071848
  000a6034: cbnz r0,0x000a6070
  000a6036: ldr r0,[sp,#0x18]
  000a6038: mov.w r1,#0xffffffff
  000a603c: ldr r0,[r0,#0x0]
  000a603e: ldr r0,[r0,#0x4]
  000a6040: ldr.w r0,[r0,#0xf4]
  000a6044: blx 0x000719d4
  000a6048: mov r1,r11
  000a604a: blx 0x000719e0
  000a604e: ldrd r0,r1,[r11,#0x0]
  000a6052: add.w r0,r1,r0, lsl #0x2
  000a6056: movs r1,#0x1
  000a6058: ldr.w r0,[r0,#-0x4]
  000a605c: blx 0x000719ec
  000a6060: ldrd r0,r1,[r11,#0x0]
  000a6064: add.w r0,r1,r0, lsl #0x2
  000a6068: ldr.w r0,[r0,#-0x4]
  000a606c: blx 0x000719f8
  000a6070: mov.w r9,#0x0
  000a6074: mov.w r8,#0x0
  000a6078: b 0x000a60f8
  000a607a: ldr r0,[0x000a60d8]
  000a607c: movs r1,#0x7
  000a607e: add r0,pc
  000a6080: ldr r0,[r0,#0x0]
  000a6082: ldr r0,[r0,#0x0]
  000a6084: blx 0x00071848
  000a6088: ldr r6,[sp,#0x0]
  000a608a: cbnz r0,0x000a60c6
  000a608c: ldr r0,[sp,#0x18]
  000a608e: mov.w r1,#0xffffffff
  000a6092: ldr r0,[r0,#0x0]
  000a6094: ldr r0,[r0,#0x4]
  000a6096: ldr.w r0,[r0,#0xf8]
  000a609a: blx 0x000719d4
  000a609e: mov r1,r11
  000a60a0: blx 0x000719e0
  000a60a4: ldrd r0,r1,[r11,#0x0]
  000a60a8: add.w r0,r1,r0, lsl #0x2
  000a60ac: movs r1,#0x3
  000a60ae: ldr.w r0,[r0,#-0x4]
  000a60b2: blx 0x000719ec
  000a60b6: ldrd r0,r1,[r11,#0x0]
  000a60ba: add.w r0,r1,r0, lsl #0x2
  000a60be: ldr.w r0,[r0,#-0x4]
  000a60c2: blx 0x000719f8
  000a60c6: mov.w r8,#0x1
  000a60ca: mov.w r9,#0x0
  000a60ce: b 0x000a60f8
  000a60dc: ldr r0,[0x000a646c]
  000a60de: movs r1,#0x5
  000a60e0: add r0,pc
  000a60e2: ldr r0,[r0,#0x0]
  000a60e4: ldr r0,[r0,#0x0]
  000a60e6: blx 0x00071848
  000a60ea: cmp r0,#0x0
  000a60ec: beq.w 0x000a6418
  000a60f0: mov.w r8,#0x0
  000a60f4: mov.w r9,#0x1
  000a60f8: ldr r0,[0x000a6470]
  000a60fa: add r0,pc
  000a60fc: ldr r5,[r0,#0x0]
  000a60fe: ldrb.w r0,[r5,#0x35]
  000a6102: cmp r0,#0x0
  000a6104: beq 0x000a61a8
  000a6106: ldr r0,[r6,#0x0]
  000a6108: blx 0x000719a4
  000a610c: and.w r0,r0,r9
  000a6110: cmp r0,#0x1
  000a6112: bne 0x000a61a8
  000a6114: ldr r0,[0x000a6474]
  000a6116: mov r4,r6
  000a6118: movs r1,#0x2
  000a611a: add r0,pc
  000a611c: ldr r6,[r0,#0x0]
  000a611e: ldr r0,[r6,#0x0]
  000a6120: blx 0x00071848
  000a6124: cbnz r0,0x000a6160
  000a6126: ldr r0,[sp,#0x18]
  000a6128: mov.w r1,#0xffffffff
  000a612c: ldr r0,[r0,#0x0]
  000a612e: ldr r0,[r0,#0x4]
  000a6130: ldr.w r0,[r0,#0x9c]
  000a6134: blx 0x000719d4
  000a6138: mov r1,r11
  000a613a: blx 0x000719e0
  000a613e: ldrd r0,r1,[r11,#0x0]
  000a6142: add.w r0,r1,r0, lsl #0x2
  000a6146: movs r1,#0x1
  000a6148: ldr.w r0,[r0,#-0x4]
  000a614c: blx 0x000719ec
  000a6150: ldrd r0,r1,[r11,#0x0]
  000a6154: add.w r0,r1,r0, lsl #0x2
  000a6158: ldr.w r0,[r0,#-0x4]
  000a615c: blx 0x000719f8
  000a6160: ldr r0,[r6,#0x0]
  000a6162: movs r1,#0x2
  000a6164: blx 0x00071848
  000a6168: cmp r0,#0x0
  000a616a: mov r6,r4
  000a616c: bne 0x000a61a8
  000a616e: ldr r0,[sp,#0x18]
  000a6170: mov.w r1,#0xffffffff
  000a6174: ldr r0,[r0,#0x0]
  000a6176: ldr r0,[r0,#0x4]
  000a6178: ldr.w r0,[r0,#0xa4]
  000a617c: blx 0x000719d4
  000a6180: mov r1,r11
  000a6182: blx 0x000719e0
  000a6186: ldrd r0,r1,[r11,#0x0]
  000a618a: add.w r0,r1,r0, lsl #0x2
  000a618e: movs r1,#0x1
  000a6190: ldr.w r0,[r0,#-0x4]
  000a6194: blx 0x000719ec
  000a6198: ldrd r0,r1,[r11,#0x0]
  000a619c: add.w r0,r1,r0, lsl #0x2
  000a61a0: ldr.w r0,[r0,#-0x4]
  000a61a4: blx 0x000719f8
  000a61a8: ldrb.w r0,[r5,#0x37]
  000a61ac: ldr r4,[sp,#0x4]
  000a61ae: cmp r0,#0x0
  000a61b0: beq.w 0x000a630e
  000a61b4: cmp.w r9,#0x0
  000a61b8: beq 0x000a6204
  000a61ba: ldr r0,[0x000a6478]
  000a61bc: movs r1,#0x4
  000a61be: add r0,pc
  000a61c0: ldr r0,[r0,#0x0]
  000a61c2: ldr r0,[r0,#0x0]
  000a61c4: blx 0x00071848
  000a61c8: cbnz r0,0x000a6204
  000a61ca: ldr r0,[sp,#0x18]
  000a61cc: mov.w r1,#0xffffffff
  000a61d0: ldr r0,[r0,#0x0]
  000a61d2: ldr r0,[r0,#0x4]
  000a61d4: ldr.w r0,[r0,#0xd8]
  000a61d8: blx 0x000719d4
  000a61dc: mov r1,r11
  000a61de: blx 0x000719e0
  000a61e2: ldrd r0,r1,[r11,#0x0]
  000a61e6: add.w r0,r1,r0, lsl #0x2
  000a61ea: movs r1,#0x1
  000a61ec: ldr.w r0,[r0,#-0x4]
  000a61f0: blx 0x000719ec
  000a61f4: ldrd r0,r1,[r11,#0x0]
  000a61f8: add.w r0,r1,r0, lsl #0x2
  000a61fc: ldr.w r0,[r0,#-0x4]
  000a6200: blx 0x000719f8
  000a6204: mov r0,r4
  000a6206: blx 0x00071824
  000a620a: cmp r0,#0x78
  000a620c: bne 0x000a6270
  000a620e: ldr r0,[r6,#0x0]
  000a6210: blx 0x00071770
  000a6214: cmp r0,#0x9f
  000a6216: blt 0x000a6270
  000a6218: ldr r0,[r6,#0x0]
  000a621a: blx 0x0007192c
  000a621e: cbnz r0,0x000a6236
  000a6220: ldr r0,[r6,#0x0]
  000a6222: blx 0x0007192c
  000a6226: cbnz r0,0x000a6270
  000a6228: ldr r0,[0x000a647c]
  000a622a: add r0,pc
  000a622c: ldr r0,[r0,#0x0]
  000a622e: ldr r0,[r0,#0x0]
  000a6230: blx 0x00071a40
  000a6234: cbz r0,0x000a6270
  000a6236: ldr r0,[sp,#0x18]
  000a6238: mov.w r1,#0xffffffff
  000a623c: ldr r0,[r0,#0x0]
  000a623e: ldr r0,[r0,#0x4]
  000a6240: ldr.w r0,[r0,#0xb0]
  000a6244: blx 0x000719d4
  000a6248: mov r1,r11
  000a624a: blx 0x000719e0
  000a624e: ldrd r0,r1,[r11,#0x0]
  000a6252: add.w r0,r1,r0, lsl #0x2
  000a6256: movs r1,#0x1
  000a6258: ldr.w r0,[r0,#-0x4]
  000a625c: blx 0x000719ec
  000a6260: ldrd r0,r1,[r11,#0x0]
  000a6264: add.w r0,r1,r0, lsl #0x2
  000a6268: ldr.w r0,[r0,#-0x4]
  000a626c: blx 0x000719f8
  000a6270: mov r0,r4
  000a6272: blx 0x00071824
  000a6276: cmp r0,#0x78
  000a6278: bne 0x000a62be
  000a627a: ldr r0,[r6,#0x0]
  000a627c: blx 0x00071770
  000a6280: cmp r0,#0x9f
  000a6282: blt 0x000a62be
  000a6284: ldr r0,[sp,#0x18]
  000a6286: mov.w r1,#0xffffffff
  000a628a: ldr r0,[r0,#0x0]
  000a628c: ldr r0,[r0,#0x4]
  000a628e: ldr.w r0,[r0,#0xc4]
  000a6292: blx 0x000719d4
  000a6296: mov r1,r11
  000a6298: blx 0x000719e0
  000a629c: ldrd r0,r1,[r11,#0x0]
  000a62a0: add.w r0,r1,r0, lsl #0x2
  000a62a4: movs r1,#0x1
  000a62a6: ldr.w r0,[r0,#-0x4]
  000a62aa: blx 0x000719ec
  000a62ae: ldrd r0,r1,[r11,#0x0]
  000a62b2: add.w r0,r1,r0, lsl #0x2
  000a62b6: ldr.w r0,[r0,#-0x4]
  000a62ba: blx 0x000719f8
  000a62be: cmp.w r8,#0x0
  000a62c2: beq 0x000a630e
  000a62c4: ldr r0,[0x000a6480]
  000a62c6: movs r1,#0x8
  000a62c8: add r0,pc
  000a62ca: ldr r0,[r0,#0x0]
  000a62cc: ldr r0,[r0,#0x0]
  000a62ce: blx 0x00071848
  000a62d2: cbnz r0,0x000a630e
  000a62d4: ldr r0,[sp,#0x18]
  000a62d6: mov.w r1,#0xffffffff
  000a62da: ldr r0,[r0,#0x0]
  000a62dc: ldr r0,[r0,#0x4]
  000a62de: ldr.w r0,[r0,#0xcc]
  000a62e2: blx 0x000719d4
  000a62e6: mov r1,r11
  000a62e8: blx 0x000719e0
  000a62ec: ldrd r0,r1,[r11,#0x0]
  000a62f0: add.w r0,r1,r0, lsl #0x2
  000a62f4: movs r1,#0x0
  000a62f6: ldr.w r0,[r0,#-0x4]
  000a62fa: blx 0x000719ec
  000a62fe: ldrd r0,r1,[r11,#0x0]
  000a6302: add.w r0,r1,r0, lsl #0x2
  000a6306: ldr.w r0,[r0,#-0x4]
  000a630a: blx 0x000719f8
  000a630e: mov r0,r4
  000a6310: blx 0x0007189c
  000a6314: cmp r0,#0x11
  000a6316: bne 0x000a6362
  000a6318: ldr r0,[0x000a6484]
  000a631a: movs r1,#0x3
  000a631c: add r0,pc
  000a631e: ldr r0,[r0,#0x0]
  000a6320: ldr r0,[r0,#0x0]
  000a6322: blx 0x00071848
  000a6326: cbnz r0,0x000a6362
  000a6328: ldr r0,[sp,#0x18]
  000a632a: mov.w r1,#0xffffffff
  000a632e: ldr r0,[r0,#0x0]
  000a6330: ldr r0,[r0,#0x4]
  000a6332: ldr.w r0,[r0,#0xa8]
  000a6336: blx 0x000719d4
  000a633a: mov r1,r11
  000a633c: blx 0x000719e0
  000a6340: ldrd r0,r1,[r11,#0x0]
  000a6344: add.w r0,r1,r0, lsl #0x2
  000a6348: movs r1,#0x1
  000a634a: ldr.w r0,[r0,#-0x4]
  000a634e: blx 0x000719ec
  000a6352: ldrd r0,r1,[r11,#0x0]
  000a6356: add.w r0,r1,r0, lsl #0x2
  000a635a: ldr.w r0,[r0,#-0x4]
  000a635e: blx 0x000719f8
  000a6362: mov r0,r4
  000a6364: blx 0x0007189c
  000a6368: cmp r0,#0x11
  000a636a: bne 0x000a63b6
  000a636c: ldr r0,[0x000a6488]
  000a636e: movs r1,#0x3
  000a6370: add r0,pc
  000a6372: ldr r0,[r0,#0x0]
  000a6374: ldr r0,[r0,#0x0]
  000a6376: blx 0x00071848
  000a637a: cbnz r0,0x000a63b6
  000a637c: ldr r0,[sp,#0x18]
  000a637e: mov.w r1,#0xffffffff
  000a6382: ldr r0,[r0,#0x0]
  000a6384: ldr r0,[r0,#0x4]
  000a6386: ldr.w r0,[r0,#0xac]
  000a638a: blx 0x000719d4
  000a638e: mov r1,r11
  000a6390: blx 0x000719e0
  000a6394: ldrd r0,r1,[r11,#0x0]
  000a6398: add.w r0,r1,r0, lsl #0x2
  000a639c: movs r1,#0x2
  000a639e: ldr.w r0,[r0,#-0x4]
  000a63a2: blx 0x000719ec
  000a63a6: ldrd r0,r1,[r11,#0x0]
  000a63aa: add.w r0,r1,r0, lsl #0x2
  000a63ae: ldr.w r0,[r0,#-0x4]
  000a63b2: blx 0x000719f8
  000a63b6: mov r0,r4
  000a63b8: blx 0x0007189c
  000a63bc: cmp r0,#0x11
  000a63be: bne.w 0x000a5c18
  000a63c2: ldr r0,[0x000a648c]
  000a63c4: movs r1,#0x3
  000a63c6: add r0,pc
  000a63c8: ldr r0,[r0,#0x0]
  000a63ca: ldr r0,[r0,#0x0]
  000a63cc: blx 0x00071848
  000a63d0: cmp r0,#0x0
  000a63d2: bne.w 0x000a5c18
  000a63d6: ldr r0,[sp,#0x18]
  000a63d8: mov.w r1,#0xffffffff
  000a63dc: ldr r0,[r0,#0x0]
  000a63de: ldr r0,[r0,#0x4]
  000a63e0: ldr.w r0,[r0,#0xd0]
  000a63e4: blx 0x000719d4
  000a63e8: mov r1,r11
  000a63ea: blx 0x000719e0
  000a63ee: ldrd r0,r1,[r11,#0x0]
  000a63f2: add.w r0,r1,r0, lsl #0x2
  000a63f6: movs r1,#0x0
  000a63f8: ldr.w r0,[r0,#-0x4]
  000a63fc: blx 0x000719ec
  000a6400: ldr.w r0,[r11,#0x0]
  000a6404: ldr.w r1,[r11,#0x4]
  000a6408: add.w r0,r1,r0, lsl #0x2
  000a640c: ldr.w r0,[r0,#-0x4]
  000a6410: blx 0x000719f8
  000a6414: b.w 0x000a5c18
  000a6418: ldr r0,[sp,#0x18]
  000a641a: mov.w r1,#0xffffffff
  000a641e: ldr r0,[r0,#0x0]
  000a6420: ldr r0,[r0,#0x4]
  000a6422: ldr.w r0,[r0,#0xfc]
  000a6426: blx 0x000719d4
  000a642a: mov r1,r11
  000a642c: blx 0x000719e0
  000a6430: ldrd r0,r1,[r11,#0x0]
  000a6434: mov.w r9,#0x1
  000a6438: add.w r0,r1,r0, lsl #0x2
  000a643c: movs r1,#0x1
  000a643e: ldr.w r0,[r0,#-0x4]
  000a6442: blx 0x000719ec
  000a6446: ldrd r0,r1,[r11,#0x0]
  000a644a: add.w r0,r1,r0, lsl #0x2
  000a644e: ldr.w r0,[r0,#-0x4]
  000a6452: blx 0x000719f8
  000a6456: b 0x000a6074
```
