# RadioMessage::triggered
elf 0x151694 body 1606
Sig: undefined __stdcall triggered(longlong param_1, PlayerEgo * param_2, LevelScript * param_3)

## decompile
```c

/* RadioMessage::triggered(long long, PlayerEgo*, LevelScript*) */

void RadioMessage::triggered(longlong param_1,PlayerEgo *param_2,LevelScript *param_3)

{
  bool bVar1;
  byte bVar2;
  RadioMessage *pRVar3;
  int iVar4;
  undefined4 uVar5;
  uint *puVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  RadioMessage RVar10;
  PlayerEgo *pPVar11;
  int iVar12;
  float unaff_r6;
  uint uVar13;
  int *piVar14;
  uint in_fpscr;
  uint uVar15;
  float fVar16;
  float fVar17;
  PlayerEgo *in_stack_00000000;
  
  pRVar3 = (RadioMessage *)param_1;
  piVar14 = *(int **)(DAT_00161a00 + 0x1616a4);
  iVar4 = *piVar14;
  if (pRVar3[0x20] != (RadioMessage)0x0) goto LAB_001616b2;
  switch(*(undefined4 *)(pRVar3 + 0x10)) {
  case 0:
    iVar12 = PlayerEgo::getRoute(in_stack_00000000);
    if (iVar12 == 0) break;
    PlayerEgo::getRoute(in_stack_00000000);
    iVar12 = Route::getCurrent();
    if (*(int *)(pRVar3 + 0x24) < iVar12) {
      RVar10 = (RadioMessage)(*(int *)(pRVar3 + 0x24) == *(int *)(pRVar3 + 0x14));
    }
    else {
      RVar10 = (RadioMessage)0x0;
    }
    PlayerEgo::getRoute(in_stack_00000000);
    uVar5 = Route::getCurrent();
    *(undefined4 *)(pRVar3 + 0x24) = uVar5;
    pRVar3[0x20] = RVar10;
    if (!(bool)RVar10) goto LAB_001616b2;
    goto LAB_00161bc2;
  case 1:
    Player::getEnemies();
    iVar12 = 0;
    do {
      if (*(int *)(pRVar3 + 0x18) <= iVar12) goto switchD_001616da_caseD_7;
      iVar7 = Player::isDead();
      iVar12 = iVar12 + 1;
    } while (iVar7 == 0);
    goto LAB_00161bbc;
  case 2:
    iVar12 = Player::getEnemies();
    for (iVar7 = 0; iVar7 < *(int *)(pRVar3 + 0x18); iVar7 = iVar7 + 1) {
      if ((*(char *)(*(int *)(*(int *)(iVar12 + 4) +
                             *(int *)(*(int *)(pRVar3 + 0x1c) + iVar7 * 4) * 4) + 0x5d) != '\0') &&
         (iVar8 = Player::isDead(), iVar8 != 0)) goto LAB_00161bbc;
    }
    break;
  case 3:
    iVar12 = Level::getEnemiesLeft((Level *)in_stack_00000000->m_pLevel);
    goto LAB_00161c1a;
  case 4:
    iVar12 = Level::getFriendsLeft((Level *)in_stack_00000000->m_pLevel);
    goto LAB_00161c1a;
  case 5:
    pPVar11 = *(PlayerEgo **)(pRVar3 + 0x14);
    iVar12 = (int)pPVar11 >> 0x1f;
    RVar10 = (RadioMessage)
             ((int)(param_3 + (-(uint)(param_2 < pPVar11) - iVar12)) < 0 ==
             (SBORROW4((int)param_3,iVar12) !=
             SBORROW4((int)param_3 - iVar12,(uint)(param_2 < pPVar11))));
LAB_0016195a:
    uVar5 = 0;
    pRVar3[0x20] = RVar10;
    if (RVar10 == (RadioMessage)0x0) goto LAB_001616b4;
    goto LAB_00161bc2;
  case 6:
    iVar12 = Radio::getMessage(*(Radio **)pRVar3,*(int *)(pRVar3 + 0x14));
    uVar13 = (uint)*(byte *)(iVar12 + 0x20);
    goto LAB_00161b6c;
  case 8:
    iVar12 = Player::getEnemies();
    for (iVar7 = 0; iVar7 < *(int *)(pRVar3 + 0x18); iVar7 = iVar7 + 1) {
      iVar8 = Player::isAsteroid(*(Player **)
                                  (*(int *)(iVar12 + 4) +
                                  *(int *)(*(int *)(pRVar3 + 0x1c) + iVar7 * 4) * 4));
      if ((iVar8 == 0) && (iVar8 = Player::isActive(), iVar8 != 0)) goto LAB_00161bbc;
    }
    break;
  case 9:
    Player::getEnemies();
    iVar12 = 0;
    do {
      if (*(int *)(pRVar3 + 0x18) <= iVar12) goto LAB_00161bbc;
      iVar7 = Player::isDead();
      iVar12 = iVar12 + 1;
    } while (iVar7 != 0);
    break;
  case 10:
    iVar12 = Player::getEnemies();
    for (iVar7 = 0; iVar7 < *(int *)(pRVar3 + 0x18); iVar7 = iVar7 + 1) {
      iVar8 = Player::isAsteroid(*(Player **)
                                  (*(int *)(iVar12 + 4) +
                                  *(int *)(*(int *)(pRVar3 + 0x1c) + iVar7 * 4) * 4));
      if (((iVar8 == 0) &&
          (*(char *)(*(int *)(*(int *)(iVar12 + 4) +
                             *(int *)(*(int *)(pRVar3 + 0x1c) + iVar7 * 4) * 4) + 0x5d) != '\0')) &&
         (iVar8 = Player::isActive(), iVar8 != 0)) goto LAB_00161bbc;
    }
    break;
  case 0xb:
    uVar13 = Objective::achieved(*(int *)(pRVar3 + 4));
    goto LAB_00161b6c;
  case 0xc:
    Player::getEnemies();
    iVar12 = 0;
    do {
      if (*(int *)(pRVar3 + 0x18) <= iVar12) goto switchD_001616da_caseD_7;
      iVar7 = Player::getHitpoints();
      iVar8 = Player::getMaxHitpoints();
      iVar12 = iVar12 + 1;
    } while (iVar8 / 2 <= iVar7);
    goto LAB_00161bbc;
  case 0xe:
    iVar12 = Player::getEnemies();
    iVar12 = Player::getKIPlayer(*(Player **)(*(int *)(iVar12 + 4) + *(int *)(pRVar3 + 0x14) * 4));
    uVar13 = (uint)*(byte *)(iVar12 + 0x69);
    goto LAB_00161b6c;
  case 0xf:
    puVar6 = (uint *)Player::getEnemies();
    for (uVar13 = 0; uVar13 < *puVar6; uVar13 = uVar13 + 1) {
      iVar12 = Player::isAsteroid(*(Player **)(puVar6[1] + uVar13 * 4));
      if ((iVar12 == 0) && (iVar12 = Player::isDead(), iVar12 != 0)) goto LAB_00161bbc;
    }
    break;
  case 0x10:
    puVar6 = (uint *)Player::getEnemies();
    for (uVar13 = 0; uVar13 < *puVar6; uVar13 = uVar13 + 1) {
      iVar12 = Player::isAsteroid(*(Player **)(puVar6[1] + uVar13 * 4));
      if (((iVar12 == 0) && (iVar12 = Player::isActive(), iVar12 != 0)) &&
         (iVar12 = Player::isAlwaysFriend(), iVar12 == 0)) goto LAB_00161bbc;
    }
    break;
  case 0x11:
    puVar6 = (uint *)Player::getEnemies();
    uVar13 = 0;
    while ((uVar13 < *puVar6 &&
           (((uVar13 == *(uint *)(pRVar3 + 0x14) ||
             (iVar12 = Player::isAsteroid(*(Player **)(puVar6[1] + uVar13 * 4)), iVar12 != 0)) ||
            (iVar12 = Player::isDead(), iVar12 == 0))))) {
      uVar13 = uVar13 + 1;
    }
    goto LAB_00161bbc;
  case 0x12:
    iVar12 = Player::getEnemies();
    iVar12 = Player::getKIPlayer(*(Player **)(*(int *)(iVar12 + 4) + *(int *)(pRVar3 + 0x14) * 4));
    if (*(char *)(iVar12 + 0x69) == '\0') {
      iVar12 = Player::getEnemies();
      iVar12 = Player::getKIPlayer(*(Player **)(*(int *)(iVar12 + 4) + *(int *)(pRVar3 + 0x14) * 4))
      ;
      uVar13 = (uint)*(byte *)(iVar12 + 0x6a);
      goto LAB_00161b6c;
    }
    goto LAB_00161bbc;
  case 0x13:
    Player::getEnemies();
    iVar12 = 0;
    do {
      if (*(int *)(pRVar3 + 0x18) <= iVar12) goto switchD_001616da_caseD_7;
      iVar7 = Player::getHitpoints();
      iVar8 = Player::getMaxHitpoints();
      iVar12 = iVar12 + 1;
    } while ((int)(iVar8 + ((uint)(iVar8 >> 0x1f) >> 0x1e)) >> 2 <= iVar7);
    goto LAB_00161bbc;
  case 0x14:
    puVar6 = (uint *)Player::getEnemies();
    iVar12 = 0;
    uVar13 = 0;
    do {
      if (*puVar6 <= uVar13) goto switchD_001616da_caseD_7;
      iVar7 = Player::isAsteroid(*(Player **)(puVar6[1] + uVar13 * 4));
      if (iVar7 == 0) {
        iVar7 = Player::isDead();
        iVar12 = iVar12 + iVar7;
      }
      uVar13 = uVar13 + 1;
    } while (iVar12 < *(int *)(pRVar3 + 0x14));
    goto LAB_00161bbc;
  case 0x15:
    iVar12 = Player::getEnemies();
    iVar12 = Player::getKIPlayer(*(Player **)(*(int *)(iVar12 + 4) + *(int *)(pRVar3 + 0x14) * 4));
    uVar13 = (uint)*(byte *)(iVar12 + 0x24);
    goto LAB_00161b6c;
  case 0x16:
    iVar12 = in_stack_00000000->m_pLevel->field_1C;
    uVar5 = 0;
    pRVar3[0x20] = (RadioMessage)(*(int *)(pRVar3 + 0x14) <= iVar12);
    if (iVar12 < *(int *)(pRVar3 + 0x14)) goto LAB_001616b4;
    goto LAB_00161bc2;
  case 0x17:
    uVar13 = Radar::stationLocked((Radar *)in_stack_00000000->field_14);
LAB_00161b6c:
    pRVar3[0x20] = SUB41(uVar13,0);
    if (uVar13 == 0) {
LAB_001616b2:
      uVar5 = 0;
      goto LAB_001616b4;
    }
    goto LAB_00161bc2;
  case 0x18:
    Player::getEnemies();
    iVar12 = Player::isActive();
    if (iVar12 == 0) {
      Player::getEnemies();
      bVar2 = Player::isDead();
      bVar2 = bVar2 ^ 1;
      bVar1 = (int)(-(uint)((PlayerEgo *)0xea5f < param_2) - (int)param_3) < 0 !=
              (SBORROW4(0,(int)param_3) !=
              SBORROW4(-(int)param_3,(uint)((PlayerEgo *)0xea5f < param_2)));
LAB_00161958:
      RVar10 = (RadioMessage)(bVar2 & bVar1);
      goto LAB_0016195a;
    }
    break;
  case 0x19:
    iVar12 = PlayerEgo::getRoute(in_stack_00000000);
    if (iVar12 != 0) {
      PlayerEgo::getRoute(in_stack_00000000);
      iVar12 = Route::getCurrent();
      iVar8 = *(int *)(pRVar3 + 0x24);
      PlayerEgo::getRoute(in_stack_00000000);
      uVar5 = Route::getCurrent();
      *(undefined4 *)(pRVar3 + 0x24) = uVar5;
      puVar6 = (uint *)Player::getEnemies();
      iVar7 = 0;
      for (uVar13 = 0; uVar13 < *puVar6; uVar13 = uVar13 + 1) {
        iVar9 = Player::isAsteroid(*(Player **)(puVar6[1] + uVar13 * 4));
        if (iVar9 == 0) {
          uVar15 = Player::isDead();
          iVar7 = iVar7 + (uVar15 ^ 1);
        }
      }
      bVar2 = *(int *)(pRVar3 + 0x14) <= iVar7;
      bVar1 = iVar8 < iVar12 && iVar8 == 0;
      goto LAB_00161958;
    }
    break;
  case 0x1a:
    Player::getEnemies();
    iVar12 = Player::isActive();
    if ((iVar12 == 0) || (iVar12 = Player::isDead(), iVar12 != 0)) break;
    Player::getPosition();
    fVar16 = (float)VectorSignedToFloat(*(undefined4 *)(pRVar3 + 0x14),(byte)(in_fpscr >> 0x16) & 3)
    ;
    fVar16 = unaff_r6 - fVar16;
    uVar13 = in_fpscr & 0xfffffff | (uint)(fVar16 < 0.0) << 0x1f | (uint)(fVar16 == 0.0) << 0x1e;
    uVar15 = uVar13 | (uint)NAN(fVar16) << 0x1c;
    bVar2 = (byte)(uVar13 >> 0x18);
    if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(uVar15 >> 0x1c) & 1)) {
      Player::getPosition();
      fVar16 = DAT_00161d20;
      uVar5 = 0;
      fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(pRVar3 + 0x14),(byte)(uVar15 >> 0x16) & 3)
      ;
      pRVar3[0x20] = (RadioMessage)(DAT_00161d20 < (float)param_3 - fVar17);
      if ((float)param_3 - fVar17 <= fVar16) goto LAB_001616b4;
    }
    else {
      Player::getPosition();
      uVar5 = 0;
      fVar16 = (float)VectorSignedToFloat(*(undefined4 *)(pRVar3 + 0x14),(byte)(uVar15 >> 0x16) & 3)
      ;
      iVar12 = (uint)((float)param_3 - fVar16 < DAT_00161d24) << 0x1f;
      pRVar3[0x20] = (RadioMessage)(iVar12 < 0);
      if (-1 < iVar12) goto LAB_001616b4;
    }
    goto LAB_00161bc2;
  case 0x1b:
    iVar7 = LevelScript::getEvent();
    iVar7 = iVar7 - *(int *)(pRVar3 + 0x14);
    goto LAB_00161c6a;
  case 0x1c:
    iVar12 = Player::getArmorHP();
LAB_00161c1a:
    uVar5 = 0;
    pRVar3[0x20] = (RadioMessage)(iVar12 < 1);
    if (0 < iVar12) goto LAB_001616b4;
    goto LAB_00161bc2;
  case 0x1e:
    iVar12 = Player::getEnemies();
    iVar7 = 0;
    for (iVar8 = 2; iVar8 != 6; iVar8 = iVar8 + 1) {
      iVar9 = Player::isAsteroid(*(Player **)(*(int *)(iVar12 + 4) + iVar8 * 4));
      if (iVar9 == 0) {
        iVar9 = Player::isDead();
        iVar7 = iVar7 + iVar9;
      }
    }
    iVar7 = iVar7 - *(int *)(pRVar3 + 0x14);
LAB_00161c6a:
    pRVar3[0x20] = (RadioMessage)(iVar7 == 0);
    if (iVar7 == 0) goto LAB_00161bc2;
    goto LAB_001616b2;
  case 0x1f:
    Player::getEnemies();
    iVar12 = 0;
    do {
      if (*(int *)(pRVar3 + 0x18) <= iVar12) goto switchD_001616da_caseD_7;
      iVar7 = Player::getHitpoints();
      iVar8 = Player::getMaxHitpoints();
      iVar12 = iVar12 + 1;
    } while (((int)(iVar8 + ((uint)(iVar8 >> 0x1f) >> 0x1e)) >> 2) * 3 <= iVar7);
LAB_00161bbc:
    pRVar3[0x20] = (RadioMessage)0x1;
LAB_00161bc2:
    Radio::setCurrentMessage(*(Radio **)pRVar3,pRVar3);
    uVar5 = 1;
    goto LAB_001616b4;
  }
switchD_001616da_caseD_7:
  uVar5 = 0;
  pRVar3[0x20] = (RadioMessage)0x0;
LAB_001616b4:
  if (*piVar14 != iVar4) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar5);
  }
  return;
}

```

## target disasm
```
  00161694: push {r4,r5,r6,r7,lr}
  00161696: add r7,sp,#0xc
  00161698: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0016169c: mov r4,r0
  0016169e: ldr r0,[0x00161a00]
  001616a0: add r0,pc
  001616a2: ldr.w r9,[r0,#0x0]
  001616a6: ldr.w r0,[r9,#0x0]
  001616aa: str r0,[sp,#0x18]
  001616ac: ldrb.w r0,[r4,#0x20]
  001616b0: cbz r0,0x001616ca
  001616b2: movs r0,#0x0
  001616b4: ldr r1,[sp,#0x18]
  001616b6: ldr.w r2,[r9,#0x0]
  001616ba: subs r1,r2,r1
  001616bc: ittt eq
  001616be: add.eq sp,#0x1c
  001616c0: pop.eq.w {r8,r9,r10,r11}
  001616c4: pop.eq {r4,r5,r6,r7,pc}
  001616c6: blx 0x0006e824
  001616ca: ldr r0,[r4,#0x10]
  001616cc: cmp r0,#0x1f
  001616ce: bhi.w 0x00161c0a
  001616d2: mov r5,r3
  001616d4: mov r6,r2
  001616d6: ldr.w r8,[r7,#0x8]
  001616da: tbh [pc,r0]
  0016171e: mov r0,r8
  00161720: blx 0x000768b8
  00161724: cmp r0,#0x0
  00161726: beq.w 0x00161c0a
  0016172a: mov r0,r8
  0016172c: blx 0x000768b8
  00161730: blx 0x00072988
  00161734: ldr r1,[r4,#0x24]
  00161736: cmp r0,r1
  00161738: ble.w 0x00161c7c
  0016173c: ldr r0,[r4,#0x14]
  0016173e: subs r0,r1,r0
  00161740: clz r0,r0
  00161744: lsrs r5,r0,#0x5
  00161746: b 0x00161c7e
  00161748: ldr.w r0,[r8,#0x0]
  0016174c: blx 0x0007582c
  00161750: mov r5,r0
  00161752: movs r6,#0x0
  00161754: b 0x00161774
  00161756: ldr r0,[r4,#0x1c]
  00161758: ldr r1,[r5,#0x4]
  0016175a: ldr.w r0,[r0,r6,lsl #0x2]
  0016175e: ldr.w r0,[r1,r0,lsl #0x2]
  00161762: ldrb.w r1,[r0,#0x5d]
  00161766: cbz r1,0x00161772
  00161768: blx 0x00075838
  0016176c: cmp r0,#0x0
  0016176e: bne.w 0x00161bbc
  00161772: adds r6,#0x1
  00161774: ldr r0,[r4,#0x18]
  00161776: cmp r6,r0
  00161778: blt 0x00161756
  0016177a: b 0x00161c0a
  0016177c: ldr r1,[r4,#0x14]
  0016177e: movs r0,#0x0
  00161780: subs r2,r6,r1
  00161782: sbcs.w r1,r5,r1, asr #0x1f
  00161786: mov.w r1,#0x0
  0016178a: it ge
  0016178c: mov.ge r1,#0x1
  0016178e: cmp r1,#0x0
  00161790: b 0x0016195a
  00161792: ldr r0,[r4,#0x4]
  00161794: mov r1,r6
  00161796: blx 0x000744dc
  0016179a: b 0x00161b6c
  0016179c: ldr.w r0,[r8,#0x0]
  001617a0: blx 0x0007582c
  001617a4: mov r8,r0
  001617a6: movs r5,#0x0
  001617a8: ldr r0,[r4,#0x18]
  001617aa: cmp r5,r0
  001617ac: bge.w 0x00161c0a
  001617b0: ldr r0,[r4,#0x1c]
  001617b2: ldr.w r1,[r8,#0x4]
  001617b6: ldr.w r0,[r0,r5,lsl #0x2]
  001617ba: ldr.w r0,[r1,r0,lsl #0x2]
  001617be: blx 0x000724f0
  001617c2: mov r6,r0
  001617c4: ldr r0,[r4,#0x1c]
  001617c6: ldr.w r1,[r8,#0x4]
  001617ca: ldr.w r0,[r0,r5,lsl #0x2]
  001617ce: ldr.w r0,[r1,r0,lsl #0x2]
  001617d2: blx 0x00072910
  001617d6: add.w r0,r0,r0, lsr #0x1f
  001617da: adds r5,#0x1
  001617dc: cmp.w r6,r0, asr #0x1
  001617e0: bge 0x001617a8
  001617e2: b 0x00161bbc
  001617e4: ldr.w r0,[r8,#0x0]
  001617e8: blx 0x0007582c
  001617ec: ldr r1,[r4,#0x14]
  001617ee: ldr r0,[r0,#0x4]
  001617f0: ldr.w r0,[r0,r1,lsl #0x2]
  001617f4: blx 0x00075850
  001617f8: ldrb.w r0,[r0,#0x69]
  001617fc: b 0x00161b6c
  001617fe: ldr.w r0,[r8,#0x0]
  00161802: blx 0x0007582c
  00161806: mov r5,r0
  00161808: movs r6,#0x0
  0016180a: b 0x0016182a
  0016180c: ldr r0,[r5,#0x4]
  0016180e: ldr.w r0,[r0,r6,lsl #0x2]
  00161812: blx 0x00077308
  00161816: cbnz r0,0x00161828
  00161818: ldr r0,[r5,#0x4]
  0016181a: ldr.w r0,[r0,r6,lsl #0x2]
  0016181e: blx 0x00075838
  00161822: cmp r0,#0x0
  00161824: bne.w 0x00161bbc
  00161828: adds r6,#0x1
  0016182a: ldr r0,[r5,#0x0]
  0016182c: cmp r6,r0
  0016182e: bcc 0x0016180c
  00161830: b 0x00161c0a
  00161832: ldr.w r0,[r8,#0x0]
  00161836: blx 0x0007582c
  0016183a: mov r5,r0
  0016183c: mov.w r8,#0x0
  00161840: movs r6,#0x0
  00161842: ldr r0,[r5,#0x0]
  00161844: cmp r6,r0
  00161846: bcs.w 0x00161c0a
  0016184a: ldr r0,[r5,#0x4]
  0016184c: ldr.w r0,[r0,r6,lsl #0x2]
  00161850: blx 0x00077308
  00161854: cbnz r0,0x00161862
  00161856: ldr r0,[r5,#0x4]
  00161858: ldr.w r0,[r0,r6,lsl #0x2]
  0016185c: blx 0x00075838
  00161860: add r8,r0
  00161862: ldr r0,[r4,#0x14]
  00161864: adds r6,#0x1
  00161866: cmp r8,r0
  00161868: blt 0x00161842
  0016186a: b 0x00161bbc
  0016186c: ldr.w r0,[r8,#0x0]
  00161870: blx 0x0007582c
  00161874: ldr r1,[r4,#0x14]
  00161876: ldr r0,[r0,#0x4]
  00161878: ldr.w r0,[r0,r1,lsl #0x2]
  0016187c: blx 0x00075850
  00161880: ldrb.w r0,[r0,#0x24]
  00161884: b 0x00161b6c
  00161886: ldr.w r0,[r8,#0x14]
  0016188a: blx 0x00077314
  0016188e: b 0x00161b6c
  00161890: ldr.w r0,[r8,#0x0]
  00161894: blx 0x0007582c
  00161898: ldr r1,[r4,#0x14]
  0016189a: ldr r0,[r0,#0x4]
  0016189c: ldr.w r0,[r0,r1,lsl #0x2]
  001618a0: blx 0x00071f14
  001618a4: cmp r0,#0x0
  001618a6: bne.w 0x00161c0a
  001618aa: ldr.w r0,[r8,#0x0]
  001618ae: blx 0x0007582c
  001618b2: ldr r1,[r4,#0x14]
  001618b4: ldr r0,[r0,#0x4]
  001618b6: ldr.w r0,[r0,r1,lsl #0x2]
  001618ba: blx 0x00075838
  001618be: movw r2,#0xea5f
  001618c2: mov r1,r0
  001618c4: subs r2,r2,r6
  001618c6: mov.w r0,#0x0
  001618ca: sbcs.w r2,r0,r5
  001618ce: eor r1,r1,#0x1
  001618d2: mov.w r2,#0x0
  001618d6: it lt
  001618d8: mov.lt r2,#0x1
  001618da: b 0x00161958
  001618dc: mov r0,r8
  001618de: blx 0x000768b8
  001618e2: cmp r0,#0x0
  001618e4: beq.w 0x00161c0a
  001618e8: mov r0,r8
  001618ea: blx 0x000768b8
  001618ee: blx 0x00072988
  001618f2: mov r10,r0
  001618f4: mov r0,r8
  001618f6: ldr.w r11,[r4,#0x24]
  001618fa: blx 0x000768b8
  001618fe: blx 0x00072988
  00161902: str r0,[r4,#0x24]
  00161904: ldr.w r0,[r8,#0x0]
  00161908: blx 0x0007582c
  0016190c: mov r5,r0
  0016190e: mov.w r8,#0x0
  00161912: movs r6,#0x0
  00161914: b 0x00161934
  00161916: ldr r0,[r5,#0x4]
  00161918: ldr.w r0,[r0,r6,lsl #0x2]
  0016191c: blx 0x00077308
  00161920: cbnz r0,0x00161932
  00161922: ldr r0,[r5,#0x4]
  00161924: ldr.w r0,[r0,r6,lsl #0x2]
  00161928: blx 0x00075838
  0016192c: eor r0,r0,#0x1
  00161930: add r8,r0
  00161932: adds r6,#0x1
  00161934: ldr r0,[r5,#0x0]
  00161936: cmp r6,r0
  00161938: bcc 0x00161916
  0016193a: ldr r1,[r4,#0x14]
  0016193c: clz r3,r11
  00161940: movs r2,#0x0
  00161942: movs r0,#0x0
  00161944: cmp r8,r1
  00161946: mov.w r1,#0x0
  0016194a: it ge
  0016194c: mov.ge r1,#0x1
  0016194e: cmp r10,r11
  00161950: it gt
  00161952: mov.gt r2,#0x1
  00161954: lsrs r3,r3,#0x5
  00161956: ands r2,r3
  00161958: ands r1,r2
  0016195a: strb.w r1,[r4,#0x20]
  0016195e: beq.w 0x001616b4
  00161962: b 0x00161bc2
  00161964: ldr r0,[r7,#0xc]
  00161966: blx 0x00077320
  0016196a: ldr r1,[r4,#0x14]
  0016196c: subs r0,r0,r1
  0016196e: b 0x00161c6a
  00161970: ldr.w r0,[r8,#0x0]
  00161974: blx 0x0007582c
  00161978: mov r8,r0
  0016197a: movs r5,#0x0
  0016197c: ldr r0,[r4,#0x18]
  0016197e: cmp r5,r0
  00161980: bge.w 0x00161c0a
  00161984: ldr r0,[r4,#0x1c]
  00161986: ldr.w r1,[r8,#0x4]
  0016198a: ldr.w r0,[r0,r5,lsl #0x2]
  0016198e: ldr.w r0,[r1,r0,lsl #0x2]
  00161992: blx 0x000724f0
  00161996: mov r6,r0
  00161998: ldr r0,[r4,#0x1c]
  0016199a: ldr.w r1,[r8,#0x4]
  0016199e: ldr.w r0,[r0,r5,lsl #0x2]
  001619a2: ldr.w r0,[r1,r0,lsl #0x2]
  001619a6: blx 0x00072910
  001619aa: asrs r1,r0,#0x1f
  001619ac: adds r5,#0x1
  001619ae: add.w r0,r0,r1, lsr #0x1e
  001619b2: asrs r0,r0,#0x2
  001619b4: add.w r0,r0,r0, lsl #0x1
  001619b8: cmp r6,r0
  001619ba: bge 0x0016197c
  001619bc: b 0x00161bbc
  001619be: ldr.w r0,[r8,#0x0]
  001619c2: blx 0x0007582c
  001619c6: mov r5,r0
  001619c8: movs r6,#0x0
  001619ca: ldr r0,[r4,#0x18]
  001619cc: cmp r6,r0
  001619ce: bge.w 0x00161c0a
  001619d2: ldr r0,[r4,#0x1c]
  001619d4: ldr r1,[r5,#0x4]
  001619d6: ldr.w r0,[r0,r6,lsl #0x2]
  001619da: ldr.w r0,[r1,r0,lsl #0x2]
  001619de: blx 0x00075838
  001619e2: adds r6,#0x1
  001619e4: cmp r0,#0x0
  001619e6: beq 0x001619ca
  001619e8: b 0x00161bbc
  001619ea: ldr.w r0,[r8,#0xc]
  001619ee: blx 0x00071ecc
  001619f2: b 0x00161c1a
  001619f4: ldr.w r0,[r8,#0xc]
  001619f8: blx 0x00071efc
  001619fc: b 0x00161c1a
  00161a04: ldr r0,[r4,#0x0]
  00161a06: ldr r1,[r4,#0x14]
  00161a08: blx 0x0007732c
  00161a0c: ldrb.w r0,[r0,#0x20]
  00161a10: b 0x00161b6c
  00161a12: ldr.w r0,[r8,#0x0]
  00161a16: blx 0x0007582c
  00161a1a: mov r5,r0
  00161a1c: movs r6,#0x0
  00161a1e: b 0x00161a4a
  00161a20: ldr r0,[r4,#0x1c]
  00161a22: ldr r1,[r5,#0x4]
  00161a24: ldr.w r0,[r0,r6,lsl #0x2]
  00161a28: ldr.w r0,[r1,r0,lsl #0x2]
  00161a2c: blx 0x00077308
  00161a30: cbnz r0,0x00161a48
  00161a32: ldr r0,[r4,#0x1c]
  00161a34: ldr r1,[r5,#0x4]
  00161a36: ldr.w r0,[r0,r6,lsl #0x2]
  00161a3a: ldr.w r0,[r1,r0,lsl #0x2]
  00161a3e: blx 0x00071f14
  00161a42: cmp r0,#0x0
  00161a44: bne.w 0x00161bbc
  00161a48: adds r6,#0x1
  00161a4a: ldr r0,[r4,#0x18]
  00161a4c: cmp r6,r0
  00161a4e: blt 0x00161a20
  00161a50: b 0x00161c0a
  00161a52: ldr.w r0,[r8,#0x0]
  00161a56: blx 0x0007582c
  00161a5a: mov r5,r0
  00161a5c: movs r6,#0x0
  00161a5e: ldr r0,[r4,#0x18]
  00161a60: cmp r6,r0
  00161a62: bge.w 0x00161bbc
  00161a66: ldr r0,[r4,#0x1c]
  00161a68: ldr r1,[r5,#0x4]
  00161a6a: ldr.w r0,[r0,r6,lsl #0x2]
  00161a6e: ldr.w r0,[r1,r0,lsl #0x2]
  00161a72: blx 0x00075838
  00161a76: adds r6,#0x1
  00161a78: cmp r0,#0x0
  00161a7a: bne 0x00161a5e
  00161a7c: b 0x00161c0a
  00161a7e: ldr.w r0,[r8,#0x0]
  00161a82: blx 0x0007582c
  00161a86: mov r5,r0
  00161a88: movs r6,#0x0
  00161a8a: b 0x00161abc
  00161a8c: ldr r0,[r4,#0x1c]
  00161a8e: ldr r1,[r5,#0x4]
  00161a90: ldr.w r0,[r0,r6,lsl #0x2]
  00161a94: ldr.w r0,[r1,r0,lsl #0x2]
  00161a98: blx 0x00077308
  00161a9c: cbnz r0,0x00161aba
  00161a9e: ldr r0,[r4,#0x1c]
  00161aa0: ldr r1,[r5,#0x4]
  00161aa2: ldr.w r0,[r0,r6,lsl #0x2]
  00161aa6: ldr.w r0,[r1,r0,lsl #0x2]
  00161aaa: ldrb.w r1,[r0,#0x5d]
  00161aae: cbz r1,0x00161aba
  00161ab0: blx 0x00071f14
  00161ab4: cmp r0,#0x0
  00161ab6: bne.w 0x00161bbc
  00161aba: adds r6,#0x1
  00161abc: ldr r0,[r4,#0x18]
  00161abe: cmp r6,r0
  00161ac0: blt 0x00161a8c
  00161ac2: b 0x00161c0a
  00161ac4: ldr.w r0,[r8,#0x0]
  00161ac8: blx 0x0007582c
  00161acc: mov r5,r0
  00161ace: movs r6,#0x0
  00161ad0: b 0x00161afa
  00161ad2: ldr r0,[r5,#0x4]
  00161ad4: ldr.w r0,[r0,r6,lsl #0x2]
  00161ad8: blx 0x00077308
  00161adc: cbnz r0,0x00161af8
  00161ade: ldr r0,[r5,#0x4]
  00161ae0: ldr.w r0,[r0,r6,lsl #0x2]
  00161ae4: blx 0x00071f14
  00161ae8: cbz r0,0x00161af8
  00161aea: ldr r0,[r5,#0x4]
  00161aec: ldr.w r0,[r0,r6,lsl #0x2]
  00161af0: blx 0x0007420c
  00161af4: cmp r0,#0x0
  00161af6: beq 0x00161bbc
  00161af8: adds r6,#0x1
  00161afa: ldr r0,[r5,#0x0]
  00161afc: cmp r6,r0
  00161afe: bcc 0x00161ad2
  00161b00: b 0x00161c0a
  00161b02: ldr.w r0,[r8,#0x0]
  00161b06: blx 0x0007582c
  00161b0a: mov r5,r0
  00161b0c: movs r6,#0x0
  00161b0e: b 0x00161b32
  00161b10: ldr r0,[r4,#0x14]
  00161b12: cmp r6,r0
  00161b14: beq 0x00161b30
  00161b16: ldr r0,[r5,#0x4]
  00161b18: ldr.w r0,[r0,r6,lsl #0x2]
  00161b1c: blx 0x00077308
  00161b20: cbnz r0,0x00161b30
  00161b22: ldr r0,[r5,#0x4]
  00161b24: ldr.w r0,[r0,r6,lsl #0x2]
  00161b28: blx 0x00075838
  00161b2c: cmp r0,#0x0
  00161b2e: bne 0x00161bbc
  00161b30: adds r6,#0x1
  00161b32: ldr r0,[r5,#0x0]
  00161b34: cmp r6,r0
  00161b36: bcc 0x00161b10
  00161b38: b 0x00161bbc
  00161b3a: ldr.w r0,[r8,#0x0]
  00161b3e: blx 0x0007582c
  00161b42: ldr r1,[r4,#0x14]
  00161b44: ldr r0,[r0,#0x4]
  00161b46: ldr.w r0,[r0,r1,lsl #0x2]
  00161b4a: blx 0x00075850
  00161b4e: ldrb.w r0,[r0,#0x69]
  00161b52: cbnz r0,0x00161bbc
  00161b54: ldr.w r0,[r8,#0x0]
  00161b58: blx 0x0007582c
  00161b5c: ldr r1,[r4,#0x14]
  00161b5e: ldr r0,[r0,#0x4]
  00161b60: ldr.w r0,[r0,r1,lsl #0x2]
  00161b64: blx 0x00075850
  00161b68: ldrb.w r0,[r0,#0x6a]
  00161b6c: cmp r0,#0x0
  00161b6e: strb.w r0,[r4,#0x20]
  00161b72: bne 0x00161bc2
  00161b74: b 0x001616b2
  00161b76: ldr.w r0,[r8,#0x0]
  00161b7a: blx 0x0007582c
  00161b7e: mov r8,r0
  00161b80: movs r5,#0x0
  00161b82: ldr r0,[r4,#0x18]
  00161b84: cmp r5,r0
  00161b86: bge 0x00161c0a
  00161b88: ldr r0,[r4,#0x1c]
  00161b8a: ldr.w r1,[r8,#0x4]
  00161b8e: ldr.w r0,[r0,r5,lsl #0x2]
  00161b92: ldr.w r0,[r1,r0,lsl #0x2]
  00161b96: blx 0x000724f0
  00161b9a: mov r6,r0
  00161b9c: ldr r0,[r4,#0x1c]
  00161b9e: ldr.w r1,[r8,#0x4]
  00161ba2: ldr.w r0,[r0,r5,lsl #0x2]
  00161ba6: ldr.w r0,[r1,r0,lsl #0x2]
  00161baa: blx 0x00072910
  00161bae: asrs r1,r0,#0x1f
  00161bb0: adds r5,#0x1
  00161bb2: add.w r0,r0,r1, lsr #0x1e
  00161bb6: cmp.w r6,r0, asr #0x2
  00161bba: bge 0x00161b82
  00161bbc: movs r0,#0x1
  00161bbe: strb.w r0,[r4,#0x20]
  00161bc2: ldr r0,[r4,#0x0]
  00161bc4: mov r1,r4
  00161bc6: blx 0x00077338
  00161bca: movs r0,#0x1
  00161bcc: b 0x001616b4
  00161bce: ldr.w r0,[r8,#0xc]
  00161bd2: ldr r1,[r4,#0x14]
  00161bd4: ldr r2,[r0,#0x1c]
  00161bd6: movs r0,#0x0
  00161bd8: cmp r2,r1
  00161bda: mov.w r1,#0x0
  00161bde: it ge
  00161be0: mov.ge r1,#0x1
  00161be2: strb.w r1,[r4,#0x20]
  00161be6: blt.w 0x001616b4
  00161bea: b 0x00161bc2
  00161bec: ldr.w r0,[r8,#0x0]
  00161bf0: blx 0x0007582c
  00161bf4: ldr r0,[r0,#0x4]
  00161bf6: ldr r5,[r0,#0x0]
  00161bf8: mov r0,r5
  00161bfa: blx 0x00071f14
  00161bfe: cbz r0,0x00161c0a
  00161c00: mov r0,r5
  00161c02: blx 0x00075838
  00161c06: cmp r0,#0x0
  00161c08: beq 0x00161c96
  00161c0a: movs r0,#0x0
  00161c0c: strb.w r0,[r4,#0x20]
  00161c10: b 0x001616b4
  00161c12: ldr.w r0,[r8,#0x0]
  00161c16: blx 0x00072928
  00161c1a: mov r1,r0
  00161c1c: cmp r1,#0x1
  00161c1e: mov.w r1,#0x0
  00161c22: mov.w r0,#0x0
  00161c26: it lt
  00161c28: mov.lt r1,#0x1
  00161c2a: strb.w r1,[r4,#0x20]
  00161c2e: bge.w 0x001616b4
  00161c32: b 0x00161bc2
  00161c34: ldr.w r0,[r8,#0x0]
  00161c38: blx 0x0007582c
  00161c3c: mov r5,r0
  00161c3e: movs r6,#0x2
  00161c40: mov.w r8,#0x0
  00161c44: b 0x00161c60
  00161c46: ldr r0,[r5,#0x4]
  00161c48: ldr.w r0,[r0,r6,lsl #0x2]
  00161c4c: blx 0x00077308
  00161c50: cbnz r0,0x00161c5e
  00161c52: ldr r0,[r5,#0x4]
  00161c54: ldr.w r0,[r0,r6,lsl #0x2]
  00161c58: blx 0x00075838
  00161c5c: add r8,r0
  00161c5e: adds r6,#0x1
  00161c60: cmp r6,#0x6
  00161c62: bne 0x00161c46
  00161c64: ldr r0,[r4,#0x14]
  00161c66: subs.w r0,r8,r0
  00161c6a: clz r0,r0
  00161c6e: lsr.w r0,r0,#0x5
  00161c72: strb.w r0,[r4,#0x20]
  00161c76: bne.w 0x001616b2
  00161c7a: b 0x00161bc2
  00161c7c: movs r5,#0x0
  00161c7e: mov r0,r8
  00161c80: blx 0x000768b8
  00161c84: blx 0x00072988
  00161c88: cmp r5,#0x0
  00161c8a: str r0,[r4,#0x24]
  00161c8c: strb.w r5,[r4,#0x20]
  00161c90: beq.w 0x001616b2
  00161c94: b 0x00161bc2
  00161c96: add r0,sp,#0xc
  00161c98: mov r1,r5
  00161c9a: blx 0x00072964
  00161c9e: vldr.32 s0,[r4,#0x14]
  00161ca2: mov r0,sp
  00161ca4: mov r1,r5
  00161ca6: vcvt.f32.s32 s0,s0
  00161caa: vldr.32 s2,[sp,#0x14]
  00161cae: vsub.f32 s0,s2,s0
  00161cb2: vcmpe.f32 s0,#0
  00161cb6: vmrs apsr,fpscr
  00161cba: ble 0x00161cee
  00161cbc: blx 0x00072964
  00161cc0: vldr.32 s0,[r4,#0x14]
  00161cc4: movs r1,#0x0
  00161cc6: movs r0,#0x0
  00161cc8: vcvt.f32.s32 s0,s0
  00161ccc: vldr.32 s2,[sp,#0x8]
  00161cd0: vsub.f32 s0,s2,s0
  00161cd4: vldr.32 s2,[pc,#0x4c]
  00161cd8: vcmpe.f32 s0,s2
  00161cdc: vmrs apsr,fpscr
  00161ce0: it mi
  00161ce2: mov.mi r1,#0x1
  00161ce4: strb.w r1,[r4,#0x20]
  00161ce8: bpl.w 0x001616b4
  00161cec: b 0x00161bc2
  00161cee: blx 0x00072964
  00161cf2: vldr.32 s0,[r4,#0x14]
  00161cf6: movs r1,#0x0
  00161cf8: movs r0,#0x0
  00161cfa: vcvt.f32.s32 s0,s0
  00161cfe: vldr.32 s2,[sp,#0x8]
  00161d02: vsub.f32 s0,s2,s0
  00161d06: vldr.32 s2,[pc,#0x18]
  00161d0a: vcmpe.f32 s0,s2
  00161d0e: vmrs apsr,fpscr
  00161d12: it gt
  00161d14: mov.gt r1,#0x1
  00161d16: strb.w r1,[r4,#0x20]
  00161d1a: ble.w 0x001616b4
  00161d1e: b 0x00161bc2
```
