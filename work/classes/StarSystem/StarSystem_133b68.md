# StarSystem::StarSystem
elf 0x133b68 body 2692
Sig: undefined __thiscall StarSystem(StarSystem * this, int param_1)

## decompile
```c

/* StarSystem::StarSystem(int) */

void __thiscall StarSystem::StarSystem(StarSystem *this,int param_1)

{
  Vector *this_00;
  float fVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  StarSystem SVar5;
  int iVar6;
  LensFlare *this_01;
  undefined4 *puVar7;
  Array<AEGeometry*> *pAVar8;
  AEGeometry *pAVar9;
  int *piVar10;
  Array<unsigned_int> *pAVar11;
  Array<AbyssEngine::AEMath::Vector> *pAVar12;
  SolarSystem *pSVar13;
  uint *puVar14;
  FileRead *this_02;
  Array *this_03;
  void *pvVar15;
  Array *pAVar16;
  int iVar17;
  Array<KIPlayer*> *this_04;
  Station *pSVar18;
  Engine *pEVar19;
  PlayerStatic *this_05;
  uint uVar20;
  uint *puVar21;
  int iVar22;
  char cVar23;
  bool bVar24;
  bool bVar25;
  uint uVar26;
  int iVar27;
  ushort uVar28;
  undefined4 *puVar29;
  undefined4 uVar30;
  Status *this_06;
  int iVar31;
  int *piVar32;
  uint in_fpscr;
  float fVar33;
  float extraout_s0;
  float fVar34;
  float fVar35;
  float extraout_s1;
  float extraout_s2;
  double dVar36;
  int local_e8;
  float local_c8;
  float local_c4;
  float local_c0;
  float local_bc [24];
  int local_5c;
  
  piVar32 = *(int **)(DAT_00143f4c + 0x143b86);
  puVar29 = *(undefined4 **)(DAT_00143f50 + 0x143b88);
  local_5c = *piVar32;
  this_00 = (Vector *)(this + 0x30);
  *(undefined4 *)this_00 = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  iVar6 = Status::getSystem();
  this[0x28] = (StarSystem)(iVar6 == 0);
  SVar5 = (StarSystem)Status::inSupernovaSystem();
  this[0xc] = SVar5;
  this_01 = operator_new(0x14);
  puVar7 = *(undefined4 **)(DAT_00143f54 + 0x143bd0);
  LensFlare::LensFlare(this_01,(PaintCanvas *)*puVar7);
  *(undefined4 *)(this + 0x44) = 0;
  *(LensFlare **)(this + 0x2c) = this_01;
  *(undefined4 *)(this + 0x4c) = 0xffffffff;
  if (this[0x28] != (StarSystem)0x0) {
    pAVar8 = operator_new(0xc);
    Array<AEGeometry*>::Array(pAVar8);
    *(Array<AEGeometry*> **)(this + 0x1c) = pAVar8;
    ArraySetLength<AEGeometry*>(2,(Array *)pAVar8);
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x1a70,(PaintCanvas *)*puVar7,false);
    fVar1 = DAT_00143f58;
    **(undefined4 **)(*(int *)(this + 0x1c) + 4) = pAVar9;
    local_bc[0] = fVar1;
    local_bc[1] = fVar1;
    local_bc[2] = fVar1;
    fVar33 = (float)AEGeometry::setScaling
                              ((AEGeometry *)**(undefined4 **)(*(int *)(this + 0x1c) + 4),
                               (Vector *)local_bc);
    AEGeometry::moveForward(fVar33);
    AEGeometry::getDirection();
    AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)local_bc);
    *(float *)(this + 0x30) = -*(float *)(this + 0x30);
    *(float *)(this + 0x34) = -*(float *)(this + 0x34);
    *(float *)(this + 0x38) = -*(float *)(this + 0x38);
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x1a70,(PaintCanvas *)*puVar7,false);
    piVar10 = *(int **)(DAT_00143f60 + 0x143cac);
    *(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + 4) = pAVar9;
    iVar6 = AbyssEngine::AERandom::nextInt(*piVar10);
    local_bc[0] = (float)VectorSignedToFloat(iVar6 + 20000,(byte)(in_fpscr >> 0x16) & 3);
    local_bc[0] = local_bc[0] * DAT_00143f64;
    local_bc[1] = local_bc[0];
    local_bc[2] = local_bc[0];
    AEGeometry::setScaling
              (*(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + 4),(Vector *)local_bc);
    local_bc[0] = 0.0;
    local_bc[1] = (float)DAT_00143f68;
    local_bc[2] = 0.0;
    fVar33 = (float)AEGeometry::setRotation
                              (*(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + 4),
                               (Vector *)local_bc);
    AEGeometry::moveForward(fVar33);
    pAVar11 = operator_new(0xc);
    Array<unsigned_int>::Array(pAVar11);
    *(Array<unsigned_int> **)(this + 0x14) = pAVar11;
    ArraySetLength<unsigned_int>(2,(Array *)pAVar11);
    AbyssEngine::PaintCanvas::TextureCreate
              ((ushort)*puVar7,(uint *)0x2739,SUB41(*(undefined4 *)(*(int *)(this + 0x14) + 4),0));
    AbyssEngine::PaintCanvas::TextureCreate
              ((ushort)*puVar7,(uint *)0x2719,
               (bool)((char)*(undefined4 *)(*(int *)(this + 0x14) + 4) + '\x04'));
    pAVar12 = operator_new(0xc);
    Array<AbyssEngine::AEMath::Vector>::Array(pAVar12);
    *(Array<AbyssEngine::AEMath::Vector> **)(this + 0x20) = pAVar12;
    ArraySetLength<AbyssEngine::AEMath::Vector>(2,(Array *)pAVar12);
    AEGeometry::getPosition();
    AbyssEngine::AEMath::Vector::operator=
              (*(Vector **)(*(int *)(this + 0x20) + 4),(Vector *)local_bc);
    AEGeometry::getPosition();
    AbyssEngine::AEMath::Vector::operator=
              ((Vector *)(*(int *)(*(int *)(this + 0x20) + 4) + 0xc),(Vector *)local_bc);
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x1a70,(PaintCanvas *)*puVar7,false);
    local_bc[0] = DAT_00143f70;
    local_bc[1] = (float)DAT_00143f6c;
    *(AEGeometry **)(this + 0x40) = pAVar9;
    local_bc[2] = fVar1;
    AEGeometry::setScaling(pAVar9,(Vector *)local_bc);
    goto LAB_00143dca;
  }
  pSVar13 = (SolarSystem *)Status::getSystem();
  puVar14 = (uint *)SolarSystem::getStations(pSVar13);
  this_02 = operator_new(1);
  FileRead::FileRead(this_02);
  pSVar13 = (SolarSystem *)Status::getSystem();
  this_03 = (Array *)FileRead::loadStationsBinary(this_02,pSVar13);
  pvVar15 = (void *)FileRead::~FileRead(this_02);
  operator_delete(pvVar15);
  pAVar11 = operator_new(0xc);
  Array<unsigned_int>::Array(pAVar11);
  *(Array<unsigned_int> **)(this + 0x14) = pAVar11;
  ArraySetLength<unsigned_int>(*(int *)this_03 + 1,(Array *)pAVar11);
  pAVar16 = operator_new(0xc);
  Array<int>::Array();
  *(Array **)(this + 0x24) = pAVar16;
  ArraySetLength<int>(*puVar14,pAVar16);
  iVar6 = Status::getCurrentCampaignMission();
  if (iVar6 == 0x59) {
    if (this[0xc] == (StarSystem)0x0) goto LAB_00143ea8;
LAB_00143e8c:
    AbyssEngine::PaintCanvas::TextureCreate
              ((ushort)*puVar7,(uint *)0x2dde,SUB41(*(undefined4 *)(*(int *)(this + 0x14) + 4),0));
    uVar30 = 3;
  }
  else {
    if (0x9d < iVar6) {
      Status::getSystem();
      iVar6 = SolarSystem::getIndex();
      if (iVar6 == 0x1b) goto LAB_00143e8c;
    }
LAB_00143ea8:
    uVar30 = *puVar7;
    Status::getSystem();
    iVar6 = SolarSystem::getTextureIndex();
    AbyssEngine::PaintCanvas::TextureCreate
              ((ushort)uVar30,(uint *)(uint)*(ushort *)(DAT_00143f74 + 0x143ec8 + iVar6 * 4),
               SUB41(*(undefined4 *)(*(int *)(this + 0x14) + 4),0));
    Status::getSystem();
    iVar6 = SolarSystem::getIndex();
    uVar30 = *(undefined4 *)(DAT_00143f78 + 0x143ee4 + iVar6 * 4);
  }
  *(undefined4 *)(this + 0x3c) = uVar30;
  iVar31 = 0;
  iVar6 = DAT_00143f7c + 0x143efa;
  iVar17 = DAT_00143f80 + 0x143f00;
  for (uVar26 = 1; uVar26 < **(uint **)(this + 0x14); uVar26 = uVar26 + 1) {
    iVar27 = *(int *)(puVar14[1] + iVar31);
    pSVar18 = (Station *)Status::getStation();
    iVar22 = Station::getIndex(pSVar18);
    if (iVar27 == iVar22) {
      iVar22 = Status::getCurrentCampaignMission();
      if (param_1 == 3 && iVar22 == 0) {
        *(uint *)(this + 0x50) = uVar26;
        cVar23 = (char)*(undefined4 *)(*(int *)(this + 0x14) + 4);
        puVar21 = (uint *)0x273b;
        uVar28 = (ushort)*puVar7;
      }
      else {
        uVar28 = (ushort)*puVar7;
        iVar22 = Station::getTextureIndex(*(Station **)(*(int *)(this_03 + 4) + iVar31));
        cVar23 = (char)*(undefined4 *)(*(int *)(this + 0x14) + 4);
        puVar21 = (uint *)(uint)*(ushort *)(iVar6 + iVar22 * 4);
      }
      AbyssEngine::PaintCanvas::TextureCreate(uVar28,puVar21,(bool)(cVar23 + (char)iVar31 + '\x04'))
      ;
    }
    else {
      uVar30 = *puVar7;
      iVar22 = Station::getTextureIndex(*(Station **)(*(int *)(this_03 + 4) + iVar31));
      AbyssEngine::PaintCanvas::TextureCreate
                ((ushort)uVar30,(uint *)(uint)*(ushort *)(iVar17 + iVar22 * 4),
                 (bool)((char)*(undefined4 *)(*(int *)(this + 0x14) + 4) + (char)iVar31 + '\x04'));
      this_06 = (Status *)*puVar29;
      iVar22 = Station::getIndex(*(Station **)(*(int *)(this_03 + 4) + iVar31));
      iVar22 = Status::orbitHasPlanetRing(this_06,iVar22);
      if (iVar22 != 0) {
        pAVar9 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar9,0x1a70,(PaintCanvas *)*puVar7,false);
        *(AEGeometry **)(this + 0x44) = pAVar9;
        AbyssEngine::PaintCanvas::TextureCreate
                  ((ushort)*puVar7,(uint *)0x7198,(bool)((char)this + 'H'));
        *(uint *)(this + 0x4c) = uVar26;
      }
    }
    uVar30 = Station::getIndex(*(Station **)(*(int *)(this_03 + 4) + iVar31));
    *(undefined4 *)(*(int *)(*(int *)(this + 0x24) + 4) + iVar31) = uVar30;
    iVar31 = iVar31 + 4;
  }
  ArrayReleaseClasses<Station*>(this_03);
  pvVar15 = (void *)Array<Station*>::~Array((Array<Station*> *)this_03);
  operator_delete(pvVar15);
  this_04 = operator_new(0xc);
  Array<KIPlayer*>::Array(this_04);
  *(Array<KIPlayer*> **)(this + 0x18) = this_04;
  ArraySetLength<KIPlayer*>(*puVar14,(Array *)this_04);
  pAVar8 = operator_new(0xc);
  Array<AEGeometry*>::Array(pAVar8);
  *(Array<AEGeometry*> **)(this + 0x1c) = pAVar8;
  ArraySetLength<AEGeometry*>(*puVar14 + 1,(Array *)pAVar8);
  pAVar12 = operator_new(0xc);
  Array<AbyssEngine::AEMath::Vector>::Array(pAVar12);
  *(Array<AbyssEngine::AEMath::Vector> **)(this + 0x20) = pAVar12;
  ArraySetLength<AbyssEngine::AEMath::Vector>(*puVar14 + 1,(Array *)pAVar12);
  piVar10 = *(int **)(DAT_00144404 + 0x14409c);
  iVar6 = *piVar10;
  pSVar18 = (Station *)Status::getStation();
  Station::getIndex(pSVar18);
  AbyssEngine::AERandom::setSeed(CONCAT44(300,iVar6));
  dVar4 = DAT_00144428;
  dVar3 = DAT_00144420;
  dVar2 = DAT_00144418;
  fVar33 = DAT_00144410;
  fVar1 = DAT_00143f64;
  for (iVar6 = 0; iVar6 != 0x18; iVar6 = iVar6 + 1) {
    local_bc[iVar6] = 0.0;
  }
  iVar6 = 0;
  iVar17 = 0;
  fVar34 = DAT_0014440c;
  for (uVar26 = 0; uVar26 < **(uint **)(this + 0x1c); uVar26 = uVar26 + 1) {
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x1a70,(PaintCanvas *)*puVar7,false);
    *(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4) = pAVar9;
    if (this[0x28] == (StarSystem)0x0) {
      Status::getSystem();
      iVar31 = SolarSystem::getTextureIndex();
      if (0xe < iVar31) {
        pEVar19 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        iVar31 = AbyssEngine::Engine::IsPostEffectActivated(pEVar19);
        if (iVar31 != 0) {
          iVar31 = AbyssEngine::PaintCanvas::MeshGetPointer
                             ((PaintCanvas *)*puVar7,
                              *(uint *)(*(int *)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4) +
                                       0x1c));
          *(undefined4 *)(iVar31 + 0x1c) = DAT_00144430;
        }
      }
    }
    if (uVar26 == 0) {
      iVar17 = AbyssEngine::AERandom::nextInt(*piVar10);
      pSVar18 = (Station *)Status::getStation();
      iVar6 = Station::getTextureIndex(pSVar18);
      uVar20 = iVar6 - 9;
      if ((uVar20 < 0xe) && ((0x3a31U >> (uVar20 & 0xff) & 1) != 0)) {
        iVar17 = *(int *)(DAT_001446b8 + 0x14443e + uVar20 * 4);
      }
      else {
        iVar17 = iVar17 + 5;
      }
      if (this[0xc] == (StarSystem)0x0) {
        local_c8 = fVar34;
        local_c4 = fVar34;
        local_c0 = fVar34;
        AEGeometry::setScaling
                  ((AEGeometry *)**(undefined4 **)(*(int *)(this + 0x1c) + 4),(Vector *)&local_c8);
        pAVar9 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar9,0x1a70,(PaintCanvas *)*puVar7,false);
        local_c4 = DAT_0014469c;
        *(AEGeometry **)(this + 0x40) = pAVar9;
        local_c8 = DAT_00144698;
        local_c0 = fVar34;
        AEGeometry::setScaling(pAVar9,(Vector *)&local_c8);
      }
      else {
        iVar6 = Status::getCurrentCampaignMission();
        local_c8 = DAT_001446a8;
        if (0x69 < iVar6) {
          local_c8 = DAT_001446ac;
        }
        local_c4 = local_c8;
        local_c0 = local_c8;
        AEGeometry::setScaling
                  ((AEGeometry *)**(undefined4 **)(*(int *)(this + 0x1c) + 4),(Vector *)&local_c8);
        pAVar9 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar9,0x1a70,(PaintCanvas *)*puVar7,false);
        local_c4 = DAT_001446b4;
        *(AEGeometry **)(this + 0x40) = pAVar9;
        local_c8 = DAT_001446b0;
        local_c0 = DAT_001446a8;
        AEGeometry::setScaling(pAVar9,(Vector *)&local_c8);
        pEVar19 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        iVar6 = AbyssEngine::Engine::IsPostEffectActivated(pEVar19);
        if (iVar6 != 0) {
          iVar6 = AbyssEngine::PaintCanvas::MeshGetPointer
                            ((PaintCanvas *)*puVar7,*(uint *)(*(int *)(this + 0x40) + 0x1c));
          *(undefined4 *)(iVar6 + 0x1c) = DAT_001446a0;
        }
        AbyssEngine::PaintCanvas::TextureCreate
                  ((ushort)*puVar7,(uint *)0x2dde,(bool)((char)this + '\x10'));
      }
      iVar31 = 0x8000;
      local_bc[iVar17] = 1.4013e-45;
      iVar6 = iVar17;
LAB_00144522:
      pAVar9 = *(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4);
      iVar22 = AbyssEngine::AERandom::nextInt(*piVar10);
      fVar34 = (float)VectorSignedToFloat(iVar6 * 0xaaa,(byte)(in_fpscr >> 0x16) & 3);
      fVar35 = (float)VectorSignedToFloat(iVar22 + -0x800,(byte)(in_fpscr >> 0x16) & 3);
      local_c0 = 0.0;
      local_c4 = fVar34 * fVar1 * fVar33;
      local_c8 = fVar35 * fVar1 * fVar33;
      fVar34 = (float)AEGeometry::setRotation(pAVar9,(Vector *)&local_c8);
    }
    else {
      iVar22 = uVar26 - 1;
      iVar27 = *(int *)(puVar14[1] + iVar22 * 4);
      pSVar18 = (Station *)Status::getStation();
      iVar31 = Station::getIndex(pSVar18);
      this_05 = operator_new(0x130);
      pAVar9 = *(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4);
      if (iVar27 == iVar31) {
        PlayerStatic::PlayerStatic(this_05,0,pAVar9,extraout_s0,extraout_s1,extraout_s2);
        *(PlayerStatic **)(*(int *)(*(int *)(this + 0x18) + 4) + iVar22 * 4) = this_05;
        iVar31 = iVar6;
        bVar25 = true;
        do {
          bVar24 = bVar25;
          iVar6 = iVar31;
          iVar31 = 0;
          bVar25 = false;
        } while (bVar24);
        iVar31 = 0x8000;
        if (iVar17 < 0xc) {
          iVar31 = 0;
        }
        **(int **)(DAT_00144434 + 0x1441de) = iVar22;
        local_e8 = AbyssEngine::AERandom::nextInt(*piVar10);
        local_e8 = local_e8 + 20000;
        iVar27 = Status::getCurrentCampaignMission();
        if (iVar27 == 0) {
          fVar34 = (float)VectorSignedToFloat(local_e8,(byte)(in_fpscr >> 0x16) & 3);
          local_e8 = (int)(fVar34 * 0.5);
        }
        pSVar18 = (Station *)Status::getStation();
        uVar20 = Station::getTextureIndex(pSVar18);
        iVar27 = Status::inPlanetRingOrbit();
        if (iVar27 == 0) {
          if (uVar20 < 0x12) {
            uVar20 = 1 << (uVar20 & 0xff);
            if ((uVar20 & DAT_001446a4) == 0) {
              if ((uVar20 & 0x10200) != 0) {
                local_e8 = AbyssEngine::AERandom::nextInt(*piVar10);
                local_e8 = local_e8 + 0x7ef4;
              }
            }
            else {
              local_e8 = AbyssEngine::AERandom::nextInt(*piVar10);
              local_e8 = local_e8 + 35000;
            }
          }
        }
        else {
          local_e8 = 26000;
        }
        local_c8 = (float)VectorSignedToFloat(local_e8,(byte)(in_fpscr >> 0x16) & 3);
        local_c8 = local_c8 * fVar1;
        *(float *)(this + 0x58) = local_c8;
        local_c4 = local_c8;
        local_c0 = local_c8;
        AEGeometry::setScaling
                  (*(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4),
                   (Vector *)&local_c8);
      }
      else {
        PlayerStatic::PlayerStatic(this_05,0,pAVar9,extraout_s0,extraout_s1,extraout_s2);
        bVar25 = false;
        *(PlayerStatic **)(*(int *)(*(int *)(this + 0x18) + 4) + iVar22 * 4) = this_05;
        while (!bVar25) {
          iVar6 = AbyssEngine::AERandom::nextInt(*piVar10);
          iVar6 = iVar6 + 7;
          bVar25 = false;
          iVar31 = iVar6 - iVar17;
          if (iVar31 < 0) {
            iVar31 = -iVar31;
          }
          if (2 < iVar31) {
            bVar25 = local_bc[iVar6] == 0.0;
          }
        }
        uVar30 = AbyssEngine::AERandom::nextInt(*piVar10);
        dVar36 = (double)VectorSignedToFloat(uVar30,(byte)(in_fpscr >> 0x16) & 3);
        local_c8 = (float)((dVar3 + dVar36 * dVar2) * dVar4);
        local_c4 = local_c8;
        local_c0 = local_c8;
        AEGeometry::setScaling
                  (*(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4),
                   (Vector *)&local_c8);
        iVar31 = 0x8000;
        if (iVar17 - iVar6 < 0xc) {
          iVar31 = 0;
        }
        if (iVar17 <= iVar6) {
          iVar31 = 0x8000;
        }
      }
      local_bc[iVar6] = 1.4013e-45;
      if (iVar6 == iVar17) {
        fVar34 = (float)VectorSignedToFloat(iVar17 * 0xaaa,(byte)(in_fpscr >> 0x16) & 3);
        local_c8 = 0.0;
        local_c0 = 0.0;
        local_c4 = fVar34 * fVar1 * fVar33;
        fVar34 = (float)AEGeometry::setRotation
                                  (*(AEGeometry **)
                                    (*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4),
                                   (Vector *)&local_c8);
        iVar6 = iVar17;
      }
      else {
        iVar27 = *(int *)(puVar14[1] + iVar22 * 4);
        pSVar18 = (Station *)Status::getStation();
        iVar22 = Station::getIndex(pSVar18);
        if (iVar27 != iVar22) goto LAB_00144522;
        fVar34 = (float)VectorSignedToFloat(iVar6 * 0xaaa,(byte)(in_fpscr >> 0x16) & 3);
        local_c8 = 0.0;
        local_c0 = 0.0;
        local_c4 = fVar34 * fVar1 * fVar33;
        fVar34 = (float)AEGeometry::setRotation
                                  (*(AEGeometry **)
                                    (*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4),
                                   (Vector *)&local_c8);
      }
    }
    AEGeometry::moveForward(fVar34);
    fVar34 = DAT_00144690;
    if (uVar26 == 0) {
      AEGeometry::getDirection();
      AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_c8);
      fVar34 = DAT_00144690;
      *(float *)(this + 0x30) = -*(float *)(this + 0x30);
      *(float *)(this + 0x34) = -*(float *)(this + 0x34);
      *(float *)(this + 0x38) = -*(float *)(this + 0x38);
    }
    else if (iVar31 != 0) {
      fVar35 = (float)VectorSignedToFloat(iVar31,(byte)(in_fpscr >> 0x16) & 3);
      local_c8 = 0.0;
      local_c0 = 0.0;
      local_c4 = fVar35 * fVar1 * fVar33;
      AEGeometry::rotate(*(Vector **)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar26 * 4));
    }
    AEGeometry::getPosition();
    AbyssEngine::AEMath::Vector::operator=
              ((Vector *)(*(int *)(*(int *)(this + 0x20) + 4) + uVar26 * 0xc),(Vector *)&local_c8);
  }
  AbyssEngine::AERandom::reset();
LAB_00143dca:
  initLight();
  iVar6 = *piVar32 - local_5c;
  if (iVar6 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar6);
  }
  return;
}

```

## target disasm
```
  00143b68: push {r4,r5,r6,r7,lr}
  00143b6a: add r7,sp,#0xc
  00143b6c: push {r7,r8,r9,r10,r11}
  00143b70: vpush {d8,d9,d10,d11,d12,d13}
  00143b74: sub sp,#0xa0
  00143b76: str r1,[sp,#0x20]
  00143b78: mov r8,r0
  00143b7a: ldr r0,[0x00143f4c]
  00143b7c: movs r4,#0x0
  00143b7e: ldr r1,[0x00143f50]
  00143b80: mov r6,r8
  00143b82: add r0,pc
  00143b84: add r1,pc
  00143b86: ldr.w r9,[r0,#0x0]
  00143b8a: ldr r5,[r1,#0x0]
  00143b8c: ldr.w r0,[r9,#0x0]
  00143b90: str r0,[sp,#0x9c]
  00143b92: str.w r4,[r6,#0x30]!
  00143b96: strd r4,r4,[r6,#0x4]
  00143b9a: str.w r4,[r6,#-0xc]
  00143b9e: strd r4,r4,[r6,#-0x1c]
  00143ba2: str.w r4,[r6,#-0x14]
  00143ba6: ldr r0,[r5,#0x0]
  00143ba8: blx 0x00071a10
  00143bac: clz r0,r0
  00143bb0: str r5,[sp,#0x24]
  00143bb2: lsrs r0,r0,#0x5
  00143bb4: strb.w r0,[r6,#-0x8]
  00143bb8: ldr r0,[r5,#0x0]
  00143bba: blx 0x00071878
  00143bbe: strb.w r0,[r6,#-0x24]
  00143bc2: movs r0,#0x14
  00143bc4: blx 0x0006eb24
  00143bc8: mov r5,r0
  00143bca: ldr r0,[0x00143f54]
  00143bcc: add r0,pc
  00143bce: ldr r0,[r0,#0x0]
  00143bd0: str r0,[sp,#0x2c]
  00143bd2: ldr r1,[r0,#0x0]
  00143bd4: mov r0,r5
  00143bd6: blx 0x00076990
  00143bda: str.w r4,[r8,#0x44]
  00143bde: mov.w r1,#0xffffffff
  00143be2: str.w r5,[r8,#0x2c]
  00143be6: ldrb.w r0,[r8,#0x28]
  00143bea: str.w r1,[r8,#0x4c]
  00143bee: cmp r0,#0x0
  00143bf0: beq.w 0x00143df0
  00143bf4: movs r0,#0xc
  00143bf6: blx 0x0006eb24
  00143bfa: mov r5,r0
  00143bfc: blx 0x000717b8
  00143c00: ldr r4,[sp,#0x2c]
  00143c02: movs r0,#0x2
  00143c04: mov r1,r5
  00143c06: str.w r5,[r8,#0x1c]
  00143c0a: blx 0x00073504
  00143c0e: movs r0,#0xc0
  00143c10: blx 0x0006eb24
  00143c14: mov r5,r0
  00143c16: ldr r2,[r4,#0x0]
  00143c18: movw r1,#0x1a70
  00143c1c: movs r3,#0x0
  00143c1e: blx 0x0007207c
  00143c22: ldr.w r0,[r8,#0x1c]
  00143c26: ldr r1,[0x00143f58]
  00143c28: ldr r0,[r0,#0x4]
  00143c2a: mov r10,r1
  00143c2c: str r5,[r0,#0x0]
  00143c2e: ldr.w r0,[r8,#0x1c]
  00143c32: ldr r0,[r0,#0x4]
  00143c34: ldr r0,[r0,#0x0]
  00143c36: strd r1,r1,[sp,#0x3c]
  00143c3a: str r1,[sp,#0x44]
  00143c3c: add r1,sp,#0x3c
  00143c3e: blx 0x00073de0
  00143c42: ldr.w r0,[r8,#0x1c]
  00143c46: ldr r1,[0x00143f5c]
  00143c48: ldr r0,[r0,#0x4]
  00143c4a: ldr r0,[r0,#0x0]
  00143c4c: blx 0x000725c8
  00143c50: ldr.w r0,[r8,#0x1c]
  00143c54: add r5,sp,#0x3c
  00143c56: ldr r0,[r0,#0x4]
  00143c58: ldr r1,[r0,#0x0]
  00143c5a: mov r0,r5
  00143c5c: blx 0x00072574
  00143c60: mov r0,r6
  00143c62: mov r1,r5
  00143c64: blx 0x0006eb3c
  00143c68: vldr.32 s0,[r8,#0x30]
  00143c6c: movs r0,#0xc0
  00143c6e: vldr.32 s2,[r8,#0x34]
  00143c72: vldr.32 s4,[r8,#0x38]
  00143c76: vneg.f32 s0,s0
  00143c7a: vneg.f32 s2,s2
  00143c7e: vneg.f32 s4,s4
  00143c82: vstr.32 s0,[r8,#0x30]
  00143c86: vstr.32 s2,[r8,#0x34]
  00143c8a: vstr.32 s4,[r8,#0x38]
  00143c8e: blx 0x0006eb24
  00143c92: mov r5,r0
  00143c94: ldr r2,[r4,#0x0]
  00143c96: movw r1,#0x1a70
  00143c9a: movs r3,#0x0
  00143c9c: movs r4,#0x0
  00143c9e: blx 0x0007207c
  00143ca2: ldr r0,[0x00143f60]
  00143ca4: ldr.w r1,[r8,#0x1c]
  00143ca8: add r0,pc
  00143caa: ldr r1,[r1,#0x4]
  00143cac: ldr r0,[r0,#0x0]
  00143cae: str r5,[r1,#0x4]
  00143cb0: movw r1,#0x4e20
  00143cb4: ldr r0,[r0,#0x0]
  00143cb6: movw r5,#0x4e20
  00143cba: blx 0x00071848
  00143cbe: add r0,r5
  00143cc0: vldr.32 s2,[pc,#0x2a0]
  00143cc4: add r1,sp,#0x3c
  00143cc6: vmov s0,r0
  00143cca: vcvt.f32.s32 s0,s0
  00143cce: ldr.w r0,[r8,#0x1c]
  00143cd2: ldr r0,[r0,#0x4]
  00143cd4: ldr r0,[r0,#0x4]
  00143cd6: vmul.f32 s0,s0,s2
  00143cda: vstr.32 s0,[sp,#0x40]
  00143cde: vstr.32 s0,[sp,#0x3c]
  00143ce2: vstr.32 s0,[sp,#0x44]
  00143ce6: blx 0x00073de0
  00143cea: ldr.w r0,[r8,#0x1c]
  00143cee: ldr r1,[0x00143f68]
  00143cf0: ldr r0,[r0,#0x4]
  00143cf2: ldr r0,[r0,#0x4]
  00143cf4: strd r4,r1,[sp,#0x3c]
  00143cf8: add r1,sp,#0x3c
  00143cfa: str r4,[sp,#0x44]
  00143cfc: blx 0x00073dd4
  00143d00: ldr.w r0,[r8,#0x1c]
  00143d04: ldr r1,[0x00143f5c]
  00143d06: ldr r0,[r0,#0x4]
  00143d08: ldr r0,[r0,#0x4]
  00143d0a: blx 0x000725c8
  00143d0e: movs r0,#0xc
  00143d10: blx 0x0006eb24
  00143d14: mov r5,r0
  00143d16: blx 0x0006f8ec
  00143d1a: movs r0,#0x2
  00143d1c: mov r1,r5
  00143d1e: str.w r5,[r8,#0x14]
  00143d22: blx 0x0007699c
  00143d26: ldr.w r0,[r8,#0x14]
  00143d2a: movw r1,#0x2739
  00143d2e: ldr r4,[sp,#0x2c]
  00143d30: movs r3,#0x0
  00143d32: ldr r2,[r0,#0x4]
  00143d34: ldr r0,[r4,#0x0]
  00143d36: blx 0x0006fbb0
  00143d3a: ldr.w r1,[r8,#0x14]
  00143d3e: movs r3,#0x0
  00143d40: ldr r0,[r4,#0x0]
  00143d42: ldr r1,[r1,#0x4]
  00143d44: adds r2,r1,#0x4
  00143d46: movw r1,#0x2719
  00143d4a: blx 0x0006fbb0
  00143d4e: movs r0,#0xc
  00143d50: blx 0x0006eb24
  00143d54: mov r5,r0
  00143d56: blx 0x000769a8
  00143d5a: movs r0,#0x2
  00143d5c: mov r1,r5
  00143d5e: str.w r5,[r8,#0x20]
  00143d62: blx 0x000769b4
  00143d66: ldr.w r0,[r8,#0x1c]
  00143d6a: add r5,sp,#0x3c
  00143d6c: ldr r0,[r0,#0x4]
  00143d6e: ldr r1,[r0,#0x0]
  00143d70: mov r0,r5
  00143d72: blx 0x000720b8
  00143d76: ldr.w r0,[r8,#0x20]
  00143d7a: mov r1,r5
  00143d7c: ldr r0,[r0,#0x4]
  00143d7e: blx 0x0006eb3c
  00143d82: ldr.w r0,[r8,#0x1c]
  00143d86: add r5,sp,#0x3c
  00143d88: ldr r0,[r0,#0x4]
  00143d8a: ldr r1,[r0,#0x4]
  00143d8c: mov r0,r5
  00143d8e: blx 0x000720b8
  00143d92: ldr.w r0,[r8,#0x20]
  00143d96: mov r1,r5
  00143d98: ldr r0,[r0,#0x4]
  00143d9a: adds r0,#0xc
  00143d9c: blx 0x0006eb3c
  00143da0: movs r0,#0xc0
  00143da2: blx 0x0006eb24
  00143da6: mov r5,r0
  00143da8: ldr r2,[r4,#0x0]
  00143daa: movw r1,#0x1a70
  00143dae: movs r3,#0x0
  00143db0: blx 0x0007207c
  00143db4: ldr r1,[0x00143f6c]
  00143db6: add r2,sp,#0x3c
  00143db8: ldr r0,[0x00143f70]
  00143dba: str.w r5,[r8,#0x40]
  00143dbe: stm r2,{r0,r1,r10}
  00143dc2: add r1,sp,#0x3c
  00143dc4: mov r0,r5
  00143dc6: blx 0x00073de0
  00143dca: mov r0,r8
  00143dcc: blx 0x000769c0
  00143dd0: ldr r0,[sp,#0x9c]
  00143dd2: ldr.w r1,[r9,#0x0]
  00143dd6: subs r0,r1,r0
  00143dd8: itttt eq
  00143dda: mov.eq r0,r8
  00143ddc: add.eq sp,#0xa0
  00143dde: vpop.eq {d8,d9,d10,d11,d12,d13}
  00143de2: add.eq sp,#0x4
  00143de4: itt eq
  00143de6: pop.eq.w {r8,r9,r10,r11}
  00143dea: pop.eq {r4,r5,r6,r7,pc}
  00143dec: blx 0x0006e824
  00143df0: ldr r4,[sp,#0x24]
  00143df2: ldr r0,[r4,#0x0]
  00143df4: blx 0x00071a10
  00143df8: blx 0x00071a88
  00143dfc: str r0,[sp,#0x28]
  00143dfe: movs r0,#0x1
  00143e00: blx 0x0006eb24
  00143e04: mov r5,r0
  00143e06: blx 0x00072124
  00143e0a: ldr.w r11,[sp,#0x2c]
  00143e0e: ldr r0,[r4,#0x0]
  00143e10: blx 0x00071a10
  00143e14: mov r1,r0
  00143e16: mov r0,r5
  00143e18: blx 0x00073690
  00143e1c: mov r10,r0
  00143e1e: mov r0,r5
  00143e20: blx 0x0007213c
  00143e24: blx 0x0006eb48
  00143e28: movs r0,#0xc
  00143e2a: blx 0x0006eb24
  00143e2e: mov r5,r0
  00143e30: blx 0x0006f8ec
  00143e34: str.w r5,[r8,#0x14]
  00143e38: mov r1,r5
  00143e3a: ldr.w r0,[r10,#0x0]
  00143e3e: adds r0,#0x1
  00143e40: blx 0x0007699c
  00143e44: movs r0,#0xc
  00143e46: blx 0x0006eb24
  00143e4a: mov r5,r0
  00143e4c: blx 0x000701f8
  00143e50: ldr r0,[sp,#0x28]
  00143e52: mov r1,r5
  00143e54: str.w r5,[r8,#0x24]
  00143e58: ldr r0,[r0,#0x0]
  00143e5a: blx 0x00071a4c
  00143e5e: ldr r0,[sp,#0x24]
  00143e60: ldr r0,[r0,#0x0]
  00143e62: blx 0x00071770
  00143e66: cmp r0,#0x59
  00143e68: str.w r9,[sp,#0x8]
  00143e6c: str r6,[sp,#0x14]
  00143e6e: bne 0x00143e78
  00143e70: ldrb.w r0,[r8,#0xc]
  00143e74: cbnz r0,0x00143e8c
  00143e76: b 0x00143ea8
  00143e78: cmp r0,#0x9e
  00143e7a: blt 0x00143ea8
  00143e7c: ldr r0,[sp,#0x24]
  00143e7e: ldr r0,[r0,#0x0]
  00143e80: blx 0x00071a10
  00143e84: blx 0x00071a7c
  00143e88: cmp r0,#0x1b
  00143e8a: bne 0x00143ea8
  00143e8c: ldr.w r0,[r8,#0x14]
  00143e90: movw r1,#0x2dde
  00143e94: movs r3,#0x0
  00143e96: ldr r2,[r0,#0x4]
  00143e98: ldr.w r0,[r11,#0x0]
  00143e9c: blx 0x0006fbb0
  00143ea0: ldr.w r9,[sp,#0x24]
  00143ea4: movs r0,#0x3
  00143ea6: b 0x00143ee6
  00143ea8: ldr.w r9,[sp,#0x24]
  00143eac: ldr.w r5,[r11,#0x0]
  00143eb0: ldr.w r0,[r9,#0x0]
  00143eb4: blx 0x00071a10
  00143eb8: blx 0x0007390c
  00143ebc: ldr r1,[0x00143f74]
  00143ebe: movs r3,#0x0
  00143ec0: ldr.w r2,[r8,#0x14]
  00143ec4: add r1,pc
  00143ec6: ldrh.w r1,[r1,r0,lsl #0x2]
  00143eca: mov r0,r5
  00143ecc: ldr r2,[r2,#0x4]
  00143ece: blx 0x0006fbb0
  00143ed2: ldr.w r0,[r9,#0x0]
  00143ed6: blx 0x00071a10
  00143eda: blx 0x00071a7c
  00143ede: ldr r1,[0x00143f78]
  00143ee0: add r1,pc
  00143ee2: ldr.w r0,[r1,r0,lsl #0x2]
  00143ee6: str.w r0,[r8,#0x3c]
  00143eea: add.w r0,r8,#0x48
  00143eee: str r0,[sp,#0x18]
  00143ef0: movs r4,#0x1
  00143ef2: ldr r0,[0x00143f7c]
  00143ef4: movs r6,#0x0
  00143ef6: add r0,pc
  00143ef8: str r0,[sp,#0x1c]
  00143efa: ldr r0,[0x00143f80]
  00143efc: add r0,pc
  00143efe: mov r11,r0
  00143f00: b 0x0014402a
  00143f02: ldr r0,[sp,#0x28]
  00143f04: ldr r0,[r0,#0x4]
  00143f06: ldr r5,[r0,r6]
  00143f08: ldr.w r0,[r9,#0x0]
  00143f0c: blx 0x00071c14
  00143f10: blx 0x00071824
  00143f14: cmp r5,r0
  00143f16: bne 0x00143f84
  00143f18: ldr.w r0,[r9,#0x0]
  00143f1c: blx 0x00071770
  00143f20: ldr r1,[sp,#0x20]
  00143f22: cmp r1,#0x3
  00143f24: it eq
  00143f26: cmp.eq r0,#0x0
  00143f28: beq 0x00143ff8
  00143f2a: ldr.w r0,[r10,#0x4]
  00143f2e: ldr r1,[sp,#0x2c]
  00143f30: ldr r0,[r0,r6]
  00143f32: ldr r5,[r1,#0x0]
  00143f34: blx 0x000736b4
  00143f38: ldr.w r1,[r8,#0x14]
  00143f3c: ldr r2,[r1,#0x4]
  00143f3e: ldr r1,[sp,#0x1c]
  00143f40: ldrh.w r1,[r1,r0,lsl #0x2]
  00143f44: adds r0,r2,r6
  00143f46: adds r2,r0,#0x4
  00143f48: mov r0,r5
  00143f4a: b 0x0014400e
  00143f84: ldr.w r0,[r10,#0x4]
  00143f88: ldr r1,[sp,#0x2c]
  00143f8a: ldr r0,[r0,r6]
  00143f8c: ldr r5,[r1,#0x0]
  00143f8e: blx 0x000736b4
  00143f92: ldr.w r1,[r8,#0x14]
  00143f96: movs r3,#0x0
  00143f98: ldr r2,[r1,#0x4]
  00143f9a: ldrh.w r1,[r11,r0,lsl #0x2]
  00143f9e: adds r0,r2,r6
  00143fa0: adds r2,r0,#0x4
  00143fa2: mov r0,r5
  00143fa4: blx 0x0006fbb0
  00143fa8: ldr.w r0,[r10,#0x4]
  00143fac: ldr.w r5,[r9,#0x0]
  00143fb0: ldr r0,[r0,r6]
  00143fb2: blx 0x00071824
  00143fb6: mov r1,r0
  00143fb8: mov r0,r5
  00143fba: blx 0x0007459c
  00143fbe: cbz r0,0x00144014
  00143fc0: movs r0,#0xc0
  00143fc2: blx 0x0006eb24
  00143fc6: ldr.w r9,[sp,#0x2c]
  00143fca: mov r5,r0
  00143fcc: ldr.w r2,[r9,#0x0]
  00143fd0: movw r1,#0x1a70
  00143fd4: movs r3,#0x0
  00143fd6: blx 0x0007207c
  00143fda: str.w r5,[r8,#0x44]
  00143fde: movw r1,#0x7198
  00143fe2: ldr.w r0,[r9,#0x0]
  00143fe6: movs r3,#0x0
  00143fe8: ldr r2,[sp,#0x18]
  00143fea: blx 0x0006fbb0
  00143fee: ldr.w r9,[sp,#0x24]
  00143ff2: str.w r4,[r8,#0x4c]
  00143ff6: b 0x00144014
  00143ff8: ldr.w r0,[r8,#0x14]
  00143ffc: str.w r4,[r8,#0x50]
  00144000: ldr r0,[r0,#0x4]
  00144002: adds r1,r0,r6
  00144004: ldr r0,[sp,#0x2c]
  00144006: adds r2,r1,#0x4
  00144008: movw r1,#0x273b
  0014400c: ldr r0,[r0,#0x0]
  0014400e: movs r3,#0x0
  00144010: blx 0x0006fbb0
  00144014: ldr.w r0,[r10,#0x4]
  00144018: ldr r0,[r0,r6]
  0014401a: blx 0x00071824
  0014401e: ldr.w r1,[r8,#0x24]
  00144022: adds r4,#0x1
  00144024: ldr r1,[r1,#0x4]
  00144026: str r0,[r1,r6]
  00144028: adds r6,#0x4
  0014402a: ldr.w r0,[r8,#0x14]
  0014402e: ldr r0,[r0,#0x0]
  00144030: cmp r4,r0
  00144032: bcc.w 0x00143f02
  00144036: mov r0,r10
  00144038: blx 0x000735dc
  0014403c: mov r0,r10
  0014403e: blx 0x000735e8
  00144042: blx 0x0006eb48
  00144046: movs r0,#0xc
  00144048: blx 0x0006eb24
  0014404c: mov r5,r0
  0014404e: blx 0x00073d08
  00144052: ldr r6,[sp,#0x28]
  00144054: mov r1,r5
  00144056: str.w r5,[r8,#0x18]
  0014405a: ldr r0,[r6,#0x0]
  0014405c: blx 0x00073d14
  00144060: movs r0,#0xc
  00144062: blx 0x0006eb24
  00144066: mov r5,r0
  00144068: blx 0x000717b8
  0014406c: ldr r4,[sp,#0x2c]
  0014406e: str.w r5,[r8,#0x1c]
  00144072: mov r1,r5
  00144074: ldr r0,[r6,#0x0]
  00144076: adds r0,#0x1
  00144078: blx 0x00073504
  0014407c: movs r0,#0xc
  0014407e: blx 0x0006eb24
  00144082: mov r5,r0
  00144084: blx 0x000769a8
  00144088: str.w r5,[r8,#0x20]
  0014408c: mov r1,r5
  0014408e: ldr r0,[r6,#0x0]
  00144090: adds r0,#0x1
  00144092: blx 0x000769b4
  00144096: ldr r0,[0x00144404]
  00144098: add r0,pc
  0014409a: ldr r0,[r0,#0x0]
  0014409c: str r0,[sp,#0x1c]
  0014409e: ldr r5,[r0,#0x0]
  001440a0: ldr.w r0,[r9,#0x0]
  001440a4: blx 0x00071c14
  001440a8: blx 0x00071824
  001440ac: mov.w r1,#0x12c
  001440b0: mul r2,r0,r1
  001440b4: mov r0,r5
  001440b6: asrs r3,r2,#0x1f
  001440b8: blx 0x000739c0
  001440bc: add r2,sp,#0x3c
  001440be: movs r0,#0x0
  001440c0: movs r1,#0x0
  001440c2: b 0x001440ca
  001440c4: str.w r0,[r2,r1,lsl #0x2]
  001440c8: adds r1,#0x1
  001440ca: cmp r1,#0x18
  001440cc: bne 0x001440c4
  001440ce: vmov.f32 s20,0x3f000000
  001440d2: add.w r0,r8,#0x10
  001440d6: str r0,[sp,#0xc]
  001440d8: mov.w r11,#0x0
  001440dc: ldr r0,[0x00144408]
  001440de: mov.w r10,#0x0
  001440e2: ldr r5,[0x0014440c]
  001440e4: mov.w r9,#0x0
  001440e8: add r0,pc
  001440ea: vldr.32 s16,[pc,#-0x188]
  001440ee: vldr.32 s18,[pc,#0x320]
  001440f2: vldr.64 d11,[pc,#0x324]
  001440f6: vldr.64 d12,[pc,#0x328]
  001440fa: vldr.64 d13,[pc,#0x32c]
  001440fe: ldr r0,[r0,#0x0]
  00144100: str r0,[sp,#0x18]
  00144102: ldr.w r0,[r8,#0x1c]
  00144106: ldr r0,[r0,#0x0]
  00144108: cmp r9,r0
  0014410a: bcs.w 0x0014464a
  0014410e: movs r0,#0xc0
  00144110: blx 0x0006eb24
  00144114: mov r6,r0
  00144116: ldr r2,[r4,#0x0]
  00144118: movw r1,#0x1a70
  0014411c: movs r3,#0x0
  0014411e: blx 0x0007207c
  00144122: ldr.w r0,[r8,#0x1c]
  00144126: ldr r0,[r0,#0x4]
  00144128: str.w r6,[r0,r9,lsl #0x2]
  0014412c: ldrb.w r0,[r8,#0x28]
  00144130: cbnz r0,0x00144166
  00144132: ldr r0,[sp,#0x24]
  00144134: ldr r0,[r0,#0x0]
  00144136: blx 0x00071a10
  0014413a: blx 0x0007390c
  0014413e: cmp r0,#0xf
  00144140: blt 0x00144166
  00144142: ldr r0,[sp,#0x18]
  00144144: ldr r0,[r0,#0x0]
  00144146: blx 0x0007258c
  0014414a: blx 0x0006ee78
  0014414e: cbz r0,0x00144166
  00144150: ldr.w r0,[r8,#0x1c]
  00144154: ldr r0,[r0,#0x4]
  00144156: ldr.w r1,[r0,r9,lsl #0x2]
  0014415a: ldr r0,[r4,#0x0]
  0014415c: ldr r1,[r1,#0x1c]
  0014415e: blx 0x00072370
  00144162: ldr r1,[0x00144430]
  00144164: str r1,[r0,#0x1c]
  00144166: cmp.w r9,#0x0
  0014416a: beq 0x00144206
  0014416c: ldr r0,[sp,#0x28]
  0014416e: sub.w r1,r9,#0x1
  00144172: str r1,[sp,#0x20]
  00144174: ldr r0,[r0,#0x4]
  00144176: ldr.w r4,[r0,r1,lsl #0x2]
  0014417a: ldr r0,[sp,#0x24]
  0014417c: ldr r0,[r0,#0x0]
  0014417e: blx 0x00071c14
  00144182: blx 0x00071824
  00144186: mov r5,r0
  00144188: mov.w r0,#0x130
  0014418c: blx 0x0006eb24
  00144190: mov r6,r0
  00144192: ldr.w r0,[r8,#0x1c]
  00144196: cmp r4,r5
  00144198: ldr r0,[r0,#0x4]
  0014419a: ldr.w r2,[r0,r9,lsl #0x2]
  0014419e: bne 0x0014423a
  001441a0: movs r0,#0x0
  001441a2: movs r1,#0x0
  001441a4: strd r0,r0,[sp,#0x0]
  001441a8: mov r0,r6
  001441aa: movs r3,#0x0
  001441ac: blx 0x000740ec
  001441b0: ldr.w r0,[r8,#0x18]
  001441b4: movs r2,#0x1
  001441b6: ldr r1,[sp,#0x20]
  001441b8: ldr r0,[r0,#0x4]
  001441ba: str.w r6,[r0,r1,lsl #0x2]
  001441be: mov r5,r11
  001441c0: lsls r0,r2,#0x1f
  001441c2: mov.w r11,#0x0
  001441c6: mov.w r2,#0x0
  001441ca: bne 0x001441be
  001441cc: ldr r0,[0x00144434]
  001441ce: mov.w r4,#0x8000
  001441d2: cmp.w r10,#0xc
  001441d6: it lt
  001441d8: mov.lt r4,#0x0
  001441da: add r0,pc
  001441dc: ldr r2,[sp,#0x20]
  001441de: ldr r1,[r0,#0x0]
  001441e0: ldr r0,[sp,#0x1c]
  001441e2: str r2,[r1,#0x0]
  001441e4: movw r1,#0x4e20
  001441e8: ldr r0,[r0,#0x0]
  001441ea: blx 0x00071848
  001441ee: ldr r1,[sp,#0x24]
  001441f0: movw r2,#0x4e20
  001441f4: adds r6,r0,r2
  001441f6: ldr r1,[r1,#0x0]
  001441f8: mov r0,r1
  001441fa: blx 0x00071770
  001441fe: cmp r0,#0x0
  00144200: beq 0x001442de
  00144202: str r6,[sp,#0x10]
  00144204: b 0x001442f4
  00144206: ldr r0,[sp,#0x1c]
  00144208: movs r1,#0xe
  0014420a: ldr r0,[r0,#0x0]
  0014420c: blx 0x00071848
  00144210: mov r6,r0
  00144212: ldr r0,[sp,#0x24]
  00144214: ldr r0,[r0,#0x0]
  00144216: blx 0x00071c14
  0014421a: blx 0x000736b4
  0014421e: subs r0,#0x9
  00144220: cmp r0,#0xe
  00144222: bcs 0x00144234
  00144224: uxth r1,r0
  00144226: movw r2,#0x3a31
  0014422a: lsr.w r1,r2,r1
  0014422e: lsls r1,r1,#0x1f
  00144230: bne.w 0x00144438
  00144234: add.w r11,r6,#0x5
  00144238: b 0x00144440
  0014423a: movs r0,#0x0
  0014423c: movs r1,#0x0
  0014423e: strd r0,r0,[sp,#0x0]
  00144242: mov r0,r6
  00144244: movs r3,#0x0
  00144246: blx 0x000740ec
  0014424a: ldr r4,[sp,#0x1c]
  0014424c: ldr.w r0,[r8,#0x18]
  00144250: movs r2,#0x0
  00144252: ldr r1,[sp,#0x20]
  00144254: ldr r0,[r0,#0x4]
  00144256: str.w r6,[r0,r1,lsl #0x2]
  0014425a: add r6,sp,#0x3c
  0014425c: b 0x00144282
  0014425e: ldr r0,[r4,#0x0]
  00144260: movs r1,#0xb
  00144262: blx 0x00071848
  00144266: add.w r11,r0,#0x7
  0014426a: movs r2,#0x0
  0014426c: subs.w r0,r11,r10
  00144270: it mi
  00144272: rsb.mi r0,r0
  00144274: cmp r0,#0x3
  00144276: blt 0x00144282
  00144278: ldr.w r0,[r6,r11,lsl #0x2]
  0014427c: clz r0,r0
  00144280: lsrs r2,r0,#0x5
  00144282: lsls r0,r2,#0x1f
  00144284: beq 0x0014425e
  00144286: ldr r0,[r4,#0x0]
  00144288: movs r1,#0x28
  0014428a: blx 0x00071848
  0014428e: vmov s0,r0
  00144292: add r1,sp,#0x30
  00144294: vmov d17,d12
  00144298: vcvt.f64.s32 d16,s0
  0014429c: ldr.w r0,[r8,#0x1c]
  001442a0: ldr r0,[r0,#0x4]
  001442a2: ldr.w r0,[r0,r9,lsl #0x2]
  001442a6: vmla.f64 d17,d16,d11
  001442aa: vmul.f64 d16,d17,d13
  001442ae: vcvt.f32.f64 s0,d16
  001442b2: vstr.32 s0,[sp,#0x34]
  001442b6: vstr.32 s0,[sp,#0x30]
  001442ba: vstr.32 s0,[sp,#0x38]
  001442be: blx 0x00073de0
  001442c2: sub.w r0,r10,r11
  001442c6: mov.w r4,#0x8000
  001442ca: cmp r0,#0xc
  001442cc: it lt
  001442ce: mov.lt r4,#0x0
  001442d0: mov.w r0,#0x8000
  001442d4: cmp r10,r11
  001442d6: it le
  001442d8: mov.le r4,r0
  001442da: mov r5,r11
  001442dc: b 0x0014436a
  001442de: vmov s0,r6
  001442e2: vcvt.f32.s32 s0,s0
  001442e6: vmul.f32 s0,s0,s20
  001442ea: vcvt.s32.f32 s0,s0
  001442ee: vmov r0,s0
  001442f2: str r0,[sp,#0x10]
  001442f4: ldr.w r11,[sp,#0x24]
  001442f8: ldr.w r0,[r11,#0x0]
  001442fc: blx 0x00071c14
  00144300: blx 0x000736b4
  00144304: mov r6,r0
  00144306: ldr.w r0,[r11,#0x0]
  0014430a: blx 0x00073cd8
  0014430e: cbz r0,0x00144316
  00144310: movw r1,#0x6590
  00144314: b 0x0014433c
  00144316: cmp r6,#0x11
  00144318: bhi.w 0x00144644
  0014431c: ldr r1,[0x001446a4]
  0014431e: movs r0,#0x1
  00144320: lsls r0,r6
  00144322: tst r0,r1
  00144324: ldr r1,[sp,#0x10]
  00144326: beq.w 0x00144626
  0014432a: ldr r0,[sp,#0x1c]
  0014432c: movw r1,#0x3a98
  00144330: ldr r0,[r0,#0x0]
  00144332: blx 0x00071848
  00144336: movw r1,#0x88b8
  0014433a: add r1,r0
  0014433c: add r6,sp,#0x3c
  0014433e: vmov s0,r1
  00144342: add r1,sp,#0x30
  00144344: vcvt.f32.s32 s0,s0
  00144348: ldr.w r0,[r8,#0x1c]
  0014434c: vmul.f32 s0,s0,s16
  00144350: vstr.32 s0,[r8,#0x58]
  00144354: ldr r0,[r0,#0x4]
  00144356: ldr.w r0,[r0,r9,lsl #0x2]
  0014435a: vstr.32 s0,[sp,#0x34]
  0014435e: vstr.32 s0,[sp,#0x30]
  00144362: vstr.32 s0,[sp,#0x38]
  00144366: blx 0x00073de0
  0014436a: movs r0,#0x1
  0014436c: cmp r5,r10
  0014436e: str.w r0,[r6,r5,lsl #0x2]
  00144372: bne 0x001443ac
  00144374: movw r0,#0xaaa
  00144378: add r6,sp,#0x30
  0014437a: mul r0,r10,r0
  0014437e: movs r1,#0x0
  00144380: vmov s0,r0
  00144384: vcvt.f32.s32 s0,s0
  00144388: ldr.w r0,[r8,#0x1c]
  0014438c: ldr r0,[r0,#0x4]
  0014438e: ldr.w r0,[r0,r9,lsl #0x2]
  00144392: vmul.f32 s0,s0,s16
  00144396: str r1,[sp,#0x30]
  00144398: str r1,[sp,#0x38]
  0014439a: mov r1,r6
  0014439c: vmul.f32 s0,s0,s18
  001443a0: vstr.32 s0,[sp,#0x34]
  001443a4: blx 0x00073dd4
  001443a8: mov r11,r10
  001443aa: b 0x00144578
  001443ac: ldr r0,[sp,#0x28]
  001443ae: ldr r1,[sp,#0x20]
  001443b0: ldr r0,[r0,#0x4]
  001443b2: ldr.w r6,[r0,r1,lsl #0x2]
  001443b6: ldr r0,[sp,#0x24]
  001443b8: ldr r0,[r0,#0x0]
  001443ba: blx 0x00071c14
  001443be: blx 0x00071824
  001443c2: cmp r6,r0
  001443c4: bne 0x001443fc
  001443c6: movw r0,#0xaaa
  001443ca: add r6,sp,#0x30
  001443cc: muls r0,r5
  001443ce: movs r1,#0x0
  001443d0: vmov s0,r0
  001443d4: vcvt.f32.s32 s0,s0
  001443d8: ldr.w r0,[r8,#0x1c]
  001443dc: ldr r0,[r0,#0x4]
  001443de: ldr.w r0,[r0,r9,lsl #0x2]
  001443e2: vmul.f32 s0,s0,s16
  001443e6: str r1,[sp,#0x30]
  001443e8: str r1,[sp,#0x38]
  001443ea: mov r1,r6
  001443ec: vmul.f32 s0,s0,s18
  001443f0: vstr.32 s0,[sp,#0x34]
  001443f4: blx 0x00073dd4
  001443f8: mov r11,r5
  001443fa: b 0x00144578
  001443fc: add r6,sp,#0x30
  001443fe: mov r11,r5
  00144400: b 0x00144522
  00144438: ldr r1,[0x001446b8]
  0014443a: add r1,pc
  0014443c: ldr.w r11,[r1,r0,lsl #0x2]
  00144440: ldrb.w r0,[r8,#0xc]
  00144444: add r6,sp,#0x30
  00144446: cmp r0,#0x0
  00144448: beq 0x001444ce
  0014444a: ldr r0,[sp,#0x24]
  0014444c: ldr r0,[r0,#0x0]
  0014444e: blx 0x00071770
  00144452: mov r1,r0
  00144454: ldr.w r0,[r8,#0x1c]
  00144458: cmp r1,#0x6a
  0014445a: ldr r0,[r0,#0x4]
  0014445c: ldr r0,[r0,#0x0]
  0014445e: ite lt
  00144460: ldr.lt r1,[0x001446a8]
  00144462: ldr.ge r1,[0x001446ac]
  00144464: str r1,[sp,#0x34]
  00144466: str r1,[sp,#0x30]
  00144468: str r1,[sp,#0x38]
  0014446a: mov r1,r6
  0014446c: blx 0x00073de0
  00144470: movs r0,#0xc0
  00144472: blx 0x0006eb24
  00144476: mov r6,r0
  00144478: ldr r2,[r4,#0x0]
  0014447a: movw r1,#0x1a70
  0014447e: movs r3,#0x0
  00144480: blx 0x0007207c
  00144484: ldr r0,[0x001446b4]
  00144486: str.w r6,[r8,#0x40]
  0014448a: str r0,[sp,#0x34]
  0014448c: ldr r0,[0x001446b0]
  0014448e: str r0,[sp,#0x30]
  00144490: ldr r0,[0x001446a8]
  00144492: str r0,[sp,#0x38]
  00144494: mov r0,r6
  00144496: add r6,sp,#0x30
  00144498: mov r1,r6
  0014449a: blx 0x00073de0
  0014449e: ldr r0,[sp,#0x18]
  001444a0: ldr r0,[r0,#0x0]
  001444a2: blx 0x0007258c
  001444a6: blx 0x0006ee78
  001444aa: cbz r0,0x001444bc
  001444ac: ldr.w r0,[r8,#0x40]
  001444b0: ldr r1,[r0,#0x1c]
  001444b2: ldr r0,[r4,#0x0]
  001444b4: blx 0x00072370
  001444b8: ldr r1,[0x001446a0]
  001444ba: str r1,[r0,#0x1c]
  001444bc: ldr r0,[r4,#0x0]
  001444be: movw r1,#0x2dde
  001444c2: ldr r2,[sp,#0xc]
  001444c4: movs r3,#0x0
  001444c6: blx 0x0006fbb0
  001444ca: add r0,sp,#0x3c
  001444cc: b 0x00144516
  001444ce: ldr.w r0,[r8,#0x1c]
  001444d2: mov r1,r6
  001444d4: ldr r0,[r0,#0x4]
  001444d6: ldr r0,[r0,#0x0]
  001444d8: strd r5,r5,[sp,#0x30]
  001444dc: str r5,[sp,#0x38]
  001444de: blx 0x00073de0
  001444e2: movs r0,#0xc0
  001444e4: blx 0x0006eb24
  001444e8: mov r1,r4
  001444ea: mov r4,r5
  001444ec: mov r5,r6
  001444ee: mov r6,r0
  001444f0: ldr r2,[r1,#0x0]
  001444f2: movw r1,#0x1a70
  001444f6: movs r3,#0x0
  001444f8: blx 0x0007207c
  001444fc: ldr r0,[0x0014469c]
  001444fe: mov r1,r5
  00144500: str.w r6,[r8,#0x40]
  00144504: str r0,[sp,#0x34]
  00144506: ldr r0,[0x00144698]
  00144508: str r0,[sp,#0x30]
  0014450a: mov r0,r6
  0014450c: str r4,[sp,#0x38]
  0014450e: blx 0x00073de0
  00144512: add r0,sp,#0x3c
  00144514: mov r6,r5
  00144516: movs r1,#0x1
  00144518: mov.w r4,#0x8000
  0014451c: str.w r1,[r0,r11,lsl #0x2]
  00144520: mov r10,r11
  00144522: ldr.w r0,[r8,#0x1c]
  00144526: mov.w r1,#0x1000
  0014452a: ldr r0,[r0,#0x4]
  0014452c: ldr.w r5,[r0,r9,lsl #0x2]
  00144530: ldr r0,[sp,#0x1c]
  00144532: ldr r0,[r0,#0x0]
  00144534: blx 0x00071848
  00144538: movw r1,#0xaaa
  0014453c: sub.w r0,r0,#0x800
  00144540: mul r1,r11,r1
  00144544: vmov s2,r0
  00144548: movs r0,#0x0
  0014454a: vmov s0,r1
  0014454e: mov r1,r6
  00144550: vcvt.f32.s32 s0,s0
  00144554: vcvt.f32.s32 s2,s2
  00144558: str r0,[sp,#0x38]
  0014455a: mov r0,r5
  0014455c: vmul.f32 s0,s0,s16
  00144560: vmul.f32 s2,s2,s16
  00144564: vmul.f32 s0,s0,s18
  00144568: vmul.f32 s2,s2,s18
  0014456c: vstr.32 s0,[sp,#0x34]
  00144570: vstr.32 s2,[sp,#0x30]
  00144574: blx 0x00073dd4
  00144578: ldr.w r0,[r8,#0x1c]
  0014457c: ldr r1,[0x00144694]
  0014457e: ldr r0,[r0,#0x4]
  00144580: ldr.w r0,[r0,r9,lsl #0x2]
  00144584: blx 0x000725c8
  00144588: cmp.w r9,#0x0
  0014458c: beq 0x001445be
  0014458e: ldr r5,[0x00144690]
  00144590: cbz r4,0x001445fa
  00144592: vmov s0,r4
  00144596: movs r1,#0x0
  00144598: vcvt.f32.s32 s0,s0
  0014459c: ldr.w r0,[r8,#0x1c]
  001445a0: ldr r0,[r0,#0x4]
  001445a2: ldr.w r0,[r0,r9,lsl #0x2]
  001445a6: vmul.f32 s0,s0,s16
  001445aa: str r1,[sp,#0x30]
  001445ac: str r1,[sp,#0x38]
  001445ae: mov r1,r6
  001445b0: vmul.f32 s0,s0,s18
  001445b4: vstr.32 s0,[sp,#0x34]
  001445b8: blx 0x00072478
  001445bc: b 0x001445fa
  001445be: ldr.w r0,[r8,#0x1c]
  001445c2: ldr r0,[r0,#0x4]
  001445c4: ldr r1,[r0,#0x0]
  001445c6: mov r0,r6
  001445c8: blx 0x00072574
  001445cc: ldr r0,[sp,#0x14]
  001445ce: mov r1,r6
  001445d0: blx 0x0006eb3c
  001445d4: vldr.32 s0,[r8,#0x30]
  001445d8: vldr.32 s2,[r8,#0x34]
  001445dc: vldr.32 s4,[r8,#0x38]
  001445e0: vneg.f32 s0,s0
  001445e4: vneg.f32 s2,s2
  001445e8: ldr r5,[0x00144690]
  001445ea: vneg.f32 s4,s4
  001445ee: vstr.32 s0,[r8,#0x30]
  001445f2: vstr.32 s2,[r8,#0x34]
  001445f6: vstr.32 s4,[r8,#0x38]
  001445fa: ldr.w r0,[r8,#0x1c]
  001445fe: ldr r0,[r0,#0x4]
  00144600: ldr.w r1,[r0,r9,lsl #0x2]
  00144604: mov r0,r6
  00144606: blx 0x000720b8
  0014460a: ldr.w r0,[r8,#0x20]
  0014460e: add.w r1,r9,r9, lsl #0x1
  00144612: ldr r0,[r0,#0x4]
  00144614: add.w r0,r0,r1, lsl #0x2
  00144618: mov r1,r6
  0014461a: blx 0x0006eb3c
  0014461e: add.w r9,r9,#0x1
  00144622: ldr r4,[sp,#0x2c]
  00144624: b 0x00144102
  00144626: add r6,sp,#0x3c
  00144628: tst r0,#0x10200
  0014462c: beq.w 0x0014433e
  00144630: ldr r0,[sp,#0x1c]
  00144632: movw r1,#0x32c8
  00144636: ldr r0,[r0,#0x0]
  00144638: blx 0x00071848
  0014463c: movw r1,#0x7ef4
  00144640: add r1,r0
  00144642: b 0x0014433e
  00144644: add r6,sp,#0x3c
  00144646: ldr r1,[sp,#0x10]
  00144648: b 0x0014433e
  0014464a: ldr r0,[sp,#0x1c]
  0014464c: ldr r0,[r0,#0x0]
  0014464e: blx 0x000718cc
  00144652: ldr.w r9,[sp,#0x8]
  00144656: b.w 0x00143dca
```
