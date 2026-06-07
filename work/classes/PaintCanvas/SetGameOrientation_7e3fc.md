# PaintCanvas::SetGameOrientation
elf 0x7e3fc body 696
Sig: undefined __thiscall SetGameOrientation(PaintCanvas * this, LandscapeMode param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetGameOrientation(AbyssEngine::LandscapeMode) */

void __thiscall AbyssEngine::PaintCanvas::SetGameOrientation(PaintCanvas *this,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  uint in_fpscr;
  float fVar7;
  float fVar8;
  
  if (*(int *)(this + 0x30) == param_2) {
    return;
  }
  *(int *)(this + 0x30) = param_2;
  Engine::SetScreenOrientation(*(undefined4 *)(this + 0x34),param_2);
  *(float *)(this + 0x3c) = -*(float *)(this + 0x3c);
  *(float *)(this + 0x48) = -*(float *)(this + 0x48);
  uVar4 = Engine::GetDisplayWidth();
  fVar8 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
  uVar5 = Engine::GetDisplayHeight();
  uVar4 = DAT_0008e6b8;
  fVar7 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  if (param_2 == 3) {
    *(undefined4 *)(this + 0xb0) = 0;
    *(undefined4 *)(this + 0xac) = 0x3f800000;
    *(undefined4 *)(this + 0xec) = 0;
    *(undefined4 *)(this + 0xf0) = 0;
    *(undefined4 *)(this + 0xb4) = 0x3f800000;
    *(undefined4 *)(this + 0xb8) = uVar4;
    uVar3 = DAT_0008e6b4;
    uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    *(undefined4 *)(this + 0x9c) = 0;
    *(undefined4 *)(this + 0xa0) = uVar5;
    *(undefined4 *)(this + 0xa4) = uVar1;
    *(undefined4 *)(this + 0xa8) = uVar2;
    *(undefined4 *)(this + 0xdc) = 0;
    *(undefined4 *)(this + 0xe0) = uVar5;
    *(undefined4 *)(this + 0xe4) = uVar1;
    *(undefined4 *)(this + 0xe8) = uVar2;
    *(undefined4 *)(this + 0xcc) = 0;
    *(undefined4 *)(this + 0xd0) = uVar5;
    *(undefined4 *)(this + 0xd4) = uVar1;
    *(undefined4 *)(this + 0xd8) = uVar2;
    *(undefined4 *)(this + 0x8c) = 0;
    *(undefined4 *)(this + 0x90) = uVar5;
    *(undefined4 *)(this + 0x94) = uVar1;
    *(undefined4 *)(this + 0x98) = uVar2;
    *(undefined4 *)(this + 0x7c) = 0;
    *(undefined4 *)(this + 0x80) = uVar5;
    *(undefined4 *)(this + 0x84) = uVar1;
    *(undefined4 *)(this + 0x88) = uVar2;
    *(undefined4 *)(this + 0xbc) = 0;
    *(undefined4 *)(this + 0xc0) = uVar5;
    *(undefined4 *)(this + 0xc4) = uVar1;
    *(undefined4 *)(this + 200) = uVar2;
    *(undefined4 *)(this + 0xa0) = uVar3;
    *(undefined4 *)(this + 0xa8) = uVar4;
    *(undefined4 *)(this + 0xe0) = 0x3f800000;
    *(undefined4 *)(this + 0xcc) = uVar4;
    *(float *)(this + 0x78) = 2.0 / fVar8;
    *(float *)(this + 0x8c) = -(2.0 / fVar7);
    *(undefined4 *)(this + 0xf4) = 0x3f800000;
    uVar4 = Engine::GetDisplayWidth();
    uVar4 = VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
    *(undefined4 *)(this + 0xe8) = uVar4;
    uVar4 = Engine::GetDisplayHeight();
  }
  else {
    if (param_2 == 1) {
      *(undefined4 *)(this + 0xb0) = 0;
      *(undefined4 *)(this + 0xac) = 0x3f800000;
      *(undefined4 *)(this + 0xb4) = 0x3f800000;
      uVar3 = DAT_0008e6b8;
      uVar2 = DAT_0008e6b4;
      uVar4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      *(undefined4 *)(this + 0x9c) = 0;
      *(undefined4 *)(this + 0xa0) = uVar4;
      *(undefined4 *)(this + 0xa4) = uVar5;
      *(undefined4 *)(this + 0xa8) = uVar1;
      *(undefined4 *)(this + 0xd8) = 0;
      *(undefined4 *)(this + 0xdc) = uVar4;
      *(undefined4 *)(this + 0xe0) = uVar5;
      *(undefined4 *)(this + 0xe4) = uVar1;
      *(undefined4 *)(this + 200) = 0;
      *(undefined4 *)(this + 0xcc) = uVar4;
      *(undefined4 *)(this + 0xd0) = uVar5;
      *(undefined4 *)(this + 0xd4) = uVar1;
      *(undefined4 *)(this + 0x8c) = 0;
      *(undefined4 *)(this + 0x90) = uVar4;
      *(undefined4 *)(this + 0x94) = uVar5;
      *(undefined4 *)(this + 0x98) = uVar1;
      *(undefined4 *)(this + 0x7c) = 0;
      *(undefined4 *)(this + 0x80) = uVar4;
      *(undefined4 *)(this + 0x84) = uVar5;
      *(undefined4 *)(this + 0x88) = uVar1;
      *(undefined4 *)(this + 0xe4) = 0;
      *(undefined4 *)(this + 0xe8) = uVar4;
      *(undefined4 *)(this + 0xec) = uVar5;
      *(undefined4 *)(this + 0xf0) = uVar1;
      *(undefined4 *)(this + 0xb8) = 0;
      *(undefined4 *)(this + 0xbc) = uVar4;
      *(undefined4 *)(this + 0xc0) = uVar5;
      *(undefined4 *)(this + 0xc4) = uVar1;
      *(undefined4 *)(this + 0xbc) = 0x3f800000;
      *(undefined4 *)(this + 0xa0) = uVar2;
      *(undefined4 *)(this + 0xa8) = uVar3;
      *(undefined4 *)(this + 0xe0) = 0x3f800000;
      *(undefined4 *)(this + 200) = uVar3;
      *(float *)(this + 0x78) = 2.0 / fVar7;
      *(float *)(this + 0x8c) = -(2.0 / fVar8);
      *(undefined4 *)(this + 0xf4) = 0x3f800000;
      uVar4 = Engine::GetDisplayHeight();
      uVar4 = VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
      *(undefined4 *)(this + 0xe8) = uVar4;
      goto LAB_0008e68e;
    }
    if (param_2 != 0) {
      *(undefined4 *)(this + 0xb0) = 0;
      *(undefined4 *)(this + 0xac) = 0x3f800000;
      *(undefined4 *)(this + 0xec) = 0;
      *(undefined4 *)(this + 0xf0) = 0;
      *(undefined4 *)(this + 0xb4) = 0x3f800000;
      *(undefined4 *)(this + 0xb8) = 0x3f800000;
      uVar3 = DAT_0008e6b8;
      uVar2 = DAT_0008e6b4;
      uVar4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      *(undefined4 *)(this + 0x9c) = 0;
      *(undefined4 *)(this + 0xa0) = uVar4;
      *(undefined4 *)(this + 0xa4) = uVar5;
      *(undefined4 *)(this + 0xa8) = uVar1;
      *(undefined4 *)(this + 0xdc) = 0;
      *(undefined4 *)(this + 0xe0) = uVar4;
      *(undefined4 *)(this + 0xe4) = uVar5;
      *(undefined4 *)(this + 0xe8) = uVar1;
      *(undefined4 *)(this + 0xcc) = 0;
      *(undefined4 *)(this + 0xd0) = uVar4;
      *(undefined4 *)(this + 0xd4) = uVar5;
      *(undefined4 *)(this + 0xd8) = uVar1;
      *(undefined4 *)(this + 0x8c) = 0;
      *(undefined4 *)(this + 0x90) = uVar4;
      *(undefined4 *)(this + 0x94) = uVar5;
      *(undefined4 *)(this + 0x98) = uVar1;
      *(undefined4 *)(this + 0x7c) = 0;
      *(undefined4 *)(this + 0x80) = uVar4;
      *(undefined4 *)(this + 0x84) = uVar5;
      *(undefined4 *)(this + 0x88) = uVar1;
      *(undefined4 *)(this + 0xbc) = 0;
      *(undefined4 *)(this + 0xc0) = uVar4;
      *(undefined4 *)(this + 0xc4) = uVar5;
      *(undefined4 *)(this + 200) = uVar1;
      *(undefined4 *)(this + 0xa0) = uVar2;
      *(undefined4 *)(this + 0xa8) = uVar3;
      *(undefined4 *)(this + 0xe0) = 0x3f800000;
      *(undefined4 *)(this + 0xcc) = 0x3f800000;
      *(float *)(this + 0x78) = 2.0 / fVar8;
      *(float *)(this + 0x8c) = -(2.0 / fVar7);
      *(undefined4 *)(this + 0xf4) = 0x3f800000;
      goto LAB_0008e68e;
    }
    *(undefined4 *)(this + 0xb0) = 0;
    *(undefined4 *)(this + 0xac) = 0x3f800000;
    *(undefined4 *)(this + 0xb4) = 0x3f800000;
    uVar3 = DAT_0008e6b8;
    uVar2 = DAT_0008e6b4;
    uVar4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    *(undefined4 *)(this + 0x9c) = 0;
    *(undefined4 *)(this + 0xa0) = uVar4;
    *(undefined4 *)(this + 0xa4) = uVar5;
    *(undefined4 *)(this + 0xa8) = uVar1;
    *(undefined4 *)(this + 0xd8) = 0;
    *(undefined4 *)(this + 0xdc) = uVar4;
    *(undefined4 *)(this + 0xe0) = uVar5;
    *(undefined4 *)(this + 0xe4) = uVar1;
    *(undefined4 *)(this + 200) = 0;
    *(undefined4 *)(this + 0xcc) = uVar4;
    *(undefined4 *)(this + 0xd0) = uVar5;
    *(undefined4 *)(this + 0xd4) = uVar1;
    *(undefined4 *)(this + 0x8c) = 0;
    *(undefined4 *)(this + 0x90) = uVar4;
    *(undefined4 *)(this + 0x94) = uVar5;
    *(undefined4 *)(this + 0x98) = uVar1;
    *(undefined4 *)(this + 0x7c) = 0;
    *(undefined4 *)(this + 0x80) = uVar4;
    *(undefined4 *)(this + 0x84) = uVar5;
    *(undefined4 *)(this + 0x88) = uVar1;
    *(undefined4 *)(this + 0xe4) = 0;
    *(undefined4 *)(this + 0xe8) = uVar4;
    *(undefined4 *)(this + 0xec) = uVar5;
    *(undefined4 *)(this + 0xf0) = uVar1;
    *(undefined4 *)(this + 0xb8) = 0;
    *(undefined4 *)(this + 0xbc) = uVar4;
    *(undefined4 *)(this + 0xc0) = uVar5;
    *(undefined4 *)(this + 0xc4) = uVar1;
    *(undefined4 *)(this + 0xbc) = uVar3;
    *(undefined4 *)(this + 0xa0) = uVar2;
    *(undefined4 *)(this + 0xa8) = uVar3;
    *(undefined4 *)(this + 0xe0) = 0x3f800000;
    *(undefined4 *)(this + 200) = 0x3f800000;
    *(float *)(this + 0x78) = 2.0 / fVar7;
    *(float *)(this + 0x8c) = -(2.0 / fVar8);
    *(undefined4 *)(this + 0xf4) = 0x3f800000;
    uVar4 = Engine::GetDisplayWidth();
  }
  uVar4 = VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 0xec) = uVar4;
LAB_0008e68e:
  if (*(int *)(this + 0x170) == -1) {
    return;
  }
  puVar6 = *(undefined4 **)(*(int *)(this + 0x168) + *(int *)(this + 0x170) * 4);
  (*(code *)(DAT_001ab2d4 + 0x1ab2d8))(this,*puVar6,puVar6[1],puVar6[2]);
  return;
}

```

## target disasm
```
  0008e3fc: push {r4,r5,r7,lr}
  0008e3fe: add r7,sp,#0x8
  0008e400: vpush {d8}
  0008e404: mov r4,r0
  0008e406: ldr r0,[r0,#0x30]
  0008e408: mov r5,r1
  0008e40a: cmp r0,r1
  0008e40c: beq.w 0x0008e6ae
  0008e410: ldr r0,[r4,#0x34]
  0008e412: mov r1,r5
  0008e414: str r5,[r4,#0x30]
  0008e416: blx 0x0006ff7c
  0008e41a: vldr.32 s0,[r4,#0x3c]
  0008e41e: vldr.32 s2,[r4,#0x48]
  0008e422: vneg.f32 s0,s0
  0008e426: ldr r0,[r4,#0x34]
  0008e428: vneg.f32 s2,s2
  0008e42c: vstr.32 s0,[r4,#0x3c]
  0008e430: vstr.32 s2,[r4,#0x48]
  0008e434: blx 0x0006e8cc
  0008e438: ldr r1,[r4,#0x34]
  0008e43a: vmov s0,r0
  0008e43e: vcvt.f32.s32 s16,s0
  0008e442: mov r0,r1
  0008e444: blx 0x0006e8d8
  0008e448: vmov s0,r0
  0008e44c: cmp r5,#0x3
  0008e44e: vmov.f32 s2,0x40000000
  0008e452: vcvt.f32.s32 s0,s0
  0008e456: beq 0x0008e4ea
  0008e458: cmp r5,#0x1
  0008e45a: beq.w 0x0008e586
  0008e45e: cmp r5,#0x0
  0008e460: bne.w 0x0008e618
  0008e464: vdiv.f32 s4,s2,s16
  0008e468: movs r2,#0x0
  0008e46a: add.w r3,r4,#0x9c
  0008e46e: str.w r2,[r4,#0xb0]
  0008e472: mov.w r2,#0x3f800000
  0008e476: movs r5,#0x3c
  0008e478: str.w r2,[r4,#0xac]
  0008e47c: str.w r2,[r4,#0xb4]
  0008e480: ldr r0,[0x0008e6b4]
  0008e482: ldr r1,[0x0008e6b8]
  0008e484: vdiv.f32 s0,s2,s0
  0008e488: vmov.i32 q8,#0x0
  0008e48c: vneg.f32 s2,s4
  0008e490: vst1.32 {d16,d17},[r3]
  0008e494: add.w r3,r4,#0xd8
  0008e498: vst1.32 {d16,d17},[r3]
  0008e49c: add.w r3,r4,#0xc8
  0008e4a0: vst1.32 {d16,d17},[r3]
  0008e4a4: add.w r3,r4,#0x8c
  0008e4a8: vst1.32 {d16,d17},[r3]
  0008e4ac: add.w r3,r4,#0x7c
  0008e4b0: vst1.32 {d16,d17},[r3]
  0008e4b4: add.w r3,r4,#0xe4
  0008e4b8: vst1.32 {d16,d17},[r3]
  0008e4bc: add.w r3,r4,#0xb8
  0008e4c0: vst1.32 {d16,d17},[r3],r5
  0008e4c4: str.w r1,[r4,#0xbc]
  0008e4c8: str.w r0,[r4,#0xa0]
  0008e4cc: str.w r1,[r4,#0xa8]
  0008e4d0: str.w r2,[r4,#0xe0]
  0008e4d4: str.w r2,[r4,#0xc8]
  0008e4d8: vstr.32 s0,[r4,#0x78]
  0008e4dc: vstr.32 s2,[r4,#0x8c]
  0008e4e0: str r2,[r3,#0x0]
  0008e4e2: ldr r0,[r4,#0x34]
  0008e4e4: blx 0x0006e8cc
  0008e4e8: b 0x0008e578
  0008e4ea: vdiv.f32 s0,s2,s0
  0008e4ee: movs r2,#0x0
  0008e4f0: ldr r0,[0x0008e6b8]
  0008e4f2: mov.w r3,#0x3f800000
  0008e4f6: str.w r2,[r4,#0xb0]
  0008e4fa: str.w r3,[r4,#0xac]
  0008e4fe: strd r2,r2,[r4,#0xec]
  0008e502: add.w r2,r4,#0x9c
  0008e506: strd r3,r0,[r4,#0xb4]
  0008e50a: ldr r1,[0x0008e6b4]
  0008e50c: vdiv.f32 s2,s2,s16
  0008e510: vmov.i32 q8,#0x0
  0008e514: vneg.f32 s0,s0
  0008e518: vst1.32 {d16,d17},[r2]
  0008e51c: add.w r2,r4,#0xdc
  0008e520: vst1.32 {d16,d17},[r2]
  0008e524: add.w r2,r4,#0xcc
  0008e528: vst1.32 {d16,d17},[r2]
  0008e52c: add.w r2,r4,#0x8c
  0008e530: vst1.32 {d16,d17},[r2]
  0008e534: add.w r2,r4,#0x7c
  0008e538: vst1.32 {d16,d17},[r2]
  0008e53c: add.w r2,r4,#0xbc
  0008e540: vst1.32 {d16,d17},[r2]
  0008e544: str.w r1,[r4,#0xa0]
  0008e548: str.w r0,[r4,#0xa8]
  0008e54c: str.w r3,[r4,#0xe0]
  0008e550: str.w r0,[r4,#0xcc]
  0008e554: vstr.32 s2,[r4,#0x78]
  0008e558: vstr.32 s0,[r4,#0x8c]
  0008e55c: str.w r3,[r4,#0xf4]
  0008e560: ldr r0,[r4,#0x34]
  0008e562: blx 0x0006e8cc
  0008e566: vmov s0,r0
  0008e56a: vcvt.f32.s32 s0,s0
  0008e56e: ldr r0,[r4,#0x34]
  0008e570: vstr.32 s0,[r4,#0xe8]
  0008e574: blx 0x0006e8d8
  0008e578: vmov s0,r0
  0008e57c: vcvt.f32.s32 s0,s0
  0008e580: vstr.32 s0,[r4,#0xec]
  0008e584: b 0x0008e68e
  0008e586: vdiv.f32 s4,s2,s16
  0008e58a: movs r2,#0x0
  0008e58c: add.w r3,r4,#0x9c
  0008e590: str.w r2,[r4,#0xb0]
  0008e594: mov.w r2,#0x3f800000
  0008e598: movs r5,#0x3c
  0008e59a: str.w r2,[r4,#0xac]
  0008e59e: str.w r2,[r4,#0xb4]
  0008e5a2: ldr r0,[0x0008e6b8]
  0008e5a4: ldr r1,[0x0008e6b4]
  0008e5a6: vdiv.f32 s0,s2,s0
  0008e5aa: vmov.i32 q8,#0x0
  0008e5ae: vneg.f32 s2,s4
  0008e5b2: vst1.32 {d16,d17},[r3]
  0008e5b6: add.w r3,r4,#0xd8
  0008e5ba: vst1.32 {d16,d17},[r3]
  0008e5be: add.w r3,r4,#0xc8
  0008e5c2: vst1.32 {d16,d17},[r3]
  0008e5c6: add.w r3,r4,#0x8c
  0008e5ca: vst1.32 {d16,d17},[r3]
  0008e5ce: add.w r3,r4,#0x7c
  0008e5d2: vst1.32 {d16,d17},[r3]
  0008e5d6: add.w r3,r4,#0xe4
  0008e5da: vst1.32 {d16,d17},[r3]
  0008e5de: add.w r3,r4,#0xb8
  0008e5e2: vst1.32 {d16,d17},[r3],r5
  0008e5e6: str.w r2,[r4,#0xbc]
  0008e5ea: str.w r1,[r4,#0xa0]
  0008e5ee: str.w r0,[r4,#0xa8]
  0008e5f2: str.w r2,[r4,#0xe0]
  0008e5f6: str.w r0,[r4,#0xc8]
  0008e5fa: vstr.32 s0,[r4,#0x78]
  0008e5fe: vstr.32 s2,[r4,#0x8c]
  0008e602: str r2,[r3,#0x0]
  0008e604: ldr r0,[r4,#0x34]
  0008e606: blx 0x0006e8d8
  0008e60a: vmov s0,r0
  0008e60e: vcvt.f32.s32 s0,s0
  0008e612: vstr.32 s0,[r4,#0xe8]
  0008e616: b 0x0008e68e
  0008e618: vdiv.f32 s0,s2,s0
  0008e61c: movs r2,#0x0
  0008e61e: mov.w r3,#0x3f800000
  0008e622: str.w r2,[r4,#0xb0]
  0008e626: str.w r3,[r4,#0xac]
  0008e62a: strd r2,r2,[r4,#0xec]
  0008e62e: add.w r2,r4,#0x9c
  0008e632: strd r3,r3,[r4,#0xb4]
  0008e636: ldr r0,[0x0008e6b8]
  0008e638: ldr r1,[0x0008e6b4]
  0008e63a: vdiv.f32 s2,s2,s16
  0008e63e: vmov.i32 q8,#0x0
  0008e642: vneg.f32 s0,s0
  0008e646: vst1.32 {d16,d17},[r2]
  0008e64a: add.w r2,r4,#0xdc
  0008e64e: vst1.32 {d16,d17},[r2]
  0008e652: add.w r2,r4,#0xcc
  0008e656: vst1.32 {d16,d17},[r2]
  0008e65a: add.w r2,r4,#0x8c
  0008e65e: vst1.32 {d16,d17},[r2]
  0008e662: add.w r2,r4,#0x7c
  0008e666: vst1.32 {d16,d17},[r2]
  0008e66a: add.w r2,r4,#0xbc
  0008e66e: vst1.32 {d16,d17},[r2]
  0008e672: str.w r1,[r4,#0xa0]
  0008e676: str.w r0,[r4,#0xa8]
  0008e67a: str.w r3,[r4,#0xe0]
  0008e67e: str.w r3,[r4,#0xcc]
  0008e682: vstr.32 s2,[r4,#0x78]
  0008e686: vstr.32 s0,[r4,#0x8c]
  0008e68a: str.w r3,[r4,#0xf4]
  0008e68e: ldr.w r0,[r4,#0x170]
  0008e692: adds r1,r0,#0x1
  0008e694: beq 0x0008e6ae
  0008e696: ldr.w r1,[r4,#0x168]
  0008e69a: ldr.w r0,[r1,r0,lsl #0x2]
  0008e69e: ldmia r0!,{r1,r2,r3}
  0008e6a0: mov r0,r4
  0008e6a2: vpop {d8}
  0008e6a6: pop.w {r4,r5,r7,lr}
  0008e6aa: b.w 0x001ab2c8
  0008e6ae: vpop {d8}
  0008e6b2: pop {r4,r5,r7,pc}
```
