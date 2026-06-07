# ModStation::OnTouchEnd
elf 0xd7600 body 4896
Sig: undefined __thiscall OnTouchEnd(ModStation * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* ModStation::OnTouchEnd(int, int, void*) */

void __thiscall ModStation::OnTouchEnd(ModStation *this,int param_1,int param_2,void *param_3)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  DialogueWindow *this_00;
  Radio *this_01;
  Array<RadioMessage*> *this_02;
  RadioMessage *pRVar5;
  ScrollTouchBox *this_03;
  String *pSVar6;
  uint uVar7;
  int *piVar8;
  Ship *pSVar9;
  Station *pSVar10;
  char *pcVar11;
  undefined4 *puVar12;
  void *pvVar13;
  Item *pIVar14;
  RecordHandler *this_04;
  HangarWindow *pHVar15;
  MenuTouchWindow *this_05;
  undefined4 *puVar16;
  undefined4 uVar17;
  uint uVar18;
  uint *puVar19;
  Agent *this_06;
  code *pcVar20;
  int *piVar21;
  int *piVar22;
  int *piVar23;
  uint in_fpscr;
  float fVar24;
  float extraout_s0;
  int iVar25;
  int iVar26;
  float fVar27;
  String aSStack_a0 [12];
  String aSStack_94 [12];
  undefined1 auStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  undefined1 auStack_64 [12];
  String aSStack_58 [12];
  undefined1 auStack_4c [12];
  String aSStack_40 [12];
  float local_34;
  float local_30;
  undefined4 local_2c;
  int local_28;
  
  piVar23 = *(int **)(DAT_000e7908 + 0xe7616);
  local_28 = *piVar23;
  if (this[2].field_8 == param_3) {
    iVar3 = this->m_nStarMapWindowOpen;
    *(undefined1 *)&this[1].field_88 = 0;
    this[2].field_8 = (int *)0x0;
    if ((char)iVar3 == '\0') {
      piVar21 = *(int **)(DAT_000e790c + 0xe7656);
      if (*(Layout *)*piVar21 == (Layout)0x0) {
        if (*(char *)((int)&this->field_68 + 1) != '\0') {
          iVar3 = DialogueWindow::OnTouchEnd(this->field_84,param_1);
          if (iVar3 != 0) {
            if (this[1].field_C != 0) {
              iVar3 = Mission::getType();
              iVar4 = Mission::isCampaignMission((Mission *)this[1].field_C);
              if (iVar3 == 8) {
                if (iVar4 == 0) {
                  iVar3 = Status::getShip();
                  iVar4 = Mission::getProductionGoodIndex((Mission *)this[1].field_C);
                  Mission::getProductionGoodAmount();
                  Ship::removeCargo(iVar3,iVar4);
                  goto LAB_000e76d8;
                }
              }
              else if ((iVar4 == 0) && (iVar3 = Mission::getType(), iVar3 == 0xb)) {
                piVar22 = *(int **)(DAT_000e7924 + 0xe77a2);
                Status::setPassengers((Status *)*piVar22,0);
                iVar3 = Mission::isCampaignMission((Mission *)this[1].field_C);
                if (iVar3 == 0) {
                  iVar3 = Mission::getProductionGoodAmount();
                  iVar4 = *piVar22;
                  *(int *)(iVar4 + 0xb8) = iVar3 + *(int *)(iVar4 + 0xb8);
                }
                Status::getShip();
                puVar19 = (uint *)Ship::getCargo();
                if (puVar19 != (uint *)0x0) {
                  for (uVar7 = 0; uVar7 < *puVar19; uVar7 = uVar7 + 1) {
                    iVar3 = Item::isUnsaleable(*(Item **)(puVar19[1] + uVar7 * 4));
                    if ((iVar3 != 0) &&
                       ((iVar3 = Item::getIndex(*(Item **)(puVar19[1] + uVar7 * 4)), iVar3 == 0x74
                        || (iVar3 = Item::getIndex(*(Item **)(puVar19[1] + uVar7 * 4)),
                           iVar3 == 0x75)))) goto LAB_000e789c;
                  }
                }
              }
              else {
                iVar3 = Mission::getType();
                if ((iVar3 != 3) &&
                   ((iVar3 = Mission::getType(), iVar3 != 5 &&
                    (iVar3 = Mission::getType(), iVar3 != 0xb)))) {
                  iVar3 = Mission::getType();
                  if (iVar3 == 0) {
                    piVar22 = *(int **)(DAT_000e792c + 0xe78b8);
                    Status::getShip();
                    puVar19 = (uint *)Ship::getCargo();
                    if (puVar19 != (uint *)0x0) {
                      for (uVar7 = 0; uVar7 < *puVar19; uVar7 = uVar7 + 1) {
                        iVar3 = Item::isUnsaleable(*(Item **)(puVar19[1] + uVar7 * 4));
                        if ((iVar3 != 0) &&
                           ((iVar3 = Item::getIndex(*(Item **)(puVar19[1] + uVar7 * 4)),
                            iVar3 == 0x74 ||
                            (iVar3 = Item::getIndex(*(Item **)(puVar19[1] + uVar7 * 4)),
                            iVar3 == 0x75)))) {
                          pIVar14 = (Item *)Status::getShip();
                          Ship::removeCargo(pIVar14);
                          if (this->field_78 != 0) {
                            HangarWindow::initialize();
                          }
                          break;
                        }
                      }
                    }
                    iVar3 = Mission::getProductionGoodAmount();
                    *(int *)(*piVar22 + 0x9c) = iVar3 + *(int *)(*piVar22 + 0x9c);
                  }
                  else {
                    iVar3 = Mission::getType();
                    if (iVar3 == 0xe) {
                      Status::getShip();
                      puVar19 = (uint *)Ship::getCargo();
                      if (puVar19 != (uint *)0x0) {
                        for (uVar7 = 0; uVar7 < *puVar19; uVar7 = uVar7 + 1) {
                          iVar3 = Item::getIndex(*(Item **)(puVar19[1] + uVar7 * 4));
                          if (iVar3 == 0x73) goto LAB_000e789c;
                        }
                      }
                    }
                  }
                }
              }
              goto LAB_000e7962;
            }
            this_00 = (DialogueWindow *)this->field_84;
            *(undefined1 *)((int)&this->field_68 + 1) = 0;
            if (this_00 != (DialogueWindow *)0x0) {
              pvVar13 = (void *)DialogueWindow::~DialogueWindow(this_00);
              operator_delete(pvVar13);
            }
            cVar1 = *(char *)((int)&this[1].field_70 + 1);
            this->field_84 = 0;
            if (cVar1 == '\0') {
              puVar12 = *(undefined4 **)(DAT_000e7914 + 0xe770c);
              pSVar10 = (Station *)Status::getStation();
              iVar3 = Station::getIndex(pSVar10);
              if (iVar3 != 4) {
                pSVar10 = (Station *)Status::getStation();
                iVar3 = Station::getIndex(pSVar10);
                if (iVar3 != 0x58) {
                  Status::getStation();
                  iVar3 = Station::stationHasPirateBase();
                  if (iVar3 != 0) {
                    *(undefined1 *)((int)&this[1].field_20 + 2) = 0;
                    pSVar10 = (Station *)*puVar12;
                    Status::getStation();
                    Status::departStation(pSVar10);
                    Achievements::resetNewMedals
                              ((Achievements *)**(undefined4 **)(DAT_000e7918 + 0xe7758));
                    iVar3 = Status::getCurrentCampaignMission();
                    uVar17 = 1;
                    piVar21 = (int *)**(undefined4 **)(DAT_000e791c + 0xe7770);
                    if (iVar3 == 0x10) {
                      uVar17 = 0xffffffff;
                    }
                    **(undefined4 **)(DAT_000e7920 + 0xe7772) = uVar17;
LAB_000e7780:
                    AbyssEngine::ApplicationManager::SetCurrentApplicationModule((uint)piVar21);
                    *(undefined1 *)&this->field_24 = 0;
                  }
                }
              }
            }
            goto LAB_000e763a;
          }
LAB_000e7c2a:
          if (*(char *)((int)&this->field_64 + 1) != '\0') goto LAB_000e763a;
        }
        if (*(char *)((int)&this->m_nStarMapWindowOpen + 3) == '\0') {
          if (*(char *)((int)&this->field_68 + 2) == '\0') {
            if (*(char *)((int)&this->field_64 + 2) == '\0') {
              if (*(char *)((int)&this->field_64 + 3) == '\0') {
                if (*(char *)((int)&this->field_64 + 1) == '\0') {
                  if ((char)this->field_68 == '\0') {
                    if ((char)this->field_64 == '\0') {
                      if (*(char *)((int)&this->m_nStarMapWindowOpen + 2) == '\0') {
                        if (*(char *)((int)&this->m_nStarMapWindowOpen + 1) != '\0') {
                          iVar3 = TouchButton::OnTouchEnd(this[1].field_0,param_1);
                          if (iVar3 != 0) {
                            if (*piVar23 - local_28 == 0) {
                              (*(code *)(DAT_001ac154 + 0x1ac158))();
                              return;
                            }
                    /* WARNING: Subroutine does not return */
                            __stack_chk_fail(*piVar23 - local_28);
                          }
                          iVar3 = TouchButton::OnTouchEnd((int)this[1].field_4,param_1);
                          if (iVar3 != 0) {
                            this_04 = (RecordHandler *)**(undefined4 **)(DAT_000e8b04 + 0xe8880);
                            *(undefined1 *)(*(int *)(DAT_000e8b08 + 0xe8882) + 0x4e) = 1;
                            RecordHandler::saveOptions(this_04);
                            if (this->field_78 == 0) {
                              pHVar15 = operator_new(0x134);
                              HangarWindow::HangarWindow(pHVar15);
                              this->field_78 = (int)pHVar15;
                            }
                            HangarWindow::initialize();
                            pHVar15 = (HangarWindow *)this->field_78;
                            *(undefined1 *)&this->field_18 = 1;
                            *(undefined1 *)((int)&this->field_64 + 2) = 1;
                            HangarWindow::showCreditsBuyWindow(pHVar15);
                          }
                          this->field_44 = -1;
                          for (uVar7 = 0; uVar7 < 5; uVar7 = uVar7 + 1) {
                            iVar3 = TouchButton::OnTouchEnd
                                              (*(int *)(this->field_8C[1] + uVar7 * 4),param_1);
                            if (iVar3 != 0) {
                              this->field_44 = uVar7;
                              pcVar20 = *(code **)(this->field_0 + 0x10);
                              goto LAB_000e81ec;
                            }
                          }
                          iVar3 = Layout::OnTouchEnd((Layout *)*piVar21,param_1,param_2);
                          if (iVar3 != 0) {
                            if (this->field_50 == 0) {
                              this_05 = operator_new(0x240);
                              MenuTouchWindow::MenuTouchWindow(this_05,2);
                              this->field_50 = (int)this_05;
                            }
                            Status::checkForLevelUp();
                            *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 2) = 1;
                            iVar4 = *(int *)(DAT_000e8b10 + 0xe891e);
                            iVar3 = *(int *)(DAT_000e8b14 + 0xe8926);
                            for (uVar7 = 0; uVar18 = **(uint **)(this->field_50 + 4), uVar7 < uVar18
                                ; uVar7 = uVar7 + 1) {
                              if (uVar7 < 10) {
                                TouchButton::getPosition();
                                *(int *)(iVar4 + uVar7 * 4) = (int)local_34;
                                TouchButton::getPosition();
                                *(int *)(iVar3 + uVar7 * 4) = (int)local_30;
                              }
                            }
                            goto LAB_000e8304;
                          }
                          iVar3 = Layout::helpPressed((Layout *)*piVar21);
                          if (iVar3 != 0) {
                            iVar3 = *piVar21;
                            pSVar6 = (String *)GameText::getText(**(int **)(DAT_000e8b18 + 0xe89be))
                            ;
                            AbyssEngine::String::String(aSStack_a0,pSVar6,false);
                            Layout::initHelpWindow(iVar3,aSStack_a0);
                            AbyssEngine::String::~String(aSStack_a0);
                          }
                          iVar3 = NewsTicker::OnTouchEnd((int)this->field_1C,param_1);
                          if (iVar3 == 0) {
                            iVar3 = this[1].m_nStarMapWindowOpen;
                            iVar4 = this[1].field_54 + iVar3;
                            iVar26 = VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
                            if (iVar3 < 0) {
                              iVar3 = -iVar3;
                            }
                            iVar25 = DAT_000e8ab8;
                            if (3 < iVar3) {
                              iVar25 = iVar26;
                            }
                            this[1].field_64 = DAT_000e8abc;
                            *(undefined1 *)&this[1].field_70 = 0;
                            this[1].field_54 = iVar4;
                            this[1].field_5C = iVar4;
                            this[1].field_68 = iVar25;
                          }
                        }
                      }
                      else {
                        iVar3 = MenuTouchWindow::OnTouchEnd
                                          ((MenuTouchWindow *)this->field_50,param_1,param_2,param_3
                                          );
                        if (iVar3 != 0) {
                          *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 2) = 0;
                          iVar4 = *(int *)(DAT_000e8938 + 0xe8636);
                          iVar3 = *(int *)(DAT_000e893c + 0xe863e);
                          for (uVar7 = 0; uVar18 = *this->field_8C, uVar7 < uVar18;
                              uVar7 = uVar7 + 1) {
                            if (uVar7 < 10) {
                              TouchButton::getPosition();
                              *(int *)(iVar4 + uVar7 * 4) = (int)local_34;
                              TouchButton::getPosition();
                              *(int *)(iVar3 + uVar7 * 4) = (int)local_30;
                            }
                          }
LAB_000e8304:
                          **(uint **)(DAT_000e8534 + 0xe830a) = uVar18;
                        }
                      }
                    }
                    else {
                      iVar4 = MissionsWindow::OnTouchEnd((int)this->m_pDialogueWindow,param_1);
                      iVar3 = DAT_000e8558;
                      if (iVar4 != 0) {
                        *(undefined1 *)&this->field_64 = 0;
                        FModSound::setParamValue
                                  ((FModSound *)**(undefined4 **)(iVar3 + 0xe848c),0,
                                   *(int *)**(undefined4 **)(iVar3 + 0xe848c),extraout_s0);
                        if (*piVar23 - local_28 == 0) {
                          (*(code *)(DAT_001ac144 + 0x1ac148))();
                          return;
                        }
                    /* WARNING: Subroutine does not return */
                        __stack_chk_fail(*piVar23 - local_28);
                      }
                    }
                  }
                  else {
                    iVar4 = StatusWindow::OnTouchEnd((StatusWindow *)this->field_7C,param_1,param_2)
                    ;
                    iVar3 = DAT_000e8548;
                    if (iVar4 != 0) {
                      *(undefined1 *)&this->field_68 = 0;
                      puVar12 = (undefined4 *)(iVar3 + 0xe83de);
                      goto LAB_000e7fe2;
                    }
                  }
                }
                else {
                  iVar3 = SpaceLounge::OnTouchEnd((SpaceLounge *)this->field_74,param_1,param_2);
                  if (iVar3 != 0) {
                    *(undefined1 *)((int)&this->field_64 + 1) = 0;
                    resetIdleCamForHangar();
                    fVar24 = (float)resetLight();
                    piVar21 = *(int **)(DAT_000e8514 + 0xe816e);
                    FModSound::setParamValue((FModSound *)*piVar21,0,*(int *)*piVar21,fVar24);
                    fVar24 = (float)FModSound::stop(*piVar21);
                    FModSound::play(*piVar21,(Vector *)0x7a,(Vector *)0x0,fVar24);
                    CutScene::checkForTurret((CutScene *)this->field_14);
                    if ((this->field_74 != 0) &&
                       (iVar3 = SpaceLounge::hangarNeedsUpdate(), iVar3 != 0)) {
                      if ((HangarWindow *)this->field_78 != (HangarWindow *)0x0) {
                        pvVar13 = (void *)HangarWindow::~HangarWindow
                                                    ((HangarWindow *)this->field_78);
                        operator_delete(pvVar13);
                      }
                      this->field_78 = 0;
                    }
                  }
                }
              }
              else {
                iVar3 = StarMap::OnTouchEnd(this->field_10,param_1);
                if (iVar3 != 0) {
                  *(undefined1 *)((int)&this->field_64 + 3) = 0;
                  resetLight();
                  puVar12 = (undefined4 *)(DAT_000e8120 + 0xe7fe4);
LAB_000e7fe2:
                  iVar3 = *piVar23 - local_28;
                  if (iVar3 == 0) {
                    (*(code *)(DAT_001ab814 + 0x1ab818))();
                    return;
                  }
                    /* WARNING: Subroutine does not return */
                  __stack_chk_fail(*(undefined4 **)*puVar12,iVar3,**(undefined4 **)*puVar12,*piVar23
                                  );
                }
              }
            }
            else {
              iVar3 = HangarWindow::OnTouchEnd((HangarWindow *)this->field_78,param_1,param_2);
              if (iVar3 != 0) {
                piVar21 = *(int **)(DAT_000e8100 + 0xe7df2);
                Status::getShip();
                puVar19 = (uint *)Ship::getCargo();
                iVar3 = 0;
                if (puVar19 != (uint *)0x0) {
                  for (uVar7 = 0; uVar7 < *puVar19; uVar7 = uVar7 + 1) {
                    iVar4 = Item::getIndex(*(Item **)(puVar19[1] + uVar7 * 4));
                    if ((0x83 < iVar4) &&
                       (iVar4 = Item::getIndex(*(Item **)(puVar19[1] + uVar7 * 4)), iVar4 < 0x9a)) {
                      iVar4 = Item::getAmount();
                      iVar3 = iVar3 + iVar4;
                    }
                  }
                }
                if (this[1].field_40 < iVar3) {
                  *(int *)(*piVar21 + 0xa8) = (iVar3 - this[1].field_40) + *(int *)(*piVar21 + 0xa8)
                  ;
                }
                HangarWindow::setSellMode(SUB41(this->field_78,0));
                resetIdleCamForHangar();
                pcVar11 = (char *)this->field_78;
                *(undefined1 *)((int)&this->field_64 + 2) = 0;
                if (*pcVar11 != '\0') {
                  *pcVar11 = '\0';
                  piVar21 = this->field_14;
                  Status::getShip();
                  iVar3 = Ship::getIndex();
                  pSVar9 = (Ship *)Status::getShip();
                  Ship::getRace(pSVar9);
                  CutScene::replacePlayerShip((int)piVar21,iVar3);
                }
                if ((CutScene *)this->field_14 != (CutScene *)0x0) {
                  CutScene::checkForTurret((CutScene *)this->field_14);
                }
                piVar21 = *(int **)(DAT_000e8528 + 0xe8282);
                fVar24 = (float)FModSound::stop(*piVar21);
                fVar24 = (float)FModSound::play(*piVar21,(Vector *)0x7a,(Vector *)0x0,fVar24);
                FModSound::setParamValue((FModSound *)*piVar21,0,*(int *)*piVar21,fVar24);
                iVar4 = *(int *)(DAT_000e852c + 0xe82ae);
                iVar3 = *(int *)(DAT_000e8530 + 0xe82b6);
                for (uVar7 = 0; uVar18 = *this->field_8C, uVar7 < uVar18; uVar7 = uVar7 + 1) {
                  if (uVar7 < 10) {
                    TouchButton::getPosition();
                    *(int *)(iVar4 + uVar7 * 4) = (int)local_34;
                    TouchButton::getPosition();
                    *(int *)(iVar3 + uVar7 * 4) = (int)local_30;
                  }
                }
                goto LAB_000e8304;
              }
            }
          }
          else {
            iVar3 = ChoiceWindow::OnTouchEnd((int)this->field_88,param_1);
            if (iVar3 == 0) {
              if (*piVar23 - local_28 == 0) {
                (*(code *)(DAT_001ac124 + 0x1ac128))();
                return;
              }
                    /* WARNING: Subroutine does not return */
              __stack_chk_fail(*piVar23 - local_28);
            }
          }
        }
        else {
          iVar3 = ChoiceWindow::OnTouchEnd(this->field_70,param_1);
          if (iVar3 == 1) {
            cVar1 = *(char *)((int)&this->field_68 + 3);
            *(undefined1 *)((int)&this[1].field_20 + 2) = 0;
            iVar3 = DAT_000e80f0;
            if (cVar1 == '\0') {
              *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 0;
              *(undefined1 *)&this[1].field_4C = 0;
            }
            else {
              *(undefined1 *)((int)&this[1].field_20 + 2) = 0;
              pSVar10 = (Station *)**(undefined4 **)(iVar3 + 0xe7d96);
              Status::getStation();
              Status::departStation(pSVar10);
              Achievements::resetNewMedals
                        ((Achievements *)**(undefined4 **)(DAT_000e80f4 + 0xe7dac));
              uVar7 = **(uint **)(DAT_000e80f8 + 0xe7dbc);
              **(undefined4 **)(DAT_000e80fc + 0xe7dbe) = 1;
              AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uVar7);
              *(undefined1 *)&this->field_24 = 0;
            }
          }
          else if (iVar3 == 0) {
            if (*(char *)((int)&this[1].field_20 + 2) != '\0') {
              piVar21 = *(int **)(DAT_000e7d70 + 0xe7c62);
              *(undefined1 *)((int)&this[1].field_20 + 2) = 0;
              iVar3 = Status::getCurrentCampaignMission();
              if (iVar3 == 0x18) {
                pSVar10 = (Station *)Status::getStation();
                iVar3 = Station::getIndex(pSVar10);
                if (iVar3 == 10) {
                  uVar7 = 0;
                  while( true ) {
                    pSVar10 = (Station *)Status::getStation();
                    puVar19 = (uint *)Station::getAgents(pSVar10);
                    if (*puVar19 <= uVar7) break;
                    pSVar10 = (Station *)Status::getStation();
                    iVar3 = Station::getAgents(pSVar10);
                    this_06 = *(Agent **)(*(int *)(iVar3 + 4) + uVar7 * 4);
                    iVar3 = Agent::getOffer(this_06);
                    if ((iVar3 == 2) && (iVar3 = Agent::getSellItemIndex(this_06), iVar3 == 0x44)) {
                      Agent::setEvent(this_06,1);
                      Agent::setOfferAccepted(this_06,true);
                    }
                    uVar7 = uVar7 + 1;
                  }
                }
              }
              iVar3 = Status::getCurrentCampaignMission();
              pSVar10 = (Station *)*piVar21;
              if (iVar3 == 0x30) {
                Galaxy::getStation(**(int **)(DAT_000e7d74 + 0xe7cee));
                Status::departStation(pSVar10);
                **(undefined1 **)(DAT_000e7d78 + 0xe7d04) = 1;
              }
              else {
                Status::getStation();
                Status::departStation(pSVar10);
              }
              iVar3 = *piVar21;
              puVar12 = *(undefined4 **)(DAT_000e8114 + 0xe7f88);
              *(undefined4 *)(iVar3 + 0x5c) = 0xffffffff;
              *(undefined4 *)(iVar3 + 0x60) = 0xffffffff;
              *(undefined4 *)(iVar3 + 100) = 0xffffffff;
              *(undefined4 *)(iVar3 + 0x68) = 0xffffffff;
              Achievements::resetNewMedals((Achievements *)*puVar12);
              uVar7 = **(uint **)(DAT_000e8118 + 0xe7fa0);
              **(undefined4 **)(DAT_000e811c + 0xe7fa2) = 1;
              goto LAB_000e7fa8;
            }
            if (*(char *)((int)&this->field_68 + 3) == '\0') {
LAB_000e803a:
              if ((char)this[1].field_4C != '\0') {
                *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 0;
                *(undefined1 *)&this[1].field_4C = 0;
                iVar3 = AbyssEngine::ApplicationManager::GetApplicationData();
                *(undefined1 *)(iVar3 + 0xd) = 1;
                if (*piVar23 == local_28) {
                  (*(code *)(DAT_001ac134 + 0x1ac138))();
                  return;
                }
                    /* WARNING: Subroutine does not return */
                __stack_chk_fail();
              }
              if (*(char *)&this->field_6C == '\0') {
                if (*(char *)((int)&this->field_6C + 1) != '\0') {
                  piVar21 = *(int **)(DAT_000e8538 + 0xe8320);
                  iVar3 = Status::getCredits();
                  if (iVar3 < 25000) {
                    pSVar6 = (String *)GameText::getText(**(int **)(DAT_000e853c + 0xe8338));
                    AbyssEngine::String::String((String *)&local_34,pSVar6,false);
                    iVar3 = *piVar21;
                    AbyssEngine::String::String(aSStack_7c,(String *)&local_34,false);
                    Status::getCredits();
                    Layout::formatCredits((int)auStack_88);
                    AbyssEngine::String::String(aSStack_94,(char *)(DAT_000e8540 + 0xe8376),false);
                    Status::replaceHash(auStack_4c,iVar3,aSStack_7c,auStack_88);
                    AbyssEngine::String::operator=((String *)&local_34,auStack_4c);
                    pcVar20 = *(code **)(DAT_000e8544 + 0xe8398);
                    (*pcVar20)(auStack_4c);
                    (*pcVar20)(aSStack_94);
                    (*pcVar20)(auStack_88);
                    (*pcVar20)(aSStack_7c);
                    ChoiceWindow::set((ChoiceWindow *)this->field_70,(String *)&local_34,false);
                    *(undefined1 *)((int)&this->field_6C + 1) = 0;
                    goto LAB_000e7f56;
                  }
                  Status::changeCredits(*piVar21);
                  iVar3 = DAT_000e8560;
                  *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 0;
                  *(undefined1 *)((int)&this->field_6C + 1) = 0;
                  pSVar10 = (Station *)*piVar21;
                  Galaxy::getStation(**(int **)(iVar3 + 0xe84cc));
                  Status::setStation(pSVar10);
                  uVar7 = **(uint **)(DAT_000e8564 + 0xe84e8);
                  **(undefined4 **)(DAT_000e8568 + 0xe84ea) = 0;
                  AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uVar7);
                }
                if ((*(char *)&this->field_18 != '\0') &&
                   (*(char *)((int)&this->field_64 + 2) != '\0')) {
                  *(undefined1 *)&this->field_18 = 0;
                  *(undefined1 *)((int)&this->field_64 + 2) = 0;
                }
                *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 0;
              }
              else {
                piVar21 = *(int **)(DAT_000e8128 + 0xe8088);
                Status::changeCredits(*piVar21);
                iVar3 = Status::getShip();
                Ship::removeCargo(iVar3,0x6d);
                puVar12 = *(undefined4 **)(DAT_000e8130 + 0xe80a6);
                *(undefined4 *)(*piVar21 + 0x114) = 3;
                RecordHandler::saveOptions((RecordHandler *)*puVar12);
                pSVar6 = (String *)this->field_70;
                GameText::getText(**(int **)(DAT_000e8134 + 0xe80bc));
                ChoiceWindow::set(pSVar6);
                pSVar10 = (Station *)Status::getStation();
                Station::setItems(pSVar10,(Array *)0x0,false);
                Station::setItems(*(Station **)(*piVar21 + 0x14c),(Array *)0x0,false);
                *(undefined1 *)&this->field_6C = 0;
              }
            }
            else {
              piVar21 = *(int **)(DAT_000e8104 + 0xe7eb2);
              iVar3 = Status::getCredits();
              if (this[1].field_44 <= iVar3) {
                Status::changeCredits(*piVar21);
                *(undefined1 *)((int)&this->field_68 + 3) = 0;
                *(undefined1 *)&this[1].field_48 = 1;
                pSVar10 = (Station *)Status::getStation();
                Station::setAttackedFriends(pSVar10,false);
                *(undefined1 *)((int)&this[1].field_70 + 1) = 1;
                enterStation();
                autosave();
                goto LAB_000e803a;
              }
              pSVar6 = (String *)GameText::getText(**(int **)(DAT_000e8108 + 0xe7eca));
              AbyssEngine::String::String((String *)&local_34,pSVar6,false);
              iVar3 = *piVar21;
              AbyssEngine::String::String(aSStack_58,(String *)&local_34,false);
              Status::getCredits();
              Layout::formatCredits((int)auStack_64);
              AbyssEngine::String::String(aSStack_70,(char *)(DAT_000e810c + 0xe7f0c),false);
              Status::replaceHash(auStack_4c,iVar3,aSStack_58,auStack_64);
              AbyssEngine::String::operator=((String *)&local_34,auStack_4c);
              pcVar20 = *(code **)(DAT_000e8110 + 0xe7f2e);
              (*pcVar20)(auStack_4c);
              (*pcVar20)(aSStack_70);
              (*pcVar20)(auStack_64);
              (*pcVar20)(aSStack_58);
              ChoiceWindow::set((ChoiceWindow *)this->field_70,(String *)&local_34,false);
              *(undefined1 *)((int)&this->field_68 + 3) = 1;
              *(undefined1 *)((int)&this[1].field_20 + 2) = 1;
              *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 1;
LAB_000e7f56:
              AbyssEngine::String::~String((String *)&local_34);
            }
          }
        }
      }
      else {
        iVar3 = Layout::OnTouchEnd((Layout *)*piVar21,param_1,param_2);
        if (iVar3 != 0) {
          *(undefined1 *)*piVar21 = 0;
        }
      }
    }
  }
LAB_000e763a:
  if (*piVar23 == local_28) {
    return;
  }
LAB_000e764c:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
LAB_000e789c:
  pIVar14 = (Item *)Status::getShip();
  Ship::removeCargo(pIVar14);
LAB_000e76d8:
  if (this->field_78 != 0) {
    HangarWindow::initialize();
  }
LAB_000e7962:
  iVar3 = Mission::isCampaignMission((Mission *)this[1].field_C);
  piVar22 = *(int **)(DAT_000e7d44 + 0xe7974);
  if (iVar3 == 0) {
    Status::incMissionCount((Status *)*piVar22);
    iVar3 = *piVar21;
    cVar1 = Mission::getReward();
    cVar2 = Mission::getBonus();
    Layout::showMissionRewardMessage(iVar3,(bool)(cVar2 + cVar1));
    goto LAB_000e7b7a;
  }
  iVar3 = Status::getCurrentCampaignMission();
  if (iVar3 == 0x2b) {
    Status::removeMission((Mission *)*piVar22);
    Status::setMission((Mission *)*piVar22);
    iVar3 = DAT_000e7d4c;
    *(undefined1 *)((int)&this->field_68 + 1) = 0;
    piVar21 = *(int **)(iVar3 + 0xe79aa);
    fVar24 = (float)FModSound::stop(*piVar21);
    FModSound::play(*piVar21,(Vector *)0x90,(Vector *)0x0,fVar24);
    this_01 = operator_new(0x48);
    Radio::Radio(this_01);
    this->field_54 = (int)this_01;
    this_02 = operator_new(0xc);
    Array<RadioMessage*>::Array(this_02);
    this->field_58 = (int)this_02;
    ArraySetLength<RadioMessage*>(4,(Array *)this_02);
    pRVar5 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar5,0x817,1,5,4000);
    **(undefined4 **)(this->field_58 + 4) = pRVar5;
    pRVar5 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar5,0x818,1,6,0);
    *(RadioMessage **)(*(int *)(this->field_58 + 4) + 4) = pRVar5;
    pRVar5 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar5,0x819,1,6,1);
    *(RadioMessage **)(*(int *)(this->field_58 + 4) + 8) = pRVar5;
    pRVar5 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar5,0x81a,1,6,2);
    *(RadioMessage **)(*(int *)(this->field_58 + 4) + 0xc) = pRVar5;
    Radio::setMessages((Radio *)this->field_54,(Array *)this->field_58);
    puVar19 = *(uint **)(DAT_000e7d50 + 0xe7a7a);
    AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar19,0x1b5a,(uint *)&this->field_5C);
    piVar21 = *(int **)(DAT_000e7d54 + 0xe7a88);
    iVar4 = *piVar21;
    iVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar19);
    local_30 = (float)VectorSignedToFloat(iVar3 + iVar4 + *piVar21 / 2 + 10,
                                          (byte)(in_fpscr >> 0x16) & 3);
    local_34 = DAT_000e7d58;
    local_2c = 0;
    AbyssEngine::AEMath::Vector::operator=((Vector *)&this[1].field_74,(Vector *)&local_34);
    this_03 = operator_new(0x40);
    fVar24 = (float)this[1].field_74;
    fVar27 = (float)VectorSignedToFloat(**(undefined4 **)(DAT_000e7d5c + 0xe7ad0),
                                        (byte)(in_fpscr >> 0x16) & 3);
    ScrollTouchBox::ScrollTouchBox
              (this_03,(int)fVar24,(int)(float)this[1].field_78,(int)(fVar27 - (fVar24 + fVar24)),
               5000);
    iVar3 = DAT_000e7d60;
    this[1].field_8 = (int *)this_03;
    pSVar6 = (String *)GameText::getText(**(int **)(iVar3 + 0xe7b14));
    AbyssEngine::String::String(aSStack_40,pSVar6,false);
    ScrollTouchBox::setText(this_03,aSStack_40);
    AbyssEngine::String::~String(aSStack_40);
    ScrollTouchBox::setTextCentered((ScrollTouchBox *)this[1].field_8,true);
    this[1].field_24 = DAT_000e7d64;
    this[1].field_28 = 0;
    *(undefined1 *)&this->m_nStarMapWindowOpen = 1;
    goto LAB_000e763a;
  }
  iVar3 = Status::getCurrentCampaignMission();
  if (iVar3 != 0x94) {
    Status::nextCampaignMission(SUB41(*piVar22,0));
  }
  iVar3 = Status::getCurrentCampaignMission();
  uVar7 = iVar3 - 0x2c;
  if (uVar7 < 0xf) {
    if ((1 << (uVar7 & 0xff) & 0x5830U) != 0) {
      piVar8 = this->field_14;
      Status::getShip();
      iVar3 = Ship::getIndex();
      pSVar9 = (Ship *)Status::getShip();
      Ship::getRace(pSVar9);
      CutScene::replacePlayerShip((int)piVar8,iVar3);
      goto LAB_000e7b7a;
    }
    if (uVar7 != 0) {
      if (uVar7 == 1) {
        Status::removeMission((Mission *)*piVar22);
        this[1].field_C = 0;
        Status::setMission((Mission *)*piVar22);
        iVar3 = this->field_0;
        this->field_44 = 1;
        *(undefined1 *)((int)&this->field_68 + 1) = 0;
        pcVar20 = *(code **)(iVar3 + 0x10);
LAB_000e81ec:
        (*pcVar20)(this,pcVar20,0x10000,0);
        goto LAB_000e763a;
      }
      goto LAB_000e81fa;
    }
  }
  else {
LAB_000e81fa:
    if ((8 < iVar3 - 0x4bU) || ((1 << (iVar3 - 0x4bU & 0xff) & 0x103U) == 0)) {
      if (iVar3 == 0x12) {
        Status::removeMission((Mission *)*piVar22);
        *(undefined1 *)((int)&this->field_68 + 1) = 0;
        this[1].field_C = 0;
        goto LAB_000e763a;
      }
      if (iVar3 == 9) goto LAB_000e8220;
      pSVar10 = (Station *)Status::getStation();
      iVar26 = Station::getIndex(pSVar10);
      iVar4 = DAT_000e854c;
      if (iVar3 == 0x4d && iVar26 == 100) {
        pSVar9 = (Ship *)Status::getStation();
        Ship::makeShip(*(int *)(*(int *)(**(int **)(DAT_000e8928 + 0xe857c) + 4) + 0x94));
        Station::addShip(pSVar9);
LAB_000e7b7a:
        iVar3 = Mission::getReward();
        iVar4 = Mission::getBonus();
        if (DAT_000e7d68 <= (uint)(iVar4 + iVar3)) {
          Status::getCurrentCampaignMission();
        }
        Status::changeCredits(*piVar22);
        Status::removeMission((Mission *)*piVar22);
        iVar3 = this->field_74;
        this[1].field_C = 0;
        if (iVar3 != 0) {
          SpaceLounge::refresh();
        }
        *(undefined1 *)((int)&this->field_68 + 1) = 0;
        goto LAB_000e7c2a;
      }
      iVar26 = *piVar22;
      if (iVar3 == 0x4e) {
        *(undefined4 *)(iVar26 + 0x5c) = 0xffffffff;
        *(undefined4 *)(iVar26 + 0x60) = 0xffffffff;
        *(undefined4 *)(iVar26 + 100) = 0xffffffff;
        *(undefined4 *)(iVar26 + 0x68) = 0xffffffff;
        Achievements::resetNewMedals((Achievements *)**(undefined4 **)(iVar4 + 0xe8424));
        pSVar10 = (Station *)*piVar22;
        Status::getStation();
        Status::departStation(pSVar10);
        uVar7 = **(uint **)(DAT_000e8550 + 0xe844a);
        **(undefined4 **)(DAT_000e8554 + 0xe844c) = 1;
        AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uVar7);
        *(undefined1 *)&this->field_24 = 0;
        goto LAB_000e7b7a;
      }
      pSVar10 = (Station *)Status::getStation();
      iVar4 = Station::getIndex(pSVar10);
      if (iVar3 == 0x54 && iVar4 == 100) {
        pSVar9 = (Ship *)Status::getStation();
        iVar3 = Station::hasShip((int)pSVar9);
        if (iVar3 == 0) {
          Ship::makeShip(*(int *)(*(int *)(**(int **)(DAT_000e8940 + 0xe86a8) + 4) + 0x98));
          Station::addShip(pSVar9);
        }
        iVar3 = Station::hasShip((int)pSVar9);
        if (iVar3 == 0) {
          Ship::makeShip(*(int *)(*(int *)(**(int **)(DAT_000e8944 + 0xe86d0) + 4) + 0xa0));
          Station::addShip(pSVar9);
        }
        pIVar14 = (Item *)Status::getShip();
        Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000e8948 + 0xe86f4) + 4) + 0x224));
        Ship::addCargo(pIVar14);
        goto LAB_000e7b7a;
      }
      if (iVar3 == 0xa0) {
        pSVar10 = (Station *)*piVar22;
        puVar12 = *(undefined4 **)(DAT_000e8afc + 0xe87c8);
        piVar21 = *(int **)(DAT_000e8b00 + 0xe87cc);
        **(undefined1 **)(DAT_000e8af8 + 0xe87c6) = 0;
        iVar3 = *piVar21;
        *puVar12 = 1;
        Galaxy::getStation(iVar3);
        Status::departStation(pSVar10);
        piVar21 = this->field_8;
        if (*piVar23 == local_28) {
          uVar17 = 2;
          goto LAB_000e8260;
        }
        goto LAB_000e764c;
      }
      if (iVar3 == 99) {
LAB_000e8748:
        pSVar10 = (Station *)*piVar22;
        puVar12 = *(undefined4 **)(DAT_000e8ad8 + 0xe875a);
        iVar3 = **(int **)(DAT_000e8ad0 + 0xe8754);
        **(undefined1 **)(DAT_000e8ad4 + 0xe8756) = 1;
      }
      else {
        if (iVar3 == 0x68) {
          if ((HangarWindow *)this->field_78 != (HangarWindow *)0x0) {
            pvVar13 = (void *)HangarWindow::~HangarWindow((HangarWindow *)this->field_78);
            operator_delete(pvVar13);
          }
          *(undefined1 *)((int)&this->field_64 + 2) = 0;
          this->field_78 = 0;
          goto LAB_000e7b7a;
        }
        if (iVar3 == 0x6d) {
          pSVar10 = (Station *)*piVar22;
          puVar12 = *(undefined4 **)(DAT_000e8acc + 0xe8778);
          iVar3 = **(int **)(DAT_000e8ac4 + 0xe8772);
          **(undefined1 **)(DAT_000e8ac8 + 0xe8774) = 1;
        }
        else {
          if (iVar3 == 0x77) goto LAB_000e8748;
          if (iVar3 == 0x80) {
            Status::activateNewWanted();
            if (*(char *)(*(int *)(DAT_000e8adc + 0xe881c) + 0x2a) == '\0') {
              *(undefined1 *)((int)&this->field_6C + 2) = 1;
            }
            goto LAB_000e7b7a;
          }
          if (iVar3 == 0x85) {
            pSVar10 = (Station *)*piVar22;
            puVar12 = *(undefined4 **)(DAT_000e8ae8 + 0xe8842);
            iVar3 = **(int **)(DAT_000e8ae0 + 0xe883c);
            **(undefined1 **)(DAT_000e8ae4 + 0xe883e) = 1;
          }
          else {
            if (iVar3 == 0x90) {
              pSVar10 = (Station *)*piVar22;
              puVar12 = *(undefined4 **)(DAT_000e8af0 + 0xe8794);
              piVar21 = *(int **)(DAT_000e8af4 + 0xe8798);
              **(undefined1 **)(DAT_000e8aec + 0xe8792) = 0;
              iVar3 = *piVar21;
              *puVar12 = 1;
              Galaxy::getStation(iVar3);
              Status::departStation(pSVar10);
              piVar21 = this->field_8;
              goto LAB_000e7780;
            }
            if (iVar3 != 0x59) goto LAB_000e7b7a;
            pSVar10 = (Station *)*piVar22;
            puVar12 = *(undefined4 **)(DAT_000e8934 + 0xe8602);
            iVar3 = **(int **)(DAT_000e892c + 0xe85f8);
            **(undefined1 **)(DAT_000e8930 + 0xe85fc) = 1;
          }
        }
      }
      *puVar12 = 1;
      Galaxy::getStation(iVar3);
      Status::departStation(pSVar10);
      uVar7 = **(uint **)(DAT_000e8ac0 + 0xe8860);
LAB_000e7fa8:
      AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uVar7);
      *(undefined1 *)&this->field_24 = 0;
      goto LAB_000e763a;
    }
  }
LAB_000e8220:
  Status::removeMission((Mission *)*piVar22);
  this[1].field_C = 0;
  Status::setMission((Mission *)*piVar22);
  iVar3 = DAT_000e8524;
  puVar12 = (undefined4 *)(DAT_000e8520 + 0xe8246);
  *(undefined1 *)((int)&this->field_68 + 1) = 0;
  puVar16 = *(undefined4 **)(iVar3 + 0xe824c);
  *(undefined4 *)*puVar12 = 0;
  piVar21 = (int *)*puVar16;
  if (*piVar23 == local_28) {
    uVar17 = 5;
LAB_000e8260:
    (*(code *)(DAT_001ab904 + 0x1ab908))(piVar21,uVar17);
    return;
  }
  goto LAB_000e764c;
}

```

## target disasm
```
  000e7600: push {r4,r5,r6,r7,lr}
  000e7602: add r7,sp,#0xc
  000e7604: push {r8,r9,r10,r11}
  000e7608: sub sp,#0x8c
  000e760a: mov r4,r0
  000e760c: ldr r0,[0x000e7908]
  000e760e: mov r5,r3
  000e7610: mov r8,r2
  000e7612: add r0,pc
  000e7614: mov r10,r1
  000e7616: ldr.w r11,[r0,#0x0]
  000e761a: ldr.w r0,[r11,#0x0]
  000e761e: str r0,[sp,#0x88]
  000e7620: ldr.w r0,[r4,#0x128]
  000e7624: cmp r0,r3
  000e7626: bne 0x000e763a
  000e7628: ldrb.w r0,[r4,#0x60]
  000e762c: movs r1,#0x0
  000e762e: strb.w r1,[r4,#0x118]
  000e7632: cmp r0,#0x0
  000e7634: str.w r1,[r4,#0x128]
  000e7638: beq 0x000e7650
  000e763a: ldr r0,[sp,#0x88]
  000e763c: ldr.w r1,[r11,#0x0]
  000e7640: subs r0,r1,r0
  000e7642: ittt eq
  000e7644: add.eq sp,#0x8c
  000e7646: pop.eq.w {r8,r9,r10,r11}
  000e764a: pop.eq {r4,r5,r6,r7,pc}
  000e764c: blx 0x0006e824
  000e7650: ldr r0,[0x000e790c]
  000e7652: add r0,pc
  000e7654: ldr r6,[r0,#0x0]
  000e7656: ldr r0,[r6,#0x0]
  000e7658: ldrb r1,[r0,#0x0]
  000e765a: cbz r1,0x000e7670
  000e765c: mov r1,r10
  000e765e: mov r2,r8
  000e7660: blx 0x0007477c
  000e7664: cmp r0,#0x0
  000e7666: beq 0x000e763a
  000e7668: ldr r0,[r6,#0x0]
  000e766a: movs r1,#0x0
  000e766c: strb r1,[r0,#0x0]
  000e766e: b 0x000e763a
  000e7670: ldrb.w r0,[r4,#0x69]
  000e7674: cmp r0,#0x0
  000e7676: beq.w 0x000e7c34
  000e767a: ldr.w r0,[r4,#0x84]
  000e767e: mov r1,r10
  000e7680: mov r2,r8
  000e7682: blx 0x00075130
  000e7686: cmp r0,#0x0
  000e7688: beq.w 0x000e7c2a
  000e768c: ldr.w r0,[r4,#0x9c]
  000e7690: cbz r0,0x000e76e6
  000e7692: str r6,[sp,#0xc]
  000e7694: blx 0x00072874
  000e7698: mov r9,r0
  000e769a: ldr.w r0,[r4,#0x9c]
  000e769e: blx 0x0007372c
  000e76a2: cmp.w r9,#0x8
  000e76a6: bne 0x000e778c
  000e76a8: cmp r0,#0x0
  000e76aa: bne.w 0x000e7962
  000e76ae: ldr r0,[0x000e7910]
  000e76b0: add r0,pc
  000e76b2: ldr r0,[r0,#0x0]
  000e76b4: ldr r0,[r0,#0x0]
  000e76b6: blx 0x00071a58
  000e76ba: mov r9,r0
  000e76bc: ldr.w r0,[r4,#0x9c]
  000e76c0: blx 0x000728a4
  000e76c4: mov r6,r0
  000e76c6: ldr.w r0,[r4,#0x9c]
  000e76ca: blx 0x00072bb0
  000e76ce: mov r2,r0
  000e76d0: mov r0,r9
  000e76d2: mov r1,r6
  000e76d4: blx 0x00072454
  000e76d8: ldr r0,[r4,#0x78]
  000e76da: cmp r0,#0x0
  000e76dc: beq.w 0x000e7962
  000e76e0: blx 0x0007513c
  000e76e4: b 0x000e7962
  000e76e6: ldr.w r0,[r4,#0x84]
  000e76ea: movs r5,#0x0
  000e76ec: strb.w r5,[r4,#0x69]
  000e76f0: cbz r0,0x000e76fa
  000e76f2: blx 0x00075010
  000e76f6: blx 0x0006eb48
  000e76fa: ldrb.w r0,[r4,#0x101]
  000e76fe: str.w r5,[r4,#0x84]
  000e7702: cmp r0,#0x0
  000e7704: bne 0x000e763a
  000e7706: ldr r0,[0x000e7914]
  000e7708: add r0,pc
  000e770a: ldr r6,[r0,#0x0]
  000e770c: ldr r0,[r6,#0x0]
  000e770e: blx 0x00071c14
  000e7712: blx 0x00071824
  000e7716: cmp r0,#0x4
  000e7718: beq 0x000e763a
  000e771a: ldr r0,[r6,#0x0]
  000e771c: blx 0x00071c14
  000e7720: blx 0x00071824
  000e7724: cmp r0,#0x58
  000e7726: beq.w 0x000e763a
  000e772a: ldr r0,[r6,#0x0]
  000e772c: blx 0x00071c14
  000e7730: blx 0x00073f00
  000e7734: cmp r0,#0x0
  000e7736: beq.w 0x000e763a
  000e773a: mov.w r8,#0x0
  000e773e: strb.w r8,[r4,#0xb2]
  000e7742: ldr r5,[r6,#0x0]
  000e7744: mov r0,r5
  000e7746: blx 0x00071c14
  000e774a: mov r1,r0
  000e774c: mov r0,r5
  000e774e: blx 0x000745fc
  000e7752: ldr r0,[0x000e7918]
  000e7754: add r0,pc
  000e7756: ldr r0,[r0,#0x0]
  000e7758: ldr r0,[r0,#0x0]
  000e775a: blx 0x00074620
  000e775e: ldr r0,[r6,#0x0]
  000e7760: blx 0x00071770
  000e7764: ldr r1,[0x000e791c]
  000e7766: movs r3,#0x1
  000e7768: ldr r2,[0x000e7920]
  000e776a: cmp r0,#0x10
  000e776c: add r1,pc
  000e776e: add r2,pc
  000e7770: ldr r1,[r1,#0x0]
  000e7772: ldr r2,[r2,#0x0]
  000e7774: ldr r1,[r1,#0x0]
  000e7776: it eq
  000e7778: mov.eq.w r3,#0xffffffff
  000e777c: str r3,[r2,#0x0]
  000e777e: mov r0,r1
  000e7780: movs r1,#0x2
  000e7782: blx 0x00071d64
  000e7786: strb.w r8,[r4,#0x24]
  000e778a: b 0x000e763a
  000e778c: cbnz r0,0x000e77ba
  000e778e: ldr.w r0,[r4,#0x9c]
  000e7792: blx 0x00072874
  000e7796: cmp r0,#0xb
  000e7798: bne 0x000e77ba
  000e779a: ldr r0,[0x000e7924]
  000e779c: movs r1,#0x0
  000e779e: add r0,pc
  000e77a0: ldr r6,[r0,#0x0]
  000e77a2: ldr r0,[r6,#0x0]
  000e77a4: blx 0x00075148
  000e77a8: ldr.w r0,[r4,#0x9c]
  000e77ac: blx 0x0007372c
  000e77b0: cmp r0,#0x0
  000e77b2: str r6,[sp,#0x8]
  000e77b4: beq 0x000e7838
  000e77b6: ldr r0,[r6,#0x0]
  000e77b8: b 0x000e784e
  000e77ba: ldr.w r0,[r4,#0x9c]
  000e77be: blx 0x00072874
  000e77c2: cmp r0,#0x3
  000e77c4: beq.w 0x000e7962
  000e77c8: ldr.w r0,[r4,#0x9c]
  000e77cc: blx 0x00072874
  000e77d0: cmp r0,#0x5
  000e77d2: beq.w 0x000e7962
  000e77d6: ldr.w r0,[r4,#0x9c]
  000e77da: blx 0x00072874
  000e77de: cmp r0,#0xb
  000e77e0: beq.w 0x000e7962
  000e77e4: ldr.w r0,[r4,#0x9c]
  000e77e8: blx 0x00072874
  000e77ec: cmp r0,#0x0
  000e77ee: beq 0x000e78b2
  000e77f0: ldr.w r0,[r4,#0x9c]
  000e77f4: blx 0x00072874
  000e77f8: cmp r0,#0xe
  000e77fa: bne.w 0x000e7962
  000e77fe: ldr r0,[0x000e7928]
  000e7800: add r0,pc
  000e7802: ldr r0,[r0,#0x0]
  000e7804: str r0,[sp,#0x8]
  000e7806: ldr r0,[r0,#0x0]
  000e7808: blx 0x00071a58
  000e780c: blx 0x00073750
  000e7810: mov r9,r0
  000e7812: cmp r0,#0x0
  000e7814: beq.w 0x000e7962
  000e7818: movs r6,#0x0
  000e781a: b 0x000e782e
  000e781c: ldr.w r0,[r9,#0x4]
  000e7820: ldr.w r0,[r0,r6,lsl #0x2]
  000e7824: blx 0x000718d8
  000e7828: cmp r0,#0x73
  000e782a: beq 0x000e789c
  000e782c: adds r6,#0x1
  000e782e: ldr.w r0,[r9,#0x0]
  000e7832: cmp r6,r0
  000e7834: bcc 0x000e781c
  000e7836: b 0x000e7962
  000e7838: ldr.w r0,[r4,#0x9c]
  000e783c: blx 0x00072bb0
  000e7840: mov r1,r0
  000e7842: ldr r0,[r6,#0x0]
  000e7844: ldr.w r2,[r0,#0xb8]
  000e7848: add r1,r2
  000e784a: str.w r1,[r0,#0xb8]
  000e784e: blx 0x00071a58
  000e7852: blx 0x00073750
  000e7856: mov r9,r0
  000e7858: cmp r0,#0x0
  000e785a: beq.w 0x000e7962
  000e785e: movs r6,#0x0
  000e7860: b 0x000e7892
  000e7862: ldr.w r0,[r9,#0x4]
  000e7866: ldr.w r0,[r0,r6,lsl #0x2]
  000e786a: blx 0x00074ad0
  000e786e: cbz r0,0x000e7890
  000e7870: ldr.w r0,[r9,#0x4]
  000e7874: ldr.w r0,[r0,r6,lsl #0x2]
  000e7878: blx 0x000718d8
  000e787c: cmp r0,#0x74
  000e787e: beq 0x000e789c
  000e7880: ldr.w r0,[r9,#0x4]
  000e7884: ldr.w r0,[r0,r6,lsl #0x2]
  000e7888: blx 0x000718d8
  000e788c: cmp r0,#0x75
  000e788e: beq 0x000e789c
  000e7890: adds r6,#0x1
  000e7892: ldr.w r0,[r9,#0x0]
  000e7896: cmp r6,r0
  000e7898: bcc 0x000e7862
  000e789a: b 0x000e7962
  000e789c: ldr r0,[sp,#0x8]
  000e789e: ldr r0,[r0,#0x0]
  000e78a0: blx 0x00071a58
  000e78a4: ldr.w r1,[r9,#0x4]
  000e78a8: ldr.w r1,[r1,r6,lsl #0x2]
  000e78ac: blx 0x000738e8
  000e78b0: b 0x000e76d8
  000e78b2: ldr r0,[0x000e792c]
  000e78b4: add r0,pc
  000e78b6: ldr r0,[r0,#0x0]
  000e78b8: str r0,[sp,#0x8]
  000e78ba: ldr r0,[r0,#0x0]
  000e78bc: blx 0x00071a58
  000e78c0: blx 0x00073750
  000e78c4: mov r9,r0
  000e78c6: cmp r0,#0x0
  000e78c8: beq 0x000e794c
  000e78ca: movs r6,#0x0
  000e78cc: b 0x000e78fe
  000e78ce: ldr.w r0,[r9,#0x4]
  000e78d2: ldr.w r0,[r0,r6,lsl #0x2]
  000e78d6: blx 0x00074ad0
  000e78da: cbz r0,0x000e78fc
  000e78dc: ldr.w r0,[r9,#0x4]
  000e78e0: ldr.w r0,[r0,r6,lsl #0x2]
  000e78e4: blx 0x000718d8
  000e78e8: cmp r0,#0x74
  000e78ea: beq 0x000e7930
  000e78ec: ldr.w r0,[r9,#0x4]
  000e78f0: ldr.w r0,[r0,r6,lsl #0x2]
  000e78f4: blx 0x000718d8
  000e78f8: cmp r0,#0x75
  000e78fa: beq 0x000e7930
  000e78fc: adds r6,#0x1
  000e78fe: ldr.w r0,[r9,#0x0]
  000e7902: cmp r6,r0
  000e7904: bcc 0x000e78ce
  000e7906: b 0x000e794c
  000e7930: ldr r0,[sp,#0x8]
  000e7932: ldr r0,[r0,#0x0]
  000e7934: blx 0x00071a58
  000e7938: ldr.w r1,[r9,#0x4]
  000e793c: ldr.w r1,[r1,r6,lsl #0x2]
  000e7940: blx 0x000738e8
  000e7944: ldr r0,[r4,#0x78]
  000e7946: cbz r0,0x000e794c
  000e7948: blx 0x0007513c
  000e794c: ldr.w r0,[r4,#0x9c]
  000e7950: blx 0x00072bb0
  000e7954: ldr r1,[sp,#0x8]
  000e7956: ldr r1,[r1,#0x0]
  000e7958: ldr.w r2,[r1,#0x9c]
  000e795c: add r0,r2
  000e795e: str.w r0,[r1,#0x9c]
  000e7962: ldr.w r0,[r4,#0x9c]
  000e7966: blx 0x0007372c
  000e796a: mov r1,r0
  000e796c: ldr r0,[0x000e7d44]
  000e796e: cmp r1,#0x0
  000e7970: add r0,pc
  000e7972: ldr r6,[r0,#0x0]
  000e7974: ldr r0,[r6,#0x0]
  000e7976: beq.w 0x000e7b50
  000e797a: blx 0x00071770
  000e797e: mov r1,r0
  000e7980: ldr r0,[r6,#0x0]
  000e7982: cmp r1,#0x2b
  000e7984: bne.w 0x000e7bac
  000e7988: ldr.w r1,[r4,#0x9c]
  000e798c: blx 0x00075154
  000e7990: ldr r0,[0x000e7d48]
  000e7992: add r0,pc
  000e7994: ldr r0,[r0,#0x0]
  000e7996: ldr r1,[r0,#0x0]
  000e7998: ldr r0,[r6,#0x0]
  000e799a: blx 0x00073c3c
  000e799e: ldr r0,[0x000e7d4c]
  000e79a0: movs r6,#0x0
  000e79a2: strb.w r6,[r4,#0x69]
  000e79a6: add r0,pc
  000e79a8: ldr r5,[r0,#0x0]
  000e79aa: ldr r0,[r5,#0x0]
  000e79ac: ldr r1,[r0,#0x0]
  000e79ae: blx 0x000724a8
  000e79b2: ldr r0,[r5,#0x0]
  000e79b4: movs r1,#0x90
  000e79b6: movs r2,#0x0
  000e79b8: movs r3,#0x0
  000e79ba: str r6,[sp,#0x0]
  000e79bc: blx 0x00071548
  000e79c0: movs r0,#0x48
  000e79c2: blx 0x0006eb24
  000e79c6: mov r5,r0
  000e79c8: blx 0x00075160
  000e79cc: movs r0,#0xc
  000e79ce: str r5,[r4,#0x54]
  000e79d0: blx 0x0006eb24
  000e79d4: mov r5,r0
  000e79d6: blx 0x00074380
  000e79da: movs r0,#0x4
  000e79dc: mov r1,r5
  000e79de: str r5,[r4,#0x58]
  000e79e0: blx 0x0007438c
  000e79e4: movs r0,#0x28
  000e79e6: blx 0x0006eb24
  000e79ea: mov r5,r0
  000e79ec: mov.w r0,#0xfa0
  000e79f0: movw r1,#0x817
  000e79f4: str r0,[sp,#0x0]
  000e79f6: mov r0,r5
  000e79f8: movs r2,#0x1
  000e79fa: movs r3,#0x5
  000e79fc: blx 0x00074398
  000e7a00: ldr r0,[r4,#0x58]
  000e7a02: ldr r0,[r0,#0x4]
  000e7a04: str r5,[r0,#0x0]
  000e7a06: movs r0,#0x28
  000e7a08: blx 0x0006eb24
  000e7a0c: mov r5,r0
  000e7a0e: movs r0,#0x0
  000e7a10: movw r1,#0x818
  000e7a14: str r0,[sp,#0x0]
  000e7a16: mov r0,r5
  000e7a18: movs r2,#0x1
  000e7a1a: movs r3,#0x6
  000e7a1c: movs r6,#0x1
  000e7a1e: blx 0x00074398
  000e7a22: ldr r0,[r4,#0x58]
  000e7a24: ldr r0,[r0,#0x4]
  000e7a26: str r5,[r0,#0x4]
  000e7a28: movs r0,#0x28
  000e7a2a: blx 0x0006eb24
  000e7a2e: mov r5,r0
  000e7a30: movw r1,#0x819
  000e7a34: movs r2,#0x1
  000e7a36: movs r3,#0x6
  000e7a38: str r6,[sp,#0x0]
  000e7a3a: blx 0x00074398
  000e7a3e: ldr r0,[r4,#0x58]
  000e7a40: ldr r0,[r0,#0x4]
  000e7a42: str r5,[r0,#0x8]
  000e7a44: movs r0,#0x28
  000e7a46: blx 0x0006eb24
  000e7a4a: mov r5,r0
  000e7a4c: movs r0,#0x2
  000e7a4e: movw r1,#0x81a
  000e7a52: str r0,[sp,#0x0]
  000e7a54: mov r0,r5
  000e7a56: movs r2,#0x1
  000e7a58: movs r3,#0x6
  000e7a5a: blx 0x00074398
  000e7a5e: ldr r0,[r4,#0x58]
  000e7a60: ldr r0,[r0,#0x4]
  000e7a62: str r5,[r0,#0xc]
  000e7a64: ldrd r0,r1,[r4,#0x54]
  000e7a68: blx 0x00074488
  000e7a6c: ldr r0,[0x000e7d50]
  000e7a6e: add.w r2,r4,#0x5c
  000e7a72: movw r1,#0x1b5a
  000e7a76: add r0,pc
  000e7a78: ldr r5,[r0,#0x0]
  000e7a7a: ldr r0,[r5,#0x0]
  000e7a7c: blx 0x00071cf8
  000e7a80: ldr r0,[0x000e7d54]
  000e7a82: ldr r1,[r4,#0x5c]
  000e7a84: add r0,pc
  000e7a86: ldr r6,[r0,#0x0]
  000e7a88: ldr r0,[r5,#0x0]
  000e7a8a: ldr.w r8,[r6,#0x0]
  000e7a8e: blx 0x00072d90
  000e7a92: ldr r1,[r6,#0x0]
  000e7a94: add r0,r8
  000e7a96: add.w r1,r1,r1, lsr #0x1f
  000e7a9a: add.w r0,r0,r1, asr #0x1
  000e7a9e: add r1,sp,#0x7c
  000e7aa0: adds r0,#0xa
  000e7aa2: vmov s0,r0
  000e7aa6: ldr r0,[0x000e7d58]
  000e7aa8: vcvt.f32.s32 s0,s0
  000e7aac: str r0,[sp,#0x7c]
  000e7aae: movs r0,#0x0
  000e7ab0: str r0,[sp,#0x84]
  000e7ab2: add.w r0,r4,#0x104
  000e7ab6: vstr.32 s0,[sp,#0x80]
  000e7aba: blx 0x0006eb3c
  000e7abe: movs r0,#0x40
  000e7ac0: blx 0x0006eb24
  000e7ac4: mov r5,r0
  000e7ac6: ldr r0,[0x000e7d5c]
  000e7ac8: vldr.32 s0,[r4,#0x104]
  000e7acc: add r0,pc
  000e7ace: vldr.32 s2,[r4,#0x108]
  000e7ad2: vadd.f32 s6,s0,s0
  000e7ad6: ldr r0,[r0,#0x0]
  000e7ad8: vldr.32 s4,[r0]
  000e7adc: movw r0,#0x1388
  000e7ae0: vcvt.f32.s32 s4,s4
  000e7ae4: vcvt.s32.f32 s0,s0
  000e7ae8: vcvt.s32.f32 s2,s2
  000e7aec: vsub.f32 s4,s4,s6
  000e7af0: vcvt.s32.f32 s4,s4
  000e7af4: str r0,[sp,#0x0]
  000e7af6: vmov r1,s0
  000e7afa: mov r0,r5
  000e7afc: vmov r2,s2
  000e7b00: vmov r3,s4
  000e7b04: blx 0x0007516c
  000e7b08: ldr r0,[0x000e7d60]
  000e7b0a: movs r1,#0x30
  000e7b0c: str.w r5,[r4,#0x98]
  000e7b10: add r0,pc
  000e7b12: ldr r0,[r0,#0x0]
  000e7b14: ldr r0,[r0,#0x0]
  000e7b16: blx 0x00072f70
  000e7b1a: add r6,sp,#0x70
  000e7b1c: mov r1,r0
  000e7b1e: movs r2,#0x0
  000e7b20: mov.w r8,#0x0
  000e7b24: mov r0,r6
  000e7b26: blx 0x0006f028
  000e7b2a: mov r0,r5
  000e7b2c: mov r1,r6
  000e7b2e: blx 0x00075178
  000e7b32: add r0,sp,#0x70
  000e7b34: blx 0x0006ee30
  000e7b38: ldr.w r0,[r4,#0x98]
  000e7b3c: movs r1,#0x1
  000e7b3e: movs r5,#0x1
  000e7b40: blx 0x00075184
  000e7b44: ldr r0,[0x000e7d64]
  000e7b46: strd r0,r8,[r4,#0xb4]
  000e7b4a: strb.w r5,[r4,#0x60]
  000e7b4e: b 0x000e763a
  000e7b50: blx 0x00075190
  000e7b54: ldr r1,[sp,#0xc]
  000e7b56: ldr.w r0,[r4,#0x9c]
  000e7b5a: ldr r1,[r1,#0x0]
  000e7b5c: str r1,[sp,#0x8]
  000e7b5e: blx 0x00071bc0
  000e7b62: mov r9,r6
  000e7b64: mov r6,r0
  000e7b66: ldr.w r0,[r4,#0x9c]
  000e7b6a: blx 0x00074b78
  000e7b6e: adds r1,r0,r6
  000e7b70: ldr r0,[sp,#0x8]
  000e7b72: movs r2,#0x0
  000e7b74: mov r6,r9
  000e7b76: blx 0x0007519c
  000e7b7a: ldr.w r0,[r4,#0x9c]
  000e7b7e: blx 0x00071bc0
  000e7b82: mov r9,r0
  000e7b84: ldr.w r0,[r4,#0x9c]
  000e7b88: blx 0x00074b78
  000e7b8c: ldr r2,[0x000e7d68]
  000e7b8e: add.w r1,r0,r9
  000e7b92: cmp r1,r2
  000e7b94: bcc 0x000e7c04
  000e7b96: ldr r0,[r6,#0x0]
  000e7b98: blx 0x00071770
  000e7b9c: subs r0,#0x37
  000e7b9e: cmp r0,#0x6
  000e7ba0: bhi 0x000e7c00
  000e7ba2: ldr r1,[0x000e7d6c]
  000e7ba4: add r1,pc
  000e7ba6: ldr.w r1,[r1,r0,lsl #0x2]
  000e7baa: b 0x000e7c04
  000e7bac: blx 0x00071770
  000e7bb0: cmp r0,#0x94
  000e7bb2: beq 0x000e7bbc
  000e7bb4: ldr r0,[r6,#0x0]
  000e7bb6: movs r1,#0x1
  000e7bb8: blx 0x00075070
  000e7bbc: ldr r0,[r6,#0x0]
  000e7bbe: blx 0x00071770
  000e7bc2: mov r9,r0
  000e7bc4: subs r0,#0x2c
  000e7bc6: cmp r0,#0xe
  000e7bc8: bhi.w 0x000e81fa
  000e7bcc: movs r1,#0x1
  000e7bce: movw r2,#0x5830
  000e7bd2: lsls r1,r0
  000e7bd4: tst r1,r2
  000e7bd6: beq.w 0x000e81b8
  000e7bda: ldr r0,[r4,#0x14]
  000e7bdc: str r0,[sp,#0x8]
  000e7bde: ldr r0,[r6,#0x0]
  000e7be0: blx 0x00071a58
  000e7be4: blx 0x000719c8
  000e7be8: mov r9,r0
  000e7bea: ldr r0,[r6,#0x0]
  000e7bec: blx 0x00071a58
  000e7bf0: blx 0x00073db0
  000e7bf4: mov r2,r0
  000e7bf6: ldr r0,[sp,#0x8]
  000e7bf8: mov r1,r9
  000e7bfa: blx 0x00074f98
  000e7bfe: b 0x000e7b7a
  000e7c00: movw r1,#0x1a0a
  000e7c04: ldr r0,[r6,#0x0]
  000e7c06: blx 0x0007171c
  000e7c0a: ldr.w r1,[r4,#0x9c]
  000e7c0e: ldr r0,[r6,#0x0]
  000e7c10: blx 0x00075154
  000e7c14: ldr r0,[r4,#0x74]
  000e7c16: mov.w r9,#0x0
  000e7c1a: str.w r9,[r4,#0x9c]
  000e7c1e: cbz r0,0x000e7c24
  000e7c20: blx 0x000751a8
  000e7c24: ldr r6,[sp,#0xc]
  000e7c26: strb.w r9,[r4,#0x69]
  000e7c2a: ldrb.w r0,[r4,#0x65]
  000e7c2e: cmp r0,#0x0
  000e7c30: bne.w 0x000e763a
  000e7c34: ldrb.w r0,[r4,#0x63]
  000e7c38: cmp r0,#0x0
  000e7c3a: beq 0x000e7d08
  000e7c3c: ldr r0,[r4,#0x70]
  000e7c3e: mov r1,r10
  000e7c40: mov r2,r8
  000e7c42: blx 0x00074770
  000e7c46: cmp r0,#0x1
  000e7c48: beq.w 0x000e7d7c
  000e7c4c: cmp r0,#0x0
  000e7c4e: bne.w 0x000e763a
  000e7c52: ldrb.w r0,[r4,#0xb2]
  000e7c56: cmp r0,#0x0
  000e7c58: beq.w 0x000e7ea2
  000e7c5c: ldr r0,[0x000e7d70]
  000e7c5e: add r0,pc
  000e7c60: ldr.w r8,[r0,#0x0]
  000e7c64: movs r0,#0x0
  000e7c66: strb.w r0,[r4,#0xb2]
  000e7c6a: ldr.w r0,[r8,#0x0]
  000e7c6e: blx 0x00071770
  000e7c72: cmp r0,#0x18
  000e7c74: bne 0x000e7cd4
  000e7c76: ldr.w r0,[r8,#0x0]
  000e7c7a: blx 0x00071c14
  000e7c7e: blx 0x00071824
  000e7c82: cmp r0,#0xa
  000e7c84: bne 0x000e7cd4
  000e7c86: movs r6,#0x0
  000e7c88: b 0x000e7cc2
  000e7c8a: ldr.w r0,[r8,#0x0]
  000e7c8e: blx 0x00071c14
  000e7c92: blx 0x00074104
  000e7c96: ldr r0,[r0,#0x4]
  000e7c98: ldr.w r5,[r0,r6,lsl #0x2]
  000e7c9c: mov r0,r5
  000e7c9e: blx 0x00071ae8
  000e7ca2: cmp r0,#0x2
  000e7ca4: bne 0x000e7cc0
  000e7ca6: mov r0,r5
  000e7ca8: blx 0x00074bb4
  000e7cac: cmp r0,#0x44
  000e7cae: bne 0x000e7cc0
  000e7cb0: mov r0,r5
  000e7cb2: movs r1,#0x1
  000e7cb4: blx 0x000749bc
  000e7cb8: mov r0,r5
  000e7cba: movs r1,#0x1
  000e7cbc: blx 0x00071b18
  000e7cc0: adds r6,#0x1
  000e7cc2: ldr.w r0,[r8,#0x0]
  000e7cc6: blx 0x00071c14
  000e7cca: blx 0x00074104
  000e7cce: ldr r0,[r0,#0x0]
  000e7cd0: cmp r6,r0
  000e7cd2: bcc 0x000e7c8a
  000e7cd4: ldr.w r0,[r8,#0x0]
  000e7cd8: blx 0x00071770
  000e7cdc: ldr.w r5,[r8,#0x0]
  000e7ce0: cmp r0,#0x30
  000e7ce2: bne.w 0x000e7f6c
  000e7ce6: ldr r0,[0x000e7d74]
  000e7ce8: movs r1,#0x3a
  000e7cea: add r0,pc
  000e7cec: ldr r0,[r0,#0x0]
  000e7cee: ldr r0,[r0,#0x0]
  000e7cf0: blx 0x00071c44
  000e7cf4: mov r1,r0
  000e7cf6: mov r0,r5
  000e7cf8: blx 0x000745fc
  000e7cfc: ldr r0,[0x000e7d78]
  000e7cfe: movs r1,#0x1
  000e7d00: add r0,pc
  000e7d02: ldr r0,[r0,#0x0]
  000e7d04: strb r1,[r0,#0x0]
  000e7d06: b 0x000e7f7a
  000e7d08: ldrb.w r0,[r4,#0x6a]
  000e7d0c: cmp r0,#0x0
  000e7d0e: beq 0x000e7dd2
  000e7d10: ldr.w r0,[r4,#0x88]
  000e7d14: mov r1,r10
  000e7d16: mov r2,r8
  000e7d18: blx 0x00074770
  000e7d1c: cmp r0,#0x0
  000e7d1e: bne.w 0x000e763a
  000e7d22: ldr r0,[sp,#0x88]
  000e7d24: ldr.w r1,[r11,#0x0]
  000e7d28: subs r0,r1,r0
  000e7d2a: itttt eq
  000e7d2c: mov.eq r0,r4
  000e7d2e: add.eq sp,#0x8c
  000e7d30: pop.eq.w {r8,r9,r10,r11}
  000e7d34: pop.eq.w {r4,r5,r6,r7,lr}
  000e7d38: it eq
  000e7d3a: b.eq.w 0x001ac118
  000e7d3e: blx 0x0006e824
  000e7d7c: ldrb.w r0,[r4,#0x6b]
  000e7d80: movs r6,#0x0
  000e7d82: strb.w r6,[r4,#0xb2]
  000e7d86: cmp r0,#0x0
  000e7d88: beq.w 0x000e7f60
  000e7d8c: ldr r0,[0x000e80f0]
  000e7d8e: strb.w r6,[r4,#0xb2]
  000e7d92: add r0,pc
  000e7d94: ldr r0,[r0,#0x0]
  000e7d96: ldr r5,[r0,#0x0]
  000e7d98: mov r0,r5
  000e7d9a: blx 0x00071c14
  000e7d9e: mov r1,r0
  000e7da0: mov r0,r5
  000e7da2: blx 0x000745fc
  000e7da6: ldr r0,[0x000e80f4]
  000e7da8: add r0,pc
  000e7daa: ldr r0,[r0,#0x0]
  000e7dac: ldr r0,[r0,#0x0]
  000e7dae: blx 0x00074620
  000e7db2: ldr r0,[0x000e80f8]
  000e7db4: movs r2,#0x1
  000e7db6: ldr r1,[0x000e80fc]
  000e7db8: add r0,pc
  000e7dba: add r1,pc
  000e7dbc: ldr r0,[r0,#0x0]
  000e7dbe: ldr r1,[r1,#0x0]
  000e7dc0: ldr r0,[r0,#0x0]
  000e7dc2: str r2,[r1,#0x0]
  000e7dc4: movs r1,#0x2
  000e7dc6: blx 0x00071d64
  000e7dca: strb.w r6,[r4,#0x24]
  000e7dce: b.w 0x000e763a
  000e7dd2: ldrb.w r0,[r4,#0x66]
  000e7dd6: cmp r0,#0x0
  000e7dd8: beq.w 0x000e7fb8
  000e7ddc: ldr r0,[r4,#0x78]
  000e7dde: mov r1,r10
  000e7de0: mov r2,r8
  000e7de2: blx 0x000751c0
  000e7de6: cmp r0,#0x0
  000e7de8: beq.w 0x000e763a
  000e7dec: ldr r0,[0x000e8100]
  000e7dee: add r0,pc
  000e7df0: ldr.w r8,[r0,#0x0]
  000e7df4: ldr.w r0,[r8,#0x0]
  000e7df8: blx 0x00071a58
  000e7dfc: blx 0x00073750
  000e7e00: mov r5,r0
  000e7e02: mov.w r9,#0x0
  000e7e06: cbz r0,0x000e7e3c
  000e7e08: movs r6,#0x0
  000e7e0a: b 0x000e7e36
  000e7e0c: ldr r0,[r5,#0x4]
  000e7e0e: ldr.w r0,[r0,r6,lsl #0x2]
  000e7e12: blx 0x000718d8
  000e7e16: cmp r0,#0x84
  000e7e18: blt 0x000e7e34
  000e7e1a: ldr r0,[r5,#0x4]
  000e7e1c: ldr.w r0,[r0,r6,lsl #0x2]
  000e7e20: blx 0x000718d8
  000e7e24: cmp r0,#0x99
  000e7e26: bgt 0x000e7e34
  000e7e28: ldr r0,[r5,#0x4]
  000e7e2a: ldr.w r0,[r0,r6,lsl #0x2]
  000e7e2e: blx 0x0007183c
  000e7e32: add r9,r0
  000e7e34: adds r6,#0x1
  000e7e36: ldr r0,[r5,#0x0]
  000e7e38: cmp r6,r0
  000e7e3a: bcc 0x000e7e0c
  000e7e3c: ldr.w r0,[r4,#0xd0]
  000e7e40: cmp r9,r0
  000e7e42: ble 0x000e7e56
  000e7e44: ldr.w r1,[r8,#0x0]
  000e7e48: sub.w r0,r9,r0
  000e7e4c: ldr.w r2,[r1,#0xa8]
  000e7e50: add r0,r2
  000e7e52: str.w r0,[r1,#0xa8]
  000e7e56: ldr r0,[r4,#0x78]
  000e7e58: movs r1,#0x0
  000e7e5a: movs r5,#0x0
  000e7e5c: blx 0x000751cc
  000e7e60: mov r0,r4
  000e7e62: blx 0x000751d8
  000e7e66: ldr r0,[r4,#0x78]
  000e7e68: strb.w r5,[r4,#0x66]
  000e7e6c: ldrb r1,[r0,#0x0]
  000e7e6e: cmp r1,#0x0
  000e7e70: beq.w 0x000e826e
  000e7e74: strb r5,[r0,#0x0]
  000e7e76: mov r5,r4
  000e7e78: ldr.w r0,[r8,#0x0]
  000e7e7c: ldr.w r9,[r5,#0x14]!
  000e7e80: blx 0x00071a58
  000e7e84: blx 0x000719c8
  000e7e88: mov r6,r0
  000e7e8a: ldr.w r0,[r8,#0x0]
  000e7e8e: blx 0x00071a58
  000e7e92: blx 0x00073db0
  000e7e96: mov r2,r0
  000e7e98: mov r0,r9
  000e7e9a: mov r1,r6
  000e7e9c: blx 0x00074f98
  000e7ea0: b 0x000e8272
  000e7ea2: ldrb.w r0,[r4,#0x6b]
  000e7ea6: cmp r0,#0x0
  000e7ea8: beq.w 0x000e803a
  000e7eac: ldr r0,[0x000e8104]
  000e7eae: add r0,pc
  000e7eb0: ldr r5,[r0,#0x0]
  000e7eb2: ldr r0,[r5,#0x0]
  000e7eb4: blx 0x000733d8
  000e7eb8: ldr.w r1,[r4,#0xd4]
  000e7ebc: cmp r0,r1
  000e7ebe: bge.w 0x000e800a
  000e7ec2: ldr r0,[0x000e8108]
  000e7ec4: movs r1,#0xcb
  000e7ec6: add r0,pc
  000e7ec8: ldr r0,[r0,#0x0]
  000e7eca: ldr r0,[r0,#0x0]
  000e7ecc: blx 0x00072f70
  000e7ed0: add.w r9,sp,#0x7c
  000e7ed4: mov r1,r0
  000e7ed6: movs r2,#0x0
  000e7ed8: mov r0,r9
  000e7eda: blx 0x0006f028
  000e7ede: ldr.w r8,[r5,#0x0]
  000e7ee2: add r0,sp,#0x58
  000e7ee4: mov r1,r9
  000e7ee6: movs r2,#0x0
  000e7ee8: blx 0x0006f028
  000e7eec: ldr.w r10,[r4,#0xd4]
  000e7ef0: ldr.w r9,[r6,#0x0]
  000e7ef4: ldr r0,[r5,#0x0]
  000e7ef6: blx 0x000733d8
  000e7efa: sub.w r2,r10,r0
  000e7efe: add r0,sp,#0x4c
  000e7f00: mov r1,r9
  000e7f02: blx 0x00074e54
  000e7f06: ldr r1,[0x000e810c]
  000e7f08: add r1,pc
  000e7f0a: add r0,sp,#0x40
  000e7f0c: movs r2,#0x0
  000e7f0e: blx 0x0006ee18
  000e7f12: str r0,[sp,#0x0]
  000e7f14: add r0,sp,#0x64
  000e7f16: add r2,sp,#0x58
  000e7f18: add r3,sp,#0x4c
  000e7f1a: mov r1,r8
  000e7f1c: blx 0x000739b4
  000e7f20: add r0,sp,#0x7c
  000e7f22: add r1,sp,#0x64
  000e7f24: blx 0x0006f2b0
  000e7f28: ldr r0,[0x000e8110]
  000e7f2a: add r0,pc
  000e7f2c: ldr r5,[r0,#0x0]
  000e7f2e: add r0,sp,#0x64
  000e7f30: blx r5
  000e7f32: add r0,sp,#0x40
  000e7f34: blx r5
  000e7f36: add r0,sp,#0x4c
  000e7f38: blx r5
  000e7f3a: add r0,sp,#0x58
  000e7f3c: blx r5
  000e7f3e: ldr r0,[r4,#0x70]
  000e7f40: add r1,sp,#0x7c
  000e7f42: movs r2,#0x0
  000e7f44: blx 0x000746f8
  000e7f48: movs r0,#0x1
  000e7f4a: strb.w r0,[r4,#0x6b]
  000e7f4e: strb.w r0,[r4,#0xb2]
  000e7f52: strb.w r0,[r4,#0x63]
  000e7f56: add r0,sp,#0x7c
  000e7f58: blx 0x0006ee30
  000e7f5c: b.w 0x000e763a
  000e7f60: strb.w r6,[r4,#0x63]
  000e7f64: strb.w r6,[r4,#0xdc]
  000e7f68: b.w 0x000e763a
  000e7f6c: mov r0,r5
  000e7f6e: blx 0x00071c14
  000e7f72: mov r1,r0
  000e7f74: mov r0,r5
  000e7f76: blx 0x000745fc
  000e7f7a: ldr r0,[0x000e8114]
  000e7f7c: mov.w r2,#0xffffffff
  000e7f80: ldr.w r1,[r8,#0x0]
  000e7f84: add r0,pc
  000e7f86: ldr r0,[r0,#0x0]
  000e7f88: strd r2,r2,[r1,#0x5c]
  000e7f8c: strd r2,r2,[r1,#0x64]
  000e7f90: ldr r0,[r0,#0x0]
  000e7f92: blx 0x00074620
  000e7f96: ldr r0,[0x000e8118]
  000e7f98: movs r2,#0x1
  000e7f9a: ldr r1,[0x000e811c]
  000e7f9c: add r0,pc
  000e7f9e: add r1,pc
  000e7fa0: ldr r0,[r0,#0x0]
  000e7fa2: ldr r1,[r1,#0x0]
  000e7fa4: ldr r0,[r0,#0x0]
  000e7fa6: str r2,[r1,#0x0]
  000e7fa8: movs r1,#0x2
  000e7faa: blx 0x00071d64
  000e7fae: movs r0,#0x0
  000e7fb0: strb.w r0,[r4,#0x24]
  000e7fb4: b.w 0x000e763a
  000e7fb8: ldrb.w r0,[r4,#0x67]
  000e7fbc: cmp r0,#0x0
  000e7fbe: beq.w 0x000e8138
  000e7fc2: ldr r0,[r4,#0x10]
  000e7fc4: mov r1,r10
  000e7fc6: mov r2,r8
  000e7fc8: blx 0x000751e4
  000e7fcc: cmp r0,#0x0
  000e7fce: beq.w 0x000e763a
  000e7fd2: movs r0,#0x0
  000e7fd4: strb.w r0,[r4,#0x67]
  000e7fd8: mov r0,r4
  000e7fda: blx 0x00074fbc
  000e7fde: ldr r0,[0x000e8120]
  000e7fe0: add r0,pc
  000e7fe2: ldr r0,[r0,#0x0]
  000e7fe4: ldr r0,[r0,#0x0]
  000e7fe6: ldr r2,[r0,#0x0]
  000e7fe8: ldr r1,[sp,#0x88]
  000e7fea: ldr.w r3,[r11,#0x0]
  000e7fee: subs r1,r3,r1
  000e7ff0: itttt eq
  000e7ff2: mov.eq r1,#0x0
  000e7ff4: mov.eq r3,#0x0
  000e7ff6: add.eq sp,#0x8c
  000e7ff8: pop.eq.w {r8,r9,r10,r11}
  000e7ffc: itt eq
  000e7ffe: pop.eq.w {r4,r5,r6,r7,lr}
  000e8002: b.eq.w 0x001ab808
  000e8006: blx 0x0006e824
  000e800a: ldr r0,[r5,#0x0]
  000e800c: rsbs r1,r1
  000e800e: blx 0x0007171c
  000e8012: movs r0,#0x0
  000e8014: mov.w r8,#0x1
  000e8018: strb.w r0,[r4,#0x6b]
  000e801c: strb.w r8,[r4,#0xd8]
  000e8020: ldr r0,[r5,#0x0]
  000e8022: blx 0x00071c14
  000e8026: movs r1,#0x0
  000e8028: blx 0x00074470
  000e802c: strb.w r8,[r4,#0x101]
  000e8030: blx 0x00074fc8
  000e8034: mov r0,r4
  000e8036: blx 0x00074f8c
  000e803a: ldrb.w r0,[r4,#0xdc]
  000e803e: cbz r0,0x000e8076
  000e8040: ldr r0,[0x000e8124]
  000e8042: movs r1,#0x0
  000e8044: strb.w r1,[r4,#0x63]
  000e8048: add r0,pc
  000e804a: strb.w r1,[r4,#0xdc]
  000e804e: ldr r0,[r0,#0x0]
  000e8050: ldr r0,[r0,#0x0]
  000e8052: blx 0x00071704
  000e8056: movs r1,#0x1
  000e8058: strb r1,[r0,#0xd]
  000e805a: ldr r0,[sp,#0x88]
  000e805c: ldr.w r1,[r11,#0x0]
  000e8060: subs r0,r1,r0
  000e8062: itttt eq
  000e8064: add.eq sp,#0x8c
  000e8066: pop.eq.w {r8,r9,r10,r11}
  000e806a: pop.eq.w {r4,r5,r6,r7,lr}
  000e806e: b.eq.w 0x001ac128
  000e8072: blx 0x0006e824
  000e8076: ldrb.w r0,[r4,#0x6c]
  000e807a: cmp r0,#0x0
  000e807c: beq.w 0x000e8310
  000e8080: ldr r0,[0x000e8128]
  000e8082: ldr r1,[0x000e812c]
  000e8084: add r0,pc
  000e8086: ldr r6,[r0,#0x0]
  000e8088: ldr r0,[r6,#0x0]
  000e808a: blx 0x0007171c
  000e808e: ldr r0,[r6,#0x0]
  000e8090: blx 0x00071a58
  000e8094: movs r1,#0x6d
  000e8096: movs r2,#0x32
  000e8098: blx 0x00072454
  000e809c: ldr r0,[0x000e8130]
  000e809e: movs r2,#0x3
  000e80a0: ldr r1,[r6,#0x0]
  000e80a2: add r0,pc
  000e80a4: ldr r0,[r0,#0x0]
  000e80a6: str.w r2,[r1,#0x114]
  000e80aa: ldr r0,[r0,#0x0]
  000e80ac: blx 0x00071788
  000e80b0: ldr r0,[0x000e8134]
  000e80b2: movw r1,#0x1e5
  000e80b6: ldr r5,[r4,#0x70]
  000e80b8: add r0,pc
  000e80ba: ldr r0,[r0,#0x0]
  000e80bc: ldr r0,[r0,#0x0]
  000e80be: blx 0x00072f70
  000e80c2: mov r1,r0
  000e80c4: mov r0,r5
  000e80c6: blx 0x00074794
  000e80ca: ldr r0,[r6,#0x0]
  000e80cc: blx 0x00071c14
  000e80d0: movs r1,#0x0
  000e80d2: movs r2,#0x0
  000e80d4: movs r5,#0x0
  000e80d6: blx 0x000736e4
  000e80da: ldr r0,[r6,#0x0]
  000e80dc: movs r1,#0x0
  000e80de: movs r2,#0x0
  000e80e0: ldr.w r0,[r0,#0x14c]
  000e80e4: blx 0x000736e4
  000e80e8: strb.w r5,[r4,#0x6c]
  000e80ec: b.w 0x000e763a
  000e8138: ldrb.w r0,[r4,#0x65]
  000e813c: cmp r0,#0x0
  000e813e: beq.w 0x000e83ba
  000e8142: ldr r0,[r4,#0x74]
  000e8144: mov r1,r10
  000e8146: mov r2,r8
  000e8148: blx 0x000751fc
  000e814c: cmp r0,#0x0
  000e814e: beq.w 0x000e763a
  000e8152: movs r5,#0x0
  000e8154: mov r0,r4
  000e8156: strb.w r5,[r4,#0x65]
  000e815a: blx 0x000751d8
  000e815e: mov r0,r4
  000e8160: blx 0x00074fbc
  000e8164: ldr r0,[0x000e8514]
  000e8166: movs r1,#0x0
  000e8168: movs r3,#0x0
  000e816a: add r0,pc
  000e816c: ldr r6,[r0,#0x0]
  000e816e: ldr r0,[r6,#0x0]
  000e8170: ldr r2,[r0,#0x0]
  000e8172: blx 0x0007159c
  000e8176: ldr r0,[r6,#0x0]
  000e8178: movs r1,#0x6c
  000e817a: blx 0x000724a8
  000e817e: ldr r0,[r6,#0x0]
  000e8180: movs r1,#0x7a
  000e8182: movs r2,#0x0
  000e8184: movs r3,#0x0
  000e8186: str r5,[sp,#0x0]
  000e8188: blx 0x00071548
  000e818c: ldr r0,[r4,#0x14]
  000e818e: blx 0x000720dc
  000e8192: ldr r0,[r4,#0x74]
  000e8194: cmp r0,#0x0
  000e8196: beq.w 0x000e763a
  000e819a: blx 0x00075208
  000e819e: cmp r0,#0x0
  000e81a0: beq.w 0x000e763a
  000e81a4: ldr r0,[r4,#0x78]
  000e81a6: cbz r0,0x000e81b0
  000e81a8: blx 0x00075214
  000e81ac: blx 0x0006eb48
  000e81b0: movs r0,#0x0
  000e81b2: str r0,[r4,#0x78]
  000e81b4: b.w 0x000e763a
  000e81b8: cbz r0,0x000e8220
  000e81ba: cmp r0,#0x1
  000e81bc: bne 0x000e81fa
  000e81be: ldr.w r1,[r4,#0x9c]
  000e81c2: ldr r0,[r6,#0x0]
  000e81c4: blx 0x00075154
  000e81c8: ldr r0,[0x000e8518]
  000e81ca: movs r5,#0x0
  000e81cc: str.w r5,[r4,#0x9c]
  000e81d0: add r0,pc
  000e81d2: ldr r0,[r0,#0x0]
  000e81d4: ldr r1,[r0,#0x0]
  000e81d6: ldr r0,[r6,#0x0]
  000e81d8: blx 0x00073c3c
  000e81dc: ldr r0,[r4,#0x0]
  000e81de: movs r1,#0x1
  000e81e0: str r1,[r4,#0x44]
  000e81e2: strb.w r5,[r4,#0x69]
  000e81e6: ldr r1,[r0,#0x10]
  000e81e8: strd r5,r5,[sp,#0x0]
  000e81ec: mov r0,r4
  000e81ee: mov.w r2,#0x10000
  000e81f2: movs r3,#0x0
  000e81f4: blx r1
  000e81f6: b.w 0x000e763a
  000e81fa: sub.w r0,r9,#0x4b
  000e81fe: cmp r0,#0x8
  000e8200: bhi 0x000e8210
  000e8202: movs r1,#0x1
  000e8204: lsl.w r0,r1,r0
  000e8208: movw r1,#0x103
  000e820c: tst r0,r1
  000e820e: bne 0x000e8220
  000e8210: cmp.w r9,#0x12
  000e8214: beq.w 0x000e83de
  000e8218: cmp.w r9,#0x9
  000e821c: bne.w 0x000e83f6
  000e8220: ldr.w r1,[r4,#0x9c]
  000e8224: ldr r0,[r6,#0x0]
  000e8226: blx 0x00075154
  000e822a: ldr r0,[0x000e851c]
  000e822c: movs r5,#0x0
  000e822e: str.w r5,[r4,#0x9c]
  000e8232: add r0,pc
  000e8234: ldr r0,[r0,#0x0]
  000e8236: ldr r1,[r0,#0x0]
  000e8238: ldr r0,[r6,#0x0]
  000e823a: blx 0x00073c3c
  000e823e: ldr r0,[0x000e8520]
  000e8240: ldr r1,[0x000e8524]
  000e8242: add r0,pc
  000e8244: strb.w r5,[r4,#0x69]
  000e8248: add r1,pc
  000e824a: ldr r0,[r0,#0x0]
  000e824c: ldr r1,[r1,#0x0]
  000e824e: str r5,[r0,#0x0]
  000e8250: ldr r0,[r1,#0x0]
  000e8252: ldr r1,[sp,#0x88]
  000e8254: ldr.w r2,[r11,#0x0]
  000e8258: subs r1,r2,r1
  000e825a: bne.w 0x000e764c
  000e825e: movs r1,#0x5
  000e8260: add sp,#0x8c
  000e8262: pop.w {r8,r9,r10,r11}
  000e8266: pop.w {r4,r5,r6,r7,lr}
  000e826a: b.w 0x001ab8f8
  000e826e: add.w r5,r4,#0x14
  000e8272: ldr r0,[r5,#0x0]
  000e8274: cbz r0,0x000e827a
  000e8276: blx 0x000720dc
  000e827a: ldr r0,[0x000e8528]
  000e827c: movs r1,#0x5f
  000e827e: add r0,pc
  000e8280: ldr r5,[r0,#0x0]
  000e8282: ldr r0,[r5,#0x0]
  000e8284: blx 0x000724a8
  000e8288: ldr r0,[r5,#0x0]
  000e828a: movs r6,#0x0
  000e828c: movs r1,#0x7a
  000e828e: movs r2,#0x0
  000e8290: movs r3,#0x0
  000e8292: str r6,[sp,#0x0]
  000e8294: blx 0x00071548
  000e8298: ldr r0,[r5,#0x0]
  000e829a: movs r1,#0x0
  000e829c: movs r3,#0x0
  000e829e: ldr r2,[r0,#0x0]
  000e82a0: blx 0x0007159c
  000e82a4: ldr r0,[0x000e852c]
  000e82a6: add.w r8,sp,#0x7c
  000e82aa: add r0,pc
  000e82ac: ldr.w r9,[r0,#0x0]
  000e82b0: ldr r0,[0x000e8530]
  000e82b2: add r0,pc
  000e82b4: ldr r5,[r0,#0x0]
  000e82b6: b 0x000e82fa
  000e82b8: cmp r6,#0x9
  000e82ba: bhi 0x000e82f8
  000e82bc: ldr r0,[r0,#0x4]
  000e82be: ldr.w r1,[r0,r6,lsl #0x2]
  000e82c2: mov r0,r8
  000e82c4: blx 0x00074dd0
  000e82c8: vldr.32 s0,[sp,#0x7c]
  000e82cc: vcvt.s32.f32 s0,s0
  000e82d0: vmov r0,s0
  000e82d4: str.w r0,[r9,r6,lsl #0x2]
  000e82d8: ldr.w r0,[r4,#0x8c]
  000e82dc: ldr r0,[r0,#0x4]
  000e82de: ldr.w r1,[r0,r6,lsl #0x2]
  000e82e2: mov r0,r8
  000e82e4: blx 0x00074dd0
  000e82e8: vldr.32 s0,[sp,#0x80]
  000e82ec: vcvt.s32.f32 s0,s0
  000e82f0: vmov r0,s0
  000e82f4: str.w r0,[r5,r6,lsl #0x2]
  000e82f8: adds r6,#0x1
  000e82fa: ldr.w r0,[r4,#0x8c]
  000e82fe: ldr r1,[r0,#0x0]
  000e8300: cmp r6,r1
  000e8302: bcc 0x000e82b8
  000e8304: ldr r0,[0x000e8534]
  000e8306: add r0,pc
  000e8308: ldr r0,[r0,#0x0]
  000e830a: str r1,[r0,#0x0]
  000e830c: b.w 0x000e763a
  000e8310: ldrb.w r0,[r4,#0x6d]
  000e8314: cmp r0,#0x0
  000e8316: beq.w 0x000e84f6
  000e831a: ldr r0,[0x000e8538]
  000e831c: add r0,pc
  000e831e: ldr r5,[r0,#0x0]
  000e8320: ldr r0,[r5,#0x0]
  000e8322: blx 0x000733d8
  000e8326: movw r1,#0x61a7
  000e832a: cmp r0,r1
  000e832c: bgt.w 0x000e84b6
  000e8330: ldr r0,[0x000e853c]
  000e8332: movs r1,#0xcb
  000e8334: add r0,pc
  000e8336: ldr r0,[r0,#0x0]
  000e8338: ldr r0,[r0,#0x0]
  000e833a: blx 0x00072f70
  000e833e: add.w r9,sp,#0x7c
  000e8342: mov r1,r0
  000e8344: movs r2,#0x0
  000e8346: mov r0,r9
  000e8348: blx 0x0006f028
  000e834c: ldr.w r8,[r5,#0x0]
  000e8350: add r0,sp,#0x34
  000e8352: mov r1,r9
  000e8354: movs r2,#0x0
  000e8356: blx 0x0006f028
  000e835a: ldr r6,[r6,#0x0]
  000e835c: ldr r0,[r5,#0x0]
  000e835e: blx 0x000733d8
  000e8362: movw r1,#0x61a8
  000e8366: subs r2,r1,r0
  000e8368: add r0,sp,#0x28
  000e836a: mov r1,r6
  000e836c: blx 0x00074e54
  000e8370: ldr r1,[0x000e8540]
  000e8372: add r1,pc
  000e8374: add r0,sp,#0x1c
  000e8376: movs r2,#0x0
  000e8378: blx 0x0006ee18
  000e837c: str r0,[sp,#0x0]
  000e837e: add r0,sp,#0x64
  000e8380: add r2,sp,#0x34
  000e8382: add r3,sp,#0x28
  000e8384: mov r1,r8
  000e8386: blx 0x000739b4
  000e838a: add r0,sp,#0x7c
  000e838c: add r1,sp,#0x64
  000e838e: blx 0x0006f2b0
  000e8392: ldr r0,[0x000e8544]
  000e8394: add r0,pc
  000e8396: ldr r5,[r0,#0x0]
  000e8398: add r0,sp,#0x64
  000e839a: blx r5
  000e839c: add r0,sp,#0x1c
  000e839e: blx r5
  000e83a0: add r0,sp,#0x28
  000e83a2: blx r5
  000e83a4: add r0,sp,#0x34
  000e83a6: blx r5
  000e83a8: ldr r0,[r4,#0x70]
  000e83aa: add r1,sp,#0x7c
  000e83ac: movs r2,#0x0
  000e83ae: movs r5,#0x0
  000e83b0: blx 0x000746f8
  000e83b4: strb.w r5,[r4,#0x6d]
  000e83b8: b 0x000e7f56
  000e83ba: ldrb.w r0,[r4,#0x68]
  000e83be: cmp r0,#0x0
  000e83c0: beq 0x000e8462
  000e83c2: ldr r0,[r4,#0x7c]
  000e83c4: mov r1,r10
  000e83c6: mov r2,r8
  000e83c8: blx 0x00075220
  000e83cc: cmp r0,#0x0
  000e83ce: beq.w 0x000e763a
  000e83d2: ldr r0,[0x000e8548]
  000e83d4: movs r1,#0x0
  000e83d6: strb.w r1,[r4,#0x68]
  000e83da: add r0,pc
  000e83dc: b 0x000e7fe2
  000e83de: ldr.w r1,[r4,#0x9c]
  000e83e2: ldr r0,[r6,#0x0]
  000e83e4: blx 0x00075154
  000e83e8: movs r0,#0x0
  000e83ea: strb.w r0,[r4,#0x69]
  000e83ee: str.w r0,[r4,#0x9c]
  000e83f2: b.w 0x000e763a
  000e83f6: ldr r0,[r6,#0x0]
  000e83f8: blx 0x00071c14
  000e83fc: blx 0x00071824
  000e8400: cmp.w r9,#0x4d
  000e8404: it eq
  000e8406: cmp.eq r0,#0x64
  000e8408: beq.w 0x000e856c
  000e840c: ldr r0,[r6,#0x0]
  000e840e: cmp.w r9,#0x4e
  000e8412: bne.w 0x000e8594
  000e8416: ldr r1,[0x000e854c]
  000e8418: mov.w r2,#0xffffffff
  000e841c: strd r2,r2,[r0,#0x5c]
  000e8420: add r1,pc
  000e8422: strd r2,r2,[r0,#0x64]
  000e8426: ldr r1,[r1,#0x0]
  000e8428: ldr r0,[r1,#0x0]
  000e842a: blx 0x00074620
  000e842e: ldr.w r9,[r6,#0x0]
  000e8432: mov r0,r9
  000e8434: blx 0x00071c14
  000e8438: mov r1,r0
  000e843a: mov r0,r9
  000e843c: blx 0x000745fc
  000e8440: ldr r0,[0x000e8550]
  000e8442: movs r2,#0x1
  000e8444: ldr r1,[0x000e8554]
  000e8446: add r0,pc
  000e8448: add r1,pc
  000e844a: ldr r0,[r0,#0x0]
  000e844c: ldr r1,[r1,#0x0]
  000e844e: ldr r0,[r0,#0x0]
  000e8450: str r2,[r1,#0x0]
  000e8452: movs r1,#0x2
  000e8454: blx 0x00071d64
  000e8458: movs r0,#0x0
  000e845a: strb.w r0,[r4,#0x24]
  000e845e: b.w 0x000e7b7a
  000e8462: ldrb.w r0,[r4,#0x64]
  000e8466: cmp r0,#0x0
  000e8468: beq.w 0x000e860c
  000e846c: ldr.w r0,[r4,#0x80]
  000e8470: mov r1,r10
  000e8472: mov r2,r8
  000e8474: blx 0x0007522c
  000e8478: cmp r0,#0x0
  000e847a: beq.w 0x000e763a
  000e847e: ldr r0,[0x000e8558]
  000e8480: movs r1,#0x0
  000e8482: strb.w r1,[r4,#0x64]
  000e8486: movs r1,#0x0
  000e8488: add r0,pc
  000e848a: movs r3,#0x0
  000e848c: ldr r0,[r0,#0x0]
  000e848e: ldr r0,[r0,#0x0]
  000e8490: ldr r2,[r0,#0x0]
  000e8492: blx 0x0007159c
  000e8496: ldr r0,[sp,#0x88]
  000e8498: ldr.w r1,[r11,#0x0]
  000e849c: subs r0,r1,r0
  000e849e: itttt eq
  000e84a0: mov.eq r0,r4
  000e84a2: add.eq sp,#0x8c
  000e84a4: pop.eq.w {r8,r9,r10,r11}
  000e84a8: pop.eq.w {r4,r5,r6,r7,lr}
  000e84ac: it eq
  000e84ae: b.eq.w 0x001ac138
  000e84b2: blx 0x0006e824
  000e84b6: ldr r0,[r5,#0x0]
  000e84b8: ldr r1,[0x000e855c]
  000e84ba: blx 0x0007171c
  000e84be: ldr r0,[0x000e8560]
  000e84c0: movs r6,#0x0
  000e84c2: strb.w r6,[r4,#0x63]
  000e84c6: movs r1,#0x46
  000e84c8: add r0,pc
  000e84ca: strb.w r6,[r4,#0x6d]
  000e84ce: ldr r5,[r5,#0x0]
  000e84d0: ldr r0,[r0,#0x0]
  000e84d2: ldr r0,[r0,#0x0]
  000e84d4: blx 0x00071c44
  000e84d8: mov r1,r0
  000e84da: mov r0,r5
  000e84dc: blx 0x0007366c
  000e84e0: ldr r0,[0x000e8564]
  000e84e2: ldr r1,[0x000e8568]
  000e84e4: add r0,pc
  000e84e6: add r1,pc
  000e84e8: ldr r0,[r0,#0x0]
  000e84ea: ldr r1,[r1,#0x0]
  000e84ec: ldr r0,[r0,#0x0]
  000e84ee: str r6,[r1,#0x0]
  000e84f0: movs r1,#0x5
  000e84f2: blx 0x00071d64
  000e84f6: ldrb r0,[r4,#0x18]
  000e84f8: cbz r0,0x000e8508
  000e84fa: ldrb.w r0,[r4,#0x66]
  000e84fe: cbz r0,0x000e8508
  000e8500: movs r0,#0x0
  000e8502: strb r0,[r4,#0x18]
  000e8504: strb.w r0,[r4,#0x66]
  000e8508: movs r0,#0x0
  000e850a: strb.w r0,[r4,#0x63]
  000e850e: b.w 0x000e763a
  000e856c: ldr r0,[r6,#0x0]
  000e856e: blx 0x00071c14
  000e8572: mov r9,r0
  000e8574: ldr r0,[0x000e8928]
  000e8576: movs r1,#0x0
  000e8578: add r0,pc
  000e857a: ldr r0,[r0,#0x0]
  000e857c: ldr r0,[r0,#0x0]
  000e857e: ldr r0,[r0,#0x4]
  000e8580: ldr.w r0,[r0,#0x94]
  000e8584: blx 0x000719d4
  000e8588: mov r1,r0
  000e858a: mov r0,r9
  000e858c: blx 0x00075004
  000e8590: b.w 0x000e7b7a
  000e8594: blx 0x00071c14
  000e8598: blx 0x00071824
  000e859c: cmp.w r9,#0x54
  000e85a0: it eq
  000e85a2: cmp.eq r0,#0x64
  000e85a4: beq 0x000e868e
  000e85a6: cmp.w r9,#0xa0
  000e85aa: beq.w 0x000e87ba
  000e85ae: cmp.w r9,#0x63
  000e85b2: beq.w 0x000e8748
  000e85b6: cmp.w r9,#0x68
  000e85ba: beq.w 0x000e87f8
  000e85be: cmp.w r9,#0x6d
  000e85c2: beq.w 0x000e8766
  000e85c6: cmp.w r9,#0x77
  000e85ca: beq.w 0x000e8748
  000e85ce: cmp.w r9,#0x80
  000e85d2: beq.w 0x000e8810
  000e85d6: cmp.w r9,#0x85
  000e85da: beq.w 0x000e8830
  000e85de: cmp.w r9,#0x90
  000e85e2: beq.w 0x000e8784
  000e85e6: cmp.w r9,#0x59
  000e85ea: bne.w 0x000e7b7a
  000e85ee: ldr r0,[0x000e892c]
  000e85f0: movs r3,#0x1
  000e85f2: ldr r1,[0x000e8930]
  000e85f4: add r0,pc
  000e85f6: ldr r2,[0x000e8934]
  000e85f8: add r1,pc
  000e85fa: ldr r5,[r6,#0x0]
  000e85fc: ldr r0,[r0,#0x0]
  000e85fe: add r2,pc
  000e8600: ldr r1,[r1,#0x0]
  000e8602: ldr r2,[r2,#0x0]
  000e8604: ldr r0,[r0,#0x0]
  000e8606: strb r3,[r1,#0x0]
  000e8608: movs r1,#0x6d
  000e860a: b 0x000e884c
  000e860c: ldrb.w r0,[r4,#0x62]
  000e8610: cmp r0,#0x0
  000e8612: beq 0x000e870c
  000e8614: ldr r0,[r4,#0x50]
  000e8616: mov r1,r10
  000e8618: mov r2,r8
  000e861a: mov r3,r5
  000e861c: blx 0x00075238
  000e8620: cmp r0,#0x0
  000e8622: beq.w 0x000e763a
  000e8626: movs r6,#0x0
  000e8628: add.w r8,sp,#0x7c
  000e862c: strb.w r6,[r4,#0x62]
  000e8630: ldr r0,[0x000e8938]
  000e8632: add r0,pc
  000e8634: ldr.w r9,[r0,#0x0]
  000e8638: ldr r0,[0x000e893c]
  000e863a: add r0,pc
  000e863c: ldr r5,[r0,#0x0]
  000e863e: b 0x000e8682
  000e8640: cmp r6,#0x9
  000e8642: bhi 0x000e8680
  000e8644: ldr r0,[r0,#0x4]
  000e8646: ldr.w r1,[r0,r6,lsl #0x2]
  000e864a: mov r0,r8
  000e864c: blx 0x00074dd0
  000e8650: vldr.32 s0,[sp,#0x7c]
  000e8654: vcvt.s32.f32 s0,s0
  000e8658: vmov r0,s0
  000e865c: str.w r0,[r9,r6,lsl #0x2]
  000e8660: ldr.w r0,[r4,#0x8c]
  000e8664: ldr r0,[r0,#0x4]
  000e8666: ldr.w r1,[r0,r6,lsl #0x2]
  000e866a: mov r0,r8
  000e866c: blx 0x00074dd0
  000e8670: vldr.32 s0,[sp,#0x80]
  000e8674: vcvt.s32.f32 s0,s0
  000e8678: vmov r0,s0
  000e867c: str.w r0,[r5,r6,lsl #0x2]
  000e8680: adds r6,#0x1
  000e8682: ldr.w r0,[r4,#0x8c]
  000e8686: ldr r1,[r0,#0x0]
  000e8688: cmp r6,r1
  000e868a: bcc 0x000e8640
  000e868c: b 0x000e8304
  000e868e: ldr r0,[r6,#0x0]
  000e8690: blx 0x00071c14
  000e8694: movs r1,#0x26
  000e8696: mov r9,r0
  000e8698: blx 0x00071af4
  000e869c: cbnz r0,0x000e86bc
  000e869e: ldr r0,[0x000e8940]
  000e86a0: mov.w r1,#0xffffffff
  000e86a4: add r0,pc
  000e86a6: ldr r0,[r0,#0x0]
  000e86a8: ldr r0,[r0,#0x0]
  000e86aa: ldr r0,[r0,#0x4]
  000e86ac: ldr.w r0,[r0,#0x98]
  000e86b0: blx 0x000719d4
  000e86b4: mov r1,r0
  000e86b6: mov r0,r9
  000e86b8: blx 0x00075004
  000e86bc: mov r0,r9
  000e86be: movs r1,#0x28
  000e86c0: blx 0x00071af4
  000e86c4: cbnz r0,0x000e86e4
  000e86c6: ldr r0,[0x000e8944]
  000e86c8: mov.w r1,#0xffffffff
  000e86cc: add r0,pc
  000e86ce: ldr r0,[r0,#0x0]
  000e86d0: ldr r0,[r0,#0x0]
  000e86d2: ldr r0,[r0,#0x4]
  000e86d4: ldr.w r0,[r0,#0xa0]
  000e86d8: blx 0x000719d4
  000e86dc: mov r1,r0
  000e86de: mov r0,r9
  000e86e0: blx 0x00075004
  000e86e4: ldr r0,[r6,#0x0]
  000e86e6: blx 0x00071a58
  000e86ea: mov r9,r0
  000e86ec: ldr r0,[0x000e8948]
  000e86ee: movs r1,#0x1
  000e86f0: add r0,pc
  000e86f2: ldr r0,[r0,#0x0]
  000e86f4: ldr r0,[r0,#0x0]
  000e86f6: ldr r0,[r0,#0x4]
  000e86f8: ldr.w r0,[r0,#0x224]
  000e86fc: blx 0x000718c0
  000e8700: mov r1,r0
  000e8702: mov r0,r9
  000e8704: blx 0x00072cdc
  000e8708: b.w 0x000e7b7a
  000e870c: ldrb.w r0,[r4,#0x61]
  000e8710: cmp r0,#0x0
  000e8712: beq.w 0x000e763a
  000e8716: ldr.w r0,[r4,#0x90]
  000e871a: mov r1,r10
  000e871c: mov r2,r8
  000e871e: blx 0x00074788
  000e8722: cmp r0,#0x0
  000e8724: beq.w 0x000e8866
  000e8728: ldr r0,[sp,#0x88]
  000e872a: ldr.w r1,[r11,#0x0]
  000e872e: subs r0,r1,r0
  000e8730: itttt eq
  000e8732: mov.eq r0,r4
  000e8734: add.eq sp,#0x8c
  000e8736: pop.eq.w {r8,r9,r10,r11}
  000e873a: pop.eq.w {r4,r5,r6,r7,lr}
  000e873e: it eq
  000e8740: b.eq.w 0x001ac148
  000e8744: blx 0x0006e824
  000e8748: ldr r0,[0x000e8ad0]
  000e874a: movs r3,#0x1
  000e874c: ldr r1,[0x000e8ad4]
  000e874e: ldr r2,[0x000e8ad8]
  000e8750: add r0,pc
  000e8752: add r1,pc
  000e8754: ldr r5,[r6,#0x0]
  000e8756: add r2,pc
  000e8758: ldr r0,[r0,#0x0]
  000e875a: ldr r1,[r1,#0x0]
  000e875c: ldr r2,[r2,#0x0]
  000e875e: ldr r0,[r0,#0x0]
  000e8760: strb r3,[r1,#0x0]
  000e8762: movs r1,#0xa
  000e8764: b 0x000e884c
  000e8766: ldr r0,[0x000e8ac4]
  000e8768: movs r3,#0x1
  000e876a: ldr r1,[0x000e8ac8]
  000e876c: ldr r2,[0x000e8acc]
  000e876e: add r0,pc
  000e8770: add r1,pc
  000e8772: ldr r5,[r6,#0x0]
  000e8774: add r2,pc
  000e8776: ldr r0,[r0,#0x0]
  000e8778: ldr r1,[r1,#0x0]
  000e877a: ldr r2,[r2,#0x0]
  000e877c: ldr r0,[r0,#0x0]
  000e877e: strb r3,[r1,#0x0]
  000e8780: movs r1,#0x72
  000e8782: b 0x000e884c
  000e8784: ldr r0,[0x000e8aec]
  000e8786: mov.w r8,#0x0
  000e878a: ldr r1,[0x000e8af0]
  000e878c: ldr r2,[0x000e8af4]
  000e878e: add r0,pc
  000e8790: add r1,pc
  000e8792: ldr r5,[r6,#0x0]
  000e8794: add r2,pc
  000e8796: ldr r0,[r0,#0x0]
  000e8798: ldr r1,[r1,#0x0]
  000e879a: ldr r2,[r2,#0x0]
  000e879c: strb.w r8,[r0,#0x0]
  000e87a0: ldr r0,[r2,#0x0]
  000e87a2: movs r2,#0x1
  000e87a4: str r2,[r1,#0x0]
  000e87a6: movs r1,#0x70
  000e87a8: blx 0x00071c44
  000e87ac: mov r1,r0
  000e87ae: mov r0,r5
  000e87b0: blx 0x000745fc
  000e87b4: ldr r0,[r4,#0x8]
  000e87b6: b.w 0x000e7780
  000e87ba: ldr r0,[0x000e8af8]
  000e87bc: movs r3,#0x0
  000e87be: ldr r1,[0x000e8afc]
  000e87c0: ldr r2,[0x000e8b00]
  000e87c2: add r0,pc
  000e87c4: add r1,pc
  000e87c6: ldr r5,[r6,#0x0]
  000e87c8: add r2,pc
  000e87ca: ldr r0,[r0,#0x0]
  000e87cc: ldr r1,[r1,#0x0]
  000e87ce: ldr r2,[r2,#0x0]
  000e87d0: strb r3,[r0,#0x0]
  000e87d2: ldr r0,[r2,#0x0]
  000e87d4: movs r2,#0x1
  000e87d6: str r2,[r1,#0x0]
  000e87d8: movs r1,#0xa
  000e87da: blx 0x00071c44
  000e87de: mov r1,r0
  000e87e0: mov r0,r5
  000e87e2: blx 0x000745fc
  000e87e6: ldr r0,[r4,#0x8]
  000e87e8: ldr r1,[sp,#0x88]
  000e87ea: ldr.w r2,[r11,#0x0]
  000e87ee: subs r1,r2,r1
  000e87f0: bne.w 0x000e764c
  000e87f4: movs r1,#0x2
  000e87f6: b 0x000e8260
  000e87f8: ldr r0,[r4,#0x78]
  000e87fa: cbz r0,0x000e8804
  000e87fc: blx 0x00075214
  000e8800: blx 0x0006eb48
  000e8804: movs r0,#0x0
  000e8806: strb.w r0,[r4,#0x66]
  000e880a: str r0,[r4,#0x78]
  000e880c: b.w 0x000e7b7a
  000e8810: ldr r0,[r6,#0x0]
  000e8812: blx 0x00075028
  000e8816: ldr r0,[0x000e8adc]
  000e8818: add r0,pc
  000e881a: ldr r0,[r0,#0x0]
  000e881c: ldrb.w r0,[r0,#0x2a]
  000e8820: cmp r0,#0x0
  000e8822: bne.w 0x000e7b7a
  000e8826: movs r0,#0x1
  000e8828: strb.w r0,[r4,#0x6e]
  000e882c: b.w 0x000e7b7a
  000e8830: ldr r0,[0x000e8ae0]
  000e8832: movs r3,#0x1
  000e8834: ldr r1,[0x000e8ae4]
  000e8836: ldr r2,[0x000e8ae8]
  000e8838: add r0,pc
  000e883a: add r1,pc
  000e883c: ldr r5,[r6,#0x0]
  000e883e: add r2,pc
  000e8840: ldr r0,[r0,#0x0]
  000e8842: ldr r1,[r1,#0x0]
  000e8844: ldr r2,[r2,#0x0]
  000e8846: ldr r0,[r0,#0x0]
  000e8848: strb r3,[r1,#0x0]
  000e884a: movs r1,#0x78
  000e884c: str r3,[r2,#0x0]
  000e884e: blx 0x00071c44
  000e8852: mov r1,r0
  000e8854: mov r0,r5
  000e8856: blx 0x000745fc
  000e885a: ldr r0,[0x000e8ac0]
  000e885c: add r0,pc
  000e885e: ldr r0,[r0,#0x0]
  000e8860: ldr r0,[r0,#0x0]
  000e8862: b.w 0x000e7fa8
  000e8866: ldr.w r0,[r4,#0x94]
  000e886a: mov r1,r10
  000e886c: mov r2,r8
  000e886e: blx 0x00074788
  000e8872: cbz r0,0x000e88b6
  000e8874: ldr r0,[0x000e8b04]
  000e8876: mov.w r9,#0x1
  000e887a: ldr r1,[0x000e8b08]
  000e887c: add r0,pc
  000e887e: add r1,pc
  000e8880: ldr r0,[r0,#0x0]
  000e8882: ldr r1,[r1,#0x0]
  000e8884: ldr r0,[r0,#0x0]
  000e8886: strb.w r9,[r1,#0x4e]
  000e888a: blx 0x00071788
  000e888e: ldr r5,[r4,#0x78]
  000e8890: cbnz r5,0x000e88a2
  000e8892: mov.w r0,#0x134
  000e8896: blx 0x0006eb24
  000e889a: mov r5,r0
  000e889c: blx 0x00075250
  000e88a0: str r5,[r4,#0x78]
  000e88a2: mov r0,r5
  000e88a4: blx 0x0007513c
  000e88a8: ldr r0,[r4,#0x78]
  000e88aa: strb.w r9,[r4,#0x18]
  000e88ae: strb.w r9,[r4,#0x66]
  000e88b2: blx 0x0007525c
  000e88b6: mov.w r0,#0xffffffff
  000e88ba: movs r5,#0x0
  000e88bc: str r0,[r4,#0x44]
  000e88be: b 0x000e88d8
  000e88c0: ldr.w r0,[r4,#0x8c]
  000e88c4: mov r1,r10
  000e88c6: mov r2,r8
  000e88c8: ldr r0,[r0,#0x4]
  000e88ca: ldr.w r0,[r0,r5,lsl #0x2]
  000e88ce: blx 0x00074788
  000e88d2: cmp r0,#0x0
  000e88d4: bne 0x000e899a
  000e88d6: adds r5,#0x1
  000e88d8: cmp r5,#0x4
  000e88da: bls 0x000e88c0
  000e88dc: ldr r0,[r6,#0x0]
  000e88de: mov r1,r10
  000e88e0: mov r2,r8
  000e88e2: blx 0x0007477c
  000e88e6: cmp r0,#0x0
  000e88e8: beq 0x000e89aa
  000e88ea: ldr r0,[r4,#0x50]
  000e88ec: cbnz r0,0x000e8900
  000e88ee: mov.w r0,#0x240
  000e88f2: blx 0x0006eb24
  000e88f6: mov r5,r0
  000e88f8: movs r1,#0x2
  000e88fa: blx 0x00075268
  000e88fe: str r5,[r4,#0x50]
  000e8900: ldr r0,[0x000e8b0c]
  000e8902: add r0,pc
  000e8904: ldr r0,[r0,#0x0]
  000e8906: ldr r0,[r0,#0x0]
  000e8908: blx 0x00075274
  000e890c: movs r0,#0x1
  000e890e: add.w r8,sp,#0x7c
  000e8912: strb.w r0,[r4,#0x62]
  000e8916: movs r6,#0x0
  000e8918: ldr r0,[0x000e8b10]
  000e891a: add r0,pc
  000e891c: ldr.w r9,[r0,#0x0]
  000e8920: ldr r0,[0x000e8b14]
  000e8922: add r0,pc
  000e8924: ldr r5,[r0,#0x0]
  000e8926: b 0x000e898e
  000e894c: cmp r6,#0x9
  000e894e: bhi 0x000e898c
  000e8950: ldr r0,[r0,#0x4]
  000e8952: ldr.w r1,[r0,r6,lsl #0x2]
  000e8956: mov r0,r8
  000e8958: blx 0x00074dd0
  000e895c: vldr.32 s0,[sp,#0x7c]
  000e8960: vcvt.s32.f32 s0,s0
  000e8964: vmov r0,s0
  000e8968: str.w r0,[r9,r6,lsl #0x2]
  000e896c: ldr r0,[r4,#0x50]
  000e896e: ldr r0,[r0,#0x4]
  000e8970: ldr r0,[r0,#0x4]
  000e8972: ldr.w r1,[r0,r6,lsl #0x2]
  000e8976: mov r0,r8
  000e8978: blx 0x00074dd0
  000e897c: vldr.32 s0,[sp,#0x80]
  000e8980: vcvt.s32.f32 s0,s0
  000e8984: vmov r0,s0
  000e8988: str.w r0,[r5,r6,lsl #0x2]
  000e898c: adds r6,#0x1
  000e898e: ldr r0,[r4,#0x50]
  000e8990: ldr r0,[r0,#0x4]
  000e8992: ldr r1,[r0,#0x0]
  000e8994: cmp r6,r1
  000e8996: bcc 0x000e894c
  000e8998: b 0x000e8304
  000e899a: ldr r0,[r4,#0x0]
  000e899c: str r5,[r4,#0x44]
  000e899e: ldr r1,[r0,#0x10]
  000e89a0: movs r0,#0x0
  000e89a2: strd r0,r0,[sp,#0x0]
  000e89a6: b.w 0x000e81ec
  000e89aa: ldr r0,[r6,#0x0]
  000e89ac: blx 0x000747b8
  000e89b0: cbz r0,0x000e89e0
  000e89b2: ldr r0,[0x000e8b18]
  000e89b4: mov.w r1,#0x284
  000e89b8: ldr r5,[r6,#0x0]
  000e89ba: add r0,pc
  000e89bc: ldr r0,[r0,#0x0]
  000e89be: ldr r0,[r0,#0x0]
  000e89c0: blx 0x00072f70
  000e89c4: add.w r9,sp,#0x10
  000e89c8: mov r1,r0
  000e89ca: movs r2,#0x0
  000e89cc: mov r0,r9
  000e89ce: blx 0x0006f028
  000e89d2: mov r0,r5
  000e89d4: mov r1,r9
  000e89d6: blx 0x000747c4
  000e89da: add r0,sp,#0x10
  000e89dc: blx 0x0006ee30
  000e89e0: ldr r0,[r4,#0x1c]
  000e89e2: mov r1,r10
  000e89e4: mov r2,r8
  000e89e6: blx 0x00075280
  000e89ea: cmp r0,#0x0
  000e89ec: bne.w 0x000e763a
  000e89f0: ldr.w r1,[r4,#0xf0]
  000e89f4: ldr.w r0,[r4,#0xe4]
  000e89f8: vldr.32 s0,[pc,#0xbc]
  000e89fc: cmp r1,#0x0
  000e89fe: vmov s2,r1
  000e8a02: mov r2,r1
  000e8a04: add r0,r1
  000e8a06: vcvt.f32.s32 s2,s2
  000e8a0a: it mi
  000e8a0c: rsb.mi r2,r1
  000e8a0e: cmp r2,#0x3
  000e8a10: ldr r2,[0x000e8abc]
  000e8a12: it gt
  000e8a14: vmov.gt.f32 s0,s2
  000e8a18: str.w r2,[r4,#0xf4]
  000e8a1c: movs r2,#0x0
  000e8a1e: strb.w r2,[r4,#0x100]
  000e8a22: str.w r0,[r4,#0xe4]
  000e8a26: str.w r0,[r4,#0xec]
  000e8a2a: vstr.32 s0,[r4,#0xf8]
  000e8a2e: b.w 0x000e763a
  001ab808: bx pc
  001ac11c: ldr r12,[0x1ac124]
  001ac120: add pc,r12,pc
  001ac128: bx pc
  001ac138: bx pc
  001ac14c: ldr r12,[0x1ac154]
  001ac150: add pc,r12,pc
```
