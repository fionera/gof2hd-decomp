# MGame::OnUpdate
elf 0x17c8d8 body 10544
Sig: undefined __thiscall OnUpdate(MGame * this)

## decompile
```c

/* MGame::OnUpdate() */

void __thiscall MGame::OnUpdate(MGame *this)

{
  PlayerEgo **ppPVar1;
  byte bVar2;
  undefined1 uVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  uint *puVar7;
  ChoiceWindow *pCVar8;
  String *pSVar9;
  undefined4 uVar10;
  char *pcVar11;
  Engine *pEVar12;
  float *pfVar13;
  undefined1 *puVar14;
  SingleLevel *pSVar15;
  float fVar16;
  Mission *pMVar17;
  DialogueWindow *pDVar18;
  String *pSVar19;
  char cVar20;
  int extraout_r1;
  int extraout_r1_00;
  int extraout_r1_01;
  int extraout_r1_02;
  uint extraout_r1_03;
  void *extraout_r2;
  void *pvVar21;
  void *extraout_r2_00;
  void *extraout_r2_01;
  undefined4 *puVar22;
  uint uVar23;
  undefined4 *puVar24;
  char *pcVar25;
  code *pcVar26;
  Vector *pVVar27;
  TargetFollowCamera *pTVar28;
  PlayerEgo *pPVar29;
  Station *pSVar30;
  int iVar31;
  undefined1 *puVar32;
  int iVar33;
  int iVar34;
  code *pcVar35;
  int *piVar36;
  int *piVar37;
  int iVar38;
  Item *this_00;
  bool bVar39;
  uint in_fpscr;
  float fVar40;
  float fVar41;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float fVar42;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  float extraout_s2_03;
  float fVar43;
  ulonglong uVar44;
  undefined1 in_q8 [16];
  undefined1 auVar45 [16];
  undefined1 auVar46 [16];
  undefined8 uVar47;
  longlong lVar48;
  String aSStack_130 [12];
  String aSStack_124 [12];
  String aSStack_118 [12];
  String aSStack_10c [12];
  undefined1 auStack_100 [12];
  String aSStack_f4 [12];
  undefined4 local_e8;
  undefined4 uStack_e4;
  undefined4 local_e0;
  String aSStack_a8 [12];
  String aSStack_9c [12];
  String aSStack_90 [12];
  Vector aVStack_84 [12];
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  int local_3c;
  
  piVar4 = *(int **)(DAT_0018cb84 + 0x18c8ee);
  local_3c = *piVar4;
  iVar5 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
  if (iVar5 < 0x97) {
    iVar5 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
    uVar44 = in_q8._0_8_;
    if (-1 < iVar5) goto LAB_0018c90c;
    uVar6 = 0;
  }
  else {
LAB_0018c90c:
    iVar5 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
    uVar44 = in_q8._0_8_;
    if (iVar5 < 0x97) {
      uVar6 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
      uVar44 = in_q8._0_8_;
    }
    else {
      uVar6 = 0x96;
    }
  }
  cVar20 = *(char *)((int)&this[1].field_94 + 2);
  uVar44 = uVar44 & 0xffff0000ffff0000 | (ulonglong)uVar6 & 0xffff;
  auVar45._8_8_ = uVar44;
  auVar45._0_8_ = uVar44;
  auVar46._0_4_ = this->field_30;
  auVar46._4_4_ = this->field_34;
  auVar46._8_4_ = this->field_38;
  auVar46._12_4_ = this->field_3C;
  auVar46 = VectorAdd(auVar46,auVar45,8);
  this->field_40 = uVar6;
  this->field_30 = (int)auVar46._0_8_;
  this->field_34 = (int)((ulonglong)auVar46._0_8_ >> 0x20);
  this->field_38 = (int)auVar46._8_8_;
  this->field_3C = (int)((ulonglong)auVar46._8_8_ >> 0x20);
  if (cVar20 != '\0') {
    iVar5 = GetKeyState((char *)(DAT_0018cb88 + 0x18c958));
    if (iVar5 == 0) {
      if (*(char *)((int)&this[1].field_94 + 2) == '\0') goto LAB_0018ca26;
      iVar5 = GetKeyState((char *)(DAT_0018cb90 + 0x18c9d8));
      if (iVar5 != 0) {
        freeCamTouchBegin((int)this,400,(void *)0x190);
        iVar5 = 0x194;
        goto LAB_0018c974;
      }
    }
    else {
      freeCamTouchBegin((int)this,400,(void *)0x190);
      iVar5 = 0x18c;
LAB_0018c974:
      freeCamTouchMove((int)this,iVar5,(void *)0x190);
      freeCamTouchEnd((int)this,extraout_r1,extraout_r2);
    }
    if (*(char *)((int)&this[1].field_94 + 2) != '\0') {
      iVar5 = GetKeyState((char *)(DAT_0018cb8c + 0x18c998));
      if (iVar5 == 0) {
        if ((*(char *)((int)&this[1].field_94 + 2) == '\0') ||
           (iVar5 = GetKeyState((char *)(DAT_0018cb94 + 0x18ca04)), iVar5 == 0)) goto LAB_0018ca26;
        freeCamTouchBegin((int)this,400,(void *)0x190);
        pvVar21 = (void *)0x18c;
      }
      else {
        freeCamTouchBegin((int)this,400,(void *)0x190);
        pvVar21 = (void *)0x194;
      }
      freeCamTouchMove((int)this,400,pvVar21);
      freeCamTouchEnd((int)this,extraout_r1_00,extraout_r2_00);
    }
  }
LAB_0018ca26:
  piVar36 = *(int **)(DAT_0018cb98 + 0x18ca2e);
  piVar37 = *(int **)(DAT_0018cb9c + 0x18ca30);
  iVar5 = *piVar36;
  iVar34 = *piVar37;
  if ((((this->field_14 == 3) && (**(int **)(DAT_0018cba0 + 0x18ca4c) != 0)) &&
      (**(int **)(DAT_0018cba4 + 0x18ca56) == 0)) && (iVar34 != 0 || iVar5 != 0)) {
    iVar31 = this[2].field_18;
    iVar38 = this[2].field_1C;
    freeCamTouchBegin((int)this,400,(void *)0x190);
    freeCamTouchMove((int)this,(iVar5 - iVar31) / 2 + 400,(void *)((iVar34 - iVar38) / 2 + 400));
    freeCamTouchEnd((int)this,extraout_r1_01,extraout_r2_01);
    iVar34 = 0;
    iVar5 = 0;
    *piVar37 = 0;
    *piVar36 = 0;
  }
  ppPVar1 = &this->m_pPlayerEgo;
  this[2].field_18 = iVar5;
  this[2].field_1C = iVar34;
  iVar5 = PlayerEgo::isDockingToAsteroid();
  if (iVar5 != 0) {
    *piVar37 = 0;
    *piVar36 = 0;
  }
  pcVar25 = *(char **)(DAT_0018cba8 + 0x18cad0);
  if (*pcVar25 != '\0') {
    iVar5 = PlayerEgo::isMining();
    if ((iVar5 == 0) && (iVar5 = Status::getWingmen(), iVar5 != 0)) {
      if (*(char *)((int)&this[1].field_0C + 2) == '\0') {
        bVar2 = *(byte *)((int)&this->field_5C + 1);
        *(undefined1 *)((int)&this[1].field_0C + 2) = 1;
        *(byte *)((int)&this->field_5C + 1) = bVar2 ^ 1;
        pauseSounds();
        Hud::initHudMenu((int)this->m_pLevel,(Level *)0x2);
      }
      else {
        *(undefined1 *)((int)&this[1].field_0C + 2) = 0;
        Hud::closeHudMenu();
        *(undefined1 *)((int)&this->field_5C + 1) = 0;
        resumeSounds();
      }
    }
    *pcVar25 = '\0';
  }
  cVar20 = *(char *)((int)&this->field_5C + 1);
  if (cVar20 != *(char *)((int)&this[2].field_14 + 2)) {
    if (cVar20 == '\0') {
      PlayerEgo::ResumeEngineSound();
      puVar7 = (uint *)Level::getEnemies();
      if (puVar7 != (uint *)0x0) {
        for (uVar6 = 0; uVar6 < *puVar7; uVar6 = uVar6 + 1) {
          KIPlayer::ResumeEngineSound(*(KIPlayer **)(puVar7[1] + uVar6 * 4));
        }
      }
    }
    else {
      PlayerEgo::PauseEngineSound();
      puVar7 = (uint *)Level::getEnemies();
      if (puVar7 != (uint *)0x0) {
        for (uVar6 = 0; uVar6 < *puVar7; uVar6 = uVar6 + 1) {
          KIPlayer::PauseEngineSound(*(KIPlayer **)(puVar7[1] + uVar6 * 4));
        }
      }
    }
    cVar20 = *(char *)((int)&this->field_5C + 1);
    *(char *)((int)&this[2].field_14 + 2) = cVar20;
  }
  if (((cVar20 == '\0') ||
      (((*(char *)((int)&this[1].field_94 + 2) != '\0' &&
        (iVar5 = MenuTouchWindow::isShowingMessage(), iVar5 == 0)) &&
       (iVar5 = MenuTouchWindow::isMakingScreenshot(), iVar5 == 0)))) && (this->field_14 == 3)) {
    if (*(char *)((int)&this[1].field_94 + 2) != '\0') {
      Level::update((ulonglong)CONCAT14(*(undefined1 *)((int)&this->field_5C + 3),this->field_78),
                    false);
    }
    iVar5 = AbyssEngine::ApplicationManager::GetEngine();
    fVar41 = DAT_0018cbb8;
    fVar42 = *(float *)(iVar5 + 0x360);
    uVar6 = in_fpscr & 0xfffffff;
    fVar40 = -fVar42;
    if (0.0 < fVar42) {
      fVar40 = fVar42;
    }
    uVar23 = uVar6 | (uint)(fVar40 < DAT_0018cbb0) << 0x1f | (uint)(fVar40 == DAT_0018cbb0) << 0x1e;
    in_fpscr = uVar23 | (uint)(NAN(fVar40) || NAN(DAT_0018cbb0)) << 0x1c;
    bVar2 = (byte)(uVar23 >> 0x18);
    if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
      fVar40 = (float)this->field_BC + fVar42 * DAT_0018cbb4;
      uVar23 = uVar6 | (uint)(fVar40 < DAT_0018cbb8) << 0x1f |
               (uint)(fVar40 == DAT_0018cbb8) << 0x1e;
      in_fpscr = uVar23 | (uint)(NAN(fVar40) || NAN(DAT_0018cbb8)) << 0x1c;
      this->field_BC = (int)fVar40;
      bVar2 = (byte)(uVar23 >> 0x18);
      if ((!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) ||
         (in_fpscr = uVar6, fVar41 = DAT_0018cbbc, fVar40 < DAT_0018cbbc)) {
        fVar40 = fVar41;
        this->field_BC = (int)fVar40;
      }
      TargetFollowCamera::zoomTarget(fVar40);
    }
    iVar5 = AbyssEngine::ApplicationManager::GetEngine();
    *(float *)(iVar5 + 0x360) = *(float *)(iVar5 + 0x360) * DAT_0018cbc0;
  }
  if (*(char *)((int)&this->field_5C + 1) == '\0') {
    iVar34 = this[1].field_A4;
    iVar5 = 0;
    if (iVar34 != 0) {
      iVar31 = this->field_40;
      if (iVar34 < 0) {
        this[1].field_A4 = iVar34 - iVar31;
        iVar5 = -this[1].field_A8;
        if (iVar34 - iVar31 < iVar5) {
          uVar3 = SUB41(this->m_pLevel,0);
          this[1].field_A4 = 0;
          bVar39 = true;
LAB_0018cd52:
          Hud::setTimeExtender((bool)uVar3,true,bVar39,true);
          iVar5 = extraout_r1_02;
        }
      }
      else {
        if (iVar34 <= iVar31) {
          piVar36 = *(int **)(DAT_0018cd60 + 0x18ccca);
          FModSound::setDownPitch(SUB41(*piVar36,0));
          fVar41 = (float)Hud::setTimeExtender(SUB41(this->m_pLevel,0),true,false,false);
          FModSound::play(*piVar36,(Vector *)0x45f,(Vector *)0x0,fVar41);
          iVar31 = this->field_40;
          iVar34 = this[1].field_A4;
        }
        iVar5 = iVar34 - iVar31;
        this[1].field_A4 = iVar5;
        if (iVar5 == 0) {
          this[1].field_A4 = -1;
        }
        else if (0 < iVar5) {
          fVar41 = (float)VectorSignedToFloat(iVar31,(byte)(in_fpscr >> 0x16) & 3);
          bVar39 = false;
          fVar40 = fVar41 * DAT_0018cd68;
          uVar3 = SUB41(this->m_pLevel,0);
          this->field_40 = (int)(fVar41 * DAT_0018cd64);
          this->field_44 = (int)fVar40;
          goto LAB_0018cd52;
        }
      }
    }
    piVar36 = *(int **)(DAT_0018d120 + 0x18cd7e);
    Status::incPlayingTime(CONCAT44(iVar5,*piVar36));
    if ((((char)this->field_80[0x15] == '\0') &&
        (iVar5 = PlayerEgo::aboutToReachAutoTarget(*ppPVar1), iVar5 == 0)) &&
       (iVar5 = Radio::isShowingMessage(), iVar5 == 0)) {
      fVar41 = (float)VectorSignedToFloat(this->field_40,(byte)(in_fpscr >> 0x16) & 3);
      fVar41 = (float)this[1].field_A0 * fVar41;
      uVar6 = (uint)fVar41;
      this->field_40 = (int)fVar41;
    }
    else {
      this[1].field_A0 = 0x3f800000;
      TargetFollowCamera::setFastForwardMode(SUB41(this[1].field_2C,0));
      *(undefined1 *)&this->m_pPlayerEgo[3].field_18 = 1;
      uVar6 = this->field_40;
    }
    piVar37 = this->field_7C;
    uVar23 = piVar37[2];
    iVar5 = piVar37[3] + ((int)uVar6 >> 0x1f) + (uint)CARRY4(uVar23,uVar6);
    piVar37[2] = uVar23 + uVar6;
    piVar37[3] = iVar5;
    if ((((int)uVar23 < 0x1389) && ((int)(uint)(uVar23 + uVar6 < 0x1389) <= iVar5)) &&
       (iVar5 = PlayerEgo::hasVolatileGoods(), iVar5 != 0)) {
      Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0x2d,(int)this->m_pPlayerEgo);
    }
    if (*(char *)((int)&this->field_5C + 3) == '\0') {
      uVar6 = this->field_40;
      uVar23 = this->field_48;
      this->field_48 = uVar23 + uVar6;
      this->field_4C = this->field_4C + ((int)uVar6 >> 0x1f) + (uint)CARRY4(uVar23,uVar6);
      if (((this->field_14 != 3) && (iVar5 = PlayerEgo::isDockingToAsteroid(), iVar5 == 0)) &&
         (iVar5 = PlayerEgo::isDockedToAsteroid(), iVar5 == 0)) {
        iVar5 = AbyssEngine::ApplicationManager::GetEngine();
        fVar41 = *(float *)(iVar5 + 0x360);
        uVar6 = in_fpscr & 0xfffffff;
        uVar23 = uVar6 | (uint)(fVar41 < 1.0) << 0x1f;
        in_fpscr = uVar23 | (uint)NAN(fVar41) << 0x1c;
        if (((byte)(uVar23 >> 0x1f) == ((byte)(in_fpscr >> 0x1c) & 1)) ||
           (in_fpscr = uVar6 | (uint)(fVar41 == -1.0) << 0x1e | (uint)(-1.0 <= fVar41) << 0x1d,
           bVar2 = (byte)(in_fpscr >> 0x18), !(bool)(bVar2 >> 5 & 1) || (bool)(bVar2 >> 6))) {
          this[2].field_24 = (int)(fVar41 + (float)this[2].field_24);
          iVar5 = AbyssEngine::ApplicationManager::GetEngine();
          fVar41 = DAT_0018d124;
          *(undefined4 *)(iVar5 + 0x360) = 0;
          fVar42 = (float)this[2].field_24;
          in_fpscr = in_fpscr & 0xfffffff;
          fVar40 = fVar41;
          if (fVar42 < fVar41) {
            fVar40 = fVar42;
          }
          fVar16 = fVar41;
          if (fVar40 < 0.0) {
            fVar16 = DAT_0018d128;
          }
          fVar43 = fVar16;
          if (fVar42 < fVar41) {
            fVar43 = fVar42;
          }
          if (fVar40 < 0.0) {
            fVar43 = fVar16;
          }
          this[2].field_24 = (int)fVar43;
          PlayerEgo::setThrust(fVar43 / fVar41);
          this[2].field_20 = (int)(1.0 - (float)this[2].field_24 / fVar41);
          PlayerEgo::throttleChanged();
        }
      }
      iVar5 = PlayerEgo::isDockingToAsteroid();
      if ((iVar5 != 0) || (iVar5 = PlayerEgo::isDockedToAsteroid(), iVar5 != 0)) {
        fVar41 = (float)PlayerEgo::getThrust();
        in_fpscr = in_fpscr & 0xfffffff;
        if (fVar41 < 1.0) {
          this[2].field_24 = (int)DAT_0018d124;
          PlayerEgo::setThrust(1.0);
          iVar5 = AbyssEngine::ApplicationManager::GetEngine();
          *(undefined4 *)(iVar5 + 0x360) = 0;
        }
      }
      iVar5 = PlayerEgo::isDockedToAsteroid();
      if ((iVar5 != 0) && (this->field_14 != 0)) {
        switchCamera((int)this);
      }
    }
    Layout::update(**(int **)(DAT_0018d12c + 0x18cf76));
    iVar5 = *(int *)(DAT_0018d130 + 0x18cf86);
    *(int *)(*piVar36 + 0x30) = *(int *)(*piVar36 + 0x30) - this->field_40;
    if (*(char *)(iVar5 + 0xf) == '\0') {
      FModSound::updateAll
                ((Vector *)**(undefined4 **)(DAT_0018d134 + 0x18cfa0),(Vector *)0x0,(Vector *)0x0,
                 (Vector *)0x0);
    }
    iVar5 = PlayerEgo::isDockedToMiningPlant();
    if (this->field_50 < 4000) {
      iVar34 = 0x40;
      if (0 < this[1].field_A4) {
        iVar34 = 0x44;
      }
      PlayerEgo::update((int)this->m_pPlayerEgo,*(Radar **)((int)&this->field_0 + iVar34),
                        (Hud *)this->field_80,(Radio *)this->m_pLevel,(LevelScript *)this->field_84,
                        (int)this->field_7C,SUB41(this[1].field_10,0),
                        (uint)*(byte *)((int)&this->field_5C + 3));
    }
    iVar34 = PlayerEgo::isInRocketControl();
    Level::update((ulonglong)CONCAT14(*(undefined1 *)((int)&this->field_5C + 3),this->field_78),
                  SUB41(this->field_40,0));
    if ((iVar34 != 0) && (iVar34 = PlayerEgo::isInRocketControl(), iVar34 == 0)) {
      switchCamera((int)this);
    }
    if (((iVar5 == 0) && (iVar5 = PlayerEgo::isDockedToMiningPlant(), iVar5 != 0)) &&
       (*(char *)((int)&this[1].field_4 + 2) == '\0')) {
      if (this->field_94 == 0) {
        pCVar8 = operator_new(0x5c);
        ChoiceWindow::ChoiceWindow(pCVar8);
        this->field_94 = (int)pCVar8;
      }
      piVar37 = *(int **)(DAT_0018d138 + 0x18d06a);
      pSVar9 = (String *)GameText::getText(*piVar37);
      AbyssEngine::String::String(aSStack_48,(char *)(DAT_0018d13c + 0x18d07e),false);
      AbyssEngine::operator+((String *)&local_e8,pSVar9);
      AbyssEngine::String::~String(aSStack_48);
      iVar5 = Status::hardCoreMode();
      iVar31 = *piVar36;
      AbyssEngine::String::String(aSStack_54,(String *)&local_e8,false);
      iVar34 = 0x1e;
      if (iVar5 != 0) {
        iVar34 = 100;
      }
      AbyssEngine::String::String(aSStack_60,iVar34);
      uVar10 = AbyssEngine::String::String(aSStack_6c,(char *)(DAT_0018d140 + 0x18d0be),false);
      Status::replaceHash(aSStack_48,iVar31,aSStack_54,aSStack_60,uVar10);
      AbyssEngine::String::operator=((String *)&local_e8,aSStack_48);
      pcVar26 = *(code **)(DAT_0018d144 + 0x18d0e4);
      (*pcVar26)(aSStack_48);
      (*pcVar26)(aSStack_6c);
      (*pcVar26)(aSStack_60);
      (*pcVar26)(aSStack_54);
      Status::getShip();
      puVar7 = (uint *)Ship::getCargo();
      if (puVar7 == (uint *)0x0) {
LAB_0018d1f6:
        AbyssEngine::String::String(aSStack_78,(char *)(DAT_0018d5a8 + 0x18d1fc),false);
        GameText::getText(*piVar37);
        AbyssEngine::operator+(aSStack_48,aSStack_78);
        AbyssEngine::String::operator+=((String *)&local_e8,aSStack_48);
        AbyssEngine::String::~String(aSStack_48);
        AbyssEngine::String::~String(aSStack_78);
      }
      else {
        bVar39 = false;
        pcVar25 = (char *)(DAT_0018d148 + 0x18d118);
        pcVar11 = (char *)(DAT_0018d14c + 0x18d11e);
        for (uVar6 = 0; uVar6 < *puVar7; uVar6 = uVar6 + 1) {
          this_00 = *(Item **)(puVar7[1] + uVar6 * 4);
          if ((this_00 != (Item *)0x0) && (iVar5 = Item::getSort(this_00), iVar5 == 0x17)) {
            AbyssEngine::String::String(aSStack_90,pcVar25,false);
            iVar5 = Item::getAmount();
            AbyssEngine::String::String(aSStack_9c,iVar5);
            AbyssEngine::operator+(aVStack_84,aSStack_90);
            AbyssEngine::String::String(aSStack_a8,pcVar11,false);
            AbyssEngine::operator+(aSStack_78,aVStack_84);
            iVar5 = *piVar37;
            Item::getIndex(this_00);
            GameText::getText(iVar5);
            AbyssEngine::operator+(aSStack_48,aSStack_78);
            AbyssEngine::String::operator+=((String *)&local_e8,aSStack_48);
            (*pcVar26)(aSStack_48);
            (*pcVar26)(aSStack_78);
            (*pcVar26)(aSStack_a8);
            (*pcVar26)(aVStack_84);
            (*pcVar26)(aSStack_9c);
            (*pcVar26)(aSStack_90);
            bVar39 = true;
          }
        }
        if (!bVar39) goto LAB_0018d1f6;
      }
      ChoiceWindow::set((ChoiceWindow *)this->field_94,(String *)&local_e8,true);
      *(undefined1 *)((int)&this->field_5C + 1) = 1;
      *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
      pauseSounds();
      *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
      *(undefined1 *)((int)&this[1].field_0 + 2) = 1;
      AbyssEngine::String::~String((String *)&local_e8);
    }
  }
  iVar5 = *(int *)(DAT_0018d5ac + 0x18d262);
  if (*(char *)(iVar5 + 0xf) != '\0') {
    piVar36 = this->field_4;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar10 = AbyssEngine::PaintCanvas::CameraGetLocal((uint)piVar36);
    __aeabi_memcpy(&local_e8,uVar10,0x3c);
    pcVar26 = *(code **)(DAT_0018d5b4 + 0x18d296);
    VectorSignedToFloat(**(undefined4 **)(DAT_0018d5b0 + 0x18d294),(byte)(in_fpscr >> 0x16) & 3);
    (*pcVar26)(**(undefined4 **)(DAT_0018d5b0 + 0x18d294),(Vector *)aSStack_90,&local_e8);
    fVar41 = (float)AbyssEngine::AEMath::operator-(aVStack_84,(Vector *)aSStack_90);
    AbyssEngine::AEMath::operator*(fVar41,(Vector *)aSStack_78);
    fVar41 = (float)VectorSignedToFloat(this->field_40,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::AEMath::operator/((Vector *)aSStack_48,fVar41);
    AbyssEngine::AEMath::MatrixGetDir((Matrix *)aVStack_84);
    AbyssEngine::AEMath::operator-((AEMath *)aSStack_78,aVStack_84);
    (*pcVar26)(aVStack_84,&local_e8);
    AbyssEngine::AEMath::MatrixGetUp((Matrix *)aSStack_90);
    FModSound::updateAll
              ((Vector *)**(undefined4 **)(DAT_0018d5b8 + 0x18d30a),aVStack_84,(Vector *)aSStack_78,
               (Vector *)aSStack_90);
    (*pcVar26)((Vector *)aSStack_9c,&local_e8);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&this[2].field_8,(Vector *)aSStack_9c);
  }
  if ((char)this->field_60 == '\0') {
    uVar47 = PlayerEgo::getHitpoints();
    uVar6 = (uint)((ulonglong)uVar47 >> 0x20);
    if (0 < (int)uVar47) {
      if (*(char *)((int)&this->field_5C + 1) != '\0') {
        if (((this->m_pDialogueWindow != (DialogueWindow *)0x0) &&
            (uVar6 = (uint)*(byte *)((int)&this[1].field_0C + 2), uVar6 == 0)) &&
           ((uVar6 = (uint)*(byte *)((int)&this->field_5C + 2), uVar6 == 0 &&
            ((((uVar6 = (uint)*(byte *)&this[1].field_0, uVar6 == 0 &&
               (uVar6 = (uint)(byte)this->field_0xc5, uVar6 == 0)) &&
              (uVar6 = (uint)(byte)this->field_0xc6, uVar6 == 0)) &&
             ((uVar6 = (uint)*(byte *)((int)&this[1].field_4 + 2), uVar6 == 0 &&
              (uVar6 = (uint)*(byte *)((int)&this[1].field_0 + 2), uVar6 == 0)))))))) {
          fVar41 = (float)MenuTouchWindow::update
                                    ((MenuTouchWindow *)this->m_pDialogueWindow,this->field_40);
          if (*(char *)((int)&this[1].field_94 + 2) != '\0') {
            TargetFollowCamera::setRumblePercentage(fVar41,this[1].field_2C);
            iVar5 = this[1].field_94;
            this->field_A0 = this->field_40 + this->field_A0;
            if ((char)iVar5 == '\0') {
              piVar37 = (int *)((float)this[1].m_pLevel * (float)this[1].field_80);
              piVar36 = (int *)-((float)this[1].m_pLevel * (float)this[1].field_80);
              if (0.0 < (float)piVar37) {
                piVar36 = piVar37;
              }
              this[1].field_80 = piVar37;
              if (1.0 < (float)piVar36) {
                this[1].field_50 = (int)((float)piVar37 + (float)this[1].field_50);
              }
              piVar37 = (int *)((float)this[1].field_78 * (float)this[1].field_84);
              piVar36 = (int *)-((float)this[1].field_78 * (float)this[1].field_84);
              if (0.0 < (float)piVar37) {
                piVar36 = piVar37;
              }
              this[1].field_84 = piVar37;
              fVar41 = DAT_0018d5e0;
              if (1.0 < (float)piVar36) {
                fVar40 = (float)piVar37 + (float)this[1].field_54;
                this[1].field_54 = (int)fVar40;
                if (fVar40 <= fVar41) {
                  if ((int)((uint)(fVar40 < DAT_0018f304) << 0x1f) < 0) {
                    this[1].field_54 = (int)DAT_0018f304;
                  }
                }
                else {
                  this[1].field_54 = (int)DAT_0018d5e0;
                }
              }
            }
            TargetFollowCamera::rotateAroundTarget
                      ((TargetFollowCamera *)this[1].field_2C,(float)this[1].field_50 * DAT_0018f308
                       ,extraout_s1_01,(float)this[1].field_50);
            TargetFollowCamera::update(this[1].field_2C);
          }
          uVar47 = MenuTouchWindow::isInMissionScreen();
          uVar6 = (uint)((ulonglong)uVar47 >> 0x20);
          if ((int)uVar47 != 0) {
            Status::incPlayingTime(CONCAT44(uVar6,**(undefined4 **)(DAT_0018f30c + 0x18f0e2)));
            uVar6 = extraout_r1_03;
          }
        }
        if ((((char)this->field_54 != '\0') && (*(char *)&this[1].field_0 == '\0')) &&
           ((this->field_0xc5 == '\0' && (this->field_0xc6 == '\0')))) {
          if (*(char *)((int)&this[1].field_4 + 2) == '\0') {
            if (*(char *)((int)&this[1].field_0C + 2) == '\0') {
              if (this->field_0xc7 == '\0') {
                if (*(char *)((int)&this->field_5C + 2) != '\0') {
                  Layout::update(**(int **)(DAT_0018f314 + 0x18f152));
                  DialogueWindow::update(this->field_8C);
                }
              }
              else {
                Status::incPlayingTime(CONCAT44(uVar6,**(undefined4 **)(DAT_0018f310 + 0x18f12c)));
                StarMap::update((StarMap *)this->field_90,this->field_40);
              }
            }
          }
          else {
            ChoiceWindow::update(this->field_94);
          }
        }
        goto LAB_0018f164;
      }
      iVar34 = PlayerEgo::isDockingToPlanet();
      if ((iVar34 != 0) && (iVar34 = Status::getCurrentCampaignMission(), iVar34 == 0x18)) {
        iVar34 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar31 = Mission::getTargetStation();
        if (iVar34 == iVar31) {
          iVar34 = Status::getShip();
          iVar34 = Ship::getFirstEquipmentOfSort(iVar34);
          if (iVar34 != 0) {
            iVar34 = Status::getShip();
            iVar34 = Ship::getFirstEquipmentOfSort(iVar34);
            if (iVar34 != 0) goto LAB_0018d828;
          }
          *(undefined1 *)((int)&this->field_5C + 1) = 1;
          pauseSounds();
          if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
            pvVar21 = (void *)DialogueWindow::~DialogueWindow((DialogueWindow *)this->field_8C);
            operator_delete(pvVar21);
            this->field_8C = 0;
          }
          pDVar18 = operator_new(0x74);
          piVar36 = *(int **)(DAT_0018e05c + 0x18dd6e);
          pSVar9 = (String *)GameText::getText(*piVar36);
          pSVar19 = (String *)GameText::getText(*piVar36);
          DialogueWindow::DialogueWindow(pDVar18,pSVar9,pSVar19,(int *)(DAT_0018e060 + 0x18ddb4));
          *(undefined1 *)((int)&this->field_5C + 2) = 1;
          this->field_8C = (int)pDVar18;
          PlayerEgo::stopPlanetDock();
          PlayerEgo::setAutoPilot((KIPlayer *)this->m_pPlayerEgo);
          goto LAB_0018f164;
        }
      }
LAB_0018d828:
      iVar34 = PlayerEgo::isDockedToPlanet();
      if (iVar34 != 0) {
        piVar36 = *(int **)(DAT_0018d974 + 0x18d83e);
        pSVar30 = (Station *)*piVar36;
        iVar5 = **(int **)(DAT_0018d978 + 0x18d840);
        Radar::getPlanetDockIndex();
        Galaxy::getStation(iVar5);
        Status::departStation(pSVar30);
        Level::setInitStreamOut();
        uVar10 = Player::getHitpoints();
        *(undefined4 *)(*piVar36 + 100) = uVar10;
        uVar10 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
        *(undefined4 *)(*piVar36 + 0x5c) = uVar10;
        uVar10 = Player::getArmorHP();
        *(undefined4 *)(*piVar36 + 0x60) = uVar10;
        uVar10 = Player::getGammaHP();
        *(undefined4 *)(*piVar36 + 0x68) = uVar10;
        uVar10 = PlayerEgo::getCurrentSecondaryWeaponIndex();
        puVar22 = *(undefined4 **)(DAT_0018d97c + 0x18d8aa);
        *(undefined4 *)(*piVar36 + 0xf4) = uVar10;
        *puVar22 = 1;
        *(undefined1 *)&this->field_54 = 0;
        AbyssEngine::ApplicationManager::SetCurrentApplicationModule((uint)this->field_8);
        goto LAB_0018f164;
      }
      iVar34 = PlayerEgo::shouldSwitchToStandardCam(*ppPVar1);
      if (iVar34 != 0) {
        switchCamera((int)this);
      }
      iVar34 = PlayerEgo::shouldSwitchToFreeLookCam();
      if (iVar34 != 0) {
        switchCamera((int)this);
        iVar34 = DAT_0018dd10;
        pTVar28 = (TargetFollowCamera *)this[1].field_2C;
        this[1].field_50 = DAT_0018dd1c;
        this[1].field_54 = iVar34;
        this->field_BC = DAT_0018dd20;
        fVar41 = (float)TargetFollowCamera::rotateAroundTarget
                                  (pTVar28,extraout_s0,extraout_s1_06,extraout_s2_03);
        TargetFollowCamera::zoomTarget(fVar41);
      }
      if ((char)this[1].field_14 == '\0') {
        iVar34 = PlayerEgo::isChargingDrive();
        if ((iVar34 != 0) && (iVar34 = PlayerEgo::driveReady(*ppPVar1), iVar34 != 0)) {
          startJumpScene(this);
          goto LAB_0018f164;
        }
      }
      else {
        iVar34 = updateJumpScene();
        if (iVar34 != 0) goto LAB_0018f164;
      }
      uVar47 = LevelScript::startSequenceOver();
      iVar34 = (int)((ulonglong)uVar47 >> 0x20);
      if ((int)uVar47 == 0) {
        uVar47 = LevelScript::startSequence((LevelScript *)this->field_7C);
        iVar34 = (int)((ulonglong)uVar47 >> 0x20);
        if ((int)uVar47 == 0) goto LAB_0018dbfa;
      }
      else {
LAB_0018dbfa:
        if (((this->field_0xc5 == '\0') && (this->field_0xc6 == '\0')) &&
           ((iVar34 = dockEvent((int)this,iVar34), iVar34 != 0 || (this->field_0xc7 != '\0'))))
        goto LAB_0018f164;
      }
      if ((int)(uint)((uint)this->field_48 < 0x1389) <= this->field_4C) {
        if ((((int)(uint)((uint)this->field_7C[2] < 0x1389) <= this->field_7C[3]) &&
            ((char)this[1].field_14 == '\0')) && (*(char *)((int)&this->field_5C + 2) == '\0')) {
          piVar36 = *(int **)(DAT_0018e3f4 + 0x18e08e);
          iVar34 = Status::getCurrentCampaignMission();
          if ((iVar34 == 0x17) && (iVar34 = PlayerEgo::isMining(), iVar34 == 0)) {
            Status::getMission();
            iVar34 = Mission::isEmpty();
            if (iVar34 == 0) goto LAB_0018e176;
            lVar48 = Status::getPlayingTime();
            lVar48 = lVar48 - *(longlong *)(*piVar36 + 0x100);
            iVar34 = (int)((ulonglong)lVar48 >> 0x20);
            bVar39 = DAT_0018e3f8 < (uint)lVar48;
            if ((int)(-(uint)bVar39 - iVar34) < 0 ==
                (SBORROW4(0,iVar34) != SBORROW4(-iVar34,(uint)bVar39))) goto LAB_0018e176;
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
              pvVar21 = (void *)DialogueWindow::~DialogueWindow((DialogueWindow *)this->field_8C);
              operator_delete(pvVar21);
              this->field_8C = 0;
            }
            iVar34 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0018e3fc + 0x18e108));
            pDVar18 = operator_new(0x74);
            piVar37 = *(int **)(DAT_0018e400 + 0x18e12c);
            pSVar9 = (String *)GameText::getText(*piVar37);
            pSVar19 = (String *)GameText::getText(*piVar37);
            fVar41 = (float)DialogueWindow::DialogueWindow
                                      (pDVar18,pSVar9,pSVar19,(int *)(DAT_0018e404 + 0x18e15c));
            piVar37 = *(int **)(DAT_0018e408 + 0x18e15e);
            this->field_8C = (int)pDVar18;
            pVVar27 = (Vector *)0x1d1;
            iVar31 = *piVar37;
            if (iVar34 == 0) {
              pVVar27 = (Vector *)0x1d0;
            }
LAB_0018e25a:
            FModSound::play(iVar31,pVVar27,(Vector *)0x0,fVar41);
            *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
            *(undefined1 *)((int)&this->field_5C + 2) = 1;
LAB_0018e26c:
            uVar47 = Status::getPlayingTime();
            *(undefined8 *)(*piVar36 + 0x100) = uVar47;
          }
          else {
LAB_0018e176:
            iVar34 = Status::getCurrentCampaignMission();
            if ((iVar34 == 0x18) && (iVar34 = PlayerEgo::isMining(), iVar34 == 0)) {
              Status::getMission();
              iVar34 = Mission::isEmpty();
              if (iVar34 != 0) {
                lVar48 = Status::getPlayingTime();
                lVar48 = lVar48 - *(longlong *)(*piVar36 + 0x100);
                iVar34 = (int)((ulonglong)lVar48 >> 0x20);
                bVar39 = DAT_0018e3f8 < (uint)lVar48;
                if ((int)(-(uint)bVar39 - iVar34) < 0 !=
                    (SBORROW4(0,iVar34) != SBORROW4(-iVar34,(uint)bVar39))) {
                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                  pauseSounds();
                  if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
                    pvVar21 = (void *)DialogueWindow::~DialogueWindow
                                                ((DialogueWindow *)this->field_8C);
                    operator_delete(pvVar21);
                    this->field_8C = 0;
                  }
                  iVar34 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0018e40c + 0x18e1ee));
                  pDVar18 = operator_new(0x74);
                  piVar37 = *(int **)(DAT_0018e410 + 0x18e212);
                  pSVar9 = (String *)GameText::getText(*piVar37);
                  pSVar19 = (String *)GameText::getText(*piVar37);
                  fVar41 = (float)DialogueWindow::DialogueWindow
                                            (pDVar18,pSVar9,pSVar19,(int *)(DAT_0018e414 + 0x18e256)
                                            );
                  piVar37 = *(int **)(DAT_0018e418 + 0x18e244);
                  this->field_8C = (int)pDVar18;
                  pVVar27 = (Vector *)0x1d4;
                  iVar31 = *piVar37;
                  if (iVar34 == 0) {
                    pVVar27 = (Vector *)0x1d3;
                  }
                  goto LAB_0018e25a;
                }
              }
            }
            iVar34 = Status::getCurrentCampaignMission();
            if (((0x79 < iVar34) && (iVar34 = Status::getCurrentCampaignMission(), iVar34 < 0x7d))
               && (iVar34 = PlayerEgo::isMining(), iVar34 == 0)) {
              Status::getMission();
              iVar34 = Mission::isEmpty();
              if (iVar34 != 0) {
                lVar48 = Status::getPlayingTime();
                lVar48 = lVar48 - *(longlong *)(*piVar36 + 0x100);
                iVar34 = (int)((ulonglong)lVar48 >> 0x20);
                bVar39 = DAT_0018e3f8 < (uint)lVar48;
                if ((((int)(-(uint)bVar39 - iVar34) < 0 !=
                      (SBORROW4(0,iVar34) != SBORROW4(-iVar34,(uint)bVar39))) &&
                    ((char)this[1].field_14 == '\0')) &&
                   (*(char *)((int)&this[1].field_14 + 1) == '\0')) {
                  AbyssEngine::AERandom::nextInt(**(int **)(DAT_0018f318 + 0x18f18a));
                  Status::getCurrentCampaignMission();
                  Level::createRadioMessage((int)this->field_78,0x1c);
                  goto LAB_0018e26c;
                }
              }
            }
            iVar34 = Status::inAlienOrbit();
            if (((iVar34 == 0) && (*(char *)(*piVar36 + 0x17c) != '\0')) &&
               ((lVar48 = Status::getPlayingTime(),
                lVar48 = lVar48 - *(longlong *)(*piVar36 + 0x100),
                (int)(uint)((uint)lVar48 < 0x2ee1) <= (int)((ulonglong)lVar48 >> 0x20) &&
                (((char)this[1].field_14 == '\0' && (*(char *)((int)&this[1].field_14 + 1) == '\0'))
                )))) {
              uVar6 = 0xffffffff;
              iVar34 = DAT_0018e41c + 0x18e32c;
              for (iVar31 = 0; iVar31 != 3; iVar31 = iVar31 + 1) {
                iVar33 = *(int *)(iVar34 + iVar31 * 4);
                iVar38 = Status::getCurrentCampaignMission();
                if (iVar33 <= iVar38) {
                  uVar6 = uVar6 + 1;
                }
              }
              if (uVar6 < 3) {
                *(undefined1 *)(*piVar36 + 0x17c) = 0;
                Level::createRadioMessage((int)this->field_78,0x1b);
              }
            }
          }
          iVar34 = *(int *)(DAT_0018e420 + 0x18e36e);
          if (*(char *)(iVar34 + 0x17) == '\0') {
            Status::getShip();
            iVar31 = Ship::hasBooster();
            if ((iVar31 == 0) || (iVar31 = Status::getCurrentCampaignMission(), iVar31 < 2))
            goto LAB_0018e42c;
            if (this->field_94 == 0) {
              pCVar8 = operator_new(0x5c);
              ChoiceWindow::ChoiceWindow(pCVar8);
              this->field_94 = (int)pCVar8;
            }
            GameText::getText(**(int **)(DAT_0018e428 + 0x18e3b4));
            Globals::replaceKeyBindingTokens((String *)&local_e8);
            ChoiceWindow::set((String *)this->field_94);
            *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
            *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            *(undefined1 *)(iVar34 + 0x17) = 1;
          }
          else {
LAB_0018e42c:
            if (*(char *)(iVar34 + 0x21) == '\0') {
              Status::getShip();
              iVar31 = Ship::hasJumpDrive();
              if (iVar31 != 0) {
                if (this->field_94 == 0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018e7a4 + 0x18e460));
                Globals::replaceKeyBindingTokens((String *)&local_e8);
                ChoiceWindow::set((String *)this->field_94);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                *(undefined1 *)(iVar34 + 0x21) = 1;
                goto LAB_0018e608;
              }
            }
            if (*(char *)(iVar34 + 0x22) == '\0') {
              Status::getShip();
              iVar31 = Ship::hasCloak();
              if (iVar31 != 0) {
                if (this->field_94 == 0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018e7ac + 0x18e4d2));
                Globals::replaceKeyBindingTokens((String *)&local_e8);
                ChoiceWindow::set((String *)this->field_94);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                *(undefined1 *)(iVar34 + 0x22) = 1;
                goto LAB_0018e608;
              }
            }
            if (((*(char *)(iVar34 + 0x20) == '\0') &&
                (iVar31 = PlayerEgo::isAutoPilot(), iVar31 == 0)) &&
               (iVar31 = Status::getCurrentCampaignMission(), 9 < iVar31)) {
              Status::getMission();
              iVar31 = Mission::isEmpty();
              if (iVar31 != 0) {
                if (this->field_94 == 0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018e7b4 + 0x18e55e));
                Globals::replaceKeyBindingTokens((String *)&local_e8);
                ChoiceWindow::set((String *)this->field_94);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                *(undefined1 *)(iVar34 + 0x20) = 1;
                goto LAB_0018e608;
              }
            }
            if ((*(int *)(*piVar36 + 0x24) == 0) || (*(char *)(iVar34 + 0x15) != '\0')) {
              if (*(char *)(iVar34 + 0x1c) == '\0') {
                Status::getStanding();
                iVar31 = Standing::isEnemyWithAnyone();
                if (iVar31 != 0) {
                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                  pauseSounds();
                  pCVar8 = (ChoiceWindow *)this->field_94;
                  if (pCVar8 == (ChoiceWindow *)0x0) {
                    pCVar8 = operator_new(0x5c);
                    ChoiceWindow::ChoiceWindow(pCVar8);
                    this->field_94 = (int)pCVar8;
                  }
                  GameText::getText(**(int **)(DAT_0018e7c0 + 0x18e650));
                  ChoiceWindow::set(pCVar8);
                  *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                  *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                  *(undefined1 *)(iVar34 + 0x1c) = 1;
                  goto LAB_0018f164;
                }
              }
              if ((*(char *)((int)&this[2].field_54 + 2) == '\0') &&
                 (iVar31 = Status::getCurrentCampaignMission(), iVar31 == 5)) {
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                pCVar8 = (ChoiceWindow *)this->field_94;
                if (pCVar8 == (ChoiceWindow *)0x0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018e7c4 + 0x18e6ae));
                ChoiceWindow::set(pCVar8);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)((int)&this[2].field_54 + 2) = 1;
                goto LAB_0018f164;
              }
              if (*(char *)(iVar34 + 0x11) == '\0') {
                iVar31 = PlayerEgo::isMining();
                if (((iVar31 == 0) && (*(char *)((int)&this[2].field_54 + 1) == '\0')) &&
                   (iVar31 = Status::getCurrentCampaignMission(), iVar31 == 2)) {
                  iVar31 = Player::getHitpoints();
                  iVar38 = Player::getMaxHitpoints();
                  if ((iVar31 < iVar38) ||
                     ((int)(uint)((uint)this->field_7C[2] < 0x9c41) <= this->field_7C[3])) {
                    *(undefined1 *)((int)&this->field_5C + 1) = 1;
                    pauseSounds();
                    if (this->field_94 == 0) {
                      pCVar8 = operator_new(0x5c);
                      ChoiceWindow::ChoiceWindow(pCVar8);
                      this->field_94 = (int)pCVar8;
                    }
                    pSVar9 = (String *)GameText::getText(**(int **)(DAT_0018e7c8 + 0x18e754));
                    AbyssEngine::String::String((String *)&local_e8,pSVar9,false);
                    Globals::replaceKeyBindingTokens(aSStack_48);
                    AbyssEngine::String::operator=((String *)&local_e8,aSStack_48);
                    AbyssEngine::String::~String(aSStack_48);
                    ChoiceWindow::set((String *)this->field_94);
                    *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                    *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                    *(undefined1 *)((int)&this[2].field_54 + 1) = 1;
                    goto LAB_0018e608;
                  }
                }
                if (((*(char *)(iVar34 + 0x11) == '\0') &&
                    (iVar31 = PlayerEgo::isMining(), iVar31 != 0)) &&
                   (*(char *)(iVar34 + 0x11) == '\0')) {
                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                  pauseSounds();
                  pCVar8 = (ChoiceWindow *)this->field_94;
                  if (pCVar8 == (ChoiceWindow *)0x0) {
                    pCVar8 = operator_new(0x5c);
                    ChoiceWindow::ChoiceWindow(pCVar8);
                    this->field_94 = (int)pCVar8;
                  }
                  GameText::getText(**(int **)(DAT_0018f300 + 0x18f06a));
                  ChoiceWindow::set(pCVar8);
                  *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                  *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                  *(undefined1 *)(iVar34 + 0x11) = 1;
                  goto LAB_0018f164;
                }
              }
              if (((*(char *)(iVar34 + 0x12) == '\0') &&
                  (iVar31 = PlayerEgo::isMining(), iVar31 != 0)) &&
                 (iVar31 = Status::getCurrentCampaignMission(), 3 < iVar31)) {
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                pCVar8 = (ChoiceWindow *)this->field_94;
                if (pCVar8 == (ChoiceWindow *)0x0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018eb7c + 0x18e824));
                ChoiceWindow::set(pCVar8);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)(iVar34 + 0x12) = 1;
                goto LAB_0018f164;
              }
              uVar6 = Status::getCurrentCampaignMission();
              bVar39 = uVar6 == 2;
              if (bVar39) {
                uVar6 = (uint)*(byte *)(iVar34 + 0x37);
              }
              if ((bVar39 && uVar6 == 0) && (iVar31 = PlayerEgo::lostMiningGame(), iVar31 != 0)) {
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                pCVar8 = (ChoiceWindow *)this->field_94;
                if (pCVar8 == (ChoiceWindow *)0x0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018ef40 + 0x18ebd4));
                ChoiceWindow::set(pCVar8);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)(iVar34 + 0x37) = 1;
                goto LAB_0018f164;
              }
              if (((*(char *)(iVar34 + 0x14) == '\0') && (iVar31 = Hud::cargoFull(), iVar31 != 0))
                 && (iVar31 = Status::getCurrentCampaignMission(), 6 < iVar31)) {
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                pCVar8 = (ChoiceWindow *)this->field_94;
                if (pCVar8 == (ChoiceWindow *)0x0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018eb80 + 0x18e8a6));
                ChoiceWindow::set(pCVar8);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)(iVar34 + 0x14) = 1;
                goto LAB_0018f164;
              }
              if ((*(char *)(iVar34 + 0x23) == '\0') &&
                 (iVar31 = Status::inAlienOrbit(), iVar31 == 0)) {
                Status::getSystem();
                iVar31 = SolarSystem::getRace();
                if (iVar31 == 2) {
                  Level::createRadioMessage((int)this->field_78,5);
                  *(undefined1 *)(iVar34 + 0x23) = 1;
                  *(undefined1 *)&this[1].field_18 = 1;
                  goto LAB_0018f164;
                }
              }
              if (((*(char *)(iVar34 + 0x23) != '\0') &&
                  ((char)this[1].field_18 == '\0' && *(char *)(iVar34 + 0x24) == '\0')) &&
                 (iVar31 = Status::inAlienOrbit(), iVar31 == 0)) {
                Status::getSystem();
                iVar31 = SolarSystem::getRace();
                if (iVar31 == 2) {
                  Level::createRadioMessage((int)this->field_78,6);
                  *(undefined1 *)(iVar34 + 0x24) = 1;
                  goto LAB_0018f164;
                }
              }
              if (*(char *)(iVar34 + 0x25) == '\0') {
                iVar31 = Status::getShip();
                iVar31 = Ship::getFirstEquipmentOfSort(iVar31);
                if ((iVar31 != 0) && (iVar31 = PlayerEgo::isMining(), iVar31 == 0)) {
                  Status::getMission();
                  iVar31 = Mission::isEmpty();
                  if ((iVar31 != 0) &&
                     (iVar31 = Status::getCurrentCampaignMission(), iVar31 == 0x18)) {
                    *(undefined1 *)((int)&this->field_5C + 1) = 1;
                    pauseSounds();
                    if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
                      pvVar21 = (void *)DialogueWindow::~DialogueWindow
                                                  ((DialogueWindow *)this->field_8C);
                      operator_delete(pvVar21);
                      this->field_8C = 0;
                    }
                    pDVar18 = operator_new(0x74);
                    piVar36 = *(int **)(DAT_0018f2f4 + 0x18efcc);
                    pSVar9 = (String *)GameText::getText(*piVar36);
                    pSVar19 = (String *)GameText::getText(*piVar36);
                    fVar41 = (float)DialogueWindow::DialogueWindow
                                              (pDVar18,pSVar9,pSVar19,
                                               (int *)(DAT_0018f2f8 + 0x18f012));
                    piVar36 = *(int **)(DAT_0018f2fc + 0x18f002);
                    this->field_8C = (int)pDVar18;
                    FModSound::play(*piVar36,(Vector *)0x1cf,(Vector *)0x0,fVar41);
                    *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                    *(undefined1 *)((int)&this->field_5C + 2) = 1;
                    *(undefined1 *)(iVar34 + 0x25) = 1;
                    goto LAB_0018f164;
                  }
                }
              }
              if ((*(char *)(iVar34 + 0x28) == '\0') &&
                 (iVar31 = PlayerEgo::isHacking(), iVar31 != 0)) {
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                pCVar8 = (ChoiceWindow *)this->field_94;
                if (pCVar8 == (ChoiceWindow *)0x0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018eb84 + 0x18e996));
                ChoiceWindow::set(pCVar8);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)(iVar34 + 0x28) = 1;
                goto LAB_0018f164;
              }
              if (*(char *)(iVar34 + 0x29) == '\0') {
                iVar31 = Status::getCurrentCampaignMission();
                if (iVar31 == 0x5b) {
                  pSVar30 = (Station *)Status::getStation();
                  iVar31 = Station::getIndex(pSVar30);
                  if (iVar31 != 0x6e) goto LAB_0018e9ec;
                  Level::getMessages();
                  iVar31 = RadioMessage::isOver();
                  if (iVar31 == 0) goto LAB_0018e9ec;
LAB_0018ea26:
                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                  pauseSounds();
                  pCVar8 = (ChoiceWindow *)this->field_94;
                  if (pCVar8 == (ChoiceWindow *)0x0) {
                    pCVar8 = operator_new(0x5c);
                    ChoiceWindow::ChoiceWindow(pCVar8);
                    this->field_94 = (int)pCVar8;
                  }
                  GameText::getText(**(int **)(DAT_0018eb88 + 0x18ea56));
                  ChoiceWindow::set(pCVar8);
                  *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                  *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                  *(undefined1 *)(iVar34 + 0x29) = 1;
                  goto LAB_0018f164;
                }
LAB_0018e9ec:
                iVar31 = Status::getCurrentCampaignMission();
                if (iVar31 != 0x5b) {
                  iVar38 = *piVar36;
                  pSVar30 = (Station *)Status::getStation();
                  iVar31 = Station::getIndex(pSVar30);
                  Status::getCurrentCampaignMission();
                  fVar41 = (float)Status::getGammaRayDamagePerSecond(iVar38,iVar31);
                  uVar6 = in_fpscr & 0xfffffff | (uint)(fVar41 < 0.0) << 0x1f |
                          (uint)(fVar41 == 0.0) << 0x1e;
                  in_fpscr = uVar6 | (uint)NAN(fVar41) << 0x1c;
                  bVar2 = (byte)(uVar6 >> 0x18);
                  if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))
                  goto LAB_0018ea26;
                }
              }
              if ((*(char *)(iVar34 + 0x2c) == '\0') &&
                 (iVar31 = PlayerEgo::hasVolatileGoods(), iVar31 != 0)) {
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                pCVar8 = (ChoiceWindow *)this->field_94;
                if (pCVar8 == (ChoiceWindow *)0x0) {
                  pCVar8 = operator_new(0x5c);
                  ChoiceWindow::ChoiceWindow(pCVar8);
                  this->field_94 = (int)pCVar8;
                }
                GameText::getText(**(int **)(DAT_0018eb8c + 0x18eab6));
                ChoiceWindow::set(pCVar8);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                *(undefined1 *)(iVar34 + 0x2c) = 1;
                goto LAB_0018f164;
              }
              if ((*(char *)(iVar34 + 0x2e) == '\0') &&
                 (iVar31 = Status::getCurrentCampaignMission(), iVar31 == 0x5b)) {
                pSVar30 = (Station *)Status::getStation();
                iVar31 = Station::getIndex(pSVar30);
                if (iVar31 == 0x6e) {
                  Level::getMessages();
                  iVar31 = RadioMessage::isOver();
                  if (iVar31 != 0) {
                    *(undefined1 *)((int)&this->field_5C + 1) = 1;
                    pauseSounds();
                    if (this->field_94 == 0) {
                      pCVar8 = operator_new(0x5c);
                      ChoiceWindow::ChoiceWindow(pCVar8);
                      this->field_94 = (int)pCVar8;
                    }
                    GameText::getText(**(int **)(DAT_0018eb94 + 0x18eb42));
                    Globals::replaceKeyBindingTokens((String *)&local_e8);
                    ChoiceWindow::set((String *)this->field_94);
                    *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                    *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                    *(undefined1 *)(iVar34 + 0x2e) = 1;
                    goto LAB_0018e608;
                  }
                }
              }
              if ((*(char *)(iVar34 + 0x2f) == '\0') &&
                 (iVar31 = Status::getCurrentCampaignMission(), iVar31 == 0x5b)) {
                pSVar30 = (Station *)Status::getStation();
                iVar31 = Station::getIndex(pSVar30);
                if ((iVar31 == 0x6e) &&
                   ((iVar31 = PlayerEgo::isDockedToDockingPoint(), iVar31 != 0 &&
                    (*(int *)(*piVar36 + 0x178) == 10)))) {
                  Level::getMessages();
                  iVar31 = RadioMessage::isOver();
                  if (iVar31 != 0) {
                    *(undefined1 *)((int)&this->field_5C + 1) = 1;
                    pauseSounds();
                    if (this->field_94 == 0) {
                      pCVar8 = operator_new(0x5c);
                      ChoiceWindow::ChoiceWindow(pCVar8);
                      this->field_94 = (int)pCVar8;
                    }
                    GameText::getText(**(int **)(DAT_0018ef48 + 0x18ec6e));
                    Globals::replaceKeyBindingTokens((String *)&local_e8);
                    ChoiceWindow::set((String *)this->field_94);
                    *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                    *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                    *(undefined1 *)(iVar34 + 0x2f) = 1;
                    goto LAB_0018e608;
                  }
                }
              }
              if ((*(char *)(iVar34 + 0x30) == '\0') &&
                 (iVar31 = Status::getCurrentCampaignMission(), iVar31 == 0x5c)) {
                pSVar30 = (Station *)Status::getStation();
                iVar31 = Station::getIndex(pSVar30);
                if ((iVar31 == 0x71) &&
                   ((iVar31 = PlayerEgo::isDockedToDockingPoint(), iVar31 != 0 &&
                    (*(int *)(*piVar36 + 0x178) == 0)))) {
                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                  pauseSounds();
                  if (this->field_94 == 0) {
                    pCVar8 = operator_new(0x5c);
                    ChoiceWindow::ChoiceWindow(pCVar8);
                    this->field_94 = (int)pCVar8;
                  }
                  GameText::getText(**(int **)(DAT_0018f570 + 0x18f2b0));
                  Globals::replaceKeyBindingTokens((String *)&local_e8);
                  ChoiceWindow::set((String *)this->field_94);
                  *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                  *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
                  *(undefined1 *)(iVar34 + 0x30) = 1;
                  goto LAB_0018e608;
                }
              }
              if ((((*(char *)(iVar5 + 0x35) != '\0') && (*(char *)(iVar34 + 0x26) == '\0')) &&
                  (*(char *)((int)&this->field_5C + 2) == '\0')) &&
                 (iVar31 = PlayerEgo::isMining(), iVar31 == 0)) {
                Status::getMission();
                iVar31 = Mission::isEmpty();
                if (((iVar31 == 0) || (iVar31 = PlayerEgo::isAutoPilot(), iVar31 != 0)) ||
                   (iVar31 = Status::gameWon(), iVar31 == 0)) goto LAB_0018ed9c;
                *(undefined1 *)((int)&this->field_5C + 1) = 1;
                pauseSounds();
                if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
                  pvVar21 = (void *)DialogueWindow::~DialogueWindow
                                              ((DialogueWindow *)this->field_8C);
                  operator_delete(pvVar21);
                  this->field_8C = 0;
                }
                pDVar18 = operator_new(0x74);
                DialogueWindow::DialogueWindow(pDVar18);
                this->field_8C = (int)pDVar18;
                pMVar17 = operator_new(0x78);
                Mission::Mission(pMVar17,0xa0,0,-1);
                DialogueWindow::set((Mission *)this->field_8C,(int)pMVar17,1);
                *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                *(undefined1 *)((int)&this->field_5C + 2) = 1;
                uVar3 = (undefined1)*piVar36;
                *(undefined1 *)(iVar34 + 0x26) = 1;
LAB_0018ee56:
                Status::nextCampaignMission((bool)uVar3);
                Status::nextCampaignMission(SUB41(*piVar36,0));
                goto LAB_0018f164;
              }
LAB_0018ed9c:
              if (((*(char *)(iVar5 + 0x37) != '\0') && (*(char *)(iVar34 + 0x31) == '\0')) &&
                 ((*(char *)((int)&this->field_5C + 2) == '\0' &&
                  (iVar31 = PlayerEgo::isMining(), iVar31 == 0)))) {
                Status::getMission();
                iVar31 = Mission::isEmpty();
                if (((iVar31 != 0) && (iVar31 = PlayerEgo::isAutoPilot(), iVar31 == 0)) &&
                   (iVar31 = Status::dlc1Won(), iVar31 != 0)) {
                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                  pauseSounds();
                  if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
                    pvVar21 = (void *)DialogueWindow::~DialogueWindow
                                                ((DialogueWindow *)this->field_8C);
                    operator_delete(pvVar21);
                    this->field_8C = 0;
                  }
                  pDVar18 = operator_new(0x74);
                  DialogueWindow::DialogueWindow(pDVar18);
                  this->field_8C = (int)pDVar18;
                  pMVar17 = operator_new(0x78);
                  Mission::Mission(pMVar17,0xa0,0,-1);
                  DialogueWindow::set((Mission *)this->field_8C,(int)pMVar17,1);
                  *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                  *(undefined1 *)((int)&this->field_5C + 2) = 1;
                  uVar3 = (undefined1)*piVar36;
                  *(undefined1 *)(iVar34 + 0x31) = 1;
                  goto LAB_0018ee56;
                }
              }
              iVar34 = Status::inBlackMarketSystem();
              if (((iVar34 != 0) && (*(char *)(*piVar36 + 0x110) == '\0')) &&
                 ((*(char *)(*piVar36 + 0x111) == '\0' &&
                  (((char)this[1].field_0C == '\0' && (iVar34 = Level::getMessages(), iVar34 != 0)))
                  ))) {
                Level::getMessages();
                iVar34 = RadioMessage::isOver();
                if (iVar34 != 0) {
                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                  pauseSounds();
                  if (this->field_94 == 0) {
                    pCVar8 = operator_new(0x5c);
                    ChoiceWindow::ChoiceWindow(pCVar8);
                    this->field_94 = (int)pCVar8;
                  }
                  this[1].field_8 = (int *)0x0;
                  Status::getShip();
                  puVar7 = (uint *)Ship::getCargo();
                  if (puVar7 != (uint *)0x0) {
                    for (uVar6 = 0; uVar6 < *puVar7; uVar6 = uVar6 + 1) {
                      if (*(int *)(puVar7[1] + uVar6 * 4) != 0) {
                        iVar34 = Item::getAmount();
                        iVar31 = Item::getSinglePrice(*(Item **)(puVar7[1] + uVar6 * 4));
                        this[1].field_8 = (int *)(iVar31 * iVar34 + (int)this[1].field_8);
                      }
                    }
                  }
                  piVar37 = this[1].field_8;
                  if (piVar37 == (int *)0x0) {
                    piVar37 = (int *)0x64;
                    this[1].field_8 = (int *)0x64;
                  }
                  fVar40 = *(float *)(iVar5 + 0x2c);
                  uVar6 = in_fpscr & 0xfffffff;
                  in_fpscr = uVar6 | (uint)(fVar40 == 0.0) << 0x1e | (uint)(0.0 <= fVar40) << 0x1d;
                  bVar2 = (byte)(in_fpscr >> 0x18);
                  fVar41 = DAT_0018f55c;
                  if (((bool)(bVar2 >> 5 & 1) && !(bool)(bVar2 >> 6)) &&
                     (in_fpscr = uVar6 | (uint)(fVar40 == 0.5) << 0x1e, fVar41 = DAT_0018f560,
                     (byte)(in_fpscr >> 0x1e) == 0)) {
                    pfVar13 = (float *)&DAT_0018f564;
                    in_fpscr = uVar6 | (uint)(fVar40 == 1.0) << 0x1e;
                    if ((byte)(in_fpscr >> 0x1e) != 0) {
                      pfVar13 = (float *)&DAT_0018f568;
                    }
                    fVar41 = *pfVar13;
                  }
                  fVar40 = (float)VectorSignedToFloat(piVar37,(byte)(in_fpscr >> 0x16) & 3);
                  piVar37 = *(int **)(DAT_0018f574 + 0x18f34a);
                  this[1].field_8 = (int *)(int)(fVar41 * fVar40);
                  iVar34 = *piVar36;
                  pSVar9 = (String *)GameText::getText(*piVar37);
                  AbyssEngine::String::String(aSStack_f4,pSVar9,false);
                  Layout::formatCredits((int)auStack_100);
                  uVar10 = AbyssEngine::String::String
                                     (aSStack_10c,(char *)(DAT_0018f57c + 0x18f37e),false);
                  Status::replaceHash(&local_e8,iVar34,aSStack_f4,auStack_100,uVar10);
                  pcVar26 = *(code **)(DAT_0018f580 + 0x18f398);
                  (*pcVar26)(aSStack_10c);
                  (*pcVar26)(auStack_100);
                  (*pcVar26)(aSStack_f4);
                  iVar34 = *piVar36;
                  AbyssEngine::String::String(aSStack_118,(String *)&local_e8,false);
                  AbyssEngine::String::String(aSStack_124,(int)(fVar41 * DAT_0018f558));
                  uVar10 = AbyssEngine::String::String
                                     (aSStack_130,(char *)(DAT_0018f584 + 0x18f3ce),false);
                  Status::replaceHash(aSStack_48,iVar34,aSStack_118,aSStack_124,uVar10);
                  AbyssEngine::String::operator=((String *)&local_e8,aSStack_48);
                  (*pcVar26)(aSStack_48);
                  (*pcVar26)(aSStack_130);
                  (*pcVar26)(aSStack_124);
                  (*pcVar26)(aSStack_118);
                  ChoiceWindow::set((ChoiceWindow *)this->field_94,(String *)&local_e8,true);
                  *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                  *(undefined2 *)((int)&this[1].field_4 + 2) = 0x101;
                  *(undefined1 *)&this[1].field_0C = 1;
                  AbyssEngine::String::~String((String *)&local_e8);
                  goto LAB_0018dc40;
                }
              }
              if ((*(int *)(*piVar36 + 0x114) == 0) &&
                 (iVar34 = Status::inAlienOrbit(), iVar34 == 0)) {
                pSVar30 = (Station *)Status::getStation();
                iVar34 = Station::getIndex(pSVar30);
                if (iVar34 == 0x6c) {
                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                  pauseSounds();
                  if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
                    pvVar21 = (void *)DialogueWindow::~DialogueWindow
                                                ((DialogueWindow *)this->field_8C);
                    operator_delete(pvVar21);
                    this->field_8C = 0;
                  }
                  pDVar18 = operator_new(0x74);
                  piVar37 = *(int **)(DAT_0018ef4c + 0x18eeea);
                  pSVar9 = (String *)GameText::getText(*piVar37);
                  pSVar19 = (String *)GameText::getText(*piVar37);
                  fVar41 = (float)DialogueWindow::DialogueWindow
                                            (pDVar18,pSVar9,pSVar19,(int *)(DAT_0018ef50 + 0x18ef08)
                                            );
                  piVar37 = *(int **)(DAT_0018ef54 + 0x18ef1a);
                  this->field_8C = (int)pDVar18;
                  FModSound::play(*piVar37,(Vector *)0x5bc,(Vector *)0x0,fVar41);
                  *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
                  *(undefined1 *)((int)&this->field_5C + 2) = 1;
                  *(undefined4 *)(*piVar36 + 0x114) = 1;
                  goto LAB_0018f164;
                }
              }
              goto LAB_0018dc40;
            }
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            if (this->field_94 == 0) {
              pCVar8 = operator_new(0x5c);
              ChoiceWindow::ChoiceWindow(pCVar8);
              this->field_94 = (int)pCVar8;
            }
            GameText::getText(**(int **)(DAT_0018e7bc + 0x18e5d6));
            Globals::replaceKeyBindingTokens((String *)&local_e8);
            ChoiceWindow::set((String *)this->field_94);
            *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
            *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
            *(undefined1 *)(iVar34 + 0x15) = 1;
          }
LAB_0018e608:
          AbyssEngine::String::~String((String *)&local_e8);
          goto LAB_0018f164;
        }
LAB_0018dc40:
        if (((int)(uint)((uint)this->field_48 < 0x1389) <= this->field_4C) &&
           (*(char *)((int)&this->field_5C + 2) == '\0')) {
          Status::getMission();
          iVar34 = Mission::hasFailed();
          if (iVar34 == 0) {
            pMVar17 = (Mission *)Status::getMission();
            iVar34 = Mission::hasWon(pMVar17);
            if (iVar34 == 0) {
              dialogueEvent();
            }
          }
        }
      }
    }
  }
  iVar34 = PlayerEgo::boosting();
  if ((iVar34 != 0) && (iVar34 = PlayerEgo::isDockingToPlanet(), iVar34 == 0)) {
    pcVar26 = *(code **)(DAT_0018d5bc + 0x18d356);
    pEVar12 = (Engine *)(*pcVar26)(this->field_8);
    AbyssEngine::Engine::SetPostEffect(pEVar12,DAT_0018d5c0,true);
    fVar42 = (float)PlayerEgo::getBoostPercentage();
    fVar41 = DAT_0018d128;
    uVar6 = in_fpscr & 0xfffffff | (uint)(fVar42 < 0.0) << 0x1f | (uint)(fVar42 == 0.0) << 0x1e;
    in_fpscr = uVar6 | (uint)NAN(fVar42) << 0x1c;
    bVar2 = (byte)(uVar6 >> 0x18);
    fVar40 = DAT_0018d128;
    if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
      fVar40 = fVar42;
    }
    iVar31 = (*pcVar26)(this->field_8);
    iVar34 = DAT_0018d5c4;
    *(float *)(iVar31 + 0x3c8) = fVar40;
    puVar7 = *(uint **)(iVar34 + 0x18d396);
    pVVar27 = (Vector *)*puVar7;
    local_e8 = 0;
    uStack_e4 = 0;
    local_e0 = 0;
    PlayerEgo::getPosition();
    AbyssEngine::PaintCanvas::GetScreenPosition(pVVar27,(Vector *)aSStack_48);
    (*pcVar26)(this->field_8);
    uVar10 = AbyssEngine::Engine::GetDisplayWidth();
    pcVar35 = *(code **)(DAT_0018d5c8 + 0x18d3ce);
    pfVar13 = (float *)(*pcVar35)((Vector *)&local_e8,0);
    fVar40 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
    *pfVar13 = *pfVar13 / fVar40;
    pfVar13 = (float *)(*pcVar35)((Vector *)&local_e8,1);
    fVar42 = *pfVar13;
    (*pcVar26)(this->field_8);
    uVar10 = AbyssEngine::Engine::GetDisplayHeight();
    fVar40 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
    pfVar13 = (float *)(*pcVar35)((Vector *)&local_e8,1);
    *pfVar13 = 1.0 - fVar42 / fVar40;
    iVar34 = (*pcVar26)(this->field_8);
    AbyssEngine::AEMath::Vector::operator=((Vector *)(iVar34 + 0x3cc),(Vector *)&local_e8);
    fVar40 = (float)PlayerEgo::getBoostPercentage();
    this->field_70 = (int *)(DAT_0018d5d0 + fVar40 * DAT_0018d5cc);
    uVar6 = *puVar7;
    iVar34 = Status::inAlienOrbit();
    fVar40 = DAT_0018d5e4;
    fVar42 = extraout_s1;
    fVar16 = extraout_s2;
    if (iVar34 != 0) {
      iVar34 = Status::getCurrentCampaignMission();
      pfVar13 = (float *)&DAT_0018d5d8;
      if (iVar34 < 0x50) {
        pfVar13 = (float *)&DAT_0018d5dc;
      }
      fVar40 = *pfVar13;
      fVar42 = extraout_s1_00;
      fVar16 = extraout_s2_00;
    }
    AbyssEngine::PaintCanvas::CameraSetPerspective(uVar6,fVar40,fVar42,fVar16);
    pTVar28 = (TargetFollowCamera *)this[1].field_2C;
    fVar40 = (float)PlayerEgo::getBoostPercentage();
    in_fpscr = in_fpscr & 0xfffffff;
    if (0.0 <= fVar40) {
      fVar41 = (float)PlayerEgo::getBoostPercentage();
    }
    fVar40 = (float)PlayerEgo::getBoostSpeed();
    TargetFollowCamera::setBoostPercentage(pTVar28,fVar40,(int)fVar41);
  }
  if ((char)this->field_60 == '\0') {
    iVar34 = PlayerEgo::isDead(*ppPVar1);
    if (iVar34 == 0) {
      Status::getMission();
      iVar34 = Mission::hasFailed();
      if ((iVar34 == 0) && (iVar34 = successCheck(this), iVar34 != 0)) goto LAB_0018f164;
    }
    gameOverCheck();
  }
  else if ((int)(uint)((uint)this->field_48 < 0xbb9) <= this->field_4C) {
    this->field_50 = this->field_40 + this->field_50;
  }
  cVar20 = *(char *)((int)&this->field_5C + 3);
  iVar34 = LevelScript::process((int)this->field_7C);
  *(char *)((int)&this->field_5C + 3) = (char)iVar34;
  if ((cVar20 == '\0') && (iVar34 == 1)) {
    puVar14 = *(undefined1 **)(DAT_0018d954 + 0x18d6b4);
    *(undefined1 *)((int)&this[2].field_4C + 1) = *puVar14;
    *puVar14 = 1;
    if (0 < this[1].field_A4) {
      FModSound::setDownPitch(SUB41(**(undefined4 **)(DAT_0018d958 + 0x18d6d0),0));
      Hud::setTimeExtender(SUB41(this->m_pLevel,0),*(bool *)&this->m_pLevel->field_0,false,false);
    }
    pSVar15 = this->m_pLevel;
    this->field_98 = 0;
    this->field_9C = 0;
    this->field_A0 = 0;
    *(undefined1 *)&this[1].field_94 = 0;
    *(undefined1 *)&this->field_5C = 0;
    *(undefined1 *)&this[1].field_48 = 0;
    Hud::enableFireForTutorial(SUB41(pSVar15,0));
    iVar34 = PlayerEgo::boosting();
    if (iVar34 != 0) {
      PlayerEgo::stopBoost();
    }
    puVar7 = *(uint **)(DAT_0018d960 + 0x18d724);
    this->field_70 = DAT_0018d964;
    uVar6 = *puVar7;
    iVar34 = Status::inAlienOrbit();
    fVar41 = DAT_0018d5e4;
    fVar40 = extraout_s1_03;
    fVar42 = extraout_s2_01;
    if (iVar34 != 0) {
      iVar34 = Status::getCurrentCampaignMission();
      pfVar13 = (float *)&DAT_0018d5d8;
      if (iVar34 < 0x50) {
        pfVar13 = (float *)&DAT_0018d5dc;
      }
      fVar41 = *pfVar13;
      fVar40 = extraout_s1_04;
      fVar42 = extraout_s2_02;
    }
    fVar41 = (float)AbyssEngine::PaintCanvas::CameraSetPerspective(uVar6,fVar41,fVar40,fVar42);
    TargetFollowCamera::setBoostPercentage((TargetFollowCamera *)this[1].field_2C,fVar41,0);
    pEVar12 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
    AbyssEngine::Engine::SetPostEffect(pEVar12,DAT_0018d96c,false);
    fVar41 = extraout_s1_05;
  }
  else {
    fVar41 = extraout_s1_02;
    if (iVar34 == 0 && cVar20 != '\0') {
      **(undefined1 **)(DAT_0018d968 + 0x18d766) = *(undefined1 *)((int)&this[2].field_4C + 1);
    }
  }
  if (*(char *)((int)this->field_7C + 0x13) != '\0') goto LAB_0018f164;
  if (*(char *)((int)this->field_7C + 0x12) != '\0') {
    this->field_48 = 0x1389;
    this->field_4C = 0;
  }
  if (this->field_14 == 3) {
    if ((char)this[1].field_94 == '\0') {
      piVar37 = (int *)((float)this[1].m_pLevel * (float)this[1].field_80);
      in_fpscr = in_fpscr & 0xfffffff;
      piVar36 = (int *)-((float)this[1].m_pLevel * (float)this[1].field_80);
      if (0.0 < (float)piVar37) {
        piVar36 = piVar37;
      }
      this[1].field_80 = piVar37;
      if (1.0 < (float)piVar36) {
        this[1].field_50 = (int)((float)piVar37 + (float)this[1].field_50);
      }
      fVar40 = (float)this[1].field_54;
      piVar37 = (int *)((float)this[1].field_78 * (float)this[1].field_84);
      piVar36 = (int *)-((float)this[1].field_78 * (float)this[1].field_84);
      if (0.0 < (float)piVar37) {
        piVar36 = piVar37;
      }
      this[1].field_84 = piVar37;
      fVar42 = DAT_0018d5e0;
      if (1.0 < (float)piVar36) {
        fVar40 = (float)piVar37 + fVar40;
        this[1].field_54 = (int)fVar40;
      }
      uVar6 = in_fpscr | (uint)(fVar40 < fVar42) << 0x1f | (uint)(fVar40 == fVar42) << 0x1e;
      uVar23 = uVar6 | (uint)(NAN(fVar40) || NAN(fVar42)) << 0x1c;
      bVar2 = (byte)(uVar6 >> 0x18);
      fVar16 = DAT_0018d980;
      if ((!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar23 >> 0x1c) & 1)) ||
         (fVar16 = DAT_0018dd24, uVar23 = in_fpscr, fVar42 = DAT_0018dd24, fVar40 < DAT_0018dd24)) {
        fVar40 = fVar42;
        in_fpscr = uVar23;
        this[1].field_54 = (int)fVar16;
      }
    }
    else {
      fVar40 = (float)this[1].field_54;
    }
    TargetFollowCamera::rotateAroundTarget
              ((TargetFollowCamera *)this[1].field_2C,fVar40 * DAT_0018dd28,fVar41,
               (float)this[1].field_50 * DAT_0018dd28);
  }
  fVar41 = (float)this[1].field_A0;
  uVar6 = in_fpscr & 0xfffffff | (uint)(fVar41 < 1.0) << 0x1f | (uint)(fVar41 == 1.0) << 0x1e;
  uVar23 = uVar6 | (uint)NAN(fVar41) << 0x1c;
  bVar2 = (byte)(uVar6 >> 0x18);
  if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(uVar23 >> 0x1c) & 1)) {
    TargetFollowCamera::update(this[1].field_2C);
  }
  else {
    for (iVar34 = 5; iVar34 != 0; iVar34 = iVar34 + -1) {
      if (this[1].field_A4 < 1) {
        VectorSignedToFloat(this->field_40,(byte)(uVar23 >> 0x16) & 3);
      }
      else {
        VectorSignedToFloat(this->field_44,(byte)(uVar23 >> 0x16) & 3);
      }
      TargetFollowCamera::update(this[1].field_2C);
    }
  }
  if ((char)this[1].field_B4 != '\0') {
    this[1].field_B0 = this->field_40 + this[1].field_B0;
  }
  if (this->field_14 == 2) {
    TargetFollowCamera::setFirstPersonMatrix((Matrix *)this[1].field_2C);
    if ((*(char *)((int)&this[2].field_14 + 1) == '\0') &&
       (iVar31 = TargetFollowCamera::hideShipForFirstPersonCam(), iVar34 = DAT_0018dd2c, iVar31 != 0
       )) {
      *(undefined1 *)((int)&this[2].field_14 + 1) = 1;
      FModSound::enableReverb(**(int **)(iVar34 + 0x18daf8));
    }
  }
  else if ((*(char *)((int)&this[2].field_14 + 1) != '\0') &&
          (iVar34 = TargetFollowCamera::hideShipForFirstPersonCam(), iVar34 == 0)) {
    FModSound::disableReverb();
    *(undefined1 *)((int)&this[2].field_14 + 1) = 0;
  }
  pPVar29 = this->m_pPlayerEgo;
  if ((char)this->field_18 == '\0') {
    TargetFollowCamera::hideShipForFirstPersonCam();
  }
  PlayerEgo::hideShipForFirstPersonCameraView(SUB41(pPVar29,0));
  if ((((char)this->field_60 == '\0') && (*(char *)((int)&this->field_5C + 3) == '\0')) &&
     (iVar34 = PlayerEgo::isDead(*ppPVar1), iVar34 == 0)) {
    if ((**(char **)(DAT_0018dd34 + 0x18db64) != '\0') &&
       ((int)(uint)((uint)this->field_7C[2] < 0x1389) <= this->field_7C[3])) {
      **(char **)(DAT_0018dd34 + 0x18db64) = '\0';
      *(undefined1 *)((int)&this[1].field_14 + 1) = 1;
      startChargingJumpDrive();
    }
    if (((((char)this->field_5C == '\0') && ((char)this[1].field_48 == '\0')) &&
        (iVar34 = Hud::firePressed((Hud *)this->m_pLevel), iVar34 == 0)) ||
       (iVar34 = PlayerEgo::isDockingToPlanet(), iVar34 != 0)) {
      if (*(char *)((int)&this[1].field_94 + 1) != '\0') {
        pPVar29 = this->m_pPlayerEgo;
        *(undefined1 *)((int)&this[1].field_94 + 1) = 0;
        PlayerEgo::stopShooting((int)pPVar29);
      }
    }
    else {
      PlayerEgo::shoot((int)this->m_pPlayerEgo,this->field_40);
      *(undefined1 *)((int)&this[1].field_94 + 1) = 1;
    }
    iVar34 = Status::getCurrentCampaignMission();
    if (iVar34 != 0x30) {
      if (*(char *)(iVar5 + 0x11) == '\0') {
        handleAccelerometer(this);
      }
      else {
        fVar41 = (float)Hud::getAnalogX();
        if ((int)((uint)(fVar41 < 0.0) << 0x1f) < 0) {
          pPVar29 = this->m_pPlayerEgo;
          fVar41 = (float)Hud::getAnalogX();
          fVar40 = (float)Hud::getAnalogX();
          PlayerEgo::left((int)pPVar29,fVar41 * fVar40);
        }
        else {
          fVar41 = (float)Hud::getAnalogX();
          if (0.0 < fVar41) {
            pPVar29 = this->m_pPlayerEgo;
            fVar41 = (float)Hud::getAnalogX();
            fVar40 = (float)Hud::getAnalogX();
            PlayerEgo::right((int)pPVar29,fVar41 * fVar40);
          }
        }
        cVar20 = *(char *)(iVar5 + 0x10);
        fVar41 = (float)Hud::getAnalogY();
        iVar5 = (uint)(fVar41 < 0.0) << 0x1f;
        if (cVar20 == '\0') {
          if (iVar5 < 0) {
LAB_0018de66:
            pPVar29 = this->m_pPlayerEgo;
            fVar41 = (float)Hud::getAnalogY();
            fVar40 = (float)Hud::getAnalogY();
            PlayerEgo::up((int)pPVar29,fVar41 * fVar40);
          }
          else {
            fVar41 = (float)Hud::getAnalogY();
            if (0.0 < fVar41) goto LAB_0018debc;
          }
        }
        else if (iVar5 < 0) {
LAB_0018debc:
          pPVar29 = this->m_pPlayerEgo;
          fVar41 = (float)Hud::getAnalogY();
          fVar40 = (float)Hud::getAnalogY();
          PlayerEgo::down((int)pPVar29,fVar41 * fVar40);
        }
        else {
          fVar41 = (float)Hud::getAnalogY();
          if (0.0 < fVar41) goto LAB_0018de66;
        }
      }
    }
  }
  *(undefined1 *)&this[1].field_48 = 0;
  iVar5 = PlayerEgo::isInWormhole();
  if (iVar5 == 0) goto LAB_0018f164;
  piVar36 = *(int **)(DAT_0018e064 + 0x18df14);
  pMVar17 = (Mission *)Status::getMission();
  iVar5 = Mission::isCampaignMission(pMVar17);
  if (iVar5 != 0) {
    iVar5 = Status::getCurrentCampaignMission();
    if (iVar5 == 0x1d) {
LAB_0018df42:
      Player::setHitpoints((*ppPVar1)->m_pPlayer);
      goto LAB_0018f164;
    }
    if (iVar5 != 0x2a) {
      if (iVar5 == 0x29) goto LAB_0018df42;
      if (iVar5 == 0x28) {
        iVar5 = LevelScript::getEvent();
        if (iVar5 < 4) goto LAB_0018df42;
        Status::nextCampaignMission(SUB41(*piVar36,0));
        Level::getEnemies();
        uVar10 = Player::getHitpoints();
        **(undefined4 **)(DAT_0018e068 + 0x18df7c) = uVar10;
      }
      else {
        Status::nextCampaignMission(SUB41(*piVar36,0));
      }
    }
    Level::removeObjectives((Level *)this->field_78);
    Status::setMission((Mission *)*piVar36);
  }
  Status::setMission((Mission *)*piVar36);
  iVar5 = Status::inAlienOrbit();
  if (iVar5 == 0) {
    Status::setStation((Station *)*piVar36);
    pSVar30 = (Station *)*piVar36;
  }
  else {
    iVar5 = Status::getCurrentCampaignMission();
    if (iVar5 == 0x2a) goto LAB_0018f164;
    pSVar30 = (Station *)*piVar36;
    Galaxy::getStation(**(int **)(DAT_0018e074 + 0x18dfc0));
  }
  Status::departStation(pSVar30);
  uVar10 = Player::getHitpoints();
  *(undefined4 *)(*piVar36 + 100) = uVar10;
  uVar10 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
  *(undefined4 *)(*piVar36 + 0x5c) = uVar10;
  uVar10 = Player::getArmorHP();
  *(undefined4 *)(*piVar36 + 0x60) = uVar10;
  uVar10 = Player::getGammaHP();
  *(undefined4 *)(*piVar36 + 0x68) = uVar10;
  uVar10 = PlayerEgo::getCurrentSecondaryWeaponIndex();
  puVar14 = *(undefined1 **)(DAT_0018e078 + 0x18e028);
  puVar22 = *(undefined4 **)(DAT_0018e07c + 0x18e02c);
  puVar32 = *(undefined1 **)(DAT_0018e084 + 0x18e032);
  puVar24 = *(undefined4 **)(DAT_0018e080 + 0x18e02e);
  *(undefined4 *)(*piVar36 + 0xf4) = uVar10;
  *puVar14 = 1;
  *puVar32 = 1;
  *puVar22 = 0;
  *puVar24 = 1;
  AbyssEngine::ApplicationManager::SetCurrentApplicationModule((uint)this->field_8);
  *(undefined1 *)&this->field_54 = 0;
LAB_0018f164:
  if (*piVar4 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0018c8d8: push {r4,r5,r6,r7,lr}
  0018c8da: add r7,sp,#0xc
  0018c8dc: push {r7,r8,r9,r10,r11}
  0018c8e0: vpush {d8,d9}
  0018c8e4: sub sp,#0x128
  0018c8e6: mov r11,r0
  0018c8e8: ldr r0,[0x0018cb84]
  0018c8ea: add r0,pc
  0018c8ec: ldr r0,[r0,#0x0]
  0018c8ee: str r0,[sp,#0x2c]
  0018c8f0: ldr r0,[r0,#0x0]
  0018c8f2: str r0,[sp,#0x124]
  0018c8f4: ldr.w r0,[r11,#0x8]
  0018c8f8: blx 0x00070e4c
  0018c8fc: cmp r0,#0x96
  0018c8fe: bgt 0x0018c90c
  0018c900: ldr.w r0,[r11,#0x8]
  0018c904: blx 0x00070e4c
  0018c908: cmp r0,#0x0
  0018c90a: blt 0x0018c926
  0018c90c: ldr.w r0,[r11,#0x8]
  0018c910: blx 0x00070e4c
  0018c914: cmp r0,#0x96
  0018c916: ble 0x0018c91c
  0018c918: movs r0,#0x96
  0018c91a: b 0x0018c928
  0018c91c: ldr.w r0,[r11,#0x8]
  0018c920: blx 0x00070e4c
  0018c924: b 0x0018c928
  0018c926: movs r0,#0x0
  0018c928: vmov.32 d16[0x0],r0
  0018c92c: asrs r1,r0,#0x1f
  0018c92e: ldrb.w r2,[r11,#0x15e]
  0018c932: vmov.32 d16[0x1],r1
  0018c936: add.w r1,r11,#0x30
  0018c93a: cmp r2,#0x0
  0018c93c: vmov d17,d16
  0018c940: vld1.64 {d18,d19},[r1]
  0018c944: vadd.i64 q8,q9,q8
  0018c948: str.w r0,[r11,#0x40]
  0018c94c: vst1.64 {d16,d17},[r1]
  0018c950: beq 0x0018ca26
  0018c952: ldr r0,[0x0018cb88]
  0018c954: add r0,pc
  0018c956: blx 0x0006e878
  0018c95a: cbz r0,0x0018c9cc
  0018c95c: mov r0,r11
  0018c95e: mov.w r1,#0x190
  0018c962: mov.w r2,#0x190
  0018c966: movw r3,#0x3039
  0018c96a: blx 0x000781fc
  0018c96e: mov r0,r11
  0018c970: mov.w r1,#0x18c
  0018c974: mov.w r2,#0x190
  0018c978: movw r3,#0x3039
  0018c97c: blx 0x00078238
  0018c980: mov r0,r11
  0018c982: movw r3,#0x3039
  0018c986: blx 0x0007816c
  0018c98a: ldrb.w r0,[r11,#0x15e]
  0018c98e: cmp r0,#0x0
  0018c990: beq 0x0018ca26
  0018c992: ldr r0,[0x0018cb8c]
  0018c994: add r0,pc
  0018c996: blx 0x0006e878
  0018c99a: cbz r0,0x0018c9f8
  0018c99c: mov r0,r11
  0018c99e: mov.w r1,#0x190
  0018c9a2: mov.w r2,#0x190
  0018c9a6: movw r3,#0x3039
  0018c9aa: blx 0x000781fc
  0018c9ae: mov r0,r11
  0018c9b0: mov.w r1,#0x190
  0018c9b4: mov.w r2,#0x194
  0018c9b8: movw r3,#0x3039
  0018c9bc: blx 0x00078238
  0018c9c0: mov r0,r11
  0018c9c2: movw r3,#0x3039
  0018c9c6: blx 0x0007816c
  0018c9ca: b 0x0018ca26
  0018c9cc: ldrb.w r0,[r11,#0x15e]
  0018c9d0: cbz r0,0x0018ca26
  0018c9d2: ldr r0,[0x0018cb90]
  0018c9d4: add r0,pc
  0018c9d6: blx 0x0006e878
  0018c9da: cmp r0,#0x0
  0018c9dc: beq 0x0018c98a
  0018c9de: mov r0,r11
  0018c9e0: mov.w r1,#0x190
  0018c9e4: mov.w r2,#0x190
  0018c9e8: movw r3,#0x3039
  0018c9ec: blx 0x000781fc
  0018c9f0: mov r0,r11
  0018c9f2: mov.w r1,#0x194
  0018c9f6: b 0x0018c974
  0018c9f8: ldrb.w r0,[r11,#0x15e]
  0018c9fc: cbz r0,0x0018ca26
  0018c9fe: ldr r0,[0x0018cb94]
  0018ca00: add r0,pc
  0018ca02: blx 0x0006e878
  0018ca06: cbz r0,0x0018ca26
  0018ca08: mov r0,r11
  0018ca0a: mov.w r1,#0x190
  0018ca0e: mov.w r2,#0x190
  0018ca12: movw r3,#0x3039
  0018ca16: blx 0x000781fc
  0018ca1a: mov r0,r11
  0018ca1c: mov.w r1,#0x190
  0018ca20: mov.w r2,#0x18c
  0018ca24: b 0x0018c9b8
  0018ca26: ldr r0,[0x0018cb98]
  0018ca28: ldr r1,[0x0018cb9c]
  0018ca2a: add r0,pc
  0018ca2c: add r1,pc
  0018ca2e: ldr.w r8,[r0,#0x0]
  0018ca32: ldr.w r9,[r1,#0x0]
  0018ca36: ldr.w r0,[r11,#0x14]
  0018ca3a: ldr.w r4,[r8,#0x0]
  0018ca3e: ldr.w r6,[r9,#0x0]
  0018ca42: cmp r0,#0x3
  0018ca44: bne 0x0018cab0
  0018ca46: ldr r0,[0x0018cba0]
  0018ca48: add r0,pc
  0018ca4a: ldr r0,[r0,#0x0]
  0018ca4c: ldr r0,[r0,#0x0]
  0018ca4e: cbz r0,0x0018cab0
  0018ca50: ldr r0,[0x0018cba4]
  0018ca52: add r0,pc
  0018ca54: ldr r0,[r0,#0x0]
  0018ca56: ldr r0,[r0,#0x0]
  0018ca58: cbnz r0,0x0018cab0
  0018ca5a: orrs.w r0,r6,r4
  0018ca5e: beq 0x0018cab0
  0018ca60: mov r0,r11
  0018ca62: mov.w r1,#0x190
  0018ca66: mov.w r2,#0x190
  0018ca6a: movw r3,#0x3039
  0018ca6e: ldrd r5,r10,[r11,#0x1a8]
  0018ca72: blx 0x000781fc
  0018ca76: subs r0,r4,r5
  0018ca78: mov.w r2,#0x190
  0018ca7c: movw r3,#0x3039
  0018ca80: add.w r0,r0,r0, lsr #0x1f
  0018ca84: add.w r1,r2,r0, asr #0x1
  0018ca88: sub.w r0,r6,r10
  0018ca8c: add.w r0,r0,r0, lsr #0x1f
  0018ca90: add.w r2,r2,r0, asr #0x1
  0018ca94: mov r0,r11
  0018ca96: blx 0x00078238
  0018ca9a: mov r0,r11
  0018ca9c: movw r3,#0x3039
  0018caa0: blx 0x0007816c
  0018caa4: movs r6,#0x0
  0018caa6: movs r4,#0x0
  0018caa8: str.w r6,[r9,#0x0]
  0018caac: str.w r6,[r8,#0x0]
  0018cab0: mov r10,r11
  0018cab2: ldr.w r0,[r10,#0x58]!
  0018cab6: strd r4,r6,[r10,#0x150]
  0018caba: blx 0x00076804
  0018cabe: cbz r0,0x0018caca
  0018cac0: movs r0,#0x0
  0018cac2: str.w r0,[r9,#0x0]
  0018cac6: str.w r0,[r8,#0x0]
  0018caca: ldr r0,[0x0018cba8]
  0018cacc: add r0,pc
  0018cace: ldr r4,[r0,#0x0]
  0018cad0: ldrb r0,[r4,#0x0]
  0018cad2: cbz r0,0x0018cb32
  0018cad4: ldr.w r0,[r10,#0x0]
  0018cad8: blx 0x00077adc
  0018cadc: cbnz r0,0x0018cb2e
  0018cade: ldr r0,[0x0018cbac]
  0018cae0: add r0,pc
  0018cae2: ldr r0,[r0,#0x0]
  0018cae4: ldr r0,[r0,#0x0]
  0018cae6: blx 0x000741dc
  0018caea: cbz r0,0x0018cb2e
  0018caec: ldrb.w r0,[r11,#0xd6]
  0018caf0: cbz r0,0x0018cb0c
  0018caf2: ldr.w r0,[r11,#0x74]
  0018caf6: movs r5,#0x0
  0018caf8: strb.w r5,[r11,#0xd6]
  0018cafc: blx 0x00077a04
  0018cb00: mov r0,r11
  0018cb02: strb.w r5,[r11,#0x5d]
  0018cb06: blx 0x0007828c
  0018cb0a: b 0x0018cb2e
  0018cb0c: ldrb.w r0,[r11,#0x5d]
  0018cb10: movs r1,#0x1
  0018cb12: strb.w r1,[r11,#0xd6]
  0018cb16: eor r0,r0,#0x1
  0018cb1a: strb.w r0,[r11,#0x5d]
  0018cb1e: mov r0,r11
  0018cb20: blx 0x0007822c
  0018cb24: ldrd r0,r2,[r11,#0x74]
  0018cb28: movs r1,#0x2
  0018cb2a: blx 0x0007837c
  0018cb2e: movs r0,#0x0
  0018cb30: strb r0,[r4,#0x0]
  0018cb32: ldrb.w r1,[r11,#0x5d]
  0018cb36: ldrb.w r0,[r11,#0x1a6]
  0018cb3a: cmp r1,r0
  0018cb3c: beq 0x0018cbde
  0018cb3e: ldr.w r0,[r10,#0x0]
  0018cb42: cbz r1,0x0018cb6e
  0018cb44: blx 0x00078190
  0018cb48: ldr.w r0,[r11,#0x78]
  0018cb4c: blx 0x00071eb4
  0018cb50: mov r4,r0
  0018cb52: cmp r0,#0x0
  0018cb54: beq 0x0018cbd6
  0018cb56: movs r5,#0x0
  0018cb58: b 0x0018cb66
  0018cb5a: ldr r0,[r4,#0x4]
  0018cb5c: ldr.w r0,[r0,r5,lsl #0x2]
  0018cb60: blx 0x0007819c
  0018cb64: adds r5,#0x1
  0018cb66: ldr r0,[r4,#0x0]
  0018cb68: cmp r5,r0
  0018cb6a: bcc 0x0018cb5a
  0018cb6c: b 0x0018cbd6
  0018cb6e: blx 0x000781a8
  0018cb72: ldr.w r0,[r11,#0x78]
  0018cb76: blx 0x00071eb4
  0018cb7a: mov r4,r0
  0018cb7c: cbz r0,0x0018cbd6
  0018cb7e: movs r5,#0x0
  0018cb80: b 0x0018cbd0
  0018cbc4: ldr r0,[r4,#0x4]
  0018cbc6: ldr.w r0,[r0,r5,lsl #0x2]
  0018cbca: blx 0x000781b4
  0018cbce: adds r5,#0x1
  0018cbd0: ldr r0,[r4,#0x0]
  0018cbd2: cmp r5,r0
  0018cbd4: bcc 0x0018cbc4
  0018cbd6: ldrb.w r1,[r11,#0x5d]
  0018cbda: strb.w r1,[r11,#0x1a6]
  0018cbde: cbz r1,0x0018cc00
  0018cbe0: ldrb.w r0,[r11,#0x15e]
  0018cbe4: cmp r0,#0x0
  0018cbe6: beq 0x0018cca4
  0018cbe8: ldr.w r0,[r11,#0x88]
  0018cbec: blx 0x000781e4
  0018cbf0: cmp r0,#0x0
  0018cbf2: bne 0x0018cca4
  0018cbf4: ldr.w r0,[r11,#0x88]
  0018cbf8: blx 0x000781f0
  0018cbfc: cmp r0,#0x0
  0018cbfe: bne 0x0018cca4
  0018cc00: ldr.w r0,[r11,#0x14]
  0018cc04: cmp r0,#0x3
  0018cc06: bne 0x0018cca4
  0018cc08: ldrb.w r0,[r11,#0x15e]
  0018cc0c: cbz r0,0x0018cc20
  0018cc0e: ldr.w r0,[r11,#0x78]
  0018cc12: movs r2,#0x0
  0018cc14: ldrb.w r1,[r11,#0x5f]
  0018cc18: movs r3,#0x0
  0018cc1a: str r1,[sp,#0x0]
  0018cc1c: blx 0x00072220
  0018cc20: ldr.w r0,[r11,#0x8]
  0018cc24: blx 0x0007258c
  0018cc28: vldr.32 s2,[r0,#0x360]
  0018cc2c: vldr.32 s4,[pc,#-0x80]
  0018cc30: vcmpe.f32 s2,#0
  0018cc34: vmrs apsr,fpscr
  0018cc38: vneg.f32 s0,s2
  0018cc3c: it gt
  0018cc3e: vmov.gt.f32 s0,s2
  0018cc42: vcmpe.f32 s0,s4
  0018cc46: vmrs apsr,fpscr
  0018cc4a: ble 0x0018cc8c
  0018cc4c: vldr.32 s4,[pc,#-0x9c]
  0018cc50: vldr.32 s0,[r11,#0xbc]
  0018cc54: vmla.f32 s0,s2,s4
  0018cc58: vldr.32 s2,[pc,#-0xa4]
  0018cc5c: vcmpe.f32 s0,s2
  0018cc60: vstr.32 s0,[r11,#0xbc]
  0018cc64: vmrs apsr,fpscr
  0018cc68: bgt 0x0018cc78
  0018cc6a: vldr.32 s2,[pc,#-0xb0]
  0018cc6e: vcmpe.f32 s0,s2
  0018cc72: vmrs apsr,fpscr
  0018cc76: bpl 0x0018cc80
  0018cc78: vmov.f32 s0,s2
  0018cc7c: vstr.32 s2,[r11,#0xbc]
  0018cc80: vmov r1,s0
  0018cc84: ldr.w r0,[r11,#0xf4]
  0018cc88: blx 0x00078178
  0018cc8c: ldr.w r0,[r11,#0x8]
  0018cc90: blx 0x0007258c
  0018cc94: vldr.32 s0,[pc,#-0xd8]
  0018cc98: vldr.32 s2,[r0,#0x360]
  0018cc9c: vmul.f32 s0,s2,s0
  0018cca0: vstr.32 s0,[r0,#0x360]
  0018cca4: ldrb.w r0,[r11,#0x5d]
  0018cca8: cmp r0,#0x0
  0018ccaa: bne.w 0x0018d25c
  0018ccae: ldr.w r1,[r11,#0x16c]
  0018ccb2: cmp r1,#0x0
  0018ccb4: beq 0x0018cd74
  0018ccb6: ldr.w r0,[r11,#0x40]
  0018ccba: blt 0x0018cd36
  0018ccbc: cmp r1,r0
  0018ccbe: bgt 0x0018ccf8
  0018ccc0: ldr r0,[0x0018cd60]
  0018ccc2: movs r1,#0x0
  0018ccc4: movs r5,#0x0
  0018ccc6: add r0,pc
  0018ccc8: ldr r4,[r0,#0x0]
  0018ccca: ldr r0,[r4,#0x0]
  0018cccc: blx 0x00074f74
  0018ccd0: ldr.w r0,[r11,#0x74]
  0018ccd4: movs r1,#0x1
  0018ccd6: movs r2,#0x0
  0018ccd8: movs r3,#0x0
  0018ccda: str r5,[sp,#0x0]
  0018ccdc: blx 0x000780d0
  0018cce0: ldr r0,[r4,#0x0]
  0018cce2: movw r1,#0x45f
  0018cce6: movs r2,#0x0
  0018cce8: movs r3,#0x0
  0018ccea: str r5,[sp,#0x0]
  0018ccec: blx 0x00071548
  0018ccf0: ldr.w r0,[r11,#0x40]
  0018ccf4: ldr.w r1,[r11,#0x16c]
  0018ccf8: subs r1,r1,r0
  0018ccfa: str.w r1,[r11,#0x16c]
  0018ccfe: beq 0x0018cd6c
  0018cd00: cmp r1,#0x1
  0018cd02: blt 0x0018cd74
  0018cd04: vmov s0,r0
  0018cd08: vldr.32 s2,[pc,#0x58]
  0018cd0c: vldr.32 s4,[pc,#0x58]
  0018cd10: movs r1,#0x1
  0018cd12: vcvt.f32.s32 s0,s0
  0018cd16: movs r2,#0x0
  0018cd18: vmul.f32 s2,s0,s2
  0018cd1c: vmul.f32 s0,s0,s4
  0018cd20: vcvt.s32.f32 s2,s2
  0018cd24: vcvt.s32.f32 s0,s0
  0018cd28: ldr.w r0,[r11,#0x74]
  0018cd2c: vstr.32 s2,[r11,#0x40]
  0018cd30: vstr.32 s0,[r11,#0x44]
  0018cd34: b 0x0018cd52
  0018cd36: ldr.w r2,[r11,#0x170]
  0018cd3a: subs r0,r1,r0
  0018cd3c: str.w r0,[r11,#0x16c]
  0018cd40: rsbs r1,r2
  0018cd42: cmp r0,r1
  0018cd44: bge 0x0018cd74
  0018cd46: ldr.w r0,[r11,#0x74]
  0018cd4a: movs r1,#0x0
  0018cd4c: str.w r1,[r11,#0x16c]
  0018cd50: movs r2,#0x1
  0018cd52: str r1,[sp,#0x0]
  0018cd54: movs r1,#0x1
  0018cd56: movs r3,#0x1
  0018cd58: blx 0x000780d0
  0018cd5c: b 0x0018cd74
  0018cd6c: mov.w r0,#0xffffffff
  0018cd70: str.w r0,[r11,#0x16c]
  0018cd74: ldr r0,[0x0018d120]
  0018cd76: ldr.w r2,[r11,#0x40]
  0018cd7a: add r0,pc
  0018cd7c: ldr.w r8,[r0,#0x0]
  0018cd80: asrs r3,r2,#0x1f
  0018cd82: ldr.w r0,[r8,#0x0]
  0018cd86: blx 0x00075454
  0018cd8a: ldr.w r0,[r11,#0x80]
  0018cd8e: ldrb.w r0,[r0,#0x54]
  0018cd92: cbnz r0,0x0018cdac
  0018cd94: ldr.w r0,[r10,#0x0]
  0018cd98: blx 0x00077b60
  0018cd9c: cbnz r0,0x0018cdac
  0018cd9e: ldr.w r0,[r11,#0x84]
  0018cda2: blx 0x00077b54
  0018cda6: cmp r0,#0x0
  0018cda8: beq.w 0x0018d9e8
  0018cdac: ldr.w r0,[r11,#0xf4]
  0018cdb0: mov.w r1,#0x3f800000
  0018cdb4: str.w r1,[r11,#0x168]
  0018cdb8: movs r1,#0x0
  0018cdba: blx 0x000781d8
  0018cdbe: ldr.w r0,[r11,#0x58]
  0018cdc2: movs r1,#0x1
  0018cdc4: strb.w r1,[r0,#0x84]
  0018cdc8: ldr.w r0,[r11,#0x40]
  0018cdcc: ldr.w r2,[r11,#0x7c]
  0018cdd0: ldrd r3,r5,[r2,#0x8]
  0018cdd4: adds r1,r3,r0
  0018cdd6: adc.w r0,r5,r0, asr #0x1f
  0018cdda: strd r1,r0,[r2,#0x8]
  0018cdde: movw r2,#0x1388
  0018cde2: cmp r3,r2
  0018cde4: bgt 0x0018ce0c
  0018cde6: movw r2,#0x1389
  0018cdea: subs r1,r1,r2
  0018cdec: sbcs r0,r0,#0x0
  0018cdf0: blt 0x0018ce0c
  0018cdf2: ldr.w r0,[r10,#0x0]
  0018cdf6: blx 0x00077b24
  0018cdfa: cbz r0,0x0018ce0c
  0018cdfc: ldr.w r2,[r11,#0x58]
  0018ce00: movs r1,#0x2d
  0018ce02: ldr.w r0,[r11,#0x74]
  0018ce06: movs r3,#0x0
  0018ce08: blx 0x00072418
  0018ce0c: ldrb.w r0,[r11,#0x5f]
  0018ce10: cmp r0,#0x0
  0018ce12: bne.w 0x0018cf6c
  0018ce16: ldr.w r1,[r11,#0x40]
  0018ce1a: ldrd r2,r3,[r11,#0x48]
  0018ce1e: ldr.w r0,[r11,#0x14]
  0018ce22: adds r2,r2,r1
  0018ce24: adc.w r1,r3,r1, asr #0x1f
  0018ce28: cmp r0,#0x3
  0018ce2a: strd r2,r1,[r11,#0x48]
  0018ce2e: beq 0x0018cf06
  0018ce30: ldr.w r0,[r10,#0x0]
  0018ce34: blx 0x00076804
  0018ce38: cmp r0,#0x0
  0018ce3a: bne 0x0018cf06
  0018ce3c: ldr.w r0,[r10,#0x0]
  0018ce40: blx 0x00077bc0
  0018ce44: cmp r0,#0x0
  0018ce46: bne 0x0018cf06
  0018ce48: ldr.w r0,[r11,#0x8]
  0018ce4c: blx 0x0007258c
  0018ce50: vmov.f32 s16,0x3f800000
  0018ce54: vldr.32 s0,[r0,#0x360]
  0018ce58: vcmpe.f32 s0,s16
  0018ce5c: vmrs apsr,fpscr
  0018ce60: bge 0x0018ce70
  0018ce62: vmov.f32 s2,0xbf800000
  0018ce66: vcmpe.f32 s0,s2
  0018ce6a: vmrs apsr,fpscr
  0018ce6e: bhi 0x0018cf06
  0018ce70: vldr.32 s2,[r11,#0x1b4]
  0018ce74: ldr.w r0,[r11,#0x8]
  0018ce78: vadd.f32 s0,s0,s2
  0018ce7c: vstr.32 s0,[r11,#0x1b4]
  0018ce80: blx 0x0007258c
  0018ce84: movs r1,#0x0
  0018ce86: vldr.32 s18,[pc,#0x29c]
  0018ce8a: str.w r1,[r0,#0x360]
  0018ce8e: vldr.32 s0,[r11,#0x1b4]
  0018ce92: vmov.f32 s2,s18
  0018ce96: vldr.32 s4,[pc,#0x290]
  0018ce9a: vmov.f32 s6,s18
  0018ce9e: vcmpe.f32 s0,s18
  0018cea2: vmrs apsr,fpscr
  0018cea6: it mi
  0018cea8: vmov.mi.f32 s2,s0
  0018ceac: vcmpe.f32 s2,#0
  0018ceb0: vmrs apsr,fpscr
  0018ceb4: vcmpe.f32 s0,s18
  0018ceb8: it mi
  0018ceba: vmov.mi.f32 s6,s4
  0018cebe: vmrs apsr,fpscr
  0018cec2: vmov.f32 s4,s6
  0018cec6: vcmpe.f32 s2,#0
  0018ceca: it mi
  0018cecc: vmov.mi.f32 s4,s0
  0018ced0: vmrs apsr,fpscr
  0018ced4: it mi
  0018ced6: vmov.mi.f32 s4,s6
  0018ceda: ldr.w r0,[r11,#0x58]
  0018cede: vdiv.f32 s0,s4,s18
  0018cee2: vmov r1,s0
  0018cee6: vstr.32 s4,[r11,#0x1b4]
  0018ceea: blx 0x00078268
  0018ceee: vldr.32 s0,[r11,#0x1b4]
  0018cef2: ldr.w r0,[r11,#0x58]
  0018cef6: vdiv.f32 s0,s0,s18
  0018cefa: vsub.f32 s0,s16,s0
  0018cefe: vstr.32 s0,[r11,#0x1b0]
  0018cf02: blx 0x00078274
  0018cf06: ldr.w r0,[r10,#0x0]
  0018cf0a: blx 0x00076804
  0018cf0e: cbnz r0,0x0018cf1a
  0018cf10: ldr.w r0,[r10,#0x0]
  0018cf14: blx 0x00077bc0
  0018cf18: cbz r0,0x0018cf54
  0018cf1a: ldr.w r0,[r10,#0x0]
  0018cf1e: blx 0x0007825c
  0018cf22: vmov.f32 s0,0x3f800000
  0018cf26: vmov s2,r0
  0018cf2a: vcmpe.f32 s2,s0
  0018cf2e: vmrs apsr,fpscr
  0018cf32: bpl 0x0018cf54
  0018cf34: ldr.w r0,[r11,#0x58]
  0018cf38: ldr r1,[0x0018d124]
  0018cf3a: str.w r1,[r11,#0x1b4]
  0018cf3e: mov.w r1,#0x3f800000
  0018cf42: blx 0x00078268
  0018cf46: ldr.w r0,[r11,#0x8]
  0018cf4a: blx 0x0007258c
  0018cf4e: movs r1,#0x0
  0018cf50: str.w r1,[r0,#0x360]
  0018cf54: ldr.w r0,[r10,#0x0]
  0018cf58: blx 0x00077bc0
  0018cf5c: cbz r0,0x0018cf6c
  0018cf5e: ldr.w r0,[r11,#0x14]
  0018cf62: cbz r0,0x0018cf6c
  0018cf64: mov r0,r11
  0018cf66: movs r1,#0x0
  0018cf68: blx 0x00078388
  0018cf6c: ldr r0,[0x0018d12c]
  0018cf6e: ldr.w r1,[r11,#0x40]
  0018cf72: add r0,pc
  0018cf74: ldr r0,[r0,#0x0]
  0018cf76: ldr r0,[r0,#0x0]
  0018cf78: blx 0x00075448
  0018cf7c: ldr r0,[0x0018d130]
  0018cf7e: ldr.w r1,[r8,#0x0]
  0018cf82: add r0,pc
  0018cf84: ldr.w r2,[r11,#0x40]
  0018cf88: ldr r3,[r1,#0x30]
  0018cf8a: ldr r0,[r0,#0x0]
  0018cf8c: subs r2,r3,r2
  0018cf8e: str r2,[r1,#0x30]
  0018cf90: ldrb r0,[r0,#0xf]
  0018cf92: cbnz r0,0x0018cfaa
  0018cf94: ldr r0,[0x0018d134]
  0018cf96: movs r1,#0x0
  0018cf98: str r1,[sp,#0x0]
  0018cf9a: movs r1,#0x0
  0018cf9c: add r0,pc
  0018cf9e: movs r2,#0x0
  0018cfa0: movs r3,#0x0
  0018cfa2: ldr r0,[r0,#0x0]
  0018cfa4: ldr r0,[r0,#0x0]
  0018cfa6: blx 0x0007543c
  0018cfaa: ldr.w r0,[r11,#0x58]
  0018cfae: blx 0x00078490
  0018cfb2: mov r9,r0
  0018cfb4: ldr.w r0,[r11,#0x50]
  0018cfb8: cmp.w r0,#0xfa0
  0018cfbc: bge 0x0018cff8
  0018cfbe: ldr.w r3,[r11,#0x74]
  0018cfc2: movs r1,#0x40
  0018cfc4: ldrd r12,r2,[r11,#0x7c]
  0018cfc8: ldr.w r0,[r11,#0x16c]
  0018cfcc: ldr.w lr,[r11,#0x84]
  0018cfd0: ldr.w r5,[r11,#0xd8]
  0018cfd4: cmp r0,#0x0
  0018cfd6: it gt
  0018cfd8: mov.gt r1,#0x44
  0018cfda: ldrb.w r4,[r11,#0x5f]
  0018cfde: ldr.w r1,[r11,r1]
  0018cfe2: ldr.w r0,[r11,#0x58]
  0018cfe6: ldr.w r6,[r11,#0x14]
  0018cfea: strd lr,r12,[sp,#0x0]
  0018cfee: strd r5,r4,[sp,#0x8]
  0018cff2: str r6,[sp,#0x10]
  0018cff4: blx 0x0007849c
  0018cff8: ldr.w r0,[r11,#0x58]
  0018cffc: blx 0x00075880
  0018d000: ldr.w r2,[r11,#0x40]
  0018d004: mov r4,r0
  0018d006: ldr.w r0,[r11,#0x78]
  0018d00a: ldrb.w r1,[r11,#0x5f]
  0018d00e: asrs r3,r2,#0x1f
  0018d010: str r1,[sp,#0x0]
  0018d012: blx 0x00072220
  0018d016: cbz r4,0x0018d02a
  0018d018: ldr.w r0,[r10,#0x0]
  0018d01c: blx 0x00075880
  0018d020: cbnz r0,0x0018d02a
  0018d022: mov r0,r11
  0018d024: movs r1,#0x0
  0018d026: blx 0x00078388
  0018d02a: cmp.w r9,#0x0
  0018d02e: bne.w 0x0018d25c
  0018d032: ldr.w r0,[r10,#0x0]
  0018d036: blx 0x00078490
  0018d03a: cmp r0,#0x0
  0018d03c: beq.w 0x0018d25c
  0018d040: ldrb.w r0,[r11,#0xce]
  0018d044: cmp r0,#0x0
  0018d046: bne.w 0x0018d25c
  0018d04a: ldr.w r0,[r11,#0x94]
  0018d04e: cbnz r0,0x0018d060
  0018d050: movs r0,#0x5c
  0018d052: blx 0x0006eb24
  0018d056: mov r5,r0
  0018d058: blx 0x00074584
  0018d05c: str.w r5,[r11,#0x94]
  0018d060: ldr r0,[0x0018d138]
  0018d062: mov.w r1,#0x122
  0018d066: add r0,pc
  0018d068: ldr r0,[r0,#0x0]
  0018d06a: str r0,[sp,#0x28]
  0018d06c: ldr r0,[r0,#0x0]
  0018d06e: blx 0x00072f70
  0018d072: ldr r1,[0x0018d13c]
  0018d074: add r5,sp,#0x118
  0018d076: mov r4,r0
  0018d078: movs r2,#0x0
  0018d07a: add r1,pc
  0018d07c: mov r0,r5
  0018d07e: blx 0x0006ee18
  0018d082: add r0,sp,#0x78
  0018d084: mov r1,r4
  0018d086: mov r2,r5
  0018d088: blx 0x0006ef98
  0018d08c: add r0,sp,#0x118
  0018d08e: blx 0x0006ee30
  0018d092: ldr.w r0,[r8,#0x0]
  0018d096: blx 0x0007192c
  0018d09a: ldr.w r5,[r8,#0x0]
  0018d09e: mov r4,r0
  0018d0a0: add r0,sp,#0x10c
  0018d0a2: add r1,sp,#0x78
  0018d0a4: movs r2,#0x0
  0018d0a6: blx 0x0006f028
  0018d0aa: movs r1,#0x1e
  0018d0ac: cmp r4,#0x0
  0018d0ae: it ne
  0018d0b0: mov.ne r1,#0x64
  0018d0b2: add r0,sp,#0x100
  0018d0b4: blx 0x0006f658
  0018d0b8: ldr r1,[0x0018d140]
  0018d0ba: add r1,pc
  0018d0bc: add r0,sp,#0xf4
  0018d0be: movs r2,#0x0
  0018d0c0: blx 0x0006ee18
  0018d0c4: str r0,[sp,#0x0]
  0018d0c6: add r0,sp,#0x118
  0018d0c8: add r2,sp,#0x10c
  0018d0ca: add r3,sp,#0x100
  0018d0cc: mov r1,r5
  0018d0ce: blx 0x000739b4
  0018d0d2: add r0,sp,#0x78
  0018d0d4: add r1,sp,#0x118
  0018d0d6: str.w r10,[sp,#0x18]
  0018d0da: blx 0x0006f2b0
  0018d0de: ldr r0,[0x0018d144]
  0018d0e0: add r0,pc
  0018d0e2: ldr r4,[r0,#0x0]
  0018d0e4: add r0,sp,#0x118
  0018d0e6: blx r4
  0018d0e8: add r0,sp,#0xf4
  0018d0ea: blx r4
  0018d0ec: add r0,sp,#0x100
  0018d0ee: blx r4
  0018d0f0: add r0,sp,#0x10c
  0018d0f2: str r4,[sp,#0x20]
  0018d0f4: blx r4
  0018d0f6: ldr.w r0,[r8,#0x0]
  0018d0fa: blx 0x00071a58
  0018d0fe: blx 0x00073750
  0018d102: mov r5,r0
  0018d104: cmp r0,#0x0
  0018d106: beq 0x0018d1f6
  0018d108: ldr r0,[0x0018d148]
  0018d10a: add.w r10,sp,#0xd0
  0018d10e: mov.w r8,#0x0
  0018d112: movs r4,#0x0
  0018d114: add r0,pc
  0018d116: str r0,[sp,#0x24]
  0018d118: ldr r0,[0x0018d14c]
  0018d11a: add r0,pc
  0018d11c: str r0,[sp,#0x1c]
  0018d11e: b 0x0018d1ec
  0018d150: ldr r0,[r5,#0x4]
  0018d152: mov r9,r11
  0018d154: ldr.w r11,[r0,r8,lsl #0x2]
  0018d158: cmp.w r11,#0x0
  0018d15c: beq 0x0018d1e6
  0018d15e: mov r0,r11
  0018d160: blx 0x00071908
  0018d164: cmp r0,#0x17
  0018d166: bne 0x0018d1e6
  0018d168: ldr r1,[sp,#0x24]
  0018d16a: mov r0,r10
  0018d16c: movs r2,#0x0
  0018d16e: blx 0x0006ee18
  0018d172: mov r0,r11
  0018d174: blx 0x0007183c
  0018d178: mov r1,r0
  0018d17a: add r4,sp,#0xc4
  0018d17c: mov r0,r4
  0018d17e: blx 0x0006f658
  0018d182: add r0,sp,#0xdc
  0018d184: mov r1,r10
  0018d186: mov r2,r4
  0018d188: blx 0x0006ef98
  0018d18c: add r4,sp,#0xb8
  0018d18e: ldr r1,[sp,#0x1c]
  0018d190: movs r2,#0x0
  0018d192: mov r0,r4
  0018d194: blx 0x0006ee18
  0018d198: add r0,sp,#0xe8
  0018d19a: add r1,sp,#0xdc
  0018d19c: mov r2,r4
  0018d19e: blx 0x0006ef98
  0018d1a2: ldr r0,[sp,#0x28]
  0018d1a4: ldr r4,[r0,#0x0]
  0018d1a6: mov r0,r11
  0018d1a8: blx 0x000718d8
  0018d1ac: add r6,sp,#0x118
  0018d1ae: addw r1,r0,#0x4fa
  0018d1b2: mov r0,r4
  0018d1b4: blx 0x00072f70
  0018d1b8: mov r2,r0
  0018d1ba: add r1,sp,#0xe8
  0018d1bc: mov r0,r6
  0018d1be: blx 0x0006ef98
  0018d1c2: add r0,sp,#0x78
  0018d1c4: mov r1,r6
  0018d1c6: blx 0x0006ef5c
  0018d1ca: ldr r4,[sp,#0x20]
  0018d1cc: mov r0,r6
  0018d1ce: blx r4
  0018d1d0: add r0,sp,#0xe8
  0018d1d2: blx r4
  0018d1d4: add r0,sp,#0xb8
  0018d1d6: blx r4
  0018d1d8: add r0,sp,#0xdc
  0018d1da: blx r4
  0018d1dc: add r0,sp,#0xc4
  0018d1de: blx r4
  0018d1e0: mov r0,r10
  0018d1e2: blx r4
  0018d1e4: movs r4,#0x1
  0018d1e6: add.w r8,r8,#0x1
  0018d1ea: mov r11,r9
  0018d1ec: ldr r0,[r5,#0x0]
  0018d1ee: cmp r8,r0
  0018d1f0: bcc 0x0018d150
  0018d1f2: lsls r0,r4,#0x1f
  0018d1f4: bne 0x0018d22c
  0018d1f6: ldr r1,[0x0018d5a8]
  0018d1f8: add r1,pc
  0018d1fa: add r0,sp,#0xe8
  0018d1fc: movs r2,#0x0
  0018d1fe: blx 0x0006ee18
  0018d202: ldr r0,[sp,#0x28]
  0018d204: ldr r0,[r0,#0x0]
  0018d206: mov.w r1,#0x11e
  0018d20a: blx 0x00072f70
  0018d20e: mov r2,r0
  0018d210: add r0,sp,#0x118
  0018d212: add r1,sp,#0xe8
  0018d214: blx 0x0006ef98
  0018d218: add r0,sp,#0x78
  0018d21a: add r1,sp,#0x118
  0018d21c: blx 0x0006ef5c
  0018d220: add r0,sp,#0x118
  0018d222: blx 0x0006ee30
  0018d226: add r0,sp,#0xe8
  0018d228: blx 0x0006ee30
  0018d22c: ldr.w r0,[r11,#0x94]
  0018d230: add r1,sp,#0x78
  0018d232: movs r2,#0x1
  0018d234: movs r4,#0x1
  0018d236: blx 0x000746f8
  0018d23a: strb.w r4,[r11,#0x5d]
  0018d23e: strb.w r4,[r11,#0x111]
  0018d242: mov r0,r11
  0018d244: blx 0x0007822c
  0018d248: movs r0,#0x1
  0018d24a: strb.w r0,[r11,#0xce]
  0018d24e: strb.w r0,[r11,#0xca]
  0018d252: add r0,sp,#0x78
  0018d254: blx 0x0006ee30
  0018d258: ldr.w r10,[sp,#0x18]
  0018d25c: ldr r0,[0x0018d5ac]
  0018d25e: add r0,pc
  0018d260: ldr r0,[r0,#0x0]
  0018d262: str r0,[sp,#0x28]
  0018d264: ldrb r0,[r0,#0xf]
  0018d266: cmp r0,#0x0
  0018d268: beq 0x0018d324
  0018d26a: ldr.w r4,[r11,#0x4]
  0018d26e: mov r0,r4
  0018d270: blx 0x000717f4
  0018d274: mov r1,r0
  0018d276: mov r0,r4
  0018d278: blx 0x0006ff1c
  0018d27c: add.w r8,sp,#0x78
  0018d280: mov r1,r0
  0018d282: movs r2,#0x3c
  0018d284: mov r0,r8
  0018d286: blx 0x0006f1e4
  0018d28a: ldr r0,[0x0018d5b0]
  0018d28c: add r4,sp,#0xd0
  0018d28e: ldr r1,[0x0018d5b4]
  0018d290: add r0,pc
  0018d292: add r1,pc
  0018d294: ldr r0,[r0,#0x0]
  0018d296: ldr.w r9,[r1,#0x0]
  0018d29a: mov r1,r8
  0018d29c: vldr.32 s0,[r0]
  0018d2a0: mov r0,r4
  0018d2a2: vcvt.f32.s32 s16,s0
  0018d2a6: blx r9
  0018d2a8: add.w r6,r11,#0x198
  0018d2ac: add r5,sp,#0xdc
  0018d2ae: mov r1,r4
  0018d2b0: mov r0,r5
  0018d2b2: mov r2,r6
  0018d2b4: blx 0x0006ec38
  0018d2b8: vmov r1,s16
  0018d2bc: add r4,sp,#0xe8
  0018d2be: mov r2,r5
  0018d2c0: mov r0,r4
  0018d2c2: blx 0x00072658
  0018d2c6: vldr.32 s0,[r11,#0x40]
  0018d2ca: add r0,sp,#0x118
  0018d2cc: mov r1,r4
  0018d2ce: vcvt.f32.s32 s0,s0
  0018d2d2: vmov r2,s0
  0018d2d6: blx 0x00073024
  0018d2da: add r4,sp,#0xdc
  0018d2dc: mov r1,r8
  0018d2de: mov r0,r4
  0018d2e0: blx 0x0006f4c0
  0018d2e4: add r0,sp,#0xe8
  0018d2e6: mov r1,r4
  0018d2e8: blx 0x0006f4cc
  0018d2ec: add r4,sp,#0xdc
  0018d2ee: mov r1,r8
  0018d2f0: mov r0,r4
  0018d2f2: blx r9
  0018d2f4: add r5,sp,#0xd0
  0018d2f6: mov r1,r8
  0018d2f8: mov r0,r5
  0018d2fa: blx 0x0006f4d8
  0018d2fe: ldr r0,[0x0018d5b8]
  0018d300: add r1,sp,#0x118
  0018d302: add r2,sp,#0xe8
  0018d304: str r1,[sp,#0x0]
  0018d306: add r0,pc
  0018d308: mov r1,r4
  0018d30a: mov r3,r5
  0018d30c: ldr r0,[r0,#0x0]
  0018d30e: ldr r0,[r0,#0x0]
  0018d310: blx 0x0007543c
  0018d314: add r4,sp,#0xc4
  0018d316: mov r1,r8
  0018d318: mov r0,r4
  0018d31a: blx r9
  0018d31c: mov r0,r6
  0018d31e: mov r1,r4
  0018d320: blx 0x0006eb3c
  0018d324: ldrb.w r0,[r11,#0x60]
  0018d328: cmp r0,#0x0
  0018d32a: beq.w 0x0018d472
  0018d32e: ldr.w r0,[r10,#0x0]
  0018d332: blx 0x00077b78
  0018d336: cmp r0,#0x0
  0018d338: beq.w 0x0018d63a
  0018d33c: ldr.w r0,[r10,#0x0]
  0018d340: blx 0x000767bc
  0018d344: cmp r0,#0x0
  0018d346: bne.w 0x0018d63a
  0018d34a: ldr r1,[0x0018d5bc]
  0018d34c: mov r9,r10
  0018d34e: ldr.w r0,[r11,#0x8]
  0018d352: add r1,pc
  0018d354: ldr r5,[r1,#0x0]
  0018d356: blx r5
  0018d358: ldr r1,[0x0018d5c0]
  0018d35a: movs r2,#0x1
  0018d35c: blx 0x000707f8
  0018d360: ldr.w r0,[r11,#0x58]
  0018d364: blx 0x000725d4
  0018d368: vmov s0,r0
  0018d36c: vldr.32 s16,[pc,#-0x248]
  0018d370: ldr.w r0,[r11,#0x8]
  0018d374: vcmpe.f32 s0,#0
  0018d378: vmrs apsr,fpscr
  0018d37c: vmov.f32 s18,s16
  0018d380: it gt
  0018d382: vmov.gt.f32 s18,s0
  0018d386: blx r5
  0018d388: ldr r1,[0x0018d5c4]
  0018d38a: add r6,sp,#0x118
  0018d38c: vstr.32 s18,[r0,#0x3c8]
  0018d390: movs r0,#0x0
  0018d392: add r1,pc
  0018d394: ldr.w r10,[r1,#0x0]
  0018d398: ldr.w r4,[r10,#0x0]
  0018d39c: strd r0,r0,[sp,#0x78]
  0018d3a0: str r0,[sp,#0x80]
  0018d3a2: mov r0,r6
  0018d3a4: ldr.w r1,[r11,#0x58]
  0018d3a8: blx 0x0007264c
  0018d3ac: add.w r8,sp,#0x78
  0018d3b0: mov r0,r4
  0018d3b2: mov r1,r6
  0018d3b4: mov r2,r8
  0018d3b6: blx 0x000729c4
  0018d3ba: ldr.w r0,[r11,#0x8]
  0018d3be: blx r5
  0018d3c0: blx 0x0006e8cc
  0018d3c4: mov r4,r0
  0018d3c6: ldr r0,[0x0018d5c8]
  0018d3c8: movs r1,#0x0
  0018d3ca: add r0,pc
  0018d3cc: ldr r6,[r0,#0x0]
  0018d3ce: mov r0,r8
  0018d3d0: blx r6
  0018d3d2: vmov s0,r4
  0018d3d6: movs r1,#0x1
  0018d3d8: vcvt.f32.s32 s0,s0
  0018d3dc: vldr.32 s2,[r0]
  0018d3e0: vdiv.f32 s0,s2,s0
  0018d3e4: vstr.32 s0,[r0]
  0018d3e8: mov r0,r8
  0018d3ea: blx r6
  0018d3ec: vldr.32 s18,[r0]
  0018d3f0: ldr.w r0,[r11,#0x8]
  0018d3f4: blx r5
  0018d3f6: blx 0x0006e8d8
  0018d3fa: vmov s0,r0
  0018d3fe: mov r0,r8
  0018d400: vmov.f32 s2,0x3f800000
  0018d404: movs r1,#0x1
  0018d406: vcvt.f32.s32 s0,s0
  0018d40a: vdiv.f32 s0,s18,s0
  0018d40e: vsub.f32 s18,s2,s0
  0018d412: blx r6
  0018d414: vstr.32 s18,[r0]
  0018d418: ldr.w r0,[r11,#0x8]
  0018d41c: blx r5
  0018d41e: add.w r0,r0,#0x3cc
  0018d422: mov r1,r8
  0018d424: blx 0x0006eb3c
  0018d428: ldr.w r0,[r11,#0x58]
  0018d42c: blx 0x000725d4
  0018d430: vmov s2,r0
  0018d434: vldr.32 s0,[pc,#0x194]
  0018d438: vldr.32 s18,[pc,#0x194]
  0018d43c: vmla.f32 s18,s2,s0
  0018d440: ldr r0,[0x0018d5d4]
  0018d442: ldr.w r5,[r11,#0xf0]
  0018d446: add r0,pc
  0018d448: ldr r6,[r0,#0x0]
  0018d44a: vstr.32 s18,[r11,#0x70]
  0018d44e: ldr r0,[r6,#0x0]
  0018d450: ldr.w r4,[r10,#0x0]
  0018d454: blx 0x000723d0
  0018d458: cmp r0,#0x0
  0018d45a: beq.w 0x0018d5e8
  0018d45e: ldr r0,[r6,#0x0]
  0018d460: blx 0x00071770
  0018d464: adr r1,[0x18d5d8]
  0018d466: cmp r0,#0x50
  0018d468: it lt
  0018d46a: add.lt r1,#0x4
  0018d46c: vldr.32 s0,[r1]
  0018d470: b 0x0018d5ec
  0018d472: ldr.w r0,[r10,#0x0]
  0018d476: blx 0x000724e4
  0018d47a: cmp r0,#0x1
  0018d47c: blt.w 0x0018d32e
  0018d480: ldrb.w r0,[r11,#0x5d]
  0018d484: cmp r0,#0x0
  0018d486: beq.w 0x0018d7d2
  0018d48a: ldr.w r0,[r11,#0x88]
  0018d48e: cmp r0,#0x0
  0018d490: beq.w 0x0018f0ea
  0018d494: ldrb.w r1,[r11,#0xd6]
  0018d498: cmp r1,#0x0
  0018d49a: bne.w 0x0018f0ea
  0018d49e: ldrb.w r1,[r11,#0x5e]
  0018d4a2: cmp r1,#0x0
  0018d4a4: bne.w 0x0018f0ea
  0018d4a8: ldrb.w r1,[r11,#0xc8]
  0018d4ac: cmp r1,#0x0
  0018d4ae: bne.w 0x0018f0ea
  0018d4b2: ldrb.w r1,[r11,#0xc5]
  0018d4b6: cmp r1,#0x0
  0018d4b8: bne.w 0x0018f0ea
  0018d4bc: ldrb.w r1,[r11,#0xc6]
  0018d4c0: cmp r1,#0x0
  0018d4c2: bne.w 0x0018f0ea
  0018d4c6: ldrb.w r1,[r11,#0xce]
  0018d4ca: cmp r1,#0x0
  0018d4cc: bne.w 0x0018f0ea
  0018d4d0: ldrb.w r1,[r11,#0xca]
  0018d4d4: cmp r1,#0x0
  0018d4d6: bne.w 0x0018f0ea
  0018d4da: ldr.w r1,[r11,#0x40]
  0018d4de: blx 0x00075508
  0018d4e2: ldrb.w r0,[r11,#0x15e]
  0018d4e6: cmp r0,#0x0
  0018d4e8: beq.w 0x0018f0ce
  0018d4ec: ldr.w r0,[r11,#0xf4]
  0018d4f0: movs r1,#0x0
  0018d4f2: movs r2,#0x0
  0018d4f4: blx 0x000728ec
  0018d4f8: ldrb.w r2,[r11,#0x15c]
  0018d4fc: ldr.w r0,[r11,#0x40]
  0018d500: ldr.w r1,[r11,#0xa0]
  0018d504: cmp r2,#0x0
  0018d506: add r0,r1
  0018d508: str.w r0,[r11,#0xa0]
  0018d50c: bne.w 0x0018f09c
  0018d510: vldr.32 s0,[r11,#0x13c]
  0018d514: mov r1,r11
  0018d516: vldr.32 s4,[r11,#0x148]
  0018d51a: vmul.f32 s2,s0,s4
  0018d51e: vnmul.f32 s4,s0,s4
  0018d522: vmov.f32 s0,0x3f800000
  0018d526: vcmpe.f32 s2,#0
  0018d52a: vmrs apsr,fpscr
  0018d52e: it gt
  0018d530: vmov.gt.f32 s4,s2
  0018d534: vstr.32 s2,[r11,#0x148]
  0018d538: vcmpe.f32 s4,s0
  0018d53c: mov r11,r1
  0018d53e: vmrs apsr,fpscr
  0018d542: itttt gt
  0018d544: mov.gt r0,r1
  0018d546: vldr.gt.32 s4,[r1,#0x118]
  0018d54a: vadd.gt.f32 s2,s2,s4
  0018d54e: vstr.gt.32 s2,[r1,#0x118]
  0018d552: vldr.32 s4,[r1,#0x140]
  0018d556: vldr.32 s6,[r1,#0x14c]
  0018d55a: vmul.f32 s2,s4,s6
  0018d55e: vnmul.f32 s4,s4,s6
  0018d562: vcmpe.f32 s2,#0
  0018d566: vmrs apsr,fpscr
  0018d56a: it gt
  0018d56c: vmov.gt.f32 s4,s2
  0018d570: vstr.32 s2,[r1,#0x14c]
  0018d574: vcmpe.f32 s4,s0
  0018d578: vmrs apsr,fpscr
  0018d57c: ble.w 0x0018f09c
  0018d580: vldr.32 s0,[r11,#0x11c]
  0018d584: vadd.f32 s0,s2,s0
  0018d588: vldr.32 s2,[pc,#0x54]
  0018d58c: vcmpe.f32 s0,s2
  0018d590: vstr.32 s0,[r11,#0x11c]
  0018d594: vmrs apsr,fpscr
  0018d598: ble.w 0x0018f086
  0018d59c: ldr r0,[0x0018d5e0]
  0018d59e: mov r11,r1
  0018d5a0: str.w r0,[r1,#0x11c]
  0018d5a4: b.w 0x0018f09c
  0018d5e8: vldr.32 s0,[pc,#-0x8]
  0018d5ec: vmov r2,s18
  0018d5f0: ldr r3,[0x0018d94c]
  0018d5f2: mov r0,r4
  0018d5f4: mov r1,r5
  0018d5f6: vstr.32 s0,[sp]
  0018d5fa: blx 0x00072004
  0018d5fe: ldr.w r0,[r11,#0x58]
  0018d602: ldr.w r4,[r11,#0xf4]
  0018d606: blx 0x000725d4
  0018d60a: vmov s0,r0
  0018d60e: mov r10,r9
  0018d610: vcmpe.f32 s0,#0
  0018d614: vmrs apsr,fpscr
  0018d618: bmi 0x0018d626
  0018d61a: ldr.w r0,[r10,#0x0]
  0018d61e: blx 0x000725d4
  0018d622: vmov s16,r0
  0018d626: ldr.w r0,[r10,#0x0]
  0018d62a: blx 0x000784a8
  0018d62e: vmov r1,s16
  0018d632: mov r2,r0
  0018d634: mov r0,r4
  0018d636: blx 0x000784b4
  0018d63a: ldrb.w r0,[r11,#0x60]
  0018d63e: cbz r0,0x0018d660
  0018d640: ldrd r0,r1,[r11,#0x48]
  0018d644: movw r2,#0xbb9
  0018d648: subs r0,r0,r2
  0018d64a: sbcs r0,r1,#0x0
  0018d64e: itttt ge
  0018d650: ldr.ge.w r0,[r11,#0x40]
  0018d654: ldr.ge.w r1,[r11,#0x50]
  0018d658: add.ge r0,r1
  0018d65a: str.w.ge r0,[r11,#0x50]
  0018d65e: b 0x0018d68e
  0018d660: ldr.w r0,[r10,#0x0]
  0018d664: blx 0x000724b4
  0018d668: cbnz r0,0x0018d688
  0018d66a: ldr r0,[0x0018d950]
  0018d66c: add r0,pc
  0018d66e: ldr r0,[r0,#0x0]
  0018d670: ldr r0,[r0,#0x0]
  0018d672: blx 0x00072850
  0018d676: blx 0x00073924
  0018d67a: cbnz r0,0x0018d688
  0018d67c: mov r0,r11
  0018d67e: blx 0x000784c0
  0018d682: cmp r0,#0x0
  0018d684: bne.w 0x0018f164
  0018d688: mov r0,r11
  0018d68a: blx 0x000784cc
  0018d68e: ldr.w r1,[r11,#0x40]
  0018d692: ldr.w r0,[r11,#0x7c]
  0018d696: ldrb.w r4,[r11,#0x5f]
  0018d69a: blx 0x000784d8
  0018d69e: strb.w r0,[r11,#0x5f]
  0018d6a2: eor r0,r0,#0x1
  0018d6a6: cmp r4,#0x0
  0018d6a8: bne 0x0018d74e
  0018d6aa: cmp r0,#0x0
  0018d6ac: bne 0x0018d74e
  0018d6ae: ldr r0,[0x0018d954]
  0018d6b0: add r0,pc
  0018d6b2: ldr r0,[r0,#0x0]
  0018d6b4: ldrb r1,[r0,#0x0]
  0018d6b6: strb.w r1,[r11,#0x1dd]
  0018d6ba: movs r1,#0x1
  0018d6bc: strb r1,[r0,#0x0]
  0018d6be: ldr.w r0,[r11,#0x16c]
  0018d6c2: cmp r0,#0x0
  0018d6c4: ble 0x0018d6e6
  0018d6c6: ldr r0,[0x0018d958]
  0018d6c8: movs r1,#0x0
  0018d6ca: movs r4,#0x0
  0018d6cc: add r0,pc
  0018d6ce: ldr r0,[r0,#0x0]
  0018d6d0: ldr r0,[r0,#0x0]
  0018d6d2: blx 0x00074f74
  0018d6d6: ldr.w r0,[r11,#0x74]
  0018d6da: movs r2,#0x0
  0018d6dc: movs r3,#0x0
  0018d6de: ldrb r1,[r0,#0x0]
  0018d6e0: str r4,[sp,#0x0]
  0018d6e2: blx 0x000780d0
  0018d6e6: ldr.w r0,[r11,#0x74]
  0018d6ea: movs r1,#0x0
  0018d6ec: strd r1,r1,[r11,#0x98]
  0018d6f0: str.w r1,[r11,#0xa0]
  0018d6f4: strb.w r1,[r11,#0x15c]
  0018d6f8: strb.w r1,[r11,#0x5c]
  0018d6fc: strb.w r1,[r11,#0x110]
  0018d700: movs r1,#0x0
  0018d702: blx 0x00078208
  0018d706: ldr.w r0,[r11,#0x58]
  0018d70a: blx 0x00077b78
  0018d70e: cbz r0,0x0018d718
  0018d710: ldr.w r0,[r10,#0x0]
  0018d714: blx 0x00072cac
  0018d718: ldr r0,[0x0018d95c]
  0018d71a: ldr r1,[0x0018d960]
  0018d71c: add r0,pc
  0018d71e: ldr r2,[0x0018d964]
  0018d720: add r1,pc
  0018d722: ldr.w r5,[r11,#0xf0]
  0018d726: ldr r6,[r0,#0x0]
  0018d728: ldr r0,[r1,#0x0]
  0018d72a: str.w r2,[r11,#0x70]
  0018d72e: ldr r4,[r0,#0x0]
  0018d730: ldr r0,[r6,#0x0]
  0018d732: blx 0x000723d0
  0018d736: cbz r0,0x0018d76a
  0018d738: ldr r0,[r6,#0x0]
  0018d73a: blx 0x00071770
  0018d73e: adr.w r1,0x18d5d8
  0018d742: cmp r0,#0x50
  0018d744: it lt
  0018d746: add.lt r1,#0x4
  0018d748: vldr.32 s0,[r1]
  0018d74c: b 0x0018d76e
  0018d74e: clz r1,r4
  0018d752: eor r0,r0,#0x1
  0018d756: lsrs r1,r1,#0x5
  0018d758: orrs r0,r1
  0018d75a: bne 0x0018d79a
  0018d75c: ldr r0,[0x0018d968]
  0018d75e: ldrb.w r1,[r11,#0x1dd]
  0018d762: add r0,pc
  0018d764: ldr r0,[r0,#0x0]
  0018d766: strb r1,[r0,#0x0]
  0018d768: b 0x0018d79a
  0018d76a: vldr.32 s0,[pc,#-0x188]
  0018d76e: ldr r2,[0x0018d964]
  0018d770: mov r0,r4
  0018d772: ldr r3,[0x0018d94c]
  0018d774: mov r1,r5
  0018d776: vstr.32 s0,[sp]
  0018d77a: blx 0x00072004
  0018d77e: ldr.w r0,[r11,#0xf4]
  0018d782: movs r1,#0x0
  0018d784: movs r2,#0x0
  0018d786: blx 0x000784b4
  0018d78a: ldr.w r0,[r11,#0x8]
  0018d78e: blx 0x0007258c
  0018d792: ldr r1,[0x0018d96c]
  0018d794: movs r2,#0x0
  0018d796: blx 0x000707f8
  0018d79a: ldr.w r0,[r11,#0x7c]
  0018d79e: ldrb r1,[r0,#0x13]
  0018d7a0: cmp r1,#0x0
  0018d7a2: bne.w 0x0018f164
  0018d7a6: ldrb r0,[r0,#0x12]
  0018d7a8: cbz r0,0x0018d7b4
  0018d7aa: movs r0,#0x0
  0018d7ac: movw r1,#0x1389
  0018d7b0: strd r1,r0,[r11,#0x48]
  0018d7b4: ldr.w r0,[r11,#0x14]
  0018d7b8: cmp r0,#0x3
  0018d7ba: bne 0x0018d7cc
  0018d7bc: ldrb.w r0,[r11,#0x15c]
  0018d7c0: cmp r0,#0x0
  0018d7c2: beq.w 0x0018d8c6
  0018d7c6: vldr.32 s0,[r11,#0x11c]
  0018d7ca: b 0x0018da24
  0018d7cc: add.w r5,r11,#0xf4
  0018d7d0: b 0x0018da48
  0018d7d2: ldr.w r0,[r10,#0x0]
  0018d7d6: blx 0x000767bc
  0018d7da: cbz r0,0x0018d828
  0018d7dc: ldr r0,[0x0018d970]
  0018d7de: add r0,pc
  0018d7e0: ldr r5,[r0,#0x0]
  0018d7e2: ldr r0,[r5,#0x0]
  0018d7e4: blx 0x00071770
  0018d7e8: cmp r0,#0x18
  0018d7ea: bne 0x0018d828
  0018d7ec: ldr.w r0,[r11,#0x80]
  0018d7f0: blx 0x00078220
  0018d7f4: mov r4,r0
  0018d7f6: ldr r0,[r5,#0x0]
  0018d7f8: blx 0x0007285c
  0018d7fc: blx 0x00073738
  0018d800: cmp r4,r0
  0018d802: bne 0x0018d828
  0018d804: ldr r0,[r5,#0x0]
  0018d806: blx 0x00071a58
  0018d80a: movs r1,#0xd
  0018d80c: blx 0x0007228c
  0018d810: cmp r0,#0x0
  0018d812: beq.w 0x0018dd40
  0018d816: ldr r0,[r5,#0x0]
  0018d818: blx 0x00071a58
  0018d81c: movs r1,#0x11
  0018d81e: blx 0x0007228c
  0018d822: cmp r0,#0x0
  0018d824: beq.w 0x0018dd40
  0018d828: ldr.w r0,[r10,#0x0]
  0018d82c: blx 0x000784e4
  0018d830: cmp r0,#0x0
  0018d832: beq.w 0x0018d984
  0018d836: ldr r0,[0x0018d974]
  0018d838: ldr r1,[0x0018d978]
  0018d83a: add r0,pc
  0018d83c: add r1,pc
  0018d83e: ldr r6,[r0,#0x0]
  0018d840: ldr r1,[r1,#0x0]
  0018d842: ldr.w r0,[r11,#0x80]
  0018d846: ldr r4,[r6,#0x0]
  0018d848: ldr r5,[r1,#0x0]
  0018d84a: blx 0x00078220
  0018d84e: mov r1,r0
  0018d850: mov r0,r5
  0018d852: blx 0x00071c44
  0018d856: mov r1,r0
  0018d858: mov r0,r4
  0018d85a: blx 0x000745fc
  0018d85e: blx 0x00074608
  0018d862: ldr.w r0,[r11,#0x58]
  0018d866: ldr r0,[r0,#0x0]
  0018d868: blx 0x000724f0
  0018d86c: ldr r1,[r6,#0x0]
  0018d86e: str r0,[r1,#0x64]
  0018d870: ldr.w r0,[r11,#0x58]
  0018d874: ldr r0,[r0,#0x0]
  0018d876: blx 0x000726c4
  0018d87a: ldr r1,[r6,#0x0]
  0018d87c: str r0,[r1,#0x5c]
  0018d87e: ldr.w r0,[r11,#0x58]
  0018d882: ldr r0,[r0,#0x0]
  0018d884: blx 0x00072928
  0018d888: ldr r1,[r6,#0x0]
  0018d88a: str r0,[r1,#0x60]
  0018d88c: ldr.w r0,[r11,#0x58]
  0018d890: ldr r0,[r0,#0x0]
  0018d892: blx 0x00072814
  0018d896: ldr r1,[r6,#0x0]
  0018d898: str r0,[r1,#0x68]
  0018d89a: ldr.w r0,[r11,#0x58]
  0018d89e: blx 0x00076d20
  0018d8a2: ldr r1,[0x0018d97c]
  0018d8a4: ldr r2,[r6,#0x0]
  0018d8a6: add r1,pc
  0018d8a8: ldr r1,[r1,#0x0]
  0018d8aa: str.w r0,[r2,#0xf4]
  0018d8ae: movs r0,#0x1
  0018d8b0: str r0,[r1,#0x0]
  0018d8b2: movs r1,#0x0
  0018d8b4: ldr.w r0,[r11,#0x8]
  0018d8b8: strb.w r1,[r11,#0x54]
  0018d8bc: movs r1,#0x2
  0018d8be: blx 0x00071d64
  0018d8c2: b.w 0x0018f164
  0018d8c6: vldr.32 s2,[r11,#0x13c]
  0018d8ca: vldr.32 s4,[r11,#0x148]
  0018d8ce: vmul.f32 s0,s2,s4
  0018d8d2: vnmul.f32 s4,s2,s4
  0018d8d6: vmov.f32 s2,0x3f800000
  0018d8da: vcmpe.f32 s0,#0
  0018d8de: vmrs apsr,fpscr
  0018d8e2: it gt
  0018d8e4: vmov.gt.f32 s4,s0
  0018d8e8: vstr.32 s0,[r11,#0x148]
  0018d8ec: vcmpe.f32 s4,s2
  0018d8f0: vmrs apsr,fpscr
  0018d8f4: ittt gt
  0018d8f6: vldr.gt.32 s4,[r11,#0x118]
  0018d8fa: vadd.gt.f32 s0,s0,s4
  0018d8fe: vstr.gt.32 s0,[r11,#0x118]
  0018d902: vldr.32 s6,[r11,#0x140]
  0018d906: vldr.32 s8,[r11,#0x14c]
  0018d90a: vldr.32 s0,[r11,#0x11c]
  0018d90e: vmul.f32 s4,s6,s8
  0018d912: vnmul.f32 s6,s6,s8
  0018d916: vcmpe.f32 s4,#0
  0018d91a: vmrs apsr,fpscr
  0018d91e: it gt
  0018d920: vmov.gt.f32 s6,s4
  0018d924: vstr.32 s4,[r11,#0x14c]
  0018d928: vcmpe.f32 s6,s2
  0018d92c: vldr.32 s2,[pc,#-0x350]
  0018d930: vmrs apsr,fpscr
  0018d934: itt gt
  0018d936: vadd.gt.f32 s0,s4,s0
  0018d93a: vstr.gt.32 s0,[r11,#0x11c]
  0018d93e: vcmpe.f32 s0,s2
  0018d942: vmrs apsr,fpscr
  0018d946: ble 0x0018da0c
  0018d948: ldr r0,[0x0018d980]
  0018d94a: b 0x0018da1c
  0018d984: ldr.w r0,[r10,#0x0]
  0018d988: blx 0x000784f0
  0018d98c: cbz r0,0x0018d996
  0018d98e: mov r0,r11
  0018d990: movs r1,#0x0
  0018d992: blx 0x00078388
  0018d996: ldr.w r0,[r10,#0x0]
  0018d99a: blx 0x000784fc
  0018d99e: cbz r0,0x0018d9d0
  0018d9a0: mov r0,r11
  0018d9a2: movs r1,#0x3
  0018d9a4: blx 0x00078388
  0018d9a8: ldr r4,[0x0018dd10]
  0018d9aa: ldr.w r0,[r11,#0xf4]
  0018d9ae: ldr r1,[0x0018dd14]
  0018d9b0: ldr r2,[0x0018dd18]
  0018d9b2: ldr r3,[0x0018dd1c]
  0018d9b4: strd r3,r4,[r11,#0x118]
  0018d9b8: movs r3,#0x0
  0018d9ba: ldr r5,[0x0018dd20]
  0018d9bc: str.w r5,[r11,#0xbc]
  0018d9c0: blx 0x00078508
  0018d9c4: ldr.w r1,[r11,#0xbc]
  0018d9c8: ldr.w r0,[r11,#0xf4]
  0018d9cc: blx 0x00078178
  0018d9d0: ldrb.w r0,[r11,#0xdc]
  0018d9d4: cmp r0,#0x0
  0018d9d6: beq.w 0x0018dbc8
  0018d9da: mov r0,r11
  0018d9dc: blx 0x00078514
  0018d9e0: cmp r0,#0x0
  0018d9e2: bne.w 0x0018f164
  0018d9e6: b 0x0018dbe6
  0018d9e8: vldr.32 s0,[r11,#0x40]
  0018d9ec: vldr.32 s2,[r11,#0x168]
  0018d9f0: vcvt.f32.s32 s0,s0
  0018d9f4: vmul.f32 s0,s2,s0
  0018d9f8: vcvt.s32.f32 s2,s0
  0018d9fc: vcvt.s32.f32 s0,s0
  0018da00: vstr.32 s2,[r11,#0x40]
  0018da04: vmov r0,s0
  0018da08: b.w 0x0018cdcc
  0018da0c: vldr.32 s2,[pc,#0x314]
  0018da10: vcmpe.f32 s0,s2
  0018da14: vmrs apsr,fpscr
  0018da18: bpl 0x0018da24
  0018da1a: ldr r0,[0x0018dd24]
  0018da1c: vmov.f32 s0,s2
  0018da20: str.w r0,[r11,#0x11c]
  0018da24: mov r5,r11
  0018da26: vldr.32 s2,[pc,#0x300]
  0018da2a: ldr.w r0,[r5,#0xf4]!
  0018da2e: movs r3,#0x0
  0018da30: vmul.f32 s0,s0,s2
  0018da34: vldr.32 s4,[r5,#0x24]
  0018da38: vmul.f32 s2,s4,s2
  0018da3c: vmov r1,s0
  0018da40: vmov r2,s2
  0018da44: blx 0x00078508
  0018da48: vmov.f32 s0,0x3f800000
  0018da4c: vldr.32 s2,[r11,#0x168]
  0018da50: vcmpe.f32 s2,s0
  0018da54: vmrs apsr,fpscr
  0018da58: ble 0x0018da98
  0018da5a: movs r4,#0x5
  0018da5c: b 0x0018da92
  0018da5e: ldr.w r1,[r11,#0x16c]
  0018da62: ldr.w r0,[r11,#0xf4]
  0018da66: cmp r1,#0x1
  0018da68: blt 0x0018da74
  0018da6a: vldr.32 s0,[r11,#0x44]
  0018da6e: vcvt.f32.s32 s0,s0
  0018da72: b 0x0018da84
  0018da74: vldr.32 s0,[r11,#0x40]
  0018da78: vldr.32 s2,[r11,#0x168]
  0018da7c: vcvt.f32.s32 s0,s0
  0018da80: vdiv.f32 s0,s0,s2
  0018da84: vcvt.s32.f32 s0,s0
  0018da88: vmov r1,s0
  0018da8c: blx 0x000721a8
  0018da90: subs r4,#0x1
  0018da92: cmp r4,#0x0
  0018da94: bne 0x0018da5e
  0018da96: b 0x0018dab0
  0018da98: ldr.w r1,[r11,#0x16c]
  0018da9c: movs r2,#0x40
  0018da9e: ldr.w r0,[r11,#0xf4]
  0018daa2: cmp r1,#0x0
  0018daa4: it gt
  0018daa6: mov.gt r2,#0x44
  0018daa8: ldr.w r1,[r11,r2]
  0018daac: blx 0x000721a8
  0018dab0: ldrb.w r0,[r11,#0x17c]
  0018dab4: cbz r0,0x0018dac4
  0018dab6: ldr.w r0,[r11,#0x40]
  0018daba: ldr.w r1,[r11,#0x178]
  0018dabe: add r0,r1
  0018dac0: str.w r0,[r11,#0x178]
  0018dac4: ldr.w r0,[r11,#0x14]
  0018dac8: cmp r0,#0x2
  0018daca: bne 0x0018db00
  0018dacc: ldr.w r1,[r11,#0x58]
  0018dad0: ldr.w r0,[r11,#0xf4]
  0018dad4: ldr r1,[r1,#0x0]
  0018dad6: adds r1,#0x4
  0018dad8: blx 0x00078520
  0018dadc: ldrb.w r0,[r11,#0x1a5]
  0018dae0: cbnz r0,0x0018db20
  0018dae2: ldr r0,[r5,#0x0]
  0018dae4: blx 0x00078460
  0018dae8: cbz r0,0x0018db20
  0018daea: ldr r0,[0x0018dd2c]
  0018daec: movs r1,#0x1
  0018daee: strb.w r1,[r11,#0x1a5]
  0018daf2: movs r1,#0x1
  0018daf4: add r0,pc
  0018daf6: ldr r0,[r0,#0x0]
  0018daf8: ldr r0,[r0,#0x0]
  0018dafa: blx 0x00074f68
  0018dafe: b 0x0018db20
  0018db00: ldrb.w r0,[r11,#0x1a5]
  0018db04: cbz r0,0x0018db20
  0018db06: ldr r0,[r5,#0x0]
  0018db08: blx 0x00078460
  0018db0c: cbnz r0,0x0018db20
  0018db0e: ldr r0,[0x0018dd30]
  0018db10: add r0,pc
  0018db12: ldr r0,[r0,#0x0]
  0018db14: ldr r0,[r0,#0x0]
  0018db16: blx 0x00075310
  0018db1a: movs r0,#0x0
  0018db1c: strb.w r0,[r11,#0x1a5]
  0018db20: ldrb.w r0,[r11,#0x18]
  0018db24: ldr.w r4,[r11,#0x58]
  0018db28: cbz r0,0x0018db2e
  0018db2a: movs r1,#0x1
  0018db2c: b 0x0018db36
  0018db2e: ldr r0,[r5,#0x0]
  0018db30: blx 0x00078460
  0018db34: mov r1,r0
  0018db36: mov r0,r4
  0018db38: blx 0x00076bac
  0018db3c: ldrb.w r0,[r11,#0x60]
  0018db40: cmp r0,#0x0
  0018db42: bne.w 0x0018defa
  0018db46: ldrb.w r0,[r11,#0x5f]
  0018db4a: cmp r0,#0x0
  0018db4c: bne.w 0x0018defa
  0018db50: ldr.w r0,[r10,#0x0]
  0018db54: blx 0x000724b4
  0018db58: cmp r0,#0x0
  0018db5a: bne.w 0x0018defa
  0018db5e: ldr r0,[0x0018dd34]
  0018db60: add r0,pc
  0018db62: ldr r0,[r0,#0x0]
  0018db64: ldrb r1,[r0,#0x0]
  0018db66: cbz r1,0x0018db8c
  0018db68: ldr.w r1,[r11,#0x7c]
  0018db6c: movw r3,#0x1389
  0018db70: ldrd r1,r2,[r1,#0x8]
  0018db74: subs r1,r1,r3
  0018db76: sbcs r1,r2,#0x0
  0018db7a: blt 0x0018db8c
  0018db7c: movs r1,#0x0
  0018db7e: strb r1,[r0,#0x0]
  0018db80: movs r0,#0x1
  0018db82: strb.w r0,[r11,#0xdd]
  0018db86: mov r0,r11
  0018db88: blx 0x00078280
  0018db8c: ldrb.w r0,[r11,#0x5c]
  0018db90: cbnz r0,0x0018dba2
  0018db92: ldrb.w r0,[r11,#0x110]
  0018db96: cbnz r0,0x0018dba2
  0018db98: ldr.w r0,[r11,#0x74]
  0018db9c: blx 0x0007852c
  0018dba0: cbz r0,0x0018dbae
  0018dba2: ldr.w r0,[r10,#0x0]
  0018dba6: blx 0x000767bc
  0018dbaa: cmp r0,#0x0
  0018dbac: beq 0x0018dc8c
  0018dbae: ldrb.w r0,[r11,#0x15d]
  0018dbb2: cmp r0,#0x0
  0018dbb4: beq 0x0018dca0
  0018dbb6: ldr.w r0,[r11,#0x58]
  0018dbba: movs r1,#0x0
  0018dbbc: strb.w r1,[r11,#0x15d]
  0018dbc0: movs r1,#0x0
  0018dbc2: blx 0x00072490
  0018dbc6: b 0x0018dca0
  0018dbc8: ldr.w r0,[r10,#0x0]
  0018dbcc: blx 0x00077a7c
  0018dbd0: cbz r0,0x0018dbe6
  0018dbd2: ldr.w r0,[r10,#0x0]
  0018dbd6: blx 0x00078538
  0018dbda: cbz r0,0x0018dbe6
  0018dbdc: mov r0,r11
  0018dbde: blx 0x000782bc
  0018dbe2: b.w 0x0018f164
  0018dbe6: ldr.w r0,[r11,#0x7c]
  0018dbea: blx 0x00078544
  0018dbee: cbnz r0,0x0018dbfa
  0018dbf0: ldr.w r0,[r11,#0x7c]
  0018dbf4: blx 0x000782d4
  0018dbf8: cbnz r0,0x0018dc0a
  0018dbfa: ldrb.w r0,[r11,#0xc5]
  0018dbfe: cbnz r0,0x0018dc0a
  0018dc00: ldrb.w r0,[r11,#0xc6]
  0018dc04: cmp r0,#0x0
  0018dc06: beq.w 0x0018de48
  0018dc0a: mov r6,r11
  0018dc0c: movw r0,#0x1389
  0018dc10: ldr.w r1,[r6,#0x48]!
  0018dc14: ldr r2,[r6,#0x4]
  0018dc16: subs r1,r1,r0
  0018dc18: sbcs r1,r2,#0x0
  0018dc1c: blt.w 0x0018d32e
  0018dc20: ldr.w r1,[r11,#0x7c]
  0018dc24: ldrd r1,r2,[r1,#0x8]
  0018dc28: subs r0,r1,r0
  0018dc2a: sbcs r0,r2,#0x0
  0018dc2e: blt 0x0018dc40
  0018dc30: ldrb.w r0,[r11,#0xdc]
  0018dc34: cbnz r0,0x0018dc40
  0018dc36: ldrb.w r0,[r11,#0x5e]
  0018dc3a: cmp r0,#0x0
  0018dc3c: beq.w 0x0018e088
  0018dc40: ldrd r0,r1,[r6,#0x0]
  0018dc44: movw r2,#0x1389
  0018dc48: subs r0,r0,r2
  0018dc4a: sbcs r0,r1,#0x0
  0018dc4e: blt.w 0x0018d32e
  0018dc52: ldrb.w r0,[r11,#0x5e]
  0018dc56: cmp r0,#0x0
  0018dc58: bne.w 0x0018d32e
  0018dc5c: ldr r0,[0x0018dd38]
  0018dc5e: add r0,pc
  0018dc60: ldr r4,[r0,#0x0]
  0018dc62: ldr r0,[r4,#0x0]
  0018dc64: blx 0x00072850
  0018dc68: blx 0x00073924
  0018dc6c: cmp r0,#0x0
  0018dc6e: bne.w 0x0018d32e
  0018dc72: ldr r0,[r4,#0x0]
  0018dc74: blx 0x00072850
  0018dc78: blx 0x00073930
  0018dc7c: cmp r0,#0x0
  0018dc7e: bne.w 0x0018d32e
  0018dc82: mov r0,r11
  0018dc84: blx 0x00078550
  0018dc88: b.w 0x0018d32e
  0018dc8c: ldr.w r1,[r11,#0x40]
  0018dc90: movs r2,#0x0
  0018dc92: ldr.w r0,[r11,#0x58]
  0018dc96: blx 0x00078340
  0018dc9a: movs r0,#0x1
  0018dc9c: strb.w r0,[r11,#0x15d]
  0018dca0: ldr r0,[0x0018dd3c]
  0018dca2: add r0,pc
  0018dca4: ldr r0,[r0,#0x0]
  0018dca6: ldr r0,[r0,#0x0]
  0018dca8: blx 0x00071770
  0018dcac: cmp r0,#0x30
  0018dcae: beq.w 0x0018defa
  0018dcb2: ldr r0,[sp,#0x28]
  0018dcb4: ldrb r0,[r0,#0x11]
  0018dcb6: cmp r0,#0x0
  0018dcb8: beq 0x0018ddb6
  0018dcba: ldr.w r0,[r11,#0x74]
  0018dcbe: blx 0x0007855c
  0018dcc2: vmov s0,r0
  0018dcc6: vcmpe.f32 s0,#0
  0018dcca: vmrs apsr,fpscr
  0018dcce: bpl 0x0018ddbe
  0018dcd0: ldr.w r1,[r11,#0x16c]
  0018dcd4: movs r2,#0x40
  0018dcd6: ldr.w r0,[r11,#0x74]
  0018dcda: ldr.w r4,[r11,#0x58]
  0018dcde: cmp r1,#0x0
  0018dce0: it gt
  0018dce2: mov.gt r2,#0x44
  0018dce4: ldr.w r5,[r11,r2]
  0018dce8: blx 0x0007855c
  0018dcec: ldr.w r1,[r11,#0x74]
  0018dcf0: vmov s16,r0
  0018dcf4: mov r0,r1
  0018dcf6: blx 0x0007855c
  0018dcfa: vmov s0,r0
  0018dcfe: mov r0,r4
  0018dd00: mov r1,r5
  0018dd02: vmul.f32 s0,s16,s0
  0018dd06: vmov r2,s0
  0018dd0a: blx 0x000729dc
  0018dd0e: b 0x0018de12
  0018dd40: movs r0,#0x1
  0018dd42: strb.w r0,[r11,#0x5d]
  0018dd46: mov r0,r11
  0018dd48: blx 0x0007822c
  0018dd4c: ldr.w r0,[r11,#0x8c]
  0018dd50: cbz r0,0x0018dd60
  0018dd52: blx 0x00075010
  0018dd56: blx 0x0006eb48
  0018dd5a: movs r0,#0x0
  0018dd5c: str.w r0,[r11,#0x8c]
  0018dd60: movs r0,#0x74
  0018dd62: blx 0x0006eb24
  0018dd66: mov r5,r0
  0018dd68: ldr r0,[0x0018e05c]
  0018dd6a: add r0,pc
  0018dd6c: ldr r6,[r0,#0x0]
  0018dd6e: ldr r0,[r6,#0x0]
  0018dd70: mov.w r1,#0x214
  0018dd74: blx 0x00072f70
  0018dd78: mov r4,r0
  0018dd7a: ldr r0,[r6,#0x0]
  0018dd7c: movw r1,#0x643
  0018dd80: blx 0x00072f70
  0018dd84: mov r2,r0
  0018dd86: ldr r0,[0x0018e060]
  0018dd88: add r0,pc
  0018dd8a: add.w r3,r0,#0x28
  0018dd8e: mov r0,r5
  0018dd90: mov r1,r4
  0018dd92: blx 0x00074f20
  0018dd96: movs r0,#0x1
  0018dd98: strb.w r0,[r11,#0x5e]
  0018dd9c: ldr.w r0,[r11,#0x58]
  0018dda0: str.w r5,[r11,#0x8c]
  0018dda4: blx 0x00078568
  0018dda8: ldr.w r0,[r11,#0x58]
  0018ddac: movs r1,#0x0
  0018ddae: blx 0x000728d4
  0018ddb2: b.w 0x0018f164
  0018ddb6: mov r0,r11
  0018ddb8: blx 0x00078574
  0018ddbc: b 0x0018defa
  0018ddbe: ldr.w r0,[r11,#0x74]
  0018ddc2: blx 0x0007855c
  0018ddc6: vmov s0,r0
  0018ddca: vcmpe.f32 s0,#0
  0018ddce: vmrs apsr,fpscr
  0018ddd2: ble 0x0018de12
  0018ddd4: ldr.w r1,[r11,#0x16c]
  0018ddd8: movs r2,#0x40
  0018ddda: ldr.w r0,[r11,#0x74]
  0018ddde: ldr.w r4,[r11,#0x58]
  0018dde2: cmp r1,#0x0
  0018dde4: it gt
  0018dde6: mov.gt r2,#0x44
  0018dde8: ldr.w r5,[r11,r2]
  0018ddec: blx 0x0007855c
  0018ddf0: ldr.w r1,[r11,#0x74]
  0018ddf4: vmov s16,r0
  0018ddf8: mov r0,r1
  0018ddfa: blx 0x0007855c
  0018ddfe: vmov s0,r0
  0018de02: mov r0,r4
  0018de04: mov r1,r5
  0018de06: vmul.f32 s0,s16,s0
  0018de0a: vmov r2,s0
  0018de0e: blx 0x000729d0
  0018de12: ldr.w r0,[r11,#0x74]
  0018de16: ldr r1,[sp,#0x28]
  0018de18: ldrb r4,[r1,#0x10]
  0018de1a: blx 0x00078580
  0018de1e: vmov s0,r0
  0018de22: cmp r4,#0x0
  0018de24: vcmpe.f32 s0,#0
  0018de28: beq 0x0018de60
  0018de2a: vmrs apsr,fpscr
  0018de2e: bmi 0x0018debc
  0018de30: ldr.w r0,[r11,#0x74]
  0018de34: blx 0x00078580
  0018de38: vmov s0,r0
  0018de3c: vcmpe.f32 s0,#0
  0018de40: vmrs apsr,fpscr
  0018de44: bgt 0x0018de66
  0018de46: b 0x0018defa
  0018de48: mov r0,r11
  0018de4a: blx 0x0007858c
  0018de4e: cmp r0,#0x0
  0018de50: bne.w 0x0018f164
  0018de54: ldrb.w r0,[r11,#0xc7]
  0018de58: cmp r0,#0x0
  0018de5a: bne.w 0x0018f164
  0018de5e: b 0x0018dc0a
  0018de60: vmrs apsr,fpscr
  0018de64: bpl 0x0018dea6
  0018de66: ldr.w r1,[r11,#0x16c]
  0018de6a: movs r2,#0x40
  0018de6c: ldr.w r0,[r11,#0x74]
  0018de70: ldr.w r4,[r11,#0x58]
  0018de74: cmp r1,#0x0
  0018de76: it gt
  0018de78: mov.gt r2,#0x44
  0018de7a: ldr.w r5,[r11,r2]
  0018de7e: blx 0x00078580
  0018de82: ldr.w r1,[r11,#0x74]
  0018de86: vmov s16,r0
  0018de8a: mov r0,r1
  0018de8c: blx 0x00078580
  0018de90: vmov s0,r0
  0018de94: mov r0,r4
  0018de96: mov r1,r5
  0018de98: vmul.f32 s0,s16,s0
  0018de9c: vmov r2,s0
  0018dea0: blx 0x000729f4
  0018dea4: b 0x0018defa
  0018dea6: ldr.w r0,[r11,#0x74]
  0018deaa: blx 0x00078580
  0018deae: vmov s0,r0
  0018deb2: vcmpe.f32 s0,#0
  0018deb6: vmrs apsr,fpscr
  0018deba: ble 0x0018defa
  0018debc: ldr.w r1,[r11,#0x16c]
  0018dec0: movs r2,#0x40
  0018dec2: ldr.w r0,[r11,#0x74]
  0018dec6: ldr.w r4,[r11,#0x58]
  0018deca: cmp r1,#0x0
  0018decc: it gt
  0018dece: mov.gt r2,#0x44
  0018ded0: ldr.w r5,[r11,r2]
  0018ded4: blx 0x00078580
  0018ded8: ldr.w r1,[r11,#0x74]
  0018dedc: vmov s16,r0
  0018dee0: mov r0,r1
  0018dee2: blx 0x00078580
  0018dee6: vmov s0,r0
  0018deea: mov r0,r4
  0018deec: mov r1,r5
  0018deee: vmul.f32 s0,s16,s0
  0018def2: vmov r2,s0
  0018def6: blx 0x000729e8
  0018defa: ldr.w r0,[r11,#0x58]
  0018defe: movs r1,#0x0
  0018df00: strb.w r1,[r11,#0x110]
  0018df04: blx 0x00076c84
  0018df08: cmp r0,#0x0
  0018df0a: beq.w 0x0018f164
  0018df0e: ldr r0,[0x0018e064]
  0018df10: add r0,pc
  0018df12: ldr r5,[r0,#0x0]
  0018df14: ldr r0,[r5,#0x0]
  0018df16: blx 0x00072850
  0018df1a: blx 0x0007372c
  0018df1e: cbz r0,0x0018df94
  0018df20: ldr r0,[r5,#0x0]
  0018df22: blx 0x00071770
  0018df26: cmp r0,#0x1d
  0018df28: beq 0x0018df42
  0018df2a: cmp r0,#0x2a
  0018df2c: beq 0x0018df7e
  0018df2e: cmp r0,#0x29
  0018df30: beq 0x0018df42
  0018df32: cmp r0,#0x28
  0018df34: bne 0x0018df52
  0018df36: ldr.w r0,[r11,#0x7c]
  0018df3a: blx 0x00077320
  0018df3e: cmp r0,#0x3
  0018df40: bgt 0x0018df5c
  0018df42: ldr.w r0,[r10,#0x0]
  0018df46: movs r1,#0x0
  0018df48: ldr r0,[r0,#0x0]
  0018df4a: blx 0x000727e4
  0018df4e: b.w 0x0018f164
  0018df52: ldr r0,[r5,#0x0]
  0018df54: movs r1,#0x1
  0018df56: blx 0x00075070
  0018df5a: b 0x0018df7e
  0018df5c: ldr r0,[r5,#0x0]
  0018df5e: movs r1,#0x1
  0018df60: blx 0x00075070
  0018df64: ldr.w r0,[r11,#0x78]
  0018df68: blx 0x00071eb4
  0018df6c: ldr r0,[r0,#0x4]
  0018df6e: ldr r0,[r0,#0x0]
  0018df70: ldr r0,[r0,#0x4]
  0018df72: blx 0x000724f0
  0018df76: ldr r1,[0x0018e068]
  0018df78: add r1,pc
  0018df7a: ldr r1,[r1,#0x0]
  0018df7c: str r0,[r1,#0x0]
  0018df7e: ldr.w r0,[r11,#0x78]
  0018df82: blx 0x000783dc
  0018df86: ldr r0,[0x0018e06c]
  0018df88: add r0,pc
  0018df8a: ldr r0,[r0,#0x0]
  0018df8c: ldr r1,[r0,#0x0]
  0018df8e: ldr r0,[r5,#0x0]
  0018df90: blx 0x00073c3c
  0018df94: ldr r0,[0x0018e070]
  0018df96: add r0,pc
  0018df98: ldr r0,[r0,#0x0]
  0018df9a: ldr r1,[r0,#0x0]
  0018df9c: ldr r0,[r5,#0x0]
  0018df9e: blx 0x00073c3c
  0018dfa2: ldr r0,[r5,#0x0]
  0018dfa4: blx 0x000723d0
  0018dfa8: mov r1,r0
  0018dfaa: ldr r0,[r5,#0x0]
  0018dfac: cbz r1,0x0018dfd0
  0018dfae: blx 0x00071770
  0018dfb2: cmp r0,#0x2a
  0018dfb4: beq.w 0x0018f164
  0018dfb8: ldr r0,[0x0018e074]
  0018dfba: ldr r4,[r5,#0x0]
  0018dfbc: add r0,pc
  0018dfbe: ldr r0,[r0,#0x0]
  0018dfc0: ldr.w r1,[r4,#0x84]
  0018dfc4: ldr r0,[r0,#0x0]
  0018dfc6: blx 0x00071c44
  0018dfca: mov r1,r0
  0018dfcc: mov r0,r4
  0018dfce: b 0x0018dfda
  0018dfd0: ldr r1,[r0,#0x78]
  0018dfd2: blx 0x0007366c
  0018dfd6: ldr r0,[r5,#0x0]
  0018dfd8: ldr r1,[r0,#0x78]
  0018dfda: blx 0x000745fc
  0018dfde: ldr.w r0,[r11,#0x58]
  0018dfe2: ldr r0,[r0,#0x0]
  0018dfe4: blx 0x000724f0
  0018dfe8: ldr r1,[r5,#0x0]
  0018dfea: str r0,[r1,#0x64]
  0018dfec: ldr.w r0,[r11,#0x58]
  0018dff0: ldr r0,[r0,#0x0]
  0018dff2: blx 0x000726c4
  0018dff6: ldr r1,[r5,#0x0]
  0018dff8: str r0,[r1,#0x5c]
  0018dffa: ldr.w r0,[r11,#0x58]
  0018dffe: ldr r0,[r0,#0x0]
  0018e000: blx 0x00072928
  0018e004: ldr r1,[r5,#0x0]
  0018e006: str r0,[r1,#0x60]
  0018e008: ldr.w r0,[r11,#0x58]
  0018e00c: ldr r0,[r0,#0x0]
  0018e00e: blx 0x00072814
  0018e012: ldr r1,[r5,#0x0]
  0018e014: str r0,[r1,#0x68]
  0018e016: ldr.w r0,[r11,#0x58]
  0018e01a: blx 0x00076d20
  0018e01e: ldr r1,[0x0018e078]
  0018e020: ldr r2,[0x0018e07c]
  0018e022: ldr r3,[0x0018e080]
  0018e024: add r1,pc
  0018e026: ldr r4,[0x0018e084]
  0018e028: add r2,pc
  0018e02a: add r3,pc
  0018e02c: ldr r6,[r5,#0x0]
  0018e02e: add r4,pc
  0018e030: ldr r1,[r1,#0x0]
  0018e032: ldr r2,[r2,#0x0]
  0018e034: ldr r5,[r4,#0x0]
  0018e036: movs r4,#0x0
  0018e038: ldr r3,[r3,#0x0]
  0018e03a: str.w r0,[r6,#0xf4]
  0018e03e: movs r0,#0x1
  0018e040: strb r0,[r1,#0x0]
  0018e042: movs r1,#0x2
  0018e044: strb r0,[r5,#0x0]
  0018e046: str r4,[r2,#0x0]
  0018e048: str r0,[r3,#0x0]
  0018e04a: ldr.w r0,[r11,#0x8]
  0018e04e: blx 0x00071d64
  0018e052: strb.w r4,[r11,#0x54]
  0018e056: b.w 0x0018f164
  0018e088: ldr r0,[0x0018e3f4]
  0018e08a: add r0,pc
  0018e08c: ldr.w r9,[r0,#0x0]
  0018e090: ldr.w r0,[r9,#0x0]
  0018e094: blx 0x00071770
  0018e098: cmp r0,#0x17
  0018e09a: bne 0x0018e176
  0018e09c: ldr.w r0,[r10,#0x0]
  0018e0a0: blx 0x00077adc
  0018e0a4: cmp r0,#0x0
  0018e0a6: bne 0x0018e176
  0018e0a8: ldr.w r0,[r9,#0x0]
  0018e0ac: blx 0x00072850
  0018e0b0: blx 0x00072868
  0018e0b4: cmp r0,#0x0
  0018e0b6: beq 0x0018e176
  0018e0b8: ldr.w r0,[r9,#0x0]
  0018e0bc: blx 0x00071710
  0018e0c0: ldr.w r2,[r9,#0x0]
  0018e0c4: ldr r4,[0x0018e3f8]
  0018e0c6: ldrd r2,r3,[r2,#0x100]
  0018e0ca: subs r0,r0,r2
  0018e0cc: sbcs r1,r3
  0018e0ce: subs r0,r4,r0
  0018e0d0: mov.w r2,#0x0
  0018e0d4: sbcs.w r0,r2,r1
  0018e0d8: bge 0x0018e176
  0018e0da: movs r0,#0x1
  0018e0dc: strb.w r0,[r11,#0x5d]
  0018e0e0: mov r0,r11
  0018e0e2: blx 0x0007822c
  0018e0e6: ldr.w r0,[r11,#0x8c]
  0018e0ea: str.w r11,[sp,#0x20]
  0018e0ee: cbz r0,0x0018e100
  0018e0f0: blx 0x00075010
  0018e0f4: blx 0x0006eb48
  0018e0f8: ldr r1,[sp,#0x20]
  0018e0fa: movs r0,#0x0
  0018e0fc: str.w r0,[r1,#0x8c]
  0018e100: ldr r0,[0x0018e3fc]
  0018e102: movs r1,#0x2
  0018e104: add r0,pc
  0018e106: ldr r0,[r0,#0x0]
  0018e108: ldr r0,[r0,#0x0]
  0018e10a: blx 0x00071848
  0018e10e: movw r4,#0x216
  0018e112: str r0,[sp,#0x24]
  0018e114: cmp r0,#0x0
  0018e116: mov.w r0,#0x74
  0018e11a: it eq
  0018e11c: movw.eq r4,#0x215
  0018e120: blx 0x0006eb24
  0018e124: mov r8,r0
  0018e126: ldr r0,[0x0018e400]
  0018e128: add r0,pc
  0018e12a: ldr r5,[r0,#0x0]
  0018e12c: ldr r0,[r5,#0x0]
  0018e12e: mov r1,r4
  0018e130: blx 0x00072f70
  0018e134: mov r4,r0
  0018e136: ldr r0,[r5,#0x0]
  0018e138: movw r1,#0x642
  0018e13c: blx 0x00072f70
  0018e140: mov r2,r0
  0018e142: ldr r0,[0x0018e404]
  0018e144: add r0,pc
  0018e146: add.w r3,r0,#0x14
  0018e14a: mov r0,r8
  0018e14c: mov r1,r4
  0018e14e: blx 0x00074f20
  0018e152: ldr r0,[0x0018e408]
  0018e154: movs r1,#0x0
  0018e156: ldr.w r11,[sp,#0x20]
  0018e15a: add r0,pc
  0018e15c: ldr r2,[sp,#0x24]
  0018e15e: ldr r0,[r0,#0x0]
  0018e160: cmp r2,#0x0
  0018e162: str.w r8,[r11,#0x8c]
  0018e166: str r1,[sp,#0x0]
  0018e168: movw r1,#0x1d1
  0018e16c: ldr r0,[r0,#0x0]
  0018e16e: it eq
  0018e170: mov.eq.w r1,#0x1d0
  0018e174: b 0x0018e25a
  0018e176: ldr.w r0,[r9,#0x0]
  0018e17a: blx 0x00071770
  0018e17e: cmp r0,#0x18
  0018e180: bne 0x0018e27e
  0018e182: ldr.w r0,[r10,#0x0]
  0018e186: blx 0x00077adc
  0018e18a: cmp r0,#0x0
  0018e18c: bne 0x0018e27e
  0018e18e: ldr.w r0,[r9,#0x0]
  0018e192: blx 0x00072850
  0018e196: blx 0x00072868
  0018e19a: cmp r0,#0x0
  0018e19c: beq 0x0018e27e
  0018e19e: ldr.w r0,[r9,#0x0]
  0018e1a2: blx 0x00071710
  0018e1a6: ldr.w r2,[r9,#0x0]
  0018e1aa: ldr r4,[0x0018e3f8]
  0018e1ac: ldrd r2,r3,[r2,#0x100]
  0018e1b0: subs r0,r0,r2
  0018e1b2: sbcs r1,r3
  0018e1b4: subs r0,r4,r0
  0018e1b6: mov.w r2,#0x0
  0018e1ba: sbcs.w r0,r2,r1
  0018e1be: bge 0x0018e27e
  0018e1c0: movs r0,#0x1
  0018e1c2: strb.w r0,[r11,#0x5d]
  0018e1c6: mov r0,r11
  0018e1c8: blx 0x0007822c
  0018e1cc: ldr.w r0,[r11,#0x8c]
  0018e1d0: str.w r11,[sp,#0x20]
  0018e1d4: cbz r0,0x0018e1e6
  0018e1d6: blx 0x00075010
  0018e1da: blx 0x0006eb48
  0018e1de: ldr r1,[sp,#0x20]
  0018e1e0: movs r0,#0x0
  0018e1e2: str.w r0,[r1,#0x8c]
  0018e1e6: ldr r0,[0x0018e40c]
  0018e1e8: movs r1,#0x2
  0018e1ea: add r0,pc
  0018e1ec: ldr r0,[r0,#0x0]
  0018e1ee: ldr r0,[r0,#0x0]
  0018e1f0: blx 0x00071848
  0018e1f4: mov.w r4,#0x218
  0018e1f8: str r0,[sp,#0x24]
  0018e1fa: cmp r0,#0x0
  0018e1fc: mov.w r0,#0x74
  0018e200: it eq
  0018e202: movw.eq r4,#0x217
  0018e206: blx 0x0006eb24
  0018e20a: mov r8,r0
  0018e20c: ldr r0,[0x0018e410]
  0018e20e: add r0,pc
  0018e210: ldr r5,[r0,#0x0]
  0018e212: ldr r0,[r5,#0x0]
  0018e214: mov r1,r4
  0018e216: blx 0x00072f70
  0018e21a: mov r4,r0
  0018e21c: ldr r0,[r5,#0x0]
  0018e21e: movw r1,#0x643
  0018e222: blx 0x00072f70
  0018e226: mov r2,r0
  0018e228: ldr r0,[0x0018e414]
  0018e22a: add r0,pc
  0018e22c: add.w r3,r0,#0x28
  0018e230: mov r0,r8
  0018e232: mov r1,r4
  0018e234: blx 0x00074f20
  0018e238: ldr r0,[0x0018e418]
  0018e23a: movs r1,#0x0
  0018e23c: ldr.w r11,[sp,#0x20]
  0018e240: add r0,pc
  0018e242: ldr r2,[sp,#0x24]
  0018e244: ldr r0,[r0,#0x0]
  0018e246: cmp r2,#0x0
  0018e248: str.w r8,[r11,#0x8c]
  0018e24c: str r1,[sp,#0x0]
  0018e24e: mov.w r1,#0x1d4
  0018e252: ldr r0,[r0,#0x0]
  0018e254: it eq
  0018e256: movw.eq r1,#0x1d3
  0018e25a: movs r2,#0x0
  0018e25c: movs r3,#0x0
  0018e25e: blx 0x00071548
  0018e262: movs r0,#0x1
  0018e264: strb.w r0,[r11,#0x111]
  0018e268: strb.w r0,[r11,#0x5e]
  0018e26c: ldr.w r0,[r9,#0x0]
  0018e270: blx 0x00071710
  0018e274: ldr.w r2,[r9,#0x0]
  0018e278: strd r0,r1,[r2,#0x100]
  0018e27c: b 0x0018e368
  0018e27e: ldr.w r0,[r9,#0x0]
  0018e282: blx 0x00071770
  0018e286: cmp r0,#0x7a
  0018e288: blt 0x0018e2e0
  0018e28a: ldr.w r0,[r9,#0x0]
  0018e28e: blx 0x00071770
  0018e292: cmp r0,#0x7c
  0018e294: bgt 0x0018e2e0
  0018e296: ldr.w r0,[r10,#0x0]
  0018e29a: blx 0x00077adc
  0018e29e: cbnz r0,0x0018e2e0
  0018e2a0: ldr.w r0,[r9,#0x0]
  0018e2a4: blx 0x00072850
  0018e2a8: blx 0x00072868
  0018e2ac: cbz r0,0x0018e2e0
  0018e2ae: ldr.w r0,[r9,#0x0]
  0018e2b2: blx 0x00071710
  0018e2b6: ldr.w r2,[r9,#0x0]
  0018e2ba: ldr r4,[0x0018e3f8]
  0018e2bc: ldrd r2,r3,[r2,#0x100]
  0018e2c0: subs r0,r0,r2
  0018e2c2: sbcs r1,r3
  0018e2c4: subs r0,r4,r0
  0018e2c6: mov.w r2,#0x0
  0018e2ca: sbcs.w r0,r2,r1
  0018e2ce: bge 0x0018e2e0
  0018e2d0: ldrb.w r0,[r11,#0xdc]
  0018e2d4: cbnz r0,0x0018e2e0
  0018e2d6: ldrb.w r0,[r11,#0xdd]
  0018e2da: cmp r0,#0x0
  0018e2dc: beq.w 0x0018f182
  0018e2e0: ldr.w r0,[r9,#0x0]
  0018e2e4: blx 0x000723d0
  0018e2e8: cbnz r0,0x0018e368
  0018e2ea: ldr.w r0,[r9,#0x0]
  0018e2ee: ldrb.w r1,[r0,#0x17c]
  0018e2f2: cbz r1,0x0018e368
  0018e2f4: blx 0x00071710
  0018e2f8: ldr.w r2,[r9,#0x0]
  0018e2fc: ldrd r2,r3,[r2,#0x100]
  0018e300: subs r0,r0,r2
  0018e302: sbcs r1,r3
  0018e304: movw r2,#0x2ee1
  0018e308: subs r0,r0,r2
  0018e30a: sbcs r0,r1,#0x0
  0018e30e: blt 0x0018e368
  0018e310: ldrb.w r0,[r11,#0xdc]
  0018e314: cbnz r0,0x0018e368
  0018e316: ldrb.w r0,[r11,#0xdd]
  0018e31a: cbnz r0,0x0018e368
  0018e31c: str.w r9,[sp,#0x24]
  0018e320: movs r4,#0x0
  0018e322: ldr r0,[0x0018e41c]
  0018e324: mov.w r8,#0xffffffff
  0018e328: add r0,pc
  0018e32a: mov r9,r0
  0018e32c: b 0x0018e344
  0018e32e: ldr r0,[sp,#0x24]
  0018e330: ldr.w r5,[r9,r4,lsl #0x2]
  0018e334: ldr r0,[r0,#0x0]
  0018e336: blx 0x00071770
  0018e33a: cmp r0,r5
  0018e33c: it ge
  0018e33e: add.ge.w r8,r8,#0x1
  0018e342: adds r4,#0x1
  0018e344: cmp r4,#0x3
  0018e346: bne 0x0018e32e
  0018e348: ldr.w r9,[sp,#0x24]
  0018e34c: cmp.w r8,#0x3
  0018e350: bcs 0x0018e368
  0018e352: ldr.w r0,[r9,#0x0]
  0018e356: movs r1,#0x0
  0018e358: mov r2,r8
  0018e35a: strb.w r1,[r0,#0x17c]
  0018e35e: movs r1,#0x1b
  0018e360: ldr.w r0,[r11,#0x78]
  0018e364: blx 0x00072754
  0018e368: ldr r0,[0x0018e420]
  0018e36a: add r0,pc
  0018e36c: ldr.w r8,[r0,#0x0]
  0018e370: ldrb.w r0,[r8,#0x17]
  0018e374: cmp r0,#0x0
  0018e376: bne 0x0018e42c
  0018e378: ldr.w r0,[r9,#0x0]
  0018e37c: blx 0x00071a58
  0018e380: blx 0x00078598
  0018e384: cmp r0,#0x0
  0018e386: beq 0x0018e42c
  0018e388: ldr.w r0,[r9,#0x0]
  0018e38c: blx 0x00071770
  0018e390: cmp r0,#0x2
  0018e392: blt 0x0018e42c
  0018e394: ldr.w r0,[r11,#0x94]
  0018e398: cbnz r0,0x0018e3aa
  0018e39a: movs r0,#0x5c
  0018e39c: blx 0x0006eb24
  0018e3a0: mov r5,r0
  0018e3a2: blx 0x00074584
  0018e3a6: str.w r5,[r11,#0x94]
  0018e3aa: ldr r0,[0x0018e424]
  0018e3ac: ldr r1,[0x0018e428]
  0018e3ae: add r0,pc
  0018e3b0: add r1,pc
  0018e3b2: ldr r0,[r0,#0x0]
  0018e3b4: ldr r1,[r1,#0x0]
  0018e3b6: ldr r4,[r0,#0x0]
  0018e3b8: ldr r0,[r1,#0x0]
  0018e3ba: mov.w r1,#0x254
  0018e3be: blx 0x00072f70
  0018e3c2: add r5,sp,#0x78
  0018e3c4: mov r2,r0
  0018e3c6: mov r1,r4
  0018e3c8: mov r0,r5
  0018e3ca: blx 0x00077c08
  0018e3ce: ldr.w r0,[r11,#0x94]
  0018e3d2: mov r1,r5
  0018e3d4: blx 0x00074794
  0018e3d8: movs r4,#0x1
  0018e3da: strb.w r4,[r11,#0x111]
  0018e3de: strb.w r4,[r11,#0xce]
  0018e3e2: strb.w r4,[r11,#0x5d]
  0018e3e6: mov r0,r11
  0018e3e8: blx 0x0007822c
  0018e3ec: strb.w r4,[r8,#0x17]
  0018e3f0: b 0x0018e608
  0018e42c: ldrb.w r0,[r8,#0x21]
  0018e430: cbnz r0,0x0018e49e
  0018e432: ldr.w r0,[r9,#0x0]
  0018e436: blx 0x00071a58
  0018e43a: blx 0x00071a64
  0018e43e: cbz r0,0x0018e49e
  0018e440: ldr.w r0,[r11,#0x94]
  0018e444: cbnz r0,0x0018e456
  0018e446: movs r0,#0x5c
  0018e448: blx 0x0006eb24
  0018e44c: mov r5,r0
  0018e44e: blx 0x00074584
  0018e452: str.w r5,[r11,#0x94]
  0018e456: ldr r0,[0x0018e7a0]
  0018e458: ldr r1,[0x0018e7a4]
  0018e45a: add r0,pc
  0018e45c: add r1,pc
  0018e45e: ldr r0,[r0,#0x0]
  0018e460: ldr r1,[r1,#0x0]
  0018e462: ldr r4,[r0,#0x0]
  0018e464: ldr r0,[r1,#0x0]
  0018e466: movw r1,#0x24e
  0018e46a: blx 0x00072f70
  0018e46e: add r5,sp,#0x78
  0018e470: mov r2,r0
  0018e472: mov r1,r4
  0018e474: mov r0,r5
  0018e476: blx 0x00077c08
  0018e47a: ldr.w r0,[r11,#0x94]
  0018e47e: mov r1,r5
  0018e480: blx 0x00074794
  0018e484: movs r4,#0x1
  0018e486: strb.w r4,[r11,#0x111]
  0018e48a: strb.w r4,[r11,#0xce]
  0018e48e: strb.w r4,[r11,#0x5d]
  0018e492: mov r0,r11
  0018e494: blx 0x0007822c
  0018e498: strb.w r4,[r8,#0x21]
  0018e49c: b 0x0018e608
  0018e49e: ldrb.w r0,[r8,#0x22]
  0018e4a2: cbnz r0,0x0018e510
  0018e4a4: ldr.w r0,[r9,#0x0]
  0018e4a8: blx 0x00071a58
  0018e4ac: blx 0x000779e0
  0018e4b0: cbz r0,0x0018e510
  0018e4b2: ldr.w r0,[r11,#0x94]
  0018e4b6: cbnz r0,0x0018e4c8
  0018e4b8: movs r0,#0x5c
  0018e4ba: blx 0x0006eb24
  0018e4be: mov r5,r0
  0018e4c0: blx 0x00074584
  0018e4c4: str.w r5,[r11,#0x94]
  0018e4c8: ldr r0,[0x0018e7a8]
  0018e4ca: ldr r1,[0x0018e7ac]
  0018e4cc: add r0,pc
  0018e4ce: add r1,pc
  0018e4d0: ldr r0,[r0,#0x0]
  0018e4d2: ldr r1,[r1,#0x0]
  0018e4d4: ldr r4,[r0,#0x0]
  0018e4d6: ldr r0,[r1,#0x0]
  0018e4d8: movw r1,#0x251
  0018e4dc: blx 0x00072f70
  0018e4e0: add r5,sp,#0x78
  0018e4e2: mov r2,r0
  0018e4e4: mov r1,r4
  0018e4e6: mov r0,r5
  0018e4e8: blx 0x00077c08
  0018e4ec: ldr.w r0,[r11,#0x94]
  0018e4f0: mov r1,r5
  0018e4f2: blx 0x00074794
  0018e4f6: movs r4,#0x1
  0018e4f8: strb.w r4,[r11,#0x111]
  0018e4fc: strb.w r4,[r11,#0xce]
  0018e500: strb.w r4,[r11,#0x5d]
  0018e504: mov r0,r11
  0018e506: blx 0x0007822c
  0018e50a: strb.w r4,[r8,#0x22]
  0018e50e: b 0x0018e608
  0018e510: ldrb.w r0,[r8,#0x20]
  0018e514: cmp r0,#0x0
  0018e516: bne 0x0018e59c
  0018e518: ldr.w r0,[r10,#0x0]
  0018e51c: blx 0x0007681c
  0018e520: cbnz r0,0x0018e59c
  0018e522: ldr.w r0,[r9,#0x0]
  0018e526: blx 0x00071770
  0018e52a: cmp r0,#0xa
  0018e52c: blt 0x0018e59c
  0018e52e: ldr.w r0,[r9,#0x0]
  0018e532: blx 0x00072850
  0018e536: blx 0x00072868
  0018e53a: cbz r0,0x0018e59c
  0018e53c: ldr.w r0,[r11,#0x94]
  0018e540: mov r6,r11
  0018e542: cbnz r0,0x0018e554
  0018e544: movs r0,#0x5c
  0018e546: blx 0x0006eb24
  0018e54a: mov r5,r0
  0018e54c: blx 0x00074584
  0018e550: str.w r5,[r6,#0x94]
  0018e554: ldr r0,[0x0018e7b0]
  0018e556: ldr r1,[0x0018e7b4]
  0018e558: add r0,pc
  0018e55a: add r1,pc
  0018e55c: ldr r0,[r0,#0x0]
  0018e55e: ldr r1,[r1,#0x0]
  0018e560: ldr r4,[r0,#0x0]
  0018e562: ldr r0,[r1,#0x0]
  0018e564: mov.w r1,#0x248
  0018e568: blx 0x00072f70
  0018e56c: add r5,sp,#0x78
  0018e56e: mov r2,r0
  0018e570: mov r1,r4
  0018e572: mov r0,r5
  0018e574: blx 0x00077c08
  0018e578: ldr.w r0,[r6,#0x94]
  0018e57c: mov r1,r5
  0018e57e: blx 0x00074794
  0018e582: movs r4,#0x1
  0018e584: mov r0,r6
  0018e586: strb.w r4,[r6,#0x111]
  0018e58a: strb.w r4,[r6,#0xce]
  0018e58e: strb.w r4,[r6,#0x5d]
  0018e592: blx 0x0007822c
  0018e596: strb.w r4,[r8,#0x20]
  0018e59a: b 0x0018e608
  0018e59c: ldr.w r0,[r9,#0x0]
  0018e5a0: ldr r1,[r0,#0x24]
  0018e5a2: cbz r1,0x0018e612
  0018e5a4: ldrb.w r1,[r8,#0x15]
  0018e5a8: cbnz r1,0x0018e612
  0018e5aa: movs r0,#0x1
  0018e5ac: strb.w r0,[r11,#0x5d]
  0018e5b0: mov r0,r11
  0018e5b2: blx 0x0007822c
  0018e5b6: ldr.w r0,[r11,#0x94]
  0018e5ba: cbnz r0,0x0018e5cc
  0018e5bc: movs r0,#0x5c
  0018e5be: blx 0x0006eb24
  0018e5c2: mov r5,r0
  0018e5c4: blx 0x00074584
  0018e5c8: str.w r5,[r11,#0x94]
  0018e5cc: ldr r0,[0x0018e7b8]
  0018e5ce: ldr r1,[0x0018e7bc]
  0018e5d0: add r0,pc
  0018e5d2: add r1,pc
  0018e5d4: ldr r0,[r0,#0x0]
  0018e5d6: ldr r1,[r1,#0x0]
  0018e5d8: ldr r4,[r0,#0x0]
  0018e5da: ldr r0,[r1,#0x0]
  0018e5dc: movw r1,#0x27d
  0018e5e0: blx 0x00072f70
  0018e5e4: add r5,sp,#0x78
  0018e5e6: mov r2,r0
  0018e5e8: mov r1,r4
  0018e5ea: mov r0,r5
  0018e5ec: blx 0x00077c08
  0018e5f0: ldr.w r0,[r11,#0x94]
  0018e5f4: mov r1,r5
  0018e5f6: blx 0x00074794
  0018e5fa: movs r0,#0x1
  0018e5fc: strb.w r0,[r11,#0x111]
  0018e600: strb.w r0,[r11,#0xce]
  0018e604: strb.w r0,[r8,#0x15]
  0018e608: add r0,sp,#0x78
  0018e60a: blx 0x0006ee30
  0018e60e: b.w 0x0018f164
  0018e612: ldrb.w r1,[r8,#0x1c]
  0018e616: cbnz r1,0x0018e66e
  0018e618: blx 0x00071b84
  0018e61c: blx 0x00077674
  0018e620: cbz r0,0x0018e66e
  0018e622: movs r4,#0x1
  0018e624: mov r0,r11
  0018e626: strb.w r4,[r11,#0x5d]
  0018e62a: blx 0x0007822c
  0018e62e: ldr.w r5,[r11,#0x94]
  0018e632: mov r6,r11
  0018e634: cbnz r5,0x0018e646
  0018e636: movs r0,#0x5c
  0018e638: blx 0x0006eb24
  0018e63c: mov r5,r0
  0018e63e: blx 0x00074584
  0018e642: str.w r5,[r6,#0x94]
  0018e646: ldr r0,[0x0018e7c0]
  0018e648: mov.w r1,#0x288
  0018e64c: add r0,pc
  0018e64e: ldr r0,[r0,#0x0]
  0018e650: ldr r0,[r0,#0x0]
  0018e652: blx 0x00072f70
  0018e656: mov r1,r0
  0018e658: mov r0,r5
  0018e65a: blx 0x00074794
  0018e65e: strb.w r4,[r6,#0x111]
  0018e662: strb.w r4,[r6,#0xce]
  0018e666: strb.w r4,[r8,#0x1c]
  0018e66a: b.w 0x0018f164
  0018e66e: ldrb.w r0,[r11,#0x1e6]
  0018e672: cbnz r0,0x0018e6cc
  0018e674: ldr.w r0,[r9,#0x0]
  0018e678: blx 0x00071770
  0018e67c: cmp r0,#0x5
  0018e67e: bne 0x0018e6cc
  0018e680: movs r4,#0x1
  0018e682: mov r0,r11
  0018e684: strb.w r4,[r11,#0x5d]
  0018e688: blx 0x0007822c
  0018e68c: ldr.w r5,[r11,#0x94]
  0018e690: mov r6,r11
  0018e692: cbnz r5,0x0018e6a4
  0018e694: movs r0,#0x5c
  0018e696: blx 0x0006eb24
  0018e69a: mov r5,r0
  0018e69c: blx 0x00074584
  0018e6a0: str.w r5,[r6,#0x94]
  0018e6a4: ldr r0,[0x0018e7c4]
  0018e6a6: movw r1,#0x26b
  0018e6aa: add r0,pc
  0018e6ac: ldr r0,[r0,#0x0]
  0018e6ae: ldr r0,[r0,#0x0]
  0018e6b0: blx 0x00072f70
  0018e6b4: mov r1,r0
  0018e6b6: mov r0,r5
  0018e6b8: blx 0x00074794
  0018e6bc: strb.w r4,[r6,#0x111]
  0018e6c0: strb.w r4,[r6,#0xce]
  0018e6c4: strb.w r4,[r6,#0x1e6]
  0018e6c8: b.w 0x0018f164
  0018e6cc: ldrb.w r0,[r8,#0x11]
  0018e6d0: cmp r0,#0x0
  0018e6d2: bne.w 0x0018e7da
  0018e6d6: ldr.w r0,[r10,#0x0]
  0018e6da: blx 0x00077adc
  0018e6de: cmp r0,#0x0
  0018e6e0: bne 0x0018e7d0
  0018e6e2: ldrb.w r0,[r11,#0x1e5]
  0018e6e6: cmp r0,#0x0
  0018e6e8: bne 0x0018e7d0
  0018e6ea: ldr.w r0,[r9,#0x0]
  0018e6ee: blx 0x00071770
  0018e6f2: cmp r0,#0x2
  0018e6f4: bne 0x0018e7d0
  0018e6f6: ldr.w r0,[r10,#0x0]
  0018e6fa: mov r5,r10
  0018e6fc: ldr r0,[r0,#0x0]
  0018e6fe: blx 0x000724f0
  0018e702: mov r4,r0
  0018e704: ldr.w r0,[r10,#0x0]
  0018e708: ldr r0,[r0,#0x0]
  0018e70a: blx 0x00072910
  0018e70e: cmp r4,r0
  0018e710: blt 0x0018e726
  0018e712: ldr.w r0,[r11,#0x7c]
  0018e716: movw r2,#0x9c41
  0018e71a: ldrd r0,r1,[r0,#0x8]
  0018e71e: subs r0,r0,r2
  0018e720: sbcs r0,r1,#0x0
  0018e724: blt 0x0018e7d0
  0018e726: movs r0,#0x1
  0018e728: strb.w r0,[r11,#0x5d]
  0018e72c: mov r0,r11
  0018e72e: blx 0x0007822c
  0018e732: ldr.w r0,[r11,#0x94]
  0018e736: mov r6,r11
  0018e738: cbnz r0,0x0018e74a
  0018e73a: movs r0,#0x5c
  0018e73c: blx 0x0006eb24
  0018e740: mov r5,r0
  0018e742: blx 0x00074584
  0018e746: str.w r5,[r6,#0x94]
  0018e74a: ldr r0,[0x0018e7c8]
  0018e74c: movw r1,#0x6bd
  0018e750: add r0,pc
  0018e752: ldr r0,[r0,#0x0]
  0018e754: ldr r0,[r0,#0x0]
  0018e756: blx 0x00072f70
  0018e75a: add r4,sp,#0x78
  0018e75c: mov r1,r0
  0018e75e: movs r2,#0x0
  0018e760: mov r0,r4
  0018e762: blx 0x0006f028
  0018e766: ldr r0,[0x0018e7cc]
  0018e768: add r0,pc
  0018e76a: ldr r0,[r0,#0x0]
  0018e76c: ldr r1,[r0,#0x0]
  0018e76e: add r0,sp,#0x118
  0018e770: mov r2,r4
  0018e772: blx 0x00077c08
  0018e776: add r0,sp,#0x78
  0018e778: add r1,sp,#0x118
  0018e77a: blx 0x0006f2b0
  0018e77e: add r0,sp,#0x118
  0018e780: blx 0x0006ee30
  0018e784: ldr.w r0,[r6,#0x94]
  0018e788: add r1,sp,#0x78
  0018e78a: blx 0x00074794
  0018e78e: movs r0,#0x1
  0018e790: strb.w r0,[r6,#0x111]
  0018e794: strb.w r0,[r6,#0xce]
  0018e798: strb.w r0,[r6,#0x1e5]
  0018e79c: b 0x0018e608
  0018e7d0: ldrb.w r0,[r8,#0x11]
  0018e7d4: cmp r0,#0x0
  0018e7d6: beq.w 0x0018f024
  0018e7da: ldrb.w r0,[r8,#0x12]
  0018e7de: cbnz r0,0x0018e842
  0018e7e0: ldr.w r0,[r10,#0x0]
  0018e7e4: blx 0x00077adc
  0018e7e8: cbz r0,0x0018e842
  0018e7ea: ldr.w r0,[r9,#0x0]
  0018e7ee: blx 0x00071770
  0018e7f2: cmp r0,#0x4
  0018e7f4: blt 0x0018e842
  0018e7f6: movs r4,#0x1
  0018e7f8: mov r0,r11
  0018e7fa: strb.w r4,[r11,#0x5d]
  0018e7fe: blx 0x0007822c
  0018e802: ldr.w r5,[r11,#0x94]
  0018e806: mov r6,r11
  0018e808: cbnz r5,0x0018e81a
  0018e80a: movs r0,#0x5c
  0018e80c: blx 0x0006eb24
  0018e810: mov r5,r0
  0018e812: blx 0x00074584
  0018e816: str.w r5,[r6,#0x94]
  0018e81a: ldr r0,[0x0018eb7c]
  0018e81c: movw r1,#0x26d
  0018e820: add r0,pc
  0018e822: ldr r0,[r0,#0x0]
  0018e824: ldr r0,[r0,#0x0]
  0018e826: blx 0x00072f70
  0018e82a: mov r1,r0
  0018e82c: mov r0,r5
  0018e82e: blx 0x00074794
  0018e832: strb.w r4,[r6,#0x111]
  0018e836: strb.w r4,[r6,#0xce]
  0018e83a: strb.w r4,[r8,#0x12]
  0018e83e: b.w 0x0018f164
  0018e842: ldr.w r0,[r9,#0x0]
  0018e846: str.w r9,[sp,#0x24]
  0018e84a: blx 0x00071770
  0018e84e: cmp r0,#0x2
  0018e850: itt eq
  0018e852: ldrb.eq.w r0,[r8,#0x37]
  0018e856: cmp.eq r0,#0x0
  0018e858: beq.w 0x0018eb98
  0018e85c: ldrb.w r0,[r8,#0x14]
  0018e860: cbnz r0,0x0018e8c4
  0018e862: ldr.w r0,[r11,#0x74]
  0018e866: blx 0x000785a4
  0018e86a: cbz r0,0x0018e8c4
  0018e86c: ldr r0,[sp,#0x24]
  0018e86e: ldr r0,[r0,#0x0]
  0018e870: blx 0x00071770
  0018e874: cmp r0,#0x7
  0018e876: blt 0x0018e8c4
  0018e878: movs r4,#0x1
  0018e87a: mov r0,r11
  0018e87c: strb.w r4,[r11,#0x5d]
  0018e880: blx 0x0007822c
  0018e884: ldr.w r5,[r11,#0x94]
  0018e888: mov r6,r11
  0018e88a: cbnz r5,0x0018e89c
  0018e88c: movs r0,#0x5c
  0018e88e: blx 0x0006eb24
  0018e892: mov r5,r0
  0018e894: blx 0x00074584
  0018e898: str.w r5,[r6,#0x94]
  0018e89c: ldr r0,[0x0018eb80]
  0018e89e: mov.w r1,#0x27c
  0018e8a2: add r0,pc
  0018e8a4: ldr r0,[r0,#0x0]
  0018e8a6: ldr r0,[r0,#0x0]
  0018e8a8: blx 0x00072f70
  0018e8ac: mov r1,r0
  0018e8ae: mov r0,r5
  0018e8b0: blx 0x00074794
  0018e8b4: strb.w r4,[r6,#0x111]
  0018e8b8: strb.w r4,[r6,#0xce]
  0018e8bc: strb.w r4,[r8,#0x14]
  0018e8c0: b.w 0x0018f164
  0018e8c4: ldrb.w r0,[r8,#0x23]
  0018e8c8: str.w r11,[sp,#0x20]
  0018e8cc: cbnz r0,0x0018e906
  0018e8ce: ldr r0,[sp,#0x24]
  0018e8d0: ldr r0,[r0,#0x0]
  0018e8d2: blx 0x000723d0
  0018e8d6: cbnz r0,0x0018e906
  0018e8d8: ldr r0,[sp,#0x24]
  0018e8da: ldr r0,[r0,#0x0]
  0018e8dc: blx 0x00071a10
  0018e8e0: blx 0x00071950
  0018e8e4: cmp r0,#0x2
  0018e8e6: bne 0x0018e906
  0018e8e8: ldr.w r9,[sp,#0x20]
  0018e8ec: movs r1,#0x5
  0018e8ee: movs r2,#0x0
  0018e8f0: ldr.w r0,[r9,#0x78]
  0018e8f4: blx 0x00072754
  0018e8f8: movs r0,#0x1
  0018e8fa: strb.w r0,[r8,#0x23]
  0018e8fe: strb.w r0,[r9,#0xe0]
  0018e902: b.w 0x0018f164
  0018e906: ldrb.w r0,[r8,#0x23]
  0018e90a: cbz r0,0x0018e94c
  0018e90c: ldr r0,[sp,#0x20]
  0018e90e: ldrb.w r1,[r8,#0x24]
  0018e912: ldrb.w r0,[r0,#0xe0]
  0018e916: orrs r0,r1
  0018e918: lsls r0,r0,#0x18
  0018e91a: bne 0x0018e94c
  0018e91c: ldr r0,[sp,#0x24]
  0018e91e: ldr r0,[r0,#0x0]
  0018e920: blx 0x000723d0
  0018e924: cbnz r0,0x0018e94c
  0018e926: ldr r0,[sp,#0x24]
  0018e928: ldr r0,[r0,#0x0]
  0018e92a: blx 0x00071a10
  0018e92e: blx 0x00071950
  0018e932: cmp r0,#0x2
  0018e934: bne 0x0018e94c
  0018e936: ldr r0,[sp,#0x20]
  0018e938: movs r1,#0x6
  0018e93a: movs r2,#0x0
  0018e93c: ldr r0,[r0,#0x78]
  0018e93e: blx 0x00072754
  0018e942: movs r0,#0x1
  0018e944: strb.w r0,[r8,#0x24]
  0018e948: b.w 0x0018f164
  0018e94c: ldrb.w r0,[r8,#0x25]
  0018e950: cmp r0,#0x0
  0018e952: beq.w 0x0018ef58
  0018e956: ldrb.w r0,[r8,#0x28]
  0018e95a: cbnz r0,0x0018e9b6
  0018e95c: ldr.w r0,[r10,#0x0]
  0018e960: blx 0x0007834c
  0018e964: cbz r0,0x0018e9b6
  0018e966: ldr r5,[sp,#0x20]
  0018e968: movs r4,#0x1
  0018e96a: mov r0,r5
  0018e96c: strb.w r4,[r5,#0x5d]
  0018e970: blx 0x0007822c
  0018e974: ldr.w r5,[r5,#0x94]
  0018e978: cbnz r5,0x0018e98c
  0018e97a: movs r0,#0x5c
  0018e97c: blx 0x0006eb24
  0018e980: mov r5,r0
  0018e982: blx 0x00074584
  0018e986: ldr r0,[sp,#0x20]
  0018e988: str.w r5,[r0,#0x94]
  0018e98c: ldr r0,[0x0018eb84]
  0018e98e: movw r1,#0x257
  0018e992: add r0,pc
  0018e994: ldr r0,[r0,#0x0]
  0018e996: ldr r0,[r0,#0x0]
  0018e998: blx 0x00072f70
  0018e99c: mov r1,r0
  0018e99e: mov r0,r5
  0018e9a0: blx 0x00074794
  0018e9a4: ldr.w r9,[sp,#0x20]
  0018e9a8: strb.w r4,[r9,#0x111]
  0018e9ac: strb.w r4,[r9,#0xce]
  0018e9b0: strb.w r4,[r8,#0x28]
  0018e9b4: b 0x0018f164
  0018e9b6: ldrb.w r0,[r8,#0x29]
  0018e9ba: cmp r0,#0x0
  0018e9bc: bne 0x0018ea76
  0018e9be: ldr r0,[sp,#0x24]
  0018e9c0: ldr r0,[r0,#0x0]
  0018e9c2: blx 0x00071770
  0018e9c6: cmp r0,#0x5b
  0018e9c8: bne 0x0018e9ec
  0018e9ca: ldr r0,[sp,#0x24]
  0018e9cc: ldr r0,[r0,#0x0]
  0018e9ce: blx 0x00071c14
  0018e9d2: blx 0x00071824
  0018e9d6: cmp r0,#0x6e
  0018e9d8: bne 0x0018e9ec
  0018e9da: ldr r0,[sp,#0x20]
  0018e9dc: ldr r0,[r0,#0x78]
  0018e9de: blx 0x00071ef0
  0018e9e2: ldr r0,[r0,#0x4]
  0018e9e4: ldr r0,[r0,#0x10]
  0018e9e6: blx 0x00071f2c
  0018e9ea: cbnz r0,0x0018ea26
  0018e9ec: ldr r0,[sp,#0x24]
  0018e9ee: ldr r0,[r0,#0x0]
  0018e9f0: blx 0x00071770
  0018e9f4: cmp r0,#0x5b
  0018e9f6: beq 0x0018ea76
  0018e9f8: ldr r5,[sp,#0x24]
  0018e9fa: ldr r4,[r5,#0x0]
  0018e9fc: mov r0,r4
  0018e9fe: blx 0x00071c14
  0018ea02: blx 0x00071824
  0018ea06: mov r9,r0
  0018ea08: ldr r0,[r5,#0x0]
  0018ea0a: blx 0x00071770
  0018ea0e: mov r2,r0
  0018ea10: mov r0,r4
  0018ea12: mov r1,r9
  0018ea14: blx 0x000723dc
  0018ea18: vmov s0,r0
  0018ea1c: vcmpe.f32 s0,#0
  0018ea20: vmrs apsr,fpscr
  0018ea24: ble 0x0018ea76
  0018ea26: ldr r5,[sp,#0x20]
  0018ea28: movs r4,#0x1
  0018ea2a: mov r0,r5
  0018ea2c: strb.w r4,[r5,#0x5d]
  0018ea30: blx 0x0007822c
  0018ea34: ldr.w r5,[r5,#0x94]
  0018ea38: cbnz r5,0x0018ea4c
  0018ea3a: movs r0,#0x5c
  0018ea3c: blx 0x0006eb24
  0018ea40: mov r5,r0
  0018ea42: blx 0x00074584
  0018ea46: ldr r0,[sp,#0x20]
  0018ea48: str.w r5,[r0,#0x94]
  0018ea4c: ldr r0,[0x0018eb88]
  0018ea4e: mov.w r1,#0x258
  0018ea52: add r0,pc
  0018ea54: ldr r0,[r0,#0x0]
  0018ea56: ldr r0,[r0,#0x0]
  0018ea58: blx 0x00072f70
  0018ea5c: mov r1,r0
  0018ea5e: mov r0,r5
  0018ea60: blx 0x00074794
  0018ea64: ldr.w r9,[sp,#0x20]
  0018ea68: strb.w r4,[r9,#0x111]
  0018ea6c: strb.w r4,[r9,#0xce]
  0018ea70: strb.w r4,[r8,#0x29]
  0018ea74: b 0x0018f164
  0018ea76: ldrb.w r0,[r8,#0x2c]
  0018ea7a: cbnz r0,0x0018ead6
  0018ea7c: ldr.w r0,[r10,#0x0]
  0018ea80: blx 0x00077b24
  0018ea84: cbz r0,0x0018ead6
  0018ea86: ldr r5,[sp,#0x20]
  0018ea88: movs r4,#0x1
  0018ea8a: mov r0,r5
  0018ea8c: strb.w r4,[r5,#0x5d]
  0018ea90: blx 0x0007822c
  0018ea94: ldr.w r5,[r5,#0x94]
  0018ea98: cbnz r5,0x0018eaac
  0018ea9a: movs r0,#0x5c
  0018ea9c: blx 0x0006eb24
  0018eaa0: mov r5,r0
  0018eaa2: blx 0x00074584
  0018eaa6: ldr r0,[sp,#0x20]
  0018eaa8: str.w r5,[r0,#0x94]
  0018eaac: ldr r0,[0x0018eb8c]
  0018eaae: movw r1,#0x25a
  0018eab2: add r0,pc
  0018eab4: ldr r0,[r0,#0x0]
  0018eab6: ldr r0,[r0,#0x0]
  0018eab8: blx 0x00072f70
  0018eabc: mov r1,r0
  0018eabe: mov r0,r5
  0018eac0: blx 0x00074794
  0018eac4: ldr.w r9,[sp,#0x20]
  0018eac8: strb.w r4,[r9,#0x111]
  0018eacc: strb.w r4,[r9,#0xce]
  0018ead0: strb.w r4,[r8,#0x2c]
  0018ead4: b 0x0018f164
  0018ead6: ldrb.w r0,[r8,#0x2e]
  0018eada: cmp r0,#0x0
  0018eadc: bne.w 0x0018ebf0
  0018eae0: ldr r0,[sp,#0x24]
  0018eae2: ldr r0,[r0,#0x0]
  0018eae4: blx 0x00071770
  0018eae8: cmp r0,#0x5b
  0018eaea: bne.w 0x0018ebf0
  0018eaee: ldr r0,[sp,#0x24]
  0018eaf0: ldr r0,[r0,#0x0]
  0018eaf2: blx 0x00071c14
  0018eaf6: blx 0x00071824
  0018eafa: cmp r0,#0x6e
  0018eafc: bne 0x0018ebf0
  0018eafe: ldr r0,[sp,#0x20]
  0018eb00: ldr r0,[r0,#0x78]
  0018eb02: blx 0x00071ef0
  0018eb06: ldr r0,[r0,#0x4]
  0018eb08: ldr r0,[r0,#0x10]
  0018eb0a: blx 0x00071f2c
  0018eb0e: cmp r0,#0x0
  0018eb10: beq 0x0018ebf0
  0018eb12: ldr r4,[sp,#0x20]
  0018eb14: movs r0,#0x1
  0018eb16: strb.w r0,[r4,#0x5d]
  0018eb1a: mov r0,r4
  0018eb1c: blx 0x0007822c
  0018eb20: ldr.w r0,[r4,#0x94]
  0018eb24: cbnz r0,0x0018eb38
  0018eb26: movs r0,#0x5c
  0018eb28: blx 0x0006eb24
  0018eb2c: mov r5,r0
  0018eb2e: blx 0x00074584
  0018eb32: ldr r0,[sp,#0x20]
  0018eb34: str.w r5,[r0,#0x94]
  0018eb38: ldr r0,[0x0018eb90]
  0018eb3a: ldr r1,[0x0018eb94]
  0018eb3c: add r0,pc
  0018eb3e: add r1,pc
  0018eb40: ldr r0,[r0,#0x0]
  0018eb42: ldr r1,[r1,#0x0]
  0018eb44: ldr r4,[r0,#0x0]
  0018eb46: ldr r0,[r1,#0x0]
  0018eb48: movw r1,#0x25b
  0018eb4c: blx 0x00072f70
  0018eb50: add r5,sp,#0x78
  0018eb52: mov r2,r0
  0018eb54: mov r1,r4
  0018eb56: mov r0,r5
  0018eb58: blx 0x00077c08
  0018eb5c: ldr r0,[sp,#0x20]
  0018eb5e: ldr.w r0,[r0,#0x94]
  0018eb62: mov r1,r5
  0018eb64: blx 0x00074794
  0018eb68: ldr.w r9,[sp,#0x20]
  0018eb6c: movs r0,#0x1
  0018eb6e: strb.w r0,[r9,#0x111]
  0018eb72: strb.w r0,[r9,#0xce]
  0018eb76: strb.w r0,[r8,#0x2e]
  0018eb7a: b 0x0018e608
  0018eb98: ldr.w r0,[r10,#0x0]
  0018eb9c: blx 0x000785b0
  0018eba0: cmp r0,#0x0
  0018eba2: beq.w 0x0018e85c
  0018eba6: movs r4,#0x1
  0018eba8: mov r0,r11
  0018ebaa: strb.w r4,[r11,#0x5d]
  0018ebae: blx 0x0007822c
  0018ebb2: ldr.w r5,[r11,#0x94]
  0018ebb6: mov r6,r11
  0018ebb8: cbnz r5,0x0018ebca
  0018ebba: movs r0,#0x5c
  0018ebbc: blx 0x0006eb24
  0018ebc0: mov r5,r0
  0018ebc2: blx 0x00074584
  0018ebc6: str.w r5,[r6,#0x94]
  0018ebca: ldr r0,[0x0018ef40]
  0018ebcc: movw r1,#0x269
  0018ebd0: add r0,pc
  0018ebd2: ldr r0,[r0,#0x0]
  0018ebd4: ldr r0,[r0,#0x0]
  0018ebd6: blx 0x00072f70
  0018ebda: mov r1,r0
  0018ebdc: mov r0,r5
  0018ebde: blx 0x00074794
  0018ebe2: strb.w r4,[r6,#0x111]
  0018ebe6: strb.w r4,[r6,#0xce]
  0018ebea: strb.w r4,[r8,#0x37]
  0018ebee: b 0x0018f164
  0018ebf0: ldrb.w r0,[r8,#0x2f]
  0018ebf4: cmp r0,#0x0
  0018ebf6: bne 0x0018eca8
  0018ebf8: ldr r0,[sp,#0x24]
  0018ebfa: ldr r0,[r0,#0x0]
  0018ebfc: blx 0x00071770
  0018ec00: cmp r0,#0x5b
  0018ec02: bne 0x0018eca8
  0018ec04: ldr r0,[sp,#0x24]
  0018ec06: ldr r0,[r0,#0x0]
  0018ec08: blx 0x00071c14
  0018ec0c: blx 0x00071824
  0018ec10: cmp r0,#0x6e
  0018ec12: bne 0x0018eca8
  0018ec14: ldr.w r0,[r10,#0x0]
  0018ec18: blx 0x00075928
  0018ec1c: cmp r0,#0x0
  0018ec1e: beq 0x0018eca8
  0018ec20: ldr r0,[sp,#0x24]
  0018ec22: ldr r0,[r0,#0x0]
  0018ec24: ldr.w r0,[r0,#0x178]
  0018ec28: cmp r0,#0xa
  0018ec2a: bne 0x0018eca8
  0018ec2c: ldr r0,[sp,#0x20]
  0018ec2e: ldr r0,[r0,#0x78]
  0018ec30: blx 0x00071ef0
  0018ec34: ldr r0,[r0,#0x4]
  0018ec36: ldr r0,[r0,#0x18]
  0018ec38: blx 0x00071f2c
  0018ec3c: cbz r0,0x0018eca8
  0018ec3e: ldr r4,[sp,#0x20]
  0018ec40: movs r0,#0x1
  0018ec42: strb.w r0,[r4,#0x5d]
  0018ec46: mov r0,r4
  0018ec48: blx 0x0007822c
  0018ec4c: ldr.w r0,[r4,#0x94]
  0018ec50: cbnz r0,0x0018ec64
  0018ec52: movs r0,#0x5c
  0018ec54: blx 0x0006eb24
  0018ec58: mov r5,r0
  0018ec5a: blx 0x00074584
  0018ec5e: ldr r0,[sp,#0x20]
  0018ec60: str.w r5,[r0,#0x94]
  0018ec64: ldr r0,[0x0018ef44]
  0018ec66: ldr r1,[0x0018ef48]
  0018ec68: add r0,pc
  0018ec6a: add r1,pc
  0018ec6c: ldr r0,[r0,#0x0]
  0018ec6e: ldr r1,[r1,#0x0]
  0018ec70: ldr r4,[r0,#0x0]
  0018ec72: ldr r0,[r1,#0x0]
  0018ec74: movw r1,#0x25e
  0018ec78: blx 0x00072f70
  0018ec7c: add r5,sp,#0x78
  0018ec7e: mov r2,r0
  0018ec80: mov r1,r4
  0018ec82: mov r0,r5
  0018ec84: blx 0x00077c08
  0018ec88: ldr r0,[sp,#0x20]
  0018ec8a: ldr.w r0,[r0,#0x94]
  0018ec8e: mov r1,r5
  0018ec90: blx 0x00074794
  0018ec94: ldr.w r9,[sp,#0x20]
  0018ec98: movs r0,#0x1
  0018ec9a: strb.w r0,[r9,#0x111]
  0018ec9e: strb.w r0,[r9,#0xce]
  0018eca2: strb.w r0,[r8,#0x2f]
  0018eca6: b 0x0018e608
  0018eca8: ldrb.w r0,[r8,#0x30]
  0018ecac: cbnz r0,0x0018ece2
  0018ecae: ldr r0,[sp,#0x24]
  0018ecb0: ldr r0,[r0,#0x0]
  0018ecb2: blx 0x00071770
  0018ecb6: cmp r0,#0x5c
  0018ecb8: bne 0x0018ece2
  0018ecba: ldr r0,[sp,#0x24]
  0018ecbc: ldr r0,[r0,#0x0]
  0018ecbe: blx 0x00071c14
  0018ecc2: blx 0x00071824
  0018ecc6: cmp r0,#0x71
  0018ecc8: bne 0x0018ece2
  0018ecca: ldr.w r0,[r10,#0x0]
  0018ecce: blx 0x00075928
  0018ecd2: cbz r0,0x0018ece2
  0018ecd4: ldr r0,[sp,#0x24]
  0018ecd6: ldr r0,[r0,#0x0]
  0018ecd8: ldr.w r0,[r0,#0x178]
  0018ecdc: cmp r0,#0x0
  0018ecde: beq.w 0x0018f280
  0018ece2: ldr r0,[sp,#0x28]
  0018ece4: ldrb.w r0,[r0,#0x35]
  0018ece8: cmp r0,#0x0
  0018ecea: beq 0x0018ed9c
  0018ecec: ldrb.w r0,[r8,#0x26]
  0018ecf0: cmp r0,#0x0
  0018ecf2: bne 0x0018ed9c
  0018ecf4: ldr r0,[sp,#0x20]
  0018ecf6: ldrb.w r0,[r0,#0x5e]
  0018ecfa: cmp r0,#0x0
  0018ecfc: bne 0x0018ed9c
  0018ecfe: ldr.w r0,[r10,#0x0]
  0018ed02: blx 0x00077adc
  0018ed06: cmp r0,#0x0
  0018ed08: bne 0x0018ed9c
  0018ed0a: ldr r0,[sp,#0x24]
  0018ed0c: ldr r0,[r0,#0x0]
  0018ed0e: blx 0x00072850
  0018ed12: blx 0x00072868
  0018ed16: cmp r0,#0x0
  0018ed18: beq 0x0018ed9c
  0018ed1a: ldr.w r0,[r10,#0x0]
  0018ed1e: blx 0x0007681c
  0018ed22: cbnz r0,0x0018ed9c
  0018ed24: ldr r0,[sp,#0x24]
  0018ed26: ldr r0,[r0,#0x0]
  0018ed28: blx 0x00073c48
  0018ed2c: cbz r0,0x0018ed9c
  0018ed2e: ldr r4,[sp,#0x20]
  0018ed30: movs r0,#0x1
  0018ed32: strb.w r0,[r4,#0x5d]
  0018ed36: mov r0,r4
  0018ed38: blx 0x0007822c
  0018ed3c: ldr.w r0,[r4,#0x8c]
  0018ed40: cbz r0,0x0018ed52
  0018ed42: blx 0x00075010
  0018ed46: blx 0x0006eb48
  0018ed4a: ldr r1,[sp,#0x20]
  0018ed4c: movs r0,#0x0
  0018ed4e: str.w r0,[r1,#0x8c]
  0018ed52: movs r0,#0x74
  0018ed54: blx 0x0006eb24
  0018ed58: mov r5,r0
  0018ed5a: blx 0x00075544
  0018ed5e: ldr r0,[sp,#0x20]
  0018ed60: str.w r5,[r0,#0x8c]
  0018ed64: movs r0,#0x78
  0018ed66: blx 0x0006eb24
  0018ed6a: mov r5,r0
  0018ed6c: movs r1,#0xa0
  0018ed6e: movs r2,#0x0
  0018ed70: mov.w r3,#0xffffffff
  0018ed74: blx 0x00073834
  0018ed78: ldr r6,[sp,#0x20]
  0018ed7a: mov r1,r5
  0018ed7c: movs r2,#0x1
  0018ed7e: movs r3,#0x2e
  0018ed80: movs r4,#0x1
  0018ed82: ldr.w r0,[r6,#0x8c]
  0018ed86: blx 0x00075550
  0018ed8a: ldr r5,[sp,#0x24]
  0018ed8c: strb.w r4,[r6,#0x111]
  0018ed90: strb.w r4,[r6,#0x5e]
  0018ed94: ldr r0,[r5,#0x0]
  0018ed96: strb.w r4,[r8,#0x26]
  0018ed9a: b 0x0018ee56
  0018ed9c: ldr r0,[sp,#0x28]
  0018ed9e: ldrb.w r0,[r0,#0x37]
  0018eda2: cmp r0,#0x0
  0018eda4: beq 0x0018ee66
  0018eda6: ldrb.w r0,[r8,#0x31]
  0018edaa: cmp r0,#0x0
  0018edac: bne 0x0018ee66
  0018edae: ldr r0,[sp,#0x20]
  0018edb0: ldrb.w r0,[r0,#0x5e]
  0018edb4: cmp r0,#0x0
  0018edb6: bne 0x0018ee66
  0018edb8: ldr.w r0,[r10,#0x0]
  0018edbc: blx 0x00077adc
  0018edc0: cmp r0,#0x0
  0018edc2: bne 0x0018ee66
  0018edc4: ldr r0,[sp,#0x24]
  0018edc6: ldr r0,[r0,#0x0]
  0018edc8: blx 0x00072850
  0018edcc: blx 0x00072868
  0018edd0: cmp r0,#0x0
  0018edd2: beq 0x0018ee66
  0018edd4: ldr.w r0,[r10,#0x0]
  0018edd8: blx 0x0007681c
  0018eddc: cmp r0,#0x0
  0018edde: bne 0x0018ee66
  0018ede0: ldr r0,[sp,#0x24]
  0018ede2: ldr r0,[r0,#0x0]
  0018ede4: blx 0x000719a4
  0018ede8: cbz r0,0x0018ee66
  0018edea: ldr r4,[sp,#0x20]
  0018edec: movs r0,#0x1
  0018edee: strb.w r0,[r4,#0x5d]
  0018edf2: mov r0,r4
  0018edf4: blx 0x0007822c
  0018edf8: ldr.w r0,[r4,#0x8c]
  0018edfc: cbz r0,0x0018ee0e
  0018edfe: blx 0x00075010
  0018ee02: blx 0x0006eb48
  0018ee06: ldr r1,[sp,#0x20]
  0018ee08: movs r0,#0x0
  0018ee0a: str.w r0,[r1,#0x8c]
  0018ee0e: movs r0,#0x74
  0018ee10: blx 0x0006eb24
  0018ee14: mov r5,r0
  0018ee16: blx 0x00075544
  0018ee1a: ldr r0,[sp,#0x20]
  0018ee1c: str.w r5,[r0,#0x8c]
  0018ee20: movs r0,#0x78
  0018ee22: blx 0x0006eb24
  0018ee26: mov r5,r0
  0018ee28: movs r1,#0xa0
  0018ee2a: movs r2,#0x0
  0018ee2c: mov.w r3,#0xffffffff
  0018ee30: blx 0x00073834
  0018ee34: ldr r6,[sp,#0x20]
  0018ee36: mov r1,r5
  0018ee38: movs r2,#0x1
  0018ee3a: movs r3,#0x55
  0018ee3c: movs r4,#0x1
  0018ee3e: ldr.w r0,[r6,#0x8c]
  0018ee42: blx 0x00075550
  0018ee46: ldr r5,[sp,#0x24]
  0018ee48: strb.w r4,[r6,#0x111]
  0018ee4c: strb.w r4,[r6,#0x5e]
  0018ee50: ldr r0,[r5,#0x0]
  0018ee52: strb.w r4,[r8,#0x31]
  0018ee56: movs r1,#0x1
  0018ee58: blx 0x00075070
  0018ee5c: ldr r0,[r5,#0x0]
  0018ee5e: movs r1,#0x1
  0018ee60: blx 0x00075070
  0018ee64: b 0x0018f164
  0018ee66: ldr r0,[sp,#0x24]
  0018ee68: ldr r0,[r0,#0x0]
  0018ee6a: blx 0x00071998
  0018ee6e: cbz r0,0x0018ee8c
  0018ee70: ldr r0,[sp,#0x24]
  0018ee72: ldr r0,[r0,#0x0]
  0018ee74: ldrb.w r1,[r0,#0x110]
  0018ee78: cbnz r1,0x0018ee8c
  0018ee7a: ldrb.w r0,[r0,#0x111]
  0018ee7e: cbnz r0,0x0018ee8c
  0018ee80: ldr r0,[sp,#0x20]
  0018ee82: ldrb.w r0,[r0,#0xd4]
  0018ee86: cmp r0,#0x0
  0018ee88: beq.w 0x0018f1b8
  0018ee8c: ldr r0,[sp,#0x24]
  0018ee8e: ldr.w r11,[sp,#0x20]
  0018ee92: ldr r0,[r0,#0x0]
  0018ee94: ldr.w r1,[r0,#0x114]
  0018ee98: cmp r1,#0x0
  0018ee9a: bne.w 0x0018dc40
  0018ee9e: blx 0x000723d0
  0018eea2: cmp r0,#0x0
  0018eea4: bne.w 0x0018dc40
  0018eea8: ldr r0,[sp,#0x24]
  0018eeaa: ldr r0,[r0,#0x0]
  0018eeac: blx 0x00071c14
  0018eeb0: blx 0x00071824
  0018eeb4: cmp r0,#0x6c
  0018eeb6: bne.w 0x0018dc40
  0018eeba: movs r0,#0x1
  0018eebc: strb.w r0,[r11,#0x5d]
  0018eec0: mov r0,r11
  0018eec2: blx 0x0007822c
  0018eec6: ldr.w r0,[r11,#0x8c]
  0018eeca: cbz r0,0x0018eedc
  0018eecc: blx 0x00075010
  0018eed0: blx 0x0006eb48
  0018eed4: ldr r1,[sp,#0x20]
  0018eed6: movs r0,#0x0
  0018eed8: str.w r0,[r1,#0x8c]
  0018eedc: movs r0,#0x74
  0018eede: blx 0x0006eb24
  0018eee2: mov r5,r0
  0018eee4: ldr r0,[0x0018ef4c]
  0018eee6: add r0,pc
  0018eee8: ldr r6,[r0,#0x0]
  0018eeea: ldr r0,[r6,#0x0]
  0018eeec: movw r1,#0x1c9
  0018eef0: blx 0x00072f70
  0018eef4: mov r4,r0
  0018eef6: ldr r0,[r6,#0x0]
  0018eef8: movw r1,#0x641
  0018eefc: blx 0x00072f70
  0018ef00: ldr r3,[0x0018ef50]
  0018ef02: mov r2,r0
  0018ef04: add r3,pc
  0018ef06: mov r0,r5
  0018ef08: mov r1,r4
  0018ef0a: blx 0x00074f20
  0018ef0e: ldr r0,[0x0018ef54]
  0018ef10: movs r1,#0x0
  0018ef12: ldr r4,[sp,#0x20]
  0018ef14: movs r2,#0x0
  0018ef16: add r0,pc
  0018ef18: movs r3,#0x0
  0018ef1a: ldr r0,[r0,#0x0]
  0018ef1c: str.w r5,[r4,#0x8c]
  0018ef20: str r1,[sp,#0x0]
  0018ef22: movw r1,#0x5bc
  0018ef26: ldr r0,[r0,#0x0]
  0018ef28: blx 0x00071548
  0018ef2c: ldr r1,[sp,#0x24]
  0018ef2e: movs r0,#0x1
  0018ef30: strb.w r0,[r4,#0x111]
  0018ef34: strb.w r0,[r4,#0x5e]
  0018ef38: ldr r1,[r1,#0x0]
  0018ef3a: str.w r0,[r1,#0x114]
  0018ef3e: b 0x0018f164
  0018ef58: ldr r0,[sp,#0x24]
  0018ef5a: ldr r0,[r0,#0x0]
  0018ef5c: blx 0x00071a58
  0018ef60: movs r1,#0xd
  0018ef62: blx 0x0007228c
  0018ef66: cmp r0,#0x0
  0018ef68: beq.w 0x0018e956
  0018ef6c: ldr.w r0,[r10,#0x0]
  0018ef70: blx 0x00077adc
  0018ef74: cmp r0,#0x0
  0018ef76: bne.w 0x0018e956
  0018ef7a: ldr r0,[sp,#0x24]
  0018ef7c: ldr r0,[r0,#0x0]
  0018ef7e: blx 0x00072850
  0018ef82: blx 0x00072868
  0018ef86: cmp r0,#0x0
  0018ef88: beq.w 0x0018e956
  0018ef8c: ldr r0,[sp,#0x24]
  0018ef8e: ldr r0,[r0,#0x0]
  0018ef90: blx 0x00071770
  0018ef94: cmp r0,#0x18
  0018ef96: bne.w 0x0018e956
  0018ef9a: ldr r4,[sp,#0x20]
  0018ef9c: movs r0,#0x1
  0018ef9e: strb.w r0,[r4,#0x5d]
  0018efa2: mov r0,r4
  0018efa4: blx 0x0007822c
  0018efa8: ldr.w r0,[r4,#0x8c]
  0018efac: cbz r0,0x0018efbe
  0018efae: blx 0x00075010
  0018efb2: blx 0x0006eb48
  0018efb6: ldr r1,[sp,#0x20]
  0018efb8: movs r0,#0x0
  0018efba: str.w r0,[r1,#0x8c]
  0018efbe: movs r0,#0x74
  0018efc0: blx 0x0006eb24
  0018efc4: mov r5,r0
  0018efc6: ldr r0,[0x0018f2f4]
  0018efc8: add r0,pc
  0018efca: ldr r6,[r0,#0x0]
  0018efcc: ldr r0,[r6,#0x0]
  0018efce: mov.w r1,#0x778
  0018efd2: blx 0x00072f70
  0018efd6: mov r4,r0
  0018efd8: ldr r0,[r6,#0x0]
  0018efda: movw r1,#0x643
  0018efde: blx 0x00072f70
  0018efe2: mov r2,r0
  0018efe4: ldr r0,[0x0018f2f8]
  0018efe6: add r0,pc
  0018efe8: add.w r3,r0,#0x28
  0018efec: mov r0,r5
  0018efee: mov r1,r4
  0018eff0: blx 0x00074f20
  0018eff4: ldr r0,[0x0018f2fc]
  0018eff6: movs r1,#0x0
  0018eff8: ldr.w r9,[sp,#0x20]
  0018effc: movs r2,#0x0
  0018effe: add r0,pc
  0018f000: movs r3,#0x0
  0018f002: ldr r0,[r0,#0x0]
  0018f004: str.w r5,[r9,#0x8c]
  0018f008: str r1,[sp,#0x0]
  0018f00a: movw r1,#0x1cf
  0018f00e: ldr r0,[r0,#0x0]
  0018f010: blx 0x00071548
  0018f014: movs r0,#0x1
  0018f016: strb.w r0,[r9,#0x111]
  0018f01a: strb.w r0,[r9,#0x5e]
  0018f01e: strb.w r0,[r8,#0x25]
  0018f022: b 0x0018f164
  0018f024: ldr.w r0,[r10,#0x0]
  0018f028: blx 0x00077adc
  0018f02c: cmp r0,#0x0
  0018f02e: beq.w 0x0018e7da
  0018f032: ldrb.w r0,[r8,#0x11]
  0018f036: cmp r0,#0x0
  0018f038: bne.w 0x0018e7da
  0018f03c: movs r4,#0x1
  0018f03e: mov r0,r11
  0018f040: strb.w r4,[r11,#0x5d]
  0018f044: blx 0x0007822c
  0018f048: ldr.w r5,[r11,#0x94]
  0018f04c: mov r6,r11
  0018f04e: cbnz r5,0x0018f060
  0018f050: movs r0,#0x5c
  0018f052: blx 0x0006eb24
  0018f056: mov r5,r0
  0018f058: blx 0x00074584
  0018f05c: str.w r5,[r6,#0x94]
  0018f060: ldr r0,[0x0018f300]
  0018f062: mov.w r1,#0x26c
  0018f066: add r0,pc
  0018f068: ldr r0,[r0,#0x0]
  0018f06a: ldr r0,[r0,#0x0]
  0018f06c: blx 0x00072f70
  0018f070: mov r1,r0
  0018f072: mov r0,r5
  0018f074: blx 0x00074794
  0018f078: strb.w r4,[r6,#0x111]
  0018f07c: strb.w r4,[r6,#0xce]
  0018f080: strb.w r4,[r8,#0x11]
  0018f084: b 0x0018f164
  0018f086: vldr.32 s2,[pc,#0x27c]
  0018f08a: mov r11,r1
  0018f08c: vcmpe.f32 s0,s2
  0018f090: vmrs apsr,fpscr
  0018f094: itt mi
  0018f096: ldr.mi r0,[0x0018f304]
  0018f098: str.w.mi r0,[r11,#0x11c]
  0018f09c: vldr.32 s0,[pc,#0x268]
  0018f0a0: movs r3,#0x0
  0018f0a2: vldr.32 s4,[r11,#0x11c]
  0018f0a6: vldr.32 s2,[r11,#0x118]
  0018f0aa: vmul.f32 s4,s4,s0
  0018f0ae: ldr.w r0,[r11,#0xf4]
  0018f0b2: vmul.f32 s0,s2,s0
  0018f0b6: vmov r1,s4
  0018f0ba: vmov r2,s0
  0018f0be: blx 0x00078508
  0018f0c2: ldr.w r1,[r11,#0x40]
  0018f0c6: ldr.w r0,[r11,#0xf4]
  0018f0ca: blx 0x000721a8
  0018f0ce: ldr.w r0,[r11,#0x88]
  0018f0d2: blx 0x000785bc
  0018f0d6: cbz r0,0x0018f0ea
  0018f0d8: ldr r0,[0x0018f30c]
  0018f0da: ldr.w r2,[r11,#0x40]
  0018f0de: add r0,pc
  0018f0e0: ldr r0,[r0,#0x0]
  0018f0e2: asrs r3,r2,#0x1f
  0018f0e4: ldr r0,[r0,#0x0]
  0018f0e6: blx 0x00075454
  0018f0ea: ldrb.w r0,[r11,#0x54]
  0018f0ee: cbz r0,0x0018f164
  0018f0f0: ldrb.w r0,[r11,#0xc8]
  0018f0f4: cbnz r0,0x0018f164
  0018f0f6: ldrb.w r0,[r11,#0xc5]
  0018f0fa: cbnz r0,0x0018f164
  0018f0fc: ldrb.w r0,[r11,#0xc6]
  0018f100: cbnz r0,0x0018f164
  0018f102: ldrb.w r0,[r11,#0xce]
  0018f106: cbz r0,0x0018f116
  0018f108: ldr.w r1,[r11,#0x40]
  0018f10c: ldr.w r0,[r11,#0x94]
  0018f110: blx 0x00074eb4
  0018f114: b 0x0018f164
  0018f116: ldrb.w r0,[r11,#0xd6]
  0018f11a: cbnz r0,0x0018f164
  0018f11c: ldrb.w r0,[r11,#0xc7]
  0018f120: cbz r0,0x0018f142
  0018f122: ldr r0,[0x0018f310]
  0018f124: ldr.w r2,[r11,#0x40]
  0018f128: add r0,pc
  0018f12a: ldr r0,[r0,#0x0]
  0018f12c: asrs r3,r2,#0x1f
  0018f12e: ldr r0,[r0,#0x0]
  0018f130: blx 0x00075454
  0018f134: ldr.w r1,[r11,#0x40]
  0018f138: ldr.w r0,[r11,#0x90]
  0018f13c: blx 0x000754c0
  0018f140: b 0x0018f164
  0018f142: ldrb.w r0,[r11,#0x5e]
  0018f146: cbz r0,0x0018f164
  0018f148: ldr r0,[0x0018f314]
  0018f14a: ldr.w r1,[r11,#0x40]
  0018f14e: add r0,pc
  0018f150: ldr r0,[r0,#0x0]
  0018f152: ldr r0,[r0,#0x0]
  0018f154: blx 0x00075448
  0018f158: ldr.w r1,[r11,#0x40]
  0018f15c: ldr.w r0,[r11,#0x8c]
  0018f160: blx 0x000754b4
  0018f164: ldr r0,[sp,#0x124]
  0018f166: ldr r1,[sp,#0x2c]
  0018f168: ldr r1,[r1,#0x0]
  0018f16a: subs r0,r1,r0
  0018f16c: itttt eq
  0018f16e: add.eq sp,#0x128
  0018f170: vpop.eq {d8,d9}
  0018f174: add.eq sp,#0x4
  0018f176: pop.eq.w {r8,r9,r10,r11}
  0018f17a: it eq
  0018f17c: pop.eq {r4,r5,r6,r7,pc}
  0018f17e: blx 0x0006e824
  0018f182: ldr r0,[0x0018f318]
  0018f184: movs r1,#0x2
  0018f186: add r0,pc
  0018f188: ldr r0,[r0,#0x0]
  0018f18a: ldr r0,[r0,#0x0]
  0018f18c: blx 0x00071848
  0018f190: mov r4,r0
  0018f192: ldr.w r0,[r9,#0x0]
  0018f196: blx 0x00071770
  0018f19a: movw r2,#0xc5e
  0018f19e: ldr.w r1,[r11,#0x78]
  0018f1a2: cmp r0,#0x7a
  0018f1a4: it eq
  0018f1a6: movw.eq r2,#0xc5c
  0018f1aa: add r2,r4
  0018f1ac: mov r0,r1
  0018f1ae: movs r1,#0x1c
  0018f1b0: blx 0x00072754
  0018f1b4: b.w 0x0018e26c
  0018f1b8: ldr r0,[sp,#0x20]
  0018f1ba: ldr r0,[r0,#0x78]
  0018f1bc: blx 0x00071ef0
  0018f1c0: cmp r0,#0x0
  0018f1c2: beq.w 0x0018ee8c
  0018f1c6: ldr r0,[sp,#0x20]
  0018f1c8: ldr r0,[r0,#0x78]
  0018f1ca: blx 0x00071ef0
  0018f1ce: ldr r0,[r0,#0x4]
  0018f1d0: ldr r0,[r0,#0x0]
  0018f1d2: blx 0x00071f2c
  0018f1d6: cmp r0,#0x0
  0018f1d8: beq.w 0x0018ee8c
  0018f1dc: ldr r4,[sp,#0x20]
  0018f1de: movs r0,#0x1
  0018f1e0: strb.w r0,[r4,#0x5d]
  0018f1e4: mov r0,r4
  0018f1e6: blx 0x0007822c
  0018f1ea: ldr.w r0,[r4,#0x94]
  0018f1ee: cbnz r0,0x0018f202
  0018f1f0: movs r0,#0x5c
  0018f1f2: blx 0x0006eb24
  0018f1f6: mov r5,r0
  0018f1f8: blx 0x00074584
  0018f1fc: ldr r0,[sp,#0x20]
  0018f1fe: str.w r5,[r0,#0x94]
  0018f202: ldr r1,[sp,#0x20]
  0018f204: movs r0,#0x0
  0018f206: str.w r0,[r1,#0xd0]
  0018f20a: ldr r0,[sp,#0x24]
  0018f20c: ldr r0,[r0,#0x0]
  0018f20e: blx 0x00071a58
  0018f212: blx 0x00073750
  0018f216: mov r5,r0
  0018f218: cbz r0,0x0018f24c
  0018f21a: movs r4,#0x0
  0018f21c: b 0x0018f246
  0018f21e: ldr r0,[r5,#0x4]
  0018f220: ldr.w r0,[r0,r4,lsl #0x2]
  0018f224: cbz r0,0x0018f244
  0018f226: blx 0x0007183c
  0018f22a: mov r8,r0
  0018f22c: ldr r0,[r5,#0x4]
  0018f22e: ldr.w r0,[r0,r4,lsl #0x2]
  0018f232: blx 0x00071944
  0018f236: ldr r2,[sp,#0x20]
  0018f238: ldr.w r1,[r2,#0xd0]
  0018f23c: mla r0,r0,r8,r1
  0018f240: str.w r0,[r2,#0xd0]
  0018f244: adds r4,#0x1
  0018f246: ldr r0,[r5,#0x0]
  0018f248: cmp r4,r0
  0018f24a: bcc 0x0018f21e
  0018f24c: ldr r0,[sp,#0x20]
  0018f24e: ldr.w r0,[r0,#0xd0]
  0018f252: cbnz r0,0x0018f25c
  0018f254: ldr r1,[sp,#0x20]
  0018f256: movs r0,#0x64
  0018f258: str.w r0,[r1,#0xd0]
  0018f25c: ldr r1,[sp,#0x28]
  0018f25e: vldr.32 s0,[r1,#0x2c]
  0018f262: vcmpe.f32 s0,#0
  0018f266: vmrs apsr,fpscr
  0018f26a: bls 0x0018f2ec
  0018f26c: vmov.f32 s2,0x3f000000
  0018f270: vcmpe.f32 s0,s2
  0018f274: vmrs apsr,fpscr
  0018f278: bne 0x0018f31c
  0018f27a: vldr.32 s16,[pc,#0x2e4]
  0018f27e: b 0x0018f332
  0018f280: ldr r4,[sp,#0x20]
  0018f282: movs r0,#0x1
  0018f284: strb.w r0,[r4,#0x5d]
  0018f288: mov r0,r4
  0018f28a: blx 0x0007822c
  0018f28e: ldr.w r0,[r4,#0x94]
  0018f292: cbnz r0,0x0018f2a6
  0018f294: movs r0,#0x5c
  0018f296: blx 0x0006eb24
  0018f29a: mov r5,r0
  0018f29c: blx 0x00074584
  0018f2a0: ldr r0,[sp,#0x20]
  0018f2a2: str.w r5,[r0,#0x94]
  0018f2a6: ldr r0,[0x0018f56c]
  0018f2a8: ldr r1,[0x0018f570]
  0018f2aa: add r0,pc
  0018f2ac: add r1,pc
  0018f2ae: ldr r0,[r0,#0x0]
  0018f2b0: ldr r1,[r1,#0x0]
  0018f2b2: ldr r4,[r0,#0x0]
  0018f2b4: ldr r0,[r1,#0x0]
  0018f2b6: movw r1,#0x261
  0018f2ba: blx 0x00072f70
  0018f2be: add r5,sp,#0x78
  0018f2c0: mov r2,r0
  0018f2c2: mov r1,r4
  0018f2c4: mov r0,r5
  0018f2c6: blx 0x00077c08
  0018f2ca: ldr r0,[sp,#0x20]
  0018f2cc: ldr.w r0,[r0,#0x94]
  0018f2d0: mov r1,r5
  0018f2d2: blx 0x00074794
  0018f2d6: ldr.w r9,[sp,#0x20]
  0018f2da: movs r0,#0x1
  0018f2dc: strb.w r0,[r9,#0x111]
  0018f2e0: strb.w r0,[r9,#0xce]
  0018f2e4: strb.w r0,[r8,#0x30]
  0018f2e8: b.w 0x0018e608
  0018f2ec: vldr.32 s16,[pc,#0x26c]
  0018f2f0: b 0x0018f332
  0018f31c: vmov.f32 s2,0x3f800000
  0018f320: adr r1,[0x18f564]
  0018f322: vcmpe.f32 s0,s2
  0018f326: vmrs apsr,fpscr
  0018f32a: it eq
  0018f32c: add.eq r1,#0x4
  0018f32e: vldr.32 s16,[r1]
  0018f332: vmov s0,r0
  0018f336: vcvt.f32.s32 s0,s0
  0018f33a: vmul.f32 s0,s16,s0
  0018f33e: vcvt.s32.f32 s0,s0
  0018f342: ldr r0,[0x0018f574]
  0018f344: ldr r4,[sp,#0x20]
  0018f346: add r0,pc
  0018f348: ldr r1,[sp,#0x24]
  0018f34a: ldr r0,[r0,#0x0]
  0018f34c: vstr.32 s0,[r4,#0xd0]
  0018f350: ldr r0,[r0,#0x0]
  0018f352: ldr r5,[r1,#0x0]
  0018f354: mov.w r1,#0x1c0
  0018f358: blx 0x00072f70
  0018f35c: mov r1,r0
  0018f35e: add r0,sp,#0x6c
  0018f360: movs r2,#0x0
  0018f362: blx 0x0006f028
  0018f366: ldr r0,[0x0018f578]
  0018f368: ldr.w r2,[r4,#0xd0]
  0018f36c: add r0,pc
  0018f36e: ldr r0,[r0,#0x0]
  0018f370: ldr r1,[r0,#0x0]
  0018f372: add r0,sp,#0x60
  0018f374: blx 0x00074e54
  0018f378: ldr r1,[0x0018f57c]
  0018f37a: add r1,pc
  0018f37c: add r0,sp,#0x54
  0018f37e: movs r2,#0x0
  0018f380: blx 0x0006ee18
  0018f384: str r0,[sp,#0x0]
  0018f386: add r0,sp,#0x78
  0018f388: add r2,sp,#0x6c
  0018f38a: add r3,sp,#0x60
  0018f38c: mov r1,r5
  0018f38e: blx 0x000739b4
  0018f392: ldr r0,[0x0018f580]
  0018f394: add r0,pc
  0018f396: ldr r4,[r0,#0x0]
  0018f398: add r0,sp,#0x54
  0018f39a: blx r4
  0018f39c: add r0,sp,#0x60
  0018f39e: blx r4
  0018f3a0: add r0,sp,#0x6c
  0018f3a2: blx r4
  0018f3a4: ldr r0,[sp,#0x24]
  0018f3a6: ldr r5,[r0,#0x0]
  0018f3a8: add r0,sp,#0x48
  0018f3aa: add r1,sp,#0x78
  0018f3ac: movs r2,#0x0
  0018f3ae: blx 0x0006f028
  0018f3b2: vldr.32 s0,[pc,#0x1a4]
  0018f3b6: vmul.f32 s0,s16,s0
  0018f3ba: vcvt.s32.f32 s0,s0
  0018f3be: vmov r1,s0
  0018f3c2: add r0,sp,#0x3c
  0018f3c4: blx 0x0006f658
  0018f3c8: ldr r1,[0x0018f584]
  0018f3ca: add r1,pc
  0018f3cc: add r0,sp,#0x30
  0018f3ce: movs r2,#0x0
  0018f3d0: blx 0x0006ee18
  0018f3d4: str r0,[sp,#0x0]
  0018f3d6: add r0,sp,#0x118
  0018f3d8: add r2,sp,#0x48
  0018f3da: add r3,sp,#0x3c
  0018f3dc: mov r1,r5
  0018f3de: blx 0x000739b4
  0018f3e2: add r0,sp,#0x78
  0018f3e4: add r1,sp,#0x118
  0018f3e6: blx 0x0006f2b0
  0018f3ea: add r0,sp,#0x118
  0018f3ec: blx r4
  0018f3ee: add r0,sp,#0x30
  0018f3f0: blx r4
  0018f3f2: add r0,sp,#0x3c
  0018f3f4: blx r4
  0018f3f6: add r0,sp,#0x48
  0018f3f8: blx r4
  0018f3fa: ldr r0,[sp,#0x20]
  0018f3fc: ldr.w r0,[r0,#0x94]
  0018f400: add r1,sp,#0x78
  0018f402: movs r2,#0x1
  0018f404: movs r4,#0x1
  0018f406: blx 0x000746f8
  0018f40a: ldr.w r11,[sp,#0x20]
  0018f40e: movw r0,#0x101
  0018f412: strb.w r4,[r11,#0x111]
  0018f416: strh.w r0,[r11,#0xce]
  0018f41a: add r0,sp,#0x78
  0018f41c: strb.w r4,[r11,#0xd4]
  0018f420: blx 0x0006ee30
  0018f424: b.w 0x0018dc40
```
