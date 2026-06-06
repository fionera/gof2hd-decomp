# AbyssEngine::Quaternion::Convert
elf 0x81520 body 314
Sig: undefined __thiscall Convert(Quaternion * this, Matrix * param_1)

## decompile
```c

/* AbyssEngine::Quaternion::Convert(AbyssEngine::AEMath::Matrix&) */

void __thiscall AbyssEngine::Quaternion::Convert(Quaternion *this,Matrix *param_1)

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
  float fVar13;
  float fVar14;
  
  fVar7 = *(float *)this;
  fVar8 = *(float *)(this + 4);
  fVar14 = fVar7 * fVar7;
  fVar9 = *(float *)(this + 8);
  fVar11 = fVar8 * fVar8;
  fVar10 = *(float *)(this + 0xc);
  fVar12 = fVar9 * fVar9;
  fVar13 = fVar10 * fVar10;
  fVar1 = 1.0 / (fVar14 + fVar11 + fVar12 + fVar13);
  fVar6 = fVar7 * fVar8 + fVar10 * fVar9;
  fVar5 = fVar7 * fVar8 - fVar10 * fVar9;
  fVar4 = fVar7 * fVar9 - fVar10 * fVar8;
  fVar3 = fVar10 * fVar8 + fVar7 * fVar9;
  fVar2 = fVar10 * fVar7 + fVar8 * fVar9;
  fVar7 = fVar8 * fVar9 - fVar10 * fVar7;
  *(float *)param_1 = (((fVar14 - fVar11) - fVar12) + fVar13) * fVar1;
  *(float *)(param_1 + 4) = (fVar6 + fVar6) * fVar1;
  *(float *)(param_1 + 8) = (fVar4 + fVar4) * fVar1;
  *(float *)(param_1 + 0x10) = (fVar5 + fVar5) * fVar1;
  *(float *)(param_1 + 0x14) = (((fVar11 - fVar14) - fVar12) + fVar13) * fVar1;
  *(float *)(param_1 + 0x18) = (fVar2 + fVar2) * fVar1;
  *(float *)(param_1 + 0x20) = (fVar3 + fVar3) * fVar1;
  *(float *)(param_1 + 0x24) = (fVar7 + fVar7) * fVar1;
  *(float *)(param_1 + 0x28) = ((-fVar14 - fVar11) + fVar12 + fVar13) * fVar1;
  return;
}

```
## target disasm
```
  00091520: vldr.32 s0,[r0]
  00091524: vldr.32 s2,[r0,#0x4]
  00091528: vmul.f32 s10,s0,s0
  0009152c: vldr.32 s4,[r0,#0x8]
  00091530: vmul.f32 s8,s2,s2
  00091534: vldr.32 s6,[r0,#0xc]
  00091538: vmov.f64 d26,0x3ff0000000000000
  0009153c: vmul.f32 s12,s0,s2
  00091540: vcvt.f64.f32 d17,s10
  00091544: vcvt.f64.f32 d16,s8
  00091548: vmul.f32 s8,s4,s4
  0009154c: vadd.f64 d18,d17,d16
  00091550: vcvt.f64.f32 d19,s8
  00091554: vmul.f32 s8,s6,s6
  00091558: vadd.f64 d18,d18,d19
  0009155c: vcvt.f64.f32 d21,s8
  00091560: vadd.f64 d18,d18,d21
  00091564: vneg.f64 d20,d17
  00091568: vmul.f32 s14,s6,s2
  0009156c: vmul.f32 s1,s0,s4
  00091570: vmul.f32 s2,s2,s4
  00091574: vmul.f32 s0,s6,s0
  00091578: vmul.f32 s10,s6,s4
  0009157c: vdiv.f64 d18,d26,d18
  00091580: vsub.f64 d22,d16,d17
  00091584: vsub.f64 d17,d17,d16
  00091588: vsub.f64 d16,d20,d16
  0009158c: vcvt.f64.f32 d20,s10
  00091590: vcvt.f64.f32 d23,s12
  00091594: vcvt.f64.f32 d24,s14
  00091598: vcvt.f64.f32 d25,s1
  0009159c: vcvt.f64.f32 d27,s2
  000915a0: vcvt.f64.f32 d28,s0
  000915a4: vsub.f64 d22,d22,d19
  000915a8: vsub.f64 d17,d17,d19
  000915ac: vadd.f64 d16,d16,d19
  000915b0: vadd.f64 d19,d23,d20
  000915b4: vsub.f64 d20,d23,d20
  000915b8: vsub.f64 d23,d25,d24
  000915bc: vadd.f64 d24,d24,d25
  000915c0: vadd.f64 d25,d28,d27
  000915c4: vsub.f64 d26,d27,d28
  000915c8: vadd.f64 d22,d22,d21
  000915cc: vadd.f64 d17,d17,d21
  000915d0: vadd.f64 d16,d16,d21
  000915d4: vadd.f64 d21,d23,d23
  000915d8: vadd.f64 d23,d24,d24
  000915dc: vadd.f64 d24,d25,d25
  000915e0: vadd.f64 d19,d19,d19
  000915e4: vadd.f64 d20,d20,d20
  000915e8: vadd.f64 d25,d26,d26
  000915ec: vmul.f64 d22,d22,d18
  000915f0: vmul.f64 d17,d17,d18
  000915f4: vmul.f64 d16,d16,d18
  000915f8: vmul.f64 d19,d19,d18
  000915fc: vmul.f64 d20,d20,d18
  00091600: vmul.f64 d21,d21,d18
  00091604: vmul.f64 d23,d23,d18
  00091608: vmul.f64 d24,d24,d18
  0009160c: vmul.f64 d18,d25,d18
  00091610: vcvt.f32.f64 s2,d17
  00091614: vcvt.f32.f64 s0,d22
  00091618: vcvt.f32.f64 s4,d16
  0009161c: vcvt.f32.f64 s6,d19
  00091620: vcvt.f32.f64 s8,d20
  00091624: vcvt.f32.f64 s10,d21
  00091628: vcvt.f32.f64 s12,d23
  0009162c: vcvt.f32.f64 s14,d24
  00091630: vcvt.f32.f64 s1,d18
  00091634: vstr.32 s2,[r1]
  00091638: vstr.32 s6,[r1,#0x4]
  0009163c: vstr.32 s10,[r1,#0x8]
  00091640: vstr.32 s8,[r1,#0x10]
  00091644: vstr.32 s0,[r1,#0x14]
  00091648: vstr.32 s14,[r1,#0x18]
  0009164c: vstr.32 s12,[r1,#0x20]
  00091650: vstr.32 s1,[r1,#0x24]
  00091654: vstr.32 s4,[r1,#0x28]
  00091658: bx lr
```
