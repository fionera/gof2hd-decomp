# TouchSlider::setPosition
elf 0xa232c body 68
Sig: undefined __thiscall setPosition(TouchSlider * this, int param_1, int param_2)

## decompile
```c

/* TouchSlider::setPosition(int, int) */

void __thiscall TouchSlider::setPosition(TouchSlider *this,int param_1,int param_2)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0x1c) - *(int *)(this + 0x14),
                                     (byte)(in_fpscr >> 0x16) & 3);
  fVar2 = (float)VectorSignedToFloat(param_1 + *(int *)(this + 0x14) / 2,
                                     (byte)(in_fpscr >> 0x16) & 3);
  *(int *)this = param_1;
  *(int *)(this + 4) = param_2;
  *(int *)(this + 0xc) = param_2 + *(int *)(this + 0x20) / 2;
  *(int *)(this + 8) = (int)(fVar2 + *(float *)(this + 0x24) * fVar1);
  return;
}

```

## target disasm
```
  000b232c: ldr.w r12,[r0,#0x14]
  000b2330: ldr r3,[r0,#0x1c]
  000b2332: sub.w r3,r3,r12
  000b2336: vmov s0,r3
  000b233a: add.w r3,r12,r12, lsr #0x1f
  000b233e: add.w r3,r1,r3, asr #0x1
  000b2342: vcvt.f32.s32 s0,s0
  000b2346: vmov s2,r3
  000b234a: vcvt.f32.s32 s2,s2
  000b234e: vldr.32 s4,[r0,#0x24]
  000b2352: vmla.f32 s2,s4,s0
  000b2356: vcvt.s32.f32 s0,s2
  000b235a: ldr r3,[r0,#0x20]
  000b235c: strd r1,r2,[r0,#0x0]
  000b2360: add.w r1,r3,r3, lsr #0x1f
  000b2364: add.w r1,r2,r1, asr #0x1
  000b2368: str r1,[r0,#0xc]
  000b236a: vstr.32 s0,[r0,#0x8]
  000b236e: bx lr
```
