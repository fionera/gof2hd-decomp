# PaintCanvas::Initialize
elf 0x7e6bc body 250
Sig: undefined __thiscall Initialize(PaintCanvas * this, bool param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::Initialize(bool) */

void __thiscall AbyssEngine::PaintCanvas::Initialize(PaintCanvas *this,bool param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  uint in_fpscr;
  float fVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  float fVar9;
  
  uVar3 = 0;
  if (param_1) {
    uVar3 = 2;
  }
  *(undefined4 *)(this + 0x30) = uVar3;
  Engine::SetScreenOrientation(*(undefined4 *)(this + 0x34));
  uVar3 = 0;
  uVar6 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar7 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0xa8) = 0;
  *(undefined4 *)(this + 0xac) = uVar6;
  *(undefined4 *)(this + 0xb0) = uVar7;
  *(undefined4 *)(this + 0xb4) = uVar8;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = uVar6;
  *(undefined4 *)(this + 0xa0) = uVar7;
  *(undefined4 *)(this + 0xa4) = uVar8;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = uVar6;
  *(undefined4 *)(this + 0x90) = uVar7;
  *(undefined4 *)(this + 0x94) = uVar8;
  iVar4 = *(int *)(this + 0x30);
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = uVar6;
  *(undefined4 *)(this + 0x80) = uVar7;
  *(undefined4 *)(this + 0x84) = uVar8;
  uVar1 = Engine::GetDisplayWidth();
  fVar9 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  uVar2 = Engine::GetDisplayHeight();
  uVar1 = DAT_0008e7b4;
  fVar5 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
  if (iVar4 == 2) {
    *(float *)(this + 0x78) = 2.0 / fVar9;
    fVar9 = fVar5;
  }
  else {
    *(undefined4 *)(this + 0xd8) = uVar3;
    *(undefined4 *)(this + 0xdc) = uVar6;
    *(undefined4 *)(this + 0xe0) = uVar7;
    *(undefined4 *)(this + 0xe4) = uVar8;
    *(undefined4 *)(this + 200) = uVar3;
    *(undefined4 *)(this + 0xcc) = uVar6;
    *(undefined4 *)(this + 0xd0) = uVar7;
    *(undefined4 *)(this + 0xd4) = uVar8;
    *(undefined4 *)(this + 0xe4) = uVar3;
    *(undefined4 *)(this + 0xe8) = uVar6;
    *(undefined4 *)(this + 0xec) = uVar7;
    *(undefined4 *)(this + 0xf0) = uVar8;
    *(undefined4 *)(this + 0xf4) = 0x3f800000;
    *(float *)(this + 0x78) = 2.0 / fVar5;
    *(undefined4 *)(this + 0xb8) = uVar3;
    *(undefined4 *)(this + 0xbc) = uVar6;
    *(undefined4 *)(this + 0xc0) = uVar7;
    *(undefined4 *)(this + 0xc4) = uVar8;
    *(undefined4 *)(this + 0xbc) = uVar1;
    *(undefined4 *)(this + 0xe0) = 0x3f800000;
    *(undefined4 *)(this + 200) = 0x3f800000;
    *(float *)(this + 0xec) = fVar9;
  }
  *(undefined4 *)(this + 0xa0) = DAT_0008e7b8;
  uVar3 = DAT_0008e7b4;
  *(undefined4 *)(this + 0xb4) = 0x3f800000;
  *(undefined4 *)(this + 0xa8) = uVar3;
  *(undefined4 *)(this + 0xac) = 0x3f800000;
  *(float *)(this + 0x8c) = -2.0 / fVar9;
  (*(code *)(DAT_001ab2d4 + 0x1ab2d8))(this,0x46800000,0x44000000,0x47800000);
  return;
}

```

## target disasm
```
  0008e6bc: push {r4,r5,r6,r7,lr}
  0008e6be: add r7,sp,#0xc
  0008e6c0: push.w r11
  0008e6c4: vpush {d8,d9,d10}
  0008e6c8: mov r4,r0
  0008e6ca: ldr r0,[r0,#0x34]
  0008e6cc: cmp r1,#0x0
  0008e6ce: it ne
  0008e6d0: mov.ne r1,#0x2
  0008e6d2: str r1,[r4,#0x30]
  0008e6d4: blx 0x0006ff7c
  0008e6d8: vmov.i32 q4,#0x0
  0008e6dc: add.w r0,r4,#0xa8
  0008e6e0: add.w r5,r4,#0x78
  0008e6e4: vst1.32 {d8,d9},[r0]
  0008e6e8: add.w r0,r4,#0x98
  0008e6ec: vst1.32 {d8,d9},[r0]
  0008e6f0: add.w r0,r4,#0x88
  0008e6f4: vst1.32 {d8,d9},[r0]
  0008e6f8: ldrd r6,r0,[r4,#0x30]
  0008e6fc: vst1.32 {d8,d9},[r5]
  0008e700: blx 0x0006e8cc
  0008e704: ldr r1,[r4,#0x34]
  0008e706: vmov s0,r0
  0008e70a: vcvt.f32.s32 s20,s0
  0008e70e: mov r0,r1
  0008e710: blx 0x0006e8d8
  0008e714: vmov s0,r0
  0008e718: cmp r6,#0x2
  0008e71a: vmov.f32 s2,0x40000000
  0008e71e: vcvt.f32.s32 s0,s0
  0008e722: bne 0x0008e732
  0008e724: vdiv.f32 s2,s2,s20
  0008e728: vmov.f32 s20,s0
  0008e72c: vstr.32 s2,[r5]
  0008e730: b 0x0008e776
  0008e732: vdiv.f32 s0,s2,s0
  0008e736: add.w r1,r4,#0xd8
  0008e73a: add.w r2,r4,#0xb8
  0008e73e: movs r3,#0x34
  0008e740: ldr r0,[0x0008e7b4]
  0008e742: vst1.32 {d8,d9},[r1]
  0008e746: add.w r1,r4,#0xc8
  0008e74a: vst1.32 {d8,d9},[r1]
  0008e74e: add.w r1,r4,#0xe4
  0008e752: vst1.32 {d8,d9},[r1]
  0008e756: mov.w r1,#0x3f800000
  0008e75a: str.w r1,[r4,#0xf4]
  0008e75e: vstr.32 s0,[r4,#0x78]
  0008e762: vst1.32 {d8,d9},[r2],r3
  0008e766: str.w r0,[r4,#0xbc]
  0008e76a: str.w r1,[r4,#0xe0]
  0008e76e: str.w r1,[r4,#0xc8]
  0008e772: vstr.32 s20,[r2]
  0008e776: vmov.f32 s0,0xc0000000
  0008e77a: ldr r1,[0x0008e7b8]
  0008e77c: str.w r1,[r4,#0xa0]
  0008e780: mov.w r1,#0x3f800000
  0008e784: ldr r0,[0x0008e7b4]
  0008e786: mov.w r2,#0x44000000
  0008e78a: str.w r1,[r4,#0xb4]
  0008e78e: mov.w r3,#0x47800000
  0008e792: strd r0,r1,[r4,#0xa8]
  0008e796: mov r0,r4
  0008e798: mov.w r1,#0x46800000
  0008e79c: vdiv.f32 s0,s0,s20
  0008e7a0: vstr.32 s0,[r4,#0x8c]
  0008e7a4: vpop {d8,d9,d10}
  0008e7a8: pop.w r11
  0008e7ac: pop.w {r4,r5,r6,r7,lr}
  0008e7b0: b.w 0x001ab2c8
  001ab2c8: bx pc
```
