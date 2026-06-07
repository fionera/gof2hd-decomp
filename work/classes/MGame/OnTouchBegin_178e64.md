# MGame::OnTouchBegin
elf 0x178e64 body 2836
Sig: undefined __thiscall OnTouchBegin(MGame * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::OnTouchBegin(int, int, void*) */

void __thiscall MGame::OnTouchBegin(MGame *this,int param_1,int param_2,void *param_3)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  byte bVar4;
  uint uVar5;
  int iVar6;
  SingleLevel *pSVar7;
  int iVar8;
  Radar *pRVar9;
  void *pvVar10;
  DialogueWindow *this_00;
  String *pSVar11;
  String *pSVar12;
  undefined4 *puVar13;
  PlayerEgo *pPVar14;
  Vector *pVVar15;
  undefined4 uVar16;
  PlayerEgo *pPVar17;
  int *piVar18;
  ChoiceWindow *pCVar19;
  bool bVar20;
  uint in_fpscr;
  float extraout_s0;
  float fVar21;
  longlong lVar22;
  
  iVar8._0_1_ = this->field_C0;
  iVar8._1_1_ = this->field_C1;
  iVar8._2_1_ = this->field_C2;
  iVar8._3_1_ = this->m_nStarMapWindowOpen;
  if (iVar8 == 0) {
    *(void **)&this->field_C0 = param_3;
  }
  if (*(char *)((int)&this->field_5C + 1) == '\0') {
    if (((char)this->field_60 != '\0') && (3999 < this->field_50)) {
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x9e) {
        uVar16 = 2;
        *(undefined1 *)&this->field_54 = 0;
LAB_00188f26:
        (*(code *)(DAT_001ab904 + 0x1ab908))(this->field_8,uVar16);
        return;
      }
      if (this[2].m_pPlayerEgo != (PlayerEgo *)0x0) {
        GameRecord::load();
        Globals::playMusicAndFadeOutCurrent(**(int **)(DAT_0018921c + 0x188f1a));
        *(undefined1 *)&this->field_54 = 0;
        uVar16 = 5;
        goto LAB_00188f26;
      }
      Globals::playMusicAndFadeOutCurrent(**(int **)(DAT_00189220 + 0x188f3e));
      *(undefined1 *)&this->field_54 = 0;
      AbyssEngine::ApplicationManager::SetCurrentApplicationModule((uint)this->field_8);
    }
  }
  else {
    if (this->field_0xc7 != '\0') {
      if (*(char *)**(undefined4 **)(DAT_001891fc + 0x188e90) != '\0') {
        Layout::OnTouchBegin((int)**(undefined4 **)(DAT_001891fc + 0x188e90),param_1);
        return;
      }
      bVar4 = StarMap::OnTouchBegin((int)this->field_90,param_1);
      this->field_0xc7 = bVar4 ^ 1;
      return;
    }
    if (((this->field_0xc5 != '\0') || (*(char *)((int)&this[1].field_4 + 2) != '\0')) ||
       (this->field_C4 != '\0')) {
      ChoiceWindow::OnTouchBegin(this->field_94,param_1);
      return;
    }
    if (*(char *)((int)&this->field_5C + 2) != '\0') {
      DialogueWindow::OnTouchBegin(this->field_8C,param_1);
      return;
    }
    if (*(char *)((int)&this[1].field_0 + 1) != '\0') {
      iVar8 = AbyssEngine::ApplicationManager::GetApplicationData();
      if (*(char *)(iVar8 + 5) != '\0') {
        return;
      }
      if (*(char *)(iVar8 + 0xc) != '\0') {
        return;
      }
      MenuTouchWindow::OnTouchBegin((int)this->m_pDialogueWindow,param_1,(void *)param_2);
      if (*(char *)((int)&this[1].field_94 + 2) == '\0') {
        return;
      }
      iVar8 = MenuTouchWindow::isShowingMessage();
      if (iVar8 != 0) {
        return;
      }
      iVar8 = MenuTouchWindow::isMakingScreenshot();
      if (iVar8 != 0) {
        return;
      }
      goto LAB_00189296;
    }
  }
  uVar5 = Hud::touchBegin((uint)this->m_pLevel,param_1,(void *)param_2);
  this[1].field_30 = uVar5;
  if (((uVar5 & 0x10) == 0) || ((char)this[1].field_14 != '\0')) {
    if ((int)(uVar5 << 0x1a) < 0) {
      lVar22 = Status::getPlayingTime();
      lVar3 = lVar22 - *(longlong *)&this[1].field_40;
      iVar8 = (int)((ulonglong)lVar3 >> 0x20);
      bVar20 = 499 < (uint)lVar3;
      if ((int)(-(uint)bVar20 - iVar8) < 0 == (SBORROW4(0,iVar8) != SBORROW4(-iVar8,(uint)bVar20)))
      {
        PlayerEgo::alignToHorizon();
      }
      this[1].field_40 = (int)lVar22;
      this[1].field_44 = (int)((ulonglong)lVar22 >> 0x20);
      if (*(char *)((int)&this->field_5C + 3) != '\0') {
        return;
      }
      if (*(void **)&this->field_C0 == param_3) {
        return;
      }
      if (this[1].field_30 != 0x20) {
        return;
      }
LAB_0018905c:
      iVar8 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
      iVar6 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
      *(undefined1 *)&this[2].field_28 = 1;
      this[2].field_2C = iVar8;
      this[2].field_30 = iVar6;
      return;
    }
    if ((uVar5 & 1) == 0) {
      if (((int)(uVar5 << 0x17) < 0) &&
         ((((iVar8 = PlayerEgo::isAutoPilot(), iVar8 != 0 ||
            (iVar8 = PlayerEgo::isDockingToAsteroid(), iVar8 != 0)) ||
           (iVar8 = PlayerEgo::isDockingToDockingPoint(), iVar8 != 0)) &&
          (((char)this->m_pPlayerEgo->field_14[0x15] == '\0' &&
           (iVar8 = PlayerEgo::aboutToReachAutoTarget(this->m_pPlayerEgo), iVar8 == 0)))))) {
        this[1].field_A0 = DAT_00189238;
        (*(code *)(DAT_001ac7c4 + 0x1ac7c8))(this[1].field_2C,1);
        return;
      }
      iVar6 = this[1].field_AC;
      iVar8 = iVar6;
      if (0 < iVar6) {
        iVar8 = this[1].field_A4;
      }
      iVar2 = iVar6 + -1;
      if (iVar6 >= 1) {
        iVar2 = iVar8;
      }
      if ((iVar2 < 0 != (iVar6 < 1 && SBORROW4(iVar6,1))) || ((this[1].field_30 & 0x100) == 0)) {
        uVar5 = this->field_14;
        if (uVar5 == 3) {
          iVar8 = PlayerEgo::isMining();
          if ((iVar8 == 0) && (*(char *)((int)&this->field_5C + 3) == '\0')) {
LAB_00189296:
            (*(code *)(DAT_001ac7d4 + 0x1ac7d8))(this,param_1,param_2,param_3);
            return;
          }
          uVar5 = this->field_14;
        }
        if (1 < uVar5) {
          return;
        }
        *(undefined1 *)&this[1].field_B4 = 1;
        this[1].field_B8 = param_1;
        this[1].field_BC = param_2;
        cVar1 = *(char *)((int)&this->field_5C + 3);
        this[1].field_B0 = 0;
        if (cVar1 != '\0') {
          return;
        }
        if (this[1].field_30 != 0) {
          return;
        }
        goto LAB_0018905c;
      }
      if (iVar8 < 1) {
        Hud::setTimeExtender(SUB41(this->m_pLevel,0),true,false,true);
        piVar18 = *(int **)(DAT_0018957c + 0x189504);
        fVar21 = (float)FModSound::setDownPitch(SUB41(*piVar18,0));
        pVVar15 = (Vector *)0x460;
        this[1].field_A4 = this[1].field_AC;
        iVar8 = *piVar18;
      }
      else {
        piVar18 = *(int **)(DAT_00189230 + 0x18913a);
        this[1].field_A4 = -1;
        FModSound::setDownPitch(SUB41(*piVar18,0));
        fVar21 = (float)Hud::setTimeExtender(SUB41(this->m_pLevel,0),true,false,false);
        iVar8 = *piVar18;
        pVVar15 = (Vector *)0x45f;
      }
    }
    else {
      iVar8 = **(int **)(DAT_00189228 + 0x189084);
      pVVar15 = (Vector *)0x7c;
      fVar21 = extraout_s0;
    }
    FModSound::play(iVar8,pVVar15,(Vector *)0x0,fVar21);
    return;
  }
  if (((this->field_80[9] == 0) || (iVar8 = Status::inAlienOrbit(), iVar8 != 0)) ||
     (iVar8 = PlayerEgo::isDockedToDockingPoint(), iVar8 != 0)) {
    if (((this->field_80[5] != 0) && (iVar8 = PlayerEgo::isDockingToPlanet(), iVar8 == 0)) &&
       (iVar8 = PlayerEgo::isDockedToDockingPoint(), iVar8 == 0)) {
      iVar8 = Status::getCurrentCampaignMission();
      if ((iVar8 < 10) || (iVar8 = Status::getCurrentCampaignMission(), iVar8 == 0x30)) {
        pPVar17 = this->m_pPlayerEgo;
        uVar16 = 0x15;
        pSVar7 = this->m_pLevel;
LAB_001891ec:
        (*(code *)(DAT_001ac7b4 + 0x1ac7b8))(pSVar7,uVar16,pPVar17,0);
        return;
      }
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x18) {
        iVar8 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar6 = Mission::getTargetStation();
        if (iVar8 == iVar6) {
          iVar8 = Status::getShip();
          iVar8 = Ship::getFirstEquipmentOfSort(iVar8);
          if (iVar8 == 0) {
LAB_001897fc:
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            if ((DialogueWindow *)this->field_8C != (DialogueWindow *)0x0) {
              pvVar10 = (void *)DialogueWindow::~DialogueWindow((DialogueWindow *)this->field_8C);
              operator_delete(pvVar10);
              this->field_8C = 0;
            }
            this_00 = operator_new(0x74);
            piVar18 = *(int **)(DAT_001899b8 + 0x18982a);
            pSVar11 = (String *)GameText::getText(*piVar18);
            pSVar12 = (String *)GameText::getText(*piVar18);
            DialogueWindow::DialogueWindow(this_00,pSVar11,pSVar12,(int *)(DAT_001899bc + 0x189870))
            ;
            *(undefined1 *)((int)&this->field_5C + 2) = 1;
            this->field_8C = (int)this_00;
            return;
          }
          iVar8 = Status::getShip();
          iVar8 = Ship::getFirstEquipmentOfSort(iVar8);
          if (iVar8 == 0) goto LAB_001897fc;
        }
      }
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x87) {
        iVar8 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar6 = Mission::getTargetStation();
        if (iVar8 == iVar6) {
          iVar8 = Status::getShip();
          iVar8 = Ship::getFirstEquipmentOfSort(iVar8);
          if (iVar8 == 0) {
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            pCVar19 = (ChoiceWindow *)this->field_94;
            if (pCVar19 == (ChoiceWindow *)0x0) {
              pCVar19 = operator_new(0x5c);
              ChoiceWindow::ChoiceWindow(pCVar19);
              this->field_94 = (int)pCVar19;
            }
            puVar13 = (undefined4 *)(DAT_001899c0 + 0x1898b8);
            goto LAB_00189980;
          }
        }
      }
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x5b) {
        iVar8 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar6 = Mission::getTargetStation();
        if (iVar8 != iVar6) goto LAB_0018964e;
        Status::getShip();
        iVar8 = Ship::getMaxPassengers();
        if (9 < iVar8) goto LAB_0018964e;
        *(undefined1 *)((int)&this->field_5C + 1) = 1;
        pauseSounds();
        pCVar19 = (ChoiceWindow *)this->field_94;
        if (pCVar19 == (ChoiceWindow *)0x0) {
          pCVar19 = operator_new(0x5c);
          ChoiceWindow::ChoiceWindow(pCVar19);
LAB_0018991c:
          this->field_94 = (int)pCVar19;
        }
LAB_00189920:
        puVar13 = (undefined4 *)(DAT_001899c4 + 0x18992a);
        goto LAB_00189980;
      }
LAB_0018964e:
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x5e) {
        iVar8 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar6 = Mission::getTargetStation();
        if (iVar8 == iVar6) {
          Status::getShip();
          iVar8 = Ship::getMaxPassengers();
          if (iVar8 == 0) {
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            pCVar19 = (ChoiceWindow *)this->field_94;
            if (pCVar19 == (ChoiceWindow *)0x0) {
              pCVar19 = operator_new(0x5c);
              ChoiceWindow::ChoiceWindow(pCVar19);
              goto LAB_0018991c;
            }
            goto LAB_00189920;
          }
        }
      }
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x69) {
        iVar8 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar6 = Mission::getTargetStation();
        if (iVar8 == iVar6) {
          iVar8 = Status::getShip();
          iVar8 = Ship::hasEquipment(iVar8,0xce);
          if (iVar8 == 0) {
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            pCVar19 = (ChoiceWindow *)this->field_94;
            if (pCVar19 == (ChoiceWindow *)0x0) {
              pCVar19 = operator_new(0x5c);
              ChoiceWindow::ChoiceWindow(pCVar19);
              this->field_94 = (int)pCVar19;
            }
            GameText::getText(**(int **)(DAT_001899c8 + 0x1898e2));
            ChoiceWindow::set(pCVar19);
            *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
            *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
            return;
          }
        }
      }
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x8b) {
        iVar8 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar6 = Mission::getTargetStation();
        if (iVar8 == iVar6) {
          Status::getShip();
          iVar8 = Ship::getIndex();
          if (iVar8 != 0x2a) {
            Status::getShip();
            iVar8 = Ship::getIndex();
            if (*(int *)(DAT_001899cc + 0x1896f8 + iVar8 * 4) == 1) {
              iVar8 = Status::getShip();
              iVar8 = Ship::hasEquipment(iVar8,0xbe);
              if (iVar8 != 0) goto LAB_00189714;
            }
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            pCVar19 = (ChoiceWindow *)this->field_94;
            if (pCVar19 == (ChoiceWindow *)0x0) {
              pCVar19 = operator_new(0x5c);
              ChoiceWindow::ChoiceWindow(pCVar19);
              this->field_94 = (int)pCVar19;
            }
            puVar13 = (undefined4 *)(DAT_001899d0 + 0x189956);
            goto LAB_00189980;
          }
        }
      }
LAB_00189714:
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x8e) {
        iVar8 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar6 = Mission::getTargetStation();
        if (iVar8 == iVar6) {
          iVar8 = Status::getShip();
          iVar8 = Ship::getFirstEquipmentOfSort(iVar8);
          if (iVar8 != 0) {
            iVar8 = Status::getShip();
            iVar8 = Ship::hasEquipment(iVar8,0xc5);
            if (iVar8 != 0) {
              iVar8 = Status::getShip();
              iVar8 = Ship::getFirstEquipmentOfSort(iVar8);
              if (iVar8 != 0) goto LAB_0018976c;
            }
          }
          *(undefined1 *)((int)&this->field_5C + 1) = 1;
          pauseSounds();
          pCVar19 = (ChoiceWindow *)this->field_94;
          if (pCVar19 == (ChoiceWindow *)0x0) {
            pCVar19 = operator_new(0x5c);
            ChoiceWindow::ChoiceWindow(pCVar19);
            this->field_94 = (int)pCVar19;
          }
          puVar13 = (undefined4 *)(DAT_001899d4 + 0x18988c);
          goto LAB_00189980;
        }
      }
LAB_0018976c:
      iVar8 = Status::getCurrentCampaignMission();
      if (iVar8 == 0x8e) {
        iVar8 = Radar::getPlanetDockIndex();
        Status::getCampaignMission();
        iVar6 = Mission::getTargetStation();
        if (iVar8 == iVar6) {
          Status::getShip();
          iVar8 = Ship::getFreeSpace();
          if (iVar8 == 0) {
            *(undefined1 *)((int)&this->field_5C + 1) = 1;
            pauseSounds();
            pCVar19 = (ChoiceWindow *)this->field_94;
            if (pCVar19 == (ChoiceWindow *)0x0) {
              pCVar19 = operator_new(0x5c);
              ChoiceWindow::ChoiceWindow(pCVar19);
              this->field_94 = (int)pCVar19;
            }
            puVar13 = (undefined4 *)(DAT_001899d8 + 0x189982);
LAB_00189980:
            GameText::getText(*(int *)*puVar13);
            ChoiceWindow::set(pCVar19);
            *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
            *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
            return;
          }
        }
      }
      TargetFollowCamera::setLookAtCam(SUB41(this[1].field_2C,0));
      PlayerEgo::dockToPlanet();
      pSVar7 = this->m_pLevel;
      *(undefined1 *)&this[1].field_48 = 0;
      *(undefined1 *)&this->field_5C = 0;
      Hud::enableFireForTutorial(SUB41(pSVar7,0));
      *(undefined1 *)((int)this->field_7C + 0x11) = 1;
      *(undefined1 *)((int)&this->field_5C + 3) = 1;
      return;
    }
    iVar8 = PlayerEgo::isMining();
    if ((iVar8 == 0) && (iVar8 = PlayerEgo::isDockedToDockingPoint(), iVar8 == 0)) {
      iVar8 = Radar::getLockedAsteroid((Radar *)this->field_80);
      if ((iVar8 != 0) && (iVar8 = PlayerEgo::isDockingToAsteroid(), iVar8 == 0)) {
        Status::getShip();
        iVar8 = Ship::getFreeSpace();
        pPVar17 = this->m_pPlayerEgo;
        pSVar7 = this->m_pLevel;
        if (iVar8 < 1) {
          uVar16 = 0x1b;
          goto LAB_001891ec;
        }
        pPVar14 = (PlayerEgo *)0xb;
LAB_001894ba:
        Hud::hudEvent((int)pSVar7,pPVar14,(int)pPVar17);
        pPVar17 = this->m_pPlayerEgo;
        pRVar9 = (Radar *)Radar::getLockedAsteroid((Radar *)this->field_80);
        PlayerEgo::dockToAsteroid((KIPlayer *)pPVar17,pRVar9);
        (*(code *)(DAT_001ac7f4 + 0x1ac7f8))(this->m_pLevel);
        return;
      }
      iVar8 = PlayerEgo::isDockingToAsteroid();
      if (iVar8 != 0) {
        pPVar17 = this->m_pPlayerEgo;
        pPVar14 = (PlayerEgo *)0x6;
        pSVar7 = this->m_pLevel;
        goto LAB_001894ba;
      }
      pPVar17 = this->m_pPlayerEgo;
      pRVar9 = (Radar *)Radar::getLockedAsteroid((Radar *)this->field_80);
      PlayerEgo::dockToAsteroid((KIPlayer *)pPVar17,pRVar9);
    }
  }
  else {
    iVar8 = PlayerEgo::isAutoPilot();
    if ((iVar8 == 0) && (iVar8 = PlayerEgo::isInTurretMode(), iVar8 == 0)) {
      PlayerEgo::setAutoPilot((KIPlayer *)this->m_pPlayerEgo);
      iVar6 = this->field_80[9];
      iVar8 = Level::getLandmarks();
      if (iVar6 == **(int **)(iVar8 + 4)) {
        pPVar14 = this->m_pPlayerEgo;
        pPVar17 = (PlayerEgo *)0xa;
        pSVar7 = this->m_pLevel;
      }
      else {
        iVar6 = this->field_80[9];
        iVar8 = Level::getLandmarks();
        pPVar14 = this->m_pPlayerEgo;
        pSVar7 = this->m_pLevel;
        if (iVar6 == *(int *)(*(int *)(iVar8 + 4) + 0xc)) {
          pPVar17 = (PlayerEgo *)0xf;
        }
        else {
          pPVar17 = (PlayerEgo *)0xc;
        }
      }
      Hud::hudEvent((int)pSVar7,pPVar17,(int)pPVar14);
      iVar8 = Hud::touchEnd((uint)this->m_pLevel,param_1,(void *)param_2);
      this[1].field_30 = iVar8;
      return;
    }
    iVar8 = PlayerEgo::isInTurretMode();
    if (iVar8 == 0) {
      Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0x6,(int)this->m_pPlayerEgo);
      PlayerEgo::setAutoPilot((KIPlayer *)this->m_pPlayerEgo);
      piVar18 = this->field_80;
      piVar18[9] = 0;
      piVar18[10] = 0;
      PlayerEgo::resetGunDelay();
    }
  }
  iVar8 = PlayerEgo::isMining();
  if (iVar8 != 0) {
LAB_0018932e:
    iVar8 = PlayerEgo::isMining();
    if (iVar8 == 0) {
      Hud::enableFireForTutorial(SUB41(this->m_pLevel,0));
      lVar22 = Status::getPlayingTime();
      lVar3 = lVar22 - *(longlong *)&this[1].field_38;
      iVar8 = (int)((ulonglong)lVar3 >> 0x20);
      bVar20 = 0xf9 < (uint)lVar3;
      if ((int)(-(uint)bVar20 - iVar8) < 0 == (SBORROW4(0,iVar8) != SBORROW4(-iVar8,(uint)bVar20)))
      {
        *(undefined1 *)&this->field_5C = 1;
        Hud::enableFireForTutorial(SUB41(this->m_pLevel,0));
      }
      else {
        if ((char)this->field_5C != '\0') {
          iVar8 = Hud::touchEnd((uint)this->m_pLevel,param_1,(void *)param_2);
          this[1].field_30 = iVar8;
        }
        *(undefined1 *)&this->field_5C = 0;
      }
      this[1].field_38 = (int)lVar22;
      this[1].field_3C = (int)((ulonglong)lVar22 >> 0x20);
      *(undefined1 *)&this[1].field_48 = 1;
      return;
    }
    *(undefined4 *)(**(int **)(DAT_0018956c + 0x18933e) + 0x124) = 0;
    (*(code *)(DAT_001ac7e4 + 0x1ac7e8))(this->m_pPlayerEgo);
    return;
  }
  if ((this->field_80[1] == 0) || (*(char *)(this->field_80[1] + 0x70) == '\0')) {
    iVar8 = PlayerEgo::isDockedToDockingPoint();
    if (iVar8 == 0) goto LAB_0018932e;
    if (this->field_80[1] != 0) goto LAB_001892ec;
LAB_00189398:
    iVar8 = PlayerEgo::isDockingToDockingPoint();
    if ((iVar8 == 0) &&
       ((iVar8 = PlayerEgo::isDockedToDockingPoint(), iVar8 == 0 ||
        (iVar8 = PlayerEgo::isInTurretMode(), iVar8 != 0)))) goto LAB_0018942e;
    iVar8 = PlayerEgo::isDockedToDockingPoint();
    if (iVar8 == 0) {
      iVar8 = PlayerEgo::isDockingToDockingPoint();
      if ((iVar8 != 0) && (iVar8 = PlayerEgo::isLandingOrTakingOff(), iVar8 == 0)) {
        Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0x6,(int)this->m_pPlayerEgo);
      }
      goto LAB_0018942e;
    }
    PlayerEgo::dockToDockingPoint((KIPlayer *)this->m_pPlayerEgo,(Radar *)this->field_80[1]);
    PlayerEgo::setAutoPilot((KIPlayer *)this->m_pPlayerEgo);
    PlayerEgo::resetGunDelay();
  }
  else {
LAB_001892ec:
    iVar8 = PlayerEgo::isDockingToDockingPoint();
    if (((iVar8 != 0) || (iVar8 = PlayerEgo::isDockedToDockingPoint(), iVar8 != 0)) ||
       (*(char *)(this->field_80[1] + 0x75) == '\0')) goto LAB_00189398;
    Hud::hudEvent((int)this->m_pLevel,(PlayerEgo *)0x22,(int)this->m_pPlayerEgo);
    PlayerEgo::dockToDockingPoint((KIPlayer *)this->m_pPlayerEgo,(Radar *)this->field_80[1]);
  }
  Hud::releaseAllKeys();
LAB_0018942e:
  Hud::enableFireForTutorial(SUB41(this->m_pLevel,0));
  lVar22 = Status::getPlayingTime();
  lVar3 = lVar22 - *(longlong *)&this[1].field_38;
  iVar8 = (int)((ulonglong)lVar3 >> 0x20);
  bVar20 = 0xf9 < (uint)lVar3;
  if ((int)(-(uint)bVar20 - iVar8) < 0 == (SBORROW4(0,iVar8) != SBORROW4(-iVar8,(uint)bVar20))) {
    *(undefined1 *)&this->field_5C = 1;
    Hud::enableFireForTutorial(SUB41(this->m_pLevel,0));
  }
  else {
    if ((char)this->field_5C != '\0') {
      iVar8 = Hud::touchEnd((uint)this->m_pLevel,param_1,(void *)param_2);
      this[1].field_30 = iVar8;
    }
    *(undefined1 *)&this->field_5C = 0;
  }
  this[1].field_38 = (int)lVar22;
  this[1].field_3C = (int)((ulonglong)lVar22 >> 0x20);
  return;
}

```

## target disasm
```
  00188e64: push {r4,r5,r6,r7,lr}
  00188e66: add r7,sp,#0xc
  00188e68: push {r7,r8,r9,r10,r11}
  00188e6c: mov r10,r0
  00188e6e: ldr.w r0,[r0,#0xc0]
  00188e72: mov r8,r3
  00188e74: mov r11,r2
  00188e76: mov r9,r1
  00188e78: cbnz r0,0x00188e7e
  00188e7a: str.w r8,[r10,#0xc0]
  00188e7e: ldrb.w r0,[r10,#0x5d]
  00188e82: cbz r0,0x00188ea0
  00188e84: ldrb.w r0,[r10,#0xc7]
  00188e88: cbz r0,0x00188ecc
  00188e8a: ldr r0,[0x001891fc]
  00188e8c: add r0,pc
  00188e8e: ldr r0,[r0,#0x0]
  00188e90: ldr r0,[r0,#0x0]
  00188e92: ldrb r1,[r0,#0x0]
  00188e94: cbz r1,0x00188ef0
  00188e96: mov r1,r9
  00188e98: mov r2,r11
  00188e9a: blx 0x00074728
  00188e9e: b 0x00189092
  00188ea0: ldrb.w r0,[r10,#0x60]
  00188ea4: cmp r0,#0x0
  00188ea6: beq 0x00188f54
  00188ea8: ldr.w r0,[r10,#0x50]
  00188eac: cmp.w r0,#0xfa0
  00188eb0: blt 0x00188f54
  00188eb2: ldr r0,[0x00189218]
  00188eb4: add r0,pc
  00188eb6: ldr r0,[r0,#0x0]
  00188eb8: ldr r0,[r0,#0x0]
  00188eba: blx 0x00071770
  00188ebe: cmp r0,#0x9e
  00188ec0: bne 0x00188f06
  00188ec2: movs r0,#0x0
  00188ec4: movs r1,#0x2
  00188ec6: strb.w r0,[r10,#0x54]
  00188eca: b 0x00188f26
  00188ecc: ldrb.w r0,[r10,#0xc5]
  00188ed0: cbnz r0,0x00188ee2
  00188ed2: ldrb.w r0,[r10,#0xce]
  00188ed6: cbnz r0,0x00188ee2
  00188ed8: ldrb.w r0,[r10,#0xc4]
  00188edc: cmp r0,#0x0
  00188ede: beq.w 0x00189162
  00188ee2: ldr.w r0,[r10,#0x94]
  00188ee6: mov r1,r9
  00188ee8: mov r2,r11
  00188eea: blx 0x0007471c
  00188eee: b 0x00189092
  00188ef0: ldr.w r0,[r10,#0x90]
  00188ef4: mov r1,r9
  00188ef6: mov r2,r11
  00188ef8: blx 0x00074704
  00188efc: eor r0,r0,#0x1
  00188f00: strb.w r0,[r10,#0xc7]
  00188f04: b 0x00189092
  00188f06: ldr.w r0,[r10,#0x1e8]
  00188f0a: cbz r0,0x00188f36
  00188f0c: blx 0x000765d0
  00188f10: ldr r0,[0x0018921c]
  00188f12: movs r1,#0x0
  00188f14: movs r4,#0x0
  00188f16: add r0,pc
  00188f18: ldr r0,[r0,#0x0]
  00188f1a: ldr r0,[r0,#0x0]
  00188f1c: blx 0x00074f80
  00188f20: strb.w r4,[r10,#0x54]
  00188f24: movs r1,#0x5
  00188f26: ldr.w r0,[r10,#0x8]
  00188f2a: pop.w {r3,r8,r9,r10,r11}
  00188f2e: pop.w {r4,r5,r6,r7,lr}
  00188f32: b.w 0x001ab8f8
  00188f36: ldr r0,[0x00189220]
  00188f38: movs r1,#0x2
  00188f3a: add r0,pc
  00188f3c: ldr r0,[r0,#0x0]
  00188f3e: ldr r0,[r0,#0x0]
  00188f40: blx 0x00074f80
  00188f44: ldr.w r0,[r10,#0x8]
  00188f48: movs r1,#0x0
  00188f4a: strb.w r1,[r10,#0x54]
  00188f4e: movs r1,#0x1
  00188f50: blx 0x00071d64
  00188f54: ldr.w r0,[r10,#0x74]
  00188f58: mov r1,r9
  00188f5a: mov r2,r11
  00188f5c: mov r3,r8
  00188f5e: blx 0x00077bd8
  00188f62: tst r0,#0x10
  00188f66: str.w r0,[r10,#0xf8]
  00188f6a: beq 0x00188f76
  00188f6c: ldrb.w r1,[r10,#0xdc]
  00188f70: cmp r1,#0x0
  00188f72: beq.w 0x00189098
  00188f76: lsls r1,r0,#0x1a
  00188f78: bmi 0x0018900e
  00188f7a: lsls r1,r0,#0x1f
  00188f7c: bne 0x0018907c
  00188f7e: lsls r0,r0,#0x17
  00188f80: bpl 0x00188fb6
  00188f82: ldr.w r0,[r10,#0x58]
  00188f86: blx 0x0007681c
  00188f8a: cbnz r0,0x00188fa0
  00188f8c: ldr.w r0,[r10,#0x58]
  00188f90: blx 0x00076804
  00188f94: cbnz r0,0x00188fa0
  00188f96: ldr.w r0,[r10,#0x58]
  00188f9a: blx 0x00076810
  00188f9e: cbz r0,0x00188fb6
  00188fa0: ldr.w r0,[r10,#0x58]
  00188fa4: ldr r1,[r0,#0x14]
  00188fa6: ldrb.w r1,[r1,#0x54]
  00188faa: cbnz r1,0x00188fb6
  00188fac: blx 0x00077b60
  00188fb0: cmp r0,#0x0
  00188fb2: beq.w 0x00189200
  00188fb6: ldr.w r0,[r10,#0x174]
  00188fba: cmp r0,#0x1
  00188fbc: itt ge
  00188fbe: ldr.ge.w r0,[r10,#0x16c]
  00188fc2: cmp.ge r0,#0x0
  00188fc4: bge.w 0x00189120
  00188fc8: ldr.w r0,[r10,#0x14]
  00188fcc: cmp r0,#0x3
  00188fce: bne 0x00188fe8
  00188fd0: ldr.w r0,[r10,#0x58]
  00188fd4: blx 0x00077adc
  00188fd8: cbnz r0,0x00188fe4
  00188fda: ldrb.w r0,[r10,#0x5f]
  00188fde: cmp r0,#0x0
  00188fe0: beq.w 0x00189296
  00188fe4: ldr.w r0,[r10,#0x14]
  00188fe8: cmp r0,#0x1
  00188fea: bhi 0x00189092
  00188fec: movs r0,#0x1
  00188fee: movs r1,#0x0
  00188ff0: strb.w r0,[r10,#0x17c]
  00188ff4: strd r9,r11,[r10,#0x180]
  00188ff8: ldrb.w r0,[r10,#0x5f]
  00188ffc: str.w r1,[r10,#0x178]
  00189000: cmp r0,#0x0
  00189002: bne 0x00189092
  00189004: ldr.w r0,[r10,#0xf8]
  00189008: cmp r0,#0x0
  0018900a: bne 0x00189092
  0018900c: b 0x0018905c
  0018900e: ldr r0,[0x00189224]
  00189010: add r0,pc
  00189012: ldr r0,[r0,#0x0]
  00189014: ldr r0,[r0,#0x0]
  00189016: blx 0x00071710
  0018901a: mov r5,r0
  0018901c: mov r6,r1
  0018901e: ldrd r0,r1,[r10,#0x108]
  00189022: movw r3,#0x1f3
  00189026: movs r2,#0x0
  00189028: add.w r4,r10,#0x108
  0018902c: subs r0,r5,r0
  0018902e: sbc.w r1,r6,r1
  00189032: subs r0,r3,r0
  00189034: sbcs.w r0,r2,r1
  00189038: itt ge
  0018903a: ldr.ge.w r0,[r10,#0x58]
  0018903e: blx.ge 0x000781c0
  00189042: strd r5,r6,[r4,#0x0]
  00189046: ldrb.w r0,[r10,#0x5f]
  0018904a: cbnz r0,0x00189092
  0018904c: ldr.w r0,[r10,#0xc0]
  00189050: cmp r0,r8
  00189052: beq 0x00189092
  00189054: ldr.w r0,[r10,#0xf8]
  00189058: cmp r0,#0x20
  0018905a: bne 0x00189092
  0018905c: vmov s0,r9
  00189060: movs r0,#0x1
  00189062: vmov s2,r11
  00189066: vcvt.f32.s32 s0,s0
  0018906a: vcvt.f32.s32 s2,s2
  0018906e: strb.w r0,[r10,#0x1b8]
  00189072: vstr.32 s0,[r10,#0x1bc]
  00189076: vstr.32 s2,[r10,#0x1c0]
  0018907a: b 0x00189092
  0018907c: ldr r0,[0x00189228]
  0018907e: movs r1,#0x0
  00189080: add r0,pc
  00189082: ldr r0,[r0,#0x0]
  00189084: ldr r0,[r0,#0x0]
  00189086: str r1,[sp,#0x0]
  00189088: movs r1,#0x7c
  0018908a: movs r2,#0x0
  0018908c: movs r3,#0x0
  0018908e: blx 0x00071548
  00189092: pop.w {r3,r8,r9,r10,r11}
  00189096: pop {r4,r5,r6,r7,pc}
  00189098: ldr.w r0,[r10,#0x80]
  0018909c: ldr r0,[r0,#0x24]
  0018909e: cbz r0,0x001890bc
  001890a0: ldr r0,[0x0018922c]
  001890a2: add r0,pc
  001890a4: ldr r0,[r0,#0x0]
  001890a6: ldr r0,[r0,#0x0]
  001890a8: blx 0x000723d0
  001890ac: cbnz r0,0x001890bc
  001890ae: mov r6,r10
  001890b0: ldr.w r0,[r6,#0x58]!
  001890b4: blx 0x00075928
  001890b8: cmp r0,#0x0
  001890ba: beq 0x00189178
  001890bc: ldr.w r0,[r10,#0x80]
  001890c0: add.w r6,r10,#0x58
  001890c4: ldr r0,[r0,#0x14]
  001890c6: cbz r0,0x001890da
  001890c8: ldr r0,[r6,#0x0]
  001890ca: blx 0x000767bc
  001890ce: cbnz r0,0x001890da
  001890d0: ldr r0,[r6,#0x0]
  001890d2: blx 0x00075928
  001890d6: cmp r0,#0x0
  001890d8: beq 0x001891c6
  001890da: ldr r0,[r6,#0x0]
  001890dc: blx 0x00077adc
  001890e0: cmp r0,#0x0
  001890e2: bne.w 0x001892c4
  001890e6: ldr r0,[r6,#0x0]
  001890e8: blx 0x00075928
  001890ec: cmp r0,#0x0
  001890ee: bne.w 0x001892c4
  001890f2: ldr.w r0,[r10,#0x80]
  001890f6: blx 0x000781cc
  001890fa: cbz r0,0x00189108
  001890fc: ldr r0,[r6,#0x0]
  001890fe: blx 0x00076804
  00189102: cmp r0,#0x0
  00189104: beq.w 0x00189498
  00189108: ldr r0,[r6,#0x0]
  0018910a: blx 0x00076804
  0018910e: cmp r0,#0x0
  00189110: beq.w 0x001892ac
  00189114: ldr.w r2,[r10,#0x58]
  00189118: movs r1,#0x6
  0018911a: ldr.w r0,[r10,#0x74]
  0018911e: b 0x001894ba
  00189120: ldrb.w r1,[r10,#0xf9]
  00189124: lsls r1,r1,#0x1f
  00189126: beq.w 0x00188fc8
  0018912a: cmp r0,#0x1
  0018912c: blt.w 0x001894e8
  00189130: ldr r0,[0x00189230]
  00189132: movs r1,#0x0
  00189134: movs r5,#0x0
  00189136: add r0,pc
  00189138: ldr r4,[r0,#0x0]
  0018913a: mov.w r0,#0xffffffff
  0018913e: str.w r0,[r10,#0x16c]
  00189142: ldr r0,[r4,#0x0]
  00189144: blx 0x00074f74
  00189148: ldr.w r0,[r10,#0x74]
  0018914c: movs r1,#0x1
  0018914e: movs r2,#0x0
  00189150: movs r3,#0x0
  00189152: str r5,[sp,#0x0]
  00189154: blx 0x000780d0
  00189158: ldr r0,[r4,#0x0]
  0018915a: movw r1,#0x45f
  0018915e: str r5,[sp,#0x0]
  00189160: b 0x0018908a
  00189162: ldrb.w r0,[r10,#0x5e]
  00189166: cmp r0,#0x0
  00189168: beq 0x0018923c
  0018916a: ldr.w r0,[r10,#0x8c]
  0018916e: mov r1,r9
  00189170: mov r2,r11
  00189172: blx 0x0007507c
  00189176: b 0x00189092
  00189178: ldr r0,[r6,#0x0]
  0018917a: blx 0x0007681c
  0018917e: cbnz r0,0x0018918c
  00189180: ldr r0,[r6,#0x0]
  00189182: blx 0x0007678c
  00189186: cmp r0,#0x0
  00189188: beq.w 0x0018951c
  0018918c: ldr r0,[r6,#0x0]
  0018918e: blx 0x0007678c
  00189192: cmp r0,#0x0
  00189194: bne.w 0x001892c4
  00189198: ldr.w r2,[r10,#0x58]
  0018919c: movs r1,#0x6
  0018919e: ldr.w r0,[r10,#0x74]
  001891a2: movs r3,#0x0
  001891a4: movs r4,#0x0
  001891a6: blx 0x00072418
  001891aa: ldr.w r0,[r10,#0x58]
  001891ae: movs r1,#0x0
  001891b0: blx 0x000728d4
  001891b4: ldr.w r0,[r10,#0x80]
  001891b8: strd r4,r4,[r0,#0x24]
  001891bc: ldr.w r0,[r10,#0x58]
  001891c0: blx 0x00072ca0
  001891c4: b 0x001892c4
  001891c6: ldr r0,[0x00189234]
  001891c8: add r0,pc
  001891ca: ldr r6,[r0,#0x0]
  001891cc: ldr r0,[r6,#0x0]
  001891ce: blx 0x00071770
  001891d2: cmp r0,#0xa
  001891d4: blt 0x001891e2
  001891d6: ldr r0,[r6,#0x0]
  001891d8: blx 0x00071770
  001891dc: cmp r0,#0x30
  001891de: bne.w 0x00189580
  001891e2: ldr.w r2,[r10,#0x58]
  001891e6: movs r1,#0x15
  001891e8: ldr.w r0,[r10,#0x74]
  001891ec: movs r3,#0x0
  001891ee: add sp,#0x4
  001891f0: pop.w {r8,r9,r10,r11}
  001891f4: pop.w {r4,r5,r6,r7,lr}
  001891f8: b.w 0x001ac7a8
  00189200: ldr r0,[0x00189238]
  00189202: movs r1,#0x1
  00189204: str.w r0,[r10,#0x168]
  00189208: ldr.w r0,[r10,#0xf4]
  0018920c: pop.w {r3,r8,r9,r10,r11}
  00189210: pop.w {r4,r5,r6,r7,lr}
  00189214: b.w 0x001ac7b8
  0018923c: ldrb.w r0,[r10,#0xc9]
  00189240: cmp r0,#0x0
  00189242: beq.w 0x00188f54
  00189246: ldr r0,[0x00189568]
  00189248: add r0,pc
  0018924a: ldr r0,[r0,#0x0]
  0018924c: ldr r0,[r0,#0x0]
  0018924e: blx 0x00071704
  00189252: ldrb r1,[r0,#0x5]
  00189254: cmp r1,#0x0
  00189256: bne.w 0x00189092
  0018925a: ldrb r0,[r0,#0xc]
  0018925c: cmp r0,#0x0
  0018925e: bne.w 0x00189092
  00189262: ldr.w r0,[r10,#0x88]
  00189266: mov r1,r9
  00189268: mov r2,r11
  0018926a: mov r3,r8
  0018926c: blx 0x000750b8
  00189270: ldrb.w r0,[r10,#0x15e]
  00189274: cmp r0,#0x0
  00189276: beq.w 0x00189092
  0018927a: ldr.w r0,[r10,#0x88]
  0018927e: blx 0x000781e4
  00189282: cmp r0,#0x0
  00189284: bne.w 0x00189092
  00189288: ldr.w r0,[r10,#0x88]
  0018928c: blx 0x000781f0
  00189290: cmp r0,#0x0
  00189292: bne.w 0x00189092
  00189296: mov r0,r10
  00189298: mov r1,r9
  0018929a: mov r2,r11
  0018929c: mov r3,r8
  0018929e: add sp,#0x4
  001892a0: pop.w {r8,r9,r10,r11}
  001892a4: pop.w {r4,r5,r6,r7,lr}
  001892a8: b.w 0x001ac7c8
  001892ac: ldr.w r0,[r10,#0x80]
  001892b0: ldr.w r5,[r10,#0x58]
  001892b4: blx 0x000781cc
  001892b8: ldr.w r2,[r10,#0x80]
  001892bc: mov r1,r0
  001892be: mov r0,r5
  001892c0: blx 0x00072820
  001892c4: ldr r0,[r6,#0x0]
  001892c6: blx 0x00077adc
  001892ca: cbnz r0,0x0018932e
  001892cc: ldr.w r0,[r10,#0x80]
  001892d0: ldr r0,[r0,#0x4]
  001892d2: cbz r0,0x001892da
  001892d4: ldrb.w r0,[r0,#0x70]
  001892d8: cbnz r0,0x001892ec
  001892da: ldr r0,[r6,#0x0]
  001892dc: blx 0x00075928
  001892e0: cbz r0,0x0018932e
  001892e2: ldr.w r0,[r10,#0x80]
  001892e6: ldr r0,[r0,#0x4]
  001892e8: cmp r0,#0x0
  001892ea: beq 0x00189398
  001892ec: ldr r0,[r6,#0x0]
  001892ee: blx 0x00076810
  001892f2: cmp r0,#0x0
  001892f4: bne 0x00189398
  001892f6: ldr r0,[r6,#0x0]
  001892f8: blx 0x00075928
  001892fc: cmp r0,#0x0
  001892fe: bne 0x00189398
  00189300: ldr.w r0,[r10,#0x80]
  00189304: ldr r0,[r0,#0x4]
  00189306: ldrb.w r0,[r0,#0x75]
  0018930a: cmp r0,#0x0
  0018930c: beq 0x00189398
  0018930e: ldr.w r2,[r10,#0x58]
  00189312: movs r1,#0x22
  00189314: ldr.w r0,[r10,#0x74]
  00189318: movs r3,#0x0
  0018931a: blx 0x00072418
  0018931e: ldr.w r2,[r10,#0x80]
  00189322: ldr.w r0,[r10,#0x58]
  00189326: ldr r1,[r2,#0x4]
  00189328: blx 0x00072838
  0018932c: b 0x001893c8
  0018932e: ldr r0,[r6,#0x0]
  00189330: blx 0x00077adc
  00189334: cbz r0,0x00189352
  00189336: ldr r0,[0x0018956c]
  00189338: movs r1,#0x0
  0018933a: add r0,pc
  0018933c: ldr r0,[r0,#0x0]
  0018933e: ldr r0,[r0,#0x0]
  00189340: str.w r1,[r0,#0x124]
  00189344: ldr r0,[r6,#0x0]
  00189346: pop.w {r3,r8,r9,r10,r11}
  0018934a: pop.w {r4,r5,r6,r7,lr}
  0018934e: b.w 0x001ac7d8
  00189352: ldr.w r0,[r10,#0x74]
  00189356: movs r1,#0x0
  00189358: blx 0x00078208
  0018935c: ldr r0,[0x00189570]
  0018935e: add r0,pc
  00189360: ldr r0,[r0,#0x0]
  00189362: ldr r0,[r0,#0x0]
  00189364: blx 0x00071710
  00189368: mov r5,r0
  0018936a: mov r4,r1
  0018936c: ldrd r0,r1,[r10,#0x100]
  00189370: add.w r6,r10,#0x100
  00189374: subs r0,r5,r0
  00189376: sbc.w r1,r4,r1
  0018937a: rsbs.w r0,r0,#0xf9
  0018937e: mov.w r0,#0x0
  00189382: sbcs r0,r1
  00189384: blt 0x001893d2
  00189386: ldr.w r0,[r10,#0x74]
  0018938a: movs r1,#0x1
  0018938c: strb.w r1,[r10,#0x5c]
  00189390: movs r1,#0x1
  00189392: blx 0x00078208
  00189396: b 0x001893f0
  00189398: ldr r0,[r6,#0x0]
  0018939a: blx 0x00076810
  0018939e: cbz r0,0x001893fc
  001893a0: ldr r0,[r6,#0x0]
  001893a2: blx 0x00075928
  001893a6: mov r1,r0
  001893a8: ldr r0,[r6,#0x0]
  001893aa: cbz r1,0x00189410
  001893ac: ldr.w r2,[r10,#0x80]
  001893b0: ldr r1,[r2,#0x4]
  001893b2: blx 0x00072838
  001893b6: ldr.w r0,[r10,#0x58]
  001893ba: movs r1,#0x0
  001893bc: blx 0x000728d4
  001893c0: ldr.w r0,[r10,#0x58]
  001893c4: blx 0x00072ca0
  001893c8: ldr.w r0,[r10,#0x74]
  001893cc: blx 0x00077a1c
  001893d0: b 0x0018942e
  001893d2: ldrb.w r0,[r10,#0x5c]
  001893d6: cbz r0,0x001893ea
  001893d8: ldr.w r0,[r10,#0x74]
  001893dc: mov r1,r9
  001893de: mov r2,r11
  001893e0: mov r3,r8
  001893e2: blx 0x00078214
  001893e6: str.w r0,[r10,#0xf8]
  001893ea: movs r0,#0x0
  001893ec: strb.w r0,[r10,#0x5c]
  001893f0: movs r0,#0x1
  001893f2: strd r5,r4,[r6,#0x0]
  001893f6: strb.w r0,[r10,#0x110]
  001893fa: b 0x00189092
  001893fc: ldr r0,[r6,#0x0]
  001893fe: blx 0x00075928
  00189402: cbz r0,0x0018942e
  00189404: ldr r0,[r6,#0x0]
  00189406: blx 0x0007678c
  0018940a: cmp r0,#0x0
  0018940c: beq 0x001893a0
  0018940e: b 0x0018942e
  00189410: blx 0x00076810
  00189414: cbz r0,0x0018942e
  00189416: ldr r0,[r6,#0x0]
  00189418: blx 0x00077b48
  0018941c: cbnz r0,0x0018942e
  0018941e: ldr.w r2,[r10,#0x58]
  00189422: movs r1,#0x6
  00189424: ldr.w r0,[r10,#0x74]
  00189428: movs r3,#0x0
  0018942a: blx 0x00072418
  0018942e: ldr.w r0,[r10,#0x74]
  00189432: movs r1,#0x0
  00189434: blx 0x00078208
  00189438: ldr r0,[0x00189574]
  0018943a: add r0,pc
  0018943c: ldr r0,[r0,#0x0]
  0018943e: ldr r0,[r0,#0x0]
  00189440: blx 0x00071710
  00189444: mov r5,r0
  00189446: mov r4,r1
  00189448: ldrd r0,r1,[r10,#0x100]
  0018944c: add.w r6,r10,#0x100
  00189450: subs r0,r5,r0
  00189452: sbc.w r1,r4,r1
  00189456: rsbs.w r0,r0,#0xf9
  0018945a: mov.w r0,#0x0
  0018945e: sbcs r0,r1
  00189460: blt 0x00189474
  00189462: ldr.w r0,[r10,#0x74]
  00189466: movs r1,#0x1
  00189468: strb.w r1,[r10,#0x5c]
  0018946c: movs r1,#0x1
  0018946e: blx 0x00078208
  00189472: b 0x00189492
  00189474: ldrb.w r0,[r10,#0x5c]
  00189478: cbz r0,0x0018948c
  0018947a: ldr.w r0,[r10,#0x74]
  0018947e: mov r1,r9
  00189480: mov r2,r11
  00189482: mov r3,r8
  00189484: blx 0x00078214
  00189488: str.w r0,[r10,#0xf8]
  0018948c: movs r0,#0x0
  0018948e: strb.w r0,[r10,#0x5c]
  00189492: strd r5,r4,[r6,#0x0]
  00189496: b 0x00189092
  00189498: ldr r0,[0x00189578]
  0018949a: add r0,pc
  0018949c: ldr r0,[r0,#0x0]
  0018949e: ldr r0,[r0,#0x0]
  001894a0: blx 0x00071a58
  001894a4: blx 0x000722ec
  001894a8: mov r1,r0
  001894aa: ldr.w r2,[r10,#0x58]
  001894ae: ldr.w r0,[r10,#0x74]
  001894b2: cmp r1,#0x0
  001894b4: ble.w 0x001897d2
  001894b8: movs r1,#0xb
  001894ba: movs r3,#0x0
  001894bc: blx 0x00072418
  001894c0: ldr.w r0,[r10,#0x80]
  001894c4: ldr.w r5,[r10,#0x58]
  001894c8: blx 0x000781cc
  001894cc: ldr.w r2,[r10,#0x80]
  001894d0: mov r1,r0
  001894d2: mov r0,r5
  001894d4: blx 0x00072820
  001894d8: ldr.w r0,[r10,#0x74]
  001894dc: pop.w {r3,r8,r9,r10,r11}
  001894e0: pop.w {r4,r5,r6,r7,lr}
  001894e4: b.w 0x001ac7e8
  001894e8: ldr.w r0,[r10,#0x74]
  001894ec: movs r1,#0x1
  001894ee: str r1,[sp,#0x0]
  001894f0: movs r1,#0x1
  001894f2: movs r2,#0x0
  001894f4: movs r3,#0x1
  001894f6: movs r4,#0x0
  001894f8: blx 0x000780d0
  001894fc: ldr r0,[0x0018957c]
  001894fe: movs r1,#0x1
  00189500: add r0,pc
  00189502: ldr r5,[r0,#0x0]
  00189504: ldr r0,[r5,#0x0]
  00189506: blx 0x00074f74
  0018950a: ldr.w r0,[r10,#0x174]
  0018950e: mov.w r1,#0x460
  00189512: str.w r0,[r10,#0x16c]
  00189516: ldr r0,[r5,#0x0]
  00189518: str r4,[sp,#0x0]
  0018951a: b 0x0018908a
  0018951c: ldr.w r1,[r10,#0x80]
  00189520: ldr.w r0,[r10,#0x58]
  00189524: ldr r1,[r1,#0x24]
  00189526: blx 0x000728d4
  0018952a: ldr.w r0,[r10,#0x78]
  0018952e: ldr.w r1,[r10,#0x80]
  00189532: ldr r4,[r1,#0x24]
  00189534: blx 0x000725b0
  00189538: ldr r0,[r0,#0x4]
  0018953a: ldr r0,[r0,#0x0]
  0018953c: cmp r4,r0
  0018953e: beq.w 0x001897d6
  00189542: ldr.w r0,[r10,#0x78]
  00189546: ldr.w r1,[r10,#0x80]
  0018954a: ldr r4,[r1,#0x24]
  0018954c: blx 0x000725b0
  00189550: ldr r1,[r0,#0x4]
  00189552: ldr.w r2,[r10,#0x58]
  00189556: ldr.w r0,[r10,#0x74]
  0018955a: ldr r1,[r1,#0xc]
  0018955c: cmp r4,r1
  0018955e: ite ne
  00189560: mov.ne r1,#0xc
  00189562: mov.eq r1,#0xf
  00189564: b 0x001897e0
  00189580: ldr r0,[r6,#0x0]
  00189582: blx 0x00071770
  00189586: cmp r0,#0x18
  00189588: bne 0x001895c6
  0018958a: ldr.w r0,[r10,#0x80]
  0018958e: blx 0x00078220
  00189592: mov r5,r0
  00189594: ldr r0,[r6,#0x0]
  00189596: blx 0x0007285c
  0018959a: blx 0x00073738
  0018959e: cmp r5,r0
  001895a0: bne 0x001895c6
  001895a2: ldr r0,[r6,#0x0]
  001895a4: blx 0x00071a58
  001895a8: movs r1,#0xd
  001895aa: blx 0x0007228c
  001895ae: cmp r0,#0x0
  001895b0: beq.w 0x001897fc
  001895b4: ldr r0,[r6,#0x0]
  001895b6: blx 0x00071a58
  001895ba: movs r1,#0x11
  001895bc: blx 0x0007228c
  001895c0: cmp r0,#0x0
  001895c2: beq.w 0x001897fc
  001895c6: ldr r0,[r6,#0x0]
  001895c8: blx 0x00071770
  001895cc: cmp r0,#0x87
  001895ce: bne 0x001895fa
  001895d0: ldr.w r0,[r10,#0x80]
  001895d4: blx 0x00078220
  001895d8: mov r5,r0
  001895da: ldr r0,[r6,#0x0]
  001895dc: blx 0x0007285c
  001895e0: blx 0x00073738
  001895e4: cmp r5,r0
  001895e6: bne 0x001895fa
  001895e8: ldr r0,[r6,#0x0]
  001895ea: blx 0x00071a58
  001895ee: movs r1,#0x13
  001895f0: blx 0x0007228c
  001895f4: cmp r0,#0x0
  001895f6: beq.w 0x0018988c
  001895fa: ldr r0,[r6,#0x0]
  001895fc: blx 0x00071770
  00189600: cmp r0,#0x5b
  00189602: bne 0x0018964e
  00189604: ldr.w r0,[r10,#0x80]
  00189608: blx 0x00078220
  0018960c: mov r5,r0
  0018960e: ldr r0,[r6,#0x0]
  00189610: blx 0x0007285c
  00189614: blx 0x00073738
  00189618: cmp r5,r0
  0018961a: bne 0x0018964e
  0018961c: ldr r0,[r6,#0x0]
  0018961e: blx 0x00071a58
  00189622: blx 0x00072bd4
  00189626: cmp r0,#0x9
  00189628: bgt 0x0018964e
  0018962a: movs r6,#0x1
  0018962c: mov r0,r10
  0018962e: strb.w r6,[r10,#0x5d]
  00189632: blx 0x0007822c
  00189636: ldr.w r5,[r10,#0x94]
  0018963a: cmp r5,#0x0
  0018963c: bne.w 0x00189920
  00189640: movs r0,#0x5c
  00189642: blx 0x0006eb24
  00189646: mov r5,r0
  00189648: blx 0x00074584
  0018964c: b 0x0018991c
  0018964e: ldr r0,[r6,#0x0]
  00189650: blx 0x00071770
  00189654: cmp r0,#0x5e
  00189656: bne 0x00189680
  00189658: ldr.w r0,[r10,#0x80]
  0018965c: blx 0x00078220
  00189660: mov r5,r0
  00189662: ldr r0,[r6,#0x0]
  00189664: blx 0x0007285c
  00189668: blx 0x00073738
  0018966c: cmp r5,r0
  0018966e: bne 0x00189680
  00189670: ldr r0,[r6,#0x0]
  00189672: blx 0x00071a58
  00189676: blx 0x00072bd4
  0018967a: cmp r0,#0x0
  0018967c: beq.w 0x001898fe
  00189680: ldr r0,[r6,#0x0]
  00189682: blx 0x00071770
  00189686: cmp r0,#0x69
  00189688: bne 0x001896b8
  0018968a: ldr.w r0,[r10,#0x80]
  0018968e: blx 0x00078220
  00189692: mov r5,r0
  00189694: ldr r0,[r6,#0x0]
  00189696: blx 0x0007285c
  0018969a: blx 0x00073738
  0018969e: cmp r5,r0
  001896a0: bne 0x001896b8
  001896a2: ldr r0,[r6,#0x0]
  001896a4: blx 0x00071a58
  001896a8: movs r1,#0xce
  001896aa: movs r2,#0x1
  001896ac: movs r4,#0x1
  001896ae: blx 0x00073978
  001896b2: cmp r0,#0x0
  001896b4: beq.w 0x001898b8
  001896b8: ldr r0,[r6,#0x0]
  001896ba: blx 0x00071770
  001896be: cmp r0,#0x8b
  001896c0: bne 0x00189714
  001896c2: ldr.w r0,[r10,#0x80]
  001896c6: blx 0x00078220
  001896ca: mov r5,r0
  001896cc: ldr r0,[r6,#0x0]
  001896ce: blx 0x0007285c
  001896d2: blx 0x00073738
  001896d6: cmp r5,r0
  001896d8: bne 0x00189714
  001896da: ldr r0,[r6,#0x0]
  001896dc: blx 0x00071a58
  001896e0: blx 0x000719c8
  001896e4: cmp r0,#0x2a
  001896e6: beq 0x00189714
  001896e8: ldr r0,[r6,#0x0]
  001896ea: blx 0x00071a58
  001896ee: blx 0x000719c8
  001896f2: ldr r1,[0x001899cc]
  001896f4: add r1,pc
  001896f6: ldr.w r0,[r1,r0,lsl #0x2]
  001896fa: cmp r0,#0x1
  001896fc: bne.w 0x0018992a
  00189700: ldr r0,[r6,#0x0]
  00189702: blx 0x00071a58
  00189706: movs r1,#0xbe
  00189708: movs r2,#0x1
  0018970a: blx 0x00073978
  0018970e: cmp r0,#0x0
  00189710: beq.w 0x0018992a
  00189714: ldr r0,[r6,#0x0]
  00189716: blx 0x00071770
  0018971a: cmp r0,#0x8e
  0018971c: bne 0x0018976c
  0018971e: ldr.w r0,[r10,#0x80]
  00189722: blx 0x00078220
  00189726: mov r5,r0
  00189728: ldr r0,[r6,#0x0]
  0018972a: blx 0x0007285c
  0018972e: blx 0x00073738
  00189732: cmp r5,r0
  00189734: bne 0x0018976c
  00189736: ldr r0,[r6,#0x0]
  00189738: blx 0x00071a58
  0018973c: movs r1,#0x21
  0018973e: blx 0x0007228c
  00189742: cmp r0,#0x0
  00189744: beq.w 0x00189860
  00189748: ldr r0,[r6,#0x0]
  0018974a: blx 0x00071a58
  0018974e: movs r1,#0xc5
  00189750: movs r2,#0xf
  00189752: blx 0x00073978
  00189756: cmp r0,#0x0
  00189758: beq.w 0x00189860
  0018975c: ldr r0,[r6,#0x0]
  0018975e: blx 0x00071a58
  00189762: movs r1,#0x23
  00189764: blx 0x0007228c
  00189768: cmp r0,#0x0
  0018976a: beq 0x00189860
  0018976c: ldr r0,[r6,#0x0]
  0018976e: blx 0x00071770
  00189772: cmp r0,#0x8e
  00189774: bne 0x0018979e
  00189776: ldr.w r0,[r10,#0x80]
  0018977a: blx 0x00078220
  0018977e: mov r5,r0
  00189780: ldr r0,[r6,#0x0]
  00189782: blx 0x0007285c
  00189786: blx 0x00073738
  0018978a: cmp r5,r0
  0018978c: bne 0x0018979e
  0018978e: ldr r0,[r6,#0x0]
  00189790: blx 0x00071a58
  00189794: blx 0x000722ec
  00189798: cmp r0,#0x0
  0018979a: beq.w 0x00189956
  0018979e: ldr.w r0,[r10,#0xf4]
  001897a2: movs r1,#0x1
  001897a4: movs r4,#0x1
  001897a6: blx 0x00072670
  001897aa: ldr.w r0,[r10,#0x58]
  001897ae: blx 0x000768a0
  001897b2: ldr.w r0,[r10,#0x74]
  001897b6: movs r1,#0x0
  001897b8: strb.w r1,[r10,#0x110]
  001897bc: strb.w r1,[r10,#0x5c]
  001897c0: movs r1,#0x0
  001897c2: blx 0x00078208
  001897c6: ldr.w r0,[r10,#0x7c]
  001897ca: strb r4,[r0,#0x11]
  001897cc: strb.w r4,[r10,#0x5f]
  001897d0: b 0x00189092
  001897d2: movs r1,#0x1b
  001897d4: b 0x001891ec
  001897d6: ldr.w r2,[r10,#0x58]
  001897da: movs r1,#0xa
  001897dc: ldr.w r0,[r10,#0x74]
  001897e0: movs r3,#0x0
  001897e2: blx 0x00072418
  001897e6: ldr.w r0,[r10,#0x74]
  001897ea: mov r1,r9
  001897ec: mov r2,r11
  001897ee: mov r3,r8
  001897f0: blx 0x00078214
  001897f4: str.w r0,[r10,#0xf8]
  001897f8: b.w 0x00189092
  001897fc: movs r0,#0x1
  001897fe: strb.w r0,[r10,#0x5d]
  00189802: mov r0,r10
  00189804: blx 0x0007822c
  00189808: ldr.w r0,[r10,#0x8c]
  0018980c: cbz r0,0x0018981c
  0018980e: blx 0x00075010
  00189812: blx 0x0006eb48
  00189816: movs r0,#0x0
  00189818: str.w r0,[r10,#0x8c]
  0018981c: movs r0,#0x74
  0018981e: blx 0x0006eb24
  00189822: mov r5,r0
  00189824: ldr r0,[0x001899b8]
  00189826: add r0,pc
  00189828: ldr r4,[r0,#0x0]
  0018982a: ldr r0,[r4,#0x0]
  0018982c: mov.w r1,#0x214
  00189830: blx 0x00072f70
  00189834: mov r6,r0
  00189836: ldr r0,[r4,#0x0]
  00189838: movw r1,#0x643
  0018983c: blx 0x00072f70
  00189840: mov r2,r0
  00189842: ldr r0,[0x001899bc]
  00189844: add r0,pc
  00189846: add.w r3,r0,#0x28
  0018984a: mov r0,r5
  0018984c: mov r1,r6
  0018984e: blx 0x00074f20
  00189852: movs r0,#0x1
  00189854: strb.w r0,[r10,#0x5e]
  00189858: str.w r5,[r10,#0x8c]
  0018985c: b.w 0x00189092
  00189860: movs r6,#0x1
  00189862: mov r0,r10
  00189864: strb.w r6,[r10,#0x5d]
  00189868: blx 0x0007822c
  0018986c: ldr.w r5,[r10,#0x94]
  00189870: cbnz r5,0x00189882
  00189872: movs r0,#0x5c
  00189874: blx 0x0006eb24
  00189878: mov r5,r0
  0018987a: blx 0x00074584
  0018987e: str.w r5,[r10,#0x94]
  00189882: ldr r0,[0x001899d4]
  00189884: mov.w r1,#0xc90
  00189888: add r0,pc
  0018988a: b 0x00189980
  0018988c: movs r6,#0x1
  0018988e: mov r0,r10
  00189890: strb.w r6,[r10,#0x5d]
  00189894: blx 0x0007822c
  00189898: ldr.w r5,[r10,#0x94]
  0018989c: cbnz r5,0x001898ae
  0018989e: movs r0,#0x5c
  001898a0: blx 0x0006eb24
  001898a4: mov r5,r0
  001898a6: blx 0x00074584
  001898aa: str.w r5,[r10,#0x94]
  001898ae: ldr r0,[0x001899c0]
  001898b0: movw r1,#0xc8d
  001898b4: add r0,pc
  001898b6: b 0x00189980
  001898b8: mov r0,r10
  001898ba: strb.w r4,[r10,#0x5d]
  001898be: blx 0x0007822c
  001898c2: ldr.w r5,[r10,#0x94]
  001898c6: cbnz r5,0x001898d8
  001898c8: movs r0,#0x5c
  001898ca: blx 0x0006eb24
  001898ce: mov r5,r0
  001898d0: blx 0x00074584
  001898d4: str.w r5,[r10,#0x94]
  001898d8: ldr r0,[0x001899c8]
  001898da: movw r1,#0xc91
  001898de: add r0,pc
  001898e0: ldr r0,[r0,#0x0]
  001898e2: ldr r0,[r0,#0x0]
  001898e4: blx 0x00072f70
  001898e8: mov r1,r0
  001898ea: mov r0,r5
  001898ec: blx 0x00074794
  001898f0: movs r0,#0x1
  001898f2: strb.w r0,[r10,#0x111]
  001898f6: strb.w r0,[r10,#0xce]
  001898fa: b.w 0x00189092
  001898fe: movs r6,#0x1
  00189900: mov r0,r10
  00189902: strb.w r6,[r10,#0x5d]
  00189906: blx 0x0007822c
  0018990a: ldr.w r5,[r10,#0x94]
  0018990e: cbnz r5,0x00189920
  00189910: movs r0,#0x5c
  00189912: blx 0x0006eb24
  00189916: mov r5,r0
  00189918: blx 0x00074584
  0018991c: str.w r5,[r10,#0x94]
  00189920: ldr r0,[0x001899c4]
  00189922: movw r1,#0xc8e
  00189926: add r0,pc
  00189928: b 0x00189980
  0018992a: movs r6,#0x1
  0018992c: mov r0,r10
  0018992e: strb.w r6,[r10,#0x5d]
  00189932: blx 0x0007822c
  00189936: ldr.w r5,[r10,#0x94]
  0018993a: cbnz r5,0x0018994c
  0018993c: movs r0,#0x5c
  0018993e: blx 0x0006eb24
  00189942: mov r5,r0
  00189944: blx 0x00074584
  00189948: str.w r5,[r10,#0x94]
  0018994c: ldr r0,[0x001899d0]
  0018994e: movw r1,#0xc8f
  00189952: add r0,pc
  00189954: b 0x00189980
  00189956: movs r6,#0x1
  00189958: mov r0,r10
  0018995a: strb.w r6,[r10,#0x5d]
  0018995e: blx 0x0007822c
  00189962: ldr.w r5,[r10,#0x94]
  00189966: cbnz r5,0x00189978
  00189968: movs r0,#0x5c
  0018996a: blx 0x0006eb24
  0018996e: mov r5,r0
  00189970: blx 0x00074584
  00189974: str.w r5,[r10,#0x94]
  00189978: ldr r0,[0x001899d8]
  0018997a: movw r1,#0xc92
  0018997e: add r0,pc
  00189980: ldr r0,[r0,#0x0]
  00189982: ldr r0,[r0,#0x0]
  00189984: blx 0x00072f70
  00189988: mov r1,r0
  0018998a: mov r0,r5
  0018998c: blx 0x00074794
  00189990: strb.w r6,[r10,#0x111]
  00189994: strb.w r6,[r10,#0xce]
  00189998: b.w 0x00189092
  001ac7b8: bx pc
  001ac7bc: ldr r12,[0x1ac7c4]
  001ac7c0: add pc,r12,pc
  001ac7c8: bx pc
  001ac7cc: ldr r12,[0x1ac7d4]
  001ac7d0: add pc,r12,pc
  001ac7d8: bx pc
  001ac7dc: ldr r12,[0x1ac7e4]
  001ac7e0: add pc,r12,pc
```
