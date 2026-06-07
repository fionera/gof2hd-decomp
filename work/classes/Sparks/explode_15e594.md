# Sparks::explode
elf 0x15e594 body 42
Sig: undefined __thiscall explode(Sparks * this, Vector * param_1)

## decompile
```c

/* Sparks::explode(AbyssEngine::AEMath::Vector const&) */

void __thiscall Sparks::explode(Sparks *this,Vector *param_1)

{
  (*(code *)(DAT_001ac604 + 0x1ac608))
            (this,(int)*(float *)param_1,(int)*(float *)(param_1 + 4),(int)*(float *)(param_1 + 8));
  return;
}

```

## target disasm
```
  0016e594: vldr.32 s0,[r1]
  0016e598: vldr.32 s2,[r1,#0x4]
  0016e59c: vldr.32 s4,[r1,#0x8]
  0016e5a0: vcvt.s32.f32 s0,s0
  0016e5a4: vcvt.s32.f32 s2,s2
  0016e5a8: vcvt.s32.f32 s4,s4
  0016e5ac: vmov r1,s0
  0016e5b0: vmov r2,s2
  0016e5b4: vmov r3,s4
  0016e5b8: b.w 0x001ac5f8
  001ac5f8: bx pc
```
