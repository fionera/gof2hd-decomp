# Level::createCampaignMission
elf 0xb4bb8 body 27540
Sig: undefined __thiscall createCampaignMission(Level * this)

## decompile
```c

/* Level::createCampaignMission() */

void __thiscall Level::createCampaignMission(Level *this)

{
  char cVar1;
  bool bVar2;
  ulonglong uVar3;
  undefined8 uVar4;
  int iVar5;
  Route *pRVar6;
  Array<KIPlayer*> *pAVar7;
  Waypoint *pWVar8;
  Waypoint *pWVar9;
  Waypoint *pWVar10;
  undefined4 uVar11;
  int iVar12;
  void *pvVar13;
  Objective *pOVar14;
  longlong *plVar15;
  Route *pRVar16;
  Mission *this_00;
  Agent *pAVar17;
  uint *puVar18;
  undefined4 *puVar19;
  Station *pSVar20;
  PlayerStation *pPVar21;
  AEGeometry *pAVar22;
  AEGeometry *pAVar23;
  AEGeometry *pAVar24;
  AEGeometry *this_01;
  String *pSVar25;
  char *pcVar26;
  uint uVar27;
  PlayerFixedObject *pPVar28;
  Route *pRVar29;
  PlayerFighter *pPVar30;
  __fn *__fn;
  byte *pbVar31;
  undefined4 uVar32;
  undefined4 uVar33;
  void *__child_stack;
  void *__child_stack_00;
  void *__child_stack_01;
  float *pfVar34;
  void *__child_stack_02;
  void *extraout_r1;
  void *extraout_r1_00;
  void *__child_stack_03;
  int __flags;
  int __flags_00;
  int __flags_01;
  int __flags_02;
  int __flags_03;
  int __flags_04;
  int __flags_05;
  int __flags_06;
  int __flags_07;
  int __flags_08;
  int __flags_09;
  int __flags_10;
  int __flags_11;
  int __flags_12;
  int __flags_13;
  int extraout_r2;
  int extraout_r2_00;
  int __flags_14;
  int __flags_15;
  int extraout_r2_01;
  int extraout_r2_02;
  int __flags_16;
  int __flags_17;
  int __flags_18;
  int __flags_19;
  int __flags_20;
  void *__arg;
  void *__arg_00;
  void *__arg_01;
  void *__arg_02;
  void *__arg_03;
  void *__arg_04;
  void *__arg_05;
  void *__arg_06;
  void *__arg_07;
  void *__arg_08;
  void *__arg_09;
  void *__arg_10;
  void *__arg_11;
  void *__arg_12;
  void *__arg_13;
  void *__arg_14;
  void *extraout_r3;
  void *extraout_r3_00;
  void *__arg_15;
  void *__arg_16;
  void *__arg_17;
  void *extraout_r3_01;
  void *extraout_r3_02;
  void *__arg_18;
  void *__arg_19;
  void *__arg_20;
  void *__arg_21;
  void *__arg_22;
  int *piVar35;
  uint uVar36;
  int *piVar37;
  Vector *pVVar38;
  KIPlayer *pKVar39;
  Player *pPVar40;
  int iVar41;
  code *pcVar42;
  uint uVar43;
  uint uVar44;
  int *piVar45;
  int iVar46;
  uint uVar47;
  uint uVar48;
  uint uVar49;
  int iVar50;
  PlayerTurret *this_02;
  undefined4 *puVar51;
  int iVar52;
  code *pcVar53;
  int *piVar54;
  int iVar55;
  uint in_fpscr;
  float fVar56;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float fVar57;
  float extraout_s0_05;
  float extraout_s0_06;
  float extraout_s0_07;
  float extraout_s0_08;
  float extraout_s0_09;
  float extraout_s0_10;
  float extraout_s0_11;
  float extraout_s0_12;
  float extraout_s0_13;
  float extraout_s0_14;
  float extraout_s0_15;
  float extraout_s0_16;
  float extraout_s0_17;
  float fVar58;
  float extraout_s0_18;
  float extraout_s0_19;
  float extraout_s0_20;
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
  undefined4 uVar59;
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
  float fVar60;
  float extraout_s2_12;
  float extraout_s2_13;
  float extraout_s2_14;
  float extraout_s2_15;
  float extraout_s2_16;
  float extraout_s2_17;
  float extraout_s2_18;
  float extraout_s2_19;
  float extraout_s2_20;
  undefined4 uVar61;
  float fVar62;
  float fVar63;
  float fVar64;
  undefined1 auVar65 [16];
  undefined8 uVar66;
  int local_1f0;
  int local_1ec;
  String aSStack_1dc [12];
  String aSStack_1d0 [12];
  String aSStack_1c4 [12];
  String aSStack_1b8 [12];
  undefined4 local_1ac;
  undefined4 local_1a8;
  undefined4 local_1a4;
  Vector aVStack_1a0 [12];
  undefined4 local_194;
  undefined4 local_190;
  float local_18c;
  float local_188;
  float local_184;
  undefined4 local_180;
  String aSStack_17c [12];
  String aSStack_170 [12];
  String aSStack_164 [12];
  float local_158;
  undefined4 local_154;
  undefined4 local_150;
  undefined4 local_14c;
  undefined4 uStack_148;
  undefined4 uStack_144;
  undefined4 local_140;
  undefined4 uStack_13c;
  undefined4 uStack_138;
  float local_134;
  float local_130;
  float local_12c;
  String aSStack_128 [12];
  float local_11c;
  float local_118;
  float local_114;
  undefined8 local_110;
  float local_108;
  undefined4 uStack_104;
  undefined8 local_100;
  float local_e8;
  float fStack_e4;
  float local_e0;
  int local_dc;
  longlong local_d8;
  int local_d0;
  undefined4 uStack_cc;
  int iStack_c8;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  int local_90;
  undefined4 uStack_8c;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined4 local_70;
  undefined8 local_68;
  float local_60;
  int local_5c;
  
  piVar35 = *(int **)(DAT_000c4e94 + 0xc4bce);
  local_5c = *piVar35;
  AEGeometry::getPosition();
  piVar45 = *(int **)(DAT_000c4e98 + 0xc4be4);
  iVar5 = Status::getCurrentCampaignMission();
  iVar46 = DAT_000c9d40;
  uVar59 = DAT_000c9340;
  uVar11 = DAT_000c8728;
  uVar4 = DAT_000c7ee0;
  uVar66 = DAT_000c7ed8;
  switch(iVar5) {
  case 0:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(3,(Array *)pAVar7);
    uVar11 = DAT_000c5310;
    for (uVar36 = 0; uVar59 = DAT_000c5314, uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1)
    {
      iVar46 = 2;
      if (uVar36 == 1) {
        iVar46 = 0x17;
      }
      uVar59 = createShip(this,8,0,iVar46,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar59;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4));
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
      piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      (**(code **)(*piVar37 + 0x48))(piVar37,uVar11,uVar11,uVar11);
      iVar46 = *(int *)(*(int *)(this + 0xf8) + 4);
      iVar5 = *(int *)(iVar46 + uVar36 * 4);
      *(undefined4 *)(iVar5 + 0x50) = 0;
      *(undefined1 *)(iVar5 + 0x4c) = 0;
      Player::setHitpoints(*(int *)(*(int *)(iVar46 + uVar36 * 4) + 4));
      if (uVar36 < 3) {
        PlayerFighter::setExhaustVisible
                  (SUB41(*(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4),0));
      }
    }
    Player::setHitpoints(**(int **)(this + 0xf0));
    *(undefined4 *)(*piVar45 + 100) = uVar59;
    *(undefined1 *)(**(int **)(this + 0xf0) + 0x5e) = 1;
    break;
  case 1:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(1,(Array *)pAVar7);
    uVar11 = createShip(this,3,0,0x1e,(Waypoint *)0x0,true,false);
    fVar58 = DAT_000c565c;
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000c5660,fVar58,DAT_000c5664)
    ;
    local_a8 = *(ulonglong *)(DAT_000c5668 + 0xc536c);
    local_a0 = *(longlong *)(DAT_000c5668 + 0xc5374);
    local_98 = *(longlong *)(DAT_000c5668 + 0xc537c);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    *(Route **)(this + 0x110) = pRVar6;
    KIPlayer::setRoute((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),pRVar6);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x1c))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),0);
    PlayerFighter::setExhaustVisible(SUB41(**(undefined4 **)(*(int *)(this + 0xf8) + 4),0));
    break;
  case 2:
  case 3:
  case 5:
  case 6:
  case 8:
  case 9:
  case 10:
  case 0xb:
  case 0xc:
  case 0xd:
  case 0xf:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x16:
  case 0x17:
  case 0x1b:
  case 0x1e:
  case 0x1f:
  case 0x20:
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x25:
  case 0x27:
  case 0x2a:
  case 0x2b:
  case 0x2c:
  case 0x2d:
  case 0x2e:
  case 0x2f:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x39:
  case 0x3a:
  case 0x3b:
  case 0x3c:
  case 0x3d:
  case 0x3e:
  case 0x42:
  case 0x44:
  case 0x47:
  case 0x48:
  case 0x4a:
  case 0x4b:
  case 0x4c:
  case 0x4d:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x58:
  case 0x5a:
  case 0x5d:
  case 0x5f:
  case 0x60:
  case 0x62:
  case 99:
  case 0x65:
  case 0x67:
  case 0x68:
    break;
  case 4:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(1,(Array *)pAVar7);
    uVar11 = createShip(this,8,0,2,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    KIPlayer::setInitActive(SUB41(**(undefined4 **)(*(int *)(this + 0xf8) + 4),0));
    Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),0,0,DAT_000c566c);
    KIPlayer::setToSleep((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4));
    break;
  case 7:
    local_a8 = *(ulonglong *)(DAT_000c5670 + 0xc5446);
    local_a0 = *(longlong *)(DAT_000c5670 + 0xc544e);
    local_98 = *(longlong *)(DAT_000c5670 + 0xc5456);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    *(Route **)(this + 0x108) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(4,(Array *)pAVar7);
    for (iVar46 = 0; iVar46 != 3; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x108),1);
      uVar11 = createShip(this,8,0,2,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
    }
    uVar11 = createShip(this,3,0,0x1e,(Waypoint *)0x0,true,false);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) = uVar11;
    Player::setAlwaysFriend
              (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) + 4),true);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc);
    (**(code **)(*piVar45 + 0x48))
              (piVar45,local_11c + DAT_000c5674,local_118 + DAT_000c565c,local_114 + DAT_000c5678);
    pKVar39 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + 0xc);
    pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x108),*(void **)(*(int *)(this + 0xf8) + 4),
                                   __flags,__arg);
    KIPlayer::setRoute(pKVar39,pRVar6);
    Player::setHitpoints(*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) + 4));
    pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c5680 + 0xc556a));
    AbyssEngine::String::operator=
              ((String *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) + 0x18),pSVar25);
    PlayerFighter::setBoostProb(*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc),0);
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x12,0,3,this);
    goto LAB_000cb9b8;
  case 0xe:
    local_a8 = *(ulonglong *)(DAT_000c5684 + 0xc55ac);
    local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000c5684 + 0xc55b4));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(7,(Array *)pAVar7);
    for (iVar46 = 0; iVar46 != 3; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,8,0,0,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
    }
    for (iVar46 = 3; iVar46 != 5; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,0,0,5,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
    }
    for (iVar46 = 5; iVar46 != 7; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,0,1,0xe,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
    }
    piVar45 = (int *)Route::getWaypoint();
    (**(code **)(*piVar45 + 0x28))((Vector *)&local_e8,piVar45);
    fVar58 = DAT_000c5688;
    local_e8 = local_e8 + DAT_000c5688;
    piVar45 = *(int **)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -8);
    (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_e8);
    piVar45 = (int *)Route::getWaypoint();
    (**(code **)(*piVar45 + 0x28))((Vector *)&local_110,piVar45);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e8,(Vector *)&local_110);
    local_e8 = local_e8 + DAT_000c568c;
    fStack_e4 = fStack_e4 + DAT_000c5690;
    local_e0 = local_e0 + fVar58;
    piVar45 = *(int **)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4);
    (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_e8);
    PlayerEgo::setPosition(extraout_s0,extraout_s1_00,extraout_s2);
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x1f,0,this);
    goto LAB_000cb9b8;
  case 0x10:
    local_a8 = *(ulonglong *)(DAT_000c5a34 + 0xc5784);
    local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000c5a34 + 0xc578c));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(10,(Array *)pAVar7);
    iVar46 = Route::getWaypoint();
    for (iVar5 = 0; iVar5 != 3; iVar5 = iVar5 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,0,1,0xf,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) = uVar11;
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) + 4),true);
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4),false);
      pPVar40 = *(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) + 4);
      uVar11 = Player::getMaxHitpoints();
      iVar50 = __aeabi_idiv(uVar11,3);
      Player::setMaxHitpoints(pPVar40,iVar50);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) + 0x50);
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) + 0x50);
      }
      *puVar51 = 0;
    }
    uVar11 = VectorSignedToFloat(*(undefined4 *)(iVar46 + 0x124),(byte)(in_fpscr >> 0x16) & 3);
    uVar59 = VectorSignedToFloat(*(undefined4 *)(iVar46 + 0x128),(byte)(in_fpscr >> 0x16) & 3);
    uVar61 = VectorSignedToFloat(*(undefined4 *)(iVar46 + 300),(byte)(in_fpscr >> 0x16) & 3);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),uVar11,uVar59,uVar61);
    pPVar40 = *(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4);
    uVar11 = Player::getMaxHitpoints();
    iVar5 = __aeabi_idiv(uVar11,6);
    Player::setMaxHitpoints(pPVar40,iVar5);
    uVar11 = VectorSignedToFloat(*(int *)(iVar46 + 0x124) + 3000,(byte)(in_fpscr >> 0x16) & 3);
    uVar59 = VectorSignedToFloat(*(int *)(iVar46 + 0x128) + 2000,(byte)(in_fpscr >> 0x16) & 3);
    uVar61 = VectorSignedToFloat(*(int *)(iVar46 + 300) + -3000,(byte)(in_fpscr >> 0x16) & 3);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
    (**(code **)(*piVar45 + 0x48))(piVar45,uVar11,uVar59,uVar61);
    uVar11 = VectorSignedToFloat(*(int *)(iVar46 + 0x124) + -9000,(byte)(in_fpscr >> 0x16) & 3);
    uVar59 = VectorSignedToFloat(*(int *)(iVar46 + 300) + -7000,(byte)(in_fpscr >> 0x16) & 3);
    uVar61 = VectorSignedToFloat(*(int *)(iVar46 + 0x128) + -8000,(byte)(in_fpscr >> 0x16) & 3);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 8);
    (**(code **)(*piVar45 + 0x48))(piVar45,uVar11,uVar61,uVar59);
    piVar45 = *(int **)(DAT_000c5a38 + 0xc595c);
    for (iVar5 = 3; fVar58 = DAT_000c5690, iVar5 != 7; iVar5 = iVar5 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,9,0,8,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) = uVar11;
      pPVar40 = *(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) + 4);
      iVar50 = Player::getMaxHitpoints();
      Player::setMaxHitpoints(pPVar40,iVar50 * 10);
      iVar52 = *(int *)(iVar46 + 0x124);
      piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4);
      iVar50 = AbyssEngine::AERandom::nextInt(*piVar45);
      iVar12 = *(int *)(iVar46 + 0x128);
      uVar61 = VectorSignedToFloat(iVar52 + iVar50 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      iVar50 = AbyssEngine::AERandom::nextInt(*piVar45);
      uVar11 = VectorSignedToFloat(iVar50 + iVar12 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      uVar59 = VectorSignedToFloat(*(int *)(iVar46 + 300) + 50000,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar37 + 0x48))(piVar37,uVar61,uVar11,uVar59);
    }
    pcVar53 = *(code **)(DAT_000c5a3c + 0xc5a28);
    for (iVar5 = 7; iVar5 != 10; iVar5 = iVar5 + 1) {
      uVar11 = createShip(this,0,0,5,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) = uVar11;
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) + 4),true);
      Player::setHitpoints(*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) + 4));
      fVar60 = local_11c;
      piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4);
      iVar50 = (*pcVar53)(*piVar45,4000);
      fVar56 = local_118;
      fVar57 = (float)VectorSignedToFloat(iVar50 + -2000,(byte)(in_fpscr >> 0x16) & 3);
      iVar50 = (*pcVar53)(*piVar45,0xd48);
      fVar62 = local_114;
      fVar64 = (float)VectorSignedToFloat(iVar50 + -0x6a4,(byte)(in_fpscr >> 0x16) & 3);
      iVar50 = (*pcVar53)(*piVar45,4000);
      fVar63 = (float)VectorSignedToFloat(iVar50 + -2000,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar37 + 0x48))
                (piVar37,fVar60 + fVar57,fVar56 + fVar64,fVar62 + fVar58 + fVar63);
    }
    pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0x100) + 4) + 0xc);
    if (pKVar39 != (KIPlayer *)0x0) {
      KIPlayer::setVisible(pKVar39,true);
      uVar11 = VectorSignedToFloat(*(undefined4 *)(iVar46 + 0x124),(byte)(in_fpscr >> 0x16) & 3);
      uVar59 = VectorSignedToFloat(*(undefined4 *)(iVar46 + 0x128),(byte)(in_fpscr >> 0x16) & 3);
      uVar61 = VectorSignedToFloat(*(int *)(iVar46 + 300) + 40000,(byte)(in_fpscr >> 0x16) & 3);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 0xc);
      (**(code **)(*piVar45 + 0x48))(piVar45,uVar11,uVar59,uVar61);
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x16,0,this);
    goto LAB_000cb9b8;
  case 0x15:
    local_a8 = *(ulonglong *)(DAT_000c5f30 + 0xc5b7a);
    local_a0 = *(longlong *)(DAT_000c5f30 + 0xc5b82);
    local_98 = *(longlong *)(DAT_000c5f30 + 0xc5b8a);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    local_e8 = *(float *)(DAT_000c5f34 + 0xc5ba2);
    fStack_e4 = *(float *)(DAT_000c5f34 + 0xc5ba6);
    local_e0 = *(float *)(DAT_000c5f34 + 0xc5baa);
    local_dc = *(int *)(DAT_000c5f34 + 0xc5bae);
    plVar15 = (longlong *)(DAT_000c5f34 + 0xc5bb2);
    *(Route **)(this + 0x108) = pRVar6;
    local_d8 = *plVar15;
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_e8,3);
    Route::setLoop(SUB41(pRVar6,0));
    local_110 = *(ulonglong *)(DAT_000c5f38 + 0xc5bd8);
    local_108 = *(float *)(DAT_000c5f38 + 0xc5be0);
    uStack_104 = *(undefined4 *)(DAT_000c5f38 + 0xc5be4);
    local_100 = *(undefined8 *)(DAT_000c5f38 + 0xc5be8);
    pRVar29 = operator_new(0x18);
    Route::Route(pRVar29,(int *)&local_110,3);
    Route::setLoop(SUB41(pRVar29,0));
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(4,(Array *)pAVar7);
    pcVar42 = *(code **)(DAT_000c5f3c + 0xc5c24);
    uVar11 = (*pcVar42)(pRVar6);
    pcVar53 = *(code **)(DAT_000c5f40 + 0xc5c38);
    uVar11 = (*pcVar53)(this,0,0,5,uVar11,1,0);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) = uVar11;
    uVar11 = (*pcVar42)(pRVar6);
    uVar11 = (*pcVar53)(this,0,0,5,uVar11,1,0);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 8) = uVar11;
    uVar11 = (*pcVar42)(pRVar6);
    uVar11 = (*pcVar53)(this,0,0,5,uVar11,1,0);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) = uVar11;
    uVar11 = (*pcVar42)(pRVar29);
    uVar11 = (*pcVar53)(this,0,0,0x11,uVar11,1,0);
    piVar45 = *(int **)(DAT_000c5f44 + 0xc5ca0);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    pSVar25 = (String *)GameText::getText(*piVar45);
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
    for (uVar36 = 0; puVar51 = (undefined4 *)(*(uint **)(this + 0xf8))[1],
        uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      KIPlayer::setToSleep((KIPlayer *)puVar51[uVar36]);
      pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      pRVar16 = pRVar29;
      if (uVar36 != 0) {
        pRVar16 = (Route *)Route::clone((__fn *)pRVar6,__child_stack,__flags_00,__arg_00);
      }
      KIPlayer::setRoute(pKVar39,pRVar16);
      AEGeometry::setRotation
                (*(AEGeometry **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 8),
                 extraout_s0_00,extraout_s1_01,extraout_s2_00);
    }
    piVar45 = (int *)*puVar51;
    iVar46 = (*pcVar42)(pRVar29);
    iVar5 = *(int *)(iVar46 + 0x124);
    iVar46 = (*pcVar42)(pRVar29);
    uVar61 = VectorSignedToFloat(*(undefined4 *)(iVar46 + 0x128),(byte)(in_fpscr >> 0x16) & 3);
    iVar46 = (*pcVar42)(pRVar29);
    uVar11 = VectorSignedToFloat(iVar5 + 1000,(byte)(in_fpscr >> 0x16) & 3);
    uVar59 = VectorSignedToFloat(*(int *)(iVar46 + 300) + 2000,(byte)(in_fpscr >> 0x16) & 3);
    (**(code **)(*piVar45 + 0x48))(piVar45,uVar11,uVar61,uVar59);
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x16,0,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,7,1,this);
    *(Objective **)(this + 0x2c) = pOVar14;
    pvVar13 = (void *)Route::~Route(pRVar6);
    operator_delete(pvVar13);
    break;
  case 0x18:
    local_a8 = *(ulonglong *)(DAT_000c5f4c + 0xc5db0);
    local_a0 = *(longlong *)(DAT_000c5f4c + 0xc5db8);
    local_98 = *(longlong *)(DAT_000c5f4c + 0xc5dc0);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(5,(Array *)pAVar7);
    for (iVar46 = 0; iVar46 != 3; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,9,0,8,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      pKVar39 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),*(void **)(*(int *)(this + 0xf8) + 4),
                                     __flags_01,__arg_01);
      KIPlayer::setRoute(pKVar39,pRVar6);
    }
    for (iVar46 = 3; iVar46 != 5; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,2,1,0xf,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4),false);
      pPVar40 = *(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4);
      uVar11 = Player::getMaxHitpoints();
      iVar5 = __aeabi_idiv(uVar11,3);
      Player::setMaxHitpoints(pPVar40,iVar5);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 0x50);
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 0x50);
      }
      *puVar51 = 0;
    }
    KIPlayer::setVisible(*(KIPlayer **)(*(int *)(*(int *)(this + 0x100) + 4) + 0xc),false);
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x1f,0,this);
    goto LAB_000cb9b8;
  case 0x19:
  case 0x1d:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(3,(Array *)pAVar7);
    puVar51 = *(undefined4 **)(DAT_000c4e9c + 0xc4cf8);
    pcVar53 = *(code **)(DAT_000c4ea0 + 0xc4cfa);
    for (iVar46 = 0; iVar46 != 3; iVar46 = iVar46 + 1) {
      uVar11 = createShip(this,9,0,8,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      iVar5 = (*pcVar53)(*puVar51,2);
      uVar11 = DAT_000c4ea4;
      iVar50 = (*pcVar53)(*puVar51);
      iVar52 = (*pcVar53)(*puVar51,2);
      iVar12 = -1;
      if (iVar52 == 0) {
        iVar12 = 1;
      }
      iVar52 = (*pcVar53)(*puVar51,uVar11);
      iVar55 = -1;
      if (iVar5 == 0) {
        iVar55 = 1;
      }
      iVar5 = (*pcVar53)(*puVar51,2);
      iVar41 = -1;
      if (iVar5 == 0) {
        iVar41 = 1;
      }
      uVar61 = VectorSignedToFloat((iVar50 + 20000) * iVar55,(byte)(in_fpscr >> 0x16) & 3);
      iVar5 = (*pcVar53)(*puVar51,DAT_000c4ea4);
      uVar11 = VectorSignedToFloat((iVar52 + 20000) * iVar12,(byte)(in_fpscr >> 0x16) & 3);
      uVar59 = VectorSignedToFloat(iVar41 * (iVar5 + 20000),(byte)(in_fpscr >> 0x16) & 3);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      (**(code **)(*piVar45 + 0x48))(piVar45,uVar61,uVar11,uVar59);
    }
    break;
  case 0x1a:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(2,(Array *)pAVar7);
    puVar51 = *(undefined4 **)(DAT_000c5f50 + 0xc5f1e);
    pcVar53 = *(code **)(DAT_000c5f54 + 0xc5f22);
    for (iVar46 = 0; iVar46 != 2; iVar46 = iVar46 + 1) {
      uVar11 = createShip(this,9,0,8,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      PlayerEgo::getPosition();
      AEGeometry::getDirection();
      fVar58 = (float)AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_e8,(Vector *)&local_110)
      ;
      AbyssEngine::AEMath::operator*((Vector *)&local_110,fVar58);
      AbyssEngine::AEMath::Vector::operator-=((Vector *)&local_a8,(Vector *)&local_110);
      iVar5 = (*pcVar53)(*puVar51,0x578);
      fVar58 = (float)VectorSignedToFloat(iVar5 + -700,(byte)(in_fpscr >> 0x16) & 3);
      local_a8._0_4_ = (float)local_a8 + fVar58;
      iVar5 = (*pcVar53)(*puVar51,0x578);
      fVar58 = (float)VectorSignedToFloat(iVar5 + -700,(byte)(in_fpscr >> 0x16) & 3);
      local_a8 = CONCAT44(local_a8._4_4_ + fVar58,(float)local_a8);
      iVar5 = (*pcVar53)(*puVar51,0x578);
      fVar58 = (float)VectorSignedToFloat(iVar5 + -700,(byte)(in_fpscr >> 0x16) & 3);
      local_a0 = CONCAT44(local_a0._4_4_,(float)local_a0 + fVar58);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_a8);
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,7,2,this);
    goto LAB_000cb9b8;
  case 0x1c:
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 0xc);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c634c,DAT_000c6350,DAT_000c6354);
    (**(code **)(**(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 0xc) + 0x28))
              ((Vector *)&local_a8);
    AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_a8);
    local_a8 = CONCAT44((int)*(float *)(this + 400),(int)*(float *)(this + 0x18c));
    local_a0 = CONCAT44(local_a0._4_4_,(int)*(float *)(this + 0x194));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(8,(Array *)pAVar7);
    for (iVar46 = 0; iVar46 != 5; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,9,0,8,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
    }
    for (iVar46 = 5; iVar46 != 8; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,0,1,0xf,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4),false);
      pPVar40 = *(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4);
      iVar5 = Player::getMaxHitpoints();
      Player::setMaxHitpoints(pPVar40,(int)(iVar5 + ((uint)(iVar5 >> 0x1f) >> 0x1e)) >> 2);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 0x50);
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 0x50);
      }
      *puVar51 = 0;
    }
    break;
  case 0x24:
    this_00 = (Mission *)Status::getMission();
    pAVar17 = operator_new(0x98);
    piVar37 = *(int **)(DAT_000c6358 + 0xc61a2);
    pSVar25 = (String *)GameText::getText(*piVar37);
    AbyssEngine::String::String(aSStack_128,pSVar25,false);
    Agent::Agent(pAVar17,0xffffffff,aSStack_128,0x1d,5,1,1,0xffffffff,0xffffffff,0xffffffff,
                 0xffffffff);
    Mission::setAgent(this_00,pAVar17);
    AbyssEngine::String::~String(aSStack_128);
    __aeabi_memcpy(&local_a8,DAT_000c635c + 0xc61f8,0x30);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,0xc);
    *(Route **)(this + 0x108) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(8,(Array *)pAVar7);
    uVar11 = createShip(this,1,0,9,(Waypoint *)0x0,true,false);
    iVar46 = DAT_000c6360;
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    piVar54 = *(int **)(iVar46 + 0xc6254);
    piVar45 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    iVar46 = AbyssEngine::AERandom::nextInt(*piVar54);
    fVar60 = (float)VectorSignedToFloat(iVar46 + -700,(byte)(in_fpscr >> 0x16) & 3);
    iVar46 = AbyssEngine::AERandom::nextInt(*piVar54);
    fVar58 = (float)VectorSignedToFloat(iVar46 + -700,(byte)(in_fpscr >> 0x16) & 3);
    (**(code **)(*piVar45 + 0x48))
              (piVar45,local_11c + fVar60,local_118 + fVar58,local_114 + DAT_000c6364);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x1c))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000c6368);
    PlayerFighter::setRotate((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),3);
    Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
    pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x108),
                                   *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_02,__arg_02);
    KIPlayer::setRoute(pKVar39,pRVar6);
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    pSVar25 = (String *)GameText::getText(*piVar37);
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    puVar18 = *(uint **)(this + 0xf8);
    *(undefined4 *)(*(int *)puVar18[1] + 0x50) = 0;
    for (uVar36 = 1; uVar36 < *puVar18; uVar36 = uVar36 + 1) {
      iVar46 = Globals::getRandomEnemyFighter
                         ((Globals *)**(undefined4 **)(DAT_000c6708 + 0xc6378),8);
      pRVar6 = *(Route **)(this + 0x108);
      iVar5 = *piVar54;
      Route::length();
      iVar5 = AbyssEngine::AERandom::nextInt(iVar5);
      pWVar8 = (Waypoint *)Route::getWaypoint(pRVar6,iVar5);
      uVar11 = createShip(this,8,0,iVar46,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4));
      puVar18 = *(uint **)(this + 0xf8);
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x14,1,8,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x15,1,8,this);
    goto LAB_000c9412;
  case 0x26:
    local_a8 = *(ulonglong *)(DAT_000c670c + 0xc6414);
    local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000c670c + 0xc641c));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(7,(Array *)pAVar7);
    pcVar53 = *(code **)(DAT_000c6710 + 0xc6456);
    puVar51 = *(undefined4 **)(DAT_000c6714 + 0xc6460);
    pcVar42 = *(code **)(DAT_000c6718 + 0xc6466);
    for (iVar46 = 0; iVar46 != 2; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)(*pcVar53)(*(undefined4 *)(this + 0x110));
      uVar11 = createShip(this,2,1,0xf,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),true);
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4),false);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      iVar5 = (*pcVar53)(*(undefined4 *)(this + 0x110));
      iVar5 = *(int *)(iVar5 + 0x124);
      iVar50 = (*pcVar42)(*puVar51,20000);
      iVar52 = (*pcVar53)(*(undefined4 *)(this + 0x110));
      iVar55 = *(int *)(iVar52 + 0x128);
      iVar52 = (*pcVar42)(*puVar51,20000);
      iVar12 = (*pcVar53)(*(undefined4 *)(this + 0x110));
      iVar12 = *(int *)(iVar12 + 300);
      uVar59 = VectorSignedToFloat(iVar5 + iVar50 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      uVar61 = VectorSignedToFloat(iVar52 + iVar55 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      iVar5 = (*pcVar42)(*puVar51,20000);
      uVar11 = VectorSignedToFloat(iVar5 + iVar12 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar45 + 0x48))(piVar45,uVar59,uVar61,uVar11);
    }
    puVar51 = *(undefined4 **)(DAT_000c671c + 0xc6564);
    for (uVar36 = 2; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,3);
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,3,0,iVar46,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x12,2,7,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,7,2,this);
    goto LAB_000c9412;
  case 0x28:
    local_e0 = *(float *)(DAT_000c6720 + 0xc65fe);
    local_e8 = (float)*(undefined8 *)(DAT_000c6720 + 0xc65f6);
    fStack_e4 = (float)((ulonglong)*(undefined8 *)(DAT_000c6720 + 0xc65f6) >> 0x20);
    local_a8 = *(ulonglong *)(DAT_000c6724 + 0xc65f8);
    local_a0 = *(longlong *)(DAT_000c6724 + 0xc6600);
    local_98 = *(longlong *)(DAT_000c6724 + 0xc6608);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_e8,3);
    *(Route **)(this + 0x110) = pRVar6;
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    *(Route **)(this + 0x10c) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(0xd,(Array *)pAVar7);
    uVar11 = createShip(this,0,1,0xd,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    piVar45 = *(int **)(DAT_000c6728 + 0xc669a);
    pSVar25 = (String *)GameText::getText(*piVar45);
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    pPVar40 = *(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4);
    iVar46 = Status::getLevel();
    Player::setMaxHitpoints(pPVar40,iVar46 * 5 + 0x708);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000c672c,DAT_000c672c,
               DAT_000c672c);
    KIPlayer::setToSleep((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4));
    KIPlayer::setVisible((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
    for (iVar46 = 1; iVar46 != 5; iVar46 = iVar46 + 1) {
      iVar5 = Globals::getRandomEnemyFighter((Globals *)**(undefined4 **)(DAT_000c6b14 + 0xc6738),0)
      ;
      uVar11 = createShip(this,0,0,iVar5,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      pKVar39 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x10c),*(void **)(*(int *)(this + 0xf8) + 4),
                                     __flags_03,__arg_03);
      KIPlayer::setRoute(pKVar39,pRVar6);
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),true);
      if (iVar46 == 2) {
        pSVar25 = (String *)GameText::getText(*piVar45);
        AbyssEngine::String::operator=
                  ((String *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 8) + 0x18),pSVar25);
      }
    }
    for (iVar46 = 5; fVar58 = DAT_000c6b18, iVar46 != 9; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,9,0,8,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
    }
    for (iVar46 = 9; iVar46 != 0xd; iVar46 = iVar46 + 1) {
      uVar11 = createShip(this,9,0,8,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      local_110 = CONCAT44(fVar58,fVar58);
      local_108 = fVar58;
      (**(code **)(*piVar45 + 0x44))(piVar45,&local_110);
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
    }
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 0xc);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c6b1c,DAT_000c6b20,DAT_000c6b24);
    if (**(char **)(DAT_000c6b28 + 0xc6882) != '\0') {
      PlayerEgo::setPosition(extraout_s0_01,extraout_s1_02,extraout_s2_01);
      AEGeometry::setRotation
                (*(AEGeometry **)(*(int *)(this + 0xf0) + 8),extraout_s0_02,extraout_s1_03,
                 extraout_s2_02);
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,7,1,this);
    goto LAB_000c9412;
  case 0x29:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(8,(Array *)pAVar7);
    uVar11 = createShip(this,1,1,0xd,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c6b38 + 0xc6912));
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    piVar45 = *(int **)(DAT_000c6b3c + 0xc692e);
    if (*piVar45 < 1) {
      iVar46 = Status::getLevel();
      fVar58 = (float)VectorSignedToFloat(iVar46 * 5 + 0x708,(byte)(in_fpscr >> 0x16) & 3);
      *piVar45 = (int)(fVar58 * DAT_000c6b40);
    }
    Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
    PlayerFixedObject::setMoving
              ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),true);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),0,0,DAT_000c6b44);
    for (iVar46 = 1; iVar46 != 8; iVar46 = iVar46 + 1) {
      uVar11 = createShip(this,9,0,8,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
    }
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
    (**(code **)(*piVar45 + 0x48))(piVar45,0,0,DAT_000c6b48);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 8);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c6b4c,DAT_000c6b50,DAT_000c6b54);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c6b5c,DAT_000c6b58,DAT_000c6b60);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x10);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c6b64,DAT_000c6b68,DAT_000c6b6c);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x14);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c6b70,DAT_000c6b74,DAT_000c6b78);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c6b7c,DAT_000c6b80,DAT_000c6b84);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c6b88,DAT_000c6b8c,DAT_000c6b90);
    PlayerEgo::setPosition(extraout_s0_03,extraout_s1_04,extraout_s2_03);
    AEGeometry::setRotation
              (*(AEGeometry **)(*(int *)(this + 0xf0) + 8),extraout_s0_04,extraout_s1_05,
               extraout_s2_04);
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x19,0,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,7,1,this);
    goto LAB_000c9412;
  case 0x30:
  case 0x31:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(5,(Array *)pAVar7);
    for (uVar36 = 0; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      uVar11 = createShip(this,1,0,9,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),true);
    }
    break;
  case 0x32:
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 4);
    if (piVar45 == (int *)0x0) {
      local_a8 = 0;
      local_a0 = CONCAT44(local_a0._4_4_,DAT_000cbd4c);
    }
    else {
      (**(code **)(*piVar45 + 0x28))(&local_a8);
    }
    AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_a8);
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(4,(Array *)pAVar7);
    for (uVar36 = 0; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      uVar11 = createShip(this,1,0,9,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),true);
    }
    break;
  case 0x33:
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 4);
    if (piVar45 == (int *)0x0) {
      local_a8 = 0;
      local_a0 = CONCAT44(local_a0._4_4_,DAT_000cbd4c);
    }
    else {
      (**(code **)(*piVar45 + 0x28))(&local_a8);
    }
    pVVar38 = (Vector *)(this + 0x18c);
    AbyssEngine::AEMath::Vector::operator=(pVVar38,(Vector *)&local_a8);
    local_a8 = CONCAT44((int)*(float *)(this + 400),(int)*(float *)(this + 0x18c));
    local_a0 = CONCAT44(local_a0._4_4_,(int)*(float *)(this + 0x194));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    Route::setLoop(SUB41(pRVar6,0));
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(6,(Array *)pAVar7);
    uVar11 = createShip(this,1,1,0xd,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
    PlayerEgo::getPosition();
    fVar58 = (float)AbyssEngine::AEMath::operator-((Vector *)&local_68,(Vector *)&local_134);
    AbyssEngine::AEMath::operator/((Vector *)&local_110,fVar58);
    AbyssEngine::AEMath::operator+((Vector *)&local_e8,pVVar38);
    AbyssEngine::AEMath::Vector::operator=(pVVar38,(Vector *)&local_e8);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),*(undefined4 *)(this + 0x18c),
               *(undefined4 *)(this + 400),*(undefined4 *)(this + 0x194));
    for (uVar36 = 1; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,1,0,9,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
      pKVar39 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),*(void **)(*(int *)(this + 0xf8) + 4),
                                     __flags_19,__arg_21);
      KIPlayer::setRoute(pKVar39,pRVar6);
    }
    break;
  case 0x34:
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 4);
    if (piVar45 == (int *)0x0) {
      local_a8 = 0;
      local_a0 = CONCAT44(local_a0._4_4_,DAT_000cbd4c);
    }
    else {
      (**(code **)(*piVar45 + 0x28))(&local_a8);
    }
    pVVar38 = (Vector *)(this + 0x18c);
    AbyssEngine::AEMath::Vector::operator=(pVVar38,(Vector *)&local_a8);
    local_a8 = CONCAT44((int)*(float *)(this + 400),(int)*(float *)(this + 0x18c));
    local_a0 = CONCAT44(local_a0._4_4_,(int)*(float *)(this + 0x194));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    Route::setLoop(SUB41(pRVar6,0));
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(8,(Array *)pAVar7);
    uVar11 = createShip(this,1,1,0xd,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
    PlayerEgo::getPosition();
    fVar58 = (float)AbyssEngine::AEMath::operator-((Vector *)&local_68,(Vector *)&local_134);
    AbyssEngine::AEMath::operator/((Vector *)&local_110,fVar58);
    AbyssEngine::AEMath::operator+((Vector *)&local_e8,pVVar38);
    AbyssEngine::AEMath::Vector::operator=(pVVar38,(Vector *)&local_e8);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),*(undefined4 *)(this + 0x18c),
               *(undefined4 *)(this + 400),*(undefined4 *)(this + 0x194));
    uVar11 = createShip(this,1,1,0xd,(Waypoint *)0x0,true,false);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) = uVar11;
    Player::setAlwaysEnemy
              (SUB41(*(undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 4),0));
    fVar60 = *(float *)(this + 0x18c) + DAT_000cbd54;
    fVar56 = *(float *)(this + 400) + DAT_000cbd58;
    fVar58 = *(float *)(this + 0x194) + DAT_000cbd54;
    *(float *)(this + 0x18c) = fVar60;
    *(float *)(this + 400) = fVar56;
    *(float *)(this + 0x194) = fVar58;
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
    (**(code **)(*piVar45 + 0x48))(piVar45,fVar60,fVar56,fVar58);
    for (uVar36 = 2; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,1,0,9,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
      pKVar39 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),*(void **)(*(int *)(this + 0xf8) + 4),
                                     __flags_20,__arg_22);
      KIPlayer::setRoute(pKVar39,pRVar6);
    }
    break;
  case 0x38:
    __aeabi_memcpy(&local_a8,DAT_000c6e68 + 0xc6ba6,0x24);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,9);
    *(Route **)(this + 0x108) = pRVar6;
    iVar46 = Route::clone((__fn *)pRVar6,__child_stack_00,__flags_04,__arg_04);
    *(int *)(this + 0x10c) = iVar46;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(9,(Array *)pAVar7);
    fVar58 = DAT_000c6e6c;
    for (iVar46 = 0; iVar46 != 3; iVar46 = iVar46 + 1) {
      uVar11 = createShip(this,8,0,0x18,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),true);
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
      fVar60 = (float)PlayerEgo::getPosition();
      AbyssEngine::AEMath::operator*((Vector *)&local_e8,fVar60);
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_e8);
      fVar56 = *(float *)(this + 0x18c);
      puVar51 = *(undefined4 **)(DAT_000c6e74 + 0xc6c7a);
      pcVar53 = *(code **)(DAT_000c6e78 + 0xc6c84);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      uVar11 = (*pcVar53)(*puVar51,7000);
      fVar62 = *(float *)(this + 400);
      uVar59 = (*pcVar53)(*puVar51,7000);
      fVar57 = *(float *)(this + 0x194);
      fVar63 = (float)VectorSignedToFloat(uVar11,(byte)(in_fpscr >> 0x16) & 3);
      fVar64 = (float)VectorSignedToFloat(uVar59,(byte)(in_fpscr >> 0x16) & 3);
      uVar11 = (*pcVar53)(*puVar51,7000);
      fVar60 = (float)VectorSignedToFloat(uVar11,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar45 + 0x48))
                (piVar45,fVar56 + fVar58 + fVar63,fVar62 + fVar58 + fVar64,fVar57 + fVar58 + fVar60)
      ;
      pKVar39 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x10c),*(void **)(*(int *)(this + 0xf8) + 4),
                                     __flags_05,__arg_05);
      KIPlayer::setRoute(pKVar39,pRVar6);
      Player::setHitpoints(*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4))
      ;
    }
    puVar51 = *(undefined4 **)(DAT_000c6e80 + 0xc6d3a);
    for (uVar36 = 3; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
      iVar5 = 2;
      if (uVar36 < 10) {
        iVar5 = 1;
      }
      if (uVar36 < 5) {
        iVar5 = 0;
      }
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x108),iVar5);
      uVar11 = createShip(this,8,0,iVar46,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4));
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x12,3,9,this);
    goto LAB_000cb9b8;
  case 0x3f:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(7,(Array *)pAVar7);
    local_a8 = 0;
    local_a0 = (ulonglong)local_a0._4_4_ << 0x20;
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    pWVar8 = (Waypoint *)Route::getWaypoint();
    uVar11 = createStaticObject(this,pWVar8,0x37a3,true);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    piVar45 = (int *)Route::getWaypoint();
    (**(code **)(*piVar45 + 0x28))(&local_e8,piVar45);
    KIPlayer::setToSleep((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4));
    pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c6e84 + 0xc6e2e));
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    puVar51 = *(undefined4 **)(DAT_000c6e88 + 0xc6e52);
    puVar19 = *(undefined4 **)(DAT_000c6e8c + 0xc6e5c);
    pcVar53 = *(code **)(DAT_000c6e90 + 0xc6e60);
    for (uVar36 = 1; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
      uVar11 = createShip(this,8,0,iVar46,(Waypoint *)0x0,true,false);
      fVar58 = local_e8;
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      iVar46 = (*pcVar53)(*puVar19,20000);
      iVar5 = (*pcVar53)(*puVar19,2);
      fVar60 = fStack_e4;
      iVar50 = (*pcVar53)(*puVar19,20000);
      iVar52 = (*pcVar53)(*puVar19,2);
      fVar56 = local_e0;
      iVar12 = -1;
      if (iVar52 == 0) {
        iVar12 = 1;
      }
      iVar52 = -1;
      if (iVar5 == 0) {
        iVar52 = 1;
      }
      iVar5 = (*pcVar53)(*puVar19,20000);
      fVar63 = (float)VectorSignedToFloat(iVar52 * iVar46 + 10000,(byte)(in_fpscr >> 0x16) & 3);
      fVar62 = (float)VectorSignedToFloat(iVar12 * iVar50 + 10000,(byte)(in_fpscr >> 0x16) & 3);
      iVar46 = (*pcVar53)(*puVar19,2);
      iVar50 = -1;
      if (iVar46 == 0) {
        iVar50 = 1;
      }
      fVar57 = (float)VectorSignedToFloat(iVar50 * iVar5 + 10000,(byte)(in_fpscr >> 0x16) & 3);
      uVar59 = VectorSignedToFloat((int)(fVar58 + fVar63),(byte)(in_fpscr >> 0x16) & 3);
      uVar61 = VectorSignedToFloat((int)(fVar60 + fVar62),(byte)(in_fpscr >> 0x16) & 3);
      uVar11 = VectorSignedToFloat((int)(fVar56 + fVar57),(byte)(in_fpscr >> 0x16) & 3);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      (**(code **)(*piVar45 + 0x48))(piVar45,uVar59,uVar61,uVar11);
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4));
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x1b,4,this);
    goto LAB_000cb9b8;
  case 0x40:
    uVar11 = *(undefined4 *)(this + 0xf0);
    PlayerEgo::getPosition();
    fVar58 = (float)AEGeometry::getDirection();
    AbyssEngine::AEMath::operator*((Vector *)&local_e8,fVar58);
    AbyssEngine::AEMath::operator-((Vector *)&local_140,(Vector *)&local_a8);
    PlayerEgo::setPosition(uVar11,local_140,uStack_13c,uStack_138);
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(9,(Array *)pAVar7);
    local_a8 = *(ulonglong *)(DAT_000c730c + 0xc7024);
    local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000c730c + 0xc702c));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    uVar11 = createShip(this,0,0,0x26,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c7310 + 0xc707c));
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),0,0,0);
    fStack_e4 = 0.0;
    local_e8 = 1.0;
    local_e0 = 0.0;
    local_110 = 0x3f80000000000000;
    local_108 = 0.0;
    AEGeometry::setDirection
              (*(Vector **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8),(Vector *)&local_e8);
    pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),
                                   *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_06,__arg_06);
    KIPlayer::setRoute(pKVar39,pRVar6);
    puVar18 = *(uint **)(this + 0xf8);
    *(undefined4 *)(*(int *)puVar18[1] + 0x50) = 0;
    for (uVar36 = 1; uVar36 < *puVar18; uVar36 = uVar36 + 1) {
      iVar46 = Globals::getRandomEnemyFighter
                         ((Globals *)**(undefined4 **)(DAT_000c7314 + 0xc70f8),8);
      uVar11 = createShip(this,8,0,iVar46,(Waypoint *)0x0,true,false);
      iVar46 = DAT_000c7318;
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      puVar51 = *(undefined4 **)(iVar46 + 0xc7126);
      pcVar53 = *(code **)(DAT_000c731c + 0xc7134);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      iVar46 = (*pcVar53)(*puVar51,0x5dc);
      iVar5 = (*pcVar53)(*puVar51,2);
      iVar50 = (*pcVar53)(*puVar51,0x5dc);
      iVar52 = (*pcVar53)(*puVar51,2);
      iVar12 = -1;
      if (iVar52 == 0) {
        iVar12 = 1;
      }
      iVar52 = -1;
      if (iVar5 == 0) {
        iVar52 = 1;
      }
      iVar5 = (*pcVar53)(*puVar51,0x5dc);
      uVar11 = VectorSignedToFloat((iVar46 + 1000) * iVar52,(byte)(in_fpscr >> 0x16) & 3);
      iVar46 = (*pcVar53)(*puVar51,2);
      iVar52 = -1;
      if (iVar46 == 0) {
        iVar52 = 1;
      }
      uVar59 = VectorSignedToFloat(iVar12 * (iVar50 + 1000),(byte)(in_fpscr >> 0x16) & 3);
      uVar61 = VectorSignedToFloat((iVar5 + 1000) * iVar52,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar45 + 0x48))(piVar45,uVar11,uVar59,uVar61);
      fStack_e4 = 0.0;
      local_e8 = 1.0;
      local_e0 = 0.0;
      local_110 = 0x3f80000000000000;
      local_108 = 0.0;
      AEGeometry::setDirection
                (*(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 8),
                 (Vector *)&local_e8);
      pKVar39 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),*(void **)(*(int *)(this + 0xf8) + 4),
                                     __flags_07,__arg_07);
      KIPlayer::setRoute(pKVar39,pRVar6);
      puVar18 = *(uint **)(this + 0xf8);
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x16,0,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,1,0,this);
    goto LAB_000c9412;
  case 0x41:
    local_a8 = 0;
    local_a0 = (ulonglong)local_a0._4_4_ << 0x20;
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(1,(Array *)pAVar7);
    uVar11 = createShip(this,0,0,0x26,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c7320 + 0xc72c6));
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),
                                   *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_08,__arg_08);
    KIPlayer::setRoute(pKVar39,pRVar6);
    Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
    break;
  case 0x43:
    local_a8 = *(ulonglong *)(DAT_000c75d4 + 0xc732e);
    local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000c75d4 + 0xc7336));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    iVar46 = Route::clone((__fn *)pRVar6,__child_stack_01,__flags_09,__arg_09);
    *(int *)(this + 0x108) = iVar46;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(0xb,(Array *)pAVar7);
    uVar11 = createStaticObject(this,(Waypoint *)0x0,0x37a3,true);
    piVar45 = *(int **)(DAT_000c75d8 + 0xc7394);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    pSVar25 = (String *)GameText::getText(*piVar45);
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    uVar11 = VectorSignedToFloat((float)local_a8,(byte)(in_fpscr >> 0x16) & 3);
    uVar59 = VectorSignedToFloat(local_a8._4_4_,(byte)(in_fpscr >> 0x16) & 3);
    uVar61 = VectorSignedToFloat((float)local_a0,(byte)(in_fpscr >> 0x16) & 3);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),uVar11,uVar59,uVar61);
    Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    KIPlayer::setToSleep((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4));
    iVar46 = DAT_000c75e0;
    puVar51 = *(undefined4 **)(DAT_000c75dc + 0xc741e);
    for (uVar36 = 1; uVar36 < **(int **)(this + 0xf8) - 1U; uVar36 = uVar36 + 1) {
      iVar5 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,8,0,iVar5,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4));
      piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      piVar37[0x4a] = iVar46;
      if (4 < uVar36) {
        (**(code **)(*piVar37 + 0x48))(piVar37,DAT_000c75e4,DAT_000c75e4,DAT_000c75e4);
      }
    }
    uVar11 = VectorSignedToFloat(local_a8._4_4_,(byte)(in_fpscr >> 0x16) & 3);
    uVar59 = VectorSignedToFloat((float)local_a0,(byte)(in_fpscr >> 0x16) & 3);
    uVar61 = VectorSignedToFloat((int)(float)local_a8 + 30000,(byte)(in_fpscr >> 0x16) & 3);
    piVar37 = *(int **)((*(int **)(this + 0xf8))[1] + 4);
    (**(code **)(*piVar37 + 0x48))(piVar37,uVar61,uVar11,uVar59);
    uVar11 = createShip(this,0,0,0x1b,(Waypoint *)0x0,true,false);
    *(undefined4 *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) = uVar11;
    PlayerEgo::getPosition();
    piVar37 = *(int **)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4);
    local_68 = CONCAT44(DAT_000c75e8,DAT_000c75f0);
    local_60 = DAT_000c75ec;
    AbyssEngine::AEMath::operator+((Vector *)&local_110,(Vector *)&local_e8);
    (**(code **)(*piVar37 + 0x44))(piVar37,(Vector *)&local_110);
    Player::setAlwaysFriend
              (*(Player **)
                (*(int *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) + 4),true)
    ;
    pSVar25 = (String *)GameText::getText(*piVar45);
    AbyssEngine::String::operator=
              ((String *)
               (*(int *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) + 0x18),
               pSVar25);
    Player::setMaxHitpoints
              (*(Player **)
                (*(int *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) + 4),
               DAT_000c75f4);
    iVar46 = **(int **)(this + 0xf8);
    pvVar13 = (void *)((*(int **)(this + 0xf8))[1] + iVar46 * 4);
    pKVar39 = *(KIPlayer **)((int)pvVar13 + -4);
    pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x108),pvVar13,iVar46,__arg_10);
    KIPlayer::setRoute(pKVar39,pRVar6);
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x16,0,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,1,0,this);
    goto LAB_000c9412;
  case 0x45:
    StarSystem::getPlanets(*(StarSystem **)(this + 0xec));
    fVar58 = (float)AEGeometry::getPosition();
    AbyssEngine::AEMath::Vector::operator*=((Vector *)&local_e8,fVar58);
    auVar65._4_4_ = fStack_e4;
    auVar65._0_4_ = local_e8;
    auVar65._8_4_ = local_e0;
    auVar65._12_4_ = local_e8 * 10.0;
    auVar65 = FPToFixed(auVar65,0,0,3,0x20);
    local_a8 = auVar65._0_8_;
    local_a0 = auVar65._8_8_;
    local_98 = CONCAT44((int)(local_e0 * 10.0),(int)(fStack_e4 * 10.0));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(5,(Array *)pAVar7);
    uVar11 = createShip(this,0,0,0xc,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c7a78 + 0xc76bc));
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),
                                   *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_10,__arg_11);
    KIPlayer::setRoute(pKVar39,pRVar6);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x44))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),(Vector *)&local_e8);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x1c))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000c7a7c);
    AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_110,(Vector *)&local_e8);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e8,(Vector *)&local_110);
    local_110 = 0x3f80000000000000;
    local_108 = 0.0;
    AEGeometry::setDirection
              (*(Vector **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8),(Vector *)&local_e8);
    local_110 = *(ulonglong *)(DAT_000c7a80 + 0xc7744);
    local_108 = *(float *)(DAT_000c7a80 + 0xc774c);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_110,3);
    *(Route **)(this + 0x10c) = pRVar6;
    for (uVar36 = 1; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      iVar46 = Globals::getRandomEnemyFighter
                         ((Globals *)**(undefined4 **)(DAT_000c7a84 + 0xc7772),0);
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,0,0,iVar46,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x16,0,this);
    goto LAB_000cb9b8;
  case 0x46:
    uVar11 = *(undefined4 *)(this + 0xf0);
    PlayerEgo::getPosition();
    fVar58 = (float)AEGeometry::getDirection();
    AbyssEngine::AEMath::operator*((Vector *)&local_e8,fVar58);
    AbyssEngine::AEMath::operator-((Vector *)&local_14c,(Vector *)&local_a8);
    PlayerEgo::setPosition(uVar11,local_14c,uStack_148,uStack_144);
    (**(code **)(**(int **)(*(int *)(*(int *)(this + 0x100) + 4) + 4) + 0x28))(&local_a8);
    PlayerEgo::getPosition();
    local_108 = (float)(int)(float)local_a0;
    local_110 = CONCAT44((int)local_a8._4_4_,(int)(float)local_a8);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_110,3);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(1,(Array *)pAVar7);
    uVar11 = createShip(this,0,0,0xc,(Waypoint *)0x0,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c7a8c + 0xc78ac));
    AbyssEngine::String::operator=((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25)
    ;
    pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),
                                   *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_11,__arg_12);
    KIPlayer::setRoute(pKVar39,pRVar6);
    fVar58 = (float)AbyssEngine::AEMath::operator-((Vector *)&local_158,(Vector *)&local_a8);
    AbyssEngine::AEMath::operator/((Vector *)&local_134,fVar58);
    AbyssEngine::AEMath::operator+((Vector *)&local_68,(Vector *)&local_e8);
    AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_68);
    (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x44))
              ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),(Vector *)(this + 0x18c));
    AbyssEngine::AEMath::operator-((Vector *)&local_134,(Vector *)&local_a8);
    AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_68,(Vector *)&local_134);
    local_134 = 0.0;
    local_130 = 1.0;
    local_12c = 0.0;
    AEGeometry::setDirection
              (*(Vector **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8),(Vector *)&local_68);
    iVar46 = *(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4);
    uVar11 = Player::getMaxHitpoints();
    VectorSignedToFloat(uVar11,(byte)(in_fpscr >> 0x16) & 3);
    Player::setHitpoints(iVar46);
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,1,0,this);
    goto LAB_000cb9b8;
  case 0x49:
    local_a8 = *(ulonglong *)(DAT_000c7a90 + 0xc79a4);
    local_a0 = *(longlong *)(DAT_000c7a90 + 0xc79ac);
    local_98 = *(longlong *)(DAT_000c7a90 + 0xc79b4);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    *(Route **)(this + 0x110) = pRVar6;
    PlayerEgo::setPosition(extraout_s0_05,extraout_s1_06,extraout_s2_05);
    uVar11 = VectorSignedToFloat((undefined4)local_98,(byte)(in_fpscr >> 0x16) & 3);
    local_108 = (float)VectorSignedToFloat(local_98._4_4_,(byte)(in_fpscr >> 0x16) & 3);
    uVar59 = VectorSignedToFloat(local_a0._4_4_ + 20000,(byte)(in_fpscr >> 0x16) & 3);
    local_110 = CONCAT44(uVar11,uVar59);
    PlayerEgo::getPosition();
    AbyssEngine::AEMath::operator-((Vector *)&local_e8,(Vector *)&local_110);
    pVVar38 = *(Vector **)(*(int *)(this + 0xf0) + 8);
    AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_110,(Vector *)&local_e8);
    local_68 = 0x3f80000000000000;
    local_60 = 0.0;
    AEGeometry::setDirection(pVVar38,(Vector *)&local_110);
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(0xc,(Array *)pAVar7);
    fVar58 = DAT_000c7aa0;
    puVar51 = *(undefined4 **)(DAT_000c7a9c + 0xc7a72);
    for (uVar36 = 0; uVar36 != 8; uVar36 = uVar36 + 1) {
      iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
      uVar11 = createShip(this,8,0,iVar46,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4));
      if (3 < uVar36) {
        piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
        local_110 = CONCAT44(fVar58,fVar58);
        local_108 = fVar58;
        (**(code **)(*piVar45 + 0x44))(piVar45,&local_110);
      }
    }
    pcVar53 = *(code **)(DAT_000c7ec8 + 0xc7b2a);
    pcVar42 = *(code **)(DAT_000c7ecc + 0xc7b32);
    puVar51 = *(undefined4 **)(DAT_000c7ed0 + 0xc7b3e);
    for (uVar36 = 8; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      uVar11 = createShip(this,0,1,0xf,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),true);
      iVar46 = *(int *)(*(int *)(this + 0xf8) + 4);
      *(undefined4 *)(*(int *)(iVar46 + uVar36 * 4) + 0x50) = 0;
      PlayerFixedObject::setMoving(*(PlayerFixedObject **)(iVar46 + uVar36 * 4),true);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      iVar46 = (*pcVar53)(*(undefined4 *)(this + 0x110),1);
      iVar46 = *(int *)(iVar46 + 0x124);
      iVar5 = (*pcVar42)(*puVar51,20000);
      iVar50 = (*pcVar53)(*(undefined4 *)(this + 0x110),1);
      iVar12 = *(int *)(iVar50 + 0x128);
      iVar50 = (*pcVar42)(*puVar51,20000);
      iVar52 = (*pcVar53)(*(undefined4 *)(this + 0x110),1);
      iVar52 = *(int *)(iVar52 + 300);
      uVar59 = VectorSignedToFloat(iVar46 + iVar5 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      uVar61 = VectorSignedToFloat(iVar50 + iVar12 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      iVar46 = (*pcVar42)(*puVar51,20000);
      uVar11 = VectorSignedToFloat(iVar46 + iVar52 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*piVar45 + 0x48))(piVar45,uVar59,uVar61,uVar11);
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x12,0,8,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x12,8,0xc,this);
    goto LAB_000c9412;
  case 0x4e:
    uStack_8c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    local_88 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_84 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar51 = (undefined4 *)((uint)&local_a8 | 4);
    *puVar51 = 0;
    puVar51[1] = uStack_8c;
    puVar51[2] = local_88;
    puVar51[3] = uStack_84;
    local_90 = 0;
    uStack_80 = uVar66;
    uStack_78 = uVar4;
    local_a8 = CONCAT44(local_a8._4_4_,0x3f800000);
    local_98 = CONCAT44(0x3f800000,(undefined4)local_98);
    local_70 = 0x3f800000;
    local_e8 = 0.0;
    fStack_e4 = 0.0;
    local_e0 = 0.0;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(0x16,(Array *)pAVar7);
    iVar5 = 0;
    iVar46 = DAT_000c7ee8 + 0xc7cce;
    for (iVar50 = 0; iVar50 != 0x38; iVar50 = iVar50 + 0x1c) {
      uVar11 = createStaticObject(this,(Waypoint *)0x0,0x381b,true);
      iVar52 = iVar46 + iVar50;
      uVar59 = *(undefined4 *)(iVar52 + 0x14);
      local_60 = *(float *)(iVar52 + 0x18);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5) = uVar11;
      uVar3 = *(ulonglong *)(iVar52 + 4);
      fVar58 = *(float *)(iVar52 + 0xc);
      local_68 = CONCAT44(uVar59,*(undefined4 *)(iVar52 + 0x10));
      AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_110,(Vector *)&local_a8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e8,(Vector *)&local_110);
      AEGeometry::setRotation
                (*(AEGeometry **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5) + 8),
                 (Vector *)&local_e8);
      local_68 = uVar3;
      local_60 = fVar58;
      AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_110,(Vector *)&local_a8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e8,(Vector *)&local_110);
      pfVar34 = (float *)&DAT_000c7eec;
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5);
      if (iVar50 == 0) {
        pfVar34 = (float *)&DAT_000c7ef0;
      }
      local_108 = local_e0;
      local_110 = CONCAT44(fStack_e4,*pfVar34 + local_e8);
      (**(code **)(*piVar45 + 0x44))(piVar45,(Matrix *)&local_110);
      iVar5 = iVar5 + 4;
    }
    puVar51 = *(undefined4 **)(DAT_000c7ef4 + 0xc7d8c);
    puVar19 = *(undefined4 **)(DAT_000c7efc + 0xc7d96);
    pcVar53 = *(code **)(DAT_000c7f00 + 0xc7d9c);
    iVar46 = DAT_000c7ef8;
    for (uVar36 = 2; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      iVar5 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
      uVar11 = createShip(this,8,0,iVar5,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
      iVar5 = (*pcVar53)(*puVar19,2000);
      iVar50 = (*pcVar53)(*puVar19,10000);
      uVar11 = VectorSignedToFloat(iVar5 + iVar46,(byte)(in_fpscr >> 0x16) & 3);
      uVar59 = VectorSignedToFloat(iVar50 + -5000,(byte)(in_fpscr >> 0x16) & 3);
      iVar5 = (*pcVar53)(*puVar19,10000);
      local_108 = (float)VectorSignedToFloat(iVar5 + DAT_000c7f04,(byte)(in_fpscr >> 0x16) & 3);
      local_110 = CONCAT44(uVar59,uVar11);
      (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_110);
      local_110 = 0;
      local_108 = DAT_000c7f08;
      local_68 = 0x3f80000000000000;
      local_60 = 0.0;
      AEGeometry::setDirection
                (*(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 8),
                 (Vector *)&local_110);
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4));
      iVar46 = iVar46 + 2000;
    }
    break;
  case 0x4f:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(7,(Array *)pAVar7);
    puVar51 = *(undefined4 **)(DAT_000c7f0c + 0xc7ec2);
    for (uVar36 = 0; uVar36 < **(uint **)(this + 0xf8); uVar36 = uVar36 + 1) {
      iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,9);
      uVar11 = createShip(this,9,0,iVar46,(Waypoint *)0x0,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
    }
    break;
  case 0x50:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    local_108 = DAT_000c8124;
    piVar45 = *(int **)(DAT_000c8120 + 0xc7f6e);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    local_110 = 0;
    pSVar20 = (Station *)Galaxy::getStation(*piVar45);
    pPVar21 = operator_new(0x174);
    PlayerStation::PlayerStation(pPVar21,pSVar20);
    ArrayAdd<KIPlayer*>((KIPlayer *)pPVar21,*(Array **)(this + 0xf8));
    Station::getName();
    AbyssEngine::String::operator=
              ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),(String *)&local_a8);
    AbyssEngine::String::~String((String *)&local_a8);
    piVar45 = (int *)**(int **)(*(int *)(this + 0xf8) + 4);
    piVar45[10] = 8;
    (**(code **)(*piVar45 + 0x44))(piVar45,&local_110);
    local_a8 = 0;
    local_a0 = (ulonglong)local_a0._4_4_ << 0x20;
    AEGeometry::setRotation
              (*(AEGeometry **)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x140),(Vector *)&local_a8);
    fVar58 = extraout_s0_06;
    fVar60 = extraout_s1_07;
    fVar56 = extraout_s2_06;
    if (pSVar20 != (Station *)0x0) {
      pvVar13 = (void *)Station::~Station(pSVar20);
      operator_delete(pvVar13);
      fVar58 = extraout_s0_07;
      fVar60 = extraout_s1_08;
      fVar56 = extraout_s2_07;
    }
    uStack_78 = DAT_000c7ee0;
    uStack_80 = DAT_000c7ed8;
    uStack_8c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    local_88 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_84 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar51 = (undefined4 *)((uint)&local_a8 | 4);
    *puVar51 = 0;
    puVar51[1] = uStack_8c;
    puVar51[2] = local_88;
    puVar51[3] = uStack_84;
    local_90 = 0;
    local_a8 = CONCAT44(local_a8._4_4_,0x3f800000);
    local_98 = CONCAT44(0x3f800000,(undefined4)local_98);
    local_70 = 0x3f800000;
    AbyssEngine::AEMath::MatrixSetRotation((Matrix *)&local_e8,fVar58,fVar60,fVar56);
    local_e8 = 0.0;
    fStack_e4 = 0.0;
    local_e0 = 0.0;
    iVar46 = Status::getLevel();
    if (iVar46 < 0x15) {
      iVar46 = Status::getLevel();
      iVar46 = iVar46 * 0xf + 0xdc;
    }
    else {
      iVar46 = 0x208;
    }
    uVar36 = 0;
    iVar5 = DAT_000cb8d8 + 0xcb794;
    for (iVar50 = 0; iVar50 != 0x150; iVar50 = iVar50 + 0x1c) {
      if ((0x9f3U >> (uVar36 & 0xff) & 1) == 0) {
        iVar52 = 0x381d;
      }
      else {
        iVar52 = 0x381b;
      }
      pKVar39 = (KIPlayer *)createStaticObject(this,(Waypoint *)0x0,iVar52,true);
      ArrayAdd<KIPlayer*>(pKVar39,*(Array **)(this + 0xf8));
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4 + 4);
      Player::setMaxHitpoints((Player *)piVar45[1],iVar46);
      iVar52 = iVar5 + iVar50;
      fVar58 = *(float *)(iVar52 + 4);
      fVar60 = *(float *)(iVar52 + 8);
      fVar56 = *(float *)(iVar52 + 0xc);
      local_134 = *(float *)(iVar52 + 0x10);
      local_130 = *(float *)(iVar52 + 0x14);
      local_12c = *(float *)(iVar52 + 0x18);
      AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_68,(Vector *)&local_a8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e8,(Vector *)&local_68);
      AEGeometry::setRotation((AEGeometry *)piVar45[2],(Vector *)&local_e8);
      local_134 = fVar58;
      local_130 = fVar60;
      local_12c = fVar56;
      AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_68,(Vector *)&local_a8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e8,(Vector *)&local_68);
      local_60 = local_108 + local_e0;
      local_68 = CONCAT44(local_110._4_4_ + fStack_e4,(float)local_110 + local_e8);
      (**(code **)(*piVar45 + 0x44))(piVar45,(Matrix *)&local_68);
      piVar45[10] = 8;
      Player::setAlwaysEnemy(SUB41(piVar45[1],0));
      uVar36 = uVar36 + 1;
    }
    local_60 = (float)(int)(local_108 * 0.5);
    local_68 = 0;
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_68,3);
    *(Route **)(this + 0x110) = pRVar6;
    puVar51 = *(undefined4 **)(DAT_000cb8dc + 0xcb8be);
    fVar58 = extraout_s0_18;
    fVar60 = extraout_s1_19;
    fVar56 = extraout_s2_18;
    for (iVar46 = 6; iVar46 != 0; iVar46 = iVar46 + -1) {
      iVar5 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
      pWVar8 = (Waypoint *)Route::getWaypoint();
      pKVar39 = (KIPlayer *)createShip(this,8,0,iVar5,pWVar8,true,false);
      Player::setAlwaysEnemy(SUB41(*(undefined4 *)(pKVar39 + 4),0));
      Player::setAlwaysFriend(*(Player **)(pKVar39 + 4),false);
      ArrayAdd<KIPlayer*>(pKVar39,*(Array **)(this + 0xf8));
      fVar58 = extraout_s0_19;
      fVar60 = extraout_s1_20;
      fVar56 = extraout_s2_19;
    }
    for (iVar46 = 3; iVar46 != 0; iVar46 = iVar46 + -1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      pKVar39 = (KIPlayer *)createShip(this,0,0,0x11,pWVar8,true,false);
      Player::setAlwaysFriend(*(Player **)(pKVar39 + 4),true);
      Player::setAlwaysEnemy(SUB41(*(undefined4 *)(pKVar39 + 4),0));
      ArrayAdd<KIPlayer*>(pKVar39,*(Array **)(this + 0xf8));
      fVar58 = extraout_s0_20;
      fVar60 = extraout_s1_21;
      fVar56 = extraout_s2_20;
    }
    PlayerEgo::setPosition(fVar58,fVar60,fVar56);
    local_130 = 0.0;
    local_134 = 1.0;
    local_12c = 1.0;
    local_158 = 0.0;
    local_154 = 0x3f800000;
    local_150 = 0;
    AEGeometry::setDirection(*(Vector **)(*(int *)(this + 0xf0) + 8),(Vector *)&local_134);
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x16,0,this);
    goto LAB_000cb9b8;
  case 0x51:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    piVar45 = *(int **)(DAT_000c812c + 0xc8070);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    pSVar20 = (Station *)Galaxy::getStation(*piVar45);
    pPVar21 = operator_new(0x174);
    PlayerStation::PlayerStation(pPVar21,pSVar20);
    ArrayAdd<KIPlayer*>((KIPlayer *)pPVar21,*(Array **)(this + 0xf8));
    Station::getName();
    AbyssEngine::String::operator=
              ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),(String *)&local_a8);
    AbyssEngine::String::~String((String *)&local_a8);
    *(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x28) = 8;
    if (pSVar20 != (Station *)0x0) {
      pvVar13 = (void *)Station::~Station(pSVar20);
      operator_delete(pvVar13);
    }
    for (iVar46 = 8; iVar46 != 0; iVar46 = iVar46 + -1) {
      pKVar39 = (KIPlayer *)createShip(this,9,0,8,(Waypoint *)0x0,true,false);
      ArrayAdd<KIPlayer*>(pKVar39,*(Array **)(this + 0xf8));
    }
    break;
  case 0x57:
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x16,0,this);
    goto LAB_000cb9b8;
  case 0x59:
    local_a8 = *(ulonglong *)(DAT_000c841c + 0xc8138);
    local_a0 = *(longlong *)(DAT_000c841c + 0xc8140);
    local_98 = *(longlong *)(DAT_000c841c + 0xc8148);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    *(Route **)(this + 0x10c) = pRVar6;
    Route::setLoop(SUB41(pRVar6,0));
    local_e8 = *(float *)(DAT_000c8420 + 0xc816e);
    fStack_e4 = *(float *)(DAT_000c8420 + 0xc8172);
    local_e0 = *(float *)(DAT_000c8420 + 0xc8176);
    local_dc = *(int *)(DAT_000c8420 + 0xc817a);
    local_d8 = *(longlong *)(DAT_000c8420 + 0xc817e);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_e8,6);
    *(Route **)(this + 0x110) = pRVar6;
    Route::setLoop(SUB41(pRVar6,0));
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(0xc,(Array *)pAVar7);
    uVar11 = createStaticObject(this,(Waypoint *)0x0,0x4260,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    PlayerFixedObject::setMoving
              ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
    pPVar40 = operator_new(0x114);
    iVar46 = DAT_000c8424;
    Player::Player(pPVar40,0x1d4c,DAT_000c8424,0,0,0);
    pPVar28 = operator_new(0x1bc);
    PlayerFixedObject::PlayerFixedObject
              (pPVar28,0x495d,0,pPVar40,(AEGeometry *)0x0,extraout_s0_08,extraout_s1_09,
               extraout_s2_08);
    *(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) = pPVar28;
    pAVar22 = operator_new(0xc0);
    puVar51 = *(undefined4 **)(DAT_000c8428 + 0xc822e);
    AEGeometry::AEGeometry(pAVar22,0x5254,(PaintCanvas *)*puVar51,false);
    pAVar23 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar23,0x5574,(PaintCanvas *)*puVar51,false);
    pAVar24 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar24,0x563c,(PaintCanvas *)*puVar51,false);
    AEGeometry::addChild((uint)pAVar22);
    AEGeometry::addChild((uint)pAVar22);
    pvVar13 = (void *)AEGeometry::~AEGeometry(pAVar23);
    operator_delete(pvVar13);
    pvVar13 = (void *)AEGeometry::~AEGeometry(pAVar24);
    operator_delete(pvVar13);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
    (**(code **)(*piVar45 + 8))(piVar45,pAVar22,0xffffffff,0);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
    local_110 = CONCAT44(DAT_000c842c,DAT_000c8430);
    local_108 = DAT_000c8434;
    (**(code **)(*piVar45 + 0x44))(piVar45,&local_110);
    AEGeometry::setRotation
              (*(AEGeometry **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 8),
               extraout_s0_09,extraout_s1_10,extraout_s2_09);
    pPVar40 = operator_new(0x114);
    Player::Player(pPVar40,0x1d4c,iVar46,0,0,0);
    pPVar28 = operator_new(0x1bc);
    PlayerFixedObject::PlayerFixedObject
              (pPVar28,0x495d,0,pPVar40,(AEGeometry *)0x0,extraout_s0_10,extraout_s1_11,
               extraout_s2_10);
    *(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 8) = pPVar28;
    pAVar22 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar22,0x5254,(PaintCanvas *)*puVar51,false);
    pAVar23 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar23,0x5574,(PaintCanvas *)*puVar51,false);
    pAVar24 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar24,0x4990,(PaintCanvas *)*puVar51,false);
    this_01 = operator_new(0xc0);
    AEGeometry::AEGeometry(this_01,0x4991,(PaintCanvas *)*puVar51,false);
    pcVar53 = *(code **)(DAT_000c843c + 0xc837a);
    (*pcVar53)(pAVar22,*(undefined4 *)(pAVar23 + 0xc));
    (*pcVar53)(pAVar22,*(undefined4 *)(pAVar24 + 0xc));
    (*pcVar53)(pAVar22,*(undefined4 *)(this_01 + 0xc));
    pcVar53 = *(code **)(DAT_000c8440 + 0xc8392);
    (*pcVar53)(pAVar23);
    pcVar42 = *(code **)(DAT_000c8444 + 0xc839c);
    (*pcVar42)(pAVar23);
    (*pcVar53)(pAVar24);
    (*pcVar42)();
    (*pcVar53)(this_01);
    (*pcVar42)();
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 8);
    (**(code **)(*piVar45 + 8))(piVar45,pAVar22,0xffffffff,0);
    KIPlayer::setVisible(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + 8),false);
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 8);
    local_110 = CONCAT44(DAT_000c842c,DAT_000c8430);
    local_108 = DAT_000c8434;
    (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_110);
    AEGeometry::setRotation
              (*(AEGeometry **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 8) + 8),
               extraout_s0_11,extraout_s1_12,extraout_s2_11);
    fVar60 = DAT_000c844c;
    fVar58 = DAT_000c8448;
    for (uVar36 = 0; uVar36 != 8; uVar36 = uVar36 + 1) {
      iVar46 = 0x110;
      if (uVar36 < 4) {
        iVar46 = 0x10c;
      }
      bVar2 = 6 < uVar36;
      if (uVar36 < 4) {
        bVar2 = uVar36 < 2;
      }
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + iVar46),(uint)!bVar2);
      iVar46 = Globals::getRandomEnemyFighter
                         ((Globals *)**(undefined4 **)(DAT_000c8708 + 0xc848a),3);
      uVar66 = createShip(this,3,0,iVar46,pWVar8,true,false);
      pKVar39 = (KIPlayer *)uVar66;
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x10c),(void *)((ulonglong)uVar66 >> 0x20),
                                     __flags_12,__arg_13);
      KIPlayer::setRoute(pKVar39,pRVar6);
      (**(code **)(*(int *)pKVar39 + 0x28))((Vector *)&local_110,pKVar39);
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_110);
      fVar56 = *(float *)(this + 400);
      if ((fVar58 < fVar56) && ((int)((uint)(fVar56 < fVar60) << 0x1f) < 0)) {
        (**(code **)(*(int *)pKVar39 + 0x48))
                  (pKVar39,*(undefined4 *)(this + 0x18c),fVar56 + fVar56,
                   *(undefined4 *)(this + 0x194));
      }
      pKVar39[0x13e] = (KIPlayer)0x0;
      *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4 + 0xc) = pKVar39;
    }
    uVar59 = createShip(this,3,1,0xf,(Waypoint *)0x0,true,false);
    uVar11 = DAT_000c870c;
    *(undefined4 *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) = uVar59;
    piVar45 = *(int **)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4);
    (**(code **)(*piVar45 + 0x48))(piVar45,DAT_000c8710,uVar11,DAT_000c8714);
    break;
  case 0x5b:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(1,(Array *)pAVar7);
    pWVar8 = operator_new(0x138);
    Waypoint::Waypoint(pWVar8,DAT_000c8718,0,60000,(Route *)0x0);
    uVar11 = createStaticObject(this,pWVar8,0x494e,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    PlayerFixedObject::setMoving
              ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
    PlayerFixedObject::setDockingType
              ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),0);
    piVar45 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    (**(code **)(*(int *)pWVar8 + 0x28))(&local_a8,pWVar8);
    (**(code **)(*piVar45 + 0x44))(piVar45,&local_a8);
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    iVar46 = *(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4);
    uVar11 = Player::getHitpoints();
    __aeabi_idiv(uVar11,0x14);
    Player::setHitpoints(iVar46);
    local_a8 = (ulonglong)DAT_000c871c << 0x20;
    local_a0 = (ulonglong)local_a0._4_4_ << 0x20;
    AEGeometry::setRotation
              (*(AEGeometry **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8),(Vector *)&local_a8);
    KIPlayer::setActive(SUB41(**(undefined4 **)(*(int *)(this + 0xf8) + 4),0));
    puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
    if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
      pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
      operator_delete(pvVar13);
      puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
    }
    *puVar51 = 0;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,1,0,this);
    goto LAB_000c9412;
  case 0x5c:
    piVar45 = *(int **)(DAT_000c8720 + 0xc8688);
    pcVar53 = *(code **)(DAT_000c8724 + 0xc868c);
    iVar5 = (*pcVar53)(*piVar45);
    iVar46 = DAT_000c872c;
    local_a8 = (ulonglong)(uint)(iVar5 + DAT_000c872c);
    local_a0 = CONCAT44(local_a0._4_4_,DAT_000c872c);
    iVar5 = (*pcVar53)(*piVar45,uVar11);
    local_a0 = CONCAT44(iVar5 + iVar46,(float)local_a0);
    local_98 = (ulonglong)DAT_000c8730 << 0x20;
    local_90 = (*pcVar53)(*piVar45,uVar11);
    local_90 = local_90 + iVar46;
    uStack_8c = 0;
    local_88 = DAT_000c8734;
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,9);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(4,(Array *)pAVar7);
    for (iVar46 = 0; iVar46 != 3; iVar46 = iVar46 + 1) {
      pRVar6 = *(Route **)(this + 0x110);
      iVar5 = *piVar45;
      Route::length();
      iVar5 = AbyssEngine::AERandom::nextInt(iVar5);
      pWVar8 = (Waypoint *)Route::getWaypoint(pRVar6,iVar5);
      uVar11 = createShip(this,10,0,0x2c,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
      PlayerFighter::setAIDisabled
                (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4),true);
      PlayerFighter::setCloakingPossible
                (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4),false);
    }
    pWVar8 = operator_new(0x138);
    Waypoint::Waypoint(pWVar8,DAT_000c8b94,0,DAT_000c8b98,(Route *)0x0);
    uVar11 = createStaticObject(this,pWVar8,0x4299,false);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) = uVar11;
    PlayerFixedObject::setMoving
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc),false);
    Player::setAlwaysFriend
              (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) + 4),true);
    PlayerFixedObject::setDockingType
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc),1);
    uVar11 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc);
    pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c8b9c + 0xc8822));
    AbyssEngine::String::String(aSStack_164,pSVar25,false);
    PlayerFixedObject::setName(uVar11,aSStack_164);
    AbyssEngine::String::~String(aSStack_164);
    puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) + 0x50);
    if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
      pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
      operator_delete(pvVar13);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0xc) + 0x50);
    }
    *puVar51 = 0;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x16,0,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,1,3,this);
    goto LAB_000c9412;
  case 0x5e:
    local_e8 = *(float *)(DAT_000c8ba0 + 0xc88a6);
    fStack_e4 = *(float *)(DAT_000c8ba0 + 0xc88aa);
    local_e0 = *(float *)(DAT_000c8ba0 + 0xc88ae);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_e8,3);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(10,(Array *)pAVar7);
    piVar45 = *(int **)(DAT_000c8ba4 + 0xc88f0);
    for (iVar46 = 0; iVar46 != 6; iVar46 = iVar46 + 1) {
      pRVar6 = *(Route **)(this + 0x110);
      iVar5 = *piVar45;
      Route::length();
      iVar5 = AbyssEngine::AERandom::nextInt(iVar5);
      pWVar8 = (Waypoint *)Route::getWaypoint(pRVar6,iVar5);
      uVar11 = createShip(this,10,0,0x2c,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
    }
    uVar11 = createStaticObject(this,(Waypoint *)0x0,0x495d,false);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18) = uVar11;
    PlayerFixedObject::setMoving
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18),false);
    PlayerFixedObject::setDockingType
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18),2);
    piVar45 = *(int **)(DAT_000c8ba8 + 0xc899a);
    uVar11 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18);
    pSVar25 = (String *)GameText::getText(*piVar45);
    AbyssEngine::String::String(aSStack_170,pSVar25,false);
    PlayerFixedObject::setName(uVar11,aSStack_170);
    AbyssEngine::String::~String(aSStack_170);
    Player::setAlwaysFriend
              (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18) + 4),true);
    local_a8 = (ulonglong)DAT_000c8bac << 0x20;
    local_a0 = (ulonglong)local_a0._4_4_ << 0x20;
    AEGeometry::rotate(*(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18) + 8));
    puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18) + 0x50);
    if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
      pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
      operator_delete(pvVar13);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18) + 0x50);
    }
    *puVar51 = 0;
    pWVar8 = operator_new(0x138);
    Waypoint::Waypoint(pWVar8,30000,DAT_000c8bb0,40000,(Route *)0x0);
    uVar11 = createStaticObject(this,pWVar8,0x4299,false);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c) = uVar11;
    PlayerFixedObject::setMoving
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c),false);
    PlayerFixedObject::setDockingType
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c),1);
    uVar11 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c);
    pSVar25 = (String *)GameText::getText(*piVar45);
    AbyssEngine::String::String(aSStack_17c,pSVar25,false);
    PlayerFixedObject::setName(uVar11,aSStack_17c);
    AbyssEngine::String::~String(aSStack_17c);
    Player::setAlwaysFriend
              (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c) + 4),true);
    puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c) + 0x50);
    if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
      pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
      operator_delete(pvVar13);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c) + 0x50);
    }
    iVar46 = DAT_000c8bb4;
    *puVar51 = 0;
    __aeabi_memcpy(&local_a8,iVar46 + 0xc8ad6,0x24);
    local_110 = *(ulonglong *)(DAT_000c8bb8 + 0xc8ae0);
    local_108 = *(float *)(DAT_000c8bb8 + 0xc8ae8);
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    ArraySetLength<KIPlayer*>(3,(Array *)pAVar7);
    **(undefined4 **)(pAVar7 + 4) = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x18);
    *(undefined4 *)(*(int *)(pAVar7 + 4) + 8) =
         *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x1c);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,(Array *)pAVar7,(int *)&local_110,9);
    Route::setLoop(SUB41(pRVar6,0));
    pRVar29 = (Route *)Route::clone((__fn *)pRVar6,__child_stack_02,__flags_13,__arg_14);
    puVar51 = *(undefined4 **)(DAT_000c8bbc + 0xc8b4e);
    for (iVar46 = 0x20; iVar46 != 0x28; iVar46 = iVar46 + 4) {
      iVar5 = Globals::getRandomEnemyFighter((Globals *)*puVar51,3);
      if (iVar46 == 0x20) {
        pWVar8 = (Waypoint *)Route::getWaypoint(pRVar6,0);
        uVar11 = createShip(this,3,0,iVar5,pWVar8,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20) = uVar11;
        pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20);
        pRVar16 = pRVar6;
      }
      else {
        pWVar8 = (Waypoint *)Route::getWaypoint(pRVar29,0);
        uVar11 = createShip(this,3,0,iVar5,pWVar8,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46) = uVar11;
        pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46);
        pRVar16 = pRVar29;
      }
      KIPlayer::setRoute(pKVar39,pRVar16);
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46) + 4),true);
      Player::setNeverAttack
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46) + 4),true);
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,1,7,this);
    goto LAB_000c9412;
  case 0x61:
    local_a8 = *(ulonglong *)(DAT_000c8f48 + 0xc8c38);
    local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000c8f48 + 0xc8c40));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    uVar36 = 8;
    pcVar26 = *(char **)(DAT_000c8f4c + 0xc8c5e);
    *(Route **)(this + 0x110) = pRVar6;
    cVar1 = *pcVar26;
    if (cVar1 == '\0') {
      uVar36 = 0xc;
    }
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    uVar27 = 0xb;
    if (cVar1 == '\0') {
      uVar27 = 0x13;
    }
    ArraySetLength<KIPlayer*>(uVar27,(Array *)pAVar7);
    puVar51 = *(undefined4 **)(DAT_000c8f50 + 0xc8ca2);
    for (uVar43 = 0; uVar36 != uVar43; uVar43 = uVar43 + 1) {
      iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
      uVar11 = createShip(this,8,0,iVar46,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4) + 4),0));
    }
    uVar44 = 0xb;
    uVar43 = uVar36;
    if (cVar1 == '\0') {
      uVar44 = 0x10;
    }
    for (; uVar43 < uVar44; uVar43 = uVar43 + 1) {
      iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,2);
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
      uVar11 = createShip(this,2,0,iVar46,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4) = uVar11;
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4) + 4),true);
    }
    for (; uVar44 < uVar27; uVar44 = uVar44 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
      uVar11 = createShip(this,2,1,0xf,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar44 * 4) = uVar11;
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar44 * 4),false);
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar44 * 4) + 4),true);
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x12,0,uVar36,this);
    *(Objective **)(this + 0x28) = pOVar14;
    break;
  case 100:
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(2,(Array *)pAVar7);
    PlayerEgo::getPosition();
    AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_a8);
    puVar51 = *(undefined4 **)(DAT_000c8f54 + 0xc8e14);
    pcVar53 = *(code **)(DAT_000c8f58 + 0xc8e16);
    iVar46 = (*pcVar53)(*puVar51,50000);
    iVar5 = (*pcVar53)(*puVar51,2);
    iVar50 = -1;
    if (iVar5 == 0) {
      iVar50 = 1;
    }
    iVar52 = -1;
    fVar58 = (float)VectorSignedToFloat(iVar50 * iVar46 + 20000,(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(this + 0x18c) = *(float *)(this + 0x18c) + fVar58;
    iVar46 = (*pcVar53)(*puVar51,50000);
    iVar5 = (*pcVar53)(*puVar51,2);
    iVar50 = -1;
    if (iVar5 == 0) {
      iVar50 = 1;
    }
    fVar58 = (float)VectorSignedToFloat(iVar50 * iVar46 + 10000,(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(this + 400) = *(float *)(this + 400) + fVar58;
    iVar46 = (*pcVar53)(*puVar51,50000);
    iVar5 = (*pcVar53)(*puVar51,2);
    if (iVar5 == 0) {
      iVar52 = 1;
    }
    fVar58 = (float)VectorSignedToFloat(iVar52 * iVar46 + 20000,(byte)(in_fpscr >> 0x16) & 3);
    fVar60 = *(float *)(this + 0x194);
    *(float *)(this + 0x194) = fVar60 + fVar58;
    local_a0 = CONCAT44(local_a0._4_4_,(int)(fVar60 + fVar58));
    local_a8 = CONCAT44((int)*(float *)(this + 400),(int)*(float *)(this + 0x18c));
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    for (iVar46 = 0; iVar46 != 2; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint();
      uVar11 = createShip(this,10,0,0x2c,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,4,1,this);
    goto LAB_000cb9b8;
  case 0x66:
    puVar51 = *(undefined4 **)(DAT_000c9338 + 0xc8f64);
    pcVar53 = *(code **)(DAT_000c933c + 0xc8f68);
    iVar5 = (*pcVar53)(*puVar51);
    iVar46 = DAT_000c9344;
    local_a8 = (ulonglong)(uint)(iVar5 + DAT_000c9344);
    local_a0 = CONCAT44(local_a0._4_4_,DAT_000c9344);
    iVar5 = (*pcVar53)(*puVar51,uVar59);
    local_a0 = CONCAT44(iVar5 + iVar46,(float)local_a0);
    local_98 = (ulonglong)DAT_000c9348 << 0x20;
    local_90 = (*pcVar53)(*puVar51,uVar59);
    local_90 = local_90 + iVar46;
    uStack_8c = 0;
    local_88 = DAT_000c934c;
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,9);
    *(Route **)(this + 0x110) = pRVar6;
    iVar5 = (*pcVar53)(*puVar51,uVar59);
    local_e8 = (float)(iVar5 + iVar46);
    fStack_e4 = 0.0;
    local_e0 = DAT_000c9350;
    local_dc = (*pcVar53)(*puVar51,uVar59);
    local_dc = local_dc + iVar46;
    local_d8 = (ulonglong)DAT_000c9354 << 0x20;
    local_d0 = (*pcVar53)(*puVar51,uVar59);
    local_d0 = local_d0 + iVar46;
    uStack_cc = 0;
    iStack_c8 = iVar46;
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_e8,9);
    *(Route **)(this + 0x10c) = pRVar6;
    pWVar8 = operator_new(0x138);
    Waypoint::Waypoint(pWVar8,DAT_000c9358,1000,DAT_000c935c,(Route *)0x0);
    pWVar9 = operator_new(0x138);
    Waypoint::Waypoint(pWVar9,20000,10000,10000,(Route *)0x0);
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(10,(Array *)pAVar7);
    uVar11 = createStaticObject(this,pWVar8,0x4974,true);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    piVar45 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    (**(code **)(*(int *)pWVar8 + 0x28))(&local_110,pWVar8);
    (**(code **)(*piVar45 + 0x44))(piVar45,&local_110);
    PlayerFixedObject::setMoving
              ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
    pPVar28 = (PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    pPVar28[0x70] = (PlayerFixedObject)0x0;
    PlayerFixedObject::setDockingType(pPVar28,1);
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
    puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
    *(undefined1 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x74) = 1;
    if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
      pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
      operator_delete(pvVar13);
      puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
    }
    *puVar51 = 0;
    uVar11 = createStaticObject(this,pWVar8,0x5279,true);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) = uVar11;
    piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
    (**(code **)(*piVar45 + 0x48))(piVar45,0,0,0);
    PlayerFixedObject::setMoving
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),false);
    PlayerFixedObject::setDockingType
              (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),0);
    Player::setAlwaysFriend
              (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 4),true);
    Player::setHitpoints(*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 4));
    local_110 = (ulonglong)DAT_000c9364 << 0x20;
    local_108 = 0.0;
    AEGeometry::rotate(*(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 8));
    KIPlayer::setActive(SUB41(*(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),0));
    puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 0x50);
    if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
      pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
      operator_delete(pvVar13);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 0x50);
    }
    iVar46 = DAT_000c9368;
    iVar5 = 25000;
    *puVar51 = 0;
    for (iVar50 = 8; iVar50 != 0x18; iVar50 = iVar50 + 4) {
      uVar11 = createShip(this,0,0,0x33,pWVar9,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar50) = uVar11;
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar50);
      if (iVar50 == 8) {
        (**(code **)(*(int *)pWVar8 + 0x28))((Vector *)&local_68);
        local_130 = DAT_000c936c;
        local_134 = DAT_000c9370;
        local_12c = DAT_000c9374;
      }
      else {
        (**(code **)(*(int *)pWVar8 + 0x28))((Vector *)&local_68);
        local_134 = (float)VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
        local_12c = (float)VectorSignedToFloat(iVar46,(byte)(in_fpscr >> 0x16) & 3);
        local_130 = DAT_000c9378;
      }
      AbyssEngine::AEMath::operator+((Vector *)&local_110,(Vector *)&local_68);
      (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_110);
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar50) + 4),true);
      iVar46 = iVar46 + -5000;
      iVar5 = iVar5 + 5000;
      *(undefined2 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar50) + 0x13d) = 0;
    }
    __aeabi_memcpy(&local_110,DAT_000c937c + 0xc9292,0x24);
    local_68 = *(ulonglong *)(DAT_000c9380 + 0xc929a);
    local_60 = *(float *)(DAT_000c9380 + 0xc92a2);
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    ArraySetLength<KIPlayer*>(3,(Array *)pAVar7);
    **(undefined4 **)(pAVar7 + 4) = **(undefined4 **)(*(int *)(this + 0xf8) + 4);
    *(undefined4 *)(*(int *)(pAVar7 + 4) + 8) =
         *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_110,(Array *)pAVar7,(int *)&local_68,9);
    Route::setLoop(SUB41(pRVar6,0));
    pvVar13 = extraout_r1;
    iVar46 = extraout_r2;
    __arg_15 = extraout_r3;
    for (iVar5 = 2; fVar58 = DAT_000c9384, iVar5 != 6; iVar5 = iVar5 + 1) {
      pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4);
      pRVar29 = pRVar6;
      if (iVar5 != 2) {
        pRVar29 = (Route *)Route::clone((__fn *)pRVar6,pvVar13,iVar46,__arg_15);
      }
      KIPlayer::setRoute(pKVar39,pRVar29);
      pvVar13 = extraout_r1_00;
      iVar46 = extraout_r2_00;
      __arg_15 = extraout_r3_00;
    }
    for (iVar46 = 6; iVar46 != 10; iVar46 = iVar46 + 1) {
      uVar11 = createShip(this,10,0,0x2c,pWVar9,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      local_134 = fVar58;
      local_130 = fVar58;
      local_12c = fVar58;
      (**(code **)(*piVar45 + 0x44))(piVar45,&local_134);
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
    }
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,4,8,this);
    *(Objective **)(this + 0x28) = pOVar14;
    pOVar14 = operator_new(0x1c);
    Objective::Objective(pOVar14,0x12,2,6,this);
LAB_000c9412:
    *(Objective **)(this + 0x2c) = pOVar14;
    break;
  case 0x69:
    local_a8 = *(ulonglong *)(DAT_000c978c + 0xc9422);
    local_a0 = *(longlong *)(DAT_000c978c + 0xc942a);
    local_98 = *(longlong *)(DAT_000c978c + 0xc9432);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,3);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(5,(Array *)pAVar7);
    for (iVar46 = 0; iVar46 != 2; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
      uVar11 = createShip(this,0,0,0x25,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),true);
    }
    for (iVar46 = 2; iVar46 != 5; iVar46 = iVar46 + 1) {
      pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
      uVar11 = createShip(this,10,0,0x2c,pWVar8,true,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
      Player::setAlwaysEnemy
                (SUB41(*(undefined4 *)
                        (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
    }
    StarSystem::getLightDirection();
    AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_e8,(Vector *)&local_110);
    local_108 = (float)(int)(local_e0 * DAT_000c9790);
    local_110 = (ulonglong)(uint)(int)(local_e8 * DAT_000c9790);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_110,3);
    local_68 = 0x3f80000000000000;
    local_60 = 0.0;
    AEGeometry::setDirection(*(Vector **)(*(int *)(this + 0xf0) + 8),(Vector *)&local_e8);
    PlayerEgo::setRoute(*(Route **)(this + 0xf0));
    setPlayerRoute((Route *)this);
    for (iVar46 = 0; iVar46 != 2; iVar46 = iVar46 + 1) {
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      PlayerEgo::getPosition();
      fVar58 = (float)AEGeometry::getRightVector();
      AbyssEngine::AEMath::operator*((Vector *)&local_188,fVar58);
      AbyssEngine::AEMath::operator+((Vector *)&local_134,(Vector *)&local_158);
      fVar58 = (float)AEGeometry::getDirection();
      AbyssEngine::AEMath::operator*(aVStack_1a0,fVar58);
      AbyssEngine::AEMath::operator+((Vector *)&local_68,(Vector *)&local_134);
      (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_68);
      local_68 = 0x3f80000000000000;
      local_60 = 0.0;
      AEGeometry::setDirection
                (*(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 8),
                 (Vector *)&local_e8);
      pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      pRVar29 = (Route *)Route::clone((__fn *)pRVar6,__child_stack_03,__flags_14,__arg_16);
      KIPlayer::setRoute(pKVar39,pRVar29);
      KIPlayer::setEnemies(*(Array **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
      pPVar30 = *(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
      pPVar30[0x13d] = (PlayerFighter)0x0;
      PlayerFighter::setAIDisabled(pPVar30,true);
    }
    break;
  case 0x6a:
    local_a8 = *(ulonglong *)(DAT_000c97a0 + 0xc9678);
    local_a0 = *(longlong *)(DAT_000c97a0 + 0xc9680);
    local_98 = *(longlong *)(DAT_000c97a0 + 0xc9688);
    pRVar6 = operator_new(0x18);
    Route::Route(pRVar6,(int *)&local_a8,6);
    *(Route **)(this + 0x110) = pRVar6;
    pAVar7 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar7);
    *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
    ArraySetLength<KIPlayer*>(1,(Array *)pAVar7);
    pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
    uVar11 = createShip(this,10,0,0x2c,pWVar8,true,false);
    **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
    piVar37 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    piVar45 = (int *)Route::getWaypoint(*(Route **)(this + 0x110),0);
    (**(code **)(*piVar45 + 0x28))(&local_e8,piVar45);
    (**(code **)(*piVar37 + 0x44))(piVar37,&local_e8);
    local_e8 = DAT_000c97a4;
    fStack_e4 = 0.0;
    local_e0 = 0.0;
    local_110 = 0x3f80000000000000;
    local_108 = 0.0;
    AEGeometry::setDirection
              (*(Vector **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8),(Vector *)&local_e8);
    pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
    pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),
                                   *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_15,__arg_17);
    KIPlayer::setRoute(pKVar39,pRVar6);
    Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
    Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
    Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
    PlayerFighter::setCloakingPossible
              ((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
    break;
  default:
    switch(iVar5) {
    case 0x83:
      pOVar14 = operator_new(0x1c);
      Objective::Objective(pOVar14,4,0,this);
      break;
    case 0x84:
    case 0x85:
    case 0x86:
    case 0x88:
    case 0x8a:
    case 0x8c:
    case 0x8d:
    case 0x8f:
    case 0x92:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:
    case 0x98:
    case 0x99:
    case 0x9c:
      goto switchD_000c4bf8_caseD_2;
    case 0x87:
      local_a8 = *(ulonglong *)(DAT_000c9940 + 0xc97ae);
      local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000c9940 + 0xc97b6));
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_a8,3);
      *(Route **)(this + 0x110) = pRVar6;
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
      ArraySetLength<KIPlayer*>(3,(Array *)pAVar7);
      uVar11 = createStaticObject(this,(Waypoint *)0x0,0x4a88,true);
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
      (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
                ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),0,0,0);
      PlayerFixedObject::setMoving
                ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
      uVar11 = **(undefined4 **)(*(int *)(this + 0xf8) + 4);
      pSVar25 = (String *)GameText::getText(**(int **)(DAT_000c9944 + 0xc982a));
      AbyssEngine::String::String(aSStack_1b8,pSVar25,false);
      PlayerFixedObject::setName(uVar11,aSStack_1b8);
      AbyssEngine::String::~String(aSStack_1b8);
      PlayerFixedObject::setDockingType
                ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),1);
      Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
      puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
      }
      *puVar51 = 0;
      puVar51 = *(undefined4 **)(DAT_000c9948 + 0xc98ae);
      for (iVar46 = 1; iVar46 != 3; iVar46 = iVar46 + 1) {
        iVar5 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
        pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
        uVar11 = createShip(this,8,0,iVar5,pWVar8,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
        KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
      }
      for (uVar36 = 0; uVar36 < **(uint **)(this + 0xfc); uVar36 = uVar36 + 1) {
        PlayerAsteroid::setAsteroidIndex
                  (*(PlayerAsteroid **)((*(uint **)(this + 0xfc))[1] + uVar36 * 4),0x9b);
      }
      goto switchD_000c4bf8_caseD_2;
    case 0x89:
      pOVar14 = operator_new(0x1c);
      Objective::Objective(pOVar14,4,4,this);
      break;
    case 0x8b:
      pfVar34 = (float *)(DAT_000c9d40 + 0xc995a);
      *(undefined4 *)(*piVar45 + 0x178) = 0;
      local_e8 = *pfVar34;
      fStack_e4 = *(float *)(iVar46 + 0xc995e);
      local_e0 = *(float *)(iVar46 + 0xc9962);
      local_dc = *(int *)(iVar46 + 0xc9966);
      local_d8 = *(longlong *)(iVar46 + 0xc996a);
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_e8,6);
      *(Route **)(this + 0x110) = pRVar6;
      Route::setLoop(SUB41(pRVar6,0));
      local_110 = *(ulonglong *)(DAT_000c9d44 + 0xc9994);
      local_108 = *(float *)(DAT_000c9d44 + 0xc999c);
      uStack_104 = *(undefined4 *)(DAT_000c9d44 + 0xc99a0);
      local_100 = *(undefined8 *)(DAT_000c9d44 + 0xc99a4);
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_110,6);
      *(Route **)(this + 0x10c) = pRVar6;
      Route::setLoop(SUB41(pRVar6,0));
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
      ArraySetLength<KIPlayer*>(0x16,(Array *)pAVar7);
      pWVar8 = operator_new(0x138);
      iVar46 = DAT_000c9d48;
      Waypoint::Waypoint(pWVar8,DAT_000c9d4c,DAT_000c9d48,DAT_000c9d50,(Route *)0x0);
      pWVar9 = operator_new(0x138);
      Waypoint::Waypoint(pWVar9,DAT_000c9d54,iVar46,30000,(Route *)0x0);
      uVar11 = createStaticObject(this,pWVar8,0x4a6b,true);
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
      piVar45 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
      (**(code **)(*(int *)pWVar8 + 0x28))(&local_a8,pWVar8);
      (**(code **)(*piVar45 + 0x44))(piVar45,&local_a8);
      PlayerFixedObject::setMoving
                ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
      PlayerFixedObject::setDockingType
                ((PlayerFixedObject *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),3);
      Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
      piVar45 = *(int **)(DAT_000c9d5c + 0xc9a7e);
      uVar11 = **(undefined4 **)(*(int *)(this + 0xf8) + 4);
      pSVar25 = (String *)GameText::getText(*piVar45);
      AbyssEngine::String::String((String *)&local_a8,(char *)(DAT_000c9d60 + 0xc9a9e),false);
      AbyssEngine::operator+(aSStack_1c4,pSVar25);
      PlayerFixedObject::setName(uVar11,aSStack_1c4);
      AbyssEngine::String::~String(aSStack_1c4);
      AbyssEngine::String::~String((String *)&local_a8);
      puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
      }
      *puVar51 = 0;
      uVar11 = createStaticObject(this,pWVar9,0x4a6b,true);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) = uVar11;
      piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
      (**(code **)(*(int *)pWVar9 + 0x28))(&local_a8,pWVar9);
      (**(code **)(*piVar37 + 0x44))(piVar37,&local_a8);
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),false);
      PlayerFixedObject::setDockingType
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),3);
      Player::setHitpoints(*(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 4));
      uVar11 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
      pSVar25 = (String *)GameText::getText(*piVar45);
      AbyssEngine::String::String((String *)&local_a8,(char *)(DAT_000c9d64 + 0xc9b74),false);
      AbyssEngine::operator+(aSStack_1d0,pSVar25);
      PlayerFixedObject::setName(uVar11,aSStack_1d0);
      AbyssEngine::String::~String(aSStack_1d0);
      AbyssEngine::String::~String((String *)&local_a8);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 0x50);
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 0x50);
      }
      uStack_78 = DAT_000c9d70;
      uStack_80 = DAT_000c9d68;
      uStack_8c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      local_88 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uStack_84 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar19 = (undefined4 *)((uint)&local_a8 | 4);
      *puVar51 = 0;
      *puVar19 = 0;
      puVar19[1] = uStack_8c;
      puVar19[2] = local_88;
      puVar19[3] = uStack_84;
      local_90 = 0;
      local_a8 = CONCAT44(local_a8._4_4_,0x3f800000);
      local_98 = CONCAT44(0x3f800000,(undefined4)local_98);
      local_70 = 0x3f800000;
      local_68 = 0;
      local_60 = 0.0;
      iVar5 = DAT_000c9d78 + 0xc9c08;
      iVar46 = 0;
      for (uVar36 = 2; uVar36 != 0xc; uVar36 = uVar36 + 1) {
        uVar11 = createStaticObject(this,(Waypoint *)0x0,0x1a76,true);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
        this_02 = *(PlayerTurret **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
        Player::setMaxHitpoints(*(Player **)(this_02 + 4),1000);
        iVar50 = iVar5 + iVar46 * 0x18;
        uVar59 = *(undefined4 *)(iVar50 + 4);
        uVar11 = *(undefined4 *)(iVar50 + 8);
        local_158 = *(float *)(iVar50 + 0xc);
        local_154 = *(undefined4 *)(iVar50 + 0x10);
        local_150 = *(undefined4 *)(iVar50 + 0x14);
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_134,(Vector *)&local_a8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_68,(Vector *)&local_134);
        AEGeometry::setRotation(*(AEGeometry **)(this_02 + 8),(Vector *)&local_68);
        local_158 = *(float *)(iVar5 + iVar46 * 0x18);
        local_154 = uVar59;
        local_150 = uVar11;
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_134,(Vector *)&local_a8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_68,(Vector *)&local_134);
        pWVar10 = pWVar8;
        if (6 < uVar36) {
          pWVar10 = pWVar9;
        }
        (**(code **)(*(int *)pWVar10 + 0x28))(&local_158,pWVar10);
        fVar60 = local_158;
        fVar56 = (float)local_68;
        (**(code **)(*(int *)pWVar10 + 0x28))(&local_188,pWVar10);
        fVar58 = local_184;
        fVar62 = local_68._4_4_;
        (**(code **)(*(int *)pWVar10 + 0x28))(&local_194,pWVar10);
        local_134 = fVar60 + fVar56;
        local_130 = fVar58 + fVar62;
        local_12c = local_18c + local_60;
        fVar58 = (float)(**(code **)(*(int *)this_02 + 0x44))(this_02,(Matrix *)&local_134);
        PlayerTurret::setScaling(this_02,fVar58);
        *(undefined4 *)(this_02 + 0x50) = 0;
        *(undefined4 *)(this_02 + 0x28) = 1;
        iVar50 = iVar46 + 1;
        if (iVar50 == 5) {
          iVar50 = iVar46 + -4;
        }
        iVar46 = iVar50;
      }
      puVar51 = *(undefined4 **)(DAT_000c9d80 + 0xc9d38);
      for (uVar36 = 0xc; uVar36 != 0x16; uVar36 = uVar36 + 1) {
        iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,1);
        if (uVar36 < 0x11) {
          uVar11 = createShip(this,1,0,iVar46,pWVar8,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
          iVar5 = *(int *)(this + 0xf8);
          __fn = *(__fn **)(this + 0x110);
          iVar46 = extraout_r2_01;
          pvVar13 = extraout_r3_01;
        }
        else {
          uVar11 = createShip(this,1,0,iVar46,pWVar9,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
          iVar5 = *(int *)(this + 0xf8);
          __fn = *(__fn **)(this + 0x10c);
          iVar46 = extraout_r2_02;
          pvVar13 = extraout_r3_02;
        }
        pKVar39 = *(KIPlayer **)((int)*(void **)(iVar5 + 4) + uVar36 * 4);
        pRVar6 = (Route *)Route::clone(__fn,*(void **)(iVar5 + 4),iVar46,pvVar13);
        KIPlayer::setRoute(pKVar39,pRVar6);
      }
      goto switchD_000c4bf8_caseD_2;
    case 0x8e:
      local_a8 = *(ulonglong *)(DAT_000ca1e0 + 0xc9e00);
      local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000ca1e0 + 0xc9e08));
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_a8,3);
      *(Route **)(this + 0x108) = pRVar6;
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
      ArraySetLength<KIPlayer*>(1,(Array *)pAVar7);
      uVar11 = createShip(this,3,0,0x1e,(Waypoint *)0x0,true,false);
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
      Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
      pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x108),
                                     *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_16,__arg_18
                                    );
      KIPlayer::setRoute(pKVar39,pRVar6);
      Player::setHitpoints(*(int *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4));
      pSVar25 = (String *)GameText::getText(**(int **)(DAT_000ca1e8 + 0xc9e9e));
      AbyssEngine::String::operator=
                ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25);
      PlayerFighter::setBoostProb((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),0);
      piVar45 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
      PlayerEgo::getPosition();
      fVar58 = (float)AEGeometry::getRightVector();
      AbyssEngine::AEMath::operator*((Vector *)&local_134,fVar58);
      AbyssEngine::AEMath::operator+((Vector *)&local_110,(Vector *)&local_68);
      fVar58 = (float)AEGeometry::getDirection();
      AbyssEngine::AEMath::operator*((Vector *)&local_188,fVar58);
      AbyssEngine::AEMath::operator+((Vector *)&local_e8,(Vector *)&local_110);
      (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_e8);
      pVVar38 = *(Vector **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8);
      PlayerEgo::GetDirVector();
      local_110 = 0x3f80000000000000;
      local_108 = 0.0;
      AEGeometry::setDirection(pVVar38,(Vector *)&local_e8);
      pOVar14 = operator_new(0x1c);
      Objective::Objective(pOVar14,4,4,this);
      break;
    case 0x90:
      local_e8 = *(float *)(DAT_000ca1f4 + 0xc9f7e);
      fStack_e4 = *(float *)(DAT_000ca1f4 + 0xc9f82);
      local_e0 = *(float *)(DAT_000ca1f4 + 0xc9f86);
      local_dc = *(int *)(DAT_000ca1f4 + 0xc9f8a);
      local_d8 = *(longlong *)(DAT_000ca1f4 + 0xc9f8e);
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_e8,6);
      *(Route **)(this + 0x110) = pRVar6;
      pWVar8 = (Waypoint *)Route::getWaypoint(pRVar6,0);
      Waypoint::reached(pWVar8);
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
      ArraySetLength<KIPlayer*>(0xd,(Array *)pAVar7);
      pcVar53 = *(code **)(DAT_000ca1f8 + 0xc9fda);
      piVar45 = (int *)(*pcVar53)(*(undefined4 *)(this + 0x110),1);
      (**(code **)(*piVar45 + 0x28))((Vector *)&local_a8,piVar45);
      piVar45 = (int *)(*pcVar53)(*(undefined4 *)(this + 0x110),0);
      (**(code **)(*piVar45 + 0x28))(&local_68,piVar45);
      AbyssEngine::AEMath::operator-((Vector *)&local_110,(Vector *)&local_a8);
      local_110 = (ulonglong)(uint)(float)local_110;
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_a8,(Vector *)&local_110);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_110,(Vector *)&local_a8);
      piVar45 = (int *)(*pcVar53)(*(undefined4 *)(this + 0x110),0);
      (**(code **)(*piVar45 + 0x28))(&local_68,piVar45);
      uVar11 = createShip(this,2,0,0x31,(Waypoint *)0x0,true,false);
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
      piVar37 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
      piVar45 = (int *)(*pcVar53)(*(undefined4 *)(this + 0x110),0);
      (**(code **)(*piVar45 + 0x28))(&local_a8,piVar45);
      (**(code **)(*piVar37 + 0x44))(piVar37,&local_a8);
      Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
      pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
      pRVar6 = (Route *)Route::getExactClone();
      KIPlayer::setRoute(pKVar39,pRVar6);
      pSVar25 = (String *)GameText::getText(**(int **)(DAT_000ca1fc + 0xca0ac));
      AbyssEngine::String::operator=
                ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25);
      local_a8 = 0x3f80000000000000;
      local_a0 = (ulonglong)local_a0._4_4_ << 0x20;
      AEGeometry::setDirection
                (*(Vector **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8),(Vector *)&local_110);
      PlayerFighter::setAIDisabled
                ((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),true);
      PlayerFighter::setCloakingPossible
                ((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
      uVar11 = AEGeometry::getMatrix();
      __aeabi_memcpy(&local_a8,uVar11,0x3c);
      pVVar38 = (Vector *)(this + 0x18c);
      AbyssEngine::AEMath::Vector::operator=(pVVar38,(Vector *)&local_68);
      local_1f0 = DAT_000ca200 + 500;
      local_1ec = 8000;
      iVar46 = DAT_000ca208;
      iVar5 = DAT_000ca204;
      for (uVar36 = 1; uVar36 != 0xd; uVar36 = uVar36 + 1) {
        uVar11 = createShip(this,10,0,0x2c,(Waypoint *)0x0,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
        local_134 = 0.0;
        local_130 = 1.0;
        local_12c = 0.0;
        AEGeometry::setDirection
                  (*(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 8),
                   (Vector *)&local_110);
        PlayerFighter::setAIDisabled
                  (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4),true);
        PlayerFighter::setCloakingPossible
                  (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4),false);
        pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
        pRVar6 = (Route *)Route::getExactClone();
        KIPlayer::setRoute(pKVar39,pRVar6);
        iVar50 = iVar46;
        if ((uVar36 < 3) || (iVar50 = local_1ec, uVar36 < 5)) {
          local_134 = (float)VectorSignedToFloat(iVar50,(byte)(in_fpscr >> 0x16) & 3);
          local_130 = DAT_000ca450;
          local_12c = DAT_000ca454;
        }
        else if (uVar36 < 10) {
          pfVar34 = (float *)&DAT_000ca460;
          local_134 = (float)VectorSignedToFloat(local_1f0,(byte)(in_fpscr >> 0x16) & 3);
          if (uVar36 == 7) {
            pfVar34 = (float *)&DAT_000ca464;
          }
          local_130 = *pfVar34;
          local_12c = DAT_000ca468;
        }
        else {
          local_130 = DAT_000ca46c;
          local_134 = (float)VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
          local_12c = DAT_000ca468;
        }
        AbyssEngine::AEMath::Vector::operator=(pVVar38,(Vector *)&local_134);
        puVar51 = *(undefined4 **)(DAT_000ca458 + 0xca238);
        pcVar53 = *(code **)(DAT_000ca45c + 0xca23a);
        iVar50 = (*pcVar53)(*puVar51,0x3c);
        fVar58 = (float)VectorSignedToFloat(iVar50 + -0x1e,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(this + 0x18c) = *(float *)(this + 0x18c) + fVar58;
        iVar50 = (*pcVar53)(*puVar51,0x3c);
        fVar58 = (float)VectorSignedToFloat(iVar50 + -0x1e,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(this + 400) = *(float *)(this + 400) + fVar58;
        iVar50 = (*pcVar53)(*puVar51,0x3c);
        fVar58 = (float)VectorSignedToFloat(iVar50 + -0x1e,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(this + 0x194) = *(float *)(this + 0x194) + fVar58;
        AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_134,(Vector *)&local_a8);
        AbyssEngine::AEMath::Vector::operator=(pVVar38,(Vector *)&local_134);
        piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4);
        (**(code **)(*piVar45 + 0x44))(piVar45,pVVar38);
        iVar46 = iVar46 + 0x5dc;
        iVar5 = iVar5 + 0x1964;
        local_1f0 = local_1f0 + 0xcb2;
        local_1ec = local_1ec + -0x5dc;
      }
      goto switchD_000c4bf8_caseD_2;
    case 0x91:
      local_a8 = *(ulonglong *)(DAT_000ca470 + 0xca318);
      local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000ca470 + 0xca320));
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_a8,3);
      *(Route **)(this + 0x110) = pRVar6;
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
      ArraySetLength<KIPlayer*>(0xe,(Array *)pAVar7);
      uVar11 = createShip(this,2,0,0x31,(Waypoint *)0x0,true,false);
      local_e0 = DAT_000ca478;
      local_e8 = DAT_000ca474;
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
      fStack_e4 = 0.0;
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_e8);
      (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x44))
                ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),(Vector *)(this + 0x18c));
      Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
      piVar45 = *(int **)(DAT_000ca47c + 0xca3b4);
      pSVar25 = (String *)GameText::getText(*piVar45);
      AbyssEngine::String::operator=
                ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25);
      pKVar39 = (KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
      pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),
                                     *(undefined4 **)(*(int *)(this + 0xf8) + 4),__flags_17,__arg_19
                                    );
      KIPlayer::setRoute(pKVar39,pRVar6);
      uVar36 = DAT_000ca484;
      local_60 = DAT_000ca480;
      piVar37 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
      pVVar38 = (Vector *)piVar37[2];
      *(undefined1 *)((int)piVar37 + 0x13d) = 0;
      local_68 = (ulonglong)uVar36;
      (**(code **)(*piVar37 + 0x28))(&local_134);
      AbyssEngine::AEMath::operator-((Vector *)&local_110,(Vector *)&local_68);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_e8,(Vector *)&local_110);
      local_158 = 0.0;
      local_154 = 0x3f800000;
      local_150 = 0;
      AEGeometry::setDirection(pVVar38,(Vector *)&local_e8);
      PlayerFighter::setAIDisabled
                ((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),true);
      iVar50 = 23000;
      iVar46 = DAT_000ca488;
      for (iVar5 = 1; iVar46 = iVar46 + 2000, iVar5 != 0xd; iVar5 = iVar5 + 1) {
        pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
        uVar11 = createShip(this,10,0,0x2c,pWVar8,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) = uVar11;
        puVar51 = *(undefined4 **)(DAT_000ca6c8 + 0xca4c6);
        piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4);
        pcVar53 = *(code **)(DAT_000ca6cc + 0xca4c8);
        iVar52 = (*pcVar53)(*puVar51,1000);
        iVar12 = (*pcVar53)(*puVar51,1000);
        iVar55 = (*pcVar53)(*puVar51,0x9c4);
        uVar61 = VectorSignedToFloat(iVar50,(byte)(in_fpscr >> 0x16) & 3);
        uVar11 = VectorSignedToFloat((iVar12 + -500) * iVar5 + iVar52,(byte)(in_fpscr >> 0x16) & 3);
        uVar59 = VectorSignedToFloat(iVar55 + iVar46,(byte)(in_fpscr >> 0x16) & 3);
        (**(code **)(*piVar37 + 0x48))(piVar37,uVar61,uVar11,uVar59);
        pKVar39 = *(KIPlayer **)((int)*(void **)(*(int *)(this + 0xf8) + 4) + iVar5 * 4);
        pRVar6 = (Route *)Route::clone(*(__fn **)(this + 0x110),
                                       *(void **)(*(int *)(this + 0xf8) + 4),__flags_18,__arg_20);
        KIPlayer::setRoute(pKVar39,pRVar6);
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4) + 4),0));
        PlayerFighter::setAIDisabled
                  (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4),true);
        pPVar30 = *(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4);
        pPVar30[0x13d] = (PlayerFighter)0x0;
        PlayerFighter::setCloakingPossible(pPVar30,false);
        piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 * 4);
        pVVar38 = (Vector *)piVar37[2];
        local_68 = (ulonglong)DAT_000ca6d0;
        local_60 = DAT_000ca6d4;
        (**(code **)(*piVar37 + 0x28))(&local_134);
        AbyssEngine::AEMath::operator-((Vector *)&local_110,(Vector *)&local_68);
        AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_e8,(Vector *)&local_110);
        local_154 = 0x3f800000;
        local_158 = 0.0;
        local_150 = 0;
        AEGeometry::setDirection(pVVar38,(Vector *)&local_e8);
        iVar50 = iVar50 + 3000;
      }
      uVar11 = createStaticObject(this,(Waypoint *)0x0,0x4a6a,true);
      fVar58 = DAT_000ca6d4;
      *(undefined4 *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) = uVar11;
      piVar37 = *(int **)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4);
      (**(code **)(*piVar37 + 0x48))(piVar37,DAT_000ca6d0,0,fVar58);
      KIPlayer::setVisible
                (*(KIPlayer **)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4),
                 false);
      KIPlayer::setActive(SUB41(*(undefined4 *)
                                 ((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4),0)
                         );
      pSVar25 = (String *)GameText::getText(*piVar45);
      AbyssEngine::String::operator=
                ((String *)
                 (*(int *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) + 0x18),
                 pSVar25);
      iVar46 = *(int *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4);
      *(undefined4 *)(iVar46 + 0x28) = 3;
      Player::setHitpoints(*(int *)(iVar46 + 4));
      *(undefined1 *)
       (*(int *)((*(int **)(this + 0xf8))[1] + **(int **)(this + 0xf8) * 4 + -4) + 0x74) = 1;
      pOVar14 = operator_new(0x1c);
      Objective::Objective(pOVar14,4,2,this);
      break;
    case 0x93:
      pOVar14 = operator_new(0x1c);
      Objective::Objective(pOVar14,4,1,this);
      break;
    case 0x9a:
      local_e0 = *(float *)(DAT_000caa9c + 0xca6ea);
      local_e8 = (float)*(undefined8 *)(DAT_000caa9c + 0xca6e2);
      fStack_e4 = (float)((ulonglong)*(undefined8 *)(DAT_000caa9c + 0xca6e2) >> 0x20);
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_e8,3);
      *(Route **)(this + 0x110) = pRVar6;
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
      ArraySetLength<KIPlayer*>(0x16,(Array *)pAVar7);
      local_110 = *(ulonglong *)(DAT_000caaa0 + 0xca722);
      local_108 = *(float *)(DAT_000caaa0 + 0xca72a);
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_110,3);
      *(Route **)(this + 0x10c) = pRVar6;
      pWVar8 = (Waypoint *)Route::getWaypoint(pRVar6,0);
      uVar11 = createShip(this,0,0,0x33,pWVar8,true,false);
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar11;
      Player::setAlwaysFriend(*(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),true);
      piVar45 = (int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4);
      PlayerEgo::getPosition();
      fVar58 = (float)AEGeometry::getRightVector();
      AbyssEngine::AEMath::operator*((Vector *)&local_158,fVar58);
      AbyssEngine::AEMath::operator+((Vector *)&local_68,(Vector *)&local_134);
      fVar58 = (float)AEGeometry::getDirection();
      AbyssEngine::AEMath::operator*((Vector *)&local_194,fVar58);
      AbyssEngine::AEMath::operator+((Vector *)&local_a8,(Vector *)&local_68);
      (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_a8);
      pVVar38 = *(Vector **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8);
      PlayerEgo::GetDirVector();
      local_68 = 0x3f80000000000000;
      local_60 = 0.0;
      AEGeometry::setDirection(pVVar38,(Vector *)&local_a8);
      KIPlayer::setEnemies((Array *)**(undefined4 **)(*(int *)(this + 0xf8) + 4));
      puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
      *(undefined1 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x13d) = 0;
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
      }
      *puVar51 = 0;
      uVar11 = createStaticObject(this,(Waypoint *)0x0,0x4220,false);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) = uVar11;
      PlayerFixedObject::setMoving
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),false);
      PlayerFixedObject::setDockingType
                (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + 4),3);
      uVar11 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
      pSVar25 = (String *)GameText::getText(**(int **)(DAT_000caaac + 0xca890));
      AbyssEngine::String::String(aSStack_1dc,pSVar25,false);
      PlayerFixedObject::setName(uVar11,aSStack_1dc);
      AbyssEngine::String::~String(aSStack_1dc);
      Player::setAlwaysFriend
                (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 4),true);
      AEGeometry::setRotation
                (*(AEGeometry **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 8),
                 extraout_s0_12,extraout_s1_13,extraout_s2_12);
      iVar46 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
      puVar51 = (undefined4 *)(iVar46 + 0x50);
      *(undefined1 *)(iVar46 + 0x70) = 0;
      *(undefined1 *)(iVar46 + 0x74) = 1;
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4) + 0x50);
      }
      iVar46 = DAT_000caab4;
      *puVar51 = 0;
      __aeabi_memcpy(&local_a8,iVar46 + 0xca91c,0x24);
      local_68 = *(ulonglong *)(DAT_000caab8 + 0xca924);
      local_60 = *(float *)(DAT_000caab8 + 0xca92c);
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      ArraySetLength<KIPlayer*>(3,(Array *)pAVar7);
      *(undefined4 *)(*(int *)(pAVar7 + 4) + 4) =
           *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 4);
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_a8,(Array *)pAVar7,(int *)&local_68,9);
      KIPlayer::setRoute((KIPlayer *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),pRVar6);
      iVar46 = **(int **)(*(int *)(this + 0xf8) + 4);
      *(undefined1 *)(iVar46 + 0x13e) = 0;
      Player::setVulnerable(SUB41(*(undefined4 *)(iVar46 + 4),0));
      local_130 = 0.0;
      local_134 = 0.0;
      local_12c = DAT_000caabc;
      uVar36 = 2;
      puVar51 = *(undefined4 **)(DAT_000caac0 + 0xca9ac);
      puVar19 = *(undefined4 **)(DAT_000caac4 + 0xca9b4);
      for (uVar27 = 4; uVar27 != 0x2c; uVar27 = uVar27 + 2) {
        iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,9);
        pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
        uVar11 = createShip(this,9,0,iVar46,pWVar8,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 2) = uVar11;
        KIPlayer::setActive(SUB41(*(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 2),
                                  0));
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 2) + 4),0));
        piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 2);
        pcVar53 = *(code **)(DAT_000caac8 + 0xcaa24);
        if (uVar36 < 0xb) {
          iVar46 = (*pcVar53)(*puVar19,100);
          uVar11 = VectorSignedToFloat(uVar36,(byte)(in_fpscr >> 0x16) & 3);
          iVar5 = (*pcVar53)(uVar11,*puVar19,200);
          uVar11 = VectorSignedToFloat(iVar5 + -200,(byte)(in_fpscr >> 0x16) & 3);
          uVar59 = VectorSignedToFloat((iVar46 + 900) * ((~uVar27 & 2) - 1),
                                       (byte)(in_fpscr >> 0x16) & 3);
          iVar46 = (*pcVar53)(*puVar19,400);
          local_18c = (float)VectorSignedToFloat(iVar46 + -200,(byte)(in_fpscr >> 0x16) & 3);
          local_194 = uVar59;
          local_190 = uVar11;
          AbyssEngine::AEMath::operator*(local_18c,(Vector *)&local_188);
        }
        else {
          iVar46 = (*pcVar53)(*puVar19,100);
          uVar11 = VectorSignedToFloat(uVar36,(byte)(in_fpscr >> 0x16) & 3);
          iVar5 = (*pcVar53)(uVar11,*puVar19,100);
          uVar11 = VectorSignedToFloat(iVar5 + -200,(byte)(in_fpscr >> 0x16) & 3);
          uVar59 = VectorSignedToFloat((iVar46 + 0x640) * ((~uVar27 & 2) - 1),
                                       (byte)(in_fpscr >> 0x16) & 3);
          iVar46 = (*pcVar53)(*puVar19,500);
          local_18c = (float)VectorSignedToFloat(iVar46 + 1000,(byte)(in_fpscr >> 0x16) & 3);
          local_194 = uVar59;
          local_190 = uVar11;
          AbyssEngine::AEMath::operator*(local_18c,(Vector *)&local_188);
        }
        AbyssEngine::AEMath::operator+((Vector *)&local_158,(Vector *)&local_134);
        (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_158);
        pVVar38 = *(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 2) + 8);
        PlayerEgo::getPosition();
        (**(code **)(**(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 2) + 0x28))
                  (aVStack_1a0);
        AbyssEngine::AEMath::operator-((Vector *)&local_188,(Vector *)&local_194);
        AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_158,(Vector *)&local_188);
        local_1a8 = 0x3f800000;
        local_1ac = 0;
        local_1a4 = 0;
        AEGeometry::setDirection(pVVar38,(Vector *)&local_158);
        uVar36 = uVar36 + 1;
      }
      pOVar14 = operator_new(0x1c);
      Objective::Objective(pOVar14,4,0xb,this);
      break;
    case 0x9b:
      pOVar14 = operator_new(0x1c);
      Objective::Objective(pOVar14,4,2,this);
      break;
    case 0x9d:
      local_a8 = *(ulonglong *)(DAT_000caf24 + 0xcabfc);
      local_a0 = *(longlong *)(DAT_000caf24 + 0xcac04);
      local_98 = *(longlong *)(DAT_000caf24 + 0xcac0c);
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_a8,6);
      *(Route **)(this + 0x10c) = pRVar6;
      pWVar8 = (Waypoint *)Route::getWaypoint(pRVar6,0);
      Waypoint::reached(pWVar8);
      local_e8 = *(float *)(DAT_000caf28 + 0xcac36);
      fStack_e4 = *(float *)(DAT_000caf28 + 0xcac3a);
      local_e0 = *(float *)(DAT_000caf28 + 0xcac3e);
      local_dc = *(int *)(DAT_000caf28 + 0xcac42);
      local_d8 = *(longlong *)(DAT_000caf28 + 0xcac46);
      pRVar6 = operator_new(0x18);
      Route::Route(pRVar6,(int *)&local_e8,6);
      *(Route **)(this + 0x110) = pRVar6;
      pWVar8 = (Waypoint *)Route::getWaypoint(pRVar6,0);
      Waypoint::reached(pWVar8);
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      pbVar31 = *(byte **)(DAT_000caf2c + 0xcac80);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
      uVar27 = (uint)*pbVar31;
      uVar36 = 10;
      if (uVar27 == 0) {
        uVar36 = 0x14;
      }
      uVar43 = uVar27 ^ 1;
      ArraySetLength<KIPlayer*>((uVar36 | uVar43) + 3,(Array *)pAVar7);
      uVar36 = 5;
      local_110 = (ulonglong)DAT_000caf34;
      local_108 = DAT_000caf38;
      if (uVar27 == 0) {
        uVar36 = 10;
      }
      pSVar20 = (Station *)Galaxy::getStation(**(int **)(DAT_000caf30 + 0xcacaa));
      pPVar21 = operator_new(0x174);
      PlayerStation::PlayerStation(pPVar21,pSVar20);
      uVar27 = uVar36 + uVar43;
      uVar44 = uVar27 + uVar36;
      uVar47 = uVar44 + 2;
      *(PlayerStation **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar47 * 4) = pPVar21;
      Station::getName();
      AbyssEngine::String::operator=
                ((String *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar47 * 4) + 0x18),
                 (String *)&local_68);
      AbyssEngine::String::~String((String *)&local_68);
      piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar47 * 4);
      piVar45[10] = 3;
      (**(code **)(*piVar45 + 0x44))(piVar45,&local_110);
      puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar47 * 4) + 0x50);
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar47 * 4) + 0x50);
      }
      *puVar51 = 0;
      if (pSVar20 != (Station *)0x0) {
        pvVar13 = (void *)Station::~Station(pSVar20);
        operator_delete(pvVar13);
      }
      puVar51 = *(undefined4 **)(DAT_000caf3c + 0xcad80);
      for (uVar48 = 0; uVar61 = DAT_000caf48, uVar59 = DAT_000caf44, uVar11 = DAT_000caf40,
          uVar49 = uVar36, uVar36 != uVar48; uVar48 = uVar48 + 1) {
        iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,0);
        pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x10c),0);
        uVar11 = createShip(this,0,0,iVar46,pWVar8,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar48 * 4) = uVar11;
        Player::setAlwaysFriend
                  (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar48 * 4) + 4),true
                  );
        pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar48 * 4);
        pRVar6 = (Route *)Route::getExactClone();
        KIPlayer::setRoute(pKVar39,pRVar6);
      }
      for (; uVar49 < uVar27; uVar49 = uVar49 + 1) {
        uVar32 = createStaticObject(this,(Waypoint *)0x0,0x4974,true);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar49 * 4) = uVar32;
        PlayerFixedObject::setMoving
                  (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar49 * 4),false);
        Player::setAlwaysFriend
                  (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar49 * 4) + 4),true
                  );
        piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar49 * 4);
        (**(code **)(*piVar45 + 0x48))(piVar45,uVar11,uVar59,uVar61);
        puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar49 * 4) + 0x50);
        if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
          pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
          operator_delete(pvVar13);
          puVar51 = (undefined4 *)
                    (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar49 * 4) + 0x50);
        }
        *puVar51 = 0;
      }
      pcVar53 = *(code **)(DAT_000caf4c + 0xcae92);
      piVar45 = (int *)(*pcVar53)(*(undefined4 *)(this + 0x110),1);
      (**(code **)(*piVar45 + 0x28))((Vector *)&local_134,piVar45);
      piVar45 = (int *)(*pcVar53)(*(undefined4 *)(this + 0x110),0);
      (**(code **)(*piVar45 + 0x28))(&local_158,piVar45);
      AbyssEngine::AEMath::operator-((Vector *)&local_68,(Vector *)&local_134);
      local_68 = (ulonglong)(uint)(float)local_68;
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_134,(Vector *)&local_68);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_68,(Vector *)&local_134);
      piVar45 = (int *)(*pcVar53)(*(undefined4 *)(this + 0x110),0);
      (**(code **)(*piVar45 + 0x28))(&local_134,piVar45);
      uVar32 = DAT_000caf60;
      uVar61 = DAT_000caf5c;
      uVar59 = DAT_000caf58;
      uVar11 = DAT_000caf54;
      iVar5 = uVar36 * 4 + uVar43 * 4;
      iVar46 = DAT_000caf50 + 500;
      uVar48 = uVar27;
      fVar58 = extraout_s0_13;
      fVar60 = extraout_s1_14;
      fVar56 = extraout_s2_13;
      for (uVar49 = 0; uVar33 = DAT_000cb468, (int)(uVar27 + uVar49) < (int)uVar44;
          uVar49 = uVar49 + 1) {
        uVar33 = createShip(this,10,0,0x2c,(Waypoint *)0x0,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4) = uVar33;
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4) + 4),0
                        ));
        KIPlayer::setVisible
                  (*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4),false);
        KIPlayer::setActive(SUB41(*(undefined4 *)
                                   (*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4),0));
        local_158 = 0.0;
        local_154 = 0x3f800000;
        local_150 = 0;
        AEGeometry::setDirection
                  (*(Vector **)
                    (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4) + 8),
                   (Vector *)&local_68);
        PlayerFighter::setAIDisabled
                  (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4),
                   true);
        PlayerFighter::setCloakingPossible
                  (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4),
                   false);
        pKVar39 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4);
        pRVar6 = (Route *)Route::getExactClone();
        KIPlayer::setRoute(pKVar39,pRVar6);
        iVar50 = iVar46;
        local_150 = uVar11;
        if (1 < uVar49) {
          if (uVar49 < 5) {
            iVar50 = 9000;
            local_150 = uVar59;
          }
          else {
            if (8 < uVar49) {
              iVar50 = 0;
              local_150 = uVar61;
              goto LAB_000cb068;
            }
            iVar50 = 20000;
            local_150 = uVar32;
          }
          iVar50 = (uVar48 - uVar27) * 3000 - iVar50;
        }
LAB_000cb068:
        local_158 = (float)VectorSignedToFloat(iVar50,(byte)(in_fpscr >> 0x16) & 3);
        local_154 = 0;
        AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x18c),(Vector *)&local_158);
        puVar51 = *(undefined4 **)(DAT_000cb45c + 0xcb098);
        pcVar53 = *(code **)(DAT_000cb460 + 0xcb09a);
        iVar50 = (*pcVar53)(*puVar51,200);
        fVar58 = (float)VectorSignedToFloat(iVar50 + -100,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(this + 0x18c) = *(float *)(this + 0x18c) + fVar58;
        iVar50 = (*pcVar53)(*puVar51,200);
        fVar58 = (float)VectorSignedToFloat(iVar50 + -100,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(this + 400) = *(float *)(this + 400) + fVar58;
        iVar50 = (*pcVar53)(*puVar51,200);
        fVar58 = (float)VectorSignedToFloat(iVar50 + -100,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(this + 0x194) = *(float *)(this + 0x194) + fVar58;
        piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar5 + uVar49 * 4);
        AbyssEngine::AEMath::operator+((Vector *)&local_158,(Vector *)&local_134);
        (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_158);
        uVar48 = uVar48 + 1;
        iVar46 = iVar46 + 2000;
        fVar58 = extraout_s0_14;
        fVar60 = extraout_s1_15;
        fVar56 = extraout_s2_14;
      }
      piVar45 = *(int **)(DAT_000cb464 + 0xcb144);
      for (uVar27 = uVar44; uVar11 = DAT_000cb46c, uVar27 <= uVar44; uVar27 = uVar27 + 1) {
        uVar11 = createShip(this,2,0,0x31,(Waypoint *)0x0,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4) = uVar11;
        piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4);
        (**(code **)(*piVar37 + 0x44))(piVar37,&local_134);
        Player::setAlwaysEnemy
                  (SUB41(*(undefined4 *)
                          (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4) + 4),0));
        PlayerFighter::setAIDisabled
                  (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4),true);
        local_158 = 0.0;
        local_154 = 0x3f800000;
        local_150 = 0;
        AEGeometry::setDirection
                  (*(Vector **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4) + 8),
                   (Vector *)&local_68);
        KIPlayer::setActive(SUB41(*(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4),
                                  0));
        KIPlayer::setVisible(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4),false)
        ;
        piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4);
        (**(code **)(*piVar37 + 0x1c))(piVar37,uVar33);
        pSVar25 = (String *)GameText::getText(*piVar45);
        AbyssEngine::String::operator=
                  ((String *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4) + 0x18),
                   pSVar25);
        puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4) + 0x50);
        fVar58 = extraout_s0_15;
        fVar60 = extraout_s1_16;
        fVar56 = extraout_s2_15;
        if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
          pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
          operator_delete(pvVar13);
          puVar51 = (undefined4 *)
                    (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar27 * 4) + 0x50);
          fVar58 = extraout_s0_16;
          fVar60 = extraout_s1_17;
          fVar56 = extraout_s2_16;
        }
        *puVar51 = 0;
      }
      uVar43 = uVar43 + uVar36 * 2;
      while (uVar43 = uVar43 + 1, uVar43 < uVar47) {
        uVar59 = createShip(this,3,0,0x14,(Waypoint *)0x0,true,false);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4) = uVar59;
        piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4);
        (**(code **)(*piVar37 + 0x48))(piVar37,uVar11,uVar11,uVar11);
        PlayerFighter::setAIDisabled
                  (*(PlayerFighter **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4),true);
        KIPlayer::setActive(SUB41(*(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4),
                                  0));
        KIPlayer::setVisible(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4),false)
        ;
        pSVar25 = (String *)GameText::getText(*piVar45);
        AbyssEngine::String::operator=
                  ((String *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar43 * 4) + 0x18),
                   pSVar25);
        fVar58 = extraout_s0_17;
        fVar60 = extraout_s1_18;
        fVar56 = extraout_s2_17;
      }
      PlayerEgo::setPosition(fVar58,fVar60,fVar56);
      pVVar38 = *(Vector **)(*(int *)(this + 0xf0) + 8);
      local_188 = DAT_000cb47c;
      local_184 = DAT_000cb478;
      local_180 = 0xbf000000;
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_158,(Vector *)&local_188);
      local_194 = 0;
      local_190 = 0x3f800000;
      local_18c = 0.0;
      AEGeometry::setDirection(pVVar38,(Vector *)&local_158);
      goto switchD_000c4bf8_caseD_2;
    case 0x9e:
      pAVar7 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar7);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
      ArraySetLength<KIPlayer*>(1,(Array *)pAVar7);
      uVar59 = createShip(this,10,0,0x31,(Waypoint *)0x0,true,false);
      uVar11 = DAT_000cb480;
      **(undefined4 **)(*(int *)(this + 0xf8) + 4) = uVar59;
      (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x48))
                ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000cb484,0,uVar11);
      pVVar38 = *(Vector **)(**(int **)(*(int *)(this + 0xf8) + 4) + 8);
      local_e8 = DAT_000cb47c;
      fStack_e4 = 0.0;
      local_e0 = DAT_000cb488;
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_a8,(Vector *)&local_e8);
      local_110 = 0x3f80000000000000;
      local_108 = 0.0;
      AEGeometry::setDirection(pVVar38,(Vector *)&local_a8);
      Player::setAlwaysEnemy(SUB41(*(undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 4),0));
      (**(code **)(*(int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4) + 0x1c))
                ((int *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),DAT_000cb48c);
      pPVar40 = *(Player **)(**(int **)(*(int *)(this + 0xf8) + 4) + 4);
      iVar46 = Player::getMaxEmpPoints(pPVar40);
      Player::setMaxEmpPoints(pPVar40,iVar46 * 3);
      PlayerFighter::setCloakingPossible
                ((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),false);
      pSVar25 = (String *)GameText::getText(**(int **)(DAT_000cb490 + 0xcb402));
      AbyssEngine::String::operator=
                ((String *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x18),pSVar25);
      PlayerFighter::setAIDisabled
                ((PlayerFighter *)**(undefined4 **)(*(int *)(this + 0xf8) + 4),true);
      puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
      if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
        pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
        operator_delete(pvVar13);
        puVar51 = (undefined4 *)(**(int **)(*(int *)(this + 0xf8) + 4) + 0x50);
      }
      uVar11 = DAT_000cb46c;
      *puVar51 = 0;
      for (iVar46 = 3; iVar46 != 0; iVar46 = iVar46 + -1) {
        pKVar39 = (KIPlayer *)createStaticObject(this,(Waypoint *)0x0,0x49c2,true);
        Player::setRadius(*(Player **)(pKVar39 + 4),800);
        Player::setAlwaysEnemy(SUB41(*(undefined4 *)(pKVar39 + 4),0));
        Player::setAlwaysFriend(*(Player **)(pKVar39 + 4),false);
        Player::setMaxHitpoints(*(Player **)(pKVar39 + 4),100);
        (**(code **)(*(int *)pKVar39 + 0x48))(pKVar39,uVar11,uVar11,uVar11);
        KIPlayer::setActive(SUB41(pKVar39,0));
        ArrayAdd<KIPlayer*>(pKVar39,*(Array **)(this + 0xf8));
      }
      pOVar14 = operator_new(0x1c);
      Objective::Objective(pOVar14,0x16,0,this);
      break;
    default:
      if (iVar5 == 0x72) {
        pAVar7 = operator_new(0xc);
        Array<KIPlayer*>::Array(pAVar7);
        *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
        ArraySetLength<KIPlayer*>(6,(Array *)pAVar7);
        local_a8 = 0;
        local_a0 = (ulonglong)local_a0._4_4_ << 0x20;
        local_e0 = *(float *)(DAT_000cb8c4 + 0xcb532);
        local_e8 = (float)*(undefined8 *)(DAT_000cb8c4 + 0xcb52a);
        fStack_e4 = (float)((ulonglong)*(undefined8 *)(DAT_000cb8c4 + 0xcb52a) >> 0x20);
        pRVar6 = operator_new(0x18);
        Route::Route(pRVar6,(int *)&local_e8,3);
        fVar58 = DAT_000cb8c8;
        *(Route **)(this + 0x110) = pRVar6;
        puVar51 = *(undefined4 **)(DAT_000cb8cc + 0xcb566);
        for (iVar46 = 0; iVar46 != 6; iVar46 = iVar46 + 1) {
          iVar5 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
          pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
          uVar11 = createShip(this,8,0,iVar5,pWVar8,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
          (**(code **)(**(int **)((*(uint **)(this + 0xfc))[1] +
                                 (iVar46 + (**(uint **)(this + 0xfc) >> 1)) * 4) + 0x28))
                    ((Vector *)&local_110);
          AbyssEngine::AEMath::Vector::operator=((Vector *)&local_a8,(Vector *)&local_110);
          piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
          (**(code **)(*piVar45 + 0x48))
                    (piVar45,(float)local_a8,local_a8._4_4_ + fVar58,(float)local_a0);
          KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4));
          iVar5 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
          pPVar40 = *(Player **)(iVar5 + 4);
          *(undefined4 *)(iVar5 + 0x128) = 7000;
          iVar5 = Player::getMaxHitpoints();
          Player::setMaxHitpoints(pPVar40,iVar5 << 1);
          iVar5 = *(int *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4);
          Player::getMaxHitpoints();
          Player::setHitpoints(iVar5);
        }
        pOVar14 = operator_new(0x1c);
        Objective::Objective(pOVar14,0x12,0,6,this);
      }
      else if (iVar5 == 0x78) {
        local_a8 = *(ulonglong *)(DAT_000cb8d0 + 0xcb650);
        local_a0 = CONCAT44(local_a0._4_4_,*(undefined4 *)(DAT_000cb8d0 + 0xcb658));
        pRVar6 = operator_new(0x18);
        Route::Route(pRVar6,(int *)&local_a8,3);
        *(Route **)(this + 0x110) = pRVar6;
        pAVar7 = operator_new(0xc);
        Array<KIPlayer*>::Array(pAVar7);
        *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
        ArraySetLength<KIPlayer*>(2,(Array *)pAVar7);
        for (iVar46 = 0; iVar46 != 2; iVar46 = iVar46 + 1) {
          pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
          uVar11 = createShip(this,10,0,0x2c,pWVar8,true,false);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) = uVar11;
          Player::setAlwaysEnemy
                    (SUB41(*(undefined4 *)
                            (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4) + 4),0));
          piVar45 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46 * 4);
          PlayerEgo::getPosition();
          fVar58 = (float)AEGeometry::getRightVector();
          AbyssEngine::AEMath::operator*((Vector *)&local_134,fVar58);
          AbyssEngine::AEMath::operator+((Vector *)&local_110,(Vector *)&local_68);
          fVar58 = (float)AEGeometry::getDirection();
          AbyssEngine::AEMath::operator*((Vector *)&local_188,fVar58);
          AbyssEngine::AEMath::operator+((Vector *)&local_e8,(Vector *)&local_110);
          (**(code **)(*piVar45 + 0x44))(piVar45,(Vector *)&local_e8);
        }
        pOVar14 = operator_new(0x1c);
        Objective::Objective(pOVar14,4,0,this);
      }
      else if (iVar5 == 0x7b) {
        pOVar14 = operator_new(0x1c);
        Objective::Objective(pOVar14,4,3,this);
      }
      else {
        if (iVar5 != 0x7d) goto switchD_000c4bf8_caseD_2;
        local_a8 = *(ulonglong *)(DAT_000c4ea8 + 0xc4e3e);
        local_a0 = *(longlong *)(DAT_000c4ea8 + 0xc4e46);
        local_98 = *(longlong *)(DAT_000c4ea8 + 0xc4e4e);
        pRVar6 = operator_new(0x18);
        Route::Route(pRVar6,(int *)&local_a8,6);
        *(Route **)(this + 0x110) = pRVar6;
        pAVar7 = operator_new(0xc);
        Array<KIPlayer*>::Array(pAVar7);
        *(Array<KIPlayer*> **)(this + 0xf8) = pAVar7;
        ArraySetLength<KIPlayer*>(0xf,(Array *)pAVar7);
        puVar51 = *(undefined4 **)(DAT_000c4eac + 0xc4e8e);
        for (uVar36 = 0; uVar36 != 8; uVar36 = uVar36 + 1) {
          iVar46 = Globals::getRandomEnemyFighter((Globals *)*puVar51,8);
          if (uVar36 < 2) {
            pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),0);
            uVar11 = createShip(this,8,0,iVar46,pWVar8,true,false);
            *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
          }
          else {
            pWVar8 = (Waypoint *)Route::getWaypoint(*(Route **)(this + 0x110),1);
            uVar11 = createShip(this,8,0,iVar46,pWVar8,true,false);
            *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) = uVar11;
            KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4));
          }
          Player::setAlwaysEnemy
                    (SUB41(*(undefined4 *)
                            (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar36 * 4) + 4),0));
        }
        pWVar8 = operator_new(0x138);
        iVar46 = DAT_000c52e4;
        Waypoint::Waypoint(pWVar8,DAT_000c52e4 + -8000,0,DAT_000c52e8,(Route *)0x0);
        pWVar9 = operator_new(0x138);
        Waypoint::Waypoint(pWVar9,iVar46,0,DAT_000c52ec,(Route *)0x0);
        pWVar10 = operator_new(0x138);
        Waypoint::Waypoint(pWVar10,DAT_000c52f0 + 4000,53000,DAT_000c52f4,(Route *)0x0);
        pcVar53 = *(code **)(DAT_000c52f8 + 0xc4fa2);
        uVar11 = (*pcVar53)(this,pWVar8,0x4961,1);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x20) = uVar11;
        uVar11 = (*pcVar53)(this,pWVar9,0x4961,1);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x24) = uVar11;
        uVar11 = (*pcVar53)(this,pWVar10,0x4961,1);
        *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + 0x28) = uVar11;
        for (iVar46 = 0x20; fVar60 = DAT_000c530c, fVar58 = DAT_000c5300, iVar46 != 0x2c;
            iVar46 = iVar46 + 4) {
          PlayerFixedObject::setMoving
                    (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46),false);
          PlayerFixedObject::setDockingType
                    (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46),3);
          Player::setAlwaysEnemy
                    (SUB41(*(undefined4 *)
                            (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46) + 4),0));
          Player::setAlwaysFriend
                    (*(Player **)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46) + 4),true);
          iVar5 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46);
          puVar51 = (undefined4 *)(iVar5 + 0x50);
          *(undefined1 *)(iVar5 + 0x74) = 1;
          if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
            pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
            operator_delete(pvVar13);
            puVar51 = (undefined4 *)(*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar46) + 0x50);
          }
          *puVar51 = 0;
        }
        iVar50 = 0;
        piVar45 = *(int **)(DAT_000c5304 + 0xc506c);
        iVar46 = DAT_000c52e4;
        iVar5 = DAT_000c52fc;
        for (iVar52 = 0xb; iVar52 != 0xf; iVar52 = iVar52 + 1) {
          uVar11 = createStaticObject(this,(Waypoint *)0x0,0x4962,true);
          *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar52 * 4) = uVar11;
          PlayerFixedObject::setMoving
                    (*(PlayerFixedObject **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar52 * 4),false
                    );
          piVar37 = *(int **)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar52 * 4);
          iVar12 = AbyssEngine::AERandom::nextInt(*piVar45);
          uVar11 = VectorSignedToFloat(iVar46,(byte)(in_fpscr >> 0x16) & 3);
          uVar59 = VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
          uVar61 = VectorSignedToFloat(iVar12 + iVar50,(byte)(in_fpscr >> 0x16) & 3);
          (**(code **)(*piVar37 + 0x48))(piVar37,uVar11,uVar61,uVar59);
          fVar56 = (float)VectorSignedToFloat(iVar52,(byte)(in_fpscr >> 0x16) & 3);
          AEGeometry::rotate(fVar56 + fVar60,extraout_s1,fVar56 + fVar58);
          iVar12 = *(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar52 * 4);
          puVar51 = (undefined4 *)(iVar12 + 0x50);
          *(undefined1 *)(iVar12 + 0x74) = 1;
          if ((Array<int> *)*puVar51 != (Array<int> *)0x0) {
            pvVar13 = (void *)Array<int>::~Array((Array<int> *)*puVar51);
            operator_delete(pvVar13);
            puVar51 = (undefined4 *)
                      (*(int *)(*(int *)(*(int *)(this + 0xf8) + 4) + iVar52 * 4) + 0x50);
          }
          *puVar51 = 0;
          iVar5 = iVar5 + -4000;
          iVar46 = iVar46 + 4000;
          iVar50 = iVar50 + -200;
        }
        pOVar14 = operator_new(0x1c);
        Objective::Objective(pOVar14,4,7,this);
      }
    }
LAB_000cb9b8:
    *(Objective **)(this + 0x28) = pOVar14;
  }
switchD_000c4bf8_caseD_2:
  iVar46 = Status::getCurrentCampaignMission();
  uVar11 = createRadioMessages(this,iVar46);
  *(undefined4 *)(this + 0x114) = uVar11;
  if (*piVar35 != local_5c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000c4bb8: push {r4,r5,r6,r7,lr}
  000c4bba: add r7,sp,#0xc
  000c4bbc: push {r7,r8,r9,r10,r11}
  000c4bc0: vpush {d8,d9,d10,d11,d12,d13}
  000c4bc4: sub sp,#0x1d0
  000c4bc6: mov r5,r0
  000c4bc8: ldr r0,[0x000c4e94]
  000c4bca: add r0,pc
  000c4bcc: ldr r4,[r0,#0x0]
  000c4bce: ldr r0,[r4,#0x0]
  000c4bd0: str r0,[sp,#0x1cc]
  000c4bd2: ldr.w r0,[r5,#0xf0]
  000c4bd6: ldr r1,[r0,#0x8]
  000c4bd8: add r0,sp,#0x10c
  000c4bda: blx 0x000720b8
  000c4bde: ldr r0,[0x000c4e98]
  000c4be0: add r0,pc
  000c4be2: ldr r6,[r0,#0x0]
  000c4be4: ldr r0,[r6,#0x0]
  000c4be6: blx 0x00071770
  000c4bea: cmp r0,#0x6a
  000c4bec: str r5,[sp,#0x48]
  000c4bee: mov r1,r5
  000c4bf0: strd r4,r6,[sp,#0x40]
  000c4bf4: bhi.w 0x000c4dc4
  000c4bf8: tbh [pc,r0]
  000c4cd2: movs r0,#0xc
  000c4cd4: blx 0x0006eb24
  000c4cd8: mov r5,r0
  000c4cda: blx 0x00073d08
  000c4cde: ldr r0,[sp,#0x48]
  000c4ce0: mov r1,r5
  000c4ce2: str.w r5,[r0,#0xf8]
  000c4ce6: movs r0,#0x3
  000c4ce8: blx 0x00073d14
  000c4cec: ldr r0,[0x000c4e9c]
  000c4cee: mov.w r9,#0x0
  000c4cf2: ldr r1,[0x000c4ea0]
  000c4cf4: add r0,pc
  000c4cf6: add r1,pc
  000c4cf8: ldr r4,[r0,#0x0]
  000c4cfa: ldr r6,[r1,#0x0]
  000c4cfc: b 0x000c4dba
  000c4cfe: movs r0,#0x0
  000c4d00: movs r1,#0x1
  000c4d02: str r0,[sp,#0x0]
  000c4d04: movs r2,#0x0
  000c4d06: strd r1,r0,[sp,#0x4]
  000c4d0a: movs r1,#0x9
  000c4d0c: ldr r5,[sp,#0x48]
  000c4d0e: movs r3,#0x8
  000c4d10: mov r0,r5
  000c4d12: blx 0x00073e88
  000c4d16: ldr.w r1,[r5,#0xf8]
  000c4d1a: ldr r1,[r1,#0x4]
  000c4d1c: str.w r0,[r1,r9,lsl #0x2]
  000c4d20: movs r1,#0x2
  000c4d22: ldr r0,[r4,#0x0]
  000c4d24: blx r6
  000c4d26: mov r11,r0
  000c4d28: ldr r1,[0x000c4ea4]
  000c4d2a: ldr r0,[r4,#0x0]
  000c4d2c: mov r8,r1
  000c4d2e: blx r6
  000c4d30: mov r10,r0
  000c4d32: ldr r0,[r4,#0x0]
  000c4d34: movs r1,#0x2
  000c4d36: blx r6
  000c4d38: mov.w r5,#0xffffffff
  000c4d3c: cmp r0,#0x0
  000c4d3e: it eq
  000c4d40: mov.eq r5,#0x1
  000c4d42: ldr r0,[r4,#0x0]
  000c4d44: mov r1,r8
  000c4d46: blx r6
  000c4d48: movw r1,#0x4e20
  000c4d4c: add r0,r1
  000c4d4e: add r1,r10
  000c4d50: cmp.w r11,#0x0
  000c4d54: mul r8,r0,r5
  000c4d58: mov.w r0,#0xffffffff
  000c4d5c: it eq
  000c4d5e: mov.eq r0,#0x1
  000c4d60: movw r10,#0x4e20
  000c4d64: muls r0,r1
  000c4d66: movs r1,#0x2
  000c4d68: vmov s16,r0
  000c4d6c: ldr r0,[r4,#0x0]
  000c4d6e: blx r6
  000c4d70: mov.w r5,#0xffffffff
  000c4d74: cmp r0,#0x0
  000c4d76: it eq
  000c4d78: mov.eq r5,#0x1
  000c4d7a: ldr r0,[r4,#0x0]
  000c4d7c: ldr r1,[0x000c4ea4]
  000c4d7e: vcvt.f32.s32 s16,s16
  000c4d82: blx r6
  000c4d84: add r0,r10
  000c4d86: vmov s0,r8
  000c4d8a: vmov r1,s16
  000c4d8e: muls r0,r5
  000c4d90: vcvt.f32.s32 s0,s0
  000c4d94: vmov s2,r0
  000c4d98: vmov r2,s0
  000c4d9c: vcvt.f32.s32 s2,s2
  000c4da0: ldr r0,[sp,#0x48]
  000c4da2: ldr.w r0,[r0,#0xf8]
  000c4da6: ldr r0,[r0,#0x4]
  000c4da8: vmov r3,s2
  000c4dac: ldr.w r0,[r0,r9,lsl #0x2]
  000c4db0: ldr r5,[r0,#0x0]
  000c4db2: ldr r5,[r5,#0x48]
  000c4db4: blx r5
  000c4db6: add.w r9,r9,#0x1
  000c4dba: cmp.w r9,#0x3
  000c4dbe: bne 0x000c4cfe
  000c4dc0: b.w 0x000cb9bc
  000c4dc4: sub.w r1,r0,#0x83
  000c4dc8: cmp r1,#0x1b
  000c4dca: bhi 0x000c4e1e
  000c4dcc: tbh [pc,r1]
  000c4e08: movs r0,#0x1c
  000c4e0a: blx 0x0006eb24
  000c4e0e: mov r5,r0
  000c4e10: ldr r3,[sp,#0x48]
  000c4e12: movs r1,#0x4
  000c4e14: movs r2,#0x0
  000c4e16: blx 0x00073edc
  000c4e1a: b.w 0x000cb9b8
  000c4e1e: cmp r0,#0x72
  000c4e20: beq.w 0x000cb504
  000c4e24: cmp r0,#0x78
  000c4e26: beq.w 0x000cb64a
  000c4e2a: cmp r0,#0x7b
  000c4e2c: beq.w 0x000cb766
  000c4e30: cmp r0,#0x7d
  000c4e32: bne.w 0x000cb9bc
  000c4e36: ldr r0,[0x000c4ea8]
  000c4e38: add r4,sp,#0x180
  000c4e3a: add r0,pc
  000c4e3c: vld1.64 {d16,d17},[r0]!
  000c4e40: vldr.64 d18,[r0]
  000c4e44: mov r0,r4
  000c4e46: vst1.64 {d16,d17},[r0]!
  000c4e4a: vstr.64 d18,[r0]
  000c4e4e: movs r0,#0x18
  000c4e50: blx 0x0006eb24
  000c4e54: mov r5,r0
  000c4e56: mov r1,r4
  000c4e58: movs r2,#0x6
  000c4e5a: blx 0x00073eb8
  000c4e5e: ldr r0,[sp,#0x48]
  000c4e60: str.w r5,[r0,#0x110]
  000c4e64: movs r0,#0xc
  000c4e66: blx 0x0006eb24
  000c4e6a: mov r5,r0
  000c4e6c: blx 0x00073d08
  000c4e70: ldr r0,[sp,#0x48]
  000c4e72: mov r1,r5
  000c4e74: str.w r5,[r0,#0xf8]
  000c4e78: movs r0,#0xf
  000c4e7a: blx 0x00073d14
  000c4e7e: ldr r0,[0x000c4eac]
  000c4e80: mov.w r8,#0x0
  000c4e84: mov.w r9,#0x1
  000c4e88: movs r4,#0x0
  000c4e8a: add r0,pc
  000c4e8c: ldr.w r10,[r0,#0x0]
  000c4e90: b 0x000c4f38
  000c4eb0: ldr.w r0,[r10,#0x0]
  000c4eb4: movs r1,#0x8
  000c4eb6: blx 0x00071a34
  000c4eba: mov r5,r0
  000c4ebc: ldr r0,[sp,#0x48]
  000c4ebe: cmp r4,#0x2
  000c4ec0: ldr.w r0,[r0,#0x110]
  000c4ec4: bcc 0x000c4efc
  000c4ec6: movs r1,#0x1
  000c4ec8: blx 0x00073ec4
  000c4ecc: strd r0,r9,[sp,#0x0]
  000c4ed0: movs r1,#0x8
  000c4ed2: ldr r6,[sp,#0x48]
  000c4ed4: movs r2,#0x0
  000c4ed6: mov r3,r5
  000c4ed8: str.w r8,[sp,#0x8]
  000c4edc: mov r0,r6
  000c4ede: blx 0x00073e88
  000c4ee2: ldr.w r1,[r6,#0xf8]
  000c4ee6: ldr r1,[r1,#0x4]
  000c4ee8: str.w r0,[r1,r4,lsl #0x2]
  000c4eec: ldr.w r0,[r6,#0xf8]
  000c4ef0: ldr r0,[r0,#0x4]
  000c4ef2: ldr.w r0,[r0,r4,lsl #0x2]
  000c4ef6: blx 0x000732dc
  000c4efa: b 0x000c4f22
  000c4efc: movs r1,#0x0
  000c4efe: blx 0x00073ec4
  000c4f02: strd r0,r9,[sp,#0x0]
  000c4f06: movs r1,#0x8
  000c4f08: ldr r6,[sp,#0x48]
  000c4f0a: movs r2,#0x0
  000c4f0c: mov r3,r5
  000c4f0e: str.w r8,[sp,#0x8]
  000c4f12: mov r0,r6
  000c4f14: blx 0x00073e88
  000c4f18: ldr.w r1,[r6,#0xf8]
  000c4f1c: ldr r1,[r1,#0x4]
  000c4f1e: str.w r0,[r1,r4,lsl #0x2]
  000c4f22: ldr r0,[sp,#0x48]
  000c4f24: movs r1,#0x1
  000c4f26: ldr.w r0,[r0,#0xf8]
  000c4f2a: ldr r0,[r0,#0x4]
  000c4f2c: ldr.w r0,[r0,r4,lsl #0x2]
  000c4f30: ldr r0,[r0,#0x4]
  000c4f32: blx 0x00073e94
  000c4f36: adds r4,#0x1
  000c4f38: cmp r4,#0x8
  000c4f3a: bne 0x000c4eb0
  000c4f3c: mov.w r0,#0x138
  000c4f40: blx 0x0006eb24
  000c4f44: ldr r5,[0x000c52e4]
  000c4f46: mov r6,r0
  000c4f48: ldr r3,[0x000c52e8]
  000c4f4a: sub.w r1,r5,#0x1f40
  000c4f4e: movs r4,#0x0
  000c4f50: movs r2,#0x0
  000c4f52: str r4,[sp,#0x0]
  000c4f54: blx 0x00073e4c
  000c4f58: mov.w r0,#0x138
  000c4f5c: blx 0x0006eb24
  000c4f60: mov r9,r0
  000c4f62: ldr r3,[0x000c52ec]
  000c4f64: mov r1,r5
  000c4f66: movs r2,#0x0
  000c4f68: str r4,[sp,#0x0]
  000c4f6a: blx 0x00073e4c
  000c4f6e: mov.w r0,#0x138
  000c4f72: mov r4,r6
  000c4f74: blx 0x0006eb24
  000c4f78: mov r10,r0
  000c4f7a: ldr r0,[0x000c52f0]
  000c4f7c: ldr r3,[0x000c52f4]
  000c4f7e: add.w r1,r0,#0xfa0
  000c4f82: mov.w r8,#0x0
  000c4f86: mov r0,r10
  000c4f88: movw r2,#0xcf08
  000c4f8c: str.w r8,[sp,#0x0]
  000c4f90: blx 0x00073e4c
  000c4f94: ldr r0,[0x000c52f8]
  000c4f96: mov r1,r4
  000c4f98: ldr r6,[sp,#0x48]
  000c4f9a: movw r2,#0x4961
  000c4f9e: add r0,pc
  000c4fa0: movs r3,#0x1
  000c4fa2: movs r5,#0x1
  000c4fa4: ldr.w r11,[r0,#0x0]
  000c4fa8: mov r0,r6
  000c4faa: blx r11
  000c4fac: ldr.w r1,[r6,#0xf8]
  000c4fb0: movw r2,#0x4961
  000c4fb4: movs r3,#0x1
  000c4fb6: ldr r1,[r1,#0x4]
  000c4fb8: str r0,[r1,#0x20]
  000c4fba: mov r0,r6
  000c4fbc: mov r1,r9
  000c4fbe: blx r11
  000c4fc0: ldr.w r1,[r6,#0xf8]
  000c4fc4: movw r2,#0x4961
  000c4fc8: movs r3,#0x1
  000c4fca: ldr r1,[r1,#0x4]
  000c4fcc: str r0,[r1,#0x24]
  000c4fce: mov r0,r6
  000c4fd0: mov r1,r10
  000c4fd2: blx r11
  000c4fd4: ldr.w r1,[r6,#0xf8]
  000c4fd8: movs r6,#0x20
  000c4fda: ldr r4,[sp,#0x48]
  000c4fdc: ldr r1,[r1,#0x4]
  000c4fde: str r0,[r1,#0x28]
  000c4fe0: b 0x000c504e
  000c4fe2: ldr.w r0,[r4,#0xf8]
  000c4fe6: movs r1,#0x0
  000c4fe8: ldr r0,[r0,#0x4]
  000c4fea: ldr r0,[r0,r6]
  000c4fec: blx 0x00073f54
  000c4ff0: ldr.w r0,[r4,#0xf8]
  000c4ff4: movs r1,#0x3
  000c4ff6: ldr r0,[r0,#0x4]
  000c4ff8: ldr r0,[r0,r6]
  000c4ffa: blx 0x00072748
  000c4ffe: ldr.w r0,[r4,#0xf8]
  000c5002: movs r1,#0x0
  000c5004: ldr r0,[r0,#0x4]
  000c5006: ldr r0,[r0,r6]
  000c5008: ldr r0,[r0,#0x4]
  000c500a: blx 0x00073e94
  000c500e: ldr.w r0,[r4,#0xf8]
  000c5012: movs r1,#0x1
  000c5014: ldr r0,[r0,#0x4]
  000c5016: ldr r0,[r0,r6]
  000c5018: ldr r0,[r0,#0x4]
  000c501a: blx 0x00073fb4
  000c501e: ldr.w r0,[r4,#0xf8]
  000c5022: ldr r0,[r0,#0x4]
  000c5024: ldr r1,[r0,r6]
  000c5026: ldr.w r0,[r1,#0x50]!
  000c502a: cmp r0,#0x0
  000c502c: strb.w r5,[r1,#0x24]
  000c5030: beq 0x000c5048
  000c5032: blx 0x00070204
  000c5036: blx 0x0006eb48
  000c503a: ldr r0,[sp,#0x48]
  000c503c: ldr.w r0,[r0,#0xf8]
  000c5040: ldr r0,[r0,#0x4]
  000c5042: ldr r0,[r0,r6]
  000c5044: add.w r1,r0,#0x50
  000c5048: str.w r8,[r1,#0x0]
  000c504c: adds r6,#0x4
  000c504e: cmp r6,#0x2c
  000c5050: bne 0x000c4fe2
  000c5052: ldr r0,[0x000c52fc]
  000c5054: mov.w r10,#0xb
  000c5058: ldr r5,[0x000c52e4]
  000c505a: mov.w r8,#0x0
  000c505e: vldr.32 s16,[pc,#0x2a0]
  000c5062: mov r11,r0
  000c5064: ldr r0,[0x000c5304]
  000c5066: ldr r6,[sp,#0x48]
  000c5068: add r0,pc
  000c506a: vldr.32 s18,[pc,#0x29c]
  000c506e: vldr.32 s20,[pc,#0x29c]
  000c5072: ldr r0,[r0,#0x0]
  000c5074: str r0,[sp,#0x3c]
  000c5076: b 0x000c515e
  000c5078: mov r0,r6
  000c507a: movs r1,#0x0
  000c507c: movw r2,#0x4962
  000c5080: movs r3,#0x1
  000c5082: blx 0x00073f6c
  000c5086: ldr.w r1,[r6,#0xf8]
  000c508a: ldr r1,[r1,#0x4]
  000c508c: str.w r0,[r1,r10,lsl #0x2]
  000c5090: movs r1,#0x0
  000c5092: ldr.w r0,[r6,#0xf8]
  000c5096: ldr r0,[r0,#0x4]
  000c5098: ldr.w r0,[r0,r10,lsl #0x2]
  000c509c: blx 0x00073f54
  000c50a0: ldr.w r0,[r6,#0xf8]
  000c50a4: ldr r1,[r0,#0x4]
  000c50a6: ldr r0,[sp,#0x3c]
  000c50a8: ldr.w r4,[r1,r10,lsl #0x2]
  000c50ac: movw r1,#0x5dc
  000c50b0: ldr r0,[r0,#0x0]
  000c50b2: blx 0x00071848
  000c50b6: add r0,r8
  000c50b8: mov r1,r11
  000c50ba: vmov s0,r5
  000c50be: mov r9,r5
  000c50c0: vmov s2,r1
  000c50c4: vmov s4,r0
  000c50c8: vcvt.f32.s32 s0,s0
  000c50cc: vcvt.f32.s32 s2,s2
  000c50d0: vcvt.f32.s32 s4,s4
  000c50d4: ldr r0,[r4,#0x0]
  000c50d6: ldr r5,[r0,#0x48]
  000c50d8: mov r0,r4
  000c50da: vmov r1,s0
  000c50de: vmov r3,s2
  000c50e2: vmov r2,s4
  000c50e6: blx r5
  000c50e8: vmov s0,r10
  000c50ec: vcvt.f32.s32 s0,s0
  000c50f0: ldr.w r0,[r6,#0xf8]
  000c50f4: ldr r0,[r0,#0x4]
  000c50f6: ldr.w r0,[r0,r10,lsl #0x2]
  000c50fa: vadd.f32 s2,s0,s16
  000c50fe: vadd.f32 s4,s0,s18
  000c5102: vadd.f32 s0,s0,s20
  000c5106: ldr r0,[r0,#0x8]
  000c5108: vmov r1,s2
  000c510c: vmov r2,s4
  000c5110: vmov r3,s0
  000c5114: blx 0x00072160
  000c5118: ldr.w r0,[r6,#0xf8]
  000c511c: movs r2,#0x1
  000c511e: ldr r0,[r0,#0x4]
  000c5120: ldr.w r1,[r0,r10,lsl #0x2]
  000c5124: ldr.w r0,[r1,#0x50]!
  000c5128: cmp r0,#0x0
  000c512a: strb.w r2,[r1,#0x24]
  000c512e: beq 0x000c5148
  000c5130: blx 0x00070204
  000c5134: blx 0x0006eb48
  000c5138: ldr r0,[sp,#0x48]
  000c513a: ldr.w r0,[r0,#0xf8]
  000c513e: ldr r0,[r0,#0x4]
  000c5140: ldr.w r0,[r0,r10,lsl #0x2]
  000c5144: add.w r1,r0,#0x50
  000c5148: movs r0,#0x0
  000c514a: mov r5,r9
  000c514c: str r0,[r1,#0x0]
  000c514e: sub.w r11,r11,#0xfa0
  000c5152: add.w r5,r9,#0xfa0
  000c5156: sub.w r8,r8,#0xc8
  000c515a: add.w r10,r10,#0x1
  000c515e: cmp.w r10,#0xf
  000c5162: bne 0x000c5078
  000c5164: movs r0,#0x1c
  000c5166: blx 0x0006eb24
  000c516a: mov r5,r0
  000c516c: ldr r3,[sp,#0x48]
  000c516e: movs r1,#0x4
  000c5170: movs r2,#0x7
  000c5172: blx 0x00073edc
  000c5176: b.w 0x000cb9b8
  000c517a: movs r0,#0xc
  000c517c: blx 0x0006eb24
  000c5180: mov r5,r0
  000c5182: blx 0x00073d08
  000c5186: ldr r0,[sp,#0x48]
  000c5188: mov r1,r5
  000c518a: str.w r5,[r0,#0xf8]
  000c518e: movs r0,#0x5
  000c5190: blx 0x00073d14
  000c5194: ldr r5,[sp,#0x48]
  000c5196: movs r4,#0x0
  000c5198: mov.w r8,#0x1
  000c519c: movs r6,#0x0
  000c519e: b 0x000c51e2
  000c51a0: mov r0,r5
  000c51a2: movs r1,#0x1
  000c51a4: movs r2,#0x0
  000c51a6: movs r3,#0x9
  000c51a8: strd r4,r8,[sp,#0x0]
  000c51ac: str r4,[sp,#0x8]
  000c51ae: blx 0x00073e88
  000c51b2: ldr.w r1,[r5,#0xf8]
  000c51b6: ldr r1,[r1,#0x4]
  000c51b8: str.w r0,[r1,r6,lsl #0x2]
  000c51bc: movs r1,#0x0
  000c51be: ldr.w r0,[r5,#0xf8]
  000c51c2: ldr r0,[r0,#0x4]
  000c51c4: ldr.w r0,[r0,r6,lsl #0x2]
  000c51c8: ldr r0,[r0,#0x4]
  000c51ca: blx 0x00073e94
  000c51ce: ldr.w r0,[r5,#0xf8]
  000c51d2: movs r1,#0x1
  000c51d4: ldr r0,[r0,#0x4]
  000c51d6: ldr.w r0,[r0,r6,lsl #0x2]
  000c51da: ldr r0,[r0,#0x4]
  000c51dc: blx 0x00073fb4
  000c51e0: adds r6,#0x1
  000c51e2: ldr r0,[sp,#0x48]
  000c51e4: ldr.w r0,[r0,#0xf8]
  000c51e8: ldr r0,[r0,#0x0]
  000c51ea: cmp r6,r0
  000c51ec: bcc 0x000c51a0
  000c51ee: b.w 0x000cb9bc
  000c51f2: movs r0,#0xc
  000c51f4: blx 0x0006eb24
  000c51f8: mov r5,r0
  000c51fa: blx 0x00073d08
  000c51fe: ldr r0,[sp,#0x48]
  000c5200: mov r1,r5
  000c5202: str.w r5,[r0,#0xf8]
  000c5206: movs r0,#0x3
  000c5208: blx 0x00073d14
  000c520c: ldr r0,[0x000c5310]
  000c520e: mov.w r9,#0x0
  000c5212: ldr r4,[sp,#0x48]
  000c5214: mov.w r8,#0x1
  000c5218: movs r6,#0x0
  000c521a: mov r10,r0
  000c521c: b 0x000c52ae
  000c521e: movs r3,#0x2
  000c5220: strd r9,r8,[sp,#0x0]
  000c5224: str.w r9,[sp,#0x8]
  000c5228: cmp r6,#0x1
  000c522a: it eq
  000c522c: mov.eq r3,#0x17
  000c522e: mov r0,r4
  000c5230: movs r1,#0x8
  000c5232: movs r2,#0x0
  000c5234: blx 0x00073e88
  000c5238: ldr.w r1,[r4,#0xf8]
  000c523c: ldr r1,[r1,#0x4]
  000c523e: str.w r0,[r1,r6,lsl #0x2]
  000c5242: ldr.w r0,[r4,#0xf8]
  000c5246: ldr r0,[r0,#0x4]
  000c5248: ldr.w r0,[r0,r6,lsl #0x2]
  000c524c: blx 0x000732dc
  000c5250: ldr.w r0,[r4,#0xf8]
  000c5254: movs r1,#0x1
  000c5256: ldr r0,[r0,#0x4]
  000c5258: ldr.w r0,[r0,r6,lsl #0x2]
  000c525c: ldr r0,[r0,#0x4]
  000c525e: blx 0x00073e94
  000c5262: ldr.w r0,[r4,#0xf8]
  000c5266: mov r2,r10
  000c5268: mov r3,r10
  000c526a: ldr r0,[r0,#0x4]
  000c526c: ldr.w r0,[r0,r6,lsl #0x2]
  000c5270: ldr r1,[r0,#0x0]
  000c5272: ldr r5,[r1,#0x48]
  000c5274: mov r1,r10
  000c5276: blx r5
  000c5278: ldr.w r0,[r4,#0xf8]
  000c527c: ldr r0,[r0,#0x4]
  000c527e: ldr.w r1,[r0,r6,lsl #0x2]
  000c5282: str.w r9,[r1,#0x50]
  000c5286: strb.w r9,[r1,#0x4c]
  000c528a: movs r1,#0x96
  000c528c: ldr.w r0,[r0,r6,lsl #0x2]
  000c5290: ldr r0,[r0,#0x4]
  000c5292: blx 0x000727e4
  000c5296: cmp r6,#0x2
  000c5298: bhi 0x000c52ac
  000c529a: ldr r0,[sp,#0x48]
  000c529c: movs r1,#0x0
  000c529e: ldr.w r0,[r0,#0xf8]
  000c52a2: ldr r0,[r0,#0x4]
  000c52a4: ldr.w r0,[r0,r6,lsl #0x2]
  000c52a8: blx 0x000721f0
  000c52ac: adds r6,#0x1
  000c52ae: ldr r0,[sp,#0x48]
  000c52b0: ldr.w r0,[r0,#0xf8]
  000c52b4: ldr r0,[r0,#0x0]
  000c52b6: cmp r6,r0
  000c52b8: bcc 0x000c521e
  000c52ba: ldr r0,[sp,#0x48]
  000c52bc: ldr r5,[0x000c5314]
  000c52be: mov r4,r0
  000c52c0: ldr.w r0,[r0,#0xf0]
  000c52c4: mov r1,r5
  000c52c6: ldr r0,[r0,#0x0]
  000c52c8: blx 0x000727e4
  000c52cc: ldr r0,[sp,#0x44]
  000c52ce: movs r1,#0x1
  000c52d0: ldr r0,[r0,#0x0]
  000c52d2: str r5,[r0,#0x64]
  000c52d4: ldr.w r0,[r4,#0xf0]
  000c52d8: ldr r0,[r0,#0x0]
  000c52da: strb.w r1,[r0,#0x5e]
  000c52de: b.w 0x000cb9bc
  000c5318: movs r0,#0xc
  000c531a: blx 0x0006eb24
  000c531e: mov r5,r0
  000c5320: blx 0x00073d08
  000c5324: ldr r6,[sp,#0x48]
  000c5326: movs r0,#0x1
  000c5328: mov r1,r5
  000c532a: movs r4,#0x1
  000c532c: str.w r5,[r6,#0xf8]
  000c5330: blx 0x00073d14
  000c5334: movs r0,#0x0
  000c5336: movs r1,#0x3
  000c5338: strd r0,r4,[sp,#0x0]
  000c533c: movs r2,#0x0
  000c533e: str r0,[sp,#0x8]
  000c5340: mov r0,r6
  000c5342: movs r3,#0x1e
  000c5344: blx 0x00073e88
  000c5348: ldr.w r1,[r6,#0xf8]
  000c534c: ldr r2,[0x000c565c]
  000c534e: ldr r1,[r1,#0x4]
  000c5350: str r0,[r1,#0x0]
  000c5352: ldr.w r0,[r6,#0xf8]
  000c5356: ldr r1,[0x000c5660]
  000c5358: ldr r0,[r0,#0x4]
  000c535a: ldr r0,[r0,#0x0]
  000c535c: ldr r3,[r0,#0x0]
  000c535e: ldr r6,[r3,#0x48]
  000c5360: ldr r3,[0x000c5664]
  000c5362: blx r6
  000c5364: ldr r0,[0x000c5668]
  000c5366: add r6,sp,#0x180
  000c5368: add r0,pc
  000c536a: vld1.64 {d16,d17},[r0]!
  000c536e: vldr.64 d18,[r0]
  000c5372: mov r0,r6
  000c5374: vst1.64 {d16,d17},[r0]!
  000c5378: vstr.64 d18,[r0]
  000c537c: movs r0,#0x18
  000c537e: blx 0x0006eb24
  000c5382: mov r5,r0
  000c5384: mov r1,r6
  000c5386: movs r2,#0x6
  000c5388: blx 0x00073eb8
  000c538c: ldr r0,[sp,#0x48]
  000c538e: mov r1,r5
  000c5390: mov r4,r0
  000c5392: ldr.w r0,[r0,#0xf8]
  000c5396: str.w r5,[r4,#0x110]
  000c539a: ldr r0,[r0,#0x4]
  000c539c: ldr r0,[r0,#0x0]
  000c539e: blx 0x00073f60
  000c53a2: ldr.w r0,[r4,#0xf8]
  000c53a6: ldr r0,[r0,#0x4]
  000c53a8: ldr r0,[r0,#0x0]
  000c53aa: ldr r1,[r0,#0x0]
  000c53ac: ldr r2,[r1,#0x1c]
  000c53ae: movs r1,#0x0
  000c53b0: blx r2
  000c53b2: ldr.w r0,[r4,#0xf8]
  000c53b6: movs r1,#0x0
  000c53b8: ldr r0,[r0,#0x4]
  000c53ba: ldr r0,[r0,#0x0]
  000c53bc: blx 0x000721f0
  000c53c0: b.w 0x000cb9bc
  000c53c4: movs r0,#0xc
  000c53c6: blx 0x0006eb24
  000c53ca: mov r5,r0
  000c53cc: blx 0x00073d08
  000c53d0: ldr r4,[sp,#0x48]
  000c53d2: movs r0,#0x1
  000c53d4: mov r1,r5
  000c53d6: movs r6,#0x1
  000c53d8: str.w r5,[r4,#0xf8]
  000c53dc: blx 0x00073d14
  000c53e0: movs r0,#0x0
  000c53e2: movs r1,#0x8
  000c53e4: strd r0,r6,[sp,#0x0]
  000c53e8: movs r2,#0x0
  000c53ea: str r0,[sp,#0x8]
  000c53ec: mov r0,r4
  000c53ee: movs r3,#0x2
  000c53f0: blx 0x00073e88
  000c53f4: ldr.w r1,[r4,#0xf8]
  000c53f8: ldr r1,[r1,#0x4]
  000c53fa: str r0,[r1,#0x0]
  000c53fc: movs r1,#0x0
  000c53fe: ldr.w r0,[r4,#0xf8]
  000c5402: ldr r0,[r0,#0x4]
  000c5404: ldr r0,[r0,#0x0]
  000c5406: blx 0x00074128
  000c540a: ldr.w r0,[r4,#0xf8]
  000c540e: movs r1,#0x1
  000c5410: ldr r0,[r0,#0x4]
  000c5412: ldr r0,[r0,#0x0]
  000c5414: ldr r0,[r0,#0x4]
  000c5416: blx 0x00073e94
  000c541a: ldr.w r0,[r4,#0xf8]
  000c541e: movs r2,#0x0
  000c5420: ldr r3,[0x000c566c]
  000c5422: ldr r0,[r0,#0x4]
  000c5424: ldr r0,[r0,#0x0]
  000c5426: ldr r1,[r0,#0x0]
  000c5428: ldr r6,[r1,#0x48]
  000c542a: movs r1,#0x0
  000c542c: blx r6
  000c542e: ldr.w r0,[r4,#0xf8]
  000c5432: ldr r0,[r0,#0x4]
  000c5434: ldr r0,[r0,#0x0]
  000c5436: blx 0x000732dc
  000c543a: b.w 0x000cb9bc
  000c543e: ldr r0,[0x000c5670]
  000c5440: add r6,sp,#0x180
  000c5442: add r0,pc
  000c5444: vld1.64 {d16,d17},[r0]!
  000c5448: vldr.64 d18,[r0]
  000c544c: mov r0,r6
  000c544e: vst1.64 {d16,d17},[r0]!
  000c5452: vstr.64 d18,[r0]
  000c5456: movs r0,#0x18
  000c5458: blx 0x0006eb24
  000c545c: mov r5,r0
  000c545e: mov r1,r6
  000c5460: movs r2,#0x6
  000c5462: blx 0x00073eb8
  000c5466: ldr r0,[sp,#0x48]
  000c5468: str.w r5,[r0,#0x108]
  000c546c: movs r0,#0xc
  000c546e: blx 0x0006eb24
  000c5472: mov r5,r0
  000c5474: blx 0x00073d08
  000c5478: ldr r0,[sp,#0x48]
  000c547a: mov r1,r5
  000c547c: str.w r5,[r0,#0xf8]
  000c5480: movs r0,#0x4
  000c5482: blx 0x00073d14
  000c5486: ldr r4,[sp,#0x48]
  000c5488: mov.w r8,#0x0
  000c548c: movs r5,#0x1
  000c548e: movs r6,#0x0
  000c5490: b 0x000c54c6
  000c5492: ldr.w r0,[r4,#0x108]
  000c5496: movs r1,#0x1
  000c5498: blx 0x00073ec4
  000c549c: stm sp,{r0,r5,r8}
  000c54a0: mov r0,r4
  000c54a2: movs r1,#0x8
  000c54a4: movs r2,#0x0
  000c54a6: movs r3,#0x2
  000c54a8: blx 0x00073e88
  000c54ac: ldr.w r1,[r4,#0xf8]
  000c54b0: ldr r1,[r1,#0x4]
  000c54b2: str.w r0,[r1,r6,lsl #0x2]
  000c54b6: ldr.w r0,[r4,#0xf8]
  000c54ba: ldr r0,[r0,#0x4]
  000c54bc: ldr.w r0,[r0,r6,lsl #0x2]
  000c54c0: blx 0x000732dc
  000c54c4: adds r6,#0x1
  000c54c6: cmp r6,#0x3
  000c54c8: bne 0x000c5492
  000c54ca: movs r0,#0x0
  000c54cc: movs r1,#0x1
  000c54ce: strd r0,r1,[sp,#0x0]
  000c54d2: movs r1,#0x3
  000c54d4: ldr r4,[sp,#0x48]
  000c54d6: movs r2,#0x0
  000c54d8: str r0,[sp,#0x8]
  000c54da: movs r3,#0x1e
  000c54dc: mov r0,r4
  000c54de: blx 0x00073e88
  000c54e2: ldr.w r1,[r4,#0xf8]
  000c54e6: ldr r1,[r1,#0x4]
  000c54e8: str r0,[r1,#0xc]
  000c54ea: movs r1,#0x1
  000c54ec: ldr.w r0,[r4,#0xf8]
  000c54f0: ldr r0,[r0,#0x4]
  000c54f2: ldr r0,[r0,#0xc]
  000c54f4: ldr r0,[r0,#0x4]
  000c54f6: blx 0x00073fb4
  000c54fa: ldr.w r0,[r4,#0xf8]
  000c54fe: vldr.32 s0,[pc,#0x174]
  000c5502: vldr.32 s2,[sp,#0x10c]
  000c5506: ldr r0,[r0,#0x4]
  000c5508: vldr.32 s4,[sp,#0x110]
  000c550c: vadd.f32 s0,s2,s0
  000c5510: vldr.32 s10,[pc,#0x148]
  000c5514: vldr.32 s6,[sp,#0x114]
  000c5518: vldr.32 s8,[pc,#0x15c]
  000c551c: vadd.f32 s2,s4,s10
  000c5520: ldr r0,[r0,#0xc]
  000c5522: vadd.f32 s4,s6,s8
  000c5526: ldr r3,[r0,#0x0]
  000c5528: vmov r1,s0
  000c552c: ldr r6,[r3,#0x48]
  000c552e: vmov r2,s2
  000c5532: vmov r3,s4
  000c5536: blx r6
  000c5538: ldr.w r1,[r4,#0xf8]
  000c553c: ldr.w r0,[r4,#0x108]
  000c5540: ldr r1,[r1,#0x4]
  000c5542: ldr r5,[r1,#0xc]
  000c5544: blx 0x00073f3c
  000c5548: mov r1,r0
  000c554a: mov r0,r5
  000c554c: blx 0x00073f60
  000c5550: ldr.w r0,[r4,#0xf8]
  000c5554: ldr r1,[0x000c567c]
  000c5556: ldr r0,[r0,#0x4]
  000c5558: ldr r0,[r0,#0xc]
  000c555a: ldr r0,[r0,#0x4]
  000c555c: blx 0x000727e4
  000c5560: ldr r0,[0x000c5680]
  000c5562: movw r1,#0x63f
  000c5566: add r0,pc
  000c5568: ldr r0,[r0,#0x0]
  000c556a: ldr r0,[r0,#0x0]
  000c556c: blx 0x00072f70
  000c5570: mov r1,r0
  000c5572: ldr.w r0,[r4,#0xf8]
  000c5576: ldr r0,[r0,#0x4]
  000c5578: ldr r0,[r0,#0xc]
  000c557a: adds r0,#0x18
  000c557c: blx 0x0006f2b0
  000c5580: ldr.w r0,[r4,#0xf8]
  000c5584: movs r1,#0x0
  000c5586: ldr r0,[r0,#0x4]
  000c5588: ldr r0,[r0,#0xc]
  000c558a: blx 0x00074134
  000c558e: movs r0,#0x1c
  000c5590: blx 0x0006eb24
  000c5594: mov r5,r0
  000c5596: movs r1,#0x12
  000c5598: movs r2,#0x0
  000c559a: movs r3,#0x3
  000c559c: str r4,[sp,#0x0]
  000c559e: blx 0x00073f9c
  000c55a2: b.w 0x000cb9b8
  000c55a6: ldr r0,[0x000c5684]
  000c55a8: add r0,pc
  000c55aa: vldr.64 d16,[r0]
  000c55ae: ldr r0,[r0,#0x8]
  000c55b0: str r0,[sp,#0x188]
  000c55b2: movs r0,#0x18
  000c55b4: vstr.64 d16,[sp,#0x180]
  000c55b8: blx 0x0006eb24
  000c55bc: mov r5,r0
  000c55be: add r1,sp,#0x180
  000c55c0: movs r2,#0x3
  000c55c2: blx 0x00073eb8
  000c55c6: ldr r0,[sp,#0x48]
  000c55c8: str.w r5,[r0,#0x110]
  000c55cc: movs r0,#0xc
  000c55ce: blx 0x0006eb24
  000c55d2: mov r5,r0
  000c55d4: blx 0x00073d08
  000c55d8: ldr r0,[sp,#0x48]
  000c55da: mov r1,r5
  000c55dc: str.w r5,[r0,#0xf8]
  000c55e0: movs r0,#0x7
  000c55e2: blx 0x00073d14
  000c55e6: ldr r4,[sp,#0x48]
  000c55e8: mov.w r8,#0x0
  000c55ec: movs r5,#0x1
  000c55ee: movs r6,#0x0
  000c55f0: b 0x000c5616
  000c55f2: ldr.w r0,[r4,#0x110]
  000c55f6: blx 0x000728bc
  000c55fa: stm sp,{r0,r5,r8}
  000c55fe: mov r0,r4
  000c5600: movs r1,#0x8
  000c5602: movs r2,#0x0
  000c5604: movs r3,#0x0
  000c5606: blx 0x00073e88
  000c560a: ldr.w r1,[r4,#0xf8]
  000c560e: ldr r1,[r1,#0x4]
  000c5610: str.w r0,[r1,r6,lsl #0x2]
  000c5614: adds r6,#0x1
  000c5616: cmp r6,#0x3
  000c5618: bne 0x000c55f2
  000c561a: movs r4,#0x3
  000c561c: mov.w r8,#0x0
  000c5620: movs r6,#0x1
  000c5622: ldr r5,[sp,#0x48]
  000c5624: b 0x000c564a
  000c5626: ldr.w r0,[r5,#0x110]
  000c562a: blx 0x000728bc
  000c562e: stm sp,{r0,r6,r8}
  000c5632: mov r0,r5
  000c5634: movs r1,#0x0
  000c5636: movs r2,#0x0
  000c5638: movs r3,#0x5
  000c563a: blx 0x00073e88
  000c563e: ldr.w r1,[r5,#0xf8]
  000c5642: ldr r1,[r1,#0x4]
  000c5644: str.w r0,[r1,r4,lsl #0x2]
  000c5648: adds r4,#0x1
  000c564a: cmp r4,#0x5
  000c564c: bne 0x000c5626
  000c564e: ldr r5,[sp,#0x48]
  000c5650: movs r4,#0x5
  000c5652: mov.w r8,#0x0
  000c5656: movs r6,#0x1
  000c5658: b 0x000c56b8
  000c5694: ldr.w r0,[r5,#0x110]
  000c5698: blx 0x000728bc
  000c569c: stm sp,{r0,r6,r8}
  000c56a0: mov r0,r5
  000c56a2: movs r1,#0x0
  000c56a4: movs r2,#0x1
  000c56a6: movs r3,#0xe
  000c56a8: blx 0x00073e88
  000c56ac: ldr.w r1,[r5,#0xf8]
  000c56b0: ldr r1,[r1,#0x4]
  000c56b2: str.w r0,[r1,r4,lsl #0x2]
  000c56b6: adds r4,#0x1
  000c56b8: cmp r4,#0x7
  000c56ba: bne 0x000c5694
  000c56bc: ldr r4,[sp,#0x48]
  000c56be: ldr.w r0,[r4,#0x110]
  000c56c2: blx 0x000728bc
  000c56c6: mov r1,r0
  000c56c8: ldr r0,[r0,#0x0]
  000c56ca: add r5,sp,#0x140
  000c56cc: ldr r2,[r0,#0x28]
  000c56ce: mov r0,r5
  000c56d0: blx r2
  000c56d2: vldr.32 s16,[pc,#-0x4c]
  000c56d6: vldr.32 s0,[sp,#0x140]
  000c56da: vadd.f32 s0,s0,s16
  000c56de: vstr.32 s0,[sp,#0x140]
  000c56e2: ldr.w r0,[r4,#0xf8]
  000c56e6: ldrd r1,r0,[r0,#0x0]
  000c56ea: add.w r0,r0,r1, lsl #0x2
  000c56ee: ldr.w r0,[r0,#-0x8]
  000c56f2: ldr r1,[r0,#0x0]
  000c56f4: ldr r2,[r1,#0x44]
  000c56f6: mov r1,r5
  000c56f8: blx r2
  000c56fa: ldr.w r0,[r4,#0x110]
  000c56fe: blx 0x000728bc
  000c5702: mov r1,r0
  000c5704: ldr r0,[r0,#0x0]
  000c5706: add r6,sp,#0x118
  000c5708: ldr r2,[r0,#0x28]
  000c570a: mov r0,r6
  000c570c: blx r2
  000c570e: mov r0,r5
  000c5710: mov r1,r6
  000c5712: blx 0x0006eb3c
  000c5716: vldr.32 s0,[pc,#-0x8c]
  000c571a: vldr.32 s2,[sp,#0x140]
  000c571e: vldr.32 s4,[sp,#0x144]
  000c5722: vldr.32 s6,[sp,#0x148]
  000c5726: vadd.f32 s0,s2,s0
  000c572a: vldr.32 s8,[pc,#-0x9c]
  000c572e: vadd.f32 s2,s4,s8
  000c5732: vadd.f32 s4,s6,s16
  000c5736: vstr.32 s0,[sp,#0x140]
  000c573a: vstr.32 s2,[sp,#0x144]
  000c573e: vstr.32 s4,[sp,#0x148]
  000c5742: ldr.w r0,[r4,#0xf8]
  000c5746: ldrd r1,r0,[r0,#0x0]
  000c574a: add.w r0,r0,r1, lsl #0x2
  000c574e: ldr.w r0,[r0,#-0x4]
  000c5752: ldr r1,[r0,#0x0]
  000c5754: ldr r2,[r1,#0x44]
  000c5756: mov r1,r5
  000c5758: blx r2
  000c575a: ldr r1,[0x000c5a2c]
  000c575c: movs r2,#0x0
  000c575e: ldr r3,[0x000c5a30]
  000c5760: ldr.w r0,[r4,#0xf0]
  000c5764: blx 0x000725bc
  000c5768: movs r0,#0x1c
  000c576a: blx 0x0006eb24
  000c576e: mov r5,r0
  000c5770: movs r1,#0x1f
  000c5772: movs r2,#0x0
  000c5774: mov r3,r4
  000c5776: blx 0x00073edc
  000c577a: b.w 0x000cb9b8
  000c577e: ldr r0,[0x000c5a34]
  000c5780: add r0,pc
  000c5782: vldr.64 d16,[r0]
  000c5786: ldr r0,[r0,#0x8]
  000c5788: str r0,[sp,#0x188]
  000c578a: movs r0,#0x18
  000c578c: vstr.64 d16,[sp,#0x180]
  000c5790: blx 0x0006eb24
  000c5794: mov r5,r0
  000c5796: add r1,sp,#0x180
  000c5798: movs r2,#0x3
  000c579a: blx 0x00073eb8
  000c579e: ldr r0,[sp,#0x48]
  000c57a0: str.w r5,[r0,#0x110]
  000c57a4: movs r0,#0xc
  000c57a6: blx 0x0006eb24
  000c57aa: mov r5,r0
  000c57ac: blx 0x00073d08
  000c57b0: ldr r0,[sp,#0x48]
  000c57b2: mov r1,r5
  000c57b4: mov r4,r0
  000c57b6: str.w r5,[r0,#0xf8]
  000c57ba: movs r0,#0xa
  000c57bc: blx 0x00073d14
  000c57c0: ldr.w r0,[r4,#0x110]
  000c57c4: blx 0x000728bc
  000c57c8: ldr r4,[sp,#0x48]
  000c57ca: mov r8,r0
  000c57cc: mov.w r10,#0x0
  000c57d0: mov.w r9,#0x1
  000c57d4: movs r5,#0x0
  000c57d6: b 0x000c586a
  000c57d8: ldr.w r0,[r4,#0x110]
  000c57dc: blx 0x000728bc
  000c57e0: stm sp,{r0,r9,r10}
  000c57e4: mov r0,r4
  000c57e6: movs r1,#0x0
  000c57e8: movs r2,#0x1
  000c57ea: movs r3,#0xf
  000c57ec: blx 0x00073e88
  000c57f0: ldr.w r1,[r4,#0xf8]
  000c57f4: ldr r1,[r1,#0x4]
  000c57f6: str.w r0,[r1,r5,lsl #0x2]
  000c57fa: movs r1,#0x1
  000c57fc: ldr.w r0,[r4,#0xf8]
  000c5800: ldr r0,[r0,#0x4]
  000c5802: ldr.w r0,[r0,r5,lsl #0x2]
  000c5806: ldr r0,[r0,#0x4]
  000c5808: blx 0x00073fb4
  000c580c: ldr.w r0,[r4,#0xf8]
  000c5810: movs r1,#0x0
  000c5812: ldr r0,[r0,#0x4]
  000c5814: ldr.w r0,[r0,r5,lsl #0x2]
  000c5818: blx 0x00073f54
  000c581c: ldr.w r0,[r4,#0xf8]
  000c5820: ldr r0,[r0,#0x4]
  000c5822: ldr.w r0,[r0,r5,lsl #0x2]
  000c5826: ldr r6,[r0,#0x4]
  000c5828: mov r0,r6
  000c582a: blx 0x00072910
  000c582e: movs r1,#0x3
  000c5830: blx 0x0007198c
  000c5834: mov r1,r0
  000c5836: mov r0,r6
  000c5838: blx 0x00073f78
  000c583c: ldr.w r0,[r4,#0xf8]
  000c5840: ldr r0,[r0,#0x4]
  000c5842: ldr.w r1,[r0,r5,lsl #0x2]
  000c5846: ldr.w r0,[r1,#0x50]!
  000c584a: cbz r0,0x000c5864
  000c584c: blx 0x00070204
  000c5850: blx 0x0006eb48
  000c5854: ldr r0,[sp,#0x48]
  000c5856: ldr.w r0,[r0,#0xf8]
  000c585a: ldr r0,[r0,#0x4]
  000c585c: ldr.w r0,[r0,r5,lsl #0x2]
  000c5860: add.w r1,r0,#0x50
  000c5864: str.w r10,[r1,#0x0]
  000c5868: adds r5,#0x1
  000c586a: cmp r5,#0x3
  000c586c: bne 0x000c57d8
  000c586e: vldr.32 s0,[r8,#0x124]
  000c5872: vldr.32 s2,[r8,#0x128]
  000c5876: vldr.32 s4,[r8,#0x12c]
  000c587a: vcvt.f32.s32 s0,s0
  000c587e: vcvt.f32.s32 s2,s2
  000c5882: vcvt.f32.s32 s4,s4
  000c5886: ldr r4,[sp,#0x48]
  000c5888: ldr.w r0,[r4,#0xf8]
  000c588c: vmov r1,s0
  000c5890: vmov r2,s2
  000c5894: ldr r0,[r0,#0x4]
  000c5896: vmov r3,s4
  000c589a: ldr r0,[r0,#0x0]
  000c589c: ldr r6,[r0,#0x0]
  000c589e: ldr r6,[r6,#0x48]
  000c58a0: blx r6
  000c58a2: ldr.w r0,[r4,#0xf8]
  000c58a6: ldr r0,[r0,#0x4]
  000c58a8: ldr r0,[r0,#0x0]
  000c58aa: ldr r6,[r0,#0x4]
  000c58ac: mov r0,r6
  000c58ae: blx 0x00072910
  000c58b2: movs r1,#0x6
  000c58b4: blx 0x0007198c
  000c58b8: mov r1,r0
  000c58ba: mov r0,r6
  000c58bc: blx 0x00073f78
  000c58c0: add.w r2,r8,#0x124
  000c58c4: ldmia r2,{r0,r1,r2}
  000c58c6: addw r0,r0,#0xbb8
  000c58ca: add.w r1,r1,#0x7d0
  000c58ce: vmov s0,r0
  000c58d2: subw r0,r2,#0xbb8
  000c58d6: vmov s2,r1
  000c58da: vmov s4,r0
  000c58de: vcvt.f32.s32 s0,s0
  000c58e2: vcvt.f32.s32 s2,s2
  000c58e6: vcvt.f32.s32 s4,s4
  000c58ea: ldr.w r0,[r4,#0xf8]
  000c58ee: ldr r0,[r0,#0x4]
  000c58f0: vmov r1,s0
  000c58f4: vmov r2,s2
  000c58f8: ldr r0,[r0,#0x4]
  000c58fa: vmov r3,s4
  000c58fe: ldr r6,[r0,#0x0]
  000c5900: ldr r6,[r6,#0x48]
  000c5902: blx r6
  000c5904: add.w r2,r8,#0x124
  000c5908: movw r3,#0x2328
  000c590c: ldmia r2,{r0,r1,r2}
  000c590e: subs r0,r0,r3
  000c5910: movw r3,#0x1b58
  000c5914: vmov s0,r0
  000c5918: subs r2,r2,r3
  000c591a: sub.w r0,r1,#0x1f40
  000c591e: vmov s2,r2
  000c5922: vmov s4,r0
  000c5926: vcvt.f32.s32 s0,s0
  000c592a: vcvt.f32.s32 s2,s2
  000c592e: vcvt.f32.s32 s4,s4
  000c5932: ldr.w r0,[r4,#0xf8]
  000c5936: ldr r0,[r0,#0x4]
  000c5938: vmov r1,s0
  000c593c: vmov r3,s2
  000c5940: ldr r0,[r0,#0x8]
  000c5942: vmov r2,s4
  000c5946: ldr r6,[r0,#0x0]
  000c5948: ldr r6,[r6,#0x48]
  000c594a: blx r6
  000c594c: ldr r0,[0x000c5a38]
  000c594e: movs r5,#0x3
  000c5950: movw r9,#0x2710
  000c5954: movw r11,#0xc350
  000c5958: add r0,pc
  000c595a: ldr.w r10,[r0,#0x0]
  000c595e: b 0x000c5a14
  000c5960: ldr r4,[sp,#0x48]
  000c5962: ldr.w r0,[r4,#0x110]
  000c5966: blx 0x000728bc
  000c596a: str r0,[sp,#0x0]
  000c596c: movs r0,#0x1
  000c596e: str r0,[sp,#0x4]
  000c5970: movs r0,#0x0
  000c5972: str r0,[sp,#0x8]
  000c5974: mov r0,r4
  000c5976: movs r1,#0x9
  000c5978: movs r2,#0x0
  000c597a: movs r3,#0x8
  000c597c: blx 0x00073e88
  000c5980: ldr.w r1,[r4,#0xf8]
  000c5984: ldr r1,[r1,#0x4]
  000c5986: str.w r0,[r1,r5,lsl #0x2]
  000c598a: ldr.w r0,[r4,#0xf8]
  000c598e: ldr r0,[r0,#0x4]
  000c5990: ldr.w r0,[r0,r5,lsl #0x2]
  000c5994: ldr r6,[r0,#0x4]
  000c5996: mov r0,r6
  000c5998: blx 0x00072910
  000c599c: add.w r0,r0,r0, lsl #0x2
  000c59a0: lsls r1,r0,#0x1
  000c59a2: mov r0,r6
  000c59a4: blx 0x00073f78
  000c59a8: ldr.w r0,[r4,#0xf8]
  000c59ac: ldr.w r4,[r8,#0x124]
  000c59b0: ldr r1,[r0,#0x4]
  000c59b2: ldr.w r0,[r10,#0x0]
  000c59b6: ldr.w r6,[r1,r5,lsl #0x2]
  000c59ba: movw r1,#0x4e20
  000c59be: blx 0x00071848
  000c59c2: adds r1,r4,r0
  000c59c4: ldr.w r0,[r10,#0x0]
  000c59c8: sub.w r1,r1,r9
  000c59cc: ldr.w r4,[r8,#0x128]
  000c59d0: vmov s0,r1
  000c59d4: movw r1,#0x4e20
  000c59d8: vcvt.f32.s32 s16,s0
  000c59dc: blx 0x00071848
  000c59e0: add r0,r4
  000c59e2: ldr.w r1,[r8,#0x12c]
  000c59e6: sub.w r0,r0,r9
  000c59ea: vmov s0,r0
  000c59ee: add.w r0,r1,r11
  000c59f2: vmov r1,s16
  000c59f6: vmov s2,r0
  000c59fa: vcvt.f32.s32 s0,s0
  000c59fe: vcvt.f32.s32 s2,s2
  000c5a02: ldr r0,[r6,#0x0]
  000c5a04: ldr r4,[r0,#0x48]
  000c5a06: mov r0,r6
  000c5a08: vmov r2,s0
  000c5a0c: vmov r3,s2
  000c5a10: blx r4
  000c5a12: adds r5,#0x1
  000c5a14: cmp r5,#0x7
  000c5a16: bne 0x000c5960
  000c5a18: ldr r0,[0x000c5a3c]
  000c5a1a: movs r5,#0x7
  000c5a1c: vldr.32 s16,[pc,#-0x390]
  000c5a20: mov.w r9,#0x0
  000c5a24: add r0,pc
  000c5a26: ldr.w r11,[r0,#0x0]
  000c5a2a: b 0x000c5b08
  000c5a40: movs r0,#0x1
  000c5a42: str.w r9,[sp,#0x0]
  000c5a46: strd r0,r9,[sp,#0x4]
  000c5a4a: movs r1,#0x0
  000c5a4c: ldr r4,[sp,#0x48]
  000c5a4e: movs r2,#0x0
  000c5a50: movs r3,#0x5
  000c5a52: mov r0,r4
  000c5a54: blx 0x00073e88
  000c5a58: ldr.w r1,[r4,#0xf8]
  000c5a5c: ldr r1,[r1,#0x4]
  000c5a5e: str.w r0,[r1,r5,lsl #0x2]
  000c5a62: movs r1,#0x1
  000c5a64: ldr.w r0,[r4,#0xf8]
  000c5a68: ldr r0,[r0,#0x4]
  000c5a6a: ldr.w r0,[r0,r5,lsl #0x2]
  000c5a6e: ldr r0,[r0,#0x4]
  000c5a70: blx 0x00073fb4
  000c5a74: ldr.w r0,[r4,#0xf8]
  000c5a78: mov.w r1,#0x258
  000c5a7c: ldr r0,[r0,#0x4]
  000c5a7e: ldr.w r0,[r0,r5,lsl #0x2]
  000c5a82: ldr r0,[r0,#0x4]
  000c5a84: blx 0x000727e4
  000c5a88: ldr.w r0,[r4,#0xf8]
  000c5a8c: vldr.32 s18,[sp,#0x10c]
  000c5a90: ldr r1,[r0,#0x4]
  000c5a92: ldr.w r0,[r10,#0x0]
  000c5a96: ldr.w r6,[r1,r5,lsl #0x2]
  000c5a9a: mov.w r1,#0xfa0
  000c5a9e: blx r11
  000c5aa0: sub.w r1,r0,#0x7d0
  000c5aa4: ldr.w r0,[r10,#0x0]
  000c5aa8: vldr.32 s20,[sp,#0x110]
  000c5aac: vmov s0,r1
  000c5ab0: movw r1,#0xd48
  000c5ab4: vcvt.f32.s32 s22,s0
  000c5ab8: blx r11
  000c5aba: subw r0,r0,#0x6a4
  000c5abe: ldr.w r1,[r10,#0x0]
  000c5ac2: vldr.32 s24,[sp,#0x114]
  000c5ac6: vmov s0,r0
  000c5aca: mov r0,r1
  000c5acc: mov.w r1,#0xfa0
  000c5ad0: vcvt.f32.s32 s26,s0
  000c5ad4: blx r11
  000c5ad6: sub.w r0,r0,#0x7d0
  000c5ada: vadd.f32 s0,s24,s16
  000c5ade: vadd.f32 s4,s18,s22
  000c5ae2: vmov s2,r0
  000c5ae6: vadd.f32 s6,s20,s26
  000c5aea: vcvt.f32.s32 s2,s2
  000c5aee: ldr r0,[r6,#0x0]
  000c5af0: ldr r4,[r0,#0x48]
  000c5af2: mov r0,r6
  000c5af4: vmov r1,s4
  000c5af8: vmov r2,s6
  000c5afc: vadd.f32 s0,s0,s2
  000c5b00: vmov r3,s0
  000c5b04: blx r4
  000c5b06: adds r5,#0x1
  000c5b08: cmp r5,#0xa
  000c5b0a: bne 0x000c5a40
  000c5b0c: ldr r0,[sp,#0x48]
  000c5b0e: ldr.w r0,[r0,#0x100]
  000c5b12: ldr r0,[r0,#0x4]
  000c5b14: ldr r0,[r0,#0xc]
  000c5b16: cbz r0,0x000c5b5c
  000c5b18: movs r1,#0x1
  000c5b1a: blx 0x000730b4
  000c5b1e: ldr.w r0,[r8,#0x12c]
  000c5b22: movw r1,#0x9c40
  000c5b26: vldr.32 s0,[r8,#0x124]
  000c5b2a: add r0,r1
  000c5b2c: vldr.32 s2,[r8,#0x128]
  000c5b30: vcvt.f32.s32 s0,s0
  000c5b34: vmov s4,r0
  000c5b38: vcvt.f32.s32 s2,s2
  000c5b3c: vcvt.f32.s32 s4,s4
  000c5b40: ldr r0,[sp,#0x48]
  000c5b42: ldr.w r0,[r0,#0x100]
  000c5b46: vmov r1,s0
  000c5b4a: vmov r2,s2
  000c5b4e: ldr r0,[r0,#0x4]
  000c5b50: vmov r3,s4
  000c5b54: ldr r0,[r0,#0xc]
  000c5b56: ldr r6,[r0,#0x0]
  000c5b58: ldr r6,[r6,#0x48]
  000c5b5a: blx r6
  000c5b5c: movs r0,#0x1c
  000c5b5e: blx 0x0006eb24
  000c5b62: mov r5,r0
  000c5b64: ldr r3,[sp,#0x48]
  000c5b66: movs r1,#0x16
  000c5b68: movs r2,#0x0
  000c5b6a: blx 0x00073edc
  000c5b6e: b.w 0x000cb9b8
  000c5b72: ldr r0,[0x000c5f30]
  000c5b74: add r6,sp,#0x180
  000c5b76: add r0,pc
  000c5b78: vld1.64 {d16,d17},[r0]!
  000c5b7c: vldr.64 d18,[r0]
  000c5b80: mov r0,r6
  000c5b82: vst1.64 {d16,d17},[r0]!
  000c5b86: vstr.64 d18,[r0]
  000c5b8a: movs r0,#0x18
  000c5b8c: blx 0x0006eb24
  000c5b90: mov r5,r0
  000c5b92: mov r1,r6
  000c5b94: movs r2,#0x6
  000c5b96: blx 0x00073eb8
  000c5b9a: ldr r0,[0x000c5f34]
  000c5b9c: ldr r1,[sp,#0x48]
  000c5b9e: add r0,pc
  000c5ba0: vld1.64 {d16,d17},[r0]!
  000c5ba4: str.w r5,[r1,#0x108]
  000c5ba8: add r5,sp,#0x140
  000c5baa: vldr.64 d18,[r0]
  000c5bae: mov r0,r5
  000c5bb0: vst1.64 {d16,d17},[r0]!
  000c5bb4: vstr.64 d18,[r0]
  000c5bb8: movs r0,#0x18
  000c5bba: blx 0x0006eb24
  000c5bbe: mov r9,r0
  000c5bc0: mov r1,r5
  000c5bc2: movs r2,#0x3
  000c5bc4: blx 0x00073eb8
  000c5bc8: mov r0,r9
  000c5bca: movs r1,#0x1
  000c5bcc: blx 0x00073f48
  000c5bd0: ldr r0,[0x000c5f38]
  000c5bd2: add r5,sp,#0x118
  000c5bd4: add r0,pc
  000c5bd6: vld1.64 {d16,d17},[r0]!
  000c5bda: vldr.64 d18,[r0]
  000c5bde: mov r0,r5
  000c5be0: vst1.64 {d16,d17},[r0]!
  000c5be4: vstr.64 d18,[r0]
  000c5be8: movs r0,#0x18
  000c5bea: blx 0x0006eb24
  000c5bee: mov r8,r0
  000c5bf0: mov r1,r5
  000c5bf2: movs r2,#0x3
  000c5bf4: blx 0x00073eb8
  000c5bf8: mov r0,r8
  000c5bfa: movs r1,#0x1
  000c5bfc: mov.w r10,#0x1
  000c5c00: blx 0x00073f48
  000c5c04: movs r0,#0xc
  000c5c06: blx 0x0006eb24
  000c5c0a: mov r6,r0
  000c5c0c: blx 0x00073d08
  000c5c10: ldr r5,[sp,#0x48]
  000c5c12: movs r0,#0x4
  000c5c14: mov r1,r6
  000c5c16: str.w r6,[r5,#0xf8]
  000c5c1a: blx 0x00073d14
  000c5c1e: ldr r0,[0x000c5f3c]
  000c5c20: add r0,pc
  000c5c22: ldr.w r11,[r0,#0x0]
  000c5c26: mov r0,r9
  000c5c28: blx r11
  000c5c2a: ldr r1,[0x000c5f40]
  000c5c2c: movs r4,#0x0
  000c5c2e: strd r0,r10,[sp,#0x0]
  000c5c32: mov r0,r5
  000c5c34: add r1,pc
  000c5c36: movs r2,#0x0
  000c5c38: movs r3,#0x5
  000c5c3a: str r4,[sp,#0x8]
  000c5c3c: ldr r6,[r1,#0x0]
  000c5c3e: movs r1,#0x0
  000c5c40: blx r6
  000c5c42: ldr.w r1,[r5,#0xf8]
  000c5c46: ldr r1,[r1,#0x4]
  000c5c48: str r0,[r1,#0x4]
  000c5c4a: mov r0,r9
  000c5c4c: blx r11
  000c5c4e: strd r0,r10,[sp,#0x0]
  000c5c52: mov r0,r5
  000c5c54: movs r1,#0x0
  000c5c56: movs r2,#0x0
  000c5c58: movs r3,#0x5
  000c5c5a: str r4,[sp,#0x8]
  000c5c5c: blx r6
  000c5c5e: ldr.w r1,[r5,#0xf8]
  000c5c62: ldr r1,[r1,#0x4]
  000c5c64: str r0,[r1,#0x8]
  000c5c66: mov r0,r9
  000c5c68: blx r11
  000c5c6a: strd r0,r10,[sp,#0x0]
  000c5c6e: mov r0,r5
  000c5c70: movs r1,#0x0
  000c5c72: movs r2,#0x0
  000c5c74: movs r3,#0x5
  000c5c76: str r4,[sp,#0x8]
  000c5c78: blx r6
  000c5c7a: ldr.w r1,[r5,#0xf8]
  000c5c7e: ldr r1,[r1,#0x4]
  000c5c80: str r0,[r1,#0xc]
  000c5c82: mov r0,r8
  000c5c84: blx r11
  000c5c86: strd r0,r10,[sp,#0x0]
  000c5c8a: mov r0,r5
  000c5c8c: movs r1,#0x0
  000c5c8e: movs r2,#0x0
  000c5c90: movs r3,#0x11
  000c5c92: str r4,[sp,#0x8]
  000c5c94: blx r6
  000c5c96: ldr r1,[0x000c5f44]
  000c5c98: ldr.w r2,[r5,#0xf8]
  000c5c9c: add r1,pc
  000c5c9e: ldr r2,[r2,#0x4]
  000c5ca0: ldr r1,[r1,#0x0]
  000c5ca2: str r0,[r2,#0x0]
  000c5ca4: ldr r0,[r1,#0x0]
  000c5ca6: movw r1,#0x64b
  000c5caa: blx 0x00072f70
  000c5cae: mov r1,r0
  000c5cb0: ldr.w r0,[r5,#0xf8]
  000c5cb4: ldr r0,[r0,#0x4]
  000c5cb6: ldr r0,[r0,#0x0]
  000c5cb8: adds r0,#0x18
  000c5cba: blx 0x0006f2b0
  000c5cbe: ldr.w r0,[r5,#0xf8]
  000c5cc2: movs r1,#0x1
  000c5cc4: ldr r0,[r0,#0x4]
  000c5cc6: ldr r0,[r0,#0x0]
  000c5cc8: ldr r0,[r0,#0x4]
  000c5cca: blx 0x00073e94
  000c5cce: ldr r6,[0x000c5f48]
  000c5cd0: b 0x000c5d16
  000c5cd2: ldr.w r0,[r0,r4,lsl #0x2]
  000c5cd6: blx 0x000732dc
  000c5cda: ldr r0,[sp,#0x48]
  000c5cdc: cmp r4,#0x0
  000c5cde: ldr.w r0,[r0,#0xf8]
  000c5ce2: ldr r0,[r0,#0x4]
  000c5ce4: ldr.w r5,[r0,r4,lsl #0x2]
  000c5ce8: beq 0x000c5cf4
  000c5cea: mov r0,r9
  000c5cec: blx 0x00073f3c
  000c5cf0: mov r1,r0
  000c5cf2: b 0x000c5cf6
  000c5cf4: mov r1,r8
  000c5cf6: mov r0,r5
  000c5cf8: blx 0x00073f60
  000c5cfc: ldr r0,[sp,#0x48]
  000c5cfe: movs r1,#0x0
  000c5d00: mov r2,r6
  000c5d02: movs r3,#0x0
  000c5d04: ldr.w r0,[r0,#0xf8]
  000c5d08: ldr r0,[r0,#0x4]
  000c5d0a: ldr.w r0,[r0,r4,lsl #0x2]
  000c5d0e: ldr r0,[r0,#0x8]
  000c5d10: blx 0x00073054
  000c5d14: adds r4,#0x1
  000c5d16: ldr r0,[sp,#0x48]
  000c5d18: ldr.w r0,[r0,#0xf8]
  000c5d1c: ldrd r1,r0,[r0,#0x0]
  000c5d20: cmp r4,r1
  000c5d22: bcc 0x000c5cd2
  000c5d24: ldr r5,[r0,#0x0]
  000c5d26: mov r0,r8
  000c5d28: blx r11
  000c5d2a: ldr.w r4,[r0,#0x124]
  000c5d2e: mov r0,r8
  000c5d30: blx r11
  000c5d32: vldr.32 s0,[r0,#0x128]
  000c5d36: mov r0,r8
  000c5d38: vcvt.f32.s32 s16,s0
  000c5d3c: blx r11
  000c5d3e: ldr.w r0,[r0,#0x12c]
  000c5d42: add.w r1,r4,#0x3e8
  000c5d46: vmov r2,s16
  000c5d4a: add.w r0,r0,#0x7d0
  000c5d4e: vmov s0,r1
  000c5d52: vmov s2,r0
  000c5d56: vcvt.f32.s32 s0,s0
  000c5d5a: vcvt.f32.s32 s2,s2
  000c5d5e: ldr r0,[r5,#0x0]
  000c5d60: ldr r6,[r0,#0x48]
  000c5d62: mov r0,r5
  000c5d64: vmov r1,s0
  000c5d68: vmov r3,s2
  000c5d6c: blx r6
  000c5d6e: movs r0,#0x1c
  000c5d70: blx 0x0006eb24
  000c5d74: mov r6,r0
  000c5d76: ldr r3,[sp,#0x48]
  000c5d78: movs r1,#0x16
  000c5d7a: movs r2,#0x0
  000c5d7c: blx 0x00073edc
  000c5d80: ldr r4,[sp,#0x48]
  000c5d82: movs r0,#0x1c
  000c5d84: str r6,[r4,#0x28]
  000c5d86: blx 0x0006eb24
  000c5d8a: mov r6,r0
  000c5d8c: movs r1,#0x7
  000c5d8e: movs r2,#0x1
  000c5d90: mov r3,r4
  000c5d92: blx 0x00073edc
  000c5d96: ldr r0,[sp,#0x48]
  000c5d98: str r6,[r0,#0x2c]
  000c5d9a: mov r0,r9
  000c5d9c: blx 0x00072310
  000c5da0: blx 0x0006eb48
  000c5da4: b.w 0x000cb9bc
  000c5da8: ldr r0,[0x000c5f4c]
  000c5daa: add r6,sp,#0x180
  000c5dac: add r0,pc
  000c5dae: vld1.64 {d16,d17},[r0]!
  000c5db2: vldr.64 d18,[r0]
  000c5db6: mov r0,r6
  000c5db8: vst1.64 {d16,d17},[r0]!
  000c5dbc: vstr.64 d18,[r0]
  000c5dc0: movs r0,#0x18
  000c5dc2: blx 0x0006eb24
  000c5dc6: mov r5,r0
  000c5dc8: mov r1,r6
  000c5dca: movs r2,#0x6
  000c5dcc: blx 0x00073eb8
  000c5dd0: ldr r0,[sp,#0x48]
  000c5dd2: str.w r5,[r0,#0x110]
  000c5dd6: movs r0,#0xc
  000c5dd8: blx 0x0006eb24
  000c5ddc: mov r5,r0
  000c5dde: blx 0x00073d08
  000c5de2: ldr r0,[sp,#0x48]
  000c5de4: mov r1,r5
  000c5de6: str.w r5,[r0,#0xf8]
  000c5dea: movs r0,#0x5
  000c5dec: blx 0x00073d14
  000c5df0: mov.w r8,#0x0
  000c5df4: movs r6,#0x1
  000c5df6: movs r4,#0x0
  000c5df8: b 0x000c5e3a
  000c5dfa: ldr r5,[sp,#0x48]
  000c5dfc: ldr.w r0,[r5,#0x110]
  000c5e00: blx 0x000728bc
  000c5e04: stm sp,{r0,r6,r8}
  000c5e08: mov r0,r5
  000c5e0a: movs r1,#0x9
  000c5e0c: movs r2,#0x0
  000c5e0e: movs r3,#0x8
  000c5e10: blx 0x00073e88
  000c5e14: ldr.w r1,[r5,#0xf8]
  000c5e18: ldr r1,[r1,#0x4]
  000c5e1a: str.w r0,[r1,r4,lsl #0x2]
  000c5e1e: ldr.w r1,[r5,#0xf8]
  000c5e22: ldr.w r0,[r5,#0x110]
  000c5e26: ldr r1,[r1,#0x4]
  000c5e28: ldr.w r5,[r1,r4,lsl #0x2]
  000c5e2c: blx 0x00073f3c
  000c5e30: mov r1,r0
  000c5e32: mov r0,r5
  000c5e34: blx 0x00073f60
  000c5e38: adds r4,#0x1
  000c5e3a: cmp r4,#0x3
  000c5e3c: bne 0x000c5dfa
  000c5e3e: ldr r6,[sp,#0x48]
  000c5e40: movs r4,#0x3
  000c5e42: mov.w r9,#0x0
  000c5e46: mov.w r8,#0x1
  000c5e4a: b 0x000c5ecc
  000c5e4c: ldr.w r0,[r6,#0x110]
  000c5e50: blx 0x000728bc
  000c5e54: stm sp,{r0,r8,r9}
  000c5e58: mov r0,r6
  000c5e5a: movs r1,#0x2
  000c5e5c: movs r2,#0x1
  000c5e5e: movs r3,#0xf
  000c5e60: blx 0x00073e88
  000c5e64: ldr.w r1,[r6,#0xf8]
  000c5e68: ldr r1,[r1,#0x4]
  000c5e6a: str.w r0,[r1,r4,lsl #0x2]
  000c5e6e: movs r1,#0x0
  000c5e70: ldr.w r0,[r6,#0xf8]
  000c5e74: ldr r0,[r0,#0x4]
  000c5e76: ldr.w r0,[r0,r4,lsl #0x2]
  000c5e7a: blx 0x00073f54
  000c5e7e: ldr.w r0,[r6,#0xf8]
  000c5e82: ldr r0,[r0,#0x4]
  000c5e84: ldr.w r0,[r0,r4,lsl #0x2]
  000c5e88: ldr r5,[r0,#0x4]
  000c5e8a: mov r0,r5
  000c5e8c: blx 0x00072910
  000c5e90: movs r1,#0x3
  000c5e92: blx 0x0007198c
  000c5e96: mov r1,r0
  000c5e98: mov r0,r5
  000c5e9a: blx 0x00073f78
  000c5e9e: ldr.w r0,[r6,#0xf8]
  000c5ea2: ldr r0,[r0,#0x4]
  000c5ea4: ldr.w r1,[r0,r4,lsl #0x2]
  000c5ea8: ldr.w r0,[r1,#0x50]!
  000c5eac: cbz r0,0x000c5ec6
  000c5eae: blx 0x00070204
  000c5eb2: blx 0x0006eb48
  000c5eb6: ldr r0,[sp,#0x48]
  000c5eb8: ldr.w r0,[r0,#0xf8]
  000c5ebc: ldr r0,[r0,#0x4]
  000c5ebe: ldr.w r0,[r0,r4,lsl #0x2]
  000c5ec2: add.w r1,r0,#0x50
  000c5ec6: str.w r9,[r1,#0x0]
  000c5eca: adds r4,#0x1
  000c5ecc: cmp r4,#0x5
  000c5ece: bne 0x000c5e4c
  000c5ed0: ldr r4,[sp,#0x48]
  000c5ed2: movs r1,#0x0
  000c5ed4: ldr.w r0,[r4,#0x100]
  000c5ed8: ldr r0,[r0,#0x4]
  000c5eda: ldr r0,[r0,#0xc]
  000c5edc: blx 0x000730b4
  000c5ee0: movs r0,#0x1c
  000c5ee2: blx 0x0006eb24
  000c5ee6: mov r5,r0
  000c5ee8: movs r1,#0x1f
  000c5eea: movs r2,#0x0
  000c5eec: mov r3,r4
  000c5eee: blx 0x00073edc
  000c5ef2: b.w 0x000cb9b8
  000c5ef6: movs r0,#0xc
  000c5ef8: blx 0x0006eb24
  000c5efc: mov r5,r0
  000c5efe: blx 0x00073d08
  000c5f02: ldr r0,[sp,#0x48]
  000c5f04: mov r1,r5
  000c5f06: str.w r5,[r0,#0xf8]
  000c5f0a: movs r0,#0x2
  000c5f0c: blx 0x00073d14
  000c5f10: ldr r0,[0x000c5f50]
  000c5f12: add.w r10,sp,#0x180
  000c5f16: ldr r1,[0x000c5f54]
  000c5f18: add r6,sp,#0x118
  000c5f1a: add r0,pc
  000c5f1c: ldr r5,[sp,#0x48]
  000c5f1e: add r1,pc
  000c5f20: add.w r8,sp,#0x140
  000c5f24: ldr r4,[r0,#0x0]
  000c5f26: mov.w r11,#0x0
  000c5f2a: ldr.w r9,[r1,#0x0]
  000c5f2e: b 0x000c601e
  000c5f58: movs r0,#0x0
  000c5f5a: movs r1,#0x1
  000c5f5c: str r0,[sp,#0x0]
  000c5f5e: movs r2,#0x0
  000c5f60: strd r1,r0,[sp,#0x4]
  000c5f64: mov r0,r5
  000c5f66: movs r1,#0x9
  000c5f68: movs r3,#0x8
  000c5f6a: blx 0x00073e88
  000c5f6e: ldr.w r1,[r5,#0xf8]
  000c5f72: ldr r1,[r1,#0x4]
  000c5f74: str.w r0,[r1,r11,lsl #0x2]
  000c5f78: mov r0,r10
  000c5f7a: ldr.w r1,[r5,#0xf0]
  000c5f7e: blx 0x0007264c
  000c5f82: ldr.w r0,[r5,#0xf0]
  000c5f86: ldr r1,[r0,#0x8]
  000c5f88: mov r0,r6
  000c5f8a: blx 0x00072574
  000c5f8e: mov r0,r8
  000c5f90: mov r1,r6
  000c5f92: blx 0x0006ec80
  000c5f96: ldr r2,[0x000c6348]
  000c5f98: mov r0,r6
  000c5f9a: mov r1,r8
  000c5f9c: blx 0x0006ec74
  000c5fa0: mov r0,r10
  000c5fa2: mov r1,r6
  000c5fa4: blx 0x00072694
  000c5fa8: ldr r0,[r4,#0x0]
  000c5faa: mov.w r1,#0x578
  000c5fae: blx r9
  000c5fb0: sub.w r0,r0,#0x2bc
  000c5fb4: mov.w r1,#0x578
  000c5fb8: vmov s0,r0
  000c5fbc: vcvt.f32.s32 s0,s0
  000c5fc0: vldr.32 s2,[sp,#0x180]
  000c5fc4: ldr r0,[r4,#0x0]
  000c5fc6: vadd.f32 s0,s2,s0
  000c5fca: vstr.32 s0,[sp,#0x180]
  000c5fce: blx r9
  000c5fd0: sub.w r0,r0,#0x2bc
  000c5fd4: mov.w r1,#0x578
  000c5fd8: vmov s0,r0
  000c5fdc: vcvt.f32.s32 s0,s0
  000c5fe0: vldr.32 s2,[sp,#0x184]
  000c5fe4: ldr r0,[r4,#0x0]
  000c5fe6: vadd.f32 s0,s2,s0
  000c5fea: vstr.32 s0,[sp,#0x184]
  000c5fee: blx r9
  000c5ff0: sub.w r0,r0,#0x2bc
  000c5ff4: vmov s0,r0
  000c5ff8: vcvt.f32.s32 s0,s0
  000c5ffc: vldr.32 s2,[sp,#0x188]
  000c6000: vadd.f32 s0,s2,s0
  000c6004: vstr.32 s0,[sp,#0x188]
  000c6008: ldr.w r0,[r5,#0xf8]
  000c600c: ldr r0,[r0,#0x4]
  000c600e: ldr.w r0,[r0,r11,lsl #0x2]
  000c6012: ldr r1,[r0,#0x0]
  000c6014: ldr r2,[r1,#0x44]
  000c6016: mov r1,r10
  000c6018: blx r2
  000c601a: add.w r11,r11,#0x1
  000c601e: cmp.w r11,#0x2
  000c6022: bne 0x000c5f58
  000c6024: movs r0,#0x1c
  000c6026: blx 0x0006eb24
  000c602a: mov r5,r0
  000c602c: ldr r3,[sp,#0x48]
  000c602e: movs r1,#0x7
  000c6030: movs r2,#0x2
  000c6032: blx 0x00073edc
  000c6036: b.w 0x000cb9b8
  000c603a: ldr r4,[sp,#0x48]
  000c603c: ldr r1,[0x000c634c]
  000c603e: ldr r2,[0x000c6350]
  000c6040: ldr.w r0,[r4,#0x100]
  000c6044: ldr r0,[r0,#0x4]
  000c6046: ldr r0,[r0,#0xc]
  000c6048: ldr r3,[r0,#0x0]
  000c604a: ldr r6,[r3,#0x48]
  000c604c: ldr r3,[0x000c6354]
  000c604e: blx r6
  000c6050: ldr.w r0,[r4,#0x100]
  000c6054: add r5,sp,#0x180
  000c6056: ldr r0,[r0,#0x4]
  000c6058: ldr r1,[r0,#0xc]
  000c605a: ldr r0,[r1,#0x0]
  000c605c: ldr r2,[r0,#0x28]
  000c605e: mov r0,r5
  000c6060: blx r2
  000c6062: add.w r0,r4,#0x18c
  000c6066: mov r1,r5
  000c6068: blx 0x0006eb3c
  000c606c: vldr.32 s0,[r4,#0x18c]
  000c6070: movs r0,#0x18
  000c6072: vcvt.s32.f32 s0,s0
  000c6076: vstr.32 s0,[sp,#0x180]
  000c607a: vldr.32 s0,[r4,#0x190]
  000c607e: vcvt.s32.f32 s0,s0
  000c6082: vstr.32 s0,[sp,#0x184]
  000c6086: vldr.32 s0,[r4,#0x194]
  000c608a: vcvt.s32.f32 s0,s0
  000c608e: vstr.32 s0,[sp,#0x188]
  000c6092: blx 0x0006eb24
  000c6096: mov r5,r0
  000c6098: add r1,sp,#0x180
  000c609a: movs r2,#0x3
  000c609c: blx 0x00073eb8
  000c60a0: ldr r0,[sp,#0x48]
  000c60a2: str.w r5,[r0,#0x110]
  000c60a6: movs r0,#0xc
  000c60a8: blx 0x0006eb24
  000c60ac: mov r5,r0
  000c60ae: blx 0x00073d08
  000c60b2: ldr r0,[sp,#0x48]
  000c60b4: mov r1,r5
  000c60b6: str.w r5,[r0,#0xf8]
  000c60ba: movs r0,#0x8
  000c60bc: blx 0x00073d14
  000c60c0: ldr r4,[sp,#0x48]
  000c60c2: mov.w r8,#0x0
  000c60c6: movs r5,#0x1
  000c60c8: movs r6,#0x0
  000c60ca: b 0x000c60f0
  000c60cc: ldr.w r0,[r4,#0x110]
  000c60d0: blx 0x000728bc
  000c60d4: stm sp,{r0,r5,r8}
  000c60d8: mov r0,r4
  000c60da: movs r1,#0x9
  000c60dc: movs r2,#0x0
  000c60de: movs r3,#0x8
  000c60e0: blx 0x00073e88
  000c60e4: ldr.w r1,[r4,#0xf8]
  000c60e8: ldr r1,[r1,#0x4]
  000c60ea: str.w r0,[r1,r6,lsl #0x2]
  000c60ee: adds r6,#0x1
  000c60f0: cmp r6,#0x5
  000c60f2: bne 0x000c60cc
  000c60f4: ldr r6,[sp,#0x48]
  000c60f6: movs r4,#0x5
  000c60f8: mov.w r9,#0x0
  000c60fc: mov.w r8,#0x1
  000c6100: b 0x000c6182
  000c6102: ldr.w r0,[r6,#0x110]
  000c6106: blx 0x000728bc
  000c610a: stm sp,{r0,r8,r9}
  000c610e: mov r0,r6
  000c6110: movs r1,#0x0
  000c6112: movs r2,#0x1
  000c6114: movs r3,#0xf
  000c6116: blx 0x00073e88
  000c611a: ldr.w r1,[r6,#0xf8]
  000c611e: ldr r1,[r1,#0x4]
  000c6120: str.w r0,[r1,r4,lsl #0x2]
  000c6124: movs r1,#0x0
  000c6126: ldr.w r0,[r6,#0xf8]
  000c612a: ldr r0,[r0,#0x4]
  000c612c: ldr.w r0,[r0,r4,lsl #0x2]
  000c6130: blx 0x00073f54
  000c6134: ldr.w r0,[r6,#0xf8]
  000c6138: ldr r0,[r0,#0x4]
  000c613a: ldr.w r0,[r0,r4,lsl #0x2]
  000c613e: ldr r5,[r0,#0x4]
  000c6140: mov r0,r5
  000c6142: blx 0x00072910
  000c6146: asrs r1,r0,#0x1f
  000c6148: add.w r0,r0,r1, lsr #0x1e
  000c614c: asrs r1,r0,#0x2
  000c614e: mov r0,r5
  000c6150: blx 0x00073f78
  000c6154: ldr.w r0,[r6,#0xf8]
  000c6158: ldr r0,[r0,#0x4]
  000c615a: ldr.w r1,[r0,r4,lsl #0x2]
  000c615e: ldr.w r0,[r1,#0x50]!
  000c6162: cbz r0,0x000c617c
  000c6164: blx 0x00070204
  000c6168: blx 0x0006eb48
  000c616c: ldr r0,[sp,#0x48]
  000c616e: ldr.w r0,[r0,#0xf8]
  000c6172: ldr r0,[r0,#0x4]
  000c6174: ldr.w r0,[r0,r4,lsl #0x2]
  000c6178: add.w r1,r0,#0x50
  000c617c: str.w r9,[r1,#0x0]
  000c6180: adds r4,#0x1
  000c6182: cmp r4,#0x8
  000c6184: bne 0x000c6102
  000c6186: b.w 0x000cb9bc
  000c618a: ldr r0,[sp,#0x44]
  000c618c: ldr r0,[r0,#0x0]
  000c618e: blx 0x00072850
  000c6192: mov r6,r0
  000c6194: movs r0,#0x98
  000c6196: blx 0x0006eb24
  000c619a: mov r5,r0
  000c619c: ldr r0,[0x000c6358]
  000c619e: add r0,pc
  000c61a0: ldr.w r9,[r0,#0x0]
  000c61a4: ldr.w r0,[r9,#0x0]
  000c61a8: movw r1,#0x644
  000c61ac: blx 0x00072f70
  000c61b0: mov r1,r0
  000c61b2: add r0,sp,#0x100
  000c61b4: movs r2,#0x0
  000c61b6: blx 0x0006f028
  000c61ba: movs r4,#0x1
  000c61bc: movs r1,#0x5
  000c61be: mov.w r0,#0xffffffff
  000c61c2: add r2,sp,#0x100
  000c61c4: strd r1,r4,[sp,#0x0]
  000c61c8: strd r4,r0,[sp,#0x8]
  000c61cc: mov.w r1,#0xffffffff
  000c61d0: strd r0,r0,[sp,#0x10]
  000c61d4: movs r3,#0x1d
  000c61d6: str r0,[sp,#0x18]
  000c61d8: mov r0,r5
  000c61da: blx 0x00071b30
  000c61de: movs r4,#0x0
  000c61e0: mov r0,r6
  000c61e2: mov r1,r5
  000c61e4: blx 0x00074140
  000c61e8: add r0,sp,#0x100
  000c61ea: blx 0x0006ee30
  000c61ee: ldr r1,[0x000c635c]
  000c61f0: add r6,sp,#0x180
  000c61f2: movs r2,#0x30
  000c61f4: add r1,pc
  000c61f6: mov r0,r6
  000c61f8: blx 0x0006f1e4
  000c61fc: movs r0,#0x18
  000c61fe: blx 0x0006eb24
  000c6202: mov r5,r0
  000c6204: mov r1,r6
  000c6206: movs r2,#0xc
  000c6208: blx 0x00073eb8
  000c620c: ldr r0,[sp,#0x48]
  000c620e: str.w r5,[r0,#0x108]
  000c6212: movs r0,#0xc
  000c6214: blx 0x0006eb24
  000c6218: mov r5,r0
  000c621a: blx 0x00073d08
  000c621e: ldr r4,[sp,#0x48]
  000c6220: movs r0,#0x8
  000c6222: mov r1,r5
  000c6224: str.w r5,[r4,#0xf8]
  000c6228: blx 0x00073d14
  000c622c: mov.w r8,#0x0
  000c6230: movs r0,#0x1
  000c6232: strd r8,r0,[sp,#0x0]
  000c6236: mov r0,r4
  000c6238: movs r1,#0x1
  000c623a: movs r2,#0x0
  000c623c: movs r3,#0x9
  000c623e: str.w r8,[sp,#0x8]
  000c6242: blx 0x00073e88
  000c6246: ldr.w r1,[r4,#0xf8]
  000c624a: ldr r1,[r1,#0x4]
  000c624c: ldr r2,[0x000c6360]
  000c624e: str r0,[r1,#0x0]
  000c6250: add r2,pc
  000c6252: ldr.w r0,[r4,#0xf8]
  000c6256: ldr.w r11,[r2,#0x0]
  000c625a: vldr.32 s16,[sp,#0x10c]
  000c625e: ldr r1,[r0,#0x4]
  000c6260: ldr.w r0,[r11,#0x0]
  000c6264: ldr r5,[r1,#0x0]
  000c6266: mov.w r1,#0x578
  000c626a: blx 0x00071848
  000c626e: sub.w r0,r0,#0x2bc
  000c6272: ldr.w r1,[r11,#0x0]
  000c6276: vldr.32 s18,[sp,#0x110]
  000c627a: vmov s0,r0
  000c627e: mov r0,r1
  000c6280: mov.w r1,#0x578
  000c6284: vcvt.f32.s32 s20,s0
  000c6288: blx 0x00071848
  000c628c: sub.w r0,r0,#0x2bc
  000c6290: vldr.32 s2,[pc,#0xd0]
  000c6294: vmov s0,r0
  000c6298: vcvt.f32.s32 s0,s0
  000c629c: vldr.32 s4,[sp,#0x114]
  000c62a0: ldr r0,[r5,#0x0]
  000c62a2: vadd.f32 s2,s4,s2
  000c62a6: vadd.f32 s4,s16,s20
  000c62aa: ldr r6,[r0,#0x48]
  000c62ac: mov r0,r5
  000c62ae: vadd.f32 s0,s18,s0
  000c62b2: vmov r3,s2
  000c62b6: vmov r1,s4
  000c62ba: vmov r2,s0
  000c62be: blx r6
  000c62c0: ldr.w r0,[r4,#0xf8]
  000c62c4: ldr r0,[r0,#0x4]
  000c62c6: ldr r0,[r0,#0x0]
  000c62c8: ldr r1,[r0,#0x0]
  000c62ca: ldr r2,[r1,#0x1c]
  000c62cc: ldr r1,[0x000c6368]
  000c62ce: blx r2
  000c62d0: ldr.w r0,[r4,#0xf8]
  000c62d4: movs r1,#0x3
  000c62d6: ldr r0,[r0,#0x4]
  000c62d8: ldr r0,[r0,#0x0]
  000c62da: blx 0x00074008
  000c62de: ldr.w r0,[r4,#0xf8]
  000c62e2: ldr r1,[0x000c636c]
  000c62e4: ldr r0,[r0,#0x4]
  000c62e6: ldr r0,[r0,#0x0]
  000c62e8: ldr r0,[r0,#0x4]
  000c62ea: blx 0x000727e4
  000c62ee: ldr.w r1,[r4,#0xf8]
  000c62f2: ldr.w r0,[r4,#0x108]
  000c62f6: ldr r1,[r1,#0x4]
  000c62f8: ldr r5,[r1,#0x0]
  000c62fa: blx 0x00073f3c
  000c62fe: mov r1,r0
  000c6300: mov r0,r5
  000c6302: blx 0x00073f60
  000c6306: ldr.w r0,[r4,#0xf8]
  000c630a: movs r1,#0x1
  000c630c: ldr r0,[r0,#0x4]
  000c630e: ldr r0,[r0,#0x0]
  000c6310: ldr r0,[r0,#0x4]
  000c6312: blx 0x00073fb4
  000c6316: ldr.w r0,[r9,#0x0]
  000c631a: movw r1,#0x644
  000c631e: blx 0x00072f70
  000c6322: mov r1,r0
  000c6324: ldr.w r0,[r4,#0xf8]
  000c6328: ldr r0,[r0,#0x4]
  000c632a: ldr r0,[r0,#0x0]
  000c632c: adds r0,#0x18
  000c632e: blx 0x0006f2b0
  000c6332: ldr.w r0,[r4,#0xf8]
  000c6336: mov.w r9,#0x1
  000c633a: ldr r5,[sp,#0x48]
  000c633c: ldr r1,[r0,#0x4]
  000c633e: ldr r1,[r1,#0x0]
  000c6340: str.w r8,[r1,#0x50]
  000c6344: b 0x000c63d2
  000c6370: ldr r0,[0x000c6708]
  000c6372: movs r1,#0x8
  000c6374: add r0,pc
  000c6376: ldr r0,[r0,#0x0]
  000c6378: ldr r0,[r0,#0x0]
  000c637a: blx 0x00071a34
  000c637e: ldr.w r4,[r5,#0x108]
  000c6382: mov r10,r0
  000c6384: ldr.w r6,[r11,#0x0]
  000c6388: mov r0,r4
  000c638a: blx 0x00073fcc
  000c638e: mov r1,r0
  000c6390: mov r0,r6
  000c6392: blx 0x00071848
  000c6396: mov r1,r0
  000c6398: mov r0,r4
  000c639a: blx 0x00073ec4
  000c639e: str r0,[sp,#0x0]
  000c63a0: movs r0,#0x1
  000c63a2: strd r0,r8,[sp,#0x4]
  000c63a6: mov r0,r5
  000c63a8: movs r1,#0x8
  000c63aa: movs r2,#0x0
  000c63ac: mov r3,r10
  000c63ae: blx 0x00073e88
  000c63b2: ldr.w r1,[r5,#0xf8]
  000c63b6: ldr r1,[r1,#0x4]
  000c63b8: str.w r0,[r1,r9,lsl #0x2]
  000c63bc: ldr.w r0,[r5,#0xf8]
  000c63c0: ldr r0,[r0,#0x4]
  000c63c2: ldr.w r0,[r0,r9,lsl #0x2]
  000c63c6: blx 0x000732dc
  000c63ca: ldr.w r0,[r5,#0xf8]
  000c63ce: add.w r9,r9,#0x1
  000c63d2: ldr r0,[r0,#0x0]
  000c63d4: cmp r9,r0
  000c63d6: bcc 0x000c6370
  000c63d8: movs r0,#0x1c
  000c63da: blx 0x0006eb24
  000c63de: mov r5,r0
  000c63e0: ldr r0,[sp,#0x48]
  000c63e2: movs r1,#0x14
  000c63e4: str r0,[sp,#0x0]
  000c63e6: mov r0,r5
  000c63e8: movs r2,#0x1
  000c63ea: movs r3,#0x8
  000c63ec: blx 0x00073f9c
  000c63f0: ldr r0,[sp,#0x48]
  000c63f2: mov r4,r0
  000c63f4: str r5,[r0,#0x28]
  000c63f6: movs r0,#0x1c
  000c63f8: blx 0x0006eb24
  000c63fc: mov r5,r0
  000c63fe: movs r1,#0x15
  000c6400: movs r2,#0x1
  000c6402: movs r3,#0x8
  000c6404: str r4,[sp,#0x0]
  000c6406: blx 0x00073f9c
  000c640a: b.w 0x000c9412
  000c640e: ldr r0,[0x000c670c]
  000c6410: add r0,pc
  000c6412: vldr.64 d16,[r0]
  000c6416: ldr r0,[r0,#0x8]
  000c6418: str r0,[sp,#0x188]
  000c641a: movs r0,#0x18
  000c641c: vstr.64 d16,[sp,#0x180]
  000c6420: blx 0x0006eb24
  000c6424: mov r5,r0
  000c6426: add r1,sp,#0x180
  000c6428: movs r2,#0x3
  000c642a: blx 0x00073eb8
  000c642e: ldr r0,[sp,#0x48]
  000c6430: str.w r5,[r0,#0x110]
  000c6434: movs r0,#0xc
  000c6436: blx 0x0006eb24
  000c643a: mov r5,r0
  000c643c: blx 0x00073d08
  000c6440: ldr r0,[sp,#0x48]
  000c6442: mov r1,r5
  000c6444: str.w r5,[r0,#0xf8]
  000c6448: movs r0,#0x7
  000c644a: blx 0x00073d14
  000c644e: ldr r0,[0x000c6710]
  000c6450: movs r4,#0x0
  000c6452: add r0,pc
  000c6454: ldr r0,[r0,#0x0]
  000c6456: str r0,[sp,#0x30]
  000c6458: ldr r0,[0x000c6714]
  000c645a: ldr r1,[0x000c6718]
  000c645c: add r0,pc
  000c645e: ldr.w r10,[sp,#0x48]
  000c6462: add r1,pc
  000c6464: ldr.w r8,[sp,#0x30]
  000c6468: ldr r0,[r0,#0x0]
  000c646a: str r0,[sp,#0x2c]
  000c646c: ldr r0,[r1,#0x0]
  000c646e: str r0,[sp,#0x28]
  000c6470: ldr.w r9,[sp,#0x28]
  000c6474: b 0x000c6550
  000c6476: ldr.w r0,[r10,#0x110]
  000c647a: blx r8
  000c647c: str r0,[sp,#0x0]
  000c647e: movs r0,#0x1
  000c6480: str r0,[sp,#0x4]
  000c6482: movs r0,#0x0
  000c6484: str r0,[sp,#0x8]
  000c6486: mov r0,r10
  000c6488: movs r1,#0x2
  000c648a: movs r2,#0x1
  000c648c: movs r3,#0xf
  000c648e: blx 0x00073e88
  000c6492: ldr.w r1,[r10,#0xf8]
  000c6496: ldr r1,[r1,#0x4]
  000c6498: str.w r0,[r1,r4,lsl #0x2]
  000c649c: movs r1,#0x1
  000c649e: ldr.w r0,[r10,#0xf8]
  000c64a2: ldr r0,[r0,#0x4]
  000c64a4: ldr.w r0,[r0,r4,lsl #0x2]
  000c64a8: ldr r0,[r0,#0x4]
  000c64aa: blx 0x00073fb4
  000c64ae: ldr.w r0,[r10,#0xf8]
  000c64b2: movs r1,#0x0
  000c64b4: ldr r0,[r0,#0x4]
  000c64b6: ldr.w r0,[r0,r4,lsl #0x2]
  000c64ba: blx 0x00073f54
  000c64be: ldr.w r1,[r10,#0xf8]
  000c64c2: ldr.w r0,[r10,#0x110]
  000c64c6: ldr r1,[r1,#0x4]
  000c64c8: ldr.w r1,[r1,r4,lsl #0x2]
  000c64cc: str r1,[sp,#0x38]
  000c64ce: blx r8
  000c64d0: ldr r6,[sp,#0x2c]
  000c64d2: movw r1,#0x4e20
  000c64d6: ldr.w r0,[r0,#0x124]
  000c64da: str r0,[sp,#0x34]
  000c64dc: ldr r0,[r6,#0x0]
  000c64de: blx r9
  000c64e0: str r4,[sp,#0x3c]
  000c64e2: mov r4,r0
  000c64e4: ldr.w r0,[r10,#0x110]
  000c64e8: blx r8
  000c64ea: ldr.w r11,[r0,#0x128]
  000c64ee: movw r1,#0x4e20
  000c64f2: ldr r0,[r6,#0x0]
  000c64f4: blx r9
  000c64f6: add r0,r11
  000c64f8: movw r11,#0x2710
  000c64fc: sub.w r5,r0,r11
  000c6500: ldr r0,[sp,#0x34]
  000c6502: add r0,r4
  000c6504: sub.w r0,r0,r11
  000c6508: vmov s16,r0
  000c650c: ldr.w r0,[r10,#0x110]
  000c6510: blx r8
  000c6512: ldr.w r4,[r0,#0x12c]
  000c6516: vcvt.f32.s32 s16,s16
  000c651a: ldr r0,[r6,#0x0]
  000c651c: vmov s0,r5
  000c6520: movw r1,#0x4e20
  000c6524: vcvt.f32.s32 s18,s0
  000c6528: blx r9
  000c652a: add r0,r4
  000c652c: ldr r4,[sp,#0x3c]
  000c652e: sub.w r0,r0,r11
  000c6532: vmov r1,s16
  000c6536: vmov r2,s18
  000c653a: vmov s0,r0
  000c653e: vcvt.f32.s32 s0,s0
  000c6542: ldr r0,[sp,#0x38]
  000c6544: ldr r6,[r0,#0x0]
  000c6546: ldr r6,[r6,#0x48]
  000c6548: vmov r3,s0
  000c654c: blx r6
  000c654e: adds r4,#0x1
  000c6550: cmp r4,#0x2
  000c6552: bne 0x000c6476
  000c6554: ldr r0,[0x000c671c]
  000c6556: movs r5,#0x2
  000c6558: mov.w r8,#0x0
  000c655c: mov.w r9,#0x1
  000c6560: add r0,pc
  000c6562: ldr r6,[sp,#0x48]
  000c6564: ldr.w r10,[r0,#0x0]
  000c6568: b 0x000c65b0
  000c656a: ldr.w r0,[r10,#0x0]
  000c656e: movs r1,#0x3
  000c6570: blx 0x00071a34
  000c6574: mov r4,r0
  000c6576: ldr.w r0,[r6,#0x110]
  000c657a: blx 0x000728bc
  000c657e: strd r0,r9,[sp,#0x0]
  000c6582: mov r0,r6
  000c6584: movs r1,#0x3
  000c6586: movs r2,#0x0
  000c6588: mov r3,r4
  000c658a: str.w r8,[sp,#0x8]
  000c658e: blx 0x00073e88
  000c6592: ldr.w r1,[r6,#0xf8]
  000c6596: ldr r1,[r1,#0x4]
  000c6598: str.w r0,[r1,r5,lsl #0x2]
  000c659c: movs r1,#0x1
  000c659e: ldr.w r0,[r6,#0xf8]
  000c65a2: ldr r0,[r0,#0x4]
  000c65a4: ldr.w r0,[r0,r5,lsl #0x2]
  000c65a8: ldr r0,[r0,#0x4]
  000c65aa: blx 0x00073e94
  000c65ae: adds r5,#0x1
  000c65b0: ldr r0,[sp,#0x48]
  000c65b2: ldr.w r0,[r0,#0xf8]
  000c65b6: ldr r0,[r0,#0x0]
  000c65b8: cmp r5,r0
  000c65ba: bcc 0x000c656a
  000c65bc: movs r0,#0x1c
  000c65be: blx 0x0006eb24
  000c65c2: mov r5,r0
  000c65c4: ldr r0,[sp,#0x48]
  000c65c6: movs r1,#0x12
  000c65c8: str r0,[sp,#0x0]
  000c65ca: mov r0,r5
  000c65cc: movs r2,#0x2
  000c65ce: movs r3,#0x7
  000c65d0: blx 0x00073f9c
  000c65d4: ldr r4,[sp,#0x48]
  000c65d6: movs r0,#0x1c
  000c65d8: str r5,[r4,#0x28]
  000c65da: blx 0x0006eb24
  000c65de: mov r5,r0
  000c65e0: movs r1,#0x7
  000c65e2: movs r2,#0x2
  000c65e4: mov r3,r4
  000c65e6: blx 0x00073edc
  000c65ea: b.w 0x000c9412
  000c65ee: ldr r0,[0x000c6720]
  000c65f0: ldr r1,[0x000c6724]
  000c65f2: add r0,pc
  000c65f4: add r1,pc
  000c65f6: vldr.64 d16,[r0]
  000c65fa: ldr r0,[r0,#0x8]
  000c65fc: vstr.64 d16,[sp,#0x140]
  000c6600: vld1.64 {d16,d17},[r1]!
  000c6604: str r0,[sp,#0x148]
  000c6606: add r0,sp,#0x180
  000c6608: vldr.64 d18,[r1]
  000c660c: vst1.64 {d16,d17},[r0]!
  000c6610: vstr.64 d18,[r0]
  000c6614: movs r0,#0x18
  000c6616: blx 0x0006eb24
  000c661a: mov r5,r0
  000c661c: add r1,sp,#0x140
  000c661e: movs r2,#0x3
  000c6620: blx 0x00073eb8
  000c6624: ldr r0,[sp,#0x48]
  000c6626: str.w r5,[r0,#0x110]
  000c662a: movs r0,#0x18
  000c662c: blx 0x0006eb24
  000c6630: mov r5,r0
  000c6632: add r1,sp,#0x180
  000c6634: movs r2,#0x6
  000c6636: blx 0x00073eb8
  000c663a: ldr r0,[sp,#0x48]
  000c663c: str.w r5,[r0,#0x10c]
  000c6640: movs r0,#0xc
  000c6642: blx 0x0006eb24
  000c6646: mov r5,r0
  000c6648: blx 0x00073d08
  000c664c: ldr r6,[sp,#0x44]
  000c664e: ldr r4,[sp,#0x48]
  000c6650: movs r0,#0xd
  000c6652: mov r1,r5
  000c6654: str.w r5,[r4,#0xf8]
  000c6658: blx 0x00073d14
  000c665c: mov.w r10,#0x0
  000c6660: mov.w r8,#0x1
  000c6664: mov r0,r4
  000c6666: movs r1,#0x0
  000c6668: movs r2,#0x1
  000c666a: movs r3,#0xd
  000c666c: strd r10,r8,[sp,#0x0]
  000c6670: str.w r10,[sp,#0x8]
  000c6674: blx 0x00073e88
  000c6678: ldr.w r1,[r4,#0xf8]
  000c667c: ldr r1,[r1,#0x4]
  000c667e: str r0,[r1,#0x0]
  000c6680: movs r1,#0x1
  000c6682: ldr.w r0,[r4,#0xf8]
  000c6686: ldr r0,[r0,#0x4]
  000c6688: ldr r0,[r0,#0x0]
  000c668a: ldr r0,[r0,#0x4]
  000c668c: blx 0x00073fb4
  000c6690: ldr r0,[0x000c6728]
  000c6692: movw r1,#0x644
  000c6696: add r0,pc
  000c6698: ldr.w r9,[r0,#0x0]
  000c669c: ldr.w r0,[r9,#0x0]
  000c66a0: blx 0x00072f70
  000c66a4: mov r1,r0
  000c66a6: ldr.w r0,[r4,#0xf8]
  000c66aa: ldr r0,[r0,#0x4]
  000c66ac: ldr r0,[r0,#0x0]
  000c66ae: adds r0,#0x18
  000c66b0: blx 0x0006f2b0
  000c66b4: ldr.w r0,[r4,#0xf8]
  000c66b8: ldr r0,[r0,#0x4]
  000c66ba: ldr r1,[r0,#0x0]
  000c66bc: ldr r0,[r6,#0x0]
  000c66be: ldr r6,[r1,#0x4]
  000c66c0: blx 0x00071c5c
  000c66c4: add.w r0,r0,r0, lsl #0x2
  000c66c8: add.w r1,r0,#0x708
  000c66cc: mov r0,r6
  000c66ce: blx 0x00073f78
  000c66d2: ldr.w r0,[r4,#0xf8]
  000c66d6: ldr r0,[r0,#0x4]
  000c66d8: ldr r0,[r0,#0x0]
  000c66da: ldr r1,[r0,#0x0]
  000c66dc: ldr r6,[r1,#0x48]
  000c66de: ldr r1,[0x000c672c]
  000c66e0: mov r2,r1
  000c66e2: mov r3,r1
  000c66e4: blx r6
  000c66e6: ldr.w r0,[r4,#0xf8]
  000c66ea: ldr r0,[r0,#0x4]
  000c66ec: ldr r0,[r0,#0x0]
  000c66ee: blx 0x000732dc
  000c66f2: ldr.w r0,[r4,#0xf8]
  000c66f6: movs r1,#0x0
  000c66f8: ldr r0,[r0,#0x4]
  000c66fa: ldr r0,[r0,#0x0]
  000c66fc: blx 0x000730b4
  000c6700: ldr r5,[sp,#0x48]
  000c6702: movs r6,#0x1
  000c6704: b 0x000c67ac
  000c6730: ldr r0,[0x000c6b14]
  000c6732: movs r1,#0x0
  000c6734: add r0,pc
  000c6736: ldr r0,[r0,#0x0]
  000c6738: ldr r0,[r0,#0x0]
  000c673a: blx 0x00071a34
  000c673e: mov r3,r0
  000c6740: mov r0,r5
  000c6742: movs r1,#0x0
  000c6744: movs r2,#0x0
  000c6746: strd r10,r8,[sp,#0x0]
  000c674a: str.w r10,[sp,#0x8]
  000c674e: blx 0x00073e88
  000c6752: ldr.w r1,[r5,#0xf8]
  000c6756: ldr r1,[r1,#0x4]
  000c6758: str.w r0,[r1,r6,lsl #0x2]
  000c675c: ldr.w r1,[r5,#0xf8]
  000c6760: ldr.w r0,[r5,#0x10c]
  000c6764: ldr r1,[r1,#0x4]
  000c6766: ldr.w r4,[r1,r6,lsl #0x2]
  000c676a: blx 0x00073f3c
  000c676e: mov r1,r0
  000c6770: mov r0,r4
  000c6772: blx 0x00073f60
  000c6776: ldr.w r0,[r5,#0xf8]
  000c677a: movs r1,#0x1
  000c677c: ldr r0,[r0,#0x4]
  000c677e: ldr.w r0,[r0,r6,lsl #0x2]
  000c6782: ldr r0,[r0,#0x4]
  000c6784: blx 0x00073fb4
  000c6788: cmp r6,#0x2
  000c678a: bne 0x000c67aa
  000c678c: ldr.w r0,[r9,#0x0]
  000c6790: movw r1,#0x645
  000c6794: blx 0x00072f70
  000c6798: mov r1,r0
  000c679a: ldr r0,[sp,#0x48]
  000c679c: ldr.w r0,[r0,#0xf8]
  000c67a0: ldr r0,[r0,#0x4]
  000c67a2: ldr r0,[r0,#0x8]
  000c67a4: adds r0,#0x18
  000c67a6: blx 0x0006f2b0
  000c67aa: adds r6,#0x1
  000c67ac: cmp r6,#0x5
  000c67ae: bne 0x000c6730
  000c67b0: movs r4,#0x5
  000c67b2: mov.w r8,#0x0
  000c67b6: movs r6,#0x1
  000c67b8: ldr r5,[sp,#0x48]
  000c67ba: b 0x000c67f2
  000c67bc: ldr.w r0,[r5,#0x110]
  000c67c0: blx 0x000728bc
  000c67c4: stm sp,{r0,r6,r8}
  000c67c8: mov r0,r5
  000c67ca: movs r1,#0x9
  000c67cc: movs r2,#0x0
  000c67ce: movs r3,#0x8
  000c67d0: blx 0x00073e88
  000c67d4: ldr.w r1,[r5,#0xf8]
  000c67d8: ldr r1,[r1,#0x4]
  000c67da: str.w r0,[r1,r4,lsl #0x2]
  000c67de: movs r1,#0x1
  000c67e0: ldr.w r0,[r5,#0xf8]
  000c67e4: ldr r0,[r0,#0x4]
  000c67e6: ldr.w r0,[r0,r4,lsl #0x2]
  000c67ea: ldr r0,[r0,#0x4]
  000c67ec: blx 0x00073e94
  000c67f0: adds r4,#0x1
  000c67f2: cmp r4,#0x9
  000c67f4: bne 0x000c67bc
  000c67f6: ldr r5,[0x000c6b18]
  000c67f8: add.w r9,sp,#0x118
  000c67fc: ldr r6,[sp,#0x48]
  000c67fe: movs r4,#0x9
  000c6800: mov.w r10,#0x0
  000c6804: mov.w r8,#0x1
  000c6808: b 0x000c6862
  000c680a: mov r0,r6
  000c680c: movs r1,#0x9
  000c680e: movs r2,#0x0
  000c6810: movs r3,#0x8
  000c6812: strd r10,r8,[sp,#0x0]
  000c6816: str.w r10,[sp,#0x8]
  000c681a: blx 0x00073e88
  000c681e: ldr.w r1,[r6,#0xf8]
  000c6822: ldr r1,[r1,#0x4]
  000c6824: str.w r0,[r1,r4,lsl #0x2]
  000c6828: ldr.w r0,[r6,#0xf8]
  000c682c: ldr r0,[r0,#0x4]
  000c682e: ldr.w r0,[r0,r4,lsl #0x2]
  000c6832: strd r5,r5,[sp,#0x118]
  000c6836: str r5,[sp,#0x120]
  000c6838: ldr r1,[r0,#0x0]
  000c683a: ldr r2,[r1,#0x44]
  000c683c: mov r1,r9
  000c683e: blx r2
  000c6840: ldr.w r0,[r6,#0xf8]
  000c6844: movs r1,#0x1
  000c6846: ldr r0,[r0,#0x4]
  000c6848: ldr.w r0,[r0,r4,lsl #0x2]
  000c684c: ldr r0,[r0,#0x4]
  000c684e: blx 0x00073e94
  000c6852: ldr.w r0,[r6,#0xf8]
  000c6856: ldr r0,[r0,#0x4]
  000c6858: ldr.w r0,[r0,r4,lsl #0x2]
  000c685c: blx 0x000732dc
  000c6860: adds r4,#0x1
  000c6862: cmp r4,#0xd
  000c6864: bne 0x000c680a
  000c6866: ldr r0,[sp,#0x48]
  000c6868: ldr r1,[0x000c6b1c]
  000c686a: ldr r2,[0x000c6b20]
  000c686c: ldr.w r0,[r0,#0x100]
  000c6870: ldr r0,[r0,#0x4]
  000c6872: ldr r0,[r0,#0xc]
  000c6874: ldr r3,[r0,#0x0]
  000c6876: ldr r6,[r3,#0x48]
  000c6878: ldr r3,[0x000c6b24]
  000c687a: blx r6
  000c687c: ldr r0,[0x000c6b28]
  000c687e: add r0,pc
  000c6880: ldr r0,[r0,#0x0]
  000c6882: ldrb r0,[r0,#0x0]
  000c6884: cbz r0,0x000c68a8
  000c6886: ldr r0,[sp,#0x48]
  000c6888: movs r2,#0x0
  000c688a: ldr r1,[0x000c6b2c]
  000c688c: ldr r3,[0x000c6b30]
  000c688e: mov r4,r0
  000c6890: ldr.w r0,[r0,#0xf0]
  000c6894: blx 0x000725bc
  000c6898: ldr.w r0,[r4,#0xf0]
  000c689c: movs r1,#0x0
  000c689e: ldr r2,[0x000c6b34]
  000c68a0: movs r3,#0x0
  000c68a2: ldr r0,[r0,#0x8]
  000c68a4: blx 0x00073054
  000c68a8: movs r0,#0x1c
  000c68aa: blx 0x0006eb24
  000c68ae: mov r5,r0
  000c68b0: ldr r3,[sp,#0x48]
  000c68b2: movs r1,#0x7
  000c68b4: movs r2,#0x1
  000c68b6: blx 0x00073edc
  000c68ba: b.w 0x000c9412
  000c68be: movs r0,#0xc
  000c68c0: blx 0x0006eb24
  000c68c4: mov r5,r0
  000c68c6: blx 0x00073d08
  000c68ca: ldr r6,[sp,#0x48]
  000c68cc: movs r0,#0x8
  000c68ce: mov r1,r5
  000c68d0: str.w r5,[r6,#0xf8]
  000c68d4: blx 0x00073d14
  000c68d8: movs r4,#0x0
  000c68da: mov.w r8,#0x1
  000c68de: mov r0,r6
  000c68e0: movs r1,#0x1
  000c68e2: movs r2,#0x1
  000c68e4: movs r3,#0xd
  000c68e6: strd r4,r8,[sp,#0x0]
  000c68ea: str r4,[sp,#0x8]
  000c68ec: blx 0x00073e88
  000c68f0: ldr.w r1,[r6,#0xf8]
  000c68f4: ldr r1,[r1,#0x4]
  000c68f6: str r0,[r1,#0x0]
  000c68f8: movs r1,#0x1
  000c68fa: ldr.w r0,[r6,#0xf8]
  000c68fe: ldr r0,[r0,#0x4]
  000c6900: ldr r0,[r0,#0x0]
  000c6902: ldr r0,[r0,#0x4]
  000c6904: blx 0x00073fb4
  000c6908: ldr r0,[0x000c6b38]
  000c690a: movw r1,#0x644
  000c690e: add r0,pc
  000c6910: ldr r0,[r0,#0x0]
  000c6912: ldr r0,[r0,#0x0]
  000c6914: blx 0x00072f70
  000c6918: mov r1,r0
  000c691a: ldr.w r0,[r6,#0xf8]
  000c691e: ldr r0,[r0,#0x4]
  000c6920: ldr r0,[r0,#0x0]
  000c6922: adds r0,#0x18
  000c6924: blx 0x0006f2b0
  000c6928: ldr r0,[0x000c6b3c]
  000c692a: add r0,pc
  000c692c: ldr r6,[r0,#0x0]
  000c692e: ldr r2,[r6,#0x0]
  000c6930: cmp r2,#0x0
  000c6932: bgt 0x000c6964
  000c6934: ldr r0,[sp,#0x44]
  000c6936: ldr r0,[r0,#0x0]
  000c6938: blx 0x00071c5c
  000c693c: add.w r0,r0,r0, lsl #0x2
  000c6940: vldr.32 s2,[pc,#0x1fc]
  000c6944: add.w r0,r0,#0x708
  000c6948: vmov s0,r0
  000c694c: vcvt.f32.s32 s0,s0
  000c6950: vmul.f32 s0,s0,s2
  000c6954: vcvt.s32.f32 s2,s0
  000c6958: vcvt.s32.f32 s0,s0
  000c695c: vstr.32 s2,[r6]
  000c6960: vmov r2,s0
  000c6964: ldr r0,[sp,#0x48]
  000c6966: mov r1,r2
  000c6968: mov r5,r0
  000c696a: ldr.w r0,[r0,#0xf8]
  000c696e: ldr r0,[r0,#0x4]
  000c6970: ldr r0,[r0,#0x0]
  000c6972: ldr r0,[r0,#0x4]
  000c6974: blx 0x000727e4
  000c6978: ldr.w r0,[r5,#0xf8]
  000c697c: movs r1,#0x1
  000c697e: ldr r0,[r0,#0x4]
  000c6980: ldr r0,[r0,#0x0]
  000c6982: blx 0x00073f54
  000c6986: ldr.w r0,[r5,#0xf8]
  000c698a: movs r2,#0x0
  000c698c: ldr r3,[0x000c6b44]
  000c698e: ldr r0,[r0,#0x4]
  000c6990: ldr r0,[r0,#0x0]
  000c6992: ldr r1,[r0,#0x0]
  000c6994: ldr r6,[r1,#0x48]
  000c6996: movs r1,#0x0
  000c6998: blx r6
  000c699a: ldr r5,[sp,#0x48]
  000c699c: movs r6,#0x1
  000c699e: b 0x000c69d0
  000c69a0: mov r0,r5
  000c69a2: movs r1,#0x9
  000c69a4: movs r2,#0x0
  000c69a6: movs r3,#0x8
  000c69a8: strd r4,r8,[sp,#0x0]
  000c69ac: str r4,[sp,#0x8]
  000c69ae: blx 0x00073e88
  000c69b2: ldr.w r1,[r5,#0xf8]
  000c69b6: ldr r1,[r1,#0x4]
  000c69b8: str.w r0,[r1,r6,lsl #0x2]
  000c69bc: movs r1,#0x1
  000c69be: ldr.w r0,[r5,#0xf8]
  000c69c2: ldr r0,[r0,#0x4]
  000c69c4: ldr.w r0,[r0,r6,lsl #0x2]
  000c69c8: ldr r0,[r0,#0x4]
  000c69ca: blx 0x00073e94
  000c69ce: adds r6,#0x1
  000c69d0: cmp r6,#0x8
  000c69d2: bne 0x000c69a0
  000c69d4: ldr r4,[sp,#0x48]
  000c69d6: movs r2,#0x0
  000c69d8: ldr r3,[0x000c6b48]
  000c69da: ldr.w r0,[r4,#0xf8]
  000c69de: ldr r0,[r0,#0x4]
  000c69e0: ldr r0,[r0,#0x4]
  000c69e2: ldr r1,[r0,#0x0]
  000c69e4: ldr r6,[r1,#0x48]
  000c69e6: movs r1,#0x0
  000c69e8: blx r6
  000c69ea: ldr.w r0,[r4,#0xf8]
  000c69ee: ldr r1,[0x000c6b4c]
  000c69f0: ldr r2,[0x000c6b50]
  000c69f2: ldr r0,[r0,#0x4]
  000c69f4: ldr r0,[r0,#0x8]
  000c69f6: ldr r3,[r0,#0x0]
  000c69f8: ldr r6,[r3,#0x48]
  000c69fa: ldr r3,[0x000c6b54]
  000c69fc: blx r6
  000c69fe: ldr.w r0,[r4,#0xf8]
  000c6a02: ldr r5,[0x000c6b58]
  000c6a04: ldr r1,[0x000c6b5c]
  000c6a06: ldr r0,[r0,#0x4]
  000c6a08: ldr r3,[0x000c6b60]
  000c6a0a: ldr r0,[r0,#0xc]
  000c6a0c: ldr r2,[r0,#0x0]
  000c6a0e: ldr r6,[r2,#0x48]
  000c6a10: mov r2,r5
  000c6a12: blx r6
  000c6a14: ldr.w r0,[r4,#0xf8]
  000c6a18: ldr r1,[0x000c6b64]
  000c6a1a: ldr r2,[0x000c6b68]
  000c6a1c: ldr r0,[r0,#0x4]
  000c6a1e: ldr r0,[r0,#0x10]
  000c6a20: ldr r3,[r0,#0x0]
  000c6a22: ldr r6,[r3,#0x48]
  000c6a24: ldr r3,[0x000c6b6c]
  000c6a26: blx r6
  000c6a28: ldr.w r0,[r4,#0xf8]
  000c6a2c: ldr r1,[0x000c6b70]
  000c6a2e: ldr r2,[0x000c6b74]
  000c6a30: ldr r0,[r0,#0x4]
  000c6a32: ldr r0,[r0,#0x14]
  000c6a34: ldr r3,[r0,#0x0]
  000c6a36: ldr r6,[r3,#0x48]
  000c6a38: ldr r3,[0x000c6b78]
  000c6a3a: blx r6
  000c6a3c: ldr.w r0,[r4,#0xf8]
  000c6a40: ldr r1,[0x000c6b7c]
  000c6a42: ldr r2,[0x000c6b80]
  000c6a44: ldr r0,[r0,#0x4]
  000c6a46: ldr r0,[r0,#0x18]
  000c6a48: ldr r3,[r0,#0x0]
  000c6a4a: ldr r6,[r3,#0x48]
  000c6a4c: ldr r3,[0x000c6b84]
  000c6a4e: blx r6
  000c6a50: ldr.w r0,[r4,#0xf8]
  000c6a54: ldr r1,[0x000c6b88]
  000c6a56: ldr r2,[0x000c6b8c]
  000c6a58: ldr r0,[r0,#0x4]
  000c6a5a: ldr r0,[r0,#0x1c]
  000c6a5c: ldr r3,[r0,#0x0]
  000c6a5e: ldr r6,[r3,#0x48]
  000c6a60: ldr r3,[0x000c6b90]
  000c6a62: blx r6
  000c6a64: ldr r1,[0x000c6b94]
  000c6a66: mov r2,r5
  000c6a68: ldr.w r0,[r4,#0xf0]
  000c6a6c: ldr r3,[0x000c6b98]
  000c6a6e: blx 0x000725bc
  000c6a72: ldr.w r0,[r4,#0xf0]
  000c6a76: movs r1,#0x0
  000c6a78: movs r2,#0x0
  000c6a7a: movs r3,#0x0
  000c6a7c: ldr r0,[r0,#0x8]
  000c6a7e: blx 0x00073054
  000c6a82: movs r0,#0x1c
  000c6a84: blx 0x0006eb24
  000c6a88: mov r5,r0
  000c6a8a: movs r1,#0x19
  000c6a8c: movs r2,#0x0
  000c6a8e: mov r3,r4
  000c6a90: blx 0x00073edc
  000c6a94: ldr r4,[sp,#0x48]
  000c6a96: movs r0,#0x1c
  000c6a98: str r5,[r4,#0x28]
  000c6a9a: blx 0x0006eb24
  000c6a9e: mov r5,r0
  000c6aa0: movs r1,#0x7
  000c6aa2: movs r2,#0x1
  000c6aa4: mov r3,r4
  000c6aa6: blx 0x00073edc
  000c6aaa: b.w 0x000c9412
  000c6aae: ldr r0,[sp,#0x48]
  000c6ab0: ldr.w r0,[r0,#0x100]
  000c6ab4: ldr r0,[r0,#0x4]
  000c6ab6: ldr r1,[r0,#0x4]
  000c6ab8: cmp r1,#0x0
  000c6aba: beq.w 0x000cb9f0
  000c6abe: ldr r0,[r1,#0x0]
  000c6ac0: add r4,sp,#0x180
  000c6ac2: ldr r2,[r0,#0x28]
  000c6ac4: mov r0,r4
  000c6ac6: blx r2
  000c6ac8: ldr r0,[sp,#0x48]
  000c6aca: mov r1,r4
  000c6acc: b.w 0x000cb9fe
  000c6ad0: ldr r0,[sp,#0x48]
  000c6ad2: ldr.w r0,[r0,#0x100]
  000c6ad6: ldr r0,[r0,#0x4]
  000c6ad8: ldr r1,[r0,#0x4]
  000c6ada: cmp r1,#0x0
  000c6adc: beq.w 0x000cba7c
  000c6ae0: ldr r0,[r1,#0x0]
  000c6ae2: add r4,sp,#0x180
  000c6ae4: ldr r2,[r0,#0x28]
  000c6ae6: mov r0,r4
  000c6ae8: blx r2
  000c6aea: ldr r0,[sp,#0x48]
  000c6aec: mov r1,r4
  000c6aee: b.w 0x000cba8a
  000c6af2: ldr r0,[sp,#0x48]
  000c6af4: ldr.w r0,[r0,#0x100]
  000c6af8: ldr r0,[r0,#0x4]
  000c6afa: ldr r1,[r0,#0x4]
  000c6afc: cmp r1,#0x0
  000c6afe: beq.w 0x000cbbd8
  000c6b02: ldr r0,[r1,#0x0]
  000c6b04: add r4,sp,#0x180
  000c6b06: ldr r2,[r0,#0x28]
  000c6b08: mov r0,r4
  000c6b0a: blx r2
  000c6b0c: ldr r0,[sp,#0x48]
  000c6b0e: mov r1,r4
  000c6b10: b.w 0x000cbbe6
  000c6b9c: ldr r1,[0x000c6e68]
  000c6b9e: add r4,sp,#0x180
  000c6ba0: movs r2,#0x24
  000c6ba2: add r1,pc
  000c6ba4: mov r0,r4
  000c6ba6: blx 0x0006f1e4
  000c6baa: movs r0,#0x18
  000c6bac: blx 0x0006eb24
  000c6bb0: mov r5,r0
  000c6bb2: mov r1,r4
  000c6bb4: movs r2,#0x9
  000c6bb6: blx 0x00073eb8
  000c6bba: ldr r0,[sp,#0x48]
  000c6bbc: mov r4,r0
  000c6bbe: str.w r5,[r0,#0x108]
  000c6bc2: mov r0,r5
  000c6bc4: blx 0x00073f3c
  000c6bc8: str.w r0,[r4,#0x10c]
  000c6bcc: movs r0,#0xc
  000c6bce: blx 0x0006eb24
  000c6bd2: mov r5,r0
  000c6bd4: blx 0x00073d08
  000c6bd8: ldr r4,[sp,#0x48]
  000c6bda: movs r0,#0x9
  000c6bdc: mov r1,r5
  000c6bde: str.w r5,[r4,#0xf8]
  000c6be2: blx 0x00073d14
  000c6be6: ldr.w r8,[sp,#0x48]
  000c6bea: add.w r9,sp,#0x118
  000c6bee: add.w r10,sp,#0x140
  000c6bf2: vldr.32 s16,[pc,#0x278]
  000c6bf6: mov.w r11,#0x0
  000c6bfa: add.w r0,r4,#0x18c
  000c6bfe: str r0,[sp,#0x3c]
  000c6c00: b 0x000c6d22
  000c6c02: movs r0,#0x0
  000c6c04: movs r1,#0x1
  000c6c06: str r0,[sp,#0x0]
  000c6c08: movs r2,#0x0
  000c6c0a: strd r1,r0,[sp,#0x4]
  000c6c0e: mov r0,r8
  000c6c10: movs r1,#0x8
  000c6c12: movs r3,#0x18
  000c6c14: blx 0x00073e88
  000c6c18: ldr.w r1,[r8,#0xf8]
  000c6c1c: ldr r1,[r1,#0x4]
  000c6c1e: str.w r0,[r1,r11,lsl #0x2]
  000c6c22: movs r1,#0x1
  000c6c24: ldr.w r0,[r8,#0xf8]
  000c6c28: ldr r0,[r0,#0x4]
  000c6c2a: ldr.w r0,[r0,r11,lsl #0x2]
  000c6c2e: ldr r0,[r0,#0x4]
  000c6c30: blx 0x00073fb4
  000c6c34: ldr.w r0,[r8,#0xf8]
  000c6c38: movs r1,#0x0
  000c6c3a: ldr r0,[r0,#0x4]
  000c6c3c: ldr.w r0,[r0,r11,lsl #0x2]
  000c6c40: ldr r0,[r0,#0x4]
  000c6c42: blx 0x00073e94
  000c6c46: ldr.w r0,[r8,#0xf8]
  000c6c4a: ldr r0,[r0,#0x4]
  000c6c4c: ldr.w r0,[r0,r11,lsl #0x2]
  000c6c50: blx 0x000732dc
  000c6c54: ldr.w r1,[r8,#0xf0]
  000c6c58: mov r0,r9
  000c6c5a: blx 0x0007264c
  000c6c5e: ldr r2,[0x000c6e70]
  000c6c60: mov r0,r10
  000c6c62: mov r1,r9
  000c6c64: blx 0x0006ec74
  000c6c68: ldr r0,[sp,#0x3c]
  000c6c6a: mov r1,r10
  000c6c6c: blx 0x0006eb3c
  000c6c70: ldr r0,[0x000c6e74]
  000c6c72: ldr.w r1,[r8,#0xf8]
  000c6c76: add r0,pc
  000c6c78: ldr r2,[0x000c6e78]
  000c6c7a: vldr.32 s18,[r8,#0x18c]
  000c6c7e: ldr r4,[r0,#0x0]
  000c6c80: add r2,pc
  000c6c82: ldr r1,[r1,#0x4]
  000c6c84: ldr r6,[r2,#0x0]
  000c6c86: ldr r0,[r4,#0x0]
  000c6c88: ldr.w r5,[r1,r11,lsl #0x2]
  000c6c8c: movw r1,#0x1b58
  000c6c90: blx r6
  000c6c92: ldr r1,[r4,#0x0]
  000c6c94: vmov s22,r0
  000c6c98: vldr.32 s20,[r8,#0x190]
  000c6c9c: mov r0,r1
  000c6c9e: movw r1,#0x1b58
  000c6ca2: blx r6
  000c6ca4: vldr.32 s24,[r8,#0x194]
  000c6ca8: vcvt.f32.s32 s22,s22
  000c6cac: ldr r1,[r4,#0x0]
  000c6cae: vmov s0,r0
  000c6cb2: vcvt.f32.s32 s26,s0
  000c6cb6: mov r0,r1
  000c6cb8: movw r1,#0x1b58
  000c6cbc: blx r6
  000c6cbe: vmov s6,r0
  000c6cc2: vadd.f32 s0,s18,s16
  000c6cc6: vadd.f32 s2,s20,s16
  000c6cca: vcvt.f32.s32 s6,s6
  000c6cce: ldr r0,[r5,#0x0]
  000c6cd0: vadd.f32 s4,s24,s16
  000c6cd4: ldr r6,[r0,#0x48]
  000c6cd6: mov r0,r5
  000c6cd8: vadd.f32 s0,s0,s22
  000c6cdc: vadd.f32 s2,s2,s26
  000c6ce0: vadd.f32 s4,s4,s6
  000c6ce4: vmov r1,s0
  000c6ce8: vmov r2,s2
  000c6cec: vmov r3,s4
  000c6cf0: blx r6
  000c6cf2: ldr.w r1,[r8,#0xf8]
  000c6cf6: ldr.w r0,[r8,#0x10c]
  000c6cfa: ldr r1,[r1,#0x4]
  000c6cfc: ldr.w r4,[r1,r11,lsl #0x2]
  000c6d00: blx 0x00073f3c
  000c6d04: mov r1,r0
  000c6d06: mov r0,r4
  000c6d08: blx 0x00073f60
  000c6d0c: ldr.w r0,[r8,#0xf8]
  000c6d10: ldr r1,[0x000c6e7c]
  000c6d12: ldr r0,[r0,#0x4]
  000c6d14: ldr.w r0,[r0,r11,lsl #0x2]
  000c6d18: ldr r0,[r0,#0x4]
  000c6d1a: blx 0x000727e4
  000c6d1e: add.w r11,r11,#0x1
  000c6d22: cmp.w r11,#0x3
  000c6d26: bne.w 0x000c6c02
  000c6d2a: ldr r0,[0x000c6e80]
  000c6d2c: movs r5,#0x3
  000c6d2e: mov.w r8,#0x0
  000c6d32: mov.w r9,#0x1
  000c6d36: add r0,pc
  000c6d38: ldr r6,[sp,#0x48]
  000c6d3a: ldr.w r10,[r0,#0x0]
  000c6d3e: b 0x000c6d90
  000c6d40: ldr.w r0,[r10,#0x0]
  000c6d44: movs r1,#0x8
  000c6d46: blx 0x00071a34
  000c6d4a: movs r1,#0x2
  000c6d4c: cmp r5,#0xa
  000c6d4e: it cc
  000c6d50: mov.cc r1,#0x1
  000c6d52: mov r4,r0
  000c6d54: ldr.w r0,[r6,#0x108]
  000c6d58: cmp r5,#0x5
  000c6d5a: it cc
  000c6d5c: mov.cc r1,#0x0
  000c6d5e: blx 0x00073ec4
  000c6d62: strd r0,r9,[sp,#0x0]
  000c6d66: mov r0,r6
  000c6d68: movs r1,#0x8
  000c6d6a: movs r2,#0x0
  000c6d6c: mov r3,r4
  000c6d6e: str.w r8,[sp,#0x8]
  000c6d72: blx 0x00073e88
  000c6d76: ldr.w r1,[r6,#0xf8]
  000c6d7a: ldr r1,[r1,#0x4]
  000c6d7c: str.w r0,[r1,r5,lsl #0x2]
  000c6d80: ldr.w r0,[r6,#0xf8]
  000c6d84: ldr r0,[r0,#0x4]
  000c6d86: ldr.w r0,[r0,r5,lsl #0x2]
  000c6d8a: blx 0x000732dc
  000c6d8e: adds r5,#0x1
  000c6d90: ldr r0,[sp,#0x48]
  000c6d92: ldr.w r0,[r0,#0xf8]
  000c6d96: ldr r0,[r0,#0x0]
  000c6d98: cmp r5,r0
  000c6d9a: bcc 0x000c6d40
  000c6d9c: movs r0,#0x1c
  000c6d9e: blx 0x0006eb24
  000c6da2: mov r5,r0
  000c6da4: ldr r0,[sp,#0x48]
  000c6da6: movs r1,#0x12
  000c6da8: str r0,[sp,#0x0]
  000c6daa: mov r0,r5
  000c6dac: movs r2,#0x3
  000c6dae: movs r3,#0x9
  000c6db0: blx 0x00073f9c
  000c6db4: b.w 0x000cb9b8
  000c6db8: movs r0,#0xc
  000c6dba: blx 0x0006eb24
  000c6dbe: mov r5,r0
  000c6dc0: blx 0x00073d08
  000c6dc4: ldr r0,[sp,#0x48]
  000c6dc6: mov r1,r5
  000c6dc8: str.w r5,[r0,#0xf8]
  000c6dcc: movs r0,#0x7
  000c6dce: blx 0x00073d14
  000c6dd2: movs r0,#0x0
  000c6dd4: strd r0,r0,[sp,#0x180]
  000c6dd8: str r0,[sp,#0x188]
  000c6dda: movs r0,#0x18
  000c6ddc: blx 0x0006eb24
  000c6de0: mov r5,r0
  000c6de2: add r1,sp,#0x180
  000c6de4: movs r2,#0x3
  000c6de6: blx 0x00073eb8
  000c6dea: mov r0,r5
  000c6dec: blx 0x000728bc
  000c6df0: ldr r4,[sp,#0x48]
  000c6df2: mov r1,r0
  000c6df4: movw r2,#0x37a3
  000c6df8: movs r3,#0x1
  000c6dfa: mov r0,r4
  000c6dfc: blx 0x00073f6c
  000c6e00: ldr.w r1,[r4,#0xf8]
  000c6e04: ldr r1,[r1,#0x4]
  000c6e06: str r0,[r1,#0x0]
  000c6e08: mov r0,r5
  000c6e0a: blx 0x000728bc
  000c6e0e: mov r1,r0
  000c6e10: ldr r0,[r0,#0x0]
  000c6e12: ldr r2,[r0,#0x28]
  000c6e14: add r0,sp,#0x140
  000c6e16: blx r2
  000c6e18: ldr.w r0,[r4,#0xf8]
  000c6e1c: ldr r0,[r0,#0x4]
  000c6e1e: ldr r0,[r0,#0x0]
  000c6e20: blx 0x000732dc
  000c6e24: ldr r0,[0x000c6e84]
  000c6e26: movw r1,#0x1b9
  000c6e2a: add r0,pc
  000c6e2c: ldr r0,[r0,#0x0]
  000c6e2e: ldr r0,[r0,#0x0]
  000c6e30: blx 0x00072f70
  000c6e34: mov r1,r0
  000c6e36: ldr.w r0,[r4,#0xf8]
  000c6e3a: ldr r0,[r0,#0x4]
  000c6e3c: ldr r0,[r0,#0x0]
  000c6e3e: adds r0,#0x18
  000c6e40: blx 0x0006f2b0
  000c6e44: ldr r0,[0x000c6e88]
  000c6e46: mov.w r11,#0x1
  000c6e4a: movw r10,#0x2710
  000c6e4e: add r0,pc
  000c6e50: ldr r0,[r0,#0x0]
  000c6e52: str r0,[sp,#0x38]
  000c6e54: ldr r0,[0x000c6e8c]
  000c6e56: ldr r1,[0x000c6e90]
  000c6e58: add r0,pc
  000c6e5a: ldr r5,[sp,#0x48]
  000c6e5c: add r1,pc
  000c6e5e: ldr.w r9,[r0,#0x0]
  000c6e62: ldr.w r8,[r1,#0x0]
  000c6e66: b 0x000c6f9e
  000c6e94: ldr r0,[sp,#0x38]
  000c6e96: movs r1,#0x8
  000c6e98: ldr r0,[r0,#0x0]
  000c6e9a: blx 0x00071a34
  000c6e9e: mov r3,r0
  000c6ea0: movs r0,#0x0
  000c6ea2: movs r1,#0x1
  000c6ea4: str r0,[sp,#0x0]
  000c6ea6: strd r1,r0,[sp,#0x4]
  000c6eaa: mov r0,r5
  000c6eac: movs r1,#0x8
  000c6eae: movs r2,#0x0
  000c6eb0: blx 0x00073e88
  000c6eb4: ldr.w r1,[r5,#0xf8]
  000c6eb8: ldr r1,[r1,#0x4]
  000c6eba: str.w r0,[r1,r11,lsl #0x2]
  000c6ebe: movw r1,#0x4e20
  000c6ec2: ldr.w r0,[r9,#0x0]
  000c6ec6: vldr.32 s16,[sp,#0x140]
  000c6eca: blx r8
  000c6ecc: str r0,[sp,#0x3c]
  000c6ece: movs r1,#0x2
  000c6ed0: ldr.w r0,[r9,#0x0]
  000c6ed4: blx r8
  000c6ed6: mov r4,r0
  000c6ed8: ldr.w r0,[r9,#0x0]
  000c6edc: movw r1,#0x4e20
  000c6ee0: vldr.32 s18,[sp,#0x144]
  000c6ee4: blx r8
  000c6ee6: mov r6,r0
  000c6ee8: ldr.w r0,[r9,#0x0]
  000c6eec: movs r1,#0x2
  000c6eee: blx r8
  000c6ef0: cmp r0,#0x0
  000c6ef2: mov.w r0,#0xffffffff
  000c6ef6: it eq
  000c6ef8: mov.eq r0,#0x1
  000c6efa: cmp r4,#0x0
  000c6efc: mla r0,r0,r6,r10
  000c6f00: vmov s20,r0
  000c6f04: mov.w r0,#0xffffffff
  000c6f08: it eq
  000c6f0a: mov.eq r0,#0x1
  000c6f0c: ldr r1,[sp,#0x3c]
  000c6f0e: vldr.32 s24,[sp,#0x148]
  000c6f12: mla r0,r0,r1,r10
  000c6f16: movw r1,#0x4e20
  000c6f1a: vmov s22,r0
  000c6f1e: ldr.w r0,[r9,#0x0]
  000c6f22: blx r8
  000c6f24: mov r4,r0
  000c6f26: ldr.w r0,[r9,#0x0]
  000c6f2a: movs r1,#0x2
  000c6f2c: vcvt.f32.s32 s22,s22
  000c6f30: vcvt.f32.s32 s20,s20
  000c6f34: blx r8
  000c6f36: cmp r0,#0x0
  000c6f38: mov.w r0,#0xffffffff
  000c6f3c: it eq
  000c6f3e: mov.eq r0,#0x1
  000c6f40: vadd.f32 s2,s16,s22
  000c6f44: mla r0,r0,r4,r10
  000c6f48: vadd.f32 s4,s18,s20
  000c6f4c: vmov s0,r0
  000c6f50: vcvt.f32.s32 s0,s0
  000c6f54: vcvt.s32.f32 s2,s2
  000c6f58: vcvt.s32.f32 s4,s4
  000c6f5c: vadd.f32 s0,s24,s0
  000c6f60: vcvt.f32.s32 s2,s2
  000c6f64: vcvt.f32.s32 s4,s4
  000c6f68: vcvt.s32.f32 s0,s0
  000c6f6c: vmov r1,s2
  000c6f70: vmov r2,s4
  000c6f74: vcvt.f32.s32 s0,s0
  000c6f78: ldr.w r0,[r5,#0xf8]
  000c6f7c: ldr r0,[r0,#0x4]
  000c6f7e: ldr.w r0,[r0,r11,lsl #0x2]
  000c6f82: vmov r3,s0
  000c6f86: ldr r6,[r0,#0x0]
  000c6f88: ldr r6,[r6,#0x48]
  000c6f8a: blx r6
  000c6f8c: ldr.w r0,[r5,#0xf8]
  000c6f90: ldr r0,[r0,#0x4]
  000c6f92: ldr.w r0,[r0,r11,lsl #0x2]
  000c6f96: blx 0x000732dc
  000c6f9a: add.w r11,r11,#0x1
  000c6f9e: ldr r0,[sp,#0x48]
  000c6fa0: ldr.w r0,[r0,#0xf8]
  000c6fa4: ldr r0,[r0,#0x0]
  000c6fa6: cmp r11,r0
  000c6fa8: bcc.w 0x000c6e94
  000c6fac: movs r0,#0x1c
  000c6fae: blx 0x0006eb24
  000c6fb2: mov r5,r0
  000c6fb4: ldr r3,[sp,#0x48]
  000c6fb6: movs r1,#0x1b
  000c6fb8: movs r2,#0x4
  000c6fba: blx 0x00073edc
  000c6fbe: b.w 0x000cb9b8
  000c6fc2: ldr r0,[sp,#0x48]
  000c6fc4: add.w r8,sp,#0x180
  000c6fc8: ldr.w r5,[r0,#0xf0]
  000c6fcc: mov r4,r0
  000c6fce: mov r0,r8
  000c6fd0: mov r1,r5
  000c6fd2: blx 0x0007264c
  000c6fd6: ldr.w r0,[r4,#0xf0]
  000c6fda: add r6,sp,#0x118
  000c6fdc: ldr r1,[r0,#0x8]
  000c6fde: mov r0,r6
  000c6fe0: blx 0x00072574
  000c6fe4: add r4,sp,#0x140
  000c6fe6: ldr r2,[0x000c7308]
  000c6fe8: mov r1,r6
  000c6fea: mov r0,r4
  000c6fec: blx 0x0006ec74
  000c6ff0: add r0,sp,#0xe8
  000c6ff2: mov r1,r8
  000c6ff4: mov r2,r4
  000c6ff6: blx 0x0006ec38
  000c6ffa: add r3,sp,#0xe8
  000c6ffc: mov r0,r5
  000c6ffe: ldmia r3,{r1,r2,r3}
  000c7000: blx 0x00072cb8
  000c7004: movs r0,#0xc
  000c7006: blx 0x0006eb24
  000c700a: mov r5,r0
  000c700c: blx 0x00073d08
  000c7010: ldr r0,[sp,#0x48]
  000c7012: mov r1,r5
  000c7014: str.w r5,[r0,#0xf8]
  000c7018: movs r0,#0x9
  000c701a: blx 0x00073d14
  000c701e: ldr r0,[0x000c730c]
  000c7020: add r0,pc
  000c7022: vldr.64 d16,[r0]
  000c7026: ldr r0,[r0,#0x8]
  000c7028: str r0,[sp,#0x188]
  000c702a: movs r0,#0x18
  000c702c: vstr.64 d16,[sp,#0x180]
  000c7030: blx 0x0006eb24
  000c7034: mov r5,r0
  000c7036: add r1,sp,#0x180
  000c7038: movs r2,#0x3
  000c703a: blx 0x00073eb8
  000c703e: ldr r4,[sp,#0x48]
  000c7040: movs r0,#0x1
  000c7042: movs r1,#0x0
  000c7044: movs r2,#0x0
  000c7046: movs r3,#0x26
  000c7048: str.w r5,[r4,#0x110]
  000c704c: movs r5,#0x0
  000c704e: strd r5,r0,[sp,#0x0]
  000c7052: mov r0,r4
  000c7054: str r5,[sp,#0x8]
  000c7056: blx 0x00073e88
  000c705a: ldr.w r1,[r4,#0xf8]
  000c705e: ldr r1,[r1,#0x4]
  000c7060: str r0,[r1,#0x0]
  000c7062: movs r1,#0x1
  000c7064: ldr.w r0,[r4,#0xf8]
  000c7068: ldr r0,[r0,#0x4]
  000c706a: ldr r0,[r0,#0x0]
  000c706c: ldr r0,[r0,#0x4]
  000c706e: blx 0x00073fb4
  000c7072: ldr r0,[0x000c7310]
  000c7074: movw r1,#0x651
  000c7078: add r0,pc
  000c707a: ldr r0,[r0,#0x0]
  000c707c: ldr r0,[r0,#0x0]
  000c707e: blx 0x00072f70
  000c7082: mov r1,r0
  000c7084: ldr.w r0,[r4,#0xf8]
  000c7088: ldr r0,[r0,#0x4]
  000c708a: ldr r0,[r0,#0x0]
  000c708c: adds r0,#0x18
  000c708e: blx 0x0006f2b0
  000c7092: ldr.w r0,[r4,#0xf8]
  000c7096: movs r2,#0x0
  000c7098: movs r3,#0x0
  000c709a: ldr r0,[r0,#0x4]
  000c709c: ldr r0,[r0,#0x0]
  000c709e: ldr r1,[r0,#0x0]
  000c70a0: ldr r6,[r1,#0x48]
  000c70a2: movs r1,#0x0
  000c70a4: blx r6
  000c70a6: ldr.w r0,[r4,#0xf8]
  000c70aa: mov.w r1,#0x3f800000
  000c70ae: add r2,sp,#0x118
  000c70b0: ldr r0,[r0,#0x4]
  000c70b2: ldr r0,[r0,#0x0]
  000c70b4: ldr r0,[r0,#0x8]
  000c70b6: str r5,[sp,#0x144]
  000c70b8: str r1,[sp,#0x140]
  000c70ba: str r5,[sp,#0x148]
  000c70bc: strd r5,r1,[sp,#0x118]
  000c70c0: add r1,sp,#0x140
  000c70c2: str r5,[sp,#0x120]
  000c70c4: blx 0x000726ac
  000c70c8: ldr.w r1,[r4,#0xf8]
  000c70cc: ldr.w r0,[r4,#0x110]
  000c70d0: ldr r1,[r1,#0x4]
  000c70d2: ldr r6,[r1,#0x0]
  000c70d4: blx 0x00073f3c
  000c70d8: mov r1,r0
  000c70da: mov r0,r6
  000c70dc: blx 0x00073f60
  000c70e0: ldr.w r0,[r4,#0xf8]
  000c70e4: mov.w r9,#0x1
  000c70e8: ldr r1,[r0,#0x4]
  000c70ea: ldr r1,[r1,#0x0]
  000c70ec: str r5,[r1,#0x50]
  000c70ee: b 0x000c7222
  000c70f0: ldr r0,[0x000c7314]
  000c70f2: movs r1,#0x8
  000c70f4: add r0,pc
  000c70f6: ldr r0,[r0,#0x0]
  000c70f8: ldr r0,[r0,#0x0]
  000c70fa: blx 0x00071a34
  000c70fe: mov r3,r0
  000c7100: movs r1,#0x0
  000c7102: movs r0,#0x1
  000c7104: str r1,[sp,#0x0]
  000c7106: strd r0,r1,[sp,#0x4]
  000c710a: movs r1,#0x8
  000c710c: ldr r4,[sp,#0x48]
  000c710e: movs r2,#0x0
  000c7110: mov r0,r4
  000c7112: blx 0x00073e88
  000c7116: ldr.w r1,[r4,#0xf8]
  000c711a: ldr r1,[r1,#0x4]
  000c711c: ldr r2,[0x000c7318]
  000c711e: str.w r0,[r1,r9,lsl #0x2]
  000c7122: add r2,pc
  000c7124: ldr.w r0,[r4,#0xf8]
  000c7128: ldr r1,[0x000c731c]
  000c712a: ldr.w r8,[r2,#0x0]
  000c712e: ldr r2,[r0,#0x4]
  000c7130: add r1,pc
  000c7132: ldr.w r10,[r1,#0x0]
  000c7136: movw r1,#0x5dc
  000c713a: ldr.w r0,[r8,#0x0]
  000c713e: ldr.w r2,[r2,r9,lsl #0x2]
  000c7142: str r2,[sp,#0x3c]
  000c7144: blx r10
  000c7146: mov r4,r0
  000c7148: ldr.w r0,[r8,#0x0]
  000c714c: movs r1,#0x2
  000c714e: blx r10
  000c7150: mov r6,r0
  000c7152: ldr.w r0,[r8,#0x0]
  000c7156: movw r1,#0x5dc
  000c715a: blx r10
  000c715c: mov r5,r0
  000c715e: ldr.w r0,[r8,#0x0]
  000c7162: movs r1,#0x2
  000c7164: blx r10
  000c7166: cmp r0,#0x0
  000c7168: mov.w r0,#0xffffffff
  000c716c: add.w r1,r5,#0x3e8
  000c7170: it eq
  000c7172: mov.eq r0,#0x1
  000c7174: cmp r6,#0x0
  000c7176: mul r11,r0,r1
  000c717a: mov.w r0,#0xffffffff
  000c717e: add.w r1,r4,#0x3e8
  000c7182: it eq
  000c7184: mov.eq r0,#0x1
  000c7186: muls r0,r1
  000c7188: movw r1,#0x5dc
  000c718c: vmov s16,r0
  000c7190: ldr.w r0,[r8,#0x0]
  000c7194: blx r10
  000c7196: mov r4,r0
  000c7198: ldr.w r0,[r8,#0x0]
  000c719c: movs r1,#0x2
  000c719e: vcvt.f32.s32 s16,s16
  000c71a2: blx r10
  000c71a4: cmp r0,#0x0
  000c71a6: mov.w r0,#0xffffffff
  000c71aa: add.w r1,r4,#0x3e8
  000c71ae: ldr r5,[sp,#0x48]
  000c71b0: it eq
  000c71b2: mov.eq r0,#0x1
  000c71b4: vmov s0,r11
  000c71b8: muls r0,r1
  000c71ba: vmov r1,s16
  000c71be: vcvt.f32.s32 s0,s0
  000c71c2: vmov s2,r0
  000c71c6: vcvt.f32.s32 s2,s2
  000c71ca: ldr r0,[sp,#0x3c]
  000c71cc: vmov r2,s0
  000c71d0: ldr r3,[r0,#0x0]
  000c71d2: ldr r6,[r3,#0x48]
  000c71d4: vmov r3,s2
  000c71d8: blx r6
  000c71da: ldr.w r0,[r5,#0xf8]
  000c71de: movs r2,#0x0
  000c71e0: mov.w r1,#0x3f800000
  000c71e4: ldr r0,[r0,#0x4]
  000c71e6: ldr.w r0,[r0,r9,lsl #0x2]
  000c71ea: ldr r0,[r0,#0x8]
  000c71ec: str r2,[sp,#0x144]
  000c71ee: str r1,[sp,#0x140]
  000c71f0: str r2,[sp,#0x148]
  000c71f2: strd r2,r1,[sp,#0x118]
  000c71f6: add r1,sp,#0x140
  000c71f8: str r2,[sp,#0x120]
  000c71fa: add r2,sp,#0x118
  000c71fc: blx 0x000726ac
  000c7200: ldr.w r1,[r5,#0xf8]
  000c7204: ldr.w r0,[r5,#0x110]
  000c7208: ldr r1,[r1,#0x4]
  000c720a: ldr.w r4,[r1,r9,lsl #0x2]
  000c720e: blx 0x00073f3c
  000c7212: mov r1,r0
  000c7214: mov r0,r4
  000c7216: blx 0x00073f60
  000c721a: ldr.w r0,[r5,#0xf8]
  000c721e: add.w r9,r9,#0x1
  000c7222: ldr r0,[r0,#0x0]
  000c7224: cmp r9,r0
  000c7226: bcc.w 0x000c70f0
  000c722a: movs r0,#0x1c
  000c722c: blx 0x0006eb24
  000c7230: mov r5,r0
  000c7232: ldr r3,[sp,#0x48]
  000c7234: movs r1,#0x16
  000c7236: movs r2,#0x0
  000c7238: blx 0x00073edc
  000c723c: ldr r4,[sp,#0x48]
  000c723e: movs r0,#0x1c
  000c7240: str r5,[r4,#0x28]
  000c7242: blx 0x0006eb24
  000c7246: mov r5,r0
  000c7248: movs r1,#0x1
  000c724a: movs r2,#0x0
  000c724c: mov r3,r4
  000c724e: blx 0x00073edc
  000c7252: b.w 0x000c9412
  000c7256: movs r0,#0x0
  000c7258: strd r0,r0,[sp,#0x180]
  000c725c: str r0,[sp,#0x188]
  000c725e: movs r0,#0x18
  000c7260: blx 0x0006eb24
  000c7264: mov r5,r0
  000c7266: add r1,sp,#0x180
  000c7268: movs r2,#0x3
  000c726a: blx 0x00073eb8
  000c726e: ldr r0,[sp,#0x48]
  000c7270: str.w r5,[r0,#0x110]
  000c7274: movs r0,#0xc
  000c7276: blx 0x0006eb24
  000c727a: mov r5,r0
  000c727c: blx 0x00073d08
  000c7280: ldr r6,[sp,#0x48]
  000c7282: movs r0,#0x1
  000c7284: mov r1,r5
  000c7286: movs r4,#0x1
  000c7288: str.w r5,[r6,#0xf8]
  000c728c: blx 0x00073d14
  000c7290: movs r0,#0x0
  000c7292: movs r1,#0x0
  000c7294: strd r0,r4,[sp,#0x0]
  000c7298: movs r2,#0x0
  000c729a: str r0,[sp,#0x8]
  000c729c: mov r0,r6
  000c729e: movs r3,#0x26
  000c72a0: blx 0x00073e88
  000c72a4: ldr.w r1,[r6,#0xf8]
  000c72a8: ldr r1,[r1,#0x4]
  000c72aa: str r0,[r1,#0x0]
  000c72ac: movs r1,#0x1
  000c72ae: ldr.w r0,[r6,#0xf8]
  000c72b2: ldr r0,[r0,#0x4]
  000c72b4: ldr r0,[r0,#0x0]
  000c72b6: ldr r0,[r0,#0x4]
  000c72b8: blx 0x00073fb4
  000c72bc: ldr r0,[0x000c7320]
  000c72be: movw r1,#0x651
  000c72c2: add r0,pc
  000c72c4: ldr r0,[r0,#0x0]
  000c72c6: ldr r0,[r0,#0x0]
  000c72c8: blx 0x00072f70
  000c72cc: mov r1,r0
  000c72ce: ldr.w r0,[r6,#0xf8]
  000c72d2: ldr r0,[r0,#0x4]
  000c72d4: ldr r0,[r0,#0x0]
  000c72d6: adds r0,#0x18
  000c72d8: blx 0x0006f2b0
  000c72dc: ldr.w r1,[r6,#0xf8]
  000c72e0: ldr.w r0,[r6,#0x110]
  000c72e4: ldr r1,[r1,#0x4]
  000c72e6: ldr r4,[r1,#0x0]
  000c72e8: blx 0x00073f3c
  000c72ec: mov r1,r0
  000c72ee: mov r0,r4
  000c72f0: blx 0x00073f60
  000c72f4: ldr.w r0,[r6,#0xf8]
  000c72f8: ldr r1,[0x000c7324]
  000c72fa: ldr r0,[r0,#0x4]
  000c72fc: ldr r0,[r0,#0x0]
  000c72fe: ldr r0,[r0,#0x4]
  000c7300: blx 0x000727e4
  000c7304: b.w 0x000cb9bc
  000c7328: ldr r0,[0x000c75d4]
  000c732a: add r0,pc
  000c732c: vldr.64 d16,[r0]
  000c7330: ldr r0,[r0,#0x8]
  000c7332: str r0,[sp,#0x188]
  000c7334: movs r0,#0x18
  000c7336: vstr.64 d16,[sp,#0x180]
  000c733a: blx 0x0006eb24
  000c733e: mov r5,r0
  000c7340: add r1,sp,#0x180
  000c7342: movs r2,#0x3
  000c7344: blx 0x00073eb8
  000c7348: ldr r0,[sp,#0x48]
  000c734a: mov r4,r0
  000c734c: str.w r5,[r0,#0x110]
  000c7350: mov r0,r5
  000c7352: blx 0x00073f3c
  000c7356: str.w r0,[r4,#0x108]
  000c735a: movs r0,#0xc
  000c735c: blx 0x0006eb24
  000c7360: mov r5,r0
  000c7362: blx 0x00073d08
  000c7366: ldr r4,[sp,#0x48]
  000c7368: movs r0,#0xb
  000c736a: mov r1,r5
  000c736c: str.w r5,[r4,#0xf8]
  000c7370: blx 0x00073d14
  000c7374: mov r0,r4
  000c7376: movs r1,#0x0
  000c7378: movw r2,#0x37a3
  000c737c: movs r3,#0x1
  000c737e: mov.w r9,#0x0
  000c7382: mov.w r11,#0x1
  000c7386: blx 0x00073f6c
  000c738a: ldr r1,[0x000c75d8]
  000c738c: ldr.w r2,[r4,#0xf8]
  000c7390: add r1,pc
  000c7392: ldr r2,[r2,#0x4]
  000c7394: ldr r1,[r1,#0x0]
  000c7396: str r1,[sp,#0x3c]
  000c7398: str r0,[r2,#0x0]
  000c739a: ldr r0,[r1,#0x0]
  000c739c: movw r1,#0x1b9
  000c73a0: blx 0x00072f70
  000c73a4: mov r1,r0
  000c73a6: ldr.w r0,[r4,#0xf8]
  000c73aa: ldr r0,[r0,#0x4]
  000c73ac: ldr r0,[r0,#0x0]
  000c73ae: adds r0,#0x18
  000c73b0: blx 0x0006f2b0
  000c73b4: ldr.w r0,[r4,#0xf8]
  000c73b8: vldr.32 s0,[sp,#0x180]
  000c73bc: vldr.32 s2,[sp,#0x184]
  000c73c0: vldr.32 s4,[sp,#0x188]
  000c73c4: vcvt.f32.s32 s0,s0
  000c73c8: ldr r0,[r0,#0x4]
  000c73ca: vcvt.f32.s32 s2,s2
  000c73ce: vcvt.f32.s32 s4,s4
  000c73d2: ldr r0,[r0,#0x0]
  000c73d4: ldr r3,[r0,#0x0]
  000c73d6: vmov r1,s0
  000c73da: vmov r2,s2
  000c73de: ldr r6,[r3,#0x48]
  000c73e0: vmov r3,s4
  000c73e4: blx r6
  000c73e6: ldr.w r0,[r4,#0xf8]
  000c73ea: movs r1,#0x0
  000c73ec: ldr r0,[r0,#0x4]
  000c73ee: ldr r0,[r0,#0x0]
  000c73f0: ldr r0,[r0,#0x4]
  000c73f2: blx 0x00073e94
  000c73f6: ldr.w r0,[r4,#0xf8]
  000c73fa: movs r1,#0x1
  000c73fc: ldr r0,[r0,#0x4]
  000c73fe: ldr r0,[r0,#0x0]
  000c7400: ldr r0,[r0,#0x4]
  000c7402: blx 0x00073fb4
  000c7406: ldr.w r0,[r4,#0xf8]
  000c740a: ldr r0,[r0,#0x4]
  000c740c: ldr r0,[r0,#0x0]
  000c740e: blx 0x000732dc
  000c7412: ldr r0,[0x000c75dc]
  000c7414: mov.w r8,#0x1
  000c7418: ldr r5,[sp,#0x48]
  000c741a: add r0,pc
  000c741c: ldr r6,[r0,#0x0]
  000c741e: ldr r0,[0x000c75e0]
  000c7420: mov r10,r0
  000c7422: b 0x000c7488
  000c7424: ldr r0,[r6,#0x0]
  000c7426: movs r1,#0x8
  000c7428: blx 0x00071a34
  000c742c: mov r4,r0
  000c742e: ldr.w r0,[r5,#0x110]
  000c7432: blx 0x000728bc
  000c7436: strd r0,r11,[sp,#0x0]
  000c743a: mov r0,r5
  000c743c: movs r1,#0x8
  000c743e: movs r2,#0x0
  000c7440: mov r3,r4
  000c7442: str.w r9,[sp,#0x8]
  000c7446: blx 0x00073e88
  000c744a: ldr.w r1,[r5,#0xf8]
  000c744e: ldr r1,[r1,#0x4]
  000c7450: str.w r0,[r1,r8,lsl #0x2]
  000c7454: ldr.w r0,[r5,#0xf8]
  000c7458: ldr r0,[r0,#0x4]
  000c745a: ldr.w r0,[r0,r8,lsl #0x2]
  000c745e: blx 0x000732dc
  000c7462: ldr.w r0,[r5,#0xf8]
  000c7466: cmp.w r8,#0x5
  000c746a: mov r1,r10
  000c746c: ldr r0,[r0,#0x4]
  000c746e: ldr.w r0,[r0,r8,lsl #0x2]
  000c7472: str.w r10,[r0,#0x128]
  000c7476: bcc 0x000c7484
  000c7478: ldr r1,[r0,#0x0]
  000c747a: ldr r4,[r1,#0x48]
  000c747c: ldr r1,[0x000c75e4]
  000c747e: mov r2,r1
  000c7480: mov r3,r1
  000c7482: blx r4
  000c7484: add.w r8,r8,#0x1
  000c7488: ldr r0,[sp,#0x48]
  000c748a: ldr.w r0,[r0,#0xf8]
  000c748e: ldr r1,[r0,#0x0]
  000c7490: subs r1,#0x1
  000c7492: cmp r8,r1
  000c7494: bcc 0x000c7424
  000c7496: ldr r1,[sp,#0x180]
  000c7498: movw r2,#0x7530
  000c749c: vldr.32 s0,[sp,#0x184]
  000c74a0: add r1,r2
  000c74a2: vldr.32 s2,[sp,#0x188]
  000c74a6: vcvt.f32.s32 s0,s0
  000c74aa: vmov s4,r1
  000c74ae: vcvt.f32.s32 s2,s2
  000c74b2: vcvt.f32.s32 s4,s4
  000c74b6: ldr r0,[r0,#0x4]
  000c74b8: ldr r0,[r0,#0x4]
  000c74ba: vmov r2,s0
  000c74be: vmov r3,s2
  000c74c2: ldr r6,[r0,#0x0]
  000c74c4: vmov r1,s4
  000c74c8: ldr r6,[r6,#0x48]
  000c74ca: blx r6
  000c74cc: movs r0,#0x0
  000c74ce: movs r1,#0x1
  000c74d0: strd r0,r1,[sp,#0x0]
  000c74d4: movs r1,#0x0
  000c74d6: ldr r6,[sp,#0x48]
  000c74d8: movs r2,#0x0
  000c74da: str r0,[sp,#0x8]
  000c74dc: movs r3,#0x1b
  000c74de: mov r0,r6
  000c74e0: blx 0x00073e88
  000c74e4: ldr.w r1,[r6,#0xf8]
  000c74e8: add.w r8,sp,#0x140
  000c74ec: ldrd r2,r1,[r1,#0x0]
  000c74f0: add.w r1,r1,r2, lsl #0x2
  000c74f4: str.w r0,[r1,#-0x4]
  000c74f8: mov r0,r8
  000c74fa: ldr.w r1,[r6,#0xf0]
  000c74fe: blx 0x0007264c
  000c7502: ldr.w r0,[r6,#0xf8]
  000c7506: add r4,sp,#0x118
  000c7508: ldr r2,[0x000c75e8]
  000c750a: ldrd r1,r0,[r0,#0x0]
  000c750e: add.w r0,r0,r1, lsl #0x2
  000c7512: ldr r1,[0x000c75ec]
  000c7514: ldr.w r5,[r0,#-0x4]
  000c7518: ldr r0,[0x000c75f0]
  000c751a: strd r0,r2,[sp,#0x1c0]
  000c751e: add r2,sp,#0x1c0
  000c7520: str r1,[sp,#0x1c8]
  000c7522: mov r0,r4
  000c7524: mov r1,r8
  000c7526: blx 0x0006f1cc
  000c752a: ldr r0,[r5,#0x0]
  000c752c: mov r1,r4
  000c752e: ldr r2,[r0,#0x44]
  000c7530: mov r0,r5
  000c7532: blx r2
  000c7534: ldr.w r0,[r6,#0xf8]
  000c7538: ldrd r1,r0,[r0,#0x0]
  000c753c: add.w r0,r0,r1, lsl #0x2
  000c7540: movs r1,#0x1
  000c7542: ldr.w r0,[r0,#-0x4]
  000c7546: ldr r0,[r0,#0x4]
  000c7548: blx 0x00073fb4
  000c754c: ldr r0,[sp,#0x3c]
  000c754e: movw r1,#0x661
  000c7552: ldr r0,[r0,#0x0]
  000c7554: blx 0x00072f70
  000c7558: mov r1,r0
  000c755a: ldr.w r0,[r6,#0xf8]
  000c755e: ldrd r2,r0,[r0,#0x0]
  000c7562: add.w r0,r0,r2, lsl #0x2
  000c7566: ldr.w r0,[r0,#-0x4]
  000c756a: adds r0,#0x18
  000c756c: blx 0x0006f2b0
  000c7570: ldr.w r0,[r6,#0xf8]
  000c7574: ldrd r1,r0,[r0,#0x0]
  000c7578: add.w r0,r0,r1, lsl #0x2
  000c757c: ldr r1,[0x000c75f4]
  000c757e: ldr.w r0,[r0,#-0x4]
  000c7582: ldr r0,[r0,#0x4]
  000c7584: blx 0x00073f78
  000c7588: ldr.w r1,[r6,#0xf8]
  000c758c: ldr.w r0,[r6,#0x108]
  000c7590: ldrd r2,r1,[r1,#0x0]
  000c7594: add.w r1,r1,r2, lsl #0x2
  000c7598: ldr.w r4,[r1,#-0x4]
  000c759c: blx 0x00073f3c
  000c75a0: mov r1,r0
  000c75a2: mov r0,r4
  000c75a4: blx 0x00073f60
  000c75a8: movs r0,#0x1c
  000c75aa: blx 0x0006eb24
  000c75ae: mov r5,r0
  000c75b0: movs r1,#0x16
  000c75b2: movs r2,#0x0
  000c75b4: mov r3,r6
  000c75b6: blx 0x00073edc
  000c75ba: ldr r4,[sp,#0x48]
  000c75bc: movs r0,#0x1c
  000c75be: str r5,[r4,#0x28]
  000c75c0: blx 0x0006eb24
  000c75c4: mov r5,r0
  000c75c6: movs r1,#0x1
  000c75c8: movs r2,#0x0
  000c75ca: mov r3,r4
  000c75cc: blx 0x00073edc
  000c75d0: b.w 0x000c9412
  000c75f8: ldr r0,[sp,#0x48]
  000c75fa: ldr.w r0,[r0,#0xec]
  000c75fe: blx 0x00073ccc
  000c7602: ldr r0,[r0,#0x4]
  000c7604: add r4,sp,#0x140
  000c7606: ldr r1,[r0,#0x4]
  000c7608: mov r0,r4
  000c760a: blx 0x000720b8
  000c760e: mov r0,r4
  000c7610: mov.w r1,#0x40800000
  000c7614: blx 0x00072628
  000c7618: vmov.f32 s0,0x41200000
  000c761c: vld1.32 {d2},[r4:64]!
  000c7620: movs r0,#0x14
  000c7622: vldr.32 s6,[r4]
  000c7626: add r4,sp,#0x180
  000c7628: mov r1,r4
  000c762a: vmul.f32 s2,s6,s0
  000c762e: vmul.f32 s8,s5,s0
  000c7632: vmul.f32 s7,s4,s0
  000c7636: vcvt.s32.f32 s0,s2
  000c763a: vcvt.s32.f32 s2,s8
  000c763e: vcvt.s32.f32 q8,q1
  000c7642: vst1.32 {d16,d17},[r1],r0
  000c7646: movs r0,#0x18
  000c7648: vstr.32 s0,[r1]
  000c764c: vstr.32 s2,[sp,#0x190]
  000c7650: blx 0x0006eb24
  000c7654: mov r5,r0
  000c7656: mov r1,r4
  000c7658: movs r2,#0x6
  000c765a: blx 0x00073eb8
  000c765e: ldr r0,[sp,#0x48]
  000c7660: str.w r5,[r0,#0x110]
  000c7664: movs r0,#0xc
  000c7666: blx 0x0006eb24
  000c766a: mov r5,r0
  000c766c: blx 0x00073d08
  000c7670: ldr r4,[sp,#0x48]
  000c7672: movs r0,#0x5
  000c7674: mov r1,r5
  000c7676: str.w r5,[r4,#0xf8]
  000c767a: blx 0x00073d14
  000c767e: mov.w r10,#0x0
  000c7682: mov.w r9,#0x1
  000c7686: mov r0,r4
  000c7688: movs r1,#0x0
  000c768a: movs r2,#0x0
  000c768c: movs r3,#0xc
  000c768e: strd r10,r9,[sp,#0x0]
  000c7692: str.w r10,[sp,#0x8]
  000c7696: blx 0x00073e88
  000c769a: ldr.w r1,[r4,#0xf8]
  000c769e: ldr r1,[r1,#0x4]
  000c76a0: str r0,[r1,#0x0]
  000c76a2: movs r1,#0x1
  000c76a4: ldr.w r0,[r4,#0xf8]
  000c76a8: ldr r0,[r0,#0x4]
  000c76aa: ldr r0,[r0,#0x0]
  000c76ac: ldr r0,[r0,#0x4]
  000c76ae: blx 0x00073fb4
  000c76b2: ldr r0,[0x000c7a78]
  000c76b4: movw r1,#0x65f
  000c76b8: add r0,pc
  000c76ba: ldr r0,[r0,#0x0]
  000c76bc: ldr r0,[r0,#0x0]
  000c76be: blx 0x00072f70
  000c76c2: mov r1,r0
  000c76c4: ldr.w r0,[r4,#0xf8]
  000c76c8: ldr r0,[r0,#0x4]
  000c76ca: ldr r0,[r0,#0x0]
  000c76cc: adds r0,#0x18
  000c76ce: blx 0x0006f2b0
  000c76d2: ldr.w r1,[r4,#0xf8]
  000c76d6: ldr.w r0,[r4,#0x110]
  000c76da: ldr r1,[r1,#0x4]
  000c76dc: ldr r5,[r1,#0x0]
  000c76de: blx 0x00073f3c
  000c76e2: mov r1,r0
  000c76e4: mov r0,r5
  000c76e6: blx 0x00073f60
  000c76ea: ldr.w r0,[r4,#0xf8]
  000c76ee: add.w r8,sp,#0x140
  000c76f2: ldr r0,[r0,#0x4]
  000c76f4: ldr r0,[r0,#0x0]
  000c76f6: ldr r1,[r0,#0x0]
  000c76f8: ldr r2,[r1,#0x44]
  000c76fa: mov r1,r8
  000c76fc: blx r2
  000c76fe: ldr.w r0,[r4,#0xf8]
  000c7702: ldr r0,[r0,#0x4]
  000c7704: ldr r0,[r0,#0x0]
  000c7706: ldr r1,[r0,#0x0]
  000c7708: ldr r2,[r1,#0x1c]
  000c770a: ldr r1,[0x000c7a7c]
  000c770c: blx r2
  000c770e: add r5,sp,#0x118
  000c7710: mov r1,r8
  000c7712: mov r0,r5
  000c7714: blx 0x0006ec80
  000c7718: mov r0,r8
  000c771a: mov r1,r5
  000c771c: blx 0x0006eb3c
  000c7720: ldr.w r0,[r4,#0xf8]
  000c7724: mov.w r1,#0x3f800000
  000c7728: add r2,sp,#0x118
  000c772a: ldr r0,[r0,#0x4]
  000c772c: ldr r0,[r0,#0x0]
  000c772e: ldr r0,[r0,#0x8]
  000c7730: strd r10,r1,[sp,#0x118]
  000c7734: mov r1,r8
  000c7736: str.w r10,[sp,#0x120]
  000c773a: blx 0x000726ac
  000c773e: ldr r0,[0x000c7a80]
  000c7740: add r0,pc
  000c7742: vldr.64 d16,[r0]
  000c7746: ldr r0,[r0,#0x8]
  000c7748: str r0,[sp,#0x120]
  000c774a: movs r0,#0x18
  000c774c: vstr.64 d16,[sp,#0x118]
  000c7750: blx 0x0006eb24
  000c7754: mov r4,r0
  000c7756: add r1,sp,#0x118
  000c7758: movs r2,#0x3
  000c775a: blx 0x00073eb8
  000c775e: ldr r0,[sp,#0x48]
  000c7760: movs r5,#0x1
  000c7762: ldr r6,[sp,#0x48]
  000c7764: str.w r4,[r0,#0x10c]
  000c7768: b 0x000c779e
  000c776a: ldr r0,[0x000c7a84]
  000c776c: movs r1,#0x0
  000c776e: add r0,pc
  000c7770: ldr r0,[r0,#0x0]
  000c7772: ldr r0,[r0,#0x0]
  000c7774: blx 0x00071a34
  000c7778: mov r4,r0
  000c777a: ldr.w r0,[r6,#0x10c]
  000c777e: blx 0x000728bc
  000c7782: stm sp,{r0,r9,r10}
  000c7786: mov r0,r6
  000c7788: movs r1,#0x0
  000c778a: movs r2,#0x0
  000c778c: mov r3,r4
  000c778e: blx 0x00073e88
  000c7792: ldr.w r1,[r6,#0xf8]
  000c7796: ldr r1,[r1,#0x4]
  000c7798: str.w r0,[r1,r5,lsl #0x2]
  000c779c: adds r5,#0x1
  000c779e: ldr r0,[sp,#0x48]
  000c77a0: ldr.w r0,[r0,#0xf8]
  000c77a4: ldr r0,[r0,#0x0]
  000c77a6: cmp r5,r0
  000c77a8: bcc 0x000c776a
  000c77aa: movs r0,#0x1c
  000c77ac: blx 0x0006eb24
  000c77b0: mov r5,r0
  000c77b2: ldr r3,[sp,#0x48]
  000c77b4: movs r1,#0x16
  000c77b6: movs r2,#0x0
  000c77b8: blx 0x00073edc
  000c77bc: b.w 0x000cb9b8
  000c77c0: ldr r4,[sp,#0x48]
  000c77c2: add.w r8,sp,#0x180
  000c77c6: mov r0,r8
  000c77c8: ldr.w r5,[r4,#0xf0]
  000c77cc: mov r1,r5
  000c77ce: blx 0x0007264c
  000c77d2: ldr.w r0,[r4,#0xf0]
  000c77d6: add r6,sp,#0x118
  000c77d8: ldr r1,[r0,#0x8]
  000c77da: mov r0,r6
  000c77dc: blx 0x00072574
  000c77e0: add.w r9,sp,#0x140
  000c77e4: ldr r2,[0x000c7a88]
  000c77e6: mov r1,r6
  000c77e8: mov r0,r9
  000c77ea: blx 0x0006ec74
  000c77ee: add r0,sp,#0xdc
  000c77f0: mov r1,r8
  000c77f2: mov r2,r9
  000c77f4: blx 0x0006ec38
  000c77f8: add r3,sp,#0xdc
  000c77fa: mov r0,r5
  000c77fc: ldmia r3,{r1,r2,r3}
  000c77fe: blx 0x00072cb8
  000c7802: ldr.w r0,[r4,#0x100]
  000c7806: ldr r0,[r0,#0x4]
  000c7808: ldr r1,[r0,#0x4]
  000c780a: ldr r0,[r1,#0x0]
  000c780c: ldr r2,[r0,#0x28]
  000c780e: add r0,sp,#0x180
  000c7810: blx r2
  000c7812: ldr.w r1,[r4,#0xf0]
  000c7816: add r0,sp,#0x140
  000c7818: blx 0x0007264c
  000c781c: vldr.32 s0,[sp,#0x180]
  000c7820: movs r0,#0x18
  000c7822: vldr.32 s2,[sp,#0x184]
  000c7826: vldr.32 s4,[sp,#0x188]
  000c782a: vcvt.s32.f32 s0,s0
  000c782e: vcvt.s32.f32 s2,s2
  000c7832: vcvt.s32.f32 s4,s4
  000c7836: vstr.32 s0,[sp,#0x118]
  000c783a: vstr.32 s2,[sp,#0x11c]
  000c783e: vstr.32 s4,[sp,#0x120]
  000c7842: blx 0x0006eb24
  000c7846: mov r5,r0
  000c7848: add r1,sp,#0x118
  000c784a: movs r2,#0x3
  000c784c: blx 0x00073eb8
  000c7850: ldr r0,[sp,#0x48]
  000c7852: str.w r5,[r0,#0x110]
  000c7856: movs r0,#0xc
  000c7858: blx 0x0006eb24
  000c785c: mov r5,r0
  000c785e: blx 0x00073d08
  000c7862: ldr r6,[sp,#0x48]
  000c7864: movs r0,#0x1
  000c7866: mov r1,r5
  000c7868: movs r4,#0x1
  000c786a: str.w r5,[r6,#0xf8]
  000c786e: blx 0x00073d14
  000c7872: mov.w r9,#0x0
  000c7876: mov r0,r6
  000c7878: movs r1,#0x0
  000c787a: movs r2,#0x0
  000c787c: movs r3,#0xc
  000c787e: strd r9,r4,[sp,#0x0]
  000c7882: str.w r9,[sp,#0x8]
  000c7886: blx 0x00073e88
  000c788a: ldr.w r1,[r6,#0xf8]
  000c788e: ldr r1,[r1,#0x4]
  000c7890: str r0,[r1,#0x0]
  000c7892: movs r1,#0x1
  000c7894: ldr.w r0,[r6,#0xf8]
  000c7898: ldr r0,[r0,#0x4]
  000c789a: ldr r0,[r0,#0x0]
  000c789c: ldr r0,[r0,#0x4]
  000c789e: blx 0x00073fb4
  000c78a2: ldr r0,[0x000c7a8c]
  000c78a4: movw r1,#0x65f
  000c78a8: add r0,pc
  000c78aa: ldr r0,[r0,#0x0]
  000c78ac: ldr r0,[r0,#0x0]
  000c78ae: blx 0x00072f70
  000c78b2: mov r1,r0
  000c78b4: ldr.w r0,[r6,#0xf8]
  000c78b8: ldr r0,[r0,#0x4]
  000c78ba: ldr r0,[r0,#0x0]
  000c78bc: adds r0,#0x18
  000c78be: blx 0x0006f2b0
  000c78c2: ldr.w r1,[r6,#0xf8]
  000c78c6: ldr.w r0,[r6,#0x110]
  000c78ca: ldr r1,[r1,#0x4]
  000c78cc: ldr r4,[r1,#0x0]
  000c78ce: blx 0x00073f3c
  000c78d2: mov r1,r0
  000c78d4: mov r0,r4
  000c78d6: blx 0x00073f60
  000c78da: add r4,sp,#0xd0
  000c78dc: add.w r8,sp,#0x180
  000c78e0: add.w r10,sp,#0x140
  000c78e4: mov r0,r4
  000c78e6: mov r1,r8
  000c78e8: mov r2,r10
  000c78ea: blx 0x0006ec38
  000c78ee: add r5,sp,#0xf4
  000c78f0: mov r1,r4
  000c78f2: mov.w r2,#0x40800000
  000c78f6: mov r0,r5
  000c78f8: blx 0x00073024
  000c78fc: add r4,sp,#0x1c0
  000c78fe: mov r1,r10
  000c7900: mov r2,r5
  000c7902: mov r0,r4
  000c7904: blx 0x0006f1cc
  000c7908: add.w r5,r6,#0x18c
  000c790c: mov r1,r4
  000c790e: mov r0,r5
  000c7910: blx 0x0006eb3c
  000c7914: ldr.w r0,[r6,#0xf8]
  000c7918: ldr r0,[r0,#0x4]
  000c791a: ldr r0,[r0,#0x0]
  000c791c: ldr r1,[r0,#0x0]
  000c791e: ldr r2,[r1,#0x44]
  000c7920: mov r1,r5
  000c7922: blx r2
  000c7924: add r4,sp,#0xf4
  000c7926: mov r1,r8
  000c7928: mov r2,r10
  000c792a: mov r0,r4
  000c792c: blx 0x0006ec38
  000c7930: add r5,sp,#0x1c0
  000c7932: mov r1,r4
  000c7934: mov r0,r5
  000c7936: blx 0x0006ec80
  000c793a: ldr.w r0,[r6,#0xf8]
  000c793e: mov.w r1,#0x3f800000
  000c7942: add r2,sp,#0xf4
  000c7944: ldr r0,[r0,#0x4]
  000c7946: ldr r0,[r0,#0x0]
  000c7948: ldr r0,[r0,#0x8]
  000c794a: strd r9,r1,[sp,#0xf4]
  000c794e: mov r1,r5
  000c7950: str.w r9,[sp,#0xfc]
  000c7954: blx 0x000726ac
  000c7958: ldr.w r0,[r6,#0xf8]
  000c795c: ldr r0,[r0,#0x4]
  000c795e: ldr r0,[r0,#0x0]
  000c7960: ldr r4,[r0,#0x4]
  000c7962: mov r0,r4
  000c7964: blx 0x00072910
  000c7968: vmov s2,r0
  000c796c: mov r0,r4
  000c796e: vmov.f32 s0,0x40200000
  000c7972: vcvt.f32.s32 s2,s2
  000c7976: vmul.f32 s0,s2,s0
  000c797a: vcvt.s32.f32 s0,s0
  000c797e: vmov r1,s0
  000c7982: blx 0x000727e4
  000c7986: movs r0,#0x1c
  000c7988: blx 0x0006eb24
  000c798c: mov r5,r0
  000c798e: movs r1,#0x1
  000c7990: movs r2,#0x0
  000c7992: mov r3,r6
  000c7994: blx 0x00073edc
  000c7998: b.w 0x000cb9b8
  000c799c: ldr r0,[0x000c7a90]
  000c799e: add r4,sp,#0x180
  000c79a0: add r0,pc
  000c79a2: vld1.64 {d16,d17},[r0]!
  000c79a6: vldr.64 d18,[r0]
  000c79aa: mov r0,r4
  000c79ac: vst1.64 {d16,d17},[r0]!
  000c79b0: vstr.64 d18,[r0]
  000c79b4: movs r0,#0x18
  000c79b6: blx 0x0006eb24
  000c79ba: mov r5,r0
  000c79bc: mov r1,r4
  000c79be: movs r2,#0x6
  000c79c0: blx 0x00073eb8
  000c79c4: ldr r6,[sp,#0x48]
  000c79c6: movs r2,#0x0
  000c79c8: ldr r1,[0x000c7a94]
  000c79ca: mov.w r9,#0x0
  000c79ce: ldr r3,[0x000c7a98]
  000c79d0: ldr.w r0,[r6,#0xf0]
  000c79d4: str.w r5,[r6,#0x110]
  000c79d8: blx 0x000725bc
  000c79dc: ldr r0,[sp,#0x18c]
  000c79de: movw r1,#0x4e20
  000c79e2: vldr.32 s0,[sp,#0x190]
  000c79e6: add r4,sp,#0x1c0
  000c79e8: add r0,r1
  000c79ea: vldr.32 s2,[sp,#0x194]
  000c79ee: vcvt.f32.s32 s0,s0
  000c79f2: vmov s4,r0
  000c79f6: mov r0,r4
  000c79f8: vcvt.f32.s32 s2,s2
  000c79fc: vcvt.f32.s32 s4,s4
  000c7a00: vstr.32 s0,[sp,#0x11c]
  000c7a04: vstr.32 s2,[sp,#0x120]
  000c7a08: vstr.32 s4,[sp,#0x118]
  000c7a0c: ldr.w r1,[r6,#0xf0]
  000c7a10: blx 0x0007264c
  000c7a14: add r5,sp,#0x140
  000c7a16: add r1,sp,#0x118
  000c7a18: mov r2,r4
  000c7a1a: mov r0,r5
  000c7a1c: blx 0x0006ec38
  000c7a20: ldr.w r0,[r6,#0xf0]
  000c7a24: add.w r8,sp,#0x118
  000c7a28: mov r1,r5
  000c7a2a: ldr r4,[r0,#0x8]
  000c7a2c: mov r0,r8
  000c7a2e: blx 0x0006ec80
  000c7a32: mov.w r0,#0x3f800000
  000c7a36: add r2,sp,#0x1c0
  000c7a38: strd r9,r0,[sp,#0x1c0]
  000c7a3c: mov r0,r4
  000c7a3e: mov r1,r8
  000c7a40: str.w r9,[sp,#0x1c8]
  000c7a44: blx 0x000726ac
  000c7a48: movs r0,#0xc
  000c7a4a: blx 0x0006eb24
  000c7a4e: mov r5,r0
  000c7a50: blx 0x00073d08
  000c7a54: ldr r0,[sp,#0x48]
  000c7a56: mov r1,r5
  000c7a58: str.w r5,[r0,#0xf8]
  000c7a5c: movs r0,#0xc
  000c7a5e: blx 0x00073d14
  000c7a62: ldr r0,[0x000c7a9c]
  000c7a64: mov.w r11,#0x1
  000c7a68: ldr r6,[sp,#0x48]
  000c7a6a: mov.w r8,#0x0
  000c7a6e: add r0,pc
  000c7a70: ldr r5,[r0,#0x0]
  000c7a72: ldr r0,[0x000c7aa0]
  000c7a74: mov r10,r0
  000c7a76: b 0x000c7b1c
  000c7aa4: ldr r0,[r5,#0x0]
  000c7aa6: movs r1,#0x8
  000c7aa8: blx 0x00071a34
  000c7aac: mov r4,r0
  000c7aae: ldr.w r0,[r6,#0x110]
  000c7ab2: movs r1,#0x0
  000c7ab4: blx 0x00073ec4
  000c7ab8: strd r0,r11,[sp,#0x0]
  000c7abc: mov r0,r6
  000c7abe: movs r1,#0x8
  000c7ac0: movs r2,#0x0
  000c7ac2: mov r3,r4
  000c7ac4: str.w r9,[sp,#0x8]
  000c7ac8: blx 0x00073e88
  000c7acc: ldr.w r1,[r6,#0xf8]
  000c7ad0: ldr r1,[r1,#0x4]
  000c7ad2: str.w r0,[r1,r8,lsl #0x2]
  000c7ad6: movs r1,#0x1
  000c7ad8: ldr.w r0,[r6,#0xf8]
  000c7adc: ldr r0,[r0,#0x4]
  000c7ade: ldr.w r0,[r0,r8,lsl #0x2]
  000c7ae2: ldr r0,[r0,#0x4]
  000c7ae4: blx 0x00073e94
  000c7ae8: ldr.w r0,[r6,#0xf8]
  000c7aec: ldr r0,[r0,#0x4]
  000c7aee: ldr.w r0,[r0,r8,lsl #0x2]
  000c7af2: blx 0x000732dc
  000c7af6: cmp.w r8,#0x4
  000c7afa: bcc 0x000c7b18
  000c7afc: ldr r0,[sp,#0x48]
  000c7afe: ldr.w r0,[r0,#0xf8]
  000c7b02: ldr r0,[r0,#0x4]
  000c7b04: ldr.w r0,[r0,r8,lsl #0x2]
  000c7b08: strd r10,r10,[sp,#0x118]
  000c7b0c: str.w r10,[sp,#0x120]
  000c7b10: ldr r1,[r0,#0x0]
  000c7b12: ldr r2,[r1,#0x44]
  000c7b14: add r1,sp,#0x118
  000c7b16: blx r2
  000c7b18: add.w r8,r8,#0x1
  000c7b1c: cmp.w r8,#0x8
  000c7b20: bne 0x000c7aa4
  000c7b22: ldr r0,[0x000c7ec8]
  000c7b24: movs r6,#0x8
  000c7b26: add r0,pc
  000c7b28: ldr r0,[r0,#0x0]
  000c7b2a: str r0,[sp,#0x34]
  000c7b2c: ldr r0,[0x000c7ecc]
  000c7b2e: add r0,pc
  000c7b30: ldr r0,[r0,#0x0]
  000c7b32: str r0,[sp,#0x30]
  000c7b34: ldr r0,[0x000c7ed0]
  000c7b36: ldr.w r8,[sp,#0x34]
  000c7b3a: add r0,pc
  000c7b3c: ldr r0,[r0,#0x0]
  000c7b3e: str r0,[sp,#0x2c]
  000c7b40: ldrd r9,r10,[sp,#0x2c]
  000c7b44: b 0x000c7c2c
  000c7b46: movs r4,#0x0
  000c7b48: movs r0,#0x1
  000c7b4a: str r4,[sp,#0x0]
  000c7b4c: movs r1,#0x0
  000c7b4e: strd r0,r4,[sp,#0x4]
  000c7b52: movs r2,#0x1
  000c7b54: ldr r5,[sp,#0x48]
  000c7b56: movs r3,#0xf
  000c7b58: mov r0,r5
  000c7b5a: blx 0x00073e88
  000c7b5e: ldr.w r1,[r5,#0xf8]
  000c7b62: ldr r1,[r1,#0x4]
  000c7b64: str.w r0,[r1,r6,lsl #0x2]
  000c7b68: movs r1,#0x1
  000c7b6a: ldr.w r0,[r5,#0xf8]
  000c7b6e: ldr r0,[r0,#0x4]
  000c7b70: ldr.w r0,[r0,r6,lsl #0x2]
  000c7b74: ldr r0,[r0,#0x4]
  000c7b76: blx 0x00073fb4
  000c7b7a: ldr.w r0,[r5,#0xf8]
  000c7b7e: ldr r0,[r0,#0x4]
  000c7b80: ldr.w r1,[r0,r6,lsl #0x2]
  000c7b84: str r4,[r1,#0x50]
  000c7b86: movs r1,#0x1
  000c7b88: ldr.w r0,[r0,r6,lsl #0x2]
  000c7b8c: blx 0x00073f54
  000c7b90: ldr.w r1,[r5,#0xf8]
  000c7b94: ldr.w r0,[r5,#0x110]
  000c7b98: ldr r1,[r1,#0x4]
  000c7b9a: ldr.w r1,[r1,r6,lsl #0x2]
  000c7b9e: str r1,[sp,#0x3c]
  000c7ba0: movs r1,#0x1
  000c7ba2: blx r8
  000c7ba4: ldr.w r0,[r0,#0x124]
  000c7ba8: movw r1,#0x4e20
  000c7bac: str r0,[sp,#0x38]
  000c7bae: ldr.w r0,[r9,#0x0]
  000c7bb2: blx r10
  000c7bb4: mov r4,r0
  000c7bb6: ldr r0,[sp,#0x48]
  000c7bb8: movs r1,#0x1
  000c7bba: ldr.w r0,[r0,#0x110]
  000c7bbe: blx r8
  000c7bc0: ldr.w r11,[r0,#0x128]
  000c7bc4: movw r1,#0x4e20
  000c7bc8: ldr.w r0,[r9,#0x0]
  000c7bcc: blx r10
  000c7bce: add r0,r11
  000c7bd0: movw r11,#0x2710
  000c7bd4: sub.w r5,r0,r11
  000c7bd8: ldr r0,[sp,#0x38]
  000c7bda: movs r1,#0x1
  000c7bdc: add r0,r4
  000c7bde: sub.w r0,r0,r11
  000c7be2: vmov s16,r0
  000c7be6: ldr r0,[sp,#0x48]
  000c7be8: ldr.w r0,[r0,#0x110]
  000c7bec: blx r8
  000c7bee: ldr.w r4,[r0,#0x12c]
  000c7bf2: vcvt.f32.s32 s16,s16
  000c7bf6: ldr.w r0,[r9,#0x0]
  000c7bfa: vmov s0,r5
  000c7bfe: movw r1,#0x4e20
  000c7c02: vcvt.f32.s32 s18,s0
  000c7c06: blx r10
  000c7c08: add r0,r4
  000c7c0a: vmov r1,s16
  000c7c0e: sub.w r0,r0,r11
  000c7c12: vmov r2,s18
  000c7c16: vmov s0,r0
  000c7c1a: vcvt.f32.s32 s0,s0
  000c7c1e: ldr r0,[sp,#0x3c]
  000c7c20: ldr r5,[r0,#0x0]
  000c7c22: ldr r5,[r5,#0x48]
  000c7c24: vmov r3,s0
  000c7c28: blx r5
  000c7c2a: adds r6,#0x1
  000c7c2c: ldr r0,[sp,#0x48]
  000c7c2e: ldr.w r0,[r0,#0xf8]
  000c7c32: ldr r0,[r0,#0x0]
  000c7c34: cmp r6,r0
  000c7c36: bcc 0x000c7b46
  000c7c38: movs r0,#0x1c
  000c7c3a: blx 0x0006eb24
  000c7c3e: mov r5,r0
  000c7c40: ldr r0,[sp,#0x48]
  000c7c42: movs r1,#0x12
  000c7c44: str r0,[sp,#0x0]
  000c7c46: mov r0,r5
  000c7c48: movs r2,#0x0
  000c7c4a: movs r3,#0x8
  000c7c4c: blx 0x00073f9c
  000c7c50: ldr r0,[sp,#0x48]
  000c7c52: mov r4,r0
  000c7c54: str r5,[r0,#0x28]
  000c7c56: movs r0,#0x1c
  000c7c58: blx 0x0006eb24
  000c7c5c: mov r5,r0
  000c7c5e: movs r1,#0x12
  000c7c60: movs r2,#0x8
  000c7c62: movs r3,#0xc
  000c7c64: str r4,[sp,#0x0]
  000c7c66: blx 0x00073f9c
  000c7c6a: b.w 0x000c9412
  000c7c6e: adr r0,[0xc7ed8]
  000c7c70: vmov.i32 q8,#0x0
  000c7c74: add r1,sp,#0x180
  000c7c76: vld1.64 {d18,d19},[r0]
  000c7c7a: orr r0,r1,#0x4
  000c7c7e: vst1.32 {d16,d17},[r0]
  000c7c82: add.w r0,r1,#0x18
  000c7c86: vst1.64 {d16,d17},[r0]
  000c7c8a: add.w r0,r1,#0x28
  000c7c8e: vst1.64 {d18,d19},[r0]
  000c7c92: mov.w r0,#0x3f800000
  000c7c96: str r0,[sp,#0x180]
  000c7c98: str r0,[sp,#0x194]
  000c7c9a: str r0,[sp,#0x1b8]
  000c7c9c: movs r0,#0x0
  000c7c9e: strd r0,r0,[sp,#0x140]
  000c7ca2: str r0,[sp,#0x148]
  000c7ca4: movs r0,#0xc
  000c7ca6: blx 0x0006eb24
  000c7caa: mov r5,r0
  000c7cac: blx 0x00073d08
  000c7cb0: ldr r0,[sp,#0x48]
  000c7cb2: mov r1,r5
  000c7cb4: str.w r5,[r0,#0xf8]
  000c7cb8: movs r0,#0x16
  000c7cba: blx 0x00073d14
  000c7cbe: ldr r0,[0x000c7ee8]
  000c7cc0: movs r4,#0x0
  000c7cc2: ldr.w r10,[sp,#0x48]
  000c7cc6: mov.w r11,#0x0
  000c7cca: add r0,pc
  000c7ccc: str r0,[sp,#0x38]
  000c7cce: b 0x000c7d7c
  000c7cd0: mov r0,r10
  000c7cd2: movs r1,#0x0
  000c7cd4: movw r2,#0x381b
  000c7cd8: movs r3,#0x1
  000c7cda: blx 0x00073f6c
  000c7cde: ldr.w r1,[r10,#0xf8]
  000c7ce2: add r5,sp,#0x180
  000c7ce4: ldr r2,[sp,#0x38]
  000c7ce6: ldr r1,[r1,#0x4]
  000c7ce8: add r2,r11
  000c7cea: ldrd r6,r3,[r2,#0x14]
  000c7cee: str r0,[r1,r4]
  000c7cf0: mov r1,r5
  000c7cf2: ldr r0,[r2,#0x4]
  000c7cf4: str r6,[sp,#0x1c4]
  000c7cf6: add r6,sp,#0x118
  000c7cf8: str r0,[sp,#0x3c]
  000c7cfa: ldrd r8,r9,[r2,#0x8]
  000c7cfe: ldr r0,[r2,#0x10]
  000c7d00: add r2,sp,#0x1c0
  000c7d02: str r0,[sp,#0x1c0]
  000c7d04: mov r0,r6
  000c7d06: str r3,[sp,#0x1c8]
  000c7d08: blx 0x0006f694
  000c7d0c: add r5,sp,#0x140
  000c7d0e: mov r1,r6
  000c7d10: mov r0,r5
  000c7d12: blx 0x0006eb3c
  000c7d16: ldr.w r0,[r10,#0xf8]
  000c7d1a: mov r1,r5
  000c7d1c: ldr r0,[r0,#0x4]
  000c7d1e: ldr r0,[r0,r4]
  000c7d20: ldr r0,[r0,#0x8]
  000c7d22: blx 0x00073dd4
  000c7d26: ldr r0,[sp,#0x3c]
  000c7d28: add r1,sp,#0x180
  000c7d2a: add r2,sp,#0x1c0
  000c7d2c: str.w r8,[sp,#0x1c4]
  000c7d30: str r0,[sp,#0x1c0]
  000c7d32: mov r0,r6
  000c7d34: str.w r9,[sp,#0x1c8]
  000c7d38: blx 0x0006f694
  000c7d3c: mov r0,r5
  000c7d3e: mov r1,r6
  000c7d40: blx 0x0006eb3c
  000c7d44: ldr.w r0,[r10,#0xf8]
  000c7d48: adr r1,[0xc7eec]
  000c7d4a: cmp.w r11,#0x0
  000c7d4e: ldr r0,[r0,#0x4]
  000c7d50: ldr r0,[r0,r4]
  000c7d52: it eq
  000c7d54: add.eq r1,#0x4
  000c7d56: vldr.32 s0,[r1]
  000c7d5a: vldr.32 s2,[sp,#0x140]
  000c7d5e: ldrd r1,r2,[sp,#0x144]
  000c7d62: vadd.f32 s0,s0,s2
  000c7d66: strd r1,r2,[sp,#0x11c]
  000c7d6a: vstr.32 s0,[sp,#0x118]
  000c7d6e: ldr r1,[r0,#0x0]
  000c7d70: ldr r2,[r1,#0x44]
  000c7d72: mov r1,r6
  000c7d74: blx r2
  000c7d76: adds r4,#0x4
  000c7d78: add.w r11,r11,#0x1c
  000c7d7c: cmp.w r11,#0x38
  000c7d80: bne 0x000c7cd0
  000c7d82: ldr r0,[0x000c7ef4]
  000c7d84: movs r5,#0x2
  000c7d86: ldr r4,[0x000c7ef8]
  000c7d88: add r0,pc
  000c7d8a: ldr r0,[r0,#0x0]
  000c7d8c: str r0,[sp,#0x3c]
  000c7d8e: ldr r0,[0x000c7efc]
  000c7d90: ldr r1,[0x000c7f00]
  000c7d92: add r0,pc
  000c7d94: ldr.w r11,[sp,#0x48]
  000c7d98: add r1,pc
  000c7d9a: ldr r0,[r0,#0x0]
  000c7d9c: str r0,[sp,#0x38]
  000c7d9e: ldr r0,[r1,#0x0]
  000c7da0: str r0,[sp,#0x34]
  000c7da2: ldrd r10,r9,[sp,#0x34]
  000c7da6: b 0x000c7e88
  000c7da8: ldr r0,[sp,#0x3c]
  000c7daa: movs r1,#0x8
  000c7dac: ldr r0,[r0,#0x0]
  000c7dae: blx 0x00071a34
  000c7db2: mov r3,r0
  000c7db4: movs r1,#0x0
  000c7db6: movs r0,#0x1
  000c7db8: str r1,[sp,#0x0]
  000c7dba: strd r0,r1,[sp,#0x4]
  000c7dbe: mov r0,r11
  000c7dc0: movs r1,#0x8
  000c7dc2: movs r2,#0x0
  000c7dc4: blx 0x00073e88
  000c7dc8: ldr.w r1,[r11,#0xf8]
  000c7dcc: ldr r1,[r1,#0x4]
  000c7dce: str.w r0,[r1,r5,lsl #0x2]
  000c7dd2: movs r1,#0x1
  000c7dd4: ldr.w r0,[r11,#0xf8]
  000c7dd8: ldr r0,[r0,#0x4]
  000c7dda: ldr.w r0,[r0,r5,lsl #0x2]
  000c7dde: ldr r0,[r0,#0x4]
  000c7de0: blx 0x00073e94
  000c7de4: ldr.w r0,[r11,#0xf8]
  000c7de8: mov.w r1,#0x7d0
  000c7dec: ldr r0,[r0,#0x4]
  000c7dee: ldr.w r6,[r0,r5,lsl #0x2]
  000c7df2: ldr.w r0,[r9,#0x0]
  000c7df6: blx r10
  000c7df8: mov r8,r0
  000c7dfa: ldr.w r0,[r9,#0x0]
  000c7dfe: movw r1,#0x2710
  000c7e02: blx r10
  000c7e04: movw r1,#0x1388
  000c7e08: subs r0,r0,r1
  000c7e0a: movw r1,#0x2710
  000c7e0e: vmov s0,r0
  000c7e12: add.w r0,r8,r4
  000c7e16: vmov s2,r0
  000c7e1a: ldr.w r0,[r9,#0x0]
  000c7e1e: vcvt.f32.s32 s16,s2
  000c7e22: vcvt.f32.s32 s18,s0
  000c7e26: blx r10
  000c7e28: ldr r1,[0x000c7f04]
  000c7e2a: add r0,r1
  000c7e2c: vmov s0,r0
  000c7e30: vcvt.f32.s32 s0,s0
  000c7e34: vstr.32 s16,[sp,#0x118]
  000c7e38: vstr.32 s18,[sp,#0x11c]
  000c7e3c: vstr.32 s0,[sp,#0x120]
  000c7e40: ldr r0,[r6,#0x0]
  000c7e42: ldr r2,[r0,#0x44]
  000c7e44: mov r0,r6
  000c7e46: add r6,sp,#0x118
  000c7e48: mov r1,r6
  000c7e4a: blx r2
  000c7e4c: ldr.w r0,[r11,#0xf8]
  000c7e50: movs r2,#0x0
  000c7e52: ldr r1,[0x000c7f08]
  000c7e54: ldr r0,[r0,#0x4]
  000c7e56: ldr.w r0,[r0,r5,lsl #0x2]
  000c7e5a: ldr r0,[r0,#0x8]
  000c7e5c: strd r2,r2,[sp,#0x118]
  000c7e60: str r1,[sp,#0x120]
  000c7e62: mov.w r1,#0x3f800000
  000c7e66: strd r2,r1,[sp,#0x1c0]
  000c7e6a: mov r1,r6
  000c7e6c: str r2,[sp,#0x1c8]
  000c7e6e: add r2,sp,#0x1c0
  000c7e70: blx 0x000726ac
  000c7e74: ldr.w r0,[r11,#0xf8]
  000c7e78: ldr r0,[r0,#0x4]
  000c7e7a: ldr.w r0,[r0,r5,lsl #0x2]
  000c7e7e: blx 0x000732dc
  000c7e82: add.w r4,r4,#0x7d0
  000c7e86: adds r5,#0x1
  000c7e88: ldr r0,[sp,#0x48]
  000c7e8a: ldr.w r0,[r0,#0xf8]
  000c7e8e: ldr r0,[r0,#0x0]
  000c7e90: cmp r5,r0
  000c7e92: bcc 0x000c7da8
  000c7e94: b.w 0x000cb9bc
  000c7e98: movs r0,#0xc
  000c7e9a: blx 0x0006eb24
  000c7e9e: mov r5,r0
  000c7ea0: blx 0x00073d08
  000c7ea4: ldr r0,[sp,#0x48]
  000c7ea6: mov r1,r5
  000c7ea8: str.w r5,[r0,#0xf8]
  000c7eac: movs r0,#0x7
  000c7eae: blx 0x00073d14
  000c7eb2: ldr r0,[0x000c7f0c]
  000c7eb4: mov.w r9,#0x0
  000c7eb8: ldr r4,[sp,#0x48]
  000c7eba: mov.w r8,#0x1
  000c7ebe: add r0,pc
  000c7ec0: movs r6,#0x0
  000c7ec2: ldr r5,[r0,#0x0]
  000c7ec4: b 0x000c7f4a
  000c7f10: ldr r0,[r5,#0x0]
  000c7f12: movs r1,#0x9
  000c7f14: blx 0x00071a34
  000c7f18: mov r3,r0
  000c7f1a: mov r0,r4
  000c7f1c: movs r1,#0x9
  000c7f1e: movs r2,#0x0
  000c7f20: strd r9,r8,[sp,#0x0]
  000c7f24: str.w r9,[sp,#0x8]
  000c7f28: blx 0x00073e88
  000c7f2c: ldr.w r1,[r4,#0xf8]
  000c7f30: ldr r1,[r1,#0x4]
  000c7f32: str.w r0,[r1,r6,lsl #0x2]
  000c7f36: movs r1,#0x1
  000c7f38: ldr.w r0,[r4,#0xf8]
  000c7f3c: ldr r0,[r0,#0x4]
  000c7f3e: ldr.w r0,[r0,r6,lsl #0x2]
  000c7f42: ldr r0,[r0,#0x4]
  000c7f44: blx 0x00073e94
  000c7f48: adds r6,#0x1
  000c7f4a: ldr r0,[sp,#0x48]
  000c7f4c: ldr.w r0,[r0,#0xf8]
  000c7f50: ldr r0,[r0,#0x0]
  000c7f52: cmp r6,r0
  000c7f54: bcc 0x000c7f10
  000c7f56: b.w 0x000cb9bc
  000c7f5a: movs r0,#0xc
  000c7f5c: blx 0x0006eb24
  000c7f60: mov r5,r0
  000c7f62: blx 0x00073d08
  000c7f66: ldr r0,[0x000c8120]
  000c7f68: ldr r2,[sp,#0x48]
  000c7f6a: add r0,pc
  000c7f6c: ldr r1,[0x000c8124]
  000c7f6e: ldr r0,[r0,#0x0]
  000c7f70: str.w r5,[r2,#0xf8]
  000c7f74: movs r2,#0x0
  000c7f76: ldr r0,[r0,#0x0]
  000c7f78: strd r2,r2,[sp,#0x118]
  000c7f7c: str r1,[sp,#0x120]
  000c7f7e: movs r1,#0x65
  000c7f80: blx 0x00071c44
  000c7f84: mov r5,r0
  000c7f86: mov.w r0,#0x174
  000c7f8a: blx 0x0006eb24
  000c7f8e: mov r6,r0
  000c7f90: mov r1,r5
  000c7f92: blx 0x00073d2c
  000c7f96: ldr r0,[sp,#0x48]
  000c7f98: ldr.w r1,[r0,#0xf8]
  000c7f9c: mov r4,r0
  000c7f9e: mov r0,r6
  000c7fa0: blx 0x000740f8
  000c7fa4: add r6,sp,#0x180
  000c7fa6: mov r1,r5
  000c7fa8: mov r0,r6
  000c7faa: blx 0x000736a8
  000c7fae: ldr.w r0,[r4,#0xf8]
  000c7fb2: ldr r0,[r0,#0x4]
  000c7fb4: ldr r0,[r0,#0x0]
  000c7fb6: adds r0,#0x18
  000c7fb8: mov r1,r6
  000c7fba: blx 0x0006f2b0
  000c7fbe: add r0,sp,#0x180
  000c7fc0: blx 0x0006ee30
  000c7fc4: ldr r0,[sp,#0x48]
  000c7fc6: movs r2,#0x8
  000c7fc8: mov r4,r0
  000c7fca: ldr.w r0,[r0,#0xf8]
  000c7fce: ldr r0,[r0,#0x4]
  000c7fd0: ldr r0,[r0,#0x0]
  000c7fd2: ldr r1,[r0,#0x0]
  000c7fd4: str r2,[r0,#0x28]
  000c7fd6: ldr r2,[r1,#0x44]
  000c7fd8: add r1,sp,#0x118
  000c7fda: blx r2
  000c7fdc: ldr.w r0,[r4,#0xf8]
  000c7fe0: add r6,sp,#0x180
  000c7fe2: movs r4,#0x0
  000c7fe4: mov r1,r6
  000c7fe6: ldr r0,[r0,#0x4]
  000c7fe8: ldr r0,[r0,#0x0]
  000c7fea: ldr.w r0,[r0,#0x140]
  000c7fee: strd r4,r4,[sp,#0x180]
  000c7ff2: str r4,[sp,#0x188]
  000c7ff4: blx 0x00073dd4
  000c7ff8: cbz r5,0x000c8004
  000c7ffa: mov r0,r5
  000c7ffc: blx 0x0007360c
  000c8000: blx 0x0006eb48
  000c8004: adr.w r0,0xc7ed8
  000c8008: vmov.i32 q8,#0x0
  000c800c: vld1.64 {d18,d19},[r0]
  000c8010: orr r0,r6,#0x4
  000c8014: mov r1,r6
  000c8016: movs r2,#0x0
  000c8018: vst1.32 {d16,d17},[r0]
  000c801c: add.w r0,r6,#0x18
  000c8020: vst1.64 {d16,d17},[r0]
  000c8024: add.w r0,r6,#0x28
  000c8028: vst1.64 {d18,d19},[r0]
  000c802c: mov.w r0,#0x3f800000
  000c8030: ldr r3,[0x000c8128]
  000c8032: str r0,[sp,#0x180]
  000c8034: str r0,[sp,#0x194]
  000c8036: str r0,[sp,#0x1b8]
  000c8038: add r0,sp,#0x140
  000c803a: str r4,[sp,#0x0]
  000c803c: blx 0x00072094
  000c8040: ldr r5,[sp,#0x44]
  000c8042: ldr r0,[r5,#0x0]
  000c8044: strd r4,r4,[sp,#0x140]
  000c8048: str r4,[sp,#0x148]
  000c804a: blx 0x00071c5c
  000c804e: cmp r0,#0x14
  000c8050: ble.w 0x000cb77a
  000c8054: mov.w r0,#0x208
  000c8058: b.w 0x000cb786
  000c805c: movs r0,#0xc
  000c805e: blx 0x0006eb24
  000c8062: mov r5,r0
  000c8064: blx 0x00073d08
  000c8068: ldr r0,[0x000c812c]
  000c806a: ldr r1,[sp,#0x48]
  000c806c: add r0,pc
  000c806e: ldr r0,[r0,#0x0]
  000c8070: str.w r5,[r1,#0xf8]
  000c8074: movs r1,#0x65
  000c8076: ldr r0,[r0,#0x0]
  000c8078: blx 0x00071c44
  000c807c: mov r5,r0
  000c807e: mov.w r0,#0x174
  000c8082: blx 0x0006eb24
  000c8086: mov r6,r0
  000c8088: mov r1,r5
  000c808a: blx 0x00073d2c
  000c808e: ldr r0,[sp,#0x48]
  000c8090: ldr.w r1,[r0,#0xf8]
  000c8094: mov r4,r0
  000c8096: mov r0,r6
  000c8098: blx 0x000740f8
  000c809c: add r6,sp,#0x180
  000c809e: mov r1,r5
  000c80a0: mov r0,r6
  000c80a2: blx 0x000736a8
  000c80a6: ldr.w r0,[r4,#0xf8]
  000c80aa: ldr r0,[r0,#0x4]
  000c80ac: ldr r0,[r0,#0x0]
  000c80ae: adds r0,#0x18
  000c80b0: mov r1,r6
  000c80b2: blx 0x0006f2b0
  000c80b6: add r0,sp,#0x180
  000c80b8: blx 0x0006ee30
  000c80bc: ldr r0,[sp,#0x48]
  000c80be: cmp r5,#0x0
  000c80c0: mov.w r1,#0x8
  000c80c4: ldr.w r0,[r0,#0xf8]
  000c80c8: ldr r0,[r0,#0x4]
  000c80ca: ldr r0,[r0,#0x0]
  000c80cc: str r1,[r0,#0x28]
  000c80ce: beq 0x000c80da
  000c80d0: mov r0,r5
  000c80d2: blx 0x0007360c
  000c80d6: blx 0x0006eb48
  000c80da: ldr r6,[sp,#0x48]
  000c80dc: movs r4,#0x8
  000c80de: movs r5,#0x0
  000c80e0: mov.w r8,#0x1
  000c80e4: b 0x000c8102
  000c80e6: mov r0,r6
  000c80e8: movs r1,#0x9
  000c80ea: movs r2,#0x0
  000c80ec: movs r3,#0x8
  000c80ee: strd r5,r8,[sp,#0x0]
  000c80f2: str r5,[sp,#0x8]
  000c80f4: blx 0x00073e88
  000c80f8: ldr.w r1,[r6,#0xf8]
  000c80fc: blx 0x000740f8
  000c8100: subs r4,#0x1
  000c8102: cmp r4,#0x0
  000c8104: bne 0x000c80e6
  000c8106: b.w 0x000cb9bc
  000c810a: movs r0,#0x1c
  000c810c: blx 0x0006eb24
  000c8110: mov r5,r0
  000c8112: ldr r3,[sp,#0x48]
  000c8114: movs r1,#0x16
  000c8116: movs r2,#0x0
  000c8118: blx 0x00073edc
  000c811c: b.w 0x000cb9b8
  000c8130: ldr r0,[0x000c841c]
  000c8132: add r4,sp,#0x180
  000c8134: add r0,pc
  000c8136: vld1.64 {d16,d17},[r0]!
  000c813a: vldr.64 d18,[r0]
  000c813e: mov r0,r4
  000c8140: vst1.64 {d16,d17},[r0]!
  000c8144: vstr.64 d18,[r0]
  000c8148: movs r0,#0x18
  000c814a: blx 0x0006eb24
  000c814e: mov r5,r0
  000c8150: mov r1,r4
  000c8152: movs r2,#0x6
  000c8154: blx 0x00073eb8
  000c8158: ldr r0,[sp,#0x48]
  000c815a: movs r1,#0x1
  000c815c: str.w r5,[r0,#0x10c]
  000c8160: mov r0,r5
  000c8162: blx 0x00073f48
  000c8166: ldr r0,[0x000c8420]
  000c8168: add r4,sp,#0x140
  000c816a: add r0,pc
  000c816c: vld1.64 {d16,d17},[r0]!
  000c8170: vldr.64 d18,[r0]
  000c8174: mov r0,r4
  000c8176: vst1.64 {d16,d17},[r0]!
  000c817a: vstr.64 d18,[r0]
  000c817e: movs r0,#0x18
  000c8180: blx 0x0006eb24
  000c8184: mov r5,r0
  000c8186: mov r1,r4
  000c8188: movs r2,#0x6
  000c818a: blx 0x00073eb8
  000c818e: ldr r0,[sp,#0x48]
  000c8190: movs r1,#0x1
  000c8192: str.w r5,[r0,#0x110]
  000c8196: mov r0,r5
  000c8198: blx 0x00073f48
  000c819c: movs r0,#0xc
  000c819e: blx 0x0006eb24
  000c81a2: mov r5,r0
  000c81a4: blx 0x00073d08
  000c81a8: ldr r6,[sp,#0x48]
  000c81aa: movs r0,#0xc
  000c81ac: mov r1,r5
  000c81ae: str.w r5,[r6,#0xf8]
  000c81b2: blx 0x00073d14
  000c81b6: mov r0,r6
  000c81b8: movs r1,#0x0
  000c81ba: movw r2,#0x4260
  000c81be: movs r3,#0x0
  000c81c0: movs r4,#0x0
  000c81c2: blx 0x00073f6c
  000c81c6: ldr.w r1,[r6,#0xf8]
  000c81ca: ldr r1,[r1,#0x4]
  000c81cc: str r0,[r1,#0x0]
  000c81ce: movs r1,#0x0
  000c81d0: ldr.w r0,[r6,#0xf8]
  000c81d4: ldr r0,[r0,#0x4]
  000c81d6: ldr r0,[r0,#0x0]
  000c81d8: blx 0x00073f54
  000c81dc: mov.w r0,#0x114
  000c81e0: blx 0x0006eb24
  000c81e4: mov r5,r0
  000c81e6: ldr r2,[0x000c8424]
  000c81e8: movw r1,#0x1d4c
  000c81ec: movs r3,#0x0
  000c81ee: strd r4,r4,[sp,#0x0]
  000c81f2: mov r11,r2
  000c81f4: blx 0x00073d80
  000c81f8: mov.w r0,#0x1bc
  000c81fc: blx 0x0006eb24
  000c8200: mov r6,r0
  000c8202: movw r1,#0x495d
  000c8206: movs r2,#0x0
  000c8208: mov r3,r5
  000c820a: strd r4,r4,[sp,#0x0]
  000c820e: strd r4,r4,[sp,#0x8]
  000c8212: blx 0x0007414c
  000c8216: ldr r0,[sp,#0x48]
  000c8218: ldr.w r0,[r0,#0xf8]
  000c821c: ldr r0,[r0,#0x4]
  000c821e: str r6,[r0,#0x4]
  000c8220: movs r0,#0xc0
  000c8222: blx 0x0006eb24
  000c8226: mov r8,r0
  000c8228: ldr r0,[0x000c8428]
  000c822a: add r0,pc
  000c822c: ldr r4,[r0,#0x0]
  000c822e: ldr r2,[r4,#0x0]
  000c8230: mov r0,r8
  000c8232: movw r1,#0x5254
  000c8236: movs r3,#0x0
  000c8238: blx 0x0007207c
  000c823c: movs r0,#0xc0
  000c823e: blx 0x0006eb24
  000c8242: mov r5,r0
  000c8244: ldr r2,[r4,#0x0]
  000c8246: movw r1,#0x5574
  000c824a: movs r3,#0x0
  000c824c: blx 0x0007207c
  000c8250: movs r0,#0xc0
  000c8252: blx 0x0006eb24
  000c8256: mov r6,r0
  000c8258: ldr r2,[r4,#0x0]
  000c825a: movw r1,#0x563c
  000c825e: movs r3,#0x0
  000c8260: mov.w r9,#0x0
  000c8264: blx 0x0007207c
  000c8268: ldr r1,[r5,#0xc]
  000c826a: mov r0,r8
  000c826c: blx 0x000720e8
  000c8270: ldr r1,[r6,#0xc]
  000c8272: mov r0,r8
  000c8274: blx 0x000720e8
  000c8278: mov r0,r5
  000c827a: blx 0x00071fc8
  000c827e: blx 0x0006eb48
  000c8282: mov r0,r6
  000c8284: blx 0x00071fc8
  000c8288: blx 0x0006eb48
  000c828c: ldr r0,[sp,#0x48]
  000c828e: mov.w r2,#0xffffffff
  000c8292: movs r3,#0x0
  000c8294: mov r5,r0
  000c8296: ldr.w r0,[r0,#0xf8]
  000c829a: ldr r0,[r0,#0x4]
  000c829c: ldr r0,[r0,#0x4]
  000c829e: ldr r1,[r0,#0x0]
  000c82a0: ldr r6,[r1,#0x8]
  000c82a2: mov r1,r8
  000c82a4: blx r6
  000c82a6: ldr.w r0,[r5,#0xf8]
  000c82aa: ldr r3,[0x000c842c]
  000c82ac: ldr r1,[0x000c8430]
  000c82ae: ldr r0,[r0,#0x4]
  000c82b0: ldr r2,[0x000c8434]
  000c82b2: ldr r0,[r0,#0x4]
  000c82b4: strd r1,r3,[sp,#0x118]
  000c82b8: str r2,[sp,#0x120]
  000c82ba: ldr r1,[r0,#0x0]
  000c82bc: ldr r2,[r1,#0x44]
  000c82be: add r1,sp,#0x118
  000c82c0: blx r2
  000c82c2: ldr.w r0,[r5,#0xf8]
  000c82c6: movs r1,#0x0
  000c82c8: ldr r2,[0x000c8438]
  000c82ca: movs r3,#0x0
  000c82cc: ldr r0,[r0,#0x4]
  000c82ce: ldr r0,[r0,#0x4]
  000c82d0: ldr r0,[r0,#0x8]
  000c82d2: blx 0x00073054
  000c82d6: mov.w r0,#0x114
  000c82da: blx 0x0006eb24
  000c82de: mov r5,r0
  000c82e0: movw r1,#0x1d4c
  000c82e4: mov r2,r11
  000c82e6: movs r3,#0x0
  000c82e8: strd r9,r9,[sp,#0x0]
  000c82ec: blx 0x00073d80
  000c82f0: mov.w r0,#0x1bc
  000c82f4: blx 0x0006eb24
  000c82f8: mov r6,r0
  000c82fa: movs r0,#0x0
  000c82fc: movw r1,#0x495d
  000c8300: strd r0,r0,[sp,#0x0]
  000c8304: movs r2,#0x0
  000c8306: strd r0,r0,[sp,#0x8]
  000c830a: mov r0,r6
  000c830c: mov r3,r5
  000c830e: blx 0x0007414c
  000c8312: ldr r0,[sp,#0x48]
  000c8314: ldr.w r0,[r0,#0xf8]
  000c8318: ldr r0,[r0,#0x4]
  000c831a: str r6,[r0,#0x8]
  000c831c: movs r0,#0xc0
  000c831e: blx 0x0006eb24
  000c8322: mov r8,r0
  000c8324: ldr r2,[r4,#0x0]
  000c8326: movw r1,#0x5254
  000c832a: movs r3,#0x0
  000c832c: blx 0x0007207c
  000c8330: movs r0,#0xc0
  000c8332: blx 0x0006eb24
  000c8336: mov r9,r0
  000c8338: ldr r2,[r4,#0x0]
  000c833a: movw r1,#0x5574
  000c833e: movs r3,#0x0
  000c8340: blx 0x0007207c
  000c8344: movs r0,#0xc0
  000c8346: blx 0x0006eb24
  000c834a: mov r10,r0
  000c834c: ldr r2,[r4,#0x0]
  000c834e: movw r1,#0x4990
  000c8352: movs r3,#0x0
  000c8354: blx 0x0007207c
  000c8358: movs r0,#0xc0
  000c835a: blx 0x0006eb24
  000c835e: mov r6,r0
  000c8360: ldr r2,[r4,#0x0]
  000c8362: movw r1,#0x4991
  000c8366: movs r3,#0x0
  000c8368: mov.w r11,#0x0
  000c836c: blx 0x0007207c
  000c8370: ldr r0,[0x000c843c]
  000c8372: ldr.w r1,[r9,#0xc]
  000c8376: add r0,pc
  000c8378: ldr r4,[r0,#0x0]
  000c837a: mov r0,r8
  000c837c: blx r4
  000c837e: ldr.w r1,[r10,#0xc]
  000c8382: mov r0,r8
  000c8384: blx r4
  000c8386: ldr r1,[r6,#0xc]
  000c8388: mov r0,r8
  000c838a: blx r4
  000c838c: ldr r0,[0x000c8440]
  000c838e: add r0,pc
  000c8390: ldr r4,[r0,#0x0]
  000c8392: mov r0,r9
  000c8394: blx r4
  000c8396: ldr r0,[0x000c8444]
  000c8398: add r0,pc
  000c839a: ldr r5,[r0,#0x0]
  000c839c: mov r0,r9
  000c839e: blx r5
  000c83a0: mov r0,r10
  000c83a2: blx r4
  000c83a4: blx r5
  000c83a6: mov r0,r6
  000c83a8: blx r4
  000c83aa: blx r5
  000c83ac: ldr r4,[sp,#0x48]
  000c83ae: mov.w r2,#0xffffffff
  000c83b2: movs r3,#0x0
  000c83b4: ldr.w r0,[r4,#0xf8]
  000c83b8: ldr r0,[r0,#0x4]
  000c83ba: ldr r0,[r0,#0x8]
  000c83bc: ldr r1,[r0,#0x0]
  000c83be: ldr r6,[r1,#0x8]
  000c83c0: mov r1,r8
  000c83c2: blx r6
  000c83c4: ldr.w r0,[r4,#0xf8]
  000c83c8: movs r1,#0x0
  000c83ca: ldr r0,[r0,#0x4]
  000c83cc: ldr r0,[r0,#0x8]
  000c83ce: blx 0x000730b4
  000c83d2: ldr.w r0,[r4,#0xf8]
  000c83d6: add.w r8,sp,#0x118
  000c83da: ldr r1,[0x000c842c]
  000c83dc: ldr r0,[r0,#0x4]
  000c83de: ldr r0,[r0,#0x8]
  000c83e0: str r1,[sp,#0x11c]
  000c83e2: ldr r1,[0x000c8430]
  000c83e4: str r1,[sp,#0x118]
  000c83e6: ldr r1,[0x000c8434]
  000c83e8: str r1,[sp,#0x120]
  000c83ea: ldr r1,[r0,#0x0]
  000c83ec: ldr r2,[r1,#0x44]
  000c83ee: mov r1,r8
  000c83f0: blx r2
  000c83f2: ldr.w r0,[r4,#0xf8]
  000c83f6: movs r1,#0x0
  000c83f8: ldr r2,[0x000c8438]
  000c83fa: movs r3,#0x0
  000c83fc: ldr r0,[r0,#0x4]
  000c83fe: ldr r0,[r0,#0x8]
  000c8400: ldr r0,[r0,#0x8]
  000c8402: blx 0x00073054
  000c8406: ldr r6,[sp,#0x48]
  000c8408: add.w r10,r4,#0x18c
  000c840c: vldr.32 s16,[pc,#0x38]
  000c8410: mov.w r9,#0x0
  000c8414: vldr.32 s18,[pc,#0x34]
  000c8418: b 0x000c8512
  000c8450: cmp.w r9,#0x1
  000c8454: mov.w r2,#0x0
  000c8458: it hi
  000c845a: mov.hi r2,#0x1
  000c845c: cmp.w r9,#0x7
  000c8460: mov.w r1,#0x0
  000c8464: mov.w r0,#0x110
  000c8468: it cc
  000c846a: mov.cc r1,#0x1
  000c846c: cmp.w r9,#0x4
  000c8470: it cc
  000c8472: mov.cc.w r0,#0x10c
  000c8476: ldr r0,[r6,r0]
  000c8478: it cc
  000c847a: mov.cc r1,r2
  000c847c: blx 0x00073ec4
  000c8480: mov r4,r0
  000c8482: ldr r0,[0x000c8708]
  000c8484: movs r1,#0x3
  000c8486: add r0,pc
  000c8488: ldr r0,[r0,#0x0]
  000c848a: ldr r0,[r0,#0x0]
  000c848c: blx 0x00071a34
  000c8490: mov r3,r0
  000c8492: movs r0,#0x1
  000c8494: str r4,[sp,#0x0]
  000c8496: movs r1,#0x3
  000c8498: strd r0,r11,[sp,#0x4]
  000c849c: mov r0,r6
  000c849e: movs r2,#0x0
  000c84a0: blx 0x00073e88
  000c84a4: mov r5,r0
  000c84a6: ldr.w r0,[r6,#0x10c]
  000c84aa: blx 0x00073f3c
  000c84ae: mov r1,r0
  000c84b0: mov r0,r5
  000c84b2: blx 0x00073f60
  000c84b6: ldr r0,[r5,#0x0]
  000c84b8: mov r1,r5
  000c84ba: ldr r2,[r0,#0x28]
  000c84bc: mov r0,r8
  000c84be: blx r2
  000c84c0: mov r0,r10
  000c84c2: mov r1,r8
  000c84c4: blx 0x0006eb3c
  000c84c8: vldr.32 s0,[r6,#0x190]
  000c84cc: vcmpe.f32 s0,s16
  000c84d0: vmrs apsr,fpscr
  000c84d4: ble 0x000c84fc
  000c84d6: vcmpe.f32 s0,s18
  000c84da: vmrs apsr,fpscr
  000c84de: bpl 0x000c84fc
  000c84e0: vadd.f32 s0,s0,s0
  000c84e4: ldr r1,[sp,#0x48]
  000c84e6: ldr r0,[r5,#0x0]
  000c84e8: mov r3,r1
  000c84ea: ldr.w r1,[r1,#0x18c]
  000c84ee: ldr r4,[r0,#0x48]
  000c84f0: mov r0,r5
  000c84f2: ldr.w r3,[r3,#0x194]
  000c84f6: vmov r2,s0
  000c84fa: blx r4
  000c84fc: ldr r0,[sp,#0x48]
  000c84fe: strb.w r11,[r5,#0x13e]
  000c8502: ldr.w r0,[r0,#0xf8]
  000c8506: ldr r0,[r0,#0x4]
  000c8508: add.w r0,r0,r9, lsl #0x2
  000c850c: add.w r9,r9,#0x1
  000c8510: str r5,[r0,#0xc]
  000c8512: cmp.w r9,#0x8
  000c8516: bne 0x000c8450
  000c8518: movs r0,#0x0
  000c851a: movs r1,#0x1
  000c851c: strd r0,r1,[sp,#0x0]
  000c8520: movs r1,#0x3
  000c8522: ldr r4,[sp,#0x48]
  000c8524: movs r2,#0x1
  000c8526: str r0,[sp,#0x8]
  000c8528: movs r3,#0xf
  000c852a: mov r0,r4
  000c852c: blx 0x00073e88
  000c8530: ldr.w r1,[r4,#0xf8]
  000c8534: ldrd r2,r1,[r1,#0x0]
  000c8538: add.w r1,r1,r2, lsl #0x2
  000c853c: ldr r2,[0x000c870c]
  000c853e: str.w r0,[r1,#-0x4]
  000c8542: ldr.w r0,[r4,#0xf8]
  000c8546: ldrd r1,r0,[r0,#0x0]
  000c854a: add.w r0,r0,r1, lsl #0x2
  000c854e: ldr r1,[0x000c8710]
  000c8550: ldr.w r0,[r0,#-0x4]
  000c8554: ldr r3,[r0,#0x0]
  000c8556: ldr r6,[r3,#0x48]
  000c8558: ldr r3,[0x000c8714]
  000c855a: blx r6
  000c855c: b.w 0x000cb9bc
  000c8560: movs r0,#0xc
  000c8562: blx 0x0006eb24
  000c8566: mov r5,r0
  000c8568: blx 0x00073d08
  000c856c: ldr r0,[sp,#0x48]
  000c856e: mov r1,r5
  000c8570: str.w r5,[r0,#0xf8]
  000c8574: movs r0,#0x1
  000c8576: blx 0x00073d14
  000c857a: mov.w r0,#0x138
  000c857e: blx 0x0006eb24
  000c8582: ldr r1,[0x000c8718]
  000c8584: mov r5,r0
  000c8586: mov.w r8,#0x0
  000c858a: movs r2,#0x0
  000c858c: movw r3,#0xea60
  000c8590: str.w r8,[sp,#0x0]
  000c8594: blx 0x00073e4c
  000c8598: ldr r6,[sp,#0x48]
  000c859a: mov r1,r5
  000c859c: movw r2,#0x494e
  000c85a0: movs r3,#0x0
  000c85a2: mov r0,r6
  000c85a4: blx 0x00073f6c
  000c85a8: ldr.w r1,[r6,#0xf8]
  000c85ac: ldr r1,[r1,#0x4]
  000c85ae: str r0,[r1,#0x0]
  000c85b0: movs r1,#0x0
  000c85b2: ldr.w r0,[r6,#0xf8]
  000c85b6: ldr r0,[r0,#0x4]
  000c85b8: ldr r0,[r0,#0x0]
  000c85ba: blx 0x00073f54
  000c85be: ldr.w r0,[r6,#0xf8]
  000c85c2: movs r1,#0x0
  000c85c4: ldr r0,[r0,#0x4]
  000c85c6: ldr r0,[r0,#0x0]
  000c85c8: blx 0x00072748
  000c85cc: ldr.w r0,[r6,#0xf8]
  000c85d0: add.w r9,sp,#0x180
  000c85d4: mov r1,r5
  000c85d6: ldr r0,[r0,#0x4]
  000c85d8: ldr r4,[r0,#0x0]
  000c85da: ldr r0,[r5,#0x0]
  000c85dc: ldr r2,[r0,#0x28]
  000c85de: mov r0,r9
  000c85e0: blx r2
  000c85e2: ldr r0,[r4,#0x0]
  000c85e4: mov r1,r9
  000c85e6: ldr r2,[r0,#0x44]
  000c85e8: mov r0,r4
  000c85ea: blx r2
  000c85ec: ldr.w r0,[r6,#0xf8]
  000c85f0: movs r1,#0x1
  000c85f2: ldr r0,[r0,#0x4]
  000c85f4: ldr r0,[r0,#0x0]
  000c85f6: ldr r0,[r0,#0x4]
  000c85f8: blx 0x00073fb4
  000c85fc: ldr.w r0,[r6,#0xf8]
  000c8600: ldr r0,[r0,#0x4]
  000c8602: ldr r0,[r0,#0x0]
  000c8604: ldr r4,[r0,#0x4]
  000c8606: mov r0,r4
  000c8608: blx 0x000724f0
  000c860c: movs r1,#0x14
  000c860e: blx 0x0007198c
  000c8612: mov r1,r0
  000c8614: mov r0,r4
  000c8616: blx 0x000727e4
  000c861a: ldr.w r0,[r6,#0xf8]
  000c861e: ldr r1,[0x000c871c]
  000c8620: ldr r0,[r0,#0x4]
  000c8622: ldr r0,[r0,#0x0]
  000c8624: ldr r0,[r0,#0x8]
  000c8626: strd r8,r1,[sp,#0x180]
  000c862a: add r1,sp,#0x180
  000c862c: str.w r8,[sp,#0x188]
  000c8630: blx 0x00073dd4
  000c8634: ldr.w r0,[r6,#0xf8]
  000c8638: movs r1,#0x0
  000c863a: ldr r0,[r0,#0x4]
  000c863c: ldr r0,[r0,#0x0]
  000c863e: blx 0x000732f4
  000c8642: ldr.w r0,[r6,#0xf8]
  000c8646: ldr r0,[r0,#0x4]
  000c8648: ldr r2,[r0,#0x0]
  000c864a: ldr.w r0,[r2,#0x50]!
  000c864e: cbz r0,0x000c8666
  000c8650: blx 0x00070204
  000c8654: blx 0x0006eb48
  000c8658: ldr r0,[sp,#0x48]
  000c865a: ldr.w r0,[r0,#0xf8]
  000c865e: ldr r0,[r0,#0x4]
  000c8660: ldr r0,[r0,#0x0]
  000c8662: add.w r2,r0,#0x50
  000c8666: movs r0,#0x0
  000c8668: str r0,[r2,#0x0]
  000c866a: movs r0,#0x1c
  000c866c: blx 0x0006eb24
  000c8670: mov r5,r0
  000c8672: ldr r3,[sp,#0x48]
  000c8674: movs r1,#0x1
  000c8676: movs r2,#0x0
  000c8678: blx 0x00073edc
  000c867c: b.w 0x000c9412
  000c8680: ldr r0,[0x000c8720]
  000c8682: ldr r2,[0x000c8724]
  000c8684: add r0,pc
  000c8686: ldr r1,[0x000c8728]
  000c8688: add r2,pc
  000c868a: ldr.w r11,[r0,#0x0]
  000c868e: ldr r6,[r2,#0x0]
  000c8690: mov r8,r1
  000c8692: ldr.w r0,[r11,#0x0]
  000c8696: blx r6
  000c8698: ldr r4,[0x000c872c]
  000c869a: movs r5,#0x0
  000c869c: mov r1,r8
  000c869e: add r0,r4
  000c86a0: str r0,[sp,#0x180]
  000c86a2: ldr.w r0,[r11,#0x0]
  000c86a6: strd r5,r4,[sp,#0x184]
  000c86aa: blx r6
  000c86ac: ldr.w r1,[r11,#0x0]
  000c86b0: add r0,r4
  000c86b2: strd r0,r5,[sp,#0x18c]
  000c86b6: ldr r2,[0x000c8730]
  000c86b8: mov r0,r1
  000c86ba: mov r1,r8
  000c86bc: str r2,[sp,#0x194]
  000c86be: blx r6
  000c86c0: add r0,r4
  000c86c2: strd r0,r5,[sp,#0x198]
  000c86c6: ldr r1,[0x000c8734]
  000c86c8: movs r0,#0x18
  000c86ca: str r1,[sp,#0x1a0]
  000c86cc: blx 0x0006eb24
  000c86d0: mov r5,r0
  000c86d2: add r1,sp,#0x180
  000c86d4: movs r2,#0x9
  000c86d6: blx 0x00073eb8
  000c86da: ldr r0,[sp,#0x48]
  000c86dc: str.w r5,[r0,#0x110]
  000c86e0: movs r0,#0xc
  000c86e2: blx 0x0006eb24
  000c86e6: mov r5,r0
  000c86e8: blx 0x00073d08
  000c86ec: ldr r0,[sp,#0x48]
  000c86ee: mov r1,r5
  000c86f0: str.w r5,[r0,#0xf8]
  000c86f4: movs r0,#0x4
  000c86f6: blx 0x00073d14
  000c86fa: ldr r5,[sp,#0x48]
  000c86fc: mov.w r8,#0x0
  000c8700: mov.w r9,#0x1
  000c8704: movs r6,#0x0
  000c8706: b 0x000c87b6
  000c8738: ldr.w r4,[r5,#0x110]
  000c873c: ldr.w r10,[r11,#0x0]
  000c8740: mov r0,r4
  000c8742: blx 0x00073fcc
  000c8746: mov r1,r0
  000c8748: mov r0,r10
  000c874a: blx 0x00071848
  000c874e: mov r1,r0
  000c8750: mov r0,r4
  000c8752: blx 0x00073ec4
  000c8756: strd r0,r9,[sp,#0x0]
  000c875a: mov r0,r5
  000c875c: movs r1,#0xa
  000c875e: movs r2,#0x0
  000c8760: movs r3,#0x2c
  000c8762: str.w r8,[sp,#0x8]
  000c8766: blx 0x00073e88
  000c876a: ldr.w r1,[r5,#0xf8]
  000c876e: ldr r1,[r1,#0x4]
  000c8770: str.w r0,[r1,r6,lsl #0x2]
  000c8774: ldr.w r0,[r5,#0xf8]
  000c8778: ldr r0,[r0,#0x4]
  000c877a: ldr.w r0,[r0,r6,lsl #0x2]
  000c877e: blx 0x000732dc
  000c8782: ldr.w r0,[r5,#0xf8]
  000c8786: movs r1,#0x1
  000c8788: ldr r0,[r0,#0x4]
  000c878a: ldr.w r0,[r0,r6,lsl #0x2]
  000c878e: ldr r0,[r0,#0x4]
  000c8790: blx 0x00073e94
  000c8794: ldr.w r0,[r5,#0xf8]
  000c8798: movs r1,#0x1
  000c879a: ldr r0,[r0,#0x4]
  000c879c: ldr.w r0,[r0,r6,lsl #0x2]
  000c87a0: blx 0x00074158
  000c87a4: ldr.w r0,[r5,#0xf8]
  000c87a8: movs r1,#0x0
  000c87aa: ldr r0,[r0,#0x4]
  000c87ac: ldr.w r0,[r0,r6,lsl #0x2]
  000c87b0: blx 0x00074164
  000c87b4: adds r6,#0x1
  000c87b6: cmp r6,#0x3
  000c87b8: bne 0x000c8738
  000c87ba: mov.w r0,#0x138
  000c87be: blx 0x0006eb24
  000c87c2: ldr r1,[0x000c8b94]
  000c87c4: mov r5,r0
  000c87c6: ldr r3,[0x000c8b98]
  000c87c8: movs r0,#0x0
  000c87ca: movs r2,#0x0
  000c87cc: str r0,[sp,#0x0]
  000c87ce: mov r0,r5
  000c87d0: blx 0x00073e4c
  000c87d4: ldr r4,[sp,#0x48]
  000c87d6: mov r1,r5
  000c87d8: movw r2,#0x4299
  000c87dc: movs r3,#0x0
  000c87de: mov r0,r4
  000c87e0: blx 0x00073f6c
  000c87e4: ldr.w r1,[r4,#0xf8]
  000c87e8: ldr r1,[r1,#0x4]
  000c87ea: str r0,[r1,#0xc]
  000c87ec: movs r1,#0x0
  000c87ee: ldr.w r0,[r4,#0xf8]
  000c87f2: ldr r0,[r0,#0x4]
  000c87f4: ldr r0,[r0,#0xc]
  000c87f6: blx 0x00073f54
  000c87fa: ldr.w r0,[r4,#0xf8]
  000c87fe: movs r1,#0x1
  000c8800: ldr r0,[r0,#0x4]
  000c8802: ldr r0,[r0,#0xc]
  000c8804: ldr r0,[r0,#0x4]
  000c8806: blx 0x00073fb4
  000c880a: ldr.w r0,[r4,#0xf8]
  000c880e: movs r1,#0x1
  000c8810: ldr r0,[r0,#0x4]
  000c8812: ldr r0,[r0,#0xc]
  000c8814: blx 0x00072748
  000c8818: ldr r0,[0x000c8b9c]
  000c881a: ldr.w r1,[r4,#0xf8]
  000c881e: add r0,pc
  000c8820: ldr r0,[r0,#0x0]
  000c8822: ldr r1,[r1,#0x4]
  000c8824: ldr r0,[r0,#0x0]
  000c8826: ldr r4,[r1,#0xc]
  000c8828: movw r1,#0xc89
  000c882c: blx 0x00072f70
  000c8830: add r5,sp,#0xc4
  000c8832: mov r1,r0
  000c8834: movs r2,#0x0
  000c8836: mov r0,r5
  000c8838: blx 0x0006f028
  000c883c: mov r0,r4
  000c883e: mov r1,r5
  000c8840: blx 0x00073fa8
  000c8844: add r0,sp,#0xc4
  000c8846: blx 0x0006ee30
  000c884a: ldr r0,[sp,#0x48]
  000c884c: ldr.w r0,[r0,#0xf8]
  000c8850: ldr r0,[r0,#0x4]
  000c8852: ldr r2,[r0,#0xc]
  000c8854: ldr.w r0,[r2,#0x50]!
  000c8858: cbz r0,0x000c8870
  000c885a: blx 0x00070204
  000c885e: blx 0x0006eb48
  000c8862: ldr r0,[sp,#0x48]
  000c8864: ldr.w r0,[r0,#0xf8]
  000c8868: ldr r0,[r0,#0x4]
  000c886a: ldr r0,[r0,#0xc]
  000c886c: add.w r2,r0,#0x50
  000c8870: movs r0,#0x0
  000c8872: str r0,[r2,#0x0]
  000c8874: movs r0,#0x1c
  000c8876: blx 0x0006eb24
  000c887a: mov r5,r0
  000c887c: ldr r3,[sp,#0x48]
  000c887e: movs r1,#0x16
  000c8880: movs r2,#0x0
  000c8882: blx 0x00073edc
  000c8886: ldr r4,[sp,#0x48]
  000c8888: movs r0,#0x1c
  000c888a: str r5,[r4,#0x28]
  000c888c: blx 0x0006eb24
  000c8890: mov r5,r0
  000c8892: movs r1,#0x1
  000c8894: movs r2,#0x3
  000c8896: mov r3,r4
  000c8898: blx 0x00073edc
  000c889c: b.w 0x000c9412
  000c88a0: ldr r0,[0x000c8ba0]
  000c88a2: add r0,pc
  000c88a4: vldr.64 d16,[r0]
  000c88a8: ldr r0,[r0,#0x8]
  000c88aa: str r0,[sp,#0x148]
  000c88ac: movs r0,#0x18
  000c88ae: vstr.64 d16,[sp,#0x140]
  000c88b2: blx 0x0006eb24
  000c88b6: mov r5,r0
  000c88b8: add r1,sp,#0x140
  000c88ba: movs r2,#0x3
  000c88bc: blx 0x00073eb8
  000c88c0: ldr r0,[sp,#0x48]
  000c88c2: str.w r5,[r0,#0x110]
  000c88c6: movs r0,#0xc
  000c88c8: blx 0x0006eb24
  000c88cc: mov r5,r0
  000c88ce: blx 0x00073d08
  000c88d2: ldr r0,[sp,#0x48]
  000c88d4: mov r1,r5
  000c88d6: str.w r5,[r0,#0xf8]
  000c88da: movs r0,#0xa
  000c88dc: blx 0x00073d14
  000c88e0: ldr r0,[0x000c8ba4]
  000c88e2: mov.w r8,#0x0
  000c88e6: ldr r5,[sp,#0x48]
  000c88e8: mov.w r9,#0x1
  000c88ec: add r0,pc
  000c88ee: movs r6,#0x0
  000c88f0: ldr.w r11,[r0,#0x0]
  000c88f4: b 0x000c8954
  000c88f6: ldr.w r4,[r5,#0x110]
  000c88fa: ldr.w r10,[r11,#0x0]
  000c88fe: mov r0,r4
  000c8900: blx 0x00073fcc
  000c8904: mov r1,r0
  000c8906: mov r0,r10
  000c8908: blx 0x00071848
  000c890c: mov r1,r0
  000c890e: mov r0,r4
  000c8910: blx 0x00073ec4
  000c8914: strd r0,r9,[sp,#0x0]
  000c8918: mov r0,r5
  000c891a: movs r1,#0xa
  000c891c: movs r2,#0x0
  000c891e: movs r3,#0x2c
  000c8920: str.w r8,[sp,#0x8]
  000c8924: blx 0x00073e88
  000c8928: ldr.w r1,[r5,#0xf8]
  000c892c: ldr r1,[r1,#0x4]
  000c892e: str.w r0,[r1,r6,lsl #0x2]
  000c8932: ldr.w r0,[r5,#0xf8]
  000c8936: ldr r0,[r0,#0x4]
  000c8938: ldr.w r0,[r0,r6,lsl #0x2]
  000c893c: blx 0x000732dc
  000c8940: ldr.w r0,[r5,#0xf8]
  000c8944: movs r1,#0x1
  000c8946: ldr r0,[r0,#0x4]
  000c8948: ldr.w r0,[r0,r6,lsl #0x2]
  000c894c: ldr r0,[r0,#0x4]
  000c894e: blx 0x00073e94
  000c8952: adds r6,#0x1
  000c8954: cmp r6,#0x6
  000c8956: bne 0x000c88f6
  000c8958: ldr r4,[sp,#0x48]
  000c895a: movs r1,#0x0
  000c895c: movw r2,#0x495d
  000c8960: movs r3,#0x0
  000c8962: mov.w r8,#0x0
  000c8966: mov r0,r4
  000c8968: blx 0x00073f6c
  000c896c: ldr.w r1,[r4,#0xf8]
  000c8970: ldr r1,[r1,#0x4]
  000c8972: str r0,[r1,#0x18]
  000c8974: movs r1,#0x0
  000c8976: ldr.w r0,[r4,#0xf8]
  000c897a: ldr r0,[r0,#0x4]
  000c897c: ldr r0,[r0,#0x18]
  000c897e: blx 0x00073f54
  000c8982: ldr.w r0,[r4,#0xf8]
  000c8986: movs r1,#0x2
  000c8988: ldr r0,[r0,#0x4]
  000c898a: ldr r0,[r0,#0x18]
  000c898c: blx 0x00072748
  000c8990: ldr r0,[0x000c8ba8]
  000c8992: ldr.w r1,[r4,#0xf8]
  000c8996: add r0,pc
  000c8998: ldr r6,[r0,#0x0]
  000c899a: ldr r1,[r1,#0x4]
  000c899c: ldr r0,[r6,#0x0]
  000c899e: ldr r4,[r1,#0x18]
  000c89a0: movw r1,#0xc88
  000c89a4: blx 0x00072f70
  000c89a8: add r5,sp,#0xb8
  000c89aa: mov r1,r0
  000c89ac: movs r2,#0x0
  000c89ae: mov r0,r5
  000c89b0: blx 0x0006f028
  000c89b4: mov r0,r4
  000c89b6: mov r1,r5
  000c89b8: blx 0x00073fa8
  000c89bc: add r0,sp,#0xb8
  000c89be: blx 0x0006ee30
  000c89c2: ldr r0,[sp,#0x48]
  000c89c4: movs r1,#0x1
  000c89c6: mov r4,r0
  000c89c8: ldr.w r0,[r0,#0xf8]
  000c89cc: ldr r0,[r0,#0x4]
  000c89ce: ldr r0,[r0,#0x18]
  000c89d0: ldr r0,[r0,#0x4]
  000c89d2: blx 0x00073fb4
  000c89d6: ldr.w r0,[r4,#0xf8]
  000c89da: ldr r1,[0x000c8bac]
  000c89dc: ldr r0,[r0,#0x4]
  000c89de: ldr r0,[r0,#0x18]
  000c89e0: ldr r0,[r0,#0x8]
  000c89e2: strd r8,r1,[sp,#0x180]
  000c89e6: add r1,sp,#0x180
  000c89e8: str.w r8,[sp,#0x188]
  000c89ec: blx 0x00072478
  000c89f0: ldr.w r0,[r4,#0xf8]
  000c89f4: ldr r0,[r0,#0x4]
  000c89f6: ldr r2,[r0,#0x18]
  000c89f8: ldr.w r0,[r2,#0x50]!
  000c89fc: cbz r0,0x000c8a14
  000c89fe: blx 0x00070204
  000c8a02: blx 0x0006eb48
  000c8a06: ldr r0,[sp,#0x48]
  000c8a08: ldr.w r0,[r0,#0xf8]
  000c8a0c: ldr r0,[r0,#0x4]
  000c8a0e: ldr r0,[r0,#0x18]
  000c8a10: add.w r2,r0,#0x50
  000c8a14: movs r4,#0x0
  000c8a16: mov.w r0,#0x138
  000c8a1a: str r4,[r2,#0x0]
  000c8a1c: blx 0x0006eb24
  000c8a20: mov r5,r0
  000c8a22: ldr r2,[0x000c8bb0]
  000c8a24: movw r1,#0x7530
  000c8a28: movw r3,#0x9c40
  000c8a2c: str r4,[sp,#0x0]
  000c8a2e: blx 0x00073e4c
  000c8a32: ldr r4,[sp,#0x48]
  000c8a34: mov r1,r5
  000c8a36: movw r2,#0x4299
  000c8a3a: movs r3,#0x0
  000c8a3c: mov r0,r4
  000c8a3e: blx 0x00073f6c
  000c8a42: ldr.w r1,[r4,#0xf8]
  000c8a46: ldr r1,[r1,#0x4]
  000c8a48: str r0,[r1,#0x1c]
  000c8a4a: movs r1,#0x0
  000c8a4c: ldr.w r0,[r4,#0xf8]
  000c8a50: ldr r0,[r0,#0x4]
  000c8a52: ldr r0,[r0,#0x1c]
  000c8a54: blx 0x00073f54
  000c8a58: ldr.w r0,[r4,#0xf8]
  000c8a5c: movs r1,#0x1
  000c8a5e: ldr r0,[r0,#0x4]
  000c8a60: ldr r0,[r0,#0x1c]
  000c8a62: blx 0x00072748
  000c8a66: ldr.w r0,[r4,#0xf8]
  000c8a6a: ldr r1,[r0,#0x4]
  000c8a6c: ldr r0,[r6,#0x0]
  000c8a6e: ldr r4,[r1,#0x1c]
  000c8a70: movw r1,#0xc89
  000c8a74: blx 0x00072f70
  000c8a78: add r5,sp,#0xac
  000c8a7a: mov r1,r0
  000c8a7c: movs r2,#0x0
  000c8a7e: mov r0,r5
  000c8a80: blx 0x0006f028
  000c8a84: mov r0,r4
  000c8a86: mov r1,r5
  000c8a88: blx 0x00073fa8
  000c8a8c: add r0,sp,#0xac
  000c8a8e: blx 0x0006ee30
  000c8a92: ldr r0,[sp,#0x48]
  000c8a94: movs r1,#0x1
  000c8a96: mov r4,r0
  000c8a98: ldr.w r0,[r0,#0xf8]
  000c8a9c: ldr r0,[r0,#0x4]
  000c8a9e: ldr r0,[r0,#0x1c]
  000c8aa0: ldr r0,[r0,#0x4]
  000c8aa2: blx 0x00073fb4
  000c8aa6: ldr.w r0,[r4,#0xf8]
  000c8aaa: ldr r0,[r0,#0x4]
  000c8aac: ldr r2,[r0,#0x1c]
  000c8aae: ldr.w r0,[r2,#0x50]!
  000c8ab2: cbz r0,0x000c8aca
  000c8ab4: blx 0x00070204
  000c8ab8: blx 0x0006eb48
  000c8abc: ldr r0,[sp,#0x48]
  000c8abe: ldr.w r0,[r0,#0xf8]
  000c8ac2: ldr r0,[r0,#0x4]
  000c8ac4: ldr r0,[r0,#0x1c]
  000c8ac6: add.w r2,r0,#0x50
  000c8aca: ldr r1,[0x000c8bb4]
  000c8acc: movs r0,#0x0
  000c8ace: str r0,[r2,#0x0]
  000c8ad0: add r0,sp,#0x180
  000c8ad2: add r1,pc
  000c8ad4: movs r2,#0x24
  000c8ad6: blx 0x0006f1e4
  000c8ada: ldr r0,[0x000c8bb8]
  000c8adc: add r0,pc
  000c8ade: vldr.64 d16,[r0]
  000c8ae2: ldr r0,[r0,#0x8]
  000c8ae4: str r0,[sp,#0x120]
  000c8ae6: movs r0,#0xc
  000c8ae8: vstr.64 d16,[sp,#0x118]
  000c8aec: blx 0x0006eb24
  000c8af0: mov r6,r0
  000c8af2: blx 0x00073d08
  000c8af6: movs r0,#0x3
  000c8af8: mov r1,r6
  000c8afa: blx 0x00073d14
  000c8afe: ldr r0,[sp,#0x48]
  000c8b00: ldr r1,[r6,#0x4]
  000c8b02: mov r2,r0
  000c8b04: ldr.w r0,[r0,#0xf8]
  000c8b08: ldr r0,[r0,#0x4]
  000c8b0a: ldr r0,[r0,#0x18]
  000c8b0c: str r0,[r1,#0x0]
  000c8b0e: ldr.w r0,[r2,#0xf8]
  000c8b12: ldr r1,[r6,#0x4]
  000c8b14: ldr r0,[r0,#0x4]
  000c8b16: ldr r0,[r0,#0x1c]
  000c8b18: str r0,[r1,#0x8]
  000c8b1a: movs r0,#0x18
  000c8b1c: blx 0x0006eb24
  000c8b20: mov r8,r0
  000c8b22: movs r0,#0x9
  000c8b24: add r1,sp,#0x180
  000c8b26: add r3,sp,#0x118
  000c8b28: str r0,[sp,#0x0]
  000c8b2a: mov r0,r8
  000c8b2c: mov r2,r6
  000c8b2e: blx 0x00073fc0
  000c8b32: mov r0,r8
  000c8b34: movs r1,#0x1
  000c8b36: blx 0x00073f48
  000c8b3a: mov r0,r8
  000c8b3c: blx 0x00073f3c
  000c8b40: mov r11,r0
  000c8b42: ldr r0,[0x000c8bbc]
  000c8b44: movs r4,#0x20
  000c8b46: mov.w r9,#0x0
  000c8b4a: add r0,pc
  000c8b4c: ldr.w r10,[r0,#0x0]
  000c8b50: b 0x000c8c1a
  000c8b52: ldr.w r0,[r10,#0x0]
  000c8b56: movs r1,#0x3
  000c8b58: blx 0x00071a34
  000c8b5c: mov r5,r0
  000c8b5e: cmp r4,#0x20
  000c8b60: bne 0x000c8bc0
  000c8b62: mov r0,r8
  000c8b64: movs r1,#0x0
  000c8b66: blx 0x00073ec4
  000c8b6a: str r0,[sp,#0x0]
  000c8b6c: movs r0,#0x1
  000c8b6e: strd r0,r9,[sp,#0x4]
  000c8b72: movs r1,#0x3
  000c8b74: ldr r6,[sp,#0x48]
  000c8b76: movs r2,#0x0
  000c8b78: mov r3,r5
  000c8b7a: mov r0,r6
  000c8b7c: blx 0x00073e88
  000c8b80: ldr.w r1,[r6,#0xf8]
  000c8b84: ldr r1,[r1,#0x4]
  000c8b86: str r0,[r1,#0x20]
  000c8b88: mov r1,r8
  000c8b8a: ldr.w r0,[r6,#0xf8]
  000c8b8e: ldr r0,[r0,#0x4]
  000c8b90: ldr r0,[r0,#0x20]
  000c8b92: b 0x000c8bf0
  000c8bc0: mov r0,r11
  000c8bc2: movs r1,#0x0
  000c8bc4: blx 0x00073ec4
  000c8bc8: str r0,[sp,#0x0]
  000c8bca: movs r0,#0x1
  000c8bcc: strd r0,r9,[sp,#0x4]
  000c8bd0: movs r1,#0x3
  000c8bd2: ldr r6,[sp,#0x48]
  000c8bd4: movs r2,#0x0
  000c8bd6: mov r3,r5
  000c8bd8: mov r0,r6
  000c8bda: blx 0x00073e88
  000c8bde: ldr.w r1,[r6,#0xf8]
  000c8be2: ldr r1,[r1,#0x4]
  000c8be4: str r0,[r1,r4]
  000c8be6: mov r1,r11
  000c8be8: ldr.w r0,[r6,#0xf8]
  000c8bec: ldr r0,[r0,#0x4]
  000c8bee: ldr r0,[r0,r4]
  000c8bf0: blx 0x00073f60
  000c8bf4: ldr r0,[sp,#0x48]
  000c8bf6: movs r1,#0x1
  000c8bf8: mov r5,r0
  000c8bfa: ldr.w r0,[r0,#0xf8]
  000c8bfe: ldr r0,[r0,#0x4]
  000c8c00: ldr r0,[r0,r4]
  000c8c02: ldr r0,[r0,#0x4]
  000c8c04: blx 0x00073fb4
  000c8c08: ldr.w r0,[r5,#0xf8]
  000c8c0c: movs r1,#0x1
  000c8c0e: ldr r0,[r0,#0x4]
  000c8c10: ldr r0,[r0,r4]
  000c8c12: ldr r0,[r0,#0x4]
  000c8c14: blx 0x00074020
  000c8c18: adds r4,#0x4
  000c8c1a: cmp r4,#0x28
  000c8c1c: bne 0x000c8b52
  000c8c1e: movs r0,#0x1c
  000c8c20: blx 0x0006eb24
  000c8c24: mov r5,r0
  000c8c26: ldr r3,[sp,#0x48]
  000c8c28: movs r1,#0x1
  000c8c2a: movs r2,#0x7
  000c8c2c: blx 0x00073edc
  000c8c30: b 0x000c9412
  000c8c32: ldr r0,[0x000c8f48]
  000c8c34: add r0,pc
  000c8c36: vldr.64 d16,[r0]
  000c8c3a: ldr r0,[r0,#0x8]
  000c8c3c: str r0,[sp,#0x188]
  000c8c3e: movs r0,#0x18
  000c8c40: vstr.64 d16,[sp,#0x180]
  000c8c44: blx 0x0006eb24
  000c8c48: mov r5,r0
  000c8c4a: add r1,sp,#0x180
  000c8c4c: movs r2,#0x3
  000c8c4e: blx 0x00073eb8
  000c8c52: ldr r0,[0x000c8f4c]
  000c8c54: mov.w r9,#0x8
  000c8c58: ldr r1,[sp,#0x48]
  000c8c5a: add r0,pc
  000c8c5c: ldr r0,[r0,#0x0]
  000c8c5e: str.w r5,[r1,#0x110]
  000c8c62: ldrb r4,[r0,#0x0]
  000c8c64: movs r0,#0xc
  000c8c66: cmp r4,#0x0
  000c8c68: it eq
  000c8c6a: mov.eq.w r9,#0xc
  000c8c6e: blx 0x0006eb24
  000c8c72: mov r8,r0
  000c8c74: blx 0x00073d08
  000c8c78: ldr r0,[sp,#0x48]
  000c8c7a: cmp r4,#0x0
  000c8c7c: str r4,[sp,#0x3c]
  000c8c7e: mov r1,r8
  000c8c80: str.w r8,[r0,#0xf8]
  000c8c84: mov.w r0,#0xb
  000c8c88: it eq
  000c8c8a: mov.eq r0,#0x13
  000c8c8c: str r0,[sp,#0x38]
  000c8c8e: blx 0x00073d14
  000c8c92: ldr r0,[0x000c8f50]
  000c8c94: mov.w r8,#0x0
  000c8c98: ldr r6,[sp,#0x48]
  000c8c9a: mov.w r10,#0x1
  000c8c9e: add r0,pc
  000c8ca0: movs r5,#0x0
  000c8ca2: ldr.w r11,[r0,#0x0]
  000c8ca6: b 0x000c8cf0
  000c8ca8: ldr.w r0,[r11,#0x0]
  000c8cac: movs r1,#0x8
  000c8cae: blx 0x00071a34
  000c8cb2: mov r4,r0
  000c8cb4: ldr.w r0,[r6,#0x110]
  000c8cb8: movs r1,#0x0
  000c8cba: blx 0x00073ec4
  000c8cbe: strd r0,r10,[sp,#0x0]
  000c8cc2: mov r0,r6
  000c8cc4: movs r1,#0x8
  000c8cc6: movs r2,#0x0
  000c8cc8: mov r3,r4
  000c8cca: str.w r8,[sp,#0x8]
  000c8cce: blx 0x00073e88
  000c8cd2: ldr.w r1,[r6,#0xf8]
  000c8cd6: ldr r1,[r1,#0x4]
  000c8cd8: str.w r0,[r1,r5,lsl #0x2]
  000c8cdc: movs r1,#0x1
  000c8cde: ldr.w r0,[r6,#0xf8]
  000c8ce2: ldr r0,[r0,#0x4]
  000c8ce4: ldr.w r0,[r0,r5,lsl #0x2]
  000c8ce8: ldr r0,[r0,#0x4]
  000c8cea: blx 0x00073e94
  000c8cee: adds r5,#0x1
  000c8cf0: cmp r9,r5
  000c8cf2: bne 0x000c8ca8
  000c8cf4: ldr r0,[sp,#0x3c]
  000c8cf6: movs r5,#0xb
  000c8cf8: mov.w r8,#0x0
  000c8cfc: mov.w r10,#0x1
  000c8d00: cmp r0,#0x0
  000c8d02: it eq
  000c8d04: mov.eq r5,#0x10
  000c8d06: ldr r6,[sp,#0x48]
  000c8d08: str.w r9,[sp,#0x3c]
  000c8d0c: b 0x000c8d58
  000c8d0e: ldr.w r0,[r11,#0x0]
  000c8d12: movs r1,#0x2
  000c8d14: blx 0x00071a34
  000c8d18: mov r4,r0
  000c8d1a: ldr.w r0,[r6,#0x110]
  000c8d1e: movs r1,#0x0
  000c8d20: blx 0x00073ec4
  000c8d24: strd r0,r10,[sp,#0x0]
  000c8d28: mov r0,r6
  000c8d2a: movs r1,#0x2
  000c8d2c: movs r2,#0x0
  000c8d2e: mov r3,r4
  000c8d30: str.w r8,[sp,#0x8]
  000c8d34: blx 0x00073e88
  000c8d38: ldr.w r1,[r6,#0xf8]
  000c8d3c: ldr r1,[r1,#0x4]
  000c8d3e: str.w r0,[r1,r9,lsl #0x2]
  000c8d42: movs r1,#0x1
  000c8d44: ldr.w r0,[r6,#0xf8]
  000c8d48: ldr r0,[r0,#0x4]
  000c8d4a: ldr.w r0,[r0,r9,lsl #0x2]
  000c8d4e: ldr r0,[r0,#0x4]
  000c8d50: blx 0x00073fb4
  000c8d54: add.w r9,r9,#0x1
  000c8d58: cmp r9,r5
  000c8d5a: bcc 0x000c8d0e
  000c8d5c: ldrd r4,r8,[sp,#0x38]
  000c8d60: mov.w r9,#0x0
  000c8d64: mov.w r10,#0x1
  000c8d68: ldr r6,[sp,#0x48]
  000c8d6a: b 0x000c8db8
  000c8d6c: ldr.w r0,[r6,#0x110]
  000c8d70: movs r1,#0x0
  000c8d72: blx 0x00073ec4
  000c8d76: strd r0,r10,[sp,#0x0]
  000c8d7a: mov r0,r6
  000c8d7c: movs r1,#0x2
  000c8d7e: movs r2,#0x1
  000c8d80: movs r3,#0xf
  000c8d82: str.w r9,[sp,#0x8]
  000c8d86: blx 0x00073e88
  000c8d8a: ldr.w r1,[r6,#0xf8]
  000c8d8e: ldr r1,[r1,#0x4]
  000c8d90: str.w r0,[r1,r5,lsl #0x2]
  000c8d94: movs r1,#0x0
  000c8d96: ldr.w r0,[r6,#0xf8]
  000c8d9a: ldr r0,[r0,#0x4]
  000c8d9c: ldr.w r0,[r0,r5,lsl #0x2]
  000c8da0: blx 0x00073f54
  000c8da4: ldr.w r0,[r6,#0xf8]
  000c8da8: movs r1,#0x1
  000c8daa: ldr r0,[r0,#0x4]
  000c8dac: ldr.w r0,[r0,r5,lsl #0x2]
  000c8db0: ldr r0,[r0,#0x4]
  000c8db2: blx 0x00073fb4
  000c8db6: adds r5,#0x1
  000c8db8: cmp r5,r4
  000c8dba: bcc 0x000c8d6c
  000c8dbc: movs r0,#0x1c
  000c8dbe: blx 0x0006eb24
  000c8dc2: mov r6,r0
  000c8dc4: ldr r0,[sp,#0x48]
  000c8dc6: movs r1,#0x12
  000c8dc8: str r0,[sp,#0x0]
  000c8dca: mov r0,r6
  000c8dcc: movs r2,#0x0
  000c8dce: mov r3,r8
  000c8dd0: blx 0x00073f9c
  000c8dd4: ldr r0,[sp,#0x48]
  000c8dd6: str r6,[r0,#0x28]
  000c8dd8: b.w 0x000cb9bc
  000c8ddc: movs r0,#0xc
  000c8dde: blx 0x0006eb24
  000c8de2: mov r5,r0
  000c8de4: blx 0x00073d08
  000c8de8: ldr r4,[sp,#0x48]
  000c8dea: movs r0,#0x2
  000c8dec: mov r1,r5
  000c8dee: str.w r5,[r4,#0xf8]
  000c8df2: blx 0x00073d14
  000c8df6: add r5,sp,#0x180
  000c8df8: ldr.w r1,[r4,#0xf0]
  000c8dfc: mov r0,r5
  000c8dfe: blx 0x0007264c
  000c8e02: add.w r0,r4,#0x18c
  000c8e06: mov r1,r5
  000c8e08: blx 0x0006eb3c
  000c8e0c: ldr r0,[0x000c8f54]
  000c8e0e: ldr r1,[0x000c8f58]
  000c8e10: add r0,pc
  000c8e12: add r1,pc
  000c8e14: ldr r6,[r0,#0x0]
  000c8e16: ldr r5,[r1,#0x0]
  000c8e18: movw r1,#0xc350
  000c8e1c: ldr r0,[r6,#0x0]
  000c8e1e: blx r5
  000c8e20: mov r10,r0
  000c8e22: ldr r0,[r6,#0x0]
  000c8e24: movs r1,#0x2
  000c8e26: blx r5
  000c8e28: cmp r0,#0x0
  000c8e2a: mov.w r0,#0xffffffff
  000c8e2e: it eq
  000c8e30: mov.eq r0,#0x1
  000c8e32: movw r9,#0x4e20
  000c8e36: mla r0,r0,r10,r9
  000c8e3a: movw r1,#0xc350
  000c8e3e: mov.w r8,#0xffffffff
  000c8e42: vmov s0,r0
  000c8e46: vcvt.f32.s32 s0,s0
  000c8e4a: vldr.32 s2,[r4,#0x18c]
  000c8e4e: vadd.f32 s0,s2,s0
  000c8e52: vstr.32 s0,[r4,#0x18c]
  000c8e56: ldr r0,[r6,#0x0]
  000c8e58: blx r5
  000c8e5a: mov r10,r0
  000c8e5c: ldr r0,[r6,#0x0]
  000c8e5e: movs r1,#0x2
  000c8e60: blx r5
  000c8e62: cmp r0,#0x0
  000c8e64: mov.w r0,#0xffffffff
  000c8e68: it eq
  000c8e6a: mov.eq r0,#0x1
  000c8e6c: movw r1,#0x2710
  000c8e70: mla r0,r0,r10,r1
  000c8e74: movw r1,#0xc350
  000c8e78: vmov s0,r0
  000c8e7c: vcvt.f32.s32 s0,s0
  000c8e80: vldr.32 s2,[r4,#0x190]
  000c8e84: vadd.f32 s0,s2,s0
  000c8e88: vstr.32 s0,[r4,#0x190]
  000c8e8c: ldr r0,[r6,#0x0]
  000c8e8e: blx r5
  000c8e90: mov r10,r0
  000c8e92: ldr r0,[r6,#0x0]
  000c8e94: movs r1,#0x2
  000c8e96: blx r5
  000c8e98: cmp r0,#0x0
  000c8e9a: it eq
  000c8e9c: mov.eq.w r8,#0x1
  000c8ea0: mla r0,r8,r10,r9
  000c8ea4: vmov s0,r0
  000c8ea8: movs r0,#0x18
  000c8eaa: vcvt.f32.s32 s0,s0
  000c8eae: vldr.32 s2,[r4,#0x194]
  000c8eb2: vadd.f32 s0,s2,s0
  000c8eb6: vstr.32 s0,[r4,#0x194]
  000c8eba: vldr.32 s2,[r4,#0x18c]
  000c8ebe: vcvt.s32.f32 s2,s2
  000c8ec2: vstr.32 s2,[sp,#0x180]
  000c8ec6: vldr.32 s2,[r4,#0x190]
  000c8eca: vcvt.s32.f32 s0,s0
  000c8ece: vcvt.s32.f32 s2,s2
  000c8ed2: vstr.32 s0,[sp,#0x188]
  000c8ed6: vstr.32 s2,[sp,#0x184]
  000c8eda: blx 0x0006eb24
  000c8ede: mov r5,r0
  000c8ee0: add r1,sp,#0x180
  000c8ee2: movs r2,#0x3
  000c8ee4: blx 0x00073eb8
  000c8ee8: ldr r6,[sp,#0x48]
  000c8eea: mov.w r8,#0x0
  000c8eee: mov.w r9,#0x1
  000c8ef2: movs r4,#0x0
  000c8ef4: b 0x000c8f2e
  000c8ef6: mov r0,r5
  000c8ef8: blx 0x000728bc
  000c8efc: strd r0,r9,[sp,#0x0]
  000c8f00: mov r0,r6
  000c8f02: movs r1,#0xa
  000c8f04: movs r2,#0x0
  000c8f06: movs r3,#0x2c
  000c8f08: str.w r8,[sp,#0x8]
  000c8f0c: blx 0x00073e88
  000c8f10: ldr.w r1,[r6,#0xf8]
  000c8f14: ldr r1,[r1,#0x4]
  000c8f16: str.w r0,[r1,r4,lsl #0x2]
  000c8f1a: movs r1,#0x1
  000c8f1c: ldr.w r0,[r6,#0xf8]
  000c8f20: ldr r0,[r0,#0x4]
  000c8f22: ldr.w r0,[r0,r4,lsl #0x2]
  000c8f26: ldr r0,[r0,#0x4]
  000c8f28: blx 0x00073e94
  000c8f2c: adds r4,#0x1
  000c8f2e: cmp r4,#0x2
  000c8f30: bne 0x000c8ef6
  000c8f32: movs r0,#0x1c
  000c8f34: blx 0x0006eb24
  000c8f38: mov r5,r0
  000c8f3a: ldr r3,[sp,#0x48]
  000c8f3c: movs r1,#0x4
  000c8f3e: movs r2,#0x1
  000c8f40: blx 0x00073edc
  000c8f44: b.w 0x000cb9b8
  000c8f5c: ldr r0,[0x000c9338]
  000c8f5e: ldr r2,[0x000c933c]
  000c8f60: add r0,pc
  000c8f62: ldr r1,[0x000c9340]
  000c8f64: add r2,pc
  000c8f66: ldr r5,[r0,#0x0]
  000c8f68: ldr r4,[r2,#0x0]
  000c8f6a: mov r9,r1
  000c8f6c: ldr r0,[r5,#0x0]
  000c8f6e: blx r4
  000c8f70: ldr r6,[0x000c9344]
  000c8f72: mov.w r8,#0x0
  000c8f76: mov r1,r9
  000c8f78: add r0,r6
  000c8f7a: str r0,[sp,#0x180]
  000c8f7c: ldr r0,[r5,#0x0]
  000c8f7e: strd r8,r6,[sp,#0x184]
  000c8f82: blx r4
  000c8f84: ldr r1,[r5,#0x0]
  000c8f86: add r0,r6
  000c8f88: strd r0,r8,[sp,#0x18c]
  000c8f8c: ldr r2,[0x000c9348]
  000c8f8e: mov r0,r1
  000c8f90: mov r1,r9
  000c8f92: str r2,[sp,#0x194]
  000c8f94: blx r4
  000c8f96: add r0,r6
  000c8f98: strd r0,r8,[sp,#0x198]
  000c8f9c: ldr r1,[0x000c934c]
  000c8f9e: movs r0,#0x18
  000c8fa0: mov r10,r6
  000c8fa2: str r1,[sp,#0x1a0]
  000c8fa4: blx 0x0006eb24
  000c8fa8: mov r6,r0
  000c8faa: add r1,sp,#0x180
  000c8fac: movs r2,#0x9
  000c8fae: blx 0x00073eb8
  000c8fb2: ldr r0,[sp,#0x48]
  000c8fb4: mov r1,r9
  000c8fb6: str.w r6,[r0,#0x110]
  000c8fba: mov r6,r9
  000c8fbc: ldr r0,[r5,#0x0]
  000c8fbe: blx r4
  000c8fc0: add r0,r10
  000c8fc2: ldr r1,[0x000c9350]
  000c8fc4: str r0,[sp,#0x140]
  000c8fc6: ldr r0,[r5,#0x0]
  000c8fc8: strd r8,r1,[sp,#0x144]
  000c8fcc: mov r1,r9
  000c8fce: blx r4
  000c8fd0: ldr r1,[r5,#0x0]
  000c8fd2: add r0,r10
  000c8fd4: strd r0,r8,[sp,#0x14c]
  000c8fd8: ldr r2,[0x000c9354]
  000c8fda: mov r0,r1
  000c8fdc: mov r1,r9
  000c8fde: str r2,[sp,#0x154]
  000c8fe0: blx r4
  000c8fe2: add r0,r10
  000c8fe4: add r1,sp,#0x158
  000c8fe6: stm r1,{r0,r8,r10}
  000c8fea: movs r0,#0x18
  000c8fec: blx 0x0006eb24
  000c8ff0: mov r5,r0
  000c8ff2: add r1,sp,#0x140
  000c8ff4: movs r2,#0x9
  000c8ff6: blx 0x00073eb8
  000c8ffa: ldr r0,[sp,#0x48]
  000c8ffc: str.w r5,[r0,#0x10c]
  000c9000: mov.w r0,#0x138
  000c9004: blx 0x0006eb24
  000c9008: ldr r1,[0x000c9358]
  000c900a: ldr r3,[0x000c935c]
  000c900c: movs r4,#0x0
  000c900e: mov.w r2,#0x3e8
  000c9012: str r4,[sp,#0x0]
  000c9014: str r0,[sp,#0x38]
  000c9016: blx 0x00073e4c
  000c901a: mov.w r0,#0x138
  000c901e: blx 0x0006eb24
  000c9022: movw r1,#0x4e20
  000c9026: movw r2,#0x2710
  000c902a: movw r3,#0x2710
  000c902e: str r4,[sp,#0x0]
  000c9030: str r0,[sp,#0x3c]
  000c9032: blx 0x00073e4c
  000c9036: movs r0,#0xc
  000c9038: blx 0x0006eb24
  000c903c: mov r5,r0
  000c903e: blx 0x00073d08
  000c9042: ldr r4,[sp,#0x48]
  000c9044: movs r0,#0xa
  000c9046: mov r1,r5
  000c9048: str.w r5,[r4,#0xf8]
  000c904c: blx 0x00073d14
  000c9050: ldr r6,[sp,#0x38]
  000c9052: mov r0,r4
  000c9054: movw r2,#0x4974
  000c9058: movs r3,#0x1
  000c905a: mov.w r8,#0x1
  000c905e: mov r1,r6
  000c9060: blx 0x00073f6c
  000c9064: ldr.w r1,[r4,#0xf8]
  000c9068: add.w r9,sp,#0x118
  000c906c: ldr r1,[r1,#0x4]
  000c906e: str r0,[r1,#0x0]
  000c9070: mov r1,r6
  000c9072: ldr.w r0,[r4,#0xf8]
  000c9076: ldr r0,[r0,#0x4]
  000c9078: ldr r5,[r0,#0x0]
  000c907a: ldr r0,[r6,#0x0]
  000c907c: ldr r2,[r0,#0x28]
  000c907e: mov r0,r9
  000c9080: blx r2
  000c9082: ldr r0,[r5,#0x0]
  000c9084: mov r1,r9
  000c9086: ldr r2,[r0,#0x44]
  000c9088: mov r0,r5
  000c908a: blx r2
  000c908c: ldr.w r0,[r4,#0xf8]
  000c9090: movs r1,#0x0
  000c9092: mov.w r9,#0x0
  000c9096: ldr r0,[r0,#0x4]
  000c9098: ldr r0,[r0,#0x0]
  000c909a: blx 0x00073f54
  000c909e: ldr.w r0,[r4,#0xf8]
  000c90a2: movs r1,#0x1
  000c90a4: ldr r0,[r0,#0x4]
  000c90a6: ldr r0,[r0,#0x0]
  000c90a8: strb.w r9,[r0,#0x70]
  000c90ac: blx 0x00072748
  000c90b0: ldr.w r0,[r4,#0xf8]
  000c90b4: movs r1,#0x1
  000c90b6: ldr r0,[r0,#0x4]
  000c90b8: ldr r0,[r0,#0x0]
  000c90ba: ldr r0,[r0,#0x4]
  000c90bc: blx 0x00073fb4
  000c90c0: ldr.w r0,[r4,#0xf8]
  000c90c4: ldr r1,[0x000c9360]
  000c90c6: ldr r0,[r0,#0x4]
  000c90c8: mov r10,r1
  000c90ca: ldr r0,[r0,#0x0]
  000c90cc: ldr r0,[r0,#0x4]
  000c90ce: blx 0x000727e4
  000c90d2: ldr.w r0,[r4,#0xf8]
  000c90d6: ldr r0,[r0,#0x4]
  000c90d8: ldr r2,[r0,#0x0]
  000c90da: ldr.w r0,[r2,#0x50]!
  000c90de: cmp r0,#0x0
  000c90e0: strb.w r8,[r2,#0x24]
  000c90e4: beq 0x000c90fc
  000c90e6: blx 0x00070204
  000c90ea: blx 0x0006eb48
  000c90ee: ldr r0,[sp,#0x48]
  000c90f0: ldr.w r0,[r0,#0xf8]
  000c90f4: ldr r0,[r0,#0x4]
  000c90f6: ldr r0,[r0,#0x0]
  000c90f8: add.w r2,r0,#0x50
  000c90fc: ldr r5,[sp,#0x48]
  000c90fe: movs r3,#0x1
  000c9100: ldr r1,[sp,#0x38]
  000c9102: str.w r9,[r2,#0x0]
  000c9106: movw r2,#0x5279
  000c910a: mov r0,r5
  000c910c: blx 0x00073f6c
  000c9110: ldr.w r1,[r5,#0xf8]
  000c9114: movs r2,#0x0
  000c9116: movs r3,#0x0
  000c9118: ldr r1,[r1,#0x4]
  000c911a: str r0,[r1,#0x4]
  000c911c: ldr.w r0,[r5,#0xf8]
  000c9120: ldr r0,[r0,#0x4]
  000c9122: ldr r0,[r0,#0x4]
  000c9124: ldr r1,[r0,#0x0]
  000c9126: ldr r6,[r1,#0x48]
  000c9128: movs r1,#0x0
  000c912a: blx r6
  000c912c: ldr.w r0,[r5,#0xf8]
  000c9130: movs r1,#0x0
  000c9132: ldr r0,[r0,#0x4]
  000c9134: ldr r0,[r0,#0x4]
  000c9136: blx 0x00073f54
  000c913a: ldr.w r0,[r5,#0xf8]
  000c913e: movs r1,#0x0
  000c9140: ldr r0,[r0,#0x4]
  000c9142: ldr r0,[r0,#0x4]
  000c9144: blx 0x00072748
  000c9148: ldr.w r0,[r5,#0xf8]
  000c914c: movs r1,#0x1
  000c914e: ldr r0,[r0,#0x4]
  000c9150: ldr r0,[r0,#0x4]
  000c9152: ldr r0,[r0,#0x4]
  000c9154: blx 0x00073fb4
  000c9158: ldr.w r0,[r5,#0xf8]
  000c915c: mov r1,r10
  000c915e: ldr r0,[r0,#0x4]
  000c9160: ldr r0,[r0,#0x4]
  000c9162: ldr r0,[r0,#0x4]
  000c9164: blx 0x000727e4
  000c9168: ldr.w r0,[r5,#0xf8]
  000c916c: ldr r1,[0x000c9364]
  000c916e: ldr r0,[r0,#0x4]
  000c9170: ldr r0,[r0,#0x4]
  000c9172: ldr r0,[r0,#0x8]
  000c9174: strd r9,r1,[sp,#0x118]
  000c9178: add r1,sp,#0x118
  000c917a: str.w r9,[sp,#0x120]
  000c917e: blx 0x00072478
  000c9182: ldr.w r0,[r5,#0xf8]
  000c9186: movs r1,#0x0
  000c9188: ldr r0,[r0,#0x4]
  000c918a: ldr r0,[r0,#0x4]
  000c918c: blx 0x000732f4
  000c9190: ldr.w r0,[r5,#0xf8]
  000c9194: ldr r0,[r0,#0x4]
  000c9196: ldr r1,[r0,#0x4]
  000c9198: ldr.w r0,[r1,#0x50]!
  000c919c: cbz r0,0x000c91b4
  000c919e: blx 0x00070204
  000c91a2: blx 0x0006eb48
  000c91a6: ldr r0,[sp,#0x48]
  000c91a8: ldr.w r0,[r0,#0xf8]
  000c91ac: ldr r0,[r0,#0x4]
  000c91ae: ldr r0,[r0,#0x4]
  000c91b0: add.w r1,r0,#0x50
  000c91b4: ldr r4,[0x000c9368]
  000c91b6: add.w r10,sp,#0x1c0
  000c91ba: add r6,sp,#0x118
  000c91bc: movs r2,#0x0
  000c91be: mov.w r9,#0x8
  000c91c2: movw r8,#0x61a8
  000c91c6: movw r11,#0x1388
  000c91ca: str r2,[r1,#0x0]
  000c91cc: b 0x000c9282
  000c91ce: ldr r0,[sp,#0x3c]
  000c91d0: movs r1,#0x0
  000c91d2: str r0,[sp,#0x0]
  000c91d4: movs r0,#0x1
  000c91d6: strd r0,r2,[sp,#0x4]
  000c91da: movs r2,#0x0
  000c91dc: ldr r5,[sp,#0x48]
  000c91de: movs r3,#0x33
  000c91e0: mov r0,r5
  000c91e2: blx 0x00073e88
  000c91e6: ldr.w r1,[r5,#0xf8]
  000c91ea: cmp.w r9,#0x8
  000c91ee: ldr r1,[r1,#0x4]
  000c91f0: str.w r0,[r1,r9]
  000c91f4: ldr.w r0,[r5,#0xf8]
  000c91f8: ldr r0,[r0,#0x4]
  000c91fa: ldr.w r5,[r0,r9]
  000c91fe: bne 0x000c9218
  000c9200: ldr r1,[sp,#0x38]
  000c9202: ldr r0,[r1,#0x0]
  000c9204: ldr r2,[r0,#0x28]
  000c9206: mov r0,r10
  000c9208: blx r2
  000c920a: ldr r0,[0x000c936c]
  000c920c: str r0,[sp,#0xf8]
  000c920e: ldr r0,[0x000c9370]
  000c9210: str r0,[sp,#0xf4]
  000c9212: ldr r0,[0x000c9374]
  000c9214: str r0,[sp,#0xfc]
  000c9216: b 0x000c923e
  000c9218: ldr r1,[sp,#0x38]
  000c921a: ldr r0,[r1,#0x0]
  000c921c: ldr r2,[r0,#0x28]
  000c921e: mov r0,r10
  000c9220: blx r2
  000c9222: vmov s0,r8
  000c9226: ldr r0,[0x000c9378]
  000c9228: vmov s2,r4
  000c922c: vcvt.f32.s32 s0,s0
  000c9230: vcvt.f32.s32 s2,s2
  000c9234: str r0,[sp,#0xf8]
  000c9236: vstr.32 s0,[sp,#0xf4]
  000c923a: vstr.32 s2,[sp,#0xfc]
  000c923e: add r2,sp,#0xf4
  000c9240: mov r0,r6
  000c9242: mov r1,r10
  000c9244: blx 0x0006f1cc
  000c9248: ldr r0,[r5,#0x0]
  000c924a: mov r1,r6
  000c924c: ldr r2,[r0,#0x44]
  000c924e: mov r0,r5
  000c9250: blx r2
  000c9252: ldr r0,[sp,#0x48]
  000c9254: movs r1,#0x1
  000c9256: mov r5,r0
  000c9258: ldr.w r0,[r0,#0xf8]
  000c925c: ldr r0,[r0,#0x4]
  000c925e: ldr.w r0,[r0,r9]
  000c9262: ldr r0,[r0,#0x4]
  000c9264: blx 0x00073fb4
  000c9268: ldr.w r0,[r5,#0xf8]
  000c926c: movs r2,#0x0
  000c926e: sub.w r4,r4,r11
  000c9272: add r8,r11
  000c9274: ldr r0,[r0,#0x4]
  000c9276: ldr.w r0,[r0,r9]
  000c927a: add.w r9,r9,#0x4
  000c927e: strh.w r2,[r0,#0x13d]
  000c9282: cmp.w r9,#0x18
  000c9286: bne 0x000c91ce
  000c9288: ldr r1,[0x000c937c]
  000c928a: add r0,sp,#0x118
  000c928c: movs r2,#0x24
  000c928e: add r1,pc
  000c9290: blx 0x0006f1e4
  000c9294: ldr r0,[0x000c9380]
  000c9296: add r0,pc
  000c9298: vldr.64 d16,[r0]
  000c929c: ldr r0,[r0,#0x8]
  000c929e: str r0,[sp,#0x1c8]
  000c92a0: movs r0,#0xc
  000c92a2: vstr.64 d16,[sp,#0x1c0]
  000c92a6: blx 0x0006eb24
  000c92aa: mov r6,r0
  000c92ac: blx 0x00073d08
  000c92b0: movs r0,#0x3
  000c92b2: mov r1,r6
  000c92b4: blx 0x00073d14
  000c92b8: ldr r0,[sp,#0x48]
  000c92ba: ldr r1,[r6,#0x4]
  000c92bc: mov r2,r0
  000c92be: ldr.w r0,[r0,#0xf8]
  000c92c2: ldr r0,[r0,#0x4]
  000c92c4: ldr r0,[r0,#0x0]
  000c92c6: str r0,[r1,#0x0]
  000c92c8: ldr.w r0,[r2,#0xf8]
  000c92cc: ldr r1,[r6,#0x4]
  000c92ce: ldr r0,[r0,#0x4]
  000c92d0: ldr r0,[r0,#0x4]
  000c92d2: str r0,[r1,#0x8]
  000c92d4: movs r0,#0x18
  000c92d6: blx 0x0006eb24
  000c92da: mov r5,r0
  000c92dc: movs r0,#0x9
  000c92de: add r1,sp,#0x118
  000c92e0: add r3,sp,#0x1c0
  000c92e2: str r0,[sp,#0x0]
  000c92e4: mov r0,r5
  000c92e6: mov r2,r6
  000c92e8: blx 0x00073fc0
  000c92ec: mov r0,r5
  000c92ee: movs r1,#0x1
  000c92f0: blx 0x00073f48
  000c92f4: movs r4,#0x2
  000c92f6: b 0x000c931e
  000c92f8: ldr r0,[sp,#0x48]
  000c92fa: cmp r4,#0x2
  000c92fc: ldr.w r0,[r0,#0xf8]
  000c9300: ldr r0,[r0,#0x4]
  000c9302: ldr.w r6,[r0,r4,lsl #0x2]
  000c9306: bne 0x000c930e
  000c9308: mov r0,r6
  000c930a: mov r1,r5
  000c930c: b 0x000c9318
  000c930e: mov r0,r5
  000c9310: blx 0x00073f3c
  000c9314: mov r1,r0
  000c9316: mov r0,r6
  000c9318: blx 0x00073f60
  000c931c: adds r4,#0x1
  000c931e: cmp r4,#0x6
  000c9320: bne 0x000c92f8
  000c9322: movs r4,#0x6
  000c9324: mov.w r8,#0x0
  000c9328: mov.w r9,#0x1
  000c932c: ldr r6,[0x000c9384]
  000c932e: add.w r10,sp,#0xf4
  000c9332: ldr r5,[sp,#0x48]
  000c9334: b 0x000c93e2
  000c9388: ldr r0,[sp,#0x3c]
  000c938a: movs r1,#0xa
  000c938c: strd r0,r9,[sp,#0x0]
  000c9390: mov r0,r5
  000c9392: movs r2,#0x0
  000c9394: movs r3,#0x2c
  000c9396: str.w r8,[sp,#0x8]
  000c939a: blx 0x00073e88
  000c939e: ldr.w r1,[r5,#0xf8]
  000c93a2: ldr r1,[r1,#0x4]
  000c93a4: str.w r0,[r1,r4,lsl #0x2]
  000c93a8: ldr.w r0,[r5,#0xf8]
  000c93ac: ldr r0,[r0,#0x4]
  000c93ae: ldr.w r0,[r0,r4,lsl #0x2]
  000c93b2: strd r6,r6,[sp,#0xf4]
  000c93b6: str r6,[sp,#0xfc]
  000c93b8: ldr r1,[r0,#0x0]
  000c93ba: ldr r2,[r1,#0x44]
  000c93bc: mov r1,r10
  000c93be: blx r2
  000c93c0: ldr.w r0,[r5,#0xf8]
  000c93c4: ldr r0,[r0,#0x4]
  000c93c6: ldr.w r0,[r0,r4,lsl #0x2]
  000c93ca: blx 0x000732dc
  000c93ce: ldr.w r0,[r5,#0xf8]
  000c93d2: movs r1,#0x1
  000c93d4: ldr r0,[r0,#0x4]
  000c93d6: ldr.w r0,[r0,r4,lsl #0x2]
  000c93da: ldr r0,[r0,#0x4]
  000c93dc: blx 0x00073e94
  000c93e0: adds r4,#0x1
  000c93e2: cmp r4,#0xa
  000c93e4: bne 0x000c9388
  000c93e6: movs r0,#0x1c
  000c93e8: blx 0x0006eb24
  000c93ec: mov r5,r0
  000c93ee: ldr r3,[sp,#0x48]
  000c93f0: movs r1,#0x4
  000c93f2: movs r2,#0x8
  000c93f4: blx 0x00073edc
  000c93f8: ldr r0,[sp,#0x48]
  000c93fa: mov r4,r0
  000c93fc: str r5,[r0,#0x28]
  000c93fe: movs r0,#0x1c
  000c9400: blx 0x0006eb24
  000c9404: mov r5,r0
  000c9406: movs r1,#0x12
  000c9408: movs r2,#0x2
  000c940a: movs r3,#0x6
  000c940c: str r4,[sp,#0x0]
  000c940e: blx 0x00073f9c
  000c9412: ldr r0,[sp,#0x48]
  000c9414: str r5,[r0,#0x2c]
  000c9416: b.w 0x000cb9bc
  000c941a: ldr r0,[0x000c978c]
  000c941c: add r4,sp,#0x180
  000c941e: add r0,pc
  000c9420: vld1.64 {d16,d17},[r0]!
  000c9424: vldr.64 d18,[r0]
  000c9428: mov r0,r4
  000c942a: vst1.64 {d16,d17},[r0]!
  000c942e: vstr.64 d18,[r0]
  000c9432: movs r0,#0x18
  000c9434: blx 0x0006eb24
  000c9438: mov r5,r0
  000c943a: mov r1,r4
  000c943c: movs r2,#0x3
  000c943e: blx 0x00073eb8
  000c9442: ldr r0,[sp,#0x48]
  000c9444: str.w r5,[r0,#0x110]
  000c9448: movs r0,#0xc
  000c944a: blx 0x0006eb24
  000c944e: mov r5,r0
  000c9450: blx 0x00073d08
  000c9454: ldr r0,[sp,#0x48]
  000c9456: mov r1,r5
  000c9458: str.w r5,[r0,#0xf8]
  000c945c: movs r0,#0x5
  000c945e: blx 0x00073d14
  000c9462: ldr r4,[sp,#0x48]
  000c9464: mov.w r8,#0x0
  000c9468: movs r5,#0x1
  000c946a: movs r6,#0x0
  000c946c: b 0x000c94a6
  000c946e: ldr.w r0,[r4,#0x110]
  000c9472: movs r1,#0x0
  000c9474: blx 0x00073ec4
  000c9478: stm sp,{r0,r5,r8}
  000c947c: mov r0,r4
  000c947e: movs r1,#0x0
  000c9480: movs r2,#0x0
  000c9482: movs r3,#0x25
  000c9484: blx 0x00073e88
  000c9488: ldr.w r1,[r4,#0xf8]
  000c948c: ldr r1,[r1,#0x4]
  000c948e: str.w r0,[r1,r6,lsl #0x2]
  000c9492: movs r1,#0x1
  000c9494: ldr.w r0,[r4,#0xf8]
  000c9498: ldr r0,[r0,#0x4]
  000c949a: ldr.w r0,[r0,r6,lsl #0x2]
  000c949e: ldr r0,[r0,#0x4]
  000c94a0: blx 0x00073fb4
  000c94a4: adds r6,#0x1
  000c94a6: cmp r6,#0x2
  000c94a8: bne 0x000c946e
  000c94aa: movs r4,#0x2
  000c94ac: mov.w r8,#0x0
  000c94b0: movs r6,#0x1
  000c94b2: ldr r5,[sp,#0x48]
  000c94b4: b 0x000c94fc
  000c94b6: ldr.w r0,[r5,#0x110]
  000c94ba: movs r1,#0x0
  000c94bc: blx 0x00073ec4
  000c94c0: stm sp,{r0,r6,r8}
  000c94c4: mov r0,r5
  000c94c6: movs r1,#0xa
  000c94c8: movs r2,#0x0
  000c94ca: movs r3,#0x2c
  000c94cc: blx 0x00073e88
  000c94d0: ldr.w r1,[r5,#0xf8]
  000c94d4: ldr r1,[r1,#0x4]
  000c94d6: str.w r0,[r1,r4,lsl #0x2]
  000c94da: ldr.w r0,[r5,#0xf8]
  000c94de: ldr r0,[r0,#0x4]
  000c94e0: ldr.w r0,[r0,r4,lsl #0x2]
  000c94e4: blx 0x000732dc
  000c94e8: ldr.w r0,[r5,#0xf8]
  000c94ec: movs r1,#0x1
  000c94ee: ldr r0,[r0,#0x4]
  000c94f0: ldr.w r0,[r0,r4,lsl #0x2]
  000c94f4: ldr r0,[r0,#0x4]
  000c94f6: blx 0x00073e94
  000c94fa: adds r4,#0x1
  000c94fc: cmp r4,#0x5
  000c94fe: bne 0x000c94b6
  000c9500: ldr r0,[sp,#0x48]
  000c9502: add r4,sp,#0x118
  000c9504: ldr.w r1,[r0,#0xec]
  000c9508: mov r0,r4
  000c950a: blx 0x000727cc
  000c950e: add r0,sp,#0x140
  000c9510: mov r1,r4
  000c9512: blx 0x0006ec80
  000c9516: vldr.32 s0,[pc,#0x278]
  000c951a: movs r0,#0x18
  000c951c: vldr.32 s2,[sp,#0x140]
  000c9520: movs r6,#0x0
  000c9522: vldr.32 s4,[sp,#0x148]
  000c9526: vmul.f32 s2,s2,s0
  000c952a: vmul.f32 s0,s4,s0
  000c952e: vcvt.s32.f32 s2,s2
  000c9532: vcvt.s32.f32 s0,s0
  000c9536: str r6,[sp,#0x11c]
  000c9538: vstr.32 s2,[sp,#0x118]
  000c953c: vstr.32 s0,[sp,#0x120]
  000c9540: blx 0x0006eb24
  000c9544: mov r4,r0
  000c9546: add r1,sp,#0x118
  000c9548: movs r2,#0x3
  000c954a: blx 0x00073eb8
  000c954e: ldr r5,[sp,#0x48]
  000c9550: mov.w r1,#0x3f800000
  000c9554: add r2,sp,#0x1c0
  000c9556: ldr.w r0,[r5,#0xf0]
  000c955a: ldr r0,[r0,#0x8]
  000c955c: strd r6,r1,[sp,#0x1c0]
  000c9560: add r1,sp,#0x140
  000c9562: str r6,[sp,#0x1c8]
  000c9564: blx 0x000726ac
  000c9568: ldr.w r0,[r5,#0xf0]
  000c956c: mov r1,r4
  000c956e: blx 0x00073cc0
  000c9572: mov r0,r5
  000c9574: mov r1,r4
  000c9576: blx 0x00074170
  000c957a: ldr r6,[sp,#0x48]
  000c957c: mov.w r9,#0x0
  000c9580: b 0x000c9666
  000c9582: ldr.w r0,[r6,#0xf8]
  000c9586: add r5,sp,#0xd0
  000c9588: ldr.w r1,[r6,#0xf0]
  000c958c: ldr r0,[r0,#0x4]
  000c958e: ldr.w r11,[r0,r9,lsl #0x2]
  000c9592: mov r0,r5
  000c9594: blx 0x0007264c
  000c9598: ldr.w r0,[r6,#0xf0]
  000c959c: add.w r8,sp,#0x94
  000c95a0: ldr r1,[r0,#0x8]
  000c95a2: mov r0,r8
  000c95a4: blx 0x0007261c
  000c95a8: adr r0,[0xc9794]
  000c95aa: cmp.w r9,#0x0
  000c95ae: it eq
  000c95b0: add.eq r0,#0x4
  000c95b2: add.w r10,sp,#0xa0
  000c95b6: ldr r2,[r0,#0x0]
  000c95b8: mov r1,r8
  000c95ba: mov r0,r10
  000c95bc: blx 0x0006ec74
  000c95c0: add.w r8,sp,#0xf4
  000c95c4: mov r1,r5
  000c95c6: mov r2,r10
  000c95c8: mov r0,r8
  000c95ca: blx 0x0006f1cc
  000c95ce: ldr.w r0,[r6,#0xf0]
  000c95d2: add r5,sp,#0x7c
  000c95d4: ldr r1,[r0,#0x8]
  000c95d6: mov r0,r5
  000c95d8: blx 0x00072574
  000c95dc: add.w r10,sp,#0x88
  000c95e0: ldr r2,[0x000c979c]
  000c95e2: mov r1,r5
  000c95e4: mov r0,r10
  000c95e6: blx 0x0006ec74
  000c95ea: add r5,sp,#0x1c0
  000c95ec: mov r1,r8
  000c95ee: mov r2,r10
  000c95f0: mov r0,r5
  000c95f2: blx 0x0006f1cc
  000c95f6: ldr.w r0,[r11,#0x0]
  000c95fa: mov r1,r5
  000c95fc: ldr r2,[r0,#0x44]
  000c95fe: mov r0,r11
  000c9600: blx r2
  000c9602: ldr.w r0,[r6,#0xf8]
  000c9606: mov.w r1,#0x3f800000
  000c960a: mov r2,r5
  000c960c: mov.w r8,#0x0
  000c9610: ldr r0,[r0,#0x4]
  000c9612: ldr.w r0,[r0,r9,lsl #0x2]
  000c9616: ldr r0,[r0,#0x8]
  000c9618: str r1,[sp,#0x1c4]
  000c961a: movs r1,#0x0
  000c961c: str r1,[sp,#0x1c0]
  000c961e: str r1,[sp,#0x1c8]
  000c9620: add r1,sp,#0x140
  000c9622: blx 0x000726ac
  000c9626: ldr.w r0,[r6,#0xf8]
  000c962a: ldr r0,[r0,#0x4]
  000c962c: ldr.w r5,[r0,r9,lsl #0x2]
  000c9630: mov r0,r4
  000c9632: blx 0x00073f3c
  000c9636: mov r1,r0
  000c9638: mov r0,r5
  000c963a: blx 0x00073f60
  000c963e: ldr.w r0,[r6,#0xf8]
  000c9642: movs r1,#0x0
  000c9644: ldr r0,[r0,#0x4]
  000c9646: ldr.w r0,[r0,r9,lsl #0x2]
  000c964a: blx 0x0007417c
  000c964e: ldr.w r0,[r6,#0xf8]
  000c9652: movs r1,#0x1
  000c9654: ldr r0,[r0,#0x4]
  000c9656: ldr.w r0,[r0,r9,lsl #0x2]
  000c965a: strb.w r8,[r0,#0x13d]
  000c965e: blx 0x00074158
  000c9662: add.w r9,r9,#0x1
  000c9666: cmp.w r9,#0x2
  000c966a: bne 0x000c9582
  000c966c: b.w 0x000cb9bc
  000c9670: ldr r0,[0x000c97a0]
  000c9672: add r4,sp,#0x180
  000c9674: add r0,pc
  000c9676: vld1.64 {d16,d17},[r0]!
  000c967a: vldr.64 d18,[r0]
  000c967e: mov r0,r4
  000c9680: vst1.64 {d16,d17},[r0]!
  000c9684: vstr.64 d18,[r0]
  000c9688: movs r0,#0x18
  000c968a: blx 0x0006eb24
  000c968e: mov r5,r0
  000c9690: mov r1,r4
  000c9692: movs r2,#0x6
  000c9694: blx 0x00073eb8
  000c9698: ldr r0,[sp,#0x48]
  000c969a: str.w r5,[r0,#0x110]
  000c969e: movs r0,#0xc
  000c96a0: blx 0x0006eb24
  000c96a4: mov r5,r0
  000c96a6: blx 0x00073d08
  000c96aa: ldr r6,[sp,#0x48]
  000c96ac: movs r0,#0x1
  000c96ae: mov r1,r5
  000c96b0: movs r4,#0x1
  000c96b2: str.w r5,[r6,#0xf8]
  000c96b6: blx 0x00073d14
  000c96ba: ldr.w r0,[r6,#0x110]
  000c96be: movs r1,#0x0
  000c96c0: mov.w r8,#0x0
  000c96c4: blx 0x00073ec4
  000c96c8: stm sp,{r0,r4,r8}
  000c96cc: mov r0,r6
  000c96ce: movs r1,#0xa
  000c96d0: movs r2,#0x0
  000c96d2: movs r3,#0x2c
  000c96d4: blx 0x00073e88
  000c96d8: ldr.w r1,[r6,#0xf8]
  000c96dc: ldr r1,[r1,#0x4]
  000c96de: str r0,[r1,#0x0]
  000c96e0: movs r1,#0x0
  000c96e2: ldr.w r0,[r6,#0xf8]
  000c96e6: ldr r0,[r0,#0x4]
  000c96e8: ldr r4,[r0,#0x0]
  000c96ea: ldr.w r0,[r6,#0x110]
  000c96ee: blx 0x00073ec4
  000c96f2: mov r1,r0
  000c96f4: ldr r0,[r0,#0x0]
  000c96f6: add r5,sp,#0x140
  000c96f8: ldr r2,[r0,#0x28]
  000c96fa: mov r0,r5
  000c96fc: blx r2
  000c96fe: ldr r0,[r4,#0x0]
  000c9700: mov r1,r5
  000c9702: ldr r2,[r0,#0x44]
  000c9704: mov r0,r4
  000c9706: blx r2
  000c9708: ldr.w r0,[r6,#0xf8]
  000c970c: add r2,sp,#0x118
  000c970e: ldr r1,[0x000c97a4]
  000c9710: ldr r0,[r0,#0x4]
  000c9712: ldr r0,[r0,#0x0]
  000c9714: ldr r0,[r0,#0x8]
  000c9716: strd r1,r8,[sp,#0x140]
  000c971a: mov.w r1,#0x3f800000
  000c971e: str.w r8,[sp,#0x148]
  000c9722: strd r8,r1,[sp,#0x118]
  000c9726: add r1,sp,#0x140
  000c9728: str.w r8,[sp,#0x120]
  000c972c: blx 0x000726ac
  000c9730: ldr.w r1,[r6,#0xf8]
  000c9734: ldr.w r0,[r6,#0x110]
  000c9738: ldr r1,[r1,#0x4]
  000c973a: ldr r4,[r1,#0x0]
  000c973c: blx 0x00073f3c
  000c9740: mov r1,r0
  000c9742: mov r0,r4
  000c9744: blx 0x00073f60
  000c9748: ldr.w r0,[r6,#0xf8]
  000c974c: movs r1,#0x1
  000c974e: ldr r0,[r0,#0x4]
  000c9750: ldr r0,[r0,#0x0]
  000c9752: ldr r0,[r0,#0x4]
  000c9754: blx 0x00073fb4
  000c9758: ldr.w r0,[r6,#0xf8]
  000c975c: movs r1,#0x0
  000c975e: ldr r0,[r0,#0x4]
  000c9760: ldr r0,[r0,#0x0]
  000c9762: ldr r0,[r0,#0x4]
  000c9764: blx 0x00073e94
  000c9768: ldr.w r0,[r6,#0xf8]
  000c976c: movs r1,#0x1
  000c976e: ldr r0,[r0,#0x4]
  000c9770: ldr r0,[r0,#0x0]
  000c9772: ldr r0,[r0,#0x4]
  000c9774: blx 0x000727e4
  000c9778: ldr.w r0,[r6,#0xf8]
  000c977c: movs r1,#0x0
  000c977e: ldr r0,[r0,#0x4]
  000c9780: ldr r0,[r0,#0x0]
  000c9782: blx 0x00074164
  000c9786: b.w 0x000cb9bc
  000c97a8: ldr r0,[0x000c9940]
  000c97aa: add r0,pc
  000c97ac: vldr.64 d16,[r0]
  000c97b0: ldr r0,[r0,#0x8]
  000c97b2: str r0,[sp,#0x188]
  000c97b4: movs r0,#0x18
  000c97b6: vstr.64 d16,[sp,#0x180]
  000c97ba: blx 0x0006eb24
  000c97be: mov r5,r0
  000c97c0: add r1,sp,#0x180
  000c97c2: movs r2,#0x3
  000c97c4: blx 0x00073eb8
  000c97c8: ldr r0,[sp,#0x48]
  000c97ca: str.w r5,[r0,#0x110]
  000c97ce: movs r0,#0xc
  000c97d0: blx 0x0006eb24
  000c97d4: mov r5,r0
  000c97d6: blx 0x00073d08
  000c97da: ldr r4,[sp,#0x48]
  000c97dc: movs r0,#0x3
  000c97de: mov r1,r5
  000c97e0: str.w r5,[r4,#0xf8]
  000c97e4: blx 0x00073d14
  000c97e8: mov r0,r4
  000c97ea: movs r1,#0x0
  000c97ec: movw r2,#0x4a88
  000c97f0: movs r3,#0x1
  000c97f2: blx 0x00073f6c
  000c97f6: ldr.w r1,[r4,#0xf8]
  000c97fa: movs r2,#0x0
  000c97fc: movs r3,#0x0
  000c97fe: ldr r1,[r1,#0x4]
  000c9800: str r0,[r1,#0x0]
  000c9802: ldr.w r0,[r4,#0xf8]
  000c9806: ldr r0,[r0,#0x4]
  000c9808: ldr r0,[r0,#0x0]
  000c980a: ldr r1,[r0,#0x0]
  000c980c: ldr r6,[r1,#0x48]
  000c980e: movs r1,#0x0
  000c9810: blx r6
  000c9812: ldr.w r0,[r4,#0xf8]
  000c9816: movs r1,#0x0
  000c9818: ldr r0,[r0,#0x4]
  000c981a: ldr r0,[r0,#0x0]
  000c981c: blx 0x00073f54
  000c9820: ldr r0,[0x000c9944]
  000c9822: ldr.w r1,[r4,#0xf8]
  000c9826: add r0,pc
  000c9828: ldr r0,[r0,#0x0]
  000c982a: ldr r1,[r1,#0x4]
  000c982c: ldr r0,[r0,#0x0]
  000c982e: ldr r4,[r1,#0x0]
  000c9830: movw r1,#0xc8a
  000c9834: blx 0x00072f70
  000c9838: add r5,sp,#0x70
  000c983a: mov r1,r0
  000c983c: movs r2,#0x0
  000c983e: mov r0,r5
  000c9840: blx 0x0006f028
  000c9844: mov r0,r4
  000c9846: mov r1,r5
  000c9848: blx 0x00073fa8
  000c984c: add r0,sp,#0x70
  000c984e: blx 0x0006ee30
  000c9852: ldr r0,[sp,#0x48]
  000c9854: movs r1,#0x1
  000c9856: mov.w r8,#0x1
  000c985a: mov r4,r0
  000c985c: ldr.w r0,[r0,#0xf8]
  000c9860: ldr r0,[r0,#0x4]
  000c9862: ldr r0,[r0,#0x0]
  000c9864: blx 0x00072748
  000c9868: ldr.w r0,[r4,#0xf8]
  000c986c: movs r1,#0x1
  000c986e: ldr r0,[r0,#0x4]
  000c9870: ldr r0,[r0,#0x0]
  000c9872: ldr r0,[r0,#0x4]
  000c9874: blx 0x00073fb4
  000c9878: ldr.w r0,[r4,#0xf8]
  000c987c: ldr r0,[r0,#0x4]
  000c987e: ldr r2,[r0,#0x0]
  000c9880: ldr.w r0,[r2,#0x50]!
  000c9884: cbz r0,0x000c989c
  000c9886: blx 0x00070204
  000c988a: blx 0x0006eb48
  000c988e: ldr r0,[sp,#0x48]
  000c9890: ldr.w r0,[r0,#0xf8]
  000c9894: ldr r0,[r0,#0x4]
  000c9896: ldr r0,[r0,#0x0]
  000c9898: add.w r2,r0,#0x50
  000c989c: mov.w r9,#0x0
  000c98a0: movs r6,#0x1
  000c98a2: str.w r9,[r2,#0x0]
  000c98a6: ldr r0,[0x000c9948]
  000c98a8: ldr r5,[sp,#0x48]
  000c98aa: add r0,pc
  000c98ac: ldr.w r10,[r0,#0x0]
  000c98b0: b 0x000c9904
  000c98b2: ldr.w r0,[r10,#0x0]
  000c98b6: movs r1,#0x8
  000c98b8: blx 0x00071a34
  000c98bc: mov r4,r0
  000c98be: ldr.w r0,[r5,#0x110]
  000c98c2: movs r1,#0x0
  000c98c4: blx 0x00073ec4
  000c98c8: stm sp,{r0,r8,r9}
  000c98cc: mov r0,r5
  000c98ce: movs r1,#0x8
  000c98d0: movs r2,#0x0
  000c98d2: mov r3,r4
  000c98d4: blx 0x00073e88
  000c98d8: ldr.w r1,[r5,#0xf8]
  000c98dc: ldr r1,[r1,#0x4]
  000c98de: str.w r0,[r1,r6,lsl #0x2]
  000c98e2: ldr.w r0,[r5,#0xf8]
  000c98e6: ldr r0,[r0,#0x4]
  000c98e8: ldr.w r0,[r0,r6,lsl #0x2]
  000c98ec: blx 0x000732dc
  000c98f0: ldr.w r0,[r5,#0xf8]
  000c98f4: movs r1,#0x1
  000c98f6: ldr r0,[r0,#0x4]
  000c98f8: ldr.w r0,[r0,r6,lsl #0x2]
  000c98fc: ldr r0,[r0,#0x4]
  000c98fe: blx 0x00073e94
  000c9902: adds r6,#0x1
  000c9904: cmp r6,#0x3
  000c9906: bne 0x000c98b2
  000c9908: movs r4,#0x0
  000c990a: b 0x000c991a
  000c990c: ldr r0,[r0,#0x4]
  000c990e: movs r1,#0x9b
  000c9910: ldr.w r0,[r0,r4,lsl #0x2]
  000c9914: blx 0x00074188
  000c9918: adds r4,#0x1
  000c991a: ldr r0,[sp,#0x48]
  000c991c: ldr.w r0,[r0,#0xfc]
  000c9920: ldr r1,[r0,#0x0]
  000c9922: cmp r4,r1
  000c9924: bcc 0x000c990c
  000c9926: b.w 0x000cb9bc
  000c992a: movs r0,#0x1c
  000c992c: blx 0x0006eb24
  000c9930: mov r5,r0
  000c9932: ldr r3,[sp,#0x48]
  000c9934: movs r1,#0x4
  000c9936: movs r2,#0x4
  000c9938: blx 0x00073edc
  000c993c: b.w 0x000cb9b8
  000c994c: ldr r0,[sp,#0x44]
  000c994e: add r4,sp,#0x140
  000c9950: movs r2,#0x0
  000c9952: ldr r0,[r0,#0x0]
  000c9954: ldr r1,[0x000c9d40]
  000c9956: add r1,pc
  000c9958: str.w r2,[r0,#0x178]
  000c995c: vld1.64 {d16,d17},[r1]!
  000c9960: mov r0,r4
  000c9962: vldr.64 d18,[r1]
  000c9966: vst1.64 {d16,d17},[r0]!
  000c996a: vstr.64 d18,[r0]
  000c996e: movs r0,#0x18
  000c9970: blx 0x0006eb24
  000c9974: mov r5,r0
  000c9976: mov r1,r4
  000c9978: movs r2,#0x6
  000c997a: blx 0x00073eb8
  000c997e: ldr r0,[sp,#0x48]
  000c9980: movs r1,#0x1
  000c9982: str.w r5,[r0,#0x110]
  000c9986: mov r0,r5
  000c9988: blx 0x00073f48
  000c998c: ldr r0,[0x000c9d44]
  000c998e: add r4,sp,#0x118
  000c9990: add r0,pc
  000c9992: vld1.64 {d16,d17},[r0]!
  000c9996: vldr.64 d18,[r0]
  000c999a: mov r0,r4
  000c999c: vst1.64 {d16,d17},[r0]!
  000c99a0: vstr.64 d18,[r0]
  000c99a4: movs r0,#0x18
  000c99a6: blx 0x0006eb24
  000c99aa: mov r5,r0
  000c99ac: mov r1,r4
  000c99ae: movs r2,#0x6
  000c99b0: blx 0x00073eb8
  000c99b4: ldr r0,[sp,#0x48]
  000c99b6: movs r1,#0x1
  000c99b8: str.w r5,[r0,#0x10c]
  000c99bc: mov r0,r5
  000c99be: blx 0x00073f48
  000c99c2: movs r0,#0xc
  000c99c4: blx 0x0006eb24
  000c99c8: mov r5,r0
  000c99ca: blx 0x00073d08
  000c99ce: ldr r0,[sp,#0x48]
  000c99d0: mov r1,r5
  000c99d2: str.w r5,[r0,#0xf8]
  000c99d6: movs r0,#0x16
  000c99d8: blx 0x00073d14
  000c99dc: mov.w r0,#0x138
  000c99e0: blx 0x0006eb24
  000c99e4: ldr r5,[0x000c9d48]
  000c99e6: mov r9,r0
  000c99e8: ldr r1,[0x000c9d4c]
  000c99ea: ldr r3,[0x000c9d50]
  000c99ec: movs r4,#0x0
  000c99ee: mov r2,r5
  000c99f0: str r4,[sp,#0x0]
  000c99f2: blx 0x00073e4c
  000c99f6: mov.w r0,#0x138
  000c99fa: blx 0x0006eb24
  000c99fe: ldr r1,[0x000c9d54]
  000c9a00: mov r2,r5
  000c9a02: movw r3,#0x7530
  000c9a06: str r4,[sp,#0x0]
  000c9a08: str r0,[sp,#0x30]
  000c9a0a: blx 0x00073e4c
  000c9a0e: ldr r4,[sp,#0x48]
  000c9a10: mov r1,r9
  000c9a12: movw r2,#0x4a6b
  000c9a16: movs r3,#0x1
  000c9a18: mov r0,r4
  000c9a1a: blx 0x00073f6c
  000c9a1e: ldr.w r1,[r4,#0xf8]
  000c9a22: add r6,sp,#0x180
  000c9a24: ldr r1,[r1,#0x4]
  000c9a26: str r0,[r1,#0x0]
  000c9a28: mov r1,r9
  000c9a2a: ldr.w r0,[r4,#0xf8]
  000c9a2e: ldr r0,[r0,#0x4]
  000c9a30: ldr r5,[r0,#0x0]
  000c9a32: ldr.w r0,[r9,#0x0]
  000c9a36: ldr r2,[r0,#0x28]
  000c9a38: mov r0,r6
  000c9a3a: blx r2
  000c9a3c: ldr r0,[r5,#0x0]
  000c9a3e: mov r1,r6
  000c9a40: ldr r2,[r0,#0x44]
  000c9a42: mov r0,r5
  000c9a44: blx r2
  000c9a46: ldr.w r0,[r4,#0xf8]
  000c9a4a: movs r1,#0x0
  000c9a4c: ldr r0,[r0,#0x4]
  000c9a4e: ldr r0,[r0,#0x0]
  000c9a50: blx 0x00073f54
  000c9a54: ldr.w r0,[r4,#0xf8]
  000c9a58: movs r1,#0x3
  000c9a5a: ldr r0,[r0,#0x4]
  000c9a5c: ldr r0,[r0,#0x0]
  000c9a5e: blx 0x00072748
  000c9a62: ldr.w r0,[r4,#0xf8]
  000c9a66: ldr r1,[0x000c9d58]
  000c9a68: ldr r0,[r0,#0x4]
  000c9a6a: mov r10,r1
  000c9a6c: ldr r0,[r0,#0x0]
  000c9a6e: ldr r0,[r0,#0x4]
  000c9a70: blx 0x000727e4
  000c9a74: ldr r0,[0x000c9d5c]
  000c9a76: ldr.w r1,[r4,#0xf8]
  000c9a7a: add r0,pc
  000c9a7c: ldr.w r11,[r0,#0x0]
  000c9a80: ldr r1,[r1,#0x4]
  000c9a82: ldr.w r0,[r11,#0x0]
  000c9a86: ldr.w r8,[r1,#0x0]
  000c9a8a: movw r1,#0x683
  000c9a8e: blx 0x00072f70
  000c9a92: ldr r1,[0x000c9d60]
  000c9a94: add r6,sp,#0x180
  000c9a96: mov r5,r0
  000c9a98: movs r2,#0x0
  000c9a9a: add r1,pc
  000c9a9c: mov r0,r6
  000c9a9e: blx 0x0006ee18
  000c9aa2: add r0,sp,#0x64
  000c9aa4: mov r1,r5
  000c9aa6: mov r2,r6
  000c9aa8: blx 0x0006ef98
  000c9aac: add r1,sp,#0x64
  000c9aae: mov r0,r8
  000c9ab0: blx 0x00073fa8
  000c9ab4: add r0,sp,#0x64
  000c9ab6: mov r8,r10
  000c9ab8: blx 0x0006ee30
  000c9abc: add r0,sp,#0x180
  000c9abe: blx 0x0006ee30
  000c9ac2: ldr r0,[sp,#0x48]
  000c9ac4: ldr.w r0,[r0,#0xf8]
  000c9ac8: ldr r0,[r0,#0x4]
  000c9aca: ldr r2,[r0,#0x0]
  000c9acc: ldr.w r0,[r2,#0x50]!
  000c9ad0: cbz r0,0x000c9ae8
  000c9ad2: blx 0x00070204
  000c9ad6: blx 0x0006eb48
  000c9ada: ldr r0,[sp,#0x48]
  000c9adc: ldr.w r0,[r0,#0xf8]
  000c9ae0: ldr r0,[r0,#0x4]
  000c9ae2: ldr r0,[r0,#0x0]
  000c9ae4: add.w r2,r0,#0x50
  000c9ae8: ldr r4,[sp,#0x30]
  000c9aea: movs r0,#0x0
  000c9aec: ldr r6,[sp,#0x48]
  000c9aee: movs r3,#0x1
  000c9af0: str r0,[r2,#0x0]
  000c9af2: movw r2,#0x4a6b
  000c9af6: mov r1,r4
  000c9af8: mov r0,r6
  000c9afa: blx 0x00073f6c
  000c9afe: ldr.w r1,[r6,#0xf8]
  000c9b02: add.w r10,sp,#0x180
  000c9b06: ldr r1,[r1,#0x4]
  000c9b08: str r0,[r1,#0x4]
  000c9b0a: mov r1,r4
  000c9b0c: ldr.w r0,[r6,#0xf8]
  000c9b10: ldr r0,[r0,#0x4]
  000c9b12: ldr r5,[r0,#0x4]
  000c9b14: ldr r0,[r4,#0x0]
  000c9b16: ldr r2,[r0,#0x28]
  000c9b18: mov r0,r10
  000c9b1a: blx r2
  000c9b1c: ldr r0,[r5,#0x0]
  000c9b1e: mov r1,r10
  000c9b20: ldr r2,[r0,#0x44]
  000c9b22: mov r0,r5
  000c9b24: blx r2
  000c9b26: ldr.w r0,[r6,#0xf8]
  000c9b2a: movs r1,#0x0
  000c9b2c: ldr r0,[r0,#0x4]
  000c9b2e: ldr r0,[r0,#0x4]
  000c9b30: blx 0x00073f54
  000c9b34: ldr.w r0,[r6,#0xf8]
  000c9b38: movs r1,#0x3
  000c9b3a: ldr r0,[r0,#0x4]
  000c9b3c: ldr r0,[r0,#0x4]
  000c9b3e: blx 0x00072748
  000c9b42: ldr.w r0,[r6,#0xf8]
  000c9b46: mov r1,r8
  000c9b48: ldr r0,[r0,#0x4]
  000c9b4a: ldr r0,[r0,#0x4]
  000c9b4c: ldr r0,[r0,#0x4]
  000c9b4e: blx 0x000727e4
  000c9b52: ldr.w r0,[r6,#0xf8]
  000c9b56: ldr r1,[r0,#0x4]
  000c9b58: ldr.w r0,[r11,#0x0]
  000c9b5c: ldr.w r8,[r1,#0x4]
  000c9b60: movw r1,#0x683
  000c9b64: blx 0x00072f70
  000c9b68: ldr r1,[0x000c9d64]
  000c9b6a: add r5,sp,#0x180
  000c9b6c: mov r6,r0
  000c9b6e: movs r2,#0x0
  000c9b70: add r1,pc
  000c9b72: mov r0,r5
  000c9b74: blx 0x0006ee18
  000c9b78: add r0,sp,#0x58
  000c9b7a: mov r1,r6
  000c9b7c: mov r2,r5
  000c9b7e: blx 0x0006ef98
  000c9b82: add r1,sp,#0x58
  000c9b84: mov r0,r8
  000c9b86: blx 0x00073fa8
  000c9b8a: add r0,sp,#0x58
  000c9b8c: str.w r9,[sp,#0x28]
  000c9b90: blx 0x0006ee30
  000c9b94: add r0,sp,#0x180
  000c9b96: blx 0x0006ee30
  000c9b9a: ldr r0,[sp,#0x48]
  000c9b9c: ldr.w r0,[r0,#0xf8]
  000c9ba0: ldr r0,[r0,#0x4]
  000c9ba2: ldr r2,[r0,#0x4]
  000c9ba4: ldr.w r0,[r2,#0x50]!
  000c9ba8: cbz r0,0x000c9bc0
  000c9baa: blx 0x00070204
  000c9bae: blx 0x0006eb48
  000c9bb2: ldr r0,[sp,#0x48]
  000c9bb4: ldr.w r0,[r0,#0xf8]
  000c9bb8: ldr r0,[r0,#0x4]
  000c9bba: ldr r0,[r0,#0x4]
  000c9bbc: add.w r2,r0,#0x50
  000c9bc0: adr r0,[0xc9d68]
  000c9bc2: vmov.i32 q9,#0x0
  000c9bc6: vld1.64 {d16,d17},[r0]
  000c9bca: add r0,sp,#0x180
  000c9bcc: movs r3,#0x0
  000c9bce: add.w r9,sp,#0xf4
  000c9bd2: mov r1,r0
  000c9bd4: orr r0,r0,#0x4
  000c9bd8: str r3,[r2,#0x0]
  000c9bda: movs r6,#0x2
  000c9bdc: vst1.32 {d18,d19},[r0]
  000c9be0: add.w r0,r1,#0x18
  000c9be4: movs r4,#0x0
  000c9be6: vst1.64 {d18,d19},[r0]
  000c9bea: add.w r0,r1,#0x28
  000c9bee: vst1.64 {d16,d17},[r0]
  000c9bf2: mov.w r0,#0x3f800000
  000c9bf6: str r0,[sp,#0x180]
  000c9bf8: str r0,[sp,#0x194]
  000c9bfa: str r0,[sp,#0x1b8]
  000c9bfc: strd r3,r3,[sp,#0x1c0]
  000c9c00: str r3,[sp,#0x1c8]
  000c9c02: ldr r0,[0x000c9d78]
  000c9c04: add r0,pc
  000c9c06: str r0,[sp,#0x2c]
  000c9c08: ldr.w r11,[sp,#0x2c]
  000c9c0c: b 0x000c9d22
  000c9c0e: ldr r5,[sp,#0x48]
  000c9c10: movs r1,#0x0
  000c9c12: movw r2,#0x1a76
  000c9c16: movs r3,#0x1
  000c9c18: mov r0,r5
  000c9c1a: blx 0x00073f6c
  000c9c1e: ldr.w r1,[r5,#0xf8]
  000c9c22: ldr r1,[r1,#0x4]
  000c9c24: str.w r0,[r1,r6,lsl #0x2]
  000c9c28: mov.w r1,#0x3e8
  000c9c2c: ldr.w r0,[r5,#0xf8]
  000c9c30: ldr r0,[r0,#0x4]
  000c9c32: ldr.w r8,[r0,r6,lsl #0x2]
  000c9c36: ldr.w r0,[r8,#0x4]
  000c9c3a: blx 0x00073f78
  000c9c3e: strd r4,r6,[sp,#0x38]
  000c9c42: add.w r6,r4,r4, lsl #0x1
  000c9c46: add r4,sp,#0xd0
  000c9c48: add r5,sp,#0x180
  000c9c4a: add.w r0,r11,r6, lsl #0x3
  000c9c4e: ldrd r10,r1,[r0,#0x4]
  000c9c52: str r1,[sp,#0x34]
  000c9c54: ldrd r1,r2,[r0,#0xc]
  000c9c58: ldr r0,[r0,#0x14]
  000c9c5a: strd r1,r2,[sp,#0xd0]
  000c9c5e: mov r1,r5
  000c9c60: str r0,[sp,#0xd8]
  000c9c62: mov r0,r9
  000c9c64: mov r2,r4
  000c9c66: blx 0x0006f694
  000c9c6a: add r5,sp,#0x1c0
  000c9c6c: mov r1,r9
  000c9c6e: mov r0,r5
  000c9c70: blx 0x0006eb3c
  000c9c74: ldr.w r0,[r8,#0x8]
  000c9c78: mov r1,r5
  000c9c7a: blx 0x00073dd4
  000c9c7e: ldr.w r0,[r11,r6,lsl #0x3]
  000c9c82: add r1,sp,#0x180
  000c9c84: ldr r6,[sp,#0x3c]
  000c9c86: strd r0,r10,[sp,#0xd0]
  000c9c8a: mov r2,r4
  000c9c8c: ldr r0,[sp,#0x34]
  000c9c8e: str r0,[sp,#0xd8]
  000c9c90: mov r0,r9
  000c9c92: blx 0x0006f694
  000c9c96: mov r0,r5
  000c9c98: mov r1,r9
  000c9c9a: blx 0x0006eb3c
  000c9c9e: cmp r6,#0x7
  000c9ca0: ite cc
  000c9ca2: ldr.cc r4,[sp,#0x28]
  000c9ca4: ldr.cs r4,[sp,#0x30]
  000c9ca6: ldr r0,[r4,#0x0]
  000c9ca8: mov r1,r4
  000c9caa: ldr r2,[r0,#0x28]
  000c9cac: add r0,sp,#0xd0
  000c9cae: blx r2
  000c9cb0: ldr r0,[r4,#0x0]
  000c9cb2: mov r1,r4
  000c9cb4: vldr.32 s16,[sp,#0x1c0]
  000c9cb8: vldr.32 s18,[sp,#0xd0]
  000c9cbc: ldr r2,[r0,#0x28]
  000c9cbe: add r0,sp,#0xa0
  000c9cc0: blx r2
  000c9cc2: ldr r0,[r4,#0x0]
  000c9cc4: mov r1,r4
  000c9cc6: vldr.32 s20,[sp,#0x1c4]
  000c9cca: vldr.32 s22,[sp,#0xa4]
  000c9cce: ldr r2,[r0,#0x28]
  000c9cd0: add r0,sp,#0x94
  000c9cd2: blx r2
  000c9cd4: vldr.32 s0,[sp,#0x1c8]
  000c9cd8: vadd.f32 s2,s18,s16
  000c9cdc: vldr.32 s4,[sp,#0x9c]
  000c9ce0: vadd.f32 s6,s22,s20
  000c9ce4: mov r1,r9
  000c9ce6: vadd.f32 s0,s4,s0
  000c9cea: vstr.32 s2,[sp,#0xf4]
  000c9cee: vstr.32 s6,[sp,#0xf8]
  000c9cf2: vstr.32 s0,[sp,#0xfc]
  000c9cf6: ldr.w r0,[r8,#0x0]
  000c9cfa: ldr r2,[r0,#0x44]
  000c9cfc: mov r0,r8
  000c9cfe: blx r2
  000c9d00: ldr r1,[0x000c9d7c]
  000c9d02: mov r0,r8
  000c9d04: blx 0x00073f84
  000c9d08: movs r0,#0x0
  000c9d0a: ldr r1,[sp,#0x38]
  000c9d0c: str.w r0,[r8,#0x50]
  000c9d10: movs r0,#0x1
  000c9d12: str.w r0,[r8,#0x28]
  000c9d16: adds r0,r1,#0x1
  000c9d18: cmp r0,#0x5
  000c9d1a: it eq
  000c9d1c: sub.eq r0,r1,#0x4
  000c9d1e: adds r6,#0x1
  000c9d20: mov r4,r0
  000c9d22: cmp r6,#0xc
  000c9d24: bne.w 0x000c9c0e
  000c9d28: ldr r0,[0x000c9d80]
  000c9d2a: movs r4,#0xc
  000c9d2c: ldr.w r11,[sp,#0x28]
  000c9d30: mov.w r8,#0x1
  000c9d34: add r0,pc
  000c9d36: mov.w r9,#0x0
  000c9d3a: ldr r6,[r0,#0x0]
  000c9d3c: b 0x000c9df2
  000c9d84: ldr r0,[r6,#0x0]
  000c9d86: movs r1,#0x1
  000c9d88: blx 0x00071a34
  000c9d8c: mov r3,r0
  000c9d8e: cmp r4,#0x10
  000c9d90: bhi 0x000c9dba
  000c9d92: strd r11,r8,[sp,#0x0]
  000c9d96: movs r1,#0x1
  000c9d98: ldr r5,[sp,#0x48]
  000c9d9a: movs r2,#0x0
  000c9d9c: str.w r9,[sp,#0x8]
  000c9da0: mov r0,r5
  000c9da2: blx 0x00073e88
  000c9da6: ldr.w r1,[r5,#0xf8]
  000c9daa: ldr r1,[r1,#0x4]
  000c9dac: str.w r0,[r1,r4,lsl #0x2]
  000c9db0: ldr.w r1,[r5,#0xf8]
  000c9db4: ldr.w r0,[r5,#0x110]
  000c9db8: b 0x000c9dde
  000c9dba: ldr r0,[sp,#0x30]
  000c9dbc: movs r1,#0x1
  000c9dbe: movs r2,#0x0
  000c9dc0: stm sp,{r0,r8,r9}
  000c9dc4: ldr r5,[sp,#0x48]
  000c9dc6: mov r0,r5
  000c9dc8: blx 0x00073e88
  000c9dcc: ldr.w r1,[r5,#0xf8]
  000c9dd0: ldr r1,[r1,#0x4]
  000c9dd2: str.w r0,[r1,r4,lsl #0x2]
  000c9dd6: ldr.w r1,[r5,#0xf8]
  000c9dda: ldr.w r0,[r5,#0x10c]
  000c9dde: ldr r1,[r1,#0x4]
  000c9de0: ldr.w r5,[r1,r4,lsl #0x2]
  000c9de4: blx 0x00073f3c
  000c9de8: mov r1,r0
  000c9dea: mov r0,r5
  000c9dec: blx 0x00073f60
  000c9df0: adds r4,#0x1
  000c9df2: cmp r4,#0x16
  000c9df4: bne 0x000c9d84
  000c9df6: b.w 0x000cb9bc
  000c9dfa: ldr r0,[0x000ca1e0]
  000c9dfc: add r0,pc
  000c9dfe: vldr.64 d16,[r0]
  000c9e02: ldr r0,[r0,#0x8]
  000c9e04: str r0,[sp,#0x188]
  000c9e06: movs r0,#0x18
  000c9e08: vstr.64 d16,[sp,#0x180]
  000c9e0c: blx 0x0006eb24
  000c9e10: mov r5,r0
  000c9e12: add r1,sp,#0x180
  000c9e14: movs r2,#0x3
  000c9e16: blx 0x00073eb8
  000c9e1a: ldr r0,[sp,#0x48]
  000c9e1c: str.w r5,[r0,#0x108]
  000c9e20: movs r0,#0xc
  000c9e22: blx 0x0006eb24
  000c9e26: mov r5,r0
  000c9e28: blx 0x00073d08
  000c9e2c: ldr r6,[sp,#0x48]
  000c9e2e: movs r0,#0x1
  000c9e30: mov r1,r5
  000c9e32: movs r4,#0x1
  000c9e34: str.w r5,[r6,#0xf8]
  000c9e38: blx 0x00073d14
  000c9e3c: mov.w r11,#0x0
  000c9e40: mov r0,r6
  000c9e42: movs r1,#0x3
  000c9e44: movs r2,#0x0
  000c9e46: movs r3,#0x1e
  000c9e48: strd r11,r4,[sp,#0x0]
  000c9e4c: str.w r11,[sp,#0x8]
  000c9e50: blx 0x00073e88
  000c9e54: ldr.w r1,[r6,#0xf8]
  000c9e58: ldr r1,[r1,#0x4]
  000c9e5a: str r0,[r1,#0x0]
  000c9e5c: movs r1,#0x1
  000c9e5e: ldr.w r0,[r6,#0xf8]
  000c9e62: ldr r0,[r0,#0x4]
  000c9e64: ldr r0,[r0,#0x0]
  000c9e66: ldr r0,[r0,#0x4]
  000c9e68: blx 0x00073fb4
  000c9e6c: ldr.w r1,[r6,#0xf8]
  000c9e70: ldr.w r0,[r6,#0x108]
  000c9e74: ldr r1,[r1,#0x4]
  000c9e76: ldr r4,[r1,#0x0]
  000c9e78: blx 0x00073f3c
  000c9e7c: mov r1,r0
  000c9e7e: mov r0,r4
  000c9e80: blx 0x00073f60
  000c9e84: ldr.w r0,[r6,#0xf8]
  000c9e88: ldr r1,[0x000ca1e4]
  000c9e8a: ldr r0,[r0,#0x4]
  000c9e8c: ldr r0,[r0,#0x0]
  000c9e8e: ldr r0,[r0,#0x4]
  000c9e90: blx 0x000727e4
  000c9e94: ldr r0,[0x000ca1e8]
  000c9e96: movw r1,#0x63f
  000c9e9a: add r0,pc
  000c9e9c: ldr r0,[r0,#0x0]
  000c9e9e: ldr r0,[r0,#0x0]
  000c9ea0: blx 0x00072f70
  000c9ea4: mov r1,r0
  000c9ea6: ldr.w r0,[r6,#0xf8]
  000c9eaa: ldr r0,[r0,#0x4]
  000c9eac: ldr r0,[r0,#0x0]
  000c9eae: adds r0,#0x18
  000c9eb0: blx 0x0006f2b0
  000c9eb4: ldr.w r0,[r6,#0xf8]
  000c9eb8: movs r1,#0x0
  000c9eba: ldr r0,[r0,#0x4]
  000c9ebc: ldr r0,[r0,#0x0]
  000c9ebe: blx 0x00074134
  000c9ec2: ldr.w r0,[r6,#0xf8]
  000c9ec6: add.w r8,sp,#0x1c0
  000c9eca: ldr.w r1,[r6,#0xf0]
  000c9ece: ldr r0,[r0,#0x4]
  000c9ed0: ldr.w r10,[r0,#0x0]
  000c9ed4: mov r0,r8
  000c9ed6: blx 0x0007264c
  000c9eda: ldr.w r0,[r6,#0xf0]
  000c9ede: add r5,sp,#0xd0
  000c9ee0: ldr r1,[r0,#0x8]
  000c9ee2: mov r0,r5
  000c9ee4: blx 0x0007261c
  000c9ee8: add r4,sp,#0xf4
  000c9eea: ldr r2,[0x000ca1ec]
  000c9eec: mov r1,r5
  000c9eee: mov r0,r4
  000c9ef0: blx 0x0006ec74
  000c9ef4: add.w r9,sp,#0x118
  000c9ef8: mov r1,r8
  000c9efa: mov r2,r4
  000c9efc: mov r0,r9
  000c9efe: blx 0x0006f1cc
  000c9f02: ldr.w r0,[r6,#0xf0]
  000c9f06: add r4,sp,#0x94
  000c9f08: ldr r1,[r0,#0x8]
  000c9f0a: mov r0,r4
  000c9f0c: blx 0x00072574
  000c9f10: add r5,sp,#0xa0
  000c9f12: ldr r2,[0x000ca1f0]
  000c9f14: mov r1,r4
  000c9f16: mov r0,r5
  000c9f18: blx 0x0006ec74
  000c9f1c: add r4,sp,#0x140
  000c9f1e: mov r1,r9
  000c9f20: mov r2,r5
  000c9f22: mov r0,r4
  000c9f24: blx 0x0006f1cc
  000c9f28: ldr.w r0,[r10,#0x0]
  000c9f2c: mov r1,r4
  000c9f2e: ldr r2,[r0,#0x44]
  000c9f30: mov r0,r10
  000c9f32: blx r2
  000c9f34: ldr.w r0,[r6,#0xf8]
  000c9f38: add r5,sp,#0x140
  000c9f3a: ldr.w r1,[r6,#0xf0]
  000c9f3e: ldr r0,[r0,#0x4]
  000c9f40: ldr r0,[r0,#0x0]
  000c9f42: ldr r4,[r0,#0x8]
  000c9f44: mov r0,r5
  000c9f46: blx 0x00073540
  000c9f4a: mov.w r0,#0x3f800000
  000c9f4e: add r2,sp,#0x118
  000c9f50: strd r11,r0,[sp,#0x118]
  000c9f54: mov r0,r4
  000c9f56: mov r1,r5
  000c9f58: str.w r11,[sp,#0x120]
  000c9f5c: blx 0x000726ac
  000c9f60: movs r0,#0x1c
  000c9f62: blx 0x0006eb24
  000c9f66: mov r5,r0
  000c9f68: movs r1,#0x4
  000c9f6a: movs r2,#0x4
  000c9f6c: mov r3,r6
  000c9f6e: blx 0x00073edc
  000c9f72: b.w 0x000cb9b8
  000c9f76: ldr r0,[0x000ca1f4]
  000c9f78: add r4,sp,#0x140
  000c9f7a: add r0,pc
  000c9f7c: vld1.64 {d16,d17},[r0]!
  000c9f80: vldr.64 d18,[r0]
  000c9f84: mov r0,r4
  000c9f86: vst1.64 {d16,d17},[r0]!
  000c9f8a: vstr.64 d18,[r0]
  000c9f8e: movs r0,#0x18
  000c9f90: blx 0x0006eb24
  000c9f94: mov r5,r0
  000c9f96: mov r1,r4
  000c9f98: movs r2,#0x6
  000c9f9a: blx 0x00073eb8
  000c9f9e: ldr r0,[sp,#0x48]
  000c9fa0: movs r1,#0x0
  000c9fa2: mov.w r11,#0x0
  000c9fa6: str.w r5,[r0,#0x110]
  000c9faa: mov r0,r5
  000c9fac: blx 0x00073ec4
  000c9fb0: blx 0x00074194
  000c9fb4: movs r0,#0xc
  000c9fb6: blx 0x0006eb24
  000c9fba: mov r5,r0
  000c9fbc: blx 0x00073d08
  000c9fc0: ldr r6,[sp,#0x48]
  000c9fc2: movs r0,#0xd
  000c9fc4: mov r1,r5
  000c9fc6: str.w r5,[r6,#0xf8]
  000c9fca: blx 0x00073d14
  000c9fce: ldr r0,[0x000ca1f8]
  000c9fd0: movs r1,#0x1
  000c9fd2: mov.w r9,#0x1
  000c9fd6: add r0,pc
  000c9fd8: ldr r5,[r0,#0x0]
  000c9fda: ldr.w r0,[r6,#0x110]
  000c9fde: blx r5
  000c9fe0: mov r1,r0
  000c9fe2: ldr r0,[r0,#0x0]
  000c9fe4: add.w r10,sp,#0x180
  000c9fe8: ldr r2,[r0,#0x28]
  000c9fea: mov r0,r10
  000c9fec: blx r2
  000c9fee: ldr.w r0,[r6,#0x110]
  000c9ff2: movs r1,#0x0
  000c9ff4: blx r5
  000c9ff6: mov r1,r0
  000c9ff8: ldr r0,[r0,#0x0]
  000c9ffa: add r4,sp,#0x1c0
  000c9ffc: ldr r2,[r0,#0x28]
  000c9ffe: mov r0,r4
  000ca000: blx r2
  000ca002: add.w r8,sp,#0x118
  000ca006: mov r1,r10
  000ca008: mov r2,r4
  000ca00a: mov r0,r8
  000ca00c: blx 0x0006ec38
  000ca010: add r4,sp,#0x180
  000ca012: mov r1,r8
  000ca014: str.w r11,[sp,#0x11c]
  000ca018: mov r0,r4
  000ca01a: blx 0x0006ec80
  000ca01e: mov r0,r8
  000ca020: mov r1,r4
  000ca022: blx 0x0006eb3c
  000ca026: ldr.w r0,[r6,#0x110]
  000ca02a: movs r1,#0x0
  000ca02c: blx r5
  000ca02e: mov r1,r0
  000ca030: ldr r0,[r0,#0x0]
  000ca032: ldr r2,[r0,#0x28]
  000ca034: add r0,sp,#0x1c0
  000ca036: blx r2
  000ca038: mov r0,r6
  000ca03a: movs r1,#0x2
  000ca03c: movs r2,#0x0
  000ca03e: movs r3,#0x31
  000ca040: strd r11,r9,[sp,#0x0]
  000ca044: str.w r11,[sp,#0x8]
  000ca048: blx 0x00073e88
  000ca04c: ldr.w r1,[r6,#0xf8]
  000ca050: ldr r1,[r1,#0x4]
  000ca052: str r0,[r1,#0x0]
  000ca054: movs r1,#0x0
  000ca056: ldr.w r0,[r6,#0xf8]
  000ca05a: ldr r0,[r0,#0x4]
  000ca05c: ldr r4,[r0,#0x0]
  000ca05e: ldr.w r0,[r6,#0x110]
  000ca062: blx r5
  000ca064: mov r1,r0
  000ca066: ldr r0,[r0,#0x0]
  000ca068: add r5,sp,#0x180
  000ca06a: ldr r2,[r0,#0x28]
  000ca06c: mov r0,r5
  000ca06e: blx r2
  000ca070: ldr r0,[r4,#0x0]
  000ca072: mov r1,r5
  000ca074: ldr r2,[r0,#0x44]
  000ca076: mov r0,r4
  000ca078: blx r2
  000ca07a: ldr.w r0,[r6,#0xf8]
  000ca07e: movs r1,#0x1
  000ca080: ldr r0,[r0,#0x4]
  000ca082: ldr r0,[r0,#0x0]
  000ca084: ldr r0,[r0,#0x4]
  000ca086: blx 0x00073e94
  000ca08a: ldr.w r1,[r6,#0xf8]
  000ca08e: ldr.w r0,[r6,#0x110]
  000ca092: ldr r1,[r1,#0x4]
  000ca094: ldr r4,[r1,#0x0]
  000ca096: blx 0x000741a0
  000ca09a: mov r1,r0
  000ca09c: mov r0,r4
  000ca09e: blx 0x00073f60
  000ca0a2: ldr r0,[0x000ca1fc]
  000ca0a4: movw r1,#0x664
  000ca0a8: add r0,pc
  000ca0aa: ldr r0,[r0,#0x0]
  000ca0ac: ldr r0,[r0,#0x0]
  000ca0ae: blx 0x00072f70
  000ca0b2: mov r1,r0
  000ca0b4: ldr.w r0,[r6,#0xf8]
  000ca0b8: ldr r0,[r0,#0x4]
  000ca0ba: ldr r0,[r0,#0x0]
  000ca0bc: adds r0,#0x18
  000ca0be: blx 0x0006f2b0
  000ca0c2: ldr.w r0,[r6,#0xf8]
  000ca0c6: mov.w r1,#0x3f800000
  000ca0ca: add r2,sp,#0x180
  000ca0cc: ldr r0,[r0,#0x4]
  000ca0ce: ldr r0,[r0,#0x0]
  000ca0d0: ldr r0,[r0,#0x8]
  000ca0d2: strd r11,r1,[sp,#0x180]
  000ca0d6: mov r1,r8
  000ca0d8: str.w r11,[sp,#0x188]
  000ca0dc: blx 0x000726ac
  000ca0e0: ldr.w r0,[r6,#0xf8]
  000ca0e4: movs r1,#0x1
  000ca0e6: ldr r0,[r0,#0x4]
  000ca0e8: ldr r0,[r0,#0x0]
  000ca0ea: blx 0x00074158
  000ca0ee: ldr.w r0,[r6,#0xf8]
  000ca0f2: movs r1,#0x0
  000ca0f4: ldr r0,[r0,#0x4]
  000ca0f6: ldr r0,[r0,#0x0]
  000ca0f8: blx 0x00074164
  000ca0fc: ldr.w r0,[r6,#0xf8]
  000ca100: ldr r0,[r0,#0x4]
  000ca102: ldr r0,[r0,#0x0]
  000ca104: ldr r0,[r0,#0x8]
  000ca106: blx 0x000721cc
  000ca10a: mov r1,r0
  000ca10c: add r0,sp,#0x180
  000ca10e: movs r2,#0x3c
  000ca110: blx 0x0006f1e4
  000ca114: add.w r10,r6,#0x18c
  000ca118: add r1,sp,#0x1c0
  000ca11a: mov r0,r10
  000ca11c: blx 0x0006eb3c
  000ca120: ldr r0,[0x000ca200]
  000ca122: add.w r8,sp,#0xf4
  000ca126: ldr r6,[0x000ca204]
  000ca128: mov.w r9,#0x1
  000ca12c: ldr r1,[0x000ca208]
  000ca12e: add.w r0,r0,#0x1f4
  000ca132: str r0,[sp,#0x38]
  000ca134: mov.w r0,#0x1f40
  000ca138: str r0,[sp,#0x3c]
  000ca13a: cmp.w r9,#0xd
  000ca13e: beq.w 0x000cb9bc
  000ca142: movs r4,#0x0
  000ca144: movs r0,#0x1
  000ca146: str r4,[sp,#0x0]
  000ca148: mov r11,r1
  000ca14a: strd r0,r4,[sp,#0x4]
  000ca14e: movs r1,#0xa
  000ca150: ldr r5,[sp,#0x48]
  000ca152: movs r2,#0x0
  000ca154: movs r3,#0x2c
  000ca156: mov r0,r5
  000ca158: blx 0x00073e88
  000ca15c: ldr.w r1,[r5,#0xf8]
  000ca160: ldr r1,[r1,#0x4]
  000ca162: str.w r0,[r1,r9,lsl #0x2]
  000ca166: movs r1,#0x1
  000ca168: ldr.w r0,[r5,#0xf8]
  000ca16c: ldr r0,[r0,#0x4]
  000ca16e: ldr.w r0,[r0,r9,lsl #0x2]
  000ca172: ldr r0,[r0,#0x4]
  000ca174: blx 0x00073e94
  000ca178: ldr.w r0,[r5,#0xf8]
  000ca17c: mov.w r1,#0x3f800000
  000ca180: mov r2,r8
  000ca182: ldr r0,[r0,#0x4]
  000ca184: ldr.w r0,[r0,r9,lsl #0x2]
  000ca188: ldr r0,[r0,#0x8]
  000ca18a: strd r4,r1,[sp,#0xf4]
  000ca18e: add r1,sp,#0x118
  000ca190: str r4,[sp,#0xfc]
  000ca192: blx 0x000726ac
  000ca196: ldr.w r0,[r5,#0xf8]
  000ca19a: movs r1,#0x1
  000ca19c: ldr r0,[r0,#0x4]
  000ca19e: ldr.w r0,[r0,r9,lsl #0x2]
  000ca1a2: blx 0x00074158
  000ca1a6: ldr.w r0,[r5,#0xf8]
  000ca1aa: movs r1,#0x0
  000ca1ac: ldr r0,[r0,#0x4]
  000ca1ae: ldr.w r0,[r0,r9,lsl #0x2]
  000ca1b2: blx 0x00074164
  000ca1b6: ldr.w r1,[r5,#0xf8]
  000ca1ba: ldr.w r0,[r5,#0x110]
  000ca1be: ldr r1,[r1,#0x4]
  000ca1c0: ldr.w r4,[r1,r9,lsl #0x2]
  000ca1c4: blx 0x000741a0
  000ca1c8: mov r1,r0
  000ca1ca: mov r0,r4
  000ca1cc: blx 0x00073f60
  000ca1d0: cmp.w r9,#0x2
  000ca1d4: str r6,[sp,#0x34]
  000ca1d6: bhi 0x000ca20c
  000ca1d8: vmov s0,r11
  000ca1dc: b 0x000ca218
  000ca20c: cmp.w r9,#0x4
  000ca210: bhi 0x000ca2da
  000ca212: ldr r0,[sp,#0x3c]
  000ca214: vmov s0,r0
  000ca218: ldr r0,[0x000ca450]
  000ca21a: vcvt.f32.s32 s0,s0
  000ca21e: str r0,[sp,#0xf8]
  000ca220: ldr r0,[0x000ca454]
  000ca222: str r0,[sp,#0xfc]
  000ca224: mov r0,r10
  000ca226: mov r1,r8
  000ca228: vstr.32 s0,[sp,#0xf4]
  000ca22c: blx 0x0006eb3c
  000ca230: ldr r0,[0x000ca458]
  000ca232: ldr r1,[0x000ca45c]
  000ca234: add r0,pc
  000ca236: add r1,pc
  000ca238: ldr r4,[r0,#0x0]
  000ca23a: ldr r6,[r1,#0x0]
  000ca23c: movs r1,#0x3c
  000ca23e: ldr r0,[r4,#0x0]
  000ca240: blx r6
  000ca242: subs r0,#0x1e
  000ca244: movs r1,#0x3c
  000ca246: vmov s0,r0
  000ca24a: vcvt.f32.s32 s0,s0
  000ca24e: ldr r5,[sp,#0x48]
  000ca250: vldr.32 s2,[r5,#0x18c]
  000ca254: vadd.f32 s0,s2,s0
  000ca258: vstr.32 s0,[r5,#0x18c]
  000ca25c: ldr r0,[r4,#0x0]
  000ca25e: blx r6
  000ca260: subs r0,#0x1e
  000ca262: movs r1,#0x3c
  000ca264: vmov s0,r0
  000ca268: vcvt.f32.s32 s0,s0
  000ca26c: vldr.32 s2,[r5,#0x190]
  000ca270: vadd.f32 s0,s2,s0
  000ca274: vstr.32 s0,[r5,#0x190]
  000ca278: ldr r0,[r4,#0x0]
  000ca27a: blx r6
  000ca27c: subs r0,#0x1e
  000ca27e: add r1,sp,#0x180
  000ca280: mov r2,r10
  000ca282: vmov s0,r0
  000ca286: mov r0,r8
  000ca288: vcvt.f32.s32 s0,s0
  000ca28c: vldr.32 s2,[r5,#0x194]
  000ca290: vadd.f32 s0,s2,s0
  000ca294: vstr.32 s0,[r5,#0x194]
  000ca298: blx 0x0006f688
  000ca29c: mov r0,r10
  000ca29e: mov r1,r8
  000ca2a0: blx 0x0006eb3c
  000ca2a4: ldr.w r0,[r5,#0xf8]
  000ca2a8: ldr r0,[r0,#0x4]
  000ca2aa: ldr.w r0,[r0,r9,lsl #0x2]
  000ca2ae: ldr r1,[r0,#0x0]
  000ca2b0: ldr r2,[r1,#0x44]
  000ca2b2: mov r1,r10
  000ca2b4: blx r2
  000ca2b6: ldr r6,[sp,#0x34]
  000ca2b8: movw r0,#0x1964
  000ca2bc: addw r11,r11,#0x5dc
  000ca2c0: add.w r9,r9,#0x1
  000ca2c4: add r6,r0
  000ca2c6: ldr r0,[sp,#0x38]
  000ca2c8: mov r1,r11
  000ca2ca: addw r0,r0,#0xcb2
  000ca2ce: str r0,[sp,#0x38]
  000ca2d0: ldr r0,[sp,#0x3c]
  000ca2d2: subw r0,r0,#0x5dc
  000ca2d6: str r0,[sp,#0x3c]
  000ca2d8: b 0x000ca13a
  000ca2da: cmp.w r9,#0x9
  000ca2de: bhi 0x000ca302
  000ca2e0: ldr r0,[sp,#0x38]
  000ca2e2: cmp.w r9,#0x7
  000ca2e6: vmov s0,r0
  000ca2ea: adr r0,[0xca460]
  000ca2ec: vcvt.f32.s32 s0,s0
  000ca2f0: it eq
  000ca2f2: add.eq r0,#0x4
  000ca2f4: vldr.32 s2,[r0]
  000ca2f8: ldr r0,[0x000ca468]
  000ca2fa: str r0,[sp,#0xfc]
  000ca2fc: vstr.32 s2,[sp,#0xf8]
  000ca300: b 0x000ca224
  000ca302: vmov s0,r6
  000ca306: ldr r0,[0x000ca46c]
  000ca308: vcvt.f32.s32 s0,s0
  000ca30c: str r0,[sp,#0xf8]
  000ca30e: ldr r0,[0x000ca468]
  000ca310: b 0x000ca222
  000ca312: ldr r0,[0x000ca470]
  000ca314: add r0,pc
  000ca316: vldr.64 d16,[r0]
  000ca31a: ldr r0,[r0,#0x8]
  000ca31c: str r0,[sp,#0x188]
  000ca31e: movs r0,#0x18
  000ca320: vstr.64 d16,[sp,#0x180]
  000ca324: blx 0x0006eb24
  000ca328: mov r5,r0
  000ca32a: add r1,sp,#0x180
  000ca32c: movs r2,#0x3
  000ca32e: blx 0x00073eb8
  000ca332: ldr r0,[sp,#0x48]
  000ca334: str.w r5,[r0,#0x110]
  000ca338: movs r0,#0xc
  000ca33a: blx 0x0006eb24
  000ca33e: mov r5,r0
  000ca340: blx 0x00073d08
  000ca344: ldr r6,[sp,#0x48]
  000ca346: movs r0,#0xe
  000ca348: mov r1,r5
  000ca34a: str.w r5,[r6,#0xf8]
  000ca34e: blx 0x00073d14
  000ca352: mov.w r8,#0x0
  000ca356: movs r0,#0x1
  000ca358: strd r8,r0,[sp,#0x0]
  000ca35c: mov r0,r6
  000ca35e: movs r1,#0x2
  000ca360: movs r2,#0x0
  000ca362: movs r3,#0x31
  000ca364: str.w r8,[sp,#0x8]
  000ca368: blx 0x00073e88
  000ca36c: ldr.w r1,[r6,#0xf8]
  000ca370: add.w r5,r6,#0x18c
  000ca374: ldr r2,[0x000ca474]
  000ca376: ldr r3,[0x000ca478]
  000ca378: ldr r1,[r1,#0x4]
  000ca37a: str r0,[r1,#0x0]
  000ca37c: add r1,sp,#0x140
  000ca37e: mov r0,r5
  000ca380: strd r2,r8,[sp,#0x140]
  000ca384: str r3,[sp,#0x148]
  000ca386: blx 0x0006eb3c
  000ca38a: ldr.w r0,[r6,#0xf8]
  000ca38e: ldr r0,[r0,#0x4]
  000ca390: ldr r0,[r0,#0x0]
  000ca392: ldr r1,[r0,#0x0]
  000ca394: ldr r2,[r1,#0x44]
  000ca396: mov r1,r5
  000ca398: blx r2
  000ca39a: ldr.w r0,[r6,#0xf8]
  000ca39e: movs r1,#0x1
  000ca3a0: ldr r0,[r0,#0x4]
  000ca3a2: ldr r0,[r0,#0x0]
  000ca3a4: ldr r0,[r0,#0x4]
  000ca3a6: blx 0x00073e94
  000ca3aa: ldr r0,[0x000ca47c]
  000ca3ac: movw r1,#0x664
  000ca3b0: add r0,pc
  000ca3b2: ldr r0,[r0,#0x0]
  000ca3b4: str r0,[sp,#0x38]
  000ca3b6: ldr r0,[r0,#0x0]
  000ca3b8: blx 0x00072f70
  000ca3bc: mov r1,r0
  000ca3be: ldr.w r0,[r6,#0xf8]
  000ca3c2: ldr r0,[r0,#0x4]
  000ca3c4: ldr r0,[r0,#0x0]
  000ca3c6: adds r0,#0x18
  000ca3c8: blx 0x0006f2b0
  000ca3cc: ldr.w r1,[r6,#0xf8]
  000ca3d0: ldr.w r0,[r6,#0x110]
  000ca3d4: ldr r1,[r1,#0x4]
  000ca3d6: ldr r5,[r1,#0x0]
  000ca3d8: blx 0x00073f3c
  000ca3dc: mov r1,r0
  000ca3de: mov r0,r5
  000ca3e0: blx 0x00073f60
  000ca3e4: ldr.w r0,[r6,#0xf8]
  000ca3e8: add r5,sp,#0xf4
  000ca3ea: ldr r2,[0x000ca480]
  000ca3ec: ldr r0,[r0,#0x4]
  000ca3ee: ldr r1,[r0,#0x0]
  000ca3f0: ldr r0,[0x000ca484]
  000ca3f2: ldr.w r9,[r1,#0x8]
  000ca3f6: strb.w r8,[r1,#0x13d]
  000ca3fa: strd r0,r8,[sp,#0x1c0]
  000ca3fe: str r2,[sp,#0x1c8]
  000ca400: ldr r0,[r1,#0x0]
  000ca402: ldr r2,[r0,#0x28]
  000ca404: mov r0,r5
  000ca406: blx r2
  000ca408: add r4,sp,#0x118
  000ca40a: add r1,sp,#0x1c0
  000ca40c: mov r2,r5
  000ca40e: mov r0,r4
  000ca410: blx 0x0006ec38
  000ca414: add r5,sp,#0x140
  000ca416: mov r1,r4
  000ca418: mov r0,r5
  000ca41a: blx 0x0006ec80
  000ca41e: mov.w r0,#0x3f800000
  000ca422: add r2,sp,#0xd0
  000ca424: strd r8,r0,[sp,#0xd0]
  000ca428: mov r0,r9
  000ca42a: mov r1,r5
  000ca42c: str.w r8,[sp,#0xd8]
  000ca430: blx 0x000726ac
  000ca434: ldr.w r0,[r6,#0xf8]
  000ca438: movs r1,#0x1
  000ca43a: ldr r0,[r0,#0x4]
  000ca43c: ldr r0,[r0,#0x0]
  000ca43e: blx 0x00074158
  000ca442: ldr r0,[0x000ca488]
  000ca444: movw r11,#0x59d8
  000ca448: movs r5,#0x1
  000ca44a: add.w r9,r0,#0x7d0
  000ca44e: b 0x000ca5d8
  000ca48c: ldr r4,[sp,#0x48]
  000ca48e: movs r1,#0x0
  000ca490: ldr.w r0,[r4,#0x110]
  000ca494: blx 0x00073ec4
  000ca498: str r0,[sp,#0x0]
  000ca49a: movs r0,#0x1
  000ca49c: str r0,[sp,#0x4]
  000ca49e: movs r0,#0x0
  000ca4a0: str r0,[sp,#0x8]
  000ca4a2: mov r0,r4
  000ca4a4: movs r1,#0xa
  000ca4a6: movs r2,#0x0
  000ca4a8: movs r3,#0x2c
  000ca4aa: blx 0x00073e88
  000ca4ae: ldr.w r1,[r4,#0xf8]
  000ca4b2: ldr r1,[r1,#0x4]
  000ca4b4: str.w r0,[r1,r5,lsl #0x2]
  000ca4b8: ldr.w r0,[r4,#0xf8]
  000ca4bc: ldr r1,[0x000ca6c8]
  000ca4be: ldr r2,[0x000ca6cc]
  000ca4c0: ldr r0,[r0,#0x4]
  000ca4c2: add r1,pc
  000ca4c4: add r2,pc
  000ca4c6: ldr r4,[r1,#0x0]
  000ca4c8: mov.w r1,#0x3e8
  000ca4cc: ldr.w r0,[r0,r5,lsl #0x2]
  000ca4d0: ldr.w r10,[r2,#0x0]
  000ca4d4: str r0,[sp,#0x3c]
  000ca4d6: ldr r0,[r4,#0x0]
  000ca4d8: blx r10
  000ca4da: mov r8,r11
  000ca4dc: mov r11,r9
  000ca4de: mov r9,r0
  000ca4e0: ldr r0,[r4,#0x0]
  000ca4e2: mov.w r1,#0x3e8
  000ca4e6: blx r10
  000ca4e8: sub.w r0,r0,#0x1f4
  000ca4ec: movw r1,#0x9c4
  000ca4f0: mla r6,r0,r5,r9
  000ca4f4: ldr r0,[r4,#0x0]
  000ca4f6: mov r9,r11
  000ca4f8: mov r11,r8
  000ca4fa: blx r10
  000ca4fc: add.w r2,r0,r9
  000ca500: ldr r0,[sp,#0x3c]
  000ca502: vmov s0,r6
  000ca506: ldr r4,[sp,#0x48]
  000ca508: vmov s2,r2
  000ca50c: ldr r1,[r0,#0x0]
  000ca50e: vmov s4,r11
  000ca512: ldr r6,[r1,#0x48]
  000ca514: vcvt.f32.s32 s4,s4
  000ca518: vcvt.f32.s32 s0,s0
  000ca51c: vcvt.f32.s32 s2,s2
  000ca520: vmov r1,s4
  000ca524: vmov r2,s0
  000ca528: vmov r3,s2
  000ca52c: blx r6
  000ca52e: ldr.w r1,[r4,#0xf8]
  000ca532: ldr.w r0,[r4,#0x110]
  000ca536: ldr r1,[r1,#0x4]
  000ca538: ldr.w r6,[r1,r5,lsl #0x2]
  000ca53c: blx 0x00073f3c
  000ca540: mov r1,r0
  000ca542: mov r0,r6
  000ca544: blx 0x00073f60
  000ca548: ldr.w r0,[r4,#0xf8]
  000ca54c: movs r1,#0x1
  000ca54e: ldr r0,[r0,#0x4]
  000ca550: ldr.w r0,[r0,r5,lsl #0x2]
  000ca554: ldr r0,[r0,#0x4]
  000ca556: blx 0x00073e94
  000ca55a: ldr.w r0,[r4,#0xf8]
  000ca55e: movs r1,#0x1
  000ca560: ldr r0,[r0,#0x4]
  000ca562: ldr.w r0,[r0,r5,lsl #0x2]
  000ca566: blx 0x00074158
  000ca56a: ldr.w r0,[r4,#0xf8]
  000ca56e: movs r6,#0x0
  000ca570: movs r1,#0x0
  000ca572: ldr r0,[r0,#0x4]
  000ca574: ldr.w r0,[r0,r5,lsl #0x2]
  000ca578: strb.w r6,[r0,#0x13d]
  000ca57c: blx 0x00074164
  000ca580: ldr.w r0,[r4,#0xf8]
  000ca584: add r4,sp,#0xf4
  000ca586: ldr r0,[r0,#0x4]
  000ca588: ldr.w r1,[r0,r5,lsl #0x2]
  000ca58c: ldr r0,[0x000ca6d0]
  000ca58e: ldr.w r8,[r1,#0x8]
  000ca592: str r6,[sp,#0x1c4]
  000ca594: str r0,[sp,#0x1c0]
  000ca596: ldr r0,[0x000ca6d4]
  000ca598: str r0,[sp,#0x1c8]
  000ca59a: ldr r0,[r1,#0x0]
  000ca59c: ldr r2,[r0,#0x28]
  000ca59e: mov r0,r4
  000ca5a0: blx r2
  000ca5a2: add r6,sp,#0x118
  000ca5a4: add r1,sp,#0x1c0
  000ca5a6: mov r2,r4
  000ca5a8: mov r0,r6
  000ca5aa: blx 0x0006ec38
  000ca5ae: add r4,sp,#0x140
  000ca5b0: mov r1,r6
  000ca5b2: mov r0,r4
  000ca5b4: blx 0x0006ec80
  000ca5b8: mov.w r0,#0x3f800000
  000ca5bc: add r2,sp,#0xd0
  000ca5be: str r0,[sp,#0xd4]
  000ca5c0: movs r0,#0x0
  000ca5c2: str r0,[sp,#0xd0]
  000ca5c4: mov r1,r4
  000ca5c6: str r0,[sp,#0xd8]
  000ca5c8: mov r0,r8
  000ca5ca: blx 0x000726ac
  000ca5ce: addw r11,r11,#0xbb8
  000ca5d2: add.w r9,r9,#0x7d0
  000ca5d6: adds r5,#0x1
  000ca5d8: cmp r5,#0xd
  000ca5da: bne.w 0x000ca48c
  000ca5de: ldr r4,[sp,#0x48]
  000ca5e0: movs r1,#0x0
  000ca5e2: movw r2,#0x4a6a
  000ca5e6: movs r3,#0x1
  000ca5e8: movs r5,#0x1
  000ca5ea: mov r0,r4
  000ca5ec: blx 0x00073f6c
  000ca5f0: ldr.w r1,[r4,#0xf8]
  000ca5f4: ldr r3,[0x000ca6d4]
  000ca5f6: ldrd r2,r1,[r1,#0x0]
  000ca5fa: add.w r1,r1,r2, lsl #0x2
  000ca5fe: str.w r0,[r1,#-0x4]
  000ca602: ldr.w r0,[r4,#0xf8]
  000ca606: ldrd r1,r0,[r0,#0x0]
  000ca60a: add.w r0,r0,r1, lsl #0x2
  000ca60e: ldr r1,[0x000ca6d0]
  000ca610: ldr.w r0,[r0,#-0x4]
  000ca614: ldr r2,[r0,#0x0]
  000ca616: ldr r6,[r2,#0x48]
  000ca618: movs r2,#0x0
  000ca61a: blx r6
  000ca61c: ldr.w r0,[r4,#0xf8]
  000ca620: ldrd r1,r0,[r0,#0x0]
  000ca624: add.w r0,r0,r1, lsl #0x2
  000ca628: movs r1,#0x0
  000ca62a: ldr.w r0,[r0,#-0x4]
  000ca62e: blx 0x000730b4
  000ca632: ldr.w r0,[r4,#0xf8]
  000ca636: ldrd r1,r0,[r0,#0x0]
  000ca63a: add.w r0,r0,r1, lsl #0x2
  000ca63e: movs r1,#0x0
  000ca640: ldr.w r0,[r0,#-0x4]
  000ca644: blx 0x000732f4
  000ca648: ldr r0,[sp,#0x38]
  000ca64a: movw r1,#0xc87
  000ca64e: ldr r0,[r0,#0x0]
  000ca650: blx 0x00072f70
  000ca654: mov r1,r0
  000ca656: ldr.w r0,[r4,#0xf8]
  000ca65a: ldrd r2,r0,[r0,#0x0]
  000ca65e: add.w r0,r0,r2, lsl #0x2
  000ca662: ldr.w r0,[r0,#-0x4]
  000ca666: adds r0,#0x18
  000ca668: blx 0x0006f2b0
  000ca66c: ldr.w r0,[r4,#0xf8]
  000ca670: movs r3,#0x3
  000ca672: ldrd r1,r0,[r0,#0x0]
  000ca676: add.w r0,r0,r1, lsl #0x2
  000ca67a: ldr r1,[0x000ca6d8]
  000ca67c: ldr.w r2,[r0,#-0x4]
  000ca680: ldr r0,[r2,#0x4]
  000ca682: str r3,[r2,#0x28]
  000ca684: blx 0x000727e4
  000ca688: ldr.w r0,[r4,#0xf8]
  000ca68c: ldrd r1,r0,[r0,#0x0]
  000ca690: add.w r0,r0,r1, lsl #0x2
  000ca694: ldr.w r0,[r0,#-0x4]
  000ca698: strb.w r5,[r0,#0x74]
  000ca69c: movs r0,#0x1c
  000ca69e: blx 0x0006eb24
  000ca6a2: mov r5,r0
  000ca6a4: movs r1,#0x4
  000ca6a6: movs r2,#0x2
  000ca6a8: mov r3,r4
  000ca6aa: blx 0x00073edc
  000ca6ae: b.w 0x000cb9b8
  000ca6b2: movs r0,#0x1c
  000ca6b4: blx 0x0006eb24
  000ca6b8: mov r5,r0
  000ca6ba: ldr r3,[sp,#0x48]
  000ca6bc: movs r1,#0x4
  000ca6be: movs r2,#0x1
  000ca6c0: blx 0x00073edc
  000ca6c4: b.w 0x000cb9b8
  000ca6dc: ldr r0,[0x000caa9c]
  000ca6de: add r0,pc
  000ca6e0: vldr.64 d16,[r0]
  000ca6e4: ldr r0,[r0,#0x8]
  000ca6e6: str r0,[sp,#0x148]
  000ca6e8: movs r0,#0x18
  000ca6ea: vstr.64 d16,[sp,#0x140]
  000ca6ee: blx 0x0006eb24
  000ca6f2: mov r5,r0
  000ca6f4: add r1,sp,#0x140
  000ca6f6: movs r2,#0x3
  000ca6f8: blx 0x00073eb8
  000ca6fc: ldr r0,[sp,#0x48]
  000ca6fe: str.w r5,[r0,#0x110]
  000ca702: movs r0,#0xc
  000ca704: blx 0x0006eb24
  000ca708: mov r5,r0
  000ca70a: blx 0x00073d08
  000ca70e: ldr r0,[sp,#0x48]
  000ca710: mov r1,r5
  000ca712: str.w r5,[r0,#0xf8]
  000ca716: movs r0,#0x16
  000ca718: blx 0x00073d14
  000ca71c: ldr r0,[0x000caaa0]
  000ca71e: add r0,pc
  000ca720: vldr.64 d16,[r0]
  000ca724: ldr r0,[r0,#0x8]
  000ca726: str r0,[sp,#0x120]
  000ca728: movs r0,#0x18
  000ca72a: vstr.64 d16,[sp,#0x118]
  000ca72e: blx 0x0006eb24
  000ca732: mov r5,r0
  000ca734: add r1,sp,#0x118
  000ca736: movs r2,#0x3
  000ca738: blx 0x00073eb8
  000ca73c: ldr r6,[sp,#0x48]
  000ca73e: mov r0,r5
  000ca740: movs r1,#0x0
  000ca742: mov.w r11,#0x0
  000ca746: str.w r5,[r6,#0x10c]
  000ca74a: blx 0x00073ec4
  000ca74e: movs r1,#0x1
  000ca750: movs r2,#0x0
  000ca752: stm sp,{r0,r1,r11}
  000ca756: mov r0,r6
  000ca758: movs r1,#0x0
  000ca75a: movs r3,#0x33
  000ca75c: blx 0x00073e88
  000ca760: ldr.w r1,[r6,#0xf8]
  000ca764: ldr r1,[r1,#0x4]
  000ca766: str r0,[r1,#0x0]
  000ca768: movs r1,#0x1
  000ca76a: ldr.w r0,[r6,#0xf8]
  000ca76e: ldr r0,[r0,#0x4]
  000ca770: ldr r0,[r0,#0x0]
  000ca772: ldr r0,[r0,#0x4]
  000ca774: blx 0x00073fb4
  000ca778: ldr.w r0,[r6,#0xf8]
  000ca77c: add.w r8,sp,#0xf4
  000ca780: ldr.w r1,[r6,#0xf0]
  000ca784: ldr r0,[r0,#0x4]
  000ca786: ldr.w r10,[r0,#0x0]
  000ca78a: mov r0,r8
  000ca78c: blx 0x0007264c
  000ca790: ldr.w r0,[r6,#0xf0]
  000ca794: add r5,sp,#0xa0
  000ca796: ldr r1,[r0,#0x8]
  000ca798: mov r0,r5
  000ca79a: blx 0x0007261c
  000ca79e: add r4,sp,#0xd0
  000ca7a0: ldr r2,[0x000caaa4]
  000ca7a2: mov r1,r5
  000ca7a4: mov r0,r4
  000ca7a6: blx 0x0006ec74
  000ca7aa: add.w r9,sp,#0x1c0
  000ca7ae: mov r1,r8
  000ca7b0: mov r2,r4
  000ca7b2: mov r0,r9
  000ca7b4: blx 0x0006f1cc
  000ca7b8: ldr.w r0,[r6,#0xf0]
  000ca7bc: add r4,sp,#0x88
  000ca7be: ldr r1,[r0,#0x8]
  000ca7c0: mov r0,r4
  000ca7c2: blx 0x00072574
  000ca7c6: add r5,sp,#0x94
  000ca7c8: ldr r2,[0x000caaa8]
  000ca7ca: mov r1,r4
  000ca7cc: mov r0,r5
  000ca7ce: blx 0x0006ec74
  000ca7d2: add r4,sp,#0x180
  000ca7d4: mov r1,r9
  000ca7d6: mov r2,r5
  000ca7d8: mov r0,r4
  000ca7da: blx 0x0006f1cc
  000ca7de: ldr.w r0,[r10,#0x0]
  000ca7e2: mov r1,r4
  000ca7e4: ldr r2,[r0,#0x44]
  000ca7e6: mov r0,r10
  000ca7e8: blx r2
  000ca7ea: ldr.w r0,[r6,#0xf8]
  000ca7ee: add r5,sp,#0x180
  000ca7f0: ldr.w r1,[r6,#0xf0]
  000ca7f4: ldr r0,[r0,#0x4]
  000ca7f6: ldr r0,[r0,#0x0]
  000ca7f8: ldr r4,[r0,#0x8]
  000ca7fa: mov r0,r5
  000ca7fc: blx 0x00073540
  000ca800: mov.w r0,#0x3f800000
  000ca804: add r2,sp,#0x1c0
  000ca806: strd r11,r0,[sp,#0x1c0]
  000ca80a: mov r0,r4
  000ca80c: mov r1,r5
  000ca80e: str.w r11,[sp,#0x1c8]
  000ca812: blx 0x000726ac
  000ca816: ldr.w r0,[r6,#0xf8]
  000ca81a: movs r1,#0x0
  000ca81c: ldr r0,[r0,#0x4]
  000ca81e: ldr r0,[r0,#0x0]
  000ca820: blx 0x0007417c
  000ca824: ldr.w r0,[r6,#0xf8]
  000ca828: ldr r0,[r0,#0x4]
  000ca82a: ldr r2,[r0,#0x0]
  000ca82c: ldr.w r0,[r2,#0x50]!
  000ca830: cmp r0,#0x0
  000ca832: strb.w r11,[r2,#0xed]
  000ca836: beq 0x000ca84e
  000ca838: blx 0x00070204
  000ca83c: blx 0x0006eb48
  000ca840: ldr r0,[sp,#0x48]
  000ca842: ldr.w r0,[r0,#0xf8]
  000ca846: ldr r0,[r0,#0x4]
  000ca848: ldr r0,[r0,#0x0]
  000ca84a: add.w r2,r0,#0x50
  000ca84e: ldr r4,[sp,#0x48]
  000ca850: movs r1,#0x0
  000ca852: str.w r11,[r2,#0x0]
  000ca856: movw r2,#0x4220
  000ca85a: movs r3,#0x0
  000ca85c: mov r0,r4
  000ca85e: blx 0x00073f6c
  000ca862: ldr.w r1,[r4,#0xf8]
  000ca866: ldr r1,[r1,#0x4]
  000ca868: str r0,[r1,#0x4]
  000ca86a: movs r1,#0x0
  000ca86c: ldr.w r0,[r4,#0xf8]
  000ca870: ldr r0,[r0,#0x4]
  000ca872: ldr r0,[r0,#0x4]
  000ca874: blx 0x00073f54
  000ca878: ldr.w r0,[r4,#0xf8]
  000ca87c: movs r1,#0x3
  000ca87e: ldr r0,[r0,#0x4]
  000ca880: ldr r0,[r0,#0x4]
  000ca882: blx 0x00072748
  000ca886: ldr r0,[0x000caaac]
  000ca888: ldr.w r1,[r4,#0xf8]
  000ca88c: add r0,pc
  000ca88e: ldr r0,[r0,#0x0]
  000ca890: ldr r1,[r1,#0x4]
  000ca892: ldr r0,[r0,#0x0]
  000ca894: ldr r4,[r1,#0x4]
  000ca896: movs r1,#0x4d
  000ca898: blx 0x00072f70
  000ca89c: add r5,sp,#0x4c
  000ca89e: mov r1,r0
  000ca8a0: movs r2,#0x0
  000ca8a2: mov r0,r5
  000ca8a4: blx 0x0006f028
  000ca8a8: mov r0,r4
  000ca8aa: mov r1,r5
  000ca8ac: blx 0x00073fa8
  000ca8b0: add r0,sp,#0x4c
  000ca8b2: blx 0x0006ee30
  000ca8b6: ldr r0,[sp,#0x48]
  000ca8b8: movs r1,#0x1
  000ca8ba: movs r5,#0x1
  000ca8bc: mov r6,r0
  000ca8be: ldr.w r0,[r0,#0xf8]
  000ca8c2: ldr r0,[r0,#0x4]
  000ca8c4: ldr r0,[r0,#0x4]
  000ca8c6: ldr r0,[r0,#0x4]
  000ca8c8: blx 0x00073fb4
  000ca8cc: ldr.w r0,[r6,#0xf8]
  000ca8d0: movs r1,#0x0
  000ca8d2: ldr r2,[0x000caab0]
  000ca8d4: movs r3,#0x0
  000ca8d6: movs r4,#0x0
  000ca8d8: ldr r0,[r0,#0x4]
  000ca8da: ldr r0,[r0,#0x4]
  000ca8dc: ldr r0,[r0,#0x8]
  000ca8de: blx 0x00073054
  000ca8e2: ldr.w r0,[r6,#0xf8]
  000ca8e6: ldr r0,[r0,#0x4]
  000ca8e8: ldr r2,[r0,#0x4]
  000ca8ea: ldr.w r0,[r2,#0x50]!
  000ca8ee: cmp r0,#0x0
  000ca8f0: strb.w r4,[r2,#0x20]
  000ca8f4: strb.w r5,[r2,#0x24]
  000ca8f8: beq 0x000ca910
  000ca8fa: blx 0x00070204
  000ca8fe: blx 0x0006eb48
  000ca902: ldr r0,[sp,#0x48]
  000ca904: ldr.w r0,[r0,#0xf8]
  000ca908: ldr r0,[r0,#0x4]
  000ca90a: ldr r0,[r0,#0x4]
  000ca90c: add.w r2,r0,#0x50
  000ca910: ldr r1,[0x000caab4]
  000ca912: add r0,sp,#0x180
  000ca914: str r4,[r2,#0x0]
  000ca916: movs r2,#0x24
  000ca918: add r1,pc
  000ca91a: blx 0x0006f1e4
  000ca91e: ldr r0,[0x000caab8]
  000ca920: add r0,pc
  000ca922: vldr.64 d16,[r0]
  000ca926: ldr r0,[r0,#0x8]
  000ca928: str r0,[sp,#0x1c8]
  000ca92a: movs r0,#0xc
  000ca92c: vstr.64 d16,[sp,#0x1c0]
  000ca930: blx 0x0006eb24
  000ca934: mov r5,r0
  000ca936: blx 0x00073d08
  000ca93a: movs r0,#0x3
  000ca93c: mov r1,r5
  000ca93e: blx 0x00073d14
  000ca942: ldr r0,[sp,#0x48]
  000ca944: ldr r1,[r5,#0x4]
  000ca946: ldr.w r0,[r0,#0xf8]
  000ca94a: ldr r0,[r0,#0x4]
  000ca94c: ldr r0,[r0,#0x4]
  000ca94e: str r0,[r1,#0x4]
  000ca950: movs r0,#0x18
  000ca952: blx 0x0006eb24
  000ca956: mov r6,r0
  000ca958: movs r0,#0x9
  000ca95a: add r1,sp,#0x180
  000ca95c: add r3,sp,#0x1c0
  000ca95e: str r0,[sp,#0x0]
  000ca960: mov r0,r6
  000ca962: mov r2,r5
  000ca964: blx 0x00073fc0
  000ca968: ldr r0,[sp,#0x48]
  000ca96a: mov r1,r6
  000ca96c: mov r4,r0
  000ca96e: ldr.w r0,[r0,#0xf8]
  000ca972: ldr r0,[r0,#0x4]
  000ca974: ldr r0,[r0,#0x0]
  000ca976: blx 0x00073f60
  000ca97a: ldr.w r0,[r4,#0xf8]
  000ca97e: movs r4,#0x0
  000ca980: ldr r0,[r0,#0x4]
  000ca982: ldr r1,[r0,#0x0]
  000ca984: ldr r0,[r1,#0x4]
  000ca986: strb.w r4,[r1,#0x13e]
  000ca98a: movs r1,#0x0
  000ca98c: blx 0x0007294c
  000ca990: ldr r0,[0x000caabc]
  000ca992: add.w r8,sp,#0xa0
  000ca996: str r4,[sp,#0xf8]
  000ca998: mov.w r10,#0x0
  000ca99c: str r4,[sp,#0xf4]
  000ca99e: mov.w r11,#0x4
  000ca9a2: str r0,[sp,#0xfc]
  000ca9a4: movs r6,#0x2
  000ca9a6: ldr r0,[0x000caac0]
  000ca9a8: add r0,pc
  000ca9aa: ldr r0,[r0,#0x0]
  000ca9ac: str r0,[sp,#0x34]
  000ca9ae: ldr r0,[0x000caac4]
  000ca9b0: add r0,pc
  000ca9b2: ldr r0,[r0,#0x0]
  000ca9b4: str r0,[sp,#0x38]
  000ca9b6: b 0x000cabc0
  000ca9b8: ldr r0,[sp,#0x34]
  000ca9ba: movs r1,#0x9
  000ca9bc: ldr r0,[r0,#0x0]
  000ca9be: blx 0x00071a34
  000ca9c2: ldr r5,[sp,#0x48]
  000ca9c4: mov r4,r0
  000ca9c6: movs r1,#0x0
  000ca9c8: ldr.w r0,[r5,#0x110]
  000ca9cc: blx 0x00073ec4
  000ca9d0: str r0,[sp,#0x0]
  000ca9d2: movs r0,#0x1
  000ca9d4: strd r0,r10,[sp,#0x4]
  000ca9d8: mov r0,r5
  000ca9da: movs r1,#0x9
  000ca9dc: movs r2,#0x0
  000ca9de: mov r3,r4
  000ca9e0: blx 0x00073e88
  000ca9e4: ldr.w r1,[r5,#0xf8]
  000ca9e8: ldr r1,[r1,#0x4]
  000ca9ea: str.w r0,[r1,r11,lsl #0x1]
  000ca9ee: movs r1,#0x1
  000ca9f0: ldr.w r0,[r5,#0xf8]
  000ca9f4: ldr r0,[r0,#0x4]
  000ca9f6: ldr.w r0,[r0,r11,lsl #0x1]
  000ca9fa: blx 0x000732f4
  000ca9fe: ldr.w r0,[r5,#0xf8]
  000caa02: movs r1,#0x1
  000caa04: ldr r0,[r0,#0x4]
  000caa06: ldr.w r0,[r0,r11,lsl #0x1]
  000caa0a: ldr r0,[r0,#0x4]
  000caa0c: blx 0x00073e94
  000caa10: ldr.w r0,[r5,#0xf8]
  000caa14: cmp r6,#0xa
  000caa16: str r6,[sp,#0x3c]
  000caa18: ldr r0,[r0,#0x4]
  000caa1a: ldr.w r9,[r0,r11,lsl #0x1]
  000caa1e: ldr r0,[0x000caac8]
  000caa20: add r0,pc
  000caa22: ldr r6,[r0,#0x0]
  000caa24: bhi 0x000caacc
  000caa26: ldr r5,[sp,#0x38]
  000caa28: movs r1,#0x64
  000caa2a: ldr r0,[r5,#0x0]
  000caa2c: blx r6
  000caa2e: ldr r1,[sp,#0x3c]
  000caa30: add.w r0,r0,#0x384
  000caa34: vmov s0,r1
  000caa38: movs r1,#0x2
  000caa3a: bic.w r1,r1,r11
  000caa3e: vcvt.f32.s32 s0,s0
  000caa42: subs r2,r1,#0x1
  000caa44: ldr r1,[r5,#0x0]
  000caa46: mul r10,r0,r2
  000caa4a: mov r0,r1
  000caa4c: movs r1,#0xc8
  000caa4e: vmov r4,s0
  000caa52: blx r6
  000caa54: sub.w r1,r0,#0xc8
  000caa58: ldr r0,[r5,#0x0]
  000caa5a: vmov s0,r1
  000caa5e: mov.w r1,#0x190
  000caa62: vcvt.f32.s32 s16,s0
  000caa66: vmov s0,r10
  000caa6a: vcvt.f32.s32 s18,s0
  000caa6e: blx r6
  000caa70: subs r0,#0xc8
  000caa72: add r5,sp,#0x94
  000caa74: mov r1,r4
  000caa76: vmov s0,r0
  000caa7a: mov r0,r8
  000caa7c: mov r2,r5
  000caa7e: vcvt.f32.s32 s0,s0
  000caa82: vstr.32 s18,[sp,#0x94]
  000caa86: vstr.32 s16,[sp,#0x98]
  000caa8a: vstr.32 s0,[sp,#0x9c]
  000caa8e: blx 0x00072658
  000caa92: add r6,sp,#0xd0
  000caa94: add r1,sp,#0xf4
  000caa96: mov r2,r8
  000caa98: b 0x000cab4a
  000caacc: mov r4,r8
  000caace: ldr.w r8,[sp,#0x38]
  000caad2: movs r1,#0x64
  000caad4: ldr.w r0,[r8,#0x0]
  000caad8: blx r6
  000caada: ldr r1,[sp,#0x3c]
  000caadc: add.w r2,r0,#0x640
  000caae0: vmov s0,r1
  000caae4: movs r1,#0x2
  000caae6: bic.w r1,r1,r11
  000caaea: vcvt.f32.s32 s0,s0
  000caaee: subs r1,#0x1
  000caaf0: ldr.w r0,[r8,#0x0]
  000caaf4: mul r5,r2,r1
  000caaf8: movs r1,#0x64
  000caafa: vmov r10,s0
  000caafe: blx r6
  000cab00: sub.w r1,r0,#0xc8
  000cab04: ldr.w r0,[r8,#0x0]
  000cab08: mov r8,r4
  000cab0a: vmov s0,r1
  000cab0e: mov.w r1,#0x1f4
  000cab12: vcvt.f32.s32 s16,s0
  000cab16: vmov s0,r5
  000cab1a: vcvt.f32.s32 s18,s0
  000cab1e: blx r6
  000cab20: add.w r0,r0,#0x3e8
  000cab24: add r5,sp,#0x94
  000cab26: mov r1,r10
  000cab28: vmov s0,r0
  000cab2c: mov r0,r4
  000cab2e: mov r2,r5
  000cab30: vcvt.f32.s32 s0,s0
  000cab34: vstr.32 s18,[sp,#0x94]
  000cab38: vstr.32 s16,[sp,#0x98]
  000cab3c: vstr.32 s0,[sp,#0x9c]
  000cab40: blx 0x00072658
  000cab44: add r6,sp,#0xd0
  000cab46: add r1,sp,#0xf4
  000cab48: mov r2,r4
  000cab4a: mov r0,r6
  000cab4c: blx 0x0006f1cc
  000cab50: ldr.w r0,[r9,#0x0]
  000cab54: mov r1,r6
  000cab56: ldr r2,[r0,#0x44]
  000cab58: mov r0,r9
  000cab5a: blx r2
  000cab5c: ldr r0,[sp,#0x48]
  000cab5e: mov r4,r0
  000cab60: ldr.w r0,[r0,#0xf8]
  000cab64: ldr.w r1,[r4,#0xf0]
  000cab68: ldr r0,[r0,#0x4]
  000cab6a: ldr.w r0,[r0,r11,lsl #0x1]
  000cab6e: ldr.w r9,[r0,#0x8]
  000cab72: mov r0,r5
  000cab74: blx 0x0007264c
  000cab78: ldr.w r0,[r4,#0xf8]
  000cab7c: add r4,sp,#0x88
  000cab7e: ldr r0,[r0,#0x4]
  000cab80: ldr.w r1,[r0,r11,lsl #0x1]
  000cab84: ldr r0,[r1,#0x0]
  000cab86: ldr r2,[r0,#0x28]
  000cab88: mov r0,r4
  000cab8a: blx r2
  000cab8c: mov r0,r8
  000cab8e: mov r1,r5
  000cab90: mov r2,r4
  000cab92: blx 0x0006ec38
  000cab96: mov r0,r6
  000cab98: mov r1,r8
  000cab9a: blx 0x0006ec80
  000cab9e: mov.w r0,#0x3f800000
  000caba2: add r2,sp,#0x7c
  000caba4: str r0,[sp,#0x80]
  000caba6: movs r0,#0x0
  000caba8: str r0,[sp,#0x7c]
  000cabaa: mov r1,r6
  000cabac: str r0,[sp,#0x84]
  000cabae: mov r0,r9
  000cabb0: mov.w r10,#0x0
  000cabb4: blx 0x000726ac
  000cabb8: ldr r6,[sp,#0x3c]
  000cabba: add.w r11,r11,#0x2
  000cabbe: adds r6,#0x1
  000cabc0: cmp.w r11,#0x2c
  000cabc4: bne.w 0x000ca9b8
  000cabc8: movs r0,#0x1c
  000cabca: blx 0x0006eb24
  000cabce: mov r5,r0
  000cabd0: ldr r3,[sp,#0x48]
  000cabd2: movs r1,#0x4
  000cabd4: movs r2,#0xb
  000cabd6: blx 0x00073edc
  000cabda: b.w 0x000cb9b8
  000cabde: movs r0,#0x1c
  000cabe0: blx 0x0006eb24
  000cabe4: mov r5,r0
  000cabe6: ldr r3,[sp,#0x48]
  000cabe8: movs r1,#0x4
  000cabea: movs r2,#0x2
  000cabec: blx 0x00073edc
  000cabf0: b.w 0x000cb9b8
  000cabf4: ldr r0,[0x000caf24]
  000cabf6: add r4,sp,#0x180
  000cabf8: add r0,pc
  000cabfa: vld1.64 {d16,d17},[r0]!
  000cabfe: vldr.64 d18,[r0]
  000cac02: mov r0,r4
  000cac04: vst1.64 {d16,d17},[r0]!
  000cac08: vstr.64 d18,[r0]
  000cac0c: movs r0,#0x18
  000cac0e: blx 0x0006eb24
  000cac12: mov r5,r0
  000cac14: mov r1,r4
  000cac16: movs r2,#0x6
  000cac18: blx 0x00073eb8
  000cac1c: ldr r0,[sp,#0x48]
  000cac1e: movs r1,#0x0
  000cac20: str.w r5,[r0,#0x10c]
  000cac24: mov r0,r5
  000cac26: blx 0x00073ec4
  000cac2a: blx 0x00074194
  000cac2e: ldr r0,[0x000caf28]
  000cac30: add r4,sp,#0x140
  000cac32: add r0,pc
  000cac34: vld1.64 {d16,d17},[r0]!
  000cac38: vldr.64 d18,[r0]
  000cac3c: mov r0,r4
  000cac3e: vst1.64 {d16,d17},[r0]!
  000cac42: vstr.64 d18,[r0]
  000cac46: movs r0,#0x18
  000cac48: blx 0x0006eb24
  000cac4c: mov r5,r0
  000cac4e: mov r1,r4
  000cac50: movs r2,#0x6
  000cac52: blx 0x00073eb8
  000cac56: ldr r0,[sp,#0x48]
  000cac58: movs r1,#0x0
  000cac5a: mov.w r8,#0x0
  000cac5e: str.w r5,[r0,#0x110]
  000cac62: mov r0,r5
  000cac64: blx 0x00073ec4
  000cac68: blx 0x00074194
  000cac6c: movs r0,#0xc
  000cac6e: blx 0x0006eb24
  000cac72: mov r5,r0
  000cac74: blx 0x00073d08
  000cac78: ldr r0,[0x000caf2c]
  000cac7a: ldr r1,[sp,#0x48]
  000cac7c: add r0,pc
  000cac7e: ldr r0,[r0,#0x0]
  000cac80: str.w r5,[r1,#0xf8]
  000cac84: mov r1,r5
  000cac86: ldrb r6,[r0,#0x0]
  000cac88: movs r0,#0xa
  000cac8a: cmp r6,#0x0
  000cac8c: it eq
  000cac8e: mov.eq r0,#0x14
  000cac90: eor r10,r6,#0x1
  000cac94: orr.w r0,r0,r10
  000cac98: adds r0,#0x3
  000cac9a: blx 0x00073d14
  000cac9e: ldr r0,[0x000caf30]
  000caca0: movs r5,#0x5
  000caca2: ldr r2,[0x000caf34]
  000caca4: cmp r6,#0x0
  000caca6: add r0,pc
  000caca8: strd r2,r8,[sp,#0x118]
  000cacac: ldr r1,[0x000caf38]
  000cacae: ldr r0,[r0,#0x0]
  000cacb0: str r1,[sp,#0x120]
  000cacb2: mov.w r1,#0x65
  000cacb6: ldr r0,[r0,#0x0]
  000cacb8: it eq
  000cacba: mov.eq r5,#0xa
  000cacbc: blx 0x00071c44
  000cacc0: mov r9,r0
  000cacc2: mov.w r0,#0x174
  000cacc6: blx 0x0006eb24
  000cacca: mov r8,r0
  000caccc: mov r1,r9
  000cacce: blx 0x00073d2c
  000cacd2: ldr r0,[sp,#0x48]
  000cacd4: add.w r1,r5,r10
  000cacd8: adds r2,r1,r5
  000cacda: str r1,[sp,#0x2c]
  000cacdc: adds r6,r2,#0x2
  000cacde: mov r1,r9
  000cace0: mov r4,r0
  000cace2: ldr.w r0,[r0,#0xf8]
  000cace6: str r2,[sp,#0x3c]
  000cace8: ldr r0,[r0,#0x4]
  000cacea: str.w r8,[r0,r6,lsl #0x2]
  000cacee: add.w r8,sp,#0x1c0
  000cacf2: mov r0,r8
  000cacf4: blx 0x000736a8
  000cacf8: ldr.w r0,[r4,#0xf8]
  000cacfc: ldr r0,[r0,#0x4]
  000cacfe: ldr.w r0,[r0,r6,lsl #0x2]
  000cad02: adds r0,#0x18
  000cad04: mov r1,r8
  000cad06: blx 0x0006f2b0
  000cad0a: add r0,sp,#0x1c0
  000cad0c: str r5,[sp,#0x30]
  000cad0e: str.w r10,[sp,#0x24]
  000cad12: blx 0x0006ee30
  000cad16: ldr r0,[sp,#0x48]
  000cad18: movs r2,#0x3
  000cad1a: mov r4,r0
  000cad1c: ldr.w r0,[r0,#0xf8]
  000cad20: ldr r0,[r0,#0x4]
  000cad22: ldr.w r0,[r0,r6,lsl #0x2]
  000cad26: ldr r1,[r0,#0x0]
  000cad28: str r2,[r0,#0x28]
  000cad2a: ldr r2,[r1,#0x44]
  000cad2c: add r1,sp,#0x118
  000cad2e: blx r2
  000cad30: ldr.w r0,[r4,#0xf8]
  000cad34: ldr r0,[r0,#0x4]
  000cad36: ldr.w r2,[r0,r6,lsl #0x2]
  000cad3a: ldr.w r0,[r2,#0x50]!
  000cad3e: cbz r0,0x000cad58
  000cad40: blx 0x00070204
  000cad44: blx 0x0006eb48
  000cad48: ldr r0,[sp,#0x48]
  000cad4a: ldr.w r0,[r0,#0xf8]
  000cad4e: ldr r0,[r0,#0x4]
  000cad50: ldr.w r0,[r0,r6,lsl #0x2]
  000cad54: add.w r2,r0,#0x50
  000cad58: mov.w r8,#0x0
  000cad5c: cmp.w r9,#0x0
  000cad60: str r6,[sp,#0x20]
  000cad62: str.w r8,[r2,#0x0]
  000cad66: beq 0x000cad72
  000cad68: mov r0,r9
  000cad6a: blx 0x0007360c
  000cad6e: blx 0x0006eb48
  000cad72: ldr r0,[0x000caf3c]
  000cad74: mov.w r9,#0x1
  000cad78: ldr r5,[sp,#0x48]
  000cad7a: movs r6,#0x0
  000cad7c: add r0,pc
  000cad7e: ldr.w r10,[r0,#0x0]
  000cad82: b 0x000cade6
  000cad84: ldr.w r0,[r10,#0x0]
  000cad88: movs r1,#0x0
  000cad8a: blx 0x00071a34
  000cad8e: mov r4,r0
  000cad90: ldr.w r0,[r5,#0x10c]
  000cad94: movs r1,#0x0
  000cad96: blx 0x00073ec4
  000cad9a: strd r0,r9,[sp,#0x0]
  000cad9e: mov r0,r5
  000cada0: movs r1,#0x0
  000cada2: movs r2,#0x0
  000cada4: mov r3,r4
  000cada6: str.w r8,[sp,#0x8]
  000cadaa: blx 0x00073e88
  000cadae: ldr.w r1,[r5,#0xf8]
  000cadb2: ldr r1,[r1,#0x4]
  000cadb4: str.w r0,[r1,r6,lsl #0x2]
  000cadb8: movs r1,#0x1
  000cadba: ldr.w r0,[r5,#0xf8]
  000cadbe: ldr r0,[r0,#0x4]
  000cadc0: ldr.w r0,[r0,r6,lsl #0x2]
  000cadc4: ldr r0,[r0,#0x4]
  000cadc6: blx 0x00073fb4
  000cadca: ldr.w r1,[r5,#0xf8]
  000cadce: ldr.w r0,[r5,#0x10c]
  000cadd2: ldr r1,[r1,#0x4]
  000cadd4: ldr.w r4,[r1,r6,lsl #0x2]
  000cadd8: blx 0x000741a0
  000caddc: mov r1,r0
  000cadde: mov r0,r4
  000cade0: blx 0x00073f60
  000cade4: adds r6,#0x1
  000cade6: ldr r0,[sp,#0x30]
  000cade8: cmp r0,r6
  000cadea: bne 0x000cad84
  000cadec: ldr r0,[0x000caf40]
  000cadee: mov.w r8,#0x0
  000cadf2: ldrd r5,r4,[sp,#0x2c]
  000cadf6: ldr r6,[sp,#0x48]
  000cadf8: mov r9,r0
  000cadfa: ldr r0,[0x000caf44]
  000cadfc: mov r10,r0
  000cadfe: ldr r0,[0x000caf48]
  000cae00: mov r11,r0
  000cae02: b 0x000cae84
  000cae04: mov r0,r6
  000cae06: movs r1,#0x0
  000cae08: movw r2,#0x4974
  000cae0c: movs r3,#0x1
  000cae0e: blx 0x00073f6c
  000cae12: ldr.w r1,[r6,#0xf8]
  000cae16: ldr r1,[r1,#0x4]
  000cae18: str.w r0,[r1,r4,lsl #0x2]
  000cae1c: movs r1,#0x0
  000cae1e: ldr.w r0,[r6,#0xf8]
  000cae22: ldr r0,[r0,#0x4]
  000cae24: ldr.w r0,[r0,r4,lsl #0x2]
  000cae28: blx 0x00073f54
  000cae2c: ldr.w r0,[r6,#0xf8]
  000cae30: movs r1,#0x1
  000cae32: ldr r0,[r0,#0x4]
  000cae34: ldr.w r0,[r0,r4,lsl #0x2]
  000cae38: ldr r0,[r0,#0x4]
  000cae3a: blx 0x00073fb4
  000cae3e: ldr.w r0,[r6,#0xf8]
  000cae42: mov r2,r10
  000cae44: mov r3,r11
  000cae46: ldr r0,[r0,#0x4]
  000cae48: ldr.w r0,[r0,r4,lsl #0x2]
  000cae4c: ldr r1,[r0,#0x0]
  000cae4e: ldr.w r12,[r1,#0x48]
  000cae52: mov r1,r9
  000cae54: blx r12
  000cae56: ldr.w r0,[r6,#0xf8]
  000cae5a: ldr r0,[r0,#0x4]
  000cae5c: ldr.w r1,[r0,r4,lsl #0x2]
  000cae60: ldr.w r0,[r1,#0x50]!
  000cae64: cbz r0,0x000cae7e
  000cae66: blx 0x00070204
  000cae6a: blx 0x0006eb48
  000cae6e: ldr r0,[sp,#0x48]
  000cae70: ldr.w r0,[r0,#0xf8]
  000cae74: ldr r0,[r0,#0x4]
  000cae76: ldr.w r0,[r0,r4,lsl #0x2]
  000cae7a: add.w r1,r0,#0x50
  000cae7e: str.w r8,[r1,#0x0]
  000cae82: adds r4,#0x1
  000cae84: cmp r4,r5
  000cae86: bcc 0x000cae04
  000cae88: ldr r1,[0x000caf4c]
  000cae8a: ldr.w r10,[sp,#0x48]
  000cae8e: add r1,pc
  000cae90: ldr.w r11,[r1,#0x0]
  000cae94: movs r1,#0x1
  000cae96: ldr.w r0,[r10,#0x110]
  000cae9a: blx r11
  000cae9c: mov r1,r0
  000cae9e: ldr r0,[r0,#0x0]
  000caea0: add.w r9,sp,#0xf4
  000caea4: ldr r2,[r0,#0x28]
  000caea6: mov r0,r9
  000caea8: blx r2
  000caeaa: ldr.w r0,[r10,#0x110]
  000caeae: movs r1,#0x0
  000caeb0: mov.w r8,#0x0
  000caeb4: blx r11
  000caeb6: mov r1,r0
  000caeb8: ldr r0,[r0,#0x0]
  000caeba: add r4,sp,#0xd0
  000caebc: ldr r2,[r0,#0x28]
  000caebe: mov r0,r4
  000caec0: blx r2
  000caec2: add r6,sp,#0x1c0
  000caec4: mov r1,r9
  000caec6: mov r2,r4
  000caec8: mov r9,r5
  000caeca: mov r0,r6
  000caecc: blx 0x0006ec38
  000caed0: add r4,sp,#0xf4
  000caed2: mov r1,r6
  000caed4: str.w r8,[sp,#0x1c4]
  000caed8: mov r0,r4
  000caeda: blx 0x0006ec80
  000caede: mov r0,r6
  000caee0: mov r1,r4
  000caee2: blx 0x0006eb3c
  000caee6: ldr.w r0,[r10,#0x110]
  000caeea: movs r1,#0x0
  000caeec: blx r11
  000caeee: mov r1,r0
  000caef0: ldr r0,[r0,#0x0]
  000caef2: ldr r2,[r0,#0x28]
  000caef4: add r0,sp,#0xf4
  000caef6: blx r2
  000caef8: ldr r1,[sp,#0x30]
  000caefa: movs r6,#0x0
  000caefc: ldr r2,[sp,#0x24]
  000caefe: ldr r0,[0x000caf50]
  000caf00: lsls r1,r1,#0x2
  000caf02: vldr.32 s16,[pc,#0x50]
  000caf06: add.w r11,r1,r2, lsl #0x2
  000caf0a: add.w r5,r0,#0x1f4
  000caf0e: vldr.32 s18,[pc,#0x48]
  000caf12: add.w r1,r10,#0x18c
  000caf16: vldr.32 s20,[pc,#0x44]
  000caf1a: mov r10,r9
  000caf1c: vldr.32 s22,[pc,#0x40]
  000caf20: str r1,[sp,#0x28]
  000caf22: b 0x000cb128
  000caf64: mov r8,r9
  000caf66: mov.w r9,#0x0
  000caf6a: movs r0,#0x1
  000caf6c: str.w r9,[sp,#0x0]
  000caf70: strd r0,r9,[sp,#0x4]
  000caf74: movs r1,#0xa
  000caf76: ldr r4,[sp,#0x48]
  000caf78: movs r2,#0x0
  000caf7a: movs r3,#0x2c
  000caf7c: mov r0,r4
  000caf7e: blx 0x00073e88
  000caf82: ldr.w r1,[r4,#0xf8]
  000caf86: ldr r1,[r1,#0x4]
  000caf88: add r1,r11
  000caf8a: str.w r0,[r1,r6,lsl #0x2]
  000caf8e: movs r1,#0x1
  000caf90: ldr.w r0,[r4,#0xf8]
  000caf94: ldr r0,[r0,#0x4]
  000caf96: add r0,r11
  000caf98: ldr.w r0,[r0,r6,lsl #0x2]
  000caf9c: ldr r0,[r0,#0x4]
  000caf9e: blx 0x00073e94
  000cafa2: ldr.w r0,[r4,#0xf8]
  000cafa6: movs r1,#0x0
  000cafa8: ldr r0,[r0,#0x4]
  000cafaa: add r0,r11
  000cafac: ldr.w r0,[r0,r6,lsl #0x2]
  000cafb0: blx 0x000730b4
  000cafb4: ldr.w r0,[r4,#0xf8]
  000cafb8: movs r1,#0x0
  000cafba: ldr r0,[r0,#0x4]
  000cafbc: add r0,r11
  000cafbe: ldr.w r0,[r0,r6,lsl #0x2]
  000cafc2: blx 0x000732f4
  000cafc6: ldr.w r0,[r4,#0xf8]
  000cafca: mov.w r1,#0x3f800000
  000cafce: add r2,sp,#0xd0
  000cafd0: ldr r0,[r0,#0x4]
  000cafd2: add r0,r11
  000cafd4: ldr.w r0,[r0,r6,lsl #0x2]
  000cafd8: ldr r0,[r0,#0x8]
  000cafda: strd r9,r1,[sp,#0xd0]
  000cafde: add r1,sp,#0x1c0
  000cafe0: str.w r9,[sp,#0xd8]
  000cafe4: blx 0x000726ac
  000cafe8: ldr.w r0,[r4,#0xf8]
  000cafec: movs r1,#0x1
  000cafee: ldr r0,[r0,#0x4]
  000caff0: add r0,r11
  000caff2: ldr.w r0,[r0,r6,lsl #0x2]
  000caff6: blx 0x00074158
  000caffa: ldr.w r0,[r4,#0xf8]
  000caffe: movs r1,#0x0
  000cb000: ldr r0,[r0,#0x4]
  000cb002: add r0,r11
  000cb004: ldr.w r0,[r0,r6,lsl #0x2]
  000cb008: blx 0x00074164
  000cb00c: ldr.w r1,[r4,#0xf8]
  000cb010: ldr.w r0,[r4,#0x110]
  000cb014: ldr r1,[r1,#0x4]
  000cb016: add r1,r11
  000cb018: ldr.w r4,[r1,r6,lsl #0x2]
  000cb01c: blx 0x000741a0
  000cb020: mov r1,r0
  000cb022: mov r0,r4
  000cb024: blx 0x00073f60
  000cb028: vmov.f32 s0,s16
  000cb02c: cmp r6,#0x2
  000cb02e: mov r0,r5
  000cb030: str r5,[sp,#0x38]
  000cb032: bcc 0x000cb068
  000cb034: sub.w r0,r10,r8
  000cb038: cmp r6,#0x5
  000cb03a: bcs 0x000cb04c
  000cb03c: movw r1,#0xbb8
  000cb040: vmov.f32 s0,s18
  000cb044: muls r0,r1
  000cb046: movw r1,#0x2328
  000cb04a: b 0x000cb05e
  000cb04c: cmp r6,#0x9
  000cb04e: bcs 0x000cb062
  000cb050: movw r1,#0xbb8
  000cb054: vmov.f32 s0,s22
  000cb058: muls r0,r1
  000cb05a: movw r1,#0x4e20
  000cb05e: subs r0,r0,r1
  000cb060: b 0x000cb068
  000cb062: movs r0,#0x0
  000cb064: vmov.f32 s0,s20
  000cb068: vmov s2,r0
  000cb06c: movs r0,#0x0
  000cb06e: vcvt.f32.s32 s2,s2
  000cb072: ldr.w r8,[sp,#0x28]
  000cb076: str.w r10,[sp,#0x34]
  000cb07a: add.w r10,sp,#0xd0
  000cb07e: str r0,[sp,#0xd4]
  000cb080: mov r0,r8
  000cb082: mov r1,r10
  000cb084: vstr.32 s0,[sp,#0xd8]
  000cb088: vstr.32 s2,[sp,#0xd0]
  000cb08c: blx 0x0006eb3c
  000cb090: ldr r0,[0x000cb45c]
  000cb092: ldr r1,[0x000cb460]
  000cb094: add r0,pc
  000cb096: add r1,pc
  000cb098: ldr r4,[r0,#0x0]
  000cb09a: ldr r5,[r1,#0x0]
  000cb09c: movs r1,#0xc8
  000cb09e: ldr r0,[r4,#0x0]
  000cb0a0: blx r5
  000cb0a2: subs r0,#0x64
  000cb0a4: movs r1,#0xc8
  000cb0a6: vmov s0,r0
  000cb0aa: vcvt.f32.s32 s0,s0
  000cb0ae: ldr.w r9,[sp,#0x48]
  000cb0b2: vldr.32 s2,[r9,#0x18c]
  000cb0b6: vadd.f32 s0,s2,s0
  000cb0ba: vstr.32 s0,[r9,#0x18c]
  000cb0be: ldr r0,[r4,#0x0]
  000cb0c0: blx r5
  000cb0c2: subs r0,#0x64
  000cb0c4: movs r1,#0xc8
  000cb0c6: vmov s0,r0
  000cb0ca: vcvt.f32.s32 s0,s0
  000cb0ce: vldr.32 s2,[r9,#0x190]
  000cb0d2: vadd.f32 s0,s2,s0
  000cb0d6: vstr.32 s0,[r9,#0x190]
  000cb0da: ldr r0,[r4,#0x0]
  000cb0dc: blx r5
  000cb0de: subs r0,#0x64
  000cb0e0: add r1,sp,#0xf4
  000cb0e2: mov r2,r8
  000cb0e4: vmov s0,r0
  000cb0e8: vcvt.f32.s32 s0,s0
  000cb0ec: vldr.32 s2,[r9,#0x194]
  000cb0f0: ldr.w r0,[r9,#0xf8]
  000cb0f4: vadd.f32 s0,s2,s0
  000cb0f8: vstr.32 s0,[r9,#0x194]
  000cb0fc: ldr r0,[r0,#0x4]
  000cb0fe: ldr.w r9,[sp,#0x2c]
  000cb102: add r0,r11
  000cb104: ldr.w r4,[r0,r6,lsl #0x2]
  000cb108: mov r0,r10
  000cb10a: blx 0x0006f1cc
  000cb10e: ldr r0,[r4,#0x0]
  000cb110: mov r1,r10
  000cb112: ldr.w r10,[sp,#0x34]
  000cb116: ldr r2,[r0,#0x44]
  000cb118: mov r0,r4
  000cb11a: blx r2
  000cb11c: ldr r5,[sp,#0x38]
  000cb11e: adds r6,#0x1
  000cb120: add.w r10,r10,#0x1
  000cb124: add.w r5,r5,#0x7d0
  000cb128: ldr r1,[sp,#0x3c]
  000cb12a: add.w r0,r9,r6
  000cb12e: cmp r0,r1
  000cb130: blt.w 0x000caf64
  000cb134: ldr r0,[0x000cb464]
  000cb136: add.w r10,sp,#0x1c0
  000cb13a: ldr r6,[0x000cb468]
  000cb13c: add.w r11,sp,#0xd0
  000cb140: add r0,pc
  000cb142: ldr.w r8,[sp,#0x3c]
  000cb146: ldr r4,[sp,#0x48]
  000cb148: movs r5,#0x0
  000cb14a: ldr.w r9,[r0,#0x0]
  000cb14e: b 0x000cb23c
  000cb150: movs r0,#0x1
  000cb152: str r5,[sp,#0x0]
  000cb154: strd r0,r5,[sp,#0x4]
  000cb158: mov r0,r4
  000cb15a: movs r1,#0x2
  000cb15c: movs r2,#0x0
  000cb15e: movs r3,#0x31
  000cb160: blx 0x00073e88
  000cb164: ldr.w r1,[r4,#0xf8]
  000cb168: ldr r1,[r1,#0x4]
  000cb16a: str.w r0,[r1,r8,lsl #0x2]
  000cb16e: ldr.w r0,[r4,#0xf8]
  000cb172: ldr r0,[r0,#0x4]
  000cb174: ldr.w r0,[r0,r8,lsl #0x2]
  000cb178: ldr r1,[r0,#0x0]
  000cb17a: ldr r2,[r1,#0x44]
  000cb17c: add r1,sp,#0xf4
  000cb17e: blx r2
  000cb180: ldr.w r0,[r4,#0xf8]
  000cb184: movs r1,#0x1
  000cb186: ldr r0,[r0,#0x4]
  000cb188: ldr.w r0,[r0,r8,lsl #0x2]
  000cb18c: ldr r0,[r0,#0x4]
  000cb18e: blx 0x00073e94
  000cb192: ldr.w r0,[r4,#0xf8]
  000cb196: movs r1,#0x1
  000cb198: ldr r0,[r0,#0x4]
  000cb19a: ldr.w r0,[r0,r8,lsl #0x2]
  000cb19e: blx 0x00074158
  000cb1a2: ldr.w r0,[r4,#0xf8]
  000cb1a6: mov.w r1,#0x3f800000
  000cb1aa: mov r2,r11
  000cb1ac: ldr r0,[r0,#0x4]
  000cb1ae: ldr.w r0,[r0,r8,lsl #0x2]
  000cb1b2: ldr r0,[r0,#0x8]
  000cb1b4: strd r5,r1,[sp,#0xd0]
  000cb1b8: mov r1,r10
  000cb1ba: str r5,[sp,#0xd8]
  000cb1bc: blx 0x000726ac
  000cb1c0: ldr.w r0,[r4,#0xf8]
  000cb1c4: movs r1,#0x0
  000cb1c6: ldr r0,[r0,#0x4]
  000cb1c8: ldr.w r0,[r0,r8,lsl #0x2]
  000cb1cc: blx 0x000732f4
  000cb1d0: ldr.w r0,[r4,#0xf8]
  000cb1d4: movs r1,#0x0
  000cb1d6: ldr r0,[r0,#0x4]
  000cb1d8: ldr.w r0,[r0,r8,lsl #0x2]
  000cb1dc: blx 0x000730b4
  000cb1e0: ldr.w r0,[r4,#0xf8]
  000cb1e4: ldr r0,[r0,#0x4]
  000cb1e6: ldr.w r0,[r0,r8,lsl #0x2]
  000cb1ea: ldr r1,[r0,#0x0]
  000cb1ec: ldr r2,[r1,#0x1c]
  000cb1ee: mov r1,r6
  000cb1f0: blx r2
  000cb1f2: ldr.w r0,[r9,#0x0]
  000cb1f6: movw r1,#0x664
  000cb1fa: blx 0x00072f70
  000cb1fe: mov r1,r0
  000cb200: ldr.w r0,[r4,#0xf8]
  000cb204: ldr r0,[r0,#0x4]
  000cb206: ldr.w r0,[r0,r8,lsl #0x2]
  000cb20a: adds r0,#0x18
  000cb20c: blx 0x0006f2b0
  000cb210: ldr.w r0,[r4,#0xf8]
  000cb214: ldr r0,[r0,#0x4]
  000cb216: ldr.w r1,[r0,r8,lsl #0x2]
  000cb21a: ldr.w r0,[r1,#0x50]!
  000cb21e: cbz r0,0x000cb236
  000cb220: blx 0x00070204
  000cb224: blx 0x0006eb48
  000cb228: ldr.w r0,[r4,#0xf8]
  000cb22c: ldr r0,[r0,#0x4]
  000cb22e: ldr.w r0,[r0,r8,lsl #0x2]
  000cb232: add.w r1,r0,#0x50
  000cb236: str r5,[r1,#0x0]
  000cb238: add.w r8,r8,#0x1
  000cb23c: ldr r0,[sp,#0x3c]
  000cb23e: cmp r8,r0
  000cb240: bls 0x000cb150
  000cb242: ldr r1,[sp,#0x30]
  000cb244: mov.w r10,#0x0
  000cb248: ldr r0,[sp,#0x24]
  000cb24a: ldr.w r11,[sp,#0x20]
  000cb24e: add.w r0,r0,r1, lsl #0x1
  000cb252: ldr r5,[sp,#0x48]
  000cb254: adds r6,r0,#0x1
  000cb256: ldr r0,[0x000cb46c]
  000cb258: mov r8,r0
  000cb25a: b 0x000cb2e2
  000cb25c: movs r0,#0x1
  000cb25e: str.w r10,[sp,#0x0]
  000cb262: strd r0,r10,[sp,#0x4]
  000cb266: mov r0,r5
  000cb268: movs r1,#0x3
  000cb26a: movs r2,#0x0
  000cb26c: movs r3,#0x14
  000cb26e: blx 0x00073e88
  000cb272: ldr.w r1,[r5,#0xf8]
  000cb276: mov r2,r8
  000cb278: mov r3,r8
  000cb27a: ldr r1,[r1,#0x4]
  000cb27c: str.w r0,[r1,r6,lsl #0x2]
  000cb280: ldr.w r0,[r5,#0xf8]
  000cb284: ldr r0,[r0,#0x4]
  000cb286: ldr.w r0,[r0,r6,lsl #0x2]
  000cb28a: ldr r1,[r0,#0x0]
  000cb28c: ldr r4,[r1,#0x48]
  000cb28e: mov r1,r8
  000cb290: blx r4
  000cb292: ldr.w r0,[r5,#0xf8]
  000cb296: movs r1,#0x1
  000cb298: ldr r0,[r0,#0x4]
  000cb29a: ldr.w r0,[r0,r6,lsl #0x2]
  000cb29e: blx 0x00074158
  000cb2a2: ldr.w r0,[r5,#0xf8]
  000cb2a6: movs r1,#0x0
  000cb2a8: ldr r0,[r0,#0x4]
  000cb2aa: ldr.w r0,[r0,r6,lsl #0x2]
  000cb2ae: blx 0x000732f4
  000cb2b2: ldr.w r0,[r5,#0xf8]
  000cb2b6: movs r1,#0x0
  000cb2b8: ldr r0,[r0,#0x4]
  000cb2ba: ldr.w r0,[r0,r6,lsl #0x2]
  000cb2be: blx 0x000730b4
  000cb2c2: ldr.w r0,[r9,#0x0]
  000cb2c6: movw r1,#0x657
  000cb2ca: blx 0x00072f70
  000cb2ce: mov r1,r0
  000cb2d0: ldr.w r0,[r5,#0xf8]
  000cb2d4: ldr r0,[r0,#0x4]
  000cb2d6: ldr.w r0,[r0,r6,lsl #0x2]
  000cb2da: adds r0,#0x18
  000cb2dc: blx 0x0006f2b0
  000cb2e0: adds r6,#0x1
  000cb2e2: cmp r6,r11
  000cb2e4: bcc 0x000cb25c
  000cb2e6: ldr r4,[sp,#0x48]
  000cb2e8: movs r2,#0x0
  000cb2ea: ldr r1,[0x000cb470]
  000cb2ec: movs r6,#0x0
  000cb2ee: ldr r3,[0x000cb474]
  000cb2f0: ldr.w r0,[r4,#0xf0]
  000cb2f4: blx 0x000725bc
  000cb2f8: ldr.w r0,[r4,#0xf0]
  000cb2fc: add r5,sp,#0xd0
  000cb2fe: ldr r1,[0x000cb478]
  000cb300: ldr r4,[r0,#0x8]
  000cb302: ldr r0,[0x000cb47c]
  000cb304: strd r0,r1,[sp,#0xa0]
  000cb308: mov.w r0,#0xbf000000
  000cb30c: add r1,sp,#0xa0
  000cb30e: str r0,[sp,#0xa8]
  000cb310: mov r0,r5
  000cb312: blx 0x0006ec80
  000cb316: mov.w r0,#0x3f800000
  000cb31a: add r2,sp,#0x94
  000cb31c: strd r6,r0,[sp,#0x94]
  000cb320: mov r0,r4
  000cb322: mov r1,r5
  000cb324: str r6,[sp,#0x9c]
  000cb326: blx 0x000726ac
  000cb32a: b 0x000cb9bc
  000cb32c: movs r0,#0xc
  000cb32e: blx 0x0006eb24
  000cb332: mov r5,r0
  000cb334: blx 0x00073d08
  000cb338: ldr r6,[sp,#0x48]
  000cb33a: movs r0,#0x1
  000cb33c: mov r1,r5
  000cb33e: movs r4,#0x1
  000cb340: str.w r5,[r6,#0xf8]
  000cb344: blx 0x00073d14
  000cb348: mov.w r8,#0x0
  000cb34c: mov r0,r6
  000cb34e: movs r1,#0xa
  000cb350: movs r2,#0x0
  000cb352: movs r3,#0x31
  000cb354: strd r8,r4,[sp,#0x0]
  000cb358: str.w r8,[sp,#0x8]
  000cb35c: blx 0x00073e88
  000cb360: ldr.w r1,[r6,#0xf8]
  000cb364: ldr r3,[0x000cb480]
  000cb366: ldr r1,[r1,#0x4]
  000cb368: str r0,[r1,#0x0]
  000cb36a: ldr.w r0,[r6,#0xf8]
  000cb36e: ldr r1,[0x000cb484]
  000cb370: ldr r0,[r0,#0x4]
  000cb372: ldr r0,[r0,#0x0]
  000cb374: ldr r2,[r0,#0x0]
  000cb376: ldr r5,[r2,#0x48]
  000cb378: movs r2,#0x0
  000cb37a: blx r5
  000cb37c: ldr.w r0,[r6,#0xf8]
  000cb380: add r5,sp,#0x180
  000cb382: ldr r1,[0x000cb47c]
  000cb384: ldr r0,[r0,#0x4]
  000cb386: ldr r0,[r0,#0x0]
  000cb388: ldr r4,[r0,#0x8]
  000cb38a: strd r1,r8,[sp,#0x140]
  000cb38e: add r1,sp,#0x140
  000cb390: ldr r0,[0x000cb488]
  000cb392: str r0,[sp,#0x148]
  000cb394: mov r0,r5
  000cb396: blx 0x0006ec80
  000cb39a: mov.w r0,#0x3f800000
  000cb39e: add r2,sp,#0x118
  000cb3a0: strd r8,r0,[sp,#0x118]
  000cb3a4: mov r0,r4
  000cb3a6: mov r1,r5
  000cb3a8: str.w r8,[sp,#0x120]
  000cb3ac: blx 0x000726ac
  000cb3b0: ldr.w r0,[r6,#0xf8]
  000cb3b4: movs r1,#0x1
  000cb3b6: ldr r0,[r0,#0x4]
  000cb3b8: ldr r0,[r0,#0x0]
  000cb3ba: ldr r0,[r0,#0x4]
  000cb3bc: blx 0x00073e94
  000cb3c0: ldr.w r0,[r6,#0xf8]
  000cb3c4: ldr r0,[r0,#0x4]
  000cb3c6: ldr r0,[r0,#0x0]
  000cb3c8: ldr r1,[r0,#0x0]
  000cb3ca: ldr r2,[r1,#0x1c]
  000cb3cc: ldr r1,[0x000cb48c]
  000cb3ce: blx r2
  000cb3d0: ldr.w r0,[r6,#0xf8]
  000cb3d4: ldr r0,[r0,#0x4]
  000cb3d6: ldr r0,[r0,#0x0]
  000cb3d8: ldr r4,[r0,#0x4]
  000cb3da: mov r0,r4
  000cb3dc: blx 0x000741ac
  000cb3e0: add.w r1,r0,r0, lsl #0x1
  000cb3e4: mov r0,r4
  000cb3e6: blx 0x000741b8
  000cb3ea: ldr.w r0,[r6,#0xf8]
  000cb3ee: movs r1,#0x0
  000cb3f0: ldr r0,[r0,#0x4]
  000cb3f2: ldr r0,[r0,#0x0]
  000cb3f4: blx 0x00074164
  000cb3f8: ldr r0,[0x000cb490]
  000cb3fa: movw r1,#0x664
  000cb3fe: add r0,pc
  000cb400: ldr r0,[r0,#0x0]
  000cb402: ldr r0,[r0,#0x0]
  000cb404: blx 0x00072f70
  000cb408: mov r1,r0
  000cb40a: ldr.w r0,[r6,#0xf8]
  000cb40e: ldr r0,[r0,#0x4]
  000cb410: ldr r0,[r0,#0x0]
  000cb412: adds r0,#0x18
  000cb414: blx 0x0006f2b0
  000cb418: ldr.w r0,[r6,#0xf8]
  000cb41c: movs r1,#0x1
  000cb41e: ldr r0,[r0,#0x4]
  000cb420: ldr r0,[r0,#0x0]
  000cb422: blx 0x00074158
  000cb426: ldr.w r0,[r6,#0xf8]
  000cb42a: ldr r0,[r0,#0x4]
  000cb42c: ldr r2,[r0,#0x0]
  000cb42e: ldr.w r0,[r2,#0x50]!
  000cb432: cbz r0,0x000cb44a
  000cb434: blx 0x00070204
  000cb438: blx 0x0006eb48
  000cb43c: ldr r0,[sp,#0x48]
  000cb43e: ldr.w r0,[r0,#0xf8]
  000cb442: ldr r0,[r0,#0x4]
  000cb444: ldr r0,[r0,#0x0]
  000cb446: add.w r2,r0,#0x50
  000cb44a: ldr r0,[0x000cb46c]
  000cb44c: ldr r4,[sp,#0x48]
  000cb44e: str.w r8,[r2,#0x0]
  000cb452: mov.w r8,#0x3
  000cb456: mov r9,r0
  000cb458: b 0x000cb4ea
  000cb494: mov r0,r4
  000cb496: movs r1,#0x0
  000cb498: movw r2,#0x49c2
  000cb49c: movs r3,#0x1
  000cb49e: blx 0x00073f6c
  000cb4a2: mov r6,r0
  000cb4a4: ldr r0,[r0,#0x4]
  000cb4a6: mov.w r1,#0x320
  000cb4aa: blx 0x000730d8
  000cb4ae: ldr r0,[r6,#0x4]
  000cb4b0: movs r1,#0x1
  000cb4b2: blx 0x00073e94
  000cb4b6: ldr r0,[r6,#0x4]
  000cb4b8: movs r1,#0x0
  000cb4ba: blx 0x00073fb4
  000cb4be: ldr r0,[r6,#0x4]
  000cb4c0: movs r1,#0x64
  000cb4c2: blx 0x00073f78
  000cb4c6: ldr r0,[r6,#0x0]
  000cb4c8: mov r1,r9
  000cb4ca: mov r2,r9
  000cb4cc: mov r3,r9
  000cb4ce: ldr r5,[r0,#0x48]
  000cb4d0: mov r0,r6
  000cb4d2: blx r5
  000cb4d4: mov r0,r6
  000cb4d6: movs r1,#0x0
  000cb4d8: blx 0x000732f4
  000cb4dc: ldr.w r1,[r4,#0xf8]
  000cb4e0: mov r0,r6
  000cb4e2: blx 0x000740f8
  000cb4e6: sub.w r8,r8,#0x1
  000cb4ea: cmp.w r8,#0x0
  000cb4ee: bne 0x000cb494
  000cb4f0: movs r0,#0x1c
  000cb4f2: blx 0x0006eb24
  000cb4f6: mov r5,r0
  000cb4f8: ldr r3,[sp,#0x48]
  000cb4fa: movs r1,#0x16
  000cb4fc: movs r2,#0x0
  000cb4fe: blx 0x00073edc
  000cb502: b 0x000cb9b8
  000cb504: movs r0,#0xc
  000cb506: blx 0x0006eb24
  000cb50a: mov r5,r0
  000cb50c: blx 0x00073d08
  000cb510: ldr r0,[sp,#0x48]
  000cb512: mov r1,r5
  000cb514: str.w r5,[r0,#0xf8]
  000cb518: movs r0,#0x6
  000cb51a: blx 0x00073d14
  000cb51e: ldr r0,[0x000cb8c4]
  000cb520: movs r1,#0x0
  000cb522: strd r1,r1,[sp,#0x180]
  000cb526: add r0,pc
  000cb528: str r1,[sp,#0x188]
  000cb52a: vldr.64 d16,[r0]
  000cb52e: ldr r0,[r0,#0x8]
  000cb530: str r0,[sp,#0x148]
  000cb532: movs r0,#0x18
  000cb534: vstr.64 d16,[sp,#0x140]
  000cb538: blx 0x0006eb24
  000cb53c: mov r5,r0
  000cb53e: add r1,sp,#0x140
  000cb540: movs r2,#0x3
  000cb542: blx 0x00073eb8
  000cb546: ldr r0,[sp,#0x48]
  000cb548: add.w r9,sp,#0x180
  000cb54c: vldr.32 s16,[pc,#0x378]
  000cb550: movw r8,#0x1b58
  000cb554: mov.w r11,#0x0
  000cb558: str.w r5,[r0,#0x110]
  000cb55c: add r5,sp,#0x118
  000cb55e: ldr r0,[0x000cb8cc]
  000cb560: ldr r6,[sp,#0x48]
  000cb562: add r0,pc
  000cb564: ldr.w r10,[r0,#0x0]
  000cb568: b 0x000cb62a
  000cb56a: ldr.w r0,[r10,#0x0]
  000cb56e: movs r1,#0x8
  000cb570: blx 0x00071a34
  000cb574: mov r4,r0
  000cb576: ldr.w r0,[r6,#0x110]
  000cb57a: movs r1,#0x0
  000cb57c: blx 0x00073ec4
  000cb580: str r0,[sp,#0x0]
  000cb582: movs r0,#0x1
  000cb584: str r0,[sp,#0x4]
  000cb586: movs r0,#0x0
  000cb588: str r0,[sp,#0x8]
  000cb58a: mov r0,r6
  000cb58c: movs r1,#0x8
  000cb58e: movs r2,#0x0
  000cb590: mov r3,r4
  000cb592: blx 0x00073e88
  000cb596: ldr.w r1,[r6,#0xf8]
  000cb59a: ldr r1,[r1,#0x4]
  000cb59c: str.w r0,[r1,r11,lsl #0x2]
  000cb5a0: ldr.w r0,[r6,#0xfc]
  000cb5a4: ldrd r1,r0,[r0,#0x0]
  000cb5a8: add.w r1,r11,r1, lsr #0x1
  000cb5ac: ldr.w r1,[r0,r1,lsl #0x2]
  000cb5b0: ldr r0,[r1,#0x0]
  000cb5b2: ldr r2,[r0,#0x28]
  000cb5b4: mov r0,r5
  000cb5b6: blx r2
  000cb5b8: mov r0,r9
  000cb5ba: mov r1,r5
  000cb5bc: blx 0x0006eb3c
  000cb5c0: vldr.32 s0,[sp,#0x184]
  000cb5c4: ldr.w r0,[r6,#0xf8]
  000cb5c8: vadd.f32 s0,s0,s16
  000cb5cc: ldr r3,[sp,#0x188]
  000cb5ce: ldr r1,[sp,#0x180]
  000cb5d0: ldr r0,[r0,#0x4]
  000cb5d2: ldr.w r0,[r0,r11,lsl #0x2]
  000cb5d6: vmov r2,s0
  000cb5da: ldr r4,[r0,#0x0]
  000cb5dc: ldr r4,[r4,#0x48]
  000cb5de: blx r4
  000cb5e0: ldr.w r0,[r6,#0xf8]
  000cb5e4: ldr r0,[r0,#0x4]
  000cb5e6: ldr.w r0,[r0,r11,lsl #0x2]
  000cb5ea: blx 0x000732dc
  000cb5ee: ldr.w r0,[r6,#0xf8]
  000cb5f2: ldr r0,[r0,#0x4]
  000cb5f4: ldr.w r0,[r0,r11,lsl #0x2]
  000cb5f8: ldr r4,[r0,#0x4]
  000cb5fa: str.w r8,[r0,#0x128]
  000cb5fe: mov r0,r4
  000cb600: blx 0x00072910
  000cb604: lsls r1,r0,#0x1
  000cb606: mov r0,r4
  000cb608: blx 0x00073f78
  000cb60c: ldr.w r0,[r6,#0xf8]
  000cb610: ldr r0,[r0,#0x4]
  000cb612: ldr.w r0,[r0,r11,lsl #0x2]
  000cb616: ldr r4,[r0,#0x4]
  000cb618: mov r0,r4
  000cb61a: blx 0x00072910
  000cb61e: mov r1,r0
  000cb620: mov r0,r4
  000cb622: blx 0x000727e4
  000cb626: add.w r11,r11,#0x1
  000cb62a: cmp.w r11,#0x6
  000cb62e: bne 0x000cb56a
  000cb630: movs r0,#0x1c
  000cb632: blx 0x0006eb24
  000cb636: mov r5,r0
  000cb638: ldr r0,[sp,#0x48]
  000cb63a: movs r1,#0x12
  000cb63c: str r0,[sp,#0x0]
  000cb63e: mov r0,r5
  000cb640: movs r2,#0x0
  000cb642: movs r3,#0x6
  000cb644: blx 0x00073f9c
  000cb648: b 0x000cb9b8
  000cb64a: ldr r0,[0x000cb8d0]
  000cb64c: add r0,pc
  000cb64e: vldr.64 d16,[r0]
  000cb652: ldr r0,[r0,#0x8]
  000cb654: str r0,[sp,#0x188]
  000cb656: movs r0,#0x18
  000cb658: vstr.64 d16,[sp,#0x180]
  000cb65c: blx 0x0006eb24
  000cb660: mov r5,r0
  000cb662: add r1,sp,#0x180
  000cb664: movs r2,#0x3
  000cb666: blx 0x00073eb8
  000cb66a: ldr r0,[sp,#0x48]
  000cb66c: str.w r5,[r0,#0x110]
  000cb670: movs r0,#0xc
  000cb672: blx 0x0006eb24
  000cb676: mov r5,r0
  000cb678: blx 0x00073d08
  000cb67c: ldr r0,[sp,#0x48]
  000cb67e: mov r1,r5
  000cb680: str.w r5,[r0,#0xf8]
  000cb684: movs r0,#0x2
  000cb686: blx 0x00073d14
  000cb68a: add.w r8,sp,#0x140
  000cb68e: movs r4,#0x0
  000cb690: b 0x000cb74e
  000cb692: ldr r5,[sp,#0x48]
  000cb694: movs r1,#0x0
  000cb696: ldr.w r0,[r5,#0x110]
  000cb69a: blx 0x00073ec4
  000cb69e: str r0,[sp,#0x0]
  000cb6a0: movs r0,#0x1
  000cb6a2: str r0,[sp,#0x4]
  000cb6a4: movs r0,#0x0
  000cb6a6: str r0,[sp,#0x8]
  000cb6a8: mov r0,r5
  000cb6aa: movs r1,#0xa
  000cb6ac: movs r2,#0x0
  000cb6ae: movs r3,#0x2c
  000cb6b0: blx 0x00073e88
  000cb6b4: ldr.w r1,[r5,#0xf8]
  000cb6b8: ldr r1,[r1,#0x4]
  000cb6ba: str.w r0,[r1,r4,lsl #0x2]
  000cb6be: movs r1,#0x1
  000cb6c0: ldr.w r0,[r5,#0xf8]
  000cb6c4: ldr r0,[r0,#0x4]
  000cb6c6: ldr.w r0,[r0,r4,lsl #0x2]
  000cb6ca: ldr r0,[r0,#0x4]
  000cb6cc: blx 0x00073e94
  000cb6d0: ldr.w r0,[r5,#0xf8]
  000cb6d4: add.w r11,sp,#0x1c0
  000cb6d8: ldr.w r1,[r5,#0xf0]
  000cb6dc: ldr r0,[r0,#0x4]
  000cb6de: ldr.w r9,[r0,r4,lsl #0x2]
  000cb6e2: mov r0,r11
  000cb6e4: blx 0x0007264c
  000cb6e8: ldr.w r0,[r5,#0xf0]
  000cb6ec: add.w r10,sp,#0xd0
  000cb6f0: ldr r1,[r0,#0x8]
  000cb6f2: mov r0,r10
  000cb6f4: blx 0x0007261c
  000cb6f8: add r6,sp,#0xf4
  000cb6fa: ldr r2,[0x000cb8d4]
  000cb6fc: mov r1,r10
  000cb6fe: mov r0,r6
  000cb700: blx 0x0006ec74
  000cb704: add.w r10,sp,#0x118
  000cb708: mov r1,r11
  000cb70a: mov r2,r6
  000cb70c: mov r0,r10
  000cb70e: blx 0x0006f1cc
  000cb712: ldr.w r0,[r5,#0xf0]
  000cb716: add r5,sp,#0x94
  000cb718: ldr r1,[r0,#0x8]
  000cb71a: mov r0,r5
  000cb71c: blx 0x00072574
  000cb720: adr.w r0,0xcbf9c
  000cb724: cmp r4,#0x0
  000cb726: it eq
  000cb728: add.eq r0,#0x4
  000cb72a: add r6,sp,#0xa0
  000cb72c: ldr r2,[r0,#0x0]
  000cb72e: mov r1,r5
  000cb730: mov r0,r6
  000cb732: blx 0x0006ec74
  000cb736: mov r0,r8
  000cb738: mov r1,r10
  000cb73a: mov r2,r6
  000cb73c: blx 0x0006f1cc
  000cb740: ldr.w r0,[r9,#0x0]
  000cb744: mov r1,r8
  000cb746: ldr r2,[r0,#0x44]
  000cb748: mov r0,r9
  000cb74a: blx r2
  000cb74c: adds r4,#0x1
  000cb74e: cmp r4,#0x2
  000cb750: bne 0x000cb692
  000cb752: movs r0,#0x1c
  000cb754: blx 0x0006eb24
  000cb758: mov r5,r0
  000cb75a: ldr r3,[sp,#0x48]
  000cb75c: movs r1,#0x4
  000cb75e: movs r2,#0x0
  000cb760: blx 0x00073edc
  000cb764: b 0x000cb9b8
  000cb766: movs r0,#0x1c
  000cb768: blx 0x0006eb24
  000cb76c: mov r5,r0
  000cb76e: ldr r3,[sp,#0x48]
  000cb770: movs r1,#0x4
  000cb772: movs r2,#0x3
  000cb774: blx 0x00073edc
  000cb778: b 0x000cb9b8
  000cb77a: ldr r0,[r5,#0x0]
  000cb77c: blx 0x00071c5c
  000cb780: rsb r0,r0,r0, lsl #0x4
  000cb784: adds r0,#0xdc
  000cb786: str r0,[sp,#0x34]
  000cb788: mov.w r9,#0x0
  000cb78c: ldr r0,[0x000cb8d8]
  000cb78e: movs r5,#0x0
  000cb790: add r0,pc
  000cb792: str r0,[sp,#0x30]
  000cb794: b 0x000cb878
  000cb796: movw r0,#0x9f3
  000cb79a: ldr r4,[sp,#0x48]
  000cb79c: lsrs r0,r5
  000cb79e: movs r1,#0x0
  000cb7a0: lsls r0,r0,#0x1f
  000cb7a2: ite eq
  000cb7a4: movw.eq r2,#0x381d
  000cb7a8: movw.ne r2,#0x381b
  000cb7ac: mov r0,r4
  000cb7ae: movs r3,#0x1
  000cb7b0: blx 0x00073f6c
  000cb7b4: ldr.w r1,[r4,#0xf8]
  000cb7b8: blx 0x000740f8
  000cb7bc: ldr.w r0,[r4,#0xf8]
  000cb7c0: str r5,[sp,#0x3c]
  000cb7c2: ldr r1,[sp,#0x34]
  000cb7c4: ldr r0,[r0,#0x4]
  000cb7c6: add.w r0,r0,r5, lsl #0x2
  000cb7ca: ldr r5,[r0,#0x4]
  000cb7cc: ldr r0,[r5,#0x4]
  000cb7ce: blx 0x00073f78
  000cb7d2: ldr r0,[sp,#0x30]
  000cb7d4: add.w r8,sp,#0x1c0
  000cb7d8: add r6,sp,#0x180
  000cb7da: add r0,r9
  000cb7dc: ldr r1,[r0,#0x4]
  000cb7de: str r1,[sp,#0x38]
  000cb7e0: ldrd r10,r11,[r0,#0x8]
  000cb7e4: ldrd r1,r2,[r0,#0x10]
  000cb7e8: strd r1,r2,[sp,#0xf4]
  000cb7ec: add r2,sp,#0xf4
  000cb7ee: ldr r0,[r0,#0x18]
  000cb7f0: mov r1,r6
  000cb7f2: str r0,[sp,#0xfc]
  000cb7f4: mov r0,r8
  000cb7f6: blx 0x0006f694
  000cb7fa: add r4,sp,#0x140
  000cb7fc: mov r1,r8
  000cb7fe: mov r0,r4
  000cb800: blx 0x0006eb3c
  000cb804: ldr r0,[r5,#0x8]
  000cb806: mov r1,r4
  000cb808: blx 0x00073dd4
  000cb80c: ldr r0,[sp,#0x38]
  000cb80e: add r2,sp,#0xf4
  000cb810: str.w r10,[sp,#0xf8]
  000cb814: mov r1,r6
  000cb816: str r0,[sp,#0xf4]
  000cb818: mov r0,r8
  000cb81a: str.w r11,[sp,#0xfc]
  000cb81e: blx 0x0006f694
  000cb822: mov r0,r4
  000cb824: mov r1,r8
  000cb826: blx 0x0006eb3c
  000cb82a: vldr.32 s0,[sp,#0x140]
  000cb82e: mov r1,r8
  000cb830: vldr.32 s6,[sp,#0x118]
  000cb834: vldr.32 s2,[sp,#0x144]
  000cb838: vldr.32 s8,[sp,#0x11c]
  000cb83c: vadd.f32 s0,s6,s0
  000cb840: vldr.32 s4,[sp,#0x148]
  000cb844: vldr.32 s10,[sp,#0x120]
  000cb848: vadd.f32 s2,s8,s2
  000cb84c: vadd.f32 s4,s10,s4
  000cb850: vstr.32 s0,[sp,#0x1c0]
  000cb854: vstr.32 s2,[sp,#0x1c4]
  000cb858: vstr.32 s4,[sp,#0x1c8]
  000cb85c: ldr r0,[r5,#0x0]
  000cb85e: ldr r2,[r0,#0x44]
  000cb860: mov r0,r5
  000cb862: blx r2
  000cb864: ldr r0,[r5,#0x4]
  000cb866: movs r1,#0x8
  000cb868: str r1,[r5,#0x28]
  000cb86a: movs r1,#0x1
  000cb86c: ldr r5,[sp,#0x3c]
  000cb86e: blx 0x00073e94
  000cb872: add.w r9,r9,#0x1c
  000cb876: adds r5,#0x1
  000cb878: cmp.w r9,#0x150
  000cb87c: bne 0x000cb796
  000cb87e: vmov.f32 s0,0x3f000000
  000cb882: vldr.32 s2,[sp,#0x120]
  000cb886: movs r0,#0x18
  000cb888: mov.w r8,#0x0
  000cb88c: vmul.f32 s0,s2,s0
  000cb890: vcvt.s32.f32 s0,s0
  000cb894: strd r8,r8,[sp,#0x1c0]
  000cb898: vstr.32 s0,[sp,#0x1c8]
  000cb89c: blx 0x0006eb24
  000cb8a0: mov r5,r0
  000cb8a2: add r1,sp,#0x1c0
  000cb8a4: movs r2,#0x3
  000cb8a6: blx 0x00073eb8
  000cb8aa: ldr r0,[sp,#0x48]
  000cb8ac: mov.w r9,#0x1
  000cb8b0: str.w r5,[r0,#0x110]
  000cb8b4: movs r5,#0x6
  000cb8b6: ldr r0,[0x000cb8dc]
  000cb8b8: ldr r6,[sp,#0x48]
  000cb8ba: add r0,pc
  000cb8bc: ldr.w r10,[r0,#0x0]
  000cb8c0: b 0x000cb926
  000cb8e0: ldr.w r0,[r10,#0x0]
  000cb8e4: movs r1,#0x8
  000cb8e6: blx 0x00071a34
  000cb8ea: mov r4,r0
  000cb8ec: ldr.w r0,[r6,#0x110]
  000cb8f0: blx 0x000728bc
  000cb8f4: strd r0,r9,[sp,#0x0]
  000cb8f8: mov r0,r6
  000cb8fa: movs r1,#0x8
  000cb8fc: movs r2,#0x0
  000cb8fe: mov r3,r4
  000cb900: str.w r8,[sp,#0x8]
  000cb904: blx 0x00073e88
  000cb908: mov r4,r0
  000cb90a: ldr r0,[r0,#0x4]
  000cb90c: movs r1,#0x1
  000cb90e: blx 0x00073e94
  000cb912: ldr r0,[r4,#0x4]
  000cb914: movs r1,#0x0
  000cb916: blx 0x00073fb4
  000cb91a: ldr.w r1,[r6,#0xf8]
  000cb91e: mov r0,r4
  000cb920: blx 0x000740f8
  000cb924: subs r5,#0x1
  000cb926: cmp r5,#0x0
  000cb928: bne 0x000cb8e0
  000cb92a: movs r5,#0x3
  000cb92c: mov.w r8,#0x0
  000cb930: mov.w r9,#0x1
  000cb934: ldr r6,[sp,#0x48]
  000cb936: b 0x000cb972
  000cb938: ldr.w r0,[r6,#0x110]
  000cb93c: blx 0x000728bc
  000cb940: strd r0,r9,[sp,#0x0]
  000cb944: mov r0,r6
  000cb946: movs r1,#0x0
  000cb948: movs r2,#0x0
  000cb94a: movs r3,#0x11
  000cb94c: str.w r8,[sp,#0x8]
  000cb950: blx 0x00073e88
  000cb954: mov r4,r0
  000cb956: ldr r0,[r0,#0x4]
  000cb958: movs r1,#0x1
  000cb95a: blx 0x00073fb4
  000cb95e: ldr r0,[r4,#0x4]
  000cb960: movs r1,#0x0
  000cb962: blx 0x00073e94
  000cb966: ldr.w r1,[r6,#0xf8]
  000cb96a: mov r0,r4
  000cb96c: blx 0x000740f8
  000cb970: subs r5,#0x1
  000cb972: cmp r5,#0x0
  000cb974: bne 0x000cb938
  000cb976: ldr r6,[sp,#0x48]
  000cb978: movs r2,#0x0
  000cb97a: ldr r1,[0x000cbd44]
  000cb97c: movs r4,#0x0
  000cb97e: ldr r3,[0x000cbd48]
  000cb980: ldr.w r0,[r6,#0xf0]
  000cb984: blx 0x000725bc
  000cb988: ldr.w r0,[r6,#0xf0]
  000cb98c: mov.w r1,#0x3f800000
  000cb990: add r2,sp,#0xd0
  000cb992: ldr r0,[r0,#0x8]
  000cb994: str r4,[sp,#0xf8]
  000cb996: str r1,[sp,#0xf4]
  000cb998: str r1,[sp,#0xfc]
  000cb99a: strd r4,r1,[sp,#0xd0]
  000cb99e: add r1,sp,#0xf4
  000cb9a0: str r4,[sp,#0xd8]
  000cb9a2: blx 0x000726ac
  000cb9a6: movs r0,#0x1c
  000cb9a8: blx 0x0006eb24
  000cb9ac: mov r5,r0
  000cb9ae: movs r1,#0x16
  000cb9b0: movs r2,#0x0
  000cb9b2: mov r3,r6
  000cb9b4: blx 0x00073edc
  000cb9b8: ldr r0,[sp,#0x48]
  000cb9ba: str r5,[r0,#0x28]
  000cb9bc: ldr r0,[sp,#0x44]
  000cb9be: ldr r0,[r0,#0x0]
  000cb9c0: blx 0x00071770
  000cb9c4: ldr r4,[sp,#0x48]
  000cb9c6: mov r1,r0
  000cb9c8: mov r0,r4
  000cb9ca: blx 0x000741c4
  000cb9ce: str.w r0,[r4,#0x114]
  000cb9d2: ldr r0,[sp,#0x1cc]
  000cb9d4: ldr r1,[sp,#0x40]
  000cb9d6: ldr r1,[r1,#0x0]
  000cb9d8: subs r0,r1,r0
  000cb9da: itttt eq
  000cb9dc: add.eq sp,#0x1d0
  000cb9de: vpop.eq {d8,d9,d10,d11,d12,d13}
  000cb9e2: add.eq sp,#0x4
  000cb9e4: pop.eq.w {r8,r9,r10,r11}
  000cb9e8: it eq
  000cb9ea: pop.eq {r4,r5,r6,r7,pc}
  000cb9ec: blx 0x0006e824
  000cb9f0: movs r1,#0x0
  000cb9f2: ldr r0,[0x000cbd4c]
  000cb9f4: strd r1,r1,[sp,#0x180]
  000cb9f8: add r1,sp,#0x180
  000cb9fa: str r0,[sp,#0x188]
  000cb9fc: ldr r0,[sp,#0x48]
  000cb9fe: add.w r0,r0,#0x18c
  000cba02: blx 0x0006eb3c
  000cba06: movs r0,#0xc
  000cba08: blx 0x0006eb24
  000cba0c: mov r5,r0
  000cba0e: blx 0x00073d08
  000cba12: ldr r0,[sp,#0x48]
  000cba14: mov r1,r5
  000cba16: str.w r5,[r0,#0xf8]
  000cba1a: movs r0,#0x4
  000cba1c: blx 0x00073d14
  000cba20: ldr r5,[sp,#0x48]
  000cba22: movs r4,#0x0
  000cba24: mov.w r8,#0x1
  000cba28: movs r6,#0x0
  000cba2a: b 0x000cba6e
  000cba2c: mov r0,r5
  000cba2e: movs r1,#0x1
  000cba30: movs r2,#0x0
  000cba32: movs r3,#0x9
  000cba34: strd r4,r8,[sp,#0x0]
  000cba38: str r4,[sp,#0x8]
  000cba3a: blx 0x00073e88
  000cba3e: ldr.w r1,[r5,#0xf8]
  000cba42: ldr r1,[r1,#0x4]
  000cba44: str.w r0,[r1,r6,lsl #0x2]
  000cba48: movs r1,#0x0
  000cba4a: ldr.w r0,[r5,#0xf8]
  000cba4e: ldr r0,[r0,#0x4]
  000cba50: ldr.w r0,[r0,r6,lsl #0x2]
  000cba54: ldr r0,[r0,#0x4]
  000cba56: blx 0x00073e94
  000cba5a: ldr.w r0,[r5,#0xf8]
  000cba5e: movs r1,#0x1
  000cba60: ldr r0,[r0,#0x4]
  000cba62: ldr.w r0,[r0,r6,lsl #0x2]
  000cba66: ldr r0,[r0,#0x4]
  000cba68: blx 0x00073fb4
  000cba6c: adds r6,#0x1
  000cba6e: ldr r0,[sp,#0x48]
  000cba70: ldr.w r0,[r0,#0xf8]
  000cba74: ldr r0,[r0,#0x0]
  000cba76: cmp r6,r0
  000cba78: bcc 0x000cba2c
  000cba7a: b 0x000cb9bc
  000cba7c: movs r1,#0x0
  000cba7e: ldr r0,[0x000cbd4c]
  000cba80: strd r1,r1,[sp,#0x180]
  000cba84: add r1,sp,#0x180
  000cba86: str r0,[sp,#0x188]
  000cba88: ldr r0,[sp,#0x48]
  000cba8a: add.w r10,r0,#0x18c
  000cba8e: mov r0,r10
  000cba90: blx 0x0006eb3c
  000cba94: ldr r0,[sp,#0x48]
  000cba96: vldr.32 s0,[r0,#0x18c]
  000cba9a: vcvt.s32.f32 s0,s0
  000cba9e: vstr.32 s0,[sp,#0x180]
  000cbaa2: vldr.32 s0,[r0,#0x190]
  000cbaa6: vcvt.s32.f32 s0,s0
  000cbaaa: vstr.32 s0,[sp,#0x184]
  000cbaae: vldr.32 s0,[r0,#0x194]
  000cbab2: movs r0,#0x18
  000cbab4: vcvt.s32.f32 s0,s0
  000cbab8: vstr.32 s0,[sp,#0x188]
  000cbabc: blx 0x0006eb24
  000cbac0: mov r6,r0
  000cbac2: add r1,sp,#0x180
  000cbac4: movs r2,#0x3
  000cbac6: blx 0x00073eb8
  000cbaca: ldr r0,[sp,#0x48]
  000cbacc: movs r1,#0x1
  000cbace: mov.w r8,#0x1
  000cbad2: str.w r6,[r0,#0x110]
  000cbad6: mov r0,r6
  000cbad8: blx 0x00073f48
  000cbadc: movs r0,#0xc
  000cbade: blx 0x0006eb24
  000cbae2: mov r6,r0
  000cbae4: blx 0x00073d08
  000cbae8: ldr r5,[sp,#0x48]
  000cbaea: movs r0,#0x6
  000cbaec: mov r1,r6
  000cbaee: str.w r6,[r5,#0xf8]
  000cbaf2: blx 0x00073d14
  000cbaf6: movs r6,#0x0
  000cbaf8: mov r0,r5
  000cbafa: movs r1,#0x1
  000cbafc: movs r2,#0x1
  000cbafe: movs r3,#0xd
  000cbb00: strd r6,r8,[sp,#0x0]
  000cbb04: str r6,[sp,#0x8]
  000cbb06: blx 0x00073e88
  000cbb0a: ldr.w r1,[r5,#0xf8]
  000cbb0e: ldr r1,[r1,#0x4]
  000cbb10: str r0,[r1,#0x0]
  000cbb12: movs r1,#0x1
  000cbb14: ldr.w r0,[r5,#0xf8]
  000cbb18: ldr r0,[r0,#0x4]
  000cbb1a: ldr r0,[r0,#0x0]
  000cbb1c: ldr r0,[r0,#0x4]
  000cbb1e: blx 0x00073e94
  000cbb22: add r4,sp,#0xf4
  000cbb24: ldr.w r1,[r5,#0xf0]
  000cbb28: mov r0,r4
  000cbb2a: blx 0x0007264c
  000cbb2e: add.w r9,sp,#0x1c0
  000cbb32: mov r1,r4
  000cbb34: mov r2,r10
  000cbb36: mov r0,r9
  000cbb38: blx 0x0006ec38
  000cbb3c: add.w r11,sp,#0x118
  000cbb40: ldr r2,[0x000cbd50]
  000cbb42: mov r1,r9
  000cbb44: mov r0,r11
  000cbb46: blx 0x00073024
  000cbb4a: add r4,sp,#0x140
  000cbb4c: mov r1,r10
  000cbb4e: mov r2,r11
  000cbb50: mov r0,r4
  000cbb52: blx 0x0006f1cc
  000cbb56: mov r0,r10
  000cbb58: mov r1,r4
  000cbb5a: blx 0x0006eb3c
  000cbb5e: ldr.w r0,[r5,#0xf8]
  000cbb62: add.w r3,r5,#0x18c
  000cbb66: ldmia r3,{r1,r2,r3}
  000cbb68: ldr r0,[r0,#0x4]
  000cbb6a: ldr r0,[r0,#0x0]
  000cbb6c: ldr r5,[r0,#0x0]
  000cbb6e: ldr r5,[r5,#0x48]
  000cbb70: blx r5
  000cbb72: movs r5,#0x1
  000cbb74: b 0x000cbbca
  000cbb76: ldr r4,[sp,#0x48]
  000cbb78: ldr.w r0,[r4,#0x110]
  000cbb7c: blx 0x000728bc
  000cbb80: strd r0,r8,[sp,#0x0]
  000cbb84: mov r0,r4
  000cbb86: movs r1,#0x1
  000cbb88: movs r2,#0x0
  000cbb8a: movs r3,#0x9
  000cbb8c: str r6,[sp,#0x8]
  000cbb8e: blx 0x00073e88
  000cbb92: ldr.w r1,[r4,#0xf8]
  000cbb96: ldr r1,[r1,#0x4]
  000cbb98: str.w r0,[r1,r5,lsl #0x2]
  000cbb9c: movs r1,#0x1
  000cbb9e: ldr.w r0,[r4,#0xf8]
  000cbba2: ldr r0,[r0,#0x4]
  000cbba4: ldr.w r0,[r0,r5,lsl #0x2]
  000cbba8: ldr r0,[r0,#0x4]
  000cbbaa: blx 0x00073e94
  000cbbae: ldr.w r1,[r4,#0xf8]
  000cbbb2: ldr.w r0,[r4,#0x110]
  000cbbb6: ldr r1,[r1,#0x4]
  000cbbb8: ldr.w r4,[r1,r5,lsl #0x2]
  000cbbbc: blx 0x00073f3c
  000cbbc0: mov r1,r0
  000cbbc2: mov r0,r4
  000cbbc4: blx 0x00073f60
  000cbbc8: adds r5,#0x1
  000cbbca: ldr r0,[sp,#0x48]
  000cbbcc: ldr.w r0,[r0,#0xf8]
  000cbbd0: ldr r0,[r0,#0x0]
  000cbbd2: cmp r5,r0
  000cbbd4: bcc 0x000cbb76
  000cbbd6: b 0x000cb9bc
  000cbbd8: movs r1,#0x0
  000cbbda: ldr r0,[0x000cbd4c]
  000cbbdc: strd r1,r1,[sp,#0x180]
  000cbbe0: add r1,sp,#0x180
  000cbbe2: str r0,[sp,#0x188]
  000cbbe4: ldr r0,[sp,#0x48]
  000cbbe6: add.w r10,r0,#0x18c
  000cbbea: mov r0,r10
  000cbbec: blx 0x0006eb3c
  000cbbf0: ldr r0,[sp,#0x48]
  000cbbf2: vldr.32 s0,[r0,#0x18c]
  000cbbf6: vcvt.s32.f32 s0,s0
  000cbbfa: vstr.32 s0,[sp,#0x180]
  000cbbfe: vldr.32 s0,[r0,#0x190]
  000cbc02: vcvt.s32.f32 s0,s0
  000cbc06: vstr.32 s0,[sp,#0x184]
  000cbc0a: vldr.32 s0,[r0,#0x194]
  000cbc0e: movs r0,#0x18
  000cbc10: vcvt.s32.f32 s0,s0
  000cbc14: vstr.32 s0,[sp,#0x188]
  000cbc18: blx 0x0006eb24
  000cbc1c: mov r6,r0
  000cbc1e: add r1,sp,#0x180
  000cbc20: movs r2,#0x3
  000cbc22: blx 0x00073eb8
  000cbc26: ldr r0,[sp,#0x48]
  000cbc28: movs r1,#0x1
  000cbc2a: mov.w r8,#0x1
  000cbc2e: str.w r6,[r0,#0x110]
  000cbc32: mov r0,r6
  000cbc34: blx 0x00073f48
  000cbc38: movs r0,#0xc
  000cbc3a: blx 0x0006eb24
  000cbc3e: mov r6,r0
  000cbc40: blx 0x00073d08
  000cbc44: ldr r4,[sp,#0x48]
  000cbc46: movs r0,#0x8
  000cbc48: mov r1,r6
  000cbc4a: str.w r6,[r4,#0xf8]
  000cbc4e: blx 0x00073d14
  000cbc52: movs r6,#0x0
  000cbc54: mov r0,r4
  000cbc56: movs r1,#0x1
  000cbc58: movs r2,#0x1
  000cbc5a: movs r3,#0xd
  000cbc5c: strd r6,r8,[sp,#0x0]
  000cbc60: str r6,[sp,#0x8]
  000cbc62: blx 0x00073e88
  000cbc66: ldr.w r1,[r4,#0xf8]
  000cbc6a: ldr r1,[r1,#0x4]
  000cbc6c: str r0,[r1,#0x0]
  000cbc6e: movs r1,#0x1
  000cbc70: ldr.w r0,[r4,#0xf8]
  000cbc74: ldr r0,[r0,#0x4]
  000cbc76: ldr r0,[r0,#0x0]
  000cbc78: ldr r0,[r0,#0x4]
  000cbc7a: blx 0x00073e94
  000cbc7e: add r5,sp,#0xf4
  000cbc80: ldr.w r1,[r4,#0xf0]
  000cbc84: mov r0,r5
  000cbc86: blx 0x0007264c
  000cbc8a: add.w r9,sp,#0x1c0
  000cbc8e: mov r1,r5
  000cbc90: mov r2,r10
  000cbc92: mov r0,r9
  000cbc94: blx 0x0006ec38
  000cbc98: add.w r11,sp,#0x118
  000cbc9c: mov r1,r9
  000cbc9e: mov.w r2,#0x40000000
  000cbca2: mov r0,r11
  000cbca4: blx 0x00073024
  000cbca8: add r5,sp,#0x140
  000cbcaa: mov r1,r10
  000cbcac: mov r2,r11
  000cbcae: mov r0,r5
  000cbcb0: blx 0x0006f1cc
  000cbcb4: mov r0,r10
  000cbcb6: mov r1,r5
  000cbcb8: blx 0x0006eb3c
  000cbcbc: ldr.w r0,[r4,#0xf8]
  000cbcc0: add.w r3,r4,#0x18c
  000cbcc4: ldmia r3,{r1,r2,r3}
  000cbcc6: ldr r0,[r0,#0x4]
  000cbcc8: ldr r0,[r0,#0x0]
  000cbcca: ldr r5,[r0,#0x0]
  000cbccc: ldr r5,[r5,#0x48]
  000cbcce: blx r5
  000cbcd0: mov r0,r4
  000cbcd2: movs r1,#0x1
  000cbcd4: movs r2,#0x1
  000cbcd6: movs r3,#0xd
  000cbcd8: strd r6,r8,[sp,#0x0]
  000cbcdc: str r6,[sp,#0x8]
  000cbcde: blx 0x00073e88
  000cbce2: ldr.w r1,[r4,#0xf8]
  000cbce6: ldr r1,[r1,#0x4]
  000cbce8: str r0,[r1,#0x4]
  000cbcea: movs r1,#0x1
  000cbcec: ldr.w r0,[r4,#0xf8]
  000cbcf0: ldr r0,[r0,#0x4]
  000cbcf2: ldr r0,[r0,#0x4]
  000cbcf4: ldr r0,[r0,#0x4]
  000cbcf6: blx 0x00073e94
  000cbcfa: vldr.32 s0,[pc,#0x58]
  000cbcfe: vldr.32 s2,[r4,#0x18c]
  000cbd02: vldr.32 s4,[r4,#0x190]
  000cbd06: vldr.32 s8,[pc,#0x50]
  000cbd0a: vadd.f32 s2,s2,s0
  000cbd0e: vldr.32 s6,[r4,#0x194]
  000cbd12: vadd.f32 s4,s4,s8
  000cbd16: ldr.w r0,[r4,#0xf8]
  000cbd1a: vadd.f32 s0,s6,s0
  000cbd1e: vmov r1,s2
  000cbd22: vstr.32 s2,[r4,#0x18c]
  000cbd26: vmov r2,s4
  000cbd2a: vstr.32 s4,[r4,#0x190]
  000cbd2e: vmov r3,s0
  000cbd32: vstr.32 s0,[r4,#0x194]
  000cbd36: ldr r0,[r0,#0x4]
  000cbd38: ldr r0,[r0,#0x4]
  000cbd3a: ldr r5,[r0,#0x0]
  000cbd3c: ldr r5,[r5,#0x48]
  000cbd3e: blx r5
  000cbd40: movs r5,#0x2
  000cbd42: b 0x000cbdb0
  000cbd5c: ldr r4,[sp,#0x48]
  000cbd5e: ldr.w r0,[r4,#0x110]
  000cbd62: blx 0x000728bc
  000cbd66: strd r0,r8,[sp,#0x0]
  000cbd6a: mov r0,r4
  000cbd6c: movs r1,#0x1
  000cbd6e: movs r2,#0x0
  000cbd70: movs r3,#0x9
  000cbd72: str r6,[sp,#0x8]
  000cbd74: blx 0x00073e88
  000cbd78: ldr.w r1,[r4,#0xf8]
  000cbd7c: ldr r1,[r1,#0x4]
  000cbd7e: str.w r0,[r1,r5,lsl #0x2]
  000cbd82: movs r1,#0x1
  000cbd84: ldr.w r0,[r4,#0xf8]
  000cbd88: ldr r0,[r0,#0x4]
  000cbd8a: ldr.w r0,[r0,r5,lsl #0x2]
  000cbd8e: ldr r0,[r0,#0x4]
  000cbd90: blx 0x00073e94
  000cbd94: ldr.w r1,[r4,#0xf8]
  000cbd98: ldr.w r0,[r4,#0x110]
  000cbd9c: ldr r1,[r1,#0x4]
  000cbd9e: ldr.w r4,[r1,r5,lsl #0x2]
  000cbda2: blx 0x00073f3c
  000cbda6: mov r1,r0
  000cbda8: mov r0,r4
  000cbdaa: blx 0x00073f60
  000cbdae: adds r5,#0x1
  000cbdb0: ldr r0,[sp,#0x48]
  000cbdb2: ldr.w r0,[r0,#0xf8]
  000cbdb6: ldr r0,[r0,#0x0]
  000cbdb8: cmp r5,r0
  000cbdba: bcc 0x000cbd5c
  000cbdbc: b 0x000cb9bc
```
