# PlayerEgo::approachAsteroid
elf 0x9f570 body 1064
Sig: undefined __stdcall approachAsteroid(Hud * param_1, int param_2, Radar * param_3)

## decompile
```c

/* PlayerEgo::approachAsteroid(Hud*, int, Radar*) */

void PlayerEgo::approachAsteroid(Hud *param_1,int param_2,Radar *param_3)

{
  uint uVar1;
  byte bVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  int iVar5;
  float fVar6;
  Ship *pSVar7;
  int iVar8;
  float fVar9;
  undefined4 *puVar10;
  undefined4 uVar11;
  MiningGame *this;
  TargetFollowCamera *this_00;
  int *piVar12;
  Vector *pVVar13;
  int *piVar14;
  uint in_fpscr;
  float fVar15;
  float extraout_s1;
  float fVar16;
  Vector aVStack_c8 [64];
  undefined4 local_88 [5];
  undefined4 local_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined4 local_50;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  int local_34;
  
  piVar14 = *(int **)(DAT_000af944 + 0xaf588);
  local_34 = *piVar14;
  iVar5 = KIPlayer::isDying();
  iVar8 = DAT_000af9e4;
  if (iVar5 == 0) {
    if (*(int *)(param_1 + 0x1c4) == 1) {
      fVar6 = *(float *)(param_1 + 0x1dc);
      uVar1 = in_fpscr & 0xfffffff | (uint)(fVar6 < DAT_000af9c0) << 0x1f |
              (uint)(fVar6 == DAT_000af9c0) << 0x1e;
      bVar2 = (byte)(uVar1 >> 0x18);
      if ((bool)(bVar2 >> 6 & 1) || (bool)(bVar2 >> 7) != (NAN(fVar6) || NAN(DAT_000af9c0))) {
        if (*(MiningGame **)(param_1 + 0x1e4) == (MiningGame *)0x0) {
          param_1[0x2f5] = (Hud)0x0;
          param_1[0x39b] = (Hud)0x0;
          *(undefined1 *)(*(int *)(iVar8 + 0xaf8ec) + 0x37) = 0;
          this = operator_new(0xd4);
          iVar8 = PlayerAsteroid::getQuality(*(PlayerAsteroid **)(param_1 + 0x1bc));
          fVar6 = (float)MiningGame::MiningGame
                                   (this,iVar8,*(int *)(*(int *)(param_1 + 0x1bc) + 0x128),
                                    (Hud *)param_2);
          iVar8 = DAT_000af9e8;
          *(MiningGame **)(param_1 + 0x1e4) = this;
          piVar12 = *(int **)(iVar8 + 0xaf928);
          *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x1bc) + 4) + 0x40) = 0;
          FModSound::play(*piVar12,(Vector *)0x1,(Vector *)0x0,fVar6);
          FModSound::pause((FModSound *)*piVar12);
        }
        else {
          iVar8 = MiningGame::update(*(MiningGame **)(param_1 + 0x1e4),*(int *)(param_1 + 0x134));
          if (iVar8 == 0) {
            iVar8 = MiningGame::gameLost(*(MiningGame **)(param_1 + 0x1e4));
            if ((iVar8 == 0) &&
               (iVar8 = MiningGame::getOreAmount(*(MiningGame **)(param_1 + 0x1e4)), 0 < iVar8)) {
              stopMining();
            }
            else {
              iVar5 = MiningGame::gameLost(*(MiningGame **)(param_1 + 0x1e4));
              iVar8 = DAT_000af9ec;
              if (iVar5 != 0) {
                param_1[0x39b] = (Hud)0x1;
                *(undefined4 *)(**(int **)(iVar8 + 0xaf984) + 0x124) = 0;
                stopMining();
                Hud::hudEvent(param_2,(PlayerEgo *)0x8,(int)param_1);
              }
            }
          }
          else {
            iVar8 = KIPlayer::isDying();
            if ((iVar8 != 0) || (iVar8 = KIPlayer::isDead(), iVar8 != 0)) {
              *(undefined4 *)(**(int **)(DAT_000af9f0 + 0xaf742) + 0x124) = 0;
              stopMining();
              Hud::hudEvent(param_2,(PlayerEgo *)0x8,(int)param_1);
            }
          }
        }
      }
      else {
        fVar9 = (float)VectorSignedToFloat(-*(int *)(param_1 + 0x134) >> 1,(byte)(uVar1 >> 0x16) & 3
                                          );
        *(float *)(param_1 + 0x1dc) = fVar6 + fVar9;
      }
    }
    else if (*(int *)(param_1 + 0x1c4) == 0) {
      if (param_1[0x1a0] != (Hud)0x0) {
        setTurretMode(SUB41(param_1,0));
      }
      (**(code **)(**(int **)(param_1 + 0x1bc) + 0x28))((Vector *)local_88);
      pVVar13 = (Vector *)(param_1 + 0xec);
      AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)local_88);
      AEGeometry::getPosition();
      AbyssEngine::AEMath::Vector::operator-=(pVVar13,(Vector *)local_88);
      fVar6 = (float)AbyssEngine::AEMath::VectorLength(pVVar13);
      if ((int)fVar6 < *(int *)(param_1 + 0x1d8)) {
        if (param_1[0x1d4] != (Hud)0x0) {
          if ((*(PlayerAsteroid **)(param_1 + 0x1bc))[0x3c] != (PlayerAsteroid)0x0) {
            PlayerAsteroid::setRotationEnabled(*(PlayerAsteroid **)(param_1 + 0x1bc),false);
          }
          *(undefined4 *)(param_1 + 0x1c4) = 1;
        }
      }
      else {
        AEGeometry::getPosition();
        pSVar7 = (Ship *)Status::getShip();
        fVar16 = (float)Ship::getHandling(pSVar7);
        fVar9 = DAT_000af9c4;
        fVar15 = 4.0;
        in_fpscr = in_fpscr & 0xfffffff;
        if (fVar16 + DAT_000af9c4 < 4.0) {
          pSVar7 = (Ship *)Status::getShip();
          fVar15 = (float)Ship::getHandling(pSVar7);
          fVar15 = fVar15 + fVar9;
        }
        moveToPosition(param_1,local_40,uStack_3c,uStack_38,1,fVar15);
        (**(code **)(**(int **)(param_1 + 0x1bc) + 0x28))((Vector *)local_88);
        getPosition();
        AbyssEngine::AEMath::operator-(aVStack_c8,(Vector *)local_88);
        fVar9 = (float)AbyssEngine::AEMath::VectorLength(aVStack_c8);
        if ((int)fVar9 < 20000) {
          param_1[0x330] = (Hud)0x1;
        }
      }
      if ((int)fVar6 < *(int *)(param_1 + 0x1d8) + 2000) {
        *(undefined4 *)(param_1 + 0xbc) = 0x3f800000;
        if (param_1[0x2f5] == (Hud)0x0) {
          fVar6 = (float)setExhaustVisible((PlayerEgo *)param_1,false);
          FModSound::play(**(int **)(DAT_000af9dc + 0xaf794),(Vector *)0x2,(Vector *)0x0,fVar6);
          iVar5 = *(int *)(param_1 + 0xcc);
          this_00 = *(TargetFollowCamera **)(param_1 + 0x88);
          iVar8 = __aeabi_idiv(iVar5,6);
          *(int *)(param_1 + 0x138) = iVar5 - iVar8;
          TargetFollowCamera::setActive(this_00,false);
          AEGeometry::getUpVector();
          pVVar13 = (Vector *)(param_1 + 0x1c8);
          AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)local_88);
          AbyssEngine::AEMath::VectorNormalize((AEMath *)local_88,pVVar13);
          AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)local_88);
          param_1[0x2f5] = (Hud)0x1;
        }
        AEGeometry::getDirection();
        pVVar13 = (Vector *)(param_1 + 0xe0);
        AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)local_88);
        AbyssEngine::AEMath::VectorNormalize((AEMath *)local_88,pVVar13);
        AbyssEngine::AEMath::Vector::operator=(pVVar13,(Vector *)local_88);
        fVar6 = (float)AbyssEngine::AEMath::VectorDot(pVVar13,(Vector *)(param_1 + 0x1c8));
        fVar9 = (float)AbyssEngine::AEMath::ACosf(fVar6);
        fVar6 = DAT_000af9d4;
        uVar4 = DAT_000af9b8;
        uVar3 = DAT_000af9b0;
        if (DAT_000af9c8 < fVar9) {
          fVar9 = *(float *)(param_1 + 0x1dc);
          uVar1 = in_fpscr & 0xfffffff | (uint)(fVar9 < DAT_000af9c0) << 0x1f |
                  (uint)(fVar9 == DAT_000af9c0) << 0x1e;
          bVar2 = (byte)(uVar1 >> 0x18);
          if (!(bool)(bVar2 >> 6 & 1) && (bool)(bVar2 >> 7) == (NAN(fVar9) || NAN(DAT_000af9c0))) {
            uStack_6c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
            uStack_68 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
            uStack_64 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
            fVar16 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x134),
                                                (byte)(uVar1 >> 0x16) & 3);
            puVar10 = (undefined4 *)((uint)local_88 | 4);
            fVar9 = fVar9 + fVar16 * DAT_000af9cc;
            fVar16 = fVar9 * DAT_000af9d0;
            *(float *)(param_1 + 0x1dc) = fVar9;
            *puVar10 = 0;
            puVar10[1] = uStack_6c;
            puVar10[2] = uStack_68;
            puVar10[3] = uStack_64;
            local_70 = 0;
            uStack_60 = uVar3;
            uStack_58 = uVar4;
            local_88[0] = 0x3f800000;
            local_74 = 0x3f800000;
            local_50 = 0x3f800000;
            AbyssEngine::AEMath::MatrixSetRotation
                      ((Matrix *)aVStack_c8,fVar9,extraout_s1,fVar16 * fVar6);
            uVar11 = AbyssEngine::PaintCanvas::TransformGetLocal
                               (**(uint **)(DAT_000af9e0 + 0xaf8aa));
            __aeabi_memcpy((Matrix *)aVStack_c8,uVar11,0x3c);
            AbyssEngine::AEMath::Matrix::operator*=((Matrix *)aVStack_c8,(Matrix *)local_88);
            AEGeometry::setMatrix(*(Matrix **)(param_1 + 4));
            goto LAB_000af75e;
          }
        }
        param_1[0x1d4] = (Hud)0x1;
        *(undefined2 *)(param_1 + 0x2f4) = 0;
      }
    }
  }
LAB_000af75e:
  iVar8 = *piVar14 - local_34;
  if (iVar8 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar8);
}

```

## target disasm
```
  000af570: push {r4,r5,r6,r7,lr}
  000af572: add r7,sp,#0xc
  000af574: push {r7,r8,r9,r10,r11}
  000af578: vpush {d8}
  000af57c: sub sp,#0xa0
  000af57e: mov r4,r0
  000af580: ldr r0,[0x000af944]
  000af582: mov r5,r1
  000af584: add r0,pc
  000af586: ldr.w r9,[r0,#0x0]
  000af58a: ldr.w r0,[r9,#0x0]
  000af58e: str r0,[sp,#0x9c]
  000af590: ldr.w r0,[r4,#0x1bc]
  000af594: blx 0x00071f50
  000af598: cmp r0,#0x0
  000af59a: bne.w 0x000af75c
  000af59e: ldr.w r0,[r4,#0x1c4]
  000af5a2: cmp r0,#0x1
  000af5a4: beq 0x000af620
  000af5a6: cmp r0,#0x0
  000af5a8: bne.w 0x000af75c
  000af5ac: ldrb.w r0,[r4,#0x1a0]
  000af5b0: cbz r0,0x000af5ba
  000af5b2: mov r0,r4
  000af5b4: movs r1,#0x0
  000af5b6: blx 0x00072b08
  000af5ba: ldr.w r1,[r4,#0x1bc]
  000af5be: add r5,sp,#0x48
  000af5c0: ldr r0,[r1,#0x0]
  000af5c2: ldr r2,[r0,#0x28]
  000af5c4: mov r0,r5
  000af5c6: blx r2
  000af5c8: add.w r6,r4,#0xec
  000af5cc: mov r1,r5
  000af5ce: mov r0,r6
  000af5d0: blx 0x0006eb3c
  000af5d4: add r5,sp,#0x48
  000af5d6: ldr r1,[r4,#0x8]
  000af5d8: mov r0,r5
  000af5da: blx 0x000720b8
  000af5de: mov r0,r6
  000af5e0: mov r1,r5
  000af5e2: blx 0x00072694
  000af5e6: mov r0,r6
  000af5e8: blx 0x0006ec44
  000af5ec: vmov s0,r0
  000af5f0: ldr.w r0,[r4,#0x1d8]
  000af5f4: vcvt.s32.f32 s0,s0
  000af5f8: vmov r10,s0
  000af5fc: cmp r0,r10
  000af5fe: ble 0x000af64c
  000af600: ldrb.w r0,[r4,#0x1d4]
  000af604: cmp r0,#0x0
  000af606: beq 0x000af6ee
  000af608: ldr.w r0,[r4,#0x1bc]
  000af60c: ldrb.w r1,[r0,#0x3c]
  000af610: cbz r1,0x000af618
  000af612: movs r1,#0x0
  000af614: blx 0x00072ac0
  000af618: movs r0,#0x1
  000af61a: str.w r0,[r4,#0x1c4]
  000af61e: b 0x000af6ee
  000af620: vldr.32 s2,[pc,#0x39c]
  000af624: vldr.32 s0,[r4,#0x1dc]
  000af628: vcmpe.f32 s0,s2
  000af62c: vmrs apsr,fpscr
  000af630: ble 0x000af70e
  000af632: ldr.w r0,[r4,#0x134]
  000af636: rsbs r0,r0
  000af638: asrs r0,r0,#0x1
  000af63a: vmov s2,r0
  000af63e: vcvt.f32.s32 s2,s2
  000af642: vadd.f32 s0,s0,s2
  000af646: vstr.32 s0,[r4,#0x1dc]
  000af64a: b 0x000af75c
  000af64c: ldr.w r0,[r4,#0x1bc]
  000af650: ldr r1,[r0,#0x8]
  000af652: add r0,sp,#0x90
  000af654: blx 0x000720b8
  000af658: ldr r0,[0x000af9d8]
  000af65a: add r0,pc
  000af65c: ldr r5,[r0,#0x0]
  000af65e: ldr r0,[r5,#0x0]
  000af660: blx 0x00071a58
  000af664: blx 0x000722a4
  000af668: vldr.32 s16,[pc,#0x358]
  000af66c: vmov s2,r0
  000af670: vmov.f32 s0,0x40800000
  000af674: vadd.f32 s2,s2,s16
  000af678: vcmpe.f32 s2,s0
  000af67c: vmrs apsr,fpscr
  000af680: bpl 0x000af694
  000af682: ldr r0,[r5,#0x0]
  000af684: blx 0x00071a58
  000af688: blx 0x000722a4
  000af68c: vmov s0,r0
  000af690: vadd.f32 s0,s0,s16
  000af694: add r3,sp,#0x90
  000af696: mov.w r11,#0x1
  000af69a: mov r0,r4
  000af69c: ldmia r3,{r1,r2,r3}
  000af69e: vstr.32 s0,[sp,#0x4]
  000af6a2: str.w r11,[sp,#0x0]
  000af6a6: blx 0x00072664
  000af6aa: ldr.w r1,[r4,#0x1bc]
  000af6ae: add.w r8,sp,#0x48
  000af6b2: ldr r0,[r1,#0x0]
  000af6b4: ldr r2,[r0,#0x28]
  000af6b6: mov r0,r8
  000af6b8: blx r2
  000af6ba: add r6,sp,#0x84
  000af6bc: mov r1,r4
  000af6be: mov r0,r6
  000af6c0: blx 0x0007264c
  000af6c4: add r5,sp,#0x8
  000af6c6: mov r1,r8
  000af6c8: mov r2,r6
  000af6ca: mov r0,r5
  000af6cc: blx 0x0006ec38
  000af6d0: mov r0,r5
  000af6d2: blx 0x0006ec44
  000af6d6: vmov s0,r0
  000af6da: movw r1,#0x4e1f
  000af6de: vcvt.s32.f32 s0,s0
  000af6e2: vmov r0,s0
  000af6e6: cmp r0,r1
  000af6e8: it le
  000af6ea: strb.le.w r11,[r4,#0x330]
  000af6ee: ldr.w r0,[r4,#0x1d8]
  000af6f2: add.w r0,r0,#0x7d0
  000af6f6: cmp r0,r10
  000af6f8: ble 0x000af75c
  000af6fa: ldrb.w r0,[r4,#0x2f5]
  000af6fe: mov.w r1,#0x3f800000
  000af702: str.w r1,[r4,#0xbc]
  000af706: cbz r0,0x000af77e
  000af708: add.w r8,r4,#0x1c8
  000af70c: b 0x000af7e8
  000af70e: ldr.w r0,[r4,#0x1e4]
  000af712: cmp r0,#0x0
  000af714: beq.w 0x000af8e0
  000af718: ldr.w r1,[r4,#0x134]
  000af71c: blx 0x00072b14
  000af720: cmp r0,#0x0
  000af722: beq.w 0x000af948
  000af726: ldr.w r0,[r4,#0x1bc]
  000af72a: blx 0x00071f50
  000af72e: cbnz r0,0x000af73a
  000af730: ldr.w r0,[r4,#0x1bc]
  000af734: blx 0x00071ec0
  000af738: cbz r0,0x000af75c
  000af73a: ldr r0,[0x000af9f0]
  000af73c: movs r1,#0x0
  000af73e: add r0,pc
  000af740: ldr r0,[r0,#0x0]
  000af742: ldr r0,[r0,#0x0]
  000af744: str.w r1,[r0,#0x124]
  000af748: mov r0,r4
  000af74a: blx 0x00072b20
  000af74e: mov r0,r5
  000af750: movs r1,#0x8
  000af752: mov r2,r4
  000af754: movs r3,#0x0
  000af756: blx 0x00072418
  000af75a: b 0x000af964
  000af75c: movs r6,#0x0
  000af75e: ldr r0,[sp,#0x9c]
  000af760: ldr.w r1,[r9,#0x0]
  000af764: subs r0,r1,r0
  000af766: itttt eq
  000af768: mov.eq r0,r6
  000af76a: add.eq sp,#0xa0
  000af76c: vpop.eq {d8}
  000af770: add.eq sp,#0x4
  000af772: itt eq
  000af774: pop.eq.w {r8,r9,r10,r11}
  000af778: pop.eq {r4,r5,r6,r7,pc}
  000af77a: blx 0x0006e824
  000af77e: mov r0,r4
  000af780: movs r1,#0x0
  000af782: movs r5,#0x0
  000af784: blx 0x00072af0
  000af788: ldr r0,[0x000af9dc]
  000af78a: movs r1,#0x2
  000af78c: movs r2,#0x0
  000af78e: movs r3,#0x0
  000af790: add r0,pc
  000af792: str r5,[sp,#0x0]
  000af794: ldr r0,[r0,#0x0]
  000af796: ldr r0,[r0,#0x0]
  000af798: blx 0x00071548
  000af79c: ldr.w r6,[r4,#0xcc]
  000af7a0: movs r1,#0x6
  000af7a2: ldr.w r5,[r4,#0x88]
  000af7a6: mov r0,r6
  000af7a8: blx 0x0007198c
  000af7ac: subs r0,r6,r0
  000af7ae: str.w r0,[r4,#0x138]
  000af7b2: mov r0,r5
  000af7b4: movs r1,#0x0
  000af7b6: blx 0x00072acc
  000af7ba: add r5,sp,#0x48
  000af7bc: ldr r1,[r4,#0x4]
  000af7be: mov r0,r5
  000af7c0: blx 0x00072568
  000af7c4: add.w r8,r4,#0x1c8
  000af7c8: mov r1,r5
  000af7ca: mov r0,r8
  000af7cc: blx 0x0006eb3c
  000af7d0: add r5,sp,#0x48
  000af7d2: mov r1,r8
  000af7d4: mov r0,r5
  000af7d6: blx 0x0006ec80
  000af7da: mov r0,r8
  000af7dc: mov r1,r5
  000af7de: blx 0x0006eb3c
  000af7e2: movs r0,#0x1
  000af7e4: strb.w r0,[r4,#0x2f5]
  000af7e8: add r5,sp,#0x48
  000af7ea: ldr r1,[r4,#0x4]
  000af7ec: mov r0,r5
  000af7ee: blx 0x00072574
  000af7f2: add.w r6,r4,#0xe0
  000af7f6: mov r1,r5
  000af7f8: mov r0,r6
  000af7fa: blx 0x0006eb3c
  000af7fe: add r5,sp,#0x48
  000af800: mov r1,r6
  000af802: mov r0,r5
  000af804: blx 0x0006ec80
  000af808: mov r0,r6
  000af80a: mov r1,r5
  000af80c: blx 0x0006eb3c
  000af810: mov r0,r6
  000af812: mov r1,r8
  000af814: blx 0x0006ec68
  000af818: blx 0x000726a0
  000af81c: vldr.32 s0,[pc,#0x1a8]
  000af820: vmov s2,r0
  000af824: vcmpe.f32 s2,s0
  000af828: vmrs apsr,fpscr
  000af82c: ble 0x000af8d2
  000af82e: vldr.32 s2,[pc,#0x190]
  000af832: vldr.32 s0,[r4,#0x1dc]
  000af836: vcmpe.f32 s0,s2
  000af83a: vmrs apsr,fpscr
  000af83e: ble 0x000af8d2
  000af840: vldr.32 s2,[r4,#0x134]
  000af844: adr r0,[0xaf9b0]
  000af846: vldr.32 s4,[pc,#0x184]
  000af84a: vmov.i32 q9,#0x0
  000af84e: vcvt.f32.s32 s2,s2
  000af852: vld1.64 {d16,d17},[r0]
  000af856: orr r0,r5,#0x4
  000af85a: movs r6,#0x0
  000af85c: mov r1,r5
  000af85e: movs r3,#0x0
  000af860: vmla.f32 s0,s2,s4
  000af864: vldr.32 s2,[pc,#0x168]
  000af868: vldr.32 s4,[pc,#0x168]
  000af86c: vmul.f32 s2,s0,s2
  000af870: vstr.32 s0,[r4,#0x1dc]
  000af874: vst1.32 {d18,d19},[r0]
  000af878: add.w r0,r5,#0x18
  000af87c: vst1.64 {d18,d19},[r0]
  000af880: add.w r0,r5,#0x28
  000af884: vst1.64 {d16,d17},[r0]
  000af888: mov.w r0,#0x3f800000
  000af88c: vmul.f32 s2,s2,s4
  000af890: str r0,[sp,#0x48]
  000af892: str r0,[sp,#0x5c]
  000af894: str r0,[sp,#0x80]
  000af896: add r0,sp,#0x8
  000af898: str r6,[sp,#0x0]
  000af89a: vmov r2,s2
  000af89e: blx 0x00072094
  000af8a2: ldr r0,[0x000af9e0]
  000af8a4: ldr r1,[r4,#0x4]
  000af8a6: add r0,pc
  000af8a8: ldr r0,[r0,#0x0]
  000af8aa: ldr r1,[r1,#0xc]
  000af8ac: ldr r0,[r0,#0x0]
  000af8ae: blx 0x000720c4
  000af8b2: add.w r8,sp,#0x8
  000af8b6: mov r1,r0
  000af8b8: movs r2,#0x3c
  000af8ba: mov r0,r8
  000af8bc: blx 0x0006f1e4
  000af8c0: mov r0,r8
  000af8c2: mov r1,r5
  000af8c4: blx 0x0006f4a8
  000af8c8: ldr r0,[r4,#0x4]
  000af8ca: mov r1,r8
  000af8cc: blx 0x000721e4
  000af8d0: b 0x000af75e
  000af8d2: movs r0,#0x1
  000af8d4: movs r6,#0x0
  000af8d6: strb.w r0,[r4,#0x1d4]
  000af8da: strh.w r6,[r4,#0x2f4]
  000af8de: b 0x000af75e
  000af8e0: ldr r0,[0x000af9e4]
  000af8e2: movs r1,#0x0
  000af8e4: strb.w r1,[r4,#0x2f5]
  000af8e8: add r0,pc
  000af8ea: strb.w r1,[r4,#0x39b]
  000af8ee: ldr r0,[r0,#0x0]
  000af8f0: strb.w r1,[r0,#0x37]
  000af8f4: movs r0,#0xd4
  000af8f6: blx 0x0006eb24
  000af8fa: mov r6,r0
  000af8fc: ldr.w r0,[r4,#0x1bc]
  000af900: blx 0x00072b2c
  000af904: mov r1,r0
  000af906: ldr.w r0,[r4,#0x1bc]
  000af90a: ldr.w r2,[r0,#0x128]
  000af90e: mov r0,r6
  000af910: mov r3,r5
  000af912: blx 0x00072b38
  000af916: ldr r0,[0x000af9e8]
  000af918: movs r2,#0x0
  000af91a: ldr.w r1,[r4,#0x1bc]
  000af91e: movs r3,#0x0
  000af920: str.w r6,[r4,#0x1e4]
  000af924: add r0,pc
  000af926: movs r6,#0x0
  000af928: ldr r1,[r1,#0x4]
  000af92a: ldr r5,[r0,#0x0]
  000af92c: str r6,[r1,#0x40]
  000af92e: movs r1,#0x1
  000af930: ldr r0,[r5,#0x0]
  000af932: str r6,[sp,#0x0]
  000af934: blx 0x00071548
  000af938: ldr r1,[r4,#0x1c]
  000af93a: ldr r0,[r5,#0x0]
  000af93c: blx 0x00072b44
  000af940: b 0x000af75e
  000af948: ldr.w r0,[r4,#0x1e4]
  000af94c: blx 0x00072b50
  000af950: cbnz r0,0x000af968
  000af952: ldr.w r0,[r4,#0x1e4]
  000af956: blx 0x000724cc
  000af95a: cmp r0,#0x1
  000af95c: blt 0x000af968
  000af95e: mov r0,r4
  000af960: blx 0x00072b20
  000af964: movs r6,#0x1
  000af966: b 0x000af75e
  000af968: ldr.w r0,[r4,#0x1e4]
  000af96c: blx 0x00072b50
  000af970: movs r6,#0x1
  000af972: cmp r0,#0x0
  000af974: beq.w 0x000af75e
  000af978: ldr r0,[0x000af9ec]
  000af97a: movs r1,#0x0
  000af97c: strb.w r6,[r4,#0x39b]
  000af980: add r0,pc
  000af982: ldr r0,[r0,#0x0]
  000af984: ldr r0,[r0,#0x0]
  000af986: str.w r1,[r0,#0x124]
  000af98a: mov r0,r4
  000af98c: blx 0x00072b20
  000af990: mov r0,r5
  000af992: movs r1,#0x8
  000af994: mov r2,r4
  000af996: movs r3,#0x0
  000af998: blx 0x00072418
  000af99c: b 0x000af75e
```
