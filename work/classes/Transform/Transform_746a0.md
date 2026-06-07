# Transform::Transform
elf 0x746a0 body 394
Sig: undefined __thiscall Transform(Transform * this)

## decompile
```c

/* AbyssEngine::Transform::Transform() */

void __thiscall AbyssEngine::Transform::Transform(Transform *this)

{
  Quaternion *this_00;
  Vector *pVVar1;
  undefined4 *puVar2;
  Matrix *pMVar3;
  int iVar4;
  int *piVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined4 local_40;
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  piVar5 = *(int **)(DAT_0008487c + 0x846b6);
  local_3c = *piVar5;
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
  uVar6 = 0;
  uVar7 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar9 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x148) = 0;
  *(undefined4 *)(this + 0x14c) = 0;
  *(undefined4 *)(this + 0x138) = 0;
  *(undefined4 *)(this + 0x13c) = uVar7;
  *(undefined4 *)(this + 0x140) = uVar8;
  *(undefined4 *)(this + 0x144) = uVar9;
  Quaternion::Quaternion((Quaternion *)(this + 0x150));
  *(undefined4 *)(this + 0xf8) = 0;
  *(undefined4 *)(this + 0xfc) = 0;
  *(undefined4 *)(this + 0x170) = 0;
  *(undefined4 *)(this + 0x174) = 0;
  *(undefined4 *)(this + 0x48) = 0xffffffff;
  this[0xed] = (Transform)0x1;
  *(undefined4 *)(this + 0xf0) = 0x3f800000;
  *(undefined4 *)(this + 0x110) = 0;
  *(undefined4 *)(this + 0x114) = 0;
  *(undefined4 *)(this + 0x118) = 0;
  *(undefined4 *)(this + 0x160) = uVar6;
  *(undefined4 *)(this + 0x164) = uVar7;
  *(undefined4 *)(this + 0x168) = uVar8;
  *(undefined4 *)(this + 0x16c) = uVar9;
  *(undefined4 *)(this + 0x100) = uVar6;
  *(undefined4 *)(this + 0x104) = uVar7;
  *(undefined4 *)(this + 0x108) = uVar8;
  *(undefined4 *)(this + 0x10c) = uVar9;
  this_00 = (Quaternion *)Quaternion::Quaternion((Quaternion *)&local_78);
  uVar6 = *(undefined4 *)(this_00 + 4);
  uVar7 = *(undefined4 *)(this_00 + 8);
  uVar8 = *(undefined4 *)(this_00 + 0xc);
  *(undefined4 *)(this + 0x150) = *(undefined4 *)this_00;
  *(undefined4 *)(this + 0x154) = uVar6;
  *(undefined4 *)(this + 0x158) = uVar7;
  *(undefined4 *)(this + 0x15c) = uVar8;
  uVar6 = *(undefined4 *)(this_00 + 4);
  uVar7 = *(undefined4 *)(this_00 + 8);
  uVar8 = *(undefined4 *)(this_00 + 0xc);
  *(undefined4 *)(this + 0x128) = *(undefined4 *)this_00;
  *(undefined4 *)(this + 300) = uVar6;
  *(undefined4 *)(this + 0x130) = uVar7;
  *(undefined4 *)(this + 0x134) = uVar8;
  Quaternion::~Quaternion(this_00);
  local_78 = 0;
  uStack_74 = 0;
  local_70 = 0;
  pVVar1 = (Vector *)AEMath::Vector::operator=((Vector *)(this + 0x160),(Vector *)&local_78);
  AEMath::Vector::operator=((Vector *)(this + 0x138),pVVar1);
  local_78 = 0x3f800000;
  uStack_74 = 0x3f800000;
  local_70 = 0x3f800000;
  pVVar1 = (Vector *)AEMath::Vector::operator=((Vector *)(this + 0x16c),(Vector *)&local_78);
  AEMath::Vector::operator=((Vector *)(this + 0x144),pVVar1);
  uStack_48 = DAT_00084870;
  uStack_50 = DAT_00084868;
  uStack_5c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_58 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_54 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar2 = (undefined4 *)((uint)&local_78 | 4);
  *puVar2 = 0;
  puVar2[1] = uStack_5c;
  puVar2[2] = uStack_58;
  puVar2[3] = uStack_54;
  local_60 = 0;
  local_78 = 0x3f800000;
  local_64 = 0x3f800000;
  local_40 = 0x3f800000;
  pMVar3 = (Matrix *)AEMath::Matrix::operator=((Matrix *)(this + 0x98),(Matrix *)&local_78);
  AEMath::Matrix::operator=((Matrix *)(this + 0x5c),pMVar3);
  uVar6 = DAT_00084878;
  *(undefined4 *)(this + 0x58) = 2;
  *(undefined4 *)(this + 0xac) = uVar6;
  *(undefined2 *)(this + 0x17c) = 1;
  this[0xec] = (Transform)0x1;
  *(undefined4 *)(this + 0xe8) = 0;
  iVar4 = *piVar5 - local_3c;
  if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

```

## target disasm
```
  000846a0: push {r4,r5,r6,r7,lr}
  000846a2: add r7,sp,#0xc
  000846a4: push {r7,r8,r9,r10,r11}
  000846a8: vpush {d8,d9}
  000846ac: sub sp,#0x50
  000846ae: mov r6,r0
  000846b0: ldr r0,[0x0008487c]
  000846b2: add r0,pc
  000846b4: ldr r5,[r0,#0x0]
  000846b6: ldr r0,[r5,#0x0]
  000846b8: str r0,[sp,#0x4c]
  000846ba: mov r0,r6
  000846bc: blx 0x0006f118
  000846c0: add.w r0,r6,#0x3c
  000846c4: str r0,[sp,#0x8]
  000846c6: blx 0x0006f724
  000846ca: add.w r4,r6,#0x4c
  000846ce: mov r0,r4
  000846d0: blx 0x0006f730
  000846d4: add.w r10,r6,#0x5c
  000846d8: mov r0,r10
  000846da: str r4,[sp,#0x4]
  000846dc: blx 0x0006f118
  000846e0: add.w r8,r6,#0x98
  000846e4: mov r0,r8
  000846e6: blx 0x0006f118
  000846ea: vmov.i32 q8,#0x0
  000846ee: add.w r0,r6,#0xd4
  000846f2: mov.w r1,#0x3f800000
  000846f6: vst1.32 {d16,d17},[r0]!
  000846fa: str r1,[r0,#0x0]
  000846fc: add.w r0,r6,#0x11c
  00084700: blx 0x0006f73c
  00084704: add.w r11,r6,#0x128
  00084708: str r0,[sp,#0x0]
  0008470a: str r5,[sp,#0xc]
  0008470c: mov r0,r11
  0008470e: blx 0x0006f124
  00084712: vmov.i32 q4,#0x0
  00084716: add.w r9,r6,#0x138
  0008471a: add.w r5,r6,#0x150
  0008471e: movs r4,#0x0
  00084720: strd r4,r4,[r6,#0x148]
  00084724: vst1.32 {d8,d9},[r9]
  00084728: mov r0,r5
  0008472a: blx 0x0006f124
  0008472e: mov.w r0,#0xffffffff
  00084732: strd r4,r4,[r6,#0xf8]
  00084736: strd r4,r4,[r6,#0x170]
  0008473a: str r0,[r6,#0x48]
  0008473c: movs r0,#0x1
  0008473e: strb.w r0,[r6,#0xed]
  00084742: mov.w r0,#0x3f800000
  00084746: str.w r0,[r6,#0xf0]
  0008474a: add.w r0,r6,#0x100
  0008474e: strd r4,r4,[r6,#0x110]
  00084752: str.w r4,[r6,#0x118]
  00084756: add.w r4,r6,#0x160
  0008475a: vst1.32 {d8,d9},[r4]
  0008475e: vst1.64 {d8,d9},[r0]
  00084762: add r0,sp,#0x10
  00084764: blx 0x0006f124
  00084768: vld1.64 {d16,d17},[r0]
  0008476c: vst1.64 {d16,d17},[r5]
  00084770: vld1.64 {d16,d17},[r0]
  00084774: vst1.64 {d16,d17},[r11]
  00084778: blx 0x0006f13c
  0008477c: movs r0,#0x0
  0008477e: strd r0,r0,[sp,#0x10]
  00084782: str r0,[sp,#0x18]
  00084784: add r1,sp,#0x10
  00084786: mov r0,r4
  00084788: blx 0x0006eb3c
  0008478c: mov r1,r0
  0008478e: mov r0,r9
  00084790: blx 0x0006eb3c
  00084794: add.w r0,r6,#0x16c
  00084798: mov.w r1,#0x3f800000
  0008479c: strd r1,r1,[sp,#0x10]
  000847a0: str r1,[sp,#0x18]
  000847a2: add r4,sp,#0x10
  000847a4: mov r1,r4
  000847a6: blx 0x0006eb3c
  000847aa: mov r1,r0
  000847ac: add.w r0,r6,#0x144
  000847b0: blx 0x0006eb3c
  000847b4: adr r0,[0x84868]
  000847b6: vmov.i32 q8,#0x0
  000847ba: vld1.64 {d18,d19},[r0]
  000847be: orr r0,r4,#0x4
  000847c2: vst1.32 {d16,d17},[r0]
  000847c6: add.w r0,r4,#0x18
  000847ca: vst1.64 {d16,d17},[r0]
  000847ce: add.w r0,r4,#0x28
  000847d2: vst1.64 {d18,d19},[r0]
  000847d6: mov.w r0,#0x3f800000
  000847da: str r0,[sp,#0x10]
  000847dc: str r0,[sp,#0x24]
  000847de: str r0,[sp,#0x48]
  000847e0: mov r0,r8
  000847e2: mov r1,r4
  000847e4: blx 0x0006f148
  000847e8: mov r1,r0
  000847ea: mov r0,r10
  000847ec: blx 0x0006f148
  000847f0: movs r1,#0x2
  000847f2: ldr r0,[0x00084878]
  000847f4: str r1,[r6,#0x58]
  000847f6: str.w r0,[r6,#0xac]
  000847fa: movs r0,#0x1
  000847fc: strh.w r0,[r6,#0x17c]
  00084800: strb.w r0,[r6,#0xec]
  00084804: movs r0,#0x0
  00084806: str.w r0,[r6,#0xe8]
  0008480a: ldr r0,[sp,#0x4c]
  0008480c: ldr r1,[sp,#0xc]
  0008480e: ldr r1,[r1,#0x0]
  00084810: subs r0,r1,r0
  00084812: itttt eq
  00084814: mov.eq r0,r6
  00084816: add.eq sp,#0x50
  00084818: vpop.eq {d8,d9}
  0008481c: add.eq sp,#0x4
  0008481e: itt eq
  00084820: pop.eq.w {r8,r9,r10,r11}
  00084824: pop.eq {r4,r5,r6,r7,pc}
  00084826: blx 0x0006e824
```
