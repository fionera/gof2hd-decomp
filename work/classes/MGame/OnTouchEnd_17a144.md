# MGame::OnTouchEnd
elf 0x17a144 body 7566
Sig: undefined __thiscall OnTouchEnd(MGame * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::OnTouchEnd(int, int, void*) */

void __thiscall MGame::OnTouchEnd(MGame *this,int param_1,int param_2,void *param_3)

{
  PlayerEgo **ppPVar1;
  bool bVar2;
  byte bVar3;
  char cVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  String *pSVar8;
  int *piVar9;
  Item *pIVar10;
  char *pcVar11;
  Layout *this_00;
  StarMap *this_01;
  Engine *this_02;
  int iVar12;
  String *pSVar13;
  uint uVar14;
  Radar *pRVar15;
  Mission *pMVar16;
  FileRead *pFVar17;
  Ship *this_03;
  int iVar18;
  uint *puVar19;
  undefined4 uVar20;
  uint extraout_r1;
  uint extraout_r1_00;
  uint uVar21;
  uint extraout_r1_01;
  uint extraout_r1_02;
  Level *pLVar22;
  int *piVar23;
  undefined1 *puVar24;
  uint extraout_r1_03;
  uint extraout_r1_04;
  uint extraout_r1_05;
  void *extraout_r2;
  void *extraout_r2_00;
  void *extraout_r2_01;
  void *extraout_r2_02;
  void *extraout_r2_03;
  void *extraout_r2_04;
  void *extraout_r2_05;
  void *extraout_r2_06;
  void *extraout_r2_07;
  void *extraout_r2_08;
  void *extraout_r2_09;
  void *extraout_r2_10;
  void *extraout_r2_11;
  void *pvVar25;
  void *extraout_r2_12;
  void *extraout_r2_13;
  void *extraout_r2_14;
  void *extraout_r2_15;
  void *extraout_r2_16;
  void *extraout_r2_17;
  void *extraout_r2_18;
  void *extraout_r2_19;
  void *extraout_r2_20;
  void *extraout_r2_21;
  void *extraout_r2_22;
  undefined4 *puVar26;
  undefined4 *puVar27;
  PlayerEgo *pPVar28;
  SingleLevel *pSVar29;
  Vector *pVVar30;
  Station *pSVar31;
  DialogueWindow *pDVar32;
  ChoiceWindow *this_04;
  code *pcVar33;
  int iVar34;
  int *piVar35;
  int *piVar36;
  float fVar37;
  float extraout_s0;
  undefined8 uVar38;
  String aSStack_b8 [12];
  undefined4 local_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  String aSStack_a0 [12];
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 local_50;
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar36 = *(int **)(DAT_0018a4e8 + 0x18a15c);
  local_28 = *piVar36;
  if (*(void **)&this->field_C0 == param_3) {
    this->field_C0 = '\0';
    this->field_C1 = '\0';
    this->field_C2 = '\0';
    this->m_nStarMapWindowOpen = '\0';
  }
  ppPVar1 = &this->m_pPlayerEgo;
  iVar6 = PlayerEgo::isAutoPilot();
  this[1].field_A0 = 0x3f800000;
  TargetFollowCamera::setFastForwardMode(SUB41(this[1].field_2C,0));
  *(undefined1 *)&(*ppPVar1)[3].field_18 = 1;
  if (*(char *)((int)&this->field_5C + 1) == '\0') {
LAB_0018a6e4:
    uVar14 = Hud::touchEnd((uint)this->m_pLevel,param_1,(void *)param_2);
    this[1].field_30 = uVar14;
    if (uVar14 != 0) {
      this->field_98 = 0;
      this->field_9C = 0;
      if ((uVar14 & 1) != 0) {
        pauseSounds();
        if (*(char *)((int)&this->field_5C + 1) == '\0') {
          pDVar32 = this->m_pDialogueWindow;
          if (pDVar32 == (DialogueWindow *)0x0) {
            pDVar32 = operator_new(0x240);
            MenuTouchWindow::MenuTouchWindow((MenuTouchWindow *)pDVar32,1);
            this->m_pDialogueWindow = pDVar32;
          }
          LevelScript::canSkipCutsceneNow();
          MenuTouchWindow::setSkipButtonVisible(SUB41(pDVar32,0));
          *(undefined1 *)((int)&this->field_5C + 1) = 1;
          pauseSounds();
          piVar35 = *(int **)(DAT_0018a810 + 0x18a772);
          *(undefined1 *)((int)&this[2].field_14 + 2) = *(undefined1 *)((int)&this->field_5C + 1);
          bVar3 = FModSound::IsCategoryEnabled(*piVar35);
          *(byte *)&this[2].field_14 = bVar3 ^ 1;
          FModSound::pauseAllPlaying();
          PlayerEgo::PauseEngineSound();
          puVar19 = (uint *)Level::getEnemies();
          if (puVar19 != (uint *)0x0) {
            for (uVar14 = 0; uVar14 < *puVar19; uVar14 = uVar14 + 1) {
              KIPlayer::PauseEngineSound(*(KIPlayer **)(puVar19[1] + uVar14 * 4));
            }
          }
          pDVar32 = this->m_pDialogueWindow;
          if (*(char *)((int)&this->field_5C + 3) == '\0') {
            PlayerEgo::isDead(*ppPVar1);
          }
          fVar37 = (float)MenuTouchWindow::setCutsceneMode(SUB41(pDVar32,0));
          *(undefined1 *)((int)&this[1].field_0 + 1) = 1;
          FModSound::play(*piVar35,(Vector *)0x7b,(Vector *)0x0,fVar37);
        }
        Hud::releaseAllKeys();
        goto LAB_0018b004;
      }
    }
    iVar7 = LevelScript::startSequence((LevelScript *)this->field_7C);
    if ((iVar7 == 0) && (iVar7 = PlayerEgo::isDead(*ppPVar1), iVar7 == 0)) {
      if ((*(char *)((int)&this->field_5C + 3) == '\0') && ((char)this[1].field_14 == '\0')) {
        if (((int)((uint)(byte)this[1].field_30 << 0x1e) < 0) &&
           ((((iVar7 = PlayerEgo::isBoostRefreshed(), iVar7 != 0 &&
              (iVar7 = PlayerEgo::boosting(), iVar7 == 0)) &&
             (iVar7 = PlayerEgo::isMining(), iVar7 == 0)) &&
            (iVar7 = PlayerEgo::isDockedToDockingPoint(), iVar7 == 0)))) {
          PlayerEgo::setThrust(extraout_s0);
          iVar7 = DAT_0018ac08;
          this[2].field_20 = 0x3f800000;
          this[2].field_24 = iVar7;
          iVar7 = AbyssEngine::ApplicationManager::GetEngine();
          *(undefined4 *)(iVar7 + 0x360) = 0;
          PlayerEgo::boost();
        }
        if (((((int)((uint)(byte)this[1].field_30 << 0x1c) < 0) &&
             (iVar7 = Player::gunAvailable((*ppPVar1)->m_pPlayer), iVar7 != 0)) &&
            ((iVar7 = PlayerEgo::isMining(), iVar7 == 0 &&
             ((iVar7 = PlayerEgo::isInTurretMode(), iVar7 == 0 &&
              (iVar7 = PlayerEgo::isDockedToDockingPoint(), iVar7 == 0)))))) &&
           (iVar7 = PlayerEgo::isLandingOrTakingOff(), iVar7 == 0)) {
          PlayerEgo::shoot((int)this->m_pPlayerEgo,this->field_40);
          Hud::checkIfQuickMenuIsEmpty();
        }
        iVar7 = PlayerEgo::isHacking();
        if (iVar7 != 0) {
          uVar14 = this[1].field_30;
          if ((uVar14 & 0x200) != 0) {
            PlayerEgo::hackingRotateLCW();
            uVar14 = this[1].field_30;
          }
          if ((int)(uVar14 << 0x15) < 0) {
            PlayerEgo::hackingRotateRCW();
          }
        }
        iVar7 = PlayerEgo::isInRocketControl();
        if (iVar7 == 0) {
          iVar7 = PlayerEgo::hasAutoTurret();
          pvVar25 = extraout_r2;
          if ((iVar7 != 0) && ((int)((uint)*(byte *)((int)&this[1].field_30 + 3) << 0x1a) < 0)) {
            pPVar28 = this->m_pPlayerEgo;
            PlayerEgo::autoTurretIsEnabled();
            PlayerEgo::setAutoTurret(SUB41(pPVar28,0));
            pSVar29 = this->m_pLevel;
            iVar7 = PlayerEgo::autoTurretIsEnabled();
            pPVar28 = (PlayerEgo *)0x21;
            if (iVar7 != 0) {
              pPVar28 = (PlayerEgo *)0x20;
            }
            Hud::hudEvent((int)pSVar29,pPVar28,(int)this->m_pPlayerEgo);
            pvVar25 = extraout_r2_00;
          }
          uVar14 = this[1].field_30;
          if ((uVar14 & 0x40) == 0) {
            uVar21 = 0;
            if (*(char *)&this[1].field_0 != '\0') {
              if ((int)(uVar14 << 10) < 0) {
                LevelScript::setAutoPilotToProgrammedStation();
                uVar14 = this[1].field_30;
              }
              if ((int)(uVar14 << 9) < 0) {
                pPVar28 = this->m_pPlayerEgo;
                Level::getLandmarks();
                PlayerEgo::setAutoPilot((KIPlayer *)pPVar28);
                Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0xc,(int)this->m_pPlayerEgo);
                uVar14 = this[1].field_30;
              }
              if ((int)(uVar14 << 8) < 0) {
                pPVar28 = this->m_pPlayerEgo;
                Level::getLandmarks();
                PlayerEgo::setAutoPilot((KIPlayer *)pPVar28);
                Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0xa,(int)this->m_pPlayerEgo);
                uVar14 = this[1].field_30;
              }
              if ((int)(uVar14 << 7) < 0) {
                pPVar28 = this->m_pPlayerEgo;
                Level::getAsteroidWaypoint();
                PlayerEgo::setAutoPilot((KIPlayer *)pPVar28);
                Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0xe,(int)this->m_pPlayerEgo);
                uVar14 = this[1].field_30;
              }
              if (((int)(uVar14 << 6) < 0) && (iVar7 = Level::getPlayerRoute(), iVar7 != 0)) {
                pPVar28 = this->m_pPlayerEgo;
                Level::getPlayerRoute();
                Route::getWaypoint();
                PlayerEgo::setAutoPilot((KIPlayer *)pPVar28);
                Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0xd,(int)this->m_pPlayerEgo);
              }
              for (uVar14 = 0; iVar7 = Level::getNumDockingTargets(), (int)uVar14 < iVar7;
                  uVar14 = uVar14 + 1) {
                if (((this[1].field_30 & 0x4000000 << (uVar14 & 0xff)) != 0) &&
                   (iVar7 = Level::getDockingTarget((int)this->field_78), iVar7 != 0)) {
                  iVar7 = Level::getDockingTarget((int)this->field_78);
                  this->field_80[1] = iVar7;
                  Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0x22,(int)this->m_pPlayerEgo);
                  pPVar28 = this->m_pPlayerEgo;
                  pRVar15 = (Radar *)Level::getDockingTarget((int)this->field_78);
                  PlayerEgo::dockToDockingPoint((KIPlayer *)pPVar28,pRVar15);
                  piVar35 = this->field_80;
                  piVar35[2] = 0;
                  piVar35[3] = 0;
                  piVar35[4] = 0;
                  Hud::releaseAllKeys();
                }
              }
              *(undefined1 *)((int)&this[1].field_0C + 2) = 0;
              *(undefined1 *)&this[1].field_0 = 0;
              *(undefined1 *)((int)&this->field_5C + 1) = 0;
              resumeSounds();
              Hud::closeHudMenu();
              pPVar28 = this->m_pPlayerEgo;
              uVar21 = extraout_r1;
              pvVar25 = extraout_r2_01;
LAB_0018aae0:
              if (pPVar28 != (PlayerEgo *)0x0) {
                PlayerEgo::resetGunDelay();
                uVar21 = extraout_r1_00;
                pvVar25 = extraout_r2_02;
              }
            }
          }
          else {
            iVar7 = Status::inAlienOrbit();
            if ((iVar7 != 0) && (iVar7 = PlayerEgo::isDockingToAsteroid(), iVar7 != 0)) {
              PlayerEgo::dockToAsteroid((KIPlayer *)this->m_pPlayerEgo,(Radar *)0x0);
            }
            iVar7 = Status::inAlienOrbit();
            if (iVar7 == 0) {
LAB_0018acbe:
              uVar38 = Status::getCurrentCampaignMission();
              uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
              pvVar25 = extraout_r2_07;
              if (1 < (int)uVar38) {
                uVar38 = Status::getCurrentCampaignMission();
                uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                pvVar25 = extraout_r2_08;
                if ((int)uVar38 != 0x30) {
                  uVar38 = PlayerEgo::isDockedToDockingPoint();
                  uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                  pvVar25 = extraout_r2_09;
                  if ((int)uVar38 == 0) {
                    uVar38 = PlayerEgo::isLandingOrTakingOff();
                    uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                    pvVar25 = extraout_r2_10;
                    if ((int)uVar38 == 0) {
                      if (*(char *)&this[1].field_0 == '\0') {
                        uVar38 = PlayerEgo::isMining();
                        uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                        if ((int)uVar38 == 0) {
                          uVar38 = PlayerEgo::isAutoPilot();
                          uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                          if ((int)uVar38 == 0) {
                            pvVar25 = extraout_r2_17;
                            if (*(char *)&this[1].field_0 != '\0') goto LAB_0018acea;
                            iVar7 = PlayerEgo::isDockingToAsteroid();
                            pPVar28 = *ppPVar1;
                            if (iVar7 == 0) {
                              iVar7 = PlayerEgo::isDockingToDockingPoint();
                              if (iVar7 != 0) {
                                this->field_80[1] = 0;
                                PlayerEgo::dockToDockingPoint
                                          ((KIPlayer *)this->m_pPlayerEgo,(Radar *)0x0);
                                goto LAB_0018bb50;
                              }
                              iVar7 = PlayerEgo::isDockingToStream();
                              if (iVar7 == 0) {
                                iVar7 = Status::inAlienOrbit();
                                if (iVar7 == 0) {
LAB_0018bfaa:
                                  iVar7 = Status::getMission();
                                  if (iVar7 != 0) {
                                    Status::getMission();
                                    uVar38 = Mission::getType();
                                    uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                                    pvVar25 = extraout_r2_21;
                                    if ((int)uVar38 == 0xb7) goto LAB_0018acea;
                                  }
                                  *(undefined1 *)((int)&this->field_5C + 1) = 1;
                                  *(undefined1 *)((int)&this[1].field_0C + 2) = 1;
                                  pauseSounds();
                                  Hud::initHudMenu((int)this->m_pLevel,(Level *)0x3);
                                  *(undefined1 *)&this[1].field_0 = 1;
                                  uVar21 = extraout_r1_05;
                                  pvVar25 = extraout_r2_22;
                                }
                                else {
                                  uVar38 = Status::inAlienOrbit();
                                  uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                                  pvVar25 = extraout_r2_19;
                                  if ((int)uVar38 != 0) {
                                    uVar38 = Status::getCurrentCampaignMission();
                                    uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                                    pvVar25 = extraout_r2_20;
                                    if ((int)uVar38 == 0x9a) goto LAB_0018bfaa;
                                  }
                                }
                                goto LAB_0018acea;
                              }
                              pPVar28 = this->m_pPlayerEgo;
                              pRVar15 = (Radar *)this->field_80[9];
                            }
                            else {
                              pRVar15 = (Radar *)Radar::getLockedAsteroid((Radar *)this->field_80);
                            }
                            PlayerEgo::dockToAsteroid((KIPlayer *)pPVar28,pRVar15);
                          }
                          else {
                            PlayerEgo::setAutoPilot((KIPlayer *)this->m_pPlayerEgo);
                          }
LAB_0018bb50:
                          Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0x6,(int)this->m_pPlayerEgo
                                       );
                          uVar21 = extraout_r1_04;
                          pvVar25 = extraout_r2_18;
                          goto LAB_0018acea;
                        }
                        pvVar25 = extraout_r2_16;
                        if (*(char *)&this[1].field_0 == '\0') goto LAB_0018acea;
                      }
                      *(undefined1 *)((int)&this[1].field_0C + 2) = 0;
                      *(undefined1 *)&this[1].field_0 = 0;
                      *(undefined1 *)((int)&this->field_5C + 1) = 0;
                      resumeSounds();
                      Hud::closeHudMenu();
                      pPVar28 = this->m_pPlayerEgo;
                      uVar21 = extraout_r1_03;
                      pvVar25 = extraout_r2_15;
                      goto LAB_0018aae0;
                    }
                  }
                }
              }
            }
            else {
              uVar38 = Status::inAlienOrbit();
              uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
              pvVar25 = extraout_r2_04;
              if ((int)uVar38 != 0) {
                uVar38 = Status::getCurrentCampaignMission();
                uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                pvVar25 = extraout_r2_05;
                if ((int)uVar38 == 0x9a) {
                  uVar38 = Level::getNumDockingTargets();
                  uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
                  pvVar25 = extraout_r2_06;
                  if (0 < (int)uVar38) goto LAB_0018acbe;
                }
              }
            }
          }
LAB_0018acea:
          if ((int)((uint)(byte)this[1].field_30 << 0x1d) < 0) {
            uVar38 = PlayerEgo::isMining();
            uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
            pvVar25 = extraout_r2_11;
            if ((int)uVar38 == 0) {
              uVar21 = (uint)*(byte *)((int)&this[1].field_0C + 2);
              bVar3 = *(byte *)((int)&this->field_5C + 1);
              pvVar25 = (void *)(uVar21 ^ 1);
              *(char *)((int)&this[1].field_0C + 2) = (char)pvVar25;
              *(byte *)((int)&this->field_5C + 1) = bVar3 ^ 1;
              if (uVar21 == 0) {
                pauseSounds();
                Hud::initHudMenu((int)this->m_pLevel,(Level *)0x0);
                uVar21 = extraout_r1_01;
                pvVar25 = extraout_r2_12;
              }
            }
          }
          if ((int)((uint)(byte)this[1].field_30 << 0x18) < 0) {
            uVar38 = PlayerEgo::isMining();
            uVar21 = (uint)((ulonglong)uVar38 >> 0x20);
            pvVar25 = extraout_r2_13;
            if ((int)uVar38 == 0) {
              switchCamera((int)this);
              uVar21 = extraout_r1_02;
              pvVar25 = extraout_r2_14;
            }
          }
          if (*(char *)((int)&this[1].field_0C + 2) == '\0') {
            if (this->field_14 == 0) {
              maneuverTouchEnd((int)this,param_1,pvVar25);
              *(undefined2 *)&this[2].field_28 = 0;
            }
            else if (this->field_14 == 3) {
              freeCamTouchEnd((int)this,uVar21,pvVar25);
            }
          }
          else {
            uVar14 = this[1].field_30;
            if ((uVar14 & 0x200) == 0) {
              if (-1 < (int)(uVar14 << 0x14)) {
                if (-1 < (int)(uVar14 << 0x15)) {
                  if ((int)(uVar14 << 0x12) < 0) {
                    iVar7 = 0;
                  }
                  else if ((int)(uVar14 << 0x11) < 0) {
                    iVar7 = 1;
                  }
                  else if ((int)(uVar14 << 0x10) < 0) {
                    iVar7 = 2;
                  }
                  else {
                    if (-1 < (int)(uVar14 << 0xf)) {
                      if ((int)(uVar14 << 0xe) < 0) {
                        iVar7 = 1;
                      }
                      else if ((int)(uVar14 << 0xd) < 0) {
                        iVar7 = 3;
                      }
                      else if ((int)(uVar14 << 0xc) < 0) {
                        iVar7 = 2;
                      }
                      else {
                        if (-1 < (int)(uVar14 << 0xb)) {
                          if ((int)(uVar14 << 0x13) < 0) {
                            UseKhadorDrive();
                          }
                          goto LAB_0018afe2;
                        }
                        iVar7 = 0;
                        *(byte *)(**(int **)(DAT_0018c0d0 + 0x18bed4) + 0xf8) =
                             *(byte *)(**(int **)(DAT_0018c0d0 + 0x18bed4) + 0xf8) ^ 1;
                      }
                      puVar19 = (uint *)Level::getEnemies();
                      if (puVar19 != (uint *)0x0) {
                        for (uVar14 = 0; uVar14 < *puVar19; uVar14 = uVar14 + 1) {
                          piVar35 = *(int **)(puVar19[1] + uVar14 * 4);
                          iVar34 = KIPlayer::isWingMan();
                          if ((iVar34 != 0) && (iVar34 = KIPlayer::isDead(), iVar34 == 0)) {
                            if (iVar7 == 3) {
                              uVar20 = Radar::getLockedEnemy();
                            }
                            else {
                              uVar20 = 0;
                            }
                            (**(code **)(*piVar35 + 0x10))(piVar35,iVar7,uVar20);
                          }
                        }
                      }
                      *(undefined1 *)((int)&this[1].field_0C + 2) = 0;
                      Hud::closeHudMenu();
                      *(undefined1 *)((int)&this->field_5C + 1) = 0;
                      resumeSounds();
                      goto LAB_0018afe2;
                    }
                    iVar7 = 3;
                  }
                  this_03 = (Ship *)Status::getShip();
                  iVar34 = Ship::getEquipment(this_03,1);
                  if (iVar34 != 0) {
                    iVar18 = Level::getPlayer();
                    Item::getIndex(*(Item **)(*(int *)(iVar34 + 4) + iVar7 * 4));
                    PlayerEgo::setCurrentSecondaryWeaponIndex(iVar18);
                    Hud::setCurrentSecondaryWeapon((Item *)this->m_pLevel);
                  }
                  *(undefined1 *)((int)&this->field_5C + 1) = 0;
                  *(undefined1 *)((int)&this[1].field_0C + 2) = 0;
                  resumeSounds();
                  Hud::closeHudMenu();
                  goto LAB_0018afe8;
                }
                pSVar29 = this->m_pLevel;
                pLVar22 = (Level *)0x2;
                goto LAB_0018afde;
              }
              *(undefined1 *)((int)&this[1].field_0C + 2) = 0;
              Hud::closeHudMenu();
              *(undefined1 *)((int)&this->field_5C + 1) = 0;
              resumeSounds();
              useCloak(this);
            }
            else {
              pSVar29 = this->m_pLevel;
              pLVar22 = (Level *)0x1;
LAB_0018afde:
              Hud::initHudMenu((int)pSVar29,pLVar22);
            }
LAB_0018afe2:
            if (this[1].field_30 == 0) {
              Hud::closeHudMenu();
              *(undefined1 *)((int)&this[1].field_0C + 2) = 0;
              this->field_98 = 0;
              this->field_9C = 0;
              *(undefined1 *)((int)&this->field_5C + 1) = 0;
              resumeSounds();
              if ((*(char *)&this[1].field_0 != '\0') &&
                 (pPVar28 = this->m_pPlayerEgo, *(undefined1 *)&this[1].field_0 = 0,
                 pPVar28 != (PlayerEgo *)0x0)) {
                PlayerEgo::resetGunDelay();
              }
            }
          }
LAB_0018afe8:
          iVar34 = PlayerEgo::isAutoPilot();
          iVar7 = DAT_0018b3b8;
          if (iVar34 == 1 && iVar6 == 0) {
            this[2].field_20 = 0x3f800000;
            this[2].field_24 = iVar7;
          }
        }
      }
    }
    else {
      LevelScript::skipSequence((LevelScript *)this->field_7C);
    }
  }
  else {
    if (*(char *)((int)&this[1].field_4 + 2) != '\0') {
      if (*(char *)((int)&this[1].field_4 + 3) == '\0') {
        if (*(char *)((int)&this[1].field_0 + 2) == '\0') {
          iVar7 = this[2].field_54;
          iVar34 = ChoiceWindow::OnTouchEnd(this->field_94,param_1);
          if ((char)iVar7 == '\0') {
            if (iVar34 == 0) {
              *(undefined1 *)((int)&this->field_5C + 1) = 0;
              *(undefined1 *)((int)&this[1].field_4 + 2) = 0;
              goto LAB_0018a6de;
            }
          }
          else {
            if (iVar34 == 2) goto LAB_0018b004;
            if (iVar34 == 1) {
              Globals::playMusicAndFadeOutCurrent(**(int **)(DAT_0018ac14 + 0x18ab7c));
              *(undefined1 *)&this->field_54 = 0;
              AbyssEngine::ApplicationManager::SetCurrentApplicationModule((uint)this->field_8);
              iVar6 = **(int **)(DAT_0018ac18 + 0x18ab98);
LAB_0018ab9a:
              if (*piVar36 == local_28) {
                (*(code *)(DAT_001ac854 + 0x1ac858))();
                return;
              }
                    /* WARNING: Subroutine does not return */
              __stack_chk_fail(iVar6);
            }
            if (iVar34 == 0) {
              if (this->m_pDialogueWindow == (DialogueWindow *)0x0) {
                pDVar32 = operator_new(0x240);
                MenuTouchWindow::MenuTouchWindow((MenuTouchWindow *)pDVar32,1);
                this->m_pDialogueWindow = pDVar32;
              }
              if (*piVar36 - local_28 == 0) {
                (*(code *)(DAT_001ac834 + 0x1ac838))();
                return;
              }
                    /* WARNING: Subroutine does not return */
              __stack_chk_fail(*piVar36 - local_28);
            }
          }
        }
        else {
          iVar7 = ChoiceWindow::OnTouchEnd(this->field_94,param_1);
          if (iVar7 == 1) {
            *(undefined1 *)((int)&this[1].field_4 + 2) = 0;
            *(undefined1 *)((int)&this[1].field_0 + 2) = 0;
            *(undefined1 *)((int)&this->field_5C + 1) = 0;
            goto LAB_0018a6de;
          }
          if (iVar7 == 0) {
            AbyssEngine::String::String(aSStack_34);
            pIVar10 = (Item *)Status::getShip();
            bVar2 = false;
            piVar35 = *(int **)(DAT_0018a504 + 0x18a35c);
            pcVar11 = (char *)(DAT_0018a508 + 0x18a368);
            for (iVar7 = 0x9a; iVar7 < 0xa6; iVar7 = iVar7 + 1) {
              iVar34 = iVar7 + 0xb;
              if (iVar7 == 0xa5) {
                iVar34 = 0xda;
                iVar7 = 0xd9;
              }
              Status::hardCoreMode();
              iVar18 = 0;
              while (iVar12 = Ship::hasCargo((int)pIVar10,iVar7), iVar12 != 0) {
                Ship::removeCargo((int)pIVar10,iVar7);
                Item::makeItem(*(int *)(*(int *)(*piVar35 + 4) + iVar34 * 4));
                Ship::addCargo(pIVar10);
                iVar18 = iVar18 + -1;
                bVar2 = true;
              }
              if (iVar18 != 0) {
                AbyssEngine::String::String(aSStack_88,pcVar11,false);
                AbyssEngine::String::String(aSStack_94,-iVar18);
                AbyssEngine::operator+(aSStack_7c,aSStack_88);
                AbyssEngine::String::String(aSStack_a0,(char *)(DAT_0018a7f8 + 0x18a5f8),false);
                AbyssEngine::operator+(aSStack_70,aSStack_7c);
                GameText::getText(**(int **)(DAT_0018a7fc + 0x18a612));
                AbyssEngine::operator+((String *)&local_58,aSStack_70);
                AbyssEngine::String::operator+=(aSStack_34,(String *)&local_58);
                pcVar33 = *(code **)(DAT_0018a800 + 0x18a638);
                (*pcVar33)(&local_58);
                (*pcVar33)(aSStack_70);
                (*pcVar33)(aSStack_a0);
                (*pcVar33)(aSStack_7c);
                (*pcVar33)(aSStack_94);
                (*pcVar33)(aSStack_88);
              }
            }
            *(undefined1 *)((int)&this[1].field_0 + 2) = 0;
            pSVar8 = (String *)this->field_94;
            if (bVar2) {
              pSVar13 = (String *)GameText::getText(**(int **)(DAT_0018a804 + 0x18a678));
              AbyssEngine::String::String(aSStack_7c,(char *)(DAT_0018a808 + 0x18a68a),false);
              AbyssEngine::operator+(aSStack_70,pSVar13);
              AbyssEngine::operator+((String *)&local_58,aSStack_70);
              ChoiceWindow::set(pSVar8);
              pcVar33 = *(code **)(DAT_0018a80c + 0x18a6b2);
              (*pcVar33)(&local_58);
              (*pcVar33)(aSStack_70);
              (*pcVar33)(aSStack_7c);
            }
            else {
              GameText::getText(**(int **)(DAT_0018b3bc + 0x18b062));
              ChoiceWindow::set(pSVar8);
            }
            AbyssEngine::String::~String(aSStack_34);
          }
        }
      }
      else {
        iVar7 = ChoiceWindow::OnTouchEnd(this->field_94,param_1);
        if (iVar7 == 1) {
          piVar35 = (int *)Level::getEnemies();
          if (piVar35 != (int *)0x0) {
            iVar7 = *piVar35;
            iVar34 = 0;
            while (iVar7 != iVar34) {
              iVar18 = *(int *)(piVar35[1] + iVar34 * 4);
              iVar34 = iVar34 + 1;
              if (*(int *)(iVar18 + 0x28) == 8) {
                *(undefined1 *)(iVar18 + 0x25) = 1;
              }
            }
          }
          *(undefined1 *)(**(int **)(DAT_0018a510 + 0x18a3de) + 0x111) = 1;
          Level::createRadioMessage((int)this->field_78,0xb);
          *(undefined1 *)((int)&this->field_5C + 1) = 0;
          *(undefined2 *)((int)&this[1].field_4 + 2) = 0;
LAB_0018a6de:
          resumeSounds();
        }
        else if (iVar7 == 0) {
          piVar35 = *(int **)(DAT_0018a4ec + 0x18a1dc);
          iVar7 = Status::getCredits();
          if (iVar7 < (int)this[1].field_8) {
            this_04 = (ChoiceWindow *)this->field_94;
            iVar6 = *piVar35;
            pSVar8 = (String *)GameText::getText(**(int **)(DAT_0018a4f0 + 0x18a1fc));
            AbyssEngine::String::String(aSStack_40,pSVar8,false);
            Layout::formatCredits((int)&local_58);
            AbyssEngine::String::String(aSStack_4c,(String *)&local_58,false);
            AbyssEngine::String::String(aSStack_64,(char *)(DAT_0018a4f8 + 0x18a236),false);
            Status::replaceHash(aSStack_34,iVar6,aSStack_40,aSStack_4c);
            ChoiceWindow::set(this_04,aSStack_34,false);
            pcVar33 = *(code **)(DAT_0018a4fc + 0x18a25c);
            (*pcVar33)(aSStack_34);
            (*pcVar33)(aSStack_64);
            (*pcVar33)(aSStack_4c);
            (*pcVar33)(&local_58);
            (*pcVar33)(aSStack_40);
            piVar9 = this->field_78;
            *(undefined1 *)((int)&this[1].field_4 + 3) = 0;
            Level::createRadioMessage((int)piVar9,0xb);
            piVar9 = (int *)Level::getEnemies();
            if (piVar9 != (int *)0x0) {
              iVar6 = *piVar9;
              iVar7 = 0;
              while (iVar6 != iVar7) {
                iVar34 = *(int *)(piVar9[1] + iVar7 * 4);
                iVar7 = iVar7 + 1;
                if (*(int *)(iVar34 + 0x28) == 8) {
                  *(undefined1 *)(iVar34 + 0x25) = 1;
                }
              }
            }
            *(undefined1 *)(*piVar35 + 0x111) = 1;
            goto LAB_0018b004;
          }
          piVar9 = (int *)Level::getEnemies();
          if (piVar9 != (int *)0x0) {
            iVar7 = *piVar9;
            iVar34 = 0;
            while (iVar7 != iVar34) {
              iVar18 = *(int *)(piVar9[1] + iVar34 * 4);
              iVar34 = iVar34 + 1;
              if (*(int *)(iVar18 + 0x28) == 8) {
                *(undefined1 *)(iVar18 + 0x25) = 0;
              }
            }
          }
          Status::changeCredits(*piVar35);
          Level::createRadioMessage((int)this->field_78,10);
          *(undefined1 *)(*piVar35 + 0x110) = 1;
          *(undefined1 *)((int)&this->field_5C + 1) = 0;
          *(undefined2 *)((int)&this[1].field_4 + 2) = 0;
          resumeSounds();
        }
      }
      goto LAB_0018a6e4;
    }
    if (this->field_0xc5 == '\0') {
      if (this->field_C4 != '\0') goto LAB_0018a6e4;
      if (this->field_0xc7 != '\0') {
        puVar27 = *(undefined4 **)(DAT_0018a50c + 0x18a38a);
        this_00 = (Layout *)*puVar27;
        if (*this_00 == (Layout)0x0) {
          iVar6 = StarMap::OnTouchEnd((int)this->field_90,param_1);
          if (iVar6 != 0) {
            Level::getStarSystem();
            StarSystem::initLight();
            *(undefined1 *)((int)&this->field_5C + 1) = 0;
            this->field_0xc7 = 0;
            resumeSounds();
            cVar4 = *(char *)((int)&this[1].field_0 + 3);
            if ((char)*this->field_90 == '\0') {
              if (cVar4 != '\0') {
                PlayerEgo::dockToStream(SUB41(this->m_pPlayerEgo,0));
                PlayerEgo::setAutoPilot((KIPlayer *)this->m_pPlayerEgo);
                pVVar30 = (Vector *)this->m_pPlayerEgo->field_4;
                Level::getLandmarks();
                AEGeometry::getDirection();
                local_58 = 0;
                uStack_54 = 0x3f800000;
                local_50 = 0;
                AEGeometry::setDirection(pVVar30,(Vector *)aSStack_34);
                pPVar28 = this->m_pPlayerEgo;
                Level::getLandmarks();
                AEGeometry::getPosition();
                local_58 = 0;
                uStack_54 = 0;
                local_50 = DAT_0018afc8;
                AbyssEngine::AEMath::operator+((Vector *)&local_ac,(Vector *)aSStack_34);
                PlayerEgo::setPosition(pPVar28,local_ac,uStack_a8,uStack_a4);
              }
            }
            else if (cVar4 == '\0') {
              if (**(char **)(DAT_0018afd0 + 0x18af38) == '\0') {
                LevelScript::setAutoPilotToProgrammedStation();
              }
            }
            else {
              *(undefined1 *)((int)&this[1].field_14 + 1) = 0;
              startJumpScene(this);
            }
            if ((StarMap *)this->field_90 != (StarMap *)0x0) {
              pvVar25 = (void *)StarMap::~StarMap((StarMap *)this->field_90);
              operator_delete(pvVar25);
            }
            this->field_90 = (int *)0x0;
          }
        }
        else {
          iVar6 = Layout::OnTouchEnd(this_00,param_1,param_2);
          if (iVar6 != 0) {
            *(undefined1 *)*puVar27 = 0;
          }
        }
        goto LAB_0018b004;
      }
      if (*(char *)((int)&this->field_5C + 2) != '\0') {
        iVar6 = DialogueWindow::OnTouchEnd(this->field_8C,param_1);
        if (iVar6 == 0) goto LAB_0018b004;
        *(undefined1 *)((int)&this->field_5C + 1) = 0;
        resumeSounds();
        iVar6 = DAT_0018ac0c;
        *(undefined1 *)((int)&this->field_5C + 2) = 0;
        piVar35 = *(int **)(iVar6 + 0x18ab1e);
        Status::getMission();
        iVar6 = Mission::hasFailed();
        pMVar16 = (Mission *)Status::getMission();
        if (iVar6 != 0) {
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) || (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x2a)) {
            Globals::playMusicAndFadeOutCurrent(**(int **)(DAT_0018ac10 + 0x18ab58));
            *(undefined1 *)&this->field_54 = 0;
            AbyssEngine::ApplicationManager::SetCurrentApplicationModule((uint)this->field_8);
            iVar6 = *piVar35;
            goto LAB_0018ab9a;
          }
          Status::getMission();
          iVar6 = Mission::getType();
          iVar7 = *piVar35;
          Status::getMission();
          if (iVar6 == 0xc) {
            Mission::getReward();
            Status::changeCredits(iVar7);
          }
          else {
            iVar6 = Mission::getType();
            if (iVar6 != 3) {
              Status::getMission();
              iVar6 = Mission::getType();
              if (iVar6 != 5) {
                Status::getMission();
                iVar6 = Mission::getType();
                if (iVar6 != 0xb) goto LAB_0018b888;
              }
            }
            Status::getShip();
            puVar19 = (uint *)Ship::getCargo();
            if (puVar19 != (uint *)0x0) {
              for (uVar14 = 0; uVar14 < *puVar19; uVar14 = uVar14 + 1) {
                iVar6 = Item::isUnsaleable(*(Item **)(puVar19[1] + uVar14 * 4));
                if ((iVar6 != 0) &&
                   ((iVar6 = Item::getIndex(*(Item **)(puVar19[1] + uVar14 * 4)), iVar6 == 0x74 ||
                    (iVar6 = Item::getIndex(*(Item **)(puVar19[1] + uVar14 * 4)), iVar6 == 0x75))))
                {
                  pIVar10 = (Item *)Status::getShip();
                  Ship::removeCargo(pIVar10);
                  break;
                }
              }
            }
          }
LAB_0018b888:
          Status::setFreelanceMission((Mission *)*piVar35);
          Level::removeObjectives((Level *)this->field_78);
          *this->field_7C = 0;
          Status::setMission((Mission *)*piVar35);
          PlayerEgo::setRoute((Route *)this->m_pPlayerEgo);
          iVar6 = PlayerEgo::goingToWaypoint();
          if (iVar6 != 0) {
            PlayerEgo::setAutoPilot((KIPlayer *)*ppPVar1);
          }
          PlayerEgo::removeRoute();
          Level::setPlayerRoute((Route *)this->field_78);
LAB_0018b8e2:
          this->field_30 = 0;
          this->field_34 = 0;
          Hud::resetAnalogStick();
          if (*piVar36 == local_28) {
            (*(code *)(DAT_001ac7f4 + 0x1ac7f8))();
            return;
          }
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail(this->m_pLevel);
        }
        iVar6 = Mission::hasWon(pMVar16);
        if (iVar6 == 0) {
          pMVar16 = (Mission *)Status::getCampaignMission();
          iVar6 = Mission::hasWon(pMVar16);
          if (iVar6 == 0) {
            LevelScript::resetStartSequenceOver();
            goto LAB_0018b8e2;
          }
        }
        pMVar16 = (Mission *)Status::getCampaignMission();
        iVar6 = Mission::hasWon(pMVar16);
        if (iVar6 == 0) {
          uVar14 = 0;
        }
        else {
          pMVar16 = (Mission *)Status::getMission();
          uVar14 = Mission::isCampaignMission(pMVar16);
          uVar14 = uVar14 ^ 1;
        }
        pMVar16 = (Mission *)Status::getMission();
        iVar6 = Mission::hasWon(pMVar16);
        if (iVar6 == 0) {
          pMVar16 = (Mission *)Status::getCampaignMission();
        }
        else {
          pMVar16 = (Mission *)Status::getMission();
        }
        iVar6 = Mission::isInstantActionMission();
        if (iVar6 != 0) {
          **(undefined4 **)(DAT_0018afd4 + 0x18af8c) = 2;
          *(undefined1 *)&this->field_54 = 0;
          piVar35 = this->field_8;
          if (*piVar36 == local_28) {
            uVar20 = 1;
            goto LAB_0018b7a2;
          }
          goto LAB_0018b016;
        }
        iVar6 = Mission::isCampaignMission(pMVar16);
        if (iVar6 == 0) {
          Status::setFreelanceMission((Mission *)*piVar35);
          iVar6 = **(int **)(DAT_0018b3c4 + 0x18b0c8);
          cVar4 = Mission::getReward();
          cVar5 = Mission::getBonus();
          Layout::showMissionRewardMessage(iVar6,(bool)(cVar5 + cVar4));
        }
        else {
          Status::nextCampaignMission(SUB41((Mission *)*piVar35,0));
        }
        iVar6 = *piVar35;
        Mission::getReward();
        Mission::getBonus();
        Status::changeCredits(iVar6);
        *this->field_7C = 0;
        iVar6 = Mission::isCampaignMission(pMVar16);
        if ((iVar6 == 0) || (iVar6 = Status::getCurrentCampaignMission(), iVar6 != 0xf)) {
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x16))
          goto LAB_0018b182;
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x2b)) {
            pFVar17 = operator_new(1);
            FileRead::FileRead(pFVar17);
            pSVar31 = (Station *)*piVar35;
            goto LAB_0018b16c;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x2a)) {
            piVar9 = this->field_78;
            if ((Objective *)piVar9[0xb] != (Objective *)0x0) {
              pvVar25 = (void *)Objective::~Objective((Objective *)piVar9[0xb]);
              operator_delete(pvVar25);
              piVar9 = this->field_78;
            }
            piVar23 = piVar9 + 10;
            piVar9[0xb] = 0;
            if ((Objective *)*piVar23 != (Objective *)0x0) {
              pvVar25 = (void *)Objective::~Objective((Objective *)*piVar23);
              operator_delete(pvVar25);
              piVar23 = this->field_78 + 10;
            }
            *piVar23 = 0;
LAB_0018b266:
            if (uVar14 == 0) {
              Level::removeObjectives((Level *)this->field_78);
              Status::setMission((Mission *)*piVar35);
              PlayerEgo::setRoute((Route *)this->m_pPlayerEgo);
              iVar6 = PlayerEgo::goingToWaypoint();
              if (iVar6 != 0) {
                PlayerEgo::setAutoPilot((KIPlayer *)*ppPVar1);
              }
              PlayerEgo::removeRoute();
              Level::setPlayerRoute((Route *)this->field_78);
            }
            iVar6 = Mission::isCampaignMission(pMVar16);
            if ((iVar6 != 0) || (iVar6 = Mission::getType(), iVar6 != 0xb7)) goto LAB_0018b8e2;
            piVar9 = *(int **)(DAT_0018b3d0 + 0x18b2d6);
            this[2].field_50 = this->field_78[9];
            iVar6 = Item::makeItem(*(int *)(*(int *)(*piVar9 + 4) + 0x360));
            pIVar10 = (Item *)Status::getShip();
            Ship::setEquipment(pIVar10,iVar6);
            pMVar16 = operator_new(0x78);
            AbyssEngine::String::String(aSStack_b8,(char *)(DAT_0018b3d4 + 0x18b30a),false);
            uVar20 = ImageFactory::createChar(SUB41(**(undefined4 **)(DAT_0018b3d8 + 0x18b316),0),1)
            ;
            pSVar31 = (Station *)Status::getStation();
            Station::getIndex(pSVar31);
            Mission::Mission(pMVar16,0xb7,aSStack_b8,uVar20);
            AbyssEngine::String::~String(aSStack_b8);
            Status::setMission((Mission *)*piVar35);
            Status::setFreelanceMission((Mission *)*piVar35);
            iVar6 = DAT_0018b3dc;
            *(undefined1 *)&this->field_54 = 0;
            AbyssEngine::ApplicationManager::SetCurrentApplicationModule
                      (**(uint **)(iVar6 + 0x18b36c));
            goto LAB_0018b004;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x41)) {
            **(undefined4 **)(DAT_0018b7b0 + 0x18b436) = 1;
            uVar20 = Player::getHitpoints();
            *(undefined4 *)(*piVar35 + 100) = uVar20;
            uVar20 = Player::getShieldHP((Player *)(*ppPVar1)->m_pPlayer);
            *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
            uVar20 = Player::getArmorHP();
            *(undefined4 *)(*piVar35 + 0x60) = uVar20;
            uVar20 = Player::getGammaHP();
            puVar24 = *(undefined1 **)(DAT_0018b7b4 + 0x18b474);
            *(undefined4 *)(*piVar35 + 0x68) = uVar20;
            *puVar24 = 1;
            pFVar17 = operator_new(1);
            FileRead::FileRead(pFVar17);
            pSVar31 = (Station *)*piVar35;
            FileRead::loadStation((int)pFVar17);
            Status::departStation(pSVar31);
            pvVar25 = (void *)FileRead::~FileRead(pFVar17);
            operator_delete(pvVar25);
LAB_0018b78a:
            *(undefined1 *)&this->field_54 = 0;
LAB_0018b790:
            piVar35 = this->field_8;
            if (*piVar36 == local_28) {
              uVar20 = 2;
              goto LAB_0018b7a2;
            }
            goto LAB_0018b016;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x51)) {
            **(undefined4 **)(DAT_0018b7b8 + 0x18b4c2) = 1;
            uVar20 = Player::getHitpoints();
            *(undefined4 *)(*piVar35 + 100) = uVar20;
            uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
            *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
            uVar20 = Player::getArmorHP();
            *(undefined4 *)(*piVar35 + 0x60) = uVar20;
            uVar20 = Player::getGammaHP();
            pSVar31 = (Station *)*piVar35;
            puVar24 = *(undefined1 **)(DAT_0018b7bc + 0x18b508);
            *(undefined4 *)(pSVar31 + 0x68) = uVar20;
            *puVar24 = 1;
            Status::setStation(pSVar31);
            pSVar31 = (Station *)*piVar35;
LAB_0018b786:
            Status::departStation(pSVar31);
            goto LAB_0018b78a;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x4a)) {
            pFVar17 = operator_new(1);
            FileRead::FileRead(pFVar17);
            pSVar31 = (Station *)*piVar35;
            FileRead::loadStation((int)pFVar17);
            Status::setStation(pSVar31);
            pvVar25 = (void *)FileRead::~FileRead(pFVar17);
            operator_delete(pvVar25);
            FModSound::stopAll();
            goto LAB_0018b182;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x5f)) {
            **(undefined4 **)(DAT_0018b7dc + 0x18b6b0) = 1;
            uVar20 = Player::getHitpoints();
            *(undefined4 *)(*piVar35 + 100) = uVar20;
            uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
            *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
            uVar20 = Player::getArmorHP();
            *(undefined4 *)(*piVar35 + 0x60) = uVar20;
            uVar20 = Player::getGammaHP();
            pSVar31 = (Station *)*piVar35;
            piVar35 = *(int **)(DAT_0018b7e0 + 0x18b6f4);
            puVar24 = *(undefined1 **)(DAT_0018b7e4 + 0x18b6fa);
            *(undefined4 *)(pSVar31 + 0x68) = uVar20;
            iVar6 = *piVar35;
LAB_0018b77c:
            *puVar24 = 1;
LAB_0018b77e:
            Galaxy::getStation(iVar6);
            goto LAB_0018b786;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x60)) {
LAB_0018b722:
            **(undefined4 **)(DAT_0018b7e8 + 0x18b72a) = 1;
            uVar20 = Player::getHitpoints();
            *(undefined4 *)(*piVar35 + 100) = uVar20;
            uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
            *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
            uVar20 = Player::getArmorHP();
            *(undefined4 *)(*piVar35 + 0x60) = uVar20;
            uVar20 = Player::getGammaHP();
            pSVar31 = (Station *)*piVar35;
            piVar35 = *(int **)(DAT_0018b7ec + 0x18b76e);
            puVar24 = *(undefined1 **)(DAT_0018b7f0 + 0x18b774);
            *(undefined4 *)(pSVar31 + 0x68) = uVar20;
            iVar6 = *piVar35;
            goto LAB_0018b77c;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 100)) {
            uVar20 = Player::getHitpoints();
            *(undefined4 *)(*piVar35 + 100) = uVar20;
            uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
            *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
            uVar20 = Player::getArmorHP();
            *(undefined4 *)(*piVar35 + 0x60) = uVar20;
            uVar20 = Player::getGammaHP();
            pSVar31 = (Station *)*piVar35;
            piVar35 = *(int **)(DAT_0018bb78 + 0x18b996);
            puVar26 = (undefined4 *)(DAT_0018bb80 + 0x18b9a2);
            puVar27 = *(undefined4 **)(DAT_0018bb7c + 0x18b99e);
            *(undefined4 *)(pSVar31 + 0x68) = uVar20;
            puVar24 = (undefined1 *)*puVar26;
            iVar6 = *piVar35;
            *puVar27 = 0;
            *puVar24 = 0;
            Galaxy::getStation(iVar6);
            Status::departStation(pSVar31);
            AbyssEngine::ApplicationManager::SetCurrentApplicationModule
                      (**(uint **)(DAT_0018bb84 + 0x18b9c2));
LAB_0018babe:
            *(undefined1 *)&this->field_54 = 0;
            goto LAB_0018b004;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x6e)) {
            uVar20 = Player::getHitpoints();
            *(undefined4 *)(*piVar35 + 100) = uVar20;
            uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
            *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
            uVar20 = Player::getArmorHP();
            *(undefined4 *)(*piVar35 + 0x60) = uVar20;
            uVar20 = Player::getGammaHP();
            pSVar31 = (Station *)*piVar35;
            piVar35 = *(int **)(DAT_0018bb8c + 0x18ba82);
            puVar26 = (undefined4 *)(DAT_0018bb94 + 0x18ba8e);
            puVar27 = *(undefined4 **)(DAT_0018bb90 + 0x18ba8a);
            *(undefined4 *)(pSVar31 + 0x68) = uVar20;
            puVar24 = (undefined1 *)*puVar26;
            iVar6 = *piVar35;
            *puVar27 = 0;
            *puVar24 = 0;
            Galaxy::getStation(iVar6);
            Status::departStation(pSVar31);
            AbyssEngine::ApplicationManager::SetCurrentApplicationModule
                      (**(uint **)(DAT_0018bb98 + 0x18baae));
            **(undefined1 **)(DAT_0018bb9c + 0x18babc) = 1;
            goto LAB_0018babe;
          }
          iVar6 = Mission::isCampaignMission(pMVar16);
          if ((iVar6 == 0) || (iVar6 = Status::getCurrentCampaignMission(), iVar6 != 0x78)) {
            iVar6 = Mission::isCampaignMission(pMVar16);
            if ((iVar6 == 0) || (iVar6 = Status::getCurrentCampaignMission(), iVar6 != 0x7e)) {
              iVar6 = Mission::isCampaignMission(pMVar16);
              if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x7f))
              goto LAB_0018b722;
              iVar6 = Mission::isCampaignMission(pMVar16);
              if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x86)) {
                **(undefined4 **)(DAT_0018bf70 + 0x18bc58) = 0;
                uVar20 = Player::getHitpoints();
                *(undefined4 *)(*piVar35 + 100) = uVar20;
                uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
                *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
                uVar20 = Player::getArmorHP();
                *(undefined4 *)(*piVar35 + 0x60) = uVar20;
                uVar20 = Player::getGammaHP();
                pSVar31 = (Station *)*piVar35;
                puVar27 = (undefined4 *)(DAT_0018bf78 + 0x18bca2);
                piVar35 = *(int **)(DAT_0018bf74 + 0x18bc9c);
                *(undefined4 *)(pSVar31 + 0x68) = uVar20;
                puVar24 = (undefined1 *)*puVar27;
                iVar6 = *piVar35;
                goto LAB_0018bcaa;
              }
              iVar6 = Mission::isCampaignMission(pMVar16);
              if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x90)) {
                **(undefined4 **)(DAT_0018bf7c + 0x18bd1c) = 1;
                uVar20 = Player::getHitpoints();
                *(undefined4 *)(*piVar35 + 100) = uVar20;
                uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
                *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
                uVar20 = Player::getArmorHP();
                *(undefined4 *)(*piVar35 + 0x60) = uVar20;
                uVar20 = Player::getGammaHP();
                pSVar31 = (Station *)*piVar35;
                piVar35 = *(int **)(DAT_0018c0a4 + 0x18bd62);
                puVar24 = *(undefined1 **)(DAT_0018c0a8 + 0x18bd68);
                *(undefined4 *)(pSVar31 + 0x68) = uVar20;
                iVar6 = *piVar35;
                goto LAB_0018bd70;
              }
              iVar6 = Mission::isCampaignMission(pMVar16);
              if ((iVar6 == 0) || (iVar6 = Status::getCurrentCampaignMission(), iVar6 != 0x9b)) {
                iVar6 = Mission::isCampaignMission(pMVar16);
                if ((iVar6 != 0) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0xa1)) {
                  uVar20 = Player::getHitpoints();
                  *(undefined4 *)(*piVar35 + 100) = uVar20;
                  uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
                  *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
                  uVar20 = Player::getArmorHP();
                  *(undefined4 *)(*piVar35 + 0x60) = uVar20;
                  uVar20 = Player::getGammaHP();
                  pSVar31 = (Station *)*piVar35;
                  piVar35 = *(int **)(DAT_0018c0b8 + 0x18be14);
                  puVar26 = (undefined4 *)(DAT_0018c0c0 + 0x18be20);
                  puVar27 = *(undefined4 **)(DAT_0018c0bc + 0x18be1c);
                  *(undefined4 *)(pSVar31 + 0x68) = uVar20;
                  puVar24 = (undefined1 *)*puVar26;
                  iVar6 = *piVar35;
                  *puVar27 = 0;
                  *puVar24 = 0;
                  goto LAB_0018bd72;
                }
                iVar6 = Mission::isCampaignMission(pMVar16);
                if ((iVar6 == 0) || (iVar6 = Status::getCurrentCampaignMission(), iVar6 != 0xa2))
                goto LAB_0018b266;
                uVar20 = Player::getHitpoints();
                *(undefined4 *)(*piVar35 + 100) = uVar20;
                uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
                *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
                uVar20 = Player::getArmorHP();
                *(undefined4 *)(*piVar35 + 0x60) = uVar20;
                uVar20 = Player::getGammaHP();
                pSVar31 = (Station *)*piVar35;
                piVar35 = *(int **)(DAT_0018c0c4 + 0x18be94);
                puVar27 = *(undefined4 **)(DAT_0018c0c8 + 0x18be96);
                puVar24 = *(undefined1 **)(DAT_0018c0cc + 0x18be9c);
                *(undefined4 *)(pSVar31 + 0x68) = uVar20;
                iVar6 = *piVar35;
                *puVar27 = 0;
                *puVar24 = 0;
                goto LAB_0018bcac;
              }
              pSVar31 = (Station *)*piVar35;
              puVar27 = *(undefined4 **)(DAT_0018c0b4 + 0x18bdac);
              iVar6 = **(int **)(DAT_0018c0ac + 0x18bda4);
              **(undefined1 **)(DAT_0018c0b0 + 0x18bda6) = 1;
              *puVar27 = 1;
              goto LAB_0018b77e;
            }
            **(undefined4 **)(DAT_0018bf64 + 0x18bbc8) = 1;
            uVar20 = Player::getHitpoints();
            *(undefined4 *)(*piVar35 + 100) = uVar20;
            uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
            *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
            uVar20 = Player::getArmorHP();
            *(undefined4 *)(*piVar35 + 0x60) = uVar20;
            uVar20 = Player::getGammaHP();
            pSVar31 = (Station *)*piVar35;
            piVar35 = *(int **)(DAT_0018bf68 + 0x18bc0e);
            puVar24 = *(undefined1 **)(DAT_0018bf6c + 0x18bc14);
            *(undefined4 *)(pSVar31 + 0x68) = uVar20;
            iVar6 = *piVar35;
LAB_0018bd70:
            *puVar24 = 0;
LAB_0018bd72:
            Galaxy::getStation(iVar6);
            Status::departStation(pSVar31);
            *(undefined1 *)&this->field_54 = 0;
            goto LAB_0018b790;
          }
          **(undefined4 **)(DAT_0018bba0 + 0x18bae4) = 1;
          uVar20 = Player::getHitpoints();
          *(undefined4 *)(*piVar35 + 100) = uVar20;
          uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
          *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
          uVar20 = Player::getArmorHP();
          *(undefined4 *)(*piVar35 + 0x60) = uVar20;
          uVar20 = Player::getGammaHP();
          pSVar31 = (Station *)*piVar35;
          piVar35 = *(int **)(DAT_0018bba4 + 0x18bb2a);
          puVar24 = *(undefined1 **)(DAT_0018bba8 + 0x18bb30);
          *(undefined4 *)(pSVar31 + 0x68) = uVar20;
          iVar6 = *piVar35;
LAB_0018bcaa:
          *puVar24 = 0;
LAB_0018bcac:
          Galaxy::getStation(iVar6);
          Status::departStation(pSVar31);
          *(undefined1 *)&this->field_54 = 0;
        }
        else {
          pFVar17 = operator_new(1);
          FileRead::FileRead(pFVar17);
          pSVar31 = (Station *)*piVar35;
LAB_0018b16c:
          FileRead::loadStation((int)pFVar17);
          Status::setStation(pSVar31);
          pvVar25 = (void *)FileRead::~FileRead(pFVar17);
          operator_delete(pvVar25);
LAB_0018b182:
          **(undefined4 **)(DAT_0018b3c8 + 0x18b18a) = 0;
          uVar20 = Player::getHitpoints();
          *(undefined4 *)(*piVar35 + 100) = uVar20;
          uVar20 = Player::getShieldHP((Player *)this->m_pPlayerEgo->m_pPlayer);
          *(undefined4 *)(*piVar35 + 0x5c) = uVar20;
          uVar20 = Player::getArmorHP();
          *(undefined4 *)(*piVar35 + 0x60) = uVar20;
          uVar20 = Player::getGammaHP();
          *(undefined4 *)(*piVar35 + 0x68) = uVar20;
          *(undefined1 *)&this->field_54 = 0;
        }
        piVar35 = this->field_8;
        if (*piVar36 == local_28) {
          uVar20 = 5;
LAB_0018b7a2:
          (*(code *)(DAT_001ab904 + 0x1ab908))(piVar35,uVar20);
          return;
        }
        goto LAB_0018b016;
      }
      if (*(char *)((int)&this[1].field_0 + 1) == '\0') goto LAB_0018a6e4;
      if (*(char *)((int)&this[1].field_94 + 2) != '\0') {
        iVar6 = AbyssEngine::ApplicationManager::GetApplicationData();
        if ((*(char *)(iVar6 + 5) != '\0') || (*(char *)(iVar6 + 0xc) != '\0')) goto LAB_0018b004;
        iVar6 = MenuTouchWindow::isShowingMessage();
        if (iVar6 == 0) {
          uVar38 = MenuTouchWindow::isMakingScreenshot();
          if ((int)uVar38 == 0) {
            freeCamTouchEnd((int)this,(int)((ulonglong)uVar38 >> 0x20),extraout_r2_03);
          }
        }
      }
      iVar6 = MenuTouchWindow::OnTouchEnd
                        ((MenuTouchWindow *)this->m_pDialogueWindow,param_1,param_2,param_3);
      if (iVar6 == 0) {
        if (*(char *)((int)&this[1].field_94 + 2) == '\0') goto LAB_0018b814;
      }
      else {
        *(undefined1 *)((int)&this[2].field_14 + 2) = 0;
        *(undefined1 *)((int)&this->field_5C + 1) = 0;
        resumeSounds();
        iVar6 = FModSound::IsCategoryEnabled(**(int **)(DAT_0018afcc + 0x18aeea));
        cVar4 = (char)this[2].field_14;
        if (iVar6 == 0) {
          if (cVar4 != '\0') goto LAB_0018b1ee;
          if (this->m_pPlayerEgo != (PlayerEgo *)0x0) {
            PlayerEgo::StopEngineSound();
          }
          puVar19 = (uint *)Level::getEnemies();
          if (puVar19 != (uint *)0x0) {
            for (uVar14 = 0; uVar14 < *puVar19; uVar14 = uVar14 + 1) {
              KIPlayer::StopEngineSound(*(KIPlayer **)(puVar19[1] + uVar14 * 4));
            }
          }
        }
        else if (cVar4 == '\0') {
          PlayerEgo::ResumeEngineSound();
          puVar19 = (uint *)Level::getEnemies();
          if (puVar19 != (uint *)0x0) {
            for (uVar14 = 0; uVar14 < *puVar19; uVar14 = uVar14 + 1) {
              KIPlayer::ResumeEngineSound(*(KIPlayer **)(puVar19[1] + uVar14 * 4));
            }
          }
        }
        else {
LAB_0018b1ee:
          PlayerEgo::PlayEngineSound();
          puVar19 = (uint *)Level::getEnemies();
          if (puVar19 != (uint *)0x0) {
            for (uVar14 = 0; uVar14 < *puVar19; uVar14 = uVar14 + 1) {
              KIPlayer::PlayEngineSound(*(KIPlayer **)(puVar19[1] + uVar14 * 4));
            }
          }
        }
        *(undefined1 *)((int)&this[1].field_0 + 1) = 0;
        this->field_98 = 0;
        this->field_9C = 0;
        this->field_C0 = '\0';
        this->field_C1 = '\0';
        this->field_C2 = '\0';
        this->m_nStarMapWindowOpen = '\0';
        Level::getStarSystem();
        StarSystem::initLight();
        Level::enableParticleEffects
                  (SUB41(this->field_78,0),
                   0.0 < *(float *)(*(int *)(DAT_0018b7c4 + 0x18b5f0) + 0x28));
        iVar6 = DAT_0018b7cc;
        if (*(char *)&this->m_pDialogueWindow->field_0 != '\0') {
          *(undefined1 *)&this->m_pDialogueWindow->field_0 = 0;
          piVar35 = *(int **)(iVar6 + 0x18b628);
          iVar6 = Status::getCurrentCampaignMission();
          if ((iVar6 - 0x9aU < 5) && ((1 << (iVar6 - 0x9aU & 0xff) & 0x19U) != 0)) {
            LevelScript::skipCutscene();
          }
          else if (iVar6 == 1) {
            puVar19 = *(uint **)(DAT_0018bb70 + 0x18b800);
            **(undefined4 **)(DAT_0018bb6c + 0x18b7fe) = 0;
            *(undefined1 *)&this->field_54 = 0;
            AbyssEngine::ApplicationManager::SetCurrentApplicationModule(*puVar19);
          }
          else if (iVar6 == 0) {
            Status::nextCampaignMission(SUB41(*piVar35,0));
            Status::setKills(*piVar35);
            puVar19 = *(uint **)(DAT_0018b7d4 + 0x18b678);
            **(undefined4 **)(DAT_0018b7d0 + 0x18b676) = 1;
            *(undefined1 *)&this->field_54 = 0;
            AbyssEngine::ApplicationManager::SetCurrentApplicationModule(*puVar19);
            **(undefined1 **)(DAT_0018b7d8 + 0x18b690) = 0;
          }
        }
        *(undefined1 *)((int)&this[1].field_94 + 2) = 0;
LAB_0018b814:
        iVar6 = MenuTouchWindow::inCinematicMode();
        if (iVar6 != 0) {
          setCinematicMode(SUB41(this,0));
          this[1].field_30 = 0;
          Level::getStarSystem();
          if (*piVar36 == local_28) {
            (*(code *)(DAT_001ac764 + 0x1ac768))();
            return;
          }
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail();
        }
        if (*(char *)((int)&this[1].field_94 + 2) == '\0') goto LAB_0018b004;
      }
      iVar6 = MenuTouchWindow::inCinematicMode();
      if (iVar6 == 0) {
        setCinematicMode(SUB41(this,0));
        this[1].field_30 = 0;
      }
    }
    else {
      iVar6 = ChoiceWindow::OnTouchEnd(this->field_94,param_1);
      if (iVar6 == 1) {
        this->field_0xc5 = 0;
        if (this->field_90 == (int *)0x0) {
          this_01 = operator_new(0x1e8);
          StarMap::StarMap(this_01,false,(Mission *)0x0,false,-1);
          this->field_90 = (int *)this_01;
        }
        this_02 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        AbyssEngine::Engine::SetPostEffect(this_02,DAT_0018a514,false);
        StarMap::initLights((StarMap *)this->field_90);
        StarMap::setJumpMapMode(SUB41(this->field_90,0),true);
        *(undefined1 *)((int)&this->field_5C + 1) = 1;
        this->field_0xc7 = 1;
        if (*piVar36 - local_28 == 0) {
          (*(code *)(DAT_001ac844 + 0x1ac848))();
          return;
        }
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(*piVar36 - local_28);
      }
      if (iVar6 == 0) {
        this->field_0xc5 = 0;
        iVar6 = PlayerEgo::isInTurretMode();
        if (iVar6 != 0) {
          PlayerEgo::setTurretMode(SUB41(*ppPVar1,0));
        }
        *(undefined1 *)((int)&this[1].field_14 + 1) = 0;
        startJumpScene(this);
        if (*piVar36 == local_28) {
          (*(code *)(DAT_001ac824 + 0x1ac828))();
          return;
        }
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(this->m_pPlayerEgo);
      }
    }
  }
LAB_0018b004:
  if (*piVar36 == local_28) {
    return;
  }
LAB_0018b016:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0018a144: push {r4,r5,r6,r7,lr}
  0018a146: add r7,sp,#0xc
  0018a148: push {r8,r9,r10,r11}
  0018a14c: sub sp,#0xcc
  0018a14e: str r1,[sp,#0x34]
  0018a150: mov r11,r0
  0018a152: ldr r0,[0x0018a4e8]
  0018a154: mov r10,r11
  0018a156: mov r6,r2
  0018a158: add r0,pc
  0018a15a: ldr.w r9,[r0,#0x0]
  0018a15e: ldr.w r0,[r9,#0x0]
  0018a162: str r0,[sp,#0xc8]
  0018a164: ldr.w r0,[r11,#0xc0]
  0018a168: str r3,[sp,#0x30]
  0018a16a: cmp r0,r3
  0018a16c: itt eq
  0018a16e: mov.eq r0,#0x0
  0018a170: str.w.eq r0,[r11,#0xc0]
  0018a174: ldr.w r0,[r10,#0x58]!
  0018a178: blx 0x0007681c
  0018a17c: mov r8,r0
  0018a17e: ldr.w r0,[r10,#0x9c]
  0018a182: mov.w r1,#0x3f800000
  0018a186: str.w r1,[r10,#0x110]
  0018a18a: movs r1,#0x0
  0018a18c: blx 0x000781d8
  0018a190: ldr.w r0,[r10,#0x0]
  0018a194: movs r1,#0x1
  0018a196: strb.w r1,[r0,#0x84]
  0018a19a: ldrb.w r0,[r10,#0x5]
  0018a19e: cmp r0,#0x0
  0018a1a0: beq.w 0x0018a6e4
  0018a1a4: ldrb.w r0,[r11,#0xce]
  0018a1a8: cmp r0,#0x0
  0018a1aa: beq.w 0x0018a2b6
  0018a1ae: ldrb.w r0,[r11,#0xcf]
  0018a1b2: cmp r0,#0x0
  0018a1b4: beq.w 0x0018a31a
  0018a1b8: ldr.w r0,[r11,#0x94]
  0018a1bc: mov r2,r6
  0018a1be: ldr r1,[sp,#0x34]
  0018a1c0: blx 0x00074770
  0018a1c4: cmp r0,#0x1
  0018a1c6: beq.w 0x0018a3ac
  0018a1ca: cmp r0,#0x0
  0018a1cc: bne.w 0x0018a6e4
  0018a1d0: str.w r9,[sp,#0x24]
  0018a1d4: mov r9,r8
  0018a1d6: ldr r0,[0x0018a4ec]
  0018a1d8: add r0,pc
  0018a1da: ldr.w r8,[r0,#0x0]
  0018a1de: ldr.w r0,[r8,#0x0]
  0018a1e2: blx 0x000733d8
  0018a1e6: ldr.w r1,[r11,#0xd0]
  0018a1ea: cmp r0,r1
  0018a1ec: bge.w 0x0018a4d6
  0018a1f0: ldr r0,[0x0018a4f0]
  0018a1f2: movs r1,#0xcb
  0018a1f4: ldr.w r6,[r11,#0x94]
  0018a1f8: add r0,pc
  0018a1fa: ldr.w r5,[r8,#0x0]
  0018a1fe: ldr r0,[r0,#0x0]
  0018a200: ldr r0,[r0,#0x0]
  0018a202: blx 0x00072f70
  0018a206: mov r1,r0
  0018a208: add r0,sp,#0xb0
  0018a20a: movs r2,#0x0
  0018a20c: blx 0x0006f028
  0018a210: ldr r0,[0x0018a4f4]
  0018a212: ldr.w r2,[r11,#0xd0]
  0018a216: add r0,pc
  0018a218: ldr r0,[r0,#0x0]
  0018a21a: ldr r1,[r0,#0x0]
  0018a21c: add r0,sp,#0x98
  0018a21e: blx 0x00074e54
  0018a222: add r0,sp,#0xa4
  0018a224: add r1,sp,#0x98
  0018a226: movs r2,#0x0
  0018a228: blx 0x0006f028
  0018a22c: ldr.w r9,[sp,#0x24]
  0018a230: ldr r1,[0x0018a4f8]
  0018a232: add r1,pc
  0018a234: add r0,sp,#0x8c
  0018a236: movs r2,#0x0
  0018a238: blx 0x0006ee18
  0018a23c: str r0,[sp,#0x0]
  0018a23e: add r0,sp,#0xbc
  0018a240: add r2,sp,#0xb0
  0018a242: add r3,sp,#0xa4
  0018a244: mov r1,r5
  0018a246: blx 0x000739b4
  0018a24a: add r1,sp,#0xbc
  0018a24c: mov r0,r6
  0018a24e: movs r2,#0x0
  0018a250: movs r5,#0x0
  0018a252: blx 0x000746f8
  0018a256: ldr r0,[0x0018a4fc]
  0018a258: add r0,pc
  0018a25a: ldr r6,[r0,#0x0]
  0018a25c: add r0,sp,#0xbc
  0018a25e: blx r6
  0018a260: add r0,sp,#0x8c
  0018a262: blx r6
  0018a264: add r0,sp,#0xa4
  0018a266: blx r6
  0018a268: add r0,sp,#0x98
  0018a26a: blx r6
  0018a26c: add r0,sp,#0xb0
  0018a26e: blx r6
  0018a270: ldr.w r0,[r11,#0x78]
  0018a274: movs r1,#0xb
  0018a276: movs r2,#0x8
  0018a278: strb.w r5,[r11,#0xcf]
  0018a27c: blx 0x00072754
  0018a280: ldr.w r0,[r11,#0x78]
  0018a284: blx 0x00071eb4
  0018a288: cbz r0,0x0018a2a8
  0018a28a: ldr r1,[r0,#0x0]
  0018a28c: movs r2,#0x0
  0018a28e: movs r3,#0x1
  0018a290: b 0x0018a2a4
  0018a292: ldr r6,[r0,#0x4]
  0018a294: ldr.w r6,[r6,r2,lsl #0x2]
  0018a298: adds r2,#0x1
  0018a29a: ldr r5,[r6,#0x28]
  0018a29c: cmp r5,#0x8
  0018a29e: it eq
  0018a2a0: strb.eq.w r3,[r6,#0x25]
  0018a2a4: cmp r1,r2
  0018a2a6: bne 0x0018a292
  0018a2a8: ldr.w r0,[r8,#0x0]
  0018a2ac: movs r1,#0x1
  0018a2ae: strb.w r1,[r0,#0x111]
  0018a2b2: b.w 0x0018b004
  0018a2b6: ldrb.w r0,[r11,#0xc5]
  0018a2ba: cmp r0,#0x0
  0018a2bc: beq 0x0018a370
  0018a2be: ldr.w r0,[r11,#0x94]
  0018a2c2: mov r2,r6
  0018a2c4: ldr r1,[sp,#0x34]
  0018a2c6: blx 0x00074770
  0018a2ca: cmp r0,#0x1
  0018a2cc: beq.w 0x0018a45c
  0018a2d0: cmp r0,#0x0
  0018a2d2: bne.w 0x0018b004
  0018a2d6: ldr.w r0,[r11,#0x58]
  0018a2da: movs r4,#0x0
  0018a2dc: strb.w r4,[r11,#0xc5]
  0018a2e0: blx 0x0007678c
  0018a2e4: cbz r0,0x0018a2f0
  0018a2e6: ldr.w r0,[r10,#0x0]
  0018a2ea: movs r1,#0x0
  0018a2ec: blx 0x00072b08
  0018a2f0: mov r0,r11
  0018a2f2: strb.w r4,[r11,#0xdd]
  0018a2f6: blx 0x000782bc
  0018a2fa: ldr.w r0,[r11,#0x58]
  0018a2fe: ldr r1,[sp,#0xc8]
  0018a300: ldr.w r2,[r9,#0x0]
  0018a304: subs r1,r2,r1
  0018a306: itttt eq
  0018a308: add.eq sp,#0xcc
  0018a30a: pop.eq.w {r8,r9,r10,r11}
  0018a30e: pop.eq.w {r4,r5,r6,r7,lr}
  0018a312: b.eq.w 0x001ac818
  0018a316: blx 0x0006e824
  0018a31a: ldrb.w r0,[r11,#0xca]
  0018a31e: cmp r0,#0x0
  0018a320: beq 0x0018a3fa
  0018a322: ldr.w r0,[r11,#0x94]
  0018a326: mov r2,r6
  0018a328: ldr r1,[sp,#0x34]
  0018a32a: blx 0x00074770
  0018a32e: cmp r0,#0x1
  0018a330: beq.w 0x0018a6c2
  0018a334: cmp r0,#0x0
  0018a336: bne.w 0x0018a6e4
  0018a33a: add r0,sp,#0xbc
  0018a33c: blx 0x0006efbc
  0018a340: ldr r0,[0x0018a500]
  0018a342: add r0,pc
  0018a344: ldr r0,[r0,#0x0]
  0018a346: str r0,[sp,#0x20]
  0018a348: ldr r0,[r0,#0x0]
  0018a34a: blx 0x00071a58
  0018a34e: mov r5,r0
  0018a350: movs r0,#0x0
  0018a352: str r0,[sp,#0x28]
  0018a354: movs r4,#0x9a
  0018a356: ldr r0,[0x0018a504]
  0018a358: add r0,pc
  0018a35a: ldr r0,[r0,#0x0]
  0018a35c: str r0,[sp,#0x2c]
  0018a35e: ldr r0,[0x0018a508]
  0018a360: str.w r9,[sp,#0x24]
  0018a364: add r0,pc
  0018a366: strd r0,r10,[sp,#0x14]
  0018a36a: str.w r8,[sp,#0x1c]
  0018a36e: b 0x0018a65c
  0018a370: ldrb.w r0,[r11,#0xc4]
  0018a374: cmp r0,#0x0
  0018a376: bne.w 0x0018a6e4
  0018a37a: ldrb.w r0,[r11,#0xc7]
  0018a37e: cmp r0,#0x0
  0018a380: beq.w 0x0018aaec
  0018a384: ldr r0,[0x0018a50c]
  0018a386: add r0,pc
  0018a388: ldr r4,[r0,#0x0]
  0018a38a: ldr r0,[r4,#0x0]
  0018a38c: ldrb r1,[r0,#0x0]
  0018a38e: cmp r1,#0x0
  0018a390: beq.w 0x0018abb6
  0018a394: ldr r1,[sp,#0x34]
  0018a396: mov r2,r6
  0018a398: blx 0x0007477c
  0018a39c: cmp r0,#0x0
  0018a39e: beq.w 0x0018b004
  0018a3a2: ldr r0,[r4,#0x0]
  0018a3a4: movs r1,#0x0
  0018a3a6: strb r1,[r0,#0x0]
  0018a3a8: b.w 0x0018b004
  0018a3ac: ldr.w r0,[r11,#0x78]
  0018a3b0: blx 0x00071eb4
  0018a3b4: cbz r0,0x0018a3d4
  0018a3b6: ldr r1,[r0,#0x0]
  0018a3b8: movs r2,#0x0
  0018a3ba: movs r3,#0x1
  0018a3bc: b 0x0018a3d0
  0018a3be: ldr r5,[r0,#0x4]
  0018a3c0: ldr.w r5,[r5,r2,lsl #0x2]
  0018a3c4: adds r2,#0x1
  0018a3c6: ldr r4,[r5,#0x28]
  0018a3c8: cmp r4,#0x8
  0018a3ca: it eq
  0018a3cc: strb.eq.w r3,[r5,#0x25]
  0018a3d0: cmp r1,r2
  0018a3d2: bne 0x0018a3be
  0018a3d4: ldr r0,[0x0018a510]
  0018a3d6: movs r1,#0x1
  0018a3d8: movs r2,#0x8
  0018a3da: add r0,pc
  0018a3dc: ldr r0,[r0,#0x0]
  0018a3de: ldr r0,[r0,#0x0]
  0018a3e0: strb.w r1,[r0,#0x111]
  0018a3e4: movs r1,#0xb
  0018a3e6: ldr.w r0,[r11,#0x78]
  0018a3ea: blx 0x00072754
  0018a3ee: movs r0,#0x0
  0018a3f0: strb.w r0,[r11,#0x5d]
  0018a3f4: strh.w r0,[r11,#0xce]
  0018a3f8: b 0x0018a6de
  0018a3fa: ldr.w r0,[r11,#0x94]
  0018a3fe: mov r2,r6
  0018a400: ldr r1,[sp,#0x34]
  0018a402: ldrb.w r4,[r11,#0x1e4]
  0018a406: blx 0x00074770
  0018a40a: cmp r4,#0x0
  0018a40c: beq.w 0x0018a6d2
  0018a410: cmp r0,#0x2
  0018a412: beq.w 0x0018b004
  0018a416: cmp r0,#0x1
  0018a418: beq.w 0x0018ab74
  0018a41c: cmp r0,#0x0
  0018a41e: bne.w 0x0018a6e4
  0018a422: ldr.w r5,[r11,#0x88]
  0018a426: cbnz r5,0x0018a43c
  0018a428: mov.w r0,#0x240
  0018a42c: blx 0x0006eb24
  0018a430: mov r5,r0
  0018a432: movs r1,#0x1
  0018a434: blx 0x00075268
  0018a438: str.w r5,[r11,#0x88]
  0018a43c: ldr r0,[sp,#0xc8]
  0018a43e: ldr.w r1,[r9,#0x0]
  0018a442: subs r0,r1,r0
  0018a444: itttt eq
  0018a446: mov.eq r0,r5
  0018a448: add.eq sp,#0xcc
  0018a44a: pop.eq.w {r8,r9,r10,r11}
  0018a44e: pop.eq.w {r4,r5,r6,r7,lr}
  0018a452: it eq
  0018a454: b.eq.w 0x001ac828
  0018a458: blx 0x0006e824
  0018a45c: ldr.w r0,[r11,#0x90]
  0018a460: movs r1,#0x0
  0018a462: strb.w r1,[r11,#0xc5]
  0018a466: cbnz r0,0x0018a488
  0018a468: mov.w r0,#0x1e8
  0018a46c: blx 0x0006eb24
  0018a470: mov r5,r0
  0018a472: mov.w r0,#0xffffffff
  0018a476: movs r1,#0x0
  0018a478: str r0,[sp,#0x0]
  0018a47a: mov r0,r5
  0018a47c: movs r2,#0x0
  0018a47e: movs r3,#0x0
  0018a480: blx 0x00075424
  0018a484: str.w r5,[r11,#0x90]
  0018a488: ldr.w r0,[r11,#0x8]
  0018a48c: blx 0x0007258c
  0018a490: ldr r1,[0x0018a514]
  0018a492: movs r2,#0x0
  0018a494: blx 0x000707f8
  0018a498: ldr.w r0,[r11,#0x90]
  0018a49c: blx 0x00078298
  0018a4a0: ldr.w r0,[r11,#0x90]
  0018a4a4: movs r1,#0x1
  0018a4a6: movs r2,#0x0
  0018a4a8: movs r4,#0x1
  0018a4aa: blx 0x00075430
  0018a4ae: strb.w r4,[r11,#0x5d]
  0018a4b2: strb.w r4,[r11,#0xc7]
  0018a4b6: ldr r0,[sp,#0xc8]
  0018a4b8: ldr.w r1,[r9,#0x0]
  0018a4bc: subs r0,r1,r0
  0018a4be: itttt eq
  0018a4c0: mov.eq r0,r11
  0018a4c2: add.eq sp,#0xcc
  0018a4c4: pop.eq.w {r8,r9,r10,r11}
  0018a4c8: pop.eq.w {r4,r5,r6,r7,lr}
  0018a4cc: it eq
  0018a4ce: b.eq.w 0x001ac838
  0018a4d2: blx 0x0006e824
  0018a4d6: ldr.w r0,[r11,#0x78]
  0018a4da: blx 0x00071eb4
  0018a4de: cbz r0,0x0018a52e
  0018a4e0: ldr r1,[r0,#0x0]
  0018a4e2: movs r2,#0x0
  0018a4e4: movs r3,#0x0
  0018a4e6: b 0x0018a52a
  0018a518: ldr r5,[r0,#0x4]
  0018a51a: ldr.w r5,[r5,r3,lsl #0x2]
  0018a51e: adds r3,#0x1
  0018a520: ldr r4,[r5,#0x28]
  0018a522: cmp r4,#0x8
  0018a524: it eq
  0018a526: strb.eq.w r2,[r5,#0x25]
  0018a52a: cmp r1,r3
  0018a52c: bne 0x0018a518
  0018a52e: ldr.w r1,[r11,#0xd0]
  0018a532: ldr.w r0,[r8,#0x0]
  0018a536: rsbs r1,r1
  0018a538: blx 0x0007171c
  0018a53c: ldr.w r0,[r11,#0x78]
  0018a540: movs r1,#0xa
  0018a542: movs r2,#0x8
  0018a544: blx 0x00072754
  0018a548: ldr.w r0,[r8,#0x0]
  0018a54c: movs r1,#0x1
  0018a54e: strb.w r1,[r0,#0x110]
  0018a552: movs r0,#0x0
  0018a554: strb.w r0,[r11,#0x5d]
  0018a558: strh.w r0,[r11,#0xce]
  0018a55c: mov r0,r11
  0018a55e: blx 0x0007828c
  0018a562: mov r8,r9
  0018a564: ldr.w r9,[sp,#0x24]
  0018a568: b 0x0018a6e4
  0018a56a: add.w r10,r4,#0xb
  0018a56e: cmp r4,#0xa5
  0018a570: itt eq
  0018a572: mov.eq.w r10,#0xda
  0018a576: mov.eq r4,#0xd9
  0018a578: ldr r0,[sp,#0x20]
  0018a57a: ldr r0,[r0,#0x0]
  0018a57c: blx 0x0007192c
  0018a580: mov.w r8,#0x1e
  0018a584: cmp r0,#0x0
  0018a586: it ne
  0018a588: mov.ne.w r8,#0x64
  0018a58c: mov.w r9,#0x0
  0018a590: b 0x0018a5bc
  0018a592: mov r0,r5
  0018a594: mov r1,r4
  0018a596: mov r2,r8
  0018a598: blx 0x00072454
  0018a59c: ldr r0,[sp,#0x2c]
  0018a59e: ldr r0,[r0,#0x0]
  0018a5a0: ldr r0,[r0,#0x4]
  0018a5a2: ldr.w r0,[r0,r10,lsl #0x2]
  0018a5a6: movs r1,#0x1
  0018a5a8: blx 0x000718c0
  0018a5ac: mov r1,r0
  0018a5ae: mov r0,r5
  0018a5b0: blx 0x00072cdc
  0018a5b4: movs r0,#0x1
  0018a5b6: sub.w r9,r9,#0x1
  0018a5ba: str r0,[sp,#0x28]
  0018a5bc: mov r0,r5
  0018a5be: mov r1,r4
  0018a5c0: mov r2,r8
  0018a5c2: blx 0x00071a70
  0018a5c6: cmp r0,#0x0
  0018a5c8: bne 0x0018a592
  0018a5ca: cmp.w r9,#0x0
  0018a5ce: beq 0x0018a652
  0018a5d0: ldr r1,[sp,#0x14]
  0018a5d2: add r0,sp,#0x68
  0018a5d4: movs r2,#0x0
  0018a5d6: blx 0x0006ee18
  0018a5da: rsb.w r1,r9,#0x0
  0018a5de: add.w r9,sp,#0x5c
  0018a5e2: mov r0,r9
  0018a5e4: blx 0x0006f658
  0018a5e8: add r0,sp,#0x74
  0018a5ea: add r1,sp,#0x68
  0018a5ec: mov r2,r9
  0018a5ee: blx 0x0006ef98
  0018a5f2: ldr r1,[0x0018a7f8]
  0018a5f4: add r1,pc
  0018a5f6: add.w r8,sp,#0x50
  0018a5fa: movs r2,#0x0
  0018a5fc: mov r0,r8
  0018a5fe: blx 0x0006ee18
  0018a602: add r0,sp,#0x80
  0018a604: add r1,sp,#0x74
  0018a606: mov r2,r8
  0018a608: blx 0x0006ef98
  0018a60c: ldr r0,[0x0018a7fc]
  0018a60e: add r0,pc
  0018a610: ldr r0,[r0,#0x0]
  0018a612: ldr r0,[r0,#0x0]
  0018a614: addw r1,r10,#0x4fa
  0018a618: blx 0x00072f70
  0018a61c: add.w r10,sp,#0x98
  0018a620: mov r2,r0
  0018a622: add r1,sp,#0x80
  0018a624: mov r0,r10
  0018a626: blx 0x0006ef98
  0018a62a: add r0,sp,#0xbc
  0018a62c: mov r1,r10
  0018a62e: blx 0x0006ef5c
  0018a632: ldr r0,[0x0018a800]
  0018a634: add r0,pc
  0018a636: ldr.w r8,[r0,#0x0]
  0018a63a: mov r0,r10
  0018a63c: blx r8
  0018a63e: add r0,sp,#0x80
  0018a640: blx r8
  0018a642: add r0,sp,#0x50
  0018a644: blx r8
  0018a646: add r0,sp,#0x74
  0018a648: blx r8
  0018a64a: mov r0,r9
  0018a64c: blx r8
  0018a64e: add r0,sp,#0x68
  0018a650: blx r8
  0018a652: ldr.w r9,[sp,#0x24]
  0018a656: adds r4,#0x1
  0018a658: ldrd r10,r8,[sp,#0x18]
  0018a65c: cmp r4,#0xa6
  0018a65e: blt 0x0018a56a
  0018a660: movs r0,#0x0
  0018a662: strb.w r0,[r11,#0xca]
  0018a666: ldr r0,[sp,#0x28]
  0018a668: ldr.w r4,[r11,#0x94]
  0018a66c: lsls r0,r0,#0x1f
  0018a66e: beq.w 0x0018b05c
  0018a672: ldr r0,[0x0018a804]
  0018a674: add r0,pc
  0018a676: ldr r0,[r0,#0x0]
  0018a678: ldr r0,[r0,#0x0]
  0018a67a: movw r1,#0x123
  0018a67e: blx 0x00072f70
  0018a682: ldr r1,[0x0018a808]
  0018a684: mov r5,r0
  0018a686: add r1,pc
  0018a688: add r0,sp,#0x74
  0018a68a: movs r2,#0x0
  0018a68c: blx 0x0006ee18
  0018a690: add r0,sp,#0x80
  0018a692: add r2,sp,#0x74
  0018a694: mov r1,r5
  0018a696: blx 0x0006ef98
  0018a69a: add r0,sp,#0x98
  0018a69c: add r1,sp,#0x80
  0018a69e: add r2,sp,#0xbc
  0018a6a0: blx 0x0006ef98
  0018a6a4: add r1,sp,#0x98
  0018a6a6: mov r0,r4
  0018a6a8: blx 0x00074794
  0018a6ac: ldr r0,[0x0018a80c]
  0018a6ae: add r0,pc
  0018a6b0: ldr r4,[r0,#0x0]
  0018a6b2: add r0,sp,#0x98
  0018a6b4: blx r4
  0018a6b6: add r0,sp,#0x80
  0018a6b8: blx r4
  0018a6ba: add r0,sp,#0x74
  0018a6bc: blx r4
  0018a6be: b.w 0x0018b074
  0018a6c2: movs r0,#0x0
  0018a6c4: strb.w r0,[r11,#0xce]
  0018a6c8: strb.w r0,[r11,#0xca]
  0018a6cc: strb.w r0,[r11,#0x5d]
  0018a6d0: b 0x0018a6de
  0018a6d2: cbnz r0,0x0018a6e4
  0018a6d4: movs r0,#0x0
  0018a6d6: strb.w r0,[r11,#0x5d]
  0018a6da: strb.w r0,[r11,#0xce]
  0018a6de: mov r0,r11
  0018a6e0: blx 0x0007828c
  0018a6e4: ldr.w r0,[r11,#0x74]
  0018a6e8: mov r2,r6
  0018a6ea: ldr r1,[sp,#0x34]
  0018a6ec: ldr r3,[sp,#0x30]
  0018a6ee: blx 0x00078214
  0018a6f2: cmp r0,#0x0
  0018a6f4: str.w r0,[r11,#0xf8]
  0018a6f8: beq 0x0018a704
  0018a6fa: movs r1,#0x0
  0018a6fc: lsls r0,r0,#0x1f
  0018a6fe: strd r1,r1,[r11,#0x98]
  0018a702: bne 0x0018a726
  0018a704: ldr.w r0,[r11,#0x7c]
  0018a708: blx 0x000782d4
  0018a70c: cbnz r0,0x0018a71a
  0018a70e: ldr.w r0,[r10,#0x0]
  0018a712: blx 0x000724b4
  0018a716: cmp r0,#0x0
  0018a718: beq 0x0018a814
  0018a71a: ldr.w r0,[r11,#0x7c]
  0018a71e: blx 0x000782e0
  0018a722: b.w 0x0018b004
  0018a726: mov r0,r11
  0018a728: blx 0x0007822c
  0018a72c: ldrb.w r0,[r11,#0x5d]
  0018a730: cmp r0,#0x0
  0018a732: bne 0x0018a7ea
  0018a734: ldr.w r5,[r11,#0x88]
  0018a738: cbnz r5,0x0018a74e
  0018a73a: mov.w r0,#0x240
  0018a73e: blx 0x0006eb24
  0018a742: mov r5,r0
  0018a744: movs r1,#0x1
  0018a746: blx 0x00075268
  0018a74a: str.w r5,[r11,#0x88]
  0018a74e: ldr.w r0,[r11,#0x7c]
  0018a752: blx 0x000782ec
  0018a756: mov r1,r0
  0018a758: mov r0,r5
  0018a75a: blx 0x000782f8
  0018a75e: movs r0,#0x1
  0018a760: strb.w r0,[r11,#0x5d]
  0018a764: mov r0,r11
  0018a766: blx 0x0007822c
  0018a76a: ldr r0,[0x0018a810]
  0018a76c: movs r1,#0x2
  0018a76e: add r0,pc
  0018a770: ldr r5,[r0,#0x0]
  0018a772: ldrb.w r0,[r11,#0x5d]
  0018a776: strb.w r0,[r11,#0x1a6]
  0018a77a: ldr r0,[r5,#0x0]
  0018a77c: blx 0x00078304
  0018a780: eor r0,r0,#0x1
  0018a784: strb.w r0,[r11,#0x1a4]
  0018a788: ldr r0,[r5,#0x0]
  0018a78a: blx 0x00078310
  0018a78e: ldr.w r0,[r11,#0x58]
  0018a792: blx 0x00078190
  0018a796: ldr.w r0,[r11,#0x78]
  0018a79a: blx 0x00071eb4
  0018a79e: mov r4,r0
  0018a7a0: cbz r0,0x0018a7b8
  0018a7a2: movs r6,#0x0
  0018a7a4: b 0x0018a7b2
  0018a7a6: ldr r0,[r4,#0x4]
  0018a7a8: ldr.w r0,[r0,r6,lsl #0x2]
  0018a7ac: blx 0x0007819c
  0018a7b0: adds r6,#0x1
  0018a7b2: ldr r0,[r4,#0x0]
  0018a7b4: cmp r6,r0
  0018a7b6: bcc 0x0018a7a6
  0018a7b8: ldrb.w r0,[r11,#0x5f]
  0018a7bc: movs r6,#0x1
  0018a7be: ldr.w r4,[r11,#0x88]
  0018a7c2: movs r1,#0x1
  0018a7c4: cbnz r0,0x0018a7d0
  0018a7c6: ldr.w r0,[r10,#0x0]
  0018a7ca: blx 0x000724b4
  0018a7ce: mov r1,r0
  0018a7d0: mov r0,r4
  0018a7d2: blx 0x0007831c
  0018a7d6: strb.w r6,[r11,#0xc9]
  0018a7da: movs r1,#0x0
  0018a7dc: ldr r0,[r5,#0x0]
  0018a7de: movs r2,#0x0
  0018a7e0: str r1,[sp,#0x0]
  0018a7e2: movs r1,#0x7b
  0018a7e4: movs r3,#0x0
  0018a7e6: blx 0x00071548
  0018a7ea: ldr.w r0,[r11,#0x74]
  0018a7ee: blx 0x00077a1c
  0018a7f2: b.w 0x0018b004
  0018a814: ldrb.w r0,[r11,#0x5f]
  0018a818: cmp r0,#0x0
  0018a81a: bne.w 0x0018b004
  0018a81e: ldrb.w r0,[r11,#0xdc]
  0018a822: cmp r0,#0x0
  0018a824: bne.w 0x0018b004
  0018a828: ldrb.w r0,[r11,#0xf8]
  0018a82c: lsls r0,r0,#0x1e
  0018a82e: bpl 0x0018a884
  0018a830: ldr.w r0,[r10,#0x0]
  0018a834: blx 0x00078328
  0018a838: cbz r0,0x0018a884
  0018a83a: ldr.w r0,[r10,#0x0]
  0018a83e: blx 0x00077b78
  0018a842: cbnz r0,0x0018a884
  0018a844: ldr.w r0,[r10,#0x0]
  0018a848: blx 0x00077adc
  0018a84c: cbnz r0,0x0018a884
  0018a84e: ldr.w r0,[r10,#0x0]
  0018a852: blx 0x00075928
  0018a856: cbnz r0,0x0018a884
  0018a858: ldr.w r0,[r11,#0x58]
  0018a85c: mov.w r1,#0x3f800000
  0018a860: mov.w r4,#0x3f800000
  0018a864: blx 0x00078268
  0018a868: ldr.w r0,[r11,#0x8]
  0018a86c: ldr r1,[0x0018ac08]
  0018a86e: strd r4,r1,[r11,#0x1b0]
  0018a872: blx 0x0007258c
  0018a876: movs r1,#0x0
  0018a878: str.w r1,[r0,#0x360]
  0018a87c: ldr.w r0,[r11,#0x58]
  0018a880: blx 0x00078334
  0018a884: ldrb.w r0,[r11,#0xf8]
  0018a888: lsls r0,r0,#0x1c
  0018a88a: bpl 0x0018a8d8
  0018a88c: ldr.w r0,[r10,#0x0]
  0018a890: movs r1,#0x1
  0018a892: ldr r0,[r0,#0x0]
  0018a894: blx 0x00072520
  0018a898: cbz r0,0x0018a8d8
  0018a89a: ldr.w r0,[r10,#0x0]
  0018a89e: blx 0x00077adc
  0018a8a2: cbnz r0,0x0018a8d8
  0018a8a4: ldr.w r0,[r10,#0x0]
  0018a8a8: blx 0x0007678c
  0018a8ac: cbnz r0,0x0018a8d8
  0018a8ae: ldr.w r0,[r10,#0x0]
  0018a8b2: blx 0x00075928
  0018a8b6: cbnz r0,0x0018a8d8
  0018a8b8: ldr.w r0,[r10,#0x0]
  0018a8bc: blx 0x00077b48
  0018a8c0: cbnz r0,0x0018a8d8
  0018a8c2: ldr.w r1,[r11,#0x40]
  0018a8c6: movs r2,#0x1
  0018a8c8: ldr.w r0,[r11,#0x58]
  0018a8cc: blx 0x00078340
  0018a8d0: ldr.w r0,[r11,#0x74]
  0018a8d4: blx 0x00077a10
  0018a8d8: ldr.w r0,[r10,#0x0]
  0018a8dc: blx 0x0007834c
  0018a8e0: cbz r0,0x0018a904
  0018a8e2: ldr.w r0,[r11,#0xf8]
  0018a8e6: tst r0,#0x200
  0018a8ea: beq 0x0018a8f8
  0018a8ec: ldr.w r0,[r11,#0x58]
  0018a8f0: blx 0x00078358
  0018a8f4: ldr.w r0,[r11,#0xf8]
  0018a8f8: lsls r0,r0,#0x15
  0018a8fa: itt mi
  0018a8fc: ldr.mi.w r0,[r10,#0x0]
  0018a900: blx.mi 0x00078364
  0018a904: ldr.w r0,[r10,#0x0]
  0018a908: blx 0x00075880
  0018a90c: cmp r0,#0x0
  0018a90e: bne.w 0x0018b004
  0018a912: ldr.w r0,[r10,#0x0]
  0018a916: blx 0x00077b6c
  0018a91a: cbz r0,0x0018a958
  0018a91c: ldrb.w r0,[r11,#0xfb]
  0018a920: lsls r0,r0,#0x1a
  0018a922: bpl 0x0018a958
  0018a924: ldr.w r4,[r11,#0x58]
  0018a928: mov r0,r4
  0018a92a: blx 0x00076c3c
  0018a92e: eor r1,r0,#0x1
  0018a932: mov r0,r4
  0018a934: blx 0x00076c48
  0018a938: ldr.w r0,[r11,#0x58]
  0018a93c: ldr.w r4,[r11,#0x74]
  0018a940: blx 0x00076c3c
  0018a944: ldr.w r2,[r11,#0x58]
  0018a948: movs r1,#0x21
  0018a94a: cmp r0,#0x0
  0018a94c: it ne
  0018a94e: mov.ne r1,#0x20
  0018a950: mov r0,r4
  0018a952: movs r3,#0x0
  0018a954: blx 0x00072418
  0018a958: ldr.w r0,[r11,#0xf8]
  0018a95c: tst r0,#0x40
  0018a960: bne.w 0x0018ac72
  0018a964: ldrb.w r1,[r11,#0xc8]
  0018a968: cmp r1,#0x0
  0018a96a: beq.w 0x0018acea
  0018a96e: lsls r1,r0,#0xa
  0018a970: bpl 0x0018a97e
  0018a972: ldr.w r0,[r11,#0x7c]
  0018a976: blx 0x00076af8
  0018a97a: ldr.w r0,[r11,#0xf8]
  0018a97e: lsls r1,r0,#0x9
  0018a980: bpl 0x0018a9ac
  0018a982: ldr.w r0,[r11,#0x78]
  0018a986: ldr.w r4,[r11,#0x58]
  0018a98a: blx 0x000725b0
  0018a98e: ldr r0,[r0,#0x4]
  0018a990: ldr r1,[r0,#0x4]
  0018a992: mov r0,r4
  0018a994: blx 0x000728d4
  0018a998: ldr.w r2,[r11,#0x58]
  0018a99c: movs r1,#0xc
  0018a99e: ldr.w r0,[r11,#0x74]
  0018a9a2: movs r3,#0x0
  0018a9a4: blx 0x00072418
  0018a9a8: ldr.w r0,[r11,#0xf8]
  0018a9ac: lsls r1,r0,#0x8
  0018a9ae: bpl 0x0018a9da
  0018a9b0: ldr.w r0,[r11,#0x78]
  0018a9b4: ldr.w r4,[r11,#0x58]
  0018a9b8: blx 0x000725b0
  0018a9bc: ldr r0,[r0,#0x4]
  0018a9be: ldr r1,[r0,#0x0]
  0018a9c0: mov r0,r4
  0018a9c2: blx 0x000728d4
  0018a9c6: ldr.w r2,[r11,#0x58]
  0018a9ca: movs r1,#0xa
  0018a9cc: ldr.w r0,[r11,#0x74]
  0018a9d0: movs r3,#0x0
  0018a9d2: blx 0x00072418
  0018a9d6: ldr.w r0,[r11,#0xf8]
  0018a9da: lsls r1,r0,#0x7
  0018a9dc: bpl 0x0018aa06
  0018a9de: ldr.w r0,[r11,#0x78]
  0018a9e2: ldr.w r4,[r11,#0x58]
  0018a9e6: blx 0x00078370
  0018a9ea: mov r1,r0
  0018a9ec: mov r0,r4
  0018a9ee: blx 0x000728d4
  0018a9f2: ldr.w r2,[r11,#0x58]
  0018a9f6: movs r1,#0xe
  0018a9f8: ldr.w r0,[r11,#0x74]
  0018a9fc: movs r3,#0x0
  0018a9fe: blx 0x00072418
  0018aa02: ldr.w r0,[r11,#0xf8]
  0018aa06: lsls r0,r0,#0x6
  0018aa08: str.w r8,[sp,#0x1c]
  0018aa0c: bpl 0x0018aa40
  0018aa0e: ldr.w r0,[r11,#0x78]
  0018aa12: blx 0x00071ed8
  0018aa16: cbz r0,0x0018aa40
  0018aa18: ldr.w r0,[r11,#0x78]
  0018aa1c: ldr.w r4,[r11,#0x58]
  0018aa20: blx 0x00071ed8
  0018aa24: blx 0x000728bc
  0018aa28: mov r1,r0
  0018aa2a: mov r0,r4
  0018aa2c: blx 0x000728d4
  0018aa30: ldr.w r2,[r11,#0x58]
  0018aa34: movs r1,#0xd
  0018aa36: ldr.w r0,[r11,#0x74]
  0018aa3a: movs r3,#0x0
  0018aa3c: blx 0x00072418
  0018aa40: movs r6,#0x0
  0018aa42: mov.w r8,#0x4000000
  0018aa46: movs r4,#0x0
  0018aa48: b 0x0018aab0
  0018aa4a: ldr.w r0,[r11,#0xf8]
  0018aa4e: lsl.w r1,r8,r4
  0018aa52: tst r0,r1
  0018aa54: beq 0x0018aaae
  0018aa56: ldr.w r0,[r11,#0x78]
  0018aa5a: mov r1,r4
  0018aa5c: blx 0x00072a48
  0018aa60: cbz r0,0x0018aaae
  0018aa62: ldr.w r0,[r11,#0x78]
  0018aa66: mov r1,r4
  0018aa68: blx 0x00072a48
  0018aa6c: ldr.w r1,[r11,#0x80]
  0018aa70: movs r3,#0x0
  0018aa72: str r0,[r1,#0x4]
  0018aa74: movs r1,#0x22
  0018aa76: ldr.w r2,[r11,#0x58]
  0018aa7a: ldr.w r0,[r11,#0x74]
  0018aa7e: blx 0x00072418
  0018aa82: ldr.w r0,[r11,#0x78]
  0018aa86: mov r1,r4
  0018aa88: ldr.w r5,[r11,#0x58]
  0018aa8c: blx 0x00072a48
  0018aa90: ldr.w r2,[r11,#0x80]
  0018aa94: mov r1,r0
  0018aa96: mov r0,r5
  0018aa98: blx 0x00072838
  0018aa9c: ldr.w r0,[r11,#0x80]
  0018aaa0: strd r6,r6,[r0,#0x8]
  0018aaa4: str r6,[r0,#0x10]
  0018aaa6: ldr.w r0,[r11,#0x74]
  0018aaaa: blx 0x00077a1c
  0018aaae: adds r4,#0x1
  0018aab0: ldr.w r0,[r11,#0x78]
  0018aab4: blx 0x00072a54
  0018aab8: cmp r4,r0
  0018aaba: blt 0x0018aa4a
  0018aabc: movs r0,#0x0
  0018aabe: strb.w r0,[r11,#0xd6]
  0018aac2: strb.w r0,[r11,#0xc8]
  0018aac6: strb.w r0,[r11,#0x5d]
  0018aaca: mov r0,r11
  0018aacc: blx 0x0007828c
  0018aad0: ldr.w r0,[r11,#0x74]
  0018aad4: blx 0x00077a04
  0018aad8: ldr.w r0,[r11,#0x58]
  0018aadc: ldr.w r8,[sp,#0x1c]
  0018aae0: cmp r0,#0x0
  0018aae2: beq.w 0x0018acea
  0018aae6: blx 0x00072ca0
  0018aaea: b 0x0018acea
  0018aaec: ldrb.w r0,[r11,#0x5e]
  0018aaf0: cmp r0,#0x0
  0018aaf2: beq.w 0x0018ac1c
  0018aaf6: ldr.w r0,[r11,#0x8c]
  0018aafa: mov r2,r6
  0018aafc: ldr r1,[sp,#0x34]
  0018aafe: blx 0x00075130
  0018ab02: cmp r0,#0x0
  0018ab04: beq.w 0x0018b004
  0018ab08: movs r4,#0x0
  0018ab0a: mov r0,r11
  0018ab0c: strb.w r4,[r11,#0x5d]
  0018ab10: blx 0x0007828c
  0018ab14: ldr r0,[0x0018ac0c]
  0018ab16: strb.w r4,[r11,#0x5e]
  0018ab1a: add r0,pc
  0018ab1c: ldr.w r8,[r0,#0x0]
  0018ab20: ldr.w r0,[r8,#0x0]
  0018ab24: blx 0x00072850
  0018ab28: blx 0x00073924
  0018ab2c: mov r4,r0
  0018ab2e: ldr.w r0,[r8,#0x0]
  0018ab32: blx 0x00072850
  0018ab36: cmp r4,#0x0
  0018ab38: beq.w 0x0018adc8
  0018ab3c: blx 0x0007372c
  0018ab40: cbnz r0,0x0018ab50
  0018ab42: ldr.w r0,[r8,#0x0]
  0018ab46: blx 0x00071770
  0018ab4a: cmp r0,#0x2a
  0018ab4c: bne.w 0x0018b07e
  0018ab50: ldr r0,[0x0018ac10]
  0018ab52: movs r1,#0x2
  0018ab54: add r0,pc
  0018ab56: ldr r0,[r0,#0x0]
  0018ab58: ldr r0,[r0,#0x0]
  0018ab5a: blx 0x00074f80
  0018ab5e: ldr.w r0,[r11,#0x8]
  0018ab62: movs r1,#0x0
  0018ab64: strb.w r1,[r11,#0x54]
  0018ab68: movs r1,#0x1
  0018ab6a: blx 0x00071d64
  0018ab6e: ldr.w r0,[r8,#0x0]
  0018ab72: b 0x0018ab9a
  0018ab74: ldr r0,[0x0018ac14]
  0018ab76: movs r1,#0x2
  0018ab78: add r0,pc
  0018ab7a: ldr r0,[r0,#0x0]
  0018ab7c: ldr r0,[r0,#0x0]
  0018ab7e: blx 0x00074f80
  0018ab82: ldr.w r0,[r11,#0x8]
  0018ab86: movs r1,#0x0
  0018ab88: strb.w r1,[r11,#0x54]
  0018ab8c: movs r1,#0x1
  0018ab8e: blx 0x00071d64
  0018ab92: ldr r0,[0x0018ac18]
  0018ab94: add r0,pc
  0018ab96: ldr r0,[r0,#0x0]
  0018ab98: ldr r0,[r0,#0x0]
  0018ab9a: ldr r1,[sp,#0xc8]
  0018ab9c: ldr.w r2,[r9,#0x0]
  0018aba0: subs r1,r2,r1
  0018aba2: itttt eq
  0018aba4: add.eq sp,#0xcc
  0018aba6: pop.eq.w {r8,r9,r10,r11}
  0018abaa: pop.eq.w {r4,r5,r6,r7,lr}
  0018abae: b.eq.w 0x001ac848
  0018abb2: blx 0x0006e824
  0018abb6: ldr.w r0,[r11,#0x90]
  0018abba: mov r2,r6
  0018abbc: ldr r1,[sp,#0x34]
  0018abbe: blx 0x000751e4
  0018abc2: cmp r0,#0x0
  0018abc4: beq.w 0x0018b004
  0018abc8: ldr.w r0,[r11,#0x78]
  0018abcc: blx 0x000727c0
  0018abd0: blx 0x000769c0
  0018abd4: movs r0,#0x0
  0018abd6: strb.w r0,[r11,#0x5d]
  0018abda: strb.w r0,[r11,#0xc7]
  0018abde: mov r0,r11
  0018abe0: blx 0x0007828c
  0018abe4: ldr.w r1,[r11,#0x90]
  0018abe8: ldrb.w r0,[r11,#0xcb]
  0018abec: ldrb r1,[r1,#0x0]
  0018abee: cmp r1,#0x0
  0018abf0: beq.w 0x0018ae08
  0018abf4: cmp r0,#0x0
  0018abf6: beq.w 0x0018af32
  0018abfa: movs r0,#0x0
  0018abfc: strb.w r0,[r11,#0xdd]
  0018ac00: mov r0,r11
  0018ac02: blx 0x000782bc
  0018ac06: b 0x0018af44
  0018ac1c: ldrb.w r0,[r11,#0xc9]
  0018ac20: cmp r0,#0x0
  0018ac22: beq.w 0x0018a6e4
  0018ac26: ldrb.w r0,[r11,#0x15e]
  0018ac2a: cmp r0,#0x0
  0018ac2c: beq.w 0x0018aebc
  0018ac30: ldr r0,[0x0018afc0]
  0018ac32: add r0,pc
  0018ac34: ldr r0,[r0,#0x0]
  0018ac36: ldr r0,[r0,#0x0]
  0018ac38: blx 0x00071704
  0018ac3c: ldrb r1,[r0,#0x5]
  0018ac3e: cmp r1,#0x0
  0018ac40: bne.w 0x0018b004
  0018ac44: ldrb r0,[r0,#0xc]
  0018ac46: cmp r0,#0x0
  0018ac48: bne.w 0x0018b004
  0018ac4c: mov r5,r11
  0018ac4e: ldr.w r0,[r5,#0x88]!
  0018ac52: blx 0x000781e4
  0018ac56: cmp r0,#0x0
  0018ac58: bne.w 0x0018aec0
  0018ac5c: ldr r0,[r5,#0x0]
  0018ac5e: blx 0x000781f0
  0018ac62: cmp r0,#0x0
  0018ac64: bne.w 0x0018aec0
  0018ac68: ldr r3,[sp,#0x30]
  0018ac6a: mov r0,r11
  0018ac6c: blx 0x0007816c
  0018ac70: b 0x0018aec0
  0018ac72: ldr r0,[0x0018afc4]
  0018ac74: add r0,pc
  0018ac76: ldr r5,[r0,#0x0]
  0018ac78: ldr r0,[r5,#0x0]
  0018ac7a: blx 0x000723d0
  0018ac7e: cbz r0,0x0018ac98
  0018ac80: ldr.w r0,[r10,#0x0]
  0018ac84: blx 0x00076804
  0018ac88: cbz r0,0x0018ac98
  0018ac8a: ldr.w r0,[r11,#0x58]
  0018ac8e: movs r1,#0x0
  0018ac90: ldr.w r2,[r11,#0x80]
  0018ac94: blx 0x00072820
  0018ac98: ldr r0,[r5,#0x0]
  0018ac9a: blx 0x000723d0
  0018ac9e: cbz r0,0x0018acbe
  0018aca0: ldr r0,[r5,#0x0]
  0018aca2: blx 0x000723d0
  0018aca6: cbz r0,0x0018acea
  0018aca8: ldr r0,[r5,#0x0]
  0018acaa: blx 0x00071770
  0018acae: cmp r0,#0x9a
  0018acb0: bne 0x0018acea
  0018acb2: ldr.w r0,[r11,#0x78]
  0018acb6: blx 0x00072a54
  0018acba: cmp r0,#0x1
  0018acbc: blt 0x0018acea
  0018acbe: ldr r0,[r5,#0x0]
  0018acc0: blx 0x00071770
  0018acc4: cmp r0,#0x2
  0018acc6: blt 0x0018acea
  0018acc8: ldr r0,[r5,#0x0]
  0018acca: blx 0x00071770
  0018acce: cmp r0,#0x30
  0018acd0: beq 0x0018acea
  0018acd2: ldr.w r0,[r10,#0x0]
  0018acd6: blx 0x00075928
  0018acda: cbnz r0,0x0018acea
  0018acdc: ldr.w r0,[r10,#0x0]
  0018ace0: blx 0x00077b48
  0018ace4: cmp r0,#0x0
  0018ace6: beq.w 0x0018b520
  0018acea: ldrb.w r0,[r11,#0xf8]
  0018acee: lsls r0,r0,#0x1d
  0018acf0: bpl 0x0018ad28
  0018acf2: ldr.w r0,[r10,#0x0]
  0018acf6: blx 0x00077adc
  0018acfa: cbnz r0,0x0018ad28
  0018acfc: ldrb.w r1,[r11,#0xd6]
  0018ad00: ldrb.w r0,[r11,#0x5d]
  0018ad04: cmp r1,#0x0
  0018ad06: eor r2,r1,#0x1
  0018ad0a: strb.w r2,[r11,#0xd6]
  0018ad0e: eor r0,r0,#0x1
  0018ad12: strb.w r0,[r11,#0x5d]
  0018ad16: bne 0x0018ad28
  0018ad18: mov r0,r11
  0018ad1a: blx 0x0007822c
  0018ad1e: ldrd r0,r2,[r11,#0x74]
  0018ad22: movs r1,#0x0
  0018ad24: blx 0x0007837c
  0018ad28: ldrb.w r0,[r11,#0xf8]
  0018ad2c: lsls r0,r0,#0x18
  0018ad2e: bpl 0x0018ad46
  0018ad30: ldr.w r0,[r10,#0x0]
  0018ad34: blx 0x00077adc
  0018ad38: cbnz r0,0x0018ad46
  0018ad3a: ldr.w r0,[r11,#0x14]
  0018ad3e: adds r1,r0,#0x1
  0018ad40: mov r0,r11
  0018ad42: blx 0x00078388
  0018ad46: ldrb.w r0,[r11,#0xd6]
  0018ad4a: cbz r0,0x0018ada8
  0018ad4c: ldr.w r0,[r11,#0xf8]
  0018ad50: tst r0,#0x200
  0018ad54: bne 0x0018adc0
  0018ad56: lsls r1,r0,#0x14
  0018ad58: bmi.w 0x0018ae8c
  0018ad5c: lsls r1,r0,#0x15
  0018ad5e: bmi.w 0x0018afd8
  0018ad62: lsls r1,r0,#0x12
  0018ad64: bmi.w 0x0018b1e4
  0018ad68: lsls r1,r0,#0x11
  0018ad6a: bmi.w 0x0018b51c
  0018ad6e: lsls r1,r0,#0x10
  0018ad70: bmi.w 0x0018b704
  0018ad74: lsls r1,r0,#0xf
  0018ad76: bmi.w 0x0018b9ca
  0018ad7a: lsls r1,r0,#0xe
  0018ad7c: str.w r8,[sp,#0x1c]
  0018ad80: bmi.w 0x0018bc1e
  0018ad84: lsls r1,r0,#0xd
  0018ad86: bmi.w 0x0018bcfa
  0018ad8a: lsls r1,r0,#0xc
  0018ad8c: bmi.w 0x0018be2e
  0018ad90: lsls r1,r0,#0xb
  0018ad92: bmi.w 0x0018beca
  0018ad96: ldr.w r8,[sp,#0x1c]
  0018ad9a: lsls r0,r0,#0x13
  0018ad9c: bpl.w 0x0018afe2
  0018ada0: mov r0,r11
  0018ada2: blx 0x00078394
  0018ada6: b 0x0018afe2
  0018ada8: ldr.w r0,[r11,#0x14]
  0018adac: cmp r0,#0x0
  0018adae: beq 0x0018aeac
  0018adb0: cmp r0,#0x3
  0018adb2: bne.w 0x0018afe8
  0018adb6: ldr r3,[sp,#0x30]
  0018adb8: mov r0,r11
  0018adba: blx 0x0007816c
  0018adbe: b 0x0018afe8
  0018adc0: ldrd r0,r2,[r11,#0x74]
  0018adc4: movs r1,#0x1
  0018adc6: b 0x0018afde
  0018adc8: blx 0x00073930
  0018adcc: cbnz r0,0x0018ade0
  0018adce: ldr.w r0,[r8,#0x0]
  0018add2: blx 0x0007285c
  0018add6: blx 0x00073930
  0018adda: cmp r0,#0x0
  0018addc: beq.w 0x0018b0aa
  0018ade0: ldr.w r0,[r8,#0x0]
  0018ade4: str.w r10,[sp,#0x18]
  0018ade8: blx 0x0007285c
  0018adec: blx 0x00073930
  0018adf0: cmp r0,#0x0
  0018adf2: beq.w 0x0018af5a
  0018adf6: ldr.w r0,[r8,#0x0]
  0018adfa: blx 0x00072850
  0018adfe: blx 0x0007372c
  0018ae02: eor r10,r0,#0x1
  0018ae06: b 0x0018af5e
  0018ae08: cmp r0,#0x0
  0018ae0a: beq.w 0x0018af44
  0018ae0e: ldr.w r0,[r11,#0x58]
  0018ae12: movs r1,#0x0
  0018ae14: movs r6,#0x0
  0018ae16: blx 0x000783a0
  0018ae1a: ldr.w r0,[r11,#0x58]
  0018ae1e: movs r1,#0x0
  0018ae20: blx 0x000728d4
  0018ae24: ldr.w r0,[r11,#0x58]
  0018ae28: ldr r4,[r0,#0x4]
  0018ae2a: ldr.w r0,[r11,#0x78]
  0018ae2e: blx 0x000725b0
  0018ae32: ldr r0,[r0,#0x4]
  0018ae34: add r5,sp,#0xbc
  0018ae36: ldr r0,[r0,#0x4]
  0018ae38: ldr r1,[r0,#0x8]
  0018ae3a: mov r0,r5
  0018ae3c: blx 0x00072574
  0018ae40: mov.w r0,#0x3f800000
  0018ae44: add r2,sp,#0x98
  0018ae46: strd r6,r0,[sp,#0x98]
  0018ae4a: mov r0,r4
  0018ae4c: mov r1,r5
  0018ae4e: str r6,[sp,#0xa0]
  0018ae50: blx 0x000726ac
  0018ae54: ldr.w r0,[r11,#0x78]
  0018ae58: ldr.w r4,[r11,#0x58]
  0018ae5c: blx 0x000725b0
  0018ae60: ldr r0,[r0,#0x4]
  0018ae62: add r5,sp,#0xbc
  0018ae64: ldr r0,[r0,#0x4]
  0018ae66: ldr r1,[r0,#0x8]
  0018ae68: mov r0,r5
  0018ae6a: blx 0x000720b8
  0018ae6e: ldr r0,[0x0018afc8]
  0018ae70: add r2,sp,#0x98
  0018ae72: strd r6,r6,[sp,#0x98]
  0018ae76: mov r1,r5
  0018ae78: str r0,[sp,#0xa0]
  0018ae7a: add r0,sp,#0x44
  0018ae7c: blx 0x0006f1cc
  0018ae80: add r3,sp,#0x44
  0018ae82: mov r0,r4
  0018ae84: ldmia r3,{r1,r2,r3}
  0018ae86: blx 0x00072cb8
  0018ae8a: b 0x0018af44
  0018ae8c: ldr.w r0,[r11,#0x74]
  0018ae90: movs r4,#0x0
  0018ae92: strb.w r4,[r11,#0xd6]
  0018ae96: blx 0x00077a04
  0018ae9a: mov r0,r11
  0018ae9c: strb.w r4,[r11,#0x5d]
  0018aea0: blx 0x0007828c
  0018aea4: mov r0,r11
  0018aea6: blx 0x000783ac
  0018aeaa: b 0x0018afe2
  0018aeac: ldr r1,[sp,#0x34]
  0018aeae: mov r0,r11
  0018aeb0: blx 0x000783b8
  0018aeb4: movs r0,#0x0
  0018aeb6: strh.w r0,[r11,#0x1b8]
  0018aeba: b 0x0018afe8
  0018aebc: add.w r5,r11,#0x88
  0018aec0: ldr r0,[r5,#0x0]
  0018aec2: mov r2,r6
  0018aec4: ldr r1,[sp,#0x34]
  0018aec6: ldr r3,[sp,#0x30]
  0018aec8: blx 0x00075238
  0018aecc: cmp r0,#0x0
  0018aece: beq.w 0x0018b050
  0018aed2: movs r0,#0x0
  0018aed4: strb.w r0,[r11,#0x1a6]
  0018aed8: strb.w r0,[r11,#0x5d]
  0018aedc: mov r0,r11
  0018aede: blx 0x0007828c
  0018aee2: ldr r0,[0x0018afcc]
  0018aee4: movs r1,#0x2
  0018aee6: add r0,pc
  0018aee8: ldr r0,[r0,#0x0]
  0018aeea: ldr r0,[r0,#0x0]
  0018aeec: blx 0x00078304
  0018aef0: mov r1,r0
  0018aef2: ldr.w r0,[r11,#0x58]
  0018aef6: ldrb.w r2,[r11,#0x1a4]
  0018aefa: cmp r1,#0x0
  0018aefc: beq.w 0x0018b1e8
  0018af00: cmp r2,#0x0
  0018af02: bne.w 0x0018b1ee
  0018af06: blx 0x000781a8
  0018af0a: ldr.w r0,[r11,#0x78]
  0018af0e: blx 0x00071eb4
  0018af12: mov r4,r0
  0018af14: cmp r0,#0x0
  0018af16: beq.w 0x0018b5c8
  0018af1a: movs r6,#0x0
  0018af1c: b 0x0018af2a
  0018af1e: ldr r0,[r4,#0x4]
  0018af20: ldr.w r0,[r0,r6,lsl #0x2]
  0018af24: blx 0x000781b4
  0018af28: adds r6,#0x1
  0018af2a: ldr r0,[r4,#0x0]
  0018af2c: cmp r6,r0
  0018af2e: bcc 0x0018af1e
  0018af30: b 0x0018b5c8
  0018af32: ldr r0,[0x0018afd0]
  0018af34: add r0,pc
  0018af36: ldr r0,[r0,#0x0]
  0018af38: ldrb r0,[r0,#0x0]
  0018af3a: cbnz r0,0x0018af44
  0018af3c: ldr.w r0,[r11,#0x7c]
  0018af40: blx 0x00076af8
  0018af44: ldr.w r0,[r11,#0x90]
  0018af48: cbz r0,0x0018af52
  0018af4a: blx 0x00075340
  0018af4e: blx 0x0006eb48
  0018af52: movs r0,#0x0
  0018af54: str.w r0,[r11,#0x90]
  0018af58: b 0x0018b004
  0018af5a: mov.w r10,#0x0
  0018af5e: ldr.w r0,[r8,#0x0]
  0018af62: blx 0x00072850
  0018af66: blx 0x00073930
  0018af6a: mov r1,r0
  0018af6c: ldr.w r0,[r8,#0x0]
  0018af70: cbz r1,0x0018af78
  0018af72: blx 0x00072850
  0018af76: b 0x0018af7c
  0018af78: blx 0x0007285c
  0018af7c: mov r5,r0
  0018af7e: blx 0x000783c4
  0018af82: cbz r0,0x0018afa6
  0018af84: ldr r0,[0x0018afd4]
  0018af86: movs r1,#0x2
  0018af88: add r0,pc
  0018af8a: ldr r0,[r0,#0x0]
  0018af8c: str r1,[r0,#0x0]
  0018af8e: movs r0,#0x0
  0018af90: strb.w r0,[r11,#0x54]
  0018af94: ldr.w r0,[r11,#0x8]
  0018af98: ldr r1,[sp,#0xc8]
  0018af9a: ldr.w r2,[r9,#0x0]
  0018af9e: subs r1,r2,r1
  0018afa0: bne 0x0018b016
  0018afa2: movs r1,#0x1
  0018afa4: b 0x0018b7a2
  0018afa6: mov r0,r5
  0018afa8: blx 0x0007372c
  0018afac: mov r1,r0
  0018afae: ldr.w r0,[r8,#0x0]
  0018afb2: cmp r1,#0x0
  0018afb4: beq.w 0x0018b0b6
  0018afb8: movs r1,#0x1
  0018afba: blx 0x00075070
  0018afbe: b 0x0018b0e2
  0018afd8: ldrd r0,r2,[r11,#0x74]
  0018afdc: movs r1,#0x2
  0018afde: blx 0x0007837c
  0018afe2: ldr.w r0,[r11,#0xf8]
  0018afe6: cbz r0,0x0018b01a
  0018afe8: ldr.w r0,[r10,#0x0]
  0018afec: blx 0x0007681c
  0018aff0: eor r0,r0,#0x1
  0018aff4: orrs.w r0,r0,r8
  0018aff8: ittt eq
  0018affa: ldr.eq r0,[0x0018b3b8]
  0018affc: mov.eq.w r1,#0x3f800000
  0018b000: strd.eq r1,r0,[r11,#0x1b0]
  0018b004: ldr r0,[sp,#0xc8]
  0018b006: ldr.w r1,[r9,#0x0]
  0018b00a: subs r0,r1,r0
  0018b00c: ittt eq
  0018b00e: add.eq sp,#0xcc
  0018b010: pop.eq.w {r8,r9,r10,r11}
  0018b014: pop.eq {r4,r5,r6,r7,pc}
  0018b016: blx 0x0006e824
  0018b01a: ldr.w r0,[r11,#0x74]
  0018b01e: blx 0x00077a04
  0018b022: movs r4,#0x0
  0018b024: mov r0,r11
  0018b026: strb.w r4,[r11,#0xd6]
  0018b02a: strd r4,r4,[r11,#0x98]
  0018b02e: strb.w r4,[r11,#0x5d]
  0018b032: blx 0x0007828c
  0018b036: ldrb.w r0,[r11,#0xc8]
  0018b03a: cmp r0,#0x0
  0018b03c: beq 0x0018afe8
  0018b03e: ldr.w r0,[r11,#0x58]
  0018b042: strb.w r4,[r11,#0xc8]
  0018b046: cmp r0,#0x0
  0018b048: beq 0x0018afe8
  0018b04a: blx 0x00072ca0
  0018b04e: b 0x0018afe8
  0018b050: ldrb.w r0,[r11,#0x15e]
  0018b054: cmp r0,#0x0
  0018b056: bne.w 0x0018b858
  0018b05a: b 0x0018b814
  0018b05c: ldr r0,[0x0018b3bc]
  0018b05e: add r0,pc
  0018b060: ldr r0,[r0,#0x0]
  0018b062: ldr r0,[r0,#0x0]
  0018b064: mov.w r1,#0x124
  0018b068: blx 0x00072f70
  0018b06c: mov r1,r0
  0018b06e: mov r0,r4
  0018b070: blx 0x00074794
  0018b074: add r0,sp,#0xbc
  0018b076: blx 0x0006ee30
  0018b07a: b.w 0x0018a6e4
  0018b07e: ldr.w r0,[r8,#0x0]
  0018b082: blx 0x00072850
  0018b086: blx 0x00072874
  0018b08a: ldr.w r4,[r8,#0x0]
  0018b08e: mov r5,r0
  0018b090: mov r0,r4
  0018b092: blx 0x00072850
  0018b096: cmp r5,#0xc
  0018b098: bne.w 0x0018b374
  0018b09c: blx 0x00071bc0
  0018b0a0: rsbs r1,r0
  0018b0a2: mov r0,r4
  0018b0a4: blx 0x0007171c
  0018b0a8: b 0x0018b888
  0018b0aa: ldr.w r0,[r11,#0x7c]
  0018b0ae: blx 0x000783d0
  0018b0b2: b.w 0x0018b8e2
  0018b0b6: ldr r1,[0x0018b3c0]
  0018b0b8: add r1,pc
  0018b0ba: ldr r1,[r1,#0x0]
  0018b0bc: ldr r1,[r1,#0x0]
  0018b0be: blx 0x00073f90
  0018b0c2: ldr r0,[0x0018b3c4]
  0018b0c4: add r0,pc
  0018b0c6: ldr r0,[r0,#0x0]
  0018b0c8: ldr r4,[r0,#0x0]
  0018b0ca: mov r0,r5
  0018b0cc: blx 0x00071bc0
  0018b0d0: mov r6,r0
  0018b0d2: mov r0,r5
  0018b0d4: blx 0x00074b78
  0018b0d8: adds r1,r0,r6
  0018b0da: mov r0,r4
  0018b0dc: movs r2,#0x0
  0018b0de: blx 0x0007519c
  0018b0e2: mov r0,r5
  0018b0e4: ldr.w r4,[r8,#0x0]
  0018b0e8: blx 0x00071bc0
  0018b0ec: mov r6,r0
  0018b0ee: mov r0,r5
  0018b0f0: blx 0x00074b78
  0018b0f4: adds r1,r0,r6
  0018b0f6: mov r0,r4
  0018b0f8: blx 0x0007171c
  0018b0fc: ldr.w r0,[r11,#0x7c]
  0018b100: movs r1,#0x0
  0018b102: str r1,[r0,#0x0]
  0018b104: mov r0,r5
  0018b106: blx 0x0007372c
  0018b10a: cbz r0,0x0018b12e
  0018b10c: ldr.w r0,[r8,#0x0]
  0018b110: blx 0x00071770
  0018b114: cmp r0,#0xf
  0018b116: bne 0x0018b12e
  0018b118: movs r0,#0x1
  0018b11a: blx 0x0006eb24
  0018b11e: mov r5,r0
  0018b120: blx 0x00072124
  0018b124: ldr.w r4,[r8,#0x0]
  0018b128: mov r0,r5
  0018b12a: movs r1,#0x62
  0018b12c: b 0x0018b16c
  0018b12e: mov r0,r5
  0018b130: blx 0x0007372c
  0018b134: cbz r0,0x0018b142
  0018b136: ldr.w r0,[r8,#0x0]
  0018b13a: blx 0x00071770
  0018b13e: cmp r0,#0x16
  0018b140: beq 0x0018b182
  0018b142: mov r0,r5
  0018b144: blx 0x0007372c
  0018b148: cmp r0,#0x0
  0018b14a: beq 0x0018b21a
  0018b14c: ldr.w r0,[r8,#0x0]
  0018b150: blx 0x00071770
  0018b154: cmp r0,#0x2b
  0018b156: bne 0x0018b21a
  0018b158: movs r0,#0x1
  0018b15a: blx 0x0006eb24
  0018b15e: mov r5,r0
  0018b160: blx 0x00072124
  0018b164: ldr.w r4,[r8,#0x0]
  0018b168: mov r0,r5
  0018b16a: movs r1,#0xa
  0018b16c: blx 0x000737a4
  0018b170: mov r1,r0
  0018b172: mov r0,r4
  0018b174: blx 0x0007366c
  0018b178: mov r0,r5
  0018b17a: blx 0x0007213c
  0018b17e: blx 0x0006eb48
  0018b182: ldr r0,[0x0018b3c8]
  0018b184: movs r4,#0x0
  0018b186: add r0,pc
  0018b188: ldr r0,[r0,#0x0]
  0018b18a: str r4,[r0,#0x0]
  0018b18c: ldr.w r0,[r11,#0x58]
  0018b190: ldr r0,[r0,#0x0]
  0018b192: blx 0x000724f0
  0018b196: ldr.w r1,[r8,#0x0]
  0018b19a: str r0,[r1,#0x64]
  0018b19c: ldr.w r0,[r11,#0x58]
  0018b1a0: ldr r0,[r0,#0x0]
  0018b1a2: blx 0x000726c4
  0018b1a6: ldr.w r1,[r8,#0x0]
  0018b1aa: str r0,[r1,#0x5c]
  0018b1ac: ldr.w r0,[r11,#0x58]
  0018b1b0: ldr r0,[r0,#0x0]
  0018b1b2: blx 0x00072928
  0018b1b6: ldr.w r1,[r8,#0x0]
  0018b1ba: str r0,[r1,#0x60]
  0018b1bc: ldr.w r0,[r11,#0x58]
  0018b1c0: ldr r0,[r0,#0x0]
  0018b1c2: blx 0x00072814
  0018b1c6: ldr.w r1,[r8,#0x0]
  0018b1ca: str r0,[r1,#0x68]
  0018b1cc: strb.w r4,[r11,#0x54]
  0018b1d0: ldr.w r0,[r11,#0x8]
  0018b1d4: ldr r1,[sp,#0xc8]
  0018b1d6: ldr.w r2,[r9,#0x0]
  0018b1da: subs r1,r2,r1
  0018b1dc: bne.w 0x0018b016
  0018b1e0: movs r1,#0x5
  0018b1e2: b 0x0018b7a2
  0018b1e4: movs r6,#0x0
  0018b1e6: b 0x0018b9cc
  0018b1e8: cmp r2,#0x0
  0018b1ea: beq.w 0x0018b5a0
  0018b1ee: blx 0x00072b68
  0018b1f2: ldr.w r0,[r11,#0x78]
  0018b1f6: blx 0x00071eb4
  0018b1fa: mov r4,r0
  0018b1fc: cmp r0,#0x0
  0018b1fe: beq.w 0x0018b5c8
  0018b202: movs r6,#0x0
  0018b204: b 0x0018b212
  0018b206: ldr r0,[r4,#0x4]
  0018b208: ldr.w r0,[r0,r6,lsl #0x2]
  0018b20c: blx 0x000780f4
  0018b210: adds r6,#0x1
  0018b212: ldr r0,[r4,#0x0]
  0018b214: cmp r6,r0
  0018b216: bcc 0x0018b206
  0018b218: b 0x0018b5c8
  0018b21a: mov r0,r5
  0018b21c: blx 0x0007372c
  0018b220: cmp r0,#0x0
  0018b222: beq.w 0x0018b416
  0018b226: ldr.w r0,[r8,#0x0]
  0018b22a: blx 0x00071770
  0018b22e: cmp r0,#0x2a
  0018b230: bne.w 0x0018b416
  0018b234: ldr.w r1,[r11,#0x78]
  0018b238: ldr r0,[r1,#0x2c]
  0018b23a: cbz r0,0x0018b248
  0018b23c: blx 0x00073b34
  0018b240: blx 0x0006eb48
  0018b244: ldr.w r1,[r11,#0x78]
  0018b248: ldr.w r0,[r1,#0x28]!
  0018b24c: movs r4,#0x0
  0018b24e: cmp r0,#0x0
  0018b250: str r4,[r1,#0x4]
  0018b252: beq 0x0018b264
  0018b254: blx 0x00073b34
  0018b258: blx 0x0006eb48
  0018b25c: ldr.w r0,[r11,#0x78]
  0018b260: add.w r1,r0,#0x28
  0018b264: str r4,[r1,#0x0]
  0018b266: cmp.w r10,#0x0
  0018b26a: bne 0x0018b2b4
  0018b26c: ldr.w r0,[r11,#0x78]
  0018b270: blx 0x000783dc
  0018b274: ldr r0,[0x0018b3cc]
  0018b276: add r0,pc
  0018b278: ldr r0,[r0,#0x0]
  0018b27a: ldr r1,[r0,#0x0]
  0018b27c: ldr.w r0,[r8,#0x0]
  0018b280: blx 0x00073c3c
  0018b284: ldr.w r0,[r11,#0x58]
  0018b288: movs r1,#0x0
  0018b28a: blx 0x00073cc0
  0018b28e: ldr.w r0,[r11,#0x58]
  0018b292: blx 0x000783e8
  0018b296: cbz r0,0x0018b2a2
  0018b298: ldr r0,[sp,#0x18]
  0018b29a: movs r1,#0x0
  0018b29c: ldr r0,[r0,#0x0]
  0018b29e: blx 0x000728d4
  0018b2a2: ldr.w r0,[r11,#0x58]
  0018b2a6: blx 0x000783f4
  0018b2aa: ldr.w r0,[r11,#0x78]
  0018b2ae: movs r1,#0x0
  0018b2b0: blx 0x00074170
  0018b2b4: mov r0,r5
  0018b2b6: blx 0x0007372c
  0018b2ba: cmp r0,#0x0
  0018b2bc: bne.w 0x0018b8e2
  0018b2c0: mov r0,r5
  0018b2c2: blx 0x00072874
  0018b2c6: cmp r0,#0xb7
  0018b2c8: bne.w 0x0018b8e2
  0018b2cc: ldr r0,[0x0018b3d0]
  0018b2ce: ldr.w r1,[r11,#0x78]
  0018b2d2: add r0,pc
  0018b2d4: ldr r0,[r0,#0x0]
  0018b2d6: ldr r1,[r1,#0x24]
  0018b2d8: str.w r1,[r11,#0x1e0]
  0018b2dc: movs r1,#0xa
  0018b2de: ldr r0,[r0,#0x0]
  0018b2e0: ldr r0,[r0,#0x4]
  0018b2e2: ldr.w r0,[r0,#0x360]
  0018b2e6: blx 0x000718c0
  0018b2ea: mov r4,r0
  0018b2ec: ldr.w r0,[r8,#0x0]
  0018b2f0: blx 0x00071a58
  0018b2f4: mov r1,r4
  0018b2f6: movs r2,#0x0
  0018b2f8: blx 0x0007387c
  0018b2fc: movs r0,#0x78
  0018b2fe: blx 0x0006eb24
  0018b302: ldr r1,[0x0018b3d4]
  0018b304: mov r5,r0
  0018b306: add r1,pc
  0018b308: add r0,sp,#0x38
  0018b30a: movs r2,#0x0
  0018b30c: blx 0x0006ee18
  0018b310: ldr r0,[0x0018b3d8]
  0018b312: add r0,pc
  0018b314: ldr r0,[r0,#0x0]
  0018b316: ldr r0,[r0,#0x0]
  0018b318: movs r1,#0x1
  0018b31a: movs r2,#0x0
  0018b31c: blx 0x00071b48
  0018b320: mov r4,r0
  0018b322: ldr.w r0,[r8,#0x0]
  0018b326: blx 0x00071c14
  0018b32a: blx 0x00071824
  0018b32e: movs r6,#0x0
  0018b330: movs r1,#0x1
  0018b332: add r2,sp,#0x38
  0018b334: strd r6,r6,[sp,#0x0]
  0018b338: strd r0,r1,[sp,#0x8]
  0018b33c: mov r0,r5
  0018b33e: movs r1,#0xb7
  0018b340: mov r3,r4
  0018b342: blx 0x00071c80
  0018b346: add r0,sp,#0x38
  0018b348: blx 0x0006ee30
  0018b34c: ldr.w r0,[r8,#0x0]
  0018b350: mov r1,r5
  0018b352: blx 0x00073c3c
  0018b356: ldr.w r0,[r8,#0x0]
  0018b35a: mov r1,r5
  0018b35c: blx 0x00073f90
  0018b360: ldr r0,[0x0018b3dc]
  0018b362: movs r1,#0x2
  0018b364: strb.w r6,[r11,#0x54]
  0018b368: add r0,pc
  0018b36a: ldr r0,[r0,#0x0]
  0018b36c: ldr r0,[r0,#0x0]
  0018b36e: blx 0x00071d64
  0018b372: b 0x0018b004
  0018b374: blx 0x00072874
  0018b378: cmp r0,#0x3
  0018b37a: beq 0x0018b39e
  0018b37c: ldr.w r0,[r8,#0x0]
  0018b380: blx 0x00072850
  0018b384: blx 0x00072874
  0018b388: cmp r0,#0x5
  0018b38a: beq 0x0018b39e
  0018b38c: ldr.w r0,[r8,#0x0]
  0018b390: blx 0x00072850
  0018b394: blx 0x00072874
  0018b398: cmp r0,#0xb
  0018b39a: bne.w 0x0018b888
  0018b39e: ldr.w r0,[r8,#0x0]
  0018b3a2: blx 0x00071a58
  0018b3a6: blx 0x00073750
  0018b3aa: mov r4,r0
  0018b3ac: cmp r0,#0x0
  0018b3ae: beq.w 0x0018b888
  0018b3b2: movs r5,#0x0
  0018b3b4: b 0x0018b40e
  0018b3e0: ldr r0,[r4,#0x4]
  0018b3e2: ldr.w r0,[r0,r5,lsl #0x2]
  0018b3e6: blx 0x00074ad0
  0018b3ea: cbz r0,0x0018b40c
  0018b3ec: ldr r0,[r4,#0x4]
  0018b3ee: ldr.w r0,[r0,r5,lsl #0x2]
  0018b3f2: blx 0x000718d8
  0018b3f6: cmp r0,#0x74
  0018b3f8: beq.w 0x0018b876
  0018b3fc: ldr r0,[r4,#0x4]
  0018b3fe: ldr.w r0,[r0,r5,lsl #0x2]
  0018b402: blx 0x000718d8
  0018b406: cmp r0,#0x75
  0018b408: beq.w 0x0018b876
  0018b40c: adds r5,#0x1
  0018b40e: ldr r0,[r4,#0x0]
  0018b410: cmp r5,r0
  0018b412: bcc 0x0018b3e0
  0018b414: b 0x0018b888
  0018b416: mov r0,r5
  0018b418: blx 0x0007372c
  0018b41c: cmp r0,#0x0
  0018b41e: beq 0x0018b4a4
  0018b420: ldr.w r0,[r8,#0x0]
  0018b424: blx 0x00071770
  0018b428: cmp r0,#0x41
  0018b42a: bne 0x0018b4a4
  0018b42c: ldr r0,[0x0018b7b0]
  0018b42e: movs r4,#0x1
  0018b430: ldr r5,[sp,#0x18]
  0018b432: add r0,pc
  0018b434: ldr r0,[r0,#0x0]
  0018b436: str r4,[r0,#0x0]
  0018b438: ldr r0,[r5,#0x0]
  0018b43a: ldr r0,[r0,#0x0]
  0018b43c: blx 0x000724f0
  0018b440: ldr.w r1,[r8,#0x0]
  0018b444: str r0,[r1,#0x64]
  0018b446: ldr r0,[r5,#0x0]
  0018b448: ldr r0,[r0,#0x0]
  0018b44a: blx 0x000726c4
  0018b44e: ldr.w r1,[r8,#0x0]
  0018b452: str r0,[r1,#0x5c]
  0018b454: ldr r0,[r5,#0x0]
  0018b456: ldr r0,[r0,#0x0]
  0018b458: blx 0x00072928
  0018b45c: ldr.w r1,[r8,#0x0]
  0018b460: str r0,[r1,#0x60]
  0018b462: ldr r0,[r5,#0x0]
  0018b464: ldr r0,[r0,#0x0]
  0018b466: blx 0x00072814
  0018b46a: ldr r1,[0x0018b7b4]
  0018b46c: ldr.w r2,[r8,#0x0]
  0018b470: add r1,pc
  0018b472: ldr r1,[r1,#0x0]
  0018b474: str r0,[r2,#0x68]
  0018b476: movs r0,#0x1
  0018b478: strb r4,[r1,#0x0]
  0018b47a: blx 0x0006eb24
  0018b47e: mov r5,r0
  0018b480: blx 0x00072124
  0018b484: mov r0,r5
  0018b486: movs r1,#0x64
  0018b488: ldr.w r4,[r8,#0x0]
  0018b48c: blx 0x000737a4
  0018b490: mov r1,r0
  0018b492: mov r0,r4
  0018b494: blx 0x000745fc
  0018b498: mov r0,r5
  0018b49a: blx 0x0007213c
  0018b49e: blx 0x0006eb48
  0018b4a2: b 0x0018b78a
  0018b4a4: mov r0,r5
  0018b4a6: blx 0x0007372c
  0018b4aa: cmp r0,#0x0
  0018b4ac: beq 0x0018b54e
  0018b4ae: ldr.w r0,[r8,#0x0]
  0018b4b2: blx 0x00071770
  0018b4b6: cmp r0,#0x51
  0018b4b8: bne 0x0018b54e
  0018b4ba: ldr r0,[0x0018b7b8]
  0018b4bc: movs r4,#0x1
  0018b4be: add r0,pc
  0018b4c0: ldr r0,[r0,#0x0]
  0018b4c2: str r4,[r0,#0x0]
  0018b4c4: ldr.w r0,[r11,#0x58]
  0018b4c8: ldr r0,[r0,#0x0]
  0018b4ca: blx 0x000724f0
  0018b4ce: ldr.w r1,[r8,#0x0]
  0018b4d2: str r0,[r1,#0x64]
  0018b4d4: ldr.w r0,[r11,#0x58]
  0018b4d8: ldr r0,[r0,#0x0]
  0018b4da: blx 0x000726c4
  0018b4de: ldr.w r1,[r8,#0x0]
  0018b4e2: str r0,[r1,#0x5c]
  0018b4e4: ldr.w r0,[r11,#0x58]
  0018b4e8: ldr r0,[r0,#0x0]
  0018b4ea: blx 0x00072928
  0018b4ee: ldr.w r1,[r8,#0x0]
  0018b4f2: str r0,[r1,#0x60]
  0018b4f4: ldr.w r0,[r11,#0x58]
  0018b4f8: ldr r0,[r0,#0x0]
  0018b4fa: blx 0x00072814
  0018b4fe: ldr r1,[0x0018b7bc]
  0018b500: ldr.w r2,[r8,#0x0]
  0018b504: add r1,pc
  0018b506: ldr r1,[r1,#0x0]
  0018b508: str r0,[r2,#0x68]
  0018b50a: mov r0,r2
  0018b50c: strb r4,[r1,#0x0]
  0018b50e: ldr r1,[r2,#0x78]
  0018b510: blx 0x0007366c
  0018b514: ldr.w r0,[r8,#0x0]
  0018b518: ldr r1,[r0,#0x78]
  0018b51a: b 0x0018b786
  0018b51c: movs r6,#0x1
  0018b51e: b 0x0018b9cc
  0018b520: ldrb.w r0,[r11,#0xc8]
  0018b524: cmp r0,#0x0
  0018b526: beq.w 0x0018b910
  0018b52a: movs r0,#0x0
  0018b52c: strb.w r0,[r11,#0xd6]
  0018b530: strb.w r0,[r11,#0xc8]
  0018b534: strb.w r0,[r11,#0x5d]
  0018b538: mov r0,r11
  0018b53a: blx 0x0007828c
  0018b53e: ldr.w r0,[r11,#0x74]
  0018b542: blx 0x00077a04
  0018b546: ldr.w r0,[r11,#0x58]
  0018b54a: b.w 0x0018aae0
  0018b54e: mov r0,r5
  0018b550: blx 0x0007372c
  0018b554: cmp r0,#0x0
  0018b556: beq.w 0x0018b694
  0018b55a: ldr.w r0,[r8,#0x0]
  0018b55e: blx 0x00071770
  0018b562: cmp r0,#0x4a
  0018b564: bne.w 0x0018b694
  0018b568: movs r0,#0x1
  0018b56a: blx 0x0006eb24
  0018b56e: mov r5,r0
  0018b570: blx 0x00072124
  0018b574: mov r0,r5
  0018b576: movs r1,#0x64
  0018b578: ldr.w r4,[r8,#0x0]
  0018b57c: blx 0x000737a4
  0018b580: mov r1,r0
  0018b582: mov r0,r4
  0018b584: blx 0x0007366c
  0018b588: mov r0,r5
  0018b58a: blx 0x0007213c
  0018b58e: blx 0x0006eb48
  0018b592: ldr r0,[0x0018b7c0]
  0018b594: add r0,pc
  0018b596: ldr r0,[r0,#0x0]
  0018b598: ldr r0,[r0,#0x0]
  0018b59a: blx 0x000766c0
  0018b59e: b 0x0018b182
  0018b5a0: cbz r0,0x0018b5a6
  0018b5a2: blx 0x00072ba4
  0018b5a6: ldr.w r0,[r11,#0x78]
  0018b5aa: blx 0x00071eb4
  0018b5ae: mov r4,r0
  0018b5b0: cbz r0,0x0018b5c8
  0018b5b2: movs r6,#0x0
  0018b5b4: b 0x0018b5c2
  0018b5b6: ldr r0,[r4,#0x4]
  0018b5b8: ldr.w r0,[r0,r6,lsl #0x2]
  0018b5bc: blx 0x00076ab0
  0018b5c0: adds r6,#0x1
  0018b5c2: ldr r0,[r4,#0x0]
  0018b5c4: cmp r6,r0
  0018b5c6: bcc 0x0018b5b6
  0018b5c8: ldr.w r0,[r11,#0x78]
  0018b5cc: movs r4,#0x0
  0018b5ce: strb.w r4,[r11,#0xc9]
  0018b5d2: strd r4,r4,[r11,#0x98]
  0018b5d6: str.w r4,[r11,#0xc0]
  0018b5da: blx 0x000727c0
  0018b5de: blx 0x000769c0
  0018b5e2: ldr r0,[0x0018b7c4]
  0018b5e4: movs r2,#0x0
  0018b5e6: vldr.32 s0,[pc,#0x1e0]
  0018b5ea: movs r1,#0x0
  0018b5ec: add r0,pc
  0018b5ee: ldr r0,[r0,#0x0]
  0018b5f0: vldr.32 s2,[r0,#0x28]
  0018b5f4: ldr.w r0,[r11,#0x78]
  0018b5f8: vcmpe.f32 s2,s0
  0018b5fc: vmrs apsr,fpscr
  0018b600: vcmpe.f32 s2,#0
  0018b604: it gt
  0018b606: mov.gt r2,#0x1
  0018b608: vmrs apsr,fpscr
  0018b60c: it gt
  0018b60e: mov.gt r1,#0x1
  0018b610: blx 0x00078100
  0018b614: ldr.w r0,[r11,#0x88]
  0018b618: ldrb r1,[r0,#0x0]
  0018b61a: cmp r1,#0x0
  0018b61c: beq.w 0x0018b810
  0018b620: ldr r1,[0x0018b7cc]
  0018b622: strb r4,[r0,#0x0]
  0018b624: add r1,pc
  0018b626: ldr r6,[r1,#0x0]
  0018b628: ldr r0,[r6,#0x0]
  0018b62a: blx 0x00071770
  0018b62e: sub.w r1,r0,#0x9a
  0018b632: cmp r1,#0x4
  0018b634: bhi 0x0018b64c
  0018b636: movs r2,#0x1
  0018b638: lsl.w r1,r2,r1
  0018b63c: tst r1,#0x19
  0018b640: beq 0x0018b64c
  0018b642: ldr.w r0,[r11,#0x7c]
  0018b646: blx 0x00078400
  0018b64a: b 0x0018b810
  0018b64c: cmp r0,#0x1
  0018b64e: beq.w 0x0018b7f4
  0018b652: cmp r0,#0x0
  0018b654: bne.w 0x0018b810
  0018b658: ldr r0,[r6,#0x0]
  0018b65a: movs r1,#0x1
  0018b65c: mov.w r8,#0x1
  0018b660: blx 0x00075070
  0018b664: ldr r0,[r6,#0x0]
  0018b666: movs r1,#0x3
  0018b668: blx 0x00076720
  0018b66c: ldr r0,[0x0018b7d0]
  0018b66e: movs r6,#0x0
  0018b670: ldr r1,[0x0018b7d4]
  0018b672: add r0,pc
  0018b674: add r1,pc
  0018b676: ldr r0,[r0,#0x0]
  0018b678: ldr r1,[r1,#0x0]
  0018b67a: str.w r8,[r0,#0x0]
  0018b67e: strb.w r6,[r11,#0x54]
  0018b682: ldr r0,[r1,#0x0]
  0018b684: movs r1,#0x2
  0018b686: blx 0x00071d64
  0018b68a: ldr r0,[0x0018b7d8]
  0018b68c: add r0,pc
  0018b68e: ldr r0,[r0,#0x0]
  0018b690: strb r6,[r0,#0x0]
  0018b692: b 0x0018b810
  0018b694: mov r0,r5
  0018b696: blx 0x0007372c
  0018b69a: cbz r0,0x0018b708
  0018b69c: ldr.w r0,[r8,#0x0]
  0018b6a0: blx 0x00071770
  0018b6a4: cmp r0,#0x5f
  0018b6a6: bne 0x0018b708
  0018b6a8: ldr r0,[0x0018b7dc]
  0018b6aa: movs r5,#0x1
  0018b6ac: add r0,pc
  0018b6ae: ldr r0,[r0,#0x0]
  0018b6b0: str r5,[r0,#0x0]
  0018b6b2: ldr.w r0,[r11,#0x58]
  0018b6b6: ldr r0,[r0,#0x0]
  0018b6b8: blx 0x000724f0
  0018b6bc: ldr.w r1,[r8,#0x0]
  0018b6c0: str r0,[r1,#0x64]
  0018b6c2: ldr.w r0,[r11,#0x58]
  0018b6c6: ldr r0,[r0,#0x0]
  0018b6c8: blx 0x000726c4
  0018b6cc: ldr.w r1,[r8,#0x0]
  0018b6d0: str r0,[r1,#0x5c]
  0018b6d2: ldr.w r0,[r11,#0x58]
  0018b6d6: ldr r0,[r0,#0x0]
  0018b6d8: blx 0x00072928
  0018b6dc: ldr.w r1,[r8,#0x0]
  0018b6e0: str r0,[r1,#0x60]
  0018b6e2: ldr.w r0,[r11,#0x58]
  0018b6e6: ldr r0,[r0,#0x0]
  0018b6e8: blx 0x00072814
  0018b6ec: ldr r1,[0x0018b7e0]
  0018b6ee: ldr r2,[0x0018b7e4]
  0018b6f0: add r1,pc
  0018b6f2: ldr.w r4,[r8,#0x0]
  0018b6f6: add r2,pc
  0018b6f8: ldr r1,[r1,#0x0]
  0018b6fa: ldr r2,[r2,#0x0]
  0018b6fc: str r0,[r4,#0x68]
  0018b6fe: ldr r0,[r1,#0x0]
  0018b700: movs r1,#0xa
  0018b702: b 0x0018b77c
  0018b704: movs r6,#0x2
  0018b706: b 0x0018b9cc
  0018b708: mov r0,r5
  0018b70a: blx 0x0007372c
  0018b70e: cmp r0,#0x0
  0018b710: beq.w 0x0018b93c
  0018b714: ldr.w r0,[r8,#0x0]
  0018b718: blx 0x00071770
  0018b71c: cmp r0,#0x60
  0018b71e: bne.w 0x0018b93c
  0018b722: ldr r0,[0x0018b7e8]
  0018b724: movs r5,#0x1
  0018b726: add r0,pc
  0018b728: ldr r0,[r0,#0x0]
  0018b72a: str r5,[r0,#0x0]
  0018b72c: ldr.w r0,[r11,#0x58]
  0018b730: ldr r0,[r0,#0x0]
  0018b732: blx 0x000724f0
  0018b736: ldr.w r1,[r8,#0x0]
  0018b73a: str r0,[r1,#0x64]
  0018b73c: ldr.w r0,[r11,#0x58]
  0018b740: ldr r0,[r0,#0x0]
  0018b742: blx 0x000726c4
  0018b746: ldr.w r1,[r8,#0x0]
  0018b74a: str r0,[r1,#0x5c]
  0018b74c: ldr.w r0,[r11,#0x58]
  0018b750: ldr r0,[r0,#0x0]
  0018b752: blx 0x00072928
  0018b756: ldr.w r1,[r8,#0x0]
  0018b75a: str r0,[r1,#0x60]
  0018b75c: ldr.w r0,[r11,#0x58]
  0018b760: ldr r0,[r0,#0x0]
  0018b762: blx 0x00072814
  0018b766: ldr r1,[0x0018b7ec]
  0018b768: ldr r2,[0x0018b7f0]
  0018b76a: add r1,pc
  0018b76c: ldr.w r4,[r8,#0x0]
  0018b770: add r2,pc
  0018b772: ldr r1,[r1,#0x0]
  0018b774: ldr r2,[r2,#0x0]
  0018b776: str r0,[r4,#0x68]
  0018b778: ldr r0,[r1,#0x0]
  0018b77a: movs r1,#0x62
  0018b77c: strb r5,[r2,#0x0]
  0018b77e: blx 0x00071c44
  0018b782: mov r1,r0
  0018b784: mov r0,r4
  0018b786: blx 0x000745fc
  0018b78a: movs r0,#0x0
  0018b78c: strb.w r0,[r11,#0x54]
  0018b790: ldr.w r0,[r11,#0x8]
  0018b794: ldr r1,[sp,#0xc8]
  0018b796: ldr.w r2,[r9,#0x0]
  0018b79a: subs r1,r2,r1
  0018b79c: bne.w 0x0018b016
  0018b7a0: movs r1,#0x2
  0018b7a2: add sp,#0xcc
  0018b7a4: pop.w {r8,r9,r10,r11}
  0018b7a8: pop.w {r4,r5,r6,r7,lr}
  0018b7ac: b.w 0x001ab8f8
  0018b7f4: ldr r0,[0x0018bb6c]
  0018b7f6: movs r2,#0x0
  0018b7f8: ldr r1,[0x0018bb70]
  0018b7fa: add r0,pc
  0018b7fc: add r1,pc
  0018b7fe: ldr r0,[r0,#0x0]
  0018b800: ldr r1,[r1,#0x0]
  0018b802: str r2,[r0,#0x0]
  0018b804: strb.w r2,[r11,#0x54]
  0018b808: ldr r0,[r1,#0x0]
  0018b80a: movs r1,#0x5
  0018b80c: blx 0x00071d64
  0018b810: strb.w r4,[r11,#0x15e]
  0018b814: ldr r0,[r5,#0x0]
  0018b816: blx 0x0007840c
  0018b81a: cbz r0,0x0018b84e
  0018b81c: mov r0,r11
  0018b81e: movs r1,#0x1
  0018b820: blx 0x00078418
  0018b824: ldr.w r0,[r11,#0x78]
  0018b828: movs r1,#0x0
  0018b82a: str.w r1,[r11,#0xf8]
  0018b82e: blx 0x000727c0
  0018b832: ldr r1,[sp,#0xc8]
  0018b834: ldr.w r2,[r9,#0x0]
  0018b838: subs r1,r2,r1
  0018b83a: itttt eq
  0018b83c: add.eq sp,#0xcc
  0018b83e: pop.eq.w {r8,r9,r10,r11}
  0018b842: pop.eq.w {r4,r5,r6,r7,lr}
  0018b846: b.eq.w 0x001ac758
  0018b84a: blx 0x0006e824
  0018b84e: ldrb.w r0,[r11,#0x15e]
  0018b852: cmp r0,#0x0
  0018b854: beq.w 0x0018b004
  0018b858: ldr r0,[r5,#0x0]
  0018b85a: blx 0x0007840c
  0018b85e: cmp r0,#0x0
  0018b860: bne.w 0x0018b004
  0018b864: mov r0,r11
  0018b866: movs r1,#0x0
  0018b868: movs r4,#0x0
  0018b86a: blx 0x00078418
  0018b86e: str.w r4,[r11,#0xf8]
  0018b872: b.w 0x0018b004
  0018b876: ldr.w r0,[r8,#0x0]
  0018b87a: blx 0x00071a58
  0018b87e: ldr r1,[r4,#0x4]
  0018b880: ldr.w r1,[r1,r5,lsl #0x2]
  0018b884: blx 0x000738e8
  0018b888: ldr r0,[0x0018bb74]
  0018b88a: add r0,pc
  0018b88c: ldr r4,[r0,#0x0]
  0018b88e: ldr.w r0,[r8,#0x0]
  0018b892: ldr r1,[r4,#0x0]
  0018b894: blx 0x00073f90
  0018b898: ldr.w r0,[r11,#0x78]
  0018b89c: blx 0x000783dc
  0018b8a0: ldr.w r0,[r11,#0x7c]
  0018b8a4: movs r1,#0x0
  0018b8a6: str r1,[r0,#0x0]
  0018b8a8: ldr r1,[r4,#0x0]
  0018b8aa: ldr.w r0,[r8,#0x0]
  0018b8ae: blx 0x00073c3c
  0018b8b2: ldr.w r0,[r11,#0x58]
  0018b8b6: movs r1,#0x0
  0018b8b8: blx 0x00073cc0
  0018b8bc: ldr.w r0,[r11,#0x58]
  0018b8c0: blx 0x000783e8
  0018b8c4: cbz r0,0x0018b8d0
  0018b8c6: ldr.w r0,[r10,#0x0]
  0018b8ca: movs r1,#0x0
  0018b8cc: blx 0x000728d4
  0018b8d0: ldr.w r0,[r11,#0x58]
  0018b8d4: blx 0x000783f4
  0018b8d8: ldr.w r0,[r11,#0x78]
  0018b8dc: movs r1,#0x0
  0018b8de: blx 0x00074170
  0018b8e2: ldr.w r0,[r11,#0x74]
  0018b8e6: movs r1,#0x0
  0018b8e8: strd r1,r1,[r11,#0x30]
  0018b8ec: blx 0x00078424
  0018b8f0: ldr.w r0,[r11,#0x74]
  0018b8f4: ldr r1,[sp,#0xc8]
  0018b8f6: ldr.w r2,[r9,#0x0]
  0018b8fa: subs r1,r2,r1
  0018b8fc: itttt eq
  0018b8fe: add.eq sp,#0xcc
  0018b900: pop.eq.w {r8,r9,r10,r11}
  0018b904: pop.eq.w {r4,r5,r6,r7,lr}
  0018b910: ldr.w r0,[r10,#0x0]
  0018b914: mov r6,r9
  0018b916: str.w r8,[sp,#0x1c]
  0018b91a: mov r4,r10
  0018b91c: blx 0x00077adc
  0018b920: cmp r0,#0x0
  0018b922: beq.w 0x0018bb3a
  0018b926: ldrb.w r0,[r11,#0xc8]
  0018b92a: mov r9,r6
  0018b92c: ldr.w r8,[sp,#0x1c]
  0018b930: mov r10,r4
  0018b932: cmp r0,#0x0
  0018b934: bne.w 0x0018b52a
  0018b938: b.w 0x0018acea
  0018b93c: mov r0,r5
  0018b93e: blx 0x0007372c
  0018b942: cmp r0,#0x0
  0018b944: beq 0x0018ba28
  0018b946: ldr.w r0,[r8,#0x0]
  0018b94a: blx 0x00071770
  0018b94e: cmp r0,#0x64
  0018b950: bne 0x0018ba28
  0018b952: ldr.w r0,[r11,#0x58]
  0018b956: ldr r0,[r0,#0x0]
  0018b958: blx 0x000724f0
  0018b95c: ldr.w r1,[r8,#0x0]
  0018b960: str r0,[r1,#0x64]
  0018b962: ldr.w r0,[r11,#0x58]
  0018b966: ldr r0,[r0,#0x0]
  0018b968: blx 0x000726c4
  0018b96c: ldr.w r1,[r8,#0x0]
  0018b970: str r0,[r1,#0x5c]
  0018b972: ldr.w r0,[r11,#0x58]
  0018b976: ldr r0,[r0,#0x0]
  0018b978: blx 0x00072928
  0018b97c: ldr.w r1,[r8,#0x0]
  0018b980: str r0,[r1,#0x60]
  0018b982: ldr.w r0,[r11,#0x58]
  0018b986: ldr r0,[r0,#0x0]
  0018b988: blx 0x00072814
  0018b98c: ldr r1,[0x0018bb78]
  0018b98e: movs r5,#0x0
  0018b990: ldr r2,[0x0018bb7c]
  0018b992: add r1,pc
  0018b994: ldr r3,[0x0018bb80]
  0018b996: ldr.w r4,[r8,#0x0]
  0018b99a: add r2,pc
  0018b99c: ldr r1,[r1,#0x0]
  0018b99e: add r3,pc
  0018b9a0: ldr r2,[r2,#0x0]
  0018b9a2: str r0,[r4,#0x68]
  0018b9a4: ldr r3,[r3,#0x0]
  0018b9a6: ldr r0,[r1,#0x0]
  0018b9a8: movs r1,#0x78
  0018b9aa: str r5,[r2,#0x0]
  0018b9ac: strb r5,[r3,#0x0]
  0018b9ae: blx 0x00071c44
  0018b9b2: mov r1,r0
  0018b9b4: mov r0,r4
  0018b9b6: blx 0x000745fc
  0018b9ba: ldr r0,[0x0018bb84]
  0018b9bc: movs r1,#0x5
  0018b9be: add r0,pc
  0018b9c0: ldr r0,[r0,#0x0]
  0018b9c2: ldr r0,[r0,#0x0]
  0018b9c4: blx 0x00071d64
  0018b9c8: b 0x0018babe
  0018b9ca: movs r6,#0x3
  0018b9cc: ldr r0,[0x0018bb88]
  0018b9ce: add r0,pc
  0018b9d0: ldr r0,[r0,#0x0]
  0018b9d2: ldr r0,[r0,#0x0]
  0018b9d4: blx 0x00071a58
  0018b9d8: movs r1,#0x1
  0018b9da: blx 0x00072118
  0018b9de: mov r4,r0
  0018b9e0: cbz r0,0x0018ba0c
  0018b9e2: ldr.w r0,[r11,#0x78]
  0018b9e6: blx 0x00072034
  0018b9ea: mov r5,r0
  0018b9ec: ldr r0,[r4,#0x4]
  0018b9ee: ldr.w r0,[r0,r6,lsl #0x2]
  0018b9f2: blx 0x000718d8
  0018b9f6: mov r1,r0
  0018b9f8: mov r0,r5
  0018b9fa: blx 0x000780dc
  0018b9fe: ldr r1,[r4,#0x4]
  0018ba00: ldr.w r0,[r11,#0x74]
  0018ba04: ldr.w r1,[r1,r6,lsl #0x2]
  0018ba08: blx 0x000780e8
  0018ba0c: movs r0,#0x0
  0018ba0e: strb.w r0,[r11,#0x5d]
  0018ba12: strb.w r0,[r11,#0xd6]
  0018ba16: mov r0,r11
  0018ba18: blx 0x0007828c
  0018ba1c: ldr.w r0,[r11,#0x74]
  0018ba20: blx 0x00077a04
  0018ba24: b.w 0x0018afe8
  0018ba28: mov r0,r5
  0018ba2a: blx 0x0007372c
  0018ba2e: cmp r0,#0x0
  0018ba30: beq 0x0018bac6
  0018ba32: ldr.w r0,[r8,#0x0]
  0018ba36: blx 0x00071770
  0018ba3a: cmp r0,#0x6e
  0018ba3c: bne 0x0018bac6
  0018ba3e: ldr.w r0,[r11,#0x58]
  0018ba42: ldr r0,[r0,#0x0]
  0018ba44: blx 0x000724f0
  0018ba48: ldr.w r1,[r8,#0x0]
  0018ba4c: str r0,[r1,#0x64]
  0018ba4e: ldr.w r0,[r11,#0x58]
  0018ba52: ldr r0,[r0,#0x0]
  0018ba54: blx 0x000726c4
  0018ba58: ldr.w r1,[r8,#0x0]
  0018ba5c: str r0,[r1,#0x5c]
  0018ba5e: ldr.w r0,[r11,#0x58]
  0018ba62: ldr r0,[r0,#0x0]
  0018ba64: blx 0x00072928
  0018ba68: ldr.w r1,[r8,#0x0]
  0018ba6c: str r0,[r1,#0x60]
  0018ba6e: ldr.w r0,[r11,#0x58]
  0018ba72: ldr r0,[r0,#0x0]
  0018ba74: blx 0x00072814
  0018ba78: ldr r1,[0x0018bb8c]
  0018ba7a: movs r5,#0x0
  0018ba7c: ldr r2,[0x0018bb90]
  0018ba7e: add r1,pc
  0018ba80: ldr r3,[0x0018bb94]
  0018ba82: ldr.w r4,[r8,#0x0]
  0018ba86: add r2,pc
  0018ba88: ldr r1,[r1,#0x0]
  0018ba8a: add r3,pc
  0018ba8c: ldr r2,[r2,#0x0]
  0018ba8e: str r0,[r4,#0x68]
  0018ba90: ldr r3,[r3,#0x0]
  0018ba92: ldr r0,[r1,#0x0]
  0018ba94: movs r1,#0xa
  0018ba96: str r5,[r2,#0x0]
  0018ba98: strb r5,[r3,#0x0]
  0018ba9a: blx 0x00071c44
  0018ba9e: mov r1,r0
  0018baa0: mov r0,r4
  0018baa2: blx 0x000745fc
  0018baa6: ldr r0,[0x0018bb98]
  0018baa8: movs r1,#0x5
  0018baaa: add r0,pc
  0018baac: ldr r0,[r0,#0x0]
  0018baae: ldr r0,[r0,#0x0]
  0018bab0: blx 0x00071d64
  0018bab4: ldr r0,[0x0018bb9c]
  0018bab6: movs r1,#0x1
  0018bab8: add r0,pc
  0018baba: ldr r0,[r0,#0x0]
  0018babc: strb r1,[r0,#0x0]
  0018babe: strb.w r5,[r11,#0x54]
  0018bac2: b.w 0x0018b004
  0018bac6: mov r0,r5
  0018bac8: blx 0x0007372c
  0018bacc: cmp r0,#0x0
  0018bace: beq 0x0018bbac
  0018bad0: ldr.w r0,[r8,#0x0]
  0018bad4: blx 0x00071770
  0018bad8: cmp r0,#0x78
  0018bada: bne 0x0018bbac
  0018badc: ldr r0,[0x0018bba0]
  0018bade: movs r1,#0x1
  0018bae0: add r0,pc
  0018bae2: ldr r0,[r0,#0x0]
  0018bae4: str r1,[r0,#0x0]
  0018bae6: ldr.w r0,[r11,#0x58]
  0018baea: ldr r0,[r0,#0x0]
  0018baec: blx 0x000724f0
  0018baf0: ldr.w r1,[r8,#0x0]
  0018baf4: str r0,[r1,#0x64]
  0018baf6: ldr.w r0,[r11,#0x58]
  0018bafa: ldr r0,[r0,#0x0]
  0018bafc: blx 0x000726c4
  0018bb00: ldr.w r1,[r8,#0x0]
  0018bb04: str r0,[r1,#0x5c]
  0018bb06: ldr.w r0,[r11,#0x58]
  0018bb0a: ldr r0,[r0,#0x0]
  0018bb0c: blx 0x00072928
  0018bb10: ldr.w r1,[r8,#0x0]
  0018bb14: str r0,[r1,#0x60]
  0018bb16: ldr.w r0,[r11,#0x58]
  0018bb1a: ldr r0,[r0,#0x0]
  0018bb1c: blx 0x00072814
  0018bb20: ldr r1,[0x0018bba4]
  0018bb22: movs r5,#0x0
  0018bb24: ldr r2,[0x0018bba8]
  0018bb26: add r1,pc
  0018bb28: ldr.w r4,[r8,#0x0]
  0018bb2c: add r2,pc
  0018bb2e: ldr r1,[r1,#0x0]
  0018bb30: ldr r2,[r2,#0x0]
  0018bb32: str r0,[r4,#0x68]
  0018bb34: ldr r0,[r1,#0x0]
  0018bb36: movs r1,#0x7e
  0018bb38: b 0x0018bcaa
  0018bb3a: ldr r0,[r4,#0x0]
  0018bb3c: blx 0x0007681c
  0018bb40: cmp r0,#0x0
  0018bb42: beq.w 0x0018bcc0
  0018bb46: ldr.w r0,[r11,#0x58]
  0018bb4a: movs r1,#0x0
  0018bb4c: blx 0x000728d4
  0018bb50: ldr.w r2,[r11,#0x58]
  0018bb54: movs r1,#0x6
  0018bb56: ldr.w r0,[r11,#0x74]
  0018bb5a: movs r3,#0x0
  0018bb5c: blx 0x00072418
  0018bb60: mov r9,r6
  0018bb62: mov r10,r4
  0018bb64: ldr.w r8,[sp,#0x1c]
  0018bb68: b.w 0x0018acea
  0018bbac: mov r0,r5
  0018bbae: blx 0x0007372c
  0018bbb2: cbz r0,0x0018bc24
  0018bbb4: ldr.w r0,[r8,#0x0]
  0018bbb8: blx 0x00071770
  0018bbbc: cmp r0,#0x7e
  0018bbbe: bne 0x0018bc24
  0018bbc0: ldr r0,[0x0018bf64]
  0018bbc2: movs r1,#0x1
  0018bbc4: add r0,pc
  0018bbc6: ldr r0,[r0,#0x0]
  0018bbc8: str r1,[r0,#0x0]
  0018bbca: ldr.w r0,[r11,#0x58]
  0018bbce: ldr r0,[r0,#0x0]
  0018bbd0: blx 0x000724f0
  0018bbd4: ldr.w r1,[r8,#0x0]
  0018bbd8: str r0,[r1,#0x64]
  0018bbda: ldr.w r0,[r11,#0x58]
  0018bbde: ldr r0,[r0,#0x0]
  0018bbe0: blx 0x000726c4
  0018bbe4: ldr.w r1,[r8,#0x0]
  0018bbe8: str r0,[r1,#0x5c]
  0018bbea: ldr.w r0,[r11,#0x58]
  0018bbee: ldr r0,[r0,#0x0]
  0018bbf0: blx 0x00072928
  0018bbf4: ldr.w r1,[r8,#0x0]
  0018bbf8: str r0,[r1,#0x60]
  0018bbfa: ldr.w r0,[r11,#0x58]
  0018bbfe: ldr r0,[r0,#0x0]
  0018bc00: blx 0x00072814
  0018bc04: ldr r1,[0x0018bf68]
  0018bc06: movs r5,#0x0
  0018bc08: ldr r2,[0x0018bf6c]
  0018bc0a: add r1,pc
  0018bc0c: ldr.w r4,[r8,#0x0]
  0018bc10: add r2,pc
  0018bc12: ldr r1,[r1,#0x0]
  0018bc14: ldr r2,[r2,#0x0]
  0018bc16: str r0,[r4,#0x68]
  0018bc18: ldr r0,[r1,#0x0]
  0018bc1a: movs r1,#0x78
  0018bc1c: b 0x0018bd70
  0018bc1e: mov.w r8,#0x1
  0018bc22: b 0x0018bee2
  0018bc24: mov r0,r5
  0018bc26: blx 0x0007372c
  0018bc2a: cbz r0,0x0018bc3a
  0018bc2c: ldr.w r0,[r8,#0x0]
  0018bc30: blx 0x00071770
  0018bc34: cmp r0,#0x7f
  0018bc36: beq.w 0x0018b722
  0018bc3a: mov r0,r5
  0018bc3c: blx 0x0007372c
  0018bc40: cmp r0,#0x0
  0018bc42: beq 0x0018bd00
  0018bc44: ldr.w r0,[r8,#0x0]
  0018bc48: blx 0x00071770
  0018bc4c: cmp r0,#0x86
  0018bc4e: bne 0x0018bd00
  0018bc50: ldr r0,[0x0018bf70]
  0018bc52: movs r5,#0x0
  0018bc54: add r0,pc
  0018bc56: ldr r0,[r0,#0x0]
  0018bc58: str r5,[r0,#0x0]
  0018bc5a: ldr.w r0,[r11,#0x58]
  0018bc5e: ldr r0,[r0,#0x0]
  0018bc60: blx 0x000724f0
  0018bc64: ldr.w r1,[r8,#0x0]
  0018bc68: str r0,[r1,#0x64]
  0018bc6a: ldr.w r0,[r11,#0x58]
  0018bc6e: ldr r0,[r0,#0x0]
  0018bc70: blx 0x000726c4
  0018bc74: ldr.w r1,[r8,#0x0]
  0018bc78: str r0,[r1,#0x5c]
  0018bc7a: ldr.w r0,[r11,#0x58]
  0018bc7e: ldr r0,[r0,#0x0]
  0018bc80: blx 0x00072928
  0018bc84: ldr.w r1,[r8,#0x0]
  0018bc88: str r0,[r1,#0x60]
  0018bc8a: ldr.w r0,[r11,#0x58]
  0018bc8e: ldr r0,[r0,#0x0]
  0018bc90: blx 0x00072814
  0018bc94: ldr r1,[0x0018bf74]
  0018bc96: ldr r2,[0x0018bf78]
  0018bc98: add r1,pc
  0018bc9a: ldr.w r4,[r8,#0x0]
  0018bc9e: add r2,pc
  0018bca0: ldr r1,[r1,#0x0]
  0018bca2: str r0,[r4,#0x68]
  0018bca4: ldr r2,[r2,#0x0]
  0018bca6: ldr r0,[r1,#0x0]
  0018bca8: movs r1,#0x70
  0018bcaa: strb r5,[r2,#0x0]
  0018bcac: blx 0x00071c44
  0018bcb0: mov r1,r0
  0018bcb2: mov r0,r4
  0018bcb4: blx 0x000745fc
  0018bcb8: strb.w r5,[r11,#0x54]
  0018bcbc: b.w 0x0018b1d0
  0018bcc0: ldrb.w r0,[r11,#0xc8]
  0018bcc4: mov r9,r6
  0018bcc6: ldr.w r8,[sp,#0x1c]
  0018bcca: mov r10,r4
  0018bccc: cmp r0,#0x0
  0018bcce: bne.w 0x0018acea
  0018bcd2: ldr.w r0,[r10,#0x0]
  0018bcd6: blx 0x00076804
  0018bcda: ldr.w r8,[r10,#0x0]
  0018bcde: cmp r0,#0x0
  0018bce0: beq.w 0x0018beac
  0018bce4: ldr.w r0,[r11,#0x80]
  0018bce8: blx 0x000781cc
  0018bcec: mov r1,r0
  0018bcee: ldr.w r2,[r11,#0x80]
  0018bcf2: mov r0,r8
  0018bcf4: blx 0x00072820
  0018bcf8: b 0x0018bb50
  0018bcfa: mov.w r8,#0x3
  0018bcfe: b 0x0018bee2
  0018bd00: mov r0,r5
  0018bd02: blx 0x0007372c
  0018bd06: cbz r0,0x0018bd84
  0018bd08: ldr.w r0,[r8,#0x0]
  0018bd0c: blx 0x00071770
  0018bd10: cmp r0,#0x90
  0018bd12: bne 0x0018bd84
  0018bd14: ldr r0,[0x0018bf7c]
  0018bd16: movs r1,#0x1
  0018bd18: add r0,pc
  0018bd1a: ldr r0,[r0,#0x0]
  0018bd1c: str r1,[r0,#0x0]
  0018bd1e: ldr.w r0,[r11,#0x58]
  0018bd22: ldr r0,[r0,#0x0]
  0018bd24: blx 0x000724f0
  0018bd28: ldr.w r1,[r8,#0x0]
  0018bd2c: str r0,[r1,#0x64]
  0018bd2e: ldr.w r0,[r11,#0x58]
  0018bd32: ldr r0,[r0,#0x0]
  0018bd34: blx 0x000726c4
  0018bd38: ldr.w r1,[r8,#0x0]
  0018bd3c: str r0,[r1,#0x5c]
  0018bd3e: ldr.w r0,[r11,#0x58]
  0018bd42: ldr r0,[r0,#0x0]
  0018bd44: blx 0x00072928
  0018bd48: ldr.w r1,[r8,#0x0]
  0018bd4c: str r0,[r1,#0x60]
  0018bd4e: ldr.w r0,[r11,#0x58]
  0018bd52: ldr r0,[r0,#0x0]
  0018bd54: blx 0x00072814
  0018bd58: ldr r1,[0x0018c0a4]
  0018bd5a: movs r5,#0x0
  0018bd5c: ldr r2,[0x0018c0a8]
  0018bd5e: add r1,pc
  0018bd60: ldr.w r4,[r8,#0x0]
  0018bd64: add r2,pc
  0018bd66: ldr r1,[r1,#0x0]
  0018bd68: ldr r2,[r2,#0x0]
  0018bd6a: str r0,[r4,#0x68]
  0018bd6c: ldr r0,[r1,#0x0]
  0018bd6e: movs r1,#0x70
  0018bd70: strb r5,[r2,#0x0]
  0018bd72: blx 0x00071c44
  0018bd76: mov r1,r0
  0018bd78: mov r0,r4
  0018bd7a: blx 0x000745fc
  0018bd7e: strb.w r5,[r11,#0x54]
  0018bd82: b 0x0018b790
  0018bd84: mov r0,r5
  0018bd86: blx 0x0007372c
  0018bd8a: cbz r0,0x0018bdbc
  0018bd8c: ldr.w r0,[r8,#0x0]
  0018bd90: blx 0x00071770
  0018bd94: cmp r0,#0x9b
  0018bd96: bne 0x0018bdbc
  0018bd98: ldr r0,[0x0018c0ac]
  0018bd9a: movs r3,#0x1
  0018bd9c: ldr r1,[0x0018c0b0]
  0018bd9e: ldr r2,[0x0018c0b4]
  0018bda0: add r0,pc
  0018bda2: add r1,pc
  0018bda4: ldr.w r4,[r8,#0x0]
  0018bda8: add r2,pc
  0018bdaa: ldr r0,[r0,#0x0]
  0018bdac: ldr r1,[r1,#0x0]
  0018bdae: ldr r2,[r2,#0x0]
  0018bdb0: ldr r0,[r0,#0x0]
  0018bdb2: strb r3,[r1,#0x0]
  0018bdb4: str r3,[r2,#0x0]
  0018bdb6: ldr.w r1,[r4,#0x84]
  0018bdba: b 0x0018b77e
  0018bdbc: mov r0,r5
  0018bdbe: blx 0x0007372c
  0018bdc2: cbz r0,0x0018be34
  0018bdc4: ldr.w r0,[r8,#0x0]
  0018bdc8: blx 0x00071770
  0018bdcc: cmp r0,#0xa1
  0018bdce: bne 0x0018be34
  0018bdd0: ldr.w r0,[r11,#0x58]
  0018bdd4: ldr r0,[r0,#0x0]
  0018bdd6: blx 0x000724f0
  0018bdda: ldr.w r1,[r8,#0x0]
  0018bdde: str r0,[r1,#0x64]
  0018bde0: ldr.w r0,[r11,#0x58]
  0018bde4: ldr r0,[r0,#0x0]
  0018bde6: blx 0x000726c4
  0018bdea: ldr.w r1,[r8,#0x0]
  0018bdee: str r0,[r1,#0x5c]
  0018bdf0: ldr.w r0,[r11,#0x58]
  0018bdf4: ldr r0,[r0,#0x0]
  0018bdf6: blx 0x00072928
  0018bdfa: ldr.w r1,[r8,#0x0]
  0018bdfe: str r0,[r1,#0x60]
  0018be00: ldr.w r0,[r11,#0x58]
  0018be04: ldr r0,[r0,#0x0]
  0018be06: blx 0x00072814
  0018be0a: ldr r1,[0x0018c0b8]
  0018be0c: movs r5,#0x0
  0018be0e: ldr r2,[0x0018c0bc]
  0018be10: add r1,pc
  0018be12: ldr r3,[0x0018c0c0]
  0018be14: ldr.w r4,[r8,#0x0]
  0018be18: add r2,pc
  0018be1a: ldr r1,[r1,#0x0]
  0018be1c: add r3,pc
  0018be1e: ldr r2,[r2,#0x0]
  0018be20: str r0,[r4,#0x68]
  0018be22: ldr r3,[r3,#0x0]
  0018be24: ldr r0,[r1,#0x0]
  0018be26: movs r1,#0x5d
  0018be28: str r5,[r2,#0x0]
  0018be2a: strb r5,[r3,#0x0]
  0018be2c: b 0x0018bd72
  0018be2e: mov.w r8,#0x2
  0018be32: b 0x0018bee2
  0018be34: mov r0,r5
  0018be36: blx 0x0007372c
  0018be3a: cmp r0,#0x0
  0018be3c: beq.w 0x0018b266
  0018be40: ldr.w r0,[r8,#0x0]
  0018be44: blx 0x00071770
  0018be48: cmp r0,#0xa2
  0018be4a: bne.w 0x0018b266
  0018be4e: ldr.w r0,[r11,#0x58]
  0018be52: ldr r0,[r0,#0x0]
  0018be54: blx 0x000724f0
  0018be58: ldr.w r1,[r8,#0x0]
  0018be5c: str r0,[r1,#0x64]
  0018be5e: ldr.w r0,[r11,#0x58]
  0018be62: ldr r0,[r0,#0x0]
  0018be64: blx 0x000726c4
  0018be68: ldr.w r1,[r8,#0x0]
  0018be6c: str r0,[r1,#0x5c]
  0018be6e: ldr.w r0,[r11,#0x58]
  0018be72: ldr r0,[r0,#0x0]
  0018be74: blx 0x00072928
  0018be78: ldr.w r1,[r8,#0x0]
  0018be7c: str r0,[r1,#0x60]
  0018be7e: ldr.w r0,[r11,#0x58]
  0018be82: ldr r0,[r0,#0x0]
  0018be84: blx 0x00072814
  0018be88: ldr r1,[0x0018c0c4]
  0018be8a: movs r5,#0x0
  0018be8c: ldr r2,[0x0018c0c8]
  0018be8e: ldr r3,[0x0018c0cc]
  0018be90: add r1,pc
  0018be92: add r2,pc
  0018be94: ldr.w r4,[r8,#0x0]
  0018be98: add r3,pc
  0018be9a: ldr r1,[r1,#0x0]
  0018be9c: ldr r2,[r2,#0x0]
  0018be9e: ldr r3,[r3,#0x0]
  0018bea0: str r0,[r4,#0x68]
  0018bea2: ldr r0,[r1,#0x0]
  0018bea4: movs r1,#0x5d
  0018bea6: str r5,[r2,#0x0]
  0018bea8: strb r5,[r3,#0x0]
  0018beaa: b 0x0018bcac
  0018beac: mov r0,r8
  0018beae: blx 0x00076810
  0018beb2: cmp r0,#0x0
  0018beb4: beq 0x0018bf4e
  0018beb6: ldr.w r2,[r11,#0x80]
  0018beba: movs r0,#0x0
  0018bebc: movs r1,#0x0
  0018bebe: str r0,[r2,#0x4]
  0018bec0: ldr.w r0,[r11,#0x58]
  0018bec4: blx 0x00072838
  0018bec8: b 0x0018bb50
  0018beca: ldr r0,[0x0018c0d0]
  0018becc: mov.w r8,#0x0
  0018bed0: add r0,pc
  0018bed2: ldr r0,[r0,#0x0]
  0018bed4: ldr r0,[r0,#0x0]
  0018bed6: ldrb.w r1,[r0,#0xf8]
  0018beda: eor r1,r1,#0x1
  0018bede: strb.w r1,[r0,#0xf8]
  0018bee2: ldr.w r0,[r11,#0x78]
  0018bee6: blx 0x00071eb4
  0018beea: mov r5,r0
  0018beec: cbz r0,0x0018bf2e
  0018beee: movs r4,#0x0
  0018bef0: b 0x0018bf28
  0018bef2: ldr r0,[r5,#0x4]
  0018bef4: ldr.w r6,[r0,r4,lsl #0x2]
  0018bef8: mov r0,r6
  0018befa: blx 0x00072ed4
  0018befe: cbz r0,0x0018bf26
  0018bf00: mov r0,r6
  0018bf02: blx 0x00071ec0
  0018bf06: cbnz r0,0x0018bf26
  0018bf08: cmp.w r8,#0x3
  0018bf0c: bne 0x0018bf1a
  0018bf0e: ldr.w r0,[r11,#0x80]
  0018bf12: blx 0x00078430
  0018bf16: mov r2,r0
  0018bf18: b 0x0018bf1c
  0018bf1a: movs r2,#0x0
  0018bf1c: ldr r0,[r6,#0x0]
  0018bf1e: mov r1,r8
  0018bf20: ldr r3,[r0,#0x10]
  0018bf22: mov r0,r6
  0018bf24: blx r3
  0018bf26: adds r4,#0x1
  0018bf28: ldr r0,[r5,#0x0]
  0018bf2a: cmp r4,r0
  0018bf2c: bcc 0x0018bef2
  0018bf2e: ldr.w r0,[r11,#0x74]
  0018bf32: movs r4,#0x0
  0018bf34: strb.w r4,[r11,#0xd6]
  0018bf38: blx 0x00077a04
  0018bf3c: mov r0,r11
  0018bf3e: strb.w r4,[r11,#0x5d]
  0018bf42: blx 0x0007828c
  0018bf46: ldr.w r8,[sp,#0x1c]
  0018bf4a: b.w 0x0018afe2
  0018bf4e: ldr r0,[r4,#0x0]
  0018bf50: blx 0x00077b84
  0018bf54: cbz r0,0x0018bf80
  0018bf56: ldr.w r2,[r11,#0x80]
  0018bf5a: ldr.w r0,[r11,#0x58]
  0018bf5e: ldr r1,[r2,#0x24]
  0018bf60: b 0x0018bcf4
  0018bf80: ldr r0,[r5,#0x0]
  0018bf82: blx 0x000723d0
  0018bf86: ldr.w r8,[sp,#0x1c]
  0018bf8a: cmp r0,#0x0
  0018bf8c: mov r9,r6
  0018bf8e: mov r10,r4
  0018bf90: beq 0x0018bfaa
  0018bf92: ldr r0,[r5,#0x0]
  0018bf94: blx 0x000723d0
  0018bf98: cmp r0,#0x0
  0018bf9a: beq.w 0x0018acea
  0018bf9e: ldr r0,[r5,#0x0]
  0018bfa0: blx 0x00071770
  0018bfa4: cmp r0,#0x9a
  0018bfa6: bne.w 0x0018acea
  0018bfaa: ldr r0,[r5,#0x0]
  0018bfac: blx 0x00072850
  0018bfb0: cbz r0,0x0018bfc2
  0018bfb2: ldr r0,[r5,#0x0]
  0018bfb4: blx 0x00072850
  0018bfb8: blx 0x00072874
  0018bfbc: cmp r0,#0xb7
  0018bfbe: beq.w 0x0018acea
  0018bfc2: movs r4,#0x1
  0018bfc4: mov r0,r11
  0018bfc6: strb.w r4,[r11,#0x5d]
  0018bfca: strb.w r4,[r11,#0xd6]
  0018bfce: blx 0x0007822c
  0018bfd2: ldrd r0,r2,[r11,#0x74]
  0018bfd6: movs r1,#0x3
  0018bfd8: blx 0x0007837c
  0018bfdc: strb.w r4,[r11,#0xc8]
  0018bfe0: b.w 0x0018acea
  001ab8f8: bx pc
  001ab8fc: ldr r12,[0x1ab904]
  001ab900: add pc,r12,pc
  001ac758: bx pc
  001ac75c: ldr r12,[0x1ac764]
  001ac760: add pc,r12,pc
  001ac818: bx pc
  001ac81c: ldr r12,[0x1ac824]
  001ac820: add pc,r12,pc
  001ac828: bx pc
  001ac82c: ldr r12,[0x1ac834]
  001ac830: add pc,r12,pc
  001ac838: bx pc
  001ac83c: ldr r12,[0x1ac844]
  001ac840: add pc,r12,pc
  001ac848: bx pc
  001ac84c: ldr r12,[0x1ac854]
  001ac850: add pc,r12,pc
```
