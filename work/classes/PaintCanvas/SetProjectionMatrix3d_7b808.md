# PaintCanvas::SetProjectionMatrix3d
elf 0x7b808 body 324
Sig: undefined __thiscall SetProjectionMatrix3d(PaintCanvas * this, float param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetProjectionMatrix3d(float, float, float) */

void __thiscall
AbyssEngine::PaintCanvas::SetProjectionMatrix3d
          (PaintCanvas *this,float param_1,float param_2,float param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  float fVar6;
  float fVar7;
  undefined4 in_r1;
  float in_r2;
  float *pfVar8;
  float in_r3;
  float *pfVar9;
  float *pfVar10;
  undefined4 *puVar11;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float fVar12;
  float fVar13;
  
  pfVar8 = *(float **)(DAT_0008b958 + 0x8b82a);
  pfVar9 = *(float **)(DAT_0008b95c + 0x8b82c);
  pfVar10 = *(float **)(DAT_0008b960 + 0x8b82e);
  puVar11 = *(undefined4 **)(DAT_0008b964 + 0x8b830);
  **(float **)(DAT_0008b954 + 0x8b826) = in_r3;
  *pfVar8 = in_r2;
  *puVar11 = in_r1;
  *pfVar9 = in_r2;
  *pfVar10 = in_r3;
  uVar4 = Engine::GetDisplayWidth();
  uVar5 = Engine::GetDisplayHeight();
  fVar6 = (float)AEMath::Sinf(extraout_s0);
  fVar7 = (float)AEMath::Cosf(extraout_s0_00);
  uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  fVar12 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
  fVar13 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = uVar1;
  *(undefined4 *)(this + 0x70) = uVar2;
  *(undefined4 *)(this + 0x74) = uVar3;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = uVar1;
  *(undefined4 *)(this + 0x60) = uVar2;
  *(undefined4 *)(this + 100) = uVar3;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = uVar1;
  *(undefined4 *)(this + 0x50) = uVar2;
  *(undefined4 *)(this + 0x54) = uVar3;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = uVar1;
  *(undefined4 *)(this + 0x40) = uVar2;
  *(undefined4 *)(this + 0x44) = uVar3;
  if (3 < *(uint *)(this + 0x30)) goto LAB_0008b910;
  fVar12 = fVar12 / fVar13;
  fVar6 = 1.0 / (fVar6 / fVar7);
  switch(*(uint *)(this + 0x30)) {
  case 0:
    fVar12 = fVar6 / fVar12;
    fVar6 = -fVar6;
    break;
  case 1:
    fVar12 = -(fVar6 / fVar12);
    break;
  case 2:
    fVar12 = fVar6 / fVar12;
    goto LAB_0008b908;
  case 3:
    fVar12 = fVar6 / fVar12;
    fVar6 = -fVar6;
    fVar12 = -fVar12;
LAB_0008b908:
    *(float *)(this + 0x4c) = fVar6;
    *(float *)(this + 0x38) = fVar12;
    goto LAB_0008b910;
  }
  *(float *)(this + 0x48) = fVar6;
  *(float *)(this + 0x3c) = fVar12;
LAB_0008b910:
  *(undefined4 *)(this + 100) = DAT_0008b950;
  *(float *)(this + 0x60) = (in_r2 + in_r3) / (in_r2 - in_r3);
  *(float *)(this + 0x70) = ((in_r3 + in_r3) * in_r2) / (in_r2 - in_r3);
  return;
}

```

## target disasm
```
  0008b808: push {r4,r5,r6,r7,lr}
  0008b80a: add r7,sp,#0xc
  0008b80c: push {r7,r8,r9,r10,r11}
  0008b810: vpush {d8}
  0008b814: mov r4,r0
  0008b816: ldr r0,[0x0008b954]
  0008b818: mov r11,r2
  0008b81a: ldr r2,[0x0008b958]
  0008b81c: mov r8,r3
  0008b81e: ldr r3,[0x0008b95c]
  0008b820: ldr r5,[0x0008b960]
  0008b822: add r0,pc
  0008b824: ldr r6,[0x0008b964]
  0008b826: add r2,pc
  0008b828: add r3,pc
  0008b82a: add r5,pc
  0008b82c: add r6,pc
  0008b82e: ldr r0,[r0,#0x0]
  0008b830: ldr r2,[r2,#0x0]
  0008b832: vmov.f32 s0,0x3f000000
  0008b836: ldr r3,[r3,#0x0]
  0008b838: vmov s2,r1
  0008b83c: ldr r5,[r5,#0x0]
  0008b83e: ldr r6,[r6,#0x0]
  0008b840: str.w r8,[r0,#0x0]
  0008b844: str.w r11,[r2,#0x0]
  0008b848: str r1,[r6,#0x0]
  0008b84a: str.w r11,[r3,#0x0]
  0008b84e: vmul.f32 s16,s2,s0
  0008b852: str.w r8,[r5,#0x0]
  0008b856: ldr r0,[r4,#0x34]
  0008b858: blx 0x0006e8cc
  0008b85c: mov r9,r0
  0008b85e: ldr r0,[r4,#0x34]
  0008b860: blx 0x0006e8d8
  0008b864: vmov r5,s16
  0008b868: mov r10,r0
  0008b86a: mov r0,r5
  0008b86c: blx 0x0006f1a8
  0008b870: mov r6,r0
  0008b872: mov r0,r5
  0008b874: blx 0x0006f49c
  0008b878: vmov s0,r9
  0008b87c: add.w r1,r4,#0x68
  0008b880: vmov.i32 q8,#0x0
  0008b884: vcvt.f32.s32 s4,s0
  0008b888: vmov s0,r10
  0008b88c: vmov s2,r11
  0008b890: vcvt.f32.s32 s6,s0
  0008b894: vst1.32 {d16,d17},[r1]
  0008b898: add.w r1,r4,#0x58
  0008b89c: vmov s0,r8
  0008b8a0: vst1.32 {d16,d17},[r1]
  0008b8a4: add.w r1,r4,#0x48
  0008b8a8: vst1.32 {d16,d17},[r1]
  0008b8ac: add.w r1,r4,#0x38
  0008b8b0: vst1.32 {d16,d17},[r1]
  0008b8b4: ldr r1,[r4,#0x30]
  0008b8b6: cmp r1,#0x3
  0008b8b8: bhi 0x0008b910
  0008b8ba: vmov s10,r0
  0008b8be: vmov s8,r6
  0008b8c2: vdiv.f32 s6,s4,s6
  0008b8c6: vdiv.f32 s8,s8,s10
  0008b8ca: vmov.f32 s10,0x3f800000
  0008b8ce: vdiv.f32 s4,s10,s8
  0008b8d2: tbb [pc,r1]
  0008b8da: vdiv.f32 s6,s4,s6
  0008b8de: vneg.f32 s4,s4
  0008b8e2: b 0x0008b8ec
  0008b8e4: vdiv.f32 s6,s4,s6
  0008b8e8: vneg.f32 s6,s6
  0008b8ec: vstr.32 s4,[r4,#0x48]
  0008b8f0: vstr.32 s6,[r4,#0x3c]
  0008b8f4: b 0x0008b910
  0008b8f6: vdiv.f32 s6,s4,s6
  0008b8fa: b 0x0008b908
  0008b8fc: vdiv.f32 s6,s4,s6
  0008b900: vneg.f32 s4,s4
  0008b904: vneg.f32 s6,s6
  0008b908: vstr.32 s4,[r4,#0x4c]
  0008b90c: vstr.32 s6,[r4,#0x38]
  0008b910: vcvt.f64.f32 d16,s0
  0008b914: ldr r0,[0x0008b950]
  0008b916: str r0,[r4,#0x64]
  0008b918: vadd.f64 d16,d16,d16
  0008b91c: vcvt.f64.f32 d17,s2
  0008b920: vsub.f32 s4,s2,s0
  0008b924: vmul.f64 d16,d16,d17
  0008b928: vcvt.f64.f32 d17,s4
  0008b92c: vdiv.f64 d16,d16,d17
  0008b930: vadd.f32 s0,s2,s0
  0008b934: vcvt.f32.f64 s2,d16
  0008b938: vdiv.f32 s0,s0,s4
  0008b93c: vstr.32 s0,[r4,#0x60]
  0008b940: vstr.32 s2,[r4,#0x70]
  0008b944: vpop {d8}
  0008b948: add sp,#0x4
  0008b94a: pop.w {r8,r9,r10,r11}
  0008b94e: pop {r4,r5,r6,r7,pc}
```
