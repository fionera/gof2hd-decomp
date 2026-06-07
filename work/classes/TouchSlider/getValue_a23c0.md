# TouchSlider::getValue
elf 0xa23c0 body 46
Sig: undefined __thiscall getValue(TouchSlider * this)

## decompile
```c

/* TouchSlider::getValue() */

float __thiscall TouchSlider::getValue(TouchSlider *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0x1c) - *(int *)(this + 0x14),
                                     (byte)(in_fpscr >> 0x16) & 3);
  fVar2 = (float)VectorSignedToFloat((*(int *)(this + 8) - *(int *)(this + 0x14) / 2) - *(int *)this
                                     ,(byte)(in_fpscr >> 0x16) & 3);
  return fVar2 / fVar1;
}

```

## target disasm
```
  000b23c0: ldr r1,[r0,#0x0]
  000b23c2: ldr r2,[r0,#0x8]
  000b23c4: ldr r3,[r0,#0x14]
  000b23c6: ldr r0,[r0,#0x1c]
  000b23c8: subs r0,r0,r3
  000b23ca: vmov s0,r0
  000b23ce: add.w r0,r3,r3, lsr #0x1f
  000b23d2: sub.w r0,r2,r0, asr #0x1
  000b23d6: vcvt.f32.s32 s0,s0
  000b23da: subs r0,r0,r1
  000b23dc: vmov s2,r0
  000b23e0: vcvt.f32.s32 s2,s2
  000b23e4: vdiv.f32 s0,s2,s0
  000b23e8: vmov r0,s0
  000b23ec: bx lr
```
