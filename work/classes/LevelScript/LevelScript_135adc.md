# LevelScript::LevelScript
elf 0x135adc body 7612
Sig: undefined __thiscall LevelScript(LevelScript * this, Level * param_1, Hud * param_2, Radar * param_3, TargetFollowCamera * param_4)

## decompile
```c

/* LevelScript::LevelScript(Level*, Hud*, Radar*, TargetFollowCamera*) */

void __thiscall
LevelScript::LevelScript
          (LevelScript *this,Level *param_1,Hud *param_2,Radar *param_3,TargetFollowCamera *param_4)

{
  char cVar1;
  bool bVar2;
  bool bVar3;
  float *this_00;
  int *piVar4;
  int iVar5;
  undefined4 *puVar6;
  PlayerEgo *pPVar7;
  undefined4 uVar8;
  Explosion *this_01;
  AEGeometry *pAVar9;
  TargetFollowCamera *pTVar10;
  MGame *pMVar11;
  AEGeometry *this_02;
  AEGeometry *pAVar12;
  AEGeometry *this_03;
  Transform *this_04;
  Station *pSVar13;
  int iVar14;
  Vector *pVVar15;
  float *pfVar16;
  KIPlayer *pKVar17;
  Route *this_05;
  code *pcVar18;
  code *pcVar19;
  undefined4 uVar20;
  code *pcVar21;
  int iVar22;
  char *pcVar23;
  uint uVar24;
  int *piVar25;
  Vector *pVVar26;
  uint *puVar27;
  Player *pPVar28;
  int *piVar29;
  int *piVar30;
  uint in_fpscr;
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
  float fVar31;
  float extraout_s0_11;
  float extraout_s0_12;
  float extraout_s0_13;
  float extraout_s0_14;
  float extraout_s0_15;
  float fVar32;
  float extraout_s0_16;
  float extraout_s0_17;
  float extraout_s0_18;
  float extraout_s0_19;
  float extraout_s0_20;
  float extraout_s0_21;
  float extraout_s0_22;
  float extraout_s0_23;
  float extraout_s0_24;
  float extraout_s0_25;
  float extraout_s0_26;
  float extraout_s0_27;
  float extraout_s0_28;
  float extraout_s0_29;
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
  float extraout_s1_23;
  float extraout_s1_24;
  float extraout_s1_25;
  float extraout_s1_26;
  float extraout_s1_27;
  float extraout_s1_28;
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
  float extraout_s2_18;
  float extraout_s2_19;
  float extraout_s2_20;
  float extraout_s2_21;
  float extraout_s2_22;
  float extraout_s2_23;
  float extraout_s2_24;
  float extraout_s2_25;
  float fVar33;
  float extraout_s2_26;
  float extraout_s2_27;
  float extraout_s2_28;
  int *piVar34;
  undefined4 local_120;
  undefined4 uStack_11c;
  undefined4 uStack_118;
  undefined4 local_114;
  undefined4 local_110;
  undefined4 local_10c;
  float local_d8;
  undefined4 local_d4;
  float local_d0;
  undefined4 local_cc;
  undefined4 uStack_c8;
  undefined4 local_c4;
  undefined4 uStack_c0;
  undefined4 local_bc;
  undefined4 uStack_b8;
  undefined4 local_b4;
  undefined4 uStack_b0;
  undefined4 local_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined1 auStack_9c [12];
  undefined1 auStack_90 [12];
  undefined1 auStack_84 [12];
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  piVar34 = (int *)0x0;
  piVar30 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  piVar25 = *(int **)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  pPVar7 = *(PlayerEgo **)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar29 = *(int **)(DAT_00145e64 + 0x145af8);
  local_3c = *piVar29;
  this->field_48 = 0;
  this->field_38 = 0;
  this->field_3C = (int)piVar30;
  this->field_40 = (int)piVar25;
  this->field_44 = (int)pPVar7;
  this_00 = &this->m_nStreamTimer;
  *this_00 = 0.0;
  this->m_nStreamOffsetX = (float)piVar30;
  this->m_nStreamOffsetZ = (float)piVar25;
  this->field_34 = (int)pPVar7;
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)this->m_pMatrix);
  this[1].field_4 = (int)param_2;
  this[1].field_8 = (int)param_3;
  this->m_pCamera = (TargetFollowCamera *)param_4;
  this->m_pLevel = (MGame *)param_1;
  Hud::drawTitleImage(SUB41(param_2,0));
  this->m_nState = 0;
  *(undefined2 *)&this->field_20 = 1;
  *(undefined2 *)&this->field_10 = 0x100;
  piVar4 = (int *)Level::getTimeLimit(param_1);
  *(int **)((int)&this->field_98 + 2) = piVar34;
  *(int **)((int)&this->m_pAlien0 + 2) = piVar30;
  *(int **)((int)&this->m_pAlien1 + 2) = piVar25;
  *(PlayerEgo **)((int)&this->m_pAlien2 + 2) = pPVar7;
  this->field_8C = (float)piVar34;
  this->field_90 = (int)piVar30;
  this->field_94 = (int)piVar25;
  this->field_98 = (int)pPVar7;
  this->m_pAsteroids = piVar34;
  this->m_pExtra = piVar30;
  this->m_nParticleSystem0 = (int)piVar25;
  this->m_nParticleSystem1 = (int)pPVar7;
  this->m_pGeometriesMission2 = piVar34;
  this->m_pExplosion = piVar30;
  this->m_pMission = piVar25;
  this->m_pPlayerEgo = pPVar7;
  this->field_8 = 0;
  this->field_C = 0;
  this->field_0 = piVar4;
  this->field_24 = 0;
  this[1].field_C = 0;
  this[1].field_10 = 0;
  *(undefined2 *)((int)&this->field_10 + 2) = 0;
  this[1].field_0 = (int *)0x0;
  bVar2 = SUB41(param_4,0);
  TargetFollowCamera::setLookAtCam(bVar2);
  piVar34 = &this->field_90;
  iVar5 = Level::getPlayer();
  if (iVar5 == 0) {
    *(undefined1 *)&this->field_20 = 0;
  }
  else {
    puVar6 = (undefined4 *)Level::getPlayer();
    Player::setVulnerable(SUB41(*puVar6,0));
  }
  pPVar7 = (PlayerEgo *)Level::getPlayer();
  PlayerEgo::setCollide(pPVar7,false);
  piVar30 = *(int **)(DAT_00145e68 + 0x145bd0);
  iVar5 = Status::getCurrentCampaignMission();
  if (iVar5 == 0) {
    puVar6 = (undefined4 *)(DAT_00145e88 + 0x145c94);
    *piVar34 = 0;
    this->field_94 = 0;
    pcVar18 = (code *)*puVar6;
    param_2[1] = (Hud)0x0;
    param_3[0x48] = (Radar)0x0;
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setComputerControlled(pPVar7,true);
    puVar6 = (undefined4 *)(*pcVar18)(param_1);
    Player::setVulnerable(SUB41(*puVar6,0));
    *(undefined1 *)((int)&this->field_20 + 1) = 1;
    TargetFollowCamera::setLookAtCam(bVar2);
    local_d8 = DAT_00145e90;
    local_d4 = DAT_00145e8c;
    local_d0 = (float)DAT_00145e94;
    TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_d8);
    (*pcVar18)(param_1);
    PlayerEgo::setPosition(extraout_s0_02,extraout_s1_02,extraout_s2_02);
    iVar5 = (*pcVar18)(param_1);
    local_d8 = 0.0;
    local_d4 = 0;
    local_d0 = 1.0;
    local_114 = 0;
    local_110 = 0x3f800000;
    local_10c = 0;
    AEGeometry::setDirection(*(Vector **)(iVar5 + 8),(Vector *)&local_d8);
    pcVar18 = *(code **)(DAT_00145e9c + 0x145d0e);
    for (iVar5 = 0; iVar5 != 3; iVar5 = iVar5 + 1) {
      iVar22 = (*pcVar18)(param_1);
      KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(iVar22 + 4) + iVar5 * 4));
      iVar22 = (*pcVar18)(param_1);
      AEGeometry::setVisible
                (SUB41(*(undefined4 *)(*(int *)(*(int *)(iVar22 + 4) + iVar5 * 4) + 8),0));
      iVar22 = (*pcVar18)(param_1);
      *(undefined4 *)(*(int *)(*(int *)(iVar22 + 4) + iVar5 * 4) + 0x128) = 0;
      iVar22 = (*pcVar18)(param_1);
      *(undefined1 *)(*(int *)(*(int *)(*(int *)(iVar22 + 4) + iVar5 * 4) + 4) + 0x5c) = 0;
    }
    LODManager::forceUpdate(*(LODManager **)param_1,0x1e,false);
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar9,0x3ab3,(PaintCanvas *)**(undefined4 **)(DAT_00145ea0 + 0x145d6e),false);
    this[1].field_C = (int)pAVar9;
    goto LAB_00146406;
  }
  iVar5 = Status::getCurrentCampaignMission();
  if (iVar5 == 1) {
    puVar6 = (undefined4 *)(DAT_00145e6c + 0x145bf8);
    param_2[1] = (Hud)0x0;
    pcVar18 = (code *)*puVar6;
    param_3[0x48] = (Radar)0x0;
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setRotation(pPVar7,extraout_s0,extraout_s1,extraout_s2);
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setFreeze(pPVar7,true);
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setComputerControlled(pPVar7,true);
    (*pcVar18)(param_1);
    PlayerEgo::setPosition(extraout_s0_00,extraout_s1_00,extraout_s2_00);
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setExhaustVisible(pPVar7,false);
    (*pcVar18)(param_1);
    PlayerEgo::StopEngineSound();
    iVar5 = Level::getEnemies();
    KIPlayer::StopEngineSound((KIPlayer *)**(undefined4 **)(iVar5 + 4));
    TargetFollowCamera::setPosition
              ((TargetFollowCamera *)param_4,extraout_s0_01,extraout_s1_01,extraout_s2_01);
    *(undefined1 *)((int)&this->field_20 + 1) = 1;
    TargetFollowCamera::setLookAtCam(bVar2);
    (*pcVar18)(param_1);
    TargetFollowCamera::setTarget((AEGeometry *)param_4);
    Layout::startFade((Layout *)**(undefined4 **)(DAT_00145e84 + 0x145c84),false,0xff,5000);
    goto LAB_00146406;
  }
  iVar5 = Status::getCurrentCampaignMission();
  if ((iVar5 == 0x51) && (iVar5 = Status::inAlienOrbit(), iVar5 != 0)) {
    puVar6 = (undefined4 *)(DAT_00145ea4 + 0x145da8);
    *piVar34 = 0;
    this->field_94 = 0;
    pcVar18 = (code *)*puVar6;
    param_2[1] = (Hud)0x0;
    param_3[0x48] = (Radar)0x0;
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setComputerControlled(pPVar7,true);
    iVar5 = (*pcVar18)(param_1);
    *(undefined1 *)(iVar5 + 0x24) = 1;
    puVar6 = (undefined4 *)(*pcVar18)(param_1);
    Player::setVulnerable(SUB41(*puVar6,0));
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setVisible(pPVar7,false);
    iVar5 = **(int **)(DAT_00145ea8 + 0x145de6);
    (*pcVar18)(param_1);
    FModSound::stop(iVar5);
    *(undefined1 *)((int)&this->field_20 + 1) = 1;
    TargetFollowCamera::setLookAtCam(bVar2);
    local_d8 = DAT_00145eb0;
    local_d4 = DAT_00145eac;
    local_d0 = DAT_00145eb4;
    TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_d8);
    (*pcVar18)(param_1);
    PlayerEgo::setPosition(extraout_s0_03,extraout_s1_03,extraout_s2_03);
    iVar5 = Level::getEnemies();
    AEGeometry::setVisible(SUB41(*(undefined4 *)(**(int **)(iVar5 + 4) + 0x140),0));
    LODManager::forceUpdate(*(LODManager **)param_1,0x1e,false);
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar9,0x3ab3,(PaintCanvas *)**(undefined4 **)(DAT_00145eb8 + 0x145e50),false);
    fVar31 = extraout_s0_04;
    fVar32 = extraout_s1_04;
    fVar33 = extraout_s2_04;
  }
  else {
    iVar5 = Status::getCurrentCampaignMission();
    if (iVar5 != 0x4e) {
      iVar5 = Status::getCurrentCampaignMission();
      if (iVar5 == 0x59) {
        puVar6 = (undefined4 *)(DAT_001462b4 + 0x145f5a);
        *piVar34 = 0;
        this->field_94 = 0;
        pcVar18 = (code *)*puVar6;
        param_2[1] = (Hud)0x0;
        param_3[0x48] = (Radar)0x0;
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setComputerControlled(pPVar7,true);
        iVar5 = (*pcVar18)(param_1);
        *(undefined1 *)(iVar5 + 0x24) = 1;
        puVar6 = (undefined4 *)(*pcVar18)(param_1);
        Player::setVulnerable(SUB41(*puVar6,0));
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setVisible(pPVar7,false);
        iVar5 = **(int **)(DAT_001462b8 + 0x145f98);
        (*pcVar18)(param_1);
        FModSound::stop(iVar5);
        *(undefined1 *)((int)&this->field_20 + 1) = 1;
        TargetFollowCamera::setLookAtCam(bVar2);
        pAVar9 = operator_new(0xc0);
        puVar6 = *(undefined4 **)(DAT_001462bc + 0x145fc0);
        AEGeometry::AEGeometry(pAVar9,(PaintCanvas *)*puVar6);
        iVar5 = DAT_001462c0;
        this[1].field_0 = (int *)pAVar9;
        pcVar19 = *(code **)(iVar5 + 0x145fd4);
        iVar5 = (*pcVar19)(param_1);
        (**(code **)(**(int **)(*(int *)(iVar5 + 4) + 4) + 0x28))((Vector *)&local_d8);
        AEGeometry::setPosition(pAVar9,(Vector *)&local_d8);
        pVVar26 = (Vector *)this[1].field_0;
        pcVar18 = *(code **)(DAT_001462c4 + 0x145ffc);
        uVar8 = (*pcVar18)(param_1);
        pcVar21 = *(code **)(DAT_001462c8 + 0x14600c);
        (*pcVar21)((Vector *)&local_114,uVar8);
        AbyssEngine::AEMath::operator-((AEMath *)&local_d8,(Vector *)&local_114);
        local_44 = 0x3f800000;
        local_48 = 0;
        local_40 = 0;
        fVar31 = (float)AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
        AEGeometry::moveForward(fVar31);
        pVVar26 = (Vector *)this[1].field_0;
        fVar31 = (float)AEGeometry::getRightVector();
        AbyssEngine::AEMath::operator*((Vector *)&local_d8,fVar31);
        AEGeometry::translate(pVVar26);
        TargetFollowCamera::setTarget((AEGeometry *)param_4);
        iVar5 = (*pcVar19)(param_1);
        (**(code **)(**(int **)(*(int *)(iVar5 + 4) + 4) + 0x28))((Vector *)&local_d8);
        TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_d8);
        TargetFollowCamera::translate
                  ((TargetFollowCamera *)param_4,extraout_s0_07,extraout_s1_07,extraout_s2_07);
        iVar5 = (*pcVar19)(param_1);
        pAVar9 = *(AEGeometry **)(**(int **)(iVar5 + 4) + 8);
        local_110 = DAT_001462e0;
        local_114 = DAT_001462e4;
        local_10c = DAT_001462e8;
        uVar8 = (*pcVar18)(param_1);
        fVar31 = (float)(*pcVar21)(&local_54,uVar8);
        AbyssEngine::AEMath::operator*(fVar31,(Vector *)&local_48);
        AbyssEngine::AEMath::operator+((Vector *)&local_d8,(Vector *)&local_114);
        AEGeometry::setPosition(pAVar9,(Vector *)&local_d8);
        iVar5 = (*pcVar19)(param_1);
        pVVar26 = *(Vector **)(**(int **)(iVar5 + 4) + 8);
        uVar8 = (*pcVar18)(param_1);
        (*pcVar21)((Vector *)&local_d8,uVar8);
        local_110 = 0x3f800000;
        local_114 = 0;
        local_10c = 0;
        AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
        iVar5 = (*pcVar19)(param_1);
        local_d8 = DAT_001462f0;
        local_d4 = 0;
        local_d0 = DAT_001462f0;
        AEGeometry::translate(*(Vector **)(**(int **)(iVar5 + 4) + 8));
        pAVar9 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar9,0x3795,(PaintCanvas *)*puVar6,false);
        this[1].field_10 = (int)pAVar9;
        AEGeometry::setScaling(extraout_s0_08,extraout_s1_08,extraout_s2_08);
        this->m_nState = 1;
        LODManager::forceUpdate(*(LODManager **)param_1,0x1e,false);
        goto LAB_00146406;
      }
      iVar5 = Status::getCurrentCampaignMission();
      if ((iVar5 == 0x69) &&
         (iVar22 = Status::inSupernovaOrbit(), iVar5 = DAT_001462f8, iVar22 != 0)) {
        *(undefined1 *)((int)&this->field_10 + 1) = 1;
        pcVar18 = *(code **)(iVar5 + 0x1461a2);
        param_2[1] = (Hud)0x0;
        param_3[0x48] = (Radar)0x0;
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setComputerControlled(pPVar7,true);
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setCollide(pPVar7,false);
        *(undefined2 *)&this->field_20 = 0x100;
        TargetFollowCamera::setLookAtCam(bVar2);
        (*pcVar18)(param_1);
        TargetFollowCamera::setTarget((AEGeometry *)param_4);
        iVar5 = Level::getEnemies();
        (**(code **)(**(int **)(*(int *)(iVar5 + 4) + 4) + 0x28))((Vector *)&local_d8);
        TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_d8);
        TargetFollowCamera::translate
                  ((TargetFollowCamera *)param_4,extraout_s0_09,extraout_s1_09,extraout_s2_09);
        (*pcVar18)(param_1);
        PlayerEgo::getPosition();
        (*pcVar18)(param_1);
        fVar31 = (float)PlayerEgo::GetDirVector();
        AbyssEngine::AEMath::operator*((Vector *)&local_54,fVar31);
        AbyssEngine::AEMath::operator+((Vector *)&local_114,(Vector *)&local_48);
        (*pcVar18)(param_1);
        fVar31 = (float)PlayerEgo::GetUpVector();
        AbyssEngine::AEMath::operator*((Vector *)&local_6c,fVar31);
        AbyssEngine::AEMath::operator+((Vector *)&local_d8,(Vector *)&local_114);
        TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_d8);
        puVar6 = (undefined4 *)(*pcVar18)(param_1);
        Player::setVulnerable(SUB41(*puVar6,0));
LAB_0014627e:
        if (**(char **)(DAT_00146308 + 0x146284) == '\0') {
          setAutoPilotToProgrammedStation();
        }
        this->m_nState = 1;
        *piVar34 = 0;
        this->field_94 = 0;
        goto LAB_00146406;
      }
      iVar5 = Status::getCurrentCampaignMission();
      if (iVar5 == 0x6a) {
        pcVar18 = *(code **)(DAT_001466b0 + 0x146320);
        (*pcVar18)(param_1);
        PlayerEgo::setPosition(extraout_s0_10,extraout_s1_10,extraout_s2_10);
        param_2[1] = (Hud)0x0;
        param_3[0x48] = (Radar)0x0;
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setComputerControlled(pPVar7,true);
        iVar5 = (*pcVar18)(param_1);
        *(undefined1 *)(iVar5 + 0x24) = 1;
        puVar6 = (undefined4 *)(*pcVar18)(param_1);
        Player::setVulnerable(SUB41(*puVar6,0));
        Level::getStarSystem();
        StarSystem::getLightDirection();
        AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
        iVar5 = (*pcVar18)(param_1);
        local_114 = 0;
        local_110 = 0x3f800000;
        local_10c = 0;
        AEGeometry::setDirection(*(Vector **)(iVar5 + 8),(Vector *)&local_d8);
        iVar5 = **(int **)(DAT_001466b8 + 0x146396);
        (*pcVar18)(param_1);
        FModSound::stop(iVar5);
        *(undefined1 *)((int)&this->field_20 + 1) = 1;
        TargetFollowCamera::setLookAtCam(bVar2);
        (*pcVar18)(param_1);
        PlayerEgo::getPosition();
        local_54 = DAT_001466c0;
        local_50 = DAT_001466bc;
        local_4c = DAT_001466c4;
        AbyssEngine::AEMath::operator+((Vector *)&local_114,(Vector *)&local_48);
        TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_114);
        iVar5 = DAT_001466c8;
        *(undefined1 *)((int)&this->field_10 + 1) = 1;
        Layout::startFade((Layout *)**(undefined4 **)(iVar5 + 0x1463f4),false,-1,8000);
        this->m_nState = 1;
        goto LAB_00146406;
      }
      iVar5 = Status::getCurrentCampaignMission();
      if (iVar5 == 0x90) {
        pAVar9 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar9,(PaintCanvas *)**(undefined4 **)(DAT_00146d50 + 0x146a38));
        this[1].field_0 = (int *)pAVar9;
        AEGeometry::setPosition(extraout_s0_13,extraout_s1_13,extraout_s2_13);
        local_d4 = 0;
        local_d8 = 1.0;
        local_d0 = (float)DAT_00146d60;
        local_114 = 0;
        local_110 = 0x3f800000;
        local_10c = 0;
        AEGeometry::setDirection((Vector *)this[1].field_0,(Vector *)&local_d8);
        iVar5 = DAT_00146d64;
        *(undefined1 *)((int)&this->field_10 + 1) = 1;
        pcVar18 = *(code **)(iVar5 + 0x146a84);
        param_2[1] = (Hud)0x0;
        param_3[0x48] = (Radar)0x0;
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setComputerControlled(pPVar7,true);
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setCollide(pPVar7,false);
        bVar3 = (bool)(*pcVar18)(param_1);
        PlayerEgo::setActive(bVar3);
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setVisible(pPVar7,false);
        pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
        PlayerEgo::setFreeze(pPVar7,true);
        iVar5 = DAT_00146d68;
        *(undefined2 *)&this->field_20 = 0x100;
        iVar5 = **(int **)(iVar5 + 0x146ad0);
        (*pcVar18)(param_1);
        FModSound::stop(iVar5);
        TargetFollowCamera::setLookAtCam(bVar2);
        TargetFollowCamera::setTarget((AEGeometry *)param_4);
        iVar5 = Level::getEnemies();
        (**(code **)(*(int *)**(undefined4 **)(iVar5 + 4) + 0x28))((Vector *)&local_d8);
        AbyssEngine::AEMath::Vector::operator=((Vector *)this_00,(Vector *)&local_d8);
        local_d8 = DAT_00146d70;
        local_d4 = DAT_00146d6c;
        local_d0 = DAT_00146d74;
        AbyssEngine::AEMath::Vector::operator+=((Vector *)this_00,(Vector *)&local_d8);
        TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)this_00);
        if (**(char **)(DAT_00146d78 + 0x146b32) == '\0') {
          setAutoPilotToProgrammedStation();
        }
        this->m_nState = 1;
        *piVar34 = 0;
        this->field_94 = 0;
        goto LAB_00146406;
      }
      iVar5 = Status::getCurrentCampaignMission();
      if ((iVar5 == 0x83) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
        pSVar13 = (Station *)Status::getStation();
        iVar22 = Station::getIndex(pSVar13);
        iVar5 = DAT_00146d7c;
        if (iVar22 == 0x70) {
          *(undefined1 *)((int)&this->field_10 + 1) = 1;
          pcVar18 = *(code **)(iVar5 + 0x146b88);
          param_2[1] = (Hud)0x0;
          param_3[0x48] = (Radar)0x0;
          pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
          PlayerEgo::setComputerControlled(pPVar7,true);
          pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
          PlayerEgo::setCollide(pPVar7,false);
          (*pcVar18)(param_1);
          PlayerEgo::setPosition(extraout_s0_14,extraout_s1_14,extraout_s2_14);
          iVar5 = (*pcVar18)(param_1);
          pVVar26 = *(Vector **)(iVar5 + 8);
          (*pcVar18)(param_1);
          PlayerEgo::getPosition();
          AbyssEngine::AEMath::operator-((AEMath *)&local_114,(Vector *)&local_48);
          AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
          local_54 = 0;
          local_50 = 0x3f800000;
          local_4c = 0;
          AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
          *(undefined2 *)&this->field_20 = 0x100;
          TargetFollowCamera::setLookAtCam(bVar2);
          (*pcVar18)(param_1);
          TargetFollowCamera::setTarget((AEGeometry *)param_4);
          (*pcVar18)(param_1);
          PlayerEgo::getPosition();
          (*pcVar18)(param_1);
          PlayerEgo::GetDirVector();
          pcVar21 = *(code **)(DAT_00146d88 + 0x146c3a);
          (*pcVar21)(&local_60,&local_6c,DAT_00146d8c);
          AbyssEngine::AEMath::operator-((Vector *)&local_48,(Vector *)&local_54);
          (*pcVar18)(param_1);
          PlayerEgo::GetUpVector();
          (*pcVar21)(&local_78,auStack_84,DAT_00146d90);
          AbyssEngine::AEMath::operator+((Vector *)&local_114,(Vector *)&local_48);
          (*pcVar18)(param_1);
          AEGeometry::getRightVector();
          (*pcVar21)(auStack_90,auStack_9c,DAT_00146d94);
          AbyssEngine::AEMath::operator-((Vector *)&local_d8,(Vector *)&local_114);
          TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_d8);
          TargetFollowCamera::update((int)param_4);
          uVar24 = **(uint **)(DAT_00146d98 + 0x146cb4);
          AbyssEngine::PaintCanvas::CameraGetCurrent();
          uVar8 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar24);
          __aeabi_memcpy((Vector *)&local_d8,uVar8,0x3c);
          AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_114);
          AbyssEngine::AEMath::Vector::operator=((Vector *)this_00,(Vector *)&local_114);
          TargetFollowCamera::setFixed((TargetFollowCamera *)param_4,true);
          AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_114,(Vector *)&local_d8);
          TargetFollowCamera::setLocal
                    (param_4,local_d8,local_d4,local_d0,local_cc,uStack_c8,local_c4,uStack_c0,
                     local_bc,uStack_b8,local_b4,uStack_b0,local_ac,uStack_a8,uStack_a4,uStack_a0);
          goto LAB_0014627e;
        }
      }
      iVar5 = Status::getCurrentCampaignMission();
      if ((iVar5 == 0x9e) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
        pSVar13 = (Station *)Status::getStation();
        iVar5 = Station::getIndex(pSVar13);
        if (iVar5 == 0x6f) {
          pAVar9 = operator_new(0xc0);
          AEGeometry::AEGeometry(pAVar9,(PaintCanvas *)**(undefined4 **)(DAT_001470b8 + 0x146dd8));
          local_d0 = DAT_001470c0;
          puVar6 = (undefined4 *)(DAT_001470bc + 0x146dee);
          this[1].field_0 = (int *)pAVar9;
          local_d4 = 0;
          pcVar21 = (code *)*puVar6;
          local_d8 = 1.0;
          local_114 = 0;
          local_110 = 0x3f800000;
          local_10c = 0;
          (*pcVar21)(pAVar9,&local_d8,&local_114);
          pcVar18 = *(code **)(DAT_001470c4 + 0x146e0e);
          *(undefined2 *)&this->field_20 = 0x100;
          (*pcVar18)(param_1);
          PlayerEgo::setPosition(extraout_s0_15,extraout_s1_15,extraout_s2_15);
          param_2[1] = (Hud)0x0;
          param_3[0x48] = (Radar)0x0;
          pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
          PlayerEgo::setFreeze(pPVar7,true);
          puVar6 = (undefined4 *)(*pcVar18)(param_1);
          Player::setVulnerable(SUB41(*puVar6,0));
          Level::getStarSystem();
          StarSystem::getLightDirection();
          AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
          iVar5 = (*pcVar18)(param_1);
          local_110 = 0x3f800000;
          local_114 = 0;
          local_10c = 0;
          (*pcVar21)(*(undefined4 *)(iVar5 + 8),(AEMath *)&local_d8,&local_114);
          iVar5 = **(int **)(DAT_001470d0 + 0x146e86);
          (*pcVar18)(param_1);
          FModSound::stop(iVar5);
          *(undefined1 *)((int)&this->field_20 + 1) = 1;
          TargetFollowCamera::setLookAtCam(bVar2);
          (*pcVar18)(param_1);
          PlayerEgo::getPosition();
          local_50 = DAT_001470d4;
          local_54 = DAT_001470d8;
          local_4c = DAT_001470dc;
          AbyssEngine::AEMath::operator+((Vector *)&local_114,(Vector *)&local_48);
          TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_114);
          pAVar9 = (AEGeometry *)this[1].field_0;
          pVVar26 = (Vector *)TargetFollowCamera::getPosition((TargetFollowCamera *)param_4);
          AEGeometry::setPosition(pAVar9,pVVar26);
          piVar25 = this[1].field_0;
          AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_114,(Vector *)&local_d8);
          local_44 = 0x3f800000;
          local_48 = 0;
          local_40 = 0;
          fVar31 = (float)(*pcVar21)(piVar25,(AEMath *)&local_114,&local_48);
          AEGeometry::moveForward(fVar31);
          TargetFollowCamera::setTarget((AEGeometry *)param_4);
          iVar5 = DAT_001470e4;
          *(undefined1 *)((int)&this->field_10 + 1) = 1;
          Layout::startFade((Layout *)**(undefined4 **)(iVar5 + 0x146f2c),false,-1,8000);
          iVar5 = Level::getEnemies();
          Player::setEnemies(*(Player **)(**(int **)(iVar5 + 4) + 4),(Array *)0x0);
          pPVar28 = *(Player **)(**(int **)(iVar5 + 4) + 4);
          puVar6 = (undefined4 *)(*pcVar18)(param_1);
          Player::addEnemy(pPVar28,(Player *)*puVar6);
          fVar31 = DAT_001470e8;
          for (iVar22 = 1; iVar22 != 4; iVar22 = iVar22 + 1) {
            Player::setEnemies(*(Player **)((*(undefined4 **)(iVar5 + 4))[iVar22] + 4),(Array *)0x0)
            ;
            pPVar28 = *(Player **)(*(int *)(*(int *)(iVar5 + 4) + iVar22 * 4) + 4);
            puVar6 = (undefined4 *)Level::getPlayer();
            Player::setEnemy(pPVar28,(Player *)*puVar6);
            iVar14 = **(int **)(**(int **)(**(int **)(*(int *)(*(int *)(iVar5 + 4) + iVar22 * 4) + 4
                                                     ) + 4) + 4);
            fVar32 = (float)VectorSignedToFloat(*(undefined4 *)(iVar14 + 0x60),
                                                (byte)(in_fpscr >> 0x16) & 3);
            *(int *)(iVar14 + 0x60) = (int)(fVar32 * fVar31);
            puVar27 = *(uint **)(param_1 + 0xb0);
            if (puVar27 != (uint *)0x0) {
              for (uVar24 = 0; uVar24 < *puVar27; uVar24 = uVar24 + 1) {
                Player::addEnemy(*(Player **)(*(int *)(*(int *)(iVar5 + 4) + iVar22 * 4) + 4),
                                 *(Player **)(*(int *)(puVar27[1] + uVar24 * 4) + 4));
                Player::addEnemy(*(Player **)
                                  (*(int *)(*(int *)(*(int *)(param_1 + 0xb0) + 4) + uVar24 * 4) + 4
                                  ),*(Player **)(*(int *)(*(int *)(iVar5 + 4) + iVar22 * 4) + 4));
                puVar27 = *(uint **)(param_1 + 0xb0);
              }
            }
          }
          KIPlayer::setActive(SUB41(**(undefined4 **)(iVar5 + 4),0));
          KIPlayer::setVisible((KIPlayer *)**(undefined4 **)(iVar5 + 4),false);
          uVar8 = (*pcVar18)(param_1);
          AEGeometry::getPosition();
          PlayerEgo::setPosition(uVar8,local_120,uStack_11c,uStack_118);
          iVar5 = (*pcVar18)(param_1);
          pVVar26 = *(Vector **)(iVar5 + 8);
          pVVar15 = (Vector *)TargetFollowCamera::getPosition((TargetFollowCamera *)param_4);
          AEGeometry::getPosition();
          AbyssEngine::AEMath::operator-((Vector *)&local_54,pVVar15);
          local_6c = DAT_001470ec;
          uStack_68 = 0;
          local_64 = 0;
          AbyssEngine::AEMath::operator+((Vector *)&local_48,(Vector *)&local_54);
          AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_114,(Vector *)&local_48);
          local_78 = 0;
          uStack_74 = 0x3f800000;
          local_70 = 0;
          AEGeometry::setDirection(pVVar26,(Vector *)&local_114);
          fVar31 = (float)(*pcVar18)(param_1);
          AEGeometry::moveForward(fVar31);
          pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
          PlayerEgo::setSpeed(pPVar7,extraout_s0_16);
          this->m_nState = 1;
          goto LAB_00146406;
        }
      }
      uVar8 = 500;
      piVar25 = *(int **)(DAT_00147498 + 0x14710a);
      pcVar18 = *(code **)(DAT_0014749c + 0x14710e);
      cVar1 = **(char **)(DAT_00147494 + 0x147106);
      if (cVar1 == '\0') {
        uVar8 = 2000;
      }
      iVar5 = (*pcVar18)(*piVar25,uVar8);
      fVar31 = (float)VectorSignedToFloat(iVar5 + 500,(byte)(in_fpscr >> 0x16) & 3);
      this->m_nStreamTimer = fVar31;
      iVar5 = (*pcVar18)(*piVar25,uVar8);
      pfVar16 = (float *)&DAT_00147b34;
      fVar31 = (float)VectorSignedToFloat(iVar5 + 500,(byte)(in_fpscr >> 0x16) & 3);
      if (cVar1 == '\0') {
        pfVar16 = (float *)&DAT_00147b38;
      }
      fVar32 = *pfVar16;
      this->m_nStreamOffsetX = fVar31;
      this->m_nStreamOffsetZ = fVar32;
      iVar5 = (*pcVar18)(*piVar25,2);
      if (iVar5 == 0) {
        *this_00 = -*this_00;
      }
      piVar4 = &this->field_34;
      iVar5 = AbyssEngine::AERandom::nextInt(*piVar25);
      if (iVar5 == 0) {
        this->m_nStreamOffsetX = -this->m_nStreamOffsetX;
      }
      iVar5 = Status::getCurrentCampaignMission();
      if (iVar5 == 0x30) {
        pcVar18 = *(code **)(DAT_001474a0 + 0x1471aa);
        (*pcVar18)(param_1);
        PlayerEgo::setPosition(extraout_s0_17,extraout_s1_16,extraout_s2_16);
        (*pcVar18)(param_1);
        AEGeometry::getDirection();
        local_d8 = local_d8 + DAT_001474a8;
        iVar5 = (*pcVar18)(param_1);
        local_114 = 0;
        local_110 = 0x3f800000;
        local_10c = 0;
        AEGeometry::setDirection(*(Vector **)(iVar5 + 8),(Vector *)&local_d8);
        pKVar17 = (KIPlayer *)(*pcVar18)(param_1);
        Level::getLandmarks();
        PlayerEgo::setAutoPilot(pKVar17);
      }
      else {
        iVar5 = Status::getCurrentCampaignMission();
        if ((iVar5 == 0x41) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
          pSVar13 = (Station *)Status::getStation();
          iVar5 = Station::getIndex(pSVar13);
          Status::getCampaignMission();
          iVar22 = Mission::getTargetStation();
          if (iVar5 == iVar22) {
            pcVar18 = *(code **)(DAT_001474ac + 0x147248);
            (*pcVar18)(param_1);
            PlayerEgo::setPosition(extraout_s0_18,extraout_s1_17,extraout_s2_17);
            iVar5 = (*pcVar18)(param_1);
            pVVar26 = *(Vector **)(iVar5 + 8);
            (*pcVar18)(param_1);
            PlayerEgo::getPosition();
            AbyssEngine::AEMath::operator-((AEMath *)&local_114,(Vector *)&local_48);
            AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
            local_50 = 0x3f800000;
            local_54 = 0;
            local_4c = 0;
            AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
            (*pcVar18)(param_1);
            PlayerEgo::getPosition();
            local_d8 = local_d8 + DAT_001474b4;
            iVar5 = Level::getEnemies();
            (**(code **)(*(int *)**(undefined4 **)(iVar5 + 4) + 0x48))
                      ((int *)**(undefined4 **)(iVar5 + 4),local_d8,local_d4,local_d0);
            iVar5 = Level::getEnemies();
            local_114 = 0;
            local_110 = DAT_001474b8;
            local_10c = 0;
            AEGeometry::setRotation
                      (*(AEGeometry **)(**(int **)(iVar5 + 4) + 8),(Vector *)&local_114);
            goto LAB_00147650;
          }
        }
        iVar5 = Status::getCurrentCampaignMission();
        if ((iVar5 == 0x57) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
          pSVar13 = (Station *)Status::getStation();
          iVar5 = Station::getIndex(pSVar13);
          Status::getCampaignMission();
          iVar22 = Mission::getTargetStation();
          if (iVar5 != iVar22) goto LAB_0014732a;
          pcVar18 = *(code **)(DAT_001474bc + 0x147322);
          (*pcVar18)(param_1);
          fVar31 = extraout_s0_19;
          fVar32 = extraout_s1_18;
          fVar33 = extraout_s2_18;
LAB_001475fe:
          PlayerEgo::setPosition(fVar31,fVar32,fVar33);
          iVar5 = (*pcVar18)(param_1);
          pVVar26 = *(Vector **)(iVar5 + 8);
          (*pcVar18)(param_1);
          PlayerEgo::getPosition();
          AbyssEngine::AEMath::operator-((AEMath *)&local_114,(Vector *)&local_48);
          AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
LAB_00147644:
          local_4c = 0;
          local_50 = 0x3f800000;
          local_54 = 0;
        }
        else {
LAB_0014732a:
          iVar5 = Status::getCurrentCampaignMission();
          if ((iVar5 == 0x5b) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
            pSVar13 = (Station *)Status::getStation();
            iVar5 = Station::getIndex(pSVar13);
            Status::getCampaignMission();
            iVar22 = Mission::getTargetStation();
            if (iVar5 == iVar22) {
              pcVar18 = *(code **)(DAT_001474c4 + 0x147364);
              (*pcVar18)(param_1);
              fVar31 = extraout_s0_20;
              fVar32 = extraout_s1_19;
              fVar33 = extraout_s2_19;
              goto LAB_001475fe;
            }
          }
          iVar5 = Status::getCurrentCampaignMission();
          if ((iVar5 != 0x5c) || (iVar5 = Status::inAlienOrbit(), iVar5 != 0)) {
LAB_0014740a:
            iVar5 = Status::getCurrentCampaignMission();
            if ((iVar5 != 0x66) || (iVar5 = Status::inAlienOrbit(), iVar5 != 0)) {
LAB_001474e4:
              iVar5 = Status::getCurrentCampaignMission();
              if ((iVar5 == 0x7b) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
                pSVar13 = (Station *)Status::getStation();
                iVar5 = Station::getIndex(pSVar13);
                Status::getCampaignMission();
                iVar22 = Mission::getTargetStation();
                if (iVar5 == iVar22) {
                  pcVar18 = *(code **)(DAT_00147800 + 0x14751e);
                  (*pcVar18)(param_1);
                  fVar31 = extraout_s0_23;
                  fVar32 = extraout_s1_22;
                  fVar33 = extraout_s2_22;
                  goto LAB_001475fe;
                }
              }
              iVar5 = Status::getCurrentCampaignMission();
              if (((0x54 < iVar5) || (iVar5 = Status::getCurrentCampaignMission(), iVar5 == 0x87))
                 && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
                pSVar13 = (Station *)Status::getStation();
                iVar5 = Station::getIndex(pSVar13);
                if (iVar5 == 0x67) {
                  pcVar18 = *(code **)(DAT_00147808 + 0x14755e);
                  (*pcVar18)(param_1);
                  fVar31 = extraout_s0_24;
                  fVar32 = extraout_s1_23;
                  fVar33 = extraout_s2_23;
                  goto LAB_001475fe;
                }
              }
              iVar5 = Status::getCurrentCampaignMission();
              if ((iVar5 == 0x72) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
                pSVar13 = (Station *)Status::getStation();
                iVar5 = Station::getIndex(pSVar13);
                Status::getCampaignMission();
                iVar22 = Mission::getTargetStation();
                if (iVar5 == iVar22) {
                  pcVar18 = *(code **)(DAT_00147814 + 0x1475a2);
                  (*pcVar18)(param_1);
                  fVar31 = extraout_s0_25;
                  fVar32 = extraout_s1_24;
                  fVar33 = extraout_s2_24;
                  goto LAB_001475fe;
                }
              }
              iVar5 = Status::getCurrentCampaignMission();
              if ((iVar5 == 0x89) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
                pSVar13 = (Station *)Status::getStation();
                iVar5 = Station::getIndex(pSVar13);
                Status::getCampaignMission();
                iVar22 = Mission::getTargetStation();
                if (iVar5 == iVar22) {
                  pcVar18 = *(code **)(DAT_0014781c + 0x1475f2);
                  (*pcVar18)(param_1);
                  fVar31 = extraout_s0_26;
                  fVar32 = extraout_s1_25;
                  fVar33 = extraout_s2_25;
                  goto LAB_001475fe;
                }
              }
              iVar5 = Status::getCurrentCampaignMission();
              if ((iVar5 == 0x8b) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
                pSVar13 = (Station *)Status::getStation();
                iVar5 = Station::getIndex(pSVar13);
                Status::getCampaignMission();
                iVar22 = Mission::getTargetStation();
                if (iVar5 == iVar22) {
                  pcVar18 = *(code **)(DAT_00147b2c + 0x147792);
                  (*pcVar18)(param_1);
                  PlayerEgo::setPosition(extraout_s0_27,extraout_s1_26,extraout_s2_26);
                  iVar5 = (*pcVar18)(param_1);
                  pVVar26 = *(Vector **)(iVar5 + 8);
                  iVar5 = Level::getEnemies();
                  (**(code **)(**(int **)(*(int *)(iVar5 + 4) + 4) + 0x28))((Vector *)&local_48);
                  (*pcVar18)(param_1);
                  PlayerEgo::getPosition();
                  AbyssEngine::AEMath::operator-((Vector *)&local_114,(Vector *)&local_48);
                  AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
                  local_60 = 0;
                  local_5c = 0x3f800000;
                  local_58 = 0;
                  AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
                  goto LAB_00147650;
                }
              }
              iVar5 = Status::getCurrentCampaignMission();
              if ((iVar5 == 0x8e) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
                pSVar13 = (Station *)Status::getStation();
                iVar5 = Station::getIndex(pSVar13);
                Status::getCampaignMission();
                iVar22 = Mission::getTargetStation();
                if (iVar5 == iVar22) {
                  pcVar18 = *(code **)(DAT_00147b5c + 0x147878);
                  (*pcVar18)(param_1);
                  PlayerEgo::setPosition(extraout_s0_28,extraout_s1_27,extraout_s2_27);
                  iVar5 = (*pcVar18)(param_1);
                  pVVar26 = *(Vector **)(iVar5 + 8);
                  pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
                  this_05 = (Route *)PlayerEgo::getRoute(pPVar7);
                  piVar25 = (int *)Route::getWaypoint(this_05,0);
                  (**(code **)(*piVar25 + 0x28))((Vector *)&local_48,piVar25);
                  (*pcVar18)(param_1);
                  PlayerEgo::getPosition();
                  AbyssEngine::AEMath::operator-((Vector *)&local_114,(Vector *)&local_48);
                  AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
                  local_5c = 0x3f800000;
                  local_60 = 0;
                  local_58 = 0;
                  AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
                  iVar5 = Level::getEnemies();
                  piVar25 = (int *)**(undefined4 **)(iVar5 + 4);
                  (*pcVar18)(param_1);
                  PlayerEgo::getPosition();
                  (*pcVar18)(param_1);
                  fVar31 = (float)AEGeometry::getRightVector();
                  AbyssEngine::AEMath::operator*((Vector *)&local_54,fVar31);
                  AbyssEngine::AEMath::operator+((Vector *)&local_114,(Vector *)&local_48);
                  (*pcVar18)(param_1);
                  fVar31 = (float)AEGeometry::getDirection();
                  AbyssEngine::AEMath::operator*((Vector *)&local_6c,fVar31);
                  AbyssEngine::AEMath::operator+((Vector *)&local_d8,(Vector *)&local_114);
                  (**(code **)(*piVar25 + 0x44))(piVar25,(Vector *)&local_d8);
                  iVar5 = Level::getEnemies();
                  pVVar26 = *(Vector **)(**(int **)(iVar5 + 4) + 8);
                  (*pcVar18)(param_1);
                  PlayerEgo::GetDirVector();
                  local_110 = 0x3f800000;
                  local_114 = 0;
                  local_10c = 0;
                  AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
                  goto LAB_00147650;
                }
              }
              iVar5 = Status::inAlienOrbit();
              if ((iVar5 != 0) && (iVar5 = Status::getCurrentCampaignMission(), iVar5 == 0x9a)) {
                pcVar21 = *(code **)(DAT_00147b60 + 0x1479b6);
                (*pcVar21)(param_1);
                PlayerEgo::setPosition(extraout_s0_29,extraout_s1_28,extraout_s2_28);
                pcVar18 = *(code **)(DAT_00147b64 + 0x1479cc);
                iVar5 = (*pcVar18)(param_1);
                piVar25 = (int *)**(undefined4 **)(iVar5 + 4);
                (*pcVar21)(param_1);
                PlayerEgo::getPosition();
                (*pcVar21)(param_1);
                fVar31 = (float)AEGeometry::getRightVector();
                AbyssEngine::AEMath::operator*((Vector *)&local_54,fVar31);
                AbyssEngine::AEMath::operator+((Vector *)&local_114,(Vector *)&local_48);
                (*pcVar21)(param_1);
                fVar31 = (float)AEGeometry::getDirection();
                AbyssEngine::AEMath::operator*((Vector *)&local_6c,fVar31);
                AbyssEngine::AEMath::operator+((Vector *)&local_d8,(Vector *)&local_114);
                (**(code **)(*piVar25 + 0x44))(piVar25,(Vector *)&local_d8);
                iVar5 = (*pcVar18)(param_1);
                pVVar26 = *(Vector **)(**(int **)(iVar5 + 4) + 8);
                (*pcVar21)(param_1);
                PlayerEgo::GetDirVector();
                local_110 = 0x3f800000;
                local_114 = 0;
                local_10c = 0;
                AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
                iVar5 = (*pcVar18)(param_1);
                pPVar28 = *(Player **)(**(int **)(iVar5 + 4) + 4);
                iVar5 = (*pcVar18)(param_1);
                Player::setEnemy(pPVar28,*(Player **)(*(int *)(*(int *)(iVar5 + 4) + 4) + 4));
                for (uVar24 = 0; puVar27 = (uint *)Level::getEnemies(), uVar24 < *puVar27;
                    uVar24 = uVar24 + 1) {
                  iVar5 = Level::getEnemies();
                  if (*(char *)(*(int *)(*(int *)(iVar5 + 4) + uVar24 * 4) + 0x3e) != '\0') {
                    iVar5 = Level::getEnemies();
                    Player::setEnemies(*(Player **)(*(int *)(*(int *)(iVar5 + 4) + uVar24 * 4) + 4),
                                       (Array *)0x0);
                  }
                  iVar5 = Level::getEnemies();
                  if (*(int *)(*(int *)(*(int *)(iVar5 + 4) + uVar24 * 4) + 0x28) == 9) {
                    iVar5 = Level::getEnemies();
                    *(undefined1 *)(*(int *)(*(int *)(iVar5 + 4) + uVar24 * 4) + 0x25) = 0;
                  }
                }
              }
              goto LAB_00147650;
            }
            pSVar13 = (Station *)Status::getStation();
            iVar5 = Station::getIndex(pSVar13);
            Status::getCampaignMission();
            iVar22 = Mission::getTargetStation();
            if (iVar5 != iVar22) goto LAB_001474e4;
            pcVar18 = *(code **)(DAT_001474d4 + 0x147446);
            (*pcVar18)(param_1);
            PlayerEgo::setPosition(extraout_s0_22,extraout_s1_21,extraout_s2_21);
            iVar5 = (*pcVar18)(param_1);
            pVVar26 = *(Vector **)(iVar5 + 8);
            (*pcVar18)(param_1);
            PlayerEgo::getPosition();
            AbyssEngine::AEMath::operator-((AEMath *)&local_114,(Vector *)&local_48);
            AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
            goto LAB_00147644;
          }
          pSVar13 = (Station *)Status::getStation();
          iVar5 = Station::getIndex(pSVar13);
          Status::getCampaignMission();
          iVar22 = Mission::getTargetStation();
          if (iVar5 != iVar22) goto LAB_0014740a;
          pcVar18 = *(code **)(DAT_001474cc + 0x1473a8);
          (*pcVar18)(param_1);
          PlayerEgo::setPosition(extraout_s0_21,extraout_s1_20,extraout_s2_20);
          iVar5 = (*pcVar18)(param_1);
          pVVar26 = *(Vector **)(iVar5 + 8);
          iVar5 = Level::getEnemies();
          (**(code **)(**(int **)(*(int *)(iVar5 + 4) + 0xc) + 0x28))((Vector *)&local_48);
          (*pcVar18)(param_1);
          PlayerEgo::getPosition();
          AbyssEngine::AEMath::operator-((Vector *)&local_114,(Vector *)&local_48);
          AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d8,(Vector *)&local_114);
          local_60 = 0;
          local_5c = 0x3f800000;
          local_58 = 0;
        }
        AEGeometry::setDirection(pVVar26,(Vector *)&local_d8);
      }
LAB_00147650:
      Level::getPlayer();
      uVar8 = AEGeometry::getMatrix();
      __aeabi_memcpy((Vector *)&local_d8,uVar8,0x3c);
      AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_114,(Vector *)&local_d8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)piVar4,(Vector *)&local_114);
      Level::getPlayer();
      AEGeometry::getPosition();
      AbyssEngine::AEMath::Vector::operator+=((Vector *)piVar4,(Vector *)&local_114);
      TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)piVar4);
      iVar5 = Status::inAlienOrbit();
      pcVar23 = *(char **)(DAT_00147828 + 0x1476b0);
      if ((*pcVar23 != '\0') || (iVar5 != 0)) {
        iVar5 = Status::getCurrentCampaignMission();
        if (iVar5 < 0x2b) {
          piVar25 = &this->field_40;
          Level::getPlayer();
          PlayerEgo::getPosition();
          AbyssEngine::AEMath::Vector::operator=((Vector *)this_00,(Vector *)&local_114);
          Level::getPlayer();
          AEGeometry::getDirection();
          fVar31 = (float)AbyssEngine::AEMath::Vector::operator=
                                    ((Vector *)piVar25,(Vector *)&local_114);
          AbyssEngine::AEMath::Vector::operator*=((Vector *)piVar25,fVar31);
          AbyssEngine::AEMath::Vector::operator-=((Vector *)this_00,(Vector *)piVar25);
          pcVar18 = *(code **)(DAT_00147830 + 0x14771a);
          iVar5 = (*pcVar18)(param_1);
          PlayerWormHole::reset(*(PlayerWormHole **)(*(int *)(iVar5 + 4) + 0xc),true);
          iVar5 = (*pcVar18)(param_1);
          piVar25 = *(int **)(*(int *)(iVar5 + 4) + 0xc);
          (**(code **)(*piVar25 + 0x48))
                    (piVar25,this->m_nStreamTimer,this->m_nStreamOffsetX,this->m_nStreamOffsetZ);
          iVar5 = (*pcVar18)(param_1);
          KIPlayer::setVisible(*(KIPlayer **)(*(int *)(iVar5 + 4) + 0xc),true);
        }
        *pcVar23 = '\0';
      }
      goto LAB_00146406;
    }
    TargetFollowCamera::setPosition
              ((TargetFollowCamera *)param_4,extraout_s0_05,extraout_s1_05,extraout_s2_05);
    this->field_24 = 0;
    *(undefined2 *)&this->field_20 = 0x100;
    iVar5 = Level::getPlayer();
    local_d8 = 0.0;
    local_d4 = 0;
    local_d0 = 1.0;
    local_114 = 0;
    local_110 = 0x3f800000;
    local_10c = 0;
    AEGeometry::setDirection(*(Vector **)(iVar5 + 8),(Vector *)&local_d8);
    *(undefined1 *)((int)&this->field_10 + 1) = 1;
    this->m_nState = 1;
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry
              (pAVar9,0x3ab3,(PaintCanvas *)**(undefined4 **)(DAT_001462ac + 0x145f1e),false);
    fVar31 = extraout_s0_06;
    fVar32 = extraout_s1_06;
    fVar33 = extraout_s2_06;
  }
  this[1].field_C = (int)pAVar9;
  AEGeometry::setScaling(fVar31,fVar32,fVar33);
LAB_00146406:
  iVar5 = Status::getCurrentCampaignMission();
  if (iVar5 == 0x50) {
    pPVar7 = operator_new(0xc0);
    puVar27 = *(uint **)(DAT_001466cc + 0x146426);
    AEGeometry::AEGeometry((AEGeometry *)pPVar7,0x3822,(PaintCanvas *)*puVar27,false);
    iVar5 = DAT_001466d0;
    this->m_pPlayerEgo = pPVar7;
    pcVar18 = *(code **)(iVar5 + 0x146446);
    iVar5 = (*pcVar18)(*puVar27,pPVar7->m_pLevel);
    *(undefined4 *)(iVar5 + 0xe0) = DAT_001466d4;
    uVar8 = (*pcVar18)(*puVar27,this->m_pPlayerEgo->m_pLevel);
    pcVar21 = *(code **)(DAT_001466d8 + 0x146468);
    (*pcVar21)(uVar8,1,0);
    AEGeometry::setVisible(SUB41(this->m_pPlayerEgo,0));
    local_d8 = 0.0;
    local_d4 = DAT_001466dc;
    local_d0 = 0.0;
    AEGeometry::setRotation((AEGeometry *)this->m_pPlayerEgo,(Vector *)&local_d8);
    iVar5 = Level::getLandmarks();
    iVar5 = *(int *)(**(int **)(iVar5 + 4) + 0x140);
    uVar8 = (*pcVar18)(*puVar27,*(undefined4 *)(iVar5 + 0xc));
    (*pcVar21)(uVar8,1,0);
    uVar8 = (*pcVar18)(*puVar27,*(undefined4 *)(iVar5 + 0x14));
    (*pcVar21)(uVar8,1,0);
    uVar8 = (*pcVar18)(*puVar27,*(undefined4 *)(iVar5 + 0x10));
    (*pcVar21)(uVar8,1,0);
    uVar24 = *puVar27;
    iVar5 = Level::getLandmarks();
    uVar8 = (*pcVar18)(uVar24,*(undefined4 *)(**(int **)(iVar5 + 4) + 0x144));
    (*pcVar21)(uVar8,2,0);
    this_01 = operator_new(0x68);
    Explosion::Explosion(this_01,0);
    this->m_nParticleSystem1 = (int)this_01;
    fVar31 = (float)Explosion::addFireStreaks(this_01);
    Explosion::setScaling(fVar31);
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,(PaintCanvas *)*puVar27);
    this[1].field_10 = (int)pAVar9;
    AEGeometry::setVisible(SUB41(pAVar9,0));
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x381e,(PaintCanvas *)*puVar27,false);
    this->m_pAsteroids = (int *)pAVar9;
    Level::getEnemies();
    AEGeometry::getPosition();
    AEGeometry::setPosition(pAVar9,(Vector *)&local_d8);
    uVar8 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar27);
    AbyssEngine::Transform::SetAnimationState(uVar8,1,0);
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x3ab3,(PaintCanvas *)*puVar27,false);
    this[1].field_C = (int)pAVar9;
    AEGeometry::setScaling(extraout_s0_11,extraout_s1_11,extraout_s2_11);
  }
  iVar5 = Status::getCurrentCampaignMission();
  if ((iVar5 == 0x29) && (iVar5 = Status::inAlienOrbit(), iVar5 != 0)) {
    uVar20 = *(undefined4 *)(param_1 + 0x74);
    Level::getEnemies();
    uVar8 = AEGeometry::getReferenceMatrix();
    pTVar10 = (TargetFollowCamera *)ParticleSystemManager::addSystem(uVar20,uVar8,0x28,0);
    this[1].m_pCamera = pTVar10;
    ParticleSystemManager::enableSystemEmit(*(int *)(param_1 + 0x74),SUB41(pTVar10,0));
    uVar20 = *(undefined4 *)(param_1 + 0x74);
    Level::getEnemies();
    uVar8 = AEGeometry::getReferenceMatrix();
    pMVar11 = (MGame *)ParticleSystemManager::addSystem(uVar20,uVar8,0x29,0);
    this[1].m_pLevel = pMVar11;
    ParticleSystemManager::enableSystemEmit(*(int *)(param_1 + 0x74),SUB41(pMVar11,0));
    LODManager::forceUpdate(*(LODManager **)param_1,0x1e,false);
    pAVar9 = operator_new(0xc0);
    puVar6 = *(undefined4 **)(DAT_001466e8 + 0x14662e);
    AEGeometry::AEGeometry(pAVar9,0x37cd,(PaintCanvas *)*puVar6,false);
    this->m_pGeometriesMission2 = (int *)pAVar9;
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x37ce,(PaintCanvas *)*puVar6,false);
    this->m_pExplosion = (int *)pAVar9;
    pAVar9 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar9,0x37cf,(PaintCanvas *)*puVar6,false);
    iVar5 = DAT_001466ec;
    this->m_pMission = (int *)pAVar9;
    pcVar18 = *(code **)(iVar5 + 0x14667a);
    uVar8 = (*pcVar18)(*puVar6,this->m_pGeometriesMission2[3]);
    pcVar21 = *(code **)(DAT_001466f0 + 0x146688);
    (*pcVar21)(uVar8,1,0);
    uVar8 = (*pcVar18)(*puVar6,this->m_pExplosion[3]);
    (*pcVar21)(uVar8,1,0);
    uVar8 = (*pcVar18)(*puVar6,this->m_pMission[3]);
    (*pcVar21)(uVar8,1,0);
  }
  else {
    iVar5 = Status::getCurrentCampaignMission();
    if ((iVar5 == 0x9d) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
      pSVar13 = (Station *)Status::getStation();
      iVar5 = Station::getIndex(pSVar13);
      Status::getCampaignMission();
      iVar22 = Mission::getTargetStation();
      if (iVar5 == iVar22) {
        pAVar9 = operator_new(0xc0);
        puVar27 = *(uint **)(DAT_001469f8 + 0x14673e);
        AEGeometry::AEGeometry(pAVar9,0x4a75,(PaintCanvas *)*puVar27,false);
        this->m_pAsteroids = (int *)pAVar9;
        pAVar9 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar9,0x4a76,(PaintCanvas *)*puVar27,false);
        this_02 = operator_new(0xc0);
        AEGeometry::AEGeometry(this_02,0x4a77,(PaintCanvas *)*puVar27,false);
        AEGeometry::addChild((uint)this->m_pAsteroids);
        AEGeometry::addChild((uint)this->m_pAsteroids);
        pAVar12 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar12,0x4a97,(PaintCanvas *)*puVar27,false);
        this->m_pExtra = (int *)pAVar12;
        pAVar12 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar12,0x4a99,(PaintCanvas *)*puVar27,false);
        this->m_nParticleSystem0 = (int)pAVar12;
        pAVar12 = operator_new(0xc0);
        AEGeometry::AEGeometry(pAVar12,0x4a98,(PaintCanvas *)*puVar27,false);
        this_03 = operator_new(0xc0);
        AEGeometry::AEGeometry(this_03,0x4a9a,(PaintCanvas *)*puVar27,false);
        AEGeometry::addChild((uint)this->m_pExtra);
        AEGeometry::addChild(this->m_nParticleSystem0);
        this_04 = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar27);
        AbyssEngine::Transform::SetVisible(this_04,false);
        AEGeometry::setVisible(SUB41(this->m_pExtra,0));
        AEGeometry::setVisible(SUB41(this->m_nParticleSystem0,0));
        pcVar18 = *(code **)(DAT_001469fc + 0x14684e);
        (*pcVar18)(pAVar9);
        pcVar21 = *(code **)(DAT_00146a00 + 0x146858);
        (*pcVar21)();
        (*pcVar18)(this_02);
        (*pcVar21)();
        (*pcVar18)(pAVar12);
        (*pcVar21)();
        (*pcVar18)(this_03);
        (*pcVar21)();
      }
    }
  }
  Layout::enableFillScreen((Layout *)**(undefined4 **)(DAT_00146a04 + 0x14687c),false);
  iVar5 = Status::inAlienOrbit();
  if ((iVar5 == 0) && (iVar5 = Status::getCampaignMission(), iVar5 != 0)) {
    Status::getCampaignMission();
    iVar5 = Mission::getType();
    if ((iVar5 == 0xa3) && (puVar27 = *(uint **)(*piVar30 + 0x90), puVar27 != (uint *)0x0)) {
      for (uVar24 = 0; uVar24 < *puVar27; uVar24 = uVar24 + 1) {
        iVar22 = *(int *)(puVar27[1] + uVar24 * 4);
        pSVar13 = (Station *)Status::getStation();
        iVar5 = Station::getIndex(pSVar13);
        if (iVar22 == iVar5) {
          *(undefined1 *)((int)&this->m_pGeometriesMission + 1) = 1;
          break;
        }
        puVar27 = *(uint **)(*piVar30 + 0x90);
      }
    }
  }
  iVar5 = Status::getCurrentCampaignMission();
  if (((((iVar5 == 0x5f) || (iVar5 = Status::getCurrentCampaignMission(), iVar5 == 99)) ||
       (iVar5 = Status::getCurrentCampaignMission(), iVar5 == 0x6d)) ||
      ((iVar5 = Status::getCurrentCampaignMission(), iVar5 == 0x77 ||
       (iVar5 = Status::getCurrentCampaignMission(), iVar5 == 0x7e)))) ||
     ((iVar5 = Status::getCurrentCampaignMission(), iVar5 == 0x85 ||
      ((iVar5 = Status::getCurrentCampaignMission(), iVar5 == 0xa0 ||
       (iVar5 = Status::getCurrentCampaignMission(), iVar5 == 0xa1)))))) {
    puVar6 = (undefined4 *)(DAT_00146a08 + 0x14694c);
    param_2[1] = (Hud)0x0;
    pcVar18 = (code *)*puVar6;
    param_3[0x48] = (Radar)0x0;
    *(undefined1 *)((int)&this->field_10 + 1) = 1;
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setComputerControlled(pPVar7,true);
    iVar5 = (*pcVar18)(param_1);
    *(undefined1 *)(iVar5 + 0x24) = 1;
    puVar6 = (undefined4 *)(*pcVar18)(param_1);
    Player::setVulnerable(SUB41(*puVar6,0));
    pPVar7 = (PlayerEgo *)(*pcVar18)(param_1);
    PlayerEgo::setVisible(pPVar7,false);
    iVar5 = **(int **)(DAT_00146a0c + 0x146988);
    (*pcVar18)(param_1);
    FModSound::stop(iVar5);
    *(undefined1 *)((int)&this->field_20 + 1) = 1;
    TargetFollowCamera::setLookAtCam(bVar2);
    local_d8 = DAT_00146a14;
    local_d4 = DAT_00146a10;
    local_d0 = DAT_00146a18;
    TargetFollowCamera::setPosition((TargetFollowCamera *)param_4,(Vector *)&local_d8);
    (*pcVar18)(param_1);
    PlayerEgo::setPosition(extraout_s0_12,extraout_s1_12,extraout_s2_12);
    LODManager::forceUpdate(*(LODManager **)param_1,0x1e,false);
    *piVar34 = 0;
    this->field_94 = 0;
  }
  iVar5 = *piVar29 - local_3c;
  if (iVar5 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar5);
}

```

## target disasm
```
  00145adc: push {r4,r5,r6,r7,lr}
  00145ade: add r7,sp,#0xc
  00145ae0: push {r7,r8,r9,r10,r11}
  00145ae4: vpush {d8,d9}
  00145ae8: sub sp,#0x140
  00145aea: mov r11,r0
  00145aec: ldr r0,[0x00145e64]
  00145aee: vmov.i32 q4,#0x0
  00145af2: movs r5,#0x0
  00145af4: add r0,pc
  00145af6: mov r6,r3
  00145af8: mov r4,r2
  00145afa: mov r10,r1
  00145afc: ldr.w r9,[r0,#0x0]
  00145b00: ldr.w r0,[r9,#0x0]
  00145b04: str r0,[sp,#0x13c]
  00145b06: add.w r0,r11,#0x38
  00145b0a: str.w r5,[r11,#0x48]
  00145b0e: vst1.32 {d8,d9},[r0]
  00145b12: add.w r0,r11,#0x28
  00145b16: str r0,[sp,#0x3c]
  00145b18: vst1.32 {d8,d9},[r0]
  00145b1c: add.w r0,r11,#0x4c
  00145b20: blx 0x0006f118
  00145b24: ldr.w r8,[r7,#0x8]
  00145b28: mov r0,r4
  00145b2a: movs r1,#0x0
  00145b2c: str r6,[sp,#0x54]
  00145b2e: strd r4,r6,[r11,#0xd0]
  00145b32: strd r8,r10,[r11,#0x14]
  00145b36: str r4,[sp,#0x50]
  00145b38: blx 0x00076a68
  00145b3c: movs r0,#0x1
  00145b3e: str.w r5,[r11,#0x1c]
  00145b42: strh.w r0,[r11,#0x20]
  00145b46: mov.w r0,#0x100
  00145b4a: strh.w r0,[r11,#0x10]
  00145b4e: mov r0,r10
  00145b50: blx 0x00076a74
  00145b54: add.w r1,r11,#0x9a
  00145b58: add.w r4,r11,#0xac
  00145b5c: vst1.16 {d8,d9},[r1]
  00145b60: add.w r1,r11,#0x8c
  00145b64: vst1.32 {d8,d9},[r1]
  00145b68: add.w r1,r11,#0xbc
  00145b6c: vst1.32 {d8,d9},[r1]
  00145b70: movs r1,#0x1
  00145b72: vst1.32 {d8,d9},[r4]
  00145b76: strd r5,r5,[r11,#0x8]
  00145b7a: str.w r0,[r11,#0x0]
  00145b7e: mov r0,r8
  00145b80: str.w r5,[r11,#0x24]
  00145b84: strd r5,r5,[r11,#0xd8]
  00145b88: strh.w r5,[r11,#0x12]
  00145b8c: str.w r5,[r11,#0xcc]
  00145b90: blx 0x00072670
  00145b94: add.w r0,r11,#0x90
  00145b98: str r0,[sp,#0x4c]
  00145b9a: mov r0,r10
  00145b9c: blx 0x00072034
  00145ba0: cmp r0,#0x0
  00145ba2: str.w r9,[sp,#0x48]
  00145ba6: beq 0x00145bb8
  00145ba8: mov r0,r10
  00145baa: blx 0x00072034
  00145bae: ldr r0,[r0,#0x0]
  00145bb0: movs r1,#0x0
  00145bb2: blx 0x0007294c
  00145bb6: b 0x00145bbc
  00145bb8: strb.w r5,[r11,#0x20]
  00145bbc: mov r0,r10
  00145bbe: blx 0x00072034
  00145bc2: movs r1,#0x0
  00145bc4: movs r5,#0x0
  00145bc6: blx 0x00076a80
  00145bca: ldr r0,[0x00145e68]
  00145bcc: add r0,pc
  00145bce: ldr.w r9,[r0,#0x0]
  00145bd2: ldr.w r0,[r9,#0x0]
  00145bd6: blx 0x00071770
  00145bda: cmp r0,#0x0
  00145bdc: str r4,[sp,#0x40]
  00145bde: beq 0x00145c8c
  00145be0: ldr.w r0,[r9,#0x0]
  00145be4: blx 0x00071770
  00145be8: cmp r0,#0x1
  00145bea: bne.w 0x00145d82
  00145bee: ldr r0,[0x00145e6c]
  00145bf0: movs r1,#0x0
  00145bf2: ldr r2,[sp,#0x50]
  00145bf4: add r0,pc
  00145bf6: strb r1,[r2,#0x1]
  00145bf8: ldr r2,[sp,#0x54]
  00145bfa: ldr r4,[r0,#0x0]
  00145bfc: mov r0,r10
  00145bfe: strb.w r1,[r2,#0x48]
  00145c02: blx r4
  00145c04: ldr r1,[0x00145e70]
  00145c06: ldr r3,[0x00145e74]
  00145c08: mov r2,r1
  00145c0a: blx 0x00076a8c
  00145c0e: mov r0,r10
  00145c10: blx r4
  00145c12: movs r1,#0x1
  00145c14: movs r5,#0x1
  00145c16: blx 0x00076a98
  00145c1a: mov r0,r10
  00145c1c: blx r4
  00145c1e: movs r1,#0x1
  00145c20: blx 0x00076aa4
  00145c24: mov r0,r10
  00145c26: blx r4
  00145c28: movs r1,#0x0
  00145c2a: movs r2,#0x0
  00145c2c: movs r3,#0x0
  00145c2e: blx 0x000725bc
  00145c32: mov r0,r10
  00145c34: blx r4
  00145c36: movs r1,#0x0
  00145c38: blx 0x00072af0
  00145c3c: mov r0,r10
  00145c3e: blx r4
  00145c40: blx 0x00072ba4
  00145c44: mov r0,r10
  00145c46: blx 0x00071eb4
  00145c4a: ldr r0,[r0,#0x4]
  00145c4c: ldr r0,[r0,#0x0]
  00145c4e: blx 0x00076ab0
  00145c52: ldr r1,[0x00145e78]
  00145c54: mov r0,r8
  00145c56: ldr r2,[0x00145e7c]
  00145c58: ldr r3,[0x00145e80]
  00145c5a: blx 0x00076abc
  00145c5e: mov r0,r8
  00145c60: movs r1,#0x1
  00145c62: strb.w r5,[r11,#0x21]
  00145c66: blx 0x00072670
  00145c6a: mov r0,r10
  00145c6c: blx r4
  00145c6e: ldr r1,[r0,#0x8]
  00145c70: mov r0,r8
  00145c72: blx 0x00076ac8
  00145c76: ldr r0,[0x00145e84]
  00145c78: movs r1,#0x0
  00145c7a: movs r2,#0xff
  00145c7c: movw r3,#0x1388
  00145c80: add r0,pc
  00145c82: ldr r0,[r0,#0x0]
  00145c84: ldr r0,[r0,#0x0]
  00145c86: blx 0x00076ad4
  00145c8a: b 0x00146406
  00145c8c: ldr r0,[0x00145e88]
  00145c8e: ldr r1,[sp,#0x4c]
  00145c90: add r0,pc
  00145c92: strd r5,r5,[r1,#0x0]
  00145c96: ldr r1,[sp,#0x50]
  00145c98: ldr r4,[r0,#0x0]
  00145c9a: mov r0,r10
  00145c9c: strb r5,[r1,#0x1]
  00145c9e: ldr r1,[sp,#0x54]
  00145ca0: strb.w r5,[r1,#0x48]
  00145ca4: blx r4
  00145ca6: movs r1,#0x1
  00145ca8: movs r6,#0x1
  00145caa: blx 0x00076aa4
  00145cae: mov r0,r10
  00145cb0: blx r4
  00145cb2: ldr r0,[r0,#0x0]
  00145cb4: movs r1,#0x1
  00145cb6: blx 0x0007294c
  00145cba: mov r0,r8
  00145cbc: movs r1,#0x1
  00145cbe: strb.w r6,[r11,#0x21]
  00145cc2: blx 0x00072670
  00145cc6: ldr r2,[0x00145e8c]
  00145cc8: ldr r0,[0x00145e90]
  00145cca: ldr r1,[0x00145e94]
  00145ccc: strd r0,r2,[sp,#0xa0]
  00145cd0: mov r0,r8
  00145cd2: str r1,[sp,#0xa8]
  00145cd4: add r1,sp,#0xa0
  00145cd6: blx 0x00076ae0
  00145cda: mov r0,r10
  00145cdc: blx r4
  00145cde: ldr r3,[0x00145e98]
  00145ce0: movs r1,#0x0
  00145ce2: movs r2,#0x0
  00145ce4: blx 0x000725bc
  00145ce8: mov r0,r10
  00145cea: blx r4
  00145cec: ldr r0,[r0,#0x8]
  00145cee: mov.w r1,#0x3f800000
  00145cf2: strd r5,r5,[sp,#0xa0]
  00145cf6: add r2,sp,#0x64
  00145cf8: str r1,[sp,#0xa8]
  00145cfa: strd r5,r1,[sp,#0x64]
  00145cfe: add r1,sp,#0xa0
  00145d00: str r5,[sp,#0x6c]
  00145d02: blx 0x000726ac
  00145d06: ldr r0,[0x00145e9c]
  00145d08: movs r6,#0x0
  00145d0a: add r0,pc
  00145d0c: ldr r4,[r0,#0x0]
  00145d0e: b 0x00145d50
  00145d10: mov r0,r10
  00145d12: blx r4
  00145d14: ldr r0,[r0,#0x4]
  00145d16: ldr.w r0,[r0,r6,lsl #0x2]
  00145d1a: blx 0x000732dc
  00145d1e: mov r0,r10
  00145d20: blx r4
  00145d22: ldr r0,[r0,#0x4]
  00145d24: movs r1,#0x0
  00145d26: ldr.w r0,[r0,r6,lsl #0x2]
  00145d2a: ldr r0,[r0,#0x8]
  00145d2c: blx 0x00072d24
  00145d30: mov r0,r10
  00145d32: blx r4
  00145d34: ldr r0,[r0,#0x4]
  00145d36: ldr.w r0,[r0,r6,lsl #0x2]
  00145d3a: str.w r5,[r0,#0x128]
  00145d3e: mov r0,r10
  00145d40: blx r4
  00145d42: ldr r0,[r0,#0x4]
  00145d44: ldr.w r0,[r0,r6,lsl #0x2]
  00145d48: adds r6,#0x1
  00145d4a: ldr r0,[r0,#0x4]
  00145d4c: strb.w r5,[r0,#0x5c]
  00145d50: cmp r6,#0x3
  00145d52: bne 0x00145d10
  00145d54: ldr.w r0,[r10,#0x0]
  00145d58: movs r1,#0x1e
  00145d5a: movs r2,#0x0
  00145d5c: blx 0x00071818
  00145d60: movs r0,#0xc0
  00145d62: blx 0x0006eb24
  00145d66: mov r6,r0
  00145d68: ldr r0,[0x00145ea0]
  00145d6a: add r0,pc
  00145d6c: ldr r0,[r0,#0x0]
  00145d6e: ldr r2,[r0,#0x0]
  00145d70: mov r0,r6
  00145d72: movw r1,#0x3ab3
  00145d76: movs r3,#0x0
  00145d78: blx 0x0007207c
  00145d7c: str.w r6,[r11,#0xd8]
  00145d80: b 0x00146406
  00145d82: ldr.w r0,[r9,#0x0]
  00145d86: blx 0x00071770
  00145d8a: cmp r0,#0x51
  00145d8c: bne.w 0x00145ec0
  00145d90: ldr.w r0,[r9,#0x0]
  00145d94: blx 0x000723d0
  00145d98: cmp r0,#0x0
  00145d9a: beq.w 0x00145ec0
  00145d9e: ldr r0,[0x00145ea4]
  00145da0: movs r1,#0x0
  00145da2: ldr r2,[sp,#0x4c]
  00145da4: add r0,pc
  00145da6: strd r1,r1,[r2,#0x0]
  00145daa: ldr r2,[sp,#0x50]
  00145dac: ldr r4,[r0,#0x0]
  00145dae: mov r0,r10
  00145db0: strb r1,[r2,#0x1]
  00145db2: ldr r2,[sp,#0x54]
  00145db4: strb.w r1,[r2,#0x48]
  00145db8: blx r4
  00145dba: movs r1,#0x1
  00145dbc: movs r5,#0x1
  00145dbe: blx 0x00076aa4
  00145dc2: mov r0,r10
  00145dc4: blx r4
  00145dc6: strb.w r5,[r0,#0x24]
  00145dca: mov r0,r10
  00145dcc: blx r4
  00145dce: ldr r0,[r0,#0x0]
  00145dd0: movs r1,#0x0
  00145dd2: blx 0x0007294c
  00145dd6: mov r0,r10
  00145dd8: blx r4
  00145dda: movs r1,#0x0
  00145ddc: blx 0x00076aec
  00145de0: ldr r0,[0x00145ea8]
  00145de2: add r0,pc
  00145de4: ldr r0,[r0,#0x0]
  00145de6: ldr r6,[r0,#0x0]
  00145de8: mov r0,r10
  00145dea: blx r4
  00145dec: ldr r1,[r0,#0x1c]
  00145dee: mov r0,r6
  00145df0: blx 0x000724a8
  00145df4: mov r0,r8
  00145df6: movs r1,#0x1
  00145df8: strb.w r5,[r11,#0x21]
  00145dfc: blx 0x00072670
  00145e00: ldr r2,[0x00145eac]
  00145e02: ldr r0,[0x00145eb0]
  00145e04: ldr r1,[0x00145eb4]
  00145e06: strd r0,r2,[sp,#0xa0]
  00145e0a: mov r0,r8
  00145e0c: str r1,[sp,#0xa8]
  00145e0e: add r1,sp,#0xa0
  00145e10: blx 0x00076ae0
  00145e14: mov r0,r10
  00145e16: blx r4
  00145e18: movs r1,#0x0
  00145e1a: movs r2,#0x0
  00145e1c: movs r3,#0x0
  00145e1e: blx 0x000725bc
  00145e22: mov r0,r10
  00145e24: blx 0x00071eb4
  00145e28: ldr r0,[r0,#0x4]
  00145e2a: movs r1,#0x0
  00145e2c: ldr r0,[r0,#0x0]
  00145e2e: ldr.w r0,[r0,#0x140]
  00145e32: blx 0x00072d24
  00145e36: ldr.w r0,[r10,#0x0]
  00145e3a: movs r1,#0x1e
  00145e3c: movs r2,#0x0
  00145e3e: blx 0x00071818
  00145e42: movs r0,#0xc0
  00145e44: blx 0x0006eb24
  00145e48: mov r6,r0
  00145e4a: ldr r0,[0x00145eb8]
  00145e4c: add r0,pc
  00145e4e: ldr r0,[r0,#0x0]
  00145e50: ldr r2,[r0,#0x0]
  00145e52: mov r0,r6
  00145e54: movw r1,#0x3ab3
  00145e58: movs r3,#0x0
  00145e5a: blx 0x0007207c
  00145e5e: ldr r1,[0x00145ebc]
  00145e60: b 0x00145f2e
  00145ec0: ldr.w r0,[r9,#0x0]
  00145ec4: blx 0x00071770
  00145ec8: cmp r0,#0x4e
  00145eca: bne 0x00145f3e
  00145ecc: ldr r1,[0x001462a0]
  00145ece: mov r0,r8
  00145ed0: ldr r2,[0x001462a4]
  00145ed2: ldr r3,[0x001462a8]
  00145ed4: blx 0x00076abc
  00145ed8: movs r4,#0x0
  00145eda: mov.w r0,#0x100
  00145ede: str.w r4,[r11,#0x24]
  00145ee2: strh.w r0,[r11,#0x20]
  00145ee6: mov r0,r10
  00145ee8: blx 0x00072034
  00145eec: ldr r0,[r0,#0x8]
  00145eee: mov.w r1,#0x3f800000
  00145ef2: strd r4,r4,[sp,#0xa0]
  00145ef6: add r2,sp,#0x64
  00145ef8: str r1,[sp,#0xa8]
  00145efa: strd r4,r1,[sp,#0x64]
  00145efe: add r1,sp,#0xa0
  00145f00: str r4,[sp,#0x6c]
  00145f02: blx 0x000726ac
  00145f06: movs r0,#0x1
  00145f08: strb.w r0,[r11,#0x11]
  00145f0c: str.w r0,[r11,#0x1c]
  00145f10: movs r0,#0xc0
  00145f12: blx 0x0006eb24
  00145f16: mov r6,r0
  00145f18: ldr r0,[0x001462ac]
  00145f1a: add r0,pc
  00145f1c: ldr r0,[r0,#0x0]
  00145f1e: ldr r2,[r0,#0x0]
  00145f20: mov r0,r6
  00145f22: movw r1,#0x3ab3
  00145f26: movs r3,#0x0
  00145f28: blx 0x0007207c
  00145f2c: ldr r1,[0x001462b0]
  00145f2e: mov r0,r6
  00145f30: mov r2,r1
  00145f32: mov r3,r1
  00145f34: str.w r6,[r11,#0xd8]
  00145f38: blx 0x000727b4
  00145f3c: b 0x00146406
  00145f3e: ldr.w r0,[r9,#0x0]
  00145f42: blx 0x00071770
  00145f46: cmp r0,#0x59
  00145f48: str.w r9,[sp,#0x44]
  00145f4c: bne.w 0x00146176
  00145f50: ldr r0,[0x001462b4]
  00145f52: movs r2,#0x0
  00145f54: ldr r1,[sp,#0x4c]
  00145f56: add r0,pc
  00145f58: strd r2,r2,[r1,#0x0]
  00145f5c: ldr r1,[sp,#0x50]
  00145f5e: ldr r5,[r0,#0x0]
  00145f60: mov r0,r10
  00145f62: strb r2,[r1,#0x1]
  00145f64: ldr r1,[sp,#0x54]
  00145f66: strb.w r2,[r1,#0x48]
  00145f6a: blx r5
  00145f6c: movs r1,#0x1
  00145f6e: movs r4,#0x1
  00145f70: blx 0x00076aa4
  00145f74: mov r0,r10
  00145f76: blx r5
  00145f78: strb.w r4,[r0,#0x24]
  00145f7c: mov r0,r10
  00145f7e: blx r5
  00145f80: ldr r0,[r0,#0x0]
  00145f82: movs r1,#0x0
  00145f84: blx 0x0007294c
  00145f88: mov r0,r10
  00145f8a: blx r5
  00145f8c: movs r1,#0x0
  00145f8e: blx 0x00076aec
  00145f92: ldr r0,[0x001462b8]
  00145f94: add r0,pc
  00145f96: ldr r0,[r0,#0x0]
  00145f98: ldr r6,[r0,#0x0]
  00145f9a: mov r0,r10
  00145f9c: blx r5
  00145f9e: ldr r1,[r0,#0x1c]
  00145fa0: mov r0,r6
  00145fa2: blx 0x000724a8
  00145fa6: mov r0,r8
  00145fa8: movs r1,#0x1
  00145faa: strb.w r4,[r11,#0x21]
  00145fae: blx 0x00072670
  00145fb2: movs r0,#0xc0
  00145fb4: blx 0x0006eb24
  00145fb8: mov r6,r0
  00145fba: ldr r0,[0x001462bc]
  00145fbc: add r0,pc
  00145fbe: ldr r0,[r0,#0x0]
  00145fc0: str r0,[sp,#0x3c]
  00145fc2: ldr r1,[r0,#0x0]
  00145fc4: mov r0,r6
  00145fc6: blx 0x000720f4
  00145fca: ldr r0,[0x001462c0]
  00145fcc: str.w r6,[r11,#0xcc]
  00145fd0: add r0,pc
  00145fd2: ldr r4,[r0,#0x0]
  00145fd4: mov r0,r10
  00145fd6: blx r4
  00145fd8: ldr r0,[r0,#0x4]
  00145fda: add.w r8,sp,#0xa0
  00145fde: mov r5,r4
  00145fe0: ldr r1,[r0,#0x4]
  00145fe2: ldr r0,[r1,#0x0]
  00145fe4: ldr r2,[r0,#0x28]
  00145fe6: mov r0,r8
  00145fe8: blx r2
  00145fea: mov r0,r6
  00145fec: mov r1,r8
  00145fee: blx 0x00072148
  00145ff2: ldr r0,[0x001462c4]
  00145ff4: ldr.w r8,[r11,#0xcc]
  00145ff8: add r0,pc
  00145ffa: ldr r1,[r0,#0x0]
  00145ffc: mov r0,r10
  00145ffe: str r1,[sp,#0x34]
  00146000: blx r1
  00146002: mov r1,r0
  00146004: ldr r0,[0x001462c8]
  00146006: add r6,sp,#0x64
  00146008: add r0,pc
  0014600a: ldr r2,[r0,#0x0]
  0014600c: mov r0,r6
  0014600e: str r2,[sp,#0x30]
  00146010: blx r2
  00146012: add.w r9,sp,#0xa0
  00146016: mov r1,r6
  00146018: mov r0,r9
  0014601a: blx 0x0006f4cc
  0014601e: mov.w r0,#0x3f800000
  00146022: add r2,sp,#0x130
  00146024: str r0,[sp,#0x134]
  00146026: movs r0,#0x0
  00146028: str r0,[sp,#0x130]
  0014602a: mov r1,r9
  0014602c: str r0,[sp,#0x138]
  0014602e: mov r0,r8
  00146030: blx 0x000726ac
  00146034: ldr.w r0,[r11,#0xcc]
  00146038: ldr r1,[0x001462cc]
  0014603a: blx 0x000725c8
  0014603e: ldr.w r6,[r11,#0xcc]
  00146042: add.w r8,sp,#0x64
  00146046: mov r0,r8
  00146048: mov r1,r6
  0014604a: blx 0x0007261c
  0014604e: add.w r9,sp,#0xa0
  00146052: ldr r2,[0x001462d0]
  00146054: mov r1,r8
  00146056: mov r0,r9
  00146058: blx 0x0006ec74
  0014605c: mov r0,r6
  0014605e: mov r1,r9
  00146060: blx 0x00072460
  00146064: ldr r4,[r7,#0x8]
  00146066: ldr.w r1,[r11,#0xcc]
  0014606a: str.w r11,[sp,#0x38]
  0014606e: mov r0,r4
  00146070: blx 0x00076ac8
  00146074: mov r0,r10
  00146076: mov r11,r5
  00146078: blx r5
  0014607a: ldr r0,[r0,#0x4]
  0014607c: add r6,sp,#0xa0
  0014607e: ldr r1,[r0,#0x4]
  00146080: ldr r0,[r1,#0x0]
  00146082: ldr r2,[r0,#0x28]
  00146084: mov r0,r6
  00146086: blx r2
  00146088: mov r0,r4
  0014608a: mov r1,r6
  0014608c: blx 0x00076ae0
  00146090: ldr r1,[0x001462d4]
  00146092: mov r0,r4
  00146094: ldr r2,[0x001462d8]
  00146096: ldr r3,[0x001462dc]
  00146098: blx 0x00072688
  0014609c: mov r0,r10
  0014609e: blx r5
  001460a0: ldr r0,[r0,#0x4]
  001460a2: ldr r4,[sp,#0x34]
  001460a4: ldr r0,[r0,#0x0]
  001460a6: ldr.w r8,[r0,#0x8]
  001460aa: ldr r0,[0x001462e0]
  001460ac: str r0,[sp,#0x68]
  001460ae: ldr r0,[0x001462e4]
  001460b0: str r0,[sp,#0x64]
  001460b2: ldr r0,[0x001462e8]
  001460b4: str r0,[sp,#0x6c]
  001460b6: mov r0,r10
  001460b8: blx r4
  001460ba: add r6,sp,#0x124
  001460bc: ldr r5,[sp,#0x30]
  001460be: mov r1,r0
  001460c0: mov r0,r6
  001460c2: blx r5
  001460c4: add.w r9,sp,#0x130
  001460c8: ldr r1,[0x001462ec]
  001460ca: mov r2,r6
  001460cc: mov r0,r9
  001460ce: blx 0x00072658
  001460d2: add r6,sp,#0xa0
  001460d4: add r1,sp,#0x64
  001460d6: mov r2,r9
  001460d8: mov r0,r6
  001460da: blx 0x0006f1cc
  001460de: mov r0,r8
  001460e0: mov r1,r6
  001460e2: blx 0x00072148
  001460e6: mov r0,r10
  001460e8: blx r11
  001460ea: ldr r0,[r0,#0x4]
  001460ec: ldr r0,[r0,#0x0]
  001460ee: ldr r6,[r0,#0x8]
  001460f0: mov r0,r10
  001460f2: blx r4
  001460f4: add.w r8,sp,#0xa0
  001460f8: mov r1,r0
  001460fa: mov r0,r8
  001460fc: blx r5
  001460fe: mov.w r0,#0x3f800000
  00146102: add r2,sp,#0x64
  00146104: str r0,[sp,#0x68]
  00146106: movs r0,#0x0
  00146108: str r0,[sp,#0x64]
  0014610a: mov r1,r8
  0014610c: str r0,[sp,#0x6c]
  0014610e: mov r0,r6
  00146110: movs r4,#0x0
  00146112: blx 0x000726ac
  00146116: mov r0,r10
  00146118: blx r11
  0014611a: ldr r0,[r0,#0x4]
  0014611c: ldr r1,[0x001462f0]
  0014611e: ldr r0,[r0,#0x0]
  00146120: ldr r0,[r0,#0x8]
  00146122: strd r1,r4,[sp,#0xa0]
  00146126: str r1,[sp,#0xa8]
  00146128: add r1,sp,#0xa0
  0014612a: blx 0x00072460
  0014612e: movs r0,#0xc0
  00146130: blx 0x0006eb24
  00146134: mov r6,r0
  00146136: ldr r0,[sp,#0x3c]
  00146138: ldr r2,[r0,#0x0]
  0014613a: mov r0,r6
  0014613c: movw r1,#0x3795
  00146140: movs r3,#0x0
  00146142: blx 0x0007207c
  00146146: ldr.w r11,[sp,#0x38]
  0014614a: mov r0,r6
  0014614c: ldr r3,[0x001462f4]
  0014614e: mov.w r1,#0x3f800000
  00146152: mov.w r2,#0x3f800000
  00146156: str.w r6,[r11,#0xdc]
  0014615a: blx 0x000727b4
  0014615e: movs r0,#0x1
  00146160: movs r1,#0x1e
  00146162: str.w r0,[r11,#0x1c]
  00146166: movs r2,#0x0
  00146168: ldr.w r0,[r10,#0x0]
  0014616c: blx 0x00071818
  00146170: ldr.w r8,[r7,#0x8]
  00146174: b 0x00146402
  00146176: ldr.w r0,[r9,#0x0]
  0014617a: blx 0x00071770
  0014617e: cmp r0,#0x69
  00146180: bne.w 0x0014630c
  00146184: ldr.w r0,[r9,#0x0]
  00146188: blx 0x000722f8
  0014618c: cmp r0,#0x0
  0014618e: beq.w 0x0014630c
  00146192: ldr r0,[0x001462f8]
  00146194: movs r1,#0x1
  00146196: strb.w r1,[r11,#0x11]
  0014619a: movs r2,#0x0
  0014619c: ldr r1,[sp,#0x50]
  0014619e: add r0,pc
  001461a0: mov r5,r11
  001461a2: ldr.w r11,[r0,#0x0]
  001461a6: mov r0,r10
  001461a8: strb r2,[r1,#0x1]
  001461aa: ldr r1,[sp,#0x54]
  001461ac: strb.w r2,[r1,#0x48]
  001461b0: blx r11
  001461b2: movs r1,#0x1
  001461b4: blx 0x00076aa4
  001461b8: mov r0,r10
  001461ba: blx r11
  001461bc: movs r1,#0x0
  001461be: blx 0x00076a80
  001461c2: mov.w r0,#0x100
  001461c6: movs r1,#0x1
  001461c8: strh r0,[r5,#0x20]
  001461ca: mov r0,r8
  001461cc: blx 0x00072670
  001461d0: mov r0,r10
  001461d2: blx r11
  001461d4: ldr r1,[r0,#0x8]
  001461d6: mov r0,r8
  001461d8: blx 0x00076ac8
  001461dc: mov r0,r10
  001461de: blx 0x00071eb4
  001461e2: ldr r0,[r0,#0x4]
  001461e4: add r6,sp,#0xa0
  001461e6: ldr r1,[r0,#0x4]
  001461e8: ldr r0,[r1,#0x0]
  001461ea: ldr r2,[r0,#0x28]
  001461ec: mov r0,r6
  001461ee: blx r2
  001461f0: mov r0,r8
  001461f2: mov r1,r6
  001461f4: blx 0x00076ae0
  001461f8: ldr r1,[0x001462fc]
  001461fa: mov r0,r8
  001461fc: ldr r2,[0x001462d8]
  001461fe: ldr r3,[0x001462dc]
  00146200: blx 0x00072688
  00146204: mov r0,r10
  00146206: blx r11
  00146208: add.w r8,sp,#0x130
  0014620c: mov r1,r0
  0014620e: mov r0,r8
  00146210: blx 0x0007264c
  00146214: mov r0,r10
  00146216: blx r11
  00146218: add r6,sp,#0x118
  0014621a: mov r1,r0
  0014621c: mov r0,r6
  0014621e: blx 0x00073540
  00146222: add.w r9,sp,#0x124
  00146226: ldr r2,[0x00146300]
  00146228: mov r1,r6
  0014622a: mov r0,r9
  0014622c: blx 0x0006ec74
  00146230: add r4,sp,#0x64
  00146232: mov r1,r8
  00146234: mov r2,r9
  00146236: mov r0,r4
  00146238: blx 0x0006f1cc
  0014623c: mov r0,r10
  0014623e: blx r11
  00146240: add r6,sp,#0x100
  00146242: mov r1,r0
  00146244: mov r0,r6
  00146246: blx 0x0007354c
  0014624a: add.w r8,sp,#0x10c
  0014624e: ldr r2,[0x00146304]
  00146250: mov r1,r6
  00146252: mov r0,r8
  00146254: blx 0x0006ec74
  00146258: add r6,sp,#0xa0
  0014625a: mov r2,r8
  0014625c: mov r1,r4
  0014625e: ldr.w r8,[r7,#0x8]
  00146262: mov r0,r6
  00146264: blx 0x0006f1cc
  00146268: mov r0,r8
  0014626a: mov r1,r6
  0014626c: blx 0x00076ae0
  00146270: mov r0,r10
  00146272: blx r11
  00146274: ldr r0,[r0,#0x0]
  00146276: movs r1,#0x1
  00146278: mov r11,r5
  0014627a: blx 0x0007294c
  0014627e: ldr r0,[0x00146308]
  00146280: add r0,pc
  00146282: ldr r0,[r0,#0x0]
  00146284: ldrb r0,[r0,#0x0]
  00146286: cbnz r0,0x0014628e
  00146288: mov r0,r11
  0014628a: blx 0x00076af8
  0014628e: movs r0,#0x1
  00146290: movs r1,#0x0
  00146292: str.w r0,[r11,#0x1c]
  00146296: ldr r0,[sp,#0x4c]
  00146298: strd r1,r1,[r0,#0x0]
  0014629c: b 0x00146402
  0014630c: ldr.w r0,[r9,#0x0]
  00146310: blx 0x00071770
  00146314: cmp r0,#0x6a
  00146316: bne.w 0x00146a1c
  0014631a: ldr r0,[0x001466b0]
  0014631c: add r0,pc
  0014631e: ldr r5,[r0,#0x0]
  00146320: mov r0,r10
  00146322: blx r5
  00146324: ldr r1,[0x001466b4]
  00146326: movs r2,#0x0
  00146328: movs r4,#0x0
  0014632a: mov r3,r1
  0014632c: blx 0x000725bc
  00146330: ldr r0,[sp,#0x50]
  00146332: strb r4,[r0,#0x1]
  00146334: ldr r0,[sp,#0x54]
  00146336: strb.w r4,[r0,#0x48]
  0014633a: mov r0,r10
  0014633c: blx r5
  0014633e: movs r1,#0x1
  00146340: mov.w r9,#0x1
  00146344: blx 0x00076aa4
  00146348: mov r0,r10
  0014634a: blx r5
  0014634c: strb.w r9,[r0,#0x24]
  00146350: mov r0,r10
  00146352: blx r5
  00146354: ldr r0,[r0,#0x0]
  00146356: movs r1,#0x0
  00146358: blx 0x0007294c
  0014635c: mov r0,r10
  0014635e: blx 0x000727c0
  00146362: add r6,sp,#0x64
  00146364: mov r1,r0
  00146366: mov r0,r6
  00146368: blx 0x000727cc
  0014636c: add.w r8,sp,#0xa0
  00146370: mov r1,r6
  00146372: mov r0,r8
  00146374: blx 0x0006ec80
  00146378: mov r0,r10
  0014637a: blx r5
  0014637c: ldr r0,[r0,#0x8]
  0014637e: mov.w r1,#0x3f800000
  00146382: add r2,sp,#0x64
  00146384: strd r4,r1,[sp,#0x64]
  00146388: mov r1,r8
  0014638a: str r4,[sp,#0x6c]
  0014638c: blx 0x000726ac
  00146390: ldr r0,[0x001466b8]
  00146392: add r0,pc
  00146394: ldr r0,[r0,#0x0]
  00146396: ldr r6,[r0,#0x0]
  00146398: mov r0,r10
  0014639a: blx r5
  0014639c: ldr r1,[r0,#0x1c]
  0014639e: mov r0,r6
  001463a0: blx 0x000724a8
  001463a4: ldr r0,[r7,#0x8]
  001463a6: movs r1,#0x1
  001463a8: strb.w r9,[r11,#0x21]
  001463ac: blx 0x00072670
  001463b0: mov r0,r10
  001463b2: blx r5
  001463b4: add.w r8,sp,#0x130
  001463b8: mov r1,r0
  001463ba: mov r0,r8
  001463bc: blx 0x0007264c
  001463c0: ldr r2,[0x001466bc]
  001463c2: add r6,sp,#0x64
  001463c4: ldr r0,[0x001466c0]
  001463c6: ldr r1,[0x001466c4]
  001463c8: strd r0,r2,[sp,#0x124]
  001463cc: add r2,sp,#0x124
  001463ce: str r1,[sp,#0x12c]
  001463d0: mov r1,r8
  001463d2: mov r0,r6
  001463d4: ldr.w r8,[r7,#0x8]
  001463d8: blx 0x0006f1cc
  001463dc: mov r0,r8
  001463de: mov r1,r6
  001463e0: blx 0x00076ae0
  001463e4: ldr r0,[0x001466c8]
  001463e6: movs r1,#0x0
  001463e8: strb.w r9,[r11,#0x11]
  001463ec: mov.w r2,#0xffffffff
  001463f0: add r0,pc
  001463f2: mov.w r3,#0x1f40
  001463f6: ldr r0,[r0,#0x0]
  001463f8: ldr r0,[r0,#0x0]
  001463fa: blx 0x00076ad4
  001463fe: str.w r9,[r11,#0x1c]
  00146402: ldr.w r9,[sp,#0x44]
  00146406: ldr.w r0,[r9,#0x0]
  0014640a: blx 0x00071770
  0014640e: cmp r0,#0x50
  00146410: bne.w 0x00146598
  00146414: movs r0,#0xc0
  00146416: str.w r9,[sp,#0x44]
  0014641a: blx 0x0006eb24
  0014641e: mov r6,r0
  00146420: ldr r0,[0x001466cc]
  00146422: add r0,pc
  00146424: ldr.w r8,[r0,#0x0]
  00146428: ldr.w r2,[r8,#0x0]
  0014642c: mov r0,r6
  0014642e: movw r1,#0x3822
  00146432: movs r3,#0x0
  00146434: mov.w r9,#0x0
  00146438: blx 0x0007207c
  0014643c: ldr r2,[0x001466d0]
  0014643e: str.w r6,[r11,#0xb8]
  00146442: add r2,pc
  00146444: ldr r1,[r6,#0xc]
  00146446: ldr.w r0,[r8,#0x0]
  0014644a: ldr r4,[r2,#0x0]
  0014644c: blx r4
  0014644e: ldr r1,[0x001466d4]
  00146450: str.w r1,[r0,#0xe0]
  00146454: ldr.w r1,[r11,#0xb8]
  00146458: ldr.w r0,[r8,#0x0]
  0014645c: ldr r1,[r1,#0xc]
  0014645e: blx r4
  00146460: ldr r1,[0x001466d8]
  00146462: movs r2,#0x0
  00146464: add r1,pc
  00146466: ldr r5,[r1,#0x0]
  00146468: movs r1,#0x1
  0014646a: blx r5
  0014646c: ldr.w r0,[r11,#0xb8]
  00146470: movs r1,#0x0
  00146472: blx 0x00072d24
  00146476: ldr r1,[0x001466dc]
  00146478: ldr.w r0,[r11,#0xb8]
  0014647c: strd r9,r1,[sp,#0xa0]
  00146480: add r1,sp,#0xa0
  00146482: str.w r9,[sp,#0xa8]
  00146486: blx 0x00073dd4
  0014648a: mov r0,r10
  0014648c: blx 0x000725b0
  00146490: ldr r0,[r0,#0x4]
  00146492: ldr r0,[r0,#0x0]
  00146494: ldr.w r6,[r0,#0x140]
  00146498: ldr.w r0,[r8,#0x0]
  0014649c: ldr r1,[r6,#0xc]
  0014649e: blx r4
  001464a0: movs r1,#0x1
  001464a2: movs r2,#0x0
  001464a4: blx r5
  001464a6: ldr r1,[r6,#0x14]
  001464a8: ldr.w r0,[r8,#0x0]
  001464ac: blx r4
  001464ae: movs r1,#0x1
  001464b0: movs r2,#0x0
  001464b2: blx r5
  001464b4: ldr r1,[r6,#0x10]
  001464b6: ldr.w r0,[r8,#0x0]
  001464ba: blx r4
  001464bc: movs r1,#0x1
  001464be: movs r2,#0x0
  001464c0: blx r5
  001464c2: mov r0,r10
  001464c4: ldr.w r6,[r8,#0x0]
  001464c8: blx 0x000725b0
  001464cc: ldr r0,[r0,#0x4]
  001464ce: ldr r0,[r0,#0x0]
  001464d0: ldr.w r1,[r0,#0x144]
  001464d4: mov r0,r6
  001464d6: blx r4
  001464d8: movs r1,#0x2
  001464da: movs r2,#0x0
  001464dc: blx r5
  001464de: movs r0,#0x68
  001464e0: blx 0x0006eb24
  001464e4: mov r6,r0
  001464e6: movs r1,#0x0
  001464e8: blx 0x000727f0
  001464ec: mov r0,r6
  001464ee: str.w r6,[r11,#0xc8]
  001464f2: blx 0x0007579c
  001464f6: ldr.w r0,[r11,#0xc8]
  001464fa: ldr r1,[0x001466e0]
  001464fc: blx 0x000727fc
  00146500: movs r0,#0xc0
  00146502: blx 0x0006eb24
  00146506: ldr.w r1,[r8,#0x0]
  0014650a: mov r6,r0
  0014650c: blx 0x000720f4
  00146510: ldr.w r9,[sp,#0x44]
  00146514: mov r0,r6
  00146516: movs r1,#0x0
  00146518: str.w r6,[r11,#0xdc]
  0014651c: blx 0x00072d24
  00146520: movs r0,#0xc0
  00146522: blx 0x0006eb24
  00146526: mov r6,r0
  00146528: ldr.w r2,[r8,#0x0]
  0014652c: movw r1,#0x381e
  00146530: movs r3,#0x0
  00146532: blx 0x0007207c
  00146536: mov r0,r10
  00146538: str.w r6,[r11,#0xbc]
  0014653c: blx 0x00071eb4
  00146540: ldr r0,[r0,#0x4]
  00146542: add r4,sp,#0xa0
  00146544: ldr r0,[r0,#0x0]
  00146546: ldr.w r1,[r0,#0x140]
  0014654a: mov r0,r4
  0014654c: blx 0x000720b8
  00146550: mov r0,r6
  00146552: mov r1,r4
  00146554: blx 0x00072148
  00146558: ldr.w r1,[r11,#0xbc]
  0014655c: ldr.w r0,[r8,#0x0]
  00146560: ldr r1,[r1,#0xc]
  00146562: blx 0x00072088
  00146566: movs r1,#0x1
  00146568: movs r2,#0x0
  0014656a: blx 0x0006fd18
  0014656e: movs r0,#0xc0
  00146570: blx 0x0006eb24
  00146574: mov r6,r0
  00146576: ldr.w r2,[r8,#0x0]
  0014657a: movw r1,#0x3ab3
  0014657e: movs r3,#0x0
  00146580: blx 0x0007207c
  00146584: ldr r1,[0x001466e4]
  00146586: mov r0,r6
  00146588: str.w r6,[r11,#0xd8]
  0014658c: mov r2,r1
  0014658e: mov r3,r1
  00146590: blx 0x000727b4
  00146594: ldr.w r8,[r7,#0x8]
  00146598: ldr.w r0,[r9,#0x0]
  0014659c: blx 0x00071770
  001465a0: cmp r0,#0x29
  001465a2: bne.w 0x001466f4
  001465a6: ldr.w r0,[r9,#0x0]
  001465aa: blx 0x000723d0
  001465ae: cmp r0,#0x0
  001465b0: beq.w 0x001466f4
  001465b4: mov r0,r10
  001465b6: ldr.w r4,[r10,#0x74]
  001465ba: blx 0x00071eb4
  001465be: ldr r0,[r0,#0x4]
  001465c0: ldr r0,[r0,#0x0]
  001465c2: ldr r0,[r0,#0x8]
  001465c4: blx 0x00072d00
  001465c8: mov r1,r0
  001465ca: mov r0,r4
  001465cc: movs r2,#0x28
  001465ce: movs r3,#0x0
  001465d0: blx 0x000724d8
  001465d4: str.w r0,[r11,#0xe0]
  001465d8: mov r1,r0
  001465da: ldr.w r0,[r10,#0x74]
  001465de: movs r2,#0x0
  001465e0: blx 0x000723c4
  001465e4: mov r0,r10
  001465e6: ldr.w r4,[r10,#0x74]
  001465ea: blx 0x00071eb4
  001465ee: ldr r0,[r0,#0x4]
  001465f0: ldr r0,[r0,#0x0]
  001465f2: ldr r0,[r0,#0x8]
  001465f4: blx 0x00072d00
  001465f8: mov r1,r0
  001465fa: mov r0,r4
  001465fc: movs r2,#0x29
  001465fe: movs r3,#0x0
  00146600: blx 0x000724d8
  00146604: str.w r0,[r11,#0xe4]
  00146608: mov r1,r0
  0014660a: ldr.w r0,[r10,#0x74]
  0014660e: movs r2,#0x0
  00146610: blx 0x000723c4
  00146614: ldr.w r0,[r10,#0x0]
  00146618: movs r1,#0x1e
  0014661a: movs r2,#0x0
  0014661c: blx 0x00071818
  00146620: movs r0,#0xc0
  00146622: blx 0x0006eb24
  00146626: mov r6,r0
  00146628: ldr r0,[0x001466e8]
  0014662a: add r0,pc
  0014662c: ldr r4,[r0,#0x0]
  0014662e: ldr r2,[r4,#0x0]
  00146630: mov r0,r6
  00146632: movw r1,#0x37cd
  00146636: movs r3,#0x0
  00146638: blx 0x0007207c
  0014663c: ldr r0,[sp,#0x40]
  0014663e: str r6,[r0,#0x0]
  00146640: movs r0,#0xc0
  00146642: blx 0x0006eb24
  00146646: mov r6,r0
  00146648: ldr r2,[r4,#0x0]
  0014664a: movw r1,#0x37ce
  0014664e: movs r3,#0x0
  00146650: blx 0x0007207c
  00146654: movs r0,#0xc0
  00146656: str.w r6,[r11,#0xb0]
  0014665a: blx 0x0006eb24
  0014665e: mov r6,r0
  00146660: ldr r2,[r4,#0x0]
  00146662: movw r1,#0x37cf
  00146666: movs r3,#0x0
  00146668: blx 0x0007207c
  0014666c: ldr.w r0,[r11,#0xac]
  00146670: ldr r2,[0x001466ec]
  00146672: str.w r6,[r11,#0xb4]
  00146676: add r2,pc
  00146678: ldr r1,[r0,#0xc]
  0014667a: ldr r0,[r4,#0x0]
  0014667c: ldr r5,[r2,#0x0]
  0014667e: blx r5
  00146680: ldr r1,[0x001466f0]
  00146682: movs r2,#0x0
  00146684: add r1,pc
  00146686: ldr r6,[r1,#0x0]
  00146688: movs r1,#0x1
  0014668a: blx r6
  0014668c: ldr.w r0,[r11,#0xb0]
  00146690: ldr r1,[r0,#0xc]
  00146692: ldr r0,[r4,#0x0]
  00146694: blx r5
  00146696: movs r1,#0x1
  00146698: movs r2,#0x0
  0014669a: blx r6
  0014669c: ldr.w r1,[r11,#0xb4]
  001466a0: ldr r0,[r4,#0x0]
  001466a2: ldr r1,[r1,#0xc]
  001466a4: blx r5
  001466a6: movs r1,#0x1
  001466a8: movs r2,#0x0
  001466aa: blx r6
  001466ac: b 0x00146874
  001466f4: ldr.w r0,[r9,#0x0]
  001466f8: blx 0x00071770
  001466fc: cmp r0,#0x9d
  001466fe: bne.w 0x00146874
  00146702: ldr.w r0,[r9,#0x0]
  00146706: blx 0x000723d0
  0014670a: cmp r0,#0x0
  0014670c: bne.w 0x00146874
  00146710: ldr.w r0,[r9,#0x0]
  00146714: blx 0x00071c14
  00146718: blx 0x00071824
  0014671c: mov r4,r0
  0014671e: ldr.w r0,[r9,#0x0]
  00146722: blx 0x0007285c
  00146726: blx 0x00073738
  0014672a: cmp r4,r0
  0014672c: bne.w 0x00146874
  00146730: movs r0,#0xc0
  00146732: blx 0x0006eb24
  00146736: mov r6,r0
  00146738: ldr r0,[0x001469f8]
  0014673a: add r0,pc
  0014673c: ldr.w r8,[r0,#0x0]
  00146740: ldr.w r2,[r8,#0x0]
  00146744: mov r0,r6
  00146746: movw r1,#0x4a75
  0014674a: movs r3,#0x0
  0014674c: blx 0x0007207c
  00146750: movs r0,#0xc0
  00146752: str.w r11,[sp,#0x38]
  00146756: str.w r6,[r11,#0xbc]
  0014675a: blx 0x0006eb24
  0014675e: mov r11,r0
  00146760: ldr.w r2,[r8,#0x0]
  00146764: movw r1,#0x4a76
  00146768: movs r3,#0x0
  0014676a: blx 0x0007207c
  0014676e: movs r0,#0xc0
  00146770: blx 0x0006eb24
  00146774: mov r5,r0
  00146776: ldr.w r2,[r8,#0x0]
  0014677a: movw r1,#0x4a77
  0014677e: movs r3,#0x0
  00146780: blx 0x0007207c
  00146784: ldr r4,[sp,#0x38]
  00146786: ldr.w r1,[r11,#0xc]
  0014678a: ldr.w r0,[r4,#0xbc]
  0014678e: blx 0x000720e8
  00146792: ldr r1,[r5,#0xc]
  00146794: ldr.w r0,[r4,#0xbc]
  00146798: blx 0x000720e8
  0014679c: movs r0,#0xc0
  0014679e: blx 0x0006eb24
  001467a2: mov r6,r0
  001467a4: ldr.w r2,[r8,#0x0]
  001467a8: movw r1,#0x4a97
  001467ac: movs r3,#0x0
  001467ae: blx 0x0007207c
  001467b2: movs r0,#0xc0
  001467b4: str r5,[sp,#0x40]
  001467b6: str.w r6,[r4,#0xc0]
  001467ba: blx 0x0006eb24
  001467be: mov r6,r0
  001467c0: ldr.w r2,[r8,#0x0]
  001467c4: movw r1,#0x4a99
  001467c8: movs r3,#0x0
  001467ca: blx 0x0007207c
  001467ce: movs r0,#0xc0
  001467d0: str.w r9,[sp,#0x44]
  001467d4: str.w r6,[r4,#0xc4]
  001467d8: blx 0x0006eb24
  001467dc: mov r5,r0
  001467de: ldr.w r2,[r8,#0x0]
  001467e2: movw r1,#0x4a98
  001467e6: movs r3,#0x0
  001467e8: blx 0x0007207c
  001467ec: movs r0,#0xc0
  001467ee: mov r6,r11
  001467f0: blx 0x0006eb24
  001467f4: mov r9,r0
  001467f6: ldr.w r2,[r8,#0x0]
  001467fa: movw r1,#0x4a9a
  001467fe: movs r3,#0x0
  00146800: blx 0x0007207c
  00146804: ldr.w r11,[sp,#0x38]
  00146808: ldr r1,[r5,#0xc]
  0014680a: ldr.w r0,[r11,#0xc0]
  0014680e: blx 0x000720e8
  00146812: ldr.w r1,[r9,#0xc]
  00146816: ldr.w r0,[r11,#0xc4]
  0014681a: blx 0x000720e8
  0014681e: ldr.w r1,[r11,#0xbc]
  00146822: ldr.w r0,[r8,#0x0]
  00146826: ldr r1,[r1,#0xc]
  00146828: blx 0x00072088
  0014682c: movs r1,#0x0
  0014682e: blx 0x0007249c
  00146832: ldr.w r0,[r11,#0xc0]
  00146836: movs r1,#0x0
  00146838: blx 0x00072d24
  0014683c: ldr.w r0,[r11,#0xc4]
  00146840: movs r1,#0x0
  00146842: blx 0x00072d24
  00146846: ldr r0,[0x001469fc]
  00146848: mov r4,r5
  0014684a: add r0,pc
  0014684c: ldr r5,[r0,#0x0]
  0014684e: mov r0,r6
  00146850: blx r5
  00146852: ldr r1,[0x00146a00]
  00146854: add r1,pc
  00146856: ldr r6,[r1,#0x0]
  00146858: blx r6
  0014685a: ldr r0,[sp,#0x40]
  0014685c: blx r5
  0014685e: blx r6
  00146860: mov r0,r4
  00146862: blx r5
  00146864: blx r6
  00146866: mov r0,r9
  00146868: blx r5
  0014686a: blx r6
  0014686c: ldr.w r8,[r7,#0x8]
  00146870: ldr.w r9,[sp,#0x44]
  00146874: ldr r0,[0x00146a04]
  00146876: movs r1,#0x0
  00146878: add r0,pc
  0014687a: ldr r0,[r0,#0x0]
  0014687c: ldr r0,[r0,#0x0]
  0014687e: blx 0x00076b04
  00146882: ldr.w r0,[r9,#0x0]
  00146886: blx 0x000723d0
  0014688a: cbnz r0,0x001468de
  0014688c: ldr.w r0,[r9,#0x0]
  00146890: blx 0x0007285c
  00146894: cbz r0,0x001468de
  00146896: ldr.w r0,[r9,#0x0]
  0014689a: blx 0x0007285c
  0014689e: blx 0x00072874
  001468a2: cmp r0,#0xa3
  001468a4: bne 0x001468de
  001468a6: ldr.w r0,[r9,#0x0]
  001468aa: ldr.w r1,[r0,#0x90]
  001468ae: cbz r1,0x001468de
  001468b0: movs r4,#0x0
  001468b2: b 0x001468d0
  001468b4: ldr r1,[r1,#0x4]
  001468b6: ldr.w r5,[r1,r4,lsl #0x2]
  001468ba: blx 0x00071c14
  001468be: blx 0x00071824
  001468c2: cmp r5,r0
  001468c4: beq 0x001468d8
  001468c6: ldr.w r0,[r9,#0x0]
  001468ca: adds r4,#0x1
  001468cc: ldr.w r1,[r0,#0x90]
  001468d0: ldr r2,[r1,#0x0]
  001468d2: cmp r4,r2
  001468d4: bcc 0x001468b4
  001468d6: b 0x001468de
  001468d8: movs r0,#0x1
  001468da: strb.w r0,[r11,#0xa9]
  001468de: ldr.w r0,[r9,#0x0]
  001468e2: blx 0x00071770
  001468e6: cmp r0,#0x5f
  001468e8: beq 0x0014693e
  001468ea: ldr.w r0,[r9,#0x0]
  001468ee: blx 0x00071770
  001468f2: cmp r0,#0x63
  001468f4: beq 0x0014693e
  001468f6: ldr.w r0,[r9,#0x0]
  001468fa: blx 0x00071770
  001468fe: cmp r0,#0x6d
  00146900: beq 0x0014693e
  00146902: ldr.w r0,[r9,#0x0]
  00146906: blx 0x00071770
  0014690a: cmp r0,#0x77
  0014690c: beq 0x0014693e
  0014690e: ldr.w r0,[r9,#0x0]
  00146912: blx 0x00071770
  00146916: cmp r0,#0x7e
  00146918: beq 0x0014693e
  0014691a: ldr.w r0,[r9,#0x0]
  0014691e: blx 0x00071770
  00146922: cmp r0,#0x85
  00146924: beq 0x0014693e
  00146926: ldr.w r0,[r9,#0x0]
  0014692a: blx 0x00071770
  0014692e: cmp r0,#0xa0
  00146930: beq 0x0014693e
  00146932: ldr.w r0,[r9,#0x0]
  00146936: blx 0x00071770
  0014693a: cmp r0,#0xa1
  0014693c: bne 0x001469d6
  0014693e: ldr r0,[0x00146a08]
  00146940: mov.w r9,#0x0
  00146944: ldr r1,[sp,#0x50]
  00146946: movs r5,#0x1
  00146948: add r0,pc
  0014694a: strb.w r9,[r1,#0x1]
  0014694e: ldr r1,[sp,#0x54]
  00146950: ldr r6,[r0,#0x0]
  00146952: mov r0,r10
  00146954: strb.w r9,[r1,#0x48]
  00146958: strb.w r5,[r11,#0x11]
  0014695c: blx r6
  0014695e: movs r1,#0x1
  00146960: blx 0x00076aa4
  00146964: mov r0,r10
  00146966: blx r6
  00146968: strb.w r5,[r0,#0x24]
  0014696c: mov r0,r10
  0014696e: blx r6
  00146970: ldr r0,[r0,#0x0]
  00146972: movs r1,#0x0
  00146974: blx 0x0007294c
  00146978: mov r0,r10
  0014697a: blx r6
  0014697c: movs r1,#0x0
  0014697e: blx 0x00076aec
  00146982: ldr r0,[0x00146a0c]
  00146984: add r0,pc
  00146986: ldr r0,[r0,#0x0]
  00146988: ldr r4,[r0,#0x0]
  0014698a: mov r0,r10
  0014698c: blx r6
  0014698e: ldr r1,[r0,#0x1c]
  00146990: mov r0,r4
  00146992: blx 0x000724a8
  00146996: mov r0,r8
  00146998: movs r1,#0x1
  0014699a: strb.w r5,[r11,#0x21]
  0014699e: blx 0x00072670
  001469a2: ldr r2,[0x00146a10]
  001469a4: ldr r0,[0x00146a14]
  001469a6: ldr r1,[0x00146a18]
  001469a8: strd r0,r2,[sp,#0xa0]
  001469ac: mov r0,r8
  001469ae: str r1,[sp,#0xa8]
  001469b0: add r1,sp,#0xa0
  001469b2: blx 0x00076ae0
  001469b6: mov r0,r10
  001469b8: blx r6
  001469ba: movs r1,#0x0
  001469bc: movs r2,#0x0
  001469be: movs r3,#0x0
  001469c0: blx 0x000725bc
  001469c4: ldr.w r0,[r10,#0x0]
  001469c8: movs r1,#0x1e
  001469ca: movs r2,#0x0
  001469cc: blx 0x00071818
  001469d0: ldr r0,[sp,#0x4c]
  001469d2: strd r9,r9,[r0,#0x0]
  001469d6: ldr r0,[sp,#0x13c]
  001469d8: ldr r1,[sp,#0x48]
  001469da: ldr r1,[r1,#0x0]
  001469dc: subs r0,r1,r0
  001469de: itttt eq
  001469e0: mov.eq r0,r11
  001469e2: add.eq sp,#0x140
  001469e4: vpop.eq {d8,d9}
  001469e8: add.eq sp,#0x4
  001469ea: itt eq
  001469ec: pop.eq.w {r8,r9,r10,r11}
  001469f0: pop.eq {r4,r5,r6,r7,pc}
  001469f2: blx 0x0006e824
  00146a1c: ldr.w r0,[r9,#0x0]
  00146a20: blx 0x00071770
  00146a24: cmp r0,#0x90
  00146a26: bne.w 0x00146b4a
  00146a2a: movs r0,#0xc0
  00146a2c: blx 0x0006eb24
  00146a30: mov r6,r0
  00146a32: ldr r0,[0x00146d50]
  00146a34: add r0,pc
  00146a36: ldr r0,[r0,#0x0]
  00146a38: ldr r1,[r0,#0x0]
  00146a3a: mov r0,r6
  00146a3c: blx 0x000720f4
  00146a40: ldr r1,[0x00146d54]
  00146a42: mov r0,r6
  00146a44: ldr r2,[0x00146d58]
  00146a46: ldr r3,[0x00146d5c]
  00146a48: str.w r6,[r11,#0xcc]
  00146a4c: blx 0x00073048
  00146a50: ldr.w r0,[r11,#0xcc]
  00146a54: mov.w r9,#0x0
  00146a58: ldr r1,[0x00146d60]
  00146a5a: mov.w r2,#0x3f800000
  00146a5e: str.w r9,[sp,#0xa4]
  00146a62: str r2,[sp,#0xa0]
  00146a64: str r1,[sp,#0xa8]
  00146a66: add r1,sp,#0xa0
  00146a68: strd r9,r2,[sp,#0x64]
  00146a6c: add r2,sp,#0x64
  00146a6e: str.w r9,[sp,#0x6c]
  00146a72: blx 0x000726ac
  00146a76: ldr r0,[0x00146d64]
  00146a78: mov.w r8,#0x1
  00146a7c: strb.w r8,[r11,#0x11]
  00146a80: add r0,pc
  00146a82: ldr r5,[r0,#0x0]
  00146a84: ldr r0,[sp,#0x50]
  00146a86: strb.w r9,[r0,#0x1]
  00146a8a: ldr r0,[sp,#0x54]
  00146a8c: strb.w r9,[r0,#0x48]
  00146a90: mov r0,r10
  00146a92: blx r5
  00146a94: movs r1,#0x1
  00146a96: blx 0x00076aa4
  00146a9a: mov r0,r10
  00146a9c: blx r5
  00146a9e: movs r1,#0x0
  00146aa0: blx 0x00076a80
  00146aa4: mov r0,r10
  00146aa6: blx r5
  00146aa8: movs r1,#0x0
  00146aaa: blx 0x00072040
  00146aae: mov r0,r10
  00146ab0: blx r5
  00146ab2: movs r1,#0x0
  00146ab4: blx 0x00076aec
  00146ab8: mov r0,r10
  00146aba: blx r5
  00146abc: movs r1,#0x1
  00146abe: blx 0x00076a98
  00146ac2: ldr r0,[0x00146d68]
  00146ac4: mov.w r1,#0x100
  00146ac8: strh.w r1,[r11,#0x20]
  00146acc: add r0,pc
  00146ace: ldr r0,[r0,#0x0]
  00146ad0: ldr r6,[r0,#0x0]
  00146ad2: mov r0,r10
  00146ad4: blx r5
  00146ad6: ldr r1,[r0,#0x1c]
  00146ad8: mov r0,r6
  00146ada: blx 0x000724a8
  00146ade: ldr r5,[r7,#0x8]
  00146ae0: movs r1,#0x1
  00146ae2: mov r0,r5
  00146ae4: blx 0x00072670
  00146ae8: ldr.w r1,[r11,#0xcc]
  00146aec: mov r0,r5
  00146aee: blx 0x00076ac8
  00146af2: mov r0,r10
  00146af4: blx 0x00071eb4
  00146af8: ldr r0,[r0,#0x4]
  00146afa: add r6,sp,#0xa0
  00146afc: ldr r1,[r0,#0x0]
  00146afe: ldr r0,[r1,#0x0]
  00146b00: ldr r2,[r0,#0x28]
  00146b02: mov r0,r6
  00146b04: blx r2
  00146b06: ldr r4,[sp,#0x3c]
  00146b08: mov r1,r6
  00146b0a: mov r0,r4
  00146b0c: blx 0x0006eb3c
  00146b10: ldr r2,[0x00146d6c]
  00146b12: ldr r0,[0x00146d70]
  00146b14: ldr r1,[0x00146d74]
  00146b16: strd r0,r2,[sp,#0xa0]
  00146b1a: mov r0,r4
  00146b1c: str r1,[sp,#0xa8]
  00146b1e: add r1,sp,#0xa0
  00146b20: blx 0x00073534
  00146b24: mov r0,r5
  00146b26: mov r1,r4
  00146b28: blx 0x00076ae0
  00146b2c: ldr r0,[0x00146d78]
  00146b2e: add r0,pc
  00146b30: ldr r0,[r0,#0x0]
  00146b32: ldrb r0,[r0,#0x0]
  00146b34: cbnz r0,0x00146b3c
  00146b36: mov r0,r11
  00146b38: blx 0x00076af8
  00146b3c: ldr r0,[sp,#0x4c]
  00146b3e: str.w r8,[r11,#0x1c]
  00146b42: strd r9,r9,[r0,#0x0]
  00146b46: b.w 0x00146170
  00146b4a: ldr.w r0,[r9,#0x0]
  00146b4e: blx 0x00071770
  00146b52: cmp r0,#0x83
  00146b54: bne.w 0x00146d9c
  00146b58: ldr.w r0,[r9,#0x0]
  00146b5c: blx 0x000723d0
  00146b60: cmp r0,#0x0
  00146b62: bne.w 0x00146d9c
  00146b66: ldr.w r0,[r9,#0x0]
  00146b6a: blx 0x00071c14
  00146b6e: blx 0x00071824
  00146b72: cmp r0,#0x70
  00146b74: bne.w 0x00146d9c
  00146b78: ldr r0,[0x00146d7c]
  00146b7a: movs r1,#0x1
  00146b7c: strb.w r1,[r11,#0x11]
  00146b80: movs r4,#0x0
  00146b82: ldr r1,[sp,#0x50]
  00146b84: add r0,pc
  00146b86: ldr r5,[r0,#0x0]
  00146b88: mov r0,r10
  00146b8a: strb r4,[r1,#0x1]
  00146b8c: ldr r1,[sp,#0x54]
  00146b8e: strb.w r4,[r1,#0x48]
  00146b92: blx r5
  00146b94: movs r1,#0x1
  00146b96: blx 0x00076aa4
  00146b9a: mov r0,r10
  00146b9c: blx r5
  00146b9e: movs r1,#0x0
  00146ba0: blx 0x00076a80
  00146ba4: mov r0,r10
  00146ba6: blx r5
  00146ba8: ldr r1,[0x00146d80]
  00146baa: movs r2,#0x0
  00146bac: ldr r3,[0x00146d84]
  00146bae: blx 0x000725bc
  00146bb2: mov r0,r10
  00146bb4: blx r5
  00146bb6: ldr.w r8,[r0,#0x8]
  00146bba: mov r0,r10
  00146bbc: blx r5
  00146bbe: add r6,sp,#0x130
  00146bc0: mov r1,r0
  00146bc2: mov r0,r6
  00146bc4: blx 0x0007264c
  00146bc8: add.w r9,sp,#0x64
  00146bcc: mov r1,r6
  00146bce: mov r0,r9
  00146bd0: blx 0x0006f4cc
  00146bd4: add r6,sp,#0xa0
  00146bd6: mov r1,r9
  00146bd8: mov r0,r6
  00146bda: blx 0x0006ec80
  00146bde: mov.w r0,#0x3f800000
  00146be2: add r2,sp,#0x124
  00146be4: strd r4,r0,[sp,#0x124]
  00146be8: mov r0,r8
  00146bea: mov r1,r6
  00146bec: str r4,[sp,#0x12c]
  00146bee: blx 0x000726ac
  00146bf2: mov.w r0,#0x100
  00146bf6: movs r1,#0x1
  00146bf8: strh.w r0,[r11,#0x20]
  00146bfc: ldr r0,[r7,#0x8]
  00146bfe: blx 0x00072670
  00146c02: mov r0,r10
  00146c04: blx r5
  00146c06: ldr r1,[r0,#0x8]
  00146c08: ldr r0,[r7,#0x8]
  00146c0a: blx 0x00076ac8
  00146c0e: mov r0,r10
  00146c10: blx r5
  00146c12: add.w r8,sp,#0x124
  00146c16: mov r1,r0
  00146c18: mov r0,r8
  00146c1a: blx 0x0007264c
  00146c1e: mov r0,r10
  00146c20: blx r5
  00146c22: add.w r9,sp,#0x10c
  00146c26: mov r1,r0
  00146c28: mov r0,r9
  00146c2a: blx 0x00073540
  00146c2e: ldr r0,[0x00146d88]
  00146c30: add r6,sp,#0x118
  00146c32: ldr r2,[0x00146d8c]
  00146c34: mov r1,r9
  00146c36: add r0,pc
  00146c38: str.w r11,[sp,#0x38]
  00146c3c: ldr.w r11,[r0,#0x0]
  00146c40: mov r0,r6
  00146c42: blx r11
  00146c44: add.w r9,sp,#0x130
  00146c48: mov r1,r8
  00146c4a: mov r2,r6
  00146c4c: mov r0,r9
  00146c4e: blx 0x0006ec38
  00146c52: mov r0,r10
  00146c54: blx r5
  00146c56: add r6,sp,#0xf4
  00146c58: mov r1,r0
  00146c5a: mov r0,r6
  00146c5c: blx 0x0007354c
  00146c60: add.w r8,sp,#0x100
  00146c64: ldr r2,[0x00146d90]
  00146c66: mov r1,r6
  00146c68: mov r0,r8
  00146c6a: blx r11
  00146c6c: add r4,sp,#0x64
  00146c6e: mov r1,r9
  00146c70: mov r2,r8
  00146c72: mov r0,r4
  00146c74: blx 0x0006f1cc
  00146c78: mov r0,r10
  00146c7a: blx r5
  00146c7c: ldr r1,[r0,#0x8]
  00146c7e: add r6,sp,#0xdc
  00146c80: mov r0,r6
  00146c82: blx 0x0007261c
  00146c86: add.w r8,sp,#0xe8
  00146c8a: ldr r2,[0x00146d94]
  00146c8c: mov r1,r6
  00146c8e: mov r0,r8
  00146c90: blx r11
  00146c92: add r6,sp,#0xa0
  00146c94: mov r1,r4
  00146c96: mov r2,r8
  00146c98: mov r0,r6
  00146c9a: blx 0x0006ec38
  00146c9e: ldr r0,[r7,#0x8]
  00146ca0: mov r1,r6
  00146ca2: blx 0x00076ae0
  00146ca6: ldr r0,[r7,#0x8]
  00146ca8: movs r1,#0xa
  00146caa: blx 0x000721a8
  00146cae: ldr r0,[0x00146d98]
  00146cb0: add r0,pc
  00146cb2: ldr r0,[r0,#0x0]
  00146cb4: ldr r6,[r0,#0x0]
  00146cb6: mov r0,r6
  00146cb8: blx 0x000717f4
  00146cbc: mov r1,r0
  00146cbe: mov r0,r6
  00146cc0: blx 0x0006ff1c
  00146cc4: add.w r8,sp,#0xa0
  00146cc8: mov r1,r0
  00146cca: movs r2,#0x3c
  00146ccc: mov r0,r8
  00146cce: blx 0x0006f1e4
  00146cd2: add r6,sp,#0x64
  00146cd4: mov r1,r8
  00146cd6: mov r0,r6
  00146cd8: blx 0x0006f16c
  00146cdc: ldr r4,[sp,#0x3c]
  00146cde: mov r1,r6
  00146ce0: mov r0,r4
  00146ce2: blx 0x0006eb3c
  00146ce6: ldr r0,[r7,#0x8]
  00146ce8: movs r1,#0x1
  00146cea: blx 0x00076b10
  00146cee: add r0,sp,#0x64
  00146cf0: mov r1,r8
  00146cf2: mov r2,r4
  00146cf4: blx 0x0006f1d8
  00146cf8: ldr r0,[sp,#0xa0]
  00146cfa: add r3,sp,#0xcc
  00146cfc: str r0,[sp,#0x3c]
  00146cfe: ldr r0,[sp,#0xa4]
  00146d00: str r0,[sp,#0x34]
  00146d02: ldr r0,[sp,#0xa8]
  00146d04: str r0,[sp,#0x30]
  00146d06: ldrd r11,r6,[sp,#0xac]
  00146d0a: ldrd r5,r4,[sp,#0xb4]
  00146d0e: ldrd r12,lr,[sp,#0xbc]
  00146d12: ldrd r8,r9,[sp,#0xc4]
  00146d16: ldmia r3,{r0,r1,r2,r3}
  00146d18: str r0,[sp,#0x20]
  00146d1a: add r0,sp,#0xc
  00146d1c: str.w r11,[sp,#0x0]
  00146d20: str r1,[sp,#0x24]
  00146d22: str r2,[sp,#0x28]
  00146d24: str r3,[sp,#0x2c]
  00146d26: ldr.w r11,[sp,#0x38]
  00146d2a: strd r6,r5,[sp,#0x4]
  00146d2e: stm r0,{r4,r12,lr}
  00146d32: str.w r8,[sp,#0x18]
  00146d36: ldr.w r8,[r7,#0x8]
  00146d3a: str.w r9,[sp,#0x1c]
  00146d3e: ldr r1,[sp,#0x3c]
  00146d40: ldrd r3,r2,[sp,#0x30]
  00146d44: mov r0,r8
  00146d46: blx 0x00076b1c
  00146d4a: b.w 0x0014627e
  00146d9c: ldr.w r0,[r9,#0x0]
  00146da0: blx 0x00071770
  00146da4: cmp r0,#0x9e
  00146da6: bne.w 0x001470f8
  00146daa: ldr.w r0,[r9,#0x0]
  00146dae: blx 0x000723d0
  00146db2: cmp r0,#0x0
  00146db4: bne.w 0x001470f8
  00146db8: ldr.w r0,[r9,#0x0]
  00146dbc: blx 0x00071c14
  00146dc0: blx 0x00071824
  00146dc4: cmp r0,#0x6f
  00146dc6: bne.w 0x001470f8
  00146dca: movs r0,#0xc0
  00146dcc: blx 0x0006eb24
  00146dd0: mov r6,r0
  00146dd2: ldr r0,[0x001470b8]
  00146dd4: add r0,pc
  00146dd6: ldr r0,[r0,#0x0]
  00146dd8: ldr r1,[r0,#0x0]
  00146dda: mov r0,r6
  00146ddc: blx 0x000720f4
  00146de0: ldr r0,[0x001470bc]
  00146de2: movs r3,#0x0
  00146de4: ldr r1,[0x001470c0]
  00146de6: mov.w r2,#0x3f800000
  00146dea: add r0,pc
  00146dec: str.w r6,[r11,#0xcc]
  00146df0: str r3,[sp,#0xa4]
  00146df2: ldr.w r8,[r0,#0x0]
  00146df6: mov r0,r6
  00146df8: str r2,[sp,#0xa0]
  00146dfa: str r1,[sp,#0xa8]
  00146dfc: add r1,sp,#0xa0
  00146dfe: strd r3,r2,[sp,#0x64]
  00146e02: add r2,sp,#0x64
  00146e04: str r3,[sp,#0x6c]
  00146e06: blx r8
  00146e08: ldr r0,[0x001470c4]
  00146e0a: add r0,pc
  00146e0c: ldr r4,[r0,#0x0]
  00146e0e: mov.w r0,#0x100
  00146e12: strh.w r0,[r11,#0x20]
  00146e16: mov r0,r10
  00146e18: blx r4
  00146e1a: ldr r1,[0x001470c8]
  00146e1c: movs r2,#0x0
  00146e1e: ldr r3,[0x001470cc]
  00146e20: mov r5,r4
  00146e22: blx 0x000725bc
  00146e26: ldr r0,[sp,#0x50]
  00146e28: movs r1,#0x0
  00146e2a: strb r1,[r0,#0x1]
  00146e2c: ldr r0,[sp,#0x54]
  00146e2e: strb.w r1,[r0,#0x48]
  00146e32: mov r0,r10
  00146e34: blx r4
  00146e36: movs r1,#0x1
  00146e38: movs r4,#0x1
  00146e3a: blx 0x00076a98
  00146e3e: mov r0,r10
  00146e40: blx r5
  00146e42: ldr r0,[r0,#0x0]
  00146e44: movs r1,#0x0
  00146e46: mov r9,r5
  00146e48: str r5,[sp,#0x3c]
  00146e4a: blx 0x0007294c
  00146e4e: mov r0,r10
  00146e50: blx 0x000727c0
  00146e54: add r6,sp,#0x64
  00146e56: mov r1,r0
  00146e58: mov r0,r6
  00146e5a: blx 0x000727cc
  00146e5e: add r5,sp,#0xa0
  00146e60: mov r1,r6
  00146e62: mov r0,r5
  00146e64: blx 0x0006ec80
  00146e68: mov r0,r10
  00146e6a: blx r9
  00146e6c: ldr r0,[r0,#0x8]
  00146e6e: mov.w r1,#0x3f800000
  00146e72: str r1,[sp,#0x68]
  00146e74: movs r1,#0x0
  00146e76: add r2,sp,#0x64
  00146e78: str r1,[sp,#0x64]
  00146e7a: str r1,[sp,#0x6c]
  00146e7c: mov r1,r5
  00146e7e: blx r8
  00146e80: ldr r0,[0x001470d0]
  00146e82: add r0,pc
  00146e84: ldr r0,[r0,#0x0]
  00146e86: ldr r6,[r0,#0x0]
  00146e88: mov r0,r10
  00146e8a: blx r9
  00146e8c: ldr r1,[r0,#0x1c]
  00146e8e: mov r0,r6
  00146e90: blx 0x000724a8
  00146e94: ldr r5,[r7,#0x8]
  00146e96: movs r1,#0x1
  00146e98: strb.w r4,[r11,#0x21]
  00146e9c: mov r0,r5
  00146e9e: blx 0x00072670
  00146ea2: mov r0,r10
  00146ea4: blx r9
  00146ea6: add.w r9,sp,#0x130
  00146eaa: mov r1,r0
  00146eac: mov r0,r9
  00146eae: blx 0x0007264c
  00146eb2: ldr r0,[0x001470d4]
  00146eb4: add r6,sp,#0x64
  00146eb6: str r0,[sp,#0x128]
  00146eb8: add r2,sp,#0x124
  00146eba: ldr r0,[0x001470d8]
  00146ebc: mov r1,r9
  00146ebe: str r0,[sp,#0x124]
  00146ec0: ldr r0,[0x001470dc]
  00146ec2: str r0,[sp,#0x12c]
  00146ec4: mov r0,r6
  00146ec6: blx 0x0006f1cc
  00146eca: mov r0,r5
  00146ecc: mov r1,r6
  00146ece: blx 0x00076ae0
  00146ed2: mov r0,r5
  00146ed4: ldr.w r6,[r11,#0xcc]
  00146ed8: blx 0x00076b28
  00146edc: mov r1,r0
  00146ede: mov r0,r6
  00146ee0: blx 0x00072148
  00146ee4: add.w r9,sp,#0x64
  00146ee8: add r1,sp,#0xa0
  00146eea: ldr.w r6,[r11,#0xcc]
  00146eee: mov r0,r9
  00146ef0: blx 0x0006ec80
  00146ef4: mov.w r0,#0x3f800000
  00146ef8: add r2,sp,#0x130
  00146efa: str r0,[sp,#0x134]
  00146efc: movs r0,#0x0
  00146efe: str r0,[sp,#0x130]
  00146f00: mov r1,r9
  00146f02: str r0,[sp,#0x138]
  00146f04: mov r0,r6
  00146f06: blx r8
  00146f08: ldr.w r0,[r11,#0xcc]
  00146f0c: ldr r1,[0x001470e0]
  00146f0e: blx 0x000725c8
  00146f12: ldr.w r1,[r11,#0xcc]
  00146f16: mov r0,r5
  00146f18: blx 0x00076ac8
  00146f1c: ldr r0,[0x001470e4]
  00146f1e: movs r1,#0x0
  00146f20: strb.w r4,[r11,#0x11]
  00146f24: mov.w r2,#0xffffffff
  00146f28: add r0,pc
  00146f2a: mov.w r3,#0x1f40
  00146f2e: mov r9,r11
  00146f30: ldr r0,[r0,#0x0]
  00146f32: ldr r0,[r0,#0x0]
  00146f34: blx 0x00076ad4
  00146f38: mov r0,r10
  00146f3a: blx 0x00071eb4
  00146f3e: mov r6,r0
  00146f40: ldr r0,[r0,#0x4]
  00146f42: movs r1,#0x0
  00146f44: ldr r0,[r0,#0x0]
  00146f46: ldr r0,[r0,#0x4]
  00146f48: blx 0x00072eb0
  00146f4c: ldr r0,[r6,#0x4]
  00146f4e: ldr r1,[sp,#0x3c]
  00146f50: ldr r0,[r0,#0x0]
  00146f52: mov r11,r1
  00146f54: ldr.w r8,[r0,#0x4]
  00146f58: mov r0,r10
  00146f5a: blx r1
  00146f5c: ldr r1,[r0,#0x0]
  00146f5e: mov r0,r8
  00146f60: blx 0x00076b34
  00146f64: vldr.32 s16,[pc,#0x180]
  00146f68: b 0x00146ff8
  00146f6a: ldr.w r0,[r0,r4,lsl #0x2]
  00146f6e: movs r1,#0x0
  00146f70: ldr r0,[r0,#0x4]
  00146f72: blx 0x00072eb0
  00146f76: ldr r0,[r6,#0x4]
  00146f78: ldr.w r0,[r0,r4,lsl #0x2]
  00146f7c: ldr.w r8,[r0,#0x4]
  00146f80: mov r0,r10
  00146f82: blx 0x00072034
  00146f86: ldr r1,[r0,#0x0]
  00146f88: mov r0,r8
  00146f8a: blx 0x00072ebc
  00146f8e: ldr r0,[r6,#0x4]
  00146f90: ldr.w r0,[r0,r4,lsl #0x2]
  00146f94: ldr r0,[r0,#0x4]
  00146f96: ldr r0,[r0,#0x0]
  00146f98: ldr r0,[r0,#0x4]
  00146f9a: ldr r0,[r0,#0x0]
  00146f9c: ldr r0,[r0,#0x4]
  00146f9e: ldr r0,[r0,#0x0]
  00146fa0: vldr.32 s0,[r0,#0x60]
  00146fa4: vcvt.f32.s32 s0,s0
  00146fa8: vmul.f32 s0,s0,s16
  00146fac: vcvt.s32.f32 s0,s0
  00146fb0: vstr.32 s0,[r0,#0x60]
  00146fb4: ldr.w r0,[r10,#0xb0]
  00146fb8: cbz r0,0x00146ff6
  00146fba: movs r5,#0x0
  00146fbc: b 0x00146ff0
  00146fbe: ldr r0,[r0,#0x4]
  00146fc0: ldr r1,[r6,#0x4]
  00146fc2: ldr.w r0,[r0,r5,lsl #0x2]
  00146fc6: ldr.w r2,[r1,r4,lsl #0x2]
  00146fca: ldr r1,[r0,#0x4]
  00146fcc: ldr r0,[r2,#0x4]
  00146fce: blx 0x00076b34
  00146fd2: ldr.w r0,[r10,#0xb0]
  00146fd6: ldr r1,[r6,#0x4]
  00146fd8: ldr r0,[r0,#0x4]
  00146fda: ldr.w r1,[r1,r4,lsl #0x2]
  00146fde: ldr.w r0,[r0,r5,lsl #0x2]
  00146fe2: ldr r1,[r1,#0x4]
  00146fe4: ldr r0,[r0,#0x4]
  00146fe6: blx 0x00076b34
  00146fea: ldr.w r0,[r10,#0xb0]
  00146fee: adds r5,#0x1
  00146ff0: ldr r1,[r0,#0x0]
  00146ff2: cmp r5,r1
  00146ff4: bcc 0x00146fbe
  00146ff6: adds r4,#0x1
  00146ff8: ldr r0,[r6,#0x4]
  00146ffa: cmp r4,#0x4
  00146ffc: bne 0x00146f6a
  00146ffe: ldr r0,[r0,#0x0]
  00147000: movs r1,#0x0
  00147002: blx 0x000732f4
  00147006: ldr r0,[r6,#0x4]
  00147008: movs r1,#0x0
  0014700a: ldr r0,[r0,#0x0]
  0014700c: blx 0x000730b4
  00147010: mov r0,r10
  00147012: blx r11
  00147014: ldr.w r1,[r9,#0xcc]
  00147018: mov r6,r0
  0014701a: add r0,sp,#0x58
  0014701c: mov r5,r9
  0014701e: blx 0x000720b8
  00147022: add r3,sp,#0x58
  00147024: mov r0,r6
  00147026: ldmia r3,{r1,r2,r3}
  00147028: blx 0x00072cb8
  0014702c: mov r0,r10
  0014702e: blx r11
  00147030: ldr r0,[r0,#0x8]
  00147032: str r0,[sp,#0x38]
  00147034: ldr r0,[r7,#0x8]
  00147036: blx 0x00076b28
  0014703a: ldr.w r1,[r9,#0xcc]
  0014703e: add.w r9,sp,#0x118
  00147042: mov r6,r0
  00147044: mov r0,r9
  00147046: blx 0x000720b8
  0014704a: add r4,sp,#0x124
  0014704c: mov r1,r6
  0014704e: mov r2,r9
  00147050: mov r0,r4
  00147052: blx 0x0006ec38
  00147056: add r6,sp,#0x130
  00147058: ldr r0,[0x001470ec]
  0014705a: movs r1,#0x0
  0014705c: add r2,sp,#0x10c
  0014705e: strd r0,r1,[sp,#0x10c]
  00147062: mov r0,r6
  00147064: str r1,[sp,#0x114]
  00147066: mov r1,r4
  00147068: mov.w r8,#0x0
  0014706c: blx 0x0006f1cc
  00147070: add.w r9,sp,#0x64
  00147074: mov r1,r6
  00147076: mov r0,r9
  00147078: blx 0x0006ec80
  0014707c: mov.w r0,#0x3f800000
  00147080: add r2,sp,#0x100
  00147082: strd r8,r0,[sp,#0x100]
  00147086: mov r1,r9
  00147088: ldr r0,[sp,#0x38]
  0014708a: str.w r8,[sp,#0x108]
  0014708e: ldr.w r8,[r7,#0x8]
  00147092: blx 0x000726ac
  00147096: mov r0,r10
  00147098: blx r11
  0014709a: ldr r0,[r0,#0x8]
  0014709c: ldr r1,[0x001470f0]
  0014709e: blx 0x000725c8
  001470a2: mov r0,r10
  001470a4: blx r11
  001470a6: ldr r1,[0x001470f4]
  001470a8: mov r11,r5
  001470aa: blx 0x00076b40
  001470ae: movs r0,#0x1
  001470b0: str r0,[r5,#0x1c]
  001470b2: b.w 0x00146402
  001470f8: ldr r0,[0x00147494]
  001470fa: mov r4,r9
  001470fc: ldr r1,[0x00147498]
  001470fe: mov.w r6,#0x1f4
  00147102: add r0,pc
  00147104: ldr r2,[0x0014749c]
  00147106: add r1,pc
  00147108: ldr r0,[r0,#0x0]
  0014710a: add r2,pc
  0014710c: ldr r5,[r1,#0x0]
  0014710e: ldr.w r8,[r2,#0x0]
  00147112: ldrb.w r9,[r0,#0x0]
  00147116: ldr r0,[r5,#0x0]
  00147118: cmp.w r9,#0x0
  0014711c: it eq
  0014711e: mov.eq.w r6,#0x7d0
  00147122: mov r1,r6
  00147124: blx r8
  00147126: add.w r0,r0,#0x1f4
  0014712a: mov r1,r6
  0014712c: vmov s0,r0
  00147130: vcvt.f32.s32 s0,s0
  00147134: vstr.32 s0,[r11,#0x28]
  00147138: ldr r0,[r5,#0x0]
  0014713a: blx r8
  0014713c: add.w r0,r0,#0x1f4
  00147140: cmp.w r9,#0x0
  00147144: mov.w r1,#0x2
  00147148: vmov s0,r0
  0014714c: adr.w r0,0x147b34
  00147150: vcvt.f32.s32 s0,s0
  00147154: it eq
  00147156: add.eq r0,#0x4
  00147158: vldr.32 s2,[r0]
  0014715c: vstr.32 s0,[r11,#0x2c]
  00147160: vstr.32 s2,[r11,#0x30]
  00147164: ldr r0,[r5,#0x0]
  00147166: blx r8
  00147168: cbnz r0,0x00147178
  0014716a: ldr r0,[sp,#0x3c]
  0014716c: vldr.32 s0,[r0]
  00147170: vneg.f32 s0,s0
  00147174: vstr.32 s0,[r0]
  00147178: add.w r0,r11,#0x34
  0014717c: str r0,[sp,#0x34]
  0014717e: ldr r0,[r5,#0x0]
  00147180: movs r1,#0x2
  00147182: blx 0x00071848
  00147186: cmp r0,#0x0
  00147188: mov r9,r4
  0014718a: ittt eq
  0014718c: vldr.eq.32 s0,[r11,#0x2c]
  00147190: vneg.eq.f32 s0,s0
  00147194: vstr.eq.32 s0,[r11,#0x2c]
  00147198: ldr.w r0,[r9,#0x0]
  0014719c: blx 0x00071770
  001471a0: cmp r0,#0x30
  001471a2: bne 0x0014720c
  001471a4: ldr r0,[0x001474a0]
  001471a6: add r0,pc
  001471a8: ldr r5,[r0,#0x0]
  001471aa: mov r0,r10
  001471ac: blx r5
  001471ae: ldr r3,[0x001474a4]
  001471b0: movs r1,#0x0
  001471b2: movs r2,#0x0
  001471b4: mov.w r8,#0x0
  001471b8: blx 0x000725bc
  001471bc: mov r0,r10
  001471be: blx r5
  001471c0: ldr r1,[r0,#0x8]
  001471c2: add r6,sp,#0xa0
  001471c4: mov r0,r6
  001471c6: blx 0x00072574
  001471ca: vldr.32 s0,[pc,#0x2dc]
  001471ce: mov r0,r10
  001471d0: vldr.32 s2,[sp,#0xa0]
  001471d4: vadd.f32 s0,s2,s0
  001471d8: vstr.32 s0,[sp,#0xa0]
  001471dc: blx r5
  001471de: ldr r0,[r0,#0x8]
  001471e0: mov.w r1,#0x3f800000
  001471e4: add r2,sp,#0x64
  001471e6: strd r8,r1,[sp,#0x64]
  001471ea: mov r1,r6
  001471ec: str.w r8,[sp,#0x6c]
  001471f0: blx 0x000726ac
  001471f4: mov r0,r10
  001471f6: blx r5
  001471f8: mov r6,r0
  001471fa: mov r0,r10
  001471fc: blx 0x000725b0
  00147200: ldr r0,[r0,#0x4]
  00147202: ldr r1,[r0,#0x0]
  00147204: mov r0,r6
  00147206: blx 0x000728d4
  0014720a: b 0x0014764e
  0014720c: ldr.w r0,[r9,#0x0]
  00147210: blx 0x00071770
  00147214: cmp r0,#0x41
  00147216: bne 0x001472e8
  00147218: ldr.w r0,[r9,#0x0]
  0014721c: blx 0x000723d0
  00147220: cmp r0,#0x0
  00147222: bne 0x001472e8
  00147224: ldr.w r0,[r9,#0x0]
  00147228: blx 0x00071c14
  0014722c: blx 0x00071824
  00147230: mov r6,r0
  00147232: ldr.w r0,[r9,#0x0]
  00147236: blx 0x0007285c
  0014723a: blx 0x00073738
  0014723e: cmp r6,r0
  00147240: bne 0x001472e8
  00147242: ldr r0,[0x001474ac]
  00147244: add r0,pc
  00147246: ldr r5,[r0,#0x0]
  00147248: mov r0,r10
  0014724a: blx r5
  0014724c: ldr r3,[0x001474b0]
  0014724e: movs r1,#0x0
  00147250: movs r2,#0x0
  00147252: blx 0x000725bc
  00147256: mov r0,r10
  00147258: blx r5
  0014725a: ldr.w r8,[r0,#0x8]
  0014725e: mov r0,r10
  00147260: blx r5
  00147262: add r6,sp,#0x130
  00147264: mov r1,r0
  00147266: mov r0,r6
  00147268: blx 0x0007264c
  0014726c: add.w r9,sp,#0x64
  00147270: mov r1,r6
  00147272: mov r0,r9
  00147274: blx 0x0006f4cc
  00147278: add r6,sp,#0xa0
  0014727a: mov r1,r9
  0014727c: mov r9,r4
  0014727e: mov r0,r6
  00147280: blx 0x0006ec80
  00147284: mov.w r0,#0x3f800000
  00147288: add r2,sp,#0x124
  0014728a: str r0,[sp,#0x128]
  0014728c: movs r0,#0x0
  0014728e: str r0,[sp,#0x124]
  00147290: mov r1,r6
  00147292: str r0,[sp,#0x12c]
  00147294: mov r0,r8
  00147296: movs r4,#0x0
  00147298: blx 0x000726ac
  0014729c: mov r0,r10
  0014729e: blx r5
  001472a0: mov r1,r0
  001472a2: add r0,sp,#0xa0
  001472a4: blx 0x0007264c
  001472a8: vldr.32 s0,[pc,#0x208]
  001472ac: mov r0,r10
  001472ae: vldr.32 s2,[sp,#0xa0]
  001472b2: vadd.f32 s0,s2,s0
  001472b6: vstr.32 s0,[sp,#0xa0]
  001472ba: blx 0x00071eb4
  001472be: ldr r0,[r0,#0x4]
  001472c0: add r3,sp,#0xa0
  001472c2: ldr r0,[r0,#0x0]
  001472c4: ldr r6,[r0,#0x0]
  001472c6: ldmia r3,{r1,r2,r3}
  001472c8: ldr r6,[r6,#0x48]
  001472ca: blx r6
  001472cc: mov r0,r10
  001472ce: blx 0x00071eb4
  001472d2: ldr r0,[r0,#0x4]
  001472d4: ldr r1,[0x001474b8]
  001472d6: ldr r0,[r0,#0x0]
  001472d8: ldr r0,[r0,#0x8]
  001472da: strd r4,r1,[sp,#0x64]
  001472de: add r1,sp,#0x64
  001472e0: str r4,[sp,#0x6c]
  001472e2: blx 0x00073dd4
  001472e6: b 0x0014764e
  001472e8: ldr.w r0,[r9,#0x0]
  001472ec: blx 0x00071770
  001472f0: cmp r0,#0x57
  001472f2: bne 0x0014732a
  001472f4: ldr.w r0,[r9,#0x0]
  001472f8: blx 0x000723d0
  001472fc: cbnz r0,0x0014732a
  001472fe: ldr.w r0,[r9,#0x0]
  00147302: blx 0x00071c14
  00147306: blx 0x00071824
  0014730a: mov r6,r0
  0014730c: ldr.w r0,[r9,#0x0]
  00147310: blx 0x0007285c
  00147314: blx 0x00073738
  00147318: cmp r6,r0
  0014731a: bne 0x0014732a
  0014731c: ldr r0,[0x001474bc]
  0014731e: add r0,pc
  00147320: ldr r6,[r0,#0x0]
  00147322: mov r0,r10
  00147324: blx r6
  00147326: ldr r3,[0x001474c0]
  00147328: b 0x001475a8
  0014732a: ldr.w r0,[r9,#0x0]
  0014732e: blx 0x00071770
  00147332: cmp r0,#0x5b
  00147334: bne 0x0014736c
  00147336: ldr.w r0,[r9,#0x0]
  0014733a: blx 0x000723d0
  0014733e: cbnz r0,0x0014736c
  00147340: ldr.w r0,[r9,#0x0]
  00147344: blx 0x00071c14
  00147348: blx 0x00071824
  0014734c: mov r6,r0
  0014734e: ldr.w r0,[r9,#0x0]
  00147352: blx 0x0007285c
  00147356: blx 0x00073738
  0014735a: cmp r6,r0
  0014735c: bne 0x0014736c
  0014735e: ldr r0,[0x001474c4]
  00147360: add r0,pc
  00147362: ldr r6,[r0,#0x0]
  00147364: mov r0,r10
  00147366: blx r6
  00147368: ldr r3,[0x001474c8]
  0014736a: b 0x001475a8
  0014736c: ldr.w r0,[r9,#0x0]
  00147370: blx 0x00071770
  00147374: cmp r0,#0x5c
  00147376: bne 0x0014740a
  00147378: ldr.w r0,[r9,#0x0]
  0014737c: blx 0x000723d0
  00147380: cmp r0,#0x0
  00147382: bne 0x0014740a
  00147384: ldr.w r0,[r9,#0x0]
  00147388: blx 0x00071c14
  0014738c: blx 0x00071824
  00147390: mov r6,r0
  00147392: ldr.w r0,[r9,#0x0]
  00147396: blx 0x0007285c
  0014739a: blx 0x00073738
  0014739e: cmp r6,r0
  001473a0: bne 0x0014740a
  001473a2: ldr r0,[0x001474cc]
  001473a4: add r0,pc
  001473a6: ldr r6,[r0,#0x0]
  001473a8: mov r0,r10
  001473aa: blx r6
  001473ac: ldr r3,[0x001474d0]
  001473ae: movs r1,#0x0
  001473b0: movs r2,#0x0
  001473b2: movs r4,#0x0
  001473b4: blx 0x000725bc
  001473b8: mov r0,r10
  001473ba: blx r6
  001473bc: ldr r0,[r0,#0x8]
  001473be: str r0,[sp,#0x38]
  001473c0: mov r0,r10
  001473c2: blx 0x00071eb4
  001473c6: ldr r0,[r0,#0x4]
  001473c8: add r5,sp,#0x130
  001473ca: ldr r1,[r0,#0xc]
  001473cc: ldr r0,[r1,#0x0]
  001473ce: ldr r2,[r0,#0x28]
  001473d0: mov r0,r5
  001473d2: blx r2
  001473d4: mov r0,r10
  001473d6: blx r6
  001473d8: add r6,sp,#0x124
  001473da: mov r1,r0
  001473dc: mov r0,r6
  001473de: blx 0x0007264c
  001473e2: add.w r8,sp,#0x64
  001473e6: mov r1,r5
  001473e8: mov r2,r6
  001473ea: mov r0,r8
  001473ec: blx 0x0006ec38
  001473f0: add r6,sp,#0xa0
  001473f2: mov r1,r8
  001473f4: mov r0,r6
  001473f6: blx 0x0006ec80
  001473fa: mov.w r0,#0x3f800000
  001473fe: add r2,sp,#0x118
  00147400: strd r4,r0,[sp,#0x118]
  00147404: str r4,[sp,#0x120]
  00147406: ldr r0,[sp,#0x38]
  00147408: b 0x00147648
  0014740a: ldr.w r0,[r9,#0x0]
  0014740e: blx 0x00071770
  00147412: cmp r0,#0x66
  00147414: bne 0x001474e4
  00147416: ldr.w r0,[r9,#0x0]
  0014741a: blx 0x000723d0
  0014741e: cmp r0,#0x0
  00147420: bne 0x001474e4
  00147422: ldr.w r0,[r9,#0x0]
  00147426: blx 0x00071c14
  0014742a: blx 0x00071824
  0014742e: mov r6,r0
  00147430: ldr.w r0,[r9,#0x0]
  00147434: blx 0x0007285c
  00147438: blx 0x00073738
  0014743c: cmp r6,r0
  0014743e: bne 0x001474e4
  00147440: ldr r0,[0x001474d4]
  00147442: add r0,pc
  00147444: ldr r5,[r0,#0x0]
  00147446: mov r0,r10
  00147448: blx r5
  0014744a: ldr r1,[0x001474d8]
  0014744c: ldr r2,[0x001474dc]
  0014744e: ldr r3,[0x001474e0]
  00147450: blx 0x000725bc
  00147454: mov r0,r10
  00147456: blx r5
  00147458: ldr.w r8,[r0,#0x8]
  0014745c: mov r0,r10
  0014745e: blx r5
  00147460: add r6,sp,#0x130
  00147462: mov r1,r0
  00147464: mov r0,r6
  00147466: blx 0x0007264c
  0014746a: mov r5,r11
  0014746c: add.w r11,sp,#0x64
  00147470: mov r1,r6
  00147472: mov r0,r11
  00147474: blx 0x0006f4cc
  00147478: add r6,sp,#0xa0
  0014747a: mov r1,r11
  0014747c: mov r11,r5
  0014747e: mov r0,r6
  00147480: blx 0x0006ec80
  00147484: mov.w r0,#0x3f800000
  00147488: str r0,[sp,#0x128]
  0014748a: movs r0,#0x0
  0014748c: str r0,[sp,#0x124]
  0014748e: str r0,[sp,#0x12c]
  00147490: b 0x00147644
  001474e4: ldr.w r0,[r9,#0x0]
  001474e8: blx 0x00071770
  001474ec: cmp r0,#0x7b
  001474ee: bne 0x00147526
  001474f0: ldr.w r0,[r9,#0x0]
  001474f4: blx 0x000723d0
  001474f8: cbnz r0,0x00147526
  001474fa: ldr.w r0,[r9,#0x0]
  001474fe: blx 0x00071c14
  00147502: blx 0x00071824
  00147506: mov r6,r0
  00147508: ldr.w r0,[r9,#0x0]
  0014750c: blx 0x0007285c
  00147510: blx 0x00073738
  00147514: cmp r6,r0
  00147516: bne 0x00147526
  00147518: ldr r0,[0x00147800]
  0014751a: add r0,pc
  0014751c: ldr r6,[r0,#0x0]
  0014751e: mov r0,r10
  00147520: blx r6
  00147522: ldr r3,[0x00147804]
  00147524: b 0x001475a8
  00147526: ldr.w r0,[r9,#0x0]
  0014752a: blx 0x00071770
  0014752e: cmp r0,#0x54
  00147530: bgt 0x0014753e
  00147532: ldr.w r0,[r9,#0x0]
  00147536: blx 0x00071770
  0014753a: cmp r0,#0x87
  0014753c: bne 0x00147568
  0014753e: ldr.w r0,[r9,#0x0]
  00147542: blx 0x000723d0
  00147546: cbnz r0,0x00147568
  00147548: ldr.w r0,[r9,#0x0]
  0014754c: blx 0x00071c14
  00147550: blx 0x00071824
  00147554: cmp r0,#0x67
  00147556: bne 0x00147568
  00147558: ldr r0,[0x00147808]
  0014755a: add r0,pc
  0014755c: ldr r6,[r0,#0x0]
  0014755e: mov r0,r10
  00147560: blx r6
  00147562: ldr r1,[0x0014780c]
  00147564: ldr r3,[0x00147810]
  00147566: b 0x001475fa
  00147568: ldr.w r0,[r9,#0x0]
  0014756c: blx 0x00071770
  00147570: cmp r0,#0x72
  00147572: bne 0x001475b0
  00147574: ldr.w r0,[r9,#0x0]
  00147578: blx 0x000723d0
  0014757c: cbnz r0,0x001475b0
  0014757e: ldr.w r0,[r9,#0x0]
  00147582: blx 0x00071c14
  00147586: blx 0x00071824
  0014758a: mov r6,r0
  0014758c: ldr.w r0,[r9,#0x0]
  00147590: blx 0x0007285c
  00147594: blx 0x00073738
  00147598: cmp r6,r0
  0014759a: bne 0x001475b0
  0014759c: ldr r0,[0x00147814]
  0014759e: add r0,pc
  001475a0: ldr r6,[r0,#0x0]
  001475a2: mov r0,r10
  001475a4: blx r6
  001475a6: ldr r3,[0x00147818]
  001475a8: mov.w r9,#0x0
  001475ac: movs r1,#0x0
  001475ae: b 0x001475fe
  001475b0: ldr.w r0,[r9,#0x0]
  001475b4: blx 0x00071770
  001475b8: cmp r0,#0x89
  001475ba: bne.w 0x00147752
  001475be: ldr.w r0,[r9,#0x0]
  001475c2: blx 0x000723d0
  001475c6: cmp r0,#0x0
  001475c8: bne.w 0x00147752
  001475cc: ldr.w r0,[r9,#0x0]
  001475d0: blx 0x00071c14
  001475d4: blx 0x00071824
  001475d8: mov r6,r0
  001475da: ldr.w r0,[r9,#0x0]
  001475de: blx 0x0007285c
  001475e2: blx 0x00073738
  001475e6: cmp r6,r0
  001475e8: bne.w 0x00147752
  001475ec: ldr r0,[0x0014781c]
  001475ee: add r0,pc
  001475f0: ldr r6,[r0,#0x0]
  001475f2: mov r0,r10
  001475f4: blx r6
  001475f6: ldr r1,[0x00147820]
  001475f8: ldr r3,[0x00147824]
  001475fa: mov.w r9,#0x0
  001475fe: movs r2,#0x0
  00147600: blx 0x000725bc
  00147604: mov r0,r10
  00147606: blx r6
  00147608: ldr.w r8,[r0,#0x8]
  0014760c: mov r0,r10
  0014760e: blx r6
  00147610: add r6,sp,#0x130
  00147612: mov r1,r0
  00147614: mov r0,r6
  00147616: blx 0x0007264c
  0014761a: mov r5,r11
  0014761c: add.w r11,sp,#0x64
  00147620: mov r1,r6
  00147622: mov r0,r11
  00147624: blx 0x0006f4cc
  00147628: add r6,sp,#0xa0
  0014762a: mov r1,r11
  0014762c: mov r11,r5
  0014762e: mov r0,r6
  00147630: blx 0x0006ec80
  00147634: mov.w r0,#0x3f800000
  00147638: str r0,[sp,#0x128]
  0014763a: str.w r9,[sp,#0x124]
  0014763e: str.w r9,[sp,#0x12c]
  00147642: mov r9,r4
  00147644: add r2,sp,#0x124
  00147646: mov r0,r8
  00147648: mov r1,r6
  0014764a: blx 0x000726ac
  0014764e: ldr r4,[sp,#0x34]
  00147650: mov r0,r10
  00147652: blx 0x00072034
  00147656: ldr r0,[r0,#0x8]
  00147658: blx 0x000721cc
  0014765c: add.w r8,sp,#0xa0
  00147660: mov r1,r0
  00147662: movs r2,#0x3c
  00147664: mov r0,r8
  00147666: blx 0x0006f1e4
  0014766a: add r6,sp,#0x64
  0014766c: ldr r2,[sp,#0x3c]
  0014766e: mov r1,r8
  00147670: mov r0,r6
  00147672: blx 0x0006f694
  00147676: mov r0,r4
  00147678: mov r1,r6
  0014767a: blx 0x0006eb3c
  0014767e: mov r0,r10
  00147680: blx 0x00072034
  00147684: ldr r1,[r0,#0x8]
  00147686: add r6,sp,#0x64
  00147688: mov r0,r6
  0014768a: blx 0x000720b8
  0014768e: mov r0,r4
  00147690: mov r1,r6
  00147692: blx 0x00073534
  00147696: ldr.w r8,[r7,#0x8]
  0014769a: mov r1,r4
  0014769c: mov r0,r8
  0014769e: blx 0x00076ae0
  001476a2: ldr.w r0,[r9,#0x0]
  001476a6: blx 0x000723d0
  001476aa: ldr r1,[0x00147828]
  001476ac: add r1,pc
  001476ae: ldr r5,[r1,#0x0]
  001476b0: ldrb r1,[r5,#0x0]
  001476b2: cbnz r1,0x001476ba
  001476b4: cmp r0,#0x0
  001476b6: beq.w 0x00146406
  001476ba: ldr.w r0,[r9,#0x0]
  001476be: blx 0x00071770
  001476c2: cmp r0,#0x2a
  001476c4: bgt 0x0014774a
  001476c6: mov r0,r10
  001476c8: add.w r6,r11,#0x40
  001476cc: blx 0x00072034
  001476d0: add.w r8,sp,#0x64
  001476d4: mov r1,r0
  001476d6: mov r0,r8
  001476d8: blx 0x0007264c
  001476dc: ldr r4,[sp,#0x3c]
  001476de: mov r1,r8
  001476e0: mov r0,r4
  001476e2: blx 0x0006eb3c
  001476e6: mov r0,r10
  001476e8: blx 0x00072034
  001476ec: ldr r1,[r0,#0x8]
  001476ee: add.w r8,sp,#0x64
  001476f2: mov r0,r8
  001476f4: blx 0x00072574
  001476f8: mov r1,r8
  001476fa: mov r0,r6
  001476fc: ldr.w r8,[r7,#0x8]
  00147700: blx 0x0006eb3c
  00147704: ldr r1,[0x0014782c]
  00147706: mov r0,r6
  00147708: blx 0x00072628
  0014770c: mov r0,r4
  0014770e: mov r1,r6
  00147710: blx 0x00072694
  00147714: ldr r0,[0x00147830]
  00147716: add r0,pc
  00147718: ldr r4,[r0,#0x0]
  0014771a: mov r0,r10
  0014771c: blx r4
  0014771e: ldr r0,[r0,#0x4]
  00147720: movs r1,#0x1
  00147722: ldr r0,[r0,#0xc]
  00147724: blx 0x00076b4c
  00147728: mov r0,r10
  0014772a: blx r4
  0014772c: ldr r0,[r0,#0x4]
  0014772e: add.w r3,r11,#0x28
  00147732: ldr r0,[r0,#0xc]
  00147734: ldr r6,[r0,#0x0]
  00147736: ldmia r3,{r1,r2,r3}
  00147738: ldr r6,[r6,#0x48]
  0014773a: blx r6
  0014773c: mov r0,r10
  0014773e: blx r4
  00147740: ldr r0,[r0,#0x4]
  00147742: movs r1,#0x1
  00147744: ldr r0,[r0,#0xc]
  00147746: blx 0x000730b4
  0014774a: movs r0,#0x0
  0014774c: strb r0,[r5,#0x0]
  0014774e: b.w 0x00146406
  00147752: ldr.w r0,[r9,#0x0]
  00147756: str.w r11,[sp,#0x38]
  0014775a: blx 0x00071770
  0014775e: cmp r0,#0x8b
  00147760: bne 0x00147834
  00147762: ldr.w r0,[r9,#0x0]
  00147766: blx 0x000723d0
  0014776a: cmp r0,#0x0
  0014776c: bne 0x00147834
  0014776e: ldr.w r0,[r9,#0x0]
  00147772: blx 0x00071c14
  00147776: blx 0x00071824
  0014777a: mov r6,r0
  0014777c: ldr.w r0,[r9,#0x0]
  00147780: blx 0x0007285c
  00147784: blx 0x00073738
  00147788: cmp r6,r0
  0014778a: bne 0x00147834
  0014778c: ldr r0,[0x00147b2c]
  0014778e: add r0,pc
  00147790: ldr r6,[r0,#0x0]
  00147792: mov r0,r10
  00147794: blx r6
  00147796: ldr r1,[0x00147b30]
  00147798: movs r2,#0x0
  0014779a: ldr r3,[0x00147b3c]
  0014779c: movs r5,#0x0
  0014779e: blx 0x000725bc
  001477a2: mov r0,r10
  001477a4: blx r6
  001477a6: ldr r4,[r0,#0x8]
  001477a8: mov r0,r10
  001477aa: blx 0x00071eb4
  001477ae: ldr r0,[r0,#0x4]
  001477b0: add.w r11,sp,#0x130
  001477b4: ldr r1,[r0,#0x4]
  001477b6: ldr r0,[r1,#0x0]
  001477b8: ldr r2,[r0,#0x28]
  001477ba: mov r0,r11
  001477bc: blx r2
  001477be: mov r0,r10
  001477c0: blx r6
  001477c2: add r6,sp,#0x124
  001477c4: mov r1,r0
  001477c6: mov r0,r6
  001477c8: blx 0x0007264c
  001477cc: add.w r8,sp,#0x64
  001477d0: mov r1,r11
  001477d2: mov r2,r6
  001477d4: mov r0,r8
  001477d6: blx 0x0006ec38
  001477da: add r6,sp,#0xa0
  001477dc: mov r1,r8
  001477de: mov r0,r6
  001477e0: blx 0x0006ec80
  001477e4: mov.w r0,#0x3f800000
  001477e8: add r2,sp,#0x118
  001477ea: strd r5,r0,[sp,#0x118]
  001477ee: mov r0,r4
  001477f0: mov r1,r6
  001477f2: str r5,[sp,#0x120]
  001477f4: blx 0x000726ac
  001477f8: ldr.w r11,[sp,#0x38]
  001477fc: b 0x0014764e
  00147834: ldr.w r0,[r9,#0x0]
  00147838: blx 0x00071770
  0014783c: ldr r4,[sp,#0x34]
  0014783e: cmp r0,#0x8e
  00147840: bne.w 0x00147990
  00147844: ldr.w r0,[r9,#0x0]
  00147848: blx 0x000723d0
  0014784c: cmp r0,#0x0
  0014784e: bne.w 0x00147990
  00147852: ldr.w r0,[r9,#0x0]
  00147856: blx 0x00071c14
  0014785a: blx 0x00071824
  0014785e: mov r6,r0
  00147860: ldr.w r0,[r9,#0x0]
  00147864: blx 0x0007285c
  00147868: blx 0x00073738
  0014786c: cmp r6,r0
  0014786e: bne.w 0x00147990
  00147872: ldr r0,[0x00147b5c]
  00147874: add r0,pc
  00147876: ldr r5,[r0,#0x0]
  00147878: mov r0,r10
  0014787a: blx r5
  0014787c: ldr r1,[0x00147b40]
  0014787e: movs r2,#0x0
  00147880: ldr r3,[0x00147b44]
  00147882: blx 0x000725bc
  00147886: mov r0,r10
  00147888: blx r5
  0014788a: ldr r0,[r0,#0x8]
  0014788c: str r0,[sp,#0x30]
  0014788e: mov r0,r10
  00147890: blx r5
  00147892: blx 0x000768b8
  00147896: movs r1,#0x0
  00147898: blx 0x00073ec4
  0014789c: mov r1,r0
  0014789e: ldr r0,[r0,#0x0]
  001478a0: add.w r11,sp,#0x130
  001478a4: ldr r2,[r0,#0x28]
  001478a6: mov r0,r11
  001478a8: blx r2
  001478aa: mov r0,r10
  001478ac: blx r5
  001478ae: add r6,sp,#0x124
  001478b0: mov r1,r0
  001478b2: mov r0,r6
  001478b4: blx 0x0007264c
  001478b8: add.w r8,sp,#0x64
  001478bc: mov r1,r11
  001478be: mov r2,r6
  001478c0: mov r0,r8
  001478c2: blx 0x0006ec38
  001478c6: add r6,sp,#0xa0
  001478c8: mov r1,r8
  001478ca: mov r0,r6
  001478cc: blx 0x0006ec80
  001478d0: mov.w r0,#0x3f800000
  001478d4: add r2,sp,#0x118
  001478d6: str r0,[sp,#0x11c]
  001478d8: movs r0,#0x0
  001478da: str r0,[sp,#0x118]
  001478dc: mov r1,r6
  001478de: str r0,[sp,#0x120]
  001478e0: ldr r0,[sp,#0x30]
  001478e2: blx 0x000726ac
  001478e6: mov r0,r10
  001478e8: blx 0x00071eb4
  001478ec: ldr r0,[r0,#0x4]
  001478ee: ldr.w r8,[r0,#0x0]
  001478f2: mov r0,r10
  001478f4: blx r5
  001478f6: mov r1,r0
  001478f8: add r0,sp,#0x130
  001478fa: blx 0x0007264c
  001478fe: mov r0,r10
  00147900: blx r5
  00147902: ldr r1,[r0,#0x8]
  00147904: add r6,sp,#0x118
  00147906: mov r0,r6
  00147908: blx 0x0007261c
  0014790c: add.w r11,sp,#0x124
  00147910: ldr r2,[0x00147b48]
  00147912: mov r1,r6
  00147914: mov r0,r11
  00147916: blx 0x0006ec74
  0014791a: add r0,sp,#0x64
  0014791c: add r1,sp,#0x130
  0014791e: mov r2,r11
  00147920: blx 0x0006f1cc
  00147924: mov r0,r10
  00147926: blx r5
  00147928: ldr r1,[r0,#0x8]
  0014792a: add r6,sp,#0x100
  0014792c: mov r0,r6
  0014792e: blx 0x00072574
  00147932: add.w r11,sp,#0x10c
  00147936: ldr r2,[0x00147b4c]
  00147938: mov r1,r6
  0014793a: mov r0,r11
  0014793c: blx 0x0006ec74
  00147940: add r6,sp,#0xa0
  00147942: add r1,sp,#0x64
  00147944: mov r2,r11
  00147946: mov r0,r6
  00147948: blx 0x0006f1cc
  0014794c: ldr.w r0,[r8,#0x0]
  00147950: mov r1,r6
  00147952: ldr r2,[r0,#0x44]
  00147954: mov r0,r8
  00147956: blx r2
  00147958: mov r0,r10
  0014795a: blx 0x00071eb4
  0014795e: ldr r0,[r0,#0x4]
  00147960: ldr r0,[r0,#0x0]
  00147962: ldr r6,[r0,#0x8]
  00147964: mov r0,r10
  00147966: blx r5
  00147968: add.w r8,sp,#0xa0
  0014796c: mov r1,r0
  0014796e: mov r0,r8
  00147970: blx 0x00073540
  00147974: mov.w r0,#0x3f800000
  00147978: add r2,sp,#0x64
  0014797a: str r0,[sp,#0x68]
  0014797c: movs r0,#0x0
  0014797e: str r0,[sp,#0x64]
  00147980: mov r1,r8
  00147982: str r0,[sp,#0x6c]
  00147984: mov r0,r6
  00147986: blx 0x000726ac
  0014798a: ldr.w r11,[sp,#0x38]
  0014798e: b 0x00147650
  00147990: ldr.w r0,[r9,#0x0]
  00147994: blx 0x000723d0
  00147998: ldr.w r11,[sp,#0x38]
  0014799c: cmp r0,#0x0
  0014799e: beq.w 0x00147650
  001479a2: ldr.w r0,[r9,#0x0]
  001479a6: blx 0x00071770
  001479aa: cmp r0,#0x9a
  001479ac: bne.w 0x00147650
  001479b0: ldr r0,[0x00147b60]
  001479b2: add r0,pc
  001479b4: ldr.w r8,[r0,#0x0]
  001479b8: mov r0,r10
  001479ba: blx r8
  001479bc: ldr r3,[0x00147b50]
  001479be: movs r1,#0x0
  001479c0: movs r2,#0x0
  001479c2: blx 0x000725bc
  001479c6: ldr r0,[0x00147b64]
  001479c8: add r0,pc
  001479ca: ldr r1,[r0,#0x0]
  001479cc: mov r0,r10
  001479ce: str r1,[sp,#0x30]
  001479d0: blx r1
  001479d2: ldr r0,[r0,#0x4]
  001479d4: ldr r5,[r0,#0x0]
  001479d6: mov r0,r10
  001479d8: blx r8
  001479da: mov r1,r0
  001479dc: add r0,sp,#0x130
  001479de: blx 0x0007264c
  001479e2: mov r0,r10
  001479e4: blx r8
  001479e6: ldr r1,[r0,#0x8]
  001479e8: add r6,sp,#0x118
  001479ea: mov r0,r6
  001479ec: blx 0x0007261c
  001479f0: add.w r11,sp,#0x124
  001479f4: ldr r2,[0x00147b54]
  001479f6: mov r1,r6
  001479f8: mov r0,r11
  001479fa: blx 0x0006ec74
  001479fe: add r0,sp,#0x64
  00147a00: add r1,sp,#0x130
  00147a02: mov r2,r11
  00147a04: blx 0x0006f1cc
  00147a08: mov r0,r10
  00147a0a: blx r8
  00147a0c: ldr r1,[r0,#0x8]
  00147a0e: add r6,sp,#0x100
  00147a10: mov r0,r6
  00147a12: blx 0x00072574
  00147a16: add.w r11,sp,#0x10c
  00147a1a: ldr r2,[0x00147b58]
  00147a1c: mov r1,r6
  00147a1e: mov r0,r11
  00147a20: blx 0x0006ec74
  00147a24: add r6,sp,#0xa0
  00147a26: add r1,sp,#0x64
  00147a28: mov r2,r11
  00147a2a: ldr.w r11,[sp,#0x38]
  00147a2e: mov r0,r6
  00147a30: blx 0x0006f1cc
  00147a34: ldr r1,[r5,#0x0]
  00147a36: mov r0,r5
  00147a38: ldr r2,[r1,#0x44]
  00147a3a: mov r1,r6
  00147a3c: blx r2
  00147a3e: ldr r5,[sp,#0x30]
  00147a40: mov r0,r10
  00147a42: blx r5
  00147a44: ldr r0,[r0,#0x4]
  00147a46: ldr r0,[r0,#0x0]
  00147a48: ldr r6,[r0,#0x8]
  00147a4a: mov r0,r10
  00147a4c: blx r8
  00147a4e: add.w r8,sp,#0xa0
  00147a52: mov r1,r0
  00147a54: mov r0,r8
  00147a56: blx 0x00073540
  00147a5a: mov.w r0,#0x3f800000
  00147a5e: add r2,sp,#0x64
  00147a60: str r0,[sp,#0x68]
  00147a62: movs r0,#0x0
  00147a64: str r0,[sp,#0x64]
  00147a66: mov r1,r8
  00147a68: str r0,[sp,#0x6c]
  00147a6a: mov r0,r6
  00147a6c: blx 0x000726ac
  00147a70: mov r0,r10
  00147a72: blx r5
  00147a74: ldr r0,[r0,#0x4]
  00147a76: ldr r0,[r0,#0x0]
  00147a78: ldr r6,[r0,#0x4]
  00147a7a: mov r0,r10
  00147a7c: blx r5
  00147a7e: ldr r0,[r0,#0x4]
  00147a80: movs r5,#0x0
  00147a82: ldr r0,[r0,#0x4]
  00147a84: ldr r1,[r0,#0x4]
  00147a86: mov r0,r6
  00147a88: blx 0x00072ebc
  00147a8c: movs r6,#0x0
  00147a8e: b 0x00147ada
  00147a90: mov r0,r10
  00147a92: blx 0x00071eb4
  00147a96: ldr r0,[r0,#0x4]
  00147a98: ldr.w r0,[r0,r6,lsl #0x2]
  00147a9c: ldrb.w r0,[r0,#0x3e]
  00147aa0: cbz r0,0x00147ab6
  00147aa2: mov r0,r10
  00147aa4: blx 0x00071eb4
  00147aa8: ldr r0,[r0,#0x4]
  00147aaa: movs r1,#0x0
  00147aac: ldr.w r0,[r0,r6,lsl #0x2]
  00147ab0: ldr r0,[r0,#0x4]
  00147ab2: blx 0x00072eb0
  00147ab6: mov r0,r10
  00147ab8: blx 0x00071eb4
  00147abc: ldr r0,[r0,#0x4]
  00147abe: ldr.w r0,[r0,r6,lsl #0x2]
  00147ac2: ldr r0,[r0,#0x28]
  00147ac4: cmp r0,#0x9
  00147ac6: bne 0x00147ad8
  00147ac8: mov r0,r10
  00147aca: blx 0x00071eb4
  00147ace: ldr r0,[r0,#0x4]
  00147ad0: ldr.w r0,[r0,r6,lsl #0x2]
  00147ad4: strb.w r5,[r0,#0x25]
  00147ad8: adds r6,#0x1
  00147ada: mov r0,r10
  00147adc: blx 0x00071eb4
  00147ae0: ldr r0,[r0,#0x0]
  00147ae2: cmp r6,r0
  00147ae4: bcc 0x00147a90
  00147ae6: b 0x00147650
```
