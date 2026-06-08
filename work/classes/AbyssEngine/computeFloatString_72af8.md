# AbyssEngine::computeFloatString
elf 0x72af8 body 430
Sig: undefined __stdcall computeFloatString(float param_1, int param_2, int * param_3, int * param_4, int param_5)

## decompile
```c

/* AbyssEngine::computeFloatString(float, int, int*, int*, int) */

void AbyssEngine::computeFloatString
               (float param_1,int param_2,int *param_3,int *param_4,int param_5)

{
  int iVar1;
  double dVar2;
  ushort *puVar3;
  ushort *puVar4;
  int iVar5;
  ushort *puVar6;
  ushort *puVar7;
  ushort *puVar8;
  ushort uVar9;
  uint uVar10;
  uint in_fpscr;
  float fVar11;
  float fVar12;
  float fVar13;
  int in_stack_00000000;
  
  puVar3 = operator_new__(0x42);
  uVar10 = (uint)param_3 & ~((int)param_3 >> 0x1f);
  in_fpscr = in_fpscr & 0xfffffff;
  *(undefined4 *)param_5 = 0;
  if (0x1d < (int)uVar10) {
    uVar10 = 0x1e;
  }
  if ((float)param_2 < 0.0) {
    param_2 = (int)-(float)param_2;
    *(undefined4 *)param_5 = 1;
  }
  dVar2 = DAT_00082ca8;
  fVar12 = (float)VectorSignedToFloat((int)(float)param_2,(byte)(in_fpscr >> 0x16) & 3);
  fVar11 = (float)param_2 - fVar12;
  if ((int)(float)param_2 == 0) {
    in_fpscr = in_fpscr | (uint)(fVar11 < 0.0) << 0x1f;
    puVar8 = puVar3;
    if (fVar11 == 0.0 || SUB41(in_fpscr >> 0x1f,0) != NAN(fVar11)) {
      iVar5 = 0;
    }
    else {
      iVar5 = 1;
      fVar12 = fVar11;
      do {
        fVar11 = fVar12;
        iVar5 = iVar5 + -1;
        in_fpscr = in_fpscr & 0xfffffff;
        fVar12 = fVar11 * 10.0;
      } while (fVar11 * 10.0 < 1.0);
    }
  }
  else {
    puVar4 = puVar3 + 0x20;
    iVar5 = 0;
    puVar8 = puVar3 + 0x1f;
    while (fVar12 != 0.0) {
      fVar13 = fVar12 / 10.0;
      iVar5 = iVar5 + 1;
      fVar12 = (float)VectorSignedToFloat((int)fVar13,(byte)(in_fpscr >> 0x16) & 3);
      *puVar8 = (short)(longlong)(((double)(fVar13 - fVar12) + dVar2) * 10.0) + 0x30;
      puVar8 = puVar8 + -1;
    }
    puVar6 = puVar8 + 1;
    puVar7 = puVar4;
    if (puVar4 < puVar6) {
      puVar7 = puVar6;
    }
    puVar8 = (ushort *)((int)puVar3 + (~(uint)puVar8 + (int)puVar7 & 0xfffffffe));
    puVar7 = puVar3;
    for (; puVar6 < puVar4; puVar6 = puVar6 + 1) {
      *puVar7 = *puVar6;
      puVar7 = puVar7 + 1;
    }
  }
  puVar7 = puVar3 + uVar10;
  *param_4 = iVar5;
  puVar4 = puVar7;
  iVar1 = 0;
  if (in_stack_00000000 == 0) {
    puVar4 = puVar7 + iVar5;
    iVar1 = iVar5;
  }
  puVar6 = puVar3;
  if (puVar3 <= puVar4) {
    while( true ) {
      puVar6 = puVar4;
      if (puVar8 <= puVar4) {
        puVar6 = puVar3 + 0x20;
      }
      if (puVar6 <= puVar8) break;
      fVar12 = (float)VectorSignedToFloat((int)(fVar11 * 10.0),(byte)(in_fpscr >> 0x16) & 3);
      fVar11 = fVar11 * 10.0 - fVar12;
      *puVar8 = (short)(int)fVar12 + 0x30;
      puVar8 = puVar8 + 1;
    }
    if (puVar4 < puVar3 + 0x20) {
      uVar9 = puVar7[iVar1] + 5;
      puVar7[iVar1] = uVar9;
      puVar8 = puVar4;
      while (puVar6 = puVar4, 0x39 < uVar9) {
        *puVar8 = 0x30;
        if (puVar3 < puVar8) {
          puVar8 = puVar8 + -1;
          uVar9 = *puVar8 + 1;
          *puVar8 = uVar9;
        }
        else {
          iVar5 = iVar5 + 1;
          uVar9 = 0x31;
          *puVar8 = 0x31;
          *param_4 = iVar5;
          if (in_stack_00000000 == 0) {
            if (puVar3 < puVar4) {
              *puVar4 = 0x30;
              uVar9 = *puVar8;
            }
            else {
              uVar9 = 0x31;
            }
            puVar4 = puVar4 + 1;
          }
        }
      }
    }
    else {
      puVar6 = puVar3 + 0x1f;
    }
  }
  *puVar6 = 0;
  return;
}

```

## target disasm
```
  00082af8: push {r4,r5,r6,r7,lr}
  00082afa: add r7,sp,#0xc
  00082afc: push {r8,r9,r11}
  00082b00: vpush {d8}
  00082b04: vmov s16,r0
  00082b08: movs r0,#0x42
  00082b0a: mov r5,r3
  00082b0c: mov r8,r2
  00082b0e: mov r6,r1
  00082b10: movs r4,#0x0
  00082b12: blx 0x0006ec08
  00082b16: bic.w r9,r6,r6, asr #0x1f
  00082b1a: vcmpe.f32 s16,#0
  00082b1e: cmp.w r9,#0x1e
  00082b22: str r4,[r5,#0x0]
  00082b24: it ge
  00082b26: mov.ge.w r9,#0x1e
  00082b2a: vmrs apsr,fpscr
  00082b2e: ldr.w r12,[r7,#0x8]
  00082b32: ittt mi
  00082b34: vneg.mi.f32 s16,s16
  00082b38: mov.mi r1,#0x1
  00082b3a: str.mi r1,[r5,#0x0]
  00082b3c: vcvt.s32.f32 s0,s16
  00082b40: vmov r1,s0
  00082b44: vcvt.f32.s32 s2,s0
  00082b48: vsub.f32 s0,s16,s2
  00082b4c: cbz r1,0x00082bc4
  00082b4e: vmov.f32 s4,0x41200000
  00082b52: add.w r6,r0,#0x3e
  00082b56: vmov.f64 d17,0x4024000000000000
  00082b5a: add.w r1,r0,#0x40
  00082b5e: movs r2,#0x0
  00082b60: vldr.64 d16,[pc,#0x144]
  00082b64: b 0x00082b92
  00082b66: vdiv.f32 s6,s2,s4
  00082b6a: adds r2,#0x1
  00082b6c: vcvt.s32.f32 s2,s6
  00082b70: vcvt.f32.s32 s2,s2
  00082b74: vsub.f32 s6,s6,s2
  00082b78: vcvt.f64.f32 d18,s6
  00082b7c: vadd.f64 d18,d18,d16
  00082b80: vmul.f64 d18,d18,d17
  00082b84: vcvt.s32.f64 s6,d18
  00082b88: vmov r5,s6
  00082b8c: adds r5,#0x30
  00082b8e: strh r5,[r6],#-0x2
  00082b92: vcmpe.f32 s2,#0
  00082b96: vmrs apsr,fpscr
  00082b9a: bne 0x00082b66
  00082b9c: adds r4,r6,#0x2
  00082b9e: mov r5,r1
  00082ba0: cmp r4,r1
  00082ba2: mvn.w r6,r6
  00082ba6: it hi
  00082ba8: mov.hi r5,r4
  00082baa: add r6,r5
  00082bac: bic r6,r6,#0x1
  00082bb0: adds r5,r0,r6
  00082bb2: mov r6,r0
  00082bb4: b 0x00082bbe
  00082bb6: ldrh.w r3,[r4],#0x2
  00082bba: strh r3,[r6],#0x2
  00082bbe: cmp r4,r1
  00082bc0: bcc 0x00082bb6
  00082bc2: b 0x00082bf6
  00082bc4: vcmpe.f32 s0,#0
  00082bc8: vmrs apsr,fpscr
  00082bcc: ble 0x00082bf2
  00082bce: vmov.f32 s2,0x41200000
  00082bd2: movs r2,#0x1
  00082bd4: vmov.f32 s4,0x3f800000
  00082bd8: vmov.f32 s6,s0
  00082bdc: subs r2,#0x1
  00082bde: vmul.f32 s0,s0,s2
  00082be2: vcmpe.f32 s0,s4
  00082be6: vmrs apsr,fpscr
  00082bea: bmi 0x00082bd8
  00082bec: vmov.f32 s0,s6
  00082bf0: b 0x00082bf4
  00082bf2: movs r2,#0x0
  00082bf4: mov r5,r0
  00082bf6: add.w r9,r0,r9, lsl #0x1
  00082bfa: movs r4,#0x0
  00082bfc: str.w r2,[r8,#0x0]
  00082c00: cmp.w r12,#0x0
  00082c04: mov r1,r9
  00082c06: it eq
  00082c08: mov.eq r4,r2
  00082c0a: it eq
  00082c0c: add.eq.w r1,r1,r2, lsl #0x1
  00082c10: mov.w lr,#0x0
  00082c14: cmp r1,r0
  00082c16: mov r6,r0
  00082c18: bcc 0x00082c98
  00082c1a: vmov.f32 s2,0x41200000
  00082c1e: add.w r6,r0,#0x40
  00082c22: b 0x00082c42
  00082c24: vmul.f32 s0,s0,s2
  00082c28: vcvt.s32.f32 s4,s0
  00082c2c: vcvt.f32.s32 s4,s4
  00082c30: vsub.f32 s0,s0,s4
  00082c34: vcvt.s32.f32 s4,s4
  00082c38: vmov r3,s4
  00082c3c: adds r3,#0x30
  00082c3e: strh r3,[r5],#0x2
  00082c42: cmp r5,r1
  00082c44: it ls
  00082c46: cmp.ls r5,r6
  00082c48: bcc 0x00082c24
  00082c4a: cmp r1,r6
  00082c4c: bcs 0x00082c94
  00082c4e: ldrh.w r3,[r9,r4,lsl #0x1]
  00082c52: movs r5,#0x30
  00082c54: adds r6,r3,#0x5
  00082c56: mov r3,r1
  00082c58: strh.w r6,[r9,r4,lsl #0x1]
  00082c5c: b 0x00082c8a
  00082c5e: cmp r3,r0
  00082c60: strh r5,[r3,#0x0]
  00082c62: bls 0x00082c6e
  00082c64: ldrh.w r4,[r3,#-0x2]!
  00082c68: adds r6,r4,#0x1
  00082c6a: strh r6,[r3,#0x0]
  00082c6c: b 0x00082c8a
  00082c6e: adds r2,#0x1
  00082c70: movs r6,#0x31
  00082c72: cmp.w r12,#0x0
  00082c76: strh r6,[r3,#0x0]
  00082c78: str.w r2,[r8,#0x0]
  00082c7c: bne 0x00082c8a
  00082c7e: cmp r1,r0
  00082c80: itte hi
  00082c82: strh.hi r5,[r1,#0x0]
  00082c84: ldrh.hi r6,[r3,#0x0]
  00082c86: mov.ls r6,#0x31
  00082c88: adds r1,#0x2
  00082c8a: uxth r4,r6
  00082c8c: cmp r4,#0x3a
  00082c8e: bcs 0x00082c5e
  00082c90: mov r6,r1
  00082c92: b 0x00082c98
  00082c94: add.w r6,r0,#0x3e
  00082c98: strh.w lr,[r6,#0x0]
  00082c9c: vpop {d8}
  00082ca0: pop.w {r8,r9,r11}
  00082ca4: pop {r4,r5,r6,r7,pc}
```
