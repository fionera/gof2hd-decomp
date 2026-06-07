# PlayerEgo::handleShip
elf 0x9bdf0 body 832
Sig: undefined __thiscall handleShip(PlayerEgo * this, int param_1)

## decompile
```c

/* PlayerEgo::handleShip(int) */

void __thiscall PlayerEgo::handleShip(PlayerEgo *this,int param_1)

{
  uint uVar1;
  byte bVar2;
  int *piVar3;
  Event *pEVar4;
  undefined4 uVar5;
  Matrix *pMVar6;
  FModSound *pFVar7;
  code *pcVar8;
  undefined4 *puVar9;
  code *pcVar10;
  uint *puVar11;
  uint in_fpscr;
  float fVar12;
  float extraout_s1;
  float extraout_s1_00;
  float fVar13;
  Matrix aMStack_11c [12];
  undefined4 local_110;
  undefined4 uStack_10c;
  undefined4 uStack_108;
  undefined4 local_104;
  undefined4 uStack_100;
  undefined4 local_fc;
  float local_f8;
  float local_f4;
  float local_f0;
  float local_ec;
  float local_e8;
  float local_e4;
  undefined4 local_b0 [5];
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined4 local_78;
  undefined4 local_70;
  undefined4 local_6c;
  float local_68;
  float local_64;
  undefined4 local_60;
  undefined4 local_5c;
  float local_58;
  float local_54;
  undefined4 local_50;
  undefined4 local_4c;
  float local_48;
  float local_44;
  int local_34;
  
  piVar3 = *(int **)(DAT_000ac15c + 0xabe0a);
  puVar9 = *(undefined4 **)(DAT_000ac160 + 0xabe0c);
  local_34 = *piVar3;
  pFVar7 = (FModSound *)*puVar9;
  pEVar4 = (Event *)Player::GetEngineEvent((Player *)this->m_pPlayer);
  in_fpscr = in_fpscr & 0xfffffff;
  FModSound::setParamValue(pFVar7,pEVar4,0,(float)this[0x11].field_4);
  pFVar7 = (FModSound *)*puVar9;
  pEVar4 = (Event *)Player::GetEngineEvent((Player *)this->m_pPlayer);
  FModSound::setParamValue(pFVar7,pEVar4,1,(float)this[0x11].field_4 * DAT_000ac140 + 0.5);
  puVar11 = *(uint **)(DAT_000ac164 + 0xabe96);
  uVar5 = AbyssEngine::PaintCanvas::TransformGetLocal(*puVar11);
  __aeabi_memcpy((Matrix *)&local_70,uVar5,0x3c);
  fVar12 = DAT_000ac144;
  uStack_80 = DAT_000ac138;
  uStack_88 = DAT_000ac130;
  uStack_94 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_90 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_8c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar9 = (undefined4 *)((uint)local_b0 | 4);
  *puVar9 = 0;
  puVar9[1] = uStack_94;
  puVar9[2] = uStack_90;
  puVar9[3] = uStack_8c;
  local_98 = 0;
  local_b0[0] = 0x3f800000;
  local_9c = 0x3f800000;
  local_78 = 0x3f800000;
  fVar13 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  uVar5 = 0;
  AbyssEngine::AEMath::MatrixSetRotation
            ((Matrix *)&local_ec,
             (float)this[0x11].field_18 * fVar12 * DAT_000ac148 * fVar13 * DAT_000ac14c,extraout_s1,
             (float)this[0x11].field_14 * fVar12 * DAT_000ac148 * fVar13 * DAT_000ac14c);
  this[0x11].field_1C = (int)this[0x11].field_18;
  AbyssEngine::AEMath::Matrix::operator*=((Matrix *)&local_70,(Matrix *)local_b0);
  if ((char)this[0x15].m_pPlayer != '\0') {
    AbyssEngine::AEMath::Matrix::operator*=((Matrix *)&local_70,(Matrix *)(this + 0x13));
  }
  fVar12 = (float)this[5].field_8 * fVar13 * (float)this[5].field_4;
  local_f4 = local_58;
  local_f8 = local_68;
  local_f0 = local_48;
  pcVar8 = *(code **)(DAT_000ac168 + 0xabf84);
  local_64 = local_64 + local_68 * fVar12;
  local_54 = local_54 + fVar12 * local_58;
  local_44 = local_44 + fVar12 * local_48;
  local_104 = local_6c;
  uStack_100 = local_5c;
  local_fc = local_4c;
  (*pcVar8)(&local_ec,&local_f8,local_5c,local_4c,uVar5);
  pcVar10 = *(code **)(DAT_000ac16c + 0xabfbe);
  (*pcVar10)(&local_f8,&local_ec);
  (*pcVar8)(&local_ec,(Vector *)&local_104);
  (*pcVar10)((Vector *)&local_104,&local_ec);
  AbyssEngine::AEMath::VectorCross((Vector *)&local_110,(Vector *)&local_104);
  (*pcVar8)(&local_ec,(Vector *)&local_110);
  (*pcVar10)((Vector *)&local_110,&local_ec);
  local_60 = uStack_10c;
  local_70 = local_110;
  local_50 = uStack_108;
  local_4c = local_fc;
  local_6c = local_104;
  local_68 = local_f8;
  local_5c = uStack_100;
  local_58 = local_f4;
  local_48 = local_f0;
  fVar13 = (float)this[0x18].field_1C;
  fVar12 = -fVar13;
  if (0.0 < fVar13) {
    fVar12 = fVar13;
  }
  uVar1 = in_fpscr & 0xfffffff | (uint)(fVar12 < DAT_000ac150) << 0x1f |
          (uint)(fVar12 == DAT_000ac150) << 0x1e;
  bVar2 = (byte)(uVar1 >> 0x18);
  if ((bool)(bVar2 >> 6 & 1) || (bool)(bVar2 >> 7) != (NAN(fVar12) || NAN(DAT_000ac150))) {
    this[0x18].field_20 = DAT_000ac154;
  }
  else {
    local_ec = (float)VectorSignedToFloat(this[8].field_14,(byte)(uVar1 >> 0x16) & 3);
    local_e8 = 0.0;
    local_e4 = 0.0;
    local_ec = fVar13 * local_ec;
    AbyssEngine::AEMath::MatrixRotateVector(aMStack_11c,(Vector *)&local_70);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&local_ec,(Vector *)aMStack_11c);
    local_64 = local_ec + local_64;
    local_54 = local_e8 + local_54;
    local_44 = local_e4 + local_44;
    fVar12 = (float)this[0x18].field_1C * DAT_000ac158;
    this[0x18].field_1C = (int)fVar12;
    TargetFollowCamera::translateNoUpdate
              ((TargetFollowCamera *)this[3].field_1C,fVar12,extraout_s1_00,local_54);
  }
  AbyssEngine::PaintCanvas::TransformSetLocal(*puVar11,(Matrix *)this->field_8[3]);
  this[0x10].field_18 = (int *)0x0;
  this[0x10].field_1C = 0;
  this[7].field_4 = (int *)0x0;
  this[7].field_8 = (int *)0x0;
  this[0x11].m_pLevel = (SingleLevel *)0x0;
  this[0x11].field_4 = (int *)0x0;
  pMVar6 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar11);
  AbyssEngine::PaintCanvas::TransformGetLocal(*puVar11);
  AbyssEngine::AEMath::operator*((Matrix *)&local_ec,pMVar6);
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this->m_pPlayer + 4),(Matrix *)&local_ec);
  if (*piVar3 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000abdf0: push {r4,r5,r6,r7,lr}
  000abdf2: add r7,sp,#0xc
  000abdf4: push {r7,r8,r9,r10,r11}
  000abdf8: vpush {d8}
  000abdfc: sub sp,#0xf8
  000abdfe: mov r4,r0
  000abe00: ldr r0,[0x000ac15c]
  000abe02: mov r8,r1
  000abe04: ldr r1,[0x000ac160]
  000abe06: add r0,pc
  000abe08: add r1,pc
  000abe0a: ldr r0,[r0,#0x0]
  000abe0c: ldr r6,[r1,#0x0]
  000abe0e: str r0,[sp,#0x8]
  000abe10: ldr r0,[r0,#0x0]
  000abe12: ldr r5,[r6,#0x0]
  000abe14: str r0,[sp,#0xf4]
  000abe16: ldr r0,[r4,#0x0]
  000abe18: blx 0x000725f8
  000abe1c: vldr.32 s2,[r4,#0x270]
  000abe20: mov r1,r0
  000abe22: vldr.32 s0,[r4,#0x268]
  000abe26: mov r0,r5
  000abe28: vcmpe.f32 s2,#0
  000abe2c: movs r2,#0x0
  000abe2e: vmrs apsr,fpscr
  000abe32: vneg.f32 s4,s2
  000abe36: vcmpe.f32 s0,#0
  000abe3a: mov.w r11,#0x0
  000abe3e: vneg.f32 s6,s0
  000abe42: it gt
  000abe44: vmov.gt.f32 s4,s2
  000abe48: vmrs apsr,fpscr
  000abe4c: it gt
  000abe4e: vmov.gt.f32 s6,s0
  000abe52: vcmpe.f32 s4,s6
  000abe56: vmrs apsr,fpscr
  000abe5a: it gt
  000abe5c: vmov.gt.f32 s6,s4
  000abe60: vmov r3,s6
  000abe64: blx 0x00072604
  000abe68: ldr r0,[r4,#0x0]
  000abe6a: ldr r5,[r6,#0x0]
  000abe6c: blx 0x000725f8
  000abe70: vmov.f32 s0,0x3f000000
  000abe74: vldr.32 s2,[pc,#0x2c8]
  000abe78: vldr.32 s4,[r4,#0x268]
  000abe7c: mov r1,r0
  000abe7e: mov r0,r5
  000abe80: movs r2,#0x1
  000abe82: vmla.f32 s0,s4,s2
  000abe86: vmov r3,s0
  000abe8a: blx 0x00072604
  000abe8e: ldr r0,[0x000ac164]
  000abe90: ldr r1,[r4,#0x8]
  000abe92: add r0,pc
  000abe94: ldr.w r10,[r0,#0x0]
  000abe98: ldr r1,[r1,#0xc]
  000abe9a: ldr.w r0,[r10,#0x0]
  000abe9e: blx 0x000720c4
  000abea2: add r6,sp,#0xb8
  000abea4: mov r1,r0
  000abea6: movs r2,#0x3c
  000abea8: mov r0,r6
  000abeaa: blx 0x0006f1e4
  000abeae: adr r0,[0xac130]
  000abeb0: add r5,sp,#0x78
  000abeb2: vmov.i32 q8,#0x0
  000abeb6: vld1.64 {d18,d19},[r0]
  000abeba: orr r0,r5,#0x4
  000abebe: vmov s6,r8
  000abec2: vldr.32 s0,[pc,#0x280]
  000abec6: mov r1,r5
  000abec8: vst1.32 {d16,d17},[r0]
  000abecc: add.w r0,r5,#0x18
  000abed0: vst1.64 {d16,d17},[r0]
  000abed4: add.w r0,r5,#0x28
  000abed8: vst1.64 {d18,d19},[r0]
  000abedc: mov.w r0,#0x3f800000
  000abee0: str r0,[sp,#0x78]
  000abee2: str r0,[sp,#0x8c]
  000abee4: str r0,[sp,#0xb0]
  000abee6: add r0,sp,#0x3c
  000abee8: vldr.32 s2,[r4,#0x278]
  000abeec: vldr.32 s4,[r4,#0x27c]
  000abef0: vcvt.f32.s32 s16,s6
  000abef4: vmul.f32 s2,s2,s0
  000abef8: str.w r11,[sp,#0x0]
  000abefc: vmul.f32 s0,s4,s0
  000abf00: vldr.32 s4,[pc,#0x244]
  000abf04: vmul.f32 s2,s2,s4
  000abf08: vmul.f32 s0,s0,s4
  000abf0c: vldr.32 s4,[pc,#0x23c]
  000abf10: vmul.f32 s2,s2,s16
  000abf14: vmul.f32 s0,s0,s16
  000abf18: vmul.f32 s2,s2,s4
  000abf1c: vmul.f32 s0,s0,s4
  000abf20: vmov r2,s2
  000abf24: vmov r3,s0
  000abf28: blx 0x00072094
  000abf2c: ldr.w r0,[r4,#0x27c]
  000abf30: mov r1,r5
  000abf32: str.w r0,[r4,#0x280]
  000abf36: mov r0,r6
  000abf38: blx 0x0006f4a8
  000abf3c: ldrb.w r0,[r4,#0x2f4]
  000abf40: cbz r0,0x000abf4c
  000abf42: add.w r1,r4,#0x2ac
  000abf46: add r0,sp,#0xb8
  000abf48: blx 0x0006f4a8
  000abf4c: vldr.32 s2,[r4,#0xbc]
  000abf50: add r6,sp,#0x3c
  000abf52: vldr.32 s0,[r4,#0xb8]
  000abf56: add.w r8,sp,#0x30
  000abf5a: vmul.f32 s2,s2,s16
  000abf5e: vldr.32 s6,[sp,#0xd0]
  000abf62: vldr.32 s10,[sp,#0xe0]
  000abf66: vldr.32 s4,[sp,#0xc4]
  000abf6a: vldr.32 s8,[sp,#0xd4]
  000abf6e: vldr.32 s12,[sp,#0xe4]
  000abf72: vmul.f32 s0,s2,s0
  000abf76: vldr.32 s2,[sp,#0xc0]
  000abf7a: ldr r0,[0x000ac168]
  000abf7c: vstr.32 s6,[sp,#0x34]
  000abf80: add r0,pc
  000abf82: vstr.32 s2,[sp,#0x30]
  000abf86: vstr.32 s10,[sp,#0x38]
  000abf8a: ldr r5,[r0,#0x0]
  000abf8c: mov r0,r6
  000abf8e: vmla.f32 s4,s2,s0
  000abf92: vmla.f32 s8,s0,s6
  000abf96: vmla.f32 s12,s0,s10
  000abf9a: vstr.32 s4,[sp,#0xc4]
  000abf9e: vstr.32 s8,[sp,#0xd4]
  000abfa2: vstr.32 s12,[sp,#0xe4]
  000abfa6: ldr r2,[sp,#0xcc]
  000abfa8: ldr r1,[sp,#0xbc]
  000abfaa: ldr r3,[sp,#0xdc]
  000abfac: strd r1,r2,[sp,#0x24]
  000abfb0: mov r1,r8
  000abfb2: str r3,[sp,#0x2c]
  000abfb4: blx r5
  000abfb6: ldr r0,[0x000ac16c]
  000abfb8: mov r1,r6
  000abfba: add r0,pc
  000abfbc: ldr.w r9,[r0,#0x0]
  000abfc0: mov r0,r8
  000abfc2: blx r9
  000abfc4: add.w r11,sp,#0x3c
  000abfc8: add r6,sp,#0x24
  000abfca: mov r0,r11
  000abfcc: mov r1,r6
  000abfce: blx r5
  000abfd0: mov r0,r6
  000abfd2: mov r1,r11
  000abfd4: blx r9
  000abfd6: add.w r11,sp,#0x18
  000abfda: mov r1,r6
  000abfdc: mov r2,r8
  000abfde: mov r0,r11
  000abfe0: blx 0x0006ec8c
  000abfe4: add r6,sp,#0x3c
  000abfe6: mov r1,r11
  000abfe8: mov r0,r6
  000abfea: blx r5
  000abfec: mov r0,r11
  000abfee: mov r1,r6
  000abff0: blx r9
  000abff2: add.w r12,sp,#0x18
  000abff6: add r2,sp,#0x30
  000abff8: vldr.32 s4,[pc,#0x154]
  000abffc: ldm.w r12,{r0,r1,r12}
  000ac000: ldrd r3,r6,[sp,#0x24]
  000ac004: str r1,[sp,#0xc8]
  000ac006: str r0,[sp,#0xb8]
  000ac008: ldr r5,[sp,#0x2c]
  000ac00a: ldmia r2,{r0,r1,r2}
  000ac00c: strd r12,r5,[sp,#0xd8]
  000ac010: strd r3,r0,[sp,#0xbc]
  000ac014: strd r6,r1,[sp,#0xcc]
  000ac018: str r2,[sp,#0xe0]
  000ac01a: vldr.32 s0,[r4,#0x37c]
  000ac01e: vcmpe.f32 s0,#0
  000ac022: vmrs apsr,fpscr
  000ac026: vneg.f32 s2,s0
  000ac02a: it gt
  000ac02c: vmov.gt.f32 s2,s0
  000ac030: vcmpe.f32 s2,s4
  000ac034: vmrs apsr,fpscr
  000ac038: ble 0x000ac0bc
  000ac03a: vldr.32 s2,[r4,#0x134]
  000ac03e: add r6,sp,#0x3c
  000ac040: add r5,sp,#0xc
  000ac042: movs r0,#0x0
  000ac044: vcvt.f32.s32 s2,s2
  000ac048: add r1,sp,#0xb8
  000ac04a: strd r0,r0,[sp,#0x40]
  000ac04e: mov r0,r5
  000ac050: mov r2,r6
  000ac052: vmul.f32 s0,s0,s2
  000ac056: vstr.32 s0,[sp,#0x3c]
  000ac05a: blx 0x0006f694
  000ac05e: mov r0,r6
  000ac060: mov r1,r5
  000ac062: blx 0x0006eb3c
  000ac066: vldr.32 s6,[sp,#0x3c]
  000ac06a: vldr.32 s8,[sp,#0x40]
  000ac06e: vldr.32 s10,[sp,#0x44]
  000ac072: vmov r1,s6
  000ac076: vmov r2,s8
  000ac07a: vldr.32 s0,[sp,#0xc4]
  000ac07e: vmov r3,s10
  000ac082: vldr.32 s2,[sp,#0xd4]
  000ac086: vldr.32 s4,[sp,#0xe4]
  000ac08a: vadd.f32 s0,s6,s0
  000ac08e: vadd.f32 s2,s8,s2
  000ac092: vldr.32 s6,[pc,#0xc4]
  000ac096: vadd.f32 s4,s10,s4
  000ac09a: vstr.32 s0,[sp,#0xc4]
  000ac09e: vstr.32 s2,[sp,#0xd4]
  000ac0a2: vstr.32 s4,[sp,#0xe4]
  000ac0a6: vldr.32 s0,[r4,#0x37c]
  000ac0aa: ldr.w r0,[r4,#0x88]
  000ac0ae: vmul.f32 s0,s0,s6
  000ac0b2: vstr.32 s0,[r4,#0x37c]
  000ac0b6: blx 0x00072610
  000ac0ba: b 0x000ac0c2
  000ac0bc: ldr r0,[0x000ac154]
  000ac0be: str.w r0,[r4,#0x380]
  000ac0c2: ldr r0,[r4,#0x8]
  000ac0c4: add r2,sp,#0xb8
  000ac0c6: ldr r1,[r0,#0xc]
  000ac0c8: ldr.w r0,[r10,#0x0]
  000ac0cc: blx 0x000721c0
  000ac0d0: movs r0,#0x0
  000ac0d2: strd r0,r0,[r4,#0x258]
  000ac0d6: strd r0,r0,[r4,#0x100]
  000ac0da: str.w r0,[r4,#0x270]
  000ac0de: str.w r0,[r4,#0x268]
  000ac0e2: ldr r1,[r4,#0x8]
  000ac0e4: ldr.w r0,[r10,#0x0]
  000ac0e8: ldr r1,[r1,#0xc]
  000ac0ea: blx 0x000720c4
  000ac0ee: ldr r1,[r4,#0x4]
  000ac0f0: mov r5,r0
  000ac0f2: ldr.w r0,[r10,#0x0]
  000ac0f6: ldr r1,[r1,#0xc]
  000ac0f8: blx 0x000720c4
  000ac0fc: add r6,sp,#0x3c
  000ac0fe: mov r2,r0
  000ac100: mov r1,r5
  000ac102: mov r0,r6
  000ac104: blx 0x0006f82c
  000ac108: ldr r0,[r4,#0x0]
  000ac10a: mov r1,r6
  000ac10c: adds r0,#0x4
  000ac10e: blx 0x0006f148
  000ac112: ldr r0,[sp,#0xf4]
  000ac114: ldr r1,[sp,#0x8]
  000ac116: ldr r1,[r1,#0x0]
  000ac118: subs r0,r1,r0
  000ac11a: itttt eq
  000ac11c: add.eq sp,#0xf8
  000ac11e: vpop.eq {d8}
  000ac122: add.eq sp,#0x4
  000ac124: pop.eq.w {r8,r9,r10,r11}
  000ac128: it eq
  000ac12a: pop.eq {r4,r5,r6,r7,pc}
  000ac12c: blx 0x0006e824
```
