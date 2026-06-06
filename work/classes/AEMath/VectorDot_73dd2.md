# AbyssEngine::AEMath::VectorDot
elf 0x73dd2 body 42
Sig: undefined __stdcall VectorDot(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::VectorDot(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector
   const&) */

float AbyssEngine::AEMath::VectorDot(Vector *param_1,Vector *param_2)

{
  return *(float *)(param_1 + 4) * *(float *)(param_2 + 4) + *(float *)param_1 * *(float *)param_2 +
         *(float *)(param_1 + 8) * *(float *)(param_2 + 8);
}

```
## target disasm
```
  00083dd2: vldr.32 s2,[r1,#0x4]
  00083dd6: vldr.32 s8,[r0,#0x4]
  00083dda: vldr.32 s0,[r1]
  00083dde: vmul.f32 s2,s8,s2
  00083de2: vldr.32 s6,[r0]
  00083de6: vldr.32 s4,[r1,#0x8]
  00083dea: vldr.32 s10,[r0,#0x8]
  00083dee: vmla.f32 s2,s6,s0
  00083df2: vmla.f32 s2,s10,s4
  00083df6: vmov r0,s2
  00083dfa: bx lr
```
