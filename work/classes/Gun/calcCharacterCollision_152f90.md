# Gun::calcCharacterCollision
elf 0x152f90 body 1934
Sig: undefined __thiscall calcCharacterCollision(Gun * this)

## decompile
```c

/* Gun::calcCharacterCollision() */

void __thiscall Gun::calcCharacterCollision(Gun *this)

{
  uint uVar1;
  uint uVar2;
  Gun GVar3;
  byte bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  int *piVar9;
  uint *puVar10;
  int iVar11;
  undefined8 *puVar12;
  Gun *pGVar13;
  int *piVar14;
  int iVar15;
  undefined4 uVar16;
  PlayerEgo *pPVar17;
  Hud *pHVar18;
  int iVar19;
  uint uVar20;
  uint uVar21;
  undefined4 *puVar22;
  Vector *this_00;
  Player *this_01;
  uint uVar23;
  uint in_fpscr;
  undefined1 uVar24;
  float extraout_s0;
  float fVar25;
  float extraout_s0_00;
  float extraout_s0_01;
  float fVar26;
  float extraout_s1;
  float extraout_s1_00;
  float fVar27;
  float extraout_s2;
  float extraout_s2_00;
  float fVar28;
  float fVar29;
  Matrix aMStack_10c [60];
  Vector aVStack_d0 [60];
  float local_94;
  float local_90;
  float local_8c;
  undefined8 local_88;
  float local_80;
  undefined8 local_78;
  float local_70;
  int local_6c;
  
  fVar29 = DAT_001632fc;
  fVar26 = DAT_001632f8;
  piVar9 = *(int **)(DAT_001632f4 + 0x162fa6);
  local_6c = *piVar9;
  puVar10 = *(uint **)(this + 0xb4);
  if (puVar10 != (uint *)0x0) {
    iVar19 = *(int *)(this + 0x5c);
    if ((iVar19 - 6U < 2) || (iVar19 == 0x2a || iVar19 == 0x22)) {
      bVar6 = true;
      bVar8 = true;
    }
    else {
      bVar6 = true;
      bVar8 = false;
      if ((1 < iVar19 - 4U) && (bVar8 = false, iVar19 != 0x28)) {
        bVar8 = false;
        bVar6 = false;
      }
    }
    bVar7 = iVar19 == 0xb;
    this_00 = (Vector *)(this + 0xd8);
    *(undefined4 *)this = 0;
    for (uVar20 = 0; uVar20 < *puVar10; uVar20 = uVar20 + 1) {
      this_01 = *(Player **)(puVar10[1] + uVar20 * 4);
      if ((((!bVar7) ||
           ((this_01[0x5c] != (Player)0x0 &&
            ((iVar11 = Player::getKIPlayer(this_01), iVar11 == 0 ||
             (iVar11 = Player::getKIPlayer(this_01), *(char *)(iVar11 + 0x41) == '\0')))))) &&
          (iVar11 = Player::isActive(), iVar11 != 0)) &&
         (iVar11 = Player::getHitpoints(), 0 < iVar11)) {
        for (uVar23 = 0; uVar23 < *(uint *)(this + 8); uVar23 = uVar23 + 1) {
          puVar12 = (undefined8 *)(*(int *)(this + 0xc) + uVar23 * 0xc);
          local_78 = *puVar12;
          local_70 = *(float *)(puVar12 + 1);
          puVar12 = (undefined8 *)(*(int *)(this + 0x18) + uVar23 * 0xc);
          local_88 = *puVar12;
          local_80 = *(float *)(puVar12 + 1);
          AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_94);
          pGVar13 = (Gun *)(this_01 + 0x40);
          if (this[0x68] != (Gun)0x0) {
            pGVar13 = this + 0x104;
          }
          iVar11 = *(int *)pGVar13;
          if (bVar7) {
            iVar11 = iVar11 * 5;
          }
          else if (iVar19 == 0x19) {
            Level::getPlayer();
            PlayerEgo::getPosition();
            AbyssEngine::AEMath::operator-(aVStack_d0,(Vector *)&local_94);
            fVar25 = (float)AbyssEngine::AEMath::VectorLength(aVStack_d0);
            iVar15 = Player::getKIPlayer(this_01);
            if ((iVar15 == 0) ||
               (iVar15 = Player::getKIPlayer(this_01), *(char *)(iVar15 + 0x71) == '\0')) {
              if ((int)fVar25 < 0x4e21) {
                fVar28 = (float)VectorSignedToFloat(iVar11,(byte)(in_fpscr >> 0x16) & 3);
                fVar27 = (float)VectorSignedToFloat(iVar11 << 1,(byte)(in_fpscr >> 0x16) & 3);
                fVar28 = fVar28 * 1.5;
                if (10000 < (int)fVar25) {
                  fVar28 = fVar27;
                }
              }
              else {
                fVar28 = (float)VectorSignedToFloat(iVar11 * 3,(byte)(in_fpscr >> 0x16) & 3);
              }
            }
            else {
              fVar25 = (float)VectorSignedToFloat(iVar11,(byte)(in_fpscr >> 0x16) & 3);
              fVar28 = fVar25 * 0.5;
            }
            iVar11 = (int)fVar28;
          }
          piVar14 = (int *)Player::getKIPlayer(this_01);
          if (this_01[0x69] == (Player)0x0) {
            if ((piVar14 != (int *)0x0) && ((char)piVar14[0x23] != '\0')) goto LAB_0016317c;
          }
          else if (piVar14 == (int *)0x0) {
LAB_001631b2:
            bVar5 = false;
LAB_001631b8:
            fVar25 = (float)VectorSignedToFloat(iVar11,(byte)(in_fpscr >> 0x16) & 3);
            fVar28 = (local_94 - (float)local_78) + (float)local_88;
            uVar21 = in_fpscr & 0xfffffff;
            in_fpscr = uVar21;
            if (fVar28 < fVar25) {
              fVar27 = (float)VectorSignedToFloat(-iVar11,(byte)(uVar21 >> 0x16) & 3);
              uVar1 = uVar21 | (uint)(fVar28 < fVar27) << 0x1f | (uint)(fVar28 == fVar27) << 0x1e;
              in_fpscr = uVar1 | (uint)(NAN(fVar28) || NAN(fVar27)) << 0x1c;
              bVar4 = (byte)(uVar1 >> 0x18);
              if (!(bool)(bVar4 >> 6 & 1) && bVar4 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
                fVar28 = (local_90 - local_78._4_4_) + local_88._4_4_;
                in_fpscr = uVar21;
                if ((fVar28 < fVar25) &&
                   (uVar1 = uVar21 | (uint)(fVar28 < fVar27) << 0x1f |
                            (uint)(fVar28 == fVar27) << 0x1e,
                   in_fpscr = uVar1 | (uint)(NAN(fVar28) || NAN(fVar27)) << 0x1c,
                   bVar4 = (byte)(uVar1 >> 0x18),
                   !(bool)(bVar4 >> 6 & 1) && bVar4 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))) {
                  fVar28 = (local_8c - local_70) + local_80;
                  uVar21 = uVar21 | (uint)(fVar28 < fVar27) << 0x1f |
                           (uint)(fVar28 == fVar27) << 0x1e;
                  in_fpscr = uVar21 | (uint)(NAN(fVar28) || NAN(fVar27)) << 0x1c;
                  bVar4 = (byte)(uVar21 >> 0x18);
                  if (!bVar5 && (fVar28 < fVar25 &&
                                (!(bool)(bVar4 >> 6 & 1) &&
                                bVar4 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)))) goto LAB_0016318c;
                }
              }
            }
          }
          else {
LAB_0016317c:
            if ((char)piVar14[0x10] == '\0') goto LAB_001631b2;
            iVar15 = (**(code **)(*piVar14 + 0x40))(piVar14,&local_78);
            if (iVar15 == 0) {
              bVar5 = true;
              goto LAB_001631b8;
            }
LAB_0016318c:
            iVar11 = Player::isAsteroid(this_01);
            if (iVar11 == 0) {
              if (bVar7) {
                iVar19 = *(int *)(this_01 + 0x40);
                fVar26 = (float)VectorSignedToFloat(iVar19,(byte)(in_fpscr >> 0x16) & 3);
                fVar29 = (local_94 - (float)local_78) + (float)local_88;
                if ((fVar29 < fVar26) &&
                   (fVar25 = (float)VectorSignedToFloat(-iVar19,(byte)((in_fpscr & 0xfffffff) >>
                                                                      0x16) & 3), fVar25 < fVar29))
                {
                  fVar29 = (local_90 - local_78._4_4_) + local_88._4_4_;
                  if (((int)((uint)(fVar29 < fVar26) << 0x1f) < 0) &&
                     (((fVar25 < fVar29 &&
                       (fVar29 = (local_8c - local_70) + local_80,
                       (int)((uint)(fVar29 < fVar26) << 0x1f) < 0)) && (fVar25 < fVar29))))
                  goto LAB_001635aa;
                }
                AbyssEngine::AEMath::operator-((Vector *)aMStack_10c,(Vector *)&local_94);
                AbyssEngine::AEMath::VectorNormalize((AEMath *)aVStack_d0,(Vector *)aMStack_10c);
                fVar26 = (float)AbyssEngine::AEMath::Vector::operator=
                                          ((Vector *)(*(int *)(this + 0x18) + uVar23 * 0xc),
                                           aVStack_d0);
                fVar26 = (float)AbyssEngine::AEMath::operator*((Vector *)aMStack_10c,fVar26);
                AbyssEngine::AEMath::operator*(aVStack_d0,fVar26);
                AbyssEngine::AEMath::Vector::operator+=
                          ((Vector *)(*(int *)(this + 0xc) + uVar23 * 0xc),aVStack_d0);
                goto LAB_00163640;
              }
LAB_0016326e:
              if (bVar8 || bVar7) {
LAB_001635aa:
                ignite(this);
                goto LAB_00163640;
              }
              if (*(int *)(this + 100) != DAT_00163300) {
                Player::damageEmp(this_01,*(int *)(this + 100),(bool)this[0xf9]);
              }
              if ((*(int *)(this + 4) == 0) || (*(char *)(*(int *)(this + 4) + 0x5c) != '\0')) {
                if ((this_01[0x69] != (Player)0x0) && (iVar11 = Level::getPlayer(), iVar11 != 0)) {
                  Level::getPlayer();
                  iVar11 = PlayerEgo::isDockedToDockingPoint();
                  if (iVar11 != 0) {
                    fVar25 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x60),
                                                        (byte)(in_fpscr >> 0x16) & 3);
                    fVar25 = fVar25 * 3.0 * 0.25;
                    goto LAB_001632e0;
                  }
                }
LAB_00163304:
                uVar24 = (undefined1)*(undefined4 *)(this + 0x60);
                GVar3 = this[0xf9];
              }
              else {
                if (this_01[0x69] == (Player)0x0) goto LAB_00163304;
                fVar25 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x60),
                                                    (byte)(in_fpscr >> 0x16) & 3);
                fVar25 = fVar25 * fVar29;
LAB_001632e0:
                GVar3 = this[0xf9];
                uVar24 = (undefined1)(int)fVar25;
              }
              Player::damage((int)this_01,(bool)uVar24,(uint)(byte)GVar3);
              this_01[0x54] = (Player)0x1;
              if (this[0xf0] != (Gun)0x0) {
                *(undefined1 *)(*(int *)(this + 0x38) + 0x30) = 1;
              }
              Player::setHitVector(this_01,extraout_s0_00,extraout_s1,extraout_s2);
              *(undefined4 *)(*(int *)(this + 0x3c) + uVar23 * 4) = DAT_001636dc;
              uVar16 = Player::getKIPlayer(this_01);
              *(undefined4 *)this = uVar16;
              if (bVar6) {
                ParticleSystemManager::emitManual
                          (*(int *)(*(int *)(this + 0x38) + 0x74),
                           *(Vector **)(*(int *)(this + 0x38) + 0x3c),(int)&local_78,(Vector *)0x0,
                           extraout_s0_01);
              }
              else if (*(int *)(this + 0x10c) != 0) {
                puVar10 = *(uint **)(DAT_001636e4 + 0x163380);
                uVar16 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
                AbyssEngine::Transform::SetAnimationState(uVar16,3,0);
                uVar16 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
                AbyssEngine::Transform::SetAnimationState(uVar16,1,0);
                uVar21 = *puVar10;
                AbyssEngine::PaintCanvas::CameraGetCurrent();
                uVar16 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar21);
                __aeabi_memcpy(aVStack_d0,uVar16,0x3c);
                AbyssEngine::AEMath::MatrixSetTranslation
                          (aMStack_10c,local_70,extraout_s1_00,extraout_s2_00);
                AbyssEngine::PaintCanvas::TransformSetLocal
                          (*puVar10,*(Matrix **)(*(int *)(this + 0x10c) + uVar23 * 4));
              }
              if (*(Sparks **)(this + 0xb8) != (Sparks *)0x0) {
                iVar11 = Sparks::isRocket(*(Sparks **)(this + 0xb8));
                if (iVar11 != 0) {
                  this_01[0x55] = (Player)0x1;
                }
                if (*(Sparks **)(this + 0xb8) != (Sparks *)0x0) {
                  Sparks::explode(*(Sparks **)(this + 0xb8),(Vector *)&local_78);
                }
              }
              if (iVar19 == 0x19) {
                *(undefined1 *)(*(int *)(this + 0x40) + uVar23) = 1;
                AbyssEngine::AEMath::Vector::operator=
                          ((Vector *)(*(int *)(this + 0x30) + uVar23 * 0xc),(Vector *)&local_78);
                for (uVar21 = 0; uVar21 < **(uint **)(this + 0xb4); uVar21 = uVar21 + 1) {
                  this_01 = *(Player **)((*(uint **)(this + 0xb4))[1] + uVar21 * 4);
                  Player::getPosition();
                  AbyssEngine::AEMath::Vector::operator=(this_00,aVStack_d0);
                  AbyssEngine::AEMath::Vector::operator-=(this_00,(Vector *)&local_78);
                  fVar25 = (float)AbyssEngine::AEMath::VectorLength(this_00);
                  iVar11 = *(int *)(this + 0x100);
                  if ((int)fVar25 < iVar11) {
                    fVar28 = (float)VectorSignedToFloat(iVar11,(byte)(in_fpscr >> 0x16) & 3);
                    fVar27 = (float)VectorSignedToFloat(iVar11 - (int)fVar25,
                                                        (byte)(in_fpscr >> 0x16) & 3);
                    fVar27 = fVar27 / fVar28;
                    uVar1 = in_fpscr & 0xfffffff;
                    uVar2 = uVar1 | (uint)(fVar27 < 1.0) << 0x1f | (uint)(fVar27 == 1.0) << 0x1e;
                    in_fpscr = uVar2 | (uint)NAN(fVar27) << 0x1c;
                    bVar4 = (byte)(uVar2 >> 0x18);
                    fVar25 = 1.0;
                    if (((bool)(bVar4 >> 6 & 1) || bVar4 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1)) &&
                       (in_fpscr = uVar1, fVar25 = fVar27, fVar27 < 0.0)) {
                      fVar25 = fVar26;
                    }
                    fVar28 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x60),
                                                        (byte)(in_fpscr >> 0x16) & 3);
                    Player::damage((int)this_01,SUB41((int)(fVar25 * fVar28),0),
                                   (uint)(byte)this[0xf9]);
                    if (0 < *(int *)(this + 100)) {
                      fVar28 = (float)VectorSignedToFloat(*(int *)(this + 100),
                                                          (byte)(in_fpscr >> 0x16) & 3);
                      Player::damageEmp(this_01,(int)(fVar25 * fVar28),(bool)this[0xf9]);
                    }
                  }
                }
              }
            }
            else if (!bVar7) {
              Player::setBombForce(extraout_s0);
              if (!bVar6) goto LAB_0016326e;
              Player::damage((int)this_01,true,0);
              puVar22 = *(undefined4 **)(DAT_00163744 + 0x163678);
              iVar19 = Achievements::hasMedal((Achievements *)*puVar22,0x29,1);
              if ((iVar19 == 0) &&
                 ((iVar19 = *(int *)(this + 0x5c), iVar19 - 4U < 2 ||
                  (iVar19 == 0x28 || iVar19 == 0x22)))) {
                piVar14 = *(int **)(DAT_00163748 + 0x163702);
                iVar19 = *piVar14;
                iVar11 = *(int *)(iVar19 + 300) + 1;
                *(int *)(iVar19 + 300) = iVar11;
                iVar19 = Achievements::getValue((Achievements *)*puVar22,0x29,1);
                if (iVar19 <= iVar11) {
                  pPVar17 = (PlayerEgo *)Level::getPlayer();
                  pHVar18 = (Hud *)PlayerEgo::getHUD(pPVar17);
                  Hud::hudEventMedal(pHVar18,0x29,100);
                  *(undefined1 *)(*piVar14 + 0x130) = 1;
                }
              }
              else if ((*(int *)(this + 0x58) == 0xb3) &&
                      (iVar19 = Achievements::hasMedal((Achievements *)*puVar22,0x2c,1), iVar19 == 0
                      )) {
                piVar14 = *(int **)(DAT_0016374c + 0x16369e);
                iVar19 = *piVar14;
                iVar11 = *(int *)(iVar19 + 0x144) + 1;
                *(int *)(iVar19 + 0x144) = iVar11;
                iVar19 = Achievements::getValue((Achievements *)*puVar22,0x2c,1);
                if (iVar19 <= iVar11) {
                  pPVar17 = (PlayerEgo *)Level::getPlayer();
                  pHVar18 = (Hud *)PlayerEgo::getHUD(pPVar17);
                  Hud::hudEventMedal(pHVar18,0x2c,100);
                  *(undefined1 *)(*piVar14 + 0x148) = 1;
                }
              }
              goto LAB_00163640;
            }
          }
        }
      }
      puVar10 = *(uint **)(this + 0xb4);
    }
  }
LAB_00163640:
  if (*piVar9 != local_6c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00162f90: push {r4,r5,r6,r7,lr}
  00162f92: add r7,sp,#0xc
  00162f94: push {r7,r8,r9,r10,r11}
  00162f98: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  00162f9c: sub sp,#0xd0
  00162f9e: mov r9,r0
  00162fa0: ldr r0,[0x001632f4]
  00162fa2: add r0,pc
  00162fa4: ldr r0,[r0,#0x0]
  00162fa6: str r0,[sp,#0xc]
  00162fa8: ldr r0,[r0,#0x0]
  00162faa: str r0,[sp,#0xcc]
  00162fac: ldr.w r0,[r9,#0xb4]
  00162fb0: cmp r0,#0x0
  00162fb2: beq.w 0x00163640
  00162fb6: ldr.w r2,[r9,#0x5c]
  00162fba: subs r1,r2,#0x6
  00162fbc: cmp r1,#0x2
  00162fbe: bcc 0x00162fca
  00162fc0: cmp r2,#0x2a
  00162fc2: it ne
  00162fc4: cmp.ne r2,#0x22
  00162fc6: bne.w 0x001635b2
  00162fca: sub.w r1,r2,#0x19
  00162fce: clz r1,r1
  00162fd2: lsrs r1,r1,#0x5
  00162fd4: str r1,[sp,#0x20]
  00162fd6: sub.w r1,r2,#0xb
  00162fda: clz r1,r1
  00162fde: movs r2,#0x1
  00162fe0: lsrs r5,r1,#0x5
  00162fe2: movs r1,#0x1
  00162fe4: str r2,[sp,#0x14]
  00162fe6: vmov.f32 s16,0x40400000
  00162fea: vldr.32 s22,[pc,#0x30c]
  00162fee: vmov.f32 s18,0x3e800000
  00162ff2: vldr.32 s24,[pc,#0x308]
  00162ff6: vmov.f32 s20,0x3f800000
  00162ffa: movs r2,#0x0
  00162ffc: vmov.f32 s26,0x3fc00000
  00163000: mov r6,r9
  00163002: vmov.f32 s28,0x3f000000
  00163006: str.w r2,[r6],#0xd8
  0016300a: orrs r1,r5
  0016300c: str r1,[sp,#0x10]
  0016300e: str r5,[sp,#0x24]
  00163010: str r6,[sp,#0x18]
  00163012: b 0x00163512
  00163014: ldr r0,[r0,#0x4]
  00163016: cmp r5,#0x0
  00163018: str r2,[sp,#0x1c]
  0016301a: ldr.w r8,[r0,r2,lsl #0x2]
  0016301e: beq 0x00163042
  00163020: ldrb.w r0,[r8,#0x5c]
  00163024: cmp r0,#0x0
  00163026: beq.w 0x00163508
  0016302a: mov r0,r8
  0016302c: blx 0x00075850
  00163030: cbz r0,0x00163042
  00163032: mov r0,r8
  00163034: blx 0x00075850
  00163038: ldrb.w r0,[r0,#0x41]
  0016303c: cmp r0,#0x0
  0016303e: bne.w 0x00163508
  00163042: mov r0,r8
  00163044: blx 0x00071f14
  00163048: cmp r0,#0x0
  0016304a: beq.w 0x00163508
  0016304e: mov r0,r8
  00163050: blx 0x000724f0
  00163054: cmp r0,#0x1
  00163056: blt.w 0x00163508
  0016305a: mov.w r11,#0x0
  0016305e: ldr.w r0,[r9,#0x8]
  00163062: cmp r11,r0
  00163064: bcs.w 0x00163508
  00163068: ldr.w r0,[r9,#0xc]
  0016306c: add.w r4,r11,r11, lsl #0x1
  00163070: add.w r1,r8,#0x4
  00163074: str.w r11,[sp,#0x28]
  00163078: add.w r0,r0,r4, lsl #0x2
  0016307c: vldr.64 d16,[r0]
  00163080: ldr r0,[r0,#0x8]
  00163082: str r0,[sp,#0xc8]
  00163084: vstr.64 d16,[sp,#0xc0]
  00163088: ldr.w r0,[r9,#0x18]
  0016308c: add.w r0,r0,r4, lsl #0x2
  00163090: vldr.64 d16,[r0]
  00163094: ldr r0,[r0,#0x8]
  00163096: str r0,[sp,#0xb8]
  00163098: add r0,sp,#0xa4
  0016309a: vstr.64 d16,[sp,#0xb0]
  0016309e: blx 0x0006f16c
  001630a2: ldrb.w r0,[r9,#0x68]
  001630a6: add.w r10,r8,#0x40
  001630aa: cmp r0,#0x0
  001630ac: mov r0,r10
  001630ae: it ne
  001630b0: add.ne.w r0,r9,#0x104
  001630b4: ldr.w r11,[r0,#0x0]
  001630b8: ldr r0,[sp,#0x24]
  001630ba: cbz r0,0x001630c2
  001630bc: add.w r11,r11,r11, lsl #0x2
  001630c0: b 0x00163160
  001630c2: ldr r0,[sp,#0x20]
  001630c4: cmp r0,#0x0
  001630c6: beq 0x00163160
  001630c8: ldr.w r0,[r9,#0x38]
  001630cc: mov r6,r4
  001630ce: blx 0x00072034
  001630d2: add r4,sp,#0x2c
  001630d4: mov r1,r0
  001630d6: mov r0,r4
  001630d8: blx 0x0007264c
  001630dc: add r5,sp,#0x68
  001630de: add r1,sp,#0xa4
  001630e0: mov r2,r4
  001630e2: mov r0,r5
  001630e4: blx 0x0006ec38
  001630e8: mov r0,r5
  001630ea: blx 0x0006ec44
  001630ee: vmov s0,r0
  001630f2: mov r0,r8
  001630f4: vcvt.s32.f32 s30,s0
  001630f8: blx 0x00075850
  001630fc: cbz r0,0x00163118
  001630fe: mov r0,r8
  00163100: blx 0x00075850
  00163104: ldrb.w r0,[r0,#0x71]
  00163108: cbz r0,0x00163118
  0016310a: vmov s0,r11
  0016310e: vcvt.f32.s32 s0,s0
  00163112: vmul.f32 s0,s0,s28
  00163116: b 0x00163156
  00163118: vmov r0,s30
  0016311c: movw r1,#0x4e21
  00163120: cmp r0,r1
  00163122: blt 0x00163132
  00163124: add.w r0,r11,r11, lsl #0x1
  00163128: vmov s0,r0
  0016312c: vcvt.f32.s32 s0,s0
  00163130: b 0x00163156
  00163132: vmov s0,r11
  00163136: lsl.w r1,r11,#0x1
  0016313a: vmov s2,r1
  0016313e: movw r1,#0x2710
  00163142: vcvt.f32.s32 s0,s0
  00163146: cmp r0,r1
  00163148: vcvt.f32.s32 s2,s2
  0016314c: vmul.f32 s0,s0,s26
  00163150: it gt
  00163152: vmov.gt.f32 s0,s2
  00163156: vcvt.s32.f32 s0,s0
  0016315a: mov r4,r6
  0016315c: vmov r11,s0
  00163160: mov r0,r8
  00163162: blx 0x00075850
  00163166: ldrb.w r1,[r8,#0x69]
  0016316a: cbz r1,0x00163170
  0016316c: cbnz r0,0x0016317c
  0016316e: b 0x001631b2
  00163170: cmp r0,#0x0
  00163172: beq 0x00163256
  00163174: ldrb.w r1,[r0,#0x8c]
  00163178: cmp r1,#0x0
  0016317a: beq 0x00163256
  0016317c: ldrb.w r1,[r0,#0x40]
  00163180: cbz r1,0x001631b2
  00163182: ldr r1,[r0,#0x0]
  00163184: ldr r2,[r1,#0x40]
  00163186: add r1,sp,#0xc0
  00163188: blx r2
  0016318a: cbz r0,0x001631b6
  0016318c: mov r0,r8
  0016318e: blx 0x00077308
  00163192: cmp r0,#0x0
  00163194: beq 0x00163260
  00163196: ldr r0,[sp,#0x24]
  00163198: ldr.w r11,[sp,#0x28]
  0016319c: ldr r6,[sp,#0x18]
  0016319e: cmp r0,#0x0
  001631a0: bne 0x0016325a
  001631a2: mov r0,r8
  001631a4: movs r1,#0x0
  001631a6: blx 0x00072aa8
  001631aa: ldr r0,[sp,#0x14]
  001631ac: cmp r0,#0x0
  001631ae: beq 0x0016326e
  001631b0: b 0x0016365e
  001631b2: movs r0,#0x0
  001631b4: b 0x001631b8
  001631b6: movs r0,#0x1
  001631b8: vldr.32 s0,[sp,#0xc0]
  001631bc: vldr.32 s2,[sp,#0xa4]
  001631c0: vldr.32 s4,[sp,#0xb0]
  001631c4: vsub.f32 s2,s2,s0
  001631c8: vmov s0,r11
  001631cc: vcvt.f32.s32 s0,s0
  001631d0: vadd.f32 s4,s2,s4
  001631d4: vcmpe.f32 s4,s0
  001631d8: vmrs apsr,fpscr
  001631dc: bpl 0x00163256
  001631de: rsb.w r1,r11,#0x0
  001631e2: vmov s2,r1
  001631e6: vcvt.f32.s32 s2,s2
  001631ea: vcmpe.f32 s4,s2
  001631ee: vmrs apsr,fpscr
  001631f2: ble 0x00163256
  001631f4: vldr.32 s4,[sp,#0xc4]
  001631f8: vldr.32 s6,[sp,#0xa8]
  001631fc: ldr.w r11,[sp,#0x28]
  00163200: vsub.f32 s4,s6,s4
  00163204: vldr.32 s6,[sp,#0xb4]
  00163208: vadd.f32 s4,s4,s6
  0016320c: vcmpe.f32 s4,s0
  00163210: vmrs apsr,fpscr
  00163214: bpl 0x0016325a
  00163216: vcmpe.f32 s4,s2
  0016321a: vmrs apsr,fpscr
  0016321e: ble 0x0016325a
  00163220: vldr.32 s4,[sp,#0xc8]
  00163224: movs r1,#0x0
  00163226: vldr.32 s6,[sp,#0xac]
  0016322a: movs r2,#0x0
  0016322c: vsub.f32 s4,s6,s4
  00163230: vldr.32 s6,[sp,#0xb8]
  00163234: vadd.f32 s4,s4,s6
  00163238: vcmpe.f32 s4,s0
  0016323c: vmrs apsr,fpscr
  00163240: vcmpe.f32 s4,s2
  00163244: it pl
  00163246: mov.pl r1,#0x1
  00163248: vmrs apsr,fpscr
  0016324c: it le
  0016324e: mov.le r2,#0x1
  00163250: orrs r1,r2
  00163252: orrs r0,r1
  00163254: beq 0x0016318c
  00163256: ldr.w r11,[sp,#0x28]
  0016325a: add.w r11,r11,#0x1
  0016325e: b 0x0016305e
  00163260: ldr r0,[sp,#0x24]
  00163262: ldr.w r11,[sp,#0x28]
  00163266: ldr r6,[sp,#0x18]
  00163268: cmp r0,#0x0
  0016326a: bne.w 0x0016351c
  0016326e: ldr r0,[sp,#0x10]
  00163270: cmp r0,#0x0
  00163272: bne.w 0x001635aa
  00163276: ldr.w r1,[r9,#0x64]
  0016327a: ldr r0,[0x00163300]
  0016327c: cmp r1,r0
  0016327e: beq 0x0016328a
  00163280: ldrb.w r2,[r9,#0xf9]
  00163284: mov r0,r8
  00163286: blx 0x000773b0
  0016328a: ldr.w r0,[r9,#0x4]
  0016328e: cbz r0,0x00163296
  00163290: ldrb.w r0,[r0,#0x5c]
  00163294: cbz r0,0x001632ca
  00163296: ldrb.w r0,[r8,#0x69]
  0016329a: add.w r10,sp,#0xc0
  0016329e: cbz r0,0x00163304
  001632a0: ldr.w r0,[r9,#0x38]
  001632a4: blx 0x00072034
  001632a8: cbz r0,0x00163304
  001632aa: ldr.w r0,[r9,#0x38]
  001632ae: blx 0x00072034
  001632b2: blx 0x00075928
  001632b6: cbz r0,0x00163304
  001632b8: vldr.32 s0,[r9,#0x60]
  001632bc: vcvt.f32.s32 s0,s0
  001632c0: vmul.f32 s0,s0,s16
  001632c4: vmul.f32 s0,s0,s18
  001632c8: b 0x001632e0
  001632ca: ldrb.w r0,[r8,#0x69]
  001632ce: add.w r10,sp,#0xc0
  001632d2: cbz r0,0x00163304
  001632d4: vldr.32 s0,[r9,#0x60]
  001632d8: vcvt.f32.s32 s0,s0
  001632dc: vmul.f32 s0,s0,s24
  001632e0: ldr.w r3,[r9,#0x58]
  001632e4: ldrb.w r2,[r9,#0xf9]
  001632e8: vcvt.s32.f32 s0,s0
  001632ec: vmov r1,s0
  001632f0: b 0x00163310
  00163304: ldr.w r3,[r9,#0x58]
  00163308: ldr.w r1,[r9,#0x60]
  0016330c: ldrb.w r2,[r9,#0xf9]
  00163310: mov r0,r8
  00163312: blx 0x00072f94
  00163316: movs r1,#0x1
  00163318: strb.w r1,[r8,#0x54]
  0016331c: ldrb.w r0,[r9,#0xf0]
  00163320: cbz r0,0x0016332a
  00163322: ldr.w r0,[r9,#0x38]
  00163326: strb.w r1,[r0,#0x30]
  0016332a: add r3,sp,#0xb0
  0016332c: ldmia r3,{r0,r2,r3}
  0016332e: eor r1,r0,#0x80000000
  00163332: mov r0,r8
  00163334: eor r2,r2,#0x80000000
  00163338: eor r3,r3,#0x80000000
  0016333c: blx 0x00072a9c
  00163340: ldr.w r0,[r9,#0x3c]
  00163344: ldr r1,[0x001636dc]
  00163346: str.w r1,[r0,r11,lsl #0x2]
  0016334a: mov r0,r8
  0016334c: blx 0x00075850
  00163350: str.w r0,[r9,#0x0]
  00163354: ldr r0,[sp,#0x14]
  00163356: cbz r0,0x00163372
  00163358: ldr.w r0,[r9,#0x38]
  0016335c: add r2,sp,#0xb0
  0016335e: movs r3,#0x0
  00163360: ldr r1,[r0,#0x3c]
  00163362: ldr r0,[r0,#0x74]
  00163364: str r2,[sp,#0x0]
  00163366: ldr r2,[0x001636e0]
  00163368: str r2,[sp,#0x4]
  0016336a: mov r2,r10
  0016336c: blx 0x00072a30
  00163370: b 0x00163400
  00163372: ldr.w r0,[r9,#0x10c]
  00163376: cmp r0,#0x0
  00163378: beq 0x00163400
  0016337a: ldr r1,[0x001636e4]
  0016337c: add r1,pc
  0016337e: ldr.w r11,[r1,#0x0]
  00163382: ldr r1,[sp,#0x28]
  00163384: ldr.w r1,[r0,r1,lsl #0x2]
  00163388: ldr.w r0,[r11,#0x0]
  0016338c: blx 0x00072088
  00163390: movs r1,#0x3
  00163392: movs r2,#0x0
  00163394: blx 0x0006fd18
  00163398: ldr.w r1,[r9,#0x10c]
  0016339c: ldr r2,[sp,#0x28]
  0016339e: ldr.w r0,[r11,#0x0]
  001633a2: ldr.w r1,[r1,r2,lsl #0x2]
  001633a6: blx 0x00072088
  001633aa: movs r1,#0x1
  001633ac: movs r2,#0x0
  001633ae: blx 0x0006fd18
  001633b2: ldr.w r10,[r11,#0x0]
  001633b6: mov r0,r10
  001633b8: blx 0x000717f4
  001633bc: mov r1,r0
  001633be: mov r0,r10
  001633c0: add.w r10,sp,#0xc0
  001633c4: blx 0x0006ff1c
  001633c8: add r5,sp,#0x68
  001633ca: mov r1,r0
  001633cc: movs r2,#0x3c
  001633ce: mov r0,r5
  001633d0: blx 0x0006f1e4
  001633d4: ldrd r2,r3,[sp,#0xc0]
  001633d8: add r0,sp,#0x2c
  001633da: vldr.32 s0,[sp,#0xc8]
  001633de: mov r1,r5
  001633e0: vstr.32 s0,[sp]
  001633e4: blx 0x0006f820
  001633e8: ldr.w r0,[r11,#0x0]
  001633ec: mov r2,r5
  001633ee: ldr.w r1,[r9,#0x10c]
  001633f2: ldr.w r11,[sp,#0x28]
  001633f6: ldr r6,[sp,#0x18]
  001633f8: ldr.w r1,[r1,r11,lsl #0x2]
  001633fc: blx 0x000721c0
  00163400: ldr.w r0,[r9,#0xb8]
  00163404: cbz r0,0x0016341e
  00163406: blx 0x000773c8
  0016340a: cbz r0,0x00163412
  0016340c: movs r0,#0x1
  0016340e: strb.w r0,[r8,#0x55]
  00163412: ldr.w r0,[r9,#0xb8]
  00163416: cbz r0,0x0016341e
  00163418: mov r1,r10
  0016341a: blx 0x000773bc
  0016341e: ldr r0,[sp,#0x20]
  00163420: cmp r0,#0x0
  00163422: beq.w 0x0016325a
  00163426: ldr.w r0,[r9,#0x40]
  0016342a: movs r1,#0x1
  0016342c: strb.w r1,[r0,r11]
  00163430: mov r1,r10
  00163432: ldr.w r0,[r9,#0x30]
  00163436: add.w r0,r0,r4, lsl #0x2
  0016343a: blx 0x0006eb3c
  0016343e: movs r5,#0x0
  00163440: b 0x001634fc
  00163442: ldr r0,[r0,#0x4]
  00163444: add r4,sp,#0x68
  00163446: ldr.w r8,[r0,r5,lsl #0x2]
  0016344a: mov r0,r4
  0016344c: mov r1,r8
  0016344e: blx 0x00072964
  00163452: mov r0,r6
  00163454: mov r1,r4
  00163456: blx 0x0006eb3c
  0016345a: mov r0,r6
  0016345c: mov r1,r10
  0016345e: blx 0x00072694
  00163462: mov r0,r6
  00163464: blx 0x0006ec44
  00163468: vmov s0,r0
  0016346c: ldr.w r0,[r9,#0x100]
  00163470: vcvt.s32.f32 s0,s0
  00163474: vmov r1,s0
  00163478: cmp r0,r1
  0016347a: ble 0x001634fa
  0016347c: vmov s0,r0
  00163480: subs r0,r0,r1
  00163482: vmov.f32 s30,s20
  00163486: vmov s2,r0
  0016348a: vcvt.f32.s32 s0,s0
  0016348e: vcvt.f32.s32 s2,s2
  00163492: vdiv.f32 s0,s2,s0
  00163496: vcmpe.f32 s0,s20
  0016349a: vmrs apsr,fpscr
  0016349e: bgt 0x001634b2
  001634a0: vcmpe.f32 s0,#0
  001634a4: vmrs apsr,fpscr
  001634a8: vmov.f32 s30,s0
  001634ac: it mi
  001634ae: vmov.mi.f32 s30,s22
  001634b2: vldr.32 s0,[r9,#0x60]
  001634b6: mov r0,r8
  001634b8: vcvt.f32.s32 s0,s0
  001634bc: ldr.w r3,[r9,#0x58]
  001634c0: ldrb.w r2,[r9,#0xf9]
  001634c4: vmul.f32 s0,s30,s0
  001634c8: vcvt.s32.f32 s0,s0
  001634cc: vmov r1,s0
  001634d0: blx 0x00072f94
  001634d4: ldr.w r0,[r9,#0x64]
  001634d8: cmp r0,#0x1
  001634da: blt 0x001634fa
  001634dc: vmov s0,r0
  001634e0: mov r0,r8
  001634e2: vcvt.f32.s32 s0,s0
  001634e6: ldrb.w r2,[r9,#0xf9]
  001634ea: vmul.f32 s0,s30,s0
  001634ee: vcvt.s32.f32 s0,s0
  001634f2: vmov r1,s0
  001634f6: blx 0x000773b0
  001634fa: adds r5,#0x1
  001634fc: ldr.w r0,[r9,#0xb4]
  00163500: ldr r1,[r0,#0x0]
  00163502: cmp r5,r1
  00163504: bcc 0x00163442
  00163506: b 0x0016325a
  00163508: ldr r2,[sp,#0x1c]
  0016350a: ldr.w r0,[r9,#0xb4]
  0016350e: adds r2,#0x1
  00163510: ldr r5,[sp,#0x24]
  00163512: ldr r1,[r0,#0x0]
  00163514: cmp r2,r1
  00163516: bcc.w 0x00163014
  0016351a: b 0x00163640
  0016351c: vldr.32 s0,[sp,#0xc0]
  00163520: vldr.32 s2,[sp,#0xa4]
  00163524: ldr.w r0,[r10,#0x0]
  00163528: vsub.f32 s2,s2,s0
  0016352c: vldr.32 s4,[sp,#0xb0]
  00163530: vmov s0,r0
  00163534: vcvt.f32.s32 s0,s0
  00163538: vadd.f32 s4,s2,s4
  0016353c: vcmpe.f32 s4,s0
  00163540: vmrs apsr,fpscr
  00163544: bpl 0x001635ea
  00163546: rsbs r0,r0
  00163548: vmov s2,r0
  0016354c: vcvt.f32.s32 s2,s2
  00163550: vcmpe.f32 s4,s2
  00163554: vmrs apsr,fpscr
  00163558: ble 0x001635ea
  0016355a: vldr.32 s4,[sp,#0xc4]
  0016355e: vldr.32 s6,[sp,#0xa8]
  00163562: vsub.f32 s4,s6,s4
  00163566: vldr.32 s6,[sp,#0xb4]
  0016356a: vadd.f32 s4,s4,s6
  0016356e: vcmpe.f32 s4,s0
  00163572: vmrs apsr,fpscr
  00163576: bpl 0x001635ea
  00163578: vcmpe.f32 s4,s2
  0016357c: vmrs apsr,fpscr
  00163580: ble 0x001635ea
  00163582: vldr.32 s4,[sp,#0xc8]
  00163586: vldr.32 s6,[sp,#0xac]
  0016358a: vsub.f32 s4,s6,s4
  0016358e: vldr.32 s6,[sp,#0xb8]
  00163592: vadd.f32 s4,s4,s6
  00163596: vcmpe.f32 s4,s0
  0016359a: vmrs apsr,fpscr
  0016359e: bpl 0x001635ea
  001635a0: vcmpe.f32 s4,s2
  001635a4: vmrs apsr,fpscr
  001635a8: ble 0x001635ea
  001635aa: mov r0,r9
  001635ac: blx 0x00072fdc
  001635b0: b 0x00163640
  001635b2: sub.w r1,r2,#0x19
  001635b6: subs r3,r2,#0x4
  001635b8: clz r1,r1
  001635bc: cmp r3,#0x2
  001635be: lsr.w r1,r1,#0x5
  001635c2: str r1,[sp,#0x20]
  001635c4: sub.w r1,r2,#0xb
  001635c8: clz r1,r1
  001635cc: lsr.w r5,r1,#0x5
  001635d0: mov.w r1,#0x1
  001635d4: str r1,[sp,#0x14]
  001635d6: mov.w r1,#0x0
  001635da: bcc.w 0x00162fe6
  001635de: cmp r2,#0x28
  001635e0: beq.w 0x00162fe6
  001635e4: movs r1,#0x0
  001635e6: movs r2,#0x0
  001635e8: b 0x00162fe4
  001635ea: add.w r8,sp,#0x2c
  001635ee: add r1,sp,#0xa4
  001635f0: add r2,sp,#0xc0
  001635f2: mov r0,r8
  001635f4: blx 0x0006ec38
  001635f8: add r6,sp,#0x68
  001635fa: mov r1,r8
  001635fc: mov r0,r6
  001635fe: blx 0x0006ec80
  00163602: ldr.w r0,[r9,#0x18]
  00163606: mov r1,r6
  00163608: add.w r0,r0,r4, lsl #0x2
  0016360c: blx 0x0006eb3c
  00163610: ldr.w r0,[r9,#0x18]
  00163614: add.w r8,sp,#0x2c
  00163618: ldr.w r2,[r9,#0x50]
  0016361c: add.w r1,r0,r4, lsl #0x2
  00163620: mov r0,r8
  00163622: blx 0x0006ec74
  00163626: add r6,sp,#0x68
  00163628: ldr r2,[0x00163740]
  0016362a: mov r1,r8
  0016362c: mov r0,r6
  0016362e: blx 0x0006ec74
  00163632: ldr.w r0,[r9,#0xc]
  00163636: mov r1,r6
  00163638: add.w r0,r0,r4, lsl #0x2
  0016363c: blx 0x00073534
  00163640: ldr r0,[sp,#0xcc]
  00163642: ldr r1,[sp,#0xc]
  00163644: ldr r1,[r1,#0x0]
  00163646: subs r0,r1,r0
  00163648: itttt eq
  0016364a: add.eq sp,#0xd0
  0016364c: vpop.eq {d8,d9,d10,d11,d12,d13,d14,d15}
  00163650: add.eq sp,#0x4
  00163652: pop.eq.w {r8,r9,r10,r11}
  00163656: it eq
  00163658: pop.eq {r4,r5,r6,r7,pc}
  0016365a: blx 0x0006e824
  0016365e: ldr.w r3,[r9,#0x58]
  00163662: mov r0,r8
  00163664: movw r1,#0x270f
  00163668: movs r2,#0x0
  0016366a: blx 0x00072f94
  0016366e: ldr r0,[0x00163744]
  00163670: movs r1,#0x29
  00163672: movs r2,#0x1
  00163674: add r0,pc
  00163676: ldr r5,[r0,#0x0]
  00163678: ldr r0,[r5,#0x0]
  0016367a: blx 0x00072ef8
  0016367e: cbz r0,0x001636e8
  00163680: ldr.w r0,[r9,#0x58]
  00163684: cmp r0,#0xb3
  00163686: bne 0x00163640
  00163688: ldr r0,[r5,#0x0]
  0016368a: movs r1,#0x2c
  0016368c: movs r2,#0x1
  0016368e: blx 0x00072ef8
  00163692: cmp r0,#0x0
  00163694: bne 0x00163640
  00163696: ldr r0,[0x0016374c]
  00163698: movs r2,#0x1
  0016369a: add r0,pc
  0016369c: ldr.w r8,[r0,#0x0]
  001636a0: ldr.w r0,[r8,#0x0]
  001636a4: ldr.w r1,[r0,#0x144]
  001636a8: adds r6,r1,#0x1
  001636aa: str.w r6,[r0,#0x144]
  001636ae: ldr r0,[r5,#0x0]
  001636b0: movs r1,#0x2c
  001636b2: movs r5,#0x1
  001636b4: blx 0x00072f04
  001636b8: cmp r6,r0
  001636ba: blt 0x00163640
  001636bc: ldr.w r0,[r9,#0x38]
  001636c0: blx 0x00072034
  001636c4: blx 0x00072f10
  001636c8: movs r1,#0x2c
  001636ca: movs r2,#0x64
  001636cc: blx 0x00072f1c
  001636d0: ldr.w r0,[r8,#0x0]
  001636d4: strb.w r5,[r0,#0x148]
  001636d8: b 0x00163640
  001636e8: ldr.w r0,[r9,#0x5c]
  001636ec: subs r1,r0,#0x4
  001636ee: cmp r1,#0x2
  001636f0: bcc 0x001636fa
  001636f2: cmp r0,#0x28
  001636f4: it ne
  001636f6: cmp.ne r0,#0x22
  001636f8: bne 0x00163680
  001636fa: ldr r0,[0x00163748]
  001636fc: movs r2,#0x1
  001636fe: add r0,pc
  00163700: ldr.w r8,[r0,#0x0]
  00163704: ldr.w r0,[r8,#0x0]
  00163708: ldr.w r1,[r0,#0x12c]
  0016370c: adds r6,r1,#0x1
  0016370e: str.w r6,[r0,#0x12c]
  00163712: ldr r0,[r5,#0x0]
  00163714: movs r1,#0x29
  00163716: movs r5,#0x1
  00163718: blx 0x00072f04
  0016371c: cmp r6,r0
  0016371e: blt 0x00163640
  00163720: ldr.w r0,[r9,#0x38]
  00163724: blx 0x00072034
  00163728: blx 0x00072f10
  0016372c: movs r1,#0x29
  0016372e: movs r2,#0x64
  00163730: blx 0x00072f1c
  00163734: ldr.w r0,[r8,#0x0]
  00163738: strb.w r5,[r0,#0x130]
  0016373c: b 0x00163640
```
