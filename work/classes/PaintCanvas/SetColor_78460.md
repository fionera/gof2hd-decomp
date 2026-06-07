# PaintCanvas::SetColor
elf 0x78460 body 128
Sig: undefined __stdcall SetColor(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetColor(unsigned int) */

void AbyssEngine::PaintCanvas::SetColor(uint param_1)

{
  uint in_r1;
  uint in_fpscr;
  float in_s1;
  float in_s3;
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  
  dVar1 = (double)VectorUnsignedToFloat(in_r1 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
  dVar1 = dVar1 / DAT_000884e0;
  dVar2 = (double)VectorUnsignedToFloat((in_r1 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
  dVar2 = dVar2 / DAT_000884e0;
  dVar3 = (double)VectorUnsignedToFloat((in_r1 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
  dVar3 = dVar3 / DAT_000884e0;
  dVar4 = (double)VectorUnsignedToFloat(in_r1 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
  dVar4 = dVar4 / DAT_000884e0;
  *(float *)(param_1 + 0x1fc) = (float)dVar1;
  *(float *)(param_1 + 0x200) = (float)dVar2;
  *(float *)(param_1 + 0x204) = (float)dVar3;
  *(float *)(param_1 + 0x208) = (float)dVar4;
  Engine::SetColor((float)dVar1,in_s1,(float)dVar2,in_s3);
  return;
}

```

## target disasm
```
  00088460: push {r5,r6,r7,lr}
  00088462: add r7,sp,#0x8
  00088464: lsrs r2,r1,#0x18
  00088466: vldr.64 d19,[pc,#0x78]
  0008846a: vmov s0,r2
  0008846e: vcvt.f64.u32 d16,s0
  00088472: ubfx r2,r1,#0x10,#0x8
  00088476: vmov s0,r2
  0008847a: vdiv.f64 d16,d16,d19
  0008847e: vcvt.f64.u32 d17,s0
  00088482: ubfx r2,r1,#0x8,#0x8
  00088486: uxtb r1,r1
  00088488: vmov s0,r2
  0008848c: vdiv.f64 d17,d17,d19
  00088490: vcvt.f64.u32 d18,s0
  00088494: vdiv.f64 d18,d18,d19
  00088498: vmov s0,r1
  0008849c: vcvt.f32.f64 s2,d17
  000884a0: vcvt.f64.u32 d20,s0
  000884a4: ldr.w r12,[r0,#0x34]
  000884a8: vdiv.f64 d19,d20,d19
  000884ac: vcvt.f32.f64 s0,d16
  000884b0: vcvt.f32.f64 s4,d18
  000884b4: vmov r1,s0
  000884b8: vstr.32 s0,[r0,#0x1fc]
  000884bc: vmov r2,s2
  000884c0: vstr.32 s2,[r0,#0x200]
  000884c4: vmov r3,s4
  000884c8: vstr.32 s4,[r0,#0x204]
  000884cc: vcvt.f32.f64 s6,d19
  000884d0: vstr.32 s6,[r0,#0x208]
  000884d4: mov r0,r12
  000884d6: vstr.32 s6,[sp]
  000884da: blx 0x0006fad8
  000884de: pop {r2,r3,r7,pc}
```
