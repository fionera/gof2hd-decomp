# AbyssEngine::AEMath::VectorLerp
elf 0x73eec body 66
Sig: undefined __thiscall VectorLerp(AEMath * this, Vector * param_1, Vector * param_2, float param_3)

## decompile
```c

/* AbyssEngine::AEMath::VectorLerp(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector
   const&, float) */

void __thiscall
AbyssEngine::AEMath::VectorLerp(AEMath *this,Vector *param_1,Vector *param_2,float param_3)

{
  float in_r3;
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  
  fVar1 = *(float *)(param_1 + 4);
  fVar3 = *(float *)(param_2 + 4);
  fVar2 = *(float *)(param_1 + 8);
  fVar4 = *(float *)(param_2 + 8);
  *(float *)this = *(float *)param_1 + (*(float *)param_2 - *(float *)param_1) * in_r3;
  *(float *)(this + 4) = fVar1 + (fVar3 - fVar1) * in_r3;
  *(float *)(this + 8) = fVar2 + (fVar4 - fVar2) * in_r3;
  return;
}

```
## target disasm
```
  00083eec: vldr.32 s0,[r1]
  00083ef0: vmov s12,r3
  00083ef4: vldr.32 s6,[r2]
  00083ef8: vldr.32 s2,[r1,#0x4]
  00083efc: vldr.32 s8,[r2,#0x4]
  00083f00: vsub.f32 s6,s6,s0
  00083f04: vldr.32 s4,[r1,#0x8]
  00083f08: vldr.32 s10,[r2,#0x8]
  00083f0c: vsub.f32 s8,s8,s2
  00083f10: vsub.f32 s10,s10,s4
  00083f14: vmla.f32 s0,s6,s12
  00083f18: vmla.f32 s2,s8,s12
  00083f1c: vmla.f32 s4,s10,s12
  00083f20: vstr.32 s0,[r0]
  00083f24: vstr.32 s2,[r0,#0x4]
  00083f28: vstr.32 s4,[r0,#0x8]
  00083f2c: bx lr
```
