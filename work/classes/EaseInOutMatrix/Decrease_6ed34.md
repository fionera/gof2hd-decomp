# EaseInOutMatrix::Decrease
elf 0x6ed34 body 44
Sig: undefined __thiscall Decrease(EaseInOutMatrix * this, float param_1)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::Decrease(float) */

void __thiscall AbyssEngine::EaseInOutMatrix::Decrease(EaseInOutMatrix *this,float param_1)

{
  float in_r1;
  undefined4 in_s1;
  undefined8 uVar1;
  undefined4 in_s5;
  
  uVar1 = FloatVectorMax(CONCAT44(in_s1,*(float *)(this + 0x74) +
                                        (in_r1 * -0.5) / *(float *)(this + 0xf0)),
                         CONCAT44(in_s5,0x3f400000),2,0x20);
  *(int *)(this + 0x74) = (int)uVar1;
  (*(code *)(DAT_001ab0b4 + 0x1ab0b8))();
  return;
}

```

## target disasm
```
  0007ed34: vmov.f32 s0,0xbf000000
  0007ed38: vldr.32 s4,[r0,#0xf0]
  0007ed3c: vmov s2,r1
  0007ed40: vmul.f32 s0,s2,s0
  0007ed44: vldr.32 s2,[r0,#0x74]
  0007ed48: vdiv.f32 s0,s0,s4
  0007ed4c: vmov.f32 s4,0x3f400000
  0007ed50: vadd.f32 s0,s2,s0
  0007ed54: vmax.f32 d0,d0,d2
  0007ed58: vstr.32 s0,[r0,#0x74]
  0007ed5c: b.w 0x001ab0a8
```
