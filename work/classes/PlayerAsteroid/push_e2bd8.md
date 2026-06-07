# PlayerAsteroid::push
elf 0xe2bd8 body 302
Sig: undefined __stdcall push(int param_1)

## decompile
```c

/* PlayerAsteroid::push(int) */

void PlayerAsteroid::push(int param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  int iVar3;
  undefined4 *puVar4;
  Matrix *pMVar5;
  int in_r1;
  Matrix *pMVar6;
  Vector *pVVar7;
  int *piVar8;
  uint in_fpscr;
  float fVar9;
  float in_s1;
  float fVar10;
  Vector aVStack_b0 [12];
  Matrix aMStack_a4 [60];
  undefined4 local_68 [5];
  undefined4 local_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined4 local_30;
  int local_2c;
  
  uVar2 = DAT_000f2d10;
  uVar1 = DAT_000f2d08;
  piVar8 = *(int **)(DAT_000f2d1c + 0xf2bee);
  local_2c = *piVar8;
  if (0 < *(int *)(param_1 + 0x104)) {
    iVar3 = *(int *)(param_1 + 0x104) - in_r1;
    uStack_4c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_48 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_44 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    fVar10 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
    fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x108),(byte)(in_fpscr >> 0x16) & 3
                                      );
    *(int *)(param_1 + 0x104) = iVar3;
    puVar4 = (undefined4 *)((uint)local_68 | 4);
    *puVar4 = 0;
    puVar4[1] = uStack_4c;
    puVar4[2] = uStack_48;
    puVar4[3] = uStack_44;
    local_50 = 0;
    fVar10 = fVar10 / fVar9;
    uStack_40 = uVar1;
    uStack_38 = uVar2;
    local_68[0] = 0x3f800000;
    local_54 = 0x3f800000;
    local_30 = 0x3f800000;
    AbyssEngine::AEMath::MatrixSetRotation
              (aMStack_a4,fVar10 * *(float *)(param_1 + 0x120),in_s1,
               fVar10 * *(float *)(param_1 + 0x11c));
    iVar3 = *(int *)(param_1 + 0x134);
    if (0 < iVar3) {
      pMVar6 = *(Matrix **)(param_1 + 8);
      pMVar5 = (Matrix *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::operator*(aMStack_a4,pMVar5);
      AEGeometry::setMatrix(pMVar6);
      iVar3 = *(int *)(param_1 + 0x134);
    }
    VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
    pVVar7 = *(Vector **)(param_1 + 8);
    fVar9 = (float)VectorSignedToFloat(*(float *)(param_1 + 0x108),(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::AEMath::operator*(aVStack_b0,*(float *)(param_1 + 0x108));
    AbyssEngine::AEMath::operator*
              ((Vector *)aMStack_a4,(2.0 - fVar10) * 3.0 * (fVar9 / DAT_000f2d18));
    AEGeometry::translate(pVVar7);
  }
  if (*piVar8 != local_2c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000f2bd8: push {r4,r5,r6,r7,lr}
  000f2bda: add r7,sp,#0xc
  000f2bdc: push.w r8
  000f2be0: vpush {d8,d9}
  000f2be4: sub sp,#0x90
  000f2be6: mov r4,r0
  000f2be8: ldr r0,[0x000f2d1c]
  000f2bea: add r0,pc
  000f2bec: ldr.w r8,[r0,#0x0]
  000f2bf0: ldr.w r0,[r8,#0x0]
  000f2bf4: str r0,[sp,#0x8c]
  000f2bf6: ldr.w r0,[r4,#0x104]
  000f2bfa: cmp r0,#0x1
  000f2bfc: blt 0x000f2cec
  000f2bfe: subs r0,r0,r1
  000f2c00: vldr.32 s0,[r4,#0x108]
  000f2c04: adr r1,[0xf2d08]
  000f2c06: vmov.i32 q9,#0x0
  000f2c0a: vmov s2,r0
  000f2c0e: vld1.64 {d16,d17},[r1]
  000f2c12: add r1,sp,#0x50
  000f2c14: vcvt.f32.s32 s2,s2
  000f2c18: vcvt.f32.s32 s0,s0
  000f2c1c: str.w r0,[r4,#0x104]
  000f2c20: orr r0,r1,#0x4
  000f2c24: vst1.32 {d18,d19},[r0]
  000f2c28: add.w r0,r1,#0x18
  000f2c2c: vst1.64 {d18,d19},[r0]
  000f2c30: add.w r0,r1,#0x28
  000f2c34: vdiv.f32 s16,s2,s0
  000f2c38: vst1.64 {d16,d17},[r0]
  000f2c3c: mov.w r0,#0x3f800000
  000f2c40: str r0,[sp,#0x50]
  000f2c42: str r0,[sp,#0x64]
  000f2c44: str r0,[sp,#0x88]
  000f2c46: add r0,sp,#0x14
  000f2c48: vldr.32 s0,[r4,#0x118]
  000f2c4c: vldr.32 s2,[r4,#0x11c]
  000f2c50: vmul.f32 s0,s16,s0
  000f2c54: vldr.32 s4,[r4,#0x120]
  000f2c58: vmul.f32 s2,s16,s2
  000f2c5c: vmov r2,s0
  000f2c60: vmov r3,s2
  000f2c64: vmul.f32 s0,s16,s4
  000f2c68: vstr.32 s0,[sp]
  000f2c6c: blx 0x00072094
  000f2c70: ldr.w r0,[r4,#0x134]
  000f2c74: cmp r0,#0x1
  000f2c76: blt 0x000f2c98
  000f2c78: ldr r5,[r4,#0x8]
  000f2c7a: mov r0,r5
  000f2c7c: blx 0x000721cc
  000f2c80: add r6,sp,#0x14
  000f2c82: add r2,sp,#0x50
  000f2c84: mov r1,r0
  000f2c86: mov r0,r6
  000f2c88: blx 0x0006f82c
  000f2c8c: mov r0,r5
  000f2c8e: mov r1,r6
  000f2c90: blx 0x000721e4
  000f2c94: ldr.w r0,[r4,#0x134]
  000f2c98: vmov s0,r0
  000f2c9c: add.w r1,r4,#0x10c
  000f2ca0: vcvt.f32.s32 s0,s0
  000f2ca4: ldr r5,[r4,#0x8]
  000f2ca6: vmov r2,s0
  000f2caa: vldr.32 s0,[r4,#0x108]
  000f2cae: add r4,sp,#0x8
  000f2cb0: vcvt.f32.s32 s18,s0
  000f2cb4: mov r0,r4
  000f2cb6: blx 0x0006ec74
  000f2cba: vldr.32 s0,[pc,#0x5c]
  000f2cbe: vmov.f32 s2,0x40000000
  000f2cc2: vmov.f32 s4,0x40400000
  000f2cc6: add r6,sp,#0x14
  000f2cc8: vdiv.f32 s0,s18,s0
  000f2ccc: mov r1,r4
  000f2cce: mov r0,r6
  000f2cd0: vsub.f32 s2,s2,s16
  000f2cd4: vmul.f32 s2,s2,s4
  000f2cd8: vmul.f32 s0,s2,s0
  000f2cdc: vmov r2,s0
  000f2ce0: blx 0x0006ec74
  000f2ce4: mov r0,r5
  000f2ce6: mov r1,r6
  000f2ce8: blx 0x00072460
  000f2cec: ldr r0,[sp,#0x8c]
  000f2cee: ldr.w r1,[r8,#0x0]
  000f2cf2: subs r0,r1,r0
  000f2cf4: itttt eq
  000f2cf6: add.eq sp,#0x90
  000f2cf8: vpop.eq {d8,d9}
  000f2cfc: pop.eq.w r8
  000f2d00: pop.eq {r4,r5,r6,r7,pc}
  000f2d02: blx 0x0006e824
```
