# PaintCanvas::SetColor
elf 0x7c938 body 118
Sig: undefined __stdcall SetColor(uchar param_1, uchar param_2, uchar param_3, uchar param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetColor(unsigned char, unsigned char, unsigned char, unsigned char) */

void AbyssEngine::PaintCanvas::SetColor(uchar param_1,uchar param_2,uchar param_3,uchar param_4)

{
  uint uVar1;
  uint in_fpscr;
  float in_s1;
  float in_s3;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  undefined4 in_stack_00000000;
  
  uVar1 = (uint)param_1;
  dVar2 = (double)VectorUnsignedToFloat((uint)param_2,(byte)(in_fpscr >> 0x16) & 3);
  dVar2 = dVar2 / DAT_0008c9b0;
  dVar3 = (double)VectorUnsignedToFloat((uint)param_3,(byte)(in_fpscr >> 0x16) & 3);
  dVar3 = dVar3 / DAT_0008c9b0;
  dVar4 = (double)VectorUnsignedToFloat((uint)param_4,(byte)(in_fpscr >> 0x16) & 3);
  dVar4 = dVar4 / DAT_0008c9b0;
  dVar5 = (double)VectorUnsignedToFloat(in_stack_00000000,(byte)(in_fpscr >> 0x16) & 3);
  dVar5 = dVar5 / DAT_0008c9b0;
  *(float *)(uVar1 + 0x1fc) = (float)dVar2;
  *(float *)(uVar1 + 0x200) = (float)dVar3;
  *(float *)(uVar1 + 0x204) = (float)dVar4;
  *(float *)(uVar1 + 0x208) = (float)dVar5;
  Engine::SetColor((float)dVar2,in_s1,(float)dVar3,in_s3);
  return;
}

```

## target disasm
```
  0008c938: push {r5,r6,r7,lr}
  0008c93a: add r7,sp,#0x8
  0008c93c: vmov s0,r1
  0008c940: vldr.64 d17,[pc,#0x6c]
  0008c944: ldr r1,[r7,#0x8]
  0008c946: vcvt.f64.u32 d16,s0
  0008c94a: vmov s0,r2
  0008c94e: vdiv.f64 d16,d16,d17
  0008c952: vcvt.f64.u32 d18,s0
  0008c956: vmov s0,r3
  0008c95a: vdiv.f64 d18,d18,d17
  0008c95e: vcvt.f64.u32 d19,s0
  0008c962: vdiv.f64 d19,d19,d17
  0008c966: vmov s0,r1
  0008c96a: vcvt.f32.f64 s2,d18
  0008c96e: vcvt.f64.u32 d20,s0
  0008c972: ldr.w r12,[r0,#0x34]
  0008c976: vdiv.f64 d17,d20,d17
  0008c97a: vcvt.f32.f64 s0,d16
  0008c97e: vcvt.f32.f64 s4,d19
  0008c982: vmov r1,s0
  0008c986: vstr.32 s0,[r0,#0x1fc]
  0008c98a: vmov r2,s2
  0008c98e: vstr.32 s2,[r0,#0x200]
  0008c992: vmov r3,s4
  0008c996: vstr.32 s4,[r0,#0x204]
  0008c99a: vcvt.f32.f64 s6,d17
  0008c99e: vstr.32 s6,[r0,#0x208]
  0008c9a2: mov r0,r12
  0008c9a4: vstr.32 s6,[sp]
  0008c9a8: blx 0x0006fad8
  0008c9ac: pop {r2,r3,r7,pc}
```
