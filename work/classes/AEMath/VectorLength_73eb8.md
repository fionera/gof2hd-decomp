# AbyssEngine::AEMath::VectorLength
elf 0x73eb8 body 52
Sig: undefined __stdcall VectorLength(Vector * param_1)

## decompile
```c

/* AbyssEngine::AEMath::VectorLength(AbyssEngine::AEMath::Vector const&) */

float AbyssEngine::AEMath::VectorLength(Vector *param_1)

{
  float fVar1;
  float fVar2;
  
  fVar1 = *(float *)(param_1 + 4) * *(float *)(param_1 + 4) + *(float *)param_1 * *(float *)param_1
          + *(float *)(param_1 + 8) * *(float *)(param_1 + 8);
  fVar2 = SQRT(fVar1);
  if (NAN(fVar2)) {
    fVar1 = (float)(*(code *)(DAT_001ab144 + 0x1ab148))(fVar1);
    return fVar1;
  }
  return fVar2;
}

```
## target disasm
```
  00083eb8: vldr.32 s0,[r0,#0x4]
  00083ebc: vldr.32 s2,[r0]
  00083ec0: vmul.f32 s0,s0,s0
  00083ec4: vldr.32 s4,[r0,#0x8]
  00083ec8: vmla.f32 s0,s2,s2
  00083ecc: vmla.f32 s0,s4,s4
  00083ed0: vsqrt.f32 s2,s0
  00083ed4: vcmpe.f32 s2,s2
  00083ed8: vmrs apsr,fpscr
  00083edc: itt vs
  00083ede: vmov.vs r0,s0
  00083ee2: b.vs.w 0x001ab138
  00083ee6: vmov r0,s2
  00083eea: bx lr
```
