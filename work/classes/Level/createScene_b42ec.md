# Level::createScene
elf 0xb42ec body 2092
Sig: undefined __thiscall createScene(Level * this)

## decompile
```c

/* Level::createScene() */

void __thiscall Level::createScene(Level *this)

{
  bool bVar1;
  float fVar2;
  int *piVar3;
  void *pvVar4;
  int iVar5;
  Station *pSVar6;
  uint uVar7;
  int *piVar8;
  Array<KIPlayer*> *pAVar9;
  Ship *this_00;
  int iVar10;
  undefined4 uVar11;
  PlayerStatic *pPVar12;
  AEGeometry *this_01;
  uint *puVar13;
  undefined4 *puVar14;
  int iVar15;
  int iVar16;
  KIPlayer *this_02;
  undefined4 *puVar17;
  int iVar18;
  uint uVar19;
  int iVar20;
  int *piVar21;
  int *piVar22;
  AEGeometry *pAVar23;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  undefined4 uVar24;
  float extraout_s0_05;
  float fVar25;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  float extraout_s2_03;
  float extraout_s2_04;
  float extraout_s2_05;
  float extraout_s2_06;
  undefined4 uVar26;
  undefined4 uVar27;
  uint local_74;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  int local_4c;
  
  piVar3 = *(int **)(DAT_000c46a0 + 0xc4302);
  local_4c = *piVar3;
  if (*(Array **)(this + 0xf8) != (Array *)0x0) {
    ArrayReleaseClasses<KIPlayer*>(*(Array **)(this + 0xf8));
    if (*(Array<KIPlayer*> **)(this + 0xf8) != (Array<KIPlayer*> *)0x0) {
      pvVar4 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0xf8));
      operator_delete(pvVar4);
    }
  }
  iVar5 = *(int *)(this + 0xc0);
  *(undefined4 *)(this + 0xf8) = 0;
  if (iVar5 == 2) {
    createPlayer();
    Status::setMission((Mission *)**(undefined4 **)(DAT_000c46ac + 0xc4368));
    createMission(this);
    iVar5 = Status::getCurrentCampaignMission();
    if (iVar5 == 0x2b) {
      pAVar23 = operator_new(0xc0);
      puVar14 = *(undefined4 **)(DAT_000c46b0 + 0xc4392);
      AEGeometry::AEGeometry(pAVar23,0x37d0,(PaintCanvas *)*puVar14,false);
      pPVar12 = operator_new(0x130);
      PlayerStatic::PlayerStatic(pPVar12,-1,pAVar23,extraout_s0,extraout_s1,extraout_s2);
      ArrayAdd<KIPlayer*>((KIPlayer *)pPVar12,*(Array **)(this + 0xf8));
      pAVar23 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar23,0x37d1,(PaintCanvas *)*puVar14,false);
      pPVar12 = operator_new(0x130);
      PlayerStatic::PlayerStatic(pPVar12,-1,pAVar23,extraout_s0_00,extraout_s1_00,extraout_s2_00);
      if (*piVar3 - local_4c != 0) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(*piVar3 - local_4c,*(undefined4 *)(this + 0xf8));
      }
      (*(code *)(DAT_001abfa4 + 0x1abfa8))();
      return;
    }
  }
  else {
    if (iVar5 == 4) {
      Status::getSystem();
      iVar5 = SolarSystem::getRace();
      uVar7 = 3;
      if (iVar5 == 1) {
        uVar7 = 2;
      }
      pSVar6 = (Station *)Status::getStation();
      piVar21 = (int *)Station::getAgents(pSVar6);
      pvVar4 = operator_new__(7);
      if (piVar21 == (int *)0x0) {
        pAVar9 = operator_new(0xc);
        Array<KIPlayer*>::Array(pAVar9);
        *(Array<KIPlayer*> **)(this + 0xf8) = pAVar9;
        ArraySetLength<KIPlayer*>(3,(Array *)pAVar9);
      }
      else {
        iVar20 = *piVar21;
        pAVar9 = operator_new(0xc);
        Array<KIPlayer*>::Array(pAVar9);
        *(Array<KIPlayer*> **)(this + 0xf8) = pAVar9;
        ArraySetLength<KIPlayer*>(iVar20 * 3 + uVar7,(Array *)pAVar9);
        for (iVar10 = 0; iVar10 != 7; iVar10 = iVar10 + 1) {
          *(undefined1 *)((int)pvVar4 + iVar10) = 0;
        }
        iVar10 = DAT_000c46b8 + 0xc44aa;
        piVar22 = *(int **)(DAT_000c46bc + 0xc44b0);
        iVar15 = DAT_000c46c0 + 0xc44ba;
        for (iVar16 = 0; iVar16 < iVar20; iVar16 = iVar16 + 1) {
          iVar18 = Agent::getRace(*(Agent **)(piVar21[1] + iVar16 * 4));
          if (iVar18 == 3) {
            iVar18 = Agent::getImageParts(*(Agent **)(piVar21[1] + iVar16 * 4));
            if (iVar18 == 0) {
              iVar18 = 3;
            }
            else {
              piVar8 = (int *)Agent::getImageParts(*(Agent **)(piVar21[1] + iVar16 * 4));
              iVar18 = *piVar8;
              if (iVar18 != 2) {
                iVar18 = 3;
              }
            }
          }
          uVar11 = *(undefined4 *)(iVar10 + iVar18 * 4);
          iVar18 = Agent::getRace(*(Agent **)(piVar21[1] + iVar16 * 4));
          if ((iVar18 == 0) &&
             (iVar18 = Agent::isMale(*(Agent **)(piVar21[1] + iVar16 * 4)), iVar18 == 0)) {
            uVar11 = 0x3984;
          }
          do {
            iVar18 = AbyssEngine::AERandom::nextInt(*piVar22);
          } while (*(char *)((int)pvVar4 + iVar18) != '\0');
          puVar14 = (undefined4 *)(iVar5 * 0x54 + iVar15 + iVar18 * 0xc);
          uVar24 = *puVar14;
          uVar27 = puVar14[1];
          uVar26 = puVar14[2];
          *(undefined1 *)((int)pvVar4 + iVar18) = 1;
          VectorSignedToFloat(uVar26,(byte)(in_fpscr >> 0x16) & 3);
          VectorSignedToFloat(uVar27,(byte)(in_fpscr >> 0x16) & 3);
          VectorSignedToFloat(uVar24,(byte)(in_fpscr >> 0x16) & 3);
          pAVar23 = operator_new(0xc0);
          puVar14 = *(undefined4 **)(DAT_000c46c4 + 0xc456c);
          AEGeometry::AEGeometry(pAVar23,(ushort)uVar11,(PaintCanvas *)*puVar14,false);
          pPVar12 = operator_new(0x130);
          PlayerStatic::PlayerStatic
                    (pPVar12,-1,pAVar23,extraout_s0_01,extraout_s1_01,extraout_s2_01);
          *(PlayerStatic **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar16 * 4) = pPVar12;
          pAVar23 = operator_new(0xc0);
          AEGeometry::AEGeometry
                    (pAVar23,*(ushort *)(DAT_000c46c8 + 0xc45ba + iVar5 * 4),(PaintCanvas *)*puVar14
                     ,false);
          pPVar12 = operator_new(0x130);
          PlayerStatic::PlayerStatic
                    (pPVar12,-1,pAVar23,extraout_s0_02,extraout_s1_02,extraout_s2_02);
          *(PlayerStatic **)(*(int *)(*(int *)(this + 0xf8) + 4) + (iVar20 + iVar16) * 4) = pPVar12;
          pAVar23 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar23,0x380c,(PaintCanvas *)*puVar14,false);
          pPVar12 = operator_new(0x130);
          PlayerStatic::PlayerStatic
                    (pPVar12,-1,pAVar23,extraout_s0_03,extraout_s1_03,extraout_s2_03);
          *(PlayerStatic **)(*(int *)(*(int *)(this + 0xf8) + 4) + (iVar20 * 2 + iVar16) * 4) =
               pPVar12;
        }
      }
      iVar10 = DAT_000c46cc + 0xc468a;
      puVar14 = *(undefined4 **)(DAT_000c46d0 + 0xc469c);
      for (uVar19 = 0; uVar19 < uVar7; uVar19 = uVar19 + 1) {
        pAVar23 = operator_new(0xc0);
        AEGeometry::AEGeometry
                  (pAVar23,*(ushort *)(iVar10 + iVar5 * 0xc + uVar19 * 4),(PaintCanvas *)*puVar14,
                   false);
        pPVar12 = operator_new(0x130);
        PlayerStatic::PlayerStatic(pPVar12,-1,pAVar23,extraout_s0_04,extraout_s1_04,extraout_s2_04);
        *(PlayerStatic **)
         ((*(int **)(this + 0xf8))[1] + (**(int **)(this + 0xf8) + (uVar19 - uVar7)) * 4) = pPVar12;
      }
      if (*piVar3 - local_4c != 0) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(*piVar3 - local_4c);
      }
      (*(code *)(DAT_001ab114 + 0x1ab118))();
      return;
    }
    if (iVar5 == 0x17) {
      piVar21 = *(int **)(DAT_000c46a4 + 0xc4340);
      pSVar6 = (Station *)Status::getStation();
      iVar5 = Station::getIndex(pSVar6);
      if (iVar5 == 0x65) {
        uVar7 = 8;
      }
      else {
        pSVar6 = (Station *)Status::getStation();
        iVar5 = Station::getIndex(pSVar6);
        if (iVar5 == 100) {
          uVar7 = 7;
        }
        else {
          Status::getSystem();
          uVar7 = SolarSystem::getRace();
        }
      }
      pAVar9 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar9);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar9;
      ArraySetLength<KIPlayer*>(1,(Array *)pAVar9);
      Status::getShip();
      iVar5 = Ship::getIndex();
      this_00 = (Ship *)Status::getShip();
      iVar10 = Ship::getRace(this_00);
      uVar11 = createShip(this,iVar10,0,iVar5,(Waypoint *)0x0,*(int *)(this + 0xc0) != 0x17,false);
      iVar10 = DAT_000c4b5c + 0xc47b2;
      uVar24 = VectorSignedToFloat(*(undefined4 *)(iVar10 + iVar5 * 4),(byte)(in_fpscr >> 0x16) & 3)
      ;
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
      (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
                ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),0,uVar24,0);
      PlayerFighter::removeTrail((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4));
      PlayerFighter::setExhaustVisible(SUB41(**(undefined4 **)(*(int *)(this + 0xf8) + 4),0));
      KIPlayer::setToSleep((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4));
      Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
      iVar5 = DAT_000c4b90 + 0xc4832;
      for (uVar19 = 0; uVar19 < 4; uVar19 = uVar19 + 1) {
        iVar20 = *(int *)(iVar5 + uVar7 * 0x10 + uVar19 * 4);
        if (-1 < iVar20) {
          pAVar23 = operator_new(0xc0);
          puVar14 = *(undefined4 **)(DAT_000c4b94 + 0xc4850);
          AEGeometry::AEGeometry(pAVar23,(ushort)iVar20,(PaintCanvas *)*puVar14,false);
          local_54 = DAT_000c4b8c;
          local_58 = 0;
          local_50 = 0;
          AEGeometry::setRotation(pAVar23,(Vector *)&local_58);
          pPVar12 = operator_new(0x130);
          PlayerStatic::PlayerStatic
                    (pPVar12,-1,pAVar23,extraout_s0_05,extraout_s1_05,extraout_s2_05);
          if ((int)uVar7 < 4 && uVar7 != 1) {
            iVar15 = *(int *)(DAT_000c4b98 + 0xc48a4 + uVar7 * 4);
            for (iVar20 = 0; iVar20 < iVar15; iVar20 = iVar20 + 1) {
              uVar11 = *(undefined4 *)(DAT_000c4b9c + 0xc48b0 + uVar7 * 4);
              this_01 = operator_new(0xc0);
              AEGeometry::AEGeometry
                        (this_01,(short)iVar20 + (short)uVar11,(PaintCanvas *)*puVar14,false);
              AEGeometry::addChild((uint)pAVar23);
              pvVar4 = (void *)AEGeometry::~AEGeometry(this_01);
              operator_delete(pvVar4);
            }
          }
          ArrayAdd<KIPlayer*>((KIPlayer *)pPVar12,*(Array **)(this + 0xf8));
        }
      }
      pSVar6 = (Station *)Status::getStation();
      iVar5 = Station::getIndex(pSVar6);
      if (iVar5 == 0x6c) {
        bVar1 = *(int *)(*piVar21 + 0x114) == 3;
      }
      else {
        bVar1 = false;
      }
      piVar21 = *(int **)(DAT_000c4ba0 + 0xc492c);
      uVar19 = *(uint *)(DAT_000c4ba4 + 0xc492e + uVar7 * 4);
      local_74 = AbyssEngine::AERandom::nextInt(*piVar21);
      if (bVar1) {
        puVar13 = (uint *)Station::getShips();
        if (puVar13 == (uint *)0x0) {
          local_74 = 0;
        }
        else {
          local_74 = *puVar13;
          if (uVar19 <= *puVar13) {
            local_74 = uVar19;
          }
        }
      }
      pvVar4 = operator_new__(uVar19);
      for (iVar5 = 0; fVar2 = DAT_000c4b88, iVar5 < (int)uVar19; iVar5 = iVar5 + 1) {
        *(undefined1 *)((int)pvVar4 + iVar5) = 0;
      }
      iVar20 = 0;
      iVar5 = DAT_000c4bac + 0xc498a;
      if (uVar7 != 7) {
        iVar5 = DAT_000c4ba8 + 0xc4988;
      }
      puVar14 = *(undefined4 **)(DAT_000c4bb0 + 0xc49aa);
      for (; iVar20 < (int)local_74; iVar20 = iVar20 + 1) {
        iVar15 = AbyssEngine::AERandom::nextInt(*piVar21);
        uVar19 = uVar7;
        if (iVar15 < 0x1e) {
          uVar19 = AbyssEngine::AERandom::nextInt(*piVar21);
          iVar15 = AbyssEngine::AERandom::nextInt(*piVar21);
          if (iVar15 < 0x1e) {
            uVar19 = 8;
          }
        }
        iVar15 = Globals::getRandomEnemyFighter((Globals *)*puVar14,uVar19);
        pSVar6 = (Station *)Status::getStation();
        iVar16 = Station::getIndex(pSVar6);
        if (iVar16 == 100) {
          iVar16 = AbyssEngine::AERandom::nextInt(*piVar21);
          iVar15 = 0x28;
          if (iVar16 == 1) {
            iVar15 = 0x26;
          }
          if (iVar16 == 0) {
            iVar15 = 0x25;
          }
        }
        if (bVar1) {
          Station::getShips();
          iVar15 = Ship::getIndex();
        }
        this_02 = (KIPlayer *)createShip(this,0,0,iVar15,(Waypoint *)0x0,false,false);
        iVar16 = AbyssEngine::AERandom::nextInt(*piVar21);
        iVar18 = -100;
        while ((iVar18 != 0 && (*(char *)((int)pvVar4 + iVar16) != '\0'))) {
          iVar16 = AbyssEngine::AERandom::nextInt(*piVar21);
          iVar18 = iVar18 + 1;
        }
        *(undefined1 *)((int)pvVar4 + iVar16) = 1;
        iVar18 = iVar5;
        if ((uVar7 < 9) && ((0x10bU >> (uVar7 & 0xff) & 1) != 0)) {
          iVar18 = *(int *)(DAT_000c4bb4 + 0xc4a86 + uVar7 * 4);
        }
        puVar17 = (undefined4 *)(iVar18 + iVar16 * 0xc);
        uVar11 = VectorSignedToFloat(*puVar17,(byte)(in_fpscr >> 0x16) & 3);
        uVar24 = VectorSignedToFloat(puVar17[2],(byte)(in_fpscr >> 0x16) & 3);
        uVar27 = VectorSignedToFloat(puVar17[1] + *(int *)(iVar10 + iVar15 * 4),
                                     (byte)(in_fpscr >> 0x16) & 3);
        (**(code **)(*(int *)this_02 + 0x48))(this_02,uVar11,uVar27,uVar24);
        Player::setAlwaysFriend(*(Player **)(this_02 + 4),true);
        KIPlayer::setToSleep(this_02);
        pAVar23 = *(AEGeometry **)(this_02 + 8);
        uVar11 = AbyssEngine::AERandom::nextInt(*piVar21);
        fVar25 = (float)VectorSignedToFloat(uVar11,(byte)(in_fpscr >> 0x16) & 3);
        AEGeometry::setRotation(pAVar23,fVar25 / fVar2,extraout_s1_06,extraout_s2_06);
        PlayerFighter::setExhaustVisible(SUB41(this_02,0));
        ArrayAdd<KIPlayer*>(this_02,*(Array **)(this + 0xf8));
      }
      operator_delete__(pvVar4);
    }
  }
  if (*piVar3 != local_4c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000c42ec: push {r4,r5,r6,r7,lr}
  000c42ee: add r7,sp,#0xc
  000c42f0: push {r7,r8,r9,r10,r11}
  000c42f4: vpush {d8,d9,d10,d11}
  000c42f8: sub sp,#0x48
  000c42fa: mov r6,r0
  000c42fc: ldr r0,[0x000c46a0]
  000c42fe: add r0,pc
  000c4300: ldr r0,[r0,#0x0]
  000c4302: str r0,[sp,#0x24]
  000c4304: ldr r0,[r0,#0x0]
  000c4306: str r0,[sp,#0x44]
  000c4308: ldr.w r0,[r6,#0xf8]
  000c430c: cbz r0,0x000c4320
  000c430e: blx 0x00073b88
  000c4312: ldr.w r0,[r6,#0xf8]
  000c4316: cbz r0,0x000c4320
  000c4318: blx 0x00073b94
  000c431c: blx 0x0006eb48
  000c4320: ldr.w r0,[r6,#0xc0]
  000c4324: movs r1,#0x0
  000c4326: str.w r1,[r6,#0xf8]
  000c432a: cmp r0,#0x2
  000c432c: beq 0x000c4358
  000c432e: cmp r0,#0x4
  000c4330: str r6,[sp,#0x30]
  000c4332: beq 0x000c442a
  000c4334: cmp r0,#0x17
  000c4336: bne.w 0x000c4b2c
  000c433a: ldr r0,[0x000c46a4]
  000c433c: add r0,pc
  000c433e: ldr.w r10,[r0,#0x0]
  000c4342: ldr.w r0,[r10,#0x0]
  000c4346: blx 0x00071c14
  000c434a: blx 0x00071824
  000c434e: cmp r0,#0x65
  000c4350: bne.w 0x000c4652
  000c4354: movs r0,#0x8
  000c4356: b 0x000c4758
  000c4358: mov r0,r6
  000c435a: blx 0x00073c00
  000c435e: ldr r0,[0x000c46a8]
  000c4360: ldr r1,[0x000c46ac]
  000c4362: add r0,pc
  000c4364: add r1,pc
  000c4366: ldr r0,[r0,#0x0]
  000c4368: ldr r4,[r1,#0x0]
  000c436a: ldr r1,[r0,#0x0]
  000c436c: ldr r0,[r4,#0x0]
  000c436e: blx 0x00073c3c
  000c4372: mov r0,r6
  000c4374: blx 0x00073c54
  000c4378: ldr r0,[r4,#0x0]
  000c437a: blx 0x00071770
  000c437e: cmp r0,#0x2b
  000c4380: bne.w 0x000c4b2c
  000c4384: movs r0,#0xc0
  000c4386: blx 0x0006eb24
  000c438a: mov r5,r0
  000c438c: ldr r0,[0x000c46b0]
  000c438e: add r0,pc
  000c4390: ldr.w r9,[r0,#0x0]
  000c4394: ldr.w r2,[r9,#0x0]
  000c4398: mov r0,r5
  000c439a: movw r1,#0x37d0
  000c439e: movs r3,#0x0
  000c43a0: mov.w r8,#0x0
  000c43a4: blx 0x0007207c
  000c43a8: mov.w r0,#0x130
  000c43ac: blx 0x0006eb24
  000c43b0: mov r4,r0
  000c43b2: mov.w r1,#0xffffffff
  000c43b6: mov r2,r5
  000c43b8: movs r3,#0x0
  000c43ba: strd r8,r8,[sp,#0x0]
  000c43be: blx 0x000740ec
  000c43c2: ldr.w r1,[r6,#0xf8]
  000c43c6: mov r0,r4
  000c43c8: blx 0x000740f8
  000c43cc: movs r0,#0xc0
  000c43ce: blx 0x0006eb24
  000c43d2: mov r5,r0
  000c43d4: ldr.w r2,[r9,#0x0]
  000c43d8: movw r1,#0x37d1
  000c43dc: movs r3,#0x0
  000c43de: mov.w r8,#0x0
  000c43e2: blx 0x0007207c
  000c43e6: mov.w r0,#0x130
  000c43ea: blx 0x0006eb24
  000c43ee: mov r4,r0
  000c43f0: mov.w r1,#0xffffffff
  000c43f4: mov r2,r5
  000c43f6: movs r3,#0x0
  000c43f8: strd r8,r8,[sp,#0x0]
  000c43fc: blx 0x000740ec
  000c4400: ldr.w r1,[r6,#0xf8]
  000c4404: ldr r0,[sp,#0x44]
  000c4406: ldr r2,[sp,#0x24]
  000c4408: ldr r2,[r2,#0x0]
  000c440a: subs r0,r2,r0
  000c440c: itttt eq
  000c440e: mov.eq r0,r4
  000c4410: add.eq sp,#0x48
  000c4412: vpop.eq {d8,d9,d10,d11}
  000c4416: add.eq sp,#0x4
  000c4418: ittt eq
  000c441a: pop.eq.w {r8,r9,r10,r11}
  000c441e: pop.eq.w {r4,r5,r6,r7,lr}
  000c4422: b.eq.w 0x001abf98
  000c4426: blx 0x0006e824
  000c442a: ldr r0,[0x000c46b4]
  000c442c: add r0,pc
  000c442e: ldr r4,[r0,#0x0]
  000c4430: ldr r0,[r4,#0x0]
  000c4432: blx 0x00071a10
  000c4436: blx 0x00071950
  000c443a: mov r2,r0
  000c443c: ldr r0,[r4,#0x0]
  000c443e: movs r1,#0x3
  000c4440: str r2,[sp,#0x28]
  000c4442: cmp r2,#0x1
  000c4444: it eq
  000c4446: mov.eq r1,#0x2
  000c4448: str r1,[sp,#0x34]
  000c444a: blx 0x00071c14
  000c444e: blx 0x00074104
  000c4452: mov r11,r0
  000c4454: movs r0,#0x7
  000c4456: blx 0x0006ec08
  000c445a: mov r5,r0
  000c445c: cmp.w r11,#0x0
  000c4460: beq.w 0x000c4666
  000c4464: movs r0,#0xc
  000c4466: ldr.w r9,[r11,#0x0]
  000c446a: blx 0x0006eb24
  000c446e: mov r4,r0
  000c4470: blx 0x00073d08
  000c4474: ldr r1,[sp,#0x34]
  000c4476: add.w r0,r9,r9, lsl #0x1
  000c447a: str.w r4,[r6,#0xf8]
  000c447e: add r0,r1
  000c4480: mov r1,r4
  000c4482: blx 0x00073d14
  000c4486: movs r0,#0x0
  000c4488: movs r1,#0x0
  000c448a: b 0x000c4490
  000c448c: strb r0,[r5,r1]
  000c448e: adds r1,#0x1
  000c4490: cmp r1,#0x7
  000c4492: bne 0x000c448c
  000c4494: lsl.w r0,r9,#0x1
  000c4498: vmov.f32 s16,0x41a00000
  000c449c: str r0,[sp,#0x14]
  000c449e: movs r1,#0x54
  000c44a0: ldr r0,[0x000c46b8]
  000c44a2: mov.w r8,#0x0
  000c44a6: add r0,pc
  000c44a8: str r0,[sp,#0x20]
  000c44aa: ldr r0,[0x000c46bc]
  000c44ac: add r0,pc
  000c44ae: ldr.w r10,[r0,#0x0]
  000c44b2: ldr r0,[0x000c46c0]
  000c44b4: ldr r2,[sp,#0x28]
  000c44b6: add r0,pc
  000c44b8: mla r0,r2,r1,r0
  000c44bc: str r0,[sp,#0x1c]
  000c44be: str.w r11,[sp,#0x18]
  000c44c2: str.w r9,[sp,#0x2c]
  000c44c6: b 0x000c464a
  000c44c8: ldr.w r0,[r11,#0x4]
  000c44cc: ldr.w r0,[r0,r8,lsl #0x2]
  000c44d0: blx 0x00071bd8
  000c44d4: cmp r0,#0x3
  000c44d6: bne 0x000c44fe
  000c44d8: ldr.w r0,[r11,#0x4]
  000c44dc: ldr.w r0,[r0,r8,lsl #0x2]
  000c44e0: blx 0x00071c74
  000c44e4: cbz r0,0x000c44fc
  000c44e6: ldr.w r0,[r11,#0x4]
  000c44ea: ldr.w r0,[r0,r8,lsl #0x2]
  000c44ee: blx 0x00071c74
  000c44f2: ldr r0,[r0,#0x0]
  000c44f4: cmp r0,#0x2
  000c44f6: it ne
  000c44f8: mov.ne r0,#0x3
  000c44fa: b 0x000c44fe
  000c44fc: movs r0,#0x3
  000c44fe: ldr.w r1,[r11,#0x4]
  000c4502: ldr r2,[sp,#0x20]
  000c4504: ldr.w r1,[r1,r8,lsl #0x2]
  000c4508: ldr.w r4,[r2,r0,lsl #0x2]
  000c450c: mov r0,r1
  000c450e: blx 0x00071bd8
  000c4512: cbnz r0,0x000c4528
  000c4514: ldr.w r0,[r11,#0x4]
  000c4518: ldr.w r0,[r0,r8,lsl #0x2]
  000c451c: blx 0x00074110
  000c4520: cmp r0,#0x0
  000c4522: it eq
  000c4524: movw.eq r4,#0x3984
  000c4528: ldr.w r0,[r10,#0x0]
  000c452c: movs r1,#0x7
  000c452e: blx 0x00071848
  000c4532: ldrb r1,[r5,r0]
  000c4534: cmp r1,#0x0
  000c4536: bne 0x000c4528
  000c4538: add.w r1,r0,r0, lsl #0x1
  000c453c: ldr r2,[sp,#0x1c]
  000c453e: add.w r1,r2,r1, lsl #0x2
  000c4542: vldr.32 s0,[r1]
  000c4546: vldr.32 s2,[r1,#0x4]
  000c454a: vldr.32 s4,[r1,#0x8]
  000c454e: movs r1,#0x1
  000c4550: strb r1,[r5,r0]
  000c4552: movs r0,#0xc0
  000c4554: vcvt.f32.s32 s18,s4
  000c4558: vcvt.f32.s32 s20,s2
  000c455c: vcvt.f32.s32 s22,s0
  000c4560: blx 0x0006eb24
  000c4564: mov r6,r0
  000c4566: ldr r0,[0x000c46c4]
  000c4568: add r0,pc
  000c456a: ldr.w r9,[r0,#0x0]
  000c456e: ldr.w r2,[r9,#0x0]
  000c4572: uxth r1,r4
  000c4574: mov r0,r6
  000c4576: movs r3,#0x0
  000c4578: blx 0x0007207c
  000c457c: mov.w r0,#0x130
  000c4580: blx 0x0006eb24
  000c4584: vmov r11,s22
  000c4588: mov r4,r0
  000c458a: mov.w r1,#0xffffffff
  000c458e: mov r2,r6
  000c4590: mov r3,r11
  000c4592: vstr.32 s20,[sp]
  000c4596: vstr.32 s18,[sp,#0x4]
  000c459a: blx 0x000740ec
  000c459e: ldr r0,[sp,#0x30]
  000c45a0: ldr.w r0,[r0,#0xf8]
  000c45a4: ldr r0,[r0,#0x4]
  000c45a6: str.w r4,[r0,r8,lsl #0x2]
  000c45aa: movs r0,#0xc0
  000c45ac: blx 0x0006eb24
  000c45b0: mov r6,r0
  000c45b2: ldr r0,[0x000c46c8]
  000c45b4: ldr r1,[sp,#0x28]
  000c45b6: add r0,pc
  000c45b8: ldr.w r2,[r9,#0x0]
  000c45bc: ldrh.w r1,[r0,r1,lsl #0x2]
  000c45c0: mov r0,r6
  000c45c2: movs r3,#0x0
  000c45c4: blx 0x0007207c
  000c45c8: mov.w r0,#0x130
  000c45cc: blx 0x0006eb24
  000c45d0: mov r4,r0
  000c45d2: mov.w r1,#0xffffffff
  000c45d6: mov r2,r6
  000c45d8: mov r3,r11
  000c45da: vstr.32 s20,[sp]
  000c45de: vstr.32 s18,[sp,#0x4]
  000c45e2: blx 0x000740ec
  000c45e6: ldr r0,[sp,#0x30]
  000c45e8: ldr r1,[sp,#0x2c]
  000c45ea: ldr.w r0,[r0,#0xf8]
  000c45ee: add r1,r8
  000c45f0: ldr r0,[r0,#0x4]
  000c45f2: str.w r4,[r0,r1,lsl #0x2]
  000c45f6: movs r0,#0xc0
  000c45f8: blx 0x0006eb24
  000c45fc: mov r6,r0
  000c45fe: ldr.w r2,[r9,#0x0]
  000c4602: movw r1,#0x380c
  000c4606: movs r3,#0x0
  000c4608: blx 0x0007207c
  000c460c: mov.w r0,#0x130
  000c4610: vadd.f32 s20,s20,s16
  000c4614: blx 0x0006eb24
  000c4618: mov r4,r0
  000c461a: mov.w r1,#0xffffffff
  000c461e: mov r2,r6
  000c4620: mov r3,r11
  000c4622: vstr.32 s20,[sp]
  000c4626: vstr.32 s18,[sp,#0x4]
  000c462a: blx 0x000740ec
  000c462e: ldr r0,[sp,#0x30]
  000c4630: ldr r1,[sp,#0x14]
  000c4632: ldr.w r11,[sp,#0x18]
  000c4636: ldr.w r0,[r0,#0xf8]
  000c463a: add r1,r8
  000c463c: add.w r8,r8,#0x1
  000c4640: ldr.w r9,[sp,#0x2c]
  000c4644: ldr r0,[r0,#0x4]
  000c4646: str.w r4,[r0,r1,lsl #0x2]
  000c464a: cmp r8,r9
  000c464c: blt.w 0x000c44c8
  000c4650: b 0x000c467e
  000c4652: ldr.w r0,[r10,#0x0]
  000c4656: blx 0x00071c14
  000c465a: blx 0x00071824
  000c465e: cmp r0,#0x64
  000c4660: bne 0x000c474c
  000c4662: movs r0,#0x7
  000c4664: b 0x000c4758
  000c4666: movs r0,#0xc
  000c4668: blx 0x0006eb24
  000c466c: mov r4,r0
  000c466e: blx 0x00073d08
  000c4672: movs r0,#0x3
  000c4674: mov r1,r4
  000c4676: str.w r4,[r6,#0xf8]
  000c467a: blx 0x00073d14
  000c467e: ldr r0,[0x000c46cc]
  000c4680: mov.w r10,#0x0
  000c4684: ldr r1,[sp,#0x28]
  000c4686: add r0,pc
  000c4688: add.w r1,r1,r1, lsl #0x1
  000c468c: add.w r8,r0,r1, lsl #0x2
  000c4690: ldr r0,[sp,#0x34]
  000c4692: rsb.w r9,r0,#0x0
  000c4696: ldr r0,[0x000c46d0]
  000c4698: add r0,pc
  000c469a: ldr.w r11,[r0,#0x0]
  000c469e: b 0x000c4720
  000c46d4: movs r0,#0xc0
  000c46d6: blx 0x0006eb24
  000c46da: ldrh.w r1,[r8,r10,lsl #0x2]
  000c46de: mov r6,r0
  000c46e0: ldr.w r2,[r11,#0x0]
  000c46e4: movs r3,#0x0
  000c46e6: blx 0x0007207c
  000c46ea: mov.w r0,#0x130
  000c46ee: blx 0x0006eb24
  000c46f2: mov r4,r0
  000c46f4: movs r0,#0x0
  000c46f6: mov.w r1,#0xffffffff
  000c46fa: strd r0,r0,[sp,#0x0]
  000c46fe: mov r0,r4
  000c4700: mov r2,r6
  000c4702: movs r3,#0x0
  000c4704: blx 0x000740ec
  000c4708: ldr r0,[sp,#0x30]
  000c470a: add.w r2,r9,r10
  000c470e: add.w r10,r10,#0x1
  000c4712: ldr.w r0,[r0,#0xf8]
  000c4716: ldrd r1,r0,[r0,#0x0]
  000c471a: add r1,r2
  000c471c: str.w r4,[r0,r1,lsl #0x2]
  000c4720: ldr r0,[sp,#0x34]
  000c4722: cmp r10,r0
  000c4724: bcc 0x000c46d4
  000c4726: ldr r0,[sp,#0x44]
  000c4728: ldr r1,[sp,#0x24]
  000c472a: ldr r1,[r1,#0x0]
  000c472c: subs r0,r1,r0
  000c472e: itttt eq
  000c4730: mov.eq r0,r5
  000c4732: add.eq sp,#0x48
  000c4734: vpop.eq {d8,d9,d10,d11}
  000c4738: add.eq sp,#0x4
  000c473a: ittt eq
  000c473c: pop.eq.w {r8,r9,r10,r11}
  000c4740: pop.eq.w {r4,r5,r6,r7,lr}
  000c4744: b.eq.w 0x001ab108
  000c4748: blx 0x0006e824
  000c474c: ldr.w r0,[r10,#0x0]
  000c4750: blx 0x00071a10
  000c4754: blx 0x00071950
  000c4758: str r0,[sp,#0x34]
  000c475a: movs r0,#0xc
  000c475c: blx 0x0006eb24
  000c4760: mov r4,r0
  000c4762: blx 0x00073d08
  000c4766: movs r0,#0x1
  000c4768: mov r1,r4
  000c476a: str.w r4,[r6,#0xf8]
  000c476e: blx 0x00073d14
  000c4772: ldr.w r0,[r10,#0x0]
  000c4776: blx 0x00071a58
  000c477a: blx 0x000719c8
  000c477e: mov r4,r0
  000c4780: ldr.w r0,[r10,#0x0]
  000c4784: blx 0x00071a58
  000c4788: blx 0x00073db0
  000c478c: mov r1,r0
  000c478e: ldr.w r0,[r6,#0xc0]
  000c4792: movs r2,#0x0
  000c4794: mov r3,r4
  000c4796: subs r0,#0x17
  000c4798: it ne
  000c479a: mov.ne r0,#0x1
  000c479c: strd r2,r0,[sp,#0x0]
  000c47a0: mov r0,r6
  000c47a2: str r2,[sp,#0x8]
  000c47a4: movs r2,#0x0
  000c47a6: blx 0x00073e88
  000c47aa: ldr r1,[0x000c4b5c]
  000c47ac: movs r3,#0x0
  000c47ae: add r1,pc
  000c47b0: str r1,[sp,#0x20]
  000c47b2: add.w r1,r1,r4, lsl #0x2
  000c47b6: vldr.32 s0,[r1]
  000c47ba: vcvt.f32.s32 s0,s0
  000c47be: ldr.w r1,[r6,#0xf8]
  000c47c2: ldr r1,[r1,#0x4]
  000c47c4: str r0,[r1,#0x0]
  000c47c6: vmov r2,s0
  000c47ca: ldr.w r0,[r6,#0xf8]
  000c47ce: ldr r0,[r0,#0x4]
  000c47d0: ldr r0,[r0,#0x0]
  000c47d2: ldr r1,[r0,#0x0]
  000c47d4: ldr r5,[r1,#0x48]
  000c47d6: movs r1,#0x0
  000c47d8: blx r5
  000c47da: ldr.w r0,[r6,#0xf8]
  000c47de: ldr r0,[r0,#0x4]
  000c47e0: ldr r0,[r0,#0x0]
  000c47e2: blx 0x0007411c
  000c47e6: ldr.w r0,[r6,#0xf8]
  000c47ea: movs r1,#0x0
  000c47ec: ldr r0,[r0,#0x4]
  000c47ee: ldr r0,[r0,#0x0]
  000c47f0: blx 0x000721f0
  000c47f4: ldr.w r0,[r6,#0xf8]
  000c47f8: ldr r0,[r0,#0x4]
  000c47fa: ldr r0,[r0,#0x0]
  000c47fc: blx 0x000732dc
  000c4800: ldr.w r0,[r6,#0xf8]
  000c4804: movs r1,#0x1
  000c4806: ldr r0,[r0,#0x4]
  000c4808: ldr r0,[r0,#0x0]
  000c480a: ldr r0,[r0,#0x4]
  000c480c: blx 0x00073fb4
  000c4810: ldr r2,[sp,#0x34]
  000c4812: movs r0,#0x0
  000c4814: movs r6,#0x0
  000c4816: subs r1,r2,#0x1
  000c4818: cmp r2,#0x3
  000c481a: clz r1,r1
  000c481e: it gt
  000c4820: mov.gt r0,#0x1
  000c4822: lsrs r1,r1,#0x5
  000c4824: orrs r0,r1
  000c4826: str r0,[sp,#0x1c]
  000c4828: ldr r0,[0x000c4b90]
  000c482a: str.w r10,[sp,#0x28]
  000c482e: add r0,pc
  000c4830: add.w r9,r0,r2, lsl #0x4
  000c4834: str.w r9,[sp,#0x18]
  000c4838: b 0x000c48fc
  000c483a: ldr.w r4,[r9,r6,lsl #0x2]
  000c483e: cmp r4,#0x0
  000c4840: blt 0x000c48fa
  000c4842: movs r0,#0xc0
  000c4844: blx 0x0006eb24
  000c4848: mov r5,r0
  000c484a: ldr r0,[0x000c4b94]
  000c484c: add r0,pc
  000c484e: ldr.w r8,[r0,#0x0]
  000c4852: ldr.w r2,[r8,#0x0]
  000c4856: uxth r1,r4
  000c4858: mov r0,r5
  000c485a: movs r3,#0x0
  000c485c: blx 0x0007207c
  000c4860: ldr r0,[0x000c4b8c]
  000c4862: add r1,sp,#0x38
  000c4864: str r0,[sp,#0x3c]
  000c4866: mov.w r10,#0x0
  000c486a: mov r0,r5
  000c486c: str.w r10,[sp,#0x38]
  000c4870: str.w r10,[sp,#0x40]
  000c4874: blx 0x00073dd4
  000c4878: mov.w r0,#0x130
  000c487c: blx 0x0006eb24
  000c4880: mov r4,r0
  000c4882: mov.w r1,#0xffffffff
  000c4886: mov r2,r5
  000c4888: movs r3,#0x0
  000c488a: strd r10,r10,[sp,#0x0]
  000c488e: blx 0x000740ec
  000c4892: str r4,[sp,#0x2c]
  000c4894: ldr r0,[sp,#0x1c]
  000c4896: cbnz r0,0x000c48e6
  000c4898: ldr r0,[0x000c4b98]
  000c489a: mov.w r10,#0x0
  000c489e: ldr r1,[sp,#0x34]
  000c48a0: add r0,pc
  000c48a2: ldr.w r11,[r0,r1,lsl #0x2]
  000c48a6: b 0x000c48e2
  000c48a8: ldr r0,[0x000c4b9c]
  000c48aa: ldr r1,[sp,#0x34]
  000c48ac: add r0,pc
  000c48ae: ldr.w r0,[r0,r1,lsl #0x2]
  000c48b2: add.w r9,r10,r0
  000c48b6: movs r0,#0xc0
  000c48b8: blx 0x0006eb24
  000c48bc: mov r4,r0
  000c48be: uxth.w r1,r9
  000c48c2: ldr.w r2,[r8,#0x0]
  000c48c6: movs r3,#0x0
  000c48c8: blx 0x0007207c
  000c48cc: ldr r1,[r4,#0xc]
  000c48ce: mov r0,r5
  000c48d0: blx 0x000720e8
  000c48d4: mov r0,r4
  000c48d6: blx 0x00071fc8
  000c48da: blx 0x0006eb48
  000c48de: add.w r10,r10,#0x1
  000c48e2: cmp r10,r11
  000c48e4: blt 0x000c48a8
  000c48e6: ldr r0,[sp,#0x30]
  000c48e8: ldr.w r1,[r0,#0xf8]
  000c48ec: ldr r0,[sp,#0x2c]
  000c48ee: blx 0x000740f8
  000c48f2: ldr.w r10,[sp,#0x28]
  000c48f6: ldr.w r9,[sp,#0x18]
  000c48fa: adds r6,#0x1
  000c48fc: cmp r6,#0x4
  000c48fe: bcc 0x000c483a
  000c4900: ldr.w r0,[r10,#0x0]
  000c4904: blx 0x00071c14
  000c4908: blx 0x00071824
  000c490c: cmp r0,#0x6c
  000c490e: bne 0x000c4922
  000c4910: ldr.w r0,[r10,#0x0]
  000c4914: ldr.w r0,[r0,#0x114]
  000c4918: subs r0,#0x3
  000c491a: clz r0,r0
  000c491e: lsrs r4,r0,#0x5
  000c4920: b 0x000c4924
  000c4922: movs r4,#0x0
  000c4924: ldr r0,[0x000c4ba0]
  000c4926: ldr r1,[0x000c4ba4]
  000c4928: add r0,pc
  000c492a: add r1,pc
  000c492c: ldr.w r8,[r0,#0x0]
  000c4930: ldr r0,[sp,#0x34]
  000c4932: ldr.w r6,[r1,r0,lsl #0x2]
  000c4936: ldr.w r0,[r8,#0x0]
  000c493a: adds r1,r6,#0x1
  000c493c: blx 0x00071848
  000c4940: cmp r4,#0x0
  000c4942: str r0,[sp,#0x1c]
  000c4944: str r4,[sp,#0x18]
  000c4946: beq 0x000c4964
  000c4948: ldr.w r0,[r10,#0x0]
  000c494c: ldr.w r0,[r0,#0x14c]
  000c4950: blx 0x00073708
  000c4954: cbz r0,0x000c4960
  000c4956: ldr r0,[r0,#0x0]
  000c4958: cmp r6,r0
  000c495a: it ls
  000c495c: mov.ls r0,r6
  000c495e: b 0x000c4962
  000c4960: movs r0,#0x0
  000c4962: str r0,[sp,#0x1c]
  000c4964: mov r0,r6
  000c4966: blx 0x0006ec08
  000c496a: mov r4,r0
  000c496c: movs r0,#0x0
  000c496e: movs r1,#0x0
  000c4970: b 0x000c4976
  000c4972: strb r0,[r4,r1]
  000c4974: adds r1,#0x1
  000c4976: cmp r1,r6
  000c4978: blt 0x000c4972
  000c497a: ldr r0,[0x000c4ba8]
  000c497c: mov.w r9,#0x0
  000c4980: ldr r2,[0x000c4bac]
  000c4982: ldr r1,[sp,#0x34]
  000c4984: add r0,pc
  000c4986: add r2,pc
  000c4988: vldr.32 s16,[pc,#0x1fc]
  000c498c: cmp r1,#0x7
  000c498e: it ne
  000c4990: mov.ne r2,r0
  000c4992: uxth r0,r1
  000c4994: movw r1,#0x10b
  000c4998: lsr.w r0,r1,r0
  000c499c: str r2,[sp,#0x2c]
  000c499e: and r0,r0,#0x1
  000c49a2: str r0,[sp,#0x10]
  000c49a4: ldr r0,[0x000c4bb0]
  000c49a6: add r0,pc
  000c49a8: ldr r0,[r0,#0x0]
  000c49aa: str r0,[sp,#0x14]
  000c49ac: b 0x000c4b1e
  000c49ae: ldr.w r0,[r8,#0x0]
  000c49b2: movs r1,#0x64
  000c49b4: blx 0x00071848
  000c49b8: ldr r5,[sp,#0x34]
  000c49ba: cmp r0,#0x1d
  000c49bc: bgt 0x000c49da
  000c49be: ldr.w r0,[r8,#0x0]
  000c49c2: movs r1,#0x4
  000c49c4: blx 0x00071848
  000c49c8: mov r5,r0
  000c49ca: ldr.w r0,[r8,#0x0]
  000c49ce: movs r1,#0x64
  000c49d0: blx 0x00071848
  000c49d4: cmp r0,#0x1e
  000c49d6: it lt
  000c49d8: mov.lt r5,#0x8
  000c49da: ldr r0,[sp,#0x14]
  000c49dc: mov r1,r5
  000c49de: ldr r0,[r0,#0x0]
  000c49e0: blx 0x00071a34
  000c49e4: mov r11,r0
  000c49e6: ldr.w r0,[r10,#0x0]
  000c49ea: blx 0x00071c14
  000c49ee: blx 0x00071824
  000c49f2: cmp r0,#0x64
  000c49f4: bne 0x000c4a14
  000c49f6: ldr.w r0,[r8,#0x0]
  000c49fa: movs r1,#0x3
  000c49fc: blx 0x00071848
  000c4a00: mov.w r11,#0x28
  000c4a04: cmp r0,#0x1
  000c4a06: it eq
  000c4a08: mov.eq.w r11,#0x26
  000c4a0c: cmp r0,#0x0
  000c4a0e: it eq
  000c4a10: mov.eq.w r11,#0x25
  000c4a14: ldr r0,[sp,#0x18]
  000c4a16: cbz r0,0x000c4a30
  000c4a18: ldr.w r0,[r10,#0x0]
  000c4a1c: ldr.w r0,[r0,#0x14c]
  000c4a20: blx 0x00073708
  000c4a24: ldr r0,[r0,#0x4]
  000c4a26: ldr.w r0,[r0,r9,lsl #0x2]
  000c4a2a: blx 0x000719c8
  000c4a2e: mov r11,r0
  000c4a30: movs r0,#0x0
  000c4a32: movs r1,#0x0
  000c4a34: strd r0,r0,[sp,#0x0]
  000c4a38: movs r2,#0x0
  000c4a3a: str r0,[sp,#0x8]
  000c4a3c: mov r3,r11
  000c4a3e: ldr r0,[sp,#0x30]
  000c4a40: blx 0x00073e88
  000c4a44: mov r10,r0
  000c4a46: ldr.w r0,[r8,#0x0]
  000c4a4a: mov r1,r6
  000c4a4c: blx 0x00071848
  000c4a50: mvn r5,#0x63
  000c4a54: b 0x000c4a62
  000c4a56: ldr.w r0,[r8,#0x0]
  000c4a5a: mov r1,r6
  000c4a5c: blx 0x00071848
  000c4a60: adds r5,#0x1
  000c4a62: cbz r5,0x000c4a6a
  000c4a64: ldrb r1,[r4,r0]
  000c4a66: cmp r1,#0x0
  000c4a68: bne 0x000c4a56
  000c4a6a: movs r1,#0x1
  000c4a6c: strb r1,[r4,r0]
  000c4a6e: ldr r1,[sp,#0x34]
  000c4a70: cmp r1,#0x8
  000c4a72: ldr r1,[sp,#0x2c]
  000c4a74: bhi 0x000c4a88
  000c4a76: ldr r1,[sp,#0x10]
  000c4a78: cmp r1,#0x0
  000c4a7a: ldr r1,[sp,#0x2c]
  000c4a7c: beq 0x000c4a88
  000c4a7e: ldr r1,[0x000c4bb4]
  000c4a80: ldr r2,[sp,#0x34]
  000c4a82: add r1,pc
  000c4a84: ldr.w r1,[r1,r2,lsl #0x2]
  000c4a88: add.w r0,r0,r0, lsl #0x1
  000c4a8c: ldr r2,[sp,#0x20]
  000c4a8e: add.w r0,r1,r0, lsl #0x2
  000c4a92: ldr.w r2,[r2,r11,lsl #0x2]
  000c4a96: vldr.32 s0,[r0]
  000c4a9a: vldr.32 s2,[r0,#0x8]
  000c4a9e: ldr r0,[r0,#0x4]
  000c4aa0: vcvt.f32.s32 s0,s0
  000c4aa4: vcvt.f32.s32 s2,s2
  000c4aa8: add r0,r2
  000c4aaa: vmov s4,r0
  000c4aae: vcvt.f32.s32 s4,s4
  000c4ab2: ldr.w r0,[r10,#0x0]
  000c4ab6: vmov r1,s0
  000c4aba: vmov r3,s2
  000c4abe: ldr r5,[r0,#0x48]
  000c4ac0: mov r0,r10
  000c4ac2: vmov r2,s4
  000c4ac6: blx r5
  000c4ac8: ldr.w r0,[r10,#0x4]
  000c4acc: movs r1,#0x1
  000c4ace: blx 0x00073fb4
  000c4ad2: mov r0,r10
  000c4ad4: blx 0x000732dc
  000c4ad8: ldr.w r0,[r8,#0x0]
  000c4adc: mov.w r1,#0x12c
  000c4ae0: ldr.w r11,[r10,#0x8]
  000c4ae4: blx 0x00071848
  000c4ae8: vmov s0,r0
  000c4aec: mov r0,r11
  000c4aee: movs r1,#0x0
  000c4af0: movs r3,#0x0
  000c4af2: vcvt.f32.s32 s0,s0
  000c4af6: vdiv.f32 s0,s0,s16
  000c4afa: vmov r2,s0
  000c4afe: blx 0x00073054
  000c4b02: mov r0,r10
  000c4b04: movs r1,#0x0
  000c4b06: blx 0x000721f0
  000c4b0a: ldr r0,[sp,#0x30]
  000c4b0c: ldr.w r1,[r0,#0xf8]
  000c4b10: mov r0,r10
  000c4b12: blx 0x000740f8
  000c4b16: add.w r9,r9,#0x1
  000c4b1a: ldr.w r10,[sp,#0x28]
  000c4b1e: ldr r0,[sp,#0x1c]
  000c4b20: cmp r9,r0
  000c4b22: blt.w 0x000c49ae
  000c4b26: mov r0,r4
  000c4b28: blx 0x0006ebfc
  000c4b2c: ldr r0,[sp,#0x44]
  000c4b2e: ldr r1,[sp,#0x24]
  000c4b30: ldr r1,[r1,#0x0]
  000c4b32: subs r0,r1,r0
  000c4b34: itttt eq
  000c4b36: add.eq sp,#0x48
  000c4b38: vpop.eq {d8,d9,d10,d11}
  000c4b3c: add.eq sp,#0x4
  000c4b3e: pop.eq.w {r8,r9,r10,r11}
  000c4b42: it eq
  000c4b44: pop.eq {r4,r5,r6,r7,pc}
  000c4b46: blx 0x0006e824
  001abf98: bx pc
```
