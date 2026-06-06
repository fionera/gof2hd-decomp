# AbyssEngine::AEMath::Min
elf 0x86a8c body 28
Sig: undefined __stdcall Min(float param_1, float param_2)

## decompile
```c

/* AbyssEngine::AEMath::Min(float, float) */

float AbyssEngine::AEMath::Min(float param_1,float param_2)

{
  float in_r0;
  float in_r1;
  
  if ((int)((uint)(in_r0 < in_r1) << 0x1f) < 0) {
    in_r1 = in_r0;
  }
  return in_r1;
}

```
## target disasm
```
  00096a8c: vmov s0,r0
  00096a90: vmov s2,r1
  00096a94: vcmpe.f32 s0,s2
  00096a98: vmrs apsr,fpscr
  00096a9c: it mi
  00096a9e: vmov.mi.f32 s2,s0
  00096aa2: vmov r0,s2
  00096aa6: bx lr
```
