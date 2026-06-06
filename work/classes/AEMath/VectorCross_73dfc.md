# AbyssEngine::AEMath::VectorCross
elf 0x73dfc body 62
Sig: undefined __stdcall VectorCross(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::VectorCross(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector
   const&) */

void AbyssEngine::AEMath::VectorCross(Vector *param_1,Vector *param_2)

{
  float *in_r2;
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  
  fVar1 = *in_r2;
  fVar6 = *(float *)(param_2 + 8);
  fVar2 = in_r2[1];
  fVar4 = *(float *)param_2;
  fVar3 = in_r2[2];
  fVar5 = *(float *)(param_2 + 4);
  *(float *)param_1 = fVar5 * fVar3 - fVar6 * fVar2;
  *(float *)(param_1 + 4) = fVar6 * fVar1 - fVar3 * fVar4;
  *(float *)(param_1 + 8) = fVar2 * fVar4 - fVar5 * fVar1;
  return;
}

```
## target disasm
```
  00083dfc: vldr.32 s0,[r2]
  00083e00: vldr.32 s10,[r1,#0x8]
  00083e04: vldr.32 s2,[r2,#0x4]
  00083e08: vldr.32 s6,[r1]
  00083e0c: vmul.f32 s12,s10,s0
  00083e10: vldr.32 s4,[r2,#0x8]
  00083e14: vldr.32 s8,[r1,#0x4]
  00083e18: vmul.f32 s1,s2,s6
  00083e1c: vmul.f32 s14,s8,s4
  00083e20: vmls.f32 s12,s4,s6
  00083e24: vmls.f32 s1,s8,s0
  00083e28: vmls.f32 s14,s10,s2
  00083e2c: vstr.32 s14,[r0]
  00083e30: vstr.32 s12,[r0,#0x4]
  00083e34: vstr.32 s1,[r0,#0x8]
  00083e38: bx lr
```
