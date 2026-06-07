# TouchSlider::setValue
elf 0xa23ee body 52
Sig: undefined __thiscall setValue(TouchSlider * this, float param_1)

## decompile
```c

/* TouchSlider::setValue(float) */

void __thiscall TouchSlider::setValue(TouchSlider *this,float param_1)

{
  float in_r1;
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0x1c) - *(int *)(this + 0x14),
                                     (byte)(in_fpscr >> 0x16) & 3);
  fVar2 = (float)VectorSignedToFloat(*(int *)this + *(int *)(this + 0x14) / 2,
                                     (byte)(in_fpscr >> 0x16) & 3);
  *(int *)(this + 8) = (int)(fVar2 + fVar1 * in_r1);
  return;
}

```

## target disasm
```
  000b23ee: ldr r3,[r0,#0x14]
  000b23f0: vmov s4,r1
  000b23f4: ldr r2,[r0,#0x1c]
  000b23f6: ldr.w r12,[r0,#0x0]
  000b23fa: subs r2,r2,r3
  000b23fc: vmov s0,r2
  000b2400: add.w r2,r3,r3, lsr #0x1f
  000b2404: add.w r2,r12,r2, asr #0x1
  000b2408: vcvt.f32.s32 s0,s0
  000b240c: vmov s2,r2
  000b2410: vcvt.f32.s32 s2,s2
  000b2414: vmla.f32 s2,s0,s4
  000b2418: vcvt.s32.f32 s0,s2
  000b241c: vstr.32 s0,[r0,#0x8]
  000b2420: bx lr
```
