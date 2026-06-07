# MiningGame::update
elf 0x11ec00 body 1114
Sig: undefined __thiscall update(MiningGame * this, int param_1)

## decompile
```c

/* MiningGame::update(int) */

undefined4 __thiscall MiningGame::update(MiningGame *this,int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int extraout_r1;
  Hud *this_00;
  code *pcVar7;
  int *piVar8;
  undefined4 *puVar9;
  int *piVar10;
  int iVar11;
  uint in_fpscr;
  uint uVar12;
  float fVar13;
  float extraout_s0;
  undefined4 extraout_s1;
  undefined8 uVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  undefined8 unaff_d9;
  
  iVar2 = *(int *)(this + 0xcc) + param_1;
  if (1999 < iVar2) {
    iVar2 = 0;
  }
  *(int *)(this + 0xcc) = iVar2;
  iVar2 = isInCurrentLayer(this);
  iVar3 = *(int *)(this + 0x6c);
  *(int *)(this + 0x6c) = iVar3 + param_1;
  if (0x9c4 < iVar3 + param_1) {
    puVar9 = *(undefined4 **)(DAT_0012ef58 + 0x12ec40);
    pcVar7 = *(code **)(DAT_0012ef5c + 0x12ec42);
    iVar3 = (*pcVar7)(*puVar9,2000);
    *(int *)(this + 0x6c) = iVar3 + 500;
    iVar3 = (*pcVar7)(*puVar9,7);
    iVar4 = (*pcVar7)(*puVar9,2);
    iVar5 = -1;
    if (iVar4 == 0) {
      iVar5 = 1;
    }
    iVar11 = -1;
    fVar13 = (float)VectorSignedToFloat((iVar3 + 5) * iVar5,(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(this + 8) = (fVar13 / 10.0) / *(float *)(this + 0x2c);
    iVar3 = (*pcVar7)(*puVar9,7);
    iVar4 = (*pcVar7)(*puVar9,2);
    if (iVar4 == 0) {
      iVar11 = 1;
    }
    fVar13 = (float)VectorSignedToFloat(iVar11 * (iVar3 + 5),(byte)(in_fpscr >> 0x16) & 3);
    fVar13 = (fVar13 / 10.0) / *(float *)(this + 0x2c);
    *(float *)(this + 0xc) = fVar13;
    if ((this[0x80] != (MiningGame)0x0) && (*(int *)(this + 0x78) == *(int *)(this + 0x7c) + -1)) {
      fVar13 = fVar13 * DAT_0012f064;
      *(float *)(this + 8) = *(float *)(this + 8) * DAT_0012f064;
      *(float *)(this + 0xc) = fVar13;
    }
  }
  if ((this[0x84] == (MiningGame)0x0) && (iVar3 = isInCurrentLayer(this), iVar3 == 0)) {
    fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x58),(byte)(in_fpscr >> 0x16) & 3);
    fVar13 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x5c),(byte)(in_fpscr >> 0x16) & 3);
    fVar13 = (fVar13 - *(float *)(this + 0x14)) * DAT_0012f068;
    *(float *)(this + 8) = (fVar17 - *(float *)(this + 0x10)) * DAT_0012f068;
    *(float *)(this + 0xc) = fVar13;
  }
  piVar10 = *(int **)(DAT_0012f088 + 0x12ed3e);
  fVar17 = *(float *)(*piVar10 + 0xe4);
  fVar13 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  *(float *)(this + 0x10) =
       *(float *)(this + 0x10) + ((*(float *)this + *(float *)(this + 8)) / fVar17) * fVar13;
  *(float *)(this + 0x14) =
       *(float *)(this + 0x14) + ((*(float *)(this + 4) + *(float *)(this + 0xc)) / fVar17) * fVar13
  ;
  MarqueeImage::update(*(MarqueeImage **)(this + 0x8c),param_1);
  MarqueeImage::update(*(MarqueeImage **)(this + 0x90),param_1);
  iVar4 = *(int *)(DAT_0012f08c + 0x12eda6);
  piVar8 = *(int **)(DAT_0012f090 + 0x12edc0);
  FModSound::setParamValue
            ((FModSound *)*piVar8,0,1,
             ((*(float *)(iVar4 + *(int *)(this + 0x78) * 4) + -5.0) / DAT_0012f06c) * 3.0);
  iVar3 = isInCurrentLayer(this);
  if (iVar3 == 0) {
    if (iVar2 != 0) {
      fVar13 = (float)FModSound::stop(*piVar8);
      FModSound::play(*piVar8,(Vector *)0x3,(Vector *)0x0,fVar13);
    }
    iVar2 = *(int *)(this + 0x20);
    *(int *)(this + 0x20) = iVar2 + param_1;
    if (0x9c4 < iVar2 + param_1) {
      puVar9 = (undefined4 *)(DAT_0012f094 + 0x12eede);
      *(undefined4 *)(this + 0x20) = 0x9c4;
      *(undefined4 *)(this + 0x24) = 0;
      piVar8 = (int *)*puVar9;
      this[0x82] = (MiningGame)0x1;
      *(undefined4 *)(*piVar8 + 0x124) = 0;
      return 0;
    }
    *(undefined4 *)(this + 100) = 0x3f800000;
  }
  else {
    if (iVar2 == 0) {
      FModSound::play(*piVar8,(Vector *)0x1,(Vector *)0x0,extraout_s0);
      FModSound::stop(*piVar8);
    }
    fVar17 = DAT_0012f070;
    fVar15 = *(float *)(this + 0x68) +
             (fVar13 / DAT_0012f070) *
             *(float *)(*piVar10 + 0xe0) * *(float *)(iVar4 + *(int *)(this + 0x78) * 4) * 3.0;
    *(float *)(this + 0x68) = fVar15;
    uVar1 = in_fpscr & 0xfffffff | (uint)(fVar15 < 1.0) << 0x1f;
    uVar12 = uVar1 | (uint)NAN(fVar15) << 0x1c;
    if ((byte)(uVar1 >> 0x1f) == ((byte)(uVar12 >> 0x1c) & 1)) {
      Sprite::nextFrame(*(Sprite **)(this + 0x94));
      *(undefined4 *)(this + 0x68) = 0;
    }
    MarqueeImage::update(*(MarqueeImage **)(this + 0x88),param_1);
    iVar2 = *(int *)(this + 0x78) + 1;
    fVar16 = (float)VectorSignedToFloat(iVar2,(byte)(uVar12 >> 0x16) & 3);
    fVar15 = *(float *)(this + 0x24);
    fVar16 = fVar15 + ((*(float *)(this + 0x28) * (DAT_0012f078 + (fVar16 / 7.0) * DAT_0012f074)) /
                      fVar17) * fVar13;
    *(float *)(this + 0x24) = fVar16;
    fVar17 = DAT_0012f07c;
    if ((int)fVar15 < (int)fVar16) {
      *(undefined4 *)(this + 100) = 0;
    }
    else {
      fVar17 = *(float *)(this + 100);
    }
    iVar3 = *(int *)(this + 0x74);
    fVar13 = fVar13 / DAT_0012f080;
    *(int *)(this + 0x74) = iVar3 + param_1;
    uVar14 = FloatVectorMin(CONCAT44(extraout_s1,fVar13 + fVar17),
                            CONCAT44((int)((ulonglong)unaff_d9 >> 0x20),0x3f800000),2,0x20);
    *(int *)(this + 100) = (int)uVar14;
    if (6000 < iVar3 + param_1) {
      *(undefined4 *)(this + 0x74) = 0;
      *(int *)(this + 0x78) = iVar2;
      if (*(int *)(this + 0x7c) <= iVar2) {
        puVar9 = (undefined4 *)(DAT_0012f098 + 0x12ef6a);
        this[0x81] = (MiningGame)0x1;
        puVar9 = (undefined4 *)*puVar9;
        this[0x83] = (MiningGame)(*(int *)(this + 0x7c) == 7);
        iVar2 = Achievements::hasMedal((Achievements *)*puVar9,0x26,1);
        if (iVar2 == 0) {
          piVar8 = *(int **)(DAT_0012f09c + 0x12ef8e);
          iVar2 = *(int *)(*piVar8 + 0x124) + 1;
          *(int *)(*piVar8 + 0x124) = iVar2;
          uVar6 = Achievements::getValue((Achievements *)*puVar9,0x26,1);
          fVar13 = DAT_0012f084;
          fVar17 = (float)VectorSignedToFloat(iVar2,(byte)(uVar12 >> 0x16) & 3);
          fVar15 = (float)VectorSignedToFloat(uVar6,(byte)(uVar12 >> 0x16) & 3);
          __aeabi_idivmod((int)((fVar17 / fVar15) * DAT_0012f084),10);
          if (extraout_r1 == 0) {
            fVar15 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar8 + 0x124),
                                                (byte)(uVar12 >> 0x16) & 3);
            uVar6 = Achievements::getValue((Achievements *)*puVar9,0x26,1);
            fVar17 = (float)VectorSignedToFloat(uVar6,(byte)(uVar12 >> 0x16) & 3);
            if (0x1d < (int)((fVar15 / fVar17) * fVar13)) {
              this_00 = *(Hud **)(this + 0xd0);
              fVar15 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar8 + 0x124),
                                                  (byte)(uVar12 >> 0x16) & 3);
              uVar6 = Achievements::getValue((Achievements *)*puVar9,0x26,1);
              fVar17 = (float)VectorSignedToFloat(uVar6,(byte)(uVar12 >> 0x16) & 3);
              Hud::hudEventMedal(this_00,0x26,(int)((fVar15 / fVar17) * fVar13));
            }
          }
          iVar3 = *(int *)(*piVar8 + 0x124);
          iVar2 = Achievements::getValue((Achievements *)*puVar9,0x26,1);
          if (iVar2 <= iVar3) {
            *(undefined1 *)(*piVar8 + 0x128) = 1;
          }
        }
        return 0;
      }
      MarqueeImage::setSpeed
                (*(MarqueeImage **)(this + 0x88),
                 *(float *)(*piVar10 + 0xe0) * *(float *)(iVar4 + iVar2 * 4));
    }
  }
  return 1;
}

```

## target disasm
```
  0012ec00: push {r4,r5,r6,r7,lr}
  0012ec02: add r7,sp,#0xc
  0012ec04: push {r7,r8,r9,r10,r11}
  0012ec08: vpush {d7,d8,d9,d10}
  0012ec0c: mov r4,r0
  0012ec0e: ldr.w r0,[r0,#0xcc]
  0012ec12: mov r10,r1
  0012ec14: add r0,r1
  0012ec16: cmp.w r0,#0x7d0
  0012ec1a: it ge
  0012ec1c: mov.ge r0,#0x0
  0012ec1e: str.w r0,[r4,#0xcc]
  0012ec22: mov r0,r4
  0012ec24: blx 0x00076288
  0012ec28: mov r8,r0
  0012ec2a: ldr r0,[r4,#0x6c]
  0012ec2c: movw r1,#0x9c5
  0012ec30: add r0,r10
  0012ec32: str r0,[r4,#0x6c]
  0012ec34: cmp r0,r1
  0012ec36: blt 0x0012ecf0
  0012ec38: ldr r0,[0x0012ef58]
  0012ec3a: ldr r1,[0x0012ef5c]
  0012ec3c: add r0,pc
  0012ec3e: add r1,pc
  0012ec40: ldr r6,[r0,#0x0]
  0012ec42: ldr r5,[r1,#0x0]
  0012ec44: mov.w r1,#0x7d0
  0012ec48: ldr r0,[r6,#0x0]
  0012ec4a: blx r5
  0012ec4c: add.w r0,r0,#0x1f4
  0012ec50: str r0,[r4,#0x6c]
  0012ec52: ldr r0,[r6,#0x0]
  0012ec54: movs r1,#0x7
  0012ec56: blx r5
  0012ec58: mov r9,r0
  0012ec5a: ldr r0,[r6,#0x0]
  0012ec5c: movs r1,#0x2
  0012ec5e: blx r5
  0012ec60: cmp r0,#0x0
  0012ec62: mov.w r0,#0xffffffff
  0012ec66: add.w r1,r9,#0x5
  0012ec6a: it eq
  0012ec6c: mov.eq r0,#0x1
  0012ec6e: vmov.f32 s16,0x41200000
  0012ec72: mov.w r11,#0xffffffff
  0012ec76: muls r0,r1
  0012ec78: movs r1,#0x7
  0012ec7a: vmov s0,r0
  0012ec7e: vcvt.f32.s32 s0,s0
  0012ec82: vldr.32 s2,[r4,#0x2c]
  0012ec86: vdiv.f32 s0,s0,s16
  0012ec8a: vdiv.f32 s0,s0,s2
  0012ec8e: vstr.32 s0,[r4,#0x8]
  0012ec92: ldr r0,[r6,#0x0]
  0012ec94: blx r5
  0012ec96: mov r9,r0
  0012ec98: ldr r0,[r6,#0x0]
  0012ec9a: movs r1,#0x2
  0012ec9c: blx r5
  0012ec9e: cmp r0,#0x0
  0012eca0: add.w r0,r9,#0x5
  0012eca4: it eq
  0012eca6: mov.eq.w r11,#0x1
  0012ecaa: mul r0,r11,r0
  0012ecae: vmov s0,r0
  0012ecb2: vcvt.f32.s32 s0,s0
  0012ecb6: vldr.32 s2,[r4,#0x2c]
  0012ecba: ldrb.w r0,[r4,#0x80]
  0012ecbe: cmp r0,#0x0
  0012ecc0: vdiv.f32 s0,s0,s16
  0012ecc4: vdiv.f32 s0,s0,s2
  0012ecc8: vstr.32 s0,[r4,#0xc]
  0012eccc: beq 0x0012ecf0
  0012ecce: ldrd r0,r1,[r4,#0x78]
  0012ecd2: subs r1,#0x1
  0012ecd4: cmp r0,r1
  0012ecd6: bne 0x0012ecf0
  0012ecd8: vldr.32 s2,[pc,#0x388]
  0012ecdc: vldr.32 s4,[r4,#0x8]
  0012ece0: vmul.f32 s0,s0,s2
  0012ece4: vmul.f32 s2,s4,s2
  0012ece8: vstr.32 s2,[r4,#0x8]
  0012ecec: vstr.32 s0,[r4,#0xc]
  0012ecf0: ldrb.w r0,[r4,#0x84]
  0012ecf4: cbnz r0,0x0012ed32
  0012ecf6: mov r0,r4
  0012ecf8: blx 0x00076288
  0012ecfc: cbnz r0,0x0012ed32
  0012ecfe: vldr.32 s4,[r4,#0x58]
  0012ed02: vldr.32 s6,[r4,#0x5c]
  0012ed06: vldr.32 s0,[r4,#0x10]
  0012ed0a: vldr.32 s2,[r4,#0x14]
  0012ed0e: vcvt.f32.s32 s4,s4
  0012ed12: vcvt.f32.s32 s6,s6
  0012ed16: vsub.f32 s0,s4,s0
  0012ed1a: vldr.32 s4,[pc,#0x34c]
  0012ed1e: vsub.f32 s2,s6,s2
  0012ed22: vmul.f32 s0,s0,s4
  0012ed26: vmul.f32 s2,s2,s4
  0012ed2a: vstr.32 s0,[r4,#0x8]
  0012ed2e: vstr.32 s2,[r4,#0xc]
  0012ed32: ldr r0,[0x0012f088]
  0012ed34: mov r1,r10
  0012ed36: vldr.32 s0,[r4]
  0012ed3a: add r0,pc
  0012ed3c: vldr.32 s4,[r4,#0x8]
  0012ed40: vldr.32 s2,[r4,#0x4]
  0012ed44: vldr.32 s6,[r4,#0xc]
  0012ed48: vadd.f32 s0,s0,s4
  0012ed4c: ldr.w r9,[r0,#0x0]
  0012ed50: vadd.f32 s2,s2,s6
  0012ed54: ldr.w r0,[r9,#0x0]
  0012ed58: vldr.32 s4,[r0,#0xe4]
  0012ed5c: vdiv.f32 s0,s0,s4
  0012ed60: vdiv.f32 s2,s2,s4
  0012ed64: vmov s4,r10
  0012ed68: vcvt.f32.s32 s16,s4
  0012ed6c: vldr.32 s4,[r4,#0x10]
  0012ed70: vldr.32 s6,[r4,#0x14]
  0012ed74: ldr.w r0,[r4,#0x8c]
  0012ed78: vmla.f32 s4,s0,s16
  0012ed7c: vmla.f32 s6,s2,s16
  0012ed80: vstr.32 s4,[r4,#0x10]
  0012ed84: vstr.32 s6,[r4,#0x14]
  0012ed88: blx 0x00076294
  0012ed8c: ldr.w r0,[r4,#0x90]
  0012ed90: mov r1,r10
  0012ed92: blx 0x00076294
  0012ed96: ldr r0,[0x0012f08c]
  0012ed98: vmov.f32 s0,0xc0a00000
  0012ed9c: ldr r1,[r4,#0x78]
  0012ed9e: vmov.f32 s18,0x40400000
  0012eda2: add r0,pc
  0012eda4: movs r2,#0x1
  0012eda6: ldr r6,[r0,#0x0]
  0012eda8: add.w r0,r6,r1, lsl #0x2
  0012edac: movs r1,#0x0
  0012edae: vldr.32 s2,[r0]
  0012edb2: ldr r0,[0x0012f090]
  0012edb4: vadd.f32 s0,s2,s0
  0012edb8: vldr.32 s2,[pc,#0x2b0]
  0012edbc: add r0,pc
  0012edbe: ldr r5,[r0,#0x0]
  0012edc0: ldr r0,[r5,#0x0]
  0012edc2: vdiv.f32 s0,s0,s2
  0012edc6: vmul.f32 s0,s0,s18
  0012edca: vmov r3,s0
  0012edce: blx 0x0007159c
  0012edd2: mov r0,r4
  0012edd4: blx 0x00076288
  0012edd8: cmp r0,#0x0
  0012edda: beq 0x0012eea6
  0012eddc: cmp.w r8,#0x0
  0012ede0: bne 0x0012edfa
  0012ede2: ldr r0,[r5,#0x0]
  0012ede4: movs r1,#0x0
  0012ede6: str r1,[sp,#0x0]
  0012ede8: movs r1,#0x1
  0012edea: movs r2,#0x0
  0012edec: movs r3,#0x0
  0012edee: blx 0x00071548
  0012edf2: ldr r0,[r5,#0x0]
  0012edf4: movs r1,#0x3
  0012edf6: blx 0x000724a8
  0012edfa: vldr.32 s20,[pc,#0x274]
  0012edfe: ldr r0,[r4,#0x78]
  0012ee00: vdiv.f32 s0,s16,s20
  0012ee04: ldr.w r1,[r9,#0x0]
  0012ee08: add.w r0,r6,r0, lsl #0x2
  0012ee0c: vldr.32 s2,[r0]
  0012ee10: vldr.32 s4,[r1,#0xe0]
  0012ee14: vmul.f32 s2,s4,s2
  0012ee18: vmul.f32 s0,s0,s2
  0012ee1c: vldr.32 s2,[r4,#0x68]
  0012ee20: vmla.f32 s2,s0,s18
  0012ee24: vmov.f32 s18,0x3f800000
  0012ee28: vstr.32 s2,[r4,#0x68]
  0012ee2c: vcmpe.f32 s2,s18
  0012ee30: vmrs apsr,fpscr
  0012ee34: blt 0x0012ee42
  0012ee36: ldr.w r0,[r4,#0x94]
  0012ee3a: blx 0x000762a0
  0012ee3e: movs r0,#0x0
  0012ee40: str r0,[r4,#0x68]
  0012ee42: ldr.w r0,[r4,#0x88]
  0012ee46: mov r1,r10
  0012ee48: blx 0x00076294
  0012ee4c: ldr r0,[r4,#0x78]
  0012ee4e: vmov.f32 s0,0x40e00000
  0012ee52: vldr.32 s4,[pc,#0x224]
  0012ee56: adds r0,#0x1
  0012ee58: vmov s2,r0
  0012ee5c: vcvt.f32.s32 s2,s2
  0012ee60: vdiv.f32 s0,s2,s0
  0012ee64: vldr.32 s2,[pc,#0x20c]
  0012ee68: vmla.f32 s4,s0,s2
  0012ee6c: vldr.32 s2,[r4,#0x28]
  0012ee70: vldr.32 s0,[r4,#0x24]
  0012ee74: vmul.f32 s2,s2,s4
  0012ee78: vmov.f32 s4,s0
  0012ee7c: vcvt.s32.f32 s0,s0
  0012ee80: vdiv.f32 s2,s2,s20
  0012ee84: vmla.f32 s4,s2,s16
  0012ee88: vmov r2,s0
  0012ee8c: vcvt.s32.f32 s2,s4
  0012ee90: vstr.32 s4,[r4,#0x24]
  0012ee94: vmov r1,s2
  0012ee98: cmp r1,r2
  0012ee9a: ble 0x0012eef0
  0012ee9c: vldr.32 s0,[pc,#0x1dc]
  0012eea0: movs r1,#0x0
  0012eea2: str r1,[r4,#0x64]
  0012eea4: b 0x0012eef4
  0012eea6: cmp.w r8,#0x0
  0012eeaa: beq 0x0012eec4
  0012eeac: ldr r0,[r5,#0x0]
  0012eeae: movs r1,#0x1
  0012eeb0: blx 0x000724a8
  0012eeb4: ldr r0,[r5,#0x0]
  0012eeb6: movs r1,#0x0
  0012eeb8: str r1,[sp,#0x0]
  0012eeba: movs r1,#0x3
  0012eebc: movs r2,#0x0
  0012eebe: movs r3,#0x0
  0012eec0: blx 0x00071548
  0012eec4: ldr r0,[r4,#0x20]
  0012eec6: movw r1,#0x9c5
  0012eeca: add r0,r10
  0012eecc: str r0,[r4,#0x20]
  0012eece: cmp r0,r1
  0012eed0: blt 0x0012ef44
  0012eed2: ldr r1,[0x0012f094]
  0012eed4: movs r0,#0x0
  0012eed6: movw r2,#0x9c4
  0012eeda: add r1,pc
  0012eedc: strd r2,r0,[r4,#0x20]
  0012eee0: movs r2,#0x1
  0012eee2: ldr r1,[r1,#0x0]
  0012eee4: strb.w r2,[r4,#0x82]
  0012eee8: ldr r1,[r1,#0x0]
  0012eeea: str.w r0,[r1,#0x124]
  0012eeee: b 0x0012ef4c
  0012eef0: vldr.32 s0,[r4,#0x64]
  0012eef4: vldr.32 s2,[pc,#0x188]
  0012eef8: movw r2,#0x1771
  0012eefc: ldr r1,[r4,#0x74]
  0012eefe: vdiv.f32 s2,s16,s2
  0012ef02: add r1,r10
  0012ef04: str r1,[r4,#0x74]
  0012ef06: cmp r1,r2
  0012ef08: vadd.f32 s0,s2,s0
  0012ef0c: vmin.f32 d0,d0,d9
  0012ef10: vstr.32 s0,[r4,#0x64]
  0012ef14: blt 0x0012ef4a
  0012ef16: ldr r1,[r4,#0x7c]
  0012ef18: movs r2,#0x0
  0012ef1a: strd r2,r0,[r4,#0x74]
  0012ef1e: cmp r0,r1
  0012ef20: bge 0x0012ef60
  0012ef22: ldr.w r1,[r9,#0x0]
  0012ef26: add.w r0,r6,r0, lsl #0x2
  0012ef2a: vldr.32 s0,[r0]
  0012ef2e: vldr.32 s2,[r1,#0xe0]
  0012ef32: ldr.w r0,[r4,#0x88]
  0012ef36: vmul.f32 s0,s2,s0
  0012ef3a: vmov r1,s0
  0012ef3e: blx 0x0007627c
  0012ef42: b 0x0012ef4a
  0012ef44: mov.w r0,#0x3f800000
  0012ef48: str r0,[r4,#0x64]
  0012ef4a: movs r0,#0x1
  0012ef4c: vpop {d7,d8,d9,d10}
  0012ef50: add sp,#0x4
  0012ef52: pop.w {r8,r9,r10,r11}
  0012ef56: pop {r4,r5,r6,r7,pc}
  0012ef60: ldr r0,[0x0012f098]
  0012ef62: subs r1,#0x7
  0012ef64: movs r2,#0x1
  0012ef66: add r0,pc
  0012ef68: strb.w r2,[r4,#0x81]
  0012ef6c: movs r2,#0x1
  0012ef6e: ldr r5,[r0,#0x0]
  0012ef70: clz r0,r1
  0012ef74: movs r1,#0x26
  0012ef76: lsrs r0,r0,#0x5
  0012ef78: strb.w r0,[r4,#0x83]
  0012ef7c: ldr r0,[r5,#0x0]
  0012ef7e: blx 0x00072ef8
  0012ef82: cmp r0,#0x0
  0012ef84: bne 0x0012f05e
  0012ef86: ldr r0,[0x0012f09c]
  0012ef88: movs r2,#0x1
  0012ef8a: add r0,pc
  0012ef8c: ldr.w r8,[r0,#0x0]
  0012ef90: ldr.w r0,[r8,#0x0]
  0012ef94: ldr.w r1,[r0,#0x124]
  0012ef98: adds r6,r1,#0x1
  0012ef9a: str.w r6,[r0,#0x124]
  0012ef9e: ldr r0,[r5,#0x0]
  0012efa0: movs r1,#0x26
  0012efa2: blx 0x00072f04
  0012efa6: vmov s0,r6
  0012efaa: vldr.32 s16,[pc,#0xd8]
  0012efae: vmov s2,r0
  0012efb2: movs r1,#0xa
  0012efb4: vcvt.f32.s32 s0,s0
  0012efb8: vcvt.f32.s32 s2,s2
  0012efbc: vdiv.f32 s0,s0,s2
  0012efc0: vmul.f32 s0,s0,s16
  0012efc4: vcvt.s32.f32 s0,s0
  0012efc8: vmov r0,s0
  0012efcc: blx 0x0006f514
  0012efd0: cbnz r1,0x0012f03e
  0012efd2: ldr.w r0,[r8,#0x0]
  0012efd6: movs r1,#0x26
  0012efd8: movs r2,#0x1
  0012efda: vldr.32 s0,[r0,#0x124]
  0012efde: ldr r0,[r5,#0x0]
  0012efe0: vcvt.f32.s32 s18,s0
  0012efe4: blx 0x00072f04
  0012efe8: vmov s0,r0
  0012efec: vcvt.f32.s32 s0,s0
  0012eff0: vdiv.f32 s0,s18,s0
  0012eff4: vmul.f32 s0,s0,s16
  0012eff8: vcvt.s32.f32 s0,s0
  0012effc: vmov r0,s0
  0012f000: cmp r0,#0x1e
  0012f002: blt 0x0012f03e
  0012f004: ldr.w r0,[r8,#0x0]
  0012f008: movs r1,#0x26
  0012f00a: movs r2,#0x1
  0012f00c: ldr.w r4,[r4,#0xd0]
  0012f010: vldr.32 s0,[r0,#0x124]
  0012f014: ldr r0,[r5,#0x0]
  0012f016: vcvt.f32.s32 s18,s0
  0012f01a: blx 0x00072f04
  0012f01e: vmov s0,r0
  0012f022: mov r0,r4
  0012f024: movs r1,#0x26
  0012f026: vcvt.f32.s32 s0,s0
  0012f02a: vdiv.f32 s0,s18,s0
  0012f02e: vmul.f32 s0,s0,s16
  0012f032: vcvt.s32.f32 s0,s0
  0012f036: vmov r2,s0
  0012f03a: blx 0x00072f1c
  0012f03e: ldr.w r1,[r8,#0x0]
  0012f042: movs r2,#0x1
  0012f044: ldr r0,[r5,#0x0]
  0012f046: movs r4,#0x1
  0012f048: ldr.w r5,[r1,#0x124]
  0012f04c: movs r1,#0x26
  0012f04e: blx 0x00072f04
  0012f052: cmp r5,r0
  0012f054: itt ge
  0012f056: ldr.ge.w r0,[r8,#0x0]
  0012f05a: strb.ge.w r4,[r0,#0x128]
  0012f05e: movs r0,#0x0
  0012f060: b 0x0012ef4c
```
