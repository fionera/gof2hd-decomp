# Level::createStaticObject
elf 0xbe6c0 body 6414
Sig: undefined __thiscall createStaticObject(Level * this, Waypoint * param_1, int param_2, bool param_3)

## decompile
```c

/* Level::createStaticObject(Waypoint*, int, bool) */

void __thiscall Level::createStaticObject(Level *this,Waypoint *param_1,int param_2,bool param_3)

{
  int iVar1;
  AEGeometry *pAVar2;
  Player *pPVar3;
  PlayerFixedObject *this_00;
  String *pSVar4;
  AEGeometry *this_01;
  uint uVar5;
  AEGeometry *pAVar6;
  AEGeometry *pAVar7;
  AEGeometry *pAVar8;
  void *pvVar9;
  Array *pAVar10;
  AEGeometry *pAVar11;
  AEGeometry *pAVar12;
  AEGeometry *pAVar13;
  AEGeometry *pAVar14;
  FileRead *pFVar15;
  int iVar16;
  Array<BoundingVolume*> *pAVar17;
  BoundingAAB *pBVar18;
  int *piVar19;
  undefined4 *puVar20;
  uint uVar21;
  ushort uVar22;
  code *pcVar23;
  code *pcVar24;
  uint *puVar25;
  code *pcVar26;
  float fVar27;
  int iVar28;
  int iVar29;
  uint uVar30;
  uint uVar31;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
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
  float extraout_s1_14;
  float extraout_s1_15;
  float extraout_s1_16;
  float extraout_s1_17;
  float extraout_s1_18;
  float extraout_s1_19;
  float extraout_s1_20;
  float extraout_s1_21;
  float extraout_s1_22;
  float extraout_s2;
  float extraout_s2_00;
  float fVar32;
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
  float extraout_s2_18;
  float extraout_s2_19;
  float extraout_s3;
  float extraout_s3_00;
  float extraout_s3_01;
  float extraout_s3_02;
  float extraout_s4;
  float extraout_s4_00;
  float extraout_s4_01;
  float extraout_s4_02;
  float extraout_s5;
  float extraout_s5_00;
  float extraout_s5_01;
  float extraout_s5_02;
  float extraout_s6;
  float extraout_s6_00;
  float extraout_s6_01;
  float extraout_s6_02;
  float extraout_s7;
  float extraout_s7_00;
  float extraout_s7_01;
  float extraout_s7_02;
  float extraout_s8;
  float extraout_s8_00;
  float extraout_s8_01;
  float extraout_s8_02;
  undefined4 uVar33;
  undefined4 uVar34;
  float fVar35;
  undefined4 uVar36;
  undefined4 uVar37;
  undefined4 uVar38;
  undefined4 uVar39;
  undefined4 uVar40;
  undefined8 uVar41;
  undefined4 uVar42;
  undefined4 uVar43;
  uint local_9c [15];
  uint local_60;
  uint local_5c;
  uint local_58;
  int local_54;
  
  piVar19 = *(int **)(DAT_000cea34 + 0xce6da);
  local_54 = *piVar19;
  if (param_1 == (Waypoint *)0x0) {
    iVar28 = 0;
    iVar29 = 0;
    fVar27 = 0.0;
  }
  else {
    iVar29 = *(int *)(param_1 + 0x124);
    iVar28 = *(int *)(param_1 + 0x128);
    fVar27 = *(float *)(param_1 + 300);
  }
  if (param_3) {
    puVar20 = *(undefined4 **)(DAT_000cea38 + 0xce704);
    pcVar23 = *(code **)(DAT_000cea3c + 0xce706);
    iVar1 = (*pcVar23)(*puVar20,20000);
    iVar29 = iVar29 + iVar1 + -10000;
    iVar1 = (*pcVar23)(*puVar20,20000);
    iVar28 = iVar28 + iVar1 + -10000;
    iVar1 = (*pcVar23)(*puVar20,20000);
    fVar27 = (float)(iVar1 + (int)fVar27 + -10000);
  }
  if (param_2 == 0x4215) {
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000cea40 + 0xce74a));
    uVar22 = 0x4217;
    if (iVar1 == 1) {
      uVar22 = 0x4216;
    }
    if (iVar1 == 0) {
      uVar22 = 0x4215;
    }
    puVar20 = *(undefined4 **)(DAT_000cea44 + 0xce762);
    pAVar2 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar2,uVar22,(PaintCanvas *)*puVar20,false);
    pPVar3 = operator_new(0x114);
    Player::Player(pPVar3,1000,1,0,0,0);
    this_00 = operator_new(0x128);
    VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
    PlayerJunk::PlayerJunk
              ((PlayerJunk *)this_00,0x4215,pPVar3,pAVar2,fVar27,extraout_s1,extraout_s2);
    AEGeometry::rotate(extraout_s0,extraout_s1_00,extraout_s2_00);
    goto LAB_000ce8b8;
  }
  pAVar2 = operator_new(0xc0);
  iVar1 = 0x1a74;
  if (param_2 != 0x1a74) {
    iVar1 = 0x381b;
  }
  uVar22 = (ushort)param_2;
  if ((param_2 != 0x1a74 && param_2 != iVar1) && (param_2 != 0x1a76)) {
    AEGeometry::AEGeometry
              (pAVar2,uVar22,(PaintCanvas *)**(undefined4 **)(DAT_000cea50 + 0xce8f2),false);
  }
  else {
    AEGeometry::AEGeometry(pAVar2,(PaintCanvas *)**(undefined4 **)(DAT_000cea48 + 0xce80e));
  }
  if (param_2 - 0x49c0U < 3) {
LAB_000ce832:
    pPVar3 = operator_new(0x114);
    Player::Player(pPVar3,1000,100,0,0,0);
    this_00 = operator_new(0x168);
    fVar27 = (float)VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
    fVar32 = (float)VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
    PlayerTurret::PlayerTurret
              ((PlayerTurret *)this_00,param_2,pPVar3,pAVar2,fVar27,extraout_s1_01,fVar32);
    (**(code **)(*(int *)this_00 + 0x14))(this_00,this);
    pSVar4 = (String *)GameText::getText(**(int **)(DAT_000cea4c + 0xce8a2));
    AbyssEngine::String::operator=((String *)(this_00 + 0x18),pSVar4);
  }
  else {
    iVar1 = 0x1a74;
    if (param_2 != 0x1a74) {
      iVar1 = 0x1a76;
    }
    if (param_2 == 0x1a74 || param_2 == iVar1) goto LAB_000ce832;
    if (param_2 == 0x37a3) {
      iVar1 = Status::getLevel();
      if (iVar1 < 0x15) {
        iVar1 = Status::getLevel();
        iVar1 = iVar1 * 0xf + 0x14;
      }
      else {
        iVar1 = 0x140;
      }
      iVar16 = Status::gameWon();
      if (iVar16 == 0) {
        iVar16 = Status::getCurrentCampaignMission();
        iVar16 = iVar16 << 2;
      }
      else {
        iVar16 = 0xb4;
      }
      fVar32 = *(float *)(*(int *)(DAT_000cf144 + 0xcf00c) + 0x2c);
      fVar35 = (float)VectorSignedToFloat((iVar16 + iVar1) * 5,(byte)(in_fpscr >> 0x16) & 3);
      pPVar3 = operator_new(0x114);
      Player::Player(pPVar3,0x1d4c,(int)(fVar35 + (fVar32 + -0.5) * fVar35),0,0,0);
      this_00 = operator_new(0x1bc);
      uVar34 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
      uVar37 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
      uVar39 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
      uVar42 = 0;
      uVar33 = uVar34;
      uVar36 = uVar37;
      uVar38 = uVar39;
      PlayerFixedObject::PlayerFixedObject
                (this_00,0x37a3,8,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_07,extraout_s2_04);
      pAVar2 = operator_new(0xc0);
      puVar20 = *(undefined4 **)(DAT_000cf148 + 0xcf090);
      AEGeometry::AEGeometry(pAVar2,0x37a3,(PaintCanvas *)*puVar20,false);
      (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0,uVar42,uVar33,uVar36,uVar38);
      pAVar2 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar2,0x37a4,(PaintCanvas *)*puVar20,false);
      pAVar6 = operator_new(0xc0);
      AEGeometry::AEGeometry(pAVar6,0x37a5,(PaintCanvas *)*puVar20,false);
      AEGeometry::addChild(*(uint *)(this_00 + 8));
      AEGeometry::addChild(*(uint *)(this_00 + 8));
      pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar6);
      operator_delete(pvVar9);
      pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar2);
      operator_delete(pvVar9);
      iVar28 = PlayerFixedObject::setWreckedMeshId(this_00,0x37a6);
      pAVar10 = (Array *)getBoundingVolume(iVar28,(AEGeometry *)0x3ea);
      PlayerFixedObject::setBV(this_00,pAVar10);
      (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar34,uVar37,uVar39);
      goto LAB_000ce8b8;
    }
    if (param_2 == 0x381b) goto LAB_000ce832;
    if (param_2 != 0x381d) {
      if (param_2 == 0x4220) {
        pPVar3 = operator_new(0x114);
        Player::Player(pPVar3,1000,DAT_000cee4c,0,0,0);
        this_00 = operator_new(0x1bc);
        uVar34 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
        uVar37 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
        uVar39 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
        uVar42 = 0;
        uVar33 = uVar34;
        uVar36 = uVar37;
        uVar38 = uVar39;
        PlayerFixedObject::PlayerFixedObject
                  (this_00,0x4220,0,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_04,extraout_s2_01);
        pAVar2 = operator_new(0xc0);
        puVar20 = *(undefined4 **)(DAT_000cee50 + 0xceac6);
        AEGeometry::AEGeometry(pAVar2,0x4220,(PaintCanvas *)*puVar20,false);
        (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0,uVar42,uVar33,uVar36,uVar38);
        pAVar2 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar2,0x4221,(PaintCanvas *)*puVar20,false);
        pAVar6 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar6,0x4222,(PaintCanvas *)*puVar20,false);
        AEGeometry::addChild(*(uint *)(this_00 + 8));
        AEGeometry::addChild(*(uint *)(this_00 + 8));
        pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar6);
        operator_delete(pvVar9);
        pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar2);
        operator_delete(pvVar9);
        pcVar23 = *(code **)(DAT_000cee54 + 0xceb44);
        uVar33 = (*pcVar23)(*puVar20,*(undefined4 *)(*(int *)(this_00 + 8) + 0xc));
        iVar28 = (*pcVar23)(*puVar20,*(undefined4 *)(*(int *)(this_00 + 8) + 0xc));
        pcVar24 = *(code **)(DAT_000cee58 + 0xceb62);
        (*pcVar24)(uVar33,(undefined4 *)(DAT_000cee58 + 0xceb62),*(undefined4 *)(iVar28 + 0xf8),
                   *(undefined4 *)(iVar28 + 0xfc),0);
        uVar33 = (*pcVar23)(*puVar20,*(undefined4 *)(*(int *)(this_00 + 8) + 0x14));
        uVar41 = (*pcVar23)(*puVar20,*(undefined4 *)(*(int *)(this_00 + 8) + 0x14));
        (*pcVar24)(uVar33,(int)((ulonglong)uVar41 >> 0x20),*(undefined4 *)((int)uVar41 + 0xf8),
                   *(undefined4 *)((int)uVar41 + 0xfc),0);
        uVar33 = (*pcVar23)(*puVar20,*(undefined4 *)(*(int *)(this_00 + 8) + 0x10));
        uVar41 = (*pcVar23)(*puVar20,*(undefined4 *)(*(int *)(this_00 + 8) + 0x10));
        (*pcVar24)(uVar33,(int)((ulonglong)uVar41 >> 0x20),*(undefined4 *)((int)uVar41 + 0xf8),
                   *(undefined4 *)((int)uVar41 + 0xfc),0);
        this_00[0x70] = (PlayerFixedObject)0x1;
        PlayerFixedObject::setWreckedMeshId(this_00,0x37a6);
        pFVar15 = operator_new(1);
        FileRead::FileRead(pFVar15);
        pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,7,-1);
        pvVar9 = (void *)FileRead::~FileRead(pFVar15);
        operator_delete(pvVar9);
        iVar28 = KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
        pAVar10 = (Array *)getBoundingVolume(iVar28,(AEGeometry *)0x3eb);
        PlayerFixedObject::setBV(this_00,pAVar10);
        (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar34,uVar37,uVar39);
        goto LAB_000ce8b8;
      }
      if (param_2 == 0x4a88) {
        pPVar3 = operator_new(0x114);
        Player::Player(pPVar3,1000,DAT_000cee4c,0,0,0);
        Player::setVulnerable(SUB41(pPVar3,0));
        this_00 = operator_new(0x1bc);
        uVar33 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
        uVar36 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
        uVar38 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
        PlayerFixedObject::PlayerFixedObject
                  (this_00,0x4a88,3,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_05,extraout_s2_02);
        pAVar2 = operator_new(0xc0);
        puVar20 = *(undefined4 **)(DAT_000cee5c + 0xcec80);
        AEGeometry::AEGeometry(pAVar2,0x4a88,(PaintCanvas *)*puVar20,false);
        pAVar6 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar6,0x4a8d,(PaintCanvas *)*puVar20,false);
        pAVar7 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar7,0x4a8c,(PaintCanvas *)*puVar20,false);
        pAVar8 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar8,0x4a89,(PaintCanvas *)*puVar20,false);
        pAVar11 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar11,0x4a8a,(PaintCanvas *)*puVar20,false);
        pAVar12 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar12,0x4a8b,(PaintCanvas *)*puVar20,false);
        pcVar26 = *(code **)(DAT_000cee60 + 0xcecfe);
        (*pcVar26)(pAVar2,*(undefined4 *)(pAVar6 + 0xc));
        (*pcVar26)(pAVar2,*(undefined4 *)(pAVar7 + 0xc));
        (*pcVar26)(pAVar2,*(undefined4 *)(pAVar8 + 0xc));
        pAVar13 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar13,0x4a8e,(PaintCanvas *)*puVar20,false);
        pAVar14 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar14,0x4a90,(PaintCanvas *)*puVar20,false);
        this_01 = operator_new(0xc0);
        AEGeometry::AEGeometry(this_01,0x4a8f,(PaintCanvas *)*puVar20,false);
        (*pcVar26)(pAVar2,*(undefined4 *)(pAVar13 + 0xc));
        (*pcVar26)(pAVar2,*(undefined4 *)(pAVar14 + 0xc));
        (*pcVar26)(pAVar2,*(undefined4 *)(this_01 + 0xc));
        pcVar23 = *(code **)(DAT_000cee64 + 0xced7a);
        (*pcVar23)(pAVar13);
        pcVar24 = *(code **)(DAT_000cee68 + 0xced84);
        (*pcVar24)(pAVar13);
        (*pcVar23)(pAVar14);
        (*pcVar24)();
        (*pcVar23)(this_01);
        (*pcVar24)();
        (*pcVar26)(pAVar2,*(undefined4 *)(pAVar11 + 0xc));
        (*pcVar26)(pAVar2,*(undefined4 *)(pAVar12 + 0xc));
        (*pcVar23)(pAVar6);
        (*pcVar24)();
        (*pcVar23)(pAVar7);
        (*pcVar24)();
        (*pcVar23)(pAVar8);
        (*pcVar24)();
        (*pcVar23)(pAVar11);
        (*pcVar24)();
        (*pcVar23)(pAVar12);
        (*pcVar24)();
        (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0);
        this_00[0x70] = (PlayerFixedObject)0x1;
        (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar33,uVar36,uVar38);
        PlayerFixedObject::setWreckedMeshId(this_00,0x4a88);
        pFVar15 = operator_new(1);
        FileRead::FileRead(pFVar15);
        pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,1,-1);
        pvVar9 = (void *)FileRead::~FileRead(pFVar15);
        operator_delete(pvVar9);
        iVar28 = KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
        pAVar10 = (Array *)getBoundingVolume(iVar28,(AEGeometry *)0x7d0);
        PlayerFixedObject::setBV(this_00,pAVar10);
        *(undefined4 *)(*(int *)(this_00 + 4) + 0x40) = 0;
        goto LAB_000ce8b8;
      }
      if (param_2 == 0x4a6a) {
        pPVar3 = operator_new(0x114);
        Player::Player(pPVar3,1000,DAT_000cea54,0,0,0);
        this_00 = operator_new(0x130);
        VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
        fVar27 = (float)VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
        fVar32 = (float)VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
        PlayerStatic::PlayerStatic
                  ((PlayerStatic *)this_00,0x4a6a,pAVar2,fVar27,extraout_s1_02,fVar32);
        goto LAB_000ce8b8;
      }
      uVar5 = param_2 - 0x493e;
      if (uVar5 < 0x20) {
        if ((1 << (uVar5 & 0xff) & 0x1249U) == 0) {
          if (uVar5 == 0x10) {
            iVar1 = Status::getLevel();
            if (iVar1 < 0x15) {
              iVar1 = Status::getLevel();
              iVar1 = iVar1 * 0xf + 100;
            }
            else {
              iVar1 = 400;
            }
            iVar16 = Status::gameWon();
            if (iVar16 == 0) {
              iVar16 = Status::getCurrentCampaignMission();
              iVar16 = iVar16 << 2;
            }
            else {
              iVar16 = 0xb4;
            }
            fVar32 = *(float *)(*(int *)(DAT_000cfb60 + 0xcf930) + 0x2c);
            fVar35 = (float)VectorSignedToFloat((iVar16 + iVar1) * 5,(byte)(in_fpscr >> 0x16) & 3);
            pPVar3 = operator_new(0x114);
            Player::Player(pPVar3,1000,(int)(fVar35 + (fVar32 + -0.5) * fVar35),0,0,0);
            this_00 = operator_new(0x1bc);
            uVar39 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
            uVar42 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
            uVar40 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
            PlayerFixedObject::PlayerFixedObject
                      (this_00,0x494e,3,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_14,
                       extraout_s2_11);
            pAVar2 = operator_new(0xc0);
            puVar20 = *(undefined4 **)(DAT_000cfb64 + 0xcf9b4);
            AEGeometry::AEGeometry(pAVar2,0x4a80,(PaintCanvas *)*puVar20,false);
            pAVar6 = operator_new(0xc0);
            AEGeometry::AEGeometry(pAVar6,0x494e,(PaintCanvas *)*puVar20,false);
            pAVar7 = operator_new(0xc0);
            AEGeometry::AEGeometry(pAVar7,0x4a81,(PaintCanvas *)*puVar20,false);
            pAVar8 = operator_new(0xc0);
            AEGeometry::AEGeometry(pAVar8,0x4a82,(PaintCanvas *)*puVar20,false);
            pAVar11 = operator_new(0xc0);
            AEGeometry::AEGeometry(pAVar11,0x4a83,(PaintCanvas *)*puVar20,false);
            pAVar12 = operator_new(0xc0);
            AEGeometry::AEGeometry(pAVar12,0x4a84,(PaintCanvas *)*puVar20,false);
            pcVar23 = *(code **)(DAT_000cfb68 + 0xcfa34);
            (*pcVar23)(pAVar2,*(undefined4 *)(pAVar6 + 0xc));
            (*pcVar23)(pAVar6,*(undefined4 *)(pAVar7 + 0xc));
            (*pcVar23)(pAVar6,*(undefined4 *)(pAVar8 + 0xc));
            (*pcVar23)(pAVar6,*(undefined4 *)(pAVar11 + 0xc));
            (*pcVar23)(pAVar6,*(undefined4 *)(pAVar12 + 0xc));
            pcVar23 = *(code **)(DAT_000cfb6c + 0xcfa5c);
            (*pcVar23)(pAVar6);
            pcVar24 = *(code **)(DAT_000cfb70 + 0xcfa6a);
            (*pcVar24)(pAVar6);
            (*pcVar23)(pAVar7);
            (*pcVar24)();
            (*pcVar23)(pAVar8);
            (*pcVar24)();
            (*pcVar23)(pAVar11);
            (*pcVar24)();
            (*pcVar23)(pAVar12);
            (*pcVar24)();
            (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0);
            this_00[0x40] = (PlayerFixedObject)0x1;
            this_00[0x70] = (PlayerFixedObject)0x1;
            pAVar17 = operator_new(0xc);
            Array<BoundingVolume*>::Array(pAVar17);
            ArraySetLength<BoundingVolume*>(2,(Array *)pAVar17);
            pBVar18 = operator_new(0x2c);
            BoundingAAB::BoundingAAB
                      (pBVar18,extraout_s0_00,extraout_s1_15,extraout_s2_12,extraout_s3,extraout_s4,
                       extraout_s5,extraout_s6,extraout_s7,extraout_s8);
            **(undefined4 **)(pAVar17 + 4) = pBVar18;
            pBVar18 = operator_new(0x2c);
            uVar43 = 0;
            uVar33 = DAT_000cfb98;
            uVar36 = DAT_000cfb94;
            uVar38 = DAT_000cfb8c;
            uVar34 = DAT_000cfb88;
            uVar37 = DAT_000cfb90;
            BoundingAAB::BoundingAAB
                      (pBVar18,extraout_s0_01,extraout_s1_16,extraout_s2_13,extraout_s3_00,
                       extraout_s4_00,extraout_s5_00,extraout_s6_00,extraout_s7_00,extraout_s8_00);
            *(BoundingAAB **)(*(int *)(pAVar17 + 4) + 4) = pBVar18;
            PlayerFixedObject::setBV(this_00,(Array *)pAVar17);
            (**(code **)(*(int *)this_00 + 0x48))
                      (this_00,uVar39,uVar42,uVar40,uVar43,uVar33,uVar36,uVar38,uVar34,uVar37);
            PlayerFixedObject::setWreckedMeshId(this_00,0x477c);
            pFVar15 = operator_new(1);
            FileRead::FileRead(pFVar15);
            iVar28 = 3;
            goto LAB_000d00a4;
          }
          if (uVar5 != 0x1f) goto LAB_000cf360;
          pPVar3 = operator_new(0x114);
          Player::Player(pPVar3,1000,DAT_000cf4a8,0,0,0);
          this_00 = operator_new(0x1bc);
          uVar33 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
          uVar36 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
          uVar38 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
          PlayerFixedObject::PlayerFixedObject
                    (this_00,0x495d,3,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_08,extraout_s2_05)
          ;
          pAVar2 = operator_new(0xc0);
          puVar20 = *(undefined4 **)(DAT_000cf4ac + 0xcf1be);
          AEGeometry::AEGeometry(pAVar2,0x5254,(PaintCanvas *)*puVar20,false);
          pAVar6 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar6,0x5574,(PaintCanvas *)*puVar20,false);
          pAVar7 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar7,0x563c,(PaintCanvas *)*puVar20,false);
          pAVar8 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar8,0x495d,(PaintCanvas *)*puVar20,false);
          pAVar11 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar11,0x495d,(PaintCanvas *)*puVar20,false);
          pAVar12 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar12,0x495e,(PaintCanvas *)*puVar20,false);
          pAVar13 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar13,0x495f,(PaintCanvas *)*puVar20,false);
          pAVar14 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar14,0x4960,(PaintCanvas *)*puVar20,false);
          pcVar23 = *(code **)(DAT_000cf4b0 + 0xcf27e);
          (*pcVar23)(pAVar2,*(undefined4 *)(pAVar14 + 0xc));
          (*pcVar23)(pAVar2,*(undefined4 *)(pAVar6 + 0xc));
          (*pcVar23)(pAVar2,*(undefined4 *)(pAVar7 + 0xc));
          (*pcVar23)(pAVar2,*(undefined4 *)(pAVar8 + 0xc));
          (*pcVar23)(pAVar2,*(undefined4 *)(pAVar11 + 0xc));
          (*pcVar23)(pAVar2,*(undefined4 *)(pAVar12 + 0xc));
          (*pcVar23)(pAVar2,*(undefined4 *)(pAVar13 + 0xc));
          pcVar23 = *(code **)(DAT_000cf4b4 + 0xcf2c4);
          (*pcVar23)(pAVar6);
          pcVar24 = *(code **)(DAT_000cf4b8 + 0xcf2ce);
          (*pcVar24)();
          (*pcVar23)(pAVar7);
          (*pcVar24)();
          (*pcVar23)(pAVar8);
          (*pcVar24)();
          (*pcVar23)(pAVar11);
          (*pcVar24)();
          (*pcVar23)(pAVar12);
          (*pcVar24)();
          (*pcVar23)(pAVar13);
          (*pcVar24)();
          (*pcVar23)(pAVar14);
          (*pcVar24)();
          (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0);
          this_00[0x70] = (PlayerFixedObject)0x1;
          (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar33,uVar36,uVar38);
          PlayerFixedObject::setWreckedMeshId(this_00,0x477c);
          pFVar15 = operator_new(1);
          FileRead::FileRead(pFVar15);
          pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,6,-1);
          pvVar9 = (void *)FileRead::~FileRead(pFVar15);
          operator_delete(pvVar9);
          iVar28 = KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
          pAVar2 = (AEGeometry *)0x6f;
        }
        else {
          pPVar3 = operator_new(0x114);
          Player::Player(pPVar3,1000,DAT_000cf138,0,0,0);
          Player::setVulnerable(SUB41(pPVar3,0));
          this_00 = operator_new(0x1bc);
          uVar33 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
          uVar36 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
          uVar38 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
          PlayerFixedObject::PlayerFixedObject
                    (this_00,param_2,3,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_06,extraout_s2_03
                    );
          pAVar2 = operator_new(0xc0);
          puVar20 = *(undefined4 **)(DAT_000cf13c + 0xceef4);
          AEGeometry::AEGeometry(pAVar2,uVar22,(PaintCanvas *)*puVar20,false);
          pAVar6 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar6,uVar22 + 2,(PaintCanvas *)*puVar20,false);
          pAVar7 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar7,uVar22 + 1,(PaintCanvas *)*puVar20,false);
          AEGeometry::addChild((uint)pAVar2);
          AEGeometry::addChild((uint)pAVar2);
          if ((param_2 == 0x494a) && (iVar28 = Status::getCurrentCampaignMission(), iVar28 == 0x91))
          {
            pAVar8 = operator_new(0xc0);
            AEGeometry::AEGeometry(pAVar8,0x494d,(PaintCanvas *)*puVar20,false);
            AEGeometry::addChild((uint)pAVar2);
            pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar8);
            operator_delete(pvVar9);
          }
          pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar6);
          operator_delete(pvVar9);
          pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar7);
          operator_delete(pvVar9);
          (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0);
          this_00[0x70] = (PlayerFixedObject)0x1;
          iVar28 = (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar33,uVar36,uVar38);
          pAVar2 = (AEGeometry *)0x7d1;
        }
        pAVar10 = (Array *)getBoundingVolume(iVar28,pAVar2);
        PlayerFixedObject::setBV(this_00,pAVar10);
      }
      else {
LAB_000cf360:
        if (param_2 == 0x4260) {
          pPVar3 = operator_new(0x114);
          Player::Player(pPVar3,1000,DAT_000cf8c0,0,0,0);
          this_00 = operator_new(0x1bc);
          uVar34 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
          uVar37 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
          uVar39 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
          uVar42 = 0;
          uVar33 = uVar34;
          uVar36 = uVar37;
          uVar38 = uVar39;
          PlayerFixedObject::PlayerFixedObject
                    (this_00,0x4260,0,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_10,extraout_s2_07)
          ;
          pAVar2 = operator_new(0xc0);
          AEGeometry::AEGeometry
                    (pAVar2,0x4260,(PaintCanvas *)**(undefined4 **)(DAT_000cf8c4 + 0xcf52a),false);
          (**(code **)(*(int *)this_00 + 8))
                    (this_00,pAVar2,0xffffffff,0,uVar42,uVar33,uVar36,uVar38);
          this_00[0x70] = (PlayerFixedObject)0x1;
          (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar34,uVar37,uVar39);
          PlayerFixedObject::setWreckedMeshId(this_00,0x477c);
          pFVar15 = operator_new(1);
          FileRead::FileRead(pFVar15);
          pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,2,-1);
          pvVar9 = (void *)FileRead::~FileRead(pFVar15);
          operator_delete(pvVar9);
          KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
          goto LAB_000ce8b8;
        }
        if (param_2 != 0x4299) {
          if (param_2 == 0x4961) {
            pPVar3 = operator_new(0x114);
            Player::Player(pPVar3,1000,DAT_000cf8c0,0,0,0);
            this_00 = operator_new(0x1bc);
            uVar33 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
            uVar36 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
            uVar38 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
            PlayerFixedObject::PlayerFixedObject
                      (this_00,0x4961,2,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_12,
                       extraout_s2_09);
            pAVar2 = operator_new(0xc0);
            puVar20 = *(undefined4 **)(DAT_000cf8d4 + 0xcf6ec);
            AEGeometry::AEGeometry(pAVar2,0x4961,(PaintCanvas *)*puVar20,false);
            pAVar6 = operator_new(0xc0);
            AEGeometry::AEGeometry(pAVar6,0x498f,(PaintCanvas *)*puVar20,false);
            AEGeometry::addChild((uint)pAVar2);
            pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar6);
            operator_delete(pvVar9);
            (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0);
            this_00[0x70] = (PlayerFixedObject)0x1;
            (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar33,uVar36,uVar38);
            PlayerFixedObject::setWreckedMeshId(this_00,0x477c);
            *(undefined4 *)(this_00 + 0x28) = 2;
            pFVar15 = operator_new(1);
            FileRead::FileRead(pFVar15);
            pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,2,-1);
            pvVar9 = (void *)FileRead::~FileRead(pFVar15);
            operator_delete(pvVar9);
            iVar28 = KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
            pAVar2 = (AEGeometry *)0x7d3;
          }
          else {
            if (param_2 == 0x4962) {
              pPVar3 = operator_new(0x114);
              Player::Player(pPVar3,0,DAT_000cf8c0,0,0,0);
              this_00 = operator_new(0x1bc);
              uVar34 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
              uVar37 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
              uVar39 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
              uVar42 = 0;
              uVar33 = uVar34;
              uVar36 = uVar37;
              uVar38 = uVar39;
              PlayerFixedObject::PlayerFixedObject
                        (this_00,0x4962,2,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_11,
                         extraout_s2_08);
              pAVar2 = operator_new(0xc0);
              AEGeometry::AEGeometry
                        (pAVar2,0x4962,(PaintCanvas *)**(undefined4 **)(DAT_000cf8c8 + 0xcf608),
                         false);
              (**(code **)(*(int *)this_00 + 8))
                        (this_00,pAVar2,0xffffffff,0,uVar42,uVar33,uVar36,uVar38);
              this_00[0x70] = (PlayerFixedObject)0x0;
              (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar34,uVar37,uVar39);
              PlayerFixedObject::setWreckedMeshId(this_00,0x477c);
              goto LAB_000ce8b8;
            }
            if (param_2 == 0x4974) {
              iVar1 = Status::getLevel();
              if (iVar1 < 0x15) {
                iVar1 = Status::getLevel();
                iVar1 = iVar1 * 0xf + 100;
              }
              else {
                iVar1 = 400;
              }
              iVar16 = Status::gameWon();
              if (iVar16 == 0) {
                iVar16 = Status::getCurrentCampaignMission();
                iVar16 = iVar16 << 2;
              }
              else {
                iVar16 = 0xb4;
              }
              fVar32 = *(float *)(*(int *)(DAT_000cff50 + 0xcfbb0) + 0x2c);
              fVar35 = (float)VectorSignedToFloat((iVar16 + iVar1) * 5,(byte)(in_fpscr >> 0x16) & 3)
              ;
              pPVar3 = operator_new(0x114);
              Player::Player(pPVar3,0,(int)(fVar35 + (fVar32 + -0.5) * fVar35),0,0,0);
              this_00 = operator_new(0x1bc);
              uVar33 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
              uVar36 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
              uVar38 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
              PlayerFixedObject::PlayerFixedObject
                        (this_00,0x4974,0,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_17,
                         extraout_s2_14);
              pAVar2 = operator_new(0xc0);
              puVar20 = *(undefined4 **)(DAT_000cff54 + 0xcfc32);
              AEGeometry::AEGeometry(pAVar2,0x4974,(PaintCanvas *)*puVar20,false);
              pAVar6 = operator_new(0xc0);
              AEGeometry::AEGeometry(pAVar6,0x4975,(PaintCanvas *)*puVar20,false);
              pAVar7 = operator_new(0xc0);
              AEGeometry::AEGeometry(pAVar7,0x4977,(PaintCanvas *)*puVar20,false);
              pAVar8 = operator_new(0xc0);
              AEGeometry::AEGeometry(pAVar8,0x4978,(PaintCanvas *)*puVar20,false);
              pAVar11 = operator_new(0xc0);
              AEGeometry::AEGeometry(pAVar11,0x4979,(PaintCanvas *)*puVar20,false);
              pAVar12 = operator_new(0xc0);
              AEGeometry::AEGeometry(pAVar12,0x497a,(PaintCanvas *)*puVar20,false);
              pAVar13 = operator_new(0xc0);
              AEGeometry::AEGeometry(pAVar13,0x4976,(PaintCanvas *)*puVar20,false);
              pcVar23 = *(code **)(DAT_000cff58 + 0xcfcda);
              (*pcVar23)(pAVar2,*(undefined4 *)(pAVar6 + 0xc));
              (*pcVar23)(pAVar2,*(undefined4 *)(pAVar7 + 0xc));
              (*pcVar23)(pAVar2,*(undefined4 *)(pAVar8 + 0xc));
              (*pcVar23)(pAVar2,*(undefined4 *)(pAVar11 + 0xc));
              (*pcVar23)(pAVar2,*(undefined4 *)(pAVar12 + 0xc));
              (*pcVar23)(pAVar2,*(undefined4 *)(pAVar13 + 0xc));
              pcVar23 = *(code **)(DAT_000cff5c + 0xcfd0e);
              (*pcVar23)(pAVar6);
              pcVar24 = *(code **)(DAT_000cff60 + 0xcfd18);
              (*pcVar24)(pAVar6);
              (*pcVar23)(pAVar7);
              (*pcVar24)();
              (*pcVar23)(pAVar8);
              (*pcVar24)();
              (*pcVar23)(pAVar11);
              (*pcVar24)();
              (*pcVar23)(pAVar12);
              (*pcVar24)();
              (*pcVar23)(pAVar13);
              (*pcVar24)();
              (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0);
              this_00[0x70] = (PlayerFixedObject)0x1;
              PlayerFixedObject::setWreckedMeshId(this_00,0x494e);
              (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar33,uVar36,uVar38);
              pFVar15 = operator_new(1);
              FileRead::FileRead(pFVar15);
              pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,5,-1);
              pvVar9 = (void *)FileRead::~FileRead(pFVar15);
              operator_delete(pvVar9);
              iVar28 = KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
              pAVar2 = (AEGeometry *)0x7d5;
            }
            else if (param_2 == 0x4a6b) {
              pPVar3 = operator_new(0x114);
              Player::Player(pPVar3,0,DAT_000cf8c0,0,0,0);
              this_00 = operator_new(0x1bc);
              uVar33 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
              uVar36 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
              uVar38 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
              PlayerFixedObject::PlayerFixedObject
                        (this_00,0x4a6b,1,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_13,
                         extraout_s2_10);
              pAVar2 = operator_new(0xc0);
              puVar20 = *(undefined4 **)(DAT_000cf8d8 + 0xcf7f8);
              AEGeometry::AEGeometry(pAVar2,0x4a6b,(PaintCanvas *)*puVar20,false);
              pAVar6 = operator_new(0xc0);
              AEGeometry::AEGeometry(pAVar6,0x4a6c,(PaintCanvas *)*puVar20,false);
              AEGeometry::addChild((uint)pAVar2);
              pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar6);
              operator_delete(pvVar9);
              pAVar6 = operator_new(0xc0);
              AEGeometry::AEGeometry(pAVar6,0x4a6d,(PaintCanvas *)*puVar20,false);
              AEGeometry::addChild((uint)pAVar2);
              pvVar9 = (void *)AEGeometry::~AEGeometry(pAVar6);
              operator_delete(pvVar9);
              (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0);
              this_00[0x70] = (PlayerFixedObject)0x1;
              (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar33,uVar36,uVar38);
              PlayerFixedObject::setWreckedMeshId(this_00,0x477c);
              pFVar15 = operator_new(1);
              FileRead::FileRead(pFVar15);
              pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,8,-1);
              pvVar9 = (void *)FileRead::~FileRead(pFVar15);
              operator_delete(pvVar9);
              iVar28 = KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
              pAVar2 = (AEGeometry *)0x7d6;
            }
            else {
              if (param_2 != 0x5279) {
                this_00 = (PlayerFixedObject *)0x0;
                goto LAB_000ce8b8;
              }
              pPVar3 = operator_new(0x114);
              Player::Player(pPVar3,0,DAT_000cf4a8,0,0,0);
              Player::setVulnerable(SUB41(pPVar3,0));
              this_00 = operator_new(0x1bc);
              uVar34 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
              uVar37 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
              uVar39 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
              uVar42 = 0;
              uVar33 = uVar34;
              uVar36 = uVar37;
              uVar38 = uVar39;
              PlayerFixedObject::PlayerFixedObject
                        (this_00,0x41a0,1,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_09,
                         extraout_s2_06);
              pAVar2 = operator_new(0xc0);
              AEGeometry::AEGeometry
                        (pAVar2,0x41a0,(PaintCanvas *)**(undefined4 **)(DAT_000cf4bc + 0xcf412),
                         false);
              (**(code **)(*(int *)this_00 + 8))
                        (this_00,pAVar2,0xffffffff,0,uVar42,uVar33,uVar36,uVar38);
              this_00[0x70] = (PlayerFixedObject)0x1;
              (**(code **)(*(int *)this_00 + 0x48))(this_00,uVar34,uVar37,uVar39);
              PlayerFixedObject::setWreckedMeshId(this_00,0x5279);
              pFVar15 = operator_new(1);
              FileRead::FileRead(pFVar15);
              pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,10,-1);
              pvVar9 = (void *)FileRead::~FileRead(pFVar15);
              operator_delete(pvVar9);
              iVar28 = KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
              pAVar2 = (AEGeometry *)0x7d2;
            }
          }
          pAVar10 = (Array *)getBoundingVolume(iVar28,pAVar2);
          PlayerFixedObject::setBV(this_00,pAVar10);
          goto LAB_000ce8b8;
        }
        iVar1 = Status::getLevel();
        if (iVar1 < 0x15) {
          iVar1 = Status::getLevel();
          iVar1 = iVar1 * 0xf + 100;
        }
        else {
          iVar1 = 400;
        }
        iVar16 = Status::gameWon();
        if (iVar16 == 0) {
          iVar16 = Status::getCurrentCampaignMission();
          iVar16 = iVar16 << 2;
        }
        else {
          iVar16 = 0xb4;
        }
        fVar32 = *(float *)(*(int *)(DAT_000cff64 + 0xcfde8) + 0x2c);
        fVar35 = (float)VectorSignedToFloat((iVar16 + iVar1) * 5,(byte)(in_fpscr >> 0x16) & 3);
        pPVar3 = operator_new(0x114);
        Player::Player(pPVar3,1000,(int)(fVar35 + (fVar32 + -0.5) * fVar35),0,0,0);
        this_00 = operator_new(0x1bc);
        uVar33 = VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
        uVar36 = VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
        uVar38 = VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
        PlayerFixedObject::PlayerFixedObject
                  (this_00,0x4299,3,pPVar3,(AEGeometry *)0x0,fVar27,extraout_s1_18,extraout_s2_15);
        pAVar2 = operator_new(0xc0);
        puVar25 = *(uint **)(DAT_000cff68 + 0xcfe6e);
        AEGeometry::AEGeometry(pAVar2,0x4299,(PaintCanvas *)*puVar25,false);
        local_60 = 0xffffffff;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar25,&local_60);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar25,(ushort)local_60,true);
        AEGeometry::addChild((uint)pAVar2);
        uVar5 = 0xffffffff;
        uVar31 = 0xffffffff;
        for (iVar28 = 0; iVar28 != 3; iVar28 = iVar28 + 1) {
          local_58 = 0xffffffff;
          local_5c = 0xffffffff;
          AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar25,&local_58);
          AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar25,&local_5c);
          AbyssEngine::PaintCanvas::TransformAddMesh(*puVar25,(ushort)local_58,true);
          AbyssEngine::PaintCanvas::TransformAddMesh(*puVar25,(ushort)local_5c,true);
          AbyssEngine::PaintCanvas::TransformGetLocal(*puVar25);
          AbyssEngine::AEMath::MatrixSetTranslation
                    ((Matrix *)local_9c,extraout_s0_02,extraout_s1_19,extraout_s2_16);
          AbyssEngine::PaintCanvas::TransformGetLocal(*puVar25);
          AbyssEngine::AEMath::MatrixSetTranslation
                    ((Matrix *)local_9c,extraout_s0_03,extraout_s1_20,extraout_s2_17);
          uVar21 = local_58;
          uVar30 = local_5c;
          if (uVar5 != 0xffffffff) {
            AbyssEngine::PaintCanvas::TransformAddChild((PaintCanvas *)*puVar25,uVar5,local_58);
            AbyssEngine::PaintCanvas::TransformAddChild((PaintCanvas *)*puVar25,uVar31,local_5c);
            uVar21 = uVar5;
            uVar30 = uVar31;
          }
          uVar5 = uVar21;
          uVar31 = uVar30;
        }
        AEGeometry::addChild((uint)pAVar2);
        local_9c[0] = 0xffffffff;
        AbyssEngine::PaintCanvas::TransformCreate((PaintCanvas *)*puVar25,local_9c);
        AbyssEngine::PaintCanvas::TransformAddMesh(*puVar25,(ushort)local_9c[0],true);
        AEGeometry::addChild((uint)pAVar2);
        local_5c = CONCAT22(local_5c._2_2_,0x429a);
        local_58 = 35000;
        AEGeometry::setLodMeshes(pAVar2,(ushort *)&local_5c,(int *)&local_58,1);
        AEGeometry::setLodChildTransform(pAVar2,uVar31);
        (**(code **)(*(int *)this_00 + 8))(this_00,pAVar2,0xffffffff,0);
        this_00[0x40] = (PlayerFixedObject)0x1;
        this_00[0x70] = (PlayerFixedObject)0x1;
        pAVar17 = operator_new(0xc);
        Array<BoundingVolume*>::Array(pAVar17);
        ArraySetLength<BoundingVolume*>(2,(Array *)pAVar17);
        pBVar18 = operator_new(0x2c);
        BoundingAAB::BoundingAAB
                  (pBVar18,extraout_s0_04,extraout_s1_21,extraout_s2_18,extraout_s3_01,
                   extraout_s4_01,extraout_s5_01,extraout_s6_01,extraout_s7_01,extraout_s8_01);
        **(undefined4 **)(pAVar17 + 4) = pBVar18;
        pBVar18 = operator_new(0x2c);
        uVar43 = 0;
        uVar34 = DAT_000d01e4;
        uVar37 = DAT_000d01e0;
        uVar39 = DAT_000d01d8;
        uVar42 = DAT_000d01d4;
        uVar40 = DAT_000d01dc;
        BoundingAAB::BoundingAAB
                  (pBVar18,extraout_s0_05,extraout_s1_22,extraout_s2_19,extraout_s3_02,
                   extraout_s4_02,extraout_s5_02,extraout_s6_02,extraout_s7_02,extraout_s8_02);
        *(BoundingAAB **)(*(int *)(pAVar17 + 4) + 4) = pBVar18;
        PlayerFixedObject::setBV(this_00,(Array *)pAVar17);
        (**(code **)(*(int *)this_00 + 0x48))
                  (this_00,uVar33,uVar36,uVar38,uVar43,uVar34,uVar37,uVar39,uVar42,uVar40);
        PlayerFixedObject::setWreckedMeshId(this_00,0x477c);
        *(undefined4 *)(this_00 + 0x28) = 3;
        pFVar15 = operator_new(1);
        FileRead::FileRead(pFVar15);
        iVar28 = 4;
LAB_000d00a4:
        pAVar10 = (Array *)FileRead::loadSpacePoints(pFVar15,iVar28,-1);
        pvVar9 = (void *)FileRead::~FileRead(pFVar15);
        operator_delete(pvVar9);
        KIPlayer::setSpacePoints((KIPlayer *)this_00,pAVar10);
      }
      goto LAB_000ce8b8;
    }
    pPVar3 = operator_new(0x114);
    Player::Player(pPVar3,1000,100,0,0,0);
    this_00 = operator_new(0x168);
    fVar27 = (float)VectorSignedToFloat(fVar27,(byte)(in_fpscr >> 0x16) & 3);
    fVar32 = (float)VectorSignedToFloat(iVar28,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(iVar29,(byte)(in_fpscr >> 0x16) & 3);
    PlayerTurret::PlayerTurret
              ((PlayerTurret *)this_00,0x381d,pPVar3,pAVar2,fVar27,extraout_s1_03,fVar32);
    (**(code **)(*(int *)this_00 + 0x14))(this_00,this);
    pSVar4 = (String *)GameText::getText(**(int **)(DAT_000cea5c + 0xcea1a));
    AbyssEngine::String::operator=((String *)(this_00 + 0x18),pSVar4);
    *(PlayerTurret *)(this_00 + 0x25) = (PlayerTurret)0x0;
  }
  *(undefined4 *)(this_00 + 0x50) = 0;
LAB_000ce8b8:
  (**(code **)(*(int *)this_00 + 0x14))(this_00,this);
  iVar28 = *piVar19 - local_54;
  if (iVar28 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar28);
  }
  return;
}

```
## target disasm
```
  000ce6c0: push {r4,r5,r6,r7,lr}
  000ce6c2: add r7,sp,#0xc
  000ce6c4: push {r7,r8,r9,r10,r11}
  000ce6c8: vpush {d8,d9,d10,d11,d12}
  000ce6cc: sub sp,#0x88
  000ce6ce: str r0,[sp,#0x38]
  000ce6d0: mov r8,r2
  000ce6d2: ldr r0,[0x000cea34]
  000ce6d4: cmp r1,#0x0
  000ce6d6: add r0,pc
  000ce6d8: ldr r2,[r0,#0x0]
  000ce6da: ldr r0,[r2,#0x0]
  000ce6dc: str r0,[sp,#0x84]
  000ce6de: beq 0x000ce6ea
  000ce6e0: ldrd r11,r10,[r1,#0x124]
  000ce6e4: ldr.w r9,[r1,#0x12c]
  000ce6e8: b 0x000ce6f6
  000ce6ea: mov.w r10,#0x0
  000ce6ee: mov.w r11,#0x0
  000ce6f2: mov.w r9,#0x0
  000ce6f6: cmp r3,#0x0
  000ce6f8: str r2,[sp,#0x34]
  000ce6fa: beq 0x000ce73a
  000ce6fc: ldr r0,[0x000cea38]
  000ce6fe: ldr r1,[0x000cea3c]
  000ce700: add r0,pc
  000ce702: add r1,pc
  000ce704: ldr r4,[r0,#0x0]
  000ce706: ldr r5,[r1,#0x0]
  000ce708: movw r1,#0x4e20
  000ce70c: ldr r0,[r4,#0x0]
  000ce70e: blx r5
  000ce710: add.w r1,r11,r0
  000ce714: ldr r0,[r4,#0x0]
  000ce716: movw r6,#0x2710
  000ce71a: sub.w r11,r1,r6
  000ce71e: movw r1,#0x4e20
  000ce722: blx r5
  000ce724: add.w r1,r10,r0
  000ce728: ldr r0,[r4,#0x0]
  000ce72a: sub.w r10,r1,r6
  000ce72e: movw r1,#0x4e20
  000ce732: blx r5
  000ce734: add r0,r9
  000ce736: sub.w r9,r0,r6
  000ce73a: movw r0,#0x4215
  000ce73e: cmp r8,r0
  000ce740: bne 0x000ce7f0
  000ce742: ldr r0,[0x000cea40]
  000ce744: movs r1,#0x3
  000ce746: add r0,pc
  000ce748: ldr r0,[r0,#0x0]
  000ce74a: ldr r0,[r0,#0x0]
  000ce74c: blx 0x00071848
  000ce750: ldr r1,[0x000cea44]
  000ce752: movw r5,#0x4217
  000ce756: cmp r0,#0x1
  000ce758: it eq
  000ce75a: movw.eq r5,#0x4216
  000ce75e: add r1,pc
  000ce760: cmp r0,#0x0
  000ce762: mov.w r0,#0xc0
  000ce766: it eq
  000ce768: movw.eq r5,#0x4215
  000ce76c: ldr r6,[r1,#0x0]
  000ce76e: blx 0x0006eb24
  000ce772: mov r4,r0
  000ce774: ldr r2,[r6,#0x0]
  000ce776: mov r1,r5
  000ce778: movs r3,#0x0
  000ce77a: mov.w r8,#0x0
  000ce77e: blx 0x0007207c
  000ce782: mov.w r0,#0x114
  000ce786: blx 0x0006eb24
  000ce78a: mov r6,r0
  000ce78c: mov.w r1,#0x3e8
  000ce790: movs r2,#0x1
  000ce792: movs r3,#0x0
  000ce794: strd r8,r8,[sp,#0x0]
  000ce798: blx 0x00073d80
  000ce79c: mov.w r0,#0x128
  000ce7a0: blx 0x0006eb24
  000ce7a4: vmov s0,r11
  000ce7a8: mov r11,r0
  000ce7aa: vcvt.f32.s32 s16,s0
  000ce7ae: vmov s0,r10
  000ce7b2: vcvt.f32.s32 s18,s0
  000ce7b6: vmov s0,r9
  000ce7ba: vcvt.f32.s32 s20,s0
  000ce7be: movw r1,#0x4215
  000ce7c2: mov r2,r6
  000ce7c4: mov r3,r4
  000ce7c6: vstr.32 s16,[sp]
  000ce7ca: vstr.32 s18,[sp,#0x4]
  000ce7ce: vstr.32 s20,[sp,#0x8]
  000ce7d2: blx 0x00074314
  000ce7d6: vmov r1,s16
  000ce7da: ldr.w r0,[r11,#0x8]
  000ce7de: vmov r2,s18
  000ce7e2: vmov r3,s20
  000ce7e6: blx 0x00072160
  000ce7ea: ldr.w r8,[sp,#0x38]
  000ce7ee: b 0x000ce8b8
  000ce7f0: movs r0,#0xc0
  000ce7f2: blx 0x0006eb24
  000ce7f6: mov r4,r0
  000ce7f8: movw r0,#0x1a74
  000ce7fc: cmp r8,r0
  000ce7fe: itt ne
  000ce800: movw.ne r0,#0x381b
  000ce804: cmp.ne r8,r0
  000ce806: bne 0x000ce8e4
  000ce808: ldr r0,[0x000cea48]
  000ce80a: add r0,pc
  000ce80c: ldr r0,[r0,#0x0]
  000ce80e: ldr r1,[r0,#0x0]
  000ce810: mov r0,r4
  000ce812: blx 0x000720f4
  000ce816: movw r0,#0x49c0
  000ce81a: sub.w r0,r8,r0
  000ce81e: cmp r0,#0x3
  000ce820: bcc 0x000ce832
  000ce822: movw r0,#0x1a74
  000ce826: cmp r8,r0
  000ce828: itt ne
  000ce82a: movw.ne r0,#0x1a76
  000ce82e: cmp.ne r8,r0
  000ce830: bne 0x000ce902
  000ce832: mov.w r0,#0x114
  000ce836: blx 0x0006eb24
  000ce83a: mov r5,r0
  000ce83c: movs r0,#0x0
  000ce83e: mov.w r1,#0x3e8
  000ce842: strd r0,r0,[sp,#0x0]
  000ce846: mov r0,r5
  000ce848: movs r2,#0x64
  000ce84a: movs r3,#0x0
  000ce84c: blx 0x00073d80
  000ce850: mov.w r0,#0x168
  000ce854: blx 0x0006eb24
  000ce858: vmov s0,r9
  000ce85c: vmov s4,r11
  000ce860: mov r11,r0
  000ce862: vmov s2,r10
  000ce866: vcvt.f32.s32 s0,s0
  000ce86a: vcvt.f32.s32 s2,s2
  000ce86e: vcvt.f32.s32 s4,s4
  000ce872: mov r1,r8
  000ce874: mov r2,r5
  000ce876: mov r3,r4
  000ce878: vstr.32 s4,[sp]
  000ce87c: vstr.32 s2,[sp,#0x4]
  000ce880: vstr.32 s0,[sp,#0x8]
  000ce884: blx 0x00074320
  000ce888: ldr.w r0,[r11,#0x0]
  000ce88c: ldr.w r8,[sp,#0x38]
  000ce890: ldr r2,[r0,#0x14]
  000ce892: mov r0,r11
  000ce894: mov r1,r8
  000ce896: blx r2
  000ce898: ldr r0,[0x000cea4c]
  000ce89a: movw r1,#0x682
  000ce89e: add r0,pc
  000ce8a0: ldr r0,[r0,#0x0]
  000ce8a2: ldr r0,[r0,#0x0]
  000ce8a4: blx 0x00072f70
  000ce8a8: mov r1,r0
  000ce8aa: add.w r0,r11,#0x18
  000ce8ae: blx 0x0006f2b0
  000ce8b2: movs r0,#0x0
  000ce8b4: str.w r0,[r11,#0x50]
  000ce8b8: ldr.w r0,[r11,#0x0]
  000ce8bc: mov r1,r8
  000ce8be: ldr r2,[r0,#0x14]
  000ce8c0: mov r0,r11
  000ce8c2: blx r2
  000ce8c4: ldr r0,[sp,#0x84]
  000ce8c6: ldr r1,[sp,#0x34]
  000ce8c8: ldr r1,[r1,#0x0]
  000ce8ca: subs r0,r1,r0
  000ce8cc: itttt eq
  000ce8ce: mov.eq r0,r11
  000ce8d0: add.eq sp,#0x88
  000ce8d2: vpop.eq {d8,d9,d10,d11,d12}
  000ce8d6: add.eq sp,#0x4
  000ce8d8: itt eq
  000ce8da: pop.eq.w {r8,r9,r10,r11}
  000ce8de: pop.eq {r4,r5,r6,r7,pc}
  000ce8e0: blx 0x0006e824
  000ce8e4: movw r0,#0x1a76
  000ce8e8: cmp r8,r0
  000ce8ea: beq 0x000ce808
  000ce8ec: ldr r0,[0x000cea50]
  000ce8ee: add r0,pc
  000ce8f0: ldr r0,[r0,#0x0]
  000ce8f2: ldr r2,[r0,#0x0]
  000ce8f4: uxth.w r1,r8
  000ce8f8: mov r0,r4
  000ce8fa: movs r3,#0x0
  000ce8fc: blx 0x0007207c
  000ce900: b 0x000ce816
  000ce902: movw r0,#0x37a3
  000ce906: cmp r8,r0
  000ce908: beq 0x000ce990
  000ce90a: movw r0,#0x381b
  000ce90e: cmp r8,r0
  000ce910: beq 0x000ce832
  000ce912: movw r0,#0x381d
  000ce916: cmp r8,r0
  000ce918: beq 0x000ce9a8
  000ce91a: movw r0,#0x4220
  000ce91e: cmp r8,r0
  000ce920: beq.w 0x000cea60
  000ce924: movw r0,#0x4a88
  000ce928: cmp r8,r0
  000ce92a: beq.w 0x000cec12
  000ce92e: movw r0,#0x4a6a
  000ce932: cmp r8,r0
  000ce934: bne.w 0x000cee6c
  000ce938: mov.w r0,#0x114
  000ce93c: blx 0x0006eb24
  000ce940: mov r5,r0
  000ce942: ldr r2,[0x000cea54]
  000ce944: movs r0,#0x0
  000ce946: mov.w r1,#0x3e8
  000ce94a: strd r0,r0,[sp,#0x0]
  000ce94e: mov r0,r5
  000ce950: movs r3,#0x0
  000ce952: blx 0x00073d80
  000ce956: mov.w r0,#0x130
  000ce95a: blx 0x0006eb24
  000ce95e: vmov s0,r11
  000ce962: mov r11,r0
  000ce964: vmov s2,r10
  000ce968: vcvt.f32.s32 s0,s0
  000ce96c: vmov r3,s0
  000ce970: vmov s0,r9
  000ce974: vcvt.f32.s32 s0,s0
  000ce978: vcvt.f32.s32 s2,s2
  000ce97c: movw r1,#0x4a6a
  000ce980: mov r2,r4
  000ce982: vstr.32 s2,[sp]
  000ce986: vstr.32 s0,[sp,#0x4]
  000ce98a: blx 0x000740ec
  000ce98e: b 0x000ce7ea
  000ce990: ldr r0,[0x000cea58]
  000ce992: add r0,pc
  000ce994: ldr r5,[r0,#0x0]
  000ce996: ldr r0,[r5,#0x0]
  000ce998: blx 0x00071c5c
  000ce99c: cmp r0,#0x14
  000ce99e: ble.w 0x000cefde
  000ce9a2: mov.w r4,#0x140
  000ce9a6: b 0x000cefec
  000ce9a8: mov.w r0,#0x114
  000ce9ac: blx 0x0006eb24
  000ce9b0: mov r6,r0
  000ce9b2: movs r0,#0x0
  000ce9b4: mov.w r1,#0x3e8
  000ce9b8: strd r0,r0,[sp,#0x0]
  000ce9bc: mov r0,r6
  000ce9be: movs r2,#0x64
  000ce9c0: movs r3,#0x0
  000ce9c2: blx 0x00073d80
  000ce9c6: mov.w r0,#0x168
  000ce9ca: blx 0x0006eb24
  000ce9ce: vmov s0,r9
  000ce9d2: vmov s4,r11
  000ce9d6: mov r11,r0
  000ce9d8: vmov s2,r10
  000ce9dc: vcvt.f32.s32 s0,s0
  000ce9e0: vcvt.f32.s32 s2,s2
  000ce9e4: vcvt.f32.s32 s4,s4
  000ce9e8: movw r1,#0x381d
  000ce9ec: mov r2,r6
  000ce9ee: mov r3,r4
  000ce9f0: vstr.32 s4,[sp]
  000ce9f4: vstr.32 s2,[sp,#0x4]
  000ce9f8: vstr.32 s0,[sp,#0x8]
  000ce9fc: blx 0x00074320
  000cea00: ldr.w r0,[r11,#0x0]
  000cea04: ldr.w r8,[sp,#0x38]
  000cea08: ldr r2,[r0,#0x14]
  000cea0a: mov r0,r11
  000cea0c: mov r1,r8
  000cea0e: blx r2
  000cea10: ldr r0,[0x000cea5c]
  000cea12: movw r1,#0x681
  000cea16: add r0,pc
  000cea18: ldr r0,[r0,#0x0]
  000cea1a: ldr r0,[r0,#0x0]
  000cea1c: blx 0x00072f70
  000cea20: mov r1,r0
  000cea22: add.w r0,r11,#0x18
  000cea26: blx 0x0006f2b0
  000cea2a: movs r0,#0x0
  000cea2c: strb.w r0,[r11,#0x25]
  000cea30: b 0x000ce8b4
  000cea60: mov.w r0,#0x114
  000cea64: blx 0x0006eb24
  000cea68: mov r4,r0
  000cea6a: ldr r2,[0x000cee4c]
  000cea6c: movs r5,#0x0
  000cea6e: mov.w r1,#0x3e8
  000cea72: movs r3,#0x0
  000cea74: strd r5,r5,[sp,#0x0]
  000cea78: blx 0x00073d80
  000cea7c: mov.w r0,#0x1bc
  000cea80: blx 0x0006eb24
  000cea84: vmov s0,r11
  000cea88: mov r11,r0
  000cea8a: vcvt.f32.s32 s16,s0
  000cea8e: vmov s0,r10
  000cea92: vcvt.f32.s32 s18,s0
  000cea96: vmov s0,r9
  000cea9a: vcvt.f32.s32 s20,s0
  000cea9e: movw r1,#0x4220
  000ceaa2: movs r2,#0x0
  000ceaa4: mov r3,r4
  000ceaa6: vstr.32 s16,[sp,#0x4]
  000ceaaa: vstr.32 s18,[sp,#0x8]
  000ceaae: vstr.32 s20,[sp,#0xc]
  000ceab2: str r5,[sp,#0x0]
  000ceab4: blx 0x0007414c
  000ceab8: movs r0,#0xc0
  000ceaba: blx 0x0006eb24
  000ceabe: mov r4,r0
  000ceac0: ldr r0,[0x000cee50]
  000ceac2: add r0,pc
  000ceac4: ldr r6,[r0,#0x0]
  000ceac6: ldr r2,[r6,#0x0]
  000ceac8: mov r0,r4
  000ceaca: movw r1,#0x4220
  000ceace: movs r3,#0x0
  000cead0: blx 0x0007207c
  000cead4: ldr.w r0,[r11,#0x0]
  000cead8: mov r1,r4
  000ceada: mov.w r2,#0xffffffff
  000ceade: movs r3,#0x0
  000ceae0: ldr r5,[r0,#0x8]
  000ceae2: mov r0,r11
  000ceae4: blx r5
  000ceae6: movs r0,#0xc0
  000ceae8: blx 0x0006eb24
  000ceaec: mov r4,r0
  000ceaee: ldr r2,[r6,#0x0]
  000ceaf0: movw r1,#0x4221
  000ceaf4: movs r3,#0x0
  000ceaf6: blx 0x0007207c
  000ceafa: movs r0,#0xc0
  000ceafc: blx 0x0006eb24
  000ceb00: mov r5,r0
  000ceb02: ldr r2,[r6,#0x0]
  000ceb04: movw r1,#0x4222
  000ceb08: movs r3,#0x0
  000ceb0a: mov.w r9,#0x0
  000ceb0e: blx 0x0007207c
  000ceb12: ldr r1,[r4,#0xc]
  000ceb14: ldr.w r0,[r11,#0x8]
  000ceb18: blx 0x000720e8
  000ceb1c: ldr r1,[r5,#0xc]
  000ceb1e: ldr.w r0,[r11,#0x8]
  000ceb22: blx 0x000720e8
  000ceb26: mov r0,r5
  000ceb28: blx 0x00071fc8
  000ceb2c: blx 0x0006eb48
  000ceb30: mov r0,r4
  000ceb32: blx 0x00071fc8
  000ceb36: blx 0x0006eb48
  000ceb3a: ldr r2,[0x000cee54]
  000ceb3c: ldr.w r1,[r11,#0x8]
  000ceb40: add r2,pc
  000ceb42: ldr r0,[r6,#0x0]
  000ceb44: ldr r1,[r1,#0xc]
  000ceb46: ldr r5,[r2,#0x0]
  000ceb48: blx r5
  000ceb4a: ldr.w r1,[r11,#0x8]
  000ceb4e: mov r4,r0
  000ceb50: ldr r0,[r6,#0x0]
  000ceb52: ldr r1,[r1,#0xc]
  000ceb54: blx r5
  000ceb56: ldr r1,[0x000cee58]
  000ceb58: ldrd r2,r3,[r0,#0xf8]
  000ceb5c: mov r0,r4
  000ceb5e: add r1,pc
  000ceb60: str.w r9,[sp,#0x0]
  000ceb64: ldr.w r10,[r1,#0x0]
  000ceb68: blx r10
  000ceb6a: ldr.w r1,[r11,#0x8]
  000ceb6e: ldr r0,[r6,#0x0]
  000ceb70: ldr r1,[r1,#0x14]
  000ceb72: blx r5
  000ceb74: ldr.w r1,[r11,#0x8]
  000ceb78: mov r4,r0
  000ceb7a: ldr r0,[r6,#0x0]
  000ceb7c: ldr r1,[r1,#0x14]
  000ceb7e: blx r5
  000ceb80: ldrd r2,r3,[r0,#0xf8]
  000ceb84: mov r0,r4
  000ceb86: str.w r9,[sp,#0x0]
  000ceb8a: blx r10
  000ceb8c: ldr.w r1,[r11,#0x8]
  000ceb90: ldr r0,[r6,#0x0]
  000ceb92: ldr r1,[r1,#0x10]
  000ceb94: blx r5
  000ceb96: ldr.w r1,[r11,#0x8]
  000ceb9a: mov r4,r0
  000ceb9c: ldr r0,[r6,#0x0]
  000ceb9e: ldr r1,[r1,#0x10]
  000ceba0: blx r5
  000ceba2: ldrd r2,r3,[r0,#0xf8]
  000ceba6: mov r0,r4
  000ceba8: str.w r9,[sp,#0x0]
  000cebac: blx r10
  000cebae: movs r0,#0x1
  000cebb0: movw r1,#0x37a6
  000cebb4: strb.w r0,[r11,#0x70]
  000cebb8: mov r0,r11
  000cebba: blx 0x0007432c
  000cebbe: movs r0,#0x1
  000cebc0: blx 0x0006eb24
  000cebc4: mov r4,r0
  000cebc6: blx 0x00072124
  000cebca: mov r0,r4
  000cebcc: movs r1,#0x7
  000cebce: mov.w r2,#0xffffffff
  000cebd2: blx 0x000740e0
  000cebd6: mov r5,r0
  000cebd8: mov r0,r4
  000cebda: blx 0x0007213c
  000cebde: blx 0x0006eb48
  000cebe2: mov r0,r11
  000cebe4: mov r1,r5
  000cebe6: blx 0x000740a4
  000cebea: movw r1,#0x3eb
  000cebee: blx 0x00074338
  000cebf2: mov r1,r0
  000cebf4: mov r0,r11
  000cebf6: blx 0x00074344
  000cebfa: vmov r1,s16
  000cebfe: ldr.w r0,[r11,#0x0]
  000cec02: vmov r2,s18
  000cec06: vmov r3,s20
  000cec0a: ldr r6,[r0,#0x48]
  000cec0c: mov r0,r11
  000cec0e: blx r6
  000cec10: b 0x000ce7ea
  000cec12: mov.w r0,#0x114
  000cec16: blx 0x0006eb24
  000cec1a: mov r4,r0
  000cec1c: ldr r2,[0x000cee4c]
  000cec1e: movs r5,#0x0
  000cec20: mov.w r1,#0x3e8
  000cec24: movs r3,#0x0
  000cec26: strd r5,r5,[sp,#0x0]
  000cec2a: blx 0x00073d80
  000cec2e: mov r0,r4
  000cec30: movs r1,#0x0
  000cec32: blx 0x0007294c
  000cec36: mov.w r0,#0x1bc
  000cec3a: blx 0x0006eb24
  000cec3e: vmov s0,r11
  000cec42: mov r8,r0
  000cec44: vcvt.f32.s32 s16,s0
  000cec48: vmov s0,r10
  000cec4c: vcvt.f32.s32 s18,s0
  000cec50: vmov s0,r9
  000cec54: vcvt.f32.s32 s20,s0
  000cec58: movw r1,#0x4a88
  000cec5c: movs r2,#0x3
  000cec5e: mov r3,r4
  000cec60: vstr.32 s16,[sp,#0x4]
  000cec64: vstr.32 s18,[sp,#0x8]
  000cec68: vstr.32 s20,[sp,#0xc]
  000cec6c: str r5,[sp,#0x0]
  000cec6e: blx 0x0007414c
  000cec72: movs r0,#0xc0
  000cec74: blx 0x0006eb24
  000cec78: mov r10,r0
  000cec7a: ldr r0,[0x000cee5c]
  000cec7c: add r0,pc
  000cec7e: ldr r4,[r0,#0x0]
  000cec80: ldr r2,[r4,#0x0]
  000cec82: mov r0,r10
  000cec84: movw r1,#0x4a88
  000cec88: movs r3,#0x0
  000cec8a: blx 0x0007207c
  000cec8e: movs r0,#0xc0
  000cec90: blx 0x0006eb24
  000cec94: ldr r2,[r4,#0x0]
  000cec96: movw r1,#0x4a8d
  000cec9a: movs r3,#0x0
  000cec9c: str r0,[sp,#0x30]
  000cec9e: blx 0x0007207c
  000ceca2: movs r0,#0xc0
  000ceca4: blx 0x0006eb24
  000ceca8: mov r11,r0
  000cecaa: ldr r2,[r4,#0x0]
  000cecac: movw r1,#0x4a8c
  000cecb0: movs r3,#0x0
  000cecb2: blx 0x0007207c
  000cecb6: movs r0,#0xc0
  000cecb8: blx 0x0006eb24
  000cecbc: mov r6,r0
  000cecbe: ldr r2,[r4,#0x0]
  000cecc0: movw r1,#0x4a89
  000cecc4: movs r3,#0x0
  000cecc6: blx 0x0007207c
  000cecca: movs r0,#0xc0
  000ceccc: blx 0x0006eb24
  000cecd0: mov r5,r0
  000cecd2: ldr r2,[r4,#0x0]
  000cecd4: movw r1,#0x4a8a
  000cecd8: movs r3,#0x0
  000cecda: blx 0x0007207c
  000cecde: movs r0,#0xc0
  000cece0: mov r9,r8
  000cece2: str r5,[sp,#0x24]
  000cece4: blx 0x0006eb24
  000cece8: ldr r2,[r4,#0x0]
  000cecea: movw r1,#0x4a8b
  000cecee: movs r3,#0x0
  000cecf0: str r0,[sp,#0x28]
  000cecf2: blx 0x0007207c
  000cecf6: ldr r0,[0x000cee60]
  000cecf8: ldr r1,[sp,#0x30]
  000cecfa: add r0,pc
  000cecfc: ldr.w r8,[r0,#0x0]
  000ced00: mov r0,r10
  000ced02: ldr r1,[r1,#0xc]
  000ced04: blx r8
  000ced06: ldr.w r1,[r11,#0xc]
  000ced0a: mov r0,r10
  000ced0c: str.w r11,[sp,#0x20]
  000ced10: blx r8
  000ced12: ldr r1,[r6,#0xc]
  000ced14: mov r0,r10
  000ced16: str r6,[sp,#0x1c]
  000ced18: blx r8
  000ced1a: movs r0,#0xc0
  000ced1c: blx 0x0006eb24
  000ced20: mov r5,r0
  000ced22: ldr r2,[r4,#0x0]
  000ced24: movw r1,#0x4a8e
  000ced28: movs r3,#0x0
  000ced2a: blx 0x0007207c
  000ced2e: movs r0,#0xc0
  000ced30: mov r6,r10
  000ced32: str.w r9,[sp,#0x2c]
  000ced36: blx 0x0006eb24
  000ced3a: mov r10,r0
  000ced3c: ldr r2,[r4,#0x0]
  000ced3e: movw r1,#0x4a90
  000ced42: movs r3,#0x0
  000ced44: blx 0x0007207c
  000ced48: movs r0,#0xc0
  000ced4a: blx 0x0006eb24
  000ced4e: mov r11,r0
  000ced50: ldr r2,[r4,#0x0]
  000ced52: movw r1,#0x4a8f
  000ced56: movs r3,#0x0
  000ced58: blx 0x0007207c
  000ced5c: ldr r1,[r5,#0xc]
  000ced5e: mov r0,r6
  000ced60: mov r9,r6
  000ced62: blx r8
  000ced64: ldr.w r1,[r10,#0xc]
  000ced68: mov r0,r6
  000ced6a: blx r8
  000ced6c: ldr.w r1,[r11,#0xc]
  000ced70: mov r0,r6
  000ced72: blx r8
  000ced74: ldr r0,[0x000cee64]
  000ced76: add r0,pc
  000ced78: ldr r4,[r0,#0x0]
  000ced7a: mov r0,r5
  000ced7c: blx r4
  000ced7e: ldr r0,[0x000cee68]
  000ced80: add r0,pc
  000ced82: ldr r6,[r0,#0x0]
  000ced84: mov r0,r5
  000ced86: blx r6
  000ced88: mov r0,r10
  000ced8a: blx r4
  000ced8c: blx r6
  000ced8e: mov r0,r11
  000ced90: blx r4
  000ced92: blx r6
  000ced94: ldr r5,[sp,#0x24]
  000ced96: mov r0,r9
  000ced98: ldr r1,[r5,#0xc]
  000ced9a: blx r8
  000ced9c: ldr.w r10,[sp,#0x28]
  000ceda0: mov r0,r9
  000ceda2: ldr.w r1,[r10,#0xc]
  000ceda6: blx r8
  000ceda8: ldr r0,[sp,#0x30]
  000cedaa: blx r4
  000cedac: blx r6
  000cedae: ldr r0,[sp,#0x20]
  000cedb0: blx r4
  000cedb2: blx r6
  000cedb4: ldr r0,[sp,#0x1c]
  000cedb6: blx r4
  000cedb8: blx r6
  000cedba: mov r0,r5
  000cedbc: blx r4
  000cedbe: blx r6
  000cedc0: mov r0,r10
  000cedc2: blx r4
  000cedc4: blx r6
  000cedc6: ldr.w r11,[sp,#0x2c]
  000cedca: mov r1,r9
  000cedcc: mov.w r2,#0xffffffff
  000cedd0: movs r3,#0x0
  000cedd2: mov.w r8,#0x0
  000cedd6: ldr.w r0,[r11,#0x0]
  000cedda: ldr r6,[r0,#0x8]
  000ceddc: mov r0,r11
  000cedde: blx r6
  000cede0: vmov r1,s16
  000cede4: ldr.w r0,[r11,#0x0]
  000cede8: vmov r2,s18
  000cedec: movs r6,#0x1
  000cedee: vmov r3,s20
  000cedf2: strb.w r6,[r11,#0x70]
  000cedf6: ldr r6,[r0,#0x48]
  000cedf8: mov r0,r11
  000cedfa: blx r6
  000cedfc: mov r0,r11
  000cedfe: movw r1,#0x4a88
  000cee02: blx 0x0007432c
  000cee06: movs r0,#0x1
  000cee08: blx 0x0006eb24
  000cee0c: mov r4,r0
  000cee0e: blx 0x00072124
  000cee12: mov r0,r4
  000cee14: movs r1,#0x1
  000cee16: mov.w r2,#0xffffffff
  000cee1a: blx 0x000740e0
  000cee1e: mov r5,r0
  000cee20: mov r0,r4
  000cee22: blx 0x0007213c
  000cee26: blx 0x0006eb48
  000cee2a: mov r0,r11
  000cee2c: mov r1,r5
  000cee2e: blx 0x000740a4
  000cee32: mov.w r1,#0x7d0
  000cee36: blx 0x00074338
  000cee3a: mov r1,r0
  000cee3c: mov r0,r11
  000cee3e: blx 0x00074344
  000cee42: ldr.w r0,[r11,#0x4]
  000cee46: str.w r8,[r0,#0x40]
  000cee4a: b 0x000ce7ea
  000cee6c: movw r0,#0x493e
  000cee70: sub.w r0,r8,r0
  000cee74: cmp r0,#0x1f
  000cee76: bhi.w 0x000cf360
  000cee7a: movs r1,#0x1
  000cee7c: movw r2,#0x1249
  000cee80: lsls r1,r0
  000cee82: tst r1,r2
  000cee84: beq.w 0x000cf14c
  000cee88: mov.w r0,#0x114
  000cee8c: blx 0x0006eb24
  000cee90: mov r4,r0
  000cee92: ldr r2,[0x000cf138]
  000cee94: movs r5,#0x0
  000cee96: mov.w r1,#0x3e8
  000cee9a: movs r3,#0x0
  000cee9c: strd r5,r5,[sp,#0x0]
  000ceea0: blx 0x00073d80
  000ceea4: mov r0,r4
  000ceea6: movs r1,#0x0
  000ceea8: blx 0x0007294c
  000ceeac: mov.w r0,#0x1bc
  000ceeb0: blx 0x0006eb24
  000ceeb4: vmov s0,r11
  000ceeb8: vcvt.f32.s32 s16,s0
  000ceebc: vmov s0,r10
  000ceec0: vcvt.f32.s32 s18,s0
  000ceec4: vmov s0,r9
  000ceec8: vcvt.f32.s32 s20,s0
  000ceecc: mov r1,r8
  000ceece: movs r2,#0x3
  000ceed0: mov r3,r4
  000ceed2: vstr.32 s16,[sp,#0x4]
  000ceed6: vstr.32 s18,[sp,#0x8]
  000ceeda: mov r6,r0
  000ceedc: vstr.32 s20,[sp,#0xc]
  000ceee0: str r5,[sp,#0x0]
  000ceee2: blx 0x0007414c
  000ceee6: movs r0,#0xc0
  000ceee8: blx 0x0006eb24
  000ceeec: mov r11,r0
  000ceeee: ldr r0,[0x000cf13c]
  000ceef0: add r0,pc
  000ceef2: ldr r5,[r0,#0x0]
  000ceef4: ldr r2,[r5,#0x0]
  000ceef6: uxth.w r1,r8
  000ceefa: mov r0,r11
  000ceefc: movs r3,#0x0
  000ceefe: blx 0x0007207c
  000cef02: movs r0,#0xc0
  000cef04: blx 0x0006eb24
  000cef08: mov r9,r0
  000cef0a: ldr r2,[r5,#0x0]
  000cef0c: add.w r0,r8,#0x2
  000cef10: movs r3,#0x0
  000cef12: uxth r1,r0
  000cef14: mov r0,r9
  000cef16: blx 0x0007207c
  000cef1a: movs r0,#0xc0
  000cef1c: blx 0x0006eb24
  000cef20: mov r4,r0
  000cef22: ldr r2,[r5,#0x0]
  000cef24: add.w r0,r8,#0x1
  000cef28: movs r3,#0x0
  000cef2a: uxth r1,r0
  000cef2c: mov r0,r4
  000cef2e: blx 0x0007207c
  000cef32: ldr.w r1,[r9,#0xc]
  000cef36: mov r0,r11
  000cef38: blx 0x000720e8
  000cef3c: ldr r1,[r4,#0xc]
  000cef3e: mov r0,r11
  000cef40: blx 0x000720e8
  000cef44: movw r0,#0x494a
  000cef48: cmp r8,r0
  000cef4a: bne 0x000cef88
  000cef4c: ldr r0,[0x000cf140]
  000cef4e: add r0,pc
  000cef50: ldr r0,[r0,#0x0]
  000cef52: ldr r0,[r0,#0x0]
  000cef54: blx 0x00071770
  000cef58: cmp r0,#0x91
  000cef5a: bne 0x000cef88
  000cef5c: movs r0,#0xc0
  000cef5e: blx 0x0006eb24
  000cef62: mov r10,r0
  000cef64: ldr r2,[r5,#0x0]
  000cef66: add.w r0,r8,#0x3
  000cef6a: movs r3,#0x0
  000cef6c: uxth r1,r0
  000cef6e: mov r0,r10
  000cef70: blx 0x0007207c
  000cef74: ldr.w r1,[r10,#0xc]
  000cef78: mov r0,r11
  000cef7a: blx 0x000720e8
  000cef7e: mov r0,r10
  000cef80: blx 0x00071fc8
  000cef84: blx 0x0006eb48
  000cef88: mov r0,r9
  000cef8a: blx 0x00071fc8
  000cef8e: blx 0x0006eb48
  000cef92: mov r0,r4
  000cef94: blx 0x00071fc8
  000cef98: blx 0x0006eb48
  000cef9c: ldr r1,[r6,#0x0]
  000cef9e: mov r0,r6
  000cefa0: mov.w r2,#0xffffffff
  000cefa4: movs r3,#0x0
  000cefa6: ldr r6,[r1,#0x8]
  000cefa8: mov r1,r11
  000cefaa: mov r11,r0
  000cefac: blx r6
  000cefae: vmov r1,s16
  000cefb2: ldr.w r0,[r11,#0x0]
  000cefb6: vmov r2,s18
  000cefba: movs r6,#0x1
  000cefbc: vmov r3,s20
  000cefc0: strb.w r6,[r11,#0x70]
  000cefc4: ldr r6,[r0,#0x48]
  000cefc6: mov r0,r11
  000cefc8: blx r6
  000cefca: movw r1,#0x7d1
  000cefce: blx 0x00074338
  000cefd2: mov r1,r0
  000cefd4: mov r0,r11
  000cefd6: blx 0x00074344
  000cefda: b.w 0x000ce7ea
  000cefde: ldr r0,[r5,#0x0]
  000cefe0: blx 0x00071c5c
  000cefe4: rsb r0,r0,r0, lsl #0x4
  000cefe8: add.w r4,r0,#0x14
  000cefec: ldr r0,[r5,#0x0]
  000cefee: blx 0x00073c48
  000ceff2: cbz r0,0x000ceff8
  000ceff4: movs r0,#0xb4
  000ceff6: b 0x000cf000
  000ceff8: ldr r0,[r5,#0x0]
  000ceffa: blx 0x00071770
  000ceffe: lsls r0,r0,#0x2
  000cf000: ldr r1,[0x000cf144]
  000cf002: vmov.f32 s0,0xbf000000
  000cf006: add r0,r4
  000cf008: add r1,pc
  000cf00a: add.w r0,r0,r0, lsl #0x2
  000cf00e: ldr r1,[r1,#0x0]
  000cf010: vmov s4,r0
  000cf014: mov.w r0,#0x114
  000cf018: vldr.32 s2,[r1,#0x2c]
  000cf01c: vcvt.f32.s32 s16,s4
  000cf020: vadd.f32 s0,s2,s0
  000cf024: vmla.f32 s16,s0,s16
  000cf028: blx 0x0006eb24
  000cf02c: mov r4,r0
  000cf02e: movs r5,#0x0
  000cf030: movw r1,#0x1d4c
  000cf034: str r5,[sp,#0x4]
  000cf036: vcvt.s32.f32 s0,s16
  000cf03a: movs r3,#0x0
  000cf03c: str r5,[sp,#0x0]
  000cf03e: vmov r2,s0
  000cf042: blx 0x00073d80
  000cf046: mov.w r0,#0x1bc
  000cf04a: blx 0x0006eb24
  000cf04e: vmov s0,r11
  000cf052: mov r11,r0
  000cf054: vcvt.f32.s32 s16,s0
  000cf058: vmov s0,r10
  000cf05c: vcvt.f32.s32 s18,s0
  000cf060: vmov s0,r9
  000cf064: vcvt.f32.s32 s20,s0
  000cf068: movw r1,#0x37a3
  000cf06c: movs r2,#0x8
  000cf06e: mov r3,r4
  000cf070: vstr.32 s16,[sp,#0x4]
  000cf074: vstr.32 s18,[sp,#0x8]
  000cf078: vstr.32 s20,[sp,#0xc]
  000cf07c: str r5,[sp,#0x0]
  000cf07e: blx 0x0007414c
  000cf082: movs r0,#0xc0
  000cf084: blx 0x0006eb24
  000cf088: mov r4,r0
  000cf08a: ldr r0,[0x000cf148]
  000cf08c: add r0,pc
  000cf08e: ldr r6,[r0,#0x0]
  000cf090: ldr r2,[r6,#0x0]
  000cf092: mov r0,r4
  000cf094: movw r1,#0x37a3
  000cf098: movs r3,#0x0
  000cf09a: blx 0x0007207c
  000cf09e: ldr.w r8,[sp,#0x38]
  000cf0a2: ldr.w r0,[r11,#0x0]
  000cf0a6: mov r1,r4
  000cf0a8: mov.w r2,#0xffffffff
  000cf0ac: movs r3,#0x0
  000cf0ae: ldr r5,[r0,#0x8]
  000cf0b0: mov r0,r11
  000cf0b2: blx r5
  000cf0b4: movs r0,#0xc0
  000cf0b6: blx 0x0006eb24
  000cf0ba: mov r4,r0
  000cf0bc: ldr r2,[r6,#0x0]
  000cf0be: movw r1,#0x37a4
  000cf0c2: movs r3,#0x0
  000cf0c4: blx 0x0007207c
  000cf0c8: movs r0,#0xc0
  000cf0ca: blx 0x0006eb24
  000cf0ce: mov r5,r0
  000cf0d0: ldr r2,[r6,#0x0]
  000cf0d2: movw r1,#0x37a5
  000cf0d6: movs r3,#0x0
  000cf0d8: blx 0x0007207c
  000cf0dc: ldr r1,[r4,#0xc]
  000cf0de: ldr.w r0,[r11,#0x8]
  000cf0e2: blx 0x000720e8
  000cf0e6: ldr r1,[r5,#0xc]
  000cf0e8: ldr.w r0,[r11,#0x8]
  000cf0ec: blx 0x000720e8
  000cf0f0: mov r0,r5
  000cf0f2: blx 0x00071fc8
  000cf0f6: blx 0x0006eb48
  000cf0fa: mov r0,r4
  000cf0fc: blx 0x00071fc8
  000cf100: blx 0x0006eb48
  000cf104: mov r0,r11
  000cf106: movw r1,#0x37a6
  000cf10a: blx 0x0007432c
  000cf10e: movw r1,#0x3ea
  000cf112: blx 0x00074338
  000cf116: mov r1,r0
  000cf118: mov r0,r11
  000cf11a: blx 0x00074344
  000cf11e: vmov r1,s16
  000cf122: ldr.w r0,[r11,#0x0]
  000cf126: vmov r2,s18
  000cf12a: vmov r3,s20
  000cf12e: ldr r6,[r0,#0x48]
  000cf130: mov r0,r11
  000cf132: blx r6
  000cf134: b.w 0x000ce8b8
  000cf14c: cmp r0,#0x10
  000cf14e: beq.w 0x000cf490
  000cf152: cmp r0,#0x1f
  000cf154: bne.w 0x000cf360
  000cf158: mov.w r0,#0x114
  000cf15c: blx 0x0006eb24
  000cf160: mov r4,r0
  000cf162: ldr r2,[0x000cf4a8]
  000cf164: movs r5,#0x0
  000cf166: mov.w r1,#0x3e8
  000cf16a: movs r3,#0x0
  000cf16c: strd r5,r5,[sp,#0x0]
  000cf170: blx 0x00073d80
  000cf174: mov.w r0,#0x1bc
  000cf178: blx 0x0006eb24
  000cf17c: vmov s0,r11
  000cf180: mov r11,r0
  000cf182: vcvt.f32.s32 s16,s0
  000cf186: vmov s0,r10
  000cf18a: vcvt.f32.s32 s18,s0
  000cf18e: vmov s0,r9
  000cf192: vcvt.f32.s32 s20,s0
  000cf196: movw r1,#0x495d
  000cf19a: movs r2,#0x3
  000cf19c: mov r3,r4
  000cf19e: vstr.32 s16,[sp,#0x4]
  000cf1a2: vstr.32 s18,[sp,#0x8]
  000cf1a6: vstr.32 s20,[sp,#0xc]
  000cf1aa: str r5,[sp,#0x0]
  000cf1ac: blx 0x0007414c
  000cf1b0: movs r0,#0xc0
  000cf1b2: blx 0x0006eb24
  000cf1b6: mov r9,r0
  000cf1b8: ldr r0,[0x000cf4ac]
  000cf1ba: add r0,pc
  000cf1bc: ldr.w r10,[r0,#0x0]
  000cf1c0: ldr.w r2,[r10,#0x0]
  000cf1c4: mov r0,r9
  000cf1c6: movw r1,#0x5254
  000cf1ca: movs r3,#0x0
  000cf1cc: blx 0x0007207c
  000cf1d0: movs r0,#0xc0
  000cf1d2: blx 0x0006eb24
  000cf1d6: ldr.w r2,[r10,#0x0]
  000cf1da: movw r1,#0x5574
  000cf1de: movs r3,#0x0
  000cf1e0: str r0,[sp,#0x28]
  000cf1e2: blx 0x0007207c
  000cf1e6: movs r0,#0xc0
  000cf1e8: blx 0x0006eb24
  000cf1ec: mov r8,r0
  000cf1ee: ldr.w r2,[r10,#0x0]
  000cf1f2: movw r1,#0x563c
  000cf1f6: movs r3,#0x0
  000cf1f8: blx 0x0007207c
  000cf1fc: movs r0,#0xc0
  000cf1fe: blx 0x0006eb24
  000cf202: mov r6,r0
  000cf204: ldr.w r2,[r10,#0x0]
  000cf208: movw r1,#0x495d
  000cf20c: movs r3,#0x0
  000cf20e: blx 0x0007207c
  000cf212: movs r0,#0xc0
  000cf214: blx 0x0006eb24
  000cf218: mov r5,r0
  000cf21a: ldr.w r2,[r10,#0x0]
  000cf21e: movw r1,#0x495d
  000cf222: movs r3,#0x0
  000cf224: blx 0x0007207c
  000cf228: movs r0,#0xc0
  000cf22a: str.w r11,[sp,#0x2c]
  000cf22e: blx 0x0006eb24
  000cf232: mov r4,r0
  000cf234: ldr.w r2,[r10,#0x0]
  000cf238: movw r1,#0x495e
  000cf23c: movs r3,#0x0
  000cf23e: blx 0x0007207c
  000cf242: movs r0,#0xc0
  000cf244: strd r4,r6,[sp,#0x20]
  000cf248: blx 0x0006eb24
  000cf24c: mov r4,r0
  000cf24e: ldr.w r2,[r10,#0x0]
  000cf252: movw r1,#0x495f
  000cf256: movs r3,#0x0
  000cf258: blx 0x0007207c
  000cf25c: movs r0,#0xc0
  000cf25e: str r4,[sp,#0x30]
  000cf260: mov r11,r5
  000cf262: blx 0x0006eb24
  000cf266: mov r4,r0
  000cf268: ldr.w r2,[r10,#0x0]
  000cf26c: movw r1,#0x4960
  000cf270: movs r3,#0x0
  000cf272: blx 0x0007207c
  000cf276: ldr r0,[0x000cf4b0]
  000cf278: ldr r1,[r4,#0xc]
  000cf27a: add r0,pc
  000cf27c: str r4,[sp,#0x1c]
  000cf27e: ldr r4,[r0,#0x0]
  000cf280: mov r0,r9
  000cf282: blx r4
  000cf284: ldr r5,[sp,#0x28]
  000cf286: mov r0,r9
  000cf288: ldr r1,[r5,#0xc]
  000cf28a: blx r4
  000cf28c: ldr.w r1,[r8,#0xc]
  000cf290: mov r0,r9
  000cf292: mov r6,r8
  000cf294: blx r4
  000cf296: ldr.w r10,[sp,#0x24]
  000cf29a: mov r0,r9
  000cf29c: ldr.w r1,[r10,#0xc]
  000cf2a0: blx r4
  000cf2a2: ldr.w r1,[r11,#0xc]
  000cf2a6: mov r0,r9
  000cf2a8: blx r4
  000cf2aa: ldr.w r8,[sp,#0x20]
  000cf2ae: mov r0,r9
  000cf2b0: ldr.w r1,[r8,#0xc]
  000cf2b4: blx r4
  000cf2b6: ldr r0,[sp,#0x30]
  000cf2b8: ldr r1,[r0,#0xc]
  000cf2ba: mov r0,r9
  000cf2bc: blx r4
  000cf2be: ldr r0,[0x000cf4b4]
  000cf2c0: add r0,pc
  000cf2c2: ldr r4,[r0,#0x0]
  000cf2c4: mov r0,r5
  000cf2c6: blx r4
  000cf2c8: ldr r1,[0x000cf4b8]
  000cf2ca: add r1,pc
  000cf2cc: ldr r5,[r1,#0x0]
  000cf2ce: blx r5
  000cf2d0: mov r0,r6
  000cf2d2: blx r4
  000cf2d4: blx r5
  000cf2d6: mov r0,r10
  000cf2d8: blx r4
  000cf2da: blx r5
  000cf2dc: mov r0,r11
  000cf2de: blx r4
  000cf2e0: blx r5
  000cf2e2: mov r0,r8
  000cf2e4: blx r4
  000cf2e6: blx r5
  000cf2e8: ldr r0,[sp,#0x30]
  000cf2ea: blx r4
  000cf2ec: blx r5
  000cf2ee: ldr r0,[sp,#0x1c]
  000cf2f0: blx r4
  000cf2f2: blx r5
  000cf2f4: ldr.w r11,[sp,#0x2c]
  000cf2f8: mov r1,r9
  000cf2fa: mov.w r2,#0xffffffff
  000cf2fe: movs r3,#0x0
  000cf300: ldr.w r0,[r11,#0x0]
  000cf304: ldr r5,[r0,#0x8]
  000cf306: mov r0,r11
  000cf308: blx r5
  000cf30a: vmov r1,s16
  000cf30e: ldr.w r0,[r11,#0x0]
  000cf312: vmov r2,s18
  000cf316: movs r6,#0x1
  000cf318: vmov r3,s20
  000cf31c: strb.w r6,[r11,#0x70]
  000cf320: ldr r6,[r0,#0x48]
  000cf322: mov r0,r11
  000cf324: blx r6
  000cf326: mov r0,r11
  000cf328: movw r1,#0x477c
  000cf32c: blx 0x0007432c
  000cf330: movs r0,#0x1
  000cf332: blx 0x0006eb24
  000cf336: mov r4,r0
  000cf338: blx 0x00072124
  000cf33c: mov r0,r4
  000cf33e: movs r1,#0x6
  000cf340: mov.w r2,#0xffffffff
  000cf344: blx 0x000740e0
  000cf348: mov r5,r0
  000cf34a: mov r0,r4
  000cf34c: blx 0x0007213c
  000cf350: blx 0x0006eb48
  000cf354: mov r0,r11
  000cf356: mov r1,r5
  000cf358: blx 0x000740a4
  000cf35c: movs r1,#0x6f
  000cf35e: b 0x000cefce
  000cf360: movw r0,#0x4260
  000cf364: cmp r8,r0
  000cf366: beq.w 0x000cf4c4
  000cf36a: movw r0,#0x4299
  000cf36e: cmp r8,r0
  000cf370: beq.w 0x000cf66e
  000cf374: movw r0,#0x4961
  000cf378: cmp r8,r0
  000cf37a: beq.w 0x000cf686
  000cf37e: movw r0,#0x4962
  000cf382: cmp r8,r0
  000cf384: beq.w 0x000cf5a4
  000cf388: movw r0,#0x4974
  000cf38c: cmp r8,r0
  000cf38e: beq.w 0x000cf656
  000cf392: movw r0,#0x4a6b
  000cf396: cmp r8,r0
  000cf398: beq.w 0x000cf794
  000cf39c: movw r0,#0x5279
  000cf3a0: cmp r8,r0
  000cf3a2: bne.w 0x000cf8dc
  000cf3a6: mov.w r0,#0x114
  000cf3aa: blx 0x0006eb24
  000cf3ae: mov r4,r0
  000cf3b0: ldr r2,[0x000cf4a8]
  000cf3b2: movs r5,#0x0
  000cf3b4: movs r1,#0x0
  000cf3b6: movs r3,#0x0
  000cf3b8: strd r5,r5,[sp,#0x0]
  000cf3bc: blx 0x00073d80
  000cf3c0: mov r0,r4
  000cf3c2: movs r1,#0x0
  000cf3c4: blx 0x0007294c
  000cf3c8: mov.w r0,#0x1bc
  000cf3cc: blx 0x0006eb24
  000cf3d0: vmov s0,r11
  000cf3d4: mov r11,r0
  000cf3d6: vcvt.f32.s32 s16,s0
  000cf3da: vmov s0,r10
  000cf3de: vcvt.f32.s32 s18,s0
  000cf3e2: vmov s0,r9
  000cf3e6: vcvt.f32.s32 s20,s0
  000cf3ea: movw r1,#0x41a0
  000cf3ee: movs r2,#0x1
  000cf3f0: mov r3,r4
  000cf3f2: vstr.32 s16,[sp,#0x4]
  000cf3f6: vstr.32 s18,[sp,#0x8]
  000cf3fa: vstr.32 s20,[sp,#0xc]
  000cf3fe: str r5,[sp,#0x0]
  000cf400: blx 0x0007414c
  000cf404: movs r0,#0xc0
  000cf406: blx 0x0006eb24
  000cf40a: mov r4,r0
  000cf40c: ldr r0,[0x000cf4bc]
  000cf40e: add r0,pc
  000cf410: ldr r0,[r0,#0x0]
  000cf412: ldr r2,[r0,#0x0]
  000cf414: mov r0,r4
  000cf416: movw r1,#0x41a0
  000cf41a: movs r3,#0x0
  000cf41c: blx 0x0007207c
  000cf420: ldr.w r8,[sp,#0x38]
  000cf424: ldr.w r0,[r11,#0x0]
  000cf428: mov r1,r4
  000cf42a: mov.w r2,#0xffffffff
  000cf42e: movs r3,#0x0
  000cf430: ldr r6,[r0,#0x8]
  000cf432: mov r0,r11
  000cf434: blx r6
  000cf436: vmov r1,s16
  000cf43a: ldr.w r0,[r11,#0x0]
  000cf43e: vmov r2,s18
  000cf442: movs r6,#0x1
  000cf444: vmov r3,s20
  000cf448: strb.w r6,[r11,#0x70]
  000cf44c: ldr r6,[r0,#0x48]
  000cf44e: mov r0,r11
  000cf450: blx r6
  000cf452: mov r0,r11
  000cf454: movw r1,#0x5279
  000cf458: blx 0x0007432c
  000cf45c: movs r0,#0x1
  000cf45e: blx 0x0006eb24
  000cf462: mov r4,r0
  000cf464: blx 0x00072124
  000cf468: mov r0,r4
  000cf46a: movs r1,#0xa
  000cf46c: mov.w r2,#0xffffffff
  000cf470: blx 0x000740e0
  000cf474: mov r5,r0
  000cf476: mov r0,r4
  000cf478: blx 0x0007213c
  000cf47c: blx 0x0006eb48
  000cf480: mov r0,r11
  000cf482: mov r1,r5
  000cf484: blx 0x000740a4
  000cf488: movw r1,#0x7d2
  000cf48c: b.w 0x000cfdaa
  000cf490: ldr r0,[0x000cf4c0]
  000cf492: add r0,pc
  000cf494: ldr r5,[r0,#0x0]
  000cf496: ldr r0,[r5,#0x0]
  000cf498: blx 0x00071c5c
  000cf49c: cmp r0,#0x14
  000cf49e: ble.w 0x000cf8e4
  000cf4a2: mov.w r4,#0x190
  000cf4a6: b 0x000cf8f2
  000cf4c4: mov.w r0,#0x114
  000cf4c8: blx 0x0006eb24
  000cf4cc: mov r4,r0
  000cf4ce: ldr r2,[0x000cf8c0]
  000cf4d0: movs r5,#0x0
  000cf4d2: mov.w r1,#0x3e8
  000cf4d6: movs r3,#0x0
  000cf4d8: strd r5,r5,[sp,#0x0]
  000cf4dc: blx 0x00073d80
  000cf4e0: mov.w r0,#0x1bc
  000cf4e4: blx 0x0006eb24
  000cf4e8: vmov s0,r11
  000cf4ec: mov r11,r0
  000cf4ee: vcvt.f32.s32 s16,s0
  000cf4f2: vmov s0,r10
  000cf4f6: vcvt.f32.s32 s18,s0
  000cf4fa: vmov s0,r9
  000cf4fe: vcvt.f32.s32 s20,s0
  000cf502: movw r1,#0x4260
  000cf506: movs r2,#0x0
  000cf508: mov r3,r4
  000cf50a: vstr.32 s16,[sp,#0x4]
  000cf50e: vstr.32 s18,[sp,#0x8]
  000cf512: vstr.32 s20,[sp,#0xc]
  000cf516: str r5,[sp,#0x0]
  000cf518: blx 0x0007414c
  000cf51c: movs r0,#0xc0
  000cf51e: blx 0x0006eb24
  000cf522: mov r4,r0
  000cf524: ldr r0,[0x000cf8c4]
  000cf526: add r0,pc
  000cf528: ldr r0,[r0,#0x0]
  000cf52a: ldr r2,[r0,#0x0]
  000cf52c: mov r0,r4
  000cf52e: movw r1,#0x4260
  000cf532: movs r3,#0x0
  000cf534: blx 0x0007207c
  000cf538: ldr.w r8,[sp,#0x38]
  000cf53c: ldr.w r0,[r11,#0x0]
  000cf540: mov r1,r4
  000cf542: mov.w r2,#0xffffffff
  000cf546: movs r3,#0x0
  000cf548: ldr r6,[r0,#0x8]
  000cf54a: mov r0,r11
  000cf54c: blx r6
  000cf54e: vmov r1,s16
  000cf552: ldr.w r0,[r11,#0x0]
  000cf556: vmov r2,s18
  000cf55a: movs r6,#0x1
  000cf55c: vmov r3,s20
  000cf560: strb.w r6,[r11,#0x70]
  000cf564: ldr r6,[r0,#0x48]
  000cf566: mov r0,r11
  000cf568: blx r6
  000cf56a: mov r0,r11
  000cf56c: movw r1,#0x477c
  000cf570: blx 0x0007432c
  000cf574: movs r0,#0x1
  000cf576: blx 0x0006eb24
  000cf57a: mov r4,r0
  000cf57c: blx 0x00072124
  000cf580: mov r0,r4
  000cf582: movs r1,#0x2
  000cf584: mov.w r2,#0xffffffff
  000cf588: blx 0x000740e0
  000cf58c: mov r5,r0
  000cf58e: mov r0,r4
  000cf590: blx 0x0007213c
  000cf594: blx 0x0006eb48
  000cf598: mov r0,r11
  000cf59a: mov r1,r5
  000cf59c: blx 0x000740a4
  000cf5a0: b.w 0x000ce8b8
  000cf5a4: mov.w r0,#0x114
  000cf5a8: blx 0x0006eb24
  000cf5ac: mov r4,r0
  000cf5ae: ldr r2,[0x000cf8c0]
  000cf5b0: movs r5,#0x0
  000cf5b2: movs r1,#0x0
  000cf5b4: movs r3,#0x0
  000cf5b6: strd r5,r5,[sp,#0x0]
  000cf5ba: blx 0x00073d80
  000cf5be: mov.w r0,#0x1bc
  000cf5c2: blx 0x0006eb24
  000cf5c6: vmov s0,r11
  000cf5ca: mov r11,r0
  000cf5cc: vcvt.f32.s32 s16,s0
  000cf5d0: vmov s0,r10
  000cf5d4: vcvt.f32.s32 s18,s0
  000cf5d8: vmov s0,r9
  000cf5dc: vcvt.f32.s32 s20,s0
  000cf5e0: movw r1,#0x4962
  000cf5e4: movs r2,#0x2
  000cf5e6: mov r3,r4
  000cf5e8: vstr.32 s16,[sp,#0x4]
  000cf5ec: vstr.32 s18,[sp,#0x8]
  000cf5f0: vstr.32 s20,[sp,#0xc]
  000cf5f4: str r5,[sp,#0x0]
  000cf5f6: blx 0x0007414c
  000cf5fa: movs r0,#0xc0
  000cf5fc: blx 0x0006eb24
  000cf600: mov r4,r0
  000cf602: ldr r0,[0x000cf8c8]
  000cf604: add r0,pc
  000cf606: ldr r0,[r0,#0x0]
  000cf608: ldr r2,[r0,#0x0]
  000cf60a: mov r0,r4
  000cf60c: movw r1,#0x4962
  000cf610: movs r3,#0x0
  000cf612: movs r5,#0x0
  000cf614: blx 0x0007207c
  000cf618: ldr.w r8,[sp,#0x38]
  000cf61c: ldr.w r0,[r11,#0x0]
  000cf620: mov r1,r4
  000cf622: mov.w r2,#0xffffffff
  000cf626: movs r3,#0x0
  000cf628: ldr r6,[r0,#0x8]
  000cf62a: mov r0,r11
  000cf62c: blx r6
  000cf62e: vmov r1,s16
  000cf632: ldr.w r0,[r11,#0x0]
  000cf636: vmov r2,s18
  000cf63a: strb.w r5,[r11,#0x70]
  000cf63e: vmov r3,s20
  000cf642: ldr r6,[r0,#0x48]
  000cf644: mov r0,r11
  000cf646: blx r6
  000cf648: mov r0,r11
  000cf64a: movw r1,#0x477c
  000cf64e: blx 0x0007432c
  000cf652: b.w 0x000ce8b8
  000cf656: ldr r0,[0x000cf8cc]
  000cf658: add r0,pc
  000cf65a: ldr r5,[r0,#0x0]
  000cf65c: ldr r0,[r5,#0x0]
  000cf65e: blx 0x00071c5c
  000cf662: cmp r0,#0x14
  000cf664: ble.w 0x000cf8fe
  000cf668: mov.w r4,#0x190
  000cf66c: b 0x000cf90c
  000cf66e: ldr r0,[0x000cf8d0]
  000cf670: add r0,pc
  000cf672: ldr r5,[r0,#0x0]
  000cf674: ldr r0,[r5,#0x0]
  000cf676: blx 0x00071c5c
  000cf67a: cmp r0,#0x14
  000cf67c: ble.w 0x000cfdba
  000cf680: mov.w r4,#0x190
  000cf684: b 0x000cfdc8
  000cf686: mov.w r0,#0x114
  000cf68a: blx 0x0006eb24
  000cf68e: mov r4,r0
  000cf690: ldr r2,[0x000cf8c0]
  000cf692: movs r5,#0x0
  000cf694: mov.w r1,#0x3e8
  000cf698: movs r3,#0x0
  000cf69a: strd r5,r5,[sp,#0x0]
  000cf69e: blx 0x00073d80
  000cf6a2: mov.w r0,#0x1bc
  000cf6a6: blx 0x0006eb24
  000cf6aa: vmov s0,r11
  000cf6ae: mov r11,r0
  000cf6b0: vcvt.f32.s32 s16,s0
  000cf6b4: vmov s0,r10
  000cf6b8: vcvt.f32.s32 s18,s0
  000cf6bc: vmov s0,r9
  000cf6c0: vcvt.f32.s32 s20,s0
  000cf6c4: movw r1,#0x4961
  000cf6c8: movs r2,#0x2
  000cf6ca: mov r3,r4
  000cf6cc: vstr.32 s16,[sp,#0x4]
  000cf6d0: vstr.32 s18,[sp,#0x8]
  000cf6d4: vstr.32 s20,[sp,#0xc]
  000cf6d8: str r5,[sp,#0x0]
  000cf6da: blx 0x0007414c
  000cf6de: movs r0,#0xc0
  000cf6e0: blx 0x0006eb24
  000cf6e4: mov r4,r0
  000cf6e6: ldr r0,[0x000cf8d4]
  000cf6e8: add r0,pc
  000cf6ea: ldr r6,[r0,#0x0]
  000cf6ec: ldr r2,[r6,#0x0]
  000cf6ee: mov r0,r4
  000cf6f0: movw r1,#0x4961
  000cf6f4: movs r3,#0x0
  000cf6f6: blx 0x0007207c
  000cf6fa: ldr.w r8,[sp,#0x38]
  000cf6fe: movs r0,#0xc0
  000cf700: blx 0x0006eb24
  000cf704: mov r5,r0
  000cf706: ldr r2,[r6,#0x0]
  000cf708: movw r1,#0x498f
  000cf70c: movs r3,#0x0
  000cf70e: blx 0x0007207c
  000cf712: ldr r1,[r5,#0xc]
  000cf714: mov r0,r4
  000cf716: blx 0x000720e8
  000cf71a: mov r0,r5
  000cf71c: blx 0x00071fc8
  000cf720: blx 0x0006eb48
  000cf724: ldr.w r0,[r11,#0x0]
  000cf728: mov r1,r4
  000cf72a: mov.w r2,#0xffffffff
  000cf72e: movs r3,#0x0
  000cf730: ldr r6,[r0,#0x8]
  000cf732: mov r0,r11
  000cf734: blx r6
  000cf736: vmov r1,s16
  000cf73a: ldr.w r0,[r11,#0x0]
  000cf73e: vmov r2,s18
  000cf742: movs r6,#0x1
  000cf744: vmov r3,s20
  000cf748: strb.w r6,[r11,#0x70]
  000cf74c: ldr r6,[r0,#0x48]
  000cf74e: mov r0,r11
  000cf750: blx r6
  000cf752: mov r0,r11
  000cf754: movw r1,#0x477c
  000cf758: blx 0x0007432c
  000cf75c: movs r0,#0x2
  000cf75e: str.w r0,[r11,#0x28]
  000cf762: movs r0,#0x1
  000cf764: blx 0x0006eb24
  000cf768: mov r4,r0
  000cf76a: blx 0x00072124
  000cf76e: mov r0,r4
  000cf770: movs r1,#0x2
  000cf772: mov.w r2,#0xffffffff
  000cf776: blx 0x000740e0
  000cf77a: mov r5,r0
  000cf77c: mov r0,r4
  000cf77e: blx 0x0007213c
  000cf782: blx 0x0006eb48
  000cf786: mov r0,r11
  000cf788: mov r1,r5
  000cf78a: blx 0x000740a4
  000cf78e: movw r1,#0x7d3
  000cf792: b 0x000cfdaa
  000cf794: mov.w r0,#0x114
  000cf798: blx 0x0006eb24
  000cf79c: mov r4,r0
  000cf79e: ldr r2,[0x000cf8c0]
  000cf7a0: movs r5,#0x0
  000cf7a2: movs r1,#0x0
  000cf7a4: movs r3,#0x0
  000cf7a6: strd r5,r5,[sp,#0x0]
  000cf7aa: blx 0x00073d80
  000cf7ae: mov.w r0,#0x1bc
  000cf7b2: blx 0x0006eb24
  000cf7b6: vmov s0,r11
  000cf7ba: mov r11,r0
  000cf7bc: vcvt.f32.s32 s16,s0
  000cf7c0: vmov s0,r10
  000cf7c4: vcvt.f32.s32 s18,s0
  000cf7c8: vmov s0,r9
  000cf7cc: vcvt.f32.s32 s20,s0
  000cf7d0: movw r1,#0x4a6b
  000cf7d4: movs r2,#0x1
  000cf7d6: mov r3,r4
  000cf7d8: vstr.32 s16,[sp,#0x4]
  000cf7dc: vstr.32 s18,[sp,#0x8]
  000cf7e0: vstr.32 s20,[sp,#0xc]
  000cf7e4: str r5,[sp,#0x0]
  000cf7e6: blx 0x0007414c
  000cf7ea: movs r0,#0xc0
  000cf7ec: blx 0x0006eb24
  000cf7f0: mov r4,r0
  000cf7f2: ldr r0,[0x000cf8d8]
  000cf7f4: add r0,pc
  000cf7f6: ldr r6,[r0,#0x0]
  000cf7f8: ldr r2,[r6,#0x0]
  000cf7fa: mov r0,r4
  000cf7fc: movw r1,#0x4a6b
  000cf800: movs r3,#0x0
  000cf802: blx 0x0007207c
  000cf806: ldr.w r8,[sp,#0x38]
  000cf80a: movs r0,#0xc0
  000cf80c: blx 0x0006eb24
  000cf810: mov r5,r0
  000cf812: ldr r2,[r6,#0x0]
  000cf814: movw r1,#0x4a6c
  000cf818: movs r3,#0x0
  000cf81a: blx 0x0007207c
  000cf81e: ldr r1,[r5,#0xc]
  000cf820: mov r0,r4
  000cf822: blx 0x000720e8
  000cf826: mov r0,r5
  000cf828: blx 0x00071fc8
  000cf82c: blx 0x0006eb48
  000cf830: movs r0,#0xc0
  000cf832: blx 0x0006eb24
  000cf836: mov r5,r0
  000cf838: ldr r2,[r6,#0x0]
  000cf83a: movw r1,#0x4a6d
  000cf83e: movs r3,#0x0
  000cf840: blx 0x0007207c
  000cf844: ldr r1,[r5,#0xc]
  000cf846: mov r0,r4
  000cf848: blx 0x000720e8
  000cf84c: mov r0,r5
  000cf84e: blx 0x00071fc8
  000cf852: blx 0x0006eb48
  000cf856: ldr.w r0,[r11,#0x0]
  000cf85a: mov r1,r4
  000cf85c: mov.w r2,#0xffffffff
  000cf860: movs r3,#0x0
  000cf862: ldr r6,[r0,#0x8]
  000cf864: mov r0,r11
  000cf866: blx r6
  000cf868: vmov r1,s16
  000cf86c: ldr.w r0,[r11,#0x0]
  000cf870: vmov r2,s18
  000cf874: movs r6,#0x1
  000cf876: vmov r3,s20
  000cf87a: strb.w r6,[r11,#0x70]
  000cf87e: ldr r6,[r0,#0x48]
  000cf880: mov r0,r11
  000cf882: blx r6
  000cf884: mov r0,r11
  000cf886: movw r1,#0x477c
  000cf88a: blx 0x0007432c
  000cf88e: movs r0,#0x1
  000cf890: blx 0x0006eb24
  000cf894: mov r4,r0
  000cf896: blx 0x00072124
  000cf89a: mov r0,r4
  000cf89c: movs r1,#0x8
  000cf89e: mov.w r2,#0xffffffff
  000cf8a2: blx 0x000740e0
  000cf8a6: mov r5,r0
  000cf8a8: mov r0,r4
  000cf8aa: blx 0x0007213c
  000cf8ae: blx 0x0006eb48
  000cf8b2: mov r0,r11
  000cf8b4: mov r1,r5
  000cf8b6: blx 0x000740a4
  000cf8ba: movw r1,#0x7d6
  000cf8be: b 0x000cfdaa
  000cf8dc: mov.w r11,#0x0
  000cf8e0: b.w 0x000ce7ea
  000cf8e4: ldr r0,[r5,#0x0]
  000cf8e6: blx 0x00071c5c
  000cf8ea: rsb r0,r0,r0, lsl #0x4
  000cf8ee: add.w r4,r0,#0x64
  000cf8f2: ldr r0,[r5,#0x0]
  000cf8f4: blx 0x00073c48
  000cf8f8: cbz r0,0x000cf91c
  000cf8fa: movs r0,#0xb4
  000cf8fc: b 0x000cf924
  000cf8fe: ldr r0,[r5,#0x0]
  000cf900: blx 0x00071c5c
  000cf904: rsb r0,r0,r0, lsl #0x4
  000cf908: add.w r4,r0,#0x64
  000cf90c: ldr r0,[r5,#0x0]
  000cf90e: blx 0x00073c48
  000cf912: cmp r0,#0x0
  000cf914: beq.w 0x000cfb9c
  000cf918: movs r0,#0xb4
  000cf91a: b 0x000cfba4
  000cf91c: ldr r0,[r5,#0x0]
  000cf91e: blx 0x00071770
  000cf922: lsls r0,r0,#0x2
  000cf924: ldr r1,[0x000cfb60]
  000cf926: vmov.f32 s0,0xbf000000
  000cf92a: add r0,r4
  000cf92c: add r1,pc
  000cf92e: add.w r0,r0,r0, lsl #0x2
  000cf932: ldr r1,[r1,#0x0]
  000cf934: vmov s4,r0
  000cf938: mov.w r0,#0x114
  000cf93c: vldr.32 s2,[r1,#0x2c]
  000cf940: vcvt.f32.s32 s16,s4
  000cf944: vadd.f32 s0,s2,s0
  000cf948: vmla.f32 s16,s0,s16
  000cf94c: blx 0x0006eb24
  000cf950: mov r4,r0
  000cf952: movs r5,#0x0
  000cf954: mov.w r1,#0x3e8
  000cf958: str r5,[sp,#0x4]
  000cf95a: vcvt.s32.f32 s0,s16
  000cf95e: movs r3,#0x0
  000cf960: str r5,[sp,#0x0]
  000cf962: vmov r2,s0
  000cf966: blx 0x00073d80
  000cf96a: mov.w r0,#0x1bc
  000cf96e: blx 0x0006eb24
  000cf972: vmov s0,r11
  000cf976: mov r8,r0
  000cf978: vcvt.f32.s32 s16,s0
  000cf97c: vmov s0,r10
  000cf980: vcvt.f32.s32 s18,s0
  000cf984: vmov s0,r9
  000cf988: vcvt.f32.s32 s20,s0
  000cf98c: movw r1,#0x494e
  000cf990: movs r2,#0x3
  000cf992: mov r3,r4
  000cf994: vstr.32 s16,[sp,#0x4]
  000cf998: vstr.32 s18,[sp,#0x8]
  000cf99c: vstr.32 s20,[sp,#0xc]
  000cf9a0: str r5,[sp,#0x0]
  000cf9a2: blx 0x0007414c
  000cf9a6: movs r0,#0xc0
  000cf9a8: blx 0x0006eb24
  000cf9ac: mov r9,r0
  000cf9ae: ldr r0,[0x000cfb64]
  000cf9b0: add r0,pc
  000cf9b2: ldr r5,[r0,#0x0]
  000cf9b4: ldr r2,[r5,#0x0]
  000cf9b6: mov r0,r9
  000cf9b8: mov.w r1,#0x4a80
  000cf9bc: movs r3,#0x0
  000cf9be: blx 0x0007207c
  000cf9c2: movs r0,#0xc0
  000cf9c4: blx 0x0006eb24
  000cf9c8: mov r6,r0
  000cf9ca: ldr r2,[r5,#0x0]
  000cf9cc: movw r1,#0x494e
  000cf9d0: movs r3,#0x0
  000cf9d2: blx 0x0007207c
  000cf9d6: movs r0,#0xc0
  000cf9d8: blx 0x0006eb24
  000cf9dc: mov r4,r0
  000cf9de: ldr r2,[r5,#0x0]
  000cf9e0: movw r1,#0x4a81
  000cf9e4: movs r3,#0x0
  000cf9e6: blx 0x0007207c
  000cf9ea: movs r0,#0xc0
  000cf9ec: str.w r8,[sp,#0x2c]
  000cf9f0: blx 0x0006eb24
  000cf9f4: mov r10,r0
  000cf9f6: ldr r2,[r5,#0x0]
  000cf9f8: movw r1,#0x4a82
  000cf9fc: movs r3,#0x0
  000cf9fe: blx 0x0007207c
  000cfa02: movs r0,#0xc0
  000cfa04: mov r8,r4
  000cfa06: blx 0x0006eb24
  000cfa0a: mov r11,r0
  000cfa0c: ldr r2,[r5,#0x0]
  000cfa0e: movw r1,#0x4a83
  000cfa12: movs r3,#0x0
  000cfa14: blx 0x0007207c
  000cfa18: movs r0,#0xc0
  000cfa1a: blx 0x0006eb24
  000cfa1e: mov r4,r0
  000cfa20: ldr r2,[r5,#0x0]
  000cfa22: movw r1,#0x4a84
  000cfa26: movs r3,#0x0
  000cfa28: blx 0x0007207c
  000cfa2c: ldr r0,[0x000cfb68]
  000cfa2e: ldr r1,[r6,#0xc]
  000cfa30: add r0,pc
  000cfa32: ldr r5,[r0,#0x0]
  000cfa34: mov r0,r9
  000cfa36: blx r5
  000cfa38: ldr.w r1,[r8,#0xc]
  000cfa3c: mov r0,r6
  000cfa3e: blx r5
  000cfa40: ldr.w r1,[r10,#0xc]
  000cfa44: mov r0,r6
  000cfa46: blx r5
  000cfa48: ldr.w r1,[r11,#0xc]
  000cfa4c: mov r0,r6
  000cfa4e: blx r5
  000cfa50: ldr r1,[r4,#0xc]
  000cfa52: mov r0,r6
  000cfa54: blx r5
  000cfa56: ldr r0,[0x000cfb6c]
  000cfa58: add r0,pc
  000cfa5a: ldr r5,[r0,#0x0]
  000cfa5c: mov r0,r6
  000cfa5e: blx r5
  000cfa60: ldr r0,[0x000cfb70]
  000cfa62: str.w r9,[sp,#0x30]
  000cfa66: add r0,pc
  000cfa68: ldr.w r9,[r0,#0x0]
  000cfa6c: mov r0,r6
  000cfa6e: blx r9
  000cfa70: mov r0,r8
  000cfa72: blx r5
  000cfa74: blx r9
  000cfa76: mov r0,r10
  000cfa78: blx r5
  000cfa7a: blx r9
  000cfa7c: mov r0,r11
  000cfa7e: blx r5
  000cfa80: blx r9
  000cfa82: mov r0,r4
  000cfa84: blx r5
  000cfa86: blx r9
  000cfa88: ldr r4,[sp,#0x2c]
  000cfa8a: mov.w r2,#0xffffffff
  000cfa8e: ldr r1,[sp,#0x30]
  000cfa90: movs r3,#0x0
  000cfa92: mov.w r11,#0x0
  000cfa96: ldr r0,[r4,#0x0]
  000cfa98: ldr r6,[r0,#0x8]
  000cfa9a: mov r0,r4
  000cfa9c: blx r6
  000cfa9e: movs r0,#0x1
  000cfaa0: strb.w r0,[r4,#0x40]
  000cfaa4: strb.w r0,[r4,#0x70]
  000cfaa8: movs r0,#0xc
  000cfaaa: blx 0x0006eb24
  000cfaae: mov r4,r0
  000cfab0: blx 0x000730c0
  000cfab4: movs r0,#0x2
  000cfab6: mov r1,r4
  000cfab8: blx 0x000730cc
  000cfabc: movs r0,#0x2c
  000cfabe: blx 0x0006eb24
  000cfac2: vmov r6,s16
  000cfac6: mov r5,r0
  000cfac8: vmov r10,s18
  000cfacc: ldr r0,[0x000cfb74]
  000cface: vmov r9,s20
  000cfad2: ldr r1,[0x000cfb78]
  000cfad4: ldr r2,[0x000cfb7c]
  000cfad6: str r0,[sp,#0x10]
  000cfad8: mov r3,r9
  000cfada: ldr r0,[0x000cfb80]
  000cfadc: str r2,[sp,#0x14]
  000cfade: ldr r2,[0x000cfb84]
  000cfae0: strd r2,r1,[sp,#0x8]
  000cfae4: mov r1,r6
  000cfae6: strd r11,r0,[sp,#0x0]
  000cfaea: mov r0,r5
  000cfaec: mov r2,r10
  000cfaee: blx 0x00074350
  000cfaf2: ldr r0,[r4,#0x4]
  000cfaf4: str r5,[r0,#0x0]
  000cfaf6: movs r0,#0x2c
  000cfaf8: blx 0x0006eb24
  000cfafc: mov r5,r0
  000cfafe: ldr r0,[0x000cfb88]
  000cfb00: ldr r1,[0x000cfb8c]
  000cfb02: ldr r2,[0x000cfb90]
  000cfb04: str r2,[sp,#0x14]
  000cfb06: mov r3,r9
  000cfb08: ldr r2,[0x000cfb94]
  000cfb0a: str r0,[sp,#0x10]
  000cfb0c: ldr r0,[0x000cfb98]
  000cfb0e: str r1,[sp,#0xc]
  000cfb10: mov r1,r6
  000cfb12: strd r0,r2,[sp,#0x4]
  000cfb16: movs r0,#0x0
  000cfb18: str r0,[sp,#0x0]
  000cfb1a: mov r0,r5
  000cfb1c: mov r2,r10
  000cfb1e: blx 0x00074350
  000cfb22: ldr.w r11,[sp,#0x2c]
  000cfb26: ldr r0,[r4,#0x4]
  000cfb28: mov r1,r4
  000cfb2a: str r5,[r0,#0x4]
  000cfb2c: mov r0,r11
  000cfb2e: blx 0x00074344
  000cfb32: ldr.w r0,[r11,#0x0]
  000cfb36: mov r1,r6
  000cfb38: mov r2,r10
  000cfb3a: mov r3,r9
  000cfb3c: ldr r5,[r0,#0x48]
  000cfb3e: mov r0,r11
  000cfb40: blx r5
  000cfb42: mov r0,r11
  000cfb44: movw r1,#0x477c
  000cfb48: blx 0x0007432c
  000cfb4c: movs r0,#0x1
  000cfb4e: blx 0x0006eb24
  000cfb52: mov r4,r0
  000cfb54: blx 0x00072124
  000cfb58: mov r0,r4
  000cfb5a: movs r1,#0x3
  000cfb5c: b 0x000d00a4
  000cfb9c: ldr r0,[r5,#0x0]
  000cfb9e: blx 0x00071770
  000cfba2: lsls r0,r0,#0x2
  000cfba4: ldr r1,[0x000cff50]
  000cfba6: vmov.f32 s0,0xbf000000
  000cfbaa: add r0,r4
  000cfbac: add r1,pc
  000cfbae: add.w r0,r0,r0, lsl #0x2
  000cfbb2: ldr r1,[r1,#0x0]
  000cfbb4: vmov s4,r0
  000cfbb8: mov.w r0,#0x114
  000cfbbc: vldr.32 s2,[r1,#0x2c]
  000cfbc0: vcvt.f32.s32 s16,s4
  000cfbc4: vadd.f32 s0,s2,s0
  000cfbc8: vmla.f32 s16,s0,s16
  000cfbcc: blx 0x0006eb24
  000cfbd0: mov r4,r0
  000cfbd2: movs r5,#0x0
  000cfbd4: movs r1,#0x0
  000cfbd6: str r5,[sp,#0x4]
  000cfbd8: vcvt.s32.f32 s0,s16
  000cfbdc: movs r3,#0x0
  000cfbde: str r5,[sp,#0x0]
  000cfbe0: vmov r2,s0
  000cfbe4: blx 0x00073d80
  000cfbe8: mov.w r0,#0x1bc
  000cfbec: blx 0x0006eb24
  000cfbf0: vmov s0,r11
  000cfbf4: mov r8,r0
  000cfbf6: vcvt.f32.s32 s16,s0
  000cfbfa: vmov s0,r10
  000cfbfe: vcvt.f32.s32 s18,s0
  000cfc02: vmov s0,r9
  000cfc06: vcvt.f32.s32 s20,s0
  000cfc0a: movw r1,#0x4974
  000cfc0e: movs r2,#0x0
  000cfc10: mov r3,r4
  000cfc12: vstr.32 s16,[sp,#0x4]
  000cfc16: vstr.32 s18,[sp,#0x8]
  000cfc1a: vstr.32 s20,[sp,#0xc]
  000cfc1e: str r5,[sp,#0x0]
  000cfc20: blx 0x0007414c
  000cfc24: movs r0,#0xc0
  000cfc26: blx 0x0006eb24
  000cfc2a: mov r10,r0
  000cfc2c: ldr r0,[0x000cff54]
  000cfc2e: add r0,pc
  000cfc30: ldr.w r9,[r0,#0x0]
  000cfc34: ldr.w r2,[r9,#0x0]
  000cfc38: mov r0,r10
  000cfc3a: movw r1,#0x4974
  000cfc3e: movs r3,#0x0
  000cfc40: blx 0x0007207c
  000cfc44: movs r0,#0xc0
  000cfc46: blx 0x0006eb24
  000cfc4a: mov r11,r0
  000cfc4c: ldr.w r2,[r9,#0x0]
  000cfc50: movw r1,#0x4975
  000cfc54: movs r3,#0x0
  000cfc56: blx 0x0007207c
  000cfc5a: movs r0,#0xc0
  000cfc5c: blx 0x0006eb24
  000cfc60: mov r5,r0
  000cfc62: ldr.w r2,[r9,#0x0]
  000cfc66: movw r1,#0x4977
  000cfc6a: movs r3,#0x0
  000cfc6c: blx 0x0007207c
  000cfc70: movs r0,#0xc0
  000cfc72: blx 0x0006eb24
  000cfc76: mov r6,r0
  000cfc78: ldr.w r2,[r9,#0x0]
  000cfc7c: movw r1,#0x4978
  000cfc80: movs r3,#0x0
  000cfc82: blx 0x0007207c
  000cfc86: movs r0,#0xc0
  000cfc88: str.w r8,[sp,#0x2c]
  000cfc8c: blx 0x0006eb24
  000cfc90: mov r8,r0
  000cfc92: ldr.w r2,[r9,#0x0]
  000cfc96: movw r1,#0x4979
  000cfc9a: movs r3,#0x0
  000cfc9c: blx 0x0007207c
  000cfca0: movs r0,#0xc0
  000cfca2: str r6,[sp,#0x28]
  000cfca4: blx 0x0006eb24
  000cfca8: ldr.w r2,[r9,#0x0]
  000cfcac: movw r1,#0x497a
  000cfcb0: movs r3,#0x0
  000cfcb2: str r0,[sp,#0x30]
  000cfcb4: blx 0x0007207c
  000cfcb8: movs r0,#0xc0
  000cfcba: blx 0x0006eb24
  000cfcbe: ldr.w r2,[r9,#0x0]
  000cfcc2: movw r1,#0x4976
  000cfcc6: movs r3,#0x0
  000cfcc8: mov r6,r0
  000cfcca: blx 0x0007207c
  000cfcce: ldr r0,[0x000cff58]
  000cfcd0: mov r9,r11
  000cfcd2: ldr.w r1,[r11,#0xc]
  000cfcd6: add r0,pc
  000cfcd8: ldr r4,[r0,#0x0]
  000cfcda: mov r0,r10
  000cfcdc: blx r4
  000cfcde: ldr r1,[r5,#0xc]
  000cfce0: mov r0,r10
  000cfce2: blx r4
  000cfce4: ldr.w r11,[sp,#0x28]
  000cfce8: mov r0,r10
  000cfcea: ldr.w r1,[r11,#0xc]
  000cfcee: blx r4
  000cfcf0: ldr.w r1,[r8,#0xc]
  000cfcf4: mov r0,r10
  000cfcf6: blx r4
  000cfcf8: ldr r0,[sp,#0x30]
  000cfcfa: ldr r1,[r0,#0xc]
  000cfcfc: mov r0,r10
  000cfcfe: blx r4
  000cfd00: ldr r1,[r6,#0xc]
  000cfd02: mov r0,r10
  000cfd04: str r6,[sp,#0x24]
  000cfd06: blx r4
  000cfd08: ldr r0,[0x000cff5c]
  000cfd0a: add r0,pc
  000cfd0c: ldr r4,[r0,#0x0]
  000cfd0e: mov r0,r9
  000cfd10: blx r4
  000cfd12: ldr r0,[0x000cff60]
  000cfd14: add r0,pc
  000cfd16: ldr r6,[r0,#0x0]
  000cfd18: mov r0,r9
  000cfd1a: blx r6
  000cfd1c: mov r0,r5
  000cfd1e: blx r4
  000cfd20: blx r6
  000cfd22: mov r0,r11
  000cfd24: blx r4
  000cfd26: blx r6
  000cfd28: mov r0,r8
  000cfd2a: blx r4
  000cfd2c: blx r6
  000cfd2e: ldr r0,[sp,#0x30]
  000cfd30: blx r4
  000cfd32: blx r6
  000cfd34: ldr r0,[sp,#0x24]
  000cfd36: blx r4
  000cfd38: blx r6
  000cfd3a: ldr.w r11,[sp,#0x2c]
  000cfd3e: mov r1,r10
  000cfd40: mov.w r2,#0xffffffff
  000cfd44: movs r3,#0x0
  000cfd46: ldr.w r0,[r11,#0x0]
  000cfd4a: ldr r6,[r0,#0x8]
  000cfd4c: mov r0,r11
  000cfd4e: blx r6
  000cfd50: movs r0,#0x1
  000cfd52: movw r1,#0x494e
  000cfd56: strb.w r0,[r11,#0x70]
  000cfd5a: mov r0,r11
  000cfd5c: blx 0x0007432c
  000cfd60: vmov r1,s16
  000cfd64: ldr.w r0,[r11,#0x0]
  000cfd68: vmov r2,s18
  000cfd6c: vmov r3,s20
  000cfd70: ldr r6,[r0,#0x48]
  000cfd72: mov r0,r11
  000cfd74: blx r6
  000cfd76: movs r0,#0x1
  000cfd78: blx 0x0006eb24
  000cfd7c: mov r4,r0
  000cfd7e: blx 0x00072124
  000cfd82: ldr.w r8,[sp,#0x38]
  000cfd86: mov r0,r4
  000cfd88: movs r1,#0x5
  000cfd8a: mov.w r2,#0xffffffff
  000cfd8e: blx 0x000740e0
  000cfd92: mov r5,r0
  000cfd94: mov r0,r4
  000cfd96: blx 0x0007213c
  000cfd9a: blx 0x0006eb48
  000cfd9e: mov r0,r11
  000cfda0: mov r1,r5
  000cfda2: blx 0x000740a4
  000cfda6: movw r1,#0x7d5
  000cfdaa: blx 0x00074338
  000cfdae: mov r1,r0
  000cfdb0: mov r0,r11
  000cfdb2: blx 0x00074344
  000cfdb6: b.w 0x000ce8b8
  000cfdba: ldr r0,[r5,#0x0]
  000cfdbc: blx 0x00071c5c
  000cfdc0: rsb r0,r0,r0, lsl #0x4
  000cfdc4: add.w r4,r0,#0x64
  000cfdc8: ldr r0,[r5,#0x0]
  000cfdca: blx 0x00073c48
  000cfdce: cbz r0,0x000cfdd4
  000cfdd0: movs r0,#0xb4
  000cfdd2: b 0x000cfddc
  000cfdd4: ldr r0,[r5,#0x0]
  000cfdd6: blx 0x00071770
  000cfdda: lsls r0,r0,#0x2
  000cfddc: ldr r1,[0x000cff64]
  000cfdde: vmov.f32 s0,0xbf000000
  000cfde2: add r0,r4
  000cfde4: add r1,pc
  000cfde6: add.w r0,r0,r0, lsl #0x2
  000cfdea: ldr r1,[r1,#0x0]
  000cfdec: vmov s4,r0
  000cfdf0: mov.w r0,#0x114
  000cfdf4: vldr.32 s2,[r1,#0x2c]
  000cfdf8: vcvt.f32.s32 s16,s4
  000cfdfc: vadd.f32 s0,s2,s0
  000cfe00: vmla.f32 s16,s0,s16
  000cfe04: blx 0x0006eb24
  000cfe08: mov r4,r0
  000cfe0a: movs r5,#0x0
  000cfe0c: mov.w r1,#0x3e8
  000cfe10: str r5,[sp,#0x4]
  000cfe12: vcvt.s32.f32 s0,s16
  000cfe16: movs r3,#0x0
  000cfe18: str r5,[sp,#0x0]
  000cfe1a: vmov r2,s0
  000cfe1e: blx 0x00073d80
  000cfe22: mov.w r0,#0x1bc
  000cfe26: blx 0x0006eb24
  000cfe2a: vmov s0,r11
  000cfe2e: mov r6,r0
  000cfe30: vcvt.f32.s32 s16,s0
  000cfe34: vmov s0,r10
  000cfe38: vcvt.f32.s32 s18,s0
  000cfe3c: vmov s0,r9
  000cfe40: vcvt.f32.s32 s20,s0
  000cfe44: movw r1,#0x4299
  000cfe48: movs r2,#0x3
  000cfe4a: mov r3,r4
  000cfe4c: vstr.32 s16,[sp,#0x4]
  000cfe50: vstr.32 s18,[sp,#0x8]
  000cfe54: vstr.32 s20,[sp,#0xc]
  000cfe58: str r5,[sp,#0x0]
  000cfe5a: blx 0x0007414c
  000cfe5e: movs r0,#0xc0
  000cfe60: str r6,[sp,#0x2c]
  000cfe62: blx 0x0006eb24
  000cfe66: mov r4,r0
  000cfe68: ldr r0,[0x000cff68]
  000cfe6a: add r0,pc
  000cfe6c: ldr r5,[r0,#0x0]
  000cfe6e: ldr r2,[r5,#0x0]
  000cfe70: mov r0,r4
  000cfe72: movw r1,#0x4299
  000cfe76: movs r3,#0x0
  000cfe78: blx 0x0007207c
  000cfe7c: ldr r0,[r5,#0x0]
  000cfe7e: add r1,sp,#0x78
  000cfe80: mov.w r9,#0xffffffff
  000cfe84: str.w r9,[sp,#0x78]
  000cfe88: blx 0x000720ac
  000cfe8c: ldr r0,[r5,#0x0]
  000cfe8e: movw r2,#0x429f
  000cfe92: ldr r1,[sp,#0x78]
  000cfe94: movs r3,#0x0
  000cfe96: mov.w r10,#0x0
  000cfe9a: blx 0x0007303c
  000cfe9e: ldr r1,[sp,#0x78]
  000cfea0: mov r0,r4
  000cfea2: mov r8,r4
  000cfea4: blx 0x000720e8
  000cfea8: vldr.32 s22,[pc,#0x310]
  000cfeac: add r6,sp,#0x3c
  000cfeae: mov.w r4,#0xffffffff
  000cfeb2: mov.w r11,#0xffffffff
  000cfeb6: b 0x000cff74
  000cfeb8: ldr r0,[r5,#0x0]
  000cfeba: add r1,sp,#0x80
  000cfebc: str.w r9,[sp,#0x80]
  000cfec0: str.w r9,[sp,#0x7c]
  000cfec4: blx 0x000720ac
  000cfec8: ldr r0,[r5,#0x0]
  000cfeca: add r1,sp,#0x7c
  000cfecc: blx 0x000720ac
  000cfed0: ldr r0,[r5,#0x0]
  000cfed2: movw r2,#0x429c
  000cfed6: ldr r1,[sp,#0x80]
  000cfed8: movs r3,#0x0
  000cfeda: blx 0x0007303c
  000cfede: ldr r0,[r5,#0x0]
  000cfee0: movw r2,#0x429d
  000cfee4: ldr r1,[sp,#0x7c]
  000cfee6: movs r3,#0x0
  000cfee8: blx 0x0007303c
  000cfeec: adr r0,[0xd01e8]
  000cfeee: cmp.w r10,#0x1
  000cfef2: it eq
  000cfef4: add.eq r0,#0x4
  000cfef6: cmp.w r10,#0x0
  000cfefa: vldr.32 s24,[r0]
  000cfefe: it eq
  000cff00: vmov.eq.f32 s24,s22
  000cff04: ldr r0,[r5,#0x0]
  000cff06: ldr r1,[sp,#0x80]
  000cff08: blx 0x000720c4
  000cff0c: mov r1,r0
  000cff0e: mov r0,r6
  000cff10: movs r2,#0x0
  000cff12: movs r3,#0x0
  000cff14: vstr.32 s24,[sp]
  000cff18: blx 0x0006f820
  000cff1c: ldr r0,[r5,#0x0]
  000cff1e: ldr r1,[sp,#0x7c]
  000cff20: blx 0x000720c4
  000cff24: mov r1,r0
  000cff26: mov r0,r6
  000cff28: movs r2,#0x0
  000cff2a: movs r3,#0x0
  000cff2c: vstr.32 s24,[sp]
  000cff30: blx 0x0006f820
  000cff34: adds r0,r4,#0x1
  000cff36: beq 0x000cff6c
  000cff38: ldr r2,[sp,#0x80]
  000cff3a: mov r1,r4
  000cff3c: ldr r0,[r5,#0x0]
  000cff3e: blx 0x000720d0
  000cff42: ldr r0,[r5,#0x0]
  000cff44: mov r1,r11
  000cff46: ldr r2,[sp,#0x7c]
  000cff48: blx 0x000720d0
  000cff4c: b 0x000cff70
  000cff6c: ldrd r11,r4,[sp,#0x7c]
  000cff70: add.w r10,r10,#0x1
  000cff74: cmp.w r10,#0x3
  000cff78: bne 0x000cfeb8
  000cff7a: mov r0,r8
  000cff7c: mov r1,r4
  000cff7e: mov r6,r8
  000cff80: blx 0x000720e8
  000cff84: mov.w r1,#0xffffffff
  000cff88: ldr r0,[r5,#0x0]
  000cff8a: str r1,[sp,#0x3c]
  000cff8c: add r1,sp,#0x3c
  000cff8e: blx 0x000720ac
  000cff92: ldr r0,[r5,#0x0]
  000cff94: movw r2,#0x429e
  000cff98: ldr r1,[sp,#0x3c]
  000cff9a: movs r3,#0x0
  000cff9c: mov.w r8,#0x0
  000cffa0: blx 0x0007303c
  000cffa4: ldr r1,[sp,#0x3c]
  000cffa6: mov r0,r6
  000cffa8: blx 0x000720e8
  000cffac: movw r0,#0x429a
  000cffb0: add r1,sp,#0x7c
  000cffb2: strh.w r0,[sp,#0x7c]
  000cffb6: movw r0,#0x88b8
  000cffba: add r2,sp,#0x80
  000cffbc: str r0,[sp,#0x80]
  000cffbe: mov r0,r6
  000cffc0: movs r3,#0x1
  000cffc2: movs r4,#0x1
  000cffc4: blx 0x00073d5c
  000cffc8: mov r0,r6
  000cffca: mov r1,r11
  000cffcc: blx 0x0007435c
  000cffd0: ldr.w r11,[sp,#0x2c]
  000cffd4: mov r1,r6
  000cffd6: mov.w r2,#0xffffffff
  000cffda: movs r3,#0x0
  000cffdc: ldr.w r0,[r11,#0x0]
  000cffe0: ldr r5,[r0,#0x8]
  000cffe2: mov r0,r11
  000cffe4: blx r5
  000cffe6: movs r0,#0xc
  000cffe8: strb.w r4,[r11,#0x40]
  000cffec: strb.w r4,[r11,#0x70]
  000cfff0: blx 0x0006eb24
  000cfff4: mov r4,r0
  000cfff6: blx 0x000730c0
  000cfffa: movs r0,#0x2
  000cfffc: mov r1,r4
  000cfffe: blx 0x000730cc
  000d0002: movs r0,#0x2c
  000d0004: blx 0x0006eb24
  000d0008: vmov r6,s16
  000d000c: mov r5,r0
  000d000e: vmov r10,s18
  000d0012: ldr r0,[0x000d01c0]
  000d0014: vmov r9,s20
  000d0018: ldr r1,[0x000d01c4]
  000d001a: ldr r2,[0x000d01c8]
  000d001c: str r0,[sp,#0x10]
  000d001e: mov r3,r9
  000d0020: ldr r0,[0x000d01d0]
  000d0022: str r2,[sp,#0x14]
  000d0024: ldr r2,[0x000d01cc]
  000d0026: strd r2,r1,[sp,#0x8]
  000d002a: mov r1,r6
  000d002c: strd r8,r0,[sp,#0x0]
  000d0030: mov r0,r5
  000d0032: mov r2,r10
  000d0034: blx 0x00074350
  000d0038: ldr r0,[r4,#0x4]
  000d003a: str r5,[r0,#0x0]
  000d003c: movs r0,#0x2c
  000d003e: blx 0x0006eb24
  000d0042: mov r5,r0
  000d0044: ldr r0,[0x000d01d4]
  000d0046: ldr r1,[0x000d01d8]
  000d0048: ldr r2,[0x000d01dc]
  000d004a: str r2,[sp,#0x14]
  000d004c: mov r3,r9
  000d004e: ldr r2,[0x000d01e0]
  000d0050: str r0,[sp,#0x10]
  000d0052: ldr r0,[0x000d01e4]
  000d0054: str r1,[sp,#0xc]
  000d0056: mov r1,r6
  000d0058: strd r0,r2,[sp,#0x4]
  000d005c: movs r0,#0x0
  000d005e: str r0,[sp,#0x0]
  000d0060: mov r0,r5
  000d0062: mov r2,r10
  000d0064: blx 0x00074350
  000d0068: ldr r0,[r4,#0x4]
  000d006a: mov r1,r4
  000d006c: str r5,[r0,#0x4]
  000d006e: mov r0,r11
  000d0070: blx 0x00074344
  000d0074: ldr.w r0,[r11,#0x0]
  000d0078: mov r1,r6
  000d007a: mov r2,r10
  000d007c: mov r3,r9
  000d007e: ldr r5,[r0,#0x48]
  000d0080: mov r0,r11
  000d0082: blx r5
  000d0084: mov r0,r11
  000d0086: movw r1,#0x477c
  000d008a: blx 0x0007432c
  000d008e: movs r0,#0x3
  000d0090: str.w r0,[r11,#0x28]
  000d0094: movs r0,#0x1
  000d0096: blx 0x0006eb24
  000d009a: mov r4,r0
  000d009c: blx 0x00072124
  000d00a0: mov r0,r4
  000d00a2: movs r1,#0x4
  000d00a4: mov.w r2,#0xffffffff
  000d00a8: blx 0x000740e0
  000d00ac: mov r5,r0
  000d00ae: mov r0,r4
  000d00b0: blx 0x0007213c
  000d00b4: blx 0x0006eb48
  000d00b8: mov r0,r11
  000d00ba: mov r1,r5
  000d00bc: blx 0x000740a4
  000d00c0: b.w 0x000ce7ea
```
