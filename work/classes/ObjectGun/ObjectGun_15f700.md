# ObjectGun::ObjectGun
elf 0x15f700 body 444
Sig: undefined __thiscall ObjectGun(ObjectGun * this, int param_1, Gun * param_2, int param_3, uint param_4, Level * param_5)

## decompile
```c

/* ObjectGun::ObjectGun(int, Gun*, int, unsigned int, Level*) */

ObjectGun * __thiscall
ObjectGun::ObjectGun
          (ObjectGun *this,int param_1,Gun *param_2,int param_3,uint param_4,Level *param_5)

{
  undefined8 uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  Array<Explosion*> *this_00;
  void *pvVar5;
  Explosion *this_01;
  AEGeometry *this_02;
  ObjectGun OVar6;
  undefined4 *puVar7;
  uint *puVar8;
  uint *puVar9;
  
  iVar3 = *(int *)(DAT_0016f8f0 + 0x16f718);
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 100) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x68) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x6c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x58) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x5c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x70) = 0;
  *(int *)this = iVar3 + 8;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x74));
  puVar7 = (undefined4 *)(DAT_0016f8f4 + 0x16f740);
  this[0x24] = (ObjectGun)0x0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  puVar9 = (uint *)*puVar7;
  *(undefined4 *)(this + 4) = 0xffffffff;
  *(Gun **)(this + 8) = param_2;
  *(Level **)(this + 0xc) = param_5;
  *(undefined4 *)(this + 0x14) = 0xffffffff;
  AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar9,(uint *)(this + 0x10));
  *(int *)(this + 0x28) = param_3;
  AbyssEngine::PaintCanvas::TransformAddMesh
            (*puVar9,(ushort)*(undefined4 *)(this + 0x10),SUB41(param_3,0));
  *(undefined4 *)(this + 0x20) = 0;
  uVar1 = DAT_0016f8e0;
  *(undefined8 *)(this + 0x3c) = DAT_0016f8d8;
  *(undefined8 *)(this + 0x44) = uVar1;
  iVar3 = DAT_0016f8f8;
  uVar4 = *(uint *)(param_2 + 0x5c);
  if (uVar4 < 9) {
    OVar6 = (ObjectGun)0x1;
    if ((1 << (uVar4 & 0xff) & 0x10aU) == 0) {
      OVar6 = (ObjectGun)0x0;
    }
  }
  else {
    OVar6 = (ObjectGun)0x0;
  }
  this[0x4c] = OVar6;
  uVar2 = DAT_0016f8e8;
  if (**(char **)(iVar3 + 0x16f7a6) == '\0') {
LAB_0016f7ba:
    if (uVar4 == 0x19) {
LAB_0016f7da:
      this_00 = operator_new(0xc);
      Array<Explosion*>::Array(this_00);
      *(Array<Explosion*> **)(this + 0x2c) = this_00;
      ArraySetLength<Explosion*>(*(uint *)(param_2 + 8),(Array *)this_00);
      puVar8 = *(uint **)(this + 0x2c);
      pvVar5 = operator_new__(*puVar8);
      *(void **)(this + 0x30) = pvVar5;
      for (uVar4 = 0; uVar4 < *puVar8; uVar4 = uVar4 + 1) {
        this_01 = operator_new(0x68);
        iVar3 = 10;
        if (*(int *)(param_2 + 0x58) == 0xb1) {
          iVar3 = 9;
        }
        if (*(int *)(param_2 + 0x58) == 0xb0) {
          iVar3 = 8;
        }
        Explosion::Explosion(this_01,iVar3);
        *(Explosion **)(*(int *)(*(int *)(this + 0x2c) + 4) + uVar4 * 4) = this_01;
        Explosion::setWeaponIndex
                  (*(Explosion **)(*(int *)(*(int *)(this + 0x2c) + 4) + uVar4 * 4),
                   *(int *)(param_2 + 0x58));
        *(undefined1 *)(*(int *)(this + 0x30) + uVar4) = 1;
        puVar8 = *(uint **)(this + 0x2c);
      }
    }
    else if (uVar4 == 0xb) {
LAB_0016f7c2:
      uVar2 = DAT_0016f8ec;
      *(undefined4 *)(this + 0x3c) = DAT_0016f8ec;
      *(undefined4 *)(this + 0x40) = uVar2;
      *(undefined4 *)(this + 0x44) = uVar2;
    }
  }
  else {
    if (uVar4 == 1 || uVar4 == 3) {
LAB_0016f7b2:
      *(undefined4 *)(this + 0x3c) = DAT_0016f8e8;
      *(undefined4 *)(this + 0x40) = uVar2;
      *(undefined4 *)(this + 0x44) = uVar2;
      goto LAB_0016f7ba;
    }
    if (uVar4 == 0x19) goto LAB_0016f7da;
    if (uVar4 == 0xb) goto LAB_0016f7c2;
    if (uVar4 == 8) goto LAB_0016f7b2;
  }
  if (param_2[0xa8] == (Gun)0x0) {
    iVar3 = Gun::isPlayerGun();
    if ((iVar3 == 0) || (*(int *)(DAT_0016f900 + 0x16f88a + *(int *)(param_2 + 0x58) * 4) < 0)) {
      this_02 = (AEGeometry *)0x0;
      this[0x1c] = (ObjectGun)0x0;
      goto LAB_0016f8a8;
    }
    iVar3 = *(int *)(param_2 + 0x5c);
    this[0x1c] = (ObjectGun)(iVar3 != 0xb);
    if (iVar3 == 0xb) {
      this_02 = (AEGeometry *)0x0;
      goto LAB_0016f8a8;
    }
  }
  else {
    this[0x1c] = (ObjectGun)0x1;
  }
  this_02 = operator_new(0xc0);
  AEGeometry::AEGeometry
            (this_02,*(ushort *)(DAT_0016f8fc + 0x16f868 + *(int *)(param_2 + 0x58) * 4),
             (PaintCanvas *)*puVar9,false);
LAB_0016f8a8:
  this[0x1d] = (ObjectGun)0x0;
  *(AEGeometry **)(this + 0x18) = this_02;
  *(undefined4 *)(this + 0x34) = 0;
  return this;
}

```

## target disasm
```
  0016f700: push {r4,r5,r6,r7,lr}
  0016f702: add r7,sp,#0xc
  0016f704: push {r8,r9,r10}
  0016f708: mov r4,r0
  0016f70a: ldr r0,[0x0016f8f0]
  0016f70c: vmov.i32 q8,#0x0
  0016f710: add.w r1,r4,#0x60
  0016f714: add r0,pc
  0016f716: movs r5,#0x0
  0016f718: mov r6,r3
  0016f71a: mov r9,r2
  0016f71c: ldr r0,[r0,#0x0]
  0016f71e: vst1.32 {d16,d17},[r1]
  0016f722: add.w r1,r4,#0x50
  0016f726: adds r0,#0x8
  0016f728: vst1.32 {d16,d17},[r1]
  0016f72c: str r5,[r4,#0x70]
  0016f72e: str r0,[r4,#0x0]
  0016f730: add.w r0,r4,#0x74
  0016f734: blx 0x0006f118
  0016f738: ldr r1,[0x0016f8f4]
  0016f73a: ldr r0,[r7,#0xc]
  0016f73c: add r1,pc
  0016f73e: strb.w r5,[r4,#0x24]
  0016f742: strd r5,r5,[r4,#0x2c]
  0016f746: ldr.w r8,[r1,#0x0]
  0016f74a: mov.w r1,#0xffffffff
  0016f74e: strd r1,r9,[r4,#0x4]
  0016f752: str r0,[r4,#0xc]
  0016f754: str r1,[r4,#0x14]
  0016f756: ldr.w r0,[r8,#0x0]
  0016f75a: add.w r1,r4,#0x10
  0016f75e: blx 0x000720ac
  0016f762: ldr r1,[r4,#0x10]
  0016f764: str r6,[r4,#0x28]
  0016f766: ldr.w r0,[r8,#0x0]
  0016f76a: uxth r2,r6
  0016f76c: movs r3,#0x0
  0016f76e: movs r5,#0x0
  0016f770: blx 0x0007303c
  0016f774: adr r0,[0x16f8d8]
  0016f776: str r5,[r4,#0x20]
  0016f778: vld1.64 {d16,d17},[r0]
  0016f77c: add.w r0,r4,#0x3c
  0016f780: vst1.32 {d16,d17},[r0]
  0016f784: ldr.w r0,[r9,#0x5c]
  0016f788: cmp r0,#0x8
  0016f78a: bhi.w 0x0016f8b8
  0016f78e: movs r1,#0x1
  0016f790: lsl.w r2,r1,r0
  0016f794: tst r2,#0x10a
  0016f798: it eq
  0016f79a: mov.eq r1,#0x0
  0016f79c: ldr r2,[0x0016f8f8]
  0016f79e: strb.w r1,[r4,#0x4c]
  0016f7a2: add r2,pc
  0016f7a4: ldr r2,[r2,#0x0]
  0016f7a6: ldrb r1,[r2,#0x0]
  0016f7a8: cbz r1,0x0016f7ba
  0016f7aa: cmp r0,#0x1
  0016f7ac: it ne
  0016f7ae: cmp.ne r0,#0x3
  0016f7b0: bne 0x0016f7cc
  0016f7b2: ldr r1,[0x0016f8e8]
  0016f7b4: strd r1,r1,[r4,#0x3c]
  0016f7b8: str r1,[r4,#0x44]
  0016f7ba: cmp r0,#0x19
  0016f7bc: beq 0x0016f7da
  0016f7be: cmp r0,#0xb
  0016f7c0: bne 0x0016f84c
  0016f7c2: ldr r0,[0x0016f8ec]
  0016f7c4: strd r0,r0,[r4,#0x3c]
  0016f7c8: str r0,[r4,#0x44]
  0016f7ca: b 0x0016f84c
  0016f7cc: cmp r0,#0x19
  0016f7ce: beq 0x0016f7da
  0016f7d0: cmp r0,#0xb
  0016f7d2: beq 0x0016f7c2
  0016f7d4: cmp r0,#0x8
  0016f7d6: beq 0x0016f7b2
  0016f7d8: b 0x0016f84c
  0016f7da: movs r0,#0xc
  0016f7dc: blx 0x0006eb24
  0016f7e0: mov r6,r0
  0016f7e2: blx 0x000775a8
  0016f7e6: str r6,[r4,#0x2c]
  0016f7e8: ldr.w r0,[r9,#0x8]
  0016f7ec: mov r1,r6
  0016f7ee: blx 0x000775b4
  0016f7f2: ldr r6,[r4,#0x2c]
  0016f7f4: ldr r0,[r6,#0x0]
  0016f7f6: blx 0x0006ec08
  0016f7fa: movs r5,#0x0
  0016f7fc: mov.w r10,#0x1
  0016f800: str r0,[r4,#0x30]
  0016f802: b 0x0016f846
  0016f804: movs r0,#0x68
  0016f806: blx 0x0006eb24
  0016f80a: mov r6,r0
  0016f80c: ldr.w r0,[r9,#0x58]
  0016f810: movs r1,#0xa
  0016f812: cmp r0,#0xb1
  0016f814: it eq
  0016f816: mov.eq r1,#0x9
  0016f818: cmp r0,#0xb0
  0016f81a: it eq
  0016f81c: mov.eq r1,#0x8
  0016f81e: mov r0,r6
  0016f820: blx 0x000727f0
  0016f824: ldr r0,[r4,#0x2c]
  0016f826: ldr r0,[r0,#0x4]
  0016f828: str.w r6,[r0,r5,lsl #0x2]
  0016f82c: ldr r0,[r4,#0x2c]
  0016f82e: ldr.w r1,[r9,#0x58]
  0016f832: ldr r0,[r0,#0x4]
  0016f834: ldr.w r0,[r0,r5,lsl #0x2]
  0016f838: blx 0x00076dc8
  0016f83c: ldr r0,[r4,#0x30]
  0016f83e: strb.w r10,[r0,r5]
  0016f842: adds r5,#0x1
  0016f844: ldr r6,[r4,#0x2c]
  0016f846: ldr r0,[r6,#0x0]
  0016f848: cmp r5,r0
  0016f84a: bcc 0x0016f804
  0016f84c: ldrb.w r0,[r9,#0xa8]
  0016f850: cbz r0,0x0016f878
  0016f852: movs r0,#0x1
  0016f854: strb r0,[r4,#0x1c]
  0016f856: movs r0,#0xc0
  0016f858: blx 0x0006eb24
  0016f85c: mov r6,r0
  0016f85e: ldr r0,[0x0016f8fc]
  0016f860: ldr.w r1,[r9,#0x58]
  0016f864: add r0,pc
  0016f866: ldr.w r2,[r8,#0x0]
  0016f86a: ldrh.w r1,[r0,r1,lsl #0x2]
  0016f86e: mov r0,r6
  0016f870: movs r3,#0x0
  0016f872: blx 0x0007207c
  0016f876: b 0x0016f8a8
  0016f878: mov r0,r9
  0016f87a: blx 0x0007798c
  0016f87e: cbz r0,0x0016f8a4
  0016f880: ldr r0,[0x0016f900]
  0016f882: ldr.w r1,[r9,#0x58]
  0016f886: add r0,pc
  0016f888: ldr.w r0,[r0,r1,lsl #0x2]
  0016f88c: cmp.w r0,#0xffffffff
  0016f890: ble 0x0016f8a4
  0016f892: ldr.w r0,[r9,#0x5c]
  0016f896: subs r0,#0xb
  0016f898: it ne
  0016f89a: mov.ne r0,#0x1
  0016f89c: strb r0,[r4,#0x1c]
  0016f89e: bne 0x0016f856
  0016f8a0: movs r6,#0x0
  0016f8a2: b 0x0016f8a8
  0016f8a4: movs r6,#0x0
  0016f8a6: strb r6,[r4,#0x1c]
  0016f8a8: movs r0,#0x0
  0016f8aa: strb r0,[r4,#0x1d]
  0016f8ac: str r6,[r4,#0x18]
  0016f8ae: str r0,[r4,#0x34]
  0016f8b0: mov r0,r4
  0016f8b2: pop.w {r8,r9,r10}
  0016f8b6: pop {r4,r5,r6,r7,pc}
  0016f8b8: movs r1,#0x0
  0016f8ba: b 0x0016f79c
```
