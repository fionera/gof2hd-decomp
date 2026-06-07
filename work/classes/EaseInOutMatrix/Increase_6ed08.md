# EaseInOutMatrix::Increase
elf 0x6ed08 body 44
Sig: undefined __stdcall Increase(float param_1)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::Increase(float) */

void AbyssEngine::EaseInOutMatrix::Increase(float param_1)

{
  int in_r0;
  float in_r1;
  undefined4 in_s1;
  undefined8 uVar1;
  undefined4 in_s5;
  
  uVar1 = FloatVectorMin(CONCAT44(in_s1,*(float *)(in_r0 + 0x74) +
                                        (in_r1 * 0.5) / *(float *)(in_r0 + 0xf0)),
                         CONCAT44(in_s5,0x3fa00000),2,0x20);
  *(int *)(in_r0 + 0x74) = (int)uVar1;
  (*(code *)(DAT_001ab0b4 + 0x1ab0b8))();
  return;
}

```

## target disasm
```
  0007ed08: vmov.f32 s0,0x3f000000
  0007ed0c: vldr.32 s4,[r0,#0xf0]
  0007ed10: vmov s2,r1
  0007ed14: vmul.f32 s0,s2,s0
  0007ed18: vldr.32 s2,[r0,#0x74]
  0007ed1c: vdiv.f32 s0,s0,s4
  0007ed20: vmov.f32 s4,0x3fa00000
  0007ed24: vadd.f32 s0,s2,s0
  0007ed28: vmin.f32 d0,d0,d2
  0007ed2c: vstr.32 s0,[r0,#0x74]
  0007ed30: b.w 0x001ab0a8
```
