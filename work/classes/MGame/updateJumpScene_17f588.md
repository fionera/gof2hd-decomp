# MGame::updateJumpScene
elf 0x17f588 body 852
Sig: undefined __stdcall updateJumpScene(void)

## decompile
```c

/* MGame::updateJumpScene() */

void MGame::updateJumpScene(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  uint uVar3;
  PlayerEgo *this;
  Station *pSVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  undefined1 *puVar8;
  float unaff_r6;
  int *piVar9;
  undefined4 *puVar10;
  int *piVar11;
  code *pcVar12;
  bool bVar13;
  uint in_fpscr;
  float extraout_s0;
  float fVar14;
  float extraout_s1;
  float extraout_s2;
  
  piVar11 = *(int **)(DAT_0018f8f4 + 0x18f598);
  iVar1 = *piVar11;
  if ((*(char *)(in_r0 + 0xdd) == '\0') || (*(int *)(in_r0 + 0x114) == 0)) {
    iVar2 = Level::getLandmarks();
    if (*(int *)(*(int *)(iVar2 + 4) + 4) != 0) {
      Level::getLandmarks();
      iVar2 = PlayerJumpgate::timeToJump();
      if (iVar2 == 0) goto LAB_0018f5ee;
    }
LAB_0018f5ea:
    bVar13 = true;
  }
  else {
    iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_0018f8f8 + 0x18f5b2));
    iVar6 = *(int *)(iVar2 + 0x114);
    bVar13 = 0x6a4 < *(uint *)(iVar2 + 0x110);
    if ((int)(-(uint)bVar13 - iVar6) < 0 != (SBORROW4(0,iVar6) != SBORROW4(-iVar6,(uint)bVar13)))
    goto LAB_0018f5ea;
LAB_0018f5ee:
    if (*(char *)(in_r0 + 0xdd) == '\0') {
      iVar7 = *(int *)(in_r0 + 0x40);
      iVar6 = **(int **)(in_r0 + 0x58);
      iVar2 = iVar7 * -3;
    }
    else {
      iVar7 = *(int *)(in_r0 + 0x40);
      iVar6 = **(int **)(in_r0 + 0x58);
      iVar2 = iVar7 * -5;
    }
    VectorSignedToFloat(iVar7 << 1,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(iVar7 * 5,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&stack0xffffffd0,(Vector *)(iVar6 + 4));
    AbyssEngine::AEMath::Vector::operator=((Vector *)(in_r0 + 0xe4),(Vector *)&stack0xffffffd0);
    TargetFollowCamera::translate
              (*(TargetFollowCamera **)(in_r0 + 0xf4),extraout_s0,extraout_s1,extraout_s2);
    if (*(char *)(in_r0 + 0xdd) == '\0') {
      iVar2 = Level::getLandmarks();
      (**(code **)(**(int **)(*(int *)(iVar2 + 4) + 4) + 0x28))(&stack0xffffffd0);
    }
    else {
      AEGeometry::getPosition();
    }
    AbyssEngine::AEMath::Vector::operator=((Vector *)(in_r0 + 0xe4),(Vector *)&stack0xffffffd0);
    bVar13 = false;
  }
  if (*(char *)(in_r0 + 0xdd) != '\0') {
    uVar3 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_0018f8fc + 0x18f6a4));
    AbyssEngine::Transform::Update((ulonglong)uVar3,SUB41(*(undefined4 *)(in_r0 + 0x40),0));
  }
  iVar2 = TargetFollowCamera::getPosition(*(TargetFollowCamera **)(in_r0 + 0xf4));
  fVar14 = *(float *)(in_r0 + 0xec) + DAT_0018f8dc;
  if (((int)((uint)(*(float *)(iVar2 + 8) < fVar14) << 0x1f) < 0) &&
     (*(char *)(in_r0 + 0xdd) == '\0')) {
    iVar2 = Level::getLandmarks();
    PlayerJumpgate::activate(*(PlayerJumpgate **)(*(int *)(iVar2 + 4) + 4));
    PlayerEgo::getPosition();
    fVar14 = *(float *)(in_r0 + 0xec) + DAT_0018f8e0;
    if ((fVar14 <= unaff_r6) && (*(char *)(in_r0 + 0xcd) == '\0')) {
      piVar9 = *(int **)(DAT_0018f900 + 0x18f71e);
      pcVar12 = *(code **)(DAT_0018f904 + 0x18f724);
      (*pcVar12)(*piVar9,*(undefined4 *)(*(int *)(in_r0 + 0x58) + 0x1c));
      (*pcVar12)(*piVar9,0x8d5);
      (*pcVar12)(*piVar9,0x8d4);
      fVar14 = (float)(*pcVar12)(*piVar9,0x23);
      fVar14 = (float)FModSound::play(*piVar9,(Vector *)&DAT_0000001f,(Vector *)0x0,fVar14);
      *(undefined1 *)(in_r0 + 0xcd) = 1;
    }
  }
  if (bVar13) {
    PlayerEgo::setSpeed(*(PlayerEgo **)(in_r0 + 0x58),fVar14);
    PlayerEgo::setVisible(*(PlayerEgo **)(in_r0 + 0x58),false);
    PlayerEgo::setExhaustVisible(*(PlayerEgo **)(in_r0 + 0x58),false);
  }
  if (*(char *)(in_r0 + 0xdd) == '\0') {
    iVar2 = Level::getLandmarks();
    iVar2 = PlayerJumpgate::animationEnded(*(PlayerJumpgate **)(*(int *)(iVar2 + 4) + 4));
    if (iVar2 == 0) goto LAB_0018f8c4;
  }
  else {
    iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_0018f908 + 0x18f782));
    if (*(char *)(iVar2 + 0xed) != '\0') goto LAB_0018f8c4;
  }
  piVar9 = *(int **)(DAT_0018f90c + 0x18f7ac);
  iVar2 = Status::getCurrentCampaignMission();
  if ((iVar2 == 0x2a) && (iVar2 = Status::inAlienOrbit(), iVar2 != 0)) {
    fVar14 = (float)LevelScript::setEvent(*(LevelScript **)(in_r0 + 0x7c),6);
    PlayerEgo::setSpeed(*(PlayerEgo **)(in_r0 + 0x58),fVar14);
    iVar2 = Level::getLandmarks();
    piVar9 = *(int **)(*(int *)(iVar2 + 4) + 0xc);
    (**(code **)(*piVar9 + 0x48))(piVar9,DAT_0018f8e8,DAT_0018f8ec,DAT_0018f8f0);
    iVar2 = Level::getLandmarks();
    (**(code **)(**(int **)(*(int *)(iVar2 + 4) + 0xc) + 0x28))((Vector *)&stack0xffffffd0);
    AbyssEngine::AEMath::Vector::operator=((Vector *)(in_r0 + 0xe4),(Vector *)&stack0xffffffd0);
    PlayerEgo::setPosition
              (*(undefined4 *)(in_r0 + 0x58),*(undefined4 *)(in_r0 + 0xe4),
               *(undefined4 *)(in_r0 + 0xe8),*(undefined4 *)(in_r0 + 0xec));
    this = *(PlayerEgo **)(in_r0 + 0x58);
    *(undefined1 *)((int)&this[1].m_pPlayer + 1) = 1;
    *(undefined1 *)(in_r0 + 0xdc) = 0;
    PlayerEgo::resetChargingDrive(this);
  }
  else {
    puVar10 = *(undefined4 **)(DAT_0018f910 + 0x18f82c);
    Status::departStation((Station *)*piVar9);
    Level::setInitStreamOut();
    if (*(char *)(in_r0 + 0xdd) == '\0') {
      Status::jumpgateUsed((Status *)*piVar9);
    }
    iVar2 = Station::equals((Station *)*puVar10,*(Station **)(*piVar9 + 0x78));
    if (iVar2 != 0) {
      puVar8 = *(undefined1 **)(DAT_0018f918 + 0x18f85c);
      pSVar4 = (Station *)*piVar9;
      **(undefined1 **)(DAT_0018f914 + 0x18f85a) = 1;
      *puVar8 = 1;
      Status::setStation(pSVar4);
    }
    *puVar10 = 0;
    uVar5 = Player::getHitpoints();
    *(undefined4 *)(*piVar9 + 100) = uVar5;
    uVar5 = Player::getShieldHP((Player *)**(undefined4 **)(in_r0 + 0x58));
    *(undefined4 *)(*piVar9 + 0x5c) = uVar5;
    uVar5 = Player::getArmorHP();
    *(undefined4 *)(*piVar9 + 0x60) = uVar5;
    uVar5 = Player::getGammaHP();
    *(undefined4 *)(*piVar9 + 0x68) = uVar5;
    uVar5 = PlayerEgo::getCurrentSecondaryWeaponIndex();
    puVar10 = *(undefined4 **)(DAT_0018f91c + 0x18f8b2);
    *(undefined4 *)(*piVar9 + 0xf4) = uVar5;
    *puVar10 = 1;
    *(undefined1 *)(in_r0 + 0x54) = 0;
    AbyssEngine::ApplicationManager::SetCurrentApplicationModule(*(uint *)(in_r0 + 8));
  }
LAB_0018f8c4:
  iVar1 = *piVar11 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  0018f588: push {r4,r5,r6,r7,lr}
  0018f58a: add r7,sp,#0xc
  0018f58c: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  0018f590: mov r4,r0
  0018f592: ldr r0,[0x0018f8f4]
  0018f594: add r0,pc
  0018f596: ldr.w r8,[r0,#0x0]
  0018f59a: ldr.w r0,[r8,#0x0]
  0018f59e: str r0,[sp,#0x1c]
  0018f5a0: ldrb.w r0,[r4,#0xdd]
  0018f5a4: cbz r0,0x0018f5ce
  0018f5a6: ldr.w r0,[r4,#0x114]
  0018f5aa: cbz r0,0x0018f5ce
  0018f5ac: ldr r1,[0x0018f8f8]
  0018f5ae: add r1,pc
  0018f5b0: ldr r2,[r1,#0x0]
  0018f5b2: ldr r1,[r0,#0xc]
  0018f5b4: ldr r0,[r2,#0x0]
  0018f5b6: blx 0x00072088
  0018f5ba: ldrd r0,r1,[r0,#0x110]
  0018f5be: movw r3,#0x6a4
  0018f5c2: movs r2,#0x0
  0018f5c4: subs r0,r3,r0
  0018f5c6: sbcs.w r0,r2,r1
  0018f5ca: blt 0x0018f5ea
  0018f5cc: b 0x0018f5ee
  0018f5ce: ldr r0,[r4,#0x78]
  0018f5d0: blx 0x000725b0
  0018f5d4: ldr r0,[r0,#0x4]
  0018f5d6: ldr r0,[r0,#0x4]
  0018f5d8: cbz r0,0x0018f5ea
  0018f5da: ldr r0,[r4,#0x78]
  0018f5dc: blx 0x000725b0
  0018f5e0: ldr r0,[r0,#0x4]
  0018f5e2: ldr r0,[r0,#0x4]
  0018f5e4: blx 0x000785c8
  0018f5e8: cbz r0,0x0018f5ee
  0018f5ea: movs r5,#0x1
  0018f5ec: b 0x0018f694
  0018f5ee: ldrb.w r0,[r4,#0xdd]
  0018f5f2: cbz r0,0x0018f60c
  0018f5f4: ldr r1,[r4,#0x40]
  0018f5f6: ldr r0,[r4,#0x58]
  0018f5f8: lsls r2,r1,#0x1
  0018f5fa: add.w r1,r1,r1, lsl #0x2
  0018f5fe: vmov s0,r2
  0018f602: ldr r0,[r0,#0x0]
  0018f604: vmov s2,r1
  0018f608: rsbs r1,r1
  0018f60a: b 0x0018f624
  0018f60c: ldr r1,[r4,#0x40]
  0018f60e: ldr r0,[r4,#0x58]
  0018f610: add.w r3,r1,r1, lsl #0x2
  0018f614: lsls r2,r1,#0x1
  0018f616: vmov s0,r2
  0018f61a: ldr r0,[r0,#0x0]
  0018f61c: vmov s2,r3
  0018f620: sub.w r1,r1,r1, lsl #0x2
  0018f624: vcvt.f32.s32 s0,s0
  0018f628: vcvt.f32.s32 s2,s2
  0018f62c: vmov s4,r1
  0018f630: add r6,sp,#0x10
  0018f632: adds r1,r0,#0x4
  0018f634: add r2,sp,#0x4
  0018f636: vcvt.f32.s32 s4,s4
  0018f63a: mov r0,r6
  0018f63c: vstr.32 s0,[sp,#0x8]
  0018f640: vstr.32 s2,[sp,#0x4]
  0018f644: vstr.32 s4,[sp,#0xc]
  0018f648: blx 0x0006f694
  0018f64c: add.w r5,r4,#0xe4
  0018f650: mov r1,r6
  0018f652: mov r0,r5
  0018f654: blx 0x0006eb3c
  0018f658: add.w r3,r4,#0xe4
  0018f65c: ldmia r3,{r1,r2,r3}
  0018f65e: ldr.w r0,[r4,#0xf4]
  0018f662: blx 0x00072688
  0018f666: ldrb.w r0,[r4,#0xdd]
  0018f66a: cbz r0,0x0018f678
  0018f66c: ldr.w r1,[r4,#0x114]
  0018f670: add r0,sp,#0x10
  0018f672: blx 0x000720b8
  0018f676: b 0x0018f68a
  0018f678: ldr r0,[r4,#0x78]
  0018f67a: blx 0x000725b0
  0018f67e: ldr r0,[r0,#0x4]
  0018f680: ldr r1,[r0,#0x4]
  0018f682: ldr r0,[r1,#0x0]
  0018f684: ldr r2,[r0,#0x28]
  0018f686: add r0,sp,#0x10
  0018f688: blx r2
  0018f68a: add r1,sp,#0x10
  0018f68c: mov r0,r5
  0018f68e: blx 0x0006eb3c
  0018f692: movs r5,#0x0
  0018f694: ldrb.w r0,[r4,#0xdd]
  0018f698: cbz r0,0x0018f6b8
  0018f69a: ldr r0,[0x0018f8fc]
  0018f69c: ldr.w r1,[r4,#0x114]
  0018f6a0: add r0,pc
  0018f6a2: ldr r0,[r0,#0x0]
  0018f6a4: ldr r1,[r1,#0xc]
  0018f6a6: ldr r0,[r0,#0x0]
  0018f6a8: blx 0x00072088
  0018f6ac: ldr r2,[r4,#0x40]
  0018f6ae: movs r1,#0x0
  0018f6b0: str r1,[sp,#0x0]
  0018f6b2: asrs r3,r2,#0x1f
  0018f6b4: blx 0x0006f7cc
  0018f6b8: ldr.w r0,[r4,#0xf4]
  0018f6bc: blx 0x00076b28
  0018f6c0: vldr.32 s0,[pc,#0x218]
  0018f6c4: vldr.32 s2,[r4,#0xec]
  0018f6c8: vadd.f32 s0,s2,s0
  0018f6cc: vldr.32 s2,[r0,#0x8]
  0018f6d0: vcmpe.f32 s2,s0
  0018f6d4: vmrs apsr,fpscr
  0018f6d8: bpl 0x0018f758
  0018f6da: ldrb.w r0,[r4,#0xdd]
  0018f6de: cbnz r0,0x0018f758
  0018f6e0: ldr r0,[r4,#0x78]
  0018f6e2: blx 0x000725b0
  0018f6e6: ldr r0,[r0,#0x4]
  0018f6e8: ldr r0,[r0,#0x4]
  0018f6ea: blx 0x000785d4
  0018f6ee: ldr r1,[r4,#0x58]
  0018f6f0: add r0,sp,#0x10
  0018f6f2: blx 0x0007264c
  0018f6f6: vldr.32 s0,[pc,#0x1e8]
  0018f6fa: vldr.32 s2,[r4,#0xec]
  0018f6fe: vadd.f32 s0,s2,s0
  0018f702: vldr.32 s2,[sp,#0x18]
  0018f706: vcmpe.f32 s2,s0
  0018f70a: vmrs apsr,fpscr
  0018f70e: blt 0x0018f758
  0018f710: ldrb.w r0,[r4,#0xcd]
  0018f714: cbnz r0,0x0018f758
  0018f716: ldr r0,[0x0018f900]
  0018f718: ldr r1,[r4,#0x58]
  0018f71a: add r0,pc
  0018f71c: ldr r2,[0x0018f904]
  0018f71e: ldr r6,[r0,#0x0]
  0018f720: add r2,pc
  0018f722: ldr r1,[r1,#0x1c]
  0018f724: ldr.w r9,[r2,#0x0]
  0018f728: ldr r0,[r6,#0x0]
  0018f72a: blx r9
  0018f72c: ldr r0,[r6,#0x0]
  0018f72e: movw r1,#0x8d5
  0018f732: blx r9
  0018f734: ldr r0,[r6,#0x0]
  0018f736: movw r1,#0x8d4
  0018f73a: blx r9
  0018f73c: ldr r0,[r6,#0x0]
  0018f73e: movs r1,#0x23
  0018f740: blx r9
  0018f742: ldr r0,[r6,#0x0]
  0018f744: movs r1,#0x0
  0018f746: str r1,[sp,#0x0]
  0018f748: movs r1,#0x1f
  0018f74a: movs r2,#0x0
  0018f74c: movs r3,#0x0
  0018f74e: blx 0x00071548
  0018f752: movs r0,#0x1
  0018f754: strb.w r0,[r4,#0xcd]
  0018f758: cbz r5,0x0018f772
  0018f75a: ldr r0,[r4,#0x58]
  0018f75c: ldr r1,[0x0018f8e4]
  0018f75e: blx 0x00076b40
  0018f762: ldr r0,[r4,#0x58]
  0018f764: movs r1,#0x0
  0018f766: blx 0x00076aec
  0018f76a: ldr r0,[r4,#0x58]
  0018f76c: movs r1,#0x0
  0018f76e: blx 0x00072af0
  0018f772: ldrb.w r0,[r4,#0xdd]
  0018f776: cbz r0,0x0018f794
  0018f778: ldr r0,[0x0018f908]
  0018f77a: ldr.w r1,[r4,#0x114]
  0018f77e: add r0,pc
  0018f780: ldr r0,[r0,#0x0]
  0018f782: ldr r1,[r1,#0xc]
  0018f784: ldr r0,[r0,#0x0]
  0018f786: blx 0x00072088
  0018f78a: ldrb.w r0,[r0,#0xed]
  0018f78e: cbz r0,0x0018f7a6
  0018f790: movs r5,#0x0
  0018f792: b 0x0018f8c4
  0018f794: ldr r0,[r4,#0x78]
  0018f796: blx 0x000725b0
  0018f79a: ldr r0,[r0,#0x4]
  0018f79c: ldr r0,[r0,#0x4]
  0018f79e: blx 0x000785e0
  0018f7a2: cmp r0,#0x0
  0018f7a4: beq 0x0018f790
  0018f7a6: ldr r0,[0x0018f90c]
  0018f7a8: add r0,pc
  0018f7aa: ldr r5,[r0,#0x0]
  0018f7ac: ldr r0,[r5,#0x0]
  0018f7ae: blx 0x00071770
  0018f7b2: cmp r0,#0x2a
  0018f7b4: bne 0x0018f826
  0018f7b6: ldr r0,[r5,#0x0]
  0018f7b8: blx 0x000723d0
  0018f7bc: cbz r0,0x0018f826
  0018f7be: ldr r0,[r4,#0x7c]
  0018f7c0: movs r1,#0x6
  0018f7c2: add.w r5,r4,#0xe4
  0018f7c6: blx 0x000785ec
  0018f7ca: ldr r0,[r4,#0x58]
  0018f7cc: movs r1,#0x0
  0018f7ce: mov.w r9,#0x0
  0018f7d2: blx 0x00076b40
  0018f7d6: ldr r0,[r4,#0x78]
  0018f7d8: blx 0x000725b0
  0018f7dc: ldr r0,[r0,#0x4]
  0018f7de: ldr r1,[0x0018f8e8]
  0018f7e0: ldr r2,[0x0018f8ec]
  0018f7e2: ldr r0,[r0,#0xc]
  0018f7e4: ldr r3,[r0,#0x0]
  0018f7e6: ldr r6,[r3,#0x48]
  0018f7e8: ldr r3,[0x0018f8f0]
  0018f7ea: blx r6
  0018f7ec: ldr r0,[r4,#0x78]
  0018f7ee: blx 0x000725b0
  0018f7f2: ldr r0,[r0,#0x4]
  0018f7f4: add r6,sp,#0x10
  0018f7f6: ldr r1,[r0,#0xc]
  0018f7f8: ldr r0,[r1,#0x0]
  0018f7fa: ldr r2,[r0,#0x28]
  0018f7fc: mov r0,r6
  0018f7fe: blx r2
  0018f800: mov r0,r5
  0018f802: mov r1,r6
  0018f804: blx 0x0006eb3c
  0018f808: add.w r3,r4,#0xe4
  0018f80c: ldr r0,[r4,#0x58]
  0018f80e: ldmia r3,{r1,r2,r3}
  0018f810: blx 0x00072cb8
  0018f814: ldr r0,[r4,#0x58]
  0018f816: movs r5,#0x1
  0018f818: strb.w r5,[r0,#0x25]
  0018f81c: strb.w r9,[r4,#0xdc]
  0018f820: blx 0x000785f8
  0018f824: b 0x0018f8c4
  0018f826: ldr r0,[0x0018f910]
  0018f828: add r0,pc
  0018f82a: ldr r6,[r0,#0x0]
  0018f82c: ldr r0,[r5,#0x0]
  0018f82e: ldr r1,[r6,#0x0]
  0018f830: blx 0x000745fc
  0018f834: blx 0x00074608
  0018f838: ldrb.w r0,[r4,#0xdd]
  0018f83c: cbnz r0,0x0018f844
  0018f83e: ldr r0,[r5,#0x0]
  0018f840: blx 0x00074614
  0018f844: ldr r1,[r5,#0x0]
  0018f846: ldr r0,[r6,#0x0]
  0018f848: ldr r1,[r1,#0x78]
  0018f84a: blx 0x00073678
  0018f84e: cbz r0,0x0018f86a
  0018f850: ldr r0,[0x0018f914]
  0018f852: movs r3,#0x1
  0018f854: ldr r1,[0x0018f918]
  0018f856: add r0,pc
  0018f858: add r1,pc
  0018f85a: ldr r2,[r0,#0x0]
  0018f85c: ldr r1,[r1,#0x0]
  0018f85e: ldr r0,[r5,#0x0]
  0018f860: strb r3,[r2,#0x0]
  0018f862: strb r3,[r1,#0x0]
  0018f864: ldr r1,[r0,#0x78]
  0018f866: blx 0x0007366c
  0018f86a: mov.w r9,#0x0
  0018f86e: str.w r9,[r6,#0x0]
  0018f872: ldr r0,[r4,#0x58]
  0018f874: ldr r0,[r0,#0x0]
  0018f876: blx 0x000724f0
  0018f87a: ldr r1,[r5,#0x0]
  0018f87c: str r0,[r1,#0x64]
  0018f87e: ldr r0,[r4,#0x58]
  0018f880: ldr r0,[r0,#0x0]
  0018f882: blx 0x000726c4
  0018f886: ldr r1,[r5,#0x0]
  0018f888: str r0,[r1,#0x5c]
  0018f88a: ldr r0,[r4,#0x58]
  0018f88c: ldr r0,[r0,#0x0]
  0018f88e: blx 0x00072928
  0018f892: ldr r1,[r5,#0x0]
  0018f894: str r0,[r1,#0x60]
  0018f896: ldr r0,[r4,#0x58]
  0018f898: ldr r0,[r0,#0x0]
  0018f89a: blx 0x00072814
  0018f89e: ldr r1,[r5,#0x0]
  0018f8a0: str r0,[r1,#0x68]
  0018f8a2: ldr r0,[r4,#0x58]
  0018f8a4: blx 0x00076d20
  0018f8a8: ldr r1,[0x0018f91c]
  0018f8aa: ldr r2,[r5,#0x0]
  0018f8ac: movs r5,#0x1
  0018f8ae: add r1,pc
  0018f8b0: ldr r1,[r1,#0x0]
  0018f8b2: str.w r0,[r2,#0xf4]
  0018f8b6: str r5,[r1,#0x0]
  0018f8b8: movs r1,#0x2
  0018f8ba: ldr r0,[r4,#0x8]
  0018f8bc: strb.w r9,[r4,#0x54]
  0018f8c0: blx 0x00071d64
  0018f8c4: ldr r0,[sp,#0x1c]
  0018f8c6: ldr.w r1,[r8,#0x0]
  0018f8ca: subs r0,r1,r0
  0018f8cc: itttt eq
  0018f8ce: mov.eq r0,r5
  0018f8d0: add.eq sp,#0x20
  0018f8d2: pop.eq.w {r8,r9,r11}
  0018f8d6: pop.eq {r4,r5,r6,r7,pc}
  0018f8d8: blx 0x0006e824
```
