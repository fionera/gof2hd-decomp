# StarMap::OnTouchMove
elf 0xcab0c body 846
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* StarMap::OnTouchMove(int, int) */

void StarMap::OnTouchMove(int param_1,int param_2)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  Matrix *pMVar4;
  undefined4 in_r2;
  uint uVar5;
  uint *puVar6;
  int *piVar7;
  uint in_fpscr;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  undefined1 auStack_58 [60];
  int local_1c;
  
  piVar7 = *(int **)(DAT_000dae84 + 0xdab22);
  local_1c = *piVar7;
  if (*(char *)(param_1 + 0xa9) != '\0') {
    ChoiceWindow::OnTouchMove(*(int *)(param_1 + 0x5c),param_2);
    goto LAB_000dab48;
  }
  if ((((*(char *)(param_1 + 0x138) != '\0') || (*(char *)(param_1 + 0x139) != '\0')) ||
      ((Layout::OnTouchMove(**(int **)(DAT_000dae88 + 0xdab6a),param_2),
       *(char *)(param_1 + 0xa8) != '\0' && (*(char *)(param_1 + 0x13a) != '\0')))) ||
     ((*(char *)(param_1 + 0x13b) != '\0' ||
      (TouchButton::OnTouchMove(*(int *)(param_1 + 0x4c),param_2),
      *(char *)(param_1 + 0x174) == '\0')))) goto LAB_000dab48;
  fVar12 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
  fVar11 = (float)VectorSignedToFloat(in_r2,(byte)(in_fpscr >> 0x16) & 3);
  fVar8 = *(float *)(param_1 + 0x15c);
  fVar9 = *(float *)(param_1 + 0x160);
  *(undefined4 *)(param_1 + 0x168) = 0x3f800000;
  *(float *)(param_1 + 0x15c) = fVar12;
  *(float *)(param_1 + 0x160) = fVar11;
  fVar10 = (fVar12 - fVar8) * *(float *)(param_1 + 0x1cc);
  fVar8 = *(float *)(param_1 + 0x1cc) * (fVar11 - fVar9);
  *(float *)(param_1 + 0x150) = fVar10;
  *(float *)(param_1 + 0x154) = fVar8;
  if (*(int *)(param_1 + 4) == 0) {
    fVar15 = fVar10 + fVar8;
    fVar9 = -fVar15;
    if (0.0 < fVar15) {
      fVar9 = fVar15;
    }
    fVar12 = *(float *)(param_1 + 0x144) - fVar12;
    fVar14 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x13c),
                                        (byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
    fVar13 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x140),
                                        (byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
    fVar15 = 10.0;
    if (fVar9 < 10.0) {
      fVar15 = fVar9;
    }
    fVar15 = fVar15 + *(float *)(param_1 + 0x1c0);
    if (DAT_000dae80 < fVar15) {
      fVar15 = DAT_000dae78;
    }
    fVar9 = -fVar12;
    if (0.0 < fVar12) {
      fVar9 = fVar12;
    }
    *(int *)(param_1 + 0x13c) = (int)(fVar10 + fVar14);
    *(int *)(param_1 + 0x140) = (int)(fVar8 + fVar13);
    *(float *)(param_1 + 0x1c0) = fVar15;
    if (3.0 < fVar9) {
LAB_000dae14:
      *(undefined4 *)(param_1 + 0x19c) = 0xffffffff;
      *(undefined4 *)(param_1 + 0x60) = 0xffffffff;
      *(undefined4 *)(param_1 + 0x1d0) = 0;
    }
    else {
      fVar11 = *(float *)(param_1 + 0x148) - fVar11;
      fVar8 = -fVar11;
      if (0.0 < fVar11) {
        fVar8 = fVar11;
      }
      if (3.0 < fVar8) goto LAB_000dae14;
    }
    puVar6 = *(uint **)(DAT_000dae8c + 0xdae2a);
    uVar5 = *puVar6;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar3 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
    __aeabi_memcpy(auStack_58,uVar3,0x3c);
    uVar5 = *puVar6;
    pMVar4 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraSetLocal(uVar5,pMVar4);
    goto LAB_000dab48;
  }
  fVar12 = *(float *)(param_1 + 0x144) - fVar12;
  fVar9 = -fVar12;
  if (0.0 < fVar12) {
    fVar9 = fVar12;
  }
  uVar5 = in_fpscr & 0xfffffff | (uint)(fVar9 < 3.0) << 0x1f | (uint)(fVar9 == 3.0) << 0x1e;
  bVar1 = (byte)(uVar5 >> 0x18);
  if ((bool)(bVar1 >> 6 & 1) || (bool)(bVar1 >> 7) != NAN(fVar9)) {
    fVar11 = *(float *)(param_1 + 0x148) - fVar11;
    fVar9 = -fVar11;
    if (0.0 < fVar11) {
      fVar9 = fVar11;
    }
    uVar5 = in_fpscr & 0xfffffff | (uint)(fVar9 < 3.0) << 0x1f | (uint)(fVar9 == 3.0) << 0x1e;
    bVar1 = (byte)(uVar5 >> 0x18);
    if (!(bool)(bVar1 >> 6 & 1) && (bool)(bVar1 >> 7) == NAN(fVar9)) goto LAB_000dac30;
  }
  else {
LAB_000dac30:
    *(undefined4 *)(param_1 + 0x1a0) = 0xffffffff;
    *(undefined4 *)(param_1 + 100) = 0xffffffff;
  }
  fVar11 = fVar8 * DAT_000dae5c;
  fVar12 = fVar10 * DAT_000dae64;
  fVar15 = fVar11 + *(float *)(param_1 + 0x18c);
  fVar9 = DAT_000dae60;
  if (DAT_000dae60 < fVar15) {
    fVar9 = DAT_000dae68;
  }
  if (DAT_000dae68 < fVar15) {
    fVar9 = DAT_000dae68;
  }
  fVar14 = fVar9;
  if (DAT_000dae60 < fVar15) {
    fVar14 = fVar15;
  }
  uVar5 = uVar5 & 0xfffffff | (uint)(fVar12 < 0.0) << 0x1f | (uint)(fVar12 == 0.0) << 0x1e;
  fVar13 = (float)VectorSignedToFloat((int)(fVar12 + *(float *)(param_1 + 0x188)) % 0x10000,
                                      (byte)(uVar5 >> 0x16) & 3);
  if (DAT_000dae68 < fVar15) {
    fVar14 = fVar9;
  }
  bVar1 = (byte)(uVar5 >> 0x18);
  fVar15 = fVar13 * DAT_000dae6c;
  fVar9 = -(fVar10 * DAT_000dae64);
  if (!(bool)(bVar1 >> 6 & 1) && (bool)(bVar1 >> 7) == NAN(fVar12)) {
    fVar9 = fVar12;
  }
  fVar8 = -(fVar8 * DAT_000dae5c);
  if (0.0 < fVar11) {
    fVar8 = fVar11;
  }
  fVar10 = DAT_000dae78;
  if (DAT_000dae74 < fVar9) {
    fVar10 = fVar12;
  }
  fVar9 = DAT_000dae78;
  if (DAT_000dae74 < fVar8) {
    fVar9 = fVar11;
  }
  *(float *)(param_1 + 0x150) = fVar12;
  *(float *)(param_1 + 0x154) = fVar11;
  *(float *)(param_1 + 0x16c) = fVar10;
  *(float *)(param_1 + 0x170) = fVar9;
  *(float *)(param_1 + 0x188) = fVar13;
  *(float *)(param_1 + 0x18c) = fVar14;
  AEGeometry::setRotation(*(AEGeometry **)(param_1 + 0xa4),fVar8,fVar15,fVar10);
LAB_000dab48:
  iVar2 = *piVar7 - local_1c;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  000dab0c: push {r4,r5,r6,r7,lr}
  000dab0e: add r7,sp,#0xc
  000dab10: push.w r8
  000dab14: sub sp,#0x40
  000dab16: mov r4,r0
  000dab18: ldr r0,[0x000dae84]
  000dab1a: mov r5,r2
  000dab1c: mov r6,r1
  000dab1e: add r0,pc
  000dab20: ldr.w r8,[r0,#0x0]
  000dab24: ldr.w r0,[r8,#0x0]
  000dab28: str r0,[sp,#0x3c]
  000dab2a: ldrb.w r0,[r4,#0xa9]
  000dab2e: cbz r0,0x000dab3c
  000dab30: ldr r0,[r4,#0x5c]
  000dab32: mov r1,r6
  000dab34: mov r2,r5
  000dab36: blx 0x0007474c
  000dab3a: b 0x000dab48
  000dab3c: ldrb.w r0,[r4,#0x138]
  000dab40: cbnz r0,0x000dab48
  000dab42: ldrb.w r0,[r4,#0x139]
  000dab46: cbz r0,0x000dab60
  000dab48: ldr r0,[sp,#0x3c]
  000dab4a: ldr.w r1,[r8,#0x0]
  000dab4e: subs r0,r1,r0
  000dab50: itttt eq
  000dab52: mov.eq r0,#0x0
  000dab54: add.eq sp,#0x40
  000dab56: pop.eq.w r8
  000dab5a: pop.eq {r4,r5,r6,r7,pc}
  000dab5c: blx 0x0006e824
  000dab60: ldr r0,[0x000dae88]
  000dab62: mov r1,r6
  000dab64: mov r2,r5
  000dab66: add r0,pc
  000dab68: ldr r0,[r0,#0x0]
  000dab6a: ldr r0,[r0,#0x0]
  000dab6c: blx 0x00074758
  000dab70: ldrb.w r0,[r4,#0xa8]
  000dab74: cbz r0,0x000dab7e
  000dab76: ldrb.w r0,[r4,#0x13a]
  000dab7a: cmp r0,#0x0
  000dab7c: bne 0x000dab48
  000dab7e: ldrb.w r0,[r4,#0x13b]
  000dab82: cmp r0,#0x0
  000dab84: bne 0x000dab48
  000dab86: ldr r0,[r4,#0x4c]
  000dab88: mov r1,r6
  000dab8a: mov r2,r5
  000dab8c: blx 0x00074764
  000dab90: ldrb.w r0,[r4,#0x174]
  000dab94: cmp r0,#0x0
  000dab96: beq 0x000dab48
  000dab98: vmov s0,r6
  000dab9c: mov.w r1,#0x3f800000
  000daba0: vcvt.f32.s32 s6,s0
  000daba4: vmov s0,r5
  000daba8: vcvt.f32.s32 s4,s0
  000dabac: vldr.32 s0,[r4,#0x15c]
  000dabb0: vldr.32 s2,[r4,#0x160]
  000dabb4: vldr.32 s10,[r4,#0x1cc]
  000dabb8: ldr r0,[r4,#0x4]
  000dabba: vsub.f32 s0,s6,s0
  000dabbe: str.w r1,[r4,#0x168]
  000dabc2: cmp r0,#0x0
  000dabc4: vstr.32 s6,[r4,#0x15c]
  000dabc8: vsub.f32 s8,s4,s2
  000dabcc: vstr.32 s4,[r4,#0x160]
  000dabd0: vmul.f32 s2,s0,s10
  000dabd4: vmul.f32 s0,s10,s8
  000dabd8: vstr.32 s2,[r4,#0x150]
  000dabdc: vstr.32 s0,[r4,#0x154]
  000dabe0: beq.w 0x000dad56
  000dabe4: vldr.32 s8,[r4,#0x144]
  000dabe8: vsub.f32 s8,s8,s6
  000dabec: vmov.f32 s6,0x40400000
  000dabf0: vcmpe.f32 s8,#0
  000dabf4: vmrs apsr,fpscr
  000dabf8: vneg.f32 s10,s8
  000dabfc: it gt
  000dabfe: vmov.gt.f32 s10,s8
  000dac02: vcmpe.f32 s10,s6
  000dac06: vmrs apsr,fpscr
  000dac0a: bgt 0x000dac30
  000dac0c: vldr.32 s8,[r4,#0x148]
  000dac10: vsub.f32 s4,s8,s4
  000dac14: vcmpe.f32 s4,#0
  000dac18: vmrs apsr,fpscr
  000dac1c: vneg.f32 s8,s4
  000dac20: it gt
  000dac22: vmov.gt.f32 s8,s4
  000dac26: vcmpe.f32 s8,s6
  000dac2a: vmrs apsr,fpscr
  000dac2e: ble 0x000dac3a
  000dac30: mov.w r0,#0xffffffff
  000dac34: str.w r0,[r4,#0x1a0]
  000dac38: str r0,[r4,#0x64]
  000dac3a: vldr.32 s6,[pc,#0x220]
  000dac3e: movw r2,#0xffff
  000dac42: vldr.32 s10,[r4,#0x18c]
  000dac46: vmul.f32 s4,s0,s6
  000dac4a: vldr.32 s12,[pc,#0x214]
  000dac4e: vldr.32 s1,[pc,#0x218]
  000dac52: vnmul.f32 s0,s0,s6
  000dac56: vmov.f32 s3,s12
  000dac5a: vldr.32 s8,[r4,#0x188]
  000dac5e: vldr.32 s14,[pc,#0x204]
  000dac62: ldr r3,[0x000dae7c]
  000dac64: vmul.f32 s5,s2,s14
  000dac68: vnmul.f32 s2,s2,s14
  000dac6c: vadd.f32 s10,s4,s10
  000dac70: vadd.f32 s8,s5,s8
  000dac74: vcmpe.f32 s10,s12
  000dac78: vmrs apsr,fpscr
  000dac7c: vcmpe.f32 s10,s1
  000dac80: it gt
  000dac82: vmov.gt.f32 s3,s1
  000dac86: vmrs apsr,fpscr
  000dac8a: vcmpe.f32 s10,s12
  000dac8e: it gt
  000dac90: vmov.gt.f32 s3,s1
  000dac94: vmrs apsr,fpscr
  000dac98: vmov.f32 s12,s3
  000dac9c: vcmpe.f32 s10,s1
  000daca0: it gt
  000daca2: vmov.gt.f32 s12,s10
  000daca6: vcvt.s32.f32 s8,s8
  000dacaa: vmrs apsr,fpscr
  000dacae: vcmpe.f32 s5,#0
  000dacb2: vldr.32 s10,[pc,#0x1b8]
  000dacb6: vmov r0,s8
  000dacba: asr.w r1,r0, asr #0x1f
  000dacbe: add.w r1,r0,r1, lsr #0x10
  000dacc2: bic.w r1,r1,r2
  000dacc6: sub.w r0,r0,r1
  000dacca: vmov s8,r0
  000dacce: vcvt.f32.s32 s8,s8
  000dacd2: it gt
  000dacd4: vmov.gt.f32 s12,s3
  000dacd8: vmrs apsr,fpscr
  000dacdc: vcmpe.f32 s4,#0
  000dace0: vldr.32 s3,[pc,#0x18c]
  000dace4: vmul.f32 s1,s8,s10
  000dace8: vmul.f32 s10,s12,s10
  000dacec: vmul.f32 s6,s1,s3
  000dacf0: vmul.f32 s10,s10,s3
  000dacf4: it gt
  000dacf6: vmov.gt.f32 s2,s5
  000dacfa: vmrs apsr,fpscr
  000dacfe: vmov r2,s6
  000dad02: vldr.32 s6,[pc,#0x170]
  000dad06: vmov r1,s10
  000dad0a: vldr.32 s10,[pc,#0x16c]
  000dad0e: vcmpe.f32 s2,s6
  000dad12: vmov.f32 s2,s10
  000dad16: it gt
  000dad18: vmov.gt.f32 s0,s4
  000dad1c: vmrs apsr,fpscr
  000dad20: vcmpe.f32 s0,s6
  000dad24: it gt
  000dad26: vmov.gt.f32 s2,s5
  000dad2a: vmrs apsr,fpscr
  000dad2e: it gt
  000dad30: vmov.gt.f32 s10,s4
  000dad34: ldr.w r0,[r4,#0xa4]
  000dad38: vstr.32 s5,[r4,#0x150]
  000dad3c: vstr.32 s4,[r4,#0x154]
  000dad40: vstr.32 s2,[r4,#0x16c]
  000dad44: vstr.32 s10,[r4,#0x170]
  000dad48: vstr.32 s8,[r4,#0x188]
  000dad4c: vstr.32 s12,[r4,#0x18c]
  000dad50: blx 0x00073054
  000dad54: b 0x000dab48
  000dad56: vadd.f32 s8,s2,s0
  000dad5a: vmov.f32 s12,0x41200000
  000dad5e: vcmpe.f32 s8,#0
  000dad62: vmrs apsr,fpscr
  000dad66: vneg.f32 s10,s8
  000dad6a: it gt
  000dad6c: vmov.gt.f32 s10,s8
  000dad70: vldr.32 s8,[r4,#0x13c]
  000dad74: vcmpe.f32 s10,s12
  000dad78: vldr.32 s1,[r4,#0x144]
  000dad7c: vmrs apsr,fpscr
  000dad80: vldr.32 s14,[r4,#0x140]
  000dad84: vsub.f32 s6,s1,s6
  000dad88: vldr.32 s3,[r4,#0x1c0]
  000dad8c: vcvt.f32.s32 s8,s8
  000dad90: vcvt.f32.s32 s14,s14
  000dad94: vadd.f32 s2,s2,s8
  000dad98: vadd.f32 s0,s0,s14
  000dad9c: vldr.32 s14,[pc,#0xd8]
  000dada0: it mi
  000dada2: vmov.mi.f32 s12,s10
  000dada6: vcvt.s32.f32 s2,s2
  000dadaa: vadd.f32 s8,s12,s3
  000dadae: vldr.32 s12,[pc,#0xd0]
  000dadb2: vcvt.s32.f32 s1,s0
  000dadb6: vmov.f32 s0,0x40400000
  000dadba: vneg.f32 s10,s6
  000dadbe: vcmpe.f32 s8,s12
  000dadc2: vmrs apsr,fpscr
  000dadc6: vcmpe.f32 s6,#0
  000dadca: it gt
  000dadcc: vmov.gt.f32 s8,s14
  000dadd0: vmrs apsr,fpscr
  000dadd4: it gt
  000dadd6: vmov.gt.f32 s10,s6
  000dadda: vstr.32 s2,[r4,#0x13c]
  000dadde: vcmpe.f32 s10,s0
  000dade2: vstr.32 s1,[r4,#0x140]
  000dade6: vmrs apsr,fpscr
  000dadea: vstr.32 s8,[r4,#0x1c0]
  000dadee: bgt 0x000dae14
  000dadf0: vldr.32 s2,[r4,#0x148]
  000dadf4: vsub.f32 s2,s2,s4
  000dadf8: vcmpe.f32 s2,#0
  000dadfc: vmrs apsr,fpscr
  000dae00: vneg.f32 s4,s2
  000dae04: it gt
  000dae06: vmov.gt.f32 s4,s2
  000dae0a: vcmpe.f32 s4,s0
  000dae0e: vmrs apsr,fpscr
  000dae12: ble 0x000dae24
  000dae14: mov.w r0,#0xffffffff
  000dae18: str.w r0,[r4,#0x19c]
  000dae1c: str r0,[r4,#0x60]
  000dae1e: movs r0,#0x0
  000dae20: str.w r0,[r4,#0x1d0]
  000dae24: ldr r0,[0x000dae8c]
  000dae26: add r0,pc
  000dae28: ldr r5,[r0,#0x0]
  000dae2a: ldr r4,[r5,#0x0]
  000dae2c: mov r0,r4
  000dae2e: blx 0x000717f4
  000dae32: mov r1,r0
  000dae34: mov r0,r4
  000dae36: blx 0x0006ff1c
  000dae3a: mov r4,sp
  000dae3c: mov r1,r0
  000dae3e: mov r0,r4
  000dae40: movs r2,#0x3c
  000dae42: blx 0x0006f1e4
  000dae46: ldr r5,[r5,#0x0]
  000dae48: mov r0,r5
  000dae4a: blx 0x000717f4
  000dae4e: mov r1,r0
  000dae50: mov r0,r5
  000dae52: mov r2,r4
  000dae54: blx 0x000720a0
  000dae58: b 0x000dab48
```
