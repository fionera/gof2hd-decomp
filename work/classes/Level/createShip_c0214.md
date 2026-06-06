# Level::createShip
elf 0xc0214 body 2190
Sig: undefined __thiscall createShip(Level * this, int param_1, int param_2, int param_3, Waypoint * param_4, bool param_5, bool param_6)

## decompile
```c

/* Level::createShip(int, int, int, Waypoint*, bool, bool) */

PlayerFixedObject * __thiscall
Level::createShip(Level *this,int param_1,int param_2,int param_3,Waypoint *param_4,bool param_5,
                 bool param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  Player *this_00;
  Array<BoundingVolume*> *this_01;
  void *pvVar9;
  BoundingAAB *pBVar10;
  undefined4 uVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  AEGeometry *pAVar15;
  undefined4 *puVar16;
  code *pcVar17;
  PlayerFixedObject *this_02;
  uint in_fpscr;
  float fVar18;
  float extraout_s1;
  undefined8 extraout_d0;
  undefined8 extraout_d0_00;
  undefined8 extraout_d0_01;
  undefined8 extraout_d0_02;
  undefined8 extraout_d0_03;
  undefined8 extraout_d0_04;
  undefined8 extraout_d0_05;
  undefined8 extraout_d0_06;
  undefined8 extraout_d0_07;
  undefined8 extraout_d0_08;
  undefined8 extraout_d0_09;
  float extraout_s1_00;
  undefined8 extraout_d0_10;
  undefined8 extraout_d0_11;
  undefined8 extraout_d0_12;
  undefined8 extraout_d0_13;
  undefined8 extraout_d0_14;
  undefined8 extraout_d0_15;
  undefined8 extraout_d0_16;
  undefined8 extraout_d0_17;
  undefined8 extraout_d0_18;
  undefined8 extraout_d0_19;
  undefined8 extraout_d0_20;
  undefined8 extraout_d0_21;
  undefined8 extraout_d0_22;
  float fVar19;
  float extraout_s2;
  undefined8 extraout_d1;
  undefined8 extraout_d1_00;
  undefined8 extraout_d1_01;
  undefined8 extraout_d1_02;
  undefined8 extraout_d1_03;
  undefined8 extraout_d1_04;
  undefined8 extraout_d1_05;
  undefined8 extraout_d1_06;
  undefined8 extraout_d1_07;
  undefined8 extraout_d1_08;
  undefined8 extraout_d1_09;
  undefined8 extraout_d1_10;
  undefined8 extraout_d1_11;
  undefined8 extraout_d1_12;
  undefined8 extraout_d1_13;
  undefined8 extraout_d1_14;
  undefined8 extraout_d1_15;
  undefined8 extraout_d1_16;
  undefined8 extraout_d1_17;
  undefined8 extraout_d1_18;
  undefined8 extraout_d1_19;
  undefined8 extraout_d1_20;
  undefined8 extraout_d1_21;
  undefined8 extraout_d1_22;
  undefined8 extraout_d2;
  undefined8 extraout_d2_00;
  undefined8 extraout_d2_01;
  undefined8 extraout_d2_02;
  undefined8 extraout_d2_03;
  undefined8 extraout_d2_04;
  undefined8 extraout_d2_05;
  undefined8 extraout_d2_06;
  undefined8 extraout_d2_07;
  undefined8 extraout_d2_08;
  undefined8 extraout_d2_09;
  undefined8 extraout_d2_10;
  undefined8 extraout_d2_11;
  undefined8 extraout_d2_12;
  undefined8 extraout_d2_13;
  undefined8 extraout_d2_14;
  undefined8 extraout_d2_15;
  undefined8 extraout_d2_16;
  undefined8 extraout_d2_17;
  undefined8 extraout_d2_18;
  undefined8 extraout_d2_19;
  undefined8 extraout_d2_20;
  undefined8 extraout_d2_21;
  undefined8 extraout_d2_22;
  undefined8 extraout_d3;
  undefined8 extraout_d3_00;
  undefined8 extraout_d3_01;
  undefined8 extraout_d3_02;
  undefined8 extraout_d3_03;
  undefined8 extraout_d3_04;
  undefined8 extraout_d3_05;
  undefined8 extraout_d3_06;
  undefined8 extraout_d3_07;
  undefined8 extraout_d3_08;
  undefined8 extraout_d3_09;
  undefined8 extraout_d3_10;
  undefined8 extraout_d3_11;
  undefined8 extraout_d3_12;
  undefined8 extraout_d3_13;
  undefined8 extraout_d3_14;
  undefined8 extraout_d3_15;
  undefined8 extraout_d3_16;
  undefined8 extraout_d3_17;
  undefined8 extraout_d3_18;
  undefined8 extraout_d3_19;
  undefined8 extraout_d3_20;
  undefined8 extraout_d3_21;
  undefined8 extraout_d3_22;
  float extraout_s8;
  float extraout_s8_00;
  float extraout_s8_01;
  float extraout_s8_02;
  float extraout_s8_03;
  float extraout_s8_04;
  float extraout_s8_05;
  float extraout_s8_06;
  float extraout_s8_07;
  float extraout_s8_08;
  float extraout_s8_09;
  float extraout_s8_10;
  float extraout_s8_11;
  float extraout_s8_12;
  float extraout_s8_13;
  float extraout_s8_14;
  float extraout_s8_15;
  float extraout_s8_16;
  float extraout_s8_17;
  float extraout_s8_18;
  float extraout_s8_19;
  float extraout_s8_20;
  float extraout_s8_21;
  float extraout_s8_22;
  undefined3 in_stack_00000005;
  int local_54;
  
  iVar1 = Status::getCurrentCampaignMission();
  if (param_4 == (Waypoint *)0x0) {
    iVar12 = 0;
    local_54 = 0;
    iVar2 = 0;
  }
  else {
    local_54 = *(int *)(param_4 + 0x124);
    iVar12 = *(int *)(param_4 + 0x128);
    iVar2 = *(int *)(param_4 + 300);
  }
  puVar16 = *(undefined4 **)(DAT_000d0468 + 0xd0262);
  pcVar17 = *(code **)(DAT_000d046c + 0xd0264);
  iVar3 = (*pcVar17)(*puVar16,40000);
  iVar4 = (*pcVar17)(*puVar16,40000);
  iVar5 = (*pcVar17)(*puVar16,40000);
  iVar6 = Status::getLevel();
  if (iVar6 < 0x15) {
    iVar6 = Status::getLevel();
  }
  else {
    iVar6 = 0x14;
  }
  iVar7 = Status::gameWon();
  iVar13 = iVar1 << 2;
  if (iVar7 != 0) {
    iVar13 = 0xb4;
  }
  iVar6 = iVar13 + iVar6 * 0xe + 0x14;
  if (param_3 == 0x33) {
    fVar19 = (float)VectorSignedToFloat(iVar6,(byte)(in_fpscr >> 0x16) & 3);
    fVar19 = fVar19 * DAT_000d0470;
LAB_000d02f4:
    iVar6 = (int)fVar19;
  }
  else {
    if (param_3 == 0x31) {
      fVar18 = 17.0;
LAB_000d02e8:
      fVar19 = (float)VectorSignedToFloat(iVar6,(byte)(in_fpscr >> 0x16) & 3);
      fVar19 = fVar19 * fVar18;
      goto LAB_000d02f4;
    }
    if (param_3 == 0x2c) {
      fVar18 = 2.25;
      goto LAB_000d02e8;
    }
  }
  if ((iVar1 - 0x31U < 8) && ((0x8fU >> (iVar1 - 0x31U & 0xff) & 1) != 0)) {
    iVar6 = 0x10e;
  }
  iVar7 = Status::getLevel();
  if (iVar7 < 0x15) {
    iVar7 = Status::getLevel();
  }
  else {
    iVar7 = 0x14;
  }
  iVar7 = iVar7 * 5 + 0x28;
  if (param_2 == 1) {
    iVar13 = 5;
    if (param_3 == 0xe) {
      iVar13 = 0x19;
    }
    iVar7 = iVar7 * 3;
    iVar6 = iVar13 * iVar6;
    iVar13 = 45000;
  }
  else {
    iVar13 = 15000;
  }
  fVar19 = (float)VectorSignedToFloat(iVar6,(byte)(in_fpscr >> 0x16) & 3);
  iVar6 = (int)(fVar19 + (*(float *)(*(int *)(DAT_000d0474 + 0xd0368) + 0x2c) + -0.5) * fVar19);
  if (iVar1 == 0x9a) {
    uVar8 = Status::inAlienOrbit();
    iVar6 = iVar6 << (uVar8 & param_1 == 9);
  }
  iVar1 = Status::hardCoreMode();
  this_00 = operator_new(0x114);
  iVar14 = 1000;
  if (iVar1 != 0) {
    iVar14 = 0x28a;
  }
  Player::Player(this_00,iVar14,iVar6,1,1,0);
  iVar3 = local_54 + iVar3 + -20000;
  iVar1 = iVar12 + iVar4 + -20000;
  fVar19 = (float)(iVar2 + iVar5 + -20000);
  Player::setEmpData(this_00,iVar7,iVar13);
  this_02 = (PlayerFixedObject *)0x0;
  if (param_2 == 0) {
    this_02 = operator_new(0x2f0);
    fVar18 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
    fVar19 = (float)VectorSignedToFloat(fVar19,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
    PlayerFighter::PlayerFighter
              ((PlayerFighter *)this_02,param_3,param_1,this_00,(AEGeometry *)0x0,fVar18,
               extraout_s1_00,fVar19,SUB41(fVar18,0));
    uVar11 = Globals::getShipGroup
                       ((Globals *)**(undefined4 **)(DAT_000d0838 + 0xd0708),param_3,param_1,param_6
                       );
    (**(code **)(*(int *)this_02 + 8))(this_02,uVar11,param_3,_param_5);
    if (*(int *)(this + 0xc0) != 1 && *(int *)(this + 0xc0) != 0x17) {
      pAVar15 = *(AEGeometry **)(this_02 + 0xc);
      if (pAVar15 == (AEGeometry *)0x0) {
        pAVar15 = *(AEGeometry **)(this_02 + 8);
      }
      LODManager::addObject(*(LODManager **)this,pAVar15);
    }
    if (param_3 != 0x2c) {
      if (param_3 == 0x33) {
        *(PlayerFighter *)(this_02 + 0x25) = (PlayerFighter)0x0;
        goto LAB_000d0b70;
      }
      if (param_3 != 0x31) goto LAB_000d0b70;
    }
    if (*(Array<int> **)(this_02 + 0x50) != (Array<int> *)0x0) {
      pvVar9 = (void *)Array<int>::~Array(*(Array<int> **)(this_02 + 0x50));
      operator_delete(pvVar9);
    }
    *(undefined4 *)(this_02 + 0x50) = 0;
    goto LAB_000d0b70;
  }
  if (param_2 != 1) goto LAB_000d0b70;
  this_02 = operator_new(0x1bc);
  VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
  VectorSignedToFloat(fVar19,(byte)(in_fpscr >> 0x16) & 3);
  PlayerFixedObject::PlayerFixedObject
            (this_02,param_3,param_1,this_00,(AEGeometry *)0x0,fVar19,extraout_s1,extraout_s2);
  this_01 = operator_new(0xc);
  Array<BoundingVolume*>::Array(this_01);
  switch(param_1) {
  case 0:
    if (param_3 != 0xe) {
      ArraySetLength<BoundingVolume*>(3,(Array *)this_01);
      pBVar10 = operator_new(0x2c);
      BoundingAAB::BoundingAAB
                (pBVar10,(float)extraout_d0_20,(float)((ulonglong)extraout_d0_20 >> 0x20),
                 (float)extraout_d1_20,(float)((ulonglong)extraout_d1_20 >> 0x20),
                 (float)extraout_d2_20,(float)((ulonglong)extraout_d2_20 >> 0x20),
                 (float)extraout_d3_20,(float)((ulonglong)extraout_d3_20 >> 0x20),extraout_s8_20);
      **(undefined4 **)(this_01 + 4) = pBVar10;
      pBVar10 = operator_new(0x2c);
      BoundingAAB::BoundingAAB
                (pBVar10,(float)extraout_d0_21,(float)((ulonglong)extraout_d0_21 >> 0x20),
                 (float)extraout_d1_21,(float)((ulonglong)extraout_d1_21 >> 0x20),
                 (float)extraout_d2_21,(float)((ulonglong)extraout_d2_21 >> 0x20),
                 (float)extraout_d3_21,(float)((ulonglong)extraout_d3_21 >> 0x20),extraout_s8_21);
      *(BoundingAAB **)(*(int *)(this_01 + 4) + 4) = pBVar10;
      pBVar10 = operator_new(0x2c);
      BoundingAAB::BoundingAAB
                (pBVar10,(float)extraout_d0_22,(float)((ulonglong)extraout_d0_22 >> 0x20),
                 (float)extraout_d1_22,(float)((ulonglong)extraout_d1_22 >> 0x20),
                 (float)extraout_d2_22,(float)((ulonglong)extraout_d2_22 >> 0x20),
                 (float)extraout_d3_22,(float)((ulonglong)extraout_d3_22 >> 0x20),extraout_s8_22);
      iVar1 = *(int *)(this_01 + 4);
      iVar12 = 0x477e;
      goto LAB_000d0b2a;
    }
    ArraySetLength<BoundingVolume*>(0xb,(Array *)this_01);
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0,(float)((ulonglong)extraout_d0 >> 0x20),(float)extraout_d1
               ,(float)((ulonglong)extraout_d1 >> 0x20),(float)extraout_d2,
               (float)((ulonglong)extraout_d2 >> 0x20),(float)extraout_d3,
               (float)((ulonglong)extraout_d3 >> 0x20),extraout_s8);
    **(undefined4 **)(this_01 + 4) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_00,(float)((ulonglong)extraout_d0_00 >> 0x20),
               (float)extraout_d1_00,(float)((ulonglong)extraout_d1_00 >> 0x20),
               (float)extraout_d2_00,(float)((ulonglong)extraout_d2_00 >> 0x20),
               (float)extraout_d3_00,(float)((ulonglong)extraout_d3_00 >> 0x20),extraout_s8_00);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 4) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_01,(float)((ulonglong)extraout_d0_01 >> 0x20),
               (float)extraout_d1_01,(float)((ulonglong)extraout_d1_01 >> 0x20),
               (float)extraout_d2_01,(float)((ulonglong)extraout_d2_01 >> 0x20),
               (float)extraout_d3_01,(float)((ulonglong)extraout_d3_01 >> 0x20),extraout_s8_01);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 8) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_02,(float)((ulonglong)extraout_d0_02 >> 0x20),
               (float)extraout_d1_02,(float)((ulonglong)extraout_d1_02 >> 0x20),
               (float)extraout_d2_02,(float)((ulonglong)extraout_d2_02 >> 0x20),
               (float)extraout_d3_02,(float)((ulonglong)extraout_d3_02 >> 0x20),extraout_s8_02);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0xc) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_03,(float)((ulonglong)extraout_d0_03 >> 0x20),
               (float)extraout_d1_03,(float)((ulonglong)extraout_d1_03 >> 0x20),
               (float)extraout_d2_03,(float)((ulonglong)extraout_d2_03 >> 0x20),
               (float)extraout_d3_03,(float)((ulonglong)extraout_d3_03 >> 0x20),extraout_s8_03);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0x10) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_04,(float)((ulonglong)extraout_d0_04 >> 0x20),
               (float)extraout_d1_04,(float)((ulonglong)extraout_d1_04 >> 0x20),
               (float)extraout_d2_04,(float)((ulonglong)extraout_d2_04 >> 0x20),
               (float)extraout_d3_04,(float)((ulonglong)extraout_d3_04 >> 0x20),extraout_s8_04);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0x14) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_05,(float)((ulonglong)extraout_d0_05 >> 0x20),
               (float)extraout_d1_05,(float)((ulonglong)extraout_d1_05 >> 0x20),
               (float)extraout_d2_05,(float)((ulonglong)extraout_d2_05 >> 0x20),
               (float)extraout_d3_05,(float)((ulonglong)extraout_d3_05 >> 0x20),extraout_s8_05);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0x18) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_06,(float)((ulonglong)extraout_d0_06 >> 0x20),
               (float)extraout_d1_06,(float)((ulonglong)extraout_d1_06 >> 0x20),
               (float)extraout_d2_06,(float)((ulonglong)extraout_d2_06 >> 0x20),
               (float)extraout_d3_06,(float)((ulonglong)extraout_d3_06 >> 0x20),extraout_s8_06);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0x1c) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_07,(float)((ulonglong)extraout_d0_07 >> 0x20),
               (float)extraout_d1_07,(float)((ulonglong)extraout_d1_07 >> 0x20),
               (float)extraout_d2_07,(float)((ulonglong)extraout_d2_07 >> 0x20),
               (float)extraout_d3_07,(float)((ulonglong)extraout_d3_07 >> 0x20),extraout_s8_07);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0x20) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_08,(float)((ulonglong)extraout_d0_08 >> 0x20),
               (float)extraout_d1_08,(float)((ulonglong)extraout_d1_08 >> 0x20),
               (float)extraout_d2_08,(float)((ulonglong)extraout_d2_08 >> 0x20),
               (float)extraout_d3_08,(float)((ulonglong)extraout_d3_08 >> 0x20),extraout_s8_08);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0x24) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_09,(float)((ulonglong)extraout_d0_09 >> 0x20),
               (float)extraout_d1_09,(float)((ulonglong)extraout_d1_09 >> 0x20),
               (float)extraout_d2_09,(float)((ulonglong)extraout_d2_09 >> 0x20),
               (float)extraout_d3_09,(float)((ulonglong)extraout_d3_09 >> 0x20),extraout_s8_09);
    iVar12 = 0x4780;
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0x28) = pBVar10;
    break;
  case 1:
    ArraySetLength<BoundingVolume*>(5,(Array *)this_01);
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_10,(float)((ulonglong)extraout_d0_10 >> 0x20),
               (float)extraout_d1_10,(float)((ulonglong)extraout_d1_10 >> 0x20),
               (float)extraout_d2_10,(float)((ulonglong)extraout_d2_10 >> 0x20),
               (float)extraout_d3_10,(float)((ulonglong)extraout_d3_10 >> 0x20),extraout_s8_10);
    **(undefined4 **)(this_01 + 4) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_11,(float)((ulonglong)extraout_d0_11 >> 0x20),
               (float)extraout_d1_11,(float)((ulonglong)extraout_d1_11 >> 0x20),
               (float)extraout_d2_11,(float)((ulonglong)extraout_d2_11 >> 0x20),
               (float)extraout_d3_11,(float)((ulonglong)extraout_d3_11 >> 0x20),extraout_s8_11);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 4) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_12,(float)((ulonglong)extraout_d0_12 >> 0x20),
               (float)extraout_d1_12,(float)((ulonglong)extraout_d1_12 >> 0x20),
               (float)extraout_d2_12,(float)((ulonglong)extraout_d2_12 >> 0x20),
               (float)extraout_d3_12,(float)((ulonglong)extraout_d3_12 >> 0x20),extraout_s8_12);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 8) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_13,(float)((ulonglong)extraout_d0_13 >> 0x20),
               (float)extraout_d1_13,(float)((ulonglong)extraout_d1_13 >> 0x20),
               (float)extraout_d2_13,(float)((ulonglong)extraout_d2_13 >> 0x20),
               (float)extraout_d3_13,(float)((ulonglong)extraout_d3_13 >> 0x20),extraout_s8_13);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0xc) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_14,(float)((ulonglong)extraout_d0_14 >> 0x20),
               (float)extraout_d1_14,(float)((ulonglong)extraout_d1_14 >> 0x20),
               (float)extraout_d2_14,(float)((ulonglong)extraout_d2_14 >> 0x20),
               (float)extraout_d3_14,(float)((ulonglong)extraout_d3_14 >> 0x20),extraout_s8_14);
    iVar12 = 0x477f;
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 0x10) = pBVar10;
    break;
  case 2:
    ArraySetLength<BoundingVolume*>(3,(Array *)this_01);
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_15,(float)((ulonglong)extraout_d0_15 >> 0x20),
               (float)extraout_d1_15,(float)((ulonglong)extraout_d1_15 >> 0x20),
               (float)extraout_d2_15,(float)((ulonglong)extraout_d2_15 >> 0x20),
               (float)extraout_d3_15,(float)((ulonglong)extraout_d3_15 >> 0x20),extraout_s8_15);
    **(undefined4 **)(this_01 + 4) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_16,(float)((ulonglong)extraout_d0_16 >> 0x20),
               (float)extraout_d1_16,(float)((ulonglong)extraout_d1_16 >> 0x20),
               (float)extraout_d2_16,(float)((ulonglong)extraout_d2_16 >> 0x20),
               (float)extraout_d3_16,(float)((ulonglong)extraout_d3_16 >> 0x20),extraout_s8_16);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 4) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_17,(float)((ulonglong)extraout_d0_17 >> 0x20),
               (float)extraout_d1_17,(float)((ulonglong)extraout_d1_17 >> 0x20),
               (float)extraout_d2_17,(float)((ulonglong)extraout_d2_17 >> 0x20),
               (float)extraout_d3_17,(float)((ulonglong)extraout_d3_17 >> 0x20),extraout_s8_17);
    iVar1 = *(int *)(this_01 + 4);
    iVar12 = 0x477d;
LAB_000d0b2a:
    *(BoundingAAB **)(iVar1 + 8) = pBVar10;
    break;
  case 3:
    ArraySetLength<BoundingVolume*>(2,(Array *)this_01);
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_18,(float)((ulonglong)extraout_d0_18 >> 0x20),
               (float)extraout_d1_18,(float)((ulonglong)extraout_d1_18 >> 0x20),
               (float)extraout_d2_18,(float)((ulonglong)extraout_d2_18 >> 0x20),
               (float)extraout_d3_18,(float)((ulonglong)extraout_d3_18 >> 0x20),extraout_s8_18);
    **(undefined4 **)(this_01 + 4) = pBVar10;
    pBVar10 = operator_new(0x2c);
    BoundingAAB::BoundingAAB
              (pBVar10,(float)extraout_d0_19,(float)((ulonglong)extraout_d0_19 >> 0x20),
               (float)extraout_d1_19,(float)((ulonglong)extraout_d1_19 >> 0x20),
               (float)extraout_d2_19,(float)((ulonglong)extraout_d2_19 >> 0x20),
               (float)extraout_d3_19,(float)((ulonglong)extraout_d3_19 >> 0x20),extraout_s8_19);
    *(BoundingAAB **)(*(int *)(this_01 + 4) + 4) = pBVar10;
    PlayerFixedObject::setWreckedMeshId(this_02,0x477c);
  default:
    goto switchD_000d0458_default;
  }
  PlayerFixedObject::setWreckedMeshId(this_02,iVar12);
switchD_000d0458_default:
  PlayerFixedObject::setBV(this_02,(Array *)this_01);
  uVar11 = Globals::getShipGroup
                     ((Globals *)**(undefined4 **)(DAT_000d0cdc + 0xd0b48),param_3,param_1,false);
  (**(code **)(*(int *)this_02 + 8))(this_02,uVar11,param_3,0);
  LODManager::addObject(*(LODManager **)this,*(AEGeometry **)(this_02 + 8));
  this_02[0x40] = (PlayerFixedObject)0x1;
LAB_000d0b70:
  (**(code **)(*(int *)this_02 + 0x14))(this_02,this);
  return this_02;
}

```
## target disasm
```
  000d0214: push {r4,r5,r6,r7,lr}
  000d0216: add r7,sp,#0xc
  000d0218: push {r7,r8,r9,r10,r11}
  000d021c: vpush {d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10}
  000d0220: str r1,[sp,#0x3c]
  000d0222: mov r9,r0
  000d0224: ldr r0,[0x000d0464]
  000d0226: mov r11,r3
  000d0228: mov r5,r2
  000d022a: add r0,pc
  000d022c: ldr r4,[r0,#0x0]
  000d022e: ldr r0,[r4,#0x0]
  000d0230: blx 0x00071770
  000d0234: mov r8,r0
  000d0236: ldr r0,[r7,#0x8]
  000d0238: mov r10,r5
  000d023a: cbz r0,0x000d024e
  000d023c: ldr.w r1,[r0,#0x124]
  000d0240: str r1,[sp,#0x2c]
  000d0242: ldr.w r1,[r0,#0x128]
  000d0246: ldr.w r0,[r0,#0x12c]
  000d024a: str r1,[sp,#0x30]
  000d024c: b 0x000d0258
  000d024e: movs r0,#0x0
  000d0250: str r0,[sp,#0x30]
  000d0252: movs r0,#0x0
  000d0254: str r0,[sp,#0x2c]
  000d0256: movs r0,#0x0
  000d0258: str r0,[sp,#0x28]
  000d025a: ldr r0,[0x000d0468]
  000d025c: ldr r1,[0x000d046c]
  000d025e: add r0,pc
  000d0260: add r1,pc
  000d0262: ldr r5,[r0,#0x0]
  000d0264: ldr r6,[r1,#0x0]
  000d0266: movw r1,#0x9c40
  000d026a: ldr r0,[r5,#0x0]
  000d026c: blx r6
  000d026e: str r0,[sp,#0x24]
  000d0270: movw r1,#0x9c40
  000d0274: ldr r0,[r5,#0x0]
  000d0276: blx r6
  000d0278: str r0,[sp,#0x20]
  000d027a: movw r1,#0x9c40
  000d027e: ldr r0,[r5,#0x0]
  000d0280: blx r6
  000d0282: mov r5,r0
  000d0284: ldr r0,[r4,#0x0]
  000d0286: blx 0x00071c5c
  000d028a: cmp r0,#0x14
  000d028c: str r5,[sp,#0x1c]
  000d028e: ble 0x000d0294
  000d0290: movs r5,#0x14
  000d0292: b 0x000d029c
  000d0294: ldr r0,[r4,#0x0]
  000d0296: blx 0x00071c5c
  000d029a: mov r5,r0
  000d029c: ldr r0,[r4,#0x0]
  000d029e: blx 0x00073c48
  000d02a2: cmp r0,#0x0
  000d02a4: lsl.w r1,r8,#0x2
  000d02a8: rsb r0,r5,r5, lsl #0x3
  000d02ac: it ne
  000d02ae: mov.ne r1,#0xb4
  000d02b0: cmp.w r11,#0x33
  000d02b4: mov r6,r10
  000d02b6: add.w r0,r1,r0, lsl #0x1
  000d02ba: add.w r5,r0,#0x14
  000d02be: beq 0x000d02d2
  000d02c0: cmp.w r11,#0x31
  000d02c4: beq 0x000d02e4
  000d02c6: cmp.w r11,#0x2c
  000d02ca: bne 0x000d02fc
  000d02cc: vmov.f32 s0,0x40100000
  000d02d0: b 0x000d02e8
  000d02d2: vmov s0,r5
  000d02d6: vldr.32 s2,[pc,#0x198]
  000d02da: vcvt.f32.s32 s0,s0
  000d02de: vmul.f32 s0,s0,s2
  000d02e2: b 0x000d02f4
  000d02e4: vmov.f32 s0,0x41880000
  000d02e8: vmov s2,r5
  000d02ec: vcvt.f32.s32 s2,s2
  000d02f0: vmul.f32 s0,s2,s0
  000d02f4: vcvt.s32.f32 s0,s0
  000d02f8: vmov r5,s0
  000d02fc: sub.w r0,r8,#0x31
  000d0300: cmp r0,#0x7
  000d0302: bhi 0x000d0314
  000d0304: uxtb r0,r0
  000d0306: movs r1,#0x8f
  000d0308: lsr.w r0,r1,r0
  000d030c: lsls r0,r0,#0x1f
  000d030e: it ne
  000d0310: mov.ne.w r5,#0x10e
  000d0314: ldr r0,[r4,#0x0]
  000d0316: blx 0x00071c5c
  000d031a: cmp r0,#0x14
  000d031c: ble 0x000d0322
  000d031e: movs r0,#0x14
  000d0320: b 0x000d0328
  000d0322: ldr r0,[r4,#0x0]
  000d0324: blx 0x00071c5c
  000d0328: add.w r0,r0,r0, lsl #0x2
  000d032c: cmp r6,#0x1
  000d032e: add.w r10,r0,#0x28
  000d0332: str.w r9,[sp,#0x38]
  000d0336: str.w r11,[sp,#0x34]
  000d033a: bne 0x000d0352
  000d033c: movs r0,#0x5
  000d033e: cmp.w r11,#0xe
  000d0342: it eq
  000d0344: mov.eq r0,#0x19
  000d0346: add.w r10,r10,r10, lsl #0x1
  000d034a: muls r5,r0
  000d034c: movw r9,#0xafc8
  000d0350: b 0x000d0356
  000d0352: movw r9,#0x3a98
  000d0356: ldr r0,[0x000d0474]
  000d0358: vmov.f32 s0,0xbf000000
  000d035c: vmov s4,r5
  000d0360: cmp.w r8,#0x9a
  000d0364: add r0,pc
  000d0366: ldr r0,[r0,#0x0]
  000d0368: vldr.32 s2,[r0,#0x2c]
  000d036c: vcvt.f32.s32 s4,s4
  000d0370: vadd.f32 s0,s2,s0
  000d0374: vmla.f32 s4,s0,s4
  000d0378: vcvt.s32.f32 s0,s4
  000d037c: vmov r11,s0
  000d0380: bne 0x000d0398
  000d0382: ldr r0,[r4,#0x0]
  000d0384: blx 0x000723d0
  000d0388: ldr r1,[sp,#0x3c]
  000d038a: subs r1,#0x9
  000d038c: clz r1,r1
  000d0390: lsrs r1,r1,#0x5
  000d0392: ands r0,r1
  000d0394: lsl.w r11,r11,r0
  000d0398: ldr r0,[r4,#0x0]
  000d039a: blx 0x0007192c
  000d039e: mov r4,r0
  000d03a0: mov.w r0,#0x114
  000d03a4: blx 0x0006eb24
  000d03a8: mov r5,r0
  000d03aa: movs r0,#0x0
  000d03ac: movs r1,#0x1
  000d03ae: strd r1,r0,[sp,#0x0]
  000d03b2: mov.w r1,#0x3e8
  000d03b6: cmp r4,#0x0
  000d03b8: it ne
  000d03ba: movw.ne r1,#0x28a
  000d03be: mov r0,r5
  000d03c0: mov r2,r11
  000d03c2: movs r3,#0x1
  000d03c4: blx 0x00073d80
  000d03c8: ldr r2,[sp,#0x30]
  000d03ca: ldr r3,[sp,#0x20]
  000d03cc: ldr r0,[sp,#0x2c]
  000d03ce: ldr r1,[sp,#0x24]
  000d03d0: add r2,r3
  000d03d2: ldr r3,[sp,#0x28]
  000d03d4: ldr r4,[sp,#0x1c]
  000d03d6: add r0,r1
  000d03d8: movw r1,#0x4e20
  000d03dc: sub.w r11,r0,r1
  000d03e0: add r3,r4
  000d03e2: subs r4,r2,r1
  000d03e4: sub.w r8,r3,r1
  000d03e8: mov r0,r5
  000d03ea: mov r1,r10
  000d03ec: mov r2,r9
  000d03ee: blx 0x00074368
  000d03f2: mov.w r9,#0x0
  000d03f6: cmp r6,#0x0
  000d03f8: beq.w 0x000d06b6
  000d03fc: cmp r6,#0x1
  000d03fe: ldr r6,[sp,#0x38]
  000d0400: bne.w 0x000d0b70
  000d0404: mov.w r0,#0x1bc
  000d0408: blx 0x0006eb24
  000d040c: vmov s0,r11
  000d0410: mov r9,r0
  000d0412: vcvt.f32.s32 s20,s0
  000d0416: vmov s0,r4
  000d041a: vcvt.f32.s32 s18,s0
  000d041e: vmov s0,r8
  000d0422: vcvt.f32.s32 s16,s0
  000d0426: ldr r4,[sp,#0x34]
  000d0428: movs r0,#0x0
  000d042a: ldr r6,[sp,#0x3c]
  000d042c: mov r3,r5
  000d042e: str r0,[sp,#0x0]
  000d0430: mov r0,r9
  000d0432: mov r1,r4
  000d0434: vstr.32 s20,[sp,#0x4]
  000d0438: mov r2,r6
  000d043a: vstr.32 s18,[sp,#0x8]
  000d043e: vstr.32 s16,[sp,#0xc]
  000d0442: blx 0x0007414c
  000d0446: movs r0,#0xc
  000d0448: blx 0x0006eb24
  000d044c: mov r11,r0
  000d044e: blx 0x000730c0
  000d0452: cmp r6,#0x3
  000d0454: bhi.w 0x000d0b34
  000d0458: tbh [pc,r6]
  000d0478: cmp r4,#0xe
  000d047a: bne.w 0x000d0a7e
  000d047e: movs r0,#0xb
  000d0480: mov r1,r11
  000d0482: blx 0x000730cc
  000d0486: movs r0,#0x2c
  000d0488: blx 0x0006eb24
  000d048c: vmov r10,s20
  000d0490: mov r5,r0
  000d0492: vmov r8,s18
  000d0496: ldr r0,[0x000d0770]
  000d0498: vmov r4,s16
  000d049c: ldr r1,[0x000d0774]
  000d049e: ldr r2,[0x000d0778]
  000d04a0: str r2,[sp,#0x14]
  000d04a2: movs r6,#0x0
  000d04a4: ldr r2,[0x000d077c]
  000d04a6: mov r3,r4
  000d04a8: str r0,[sp,#0x10]
  000d04aa: ldr r0,[0x000d0780]
  000d04ac: str r1,[sp,#0xc]
  000d04ae: mov r1,r10
  000d04b0: strd r0,r2,[sp,#0x4]
  000d04b4: mov r0,r5
  000d04b6: mov r2,r8
  000d04b8: str r6,[sp,#0x0]
  000d04ba: blx 0x00074350
  000d04be: ldr.w r0,[r11,#0x4]
  000d04c2: str r5,[r0,#0x0]
  000d04c4: movs r0,#0x2c
  000d04c6: blx 0x0006eb24
  000d04ca: mov r5,r0
  000d04cc: ldr r0,[0x000d0784]
  000d04ce: ldr r1,[0x000d0788]
  000d04d0: ldr r2,[0x000d078c]
  000d04d2: str r0,[sp,#0x10]
  000d04d4: mov r3,r4
  000d04d6: ldr r0,[0x000d0790]
  000d04d8: str r2,[sp,#0x14]
  000d04da: ldr r2,[0x000d0794]
  000d04dc: strd r2,r1,[sp,#0x8]
  000d04e0: mov r1,r10
  000d04e2: strd r6,r0,[sp,#0x0]
  000d04e6: mov r0,r5
  000d04e8: mov r2,r8
  000d04ea: blx 0x00074350
  000d04ee: ldr.w r0,[r11,#0x4]
  000d04f2: str r5,[r0,#0x4]
  000d04f4: movs r0,#0x2c
  000d04f6: blx 0x0006eb24
  000d04fa: mov r5,r0
  000d04fc: ldr r0,[0x000d0798]
  000d04fe: ldr r1,[0x000d079c]
  000d0500: ldr r2,[0x000d07a0]
  000d0502: str r2,[sp,#0x14]
  000d0504: movs r6,#0x0
  000d0506: ldr r2,[0x000d07a4]
  000d0508: mov r3,r4
  000d050a: str r0,[sp,#0x10]
  000d050c: ldr r0,[0x000d07a8]
  000d050e: str r1,[sp,#0xc]
  000d0510: mov r1,r10
  000d0512: strd r0,r2,[sp,#0x4]
  000d0516: mov r0,r5
  000d0518: mov r2,r8
  000d051a: str r6,[sp,#0x0]
  000d051c: blx 0x00074350
  000d0520: ldr.w r0,[r11,#0x4]
  000d0524: str r5,[r0,#0x8]
  000d0526: movs r0,#0x2c
  000d0528: blx 0x0006eb24
  000d052c: mov r5,r0
  000d052e: ldr r0,[0x000d07ac]
  000d0530: ldr r1,[0x000d07b0]
  000d0532: ldr r2,[0x000d07b4]
  000d0534: str r0,[sp,#0x10]
  000d0536: mov r3,r4
  000d0538: ldr r0,[0x000d07b8]
  000d053a: str r2,[sp,#0x14]
  000d053c: ldr r2,[0x000d07bc]
  000d053e: strd r2,r1,[sp,#0x8]
  000d0542: mov r1,r10
  000d0544: strd r6,r0,[sp,#0x0]
  000d0548: mov r0,r5
  000d054a: mov r2,r8
  000d054c: blx 0x00074350
  000d0550: ldr.w r0,[r11,#0x4]
  000d0554: str r5,[r0,#0xc]
  000d0556: movs r0,#0x2c
  000d0558: blx 0x0006eb24
  000d055c: mov r5,r0
  000d055e: ldr r0,[0x000d07c0]
  000d0560: ldr r1,[0x000d07c4]
  000d0562: ldr r2,[0x000d07c8]
  000d0564: str r2,[sp,#0x14]
  000d0566: movs r6,#0x0
  000d0568: ldr r2,[0x000d07cc]
  000d056a: mov r3,r4
  000d056c: str r0,[sp,#0x10]
  000d056e: ldr r0,[0x000d07d0]
  000d0570: str r1,[sp,#0xc]
  000d0572: mov r1,r10
  000d0574: strd r0,r2,[sp,#0x4]
  000d0578: mov r0,r5
  000d057a: mov r2,r8
  000d057c: str r6,[sp,#0x0]
  000d057e: blx 0x00074350
  000d0582: ldr.w r0,[r11,#0x4]
  000d0586: str r5,[r0,#0x10]
  000d0588: movs r0,#0x2c
  000d058a: blx 0x0006eb24
  000d058e: mov r5,r0
  000d0590: ldr r0,[0x000d07d4]
  000d0592: ldr r1,[0x000d07d8]
  000d0594: ldr r2,[0x000d07dc]
  000d0596: str r0,[sp,#0x10]
  000d0598: mov r3,r4
  000d059a: ldr r0,[0x000d07e0]
  000d059c: str r2,[sp,#0x14]
  000d059e: ldr r2,[0x000d07e4]
  000d05a0: strd r2,r1,[sp,#0x8]
  000d05a4: mov r1,r10
  000d05a6: strd r6,r0,[sp,#0x0]
  000d05aa: mov r0,r5
  000d05ac: mov r2,r8
  000d05ae: blx 0x00074350
  000d05b2: ldr.w r0,[r11,#0x4]
  000d05b6: str r5,[r0,#0x14]
  000d05b8: movs r0,#0x2c
  000d05ba: blx 0x0006eb24
  000d05be: mov r5,r0
  000d05c0: ldr r0,[0x000d07e8]
  000d05c2: ldr r1,[0x000d07ec]
  000d05c4: ldr r2,[0x000d07f0]
  000d05c6: str r2,[sp,#0x14]
  000d05c8: movs r6,#0x0
  000d05ca: ldr r2,[0x000d07f4]
  000d05cc: mov r3,r4
  000d05ce: str r0,[sp,#0x10]
  000d05d0: ldr r0,[0x000d07f8]
  000d05d2: str r1,[sp,#0xc]
  000d05d4: mov r1,r10
  000d05d6: strd r0,r2,[sp,#0x4]
  000d05da: mov r0,r5
  000d05dc: mov r2,r8
  000d05de: str r6,[sp,#0x0]
  000d05e0: blx 0x00074350
  000d05e4: ldr.w r0,[r11,#0x4]
  000d05e8: str r5,[r0,#0x18]
  000d05ea: movs r0,#0x2c
  000d05ec: blx 0x0006eb24
  000d05f0: mov r5,r0
  000d05f2: ldr r0,[0x000d07fc]
  000d05f4: ldr r1,[0x000d0800]
  000d05f6: ldr r2,[0x000d0804]
  000d05f8: str r0,[sp,#0x10]
  000d05fa: mov r3,r4
  000d05fc: ldr r0,[0x000d0808]
  000d05fe: str r1,[sp,#0x14]
  000d0600: ldr r1,[0x000d080c]
  000d0602: strd r1,r2,[sp,#0x8]
  000d0606: mov r1,r10
  000d0608: strd r6,r0,[sp,#0x0]
  000d060c: mov r0,r5
  000d060e: mov r2,r8
  000d0610: mov r6,r4
  000d0612: blx 0x00074350
  000d0616: ldr.w r0,[r11,#0x4]
  000d061a: str r5,[r0,#0x1c]
  000d061c: movs r0,#0x2c
  000d061e: blx 0x0006eb24
  000d0622: mov r5,r0
  000d0624: ldr r0,[0x000d0810]
  000d0626: ldr r1,[0x000d0814]
  000d0628: str r1,[sp,#0x14]
  000d062a: add r3,sp,#0x4
  000d062c: ldr r1,[0x000d0818]
  000d062e: movs r4,#0x0
  000d0630: ldr r2,[0x000d0804]
  000d0632: str r0,[sp,#0x10]
  000d0634: ldr r0,[0x000d081c]
  000d0636: stmia r3!,{r0,r1,r2}
  000d0638: mov r0,r5
  000d063a: mov r1,r10
  000d063c: mov r2,r8
  000d063e: mov r3,r6
  000d0640: str r4,[sp,#0x0]
  000d0642: blx 0x00074350
  000d0646: ldr.w r0,[r11,#0x4]
  000d064a: str r5,[r0,#0x20]
  000d064c: movs r0,#0x2c
  000d064e: blx 0x0006eb24
  000d0652: mov r5,r0
  000d0654: ldr r0,[0x000d0820]
  000d0656: ldr r1,[0x000d0824]
  000d0658: ldr r2,[0x000d0828]
  000d065a: str r0,[sp,#0x10]
  000d065c: mov r3,r6
  000d065e: ldr r0,[0x000d082c]
  000d0660: str r2,[sp,#0x14]
  000d0662: ldr r2,[0x000d0830]
  000d0664: strd r2,r1,[sp,#0x8]
  000d0668: mov r1,r10
  000d066a: strd r4,r0,[sp,#0x0]
  000d066e: mov r0,r5
  000d0670: mov r2,r8
  000d0672: blx 0x00074350
  000d0676: ldr.w r0,[r11,#0x4]
  000d067a: str r5,[r0,#0x24]
  000d067c: movs r0,#0x2c
  000d067e: blx 0x0006eb24
  000d0682: mov r5,r0
  000d0684: ldr r1,[0x000d0824]
  000d0686: mov r2,r8
  000d0688: ldr r0,[0x000d0828]
  000d068a: mov r3,r6
  000d068c: str r1,[sp,#0xc]
  000d068e: ldr r1,[0x000d0830]
  000d0690: str r0,[sp,#0x14]
  000d0692: ldr r0,[0x000d0820]
  000d0694: str r0,[sp,#0x10]
  000d0696: ldr r0,[0x000d0834]
  000d0698: strd r0,r1,[sp,#0x4]
  000d069c: movs r0,#0x0
  000d069e: str r0,[sp,#0x0]
  000d06a0: mov r0,r5
  000d06a2: mov r1,r10
  000d06a4: blx 0x00074350
  000d06a8: ldr r4,[sp,#0x34]
  000d06aa: ldr.w r0,[r11,#0x4]
  000d06ae: mov.w r1,#0x4780
  000d06b2: str r5,[r0,#0x28]
  000d06b4: b 0x000d0b2c
  000d06b6: mov.w r0,#0x2f0
  000d06ba: ldr r6,[r7,#0xc]
  000d06bc: blx 0x0006eb24
  000d06c0: vmov s0,r11
  000d06c4: mov r10,r0
  000d06c6: vmov s2,r8
  000d06ca: mov r8,r6
  000d06cc: vmov s4,r4
  000d06d0: vcvt.f32.s32 s0,s0
  000d06d4: vcvt.f32.s32 s2,s2
  000d06d8: vcvt.f32.s32 s4,s4
  000d06dc: str r6,[sp,#0x10]
  000d06de: mov r3,r5
  000d06e0: ldr r4,[sp,#0x34]
  000d06e2: ldr r6,[sp,#0x3c]
  000d06e4: str.w r9,[sp,#0x0]
  000d06e8: mov r1,r4
  000d06ea: vstr.32 s0,[sp,#0x4]
  000d06ee: mov r2,r6
  000d06f0: vstr.32 s4,[sp,#0x8]
  000d06f4: vstr.32 s2,[sp,#0xc]
  000d06f8: blx 0x00074374
  000d06fc: ldr r0,[0x000d0838]
  000d06fe: mov r1,r4
  000d0700: ldr r3,[r7,#0x10]
  000d0702: mov r2,r6
  000d0704: add r0,pc
  000d0706: ldr r0,[r0,#0x0]
  000d0708: ldr r0,[r0,#0x0]
  000d070a: blx 0x000721d8
  000d070e: mov r1,r0
  000d0710: ldr.w r0,[r10,#0x0]
  000d0714: mov r2,r4
  000d0716: mov r3,r8
  000d0718: ldr r5,[r0,#0x8]
  000d071a: mov r0,r10
  000d071c: blx r5
  000d071e: ldr r6,[sp,#0x38]
  000d0720: ldr.w r0,[r6,#0xc0]
  000d0724: cmp r0,#0x1
  000d0726: it ne
  000d0728: cmp.ne r0,#0x17
  000d072a: beq 0x000d073c
  000d072c: ldr.w r1,[r10,#0xc]
  000d0730: ldr r0,[r6,#0x0]
  000d0732: cbnz r1,0x000d0738
  000d0734: ldr.w r1,[r10,#0x8]
  000d0738: blx 0x00073d38
  000d073c: cmp r4,#0x2c
  000d073e: beq 0x000d074e
  000d0740: ldr r0,[sp,#0x34]
  000d0742: cmp r0,#0x33
  000d0744: beq 0x000d0766
  000d0746: cmp r0,#0x31
  000d0748: mov r9,r10
  000d074a: bne.w 0x000d0b70
  000d074e: ldr.w r0,[r10,#0x50]
  000d0752: cbz r0,0x000d075c
  000d0754: blx 0x00070204
  000d0758: blx 0x0006eb48
  000d075c: movs r0,#0x0
  000d075e: mov r9,r10
  000d0760: str.w r0,[r10,#0x50]
  000d0764: b 0x000d0b70
  000d0766: movs r0,#0x0
  000d0768: mov r9,r10
  000d076a: strb.w r0,[r10,#0x25]
  000d076e: b 0x000d0b70
  000d083c: movs r0,#0x5
  000d083e: mov r1,r11
  000d0840: blx 0x000730cc
  000d0844: movs r0,#0x2c
  000d0846: blx 0x0006eb24
  000d084a: vmov r10,s20
  000d084e: mov r5,r0
  000d0850: vmov r6,s18
  000d0854: ldr r0,[0x000d0bdc]
  000d0856: vmov r8,s16
  000d085a: ldr r1,[0x000d0be0]
  000d085c: ldr r2,[0x000d0be4]
  000d085e: str r2,[sp,#0x14]
  000d0860: movs r4,#0x0
  000d0862: ldr r2,[0x000d0be8]
  000d0864: mov r3,r8
  000d0866: str r0,[sp,#0x10]
  000d0868: ldr r0,[0x000d0bec]
  000d086a: str r1,[sp,#0xc]
  000d086c: mov r1,r10
  000d086e: strd r0,r2,[sp,#0x4]
  000d0872: mov r0,r5
  000d0874: mov r2,r6
  000d0876: str r4,[sp,#0x0]
  000d0878: blx 0x00074350
  000d087c: ldr.w r0,[r11,#0x4]
  000d0880: str r5,[r0,#0x0]
  000d0882: movs r0,#0x2c
  000d0884: blx 0x0006eb24
  000d0888: mov r5,r0
  000d088a: ldr r0,[0x000d0bf0]
  000d088c: ldr r1,[0x000d0bf4]
  000d088e: ldr r2,[0x000d0bf8]
  000d0890: str r0,[sp,#0x10]
  000d0892: mov r3,r8
  000d0894: ldr r0,[0x000d0c00]
  000d0896: str r2,[sp,#0x14]
  000d0898: ldr r2,[0x000d0bfc]
  000d089a: strd r2,r1,[sp,#0x8]
  000d089e: mov r1,r10
  000d08a0: strd r4,r0,[sp,#0x0]
  000d08a4: mov r0,r5
  000d08a6: mov r2,r6
  000d08a8: blx 0x00074350
  000d08ac: ldr.w r0,[r11,#0x4]
  000d08b0: str r5,[r0,#0x4]
  000d08b2: movs r0,#0x2c
  000d08b4: blx 0x0006eb24
  000d08b8: mov r5,r0
  000d08ba: ldr r0,[0x000d0c04]
  000d08bc: ldr r1,[0x000d0c08]
  000d08be: ldr r2,[0x000d0c0c]
  000d08c0: str r2,[sp,#0x14]
  000d08c2: movs r4,#0x0
  000d08c4: ldr r2,[0x000d0c10]
  000d08c6: mov r3,r8
  000d08c8: str r0,[sp,#0x10]
  000d08ca: ldr r0,[0x000d0c14]
  000d08cc: str r1,[sp,#0xc]
  000d08ce: mov r1,r10
  000d08d0: strd r0,r2,[sp,#0x4]
  000d08d4: mov r0,r5
  000d08d6: mov r2,r6
  000d08d8: str r4,[sp,#0x0]
  000d08da: blx 0x00074350
  000d08de: ldr.w r0,[r11,#0x4]
  000d08e2: str r5,[r0,#0x8]
  000d08e4: movs r0,#0x2c
  000d08e6: blx 0x0006eb24
  000d08ea: mov r5,r0
  000d08ec: ldr r0,[0x000d0c18]
  000d08ee: ldr r1,[0x000d0c1c]
  000d08f0: ldr r2,[0x000d0c20]
  000d08f2: str r0,[sp,#0x10]
  000d08f4: mov r3,r8
  000d08f6: ldr r0,[0x000d0c28]
  000d08f8: str r2,[sp,#0x14]
  000d08fa: ldr r2,[0x000d0c24]
  000d08fc: strd r2,r1,[sp,#0x8]
  000d0900: mov r1,r10
  000d0902: strd r4,r0,[sp,#0x0]
  000d0906: mov r0,r5
  000d0908: mov r2,r6
  000d090a: blx 0x00074350
  000d090e: ldr.w r0,[r11,#0x4]
  000d0912: str r5,[r0,#0xc]
  000d0914: movs r0,#0x2c
  000d0916: blx 0x0006eb24
  000d091a: mov r5,r0
  000d091c: ldr r0,[0x000d0c2c]
  000d091e: ldr r1,[0x000d0c30]
  000d0920: ldr r2,[0x000d0c34]
  000d0922: str r2,[sp,#0x14]
  000d0924: mov r3,r8
  000d0926: ldr r2,[0x000d0c38]
  000d0928: str r0,[sp,#0x10]
  000d092a: ldr r0,[0x000d0c3c]
  000d092c: str r1,[sp,#0xc]
  000d092e: mov r1,r10
  000d0930: strd r0,r2,[sp,#0x4]
  000d0934: movs r0,#0x0
  000d0936: str r0,[sp,#0x0]
  000d0938: mov r0,r5
  000d093a: mov r2,r6
  000d093c: blx 0x00074350
  000d0940: ldr r4,[sp,#0x34]
  000d0942: ldr.w r0,[r11,#0x4]
  000d0946: movw r1,#0x477f
  000d094a: str r5,[r0,#0x10]
  000d094c: b 0x000d0b2c
  000d094e: movs r0,#0x3
  000d0950: mov r1,r11
  000d0952: blx 0x000730cc
  000d0956: movs r0,#0x2c
  000d0958: blx 0x0006eb24
  000d095c: vmov r10,s20
  000d0960: mov r5,r0
  000d0962: vmov r6,s18
  000d0966: ldr r0,[0x000d0c40]
  000d0968: vmov r8,s16
  000d096c: ldr r1,[0x000d0c44]
  000d096e: ldr r2,[0x000d0c48]
  000d0970: str r2,[sp,#0x14]
  000d0972: movs r4,#0x0
  000d0974: ldr r2,[0x000d0c4c]
  000d0976: mov r3,r8
  000d0978: str r0,[sp,#0x10]
  000d097a: ldr r0,[0x000d0c50]
  000d097c: str r1,[sp,#0xc]
  000d097e: mov r1,r10
  000d0980: strd r0,r2,[sp,#0x4]
  000d0984: mov r0,r5
  000d0986: mov r2,r6
  000d0988: str r4,[sp,#0x0]
  000d098a: blx 0x00074350
  000d098e: ldr.w r0,[r11,#0x4]
  000d0992: str r5,[r0,#0x0]
  000d0994: movs r0,#0x2c
  000d0996: blx 0x0006eb24
  000d099a: mov r5,r0
  000d099c: ldr r0,[0x000d0c54]
  000d099e: ldr r1,[0x000d0c58]
  000d09a0: ldr r2,[0x000d0c5c]
  000d09a2: str r0,[sp,#0x10]
  000d09a4: mov r3,r8
  000d09a6: ldr r0,[0x000d0c64]
  000d09a8: str r2,[sp,#0x14]
  000d09aa: ldr r2,[0x000d0c60]
  000d09ac: strd r2,r1,[sp,#0x8]
  000d09b0: mov r1,r10
  000d09b2: strd r4,r0,[sp,#0x0]
  000d09b6: mov r0,r5
  000d09b8: mov r2,r6
  000d09ba: blx 0x00074350
  000d09be: ldr.w r0,[r11,#0x4]
  000d09c2: str r5,[r0,#0x4]
  000d09c4: movs r0,#0x2c
  000d09c6: blx 0x0006eb24
  000d09ca: mov r5,r0
  000d09cc: ldr r0,[0x000d0c68]
  000d09ce: ldr r1,[0x000d0c6c]
  000d09d0: ldr r2,[0x000d0c70]
  000d09d2: str r2,[sp,#0x10]
  000d09d4: mov r3,r8
  000d09d6: ldr r2,[0x000d0c74]
  000d09d8: str r0,[sp,#0x14]
  000d09da: str r0,[sp,#0xc]
  000d09dc: movs r0,#0x0
  000d09de: strd r2,r1,[sp,#0x4]
  000d09e2: mov r1,r10
  000d09e4: str r0,[sp,#0x0]
  000d09e6: mov r0,r5
  000d09e8: mov r2,r6
  000d09ea: blx 0x00074350
  000d09ee: ldr r4,[sp,#0x34]
  000d09f0: ldr.w r0,[r11,#0x4]
  000d09f4: movw r1,#0x477d
  000d09f8: b 0x000d0b2a
  000d09fa: movs r0,#0x2
  000d09fc: mov r1,r11
  000d09fe: blx 0x000730cc
  000d0a02: movs r0,#0x2c
  000d0a04: blx 0x0006eb24
  000d0a08: vmov r10,s20
  000d0a0c: mov r5,r0
  000d0a0e: vmov r6,s18
  000d0a12: ldr r0,[0x000d0c78]
  000d0a14: vmov r8,s16
  000d0a18: ldr r1,[0x000d0c7c]
  000d0a1a: ldr r2,[0x000d0c80]
  000d0a1c: str r2,[sp,#0x14]
  000d0a1e: movs r4,#0x0
  000d0a20: ldr r2,[0x000d0c84]
  000d0a22: mov r3,r8
  000d0a24: str r0,[sp,#0x10]
  000d0a26: ldr r0,[0x000d0c88]
  000d0a28: str r1,[sp,#0xc]
  000d0a2a: mov r1,r10
  000d0a2c: strd r0,r2,[sp,#0x4]
  000d0a30: mov r0,r5
  000d0a32: mov r2,r6
  000d0a34: str r4,[sp,#0x0]
  000d0a36: blx 0x00074350
  000d0a3a: ldr.w r0,[r11,#0x4]
  000d0a3e: str r5,[r0,#0x0]
  000d0a40: movs r0,#0x2c
  000d0a42: blx 0x0006eb24
  000d0a46: mov r5,r0
  000d0a48: ldr r0,[0x000d0c8c]
  000d0a4a: ldr r1,[0x000d0c90]
  000d0a4c: ldr r2,[0x000d0c94]
  000d0a4e: str r0,[sp,#0x10]
  000d0a50: mov r3,r8
  000d0a52: ldr r0,[0x000d0c9c]
  000d0a54: str r2,[sp,#0x14]
  000d0a56: ldr r2,[0x000d0c98]
  000d0a58: strd r2,r1,[sp,#0x8]
  000d0a5c: mov r1,r10
  000d0a5e: strd r4,r0,[sp,#0x0]
  000d0a62: mov r0,r5
  000d0a64: mov r2,r6
  000d0a66: blx 0x00074350
  000d0a6a: ldr.w r0,[r11,#0x4]
  000d0a6e: movw r1,#0x477c
  000d0a72: str r5,[r0,#0x4]
  000d0a74: mov r0,r9
  000d0a76: blx 0x0007432c
  000d0a7a: ldr r4,[sp,#0x34]
  000d0a7c: b 0x000d0b32
  000d0a7e: movs r0,#0x3
  000d0a80: mov r1,r11
  000d0a82: blx 0x000730cc
  000d0a86: movs r0,#0x2c
  000d0a88: blx 0x0006eb24
  000d0a8c: vmov r10,s20
  000d0a90: mov r5,r0
  000d0a92: vmov r6,s18
  000d0a96: ldr r0,[0x000d0ca0]
  000d0a98: vmov r8,s16
  000d0a9c: ldr r1,[0x000d0ca4]
  000d0a9e: ldr r2,[0x000d0ca8]
  000d0aa0: str r2,[sp,#0x14]
  000d0aa2: movs r4,#0x0
  000d0aa4: ldr r2,[0x000d0cac]
  000d0aa6: mov r3,r8
  000d0aa8: str r0,[sp,#0x10]
  000d0aaa: ldr r0,[0x000d0cb0]
  000d0aac: str r1,[sp,#0xc]
  000d0aae: mov r1,r10
  000d0ab0: strd r0,r2,[sp,#0x4]
  000d0ab4: mov r0,r5
  000d0ab6: mov r2,r6
  000d0ab8: str r4,[sp,#0x0]
  000d0aba: blx 0x00074350
  000d0abe: ldr.w r0,[r11,#0x4]
  000d0ac2: str r5,[r0,#0x0]
  000d0ac4: movs r0,#0x2c
  000d0ac6: blx 0x0006eb24
  000d0aca: mov r5,r0
  000d0acc: ldr r0,[0x000d0cb4]
  000d0ace: ldr r1,[0x000d0cb8]
  000d0ad0: ldr r2,[0x000d0cbc]
  000d0ad2: str r0,[sp,#0x10]
  000d0ad4: mov r3,r8
  000d0ad6: ldr r0,[0x000d0cc4]
  000d0ad8: str r2,[sp,#0x14]
  000d0ada: ldr r2,[0x000d0cc0]
  000d0adc: strd r2,r1,[sp,#0x8]
  000d0ae0: mov r1,r10
  000d0ae2: strd r4,r0,[sp,#0x0]
  000d0ae6: mov r0,r5
  000d0ae8: mov r2,r6
  000d0aea: blx 0x00074350
  000d0aee: ldr.w r0,[r11,#0x4]
  000d0af2: str r5,[r0,#0x4]
  000d0af4: movs r0,#0x2c
  000d0af6: blx 0x0006eb24
  000d0afa: mov r5,r0
  000d0afc: ldr r0,[0x000d0cc8]
  000d0afe: ldr r1,[0x000d0ccc]
  000d0b00: ldr r2,[0x000d0cd0]
  000d0b02: str r2,[sp,#0x14]
  000d0b04: mov r3,r8
  000d0b06: ldr r2,[0x000d0cd4]
  000d0b08: str r0,[sp,#0x10]
  000d0b0a: ldr r0,[0x000d0cd8]
  000d0b0c: str r1,[sp,#0xc]
  000d0b0e: mov r1,r10
  000d0b10: strd r0,r2,[sp,#0x4]
  000d0b14: movs r0,#0x0
  000d0b16: str r0,[sp,#0x0]
  000d0b18: mov r0,r5
  000d0b1a: mov r2,r6
  000d0b1c: blx 0x00074350
  000d0b20: ldr r4,[sp,#0x34]
  000d0b22: ldr.w r0,[r11,#0x4]
  000d0b26: movw r1,#0x477e
  000d0b2a: str r5,[r0,#0x8]
  000d0b2c: mov r0,r9
  000d0b2e: blx 0x0007432c
  000d0b32: ldr r6,[sp,#0x3c]
  000d0b34: mov r0,r9
  000d0b36: mov r1,r11
  000d0b38: blx 0x00074344
  000d0b3c: ldr r0,[0x000d0cdc]
  000d0b3e: mov r1,r4
  000d0b40: mov r2,r6
  000d0b42: movs r3,#0x0
  000d0b44: add r0,pc
  000d0b46: ldr r0,[r0,#0x0]
  000d0b48: ldr r0,[r0,#0x0]
  000d0b4a: blx 0x000721d8
  000d0b4e: mov r1,r0
  000d0b50: ldr.w r0,[r9,#0x0]
  000d0b54: mov r2,r4
  000d0b56: movs r3,#0x0
  000d0b58: ldr r6,[r0,#0x8]
  000d0b5a: mov r0,r9
  000d0b5c: blx r6
  000d0b5e: ldr r6,[sp,#0x38]
  000d0b60: ldr.w r1,[r9,#0x8]
  000d0b64: ldr r0,[r6,#0x0]
  000d0b66: blx 0x00073d38
  000d0b6a: movs r0,#0x1
  000d0b6c: strb.w r0,[r9,#0x40]
  000d0b70: ldr.w r0,[r9,#0x0]
  000d0b74: mov r1,r6
  000d0b76: ldr r2,[r0,#0x14]
  000d0b78: mov r0,r9
  000d0b7a: blx r2
  000d0b7c: mov r0,r9
  000d0b7e: vpop {d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10}
  000d0b82: add sp,#0x4
  000d0b84: pop.w {r8,r9,r10,r11}
  000d0b88: pop {r4,r5,r6,r7,pc}
```
