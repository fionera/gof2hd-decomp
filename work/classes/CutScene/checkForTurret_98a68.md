# CutScene::checkForTurret
elf 0x98a68 body 942
Sig: undefined __thiscall checkForTurret(CutScene * this)

## decompile
```c

/* CutScene::checkForTurret() */

void __thiscall CutScene::checkForTurret(CutScene *this)

{
  int iVar1;
  Ship *this_00;
  int *piVar2;
  AEGeometry *this_01;
  AEGeometry *this_02;
  AEGeometry *pAVar3;
  void *pvVar4;
  FileRead *this_03;
  Array *this_04;
  Array<AbyssEngine::AEMath::Vector*> *this_05;
  uint uVar5;
  PaintCanvas *this_06;
  undefined4 *puVar6;
  code *pcVar7;
  int iVar8;
  ushort uVar9;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float fVar10;
  float extraout_s0_02;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float fVar11;
  float extraout_s1_02;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar12;
  float extraout_s2_02;
  int local_48;
  ushort local_44;
  
  if (*(int *)(this + 100) != 0) {
    this_06 = (PaintCanvas *)**(undefined4 **)(DAT_000a8e24 + 0xa8a80);
    iVar1 = Level::getEnemies();
    AbyssEngine::PaintCanvas::TransformRemoveChild
              (this_06,*(uint *)(*(int *)(**(int **)(iVar1 + 4) + 8) + 0xc),
               *(uint *)(*(int *)(this + 100) + 0xc));
  }
  this_00 = (Ship *)Status::getShip();
  piVar2 = (int *)Ship::getEquipment(this_00,2);
  if (piVar2 == (int *)0x0) {
    return;
  }
  if (*piVar2 == 0) {
    return;
  }
  if (*(Item **)piVar2[1] == (Item *)0x0) {
    return;
  }
  iVar1 = Item::getIndex(*(Item **)piVar2[1]);
  if (iVar1 == 0xe0) {
    uVar9 = 0x499a;
    iVar1 = 0x499c;
    iVar8 = 0x499d;
    local_48 = -1;
    local_44 = 0x499b;
    goto LAB_000a8c12;
  }
  if (iVar1 == 0x30) {
    uVar9 = 0x1a74;
    local_44 = 0x1a75;
  }
  else if (iVar1 == 0x31) {
    uVar9 = 0x1a76;
    local_44 = 0x1a77;
  }
  else if (iVar1 == 0xb4) {
    uVar9 = 0x1a95;
    local_44 = 0x1a96;
  }
  else if (iVar1 == 0xb5) {
    uVar9 = 0x1a97;
    local_44 = 0x1a98;
  }
  else if (iVar1 == 0xb6) {
    uVar9 = 0x1a99;
    local_44 = 0x1a9a;
  }
  else {
    if (iVar1 == 0xc6) {
      uVar9 = 0x4963;
      iVar1 = 0x4964;
      local_48 = -1;
      local_44 = 0x4967;
      iVar8 = 0x4966;
      goto LAB_000a8c12;
    }
    if (iVar1 == 199) {
      uVar9 = 0x4968;
      iVar1 = 0x4969;
      local_48 = -1;
      local_44 = 0x496b;
      iVar8 = 0x496a;
      goto LAB_000a8c12;
    }
    if (iVar1 == 200) {
      uVar9 = 0x496c;
      iVar1 = 0x496d;
      local_44 = 0x496f;
      local_48 = 0x4970;
      iVar8 = 0x496e;
      goto LAB_000a8c12;
    }
    if (iVar1 != 0x2f) {
      iVar8 = -1;
      iVar1 = -1;
      local_48 = -1;
      local_44 = 0xffff;
      uVar9 = 0xffff;
      goto LAB_000a8c12;
    }
    uVar9 = 0x1a72;
    local_44 = 0x1a73;
  }
  iVar8 = -1;
  iVar1 = -1;
  local_48 = -1;
LAB_000a8c12:
  this_01 = operator_new(0xc0);
  puVar6 = *(undefined4 **)(DAT_000a8e78 + 0xa8c20);
  AEGeometry::AEGeometry(this_01,uVar9,(PaintCanvas *)*puVar6,false);
  this_02 = operator_new(0xc0);
  AEGeometry::AEGeometry(this_02,local_44,(PaintCanvas *)*puVar6,false);
  AEGeometry::setRotationOrder(this_02,2);
  if (iVar1 != -1) {
    pAVar3 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar3,(ushort)iVar1,(PaintCanvas *)*puVar6,false);
    AEGeometry::addChild((uint)this_01);
    pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar3);
    operator_delete(pvVar4);
  }
  if (iVar8 != -1) {
    pAVar3 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar3,(ushort)iVar8,(PaintCanvas *)*puVar6,false);
    AEGeometry::addChild((uint)this_02);
    pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar3);
    operator_delete(pvVar4);
  }
  if (local_48 != -1) {
    pAVar3 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar3,(ushort)local_48,(PaintCanvas *)*puVar6,false);
    AEGeometry::addChild((uint)this_02);
    pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar3);
    operator_delete(pvVar4);
  }
  if (*(AEGeometry **)(this + 100) != (AEGeometry *)0x0) {
    pvVar4 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 100));
    operator_delete(pvVar4);
  }
  *(undefined4 *)(this + 100) = 0;
  pAVar3 = operator_new(0xc0);
  AEGeometry::AEGeometry(pAVar3,(PaintCanvas *)*puVar6);
  *(AEGeometry **)(this + 100) = pAVar3;
  this_03 = operator_new(1);
  FileRead::FileRead(this_03);
  Status::getShip();
  iVar1 = Ship::getIndex();
  this_04 = (Array *)FileRead::loadWeaponPositions(this_03,iVar1);
  pvVar4 = (void *)FileRead::~FileRead(this_03);
  operator_delete(pvVar4);
  AEGeometry::setPosition
            (this_01,(Vector *)**(undefined4 **)(*(int *)(*(int *)(this_04 + 4) + 8) + 4));
  AEGeometry::setPosition
            (this_02,(Vector *)**(undefined4 **)(*(int *)(*(int *)(this_04 + 4) + 8) + 4));
  AEGeometry::translate(extraout_s0,extraout_s1,extraout_s2);
  iVar1 = Item::getIndex(*(Item **)piVar2[1]);
  fVar10 = extraout_s0_00;
  fVar11 = extraout_s1_00;
  fVar12 = extraout_s2_00;
  if ((iVar1 < 0xc6) ||
     (iVar1 = Item::getIndex(*(Item **)piVar2[1]), fVar10 = extraout_s0_01, fVar11 = extraout_s1_01,
     fVar12 = extraout_s2_01, 200 < iVar1)) {
    AEGeometry::rotate(fVar10,fVar11,fVar12);
    AEGeometry::rotate(extraout_s0_02,extraout_s1_02,extraout_s2_02);
  }
  pcVar7 = *(code **)(DAT_000a8e7c + 0xa8d92);
  (*pcVar7)(*(undefined4 *)(this + 100),*(undefined4 *)(this_01 + 0xc));
  (*pcVar7)(*(undefined4 *)(this + 100),*(undefined4 *)(this_02 + 0xc));
  iVar1 = Level::getEnemies();
  (*pcVar7)(*(undefined4 *)(**(int **)(iVar1 + 4) + 8),*(undefined4 *)(*(int *)(this + 100) + 0xc));
  if (this_04 == (Array *)0x0) {
    return;
  }
  iVar1 = 0;
  for (uVar5 = 0; uVar5 < *(uint *)this_04; uVar5 = uVar5 + 1) {
    if (*(Array **)(*(int *)(this_04 + 4) + iVar1) != (Array *)0x0) {
      ArrayReleaseClasses<AbyssEngine::AEMath::Vector*>(*(Array **)(*(int *)(this_04 + 4) + iVar1));
      this_05 = *(Array<AbyssEngine::AEMath::Vector*> **)(*(int *)(this_04 + 4) + iVar1);
      if (this_05 == (Array<AbyssEngine::AEMath::Vector*> *)0x0) {
        puVar6 = (undefined4 *)(*(int *)(this_04 + 4) + uVar5 * 4);
      }
      else {
        pvVar4 = (void *)Array<AbyssEngine::AEMath::Vector*>::~Array(this_05);
        operator_delete(pvVar4);
        puVar6 = (undefined4 *)(*(int *)(this_04 + 4) + iVar1);
      }
      *puVar6 = 0;
    }
    iVar1 = iVar1 + 4;
  }
  ArrayReleaseClasses<Array<AbyssEngine::AEMath::Vector*>*>(this_04);
  Array<Array<AbyssEngine::AEMath::Vector*>*>::~Array
            ((Array<Array<AbyssEngine::AEMath::Vector*>*> *)this_04);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a8a68: push {r4,r5,r6,r7,lr}
  000a8a6a: add r7,sp,#0xc
  000a8a6c: push {r7,r8,r9,r10,r11}
  000a8a70: vpush {d6,d7,d8,d9,d10}
  000a8a74: mov r4,r0
  000a8a76: ldr r0,[r0,#0x64]
  000a8a78: cbz r0,0x000a8a9a
  000a8a7a: ldr r0,[0x000a8e24]
  000a8a7c: add r0,pc
  000a8a7e: ldr r1,[r0,#0x0]
  000a8a80: ldr r0,[r4,#0x0]
  000a8a82: ldr r5,[r1,#0x0]
  000a8a84: blx 0x00071eb4
  000a8a88: ldr r0,[r0,#0x4]
  000a8a8a: ldr r1,[r4,#0x64]
  000a8a8c: ldr r0,[r0,#0x0]
  000a8a8e: ldr r2,[r1,#0xc]
  000a8a90: ldr r0,[r0,#0x8]
  000a8a92: ldr r1,[r0,#0xc]
  000a8a94: mov r0,r5
  000a8a96: blx 0x0007210c
  000a8a9a: ldr r0,[0x000a8e28]
  000a8a9c: add r0,pc
  000a8a9e: ldr r5,[r0,#0x0]
  000a8aa0: ldr r0,[r5,#0x0]
  000a8aa2: blx 0x00071a58
  000a8aa6: movs r1,#0x2
  000a8aa8: blx 0x00072118
  000a8aac: mov r10,r0
  000a8aae: cmp r0,#0x0
  000a8ab0: beq.w 0x000a8e0a
  000a8ab4: ldr.w r0,[r10,#0x0]
  000a8ab8: cmp r0,#0x0
  000a8aba: beq.w 0x000a8e0a
  000a8abe: ldr.w r0,[r10,#0x4]
  000a8ac2: ldr r0,[r0,#0x0]
  000a8ac4: cmp r0,#0x0
  000a8ac6: beq.w 0x000a8e0a
  000a8aca: blx 0x000718d8
  000a8ace: vldr.32 s16,[pc,#0x378]
  000a8ad2: cmp r0,#0xe0
  000a8ad4: str r5,[sp,#0x4]
  000a8ad6: beq 0x000a8b0a
  000a8ad8: cmp r0,#0x30
  000a8ada: beq 0x000a8b28
  000a8adc: cmp r0,#0x31
  000a8ade: beq 0x000a8b36
  000a8ae0: cmp r0,#0xb4
  000a8ae2: beq 0x000a8b44
  000a8ae4: cmp r0,#0xb5
  000a8ae6: beq 0x000a8b5c
  000a8ae8: cmp r0,#0xb6
  000a8aea: beq 0x000a8b6e
  000a8aec: cmp r0,#0xc6
  000a8aee: beq 0x000a8b94
  000a8af0: cmp r0,#0xc7
  000a8af2: beq 0x000a8bb2
  000a8af4: cmp r0,#0xc8
  000a8af6: beq 0x000a8bd0
  000a8af8: cmp r0,#0x2f
  000a8afa: bne 0x000a8bf2
  000a8afc: vldr.32 s18,[pc,#0x36c]
  000a8b00: movw r11,#0x1a72
  000a8b04: movw r0,#0x1a73
  000a8b08: b 0x000a8b50
  000a8b0a: movw r0,#0x499b
  000a8b0e: vldr.32 s18,[pc,#0x33c]
  000a8b12: str r0,[sp,#0xc]
  000a8b14: mov.w r0,#0xffffffff
  000a8b18: movw r11,#0x499a
  000a8b1c: movw r5,#0x499c
  000a8b20: movw r9,#0x499d
  000a8b24: str r0,[sp,#0x8]
  000a8b26: b 0x000a8bec
  000a8b28: movw r11,#0x1a74
  000a8b2c: vldr.32 s18,[pc,#0x340]
  000a8b30: movw r0,#0x1a75
  000a8b34: b 0x000a8b50
  000a8b36: movw r11,#0x1a76
  000a8b3a: vldr.32 s18,[pc,#0x330]
  000a8b3e: movw r0,#0x1a77
  000a8b42: b 0x000a8b50
  000a8b44: movw r11,#0x1a95
  000a8b48: vldr.32 s18,[pc,#0x31c]
  000a8b4c: movw r0,#0x1a96
  000a8b50: str r0,[sp,#0xc]
  000a8b52: mov.w r9,#0xffffffff
  000a8b56: vmov.f32 s20,s16
  000a8b5a: b 0x000a8b88
  000a8b5c: vmov.f32 s20,0xc1300000
  000a8b60: vldr.32 s18,[pc,#0x2fc]
  000a8b64: movw r11,#0x1a97
  000a8b68: movw r0,#0x1a98
  000a8b6c: b 0x000a8b82
  000a8b6e: vldr.32 s16,[pc,#0x2ec]
  000a8b72: movw r11,#0x1a99
  000a8b76: vldr.32 s18,[pc,#0x2e8]
  000a8b7a: movw r0,#0x1a9a
  000a8b7e: vldr.32 s20,[pc,#0x2e4]
  000a8b82: mov.w r9,#0xffffffff
  000a8b86: str r0,[sp,#0xc]
  000a8b88: mov.w r0,#0xffffffff
  000a8b8c: mov.w r5,#0xffffffff
  000a8b90: str r0,[sp,#0x8]
  000a8b92: b 0x000a8c12
  000a8b94: movw r0,#0x4967
  000a8b98: movw r11,#0x4963
  000a8b9c: str r0,[sp,#0xc]
  000a8b9e: mov.w r0,#0xffffffff
  000a8ba2: vldr.32 s18,[pc,#0x2b4]
  000a8ba6: movw r5,#0x4964
  000a8baa: str r0,[sp,#0x8]
  000a8bac: movw r9,#0x4966
  000a8bb0: b 0x000a8bec
  000a8bb2: movw r0,#0x496b
  000a8bb6: movw r11,#0x4968
  000a8bba: str r0,[sp,#0xc]
  000a8bbc: mov.w r0,#0xffffffff
  000a8bc0: vldr.32 s18,[pc,#0x290]
  000a8bc4: movw r5,#0x4969
  000a8bc8: str r0,[sp,#0x8]
  000a8bca: movw r9,#0x496a
  000a8bce: b 0x000a8bec
  000a8bd0: movw r0,#0x496f
  000a8bd4: movw r11,#0x496c
  000a8bd8: str r0,[sp,#0xc]
  000a8bda: movw r0,#0x4970
  000a8bde: vldr.32 s18,[pc,#0x270]
  000a8be2: movw r5,#0x496d
  000a8be6: str r0,[sp,#0x8]
  000a8be8: movw r9,#0x496e
  000a8bec: vmov.f32 s20,s16
  000a8bf0: b 0x000a8c12
  000a8bf2: movw r0,#0xffff
  000a8bf6: mov.w r9,#0xffffffff
  000a8bfa: str r0,[sp,#0xc]
  000a8bfc: mov.w r0,#0xffffffff
  000a8c00: vmov.f32 s20,s16
  000a8c04: mov.w r5,#0xffffffff
  000a8c08: vmov.f32 s18,s16
  000a8c0c: str r0,[sp,#0x8]
  000a8c0e: movw r11,#0xffff
  000a8c12: movs r0,#0xc0
  000a8c14: blx 0x0006eb24
  000a8c18: mov r8,r0
  000a8c1a: ldr r0,[0x000a8e78]
  000a8c1c: add r0,pc
  000a8c1e: ldr r6,[r0,#0x0]
  000a8c20: ldr r2,[r6,#0x0]
  000a8c22: mov r0,r8
  000a8c24: mov r1,r11
  000a8c26: movs r3,#0x0
  000a8c28: blx 0x0007207c
  000a8c2c: movs r0,#0xc0
  000a8c2e: blx 0x0006eb24
  000a8c32: mov r11,r0
  000a8c34: ldr r2,[r6,#0x0]
  000a8c36: ldr r1,[sp,#0xc]
  000a8c38: movs r3,#0x0
  000a8c3a: blx 0x0007207c
  000a8c3e: mov r0,r11
  000a8c40: movs r1,#0x2
  000a8c42: blx 0x00072100
  000a8c46: adds r0,r5,#0x1
  000a8c48: beq 0x000a8c7a
  000a8c4a: movs r0,#0xc0
  000a8c4c: str.w r9,[sp,#0xc]
  000a8c50: mov r9,r5
  000a8c52: blx 0x0006eb24
  000a8c56: mov r5,r0
  000a8c58: ldr r2,[r6,#0x0]
  000a8c5a: uxth.w r1,r9
  000a8c5e: movs r3,#0x0
  000a8c60: blx 0x0007207c
  000a8c64: ldr r1,[r5,#0xc]
  000a8c66: mov r0,r8
  000a8c68: blx 0x000720e8
  000a8c6c: mov r0,r5
  000a8c6e: blx 0x00071fc8
  000a8c72: blx 0x0006eb48
  000a8c76: ldr.w r9,[sp,#0xc]
  000a8c7a: adds.w r0,r9,#0x1
  000a8c7e: beq 0x000a8ca6
  000a8c80: movs r0,#0xc0
  000a8c82: blx 0x0006eb24
  000a8c86: mov r5,r0
  000a8c88: ldr r2,[r6,#0x0]
  000a8c8a: uxth.w r1,r9
  000a8c8e: movs r3,#0x0
  000a8c90: blx 0x0007207c
  000a8c94: ldr r1,[r5,#0xc]
  000a8c96: mov r0,r11
  000a8c98: blx 0x000720e8
  000a8c9c: mov r0,r5
  000a8c9e: blx 0x00071fc8
  000a8ca2: blx 0x0006eb48
  000a8ca6: ldr r1,[sp,#0x8]
  000a8ca8: adds r0,r1,#0x1
  000a8caa: beq 0x000a8cd8
  000a8cac: movs r0,#0xc0
  000a8cae: mov r9,r8
  000a8cb0: mov r8,r1
  000a8cb2: blx 0x0006eb24
  000a8cb6: mov r5,r0
  000a8cb8: ldr r2,[r6,#0x0]
  000a8cba: uxth.w r1,r8
  000a8cbe: movs r3,#0x0
  000a8cc0: blx 0x0007207c
  000a8cc4: ldr r1,[r5,#0xc]
  000a8cc6: mov r0,r11
  000a8cc8: blx 0x000720e8
  000a8ccc: mov r0,r5
  000a8cce: blx 0x00071fc8
  000a8cd2: blx 0x0006eb48
  000a8cd6: mov r8,r9
  000a8cd8: ldr r0,[r4,#0x64]
  000a8cda: cbz r0,0x000a8ce4
  000a8cdc: blx 0x00071fc8
  000a8ce0: blx 0x0006eb48
  000a8ce4: movs r0,#0x0
  000a8ce6: str r0,[r4,#0x64]
  000a8ce8: movs r0,#0xc0
  000a8cea: blx 0x0006eb24
  000a8cee: ldr r1,[r6,#0x0]
  000a8cf0: mov r5,r0
  000a8cf2: blx 0x000720f4
  000a8cf6: movs r0,#0x1
  000a8cf8: str r5,[r4,#0x64]
  000a8cfa: blx 0x0006eb24
  000a8cfe: mov r6,r0
  000a8d00: blx 0x00072124
  000a8d04: ldr r0,[sp,#0x4]
  000a8d06: ldr r0,[r0,#0x0]
  000a8d08: blx 0x00071a58
  000a8d0c: blx 0x000719c8
  000a8d10: mov r1,r0
  000a8d12: mov r0,r6
  000a8d14: blx 0x00072130
  000a8d18: mov r5,r0
  000a8d1a: mov r0,r6
  000a8d1c: blx 0x0007213c
  000a8d20: blx 0x0006eb48
  000a8d24: ldr r0,[r5,#0x4]
  000a8d26: ldr r0,[r0,#0x8]
  000a8d28: ldr r0,[r0,#0x4]
  000a8d2a: ldr r1,[r0,#0x0]
  000a8d2c: mov r0,r8
  000a8d2e: blx 0x00072148
  000a8d32: ldr r0,[r5,#0x4]
  000a8d34: ldr r0,[r0,#0x8]
  000a8d36: ldr r0,[r0,#0x4]
  000a8d38: ldr r1,[r0,#0x0]
  000a8d3a: mov r0,r11
  000a8d3c: blx 0x00072148
  000a8d40: vmov r1,s16
  000a8d44: mov r0,r11
  000a8d46: vmov r2,s18
  000a8d4a: vmov r3,s20
  000a8d4e: blx 0x00072154
  000a8d52: ldr.w r0,[r10,#0x4]
  000a8d56: ldr r0,[r0,#0x0]
  000a8d58: blx 0x000718d8
  000a8d5c: cmp r0,#0xc6
  000a8d5e: blt 0x000a8d6e
  000a8d60: ldr.w r0,[r10,#0x4]
  000a8d64: ldr r0,[r0,#0x0]
  000a8d66: blx 0x000718d8
  000a8d6a: cmp r0,#0xc9
  000a8d6c: blt 0x000a8d88
  000a8d6e: ldr r6,[0x000a8e74]
  000a8d70: mov r0,r8
  000a8d72: movs r1,#0x0
  000a8d74: movs r3,#0x0
  000a8d76: mov r2,r6
  000a8d78: blx 0x00072160
  000a8d7c: mov r0,r11
  000a8d7e: movs r1,#0x0
  000a8d80: mov r2,r6
  000a8d82: movs r3,#0x0
  000a8d84: blx 0x00072160
  000a8d88: ldr r2,[0x000a8e7c]
  000a8d8a: ldr.w r1,[r8,#0xc]
  000a8d8e: add r2,pc
  000a8d90: ldr r0,[r4,#0x64]
  000a8d92: ldr r6,[r2,#0x0]
  000a8d94: blx r6
  000a8d96: ldr.w r1,[r11,#0xc]
  000a8d9a: ldr r0,[r4,#0x64]
  000a8d9c: blx r6
  000a8d9e: ldr r0,[r4,#0x0]
  000a8da0: blx 0x00071eb4
  000a8da4: ldr r0,[r0,#0x4]
  000a8da6: ldr r1,[r4,#0x64]
  000a8da8: ldr r0,[r0,#0x0]
  000a8daa: ldr r1,[r1,#0xc]
  000a8dac: ldr r0,[r0,#0x8]
  000a8dae: blx r6
  000a8db0: cbz r5,0x000a8e0a
  000a8db2: mov.w r8,#0x0
  000a8db6: movs r6,#0x0
  000a8db8: movs r4,#0x0
  000a8dba: b 0x000a8de6
  000a8dbc: ldr r0,[r5,#0x4]
  000a8dbe: ldr r0,[r0,r6]
  000a8dc0: cbz r0,0x000a8de2
  000a8dc2: blx 0x0007216c
  000a8dc6: ldr r1,[r5,#0x4]
  000a8dc8: ldr r0,[r1,r6]
  000a8dca: cbz r0,0x000a8dda
  000a8dcc: blx 0x00072178
  000a8dd0: blx 0x0006eb48
  000a8dd4: ldr r0,[r5,#0x4]
  000a8dd6: add r0,r6
  000a8dd8: b 0x000a8dde
  000a8dda: add.w r0,r1,r4, lsl #0x2
  000a8dde: str.w r8,[r0,#0x0]
  000a8de2: adds r6,#0x4
  000a8de4: adds r4,#0x1
  000a8de6: ldr r0,[r5,#0x0]
  000a8de8: cmp r4,r0
  000a8dea: bcc 0x000a8dbc
  000a8dec: mov r0,r5
  000a8dee: blx 0x00072184
  000a8df2: mov r0,r5
  000a8df4: blx 0x00072190
  000a8df8: vpop {d6,d7,d8,d9,d10}
  000a8dfc: add sp,#0x4
  000a8dfe: pop.w {r8,r9,r10,r11}
  000a8e02: pop.w {r4,r5,r6,r7,lr}
  000a8e06: b.w 0x001ab098
  000a8e0a: vpop {d6,d7,d8,d9,d10}
  000a8e0e: add sp,#0x4
  000a8e10: pop.w {r8,r9,r10,r11}
  000a8e14: pop {r4,r5,r6,r7,pc}
```
