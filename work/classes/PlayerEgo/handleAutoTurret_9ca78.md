# PlayerEgo::handleAutoTurret
elf 0x9ca78 body 910
Sig: undefined __thiscall handleAutoTurret(PlayerEgo * this, int param_1)

## decompile
```c

/* PlayerEgo::handleAutoTurret(int) */

void __thiscall PlayerEgo::handleAutoTurret(PlayerEgo *this,int param_1)

{
  byte bVar1;
  bool bVar2;
  int iVar3;
  uint *puVar4;
  int iVar5;
  Matrix *pMVar6;
  KIPlayer *this_00;
  int *piVar7;
  uint uVar8;
  int *piVar9;
  uint in_fpscr;
  uint uVar10;
  float fVar11;
  float fVar12;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float fVar13;
  float extraout_s2;
  longlong lVar14;
  float local_dc;
  float local_d8;
  Matrix aMStack_a0 [12];
  AEMath aAStack_94 [12];
  Vector aVStack_88 [12];
  Vector aVStack_7c [12];
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  int local_34;
  
  piVar9 = *(int **)(DAT_000ace2c + 0xaca90);
  local_34 = *piVar9;
  iVar3 = this[10].field_1C + param_1;
  this[10].field_1C = iVar3;
  if (3000 < iVar3) {
    this[10].field_1C = 0;
    this[0xb].m_pPlayer = 0;
    puVar4 = (uint *)Level::getEnemies();
    if (puVar4 != (uint *)0x0) {
      iVar3 = 60000;
      for (uVar8 = 0; uVar8 < *puVar4; uVar8 = uVar8 + 1) {
        this_00 = *(KIPlayer **)(puVar4[1] + uVar8 * 4);
        iVar5 = KIPlayer::isDead();
        if ((((iVar5 == 0) && (iVar5 = KIPlayer::isDying(), iVar5 == 0)) &&
            (iVar5 = Player::isActive(), iVar5 != 0)) &&
           ((iVar5 = KIPlayer::isEnemy(this_00), iVar5 != 0 && (this_00[0x74] == (KIPlayer)0x0)))) {
          (**(code **)(*(int *)this_00 + 0x28))(&local_dc,this_00);
          AbyssEngine::AEMath::operator-((Vector *)&local_70,(Vector *)&this[9].field_4);
          fVar11 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_70);
          if (((int)fVar11 < iVar3) &&
             (((int *)this[0xb].m_pPlayer == (int *)0x0 ||
              ((int *)this[0xb].m_pPlayer != this[0xb].field_4)))) {
            this[0xb].m_pPlayer = (int)this_00;
            iVar3 = (int)fVar11;
          }
        }
      }
    }
  }
  piVar7 = (int *)this[0xb].m_pPlayer;
  if ((piVar7 != (int *)0x0) && (piVar7[2] != 0)) {
    __aeabi_memcpy((Matrix *)&local_70,piVar7[1] + 4,0x3c);
    (**(code **)(*piVar7 + 0x28))(aVStack_7c,piVar7);
    AbyssEngine::AEMath::MatrixGetDir(aMStack_a0);
    fVar11 = (float)AbyssEngine::AEMath::VectorNormalize(aAStack_94,(Vector *)aMStack_a0);
    AbyssEngine::AEMath::operator*(aVStack_88,fVar11);
    AbyssEngine::AEMath::operator+((Vector *)&local_dc,aVStack_7c);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&this[6].field_14,(Vector *)&local_dc);
    pMVar6 = (Matrix *)AEGeometry::getMatrix();
    AEGeometry::getMatrix();
    AbyssEngine::AEMath::operator*((Matrix *)&local_dc,pMVar6);
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)&local_70,(Matrix *)&local_dc);
    AbyssEngine::AEMath::MatrixInverseTransformVector((Matrix *)aVStack_7c,(Vector *)&local_70);
    AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_dc,aVStack_7c);
    fVar11 = DAT_000ace0c;
    in_fpscr = in_fpscr & 0xfffffff;
    uVar8 = in_fpscr | (uint)(local_dc < DAT_000ace0c) << 0x1f |
            (uint)(local_dc == DAT_000ace0c) << 0x1e;
    uVar10 = uVar8 | (uint)(NAN(local_dc) || NAN(DAT_000ace0c)) << 0x1c;
    bVar1 = (byte)(uVar8 >> 0x18);
    if ((bool)(bVar1 >> 6 & 1) || bVar1 >> 7 != ((byte)(uVar10 >> 0x1c) & 1)) {
      if (local_dc < DAT_000ace10) {
        iVar3 = -param_1;
        fVar12 = (float)VectorSignedToFloat(this[0xe].m_pPlayer,(byte)(in_fpscr >> 0x16) & 3);
        goto LAB_000acc4c;
      }
      bVar2 = true;
      fVar12 = extraout_s1;
    }
    else {
      fVar12 = (float)VectorSignedToFloat(this[0xe].m_pPlayer,(byte)(uVar10 >> 0x16) & 3);
      in_fpscr = uVar10;
      iVar3 = param_1;
LAB_000acc4c:
      fVar13 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
      bVar2 = false;
      AEGeometry::rotate((fVar13 / (DAT_000ace14 / fVar12)) * DAT_000ace18 * DAT_000ace1c,
                         extraout_s1,DAT_000ace1c);
      AEGeometry::rotate(extraout_s0,extraout_s1_00,extraout_s2);
      fVar12 = extraout_s1_01;
    }
    if (local_d8 <= fVar11) {
      if (local_d8 < DAT_000ace10) {
        if (DAT_000ace20 <= (float)this[0xb].field_1C) goto LAB_000acd18;
        fVar13 = (float)VectorSignedToFloat(this[8].field_14,
                                            (byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
        fVar11 = (float)this[0xb].field_1C + fVar13;
        goto LAB_000accfa;
      }
      if (bVar2) {
        Player::shoot(this->m_pPlayer,2,param_1,param_1 >> 0x1f,0,local_70,uStack_6c,local_68,
                      local_64,local_60,local_5c,local_58,local_54,uStack_50,local_4c,uStack_48,
                      uStack_44,uStack_40,uStack_3c,local_38);
        puVar4 = *(uint **)(DAT_000ace30 + 0xacd9e);
        lVar14 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar4);
        AbyssEngine::Transform::Update(lVar14,SUB41(param_1,0));
        lVar14 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar4);
        AbyssEngine::Transform::Update(lVar14,SUB41(param_1,0));
        this[10].field_20 = 0;
        goto LAB_000acde8;
      }
    }
    else {
      fVar11 = (float)this[0xb].field_1C;
      uVar8 = in_fpscr & 0xfffffff | (uint)(fVar11 < DAT_000ace24) << 0x1f |
              (uint)(fVar11 == DAT_000ace24) << 0x1e;
      bVar1 = (byte)(uVar8 >> 0x18);
      if ((bool)(bVar1 >> 6 & 1) || (bool)(bVar1 >> 7) != (NAN(fVar11) || NAN(DAT_000ace24))) {
LAB_000acd18:
        iVar3 = this[10].field_1C;
        this[0xb].field_4 = (int *)this[0xb].m_pPlayer;
        this[10].field_1C = iVar3 + param_1;
      }
      else {
        fVar13 = (float)VectorSignedToFloat(this[8].field_14,(byte)(uVar8 >> 0x16) & 3);
        fVar11 = fVar11 - fVar13;
LAB_000accfa:
        this[0xb].field_1C = (int)fVar11;
        AEGeometry::rotate(fVar11,fVar12,fVar13);
        this[0xb].field_4 = (int *)0x0;
      }
    }
    iVar3 = this[10].field_20 + param_1;
    this[10].field_20 = iVar3;
    if (iVar3 < 0x1f5) goto LAB_000acde8;
  }
  Player::stopShooting(this->m_pPlayer);
LAB_000acde8:
  if (*piVar9 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000aca78: push {r4,r5,r6,r7,lr}
  000aca7a: add r7,sp,#0xc
  000aca7c: push {r7,r8,r9,r10,r11}
  000aca80: vpush {d8}
  000aca84: sub sp,#0x108
  000aca86: mov r4,r0
  000aca88: ldr r0,[0x000ace2c]
  000aca8a: mov r11,r1
  000aca8c: add r0,pc
  000aca8e: ldr.w r10,[r0,#0x0]
  000aca92: ldr.w r0,[r10,#0x0]
  000aca96: str r0,[sp,#0x104]
  000aca98: ldr.w r0,[r4,#0x184]
  000aca9c: add r0,r1
  000aca9e: movw r1,#0xbb9
  000acaa2: cmp r0,r1
  000acaa4: str.w r0,[r4,#0x184]
  000acaa8: blt 0x000acb5e
  000acaaa: ldr r0,[r4,#0xc]
  000acaac: movs r1,#0x0
  000acaae: str.w r1,[r4,#0x184]
  000acab2: str.w r1,[r4,#0x18c]
  000acab6: blx 0x00071eb4
  000acaba: mov r6,r0
  000acabc: cmp r0,#0x0
  000acabe: beq 0x000acb5e
  000acac0: movw r8,#0xea60
  000acac4: mov.w r9,#0x0
  000acac8: add.w r0,r4,#0x148
  000acacc: str r0,[sp,#0x58]
  000acace: b 0x000acb58
  000acad0: ldr r0,[r6,#0x4]
  000acad2: ldr.w r5,[r0,r9,lsl #0x2]
  000acad6: mov r0,r5
  000acad8: blx 0x00071ec0
  000acadc: cbnz r0,0x000acb54
  000acade: mov r0,r5
  000acae0: blx 0x00071f50
  000acae4: cbnz r0,0x000acb54
  000acae6: ldr r0,[r5,#0x4]
  000acae8: blx 0x00071f14
  000acaec: cbz r0,0x000acb54
  000acaee: mov r0,r5
  000acaf0: blx 0x000726b8
  000acaf4: cbz r0,0x000acb54
  000acaf6: ldrb.w r0,[r5,#0x74]
  000acafa: cbnz r0,0x000acb54
  000acafc: strd r10,r11,[sp,#0x50]
  000acb00: mov r10,r8
  000acb02: ldr r0,[r5,#0x0]
  000acb04: add.w r8,sp,#0x5c
  000acb08: mov r1,r5
  000acb0a: ldr r2,[r0,#0x28]
  000acb0c: mov r0,r8
  000acb0e: blx r2
  000acb10: add.w r11,sp,#0xc8
  000acb14: ldr r1,[sp,#0x58]
  000acb16: mov r2,r8
  000acb18: mov r8,r10
  000acb1a: mov r0,r11
  000acb1c: blx 0x0006ec38
  000acb20: mov r0,r11
  000acb22: blx 0x0006ec44
  000acb26: vmov s0,r0
  000acb2a: vcvt.s32.f32 s0,s0
  000acb2e: vmov r0,s0
  000acb32: cmp r10,r0
  000acb34: ble 0x000acb50
  000acb36: ldr.w r1,[r4,#0x18c]
  000acb3a: ldrd r10,r11,[sp,#0x50]
  000acb3e: cbz r1,0x000acb48
  000acb40: ldr.w r2,[r4,#0x190]
  000acb44: cmp r1,r2
  000acb46: beq 0x000acb54
  000acb48: mov r8,r0
  000acb4a: str.w r5,[r4,#0x18c]
  000acb4e: b 0x000acb54
  000acb50: ldrd r10,r11,[sp,#0x50]
  000acb54: add.w r9,r9,#0x1
  000acb58: ldr r0,[r6,#0x0]
  000acb5a: cmp r9,r0
  000acb5c: bcc 0x000acad0
  000acb5e: ldr.w r5,[r4,#0x18c]
  000acb62: cmp r5,#0x0
  000acb64: beq.w 0x000acde0
  000acb68: ldr r0,[r5,#0x8]
  000acb6a: cmp r0,#0x0
  000acb6c: beq.w 0x000acde0
  000acb70: ldr r0,[r5,#0x4]
  000acb72: add.w r8,sp,#0xc8
  000acb76: movs r2,#0x3c
  000acb78: adds r1,r0,#0x4
  000acb7a: mov r0,r8
  000acb7c: blx 0x0006f1e4
  000acb80: ldr r0,[r5,#0x0]
  000acb82: add.w r9,sp,#0xbc
  000acb86: mov r1,r5
  000acb88: ldr r2,[r0,#0x28]
  000acb8a: mov r0,r9
  000acb8c: blx r2
  000acb8e: add r5,sp,#0x98
  000acb90: mov r1,r8
  000acb92: mov r0,r5
  000acb94: blx 0x0006f4c0
  000acb98: add r6,sp,#0xa4
  000acb9a: mov r1,r5
  000acb9c: mov r0,r6
  000acb9e: blx 0x0006ec80
  000acba2: add r5,sp,#0xb0
  000acba4: ldr r2,[0x000ace08]
  000acba6: mov r1,r6
  000acba8: mov r0,r5
  000acbaa: blx 0x0006ec74
  000acbae: add r6,sp,#0x5c
  000acbb0: mov r1,r9
  000acbb2: mov r2,r5
  000acbb4: mov r0,r6
  000acbb6: blx 0x0006f1cc
  000acbba: add.w r5,r4,#0xec
  000acbbe: mov r1,r6
  000acbc0: mov r0,r5
  000acbc2: blx 0x0006eb3c
  000acbc6: ldr r0,[r4,#0x8]
  000acbc8: blx 0x000721cc
  000acbcc: mov r6,r0
  000acbce: ldr r0,[r4,#0x28]
  000acbd0: blx 0x000721cc
  000acbd4: add.w r9,sp,#0x5c
  000acbd8: mov r2,r0
  000acbda: mov r1,r6
  000acbdc: mov r0,r9
  000acbde: blx 0x0006f82c
  000acbe2: mov r0,r8
  000acbe4: mov r1,r9
  000acbe6: blx 0x0006f148
  000acbea: add r6,sp,#0xbc
  000acbec: mov r1,r8
  000acbee: mov r2,r5
  000acbf0: mov r0,r6
  000acbf2: blx 0x0006f4b4
  000acbf6: add r0,sp,#0x5c
  000acbf8: mov r1,r6
  000acbfa: blx 0x0006ec80
  000acbfe: vldr.32 s16,[pc,#0x20c]
  000acc02: vldr.32 s0,[sp,#0x5c]
  000acc06: vcmpe.f32 s0,s16
  000acc0a: vmrs apsr,fpscr
  000acc0e: ble 0x000acc26
  000acc10: vldr.32 s0,[r4,#0x1f8]
  000acc14: vldr.32 s2,[pc,#0x1fc]
  000acc18: vcvt.f32.s32 s0,s0
  000acc1c: vdiv.f32 s0,s2,s0
  000acc20: vmov s2,r11
  000acc24: b 0x000acc4c
  000acc26: vldr.32 s2,[pc,#0x1e8]
  000acc2a: vcmpe.f32 s0,s2
  000acc2e: vmrs apsr,fpscr
  000acc32: bpl 0x000acc86
  000acc34: vldr.32 s0,[r4,#0x1f8]
  000acc38: rsb.w r0,r11,#0x0
  000acc3c: vldr.32 s2,[pc,#0x1d4]
  000acc40: vcvt.f32.s32 s0,s0
  000acc44: vdiv.f32 s0,s2,s0
  000acc48: vmov s2,r0
  000acc4c: vcvt.f32.s32 s2,s2
  000acc50: ldr.w r0,[r4,#0xdc]
  000acc54: movs r1,#0x0
  000acc56: movs r3,#0x0
  000acc58: movs r6,#0x0
  000acc5a: vdiv.f32 s0,s2,s0
  000acc5e: vldr.32 s2,[pc,#0x1b8]
  000acc62: vmul.f32 s0,s0,s2
  000acc66: vldr.32 s2,[pc,#0x1b4]
  000acc6a: vmul.f32 s0,s0,s2
  000acc6e: vmov r5,s0
  000acc72: mov r2,r5
  000acc74: blx 0x00072160
  000acc78: ldr r0,[r4,#0x28]
  000acc7a: movs r1,#0x0
  000acc7c: mov r2,r5
  000acc7e: movs r3,#0x0
  000acc80: blx 0x00072160
  000acc84: b 0x000acc88
  000acc86: movs r6,#0x1
  000acc88: vldr.32 s0,[sp,#0x60]
  000acc8c: vcmpe.f32 s0,s16
  000acc90: vmrs apsr,fpscr
  000acc94: ble 0x000accc2
  000acc96: vldr.32 s2,[pc,#0x18c]
  000acc9a: vldr.32 s0,[r4,#0x1a8]
  000acc9e: vcmpe.f32 s0,s2
  000acca2: vmrs apsr,fpscr
  000acca6: ble 0x000acd18
  000acca8: vldr.32 s2,[r4,#0x134]
  000accac: vldr.32 s4,[pc,#0x168]
  000accb0: vcvt.f32.s32 s2,s2
  000accb4: vldr.32 s6,[pc,#0x170]
  000accb8: vmul.f32 s4,s2,s4
  000accbc: vsub.f32 s0,s0,s2
  000accc0: b 0x000accfa
  000accc2: vldr.32 s2,[pc,#0x14c]
  000accc6: vcmpe.f32 s0,s2
  000accca: vmrs apsr,fpscr
  000accce: bpl 0x000acd2c
  000accd0: vldr.32 s2,[pc,#0x14c]
  000accd4: vldr.32 s0,[r4,#0x1a8]
  000accd8: vcmpe.f32 s0,s2
  000accdc: vmrs apsr,fpscr
  000acce0: bpl 0x000acd18
  000acce2: vldr.32 s2,[r4,#0x134]
  000acce6: vldr.32 s4,[pc,#0x130]
  000accea: vcvt.f32.s32 s2,s2
  000accee: vldr.32 s6,[pc,#0x12c]
  000accf2: vmul.f32 s4,s2,s4
  000accf6: vadd.f32 s0,s0,s2
  000accfa: vmul.f32 s4,s4,s6
  000accfe: vmov r1,s4
  000acd02: ldr r0,[r4,#0x28]
  000acd04: movs r2,#0x0
  000acd06: movs r3,#0x0
  000acd08: vstr.32 s0,[r4,#0x1a8]
  000acd0c: movs r5,#0x0
  000acd0e: blx 0x00072160
  000acd12: str.w r5,[r4,#0x190]
  000acd16: b 0x000acdd0
  000acd18: ldr.w r0,[r4,#0x184]
  000acd1c: ldr.w r1,[r4,#0x18c]
  000acd20: str.w r1,[r4,#0x190]
  000acd24: add r0,r11
  000acd26: str.w r0,[r4,#0x184]
  000acd2a: b 0x000acdd0
  000acd2c: cmp r6,#0x0
  000acd2e: beq 0x000acdd0
  000acd30: ldrd r9,r0,[sp,#0xc8]
  000acd34: add.w lr,sp,#0xec
  000acd38: str r0,[sp,#0x58]
  000acd3a: ldr r0,[sp,#0xd0]
  000acd3c: str r0,[sp,#0x54]
  000acd3e: ldr r0,[sp,#0xd4]
  000acd40: str r0,[sp,#0x50]
  000acd42: ldr r0,[sp,#0xd8]
  000acd44: str r0,[sp,#0x4c]
  000acd46: ldr r0,[sp,#0xdc]
  000acd48: str r0,[sp,#0x48]
  000acd4a: ldr r0,[sp,#0xe0]
  000acd4c: str r0,[sp,#0x44]
  000acd4e: ldrd r1,r8,[sp,#0xe4]
  000acd52: ldm.w lr,{r0,r2,r3,r12,lr}
  000acd56: ldr r5,[sp,#0x100]
  000acd58: ldr r6,[r4,#0x0]
  000acd5a: strd r1,r8,[sp,#0x20]
  000acd5e: add r1,sp,#0x28
  000acd60: mov.w r8,#0x0
  000acd64: stm r1,{r0,r2,r3,r12,lr}
  000acd68: movs r1,#0x2
  000acd6a: mov r2,r11
  000acd6c: str r5,[sp,#0x3c]
  000acd6e: asr.w r5,r11, asr #0x1f
  000acd72: strd r8,r9,[sp,#0x0]
  000acd76: mov r3,r5
  000acd78: ldr r0,[sp,#0x58]
  000acd7a: str r0,[sp,#0x8]
  000acd7c: ldr r0,[sp,#0x54]
  000acd7e: str r0,[sp,#0xc]
  000acd80: ldr r0,[sp,#0x50]
  000acd82: str r0,[sp,#0x10]
  000acd84: ldr r0,[sp,#0x4c]
  000acd86: str r0,[sp,#0x14]
  000acd88: ldr r0,[sp,#0x48]
  000acd8a: str r0,[sp,#0x18]
  000acd8c: ldr r0,[sp,#0x44]
  000acd8e: str r0,[sp,#0x1c]
  000acd90: mov r0,r6
  000acd92: blx 0x000724fc
  000acd96: ldr r0,[0x000ace30]
  000acd98: ldr r1,[r4,#0x28]
  000acd9a: add r0,pc
  000acd9c: ldr r6,[r0,#0x0]
  000acd9e: ldr r1,[r1,#0xc]
  000acda0: ldr r0,[r6,#0x0]
  000acda2: blx 0x00072088
  000acda6: mov r2,r11
  000acda8: mov r3,r5
  000acdaa: str.w r8,[sp,#0x0]
  000acdae: blx 0x0006f7cc
  000acdb2: ldr.w r1,[r4,#0xdc]
  000acdb6: ldr r0,[r6,#0x0]
  000acdb8: ldr r1,[r1,#0xc]
  000acdba: blx 0x00072088
  000acdbe: mov r2,r11
  000acdc0: mov r3,r5
  000acdc2: str.w r8,[sp,#0x0]
  000acdc6: blx 0x0006f7cc
  000acdca: str.w r8,[r4,#0x188]
  000acdce: b 0x000acde8
  000acdd0: ldr.w r0,[r4,#0x188]
  000acdd4: add r0,r11
  000acdd6: str.w r0,[r4,#0x188]
  000acdda: cmp.w r0,#0x1f4
  000acdde: ble 0x000acde8
  000acde0: ldr r0,[r4,#0x0]
  000acde2: movs r1,#0x2
  000acde4: blx 0x00072484
  000acde8: ldr r0,[sp,#0x104]
  000acdea: ldr.w r1,[r10,#0x0]
  000acdee: subs r0,r1,r0
  000acdf0: itttt eq
  000acdf2: add.eq sp,#0x108
  000acdf4: vpop.eq {d8}
  000acdf8: add.eq sp,#0x4
  000acdfa: pop.eq.w {r8,r9,r10,r11}
  000acdfe: it eq
  000ace00: pop.eq {r4,r5,r6,r7,pc}
  000ace02: blx 0x0006e824
```
