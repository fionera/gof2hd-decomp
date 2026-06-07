# StarMap::initStarSystem
elf 0xc7580 body 1912
Sig: undefined __thiscall initStarSystem(StarMap * this)

## decompile
```c

/* StarMap::initStarSystem() */

void __thiscall StarMap::initStarSystem(StarMap *this)

{
  undefined8 uVar1;
  undefined8 uVar2;
  Matrix *pMVar3;
  short sVar4;
  uint *puVar5;
  Array<Station*> *this_00;
  FileRead *this_01;
  undefined4 uVar6;
  void *pvVar7;
  Array<AEGeometry*> *pAVar8;
  AEGeometry *pAVar9;
  Array<bool> *this_02;
  int *piVar10;
  undefined4 *puVar11;
  uint *puVar12;
  undefined4 *puVar13;
  int iVar14;
  uint uVar15;
  Array<AbyssEngine::AEMath::Vector*> *this_03;
  Engine *pEVar16;
  ushort uVar17;
  undefined4 extraout_r1;
  int iVar18;
  code *pcVar19;
  bool bVar20;
  uint uVar21;
  int iVar22;
  PaintCanvas *this_04;
  int *piVar23;
  Status *this_05;
  uint uVar24;
  int *piVar25;
  uint in_fpscr;
  float fVar26;
  float fVar27;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
  float extraout_s0_06;
  float fVar28;
  float extraout_s0_07;
  float extraout_s0_08;
  float extraout_s0_09;
  float extraout_s0_10;
  float extraout_s0_11;
  float extraout_s0_12;
  float extraout_s0_13;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s1_07;
  float extraout_s1_08;
  float extraout_s1_09;
  float extraout_s1_10;
  float extraout_s1_11;
  float fVar29;
  float extraout_s1_12;
  float extraout_s1_13;
  float extraout_s1_14;
  float extraout_s1_15;
  float extraout_s1_16;
  float extraout_s1_17;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  float extraout_s2_03;
  float extraout_s2_04;
  float extraout_s2_05;
  float extraout_s2_06;
  float extraout_s2_07;
  float extraout_s2_08;
  float extraout_s2_09;
  float extraout_s2_10;
  float extraout_s2_11;
  float extraout_s2_12;
  float extraout_s2_13;
  float extraout_s2_14;
  float extraout_s2_15;
  float extraout_s2_16;
  float extraout_s2_17;
  float fVar30;
  undefined4 uVar31;
  undefined4 uVar32;
  undefined4 uVar33;
  Matrix *local_f8 [3];
  Matrix aMStack_ec [12];
  undefined4 local_e0;
  undefined4 uStack_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 uStack_d0;
  undefined4 local_cc;
  uint local_98 [5];
  undefined4 local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined4 local_60;
  int local_5c;
  
  piVar23 = *(int **)(DAT_000d7950 + 0xd7596);
  local_5c = *piVar23;
  puVar5 = (uint *)SolarSystem::getStations
                             (*(SolarSystem **)
                               (*(int *)(*(int *)(this + 0x54) + 4) + *(int *)(this + 0x60) * 4));
  uVar24 = *puVar5;
  this_00 = operator_new(0xc);
  Array<Station*>::Array(this_00);
  *(Array<Station*> **)(this + 0x58) = this_00;
  ArraySetLength<Station*>(uVar24,(Array *)this_00);
  this_01 = operator_new(1);
  FileRead::FileRead(this_01);
  uVar6 = FileRead::loadStationsBinary
                    (this_01,*(SolarSystem **)
                              (*(int *)(*(int *)(this + 0x54) + 4) + *(int *)(this + 0x60) * 4));
  *(undefined4 *)(this + 0x58) = uVar6;
  pvVar7 = (void *)FileRead::~FileRead(this_01);
  operator_delete(pvVar7);
  uVar21 = (uint)((ulonglong)uVar24 * 4);
  if ((int)((ulonglong)uVar24 * 4 >> 0x20) != 0) {
    uVar21 = 0xffffffff;
  }
  pvVar7 = operator_new__(uVar21);
  *(void **)(this + 0x98) = pvVar7;
  pvVar7 = operator_new__(uVar21);
  piVar25 = *(int **)(DAT_000d7954 + 0xd7616);
  *(void **)(this + 0x9c) = pvVar7;
  *(undefined4 *)(this + 0x1c4) = 0xffffffff;
  iVar22 = *piVar25;
  SolarSystem::getIndex();
  AbyssEngine::AERandom::setSeed(CONCAT44(1000,iVar22));
  pAVar8 = operator_new(0xc);
  Array<AEGeometry*>::Array(pAVar8);
  *(Array<AEGeometry*> **)(this + 0x90) = pAVar8;
  ArraySetLength<AEGeometry*>(uVar24 + 1,(Array *)pAVar8);
  pAVar9 = operator_new(0xc0);
  puVar5 = *(uint **)(DAT_000d7958 + 0xd766c);
  AEGeometry::AEGeometry(pAVar9,(PaintCanvas *)*puVar5);
  *(AEGeometry **)(this + 0xa4) = pAVar9;
  this_02 = operator_new(0xc);
  Array<bool>::Array(this_02);
  *(Array<bool> **)(this + 0x100) = this_02;
  ArraySetLength<bool>(**(uint **)(this + 0x90),(Array *)this_02);
  piVar10 = *(int **)(this + 0x100);
  iVar22 = *piVar10;
  for (iVar18 = 0; uVar2 = DAT_000d7d50, uVar1 = DAT_000d7d48, fVar29 = DAT_000d7960,
      fVar28 = DAT_000d795c, iVar22 != iVar18; iVar18 = iVar18 + 1) {
    *(undefined1 *)(piVar10[1] + iVar18) = 0;
  }
  puVar11 = (undefined4 *)((uint)local_98 | 4);
  uVar6 = 0;
  uVar31 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar32 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar33 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  uVar21 = 0;
  do {
    puVar12 = *(uint **)(this + 0x90);
    if (*puVar12 <= uVar21) {
      AEGeometry::setVisible(SUB41(*(undefined4 *)(puVar12[1] + 4),0));
      pAVar8 = operator_new(0xc);
      Array<AEGeometry*>::Array(pAVar8);
      *(Array<AEGeometry*> **)(this + 0x94) = pAVar8;
      ArraySetLength<AEGeometry*>(uVar24,(Array *)pAVar8);
      fVar29 = DAT_000d7d44;
      for (uVar21 = 0; uVar21 < **(uint **)(this + 0x94); uVar21 = uVar21 + 1) {
        pAVar9 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar9,0x1a7b,(PaintCanvas *)*puVar5,false);
        *(AEGeometry **)(*(int *)(*(int *)(this + 0x94) + 4) + uVar21 * 4) = pAVar9;
        AEGeometry::addChild(*(uint *)(this + 0xa4));
        pAVar9 = *(AEGeometry **)(*(int *)(*(int *)(this + 0x94) + 4) + uVar21 * 4);
        fVar30 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 0x9c) + uVar21 * 4),
                                            (byte)(in_fpscr >> 0x16) & 3);
        uVar6 = AbyssEngine::AERandom::nextInt(*piVar25);
        fVar27 = (float)VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
        AEGeometry::setRotation(pAVar9,fVar27 / fVar29,extraout_s1_02,extraout_s2_02);
        fVar27 = (float)VectorSignedToFloat((int)fVar30 << 1,(byte)(in_fpscr >> 0x16) & 3);
        AEGeometry::setScaling(fVar27 * fVar28,extraout_s1_03,extraout_s2_03);
      }
      pAVar9 = *(AEGeometry **)(this + 0xa4);
      AEGeometry::getPosition();
      AEGeometry::setPosition(pAVar9,(Vector *)local_98);
      AEGeometry::setScaling(extraout_s0,extraout_s1_04,extraout_s2_04);
      AEGeometry::setRotation
                (*(AEGeometry **)(this + 0xa4),extraout_s0_00,extraout_s1_05,extraout_s2_05);
      uVar6 = DAT_000d7d5c;
      *(undefined4 *)(this + 0x188) = 0x45800000;
      *(undefined4 *)(this + 0x18c) = uVar6;
      AEGeometry::setRotation
                (*(AEGeometry **)(this + 0xa4),extraout_s0_01,extraout_s1_06,extraout_s2_06);
      *(undefined4 *)(this + 100) = 0xffffffff;
      this_04 = (PaintCanvas *)*puVar5;
      iVar22 = SolarSystem::getRace();
      AbyssEngine::PaintCanvas::Image2DCreate
                (this_04,*(ushort *)(DAT_000d7d84 + 0xd7aca + iVar22 * 4),(uint *)(this + 0x34));
      if (*(Array **)(this + 0x198) != (Array *)0x0) {
        ArrayReleaseClasses<AbyssEngine::AEMath::Vector*>(*(Array **)(this + 0x198));
        if (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x198) !=
            (Array<AbyssEngine::AEMath::Vector*> *)0x0) {
          pvVar7 = (void *)Array<AbyssEngine::AEMath::Vector*>::~Array
                                     (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x198));
          operator_delete(pvVar7);
        }
      }
      *(undefined4 *)(this + 0x198) = 0;
      this_03 = operator_new(0xc);
      Array<AbyssEngine::AEMath::Vector*>::Array(this_03);
      *(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x198) = this_03;
      ArraySetLength<AbyssEngine::AEMath::Vector*>(**(uint **)(this + 0x58),(Array *)this_03);
      for (uVar21 = 0; puVar12 = *(uint **)(this + 0x198), uVar21 < *puVar12; uVar21 = uVar21 + 1) {
        puVar11 = operator_new(0xc);
        *puVar11 = 0;
        puVar11[1] = 0;
        puVar11[2] = 0;
        *(undefined4 **)(puVar12[1] + uVar21 * 4) = puVar11;
      }
      puVar11 = *(undefined4 **)(DAT_000d7d88 + 0xd7b36);
      uVar21 = AbyssEngine::ApplicationManager::GetEngine();
      AbyssEngine::Engine::LightSetLightDirection
                (extraout_s0_02,extraout_s1_07,extraout_s2_07,uVar21);
      if (*(AEGeometry **)(this + 0x1bc) != (AEGeometry *)0x0) {
        pvVar7 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x1bc));
        operator_delete(pvVar7);
      }
      *(undefined4 *)(this + 0x178) = 0xffffffff;
      *(undefined4 *)(this + 0x1bc) = 0;
      iVar22 = SolarSystem::getTextureIndex();
      uVar17 = *(ushort *)(DAT_000d7d8c + 0xd7b84 + iVar22 * 4);
      if (*(int *)(this + 0x60) == 0x1b) {
        uVar17 = 0x2734;
      }
      AbyssEngine::PaintCanvas::TextureCreate
                ((ushort)*puVar5,(uint *)(uint)uVar17,(bool)((char)this + 'x'));
      pAVar9 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar9,0x1a70,(PaintCanvas *)*puVar5,false);
      *(AEGeometry **)(this + 0x1bc) = pAVar9;
      AEGeometry::getPosition();
      AEGeometry::setPosition(pAVar9,(Vector *)local_98);
      AEGeometry::setRotation
                (*(AEGeometry **)(this + 0x1bc),extraout_s0_03,extraout_s1_08,extraout_s2_08);
      iVar22 = SolarSystem::getTextureIndex();
      if (iVar22 == 0xf) {
        iVar22 = Status::getCurrentCampaignMission();
        fVar28 = extraout_s0_05;
        fVar29 = extraout_s1_10;
        fVar27 = extraout_s2_10;
        if (0x69 < iVar22) {
          Status::getCurrentCampaignMission();
          fVar28 = extraout_s0_06;
          fVar29 = extraout_s1_11;
          fVar27 = extraout_s2_11;
        }
        AEGeometry::setScaling(fVar28,fVar29,fVar27);
      }
      else {
        AEGeometry::setScaling(extraout_s0_04,extraout_s1_09,extraout_s2_09);
      }
      AEGeometry::setVisible
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(this + 0x68) + 4) + *(int *)(this + 0x60) * 4),0));
      AEGeometry::getPosition();
      pcVar19 = *(code **)(DAT_000d7d94 + 0xd7c5a);
      pEVar16 = (Engine *)(*pcVar19)(*puVar11);
      AbyssEngine::Engine::LightSetLightPosition
                (pEVar16,extraout_s0_07,extraout_s1_12,extraout_s2_12,local_98[0]);
      uVar21 = (*pcVar19)(*puVar11);
      AbyssEngine::Engine::LightSetLightColorAmbient
                (extraout_s0_08,extraout_s1_13,extraout_s2_13,uVar21);
      uVar21 = (*pcVar19)(*puVar11);
      AbyssEngine::Engine::LightSetLightColorDiffuse
                (extraout_s0_09,extraout_s1_14,extraout_s2_14,uVar21);
      (*pcVar19)(*puVar11);
      AbyssEngine::Engine::LightSetMaterialColorAmbient
                (extraout_s0_10,extraout_s1_15,extraout_s2_15);
      (*pcVar19)(*puVar11);
      AbyssEngine::Engine::LightSetMaterialColorDiffuse
                (extraout_s0_11,extraout_s1_16,extraout_s2_16);
      (*pcVar19)(*puVar11);
      AbyssEngine::Engine::LightSetMaterialColorSpecular
                (extraout_s0_12,extraout_s1_17,extraout_s2_17);
      pEVar16 = (Engine *)(*pcVar19)(*puVar11);
      AbyssEngine::Engine::LightSetMaterialColorShininess(pEVar16,extraout_s0_13);
      iVar22 = (*pcVar19)(*puVar11);
      *(undefined4 *)(iVar22 + 0x32c) = 1;
      iVar22 = *piVar25;
      Status::getPlayingTime();
      AbyssEngine::AERandom::setSeed(CONCAT44(extraout_r1,iVar22));
      if (*piVar23 != local_5c) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return;
    }
    if (uVar21 != 0) {
      pAVar9 = operator_new(0xc0);
      iVar18 = uVar21 - 1;
      sVar4 = Station::getTextureIndex
                        (*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + iVar18 * 4));
      AEGeometry::AEGeometry(pAVar9,sVar4 + 0x4704,(PaintCanvas *)*puVar5,false);
      *(AEGeometry **)(*(int *)(*(int *)(this + 0x90) + 4) + uVar21 * 4) = pAVar9;
      local_98[0] = 0x3f800000;
      *puVar11 = uVar6;
      puVar11[1] = uVar31;
      puVar11[2] = uVar32;
      puVar11[3] = uVar33;
      local_84 = 0x3f800000;
      uStack_70 = uVar1;
      uStack_68 = uVar2;
      iVar22 = 0;
      local_60 = 0x3f800000;
      bVar20 = false;
      fVar27 = extraout_s1;
      fVar30 = extraout_s2;
      local_80 = uVar6;
      uStack_7c = uVar31;
      uStack_78 = uVar32;
      uStack_74 = uVar33;
      while (!bVar20) {
        do {
          iVar22 = AbyssEngine::AERandom::nextInt(*piVar25);
          puVar13 = *(undefined4 **)(this + 0x100);
          iVar14 = puVar13[1];
        } while (*(char *)(iVar14 + iVar22) != '\0');
        *(undefined1 *)(iVar14 + iVar22) = 1;
        iVar14 = __aeabi_uidiv(0x10000,*puVar13);
        iVar22 = iVar22 * iVar14;
        fVar27 = extraout_s1_00;
        fVar30 = extraout_s2_00;
        bVar20 = true;
      }
      fVar26 = (float)VectorSignedToFloat(iVar22,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(*(int *)(this + 0x98) + iVar18 * 4) = iVar22;
      AbyssEngine::AEMath::MatrixSetRotation
                ((Matrix *)&local_d4,fVar26 * fVar28 * fVar29,fVar27,fVar30);
      if (uVar21 == 1) {
        iVar22 = 0x1900;
      }
      else {
        iVar22 = *(int *)(*(int *)(this + 0x9c) + uVar21 * 4 + -8);
      }
      iVar14 = AbyssEngine::AERandom::nextInt(*piVar25);
      iVar22 = iVar14 + iVar22 + 0x640;
      local_cc = VectorSignedToFloat(iVar22,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(*(int *)(this + 0x9c) + iVar18 * 4) = iVar22;
      local_d4 = 0;
      uStack_d0 = 0;
      local_e0 = 0;
      uStack_dc = 0;
      local_d8 = 0;
      AbyssEngine::AEMath::MatrixTransformVector(aMStack_ec,(Vector *)local_98);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e0,(Vector *)aMStack_ec);
      AEGeometry::translate(*(Vector **)(*(int *)(*(int *)(this + 0x90) + 4) + uVar21 * 4));
      uVar15 = Station::getTextureIndex
                         (*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + iVar18 * 4));
      fVar27 = (float)VectorSignedToFloat(*(int *)(DAT_000d7964 + 0xd782a + uVar15 * 4) << 4,
                                          (byte)(in_fpscr >> 0x16) & 3);
      AEGeometry::setScaling(fVar27 * fVar28,extraout_s1_01,extraout_s2_01);
      AEGeometry::getPosition();
      AbyssEngine::AEMath::VectorNormalize((AEMath *)local_f8,(Vector *)aMStack_ec);
      AbyssEngine::AEMath::Vector::operator=((Vector *)aMStack_ec,(Vector *)local_f8);
      AEGeometry::addChild(*(uint *)(this + 0xa4));
      if ((uVar15 < 0x16) && ((1 << (uVar15 & 0xff) & DAT_000d7968) != 0)) {
LAB_000d7898:
        local_f8[0] = (Matrix *)0xffffffff;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar5,(uint *)local_f8);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar5,(ushort)local_f8[0],true);
        pMVar3 = local_f8[0];
        uVar15 = *puVar5;
        AEGeometry::getMatrix();
        AbyssEngine::PaintCanvas::TransformSetLocal(uVar15,pMVar3);
        AEGeometry::addChild(*(uint *)(this + 0xa4));
      }
      else {
        this_05 = (Status *)**(undefined4 **)(DAT_000d7970 + 0xd7934);
        iVar22 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + iVar18 * 4));
        iVar22 = Status::orbitHasPlanetRing(this_05,iVar22);
        if (iVar22 != 0) goto LAB_000d7898;
      }
      if (((*(int *)(this + 0xf8) != 0) &&
          (piVar10 = *(int **)(DAT_000d796c + 0xd78ea),
          *(int *)(this + 0x60) == *(int *)(*piVar10 + 0x7c))) &&
         (iVar22 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + iVar18 * 4)
                                    ), iVar22 == *(int *)(*piVar10 + 0x80))) {
        *(uint *)(this + 0x1c4) = uVar21;
      }
      puVar12 = *(uint **)(this + 0x90);
    }
    if (*(int *)(puVar12[1] + uVar21 * 4) != 0) {
      AEGeometry::addChild(*(uint *)(this + 0xa4));
    }
    uVar21 = uVar21 + 1;
  } while( true );
}

```

## target disasm
```
  000d7580: push {r4,r5,r6,r7,lr}
  000d7582: add r7,sp,#0xc
  000d7584: push {r7,r8,r9,r10,r11}
  000d7588: vpush {d8,d9,d10,d11,d12,d13}
  000d758c: sub sp,#0xc0
  000d758e: mov r4,r0
  000d7590: ldr r0,[0x000d7950]
  000d7592: add r0,pc
  000d7594: ldr r6,[r0,#0x0]
  000d7596: ldr r0,[r6,#0x0]
  000d7598: str r0,[sp,#0xbc]
  000d759a: ldr r0,[r4,#0x54]
  000d759c: ldr r1,[r4,#0x60]
  000d759e: ldr r0,[r0,#0x4]
  000d75a0: ldr.w r0,[r0,r1,lsl #0x2]
  000d75a4: blx 0x00071a88
  000d75a8: ldr.w r8,[r0,#0x0]
  000d75ac: movs r0,#0xc
  000d75ae: blx 0x0006eb24
  000d75b2: mov r5,r0
  000d75b4: blx 0x00073594
  000d75b8: mov r0,r8
  000d75ba: mov r1,r5
  000d75bc: str r5,[r4,#0x58]
  000d75be: blx 0x000735b8
  000d75c2: movs r0,#0x1
  000d75c4: blx 0x0006eb24
  000d75c8: mov r5,r0
  000d75ca: blx 0x00072124
  000d75ce: ldr r0,[r4,#0x54]
  000d75d0: ldr r1,[r4,#0x60]
  000d75d2: ldr r0,[r0,#0x4]
  000d75d4: ldr.w r1,[r0,r1,lsl #0x2]
  000d75d8: mov r0,r5
  000d75da: blx 0x00073690
  000d75de: str r0,[r4,#0x58]
  000d75e0: mov r0,r5
  000d75e2: blx 0x0007213c
  000d75e6: blx 0x0006eb48
  000d75ea: movs r0,#0x4
  000d75ec: umull r5,r0,r8,r0
  000d75f0: cmp r0,#0x0
  000d75f2: it ne
  000d75f4: mov.ne r0,#0x1
  000d75f6: cmp r0,#0x0
  000d75f8: it ne
  000d75fa: mov.ne.w r5,#0xffffffff
  000d75fe: mov r0,r5
  000d7600: blx 0x0006ec08
  000d7604: str.w r0,[r4,#0x98]
  000d7608: mov r0,r5
  000d760a: blx 0x0006ec08
  000d760e: ldr r1,[0x000d7954]
  000d7610: ldr r2,[r4,#0x60]
  000d7612: add r1,pc
  000d7614: ldr.w r11,[r1,#0x0]
  000d7618: ldr r1,[r4,#0x54]
  000d761a: str.w r0,[r4,#0x9c]
  000d761e: mov.w r0,#0xffffffff
  000d7622: str.w r0,[r4,#0x1c4]
  000d7626: ldr r0,[r1,#0x4]
  000d7628: ldr.w r5,[r11,#0x0]
  000d762c: ldr.w r0,[r0,r2,lsl #0x2]
  000d7630: blx 0x00071a7c
  000d7634: mov.w r1,#0x3e8
  000d7638: mul r2,r0,r1
  000d763c: mov r0,r5
  000d763e: asrs r3,r2,#0x1f
  000d7640: blx 0x000739c0
  000d7644: movs r0,#0xc
  000d7646: blx 0x0006eb24
  000d764a: mov r5,r0
  000d764c: blx 0x000717b8
  000d7650: add.w r0,r8,#0x1
  000d7654: mov r1,r5
  000d7656: str.w r5,[r4,#0x90]
  000d765a: blx 0x00073504
  000d765e: movs r0,#0xc0
  000d7660: blx 0x0006eb24
  000d7664: mov r5,r0
  000d7666: ldr r0,[0x000d7958]
  000d7668: add r0,pc
  000d766a: ldr r0,[r0,#0x0]
  000d766c: str r0,[sp,#0x1c]
  000d766e: ldr r1,[r0,#0x0]
  000d7670: mov r0,r5
  000d7672: blx 0x000720f4
  000d7676: movs r0,#0xc
  000d7678: str r6,[sp,#0x8]
  000d767a: str.w r5,[r4,#0xa4]
  000d767e: blx 0x0006eb24
  000d7682: mov r5,r0
  000d7684: blx 0x000735a0
  000d7688: ldr.w r0,[r4,#0x90]
  000d768c: mov r1,r5
  000d768e: str.w r5,[r4,#0x100]
  000d7692: ldr r0,[r0,#0x0]
  000d7694: blx 0x000735c4
  000d7698: ldr.w r0,[r4,#0x100]
  000d769c: movs r2,#0x0
  000d769e: movs r3,#0x0
  000d76a0: ldr r1,[r0,#0x0]
  000d76a2: b 0x000d76aa
  000d76a4: ldr r6,[r0,#0x4]
  000d76a6: strb r2,[r6,r3]
  000d76a8: adds r3,#0x1
  000d76aa: cmp r1,r3
  000d76ac: bne 0x000d76a4
  000d76ae: str.w r8,[sp,#0xc]
  000d76b2: add.w r8,sp,#0x80
  000d76b6: add.w r0,r8,#0x28
  000d76ba: str r0,[sp,#0x18]
  000d76bc: add.w r0,r8,#0x18
  000d76c0: str r0,[sp,#0x14]
  000d76c2: orr r0,r8,#0x4
  000d76c6: str r0,[sp,#0x10]
  000d76c8: adr.w r0,0xd7d48
  000d76cc: vmov.i32 q6,#0x0
  000d76d0: vld1.64 {d10,d11},[r0]
  000d76d4: mov.w r10,#0x0
  000d76d8: vldr.32 s16,[pc,#0x280]
  000d76dc: vldr.32 s18,[pc,#0x280]
  000d76e0: ldr.w r0,[r4,#0x90]
  000d76e4: ldr r1,[r0,#0x0]
  000d76e6: cmp r10,r1
  000d76e8: bcs.w 0x000d7974
  000d76ec: cmp.w r10,#0x0
  000d76f0: beq.w 0x000d7914
  000d76f4: movs r0,#0xc0
  000d76f6: blx 0x0006eb24
  000d76fa: mov r6,r0
  000d76fc: ldr r0,[r4,#0x58]
  000d76fe: sub.w r9,r10,#0x1
  000d7702: ldr r0,[r0,#0x4]
  000d7704: ldr.w r0,[r0,r9,lsl #0x2]
  000d7708: blx 0x000736b4
  000d770c: ldr r1,[sp,#0x1c]
  000d770e: ldr r2,[r1,#0x0]
  000d7710: movw r1,#0x4704
  000d7714: add r0,r1
  000d7716: uxth r1,r0
  000d7718: mov r0,r6
  000d771a: movs r3,#0x0
  000d771c: blx 0x0007207c
  000d7720: ldr.w r0,[r4,#0x90]
  000d7724: mov.w r1,#0x3f800000
  000d7728: movs r5,#0x0
  000d772a: ldr r0,[r0,#0x4]
  000d772c: str.w r6,[r0,r10,lsl #0x2]
  000d7730: ldr r0,[sp,#0x10]
  000d7732: str r1,[sp,#0x80]
  000d7734: vst1.32 {d12,d13},[r0]
  000d7738: ldr r0,[sp,#0x14]
  000d773a: str r1,[sp,#0x94]
  000d773c: vst1.64 {d12,d13},[r0]
  000d7740: ldr r0,[sp,#0x18]
  000d7742: vst1.32 {d10,d11},[r0]
  000d7746: movs r0,#0x0
  000d7748: str r1,[sp,#0xb8]
  000d774a: b 0x000d7778
  000d774c: ldr.w r0,[r4,#0x100]
  000d7750: ldr r1,[r0,#0x0]
  000d7752: ldr.w r0,[r11,#0x0]
  000d7756: blx 0x00071848
  000d775a: mov r6,r0
  000d775c: ldr.w r0,[r4,#0x100]
  000d7760: ldr r1,[r0,#0x4]
  000d7762: ldrb r2,[r1,r6]
  000d7764: cmp r2,#0x0
  000d7766: bne 0x000d774c
  000d7768: movs r5,#0x1
  000d776a: strb r5,[r1,r6]
  000d776c: ldr r1,[r0,#0x0]
  000d776e: mov.w r0,#0x10000
  000d7772: blx 0x0006ec2c
  000d7776: muls r0,r6
  000d7778: lsls r1,r5,#0x1f
  000d777a: beq 0x000d774c
  000d777c: vmov s0,r0
  000d7780: movs r2,#0x0
  000d7782: vcvt.f32.s32 s0,s0
  000d7786: ldr.w r1,[r4,#0x98]
  000d778a: str.w r0,[r1,r9,lsl #0x2]
  000d778e: movs r0,#0x0
  000d7790: mov r1,r8
  000d7792: str r0,[sp,#0x0]
  000d7794: add r0,sp,#0x44
  000d7796: vmul.f32 s0,s0,s16
  000d779a: vmul.f32 s0,s0,s18
  000d779e: vmov r3,s0
  000d77a2: blx 0x00072094
  000d77a6: cmp.w r10,#0x1
  000d77aa: bne 0x000d77b2
  000d77ac: mov.w r5,#0x1900
  000d77b0: b 0x000d77be
  000d77b2: ldr.w r0,[r4,#0x9c]
  000d77b6: add.w r0,r0,r10, lsl #0x2
  000d77ba: ldr.w r5,[r0,#-0x8]
  000d77be: ldr.w r0,[r11,#0x0]
  000d77c2: mov.w r1,#0x15e0
  000d77c6: blx 0x00071848
  000d77ca: add r0,r5
  000d77cc: ldr.w r1,[r4,#0x9c]
  000d77d0: add.w r0,r0,#0x640
  000d77d4: add r5,sp,#0x2c
  000d77d6: add r2,sp,#0x44
  000d77d8: vmov s0,r0
  000d77dc: vcvt.f32.s32 s0,s0
  000d77e0: str.w r0,[r1,r9,lsl #0x2]
  000d77e4: movs r0,#0x0
  000d77e6: strd r0,r0,[sp,#0x44]
  000d77ea: mov r1,r8
  000d77ec: vstr.32 s0,[sp,#0x4c]
  000d77f0: strd r0,r0,[sp,#0x38]
  000d77f4: str r0,[sp,#0x40]
  000d77f6: mov r0,r5
  000d77f8: blx 0x0006f688
  000d77fc: add r6,sp,#0x38
  000d77fe: mov r1,r5
  000d7800: mov r0,r6
  000d7802: blx 0x0006eb3c
  000d7806: ldr.w r0,[r4,#0x90]
  000d780a: mov r1,r6
  000d780c: ldr r0,[r0,#0x4]
  000d780e: ldr.w r0,[r0,r10,lsl #0x2]
  000d7812: blx 0x00072460
  000d7816: ldr r0,[r4,#0x58]
  000d7818: ldr r0,[r0,#0x4]
  000d781a: ldr.w r0,[r0,r9,lsl #0x2]
  000d781e: blx 0x000736b4
  000d7822: mov r8,r0
  000d7824: ldr r0,[0x000d7964]
  000d7826: add r0,pc
  000d7828: ldr.w r0,[r0,r8,lsl #0x2]
  000d782c: lsls r0,r0,#0x4
  000d782e: vmov s0,r0
  000d7832: vcvt.f32.s32 s0,s0
  000d7836: ldr.w r0,[r4,#0x90]
  000d783a: ldr r0,[r0,#0x4]
  000d783c: ldr.w r0,[r0,r10,lsl #0x2]
  000d7840: vmul.f32 s0,s0,s16
  000d7844: vmov r1,s0
  000d7848: mov r2,r1
  000d784a: mov r3,r1
  000d784c: blx 0x000727b4
  000d7850: ldr.w r0,[r4,#0x90]
  000d7854: ldr r0,[r0,#0x4]
  000d7856: ldr.w r1,[r0,r10,lsl #0x2]
  000d785a: mov r0,r5
  000d785c: blx 0x000720b8
  000d7860: add r6,sp,#0x20
  000d7862: mov r1,r5
  000d7864: mov r0,r6
  000d7866: blx 0x0006ec80
  000d786a: mov r0,r5
  000d786c: mov r1,r6
  000d786e: blx 0x0006eb3c
  000d7872: ldr.w r1,[r4,#0x90]
  000d7876: ldr.w r0,[r4,#0xa4]
  000d787a: ldr r1,[r1,#0x4]
  000d787c: ldr.w r1,[r1,r10,lsl #0x2]
  000d7880: ldr r1,[r1,#0xc]
  000d7882: blx 0x000720e8
  000d7886: cmp.w r8,#0x15
  000d788a: bhi 0x000d792c
  000d788c: ldr r1,[0x000d7968]
  000d788e: movs r0,#0x1
  000d7890: lsl.w r0,r0,r8
  000d7894: tst r0,r1
  000d7896: beq 0x000d792c
  000d7898: ldr r5,[sp,#0x1c]
  000d789a: mov.w r1,#0xffffffff
  000d789e: str r1,[sp,#0x20]
  000d78a0: add r1,sp,#0x20
  000d78a2: ldr r0,[r5,#0x0]
  000d78a4: blx 0x000720ac
  000d78a8: ldr r0,[r5,#0x0]
  000d78aa: movw r2,#0x41d5
  000d78ae: ldr r1,[sp,#0x20]
  000d78b0: movs r3,#0x0
  000d78b2: blx 0x0007303c
  000d78b6: ldr.w r0,[r4,#0x90]
  000d78ba: ldr r6,[r5,#0x0]
  000d78bc: ldr r5,[sp,#0x20]
  000d78be: ldr r0,[r0,#0x4]
  000d78c0: ldr.w r0,[r0,r10,lsl #0x2]
  000d78c4: blx 0x000721cc
  000d78c8: mov r2,r0
  000d78ca: mov r0,r6
  000d78cc: mov r1,r5
  000d78ce: blx 0x000721c0
  000d78d2: ldr.w r0,[r4,#0xa4]
  000d78d6: ldr r1,[sp,#0x20]
  000d78d8: blx 0x000720e8
  000d78dc: ldr.w r0,[r4,#0xf8]
  000d78e0: cbz r0,0x000d790c
  000d78e2: ldr r0,[0x000d796c]
  000d78e4: ldr r1,[r4,#0x60]
  000d78e6: add r0,pc
  000d78e8: ldr r5,[r0,#0x0]
  000d78ea: ldr r0,[r5,#0x0]
  000d78ec: ldr r0,[r0,#0x7c]
  000d78ee: cmp r1,r0
  000d78f0: bne 0x000d790c
  000d78f2: ldr r0,[r4,#0x58]
  000d78f4: ldr r0,[r0,#0x4]
  000d78f6: ldr.w r0,[r0,r9,lsl #0x2]
  000d78fa: blx 0x00071824
  000d78fe: ldr r1,[r5,#0x0]
  000d7900: ldr.w r1,[r1,#0x80]
  000d7904: cmp r0,r1
  000d7906: it eq
  000d7908: str.w.eq r10,[r4,#0x1c4]
  000d790c: ldr.w r0,[r4,#0x90]
  000d7910: add.w r8,sp,#0x80
  000d7914: ldr r0,[r0,#0x4]
  000d7916: ldr.w r0,[r0,r10,lsl #0x2]
  000d791a: cbz r0,0x000d7926
  000d791c: ldr r1,[r0,#0xc]
  000d791e: ldr.w r0,[r4,#0xa4]
  000d7922: blx 0x000720e8
  000d7926: add.w r10,r10,#0x1
  000d792a: b 0x000d76e0
  000d792c: ldr r0,[0x000d7970]
  000d792e: ldr r1,[r4,#0x58]
  000d7930: add r0,pc
  000d7932: ldr r1,[r1,#0x4]
  000d7934: ldr r2,[r0,#0x0]
  000d7936: ldr.w r0,[r1,r9,lsl #0x2]
  000d793a: ldr r6,[r2,#0x0]
  000d793c: blx 0x00071824
  000d7940: mov r1,r0
  000d7942: mov r0,r6
  000d7944: blx 0x0007459c
  000d7948: cmp r0,#0x0
  000d794a: bne 0x000d7898
  000d794c: b 0x000d78dc
  000d7974: ldr r0,[r0,#0x4]
  000d7976: movs r1,#0x0
  000d7978: ldr r0,[r0,#0x4]
  000d797a: blx 0x00072d24
  000d797e: movs r0,#0xc
  000d7980: blx 0x0006eb24
  000d7984: mov r5,r0
  000d7986: blx 0x000717b8
  000d798a: ldr r0,[sp,#0xc]
  000d798c: mov r1,r5
  000d798e: str.w r5,[r4,#0x94]
  000d7992: blx 0x00073504
  000d7996: vldr.32 s18,[pc,#0x3ac]
  000d799a: movs r6,#0x0
  000d799c: b 0x000d7a42
  000d799e: movs r0,#0xc0
  000d79a0: blx 0x0006eb24
  000d79a4: mov r5,r0
  000d79a6: ldr r0,[sp,#0x1c]
  000d79a8: ldr r2,[r0,#0x0]
  000d79aa: mov r0,r5
  000d79ac: movw r1,#0x1a7b
  000d79b0: movs r3,#0x0
  000d79b2: blx 0x0007207c
  000d79b6: ldr.w r0,[r4,#0x94]
  000d79ba: ldr r0,[r0,#0x4]
  000d79bc: str.w r5,[r0,r6,lsl #0x2]
  000d79c0: ldr.w r1,[r4,#0x94]
  000d79c4: ldr.w r0,[r4,#0xa4]
  000d79c8: ldr r1,[r1,#0x4]
  000d79ca: ldr.w r1,[r1,r6,lsl #0x2]
  000d79ce: ldr r1,[r1,#0xc]
  000d79d0: blx 0x000720e8
  000d79d4: ldr.w r0,[r4,#0x94]
  000d79d8: ldr.w r1,[r4,#0x9c]
  000d79dc: ldr r2,[r0,#0x4]
  000d79de: add.w r0,r1,r6, lsl #0x2
  000d79e2: movw r1,#0xc45
  000d79e6: vldr.32 s0,[r0]
  000d79ea: ldr.w r0,[r11,#0x0]
  000d79ee: ldr.w r5,[r2,r6,lsl #0x2]
  000d79f2: vcvt.f32.s32 s20,s0
  000d79f6: blx 0x00071848
  000d79fa: vmov s0,r0
  000d79fe: mov r0,r5
  000d7a00: movs r1,#0x0
  000d7a02: movs r3,#0x0
  000d7a04: vcvt.f32.s32 s0,s0
  000d7a08: vdiv.f32 s0,s0,s18
  000d7a0c: vmov r2,s0
  000d7a10: blx 0x00073054
  000d7a14: vcvt.s32.f32 s0,s20
  000d7a18: vmov r0,s0
  000d7a1c: lsls r0,r0,#0x1
  000d7a1e: vmov s0,r0
  000d7a22: vcvt.f32.s32 s0,s0
  000d7a26: ldr.w r0,[r4,#0x94]
  000d7a2a: ldr r0,[r0,#0x4]
  000d7a2c: ldr.w r0,[r0,r6,lsl #0x2]
  000d7a30: vmul.f32 s0,s0,s16
  000d7a34: vmov r1,s0
  000d7a38: mov r2,r1
  000d7a3a: mov r3,r1
  000d7a3c: blx 0x000727b4
  000d7a40: adds r6,#0x1
  000d7a42: ldr.w r0,[r4,#0x94]
  000d7a46: ldr r0,[r0,#0x0]
  000d7a48: cmp r6,r0
  000d7a4a: bcc 0x000d799e
  000d7a4c: ldr r1,[r4,#0x68]
  000d7a4e: add r6,sp,#0x80
  000d7a50: ldr r0,[r4,#0x60]
  000d7a52: ldr.w r5,[r4,#0xa4]
  000d7a56: ldr r1,[r1,#0x4]
  000d7a58: ldr.w r1,[r1,r0,lsl #0x2]
  000d7a5c: mov r0,r6
  000d7a5e: blx 0x000720b8
  000d7a62: mov r0,r5
  000d7a64: mov r1,r6
  000d7a66: blx 0x00072148
  000d7a6a: ldr.w r0,[r4,#0xa4]
  000d7a6e: mov.w r1,#0x3c000000
  000d7a72: mov.w r2,#0x3c000000
  000d7a76: mov.w r3,#0x3c000000
  000d7a7a: blx 0x000727b4
  000d7a7e: ldr r2,[0x000d7d58]
  000d7a80: movs r1,#0x0
  000d7a82: ldr.w r0,[r4,#0xa4]
  000d7a86: movs r3,#0x0
  000d7a88: mov.w r8,#0x0
  000d7a8c: blx 0x00073054
  000d7a90: ldr r5,[0x000d7d5c]
  000d7a92: mov.w r6,#0x45800000
  000d7a96: ldr.w r0,[r4,#0xa4]
  000d7a9a: ldr r1,[0x000d7d60]
  000d7a9c: ldr r2,[0x000d7d64]
  000d7a9e: ldr r3,[0x000d7d68]
  000d7aa0: strd r6,r5,[r4,#0x188]
  000d7aa4: blx 0x00073054
  000d7aa8: ldr r0,[r4,#0x54]
  000d7aaa: mov.w r2,#0xffffffff
  000d7aae: ldr r1,[r4,#0x60]
  000d7ab0: str r2,[r4,#0x64]
  000d7ab2: ldr r0,[r0,#0x4]
  000d7ab4: ldr r2,[sp,#0x1c]
  000d7ab6: ldr.w r0,[r0,r1,lsl #0x2]
  000d7aba: ldr r5,[r2,#0x0]
  000d7abc: blx 0x00071950
  000d7ac0: ldr r1,[0x000d7d84]
  000d7ac2: add.w r2,r4,#0x34
  000d7ac6: add r1,pc
  000d7ac8: ldrh.w r1,[r1,r0,lsl #0x2]
  000d7acc: mov r0,r5
  000d7ace: blx 0x00071cf8
  000d7ad2: ldr.w r0,[r4,#0x198]
  000d7ad6: cbz r0,0x000d7aea
  000d7ad8: blx 0x0007216c
  000d7adc: ldr.w r0,[r4,#0x198]
  000d7ae0: cbz r0,0x000d7aea
  000d7ae2: blx 0x00072178
  000d7ae6: blx 0x0006eb48
  000d7aea: movs r0,#0xc
  000d7aec: str.w r8,[r4,#0x198]
  000d7af0: blx 0x0006eb24
  000d7af4: mov r5,r0
  000d7af6: blx 0x00074530
  000d7afa: ldr r0,[r4,#0x58]
  000d7afc: mov r1,r5
  000d7afe: str.w r5,[r4,#0x198]
  000d7b02: ldr r0,[r0,#0x0]
  000d7b04: blx 0x0007453c
  000d7b08: mov.w r8,#0x0
  000d7b0c: movs r6,#0x0
  000d7b0e: b 0x000d7b26
  000d7b10: movs r0,#0xc
  000d7b12: blx 0x0006eb24
  000d7b16: strd r8,r8,[r0,#0x0]
  000d7b1a: str.w r8,[r0,#0x8]
  000d7b1e: ldr r1,[r5,#0x4]
  000d7b20: str.w r0,[r1,r6,lsl #0x2]
  000d7b24: adds r6,#0x1
  000d7b26: ldr.w r5,[r4,#0x198]
  000d7b2a: ldr r0,[r5,#0x0]
  000d7b2c: cmp r6,r0
  000d7b2e: bcc 0x000d7b10
  000d7b30: ldr r0,[0x000d7d88]
  000d7b32: add r0,pc
  000d7b34: ldr.w r8,[r0,#0x0]
  000d7b38: ldr.w r0,[r8,#0x0]
  000d7b3c: blx 0x0007258c
  000d7b40: ldr r2,[0x000d7d6c]
  000d7b42: mov.w r1,#0x4000
  000d7b46: str r1,[sp,#0x0]
  000d7b48: movs r1,#0x0
  000d7b4a: movs r5,#0x0
  000d7b4c: mov r3,r2
  000d7b4e: blx 0x000745a8
  000d7b52: ldr.w r0,[r4,#0x1bc]
  000d7b56: cbz r0,0x000d7b60
  000d7b58: blx 0x00071fc8
  000d7b5c: blx 0x0006eb48
  000d7b60: ldr r1,[r4,#0x54]
  000d7b62: mov.w r0,#0xffffffff
  000d7b66: ldr r2,[r4,#0x60]
  000d7b68: str.w r0,[r4,#0x178]
  000d7b6c: str.w r5,[r4,#0x1bc]
  000d7b70: ldr r0,[r1,#0x4]
  000d7b72: ldr.w r0,[r0,r2,lsl #0x2]
  000d7b76: blx 0x0007390c
  000d7b7a: ldr r1,[0x000d7d8c]
  000d7b7c: movs r3,#0x0
  000d7b7e: ldr r6,[sp,#0x1c]
  000d7b80: add r1,pc
  000d7b82: ldr r2,[r4,#0x60]
  000d7b84: ldrh.w r1,[r1,r0,lsl #0x2]
  000d7b88: cmp r2,#0x1b
  000d7b8a: ldr r0,[r6,#0x0]
  000d7b8c: it eq
  000d7b8e: movw.eq r1,#0x2734
  000d7b92: add.w r2,r4,#0x178
  000d7b96: uxth r1,r1
  000d7b98: blx 0x0006fbb0
  000d7b9c: movs r0,#0xc0
  000d7b9e: blx 0x0006eb24
  000d7ba2: mov r5,r0
  000d7ba4: ldr r2,[r6,#0x0]
  000d7ba6: movw r1,#0x1a70
  000d7baa: movs r3,#0x0
  000d7bac: blx 0x0007207c
  000d7bb0: str.w r5,[r4,#0x1bc]
  000d7bb4: add r6,sp,#0x80
  000d7bb6: ldr r1,[r4,#0x68]
  000d7bb8: ldr r0,[r4,#0x60]
  000d7bba: ldr r1,[r1,#0x4]
  000d7bbc: ldr.w r1,[r1,r0,lsl #0x2]
  000d7bc0: mov r0,r6
  000d7bc2: blx 0x000720b8
  000d7bc6: mov r0,r5
  000d7bc8: mov r1,r6
  000d7bca: blx 0x00072148
  000d7bce: ldr.w r0,[r4,#0x1bc]
  000d7bd2: movs r1,#0x0
  000d7bd4: movs r2,#0x0
  000d7bd6: movs r3,#0x0
  000d7bd8: blx 0x00073054
  000d7bdc: ldr r0,[r4,#0x54]
  000d7bde: ldr r1,[r4,#0x60]
  000d7be0: ldr r0,[r0,#0x4]
  000d7be2: ldr.w r0,[r0,r1,lsl #0x2]
  000d7be6: blx 0x0007390c
  000d7bea: cmp r0,#0xf
  000d7bec: bne 0x000d7c08
  000d7bee: ldr r0,[0x000d7d90]
  000d7bf0: add r0,pc
  000d7bf2: ldr r5,[r0,#0x0]
  000d7bf4: ldr r0,[r5,#0x0]
  000d7bf6: blx 0x00071770
  000d7bfa: ldr r6,[sp,#0x8]
  000d7bfc: cmp r0,#0x69
  000d7bfe: bgt 0x000d7c1a
  000d7c00: ldr.w r0,[r4,#0x1bc]
  000d7c04: ldr r1,[0x000d7d78]
  000d7c06: b 0x000d7c2e
  000d7c08: ldr r1,[0x000d7d70]
  000d7c0a: ldr.w r0,[r4,#0x1bc]
  000d7c0e: mov r2,r1
  000d7c10: mov r3,r1
  000d7c12: blx 0x000727b4
  000d7c16: ldr r6,[sp,#0x8]
  000d7c18: b 0x000d7c36
  000d7c1a: ldr r0,[r5,#0x0]
  000d7c1c: blx 0x00071770
  000d7c20: mov r1,r0
  000d7c22: ldr.w r0,[r4,#0x1bc]
  000d7c26: cmp r1,#0x9d
  000d7c28: ite le
  000d7c2a: ldr.le r1,[0x000d7d74]
  000d7c2c: ldr.gt r1,[0x000d7d70]
  000d7c2e: mov r2,r1
  000d7c30: mov r3,r1
  000d7c32: blx 0x000727b4
  000d7c36: ldr r1,[r4,#0x68]
  000d7c38: ldr r0,[r4,#0x60]
  000d7c3a: ldr r1,[r1,#0x4]
  000d7c3c: ldr.w r0,[r1,r0,lsl #0x2]
  000d7c40: movs r1,#0x0
  000d7c42: blx 0x00072d24
  000d7c46: ldr.w r1,[r4,#0x1bc]
  000d7c4a: add r0,sp,#0x80
  000d7c4c: blx 0x000720b8
  000d7c50: ldr r1,[0x000d7d94]
  000d7c52: ldr.w r0,[r8,#0x0]
  000d7c56: add r1,pc
  000d7c58: ldr r4,[r1,#0x0]
  000d7c5a: blx r4
  000d7c5c: add r3,sp,#0x80
  000d7c5e: mov.w r5,#0x4000
  000d7c62: ldmia r3,{r1,r2,r3}
  000d7c64: str r5,[sp,#0x0]
  000d7c66: blx 0x000745b4
  000d7c6a: ldr.w r0,[r8,#0x0]
  000d7c6e: blx r4
  000d7c70: ldr r1,[0x000d7d7c]
  000d7c72: str r5,[sp,#0x0]
  000d7c74: mov r2,r1
  000d7c76: mov r3,r1
  000d7c78: blx 0x0007087c
  000d7c7c: ldr.w r0,[r8,#0x0]
  000d7c80: blx r4
  000d7c82: mov.w r1,#0x40000000
  000d7c86: mov.w r2,#0x40000000
  000d7c8a: mov.w r3,#0x40000000
  000d7c8e: str r5,[sp,#0x0]
  000d7c90: blx 0x000745c0
  000d7c94: ldr.w r0,[r8,#0x0]
  000d7c98: blx r4
  000d7c9a: mov.w r1,#0x3f800000
  000d7c9e: mov.w r2,#0x3f800000
  000d7ca2: mov.w r3,#0x3f800000
  000d7ca6: blx 0x000745cc
  000d7caa: ldr.w r0,[r8,#0x0]
  000d7cae: blx r4
  000d7cb0: mov.w r1,#0x3f800000
  000d7cb4: mov.w r2,#0x3f800000
  000d7cb8: mov.w r3,#0x3f800000
  000d7cbc: blx 0x000745d8
  000d7cc0: ldr.w r0,[r8,#0x0]
  000d7cc4: blx r4
  000d7cc6: movs r1,#0x0
  000d7cc8: movs r2,#0x0
  000d7cca: movs r3,#0x0
  000d7ccc: blx 0x000745e4
  000d7cd0: ldr.w r0,[r8,#0x0]
  000d7cd4: blx r4
  000d7cd6: ldr r1,[0x000d7d80]
  000d7cd8: blx 0x000745f0
  000d7cdc: ldr.w r0,[r8,#0x0]
  000d7ce0: blx r4
  000d7ce2: ldr r1,[0x000d7d98]
  000d7ce4: movs r2,#0x1
  000d7ce6: str.w r2,[r0,#0x32c]
  000d7cea: add r1,pc
  000d7cec: ldr.w r4,[r11,#0x0]
  000d7cf0: ldr r1,[r1,#0x0]
  000d7cf2: ldr r0,[r1,#0x0]
  000d7cf4: blx 0x00071710
  000d7cf8: mov r2,r0
  000d7cfa: mov r0,r4
  000d7cfc: mov r3,r1
  000d7cfe: blx 0x000739c0
  000d7d02: ldr r0,[sp,#0xbc]
  000d7d04: ldr r1,[r6,#0x0]
  000d7d06: subs r0,r1,r0
  000d7d08: itttt eq
  000d7d0a: add.eq sp,#0xc0
  000d7d0c: vpop.eq {d8,d9,d10,d11,d12,d13}
  000d7d10: add.eq sp,#0x4
  000d7d12: pop.eq.w {r8,r9,r10,r11}
  000d7d16: it eq
  000d7d18: pop.eq {r4,r5,r6,r7,pc}
  000d7d1a: blx 0x0006e824
```
