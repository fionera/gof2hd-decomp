# MenuTouchWindow::OnTouchEnd
elf 0x12aabc body 6358
Sig: undefined __thiscall OnTouchEnd(MenuTouchWindow * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* MenuTouchWindow::OnTouchEnd(int, int, void*) */

void __thiscall
MenuTouchWindow::OnTouchEnd(MenuTouchWindow *this,int param_1,int param_2,void *param_3)

{
  byte bVar1;
  float fVar2;
  MenuTouchWindow MVar3;
  int iVar4;
  MenuTouchWindow *pMVar5;
  undefined4 *puVar6;
  void *pvVar7;
  FModSound *this_00;
  float fVar8;
  RecordHandler *this_01;
  int iVar9;
  Engine *pEVar10;
  double *pdVar11;
  int iVar12;
  int *piVar13;
  String *pSVar14;
  int *piVar15;
  EVP_PKEY_CTX *ctx;
  int extraout_r1;
  Globals *pGVar16;
  int *piVar17;
  undefined4 *puVar18;
  ApplicationManager *this_02;
  code *pcVar19;
  ChoiceWindow *pCVar20;
  uint uVar21;
  String *pSVar22;
  ScrollTouchWindow *pSVar23;
  int iVar24;
  int *piVar25;
  uint uVar26;
  uint uVar27;
  uint *puVar28;
  undefined4 *puVar29;
  char *pcVar30;
  int *piVar31;
  bool bVar32;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  undefined4 uVar33;
  float extraout_s0_01;
  float extraout_s0_02;
  undefined4 uVar34;
  float fVar35;
  undefined4 uVar36;
  double dVar37;
  undefined8 uVar38;
  AbyssEngine aAStack_80 [12];
  String aSStack_74 [12];
  String aSStack_68 [12];
  int local_5c;
  
  piVar25 = *(int **)(DAT_0013ad84 + 0x13aad6);
  piVar17 = *(int **)(DAT_0013ad88 + 0x13aad8);
  local_5c = *piVar25;
  this[1] = (MenuTouchWindow)0x0;
  this[0x224] = (MenuTouchWindow)0x0;
  if (*(Layout *)*piVar17 != (Layout)0x0) {
    iVar4 = Layout::OnTouchEnd((Layout *)*piVar17,param_1,param_2);
    if (iVar4 != 0) {
      *(undefined1 *)*piVar17 = 0;
    }
    goto switchD_0013bf40_caseD_0;
  }
  pMVar5 = this + 0x16c;
  switch(*(undefined4 *)(this + 0x16c)) {
  case 0:
    if (this[0x170] != (MenuTouchWindow)0x0) {
      if (this[0x178] == (MenuTouchWindow)0x0) {
        if (this[0x191] != (MenuTouchWindow)0x0) {
          iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
          if (iVar4 != 1) {
            if (iVar4 != 0) goto switchD_0013bf40_caseD_0;
            **(undefined4 **)(DAT_0013be28 + 0x13bcd8) = 1;
          }
          this[0x170] = (MenuTouchWindow)0x0;
          this[0x191] = (MenuTouchWindow)0x0;
          goto switchD_0013bf40_caseD_0;
        }
        if (this[0x177] == (MenuTouchWindow)0x0) {
          if (this[0x179] != (MenuTouchWindow)0x0) {
            uVar21 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
            if (uVar21 < 2) {
              Globals::reportLeaderboards();
            }
            this[0x179] = (MenuTouchWindow)0x0;
            this[0x170] = (MenuTouchWindow)0x0;
          }
          if (this[0x17a] == (MenuTouchWindow)0x0) {
            if ((this[0x17d] != (MenuTouchWindow)0x0) || (this[0x17c] != (MenuTouchWindow)0x0))
            goto LAB_0013ba74;
            if (this[0x17e] == (MenuTouchWindow)0x0) {
              if ((this[0x17f] != (MenuTouchWindow)0x0) &&
                 (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 == 0)) {
                this[0x170] = (MenuTouchWindow)0x0;
                this[0x17f] = (MenuTouchWindow)0x0;
                goto switchD_0013bf40_caseD_0;
              }
              if ((this[0x180] == (MenuTouchWindow)0x0) ||
                 (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 != 0)) {
                if ((this[0x181] == (MenuTouchWindow)0x0) ||
                   (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 != 0))
                goto LAB_0013c30c;
                this[0x181] = (MenuTouchWindow)0x0;
              }
              else {
                this[0x180] = (MenuTouchWindow)0x0;
              }
            }
            else {
              this[0x17e] = (MenuTouchWindow)0x0;
            }
            goto LAB_0013b71e;
          }
          iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
          goto LAB_0013ab9a;
        }
        iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
        if (iVar4 == 1) {
          this[0x177] = (MenuTouchWindow)0x0;
          this[0x170] = (MenuTouchWindow)0x0;
          this[0x174] = (MenuTouchWindow)0x0;
          goto switchD_0013bf40_caseD_0;
        }
        if (iVar4 != 0) goto switchD_0013bf40_caseD_0;
        FModSound::resumeAll((FModSound *)**(undefined4 **)(DAT_0013c194 + 0x13be62));
        FModSound::stopAll();
        uVar21 = **(uint **)(DAT_0013c198 + 0x13be78);
        **(undefined4 **)(DAT_0013c19c + 0x13be7a) = 2;
        AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uVar21);
        this[0x177] = (MenuTouchWindow)0x0;
      }
      else {
        iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
        if (iVar4 != 1) {
          if (iVar4 != 0) goto switchD_0013bf40_caseD_0;
          AbyssEngine::ApplicationManager::Quit
                    ((ApplicationManager *)**(undefined4 **)(DAT_0013ad8c + 0x13ab78));
          goto switchD_0013bf40_caseD_b;
        }
        this[0x178] = (MenuTouchWindow)0x0;
      }
LAB_0013bdc0:
      this[0x170] = (MenuTouchWindow)0x0;
      goto switchD_0013bf40_caseD_0;
    }
LAB_0013c30c:
    piVar13 = *(int **)(DAT_0013c4e4 + 0x13c31a);
    for (uVar21 = 0; uVar21 < **(uint **)(this + 4); uVar21 = uVar21 + 1) {
      iVar4 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(this + 4))[1] + uVar21 * 4),param_1);
      if (iVar4 != 0) {
        puVar28 = *(uint **)(*(int *)(*(int *)(this + 4) + 4) + uVar21 * 4);
        uVar26 = *puVar28;
        uVar27 = puVar28[1];
        if (-uVar27 < (uint)(uVar26 < 0x1a)) {
                    /* WARNING: Could not recover jumptable at 0x0013c358. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (*(code *)(&DAT_0013c35c + (uint)*(ushort *)(&DAT_0013c35c + uVar26 * 2) * 2))
                    (uVar26,uVar27,-(uint)(uVar26 >= 0x1a) - uVar27);
          return;
        }
        if (uVar26 == 0x6b && uVar27 == 0) {
          *(undefined4 *)pMVar5 = 0x12;
        }
      }
    }
    piVar31 = *(int **)(DAT_0013ca64 + 0x13c75c);
    pcVar30 = (char *)(DAT_0013ca68 + 0x13c764);
    puVar6 = *(undefined4 **)(DAT_0013ca6c + 0x13c76a);
    for (uVar21 = 0; uVar21 < **(uint **)(this + 0xc0); uVar21 = uVar21 + 1) {
      piVar15 = *(int **)((*(uint **)(this + 0xc0))[1] + uVar21 * 4);
      iVar4 = *piVar15;
      if (iVar4 == 0x18) {
        iVar4 = TouchButton::OnTouchEnd((int)piVar15,param_1);
        if (iVar4 != 0) {
          puVar29 = puVar6;
          if (*piVar31 != 0) goto LAB_0013c7bc;
          pCVar20 = *(ChoiceWindow **)(this + 0x104);
          AbyssEngine::String::String((String *)aAStack_80,pcVar30,false);
          pSVar14 = (String *)GameText::getText(*piVar13);
          ChoiceWindow::set(pCVar20,(String *)aAStack_80,pSVar14,true);
LAB_0013c8ca:
          AbyssEngine::String::~String((String *)aAStack_80);
          this[0x191] = (MenuTouchWindow)0x1;
          this[0x170] = (MenuTouchWindow)0x1;
        }
LAB_0013c8dc:
        Globals::reportLeaderboards();
      }
      else {
        if (iVar4 == 0x17) {
          iVar4 = TouchButton::OnTouchEnd((int)piVar15,param_1);
          if (iVar4 != 0) {
            if (*piVar31 == 0) {
              pCVar20 = *(ChoiceWindow **)(this + 0x104);
              AbyssEngine::String::String
                        ((String *)aAStack_80,(char *)(DAT_0013ca74 + 0x13c862),false);
              pSVar14 = (String *)GameText::getText(*piVar13);
              ChoiceWindow::set(pCVar20,(String *)aAStack_80,pSVar14,true);
              goto LAB_0013c8ca;
            }
            puVar29 = *(undefined4 **)(DAT_0013ca78 + 0x13c7a2);
LAB_0013c7bc:
            *puVar29 = 1;
          }
          goto LAB_0013c8dc;
        }
        Globals::reportLeaderboards();
        if (iVar4 == 0x35) {
          iVar4 = TouchButton::OnTouchEnd
                            (*(int *)(*(int *)(*(int *)(this + 0xc0) + 4) + uVar21 * 4),param_1);
          if (iVar4 != 0) {
            iVar4 = AbyssEngine::ApplicationManager::GetApplicationData();
            *(undefined1 *)(iVar4 + 0x4c) = 0;
            iVar4 = AbyssEngine::ApplicationManager::GetApplicationData();
            *(undefined1 *)(iVar4 + 0x3d) = 1;
            pCVar20 = *(ChoiceWindow **)(this + 0x104);
            this[0x170] = (MenuTouchWindow)0x1;
            this[0x17a] = (MenuTouchWindow)0x1;
            pSVar14 = (String *)GameText::getText(*piVar13);
            pSVar22 = (String *)GameText::getText(*piVar13);
            ChoiceWindow::set(pCVar20,pSVar14,pSVar22);
          }
        }
        else if ((iVar4 == 0x11) &&
                (iVar4 = TouchButton::OnTouchEnd
                                   (*(int *)(*(int *)(*(int *)(this + 0xc0) + 4) + uVar21 * 4),
                                    param_1), iVar4 != 0)) {
          pCVar20 = *(ChoiceWindow **)(this + 0x104);
          pSVar14 = (String *)GameText::getText(*piVar13);
          ChoiceWindow::set(pCVar20,pSVar14,true);
          this[0x178] = (MenuTouchWindow)0x1;
          goto LAB_0013bde4;
        }
      }
    }
    break;
  case 1:
    if (this[0x170] != (MenuTouchWindow)0x0) {
      if (this[0x1db] != (MenuTouchWindow)0x0) goto switchD_0013bf40_caseD_0;
      if ((this[0x1da] != (MenuTouchWindow)0x0) ||
         (((iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 == 0 &&
           (iVar4 = ChoiceWindow::hasChoice(*(ChoiceWindow **)(this + 0x104)), iVar4 != 0)) &&
          (iVar4 = loadGame(this,*(int *)(this + 0x18c)), iVar4 == 0))))
      goto switchD_0013bf40_caseD_b;
      this[0x170] = (MenuTouchWindow)0x0;
    }
    iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xc4),param_1);
    if (iVar4 != 0) {
      if (*(int *)(*(int *)(*(int *)(this + 0xbc) + 4) + *(int *)(this + 0x18c) * 4) == 0)
      goto switchD_0013bf40_caseD_0;
      iVar4 = Status::getPlayingTime();
      if (extraout_r1 < (int)(uint)(iVar4 == 0)) {
        FModSound::play(**(int **)(DAT_0013c1b8 + 0x13bfa2),(Vector *)0x7b,(Vector *)0x0,extraout_s0
                       );
        loadGame(this,*(int *)(this + 0x18c));
        goto switchD_0013bf40_caseD_0;
      }
      pCVar20 = *(ChoiceWindow **)(this + 0x104);
      pSVar14 = (String *)GameText::getText(**(int **)(DAT_0013b168 + 0x13af94));
      ChoiceWindow::set(pCVar20,pSVar14,true);
LAB_0013bde4:
      this[0x170] = (MenuTouchWindow)0x1;
      goto switchD_0013bf40_caseD_0;
    }
    break;
  case 2:
    if (this[0x170] != (MenuTouchWindow)0x0) {
      if (this[0x1db] != (MenuTouchWindow)0x0) goto switchD_0013bf40_caseD_0;
      if (this[0x1da] != (MenuTouchWindow)0x0) goto switchD_0013bf40_caseD_b;
      MVar3 = this[0x173];
      iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
      if (MVar3 == (MenuTouchWindow)0x0) {
        if (iVar4 != 0) goto switchD_0013bf40_caseD_0;
      }
      else {
        if (iVar4 != 1) {
          if (iVar4 == 0) {
            saveGame(this,*(int *)(this + 0x18c));
            this[0x173] = (MenuTouchWindow)0x0;
          }
          goto switchD_0013bf40_caseD_0;
        }
        this[0x173] = (MenuTouchWindow)0x0;
      }
      goto LAB_0013bdc0;
    }
    iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xc4),param_1);
    if (iVar4 != 0) {
      if (*(int *)(this + 0x18c) == 0) {
        pSVar22 = *(String **)(this + 0x104);
        pSVar14 = (String *)GameText::getText(**(int **)(DAT_0013c1a0 + 0x13be9c));
        AbyssEngine::operator+(aAStack_80,pSVar14);
        ChoiceWindow::set(pSVar22);
        AbyssEngine::String::~String((String *)aAStack_80);
        this[0x170] = (MenuTouchWindow)0x1;
        goto switchD_0013bf40_caseD_0;
      }
      if (*(int *)(*(int *)(*(int *)(this + 0xbc) + 4) + *(int *)(this + 0x18c) * 4) == 0) {
        FModSound::play(**(int **)(DAT_0013c1c0 + 0x13bff4),(Vector *)0x7b,(Vector *)0x0,
                        extraout_s0_02);
        saveGame(this,*(int *)(this + 0x18c));
        goto switchD_0013bf40_caseD_0;
      }
      pCVar20 = *(ChoiceWindow **)(this + 0x104);
      pSVar14 = (String *)GameText::getText(**(int **)(DAT_0013be18 + 0x13bb46));
      ChoiceWindow::set(pCVar20,pSVar14,true);
      this[0x173] = (MenuTouchWindow)0x1;
      goto LAB_0013bde4;
    }
    break;
  case 3:
    if (**(char **)(DAT_0013b50c + 0x13b182) == '\0') {
      for (uVar21 = 0; uVar21 < **(uint **)(this + 0xac); uVar21 = uVar21 + 1) {
        iVar4 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(this + 0xac))[1] + uVar21 * 4),param_1)
        ;
        if (iVar4 != 0) {
          puVar28 = *(uint **)(*(int *)(*(int *)(this + 0xac) + 4) + uVar21 * 4);
          uVar27 = *puVar28;
          uVar26 = puVar28[1];
          if (-(uVar26 - (uVar27 < 4)) < (uint)(uVar27 - 4 < 6)) {
                    /* WARNING: Could not recover jumptable at 0x0013bbc2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*(code *)(&UNK_0013bbc6 + (uint)*(byte *)(uVar27 + 0x13bbc2) * 2))();
            return;
          }
          if (uVar27 == 0x19 && uVar26 == 0) {
            *(undefined4 *)pMVar5 = 0xe;
          }
        }
      }
    }
    else {
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xe4),param_1);
      if (iVar4 != 0) {
        *(undefined4 *)pMVar5 = 0xb;
      }
      MVar3 = (MenuTouchWindow)0x0;
      if (this[0x170] != (MenuTouchWindow)0x0) {
        MVar3 = this[0x176];
      }
      if ((this[0x170] != (MenuTouchWindow)0x0 && MVar3 != (MenuTouchWindow)0x0) &&
         (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 == 0)) {
        pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        iVar4 = AbyssEngine::Engine::GetAccelValue(pEVar10);
        dVar37 = *(double *)(iVar4 + 0x10);
        pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        pdVar11 = (double *)AbyssEngine::Engine::GetAccelValue(pEVar10);
        uVar21 = in_fpscr & 0xfffffff;
        uVar26 = uVar21 | (uint)(dVar37 < 0.0) << 0x1f | (uint)(dVar37 == 0.0) << 0x1e;
        in_fpscr = uVar26 | (uint)NAN(dVar37) << 0x1c;
        dVar37 = *pdVar11;
        bVar1 = (byte)(uVar26 >> 0x18);
        if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
          uVar21 = uVar21 | (uint)(dVar37 < 1.0) << 0x1f | (uint)(dVar37 == 1.0) << 0x1e;
          in_fpscr = uVar21 | (uint)NAN(dVar37) << 0x1c;
          bVar1 = (byte)(uVar21 >> 0x18);
          if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
            pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
            puVar6 = (undefined4 *)AbyssEngine::Engine::GetAccelValue(pEVar10);
            uVar36 = DAT_0013be20;
            *puVar6 = 0;
            puVar6[1] = uVar36;
          }
          pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
          pdVar11 = (double *)AbyssEngine::Engine::GetAccelValue(pEVar10);
          dVar37 = (1.0 - *pdVar11) + 1.0;
        }
        iVar12 = *(int *)(DAT_0013be34 + 0x13bd94);
        *(float *)(iVar12 + 0x1c) = (float)dVar37;
        pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        iVar4 = AbyssEngine::Engine::GetAccelValue(pEVar10);
        *(float *)(iVar12 + 0x20) = (float)*(double *)(iVar4 + 0x10);
        this[0x176] = (MenuTouchWindow)0x0;
        this[0x170] = (MenuTouchWindow)0x0;
      }
      iVar4 = 0x14;
      puVar6 = *(undefined4 **)(DAT_0013b510 + 0x13b1c4);
      if (*(char *)((int)puVar6 + 0x11) == '\0') {
        iVar4 = 0x18;
      }
      uVar36 = TouchSlider::getValue((TouchSlider *)**(undefined4 **)(*(int *)(this + 0xec) + 4));
      *(undefined4 *)((int)puVar6 + iVar4) = uVar36;
      if (this[0x108] != (MenuTouchWindow)0x0) {
        iVar4 = *piVar17;
        iVar12 = *(int *)(this + 0x19c) + *(int *)(iVar4 + 0x28);
        if ((((iVar12 < param_1) && (param_1 < iVar12 + *(int *)(this + 0x158))) &&
            (*(int *)(iVar4 + 0x28) + *(int *)(iVar4 + 0xc) < param_2)) &&
           (param_2 < *(int *)(iVar4 + 0x20) + *(int *)(iVar4 + 0xc) + *(int *)(this + 0x154))) {
          *(undefined1 *)((int)puVar6 + 0x11) = 1;
          fVar35 = (float)TouchButton::setHalfTransparent(SUB41(*(undefined4 *)(this + 0xd0),0));
          fVar35 = (float)TouchSlider::setValue
                                    ((TouchSlider *)**(undefined4 **)(*(int *)(this + 0xec) + 4),
                                     fVar35);
          FModSound::play(**(int **)(DAT_0013b514 + 0x13b23e),(Vector *)0x7b,(Vector *)0x0,fVar35);
        }
      }
      if (this[0x109] != (MenuTouchWindow)0x0) {
        iVar4 = *piVar17;
        iVar12 = *(int *)(iVar4 + 0x28);
        if (((*(int *)(this + 0x158) + iVar12 + *(int *)(this + 0x19c) < param_1) &&
            (param_1 < (*(int *)(this + 0x19c) - iVar12) + *(int *)(this + 0x1a0))) &&
           ((iVar12 + *(int *)(iVar4 + 0xc) < param_2 &&
            (param_2 < *(int *)(iVar4 + 0x20) + *(int *)(iVar4 + 0xc) + *(int *)(this + 0x154))))) {
          *(undefined1 *)((int)puVar6 + 0x11) = 0;
          fVar35 = (float)TouchButton::setHalfTransparent(SUB41(*(undefined4 *)(this + 0xd0),0));
          fVar35 = (float)TouchSlider::setValue
                                    ((TouchSlider *)**(undefined4 **)(*(int *)(this + 0xec) + 4),
                                     fVar35);
          FModSound::play(**(int **)(DAT_0013b518 + 0x13b2b4),(Vector *)0x7b,(Vector *)0x0,fVar35);
        }
      }
      *(undefined2 *)(this + 0x108) = 0;
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xcc),param_1);
      if (iVar4 != 0) {
        bVar1 = *(byte *)(puVar6 + 4);
        *(byte *)(puVar6 + 4) = bVar1 ^ 1;
        TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xcc),bVar1 == 0);
      }
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xd0),param_1);
      if (iVar4 != 0) {
        pSVar14 = *(String **)(this + 0x104);
        GameText::getText(**(int **)(DAT_0013b51c + 0x13b30c));
        ChoiceWindow::set(pSVar14);
        this[0x176] = (MenuTouchWindow)0x1;
        this[0x170] = (MenuTouchWindow)0x1;
      }
      TouchSlider::OnTouchEnd(**(int **)(*(int *)(this + 0xec) + 4),param_1);
      if ((this[0x170] != (MenuTouchWindow)0x0) &&
         (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 == 0))
      goto LAB_0013ba56;
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xd4),param_1);
      if (iVar4 != 0) {
        bVar1 = *(byte *)((int)puVar6 + 0xd);
        *(byte *)((int)puVar6 + 0xd) = bVar1 ^ 1;
        TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xd4),bVar1 == 0);
        TouchSlider::setHalfTransparent
                  (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 4),
                   *(char *)((int)puVar6 + 0xd) == '\0');
        FModSound::enableCategory(**(int **)(DAT_0013b520 + 0x13b398),true);
      }
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xd8),param_1);
      if (iVar4 != 0) {
        bVar1 = *(byte *)(puVar6 + 3);
        *(byte *)(puVar6 + 3) = bVar1 ^ 1;
        TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xd8),bVar1 == 0);
        TouchSlider::setHalfTransparent
                  (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 8),
                   *(char *)(puVar6 + 3) == '\0');
        piVar13 = *(int **)(DAT_0013b524 + 0x13b3e8);
        fVar35 = (float)FModSound::enableCategory(*piVar13,true);
        FModSound::play(*piVar13,(Vector *)0x7e,(Vector *)0x0,fVar35);
      }
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xdc),param_1);
      if (iVar4 != 0) {
        bVar1 = *(byte *)((int)puVar6 + 0xe);
        *(byte *)((int)puVar6 + 0xe) = bVar1 ^ 1;
        TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xdc),bVar1 == 0);
        TouchSlider::setHalfTransparent
                  (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0xc),
                   *(char *)((int)puVar6 + 0xe) == '\0');
        FModSound::enableCategory(**(int **)(DAT_0013b528 + 0x13b44a),true);
      }
      pcVar19 = *(code **)(DAT_0013b52c + 0x13b45c);
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 4));
      *puVar6 = uVar36;
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 8));
      puVar6[1] = uVar36;
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 0xc));
      puVar6[2] = uVar36;
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 0x10));
      puVar6[9] = uVar36;
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 0x14));
      puVar6[10] = uVar36;
      uVar36 = DAT_0013b53c;
      fVar2 = DAT_0013b538;
      fVar35 = DAT_0013b534;
      pcVar30 = *(char **)(DAT_0013b530 + 0x13b4b0);
      piVar13 = *(int **)(DAT_0013b540 + 0x13b4c0);
      puVar29 = *(undefined4 **)(DAT_0013b544 + 0x13b4c8);
      piVar31 = *(int **)(DAT_0013b548 + 0x13b4d2);
      for (uVar21 = 1; uVar21 < **(uint **)(this + 0xec); uVar21 = uVar21 + 1) {
        iVar4 = TouchSlider::OnTouchEnd(*(int *)((*(uint **)(this + 0xec))[1] + uVar21 * 4),param_1)
        ;
        if (iVar4 != 0) {
          if ((uVar21 & 0x7fffffff) == 5) {
            fVar8 = (float)TouchSlider::getValue
                                     (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x14))
            ;
            if (*pcVar30 == '\0') {
              in_fpscr = in_fpscr & 0xfffffff;
              pSVar14 = *(String **)(this + 0x104);
              GameText::getText(*piVar13);
              ChoiceWindow::set(pSVar14);
              this[0x170] = (MenuTouchWindow)0x1;
            }
            else {
              pSVar14 = *(String **)(this + 0x104);
              GameText::getText(*piVar13);
              ChoiceWindow::set(pSVar14);
              uVar33 = 0x3f800000;
              in_fpscr = in_fpscr & 0xfffffff | (uint)(fVar8 == fVar35) << 0x1e |
                         (uint)(fVar35 <= fVar8) << 0x1d;
              if (fVar8 <= fVar2) {
                uVar33 = 0x3f000000;
              }
              bVar1 = (byte)(in_fpscr >> 0x18);
              uVar34 = uVar36;
              if ((bool)(bVar1 >> 5 & 1) && !(bool)(bVar1 >> 6)) {
                uVar34 = uVar33;
              }
              this[0x170] = (MenuTouchWindow)0x1;
              puVar6[0x11] = uVar34;
              RecordHandler::saveOptions((RecordHandler *)*puVar29);
            }
          }
          else if ((uVar21 & 0x7fffffff) == 2) {
            FModSound::play(*piVar31,(Vector *)0x7e,(Vector *)0x0,extraout_s0_00);
          }
        }
      }
      if (((*pcVar30 != '\0') && (*(int *)(this + 0xe8) != 0)) &&
         (iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xe8),param_1), iVar4 != 0)) {
        this_01 = (RecordHandler *)*puVar29;
        *(byte *)(puVar6 + 0x10) = *(byte *)(puVar6 + 0x10) ^ 1;
        RecordHandler::saveOptions(this_01);
        TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xe8),*(bool *)(puVar6 + 0x10));
        pSVar14 = *(String **)(this + 0x104);
        GameText::getText(*piVar13);
        ChoiceWindow::set(pSVar14);
        MVar3 = (MenuTouchWindow)0x1;
        goto LAB_0013ba58;
      }
    }
    break;
  case 4:
    ScrollTouchWindow::OnTouchEnd(*(int *)(this + 0xf0),param_1);
    for (uVar21 = 0; uVar21 < **(uint **)(this + 0xc0); uVar21 = uVar21 + 1) {
      piVar13 = *(int **)((*(uint **)(this + 0xc0))[1] + uVar21 * 4);
      iVar12 = *piVar13;
      iVar4 = piVar13[1];
      if (iVar12 == 0x6e && iVar4 == 0) {
        iVar4 = TouchButton::OnTouchEnd((int)piVar13,param_1);
        if (iVar4 != 0) {
          NFC::openTermsOfService();
        }
      }
      else if (iVar12 == 0x6a && iVar4 == 0) {
        iVar4 = TouchButton::OnTouchEnd((int)piVar13,param_1);
        if (iVar4 != 0) {
          NFC::rateGame();
          iVar4 = AbyssEngine::ApplicationManager::GetApplicationData();
          *(undefined1 *)(iVar4 + 0xe) = 1;
        }
      }
      else if (iVar12 == 0x6d && iVar4 == 0) {
        iVar4 = TouchButton::OnTouchEnd((int)piVar13,param_1);
        if (iVar4 != 0) {
          NFC::openPrivacyPolicy();
        }
      }
      else if ((iVar4 == 0 && iVar12 == 0x16) &&
              (iVar4 = TouchButton::OnTouchEnd((int)piVar13,param_1), iVar4 != 0)) {
        iVar4 = AbyssEngine::ApplicationManager::GetApplicationData();
        *(undefined1 *)(iVar4 + 0xd) = 1;
        NFC::rateGame();
      }
    }
    break;
  case 7:
    if ((this[0x170] == (MenuTouchWindow)0x0) ||
       (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 != 0)) {
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xd4),param_1);
      if (iVar4 != 0) {
        iVar4 = *(int *)(DAT_0013b14c + 0x13adea);
        bVar1 = *(byte *)(iVar4 + 0xd);
        *(byte *)(iVar4 + 0xd) = bVar1 ^ 1;
        TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xd4),bVar1 == 0);
        TouchSlider::setHalfTransparent
                  (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 4),
                   *(char *)(iVar4 + 0xd) == '\0');
        FModSound::enableCategory(**(int **)(DAT_0013b150 + 0x13ae1e),true);
      }
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xd8),param_1);
      if (iVar4 != 0) {
        iVar4 = *(int *)(DAT_0013b154 + 0x13ae3a);
        bVar1 = *(byte *)(iVar4 + 0xc);
        *(byte *)(iVar4 + 0xc) = bVar1 ^ 1;
        TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xd8),bVar1 == 0);
        TouchSlider::setHalfTransparent
                  (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 8),
                   *(char *)(iVar4 + 0xc) == '\0');
        piVar13 = *(int **)(DAT_0013b158 + 0x13ae6e);
        fVar35 = (float)FModSound::enableCategory(*piVar13,true);
        FModSound::play(*piVar13,(Vector *)0x7e,(Vector *)0x0,fVar35);
      }
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xdc),param_1);
      if (iVar4 != 0) {
        iVar4 = *(int *)(DAT_0013b15c + 0x13ae9e);
        bVar1 = *(byte *)(iVar4 + 0xe);
        *(byte *)(iVar4 + 0xe) = bVar1 ^ 1;
        TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xdc),bVar1 == 0);
        TouchSlider::setHalfTransparent
                  (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0xc),
                   *(char *)(iVar4 + 0xe) == '\0');
        FModSound::enableCategory(**(int **)(DAT_0013b160 + 0x13aed4),true);
      }
      pcVar19 = *(code **)(DAT_0013b9e0 + 0x13b924);
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 4));
      puVar6 = *(undefined4 **)(DAT_0013b9e4 + 0x13b930);
      *puVar6 = uVar36;
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 8));
      puVar6[1] = uVar36;
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 0xc));
      puVar6[2] = uVar36;
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 0x10));
      puVar6[9] = uVar36;
      uVar36 = (*pcVar19)(*(undefined4 *)(*(int *)(*(int *)(this + 0xec) + 4) + 0x14));
      puVar6[10] = uVar36;
      uVar21 = 1;
      piVar31 = *(int **)(DAT_0013b9e8 + 0x13b976);
      piVar13 = *(int **)(DAT_0013b9f4 + 0x13b982);
      while( true ) {
        puVar28 = *(uint **)(this + 0xec);
        uVar26 = *puVar28;
        if (uVar26 <= uVar21) break;
        if (uVar21 == 5) {
          uVar26 = (uint)*(byte *)(*piVar17 + 0x284);
        }
        if ((uVar21 != 5 || uVar26 != 0) &&
           (iVar4 = TouchSlider::OnTouchEnd(*(int *)(puVar28[1] + uVar21 * 4),param_1), iVar4 != 0))
        {
          if (uVar21 == 2) {
            FModSound::play(*piVar31,(Vector *)0x7e,(Vector *)0x0,extraout_s0_01);
          }
          else if (uVar21 == 5) {
            TouchSlider::getValue(*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x14));
            pSVar14 = *(String **)(this + 0x104);
            in_fpscr = in_fpscr & 0xfffffff;
            GameText::getText(*piVar13);
            ChoiceWindow::set(pSVar14);
            this[0x170] = (MenuTouchWindow)0x1;
          }
        }
        uVar21 = uVar21 + 1;
      }
    }
    else {
LAB_0013ba56:
      MVar3 = (MenuTouchWindow)0x0;
LAB_0013ba58:
      this[0x170] = MVar3;
    }
    break;
  case 8:
    MVar3 = (MenuTouchWindow)0x0;
    if (this[0x170] != (MenuTouchWindow)0x0) {
      MVar3 = this[0x176];
    }
    if ((this[0x170] != (MenuTouchWindow)0x0 && MVar3 != (MenuTouchWindow)0x0) &&
       (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 == 0)) {
      pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
      iVar4 = AbyssEngine::Engine::GetAccelValue(pEVar10);
      dVar37 = *(double *)(iVar4 + 0x10);
      pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
      pdVar11 = (double *)AbyssEngine::Engine::GetAccelValue(pEVar10);
      uVar21 = in_fpscr & 0xfffffff;
      uVar26 = uVar21 | (uint)(dVar37 < 0.0) << 0x1f | (uint)(dVar37 == 0.0) << 0x1e;
      in_fpscr = uVar26 | (uint)NAN(dVar37) << 0x1c;
      dVar37 = *pdVar11;
      bVar1 = (byte)(uVar26 >> 0x18);
      if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
        uVar21 = uVar21 | (uint)(dVar37 < 1.0) << 0x1f | (uint)(dVar37 == 1.0) << 0x1e;
        in_fpscr = uVar21 | (uint)NAN(dVar37) << 0x1c;
        bVar1 = (byte)(uVar21 >> 0x18);
        if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
          pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
          puVar6 = (undefined4 *)AbyssEngine::Engine::GetAccelValue(pEVar10);
          uVar36 = DAT_0013be20;
          *puVar6 = 0;
          puVar6[1] = uVar36;
        }
        pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        pdVar11 = (double *)AbyssEngine::Engine::GetAccelValue(pEVar10);
        dVar37 = (1.0 - *pdVar11) + 1.0;
      }
      iVar12 = *(int *)(DAT_0013be24 + 0x13bc82);
      *(float *)(iVar12 + 0x1c) = (float)dVar37;
      pEVar10 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
      iVar4 = AbyssEngine::Engine::GetAccelValue(pEVar10);
      *(float *)(iVar12 + 0x20) = (float)*(double *)(iVar4 + 0x10);
      this[0x176] = (MenuTouchWindow)0x0;
      this[0x170] = (MenuTouchWindow)0x0;
    }
    iVar12 = 0x14;
    iVar4 = *(int *)(DAT_0013b16c + 0x13afcc);
    if (*(char *)(iVar4 + 0x11) == '\0') {
      iVar12 = 0x18;
    }
    uVar36 = TouchSlider::getValue((TouchSlider *)**(undefined4 **)(*(int *)(this + 0xec) + 4));
    *(undefined4 *)(iVar4 + iVar12) = uVar36;
    if (this[0x108] != (MenuTouchWindow)0x0) {
      iVar12 = *piVar17;
      iVar24 = *(int *)(this + 0x19c) + *(int *)(iVar12 + 0x28);
      if ((((iVar24 < param_1) && (param_1 < iVar24 + *(int *)(this + 0x158))) &&
          (*(int *)(iVar12 + 0x28) + *(int *)(iVar12 + 0xc) < param_2)) &&
         (param_2 < *(int *)(iVar12 + 0x20) + *(int *)(iVar12 + 0xc) + *(int *)(this + 0x154))) {
        *(undefined1 *)(iVar4 + 0x11) = 1;
        fVar35 = (float)TouchButton::setHalfTransparent(SUB41(*(undefined4 *)(this + 0xd0),0));
        fVar35 = (float)TouchSlider::setValue
                                  ((TouchSlider *)**(undefined4 **)(*(int *)(this + 0xec) + 4),
                                   fVar35);
        FModSound::play(**(int **)(DAT_0013b170 + 0x13b040),(Vector *)0x7b,(Vector *)0x0,fVar35);
      }
    }
    if (this[0x109] != (MenuTouchWindow)0x0) {
      iVar12 = *piVar17;
      iVar24 = *(int *)(iVar12 + 0x28);
      if (((*(int *)(this + 0x158) + iVar24 + *(int *)(this + 0x19c) < param_1) &&
          (param_1 < (*(int *)(this + 0x19c) - iVar24) + *(int *)(this + 0x1a0))) &&
         ((iVar24 + *(int *)(iVar12 + 0xc) < param_2 &&
          (param_2 < *(int *)(iVar12 + 0x20) + *(int *)(iVar12 + 0xc) + *(int *)(this + 0x154))))) {
        *(undefined1 *)(iVar4 + 0x11) = 0;
        fVar35 = (float)TouchButton::setHalfTransparent(SUB41(*(undefined4 *)(this + 0xd0),0));
        fVar35 = (float)TouchSlider::setValue
                                  ((TouchSlider *)**(undefined4 **)(*(int *)(this + 0xec) + 4),
                                   fVar35);
        FModSound::play(**(int **)(DAT_0013b174 + 0x13b0b4),(Vector *)0x7b,(Vector *)0x0,fVar35);
      }
    }
    *(undefined2 *)(this + 0x108) = 0;
    iVar12 = TouchButton::OnTouchEnd(*(int *)(this + 0xcc),param_1);
    if (iVar12 != 0) {
      bVar1 = *(byte *)(iVar4 + 0x10);
      *(byte *)(iVar4 + 0x10) = bVar1 ^ 1;
      TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xcc),bVar1 == 0);
    }
    iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0xd0),param_1);
    if (iVar4 != 0) {
      pSVar14 = *(String **)(this + 0x104);
      GameText::getText(**(int **)(DAT_0013b178 + 0x13b104));
      ChoiceWindow::set(pSVar14);
      this[0x176] = (MenuTouchWindow)0x1;
      this[0x170] = (MenuTouchWindow)0x1;
    }
    TouchSlider::OnTouchEnd(**(int **)(*(int *)(this + 0xec) + 4),param_1);
    break;
  case 9:
    iVar4 = MissionsWindow::OnTouchEnd(*(int *)(this + 0xfc),param_1);
    if (iVar4 != 0) {
      uVar36 = 0;
LAB_0013bedc:
      *(undefined4 *)pMVar5 = uVar36;
    }
    break;
  case 10:
    if ((this[0x170] != (MenuTouchWindow)0x0) &&
       (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 == 0)) {
      MVar3 = (MenuTouchWindow)0x0;
      *(undefined4 *)(this + 0x16c) = 0;
      goto LAB_0013ba58;
    }
    break;
  case 0xb:
    if ((**(char **)(DAT_0013b9c8 + 0x13b750) != '\0') &&
       (**(char **)(DAT_0013b9cc + 0x13b75e) == '\0')) {
      if (param_3 != (void *)0x0) {
        if (*(void **)(this + 8) == param_3) {
          *(undefined4 *)(this + 8) = 0;
        }
        if (*(void **)(this + 0xc) == param_3) {
          *(undefined4 *)(this + 0xc) = 0;
        }
      }
      *(undefined2 *)(this + 0x98) = 0;
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0x14),param_1);
      if (iVar4 != 0) {
        *(undefined4 *)pMVar5 = 3;
      }
      iVar4 = TouchButton::OnTouchEnd(*(int *)(this + 0x18),param_1);
      if (iVar4 != 0) {
        iVar4 = 0x201;
        iVar12 = *(int *)(DAT_0013b9d0 + 0x13b7c2);
        pcVar19 = *(code **)(DAT_0013b9d4 + 0x13b7c8);
        uVar36 = 0x247;
        puVar6 = *(undefined4 **)(DAT_0013b9d8 + 0x13b7ce);
        fVar35 = *(float *)(iVar12 + 0x44);
        pGVar16 = (Globals *)*puVar6;
        puVar29 = *(undefined4 **)(DAT_0013b9dc + 0x13b7d8);
        if (!NAN(fVar35)) {
          uVar36 = 0x33e;
        }
        if (fVar35 <= 0.0) {
          uVar36 = 0x19f;
        }
        in_fpscr = in_fpscr & 0xfffffff | (uint)(fVar35 == 0.0) << 0x1e |
                   (uint)(0.0 <= fVar35) << 0x1d;
        *(undefined4 *)(iVar12 + 0x54) = uVar36;
        if (!NAN(fVar35)) {
          iVar4 = 0x2da;
        }
        bVar1 = (byte)(in_fpscr >> 0x18);
        uVar36 = *puVar29;
        if (!(bool)(bVar1 >> 5 & 1) || (bool)(bVar1 >> 6)) {
          iVar4 = 0x16d;
        }
        *(int *)(iVar12 + 0x58) = iVar4;
        iVar24 = (*pcVar19)(uVar36,*(undefined4 *)(this + 0x78));
        Globals::setCoordsFire
                  (pGVar16,iVar4,iVar24,*(uint *)(this + 0x78),*(uint *)(this + 0x7c),
                   (uint *)(this + 0x74),(ushort *)(this + 0x60),(ushort *)(this + 0x62),
                   (ushort *)(this + 100),(ushort *)(this + 0x66),(ushort *)(this + 0x68),
                   (ushort *)(this + 0x6a),(ushort *)(this + 0x6c),(ushort *)(this + 0x6e),
                   (ushort *)(this + 0x70),(ushort *)(this + 0x72),(ushort *)(this + 0x4c),
                   (ushort *)(this + 0x4e));
        iVar4 = *(int *)(iVar12 + 0x54);
        pGVar16 = (Globals *)*puVar6;
        iVar12 = (*pcVar19)(*puVar29,*(undefined4 *)(this + 0x1c));
        iVar24 = (*pcVar19)(*puVar29,*(undefined4 *)(this + 0x24));
        iVar9 = (*pcVar19)(*puVar29,*(undefined4 *)(this + 0x20));
        Globals::setCoordsSteer
                  (pGVar16,iVar4,iVar12,iVar24,iVar9,(ushort *)(this + 0x2e),(ushort *)(this + 0x30)
                   ,(ushort *)(this + 0x40),(ushort *)(this + 0x42),(ushort *)(this + 0x3c),
                   (ushort *)(this + 0x3e),(ushort *)(this + 0x34),(ushort *)(this + 0x32),
                   (ushort *)(this + 0x48),(ushort *)(this + 0x4a));
      }
    }
    break;
  case 0xc:
    if (this[0x174] == (MenuTouchWindow)0x0) {
      iVar4 = *(int *)(DAT_0013be0c + 0x13ba90);
      piVar13 = *(int **)(DAT_0013be14 + 0x13baa0);
      for (uVar21 = 0; uVar21 < **(uint **)(this + 0xb4); uVar21 = uVar21 + 1) {
        iVar12 = TouchButton::OnTouchEnd
                           (*(int *)((*(uint **)(this + 0xb4))[1] + uVar21 * 4),param_1);
        if (iVar12 != 0) {
          uVar36 = 0x3fc00000;
          if (uVar21 == 0) {
            uVar36 = 0x3f000000;
          }
          *(undefined4 *)(this + 0x1a4) = uVar36;
          *(undefined4 *)(iVar4 + 0x2c) = uVar36;
          if (uVar21 != 0) {
            pCVar20 = *(ChoiceWindow **)(this + 0x104);
            pSVar14 = (String *)GameText::getText(*piVar13);
            ChoiceWindow::set(pCVar20,pSVar14,true);
            this[0x174] = (MenuTouchWindow)0x1;
            goto LAB_0013bde4;
          }
          uVar38 = Status::getPlayingTime();
          if ((int)((ulonglong)uVar38 >> 0x20) < (int)(uint)((int)uVar38 == 0)) {
            iVar4 = *(int *)(this + 0x234);
            if (iVar4 != 2) goto LAB_0013bdf6;
            startSupernova(this);
            goto switchD_0013bf40_caseD_0;
          }
          pCVar20 = *(ChoiceWindow **)(this + 0x104);
          pSVar14 = (String *)GameText::getText(*piVar13);
          ChoiceWindow::set(pCVar20,pSVar14,true);
          this[0x174] = (MenuTouchWindow)0x1;
          this[0x170] = (MenuTouchWindow)0x1;
        }
      }
      break;
    }
    iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
    if (iVar4 == 1) {
      this[0x174] = (MenuTouchWindow)0x0;
      goto LAB_0013bdc0;
    }
    if (iVar4 != 0) goto switchD_0013bf40_caseD_0;
    iVar4 = *(int *)(this + 0x234);
    if (iVar4 == 2) {
      startSupernova(this);
    }
    else {
LAB_0013bdf6:
      if (iVar4 == 1) {
        startValkyrie(this);
      }
      else {
        startGOF2(this);
      }
    }
    goto switchD_0013bf40_caseD_b;
  case 0xd:
    if ((this[0x170] != (MenuTouchWindow)0x0) &&
       (iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1), iVar4 == 0)) {
LAB_0013b71e:
      this[0x170] = (MenuTouchWindow)0x0;
      goto switchD_0013bf40_caseD_0;
    }
    break;
  case 0xe:
    iVar4 = DAT_0013ad98 + 0x13ac04;
    piVar13 = *(int **)(DAT_0013ad94 + 0x13ac02);
    puVar6 = *(undefined4 **)(DAT_0013ad9c + 0x13ac0e);
    puVar29 = *(undefined4 **)(DAT_0013ada0 + 0x13ac16);
    for (uVar21 = 0; uVar21 < **(uint **)(this + 0xb0); uVar21 = uVar21 + 1) {
      iVar12 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(this + 0xb0))[1] + uVar21 * 4),param_1);
      if (iVar12 != 0) {
        iVar12 = GameText::getLanguage();
        puVar28 = *(uint **)(*(int *)(*(int *)(this + 0xb0) + 4) + uVar21 * 4);
        uVar26 = *puVar28;
        uVar27 = uVar26 - 0x1b;
        if (-(puVar28[1] - (uint)(uVar26 < 0x1b)) < (uint)(uVar27 < 0xd)) {
          uVar36 = *(undefined4 *)(iVar4 + uVar27 * 4);
          Globals::loadFont(*piVar13);
          GameText::setLanguage((short)*puVar6,(int)(short)uVar36);
          if (iVar12 == 1 || uVar27 == 1) {
            if ((FModSound *)*puVar29 != (FModSound *)0x0) {
              pvVar7 = (void *)FModSound::~FModSound((FModSound *)*puVar29);
              operator_delete(pvVar7);
            }
            *puVar29 = 0;
            this_00 = operator_new(0x243c);
            FModSound::FModSound(this_00);
            *puVar29 = this_00;
            FModSound::init(this_00,ctx);
            iVar12 = AbyssEngine::ApplicationManager::GetCurrentApplicationModule
                               ((ApplicationManager *)**(undefined4 **)(DAT_0013ada4 + 0x13acb0));
            uVar36 = 2;
            if (iVar12 == 5) {
              uVar36 = 1;
            }
            **(undefined4 **)(DAT_0013ada8 + 0x13acc2) = uVar36;
          }
          RecordHandler::saveOptions((RecordHandler *)**(undefined4 **)(DAT_0013adac + 0x13acce));
          Achievements::resetNewMedals((Achievements *)**(undefined4 **)(DAT_0013adb0 + 0x13acda));
          Layout::reload();
          puVar18 = *(undefined4 **)(DAT_0013adb4 + 0x13acee);
          iVar12 = AbyssEngine::ApplicationManager::GetCurrentApplicationModule
                             ((ApplicationManager *)*puVar18);
          if (iVar12 == 5) {
            *(undefined1 *)(**(int **)(DAT_0013adb8 + 0x13ad00) + 0x108) = 1;
          }
          this_02 = (ApplicationManager *)*puVar18;
          AbyssEngine::ApplicationManager::GetCurrentApplicationModule(this_02);
          AbyssEngine::ApplicationManager::SetCurrentApplicationModule((uint)this_02);
        }
      }
    }
    break;
  case 0xf:
    if (this[0x17a] != (MenuTouchWindow)0x0) {
      iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
LAB_0013ab9a:
      if (iVar4 != 0) goto switchD_0013bf40_caseD_0;
      iVar4 = AbyssEngine::ApplicationManager::GetApplicationData();
      *(undefined1 *)(iVar4 + 0x3c) = 1;
      this[0x17a] = (MenuTouchWindow)0x0;
      goto LAB_0013bdc0;
    }
    MVar3 = this[0x17d];
    bVar32 = MVar3 == (MenuTouchWindow)0x0;
    if (bVar32) {
      MVar3 = this[0x17c];
    }
    if (!bVar32 || MVar3 != (MenuTouchWindow)0x0) {
LAB_0013ba74:
      this[0x170] = (MenuTouchWindow)0x0;
      *(undefined2 *)(this + 0x17c) = 0;
      goto switchD_0013bf40_caseD_0;
    }
    if (this[0x170] != (MenuTouchWindow)0x0) {
      iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
      if (iVar4 == 0) {
        this[0x170] = (MenuTouchWindow)0x0;
        iVar4 = AbyssEngine::ApplicationManager::GetApplicationData();
        *(undefined1 *)(iVar4 + 0x40) = 0;
      }
      goto switchD_0013bf40_caseD_0;
    }
    ScrollTouchWindow::OnTouchEnd(*(int *)(this + 0xf4),param_1);
    iVar4 = *(int *)(DAT_0013c1cc + 0x13c08c);
    for (uVar21 = 0; uVar21 < **(uint **)(this + 0xc0); uVar21 = uVar21 + 1) {
      piVar13 = *(int **)((*(uint **)(this + 0xc0))[1] + uVar21 * 4);
      if (*piVar13 == 0x3c && piVar13[1] == 0) {
        iVar12 = TouchButton::OnTouchEnd((int)piVar13,param_1);
        if (iVar12 != 0) {
          iVar12 = AbyssEngine::ApplicationManager::GetApplicationData();
          *(undefined4 *)(iVar12 + 0x48) = 0xffffffff;
          iVar12 = AbyssEngine::ApplicationManager::GetApplicationData();
          *(undefined1 *)(iVar12 + 0x3f) = 1;
          this[0x170] = (MenuTouchWindow)0x0;
          this[400] = (MenuTouchWindow)0x1;
        }
      }
      else if ((piVar13[1] == 0 && *piVar13 == 0x34) &&
              (iVar12 = TouchButton::OnTouchEnd((int)piVar13,param_1), iVar12 != 0)) {
        switch(*(undefined4 *)(this + 0x1e0)) {
        case 0:
          if (*(char *)(iVar4 + 0x35) == '\0') {
            NFC::iap_buy_dlc_valkyrie();
          }
          break;
        case 1:
          if (*(char *)(iVar4 + 0x36) == '\0') {
            NFC::iap_buy_dlc_kaamo_club();
          }
          break;
        case 2:
          if (*(char *)(iVar4 + 0x37) == '\0') {
            NFC::iap_buy_dlc_supernova();
          }
          break;
        case 3:
          if (*(char *)(iVar4 + 0x38) == '\0') {
            NFC::iap_buy_dlc_vip();
          }
          break;
        case 4:
          if (*(char *)(iVar4 + 0x39) == '\0') {
            NFC::iap_buy_dlc_full_package();
          }
        }
        uVar36 = *(undefined4 *)(this + 0x1e0);
        iVar12 = AbyssEngine::ApplicationManager::GetApplicationData();
        *(undefined4 *)(iVar12 + 0x48) = uVar36;
        this[0x170] = (MenuTouchWindow)0x0;
      }
    }
    uVar21 = *(uint *)(this + 0x1e0);
    for (uVar26 = 0; uVar26 < **(uint **)(this + 0xf8); uVar26 = uVar26 + 1) {
      iVar4 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(this + 0xf8))[1] + uVar26 * 4),param_1);
      if (iVar4 != 0) {
        *(uint *)(this + 0x1e0) = uVar26;
        if (uVar21 == uVar26) {
          switch(uVar21 & 0x7fffffff) {
          case 0:
            NFC::iap_buy_dlc_valkyrie();
            break;
          case 1:
            NFC::iap_buy_dlc_kaamo_club();
            break;
          case 2:
            NFC::iap_buy_dlc_supernova();
            break;
          case 3:
            NFC::iap_buy_dlc_vip();
            break;
          case 4:
            NFC::iap_buy_dlc_full_package();
          }
        }
        else {
          pSVar23 = *(ScrollTouchWindow **)(this + 0xf4);
          AbyssEngine::String::String(aSStack_68,(char *)(DAT_0013c4d4 + 0x13c224),false);
          pSVar14 = (String *)GameText::getText(**(int **)(DAT_0013c4d8 + 0x13c230));
          AbyssEngine::String::String(aSStack_74,pSVar14,false);
          ScrollTouchWindow::setText(pSVar23,aSStack_68,aSStack_74);
          AbyssEngine::String::~String(aSStack_74);
          AbyssEngine::String::~String(aSStack_68);
        }
        break;
      }
    }
    if (this[0x1d9] != (MenuTouchWindow)0x0) {
      puVar28 = *(uint **)(DAT_0013c4e0 + 0x13c26e);
      iVar24 = **(int **)(DAT_0013c4dc + 0x13c26a);
      iVar12 = *(int *)(*piVar17 + 0x28);
      iVar4 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar28);
      if ((iVar24 - iVar12) - iVar4 < param_1) {
        AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar28);
      }
    }
    break;
  case 0x11:
    if (this[0x175] != (MenuTouchWindow)0x0) {
      iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x104),param_1);
      if (iVar4 == 0) {
        *(undefined4 *)pMVar5 = 0xc;
      }
      this[0x175] = (MenuTouchWindow)0x0;
      goto LAB_0013b71e;
    }
    for (uVar21 = 0; uVar21 < **(uint **)(this + 0xb8); uVar21 = uVar21 + 1) {
      iVar4 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(this + 0xb8))[1] + uVar21 * 4),param_1);
      if (iVar4 != 0) {
        *(uint *)(this + 0x234) = uVar21;
        if ((uVar21 & 0x7fffffff) == 2) {
          if (*(char *)(*(int *)(DAT_0013c1a4 + 0x13beca) + 0x37) == '\0') {
            NFC::iap_buy_dlc_supernova();
            goto switchD_0013bf40_caseD_0;
          }
        }
        else if ((uVar21 & 0x7fffffff) == 1) {
          if (*(char *)(*(int *)(DAT_0013be2c + 0x13bd06) + 0x35) == '\0') {
            NFC::iap_buy_dlc_valkyrie();
            goto switchD_0013bf40_caseD_0;
          }
        }
        else {
          *(undefined4 *)pMVar5 = 0xc;
        }
        uVar36 = 0xc;
        goto LAB_0013bedc;
      }
    }
  }
  if (*(int *)pMVar5 - 1U < 2) {
    iVar4 = *(int *)(this + 0x214);
    iVar12 = *(int *)(this + 0x194) + iVar4;
    uVar36 = VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
    if (iVar4 < 0) {
      iVar4 = -iVar4;
    }
    uVar33 = DAT_0013c1a8;
    if (3 < iVar4) {
      uVar33 = uVar36;
    }
    *(undefined4 *)(this + 0x218) = DAT_0013c1ac;
    this[0x224] = (MenuTouchWindow)0x0;
    *(int *)(this + 0x194) = iVar12;
    *(int *)(this + 0x210) = iVar12;
    *(undefined4 *)(this + 0x21c) = uVar33;
  }
  iVar4 = Layout::OnTouchEnd((Layout *)*piVar17,param_1,param_2);
  if (iVar4 != 0) {
    uVar36 = 0x11;
    switch(*(undefined4 *)pMVar5) {
    case 0:
      goto switchD_0013bf40_caseD_0;
    case 3:
      RecordHandler::saveOptions((RecordHandler *)**(undefined4 **)(DAT_0013c1b4 + 0x13bf6c));
    default:
      uVar36 = 0;
      break;
    case 6:
    case 7:
    case 8:
    case 0xe:
      uVar36 = 0;
      if (**(char **)(DAT_0013c1b0 + 0x13bf5a) == '\0') {
        uVar36 = 3;
      }
      break;
    case 0xb:
      goto switchD_0013bf40_caseD_b;
    case 0xc:
      break;
    }
    *(undefined4 *)pMVar5 = uVar36;
  }
switchD_0013bf40_caseD_b:
switchD_0013bf40_caseD_0:
  iVar4 = *piVar25 - local_5c;
  if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

```

## target disasm
```
  0013aabc: push {r4,r5,r6,r7,lr}
  0013aabe: add r7,sp,#0xc
  0013aac0: push {r7,r8,r9,r10,r11}
  0013aac4: vpush {d8,d9,d10,d11,d12,d13}
  0013aac8: sub sp,#0x118
  0013aaca: mov r11,r0
  0013aacc: ldr r0,[0x0013ad84]
  0013aace: ldr r4,[0x0013ad88]
  0013aad0: movs r5,#0x0
  0013aad2: add r0,pc
  0013aad4: add r4,pc
  0013aad6: ldr r6,[r0,#0x0]
  0013aad8: ldr r4,[r4,#0x0]
  0013aada: ldr r0,[r6,#0x0]
  0013aadc: str r0,[sp,#0x114]
  0013aade: strb.w r5,[r11,#0x1]
  0013aae2: strb.w r5,[r11,#0x224]
  0013aae6: ldr r0,[r4,#0x0]
  0013aae8: ldrb r5,[r0,#0x0]
  0013aaea: cbz r5,0x0013ab00
  0013aaec: blx 0x0007477c
  0013aaf0: movs r5,#0x0
  0013aaf2: cmp r0,#0x0
  0013aaf4: beq.w 0x0013bf78
  0013aaf8: ldr r0,[r4,#0x0]
  0013aafa: strb r5,[r0,#0x0]
  0013aafc: b.w 0x0013bf78
  0013ab00: strd r4,r2,[sp,#0x58]
  0013ab04: add.w r0,r11,#0x16c
  0013ab08: str r0,[sp,#0x54]
  0013ab0a: mov r4,r1
  0013ab0c: ldr.w r0,[r11,#0x16c]
  0013ab10: str r6,[sp,#0x50]
  0013ab12: cmp r0,#0x11
  0013ab14: bhi.w 0x0013bede
  0013ab18: tbh [pc,r0]
  0013ab40: ldrb.w r0,[r11,#0x170]
  0013ab44: cmp r0,#0x0
  0013ab46: beq.w 0x0013c30c
  0013ab4a: ldrb.w r0,[r11,#0x178]
  0013ab4e: ldr r5,[sp,#0x5c]
  0013ab50: cmp r0,#0x0
  0013ab52: beq.w 0x0013bcaa
  0013ab56: ldr.w r0,[r11,#0x104]
  0013ab5a: mov r1,r4
  0013ab5c: mov r2,r5
  0013ab5e: blx 0x00074770
  0013ab62: ldr r6,[sp,#0x50]
  0013ab64: movs r5,#0x0
  0013ab66: cmp r0,#0x1
  0013ab68: beq.w 0x0013bdbc
  0013ab6c: cmp r0,#0x0
  0013ab6e: bne.w 0x0013bf78
  0013ab72: ldr r0,[0x0013ad8c]
  0013ab74: add r0,pc
  0013ab76: ldr r0,[r0,#0x0]
  0013ab78: ldr r0,[r0,#0x0]
  0013ab7a: blx 0x0007663c
  0013ab7e: b.w 0x0013bf76
  0013ab82: ldrb.w r0,[r11,#0x17a]
  0013ab86: cmp r0,#0x0
  0013ab88: beq.w 0x0013ba5e
  0013ab8c: ldr.w r0,[r11,#0x104]
  0013ab90: mov r1,r4
  0013ab92: ldr r2,[sp,#0x5c]
  0013ab94: blx 0x00074770
  0013ab98: ldr r6,[sp,#0x50]
  0013ab9a: movs r5,#0x0
  0013ab9c: cmp r0,#0x0
  0013ab9e: bne.w 0x0013bf78
  0013aba2: ldr r0,[0x0013ad90]
  0013aba4: add r0,pc
  0013aba6: ldr r0,[r0,#0x0]
  0013aba8: ldr r0,[r0,#0x0]
  0013abaa: blx 0x00071704
  0013abae: movs r1,#0x1
  0013abb0: strb.w r1,[r0,#0x3c]
  0013abb4: strb.w r5,[r11,#0x17a]
  0013abb8: b.w 0x0013bdc0
  0013abbc: ldrb.w r0,[r11,#0x174]
  0013abc0: cmp r0,#0x0
  0013abc2: beq.w 0x0013ba80
  0013abc6: ldr.w r0,[r11,#0x104]
  0013abca: mov r1,r4
  0013abcc: ldr r2,[sp,#0x5c]
  0013abce: blx 0x00074770
  0013abd2: ldr r6,[sp,#0x50]
  0013abd4: movs r5,#0x0
  0013abd6: cmp r0,#0x1
  0013abd8: beq.w 0x0013bce4
  0013abdc: cmp r0,#0x0
  0013abde: bne.w 0x0013bf78
  0013abe2: ldr.w r0,[r11,#0x234]
  0013abe6: cmp r0,#0x2
  0013abe8: bne.w 0x0013bdf6
  0013abec: mov r0,r11
  0013abee: blx 0x00076648
  0013abf2: b.w 0x0013bf76
  0013abf6: ldr r0,[0x0013ad94]
  0013abf8: mov.w r10,#0x0
  0013abfc: ldr r1,[0x0013ad98]
  0013abfe: add r0,pc
  0013ac00: add r1,pc
  0013ac02: str r1,[sp,#0x4c]
  0013ac04: ldr r0,[r0,#0x0]
  0013ac06: str r0,[sp,#0x48]
  0013ac08: ldr r0,[0x0013ad9c]
  0013ac0a: add r0,pc
  0013ac0c: ldr r0,[r0,#0x0]
  0013ac0e: str r0,[sp,#0x44]
  0013ac10: ldr r0,[0x0013ada0]
  0013ac12: add r0,pc
  0013ac14: ldr.w r9,[r0,#0x0]
  0013ac18: b 0x0013ad1a
  0013ac1a: ldr r0,[r0,#0x4]
  0013ac1c: mov r1,r4
  0013ac1e: ldr r2,[sp,#0x5c]
  0013ac20: ldr.w r0,[r0,r10,lsl #0x2]
  0013ac24: blx 0x00074788
  0013ac28: cmp r0,#0x0
  0013ac2a: beq 0x0013ad16
  0013ac2c: blx 0x0006f544
  0013ac30: mov r5,r0
  0013ac32: ldr.w r0,[r11,#0xb0]
  0013ac36: ldr r0,[r0,#0x4]
  0013ac38: ldr.w r0,[r0,r10,lsl #0x2]
  0013ac3c: ldrd r0,r1,[r0,#0x0]
  0013ac40: subs.w r8,r0,#0x1b
  0013ac44: sbc r0,r1,#0x0
  0013ac48: rsbs.w r1,r8,#0xc
  0013ac4c: mov.w r1,#0x0
  0013ac50: sbcs.w r0,r1,r0
  0013ac54: bcc 0x0013ad16
  0013ac56: ldr r0,[sp,#0x4c]
  0013ac58: ldr.w r6,[r0,r8,lsl #0x2]
  0013ac5c: ldr r0,[sp,#0x48]
  0013ac5e: ldr r0,[r0,#0x0]
  0013ac60: mov r1,r6
  0013ac62: blx 0x00071d04
  0013ac66: ldr r0,[sp,#0x44]
  0013ac68: sxth r1,r6
  0013ac6a: movw r2,#0xd49
  0013ac6e: ldr r0,[r0,#0x0]
  0013ac70: blx 0x0006ffdc
  0013ac74: cmp r5,#0x1
  0013ac76: it ne
  0013ac78: cmp.ne.w r8,#0x1
  0013ac7c: bne 0x0013acc8
  0013ac7e: ldr.w r0,[r9,#0x0]
  0013ac82: cbz r0,0x0013ac8c
  0013ac84: blx 0x00075cdc
  0013ac88: blx 0x0006eb48
  0013ac8c: movs r0,#0x0
  0013ac8e: str.w r0,[r9,#0x0]
  0013ac92: movw r0,#0x243c
  0013ac96: blx 0x0006eb24
  0013ac9a: mov r5,r0
  0013ac9c: blx 0x00075d6c
  0013aca0: mov r0,r5
  0013aca2: str.w r5,[r9,#0x0]
  0013aca6: blx 0x00075d78
  0013acaa: ldr r0,[0x0013ada4]
  0013acac: add r0,pc
  0013acae: ldr r0,[r0,#0x0]
  0013acb0: ldr r0,[r0,#0x0]
  0013acb2: blx 0x0006e848
  0013acb6: ldr r1,[0x0013ada8]
  0013acb8: cmp r0,#0x5
  0013acba: mov.w r0,#0x2
  0013acbe: add r1,pc
  0013acc0: it eq
  0013acc2: mov.eq r0,#0x1
  0013acc4: ldr r1,[r1,#0x0]
  0013acc6: str r0,[r1,#0x0]
  0013acc8: ldr r0,[0x0013adac]
  0013acca: add r0,pc
  0013accc: ldr r0,[r0,#0x0]
  0013acce: ldr r0,[r0,#0x0]
  0013acd0: blx 0x00071788
  0013acd4: ldr r0,[0x0013adb0]
  0013acd6: add r0,pc
  0013acd8: ldr r0,[r0,#0x0]
  0013acda: ldr r0,[r0,#0x0]
  0013acdc: blx 0x00074620
  0013ace0: ldr r0,[sp,#0x58]
  0013ace2: ldr r0,[r0,#0x0]
  0013ace4: blx 0x00071d10
  0013ace8: ldr r0,[0x0013adb4]
  0013acea: add r0,pc
  0013acec: ldr r5,[r0,#0x0]
  0013acee: ldr r0,[r5,#0x0]
  0013acf0: blx 0x0006e848
  0013acf4: cmp r0,#0x5
  0013acf6: bne 0x0013ad06
  0013acf8: ldr r0,[0x0013adb8]
  0013acfa: movs r1,#0x1
  0013acfc: add r0,pc
  0013acfe: ldr r0,[r0,#0x0]
  0013ad00: ldr r0,[r0,#0x0]
  0013ad02: strb.w r1,[r0,#0x108]
  0013ad06: ldr r5,[r5,#0x0]
  0013ad08: mov r0,r5
  0013ad0a: blx 0x0006e848
  0013ad0e: mov r1,r0
  0013ad10: mov r0,r5
  0013ad12: blx 0x00071d64
  0013ad16: add.w r10,r10,#0x1
  0013ad1a: ldr.w r0,[r11,#0xb0]
  0013ad1e: ldr r1,[r0,#0x0]
  0013ad20: cmp r10,r1
  0013ad22: bcc.w 0x0013ac1a
  0013ad26: b.w 0x0013bede
  0013ad2a: ldrb.w r0,[r11,#0x170]
  0013ad2e: cmp r0,#0x0
  0013ad30: beq.w 0x0013bb0e
  0013ad34: ldrb.w r0,[r11,#0x1db]
  0013ad38: cmp r0,#0x0
  0013ad3a: bne.w 0x0013c062
  0013ad3e: ldrb.w r0,[r11,#0x1da]
  0013ad42: ldr r6,[sp,#0x50]
  0013ad44: cmp r0,#0x0
  0013ad46: bne.w 0x0013bf76
  0013ad4a: ldr.w r0,[r11,#0x104]
  0013ad4e: mov r1,r4
  0013ad50: ldr r2,[sp,#0x5c]
  0013ad52: ldrb.w r5,[r11,#0x173]
  0013ad56: blx 0x00074770
  0013ad5a: cmp r5,#0x0
  0013ad5c: beq.w 0x0013c00a
  0013ad60: movs r5,#0x0
  0013ad62: cmp r0,#0x1
  0013ad64: beq.w 0x0013c180
  0013ad68: cmp r0,#0x0
  0013ad6a: bne.w 0x0013bf78
  0013ad6e: ldr.w r1,[r11,#0x18c]
  0013ad72: mov r0,r11
  0013ad74: blx 0x00076654
  0013ad78: movs r5,#0x0
  0013ad7a: strb.w r5,[r11,#0x173]
  0013ad7e: b.w 0x0013bf78
  0013adbc: ldrb.w r0,[r11,#0x170]
  0013adc0: ldr r5,[sp,#0x5c]
  0013adc2: cbz r0,0x0013add6
  0013adc4: ldr.w r0,[r11,#0x104]
  0013adc8: mov r1,r4
  0013adca: mov r2,r5
  0013adcc: blx 0x00074770
  0013add0: cmp r0,#0x0
  0013add2: beq.w 0x0013ba56
  0013add6: ldr.w r0,[r11,#0xd4]
  0013adda: mov r1,r4
  0013addc: mov r2,r5
  0013adde: blx 0x00074788
  0013ade2: cbz r0,0x0013ae26
  0013ade4: ldr r0,[0x0013b14c]
  0013ade6: add r0,pc
  0013ade8: ldr r5,[r0,#0x0]
  0013adea: ldrb r1,[r5,#0xd]
  0013adec: eor r0,r1,#0x1
  0013adf0: strb r0,[r5,#0xd]
  0013adf2: clz r1,r1
  0013adf6: ldr.w r0,[r11,#0xd4]
  0013adfa: lsrs r1,r1,#0x5
  0013adfc: blx 0x00075acc
  0013ae00: ldr.w r0,[r11,#0xec]
  0013ae04: ldrb r1,[r5,#0xd]
  0013ae06: ldr r0,[r0,#0x4]
  0013ae08: clz r1,r1
  0013ae0c: lsrs r1,r1,#0x5
  0013ae0e: ldr r0,[r0,#0x4]
  0013ae10: blx 0x00076510
  0013ae14: ldr r0,[0x0013b150]
  0013ae16: movs r1,#0x1
  0013ae18: ldrb r2,[r5,#0xd]
  0013ae1a: add r0,pc
  0013ae1c: ldr r5,[sp,#0x5c]
  0013ae1e: ldr r0,[r0,#0x0]
  0013ae20: ldr r0,[r0,#0x0]
  0013ae22: blx 0x00074914
  0013ae26: ldr.w r0,[r11,#0xd8]
  0013ae2a: mov r1,r4
  0013ae2c: mov r2,r5
  0013ae2e: blx 0x00074788
  0013ae32: cbz r0,0x0013ae86
  0013ae34: ldr r0,[0x0013b154]
  0013ae36: add r0,pc
  0013ae38: ldr r5,[r0,#0x0]
  0013ae3a: ldrb r1,[r5,#0xc]
  0013ae3c: eor r0,r1,#0x1
  0013ae40: strb r0,[r5,#0xc]
  0013ae42: clz r1,r1
  0013ae46: ldr.w r0,[r11,#0xd8]
  0013ae4a: lsrs r1,r1,#0x5
  0013ae4c: blx 0x00075acc
  0013ae50: ldr.w r0,[r11,#0xec]
  0013ae54: ldrb r1,[r5,#0xc]
  0013ae56: ldr r0,[r0,#0x4]
  0013ae58: clz r1,r1
  0013ae5c: lsrs r1,r1,#0x5
  0013ae5e: ldr r0,[r0,#0x8]
  0013ae60: blx 0x00076510
  0013ae64: ldr r0,[0x0013b158]
  0013ae66: movs r1,#0x2
  0013ae68: ldrb r2,[r5,#0xc]
  0013ae6a: add r0,pc
  0013ae6c: ldr r5,[sp,#0x5c]
  0013ae6e: ldr r6,[r0,#0x0]
  0013ae70: ldr r0,[r6,#0x0]
  0013ae72: blx 0x00074914
  0013ae76: ldr r0,[r6,#0x0]
  0013ae78: movs r1,#0x0
  0013ae7a: str r1,[sp,#0x0]
  0013ae7c: movs r1,#0x7e
  0013ae7e: movs r2,#0x0
  0013ae80: movs r3,#0x0
  0013ae82: blx 0x00071548
  0013ae86: ldr.w r0,[r11,#0xdc]
  0013ae8a: mov r1,r4
  0013ae8c: mov r2,r5
  0013ae8e: blx 0x00074788
  0013ae92: cmp r0,#0x0
  0013ae94: beq.w 0x0013b914
  0013ae98: ldr r0,[0x0013b15c]
  0013ae9a: add r0,pc
  0013ae9c: ldr r5,[r0,#0x0]
  0013ae9e: ldrb r1,[r5,#0xe]
  0013aea0: eor r0,r1,#0x1
  0013aea4: strb r0,[r5,#0xe]
  0013aea6: clz r1,r1
  0013aeaa: ldr.w r0,[r11,#0xdc]
  0013aeae: lsrs r1,r1,#0x5
  0013aeb0: blx 0x00075acc
  0013aeb4: mov r6,r11
  0013aeb6: ldrb r1,[r5,#0xe]
  0013aeb8: ldr.w r0,[r6,#0xec]!
  0013aebc: clz r1,r1
  0013aec0: ldr r0,[r0,#0x4]
  0013aec2: lsrs r1,r1,#0x5
  0013aec4: ldr r0,[r0,#0xc]
  0013aec6: blx 0x00076510
  0013aeca: ldr r0,[0x0013b160]
  0013aecc: movs r1,#0x3
  0013aece: ldrb r2,[r5,#0xe]
  0013aed0: add r0,pc
  0013aed2: ldr r0,[r0,#0x0]
  0013aed4: ldr r0,[r0,#0x0]
  0013aed6: blx 0x00074914
  0013aeda: b.w 0x0013b918
  0013aede: ldrb.w r0,[r11,#0x170]
  0013aee2: cmp r0,#0x0
  0013aee4: beq.w 0x0013bede
  0013aee8: ldr.w r0,[r11,#0x104]
  0013aeec: mov r1,r4
  0013aeee: ldr r2,[sp,#0x5c]
  0013aef0: blx 0x00074770
  0013aef4: cmp r0,#0x0
  0013aef6: bne.w 0x0013bede
  0013aefa: movs r5,#0x0
  0013aefc: b.w 0x0013b71e
  0013af00: ldrb.w r0,[r11,#0x170]
  0013af04: ldr r5,[sp,#0x5c]
  0013af06: cbz r0,0x0013af4c
  0013af08: ldrb.w r0,[r11,#0x1db]
  0013af0c: cmp r0,#0x0
  0013af0e: bne.w 0x0013c062
  0013af12: ldrb.w r0,[r11,#0x1da]
  0013af16: ldr r6,[sp,#0x50]
  0013af18: cmp r0,#0x0
  0013af1a: bne.w 0x0013bf76
  0013af1e: ldr.w r0,[r11,#0x104]
  0013af22: mov r1,r4
  0013af24: mov r2,r5
  0013af26: blx 0x00074770
  0013af2a: cbnz r0,0x0013af46
  0013af2c: ldr.w r0,[r11,#0x104]
  0013af30: blx 0x00076660
  0013af34: cbz r0,0x0013af46
  0013af36: ldr.w r1,[r11,#0x18c]
  0013af3a: mov r0,r11
  0013af3c: blx 0x0007666c
  0013af40: cmp r0,#0x0
  0013af42: beq.w 0x0013bf76
  0013af46: movs r0,#0x0
  0013af48: strb.w r0,[r11,#0x170]
  0013af4c: ldr.w r0,[r11,#0xc4]
  0013af50: mov r1,r4
  0013af52: mov r2,r5
  0013af54: blx 0x00074788
  0013af58: cmp r0,#0x0
  0013af5a: beq.w 0x0013bede
  0013af5e: ldr.w r0,[r11,#0xbc]
  0013af62: ldr.w r1,[r11,#0x18c]
  0013af66: ldr r0,[r0,#0x4]
  0013af68: ldr.w r0,[r0,r1,lsl #0x2]
  0013af6c: cmp r0,#0x0
  0013af6e: beq.w 0x0013c062
  0013af72: ldr r0,[0x0013b164]
  0013af74: add r0,pc
  0013af76: ldr r0,[r0,#0x0]
  0013af78: ldr r0,[r0,#0x0]
  0013af7a: blx 0x00071710
  0013af7e: subs r0,#0x1
  0013af80: sbcs r0,r1,#0x0
  0013af84: blt.w 0x0013bf96
  0013af88: ldr r0,[0x0013b168]
  0013af8a: movs r1,#0x33
  0013af8c: ldr.w r5,[r11,#0x104]
  0013af90: add r0,pc
  0013af92: ldr r0,[r0,#0x0]
  0013af94: ldr r0,[r0,#0x0]
  0013af96: blx 0x00072f70
  0013af9a: mov r1,r0
  0013af9c: mov r0,r5
  0013af9e: movs r2,#0x1
  0013afa0: movs r6,#0x1
  0013afa2: blx 0x000746f8
  0013afa6: b.w 0x0013bde4
  0013afaa: ldrb.w r0,[r11,#0x170]
  0013afae: ldr.w r8,[sp,#0x58]
  0013afb2: cmp r0,#0x0
  0013afb4: itt ne
  0013afb6: ldrb.ne.w r0,[r11,#0x176]
  0013afba: cmp.ne r0,#0x0
  0013afbc: bne.w 0x0013bc04
  0013afc0: ldr r0,[0x0013b16c]
  0013afc2: movs r6,#0x14
  0013afc4: ldr.w r1,[r11,#0xec]
  0013afc8: add r0,pc
  0013afca: ldr r5,[r0,#0x0]
  0013afcc: ldr r0,[r1,#0x4]
  0013afce: ldrb r1,[r5,#0x11]
  0013afd0: ldr r0,[r0,#0x0]
  0013afd2: cmp r1,#0x0
  0013afd4: it eq
  0013afd6: mov.eq r6,#0x18
  0013afd8: blx 0x00072dd8
  0013afdc: str r0,[r5,r6]
  0013afde: ldrb.w r0,[r11,#0x108]
  0013afe2: cbz r0,0x0013b04a
  0013afe4: ldr.w r0,[r8,#0x0]
  0013afe8: ldr.w r2,[r11,#0x19c]
  0013afec: ldr r1,[r0,#0x28]
  0013afee: add r2,r1
  0013aff0: cmp r2,r4
  0013aff2: bge 0x0013b04a
  0013aff4: ldr.w r3,[r11,#0x158]
  0013aff8: add r2,r3
  0013affa: cmp r2,r4
  0013affc: ble 0x0013b04a
  0013affe: ldr r2,[r0,#0xc]
  0013b000: ldr r3,[sp,#0x5c]
  0013b002: add r1,r2
  0013b004: cmp r1,r3
  0013b006: bge 0x0013b04a
  0013b008: ldr r0,[r0,#0x20]
  0013b00a: ldr.w r1,[r11,#0x154]
  0013b00e: add r0,r2
  0013b010: add r0,r1
  0013b012: ldr r1,[sp,#0x5c]
  0013b014: cmp r0,r1
  0013b016: ble 0x0013b04a
  0013b018: movs r0,#0x1
  0013b01a: movs r1,#0x1
  0013b01c: strb r0,[r5,#0x11]
  0013b01e: ldr.w r0,[r11,#0xd0]
  0013b022: blx 0x00074fa4
  0013b026: ldr.w r0,[r11,#0xec]
  0013b02a: ldr r1,[r5,#0x14]
  0013b02c: ldr r0,[r0,#0x4]
  0013b02e: ldr r0,[r0,#0x0]
  0013b030: blx 0x00072de4
  0013b034: ldr r0,[0x0013b170]
  0013b036: movs r1,#0x0
  0013b038: str r1,[sp,#0x0]
  0013b03a: movs r1,#0x7b
  0013b03c: add r0,pc
  0013b03e: movs r2,#0x0
  0013b040: movs r3,#0x0
  0013b042: ldr r0,[r0,#0x0]
  0013b044: ldr r0,[r0,#0x0]
  0013b046: blx 0x00071548
  0013b04a: ldrb.w r0,[r11,#0x109]
  0013b04e: cbz r0,0x0013b0bc
  0013b050: ldr.w r0,[r8,#0x0]
  0013b054: ldr.w r2,[r11,#0x19c]
  0013b058: ldr.w r3,[r11,#0x158]
  0013b05c: ldr r1,[r0,#0x28]
  0013b05e: adds r6,r1,r2
  0013b060: add r3,r6
  0013b062: cmp r3,r4
  0013b064: bge 0x0013b0bc
  0013b066: ldr.w r3,[r11,#0x1a0]
  0013b06a: subs r2,r2,r1
  0013b06c: add r2,r3
  0013b06e: cmp r2,r4
  0013b070: ble 0x0013b0bc
  0013b072: ldr r2,[r0,#0xc]
  0013b074: ldr r3,[sp,#0x5c]
  0013b076: add r1,r2
  0013b078: cmp r1,r3
  0013b07a: bge 0x0013b0bc
  0013b07c: ldr r0,[r0,#0x20]
  0013b07e: ldr.w r1,[r11,#0x154]
  0013b082: add r0,r2
  0013b084: add r0,r1
  0013b086: ldr r1,[sp,#0x5c]
  0013b088: cmp r0,r1
  0013b08a: ble 0x0013b0bc
  0013b08c: movs r6,#0x0
  0013b08e: movs r1,#0x0
  0013b090: strb r6,[r5,#0x11]
  0013b092: ldr.w r0,[r11,#0xd0]
  0013b096: blx 0x00074fa4
  0013b09a: ldr.w r0,[r11,#0xec]
  0013b09e: ldr r1,[r5,#0x18]
  0013b0a0: ldr r0,[r0,#0x4]
  0013b0a2: ldr r0,[r0,#0x0]
  0013b0a4: blx 0x00072de4
  0013b0a8: ldr r0,[0x0013b174]
  0013b0aa: movs r1,#0x7b
  0013b0ac: movs r2,#0x0
  0013b0ae: movs r3,#0x0
  0013b0b0: add r0,pc
  0013b0b2: str r6,[sp,#0x0]
  0013b0b4: ldr r0,[r0,#0x0]
  0013b0b6: ldr r0,[r0,#0x0]
  0013b0b8: blx 0x00071548
  0013b0bc: ldr.w r0,[r11,#0xcc]
  0013b0c0: movs r1,#0x0
  0013b0c2: ldr r2,[sp,#0x5c]
  0013b0c4: strh.w r1,[r11,#0x108]
  0013b0c8: mov r1,r4
  0013b0ca: blx 0x00074788
  0013b0ce: cbz r0,0x0013b0e6
  0013b0d0: ldrb r0,[r5,#0x10]
  0013b0d2: eor r1,r0,#0x1
  0013b0d6: strb r1,[r5,#0x10]
  0013b0d8: clz r1,r0
  0013b0dc: ldr.w r0,[r11,#0xcc]
  0013b0e0: lsrs r1,r1,#0x5
  0013b0e2: blx 0x00075acc
  0013b0e6: ldr r5,[sp,#0x5c]
  0013b0e8: mov r1,r4
  0013b0ea: ldr.w r0,[r11,#0xd0]
  0013b0ee: mov r2,r5
  0013b0f0: blx 0x00074788
  0013b0f4: cbz r0,0x0013b11e
  0013b0f6: ldr r0,[0x0013b178]
  0013b0f8: movw r1,#0x1ed
  0013b0fc: ldr.w r5,[r11,#0x104]
  0013b100: add r0,pc
  0013b102: ldr r0,[r0,#0x0]
  0013b104: ldr r0,[r0,#0x0]
  0013b106: blx 0x00072f70
  0013b10a: mov r1,r0
  0013b10c: mov r0,r5
  0013b10e: ldr r5,[sp,#0x5c]
  0013b110: blx 0x00074794
  0013b114: movs r0,#0x1
  0013b116: strb.w r0,[r11,#0x176]
  0013b11a: strb.w r0,[r11,#0x170]
  0013b11e: ldr.w r0,[r11,#0xec]
  0013b122: mov r1,r4
  0013b124: mov r2,r5
  0013b126: ldr r0,[r0,#0x4]
  0013b128: ldr r0,[r0,#0x0]
  0013b12a: blx 0x00076678
  0013b12e: b.w 0x0013bede
  0013b132: ldr.w r0,[r11,#0xfc]
  0013b136: mov r1,r4
  0013b138: ldr r2,[sp,#0x5c]
  0013b13a: blx 0x0007522c
  0013b13e: cmp r0,#0x0
  0013b140: beq.w 0x0013bede
  0013b144: ldr r1,[sp,#0x54]
  0013b146: movs r0,#0x0
  0013b148: b.w 0x0013bedc
  0013b17c: ldr r0,[0x0013b50c]
  0013b17e: add r0,pc
  0013b180: ldr r0,[r0,#0x0]
  0013b182: ldrb r0,[r0,#0x0]
  0013b184: cmp r0,#0x0
  0013b186: beq.w 0x0013bb84
  0013b18a: ldr r5,[sp,#0x5c]
  0013b18c: mov r1,r4
  0013b18e: ldr.w r0,[r11,#0xe4]
  0013b192: mov r2,r5
  0013b194: blx 0x00074788
  0013b198: ldr r1,[sp,#0x54]
  0013b19a: cmp r0,#0x0
  0013b19c: itt ne
  0013b19e: mov.ne r0,#0xb
  0013b1a0: str.ne r0,[r1,#0x0]
  0013b1a2: ldrb.w r0,[r11,#0x170]
  0013b1a6: ldr.w r8,[sp,#0x58]
  0013b1aa: cmp r0,#0x0
  0013b1ac: itt ne
  0013b1ae: ldrb.ne.w r0,[r11,#0x176]
  0013b1b2: cmp.ne r0,#0x0
  0013b1b4: bne.w 0x0013bd16
  0013b1b8: ldr r0,[0x0013b510]
  0013b1ba: movs r5,#0x14
  0013b1bc: ldr.w r1,[r11,#0xec]
  0013b1c0: add r0,pc
  0013b1c2: ldr r6,[r0,#0x0]
  0013b1c4: ldr r0,[r1,#0x4]
  0013b1c6: ldrb r1,[r6,#0x11]
  0013b1c8: ldr r0,[r0,#0x0]
  0013b1ca: cmp r1,#0x0
  0013b1cc: it eq
  0013b1ce: mov.eq r5,#0x18
  0013b1d0: blx 0x00072dd8
  0013b1d4: str r0,[r6,r5]
  0013b1d6: mov r9,r6
  0013b1d8: ldrb.w r0,[r11,#0x108]
  0013b1dc: ldr r5,[sp,#0x5c]
  0013b1de: cbz r0,0x0013b248
  0013b1e0: ldr.w r0,[r8,#0x0]
  0013b1e4: ldr.w r2,[r11,#0x19c]
  0013b1e8: ldr r1,[r0,#0x28]
  0013b1ea: add r2,r1
  0013b1ec: cmp r2,r4
  0013b1ee: bge 0x0013b248
  0013b1f0: ldr.w r3,[r11,#0x158]
  0013b1f4: add r2,r3
  0013b1f6: cmp r2,r4
  0013b1f8: ble 0x0013b248
  0013b1fa: ldr r2,[r0,#0xc]
  0013b1fc: add r1,r2
  0013b1fe: cmp r1,r5
  0013b200: bge 0x0013b248
  0013b202: ldr r0,[r0,#0x20]
  0013b204: ldr.w r1,[r11,#0x154]
  0013b208: add r0,r2
  0013b20a: add r0,r1
  0013b20c: cmp r0,r5
  0013b20e: ble 0x0013b248
  0013b210: movs r0,#0x1
  0013b212: movs r1,#0x1
  0013b214: strb.w r0,[r9,#0x11]
  0013b218: mov r6,r9
  0013b21a: ldr.w r0,[r11,#0xd0]
  0013b21e: blx 0x00074fa4
  0013b222: ldr.w r0,[r11,#0xec]
  0013b226: ldr.w r1,[r9,#0x14]
  0013b22a: ldr r0,[r0,#0x4]
  0013b22c: ldr r0,[r0,#0x0]
  0013b22e: blx 0x00072de4
  0013b232: ldr r0,[0x0013b514]
  0013b234: movs r1,#0x0
  0013b236: str r1,[sp,#0x0]
  0013b238: movs r1,#0x7b
  0013b23a: add r0,pc
  0013b23c: movs r2,#0x0
  0013b23e: movs r3,#0x0
  0013b240: ldr r0,[r0,#0x0]
  0013b242: ldr r0,[r0,#0x0]
  0013b244: blx 0x00071548
  0013b248: ldrb.w r0,[r11,#0x109]
  0013b24c: cbz r0,0x0013b2be
  0013b24e: ldr.w r0,[r8,#0x0]
  0013b252: ldr.w r2,[r11,#0x19c]
  0013b256: ldr.w r3,[r11,#0x158]
  0013b25a: ldr r1,[r0,#0x28]
  0013b25c: adds r6,r1,r2
  0013b25e: add r3,r6
  0013b260: cmp r3,r4
  0013b262: bge 0x0013b2be
  0013b264: ldr.w r3,[r11,#0x1a0]
  0013b268: subs r2,r2,r1
  0013b26a: add r2,r3
  0013b26c: cmp r2,r4
  0013b26e: ble 0x0013b2be
  0013b270: ldr r2,[r0,#0xc]
  0013b272: add r1,r2
  0013b274: cmp r1,r5
  0013b276: bge 0x0013b2be
  0013b278: ldr r0,[r0,#0x20]
  0013b27a: ldr.w r1,[r11,#0x154]
  0013b27e: add r0,r2
  0013b280: add r0,r1
  0013b282: cmp r0,r5
  0013b284: ble 0x0013b2be
  0013b286: movs r5,#0x0
  0013b288: movs r1,#0x0
  0013b28a: strb.w r5,[r9,#0x11]
  0013b28e: mov r6,r9
  0013b290: ldr.w r0,[r11,#0xd0]
  0013b294: blx 0x00074fa4
  0013b298: ldr.w r0,[r11,#0xec]
  0013b29c: ldr.w r1,[r9,#0x18]
  0013b2a0: ldr r0,[r0,#0x4]
  0013b2a2: ldr r0,[r0,#0x0]
  0013b2a4: blx 0x00072de4
  0013b2a8: ldr r0,[0x0013b518]
  0013b2aa: movs r1,#0x7b
  0013b2ac: movs r2,#0x0
  0013b2ae: movs r3,#0x0
  0013b2b0: add r0,pc
  0013b2b2: str r5,[sp,#0x0]
  0013b2b4: ldr r5,[sp,#0x5c]
  0013b2b6: ldr r0,[r0,#0x0]
  0013b2b8: ldr r0,[r0,#0x0]
  0013b2ba: blx 0x00071548
  0013b2be: ldr.w r0,[r11,#0xcc]
  0013b2c2: movs r1,#0x0
  0013b2c4: strh.w r1,[r11,#0x108]
  0013b2c8: mov r1,r4
  0013b2ca: mov r2,r5
  0013b2cc: blx 0x00074788
  0013b2d0: cmp r0,#0x0
  0013b2d2: mov r8,r9
  0013b2d4: beq 0x0013b2f0
  0013b2d6: ldrb.w r0,[r8,#0x10]
  0013b2da: eor r1,r0,#0x1
  0013b2de: strb.w r1,[r8,#0x10]
  0013b2e2: clz r1,r0
  0013b2e6: ldr.w r0,[r11,#0xcc]
  0013b2ea: lsrs r1,r1,#0x5
  0013b2ec: blx 0x00075acc
  0013b2f0: ldr.w r0,[r11,#0xd0]
  0013b2f4: mov r1,r4
  0013b2f6: mov r2,r5
  0013b2f8: blx 0x00074788
  0013b2fc: cbz r0,0x0013b326
  0013b2fe: ldr r0,[0x0013b51c]
  0013b300: movw r1,#0x1ed
  0013b304: ldr.w r5,[r11,#0x104]
  0013b308: add r0,pc
  0013b30a: ldr r0,[r0,#0x0]
  0013b30c: ldr r0,[r0,#0x0]
  0013b30e: blx 0x00072f70
  0013b312: mov r1,r0
  0013b314: mov r0,r5
  0013b316: ldr r5,[sp,#0x5c]
  0013b318: blx 0x00074794
  0013b31c: movs r0,#0x1
  0013b31e: strb.w r0,[r11,#0x176]
  0013b322: strb.w r0,[r11,#0x170]
  0013b326: ldr.w r0,[r11,#0xec]
  0013b32a: mov r1,r4
  0013b32c: mov r2,r5
  0013b32e: ldr r0,[r0,#0x4]
  0013b330: ldr r0,[r0,#0x0]
  0013b332: blx 0x00076678
  0013b336: ldrb.w r0,[r11,#0x170]
  0013b33a: cbz r0,0x0013b34e
  0013b33c: ldr.w r0,[r11,#0x104]
  0013b340: mov r1,r4
  0013b342: mov r2,r5
  0013b344: blx 0x00074770
  0013b348: cmp r0,#0x0
  0013b34a: beq.w 0x0013ba56
  0013b34e: ldr.w r0,[r11,#0xd4]
  0013b352: mov r1,r4
  0013b354: mov r2,r5
  0013b356: blx 0x00074788
  0013b35a: cbz r0,0x0013b39e
  0013b35c: ldrb.w r1,[r8,#0xd]
  0013b360: eor r0,r1,#0x1
  0013b364: strb.w r0,[r8,#0xd]
  0013b368: clz r1,r1
  0013b36c: ldr.w r0,[r11,#0xd4]
  0013b370: lsrs r1,r1,#0x5
  0013b372: blx 0x00075acc
  0013b376: ldr.w r0,[r11,#0xec]
  0013b37a: ldrb.w r1,[r8,#0xd]
  0013b37e: ldr r0,[r0,#0x4]
  0013b380: clz r1,r1
  0013b384: lsrs r1,r1,#0x5
  0013b386: ldr r0,[r0,#0x4]
  0013b388: blx 0x00076510
  0013b38c: ldr r0,[0x0013b520]
  0013b38e: movs r1,#0x1
  0013b390: ldrb.w r2,[r8,#0xd]
  0013b394: add r0,pc
  0013b396: ldr r0,[r0,#0x0]
  0013b398: ldr r0,[r0,#0x0]
  0013b39a: blx 0x00074914
  0013b39e: ldr.w r0,[r11,#0xd8]
  0013b3a2: mov r1,r4
  0013b3a4: mov r2,r5
  0013b3a6: blx 0x00074788
  0013b3aa: cbz r0,0x0013b400
  0013b3ac: ldrb.w r1,[r8,#0xc]
  0013b3b0: eor r0,r1,#0x1
  0013b3b4: strb.w r0,[r8,#0xc]
  0013b3b8: clz r1,r1
  0013b3bc: ldr.w r0,[r11,#0xd8]
  0013b3c0: lsrs r1,r1,#0x5
  0013b3c2: blx 0x00075acc
  0013b3c6: ldr.w r0,[r11,#0xec]
  0013b3ca: ldrb.w r1,[r8,#0xc]
  0013b3ce: ldr r0,[r0,#0x4]
  0013b3d0: clz r1,r1
  0013b3d4: lsrs r1,r1,#0x5
  0013b3d6: ldr r0,[r0,#0x8]
  0013b3d8: blx 0x00076510
  0013b3dc: ldr r0,[0x0013b524]
  0013b3de: movs r1,#0x2
  0013b3e0: ldrb.w r2,[r8,#0xc]
  0013b3e4: add r0,pc
  0013b3e6: ldr r5,[r0,#0x0]
  0013b3e8: ldr r0,[r5,#0x0]
  0013b3ea: blx 0x00074914
  0013b3ee: ldr r0,[r5,#0x0]
  0013b3f0: movs r1,#0x0
  0013b3f2: str r1,[sp,#0x0]
  0013b3f4: movs r1,#0x7e
  0013b3f6: movs r2,#0x0
  0013b3f8: movs r3,#0x0
  0013b3fa: ldr r5,[sp,#0x5c]
  0013b3fc: blx 0x00071548
  0013b400: ldr.w r0,[r11,#0xdc]
  0013b404: mov r1,r4
  0013b406: mov r2,r5
  0013b408: blx 0x00074788
  0013b40c: cbz r0,0x0013b450
  0013b40e: ldrb.w r1,[r8,#0xe]
  0013b412: eor r0,r1,#0x1
  0013b416: strb.w r0,[r8,#0xe]
  0013b41a: clz r1,r1
  0013b41e: ldr.w r0,[r11,#0xdc]
  0013b422: lsrs r1,r1,#0x5
  0013b424: blx 0x00075acc
  0013b428: ldr.w r0,[r11,#0xec]
  0013b42c: ldrb.w r1,[r8,#0xe]
  0013b430: ldr r0,[r0,#0x4]
  0013b432: clz r1,r1
  0013b436: lsrs r1,r1,#0x5
  0013b438: ldr r0,[r0,#0xc]
  0013b43a: blx 0x00076510
  0013b43e: ldr r0,[0x0013b528]
  0013b440: movs r1,#0x3
  0013b442: ldrb.w r2,[r8,#0xe]
  0013b446: add r0,pc
  0013b448: ldr r0,[r0,#0x0]
  0013b44a: ldr r0,[r0,#0x0]
  0013b44c: blx 0x00074914
  0013b450: ldr.w r0,[r11,#0xec]
  0013b454: ldr r1,[0x0013b52c]
  0013b456: ldr r0,[r0,#0x4]
  0013b458: add r1,pc
  0013b45a: ldr r5,[r1,#0x0]
  0013b45c: ldr r0,[r0,#0x4]
  0013b45e: blx r5
  0013b460: str.w r0,[r8,#0x0]
  0013b464: ldr.w r0,[r11,#0xec]
  0013b468: ldr r0,[r0,#0x4]
  0013b46a: ldr r0,[r0,#0x8]
  0013b46c: blx r5
  0013b46e: str.w r0,[r8,#0x4]
  0013b472: ldr.w r0,[r11,#0xec]
  0013b476: ldr r0,[r0,#0x4]
  0013b478: ldr r0,[r0,#0xc]
  0013b47a: blx r5
  0013b47c: str.w r0,[r8,#0x8]
  0013b480: ldr.w r0,[r11,#0xec]
  0013b484: ldr r0,[r0,#0x4]
  0013b486: ldr r0,[r0,#0x10]
  0013b488: blx r5
  0013b48a: str.w r0,[r8,#0x24]
  0013b48e: ldr.w r0,[r11,#0xec]
  0013b492: ldr r0,[r0,#0x4]
  0013b494: ldr r0,[r0,#0x14]
  0013b496: blx r5
  0013b498: str.w r0,[r8,#0x28]
  0013b49c: vmov.f32 s20,0x3f800000
  0013b4a0: ldr r0,[0x0013b530]
  0013b4a2: vmov.f32 s22,0x3f000000
  0013b4a6: vldr.32 s16,[pc,#0x8c]
  0013b4aa: movs r6,#0x1
  0013b4ac: add r0,pc
  0013b4ae: vldr.32 s18,[pc,#0x88]
  0013b4b2: vldr.32 s24,[pc,#0x88]
  0013b4b6: ldr.w r10,[r0,#0x0]
  0013b4ba: ldr r0,[0x0013b540]
  0013b4bc: add r0,pc
  0013b4be: ldr r0,[r0,#0x0]
  0013b4c0: str r0,[sp,#0x4c]
  0013b4c2: ldr r0,[0x0013b544]
  0013b4c4: add r0,pc
  0013b4c6: ldr r0,[r0,#0x0]
  0013b4c8: str r0,[sp,#0x48]
  0013b4ca: ldr r0,[0x0013b548]
  0013b4cc: ldr r5,[sp,#0x5c]
  0013b4ce: add r0,pc
  0013b4d0: ldr.w r9,[r0,#0x0]
  0013b4d4: b 0x0013b5ee
  0013b4d6: ldr r0,[r0,#0x4]
  0013b4d8: mov r1,r4
  0013b4da: mov r2,r5
  0013b4dc: ldr.w r0,[r0,r6,lsl #0x2]
  0013b4e0: blx 0x00076678
  0013b4e4: cmp r0,#0x0
  0013b4e6: beq.w 0x0013b5ec
  0013b4ea: bic r0,r6,#0x80000000
  0013b4ee: cmp r0,#0x5
  0013b4f0: beq 0x0013b54c
  0013b4f2: cmp r0,#0x2
  0013b4f4: bne 0x0013b5ec
  0013b4f6: ldr.w r0,[r9,#0x0]
  0013b4fa: movs r1,#0x0
  0013b4fc: str r1,[sp,#0x0]
  0013b4fe: movs r1,#0x7e
  0013b500: movs r2,#0x0
  0013b502: movs r3,#0x0
  0013b504: blx 0x00071548
  0013b508: b 0x0013b5ec
  0013b54c: ldr.w r0,[r11,#0xec]
  0013b550: ldr r0,[r0,#0x4]
  0013b552: ldr r0,[r0,#0x14]
  0013b554: blx 0x00072dd8
  0013b558: vmov s26,r0
  0013b55c: ldrb.w r0,[r10,#0x0]
  0013b560: cbz r0,0x0013b5b0
  0013b562: ldr r0,[sp,#0x4c]
  0013b564: movs r1,#0x7a
  0013b566: ldr.w r5,[r11,#0x104]
  0013b56a: ldr r0,[r0,#0x0]
  0013b56c: blx 0x00072f70
  0013b570: mov r1,r0
  0013b572: mov r0,r5
  0013b574: blx 0x00074794
  0013b578: vcmpe.f32 s26,s18
  0013b57c: movs r0,#0x1
  0013b57e: vmrs apsr,fpscr
  0013b582: vmov.f32 s0,s20
  0013b586: vcmpe.f32 s26,s16
  0013b58a: vmov.f32 s2,s24
  0013b58e: it ls
  0013b590: vmov.ls.f32 s0,s22
  0013b594: vmrs apsr,fpscr
  0013b598: it hi
  0013b59a: vmov.hi.f32 s2,s0
  0013b59e: strb.w r0,[r11,#0x170]
  0013b5a2: ldr r0,[sp,#0x48]
  0013b5a4: vstr.32 s2,[r8,#0x44]
  0013b5a8: ldr r0,[r0,#0x0]
  0013b5aa: blx 0x00071788
  0013b5ae: b 0x0013b5ea
  0013b5b0: vcmpe.f32 s26,s18
  0013b5b4: mov.w r1,#0x200
  0013b5b8: vmrs apsr,fpscr
  0013b5bc: vcmpe.f32 s26,s16
  0013b5c0: ldr.w r5,[r11,#0x104]
  0013b5c4: it ls
  0013b5c6: movw.ls r1,#0x1ff
  0013b5ca: vmrs apsr,fpscr
  0013b5ce: ldr r0,[sp,#0x4c]
  0013b5d0: ldr r0,[r0,#0x0]
  0013b5d2: it ls
  0013b5d4: mov.ls.w r1,#0x1fe
  0013b5d8: blx 0x00072f70
  0013b5dc: mov r1,r0
  0013b5de: mov r0,r5
  0013b5e0: blx 0x00074794
  0013b5e4: movs r0,#0x1
  0013b5e6: strb.w r0,[r11,#0x170]
  0013b5ea: ldr r5,[sp,#0x5c]
  0013b5ec: adds r6,#0x1
  0013b5ee: ldr.w r0,[r11,#0xec]
  0013b5f2: ldr r1,[r0,#0x0]
  0013b5f4: cmp r6,r1
  0013b5f6: bcc.w 0x0013b4d6
  0013b5fa: ldrb.w r0,[r10,#0x0]
  0013b5fe: cmp r0,#0x0
  0013b600: beq.w 0x0013bede
  0013b604: ldr.w r0,[r11,#0xe8]
  0013b608: cmp r0,#0x0
  0013b60a: beq.w 0x0013bede
  0013b60e: ldr r2,[sp,#0x5c]
  0013b610: mov r1,r4
  0013b612: blx 0x00074788
  0013b616: cmp r0,#0x0
  0013b618: beq.w 0x0013bede
  0013b61c: ldr r0,[sp,#0x48]
  0013b61e: ldrb.w r1,[r8,#0x40]
  0013b622: ldr r0,[r0,#0x0]
  0013b624: eor r1,r1,#0x1
  0013b628: strb.w r1,[r8,#0x40]
  0013b62c: blx 0x00071788
  0013b630: ldrb.w r1,[r8,#0x40]
  0013b634: ldr.w r0,[r11,#0xe8]
  0013b638: blx 0x00075acc
  0013b63c: ldr r0,[sp,#0x4c]
  0013b63e: movs r1,#0x7a
  0013b640: ldr.w r5,[r11,#0x104]
  0013b644: ldr r0,[r0,#0x0]
  0013b646: blx 0x00072f70
  0013b64a: mov r1,r0
  0013b64c: mov r0,r5
  0013b64e: blx 0x00074794
  0013b652: movs r0,#0x1
  0013b654: b 0x0013ba58
  0013b656: ldr.w r0,[r11,#0xf0]
  0013b65a: mov r1,r4
  0013b65c: ldr r2,[sp,#0x5c]
  0013b65e: blx 0x00075bc8
  0013b662: ldr r0,[0x0013b9c4]
  0013b664: movs r5,#0x0
  0013b666: mov.w r9,#0x1
  0013b66a: add r0,pc
  0013b66c: ldr r6,[r0,#0x0]
  0013b66e: b 0x0013b6ee
  0013b670: ldr r0,[r0,#0x4]
  0013b672: ldr.w r0,[r0,r5,lsl #0x2]
  0013b676: ldrd r2,r1,[r0,#0x0]
  0013b67a: eor r3,r2,#0x6e
  0013b67e: orrs r3,r1
  0013b680: beq 0x0013b6de
  0013b682: eor r3,r2,#0x6a
  0013b686: orrs r3,r1
  0013b688: beq 0x0013b6b4
  0013b68a: eor r3,r2,#0x6d
  0013b68e: orrs r3,r1
  0013b690: beq 0x0013b6ce
  0013b692: eor r2,r2,#0x16
  0013b696: orrs r1,r2
  0013b698: bne 0x0013b6ec
  0013b69a: ldr r2,[sp,#0x5c]
  0013b69c: mov r1,r4
  0013b69e: blx 0x00074788
  0013b6a2: cbz r0,0x0013b6ec
  0013b6a4: ldr r0,[r6,#0x0]
  0013b6a6: blx 0x00071704
  0013b6aa: strb.w r9,[r0,#0xd]
  0013b6ae: blx 0x000751f0
  0013b6b2: b 0x0013b6ec
  0013b6b4: ldr r2,[sp,#0x5c]
  0013b6b6: mov r1,r4
  0013b6b8: blx 0x00074788
  0013b6bc: cbz r0,0x0013b6ec
  0013b6be: blx 0x000751f0
  0013b6c2: ldr r0,[r6,#0x0]
  0013b6c4: blx 0x00071704
  0013b6c8: strb.w r9,[r0,#0xe]
  0013b6cc: b 0x0013b6ec
  0013b6ce: ldr r2,[sp,#0x5c]
  0013b6d0: mov r1,r4
  0013b6d2: blx 0x00074788
  0013b6d6: cbz r0,0x0013b6ec
  0013b6d8: blx 0x00076684
  0013b6dc: b 0x0013b6ec
  0013b6de: ldr r2,[sp,#0x5c]
  0013b6e0: mov r1,r4
  0013b6e2: blx 0x00074788
  0013b6e6: cbz r0,0x0013b6ec
  0013b6e8: blx 0x00076690
  0013b6ec: adds r5,#0x1
  0013b6ee: ldr.w r0,[r11,#0xc0]
  0013b6f2: ldr r1,[r0,#0x0]
  0013b6f4: cmp r5,r1
  0013b6f6: bcc 0x0013b670
  0013b6f8: b 0x0013bede
  0013b6fa: ldrb.w r0,[r11,#0x175]
  0013b6fe: cmp r0,#0x0
  0013b700: beq.w 0x0013bb5e
  0013b704: ldr.w r0,[r11,#0x104]
  0013b708: mov r1,r4
  0013b70a: ldr r2,[sp,#0x5c]
  0013b70c: blx 0x00074770
  0013b710: cbnz r0,0x0013b718
  0013b712: ldr r1,[sp,#0x54]
  0013b714: movs r0,#0xc
  0013b716: str r0,[r1,#0x0]
  0013b718: movs r5,#0x0
  0013b71a: strb.w r5,[r11,#0x175]
  0013b71e: strb.w r5,[r11,#0x170]
  0013b722: b.w 0x0013c064
  0013b726: ldrb.w r0,[r11,#0x170]
  0013b72a: cmp r0,#0x0
  0013b72c: beq.w 0x0013bede
  0013b730: ldr.w r0,[r11,#0x104]
  0013b734: mov r1,r4
  0013b736: ldr r2,[sp,#0x5c]
  0013b738: blx 0x00074770
  0013b73c: cmp r0,#0x0
  0013b73e: bne.w 0x0013bede
  0013b742: movs r0,#0x0
  0013b744: str.w r0,[r11,#0x16c]
  0013b748: b 0x0013ba58
  0013b74a: ldr r0,[0x0013b9c8]
  0013b74c: add r0,pc
  0013b74e: ldr r0,[r0,#0x0]
  0013b750: ldrb r0,[r0,#0x0]
  0013b752: cmp r0,#0x0
  0013b754: beq.w 0x0013bede
  0013b758: ldr r0,[0x0013b9cc]
  0013b75a: add r0,pc
  0013b75c: ldr r0,[r0,#0x0]
  0013b75e: ldrb r0,[r0,#0x0]
  0013b760: cmp r0,#0x0
  0013b762: bne.w 0x0013bede
  0013b766: ldr r5,[sp,#0x5c]
  0013b768: cbz r3,0x0013b786
  0013b76a: ldr.w r0,[r11,#0x8]
  0013b76e: cmp r0,r3
  0013b770: itt eq
  0013b772: mov.eq r0,#0x0
  0013b774: str.w.eq r0,[r11,#0x8]
  0013b778: ldr.w r0,[r11,#0xc]
  0013b77c: cmp r0,r3
  0013b77e: itt eq
  0013b780: mov.eq r0,#0x0
  0013b782: str.w.eq r0,[r11,#0xc]
  0013b786: ldr.w r0,[r11,#0x14]
  0013b78a: movs r1,#0x0
  0013b78c: strh.w r1,[r11,#0x98]
  0013b790: mov r1,r4
  0013b792: mov r2,r5
  0013b794: blx 0x00074788
  0013b798: cbz r0,0x0013b7a0
  0013b79a: ldr r1,[sp,#0x54]
  0013b79c: movs r0,#0x3
  0013b79e: str r0,[r1,#0x0]
  0013b7a0: ldr.w r0,[r11,#0x18]
  0013b7a4: mov r1,r4
  0013b7a6: mov r2,r5
  0013b7a8: blx 0x00074788
  0013b7ac: cmp r0,#0x0
  0013b7ae: beq.w 0x0013bede
  0013b7b2: ldr r0,[0x0013b9d0]
  0013b7b4: vmov.f32 s0,0x3f800000
  0013b7b8: ldr r1,[0x0013b9d4]
  0013b7ba: movw r6,#0x201
  0013b7be: add r0,pc
  0013b7c0: ldr r2,[0x0013b9d8]
  0013b7c2: ldr r3,[0x0013b9dc]
  0013b7c4: add r1,pc
  0013b7c6: ldr.w r9,[r0,#0x0]
  0013b7ca: add r2,pc
  0013b7cc: ldr r5,[r1,#0x0]
  0013b7ce: movw r0,#0x247
  0013b7d2: ldr r1,[r2,#0x0]
  0013b7d4: add r3,pc
  0013b7d6: vldr.32 s2,[r9,#0x44]
  0013b7da: str r1,[sp,#0x4c]
  0013b7dc: vcmpe.f32 s2,s0
  0013b7e0: ldr r1,[r1,#0x0]
  0013b7e2: vmrs apsr,fpscr
  0013b7e6: vcmpe.f32 s2,#0
  0013b7ea: str r5,[sp,#0x44]
  0013b7ec: str r1,[sp,#0x48]
  0013b7ee: ldr.w r8,[r3,#0x0]
  0013b7f2: it ge
  0013b7f4: movw.ge r0,#0x33e
  0013b7f8: vmrs apsr,fpscr
  0013b7fc: vcmpe.f32 s2,s0
  0013b800: it ls
  0013b802: movw.ls r0,#0x19f
  0013b806: vmrs apsr,fpscr
  0013b80a: vcmpe.f32 s2,#0
  0013b80e: str.w r0,[r9,#0x54]
  0013b812: it ge
  0013b814: movw.ge r6,#0x2da
  0013b818: vmrs apsr,fpscr
  0013b81c: ldr.w r0,[r8,#0x0]
  0013b820: it ls
  0013b822: movw.ls r6,#0x16d
  0013b826: str.w r6,[r9,#0x58]
  0013b82a: ldr.w r1,[r11,#0x78]
  0013b82e: blx r5
  0013b830: str r0,[sp,#0x40]
  0013b832: add.w r3,r11,#0x4e
  0013b836: ldr.w r0,[r11,#0x78]
  0013b83a: add.w r1,r11,#0x72
  0013b83e: str r0,[sp,#0x3c]
  0013b840: add.w r0,r11,#0x6e
  0013b844: add.w r2,r11,#0x4c
  0013b848: add.w r5,r11,#0x70
  0013b84c: ldr.w r10,[r11,#0x7c]
  0013b850: add.w r12,r11,#0x6c
  0013b854: strd r12,r0,[sp,#0x20]
  0013b858: add.w lr,r11,#0x6a
  0013b85c: strd r5,r1,[sp,#0x28]
  0013b860: add.w r5,r11,#0x62
  0013b864: strd r2,r3,[sp,#0x30]
  0013b868: add.w r0,r11,#0x74
  0013b86c: add.w r2,r11,#0x66
  0013b870: add.w r12,r11,#0x68
  0013b874: add.w r3,r11,#0x64
  0013b878: add.w r1,r11,#0x60
  0013b87c: strd r10,r0,[sp,#0x0]
  0013b880: strd r1,r5,[sp,#0x8]
  0013b884: mov r1,r6
  0013b886: strd r3,r2,[sp,#0x10]
  0013b88a: strd r12,lr,[sp,#0x18]
  0013b88e: ldr r0,[sp,#0x48]
  0013b890: ldrd r3,r2,[sp,#0x3c]
  0013b894: blx 0x000764c8
  0013b898: ldr.w r0,[r9,#0x54]
  0013b89c: str r0,[sp,#0x48]
  0013b89e: ldr r0,[sp,#0x4c]
  0013b8a0: ldr.w r1,[r11,#0x1c]
  0013b8a4: ldr r5,[sp,#0x44]
  0013b8a6: ldr r0,[r0,#0x0]
  0013b8a8: str r0,[sp,#0x4c]
  0013b8aa: ldr.w r0,[r8,#0x0]
  0013b8ae: blx r5
  0013b8b0: mov r9,r0
  0013b8b2: ldr.w r0,[r8,#0x0]
  0013b8b6: ldr.w r1,[r11,#0x24]
  0013b8ba: blx r5
  0013b8bc: mov r10,r0
  0013b8be: ldr.w r0,[r8,#0x0]
  0013b8c2: ldr.w r1,[r11,#0x20]
  0013b8c6: blx r5
  0013b8c8: add.w r2,r11,#0x48
  0013b8cc: add.w r3,r11,#0x32
  0013b8d0: add.w r12,r11,#0x34
  0013b8d4: add.w r6,r11,#0x40
  0013b8d8: strd r3,r2,[sp,#0x20]
  0013b8dc: add.w r3,r11,#0x3c
  0013b8e0: add.w r2,r11,#0x2e
  0013b8e4: add.w r1,r11,#0x4a
  0013b8e8: str r1,[sp,#0x28]
  0013b8ea: add.w lr,r11,#0x3e
  0013b8ee: strd r0,r2,[sp,#0x0]
  0013b8f2: add.w r1,r11,#0x30
  0013b8f6: add.w r5,r11,#0x42
  0013b8fa: strd r1,r6,[sp,#0x8]
  0013b8fe: strd r5,r3,[sp,#0x10]
  0013b902: mov r2,r9
  0013b904: strd lr,r12,[sp,#0x18]
  0013b908: mov r3,r10
  0013b90a: ldrd r1,r0,[sp,#0x48]
  0013b90e: blx 0x000764bc
  0013b912: b 0x0013bede
  0013b914: add.w r6,r11,#0xec
  0013b918: ldr.w r0,[r11,#0xec]
  0013b91c: ldr r1,[0x0013b9e0]
  0013b91e: ldr r0,[r0,#0x4]
  0013b920: add r1,pc
  0013b922: ldr.w r9,[r1,#0x0]
  0013b926: ldr r0,[r0,#0x4]
  0013b928: blx r9
  0013b92a: ldr r1,[0x0013b9e4]
  0013b92c: add r1,pc
  0013b92e: ldr r5,[r1,#0x0]
  0013b930: str r0,[r5,#0x0]
  0013b932: ldr.w r0,[r11,#0xec]
  0013b936: ldr r0,[r0,#0x4]
  0013b938: ldr r0,[r0,#0x8]
  0013b93a: blx r9
  0013b93c: str r0,[r5,#0x4]
  0013b93e: ldr.w r0,[r11,#0xec]
  0013b942: ldr r0,[r0,#0x4]
  0013b944: ldr r0,[r0,#0xc]
  0013b946: blx r9
  0013b948: str r0,[r5,#0x8]
  0013b94a: ldr.w r0,[r11,#0xec]
  0013b94e: ldr r0,[r0,#0x4]
  0013b950: ldr r0,[r0,#0x10]
  0013b952: blx r9
  0013b954: str r0,[r5,#0x24]
  0013b956: ldr.w r0,[r11,#0xec]
  0013b95a: ldr r0,[r0,#0x4]
  0013b95c: ldr r0,[r0,#0x14]
  0013b95e: blx r9
  0013b960: str r0,[r5,#0x28]
  0013b962: mov.w r8,#0x1
  0013b966: ldr r0,[0x0013b9e8]
  0013b968: movs r5,#0x0
  0013b96a: vldr.32 s16,[pc,#0x80]
  0013b96e: mov.w r9,#0x1
  0013b972: add r0,pc
  0013b974: vldr.32 s18,[pc,#0x78]
  0013b978: ldr.w r10,[r0,#0x0]
  0013b97c: ldr r0,[0x0013b9f4]
  0013b97e: add r0,pc
  0013b980: ldr r0,[r0,#0x0]
  0013b982: str r0,[sp,#0x4c]
  0013b984: b 0x0013ba4c
  0013b986: cmp.w r9,#0x5
  0013b98a: itttt eq
  0013b98c: ldr.eq r1,[sp,#0x58]
  0013b98e: ldr.eq r1,[r1,#0x0]
  0013b990: ldrb.eq.w r1,[r1,#0x284]
  0013b994: cmp.eq r1,#0x0
  0013b996: beq 0x0013ba48
  0013b998: ldr r0,[r0,#0x4]
  0013b99a: mov r1,r4
  0013b99c: ldr r2,[sp,#0x5c]
  0013b99e: ldr.w r0,[r0,r9,lsl #0x2]
  0013b9a2: blx 0x00076678
  0013b9a6: cmp r0,#0x0
  0013b9a8: beq 0x0013ba48
  0013b9aa: cmp.w r9,#0x2
  0013b9ae: bne 0x0013b9f8
  0013b9b0: ldr.w r0,[r10,#0x0]
  0013b9b4: movs r1,#0x7e
  0013b9b6: movs r2,#0x0
  0013b9b8: movs r3,#0x0
  0013b9ba: str r5,[sp,#0x0]
  0013b9bc: blx 0x00071548
  0013b9c0: b 0x0013ba48
  0013b9f8: cmp.w r9,#0x5
  0013b9fc: bne 0x0013ba48
  0013b9fe: ldr.w r0,[r11,#0xec]
  0013ba02: ldr r0,[r0,#0x4]
  0013ba04: ldr r0,[r0,#0x14]
  0013ba06: blx 0x00072dd8
  0013ba0a: vmov s0,r0
  0013ba0e: ldr r0,[sp,#0x4c]
  0013ba10: mov.w r1,#0x200
  0013ba14: ldr.w r5,[r11,#0x104]
  0013ba18: vcmpe.f32 s0,s18
  0013ba1c: vmrs apsr,fpscr
  0013ba20: vcmpe.f32 s0,s16
  0013ba24: ldr r0,[r0,#0x0]
  0013ba26: it ls
  0013ba28: movw.ls r1,#0x1ff
  0013ba2c: vmrs apsr,fpscr
  0013ba30: it ls
  0013ba32: mov.ls.w r1,#0x1fe
  0013ba36: blx 0x00072f70
  0013ba3a: mov r1,r0
  0013ba3c: mov r0,r5
  0013ba3e: movs r5,#0x0
  0013ba40: blx 0x00074794
  0013ba44: strb.w r8,[r11,#0x170]
  0013ba48: add.w r9,r9,#0x1
  0013ba4c: ldr r0,[r6,#0x0]
  0013ba4e: ldr r1,[r0,#0x0]
  0013ba50: cmp r9,r1
  0013ba52: bcc 0x0013b986
  0013ba54: b 0x0013bede
  0013ba56: movs r0,#0x0
  0013ba58: strb.w r0,[r11,#0x170]
  0013ba5c: b 0x0013bede
  0013ba5e: ldrb.w r0,[r11,#0x17d]
  0013ba62: ldr r6,[sp,#0x50]
  0013ba64: cmp r0,#0x0
  0013ba66: ldr r5,[sp,#0x5c]
  0013ba68: itt eq
  0013ba6a: ldrb.eq.w r0,[r11,#0x17c]
  0013ba6e: cmp.eq r0,#0x0
  0013ba70: beq.w 0x0013bfb8
  0013ba74: movs r5,#0x0
  0013ba76: strb.w r5,[r11,#0x170]
  0013ba7a: strh.w r5,[r11,#0x17c]
  0013ba7e: b 0x0013bf78
  0013ba80: ldr r0,[0x0013be0c]
  0013ba82: vmov.f32 s16,0x3fc00000
  0013ba86: vmov.f32 s18,0x3f000000
  0013ba8a: movs r6,#0x0
  0013ba8c: add r0,pc
  0013ba8e: ldr.w r9,[r0,#0x0]
  0013ba92: ldr r0,[0x0013be10]
  0013ba94: add r0,pc
  0013ba96: ldr.w r10,[r0,#0x0]
  0013ba9a: ldr r0,[0x0013be14]
  0013ba9c: add r0,pc
  0013ba9e: ldr.w r8,[r0,#0x0]
  0013baa2: b 0x0013bb02
  0013baa4: ldr r0,[r0,#0x4]
  0013baa6: mov r1,r4
  0013baa8: ldr r2,[sp,#0x5c]
  0013baaa: ldr.w r0,[r0,r6,lsl #0x2]
  0013baae: blx 0x00074788
  0013bab2: cbz r0,0x0013bb00
  0013bab4: vmov.f32 s0,s16
  0013bab8: cmp r6,#0x0
  0013baba: it eq
  0013babc: vmov.eq.f32 s0,s18
  0013bac0: vstr.32 s0,[r11,#0x1a4]
  0013bac4: vstr.32 s0,[r9,#0x2c]
  0013bac8: bne.w 0x0013bdc6
  0013bacc: ldr.w r0,[r10,#0x0]
  0013bad0: blx 0x00071710
  0013bad4: subs r0,#0x1
  0013bad6: sbcs r0,r1,#0x0
  0013bada: blt.w 0x0013bdea
  0013bade: ldr.w r0,[r8,#0x0]
  0013bae2: movs r1,#0x34
  0013bae4: ldr.w r5,[r11,#0x104]
  0013bae8: blx 0x00072f70
  0013baec: mov r1,r0
  0013baee: mov r0,r5
  0013baf0: movs r2,#0x1
  0013baf2: blx 0x000746f8
  0013baf6: movs r0,#0x1
  0013baf8: strb.w r0,[r11,#0x174]
  0013bafc: strb.w r0,[r11,#0x170]
  0013bb00: adds r6,#0x1
  0013bb02: ldr.w r0,[r11,#0xb4]
  0013bb06: ldr r1,[r0,#0x0]
  0013bb08: cmp r6,r1
  0013bb0a: bcc 0x0013baa4
  0013bb0c: b 0x0013bede
  0013bb0e: ldr.w r0,[r11,#0xc4]
  0013bb12: mov r1,r4
  0013bb14: ldr r2,[sp,#0x5c]
  0013bb16: blx 0x00074788
  0013bb1a: cmp r0,#0x0
  0013bb1c: beq.w 0x0013bede
  0013bb20: ldr.w r0,[r11,#0x18c]
  0013bb24: cmp r0,#0x0
  0013bb26: beq.w 0x0013be8e
  0013bb2a: ldr.w r1,[r11,#0xbc]
  0013bb2e: ldr r1,[r1,#0x4]
  0013bb30: ldr.w r0,[r1,r0,lsl #0x2]
  0013bb34: cmp r0,#0x0
  0013bb36: beq.w 0x0013bfe8
  0013bb3a: ldr r0,[0x0013be18]
  0013bb3c: movs r1,#0x31
  0013bb3e: ldr.w r5,[r11,#0x104]
  0013bb42: add r0,pc
  0013bb44: ldr r0,[r0,#0x0]
  0013bb46: ldr r0,[r0,#0x0]
  0013bb48: blx 0x00072f70
  0013bb4c: mov r1,r0
  0013bb4e: mov r0,r5
  0013bb50: movs r2,#0x1
  0013bb52: movs r6,#0x1
  0013bb54: blx 0x000746f8
  0013bb58: strb.w r6,[r11,#0x173]
  0013bb5c: b 0x0013bde4
  0013bb5e: movs r5,#0x0
  0013bb60: b 0x0013bb78
  0013bb62: ldr r0,[r0,#0x4]
  0013bb64: mov r1,r4
  0013bb66: ldr r2,[sp,#0x5c]
  0013bb68: ldr.w r0,[r0,r5,lsl #0x2]
  0013bb6c: blx 0x00074788
  0013bb70: cmp r0,#0x0
  0013bb72: bne.w 0x0013bcea
  0013bb76: adds r5,#0x1
  0013bb78: ldr.w r0,[r11,#0xb8]
  0013bb7c: ldr r1,[r0,#0x0]
  0013bb7e: cmp r5,r1
  0013bb80: bcc 0x0013bb62
  0013bb82: b 0x0013bede
  0013bb84: mov.w r8,#0x0
  0013bb88: mov.w r9,#0x7
  0013bb8c: mov.w r10,#0x8
  0013bb90: movs r6,#0x0
  0013bb92: b 0x0013bbf8
  0013bb94: ldr r0,[r0,#0x4]
  0013bb96: mov r1,r4
  0013bb98: ldr r2,[sp,#0x5c]
  0013bb9a: ldr.w r0,[r0,r6,lsl #0x2]
  0013bb9e: blx 0x00074788
  0013bba2: cbz r0,0x0013bbf6
  0013bba4: ldr.w r0,[r11,#0xac]
  0013bba8: ldr r0,[r0,#0x4]
  0013bbaa: ldr.w r0,[r0,r6,lsl #0x2]
  0013bbae: ldrd r2,r1,[r0,#0x0]
  0013bbb2: subs r0,r2,#0x4
  0013bbb4: sbc r3,r1,#0x0
  0013bbb8: rsbs.w r5,r0,#0x5
  0013bbbc: sbcs.w r3,r8,r3
  0013bbc0: bcc 0x0013bbd2
  0013bbc2: tbb [pc,r0]
  0013bbd2: eor r0,r2,#0x19
  0013bbd6: orrs r0,r1
  0013bbd8: bne 0x0013bbf6
  0013bbda: ldr r0,[sp,#0x54]
  0013bbdc: movs r1,#0xe
  0013bbde: b 0x0013bbec
  0013bbec: str r1,[r0,#0x0]
  0013bbee: b 0x0013bbf6
  0013bbf6: adds r6,#0x1
  0013bbf8: ldr.w r0,[r11,#0xac]
  0013bbfc: ldr r1,[r0,#0x0]
  0013bbfe: cmp r6,r1
  0013bc00: bcc 0x0013bb94
  0013bc02: b 0x0013bede
  0013bc04: ldr.w r0,[r11,#0x104]
  0013bc08: mov r1,r4
  0013bc0a: ldr r2,[sp,#0x5c]
  0013bc0c: blx 0x00074770
  0013bc10: cmp r0,#0x0
  0013bc12: bne.w 0x0013afc0
  0013bc16: ldr r0,[0x0013be1c]
  0013bc18: add r0,pc
  0013bc1a: ldr r5,[r0,#0x0]
  0013bc1c: ldr r0,[r5,#0x0]
  0013bc1e: blx 0x0007258c
  0013bc22: blx 0x0006ff88
  0013bc26: vldr.64 d8,[r0,#0x10]
  0013bc2a: ldr r0,[r5,#0x0]
  0013bc2c: blx 0x0007258c
  0013bc30: blx 0x0006ff88
  0013bc34: vcmpe.f64 d8,#0
  0013bc38: vldr.64 d16,[r0]
  0013bc3c: vmrs apsr,fpscr
  0013bc40: ble 0x0013bc78
  0013bc42: vmov.f64 d8,0x3ff0000000000000
  0013bc46: vcmpe.f64 d16,d8
  0013bc4a: vmrs apsr,fpscr
  0013bc4e: ble 0x0013bc62
  0013bc50: ldr r0,[r5,#0x0]
  0013bc52: blx 0x0007258c
  0013bc56: blx 0x0006ff88
  0013bc5a: ldr r1,[0x0013be20]
  0013bc5c: movs r2,#0x0
  0013bc5e: strd r2,r1,[r0,#0x0]
  0013bc62: ldr r0,[r5,#0x0]
  0013bc64: blx 0x0007258c
  0013bc68: blx 0x0006ff88
  0013bc6c: vldr.64 d16,[r0]
  0013bc70: vsub.f64 d16,d8,d16
  0013bc74: vadd.f64 d16,d16,d8
  0013bc78: ldr r0,[0x0013be24]
  0013bc7a: vcvt.f32.f64 s0,d16
  0013bc7e: add r0,pc
  0013bc80: ldr r6,[r0,#0x0]
  0013bc82: ldr r0,[r5,#0x0]
  0013bc84: vstr.32 s0,[r6,#0x1c]
  0013bc88: blx 0x0007258c
  0013bc8c: blx 0x0006ff88
  0013bc90: vldr.64 d16,[r0,#0x10]
  0013bc94: movs r0,#0x0
  0013bc96: vcvt.f32.f64 s0,d16
  0013bc9a: vstr.32 s0,[r6,#0x20]
  0013bc9e: strb.w r0,[r11,#0x176]
  0013bca2: strb.w r0,[r11,#0x170]
  0013bca6: b.w 0x0013afc0
  0013bcaa: ldrb.w r0,[r11,#0x191]
  0013bcae: ldr r6,[sp,#0x50]
  0013bcb0: cmp r0,#0x0
  0013bcb2: beq.w 0x0013be38
  0013bcb6: ldr.w r0,[r11,#0x104]
  0013bcba: mov r1,r4
  0013bcbc: mov r2,r5
  0013bcbe: blx 0x00074770
  0013bcc2: movs r5,#0x0
  0013bcc4: cmp r0,#0x1
  0013bcc6: beq 0x0013bcda
  0013bcc8: cmp r0,#0x0
  0013bcca: bne.w 0x0013bf78
  0013bcce: ldr r0,[0x0013be28]
  0013bcd0: movs r5,#0x0
  0013bcd2: movs r1,#0x1
  0013bcd4: add r0,pc
  0013bcd6: ldr r0,[r0,#0x0]
  0013bcd8: str r1,[r0,#0x0]
  0013bcda: strb.w r5,[r11,#0x170]
  0013bcde: strb.w r5,[r11,#0x191]
  0013bce2: b 0x0013bf78
  0013bce4: strb.w r5,[r11,#0x174]
  0013bce8: b 0x0013bdc0
  0013bcea: bic r0,r5,#0x80000000
  0013bcee: str.w r5,[r11,#0x234]
  0013bcf2: cmp r0,#0x2
  0013bcf4: beq.w 0x0013bec2
  0013bcf8: ldr r1,[sp,#0x54]
  0013bcfa: cmp r0,#0x1
  0013bcfc: bne.w 0x0013bed6
  0013bd00: ldr r0,[0x0013be2c]
  0013bd02: add r0,pc
  0013bd04: ldr r0,[r0,#0x0]
  0013bd06: ldrb.w r0,[r0,#0x35]
  0013bd0a: cmp r0,#0x0
  0013bd0c: bne.w 0x0013beda
  0013bd10: blx 0x0007669c
  0013bd14: b 0x0013c062
  0013bd16: ldr.w r0,[r11,#0x104]
  0013bd1a: mov r1,r4
  0013bd1c: mov r2,r5
  0013bd1e: blx 0x00074770
  0013bd22: cmp r0,#0x0
  0013bd24: bne.w 0x0013b1b8
  0013bd28: ldr r0,[0x0013be30]
  0013bd2a: add r0,pc
  0013bd2c: ldr r5,[r0,#0x0]
  0013bd2e: ldr r0,[r5,#0x0]
  0013bd30: blx 0x0007258c
  0013bd34: blx 0x0006ff88
  0013bd38: vldr.64 d8,[r0,#0x10]
  0013bd3c: ldr r0,[r5,#0x0]
  0013bd3e: blx 0x0007258c
  0013bd42: blx 0x0006ff88
  0013bd46: vcmpe.f64 d8,#0
  0013bd4a: vldr.64 d16,[r0]
  0013bd4e: vmrs apsr,fpscr
  0013bd52: ble 0x0013bd8a
  0013bd54: vmov.f64 d8,0x3ff0000000000000
  0013bd58: vcmpe.f64 d16,d8
  0013bd5c: vmrs apsr,fpscr
  0013bd60: ble 0x0013bd74
  0013bd62: ldr r0,[r5,#0x0]
  0013bd64: blx 0x0007258c
  0013bd68: blx 0x0006ff88
  0013bd6c: ldr r1,[0x0013be20]
  0013bd6e: movs r2,#0x0
  0013bd70: strd r2,r1,[r0,#0x0]
  0013bd74: ldr r0,[r5,#0x0]
  0013bd76: blx 0x0007258c
  0013bd7a: blx 0x0006ff88
  0013bd7e: vldr.64 d16,[r0]
  0013bd82: vsub.f64 d16,d8,d16
  0013bd86: vadd.f64 d16,d16,d8
  0013bd8a: ldr r0,[0x0013be34]
  0013bd8c: vcvt.f32.f64 s0,d16
  0013bd90: add r0,pc
  0013bd92: ldr r6,[r0,#0x0]
  0013bd94: ldr r0,[r5,#0x0]
  0013bd96: vstr.32 s0,[r6,#0x1c]
  0013bd9a: blx 0x0007258c
  0013bd9e: blx 0x0006ff88
  0013bda2: vldr.64 d16,[r0,#0x10]
  0013bda6: movs r0,#0x0
  0013bda8: vcvt.f32.f64 s0,d16
  0013bdac: vstr.32 s0,[r6,#0x20]
  0013bdb0: strb.w r0,[r11,#0x176]
  0013bdb4: strb.w r0,[r11,#0x170]
  0013bdb8: b.w 0x0013b1b8
  0013bdbc: strb.w r5,[r11,#0x178]
  0013bdc0: strb.w r5,[r11,#0x170]
  0013bdc4: b 0x0013bf78
  0013bdc6: ldr.w r0,[r8,#0x0]
  0013bdca: movs r1,#0x1a
  0013bdcc: ldr.w r5,[r11,#0x104]
  0013bdd0: blx 0x00072f70
  0013bdd4: mov r1,r0
  0013bdd6: mov r0,r5
  0013bdd8: movs r2,#0x1
  0013bdda: movs r6,#0x1
  0013bddc: blx 0x000746f8
  0013bde0: strb.w r6,[r11,#0x174]
  0013bde4: strb.w r6,[r11,#0x170]
  0013bde8: b 0x0013c062
  0013bdea: ldr.w r0,[r11,#0x234]
  0013bdee: cmp r0,#0x2
  0013bdf0: beq.w 0x0013c05c
  0013bdf4: ldr r6,[sp,#0x50]
  0013bdf6: cmp r0,#0x1
  0013bdf8: bne 0x0013be02
  0013bdfa: mov r0,r11
  0013bdfc: blx 0x000766a8
  0013be00: b 0x0013bf76
  0013be02: mov r0,r11
  0013be04: blx 0x000766b4
  0013be08: b 0x0013bf76
  0013be38: ldrb.w r0,[r11,#0x177]
  0013be3c: cmp r0,#0x0
  0013be3e: beq.w 0x0013c012
  0013be42: ldr.w r0,[r11,#0x104]
  0013be46: mov r1,r4
  0013be48: mov r2,r5
  0013be4a: blx 0x00074770
  0013be4e: movs r5,#0x0
  0013be50: cmp r0,#0x1
  0013be52: beq.w 0x0013c186
  0013be56: cmp r0,#0x0
  0013be58: bne.w 0x0013bf78
  0013be5c: ldr r0,[0x0013c194]
  0013be5e: add r0,pc
  0013be60: ldr r5,[r0,#0x0]
  0013be62: ldr r0,[r5,#0x0]
  0013be64: blx 0x00071740
  0013be68: ldr r0,[r5,#0x0]
  0013be6a: blx 0x000766c0
  0013be6e: ldr r0,[0x0013c198]
  0013be70: movs r2,#0x2
  0013be72: ldr r1,[0x0013c19c]
  0013be74: add r0,pc
  0013be76: add r1,pc
  0013be78: ldr r0,[r0,#0x0]
  0013be7a: ldr r1,[r1,#0x0]
  0013be7c: ldr r0,[r0,#0x0]
  0013be7e: str r2,[r1,#0x0]
  0013be80: movs r1,#0x1
  0013be82: blx 0x00071d64
  0013be86: movs r5,#0x0
  0013be88: strb.w r5,[r11,#0x177]
  0013be8c: b 0x0013bdc0
  0013be8e: ldr r0,[0x0013c1a0]
  0013be90: movw r1,#0x1e7
  0013be94: ldr.w r5,[r11,#0x104]
  0013be98: add r0,pc
  0013be9a: ldr r0,[r0,#0x0]
  0013be9c: ldr r0,[r0,#0x0]
  0013be9e: blx 0x00072f70
  0013bea2: add r6,sp,#0xf0
  0013bea4: mov r1,r0
  0013bea6: mov r0,r6
  0013bea8: blx 0x000766cc
  0013beac: mov r0,r5
  0013beae: mov r1,r6
  0013beb0: blx 0x00074794
  0013beb4: add r0,sp,#0xf0
  0013beb6: blx 0x0006ee30
  0013beba: movs r0,#0x1
  0013bebc: strb.w r0,[r11,#0x170]
  0013bec0: b 0x0013c062
  0013bec2: ldr r0,[0x0013c1a4]
  0013bec4: ldr r1,[sp,#0x54]
  0013bec6: add r0,pc
  0013bec8: ldr r0,[r0,#0x0]
  0013beca: ldrb.w r0,[r0,#0x37]
  0013bece: cbnz r0,0x0013beda
  0013bed0: blx 0x000766d8
  0013bed4: b 0x0013c062
  0013bed6: movs r0,#0xc
  0013bed8: str r0,[r1,#0x0]
  0013beda: movs r0,#0xc
  0013bedc: str r0,[r1,#0x0]
  0013bede: mov r1,r4
  0013bee0: ldr r4,[sp,#0x54]
  0013bee2: ldr r0,[r4,#0x0]
  0013bee4: ldrd r5,r2,[sp,#0x58]
  0013bee8: subs r0,#0x1
  0013beea: cmp r0,#0x1
  0013beec: bhi 0x0013bf2c
  0013beee: ldr.w r6,[r11,#0x214]
  0013bef2: ldr.w r0,[r11,#0x194]
  0013bef6: vldr.32 s0,[pc,#0x2b0]
  0013befa: cmp r6,#0x0
  0013befc: vmov s2,r6
  0013bf00: mov r3,r6
  0013bf02: add r0,r6
  0013bf04: vcvt.f32.s32 s2,s2
  0013bf08: it mi
  0013bf0a: rsb.mi r3,r6
  0013bf0c: cmp r3,#0x3
  0013bf0e: ldr r3,[0x0013c1ac]
  0013bf10: it gt
  0013bf12: vmov.gt.f32 s0,s2
  0013bf16: str.w r3,[r11,#0x218]
  0013bf1a: movs r3,#0x0
  0013bf1c: strb.w r3,[r11,#0x224]
  0013bf20: str.w r0,[r11,#0x194]
  0013bf24: str.w r0,[r11,#0x210]
  0013bf28: vstr.32 s0,[r11,#0x21c]
  0013bf2c: ldr r0,[r5,#0x0]
  0013bf2e: blx 0x0007477c
  0013bf32: ldr r6,[sp,#0x50]
  0013bf34: cbz r0,0x0013bf76
  0013bf36: ldr r1,[r4,#0x0]
  0013bf38: cmp r1,#0xe
  0013bf3a: bhi 0x0013bf72
  0013bf3c: movs r0,#0x11
  0013bf3e: movs r5,#0x1
  0013bf40: tbb [pc,r1]
  0013bf54: ldr r0,[0x0013c1b0]
  0013bf56: add r0,pc
  0013bf58: ldr r0,[r0,#0x0]
  0013bf5a: ldrb r1,[r0,#0x0]
  0013bf5c: movs r0,#0x0
  0013bf5e: cmp r1,#0x0
  0013bf60: it eq
  0013bf62: mov.eq r0,#0x3
  0013bf64: b 0x0013bf74
  0013bf66: ldr r0,[0x0013c1b4]
  0013bf68: add r0,pc
  0013bf6a: ldr r0,[r0,#0x0]
  0013bf6c: ldr r0,[r0,#0x0]
  0013bf6e: blx 0x00071788
  0013bf72: movs r0,#0x0
  0013bf74: str r0,[r4,#0x0]
  0013bf76: movs r5,#0x0
  0013bf78: ldr r0,[sp,#0x114]
  0013bf7a: ldr r1,[r6,#0x0]
  0013bf7c: subs r0,r1,r0
  0013bf7e: itttt eq
  0013bf80: mov.eq r0,r5
  0013bf82: add.eq sp,#0x118
  0013bf84: vpop.eq {d8,d9,d10,d11,d12,d13}
  0013bf88: add.eq sp,#0x4
  0013bf8a: itt eq
  0013bf8c: pop.eq.w {r8,r9,r10,r11}
  0013bf90: pop.eq {r4,r5,r6,r7,pc}
  0013bf92: blx 0x0006e824
  0013bf96: ldr r0,[0x0013c1b8]
  0013bf98: movs r5,#0x0
  0013bf9a: movs r1,#0x7b
  0013bf9c: movs r2,#0x0
  0013bf9e: add r0,pc
  0013bfa0: movs r3,#0x0
  0013bfa2: str r5,[sp,#0x0]
  0013bfa4: ldr r0,[r0,#0x0]
  0013bfa6: ldr r0,[r0,#0x0]
  0013bfa8: blx 0x00071548
  0013bfac: ldr.w r1,[r11,#0x18c]
  0013bfb0: mov r0,r11
  0013bfb2: blx 0x0007666c
  0013bfb6: b 0x0013c064
  0013bfb8: ldrb.w r0,[r11,#0x170]
  0013bfbc: cmp r0,#0x0
  0013bfbe: beq 0x0013c068
  0013bfc0: ldr.w r0,[r11,#0x104]
  0013bfc4: mov r1,r4
  0013bfc6: mov r2,r5
  0013bfc8: blx 0x00074770
  0013bfcc: movs r5,#0x0
  0013bfce: cmp r0,#0x0
  0013bfd0: bne 0x0013bf78
  0013bfd2: ldr r0,[0x0013c1bc]
  0013bfd4: strb.w r5,[r11,#0x170]
  0013bfd8: add r0,pc
  0013bfda: ldr r0,[r0,#0x0]
  0013bfdc: ldr r0,[r0,#0x0]
  0013bfde: blx 0x00071704
  0013bfe2: strb.w r5,[r0,#0x40]
  0013bfe6: b 0x0013bf78
  0013bfe8: ldr r0,[0x0013c1c0]
  0013bfea: movs r5,#0x0
  0013bfec: movs r1,#0x7b
  0013bfee: movs r2,#0x0
  0013bff0: add r0,pc
  0013bff2: movs r3,#0x0
  0013bff4: str r5,[sp,#0x0]
  0013bff6: ldr r0,[r0,#0x0]
  0013bff8: ldr r0,[r0,#0x0]
  0013bffa: blx 0x00071548
  0013bffe: ldr.w r1,[r11,#0x18c]
  0013c002: mov r0,r11
  0013c004: blx 0x00076654
  0013c008: b 0x0013c064
  0013c00a: movs r5,#0x0
  0013c00c: cmp r0,#0x0
  0013c00e: bne 0x0013bf78
  0013c010: b 0x0013bdc0
  0013c012: ldrb.w r0,[r11,#0x179]
  0013c016: cbz r0,0x0013c042
  0013c018: ldr.w r0,[r11,#0x104]
  0013c01c: mov r1,r4
  0013c01e: mov r2,r5
  0013c020: blx 0x00074770
  0013c024: cmp r0,#0x1
  0013c026: it ne
  0013c028: cmp.ne r0,#0x0
  0013c02a: bne 0x0013c038
  0013c02c: ldr r0,[0x0013c1c4]
  0013c02e: add r0,pc
  0013c030: ldr r0,[r0,#0x0]
  0013c032: ldr r0,[r0,#0x0]
  0013c034: blx 0x0007528c
  0013c038: movs r0,#0x0
  0013c03a: strb.w r0,[r11,#0x179]
  0013c03e: strb.w r0,[r11,#0x170]
  0013c042: ldrb.w r0,[r11,#0x17a]
  0013c046: cmp r0,#0x0
  0013c048: beq.w 0x0013c1d0
  0013c04c: ldr.w r0,[r11,#0x104]
  0013c050: mov r1,r4
  0013c052: mov r2,r5
  0013c054: blx 0x00074770
  0013c058: b.w 0x0013ab9a
  0013c05c: mov r0,r11
  0013c05e: blx 0x00076648
  0013c062: movs r5,#0x0
  0013c064: ldr r6,[sp,#0x50]
  0013c066: b 0x0013bf78
  0013c068: ldr.w r0,[r11,#0xf4]
  0013c06c: mov r1,r4
  0013c06e: mov r2,r5
  0013c070: blx 0x00075bc8
  0013c074: ldr r0,[0x0013c1c8]
  0013c076: mov.w r10,#0x0
  0013c07a: mov.w r8,#0x1
  0013c07e: mov.w r9,#0x0
  0013c082: add r0,pc
  0013c084: ldr r6,[r0,#0x0]
  0013c086: ldr r0,[0x0013c1cc]
  0013c088: add r0,pc
  0013c08a: ldr r0,[r0,#0x0]
  0013c08c: str r0,[sp,#0x4c]
  0013c08e: b 0x0013c14c
  0013c090: ldr r0,[r0,#0x4]
  0013c092: ldr.w r0,[r0,r9,lsl #0x2]
  0013c096: ldrd r2,r1,[r0,#0x0]
  0013c09a: eor r3,r2,#0x3c
  0013c09e: orrs r3,r1
  0013c0a0: beq 0x0013c0d6
  0013c0a2: eor r2,r2,#0x34
  0013c0a6: orrs r1,r2
  0013c0a8: bne 0x0013c148
  0013c0aa: mov r1,r4
  0013c0ac: mov r2,r5
  0013c0ae: blx 0x00074788
  0013c0b2: cmp r0,#0x0
  0013c0b4: beq 0x0013c148
  0013c0b6: ldr.w r0,[r11,#0x1e0]
  0013c0ba: cmp r0,#0x4
  0013c0bc: bhi 0x0013c136
  0013c0be: tbb [pc,r0]
  0013c0c8: ldr r0,[sp,#0x4c]
  0013c0ca: ldrb.w r0,[r0,#0x35]
  0013c0ce: cbnz r0,0x0013c136
  0013c0d0: blx 0x0007669c
  0013c0d4: b 0x0013c136
  0013c0d6: mov r1,r4
  0013c0d8: mov r2,r5
  0013c0da: blx 0x00074788
  0013c0de: cbz r0,0x0013c148
  0013c0e0: ldr r0,[r6,#0x0]
  0013c0e2: blx 0x00071704
  0013c0e6: mov.w r1,#0xffffffff
  0013c0ea: str r1,[r0,#0x48]
  0013c0ec: ldr r0,[r6,#0x0]
  0013c0ee: blx 0x00071704
  0013c0f2: strb.w r8,[r0,#0x3f]
  0013c0f6: strb.w r10,[r11,#0x170]
  0013c0fa: strb.w r8,[r11,#0x190]
  0013c0fe: b 0x0013c148
  0013c100: ldr r0,[sp,#0x4c]
  0013c102: ldrb.w r0,[r0,#0x36]
  0013c106: cbnz r0,0x0013c136
  0013c108: blx 0x000766e4
  0013c10c: b 0x0013c136
  0013c10e: ldr r0,[sp,#0x4c]
  0013c110: ldrb.w r0,[r0,#0x37]
  0013c114: cbnz r0,0x0013c136
  0013c116: blx 0x000766d8
  0013c11a: b 0x0013c136
  0013c11c: ldr r0,[sp,#0x4c]
  0013c11e: ldrb.w r0,[r0,#0x38]
  0013c122: cbnz r0,0x0013c136
  0013c124: blx 0x000766f0
  0013c128: b 0x0013c136
  0013c12a: ldr r0,[sp,#0x4c]
  0013c12c: ldrb.w r0,[r0,#0x39]
  0013c130: cbnz r0,0x0013c136
  0013c132: blx 0x000766fc
  0013c136: ldr r0,[r6,#0x0]
  0013c138: ldr.w r5,[r11,#0x1e0]
  0013c13c: blx 0x00071704
  0013c140: str r5,[r0,#0x48]
  0013c142: strb.w r10,[r11,#0x170]
  0013c146: ldr r5,[sp,#0x5c]
  0013c148: add.w r9,r9,#0x1
  0013c14c: ldr.w r0,[r11,#0xc0]
  0013c150: ldr r1,[r0,#0x0]
  0013c152: cmp r9,r1
  0013c154: bcc.w 0x0013c090
  0013c158: ldr.w r5,[r11,#0x1e0]
  0013c15c: movs r6,#0x0
  0013c15e: b 0x0013c174
  0013c160: ldr r0,[r0,#0x4]
  0013c162: mov r1,r4
  0013c164: ldr r2,[sp,#0x5c]
  0013c166: ldr.w r0,[r0,r6,lsl #0x2]
  0013c16a: blx 0x00074788
  0013c16e: cmp r0,#0x0
  0013c170: bne 0x0013c1f6
  0013c172: adds r6,#0x1
  0013c174: ldr.w r0,[r11,#0xf8]
  0013c178: ldr r1,[r0,#0x0]
  0013c17a: cmp r6,r1
  0013c17c: bcc 0x0013c160
  0013c17e: b 0x0013c258
  0013c180: strb.w r5,[r11,#0x173]
  0013c184: b 0x0013bdc0
  0013c186: strb.w r5,[r11,#0x177]
  0013c18a: strb.w r5,[r11,#0x170]
  0013c18e: strb.w r5,[r11,#0x174]
  0013c192: b 0x0013bf78
  0013c1d0: ldrb.w r0,[r11,#0x17d]
  0013c1d4: cmp r0,#0x0
  0013c1d6: bne.w 0x0013ba74
  0013c1da: ldrb.w r0,[r11,#0x17c]
  0013c1de: cmp r0,#0x0
  0013c1e0: bne.w 0x0013ba74
  0013c1e4: ldrb.w r0,[r11,#0x17e]
  0013c1e8: cmp r0,#0x0
  0013c1ea: beq 0x0013c2c4
  0013c1ec: movs r5,#0x0
  0013c1ee: strb.w r5,[r11,#0x17e]
  0013c1f2: b.w 0x0013b71e
  0013c1f6: cmp r5,r6
  0013c1f8: str.w r6,[r11,#0x1e0]
  0013c1fc: bne 0x0013c216
  0013c1fe: bic r0,r5,#0x80000000
  0013c202: cmp r0,#0x4
  0013c204: bhi 0x0013c258
  0013c206: tbb [pc,r0]
  0013c210: blx 0x0007669c
  0013c214: b 0x0013c258
  0013c216: ldr.w r5,[r11,#0xf4]
  0013c21a: add r0,sp,#0x108
  0013c21c: ldr r1,[0x0013c4d4]
  0013c21e: movs r2,#0x0
  0013c220: add r1,pc
  0013c222: blx 0x0006ee18
  0013c226: ldr r0,[0x0013c4d8]
  0013c228: ldr.w r1,[r11,#0x1e0]
  0013c22c: add r0,pc
  0013c22e: ldr r0,[r0,#0x0]
  0013c230: ldr r0,[r0,#0x0]
  0013c232: adds r1,#0x57
  0013c234: blx 0x00072f70
  0013c238: mov r1,r0
  0013c23a: add r0,sp,#0xfc
  0013c23c: movs r2,#0x0
  0013c23e: blx 0x0006f028
  0013c242: add r1,sp,#0x108
  0013c244: add r2,sp,#0xfc
  0013c246: mov r0,r5
  0013c248: blx 0x00075b2c
  0013c24c: add r0,sp,#0xfc
  0013c24e: blx 0x0006ee30
  0013c252: add r0,sp,#0x108
  0013c254: blx 0x0006ee30
  0013c258: ldrb.w r0,[r11,#0x1d9]
  0013c25c: cmp r0,#0x0
  0013c25e: beq.w 0x0013bede
  0013c262: ldr r0,[0x0013c4dc]
  0013c264: ldr r1,[0x0013c4e0]
  0013c266: add r0,pc
  0013c268: ldr r2,[sp,#0x58]
  0013c26a: add r1,pc
  0013c26c: ldr.w r3,[r11,#0x138]
  0013c270: ldr.w r6,[r11,#0x1e0]
  0013c274: ldr r0,[r0,#0x0]
  0013c276: ldr.w r8,[r1,#0x0]
  0013c27a: ldr r2,[r2,#0x0]
  0013c27c: ldr.w r1,[r3,r6,lsl #0x2]
  0013c280: ldr r5,[r0,#0x0]
  0013c282: ldr.w r0,[r8,#0x0]
  0013c286: ldr r2,[r2,#0x28]
  0013c288: subs r5,r5,r2
  0013c28a: blx 0x00072d84
  0013c28e: subs r0,r5,r0
  0013c290: cmp r0,r4
  0013c292: bge.w 0x0013bede
  0013c296: ldr.w r0,[r11,#0x138]
  0013c29a: ldr.w r1,[r11,#0x1e0]
  0013c29e: ldr.w r1,[r0,r1,lsl #0x2]
  0013c2a2: ldr.w r0,[r8,#0x0]
  0013c2a6: blx 0x00072d90
  0013c2aa: b 0x0013bede
  0013c2ac: blx 0x000766d8
  0013c2b0: b 0x0013c258
  0013c2b2: blx 0x000766fc
  0013c2b6: b 0x0013c258
  0013c2b8: blx 0x000766e4
  0013c2bc: b 0x0013c258
  0013c2be: blx 0x000766f0
  0013c2c2: b 0x0013c258
  0013c2c4: ldrb.w r0,[r11,#0x17f]
  0013c2c8: cbz r0,0x0013c2dc
  0013c2ca: ldr.w r0,[r11,#0x104]
  0013c2ce: mov r1,r4
  0013c2d0: ldr r2,[sp,#0x5c]
  0013c2d2: blx 0x00074770
  0013c2d6: cmp r0,#0x0
  0013c2d8: beq.w 0x0013c924
  0013c2dc: ldrb.w r0,[r11,#0x180]
  0013c2e0: cbz r0,0x0013c2f4
  0013c2e2: ldr.w r0,[r11,#0x104]
  0013c2e6: mov r1,r4
  0013c2e8: ldr r2,[sp,#0x5c]
  0013c2ea: blx 0x00074770
  0013c2ee: cmp r0,#0x0
  0013c2f0: beq.w 0x0013c934
  0013c2f4: ldrb.w r0,[r11,#0x181]
  0013c2f8: cbz r0,0x0013c30c
  0013c2fa: ldr.w r0,[r11,#0x104]
  0013c2fe: mov r1,r4
  0013c300: ldr r2,[sp,#0x5c]
  0013c302: blx 0x00074770
  0013c306: cmp r0,#0x0
  0013c308: beq.w 0x0013c93e
  0013c30c: ldr r0,[0x0013c4e4]
  0013c30e: mov.w r9,#0x0
  0013c312: ldr r5,[sp,#0x5c]
  0013c314: movs r6,#0x0
  0013c316: add r0,pc
  0013c318: ldr.w r8,[sp,#0x54]
  0013c31c: ldr.w r10,[r0,#0x0]
  0013c320: ldr.w r0,[r11,#0x4]
  0013c324: ldr r1,[r0,#0x0]
  0013c326: cmp r6,r1
  0013c328: bcs.w 0x0013c750
  0013c32c: ldr r0,[r0,#0x4]
  0013c32e: mov r1,r4
  0013c330: mov r2,r5
  0013c332: ldr.w r0,[r0,r6,lsl #0x2]
  0013c336: blx 0x00074788
  0013c33a: cmp r0,#0x0
  0013c33c: beq.w 0x0013c5fe
  0013c340: ldr.w r0,[r11,#0x4]
  0013c344: ldr r0,[r0,#0x4]
  0013c346: ldr.w r0,[r0,r6,lsl #0x2]
  0013c34a: ldrd r0,r1,[r0,#0x0]
  0013c34e: rsbs.w r2,r0,#0x19
  0013c352: sbcs.w r2,r9,r1
  0013c356: bcc 0x0013c394
  0013c358: tbh [pc,r0]
  0013c394: eor r0,r0,#0x6b
  0013c398: orrs r0,r1
  0013c39a: bne.w 0x0013c5fe
  0013c39e: movs r0,#0x12
  0013c3a0: b 0x0013c5fa
  0013c5fa: str.w r0,[r8,#0x0]
  0013c5fe: adds r6,#0x1
  0013c600: b 0x0013c320
  0013c750: ldr r0,[0x0013ca64]
  0013c752: mov.w r8,#0x1
  0013c756: movs r6,#0x0
  0013c758: add r0,pc
  0013c75a: ldr.w r9,[r0,#0x0]
  0013c75e: ldr r0,[0x0013ca68]
  0013c760: add r0,pc
  0013c762: str r0,[sp,#0x3c]
  0013c764: ldr r0,[0x0013ca6c]
  0013c766: add r0,pc
  0013c768: ldr r0,[r0,#0x0]
  0013c76a: str r0,[sp,#0x48]
  0013c76c: ldr r0,[0x0013ca70]
  0013c76e: add r0,pc
  0013c770: ldr r0,[r0,#0x0]
  0013c772: str r0,[sp,#0x4c]
  0013c774: b 0x0013c8e6
  0013c776: ldr r0,[r0,#0x4]
  0013c778: ldr.w r0,[r0,r6,lsl #0x2]
  0013c77c: ldr r5,[r0,#0x0]
  0013c77e: cmp r5,#0x18
  0013c780: beq 0x0013c7a4
  0013c782: cmp r5,#0x17
  0013c784: bne 0x0013c7c2
  0013c786: ldr r2,[sp,#0x5c]
  0013c788: mov r1,r4
  0013c78a: blx 0x00074788
  0013c78e: cmp r0,#0x0
  0013c790: beq.w 0x0013c8dc
  0013c794: ldr.w r0,[r9,#0x0]
  0013c798: cmp r0,#0x0
  0013c79a: beq 0x0013c850
  0013c79c: ldr r0,[0x0013ca78]
  0013c79e: add r0,pc
  0013c7a0: ldr r0,[r0,#0x0]
  0013c7a2: b 0x0013c7bc
  0013c7a4: ldr r2,[sp,#0x5c]
  0013c7a6: mov r1,r4
  0013c7a8: blx 0x00074788
  0013c7ac: cmp r0,#0x0
  0013c7ae: beq.w 0x0013c8dc
  0013c7b2: ldr.w r0,[r9,#0x0]
  0013c7b6: cmp r0,#0x0
  0013c7b8: beq 0x0013c8a0
  0013c7ba: ldr r0,[sp,#0x48]
  0013c7bc: str.w r8,[r0,#0x0]
  0013c7c0: b 0x0013c8dc
  0013c7c2: ldr r0,[sp,#0x4c]
  0013c7c4: ldr r0,[r0,#0x0]
  0013c7c6: blx 0x0007528c
  0013c7ca: cmp r5,#0x35
  0013c7cc: beq 0x0013c7ec
  0013c7ce: ldr r2,[sp,#0x5c]
  0013c7d0: cmp r5,#0x11
  0013c7d2: bne.w 0x0013c8e4
  0013c7d6: ldr.w r0,[r11,#0xc0]
  0013c7da: mov r1,r4
  0013c7dc: ldr r0,[r0,#0x4]
  0013c7de: ldr.w r0,[r0,r6,lsl #0x2]
  0013c7e2: blx 0x00074788
  0013c7e6: cmp r0,#0x0
  0013c7e8: beq 0x0013c8e4
  0013c7ea: b 0x0013c902
  0013c7ec: ldr.w r0,[r11,#0xc0]
  0013c7f0: mov r1,r4
  0013c7f2: ldr r2,[sp,#0x5c]
  0013c7f4: ldr r0,[r0,#0x4]
  0013c7f6: ldr.w r0,[r0,r6,lsl #0x2]
  0013c7fa: blx 0x00074788
  0013c7fe: cmp r0,#0x0
  0013c800: beq 0x0013c8e4
  0013c802: ldr r0,[0x0013ca7c]
  0013c804: add r0,pc
  0013c806: ldr r5,[r0,#0x0]
  0013c808: ldr r0,[r5,#0x0]
  0013c80a: blx 0x00071704
  0013c80e: movs r1,#0x0
  0013c810: strb.w r1,[r0,#0x4c]
  0013c814: ldr r0,[r5,#0x0]
  0013c816: blx 0x00071704
  0013c81a: strb.w r8,[r0,#0x3d]
  0013c81e: movs r1,#0x47
  0013c820: ldr.w r0,[r11,#0x104]
  0013c824: strb.w r8,[r11,#0x170]
  0013c828: strb.w r8,[r11,#0x17a]
  0013c82c: str r0,[sp,#0x40]
  0013c82e: ldr.w r0,[r10,#0x0]
  0013c832: blx 0x00072f70
  0013c836: mov r5,r0
  0013c838: ldr.w r0,[r10,#0x0]
  0013c83c: movw r1,#0x1a9
  0013c840: blx 0x00072f70
  0013c844: mov r2,r0
  0013c846: ldr r0,[sp,#0x40]
  0013c848: mov r1,r5
  0013c84a: blx 0x00076708
  0013c84e: b 0x0013c8e4
  0013c850: ldr.w r5,[r11,#0x104]
  0013c854: add.w r8,sp,#0xf0
  0013c858: ldr r1,[0x0013ca74]
  0013c85a: movs r2,#0x0
  0013c85c: mov r0,r8
  0013c85e: add r1,pc
  0013c860: blx 0x0006ee18
  0013c864: ldr.w r0,[r10,#0x0]
  0013c868: movw r1,#0xd46
  0013c86c: blx 0x00072f70
  0013c870: mov r2,r0
  0013c872: mov r0,r5
  0013c874: mov r1,r8
  0013c876: movs r3,#0x1
  0013c878: blx 0x00074ea8
  0013c87c: b 0x0013c8ca
  0013c8a0: add.w r8,sp,#0xf0
  0013c8a4: ldr r1,[sp,#0x3c]
  0013c8a6: movs r2,#0x0
  0013c8a8: ldr.w r5,[r11,#0x104]
  0013c8ac: mov r0,r8
  0013c8ae: blx 0x0006ee18
  0013c8b2: ldr.w r0,[r10,#0x0]
  0013c8b6: movw r1,#0xd46
  0013c8ba: blx 0x00072f70
  0013c8be: mov r2,r0
  0013c8c0: mov r0,r5
  0013c8c2: mov r1,r8
  0013c8c4: movs r3,#0x1
  0013c8c6: blx 0x00074ea8
  0013c8ca: mov r0,r8
  0013c8cc: blx 0x0006ee30
  0013c8d0: mov.w r8,#0x1
  0013c8d4: strb.w r8,[r11,#0x191]
  0013c8d8: strb.w r8,[r11,#0x170]
  0013c8dc: ldr r0,[sp,#0x4c]
  0013c8de: ldr r0,[r0,#0x0]
  0013c8e0: blx 0x0007528c
  0013c8e4: adds r6,#0x1
  0013c8e6: ldr.w r0,[r11,#0xc0]
  0013c8ea: ldr r1,[r0,#0x0]
  0013c8ec: cmp r6,r1
  0013c8ee: bcc.w 0x0013c776
  0013c8f2: b.w 0x0013bede
  0013c902: ldr.w r0,[r10,#0x0]
  0013c906: movs r1,#0x35
  0013c908: ldr.w r5,[r11,#0x104]
  0013c90c: blx 0x00072f70
  0013c910: mov r1,r0
  0013c912: mov r0,r5
  0013c914: movs r2,#0x1
  0013c916: movs r6,#0x1
  0013c918: blx 0x000746f8
  0013c91c: strb.w r6,[r11,#0x178]
  0013c920: b.w 0x0013bde4
  0013c924: movs r5,#0x0
  0013c926: ldr r6,[sp,#0x50]
  0013c928: strb.w r5,[r11,#0x170]
  0013c92c: strb.w r5,[r11,#0x17f]
  0013c930: b.w 0x0013bf78
  0013c934: movs r5,#0x0
  0013c936: strb.w r5,[r11,#0x180]
  0013c93a: b.w 0x0013b71e
  0013c93e: movs r5,#0x0
  0013c940: strb.w r5,[r11,#0x181]
  0013c944: b.w 0x0013b71e
```
