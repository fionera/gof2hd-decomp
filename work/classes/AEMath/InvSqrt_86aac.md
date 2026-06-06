# AbyssEngine::AEMath::InvSqrt
elf 0x86aac body 44
Sig: undefined __stdcall InvSqrt(float param_1)

## decompile
```c

/* AbyssEngine::AEMath::InvSqrt(float) */

float AbyssEngine::AEMath::InvSqrt(float param_1)

{
  float in_r0;
  float fVar1;
  
  fVar1 = (float)(DAT_00096ad8 - ((int)in_r0 >> 1));
  return (in_r0 * -0.5 * fVar1 * fVar1 + 1.5) * fVar1;
}

```
## target disasm
```
  00096aac: vmov.f32 s0,0xbf000000
  00096ab0: ldr r1,[0x00096ad8]
  00096ab2: vmov s2,r0
  00096ab6: sub.w r0,r1,r0, asr #0x1
  00096aba: vmov.f32 s4,0x3fc00000
  00096abe: vmul.f32 s0,s2,s0
  00096ac2: vmov s2,r0
  00096ac6: vmul.f32 s0,s0,s2
  00096aca: vmla.f32 s4,s0,s2
  00096ace: vmul.f32 s0,s4,s2
  00096ad2: vmov r0,s0
  00096ad6: bx lr
```
