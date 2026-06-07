# EaseInOut::Decrease
elf 0x718a8 body 70
Sig: undefined __thiscall Decrease(EaseInOut * this, float param_1)

## decompile
```c

/* AbyssEngine::EaseInOut::Decrease(float) */

void __thiscall AbyssEngine::EaseInOut::Decrease(EaseInOut *this,float param_1)

{
  float in_r1;
  float fVar1;
  
  fVar1 = (float)((double)*(float *)(this + 8) + (double)in_r1 * DAT_000818f0 * DAT_000818f8);
  if ((int)((uint)((double)fVar1 < DAT_00081900) << 0x1f) < 0) {
    fVar1 = DAT_00081908;
  }
  *(float *)(this + 8) = fVar1;
  (*(code *)(DAT_001ab0f4 + 0x1ab0f8))();
  return;
}

```

## target disasm
```
  000818a8: vmov s0,r1
  000818ac: vldr.64 d17,[pc,#0x40]
  000818b0: vldr.64 d18,[pc,#0x44]
  000818b4: vcvt.f64.f32 d16,s0
  000818b8: vmul.f64 d16,d16,d17
  000818bc: vldr.32 s0,[r0,#0x8]
  000818c0: vldr.32 s2,[pc,#0x44]
  000818c4: vcvt.f64.f32 d17,s0
  000818c8: vmla.f64 d17,d16,d18
  000818cc: vldr.64 d16,[pc,#0x30]
  000818d0: vcvt.f32.f64 s0,d17
  000818d4: vcvt.f64.f32 d17,s0
  000818d8: vcmpe.f64 d17,d16
  000818dc: vmrs apsr,fpscr
  000818e0: it mi
  000818e2: vmov.mi.f32 s0,s2
  000818e6: vstr.32 s0,[r0,#0x8]
  000818ea: b.w 0x001ab0e8
```
