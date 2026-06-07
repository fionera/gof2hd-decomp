# HangarWindow::OnTouchEnd
elf 0x14c740 body 4970
Sig: undefined __thiscall OnTouchEnd(HangarWindow * this, int param_1, int param_2)

## decompile
```c

/* HangarWindow::OnTouchEnd(int, int) */

void __thiscall HangarWindow::OnTouchEnd(HangarWindow *this,int param_1,int param_2)

{
  HangarWindow HVar1;
  byte bVar2;
  undefined1 *puVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  String *pSVar7;
  RecordHandler *pRVar8;
  Station *this_00;
  undefined4 uVar9;
  ListItem *pLVar10;
  String *this_01;
  int iVar11;
  HangarList *pHVar12;
  Ship *pSVar13;
  Array *pAVar14;
  String *pSVar15;
  String *pSVar16;
  uint *puVar17;
  Item *pIVar18;
  undefined4 *puVar19;
  ModStation *this_02;
  void *pvVar20;
  Standing *this_03;
  float fVar21;
  Mission *this_04;
  Vector *pVVar22;
  __fn *extraout_r1;
  __fn *extraout_r1_00;
  __fn *extraout_r1_01;
  __fn *__fn;
  int extraout_r1_02;
  int extraout_r1_03;
  void *extraout_r2;
  void *extraout_r2_00;
  void *extraout_r2_01;
  void *extraout_r2_02;
  int extraout_r3;
  int extraout_r3_00;
  int extraout_r3_01;
  int extraout_r3_02;
  code *pcVar23;
  uint uVar24;
  uint uVar25;
  int *piVar26;
  uint *puVar27;
  ScrollTouchWindow *pSVar28;
  HangarWindow *pHVar29;
  Ship *pSVar30;
  String *pSVar31;
  ChoiceWindow *pCVar32;
  Agent *this_05;
  Array *pAVar33;
  bool bVar34;
  uint in_fpscr;
  undefined4 uVar35;
  float fVar36;
  float extraout_s0;
  undefined8 uVar37;
  void *in_stack_fffffe88;
  Generator *in_stack_fffffe94;
  undefined4 *in_stack_fffffe98;
  int *piStack_164;
  Agent *local_160;
  Ship *local_15c;
  undefined4 *local_158;
  Ship *local_154;
  int *local_150;
  Agent *local_14c;
  String aSStack_148 [12];
  String aSStack_13c [12];
  String aSStack_130 [12];
  String aSStack_124 [12];
  String aSStack_118 [12];
  String aSStack_10c [12];
  String aSStack_100 [12];
  String aSStack_f4 [12];
  String aSStack_e8 [12];
  String aSStack_dc [12];
  String aSStack_d0 [12];
  String aSStack_c4 [12];
  String aSStack_b8 [12];
  String aSStack_ac [12];
  String aSStack_a0 [12];
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  int aiStack_34 [3];
  int local_28;
  
  this_05 = *(Agent **)(DAT_0015cae4 + 0x15c756);
  local_28 = *(int *)this_05;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  this[0xd0] = (HangarWindow)0x0;
  if (this[0xd1] != (HangarWindow)0x0) {
    this[0xd1] = (HangarWindow)0x0;
    goto LAB_0015c774;
  }
  bVar4 = SUB41(this,0);
  if (this[0x3c] == (HangarWindow)0x0) {
    if (*(int *)(this + 0x58) == 1) {
      ListItemWindow::OnTouchEnd(*(int *)(this + 0x18),param_1);
    }
    piVar26 = *(int **)(DAT_0015cb00 + 0x15c8b6);
    iVar5 = Layout::OnTouchEnd((Layout *)*piVar26,param_1,param_2);
    if (iVar5 == 0) {
      iVar5 = *(int *)(this + 0xc0);
      uVar24 = 0;
      iVar6 = *(int *)(this + 0xb4) + iVar5;
      uVar9 = VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
      if (iVar5 < 0) {
        iVar5 = -iVar5;
      }
      uVar35 = DAT_0015cb04;
      if (3 < iVar5) {
        uVar35 = uVar9;
      }
      *(undefined4 *)(this + 0xc4) = DAT_0015cb08;
      *(int *)(this + 0xb4) = iVar6;
      *(int *)(this + 0xbc) = iVar6;
      *(undefined4 *)(this + 200) = uVar35;
      pcVar23 = *(code **)(DAT_0015cb0c + 0x15c97e);
      local_150 = piVar26;
      for (; uVar24 < **(uint **)(this + 4); uVar24 = uVar24 + 1) {
        iVar5 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(this + 4))[1] + uVar24 * 4),param_1);
        if (iVar5 != 0) {
          (*pcVar23)(this,0);
          (*pcVar23)(this,1);
          (*pcVar23)(this,0);
          *(undefined4 *)(this + 0x68) = 0;
          HangarList::setCurrentTab(*(int *)(this + 0x14),SUB41(uVar24,0));
          if (uVar24 == 2) {
            refreshCargoAvailabilityForBlueprints(this);
          }
          refreshCurrentContentHeight(this);
          *(undefined4 *)(this + 0xb4) = 0;
          *(undefined4 *)(this + 0xbc) = 0;
          HangarList::setCurrentItemIndex(*(HangarList **)(this + 0x14),-1);
        }
      }
      iVar5 = *local_150;
      piVar26 = local_150;
      if (*(int *)(iVar5 + 0xc) < param_2) {
        pHVar12 = *(HangarList **)(this + 0x14);
        iVar5 = __aeabi_idiv((((param_2 - *(int *)(iVar5 + 0xc)) - *(int *)(iVar5 + 0x20)) -
                             *(int *)(this + 0x10c)) - *(int *)(this + 0xb4),
                             *(int *)(iVar5 + 0x70) + *(int *)(this + 0x10c));
        piVar26 = local_150;
        iVar6 = HangarList::getCurrentLength(pHVar12);
        if (iVar5 < iVar6) {
          HangarList::setCurrentItemIndex(*(HangarList **)(this + 0x14),iVar5);
          iVar5 = currentItemIsHighlighted(this);
          if ((iVar5 != 0) && (this[0xd2] != (HangarWindow)0x0)) {
            setSellMode(bVar4);
            setSellMode(bVar4);
          }
        }
      }
      if (this[0xd2] != (HangarWindow)0x0) {
        this[0xd2] = (HangarWindow)0x0;
        goto LAB_0015c774;
      }
      local_14c = this_05;
      iVar5 = TouchButton::OnTouchEnd(*(int *)(*(int *)(*(int *)(this + 0x24) + 4) + 0x5c),param_1);
      if (iVar5 != 0) {
        BluePrint::getAutoCompletionPrice(*(BluePrint **)(this + 0x80));
        pCVar32 = *(ChoiceWindow **)(this + 0x20);
        uVar9 = **(undefined4 **)(DAT_0015ce70 + 0x15cb42);
        pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015ce74 + 0x15cb48));
        AbyssEngine::String::String(aSStack_e8,pSVar7,false);
        Layout::formatCredits((int)aSStack_f4);
        AbyssEngine::String::String(aSStack_100,(char *)(DAT_0015ce78 + 0x15cb70),false);
        Status::replaceHash(aiStack_34,uVar9,aSStack_e8,aSStack_f4);
        ChoiceWindow::set(pCVar32,(String *)aiStack_34,true);
        pcVar23 = *(code **)(DAT_0015ce7c + 0x15cb96);
        (*pcVar23)(aiStack_34);
        (*pcVar23)(aSStack_100);
        (*pcVar23)(aSStack_f4);
        (*pcVar23)(aSStack_e8);
        this[0xb1] = (HangarWindow)0x1;
        this[0x3c] = (HangarWindow)0x1;
        piVar26 = local_150;
      }
      iVar5 = currentItemIsHighlighted(this);
      if (iVar5 != 0) {
        local_150 = *(int **)(DAT_0015ce80 + 0x15cbc6);
        local_154 = *(Ship **)(DAT_0015ce84 + 0x15cbce);
        local_158 = *(undefined4 **)(DAT_0015ce88 + 0x15cbd6);
        for (uVar24 = 0; uVar24 < **(uint **)(this + 0x24); uVar24 = uVar24 + 1) {
          iVar5 = TouchButton::OnTouchEnd
                            (*(int *)((*(uint **)(this + 0x24))[1] + uVar24 * 4),param_1);
          if (iVar5 != 0) {
            iVar5 = *piVar26;
            uVar9 = __aeabi_idiv((((param_2 - *(int *)(iVar5 + 0xc)) - *(int *)(iVar5 + 0x20)) -
                                 *(int *)(this + 0x10c)) - *(int *)(this + 0xb4),
                                 *(int *)(iVar5 + 0x70) + *(int *)(this + 0x10c));
            if ((uVar24 & 0x7fffffff) < 0xc) {
                    /* WARNING: Could not recover jumptable at 0x0015cc1e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
              (*(code *)(&DAT_0015cc22 + (ushort)(&DAT_0015cc22)[uVar24]))
                        (uVar24 & 0x7fffffff,uVar9);
              return;
            }
          }
        }
      }
      iVar5 = Layout::helpPressed((Layout *)*piVar26);
      this_05 = local_14c;
      if (iVar5 != 0) {
        if (*(int *)(this + 0x58) == 1) {
          iVar5 = *piVar26;
          pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015ce90 + 0x15cdae));
          AbyssEngine::String::String(aSStack_10c,pSVar7,false);
          Layout::initHelpWindow(iVar5,aSStack_10c);
          this_01 = aSStack_10c;
        }
        else {
          uVar9 = HangarList::getCurrentTab(*(HangarList **)(this + 0x14));
          switch(uVar9) {
          case 0:
            iVar5 = *piVar26;
            pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015d21c + 0x15cf8e));
            AbyssEngine::String::String(aSStack_118,pSVar7,false);
            Layout::initHelpWindow(iVar5,aSStack_118);
            this_01 = aSStack_118;
            break;
          case 1:
            iVar5 = *piVar26;
            pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015d618 + 0x15d34c));
            AbyssEngine::String::String(aSStack_124,pSVar7,false);
            Layout::initHelpWindow(iVar5,aSStack_124);
            this_01 = aSStack_124;
            break;
          case 2:
            iVar5 = *piVar26;
            pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015d610 + 0x15d2f4));
            AbyssEngine::String::String(aSStack_130,pSVar7,false);
            Layout::initHelpWindow(iVar5,aSStack_130);
            this_01 = aSStack_130;
            break;
          case 3:
            iVar5 = *piVar26;
            pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015d614 + 0x15d320));
            AbyssEngine::String::String(aSStack_13c,pSVar7,false);
            Layout::initHelpWindow(iVar5,aSStack_13c);
            this_01 = aSStack_13c;
            break;
          case 4:
            iVar5 = *piVar26;
            pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015d60c + 0x15d2c8));
            AbyssEngine::String::String(aSStack_148,pSVar7,false);
            Layout::initHelpWindow(iVar5,aSStack_148);
            this_01 = aSStack_148;
            break;
          default:
            goto switchD_0015cf74_default;
          }
        }
        AbyssEngine::String::~String(this_01);
        this_05 = local_14c;
      }
switchD_0015cf74_default:
      iVar5 = TouchButton::OnTouchEnd(*(int *)(*(int *)(*(int *)(this + 0x24) + 4) + 0x2c),param_1);
      if (iVar5 != 0) {
        pRVar8 = (RecordHandler *)**(undefined4 **)(DAT_0015ce94 + 0x15cdf4);
        *(undefined1 *)(*(int *)(DAT_0015ce98 + 0x15cdf6) + 0x4e) = 1;
        RecordHandler::saveOptions(pRVar8);
        showCreditsBuyWindow(this);
      }
      goto LAB_0015c774;
    }
    if (*(int *)(this + 0x58) == 1) {
      Layout::resetWindowDimensions((Layout *)*piVar26);
      *(undefined4 *)(this + 0x58) = 0;
      goto LAB_0015c774;
    }
    iVar5 = HangarList::getCurrentTab(*(HangarList **)(this + 0x14));
    if (iVar5 != 4) {
      iVar5 = HangarList::getCurrentTab(*(HangarList **)(this + 0x14));
      if (iVar5 == 3) {
        HangarList::setCurrentTab(*(int *)(this + 0x14),false);
        refreshCargoAvailabilityForBlueprints(this);
        refreshCurrentContentHeight(this);
      }
      else {
        iVar5 = readyToClose(this);
        if (iVar5 != 0) {
          setSellMode(bVar4);
          *(undefined4 *)(this + 0x68) = 0;
          HangarList::setCurrentItemIndex(*(HangarList **)(this + 0x14),-1);
        }
      }
      goto LAB_0015c774;
    }
    setSellMode(bVar4);
    *(undefined4 *)(this + 0x68) = 0;
    HangarList::setCurrentTab(*(int *)(this + 0x14),true);
    refreshCargoAvailabilityForBlueprints(this);
    refreshCurrentContentHeight(this);
    pHVar12 = *(HangarList **)(this + 0x14);
    *(undefined4 *)(this + 0xb4) = 0;
    *(undefined4 *)(this + 0xbc) = 0;
LAB_0015d16a:
    HangarList::setCurrentItemIndex(pHVar12,-1);
  }
  else {
    if (this[0xb1] != (HangarWindow)0x0) {
      iVar5 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
      if (iVar5 == 1) {
        this[0x3c] = (HangarWindow)0x0;
      }
      else if (iVar5 == 0) {
        iVar5 = BluePrint::getAutoCompletionPrice(*(BluePrint **)(this + 0x80));
        piVar26 = *(int **)(DAT_0015cae8 + 0x15c7c8);
        iVar6 = Status::getCredits();
        if (iVar6 < iVar5) {
          iVar5 = *piVar26;
          piVar26 = *(int **)(DAT_0015caec + 0x15c7e2);
          pSVar7 = (String *)GameText::getText(*piVar26);
          AbyssEngine::String::String(aSStack_40,pSVar7,false);
          Status::getCredits();
          Layout::formatCredits((int)aSStack_58);
          AbyssEngine::String::String(aSStack_4c,aSStack_58,false);
          AbyssEngine::String::String(aSStack_64,(char *)(DAT_0015caf4 + 0x15c81e),false);
          Status::replaceHash(aiStack_34,iVar5,aSStack_40,aSStack_4c);
          pcVar23 = *(code **)(DAT_0015caf8 + 0x15c838);
          (*pcVar23)(aSStack_64);
          (*pcVar23)(aSStack_4c);
          (*pcVar23)(aSStack_58);
          (*pcVar23)(aSStack_40);
          AbyssEngine::String::String(aSStack_70,(char *)(DAT_0015cafc + 0x15c84e),false);
          GameText::getText(*piVar26);
          AbyssEngine::operator+(aSStack_58,aSStack_70);
          AbyssEngine::String::operator+=((String *)aiStack_34,aSStack_58);
          AbyssEngine::String::~String(aSStack_58);
          AbyssEngine::String::~String(aSStack_70);
          ChoiceWindow::set(*(ChoiceWindow **)(this + 0x20),(String *)aiStack_34,true);
          this[0xaf] = (HangarWindow)0x1;
          AbyssEngine::String::~String((String *)aiStack_34);
        }
        else {
          this[0x3c] = (HangarWindow)0x0;
          if (*(BluePrint **)(this + 0x80) != (BluePrint *)0x0) {
            iVar5 = BluePrint::isEmpty(*(BluePrint **)(this + 0x80));
            if (iVar5 != 0) {
              this_00 = (Station *)Status::getStation();
              uVar9 = Station::getIndex(this_00);
              *(undefined4 *)(*(int *)(this + 0x80) + 0x10) = uVar9;
              Status::getStation();
              Station::getName();
              AbyssEngine::String::operator=
                        ((String *)(*(int *)(this + 0x80) + 0x14),(String *)aiStack_34);
              AbyssEngine::String::~String((String *)aiStack_34);
            }
            BluePrint::complete(*(BluePrint **)(this + 0x80));
            pLVar10 = (ListItem *)HangarList::getCurrentItemAt(*(HangarList **)(this + 0x14),1);
            highlightItem(this,pLVar10);
            this[0x88] = (HangarWindow)0x1;
            setSellMode(bVar4);
            Status::changeCredits(*piVar26);
          }
        }
      }
      this[0xb1] = (HangarWindow)0x0;
      goto LAB_0015c774;
    }
    if (this[0x11c] == (HangarWindow)0x0) {
      if (this[0xaf] != (HangarWindow)0x0) {
        local_14c = this_05;
        iVar5 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
        if (iVar5 == 1) {
          this[0xaf] = (HangarWindow)0x0;
LAB_0015cf60:
          this[0x3c] = (HangarWindow)0x0;
        }
        else if (iVar5 == 0) {
          pRVar8 = (RecordHandler *)**(undefined4 **)(DAT_0015cb10 + 0x15cacc);
          *(undefined1 *)(*(int *)(DAT_0015cb14 + 0x15cace) + 0x4e) = 1;
          RecordHandler::saveOptions(pRVar8);
          showCreditsBuyWindow(this);
        }
        goto LAB_0015d0f0;
      }
      if (this[0xae] == (HangarWindow)0x0) {
        if (this[0xb0] != (HangarWindow)0x0) {
          local_14c = this_05;
          iVar5 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
          if (iVar5 == 0) {
            for (iVar5 = 0x12; iVar5 != 0x17; iVar5 = iVar5 + 1) {
              TouchButton::setVisible
                        (*(TouchButton **)(*(int *)(*(int *)(this + 0x24) + 4) + iVar5 * 4),false);
            }
            this[0xb0] = (HangarWindow)0x0;
            showCreditsBuyWindow(this);
          }
          iVar5 = AbyssEngine::ApplicationManager::GetApplicationData();
          local_158 = *(undefined4 **)(DAT_0015d224 + 0x15d004);
          local_150 = (int *)(DAT_0015d22c + 0x15d010);
          local_154 = *(Ship **)(DAT_0015d228 + 0x15d00e);
          iVar6 = *(int *)(DAT_0015d230 + 0x15d01a);
          for (uVar24 = 0; uVar24 != 5; uVar24 = uVar24 + 1) {
            iVar11 = TouchButton::OnTouchEnd
                               (*(int *)(*(int *)(*(int *)(this + 0x24) + 4) + uVar24 * 4 + 0x48),
                                param_1);
            puVar19 = local_158;
            if (iVar11 != 0) {
              switch(uVar24 & 0x7fffffff) {
              case 0:
                RecordHandler::recordStoreWrite((RecordHandler *)*local_158,0);
                RecordHandler::recordStoreWritePreview((RecordHandler *)*puVar19,0);
                break;
              case 1:
                *(undefined1 *)(iVar5 + 0xa0) = 1;
                NFC::free_credits_likeGOF2OnFacebook();
                Status::changeCredits(*(int *)local_154);
                *(undefined1 *)(iVar6 + 0x49) = 1;
                break;
              case 2:
                *(undefined1 *)(iVar5 + 0xa1) = 1;
                NFC::free_credits_likeFishlabsOnFacebook();
                Status::changeCredits(*(int *)local_154);
                *(undefined1 *)(iVar6 + 0x4a) = 1;
                break;
              case 3:
                *(undefined1 *)(iVar5 + 0xa2) = 1;
                NFC::free_credits_subscribeToYoutubeChannel();
                Status::changeCredits(*(int *)local_154);
                *(undefined1 *)(iVar6 + 0x4b) = 1;
                break;
              case 4:
                *(undefined1 *)(iVar5 + 0xa3) = 1;
                NFC::free_credits_followOnTwitter();
                Status::changeCredits(*(int *)local_154);
                *(undefined1 *)(iVar6 + 0x4c) = 1;
                break;
              case 5:
                *(undefined1 *)(iVar5 + 0xd) = 1;
                NFC::free_credits_rateGame();
                Status::changeCredits(*(int *)local_154);
                *(undefined1 *)(iVar6 + 0x4d) = 1;
              }
            }
          }
          goto LAB_0015d0f0;
        }
        if (this[0xac] != (HangarWindow)0x0) {
          local_14c = this_05;
          iVar5 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
          iVar6 = Item::getBlueprintAmount(*(Item **)(*(int *)(this + 0x84) + 0x10));
          iVar6 = iVar6 * 200;
          if (iVar5 == 0) {
            piVar26 = *(int **)(DAT_0015d620 + 0x15d3fe);
            iVar11 = Status::getCredits();
            if ((iVar11 < iVar6) || (this[0x11e] != (HangarWindow)0x0)) goto LAB_0015d412;
            Status::changeCredits(*piVar26);
            setSellMode(bVar4);
            *(undefined4 *)(this + 0x68) = 0;
            HangarList::setCurrentItemIndex(*(HangarList **)(this + 0x14),-1);
            this[0x11e] = (HangarWindow)0x0;
            this[0xac] = (HangarWindow)0x0;
          }
          else {
            if (iVar5 != 1) {
LAB_0015d412:
              iVar11 = Status::getCredits();
              if ((iVar6 <= iVar11) && ((iVar5 != 0 || (this[0x11e] == (HangarWindow)0x0))))
              goto LAB_0015d5fa;
            }
            Item::setStationAmount(*(Item **)(*(int *)(this + 0x84) + 0x10),*(int *)(this + 0x8c));
            Item::setAmount(*(Item **)(*(int *)(this + 0x84) + 0x10),*(int *)(this + 0xa0));
            Item::setBlueprintAmount(*(Item **)(*(int *)(this + 0x84) + 0x10),*(int *)(this + 0xa4))
            ;
            piVar26 = *(int **)(DAT_0015d628 + 0x15d458);
            Status::setCredits(*piVar26);
            *(undefined4 *)(this + 0x8c) = 0;
            *(undefined4 *)(this + 0x94) = 0;
            *(undefined4 *)(this + 0x68) = 0;
            *(undefined4 *)(this + 0xa0) = 0;
            *(undefined4 *)(this + 0xa4) = 0;
            *(undefined4 *)(this + 0xa8) = *(undefined4 *)(this + 0x9c);
            HangarList::setCurrentItemIndex(*(HangarList **)(this + 0x14),-1);
            this[0x3c] = (HangarWindow)0x0;
            this[0xac] = (HangarWindow)0x0;
            this[0x88] = (HangarWindow)0x0;
            iVar5 = Status::getCredits();
            if ((iVar5 < iVar6) && (this[0x11e] == (HangarWindow)0x0)) {
              pSVar31 = *(String **)(this + 0x20);
              iVar5 = *piVar26;
              pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015d62c + 0x15d4ae));
              AbyssEngine::String::String(aSStack_7c,pSVar7,false);
              Status::getCredits();
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_88,aSStack_58,false);
              AbyssEngine::String::String(aSStack_94,(char *)(DAT_0015d634 + 0x15d4ec),false);
              Status::replaceHash(aiStack_34,iVar5,aSStack_7c,aSStack_88);
              ChoiceWindow::set(pSVar31);
              this_05 = local_14c;
              pcVar23 = *(code **)(DAT_0015d638 + 0x15d512);
              (*pcVar23)(aiStack_34);
              (*pcVar23)(aSStack_94);
              (*pcVar23)(aSStack_88);
              (*pcVar23)(aSStack_58);
              (*pcVar23)(aSStack_7c);
              this[0x3c] = (HangarWindow)0x1;
            }
            this[0x11e] = (HangarWindow)0x0;
          }
LAB_0015d5fa:
          refreshCurrentContentHeight(this);
          goto LAB_0015c774;
        }
        if (this[0x93] == (HangarWindow)0x0) {
          local_14c = this_05;
          if (this[0x90] != (HangarWindow)0x0) {
            iVar5 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
            local_150 = *(int **)(DAT_0015d63c + 0x15d564);
            if (*(int *)(*local_150 + 0x114) == 3) {
              if (this[0x11d] == (HangarWindow)0x0) {
                if (this[0x91] != (HangarWindow)0x0) {
                  if (iVar5 != 0) {
                    if (iVar5 == 1) {
                      iVar5 = *(int *)(*local_150 + 0x14c);
                      Status::getShip();
                      Ship::getIndex();
                      iVar5 = Station::hasShip(iVar5);
                      if (iVar5 == 0) {
                        iVar5 = Ship::getPrice(*(Ship **)(*(int *)(this + 0x68) + 0xc));
                        iVar6 = Status::getCredits();
                        if (iVar6 < iVar5) {
                          piVar26 = *(int **)(DAT_0015dde4 + 0x15dbf0);
                          iVar5 = *local_150;
                          pSVar7 = (String *)GameText::getText(*piVar26);
                          AbyssEngine::String::String(aSStack_a0,pSVar7,false);
                          Ship::getPrice(*(Ship **)(*(int *)(this + 0x68) + 0xc));
                          Status::getCredits();
                          Layout::formatCredits((int)aSStack_58);
                          AbyssEngine::String::String(aSStack_ac,aSStack_58,false);
                          AbyssEngine::String::String
                                    (aSStack_b8,(char *)(DAT_0015ddec + 0x15dc40),false);
                          Status::replaceHash(aiStack_34,iVar5,aSStack_a0,aSStack_ac);
                          pcVar23 = *(code **)(DAT_0015ddf0 + 0x15dc5a);
                          (*pcVar23)(aSStack_b8);
                          (*pcVar23)(aSStack_ac);
                          (*pcVar23)(aSStack_58);
                          (*pcVar23)(aSStack_a0);
                          AbyssEngine::String::String
                                    (aSStack_70,(char *)(DAT_0015ddf4 + 0x15dc70),false);
                          GameText::getText(*piVar26);
                          AbyssEngine::operator+(aSStack_58,aSStack_70);
                          AbyssEngine::String::operator+=((String *)aiStack_34,aSStack_58);
                          AbyssEngine::String::~String(aSStack_58);
                          AbyssEngine::String::~String(aSStack_70);
                          ChoiceWindow::set(*(ChoiceWindow **)(this + 0x20),(String *)aiStack_34,
                                            true);
                          this[0x3c] = (HangarWindow)0x1;
                          this[0xaf] = (HangarWindow)0x1;
                          *(undefined2 *)(this + 0x90) = 0;
                          AbyssEngine::String::~String((String *)aiStack_34);
                          this_05 = local_14c;
                          goto LAB_0015c774;
                        }
                        local_158 = (undefined4 *)0x1;
                        goto LAB_0015d656;
                      }
                      pSVar7 = *(String **)(this + 0x20);
                      GameText::getText(**(int **)(DAT_0015d640 + 0x15d5b0));
                      ChoiceWindow::set(pSVar7);
                      this[0x3c] = (HangarWindow)0x1;
                      goto LAB_0015dbc8;
                    }
                    goto LAB_0015d0f0;
                  }
LAB_0015d652:
                  local_158 = (undefined4 *)0x0;
LAB_0015d656:
                  pSVar13 = (Ship *)Status::getShip();
                  pAVar14 = (Array *)Item::extractItems(*(Array **)(this + 0x10),true);
                  Ship::setCargo(pSVar13,pAVar14);
                  if (local_158 == (undefined4 *)0x0) {
                    if (this[0x11d] == (HangarWindow)0x0) {
                      iVar5 = *local_150;
                      pSVar13 = (Ship *)Status::getShip();
                      Ship::getPrice(pSVar13);
                      Ship::getPrice(*(Ship **)(*(int *)(this + 0x68) + 0xc));
                      goto LAB_0015d794;
                    }
                  }
                  else {
                    iVar5 = *local_150;
                    Ship::getPrice(*(Ship **)(*(int *)(this + 0x68) + 0xc));
LAB_0015d794:
                    Status::changeCredits(iVar5);
                  }
                  pHVar29 = this + 0x68;
                  pSVar13 = (Ship *)Status::getShip();
                  puVar27 = (uint *)Ship::getEquipment(pSVar13);
                  local_154 = pSVar13;
                  puVar17 = (uint *)Ship::getCargo();
                  piVar26 = *(int **)(DAT_0015da0c + 0x15d7ba);
                  iVar6 = *piVar26;
                  iVar5 = Ship::getIndex();
                  pSVar13 = (Ship *)Ship::makeShip(*(int *)(*(int *)(iVar6 + 4) + iVar5 * 4));
                  Ship::getRace(*(Ship **)(*(int *)pHVar29 + 0xc));
                  Ship::setRace((int)pSVar13);
                  Ship::adjustPrice(pSVar13);
                  __fn = extraout_r1;
                  pvVar20 = extraout_r2;
                  iVar5 = extraout_r3;
                  if (puVar17 != (uint *)0x0) {
                    for (uVar24 = 0; uVar24 < *puVar17; uVar24 = uVar24 + 1) {
                      pIVar18 = *(Item **)(puVar17[1] + uVar24 * 4);
                      if (pIVar18 != (Item *)0x0) {
                        Item::clone(pIVar18,__fn,pvVar20,iVar5,in_stack_fffffe88);
                        Ship::addCargo((Item *)pSVar13);
                        __fn = extraout_r1_00;
                        pvVar20 = extraout_r2_00;
                        iVar5 = extraout_r3_00;
                      }
                    }
                  }
                  if (puVar27 != (uint *)0x0) {
                    for (uVar24 = 0; uVar24 < *puVar27; uVar24 = uVar24 + 1) {
                      pIVar18 = *(Item **)(puVar27[1] + uVar24 * 4);
                      if (pIVar18 != (Item *)0x0) {
                        pIVar18 = (Item *)Item::clone(pIVar18,__fn,pvVar20,iVar5,in_stack_fffffe88);
                        uVar37 = Ship::addEquipment(pSVar13,pIVar18);
                        __fn = (__fn *)((ulonglong)uVar37 >> 0x20);
                        pvVar20 = extraout_r2_01;
                        iVar5 = extraout_r3_01;
                        if ((int)uVar37 == 0) {
                          Ship::addCargo((Item *)pSVar13);
                          __fn = extraout_r1_01;
                          pvVar20 = extraout_r2_02;
                          iVar5 = extraout_r3_02;
                        }
                      }
                    }
                  }
                  iVar5 = Ship::getMods(*(Ship **)(*(int *)pHVar29 + 0xc));
                  if (iVar5 != 0) {
                    for (uVar24 = 0;
                        puVar27 = (uint *)Ship::getMods(*(Ship **)(*(int *)pHVar29 + 0xc)),
                        uVar24 < *puVar27; uVar24 = uVar24 + 1) {
                      iVar5 = Ship::getMods(*(Ship **)(*(int *)pHVar29 + 0xc));
                      Ship::addMod(pSVar13,*(int *)(*(int *)(iVar5 + 4) + uVar24 * 4));
                    }
                  }
                  local_15c = pSVar13;
                  Status::getStation();
                  pAVar14 = (Array *)Station::getShips();
                  for (uVar24 = 0; uVar24 < *(uint *)pAVar14; uVar24 = uVar24 + 1) {
                    iVar5 = Ship::getIndex();
                    iVar6 = Ship::getIndex();
                    if (iVar5 == iVar6) {
                      pSVar13 = *(Ship **)(*(int *)(pAVar14 + 4) + uVar24 * 4);
                      if (local_158 != (undefined4 *)0x0) {
                        ArrayRemove<Ship*>(pSVar13,pAVar14);
                        goto LAB_0015d8d8;
                      }
                      if (pSVar13 == (Ship *)0x0) {
                        puVar19 = (undefined4 *)(*(int *)(pAVar14 + 4) + uVar24 * 4);
                      }
                      else {
                        pvVar20 = (void *)Ship::~Ship(pSVar13);
                        operator_delete(pvVar20);
                        puVar19 = (undefined4 *)(*(int *)(pAVar14 + 4) + uVar24 * 4);
                      }
                      pSVar13 = local_154;
                      *puVar19 = 0;
                      iVar6 = *piVar26;
                      iVar5 = Ship::getIndex();
                      uVar9 = Ship::makeShip(*(int *)(*(int *)(iVar6 + 4) + iVar5 * 4));
                      *(undefined4 *)(*(int *)(pAVar14 + 4) + uVar24 * 4) = uVar9;
                      Ship::adjustPrice(*(Ship **)(*(int *)(pAVar14 + 4) + uVar24 * 4));
                      iVar5 = *(int *)(*(int *)(pAVar14 + 4) + uVar24 * 4);
                      Ship::getRace(pSVar13);
                      Ship::setRace(iVar5);
                      goto LAB_0015d9e2;
                    }
                  }
                  if (local_158 == (undefined4 *)0x0) {
LAB_0015d95e:
                    uVar24 = 0xffffffff;
                  }
                  else {
LAB_0015d8d8:
                    pSVar13 = local_154;
                    iVar6 = *piVar26;
                    pSVar30 = *(Ship **)(*local_150 + 0x14c);
                    iVar5 = Ship::getIndex();
                    uVar24 = 0xffffffff;
                    Ship::makeShip(*(int *)(*(int *)(iVar6 + 4) + iVar5 * 4));
                    Station::addShip(pSVar30);
                    iVar5 = Ship::getMods(pSVar13);
                    pSVar13 = local_154;
                    if (iVar5 != 0) {
                      for (uVar24 = 0; puVar27 = (uint *)Ship::getMods(pSVar13), uVar24 < *puVar27;
                          uVar24 = uVar24 + 1) {
                        iVar5 = Station::getShips();
                        piVar26 = (int *)Station::getShips();
                        pSVar30 = *(Ship **)(*(int *)(iVar5 + 4) + *piVar26 * 4 + -4);
                        iVar5 = Ship::getMods(pSVar13);
                        Ship::addMod(pSVar30,*(int *)(*(int *)(iVar5 + 4) + uVar24 * 4));
                      }
                      goto LAB_0015d95e;
                    }
                  }
LAB_0015d9e2:
                  pSVar13 = local_154;
                  iVar5 = Ship::getMods(local_154);
                  piVar26 = local_150;
                  if ((-1 < (int)uVar24) && (iVar5 != 0)) {
                    for (uVar25 = 0; puVar27 = (uint *)Ship::getMods(pSVar13), uVar25 < *puVar27;
                        uVar25 = uVar25 + 1) {
                      if (this[0x11d] != (HangarWindow)0x0) {
                        iVar5 = Station::getShips();
                        pSVar30 = *(Ship **)(*(int *)(iVar5 + 4) + uVar24 * 4);
                        iVar5 = Ship::getMods(pSVar13);
                        Ship::addMod(pSVar30,*(int *)(*(int *)(iVar5 + 4) + uVar25 * 4));
                      }
                      pSVar30 = *(Ship **)(*(int *)(pAVar14 + 4) + uVar24 * 4);
                      iVar5 = Ship::getMods(pSVar13);
                      Ship::addMod(pSVar30,*(int *)(*(int *)(iVar5 + 4) + uVar25 * 4));
                    }
                  }
                  if (this[0x11d] != (HangarWindow)0x0) {
                    pAVar14 = *(Array **)(*piVar26 + 0x14c);
                    Status::getStation();
                    bVar4 = (bool)Station::getShips();
                    Station::setShips(pAVar14,bVar4);
                  }
                  Status::setShip((Ship *)*piVar26);
                  pSVar13 = (Ship *)Status::getShip();
                  Ship::refreshValue(pSVar13);
                  pHVar12 = *(HangarList **)(this + 0x14);
                  pSVar13 = (Ship *)Status::getShip();
                  HangarList::initShipTab(pHVar12,pSVar13);
                  if (*(Array **)(this + 0x10) != (Array *)0x0) {
                    ArrayReleaseClasses<Item*>(*(Array **)(this + 0x10));
                    if (*(Array<Item*> **)(this + 0x10) != (Array<Item*> *)0x0) {
                      pvVar20 = (void *)Array<Item*>::~Array(*(Array<Item*> **)(this + 0x10));
                      operator_delete(pvVar20);
                    }
                  }
                  *(undefined4 *)(this + 0x10) = 0;
                  Status::getShip();
                  pAVar14 = (Array *)Ship::getCargo();
                  Status::getStation();
                  pAVar33 = (Array *)Station::getItems();
                  pAVar14 = (Array *)Item::mixItems(pAVar14,pAVar33);
                  pHVar12 = *(HangarList **)(this + 0x14);
                  *(Array **)(this + 0x10) = pAVar14;
                  Status::getStation();
                  pAVar33 = (Array *)Station::getShips();
                  HangarList::initShopTab(pHVar12,pAVar14,pAVar33);
                  HangarList::setCurrentTab(*(int *)(this + 0x14),false);
                  refreshCurrentContentHeight(this);
                  this[0x3c] = (HangarWindow)0x0;
                  if (this[0x11d] == (HangarWindow)0x0) {
                    piVar26 = *(int **)(DAT_0015ddf8 + 0x15db38);
                    pSVar7 = (String *)GameText::getText(*piVar26);
                    AbyssEngine::String::String((String *)aiStack_34,pSVar7,false);
                    iVar6 = *local_150;
                    AbyssEngine::String::String(aSStack_c4,(String *)aiStack_34,false);
                    iVar5 = *piVar26;
                    Ship::getIndex();
                    pSVar7 = (String *)GameText::getText(iVar5);
                    AbyssEngine::String::String(aSStack_d0,pSVar7,false);
                    AbyssEngine::String::String(aSStack_dc,(char *)(DAT_0015ddfc + 0x15db7c),false);
                    Status::replaceHash(aSStack_58,iVar6,aSStack_c4,aSStack_d0);
                    AbyssEngine::String::operator=((String *)aiStack_34,aSStack_58);
                    pcVar23 = *(code **)(DAT_0015de00 + 0x15db9e);
                    (*pcVar23)(aSStack_58);
                    (*pcVar23)(aSStack_dc);
                    (*pcVar23)(aSStack_d0);
                    (*pcVar23)(aSStack_c4);
                    ChoiceWindow::set(*(String **)(this + 0x20));
                    this[0x3c] = (HangarWindow)0x1;
                    AbyssEngine::String::~String((String *)aiStack_34);
                  }
                  *this = (HangarWindow)0x1;
                  goto LAB_0015dbc8;
                }
                if (iVar5 != 1) {
                  if (iVar5 == 0) {
                    pCVar32 = *(ChoiceWindow **)(this + 0x20);
                    AbyssEngine::String::String
                              ((String *)aiStack_34,(char *)(DAT_0015d9fc + 0x15d6cc),false);
                    piVar26 = *(int **)(DAT_0015da00 + 0x15d6d4);
                    pSVar7 = (String *)GameText::getText(*piVar26);
                    pSVar31 = (String *)GameText::getText(*piVar26);
                    pSVar15 = (String *)GameText::getText(*piVar26);
                    pSVar16 = (String *)GameText::getText(*piVar26);
                    ChoiceWindow::set(pCVar32,(String *)aiStack_34,pSVar7,true,pSVar31,pSVar15,
                                      pSVar16,-1,-1);
                    AbyssEngine::String::~String((String *)aiStack_34);
                    this[0x91] = (HangarWindow)0x1;
                    this_05 = local_14c;
                    goto LAB_0015c774;
                  }
                  goto LAB_0015d0f0;
                }
              }
              else {
LAB_0015d646:
                if (iVar5 != 1) {
                  if (iVar5 != 0) goto LAB_0015d0f0;
                  goto LAB_0015d652;
                }
              }
LAB_0015d76e:
              this[0x3c] = (HangarWindow)0x0;
            }
            else {
              if (this[0x11d] != (HangarWindow)0x0) goto LAB_0015d646;
              if (this[0x92] == (HangarWindow)0x0) {
                if (iVar5 == 1) goto LAB_0015d76e;
                if (iVar5 == 0) {
                  pCVar32 = *(ChoiceWindow **)(this + 0x20);
                  AbyssEngine::String::String
                            ((String *)aiStack_34,(char *)(DAT_0015da04 + 0x15d742),false);
                  pSVar7 = (String *)GameText::getText(**(int **)(DAT_0015da08 + 0x15d74a));
                  ChoiceWindow::set(pCVar32,(String *)aiStack_34,pSVar7,true);
                  AbyssEngine::String::~String((String *)aiStack_34);
                  this[0x92] = (HangarWindow)0x1;
                  this_05 = local_14c;
                  goto LAB_0015c774;
                }
                goto LAB_0015d0f0;
              }
              if (iVar5 != 0) {
                if (iVar5 != 1) goto LAB_0015d0f0;
                local_158 = (undefined4 *)0x0;
                this[0x92] = (HangarWindow)0x0;
                goto LAB_0015d656;
              }
              this_02 = (ModStation *)
                        AbyssEngine::ApplicationManager::GetApplicationModule
                                  (**(uint **)(DAT_0015da10 + 0x15d980));
              ModStation::showDlcMenu(this_02);
              this[0x3c] = (HangarWindow)0x0;
              this[0x92] = (HangarWindow)0x0;
            }
LAB_0015dbc8:
            *(undefined2 *)(this + 0x90) = 0;
            this_05 = local_14c;
            goto LAB_0015c774;
          }
        }
        else {
          local_14c = this_05;
          iVar5 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
          if (iVar5 == 1) {
            this[0x93] = (HangarWindow)0x0;
            goto LAB_0015cf60;
          }
          if (iVar5 == 0) {
            piVar26 = *(int **)(DAT_0015d61c + 0x15d398);
            this[0x93] = (HangarWindow)0x0;
            this[0x3c] = (HangarWindow)0x0;
            iVar5 = *piVar26;
            ::ListItem::getPrice(*(ListItem **)(this + 0x68));
            Status::changeCredits(iVar5);
            pSVar13 = (Ship *)Status::getStation();
            Station::removeShip(pSVar13);
            pAVar33 = *(Array **)(this + 0x10);
            pHVar12 = *(HangarList **)(this + 0x14);
            Status::getStation();
            pAVar14 = (Array *)Station::getShips();
            HangarList::initShopTab(pHVar12,pAVar33,pAVar14);
            refreshCurrentContentHeight(this);
          }
        }
        goto LAB_0015d0f0;
      }
      local_14c = this_05;
      iVar5 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
      if (iVar5 != 0) {
LAB_0015ce6a:
        pHVar29 = this + 0x24;
        for (uVar24 = 0; uVar24 < 5; uVar24 = uVar24 + 1) {
          iVar5 = TouchButton::OnTouchEnd
                            (*(int *)(*(int *)(*(int *)pHVar29 + 4) + uVar24 * 4 + 0x30),param_1);
          if (iVar5 != 0) {
            if ((uVar24 & 0x7fffffff) < 5) {
              switch(uVar24) {
              case 0:
                NFC::iap_buy_credits_300_000();
                break;
              case 1:
                NFC::iap_buy_credits_1_000_000();
                break;
              case 2:
                NFC::iap_buy_credits_3_000_000();
                break;
              case 3:
                NFC::iap_buy_credits_10_000_000();
                break;
              case 4:
                AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
                (*(code *)this)(aSStack_a0);
                (*(code *)this)(&stack0x000003d8);
                (*(code *)this)(&stack0x000003e4);
                (*(code *)this)(&stack0x000003f0);
                uVar9 = *in_stack_fffffe98;
                AbyssEngine::String::String((String *)&stack0x000003cc,aSStack_7c,false);
                iVar5 = Mission::getProductionGoodAmount();
                AbyssEngine::String::String(aSStack_b8,iVar5);
                AbyssEngine::String::String((String *)&stack0x000003c0,aSStack_b8,false);
                AbyssEngine::String::String
                          ((String *)&stack0x000003b4,(char *)(DAT_0017c35c + 0x17c07e),false);
                Status::replaceHash(aSStack_a0,uVar9,&stack0x000003cc,&stack0x000003c0);
                AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
                (*(code *)this)(aSStack_a0);
                (*(code *)this)(&stack0x000003b4);
                (*(code *)this)(&stack0x000003c0);
                (*(code *)this)(aSStack_b8);
                (*(code *)this)(&stack0x000003cc);
                iVar5 = Mission::getType();
                if (iVar5 == 0xe) {
                  uVar9 = *in_stack_fffffe98;
                  AbyssEngine::String::String((String *)&stack0x000003a8,aSStack_7c,false);
                  Mission::getTargetSystemName();
                  AbyssEngine::String::String((String *)&stack0x0000039c,aSStack_b8,false);
                  AbyssEngine::String::String
                            ((String *)&stack0x00000390,(char *)(DAT_0017c360 + 0x17c100),false);
                  Status::replaceHash(aSStack_a0,uVar9,&stack0x000003a8,&stack0x0000039c);
                  AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
                  (*(code *)this)(aSStack_a0);
                  (*(code *)this)(&stack0x00000390);
                  (*(code *)this)(&stack0x0000039c);
                  (*(code *)this)(aSStack_b8);
                  puVar3 = &stack0x000004fc;
                }
                else {
                  uVar9 = *in_stack_fffffe98;
                  AbyssEngine::String::String((String *)&stack0x00000384,aSStack_7c,false);
                  Mission::getTargetStationName();
                  AbyssEngine::String::String((String *)&stack0x00000378,aSStack_b8,false);
                  AbyssEngine::String::String
                            ((String *)&stack0x0000036c,(char *)(DAT_0017c364 + 0x17c176),false);
                  Status::replaceHash(aSStack_a0,uVar9,&stack0x00000384,&stack0x00000378);
                  AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
                  (*(code *)this)(aSStack_a0);
                  (*(code *)this)(&stack0x0000036c);
                  (*(code *)this)(&stack0x00000378);
                  (*(code *)this)(aSStack_b8);
                  puVar3 = &stack0x000004d8;
                }
                (*(code *)this)(puVar3 + -0x154);
                uVar9 = *in_stack_fffffe98;
                AbyssEngine::String::String((String *)&stack0x00000360,aSStack_7c,false);
                Mission::getTargetName();
                AbyssEngine::String::String((String *)&stack0x00000354,aSStack_b8,false);
                AbyssEngine::String::String
                          ((String *)&stack0x00000348,(char *)(DAT_0017c368 + 0x17c1ec),false);
                Status::replaceHash(aSStack_a0,uVar9,&stack0x00000360,&stack0x00000354);
                AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
                (*(code *)this)(aSStack_a0);
                (*(code *)this)(&stack0x00000348);
                (*(code *)this)(&stack0x00000354);
                (*(code *)this)(aSStack_b8);
                (*(code *)this)(&stack0x00000360);
                iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017c36c + 0x17c238));
                iVar5 = DAT_0017c370;
                *(int *)(this_05 + 0x28) = iVar6 + 0x2fc;
                AbyssEngine::String::String(aSStack_b8,(char *)(iVar5 + 0x17c24e),false);
                GameText::getText(*(int *)param_1);
                AbyssEngine::operator+(aSStack_a0,aSStack_b8);
                AbyssEngine::String::operator+=(aSStack_7c,aSStack_a0);
                AbyssEngine::String::~String(aSStack_a0);
                AbyssEngine::String::~String(aSStack_b8);
                this_03 = (Standing *)Status::getStanding();
                iVar5 = Agent::getRace(this_05);
                fVar21 = (float)Standing::getMissionBonus(this_03,iVar5);
                uVar24 = in_fpscr & 0xfffffff | (uint)(fVar21 < 0.0) << 0x1f |
                         (uint)(fVar21 == 0.0) << 0x1e;
                uVar25 = uVar24 | (uint)NAN(fVar21) << 0x1c;
                bVar2 = (byte)(uVar24 >> 0x18);
                bVar34 = (bool)(bVar2 >> 6 & 1);
                bVar4 = bVar2 >> 7 != ((byte)(uVar25 >> 0x1c) & 1);
                if (bVar34 || bVar4) {
                  AbyssEngine::String::String(aSStack_a0,(char *)(DAT_0017c6f4 + 0x17c38c),false);
                }
                else {
                  AbyssEngine::String::String(aSStack_c4,(char *)(DAT_0017c374 + 0x17c2b8),false);
                  uVar9 = *in_stack_fffffe98;
                  pSVar7 = (String *)GameText::getText(*(int *)param_1);
                  AbyssEngine::String::String((String *)&stack0x0000033c,pSVar7,false);
                  AbyssEngine::String::String(aSStack_f4,(int)(fVar21 * DAT_0017c378));
                  AbyssEngine::String::String((String *)&stack0x00000330,aSStack_f4,false);
                  AbyssEngine::String::String
                            ((String *)&stack0x00000324,(char *)(DAT_0017c37c + 0x17c308),false);
                  Status::replaceHash(aSStack_e8,uVar9,&stack0x0000033c,&stack0x00000330);
                  AbyssEngine::operator+(aSStack_b8,aSStack_c4);
                  AbyssEngine::String::String
                            ((String *)&stack0x00000318,(char *)(DAT_0017c380 + 0x17c33a),false);
                  AbyssEngine::operator+(aSStack_a0,aSStack_b8);
                }
                AbyssEngine::String::operator=((String *)(param_2 + 0xa4),aSStack_a0);
                AbyssEngine::String::~String(aSStack_a0);
                if (!bVar34 && !bVar4) {
                  (*(code *)this)(&stack0x00000318);
                  (*(code *)this)(aSStack_b8);
                  (*(code *)this)(aSStack_e8);
                  (*(code *)this)(&stack0x00000324);
                  (*(code *)this)(&stack0x00000330);
                  (*(code *)this)(aSStack_f4);
                  (*(code *)this)(&stack0x0000033c);
                  (*(code *)this)(aSStack_c4);
                }
                AbyssEngine::String::String((String *)&stack0x0000030c,aSStack_7c,false);
                Agent::setMissionString(this_05,&stack0x0000030c);
                AbyssEngine::String::~String((String *)&stack0x0000030c);
                Agent::getMission(this_05);
                uVar9 = Mission::getReward();
                fVar36 = (float)VectorSignedToFloat(uVar9,(byte)(uVar25 >> 0x16) & 3);
                iVar6 = (int)(fVar21 * fVar36);
                __aeabi_idivmod(iVar6,0x32);
                this_04 = (Mission *)Agent::getMission(this_05);
                iVar5 = extraout_r1_02 + iVar6;
                __aeabi_idivmod(iVar5,0x32);
                if (extraout_r1_03 != 0) {
                  iVar5 = iVar6 - extraout_r1_02;
                }
                Mission::setBonus(this_04,iVar5);
                uVar9 = *in_stack_fffffe98;
                AbyssEngine::String::String((String *)&stack0x00000300,aSStack_7c,false);
                Mission::getReward();
                Mission::getBonus();
                Layout::formatCredits((int)&stack0x000002dc);
                AbyssEngine::String::String((String *)&stack0x000002e8,&stack0x000002dc,false);
                AbyssEngine::operator+(&stack0x000002f4,&stack0x000002e8);
                AbyssEngine::String::String
                          ((String *)&stack0x000002d0,(char *)(DAT_0017c6fc + 0x17c4a6),false);
                Status::replaceHash(aSStack_a0,uVar9,&stack0x00000300,&stack0x000002f4);
                AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
                (*(code *)this)(aSStack_a0);
                (*(code *)this)(&stack0x000002d0);
                (*(code *)this)(&stack0x000002f4);
                (*(code *)this)(&stack0x000002e8);
                (*(code *)this)(&stack0x000002dc);
                (*(code *)this)(&stack0x00000300);
                pvVar20 = (void *)Generator::~Generator(in_stack_fffffe94);
                operator_delete(pvVar20);
                iVar5 = Agent::getOffer(local_160);
                if (iVar5 == 0) {
                  Agent::getMission(local_160);
                  iVar5 = Mission::getType();
                  if (iVar5 == 8) goto LAB_0017c512;
                }
                else {
LAB_0017c512:
                  AbyssEngine::String::String((String *)&stack0x000001ec,aSStack_7c,false);
                  Agent::setMissionString(local_160,&stack0x000001ec);
                  AbyssEngine::String::~String((String *)&stack0x000001ec);
                }
                iVar5 = Agent::getOffer(local_160);
                if (iVar5 == 1) {
                  pSVar28 = *(ScrollTouchWindow **)(param_2 + 0x60);
                  Agent::getName();
                  AbyssEngine::String::String((String *)&stack0x000001d4,aSStack_7c,false);
                  ScrollTouchWindow::setText(pSVar28,&stack0x000001e0,&stack0x000001d4);
                  AbyssEngine::String::~String((String *)&stack0x000001d4);
                  AbyssEngine::String::~String((String *)&stack0x000001e0);
                  Agent::setEvent(local_160,1);
                  goto LAB_0017c83e;
                }
                iVar5 = Agent::getOffer(local_160);
                if (iVar5 == 7) {
                  pSVar28 = *(ScrollTouchWindow **)(param_2 + 0x60);
                  Agent::getName();
                  AbyssEngine::String::String((String *)&stack0x000001bc,aSStack_7c,false);
                  ScrollTouchWindow::setText(pSVar28,&stack0x000001c8,&stack0x000001bc);
                  AbyssEngine::String::~String((String *)&stack0x000001bc);
                  AbyssEngine::String::~String((String *)&stack0x000001c8);
                  goto LAB_0017c83e;
                }
                iVar5 = Agent::getMission(local_160);
                if (iVar5 == 0) {
LAB_0017c714:
                  AbyssEngine::String::String
                            ((String *)&stack0x000002e8,(char *)(DAT_0017cb00 + 0x17c71a),false);
                  AbyssEngine::operator+(aSStack_a0,&stack0x000002e8);
                  AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
                  AbyssEngine::String::~String(aSStack_a0);
                  AbyssEngine::String::~String((String *)&stack0x000002e8);
                  AbyssEngine::String::String
                            ((String *)&stack0x000002e8,(char *)(DAT_0017cb04 + 0x17c754),false);
                  iVar5 = **(int **)(DAT_0017cb08 + 0x17c766);
                  AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017cb0c + 0x17c768));
                  GameText::getText(iVar5);
                  AbyssEngine::operator+(aSStack_a0,&stack0x000002e8);
                  AbyssEngine::String::~String((String *)&stack0x000002e8);
                  pSVar28 = *(ScrollTouchWindow **)(param_2 + 0x60);
                  Agent::getName();
                  AbyssEngine::String::String
                            ((String *)&stack0x0000012c,(char *)(DAT_0017cb10 + 0x17c7a6),false);
                  AbyssEngine::operator+(&stack0x00000138,aSStack_40);
                  AbyssEngine::operator+(&stack0x00000144,&stack0x00000138);
                  AbyssEngine::String::String
                            ((String *)&stack0x00000120,(char *)(DAT_0017cb14 + 0x17c7ca),false);
                  AbyssEngine::operator+(&stack0x00000150,&stack0x00000144);
                  AbyssEngine::operator+(&stack0x000002dc,&stack0x00000150);
                  AbyssEngine::operator+(&stack0x000002e8,&stack0x000002dc);
                  AbyssEngine::operator+(&stack0x0000015c,&stack0x000002e8);
                  ScrollTouchWindow::setText(pSVar28,&stack0x00000168,&stack0x0000015c);
                  (*(code *)this)(&stack0x0000015c);
                  (*(code *)this)(&stack0x000002e8);
                  (*(code *)this)(&stack0x000002dc);
                  (*(code *)this)(&stack0x00000150);
                  (*(code *)this)(&stack0x00000120);
                  (*(code *)this)(&stack0x00000144);
                  (*(code *)this)(&stack0x00000138);
                  (*(code *)this)(&stack0x0000012c);
                  puVar3 = &stack0x000002bc;
                }
                else {
                  Agent::getMission(local_160);
                  iVar5 = Mission::getType();
                  if (iVar5 != 0xc) goto LAB_0017c714;
                  piVar26 = *(int **)(DAT_0017c700 + 0x17c5cc);
                  pSVar7 = (String *)GameText::getText(*piVar26);
                  AbyssEngine::String::operator=(aSStack_7c,pSVar7);
                  uVar9 = *in_stack_fffffe98;
                  AbyssEngine::String::String((String *)&stack0x000001b0,aSStack_7c,false);
                  Agent::getMission(local_160);
                  Mission::getReward();
                  Agent::getMission(local_160);
                  Mission::getBonus();
                  Layout::formatCredits((int)&stack0x000002e8);
                  AbyssEngine::String::String((String *)&stack0x000001a4,&stack0x000002e8,false);
                  AbyssEngine::String::String
                            ((String *)&stack0x00000198,(char *)(DAT_0017c708 + 0x17c632),false);
                  Status::replaceHash(aSStack_a0,uVar9,&stack0x000001b0,&stack0x000001a4);
                  AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
                  (*(code *)this)(aSStack_a0);
                  (*(code *)this)(&stack0x00000198);
                  (*(code *)this)(&stack0x000001a4);
                  (*(code *)this)(&stack0x000002e8);
                  (*(code *)this)(&stack0x000001b0);
                  AbyssEngine::String::String((String *)&stack0x0000018c,aSStack_7c,false);
                  Agent::setMissionString(local_160,&stack0x0000018c);
                  AbyssEngine::String::~String((String *)&stack0x0000018c);
                  AbyssEngine::String::String
                            ((String *)&stack0x000002e8,(char *)(DAT_0017c70c + 0x17c68c),false);
                  iVar5 = *piVar26;
                  AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017c710 + 0x17c69e));
                  GameText::getText(iVar5);
                  AbyssEngine::operator+(aSStack_a0,&stack0x000002e8);
                  AbyssEngine::String::~String((String *)&stack0x000002e8);
                  pSVar28 = *(ScrollTouchWindow **)(param_2 + 0x60);
                  Agent::getName();
                  AbyssEngine::operator+(&stack0x00000174,aSStack_7c);
                  ScrollTouchWindow::setText(pSVar28,&stack0x00000180,&stack0x00000174);
                  (*(code *)this)(&stack0x00000174);
                  puVar3 = &stack0x000002d4;
                }
                (*(code *)this)(puVar3 + -0x154);
                (*(code *)this)(aSStack_a0);
LAB_0017c83e:
                (*(code *)this)(aSStack_7c);
                (*(code *)this)(aSStack_70);
                (*(code *)this)(aSStack_64);
                (*(code *)this)(aSStack_40);
                Status::getStation();
                Station::getName();
                Agent::setStationName(local_160,&local_150);
                AbyssEngine::String::~String((String *)&local_150);
                Status::getSystem();
                SolarSystem::getName();
                Agent::setSystemName(local_160,&local_15c);
                AbyssEngine::String::~String((String *)&local_15c);
                *(undefined4 *)(param_2 + 0x2c) = 0;
                *(undefined4 *)(param_2 + 0x14) = 1;
                SpaceLounge::onKeyPress((SpaceLounge *)param_2,0x10000);
                pVVar22 = (Vector *)SpaceLounge::getSoundId((SpaceLounge *)param_2,local_160);
                if (-1 < (int)pVVar22) {
                  FModSound::play(**(int **)(DAT_0017cb1c + 0x17c8ce),pVVar22,(Vector *)0x0,
                                  extraout_s0);
                }
                if (*piStack_164 == aiStack_34[0]) {
                  return;
                }
                    /* WARNING: Subroutine does not return */
                __stack_chk_fail();
              default:
                NFC::iap_buy_credits_100_000();
              }
            }
            break;
          }
        }
        iVar5 = TouchButton::OnTouchEnd(*(int *)(*(int *)(*(int *)pHVar29 + 4) + 0x44),param_1);
        if (iVar5 != 0) {
          bVar4 = true;
          iVar5 = *(int *)(DAT_0015d214 + 0x15ced6);
          if ((((*(char *)(iVar5 + 0x4a) != '\0') && (*(char *)(iVar5 + 0x49) != '\0')) &&
              (*(char *)(iVar5 + 0x4d) != '\0')) && (*(char *)(iVar5 + 0x4c) != '\0')) {
            bVar4 = *(char *)(iVar5 + 0x4b) == '\0';
          }
          iVar5 = AbyssEngine::ApplicationManager::GetApplicationData();
          if ((bVar4) || (*(char *)(iVar5 + 0x15) == '\0')) {
            for (iVar5 = 0xc; iVar5 != 0x11; iVar5 = iVar5 + 1) {
              TouchButton::setVisible
                        (*(TouchButton **)(*(int *)(*(int *)pHVar29 + 4) + iVar5 * 4),false);
            }
            showFreeCreditsWindow(this);
          }
        }
        goto LAB_0015d0f0;
      }
      this[0xae] = (HangarWindow)0x0;
      this[0x3c] = (HangarWindow)0x0;
      for (iVar5 = 0xc; iVar5 != 0x11; iVar5 = iVar5 + 1) {
        TouchButton::setVisible
                  (*(TouchButton **)(*(int *)(*(int *)(this + 0x24) + 4) + iVar5 * 4),false);
      }
      TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)(this + 0x24) + 4) + 0x44),false);
      puVar27 = *(uint **)(DAT_0015d608 + 0x15d25e);
      iVar5 = AbyssEngine::ApplicationManager::GetApplicationData();
      *(undefined1 *)(iVar5 + 0x40) = 0;
      iVar5 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar27);
      if ((iVar5 == 0) || (*(char *)(iVar5 + 0x18) == '\0')) goto LAB_0015ce6a;
    }
    else {
      local_14c = this_05;
      iVar5 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
      if (iVar5 == 1) {
        this[0x3c] = (HangarWindow)0x0;
        this[0x11c] = (HangarWindow)0x0;
        *(undefined4 *)(this + 0xb4) = *(undefined4 *)(this + 0xe4);
      }
      else if (((iVar5 == 0) && (*(int *)(this + 0x28) != 0)) &&
              (*(Item **)(this + 0x2c) != (Item *)0x0)) {
        demountItem(this,*(Item **)(this + 0x2c),-1);
        *(undefined4 *)(this + 0xe4) = *(undefined4 *)(this + 0xb4);
        mountItem(this,*(Item **)(this + 0x28));
        this[0x3c] = (HangarWindow)0x0;
        this[0x11c] = (HangarWindow)0x0;
      }
LAB_0015d0f0:
      HVar1 = this[0x88];
      iVar6 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x20),param_1);
      this_05 = local_14c;
      iVar5 = DAT_0015d234;
      if (HVar1 != (HangarWindow)0x0) {
        if (iVar6 == 1) {
          pHVar12 = *(HangarList **)(this + 0x14);
          this[0x88] = (HangarWindow)0x0;
          this[0x3c] = (HangarWindow)0x0;
        }
        else {
          if (iVar6 != 0) goto LAB_0015c774;
          if (this[0x130] == (HangarWindow)0x0) {
            this[0x3c] = (HangarWindow)0x0;
            if (this[0xad] == (HangarWindow)0x0) {
              this[0x88] = (HangarWindow)0x1;
            }
            else {
              for (uVar24 = 0; uVar24 < **(uint **)(this + 4); uVar24 = uVar24 + 1) {
                TouchButton::resetTouch(*(TouchButton **)((*(uint **)(this + 4))[1] + uVar24 * 4));
              }
            }
            goto LAB_0015c774;
          }
          pHVar12 = *(HangarList **)(this + 0x14);
          this[0x3c] = (HangarWindow)0x0;
          this[0x130] = (HangarWindow)0x0;
          this[0x88] = (HangarWindow)0x0;
        }
        *(undefined4 *)(this + 0x68) = 0;
        goto LAB_0015d16a;
      }
      if (iVar6 != 0) goto LAB_0015c774;
      this[0x3c] = (HangarWindow)0x0;
      iVar5 = AbyssEngine::ApplicationManager::GetApplicationModule(**(uint **)(iVar5 + 0x15d142));
      if (*(char *)(iVar5 + 0x18) == '\0') goto LAB_0015c774;
    }
    *(undefined1 *)(iVar5 + 0x18) = 0;
  }
LAB_0015c774:
  if (*(int *)this_05 - local_28 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(*(int *)this_05 - local_28);
}

```

## target disasm
```
  0015c740: push {r4,r5,r6,r7,lr}
  0015c742: add r7,sp,#0xc
  0015c744: push {r8,r9,r10,r11}
  0015c748: sub sp,#0x154
  0015c74a: mov r10,r0
  0015c74c: ldr r0,[0x0015cae4]
  0015c74e: mov r11,r2
  0015c750: mov r8,r1
  0015c752: add r0,pc
  0015c754: movs r6,#0x0
  0015c756: ldr.w r9,[r0,#0x0]
  0015c75a: ldr.w r0,[r9,#0x0]
  0015c75e: str r0,[sp,#0x150]
  0015c760: ldrb.w r0,[r10,#0xd1]
  0015c764: strd r6,r6,[r10,#0x6c]
  0015c768: cmp r0,#0x0
  0015c76a: strb.w r6,[r10,#0xd0]
  0015c76e: beq 0x0015c78c
  0015c770: strb.w r6,[r10,#0xd1]
  0015c774: ldr r0,[sp,#0x150]
  0015c776: ldr.w r1,[r9,#0x0]
  0015c77a: subs r0,r1,r0
  0015c77c: itttt eq
  0015c77e: mov.eq r0,r6
  0015c780: add.eq sp,#0x154
  0015c782: pop.eq.w {r8,r9,r10,r11}
  0015c786: pop.eq {r4,r5,r6,r7,pc}
  0015c788: blx 0x0006e824
  0015c78c: ldrb.w r0,[r10,#0x3c]
  0015c790: cmp r0,#0x0
  0015c792: beq.w 0x0015c898
  0015c796: ldrb.w r0,[r10,#0xb1]
  0015c79a: cmp r0,#0x0
  0015c79c: beq.w 0x0015c8d6
  0015c7a0: ldr.w r0,[r10,#0x20]
  0015c7a4: mov r1,r8
  0015c7a6: mov r2,r11
  0015c7a8: blx 0x00074770
  0015c7ac: cmp r0,#0x1
  0015c7ae: beq.w 0x0015ca56
  0015c7b2: cmp r0,#0x0
  0015c7b4: bne.w 0x0015cd84
  0015c7b8: ldr.w r0,[r10,#0x80]
  0015c7bc: blx 0x00077128
  0015c7c0: mov r4,r0
  0015c7c2: ldr r0,[0x0015cae8]
  0015c7c4: add r0,pc
  0015c7c6: ldr r5,[r0,#0x0]
  0015c7c8: ldr r0,[r5,#0x0]
  0015c7ca: blx 0x000733d8
  0015c7ce: cmp r4,r0
  0015c7d0: ble.w 0x0015cd10
  0015c7d4: ldr r0,[0x0015caec]
  0015c7d6: mov r11,r9
  0015c7d8: movs r1,#0xcb
  0015c7da: ldr.w r8,[r5,#0x0]
  0015c7de: add r0,pc
  0015c7e0: ldr.w r9,[r0,#0x0]
  0015c7e4: ldr.w r0,[r9,#0x0]
  0015c7e8: blx 0x00072f70
  0015c7ec: mov r1,r0
  0015c7ee: add r0,sp,#0x138
  0015c7f0: movs r2,#0x0
  0015c7f2: blx 0x0006f028
  0015c7f6: ldr r0,[0x0015caf0]
  0015c7f8: add r0,pc
  0015c7fa: ldr r0,[r0,#0x0]
  0015c7fc: ldr r6,[r0,#0x0]
  0015c7fe: ldr r0,[r5,#0x0]
  0015c800: blx 0x000733d8
  0015c804: subs r2,r4,r0
  0015c806: add r0,sp,#0x120
  0015c808: mov r1,r6
  0015c80a: blx 0x00074e54
  0015c80e: add r0,sp,#0x12c
  0015c810: add r1,sp,#0x120
  0015c812: movs r2,#0x0
  0015c814: blx 0x0006f028
  0015c818: ldr r1,[0x0015caf4]
  0015c81a: add r1,pc
  0015c81c: add r0,sp,#0x114
  0015c81e: movs r2,#0x0
  0015c820: blx 0x0006ee18
  0015c824: str r0,[sp,#0x0]
  0015c826: add r0,sp,#0x144
  0015c828: add r2,sp,#0x138
  0015c82a: add r3,sp,#0x12c
  0015c82c: mov r1,r8
  0015c82e: blx 0x000739b4
  0015c832: ldr r0,[0x0015caf8]
  0015c834: add r0,pc
  0015c836: ldr r4,[r0,#0x0]
  0015c838: add r0,sp,#0x114
  0015c83a: blx r4
  0015c83c: add r0,sp,#0x12c
  0015c83e: blx r4
  0015c840: add r0,sp,#0x120
  0015c842: blx r4
  0015c844: add r0,sp,#0x138
  0015c846: blx r4
  0015c848: ldr r1,[0x0015cafc]
  0015c84a: add r1,pc
  0015c84c: add r0,sp,#0x108
  0015c84e: movs r2,#0x0
  0015c850: blx 0x0006ee18
  0015c854: ldr.w r0,[r9,#0x0]
  0015c858: movs r1,#0x7c
  0015c85a: blx 0x00072f70
  0015c85e: mov r2,r0
  0015c860: add r0,sp,#0x120
  0015c862: add r1,sp,#0x108
  0015c864: blx 0x0006ef98
  0015c868: add r0,sp,#0x144
  0015c86a: add r1,sp,#0x120
  0015c86c: blx 0x0006ef5c
  0015c870: add r0,sp,#0x120
  0015c872: mov r9,r11
  0015c874: blx 0x0006ee30
  0015c878: add r0,sp,#0x108
  0015c87a: blx 0x0006ee30
  0015c87e: ldr.w r0,[r10,#0x20]
  0015c882: add r1,sp,#0x144
  0015c884: movs r2,#0x1
  0015c886: movs r4,#0x1
  0015c888: blx 0x000746f8
  0015c88c: add r0,sp,#0x144
  0015c88e: strb.w r4,[r10,#0xaf]
  0015c892: blx 0x0006ee30
  0015c896: b 0x0015cd84
  0015c898: ldr.w r0,[r10,#0x58]
  0015c89c: cmp r0,#0x1
  0015c89e: bne 0x0015c8ac
  0015c8a0: ldr.w r0,[r10,#0x18]
  0015c8a4: mov r1,r8
  0015c8a6: mov r2,r11
  0015c8a8: blx 0x00077134
  0015c8ac: ldr r0,[0x0015cb00]
  0015c8ae: mov r1,r8
  0015c8b0: mov r2,r11
  0015c8b2: add r0,pc
  0015c8b4: ldr r4,[r0,#0x0]
  0015c8b6: ldr r0,[r4,#0x0]
  0015c8b8: blx 0x0007477c
  0015c8bc: cbz r0,0x0015c93a
  0015c8be: ldr.w r0,[r10,#0x58]
  0015c8c2: cmp r0,#0x1
  0015c8c4: bne.w 0x0015ca5e
  0015c8c8: ldr r0,[r4,#0x0]
  0015c8ca: blx 0x00074dc4
  0015c8ce: movs r6,#0x0
  0015c8d0: str.w r6,[r10,#0x58]
  0015c8d4: b 0x0015c774
  0015c8d6: ldrb.w r0,[r10,#0x11c]
  0015c8da: str.w r9,[sp,#0x2c]
  0015c8de: cmp r0,#0x0
  0015c8e0: beq.w 0x0015caa0
  0015c8e4: ldr.w r0,[r10,#0x20]
  0015c8e8: mov r1,r8
  0015c8ea: mov r2,r11
  0015c8ec: blx 0x00074770
  0015c8f0: cmp r0,#0x1
  0015c8f2: beq.w 0x0015ce0c
  0015c8f6: cmp r0,#0x0
  0015c8f8: bne.w 0x0015d0f0
  0015c8fc: ldr.w r0,[r10,#0x28]
  0015c900: cmp r0,#0x0
  0015c902: beq.w 0x0015d0f0
  0015c906: ldr.w r1,[r10,#0x2c]
  0015c90a: cmp r1,#0x0
  0015c90c: beq.w 0x0015d0f0
  0015c910: mov r0,r10
  0015c912: mov.w r2,#0xffffffff
  0015c916: blx 0x00077140
  0015c91a: ldr.w r1,[r10,#0x28]
  0015c91e: ldr.w r0,[r10,#0xb4]
  0015c922: str.w r0,[r10,#0xe4]
  0015c926: mov r0,r10
  0015c928: blx 0x0007714c
  0015c92c: movs r0,#0x0
  0015c92e: strb.w r0,[r10,#0x3c]
  0015c932: strb.w r0,[r10,#0x11c]
  0015c936: b.w 0x0015d0f0
  0015c93a: ldr.w r1,[r10,#0xc0]
  0015c93e: movs r5,#0x0
  0015c940: ldr.w r0,[r10,#0xb4]
  0015c944: movs r6,#0x0
  0015c946: str r4,[sp,#0x28]
  0015c948: cmp r1,#0x0
  0015c94a: vmov s2,r1
  0015c94e: vldr.32 s0,[pc,#0x1b4]
  0015c952: mov r2,r1
  0015c954: add r0,r1
  0015c956: vcvt.f32.s32 s2,s2
  0015c95a: it mi
  0015c95c: rsb.mi r2,r1
  0015c95e: cmp r2,#0x3
  0015c960: ldr r2,[0x0015cb08]
  0015c962: it gt
  0015c964: vmov.gt.f32 s0,s2
  0015c968: str.w r2,[r10,#0xc4]
  0015c96c: str.w r0,[r10,#0xb4]
  0015c970: str.w r0,[r10,#0xbc]
  0015c974: vstr.32 s0,[r10,#0xc8]
  0015c978: ldr r0,[0x0015cb0c]
  0015c97a: add r0,pc
  0015c97c: ldr r4,[r0,#0x0]
  0015c97e: b 0x0015c9d8
  0015c980: ldr r0,[r0,#0x4]
  0015c982: mov r1,r8
  0015c984: mov r2,r11
  0015c986: ldr.w r0,[r0,r6,lsl #0x2]
  0015c98a: blx 0x00074788
  0015c98e: cbz r0,0x0015c9d6
  0015c990: mov r0,r10
  0015c992: movs r1,#0x0
  0015c994: blx r4
  0015c996: mov r0,r10
  0015c998: movs r1,#0x1
  0015c99a: blx r4
  0015c99c: mov r0,r10
  0015c99e: movs r1,#0x0
  0015c9a0: blx r4
  0015c9a2: ldr.w r0,[r10,#0x14]
  0015c9a6: mov r1,r6
  0015c9a8: movs r2,#0x1
  0015c9aa: str.w r5,[r10,#0x68]
  0015c9ae: blx 0x00076f24
  0015c9b2: cmp r6,#0x2
  0015c9b4: itt eq
  0015c9b6: mov.eq r0,r10
  0015c9b8: blx.eq 0x0007711c
  0015c9bc: mov r0,r10
  0015c9be: blx 0x00076f30
  0015c9c2: ldr.w r0,[r10,#0x14]
  0015c9c6: mov.w r1,#0xffffffff
  0015c9ca: str.w r5,[r10,#0xb4]
  0015c9ce: str.w r5,[r10,#0xbc]
  0015c9d2: blx 0x000770b0
  0015c9d6: adds r6,#0x1
  0015c9d8: ldr.w r0,[r10,#0x4]
  0015c9dc: ldr r1,[r0,#0x0]
  0015c9de: cmp r6,r1
  0015c9e0: bcc 0x0015c980
  0015c9e2: ldr r4,[sp,#0x28]
  0015c9e4: ldr r1,[r4,#0x0]
  0015c9e6: ldr r0,[r1,#0xc]
  0015c9e8: cmp r0,r11
  0015c9ea: bge 0x0015ca46
  0015c9ec: ldr r6,[r1,#0x20]
  0015c9ee: sub.w r0,r11,r0
  0015c9f2: ldr.w r3,[r10,#0x10c]
  0015c9f6: mov r4,r9
  0015c9f8: ldr r1,[r1,#0x70]
  0015c9fa: subs r0,r0,r6
  0015c9fc: ldr.w r2,[r10,#0xb4]
  0015ca00: subs r0,r0,r3
  0015ca02: add r1,r3
  0015ca04: ldr.w r9,[r10,#0x14]
  0015ca08: subs r0,r0,r2
  0015ca0a: blx 0x0007198c
  0015ca0e: mov r6,r0
  0015ca10: mov r0,r9
  0015ca12: mov r9,r4
  0015ca14: ldr r4,[sp,#0x28]
  0015ca16: blx 0x000770a4
  0015ca1a: cmp r6,r0
  0015ca1c: bge 0x0015ca46
  0015ca1e: ldr.w r0,[r10,#0x14]
  0015ca22: mov r1,r6
  0015ca24: blx 0x000770b0
  0015ca28: mov r0,r10
  0015ca2a: blx 0x00077158
  0015ca2e: cbz r0,0x0015ca46
  0015ca30: ldrb.w r0,[r10,#0xd2]
  0015ca34: cbz r0,0x0015ca46
  0015ca36: mov r0,r10
  0015ca38: movs r1,#0x0
  0015ca3a: blx 0x000751cc
  0015ca3e: mov r0,r10
  0015ca40: movs r1,#0x1
  0015ca42: blx 0x000751cc
  0015ca46: ldrb.w r0,[r10,#0xd2]
  0015ca4a: cmp r0,#0x0
  0015ca4c: beq 0x0015cb18
  0015ca4e: movs r6,#0x0
  0015ca50: strb.w r6,[r10,#0xd2]
  0015ca54: b 0x0015c774
  0015ca56: movs r0,#0x0
  0015ca58: strb.w r0,[r10,#0x3c]
  0015ca5c: b 0x0015cd84
  0015ca5e: ldr.w r0,[r10,#0x14]
  0015ca62: blx 0x00076f48
  0015ca66: cmp r0,#0x4
  0015ca68: bne.w 0x0015ce20
  0015ca6c: mov r0,r10
  0015ca6e: movs r1,#0x0
  0015ca70: movs r6,#0x0
  0015ca72: blx 0x000751cc
  0015ca76: ldr.w r0,[r10,#0x14]
  0015ca7a: movs r1,#0x2
  0015ca7c: movs r2,#0x1
  0015ca7e: str.w r6,[r10,#0x68]
  0015ca82: blx 0x00076f24
  0015ca86: mov r0,r10
  0015ca88: blx 0x0007711c
  0015ca8c: mov r0,r10
  0015ca8e: blx 0x00076f30
  0015ca92: ldr.w r0,[r10,#0x14]
  0015ca96: str.w r6,[r10,#0xb4]
  0015ca9a: str.w r6,[r10,#0xbc]
  0015ca9e: b 0x0015d16a
  0015caa0: ldrb.w r0,[r10,#0xaf]
  0015caa4: cmp r0,#0x0
  0015caa6: beq.w 0x0015ce4a
  0015caaa: ldr.w r0,[r10,#0x20]
  0015caae: mov r1,r8
  0015cab0: mov r2,r11
  0015cab2: blx 0x00074770
  0015cab6: cmp r0,#0x1
  0015cab8: beq.w 0x0015cf5a
  0015cabc: cmp r0,#0x0
  0015cabe: bne.w 0x0015d0f0
  0015cac2: ldr r0,[0x0015cb10]
  0015cac4: movs r2,#0x1
  0015cac6: ldr r1,[0x0015cb14]
  0015cac8: add r0,pc
  0015caca: add r1,pc
  0015cacc: ldr r0,[r0,#0x0]
  0015cace: ldr r1,[r1,#0x0]
  0015cad0: ldr r0,[r0,#0x0]
  0015cad2: strb.w r2,[r1,#0x4e]
  0015cad6: blx 0x00071788
  0015cada: mov r0,r10
  0015cadc: blx 0x0007525c
  0015cae0: b 0x0015d0f0
  0015cb18: ldr.w r0,[r10,#0x24]
  0015cb1c: mov r1,r8
  0015cb1e: mov r2,r11
  0015cb20: str.w r9,[sp,#0x2c]
  0015cb24: ldr r0,[r0,#0x4]
  0015cb26: ldr r0,[r0,#0x5c]
  0015cb28: blx 0x00074788
  0015cb2c: cmp r0,#0x0
  0015cb2e: beq 0x0015cbb0
  0015cb30: ldr.w r0,[r10,#0x80]
  0015cb34: blx 0x00077128
  0015cb38: mov r6,r0
  0015cb3a: ldr r0,[0x0015ce70]
  0015cb3c: ldr r1,[0x0015ce74]
  0015cb3e: add r0,pc
  0015cb40: ldr.w r5,[r10,#0x20]
  0015cb44: add r1,pc
  0015cb46: ldr r0,[r0,#0x0]
  0015cb48: ldr r1,[r1,#0x0]
  0015cb4a: ldr.w r9,[r0,#0x0]
  0015cb4e: ldr r0,[r1,#0x0]
  0015cb50: movs r1,#0xc3
  0015cb52: blx 0x00072f70
  0015cb56: mov r1,r0
  0015cb58: add r0,sp,#0x90
  0015cb5a: movs r2,#0x0
  0015cb5c: blx 0x0006f028
  0015cb60: ldr r1,[r4,#0x0]
  0015cb62: add r0,sp,#0x84
  0015cb64: mov r2,r6
  0015cb66: blx 0x00074e54
  0015cb6a: ldr r1,[0x0015ce78]
  0015cb6c: add r1,pc
  0015cb6e: add r0,sp,#0x78
  0015cb70: movs r2,#0x0
  0015cb72: blx 0x0006ee18
  0015cb76: str r0,[sp,#0x0]
  0015cb78: add r0,sp,#0x144
  0015cb7a: add r2,sp,#0x90
  0015cb7c: add r3,sp,#0x84
  0015cb7e: mov r1,r9
  0015cb80: blx 0x000739b4
  0015cb84: add r1,sp,#0x144
  0015cb86: mov r0,r5
  0015cb88: movs r2,#0x1
  0015cb8a: movs r4,#0x1
  0015cb8c: blx 0x000746f8
  0015cb90: ldr r0,[0x0015ce7c]
  0015cb92: add r0,pc
  0015cb94: ldr r5,[r0,#0x0]
  0015cb96: add r0,sp,#0x144
  0015cb98: blx r5
  0015cb9a: add r0,sp,#0x78
  0015cb9c: blx r5
  0015cb9e: add r0,sp,#0x84
  0015cba0: blx r5
  0015cba2: add r0,sp,#0x90
  0015cba4: blx r5
  0015cba6: strb.w r4,[r10,#0xb1]
  0015cbaa: strb.w r4,[r10,#0x3c]
  0015cbae: ldr r4,[sp,#0x28]
  0015cbb0: mov r0,r10
  0015cbb2: blx 0x00077158
  0015cbb6: cmp r0,#0x0
  0015cbb8: beq.w 0x0015cd8c
  0015cbbc: ldr r0,[0x0015ce80]
  0015cbbe: mov.w r9,#0x0
  0015cbc2: add r0,pc
  0015cbc4: ldr r0,[r0,#0x0]
  0015cbc6: str r0,[sp,#0x28]
  0015cbc8: ldr r0,[0x0015ce84]
  0015cbca: add r0,pc
  0015cbcc: ldr r0,[r0,#0x0]
  0015cbce: str r0,[sp,#0x24]
  0015cbd0: ldr r0,[0x0015ce88]
  0015cbd2: add r0,pc
  0015cbd4: ldr r0,[r0,#0x0]
  0015cbd6: str r0,[sp,#0x20]
  0015cbd8: ldr.w r0,[r10,#0x24]
  0015cbdc: ldr r1,[r0,#0x0]
  0015cbde: cmp r9,r1
  0015cbe0: bcs.w 0x0015cd8c
  0015cbe4: ldr r0,[r0,#0x4]
  0015cbe6: mov r1,r8
  0015cbe8: mov r2,r11
  0015cbea: ldr.w r0,[r0,r9,lsl #0x2]
  0015cbee: blx 0x00074788
  0015cbf2: cbz r0,0x0015cc4c
  0015cbf4: ldr r0,[r4,#0x0]
  0015cbf6: ldr.w r3,[r10,#0x10c]
  0015cbfa: ldr.w r2,[r10,#0xb4]
  0015cbfe: ldr r6,[r0,#0xc]
  0015cc00: ldr r5,[r0,#0x20]
  0015cc02: ldr r0,[r0,#0x70]
  0015cc04: adds r1,r0,r3
  0015cc06: sub.w r0,r11,r6
  0015cc0a: subs r0,r0,r5
  0015cc0c: subs r0,r0,r3
  0015cc0e: subs r0,r0,r2
  0015cc10: blx 0x0007198c
  0015cc14: mov r1,r0
  0015cc16: bic r0,r9,#0x80000000
  0015cc1a: cmp r0,#0xb
  0015cc1c: bhi 0x0015cc4c
  0015cc1e: tbh [pc,r0]
  0015cc4c: add.w r9,r9,#0x1
  0015cc50: b 0x0015cbd8
  0015cd10: ldr.w r0,[r10,#0x80]
  0015cd14: movs r1,#0x0
  0015cd16: strb.w r1,[r10,#0x3c]
  0015cd1a: cbz r0,0x0015cd84
  0015cd1c: blx 0x000738f4
  0015cd20: cbz r0,0x0015cd54
  0015cd22: ldr r0,[r5,#0x0]
  0015cd24: blx 0x00071c14
  0015cd28: blx 0x00071824
  0015cd2c: ldr.w r1,[r10,#0x80]
  0015cd30: str r0,[r1,#0x10]
  0015cd32: ldr r0,[r5,#0x0]
  0015cd34: blx 0x00071c14
  0015cd38: add r6,sp,#0x144
  0015cd3a: mov r1,r0
  0015cd3c: mov r0,r6
  0015cd3e: blx 0x000736a8
  0015cd42: ldr.w r0,[r10,#0x80]
  0015cd46: adds r0,#0x14
  0015cd48: mov r1,r6
  0015cd4a: blx 0x0006f2b0
  0015cd4e: add r0,sp,#0x144
  0015cd50: blx 0x0006ee30
  0015cd54: ldr.w r0,[r10,#0x80]
  0015cd58: blx 0x00077170
  0015cd5c: ldr.w r0,[r10,#0x14]
  0015cd60: movs r1,#0x1
  0015cd62: movs r6,#0x1
  0015cd64: blx 0x0007624c
  0015cd68: mov r1,r0
  0015cd6a: mov r0,r10
  0015cd6c: blx 0x000770c8
  0015cd70: mov r0,r10
  0015cd72: movs r1,#0x0
  0015cd74: strb.w r6,[r10,#0x88]
  0015cd78: blx 0x000751cc
  0015cd7c: ldr r0,[r5,#0x0]
  0015cd7e: rsbs r1,r4
  0015cd80: blx 0x0007171c
  0015cd84: movs r6,#0x0
  0015cd86: strb.w r6,[r10,#0xb1]
  0015cd8a: b 0x0015c774
  0015cd8c: ldr r0,[r4,#0x0]
  0015cd8e: blx 0x000747b8
  0015cd92: ldr.w r9,[sp,#0x2c]
  0015cd96: cbz r0,0x0015cdd4
  0015cd98: ldr.w r0,[r10,#0x58]
  0015cd9c: cmp r0,#0x1
  0015cd9e: bne.w 0x0015cf66
  0015cda2: ldr r0,[0x0015ce90]
  0015cda4: movw r1,#0x283
  0015cda8: ldr r6,[r4,#0x0]
  0015cdaa: add r0,pc
  0015cdac: ldr r0,[r0,#0x0]
  0015cdae: ldr r0,[r0,#0x0]
  0015cdb0: blx 0x00072f70
  0015cdb4: add.w r9,sp,#0x6c
  0015cdb8: mov r1,r0
  0015cdba: movs r2,#0x0
  0015cdbc: mov r0,r9
  0015cdbe: blx 0x0006f028
  0015cdc2: mov r0,r6
  0015cdc4: mov r1,r9
  0015cdc6: blx 0x000747c4
  0015cdca: add r0,sp,#0x6c
  0015cdcc: blx 0x0006ee30
  0015cdd0: ldr.w r9,[sp,#0x2c]
  0015cdd4: ldr.w r0,[r10,#0x24]
  0015cdd8: mov r1,r8
  0015cdda: mov r2,r11
  0015cddc: ldr r0,[r0,#0x4]
  0015cdde: ldr r0,[r0,#0x2c]
  0015cde0: blx 0x00074788
  0015cde4: cmp r0,#0x0
  0015cde6: beq.w 0x0015d600
  0015cdea: ldr r0,[0x0015ce94]
  0015cdec: movs r2,#0x1
  0015cdee: ldr r1,[0x0015ce98]
  0015cdf0: add r0,pc
  0015cdf2: add r1,pc
  0015cdf4: ldr r0,[r0,#0x0]
  0015cdf6: ldr r1,[r1,#0x0]
  0015cdf8: ldr r0,[r0,#0x0]
  0015cdfa: strb.w r2,[r1,#0x4e]
  0015cdfe: blx 0x00071788
  0015ce02: mov r0,r10
  0015ce04: blx 0x0007525c
  0015ce08: b.w 0x0015d600
  0015ce0c: ldr.w r0,[r10,#0xe4]
  0015ce10: movs r1,#0x0
  0015ce12: strb.w r1,[r10,#0x3c]
  0015ce16: strb.w r1,[r10,#0x11c]
  0015ce1a: str.w r0,[r10,#0xb4]
  0015ce1e: b 0x0015d0f0
  0015ce20: ldr.w r0,[r10,#0x14]
  0015ce24: blx 0x00076f48
  0015ce28: cmp r0,#0x3
  0015ce2a: bne.w 0x0015cf2e
  0015ce2e: ldr.w r0,[r10,#0x14]
  0015ce32: movs r1,#0x0
  0015ce34: movs r2,#0x1
  0015ce36: movs r6,#0x0
  0015ce38: blx 0x00076f24
  0015ce3c: mov r0,r10
  0015ce3e: blx 0x0007711c
  0015ce42: mov r0,r10
  0015ce44: blx 0x00076f30
  0015ce48: b 0x0015c774
  0015ce4a: ldrb.w r0,[r10,#0xae]
  0015ce4e: cmp r0,#0x0
  0015ce50: beq.w 0x0015cfae
  0015ce54: ldr.w r0,[r10,#0x20]
  0015ce58: mov r1,r8
  0015ce5a: mov r2,r11
  0015ce5c: blx 0x00074770
  0015ce60: cmp r0,#0x0
  0015ce62: beq.w 0x0015d202
  0015ce66: add.w r6,r10,#0x24
  0015ce6a: movs r4,#0x0
  0015ce6c: b 0x0015ceb6
  0015ce9c: ldr r0,[r6,#0x0]
  0015ce9e: mov r1,r8
  0015cea0: mov r2,r11
  0015cea2: ldr r0,[r0,#0x4]
  0015cea4: add.w r0,r0,r4, lsl #0x2
  0015cea8: ldr r0,[r0,#0x30]
  0015ceaa: blx 0x00074788
  0015ceae: cmp r0,#0x0
  0015ceb0: bne.w 0x0015d1e4
  0015ceb4: adds r4,#0x1
  0015ceb6: cmp r4,#0x4
  0015ceb8: bls 0x0015ce9c
  0015ceba: ldr r0,[r6,#0x0]
  0015cebc: mov r1,r8
  0015cebe: mov r2,r11
  0015cec0: ldr r0,[r0,#0x4]
  0015cec2: ldr r0,[r0,#0x44]
  0015cec4: blx 0x00074788
  0015cec8: cmp r0,#0x0
  0015ceca: beq.w 0x0015d0f0
  0015cece: ldr r0,[0x0015d214]
  0015ced0: movs r4,#0x1
  0015ced2: add r0,pc
  0015ced4: ldr r0,[r0,#0x0]
  0015ced6: ldrb.w r1,[r0,#0x4a]
  0015ceda: cbz r1,0x0015cef8
  0015cedc: ldrb.w r1,[r0,#0x49]
  0015cee0: cbz r1,0x0015cef8
  0015cee2: ldrb.w r1,[r0,#0x4d]
  0015cee6: cbz r1,0x0015cef8
  0015cee8: ldrb.w r1,[r0,#0x4c]
  0015ceec: cbz r1,0x0015cef8
  0015ceee: ldrb.w r0,[r0,#0x4b]
  0015cef2: clz r0,r0
  0015cef6: lsrs r4,r0,#0x5
  0015cef8: ldr r0,[0x0015d218]
  0015cefa: add r0,pc
  0015cefc: ldr r0,[r0,#0x0]
  0015cefe: ldr r0,[r0,#0x0]
  0015cf00: blx 0x00071704
  0015cf04: cbnz r4,0x0015cf0e
  0015cf06: ldrb r0,[r0,#0x15]
  0015cf08: cmp r0,#0x0
  0015cf0a: bne.w 0x0015d0f0
  0015cf0e: movs r4,#0xc
  0015cf10: b 0x0015cf22
  0015cf12: ldr r0,[r6,#0x0]
  0015cf14: movs r1,#0x0
  0015cf16: ldr r0,[r0,#0x4]
  0015cf18: ldr.w r0,[r0,r4,lsl #0x2]
  0015cf1c: blx 0x00074e48
  0015cf20: adds r4,#0x1
  0015cf22: cmp r4,#0x11
  0015cf24: bne 0x0015cf12
  0015cf26: mov r0,r10
  0015cf28: blx 0x0007717c
  0015cf2c: b 0x0015d0f0
  0015cf2e: mov r0,r10
  0015cf30: blx 0x00077188
  0015cf34: movs r6,#0x0
  0015cf36: cmp r0,#0x0
  0015cf38: beq.w 0x0015c774
  0015cf3c: mov r0,r10
  0015cf3e: movs r1,#0x0
  0015cf40: blx 0x000751cc
  0015cf44: ldr.w r0,[r10,#0x14]
  0015cf48: mov.w r1,#0xffffffff
  0015cf4c: str.w r6,[r10,#0x68]
  0015cf50: blx 0x000770b0
  0015cf54: movs r6,#0x1
  0015cf56: b.w 0x0015c774
  0015cf5a: movs r0,#0x0
  0015cf5c: strb.w r0,[r10,#0xaf]
  0015cf60: strb.w r0,[r10,#0x3c]
  0015cf64: b 0x0015d0f0
  0015cf66: ldr.w r0,[r10,#0x14]
  0015cf6a: blx 0x00076f48
  0015cf6e: cmp r0,#0x4
  0015cf70: bhi.w 0x0015cdd4
  0015cf74: tbh [pc,r0]
  0015cf82: ldr r0,[0x0015d21c]
  0015cf84: movw r1,#0x26f
  0015cf88: ldr r6,[r4,#0x0]
  0015cf8a: add r0,pc
  0015cf8c: ldr r0,[r0,#0x0]
  0015cf8e: ldr r0,[r0,#0x0]
  0015cf90: blx 0x00072f70
  0015cf94: add.w r9,sp,#0x60
  0015cf98: mov r1,r0
  0015cf9a: movs r2,#0x0
  0015cf9c: mov r0,r9
  0015cf9e: blx 0x0006f028
  0015cfa2: mov r0,r6
  0015cfa4: mov r1,r9
  0015cfa6: blx 0x000747c4
  0015cfaa: add r0,sp,#0x60
  0015cfac: b 0x0015cdcc
  0015cfae: ldrb.w r0,[r10,#0xb0]
  0015cfb2: cmp r0,#0x0
  0015cfb4: beq.w 0x0015d288
  0015cfb8: ldr.w r0,[r10,#0x20]
  0015cfbc: mov r1,r8
  0015cfbe: mov r2,r11
  0015cfc0: blx 0x00074770
  0015cfc4: cbnz r0,0x0015cfec
  0015cfc6: movs r4,#0x12
  0015cfc8: b 0x0015cfdc
  0015cfca: ldr.w r0,[r10,#0x24]
  0015cfce: movs r1,#0x0
  0015cfd0: ldr r0,[r0,#0x4]
  0015cfd2: ldr.w r0,[r0,r4,lsl #0x2]
  0015cfd6: blx 0x00074e48
  0015cfda: adds r4,#0x1
  0015cfdc: cmp r4,#0x17
  0015cfde: bne 0x0015cfca
  0015cfe0: movs r0,#0x0
  0015cfe2: strb.w r0,[r10,#0xb0]
  0015cfe6: mov r0,r10
  0015cfe8: blx 0x0007525c
  0015cfec: ldr r0,[0x0015d220]
  0015cfee: add r0,pc
  0015cff0: ldr r0,[r0,#0x0]
  0015cff2: ldr r0,[r0,#0x0]
  0015cff4: blx 0x00071704
  0015cff8: mov r5,r0
  0015cffa: ldr r0,[0x0015d224]
  0015cffc: movs r6,#0x0
  0015cffe: movs r4,#0x1
  0015d000: add r0,pc
  0015d002: ldr r0,[r0,#0x0]
  0015d004: str r0,[sp,#0x20]
  0015d006: ldr r0,[0x0015d228]
  0015d008: ldr r1,[0x0015d22c]
  0015d00a: add r0,pc
  0015d00c: add r1,pc
  0015d00e: str r1,[sp,#0x28]
  0015d010: ldr r0,[r0,#0x0]
  0015d012: str r0,[sp,#0x24]
  0015d014: ldr r0,[0x0015d230]
  0015d016: add r0,pc
  0015d018: ldr.w r9,[r0,#0x0]
  0015d01c: b 0x0015d0ec
  0015d01e: ldr.w r0,[r10,#0x24]
  0015d022: mov r1,r8
  0015d024: mov r2,r11
  0015d026: ldr r0,[r0,#0x4]
  0015d028: add.w r0,r0,r6, lsl #0x2
  0015d02c: ldr r0,[r0,#0x48]
  0015d02e: blx 0x00074788
  0015d032: cmp r0,#0x0
  0015d034: beq 0x0015d0ea
  0015d036: bic r0,r6,#0x80000000
  0015d03a: cmp r0,#0x5
  0015d03c: bhi 0x0015d0ea
  0015d03e: tbb [pc,r0]
  0015d048: mov r4,r5
  0015d04a: ldr r5,[sp,#0x20]
  0015d04c: movs r1,#0x0
  0015d04e: ldr r0,[r5,#0x0]
  0015d050: blx 0x000717a0
  0015d054: ldr r0,[r5,#0x0]
  0015d056: movs r1,#0x0
  0015d058: mov r5,r4
  0015d05a: movs r4,#0x1
  0015d05c: blx 0x000717ac
  0015d060: b 0x0015d0ea
  0015d062: strb.w r4,[r5,#0xa3]
  0015d066: blx 0x00077194
  0015d06a: ldr r0,[sp,#0x28]
  0015d06c: ldr.w r1,[r0,r6,lsl #0x2]
  0015d070: ldr r0,[sp,#0x24]
  0015d072: ldr r0,[r0,#0x0]
  0015d074: blx 0x0007171c
  0015d078: strb.w r4,[r9,#0x4c]
  0015d07c: b 0x0015d0ea
  0015d07e: strb.w r4,[r5,#0xa1]
  0015d082: blx 0x000771a0
  0015d086: ldr r0,[sp,#0x28]
  0015d088: ldr.w r1,[r0,r6,lsl #0x2]
  0015d08c: ldr r0,[sp,#0x24]
  0015d08e: ldr r0,[r0,#0x0]
  0015d090: blx 0x0007171c
  0015d094: strb.w r4,[r9,#0x4a]
  0015d098: b 0x0015d0ea
  0015d09a: strb.w r4,[r5,#0xa2]
  0015d09e: blx 0x000771ac
  0015d0a2: ldr r0,[sp,#0x28]
  0015d0a4: ldr.w r1,[r0,r6,lsl #0x2]
  0015d0a8: ldr r0,[sp,#0x24]
  0015d0aa: ldr r0,[r0,#0x0]
  0015d0ac: blx 0x0007171c
  0015d0b0: strb.w r4,[r9,#0x4b]
  0015d0b4: b 0x0015d0ea
  0015d0b6: strb.w r4,[r5,#0xa0]
  0015d0ba: blx 0x000771b8
  0015d0be: ldr r0,[sp,#0x28]
  0015d0c0: ldr.w r1,[r0,r6,lsl #0x2]
  0015d0c4: ldr r0,[sp,#0x24]
  0015d0c6: ldr r0,[r0,#0x0]
  0015d0c8: blx 0x0007171c
  0015d0cc: strb.w r4,[r9,#0x49]
  0015d0d0: b 0x0015d0ea
  0015d0d2: strb r4,[r5,#0xd]
  0015d0d4: blx 0x000771c4
  0015d0d8: ldr r0,[sp,#0x28]
  0015d0da: ldr.w r1,[r0,r6,lsl #0x2]
  0015d0de: ldr r0,[sp,#0x24]
  0015d0e0: ldr r0,[r0,#0x0]
  0015d0e2: blx 0x0007171c
  0015d0e6: strb.w r4,[r9,#0x4d]
  0015d0ea: adds r6,#0x1
  0015d0ec: cmp r6,#0x5
  0015d0ee: bne 0x0015d01e
  0015d0f0: ldr.w r0,[r10,#0x20]
  0015d0f4: mov r1,r8
  0015d0f6: mov r2,r11
  0015d0f8: ldrb.w r4,[r10,#0x88]
  0015d0fc: blx 0x00074770
  0015d100: movs r6,#0x0
  0015d102: cbz r4,0x0015d12c
  0015d104: ldr.w r9,[sp,#0x2c]
  0015d108: cmp r0,#0x1
  0015d10a: beq 0x0015d15a
  0015d10c: cmp r0,#0x0
  0015d10e: bne.w 0x0015c774
  0015d112: ldrb.w r0,[r10,#0x130]
  0015d116: cbz r0,0x0015d176
  0015d118: movs r6,#0x0
  0015d11a: ldr.w r0,[r10,#0x14]
  0015d11e: strb.w r6,[r10,#0x3c]
  0015d122: strb.w r6,[r10,#0x130]
  0015d126: strb.w r6,[r10,#0x88]
  0015d12a: b 0x0015d166
  0015d12c: ldr.w r9,[sp,#0x2c]
  0015d130: cmp r0,#0x0
  0015d132: bne.w 0x0015c774
  0015d136: ldr r0,[0x0015d234]
  0015d138: movs r1,#0x5
  0015d13a: strb.w r6,[r10,#0x3c]
  0015d13e: add r0,pc
  0015d140: ldr r0,[r0,#0x0]
  0015d142: ldr r0,[r0,#0x0]
  0015d144: blx 0x00075bd4
  0015d148: ldrb r1,[r0,#0x18]
  0015d14a: cmp r1,#0x0
  0015d14c: beq.w 0x0015c774
  0015d150: movs r1,#0x0
  0015d152: movs r6,#0x1
  0015d154: strb r1,[r0,#0x18]
  0015d156: b.w 0x0015c774
  0015d15a: ldr.w r0,[r10,#0x14]
  0015d15e: strb.w r6,[r10,#0x88]
  0015d162: strb.w r6,[r10,#0x3c]
  0015d166: str.w r6,[r10,#0x68]
  0015d16a: mov.w r1,#0xffffffff
  0015d16e: blx 0x000770b0
  0015d172: b.w 0x0015c774
  0015d176: ldrb.w r0,[r10,#0xad]
  0015d17a: movs r1,#0x0
  0015d17c: strb.w r1,[r10,#0x3c]
  0015d180: cmp r0,#0x0
  0015d182: beq.w 0x0015d280
  0015d186: movs r4,#0x0
  0015d188: b 0x0015d196
  0015d18a: ldr r0,[r0,#0x4]
  0015d18c: ldr.w r0,[r0,r4,lsl #0x2]
  0015d190: blx 0x00076fb4
  0015d194: adds r4,#0x1
  0015d196: ldr.w r0,[r10,#0x4]
  0015d19a: ldr r1,[r0,#0x0]
  0015d19c: cmp r4,r1
  0015d19e: bcc 0x0015d18a
  0015d1a0: b 0x0015d600
  0015d1dc: ldr.w r9,[sp,#0x2c]
  0015d1e0: b.w 0x0015c774
  0015d1e4: bic r0,r4,#0x80000000
  0015d1e8: cmp r0,#0x4
  0015d1ea: bhi.w 0x0015ceba
  0015d1ee: tbh [pc,r0]
  0015d202: add.w r6,r10,#0x24
  0015d206: movs r0,#0x0
  0015d208: movs r4,#0xc
  0015d20a: strb.w r0,[r10,#0xae]
  0015d20e: strb.w r0,[r10,#0x3c]
  0015d212: b 0x0015d246
  0015d238: ldr r0,[r0,#0x4]
  0015d23a: movs r1,#0x0
  0015d23c: ldr.w r0,[r0,r4,lsl #0x2]
  0015d240: blx 0x00074e48
  0015d244: adds r4,#0x1
  0015d246: ldr r0,[r6,#0x0]
  0015d248: cmp r4,#0x11
  0015d24a: bne 0x0015d238
  0015d24c: ldr r0,[r0,#0x4]
  0015d24e: movs r1,#0x0
  0015d250: movs r4,#0x0
  0015d252: ldr r0,[r0,#0x44]
  0015d254: blx 0x00074e48
  0015d258: ldr r0,[0x0015d608]
  0015d25a: add r0,pc
  0015d25c: ldr r5,[r0,#0x0]
  0015d25e: ldr r0,[r5,#0x0]
  0015d260: blx 0x00071704
  0015d264: strb.w r4,[r0,#0x40]
  0015d268: movs r1,#0x5
  0015d26a: ldr r0,[r5,#0x0]
  0015d26c: blx 0x00075bd4
  0015d270: cmp r0,#0x0
  0015d272: beq.w 0x0015ce6a
  0015d276: ldrb r1,[r0,#0x18]
  0015d278: cmp r1,#0x0
  0015d27a: bne.w 0x0015d150
  0015d27e: b 0x0015ce6a
  0015d280: movs r0,#0x1
  0015d282: strb.w r0,[r10,#0x88]
  0015d286: b 0x0015d600
  0015d288: ldrb.w r0,[r10,#0xac]
  0015d28c: cmp r0,#0x0
  0015d28e: beq 0x0015d36c
  0015d290: ldr.w r0,[r10,#0x20]
  0015d294: mov r1,r8
  0015d296: mov r2,r11
  0015d298: blx 0x00074770
  0015d29c: mov r5,r0
  0015d29e: ldr.w r0,[r10,#0x84]
  0015d2a2: ldr r0,[r0,#0x10]
  0015d2a4: blx 0x00076f78
  0015d2a8: movs r1,#0xc8
  0015d2aa: cmp r5,#0x0
  0015d2ac: mul r4,r0,r1
  0015d2b0: beq.w 0x0015d3f8
  0015d2b4: cmp r5,#0x1
  0015d2b6: bne.w 0x0015d412
  0015d2ba: b 0x0015d424
  0015d2bc: ldr r0,[0x0015d60c]
  0015d2be: movw r1,#0x272
  0015d2c2: ldr r6,[r4,#0x0]
  0015d2c4: add r0,pc
  0015d2c6: ldr r0,[r0,#0x0]
  0015d2c8: ldr r0,[r0,#0x0]
  0015d2ca: blx 0x00072f70
  0015d2ce: add.w r9,sp,#0x30
  0015d2d2: mov r1,r0
  0015d2d4: movs r2,#0x0
  0015d2d6: mov r0,r9
  0015d2d8: blx 0x0006f028
  0015d2dc: mov r0,r6
  0015d2de: mov r1,r9
  0015d2e0: blx 0x000747c4
  0015d2e4: add r0,sp,#0x30
  0015d2e6: b 0x0015cdcc
  0015d2e8: ldr r0,[0x0015d610]
  0015d2ea: movw r1,#0x271
  0015d2ee: ldr r6,[r4,#0x0]
  0015d2f0: add r0,pc
  0015d2f2: ldr r0,[r0,#0x0]
  0015d2f4: ldr r0,[r0,#0x0]
  0015d2f6: blx 0x00072f70
  0015d2fa: add.w r9,sp,#0x48
  0015d2fe: mov r1,r0
  0015d300: movs r2,#0x0
  0015d302: mov r0,r9
  0015d304: blx 0x0006f028
  0015d308: mov r0,r6
  0015d30a: mov r1,r9
  0015d30c: blx 0x000747c4
  0015d310: add r0,sp,#0x48
  0015d312: b 0x0015cdcc
  0015d314: ldr r0,[0x0015d614]
  0015d316: mov.w r1,#0x270
  0015d31a: ldr r6,[r4,#0x0]
  0015d31c: add r0,pc
  0015d31e: ldr r0,[r0,#0x0]
  0015d320: ldr r0,[r0,#0x0]
  0015d322: blx 0x00072f70
  0015d326: add.w r9,sp,#0x3c
  0015d32a: mov r1,r0
  0015d32c: movs r2,#0x0
  0015d32e: mov r0,r9
  0015d330: blx 0x0006f028
  0015d334: mov r0,r6
  0015d336: mov r1,r9
  0015d338: blx 0x000747c4
  0015d33c: add r0,sp,#0x3c
  0015d33e: b 0x0015cdcc
  0015d340: ldr r0,[0x0015d618]
  0015d342: movw r1,#0x26e
  0015d346: ldr r6,[r4,#0x0]
  0015d348: add r0,pc
  0015d34a: ldr r0,[r0,#0x0]
  0015d34c: ldr r0,[r0,#0x0]
  0015d34e: blx 0x00072f70
  0015d352: add.w r9,sp,#0x54
  0015d356: mov r1,r0
  0015d358: movs r2,#0x0
  0015d35a: mov r0,r9
  0015d35c: blx 0x0006f028
  0015d360: mov r0,r6
  0015d362: mov r1,r9
  0015d364: blx 0x000747c4
  0015d368: add r0,sp,#0x54
  0015d36a: b 0x0015cdcc
  0015d36c: ldrb.w r0,[r10,#0x93]
  0015d370: cmp r0,#0x0
  0015d372: beq.w 0x0015d542
  0015d376: ldr.w r0,[r10,#0x20]
  0015d37a: mov r1,r8
  0015d37c: mov r2,r11
  0015d37e: blx 0x00074770
  0015d382: cmp r0,#0x1
  0015d384: beq.w 0x0015d5c8
  0015d388: cmp r0,#0x0
  0015d38a: bne.w 0x0015d0f0
  0015d38e: ldr r1,[0x0015d61c]
  0015d390: ldr.w r0,[r10,#0x68]
  0015d394: add r1,pc
  0015d396: ldr r4,[r1,#0x0]
  0015d398: movs r1,#0x0
  0015d39a: strb.w r1,[r10,#0x93]
  0015d39e: strb.w r1,[r10,#0x3c]
  0015d3a2: ldr r6,[r4,#0x0]
  0015d3a4: blx 0x00076960
  0015d3a8: mov r1,r0
  0015d3aa: mov r0,r6
  0015d3ac: blx 0x0007171c
  0015d3b0: ldr r0,[r4,#0x0]
  0015d3b2: blx 0x00071c14
  0015d3b6: ldr.w r1,[r10,#0x68]
  0015d3ba: ldr r1,[r1,#0xc]
  0015d3bc: blx 0x00074ff8
  0015d3c0: ldrd r9,r6,[r10,#0x10]
  0015d3c4: ldr r0,[r4,#0x0]
  0015d3c6: blx 0x00071c14
  0015d3ca: blx 0x00073708
  0015d3ce: mov r2,r0
  0015d3d0: mov r0,r6
  0015d3d2: mov r1,r9
  0015d3d4: blx 0x000761ec
  0015d3d8: mov r0,r10
  0015d3da: blx 0x00076f30
  0015d3de: b 0x0015d0f0
  0015d3e0: blx 0x000771e8
  0015d3e4: b 0x0015ceba
  0015d3e6: blx 0x000771f4
  0015d3ea: b 0x0015ceba
  0015d3ec: blx 0x00077200
  0015d3f0: b 0x0015ceba
  0015d3f2: blx 0x0007720c
  0015d3f6: b 0x0015ceba
  0015d3f8: ldr r0,[0x0015d620]
  0015d3fa: add r0,pc
  0015d3fc: ldr r6,[r0,#0x0]
  0015d3fe: ldr r0,[r6,#0x0]
  0015d400: blx 0x000733d8
  0015d404: cmp r0,r4
  0015d406: blt 0x0015d412
  0015d408: ldrb.w r0,[r10,#0x11e]
  0015d40c: cmp r0,#0x0
  0015d40e: beq.w 0x0015d5d0
  0015d412: ldr r0,[0x0015d624]
  0015d414: add r0,pc
  0015d416: ldr r0,[r0,#0x0]
  0015d418: ldr r0,[r0,#0x0]
  0015d41a: blx 0x000733d8
  0015d41e: cmp r0,r4
  0015d420: bge.w 0x0015d532
  0015d424: ldr.w r0,[r10,#0x84]
  0015d428: ldr.w r1,[r10,#0x8c]
  0015d42c: ldr r0,[r0,#0x10]
  0015d42e: blx 0x0007375c
  0015d432: ldr.w r0,[r10,#0x84]
  0015d436: ldr.w r1,[r10,#0xa0]
  0015d43a: ldr r0,[r0,#0x10]
  0015d43c: blx 0x00076fc0
  0015d440: ldr.w r0,[r10,#0x84]
  0015d444: ldr.w r1,[r10,#0xa4]
  0015d448: ldr r0,[r0,#0x10]
  0015d44a: blx 0x00076234
  0015d44e: ldr r0,[0x0015d628]
  0015d450: ldr.w r1,[r10,#0x98]
  0015d454: add r0,pc
  0015d456: ldr r5,[r0,#0x0]
  0015d458: ldr r0,[r5,#0x0]
  0015d45a: blx 0x000758b0
  0015d45e: ldr.w r0,[r10,#0x14]
  0015d462: movs r6,#0x0
  0015d464: ldr.w r1,[r10,#0x9c]
  0015d468: str.w r6,[r10,#0x8c]
  0015d46c: str.w r6,[r10,#0x94]
  0015d470: str.w r6,[r10,#0x68]
  0015d474: strd r6,r6,[r10,#0xa0]
  0015d478: str.w r1,[r10,#0xa8]
  0015d47c: mov.w r1,#0xffffffff
  0015d480: blx 0x000770b0
  0015d484: strb.w r6,[r10,#0x3c]
  0015d488: strb.w r6,[r10,#0xac]
  0015d48c: strb.w r6,[r10,#0x88]
  0015d490: ldr r0,[r5,#0x0]
  0015d492: blx 0x000733d8
  0015d496: cmp r0,r4
  0015d498: bge 0x0015d52c
  0015d49a: ldrb.w r0,[r10,#0x11e]
  0015d49e: cmp r0,#0x0
  0015d4a0: bne 0x0015d52c
  0015d4a2: ldr r0,[0x0015d62c]
  0015d4a4: movs r1,#0xcb
  0015d4a6: ldr.w r8,[r10,#0x20]
  0015d4aa: add r0,pc
  0015d4ac: ldr.w r9,[r5,#0x0]
  0015d4b0: ldr r0,[r0,#0x0]
  0015d4b2: ldr r0,[r0,#0x0]
  0015d4b4: blx 0x00072f70
  0015d4b8: mov r1,r0
  0015d4ba: add r0,sp,#0xfc
  0015d4bc: movs r2,#0x0
  0015d4be: blx 0x0006f028
  0015d4c2: ldr r0,[0x0015d630]
  0015d4c4: add r0,pc
  0015d4c6: ldr r0,[r0,#0x0]
  0015d4c8: ldr.w r11,[r0,#0x0]
  0015d4cc: ldr r0,[r5,#0x0]
  0015d4ce: blx 0x000733d8
  0015d4d2: subs r2,r4,r0
  0015d4d4: add r0,sp,#0x120
  0015d4d6: mov r1,r11
  0015d4d8: blx 0x00074e54
  0015d4dc: add r0,sp,#0xf0
  0015d4de: add r1,sp,#0x120
  0015d4e0: movs r2,#0x0
  0015d4e2: blx 0x0006f028
  0015d4e6: ldr r1,[0x0015d634]
  0015d4e8: add r1,pc
  0015d4ea: add r0,sp,#0xe4
  0015d4ec: movs r2,#0x0
  0015d4ee: blx 0x0006ee18
  0015d4f2: str r0,[sp,#0x0]
  0015d4f4: add r0,sp,#0x144
  0015d4f6: add r2,sp,#0xfc
  0015d4f8: add r3,sp,#0xf0
  0015d4fa: mov r1,r9
  0015d4fc: blx 0x000739b4
  0015d500: add r1,sp,#0x144
  0015d502: mov r0,r8
  0015d504: blx 0x00074794
  0015d508: ldr.w r9,[sp,#0x2c]
  0015d50c: ldr r0,[0x0015d638]
  0015d50e: add r0,pc
  0015d510: ldr r4,[r0,#0x0]
  0015d512: add r0,sp,#0x144
  0015d514: blx r4
  0015d516: add r0,sp,#0xe4
  0015d518: blx r4
  0015d51a: add r0,sp,#0xf0
  0015d51c: blx r4
  0015d51e: add r0,sp,#0x120
  0015d520: blx r4
  0015d522: add r0,sp,#0xfc
  0015d524: blx r4
  0015d526: movs r0,#0x1
  0015d528: strb.w r0,[r10,#0x3c]
  0015d52c: strb.w r6,[r10,#0x11e]
  0015d530: b 0x0015d5fa
  0015d532: cmp r5,#0x0
  0015d534: bne 0x0015d5fa
  0015d536: ldrb.w r0,[r10,#0x11e]
  0015d53a: cmp r0,#0x0
  0015d53c: bne.w 0x0015d424
  0015d540: b 0x0015d5fa
  0015d542: ldrb.w r0,[r10,#0x90]
  0015d546: cmp r0,#0x0
  0015d548: beq.w 0x0015d0f0
  0015d54c: ldr.w r0,[r10,#0x20]
  0015d550: mov r1,r8
  0015d552: mov r2,r11
  0015d554: blx 0x00074770
  0015d558: mov r1,r0
  0015d55a: ldr r0,[0x0015d63c]
  0015d55c: ldrb.w r2,[r10,#0x11d]
  0015d560: add r0,pc
  0015d562: ldr r0,[r0,#0x0]
  0015d564: str r0,[sp,#0x28]
  0015d566: ldr r0,[r0,#0x0]
  0015d568: ldr.w r3,[r0,#0x114]
  0015d56c: cmp r3,#0x3
  0015d56e: bne 0x0015d644
  0015d570: cmp r2,#0x0
  0015d572: bne 0x0015d646
  0015d574: ldrb.w r2,[r10,#0x91]
  0015d578: cmp r2,#0x0
  0015d57a: beq.w 0x0015d6b4
  0015d57e: cmp r1,#0x0
  0015d580: beq 0x0015d652
  0015d582: cmp r1,#0x1
  0015d584: bne.w 0x0015d0f0
  0015d588: ldr.w r5,[r0,#0x14c]
  0015d58c: blx 0x00071a58
  0015d590: blx 0x000719c8
  0015d594: mov r1,r0
  0015d596: mov r0,r5
  0015d598: blx 0x00071af4
  0015d59c: cmp r0,#0x0
  0015d59e: beq.w 0x0015dbd0
  0015d5a2: ldr r0,[0x0015d640]
  0015d5a4: mov.w r1,#0x148
  0015d5a8: ldr.w r5,[r10,#0x20]
  0015d5ac: add r0,pc
  0015d5ae: ldr r0,[r0,#0x0]
  0015d5b0: ldr r0,[r0,#0x0]
  0015d5b2: blx 0x00072f70
  0015d5b6: mov r1,r0
  0015d5b8: mov r0,r5
  0015d5ba: blx 0x00074794
  0015d5be: movs r0,#0x1
  0015d5c0: movs r6,#0x0
  0015d5c2: strb.w r0,[r10,#0x3c]
  0015d5c6: b 0x0015dbc8
  0015d5c8: movs r0,#0x0
  0015d5ca: strb.w r0,[r10,#0x93]
  0015d5ce: b 0x0015cf60
  0015d5d0: ldr r0,[r6,#0x0]
  0015d5d2: rsbs r1,r4
  0015d5d4: blx 0x0007171c
  0015d5d8: mov r0,r10
  0015d5da: movs r1,#0x0
  0015d5dc: movs r4,#0x0
  0015d5de: blx 0x000751cc
  0015d5e2: ldr.w r0,[r10,#0x14]
  0015d5e6: mov.w r1,#0xffffffff
  0015d5ea: str.w r4,[r10,#0x68]
  0015d5ee: blx 0x000770b0
  0015d5f2: strb.w r4,[r10,#0x11e]
  0015d5f6: strb.w r4,[r10,#0xac]
  0015d5fa: mov r0,r10
  0015d5fc: blx 0x00076f30
  0015d600: movs r6,#0x0
  0015d602: b.w 0x0015c774
  0015d644: cbz r2,0x0015d686
  0015d646: cmp r1,#0x1
  0015d648: beq.w 0x0015d76e
  0015d64c: cmp r1,#0x0
  0015d64e: bne.w 0x0015d0f0
  0015d652: movs r1,#0x0
  0015d654: str r1,[sp,#0x20]
  0015d656: blx 0x00071a58
  0015d65a: mov r5,r0
  0015d65c: ldr.w r0,[r10,#0x10]
  0015d660: movs r1,#0x1
  0015d662: blx 0x000770d4
  0015d666: mov r1,r0
  0015d668: mov r0,r5
  0015d66a: blx 0x000738b8
  0015d66e: ldr r0,[sp,#0x20]
  0015d670: cbz r0,0x0015d6a6
  0015d672: mov r11,r10
  0015d674: ldr r1,[sp,#0x28]
  0015d676: ldr.w r0,[r11,#0x68]!
  0015d67a: ldr r5,[r1,#0x0]
  0015d67c: ldr r0,[r0,#0xc]
  0015d67e: blx 0x00071b00
  0015d682: rsbs r1,r0
  0015d684: b 0x0015d794
  0015d686: ldrb.w r2,[r10,#0x92]
  0015d68a: cmp r2,#0x0
  0015d68c: beq 0x0015d72a
  0015d68e: cmp r1,#0x0
  0015d690: beq.w 0x0015d978
  0015d694: cmp r1,#0x1
  0015d696: bne.w 0x0015d0f0
  0015d69a: movs r2,#0x0
  0015d69c: movs r1,#0x0
  0015d69e: str r1,[sp,#0x20]
  0015d6a0: strb.w r2,[r10,#0x92]
  0015d6a4: b 0x0015d656
  0015d6a6: ldrb.w r0,[r10,#0x11d]
  0015d6aa: cmp r0,#0x0
  0015d6ac: beq 0x0015d776
  0015d6ae: add.w r11,r10,#0x68
  0015d6b2: b 0x0015d79a
  0015d6b4: cmp r1,#0x1
  0015d6b6: beq 0x0015d76e
  0015d6b8: cmp r1,#0x0
  0015d6ba: bne.w 0x0015d0f0
  0015d6be: ldr.w r8,[r10,#0x20]
  0015d6c2: add r0,sp,#0x144
  0015d6c4: ldr r1,[0x0015d9fc]
  0015d6c6: movs r2,#0x0
  0015d6c8: add r1,pc
  0015d6ca: blx 0x0006ee18
  0015d6ce: ldr r0,[0x0015da00]
  0015d6d0: add r0,pc
  0015d6d2: ldr r4,[r0,#0x0]
  0015d6d4: ldr r0,[r4,#0x0]
  0015d6d6: movw r1,#0x147
  0015d6da: blx 0x00072f70
  0015d6de: mov r9,r0
  0015d6e0: ldr r0,[r4,#0x0]
  0015d6e2: mov.w r1,#0x14a
  0015d6e6: blx 0x00072f70
  0015d6ea: mov r5,r0
  0015d6ec: ldr r0,[r4,#0x0]
  0015d6ee: movw r1,#0x14b
  0015d6f2: blx 0x00072f70
  0015d6f6: mov r6,r0
  0015d6f8: ldr r0,[r4,#0x0]
  0015d6fa: mov.w r1,#0x14a
  0015d6fe: blx 0x00072f70
  0015d702: mov.w r1,#0xffffffff
  0015d706: strd r5,r6,[sp,#0x0]
  0015d70a: strd r0,r1,[sp,#0x8]
  0015d70e: mov r0,r8
  0015d710: str r1,[sp,#0x10]
  0015d712: add r1,sp,#0x144
  0015d714: mov r2,r9
  0015d716: movs r3,#0x1
  0015d718: movs r4,#0x1
  0015d71a: blx 0x00075574
  0015d71e: add r0,sp,#0x144
  0015d720: blx 0x0006ee30
  0015d724: strb.w r4,[r10,#0x91]
  0015d728: b 0x0015d76a
  0015d72a: cmp r1,#0x1
  0015d72c: beq 0x0015d76e
  0015d72e: cmp r1,#0x0
  0015d730: bne.w 0x0015d0f0
  0015d734: ldr.w r5,[r10,#0x20]
  0015d738: add r0,sp,#0x144
  0015d73a: ldr r1,[0x0015da04]
  0015d73c: movs r2,#0x0
  0015d73e: add r1,pc
  0015d740: blx 0x0006ee18
  0015d744: ldr r0,[0x0015da08]
  0015d746: add r0,pc
  0015d748: ldr r0,[r0,#0x0]
  0015d74a: ldr r0,[r0,#0x0]
  0015d74c: movs r1,#0xbd
  0015d74e: blx 0x00072f70
  0015d752: mov r2,r0
  0015d754: add r1,sp,#0x144
  0015d756: mov r0,r5
  0015d758: movs r3,#0x1
  0015d75a: movs r4,#0x1
  0015d75c: blx 0x00074ea8
  0015d760: add r0,sp,#0x144
  0015d762: blx 0x0006ee30
  0015d766: strb.w r4,[r10,#0x92]
  0015d76a: movs r6,#0x0
  0015d76c: b 0x0015d1dc
  0015d76e: movs r6,#0x0
  0015d770: strb.w r6,[r10,#0x3c]
  0015d774: b 0x0015dbc8
  0015d776: ldr r0,[sp,#0x28]
  0015d778: ldr r5,[r0,#0x0]
  0015d77a: mov r0,r5
  0015d77c: blx 0x00071a58
  0015d780: blx 0x00071b00
  0015d784: mov r11,r10
  0015d786: mov r6,r0
  0015d788: ldr.w r0,[r11,#0x68]!
  0015d78c: ldr r0,[r0,#0xc]
  0015d78e: blx 0x00071b00
  0015d792: subs r1,r6,r0
  0015d794: mov r0,r5
  0015d796: blx 0x0007171c
  0015d79a: ldr r0,[sp,#0x28]
  0015d79c: ldr r0,[r0,#0x0]
  0015d79e: blx 0x00071a58
  0015d7a2: mov r4,r0
  0015d7a4: blx 0x0007333c
  0015d7a8: mov r8,r0
  0015d7aa: mov r0,r4
  0015d7ac: str r4,[sp,#0x24]
  0015d7ae: blx 0x00073750
  0015d7b2: mov r6,r0
  0015d7b4: ldr r0,[0x0015da0c]
  0015d7b6: add r0,pc
  0015d7b8: ldr r1,[r0,#0x0]
  0015d7ba: ldr.w r0,[r11,#0x0]
  0015d7be: str r1,[sp,#0x18]
  0015d7c0: ldr r4,[r1,#0x0]
  0015d7c2: ldr r0,[r0,#0xc]
  0015d7c4: blx 0x000719c8
  0015d7c8: ldr r1,[r4,#0x4]
  0015d7ca: ldr.w r0,[r1,r0,lsl #0x2]
  0015d7ce: mov.w r1,#0xffffffff
  0015d7d2: blx 0x000719d4
  0015d7d6: mov r5,r0
  0015d7d8: ldr.w r0,[r11,#0x0]
  0015d7dc: ldr r0,[r0,#0xc]
  0015d7de: blx 0x00073db0
  0015d7e2: mov r1,r0
  0015d7e4: mov r0,r5
  0015d7e6: blx 0x000719ec
  0015d7ea: mov r0,r5
  0015d7ec: blx 0x000719f8
  0015d7f0: cbz r6,0x0015d812
  0015d7f2: movs r4,#0x0
  0015d7f4: b 0x0015d80c
  0015d7f6: ldr r0,[r6,#0x4]
  0015d7f8: ldr.w r0,[r0,r4,lsl #0x2]
  0015d7fc: cbz r0,0x0015d80a
  0015d7fe: blx 0x000734c8
  0015d802: mov r1,r0
  0015d804: mov r0,r5
  0015d806: blx 0x00072cdc
  0015d80a: adds r4,#0x1
  0015d80c: ldr r0,[r6,#0x0]
  0015d80e: cmp r4,r0
  0015d810: bcc 0x0015d7f6
  0015d812: cmp.w r8,#0x0
  0015d816: beq 0x0015d848
  0015d818: movs r4,#0x0
  0015d81a: b 0x0015d840
  0015d81c: ldr.w r0,[r8,#0x4]
  0015d820: ldr.w r0,[r0,r4,lsl #0x2]
  0015d824: cbz r0,0x0015d83e
  0015d826: blx 0x000734c8
  0015d82a: mov r6,r0
  0015d82c: mov r0,r5
  0015d82e: mov r1,r6
  0015d830: blx 0x000738c4
  0015d834: cbnz r0,0x0015d83e
  0015d836: mov r0,r5
  0015d838: mov r1,r6
  0015d83a: blx 0x00072cdc
  0015d83e: adds r4,#0x1
  0015d840: ldr.w r0,[r8,#0x0]
  0015d844: cmp r4,r0
  0015d846: bcc 0x0015d81c
  0015d848: ldr.w r0,[r11,#0x0]
  0015d84c: ldr r0,[r0,#0xc]
  0015d84e: blx 0x00074b00
  0015d852: cbz r0,0x0015d880
  0015d854: movs r4,#0x0
  0015d856: b 0x0015d870
  0015d858: ldr.w r0,[r11,#0x0]
  0015d85c: ldr r0,[r0,#0xc]
  0015d85e: blx 0x00074b00
  0015d862: ldr r0,[r0,#0x4]
  0015d864: ldr.w r1,[r0,r4,lsl #0x2]
  0015d868: mov r0,r5
  0015d86a: blx 0x000748d8
  0015d86e: adds r4,#0x1
  0015d870: ldr.w r0,[r11,#0x0]
  0015d874: ldr r0,[r0,#0xc]
  0015d876: blx 0x00074b00
  0015d87a: ldr r0,[r0,#0x0]
  0015d87c: cmp r4,r0
  0015d87e: bcc 0x0015d858
  0015d880: ldr r0,[sp,#0x28]
  0015d882: str r5,[sp,#0x1c]
  0015d884: ldr r0,[r0,#0x0]
  0015d886: blx 0x00071c14
  0015d88a: blx 0x00073708
  0015d88e: mov r9,r0
  0015d890: mov.w r8,#0x0
  0015d894: b 0x0015d8b6
  0015d896: ldr.w r0,[r9,#0x4]
  0015d89a: ldr.w r0,[r0,r8,lsl #0x2]
  0015d89e: blx 0x000719c8
  0015d8a2: mov r6,r0
  0015d8a4: ldr.w r0,[r11,#0x0]
  0015d8a8: ldr r0,[r0,#0xc]
  0015d8aa: blx 0x000719c8
  0015d8ae: cmp r6,r0
  0015d8b0: beq 0x0015d8c4
  0015d8b2: add.w r8,r8,#0x1
  0015d8b6: ldr.w r0,[r9,#0x0]
  0015d8ba: cmp r8,r0
  0015d8bc: bcc 0x0015d896
  0015d8be: ldr r0,[sp,#0x20]
  0015d8c0: cbnz r0,0x0015d8d8
  0015d8c2: b 0x0015d95e
  0015d8c4: ldr.w r1,[r9,#0x4]
  0015d8c8: ldr r2,[sp,#0x20]
  0015d8ca: ldr.w r0,[r1,r8,lsl #0x2]
  0015d8ce: cmp r2,#0x0
  0015d8d0: beq 0x0015d964
  0015d8d2: mov r1,r9
  0015d8d4: blx 0x000734b0
  0015d8d8: ldr r0,[sp,#0x28]
  0015d8da: ldr r5,[sp,#0x24]
  0015d8dc: ldr r1,[sp,#0x18]
  0015d8de: ldr r0,[r0,#0x0]
  0015d8e0: ldr r4,[r1,#0x0]
  0015d8e2: ldr.w r6,[r0,#0x14c]
  0015d8e6: mov r0,r5
  0015d8e8: blx 0x000719c8
  0015d8ec: ldr r1,[r4,#0x4]
  0015d8ee: mov.w r8,#0xffffffff
  0015d8f2: ldr.w r0,[r1,r0,lsl #0x2]
  0015d8f6: mov.w r1,#0xffffffff
  0015d8fa: blx 0x000719d4
  0015d8fe: mov r1,r0
  0015d900: mov r0,r6
  0015d902: blx 0x00075004
  0015d906: mov r0,r5
  0015d908: blx 0x00074b00
  0015d90c: cmp r0,#0x0
  0015d90e: beq 0x0015d9e2
  0015d910: ldrd r5,r8,[sp,#0x24]
  0015d914: movs r4,#0x0
  0015d916: b 0x0015d952
  0015d918: ldr.w r0,[r8,#0x0]
  0015d91c: ldr.w r0,[r0,#0x14c]
  0015d920: blx 0x00073708
  0015d924: mov r6,r0
  0015d926: ldr.w r0,[r8,#0x0]
  0015d92a: ldr.w r0,[r0,#0x14c]
  0015d92e: blx 0x00073708
  0015d932: ldr r0,[r0,#0x0]
  0015d934: ldr r1,[r6,#0x4]
  0015d936: add.w r0,r1,r0, lsl #0x2
  0015d93a: ldr.w r6,[r0,#-0x4]
  0015d93e: mov r0,r5
  0015d940: blx 0x00074b00
  0015d944: ldr r0,[r0,#0x4]
  0015d946: ldr.w r1,[r0,r4,lsl #0x2]
  0015d94a: mov r0,r6
  0015d94c: blx 0x000748d8
  0015d950: adds r4,#0x1
  0015d952: mov r0,r5
  0015d954: blx 0x00074b00
  0015d958: ldr r0,[r0,#0x0]
  0015d95a: cmp r4,r0
  0015d95c: bcc 0x0015d918
  0015d95e: mov.w r8,#0xffffffff
  0015d962: b 0x0015d9e2
  0015d964: cbz r0,0x0015d996
  0015d966: blx 0x00073468
  0015d96a: blx 0x0006eb48
  0015d96e: ldr.w r0,[r9,#0x4]
  0015d972: add.w r0,r0,r8, lsl #0x2
  0015d976: b 0x0015d99a
  0015d978: ldr r0,[0x0015da10]
  0015d97a: movs r1,#0x5
  0015d97c: add r0,pc
  0015d97e: ldr r0,[r0,#0x0]
  0015d980: ldr r0,[r0,#0x0]
  0015d982: blx 0x00075bd4
  0015d986: blx 0x00077218
  0015d98a: movs r6,#0x0
  0015d98c: strb.w r6,[r10,#0x3c]
  0015d990: strb.w r6,[r10,#0x92]
  0015d994: b 0x0015dbc8
  0015d996: add.w r0,r1,r8, lsl #0x2
  0015d99a: movs r1,#0x0
  0015d99c: ldr r5,[sp,#0x24]
  0015d99e: str r1,[r0,#0x0]
  0015d9a0: ldr r0,[sp,#0x18]
  0015d9a2: ldr r4,[r0,#0x0]
  0015d9a4: mov r0,r5
  0015d9a6: blx 0x000719c8
  0015d9aa: ldr r1,[r4,#0x4]
  0015d9ac: ldr.w r0,[r1,r0,lsl #0x2]
  0015d9b0: mov.w r1,#0xffffffff
  0015d9b4: blx 0x000719d4
  0015d9b8: ldr.w r1,[r9,#0x4]
  0015d9bc: str.w r0,[r1,r8,lsl #0x2]
  0015d9c0: ldr.w r0,[r9,#0x4]
  0015d9c4: ldr.w r0,[r0,r8,lsl #0x2]
  0015d9c8: blx 0x000719f8
  0015d9cc: ldr.w r0,[r9,#0x4]
  0015d9d0: ldr.w r6,[r0,r8,lsl #0x2]
  0015d9d4: mov r0,r5
  0015d9d6: blx 0x00073db0
  0015d9da: mov r1,r0
  0015d9dc: mov r0,r6
  0015d9de: blx 0x000719ec
  0015d9e2: ldr r5,[sp,#0x24]
  0015d9e4: mov r0,r5
  0015d9e6: blx 0x00074b00
  0015d9ea: ldr.w r11,[sp,#0x28]
  0015d9ee: cmp.w r8,#0x0
  0015d9f2: blt 0x0015da66
  0015d9f4: cbz r0,0x0015da66
  0015d9f6: movs r4,#0x0
  0015d9f8: b 0x0015da5a
  0015da14: ldrb.w r0,[r10,#0x11d]
  0015da18: cbz r0,0x0015da3e
  0015da1a: ldr.w r0,[r11,#0x0]
  0015da1e: ldr.w r0,[r0,#0x14c]
  0015da22: blx 0x00073708
  0015da26: ldr r0,[r0,#0x4]
  0015da28: ldr.w r6,[r0,r8,lsl #0x2]
  0015da2c: mov r0,r5
  0015da2e: blx 0x00074b00
  0015da32: ldr r0,[r0,#0x4]
  0015da34: ldr.w r1,[r0,r4,lsl #0x2]
  0015da38: mov r0,r6
  0015da3a: blx 0x000748d8
  0015da3e: ldr.w r0,[r9,#0x4]
  0015da42: ldr.w r6,[r0,r8,lsl #0x2]
  0015da46: mov r0,r5
  0015da48: blx 0x00074b00
  0015da4c: ldr r0,[r0,#0x4]
  0015da4e: ldr.w r1,[r0,r4,lsl #0x2]
  0015da52: mov r0,r6
  0015da54: blx 0x000748d8
  0015da58: adds r4,#0x1
  0015da5a: mov r0,r5
  0015da5c: blx 0x00074b00
  0015da60: ldr r0,[r0,#0x0]
  0015da62: cmp r4,r0
  0015da64: bcc 0x0015da14
  0015da66: ldrb.w r0,[r10,#0x11d]
  0015da6a: cbz r0,0x0015da86
  0015da6c: ldr.w r0,[r11,#0x0]
  0015da70: ldr.w r5,[r0,#0x14c]
  0015da74: blx 0x00071c14
  0015da78: blx 0x00073708
  0015da7c: mov r1,r0
  0015da7e: mov r0,r5
  0015da80: movs r2,#0x1
  0015da82: blx 0x000736fc
  0015da86: ldr.w r0,[r11,#0x0]
  0015da8a: ldr r1,[sp,#0x1c]
  0015da8c: blx 0x00073870
  0015da90: ldr.w r0,[r11,#0x0]
  0015da94: blx 0x00071a58
  0015da98: blx 0x00073768
  0015da9c: ldr.w r0,[r11,#0x0]
  0015daa0: ldr.w r5,[r10,#0x14]
  0015daa4: blx 0x00071a58
  0015daa8: mov r1,r0
  0015daaa: mov r0,r5
  0015daac: blx 0x000761e0
  0015dab0: ldr.w r0,[r10,#0x10]
  0015dab4: cbz r0,0x0015dac8
  0015dab6: blx 0x00073420
  0015daba: ldr.w r0,[r10,#0x10]
  0015dabe: cbz r0,0x0015dac8
  0015dac0: blx 0x0007342c
  0015dac4: blx 0x0006eb48
  0015dac8: ldr r4,[sp,#0x28]
  0015daca: movs r6,#0x0
  0015dacc: str.w r6,[r10,#0x10]
  0015dad0: ldr r0,[r4,#0x0]
  0015dad2: blx 0x00071a58
  0015dad6: blx 0x00073750
  0015dada: mov r5,r0
  0015dadc: ldr r0,[r4,#0x0]
  0015dade: blx 0x00071c14
  0015dae2: blx 0x00071830
  0015dae6: mov r1,r0
  0015dae8: mov r0,r5
  0015daea: blx 0x00076ee8
  0015daee: ldr.w r9,[r10,#0x14]
  0015daf2: mov r5,r0
  0015daf4: str.w r0,[r10,#0x10]
  0015daf8: ldr r0,[r4,#0x0]
  0015dafa: blx 0x00071c14
  0015dafe: blx 0x00073708
  0015db02: mov r2,r0
  0015db04: mov r0,r9
  0015db06: mov r1,r5
  0015db08: blx 0x000761ec
  0015db0c: ldr.w r0,[r10,#0x14]
  0015db10: movs r1,#0x0
  0015db12: movs r2,#0x1
  0015db14: mov.w r8,#0x1
  0015db18: blx 0x00076f24
  0015db1c: mov r0,r10
  0015db1e: blx 0x00076f30
  0015db22: ldrb.w r0,[r10,#0x11d]
  0015db26: strb.w r6,[r10,#0x3c]
  0015db2a: cmp r0,#0x0
  0015db2c: bne 0x0015dbc4
  0015db2e: ldr r0,[0x0015ddf8]
  0015db30: movw r1,#0x12f
  0015db34: add r0,pc
  0015db36: ldr r4,[r0,#0x0]
  0015db38: ldr r0,[r4,#0x0]
  0015db3a: blx 0x00072f70
  0015db3e: add r5,sp,#0x144
  0015db40: mov r1,r0
  0015db42: movs r2,#0x0
  0015db44: mov r0,r5
  0015db46: blx 0x0006f028
  0015db4a: ldr r0,[sp,#0x28]
  0015db4c: ldr.w r9,[r0,#0x0]
  0015db50: add r0,sp,#0xb4
  0015db52: mov r1,r5
  0015db54: movs r2,#0x0
  0015db56: blx 0x0006f028
  0015db5a: ldr r5,[r4,#0x0]
  0015db5c: ldr r0,[sp,#0x1c]
  0015db5e: blx 0x000719c8
  0015db62: addw r1,r0,#0x391
  0015db66: mov r0,r5
  0015db68: blx 0x00072f70
  0015db6c: mov r1,r0
  0015db6e: add r0,sp,#0xa8
  0015db70: movs r2,#0x0
  0015db72: blx 0x0006f028
  0015db76: ldr r1,[0x0015ddfc]
  0015db78: add r1,pc
  0015db7a: add r0,sp,#0x9c
  0015db7c: movs r2,#0x0
  0015db7e: blx 0x0006ee18
  0015db82: str r0,[sp,#0x0]
  0015db84: add r0,sp,#0x120
  0015db86: add r2,sp,#0xb4
  0015db88: add r3,sp,#0xa8
  0015db8a: mov r1,r9
  0015db8c: blx 0x000739b4
  0015db90: add r0,sp,#0x144
  0015db92: add r1,sp,#0x120
  0015db94: blx 0x0006f2b0
  0015db98: ldr r0,[0x0015de00]
  0015db9a: add r0,pc
  0015db9c: ldr r4,[r0,#0x0]
  0015db9e: add r0,sp,#0x120
  0015dba0: blx r4
  0015dba2: add r0,sp,#0x9c
  0015dba4: blx r4
  0015dba6: add r0,sp,#0xa8
  0015dba8: blx r4
  0015dbaa: add r0,sp,#0xb4
  0015dbac: blx r4
  0015dbae: ldr.w r0,[r10,#0x20]
  0015dbb2: add r1,sp,#0x144
  0015dbb4: blx 0x00074794
  0015dbb8: movs r0,#0x1
  0015dbba: strb.w r0,[r10,#0x3c]
  0015dbbe: add r0,sp,#0x144
  0015dbc0: blx 0x0006ee30
  0015dbc4: strb.w r8,[r10,#0x0]
  0015dbc8: strh.w r6,[r10,#0x90]
  0015dbcc: b.w 0x0015d1dc
  0015dbd0: ldr.w r0,[r10,#0x68]
  0015dbd4: ldr r0,[r0,#0xc]
  0015dbd6: blx 0x00071b00
  0015dbda: mov r5,r0
  0015dbdc: ldr r0,[sp,#0x28]
  0015dbde: ldr r0,[r0,#0x0]
  0015dbe0: blx 0x000733d8
  0015dbe4: cmp r5,r0
  0015dbe6: ble 0x0015dcc4
  0015dbe8: ldr r0,[0x0015dde4]
  0015dbea: movs r1,#0xcb
  0015dbec: add r0,pc
  0015dbee: ldr.w r9,[r0,#0x0]
  0015dbf2: ldr r0,[sp,#0x28]
  0015dbf4: ldr.w r8,[r0,#0x0]
  0015dbf8: ldr.w r0,[r9,#0x0]
  0015dbfc: blx 0x00072f70
  0015dc00: mov r1,r0
  0015dc02: add r0,sp,#0xd8
  0015dc04: movs r2,#0x0
  0015dc06: blx 0x0006f028
  0015dc0a: ldr r0,[0x0015dde8]
  0015dc0c: ldr.w r1,[r10,#0x68]
  0015dc10: add r0,pc
  0015dc12: ldr r0,[r0,#0x0]
  0015dc14: ldr r6,[r0,#0x0]
  0015dc16: ldr r0,[r1,#0xc]
  0015dc18: blx 0x00071b00
  0015dc1c: mov r5,r0
  0015dc1e: ldr r0,[sp,#0x28]
  0015dc20: ldr r0,[r0,#0x0]
  0015dc22: blx 0x000733d8
  0015dc26: subs r2,r5,r0
  0015dc28: add r0,sp,#0x120
  0015dc2a: mov r1,r6
  0015dc2c: blx 0x00074e54
  0015dc30: add r0,sp,#0xcc
  0015dc32: add r1,sp,#0x120
  0015dc34: movs r2,#0x0
  0015dc36: blx 0x0006f028
  0015dc3a: ldr r1,[0x0015ddec]
  0015dc3c: add r1,pc
  0015dc3e: add r0,sp,#0xc0
  0015dc40: movs r2,#0x0
  0015dc42: blx 0x0006ee18
  0015dc46: str r0,[sp,#0x0]
  0015dc48: add r0,sp,#0x144
  0015dc4a: add r2,sp,#0xd8
  0015dc4c: add r3,sp,#0xcc
  0015dc4e: mov r1,r8
  0015dc50: blx 0x000739b4
  0015dc54: ldr r0,[0x0015ddf0]
  0015dc56: add r0,pc
  0015dc58: ldr r4,[r0,#0x0]
  0015dc5a: add r0,sp,#0xc0
  0015dc5c: blx r4
  0015dc5e: add r0,sp,#0xcc
  0015dc60: blx r4
  0015dc62: add r0,sp,#0x120
  0015dc64: blx r4
  0015dc66: add r0,sp,#0xd8
  0015dc68: blx r4
  0015dc6a: ldr r1,[0x0015ddf4]
  0015dc6c: add r1,pc
  0015dc6e: add r0,sp,#0x108
  0015dc70: movs r2,#0x0
  0015dc72: blx 0x0006ee18
  0015dc76: ldr.w r0,[r9,#0x0]
  0015dc7a: movs r1,#0x7c
  0015dc7c: blx 0x00072f70
  0015dc80: mov r2,r0
  0015dc82: add r0,sp,#0x120
  0015dc84: add r1,sp,#0x108
  0015dc86: blx 0x0006ef98
  0015dc8a: add r0,sp,#0x144
  0015dc8c: add r1,sp,#0x120
  0015dc8e: blx 0x0006ef5c
  0015dc92: add r0,sp,#0x120
  0015dc94: blx 0x0006ee30
  0015dc98: add r0,sp,#0x108
  0015dc9a: blx 0x0006ee30
  0015dc9e: ldr.w r0,[r10,#0x20]
  0015dca2: add r1,sp,#0x144
  0015dca4: movs r2,#0x1
  0015dca6: movs r4,#0x1
  0015dca8: blx 0x000746f8
  0015dcac: add r0,sp,#0x144
  0015dcae: movs r6,#0x0
  0015dcb0: strb.w r4,[r10,#0x3c]
  0015dcb4: strb.w r4,[r10,#0xaf]
  0015dcb8: strh.w r6,[r10,#0x90]
  0015dcbc: blx 0x0006ee30
  0015dcc0: b.w 0x0015d1dc
  0015dcc4: ldr r0,[sp,#0x28]
  0015dcc6: movs r1,#0x1
  0015dcc8: ldr r0,[r0,#0x0]
  0015dcca: b 0x0015d654
```
