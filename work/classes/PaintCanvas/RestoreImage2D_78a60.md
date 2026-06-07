# PaintCanvas::RestoreImage2D
elf 0x78a60 body 204
Sig: undefined __thiscall RestoreImage2D(PaintCanvas * this, Image2D * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::RestoreImage2D(AbyssEngine::Image2D*) */

void __thiscall AbyssEngine::PaintCanvas::RestoreImage2D(PaintCanvas *this,Image2D *param_1)

{
  undefined4 uVar1;
  float *pfVar2;
  undefined4 *puVar3;
  int iVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  double dVar11;
  double dVar12;
  
  iVar4 = *(int *)param_1;
  puVar3 = *(undefined4 **)(iVar4 + 4);
  param_1[0x14] = (Image2D)0x0;
  *puVar3 = 0;
  puVar3[1] = 0;
  puVar3[2] = 0;
  fVar5 = (float)VectorUnsignedToFloat
                           ((uint)*(ushort *)(param_1 + 0x10),(byte)(in_fpscr >> 0x16) & 3);
  puVar3[4] = 0;
  puVar3[5] = 0;
  puVar3[3] = fVar5;
  puVar3[6] = fVar5;
  fVar6 = (float)VectorUnsignedToFloat
                           ((uint)*(ushort *)(param_1 + 0x12),(byte)(in_fpscr >> 0x16) & 3);
  puVar3[8] = 0;
  puVar3[9] = 0;
  puVar3[0xb] = 0;
  puVar3[7] = fVar6;
  puVar3[10] = fVar6;
  uVar1 = DAT_00088b2c;
  dVar11 = (double)VectorUnsignedToFloat
                             ((uint)*(ushort *)(param_1 + 10),(byte)(in_fpscr >> 0x16) & 3);
  dVar12 = (double)VectorUnsignedToFloat
                             ((uint)*(ushort *)(param_1 + 8),(byte)(in_fpscr >> 0x16) & 3);
  fVar7 = (float)VectorUnsignedToFloat
                           ((uint)*(ushort *)(param_1 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
  fVar9 = (float)VectorUnsignedToFloat
                           ((uint)*(ushort *)(param_1 + 0xe),(byte)(in_fpscr >> 0x16) & 3);
  pfVar2 = *(float **)(iVar4 + 8);
  fVar8 = fVar7 * (float)(1.0 / dVar12);
  fVar10 = fVar9 * (float)(1.0 / dVar11);
  fVar6 = (fVar6 + fVar9) * (float)(1.0 / dVar11);
  fVar5 = (fVar5 + fVar7) * (float)(1.0 / dVar12);
  *pfVar2 = fVar8;
  pfVar2[1] = fVar10;
  pfVar2[2] = fVar5;
  pfVar2[3] = fVar10;
  pfVar2[4] = fVar5;
  pfVar2[5] = fVar6;
  pfVar2[6] = fVar8;
  pfVar2[7] = fVar6;
  puVar3 = *(undefined4 **)(iVar4 + 0x2c);
  *puVar3 = 0x20000;
  puVar3[1] = 1;
  puVar3[2] = uVar1;
  return;
}

```

## target disasm
```
  00088a60: ldr.w r12,[r1,#0x0]
  00088a64: movs r3,#0x0
  00088a66: vmov.f64 d17,0x3ff0000000000000
  00088a6a: ldr.w r2,[r12,#0x4]
  00088a6e: strb r3,[r1,#0x14]
  00088a70: strd r3,r3,[r2,#0x0]
  00088a74: str r3,[r2,#0x8]
  00088a76: ldrh r0,[r1,#0x10]
  00088a78: vmov s0,r0
  00088a7c: vcvt.f32.u32 s0,s0
  00088a80: strd r3,r3,[r2,#0x10]
  00088a84: vstr.32 s0,[r2,#0xc]
  00088a88: vstr.32 s0,[r2,#0x18]
  00088a8c: ldrh r0,[r1,#0x12]
  00088a8e: vmov s2,r0
  00088a92: vcvt.f32.u32 s2,s2
  00088a96: strd r3,r3,[r2,#0x20]
  00088a9a: str r3,[r2,#0x2c]
  00088a9c: vstr.32 s2,[r2,#0x1c]
  00088aa0: vstr.32 s2,[r2,#0x28]
  00088aa4: mov.w r2,#0x20000
  00088aa8: ldrh r0,[r1,#0xa]
  00088aaa: vmov s4,r0
  00088aae: vcvt.f64.u32 d16,s4
  00088ab2: ldrh r0,[r1,#0x8]
  00088ab4: vmov s4,r0
  00088ab8: vdiv.f64 d16,d17,d16
  00088abc: vcvt.f64.u32 d18,s4
  00088ac0: ldrh r0,[r1,#0xc]
  00088ac2: ldrh r1,[r1,#0xe]
  00088ac4: vdiv.f64 d17,d17,d18
  00088ac8: vmov s4,r0
  00088acc: vmov s6,r1
  00088ad0: ldr r1,[0x00088b2c]
  00088ad2: vcvt.f32.u32 s4,s4
  00088ad6: vcvt.f32.u32 s6,s6
  00088ada: ldr.w r0,[r12,#0x8]
  00088ade: vcvt.f32.f64 s8,d16
  00088ae2: vadd.f32 s2,s2,s6
  00088ae6: vcvt.f32.f64 s10,d17
  00088aea: vadd.f32 s0,s0,s4
  00088aee: vmul.f32 s4,s4,s10
  00088af2: vmul.f32 s6,s6,s8
  00088af6: vmul.f32 s2,s2,s8
  00088afa: vmul.f32 s0,s0,s10
  00088afe: vstr.32 s4,[r0]
  00088b02: vstr.32 s6,[r0,#0x4]
  00088b06: vstr.32 s0,[r0,#0x8]
  00088b0a: vstr.32 s6,[r0,#0xc]
  00088b0e: vstr.32 s0,[r0,#0x10]
  00088b12: vstr.32 s2,[r0,#0x14]
  00088b16: vstr.32 s4,[r0,#0x18]
  00088b1a: vstr.32 s2,[r0,#0x1c]
  00088b1e: ldr.w r0,[r12,#0x2c]
  00088b22: str r2,[r0,#0x0]
  00088b24: movs r2,#0x1
  00088b26: str r2,[r0,#0x4]
  00088b28: str r1,[r0,#0x8]
  00088b2a: bx lr
```
