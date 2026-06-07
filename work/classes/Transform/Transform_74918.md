# Transform::Transform
elf 0x74918 body 508
Sig: undefined __thiscall Transform(Transform * this, Transform * param_1)

## decompile
```c

/* AbyssEngine::Transform::Transform(AbyssEngine::Transform*) */

void __thiscall AbyssEngine::Transform::Transform(Transform *this,Transform *param_1)

{
  Mesh *this_00;
  Transform *this_01;
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  uint uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  Transform aTStack_1c0 [388];
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  piVar3 = *(int **)(DAT_00084b68 + 0x84930);
  local_3c = *piVar3;
  AEMath::Matrix::Matrix((Matrix *)this);
  Array<AbyssEngine::Mesh*>::Array((Array<AbyssEngine::Mesh*> *)(this + 0x3c));
  Array<AbyssEngine::Transform*>::Array((Array<AbyssEngine::Transform*> *)(this + 0x4c));
  AEMath::Matrix::Matrix((Matrix *)(this + 0x5c));
  AEMath::Matrix::Matrix((Matrix *)(this + 0x98));
  *(undefined4 *)(this + 0xd4) = 0;
  *(undefined4 *)(this + 0xd8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0xdc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xe0) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0xe4) = 0x3f800000;
  Array<AbyssEngine::KeyFrame*>::Array((Array<AbyssEngine::KeyFrame*> *)(this + 0x11c));
  Quaternion::Quaternion((Quaternion *)(this + 0x128));
  uVar2 = 0;
  uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar6 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar7 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x148) = 0;
  *(undefined4 *)(this + 0x14c) = 0;
  *(undefined4 *)(this + 0x138) = 0;
  *(undefined4 *)(this + 0x13c) = uVar5;
  *(undefined4 *)(this + 0x140) = uVar6;
  *(undefined4 *)(this + 0x144) = uVar7;
  Quaternion::Quaternion((Quaternion *)(this + 0x150));
  *(undefined4 *)(this + 0x170) = 0;
  *(undefined4 *)(this + 0x174) = 0;
  *(undefined4 *)(this + 0x160) = uVar2;
  *(undefined4 *)(this + 0x164) = uVar5;
  *(undefined4 *)(this + 0x168) = uVar6;
  *(undefined4 *)(this + 0x16c) = uVar7;
  if (param_1 == (Transform *)0x0) {
    Transform(aTStack_1c0);
    ~Transform(aTStack_1c0);
  }
  else {
    uVar2 = *(undefined4 *)(param_1 + 0xe8);
    this[0xed] = (Transform)0x1;
    *(undefined4 *)(this + 0xe8) = uVar2;
    *(undefined4 *)(this + 0xf0) = 0x3f800000;
    uVar2 = *(undefined4 *)(param_1 + 0xf8);
    uVar5 = *(undefined4 *)(param_1 + 0xfc);
    uVar6 = *(undefined4 *)(param_1 + 0x100);
    uVar7 = *(undefined4 *)(param_1 + 0x104);
    *(undefined4 *)(this + 0x48) = 0xffffffff;
    *(undefined4 *)(this + 0xf8) = uVar2;
    *(undefined4 *)(this + 0xfc) = uVar5;
    *(undefined4 *)(this + 0x100) = uVar6;
    *(undefined4 *)(this + 0x104) = uVar7;
    uVar2 = *(undefined4 *)(param_1 + 0x10c);
    uVar5 = *(undefined4 *)(param_1 + 0x110);
    uVar6 = *(undefined4 *)(param_1 + 0x114);
    *(undefined4 *)(this + 0x108) = *(undefined4 *)(param_1 + 0x108);
    *(undefined4 *)(this + 0x10c) = uVar2;
    *(undefined4 *)(this + 0x110) = uVar5;
    *(undefined4 *)(this + 0x114) = uVar6;
    *(undefined4 *)(this + 0x118) = *(undefined4 *)(param_1 + 0x118);
    uVar2 = *(undefined4 *)(param_1 + 300);
    uVar5 = *(undefined4 *)(param_1 + 0x130);
    uVar6 = *(undefined4 *)(param_1 + 0x134);
    *(undefined4 *)(this + 0x128) = *(undefined4 *)(param_1 + 0x128);
    *(undefined4 *)(this + 300) = uVar2;
    *(undefined4 *)(this + 0x130) = uVar5;
    *(undefined4 *)(this + 0x134) = uVar6;
    AEMath::Vector::operator=((Vector *)(this + 0x138),(Vector *)(param_1 + 0x138));
    AEMath::Vector::operator=((Vector *)(this + 0x144),(Vector *)(param_1 + 0x144));
    AEMath::Matrix::operator=((Matrix *)(this + 0x5c),(Matrix *)(param_1 + 0x5c));
    uVar2 = *(undefined4 *)(param_1 + 0x154);
    uVar5 = *(undefined4 *)(param_1 + 0x158);
    uVar6 = *(undefined4 *)(param_1 + 0x15c);
    *(undefined4 *)(this + 0x150) = *(undefined4 *)(param_1 + 0x150);
    *(undefined4 *)(this + 0x154) = uVar2;
    *(undefined4 *)(this + 0x158) = uVar5;
    *(undefined4 *)(this + 0x15c) = uVar6;
    AEMath::Vector::operator=((Vector *)(this + 0x160),(Vector *)(param_1 + 0x160));
    AEMath::Vector::operator=((Vector *)(this + 0x16c),(Vector *)(param_1 + 0x16c));
    AEMath::Matrix::operator=((Matrix *)(this + 0x98),(Matrix *)(param_1 + 0x98));
    *(undefined4 *)(this + 0x58) = *(undefined4 *)(param_1 + 0x58);
    for (uVar4 = 0; uVar4 < *(uint *)(param_1 + 0x3c); uVar4 = uVar4 + 1) {
      this_00 = operator_new(0x88);
      Mesh::Mesh(this_00,*(Mesh **)(*(int *)(param_1 + 0x40) + uVar4 * 4));
      ArrayAdd<AbyssEngine::Mesh*>(this_00,(Array *)(this + 0x3c));
    }
    ArraySet<AbyssEngine::KeyFrame*>((Array *)(param_1 + 0x11c),(Array *)(this + 0x11c));
    for (uVar4 = 0; uVar4 < *(uint *)(param_1 + 0x4c); uVar4 = uVar4 + 1) {
      this_01 = operator_new(0x180);
      Transform(this_01,*(Transform **)(*(int *)(param_1 + 0x50) + uVar4 * 4));
      ArrayAdd<AbyssEngine::Transform*>(this_01,(Array *)(this + 0x4c));
    }
    AEMath::BSphere::operator=((BSphere *)(this + 0xd4),(BSphere *)(param_1 + 0xd4));
    this[0xec] = (Transform)0x1;
    *(undefined2 *)(this + 0x17c) = 0x101;
  }
  iVar1 = *piVar3 - local_3c;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  00084918: push {r4,r5,r6,r7,lr}
  0008491a: add r7,sp,#0xc
  0008491c: push {r7,r8,r9,r10,r11}
  00084920: vpush {d8,d9}
  00084924: sub sp,#0x1a8
  00084926: mov r8,r0
  00084928: ldr r0,[0x00084b68]
  0008492a: mov r5,r1
  0008492c: add r0,pc
  0008492e: ldr r6,[r0,#0x0]
  00084930: ldr r0,[r6,#0x0]
  00084932: str r0,[sp,#0x1a4]
  00084934: mov r0,r8
  00084936: blx 0x0006f118
  0008493a: add.w r10,r8,#0x3c
  0008493e: mov r0,r10
  00084940: blx 0x0006f724
  00084944: add.w r11,r8,#0x4c
  00084948: mov r0,r11
  0008494a: blx 0x0006f730
  0008494e: add.w r9,r8,#0x5c
  00084952: mov r0,r9
  00084954: blx 0x0006f118
  00084958: add.w r4,r8,#0x98
  0008495c: mov r0,r4
  0008495e: blx 0x0006f118
  00084962: vmov.i32 q8,#0x0
  00084966: add.w r0,r8,#0xd4
  0008496a: str r0,[sp,#0x4]
  0008496c: mov.w r1,#0x3f800000
  00084970: vst1.32 {d16,d17},[r0]!
  00084974: str r1,[r0,#0x0]
  00084976: add.w r0,r8,#0x11c
  0008497a: str r0,[sp,#0xc]
  0008497c: blx 0x0006f73c
  00084980: add.w r0,r8,#0x128
  00084984: str r6,[sp,#0x8]
  00084986: str.w r11,[sp,#0x14]
  0008498a: str r0,[sp,#0x1c]
  0008498c: blx 0x0006f124
  00084990: vmov.i32 q4,#0x0
  00084994: add.w r11,r8,#0x138
  00084998: add.w r0,r8,#0x150
  0008499c: movs r6,#0x0
  0008499e: str.w r10,[sp,#0x10]
  000849a2: strd r6,r6,[r8,#0x148]
  000849a6: vst1.32 {d8,d9},[r11]
  000849aa: str r0,[sp,#0x18]
  000849ac: blx 0x0006f124
  000849b0: add.w r10,r8,#0x160
  000849b4: cmp r5,#0x0
  000849b6: strd r6,r6,[r8,#0x170]
  000849ba: vst1.32 {d8,d9},[r10]
  000849be: beq.w 0x00084ae8
  000849c2: movs r0,#0x1
  000849c4: ldr.w r1,[r5,#0xe8]
  000849c8: strb.w r0,[r8,#0xed]
  000849cc: mov.w r0,#0x3f800000
  000849d0: str.w r1,[r8,#0xe8]
  000849d4: str.w r0,[r8,#0xf0]
  000849d8: add.w r0,r5,#0xf8
  000849dc: vld1.64 {d16,d17},[r0]
  000849e0: mov.w r0,#0xffffffff
  000849e4: str.w r0,[r8,#0x48]
  000849e8: add.w r0,r8,#0xf8
  000849ec: vst1.64 {d16,d17},[r0]
  000849f0: add.w r0,r5,#0x108
  000849f4: vld1.64 {d16,d17},[r0]
  000849f8: add.w r0,r8,#0x108
  000849fc: vst1.64 {d16,d17},[r0]
  00084a00: ldr.w r0,[r5,#0x118]
  00084a04: str.w r0,[r8,#0x118]
  00084a08: add.w r0,r5,#0x128
  00084a0c: vld1.32 {d16,d17},[r0]
  00084a10: ldr r0,[sp,#0x1c]
  00084a12: vst1.32 {d16,d17},[r0]
  00084a16: add.w r1,r5,#0x138
  00084a1a: mov r0,r11
  00084a1c: blx 0x0006eb3c
  00084a20: add.w r0,r8,#0x144
  00084a24: add.w r1,r5,#0x144
  00084a28: blx 0x0006eb3c
  00084a2c: add.w r1,r5,#0x5c
  00084a30: mov r0,r9
  00084a32: blx 0x0006f148
  00084a36: add.w r0,r5,#0x150
  00084a3a: vld1.32 {d16,d17},[r0]
  00084a3e: ldr r0,[sp,#0x18]
  00084a40: vst1.32 {d16,d17},[r0]
  00084a44: add.w r1,r5,#0x160
  00084a48: mov r0,r10
  00084a4a: blx 0x0006eb3c
  00084a4e: add.w r0,r8,#0x16c
  00084a52: add.w r1,r5,#0x16c
  00084a56: blx 0x0006eb3c
  00084a5a: add.w r1,r5,#0x98
  00084a5e: mov r0,r4
  00084a60: blx 0x0006f148
  00084a64: ldr r0,[r5,#0x58]
  00084a66: movs r6,#0x0
  00084a68: str.w r0,[r8,#0x58]
  00084a6c: ldrd r10,r11,[sp,#0x10]
  00084a70: b 0x00084a90
  00084a72: movs r0,#0x88
  00084a74: blx 0x0006eb24
  00084a78: mov r4,r0
  00084a7a: ldr r0,[r5,#0x40]
  00084a7c: ldr.w r1,[r0,r6,lsl #0x2]
  00084a80: mov r0,r4
  00084a82: blx 0x0006f76c
  00084a86: mov r0,r4
  00084a88: mov r1,r10
  00084a8a: blx 0x0006ecbc
  00084a8e: adds r6,#0x1
  00084a90: ldr r0,[r5,#0x3c]
  00084a92: cmp r6,r0
  00084a94: bcc 0x00084a72
  00084a96: ldr r1,[sp,#0xc]
  00084a98: add.w r0,r5,#0x11c
  00084a9c: blx 0x0006f778
  00084aa0: ldrd r10,r11,[sp,#0x10]
  00084aa4: movs r6,#0x0
  00084aa6: b 0x00084ac8
  00084aa8: mov.w r0,#0x180
  00084aac: blx 0x0006eb24
  00084ab0: mov r4,r0
  00084ab2: ldr r0,[r5,#0x50]
  00084ab4: ldr.w r1,[r0,r6,lsl #0x2]
  00084ab8: mov r0,r4
  00084aba: blx 0x0006f784
  00084abe: mov r0,r4
  00084ac0: mov r1,r11
  00084ac2: blx 0x0006f790
  00084ac6: adds r6,#0x1
  00084ac8: ldr r0,[r5,#0x4c]
  00084aca: cmp r6,r0
  00084acc: bcc 0x00084aa8
  00084ace: ldr r0,[sp,#0x4]
  00084ad0: add.w r1,r5,#0xd4
  00084ad4: blx 0x0006eb18
  00084ad8: movs r0,#0x1
  00084ada: strb.w r0,[r8,#0xec]
  00084ade: movw r0,#0x101
  00084ae2: strh.w r0,[r8,#0x17c]
  00084ae6: b 0x00084af4
  00084ae8: add r0,sp,#0x20
  00084aea: blx 0x0006f79c
  00084aee: add r0,sp,#0x20
  00084af0: blx 0x0006f7a8
  00084af4: ldr r0,[sp,#0x1a4]
  00084af6: ldr r1,[sp,#0x8]
  00084af8: ldr r1,[r1,#0x0]
  00084afa: subs r0,r1,r0
  00084afc: itttt eq
  00084afe: mov.eq r0,r8
  00084b00: add.eq sp,#0x1a8
  00084b02: vpop.eq {d8,d9}
  00084b06: add.eq sp,#0x4
  00084b08: itt eq
  00084b0a: pop.eq.w {r8,r9,r10,r11}
  00084b0e: pop.eq {r4,r5,r6,r7,pc}
  00084b10: blx 0x0006e824
```
