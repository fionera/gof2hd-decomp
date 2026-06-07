# ModStation::OnUpdate
elf 0xd9c78 body 3460
Sig: undefined __thiscall OnUpdate(ModStation * this)

## decompile
```c

/* ModStation::OnUpdate() */

void __thiscall ModStation::OnUpdate(ModStation *this)

{
  byte bVar1;
  char cVar2;
  bool bVar3;
  int iVar4;
  uint uVar5;
  char *pcVar6;
  int iVar7;
  void *pvVar8;
  float fVar9;
  float fVar10;
  CutScene *this_00;
  SpaceLounge *this_01;
  uint *puVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  Mission *pMVar14;
  Station *pSVar15;
  DialogueWindow *pDVar16;
  String *pSVar17;
  undefined4 extraout_r1;
  uint uVar18;
  undefined4 extraout_r3;
  undefined4 extraout_r3_00;
  undefined4 extraout_r3_01;
  undefined4 extraout_r3_02;
  undefined4 extraout_r3_03;
  undefined4 extraout_r3_04;
  undefined4 extraout_r3_05;
  undefined4 extraout_r3_06;
  undefined4 extraout_r3_07;
  undefined4 extraout_r3_08;
  undefined4 extraout_r3_09;
  undefined4 extraout_r3_10;
  undefined4 extraout_r3_11;
  undefined4 extraout_r3_12;
  undefined4 extraout_r3_13;
  undefined4 extraout_r3_14;
  undefined4 extraout_r3_15;
  undefined4 extraout_r3_16;
  undefined4 extraout_r3_17;
  undefined4 extraout_r3_18;
  undefined4 extraout_r3_19;
  int *piVar19;
  int *piVar20;
  Achievements *this_02;
  String *pSVar21;
  ScrollTouchBox *this_03;
  int *piVar22;
  uint *puVar23;
  int *piVar24;
  int *piVar25;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float fVar26;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float fVar27;
  float extraout_s0_04;
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
  undefined8 uVar28;
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
  undefined4 extraout_s9;
  undefined4 extraout_s9_00;
  undefined4 extraout_s9_01;
  undefined4 extraout_s9_02;
  undefined4 extraout_s9_03;
  undefined4 extraout_s9_04;
  undefined4 extraout_s9_05;
  undefined4 extraout_s9_06;
  undefined4 extraout_s9_07;
  undefined4 extraout_s9_08;
  undefined4 extraout_s9_09;
  undefined4 extraout_s9_10;
  undefined4 extraout_s9_11;
  undefined4 extraout_s9_12;
  undefined4 extraout_s9_13;
  undefined4 extraout_s9_14;
  undefined4 extraout_s9_15;
  undefined4 extraout_s9_16;
  int local_88;
  int local_84;
  int *local_80;
  int *local_7c;
  String aSStack_78 [60];
  int local_3c;
  
  piVar19 = *(int **)(DAT_000e9d98 + 0xe9c8e);
  local_3c = *piVar19;
  iVar4 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
  local_7c = piVar19;
  if ((iVar4 < 0x97) && (iVar4 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis(), iVar4 < 0)
     ) {
    uVar5 = 0;
  }
  else {
    iVar4 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
    if (iVar4 < 0x97) {
      uVar5 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
    }
    else {
      uVar5 = 0x96;
    }
  }
  piVar19 = &this->field_30;
  uVar18 = *piVar19;
  piVar25 = *(int **)(DAT_000e9d9c + 0xe9cd4);
  this->field_28 = uVar5;
  *piVar19 = uVar18 + uVar5;
  this->field_34 = this->field_34 + ((int)uVar5 >> 0x1f) + (uint)CARRY4(uVar18,uVar5);
  FModSound::updateAll((Vector *)*piVar25,(Vector *)0x0,(Vector *)0x0,(Vector *)0x0);
  piVar20 = *(int **)(DAT_000e9da0 + 0xe9d04);
  Layout::update(*piVar20);
  if (*(char *)((int)&this->m_nStarMapWindowOpen + 2) == '\0') {
    Status::incPlayingTime(CONCAT44(extraout_r1,**(undefined4 **)(DAT_000e9da4 + 0xe9d1a)));
  }
  piVar22 = this[1].field_4;
  piVar24 = *(int **)(DAT_000e9da8 + 0xe9d2c);
  local_80 = piVar20;
  Status::getCredits();
  Layout::formatCredits((int)aSStack_78);
  TouchButton::setText((String *)piVar22);
  AbyssEngine::String::~String(aSStack_78);
  this_02 = (Achievements *)**(undefined4 **)(DAT_000e9dac + 0xe9d5a);
  iVar4 = Status::getCredits();
  Achievements::updateCredits(this_02,iVar4);
  if (((char)this->field_24 != '\0') &&
     (pcVar6 = *(char **)(DAT_000e9db0 + 0xe9d78), *pcVar6 != '\0')) {
    *(undefined1 *)&this[1].field_48 = 1;
    *pcVar6 = '\0';
    if ((EVP_PKEY_CTX *)this->field_74 == (EVP_PKEY_CTX *)0x0) {
      this_01 = operator_new(0x10c);
      fVar27 = (float)SpaceLounge::SpaceLounge(this_01);
      this->field_74 = (int)this_01;
    }
    else {
      SpaceLounge::init((EVP_PKEY_CTX *)this->field_74);
      fVar27 = extraout_s0;
    }
    FModSound::setParamValue((FModSound *)*piVar25,0,*(int *)*piVar25,fVar27);
    fVar27 = (float)FModSound::stop(*piVar25);
    FModSound::play(*piVar25,(Vector *)0x6c,(Vector *)0x0,fVar27);
    *(undefined1 *)((int)&this->field_64 + 1) = 1;
    goto LAB_000ea27c;
  }
  puVar23 = *(uint **)(DAT_000e9dcc + 0xe9dba);
  iVar4 = AbyssEngine::ApplicationManager::GetApplicationData();
  uVar12 = extraout_r3;
  fVar27 = extraout_s2;
  uVar13 = extraout_s9;
  fVar26 = extraout_s0_00;
  fVar9 = extraout_s1;
  if (*(char *)(iVar4 + 0x42) != '\0') {
    pSVar21 = (String *)this->field_70;
    GameText::getText(**(int **)(DAT_000e9df8 + 0xe9ddc));
    fVar26 = (float)ChoiceWindow::set(pSVar21);
    *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 1;
    *(undefined1 *)(iVar4 + 0x42) = 0;
    uVar12 = extraout_r3_00;
    fVar27 = extraout_s2_00;
    uVar13 = extraout_s9_00;
    fVar9 = extraout_s1_00;
  }
  if ((((*(char *)(iVar4 + 0x41) != '\0') && (*(int **)(iVar4 + 0x58) != (int *)0x0)) &&
      (**(int **)(iVar4 + 0x58) != 0)) &&
     ((*(Array **)(iVar4 + 0x48) + -0x32 < (Array *)&DAT_00000005 &&
      (iVar7 = Globals::getInAppPurchaseArrayIndex
                         (**(int **)(DAT_000e9e38 + 0xe9e22),*(Array **)(iVar4 + 0x48)),
      uVar12 = extraout_r3_01, fVar27 = extraout_s2_01, uVar13 = extraout_s9_01,
      fVar26 = extraout_s0_01, fVar9 = extraout_s1_01,
      -1 < *(int *)(*(int *)(*(int *)(iVar4 + 0x60) + 4) + iVar7 * 4))))) {
    Status::changeCredits(*piVar24);
    autosave();
    pSVar21 = (String *)this->field_70;
    GameText::getText(**(int **)(DAT_000e9e78 + 0xe9e56));
    fVar26 = (float)ChoiceWindow::set(pSVar21);
    *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 1;
    *(undefined1 *)(iVar4 + 0x41) = 0;
    uVar12 = extraout_r3_02;
    fVar27 = extraout_s2_02;
    uVar13 = extraout_s9_02;
    fVar9 = extraout_s1_02;
    if (this->field_78 != 0) {
      fVar26 = (float)HangarWindow::hideMessage();
      uVar12 = extraout_r3_03;
      fVar27 = extraout_s2_03;
      uVar13 = extraout_s9_03;
      fVar9 = extraout_s1_03;
    }
    if ((MenuTouchWindow *)this->field_50 != (MenuTouchWindow *)0x0) {
      pvVar8 = (void *)MenuTouchWindow::~MenuTouchWindow((MenuTouchWindow *)this->field_50);
      operator_delete(pvVar8);
      uVar12 = extraout_r3_04;
      fVar27 = extraout_s2_04;
      uVar13 = extraout_s9_04;
      fVar26 = extraout_s0_02;
      fVar9 = extraout_s1_04;
    }
    this->field_50 = 0;
  }
  if ((((char)this->field_64 == '\0') && (*(char *)((int)&this->field_64 + 1) == '\0')) &&
     ((*(char *)((int)&this->field_64 + 2) == '\0' &&
      (((*(char *)((int)&this->field_64 + 3) == '\0' && ((char)this->field_68 == '\0')) &&
       ((char)this->m_nStarMapWindowOpen == '\0')))))) {
    if (this->field_1C != (int *)0x0) {
      NewsTicker::update((int)this->field_1C);
    }
    if (this->field_14 != (int *)0x0) {
      CutScene::process((int)this->field_14);
    }
    local_84 = this->field_28;
    local_88 = -local_84;
    iVar4 = local_88;
    if (*(char *)((int)&this[2].field_C + 2) == '\0') {
      iVar4 = local_84;
    }
    fVar27 = (float)VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::EaseInOut::Increase(fVar27);
    fVar9 = (float)AbyssEngine::EaseInOut::GetValue();
    fVar26 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar10 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar27 = -(fVar9 - fVar10);
    if (0.0 < fVar9 - fVar26) {
      fVar27 = fVar9 - fVar10;
    }
    uVar5 = in_fpscr & 0xfffffff | (uint)(fVar27 == 5.0) << 0x1e | (uint)(5.0 <= fVar27) << 0x1d;
    bVar1 = (byte)(uVar5 >> 0x18);
    if (!(bool)(bVar1 >> 5 & 1) || (bool)(bVar1 >> 6)) {
      bVar1 = *(byte *)((int)&this[2].field_C + 2);
      fVar27 = (float)this[2].field_10;
      piVar20 = *(int **)(DAT_000ea2c4 + 0xe9f70);
      *(byte *)((int)&this[2].field_C + 2) = bVar1 ^ 1;
      iVar4 = AbyssEngine::AERandom::nextInt(*piVar20);
      iVar7 = iVar4 + 0x12;
      if (bVar1 == 0) {
        iVar7 = -0x12 - iVar4;
      }
      fVar9 = (float)VectorSignedToFloat(iVar7,(byte)(uVar5 >> 0x16) & 3);
      AbyssEngine::EaseInOut::SetRange(fVar27 + fVar9,extraout_s1_05);
    }
    piVar20 = &local_84;
    if (*(char *)((int)&this[2].field_C + 1) == '\0') {
      piVar20 = &local_88;
    }
    fVar27 = (float)VectorSignedToFloat(*piVar20,(byte)(uVar5 >> 0x16) & 3);
    AbyssEngine::EaseInOut::Increase(fVar27);
    fVar9 = (float)AbyssEngine::EaseInOut::GetValue();
    fVar26 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar10 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar27 = -(fVar9 - fVar10);
    if (0.0 < fVar9 - fVar26) {
      fVar27 = fVar9 - fVar10;
    }
    uVar5 = uVar5 & 0xfffffff | (uint)(fVar27 == 5.0) << 0x1e | (uint)(5.0 <= fVar27) << 0x1d;
    bVar1 = (byte)(uVar5 >> 0x18);
    if (!(bool)(bVar1 >> 5 & 1) || (bool)(bVar1 >> 6)) {
      bVar1 = *(byte *)((int)&this[2].field_C + 1);
      piVar22 = this[2].field_14;
      piVar20 = *(int **)(DAT_000ea2c8 + 0xea028);
      *(byte *)((int)&this[2].field_C + 1) = bVar1 ^ 1;
      iVar4 = AbyssEngine::AERandom::nextInt(*piVar20);
      iVar7 = iVar4 + 0x1e;
      if (bVar1 == 0) {
        iVar7 = -0x1e - iVar4;
      }
      fVar27 = (float)VectorSignedToFloat(iVar7,(byte)(uVar5 >> 0x16) & 3);
      AbyssEngine::EaseInOut::SetRange((float)piVar22 + fVar27,extraout_s1_06);
    }
    piVar20 = &local_84;
    if (*(char *)((int)&this[2].field_C + 3) == '\0') {
      piVar20 = &local_88;
    }
    fVar27 = (float)VectorSignedToFloat(*piVar20,(byte)(uVar5 >> 0x16) & 3);
    AbyssEngine::EaseInOut::Increase(fVar27);
    fVar9 = (float)AbyssEngine::EaseInOut::GetValue();
    fVar26 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar10 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar27 = -(fVar9 - fVar10);
    if (0.0 < fVar9 - fVar26) {
      fVar27 = fVar9 - fVar10;
    }
    in_fpscr = uVar5 & 0xfffffff | (uint)(fVar27 == 5.0) << 0x1e | (uint)(5.0 <= fVar27) << 0x1d;
    bVar1 = (byte)(in_fpscr >> 0x18);
    if (!(bool)(bVar1 >> 5 & 1) || (bool)(bVar1 >> 6)) {
      bVar1 = *(byte *)((int)&this[2].field_C + 3);
      local_84 = this[2].field_24;
      piVar20 = *(int **)(DAT_000ea2cc + 0xea0de);
      piVar22 = this[2].field_18;
      *(byte *)((int)&this[2].field_C + 3) = bVar1 ^ 1;
      iVar4 = AbyssEngine::AERandom::nextInt(*piVar20);
      iVar7 = iVar4 + 0x32;
      if (bVar1 == 0) {
        iVar7 = -0x32 - iVar4;
      }
      fVar27 = (float)VectorSignedToFloat(iVar7,(byte)(in_fpscr >> 0x16) & 3);
      AbyssEngine::EaseInOut::SetRange((float)piVar22 + fVar27,extraout_s1_07);
    }
    uVar5 = **(uint **)(DAT_000ea2d0 + 0xea124);
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    fVar27 = (float)AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
    fVar26 = (float)AbyssEngine::AEMath::MatrixSetTranslation
                              ((Matrix *)aSStack_78,fVar27,extraout_s1_08,extraout_s2_05);
    uVar12 = extraout_r3_05;
    fVar27 = extraout_s2_06;
    uVar13 = extraout_s9_05;
    fVar9 = extraout_s1_09;
  }
  if (this->field_34 < (int)(uint)((uint)*piVar19 < 1000)) goto LAB_000ea27c;
  if ((char)this->m_nStarMapWindowOpen == '\0') {
    if (*(char *)((int)&this->field_68 + 1) == '\0') {
      if (*(char *)((int)&this->field_64 + 3) == '\0') {
        if (*(char *)((int)&this->field_64 + 2) == '\0') {
          if ((char)this->field_64 == '\0') {
            if (*(char *)((int)&this->field_64 + 1) == '\0') {
              if ((char)this->field_68 != '\0') {
                fVar26 = (float)StatusWindow::update(this->field_7C);
                uVar12 = extraout_r3_13;
                fVar27 = extraout_s2_14;
                uVar13 = extraout_s9_13;
                fVar9 = extraout_s1_17;
              }
            }
            else {
              fVar26 = (float)SpaceLounge::update(this->field_74);
              uVar12 = extraout_r3_12;
              fVar27 = extraout_s2_13;
              uVar13 = extraout_s9_12;
              fVar9 = extraout_s1_16;
            }
          }
          else {
            fVar26 = (float)MissionsWindow::update((int)this->m_pDialogueWindow);
            uVar12 = extraout_r3_11;
            fVar27 = extraout_s2_12;
            uVar13 = extraout_s9_11;
            fVar9 = extraout_s1_15;
          }
        }
        else {
          fVar26 = (float)HangarWindow::update(this->field_78);
          uVar12 = extraout_r3_10;
          fVar27 = extraout_s2_11;
          uVar13 = extraout_s9_10;
          fVar9 = extraout_s1_14;
        }
      }
      else {
        fVar26 = (float)StarMap::update((StarMap *)this->field_10,this->field_28);
        uVar12 = extraout_r3_08;
        fVar27 = extraout_s2_09;
        uVar13 = extraout_s9_08;
        fVar9 = extraout_s1_12;
      }
    }
    else {
      fVar26 = (float)DialogueWindow::update(this->field_84);
      uVar12 = extraout_r3_07;
      fVar27 = extraout_s2_08;
      uVar13 = extraout_s9_07;
      fVar9 = extraout_s1_11;
    }
  }
  else {
    iVar4 = this[1].field_24;
    if ((iVar4 < DAT_000ea2d4 + 1) && (DAT_000ea2d4 < this->field_28 + iVar4)) {
      if ((CutScene *)this->field_14 != (CutScene *)0x0) {
        pvVar8 = (void *)CutScene::~CutScene((CutScene *)this->field_14);
        operator_delete(pvVar8);
      }
      this->field_14 = (int *)0x0;
      this_00 = operator_new(0xa0);
      CutScene::CutScene(this_00,2);
      this->field_14 = (int *)this_00;
      CutScene::initialize();
      this->field_14[9] = DAT_000ea2d8;
      iVar4 = this[1].field_24;
      *(undefined1 *)&this->m_nStarMapWindowOpen = 1;
    }
    piVar19 = this->field_14;
    this[1].field_24 = iVar4 + this->field_28;
    CutScene::update((int)piVar19);
    iVar4 = Radio::lastMessageShown();
    uVar12 = extraout_r3_06;
    fVar27 = extraout_s2_07;
    uVar13 = extraout_s9_06;
    fVar26 = extraout_s0_03;
    fVar9 = extraout_s1_10;
    if (iVar4 != 0) {
      piVar19 = *(int **)(DAT_000ea2dc + 0xea1d8);
      fVar26 = (float)VectorSignedToFloat(*piVar19 / 2,(byte)(in_fpscr >> 0x16) & 3);
      fVar10 = (float)this[1].field_78;
      iVar4 = this->field_28;
      uVar5 = in_fpscr & 0xfffffff;
      uVar18 = uVar5 | (uint)(fVar10 < fVar26) << 0x1f | (uint)(fVar10 == fVar26) << 0x1e;
      in_fpscr = uVar18 | (uint)(NAN(fVar10) || NAN(fVar26)) << 0x1c;
      bVar1 = (byte)(uVar18 >> 0x18);
      if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
        fVar27 = (float)VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
        fVar27 = fVar10 + fVar27 * DAT_000ea2e0;
        in_fpscr = uVar5 | (uint)(fVar27 == fVar26) << 0x1e | (uint)(fVar26 <= fVar27) << 0x1d;
        bVar1 = (byte)(in_fpscr >> 0x18);
        if (!(bool)(bVar1 >> 5 & 1) || (bool)(bVar1 >> 6)) {
          this[1].field_78 = (int)fVar27;
          this[1].field_28 = 4000;
          fVar10 = fVar27;
        }
      }
      if (this[1].field_28 < 1) {
        fVar27 = (float)VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
        this_03 = (ScrollTouchBox *)this[1].field_8;
        fVar26 = (float)this[1].field_74;
        puVar11 = *(uint **)(DAT_000ea660 + 0xea2fa);
        fVar10 = fVar10 + fVar27 * DAT_000ea2e0;
        this[1].field_78 = (int)fVar10;
        uVar12 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
        fVar27 = (float)VectorSignedToFloat(uVar12,(byte)(in_fpscr >> 0x16) & 3);
        fVar9 = (float)VectorSignedToFloat(*piVar19 / 2,(byte)(in_fpscr >> 0x16) & 3);
        fVar26 = (float)ScrollTouchBox::setPosition
                                  (this_03,(int)fVar26,(int)(fVar10 + fVar27 + fVar9));
        uVar12 = extraout_r3_09;
        fVar27 = extraout_s2_10;
        uVar13 = extraout_s9_09;
        fVar9 = extraout_s1_13;
      }
      else {
        this[1].field_28 = this[1].field_28 - iVar4;
      }
    }
    if (DAT_000ea664 <= this[1].field_24) {
      *(undefined1 *)&this->field_24 = 0;
      Status::nextCampaignMission(SUB41(*piVar24,0));
      uVar5 = *puVar23;
      **(undefined4 **)(DAT_000ea668 + 0xea366) = 0;
      AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uVar5);
      *(undefined1 *)&this->m_nStarMapWindowOpen = 0;
      goto LAB_000ea27c;
    }
  }
  if (*(char *)((int)&this->m_nStarMapWindowOpen + 3) != '\0') {
    fVar26 = (float)ChoiceWindow::update(this->field_70);
    uVar12 = extraout_r3_14;
    fVar27 = extraout_s2_15;
    uVar13 = extraout_s9_14;
    fVar9 = extraout_s1_18;
  }
  if (*(char *)((int)&this->m_nStarMapWindowOpen + 2) != '\0') {
    fVar26 = (float)MenuTouchWindow::update((MenuTouchWindow *)this->field_50,this->field_28);
    uVar12 = extraout_r3_15;
    fVar27 = extraout_s2_16;
    uVar13 = extraout_s9_15;
    fVar9 = extraout_s1_19;
  }
  if (((*(char *)((int)&this->m_nStarMapWindowOpen + 3) == '\0') &&
      (*(char *)((int)&this->field_68 + 1) == '\0')) && ((char)this[1].field_20 == '\0')) {
    if (*(char *)((int)&this[1].field_48 + 2) == '\0') {
      checkPendingProducts();
      *(undefined1 *)((int)&this[1].field_48 + 2) = 1;
      uVar12 = extraout_r3_16;
    }
    if (*(char *)((int)&this[1].field_48 + 1) == '\0') {
      checkMedals();
      *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
      uVar12 = extraout_r3_17;
    }
    if (((*(char *)((int)&this->field_68 + 2) == '\0') &&
        (*(char *)((int)&this->field_6C + 2) != '\0')) &&
       (iVar4 = *(int *)(DAT_000ea66c + 0xea43c), *(char *)(iVar4 + 0x2a) == '\0')) {
      pSVar21 = (String *)this->field_70;
      GameText::getText(**(int **)(DAT_000ea670 + 0xea450));
      ChoiceWindow::set(pSVar21);
      *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 1;
      *(undefined1 *)(iVar4 + 0x2a) = 1;
      *(undefined1 *)((int)&this->field_6C + 2) = 0;
      uVar12 = extraout_r3_18;
    }
    bVar3 = false;
    iVar4 = *piVar24;
    if (*(char *)((int)&this->field_64 + 1) == '\0') {
      uVar13 = 0;
    }
    else {
      uVar13 = SpaceLounge::introFinished();
      uVar12 = extraout_r3_19;
    }
    pMVar14 = (Mission *)Status::missionCompleted(SUB41(iVar4,0),true,CONCAT44(uVar12,uVar13));
    iVar4 = Status::getCurrentCampaignMission();
    if ((pMVar14 == (Mission *)0x0) && (iVar4 == 0x74)) {
      if ((*(char *)((int)&this->field_64 + 1) != '\0') &&
         (iVar4 = SpaceLounge::introFinished(), iVar4 != 0)) {
        Status::getSystem();
        iVar4 = SolarSystem::getIndex();
        if (iVar4 == 0x12) {
          Status::getCampaignMission();
          uVar5 = Mission::getStatusValue();
          pSVar15 = (Station *)Status::getStation();
          iVar4 = Station::getIndex(pSVar15);
          bVar3 = true;
          if ((1 << (iVar4 - 0x5aU & 0xff) & uVar5) == 0) {
            iVar4 = Status::getCampaignMission();
            Status::getCampaignMission();
            Mission::getStatusValue();
            pSVar15 = (Station *)Status::getStation();
            Station::getIndex(pSVar15);
            Mission::setStatusValue(iVar4);
            goto LAB_000ea4ee;
          }
        }
      }
      bVar3 = false;
    }
LAB_000ea4ee:
    iVar4 = Status::getCurrentCampaignMission();
    if ((iVar4 < 0x94) || (iVar4 = Status::getCurrentCampaignMission(), 0x97 < iVar4)) {
LAB_000ea6ae:
      if (bVar3) {
        pSVar15 = (Station *)Status::getStation();
        iVar4 = Station::getIndex(pSVar15);
        iVar7 = iVar4 + -0x5a;
        if (iVar4 == 0x5e) {
          iVar7 = 3;
        }
        FModSound::play(*piVar25,(Vector *)(iVar7 + 0x619),(Vector *)0x0,extraout_s0_04);
        iVar4 = 0x3a;
        if (iVar7 == 1) {
          iVar4 = 0x39;
        }
        if (iVar7 == 0 || iVar7 == 2) {
          iVar4 = 0;
        }
        pDVar16 = operator_new(0x74);
        piVar19 = *(int **)(DAT_000eaaa8 + 0xea716);
        pSVar21 = (String *)GameText::getText(*piVar19);
        pSVar17 = (String *)GameText::getText(*piVar19);
        DialogueWindow::DialogueWindow
                  (pDVar16,pSVar21,pSVar17,*(int **)(DAT_000eaaac + 0xea73c + iVar4 * 4));
        *(undefined1 *)((int)&this->field_68 + 1) = 1;
        this->field_84 = (int)pDVar16;
      }
      else if (pMVar14 == (Mission *)0x0) {
        pMVar14 = (Mission *)Status::missionFailed((Status *)*piVar24,true,0);
        if (pMVar14 != (Mission *)0x0) {
          pDVar16 = operator_new(0x74);
          DialogueWindow::DialogueWindow(pDVar16,pMVar14,(Level *)0x0,2);
          *(undefined1 *)((int)&this->field_68 + 1) = 1;
          this->field_84 = (int)pDVar16;
          Status::removeMission((Mission *)*piVar24);
          iVar4 = Mission::getType();
          if (iVar4 == 0xd) {
            *(undefined2 *)(*piVar24 + 0xf0) = 0;
            autosave();
          }
        }
      }
      else {
        this[1].field_C = (int)pMVar14;
        pDVar16 = operator_new(0x74);
        DialogueWindow::DialogueWindow(pDVar16,pMVar14,(Level *)0x0,1);
        *(undefined1 *)((int)&this->field_68 + 1) = 1;
        this->field_84 = (int)pDVar16;
        iVar4 = Mission::getType();
        if (iVar4 == 0xd) {
          *(undefined2 *)(*piVar24 + 0xf0) = 0;
          autosave();
        }
        if (this->field_74 != 0) {
          SpaceLounge::setHangarUpdate(SUB41(this->field_74,0));
        }
      }
    }
    else {
      pSVar15 = (Station *)Status::getStation();
      iVar4 = Station::getIndex(pSVar15);
      if (iVar4 == 0x42) {
        uVar5 = 2;
      }
      else if (iVar4 == 0x37) {
        uVar5 = 1;
      }
      else if (iVar4 == 9) {
        uVar5 = 4;
      }
      else {
        uVar5 = 0;
      }
      cVar2 = *(char *)((int)&this->field_64 + 1);
      if (pMVar14 == (Mission *)0x0) {
        if (((((cVar2 != '\0') && (iVar7 = SpaceLounge::introFinished(), iVar7 != 0)) &&
             (iVar7 = Status::getCurrentCampaignMission(), 0x93 < iVar7)) &&
            (iVar7 = Status::getCurrentCampaignMission(), iVar7 < 0x97)) &&
           ((iVar4 == 9 || iVar4 == 0x37 || (iVar4 == 0x42)))) {
          Status::getCampaignMission();
          uVar18 = Mission::getStatusValue();
          if ((uVar18 & uVar5) == 0) {
            iVar4 = Status::getCampaignMission();
            Status::getCampaignMission();
            Mission::getStatusValue();
            Mission::setStatusValue(iVar4);
            pSVar15 = (Station *)Status::getStation();
            Station::getIndex(pSVar15);
            pDVar16 = operator_new(0x74);
            DialogueWindow::DialogueWindow(pDVar16);
            this->field_84 = (int)pDVar16;
            pMVar14 = operator_new(0x78);
            Mission::Mission(pMVar14,0xa0,0,-1);
            Mission::setCampaignMission(SUB41(pMVar14,0));
            pDVar16 = (DialogueWindow *)this->field_84;
            goto LAB_000ea59c;
          }
        }
        goto LAB_000ea6ae;
      }
      if (((cVar2 == '\0') || (iVar7 = SpaceLounge::introFinished(), iVar7 == 0)) ||
         ((iVar7 = Status::getCurrentCampaignMission(), iVar7 < 0x94 ||
          ((iVar7 = Status::getCurrentCampaignMission(), iVar4 != 0x60 || (0x96 < iVar7))))))
      goto LAB_000ea6ae;
      Status::setCurrentCampaignMission(*piVar24);
      pDVar16 = operator_new(0x74);
      DialogueWindow::DialogueWindow(pDVar16);
      this[1].field_C = (int)pMVar14;
      this->field_84 = (int)pDVar16;
LAB_000ea59c:
      DialogueWindow::set((Mission *)pDVar16,(int)pMVar14,1);
      *(undefined1 *)((int)&this->field_68 + 1) = 1;
    }
    fVar26 = (float)checkHints();
    fVar27 = extraout_s2_17;
    uVar13 = extraout_s9_16;
    fVar9 = extraout_s1_20;
  }
  if ((char)this[1].field_70 == '\0') {
    fVar27 = (float)VectorSignedToFloat(this->field_28,(byte)(in_fpscr >> 0x16) & 3);
    uVar28 = FloatVectorMin(CONCAT44(fVar9,fVar27 * DAT_000eaa90),CONCAT44(uVar13,0x3f800000),2,0x20
                           );
    fVar27 = (float)this[1].field_64 * (float)uVar28;
    fVar9 = (float)((ulonglong)uVar28 >> 0x20);
    fVar26 = (float)this[1].field_68 * fVar27;
    fVar27 = -((float)this[1].field_68 * fVar27);
    if (0.0 < fVar26) {
      fVar27 = fVar26;
    }
    this[1].field_68 = (int)fVar26;
    uVar5 = in_fpscr & 0xfffffff | (uint)(fVar27 < 1.0) << 0x1f | (uint)(fVar27 == 1.0) << 0x1e;
    in_fpscr = uVar5 | (uint)NAN(fVar27) << 0x1c;
    bVar1 = (byte)(uVar5 >> 0x18);
    if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
      fVar27 = (float)VectorSignedToFloat(this[1].field_54,(byte)(in_fpscr >> 0x16) & 3);
      fVar26 = (float)(int)(fVar26 + fVar27);
      this[1].field_54 = (int)fVar26;
    }
  }
  fVar10 = DAT_000eaa94;
  if ((this->field_14 != (int *)0x0) && ((char)this->m_nStarMapWindowOpen == '\0')) {
    fVar26 = (float)VectorSignedToFloat(this[1].field_54,(byte)(in_fpscr >> 0x16) & 3);
    fVar26 = fVar26 / DAT_000eaa94;
    this->field_14[1] = (int)fVar26;
    fVar27 = fVar10;
  }
  if (*(char *)&this[1].field_88 != '\0') {
    iVar4 = this[1].field_84;
    if (iVar4 < *(int *)(*local_80 + 0xc)) {
      pDVar16 = this[1].m_pDialogueWindow;
      if ((int)pDVar16 < 100) {
        AEGeometry::translate(fVar26,fVar9,fVar27);
        fVar27 = -10.0;
      }
      else {
        if (199 < (int)pDVar16) {
          if ((int)pDVar16 < 300) {
            AEGeometry::rotate(fVar26,fVar9,fVar27);
            piVar19 = this->field_14;
            fVar27 = DAT_000eaa9c;
          }
          else {
            if (399 < (int)pDVar16) goto LAB_000ea27c;
            AEGeometry::rotate(fVar26,fVar9,fVar27);
            piVar19 = this->field_14;
            fVar27 = DAT_000eaa98;
          }
          piVar19[5] = (int)((float)piVar19[5] + fVar27);
          goto LAB_000ea27c;
        }
        AEGeometry::translate(fVar26,fVar9,fVar27);
        fVar27 = 10.0;
      }
      this->field_14[3] = (int)((float)this->field_14[3] + fVar27);
    }
    else {
      pDVar16 = this[1].m_pDialogueWindow;
      if (iVar4 < **(int **)(DAT_000eaab0 + 0xea8b0) + *(int *)(*local_80 + 0x10) * -2) {
        if ((int)pDVar16 < 0x46) {
          AEGeometry::rotate(fVar26,fVar9,fVar27);
          piVar19 = this->field_14;
          fVar27 = DAT_000eaa9c;
        }
        else {
          if ((int)pDVar16 <= **(int **)(DAT_000eaab4 + 0xea93a) + -0x46) {
            if ((100 < (int)pDVar16) && ((int)pDVar16 < **(int **)(DAT_000eaab4 + 0xea93a) + -100))
            {
              if (iVar4 < **(int **)(DAT_000eaab0 + 0xea8b0) / 2) {
                fVar27 = (float)VectorSignedToFloat(-this->field_28,(byte)(in_fpscr >> 0x16) & 3);
                AEGeometry::moveForward(fVar27);
                fVar27 = (float)VectorSignedToFloat(this->field_28,(byte)(in_fpscr >> 0x16) & 3);
                piVar19 = this->field_14;
                fVar27 = (float)piVar19[4] - fVar27;
              }
              else {
                fVar27 = (float)VectorSignedToFloat(this->field_28,(byte)(in_fpscr >> 0x16) & 3);
                AEGeometry::moveForward(fVar27);
                fVar27 = (float)VectorSignedToFloat(this->field_28,(byte)(in_fpscr >> 0x16) & 3);
                piVar19 = this->field_14;
                fVar27 = (float)piVar19[4] + fVar27;
              }
              piVar19[4] = (int)fVar27;
            }
            goto LAB_000ea27c;
          }
          AEGeometry::rotate(fVar26,fVar9,fVar27);
          piVar19 = this->field_14;
          fVar27 = DAT_000eaa98;
        }
        piVar19[6] = (int)((float)piVar19[6] + fVar27);
      }
      else {
        if ((int)pDVar16 < 0x46) {
          fVar26 = (float)VectorSignedToFloat(-this->field_28,(byte)(in_fpscr >> 0x16) & 3);
          AEGeometry::translate(fVar26,fVar9,fVar27);
          fVar27 = (float)VectorSignedToFloat(this->field_28,(byte)(in_fpscr >> 0x16) & 3);
          piVar19 = this->field_14;
          fVar27 = (float)piVar19[2] - fVar27;
        }
        else {
          if ((int)pDVar16 <= **(int **)(DAT_000eaab8 + 0xea988) + -0x46) goto LAB_000ea27c;
          fVar26 = (float)VectorSignedToFloat(this->field_28,(byte)(in_fpscr >> 0x16) & 3);
          AEGeometry::translate(fVar26,fVar9,fVar27);
          fVar27 = (float)VectorSignedToFloat(this->field_28,(byte)(in_fpscr >> 0x16) & 3);
          piVar19 = this->field_14;
          fVar27 = (float)piVar19[2] + fVar27;
        }
        piVar19[2] = (int)fVar27;
      }
    }
  }
LAB_000ea27c:
  if (*local_7c != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e9c78: push {r4,r5,r6,r7,lr}
  000e9c7a: add r7,sp,#0xc
  000e9c7c: push {r7,r8,r9,r10,r11}
  000e9c80: vpush {d8,d9}
  000e9c84: sub sp,#0x60
  000e9c86: mov r11,r0
  000e9c88: ldr r0,[0x000e9d98]
  000e9c8a: add r0,pc
  000e9c8c: ldr r4,[r0,#0x0]
  000e9c8e: ldr r0,[r4,#0x0]
  000e9c90: str r0,[sp,#0x5c]
  000e9c92: ldr.w r0,[r11,#0x8]
  000e9c96: blx 0x00070e4c
  000e9c9a: cmp r0,#0x96
  000e9c9c: str r4,[sp,#0x1c]
  000e9c9e: bgt 0x000e9cac
  000e9ca0: ldr.w r0,[r11,#0x8]
  000e9ca4: blx 0x00070e4c
  000e9ca8: cmp r0,#0x0
  000e9caa: blt 0x000e9cc6
  000e9cac: ldr.w r0,[r11,#0x8]
  000e9cb0: blx 0x00070e4c
  000e9cb4: cmp r0,#0x96
  000e9cb6: ble 0x000e9cbc
  000e9cb8: movs r0,#0x96
  000e9cba: b 0x000e9cc8
  000e9cbc: ldr.w r0,[r11,#0x8]
  000e9cc0: blx 0x00070e4c
  000e9cc4: b 0x000e9cc8
  000e9cc6: movs r0,#0x0
  000e9cc8: mov r9,r11
  000e9cca: ldr r1,[0x000e9d9c]
  000e9ccc: ldr.w r2,[r9,#0x30]!
  000e9cd0: add r1,pc
  000e9cd2: ldr.w r3,[r9,#0x4]
  000e9cd6: ldr.w r10,[r1,#0x0]
  000e9cda: adds r1,r2,r0
  000e9cdc: str.w r0,[r9,#-0x8]
  000e9ce0: adc.w r0,r3,r0, asr #0x1f
  000e9ce4: movs r2,#0x0
  000e9ce6: movs r3,#0x0
  000e9ce8: strd r1,r0,[r9,#0x0]
  000e9cec: movs r1,#0x0
  000e9cee: ldr.w r0,[r10,#0x0]
  000e9cf2: str r1,[sp,#0x0]
  000e9cf4: movs r1,#0x0
  000e9cf6: blx 0x0007543c
  000e9cfa: ldr r0,[0x000e9da0]
  000e9cfc: ldr.w r1,[r9,#-0x8]
  000e9d00: add r0,pc
  000e9d02: ldr r4,[r0,#0x0]
  000e9d04: ldr r0,[r4,#0x0]
  000e9d06: blx 0x00075448
  000e9d0a: ldrb.w r0,[r9,#0x32]
  000e9d0e: cbnz r0,0x000e9d22
  000e9d10: ldr r0,[0x000e9da4]
  000e9d12: ldr.w r2,[r11,#0x28]
  000e9d16: add r0,pc
  000e9d18: ldr r0,[r0,#0x0]
  000e9d1a: asrs r3,r2,#0x1f
  000e9d1c: ldr r0,[r0,#0x0]
  000e9d1e: blx 0x00075454
  000e9d22: ldr r0,[0x000e9da8]
  000e9d24: ldr.w r5,[r11,#0x94]
  000e9d28: add r0,pc
  000e9d2a: str r4,[sp,#0x18]
  000e9d2c: ldr r6,[r4,#0x0]
  000e9d2e: ldr.w r8,[r0,#0x0]
  000e9d32: ldr.w r0,[r8,#0x0]
  000e9d36: blx 0x000733d8
  000e9d3a: add r4,sp,#0x20
  000e9d3c: mov r2,r0
  000e9d3e: mov r1,r6
  000e9d40: mov r0,r4
  000e9d42: blx 0x00074e54
  000e9d46: mov r0,r5
  000e9d48: mov r1,r4
  000e9d4a: blx 0x00075460
  000e9d4e: add r0,sp,#0x20
  000e9d50: blx 0x0006ee30
  000e9d54: ldr r0,[0x000e9dac]
  000e9d56: add r0,pc
  000e9d58: ldr r0,[r0,#0x0]
  000e9d5a: ldr r4,[r0,#0x0]
  000e9d5c: ldr.w r0,[r8,#0x0]
  000e9d60: blx 0x000733d8
  000e9d64: mov r1,r0
  000e9d66: mov r0,r4
  000e9d68: blx 0x0007546c
  000e9d6c: ldrb.w r0,[r11,#0x24]
  000e9d70: cbz r0,0x000e9db4
  000e9d72: ldr r0,[0x000e9db0]
  000e9d74: add r0,pc
  000e9d76: ldr r0,[r0,#0x0]
  000e9d78: ldrb r1,[r0,#0x0]
  000e9d7a: cbz r1,0x000e9db4
  000e9d7c: movs r1,#0x1
  000e9d7e: strb.w r1,[r11,#0xd8]
  000e9d82: movs r1,#0x0
  000e9d84: strb r1,[r0,#0x0]
  000e9d86: ldr.w r0,[r11,#0x74]
  000e9d8a: cmp r0,#0x0
  000e9d8c: beq.w 0x000ea238
  000e9d90: blx 0x000753dc
  000e9d94: b 0x000ea24a
  000e9db4: ldr r0,[0x000e9dcc]
  000e9db6: add r0,pc
  000e9db8: ldr r6,[r0,#0x0]
  000e9dba: ldr r0,[r6,#0x0]
  000e9dbc: blx 0x00071704
  000e9dc0: mov r5,r0
  000e9dc2: ldrb.w r0,[r0,#0x42]
  000e9dc6: cbz r0,0x000e9dfc
  000e9dc8: b 0x000e9dd0
  000e9dd0: ldr r0,[0x000e9df8]
  000e9dd2: movs r1,#0x64
  000e9dd4: ldr.w r4,[r11,#0x70]
  000e9dd8: add r0,pc
  000e9dda: ldr r0,[r0,#0x0]
  000e9ddc: ldr r0,[r0,#0x0]
  000e9dde: blx 0x00072f70
  000e9de2: mov r1,r0
  000e9de4: mov r0,r4
  000e9de6: blx 0x00074794
  000e9dea: movs r0,#0x1
  000e9dec: strb.w r0,[r11,#0x63]
  000e9df0: movs r0,#0x0
  000e9df2: strb.w r0,[r5,#0x42]
  000e9df6: b 0x000e9dfc
  000e9dfc: ldrb.w r0,[r5,#0x41]
  000e9e00: cmp r0,#0x0
  000e9e02: beq 0x000e9e94
  000e9e04: ldr r0,[r5,#0x58]
  000e9e06: cmp r0,#0x0
  000e9e08: beq 0x000e9e94
  000e9e0a: ldr r0,[r0,#0x0]
  000e9e0c: cmp r0,#0x0
  000e9e0e: beq 0x000e9e94
  000e9e10: ldr r1,[r5,#0x48]
  000e9e12: sub.w r0,r1,#0x32
  000e9e16: cmp r0,#0x4
  000e9e18: bhi 0x000e9e94
  000e9e1a: ldr r0,[0x000e9e38]
  000e9e1c: ldr r2,[r5,#0x54]
  000e9e1e: add r0,pc
  000e9e20: ldr r0,[r0,#0x0]
  000e9e22: ldr r0,[r0,#0x0]
  000e9e24: blx 0x00075478
  000e9e28: ldr r1,[r5,#0x60]
  000e9e2a: ldr r1,[r1,#0x4]
  000e9e2c: ldr.w r1,[r1,r0,lsl #0x2]
  000e9e30: cmp r1,#0x0
  000e9e32: blt 0x000e9e94
  000e9e34: b 0x000e9e3c
  000e9e3c: ldr.w r0,[r8,#0x0]
  000e9e40: blx 0x0007171c
  000e9e44: mov r0,r11
  000e9e46: blx 0x00074f8c
  000e9e4a: ldr r0,[0x000e9e78]
  000e9e4c: movs r1,#0x7d
  000e9e4e: ldr.w r4,[r11,#0x70]
  000e9e52: add r0,pc
  000e9e54: ldr r0,[r0,#0x0]
  000e9e56: ldr r0,[r0,#0x0]
  000e9e58: blx 0x00072f70
  000e9e5c: mov r1,r0
  000e9e5e: mov r0,r4
  000e9e60: blx 0x00074794
  000e9e64: movs r0,#0x1
  000e9e66: strb.w r0,[r11,#0x63]
  000e9e6a: movs r0,#0x0
  000e9e6c: strb.w r0,[r5,#0x41]
  000e9e70: ldr.w r0,[r11,#0x78]
  000e9e74: cbz r0,0x000e9e80
  000e9e76: b 0x000e9e7c
  000e9e7c: blx 0x00075484
  000e9e80: ldr.w r0,[r11,#0x50]
  000e9e84: cbz r0,0x000e9e8e
  000e9e86: blx 0x0007537c
  000e9e8a: blx 0x0006eb48
  000e9e8e: movs r0,#0x0
  000e9e90: str.w r0,[r11,#0x50]
  000e9e94: ldrb.w r0,[r11,#0x64]
  000e9e98: cmp r0,#0x0
  000e9e9a: bne.w 0x000ea144
  000e9e9e: ldrb.w r0,[r11,#0x65]
  000e9ea2: cmp r0,#0x0
  000e9ea4: bne.w 0x000ea144
  000e9ea8: ldrb.w r0,[r11,#0x66]
  000e9eac: cmp r0,#0x0
  000e9eae: bne.w 0x000ea144
  000e9eb2: ldrb.w r0,[r11,#0x67]
  000e9eb6: cmp r0,#0x0
  000e9eb8: bne.w 0x000ea144
  000e9ebc: ldrb.w r0,[r11,#0x68]
  000e9ec0: cmp r0,#0x0
  000e9ec2: bne.w 0x000ea144
  000e9ec6: ldrb.w r0,[r11,#0x60]
  000e9eca: cmp r0,#0x0
  000e9ecc: bne.w 0x000ea144
  000e9ed0: ldr.w r0,[r11,#0x1c]
  000e9ed4: cbz r0,0x000e9ede
  000e9ed6: ldr.w r1,[r11,#0x28]
  000e9eda: blx 0x00075490
  000e9ede: ldr.w r0,[r11,#0x14]
  000e9ee2: cbz r0,0x000e9eea
  000e9ee4: movs r1,#0x0
  000e9ee6: blx 0x0007219c
  000e9eea: ldr.w r1,[r11,#0x28]
  000e9eee: ldrb.w r0,[r11,#0x12e]
  000e9ef2: rsbs r2,r1
  000e9ef4: str r2,[sp,#0x10]
  000e9ef6: cmp r0,#0x0
  000e9ef8: str r1,[sp,#0x14]
  000e9efa: mov r0,r2
  000e9efc: it eq
  000e9efe: mov.eq r0,r1
  000e9f00: vmov s0,r0
  000e9f04: vcvt.f32.s32 s0,s0
  000e9f08: ldr.w r0,[r11,#0x13c]
  000e9f0c: vmov r1,s0
  000e9f10: blx 0x0007549c
  000e9f14: ldr.w r0,[r11,#0x13c]
  000e9f18: blx 0x0007468c
  000e9f1c: mov r5,r0
  000e9f1e: ldr.w r0,[r11,#0x13c]
  000e9f22: blx 0x000746a4
  000e9f26: mov r4,r0
  000e9f28: ldr.w r0,[r11,#0x13c]
  000e9f2c: str r5,[sp,#0xc]
  000e9f2e: vmov s16,r5
  000e9f32: blx 0x000746a4
  000e9f36: vmov s2,r4
  000e9f3a: vmov s0,r0
  000e9f3e: vsub.f32 s2,s16,s2
  000e9f42: vsub.f32 s0,s16,s0
  000e9f46: vmov.f32 s16,0x40a00000
  000e9f4a: vcmpe.f32 s2,#0
  000e9f4e: vmrs apsr,fpscr
  000e9f52: vneg.f32 s4,s0
  000e9f56: it gt
  000e9f58: vmov.gt.f32 s4,s0
  000e9f5c: vcmpe.f32 s4,s16
  000e9f60: vmrs apsr,fpscr
  000e9f64: bhi 0x000e9fae
  000e9f66: ldr r0,[0x000ea2c4]
  000e9f68: ldrb.w r4,[r11,#0x12e]
  000e9f6c: add r0,pc
  000e9f6e: ldr.w r5,[r11,#0x13c]
  000e9f72: vldr.32 s18,[r11,#0x130]
  000e9f76: eor r1,r4,#0x1
  000e9f7a: ldr r0,[r0,#0x0]
  000e9f7c: strb.w r1,[r11,#0x12e]
  000e9f80: movs r1,#0x83
  000e9f82: ldr r0,[r0,#0x0]
  000e9f84: blx 0x00071848
  000e9f88: add.w r2,r0,#0x12
  000e9f8c: mvn r1,#0x11
  000e9f90: cmp r4,#0x0
  000e9f92: it eq
  000e9f94: sub.eq r2,r1,r0
  000e9f96: vmov s0,r2
  000e9f9a: mov r0,r5
  000e9f9c: vcvt.f32.s32 s0,s0
  000e9fa0: ldr r1,[sp,#0xc]
  000e9fa2: vadd.f32 s0,s18,s0
  000e9fa6: vmov r2,s0
  000e9faa: blx 0x0006f4fc
  000e9fae: ldrb.w r0,[r11,#0x12d]
  000e9fb2: cmp r0,#0x0
  000e9fb4: ldrd r1,r0,[sp,#0x10]
  000e9fb8: it eq
  000e9fba: mov.eq r0,r1
  000e9fbc: vmov s0,r0
  000e9fc0: vcvt.f32.s32 s0,s0
  000e9fc4: ldr.w r0,[r11,#0x140]
  000e9fc8: vmov r1,s0
  000e9fcc: blx 0x0007549c
  000e9fd0: ldr.w r0,[r11,#0x140]
  000e9fd4: blx 0x0007468c
  000e9fd8: mov r5,r0
  000e9fda: ldr.w r0,[r11,#0x140]
  000e9fde: blx 0x000746a4
  000e9fe2: mov r4,r0
  000e9fe4: ldr.w r0,[r11,#0x140]
  000e9fe8: str r5,[sp,#0x8]
  000e9fea: vmov s18,r5
  000e9fee: blx 0x000746a4
  000e9ff2: vmov s2,r4
  000e9ff6: vmov s0,r0
  000e9ffa: vsub.f32 s2,s18,s2
  000e9ffe: vsub.f32 s0,s18,s0
  000ea002: vcmpe.f32 s2,#0
  000ea006: vmrs apsr,fpscr
  000ea00a: vneg.f32 s4,s0
  000ea00e: it gt
  000ea010: vmov.gt.f32 s4,s0
  000ea014: vcmpe.f32 s4,s16
  000ea018: vmrs apsr,fpscr
  000ea01c: bhi 0x000ea066
  000ea01e: ldr r0,[0x000ea2c8]
  000ea020: ldrb.w r4,[r11,#0x12d]
  000ea024: add r0,pc
  000ea026: ldr.w r5,[r11,#0x140]
  000ea02a: vldr.32 s18,[r11,#0x134]
  000ea02e: eor r1,r4,#0x1
  000ea032: ldr r0,[r0,#0x0]
  000ea034: strb.w r1,[r11,#0x12d]
  000ea038: movs r1,#0x78
  000ea03a: ldr r0,[r0,#0x0]
  000ea03c: blx 0x00071848
  000ea040: add.w r2,r0,#0x1e
  000ea044: mvn r1,#0x1d
  000ea048: cmp r4,#0x0
  000ea04a: it eq
  000ea04c: sub.eq r2,r1,r0
  000ea04e: vmov s0,r2
  000ea052: mov r0,r5
  000ea054: vcvt.f32.s32 s0,s0
  000ea058: ldr r1,[sp,#0x8]
  000ea05a: vadd.f32 s0,s18,s0
  000ea05e: vmov r2,s0
  000ea062: blx 0x0006f4fc
  000ea066: ldrb.w r0,[r11,#0x12f]
  000ea06a: cmp r0,#0x0
  000ea06c: ldrd r0,r1,[sp,#0x10]
  000ea070: it eq
  000ea072: mov.eq r1,r0
  000ea074: vmov s0,r1
  000ea078: vcvt.f32.s32 s0,s0
  000ea07c: ldr.w r0,[r11,#0x144]
  000ea080: vmov r1,s0
  000ea084: blx 0x0007549c
  000ea088: ldr.w r0,[r11,#0x144]
  000ea08c: blx 0x0007468c
  000ea090: mov r5,r0
  000ea092: ldr.w r0,[r11,#0x144]
  000ea096: blx 0x000746a4
  000ea09a: mov r4,r0
  000ea09c: ldr.w r0,[r11,#0x144]
  000ea0a0: vmov s18,r5
  000ea0a4: blx 0x000746a4
  000ea0a8: vmov s2,r4
  000ea0ac: vmov s0,r0
  000ea0b0: vsub.f32 s2,s18,s2
  000ea0b4: vsub.f32 s0,s18,s0
  000ea0b8: vcmpe.f32 s2,#0
  000ea0bc: vmrs apsr,fpscr
  000ea0c0: vneg.f32 s4,s0
  000ea0c4: it gt
  000ea0c6: vmov.gt.f32 s4,s0
  000ea0ca: vcmpe.f32 s4,s16
  000ea0ce: vmrs apsr,fpscr
  000ea0d2: bhi 0x000ea11e
  000ea0d4: ldr r0,[0x000ea2cc]
  000ea0d6: ldrb.w r4,[r11,#0x12f]
  000ea0da: add r0,pc
  000ea0dc: ldr.w r1,[r11,#0x144]
  000ea0e0: str r1,[sp,#0x14]
  000ea0e2: eor r1,r4,#0x1
  000ea0e6: ldr r0,[r0,#0x0]
  000ea0e8: vldr.32 s16,[r11,#0x138]
  000ea0ec: strb.w r1,[r11,#0x12f]
  000ea0f0: movs r1,#0x64
  000ea0f2: ldr r0,[r0,#0x0]
  000ea0f4: blx 0x00071848
  000ea0f8: add.w r2,r0,#0x32
  000ea0fc: mvn r1,#0x31
  000ea100: cmp r4,#0x0
  000ea102: it eq
  000ea104: sub.eq r2,r1,r0
  000ea106: vmov s0,r2
  000ea10a: mov r1,r5
  000ea10c: vcvt.f32.s32 s0,s0
  000ea110: ldr r0,[sp,#0x14]
  000ea112: vadd.f32 s0,s16,s0
  000ea116: vmov r2,s0
  000ea11a: blx 0x0006f4fc
  000ea11e: ldr r0,[0x000ea2d0]
  000ea120: add r0,pc
  000ea122: ldr r0,[r0,#0x0]
  000ea124: ldr r4,[r0,#0x0]
  000ea126: mov r0,r4
  000ea128: blx 0x000717f4
  000ea12c: mov r1,r0
  000ea12e: mov r0,r4
  000ea130: blx 0x0006ff1c
  000ea134: vstr.32 s18,[sp]
  000ea138: mov r1,r0
  000ea13a: ldrd r3,r2,[sp,#0x8]
  000ea13e: add r0,sp,#0x20
  000ea140: blx 0x0006f820
  000ea144: ldrd r0,r1,[r9,#0x0]
  000ea148: subs.w r0,r0,#0x3e8
  000ea14c: sbcs r0,r1,#0x0
  000ea150: blt.w 0x000ea27c
  000ea154: ldrb.w r0,[r11,#0x60]
  000ea158: cmp r0,#0x0
  000ea15a: beq.w 0x000ea29a
  000ea15e: ldr r0,[0x000ea2d4]
  000ea160: ldr.w r2,[r11,#0xb4]
  000ea164: adds r1,r0,#0x1
  000ea166: cmp r2,r1
  000ea168: bge 0x000ea1b2
  000ea16a: ldr.w r1,[r11,#0x28]
  000ea16e: add r1,r2
  000ea170: cmp r1,r0
  000ea172: ble 0x000ea1b2
  000ea174: ldr.w r0,[r11,#0x14]
  000ea178: cbz r0,0x000ea182
  000ea17a: blx 0x00075370
  000ea17e: blx 0x0006eb48
  000ea182: movs r0,#0x0
  000ea184: str.w r0,[r11,#0x14]
  000ea188: movs r0,#0xa0
  000ea18a: blx 0x0006eb24
  000ea18e: mov r5,r0
  000ea190: movs r1,#0x2
  000ea192: blx 0x00074ef0
  000ea196: mov r0,r5
  000ea198: str.w r5,[r11,#0x14]
  000ea19c: blx 0x00074efc
  000ea1a0: ldr.w r0,[r11,#0x14]
  000ea1a4: ldr r1,[0x000ea2d8]
  000ea1a6: str r1,[r0,#0x24]
  000ea1a8: movs r0,#0x1
  000ea1aa: ldr.w r2,[r11,#0xb4]
  000ea1ae: strb.w r0,[r11,#0x60]
  000ea1b2: ldr.w r1,[r11,#0x28]
  000ea1b6: ldr.w r0,[r11,#0x14]
  000ea1ba: add r2,r1
  000ea1bc: str.w r2,[r11,#0xb4]
  000ea1c0: blx 0x000754a8
  000ea1c4: ldr.w r0,[r11,#0x54]
  000ea1c8: blx 0x00075064
  000ea1cc: cmp r0,#0x0
  000ea1ce: beq.w 0x000ea346
  000ea1d2: ldr r0,[0x000ea2dc]
  000ea1d4: add r0,pc
  000ea1d6: ldr r5,[r0,#0x0]
  000ea1d8: ldr r0,[r5,#0x0]
  000ea1da: add.w r0,r0,r0, lsr #0x1f
  000ea1de: asrs r0,r0,#0x1
  000ea1e0: vmov s0,r0
  000ea1e4: vcvt.f32.s32 s0,s0
  000ea1e8: vldr.32 s16,[r11,#0x108]
  000ea1ec: ldr.w r0,[r11,#0x28]
  000ea1f0: vcmpe.f32 s16,s0
  000ea1f4: vmrs apsr,fpscr
  000ea1f8: ble 0x000ea228
  000ea1fa: vmov s2,r0
  000ea1fe: vldr.32 s6,[pc,#0xe0]
  000ea202: vcvt.f32.s32 s4,s2
  000ea206: vmov.f32 s2,s16
  000ea20a: vmla.f32 s2,s4,s6
  000ea20e: vcmpe.f32 s2,s0
  000ea212: vmrs apsr,fpscr
  000ea216: itttt ls
  000ea218: vstr.ls.32 s2,[r11,#0x108]
  000ea21c: mov.ls.w r1,#0xfa0
  000ea220: str.w.ls r1,[r11,#0xb8]
  000ea224: vmov.ls.f32 s16,s2
  000ea228: ldr.w r1,[r11,#0xb8]
  000ea22c: cmp r1,#0x0
  000ea22e: ble 0x000ea2e4
  000ea230: subs r0,r1,r0
  000ea232: str.w r0,[r11,#0xb8]
  000ea236: b 0x000ea346
  000ea238: mov.w r0,#0x10c
  000ea23c: blx 0x0006eb24
  000ea240: mov r5,r0
  000ea242: blx 0x000753f4
  000ea246: str.w r5,[r11,#0x74]
  000ea24a: ldr.w r0,[r10,#0x0]
  000ea24e: movs r1,#0x0
  000ea250: mov.w r3,#0x40000000
  000ea254: movs r4,#0x0
  000ea256: ldr r2,[r0,#0x0]
  000ea258: blx 0x0007159c
  000ea25c: ldr.w r0,[r10,#0x0]
  000ea260: movs r1,#0x7a
  000ea262: blx 0x000724a8
  000ea266: ldr.w r0,[r10,#0x0]
  000ea26a: movs r1,#0x6c
  000ea26c: movs r2,#0x0
  000ea26e: movs r3,#0x0
  000ea270: str r4,[sp,#0x0]
  000ea272: blx 0x00071548
  000ea276: movs r0,#0x1
  000ea278: strb.w r0,[r11,#0x65]
  000ea27c: ldr r0,[sp,#0x5c]
  000ea27e: ldr r1,[sp,#0x1c]
  000ea280: ldr r1,[r1,#0x0]
  000ea282: subs r0,r1,r0
  000ea284: itttt eq
  000ea286: add.eq sp,#0x60
  000ea288: vpop.eq {d8,d9}
  000ea28c: add.eq sp,#0x4
  000ea28e: pop.eq.w {r8,r9,r10,r11}
  000ea292: it eq
  000ea294: pop.eq {r4,r5,r6,r7,pc}
  000ea296: blx 0x0006e824
  000ea29a: ldrb.w r0,[r11,#0x69]
  000ea29e: cbz r0,0x000ea2ae
  000ea2a0: ldr.w r1,[r11,#0x28]
  000ea2a4: ldr.w r0,[r11,#0x84]
  000ea2a8: blx 0x000754b4
  000ea2ac: b 0x000ea3c4
  000ea2ae: ldrb.w r0,[r11,#0x67]
  000ea2b2: cmp r0,#0x0
  000ea2b4: beq 0x000ea376
  000ea2b6: ldr.w r0,[r11,#0x10]
  000ea2ba: ldr.w r1,[r11,#0x28]
  000ea2be: blx 0x000754c0
  000ea2c2: b 0x000ea3c4
  000ea2e4: vmov s0,r0
  000ea2e8: vldr.32 s2,[pc,#-0xc]
  000ea2ec: vcvt.f32.s32 s0,s0
  000ea2f0: ldr r0,[0x000ea660]
  000ea2f2: ldr.w r4,[r11,#0x98]
  000ea2f6: add r0,pc
  000ea2f8: vldr.32 s18,[r11,#0x104]
  000ea2fc: ldr.w r1,[r11,#0x5c]
  000ea300: ldr r0,[r0,#0x0]
  000ea302: vmla.f32 s16,s0,s2
  000ea306: vstr.32 s16,[r11,#0x108]
  000ea30a: ldr r0,[r0,#0x0]
  000ea30c: blx 0x00072d90
  000ea310: vmov s0,r0
  000ea314: vcvt.f32.s32 s0,s0
  000ea318: ldr r0,[r5,#0x0]
  000ea31a: add.w r0,r0,r0, lsr #0x1f
  000ea31e: asrs r0,r0,#0x1
  000ea320: vmov s2,r0
  000ea324: mov r0,r4
  000ea326: vadd.f32 s0,s16,s0
  000ea32a: vcvt.f32.s32 s2,s2
  000ea32e: vadd.f32 s0,s0,s2
  000ea332: vcvt.s32.f32 s0,s0
  000ea336: vcvt.s32.f32 s2,s18
  000ea33a: vmov r2,s0
  000ea33e: vmov r1,s2
  000ea342: blx 0x000754cc
  000ea346: ldr r0,[0x000ea664]
  000ea348: ldr.w r1,[r11,#0xb4]
  000ea34c: cmp r1,r0
  000ea34e: blt 0x000ea3c4
  000ea350: movs r4,#0x0
  000ea352: movs r1,#0x1
  000ea354: strb.w r4,[r11,#0x24]
  000ea358: ldr.w r0,[r8,#0x0]
  000ea35c: blx 0x00075070
  000ea360: ldr r0,[0x000ea668]
  000ea362: add r0,pc
  000ea364: ldr r1,[r0,#0x0]
  000ea366: ldr r0,[r6,#0x0]
  000ea368: str r4,[r1,#0x0]
  000ea36a: movs r1,#0x5
  000ea36c: blx 0x00071d64
  000ea370: strb.w r4,[r11,#0x60]
  000ea374: b 0x000ea27c
  000ea376: ldrb.w r0,[r11,#0x66]
  000ea37a: cbz r0,0x000ea38a
  000ea37c: ldr.w r1,[r11,#0x28]
  000ea380: ldr.w r0,[r11,#0x78]
  000ea384: blx 0x000754d8
  000ea388: b 0x000ea3c4
  000ea38a: ldrb.w r0,[r11,#0x64]
  000ea38e: cbz r0,0x000ea39e
  000ea390: ldr.w r1,[r11,#0x28]
  000ea394: ldr.w r0,[r11,#0x80]
  000ea398: blx 0x000754e4
  000ea39c: b 0x000ea3c4
  000ea39e: ldrb.w r0,[r11,#0x65]
  000ea3a2: cbz r0,0x000ea3b2
  000ea3a4: ldr.w r1,[r11,#0x28]
  000ea3a8: ldr.w r0,[r11,#0x74]
  000ea3ac: blx 0x000754f0
  000ea3b0: b 0x000ea3c4
  000ea3b2: ldrb.w r0,[r11,#0x68]
  000ea3b6: cbz r0,0x000ea3c4
  000ea3b8: ldr.w r1,[r11,#0x28]
  000ea3bc: ldr.w r0,[r11,#0x7c]
  000ea3c0: blx 0x000754fc
  000ea3c4: ldrb.w r0,[r11,#0x63]
  000ea3c8: cbz r0,0x000ea3d6
  000ea3ca: ldr.w r1,[r11,#0x28]
  000ea3ce: ldr.w r0,[r11,#0x70]
  000ea3d2: blx 0x00074eb4
  000ea3d6: ldrb.w r0,[r11,#0x62]
  000ea3da: cbz r0,0x000ea3e8
  000ea3dc: ldr.w r1,[r11,#0x28]
  000ea3e0: ldr.w r0,[r11,#0x50]
  000ea3e4: blx 0x00075508
  000ea3e8: ldrb.w r0,[r11,#0x63]
  000ea3ec: cmp r0,#0x0
  000ea3ee: bne.w 0x000ea7f4
  000ea3f2: ldrb.w r0,[r11,#0x69]
  000ea3f6: cmp r0,#0x0
  000ea3f8: bne.w 0x000ea7f4
  000ea3fc: ldrb.w r0,[r11,#0xb0]
  000ea400: cmp r0,#0x0
  000ea402: bne.w 0x000ea7f4
  000ea406: ldrb.w r0,[r11,#0xda]
  000ea40a: cbnz r0,0x000ea418
  000ea40c: mov r0,r11
  000ea40e: blx 0x00075514
  000ea412: movs r0,#0x1
  000ea414: strb.w r0,[r11,#0xda]
  000ea418: ldrb.w r0,[r11,#0xd9]
  000ea41c: cbnz r0,0x000ea42a
  000ea41e: mov r0,r11
  000ea420: blx 0x000751b4
  000ea424: movs r0,#0x1
  000ea426: strb.w r0,[r11,#0xd9]
  000ea42a: ldrb.w r0,[r11,#0x6a]
  000ea42e: cbnz r0,0x000ea46e
  000ea430: ldrb.w r0,[r11,#0x6e]
  000ea434: cbz r0,0x000ea46e
  000ea436: ldr r0,[0x000ea66c]
  000ea438: add r0,pc
  000ea43a: ldr r5,[r0,#0x0]
  000ea43c: ldrb.w r0,[r5,#0x2a]
  000ea440: cbnz r0,0x000ea46e
  000ea442: ldr r0,[0x000ea670]
  000ea444: movw r1,#0x259
  000ea448: ldr.w r4,[r11,#0x70]
  000ea44c: add r0,pc
  000ea44e: ldr r0,[r0,#0x0]
  000ea450: ldr r0,[r0,#0x0]
  000ea452: blx 0x00072f70
  000ea456: mov r1,r0
  000ea458: mov r0,r4
  000ea45a: blx 0x00074794
  000ea45e: movs r0,#0x1
  000ea460: strb.w r0,[r11,#0x63]
  000ea464: strb.w r0,[r5,#0x2a]
  000ea468: movs r0,#0x0
  000ea46a: strb.w r0,[r11,#0x6e]
  000ea46e: ldrb.w r0,[r11,#0x65]
  000ea472: movs r4,#0x0
  000ea474: ldr.w r5,[r8,#0x0]
  000ea478: cbz r0,0x000ea486
  000ea47a: ldr.w r0,[r11,#0x74]
  000ea47e: blx 0x00075520
  000ea482: mov r2,r0
  000ea484: b 0x000ea488
  000ea486: movs r2,#0x0
  000ea488: mov r0,r5
  000ea48a: movs r1,#0x1
  000ea48c: strd r4,r4,[sp,#0x0]
  000ea490: blx 0x0007552c
  000ea494: mov r5,r0
  000ea496: ldr.w r0,[r8,#0x0]
  000ea49a: blx 0x00071770
  000ea49e: cbnz r5,0x000ea4ee
  000ea4a0: cmp r0,#0x74
  000ea4a2: bne 0x000ea4ee
  000ea4a4: ldrb.w r0,[r11,#0x65]
  000ea4a8: cbz r0,0x000ea4ec
  000ea4aa: ldr.w r0,[r11,#0x74]
  000ea4ae: blx 0x00075520
  000ea4b2: cbz r0,0x000ea4ec
  000ea4b4: ldr.w r0,[r8,#0x0]
  000ea4b8: blx 0x00071a10
  000ea4bc: blx 0x00071a7c
  000ea4c0: cmp r0,#0x12
  000ea4c2: bne 0x000ea4ec
  000ea4c4: ldr.w r0,[r8,#0x0]
  000ea4c8: blx 0x0007285c
  000ea4cc: blx 0x0007288c
  000ea4d0: mov r6,r0
  000ea4d2: ldr.w r0,[r8,#0x0]
  000ea4d6: blx 0x00071c14
  000ea4da: blx 0x00071824
  000ea4de: subs r0,#0x5a
  000ea4e0: movs r4,#0x1
  000ea4e2: lsl.w r0,r4,r0
  000ea4e6: tst r0,r6
  000ea4e8: beq.w 0x000ea674
  000ea4ec: movs r4,#0x0
  000ea4ee: ldr.w r0,[r8,#0x0]
  000ea4f2: blx 0x00071770
  000ea4f6: cmp r0,#0x94
  000ea4f8: blt.w 0x000ea6ae
  000ea4fc: ldr.w r0,[r8,#0x0]
  000ea500: blx 0x00071770
  000ea504: cmp r0,#0x97
  000ea506: bgt.w 0x000ea6ae
  000ea50a: ldr.w r0,[r8,#0x0]
  000ea50e: blx 0x00071c14
  000ea512: blx 0x00071824
  000ea516: mov r6,r0
  000ea518: cmp r0,#0x42
  000ea51a: beq 0x000ea52e
  000ea51c: cmp r6,#0x37
  000ea51e: beq 0x000ea534
  000ea520: cmp r6,#0x9
  000ea522: ite ne
  000ea524: mov.ne.w r9,#0x0
  000ea528: mov.eq.w r9,#0x4
  000ea52c: b 0x000ea538
  000ea52e: mov.w r9,#0x2
  000ea532: b 0x000ea538
  000ea534: mov.w r9,#0x1
  000ea538: ldrb.w r0,[r11,#0x65]
  000ea53c: cbz r5,0x000ea5a6
  000ea53e: cmp r0,#0x0
  000ea540: beq.w 0x000ea6ae
  000ea544: ldr.w r0,[r11,#0x74]
  000ea548: blx 0x00075520
  000ea54c: cmp r0,#0x0
  000ea54e: beq.w 0x000ea6ae
  000ea552: ldr.w r0,[r8,#0x0]
  000ea556: blx 0x00071770
  000ea55a: cmp r0,#0x93
  000ea55c: ble.w 0x000ea6ae
  000ea560: ldr.w r0,[r8,#0x0]
  000ea564: blx 0x00071770
  000ea568: cmp r6,#0x60
  000ea56a: bne.w 0x000ea6ae
  000ea56e: cmp r0,#0x96
  000ea570: bgt.w 0x000ea6ae
  000ea574: ldr.w r0,[r8,#0x0]
  000ea578: movs r1,#0x97
  000ea57a: blx 0x00075538
  000ea57e: movs r0,#0x74
  000ea580: blx 0x0006eb24
  000ea584: mov r6,r0
  000ea586: blx 0x00075544
  000ea58a: movs r4,#0x1
  000ea58c: mov r0,r6
  000ea58e: mov r1,r5
  000ea590: movs r2,#0x1
  000ea592: movs r3,#0x97
  000ea594: str.w r5,[r11,#0x9c]
  000ea598: str.w r6,[r11,#0x84]
  000ea59c: blx 0x00075550
  000ea5a0: strb.w r4,[r11,#0x69]
  000ea5a4: b 0x000ea7ee
  000ea5a6: cmp r0,#0x0
  000ea5a8: beq.w 0x000ea6ae
  000ea5ac: ldr.w r0,[r11,#0x74]
  000ea5b0: blx 0x00075520
  000ea5b4: cmp r0,#0x0
  000ea5b6: beq 0x000ea6ae
  000ea5b8: ldr.w r0,[r8,#0x0]
  000ea5bc: blx 0x00071770
  000ea5c0: cmp r0,#0x94
  000ea5c2: blt 0x000ea6ae
  000ea5c4: ldr.w r0,[r8,#0x0]
  000ea5c8: blx 0x00071770
  000ea5cc: cmp r0,#0x96
  000ea5ce: bgt 0x000ea6ae
  000ea5d0: cmp r6,#0x9
  000ea5d2: it ne
  000ea5d4: cmp.ne r6,#0x37
  000ea5d6: bne 0x000ea6aa
  000ea5d8: ldr.w r0,[r8,#0x0]
  000ea5dc: blx 0x0007285c
  000ea5e0: blx 0x0007288c
  000ea5e4: tst.w r0,r9
  000ea5e8: bne 0x000ea6ae
  000ea5ea: ldr.w r0,[r8,#0x0]
  000ea5ee: blx 0x0007285c
  000ea5f2: mov r4,r0
  000ea5f4: ldr.w r0,[r8,#0x0]
  000ea5f8: blx 0x0007285c
  000ea5fc: blx 0x0007288c
  000ea600: orr.w r1,r0,r9
  000ea604: mov r0,r4
  000ea606: blx 0x00072898
  000ea60a: ldr.w r0,[r8,#0x0]
  000ea60e: blx 0x00071c14
  000ea612: blx 0x00071824
  000ea616: movs r5,#0x94
  000ea618: cmp r0,#0x42
  000ea61a: it eq
  000ea61c: mov.eq r5,#0x95
  000ea61e: cmp r0,#0x9
  000ea620: mov.w r0,#0x74
  000ea624: it eq
  000ea626: mov.eq r5,#0x96
  000ea628: blx 0x0006eb24
  000ea62c: mov r6,r0
  000ea62e: blx 0x00075544
  000ea632: movs r0,#0x78
  000ea634: str.w r6,[r11,#0x84]
  000ea638: blx 0x0006eb24
  000ea63c: mov r6,r0
  000ea63e: movs r1,#0xa0
  000ea640: movs r2,#0x0
  000ea642: mov.w r3,#0xffffffff
  000ea646: blx 0x00073834
  000ea64a: mov r0,r6
  000ea64c: movs r1,#0x1
  000ea64e: movs r4,#0x1
  000ea650: blx 0x00073828
  000ea654: ldr.w r0,[r11,#0x84]
  000ea658: mov r1,r6
  000ea65a: movs r2,#0x1
  000ea65c: mov r3,r5
  000ea65e: b 0x000ea59c
  000ea674: ldr.w r0,[r8,#0x0]
  000ea678: blx 0x0007285c
  000ea67c: mov r9,r0
  000ea67e: ldr.w r0,[r8,#0x0]
  000ea682: blx 0x0007285c
  000ea686: blx 0x0007288c
  000ea68a: mov r6,r0
  000ea68c: ldr.w r0,[r8,#0x0]
  000ea690: blx 0x00071c14
  000ea694: blx 0x00071824
  000ea698: subs r0,#0x5a
  000ea69a: lsl.w r0,r4,r0
  000ea69e: orr.w r1,r0,r6
  000ea6a2: mov r0,r9
  000ea6a4: blx 0x00072898
  000ea6a8: b 0x000ea4ee
  000ea6aa: cmp r6,#0x42
  000ea6ac: beq 0x000ea5d8
  000ea6ae: cmp r4,#0x0
  000ea6b0: beq 0x000ea752
  000ea6b2: ldr.w r0,[r8,#0x0]
  000ea6b6: blx 0x00071c14
  000ea6ba: blx 0x00071824
  000ea6be: movs r1,#0x0
  000ea6c0: ldr.w r2,[r10,#0x0]
  000ea6c4: sub.w r4,r0,#0x5a
  000ea6c8: cmp r0,#0x5e
  000ea6ca: str r1,[sp,#0x0]
  000ea6cc: it eq
  000ea6ce: mov.eq r4,#0x3
  000ea6d0: addw r1,r4,#0x619
  000ea6d4: mov r0,r2
  000ea6d6: movs r2,#0x0
  000ea6d8: movs r3,#0x0
  000ea6da: blx 0x00071548
  000ea6de: subs r0,r4,#0x2
  000ea6e0: clz r1,r4
  000ea6e4: clz r0,r0
  000ea6e8: addw r4,r4,#0xa9c
  000ea6ec: lsrs r1,r1,#0x5
  000ea6ee: movs r6,#0x3a
  000ea6f0: lsrs r0,r0,#0x5
  000ea6f2: orr.w r9,r1,r0
  000ea6f6: movw r0,#0xa9d
  000ea6fa: cmp r4,r0
  000ea6fc: it eq
  000ea6fe: mov.eq r6,#0x39
  000ea700: movs r0,#0x74
  000ea702: cmp.w r9,#0x0
  000ea706: it ne
  000ea708: mov.ne r6,#0x0
  000ea70a: blx 0x0006eb24
  000ea70e: mov r8,r0
  000ea710: ldr r0,[0x000eaaa8]
  000ea712: add r0,pc
  000ea714: ldr r5,[r0,#0x0]
  000ea716: ldr r0,[r5,#0x0]
  000ea718: mov r1,r4
  000ea71a: blx 0x00072f70
  000ea71e: mov r4,r0
  000ea720: ldr r0,[r5,#0x0]
  000ea722: movw r1,#0x5ec
  000ea726: cmp.w r9,#0x0
  000ea72a: it ne
  000ea72c: movw.ne r1,#0x63d
  000ea730: blx 0x00072f70
  000ea734: mov r2,r0
  000ea736: ldr r0,[0x000eaaac]
  000ea738: add r0,pc
  000ea73a: ldr.w r3,[r0,r6,lsl #0x2]
  000ea73e: mov r0,r8
  000ea740: mov r1,r4
  000ea742: blx 0x00074f20
  000ea746: movs r0,#0x1
  000ea748: strb.w r0,[r11,#0x69]
  000ea74c: str.w r8,[r11,#0x84]
  000ea750: b 0x000ea7ee
  000ea752: cbz r5,0x000ea79c
  000ea754: movs r0,#0x74
  000ea756: str.w r5,[r11,#0x9c]
  000ea75a: blx 0x0006eb24
  000ea75e: mov r6,r0
  000ea760: mov r1,r5
  000ea762: movs r2,#0x0
  000ea764: movs r3,#0x1
  000ea766: movs r4,#0x1
  000ea768: blx 0x0007501c
  000ea76c: mov r0,r5
  000ea76e: strb.w r4,[r11,#0x69]
  000ea772: str.w r6,[r11,#0x84]
  000ea776: blx 0x00072874
  000ea77a: cmp r0,#0xd
  000ea77c: bne 0x000ea78e
  000ea77e: ldr.w r0,[r8,#0x0]
  000ea782: movs r1,#0x0
  000ea784: strh.w r1,[r0,#0xf0]
  000ea788: mov r0,r11
  000ea78a: blx 0x00074f8c
  000ea78e: ldr.w r0,[r11,#0x74]
  000ea792: cbz r0,0x000ea7ee
  000ea794: movs r1,#0x1
  000ea796: blx 0x00075400
  000ea79a: b 0x000ea7ee
  000ea79c: ldr.w r0,[r8,#0x0]
  000ea7a0: movs r1,#0x1
  000ea7a2: movs r2,#0x0
  000ea7a4: movs r3,#0x0
  000ea7a6: blx 0x0007555c
  000ea7aa: mov r5,r0
  000ea7ac: cbz r0,0x000ea7ee
  000ea7ae: movs r0,#0x74
  000ea7b0: blx 0x0006eb24
  000ea7b4: mov r6,r0
  000ea7b6: mov r1,r5
  000ea7b8: movs r2,#0x0
  000ea7ba: movs r3,#0x2
  000ea7bc: blx 0x0007501c
  000ea7c0: movs r0,#0x1
  000ea7c2: mov r1,r5
  000ea7c4: strb.w r0,[r11,#0x69]
  000ea7c8: str.w r6,[r11,#0x84]
  000ea7cc: ldr.w r0,[r8,#0x0]
  000ea7d0: blx 0x00075154
  000ea7d4: mov r0,r5
  000ea7d6: blx 0x00072874
  000ea7da: cmp r0,#0xd
  000ea7dc: bne 0x000ea7ee
  000ea7de: ldr.w r0,[r8,#0x0]
  000ea7e2: movs r1,#0x0
  000ea7e4: strh.w r1,[r0,#0xf0]
  000ea7e8: mov r0,r11
  000ea7ea: blx 0x00074f8c
  000ea7ee: mov r0,r11
  000ea7f0: blx 0x00075568
  000ea7f4: ldrb.w r0,[r11,#0x100]
  000ea7f8: cbnz r0,0x000ea856
  000ea7fa: vldr.32 s0,[r11,#0x28]
  000ea7fe: vmov.f32 s8,0x3f800000
  000ea802: vldr.32 s2,[r11,#0xf4]
  000ea806: vldr.32 s4,[r11,#0xf8]
  000ea80a: vcvt.f32.s32 s0,s0
  000ea80e: vldr.32 s6,[pc,#0x280]
  000ea812: vmul.f32 s0,s0,s6
  000ea816: vmin.f32 d0,d0,d4
  000ea81a: vmul.f32 s2,s2,s0
  000ea81e: vmul.f32 s0,s4,s2
  000ea822: vnmul.f32 s2,s4,s2
  000ea826: vcmpe.f32 s0,#0
  000ea82a: vmrs apsr,fpscr
  000ea82e: it gt
  000ea830: vmov.gt.f32 s2,s0
  000ea834: vstr.32 s0,[r11,#0xf8]
  000ea838: vcmpe.f32 s2,s8
  000ea83c: vmrs apsr,fpscr
  000ea840: ble 0x000ea856
  000ea842: vldr.32 s2,[r11,#0xe4]
  000ea846: vcvt.f32.s32 s2,s2
  000ea84a: vadd.f32 s0,s0,s2
  000ea84e: vcvt.s32.f32 s0,s0
  000ea852: vstr.32 s0,[r11,#0xe4]
  000ea856: ldr.w r0,[r11,#0x14]
  000ea85a: cbz r0,0x000ea876
  000ea85c: ldrb.w r1,[r11,#0x60]
  000ea860: cbnz r1,0x000ea876
  000ea862: vldr.32 s0,[r11,#0xe4]
  000ea866: vldr.32 s2,[pc,#0x22c]
  000ea86a: vcvt.f32.s32 s0,s0
  000ea86e: vdiv.f32 s0,s0,s2
  000ea872: vstr.32 s0,[r0,#0x4]
  000ea876: ldrb.w r1,[r11,#0x118]
  000ea87a: cmp r1,#0x0
  000ea87c: beq.w 0x000ea27c
  000ea880: ldr r1,[sp,#0x18]
  000ea882: ldr r2,[r1,#0x0]
  000ea884: ldr.w r1,[r11,#0x114]
  000ea888: ldr r3,[r2,#0xc]
  000ea88a: cmp r1,r3
  000ea88c: bge 0x000ea8a8
  000ea88e: ldr.w r1,[r11,#0x110]
  000ea892: cmp r1,#0x63
  000ea894: bgt 0x000ea8d8
  000ea896: ldr r2,[0x000eaaa4]
  000ea898: movs r1,#0x0
  000ea89a: ldr r0,[r0,#0x20]
  000ea89c: movs r3,#0x0
  000ea89e: blx 0x00072154
  000ea8a2: vmov.f32 s0,0xc1200000
  000ea8a6: b 0x000ea8ec
  000ea8a8: ldr r3,[0x000eaab0]
  000ea8aa: ldr r6,[r2,#0x10]
  000ea8ac: add r3,pc
  000ea8ae: ldr.w r2,[r11,#0x110]
  000ea8b2: ldr r3,[r3,#0x0]
  000ea8b4: ldr r3,[r3,#0x0]
  000ea8b6: sub.w r6,r3,r6, lsl #0x1
  000ea8ba: cmp r1,r6
  000ea8bc: bge 0x000ea8fe
  000ea8be: cmp r2,#0x45
  000ea8c0: bgt 0x000ea934
  000ea8c2: ldr r2,[0x000eaa9c]
  000ea8c4: movs r1,#0x0
  000ea8c6: ldr r0,[r0,#0x20]
  000ea8c8: movs r3,#0x0
  000ea8ca: blx 0x00072160
  000ea8ce: ldr.w r0,[r11,#0x14]
  000ea8d2: vldr.32 s0,[pc,#0x1c8]
  000ea8d6: b 0x000ea958
  000ea8d8: cmp r1,#0xc7
  000ea8da: bgt 0x000ea966
  000ea8dc: ldr r2,[0x000eaaa0]
  000ea8de: movs r1,#0x0
  000ea8e0: ldr r0,[r0,#0x20]
  000ea8e2: movs r3,#0x0
  000ea8e4: blx 0x00072154
  000ea8e8: vmov.f32 s0,0x41200000
  000ea8ec: ldr.w r0,[r11,#0x14]
  000ea8f0: vldr.32 s2,[r0,#0xc]
  000ea8f4: vadd.f32 s0,s2,s0
  000ea8f8: vstr.32 s0,[r0,#0xc]
  000ea8fc: b 0x000ea27c
  000ea8fe: cmp r2,#0x45
  000ea900: bgt 0x000ea982
  000ea902: ldr.w r1,[r11,#0x28]
  000ea906: movs r2,#0x0
  000ea908: movs r3,#0x0
  000ea90a: rsbs r1,r1
  000ea90c: vmov s0,r1
  000ea910: vcvt.f32.s32 s0,s0
  000ea914: ldr r0,[r0,#0x20]
  000ea916: vmov r1,s0
  000ea91a: blx 0x00072154
  000ea91e: vldr.32 s0,[r11,#0x28]
  000ea922: vcvt.f32.s32 s0,s0
  000ea926: ldr.w r0,[r11,#0x14]
  000ea92a: vldr.32 s2,[r0,#0x8]
  000ea92e: vsub.f32 s0,s2,s0
  000ea932: b 0x000ea9bc
  000ea934: ldr r4,[0x000eaab4]
  000ea936: add r4,pc
  000ea938: ldr r6,[r4,#0x0]
  000ea93a: ldr r6,[r6,#0x0]
  000ea93c: sub.w r5,r6,#0x46
  000ea940: cmp r2,r5
  000ea942: ble 0x000ea9c4
  000ea944: ldr r2,[0x000eaa98]
  000ea946: movs r1,#0x0
  000ea948: ldr r0,[r0,#0x20]
  000ea94a: movs r3,#0x0
  000ea94c: blx 0x00072160
  000ea950: ldr.w r0,[r11,#0x14]
  000ea954: vldr.32 s0,[pc,#0x140]
  000ea958: vldr.32 s2,[r0,#0x18]
  000ea95c: vadd.f32 s0,s2,s0
  000ea960: vstr.32 s0,[r0,#0x18]
  000ea964: b 0x000ea27c
  000ea966: cmp.w r1,#0x12c
  000ea96a: bge 0x000eaa0a
  000ea96c: ldr r1,[0x000eaa9c]
  000ea96e: movs r2,#0x0
  000ea970: ldr r0,[r0,#0x20]
  000ea972: movs r3,#0x0
  000ea974: blx 0x00072160
  000ea978: ldr.w r0,[r11,#0x14]
  000ea97c: vldr.32 s0,[pc,#0x11c]
  000ea980: b 0x000eaa26
  000ea982: ldr r1,[0x000eaab8]
  000ea984: add r1,pc
  000ea986: ldr r1,[r1,#0x0]
  000ea988: ldr r1,[r1,#0x0]
  000ea98a: subs r1,#0x46
  000ea98c: cmp r2,r1
  000ea98e: ble.w 0x000ea27c
  000ea992: vldr.32 s0,[r11,#0x28]
  000ea996: movs r2,#0x0
  000ea998: movs r3,#0x0
  000ea99a: vcvt.f32.s32 s0,s0
  000ea99e: ldr r0,[r0,#0x20]
  000ea9a0: vmov r1,s0
  000ea9a4: blx 0x00072154
  000ea9a8: vldr.32 s0,[r11,#0x28]
  000ea9ac: vcvt.f32.s32 s0,s0
  000ea9b0: ldr.w r0,[r11,#0x14]
  000ea9b4: vldr.32 s2,[r0,#0x8]
  000ea9b8: vadd.f32 s0,s2,s0
  000ea9bc: vstr.32 s0,[r0,#0x8]
  000ea9c0: b.w 0x000ea27c
  000ea9c4: cmp r2,#0x65
  000ea9c6: blt.w 0x000ea27c
  000ea9ca: subs r6,#0x64
  000ea9cc: cmp r2,r6
  000ea9ce: bge.w 0x000ea27c
  000ea9d2: add.w r3,r3,r3, lsr #0x1f
  000ea9d6: ldr.w r2,[r11,#0x28]
  000ea9da: ldr r0,[r0,#0x20]
  000ea9dc: cmp.w r1,r3, asr #0x1
  000ea9e0: bge 0x000eaa36
  000ea9e2: rsbs r1,r2
  000ea9e4: vmov s0,r1
  000ea9e8: vcvt.f32.s32 s0,s0
  000ea9ec: vmov r1,s0
  000ea9f0: blx 0x000725c8
  000ea9f4: vldr.32 s0,[r11,#0x28]
  000ea9f8: vcvt.f32.s32 s0,s0
  000ea9fc: ldr.w r0,[r11,#0x14]
  000eaa00: vldr.32 s2,[r0,#0x10]
  000eaa04: vsub.f32 s0,s2,s0
  000eaa08: b 0x000eaa5a
  000eaa0a: cmp.w r1,#0x190
  000eaa0e: bge.w 0x000ea27c
  000eaa12: ldr r1,[0x000eaa98]
  000eaa14: movs r2,#0x0
  000eaa16: ldr r0,[r0,#0x20]
  000eaa18: movs r3,#0x0
  000eaa1a: blx 0x00072160
  000eaa1e: ldr.w r0,[r11,#0x14]
  000eaa22: vldr.32 s0,[pc,#0x74]
  000eaa26: vldr.32 s2,[r0,#0x14]
  000eaa2a: vadd.f32 s0,s2,s0
  000eaa2e: vstr.32 s0,[r0,#0x14]
  000eaa32: b.w 0x000ea27c
  000eaa36: vmov s0,r2
  000eaa3a: vcvt.f32.s32 s0,s0
  000eaa3e: vmov r1,s0
  000eaa42: blx 0x000725c8
  000eaa46: vldr.32 s0,[r11,#0x28]
  000eaa4a: vcvt.f32.s32 s0,s0
  000eaa4e: ldr.w r0,[r11,#0x14]
  000eaa52: vldr.32 s2,[r0,#0x10]
  000eaa56: vadd.f32 s0,s2,s0
  000eaa5a: vstr.32 s0,[r0,#0x10]
  000eaa5e: b.w 0x000ea27c
```
