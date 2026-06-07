# Globals::getShipGroup
elf 0xe4e14 body 1710
Sig: undefined __thiscall getShipGroup(Globals * this, int param_1, int param_2, bool param_3)

## decompile
```c

/* Globals::getShipGroup(int, int, bool) */

void __thiscall Globals::getShipGroup(Globals *this,int param_1,int param_2,bool param_3)

{
  int iVar1;
  short sVar2;
  AEGeometry *pAVar3;
  ushort *puVar4;
  uint *puVar5;
  int *piVar6;
  ushort *puVar7;
  int iVar8;
  ushort uVar9;
  undefined4 extraout_r1;
  char cVar10;
  uint *puVar11;
  undefined4 uVar12;
  code *pcVar13;
  uint uVar14;
  uint uVar15;
  uint *puVar16;
  int iVar17;
  uint uVar18;
  code *pcVar19;
  undefined4 *puVar20;
  code *pcVar21;
  int *piVar22;
  uint uVar23;
  int iVar24;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  undefined4 local_8c;
  uint local_88 [15];
  int local_4c;
  uint local_48;
  uint local_44;
  uint local_40;
  int local_3c;
  
  piVar22 = *(int **)(DAT_000f51dc + 0xf4e2c);
  local_3c = *piVar22;
  if (param_1 == 0xf) {
    if (param_2 == 0) {
      pAVar3 = operator_new(0xc0);
      puVar20 = *(undefined4 **)(DAT_000f5530 + 0xf532c);
      AEGeometry::AEGeometry(pAVar3,0x42a9,(PaintCanvas *)*puVar20,false);
      local_48 = 0xffffffff;
      pcVar19 = *(code **)(DAT_000f5534 + 0xf5346);
      (*pcVar19)(*puVar20,&local_48);
      pcVar21 = *(code **)(DAT_000f5538 + 0xf535a);
      (*pcVar21)(*puVar20,local_48,0x42ae,0);
      pcVar13 = *(code **)(DAT_000f553c + 0xf536c);
      (*pcVar13)(pAVar3,local_48);
      local_40 = 0xffffffff;
      (*pcVar19)(*puVar20,&local_40);
      (*pcVar21)(*puVar20,local_40,0x42b2,0);
      (*pcVar13)(pAVar3,local_40);
      local_44 = 0xffffffff;
      (*pcVar19)(*puVar20,&local_44);
      (*pcVar21)(*puVar20,local_44,0x42ad,0);
      (*pcVar13)(pAVar3,local_44);
      local_4c = DAT_000f551c + 0x1a001a;
      local_88[1] = 45000;
      local_88[0] = 25000;
      AEGeometry::setLodMeshes(pAVar3,(ushort *)&local_4c,(int *)local_88,2);
      local_8c = DAT_000f5520;
      AEGeometry::setLodChildMeshes(pAVar3,(ushort *)&local_8c);
    }
    else if (param_2 == 3) {
      pAVar3 = operator_new(0xc0);
      puVar11 = *(uint **)(DAT_000f51ec + 0xf4ef0);
      AEGeometry::AEGeometry(pAVar3,0x4299,(PaintCanvas *)*puVar11,false);
      local_48 = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_48);
      AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_48,true);
      AEGeometry::addChild((uint)pAVar3);
      iVar8 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f51f0 + 0xf4f32));
      uVar18 = 0xffffffff;
      uVar14 = 0xffffffff;
      for (iVar24 = 0; iVar24 < iVar8; iVar24 = iVar24 + 1) {
        local_40 = 0xffffffff;
        local_44 = 0xffffffff;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_40);
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_44);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_40,true);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_44,true);
        uVar15 = local_40;
        uVar23 = local_44;
        if (uVar18 != 0xffffffff) {
          AbyssEngine::PaintCanvas::TransformAddChild((PaintCanvas *)*puVar11,uVar18,local_40);
          AbyssEngine::PaintCanvas::TransformAddChild((PaintCanvas *)*puVar11,uVar14,local_44);
          uVar15 = uVar18;
          uVar23 = uVar14;
        }
        AbyssEngine::PaintCanvas::TransformGetLocal(*puVar11);
        AbyssEngine::AEMath::MatrixSetTranslation
                  ((Matrix *)local_88,extraout_s0,extraout_s1,extraout_s2);
        AbyssEngine::PaintCanvas::TransformGetLocal(*puVar11);
        AbyssEngine::AEMath::MatrixSetTranslation
                  ((Matrix *)local_88,extraout_s0_00,extraout_s1_00,extraout_s2_00);
        uVar18 = uVar15;
        uVar14 = uVar23;
      }
      if (uVar18 != 0xffffffff) {
        iVar8 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar11);
        *(undefined4 *)(iVar8 + 0xe0) = DAT_000f51f8;
        AEGeometry::addChild((uint)pAVar3);
      }
      local_88[0] = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,local_88);
      AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_88[0],true);
      AEGeometry::addChild((uint)pAVar3);
      local_44 = CONCAT22(local_44._2_2_,0x429a);
      local_40 = 35000;
      AEGeometry::setLodMeshes(pAVar3,(ushort *)&local_44,(int *)&local_40,1);
      AEGeometry::setLodChildTransform(pAVar3,uVar14);
    }
    else {
      pAVar3 = operator_new(0xc0);
      puVar11 = *(uint **)(DAT_000f5540 + 0xf53e6);
      AEGeometry::AEGeometry(pAVar3,0x42a4,(PaintCanvas *)*puVar11,false);
      local_48 = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_48);
      AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_48,true);
      AEGeometry::addChild((uint)pAVar3);
      local_40 = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_40);
      AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_40,true);
      AEGeometry::addChild((uint)pAVar3);
      local_44 = DAT_000f551c + 0x160016;
      local_88[1] = 60000;
      local_88[0] = 35000;
      AEGeometry::setLodMeshes(pAVar3,(ushort *)&local_44,(int *)local_88,2);
    }
    goto LAB_000f54da;
  }
  if (param_1 == 0xe) {
    pAVar3 = operator_new(0xc0);
    puVar11 = *(uint **)(DAT_000f51fc + 0xf5068);
    AEGeometry::AEGeometry(pAVar3,0x37e7,(PaintCanvas *)*puVar11,false);
    local_48 = 0xffffffff;
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_48);
    AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_48,true);
    AEGeometry::addChild((uint)pAVar3);
    local_40 = 0xffffffff;
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_40);
    AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_40,true);
    AEGeometry::addChild((uint)pAVar3);
    local_44 = DAT_000f5200;
    local_88[1] = 60000;
    local_88[0] = 35000;
    AEGeometry::setLodMeshes(pAVar3,(ushort *)&local_44,(int *)local_88,2);
    local_4c = DAT_000f5204;
    AEGeometry::setLodChildMeshes(pAVar3,(ushort *)&local_4c);
    AEGeometry::setScaling(extraout_s0_01,extraout_s1_01,extraout_s2_01);
    goto LAB_000f54da;
  }
  if (param_1 == 0xd) {
    pAVar3 = operator_new(0xc0);
    puVar11 = *(uint **)(DAT_000f51e0 + 0xf4e50);
    AEGeometry::AEGeometry(pAVar3,0x4275,(PaintCanvas *)*puVar11,false);
    local_48 = 0xffffffff;
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_48);
    AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_48,true);
    AEGeometry::addChild((uint)pAVar3);
    local_40 = 0xffffffff;
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_40);
    AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_40,true);
    AEGeometry::addChild((uint)pAVar3);
    local_44 = DAT_000f51e4;
    local_88[1] = 45000;
    local_88[0] = 35000;
    AEGeometry::setLodMeshes(pAVar3,(ushort *)&local_44,(int *)local_88,2);
    local_4c = DAT_000f51e8;
    AEGeometry::setLodChildMeshes(pAVar3,(ushort *)&local_4c);
    goto LAB_000f54da;
  }
  pAVar3 = operator_new(0xc0);
  puVar11 = *(uint **)(DAT_000f5208 + 0xf5108);
  AEGeometry::AEGeometry
            (pAVar3,*(ushort *)(DAT_000f520c + 0xf510a + param_1 * 2),(PaintCanvas *)*puVar11,true);
  uVar9 = *(ushort *)(DAT_000f5210 + 0xf5124 + param_1 * 2);
  local_88[0] = 0xffffffff;
  local_48 = 0xffffffff;
  if (uVar9 != 0xffff) {
    AbyssEngine::PaintCanvas::MeshCreate((PaintCanvas *)*puVar11,uVar9,&local_48,true);
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,local_88);
    AbyssEngine::PaintCanvas::TransformAddMeshId((PaintCanvas *)*puVar11,local_88[0],local_48);
    AEGeometry::addChild((uint)pAVar3);
    *(uint *)(pAVar3 + 0x20) = local_48;
  }
  if (!param_3) {
    uVar9 = (short)param_1 + 0x7dc8;
    local_40 = 0xffffffff;
    AbyssEngine::PaintCanvas::MaterialCreate((PaintCanvas *)*puVar11,uVar9,&local_40);
    AbyssEngine::PaintCanvas::MeshChangeResourceMaterial
              ((PaintCanvas *)*puVar11,*(uint *)(pAVar3 + 0x1c),uVar9);
  }
  sVar2 = *(short *)(DAT_000f5544 + 0xf5190 + param_1 * 2);
  if (sVar2 != -1) {
    local_40 = 0xffffffff;
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_40);
    AbyssEngine::PaintCanvas::TransformAddMesh(*puVar11,(ushort)local_40,SUB21(sVar2,0));
    AEGeometry::addChild((uint)pAVar3);
  }
  if (param_3) {
    if (param_1 != 0x27 && param_1 != 0x29) {
      local_40 = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_40);
      cVar10 = -0x14;
      goto LAB_000f5228;
    }
  }
  else {
    local_40 = 0xffffffff;
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar11,&local_40);
    cVar10 = 'P';
LAB_000f5228:
    AbyssEngine::PaintCanvas::TransformAddMesh
              (*puVar11,(ushort)local_40,(bool)(cVar10 + (char)param_1));
    AEGeometry::addChild((uint)pAVar3);
  }
  uVar18 = 0;
  iVar8 = 0;
  puVar20 = (undefined4 *)(DAT_000f5548 + 0xf524e + param_1 * 0xc);
  while (iVar8 != 2) {
    piVar6 = puVar20 + iVar8;
    iVar8 = iVar8 + 1;
    if (*piVar6 != 0xffff) {
      uVar18 = uVar18 + 1;
    }
  }
  uVar12 = 2;
  if (uVar18 != 0) {
    pcVar13 = *(code **)(DAT_000f554c + 0xf527c);
    puVar4 = (ushort *)(*pcVar13)(uVar18 << 1);
    uVar12 = (undefined4)((ulonglong)uVar18 * 4);
    if ((int)((ulonglong)uVar18 * 4 >> 0x20) != 0) {
      uVar12 = 0xffffffff;
    }
    puVar5 = (uint *)(*pcVar13)(uVar12);
    piVar6 = (int *)(*pcVar13)(uVar12);
    iVar8 = 0;
    iVar24 = 5000;
    puVar16 = puVar5;
    for (uVar14 = uVar18; uVar14 != 0; uVar14 = uVar14 - 1) {
      uVar12 = *puVar20;
      piVar6[iVar8] = iVar24;
      puVar4[iVar8] = (ushort)uVar12;
      if (!param_3) {
        AbyssEngine::PaintCanvas::MeshCreate((PaintCanvas *)*puVar11,(ushort)uVar12,puVar16,true);
        AbyssEngine::PaintCanvas::MeshChangeResourceMaterial
                  ((PaintCanvas *)*puVar11,*puVar16,(short)param_1 + 0x7dc8);
      }
      iVar24 = iVar24 + 8000;
      puVar16 = puVar16 + 1;
      puVar20 = puVar20 + 1;
      iVar8 = iVar8 + 1;
    }
    if (param_3) {
      AEGeometry::setLodMeshes(pAVar3,puVar4,piVar6,uVar18);
    }
    else {
      AEGeometry::setLodMeshesWithMeshIds(pAVar3,puVar4,puVar5,piVar6,uVar18);
    }
    iVar17 = 0;
    iVar24 = DAT_000f5550 + 0xf547c + param_1 * 0xc;
    iVar8 = 0;
    while (iVar8 != 2) {
      iVar1 = iVar8 * 4;
      iVar8 = iVar8 + 1;
      if (*(int *)(iVar24 + iVar1) != 0xffff) {
        iVar17 = iVar17 + 1;
      }
    }
    if (iVar17 != 0) {
      puVar7 = operator_new__(iVar17 << 1);
      for (iVar8 = 0; iVar17 != iVar8; iVar8 = iVar8 + 1) {
        puVar7[iVar8] = (ushort)*(undefined4 *)(iVar24 + iVar8 * 4);
      }
      AEGeometry::setLodChildMeshes(pAVar3,puVar7);
      operator_delete__(puVar7);
    }
    operator_delete__(puVar4);
    operator_delete__(piVar6);
    uVar12 = extraout_r1;
  }
  AEGeometry::setLodLastVisibleDistance(CONCAT44(uVar12,pAVar3));
LAB_000f54da:
  iVar8 = *piVar22 - local_3c;
  if (iVar8 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar8);
}

```

## target disasm
```
  000f4e14: push {r4,r5,r6,r7,lr}
  000f4e16: add r7,sp,#0xc
  000f4e18: push {r7,r8,r9,r10,r11}
  000f4e1c: vpush {d8,d9}
  000f4e20: sub sp,#0x80
  000f4e22: ldr r0,[0x000f51dc]
  000f4e24: mov r6,r1
  000f4e26: cmp r1,#0xf
  000f4e28: add r0,pc
  000f4e2a: ldr.w r11,[r0,#0x0]
  000f4e2e: ldr.w r0,[r11,#0x0]
  000f4e32: str r0,[sp,#0x7c]
  000f4e34: beq 0x000f4ed2
  000f4e36: cmp r6,#0xe
  000f4e38: beq.w 0x000f505a
  000f4e3c: cmp r6,#0xd
  000f4e3e: bne.w 0x000f50f6
  000f4e42: movs r0,#0xc0
  000f4e44: blx 0x0006eb24
  000f4e48: mov r9,r0
  000f4e4a: ldr r0,[0x000f51e0]
  000f4e4c: add r0,pc
  000f4e4e: ldr r4,[r0,#0x0]
  000f4e50: ldr r2,[r4,#0x0]
  000f4e52: mov r0,r9
  000f4e54: movw r1,#0x4275
  000f4e58: movs r3,#0x0
  000f4e5a: blx 0x0007207c
  000f4e5e: ldr r0,[r4,#0x0]
  000f4e60: add r1,sp,#0x70
  000f4e62: mov.w r5,#0xffffffff
  000f4e66: str r5,[sp,#0x70]
  000f4e68: blx 0x000720ac
  000f4e6c: ldr r0,[r4,#0x0]
  000f4e6e: movw r2,#0x4919
  000f4e72: ldr r1,[sp,#0x70]
  000f4e74: movs r3,#0x0
  000f4e76: blx 0x0007303c
  000f4e7a: ldr r1,[sp,#0x70]
  000f4e7c: mov r0,r9
  000f4e7e: blx 0x000720e8
  000f4e82: ldr r0,[r4,#0x0]
  000f4e84: add r1,sp,#0x78
  000f4e86: str r5,[sp,#0x78]
  000f4e88: blx 0x000720ac
  000f4e8c: ldr r0,[r4,#0x0]
  000f4e8e: movw r2,#0x428e
  000f4e92: ldr r1,[sp,#0x78]
  000f4e94: movs r3,#0x0
  000f4e96: blx 0x0007303c
  000f4e9a: ldr r1,[sp,#0x78]
  000f4e9c: mov r0,r9
  000f4e9e: blx 0x000720e8
  000f4ea2: ldr r0,[0x000f51e4]
  000f4ea4: movw r1,#0xea60
  000f4ea8: str r0,[sp,#0x74]
  000f4eaa: movw r0,#0x3a98
  000f4eae: subs r0,r1,r0
  000f4eb0: str r0,[sp,#0x34]
  000f4eb2: movw r0,#0x88b8
  000f4eb6: add r1,sp,#0x74
  000f4eb8: add r2,sp,#0x30
  000f4eba: str r0,[sp,#0x30]
  000f4ebc: mov r0,r9
  000f4ebe: movs r3,#0x2
  000f4ec0: blx 0x00073d5c
  000f4ec4: ldr r0,[0x000f51e8]
  000f4ec6: add r1,sp,#0x6c
  000f4ec8: str r0,[sp,#0x6c]
  000f4eca: mov r0,r9
  000f4ecc: blx 0x00073d68
  000f4ed0: b 0x000f54da
  000f4ed2: cmp r2,#0x0
  000f4ed4: beq.w 0x000f531e
  000f4ed8: cmp r2,#0x3
  000f4eda: bne.w 0x000f53d8
  000f4ede: movs r0,#0xc0
  000f4ee0: str.w r11,[sp,#0x18]
  000f4ee4: blx 0x0006eb24
  000f4ee8: mov r5,r0
  000f4eea: ldr r0,[0x000f51ec]
  000f4eec: add r0,pc
  000f4eee: ldr r4,[r0,#0x0]
  000f4ef0: ldr r2,[r4,#0x0]
  000f4ef2: mov r0,r5
  000f4ef4: movw r1,#0x4299
  000f4ef8: movs r3,#0x0
  000f4efa: blx 0x0007207c
  000f4efe: ldr r0,[r4,#0x0]
  000f4f00: add r1,sp,#0x70
  000f4f02: mov.w r9,#0xffffffff
  000f4f06: str.w r9,[sp,#0x70]
  000f4f0a: blx 0x000720ac
  000f4f0e: ldr r0,[r4,#0x0]
  000f4f10: movw r2,#0x429f
  000f4f14: ldr r1,[sp,#0x70]
  000f4f16: movs r3,#0x0
  000f4f18: mov.w r10,#0x0
  000f4f1c: blx 0x0007303c
  000f4f20: ldr r1,[sp,#0x70]
  000f4f22: mov r0,r5
  000f4f24: str r5,[sp,#0x14]
  000f4f26: blx 0x000720e8
  000f4f2a: ldr r0,[0x000f51f0]
  000f4f2c: movs r1,#0x4
  000f4f2e: add r0,pc
  000f4f30: ldr r0,[r0,#0x0]
  000f4f32: ldr r0,[r0,#0x0]
  000f4f34: blx 0x00071848
  000f4f38: vldr.32 s16,[pc,#0x2b8]
  000f4f3c: add r5,sp,#0x30
  000f4f3e: mov r8,r0
  000f4f40: mov.w r11,#0xffffffff
  000f4f44: mov.w r6,#0xffffffff
  000f4f48: b 0x000f4fec
  000f4f4a: ldr r0,[r4,#0x0]
  000f4f4c: add r1,sp,#0x78
  000f4f4e: str.w r9,[sp,#0x78]
  000f4f52: str.w r9,[sp,#0x74]
  000f4f56: blx 0x000720ac
  000f4f5a: ldr r0,[r4,#0x0]
  000f4f5c: add r1,sp,#0x74
  000f4f5e: blx 0x000720ac
  000f4f62: ldr r0,[r4,#0x0]
  000f4f64: movw r2,#0x429c
  000f4f68: ldr r1,[sp,#0x78]
  000f4f6a: movs r3,#0x0
  000f4f6c: blx 0x0007303c
  000f4f70: ldr r0,[r4,#0x0]
  000f4f72: movw r2,#0x429d
  000f4f76: ldr r1,[sp,#0x74]
  000f4f78: movs r3,#0x0
  000f4f7a: blx 0x0007303c
  000f4f7e: adds r0,r6,#0x1
  000f4f80: beq 0x000f4f9a
  000f4f82: ldr r2,[sp,#0x78]
  000f4f84: mov r1,r6
  000f4f86: ldr r0,[r4,#0x0]
  000f4f88: blx 0x000720d0
  000f4f8c: ldr r0,[r4,#0x0]
  000f4f8e: mov r1,r11
  000f4f90: ldr r2,[sp,#0x74]
  000f4f92: blx 0x000720d0
  000f4f96: ldr r1,[sp,#0x78]
  000f4f98: b 0x000f4fa0
  000f4f9a: ldrd r11,r1,[sp,#0x74]
  000f4f9e: mov r6,r1
  000f4fa0: adr.w r0,0xf5524
  000f4fa4: cmp.w r10,#0x1
  000f4fa8: it eq
  000f4faa: add.eq r0,#0x4
  000f4fac: cmp.w r10,#0x0
  000f4fb0: vldr.32 s18,[r0]
  000f4fb4: it eq
  000f4fb6: vmov.eq.f32 s18,s16
  000f4fba: ldr r0,[r4,#0x0]
  000f4fbc: blx 0x000720c4
  000f4fc0: mov r1,r0
  000f4fc2: mov r0,r5
  000f4fc4: movs r2,#0x0
  000f4fc6: movs r3,#0x0
  000f4fc8: vstr.32 s18,[sp]
  000f4fcc: blx 0x0006f820
  000f4fd0: ldr r0,[r4,#0x0]
  000f4fd2: ldr r1,[sp,#0x74]
  000f4fd4: blx 0x000720c4
  000f4fd8: mov r1,r0
  000f4fda: mov r0,r5
  000f4fdc: movs r2,#0x0
  000f4fde: movs r3,#0x0
  000f4fe0: vstr.32 s18,[sp]
  000f4fe4: blx 0x0006f820
  000f4fe8: add.w r10,r10,#0x1
  000f4fec: cmp r10,r8
  000f4fee: blt 0x000f4f4a
  000f4ff0: ldr.w r9,[sp,#0x14]
  000f4ff4: adds r0,r6,#0x1
  000f4ff6: beq 0x000f500e
  000f4ff8: ldr r0,[r4,#0x0]
  000f4ffa: mov r1,r6
  000f4ffc: blx 0x00072088
  000f5000: ldr r1,[0x000f51f8]
  000f5002: str.w r1,[r0,#0xe0]
  000f5006: mov r0,r9
  000f5008: mov r1,r6
  000f500a: blx 0x000720e8
  000f500e: mov.w r1,#0xffffffff
  000f5012: ldr r0,[r4,#0x0]
  000f5014: str r1,[sp,#0x30]
  000f5016: add r1,sp,#0x30
  000f5018: blx 0x000720ac
  000f501c: ldr r0,[r4,#0x0]
  000f501e: movw r2,#0x429e
  000f5022: ldr r1,[sp,#0x30]
  000f5024: movs r3,#0x0
  000f5026: blx 0x0007303c
  000f502a: ldr r1,[sp,#0x30]
  000f502c: mov r0,r9
  000f502e: blx 0x000720e8
  000f5032: movw r0,#0x429a
  000f5036: add r1,sp,#0x74
  000f5038: strh.w r0,[sp,#0x74]
  000f503c: movw r0,#0x88b8
  000f5040: add r2,sp,#0x78
  000f5042: str r0,[sp,#0x78]
  000f5044: mov r0,r9
  000f5046: movs r3,#0x1
  000f5048: blx 0x00073d5c
  000f504c: mov r0,r9
  000f504e: mov r1,r11
  000f5050: blx 0x0007435c
  000f5054: ldr.w r11,[sp,#0x18]
  000f5058: b 0x000f54da
  000f505a: movs r0,#0xc0
  000f505c: blx 0x0006eb24
  000f5060: mov r9,r0
  000f5062: ldr r0,[0x000f51fc]
  000f5064: add r0,pc
  000f5066: ldr r4,[r0,#0x0]
  000f5068: ldr r2,[r4,#0x0]
  000f506a: mov r0,r9
  000f506c: movw r1,#0x37e7
  000f5070: movs r3,#0x0
  000f5072: blx 0x0007207c
  000f5076: ldr r0,[r4,#0x0]
  000f5078: add r1,sp,#0x70
  000f507a: mov.w r5,#0xffffffff
  000f507e: str r5,[sp,#0x70]
  000f5080: blx 0x000720ac
  000f5084: ldr r0,[r4,#0x0]
  000f5086: movw r2,#0x37ec
  000f508a: ldr r1,[sp,#0x70]
  000f508c: movs r3,#0x0
  000f508e: blx 0x0007303c
  000f5092: ldr r1,[sp,#0x70]
  000f5094: mov r0,r9
  000f5096: blx 0x000720e8
  000f509a: ldr r0,[r4,#0x0]
  000f509c: add r1,sp,#0x78
  000f509e: str r5,[sp,#0x78]
  000f50a0: blx 0x000720ac
  000f50a4: ldr r0,[r4,#0x0]
  000f50a6: movw r2,#0x37eb
  000f50aa: ldr r1,[sp,#0x78]
  000f50ac: movs r3,#0x0
  000f50ae: blx 0x0007303c
  000f50b2: ldr r1,[sp,#0x78]
  000f50b4: mov r0,r9
  000f50b6: blx 0x000720e8
  000f50ba: ldr r0,[0x000f5200]
  000f50bc: add r1,sp,#0x74
  000f50be: str r0,[sp,#0x74]
  000f50c0: movw r0,#0xea60
  000f50c4: str r0,[sp,#0x34]
  000f50c6: movw r0,#0x88b8
  000f50ca: add r2,sp,#0x30
  000f50cc: str r0,[sp,#0x30]
  000f50ce: mov r0,r9
  000f50d0: movs r3,#0x2
  000f50d2: blx 0x00073d5c
  000f50d6: ldr r0,[0x000f5204]
  000f50d8: add r1,sp,#0x6c
  000f50da: str r0,[sp,#0x6c]
  000f50dc: mov r0,r9
  000f50de: blx 0x00073d68
  000f50e2: mov r0,r9
  000f50e4: mov.w r1,#0x40000000
  000f50e8: mov.w r2,#0x40000000
  000f50ec: mov.w r3,#0x40000000
  000f50f0: blx 0x000727b4
  000f50f4: b 0x000f54da
  000f50f6: movs r0,#0xc0
  000f50f8: str r3,[sp,#0x24]
  000f50fa: blx 0x0006eb24
  000f50fe: mov r9,r0
  000f5100: ldr r0,[0x000f5208]
  000f5102: ldr r1,[0x000f520c]
  000f5104: add r0,pc
  000f5106: add r1,pc
  000f5108: ldr r0,[r0,#0x0]
  000f510a: ldrh.w r1,[r1,r6,lsl #0x1]
  000f510e: str r0,[sp,#0x20]
  000f5110: ldr r2,[r0,#0x0]
  000f5112: mov r0,r9
  000f5114: movs r3,#0x1
  000f5116: blx 0x0007207c
  000f511a: ldr r0,[0x000f5210]
  000f511c: movw r8,#0xffff
  000f5120: add r0,pc
  000f5122: ldrh.w r1,[r0,r6,lsl #0x1]
  000f5126: mov.w r0,#0xffffffff
  000f512a: str r0,[sp,#0x30]
  000f512c: str r0,[sp,#0x70]
  000f512e: cmp r1,r8
  000f5130: beq 0x000f515e
  000f5132: ldr r4,[sp,#0x20]
  000f5134: add r2,sp,#0x70
  000f5136: movs r3,#0x1
  000f5138: ldr r0,[r4,#0x0]
  000f513a: blx 0x0006fcf4
  000f513e: ldr r0,[r4,#0x0]
  000f5140: add r1,sp,#0x30
  000f5142: blx 0x000720ac
  000f5146: ldr r0,[r4,#0x0]
  000f5148: ldr r2,[sp,#0x70]
  000f514a: ldr r1,[sp,#0x30]
  000f514c: blx 0x00073030
  000f5150: ldr r1,[sp,#0x30]
  000f5152: mov r0,r9
  000f5154: blx 0x000720e8
  000f5158: ldr r0,[sp,#0x70]
  000f515a: str.w r0,[r9,#0x20]
  000f515e: ldr r1,[sp,#0x24]
  000f5160: cbnz r1,0x000f518a
  000f5162: ldr r5,[sp,#0x20]
  000f5164: movw r1,#0x7dc8
  000f5168: mov.w r0,#0xffffffff
  000f516c: add r1,r6
  000f516e: str r0,[sp,#0x78]
  000f5170: add r2,sp,#0x78
  000f5172: ldr r0,[r5,#0x0]
  000f5174: uxth r4,r1
  000f5176: mov r1,r4
  000f5178: blx 0x0006fc1c
  000f517c: ldr.w r1,[r9,#0x1c]
  000f5180: mov r2,r4
  000f5182: ldr r0,[r5,#0x0]
  000f5184: blx 0x00072448
  000f5188: ldr r1,[sp,#0x24]
  000f518a: ldr r0,[0x000f5544]
  000f518c: add r0,pc
  000f518e: ldrh.w r4,[r0,r6,lsl #0x1]
  000f5192: cmp r4,r8
  000f5194: beq 0x000f51bc
  000f5196: ldr r5,[sp,#0x20]
  000f5198: mov.w r1,#0xffffffff
  000f519c: str r1,[sp,#0x78]
  000f519e: add r1,sp,#0x78
  000f51a0: ldr r0,[r5,#0x0]
  000f51a2: blx 0x000720ac
  000f51a6: ldr r0,[r5,#0x0]
  000f51a8: mov r2,r4
  000f51aa: ldr r1,[sp,#0x78]
  000f51ac: movs r3,#0x0
  000f51ae: blx 0x0007303c
  000f51b2: ldr r1,[sp,#0x78]
  000f51b4: mov r0,r9
  000f51b6: blx 0x000720e8
  000f51ba: ldr r1,[sp,#0x24]
  000f51bc: cbz r1,0x000f5214
  000f51be: cmp r6,#0x27
  000f51c0: it ne
  000f51c2: cmp.ne r6,#0x29
  000f51c4: beq 0x000f523e
  000f51c6: ldr r4,[sp,#0x20]
  000f51c8: mov.w r1,#0xffffffff
  000f51cc: str r1,[sp,#0x78]
  000f51ce: add r1,sp,#0x78
  000f51d0: ldr r0,[r4,#0x0]
  000f51d2: blx 0x000720ac
  000f51d6: movw r2,#0x45ec
  000f51da: b 0x000f5228
  000f5214: ldr r4,[sp,#0x20]
  000f5216: mov.w r1,#0xffffffff
  000f521a: str r1,[sp,#0x78]
  000f521c: add r1,sp,#0x78
  000f521e: ldr r0,[r4,#0x0]
  000f5220: blx 0x000720ac
  000f5224: movw r2,#0x4650
  000f5228: add r2,r6
  000f522a: ldr r0,[r4,#0x0]
  000f522c: ldr r1,[sp,#0x78]
  000f522e: movs r3,#0x0
  000f5230: uxth r2,r2
  000f5232: blx 0x0007303c
  000f5236: ldr r1,[sp,#0x78]
  000f5238: mov r0,r9
  000f523a: blx 0x000720e8
  000f523e: ldr r0,[0x000f5548]
  000f5240: add.w r3,r6,r6, lsl #0x1
  000f5244: mov.w r8,#0x0
  000f5248: movs r1,#0x0
  000f524a: add r0,pc
  000f524c: add.w r10,r0,r3, lsl #0x2
  000f5250: movw r0,#0xffff
  000f5254: b 0x000f5264
  000f5256: ldr.w r2,[r10,r1,lsl #0x2]
  000f525a: adds r1,#0x1
  000f525c: cmp r2,r0
  000f525e: it ne
  000f5260: add.ne.w r8,r8,#0x1
  000f5264: cmp r1,#0x2
  000f5266: bne 0x000f5256
  000f5268: cmp.w r8,#0x0
  000f526c: beq.w 0x000f54d0
  000f5270: str r3,[sp,#0xc]
  000f5272: strd r9,r11,[sp,#0x14]
  000f5276: ldr r0,[0x000f554c]
  000f5278: add r0,pc
  000f527a: ldr r5,[r0,#0x0]
  000f527c: lsl.w r0,r8,#0x1
  000f5280: blx r5
  000f5282: str r0,[sp,#0x28]
  000f5284: movs r0,#0x4
  000f5286: umull r4,r0,r8,r0
  000f528a: cmp r0,#0x0
  000f528c: it ne
  000f528e: mov.ne r0,#0x1
  000f5290: cmp r0,#0x0
  000f5292: it ne
  000f5294: mov.ne.w r4,#0xffffffff
  000f5298: mov r0,r4
  000f529a: blx r5
  000f529c: str r0,[sp,#0x10]
  000f529e: mov r0,r4
  000f52a0: blx r5
  000f52a2: mov r4,r0
  000f52a4: movw r0,#0x7dc8
  000f52a8: add r0,r6
  000f52aa: ldr r6,[sp,#0x10]
  000f52ac: mov.w r9,#0x0
  000f52b0: movw r11,#0x1388
  000f52b4: uxth r0,r0
  000f52b6: mov r5,r8
  000f52b8: str r0,[sp,#0x1c]
  000f52ba: ldr r0,[sp,#0x24]
  000f52bc: str.w r8,[sp,#0x8]
  000f52c0: b 0x000f5302
  000f52c2: ldr.w r1,[r10,#0x0]
  000f52c6: cmp r0,#0x0
  000f52c8: ldr r2,[sp,#0x28]
  000f52ca: str.w r11,[r4,r9,lsl #0x2]
  000f52ce: strh.w r1,[r2,r9,lsl #0x1]
  000f52d2: bne 0x000f52f2
  000f52d4: mov r8,r4
  000f52d6: ldr r4,[sp,#0x20]
  000f52d8: uxth r1,r1
  000f52da: mov r2,r6
  000f52dc: movs r3,#0x1
  000f52de: ldr r0,[r4,#0x0]
  000f52e0: blx 0x0006fcf4
  000f52e4: ldr r0,[r4,#0x0]
  000f52e6: mov r4,r8
  000f52e8: ldr r1,[r6,#0x0]
  000f52ea: ldr r2,[sp,#0x1c]
  000f52ec: blx 0x00072448
  000f52f0: ldr r0,[sp,#0x24]
  000f52f2: add.w r11,r11,#0x1f40
  000f52f6: adds r6,#0x4
  000f52f8: add.w r10,r10,#0x4
  000f52fc: subs r5,#0x1
  000f52fe: add.w r9,r9,#0x1
  000f5302: cmp r5,#0x0
  000f5304: bne 0x000f52c2
  000f5306: cmp r0,#0x0
  000f5308: beq.w 0x000f545a
  000f530c: ldr.w r9,[sp,#0x14]
  000f5310: mov r2,r4
  000f5312: ldr r1,[sp,#0x28]
  000f5314: ldr r3,[sp,#0x8]
  000f5316: mov r0,r9
  000f5318: blx 0x00073d5c
  000f531c: b 0x000f546e
  000f531e: movs r0,#0xc0
  000f5320: blx 0x0006eb24
  000f5324: mov r6,r0
  000f5326: ldr r0,[0x000f5530]
  000f5328: add r0,pc
  000f532a: ldr r4,[r0,#0x0]
  000f532c: ldr r2,[r4,#0x0]
  000f532e: mov r0,r6
  000f5330: movw r1,#0x42a9
  000f5334: movs r3,#0x0
  000f5336: blx 0x0007207c
  000f533a: ldr r1,[0x000f5534]
  000f533c: mov.w r8,#0xffffffff
  000f5340: ldr r0,[r4,#0x0]
  000f5342: add r1,pc
  000f5344: str.w r8,[sp,#0x70]
  000f5348: ldr.w r9,[r1,#0x0]
  000f534c: add r1,sp,#0x70
  000f534e: blx r9
  000f5350: ldr r2,[0x000f5538]
  000f5352: movs r3,#0x0
  000f5354: ldr r0,[r4,#0x0]
  000f5356: add r2,pc
  000f5358: ldr r1,[sp,#0x70]
  000f535a: ldr.w r10,[r2,#0x0]
  000f535e: movw r2,#0x42ae
  000f5362: blx r10
  000f5364: ldr r0,[0x000f553c]
  000f5366: ldr r1,[sp,#0x70]
  000f5368: add r0,pc
  000f536a: ldr r5,[r0,#0x0]
  000f536c: mov r0,r6
  000f536e: blx r5
  000f5370: ldr r0,[r4,#0x0]
  000f5372: add r1,sp,#0x78
  000f5374: str.w r8,[sp,#0x78]
  000f5378: blx r9
  000f537a: ldr r0,[r4,#0x0]
  000f537c: movw r2,#0x42b2
  000f5380: ldr r1,[sp,#0x78]
  000f5382: movs r3,#0x0
  000f5384: blx r10
  000f5386: ldr r1,[sp,#0x78]
  000f5388: mov r0,r6
  000f538a: blx r5
  000f538c: ldr r0,[r4,#0x0]
  000f538e: add r1,sp,#0x74
  000f5390: str.w r8,[sp,#0x74]
  000f5394: blx r9
  000f5396: ldr r0,[r4,#0x0]
  000f5398: movw r2,#0x42ad
  000f539c: ldr r1,[sp,#0x74]
  000f539e: movs r3,#0x0
  000f53a0: blx r10
  000f53a2: ldr r1,[sp,#0x74]
  000f53a4: mov r0,r6
  000f53a6: blx r5
  000f53a8: ldr r0,[0x000f551c]
  000f53aa: add r1,sp,#0x6c
  000f53ac: add r2,sp,#0x30
  000f53ae: movs r3,#0x2
  000f53b0: add.w r0,r0,#0x1a001a
  000f53b4: str r0,[sp,#0x6c]
  000f53b6: movw r0,#0xafc8
  000f53ba: str r0,[sp,#0x34]
  000f53bc: movw r0,#0x61a8
  000f53c0: str r0,[sp,#0x30]
  000f53c2: mov r0,r6
  000f53c4: blx 0x00073d5c
  000f53c8: ldr r0,[0x000f5520]
  000f53ca: add r1,sp,#0x2c
  000f53cc: str r0,[sp,#0x2c]
  000f53ce: mov r0,r6
  000f53d0: blx 0x00073d68
  000f53d4: mov r9,r6
  000f53d6: b 0x000f54da
  000f53d8: movs r0,#0xc0
  000f53da: blx 0x0006eb24
  000f53de: mov r9,r0
  000f53e0: ldr r0,[0x000f5540]
  000f53e2: add r0,pc
  000f53e4: ldr r4,[r0,#0x0]
  000f53e6: ldr r2,[r4,#0x0]
  000f53e8: mov r0,r9
  000f53ea: movw r1,#0x42a4
  000f53ee: movs r3,#0x0
  000f53f0: blx 0x0007207c
  000f53f4: ldr r0,[r4,#0x0]
  000f53f6: add r1,sp,#0x70
  000f53f8: mov.w r5,#0xffffffff
  000f53fc: str r5,[sp,#0x70]
  000f53fe: blx 0x000720ac
  000f5402: ldr r0,[r4,#0x0]
  000f5404: movw r2,#0x42a5
  000f5408: ldr r1,[sp,#0x70]
  000f540a: movs r3,#0x0
  000f540c: blx 0x0007303c
  000f5410: ldr r1,[sp,#0x70]
  000f5412: mov r0,r9
  000f5414: blx 0x000720e8
  000f5418: ldr r0,[r4,#0x0]
  000f541a: add r1,sp,#0x78
  000f541c: str r5,[sp,#0x78]
  000f541e: blx 0x000720ac
  000f5422: ldr r0,[r4,#0x0]
  000f5424: movw r2,#0x42a8
  000f5428: ldr r1,[sp,#0x78]
  000f542a: movs r3,#0x0
  000f542c: blx 0x0007303c
  000f5430: ldr r1,[sp,#0x78]
  000f5432: mov r0,r9
  000f5434: blx 0x000720e8
  000f5438: ldr r0,[0x000f551c]
  000f543a: add r1,sp,#0x74
  000f543c: add r2,sp,#0x30
  000f543e: movs r3,#0x2
  000f5440: add.w r0,r0,#0x160016
  000f5444: str r0,[sp,#0x74]
  000f5446: movw r0,#0xea60
  000f544a: str r0,[sp,#0x34]
  000f544c: movw r0,#0x88b8
  000f5450: str r0,[sp,#0x30]
  000f5452: mov r0,r9
  000f5454: blx 0x00073d5c
  000f5458: b 0x000f54da
  000f545a: ldr.w r9,[sp,#0x14]
  000f545e: mov r3,r4
  000f5460: ldr r1,[sp,#0x28]
  000f5462: ldr r2,[sp,#0x10]
  000f5464: ldr r0,[sp,#0x8]
  000f5466: str r0,[sp,#0x0]
  000f5468: mov r0,r9
  000f546a: blx 0x00075df0
  000f546e: ldr.w r11,[sp,#0x18]
  000f5472: movs r6,#0x0
  000f5474: ldr r1,[sp,#0xc]
  000f5476: ldr r0,[0x000f5550]
  000f5478: add r0,pc
  000f547a: add.w r5,r0,r1, lsl #0x2
  000f547e: movw r0,#0xffff
  000f5482: movs r1,#0x0
  000f5484: b 0x000f5492
  000f5486: ldr.w r2,[r5,r1,lsl #0x2]
  000f548a: adds r1,#0x1
  000f548c: cmp r2,r0
  000f548e: it ne
  000f5490: add.ne r6,#0x1
  000f5492: cmp r1,#0x2
  000f5494: bne 0x000f5486
  000f5496: cbz r6,0x000f54c4
  000f5498: lsls r0,r6,#0x1
  000f549a: mov r8,r4
  000f549c: blx 0x0006ec08
  000f54a0: mov r4,r0
  000f54a2: movs r0,#0x0
  000f54a4: b 0x000f54b0
  000f54a6: ldr.w r1,[r5,r0,lsl #0x2]
  000f54aa: strh.w r1,[r4,r0,lsl #0x1]
  000f54ae: adds r0,#0x1
  000f54b0: cmp r6,r0
  000f54b2: bne 0x000f54a6
  000f54b4: mov r0,r9
  000f54b6: mov r1,r4
  000f54b8: blx 0x00073d68
  000f54bc: mov r0,r4
  000f54be: blx 0x0006ebfc
  000f54c2: mov r4,r8
  000f54c4: ldr r0,[sp,#0x28]
  000f54c6: blx 0x0006ebfc
  000f54ca: mov r0,r4
  000f54cc: blx 0x0006ebfc
  000f54d0: ldr r2,[0x000f552c]
  000f54d2: mov r0,r9
  000f54d4: movs r3,#0x0
  000f54d6: blx 0x00075dfc
  000f54da: ldr r0,[sp,#0x7c]
  000f54dc: ldr.w r1,[r11,#0x0]
  000f54e0: subs r0,r1,r0
  000f54e2: itttt eq
  000f54e4: mov.eq r0,r9
  000f54e6: add.eq sp,#0x80
  000f54e8: vpop.eq {d8,d9}
  000f54ec: add.eq sp,#0x4
  000f54ee: itt eq
  000f54f0: pop.eq.w {r8,r9,r10,r11}
  000f54f4: pop.eq {r4,r5,r6,r7,pc}
  000f54f6: blx 0x0006e824
```
