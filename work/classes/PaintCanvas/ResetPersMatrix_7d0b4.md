# PaintCanvas::ResetPersMatrix
elf 0x7d0b4 body 380
Sig: undefined __thiscall ResetPersMatrix(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::ResetPersMatrix() */

void __thiscall AbyssEngine::PaintCanvas::ResetPersMatrix(PaintCanvas *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  float fVar7;
  float fVar8;
  char *pcVar9;
  code *pcVar10;
  float *pfVar11;
  PaintCanvas *pPVar12;
  uint in_fpscr;
  float fVar13;
  float fVar14;
  
  uVar5 = Engine::GetDisplayWidth();
  uVar6 = Engine::GetDisplayHeight();
  pfVar11 = *(float **)(DAT_0008d238 + 0x8d0dc);
  fVar7 = (float)AEMath::Sinf(*pfVar11 * 0.5);
  fVar8 = (float)AEMath::Cosf(*pfVar11 * 0.5);
  uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  pPVar12 = this + 0x38;
  fVar13 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  fVar14 = (float)VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = uVar1;
  *(undefined4 *)(this + 0x70) = uVar2;
  *(undefined4 *)(this + 0x74) = uVar3;
  *(undefined4 *)pPVar12 = 0;
  *(undefined4 *)(this + 0x3c) = uVar1;
  *(undefined4 *)(this + 0x40) = uVar2;
  *(undefined4 *)(this + 0x44) = uVar3;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = uVar1;
  *(undefined4 *)(this + 0x60) = uVar2;
  *(undefined4 *)(this + 100) = uVar3;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = uVar1;
  *(undefined4 *)(this + 0x50) = uVar2;
  *(undefined4 *)(this + 0x54) = uVar3;
  if (3 < *(uint *)(this + 0x30)) goto LAB_0008d18e;
  fVar13 = fVar13 / fVar14;
  fVar7 = 1.0 / (fVar7 / fVar8);
  switch(*(uint *)(this + 0x30)) {
  case 0:
    fVar13 = fVar7 / fVar13;
    fVar7 = -fVar7;
    break;
  case 1:
    fVar13 = -(fVar7 / fVar13);
    break;
  case 2:
    fVar13 = fVar7 / fVar13;
    goto LAB_0008d186;
  case 3:
    fVar13 = fVar7 / fVar13;
    fVar7 = -fVar7;
    fVar13 = -fVar13;
LAB_0008d186:
    *(float *)(this + 0x4c) = fVar7;
    *(float *)(this + 0x38) = fVar13;
    goto LAB_0008d18e;
  }
  *(float *)(this + 0x48) = fVar7;
  *(float *)(this + 0x3c) = fVar13;
LAB_0008d18e:
  iVar4 = DAT_0008d244;
  fVar7 = **(float **)(DAT_0008d23c + 0x8d196);
  fVar8 = **(float **)(DAT_0008d240 + 0x8d1a4);
  *(undefined4 *)(this + 100) = DAT_0008d234;
  pcVar9 = *(char **)(iVar4 + 0x8d1b8);
  *(float *)(this + 0x60) = (fVar7 + fVar8) / (fVar8 - fVar7);
  *(float *)(this + 0x70) = ((fVar7 + fVar7) * fVar8) / (fVar8 - fVar7);
  if (*pcVar9 == '\0') {
    pcVar10 = *(code **)(DAT_0008d248 + 0x8d1fa);
    (*pcVar10)(0x1702);
    glLoadIdentity();
    glScalef(0x3f800000,DAT_0008d234,0x3f800000);
    (*pcVar10)(0x1701);
    glLoadMatrixf(pPVar12);
    (*pcVar10)(0x1700);
    (*(code *)(DAT_001ab3e4 + 0x1ab3e8))();
    return;
  }
  (*(code *)(DAT_001ab3d4 + 0x1ab3d8))(*(undefined4 *)(this + 0x34),pPVar12);
  return;
}

```

## target disasm
```
  0008d0b4: push {r4,r5,r6,r7,lr}
  0008d0b6: add r7,sp,#0xc
  0008d0b8: push {r8,r9,r11}
  0008d0bc: vpush {d8}
  0008d0c0: mov r4,r0
  0008d0c2: ldr r0,[r0,#0x34]
  0008d0c4: blx 0x0006e8cc
  0008d0c8: mov r9,r0
  0008d0ca: ldr r0,[r4,#0x34]
  0008d0cc: blx 0x0006e8d8
  0008d0d0: mov r6,r0
  0008d0d2: ldr r0,[0x0008d238]
  0008d0d4: vmov.f32 s16,0x3f000000
  0008d0d8: add r0,pc
  0008d0da: ldr r5,[r0,#0x0]
  0008d0dc: vldr.32 s0,[r5]
  0008d0e0: vmul.f32 s0,s0,s16
  0008d0e4: vmov r0,s0
  0008d0e8: blx 0x0006f1a8
  0008d0ec: vldr.32 s0,[r5]
  0008d0f0: mov r8,r0
  0008d0f2: vmul.f32 s0,s0,s16
  0008d0f6: vmov r0,s0
  0008d0fa: blx 0x0006f49c
  0008d0fe: vmov.i32 q8,#0x0
  0008d102: add.w r1,r4,#0x68
  0008d106: vmov s0,r9
  0008d10a: add.w r5,r4,#0x38
  0008d10e: vmov s2,r6
  0008d112: vcvt.f32.s32 s0,s0
  0008d116: vcvt.f32.s32 s2,s2
  0008d11a: vst1.32 {d16,d17},[r1]
  0008d11e: add.w r1,r4,#0x58
  0008d122: vst1.32 {d16,d17},[r5]
  0008d126: vst1.32 {d16,d17},[r1]
  0008d12a: add.w r1,r4,#0x48
  0008d12e: vst1.32 {d16,d17},[r1]
  0008d132: ldr r1,[r4,#0x30]
  0008d134: cmp r1,#0x3
  0008d136: bhi 0x0008d18e
  0008d138: vmov s6,r0
  0008d13c: vmov s4,r8
  0008d140: vdiv.f32 s2,s0,s2
  0008d144: vdiv.f32 s4,s4,s6
  0008d148: vmov.f32 s6,0x3f800000
  0008d14c: vdiv.f32 s0,s6,s4
  0008d150: tbb [pc,r1]
  0008d158: vdiv.f32 s2,s0,s2
  0008d15c: vneg.f32 s0,s0
  0008d160: b 0x0008d16a
  0008d162: vdiv.f32 s2,s0,s2
  0008d166: vneg.f32 s2,s2
  0008d16a: vstr.32 s0,[r4,#0x48]
  0008d16e: vstr.32 s2,[r4,#0x3c]
  0008d172: b 0x0008d18e
  0008d174: vdiv.f32 s2,s0,s2
  0008d178: b 0x0008d186
  0008d17a: vdiv.f32 s2,s0,s2
  0008d17e: vneg.f32 s0,s0
  0008d182: vneg.f32 s2,s2
  0008d186: vstr.32 s0,[r4,#0x4c]
  0008d18a: vstr.32 s2,[r4,#0x38]
  0008d18e: ldr r0,[0x0008d23c]
  0008d190: ldr r1,[0x0008d234]
  0008d192: add r0,pc
  0008d194: ldr r0,[r0,#0x0]
  0008d196: vldr.32 s0,[r0]
  0008d19a: ldr r0,[0x0008d240]
  0008d19c: vcvt.f64.f32 d16,s0
  0008d1a0: add r0,pc
  0008d1a2: ldr r0,[r0,#0x0]
  0008d1a4: vadd.f64 d16,d16,d16
  0008d1a8: vldr.32 s2,[r0]
  0008d1ac: ldr r0,[0x0008d244]
  0008d1ae: vcvt.f64.f32 d17,s2
  0008d1b2: str r1,[r4,#0x64]
  0008d1b4: add r0,pc
  0008d1b6: ldr r0,[r0,#0x0]
  0008d1b8: vsub.f32 s4,s2,s0
  0008d1bc: vmul.f64 d16,d16,d17
  0008d1c0: vcvt.f64.f32 d17,s4
  0008d1c4: vdiv.f64 d16,d16,d17
  0008d1c8: vadd.f32 s0,s0,s2
  0008d1cc: vcvt.f32.f64 s2,d16
  0008d1d0: vdiv.f32 s0,s0,s4
  0008d1d4: vstr.32 s0,[r4,#0x60]
  0008d1d8: vstr.32 s2,[r4,#0x70]
  0008d1dc: ldrb r0,[r0,#0x0]
  0008d1de: cbz r0,0x0008d1f4
  0008d1e0: ldr r0,[r4,#0x34]
  0008d1e2: mov r1,r5
  0008d1e4: vpop {d8}
  0008d1e8: pop.w {r8,r9,r11}
  0008d1ec: pop.w {r4,r5,r6,r7,lr}
  0008d1f0: b.w 0x001ab3c8
  0008d1f4: ldr r0,[0x0008d248]
  0008d1f6: add r0,pc
  0008d1f8: ldr r4,[r0,#0x0]
  0008d1fa: movw r0,#0x1702
  0008d1fe: blx r4
  0008d200: blx 0x0006fe8c
  0008d204: ldr r1,[0x0008d234]
  0008d206: mov.w r0,#0x3f800000
  0008d20a: mov.w r2,#0x3f800000
  0008d20e: blx 0x0006fe98
  0008d212: movw r0,#0x1701
  0008d216: blx r4
  0008d218: mov r0,r5
  0008d21a: blx 0x0006fea4
  0008d21e: mov.w r0,#0x1700
  0008d222: blx r4
  0008d224: vpop {d8}
  0008d228: pop.w {r8,r9,r11}
  0008d22c: pop.w {r4,r5,r6,r7,lr}
  0008d230: b.w 0x001ab3d8
```
