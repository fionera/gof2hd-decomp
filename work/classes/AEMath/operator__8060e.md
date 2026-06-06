# AbyssEngine::AEMath::operator*
elf 0x8060e body 122
Sig: undefined __thiscall operator*(AEMath * this, Matrix * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Vector const&) */

void __thiscall AbyssEngine::AEMath::operator*(AEMath *this,Matrix *param_1,Vector *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  
  fVar3 = *(float *)(param_2 + 4);
  fVar2 = *(float *)(param_1 + 0x14);
  fVar8 = *(float *)(param_1 + 0x24);
  fVar1 = *(float *)param_2;
  fVar11 = *(float *)(param_1 + 0x10);
  fVar7 = *(float *)(param_1 + 0x20);
  fVar5 = *(float *)(param_2 + 8);
  fVar4 = *(float *)(param_1 + 0x18);
  fVar9 = *(float *)(param_1 + 0x28);
  fVar6 = *(float *)(param_1 + 0x1c);
  fVar10 = *(float *)(param_1 + 0x2c);
  *(float *)this =
       *(float *)(param_1 + 0xc) +
       fVar3 * *(float *)(param_1 + 4) + fVar1 * *(float *)param_1 + fVar5 * *(float *)(param_1 + 8)
  ;
  *(float *)(this + 4) = fVar6 + fVar3 * fVar2 + fVar1 * fVar11 + fVar5 * fVar4;
  *(float *)(this + 8) = fVar10 + fVar3 * fVar8 + fVar1 * fVar7 + fVar5 * fVar9;
  return;
}

```
## target disasm
```
  0009060e: vldr.32 s2,[r2,#0x4]
  00090612: vldr.32 s8,[r1,#0x4]
  00090616: vldr.32 s1,[r1,#0x14]
  0009061a: vldr.32 s9,[r1,#0x24]
  0009061e: vmul.f32 s8,s2,s8
  00090622: vmul.f32 s1,s2,s1
  00090626: vldr.32 s0,[r2]
  0009062a: vmul.f32 s2,s2,s9
  0009062e: vldr.32 s6,[r1]
  00090632: vldr.32 s14,[r1,#0x10]
  00090636: vldr.32 s7,[r1,#0x20]
  0009063a: vldr.32 s4,[r2,#0x8]
  0009063e: vmla.f32 s8,s0,s6
  00090642: vldr.32 s10,[r1,#0x8]
  00090646: vmla.f32 s1,s0,s14
  0009064a: vldr.32 s3,[r1,#0x18]
  0009064e: vmla.f32 s2,s0,s7
  00090652: vldr.32 s11,[r1,#0x28]
  00090656: vldr.32 s12,[r1,#0xc]
  0009065a: vldr.32 s5,[r1,#0x1c]
  0009065e: vldr.32 s13,[r1,#0x2c]
  00090662: vmla.f32 s8,s4,s10
  00090666: vmla.f32 s1,s4,s3
  0009066a: vmla.f32 s2,s4,s11
  0009066e: vadd.f32 s4,s12,s8
  00090672: vadd.f32 s0,s5,s1
  00090676: vadd.f32 s2,s13,s2
  0009067a: vstr.32 s4,[r0]
  0009067e: vstr.32 s0,[r0,#0x4]
  00090682: vstr.32 s2,[r0,#0x8]
  00090686: bx lr
```
