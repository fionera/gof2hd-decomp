# AbyssEngine::AEMath::Matrix::operator*=
elf 0x8016a body 350
Sig: undefined __thiscall operator*=(Matrix * this, Matrix * param_1)

## decompile
```c

/* AbyssEngine::AEMath::Matrix::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Matrix const&) */

Matrix * __thiscall AbyssEngine::AEMath::Matrix::operator*=(Matrix *this,Matrix *param_1)

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
  float fVar12;
  
  fVar10 = *(float *)(this + 4);
  fVar2 = *(float *)(param_1 + 0x10);
  fVar8 = *(float *)this;
  fVar11 = *(float *)(this + 8);
  fVar1 = *(float *)(param_1 + 0x20);
  *(float *)this = fVar10 * fVar2 + fVar8 * *(float *)param_1 + fVar11 * fVar1;
  fVar4 = *(float *)(param_1 + 0x14);
  fVar3 = *(float *)(param_1 + 0x24);
  *(float *)(this + 4) = fVar10 * fVar4 + fVar8 * *(float *)(param_1 + 4) + fVar11 * fVar3;
  fVar6 = *(float *)(param_1 + 0x18);
  fVar5 = *(float *)(param_1 + 0x28);
  *(float *)(this + 8) = fVar10 * fVar6 + fVar8 * *(float *)(param_1 + 8) + fVar11 * fVar5;
  fVar9 = *(float *)(param_1 + 0x1c);
  fVar7 = *(float *)(param_1 + 0x2c);
  *(float *)(this + 0xc) =
       *(float *)(this + 0xc) + fVar10 * fVar9 + fVar8 * *(float *)(param_1 + 0xc) + fVar11 * fVar7;
  fVar8 = *(float *)(this + 0x10);
  fVar12 = *(float *)param_1;
  fVar10 = *(float *)(this + 0x14);
  fVar11 = *(float *)(this + 0x18);
  *(float *)(this + 0x10) = fVar8 * fVar12 + fVar2 * fVar10 + fVar1 * fVar11;
  fVar2 = *(float *)(param_1 + 4);
  *(float *)(this + 0x14) = fVar8 * fVar2 + fVar4 * fVar10 + fVar3 * fVar11;
  fVar4 = *(float *)(param_1 + 8);
  *(float *)(this + 0x18) = fVar8 * fVar4 + fVar6 * fVar10 + fVar5 * fVar11;
  fVar6 = *(float *)(param_1 + 0xc);
  *(float *)(this + 0x1c) =
       *(float *)(this + 0x1c) + fVar8 * fVar6 + fVar9 * fVar10 + fVar7 * fVar11;
  fVar9 = *(float *)(this + 0x24);
  fVar8 = *(float *)(this + 0x20);
  fVar10 = *(float *)(this + 0x28);
  *(float *)(this + 0x20) = fVar9 * *(float *)(param_1 + 0x10) + fVar12 * fVar8 + fVar1 * fVar10;
  *(float *)(this + 0x24) = fVar9 * *(float *)(param_1 + 0x14) + fVar2 * fVar8 + fVar3 * fVar10;
  *(float *)(this + 0x28) = fVar9 * *(float *)(param_1 + 0x18) + fVar4 * fVar8 + fVar5 * fVar10;
  *(float *)(this + 0x2c) =
       *(float *)(this + 0x2c) + fVar9 * *(float *)(param_1 + 0x1c) + fVar6 * fVar8 + fVar7 * fVar10
  ;
  Vector::operator*=((Vector *)(this + 0x30),(Vector *)(param_1 + 0x30));
  return this;
}

```
## target disasm
```
  0009016a: push {r4,r6,r7,lr}
  0009016c: add r7,sp,#0x8
  0009016e: vldr.32 s10,[r0,#0x4]
  00090172: mov r4,r0
  00090174: vldr.32 s1,[r1,#0x10]
  00090178: vldr.32 s8,[r0]
  0009017c: vmul.f32 s4,s10,s1
  00090180: vldr.32 s2,[r1]
  00090184: vldr.32 s12,[r0,#0x8]
  00090188: vldr.32 s0,[r1,#0x20]
  0009018c: vldr.32 s14,[r0,#0xc]
  00090190: vmla.f32 s4,s8,s2
  00090194: vmla.f32 s4,s12,s0
  00090198: vstr.32 s4,[r0]
  0009019c: vldr.32 s3,[r1,#0x14]
  000901a0: vldr.32 s4,[r1,#0x4]
  000901a4: vmul.f32 s6,s10,s3
  000901a8: vldr.32 s2,[r1,#0x24]
  000901ac: vmla.f32 s6,s8,s4
  000901b0: vmla.f32 s6,s12,s2
  000901b4: vstr.32 s6,[r0,#0x4]
  000901b8: vldr.32 s5,[r1,#0x18]
  000901bc: vldr.32 s6,[r1,#0x8]
  000901c0: vmul.f32 s7,s10,s5
  000901c4: vldr.32 s4,[r1,#0x28]
  000901c8: vmla.f32 s7,s8,s6
  000901cc: vmla.f32 s7,s12,s4
  000901d0: vstr.32 s7,[r0,#0x8]
  000901d4: vldr.32 s9,[r1,#0x1c]
  000901d8: vldr.32 s7,[r1,#0xc]
  000901dc: vmul.f32 s10,s10,s9
  000901e0: vldr.32 s6,[r1,#0x2c]
  000901e4: vmla.f32 s10,s8,s7
  000901e8: vmla.f32 s10,s12,s6
  000901ec: vadd.f32 s8,s14,s10
  000901f0: vstr.32 s8,[r0,#0xc]
  000901f4: vldr.32 s8,[r0,#0x10]
  000901f8: vldr.32 s14,[r1]
  000901fc: vldr.32 s10,[r0,#0x14]
  00090200: vmul.f32 s7,s8,s14
  00090204: vldr.32 s12,[r0,#0x18]
  00090208: vmla.f32 s7,s1,s10
  0009020c: vmla.f32 s7,s0,s12
  00090210: vstr.32 s7,[r0,#0x10]
  00090214: vldr.32 s1,[r1,#0x4]
  00090218: vmul.f32 s7,s8,s1
  0009021c: vmla.f32 s7,s3,s10
  00090220: vmla.f32 s7,s2,s12
  00090224: vstr.32 s7,[r0,#0x14]
  00090228: vldr.32 s3,[r1,#0x8]
  0009022c: vmul.f32 s7,s8,s3
  00090230: vmla.f32 s7,s5,s10
  00090234: vmla.f32 s7,s4,s12
  00090238: vstr.32 s7,[r0,#0x18]
  0009023c: vldr.32 s5,[r1,#0xc]
  00090240: vmul.f32 s8,s8,s5
  00090244: vmla.f32 s8,s9,s10
  00090248: vldr.32 s10,[r0,#0x1c]
  0009024c: vmla.f32 s8,s6,s12
  00090250: vadd.f32 s8,s10,s8
  00090254: vstr.32 s8,[r0,#0x1c]
  00090258: vldr.32 s10,[r0,#0x24]
  0009025c: vldr.32 s7,[r1,#0x10]
  00090260: vldr.32 s8,[r0,#0x20]
  00090264: vmul.f32 s7,s10,s7
  00090268: vldr.32 s12,[r0,#0x28]
  0009026c: vmla.f32 s7,s14,s8
  00090270: vmla.f32 s7,s0,s12
  00090274: vstr.32 s7,[r0,#0x20]
  00090278: vldr.32 s0,[r1,#0x14]
  0009027c: vmul.f32 s0,s10,s0
  00090280: vmla.f32 s0,s1,s8
  00090284: vmla.f32 s0,s2,s12
  00090288: vstr.32 s0,[r0,#0x24]
  0009028c: vldr.32 s0,[r1,#0x18]
  00090290: vmul.f32 s0,s10,s0
  00090294: vmla.f32 s0,s3,s8
  00090298: vmla.f32 s0,s4,s12
  0009029c: vstr.32 s0,[r0,#0x28]
  000902a0: vldr.32 s0,[r1,#0x1c]
  000902a4: adds r1,#0x30
  000902a6: vldr.32 s2,[r0,#0x2c]
  000902aa: vmul.f32 s0,s10,s0
  000902ae: vmla.f32 s0,s5,s8
  000902b2: vmla.f32 s0,s6,s12
  000902b6: vadd.f32 s0,s2,s0
  000902ba: vstr.32 s0,[r0,#0x2c]
  000902be: adds r0,#0x30
  000902c0: blx 0x00070198
  000902c4: mov r0,r4
  000902c6: pop {r4,r6,r7,pc}
```
