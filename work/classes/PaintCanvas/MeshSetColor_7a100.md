# PaintCanvas::MeshSetColor
elf 0x7a100 body 126
Sig: undefined __stdcall MeshSetColor(uint param_1, ushort param_2, uint param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetColor(unsigned int, unsigned short, unsigned int) */

void AbyssEngine::PaintCanvas::MeshSetColor(uint param_1,ushort param_2,uint param_3)

{
  int iVar1;
  float *pfVar2;
  uint in_r3;
  uint in_fpscr;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  
  if ((uint)param_2 < *(uint *)(param_1 + 0x24)) {
    iVar1 = *(int *)(*(int *)(param_1 + 0x28) + (uint)param_2 * 4);
    if (*(ushort *)(iVar1 + 2) <= param_3) {
      return;
    }
    dVar3 = (double)VectorUnsignedToFloat((in_r3 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
    dVar3 = dVar3 / DAT_0008a180;
    dVar4 = (double)VectorUnsignedToFloat(in_r3 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
    dVar5 = (double)VectorUnsignedToFloat((in_r3 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
    dVar5 = dVar5 / DAT_0008a180;
    dVar6 = (double)VectorUnsignedToFloat(in_r3 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
    pfVar2 = (float *)(*(int *)(iVar1 + 0xc) + param_3 * 0x10);
    dVar6 = dVar6 / DAT_0008a180;
    *pfVar2 = (float)(dVar4 / DAT_0008a180);
    pfVar2[1] = (float)dVar3;
    pfVar2[2] = (float)dVar5;
    pfVar2[3] = (float)dVar6;
  }
  return;
}

```

## target disasm
```
  0008a100: ldr.w r12,[r0,#0x24]
  0008a104: cmp r12,r1
  0008a106: bls 0x0008a17c
  0008a108: ldr r0,[r0,#0x28]
  0008a10a: ldr.w r0,[r0,r1,lsl #0x2]
  0008a10e: ldrh r1,[r0,#0x2]
  0008a110: cmp r1,r2
  0008a112: it ls
  0008a114: bx.ls lr
  0008a116: ubfx r1,r3,#0x10,#0x8
  0008a11a: vldr.64 d19,[pc,#0x64]
  0008a11e: vmov s0,r1
  0008a122: lsrs r1,r3,#0x18
  0008a124: vcvt.f64.u32 d16,s0
  0008a128: vmov s0,r1
  0008a12c: vdiv.f64 d16,d16,d19
  0008a130: vcvt.f64.u32 d17,s0
  0008a134: ubfx r1,r3,#0x8,#0x8
  0008a138: vmov s0,r1
  0008a13c: uxtb r1,r3
  0008a13e: vdiv.f64 d17,d17,d19
  0008a142: vcvt.f64.u32 d18,s0
  0008a146: vmov s0,r1
  0008a14a: vdiv.f64 d18,d18,d19
  0008a14e: vcvt.f64.u32 d20,s0
  0008a152: ldr r0,[r0,#0xc]
  0008a154: add.w r0,r0,r2, lsl #0x4
  0008a158: vdiv.f64 d19,d20,d19
  0008a15c: vcvt.f32.f64 s2,d17
  0008a160: vcvt.f32.f64 s0,d16
  0008a164: vcvt.f32.f64 s4,d18
  0008a168: vcvt.f32.f64 s6,d19
  0008a16c: vstr.32 s2,[r0]
  0008a170: vstr.32 s0,[r0,#0x4]
  0008a174: vstr.32 s4,[r0,#0x8]
  0008a178: vstr.32 s6,[r0,#0xc]
  0008a17c: bx lr
```
