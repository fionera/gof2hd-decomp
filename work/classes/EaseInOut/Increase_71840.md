# EaseInOut::Increase
elf 0x71840 body 70
Sig: undefined __stdcall Increase(float param_1)

## decompile
```c

/* AbyssEngine::EaseInOut::Increase(float) */

void AbyssEngine::EaseInOut::Increase(float param_1)

{
  int in_r0;
  float in_r1;
  float fVar1;
  
  fVar1 = (float)((double)*(float *)(in_r0 + 8) + (double)in_r1 * DAT_00081888 * DAT_00081890);
  if (DAT_00081898 < (double)fVar1) {
    fVar1 = DAT_000818a0;
  }
  *(float *)(in_r0 + 8) = fVar1;
  (*(code *)(DAT_001ab0f4 + 0x1ab0f8))();
  return;
}

```

## target disasm
```
  00081840: vmov s0,r1
  00081844: vldr.64 d17,[pc,#0x40]
  00081848: vldr.64 d18,[pc,#0x44]
  0008184c: vcvt.f64.f32 d16,s0
  00081850: vmul.f64 d16,d16,d17
  00081854: vldr.32 s0,[r0,#0x8]
  00081858: vldr.32 s2,[pc,#0x44]
  0008185c: vcvt.f64.f32 d17,s0
  00081860: vmla.f64 d17,d16,d18
  00081864: vldr.64 d16,[pc,#0x30]
  00081868: vcvt.f32.f64 s0,d17
  0008186c: vcvt.f64.f32 d17,s0
  00081870: vcmpe.f64 d17,d16
  00081874: vmrs apsr,fpscr
  00081878: it gt
  0008187a: vmov.gt.f32 s0,s2
  0008187e: vstr.32 s0,[r0,#0x8]
  00081882: b.w 0x001ab0e8
```
