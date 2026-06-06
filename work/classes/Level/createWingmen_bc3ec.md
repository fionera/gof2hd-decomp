# Level::createWingmen
elf 0xbc3ec body 604
Sig: undefined __thiscall createWingmen(Level * this)

## decompile
```c

/* Level::createWingmen() */

void __thiscall Level::createWingmen(Level *this)

{
  float fVar1;
  int iVar2;
  Array<KIPlayer*> *this_00;
  uint *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  float *pfVar6;
  int *piVar7;
  uint uVar8;
  undefined4 extraout_r1;
  int *piVar9;
  Vector *pVVar10;
  uint uVar11;
  undefined4 uVar12;
  int *piVar13;
  float fVar14;
  Vector aVStack_58 [24];
  Vector aVStack_40 [4];
  float local_3c;
  int local_34;
  
  piVar9 = *(int **)(DAT_000cc670 + 0xcc404);
  piVar13 = *(int **)(DAT_000cc674 + 0xcc406);
  local_34 = *piVar9;
  iVar2 = Status::inSupernovaSystem();
  if ((((iVar2 == 0) && (iVar2 = Status::getCurrentCampaignMission(), iVar2 != 0x9e)) &&
      (iVar2 = Status::getWingmen(), iVar2 != 0)) && (*(int *)(this + 0xf0) != 0)) {
    this_00 = operator_new(0xc);
    Array<KIPlayer*>::Array(this_00);
    puVar3 = (uint *)Status::getWingmen();
    ArraySetLength<KIPlayer*>(*puVar3,(Array *)this_00);
    fVar1 = DAT_000cc660;
    puVar4 = *(undefined4 **)(DAT_000cc678 + 0xcc46c);
    puVar5 = *(undefined4 **)(DAT_000cc67c + 0xcc474);
    for (uVar11 = 0; uVar8 = *(uint *)this_00, uVar11 < uVar8; uVar11 = uVar11 + 1) {
      uVar12 = *puVar4;
      Status::getWingmen();
      AbyssEngine::AERandom::setSeed(CONCAT44(extraout_r1,uVar12));
      iVar2 = Globals::getRandomEnemyFighter((Globals *)*puVar5,*(int *)(*piVar13 + 0x2c));
      uVar12 = createShip(this,5,0,iVar2,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(this_00 + 4) + uVar11 * 4) = uVar12;
      AEGeometry::getPosition();
      AEGeometry::getRightVector();
      pfVar6 = (float *)&DAT_000cc658;
      if (uVar11 == 1) {
        pfVar6 = (float *)&DAT_000cc65c;
      }
      fVar14 = *pfVar6;
      if (uVar11 == 0) {
        fVar14 = fVar1;
      }
      AbyssEngine::AEMath::operator*(aVStack_58,fVar14);
      AbyssEngine::AEMath::Vector::operator+=(aVStack_40,aVStack_58);
      fVar14 = (float)AEGeometry::getDirection();
      AbyssEngine::AEMath::operator*(aVStack_58,fVar14);
      AbyssEngine::AEMath::Vector::operator-=(aVStack_40,aVStack_58);
      pfVar6 = (float *)&DAT_000cc668;
      if (uVar11 == 2) {
        pfVar6 = (float *)&DAT_000cc66c;
      }
      local_3c = *pfVar6 + local_3c;
      piVar7 = *(int **)(*(int *)(this_00 + 4) + uVar11 * 4);
      (**(code **)(*piVar7 + 0x44))(piVar7,aVStack_40);
      pVVar10 = *(Vector **)(*(int *)(*(int *)(this_00 + 4) + uVar11 * 4) + 8);
      AEGeometry::getDirection();
      AEGeometry::setDirection(pVVar10,aVStack_58);
      KIPlayer::setWingman(*(KIPlayer **)(*(int *)(this_00 + 4) + uVar11 * 4),true,uVar11);
      Player::setAlwaysFriend(*(Player **)(*(int *)(*(int *)(this_00 + 4) + uVar11 * 4) + 4),true);
      Player::setHitpoints(*(int *)(*(int *)(*(int *)(this_00 + 4) + uVar11 * 4) + 4));
      iVar2 = Status::getWingmen();
      AbyssEngine::String::operator=
                ((String *)(*(int *)(*(int *)(this_00 + 4) + uVar11 * 4) + 0x18),
                 *(String **)(*(int *)(iVar2 + 4) + uVar11 * 4));
      *(undefined4 *)(*(int *)(*(int *)(this_00 + 4) + uVar11 * 4) + 0x28) =
           *(undefined4 *)(*piVar13 + 0x2c);
      Status::getMission();
      iVar2 = Mission::getType();
      if (iVar2 == 0xc) {
        *(undefined1 *)(*(int *)(*(int *)(this_00 + 4) + uVar11 * 4) + 0x25) = 0;
      }
    }
    if (*(int *)(this + 0xf8) == 0) {
      *(Array<KIPlayer*> **)(this + 0xf8) = this_00;
    }
    else {
      for (uVar11 = 0; uVar11 < uVar8; uVar11 = uVar11 + 1) {
        ArrayAdd<KIPlayer*>(*(KIPlayer **)(*(int *)(this_00 + 4) + uVar11 * 4),
                            *(Array **)(this + 0xf8));
        uVar8 = *(uint *)this_00;
      }
    }
    AbyssEngine::AERandom::reset();
  }
  if (*piVar9 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```
## target disasm
```
  000cc3ec: push {r4,r5,r6,r7,lr}
  000cc3ee: add r7,sp,#0xc
  000cc3f0: push {r7,r8,r9,r10,r11}
  000cc3f4: vpush {d8}
  000cc3f8: sub sp,#0x50
  000cc3fa: mov r11,r0
  000cc3fc: ldr r0,[0x000cc670]
  000cc3fe: ldr r1,[0x000cc674]
  000cc400: add r0,pc
  000cc402: add r1,pc
  000cc404: ldr r4,[r0,#0x0]
  000cc406: ldr.w r10,[r1,#0x0]
  000cc40a: ldr r1,[r4,#0x0]
  000cc40c: ldr.w r0,[r10,#0x0]
  000cc410: str r1,[sp,#0x4c]
  000cc412: blx 0x00071878
  000cc416: cmp r0,#0x0
  000cc418: bne.w 0x000cc62c
  000cc41c: ldr.w r0,[r10,#0x0]
  000cc420: blx 0x00071770
  000cc424: cmp r0,#0x9e
  000cc426: beq.w 0x000cc62c
  000cc42a: ldr.w r0,[r10,#0x0]
  000cc42e: blx 0x000741dc
  000cc432: cmp r0,#0x0
  000cc434: beq.w 0x000cc62c
  000cc438: ldr.w r0,[r11,#0xf0]
  000cc43c: cmp r0,#0x0
  000cc43e: beq.w 0x000cc62c
  000cc442: movs r0,#0xc
  000cc444: str r4,[sp,#0x10]
  000cc446: blx 0x0006eb24
  000cc44a: mov r5,r0
  000cc44c: blx 0x00073d08
  000cc450: ldr.w r0,[r10,#0x0]
  000cc454: blx 0x000741dc
  000cc458: ldr r0,[r0,#0x0]
  000cc45a: mov r1,r5
  000cc45c: blx 0x00073d14
  000cc460: ldr r0,[0x000cc678]
  000cc462: movs r6,#0x0
  000cc464: vldr.32 s16,[pc,#0x1f8]
  000cc468: add r0,pc
  000cc46a: ldr r0,[r0,#0x0]
  000cc46c: str r0,[sp,#0x18]
  000cc46e: ldr r0,[0x000cc67c]
  000cc470: add r0,pc
  000cc472: ldr r0,[r0,#0x0]
  000cc474: str r0,[sp,#0x14]
  000cc476: b 0x000cc5f4
  000cc478: ldr r0,[sp,#0x18]
  000cc47a: ldr.w r8,[r0,#0x0]
  000cc47e: ldr.w r0,[r10,#0x0]
  000cc482: blx 0x000741dc
  000cc486: ldr r0,[r0,#0x4]
  000cc488: movs r3,#0x0
  000cc48a: ldr.w r0,[r0,r6,lsl #0x2]
  000cc48e: ldr r0,[r0,#0x8]
  000cc490: add.w r2,r0,r0, lsl #0x2
  000cc494: mov r0,r8
  000cc496: blx 0x000739c0
  000cc49a: ldr.w r1,[r10,#0x0]
  000cc49e: ldr r0,[sp,#0x14]
  000cc4a0: ldr r1,[r1,#0x2c]
  000cc4a2: ldr r0,[r0,#0x0]
  000cc4a4: blx 0x00071a34
  000cc4a8: mov r3,r0
  000cc4aa: movs r1,#0x0
  000cc4ac: movs r0,#0x1
  000cc4ae: str r1,[sp,#0x0]
  000cc4b0: strd r0,r1,[sp,#0x4]
  000cc4b4: mov r0,r11
  000cc4b6: movs r1,#0x5
  000cc4b8: movs r2,#0x0
  000cc4ba: blx 0x00073e88
  000cc4be: ldr r1,[r5,#0x4]
  000cc4c0: add r4,sp,#0x40
  000cc4c2: str.w r0,[r1,r6,lsl #0x2]
  000cc4c6: ldr.w r0,[r11,#0xf0]
  000cc4ca: ldr r1,[r0,#0x8]
  000cc4cc: mov r0,r4
  000cc4ce: blx 0x000720b8
  000cc4d2: ldr.w r0,[r11,#0xf0]
  000cc4d6: mov r8,r11
  000cc4d8: add.w r11,sp,#0x34
  000cc4dc: ldr r1,[r0,#0x8]
  000cc4de: mov r0,r11
  000cc4e0: blx 0x0007261c
  000cc4e4: adr r0,[0xcc658]
  000cc4e6: cmp r6,#0x1
  000cc4e8: it eq
  000cc4ea: add.eq r0,#0x4
  000cc4ec: cmp r6,#0x0
  000cc4ee: vldr.32 s0,[r0]
  000cc4f2: add.w r9,sp,#0x28
  000cc4f6: mov r1,r11
  000cc4f8: mov r11,r8
  000cc4fa: it eq
  000cc4fc: vmov.eq.f32 s0,s16
  000cc500: mov r0,r9
  000cc502: vmov r2,s0
  000cc506: blx 0x0006ec74
  000cc50a: mov r0,r4
  000cc50c: mov r1,r9
  000cc50e: blx 0x00073534
  000cc512: ldr.w r0,[r8,#0xf0]
  000cc516: add.w r8,sp,#0x1c
  000cc51a: ldr r1,[r0,#0x8]
  000cc51c: mov r0,r8
  000cc51e: blx 0x00072574
  000cc522: ldr r2,[0x000cc664]
  000cc524: mov r0,r9
  000cc526: mov r1,r8
  000cc528: blx 0x0006ec74
  000cc52c: mov r0,r4
  000cc52e: mov r1,r9
  000cc530: blx 0x00072694
  000cc534: adr r0,[0xcc668]
  000cc536: cmp r6,#0x2
  000cc538: it eq
  000cc53a: add.eq r0,#0x4
  000cc53c: vldr.32 s2,[sp,#0x44]
  000cc540: vldr.32 s0,[r0]
  000cc544: ldr r0,[r5,#0x4]
  000cc546: vadd.f32 s0,s0,s2
  000cc54a: vstr.32 s0,[sp,#0x44]
  000cc54e: ldr.w r0,[r0,r6,lsl #0x2]
  000cc552: ldr r1,[r0,#0x0]
  000cc554: ldr r2,[r1,#0x44]
  000cc556: mov r1,r4
  000cc558: blx r2
  000cc55a: ldr r0,[r5,#0x4]
  000cc55c: ldr.w r0,[r0,r6,lsl #0x2]
  000cc560: ldr r4,[r0,#0x8]
  000cc562: ldr.w r0,[r11,#0xf0]
  000cc566: ldr r1,[r0,#0x8]
  000cc568: mov r0,r9
  000cc56a: blx 0x00072574
  000cc56e: mov.w r0,#0x3f800000
  000cc572: mov r1,r9
  000cc574: str r0,[sp,#0x20]
  000cc576: movs r0,#0x0
  000cc578: str r0,[sp,#0x1c]
  000cc57a: mov r2,r8
  000cc57c: str r0,[sp,#0x24]
  000cc57e: mov r0,r4
  000cc580: blx 0x000726ac
  000cc584: ldr r0,[r5,#0x4]
  000cc586: movs r1,#0x1
  000cc588: mov r2,r6
  000cc58a: ldr.w r0,[r0,r6,lsl #0x2]
  000cc58e: blx 0x000741e8
  000cc592: ldr r0,[r5,#0x4]
  000cc594: movs r1,#0x1
  000cc596: ldr.w r0,[r0,r6,lsl #0x2]
  000cc59a: ldr r0,[r0,#0x4]
  000cc59c: blx 0x00073fb4
  000cc5a0: ldr r0,[r5,#0x4]
  000cc5a2: mov.w r1,#0x258
  000cc5a6: ldr.w r0,[r0,r6,lsl #0x2]
  000cc5aa: ldr r0,[r0,#0x4]
  000cc5ac: blx 0x000727e4
  000cc5b0: ldr.w r0,[r10,#0x0]
  000cc5b4: blx 0x000741dc
  000cc5b8: ldr r1,[r5,#0x4]
  000cc5ba: ldr r0,[r0,#0x4]
  000cc5bc: ldr.w r2,[r1,r6,lsl #0x2]
  000cc5c0: ldr.w r1,[r0,r6,lsl #0x2]
  000cc5c4: add.w r0,r2,#0x18
  000cc5c8: blx 0x0006f2b0
  000cc5cc: ldr r1,[r5,#0x4]
  000cc5ce: ldr.w r0,[r10,#0x0]
  000cc5d2: ldr.w r1,[r1,r6,lsl #0x2]
  000cc5d6: ldr r2,[r0,#0x2c]
  000cc5d8: str r2,[r1,#0x28]
  000cc5da: blx 0x00072850
  000cc5de: blx 0x00072874
  000cc5e2: cmp r0,#0xc
  000cc5e4: bne 0x000cc5f2
  000cc5e6: ldr r0,[r5,#0x4]
  000cc5e8: movs r1,#0x0
  000cc5ea: ldr.w r0,[r0,r6,lsl #0x2]
  000cc5ee: strb.w r1,[r0,#0x25]
  000cc5f2: adds r6,#0x1
  000cc5f4: ldr r0,[r5,#0x0]
  000cc5f6: cmp r6,r0
  000cc5f8: bcc.w 0x000cc478
  000cc5fc: ldr.w r1,[r11,#0xf8]
  000cc600: cbz r1,0x000cc61e
  000cc602: movs r4,#0x0
  000cc604: b 0x000cc618
  000cc606: ldr r0,[r5,#0x4]
  000cc608: ldr.w r1,[r11,#0xf8]
  000cc60c: ldr.w r0,[r0,r4,lsl #0x2]
  000cc610: blx 0x000740f8
  000cc614: ldr r0,[r5,#0x0]
  000cc616: adds r4,#0x1
  000cc618: cmp r4,r0
  000cc61a: bcc 0x000cc606
  000cc61c: b 0x000cc622
  000cc61e: str.w r5,[r11,#0xf8]
  000cc622: ldr r0,[sp,#0x18]
  000cc624: ldr r0,[r0,#0x0]
  000cc626: blx 0x000718cc
  000cc62a: ldr r4,[sp,#0x10]
  000cc62c: ldr r0,[sp,#0x4c]
  000cc62e: ldr r1,[r4,#0x0]
  000cc630: subs r0,r1,r0
  000cc632: itttt eq
  000cc634: add.eq sp,#0x50
  000cc636: vpop.eq {d8}
  000cc63a: add.eq sp,#0x4
  000cc63c: pop.eq.w {r8,r9,r10,r11}
  000cc640: it eq
  000cc642: pop.eq {r4,r5,r6,r7,pc}
  000cc644: blx 0x0006e824
```
