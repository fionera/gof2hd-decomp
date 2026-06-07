# SpaceLounge::startChat
elf 0x16aa7c body 10882
Sig: undefined __thiscall startChat(SpaceLounge * this)

## decompile
```c

/* SpaceLounge::startChat() */

void __thiscall SpaceLounge::startChat(SpaceLounge *this)

{
  bool bVar1;
  byte bVar2;
  float fVar3;
  int *piVar4;
  int iVar5;
  undefined4 uVar6;
  void *pvVar7;
  ScrollTouchWindow *pSVar8;
  undefined4 *puVar9;
  String *pSVar10;
  Generator *this_00;
  Ship *pSVar11;
  SpaceLounge *pSVar12;
  Station *this_01;
  Array *pAVar13;
  String *pSVar14;
  undefined1 *puVar15;
  Vector *pVVar16;
  Standing *pSVar17;
  Mission *pMVar18;
  int iVar19;
  int extraout_r1;
  int extraout_r1_00;
  int extraout_r1_01;
  int extraout_r1_02;
  int iVar20;
  code *pcVar21;
  int iVar22;
  Galaxy *this_02;
  Agent *pAVar23;
  undefined4 uVar24;
  char *pcVar25;
  Agent *this_03;
  int *piVar26;
  int *piVar27;
  int *piVar28;
  bool bVar29;
  uint in_fpscr;
  uint uVar30;
  uint uVar31;
  float fVar32;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float fVar33;
  float fVar34;
  float fVar35;
  float fVar36;
  String aSStack_7e4 [12];
  String aSStack_7d8 [12];
  String aSStack_7cc [12];
  String aSStack_7c0 [12];
  String aSStack_7b4 [12];
  String aSStack_7a8 [12];
  String aSStack_79c [12];
  String aSStack_790 [12];
  String aSStack_784 [12];
  String aSStack_778 [12];
  String aSStack_76c [12];
  String aSStack_760 [12];
  String aSStack_754 [12];
  String aSStack_748 [12];
  String aSStack_73c [12];
  String aSStack_730 [12];
  String aSStack_724 [12];
  String aSStack_718 [12];
  String aSStack_70c [12];
  String aSStack_700 [12];
  String aSStack_6f4 [12];
  String aSStack_6e8 [12];
  String aSStack_6dc [12];
  String aSStack_6d0 [12];
  String aSStack_6c4 [12];
  String aSStack_6b8 [12];
  String aSStack_6ac [12];
  String aSStack_6a0 [12];
  String aSStack_694 [12];
  String aSStack_688 [12];
  String aSStack_67c [12];
  String aSStack_670 [12];
  String aSStack_664 [12];
  String aSStack_658 [12];
  String aSStack_64c [12];
  String aSStack_640 [12];
  String aSStack_634 [12];
  String aSStack_628 [12];
  String aSStack_61c [12];
  String aSStack_610 [12];
  String aSStack_604 [12];
  String aSStack_5f8 [12];
  String aSStack_5ec [12];
  String aSStack_5e0 [12];
  String aSStack_5d4 [12];
  String aSStack_5c8 [12];
  String aSStack_5bc [12];
  String aSStack_5b0 [12];
  String aSStack_5a4 [12];
  String aSStack_598 [12];
  String aSStack_58c [12];
  String aSStack_580 [12];
  String aSStack_574 [12];
  String aSStack_568 [12];
  String aSStack_55c [12];
  String aSStack_550 [12];
  String aSStack_544 [12];
  String aSStack_538 [12];
  String aSStack_52c [12];
  undefined1 auStack_520 [12];
  String aSStack_514 [12];
  undefined1 auStack_508 [12];
  String aSStack_4fc [12];
  String aSStack_4f0 [12];
  String aSStack_4e4 [12];
  String aSStack_4d8 [12];
  String aSStack_4cc [12];
  String aSStack_4c0 [12];
  String aSStack_4b4 [12];
  String aSStack_4a8 [12];
  String aSStack_49c [12];
  String aSStack_490 [12];
  String aSStack_484 [12];
  String aSStack_478 [12];
  String aSStack_46c [12];
  String aSStack_460 [12];
  String aSStack_454 [12];
  String aSStack_448 [12];
  String aSStack_43c [12];
  String aSStack_430 [12];
  String aSStack_424 [12];
  String aSStack_418 [12];
  String aSStack_40c [12];
  String aSStack_400 [12];
  String aSStack_3f4 [12];
  String aSStack_3e8 [12];
  String aSStack_3dc [12];
  String aSStack_3d0 [12];
  String aSStack_3c4 [12];
  String aSStack_3b8 [12];
  undefined1 auStack_3ac [12];
  String aSStack_3a0 [12];
  String aSStack_394 [12];
  String aSStack_388 [12];
  String aSStack_37c [12];
  String aSStack_370 [12];
  String aSStack_364 [12];
  String aSStack_358 [12];
  String aSStack_34c [12];
  String aSStack_340 [12];
  String aSStack_334 [12];
  String aSStack_328 [12];
  String aSStack_31c [12];
  String aSStack_310 [12];
  String aSStack_304 [12];
  String aSStack_2f8 [12];
  String aSStack_2ec [12];
  String aSStack_2e0 [12];
  String aSStack_2d4 [12];
  String aSStack_2c8 [12];
  String aSStack_2bc [12];
  String aSStack_2b0 [12];
  String aSStack_2a4 [12];
  String aSStack_298 [12];
  String aSStack_28c [12];
  String aSStack_280 [12];
  String aSStack_274 [12];
  String aSStack_268 [12];
  String aSStack_25c [12];
  String aSStack_250 [12];
  String aSStack_244 [12];
  String aSStack_238 [12];
  String aSStack_22c [12];
  String aSStack_220 [12];
  String aSStack_214 [12];
  String aSStack_208 [12];
  String aSStack_1fc [12];
  String aSStack_1f0 [12];
  String aSStack_1e4 [12];
  String aSStack_1d8 [12];
  String aSStack_1cc [12];
  String aSStack_1c0 [12];
  String aSStack_1b4 [12];
  String aSStack_1a8 [12];
  String aSStack_19c [12];
  String aSStack_190 [12];
  undefined1 auStack_184 [12];
  String aSStack_178 [12];
  String aSStack_16c [12];
  String aSStack_160 [12];
  String aSStack_154 [12];
  String aSStack_148 [12];
  String aSStack_13c [12];
  String aSStack_130 [12];
  undefined1 auStack_124 [12];
  String aSStack_118 [12];
  undefined1 auStack_10c [12];
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
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  int iStack_34;
  
  piVar4 = *(int **)(DAT_0017ae60 + 0x17aa96);
  iStack_34 = *piVar4;
  iVar5 = *(int *)(this + 0x20);
  this_03 = *(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + iVar5 * 4);
  if (**(char **)(DAT_0017ae64 + 0x17aa98) != '\0') {
    pcVar25 = *(char **)(DAT_0017ae68 + 0x17aabe);
    fVar32 = DAT_0017ae6c;
    if ((*pcVar25 == '\0') && (fVar32 = 20.0, **(char **)(DAT_0017ae70 + 0x17aada) == '\0')) {
      fVar32 = 10.0;
    }
    iVar19 = *(int *)(*(int *)(this + 0x40) + 4);
    piVar26 = *(int **)(DAT_0017ae74 + 0x17aaee);
    fVar33 = (float)VectorSignedToFloat((int)fVar32,(byte)(in_fpscr >> 0x16) & 3);
    iVar20 = *piVar26;
    fVar32 = (float)VectorSignedToFloat(*(undefined4 *)(iVar20 + 0x68),(byte)(in_fpscr >> 0x16) & 3)
    ;
    fVar36 = (float)VectorSignedToFloat(*(int *)(iVar20 + 0x28),(byte)(in_fpscr >> 0x16) & 3);
    fVar32 = (**(float **)(iVar19 + iVar5 * 8) - fVar33) - fVar32;
    uVar30 = in_fpscr & 0xfffffff;
    if (fVar32 < fVar36) {
      fVar33 = **(float **)(iVar19 + (iVar5 << 1 | 1U) * 4) + fVar33;
      fVar32 = (float)VectorSignedToFloat(**(int **)(DAT_0017ae78 + 0x17ab36) -
                                          *(int *)(iVar20 + 0x28),(byte)(uVar30 >> 0x16) & 3);
      if (fVar33 < fVar32) {
        fVar32 = fVar33;
      }
    }
    *(int *)(this + 0x70) = (int)fVar32;
    iVar22 = *(int *)(iVar20 + 0x6c);
    uVar6 = __aeabi_idiv(iVar22,3);
    fVar33 = (float)VectorSignedToFloat(uVar6,(byte)(uVar30 >> 0x16) & 3);
    fVar33 = *(float *)(*(int *)(iVar19 + (iVar5 << 1 | 1U) * 4) + 4) - fVar33;
    fVar34 = (float)VectorSignedToFloat(((**(int **)(DAT_0017ae7c + 0x17ab86) -
                                         *(int *)(iVar20 + 0x10)) - *(int *)(iVar20 + 0x24)) -
                                        iVar22,(byte)(uVar30 >> 0x16) & 3);
    fVar36 = (float)VectorSignedToFloat(*(int *)(iVar20 + 0x20) + *(int *)(iVar20 + 0xc),
                                        (byte)(uVar30 >> 0x16) & 3);
    in_fpscr = uVar30 & 0xfffffff;
    fVar32 = fVar34;
    if (fVar33 < fVar34) {
      fVar32 = fVar33;
    }
    fVar35 = fVar34;
    fVar3 = fVar33;
    if (fVar32 < fVar36) {
      fVar35 = fVar36;
      fVar3 = fVar36;
    }
    if (fVar33 < fVar34) {
      fVar35 = fVar3;
    }
    *(int *)(this + 0x74) = (int)fVar35;
    if (*(ScrollTouchWindow **)(this + 0x60) != (ScrollTouchWindow *)0x0) {
      pvVar7 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(this + 0x60));
      operator_delete(pvVar7);
      iVar20 = *piVar26;
    }
    *(undefined4 *)(this + 0x60) = 0;
    pSVar8 = operator_new(0x24);
    ScrollTouchWindow::ScrollTouchWindow
              (pSVar8,*(int *)(this + 0x70) + *(int *)(iVar20 + 0x4c) * 2 + *(int *)(iVar20 + 0x2d4)
               ,*(int *)(this + 0x74),*(int *)(this + 0x6c),*(int *)(iVar20 + 0x6c),false);
    *(ScrollTouchWindow **)(this + 0x60) = pSVar8;
    iVar5 = *piVar26;
    iVar22 = *(int *)(this + 0x74) + *(int *)(iVar5 + 0x6c) + *(int *)(iVar5 + 0x2c);
    *(int *)(this + 0x78) = iVar22;
    iVar20 = *(int *)(iVar5 + 0x4c);
    iVar19 = ((*(int *)(this + 0x70) + *(int *)(iVar5 + 0x68)) - iVar20) - *(int *)(this + 0x6c);
    *(int *)(this + 0x84) = iVar19;
    iVar20 = iVar20 + iVar22;
    fVar33 = (float)VectorSignedToFloat(*(undefined4 *)(iVar5 + 0x34),(byte)(in_fpscr >> 0x16) & 3);
    fVar32 = DAT_0017ae80;
    if ((*pcVar25 == '\0') && (**(char **)(DAT_0017ae84 + 0x17ac8a) == '\0')) {
      fVar32 = 11.0;
    }
    iVar22 = *(int *)(iVar5 + 0x30);
    pcVar21 = *(code **)(DAT_0017ae88 + 0x17acac);
    *(int *)(this + 0x7c) = (iVar20 - iVar22 / 2) + *(int *)(iVar5 + 0x2d8) / 2;
    *(int *)(this + 0x80) = iVar20;
    iVar5 = iVar20 + iVar22 + (int)(fVar32 + fVar33);
    (*pcVar21)(*(undefined4 *)(*(int *)(*(int *)(this + 0x5c) + 4) + 8),iVar19,iVar5);
    (*pcVar21)(*(undefined4 *)(*(int *)(*(int *)(this + 0x5c) + 4) + 0xc),
               *(undefined4 *)(this + 0x84),iVar5);
    (*pcVar21)(*(undefined4 *)(*(int *)(*(int *)(this + 0x5c) + 4) + 0x10),
               *(undefined4 *)(this + 0x84),
               *(int *)(*piVar26 + 0x30) + iVar5 + (int)(fVar32 + fVar33));
    puVar9 = *(undefined4 **)(*(int *)(*(int *)(this + 0x40) + 4) + *(int *)(this + 0x20) * 8);
    uStack_40 = *puVar9;
    uStack_3c = puVar9[1];
    uStack_38 = puVar9[2];
    AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x8c),(Vector *)&uStack_40);
    puVar9 = *(undefined4 **)
              (*(int *)(*(int *)(this + 0x40) + 4) + (*(int *)(this + 0x20) << 3 | 4U));
    uStack_40 = *puVar9;
    uStack_3c = puVar9[1];
    uStack_38 = puVar9[2];
    AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x98),(Vector *)&uStack_40);
  }
  iVar5 = Agent::getOffer(this_03);
  iVar19 = Agent::isGenericAgent(this_03);
  if (iVar19 == 0) {
    AbyssEngine::String::String((String *)&uStack_40);
    iVar5 = Agent::getEvent(this_03);
    if (iVar5 < 1) {
      iVar5 = Agent::hasAcceptedOffer(this_03);
      if (iVar5 != 0) {
        pSVar10 = (String *)GameText::getText(**(int **)(DAT_0017ae98 + 0x17ae28));
        AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
        pVVar16 = (Vector *)getSoundId(this,this_03);
        if (-1 < (int)pVVar16) {
          FModSound::play(**(int **)(DAT_0017ae9c + 0x17ae50),pVVar16,(Vector *)0x0,extraout_s0);
        }
        goto LAB_0017adce;
      }
      piVar26 = *(int **)(DAT_0017b274 + 0x17b14c);
      *(int *)(*piVar26 + 0xd0) = *(int *)(*piVar26 + 0xd0) + 1;
      iVar5 = Agent::getOffer(this_03);
      if (iVar5 != 8) {
        iVar5 = Agent::getOffer(this_03);
        if (iVar5 == 9) {
          iVar5 = *(int *)(this_03 + 0x28);
          if (iVar5 == -1) {
            iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017bf9c + 0x17bbe0));
            iVar5 = iVar5 + 0x36a;
          }
          iVar19 = DAT_0017bfa0;
          *(int *)(this_03 + 0x28) = iVar5;
          piVar28 = *(int **)(iVar19 + 0x17bbf6);
          pSVar10 = (String *)GameText::getText(*piVar28);
          AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
          AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017bfa4 + 0x17bc10),false);
          iVar5 = *piVar28;
          Agent::getIndex(this_03);
          GameText::getText(iVar5);
          AbyssEngine::operator+(aSStack_64,aSStack_70);
          AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
          AbyssEngine::String::~String(aSStack_64);
          AbyssEngine::String::~String(aSStack_70);
          iVar5 = *(int *)(this_03 + 0x2c);
          if (iVar5 == -1) {
            iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017bfa8 + 0x17bc64));
            iVar5 = iVar5 + 0x305;
          }
          iVar19 = DAT_0017bfac;
          *(int *)(this_03 + 0x2c) = iVar5;
          AbyssEngine::String::String(aSStack_70,(char *)(iVar19 + 0x17bc7e),false);
          GameText::getText(*piVar28);
          AbyssEngine::operator+(aSStack_64,aSStack_70);
          AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
          AbyssEngine::String::~String(aSStack_64);
          AbyssEngine::String::~String(aSStack_70);
          iVar19 = *piVar26;
          AbyssEngine::String::String(aSStack_5f8,(String *)&uStack_40,false);
          iVar5 = Agent::getSellItemQuantity(this_03);
          AbyssEngine::String::String(aSStack_70,iVar5);
          AbyssEngine::String::String(aSStack_604,aSStack_70,false);
          uVar6 = AbyssEngine::String::String(aSStack_610,(char *)(DAT_0017bfb0 + 0x17bcec),false);
          Status::replaceHash(aSStack_64,iVar19,aSStack_5f8,aSStack_604,uVar6);
          AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
          pcVar21 = *(code **)(DAT_0017bfb4 + 0x17bd14);
          (*pcVar21)(aSStack_64);
          (*pcVar21)(aSStack_610);
          (*pcVar21)(aSStack_604);
          (*pcVar21)(aSStack_70);
          (*pcVar21)(aSStack_5f8);
          iVar19 = *piVar26;
          AbyssEngine::String::String(aSStack_61c,(String *)&uStack_40,false);
          iVar5 = *piVar28;
          Agent::getSellItemIndex(this_03);
          pSVar10 = (String *)GameText::getText(iVar5);
          AbyssEngine::String::String(aSStack_628,pSVar10,false);
          uVar6 = AbyssEngine::String::String(aSStack_634,(char *)(DAT_0017bfb8 + 0x17bd62),false);
          Status::replaceHash(aSStack_64,iVar19,aSStack_61c,aSStack_628,uVar6);
          AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
          (*pcVar21)(aSStack_64);
          (*pcVar21)(aSStack_634);
          (*pcVar21)(aSStack_628);
          (*pcVar21)(aSStack_61c);
          iVar5 = *piVar26;
          AbyssEngine::String::String(aSStack_640,(String *)&uStack_40,false);
          uVar6 = Agent::getSellItemPrice(this_03);
          VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
          Layout::formatCredits((int)aSStack_70);
          AbyssEngine::String::String(aSStack_64c,aSStack_70,false);
          uVar6 = AbyssEngine::String::String(aSStack_658,(char *)(DAT_0017bfc0 + 0x17bdea),false);
          Status::replaceHash(aSStack_64,iVar5,aSStack_640,aSStack_64c,uVar6);
          AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
          (*pcVar21)(aSStack_64);
          (*pcVar21)(aSStack_658);
          (*pcVar21)(aSStack_64c);
          (*pcVar21)(aSStack_70);
          (*pcVar21)(aSStack_640);
          iVar5 = Agent::getSellItemQuantity(this_03);
          if (1 < iVar5) {
            AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017bfc4 + 0x17be36),false);
            GameText::getText(*piVar28);
            AbyssEngine::operator+(aSStack_64,aSStack_70);
            AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
            AbyssEngine::String::~String(aSStack_64);
            AbyssEngine::String::~String(aSStack_70);
            iVar5 = *piVar26;
            AbyssEngine::String::String(aSStack_664,(String *)&uStack_40,false);
            uVar6 = Agent::getSellItemPrice(this_03);
            uVar24 = Agent::getSellItemQuantity(this_03);
            uVar6 = __aeabi_idiv(uVar6,uVar24);
            VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
            Layout::formatCredits((int)aSStack_70);
            AbyssEngine::String::String(aSStack_670,aSStack_70,false);
            uVar6 = AbyssEngine::String::String(aSStack_67c,(char *)(DAT_0017bfc8 + 0x17bece),false)
            ;
            Status::replaceHash(aSStack_64,iVar5,aSStack_664,aSStack_670,uVar6);
            AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
            (*pcVar21)(aSStack_64);
            (*pcVar21)(aSStack_67c);
            (*pcVar21)(aSStack_670);
            (*pcVar21)(aSStack_70);
            pSVar14 = aSStack_664;
LAB_0017d366:
            (*pcVar21)(pSVar14);
          }
        }
        else {
          iVar5 = Agent::getOffer(this_03);
          if (iVar5 == 10) {
            if (*(int *)(*piVar26 + 0x114) < 3) {
              piVar28 = *(int **)(DAT_0017cb3c + 0x17ca66);
              iVar5 = **(int **)(DAT_0017cb38 + 0x17ca64);
              AbyssEngine::AERandom::nextInt(*piVar28);
              pSVar10 = (String *)GameText::getText(iVar5);
              AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
              iVar20 = **(int **)(DAT_0017cb40 + 0x17ca8c);
              iVar5 = AbyssEngine::AERandom::nextInt(*piVar28);
              iVar19 = Agent::getRace(this_03);
              pSVar12 = (SpaceLounge *)Agent::isMale(this_03);
              pVVar16 = (Vector *)
                        getSpecificSoundForRace(pSVar12,iVar5 + 0x30d,iVar19,SUB41(pSVar12,0));
              FModSound::play(iVar20,pVVar16,(Vector *)0x0,extraout_s0_02);
              *(int *)(*piVar26 + 0xd0) = *(int *)(*piVar26 + 0xd0) + -1;
              pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
              Agent::getName();
              AbyssEngine::String::String(aSStack_694,(String *)&uStack_40,false);
              ScrollTouchWindow::setText(pSVar8,aSStack_688,aSStack_694);
              AbyssEngine::String::~String(aSStack_694);
              pSVar14 = aSStack_688;
              goto LAB_0017adf8;
            }
            iVar5 = *(int *)(this_03 + 0x28);
            if (iVar5 == -1) {
              iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017d370 + 0x17d134));
              iVar5 = iVar5 + 0x36a;
            }
            iVar19 = DAT_0017d374;
            *(int *)(this_03 + 0x28) = iVar5;
            piVar28 = *(int **)(iVar19 + 0x17d14a);
            pSVar10 = (String *)GameText::getText(*piVar28);
            AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
            AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017d378 + 0x17d164),false);
            iVar5 = *piVar28;
            Agent::getIndex(this_03);
            GameText::getText(iVar5);
            AbyssEngine::operator+(aSStack_64,aSStack_70);
            AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
            AbyssEngine::String::~String(aSStack_64);
            AbyssEngine::String::~String(aSStack_70);
            iVar5 = *(int *)(this_03 + 0x2c);
            if (iVar5 == -1) {
              iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017d37c + 0x17d1b8));
              iVar5 = iVar5 + 0x305;
            }
            iVar19 = DAT_0017d380;
            *(int *)(this_03 + 0x2c) = iVar5;
            AbyssEngine::String::String(aSStack_70,(char *)(iVar19 + 0x17d1ce),false);
            GameText::getText(*piVar28);
            AbyssEngine::operator+(aSStack_64,aSStack_70);
            AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
            AbyssEngine::String::~String(aSStack_64);
            AbyssEngine::String::~String(aSStack_70);
            iVar5 = *piVar26;
            AbyssEngine::String::String(aSStack_6a0,(String *)&uStack_40,false);
            AbyssEngine::String::String(aSStack_70,1);
            AbyssEngine::String::String(aSStack_6ac,aSStack_70,false);
            uVar6 = AbyssEngine::String::String(aSStack_6b8,(char *)(DAT_0017d384 + 0x17d236),false)
            ;
            Status::replaceHash(aSStack_64,iVar5,aSStack_6a0,aSStack_6ac,uVar6);
            AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
            pcVar21 = *(code **)(DAT_0017d388 + 0x17d25e);
            (*pcVar21)(aSStack_64);
            (*pcVar21)(aSStack_6b8);
            (*pcVar21)(aSStack_6ac);
            (*pcVar21)(aSStack_70);
            (*pcVar21)(aSStack_6a0);
            iVar19 = *piVar26;
            AbyssEngine::String::String(aSStack_6c4,(String *)&uStack_40,false);
            iVar5 = *piVar28;
            Agent::getSellItemIndex(this_03);
            pSVar10 = (String *)GameText::getText(iVar5);
            AbyssEngine::String::String(aSStack_6d0,pSVar10,false);
            uVar6 = AbyssEngine::String::String(aSStack_6dc,(char *)(DAT_0017d38c + 0x17d2ac),false)
            ;
            Status::replaceHash(aSStack_64,iVar19,aSStack_6c4,aSStack_6d0,uVar6);
            AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
            (*pcVar21)(aSStack_64);
            (*pcVar21)(aSStack_6dc);
            (*pcVar21)(aSStack_6d0);
            (*pcVar21)(aSStack_6c4);
            iVar5 = *piVar26;
            AbyssEngine::String::String(aSStack_6e8,(String *)&uStack_40,false);
            uVar6 = Agent::getSellItemPrice(this_03);
            VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
            Layout::formatCredits((int)aSStack_70);
            AbyssEngine::String::String(aSStack_6f4,aSStack_70,false);
            uVar6 = AbyssEngine::String::String(aSStack_700,(char *)(DAT_0017d394 + 0x17d32e),false)
            ;
            Status::replaceHash(aSStack_64,iVar5,aSStack_6e8,aSStack_6f4,uVar6);
            AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
            (*pcVar21)(aSStack_64);
            (*pcVar21)(aSStack_700);
            (*pcVar21)(aSStack_6f4);
            (*pcVar21)(aSStack_70);
            pSVar14 = aSStack_6e8;
            goto LAB_0017d366;
          }
          iVar5 = Agent::getOffer(this_03);
          if (iVar5 == 4) {
            iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017d0dc + 0x17ce70));
            piVar28 = *(int **)(DAT_0017d0e0 + 0x17ce82);
            *(int *)(this_03 + 0x28) = iVar5 + 0x36a;
            pSVar10 = (String *)GameText::getText(*piVar28);
            AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
            AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017d0e4 + 0x17cea0),false);
            iVar5 = *piVar28;
            Agent::getIndex(this_03);
            GameText::getText(iVar5);
            AbyssEngine::operator+(aSStack_64,aSStack_70);
            AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
            AbyssEngine::String::~String(aSStack_64);
            AbyssEngine::String::~String(aSStack_70);
            AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017d0e8 + 0x17ceea),false);
            GameText::getText(*piVar28);
            AbyssEngine::operator+(aSStack_64,aSStack_70);
            AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
            AbyssEngine::String::~String(aSStack_64);
            AbyssEngine::String::~String(aSStack_70);
            this_02 = (Galaxy *)**(undefined4 **)(DAT_0017d0ec + 0x17cf2c);
            iVar5 = Agent::getSellSystemIndex(this_03);
            Galaxy::getSystem(this_02,iVar5);
            iVar5 = *piVar26;
            AbyssEngine::String::String(aSStack_70c,(String *)&uStack_40,false);
            SolarSystem::getName();
            AbyssEngine::String::String(aSStack_718,aSStack_70,false);
            uVar6 = AbyssEngine::String::String(aSStack_724,(char *)(DAT_0017d0f0 + 0x17cf6a),false)
            ;
            Status::replaceHash(aSStack_64,iVar5,aSStack_70c,aSStack_718,uVar6);
            AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
            pcVar21 = *(code **)(DAT_0017d0f4 + 0x17cf92);
            (*pcVar21)(aSStack_64);
            (*pcVar21)(aSStack_724);
            (*pcVar21)(aSStack_718);
            (*pcVar21)(aSStack_70);
            (*pcVar21)(aSStack_70c);
            iVar5 = *piVar26;
            AbyssEngine::String::String(aSStack_730,(String *)&uStack_40,false);
            Agent::getSellItemPrice(this_03);
            Layout::formatCredits((int)aSStack_70);
            AbyssEngine::String::String(aSStack_73c,aSStack_70,false);
            uVar6 = AbyssEngine::String::String(aSStack_748,(char *)(DAT_0017d0fc + 0x17cfe6),false)
            ;
            Status::replaceHash(aSStack_64,iVar5,aSStack_730,aSStack_73c,uVar6);
            AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
            (*pcVar21)(aSStack_64);
            (*pcVar21)(aSStack_748);
            (*pcVar21)(aSStack_73c);
            (*pcVar21)(aSStack_70);
            pSVar14 = aSStack_730;
          }
          else {
            iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017d0dc + 0x17ce70));
            piVar28 = *(int **)(DAT_0017d78c + 0x17d3a4);
            *(int *)(this_03 + 0x28) = iVar5 + 0x36a;
            pSVar10 = (String *)GameText::getText(*piVar28);
            AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
            AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017d790 + 0x17d3c2),false);
            iVar5 = *piVar28;
            Agent::getIndex(this_03);
            GameText::getText(iVar5);
            AbyssEngine::operator+(aSStack_64,aSStack_70);
            AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
            AbyssEngine::String::~String(aSStack_64);
            AbyssEngine::String::~String(aSStack_70);
            AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017d794 + 0x17d40c),false);
            GameText::getText(*piVar28);
            AbyssEngine::operator+(aSStack_64,aSStack_70);
            AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
            AbyssEngine::String::~String(aSStack_64);
            AbyssEngine::String::~String(aSStack_70);
            iVar22 = *piVar26;
            AbyssEngine::String::String(aSStack_754,(String *)&uStack_40,false);
            iVar19 = *piVar28;
            iVar20 = **(int **)(DAT_0017d798 + 0x17d460);
            iVar5 = Agent::getSellBlueprintIndex(this_03);
            Item::getIndex(*(Item **)(*(int *)(iVar20 + 4) + iVar5 * 4));
            pSVar10 = (String *)GameText::getText(iVar19);
            AbyssEngine::String::String(aSStack_760,pSVar10,false);
            uVar6 = AbyssEngine::String::String(aSStack_76c,(char *)(DAT_0017d79c + 0x17d48c),false)
            ;
            Status::replaceHash(aSStack_64,iVar22,aSStack_754,aSStack_760,uVar6);
            AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
            pcVar21 = *(code **)(DAT_0017d7a0 + 0x17d4b4);
            (*pcVar21)(aSStack_64);
            (*pcVar21)(aSStack_76c);
            (*pcVar21)(aSStack_760);
            (*pcVar21)(aSStack_754);
            iVar5 = *piVar26;
            AbyssEngine::String::String(aSStack_778,(String *)&uStack_40,false);
            Agent::getSellItemPrice(this_03);
            Layout::formatCredits((int)aSStack_70);
            AbyssEngine::String::String(aSStack_784,aSStack_70,false);
            uVar6 = AbyssEngine::String::String(aSStack_790,(char *)(DAT_0017d7a8 + 0x17d502),false)
            ;
            Status::replaceHash(aSStack_64,iVar5,aSStack_778,aSStack_784,uVar6);
            AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
            (*pcVar21)(aSStack_64);
            (*pcVar21)(aSStack_790);
            (*pcVar21)(aSStack_784);
            (*pcVar21)(aSStack_70);
            pSVar14 = aSStack_778;
          }
          (*pcVar21)(pSVar14);
        }
        AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017d7ac + 0x17d542),false);
        iVar5 = **(int **)(DAT_0017d7b0 + 0x17d552);
        AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017d7b4 + 0x17d554));
        GameText::getText(iVar5);
        AbyssEngine::operator+(aSStack_64,aSStack_70);
        AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
        AbyssEngine::String::~String(aSStack_64);
        AbyssEngine::String::~String(aSStack_70);
        pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
        Agent::getName();
        AbyssEngine::String::String(aSStack_7a8,(String *)&uStack_40,false);
        ScrollTouchWindow::setText(pSVar8,aSStack_79c,aSStack_7a8);
        AbyssEngine::String::~String(aSStack_7a8);
        AbyssEngine::String::~String(aSStack_79c);
        pAVar23 = *(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4);
        AbyssEngine::String::String(aSStack_7b4,(String *)&uStack_40,false);
        Agent::setMissionString(pAVar23,aSStack_7b4);
        AbyssEngine::String::~String(aSStack_7b4);
        *(undefined4 *)(this + 0x14) = 2;
        *(undefined4 *)(this + 0x2c) = 0;
        pVVar16 = (Vector *)getSoundId(this,this_03);
        if (-1 < (int)pVVar16) {
          FModSound::play(**(int **)(DAT_0017d7b8 + 0x17d60a),pVVar16,(Vector *)0x0,extraout_s0_04);
        }
        goto LAB_0017ae08;
      }
      pSVar11 = (Ship *)Status::getShip();
      iVar5 = Agent::getSellModIndex(this_03);
      iVar5 = Ship::hasModInstalled(pSVar11,iVar5);
      if (iVar5 == 0) {
        iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017ce34 + 0x17cb4a));
        *(int *)(this_03 + 0x28) = iVar5 + 0x36a;
        pSVar11 = (Ship *)Status::getShip();
        iVar5 = Ship::getPrice(pSVar11);
        iVar19 = Agent::getModPricePercentage(this_03);
        iVar5 = __aeabi_idiv(iVar5 * iVar19,100);
        Agent::setSellItemPrice(this_03,iVar5);
        piVar28 = *(int **)(DAT_0017ce38 + 0x17cb88);
        pSVar10 = (String *)GameText::getText(*piVar28);
        AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
        AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017ce3c + 0x17cb9e),false);
        iVar5 = *piVar28;
        Agent::getIndex(this_03);
        GameText::getText(iVar5);
        AbyssEngine::operator+(aSStack_64,aSStack_70);
        AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
        AbyssEngine::String::~String(aSStack_64);
        AbyssEngine::String::~String(aSStack_70);
        AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017ce40 + 0x17cbe8),false);
        GameText::getText(*piVar28);
        AbyssEngine::operator+(aSStack_64,aSStack_70);
        AbyssEngine::String::operator+=((String *)&uStack_40,aSStack_64);
        AbyssEngine::String::~String(aSStack_64);
        AbyssEngine::String::~String(aSStack_70);
        iVar19 = *piVar26;
        AbyssEngine::String::String(aSStack_58c,(String *)&uStack_40,false);
        iVar5 = *piVar28;
        Status::getShip();
        Ship::getIndex();
        pSVar10 = (String *)GameText::getText(iVar5);
        AbyssEngine::String::String(aSStack_598,pSVar10,false);
        uVar6 = AbyssEngine::String::String(aSStack_5a4,(char *)(DAT_0017ce44 + 0x17cc5c),false);
        Status::replaceHash(aSStack_64,iVar19,aSStack_58c,aSStack_598,uVar6);
        AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
        pcVar21 = *(code **)(DAT_0017ce48 + 0x17cc84);
        (*pcVar21)(aSStack_64);
        (*pcVar21)(aSStack_5a4);
        (*pcVar21)(aSStack_598);
        (*pcVar21)(aSStack_58c);
        iVar19 = *piVar26;
        AbyssEngine::String::String(aSStack_5b0,(String *)&uStack_40,false);
        iVar5 = Agent::getSellModIndex(this_03);
        AbyssEngine::String::String(aSStack_70,*(int *)(DAT_0017ce4c + iVar5 * 8 + 0x17ccb6));
        AbyssEngine::String::String(aSStack_5bc,aSStack_70,false);
        uVar6 = AbyssEngine::String::String(aSStack_5c8,(char *)(DAT_0017ce50 + 0x17ccd0),false);
        Status::replaceHash(aSStack_64,iVar19,aSStack_5b0,aSStack_5bc,uVar6);
        AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
        (*pcVar21)(aSStack_64);
        (*pcVar21)(aSStack_5c8);
        (*pcVar21)(aSStack_5bc);
        (*pcVar21)(aSStack_70);
        (*pcVar21)(aSStack_5b0);
        iVar5 = *piVar26;
        AbyssEngine::String::String(aSStack_5d4,(String *)&uStack_40,false);
        uVar6 = Agent::getSellItemPrice(this_03);
        VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
        Layout::formatCredits((int)aSStack_70);
        AbyssEngine::String::String(aSStack_5e0,aSStack_70,false);
        uVar6 = AbyssEngine::String::String(aSStack_5ec,(char *)(DAT_0017ce58 + 0x17cd58),false);
        Status::replaceHash(aSStack_64,iVar5,aSStack_5d4,aSStack_5e0,uVar6);
        AbyssEngine::String::operator=((String *)&uStack_40,aSStack_64);
        (*pcVar21)(aSStack_64);
        (*pcVar21)(aSStack_5ec);
        (*pcVar21)(aSStack_5e0);
        (*pcVar21)(aSStack_70);
        pSVar14 = aSStack_5d4;
        goto LAB_0017d366;
      }
      pSVar10 = (String *)GameText::getText(**(int **)(DAT_0017b27c + 0x17b1a6));
      AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
      iVar20 = **(int **)(DAT_0017b280 + 0x17b1c2);
      iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017b284 + 0x17b1c4));
      iVar19 = Agent::getRace(this_03);
      pSVar12 = (SpaceLounge *)Agent::isMale(this_03);
      pVVar16 = (Vector *)getSpecificSoundForRace(pSVar12,iVar5 + 0x30d,iVar19,SUB41(pSVar12,0));
      FModSound::play(iVar20,pVVar16,(Vector *)0x0,extraout_s0_00);
      *(int *)(*piVar26 + 0xd0) = *(int *)(*piVar26 + 0xd0) + -1;
      pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
      Agent::getName();
      AbyssEngine::String::String(aSStack_580,(String *)&uStack_40,false);
      ScrollTouchWindow::setText(pSVar8,aSStack_574,aSStack_580);
      AbyssEngine::String::~String(aSStack_580);
      pSVar14 = aSStack_574;
    }
    else {
      pSVar10 = (String *)GameText::getText(**(int **)(DAT_0017ae94 + 0x17adba));
      AbyssEngine::String::operator=((String *)&uStack_40,pSVar10);
LAB_0017adce:
      pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
      Agent::getName();
      AbyssEngine::String::String(aSStack_7cc,(String *)&uStack_40,false);
      ScrollTouchWindow::setText(pSVar8,aSStack_7c0,aSStack_7cc);
      AbyssEngine::String::~String(aSStack_7cc);
      pSVar14 = aSStack_7c0;
    }
LAB_0017adf8:
    AbyssEngine::String::~String(pSVar14);
    *(undefined4 *)(this + 0x14) = 3;
    *(undefined4 *)(this + 0x2c) = 0;
LAB_0017ae08:
    AbyssEngine::String::~String((String *)&uStack_40);
    goto LAB_0017c8d4;
  }
  iVar19 = Agent::isKnown(this_03);
  if ((iVar19 != 0) && (iVar19 = Agent::getOffer(this_03), iVar19 != 7)) {
    iVar5 = Agent::hasAcceptedOffer(this_03);
    if (iVar5 != 0) {
      iVar19 = **(int **)(DAT_0017b238 + 0x17aeb2);
      iVar5 = Agent::getOffer(this_03);
      if (((iVar5 != 5) && (iVar5 = Agent::getOffer(this_03), iVar5 != 6)) &&
         (iVar5 = Agent::getMission(this_03), iVar5 != 0)) {
        Agent::getMission(this_03);
        Mission::getType();
      }
      pSVar10 = (String *)GameText::getText(iVar19);
      AbyssEngine::String::String((String *)&uStack_40,pSVar10,false);
      pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
      Agent::getName();
      AbyssEngine::String::String(aSStack_58,(String *)&uStack_40,false);
      ScrollTouchWindow::setText(pSVar8,aSStack_4c,aSStack_58);
      AbyssEngine::String::~String(aSStack_58);
      AbyssEngine::String::~String(aSStack_4c);
      *(undefined4 *)(this + 0x14) = 3;
      *(undefined4 *)(this + 0x2c) = 0;
      pVVar16 = (Vector *)getSoundId(this,this_03);
      if (-1 < (int)pVVar16) {
        FModSound::play(**(int **)(DAT_0017ce5c + 0x17ce20),pVVar16,(Vector *)0x0,extraout_s0_03);
      }
      goto LAB_0017ae08;
    }
    iVar5 = Agent::getOffer(this_03);
    if ((iVar5 == 0) && (iVar5 = Agent::getMission(this_03), iVar5 != 0)) {
      Agent::getMission(this_03);
      iVar5 = Mission::getType();
      if (iVar5 == 8) goto LAB_0017b294;
      Agent::getMission(this_03);
      iVar5 = Mission::getType();
      if (iVar5 == 0xc) goto LAB_0017b294;
      puVar9 = *(undefined4 **)(DAT_0017cb20 + 0x17c926);
      pSVar17 = (Standing *)Status::getStanding();
      iVar5 = Agent::getRace(this_03);
      fVar32 = (float)Standing::getMissionBonus(pSVar17,iVar5);
      Agent::getMission(this_03);
      uVar6 = Mission::getReward();
      fVar33 = (float)VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
      iVar19 = (int)(fVar32 * fVar33);
      __aeabi_idivmod(iVar19,0x32);
      iVar5 = extraout_r1_01 + iVar19;
      __aeabi_idivmod(iVar5,0x32);
      if (extraout_r1_02 != 0) {
        iVar5 = iVar19 - extraout_r1_01;
      }
      pMVar18 = (Mission *)Agent::getMission(this_03);
      Mission::setBonus(pMVar18,iVar5);
      if (fVar32 <= 0.0) {
        AbyssEngine::String::String((String *)&uStack_40,(char *)(DAT_0017d7bc + 0x17d624),false);
      }
      else {
        AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017cb24 + 0x17c9a4),false);
        uVar24 = *puVar9;
        pSVar10 = (String *)GameText::getText(**(int **)(DAT_0017cb28 + 0x17c9b0));
        AbyssEngine::String::String(aSStack_88,pSVar10,false);
        AbyssEngine::String::String(aSStack_a0,(int)(fVar32 * DAT_0017cb2c));
        AbyssEngine::String::String(aSStack_94,aSStack_a0,false);
        uVar6 = AbyssEngine::String::String(aSStack_ac,(char *)(DAT_0017cb30 + 0x17c9f2),false);
        Status::replaceHash(aSStack_7c,uVar24,aSStack_88,aSStack_94,uVar6);
        AbyssEngine::operator+(aSStack_64,aSStack_70);
        AbyssEngine::String::String(aSStack_b8,(char *)(DAT_0017cb34 + 0x17ca24),false);
        AbyssEngine::operator+((String *)&uStack_40,aSStack_64);
      }
      AbyssEngine::String::operator=((String *)(this + 0xa4),(String *)&uStack_40);
      AbyssEngine::String::~String((String *)&uStack_40);
      if (0.0 < fVar32) {
        pcVar21 = *(code **)(DAT_0017d7c0 + 0x17d64a);
        (*pcVar21)(aSStack_b8);
        (*pcVar21)(aSStack_64);
        (*pcVar21)(aSStack_7c);
        (*pcVar21)(aSStack_ac);
        (*pcVar21)(aSStack_94);
        (*pcVar21)(aSStack_a0);
        (*pcVar21)(aSStack_88);
        (*pcVar21)(aSStack_70);
      }
      Globals::getAgentMissionText((Agent *)&uStack_40);
      uVar24 = *puVar9;
      AbyssEngine::String::String(aSStack_d0,(Agent *)&uStack_40,false);
      Agent::getMission(this_03);
      Mission::getReward();
      Agent::getMission(this_03);
      Mission::getBonus();
      Layout::formatCredits((int)aSStack_f4);
      AbyssEngine::String::String(aSStack_e8,aSStack_f4,false);
      AbyssEngine::operator+(aSStack_dc,aSStack_e8);
      uVar6 = AbyssEngine::String::String(aSStack_100,(char *)(DAT_0017d7cc + 0x17d6ec),false);
      Status::replaceHash(aSStack_c4,uVar24,aSStack_d0,aSStack_dc,uVar6);
      AbyssEngine::String::operator=((String *)&uStack_40,aSStack_c4);
      pcVar21 = *(code **)(DAT_0017d7d0 + 0x17d71e);
      (*pcVar21)(aSStack_c4);
      (*pcVar21)(aSStack_100);
      (*pcVar21)(aSStack_dc);
      (*pcVar21)(aSStack_e8);
      (*pcVar21)(aSStack_f4);
      (*pcVar21)(aSStack_d0);
      pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
      Agent::getName();
      AbyssEngine::String::String(aSStack_118,(String *)&uStack_40,false);
      ScrollTouchWindow::setText(pSVar8,auStack_10c,aSStack_118);
      (*pcVar21)(aSStack_118);
      puVar15 = auStack_10c;
    }
    else {
LAB_0017b294:
      Globals::getAgentMissionText((Agent *)&uStack_40);
      pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
      Agent::getName();
      AbyssEngine::String::String(aSStack_130,(String *)&uStack_40,false);
      ScrollTouchWindow::setText(pSVar8,auStack_124,aSStack_130);
      pcVar21 = *(code **)(DAT_0017b4cc + 0x17b2d6);
      (*pcVar21)(aSStack_130);
      puVar15 = auStack_124;
    }
    (*pcVar21)(puVar15);
    (*pcVar21)(&uStack_40);
    goto LAB_0017c85a;
  }
  piVar26 = *(int **)(DAT_0017ae8c + 0x17ad7c);
  *(int *)(*piVar26 + 0xd0) = *(int *)(*piVar26 + 0xd0) + 1;
  if (iVar5 == 1 || iVar5 == 7) {
    AbyssEngine::String::String((String *)&uStack_40,(char *)(DAT_0017ae90 + 0x17ad9e),false);
    AbyssEngine::String::String(aSStack_64,(char *)(DAT_0017b244 + 0x17af02),false);
  }
  else {
    iVar19 = **(int **)(DAT_0017b23c + 0x17aed0);
    AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017b240 + 0x17aed2));
    pSVar10 = (String *)GameText::getText(iVar19);
    AbyssEngine::String::String((String *)&uStack_40,pSVar10,false);
    iVar19 = **(int **)(DAT_0017b248 + 0x17af14);
    AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017b24c + 0x17af16));
    pSVar10 = (String *)GameText::getText(iVar19);
    AbyssEngine::String::String(aSStack_64,pSVar10,false);
  }
  if (iVar5 == 5 || iVar5 == 0) {
    iVar5 = **(int **)(DAT_0017b250 + 0x17af48);
    AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017b254 + 0x17af4a));
    pSVar10 = (String *)GameText::getText(iVar5);
    AbyssEngine::String::String(aSStack_70,pSVar10,false);
  }
  else {
    AbyssEngine::String::String(aSStack_70,(char *)(DAT_0017b258 + 0x17af74),false);
  }
  AbyssEngine::String::String(aSStack_7c,(char *)(DAT_0017b25c + 0x17af82),false);
  iVar5 = *piVar26;
  AbyssEngine::String::String(aSStack_13c,aSStack_64,false);
  Agent::getName();
  AbyssEngine::String::String(aSStack_148,aSStack_b8,false);
  uVar6 = AbyssEngine::String::String(aSStack_154,(char *)(DAT_0017b260 + 0x17afb8),false);
  Status::replaceHash(aSStack_a0,iVar5,aSStack_13c,aSStack_148,uVar6);
  AbyssEngine::String::operator=(aSStack_64,aSStack_a0);
  pcVar21 = *(code **)(DAT_0017b264 + 0x17afe8);
  (*pcVar21)(aSStack_a0);
  (*pcVar21)(aSStack_154);
  (*pcVar21)(aSStack_148);
  (*pcVar21)(aSStack_b8);
  (*pcVar21)(aSStack_13c);
  this_00 = operator_new(1);
  Generator::Generator(this_00);
  uVar6 = Agent::getOffer(this_03);
  switch(uVar6) {
  case 0:
    pMVar18 = (Mission *)Agent::getMission(this_03);
    piVar28 = *(int **)(DAT_0017b268 + 0x17b048);
    iVar5 = *piVar28;
    Mission::getType();
    pSVar10 = (String *)GameText::getText(iVar5);
    AbyssEngine::String::operator+=(aSStack_7c,pSVar10);
    iVar5 = Mission::getType();
    if ((iVar5 == 5) || (iVar5 = Mission::getType(), iVar5 == 3)) {
      AbyssEngine::String::String(aSStack_b8,(char *)(DAT_0017b26c + 0x17b082),false);
      GameText::getText(*piVar28);
      AbyssEngine::operator+(aSStack_a0,aSStack_b8);
      AbyssEngine::String::operator+=(aSStack_7c,aSStack_a0);
      AbyssEngine::String::~String(aSStack_a0);
      AbyssEngine::String::~String(aSStack_b8);
    }
    iVar5 = Mission::getType();
    if (iVar5 == 0xf) {
      iVar19 = *piVar26;
      AbyssEngine::String::String(aSStack_274,aSStack_7c,false);
      iVar5 = *piVar28;
      Mission::getProductionGoodIndex(pMVar18);
      pSVar10 = (String *)GameText::getText(iVar5);
      AbyssEngine::String::String(aSStack_280,pSVar10,false);
      uVar6 = AbyssEngine::String::String(aSStack_28c,(char *)(DAT_0017b270 + 0x17b104),false);
      Status::replaceHash(aSStack_a0,iVar19,aSStack_274,aSStack_280,uVar6);
      AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
      (*pcVar21)(aSStack_a0);
      (*pcVar21)(aSStack_28c);
      (*pcVar21)(aSStack_280);
      pSVar14 = aSStack_274;
    }
    else {
      iVar19 = *piVar26;
      AbyssEngine::String::String(aSStack_298,aSStack_7c,false);
      iVar5 = *piVar28;
      Mission::getProductionGoodIndex(pMVar18);
      pSVar10 = (String *)GameText::getText(iVar5);
      AbyssEngine::String::String(aSStack_2a4,pSVar10,false);
      uVar6 = AbyssEngine::String::String(aSStack_2b0,(char *)(DAT_0017c358 + 0x17c008),false);
      Status::replaceHash(aSStack_a0,iVar19,aSStack_298,aSStack_2a4,uVar6);
      AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
      (*pcVar21)(aSStack_a0);
      (*pcVar21)(aSStack_2b0);
      (*pcVar21)(aSStack_2a4);
      pSVar14 = aSStack_298;
    }
    (*pcVar21)(pSVar14);
    iVar19 = *piVar26;
    AbyssEngine::String::String(aSStack_2bc,aSStack_7c,false);
    iVar5 = Mission::getProductionGoodAmount();
    AbyssEngine::String::String(aSStack_b8,iVar5);
    AbyssEngine::String::String(aSStack_2c8,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_2d4,(char *)(DAT_0017c35c + 0x17c07e),false);
    Status::replaceHash(aSStack_a0,iVar19,aSStack_2bc,aSStack_2c8,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_2d4);
    (*pcVar21)(aSStack_2c8);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_2bc);
    iVar5 = Mission::getType();
    if (iVar5 == 0xe) {
      iVar5 = *piVar26;
      AbyssEngine::String::String(aSStack_2e0,aSStack_7c,false);
      Mission::getTargetSystemName();
      AbyssEngine::String::String(aSStack_2ec,aSStack_b8,false);
      uVar6 = AbyssEngine::String::String(aSStack_2f8,(char *)(DAT_0017c360 + 0x17c100),false);
      Status::replaceHash(aSStack_a0,iVar5,aSStack_2e0,aSStack_2ec,uVar6);
      AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
      (*pcVar21)(aSStack_a0);
      (*pcVar21)(aSStack_2f8);
      (*pcVar21)(aSStack_2ec);
      (*pcVar21)(aSStack_b8);
      pSVar14 = aSStack_2e0;
    }
    else {
      iVar5 = *piVar26;
      AbyssEngine::String::String(aSStack_304,aSStack_7c,false);
      Mission::getTargetStationName();
      AbyssEngine::String::String(aSStack_310,aSStack_b8,false);
      uVar6 = AbyssEngine::String::String(aSStack_31c,(char *)(DAT_0017c364 + 0x17c176),false);
      Status::replaceHash(aSStack_a0,iVar5,aSStack_304,aSStack_310,uVar6);
      AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
      (*pcVar21)(aSStack_a0);
      (*pcVar21)(aSStack_31c);
      (*pcVar21)(aSStack_310);
      (*pcVar21)(aSStack_b8);
      pSVar14 = aSStack_304;
    }
    (*pcVar21)(pSVar14);
    iVar5 = *piVar26;
    AbyssEngine::String::String(aSStack_328,aSStack_7c,false);
    Mission::getTargetName();
    AbyssEngine::String::String(aSStack_334,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_340,(char *)(DAT_0017c368 + 0x17c1ec),false);
    Status::replaceHash(aSStack_a0,iVar5,aSStack_328,aSStack_334,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_340);
    (*pcVar21)(aSStack_334);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_328);
    iVar19 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017c36c + 0x17c238));
    iVar5 = DAT_0017c370;
    *(int *)(this_03 + 0x28) = iVar19 + 0x2fc;
    AbyssEngine::String::String(aSStack_b8,(char *)(iVar5 + 0x17c24e),false);
    GameText::getText(*piVar28);
    AbyssEngine::operator+(aSStack_a0,aSStack_b8);
    AbyssEngine::String::operator+=(aSStack_7c,aSStack_a0);
    AbyssEngine::String::~String(aSStack_a0);
    AbyssEngine::String::~String(aSStack_b8);
    pSVar17 = (Standing *)Status::getStanding();
    iVar5 = Agent::getRace(this_03);
    fVar32 = (float)Standing::getMissionBonus(pSVar17,iVar5);
    uVar30 = in_fpscr & 0xfffffff | (uint)(fVar32 < 0.0) << 0x1f | (uint)(fVar32 == 0.0) << 0x1e;
    uVar31 = uVar30 | (uint)NAN(fVar32) << 0x1c;
    bVar2 = (byte)(uVar30 >> 0x18);
    bVar29 = (bool)(bVar2 >> 6 & 1);
    bVar1 = bVar2 >> 7 != ((byte)(uVar31 >> 0x1c) & 1);
    if (bVar29 || bVar1) {
      AbyssEngine::String::String(aSStack_a0,(char *)(DAT_0017c6f4 + 0x17c38c),false);
    }
    else {
      AbyssEngine::String::String(aSStack_c4,(char *)(DAT_0017c374 + 0x17c2b8),false);
      iVar5 = *piVar26;
      pSVar10 = (String *)GameText::getText(*piVar28);
      AbyssEngine::String::String(aSStack_34c,pSVar10,false);
      AbyssEngine::String::String(aSStack_f4,(int)(fVar32 * DAT_0017c378));
      AbyssEngine::String::String(aSStack_358,aSStack_f4,false);
      uVar6 = AbyssEngine::String::String(aSStack_364,(char *)(DAT_0017c37c + 0x17c308),false);
      Status::replaceHash(aSStack_e8,iVar5,aSStack_34c,aSStack_358,uVar6);
      AbyssEngine::operator+(aSStack_b8,aSStack_c4);
      AbyssEngine::String::String(aSStack_370,(char *)(DAT_0017c380 + 0x17c33a),false);
      AbyssEngine::operator+(aSStack_a0,aSStack_b8);
    }
    AbyssEngine::String::operator=((String *)(this + 0xa4),aSStack_a0);
    AbyssEngine::String::~String(aSStack_a0);
    if (!bVar29 && !bVar1) {
      (*pcVar21)(aSStack_370);
      (*pcVar21)(aSStack_b8);
      (*pcVar21)(aSStack_e8);
      (*pcVar21)(aSStack_364);
      (*pcVar21)(aSStack_358);
      (*pcVar21)(aSStack_f4);
      (*pcVar21)(aSStack_34c);
      (*pcVar21)(aSStack_c4);
    }
    AbyssEngine::String::String(aSStack_37c,aSStack_7c,false);
    Agent::setMissionString(this_03,aSStack_37c);
    AbyssEngine::String::~String(aSStack_37c);
    Agent::getMission(this_03);
    uVar6 = Mission::getReward();
    fVar33 = (float)VectorSignedToFloat(uVar6,(byte)(uVar31 >> 0x16) & 3);
    iVar19 = (int)(fVar32 * fVar33);
    __aeabi_idivmod(iVar19,0x32);
    pMVar18 = (Mission *)Agent::getMission(this_03);
    iVar5 = extraout_r1 + iVar19;
    __aeabi_idivmod(iVar5,0x32);
    if (extraout_r1_00 != 0) {
      iVar5 = iVar19 - extraout_r1;
    }
    Mission::setBonus(pMVar18,iVar5);
    iVar5 = *piVar26;
    AbyssEngine::String::String(aSStack_388,aSStack_7c,false);
    Mission::getReward();
    Mission::getBonus();
    Layout::formatCredits((int)auStack_3ac);
    AbyssEngine::String::String(aSStack_3a0,auStack_3ac,false);
    AbyssEngine::operator+(aSStack_394,aSStack_3a0);
    uVar6 = AbyssEngine::String::String(aSStack_3b8,(char *)(DAT_0017c6fc + 0x17c4a6),false);
    Status::replaceHash(aSStack_a0,iVar5,aSStack_388,aSStack_394,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_3b8);
    (*pcVar21)(aSStack_394);
    (*pcVar21)(aSStack_3a0);
    (*pcVar21)(auStack_3ac);
    pSVar14 = aSStack_388;
    goto LAB_0017c4f0;
  case 1:
    piVar28 = *(int **)(DAT_0017b4d0 + 0x17b2f2);
    do {
      iVar5 = AbyssEngine::AERandom::nextInt(*piVar28);
    } while (*(char *)(*(int *)(this + 0x58) + iVar5) != '\0');
    *(undefined1 *)(*(int *)(this + 0x58) + iVar5) = 1;
    iVar20 = Agent::getRace(this_03);
    iVar19 = iVar5;
    if (iVar20 != 0) {
      iVar19 = 4;
    }
    if (iVar5 != 0x10) {
      iVar19 = iVar5;
    }
    iVar20 = Agent::isMale(this_03);
    piVar27 = *(int **)(DAT_0017b4d4 + 0x17b330);
    iVar5 = iVar19 + 0x334;
    if (iVar20 == 0) {
      iVar5 = 0x338;
    }
    if (iVar19 != 0xd) {
      iVar5 = iVar19 + 0x334;
    }
    *(int *)(this_03 + 0x28) = iVar5;
    pSVar10 = (String *)GameText::getText(*piVar27);
    AbyssEngine::String::operator=(aSStack_7c,pSVar10);
    iVar5 = *piVar26;
    AbyssEngine::String::String(aSStack_208,aSStack_7c,false);
    Globals::getRandomPlanetName();
    AbyssEngine::String::String(aSStack_214,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_220,(char *)(DAT_0017b4dc + 0x17b38c),false);
    Status::replaceHash(aSStack_a0,iVar5,aSStack_208,aSStack_214,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_220);
    (*pcVar21)(aSStack_214);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_208);
    iVar5 = *piVar26;
    AbyssEngine::String::String(aSStack_22c,aSStack_7c,false);
    Agent::getName();
    AbyssEngine::String::String(aSStack_238,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_244,(char *)(DAT_0017b4e0 + 0x17b400),false);
    Status::replaceHash(aSStack_a0,iVar5,aSStack_22c,aSStack_238,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_244);
    (*pcVar21)(aSStack_238);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_22c);
    iVar19 = *piVar26;
    AbyssEngine::String::String(aSStack_250,aSStack_7c,false);
    iVar5 = *piVar27;
    AbyssEngine::AERandom::nextInt(*piVar28);
    pSVar10 = (String *)GameText::getText(iVar5);
    AbyssEngine::String::String(aSStack_25c,pSVar10,false);
    uVar6 = AbyssEngine::String::String(aSStack_268,(char *)(DAT_0017b4e4 + 0x17b480),false);
    Status::replaceHash(aSStack_a0,iVar19,aSStack_250,aSStack_25c,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_268);
    (*pcVar21)(aSStack_25c);
    pSVar14 = aSStack_250;
    goto LAB_0017c4f0;
  case 2:
    piVar28 = *(int **)(DAT_0017b7e8 + 0x17b4ee);
    iVar5 = AbyssEngine::AERandom::nextInt(*piVar28);
    *(int *)(this_03 + 0x28) = iVar5 + 0x300;
    iVar5 = AbyssEngine::AERandom::nextInt(*piVar28);
    *(int *)(this_03 + 0x2c) = iVar5 + 0x305;
    iVar5 = Status::getCurrentCampaignMission();
    if (iVar5 == 0x18) {
      this_01 = (Station *)Status::getStation();
      iVar5 = Station::getIndex(this_01);
      if ((iVar5 == 10) && (iVar5 = Agent::getSellItemIndex(this_03), iVar5 == 0x44)) {
        *(undefined4 *)(this_03 + 0x28) = 0xc1;
        *(undefined4 *)(this_03 + 0x2c) = 0xc2;
      }
    }
    piVar28 = *(int **)(DAT_0017b7ec + 0x17b544);
    pSVar10 = (String *)GameText::getText(*piVar28);
    AbyssEngine::String::operator+=(aSStack_7c,pSVar10);
    AbyssEngine::String::String(aSStack_b8,(char *)(DAT_0017b7f0 + 0x17b562),false);
    GameText::getText(*piVar28);
    AbyssEngine::operator+(aSStack_a0,aSStack_b8);
    AbyssEngine::String::operator+=(aSStack_7c,aSStack_a0);
    AbyssEngine::String::~String(aSStack_a0);
    AbyssEngine::String::~String(aSStack_b8);
    iVar19 = *piVar26;
    AbyssEngine::String::String(aSStack_3c4,aSStack_7c,false);
    iVar5 = Agent::getSellItemQuantity(this_03);
    AbyssEngine::String::String(aSStack_b8,iVar5);
    AbyssEngine::String::String(aSStack_3d0,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_3dc,(char *)(DAT_0017b7f4 + 0x17b5d6),false);
    Status::replaceHash(aSStack_a0,iVar19,aSStack_3c4,aSStack_3d0,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_3dc);
    (*pcVar21)(aSStack_3d0);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_3c4);
    iVar19 = *piVar26;
    AbyssEngine::String::String(aSStack_3e8,aSStack_7c,false);
    iVar5 = *piVar28;
    Agent::getSellItemIndex(this_03);
    pSVar10 = (String *)GameText::getText(iVar5);
    AbyssEngine::String::String(aSStack_3f4,pSVar10,false);
    uVar6 = AbyssEngine::String::String(aSStack_400,(char *)(DAT_0017b7f8 + 0x17b654),false);
    Status::replaceHash(aSStack_a0,iVar19,aSStack_3e8,aSStack_3f4,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_400);
    (*pcVar21)(aSStack_3f4);
    (*pcVar21)(aSStack_3e8);
    iVar5 = *piVar26;
    AbyssEngine::String::String(aSStack_40c,aSStack_7c,false);
    Agent::getSellItemPrice(this_03);
    Layout::formatCredits((int)aSStack_b8);
    AbyssEngine::String::String(aSStack_418,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_424,(char *)(DAT_0017b800 + 0x17b6d4),false);
    Status::replaceHash(aSStack_a0,iVar5,aSStack_40c,aSStack_418,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_424);
    (*pcVar21)(aSStack_418);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_40c);
    iVar5 = Agent::getSellItemQuantity(this_03);
    if (1 < iVar5) {
      AbyssEngine::String::String(aSStack_b8,(char *)(DAT_0017b804 + 0x17b726),false);
      GameText::getText(*piVar28);
      AbyssEngine::operator+(aSStack_a0,aSStack_b8);
      AbyssEngine::String::operator+=(aSStack_7c,aSStack_a0);
      AbyssEngine::String::~String(aSStack_a0);
      AbyssEngine::String::~String(aSStack_b8);
      iVar5 = *piVar26;
      AbyssEngine::String::String(aSStack_430,aSStack_7c,false);
      uVar6 = Agent::getSellItemPrice(this_03);
      uVar24 = Agent::getSellItemQuantity(this_03);
      __aeabi_idiv(uVar6,uVar24);
      Layout::formatCredits((int)aSStack_b8);
      AbyssEngine::String::String(aSStack_43c,aSStack_b8,false);
      uVar6 = AbyssEngine::String::String(aSStack_448,(char *)(DAT_0017b808 + 0x17b7aa),false);
      Status::replaceHash(aSStack_a0,iVar5,aSStack_430,aSStack_43c,uVar6);
      AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
      (*pcVar21)(aSStack_a0);
      (*pcVar21)(aSStack_448);
      (*pcVar21)(aSStack_43c);
      (*pcVar21)(aSStack_b8);
      pSVar14 = aSStack_430;
      goto LAB_0017c4f0;
    }
    break;
  case 5:
    pAVar13 = (Array *)Galaxy::getSystems((Galaxy *)**(undefined4 **)(DAT_0017bb74 + 0x17b812));
    pMVar18 = (Mission *)Generator::createMission(this_00,this_03,pAVar13);
    Agent::setMission(this_03,pMVar18);
    iVar5 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017bb78 + 0x17b830));
    piVar28 = *(int **)(DAT_0017bb7c + 0x17b83e);
    *(int *)(this_03 + 0x28) = iVar5 + 0x309;
    pSVar10 = (String *)GameText::getText(*piVar28);
    AbyssEngine::String::operator+=(aSStack_7c,pSVar10);
    iVar19 = *piVar26;
    AbyssEngine::String::String(aSStack_19c,aSStack_7c,false);
    Agent::getMission(this_03);
    iVar5 = Mission::getProductionGoodAmount();
    AbyssEngine::String::String(aSStack_b8,iVar5);
    AbyssEngine::String::String(aSStack_1a8,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_1b4,(char *)(DAT_0017bb80 + 0x17b890),false);
    Status::replaceHash(aSStack_a0,iVar19,aSStack_19c,aSStack_1a8,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_1b4);
    (*pcVar21)(aSStack_1a8);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_19c);
    iVar19 = *piVar26;
    AbyssEngine::String::String(aSStack_1c0,aSStack_7c,false);
    iVar5 = *piVar28;
    pMVar18 = (Mission *)Agent::getMission(this_03);
    Mission::getProductionGoodIndex(pMVar18);
    pSVar10 = (String *)GameText::getText(iVar5);
    AbyssEngine::String::String(aSStack_1cc,pSVar10,false);
    uVar6 = AbyssEngine::String::String(aSStack_1d8,(char *)(DAT_0017bb84 + 0x17b910),false);
    Status::replaceHash(aSStack_a0,iVar19,aSStack_1c0,aSStack_1cc,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_1d8);
    (*pcVar21)(aSStack_1cc);
    (*pcVar21)(aSStack_1c0);
    iVar5 = *piVar26;
    AbyssEngine::String::String(aSStack_1e4,aSStack_7c,false);
    Agent::getMission(this_03);
    Mission::getReward();
    Layout::formatCredits((int)aSStack_b8);
    AbyssEngine::String::String(aSStack_1f0,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_1fc,(char *)(DAT_0017bb8c + 0x17b994),false);
    Status::replaceHash(aSStack_a0,iVar5,aSStack_1e4,aSStack_1f0,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_1fc);
    (*pcVar21)(aSStack_1f0);
    (*pcVar21)(aSStack_b8);
    pSVar14 = aSStack_1e4;
LAB_0017c4f0:
    (*pcVar21)(pSVar14);
    break;
  case 6:
    iVar5 = **(int **)(DAT_0017bb90 + 0x17b9e4);
    Achievements::gotAllMedals((Achievements *)**(undefined4 **)(DAT_0017bb94 + 0x17b9e6));
    Agent::getWingmanFriendsCount(this_03);
    pSVar10 = (String *)GameText::getText(iVar5);
    AbyssEngine::String::operator+=(aSStack_7c,pSVar10);
    iVar5 = *piVar26;
    AbyssEngine::String::String(aSStack_454,aSStack_7c,false);
    Agent::getCosts(this_03);
    Layout::formatCredits((int)aSStack_b8);
    AbyssEngine::String::String(aSStack_460,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_46c,(char *)(DAT_0017bb9c + 0x17ba54),false);
    Status::replaceHash(aSStack_a0,iVar5,aSStack_454,aSStack_460,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_46c);
    (*pcVar21)(aSStack_460);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_454);
    iVar5 = Agent::getWingmanFriendsCount(this_03);
    if (0 < iVar5) {
      iVar5 = *piVar26;
      AbyssEngine::String::String(aSStack_478,aSStack_7c,false);
      Agent::getWingmanName((int)aSStack_b8);
      AbyssEngine::String::String(aSStack_484,aSStack_b8,false);
      uVar6 = AbyssEngine::String::String(aSStack_490,(char *)(DAT_0017bba0 + 0x17bacc),false);
      Status::replaceHash(aSStack_a0,iVar5,aSStack_478,aSStack_484,uVar6);
      AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
      (*pcVar21)(aSStack_a0);
      (*pcVar21)(aSStack_490);
      (*pcVar21)(aSStack_484);
      (*pcVar21)(aSStack_b8);
      pSVar14 = aSStack_478;
      goto LAB_0017c4f0;
    }
    break;
  case 7:
    Agent::getRace(this_03);
    iVar5 = Status::getStanding();
    iVar5 = Standing::getStanding(iVar5);
    iVar19 = Status::getStanding();
    iVar19 = Standing::isEnemy(iVar19);
    if (iVar19 == 0) {
      pSVar10 = (String *)GameText::getText(**(int **)(DAT_0017bfcc + 0x17bf24));
      AbyssEngine::String::operator=(aSStack_7c,pSVar10);
      pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
      Agent::getName();
      AbyssEngine::String::String(aSStack_190,aSStack_7c,false);
      ScrollTouchWindow::setText(pSVar8,auStack_184,aSStack_190);
      (*pcVar21)(aSStack_190);
      (*pcVar21)(auStack_184);
      *(undefined4 *)(this + 0x14) = 3;
      *(undefined4 *)(this + 0x2c) = 0;
      (*pcVar21)(aSStack_7c);
      (*pcVar21)(aSStack_70);
      (*pcVar21)(aSStack_64);
      (*pcVar21)(&uStack_40);
      goto LAB_0017c8d4;
    }
    if (iVar5 < 0) {
      iVar5 = -iVar5;
    }
    fVar32 = (float)VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
    fVar32 = (fVar32 / DAT_0017bba4) * DAT_0017bba8;
    pSVar10 = (String *)GameText::getText(**(int **)(DAT_0017d100 + 0x17d038));
    AbyssEngine::String::operator=(aSStack_7c,pSVar10);
    iVar5 = *piVar26;
    AbyssEngine::String::String(aSStack_160,aSStack_7c,false);
    Layout::formatCredits((int)aSStack_b8);
    AbyssEngine::String::String(aSStack_16c,aSStack_b8,false);
    uVar6 = AbyssEngine::String::String(aSStack_178,(char *)(DAT_0017d108 + 0x17d086),false);
    Status::replaceHash(aSStack_a0,iVar5,aSStack_160,aSStack_16c,uVar6);
    AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
    (*pcVar21)(aSStack_a0);
    (*pcVar21)(aSStack_178);
    (*pcVar21)(aSStack_16c);
    (*pcVar21)(aSStack_b8);
    (*pcVar21)(aSStack_160);
    Agent::setCosts(this_03,(int)fVar32);
  }
  pvVar7 = (void *)Generator::~Generator(this_00);
  operator_delete(pvVar7);
  iVar5 = Agent::getOffer(this_03);
  if (iVar5 == 0) {
    Agent::getMission(this_03);
    iVar5 = Mission::getType();
    if (iVar5 == 8) goto LAB_0017c512;
  }
  else {
LAB_0017c512:
    AbyssEngine::String::String(aSStack_49c,aSStack_7c,false);
    Agent::setMissionString(this_03,aSStack_49c);
    AbyssEngine::String::~String(aSStack_49c);
  }
  iVar5 = Agent::getOffer(this_03);
  if (iVar5 == 1) {
    pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
    Agent::getName();
    AbyssEngine::String::String(aSStack_4b4,aSStack_7c,false);
    ScrollTouchWindow::setText(pSVar8,aSStack_4a8,aSStack_4b4);
    AbyssEngine::String::~String(aSStack_4b4);
    AbyssEngine::String::~String(aSStack_4a8);
    Agent::setEvent(this_03,1);
  }
  else {
    iVar5 = Agent::getOffer(this_03);
    if (iVar5 == 7) {
      pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
      Agent::getName();
      AbyssEngine::String::String(aSStack_4cc,aSStack_7c,false);
      ScrollTouchWindow::setText(pSVar8,aSStack_4c0,aSStack_4cc);
      AbyssEngine::String::~String(aSStack_4cc);
      AbyssEngine::String::~String(aSStack_4c0);
    }
    else {
      iVar5 = Agent::getMission(this_03);
      if (iVar5 == 0) {
LAB_0017c714:
        AbyssEngine::String::String(aSStack_3a0,(char *)(DAT_0017cb00 + 0x17c71a),false);
        AbyssEngine::operator+(aSStack_a0,aSStack_3a0);
        AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
        AbyssEngine::String::~String(aSStack_a0);
        AbyssEngine::String::~String(aSStack_3a0);
        AbyssEngine::String::String(aSStack_3a0,(char *)(DAT_0017cb04 + 0x17c754),false);
        iVar5 = **(int **)(DAT_0017cb08 + 0x17c766);
        AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017cb0c + 0x17c768));
        GameText::getText(iVar5);
        AbyssEngine::operator+(aSStack_a0,aSStack_3a0);
        AbyssEngine::String::~String(aSStack_3a0);
        pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
        Agent::getName();
        AbyssEngine::String::String(aSStack_55c,(char *)(DAT_0017cb10 + 0x17c7a6),false);
        AbyssEngine::operator+(aSStack_550,(String *)&uStack_40);
        AbyssEngine::operator+(aSStack_544,aSStack_550);
        AbyssEngine::String::String(aSStack_568,(char *)(DAT_0017cb14 + 0x17c7ca),false);
        AbyssEngine::operator+(aSStack_538,aSStack_544);
        AbyssEngine::operator+(auStack_3ac,aSStack_538);
        AbyssEngine::operator+(aSStack_3a0,auStack_3ac);
        AbyssEngine::operator+(aSStack_52c,aSStack_3a0);
        ScrollTouchWindow::setText(pSVar8,auStack_520,aSStack_52c);
        (*pcVar21)(aSStack_52c);
        (*pcVar21)(aSStack_3a0);
        (*pcVar21)(auStack_3ac);
        (*pcVar21)(aSStack_538);
        (*pcVar21)(aSStack_568);
        (*pcVar21)(aSStack_544);
        (*pcVar21)(aSStack_550);
        (*pcVar21)(aSStack_55c);
        puVar15 = auStack_520;
      }
      else {
        Agent::getMission(this_03);
        iVar5 = Mission::getType();
        if (iVar5 != 0xc) goto LAB_0017c714;
        piVar28 = *(int **)(DAT_0017c700 + 0x17c5cc);
        pSVar10 = (String *)GameText::getText(*piVar28);
        AbyssEngine::String::operator=(aSStack_7c,pSVar10);
        iVar5 = *piVar26;
        AbyssEngine::String::String(aSStack_4d8,aSStack_7c,false);
        Agent::getMission(this_03);
        Mission::getReward();
        Agent::getMission(this_03);
        Mission::getBonus();
        Layout::formatCredits((int)aSStack_3a0);
        AbyssEngine::String::String(aSStack_4e4,aSStack_3a0,false);
        uVar6 = AbyssEngine::String::String(aSStack_4f0,(char *)(DAT_0017c708 + 0x17c632),false);
        Status::replaceHash(aSStack_a0,iVar5,aSStack_4d8,aSStack_4e4,uVar6);
        AbyssEngine::String::operator=(aSStack_7c,aSStack_a0);
        (*pcVar21)(aSStack_a0);
        (*pcVar21)(aSStack_4f0);
        (*pcVar21)(aSStack_4e4);
        (*pcVar21)(aSStack_3a0);
        (*pcVar21)(aSStack_4d8);
        AbyssEngine::String::String(aSStack_4fc,aSStack_7c,false);
        Agent::setMissionString(this_03,aSStack_4fc);
        AbyssEngine::String::~String(aSStack_4fc);
        AbyssEngine::String::String(aSStack_3a0,(char *)(DAT_0017c70c + 0x17c68c),false);
        iVar5 = *piVar28;
        AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017c710 + 0x17c69e));
        GameText::getText(iVar5);
        AbyssEngine::operator+(aSStack_a0,aSStack_3a0);
        AbyssEngine::String::~String(aSStack_3a0);
        pSVar8 = *(ScrollTouchWindow **)(this + 0x60);
        Agent::getName();
        AbyssEngine::operator+(aSStack_514,aSStack_7c);
        ScrollTouchWindow::setText(pSVar8,auStack_508,aSStack_514);
        (*pcVar21)(aSStack_514);
        puVar15 = auStack_508;
      }
      (*pcVar21)(puVar15);
      (*pcVar21)(aSStack_a0);
    }
  }
  (*pcVar21)(aSStack_7c);
  (*pcVar21)(aSStack_70);
  (*pcVar21)(aSStack_64);
  (*pcVar21)(&uStack_40);
LAB_0017c85a:
  Status::getStation();
  Station::getName();
  Agent::setStationName(this_03,aSStack_7d8);
  AbyssEngine::String::~String(aSStack_7d8);
  Status::getSystem();
  SolarSystem::getName();
  Agent::setSystemName(this_03,aSStack_7e4);
  AbyssEngine::String::~String(aSStack_7e4);
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x14) = 1;
  onKeyPress(this,0x10000);
  pVVar16 = (Vector *)getSoundId(this,this_03);
  if (-1 < (int)pVVar16) {
    FModSound::play(**(int **)(DAT_0017cb1c + 0x17c8ce),pVVar16,(Vector *)0x0,extraout_s0_01);
  }
LAB_0017c8d4:
  if (*piVar4 != iStack_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0017aa7c: push {r4,r5,r6,r7,lr}
  0017aa7e: add r7,sp,#0xc
  0017aa80: push {r7,r8,r9,r10,r11}
  0017aa84: vpush {d8}
  0017aa88: sub.w sp,sp,#0x7d0
  0017aa8c: mov r11,r0
  0017aa8e: ldr r0,[0x0017ae60]
  0017aa90: ldr r1,[0x0017ae64]
  0017aa92: add r0,pc
  0017aa94: add r1,pc
  0017aa96: ldr r0,[r0,#0x0]
  0017aa98: str r0,[sp,#0x14]
  0017aa9a: ldr r1,[r1,#0x0]
  0017aa9c: ldr r0,[r0,#0x0]
  0017aa9e: str.w r0,[r7,#-0x2c]
  0017aaa2: ldrd r0,r2,[r11,#0x20]
  0017aaa6: ldr r2,[r2,#0x4]
  0017aaa8: ldrb r1,[r1,#0x0]
  0017aaaa: ldr.w r9,[r2,r0,lsl #0x2]
  0017aaae: cmp r1,#0x0
  0017aab0: str.w r9,[sp,#0x18]
  0017aab4: beq.w 0x0017ad50
  0017aab8: ldr r1,[0x0017ae68]
  0017aaba: add r1,pc
  0017aabc: ldr.w r8,[r1,#0x0]
  0017aac0: ldrb.w r1,[r8,#0x0]
  0017aac4: cbz r1,0x0017aacc
  0017aac6: vldr.32 s0,[pc,#0x3a4]
  0017aaca: b 0x0017aae4
  0017aacc: ldr r1,[0x0017ae70]
  0017aace: vmov.f32 s2,0x41200000
  0017aad2: vmov.f32 s0,0x41a00000
  0017aad6: add r1,pc
  0017aad8: ldr r1,[r1,#0x0]
  0017aada: ldrb r1,[r1,#0x0]
  0017aadc: cmp r1,#0x0
  0017aade: it eq
  0017aae0: vmov.eq.f32 s0,s2
  0017aae4: ldr r2,[0x0017ae74]
  0017aae6: ldr.w r1,[r11,#0x40]
  0017aaea: add r2,pc
  0017aaec: ldr r1,[r1,#0x4]
  0017aaee: vcvt.s32.f32 s0,s0
  0017aaf2: ldr.w r9,[r2,#0x0]
  0017aaf6: vcvt.f32.s32 s2,s0
  0017aafa: ldr.w r2,[r1,r0,lsl #0x3]
  0017aafe: ldr.w r4,[r9,#0x0]
  0017ab02: lsls r0,r0,#0x1
  0017ab04: vldr.32 s4,[r2]
  0017ab08: vldr.32 s0,[r4,#0x68]
  0017ab0c: vcvt.f32.s32 s0,s0
  0017ab10: ldr r2,[r4,#0x28]
  0017ab12: vsub.f32 s4,s4,s2
  0017ab16: vmov s6,r2
  0017ab1a: vcvt.f32.s32 s6,s6
  0017ab1e: vsub.f32 s0,s4,s0
  0017ab22: vcmpe.f32 s0,s6
  0017ab26: vmrs apsr,fpscr
  0017ab2a: bpl 0x0017ab62
  0017ab2c: ldr r3,[0x0017ae78]
  0017ab2e: orr r0,r0,#0x1
  0017ab32: add r3,pc
  0017ab34: ldr.w r6,[r1,r0,lsl #0x2]
  0017ab38: add.w r10,r1,r0, lsl #0x2
  0017ab3c: ldr r3,[r3,#0x0]
  0017ab3e: vldr.32 s0,[r6]
  0017ab42: ldr r3,[r3,#0x0]
  0017ab44: vadd.f32 s2,s0,s2
  0017ab48: subs r2,r3,r2
  0017ab4a: vmov s0,r2
  0017ab4e: vcvt.f32.s32 s0,s0
  0017ab52: vcmpe.f32 s2,s0
  0017ab56: vmrs apsr,fpscr
  0017ab5a: it mi
  0017ab5c: vmov.mi.f32 s0,s2
  0017ab60: b 0x0017ab6a
  0017ab62: orr r0,r0,#0x1
  0017ab66: add.w r10,r1,r0, lsl #0x2
  0017ab6a: vcvt.s32.f32 s0,s0
  0017ab6e: movs r1,#0x3
  0017ab70: vstr.32 s0,[r11,#0x70]
  0017ab74: ldr r5,[r4,#0x6c]
  0017ab76: mov r0,r5
  0017ab78: blx 0x0007198c
  0017ab7c: ldr r1,[0x0017ae7c]
  0017ab7e: vmov s0,r0
  0017ab82: add r1,pc
  0017ab84: vcvt.f32.s32 s0,s0
  0017ab88: ldrd r12,r2,[r4,#0xc]
  0017ab8c: ldr r1,[r1,#0x0]
  0017ab8e: ldrd r3,r6,[r4,#0x20]
  0017ab92: ldr.w r0,[r10,#0x0]
  0017ab96: ldr r1,[r1,#0x0]
  0017ab98: subs r1,r1,r2
  0017ab9a: vldr.32 s2,[r0,#0x4]
  0017ab9e: subs r1,r1,r6
  0017aba0: subs r0,r1,r5
  0017aba2: vsub.f32 s0,s2,s0
  0017aba6: vmov s4,r0
  0017abaa: add.w r0,r3,r12
  0017abae: vcvt.f32.s32 s4,s4
  0017abb2: vmov s2,r0
  0017abb6: vcvt.f32.s32 s2,s2
  0017abba: vcmpe.f32 s0,s4
  0017abbe: vmrs apsr,fpscr
  0017abc2: vmov.f32 s6,s4
  0017abc6: it mi
  0017abc8: vmov.mi.f32 s6,s0
  0017abcc: vcmpe.f32 s6,s2
  0017abd0: vmrs apsr,fpscr
  0017abd4: vmov.f32 s6,s0
  0017abd8: vcmpe.f32 s0,s4
  0017abdc: it mi
  0017abde: vmov.mi.f32 s6,s2
  0017abe2: it mi
  0017abe4: vmov.mi.f32 s4,s6
  0017abe8: vmrs apsr,fpscr
  0017abec: it mi
  0017abee: vmov.mi.f32 s4,s6
  0017abf2: vcvt.s32.f32 s0,s4
  0017abf6: ldr.w r0,[r11,#0x60]
  0017abfa: cmp r0,#0x0
  0017abfc: vstr.32 s0,[r11,#0x74]
  0017ac00: beq 0x0017ac0e
  0017ac02: blx 0x00075afc
  0017ac06: blx 0x0006eb48
  0017ac0a: ldr.w r4,[r9,#0x0]
  0017ac0e: mov.w r10,#0x0
  0017ac12: movs r0,#0x24
  0017ac14: str.w r10,[r11,#0x60]
  0017ac18: blx 0x0006eb24
  0017ac1c: mov r5,r0
  0017ac1e: ldrd r3,r0,[r11,#0x6c]
  0017ac22: ldr r1,[r4,#0x4c]
  0017ac24: ldr r6,[r4,#0x6c]
  0017ac26: ldr.w r2,[r11,#0x74]
  0017ac2a: ldr.w r4,[r4,#0x2d4]
  0017ac2e: add.w r0,r0,r1, lsl #0x1
  0017ac32: strd r6,r10,[sp,#0x0]
  0017ac36: adds r1,r0,r4
  0017ac38: mov r0,r5
  0017ac3a: blx 0x00075b20
  0017ac3e: str.w r5,[r11,#0x60]
  0017ac42: add.w r3,r11,#0x6c
  0017ac46: ldr.w r0,[r9,#0x0]
  0017ac4a: ldmia r3,{r1,r2,r3}
  0017ac4c: ldr r5,[r0,#0x6c]
  0017ac4e: ldr r6,[r0,#0x2c]
  0017ac50: add r3,r5
  0017ac52: add r3,r6
  0017ac54: str.w r3,[r11,#0x78]
  0017ac58: ldr r5,[r0,#0x68]
  0017ac5a: ldr r6,[r0,#0x4c]
  0017ac5c: add r2,r5
  0017ac5e: subs r2,r2,r6
  0017ac60: subs r1,r2,r1
  0017ac62: str.w r1,[r11,#0x84]
  0017ac66: vldr.32 s0,[r0,#0x34]
  0017ac6a: adds r2,r6,r3
  0017ac6c: vcvt.f32.s32 s0,s0
  0017ac70: ldrb.w r5,[r8,#0x0]
  0017ac74: cbz r5,0x0017ac7c
  0017ac76: vldr.32 s2,[pc,#0x208]
  0017ac7a: b 0x0017ac94
  0017ac7c: ldr r3,[0x0017ae84]
  0017ac7e: vmov.f32 s4,0x41300000
  0017ac82: vldr.32 s2,[pc,#0x1fc]
  0017ac86: add r3,pc
  0017ac88: ldr r3,[r3,#0x0]
  0017ac8a: ldrb r3,[r3,#0x0]
  0017ac8c: cmp r3,#0x0
  0017ac8e: it eq
  0017ac90: vmov.eq.f32 s2,s4
  0017ac94: ldr r3,[r0,#0x30]
  0017ac96: vadd.f32 s0,s2,s0
  0017ac9a: ldr r4,[0x0017ae88]
  0017ac9c: ldr.w r0,[r0,#0x2d8]
  0017aca0: add.w r5,r3,r3, lsr #0x1f
  0017aca4: ldr.w r6,[r11,#0x5c]
  0017aca8: add r4,pc
  0017acaa: add.w r0,r0,r0, lsr #0x1f
  0017acae: sub.w r5,r2,r5, asr #0x1
  0017acb2: ldr r4,[r4,#0x0]
  0017acb4: add.w r0,r5,r0, asr #0x1
  0017acb8: strd r0,r2,[r11,#0x7c]
  0017acbc: ldr r0,[r6,#0x4]
  0017acbe: vcvt.s32.f32 s0,s0
  0017acc2: add r2,r3
  0017acc4: ldr r0,[r0,#0x8]
  0017acc6: vmov r6,s0
  0017acca: adds r5,r2,r6
  0017accc: mov r2,r5
  0017acce: blx r4
  0017acd0: ldr.w r0,[r11,#0x5c]
  0017acd4: mov r2,r5
  0017acd6: ldr.w r1,[r11,#0x84]
  0017acda: ldr r0,[r0,#0x4]
  0017acdc: ldr r0,[r0,#0xc]
  0017acde: blx r4
  0017ace0: ldr.w r0,[r11,#0x5c]
  0017ace4: adds r3,r5,r6
  0017ace6: ldr.w r2,[r9,#0x0]
  0017acea: ldr.w r1,[r11,#0x84]
  0017acee: ldr r0,[r0,#0x4]
  0017acf0: ldr r2,[r2,#0x30]
  0017acf2: ldr r0,[r0,#0x10]
  0017acf4: add r2,r3
  0017acf6: blx r4
  0017acf8: ldr.w r1,[r11,#0x40]
  0017acfc: ldr.w r0,[r11,#0x20]
  0017ad00: ldr r1,[r1,#0x4]
  0017ad02: ldr.w r0,[r1,r0,lsl #0x3]
  0017ad06: ldrd r1,r2,[r0,#0x0]
  0017ad0a: ldr r0,[r0,#0x8]
  0017ad0c: strd r1,r2,[r7,#-0x38]
  0017ad10: sub.w r1,r7,#0x38
  0017ad14: str.w r0,[r7,#-0x30]
  0017ad18: add.w r0,r11,#0x8c
  0017ad1c: blx 0x0006eb3c
  0017ad20: ldr.w r1,[r11,#0x40]
  0017ad24: movs r2,#0x4
  0017ad26: ldr.w r0,[r11,#0x20]
  0017ad2a: ldr r1,[r1,#0x4]
  0017ad2c: orr.w r0,r2,r0, lsl #0x3
  0017ad30: ldr r0,[r1,r0]
  0017ad32: ldrd r1,r2,[r0,#0x0]
  0017ad36: ldr r0,[r0,#0x8]
  0017ad38: strd r1,r2,[r7,#-0x38]
  0017ad3c: sub.w r1,r7,#0x38
  0017ad40: str.w r0,[r7,#-0x30]
  0017ad44: add.w r0,r11,#0x98
  0017ad48: blx 0x0006eb3c
  0017ad4c: ldr.w r9,[sp,#0x18]
  0017ad50: mov r0,r9
  0017ad52: blx 0x00071ae8
  0017ad56: mov r6,r0
  0017ad58: mov r0,r9
  0017ad5a: blx 0x00071b9c
  0017ad5e: cbz r0,0x0017ada2
  0017ad60: mov r0,r9
  0017ad62: blx 0x00077d1c
  0017ad66: cbz r0,0x0017ad74
  0017ad68: mov r0,r9
  0017ad6a: blx 0x00071ae8
  0017ad6e: cmp r0,#0x7
  0017ad70: bne.w 0x0017aea0
  0017ad74: ldr r0,[0x0017ae8c]
  0017ad76: cmp r6,#0x1
  0017ad78: add r0,pc
  0017ad7a: ldr r4,[r0,#0x0]
  0017ad7c: ldr r0,[r4,#0x0]
  0017ad7e: ldr.w r1,[r0,#0xd0]
  0017ad82: add.w r1,r1,#0x1
  0017ad86: str.w r1,[r0,#0xd0]
  0017ad8a: it ne
  0017ad8c: cmp.ne r6,#0x7
  0017ad8e: bne.w 0x0017aec8
  0017ad92: ldr r1,[0x0017ae90]
  0017ad94: sub.w r0,r7,#0x38
  0017ad98: movs r2,#0x0
  0017ad9a: add r1,pc
  0017ad9c: blx 0x0006ee18
  0017ada0: b 0x0017aef4
  0017ada2: sub.w r0,r7,#0x38
  0017ada6: blx 0x0006efbc
  0017adaa: mov r0,r9
  0017adac: blx 0x00074b9c
  0017adb0: cmp r0,#0x1
  0017adb2: blt 0x0017ae16
  0017adb4: ldr r0,[0x0017ae94]
  0017adb6: add r0,pc
  0017adb8: ldr r0,[r0,#0x0]
  0017adba: ldr r0,[r0,#0x0]
  0017adbc: movw r1,#0x35a
  0017adc0: blx 0x00072f70
  0017adc4: mov r1,r0
  0017adc6: sub.w r0,r7,#0x38
  0017adca: blx 0x0006f2b0
  0017adce: ldr.w r5,[r11,#0x60]
  0017add2: add r0,sp,#0x40
  0017add4: mov r1,r9
  0017add6: blx 0x00071c2c
  0017adda: add r0,sp,#0x34
  0017addc: sub.w r1,r7,#0x38
  0017ade0: movs r2,#0x0
  0017ade2: blx 0x0006f028
  0017ade6: add r1,sp,#0x40
  0017ade8: add r2,sp,#0x34
  0017adea: mov r0,r5
  0017adec: blx 0x00075b2c
  0017adf0: add r0,sp,#0x34
  0017adf2: blx 0x0006ee30
  0017adf6: add r0,sp,#0x40
  0017adf8: blx 0x0006ee30
  0017adfc: movs r0,#0x3
  0017adfe: str.w r0,[r11,#0x14]
  0017ae02: movs r0,#0x0
  0017ae04: str.w r0,[r11,#0x2c]
  0017ae08: sub.w r0,r7,#0x38
  0017ae0c: blx 0x0006ee30
  0017ae10: ldr r5,[sp,#0x14]
  0017ae12: b.w 0x0017c8d4
  0017ae16: mov r0,r9
  0017ae18: blx 0x00074bf0
  0017ae1c: cmp r0,#0x0
  0017ae1e: beq.w 0x0017b146
  0017ae22: ldr r0,[0x0017ae98]
  0017ae24: add r0,pc
  0017ae26: ldr r0,[r0,#0x0]
  0017ae28: ldr r0,[r0,#0x0]
  0017ae2a: movw r1,#0x35a
  0017ae2e: blx 0x00072f70
  0017ae32: mov r1,r0
  0017ae34: sub.w r0,r7,#0x38
  0017ae38: blx 0x0006f2b0
  0017ae3c: mov r0,r11
  0017ae3e: mov r1,r9
  0017ae40: blx 0x00077d4c
  0017ae44: mov r1,r0
  0017ae46: cmp r1,#0x0
  0017ae48: blt 0x0017adce
  0017ae4a: ldr r0,[0x0017ae9c]
  0017ae4c: add r0,pc
  0017ae4e: ldr r0,[r0,#0x0]
  0017ae50: ldr r0,[r0,#0x0]
  0017ae52: movs r2,#0x0
  0017ae54: movs r3,#0x0
  0017ae56: str r2,[sp,#0x0]
  0017ae58: movs r2,#0x0
  0017ae5a: blx 0x00071548
  0017ae5e: b 0x0017adce
  0017aea0: mov r0,r9
  0017aea2: blx 0x00074bf0
  0017aea6: cmp r0,#0x0
  0017aea8: beq.w 0x0017b288
  0017aeac: ldr r0,[0x0017b238]
  0017aeae: add r0,pc
  0017aeb0: ldr r0,[r0,#0x0]
  0017aeb2: ldr r6,[r0,#0x0]
  0017aeb4: mov r0,r9
  0017aeb6: blx 0x00071ae8
  0017aeba: cmp r0,#0x5
  0017aebc: bne.w 0x0017bf0a
  0017aec0: movw r5,#0x359
  0017aec4: b.w 0x0017cdb0
  0017aec8: ldr r0,[0x0017b23c]
  0017aeca: ldr r1,[0x0017b240]
  0017aecc: add r0,pc
  0017aece: add r1,pc
  0017aed0: ldr r0,[r0,#0x0]
  0017aed2: ldr r1,[r1,#0x0]
  0017aed4: ldr r5,[r0,#0x0]
  0017aed6: ldr r0,[r1,#0x0]
  0017aed8: movs r1,#0x6
  0017aeda: blx 0x00071848
  0017aede: addw r1,r0,#0x2ee
  0017aee2: mov r0,r5
  0017aee4: blx 0x00072f70
  0017aee8: mov r1,r0
  0017aeea: sub.w r0,r7,#0x38
  0017aeee: movs r2,#0x0
  0017aef0: blx 0x0006f028
  0017aef4: cmp r6,#0x1
  0017aef6: it ne
  0017aef8: cmp.ne r6,#0x7
  0017aefa: bne 0x0017af0c
  0017aefc: ldr r1,[0x0017b244]
  0017aefe: add r1,pc
  0017af00: sub.w r0,r7,#0x5c
  0017af04: movs r2,#0x0
  0017af06: blx 0x0006ee18
  0017af0a: b 0x0017af38
  0017af0c: ldr r0,[0x0017b248]
  0017af0e: ldr r1,[0x0017b24c]
  0017af10: add r0,pc
  0017af12: add r1,pc
  0017af14: ldr r0,[r0,#0x0]
  0017af16: ldr r1,[r1,#0x0]
  0017af18: ldr r5,[r0,#0x0]
  0017af1a: ldr r0,[r1,#0x0]
  0017af1c: movs r1,#0x2
  0017af1e: blx 0x00071848
  0017af22: add.w r1,r0,#0x2f4
  0017af26: mov r0,r5
  0017af28: blx 0x00072f70
  0017af2c: mov r1,r0
  0017af2e: sub.w r0,r7,#0x5c
  0017af32: movs r2,#0x0
  0017af34: blx 0x0006f028
  0017af38: cmp r6,#0x5
  0017af3a: it ne
  0017af3c: cmp.ne r6,#0x0
  0017af3e: bne 0x0017af6e
  0017af40: ldr r0,[0x0017b250]
  0017af42: ldr r1,[0x0017b254]
  0017af44: add r0,pc
  0017af46: add r1,pc
  0017af48: ldr r0,[r0,#0x0]
  0017af4a: ldr r1,[r1,#0x0]
  0017af4c: ldr r5,[r0,#0x0]
  0017af4e: ldr r0,[r1,#0x0]
  0017af50: movs r1,#0x6
  0017af52: blx 0x00071848
  0017af56: addw r1,r0,#0x2f6
  0017af5a: mov r0,r5
  0017af5c: blx 0x00072f70
  0017af60: mov r1,r0
  0017af62: sub.w r0,r7,#0x68
  0017af66: movs r2,#0x0
  0017af68: blx 0x0006f028
  0017af6c: b 0x0017af7c
  0017af6e: ldr r1,[0x0017b258]
  0017af70: add r1,pc
  0017af72: sub.w r0,r7,#0x68
  0017af76: movs r2,#0x0
  0017af78: blx 0x0006ee18
  0017af7c: ldr r1,[0x0017b25c]
  0017af7e: add r1,pc
  0017af80: sub.w r0,r7,#0x74
  0017af84: movs r2,#0x0
  0017af86: blx 0x0006ee18
  0017af8a: ldr r6,[r4,#0x0]
  0017af8c: addw r0,sp,#0x6c4
  0017af90: sub.w r1,r7,#0x5c
  0017af94: movs r2,#0x0
  0017af96: blx 0x0006f028
  0017af9a: sub.w r0,r7,#0xb0
  0017af9e: mov r1,r9
  0017afa0: blx 0x00071c2c
  0017afa4: add.w r0,sp,#0x6b8
  0017afa8: sub.w r1,r7,#0xb0
  0017afac: movs r2,#0x0
  0017afae: blx 0x0006f028
  0017afb2: ldr r1,[0x0017b260]
  0017afb4: add r1,pc
  0017afb6: addw r0,sp,#0x6ac
  0017afba: movs r2,#0x0
  0017afbc: blx 0x0006ee18
  0017afc0: str r0,[sp,#0x0]
  0017afc2: sub.w r0,r7,#0x98
  0017afc6: addw r2,sp,#0x6c4
  0017afca: add.w r3,sp,#0x6b8
  0017afce: mov r1,r6
  0017afd0: str r4,[sp,#0x10]
  0017afd2: blx 0x000739b4
  0017afd6: sub.w r0,r7,#0x5c
  0017afda: sub.w r1,r7,#0x98
  0017afde: blx 0x0006f2b0
  0017afe2: ldr r0,[0x0017b264]
  0017afe4: add r0,pc
  0017afe6: ldr.w r10,[r0,#0x0]
  0017afea: sub.w r0,r7,#0x98
  0017afee: blx r10
  0017aff0: addw r0,sp,#0x6ac
  0017aff4: blx r10
  0017aff6: add.w r0,sp,#0x6b8
  0017affa: blx r10
  0017affc: sub.w r0,r7,#0xb0
  0017b000: blx r10
  0017b002: addw r0,sp,#0x6c4
  0017b006: blx r10
  0017b008: movs r0,#0x1
  0017b00a: blx 0x0006eb24
  0017b00e: mov r5,r0
  0017b010: blx 0x000736cc
  0017b014: ldr r0,[sp,#0x18]
  0017b016: blx 0x00071ae8
  0017b01a: ldr.w r9,[sp,#0x18]
  0017b01e: cmp r0,#0x7
  0017b020: str r5,[sp,#0xc]
  0017b022: bhi.w 0x0017c4f2
  0017b026: tbh [pc,r0]
  0017b03a: mov r0,r9
  0017b03c: blx 0x00071bb4
  0017b040: mov r4,r0
  0017b042: ldr r0,[0x0017b268]
  0017b044: add r0,pc
  0017b046: ldr.w r8,[r0,#0x0]
  0017b04a: ldr.w r5,[r8,#0x0]
  0017b04e: mov r0,r4
  0017b050: blx 0x00072874
  0017b054: addw r1,r0,#0x312
  0017b058: mov r0,r5
  0017b05a: blx 0x00072f70
  0017b05e: mov r1,r0
  0017b060: sub.w r0,r7,#0x74
  0017b064: blx 0x0006ef5c
  0017b068: mov r0,r4
  0017b06a: blx 0x00072874
  0017b06e: cmp r0,#0x5
  0017b070: beq 0x0017b07c
  0017b072: mov r0,r4
  0017b074: blx 0x00072874
  0017b078: cmp r0,#0x3
  0017b07a: bne 0x0017b0c0
  0017b07c: ldr r1,[0x0017b26c]
  0017b07e: add r1,pc
  0017b080: sub.w r0,r7,#0xb0
  0017b084: movs r2,#0x0
  0017b086: blx 0x0006ee18
  0017b08a: ldr.w r0,[r8,#0x0]
  0017b08e: movw r1,#0x322
  0017b092: blx 0x00072f70
  0017b096: mov r2,r0
  0017b098: sub.w r0,r7,#0x98
  0017b09c: sub.w r1,r7,#0xb0
  0017b0a0: blx 0x0006ef98
  0017b0a4: sub.w r0,r7,#0x74
  0017b0a8: sub.w r1,r7,#0x98
  0017b0ac: blx 0x0006ef5c
  0017b0b0: sub.w r0,r7,#0x98
  0017b0b4: blx 0x0006ee30
  0017b0b8: sub.w r0,r7,#0xb0
  0017b0bc: blx 0x0006ee30
  0017b0c0: mov r0,r4
  0017b0c2: blx 0x00072874
  0017b0c6: cmp r0,#0xf
  0017b0c8: bne.w 0x0017bfd0
  0017b0cc: ldr r0,[sp,#0x10]
  0017b0ce: ldr r6,[r0,#0x0]
  0017b0d0: addw r0,sp,#0x58c
  0017b0d4: sub.w r1,r7,#0x74
  0017b0d8: movs r2,#0x0
  0017b0da: blx 0x0006f028
  0017b0de: ldr.w r5,[r8,#0x0]
  0017b0e2: mov r0,r4
  0017b0e4: blx 0x000728a4
  0017b0e8: addw r1,r0,#0x4fa
  0017b0ec: mov r0,r5
  0017b0ee: blx 0x00072f70
  0017b0f2: mov r1,r0
  0017b0f4: add.w r0,sp,#0x580
  0017b0f8: movs r2,#0x0
  0017b0fa: blx 0x0006f028
  0017b0fe: ldr r1,[0x0017b270]
  0017b100: add r1,pc
  0017b102: addw r0,sp,#0x574
  0017b106: movs r2,#0x0
  0017b108: blx 0x0006ee18
  0017b10c: str r0,[sp,#0x0]
  0017b10e: sub.w r0,r7,#0x98
  0017b112: addw r2,sp,#0x58c
  0017b116: add.w r3,sp,#0x580
  0017b11a: mov r1,r6
  0017b11c: blx 0x000739b4
  0017b120: sub.w r0,r7,#0x74
  0017b124: sub.w r1,r7,#0x98
  0017b128: blx 0x0006f2b0
  0017b12c: sub.w r0,r7,#0x98
  0017b130: blx r10
  0017b132: addw r0,sp,#0x574
  0017b136: blx r10
  0017b138: add.w r0,sp,#0x580
  0017b13c: blx r10
  0017b13e: addw r0,sp,#0x58c
  0017b142: b.w 0x0017c046
  0017b146: ldr r0,[0x0017b274]
  0017b148: add r0,pc
  0017b14a: ldr.w r10,[r0,#0x0]
  0017b14e: ldr.w r0,[r10,#0x0]
  0017b152: ldr.w r1,[r0,#0xd0]
  0017b156: adds r1,#0x1
  0017b158: str.w r1,[r0,#0xd0]
  0017b15c: mov r0,r9
  0017b15e: blx 0x00071ae8
  0017b162: cmp r0,#0x8
  0017b164: bne.w 0x0017bbac
  0017b168: ldr r0,[0x0017b278]
  0017b16a: vmov.f32 s0,0x40000000
  0017b16e: vmov.f32 s16,0x3f800000
  0017b172: add r0,pc
  0017b174: ldr r0,[r0,#0x0]
  0017b176: ldrb.w r0,[r0,#0x38]
  0017b17a: cmp r0,#0x0
  0017b17c: it eq
  0017b17e: vmov.eq.f32 s16,s0
  0017b182: ldr.w r0,[r10,#0x0]
  0017b186: blx 0x00071a58
  0017b18a: mov r5,r0
  0017b18c: mov r0,r9
  0017b18e: blx 0x00074bfc
  0017b192: mov r1,r0
  0017b194: mov r0,r5
  0017b196: blx 0x00075e2c
  0017b19a: cmp r0,#0x0
  0017b19c: beq.w 0x0017cb44
  0017b1a0: ldr r0,[0x0017b27c]
  0017b1a2: add r0,pc
  0017b1a4: ldr r0,[r0,#0x0]
  0017b1a6: ldr r0,[r0,#0x0]
  0017b1a8: movw r1,#0x35a
  0017b1ac: blx 0x00072f70
  0017b1b0: mov r1,r0
  0017b1b2: sub.w r0,r7,#0x38
  0017b1b6: blx 0x0006f2b0
  0017b1ba: ldr r0,[0x0017b280]
  0017b1bc: ldr r1,[0x0017b284]
  0017b1be: add r0,pc
  0017b1c0: add r1,pc
  0017b1c2: ldr r0,[r0,#0x0]
  0017b1c4: ldr r1,[r1,#0x0]
  0017b1c6: ldr.w r8,[r0,#0x0]
  0017b1ca: ldr r0,[r1,#0x0]
  0017b1cc: movs r1,#0x2
  0017b1ce: blx 0x00071848
  0017b1d2: mov r5,r0
  0017b1d4: mov r0,r9
  0017b1d6: blx 0x00071bd8
  0017b1da: mov r6,r0
  0017b1dc: mov r0,r9
  0017b1de: blx 0x00074110
  0017b1e2: addw r1,r5,#0x30d
  0017b1e6: mov r2,r6
  0017b1e8: mov r3,r0
  0017b1ea: blx 0x00077d58
  0017b1ee: mov r1,r0
  0017b1f0: movs r0,#0x0
  0017b1f2: movs r2,#0x0
  0017b1f4: str r0,[sp,#0x0]
  0017b1f6: mov r0,r8
  0017b1f8: movs r3,#0x0
  0017b1fa: blx 0x00071548
  0017b1fe: ldr.w r0,[r10,#0x0]
  0017b202: ldr.w r1,[r0,#0xd0]
  0017b206: subs r1,#0x1
  0017b208: str.w r1,[r0,#0xd0]
  0017b20c: ldr.w r5,[r11,#0x60]
  0017b210: add r0,sp,#0x28c
  0017b212: mov r1,r9
  0017b214: blx 0x00071c2c
  0017b218: add r0,sp,#0x280
  0017b21a: sub.w r1,r7,#0x38
  0017b21e: movs r2,#0x0
  0017b220: blx 0x0006f028
  0017b224: add r1,sp,#0x28c
  0017b226: add r2,sp,#0x280
  0017b228: mov r0,r5
  0017b22a: blx 0x00075b2c
  0017b22e: add r0,sp,#0x280
  0017b230: blx 0x0006ee30
  0017b234: add r0,sp,#0x28c
  0017b236: b 0x0017adf8
  0017b288: mov r0,r9
  0017b28a: blx 0x00071ae8
  0017b28e: cmp r0,#0x0
  0017b290: beq.w 0x0017c8f4
  0017b294: ldr r0,[0x0017b4c8]
  0017b296: mov r2,r9
  0017b298: add r0,pc
  0017b29a: ldr r0,[r0,#0x0]
  0017b29c: ldr r1,[r0,#0x0]
  0017b29e: sub.w r0,r7,#0x38
  0017b2a2: blx 0x00077278
  0017b2a6: ldr.w r5,[r11,#0x60]
  0017b2aa: addw r0,sp,#0x6dc
  0017b2ae: mov r1,r9
  0017b2b0: blx 0x00071c2c
  0017b2b4: add.w r0,sp,#0x6d0
  0017b2b8: sub.w r1,r7,#0x38
  0017b2bc: movs r2,#0x0
  0017b2be: blx 0x0006f028
  0017b2c2: addw r1,sp,#0x6dc
  0017b2c6: add.w r2,sp,#0x6d0
  0017b2ca: mov r0,r5
  0017b2cc: blx 0x00075b2c
  0017b2d0: ldr r0,[0x0017b4cc]
  0017b2d2: add r0,pc
  0017b2d4: ldr r4,[r0,#0x0]
  0017b2d6: add.w r0,sp,#0x6d0
  0017b2da: blx r4
  0017b2dc: addw r0,sp,#0x6dc
  0017b2e0: blx r4
  0017b2e2: sub.w r0,r7,#0x38
  0017b2e6: blx r4
  0017b2e8: b.w 0x0017c85a
  0017b2ec: ldr r0,[0x0017b4d0]
  0017b2ee: add r0,pc
  0017b2f0: ldr r6,[r0,#0x0]
  0017b2f2: ldr r0,[r6,#0x0]
  0017b2f4: movs r1,#0x15
  0017b2f6: blx 0x00071848
  0017b2fa: mov r5,r0
  0017b2fc: ldr.w r0,[r11,#0x58]
  0017b300: ldrb r1,[r0,r5]
  0017b302: cmp r1,#0x0
  0017b304: bne 0x0017b2f2
  0017b306: movs r1,#0x1
  0017b308: strb r1,[r0,r5]
  0017b30a: mov r0,r9
  0017b30c: blx 0x00071bd8
  0017b310: mov r4,r5
  0017b312: cmp r0,#0x0
  0017b314: it ne
  0017b316: mov.ne r4,#0x4
  0017b318: cmp r5,#0x10
  0017b31a: it ne
  0017b31c: mov.ne r4,r5
  0017b31e: ldr r0,[sp,#0x18]
  0017b320: blx 0x00074110
  0017b324: ldr r1,[0x0017b4d4]
  0017b326: add.w r2,r4,#0x334
  0017b32a: cmp r0,#0x0
  0017b32c: add r1,pc
  0017b32e: ldr.w r9,[r1,#0x0]
  0017b332: mov r1,r2
  0017b334: it eq
  0017b336: mov.eq.w r1,#0x338
  0017b33a: cmp r4,#0xd
  0017b33c: it ne
  0017b33e: mov.ne r1,r2
  0017b340: ldr r0,[sp,#0x18]
  0017b342: str r1,[r0,#0x28]
  0017b344: ldr.w r0,[r9,#0x0]
  0017b348: blx 0x00072f70
  0017b34c: mov r1,r0
  0017b34e: sub.w r0,r7,#0x74
  0017b352: blx 0x0006f2b0
  0017b356: ldr r4,[sp,#0x10]
  0017b358: ldr r5,[r4,#0x0]
  0017b35a: add.w r0,sp,#0x5f8
  0017b35e: sub.w r1,r7,#0x74
  0017b362: movs r2,#0x0
  0017b364: blx 0x0006f028
  0017b368: ldr r0,[0x0017b4d8]
  0017b36a: add r0,pc
  0017b36c: ldr r0,[r0,#0x0]
  0017b36e: ldr r1,[r0,#0x0]
  0017b370: sub.w r0,r7,#0xb0
  0017b374: blx 0x00075e38
  0017b378: addw r0,sp,#0x5ec
  0017b37c: sub.w r1,r7,#0xb0
  0017b380: movs r2,#0x0
  0017b382: blx 0x0006f028
  0017b386: ldr r1,[0x0017b4dc]
  0017b388: add r1,pc
  0017b38a: add.w r0,sp,#0x5e0
  0017b38e: movs r2,#0x0
  0017b390: blx 0x0006ee18
  0017b394: str r0,[sp,#0x0]
  0017b396: sub.w r0,r7,#0x98
  0017b39a: add.w r2,sp,#0x5f8
  0017b39e: addw r3,sp,#0x5ec
  0017b3a2: mov r1,r5
  0017b3a4: blx 0x000739b4
  0017b3a8: sub.w r0,r7,#0x74
  0017b3ac: sub.w r1,r7,#0x98
  0017b3b0: blx 0x0006f2b0
  0017b3b4: sub.w r0,r7,#0x98
  0017b3b8: blx r10
  0017b3ba: add.w r0,sp,#0x5e0
  0017b3be: blx r10
  0017b3c0: addw r0,sp,#0x5ec
  0017b3c4: blx r10
  0017b3c6: sub.w r0,r7,#0xb0
  0017b3ca: blx r10
  0017b3cc: add.w r0,sp,#0x5f8
  0017b3d0: blx r10
  0017b3d2: ldr r5,[r4,#0x0]
  0017b3d4: addw r0,sp,#0x5d4
  0017b3d8: sub.w r1,r7,#0x74
  0017b3dc: movs r2,#0x0
  0017b3de: blx 0x0006f028
  0017b3e2: ldr r1,[sp,#0x18]
  0017b3e4: sub.w r0,r7,#0xb0
  0017b3e8: blx 0x00071c2c
  0017b3ec: add.w r0,sp,#0x5c8
  0017b3f0: sub.w r1,r7,#0xb0
  0017b3f4: movs r2,#0x0
  0017b3f6: blx 0x0006f028
  0017b3fa: ldr r1,[0x0017b4e0]
  0017b3fc: add r1,pc
  0017b3fe: addw r0,sp,#0x5bc
  0017b402: movs r2,#0x0
  0017b404: blx 0x0006ee18
  0017b408: str r0,[sp,#0x0]
  0017b40a: sub.w r0,r7,#0x98
  0017b40e: addw r2,sp,#0x5d4
  0017b412: add.w r3,sp,#0x5c8
  0017b416: mov r1,r5
  0017b418: blx 0x000739b4
  0017b41c: sub.w r0,r7,#0x74
  0017b420: sub.w r1,r7,#0x98
  0017b424: blx 0x0006f2b0
  0017b428: sub.w r0,r7,#0x98
  0017b42c: blx r10
  0017b42e: addw r0,sp,#0x5bc
  0017b432: blx r10
  0017b434: add.w r0,sp,#0x5c8
  0017b438: blx r10
  0017b43a: sub.w r0,r7,#0xb0
  0017b43e: blx r10
  0017b440: addw r0,sp,#0x5d4
  0017b444: blx r10
  0017b446: ldr.w r8,[r4,#0x0]
  0017b44a: add.w r0,sp,#0x5b0
  0017b44e: sub.w r1,r7,#0x74
  0017b452: movs r2,#0x0
  0017b454: blx 0x0006f028
  0017b458: ldr.w r5,[r9,#0x0]
  0017b45c: ldr r0,[r6,#0x0]
  0017b45e: movs r1,#0xa
  0017b460: blx 0x00071848
  0017b464: addw r1,r0,#0x594
  0017b468: mov r0,r5
  0017b46a: blx 0x00072f70
  0017b46e: mov r1,r0
  0017b470: addw r0,sp,#0x5a4
  0017b474: movs r2,#0x0
  0017b476: blx 0x0006f028
  0017b47a: ldr r1,[0x0017b4e4]
  0017b47c: add r1,pc
  0017b47e: add.w r0,sp,#0x598
  0017b482: movs r2,#0x0
  0017b484: blx 0x0006ee18
  0017b488: ldr.w r9,[sp,#0x18]
  0017b48c: str r0,[sp,#0x0]
  0017b48e: sub.w r0,r7,#0x98
  0017b492: add.w r2,sp,#0x5b0
  0017b496: addw r3,sp,#0x5a4
  0017b49a: mov r1,r8
  0017b49c: blx 0x000739b4
  0017b4a0: sub.w r0,r7,#0x74
  0017b4a4: sub.w r1,r7,#0x98
  0017b4a8: blx 0x0006f2b0
  0017b4ac: sub.w r0,r7,#0x98
  0017b4b0: blx r10
  0017b4b2: add.w r0,sp,#0x598
  0017b4b6: blx r10
  0017b4b8: addw r0,sp,#0x5a4
  0017b4bc: blx r10
  0017b4be: add.w r0,sp,#0x5b0
  0017b4c2: b.w 0x0017c4f0
  0017b4e8: ldr r0,[0x0017b7e8]
  0017b4ea: add r0,pc
  0017b4ec: ldr r4,[r0,#0x0]
  0017b4ee: ldr r0,[r4,#0x0]
  0017b4f0: movs r1,#0x5
  0017b4f2: blx 0x00071848
  0017b4f6: ldr r1,[sp,#0x18]
  0017b4f8: add.w r0,r0,#0x300
  0017b4fc: str r0,[r1,#0x28]
  0017b4fe: ldr r0,[r4,#0x0]
  0017b500: movs r1,#0x2
  0017b502: blx 0x00071848
  0017b506: ldr r1,[sp,#0x18]
  0017b508: addw r0,r0,#0x305
  0017b50c: str r0,[r1,#0x2c]
  0017b50e: ldr r0,[sp,#0x10]
  0017b510: ldr r0,[r0,#0x0]
  0017b512: blx 0x00071770
  0017b516: cmp r0,#0x18
  0017b518: bne 0x0017b53e
  0017b51a: ldr r0,[sp,#0x10]
  0017b51c: ldr r0,[r0,#0x0]
  0017b51e: blx 0x00071c14
  0017b522: blx 0x00071824
  0017b526: cmp r0,#0xa
  0017b528: bne 0x0017b53e
  0017b52a: ldr r0,[sp,#0x18]
  0017b52c: blx 0x00074bb4
  0017b530: cmp r0,#0x44
  0017b532: itttt eq
  0017b534: mov.eq r0,#0xc2
  0017b536: mov.eq r1,#0xc1
  0017b538: ldr.eq r2,[sp,#0x18]
  0017b53a: strd.eq r1,r0,[r2,#0x28]
  0017b53e: ldr r0,[0x0017b7ec]
  0017b540: add r0,pc
  0017b542: ldr.w r9,[r0,#0x0]
  0017b546: ldr r0,[sp,#0x18]
  0017b548: ldr r1,[r0,#0x28]
  0017b54a: ldr.w r0,[r9,#0x0]
  0017b54e: blx 0x00072f70
  0017b552: mov r1,r0
  0017b554: sub.w r0,r7,#0x74
  0017b558: blx 0x0006ef5c
  0017b55c: ldr r1,[0x0017b7f0]
  0017b55e: add r1,pc
  0017b560: sub.w r0,r7,#0xb0
  0017b564: movs r2,#0x0
  0017b566: blx 0x0006ee18
  0017b56a: ldr r4,[sp,#0x18]
  0017b56c: ldr.w r0,[r9,#0x0]
  0017b570: ldr r1,[r4,#0x2c]
  0017b572: blx 0x00072f70
  0017b576: mov r2,r0
  0017b578: sub.w r0,r7,#0x98
  0017b57c: sub.w r1,r7,#0xb0
  0017b580: blx 0x0006ef98
  0017b584: sub.w r0,r7,#0x74
  0017b588: sub.w r1,r7,#0x98
  0017b58c: blx 0x0006ef5c
  0017b590: sub.w r0,r7,#0x98
  0017b594: blx 0x0006ee30
  0017b598: sub.w r0,r7,#0xb0
  0017b59c: blx 0x0006ee30
  0017b5a0: ldr r0,[sp,#0x10]
  0017b5a2: ldr r6,[r0,#0x0]
  0017b5a4: addw r0,sp,#0x43c
  0017b5a8: sub.w r1,r7,#0x74
  0017b5ac: movs r2,#0x0
  0017b5ae: blx 0x0006f028
  0017b5b2: mov r0,r4
  0017b5b4: blx 0x00074bcc
  0017b5b8: mov r1,r0
  0017b5ba: sub.w r0,r7,#0xb0
  0017b5be: blx 0x0006f658
  0017b5c2: add.w r0,sp,#0x430
  0017b5c6: sub.w r1,r7,#0xb0
  0017b5ca: movs r2,#0x0
  0017b5cc: blx 0x0006f028
  0017b5d0: ldr r1,[0x0017b7f4]
  0017b5d2: add r1,pc
  0017b5d4: addw r0,sp,#0x424
  0017b5d8: movs r2,#0x0
  0017b5da: blx 0x0006ee18
  0017b5de: str r0,[sp,#0x0]
  0017b5e0: sub.w r0,r7,#0x98
  0017b5e4: addw r2,sp,#0x43c
  0017b5e8: add.w r3,sp,#0x430
  0017b5ec: mov r1,r6
  0017b5ee: blx 0x000739b4
  0017b5f2: sub.w r0,r7,#0x74
  0017b5f6: sub.w r1,r7,#0x98
  0017b5fa: blx 0x0006f2b0
  0017b5fe: sub.w r0,r7,#0x98
  0017b602: blx r10
  0017b604: addw r0,sp,#0x424
  0017b608: blx r10
  0017b60a: add.w r0,sp,#0x430
  0017b60e: blx r10
  0017b610: sub.w r0,r7,#0xb0
  0017b614: blx r10
  0017b616: addw r0,sp,#0x43c
  0017b61a: blx r10
  0017b61c: ldr r0,[sp,#0x10]
  0017b61e: ldr r6,[r0,#0x0]
  0017b620: add.w r0,sp,#0x418
  0017b624: sub.w r1,r7,#0x74
  0017b628: movs r2,#0x0
  0017b62a: blx 0x0006f028
  0017b62e: ldr.w r5,[r9,#0x0]
  0017b632: mov r0,r4
  0017b634: blx 0x00074bb4
  0017b638: addw r1,r0,#0x4fa
  0017b63c: mov r0,r5
  0017b63e: blx 0x00072f70
  0017b642: mov r1,r0
  0017b644: addw r0,sp,#0x40c
  0017b648: movs r2,#0x0
  0017b64a: blx 0x0006f028
  0017b64e: ldr r1,[0x0017b7f8]
  0017b650: add r1,pc
  0017b652: add.w r0,sp,#0x400
  0017b656: movs r2,#0x0
  0017b658: blx 0x0006ee18
  0017b65c: str r0,[sp,#0x0]
  0017b65e: sub.w r0,r7,#0x98
  0017b662: add.w r2,sp,#0x418
  0017b666: addw r3,sp,#0x40c
  0017b66a: mov r1,r6
  0017b66c: blx 0x000739b4
  0017b670: sub.w r0,r7,#0x74
  0017b674: sub.w r1,r7,#0x98
  0017b678: blx 0x0006f2b0
  0017b67c: sub.w r0,r7,#0x98
  0017b680: blx r10
  0017b682: add.w r0,sp,#0x400
  0017b686: blx r10
  0017b688: addw r0,sp,#0x40c
  0017b68c: blx r10
  0017b68e: add.w r0,sp,#0x418
  0017b692: blx r10
  0017b694: ldr r0,[sp,#0x10]
  0017b696: ldr r6,[r0,#0x0]
  0017b698: add r0,sp,#0x3f4
  0017b69a: sub.w r1,r7,#0x74
  0017b69e: movs r2,#0x0
  0017b6a0: blx 0x0006f028
  0017b6a4: ldr r0,[0x0017b7fc]
  0017b6a6: add r0,pc
  0017b6a8: ldr.w r8,[r0,#0x0]
  0017b6ac: ldr.w r5,[r8,#0x0]
  0017b6b0: mov r0,r4
  0017b6b2: blx 0x00074bc0
  0017b6b6: mov r2,r0
  0017b6b8: sub.w r0,r7,#0xb0
  0017b6bc: mov r1,r5
  0017b6be: blx 0x00074e54
  0017b6c2: add r0,sp,#0x3e8
  0017b6c4: sub.w r1,r7,#0xb0
  0017b6c8: movs r2,#0x0
  0017b6ca: blx 0x0006f028
  0017b6ce: ldr r1,[0x0017b800]
  0017b6d0: add r1,pc
  0017b6d2: add r0,sp,#0x3dc
  0017b6d4: movs r2,#0x0
  0017b6d6: blx 0x0006ee18
  0017b6da: str r0,[sp,#0x0]
  0017b6dc: sub.w r0,r7,#0x98
  0017b6e0: add r2,sp,#0x3f4
  0017b6e2: add r3,sp,#0x3e8
  0017b6e4: mov r1,r6
  0017b6e6: blx 0x000739b4
  0017b6ea: sub.w r0,r7,#0x74
  0017b6ee: sub.w r1,r7,#0x98
  0017b6f2: blx 0x0006f2b0
  0017b6f6: sub.w r0,r7,#0x98
  0017b6fa: blx r10
  0017b6fc: add r0,sp,#0x3dc
  0017b6fe: blx r10
  0017b700: add r0,sp,#0x3e8
  0017b702: blx r10
  0017b704: sub.w r0,r7,#0xb0
  0017b708: blx r10
  0017b70a: add r0,sp,#0x3f4
  0017b70c: blx r10
  0017b70e: mov r0,r4
  0017b710: blx 0x00074bcc
  0017b714: mov r4,r9
  0017b716: ldr.w r9,[sp,#0x18]
  0017b71a: cmp r0,#0x2
  0017b71c: blt.w 0x0017c4f2
  0017b720: ldr r1,[0x0017b804]
  0017b722: add r1,pc
  0017b724: sub.w r0,r7,#0xb0
  0017b728: movs r2,#0x0
  0017b72a: blx 0x0006ee18
  0017b72e: ldr r0,[r4,#0x0]
  0017b730: movw r1,#0x307
  0017b734: blx 0x00072f70
  0017b738: mov r2,r0
  0017b73a: sub.w r0,r7,#0x98
  0017b73e: sub.w r1,r7,#0xb0
  0017b742: blx 0x0006ef98
  0017b746: sub.w r0,r7,#0x74
  0017b74a: sub.w r1,r7,#0x98
  0017b74e: blx 0x0006ef5c
  0017b752: ldr r4,[sp,#0x10]
  0017b754: sub.w r0,r7,#0x98
  0017b758: blx 0x0006ee30
  0017b75c: sub.w r0,r7,#0xb0
  0017b760: blx 0x0006ee30
  0017b764: ldr r4,[r4,#0x0]
  0017b766: add r0,sp,#0x3d0
  0017b768: sub.w r1,r7,#0x74
  0017b76c: movs r2,#0x0
  0017b76e: blx 0x0006f028
  0017b772: ldr.w r5,[r8,#0x0]
  0017b776: mov r0,r9
  0017b778: blx 0x00074bc0
  0017b77c: mov r6,r0
  0017b77e: mov r0,r9
  0017b780: blx 0x00074bcc
  0017b784: mov r1,r0
  0017b786: mov r0,r6
  0017b788: blx 0x0007198c
  0017b78c: mov r2,r0
  0017b78e: sub.w r0,r7,#0xb0
  0017b792: mov r1,r5
  0017b794: blx 0x00074e54
  0017b798: add r0,sp,#0x3c4
  0017b79a: sub.w r1,r7,#0xb0
  0017b79e: movs r2,#0x0
  0017b7a0: blx 0x0006f028
  0017b7a4: ldr r1,[0x0017b808]
  0017b7a6: add r1,pc
  0017b7a8: add r0,sp,#0x3b8
  0017b7aa: movs r2,#0x0
  0017b7ac: blx 0x0006ee18
  0017b7b0: str r0,[sp,#0x0]
  0017b7b2: sub.w r0,r7,#0x98
  0017b7b6: add r2,sp,#0x3d0
  0017b7b8: add r3,sp,#0x3c4
  0017b7ba: mov r1,r4
  0017b7bc: blx 0x000739b4
  0017b7c0: sub.w r0,r7,#0x74
  0017b7c4: sub.w r1,r7,#0x98
  0017b7c8: blx 0x0006f2b0
  0017b7cc: sub.w r0,r7,#0x98
  0017b7d0: blx r10
  0017b7d2: add r0,sp,#0x3b8
  0017b7d4: blx r10
  0017b7d6: add r0,sp,#0x3c4
  0017b7d8: blx r10
  0017b7da: sub.w r0,r7,#0xb0
  0017b7de: blx r10
  0017b7e0: add r0,sp,#0x3d0
  0017b7e2: b.w 0x0017c4f0
  0017b80c: ldr r0,[0x0017bb74]
  0017b80e: add r0,pc
  0017b810: ldr r0,[r0,#0x0]
  0017b812: ldr r0,[r0,#0x0]
  0017b814: blx 0x00071884
  0017b818: mov r2,r0
  0017b81a: ldr r0,[sp,#0xc]
  0017b81c: mov r1,r9
  0017b81e: blx 0x00071b6c
  0017b822: mov r1,r0
  0017b824: mov r0,r9
  0017b826: blx 0x00071b78
  0017b82a: ldr r0,[0x0017bb78]
  0017b82c: add r0,pc
  0017b82e: ldr r0,[r0,#0x0]
  0017b830: ldr r0,[r0,#0x0]
  0017b832: movs r1,#0x2
  0017b834: blx 0x00071848
  0017b838: ldr r1,[0x0017bb7c]
  0017b83a: add r1,pc
  0017b83c: ldr r4,[r1,#0x0]
  0017b83e: addw r1,r0,#0x309
  0017b842: str.w r1,[r9,#0x28]
  0017b846: ldr r0,[r4,#0x0]
  0017b848: blx 0x00072f70
  0017b84c: mov r1,r0
  0017b84e: sub.w r0,r7,#0x74
  0017b852: blx 0x0006ef5c
  0017b856: ldr r0,[sp,#0x10]
  0017b858: ldr r6,[r0,#0x0]
  0017b85a: addw r0,sp,#0x664
  0017b85e: sub.w r1,r7,#0x74
  0017b862: movs r2,#0x0
  0017b864: blx 0x0006f028
  0017b868: mov r0,r9
  0017b86a: blx 0x00071bb4
  0017b86e: blx 0x00072bb0
  0017b872: mov r1,r0
  0017b874: sub.w r0,r7,#0xb0
  0017b878: blx 0x0006f658
  0017b87c: add.w r0,sp,#0x658
  0017b880: sub.w r1,r7,#0xb0
  0017b884: movs r2,#0x0
  0017b886: blx 0x0006f028
  0017b88a: ldr r1,[0x0017bb80]
  0017b88c: add r1,pc
  0017b88e: addw r0,sp,#0x64c
  0017b892: movs r2,#0x0
  0017b894: blx 0x0006ee18
  0017b898: str r0,[sp,#0x0]
  0017b89a: sub.w r0,r7,#0x98
  0017b89e: addw r2,sp,#0x664
  0017b8a2: add.w r3,sp,#0x658
  0017b8a6: mov r1,r6
  0017b8a8: blx 0x000739b4
  0017b8ac: sub.w r0,r7,#0x74
  0017b8b0: sub.w r1,r7,#0x98
  0017b8b4: blx 0x0006f2b0
  0017b8b8: sub.w r0,r7,#0x98
  0017b8bc: blx r10
  0017b8be: addw r0,sp,#0x64c
  0017b8c2: blx r10
  0017b8c4: add.w r0,sp,#0x658
  0017b8c8: blx r10
  0017b8ca: sub.w r0,r7,#0xb0
  0017b8ce: blx r10
  0017b8d0: addw r0,sp,#0x664
  0017b8d4: blx r10
  0017b8d6: ldr r0,[sp,#0x10]
  0017b8d8: ldr r6,[r0,#0x0]
  0017b8da: add.w r0,sp,#0x640
  0017b8de: sub.w r1,r7,#0x74
  0017b8e2: movs r2,#0x0
  0017b8e4: blx 0x0006f028
  0017b8e8: ldr r5,[r4,#0x0]
  0017b8ea: mov r0,r9
  0017b8ec: blx 0x00071bb4
  0017b8f0: blx 0x000728a4
  0017b8f4: addw r1,r0,#0x4fa
  0017b8f8: mov r0,r5
  0017b8fa: blx 0x00072f70
  0017b8fe: mov r1,r0
  0017b900: addw r0,sp,#0x634
  0017b904: movs r2,#0x0
  0017b906: blx 0x0006f028
  0017b90a: ldr r1,[0x0017bb84]
  0017b90c: add r1,pc
  0017b90e: add.w r0,sp,#0x628
  0017b912: movs r2,#0x0
  0017b914: blx 0x0006ee18
  0017b918: ldr r4,[sp,#0x10]
  0017b91a: str r0,[sp,#0x0]
  0017b91c: sub.w r0,r7,#0x98
  0017b920: add.w r2,sp,#0x640
  0017b924: addw r3,sp,#0x634
  0017b928: mov r1,r6
  0017b92a: blx 0x000739b4
  0017b92e: sub.w r0,r7,#0x74
  0017b932: sub.w r1,r7,#0x98
  0017b936: blx 0x0006f2b0
  0017b93a: sub.w r0,r7,#0x98
  0017b93e: blx r10
  0017b940: add.w r0,sp,#0x628
  0017b944: blx r10
  0017b946: addw r0,sp,#0x634
  0017b94a: blx r10
  0017b94c: add.w r0,sp,#0x640
  0017b950: blx r10
  0017b952: ldr r6,[r4,#0x0]
  0017b954: addw r0,sp,#0x61c
  0017b958: sub.w r1,r7,#0x74
  0017b95c: movs r2,#0x0
  0017b95e: blx 0x0006f028
  0017b962: ldr r0,[0x0017bb88]
  0017b964: add r0,pc
  0017b966: ldr r0,[r0,#0x0]
  0017b968: ldr r5,[r0,#0x0]
  0017b96a: mov r0,r9
  0017b96c: blx 0x00071bb4
  0017b970: blx 0x00071bc0
  0017b974: mov r2,r0
  0017b976: sub.w r0,r7,#0xb0
  0017b97a: mov r1,r5
  0017b97c: blx 0x00074e54
  0017b980: add.w r0,sp,#0x610
  0017b984: sub.w r1,r7,#0xb0
  0017b988: movs r2,#0x0
  0017b98a: blx 0x0006f028
  0017b98e: ldr r1,[0x0017bb8c]
  0017b990: add r1,pc
  0017b992: addw r0,sp,#0x604
  0017b996: movs r2,#0x0
  0017b998: blx 0x0006ee18
  0017b99c: str r0,[sp,#0x0]
  0017b99e: sub.w r0,r7,#0x98
  0017b9a2: addw r2,sp,#0x61c
  0017b9a6: add.w r3,sp,#0x610
  0017b9aa: mov r1,r6
  0017b9ac: blx 0x000739b4
  0017b9b0: sub.w r0,r7,#0x74
  0017b9b4: sub.w r1,r7,#0x98
  0017b9b8: blx 0x0006f2b0
  0017b9bc: sub.w r0,r7,#0x98
  0017b9c0: blx r10
  0017b9c2: addw r0,sp,#0x604
  0017b9c6: blx r10
  0017b9c8: add.w r0,sp,#0x610
  0017b9cc: blx r10
  0017b9ce: sub.w r0,r7,#0xb0
  0017b9d2: blx r10
  0017b9d4: addw r0,sp,#0x61c
  0017b9d8: b.w 0x0017c4f0
  0017b9dc: ldr r0,[0x0017bb90]
  0017b9de: ldr r1,[0x0017bb94]
  0017b9e0: add r0,pc
  0017b9e2: add r1,pc
  0017b9e4: ldr r0,[r0,#0x0]
  0017b9e6: ldr r1,[r1,#0x0]
  0017b9e8: ldr r5,[r0,#0x0]
  0017b9ea: ldr r0,[r1,#0x0]
  0017b9ec: blx 0x00075580
  0017b9f0: mov r6,r0
  0017b9f2: ldr r0,[sp,#0x18]
  0017b9f4: blx 0x00074bd8
  0017b9f8: movw r1,#0x30b
  0017b9fc: cmp r6,#0x0
  0017b9fe: it ne
  0017ba00: movw.ne r1,#0x30e
  0017ba04: add r1,r0
  0017ba06: mov r0,r5
  0017ba08: blx 0x00072f70
  0017ba0c: mov r1,r0
  0017ba0e: sub.w r0,r7,#0x74
  0017ba12: blx 0x0006ef5c
  0017ba16: ldr r0,[sp,#0x10]
  0017ba18: ldr r6,[r0,#0x0]
  0017ba1a: add r0,sp,#0x3ac
  0017ba1c: sub.w r1,r7,#0x74
  0017ba20: movs r2,#0x0
  0017ba22: blx 0x0006f028
  0017ba26: ldr r0,[0x0017bb98]
  0017ba28: add r0,pc
  0017ba2a: ldr r0,[r0,#0x0]
  0017ba2c: ldr r5,[r0,#0x0]
  0017ba2e: ldr r4,[sp,#0x18]
  0017ba30: mov r0,r4
  0017ba32: blx 0x00071bfc
  0017ba36: mov r2,r0
  0017ba38: sub.w r0,r7,#0xb0
  0017ba3c: mov r1,r5
  0017ba3e: blx 0x00074e54
  0017ba42: add r0,sp,#0x3a0
  0017ba44: sub.w r1,r7,#0xb0
  0017ba48: movs r2,#0x0
  0017ba4a: blx 0x0006f028
  0017ba4e: ldr r1,[0x0017bb9c]
  0017ba50: add r1,pc
  0017ba52: add r0,sp,#0x394
  0017ba54: movs r2,#0x0
  0017ba56: blx 0x0006ee18
  0017ba5a: str r0,[sp,#0x0]
  0017ba5c: sub.w r0,r7,#0x98
  0017ba60: add r2,sp,#0x3ac
  0017ba62: add r3,sp,#0x3a0
  0017ba64: mov r1,r6
  0017ba66: blx 0x000739b4
  0017ba6a: sub.w r0,r7,#0x74
  0017ba6e: sub.w r1,r7,#0x98
  0017ba72: blx 0x0006f2b0
  0017ba76: sub.w r0,r7,#0x98
  0017ba7a: blx r10
  0017ba7c: add r0,sp,#0x394
  0017ba7e: blx r10
  0017ba80: add r0,sp,#0x3a0
  0017ba82: blx r10
  0017ba84: sub.w r0,r7,#0xb0
  0017ba88: blx r10
  0017ba8a: add r0,sp,#0x3ac
  0017ba8c: blx r10
  0017ba8e: mov r0,r4
  0017ba90: blx 0x00074bd8
  0017ba94: ldr.w r9,[sp,#0x18]
  0017ba98: cmp r0,#0x1
  0017ba9a: blt.w 0x0017c4f2
  0017ba9e: ldr r0,[sp,#0x10]
  0017baa0: ldr r6,[r0,#0x0]
  0017baa2: add r0,sp,#0x388
  0017baa4: sub.w r1,r7,#0x74
  0017baa8: movs r2,#0x0
  0017baaa: blx 0x0006f028
  0017baae: sub.w r0,r7,#0xb0
  0017bab2: mov r1,r9
  0017bab4: movs r2,#0x1
  0017bab6: blx 0x00075e44
  0017baba: add r0,sp,#0x37c
  0017babc: sub.w r1,r7,#0xb0
  0017bac0: movs r2,#0x0
  0017bac2: blx 0x0006f028
  0017bac6: ldr r1,[0x0017bba0]
  0017bac8: add r1,pc
  0017baca: add r0,sp,#0x370
  0017bacc: movs r2,#0x0
  0017bace: blx 0x0006ee18
  0017bad2: str r0,[sp,#0x0]
  0017bad4: sub.w r0,r7,#0x98
  0017bad8: add r2,sp,#0x388
  0017bada: add r3,sp,#0x37c
  0017badc: mov r1,r6
  0017bade: blx 0x000739b4
  0017bae2: sub.w r0,r7,#0x74
  0017bae6: sub.w r1,r7,#0x98
  0017baea: blx 0x0006f2b0
  0017baee: sub.w r0,r7,#0x98
  0017baf2: blx r10
  0017baf4: add r0,sp,#0x370
  0017baf6: blx r10
  0017baf8: add r0,sp,#0x37c
  0017bafa: blx r10
  0017bafc: sub.w r0,r7,#0xb0
  0017bb00: blx r10
  0017bb02: add r0,sp,#0x388
  0017bb04: b.w 0x0017c4f0
  0017bb08: mov r0,r9
  0017bb0a: blx 0x00071bd8
  0017bb0e: ldr r4,[sp,#0x10]
  0017bb10: mov r6,r0
  0017bb12: ldr r0,[r4,#0x0]
  0017bb14: blx 0x00071b84
  0017bb18: orr r1,r6,#0x1
  0017bb1c: subs r1,#0x3
  0017bb1e: clz r1,r1
  0017bb22: lsrs r1,r1,#0x5
  0017bb24: blx 0x00074fb0
  0017bb28: mov r5,r0
  0017bb2a: ldr r0,[r4,#0x0]
  0017bb2c: blx 0x00071b84
  0017bb30: mov r1,r6
  0017bb32: blx 0x00071b90
  0017bb36: cmp r0,#0x0
  0017bb38: beq.w 0x0017bf1e
  0017bb3c: cmp r5,#0x0
  0017bb3e: bpl 0x0017bb42
  0017bb40: rsbs r5,r5
  0017bb42: vmov s0,r5
  0017bb46: vldr.32 s2,[pc,#0x5c]
  0017bb4a: cmp r6,#0x2
  0017bb4c: vcvt.f32.s32 s0,s0
  0017bb50: vdiv.f32 s0,s0,s2
  0017bb54: vldr.32 s2,[pc,#0x50]
  0017bb58: vmul.f32 s0,s0,s2
  0017bb5c: vcvt.s32.f32 s16,s0
  0017bb60: beq.w 0x0017d020
  0017bb64: cmp r6,#0x3
  0017bb66: bne.w 0x0017d026
  0017bb6a: mov.w r1,#0x370
  0017bb6e: b.w 0x0017d032
  0017bbac: mov r0,r9
  0017bbae: blx 0x00071ae8
  0017bbb2: cmp r0,#0x9
  0017bbb4: bne.w 0x0017ca42
  0017bbb8: ldr r0,[0x0017bf98]
  0017bbba: vmov.f32 s0,0x40000000
  0017bbbe: vmov.f32 s16,0x3f800000
  0017bbc2: add r0,pc
  0017bbc4: ldr r0,[r0,#0x0]
  0017bbc6: ldrb.w r0,[r0,#0x38]
  0017bbca: cmp r0,#0x0
  0017bbcc: it eq
  0017bbce: vmov.eq.f32 s16,s0
  0017bbd2: ldr.w r1,[r9,#0x28]
  0017bbd6: adds r0,r1,#0x1
  0017bbd8: bne 0x0017bbec
  0017bbda: ldr r0,[0x0017bf9c]
  0017bbdc: add r0,pc
  0017bbde: ldr r0,[r0,#0x0]
  0017bbe0: ldr r0,[r0,#0x0]
  0017bbe2: movs r1,#0x2
  0017bbe4: blx 0x00071848
  0017bbe8: addw r1,r0,#0x36a
  0017bbec: ldr r0,[0x0017bfa0]
  0017bbee: str.w r1,[r9,#0x28]
  0017bbf2: add r0,pc
  0017bbf4: ldr.w r8,[r0,#0x0]
  0017bbf8: ldr.w r0,[r8,#0x0]
  0017bbfc: blx 0x00072f70
  0017bc00: mov r1,r0
  0017bc02: sub.w r0,r7,#0x38
  0017bc06: blx 0x0006f2b0
  0017bc0a: ldr r1,[0x0017bfa4]
  0017bc0c: add r1,pc
  0017bc0e: sub.w r0,r7,#0x68
  0017bc12: movs r2,#0x0
  0017bc14: blx 0x0006ee18
  0017bc18: ldr.w r5,[r8,#0x0]
  0017bc1c: mov r0,r9
  0017bc1e: blx 0x00074ba8
  0017bc22: addw r1,r0,#0x376
  0017bc26: mov r0,r5
  0017bc28: blx 0x00072f70
  0017bc2c: mov r2,r0
  0017bc2e: sub.w r0,r7,#0x5c
  0017bc32: sub.w r1,r7,#0x68
  0017bc36: blx 0x0006ef98
  0017bc3a: sub.w r0,r7,#0x38
  0017bc3e: sub.w r1,r7,#0x5c
  0017bc42: blx 0x0006ef5c
  0017bc46: sub.w r0,r7,#0x5c
  0017bc4a: blx 0x0006ee30
  0017bc4e: sub.w r0,r7,#0x68
  0017bc52: blx 0x0006ee30
  0017bc56: ldr.w r5,[r9,#0x2c]
  0017bc5a: adds r0,r5,#0x1
  0017bc5c: bne 0x0017bc74
  0017bc5e: ldr r0,[0x0017bfa8]
  0017bc60: add r0,pc
  0017bc62: ldr r0,[r0,#0x0]
  0017bc64: ldr r0,[r0,#0x0]
  0017bc66: movs r1,#0x2
  0017bc68: blx 0x00071848
  0017bc6c: ldr.w r9,[sp,#0x18]
  0017bc70: addw r5,r0,#0x305
  0017bc74: ldr r1,[0x0017bfac]
  0017bc76: str.w r5,[r9,#0x2c]
  0017bc7a: add r1,pc
  0017bc7c: sub.w r0,r7,#0x68
  0017bc80: movs r2,#0x0
  0017bc82: blx 0x0006ee18
  0017bc86: ldr.w r0,[r8,#0x0]
  0017bc8a: mov r1,r5
  0017bc8c: blx 0x00072f70
  0017bc90: mov r2,r0
  0017bc92: sub.w r0,r7,#0x5c
  0017bc96: sub.w r1,r7,#0x68
  0017bc9a: blx 0x0006ef98
  0017bc9e: sub.w r0,r7,#0x38
  0017bca2: sub.w r1,r7,#0x5c
  0017bca6: blx 0x0006ef5c
  0017bcaa: sub.w r0,r7,#0x5c
  0017bcae: blx 0x0006ee30
  0017bcb2: sub.w r0,r7,#0x68
  0017bcb6: blx 0x0006ee30
  0017bcba: ldr.w r6,[r10,#0x0]
  0017bcbe: add r0,sp,#0x208
  0017bcc0: sub.w r1,r7,#0x38
  0017bcc4: movs r2,#0x0
  0017bcc6: blx 0x0006f028
  0017bcca: mov r0,r9
  0017bccc: blx 0x00074bcc
  0017bcd0: mov r1,r0
  0017bcd2: sub.w r0,r7,#0x68
  0017bcd6: blx 0x0006f658
  0017bcda: add r0,sp,#0x1fc
  0017bcdc: sub.w r1,r7,#0x68
  0017bce0: movs r2,#0x0
  0017bce2: blx 0x0006f028
  0017bce6: ldr r1,[0x0017bfb0]
  0017bce8: add r1,pc
  0017bcea: add r0,sp,#0x1f0
  0017bcec: movs r2,#0x0
  0017bcee: blx 0x0006ee18
  0017bcf2: str r0,[sp,#0x0]
  0017bcf4: sub.w r0,r7,#0x5c
  0017bcf8: add r2,sp,#0x208
  0017bcfa: add r3,sp,#0x1fc
  0017bcfc: mov r1,r6
  0017bcfe: blx 0x000739b4
  0017bd02: sub.w r0,r7,#0x38
  0017bd06: sub.w r1,r7,#0x5c
  0017bd0a: blx 0x0006f2b0
  0017bd0e: ldr r0,[0x0017bfb4]
  0017bd10: add r0,pc
  0017bd12: ldr r6,[r0,#0x0]
  0017bd14: sub.w r0,r7,#0x5c
  0017bd18: blx r6
  0017bd1a: add r0,sp,#0x1f0
  0017bd1c: blx r6
  0017bd1e: add r0,sp,#0x1fc
  0017bd20: blx r6
  0017bd22: sub.w r0,r7,#0x68
  0017bd26: blx r6
  0017bd28: add r0,sp,#0x208
  0017bd2a: blx r6
  0017bd2c: mov r4,r10
  0017bd2e: ldr.w r10,[r10,#0x0]
  0017bd32: add r0,sp,#0x1e4
  0017bd34: sub.w r1,r7,#0x38
  0017bd38: movs r2,#0x0
  0017bd3a: blx 0x0006f028
  0017bd3e: ldr.w r5,[r8,#0x0]
  0017bd42: mov r0,r9
  0017bd44: blx 0x00074bb4
  0017bd48: addw r1,r0,#0x4fa
  0017bd4c: mov r0,r5
  0017bd4e: blx 0x00072f70
  0017bd52: mov r1,r0
  0017bd54: add r0,sp,#0x1d8
  0017bd56: movs r2,#0x0
  0017bd58: blx 0x0006f028
  0017bd5c: ldr r1,[0x0017bfb8]
  0017bd5e: add r1,pc
  0017bd60: add r0,sp,#0x1cc
  0017bd62: movs r2,#0x0
  0017bd64: blx 0x0006ee18
  0017bd68: str r0,[sp,#0x0]
  0017bd6a: sub.w r0,r7,#0x5c
  0017bd6e: add r2,sp,#0x1e4
  0017bd70: add r3,sp,#0x1d8
  0017bd72: mov r1,r10
  0017bd74: blx 0x000739b4
  0017bd78: sub.w r0,r7,#0x38
  0017bd7c: sub.w r1,r7,#0x5c
  0017bd80: blx 0x0006f2b0
  0017bd84: sub.w r0,r7,#0x5c
  0017bd88: mov r5,r4
  0017bd8a: blx r6
  0017bd8c: add r0,sp,#0x1cc
  0017bd8e: blx r6
  0017bd90: add r0,sp,#0x1d8
  0017bd92: blx r6
  0017bd94: add r0,sp,#0x1e4
  0017bd96: blx r6
  0017bd98: ldr.w r10,[r4,#0x0]
  0017bd9c: add r0,sp,#0x1c0
  0017bd9e: sub.w r1,r7,#0x38
  0017bda2: movs r2,#0x0
  0017bda4: blx 0x0006f028
  0017bda8: ldr r0,[0x0017bfbc]
  0017bdaa: add r0,pc
  0017bdac: ldr.w r9,[r0,#0x0]
  0017bdb0: ldr.w r5,[r9,#0x0]
  0017bdb4: ldr r0,[sp,#0x18]
  0017bdb6: blx 0x00074bc0
  0017bdba: vmov s0,r0
  0017bdbe: vcvt.f32.s32 s0,s0
  0017bdc2: vmul.f32 s0,s16,s0
  0017bdc6: vcvt.s32.f32 s0,s0
  0017bdca: vmov r2,s0
  0017bdce: sub.w r0,r7,#0x68
  0017bdd2: mov r1,r5
  0017bdd4: blx 0x00074e54
  0017bdd8: add r0,sp,#0x1b4
  0017bdda: sub.w r1,r7,#0x68
  0017bdde: movs r2,#0x0
  0017bde0: blx 0x0006f028
  0017bde4: ldr r1,[0x0017bfc0]
  0017bde6: add r1,pc
  0017bde8: add r0,sp,#0x1a8
  0017bdea: movs r2,#0x0
  0017bdec: blx 0x0006ee18
  0017bdf0: str r0,[sp,#0x0]
  0017bdf2: sub.w r0,r7,#0x5c
  0017bdf6: add r2,sp,#0x1c0
  0017bdf8: add r3,sp,#0x1b4
  0017bdfa: mov r1,r10
  0017bdfc: blx 0x000739b4
  0017be00: sub.w r0,r7,#0x38
  0017be04: sub.w r1,r7,#0x5c
  0017be08: blx 0x0006f2b0
  0017be0c: sub.w r0,r7,#0x5c
  0017be10: blx r6
  0017be12: add r0,sp,#0x1a8
  0017be14: blx r6
  0017be16: add r0,sp,#0x1b4
  0017be18: blx r6
  0017be1a: sub.w r0,r7,#0x68
  0017be1e: blx r6
  0017be20: add r0,sp,#0x1c0
  0017be22: blx r6
  0017be24: ldr r0,[sp,#0x18]
  0017be26: blx 0x00074bcc
  0017be2a: cmp r0,#0x2
  0017be2c: blt.w 0x0017d53c
  0017be30: ldr r1,[0x0017bfc4]
  0017be32: add r1,pc
  0017be34: sub.w r0,r7,#0x68
  0017be38: movs r2,#0x0
  0017be3a: blx 0x0006ee18
  0017be3e: ldr.w r0,[r8,#0x0]
  0017be42: movw r1,#0x307
  0017be46: blx 0x00072f70
  0017be4a: mov r2,r0
  0017be4c: sub.w r0,r7,#0x5c
  0017be50: sub.w r1,r7,#0x68
  0017be54: blx 0x0006ef98
  0017be58: sub.w r0,r7,#0x38
  0017be5c: sub.w r1,r7,#0x5c
  0017be60: blx 0x0006ef5c
  0017be64: sub.w r0,r7,#0x5c
  0017be68: blx 0x0006ee30
  0017be6c: sub.w r0,r7,#0x68
  0017be70: blx 0x0006ee30
  0017be74: ldr.w r8,[r4,#0x0]
  0017be78: add r0,sp,#0x19c
  0017be7a: sub.w r1,r7,#0x38
  0017be7e: movs r2,#0x0
  0017be80: blx 0x0006f028
  0017be84: ldr.w r9,[r9,#0x0]
  0017be88: ldr r0,[sp,#0x18]
  0017be8a: blx 0x00074bc0
  0017be8e: mov r5,r0
  0017be90: ldr r0,[sp,#0x18]
  0017be92: blx 0x00074bcc
  0017be96: mov r1,r0
  0017be98: mov r0,r5
  0017be9a: blx 0x0007198c
  0017be9e: vmov s0,r0
  0017bea2: vcvt.f32.s32 s0,s0
  0017bea6: vmul.f32 s0,s16,s0
  0017beaa: vcvt.s32.f32 s0,s0
  0017beae: vmov r2,s0
  0017beb2: sub.w r0,r7,#0x68
  0017beb6: mov r1,r9
  0017beb8: blx 0x00074e54
  0017bebc: add r0,sp,#0x190
  0017bebe: sub.w r1,r7,#0x68
  0017bec2: movs r2,#0x0
  0017bec4: blx 0x0006f028
  0017bec8: ldr r1,[0x0017bfc8]
  0017beca: add r1,pc
  0017becc: add r0,sp,#0x184
  0017bece: movs r2,#0x0
  0017bed0: blx 0x0006ee18
  0017bed4: str r0,[sp,#0x0]
  0017bed6: sub.w r0,r7,#0x5c
  0017beda: add r2,sp,#0x19c
  0017bedc: add r3,sp,#0x190
  0017bede: mov r1,r8
  0017bee0: blx 0x000739b4
  0017bee4: sub.w r0,r7,#0x38
  0017bee8: sub.w r1,r7,#0x5c
  0017beec: blx 0x0006f2b0
  0017bef0: sub.w r0,r7,#0x5c
  0017bef4: blx r6
  0017bef6: add r0,sp,#0x184
  0017bef8: blx r6
  0017befa: add r0,sp,#0x190
  0017befc: blx r6
  0017befe: sub.w r0,r7,#0x68
  0017bf02: blx r6
  0017bf04: add r0,sp,#0x19c
  0017bf06: b.w 0x0017d366
  0017bf0a: mov r0,r9
  0017bf0c: blx 0x00071ae8
  0017bf10: cmp r0,#0x6
  0017bf12: bne.w 0x0017cd92
  0017bf16: movw r5,#0x35b
  0017bf1a: b.w 0x0017cdb0
  0017bf1e: ldr r0,[0x0017bfcc]
  0017bf20: add r0,pc
  0017bf22: ldr r0,[r0,#0x0]
  0017bf24: ldr r0,[r0,#0x0]
  0017bf26: movw r1,#0x373
  0017bf2a: blx 0x00072f70
  0017bf2e: mov r1,r0
  0017bf30: sub.w r0,r7,#0x74
  0017bf34: blx 0x0006f2b0
  0017bf38: ldr.w r5,[r11,#0x60]
  0017bf3c: addw r0,sp,#0x67c
  0017bf40: mov r1,r9
  0017bf42: blx 0x00071c2c
  0017bf46: add.w r0,sp,#0x670
  0017bf4a: sub.w r1,r7,#0x74
  0017bf4e: movs r2,#0x0
  0017bf50: blx 0x0006f028
  0017bf54: addw r1,sp,#0x67c
  0017bf58: add.w r2,sp,#0x670
  0017bf5c: mov r0,r5
  0017bf5e: blx 0x00075b2c
  0017bf62: add.w r0,sp,#0x670
  0017bf66: blx r10
  0017bf68: addw r0,sp,#0x67c
  0017bf6c: blx r10
  0017bf6e: movs r0,#0x3
  0017bf70: str.w r0,[r11,#0x14]
  0017bf74: movs r0,#0x0
  0017bf76: str.w r0,[r11,#0x2c]
  0017bf7a: sub.w r0,r7,#0x74
  0017bf7e: blx r10
  0017bf80: sub.w r0,r7,#0x68
  0017bf84: blx r10
  0017bf86: sub.w r0,r7,#0x5c
  0017bf8a: blx r10
  0017bf8c: sub.w r0,r7,#0x38
  0017bf90: blx r10
  0017bf92: b.w 0x0017ae10
  0017bfd0: ldr r0,[sp,#0x10]
  0017bfd2: ldr r6,[r0,#0x0]
  0017bfd4: add.w r0,sp,#0x568
  0017bfd8: sub.w r1,r7,#0x74
  0017bfdc: movs r2,#0x0
  0017bfde: blx 0x0006f028
  0017bfe2: ldr.w r5,[r8,#0x0]
  0017bfe6: mov r0,r4
  0017bfe8: blx 0x000728a4
  0017bfec: addw r1,r0,#0x32d
  0017bff0: mov r0,r5
  0017bff2: blx 0x00072f70
  0017bff6: mov r1,r0
  0017bff8: addw r0,sp,#0x55c
  0017bffc: movs r2,#0x0
  0017bffe: blx 0x0006f028
  0017c002: ldr r1,[0x0017c358]
  0017c004: add r1,pc
  0017c006: add.w r0,sp,#0x550
  0017c00a: movs r2,#0x0
  0017c00c: blx 0x0006ee18
  0017c010: str r0,[sp,#0x0]
  0017c012: sub.w r0,r7,#0x98
  0017c016: add.w r2,sp,#0x568
  0017c01a: addw r3,sp,#0x55c
  0017c01e: mov r1,r6
  0017c020: blx 0x000739b4
  0017c024: sub.w r0,r7,#0x74
  0017c028: sub.w r1,r7,#0x98
  0017c02c: blx 0x0006f2b0
  0017c030: sub.w r0,r7,#0x98
  0017c034: blx r10
  0017c036: add.w r0,sp,#0x550
  0017c03a: blx r10
  0017c03c: addw r0,sp,#0x55c
  0017c040: blx r10
  0017c042: add.w r0,sp,#0x568
  0017c046: blx r10
  0017c048: ldr r0,[sp,#0x10]
  0017c04a: ldr r6,[r0,#0x0]
  0017c04c: addw r0,sp,#0x544
  0017c050: sub.w r1,r7,#0x74
  0017c054: movs r2,#0x0
  0017c056: blx 0x0006f028
  0017c05a: mov r0,r4
  0017c05c: blx 0x00072bb0
  0017c060: mov r1,r0
  0017c062: sub.w r0,r7,#0xb0
  0017c066: blx 0x0006f658
  0017c06a: add.w r0,sp,#0x538
  0017c06e: sub.w r1,r7,#0xb0
  0017c072: movs r2,#0x0
  0017c074: blx 0x0006f028
  0017c078: ldr r1,[0x0017c35c]
  0017c07a: add r1,pc
  0017c07c: addw r0,sp,#0x52c
  0017c080: movs r2,#0x0
  0017c082: blx 0x0006ee18
  0017c086: str r0,[sp,#0x0]
  0017c088: sub.w r0,r7,#0x98
  0017c08c: addw r2,sp,#0x544
  0017c090: add.w r3,sp,#0x538
  0017c094: mov r1,r6
  0017c096: blx 0x000739b4
  0017c09a: sub.w r0,r7,#0x74
  0017c09e: sub.w r1,r7,#0x98
  0017c0a2: blx 0x0006f2b0
  0017c0a6: sub.w r0,r7,#0x98
  0017c0aa: mov r5,r4
  0017c0ac: blx r10
  0017c0ae: addw r0,sp,#0x52c
  0017c0b2: blx r10
  0017c0b4: add.w r0,sp,#0x538
  0017c0b8: blx r10
  0017c0ba: sub.w r0,r7,#0xb0
  0017c0be: blx r10
  0017c0c0: addw r0,sp,#0x544
  0017c0c4: blx r10
  0017c0c6: mov r0,r4
  0017c0c8: blx 0x00072874
  0017c0cc: cmp r0,#0xe
  0017c0ce: bne 0x0017c146
  0017c0d0: ldr r0,[sp,#0x10]
  0017c0d2: ldr r6,[r0,#0x0]
  0017c0d4: add.w r0,sp,#0x520
  0017c0d8: sub.w r1,r7,#0x74
  0017c0dc: movs r2,#0x0
  0017c0de: blx 0x0006f028
  0017c0e2: sub.w r0,r7,#0xb0
  0017c0e6: mov r1,r4
  0017c0e8: blx 0x00074b60
  0017c0ec: addw r0,sp,#0x514
  0017c0f0: sub.w r1,r7,#0xb0
  0017c0f4: movs r2,#0x0
  0017c0f6: blx 0x0006f028
  0017c0fa: ldr r1,[0x0017c360]
  0017c0fc: add r1,pc
  0017c0fe: add.w r0,sp,#0x508
  0017c102: movs r2,#0x0
  0017c104: blx 0x0006ee18
  0017c108: str r0,[sp,#0x0]
  0017c10a: sub.w r0,r7,#0x98
  0017c10e: add.w r2,sp,#0x520
  0017c112: addw r3,sp,#0x514
  0017c116: mov r1,r6
  0017c118: blx 0x000739b4
  0017c11c: sub.w r0,r7,#0x74
  0017c120: sub.w r1,r7,#0x98
  0017c124: blx 0x0006f2b0
  0017c128: sub.w r0,r7,#0x98
  0017c12c: blx r10
  0017c12e: add.w r0,sp,#0x508
  0017c132: blx r10
  0017c134: addw r0,sp,#0x514
  0017c138: blx r10
  0017c13a: sub.w r0,r7,#0xb0
  0017c13e: blx r10
  0017c140: add.w r0,sp,#0x520
  0017c144: b 0x0017c1ba
  0017c146: ldr r0,[sp,#0x10]
  0017c148: ldr r6,[r0,#0x0]
  0017c14a: addw r0,sp,#0x4fc
  0017c14e: sub.w r1,r7,#0x74
  0017c152: movs r2,#0x0
  0017c154: blx 0x0006f028
  0017c158: sub.w r0,r7,#0xb0
  0017c15c: mov r1,r4
  0017c15e: blx 0x00074b54
  0017c162: add.w r0,sp,#0x4f0
  0017c166: sub.w r1,r7,#0xb0
  0017c16a: movs r2,#0x0
  0017c16c: blx 0x0006f028
  0017c170: ldr r1,[0x0017c364]
  0017c172: add r1,pc
  0017c174: addw r0,sp,#0x4e4
  0017c178: movs r2,#0x0
  0017c17a: blx 0x0006ee18
  0017c17e: str r0,[sp,#0x0]
  0017c180: sub.w r0,r7,#0x98
  0017c184: addw r2,sp,#0x4fc
  0017c188: add.w r3,sp,#0x4f0
  0017c18c: mov r1,r6
  0017c18e: blx 0x000739b4
  0017c192: sub.w r0,r7,#0x74
  0017c196: sub.w r1,r7,#0x98
  0017c19a: blx 0x0006f2b0
  0017c19e: sub.w r0,r7,#0x98
  0017c1a2: blx r10
  0017c1a4: addw r0,sp,#0x4e4
  0017c1a8: blx r10
  0017c1aa: add.w r0,sp,#0x4f0
  0017c1ae: blx r10
  0017c1b0: sub.w r0,r7,#0xb0
  0017c1b4: blx r10
  0017c1b6: addw r0,sp,#0x4fc
  0017c1ba: blx r10
  0017c1bc: ldr r0,[sp,#0x10]
  0017c1be: ldr r6,[r0,#0x0]
  0017c1c0: add.w r0,sp,#0x4d8
  0017c1c4: sub.w r1,r7,#0x74
  0017c1c8: movs r2,#0x0
  0017c1ca: blx 0x0006f028
  0017c1ce: sub.w r0,r7,#0xb0
  0017c1d2: mov r1,r4
  0017c1d4: blx 0x00074b48
  0017c1d8: addw r0,sp,#0x4cc
  0017c1dc: sub.w r1,r7,#0xb0
  0017c1e0: movs r2,#0x0
  0017c1e2: blx 0x0006f028
  0017c1e6: ldr r1,[0x0017c368]
  0017c1e8: add r1,pc
  0017c1ea: add.w r0,sp,#0x4c0
  0017c1ee: movs r2,#0x0
  0017c1f0: blx 0x0006ee18
  0017c1f4: str r0,[sp,#0x0]
  0017c1f6: sub.w r0,r7,#0x98
  0017c1fa: add.w r2,sp,#0x4d8
  0017c1fe: addw r3,sp,#0x4cc
  0017c202: mov r1,r6
  0017c204: blx 0x000739b4
  0017c208: sub.w r0,r7,#0x74
  0017c20c: sub.w r1,r7,#0x98
  0017c210: blx 0x0006f2b0
  0017c214: sub.w r0,r7,#0x98
  0017c218: blx r10
  0017c21a: add.w r0,sp,#0x4c0
  0017c21e: blx r10
  0017c220: addw r0,sp,#0x4cc
  0017c224: blx r10
  0017c226: sub.w r0,r7,#0xb0
  0017c22a: blx r10
  0017c22c: add.w r0,sp,#0x4d8
  0017c230: blx r10
  0017c232: ldr r0,[0x0017c36c]
  0017c234: add r0,pc
  0017c236: ldr r0,[r0,#0x0]
  0017c238: ldr r0,[r0,#0x0]
  0017c23a: movs r1,#0x3
  0017c23c: blx 0x00071848
  0017c240: ldr r1,[0x0017c370]
  0017c242: add.w r0,r0,#0x2fc
  0017c246: str.w r0,[r9,#0x28]
  0017c24a: add r1,pc
  0017c24c: sub.w r0,r7,#0xb0
  0017c250: movs r2,#0x0
  0017c252: blx 0x0006ee18
  0017c256: ldr.w r1,[r9,#0x28]
  0017c25a: ldr.w r0,[r8,#0x0]
  0017c25e: blx 0x00072f70
  0017c262: mov r2,r0
  0017c264: sub.w r0,r7,#0x98
  0017c268: sub.w r1,r7,#0xb0
  0017c26c: blx 0x0006ef98
  0017c270: sub.w r0,r7,#0x74
  0017c274: sub.w r1,r7,#0x98
  0017c278: blx 0x0006ef5c
  0017c27c: sub.w r0,r7,#0x98
  0017c280: blx 0x0006ee30
  0017c284: sub.w r0,r7,#0xb0
  0017c288: blx 0x0006ee30
  0017c28c: ldr r0,[sp,#0x10]
  0017c28e: ldr r0,[r0,#0x0]
  0017c290: blx 0x00071b84
  0017c294: mov r5,r0
  0017c296: mov r0,r9
  0017c298: blx 0x00071bd8
  0017c29c: mov r1,r0
  0017c29e: mov r0,r5
  0017c2a0: blx 0x00071c68
  0017c2a4: vmov s16,r0
  0017c2a8: vcmpe.f32 s16,#0
  0017c2ac: vmrs apsr,fpscr
  0017c2b0: ble 0x0017c384
  0017c2b2: ldr r1,[0x0017c374]
  0017c2b4: add r1,pc
  0017c2b6: sub.w r0,r7,#0xbc
  0017c2ba: movs r2,#0x0
  0017c2bc: blx 0x0006ee18
  0017c2c0: ldr r0,[sp,#0x10]
  0017c2c2: ldr r6,[r0,#0x0]
  0017c2c4: ldr.w r0,[r8,#0x0]
  0017c2c8: movw r1,#0x2ff
  0017c2cc: blx 0x00072f70
  0017c2d0: mov r1,r0
  0017c2d2: addw r0,sp,#0x4b4
  0017c2d6: movs r2,#0x0
  0017c2d8: blx 0x0006f028
  0017c2dc: vldr.32 s0,[pc,#0x98]
  0017c2e0: vmul.f32 s0,s16,s0
  0017c2e4: vcvt.s32.f32 s0,s0
  0017c2e8: vmov r1,s0
  0017c2ec: sub.w r0,r7,#0xec
  0017c2f0: blx 0x0006f658
  0017c2f4: add.w r0,sp,#0x4a8
  0017c2f8: sub.w r1,r7,#0xec
  0017c2fc: movs r2,#0x0
  0017c2fe: blx 0x0006f028
  0017c302: ldr r1,[0x0017c37c]
  0017c304: add r1,pc
  0017c306: addw r0,sp,#0x49c
  0017c30a: movs r2,#0x0
  0017c30c: blx 0x0006ee18
  0017c310: str r0,[sp,#0x0]
  0017c312: sub.w r0,r7,#0xe0
  0017c316: addw r2,sp,#0x4b4
  0017c31a: add.w r3,sp,#0x4a8
  0017c31e: mov r1,r6
  0017c320: blx 0x000739b4
  0017c324: sub.w r0,r7,#0xb0
  0017c328: sub.w r1,r7,#0xbc
  0017c32c: sub.w r2,r7,#0xe0
  0017c330: blx 0x0006ef98
  0017c334: ldr r1,[0x0017c380]
  0017c336: add r1,pc
  0017c338: add.w r0,sp,#0x490
  0017c33c: movs r2,#0x0
  0017c33e: blx 0x0006ee18
  0017c342: sub.w r0,r7,#0x98
  0017c346: sub.w r1,r7,#0xb0
  0017c34a: add.w r2,sp,#0x490
  0017c34e: blx 0x0006ef98
  0017c352: movs r5,#0x1
  0017c354: b 0x0017c394
  0017c384: ldr r1,[0x0017c6f4]
  0017c386: movs r5,#0x0
  0017c388: add r1,pc
  0017c38a: sub.w r0,r7,#0x98
  0017c38e: movs r2,#0x0
  0017c390: blx 0x0006ee18
  0017c394: add.w r6,r11,#0xa4
  0017c398: sub.w r1,r7,#0x98
  0017c39c: mov r0,r6
  0017c39e: blx 0x0006f2b0
  0017c3a2: sub.w r0,r7,#0x98
  0017c3a6: blx 0x0006ee30
  0017c3aa: cbz r5,0x0017c3dc
  0017c3ac: add.w r0,sp,#0x490
  0017c3b0: blx r10
  0017c3b2: sub.w r0,r7,#0xb0
  0017c3b6: blx r10
  0017c3b8: sub.w r0,r7,#0xe0
  0017c3bc: blx r10
  0017c3be: addw r0,sp,#0x49c
  0017c3c2: blx r10
  0017c3c4: add.w r0,sp,#0x4a8
  0017c3c8: blx r10
  0017c3ca: sub.w r0,r7,#0xec
  0017c3ce: blx r10
  0017c3d0: addw r0,sp,#0x4b4
  0017c3d4: blx r10
  0017c3d6: sub.w r0,r7,#0xbc
  0017c3da: blx r10
  0017c3dc: addw r0,sp,#0x484
  0017c3e0: sub.w r1,r7,#0x74
  0017c3e4: movs r2,#0x0
  0017c3e6: blx 0x0006f028
  0017c3ea: addw r1,sp,#0x484
  0017c3ee: mov r0,r9
  0017c3f0: blx 0x000749d4
  0017c3f4: addw r0,sp,#0x484
  0017c3f8: str r6,[sp,#0x8]
  0017c3fa: blx 0x0006ee30
  0017c3fe: mov r0,r9
  0017c400: blx 0x00071bb4
  0017c404: blx 0x00071bc0
  0017c408: vmov s0,r0
  0017c40c: movs r1,#0x32
  0017c40e: vcvt.f32.s32 s0,s0
  0017c412: vmul.f32 s0,s16,s0
  0017c416: vcvt.s32.f32 s0,s0
  0017c41a: vmov r5,s0
  0017c41e: mov r0,r5
  0017c420: blx 0x0006f514
  0017c424: mov r6,r1
  0017c426: mov r0,r9
  0017c428: blx 0x00071bb4
  0017c42c: add.w r9,r6,r5
  0017c430: mov r8,r0
  0017c432: movs r1,#0x32
  0017c434: mov r0,r9
  0017c436: blx 0x0006f514
  0017c43a: cmp r1,#0x0
  0017c43c: it ne
  0017c43e: sub.ne.w r9,r5,r6
  0017c442: mov r0,r8
  0017c444: mov r1,r9
  0017c446: blx 0x00071cb0
  0017c44a: ldr r0,[sp,#0x10]
  0017c44c: ldr.w r8,[r0,#0x0]
  0017c450: add.w r0,sp,#0x478
  0017c454: sub.w r1,r7,#0x74
  0017c458: movs r2,#0x0
  0017c45a: blx 0x0006f028
  0017c45e: ldr.w r9,[sp,#0x18]
  0017c462: ldr r0,[0x0017c6f8]
  0017c464: add r0,pc
  0017c466: ldr r0,[r0,#0x0]
  0017c468: ldr r5,[r0,#0x0]
  0017c46a: mov r0,r4
  0017c46c: blx 0x00071bc0
  0017c470: mov r6,r0
  0017c472: mov r0,r4
  0017c474: blx 0x00074b78
  0017c478: adds r2,r0,r6
  0017c47a: addw r0,sp,#0x454
  0017c47e: mov r1,r5
  0017c480: blx 0x00074e54
  0017c484: add.w r0,sp,#0x460
  0017c488: addw r1,sp,#0x454
  0017c48c: movs r2,#0x0
  0017c48e: blx 0x0006f028
  0017c492: ldr r2,[sp,#0x8]
  0017c494: addw r0,sp,#0x46c
  0017c498: add.w r1,sp,#0x460
  0017c49c: blx 0x0006ef98
  0017c4a0: ldr r1,[0x0017c6fc]
  0017c4a2: add r1,pc
  0017c4a4: add.w r0,sp,#0x448
  0017c4a8: movs r2,#0x0
  0017c4aa: blx 0x0006ee18
  0017c4ae: str r0,[sp,#0x0]
  0017c4b0: sub.w r0,r7,#0x98
  0017c4b4: add.w r2,sp,#0x478
  0017c4b8: addw r3,sp,#0x46c
  0017c4bc: mov r1,r8
  0017c4be: blx 0x000739b4
  0017c4c2: sub.w r0,r7,#0x74
  0017c4c6: sub.w r1,r7,#0x98
  0017c4ca: blx 0x0006f2b0
  0017c4ce: sub.w r0,r7,#0x98
  0017c4d2: blx r10
  0017c4d4: add.w r0,sp,#0x448
  0017c4d8: blx r10
  0017c4da: addw r0,sp,#0x46c
  0017c4de: blx r10
  0017c4e0: add.w r0,sp,#0x460
  0017c4e4: blx r10
  0017c4e6: addw r0,sp,#0x454
  0017c4ea: blx r10
  0017c4ec: add.w r0,sp,#0x478
  0017c4f0: blx r10
  0017c4f2: ldr r0,[sp,#0xc]
  0017c4f4: blx 0x000740d4
  0017c4f8: blx 0x0006eb48
  0017c4fc: mov r0,r9
  0017c4fe: blx 0x00071ae8
  0017c502: cbnz r0,0x0017c512
  0017c504: ldr r0,[sp,#0x18]
  0017c506: blx 0x00071bb4
  0017c50a: blx 0x00072874
  0017c50e: cmp r0,#0x8
  0017c510: bne 0x0017c52c
  0017c512: add r0,sp,#0x364
  0017c514: sub.w r1,r7,#0x74
  0017c518: movs r2,#0x0
  0017c51a: blx 0x0006f028
  0017c51e: ldr r0,[sp,#0x18]
  0017c520: add r1,sp,#0x364
  0017c522: blx 0x000749d4
  0017c526: add r0,sp,#0x364
  0017c528: blx 0x0006ee30
  0017c52c: ldr r0,[sp,#0x18]
  0017c52e: blx 0x00071ae8
  0017c532: cmp r0,#0x1
  0017c534: bne 0x0017c570
  0017c536: ldr.w r5,[r11,#0x60]
  0017c53a: ldr r1,[sp,#0x18]
  0017c53c: add r0,sp,#0x358
  0017c53e: blx 0x00071c2c
  0017c542: add r0,sp,#0x34c
  0017c544: sub.w r1,r7,#0x74
  0017c548: movs r2,#0x0
  0017c54a: blx 0x0006f028
  0017c54e: add r1,sp,#0x358
  0017c550: add r2,sp,#0x34c
  0017c552: mov r0,r5
  0017c554: blx 0x00075b2c
  0017c558: ldr r4,[sp,#0x18]
  0017c55a: add r0,sp,#0x34c
  0017c55c: blx 0x0006ee30
  0017c560: add r0,sp,#0x358
  0017c562: blx 0x0006ee30
  0017c566: mov r0,r4
  0017c568: movs r1,#0x1
  0017c56a: blx 0x000749bc
  0017c56e: b 0x0017c83e
  0017c570: ldr r0,[sp,#0x18]
  0017c572: blx 0x00071ae8
  0017c576: cmp r0,#0x7
  0017c578: bne 0x0017c5aa
  0017c57a: ldr.w r5,[r11,#0x60]
  0017c57e: ldr r1,[sp,#0x18]
  0017c580: add r0,sp,#0x340
  0017c582: blx 0x00071c2c
  0017c586: add r0,sp,#0x334
  0017c588: sub.w r1,r7,#0x74
  0017c58c: movs r2,#0x0
  0017c58e: blx 0x0006f028
  0017c592: add r1,sp,#0x340
  0017c594: add r2,sp,#0x334
  0017c596: mov r0,r5
  0017c598: blx 0x00075b2c
  0017c59c: add r0,sp,#0x334
  0017c59e: blx 0x0006ee30
  0017c5a2: add r0,sp,#0x340
  0017c5a4: blx 0x0006ee30
  0017c5a8: b 0x0017c83e
  0017c5aa: ldr r0,[sp,#0x18]
  0017c5ac: blx 0x00071bb4
  0017c5b0: cmp r0,#0x0
  0017c5b2: beq.w 0x0017c714
  0017c5b6: ldr r0,[sp,#0x18]
  0017c5b8: blx 0x00071bb4
  0017c5bc: blx 0x00072874
  0017c5c0: cmp r0,#0xc
  0017c5c2: bne.w 0x0017c714
  0017c5c6: ldr r0,[0x0017c700]
  0017c5c8: add r0,pc
  0017c5ca: ldr.w r9,[r0,#0x0]
  0017c5ce: ldr.w r0,[r9,#0x0]
  0017c5d2: movw r1,#0x31e
  0017c5d6: blx 0x00072f70
  0017c5da: mov r1,r0
  0017c5dc: sub.w r0,r7,#0x74
  0017c5e0: blx 0x0006f2b0
  0017c5e4: ldr r0,[sp,#0x10]
  0017c5e6: ldr.w r8,[r0,#0x0]
  0017c5ea: add r0,sp,#0x328
  0017c5ec: sub.w r1,r7,#0x74
  0017c5f0: movs r2,#0x0
  0017c5f2: blx 0x0006f028
  0017c5f6: ldr r0,[0x0017c704]
  0017c5f8: add r0,pc
  0017c5fa: ldr r0,[r0,#0x0]
  0017c5fc: ldr r6,[r0,#0x0]
  0017c5fe: ldr r0,[sp,#0x18]
  0017c600: blx 0x00071bb4
  0017c604: blx 0x00071bc0
  0017c608: mov r5,r0
  0017c60a: ldr r0,[sp,#0x18]
  0017c60c: blx 0x00071bb4
  0017c610: blx 0x00074b78
  0017c614: adds r2,r0,r5
  0017c616: add.w r0,sp,#0x460
  0017c61a: mov r1,r6
  0017c61c: blx 0x00074e54
  0017c620: add r0,sp,#0x31c
  0017c622: add.w r1,sp,#0x460
  0017c626: movs r2,#0x0
  0017c628: blx 0x0006f028
  0017c62c: ldr r1,[0x0017c708]
  0017c62e: add r1,pc
  0017c630: add r0,sp,#0x310
  0017c632: movs r2,#0x0
  0017c634: blx 0x0006ee18
  0017c638: str r0,[sp,#0x0]
  0017c63a: sub.w r0,r7,#0x98
  0017c63e: add r2,sp,#0x328
  0017c640: add r3,sp,#0x31c
  0017c642: mov r1,r8
  0017c644: blx 0x000739b4
  0017c648: sub.w r0,r7,#0x74
  0017c64c: sub.w r1,r7,#0x98
  0017c650: blx 0x0006f2b0
  0017c654: sub.w r0,r7,#0x98
  0017c658: blx r10
  0017c65a: add r0,sp,#0x310
  0017c65c: blx r10
  0017c65e: add r0,sp,#0x31c
  0017c660: blx r10
  0017c662: add.w r0,sp,#0x460
  0017c666: blx r10
  0017c668: add r0,sp,#0x328
  0017c66a: blx r10
  0017c66c: add r0,sp,#0x304
  0017c66e: sub.w r1,r7,#0x74
  0017c672: movs r2,#0x0
  0017c674: blx 0x0006f028
  0017c678: ldr r0,[sp,#0x18]
  0017c67a: add r1,sp,#0x304
  0017c67c: blx 0x000749d4
  0017c680: add r0,sp,#0x304
  0017c682: blx 0x0006ee30
  0017c686: ldr r1,[0x0017c70c]
  0017c688: add r1,pc
  0017c68a: add.w r0,sp,#0x460
  0017c68e: movs r2,#0x0
  0017c690: blx 0x0006ee18
  0017c694: ldr r0,[0x0017c710]
  0017c696: ldr.w r5,[r9,#0x0]
  0017c69a: add r0,pc
  0017c69c: ldr r0,[r0,#0x0]
  0017c69e: ldr r0,[r0,#0x0]
  0017c6a0: movs r1,#0x3
  0017c6a2: blx 0x00071848
  0017c6a6: addw r1,r0,#0x349
  0017c6aa: mov r0,r5
  0017c6ac: blx 0x00072f70
  0017c6b0: mov r2,r0
  0017c6b2: sub.w r0,r7,#0x98
  0017c6b6: add.w r1,sp,#0x460
  0017c6ba: blx 0x0006ef98
  0017c6be: add.w r0,sp,#0x460
  0017c6c2: blx 0x0006ee30
  0017c6c6: ldr.w r5,[r11,#0x60]
  0017c6ca: ldr r1,[sp,#0x18]
  0017c6cc: add r0,sp,#0x2f8
  0017c6ce: blx 0x00071c2c
  0017c6d2: add r0,sp,#0x2ec
  0017c6d4: sub.w r1,r7,#0x74
  0017c6d8: sub.w r2,r7,#0x98
  0017c6dc: blx 0x0006ef98
  0017c6e0: add r1,sp,#0x2f8
  0017c6e2: add r2,sp,#0x2ec
  0017c6e4: mov r0,r5
  0017c6e6: blx 0x00075b2c
  0017c6ea: add r0,sp,#0x2ec
  0017c6ec: blx r10
  0017c6ee: add r0,sp,#0x2f8
  0017c6f0: b 0x0017c836
  0017c714: ldr r1,[0x0017cb00]
  0017c716: add r1,pc
  0017c718: add.w r0,sp,#0x460
  0017c71c: movs r2,#0x0
  0017c71e: blx 0x0006ee18
  0017c722: sub.w r0,r7,#0x98
  0017c726: add.w r1,sp,#0x460
  0017c72a: sub.w r2,r7,#0x74
  0017c72e: blx 0x0006ef98
  0017c732: sub.w r0,r7,#0x74
  0017c736: sub.w r1,r7,#0x98
  0017c73a: blx 0x0006f2b0
  0017c73e: sub.w r0,r7,#0x98
  0017c742: blx 0x0006ee30
  0017c746: add.w r0,sp,#0x460
  0017c74a: blx 0x0006ee30
  0017c74e: ldr r1,[0x0017cb04]
  0017c750: add r1,pc
  0017c752: add.w r0,sp,#0x460
  0017c756: movs r2,#0x0
  0017c758: blx 0x0006ee18
  0017c75c: ldr r4,[sp,#0x18]
  0017c75e: ldr r0,[0x0017cb08]
  0017c760: ldr r1,[0x0017cb0c]
  0017c762: add r0,pc
  0017c764: add r1,pc
  0017c766: ldr r0,[r0,#0x0]
  0017c768: ldr r1,[r1,#0x0]
  0017c76a: ldr r5,[r0,#0x0]
  0017c76c: ldr r0,[r1,#0x0]
  0017c76e: movs r1,#0x3
  0017c770: blx 0x00071848
  0017c774: addw r1,r0,#0x349
  0017c778: mov r0,r5
  0017c77a: blx 0x00072f70
  0017c77e: mov r2,r0
  0017c780: sub.w r0,r7,#0x98
  0017c784: add.w r1,sp,#0x460
  0017c788: blx 0x0006ef98
  0017c78c: add.w r0,sp,#0x460
  0017c790: blx 0x0006ee30
  0017c794: ldr.w r6,[r11,#0x60]
  0017c798: add r0,sp,#0x2e0
  0017c79a: mov r1,r4
  0017c79c: blx 0x00071c2c
  0017c7a0: ldr r1,[0x0017cb10]
  0017c7a2: add r1,pc
  0017c7a4: add r0,sp,#0x2a4
  0017c7a6: movs r2,#0x0
  0017c7a8: blx 0x0006ee18
  0017c7ac: add r0,sp,#0x2b0
  0017c7ae: sub.w r1,r7,#0x38
  0017c7b2: add r2,sp,#0x2a4
  0017c7b4: blx 0x0006ef98
  0017c7b8: add r0,sp,#0x2bc
  0017c7ba: add r1,sp,#0x2b0
  0017c7bc: sub.w r2,r7,#0x5c
  0017c7c0: blx 0x0006ef98
  0017c7c4: ldr r1,[0x0017cb14]
  0017c7c6: add r1,pc
  0017c7c8: add r0,sp,#0x298
  0017c7ca: movs r2,#0x0
  0017c7cc: blx 0x0006ee18
  0017c7d0: add r0,sp,#0x2c8
  0017c7d2: add r1,sp,#0x2bc
  0017c7d4: add r2,sp,#0x298
  0017c7d6: blx 0x0006ef98
  0017c7da: addw r0,sp,#0x454
  0017c7de: add r1,sp,#0x2c8
  0017c7e0: sub.w r2,r7,#0x68
  0017c7e4: blx 0x0006ef98
  0017c7e8: add.w r0,sp,#0x460
  0017c7ec: addw r1,sp,#0x454
  0017c7f0: sub.w r2,r7,#0x74
  0017c7f4: blx 0x0006ef98
  0017c7f8: add r0,sp,#0x2d4
  0017c7fa: add.w r1,sp,#0x460
  0017c7fe: sub.w r2,r7,#0x98
  0017c802: blx 0x0006ef98
  0017c806: add r1,sp,#0x2e0
  0017c808: add r2,sp,#0x2d4
  0017c80a: mov r0,r6
  0017c80c: blx 0x00075b2c
  0017c810: add r0,sp,#0x2d4
  0017c812: blx r10
  0017c814: add.w r0,sp,#0x460
  0017c818: blx r10
  0017c81a: addw r0,sp,#0x454
  0017c81e: blx r10
  0017c820: add r0,sp,#0x2c8
  0017c822: blx r10
  0017c824: add r0,sp,#0x298
  0017c826: blx r10
  0017c828: add r0,sp,#0x2bc
  0017c82a: blx r10
  0017c82c: add r0,sp,#0x2b0
  0017c82e: blx r10
  0017c830: add r0,sp,#0x2a4
  0017c832: blx r10
  0017c834: add r0,sp,#0x2e0
  0017c836: blx r10
  0017c838: sub.w r0,r7,#0x98
  0017c83c: blx r10
  0017c83e: sub.w r0,r7,#0x74
  0017c842: blx r10
  0017c844: sub.w r0,r7,#0x68
  0017c848: blx r10
  0017c84a: sub.w r0,r7,#0x5c
  0017c84e: blx r10
  0017c850: sub.w r0,r7,#0x38
  0017c854: blx r10
  0017c856: ldr.w r9,[sp,#0x18]
  0017c85a: ldr r0,[0x0017cb18]
  0017c85c: add r0,pc
  0017c85e: ldr r4,[r0,#0x0]
  0017c860: ldr r0,[r4,#0x0]
  0017c862: blx 0x00071c14
  0017c866: add r5,sp,#0x28
  0017c868: mov r1,r0
  0017c86a: mov r0,r5
  0017c86c: blx 0x000736a8
  0017c870: mov r0,r9
  0017c872: mov r1,r5
  0017c874: blx 0x000749e0
  0017c878: add r0,sp,#0x28
  0017c87a: blx 0x0006ee30
  0017c87e: ldr r0,[r4,#0x0]
  0017c880: blx 0x00071a10
  0017c884: add r5,sp,#0x1c
  0017c886: mov r1,r0
  0017c888: mov r0,r5
  0017c88a: blx 0x00071cc8
  0017c88e: mov r0,r9
  0017c890: mov r1,r5
  0017c892: blx 0x000749ec
  0017c896: add r0,sp,#0x1c
  0017c898: blx 0x0006ee30
  0017c89c: movs r4,#0x0
  0017c89e: movs r0,#0x1
  0017c8a0: str.w r4,[r11,#0x2c]
  0017c8a4: mov.w r1,#0x10000
  0017c8a8: str.w r0,[r11,#0x14]
  0017c8ac: mov r0,r11
  0017c8ae: blx 0x00077d64
  0017c8b2: mov r0,r11
  0017c8b4: mov r1,r9
  0017c8b6: blx 0x00077d4c
  0017c8ba: ldr r5,[sp,#0x14]
  0017c8bc: mov r1,r0
  0017c8be: cmp r0,#0x0
  0017c8c0: blt 0x0017c8d4
  0017c8c2: ldr r0,[0x0017cb1c]
  0017c8c4: movs r2,#0x0
  0017c8c6: movs r3,#0x0
  0017c8c8: str r4,[sp,#0x0]
  0017c8ca: add r0,pc
  0017c8cc: ldr r0,[r0,#0x0]
  0017c8ce: ldr r0,[r0,#0x0]
  0017c8d0: blx 0x00071548
  0017c8d4: ldr.w r0,[r7,#-0x2c]
  0017c8d8: ldr r1,[r5,#0x0]
  0017c8da: subs r0,r1,r0
  0017c8dc: itttt eq
  0017c8de: add.eq.w sp,sp,#0x7d0
  0017c8e2: vpop.eq {d8}
  0017c8e6: add.eq sp,#0x4
  0017c8e8: pop.eq.w {r8,r9,r10,r11}
  0017c8ec: it eq
  0017c8ee: pop.eq {r4,r5,r6,r7,pc}
  0017c8f0: blx 0x0006e824
  0017c8f4: mov r0,r9
  0017c8f6: blx 0x00071bb4
  0017c8fa: cmp r0,#0x0
  0017c8fc: beq.w 0x0017b294
  0017c900: mov r0,r9
  0017c902: blx 0x00071bb4
  0017c906: blx 0x00072874
  0017c90a: cmp r0,#0x8
  0017c90c: beq.w 0x0017b294
  0017c910: mov r0,r9
  0017c912: blx 0x00071bb4
  0017c916: blx 0x00072874
  0017c91a: cmp r0,#0xc
  0017c91c: beq.w 0x0017b294
  0017c920: ldr r0,[0x0017cb20]
  0017c922: add r0,pc
  0017c924: ldr.w r10,[r0,#0x0]
  0017c928: ldr.w r0,[r10,#0x0]
  0017c92c: blx 0x00071b84
  0017c930: mov r5,r0
  0017c932: mov r0,r9
  0017c934: blx 0x00071bd8
  0017c938: mov r1,r0
  0017c93a: mov r0,r5
  0017c93c: blx 0x00071c68
  0017c940: mov r5,r0
  0017c942: mov r0,r9
  0017c944: blx 0x00071bb4
  0017c948: blx 0x00071bc0
  0017c94c: vmov s0,r0
  0017c950: movs r1,#0x32
  0017c952: vmov s16,r5
  0017c956: vcvt.f32.s32 s0,s0
  0017c95a: vmul.f32 s0,s16,s0
  0017c95e: vcvt.s32.f32 s0,s0
  0017c962: vmov r5,s0
  0017c966: mov r0,r5
  0017c968: blx 0x0006f514
  0017c96c: adds r6,r1,r5
  0017c96e: mov r8,r1
  0017c970: movs r1,#0x32
  0017c972: mov r0,r6
  0017c974: blx 0x0006f514
  0017c978: mov r0,r9
  0017c97a: cmp r1,#0x0
  0017c97c: it ne
  0017c97e: sub.ne.w r6,r5,r8
  0017c982: blx 0x00071bb4
  0017c986: mov r1,r6
  0017c988: blx 0x00071cb0
  0017c98c: vcmpe.f32 s16,#0
  0017c990: vmrs apsr,fpscr
  0017c994: ble.w 0x0017d61c
  0017c998: ldr r1,[0x0017cb24]
  0017c99a: sub.w r0,r7,#0x68
  0017c99e: movs r2,#0x0
  0017c9a0: add r1,pc
  0017c9a2: blx 0x0006ee18
  0017c9a6: ldr r0,[0x0017cb28]
  0017c9a8: ldr.w r6,[r10,#0x0]
  0017c9ac: add r0,pc
  0017c9ae: ldr r0,[r0,#0x0]
  0017c9b0: ldr r0,[r0,#0x0]
  0017c9b2: movw r1,#0x2ff
  0017c9b6: blx 0x00072f70
  0017c9ba: mov r1,r0
  0017c9bc: sub.w r0,r7,#0x80
  0017c9c0: movs r2,#0x0
  0017c9c2: blx 0x0006f028
  0017c9c6: vldr.32 s0,[pc,#0x164]
  0017c9ca: vmul.f32 s0,s16,s0
  0017c9ce: vcvt.s32.f32 s0,s0
  0017c9d2: vmov r1,s0
  0017c9d6: sub.w r0,r7,#0x98
  0017c9da: blx 0x0006f658
  0017c9de: sub.w r0,r7,#0x8c
  0017c9e2: sub.w r1,r7,#0x98
  0017c9e6: movs r2,#0x0
  0017c9e8: blx 0x0006f028
  0017c9ec: ldr r1,[0x0017cb30]
  0017c9ee: add r1,pc
  0017c9f0: sub.w r0,r7,#0xa4
  0017c9f4: movs r2,#0x0
  0017c9f6: blx 0x0006ee18
  0017c9fa: str r0,[sp,#0x0]
  0017c9fc: sub.w r0,r7,#0x74
  0017ca00: sub.w r2,r7,#0x80
  0017ca04: sub.w r3,r7,#0x8c
  0017ca08: mov r1,r6
  0017ca0a: blx 0x000739b4
  0017ca0e: sub.w r0,r7,#0x5c
  0017ca12: sub.w r1,r7,#0x68
  0017ca16: sub.w r2,r7,#0x74
  0017ca1a: blx 0x0006ef98
  0017ca1e: ldr r1,[0x0017cb34]
  0017ca20: add r1,pc
  0017ca22: sub.w r0,r7,#0xb0
  0017ca26: movs r2,#0x0
  0017ca28: blx 0x0006ee18
  0017ca2c: sub.w r0,r7,#0x38
  0017ca30: sub.w r1,r7,#0x5c
  0017ca34: sub.w r2,r7,#0xb0
  0017ca38: blx 0x0006ef98
  0017ca3c: movs r5,#0x1
  0017ca3e: b.w 0x0017d62c
  0017ca42: mov r0,r9
  0017ca44: blx 0x00071ae8
  0017ca48: cmp r0,#0xa
  0017ca4a: bne.w 0x0017ce60
  0017ca4e: ldr.w r0,[r10,#0x0]
  0017ca52: ldr.w r0,[r0,#0x114]
  0017ca56: cmp r0,#0x2
  0017ca58: bgt.w 0x0017d10c
  0017ca5c: ldr r0,[0x0017cb38]
  0017ca5e: ldr r1,[0x0017cb3c]
  0017ca60: add r0,pc
  0017ca62: add r1,pc
  0017ca64: ldr r0,[r0,#0x0]
  0017ca66: ldr r4,[r1,#0x0]
  0017ca68: ldr r5,[r0,#0x0]
  0017ca6a: ldr r0,[r4,#0x0]
  0017ca6c: movs r1,#0x6
  0017ca6e: blx 0x00071848
  0017ca72: addw r1,r0,#0x2ee
  0017ca76: mov r0,r5
  0017ca78: blx 0x00072f70
  0017ca7c: mov r1,r0
  0017ca7e: sub.w r0,r7,#0x38
  0017ca82: blx 0x0006f2b0
  0017ca86: ldr r0,[0x0017cb40]
  0017ca88: add r0,pc
  0017ca8a: ldr r0,[r0,#0x0]
  0017ca8c: ldr.w r8,[r0,#0x0]
  0017ca90: ldr r0,[r4,#0x0]
  0017ca92: movs r1,#0x2
  0017ca94: blx 0x00071848
  0017ca98: mov r5,r0
  0017ca9a: mov r0,r9
  0017ca9c: blx 0x00071bd8
  0017caa0: mov r6,r0
  0017caa2: mov r0,r9
  0017caa4: blx 0x00074110
  0017caa8: addw r1,r5,#0x30d
  0017caac: mov r2,r6
  0017caae: mov r3,r0
  0017cab0: blx 0x00077d58
  0017cab4: mov r1,r0
  0017cab6: movs r0,#0x0
  0017cab8: movs r2,#0x0
  0017caba: str r0,[sp,#0x0]
  0017cabc: mov r0,r8
  0017cabe: movs r3,#0x0
  0017cac0: blx 0x00071548
  0017cac4: ldr.w r0,[r10,#0x0]
  0017cac8: ldr.w r1,[r0,#0xd0]
  0017cacc: subs r1,#0x1
  0017cace: str.w r1,[r0,#0xd0]
  0017cad2: ldr.w r5,[r11,#0x60]
  0017cad6: add r0,sp,#0x178
  0017cad8: mov r1,r9
  0017cada: blx 0x00071c2c
  0017cade: add r0,sp,#0x16c
  0017cae0: sub.w r1,r7,#0x38
  0017cae4: movs r2,#0x0
  0017cae6: blx 0x0006f028
  0017caea: add r1,sp,#0x178
  0017caec: add r2,sp,#0x16c
  0017caee: mov r0,r5
  0017caf0: blx 0x00075b2c
  0017caf4: add r0,sp,#0x16c
  0017caf6: blx 0x0006ee30
  0017cafa: add r0,sp,#0x178
  0017cafc: b.w 0x0017adf8
  0017cb44: ldr r0,[0x0017ce34]
  0017cb46: add r0,pc
  0017cb48: ldr r0,[r0,#0x0]
  0017cb4a: ldr r0,[r0,#0x0]
  0017cb4c: movs r1,#0x2
  0017cb4e: blx 0x00071848
  0017cb52: addw r0,r0,#0x36a
  0017cb56: str.w r0,[r9,#0x28]
  0017cb5a: ldr.w r0,[r10,#0x0]
  0017cb5e: blx 0x00071a58
  0017cb62: blx 0x00071b00
  0017cb66: mov r5,r0
  0017cb68: mov r0,r9
  0017cb6a: blx 0x00075e14
  0017cb6e: muls r0,r5
  0017cb70: movs r1,#0x64
  0017cb72: blx 0x0007198c
  0017cb76: mov r1,r0
  0017cb78: mov r0,r9
  0017cb7a: blx 0x00075e20
  0017cb7e: ldr r0,[0x0017ce38]
  0017cb80: ldr.w r1,[r9,#0x28]
  0017cb84: add r0,pc
  0017cb86: ldr r4,[r0,#0x0]
  0017cb88: ldr r0,[r4,#0x0]
  0017cb8a: blx 0x00072f70
  0017cb8e: mov r1,r0
  0017cb90: sub.w r0,r7,#0x38
  0017cb94: blx 0x0006f2b0
  0017cb98: ldr r1,[0x0017ce3c]
  0017cb9a: add r1,pc
  0017cb9c: sub.w r0,r7,#0x68
  0017cba0: movs r2,#0x0
  0017cba2: blx 0x0006ee18
  0017cba6: ldr r5,[r4,#0x0]
  0017cba8: mov r0,r9
  0017cbaa: blx 0x00074ba8
  0017cbae: addw r1,r0,#0x376
  0017cbb2: mov r0,r5
  0017cbb4: blx 0x00072f70
  0017cbb8: mov r2,r0
  0017cbba: sub.w r0,r7,#0x5c
  0017cbbe: sub.w r1,r7,#0x68
  0017cbc2: blx 0x0006ef98
  0017cbc6: sub.w r0,r7,#0x38
  0017cbca: sub.w r1,r7,#0x5c
  0017cbce: blx 0x0006ef5c
  0017cbd2: sub.w r0,r7,#0x5c
  0017cbd6: blx 0x0006ee30
  0017cbda: sub.w r0,r7,#0x68
  0017cbde: blx 0x0006ee30
  0017cbe2: ldr r1,[0x0017ce40]
  0017cbe4: add r1,pc
  0017cbe6: sub.w r0,r7,#0x68
  0017cbea: movs r2,#0x0
  0017cbec: blx 0x0006ee18
  0017cbf0: ldr r0,[r4,#0x0]
  0017cbf2: movw r1,#0x36f
  0017cbf6: blx 0x00072f70
  0017cbfa: mov r2,r0
  0017cbfc: sub.w r0,r7,#0x5c
  0017cc00: sub.w r1,r7,#0x68
  0017cc04: blx 0x0006ef98
  0017cc08: sub.w r0,r7,#0x38
  0017cc0c: sub.w r1,r7,#0x5c
  0017cc10: blx 0x0006ef5c
  0017cc14: sub.w r0,r7,#0x5c
  0017cc18: blx 0x0006ee30
  0017cc1c: sub.w r0,r7,#0x68
  0017cc20: blx 0x0006ee30
  0017cc24: ldr.w r6,[r10,#0x0]
  0017cc28: add r0,sp,#0x274
  0017cc2a: sub.w r1,r7,#0x38
  0017cc2e: movs r2,#0x0
  0017cc30: blx 0x0006f028
  0017cc34: ldr r5,[r4,#0x0]
  0017cc36: ldr.w r0,[r10,#0x0]
  0017cc3a: blx 0x00071a58
  0017cc3e: blx 0x000719c8
  0017cc42: addw r1,r0,#0x391
  0017cc46: mov r0,r5
  0017cc48: blx 0x00072f70
  0017cc4c: mov r1,r0
  0017cc4e: add r0,sp,#0x268
  0017cc50: movs r2,#0x0
  0017cc52: blx 0x0006f028
  0017cc56: ldr r1,[0x0017ce44]
  0017cc58: add r1,pc
  0017cc5a: add r0,sp,#0x25c
  0017cc5c: movs r2,#0x0
  0017cc5e: blx 0x0006ee18
  0017cc62: str r0,[sp,#0x0]
  0017cc64: sub.w r0,r7,#0x5c
  0017cc68: add r2,sp,#0x274
  0017cc6a: add r3,sp,#0x268
  0017cc6c: mov r1,r6
  0017cc6e: blx 0x000739b4
  0017cc72: sub.w r0,r7,#0x38
  0017cc76: sub.w r1,r7,#0x5c
  0017cc7a: blx 0x0006f2b0
  0017cc7e: ldr r0,[0x0017ce48]
  0017cc80: add r0,pc
  0017cc82: ldr r6,[r0,#0x0]
  0017cc84: sub.w r0,r7,#0x5c
  0017cc88: blx r6
  0017cc8a: add r0,sp,#0x25c
  0017cc8c: blx r6
  0017cc8e: add r0,sp,#0x268
  0017cc90: blx r6
  0017cc92: add r0,sp,#0x274
  0017cc94: blx r6
  0017cc96: ldr.w r5,[r10,#0x0]
  0017cc9a: add r0,sp,#0x250
  0017cc9c: sub.w r1,r7,#0x38
  0017cca0: movs r2,#0x0
  0017cca2: blx 0x0006f028
  0017cca6: mov r0,r9
  0017cca8: blx 0x00074bfc
  0017ccac: ldr r1,[0x0017ce4c]
  0017ccae: add r1,pc
  0017ccb0: add.w r0,r1,r0, lsl #0x3
  0017ccb4: ldr r1,[r0,#0x4]
  0017ccb6: sub.w r0,r7,#0x68
  0017ccba: blx 0x0006f658
  0017ccbe: add r0,sp,#0x244
  0017ccc0: sub.w r1,r7,#0x68
  0017ccc4: movs r2,#0x0
  0017ccc6: blx 0x0006f028
  0017ccca: ldr r1,[0x0017ce50]
  0017cccc: add r1,pc
  0017ccce: add r0,sp,#0x238
  0017ccd0: movs r2,#0x0
  0017ccd2: blx 0x0006ee18
  0017ccd6: str r0,[sp,#0x0]
  0017ccd8: sub.w r0,r7,#0x5c
  0017ccdc: add r2,sp,#0x250
  0017ccde: add r3,sp,#0x244
  0017cce0: mov r1,r5
  0017cce2: blx 0x000739b4
  0017cce6: sub.w r0,r7,#0x38
  0017ccea: sub.w r1,r7,#0x5c
  0017ccee: blx 0x0006f2b0
  0017ccf2: sub.w r0,r7,#0x5c
  0017ccf6: blx r6
  0017ccf8: add r0,sp,#0x238
  0017ccfa: blx r6
  0017ccfc: add r0,sp,#0x244
  0017ccfe: blx r6
  0017cd00: sub.w r0,r7,#0x68
  0017cd04: blx r6
  0017cd06: add r0,sp,#0x250
  0017cd08: blx r6
  0017cd0a: ldr.w r8,[r10,#0x0]
  0017cd0e: add r0,sp,#0x22c
  0017cd10: sub.w r1,r7,#0x38
  0017cd14: movs r2,#0x0
  0017cd16: blx 0x0006f028
  0017cd1a: ldr r0,[0x0017ce54]
  0017cd1c: add r0,pc
  0017cd1e: ldr r0,[r0,#0x0]
  0017cd20: ldr r5,[r0,#0x0]
  0017cd22: mov r0,r9
  0017cd24: blx 0x00074bc0
  0017cd28: vmov s0,r0
  0017cd2c: vcvt.f32.s32 s0,s0
  0017cd30: vmul.f32 s0,s16,s0
  0017cd34: vcvt.s32.f32 s0,s0
  0017cd38: vmov r2,s0
  0017cd3c: sub.w r0,r7,#0x68
  0017cd40: mov r1,r5
  0017cd42: blx 0x00074e54
  0017cd46: add r0,sp,#0x220
  0017cd48: sub.w r1,r7,#0x68
  0017cd4c: movs r2,#0x0
  0017cd4e: blx 0x0006f028
  0017cd52: ldr r1,[0x0017ce58]
  0017cd54: add r1,pc
  0017cd56: add r0,sp,#0x214
  0017cd58: movs r2,#0x0
  0017cd5a: blx 0x0006ee18
  0017cd5e: str r0,[sp,#0x0]
  0017cd60: sub.w r0,r7,#0x5c
  0017cd64: add r2,sp,#0x22c
  0017cd66: add r3,sp,#0x220
  0017cd68: mov r1,r8
  0017cd6a: blx 0x000739b4
  0017cd6e: sub.w r0,r7,#0x38
  0017cd72: sub.w r1,r7,#0x5c
  0017cd76: blx 0x0006f2b0
  0017cd7a: sub.w r0,r7,#0x5c
  0017cd7e: blx r6
  0017cd80: add r0,sp,#0x214
  0017cd82: blx r6
  0017cd84: add r0,sp,#0x220
  0017cd86: blx r6
  0017cd88: sub.w r0,r7,#0x68
  0017cd8c: blx r6
  0017cd8e: add r0,sp,#0x22c
  0017cd90: b 0x0017d366
  0017cd92: mov r0,r9
  0017cd94: blx 0x00071bb4
  0017cd98: movw r5,#0x35a
  0017cd9c: cbz r0,0x0017cdb0
  0017cd9e: mov r0,r9
  0017cda0: blx 0x00071bb4
  0017cda4: blx 0x00072874
  0017cda8: cmp r0,#0xc
  0017cdaa: it eq
  0017cdac: movw.eq r5,#0x35b
  0017cdb0: mov r0,r6
  0017cdb2: mov r1,r5
  0017cdb4: blx 0x00072f70
  0017cdb8: mov r1,r0
  0017cdba: sub.w r0,r7,#0x38
  0017cdbe: movs r2,#0x0
  0017cdc0: blx 0x0006f028
  0017cdc4: ldr.w r5,[r11,#0x60]
  0017cdc8: sub.w r0,r7,#0x44
  0017cdcc: mov r1,r9
  0017cdce: blx 0x00071c2c
  0017cdd2: sub.w r0,r7,#0x50
  0017cdd6: sub.w r1,r7,#0x38
  0017cdda: movs r2,#0x0
  0017cddc: blx 0x0006f028
  0017cde0: sub.w r1,r7,#0x44
  0017cde4: sub.w r2,r7,#0x50
  0017cde8: mov r0,r5
  0017cdea: blx 0x00075b2c
  0017cdee: sub.w r0,r7,#0x50
  0017cdf2: blx 0x0006ee30
  0017cdf6: sub.w r0,r7,#0x44
  0017cdfa: blx 0x0006ee30
  0017cdfe: movs r0,#0x3
  0017ce00: str.w r0,[r11,#0x14]
  0017ce04: movs r0,#0x0
  0017ce06: str.w r0,[r11,#0x2c]
  0017ce0a: mov r0,r11
  0017ce0c: mov r1,r9
  0017ce0e: blx 0x00077d4c
  0017ce12: mov r1,r0
  0017ce14: cmp r1,#0x0
  0017ce16: blt.w 0x0017ae08
  0017ce1a: ldr r0,[0x0017ce5c]
  0017ce1c: add r0,pc
  0017ce1e: ldr r0,[r0,#0x0]
  0017ce20: ldr r0,[r0,#0x0]
  0017ce22: movs r2,#0x0
  0017ce24: movs r3,#0x0
  0017ce26: str r2,[sp,#0x0]
  0017ce28: movs r2,#0x0
  0017ce2a: blx 0x00071548
  0017ce2e: b.w 0x0017ae08
  0017ce60: mov r0,r9
  0017ce62: blx 0x00071ae8
  0017ce66: mov r1,r0
  0017ce68: ldr r0,[0x0017d0dc]
  0017ce6a: cmp r1,#0x4
  0017ce6c: add r0,pc
  0017ce6e: ldr r0,[r0,#0x0]
  0017ce70: ldr r0,[r0,#0x0]
  0017ce72: bne.w 0x0017d398
  0017ce76: movs r1,#0x2
  0017ce78: blx 0x00071848
  0017ce7c: ldr r1,[0x0017d0e0]
  0017ce7e: add r1,pc
  0017ce80: ldr r4,[r1,#0x0]
  0017ce82: addw r1,r0,#0x36a
  0017ce86: str.w r1,[r9,#0x28]
  0017ce8a: ldr r0,[r4,#0x0]
  0017ce8c: blx 0x00072f70
  0017ce90: mov r1,r0
  0017ce92: sub.w r0,r7,#0x38
  0017ce96: blx 0x0006f2b0
  0017ce9a: ldr r1,[0x0017d0e4]
  0017ce9c: add r1,pc
  0017ce9e: sub.w r0,r7,#0x68
  0017cea2: movs r2,#0x0
  0017cea4: blx 0x0006ee18
  0017cea8: ldr r5,[r4,#0x0]
  0017ceaa: mov r0,r9
  0017ceac: blx 0x00074ba8
  0017ceb0: addw r1,r0,#0x376
  0017ceb4: mov r0,r5
  0017ceb6: blx 0x00072f70
  0017ceba: mov r2,r0
  0017cebc: sub.w r0,r7,#0x5c
  0017cec0: sub.w r1,r7,#0x68
  0017cec4: blx 0x0006ef98
  0017cec8: sub.w r0,r7,#0x38
  0017cecc: sub.w r1,r7,#0x5c
  0017ced0: blx 0x0006ef5c
  0017ced4: sub.w r0,r7,#0x5c
  0017ced8: blx 0x0006ee30
  0017cedc: sub.w r0,r7,#0x68
  0017cee0: blx 0x0006ee30
  0017cee4: ldr r1,[0x0017d0e8]
  0017cee6: add r1,pc
  0017cee8: sub.w r0,r7,#0x68
  0017ceec: movs r2,#0x0
  0017ceee: blx 0x0006ee18
  0017cef2: ldr r0,[r4,#0x0]
  0017cef4: movw r1,#0x36d
  0017cef8: blx 0x00072f70
  0017cefc: mov r2,r0
  0017cefe: sub.w r0,r7,#0x5c
  0017cf02: sub.w r1,r7,#0x68
  0017cf06: blx 0x0006ef98
  0017cf0a: sub.w r0,r7,#0x38
  0017cf0e: sub.w r1,r7,#0x5c
  0017cf12: blx 0x0006ef5c
  0017cf16: sub.w r0,r7,#0x5c
  0017cf1a: blx 0x0006ee30
  0017cf1e: sub.w r0,r7,#0x68
  0017cf22: blx 0x0006ee30
  0017cf26: ldr r0,[0x0017d0ec]
  0017cf28: add r0,pc
  0017cf2a: ldr r0,[r0,#0x0]
  0017cf2c: ldr r5,[r0,#0x0]
  0017cf2e: mov r0,r9
  0017cf30: blx 0x00074b84
  0017cf34: mov r1,r0
  0017cf36: mov r0,r5
  0017cf38: blx 0x00073684
  0017cf3c: ldr.w r6,[r10,#0x0]
  0017cf40: mov r5,r0
  0017cf42: add r0,sp,#0xf4
  0017cf44: sub.w r1,r7,#0x38
  0017cf48: movs r2,#0x0
  0017cf4a: blx 0x0006f028
  0017cf4e: sub.w r0,r7,#0x68
  0017cf52: mov r1,r5
  0017cf54: blx 0x00071cc8
  0017cf58: add r0,sp,#0xe8
  0017cf5a: sub.w r1,r7,#0x68
  0017cf5e: movs r2,#0x0
  0017cf60: blx 0x0006f028
  0017cf64: ldr r1,[0x0017d0f0]
  0017cf66: add r1,pc
  0017cf68: add r0,sp,#0xdc
  0017cf6a: movs r2,#0x0
  0017cf6c: blx 0x0006ee18
  0017cf70: str r0,[sp,#0x0]
  0017cf72: sub.w r0,r7,#0x5c
  0017cf76: add r2,sp,#0xf4
  0017cf78: add r3,sp,#0xe8
  0017cf7a: mov r1,r6
  0017cf7c: blx 0x000739b4
  0017cf80: sub.w r0,r7,#0x38
  0017cf84: sub.w r1,r7,#0x5c
  0017cf88: blx 0x0006f2b0
  0017cf8c: ldr r0,[0x0017d0f4]
  0017cf8e: add r0,pc
  0017cf90: ldr r4,[r0,#0x0]
  0017cf92: sub.w r0,r7,#0x5c
  0017cf96: blx r4
  0017cf98: add r0,sp,#0xdc
  0017cf9a: blx r4
  0017cf9c: add r0,sp,#0xe8
  0017cf9e: blx r4
  0017cfa0: sub.w r0,r7,#0x68
  0017cfa4: blx r4
  0017cfa6: add r0,sp,#0xf4
  0017cfa8: blx r4
  0017cfaa: ldr.w r6,[r10,#0x0]
  0017cfae: add r0,sp,#0xd0
  0017cfb0: sub.w r1,r7,#0x38
  0017cfb4: movs r2,#0x0
  0017cfb6: blx 0x0006f028
  0017cfba: ldr r0,[0x0017d0f8]
  0017cfbc: add r0,pc
  0017cfbe: ldr r0,[r0,#0x0]
  0017cfc0: ldr r5,[r0,#0x0]
  0017cfc2: mov r0,r9
  0017cfc4: blx 0x00074bc0
  0017cfc8: mov r2,r0
  0017cfca: sub.w r0,r7,#0x68
  0017cfce: mov r1,r5
  0017cfd0: blx 0x00074e54
  0017cfd4: add r0,sp,#0xc4
  0017cfd6: sub.w r1,r7,#0x68
  0017cfda: movs r2,#0x0
  0017cfdc: blx 0x0006f028
  0017cfe0: ldr r1,[0x0017d0fc]
  0017cfe2: add r1,pc
  0017cfe4: add r0,sp,#0xb8
  0017cfe6: movs r2,#0x0
  0017cfe8: blx 0x0006ee18
  0017cfec: str r0,[sp,#0x0]
  0017cfee: sub.w r0,r7,#0x5c
  0017cff2: add r2,sp,#0xd0
  0017cff4: add r3,sp,#0xc4
  0017cff6: mov r1,r6
  0017cff8: blx 0x000739b4
  0017cffc: sub.w r0,r7,#0x38
  0017d000: sub.w r1,r7,#0x5c
  0017d004: blx 0x0006f2b0
  0017d008: sub.w r0,r7,#0x5c
  0017d00c: blx r4
  0017d00e: add r0,sp,#0xb8
  0017d010: blx r4
  0017d012: add r0,sp,#0xc4
  0017d014: blx r4
  0017d016: sub.w r0,r7,#0x68
  0017d01a: blx r4
  0017d01c: add r0,sp,#0xd0
  0017d01e: b 0x0017d53a
  0017d020: movw r1,#0x36e
  0017d024: b 0x0017d032
  0017d026: movw r1,#0x372
  0017d02a: cmp r6,#0x0
  0017d02c: it eq
  0017d02e: movw.eq r1,#0x371
  0017d032: ldr r0,[0x0017d100]
  0017d034: add r0,pc
  0017d036: ldr r0,[r0,#0x0]
  0017d038: ldr r0,[r0,#0x0]
  0017d03a: blx 0x00072f70
  0017d03e: mov r1,r0
  0017d040: sub.w r0,r7,#0x74
  0017d044: blx 0x0006f2b0
  0017d048: ldr r0,[sp,#0x10]
  0017d04a: ldr.w r9,[r0,#0x0]
  0017d04e: add.w r0,sp,#0x6a0
  0017d052: sub.w r1,r7,#0x74
  0017d056: movs r2,#0x0
  0017d058: blx 0x0006f028
  0017d05c: vmov r6,s16
  0017d060: ldr r0,[0x0017d104]
  0017d062: add r0,pc
  0017d064: ldr r0,[r0,#0x0]
  0017d066: ldr r1,[r0,#0x0]
  0017d068: sub.w r0,r7,#0xb0
  0017d06c: mov r2,r6
  0017d06e: blx 0x00074e54
  0017d072: addw r0,sp,#0x694
  0017d076: sub.w r1,r7,#0xb0
  0017d07a: movs r2,#0x0
  0017d07c: blx 0x0006f028
  0017d080: ldr r1,[0x0017d108]
  0017d082: add r1,pc
  0017d084: add.w r0,sp,#0x688
  0017d088: movs r2,#0x0
  0017d08a: blx 0x0006ee18
  0017d08e: str r0,[sp,#0x0]
  0017d090: sub.w r0,r7,#0x98
  0017d094: add.w r2,sp,#0x6a0
  0017d098: addw r3,sp,#0x694
  0017d09c: mov r1,r9
  0017d09e: blx 0x000739b4
  0017d0a2: sub.w r0,r7,#0x74
  0017d0a6: sub.w r1,r7,#0x98
  0017d0aa: blx 0x0006f2b0
  0017d0ae: ldr.w r9,[sp,#0x18]
  0017d0b2: sub.w r0,r7,#0x98
  0017d0b6: blx r10
  0017d0b8: add.w r0,sp,#0x688
  0017d0bc: blx r10
  0017d0be: addw r0,sp,#0x694
  0017d0c2: blx r10
  0017d0c4: sub.w r0,r7,#0xb0
  0017d0c8: blx r10
  0017d0ca: add.w r0,sp,#0x6a0
  0017d0ce: blx r10
  0017d0d0: mov r0,r9
  0017d0d2: mov r1,r6
  0017d0d4: blx 0x00071bf0
  0017d0d8: b.w 0x0017c4f2
  0017d10c: ldr r0,[0x0017d36c]
  0017d10e: vmov.f32 s0,0x40000000
  0017d112: vmov.f32 s16,0x3f800000
  0017d116: add r0,pc
  0017d118: ldr r0,[r0,#0x0]
  0017d11a: ldrb.w r0,[r0,#0x38]
  0017d11e: cmp r0,#0x0
  0017d120: it eq
  0017d122: vmov.eq.f32 s16,s0
  0017d126: ldr.w r1,[r9,#0x28]
  0017d12a: adds r0,r1,#0x1
  0017d12c: bne 0x0017d140
  0017d12e: ldr r0,[0x0017d370]
  0017d130: add r0,pc
  0017d132: ldr r0,[r0,#0x0]
  0017d134: ldr r0,[r0,#0x0]
  0017d136: movs r1,#0x2
  0017d138: blx 0x00071848
  0017d13c: addw r1,r0,#0x36a
  0017d140: ldr r0,[0x0017d374]
  0017d142: str.w r1,[r9,#0x28]
  0017d146: add r0,pc
  0017d148: ldr.w r8,[r0,#0x0]
  0017d14c: ldr.w r0,[r8,#0x0]
  0017d150: blx 0x00072f70
  0017d154: mov r1,r0
  0017d156: sub.w r0,r7,#0x38
  0017d15a: blx 0x0006f2b0
  0017d15e: ldr r1,[0x0017d378]
  0017d160: add r1,pc
  0017d162: sub.w r0,r7,#0x68
  0017d166: movs r2,#0x0
  0017d168: blx 0x0006ee18
  0017d16c: ldr.w r5,[r8,#0x0]
  0017d170: mov r0,r9
  0017d172: blx 0x00074ba8
  0017d176: addw r1,r0,#0x376
  0017d17a: mov r0,r5
  0017d17c: blx 0x00072f70
  0017d180: mov r2,r0
  0017d182: sub.w r0,r7,#0x5c
  0017d186: sub.w r1,r7,#0x68
  0017d18a: blx 0x0006ef98
  0017d18e: sub.w r0,r7,#0x38
  0017d192: sub.w r1,r7,#0x5c
  0017d196: blx 0x0006ef5c
  0017d19a: sub.w r0,r7,#0x5c
  0017d19e: blx 0x0006ee30
  0017d1a2: sub.w r0,r7,#0x68
  0017d1a6: blx 0x0006ee30
  0017d1aa: ldr.w r5,[r9,#0x2c]
  0017d1ae: adds r0,r5,#0x1
  0017d1b0: bne 0x0017d1c4
  0017d1b2: ldr r0,[0x0017d37c]
  0017d1b4: add r0,pc
  0017d1b6: ldr r0,[r0,#0x0]
  0017d1b8: ldr r0,[r0,#0x0]
  0017d1ba: movs r1,#0x2
  0017d1bc: blx 0x00071848
  0017d1c0: addw r5,r0,#0x305
  0017d1c4: ldr r1,[0x0017d380]
  0017d1c6: str.w r5,[r9,#0x2c]
  0017d1ca: add r1,pc
  0017d1cc: sub.w r0,r7,#0x68
  0017d1d0: movs r2,#0x0
  0017d1d2: blx 0x0006ee18
  0017d1d6: ldr.w r0,[r8,#0x0]
  0017d1da: mov r1,r5
  0017d1dc: blx 0x00072f70
  0017d1e0: mov r2,r0
  0017d1e2: sub.w r0,r7,#0x5c
  0017d1e6: sub.w r1,r7,#0x68
  0017d1ea: blx 0x0006ef98
  0017d1ee: sub.w r0,r7,#0x38
  0017d1f2: sub.w r1,r7,#0x5c
  0017d1f6: blx 0x0006ef5c
  0017d1fa: sub.w r0,r7,#0x5c
  0017d1fe: blx 0x0006ee30
  0017d202: sub.w r0,r7,#0x68
  0017d206: blx 0x0006ee30
  0017d20a: ldr.w r6,[r10,#0x0]
  0017d20e: add r0,sp,#0x160
  0017d210: sub.w r1,r7,#0x38
  0017d214: movs r2,#0x0
  0017d216: blx 0x0006f028
  0017d21a: sub.w r0,r7,#0x68
  0017d21e: movs r1,#0x1
  0017d220: blx 0x0006f658
  0017d224: add r0,sp,#0x154
  0017d226: sub.w r1,r7,#0x68
  0017d22a: movs r2,#0x0
  0017d22c: blx 0x0006f028
  0017d230: ldr r1,[0x0017d384]
  0017d232: add r1,pc
  0017d234: add r0,sp,#0x148
  0017d236: movs r2,#0x0
  0017d238: blx 0x0006ee18
  0017d23c: str r0,[sp,#0x0]
  0017d23e: sub.w r0,r7,#0x5c
  0017d242: add r2,sp,#0x160
  0017d244: add r3,sp,#0x154
  0017d246: mov r1,r6
  0017d248: blx 0x000739b4
  0017d24c: sub.w r0,r7,#0x38
  0017d250: sub.w r1,r7,#0x5c
  0017d254: blx 0x0006f2b0
  0017d258: ldr r0,[0x0017d388]
  0017d25a: add r0,pc
  0017d25c: ldr r6,[r0,#0x0]
  0017d25e: sub.w r0,r7,#0x5c
  0017d262: blx r6
  0017d264: add r0,sp,#0x148
  0017d266: blx r6
  0017d268: add r0,sp,#0x154
  0017d26a: blx r6
  0017d26c: sub.w r0,r7,#0x68
  0017d270: blx r6
  0017d272: add r0,sp,#0x160
  0017d274: blx r6
  0017d276: mov r4,r10
  0017d278: ldr.w r10,[r10,#0x0]
  0017d27c: add r0,sp,#0x13c
  0017d27e: sub.w r1,r7,#0x38
  0017d282: movs r2,#0x0
  0017d284: blx 0x0006f028
  0017d288: ldr.w r5,[r8,#0x0]
  0017d28c: mov r0,r9
  0017d28e: blx 0x00074bb4
  0017d292: addw r1,r0,#0x391
  0017d296: mov r0,r5
  0017d298: blx 0x00072f70
  0017d29c: mov r1,r0
  0017d29e: add r0,sp,#0x130
  0017d2a0: movs r2,#0x0
  0017d2a2: blx 0x0006f028
  0017d2a6: ldr r1,[0x0017d38c]
  0017d2a8: add r1,pc
  0017d2aa: add r0,sp,#0x124
  0017d2ac: movs r2,#0x0
  0017d2ae: blx 0x0006ee18
  0017d2b2: str r0,[sp,#0x0]
  0017d2b4: sub.w r0,r7,#0x5c
  0017d2b8: add r2,sp,#0x13c
  0017d2ba: add r3,sp,#0x130
  0017d2bc: mov r1,r10
  0017d2be: blx 0x000739b4
  0017d2c2: sub.w r0,r7,#0x38
  0017d2c6: sub.w r1,r7,#0x5c
  0017d2ca: blx 0x0006f2b0
  0017d2ce: sub.w r0,r7,#0x5c
  0017d2d2: blx r6
  0017d2d4: add r0,sp,#0x124
  0017d2d6: blx r6
  0017d2d8: add r0,sp,#0x130
  0017d2da: blx r6
  0017d2dc: add r0,sp,#0x13c
  0017d2de: blx r6
  0017d2e0: ldr.w r8,[r4,#0x0]
  0017d2e4: add r0,sp,#0x118
  0017d2e6: sub.w r1,r7,#0x38
  0017d2ea: movs r2,#0x0
  0017d2ec: blx 0x0006f028
  0017d2f0: ldr r0,[0x0017d390]
  0017d2f2: add r0,pc
  0017d2f4: ldr r0,[r0,#0x0]
  0017d2f6: ldr r5,[r0,#0x0]
  0017d2f8: mov r0,r9
  0017d2fa: blx 0x00074bc0
  0017d2fe: vmov s0,r0
  0017d302: vcvt.f32.s32 s0,s0
  0017d306: vmul.f32 s0,s16,s0
  0017d30a: vcvt.s32.f32 s0,s0
  0017d30e: vmov r2,s0
  0017d312: sub.w r0,r7,#0x68
  0017d316: mov r1,r5
  0017d318: blx 0x00074e54
  0017d31c: add r0,sp,#0x10c
  0017d31e: sub.w r1,r7,#0x68
  0017d322: movs r2,#0x0
  0017d324: blx 0x0006f028
  0017d328: ldr r1,[0x0017d394]
  0017d32a: add r1,pc
  0017d32c: add r0,sp,#0x100
  0017d32e: movs r2,#0x0
  0017d330: blx 0x0006ee18
  0017d334: str r0,[sp,#0x0]
  0017d336: sub.w r0,r7,#0x5c
  0017d33a: add r2,sp,#0x118
  0017d33c: add r3,sp,#0x10c
  0017d33e: mov r1,r8
  0017d340: blx 0x000739b4
  0017d344: sub.w r0,r7,#0x38
  0017d348: sub.w r1,r7,#0x5c
  0017d34c: blx 0x0006f2b0
  0017d350: sub.w r0,r7,#0x5c
  0017d354: blx r6
  0017d356: add r0,sp,#0x100
  0017d358: blx r6
  0017d35a: add r0,sp,#0x10c
  0017d35c: blx r6
  0017d35e: sub.w r0,r7,#0x68
  0017d362: blx r6
  0017d364: add r0,sp,#0x118
  0017d366: blx r6
  0017d368: b 0x0017d53c
  0017d398: movs r1,#0x2
  0017d39a: blx 0x00071848
  0017d39e: ldr r1,[0x0017d78c]
  0017d3a0: add r1,pc
  0017d3a2: ldr r4,[r1,#0x0]
  0017d3a4: addw r1,r0,#0x36a
  0017d3a8: str.w r1,[r9,#0x28]
  0017d3ac: ldr r0,[r4,#0x0]
  0017d3ae: blx 0x00072f70
  0017d3b2: mov r1,r0
  0017d3b4: sub.w r0,r7,#0x38
  0017d3b8: blx 0x0006f2b0
  0017d3bc: ldr r1,[0x0017d790]
  0017d3be: add r1,pc
  0017d3c0: sub.w r0,r7,#0x68
  0017d3c4: movs r2,#0x0
  0017d3c6: blx 0x0006ee18
  0017d3ca: ldr r5,[r4,#0x0]
  0017d3cc: mov r0,r9
  0017d3ce: blx 0x00074ba8
  0017d3d2: addw r1,r0,#0x376
  0017d3d6: mov r0,r5
  0017d3d8: blx 0x00072f70
  0017d3dc: mov r2,r0
  0017d3de: sub.w r0,r7,#0x5c
  0017d3e2: sub.w r1,r7,#0x68
  0017d3e6: blx 0x0006ef98
  0017d3ea: sub.w r0,r7,#0x38
  0017d3ee: sub.w r1,r7,#0x5c
  0017d3f2: blx 0x0006ef5c
  0017d3f6: sub.w r0,r7,#0x5c
  0017d3fa: blx 0x0006ee30
  0017d3fe: sub.w r0,r7,#0x68
  0017d402: blx 0x0006ee30
  0017d406: ldr r1,[0x0017d794]
  0017d408: add r1,pc
  0017d40a: sub.w r0,r7,#0x68
  0017d40e: movs r2,#0x0
  0017d410: blx 0x0006ee18
  0017d414: ldr r0,[r4,#0x0]
  0017d416: mov.w r1,#0x36c
  0017d41a: blx 0x00072f70
  0017d41e: mov r2,r0
  0017d420: sub.w r0,r7,#0x5c
  0017d424: sub.w r1,r7,#0x68
  0017d428: blx 0x0006ef98
  0017d42c: sub.w r0,r7,#0x38
  0017d430: sub.w r1,r7,#0x5c
  0017d434: blx 0x0006ef5c
  0017d438: sub.w r0,r7,#0x5c
  0017d43c: blx 0x0006ee30
  0017d440: sub.w r0,r7,#0x68
  0017d444: blx 0x0006ee30
  0017d448: ldr.w r8,[r10,#0x0]
  0017d44c: add r0,sp,#0xac
  0017d44e: sub.w r1,r7,#0x38
  0017d452: movs r2,#0x0
  0017d454: blx 0x0006f028
  0017d458: ldr r0,[0x0017d798]
  0017d45a: ldr r5,[r4,#0x0]
  0017d45c: add r0,pc
  0017d45e: ldr r0,[r0,#0x0]
  0017d460: ldr r6,[r0,#0x0]
  0017d462: mov r0,r9
  0017d464: blx 0x00074b90
  0017d468: ldr r1,[r6,#0x4]
  0017d46a: ldr.w r0,[r1,r0,lsl #0x2]
  0017d46e: blx 0x000718d8
  0017d472: addw r1,r0,#0x4fa
  0017d476: mov r0,r5
  0017d478: blx 0x00072f70
  0017d47c: mov r1,r0
  0017d47e: add r0,sp,#0xa0
  0017d480: movs r2,#0x0
  0017d482: blx 0x0006f028
  0017d486: ldr r1,[0x0017d79c]
  0017d488: add r1,pc
  0017d48a: add r0,sp,#0x94
  0017d48c: movs r2,#0x0
  0017d48e: blx 0x0006ee18
  0017d492: str r0,[sp,#0x0]
  0017d494: sub.w r0,r7,#0x5c
  0017d498: add r2,sp,#0xac
  0017d49a: add r3,sp,#0xa0
  0017d49c: mov r1,r8
  0017d49e: blx 0x000739b4
  0017d4a2: sub.w r0,r7,#0x38
  0017d4a6: sub.w r1,r7,#0x5c
  0017d4aa: blx 0x0006f2b0
  0017d4ae: ldr r0,[0x0017d7a0]
  0017d4b0: add r0,pc
  0017d4b2: ldr r4,[r0,#0x0]
  0017d4b4: sub.w r0,r7,#0x5c
  0017d4b8: blx r4
  0017d4ba: add r0,sp,#0x94
  0017d4bc: blx r4
  0017d4be: add r0,sp,#0xa0
  0017d4c0: blx r4
  0017d4c2: add r0,sp,#0xac
  0017d4c4: blx r4
  0017d4c6: ldr.w r6,[r10,#0x0]
  0017d4ca: add r0,sp,#0x88
  0017d4cc: sub.w r1,r7,#0x38
  0017d4d0: movs r2,#0x0
  0017d4d2: blx 0x0006f028
  0017d4d6: ldr r0,[0x0017d7a4]
  0017d4d8: add r0,pc
  0017d4da: ldr r0,[r0,#0x0]
  0017d4dc: ldr r5,[r0,#0x0]
  0017d4de: mov r0,r9
  0017d4e0: blx 0x00074bc0
  0017d4e4: mov r2,r0
  0017d4e6: sub.w r0,r7,#0x68
  0017d4ea: mov r1,r5
  0017d4ec: blx 0x00074e54
  0017d4f0: add r0,sp,#0x7c
  0017d4f2: sub.w r1,r7,#0x68
  0017d4f6: movs r2,#0x0
  0017d4f8: blx 0x0006f028
  0017d4fc: ldr r1,[0x0017d7a8]
  0017d4fe: add r1,pc
  0017d500: add r0,sp,#0x70
  0017d502: movs r2,#0x0
  0017d504: blx 0x0006ee18
  0017d508: str r0,[sp,#0x0]
  0017d50a: sub.w r0,r7,#0x5c
  0017d50e: add r2,sp,#0x88
  0017d510: add r3,sp,#0x7c
  0017d512: mov r1,r6
  0017d514: blx 0x000739b4
  0017d518: sub.w r0,r7,#0x38
  0017d51c: sub.w r1,r7,#0x5c
  0017d520: blx 0x0006f2b0
  0017d524: sub.w r0,r7,#0x5c
  0017d528: blx r4
  0017d52a: add r0,sp,#0x70
  0017d52c: blx r4
  0017d52e: add r0,sp,#0x7c
  0017d530: blx r4
  0017d532: sub.w r0,r7,#0x68
  0017d536: blx r4
  0017d538: add r0,sp,#0x88
  0017d53a: blx r4
  0017d53c: ldr r1,[0x0017d7ac]
  0017d53e: add r1,pc
  0017d540: sub.w r0,r7,#0x68
  0017d544: movs r2,#0x0
  0017d546: blx 0x0006ee18
  0017d54a: ldr r0,[0x0017d7b0]
  0017d54c: ldr r1,[0x0017d7b4]
  0017d54e: add r0,pc
  0017d550: add r1,pc
  0017d552: ldr r0,[r0,#0x0]
  0017d554: ldr r1,[r1,#0x0]
  0017d556: ldr r5,[r0,#0x0]
  0017d558: ldr r0,[r1,#0x0]
  0017d55a: movs r1,#0x3
  0017d55c: blx 0x00071848
  0017d560: ldr r4,[sp,#0x18]
  0017d562: addw r1,r0,#0x349
  0017d566: mov r0,r5
  0017d568: blx 0x00072f70
  0017d56c: mov r2,r0
  0017d56e: sub.w r0,r7,#0x5c
  0017d572: sub.w r1,r7,#0x68
  0017d576: blx 0x0006ef98
  0017d57a: sub.w r0,r7,#0x38
  0017d57e: sub.w r1,r7,#0x5c
  0017d582: blx 0x0006ef5c
  0017d586: sub.w r0,r7,#0x5c
  0017d58a: blx 0x0006ee30
  0017d58e: sub.w r0,r7,#0x68
  0017d592: blx 0x0006ee30
  0017d596: ldr.w r5,[r11,#0x60]
  0017d59a: add r0,sp,#0x64
  0017d59c: mov r1,r4
  0017d59e: blx 0x00071c2c
  0017d5a2: add r0,sp,#0x58
  0017d5a4: sub.w r1,r7,#0x38
  0017d5a8: movs r2,#0x0
  0017d5aa: blx 0x0006f028
  0017d5ae: add r1,sp,#0x64
  0017d5b0: add r2,sp,#0x58
  0017d5b2: mov r0,r5
  0017d5b4: blx 0x00075b2c
  0017d5b8: add r0,sp,#0x58
  0017d5ba: blx 0x0006ee30
  0017d5be: add r0,sp,#0x64
  0017d5c0: blx 0x0006ee30
  0017d5c4: ldrd r0,r1,[r11,#0x20]
  0017d5c8: ldr r1,[r1,#0x4]
  0017d5ca: ldr.w r5,[r1,r0,lsl #0x2]
  0017d5ce: add r0,sp,#0x4c
  0017d5d0: sub.w r1,r7,#0x38
  0017d5d4: movs r2,#0x0
  0017d5d6: blx 0x0006f028
  0017d5da: add r1,sp,#0x4c
  0017d5dc: mov r0,r5
  0017d5de: blx 0x000749d4
  0017d5e2: add r0,sp,#0x4c
  0017d5e4: blx 0x0006ee30
  0017d5e8: movs r0,#0x2
  0017d5ea: str.w r0,[r11,#0x14]
  0017d5ee: movs r0,#0x0
  0017d5f0: str.w r0,[r11,#0x2c]
  0017d5f4: mov r0,r11
  0017d5f6: mov r1,r4
  0017d5f8: blx 0x00077d4c
  0017d5fc: mov r1,r0
  0017d5fe: cmp r1,#0x0
  0017d600: blt.w 0x0017ae08
  0017d604: ldr r0,[0x0017d7b8]
  0017d606: add r0,pc
  0017d608: ldr r0,[r0,#0x0]
  0017d60a: ldr r0,[r0,#0x0]
  0017d60c: movs r2,#0x0
  0017d60e: movs r3,#0x0
  0017d610: str r2,[sp,#0x0]
  0017d612: movs r2,#0x0
  0017d614: blx 0x00071548
  0017d618: b.w 0x0017ae08
  0017d61c: ldr r1,[0x0017d7bc]
  0017d61e: movs r5,#0x0
  0017d620: add r1,pc
  0017d622: sub.w r0,r7,#0x38
  0017d626: movs r2,#0x0
  0017d628: blx 0x0006ee18
  0017d62c: add.w r9,r11,#0xa4
  0017d630: sub.w r1,r7,#0x38
  0017d634: mov r0,r9
  0017d636: blx 0x0006f2b0
  0017d63a: sub.w r0,r7,#0x38
  0017d63e: blx 0x0006ee30
  0017d642: cbz r5,0x0017d67a
  0017d644: ldr r0,[0x0017d7c0]
  0017d646: add r0,pc
  0017d648: ldr r4,[r0,#0x0]
  0017d64a: sub.w r0,r7,#0xb0
  0017d64e: blx r4
  0017d650: sub.w r0,r7,#0x5c
  0017d654: blx r4
  0017d656: sub.w r0,r7,#0x74
  0017d65a: blx r4
  0017d65c: sub.w r0,r7,#0xa4
  0017d660: blx r4
  0017d662: sub.w r0,r7,#0x8c
  0017d666: blx r4
  0017d668: sub.w r0,r7,#0x98
  0017d66c: blx r4
  0017d66e: sub.w r0,r7,#0x80
  0017d672: blx r4
  0017d674: sub.w r0,r7,#0x68
  0017d678: blx r4
  0017d67a: ldr r0,[0x0017d7c4]
  0017d67c: sub.w r5,r7,#0x38
  0017d680: ldr r4,[sp,#0x18]
  0017d682: add r0,pc
  0017d684: ldr r0,[r0,#0x0]
  0017d686: mov r2,r4
  0017d688: ldr r1,[r0,#0x0]
  0017d68a: mov r0,r5
  0017d68c: blx 0x00077278
  0017d690: ldr.w r8,[r10,#0x0]
  0017d694: sub.w r0,r7,#0xc8
  0017d698: mov r1,r5
  0017d69a: movs r2,#0x0
  0017d69c: blx 0x0006f028
  0017d6a0: ldr r0,[0x0017d7c8]
  0017d6a2: add r0,pc
  0017d6a4: ldr r0,[r0,#0x0]
  0017d6a6: ldr r6,[r0,#0x0]
  0017d6a8: mov r0,r4
  0017d6aa: blx 0x00071bb4
  0017d6ae: blx 0x00071bc0
  0017d6b2: mov r5,r0
  0017d6b4: ldr r0,[sp,#0x18]
  0017d6b6: blx 0x00071bb4
  0017d6ba: blx 0x00074b78
  0017d6be: adds r2,r0,r5
  0017d6c0: sub.w r0,r7,#0xec
  0017d6c4: mov r1,r6
  0017d6c6: blx 0x00074e54
  0017d6ca: sub.w r0,r7,#0xe0
  0017d6ce: sub.w r1,r7,#0xec
  0017d6d2: movs r2,#0x0
  0017d6d4: blx 0x0006f028
  0017d6d8: sub.w r0,r7,#0xd4
  0017d6dc: sub.w r1,r7,#0xe0
  0017d6e0: mov r2,r9
  0017d6e2: blx 0x0006ef98
  0017d6e6: ldr r1,[0x0017d7cc]
  0017d6e8: add r1,pc
  0017d6ea: sub.w r0,r7,#0xf8
  0017d6ee: movs r2,#0x0
  0017d6f0: blx 0x0006ee18
  0017d6f4: ldr.w r9,[sp,#0x18]
  0017d6f8: str r0,[sp,#0x0]
  0017d6fa: sub.w r0,r7,#0xbc
  0017d6fe: sub.w r2,r7,#0xc8
  0017d702: sub.w r3,r7,#0xd4
  0017d706: mov r1,r8
  0017d708: blx 0x000739b4
  0017d70c: sub.w r0,r7,#0x38
  0017d710: sub.w r1,r7,#0xbc
  0017d714: blx 0x0006f2b0
  0017d718: ldr r0,[0x0017d7d0]
  0017d71a: add r0,pc
  0017d71c: ldr r4,[r0,#0x0]
  0017d71e: sub.w r0,r7,#0xbc
  0017d722: blx r4
  0017d724: sub.w r0,r7,#0xf8
  0017d728: blx r4
  0017d72a: sub.w r0,r7,#0xd4
  0017d72e: blx r4
  0017d730: sub.w r0,r7,#0xe0
  0017d734: blx r4
  0017d736: sub.w r0,r7,#0xec
  0017d73a: blx r4
  0017d73c: sub.w r0,r7,#0xc8
  0017d740: blx r4
  0017d742: ldr.w r5,[r11,#0x60]
  0017d746: addw r0,sp,#0x6f4
  0017d74a: mov r1,r9
  0017d74c: blx 0x00071c2c
  0017d750: add.w r0,sp,#0x6e8
  0017d754: sub.w r1,r7,#0x38
  0017d758: movs r2,#0x0
  0017d75a: blx 0x0006f028
  0017d75e: addw r1,sp,#0x6f4
  0017d762: add.w r2,sp,#0x6e8
  0017d766: mov r0,r5
  0017d768: blx 0x00075b2c
  0017d76c: add.w r0,sp,#0x6e8
  0017d770: blx r4
  0017d772: addw r0,sp,#0x6f4
  0017d776: b.w 0x0017b2e0
```
