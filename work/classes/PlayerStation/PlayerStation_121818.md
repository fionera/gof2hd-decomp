# PlayerStation::PlayerStation
elf 0x121818 body 3098
Sig: undefined __thiscall PlayerStation(PlayerStation * this, Station * param_1)

## decompile
```c

/* PlayerStation::PlayerStation(Station*) */

void __thiscall PlayerStation::PlayerStation(PlayerStation *this,Station *param_1)

{
  undefined1 uVar1;
  int iVar2;
  FileRead *pFVar3;
  Array *this_00;
  void *pvVar4;
  int iVar5;
  AEGeometry *pAVar6;
  undefined4 uVar7;
  uint uVar8;
  Station *this_01;
  AEGeometry *this_02;
  AEGeometry *pAVar9;
  AEGeometry *pAVar10;
  Array<BoundingVolume*> *this_03;
  int iVar11;
  BoundingSphere *this_04;
  ushort uVar12;
  int iVar13;
  int iVar14;
  code *pcVar15;
  uint *puVar16;
  undefined4 *puVar17;
  code *pcVar18;
  undefined1 uVar19;
  PlayerStation *pPVar20;
  undefined1 uVar21;
  int *piVar22;
  int iVar23;
  uint in_fpscr;
  float in_s0;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
  float extraout_s0_06;
  float extraout_s0_07;
  float extraout_s0_08;
  float extraout_s0_09;
  float extraout_s0_10;
  float extraout_s0_11;
  float extraout_s0_12;
  float extraout_s0_13;
  float fVar24;
  float extraout_s0_14;
  float extraout_s0_15;
  float in_s1;
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
  float extraout_s1_12;
  float extraout_s1_13;
  float fVar25;
  float extraout_s1_14;
  float extraout_s1_15;
  float extraout_s1_16;
  float extraout_s1_17;
  float in_s2;
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
  float fVar26;
  float extraout_s2_14;
  float extraout_s2_15;
  float extraout_s2_16;
  float extraout_s3;
  float extraout_s3_00;
  float extraout_s4;
  float extraout_s5;
  float extraout_s5_00;
  float extraout_s6;
  float extraout_s6_00;
  float extraout_s7;
  float extraout_s8;
  undefined8 uVar27;
  float local_64;
  float local_60;
  float local_5c;
  uint local_58 [2];
  ushort uStack_4e;
  uint local_4c;
  uint local_48;
  int local_44;
  
  piVar22 = *(int **)(DAT_00131b94 + 0x131832);
  local_44 = *piVar22;
  PlayerStaticFar::PlayerStaticFar((PlayerStaticFar *)this,-1,(AEGeometry *)0x0,in_s0,in_s1,in_s2);
  *(int *)this = *(int *)(DAT_00131b98 + 0x131852) + 8;
  *(undefined4 *)(this + 0x158) = 0;
  *(undefined4 *)(this + 0x15c) = 0;
  *(undefined4 *)(this + 0x160) = 0;
  Player::setRadius(*(Player **)(this + 4),15000);
  iVar2 = DAT_00131b9c;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 0x130) = 0;
  this[0x25] = (PlayerStation)0x0;
  Player::setMaxHitpoints(*(Player **)(this + 4),iVar2);
  *(undefined4 *)(this + 0x140) = 0;
  *(undefined4 *)(this + 0x14c) = 0;
  *(undefined4 *)(this + 0x154) = 0;
  iVar2 = Station::getIndex(param_1);
  *(int *)(this + 0x148) = iVar2;
  pFVar3 = operator_new(1);
  FileRead::FileRead(pFVar3);
  this_00 = (Array *)FileRead::loadStationCollision((int)pFVar3);
  pvVar4 = (void *)FileRead::~FileRead(pFVar3);
  operator_delete(pvVar4);
  iVar5 = Status::inAlienOrbit();
  pPVar20 = this + 0x140;
  fVar24 = extraout_s0;
  fVar25 = extraout_s1;
  fVar26 = extraout_s2;
  if ((1 < iVar2 - 0x6dU) && (this_00 == (Array *)0x0)) {
    if (iVar5 == 0) {
      pAVar6 = operator_new(0xc0);
      puVar17 = *(undefined4 **)(DAT_00131ba8 + 0x131a12);
      AEGeometry::AEGeometry(pAVar6,0x4034,(PaintCanvas *)*puVar17,false);
      *(AEGeometry **)pPVar20 = pAVar6;
      pAVar6 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar6,0x4037,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pAVar9 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar9,0x403a,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar6);
      operator_delete(pvVar4);
      pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar9);
      operator_delete(pvVar4);
      pFVar3 = operator_new(1);
      FileRead::FileRead(pFVar3);
      this_00 = (Array *)FileRead::loadStationCollision((int)pFVar3);
    }
    else {
      iVar2 = Status::dlc1Won();
      if (iVar2 != 0) {
        pAVar6 = operator_new(0xc0);
        puVar16 = *(uint **)(DAT_00131ba4 + 0x13190a);
        AEGeometry::AEGeometry(pAVar6,0x4220,(PaintCanvas *)*puVar16,false);
        *(AEGeometry **)pPVar20 = pAVar6;
        local_64 = -NAN;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,(uint *)&local_64);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,SUB42(local_64,0),true);
        AEGeometry::addChild(*(uint *)pPVar20);
        local_58[1] = 0xffffffff;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,local_58 + 1);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,(ushort)local_58[1],true);
        AEGeometry::addChild(*(uint *)pPVar20);
        pFVar3 = operator_new(1);
        FileRead::FileRead(pFVar3);
        this_00 = (Array *)FileRead::loadStationCollision((int)pFVar3);
        pvVar4 = (void *)FileRead::~FileRead(pFVar3);
        operator_delete(pvVar4);
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        uVar27 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        AbyssEngine::Transform::Update
                  (CONCAT44((int)((ulonglong)uVar27 >> 0x20),uVar7),
                   SUB41(*(undefined4 *)((int)uVar27 + 0xf8),0));
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        uVar27 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        AbyssEngine::Transform::Update
                  (CONCAT44((int)((ulonglong)uVar27 >> 0x20),uVar7),
                   SUB41(*(undefined4 *)((int)uVar27 + 0xf8),0));
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        uVar27 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        AbyssEngine::Transform::Update
                  (CONCAT44((int)((ulonglong)uVar27 >> 0x20),uVar7),
                   SUB41(*(undefined4 *)((int)uVar27 + 0xf8),0));
        fVar24 = extraout_s0_00;
        fVar25 = extraout_s1_00;
        fVar26 = extraout_s2_00;
        goto LAB_00131b1e;
      }
      pAVar6 = operator_new(0xc0);
      puVar17 = *(undefined4 **)(DAT_00131bac + 0x131a9a);
      AEGeometry::AEGeometry(pAVar6,0x403b,(PaintCanvas *)*puVar17,false);
      *(AEGeometry **)pPVar20 = pAVar6;
      pAVar6 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar6,0x403e,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pAVar9 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar9,0x4041,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar6);
      operator_delete(pvVar4);
      pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar9);
      operator_delete(pvVar4);
      pFVar3 = operator_new(1);
      FileRead::FileRead(pFVar3);
      this_00 = (Array *)FileRead::loadStationCollision((int)pFVar3);
    }
    pvVar4 = (void *)FileRead::~FileRead(pFVar3);
    operator_delete(pvVar4);
    fVar24 = extraout_s0_01;
    fVar25 = extraout_s1_01;
    fVar26 = extraout_s2_01;
  }
LAB_00131b1e:
  if (*(int *)pPVar20 != 0) goto LAB_0013222e;
  iVar2 = *(int *)(this + 0x148);
  if (iVar2 - 0x6dU < 3) {
    pAVar6 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar6,0x5254,(PaintCanvas *)**(undefined4 **)(DAT_00131bb0 + 0x131b44),false);
  }
  else {
    pAVar6 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar6,(short)iVar2 + 21000,(PaintCanvas *)**(undefined4 **)(DAT_00131bb4 + 0x131b66)
               ,false);
  }
  *(AEGeometry **)pPVar20 = pAVar6;
  iVar2 = Status::getSystem();
  if (iVar2 == 0) {
    uVar8 = 9;
    fVar24 = extraout_s0_02;
    fVar25 = extraout_s1_02;
    fVar26 = extraout_s2_02;
  }
  else {
    Status::getSystem();
    uVar8 = SolarSystem::getRace();
    fVar24 = extraout_s0_03;
    fVar25 = extraout_s1_03;
    fVar26 = extraout_s2_03;
  }
  iVar2 = *(int *)(this + 0x148);
  if (iVar2 == 0x6f) {
    iVar2 = Status::getCurrentCampaignMission();
    if (iVar2 < 0x5e) {
      pAVar6 = operator_new(0xc0);
      puVar17 = *(undefined4 **)(DAT_00131ef8 + 0x131d0e);
      AEGeometry::AEGeometry(pAVar6,0x5254,(PaintCanvas *)*puVar17,false);
      *(AEGeometry **)pPVar20 = pAVar6;
      pAVar6 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar6,0x5574,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pAVar9 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar9,0x563c,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pAVar10 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar10,0x495d,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      this_02 = operator_new(0xc0);
      AEGeometry::AEGeometry(this_02,0x495e,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pcVar15 = *(code **)(DAT_00131efc + 0x131dac);
      (*pcVar15)(pAVar6);
      pcVar18 = *(code **)(DAT_00131f00 + 0x131db6);
      (*pcVar18)(pAVar6);
      (*pcVar15)(pAVar9);
      (*pcVar18)();
      (*pcVar15)(pAVar10);
      (*pcVar18)();
      (*pcVar15)(this_02);
      (*pcVar18)();
      fVar24 = extraout_s0_06;
      fVar25 = extraout_s1_06;
      fVar26 = extraout_s2_06;
      goto LAB_0013222e;
    }
    iVar2 = *(int *)(this + 0x148);
    fVar24 = extraout_s0_05;
    fVar25 = extraout_s1_05;
    fVar26 = extraout_s2_05;
  }
  else if (iVar2 == 0x65) {
    pAVar6 = operator_new(0xc0);
    puVar16 = *(uint **)(DAT_00131ef4 + 0x131bd8);
    AEGeometry::AEGeometry(pAVar6,0x4220,(PaintCanvas *)*puVar16,false);
    *(AEGeometry **)pPVar20 = pAVar6;
    local_64 = -NAN;
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,(uint *)&local_64);
    AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,SUB42(local_64,0),true);
    AEGeometry::addChild(*(uint *)pPVar20);
    local_58[1] = 0xffffffff;
    AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,local_58 + 1);
    AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,(ushort)local_58[1],true);
    AEGeometry::addChild(*(uint *)pPVar20);
    iVar2 = Status::getCurrentCampaignMission();
    if (iVar2 == 0x9d) {
      this_01 = (Station *)Status::getStation();
      iVar2 = Station::getIndex(this_01);
      if (iVar2 == 0x70) {
        local_58[0] = 0xffffffff;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,local_58);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,(ushort)local_58[0],true);
        AEGeometry::addChild(*(uint *)pPVar20);
        local_48 = 0xffffffff;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,&local_48);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,(ushort)local_48,true);
        AEGeometry::addChild(*(uint *)pPVar20);
        local_4c = 0xffffffff;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,&local_4c);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,(ushort)local_4c,true);
        AEGeometry::addChild(*(uint *)pPVar20);
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        uVar27 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        AbyssEngine::Transform::Update
                  (CONCAT44((int)((ulonglong)uVar27 >> 0x20),uVar7),
                   SUB41(*(undefined4 *)((int)uVar27 + 0xf8),0));
        fVar24 = extraout_s0_04;
        fVar25 = extraout_s1_04;
        fVar26 = extraout_s2_04;
        goto LAB_0013222e;
      }
    }
    iVar2 = Status::getCurrentCampaignMission();
    fVar24 = extraout_s0_07;
    fVar25 = extraout_s1_07;
    fVar26 = extraout_s2_07;
    if (0x4f < iVar2) {
      uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
      uVar27 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
      AbyssEngine::Transform::Update
                (CONCAT44((int)((ulonglong)uVar27 >> 0x20),uVar7),
                 SUB41(*(undefined4 *)((int)uVar27 + 0xf8),0));
      fVar24 = extraout_s0_08;
      fVar25 = extraout_s1_08;
      fVar26 = extraout_s2_08;
    }
    goto LAB_0013222e;
  }
  if (iVar2 - 0x6dU < 2) {
LAB_00131e16:
    pAVar6 = operator_new(0xc0);
    puVar17 = *(undefined4 **)(DAT_00131f04 + 0x131e24);
    AEGeometry::AEGeometry(pAVar6,0x4953,(PaintCanvas *)*puVar17,false);
    *(AEGeometry **)pPVar20 = pAVar6;
    pAVar6 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar6,0x4954,(PaintCanvas *)*puVar17,false);
    AEGeometry::addChild(*(uint *)pPVar20);
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x4955,(PaintCanvas *)*puVar17,false);
    AEGeometry::addChild(*(uint *)pPVar20);
    pAVar10 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar10,0x4956,(PaintCanvas *)*puVar17,false);
    AEGeometry::addChild(*(uint *)pPVar20);
    if (this_00 != (Array *)0x0) {
      pvVar4 = (void *)Array<int>::~Array((Array<int> *)this_00);
      operator_delete(pvVar4);
    }
    pFVar3 = operator_new(1);
    FileRead::FileRead(pFVar3);
    this_00 = (Array *)FileRead::loadStaticCollision((int)pFVar3);
    FileRead::~FileRead(pFVar3);
    pcVar18 = *(code **)(DAT_00131f08 + 0x131ece);
    (*pcVar18)(pFVar3);
    pcVar15 = *(code **)(DAT_00131f0c + 0x131ed8);
    (*pcVar15)(pAVar6);
    (*pcVar18)();
    (*pcVar15)(pAVar9);
    (*pcVar18)();
    (*pcVar15)(pAVar10);
    (*pcVar18)();
    fVar24 = extraout_s0_09;
    fVar25 = extraout_s1_09;
    fVar26 = extraout_s2_09;
  }
  else {
    if (iVar2 == 0x6f) {
      iVar2 = Status::getCurrentCampaignMission();
      if (0x5e < iVar2) goto LAB_00131e16;
      iVar2 = *(int *)(this + 0x148);
      fVar24 = extraout_s0_10;
      fVar25 = extraout_s1_10;
      fVar26 = extraout_s2_10;
    }
    if (iVar2 == 100) {
      iVar2 = Status::getCurrentCampaignMission();
      iVar23 = Status::dlc1Won();
      uVar1 = 0x25;
      if (iVar2 == 0x50) {
        uVar1 = 0x21;
      }
      uVar19 = 0x24;
      if (iVar2 == 0x50) {
        uVar19 = 0x20;
      }
      uVar21 = 0x28;
      if (iVar2 == 0x50 || iVar23 != 0) {
        uVar21 = 0x2a;
      }
      pAVar6 = operator_new(0xc0);
      uVar12 = 0x4223;
      if (iVar23 != 0) {
        uVar12 = 0x3823;
      }
      puVar16 = *(uint **)(DAT_00132320 + 0x131f9c);
      if (iVar2 == 0x50) {
        uVar12 = 0x381f;
      }
      AEGeometry::AEGeometry(pAVar6,uVar12,(PaintCanvas *)*puVar16,false);
      *(AEGeometry **)pPVar20 = pAVar6;
      local_64 = -NAN;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,(uint *)&local_64);
      AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,SUB42(local_64,0),(bool)uVar21);
      AEGeometry::addChild(*(uint *)pPVar20);
      *(float *)(this + 0x144) = local_64;
      local_58[1] = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,local_58 + 1);
      AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,(ushort)local_58[1],(bool)uVar19);
      AEGeometry::addChild(*(uint *)pPVar20);
      local_58[0] = 0xffffffff;
      AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar16,local_58);
      AbyssEngine::PaintCanvas::TransformAddMesh(*puVar16,(ushort)local_58[0],(bool)uVar1);
      AEGeometry::addChild(*(uint *)pPVar20);
      if (iVar2 == 0x50 || iVar23 != 0) {
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        AbyssEngine::Transform::SetAnimationState(uVar7,0,0);
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        AbyssEngine::Transform::SetAnimationState(uVar7,0,0);
        uVar7 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar16);
        AbyssEngine::Transform::SetAnimationState(uVar7,0,0);
        if (this_00 != (Array *)0x0) {
          pvVar4 = (void *)Array<int>::~Array((Array<int> *)this_00);
          operator_delete(pvVar4);
        }
        pFVar3 = operator_new(1);
        FileRead::FileRead(pFVar3);
        this_00 = (Array *)FileRead::loadStationCollision((int)pFVar3);
        pvVar4 = (void *)FileRead::~FileRead(pFVar3);
        operator_delete(pvVar4);
        fVar24 = extraout_s0_11;
        fVar25 = extraout_s1_11;
        fVar26 = extraout_s2_11;
      }
      else {
        local_4c = CONCAT22(local_4c._2_2_,0x4226);
        local_48 = DAT_0013256c;
        AEGeometry::setLodMeshes(*(AEGeometry **)pPVar20,(ushort *)&local_4c,(int *)&local_48,1);
        uStack_4e = 0x4227;
        AEGeometry::setLodChildMeshes(*(AEGeometry **)pPVar20,&uStack_4e);
        fVar24 = extraout_s0_15;
        fVar25 = extraout_s1_17;
        fVar26 = extraout_s2_16;
      }
    }
    else if ((uVar8 | 2) == 2) {
      pAVar6 = operator_new(0xc0);
      puVar17 = *(undefined4 **)(DAT_00132324 + 0x1320ce);
      AEGeometry::AEGeometry(pAVar6,(short)iVar2 + 0x5528,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pAVar9 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar9,*(short *)(this + 0x148) + 22000,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar6);
      operator_delete(pvVar4);
      pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar9);
      operator_delete(pvVar4);
      fVar24 = extraout_s0_12;
      fVar25 = extraout_s1_12;
      fVar26 = extraout_s2_12;
    }
    else if (uVar8 == 3) {
      pAVar6 = operator_new(0xc0);
      puVar17 = *(undefined4 **)(DAT_00132328 + 0x132142);
      AEGeometry::AEGeometry(pAVar6,(short)iVar2 + 0x5528,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      pAVar9 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar9,*(short *)(this + 0x148) + 22000,(PaintCanvas *)*puVar17,false);
      AEGeometry::addChild(*(uint *)pPVar20);
      if (*(int *)(this + 0x148) == 0x6c) {
        pAVar10 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar10,0x5974,(PaintCanvas *)*puVar17,false);
        AEGeometry::addChild(*(uint *)pPVar20);
        *(undefined4 *)(this + 0x164) = *(undefined4 *)(pAVar10 + 0xc);
        pAVar10 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar10,0x5975,(PaintCanvas *)*puVar17,false);
        AEGeometry::addChild(*(uint *)pPVar20);
        *(undefined4 *)(this + 0x168) = *(undefined4 *)(pAVar10 + 0xc);
        pAVar10 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar10,0x5976,(PaintCanvas *)*puVar17,false);
        AEGeometry::addChild(*(uint *)pPVar20);
        *(undefined4 *)(this + 0x16c) = *(undefined4 *)(pAVar10 + 0xc);
        pAVar10 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar10,0x5977,(PaintCanvas *)*puVar17,false);
        AEGeometry::addChild(*(uint *)pPVar20);
        *(undefined4 *)(this + 0x170) = *(undefined4 *)(pAVar10 + 0xc);
      }
      pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar6);
      operator_delete(pvVar4);
      pvVar4 = (void *)AEGeometry::~AEGeometry(pAVar9);
      operator_delete(pvVar4);
      fVar24 = extraout_s0_13;
      fVar25 = extraout_s1_13;
      fVar26 = extraout_s2_13;
    }
  }
LAB_0013222e:
  if (this_00 != (Array *)0x0) {
    uVar8 = **(uint **)(this_00 + 4);
    local_64 = 0.0;
    local_60 = 0.0;
    local_5c = 0.0;
    this_03 = operator_new(0xc);
    Array<BoundingVolume*>::Array(this_03);
    *(Array<BoundingVolume*> **)(this + 0x130) = this_03;
    ArraySetLength<BoundingVolume*>(uVar8,(Array *)this_03);
    iVar2 = 1;
    for (iVar23 = 0; iVar23 < (int)uVar8; iVar23 = iVar23 + 1) {
      iVar11 = *(int *)(this_00 + 4);
      iVar13 = iVar2 + 1;
      iVar14 = *(int *)(iVar11 + iVar2 * 4);
      if (iVar14 == 1) {
        iVar14 = iVar11 + iVar2 * 4;
        local_64 = (float)VectorSignedToFloat(*(undefined4 *)(iVar14 + 0x10),
                                              (byte)(in_fpscr >> 0x16) & 3);
        VectorSignedToFloat(*(undefined4 *)(iVar14 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
        VectorSignedToFloat(*(undefined4 *)(iVar14 + 8),(byte)(in_fpscr >> 0x16) & 3);
        local_5c = (float)VectorSignedToFloat(*(undefined4 *)(iVar14 + 0x14),
                                              (byte)(in_fpscr >> 0x16) & 3);
        local_60 = (float)VectorSignedToFloat(*(undefined4 *)(iVar14 + 0x18),
                                              (byte)(in_fpscr >> 0x16) & 3);
        VectorSignedToFloat(-*(int *)(iVar11 + iVar13 * 4),(byte)(in_fpscr >> 0x16) & 3);
        if (iVar5 == 0) {
          AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_64,local_60);
        }
        else {
          AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_64,local_60);
        }
        this_04 = operator_new(0x2c);
        BoundingAAB::BoundingAAB
                  ((BoundingAAB *)this_04,local_64 + local_64,extraout_s1_15,local_60 + local_60,
                   extraout_s3_00,local_5c + local_5c,extraout_s5_00,extraout_s6_00,extraout_s7,
                   extraout_s8);
        iVar13 = iVar2 + 7;
LAB_001323c8:
        *(BoundingSphere **)(*(int *)(*(int *)(this + 0x130) + 4) + iVar23 * 4) = this_04;
      }
      else if (iVar14 == 0) {
        iVar14 = iVar11 + iVar2 * 4;
        fVar24 = (float)-*(int *)(iVar11 + iVar13 * 4);
        local_64 = (float)VectorSignedToFloat(*(undefined4 *)(iVar14 + 0x10),
                                              (byte)(in_fpscr >> 0x16) & 3);
        VectorSignedToFloat(*(undefined4 *)(iVar14 + 0xc),(byte)(in_fpscr >> 0x16) & 3);
        VectorSignedToFloat(*(undefined4 *)(iVar14 + 8),(byte)(in_fpscr >> 0x16) & 3);
        VectorSignedToFloat(fVar24,(byte)(in_fpscr >> 0x16) & 3);
        if (iVar5 == 0) {
          AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_64,fVar24);
        }
        else {
          AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_64,fVar24);
        }
        in_fpscr = in_fpscr & 0xfffffff;
        if (local_64 < 0.0) {
          AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_64,local_64);
        }
        this_04 = operator_new(0x48);
        BoundingSphere::BoundingSphere
                  (this_04,local_64,extraout_s1_14,extraout_s2_14,extraout_s3,extraout_s4,
                   extraout_s5,extraout_s6);
        iVar13 = iVar2 + 5;
        goto LAB_001323c8;
      }
      iVar2 = iVar13;
    }
    ArrayRelease<int>(this_00);
    pvVar4 = (void *)Array<int>::~Array((Array<int> *)this_00);
    operator_delete(pvVar4);
    fVar24 = extraout_s0_14;
    fVar25 = extraout_s1_16;
    fVar26 = extraout_s2_15;
  }
  this[0x71] = (PlayerStation)0x1;
  AEGeometry::setRotation(*(AEGeometry **)(this + 0x140),fVar24,fVar25,fVar26);
  iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_00132584 + 0x132416));
  *(int *)(this + 0x154) = (int)(*(float *)(iVar2 + 0xe0) + DAT_00132580);
  iVar2 = *piVar22 - local_44;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  00131818: push {r4,r5,r6,r7,lr}
  0013181a: add r7,sp,#0xc
  0013181c: push {r7,r8,r9,r10,r11}
  00131820: vpush {d8,d9,d10}
  00131824: sub sp,#0x58
  00131826: mov r10,r0
  00131828: ldr r0,[0x00131b94]
  0013182a: mov r4,r1
  0013182c: movs r5,#0x0
  0013182e: add r0,pc
  00131830: mov.w r1,#0xffffffff
  00131834: movs r2,#0x0
  00131836: movs r3,#0x0
  00131838: ldr.w r11,[r0,#0x0]
  0013183c: ldr.w r0,[r11,#0x0]
  00131840: str r0,[sp,#0x54]
  00131842: mov r0,r10
  00131844: strd r5,r5,[sp,#0x0]
  00131848: blx 0x000730a8
  0013184c: ldr r0,[0x00131b98]
  0013184e: add r0,pc
  00131850: ldr r1,[r0,#0x0]
  00131852: ldr.w r0,[r10,#0x4]
  00131856: adds r1,#0x8
  00131858: str.w r1,[r10,#0x0]
  0013185c: strd r5,r5,[r10,#0x158]
  00131860: str.w r5,[r10,#0x160]
  00131864: movw r1,#0x3a98
  00131868: blx 0x000730d8
  0013186c: ldr.w r0,[r10,#0x4]
  00131870: ldr r1,[0x00131b9c]
  00131872: strd r5,r5,[r10,#0x58]
  00131876: str.w r5,[r10,#0x60]
  0013187a: str.w r5,[r10,#0x130]
  0013187e: strb.w r5,[r10,#0x25]
  00131882: blx 0x00073f78
  00131886: movs r0,#0x0
  00131888: str.w r0,[r10,#0x140]
  0013188c: str.w r0,[r10,#0x14c]
  00131890: str.w r0,[r10,#0x154]
  00131894: mov r0,r4
  00131896: blx 0x00071824
  0013189a: mov r4,r0
  0013189c: str.w r0,[r10,#0x148]
  001318a0: movs r0,#0x1
  001318a2: blx 0x0006eb24
  001318a6: mov r5,r0
  001318a8: blx 0x00072124
  001318ac: mov r0,r5
  001318ae: mov r1,r4
  001318b0: blx 0x000743bc
  001318b4: mov r6,r0
  001318b6: mov r0,r5
  001318b8: blx 0x0007213c
  001318bc: blx 0x0006eb48
  001318c0: ldr r0,[0x00131ba0]
  001318c2: add r0,pc
  001318c4: ldr.w r8,[r0,#0x0]
  001318c8: ldr.w r0,[r8,#0x0]
  001318cc: blx 0x000723d0
  001318d0: str r0,[sp,#0x30]
  001318d2: sub.w r0,r4,#0x6d
  001318d6: add.w r9,r10,#0x140
  001318da: cmp r0,#0x2
  001318dc: bcc.w 0x00131b1e
  001318e0: cmp r6,#0x0
  001318e2: bne.w 0x00131b1e
  001318e6: ldr r0,[sp,#0x30]
  001318e8: cmp r0,#0x0
  001318ea: beq.w 0x00131a04
  001318ee: ldr.w r0,[r8,#0x0]
  001318f2: blx 0x000719a4
  001318f6: cmp r0,#0x0
  001318f8: beq.w 0x00131a8c
  001318fc: movs r0,#0xc0
  001318fe: blx 0x0006eb24
  00131902: mov r4,r0
  00131904: ldr r0,[0x00131ba4]
  00131906: add r0,pc
  00131908: ldr r5,[r0,#0x0]
  0013190a: ldr r2,[r5,#0x0]
  0013190c: mov r0,r4
  0013190e: movw r1,#0x4220
  00131912: movs r3,#0x0
  00131914: blx 0x0007207c
  00131918: str.w r4,[r9,#0x0]
  0013191c: mov.w r1,#0xffffffff
  00131920: ldr r0,[r5,#0x0]
  00131922: str r1,[sp,#0x34]
  00131924: add r1,sp,#0x34
  00131926: blx 0x000720ac
  0013192a: ldr r1,[sp,#0x34]
  0013192c: ldr r0,[r5,#0x0]
  0013192e: movw r2,#0x4221
  00131932: movs r3,#0x0
  00131934: blx 0x0007303c
  00131938: ldr r1,[sp,#0x34]
  0013193a: ldr.w r0,[r9,#0x0]
  0013193e: blx 0x000720e8
  00131942: ldr r0,[r5,#0x0]
  00131944: mov.w r1,#0xffffffff
  00131948: str r1,[sp,#0x44]
  0013194a: add r1,sp,#0x44
  0013194c: blx 0x000720ac
  00131950: ldr r1,[sp,#0x44]
  00131952: ldr r0,[r5,#0x0]
  00131954: movw r2,#0x4222
  00131958: movs r3,#0x0
  0013195a: blx 0x0007303c
  0013195e: ldr r1,[sp,#0x44]
  00131960: ldr.w r0,[r9,#0x0]
  00131964: blx 0x000720e8
  00131968: movs r0,#0x1
  0013196a: blx 0x0006eb24
  0013196e: mov r4,r0
  00131970: blx 0x00072124
  00131974: mov r0,r4
  00131976: movw r1,#0x3eb
  0013197a: blx 0x000743bc
  0013197e: mov r6,r0
  00131980: mov r0,r4
  00131982: blx 0x0007213c
  00131986: blx 0x0006eb48
  0013198a: ldr.w r0,[r9,#0x0]
  0013198e: ldr r1,[r0,#0xc]
  00131990: ldr r0,[r5,#0x0]
  00131992: blx 0x00072088
  00131996: mov r4,r0
  00131998: ldr.w r0,[r9,#0x0]
  0013199c: ldr r1,[r0,#0xc]
  0013199e: ldr r0,[r5,#0x0]
  001319a0: blx 0x00072088
  001319a4: ldrd r2,r3,[r0,#0xf8]
  001319a8: movs r0,#0x0
  001319aa: str r0,[sp,#0x0]
  001319ac: mov r0,r4
  001319ae: blx 0x0006f7cc
  001319b2: ldr.w r0,[r9,#0x0]
  001319b6: ldr r1,[r0,#0x14]
  001319b8: ldr r0,[r5,#0x0]
  001319ba: blx 0x00072088
  001319be: mov r4,r0
  001319c0: ldr.w r0,[r9,#0x0]
  001319c4: ldr r1,[r0,#0x14]
  001319c6: ldr r0,[r5,#0x0]
  001319c8: blx 0x00072088
  001319cc: ldrd r2,r3,[r0,#0xf8]
  001319d0: movs r0,#0x0
  001319d2: str r0,[sp,#0x0]
  001319d4: mov r0,r4
  001319d6: blx 0x0006f7cc
  001319da: ldr.w r0,[r9,#0x0]
  001319de: ldr r1,[r0,#0x10]
  001319e0: ldr r0,[r5,#0x0]
  001319e2: blx 0x00072088
  001319e6: mov r4,r0
  001319e8: ldr.w r0,[r9,#0x0]
  001319ec: ldr r1,[r0,#0x10]
  001319ee: ldr r0,[r5,#0x0]
  001319f0: blx 0x00072088
  001319f4: ldrd r2,r3,[r0,#0xf8]
  001319f8: movs r0,#0x0
  001319fa: str r0,[sp,#0x0]
  001319fc: mov r0,r4
  001319fe: blx 0x0006f7cc
  00131a02: b 0x00131b1e
  00131a04: movs r0,#0xc0
  00131a06: blx 0x0006eb24
  00131a0a: mov r4,r0
  00131a0c: ldr r0,[0x00131ba8]
  00131a0e: add r0,pc
  00131a10: ldr r6,[r0,#0x0]
  00131a12: ldr r2,[r6,#0x0]
  00131a14: mov r0,r4
  00131a16: movw r1,#0x4034
  00131a1a: movs r3,#0x0
  00131a1c: blx 0x0007207c
  00131a20: str.w r4,[r9,#0x0]
  00131a24: movs r0,#0xc0
  00131a26: blx 0x0006eb24
  00131a2a: mov r4,r0
  00131a2c: ldr r2,[r6,#0x0]
  00131a2e: movw r1,#0x4037
  00131a32: movs r3,#0x0
  00131a34: blx 0x0007207c
  00131a38: ldr r1,[r4,#0xc]
  00131a3a: ldr.w r0,[r9,#0x0]
  00131a3e: blx 0x000720e8
  00131a42: movs r0,#0xc0
  00131a44: blx 0x0006eb24
  00131a48: mov r5,r0
  00131a4a: ldr r2,[r6,#0x0]
  00131a4c: movw r1,#0x403a
  00131a50: movs r3,#0x0
  00131a52: blx 0x0007207c
  00131a56: ldr r1,[r5,#0xc]
  00131a58: ldr.w r0,[r9,#0x0]
  00131a5c: blx 0x000720e8
  00131a60: mov r0,r4
  00131a62: blx 0x00071fc8
  00131a66: blx 0x0006eb48
  00131a6a: mov r0,r5
  00131a6c: blx 0x00071fc8
  00131a70: blx 0x0006eb48
  00131a74: movs r0,#0x1
  00131a76: blx 0x0006eb24
  00131a7a: mov r4,r0
  00131a7c: blx 0x00072124
  00131a80: mov r0,r4
  00131a82: mov.w r1,#0x3e8
  00131a86: blx 0x000743bc
  00131a8a: b 0x00131b12
  00131a8c: movs r0,#0xc0
  00131a8e: blx 0x0006eb24
  00131a92: mov r4,r0
  00131a94: ldr r0,[0x00131bac]
  00131a96: add r0,pc
  00131a98: ldr r6,[r0,#0x0]
  00131a9a: ldr r2,[r6,#0x0]
  00131a9c: mov r0,r4
  00131a9e: movw r1,#0x403b
  00131aa2: movs r3,#0x0
  00131aa4: blx 0x0007207c
  00131aa8: str.w r4,[r9,#0x0]
  00131aac: movs r0,#0xc0
  00131aae: blx 0x0006eb24
  00131ab2: mov r4,r0
  00131ab4: ldr r2,[r6,#0x0]
  00131ab6: movw r1,#0x403e
  00131aba: movs r3,#0x0
  00131abc: blx 0x0007207c
  00131ac0: ldr r1,[r4,#0xc]
  00131ac2: ldr.w r0,[r9,#0x0]
  00131ac6: blx 0x000720e8
  00131aca: movs r0,#0xc0
  00131acc: blx 0x0006eb24
  00131ad0: mov r5,r0
  00131ad2: ldr r2,[r6,#0x0]
  00131ad4: movw r1,#0x4041
  00131ad8: movs r3,#0x0
  00131ada: blx 0x0007207c
  00131ade: ldr r1,[r5,#0xc]
  00131ae0: ldr.w r0,[r9,#0x0]
  00131ae4: blx 0x000720e8
  00131ae8: mov r0,r4
  00131aea: blx 0x00071fc8
  00131aee: blx 0x0006eb48
  00131af2: mov r0,r5
  00131af4: blx 0x00071fc8
  00131af8: blx 0x0006eb48
  00131afc: movs r0,#0x1
  00131afe: blx 0x0006eb24
  00131b02: mov r4,r0
  00131b04: blx 0x00072124
  00131b08: mov r0,r4
  00131b0a: movw r1,#0x3e9
  00131b0e: blx 0x000743bc
  00131b12: mov r6,r0
  00131b14: mov r0,r4
  00131b16: blx 0x0007213c
  00131b1a: blx 0x0006eb48
  00131b1e: ldr.w r0,[r9,#0x0]
  00131b22: cmp r0,#0x0
  00131b24: bne.w 0x0013222e
  00131b28: ldr.w r5,[r10,#0x148]
  00131b2c: str r6,[sp,#0x20]
  00131b2e: sub.w r0,r5,#0x6d
  00131b32: cmp r0,#0x2
  00131b34: bhi 0x00131b54
  00131b36: movs r0,#0xc0
  00131b38: blx 0x0006eb24
  00131b3c: mov r4,r0
  00131b3e: ldr r0,[0x00131bb0]
  00131b40: add r0,pc
  00131b42: ldr r0,[r0,#0x0]
  00131b44: ldr r2,[r0,#0x0]
  00131b46: mov r0,r4
  00131b48: movw r1,#0x5254
  00131b4c: movs r3,#0x0
  00131b4e: blx 0x0007207c
  00131b52: b 0x00131b74
  00131b54: movs r0,#0xc0
  00131b56: blx 0x0006eb24
  00131b5a: mov r4,r0
  00131b5c: ldr r0,[0x00131bb4]
  00131b5e: movw r1,#0x5208
  00131b62: add r0,pc
  00131b64: ldr r0,[r0,#0x0]
  00131b66: ldr r2,[r0,#0x0]
  00131b68: adds r0,r5,r1
  00131b6a: uxth r1,r0
  00131b6c: mov r0,r4
  00131b6e: movs r3,#0x0
  00131b70: blx 0x0007207c
  00131b74: str.w r4,[r9,#0x0]
  00131b78: ldr.w r0,[r8,#0x0]
  00131b7c: blx 0x00071a10
  00131b80: cbz r0,0x00131bb8
  00131b82: ldr.w r0,[r8,#0x0]
  00131b86: blx 0x00071a10
  00131b8a: blx 0x00071950
  00131b8e: mov r4,r0
  00131b90: b 0x00131bba
  00131bb8: movs r4,#0x9
  00131bba: ldr.w r5,[r10,#0x148]
  00131bbe: cmp r5,#0x6f
  00131bc0: beq.w 0x00131cf2
  00131bc4: cmp r5,#0x65
  00131bc6: bne.w 0x00131e0e
  00131bca: movs r0,#0xc0
  00131bcc: blx 0x0006eb24
  00131bd0: mov r4,r0
  00131bd2: ldr r0,[0x00131ef4]
  00131bd4: add r0,pc
  00131bd6: ldr r5,[r0,#0x0]
  00131bd8: ldr r2,[r5,#0x0]
  00131bda: mov r0,r4
  00131bdc: movw r1,#0x4220
  00131be0: movs r3,#0x0
  00131be2: blx 0x0007207c
  00131be6: str.w r4,[r9,#0x0]
  00131bea: mov.w r1,#0xffffffff
  00131bee: ldr r0,[r5,#0x0]
  00131bf0: str r1,[sp,#0x34]
  00131bf2: add r1,sp,#0x34
  00131bf4: blx 0x000720ac
  00131bf8: ldr r1,[sp,#0x34]
  00131bfa: ldr r0,[r5,#0x0]
  00131bfc: movw r2,#0x4221
  00131c00: movs r3,#0x0
  00131c02: blx 0x0007303c
  00131c06: ldr r1,[sp,#0x34]
  00131c08: ldr.w r0,[r9,#0x0]
  00131c0c: blx 0x000720e8
  00131c10: ldr r0,[r5,#0x0]
  00131c12: mov.w r1,#0xffffffff
  00131c16: str r1,[sp,#0x44]
  00131c18: add r1,sp,#0x44
  00131c1a: blx 0x000720ac
  00131c1e: ldr r1,[sp,#0x44]
  00131c20: ldr r0,[r5,#0x0]
  00131c22: movw r2,#0x4222
  00131c26: movs r3,#0x0
  00131c28: blx 0x0007303c
  00131c2c: ldr r1,[sp,#0x44]
  00131c2e: ldr.w r0,[r9,#0x0]
  00131c32: blx 0x000720e8
  00131c36: ldr.w r0,[r8,#0x0]
  00131c3a: blx 0x00071770
  00131c3e: cmp r0,#0x9d
  00131c40: bne.w 0x00131dd2
  00131c44: ldr.w r0,[r8,#0x0]
  00131c48: blx 0x00071c14
  00131c4c: blx 0x00071824
  00131c50: cmp r0,#0x70
  00131c52: bne.w 0x00131dd2
  00131c56: ldr r0,[r5,#0x0]
  00131c58: mov.w r1,#0xffffffff
  00131c5c: str r1,[sp,#0x40]
  00131c5e: add r1,sp,#0x40
  00131c60: blx 0x000720ac
  00131c64: ldr r1,[sp,#0x40]
  00131c66: ldr r0,[r5,#0x0]
  00131c68: movw r2,#0x4950
  00131c6c: movs r3,#0x0
  00131c6e: blx 0x0007303c
  00131c72: ldr r1,[sp,#0x40]
  00131c74: ldr.w r0,[r9,#0x0]
  00131c78: blx 0x000720e8
  00131c7c: ldr r0,[r5,#0x0]
  00131c7e: mov.w r1,#0xffffffff
  00131c82: str r1,[sp,#0x50]
  00131c84: add r1,sp,#0x50
  00131c86: blx 0x000720ac
  00131c8a: ldr r1,[sp,#0x50]
  00131c8c: ldr r0,[r5,#0x0]
  00131c8e: movw r2,#0x4952
  00131c92: movs r3,#0x0
  00131c94: blx 0x0007303c
  00131c98: ldr r1,[sp,#0x50]
  00131c9a: ldr.w r0,[r9,#0x0]
  00131c9e: blx 0x000720e8
  00131ca2: ldr r0,[r5,#0x0]
  00131ca4: mov.w r1,#0xffffffff
  00131ca8: str r1,[sp,#0x4c]
  00131caa: add r1,sp,#0x4c
  00131cac: blx 0x000720ac
  00131cb0: ldr r1,[sp,#0x4c]
  00131cb2: ldr r0,[r5,#0x0]
  00131cb4: movw r2,#0x4951
  00131cb8: movs r3,#0x0
  00131cba: blx 0x0007303c
  00131cbe: ldr r1,[sp,#0x4c]
  00131cc0: ldr.w r0,[r9,#0x0]
  00131cc4: blx 0x000720e8
  00131cc8: ldr.w r0,[r9,#0x0]
  00131ccc: ldr r1,[r0,#0xc]
  00131cce: ldr r0,[r5,#0x0]
  00131cd0: blx 0x00072088
  00131cd4: mov r4,r0
  00131cd6: ldr.w r0,[r9,#0x0]
  00131cda: ldr r1,[r0,#0xc]
  00131cdc: ldr r0,[r5,#0x0]
  00131cde: blx 0x00072088
  00131ce2: ldrd r2,r3,[r0,#0xf8]
  00131ce6: movs r0,#0x0
  00131ce8: str r0,[sp,#0x0]
  00131cea: mov r0,r4
  00131cec: blx 0x0006f7cc
  00131cf0: b 0x0013222c
  00131cf2: ldr.w r0,[r8,#0x0]
  00131cf6: blx 0x00071770
  00131cfa: cmp r0,#0x5d
  00131cfc: bgt.w 0x00131e0a
  00131d00: movs r0,#0xc0
  00131d02: blx 0x0006eb24
  00131d06: mov r4,r0
  00131d08: ldr r0,[0x00131ef8]
  00131d0a: add r0,pc
  00131d0c: ldr r5,[r0,#0x0]
  00131d0e: ldr r2,[r5,#0x0]
  00131d10: mov r0,r4
  00131d12: movw r1,#0x5254
  00131d16: movs r3,#0x0
  00131d18: blx 0x0007207c
  00131d1c: str.w r9,[sp,#0x28]
  00131d20: str.w r4,[r9,#0x0]
  00131d24: movs r0,#0xc0
  00131d26: blx 0x0006eb24
  00131d2a: mov r9,r0
  00131d2c: ldr r2,[r5,#0x0]
  00131d2e: movw r1,#0x5574
  00131d32: movs r3,#0x0
  00131d34: blx 0x0007207c
  00131d38: ldr r0,[sp,#0x28]
  00131d3a: ldr.w r1,[r9,#0xc]
  00131d3e: ldr r0,[r0,#0x0]
  00131d40: blx 0x000720e8
  00131d44: movs r0,#0xc0
  00131d46: str.w r11,[sp,#0x24]
  00131d4a: blx 0x0006eb24
  00131d4e: mov r11,r0
  00131d50: ldr r2,[r5,#0x0]
  00131d52: movw r1,#0x563c
  00131d56: movs r3,#0x0
  00131d58: blx 0x0007207c
  00131d5c: ldr r0,[sp,#0x28]
  00131d5e: ldr.w r1,[r11,#0xc]
  00131d62: ldr r0,[r0,#0x0]
  00131d64: blx 0x000720e8
  00131d68: movs r0,#0xc0
  00131d6a: blx 0x0006eb24
  00131d6e: mov r8,r0
  00131d70: ldr r2,[r5,#0x0]
  00131d72: movw r1,#0x495d
  00131d76: movs r3,#0x0
  00131d78: blx 0x0007207c
  00131d7c: ldr r0,[sp,#0x28]
  00131d7e: ldr.w r1,[r8,#0xc]
  00131d82: ldr r0,[r0,#0x0]
  00131d84: blx 0x000720e8
  00131d88: movs r0,#0xc0
  00131d8a: blx 0x0006eb24
  00131d8e: mov r4,r0
  00131d90: ldr r2,[r5,#0x0]
  00131d92: movw r1,#0x495e
  00131d96: movs r3,#0x0
  00131d98: blx 0x0007207c
  00131d9c: ldr r0,[sp,#0x28]
  00131d9e: ldr r1,[r4,#0xc]
  00131da0: ldr r0,[r0,#0x0]
  00131da2: blx 0x000720e8
  00131da6: ldr r0,[0x00131efc]
  00131da8: add r0,pc
  00131daa: ldr r5,[r0,#0x0]
  00131dac: mov r0,r9
  00131dae: blx r5
  00131db0: ldr r0,[0x00131f00]
  00131db2: add r0,pc
  00131db4: ldr r6,[r0,#0x0]
  00131db6: mov r0,r9
  00131db8: blx r6
  00131dba: mov r0,r11
  00131dbc: blx r5
  00131dbe: blx r6
  00131dc0: mov r0,r8
  00131dc2: blx r5
  00131dc4: blx r6
  00131dc6: mov r0,r4
  00131dc8: blx r5
  00131dca: blx r6
  00131dcc: ldrd r11,r9,[sp,#0x24]
  00131dd0: b 0x0013222c
  00131dd2: ldr.w r0,[r8,#0x0]
  00131dd6: blx 0x00071770
  00131dda: cmp r0,#0x50
  00131ddc: blt.w 0x0013222c
  00131de0: ldr.w r0,[r9,#0x0]
  00131de4: ldr r1,[r0,#0xc]
  00131de6: ldr r0,[r5,#0x0]
  00131de8: blx 0x00072088
  00131dec: mov r4,r0
  00131dee: ldr.w r0,[r9,#0x0]
  00131df2: ldr r1,[r0,#0xc]
  00131df4: ldr r0,[r5,#0x0]
  00131df6: blx 0x00072088
  00131dfa: ldrd r2,r3,[r0,#0xf8]
  00131dfe: movs r0,#0x0
  00131e00: str r0,[sp,#0x0]
  00131e02: mov r0,r4
  00131e04: blx 0x0006f7cc
  00131e08: b 0x0013222c
  00131e0a: ldr.w r5,[r10,#0x148]
  00131e0e: sub.w r0,r5,#0x6d
  00131e12: cmp r0,#0x2
  00131e14: bcs 0x00131f10
  00131e16: movs r0,#0xc0
  00131e18: blx 0x0006eb24
  00131e1c: mov r4,r0
  00131e1e: ldr r0,[0x00131f04]
  00131e20: add r0,pc
  00131e22: ldr r5,[r0,#0x0]
  00131e24: ldr r2,[r5,#0x0]
  00131e26: mov r0,r4
  00131e28: movw r1,#0x4953
  00131e2c: movs r3,#0x0
  00131e2e: blx 0x0007207c
  00131e32: str.w r9,[sp,#0x28]
  00131e36: str.w r4,[r9,#0x0]
  00131e3a: movs r0,#0xc0
  00131e3c: blx 0x0006eb24
  00131e40: ldr r4,[sp,#0x20]
  00131e42: mov r9,r0
  00131e44: ldr r2,[r5,#0x0]
  00131e46: movw r1,#0x4954
  00131e4a: movs r3,#0x0
  00131e4c: blx 0x0007207c
  00131e50: ldr r0,[sp,#0x28]
  00131e52: ldr.w r1,[r9,#0xc]
  00131e56: ldr r0,[r0,#0x0]
  00131e58: blx 0x000720e8
  00131e5c: movs r0,#0xc0
  00131e5e: str.w r11,[sp,#0x24]
  00131e62: blx 0x0006eb24
  00131e66: mov r11,r0
  00131e68: ldr r2,[r5,#0x0]
  00131e6a: movw r1,#0x4955
  00131e6e: movs r3,#0x0
  00131e70: blx 0x0007207c
  00131e74: ldr r0,[sp,#0x28]
  00131e76: ldr.w r1,[r11,#0xc]
  00131e7a: ldr r0,[r0,#0x0]
  00131e7c: blx 0x000720e8
  00131e80: movs r0,#0xc0
  00131e82: blx 0x0006eb24
  00131e86: mov r6,r0
  00131e88: ldr r2,[r5,#0x0]
  00131e8a: movw r1,#0x4956
  00131e8e: movs r3,#0x0
  00131e90: blx 0x0007207c
  00131e94: ldr r0,[sp,#0x28]
  00131e96: ldr r1,[r6,#0xc]
  00131e98: ldr r0,[r0,#0x0]
  00131e9a: blx 0x000720e8
  00131e9e: cbz r4,0x00131eaa
  00131ea0: mov r0,r4
  00131ea2: blx 0x00070204
  00131ea6: blx 0x0006eb48
  00131eaa: movs r0,#0x1
  00131eac: blx 0x0006eb24
  00131eb0: mov r4,r0
  00131eb2: blx 0x00072124
  00131eb6: mov r0,r4
  00131eb8: movw r1,#0x7d2
  00131ebc: blx 0x000743b0
  00131ec0: mov r8,r0
  00131ec2: mov r0,r4
  00131ec4: blx 0x0007213c
  00131ec8: ldr r0,[0x00131f08]
  00131eca: add r0,pc
  00131ecc: ldr r5,[r0,#0x0]
  00131ece: mov r0,r4
  00131ed0: blx r5
  00131ed2: ldr r0,[0x00131f0c]
  00131ed4: add r0,pc
  00131ed6: ldr r4,[r0,#0x0]
  00131ed8: mov r0,r9
  00131eda: blx r4
  00131edc: blx r5
  00131ede: mov r0,r11
  00131ee0: blx r4
  00131ee2: blx r5
  00131ee4: mov r0,r6
  00131ee6: blx r4
  00131ee8: blx r5
  00131eea: ldrd r11,r9,[sp,#0x24]
  00131eee: mov r6,r8
  00131ef0: b 0x0013222e
  00131f10: cmp r5,#0x6f
  00131f12: bne 0x00131f26
  00131f14: ldr.w r0,[r8,#0x0]
  00131f18: blx 0x00071770
  00131f1c: cmp r0,#0x5e
  00131f1e: bgt.w 0x00131e16
  00131f22: ldr.w r5,[r10,#0x148]
  00131f26: cmp r5,#0x64
  00131f28: bne.w 0x001320b8
  00131f2c: ldr.w r0,[r8,#0x0]
  00131f30: blx 0x00071770
  00131f34: strd r11,r9,[sp,#0x24]
  00131f38: mov r4,r0
  00131f3a: ldr.w r0,[r8,#0x0]
  00131f3e: blx 0x000719a4
  00131f42: mov r5,r0
  00131f44: movw r0,#0x4225
  00131f48: cmp r5,#0x0
  00131f4a: it ne
  00131f4c: movw.ne r0,#0x3825
  00131f50: cmp r4,#0x50
  00131f52: it eq
  00131f54: movw.eq r0,#0x3821
  00131f58: str r0,[sp,#0x1c]
  00131f5a: sub.w r0,r4,#0x50
  00131f5e: clz r0,r0
  00131f62: movw r9,#0x4224
  00131f66: cmp r5,#0x0
  00131f68: it ne
  00131f6a: movw.ne r9,#0x3824
  00131f6e: cmp r4,#0x50
  00131f70: lsr.w r0,r0,#0x5
  00131f74: it eq
  00131f76: movw.eq r9,#0x3820
  00131f7a: orrs r0,r5
  00131f7c: movw r11,#0x4228
  00131f80: str r0,[sp,#0x18]
  00131f82: it ne
  00131f84: movw.ne r11,#0x422a
  00131f88: movs r0,#0xc0
  00131f8a: blx 0x0006eb24
  00131f8e: mov r6,r0
  00131f90: ldr r0,[0x00132320]
  00131f92: movw r1,#0x4223
  00131f96: cmp r5,#0x0
  00131f98: add r0,pc
  00131f9a: it ne
  00131f9c: movw.ne r1,#0x3823
  00131fa0: cmp r4,#0x50
  00131fa2: ldr.w r8,[r0,#0x0]
  00131fa6: ldr.w r2,[r8,#0x0]
  00131faa: it eq
  00131fac: movw.eq r1,#0x381f
  00131fb0: mov r0,r6
  00131fb2: movs r3,#0x0
  00131fb4: blx 0x0007207c
  00131fb8: ldr r0,[sp,#0x28]
  00131fba: mov.w r1,#0xffffffff
  00131fbe: str r6,[r0,#0x0]
  00131fc0: ldr.w r0,[r8,#0x0]
  00131fc4: str r1,[sp,#0x34]
  00131fc6: add r1,sp,#0x34
  00131fc8: blx 0x000720ac
  00131fcc: ldr r6,[sp,#0x20]
  00131fce: ldr r1,[sp,#0x34]
  00131fd0: ldr.w r0,[r8,#0x0]
  00131fd4: uxth.w r2,r11
  00131fd8: movs r3,#0x0
  00131fda: blx 0x0007303c
  00131fde: ldr r0,[sp,#0x28]
  00131fe0: ldr r1,[sp,#0x34]
  00131fe2: ldr r0,[r0,#0x0]
  00131fe4: blx 0x000720e8
  00131fe8: ldr r0,[sp,#0x34]
  00131fea: mov.w r1,#0xffffffff
  00131fee: str.w r0,[r10,#0x144]
  00131ff2: ldr.w r0,[r8,#0x0]
  00131ff6: str r1,[sp,#0x44]
  00131ff8: add r1,sp,#0x44
  00131ffa: blx 0x000720ac
  00131ffe: ldr r1,[sp,#0x44]
  00132000: ldr.w r0,[r8,#0x0]
  00132004: uxth.w r2,r9
  00132008: movs r3,#0x0
  0013200a: blx 0x0007303c
  0013200e: ldr r0,[sp,#0x28]
  00132010: ldr r1,[sp,#0x44]
  00132012: ldr r0,[r0,#0x0]
  00132014: blx 0x000720e8
  00132018: ldr.w r0,[r8,#0x0]
  0013201c: mov.w r1,#0xffffffff
  00132020: str r1,[sp,#0x40]
  00132022: add r1,sp,#0x40
  00132024: blx 0x000720ac
  00132028: ldrd r11,r9,[sp,#0x24]
  0013202c: ldr r1,[sp,#0x40]
  0013202e: ldr.w r0,[r8,#0x0]
  00132032: ldr r2,[sp,#0x1c]
  00132034: movs r3,#0x0
  00132036: uxth r2,r2
  00132038: blx 0x0007303c
  0013203c: ldr r1,[sp,#0x40]
  0013203e: ldr.w r0,[r9,#0x0]
  00132042: blx 0x000720e8
  00132046: ldr r0,[sp,#0x18]
  00132048: cmp r0,#0x0
  0013204a: beq.w 0x00132452
  0013204e: ldr.w r0,[r9,#0x0]
  00132052: ldr r1,[r0,#0xc]
  00132054: ldr.w r0,[r8,#0x0]
  00132058: blx 0x00072088
  0013205c: movs r1,#0x0
  0013205e: movs r2,#0x0
  00132060: blx 0x0006fd18
  00132064: ldr r1,[sp,#0x44]
  00132066: ldr.w r0,[r8,#0x0]
  0013206a: blx 0x00072088
  0013206e: movs r1,#0x0
  00132070: movs r2,#0x0
  00132072: blx 0x0006fd18
  00132076: ldr r1,[sp,#0x40]
  00132078: ldr.w r0,[r8,#0x0]
  0013207c: blx 0x00072088
  00132080: movs r1,#0x0
  00132082: movs r2,#0x0
  00132084: blx 0x0006fd18
  00132088: cbz r6,0x00132094
  0013208a: mov r0,r6
  0013208c: blx 0x00070204
  00132090: blx 0x0006eb48
  00132094: movs r0,#0x1
  00132096: blx 0x0006eb24
  0013209a: mov r4,r0
  0013209c: blx 0x00072124
  001320a0: mov r0,r4
  001320a2: movw r1,#0x3ed
  001320a6: blx 0x000743bc
  001320aa: mov r6,r0
  001320ac: mov r0,r4
  001320ae: blx 0x0007213c
  001320b2: blx 0x0006eb48
  001320b6: b 0x0013222e
  001320b8: orr r0,r4,#0x2
  001320bc: cmp r0,#0x2
  001320be: bne 0x0013212e
  001320c0: movs r0,#0xc0
  001320c2: blx 0x0006eb24
  001320c6: mov r6,r0
  001320c8: ldr r0,[0x00132324]
  001320ca: add r0,pc
  001320cc: ldr.w r8,[r0,#0x0]
  001320d0: movw r0,#0x5528
  001320d4: add r0,r5
  001320d6: uxth r1,r0
  001320d8: ldr.w r2,[r8,#0x0]
  001320dc: mov r0,r6
  001320de: movs r3,#0x0
  001320e0: blx 0x0007207c
  001320e4: ldr r1,[r6,#0xc]
  001320e6: ldr.w r0,[r9,#0x0]
  001320ea: blx 0x000720e8
  001320ee: movs r0,#0xc0
  001320f0: blx 0x0006eb24
  001320f4: mov r4,r0
  001320f6: ldrh.w r0,[r10,#0x148]
  001320fa: movw r1,#0x55f0
  001320fe: ldr.w r2,[r8,#0x0]
  00132102: add r0,r1
  00132104: uxth r1,r0
  00132106: mov r0,r4
  00132108: movs r3,#0x0
  0013210a: blx 0x0007207c
  0013210e: ldr r1,[r4,#0xc]
  00132110: ldr.w r0,[r9,#0x0]
  00132114: blx 0x000720e8
  00132118: mov r0,r6
  0013211a: blx 0x00071fc8
  0013211e: blx 0x0006eb48
  00132122: mov r0,r4
  00132124: blx 0x00071fc8
  00132128: blx 0x0006eb48
  0013212c: b 0x0013222c
  0013212e: cmp r4,#0x3
  00132130: bne 0x0013222c
  00132132: movs r0,#0xc0
  00132134: mov r4,r9
  00132136: blx 0x0006eb24
  0013213a: mov r9,r0
  0013213c: ldr r0,[0x00132328]
  0013213e: add r0,pc
  00132140: ldr r6,[r0,#0x0]
  00132142: movw r0,#0x5528
  00132146: add r0,r5
  00132148: uxth r1,r0
  0013214a: ldr r2,[r6,#0x0]
  0013214c: mov r0,r9
  0013214e: movs r3,#0x0
  00132150: blx 0x0007207c
  00132154: ldr.w r1,[r9,#0xc]
  00132158: ldr r0,[r4,#0x0]
  0013215a: blx 0x000720e8
  0013215e: movs r0,#0xc0
  00132160: blx 0x0006eb24
  00132164: mov r8,r0
  00132166: ldrh.w r0,[r10,#0x148]
  0013216a: movw r1,#0x55f0
  0013216e: ldr r2,[r6,#0x0]
  00132170: add r0,r1
  00132172: uxth r1,r0
  00132174: mov r0,r8
  00132176: movs r3,#0x0
  00132178: blx 0x0007207c
  0013217c: ldr.w r1,[r8,#0xc]
  00132180: ldr r0,[r4,#0x0]
  00132182: blx 0x000720e8
  00132186: ldr.w r0,[r10,#0x148]
  0013218a: cmp r0,#0x6c
  0013218c: bne 0x00132216
  0013218e: movs r0,#0xc0
  00132190: blx 0x0006eb24
  00132194: mov r5,r0
  00132196: ldr r2,[r6,#0x0]
  00132198: movw r1,#0x5974
  0013219c: movs r3,#0x0
  0013219e: blx 0x0007207c
  001321a2: ldr r1,[r5,#0xc]
  001321a4: ldr r0,[r4,#0x0]
  001321a6: blx 0x000720e8
  001321aa: ldr r0,[r5,#0xc]
  001321ac: str.w r0,[r10,#0x164]
  001321b0: movs r0,#0xc0
  001321b2: blx 0x0006eb24
  001321b6: mov r5,r0
  001321b8: ldr r2,[r6,#0x0]
  001321ba: movw r1,#0x5975
  001321be: movs r3,#0x0
  001321c0: blx 0x0007207c
  001321c4: ldr r1,[r5,#0xc]
  001321c6: ldr r0,[r4,#0x0]
  001321c8: blx 0x000720e8
  001321cc: ldr r0,[r5,#0xc]
  001321ce: str.w r0,[r10,#0x168]
  001321d2: movs r0,#0xc0
  001321d4: blx 0x0006eb24
  001321d8: mov r5,r0
  001321da: ldr r2,[r6,#0x0]
  001321dc: movw r1,#0x5976
  001321e0: movs r3,#0x0
  001321e2: blx 0x0007207c
  001321e6: ldr r1,[r5,#0xc]
  001321e8: ldr r0,[r4,#0x0]
  001321ea: blx 0x000720e8
  001321ee: ldr r0,[r5,#0xc]
  001321f0: str.w r0,[r10,#0x16c]
  001321f4: movs r0,#0xc0
  001321f6: blx 0x0006eb24
  001321fa: mov r5,r0
  001321fc: ldr r2,[r6,#0x0]
  001321fe: movw r1,#0x5977
  00132202: movs r3,#0x0
  00132204: blx 0x0007207c
  00132208: ldr r1,[r5,#0xc]
  0013220a: ldr r0,[r4,#0x0]
  0013220c: blx 0x000720e8
  00132210: ldr r0,[r5,#0xc]
  00132212: str.w r0,[r10,#0x170]
  00132216: mov r0,r9
  00132218: blx 0x00071fc8
  0013221c: blx 0x0006eb48
  00132220: mov r0,r8
  00132222: blx 0x00071fc8
  00132226: blx 0x0006eb48
  0013222a: mov r9,r4
  0013222c: ldr r6,[sp,#0x20]
  0013222e: cmp r6,#0x0
  00132230: beq.w 0x001323f8
  00132234: ldr r0,[r6,#0x4]
  00132236: str.w r10,[sp,#0x2c]
  0013223a: mov r10,r6
  0013223c: ldr r5,[r0,#0x0]
  0013223e: movs r0,#0x0
  00132240: strd r0,r0,[sp,#0x34]
  00132244: str r0,[sp,#0x3c]
  00132246: movs r0,#0xc
  00132248: blx 0x0006eb24
  0013224c: mov r4,r0
  0013224e: blx 0x000730c0
  00132252: ldr r0,[sp,#0x2c]
  00132254: str.w r4,[r0,#0x130]
  00132258: mov r0,r5
  0013225a: mov r1,r4
  0013225c: blx 0x000730cc
  00132260: strd r11,r9,[sp,#0x24]
  00132264: add.w r9,sp,#0x34
  00132268: movs r6,#0x0
  0013226a: mov.w r8,#0x1
  0013226e: mov.w r11,#0x0
  00132272: mov r4,r10
  00132274: b 0x001323da
  00132276: ldr r0,[r4,#0x4]
  00132278: add.w r1,r8,#0x1
  0013227c: ldr.w r2,[r0,r8,lsl #0x2]
  00132280: cmp r2,#0x1
  00132282: beq 0x001322c6
  00132284: cmp r2,#0x0
  00132286: bne 0x0013231c
  00132288: add.w r2,r0,r8, lsl #0x2
  0013228c: ldr.w r0,[r0,r1,lsl #0x2]
  00132290: vldr.32 s0,[r2,#0x8]
  00132294: vldr.32 s2,[r2,#0xc]
  00132298: rsbs r0,r0
  0013229a: vldr.32 s4,[r2,#0x10]
  0013229e: vcvt.f32.s32 s4,s4
  001322a2: vcvt.f32.s32 s18,s2
  001322a6: vcvt.f32.s32 s16,s0
  001322aa: vmov s0,r0
  001322ae: vcvt.f32.s32 s20,s0
  001322b2: ldr r0,[sp,#0x30]
  001322b4: cmp r0,#0x0
  001322b6: vstr.32 s4,[sp,#0x34]
  001322ba: beq 0x0013232c
  001322bc: ldr r1,[0x00132574]
  001322be: mov r0,r9
  001322c0: blx 0x00072628
  001322c4: b 0x00132336
  001322c6: add.w r2,r0,r8, lsl #0x2
  001322ca: vldr.32 s4,[r2,#0x10]
  001322ce: vldr.32 s0,[r2,#0x8]
  001322d2: vldr.32 s2,[r2,#0xc]
  001322d6: vcvt.f32.s32 s4,s4
  001322da: vcvt.f32.s32 s16,s2
  001322de: ldr.w r0,[r0,r1,lsl #0x2]
  001322e2: vcvt.f32.s32 s18,s0
  001322e6: rsbs r0,r0
  001322e8: vstr.32 s4,[sp,#0x34]
  001322ec: vmov s2,r0
  001322f0: vldr.32 s0,[r2,#0x14]
  001322f4: vcvt.f32.s32 s0,s0
  001322f8: vstr.32 s0,[sp,#0x3c]
  001322fc: vldr.32 s0,[r2,#0x18]
  00132300: vcvt.f32.s32 s0,s0
  00132304: vcvt.f32.s32 s20,s2
  00132308: ldr r0,[sp,#0x30]
  0013230a: cmp r0,#0x0
  0013230c: vstr.32 s0,[sp,#0x38]
  00132310: beq 0x00132378
  00132312: ldr r1,[0x00132570]
  00132314: mov r0,r9
  00132316: blx 0x00072628
  0013231a: b 0x00132382
  0013231c: mov r8,r1
  0013231e: b 0x001323d6
  0013232c: mov r0,r9
  0013232e: mov.w r1,#0x3f000000
  00132332: blx 0x00072628
  00132336: vldr.32 s0,[sp,#0x34]
  0013233a: vcmpe.f32 s0,#0
  0013233e: vmrs apsr,fpscr
  00132342: bpl 0x0013234c
  00132344: ldr r1,[0x00132578]
  00132346: mov r0,r9
  00132348: blx 0x00072628
  0013234c: movs r0,#0x48
  0013234e: blx 0x0006eb24
  00132352: vmov r1,s20
  00132356: mov r4,r0
  00132358: vmov r2,s18
  0013235c: vldr.32 s0,[sp,#0x34]
  00132360: vmov r3,s16
  00132364: vstr.32 s0,[sp,#0xc]
  00132368: strd r6,r6,[sp,#0x0]
  0013236c: str r6,[sp,#0x8]
  0013236e: blx 0x000730e4
  00132372: add.w r8,r8,#0x5
  00132376: b 0x001323c8
  00132378: mov r0,r9
  0013237a: mov.w r1,#0x3f800000
  0013237e: blx 0x00072628
  00132382: movs r0,#0x2c
  00132384: blx 0x0006eb24
  00132388: vmov r1,s20
  0013238c: vldr.32 s4,[sp,#0x3c]
  00132390: mov r4,r0
  00132392: vmov r2,s16
  00132396: vmov r3,s18
  0013239a: vldr.32 s0,[sp,#0x34]
  0013239e: vldr.32 s2,[sp,#0x38]
  001323a2: vadd.f32 s4,s4,s4
  001323a6: vadd.f32 s2,s2,s2
  001323aa: strd r6,r6,[sp,#0x0]
  001323ae: vadd.f32 s0,s0,s0
  001323b2: str r6,[sp,#0x8]
  001323b4: vstr.32 s4,[sp,#0x14]
  001323b8: vstr.32 s2,[sp,#0x10]
  001323bc: vstr.32 s0,[sp,#0xc]
  001323c0: blx 0x00074350
  001323c4: add.w r8,r8,#0x7
  001323c8: ldr r0,[sp,#0x2c]
  001323ca: ldr.w r0,[r0,#0x130]
  001323ce: ldr r0,[r0,#0x4]
  001323d0: str.w r4,[r0,r11,lsl #0x2]
  001323d4: mov r4,r10
  001323d6: add.w r11,r11,#0x1
  001323da: cmp r11,r5
  001323dc: blt.w 0x00132276
  001323e0: mov r0,r4
  001323e2: blx 0x0007369c
  001323e6: mov r0,r4
  001323e8: blx 0x00070204
  001323ec: blx 0x0006eb48
  001323f0: ldrd r11,r9,[sp,#0x24]
  001323f4: ldr.w r10,[sp,#0x2c]
  001323f8: ldr.w r0,[r10,#0x140]
  001323fc: movs r1,#0x1
  001323fe: ldr r2,[0x0013257c]
  00132400: strb.w r1,[r10,#0x71]
  00132404: movs r1,#0x0
  00132406: movs r3,#0x0
  00132408: blx 0x00073054
  0013240c: ldr r0,[0x00132584]
  0013240e: ldr.w r1,[r9,#0x0]
  00132412: add r0,pc
  00132414: ldr r0,[r0,#0x0]
  00132416: ldr r1,[r1,#0xc]
  00132418: ldr r0,[r0,#0x0]
  0013241a: blx 0x00072088
  0013241e: vldr.32 s0,[pc,#0x160]
  00132422: vldr.32 s2,[r0,#0xe0]
  00132426: vadd.f32 s0,s2,s0
  0013242a: vcvt.s32.f32 s0,s0
  0013242e: vstr.32 s0,[r10,#0x154]
  00132432: ldr r0,[sp,#0x54]
  00132434: ldr.w r1,[r11,#0x0]
  00132438: subs r0,r1,r0
  0013243a: itttt eq
  0013243c: mov.eq r0,r10
  0013243e: add.eq sp,#0x58
  00132440: vpop.eq {d8,d9,d10}
  00132444: add.eq sp,#0x4
  00132446: itt eq
  00132448: pop.eq.w {r8,r9,r10,r11}
  0013244c: pop.eq {r4,r5,r6,r7,pc}
  0013244e: blx 0x0006e824
  00132452: ldr r0,[0x0013256c]
  00132454: movw r1,#0x4226
  00132458: strh.w r1,[sp,#0x4c]
  0013245c: str r0,[sp,#0x50]
  0013245e: ldr.w r0,[r9,#0x0]
  00132462: add r1,sp,#0x4c
  00132464: add r2,sp,#0x50
  00132466: movs r3,#0x1
  00132468: blx 0x00073d5c
  0013246c: movw r0,#0x4227
  00132470: strh r0,[r7,#-0x46]
  00132474: ldr.w r0,[r9,#0x0]
  00132478: sub.w r1,r7,#0x46
  0013247c: blx 0x00073d68
  00132480: b 0x0013222e
```
