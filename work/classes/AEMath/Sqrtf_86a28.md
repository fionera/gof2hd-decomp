# AbyssEngine::AEMath::Sqrtf
elf 0x86a28 body 30
Sig: undefined __stdcall Sqrtf(float param_1)

## decompile
```c

/* AbyssEngine::AEMath::Sqrtf(float) */

float AbyssEngine::AEMath::Sqrtf(float param_1)

{
  float in_r0;
  float fVar1;
  
  if (!NAN(SQRT(in_r0))) {
    return SQRT(in_r0);
  }
  fVar1 = (float)(*(code *)(DAT_001ab144 + 0x1ab148))();
  return fVar1;
}

```
## target disasm
```
  00096a28: vmov s0,r0
  00096a2c: vsqrt.f32 s0,s0
  00096a30: vcmpe.f32 s0,s0
  00096a34: vmrs apsr,fpscr
  00096a38: it vs
  00096a3a: b.vs.w 0x001ab138
  00096a3e: vmov r0,s0
  00096a42: bx lr
  001ab138: bx pc
```
