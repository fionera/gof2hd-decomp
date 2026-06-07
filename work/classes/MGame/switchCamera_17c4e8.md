# MGame::switchCamera
elf 0x17c4e8 body 314
Sig: undefined __stdcall switchCamera(int param_1)

## decompile
```c

/* MGame::switchCamera(int) */

void MGame::switchCamera(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int in_r1;
  undefined4 uVar3;
  int iVar4;
  
LAB_0018c4f4:
  iVar4 = *(int *)(param_1 + 0x14);
  if (in_r1 == 2) {
    in_r1 = 3;
  }
  *(int *)(param_1 + 0x14) = in_r1;
  if (in_r1 == 1) {
    iVar1 = PlayerEgo::isDockingToAsteroid();
    if (iVar1 == 0) {
      iVar1 = PlayerEgo::setTurretMode(SUB41(*(undefined4 *)(param_1 + 0x58),0));
      *(char *)(param_1 + 0xd5) = (char)iVar1;
      if (iVar1 != 0) {
        in_r1 = *(int *)(param_1 + 0x14);
        goto LAB_0018c536;
      }
    }
    else {
      *(undefined1 *)(param_1 + 0xd5) = 0;
    }
    in_r1 = *(int *)(param_1 + 0x14) + 1;
    *(int *)(param_1 + 0x14) = in_r1;
  }
  else {
    *(undefined1 *)(param_1 + 0xd5) = 0;
  }
LAB_0018c536:
  if (in_r1 == 2) {
    uVar2 = 3;
LAB_0018c544:
    *(undefined4 *)(param_1 + 0x14) = uVar2;
  }
  else if (3 < in_r1) {
    uVar2 = 0;
    goto LAB_0018c544;
  }
  *(undefined1 *)(param_1 + 0x18) = 0;
  PlayerEgo::setTurretMode(SUB41(*(undefined4 *)(param_1 + 0x58),0));
  switch(*(undefined4 *)(param_1 + 0x14)) {
  case 0:
    break;
  case 1:
  case 3:
    iVar4 = PlayerEgo::isDockedToDockingPoint();
    if (iVar4 != 0) {
      TargetFollowCamera::setLookAtCam(SUB41(*(undefined4 *)(param_1 + 0xf4),0));
    }
    LevelScript::lookBehind(*(LevelScript **)(param_1 + 0x7c));
    TargetFollowCamera::setRotationAroundTarget(*(TargetFollowCamera **)(param_1 + 0xf4),true);
    PlayerEgo::setFreeLookMode(*(PlayerEgo **)(param_1 + 0x58),true);
    goto switchD_0018c556_default;
  case 2:
    LevelScript::resetCamera(*(Level **)(param_1 + 0x7c));
    TargetFollowCamera::setRotationAroundTarget(*(TargetFollowCamera **)(param_1 + 0xf4),false);
    PlayerEgo::setFreeLookMode(*(PlayerEgo **)(param_1 + 0x58),false);
    if (iVar4 == 1) {
      *(undefined1 *)(param_1 + 0x18) = 1;
    }
  default:
    goto switchD_0018c556_default;
  }
  iVar4 = PlayerEgo::isDockedToDockingPoint();
  in_r1 = 1;
  if (iVar4 == 0) {
    LevelScript::resetCamera(*(Level **)(param_1 + 0x7c));
    TargetFollowCamera::setRotationAroundTarget(*(TargetFollowCamera **)(param_1 + 0xf4),false);
    PlayerEgo::setFreeLookMode(*(PlayerEgo **)(param_1 + 0x58),false);
    iVar4 = AbyssEngine::ApplicationManager::GetEngine();
    *(undefined4 *)(iVar4 + 0x360) = 0;
switchD_0018c556_default:
    TargetFollowCamera::enableFirstPersonCam
              (*(TargetFollowCamera **)(param_1 + 0xf4),*(int *)(param_1 + 0x14) == 2);
    uVar2 = *(undefined4 *)(param_1 + 0x58);
    if (*(char *)(param_1 + 0x18) == '\0') {
      uVar3 = TargetFollowCamera::hideShipForFirstPersonCam();
    }
    else {
      uVar3 = 1;
    }
    (*(code *)(DAT_001ac874 + 0x1ac878))(uVar2,uVar3);
    return;
  }
  goto LAB_0018c4f4;
}

```

## target disasm
```
  0018c4e8: push {r4,r5,r6,r7,lr}
  0018c4ea: add r7,sp,#0xc
  0018c4ec: push.w r11
  0018c4f0: mov r4,r0
  0018c4f2: movs r6,#0x0
  0018c4f4: ldr r5,[r4,#0x14]
  0018c4f6: cmp r1,#0x2
  0018c4f8: it eq
  0018c4fa: add.eq r1,#0x1
  0018c4fc: cmp r1,#0x1
  0018c4fe: str r1,[r4,#0x14]
  0018c500: bne 0x0018c510
  0018c502: ldr r0,[r4,#0x58]
  0018c504: blx 0x00076804
  0018c508: cbz r0,0x0018c518
  0018c50a: strb.w r6,[r4,#0xd5]
  0018c50e: b 0x0018c52e
  0018c510: movs r2,#0x0
  0018c512: strb.w r2,[r4,#0xd5]
  0018c516: b 0x0018c536
  0018c518: ldr r0,[r4,#0x58]
  0018c51a: movs r1,#0x1
  0018c51c: blx 0x00072b08
  0018c520: cmp r0,#0x0
  0018c522: strb.w r0,[r4,#0xd5]
  0018c526: beq 0x0018c52e
  0018c528: ldr r1,[r4,#0x14]
  0018c52a: movs r2,#0x1
  0018c52c: b 0x0018c536
  0018c52e: ldr r0,[r4,#0x14]
  0018c530: movs r2,#0x0
  0018c532: adds r1,r0,#0x1
  0018c534: str r1,[r4,#0x14]
  0018c536: cmp r1,#0x2
  0018c538: bne 0x0018c53e
  0018c53a: movs r0,#0x3
  0018c53c: b 0x0018c544
  0018c53e: cmp r1,#0x4
  0018c540: blt 0x0018c546
  0018c542: movs r0,#0x0
  0018c544: str r0,[r4,#0x14]
  0018c546: ldr r0,[r4,#0x58]
  0018c548: mov r1,r2
  0018c54a: strb r6,[r4,#0x18]
  0018c54c: blx 0x00072b08
  0018c550: ldr r0,[r4,#0x14]
  0018c552: cmp r0,#0x3
  0018c554: bhi 0x0018c5c6
  0018c556: tbb [pc,r0]
  0018c55e: ldr r0,[r4,#0x58]
  0018c560: blx 0x00075928
  0018c564: movs r1,#0x1
  0018c566: cmp r0,#0x0
  0018c568: bne 0x0018c4f4
  0018c56a: ldrd r1,r0,[r4,#0x78]
  0018c56e: blx 0x0007246c
  0018c572: mov r6,r4
  0018c574: movs r1,#0x0
  0018c576: ldr.w r0,[r6,#0xf4]!
  0018c57a: movs r5,#0x0
  0018c57c: blx 0x00076c0c
  0018c580: ldr.w r0,[r6,#-0x9c]
  0018c584: movs r1,#0x0
  0018c586: blx 0x00076b94
  0018c58a: ldr.w r0,[r6,#-0xec]
  0018c58e: blx 0x0007258c
  0018c592: str.w r5,[r0,#0x360]
  0018c596: b 0x0018c5f2
  0018c598: ldr r0,[r4,#0x58]
  0018c59a: blx 0x00075928
  0018c59e: add.w r6,r4,#0xf4
  0018c5a2: cbz r0,0x0018c5ac
  0018c5a4: ldr r0,[r6,#0x0]
  0018c5a6: movs r1,#0x0
  0018c5a8: blx 0x00072670
  0018c5ac: ldr r0,[r4,#0x7c]
  0018c5ae: blx 0x00076cfc
  0018c5b2: ldr.w r0,[r4,#0xf4]
  0018c5b6: movs r1,#0x1
  0018c5b8: blx 0x00076c0c
  0018c5bc: ldr r0,[r4,#0x58]
  0018c5be: movs r1,#0x1
  0018c5c0: blx 0x00076b94
  0018c5c4: b 0x0018c5f2
  0018c5c6: add.w r6,r4,#0xf4
  0018c5ca: b 0x0018c5f2
  0018c5cc: ldrd r1,r0,[r4,#0x78]
  0018c5d0: blx 0x0007246c
  0018c5d4: mov r6,r4
  0018c5d6: movs r1,#0x0
  0018c5d8: ldr.w r0,[r6,#0xf4]!
  0018c5dc: blx 0x00076c0c
  0018c5e0: ldr.w r0,[r6,#-0x9c]
  0018c5e4: movs r1,#0x0
  0018c5e6: blx 0x00076b94
  0018c5ea: cmp r5,#0x1
  0018c5ec: itt eq
  0018c5ee: mov.eq r0,#0x1
  0018c5f0: strb.eq r0,[r4,#0x18]
  0018c5f2: ldr r1,[r4,#0x14]
  0018c5f4: ldr.w r0,[r4,#0xf4]
  0018c5f8: subs r1,#0x2
  0018c5fa: clz r1,r1
  0018c5fe: lsrs r1,r1,#0x5
  0018c600: blx 0x00076ba0
  0018c604: ldrb r0,[r4,#0x18]
  0018c606: ldr r5,[r4,#0x58]
  0018c608: cbz r0,0x0018c60e
  0018c60a: movs r1,#0x1
  0018c60c: b 0x0018c616
  0018c60e: ldr r0,[r6,#0x0]
  0018c610: blx 0x00078460
  0018c614: mov r1,r0
  0018c616: mov r0,r5
  0018c618: pop.w r11
  0018c61c: pop.w {r4,r5,r6,r7,lr}
  0018c620: b.w 0x001ac868
  001ac868: bx pc
```
