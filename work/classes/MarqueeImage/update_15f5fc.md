# MarqueeImage::update
elf 0x15f5fc body 84
Sig: undefined __thiscall update(MarqueeImage * this, int param_1)

## decompile
```c

/* MarqueeImage::update(int) */

void __thiscall MarqueeImage::update(MarqueeImage *this,int param_1)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  float fVar3;
  
  fVar1 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = (fVar1 / DAT_0016f650) * *(float *)(this + 0x1c);
  fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 4),(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = *(float *)(this + 0x18) + fVar3;
  *(float *)(this + 0x18) = fVar1;
  if (fVar2 < fVar1) {
    fVar1 = (fVar1 - fVar2) + fVar3 * 0.5;
    *(float *)(this + 0x18) = fVar1;
  }
  *(int *)(this + 0x20) = (int)(fVar2 - fVar1);
  return;
}

```

## target disasm
```
  0016f5fc: vmov s0,r1
  0016f600: vldr.32 s2,[pc,#0x4c]
  0016f604: vcvt.f32.s32 s0,s0
  0016f608: vldr.32 s4,[r0,#0x1c]
  0016f60c: vldr.32 s6,[r0,#0x18]
  0016f610: vdiv.f32 s0,s0,s2
  0016f614: vmul.f32 s4,s0,s4
  0016f618: vldr.32 s2,[r0,#0x4]
  0016f61c: vcvt.f32.s32 s2,s2
  0016f620: vadd.f32 s0,s6,s4
  0016f624: vcmpe.f32 s0,s2
  0016f628: vstr.32 s0,[r0,#0x18]
  0016f62c: vmrs apsr,fpscr
  0016f630: itttt gt
  0016f632: vmov.gt.f32 s6,0x3f000000
  0016f636: vsub.gt.f32 s0,s0,s2
  0016f63a: vmla.gt.f32 s0,s4,s6
  0016f63e: vstr.gt.32 s0,[r0,#0x18]
  0016f642: vsub.f32 s0,s2,s0
  0016f646: vcvt.s32.f32 s0,s0
  0016f64a: vstr.32 s0,[r0,#0x20]
  0016f64e: bx lr
```
