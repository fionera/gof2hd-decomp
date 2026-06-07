# Gun::ignite
elf 0x152b7c body 996
Sig: undefined __thiscall ignite(Gun * this)

## decompile
```c

/* Gun::ignite() */

void __thiscall Gun::ignite(Gun *this)

{
  uint uVar1;
  byte bVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  uint *puVar6;
  Player *this_00;
  int iVar7;
  float fVar8;
  int iVar9;
  undefined4 uVar10;
  undefined4 *puVar11;
  int *piVar12;
  PlayerEgo *this_01;
  Hud *this_02;
  uint uVar13;
  int iVar14;
  uint uVar15;
  int *piVar16;
  Vector *this_03;
  code *pcVar17;
  Vector *this_04;
  uint uVar18;
  uint in_fpscr;
  int iVar19;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float fVar20;
  float extraout_s2;
  undefined8 in_d6;
  undefined4 in_s14;
  float fVar21;
  undefined8 local_68;
  undefined4 local_60;
  int local_5c;
  
  piVar16 = *(int **)(DAT_00162e84 + 0x162b90);
  _local_60 = CONCAT44(*piVar16,in_s14);
  if (*(int *)(this + 0x5c) != 6) {
    if (*(int *)(this + 0x5c) != 7) goto LAB_00162bb8;
    *(int *)(**(int **)(DAT_00162e88 + 0x162ba4) + 200) =
         *(int *)(**(int **)(DAT_00162e88 + 0x162ba4) + 200) + 1;
  }
  *(undefined1 *)(*(int *)(this + 0x38) + 0x69) = 0;
LAB_00162bb8:
  puVar6 = *(uint **)(this + 0xb4);
  this[0x88] = (Gun)0x1;
  local_68 = in_d6;
  if (puVar6 != (uint *)0x0) {
    this_03 = (Vector *)(this + 0xd8);
    *(undefined4 *)this = 0;
    fVar5 = DAT_00162f7c;
    fVar4 = DAT_00162f70;
    fVar3 = DAT_00162f6c;
    this_04 = (Vector *)(this + 0xc0);
    for (uVar15 = 0; uVar15 < *puVar6; uVar15 = uVar15 + 1) {
      this_00 = *(Player **)(puVar6[1] + uVar15 * 4);
      *(Player **)(this + 0xbc) = this_00;
      if (((*(int *)(this + 0x5c) != 6) || (iVar7 = Player::isAsteroid(this_00), iVar7 == 0)) &&
         (iVar7 = Player::isActive(), iVar7 != 0)) {
        iVar7 = 0;
        for (uVar18 = 0; uVar18 < *(uint *)(this + 8); uVar18 = uVar18 + 1) {
          local_68 = *(undefined8 *)(*(int *)(this + 0xc) + iVar7);
          _local_60 = CONCAT44(local_5c,*(undefined4 *)
                                         ((undefined8 *)(*(int *)(this + 0xc) + iVar7) + 1));
          AbyssEngine::AEMath::Vector::operator=(this_04,(Vector *)&local_68);
          Player::getPosition();
          AbyssEngine::AEMath::Vector::operator=(this_03,(Vector *)&local_68);
          AbyssEngine::AEMath::Vector::operator-=(this_03,this_04);
          fVar8 = (float)AbyssEngine::AEMath::VectorLength(this_03);
          iVar19 = (int)fVar8;
          if (iVar19 < *(int *)(this + 0x100)) {
            *(undefined1 *)(*(int *)(this + 0x40) + uVar18) = 1;
            AbyssEngine::AEMath::Vector::operator=
                      ((Vector *)(*(int *)(this + 0x30) + iVar7),this_04);
            fVar8 = (float)VectorSignedToFloat(*(int *)(this + 0x100),(byte)(in_fpscr >> 0x16) & 3);
            fVar20 = (float)VectorSignedToFloat(*(int *)(this + 0x100) - iVar19,
                                                (byte)(in_fpscr >> 0x16) & 3);
            if (*(int *)(this + 0x5c) == 0xb) {
              fVar20 = (float)VectorSignedToFloat(10000 - iVar19,(byte)(in_fpscr >> 0x16) & 3);
              fVar20 = fVar20 / fVar3;
            }
            else {
              fVar20 = fVar20 / fVar8;
            }
            uVar13 = in_fpscr & 0xfffffff;
            uVar1 = uVar13 | (uint)(fVar20 < 1.0) << 0x1f | (uint)(fVar20 == 1.0) << 0x1e;
            in_fpscr = uVar1 | (uint)NAN(fVar20) << 0x1c;
            bVar2 = (byte)(uVar1 >> 0x18);
            fVar8 = 1.0;
            if (((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1)) &&
               (in_fpscr = uVar13, fVar8 = fVar20, fVar20 < 0.0)) {
              fVar8 = fVar4;
            }
            iVar19 = Item::getAttribute(*(int *)(*(int *)(**(int **)(DAT_00162f80 + 0x162cdc) + 4) +
                                                *(int *)(this + 0x58) * 4));
            uVar13 = *(int *)(this + 0x5c) - 0xb;
            if (((uVar13 < 0x20) && ((1 << (uVar13 & 0xff) & DAT_00162f74) != 0)) ||
               (*(int *)(this + 0x5c) == 7)) {
              fVar20 = extraout_s0;
              if (iVar19 != DAT_00162f78) {
                fVar20 = (float)VectorSignedToFloat(iVar19,(byte)(in_fpscr >> 0x16) & 3);
                fVar20 = (float)Player::setEmpForce(*(Player **)(this + 0xbc),fVar8 * fVar20);
              }
              Player::setBombForce(fVar20);
              iVar9 = Player::isAsteroid(*(Player **)(this + 0xbc));
              if (iVar9 != 0) {
                fVar8 = fVar8 * fVar5;
              }
              if (0 < *(int *)(this + 0x60)) {
                fVar20 = (float)VectorSignedToFloat(*(int *)(this + 0x60),
                                                    (byte)(in_fpscr >> 0x16) & 3);
                Player::damage(*(int *)(this + 0xbc),SUB41((int)(fVar8 * fVar20),0),
                               (uint)(byte)this[0xf9]);
                iVar9 = Player::isGasCloud(*(Player **)(this + 0xbc));
                if ((iVar9 != 0) && (*(int *)(this + 0x5c) == 0x22)) {
                  uVar10 = Player::getKIPlayer(*(Player **)(this + 0xbc));
                  VectorSignedToFloat(*(undefined4 *)(this + 0x100),(byte)(in_fpscr >> 0x16) & 3);
                  iVar9 = *(int *)(this + 0xc) + iVar7;
                  PlayerGasCloud::explode
                            (uVar10,*(undefined4 *)(this + 0x58),
                             *(undefined4 *)(*(int *)(this + 0xc) + iVar7),
                             *(undefined4 *)(iVar9 + 4),*(undefined4 *)(iVar9 + 8));
                }
                if ((*(int *)(this + 0x58) == 0xb3) &&
                   (iVar9 = Player::isAsteroid(*(Player **)(this + 0xbc)), iVar9 != 0)) {
                  fVar21 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x60),
                                                      (byte)(in_fpscr >> 0x16) & 3);
                  uVar10 = Player::getHitpoints();
                  fVar20 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
                  uVar13 = in_fpscr & 0xfffffff | (uint)(fVar8 * fVar21 < fVar20) << 0x1f;
                  in_fpscr = uVar13 | (uint)(NAN(fVar8 * fVar21) || NAN(fVar20)) << 0x1c;
                  if ((byte)(uVar13 >> 0x1f) == ((byte)(in_fpscr >> 0x1c) & 1)) {
                    puVar11 = *(undefined4 **)(DAT_00162f84 + 0x162dea);
                    iVar9 = Achievements::hasMedal((Achievements *)*puVar11,0x2c,1);
                    if (iVar9 == 0) {
                      piVar12 = *(int **)(DAT_00162f88 + 0x162dfc);
                      iVar9 = *piVar12;
                      iVar14 = *(int *)(iVar9 + 0x144) + 1;
                      *(int *)(iVar9 + 0x144) = iVar14;
                      iVar9 = Achievements::getValue((Achievements *)*puVar11,0x2c,1);
                      if (iVar9 <= iVar14) {
                        this_01 = (PlayerEgo *)Level::getPlayer();
                        this_02 = (Hud *)PlayerEgo::getHUD(this_01);
                        Hud::hudEventMedal(this_02,0x2c,100);
                        *(undefined1 *)(*piVar12 + 0x148) = 1;
                      }
                    }
                  }
                }
              }
              if (0 < iVar19) {
                fVar20 = (float)VectorSignedToFloat(iVar19,(byte)(in_fpscr >> 0x16) & 3);
                Player::damageEmp(*(Player **)(this + 0xbc),(int)(fVar8 * fVar20),(bool)this[0xf9]);
              }
              if ((*(int *)(this + 0x5c) == 0x2a) &&
                 (iVar19 = Player::getKIPlayer(*(Player **)(this + 0xbc)), iVar19 != 0)) {
                piVar12 = (int *)Player::getKIPlayer(*(Player **)(this + 0xbc));
                (**(code **)(*piVar12 + 0x2c))(piVar12,this_04,*(undefined4 *)(this + 0x100));
              }
            }
            else {
              fVar20 = (float)VectorSignedToFloat(iVar19,(byte)(in_fpscr >> 0x16) & 3);
              Player::damageEmp(*(Player **)(this + 0xbc),(int)(fVar8 * fVar20),(bool)this[0xf9]);
            }
            AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_68,this_03);
            pcVar17 = *(code **)(DAT_00162f8c + 0x162ebc);
            (*pcVar17)(this_03,(AEMath *)&local_68);
            Player::setHitVector(*(Player **)(this + 0xbc),extraout_s0_00,extraout_s1,extraout_s2);
            local_68 = *(undefined8 *)(*(int *)(this + 0x18) + iVar7);
            _local_60 = CONCAT44(local_5c,*(undefined4 *)
                                           ((undefined8 *)(*(int *)(this + 0x18) + iVar7) + 1));
            (*pcVar17)(this_03,(AEMath *)&local_68);
            AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_68,this_03);
            (*pcVar17)(this_03,(AEMath *)&local_68);
            uVar10 = Player::getKIPlayer(*(Player **)(this + 0xbc));
            *(undefined4 *)this = uVar10;
            *(undefined4 *)(*(int *)(this + 0x3c) + uVar18 * 4) = 0xffffffff;
          }
          iVar7 = iVar7 + 0xc;
          if (*(int *)(this + 0x5c) != 0xb) {
            *(undefined4 *)(*(int *)(this + 0x3c) + uVar18 * 4) = 0xffffffff;
          }
        }
      }
      puVar6 = *(uint **)(this + 0xb4);
    }
    if (*(int *)(this + 0x5c) == 6) {
      **(undefined4 **)(this + 0x3c) = 0xffffffff;
    }
  }
  if (*(Sparks **)(this + 0xb8) != (Sparks *)0x0) {
    Sparks::explode(*(Sparks **)(this + 0xb8),*(Vector **)(this + 0xc));
  }
  if (*piVar16 == local_5c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00162b7c: push {r4,r5,r6,r7,lr}
  00162b7e: add r7,sp,#0xc
  00162b80: push {r7,r8,r9,r10,r11}
  00162b84: vpush {d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13}
  00162b88: mov r4,r0
  00162b8a: ldr r0,[0x00162e84]
  00162b8c: add r0,pc
  00162b8e: ldr r5,[r0,#0x0]
  00162b90: ldr r0,[r5,#0x0]
  00162b92: str r0,[sp,#0x2c]
  00162b94: ldr r0,[r4,#0x5c]
  00162b96: cmp r0,#0x6
  00162b98: beq 0x00162bb0
  00162b9a: cmp r0,#0x7
  00162b9c: bne 0x00162bb8
  00162b9e: ldr r0,[0x00162e88]
  00162ba0: add r0,pc
  00162ba2: ldr r0,[r0,#0x0]
  00162ba4: ldr r0,[r0,#0x0]
  00162ba6: ldr.w r1,[r0,#0xc8]
  00162baa: adds r1,#0x1
  00162bac: str.w r1,[r0,#0xc8]
  00162bb0: ldr r0,[r4,#0x38]
  00162bb2: movs r1,#0x0
  00162bb4: strb.w r1,[r0,#0x69]
  00162bb8: ldr.w r0,[r4,#0xb4]
  00162bbc: movs r1,#0x1
  00162bbe: strb.w r1,[r4,#0x88]
  00162bc2: cmp r0,#0x0
  00162bc4: beq.w 0x00162f46
  00162bc8: vmov.f32 s18,0x3f800000
  00162bcc: str r5,[sp,#0xc]
  00162bce: movs r2,#0x0
  00162bd0: mov r5,r4
  00162bd2: str.w r2,[r5],#0xd8
  00162bd6: add.w r8,r4,#0xc0
  00162bda: add.w r9,sp,#0x20
  00162bde: vldr.32 s16,[pc,#0x38c]
  00162be2: vldr.32 s20,[pc,#0x398]
  00162be6: vldr.32 s22,[pc,#0x388]
  00162bea: b 0x00162f2e
  00162bec: ldr r0,[r0,#0x4]
  00162bee: ldr r1,[r4,#0x5c]
  00162bf0: str r2,[sp,#0x1c]
  00162bf2: ldr.w r0,[r0,r2,lsl #0x2]
  00162bf6: cmp r1,#0x6
  00162bf8: str.w r0,[r4,#0xbc]
  00162bfc: bne 0x00162c0c
  00162bfe: blx 0x00077308
  00162c02: cmp r0,#0x0
  00162c04: bne.w 0x00162f26
  00162c08: ldr.w r0,[r4,#0xbc]
  00162c0c: blx 0x00071f14
  00162c10: cmp r0,#0x0
  00162c12: beq.w 0x00162f26
  00162c16: mov.w r11,#0x0
  00162c1a: mov.w r10,#0x0
  00162c1e: b 0x00162f1e
  00162c20: ldr r0,[r4,#0xc]
  00162c22: mov r1,r9
  00162c24: add r0,r11
  00162c26: vldr.64 d16,[r0]
  00162c2a: ldr r0,[r0,#0x8]
  00162c2c: str r0,[sp,#0x28]
  00162c2e: mov r0,r8
  00162c30: vstr.64 d16,[sp,#0x20]
  00162c34: blx 0x0006eb3c
  00162c38: ldr.w r1,[r4,#0xbc]
  00162c3c: mov r0,r9
  00162c3e: blx 0x00072964
  00162c42: mov r0,r5
  00162c44: mov r1,r9
  00162c46: blx 0x0006eb3c
  00162c4a: mov r0,r5
  00162c4c: mov r1,r8
  00162c4e: blx 0x00072694
  00162c52: mov r0,r5
  00162c54: blx 0x0006ec44
  00162c58: vmov s0,r0
  00162c5c: ldr.w r0,[r4,#0x100]
  00162c60: vcvt.s32.f32 s0,s0
  00162c64: vmov r6,s0
  00162c68: cmp r0,r6
  00162c6a: ble.w 0x00162f06
  00162c6e: ldr r0,[r4,#0x40]
  00162c70: movs r1,#0x1
  00162c72: strb.w r1,[r0,r10]
  00162c76: mov r1,r8
  00162c78: ldr r0,[r4,#0x30]
  00162c7a: add r0,r11
  00162c7c: blx 0x0006eb3c
  00162c80: ldr.w r1,[r4,#0x100]
  00162c84: ldr r0,[r4,#0x5c]
  00162c86: vmov s0,r1
  00162c8a: subs r1,r1,r6
  00162c8c: cmp r0,#0xb
  00162c8e: vmov s2,r1
  00162c92: vcvt.f32.s32 s0,s0
  00162c96: vcvt.f32.s32 s2,s2
  00162c9a: bne 0x00162cb0
  00162c9c: movw r0,#0x2710
  00162ca0: subs r0,r0,r6
  00162ca2: vmov s0,r0
  00162ca6: vcvt.f32.s32 s0,s0
  00162caa: vdiv.f32 s0,s0,s16
  00162cae: b 0x00162cb4
  00162cb0: vdiv.f32 s0,s2,s0
  00162cb4: vmov.f32 s24,s18
  00162cb8: vcmpe.f32 s0,s18
  00162cbc: vmrs apsr,fpscr
  00162cc0: bgt 0x00162cd4
  00162cc2: vcmpe.f32 s0,#0
  00162cc6: vmrs apsr,fpscr
  00162cca: vmov.f32 s24,s0
  00162cce: it mi
  00162cd0: vmov.mi.f32 s24,s22
  00162cd4: ldr r0,[0x00162f80]
  00162cd6: ldr r1,[r4,#0x58]
  00162cd8: add r0,pc
  00162cda: ldr r0,[r0,#0x0]
  00162cdc: ldr r0,[r0,#0x0]
  00162cde: ldr r0,[r0,#0x4]
  00162ce0: ldr.w r0,[r0,r1,lsl #0x2]
  00162ce4: movs r1,#0xa
  00162ce6: blx 0x000718e4
  00162cea: mov r6,r0
  00162cec: ldr r0,[r4,#0x5c]
  00162cee: sub.w r1,r0,#0xb
  00162cf2: cmp r1,#0x1f
  00162cf4: bhi 0x00162d02
  00162cf6: movs r2,#0x1
  00162cf8: lsl.w r1,r2,r1
  00162cfc: ldr r2,[0x00162f74]
  00162cfe: tst r1,r2
  00162d00: bne 0x00162d08
  00162d02: cmp r0,#0x7
  00162d04: bne.w 0x00162e8c
  00162d08: ldr r0,[0x00162f78]
  00162d0a: cmp r6,r0
  00162d0c: beq 0x00162d26
  00162d0e: vmov s0,r6
  00162d12: vcvt.f32.s32 s0,s0
  00162d16: ldr.w r0,[r4,#0xbc]
  00162d1a: vmul.f32 s0,s24,s0
  00162d1e: vmov r1,s0
  00162d22: blx 0x0007738c
  00162d26: vmov r1,s24
  00162d2a: ldr.w r0,[r4,#0xbc]
  00162d2e: blx 0x00072aa8
  00162d32: ldr.w r0,[r4,#0xbc]
  00162d36: vmul.f32 s26,s24,s20
  00162d3a: blx 0x00077308
  00162d3e: cmp r0,#0x0
  00162d40: it ne
  00162d42: vmov.ne.f32 s24,s26
  00162d46: ldr r0,[r4,#0x60]
  00162d48: cmp r0,#0x1
  00162d4a: blt 0x00162e38
  00162d4c: vmov s0,r0
  00162d50: vcvt.f32.s32 s0,s0
  00162d54: ldr r3,[r4,#0x58]
  00162d56: ldr.w r0,[r4,#0xbc]
  00162d5a: ldrb.w r2,[r4,#0xf9]
  00162d5e: vmul.f32 s0,s24,s0
  00162d62: vcvt.s32.f32 s0,s0
  00162d66: vmov r1,s0
  00162d6a: blx 0x00072f94
  00162d6e: ldr.w r0,[r4,#0xbc]
  00162d72: blx 0x00077398
  00162d76: cbz r0,0x00162daa
  00162d78: ldr r0,[r4,#0x5c]
  00162d7a: cmp r0,#0x22
  00162d7c: bne 0x00162daa
  00162d7e: ldr.w r0,[r4,#0xbc]
  00162d82: blx 0x00075850
  00162d86: vldr.32 s0,[r4,#0x100]
  00162d8a: ldr r3,[r4,#0xc]
  00162d8c: ldr r1,[r4,#0x58]
  00162d8e: vcvt.f32.s32 s0,s0
  00162d92: add.w r12,r3,r11
  00162d96: ldr.w r2,[r3,r11]
  00162d9a: ldrd r3,r12,[r12,#0x4]
  00162d9e: str.w r12,[sp,#0x0]
  00162da2: vstr.32 s0,[sp,#0x4]
  00162da6: blx 0x000773a4
  00162daa: ldr r0,[r4,#0x58]
  00162dac: cmp r0,#0xb3
  00162dae: bne 0x00162e38
  00162db0: ldr.w r0,[r4,#0xbc]
  00162db4: blx 0x00077308
  00162db8: cbz r0,0x00162e38
  00162dba: vldr.32 s0,[r4,#0x60]
  00162dbe: vcvt.f32.s32 s26,s0
  00162dc2: ldr.w r0,[r4,#0xbc]
  00162dc6: blx 0x000724f0
  00162dca: vmov s0,r0
  00162dce: vmul.f32 s2,s24,s26
  00162dd2: vcvt.f32.s32 s0,s0
  00162dd6: vcmpe.f32 s2,s0
  00162dda: vmrs apsr,fpscr
  00162dde: blt 0x00162e38
  00162de0: ldr r0,[0x00162f84]
  00162de2: movs r1,#0x2c
  00162de4: movs r2,#0x1
  00162de6: add r0,pc
  00162de8: ldr r0,[r0,#0x0]
  00162dea: str r0,[sp,#0x18]
  00162dec: ldr r0,[r0,#0x0]
  00162dee: blx 0x00072ef8
  00162df2: cbnz r0,0x00162e38
  00162df4: ldr r0,[0x00162f88]
  00162df6: movs r2,#0x1
  00162df8: add r0,pc
  00162dfa: ldr r0,[r0,#0x0]
  00162dfc: str r0,[sp,#0x10]
  00162dfe: ldr r0,[r0,#0x0]
  00162e00: ldr.w r1,[r0,#0x144]
  00162e04: adds r1,#0x1
  00162e06: str.w r1,[r0,#0x144]
  00162e0a: ldr r0,[sp,#0x18]
  00162e0c: str r1,[sp,#0x14]
  00162e0e: movs r1,#0x2c
  00162e10: ldr r0,[r0,#0x0]
  00162e12: blx 0x00072f04
  00162e16: ldr r1,[sp,#0x14]
  00162e18: cmp r1,r0
  00162e1a: blt 0x00162e38
  00162e1c: ldr r0,[r4,#0x38]
  00162e1e: blx 0x00072034
  00162e22: blx 0x00072f10
  00162e26: movs r1,#0x2c
  00162e28: movs r2,#0x64
  00162e2a: blx 0x00072f1c
  00162e2e: ldr r0,[sp,#0x10]
  00162e30: movs r1,#0x1
  00162e32: ldr r0,[r0,#0x0]
  00162e34: strb.w r1,[r0,#0x148]
  00162e38: cmp r6,#0x1
  00162e3a: blt 0x00162e5c
  00162e3c: vmov s0,r6
  00162e40: vcvt.f32.s32 s0,s0
  00162e44: ldrb.w r2,[r4,#0xf9]
  00162e48: ldr.w r0,[r4,#0xbc]
  00162e4c: vmul.f32 s0,s24,s0
  00162e50: vcvt.s32.f32 s0,s0
  00162e54: vmov r1,s0
  00162e58: blx 0x000773b0
  00162e5c: ldr r0,[r4,#0x5c]
  00162e5e: cmp r0,#0x2a
  00162e60: bne 0x00162eac
  00162e62: ldr.w r0,[r4,#0xbc]
  00162e66: blx 0x00075850
  00162e6a: cbz r0,0x00162eac
  00162e6c: ldr.w r0,[r4,#0xbc]
  00162e70: blx 0x00075850
  00162e74: ldr r1,[r0,#0x0]
  00162e76: ldr.w r2,[r4,#0x100]
  00162e7a: ldr r3,[r1,#0x2c]
  00162e7c: mov r1,r8
  00162e7e: blx r3
  00162e80: b 0x00162eac
  00162e8c: vmov s0,r6
  00162e90: vcvt.f32.s32 s0,s0
  00162e94: ldrb.w r2,[r4,#0xf9]
  00162e98: ldr.w r0,[r4,#0xbc]
  00162e9c: vmul.f32 s0,s24,s0
  00162ea0: vcvt.s32.f32 s0,s0
  00162ea4: vmov r1,s0
  00162ea8: blx 0x000773b0
  00162eac: mov r0,r9
  00162eae: mov r1,r5
  00162eb0: blx 0x0006ec80
  00162eb4: ldr r0,[0x00162f8c]
  00162eb6: mov r1,r9
  00162eb8: add r0,pc
  00162eba: ldr r6,[r0,#0x0]
  00162ebc: mov r0,r5
  00162ebe: blx r6
  00162ec0: add.w r3,r4,#0xd8
  00162ec4: ldr.w r0,[r4,#0xbc]
  00162ec8: ldmia r3,{r1,r2,r3}
  00162eca: blx 0x00072a9c
  00162ece: ldr r0,[r4,#0x18]
  00162ed0: mov r1,r9
  00162ed2: add r0,r11
  00162ed4: vldr.64 d16,[r0]
  00162ed8: ldr r0,[r0,#0x8]
  00162eda: str r0,[sp,#0x28]
  00162edc: mov r0,r5
  00162ede: vstr.64 d16,[sp,#0x20]
  00162ee2: blx r6
  00162ee4: mov r0,r9
  00162ee6: mov r1,r5
  00162ee8: blx 0x0006ec80
  00162eec: mov r0,r5
  00162eee: mov r1,r9
  00162ef0: blx r6
  00162ef2: ldr.w r0,[r4,#0xbc]
  00162ef6: blx 0x00075850
  00162efa: ldr r1,[r4,#0x3c]
  00162efc: str r0,[r4,#0x0]
  00162efe: mov.w r0,#0xffffffff
  00162f02: str.w r0,[r1,r10,lsl #0x2]
  00162f06: ldr r0,[r4,#0x5c]
  00162f08: add.w r11,r11,#0xc
  00162f0c: cmp r0,#0xb
  00162f0e: ittt ne
  00162f10: ldr.ne r0,[r4,#0x3c]
  00162f12: mov.ne.w r1,#0xffffffff
  00162f16: str.ne.w r1,[r0,r10,lsl #0x2]
  00162f1a: add.w r10,r10,#0x1
  00162f1e: ldr r0,[r4,#0x8]
  00162f20: cmp r10,r0
  00162f22: bcc.w 0x00162c20
  00162f26: ldr r2,[sp,#0x1c]
  00162f28: ldr.w r0,[r4,#0xb4]
  00162f2c: adds r2,#0x1
  00162f2e: ldr r1,[r0,#0x0]
  00162f30: cmp r2,r1
  00162f32: bcc.w 0x00162bec
  00162f36: ldr r0,[r4,#0x5c]
  00162f38: ldr r5,[sp,#0xc]
  00162f3a: cmp r0,#0x6
  00162f3c: ittt eq
  00162f3e: ldr.eq r0,[r4,#0x3c]
  00162f40: mov.eq.w r1,#0xffffffff
  00162f44: str.eq r1,[r0,#0x0]
  00162f46: ldr.w r0,[r4,#0xb8]
  00162f4a: cbz r0,0x00162f52
  00162f4c: ldr r1,[r4,#0xc]
  00162f4e: blx 0x000773bc
  00162f52: ldr r0,[sp,#0x2c]
  00162f54: ldr r1,[r5,#0x0]
  00162f56: subs r0,r1,r0
  00162f58: itttt eq
  00162f5a: vpop.eq {d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13}
  00162f5e: add.eq sp,#0x4
  00162f60: pop.eq.w {r8,r9,r10,r11}
  00162f64: pop.eq {r4,r5,r6,r7,pc}
  00162f66: blx 0x0006e824
```
