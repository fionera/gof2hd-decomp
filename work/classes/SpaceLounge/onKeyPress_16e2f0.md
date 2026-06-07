# SpaceLounge::onKeyPress
elf 0x16e2f0 body 6538
Sig: undefined __thiscall onKeyPress(SpaceLounge * this, int param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* SpaceLounge::onKeyPress(int) */

void __thiscall SpaceLounge::onKeyPress(SpaceLounge *this,int param_1)

{
  SpaceLounge *pSVar1;
  byte bVar2;
  bool bVar3;
  int iVar4;
  Engine *this_00;
  uint uVar5;
  uint uVar6;
  StarMap *pSVar7;
  int *piVar8;
  String *pSVar9;
  undefined4 uVar10;
  int *piVar11;
  ListItemWindow *pLVar12;
  ListItem *this_01;
  Array *pAVar13;
  undefined4 uVar14;
  void *pvVar15;
  undefined4 *puVar16;
  Standing *this_02;
  int iVar17;
  Station *this_03;
  Item *pIVar18;
  Item *pIVar19;
  SolarSystem *this_04;
  undefined4 *puVar20;
  String *pSVar21;
  Mission *pMVar22;
  Agent *pAVar23;
  int *piVar24;
  undefined4 extraout_r3;
  uint *puVar25;
  code *pcVar26;
  Agent *pAVar27;
  Galaxy *this_05;
  EVP_PKEY_CTX *pEVar28;
  Status *pSVar29;
  Ship *pSVar30;
  int iVar31;
  ScrollTouchWindow *pSVar32;
  ChoiceWindow *pCVar33;
  undefined4 uVar34;
  SpaceLounge *pSVar35;
  int *piVar36;
  uint in_fpscr;
  float fVar37;
  float fVar38;
  float fVar39;
  undefined8 uVar40;
  String aSStack_3c4 [12];
  String aSStack_3b8 [12];
  String aSStack_3ac [12];
  String aSStack_3a0 [12];
  String aSStack_394 [12];
  String aSStack_388 [12];
  Agent aAStack_37c [12];
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
  String aSStack_184 [72];
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
  undefined1 auStack_94 [12];
  String aSStack_88 [12];
  undefined1 auStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  int local_34;
  
  piVar36 = *(int **)(DAT_0017e66c + 0x17e30c);
  local_34 = *piVar36;
  pSVar35 = this + 0x1b;
  if (*pSVar35 != (SpaceLounge)0x0) {
    if (param_1 == 0x1000) {
LAB_0017e4f2:
      ChoiceWindow::left();
    }
    else {
      if (param_1 != 0x2000) {
        if (param_1 == 0x10000) {
          *pSVar35 = (SpaceLounge)0x0;
        }
        goto switchD_0017e340_default;
      }
LAB_0017e4fc:
      ChoiceWindow::right();
    }
    goto switchD_0017e340_default;
  }
  switch(*(int *)(this + 0x14)) {
  case 0:
    if (param_1 == 0x1000) {
LAB_0017e528:
      uVar6 = *(int *)(this + 0x20) + 1;
      *(uint *)(this + 0x20) = uVar6;
      uVar5 = 0;
      if (uVar6 < **(uint **)(this + 0x24)) {
        uVar5 = uVar6;
      }
      *(uint *)(this + 0x20) = uVar5;
    }
    else if (param_1 == 0x2000) {
LAB_0017e546:
      iVar4 = *(int *)(this + 0x20);
      *(int *)(this + 0x20) = iVar4 + -1;
      if (iVar4 < 1) {
        *(int *)(this + 0x20) = **(int **)(this + 0x24) + -1;
      }
    }
    else {
      if (param_1 == 0x40000) goto LAB_0017e506;
      if (param_1 == 0x8000) goto LAB_0017e528;
      if (param_1 == 0x10000 || param_1 == 0x20000) {
        startChat(this);
      }
      else if (param_1 == 0x4000) goto LAB_0017e546;
    }
    goto switchD_0017e340_default;
  case 1:
  case 3:
    if (param_1 == 0x10000 || param_1 == 0x20000) {
      *(undefined4 *)(this + 0x30) = 0;
      pAVar27 = *(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4);
      iVar4 = Agent::getOffer(pAVar27);
      if (iVar4 == 1) {
        *(undefined4 *)(this + 0x14) = 2;
      }
      else {
        iVar4 = *(int *)(this + 0x14);
        uVar5 = (uint)(iVar4 == 1) << 1;
        *(uint *)(this + 0x14) = uVar5;
        if (iVar4 != 1) {
          uVar5 = (uint)(byte)this[0x18];
        }
        if (iVar4 != 1 && uVar5 != 0) {
          puVar25 = *(uint **)(DAT_0017e674 + 0x17e4ac);
          iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar25);
          pEVar28 = *(EVP_PKEY_CTX **)(iVar4 + 0x10);
          *(EVP_PKEY_CTX **)(this + 4) = pEVar28;
          if (pEVar28 == (EVP_PKEY_CTX *)0x0) {
            pSVar7 = operator_new(0x1e8);
            iVar4 = Agent::getSellSystemIndex(pAVar27);
            StarMap::StarMap(pSVar7,false,(Mission *)0x0,true,iVar4);
            iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar25);
            *(StarMap **)(iVar4 + 0x10) = pSVar7;
            iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar25);
            *(undefined4 *)(this + 4) = *(undefined4 *)(iVar4 + 0x10);
          }
          else {
            Agent::getSellSystemIndex(pAVar27);
            StarMap::init(pEVar28);
          }
          this[0x18] = (SpaceLounge)0x0;
          this[0x34] = (SpaceLounge)0x1;
        }
      }
    }
    else if (param_1 == 0x40000) {
      if (*(int *)(this + 0x2c) < 3) {
        if (*(int *)(this + 0x14) == 1) {
          *(undefined4 *)(this + 0x2c) = 0;
          *(undefined4 *)(this + 0x14) = 0;
        }
      }
      else {
        *(int *)(this + 0x2c) = *(int *)(this + 0x2c) + -0x28;
      }
    }
    goto switchD_0017e340_default;
  case 2:
    iVar4 = Agent::getMission(*(Agent **)
                               (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
    if (iVar4 == 0) {
LAB_0017e3da:
      iVar4 = Agent::getOffer(*(Agent **)
                               (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
      if ((((iVar4 == 2) ||
           (iVar4 = Agent::getOffer(*(Agent **)
                                     (*(int *)(*(int *)(this + 0x24) + 4) +
                                     *(int *)(this + 0x20) * 4)), iVar4 == 3)) ||
          (iVar4 = Agent::getOffer(*(Agent **)
                                    (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4
                                    )), iVar4 == 8)) ||
         ((iVar4 = Agent::getOffer(*(Agent **)
                                    (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4
                                    )), iVar4 == 9 ||
          (iVar4 = Agent::getOffer(*(Agent **)
                                    (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4
                                    )), iVar4 == 10)))) {
        iVar4 = 4;
      }
      else {
        iVar4 = 3;
      }
    }
    else {
      Agent::getMission(*(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4)
                       );
      iVar4 = Mission::isOutsideMission();
      if (iVar4 == 0) goto LAB_0017e3da;
      iVar4 = 5;
    }
    if (this[0x1c] != (SpaceLounge)0x0) {
      if (param_1 == 0x40000) {
        this[0x1c] = (SpaceLounge)0x0;
        this_00 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        AbyssEngine::Engine::SetPostEffect(this_00,0x1400000,(bool)this[0xb1]);
      }
      goto switchD_0017e340_default;
    }
    pSVar1 = this + 0x19;
    if (*pSVar1 == (SpaceLounge)0x0) {
      switch(param_1 - 0x4000U >> 0xe | param_1 << 0x12) {
      case 0:
        iVar17 = *(int *)(this + 0x30);
        if (*(int *)(this + 0x30) < 1) {
          iVar17 = iVar4;
        }
        *(int *)(this + 0x30) = iVar17 + -1;
        if (param_1 == 0x20000 || param_1 == 0x10000) goto switchD_0017e580_caseD_3;
        if (param_1 == 0x8000) {
          if (iVar4 <= iVar17) {
            iVar17 = 0;
          }
          *(int *)(this + 0x30) = iVar17;
        }
        break;
      case 1:
        iVar17 = *(int *)(this + 0x30) + 1;
        if (iVar4 <= iVar17) {
          iVar17 = 0;
        }
        *(int *)(this + 0x30) = iVar17;
        if (param_1 == 0x20000 || param_1 == 0x10000) goto switchD_0017e580_caseD_3;
        break;
      case 3:
      case 7:
switchD_0017e580_caseD_3:
        if (4 < *(uint *)(this + 0x30)) goto LAB_0017fdbc;
        pAVar27 = *(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4);
        switch(*(uint *)(this + 0x30)) {
        case 0:
          iVar4 = Agent::getOffer(pAVar27);
          AbyssEngine::String::String(aSStack_40,(char *)(DAT_0017ea44 + 0x17e7ba),false);
          switch(iVar4) {
          case 0:
          case 5:
            Agent::getMission(pAVar27);
            iVar4 = Mission::getType();
            if (iVar4 != 0) {
              Agent::getMission(pAVar27);
              iVar4 = Mission::getType();
              if (iVar4 == 0xb) {
                Agent::getMission(pAVar27);
                iVar4 = Mission::getProductionGoodAmount();
                puVar16 = *(undefined4 **)(DAT_0017f278 + 0x17f132);
                Status::getShip();
                iVar17 = Ship::getMaxPassengers();
                if (iVar17 < iVar4) {
                  pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017f27c + 0x17f148));
                  AbyssEngine::String::operator=(aSStack_40,pSVar9);
                  uVar14 = *puVar16;
                  AbyssEngine::String::String(aSStack_118,aSStack_40,false);
                  AbyssEngine::String::String(aSStack_58,iVar4);
                  AbyssEngine::String::String(aSStack_124,aSStack_58,false);
                  uVar10 = AbyssEngine::String::String
                                     (aSStack_130,(char *)(DAT_0017f280 + 0x17f17e),false);
                  Status::replaceHash(aSStack_4c,uVar14,aSStack_118,aSStack_124,uVar10);
                  AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
                  pcVar26 = *(code **)(DAT_0017f284 + 0x17f1a0);
                  (*pcVar26)(aSStack_4c);
                  (*pcVar26)(aSStack_130);
                  (*pcVar26)(aSStack_124);
                  (*pcVar26)(aSStack_58);
                  pSVar21 = aSStack_118;
                  goto LAB_0017fc3a;
                }
              }
LAB_0017f5d6:
              iVar4 = Agent::isGenericAgent(pAVar27);
              piVar8 = *(int **)(DAT_0017f728 + 0x17f5e6);
              iVar17 = *piVar8;
              if (iVar4 != 0) {
                pSVar9 = (String *)GameText::getText(iVar17);
                AbyssEngine::String::operator=(aSStack_40,pSVar9);
                AbyssEngine::String::String(aSStack_13c,aSStack_40,false);
                Agent::getMission(pAVar27);
                Mission::getReward();
                    /* WARNING: Bad instruction - Truncating control flow here */
                halt_baddata();
              }
              pSVar9 = (String *)GameText::getText(iVar17);
              AbyssEngine::String::operator=(aSStack_40,pSVar9);
              iVar4 = Status::getFreelanceMission();
              if ((iVar4 != 0) && (iVar4 = Mission::isEmpty(), iVar4 == 0)) {
                AbyssEngine::String::String(aSStack_58,(char *)(DAT_0017fae0 + 0x17f772),false);
                GameText::getText(*piVar8);
                AbyssEngine::operator+(aSStack_4c,aSStack_58);
                AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
                AbyssEngine::String::~String(aSStack_4c);
                AbyssEngine::String::~String(aSStack_58);
              }
              goto switchD_0017e7c2_default;
            }
            Agent::getMission(pAVar27);
            iVar4 = Mission::getProductionGoodAmount();
            puVar16 = *(undefined4 **)(DAT_0017f724 + 0x17f5c4);
            pSVar30 = (Ship *)Status::getShip();
            iVar17 = Ship::spaceAvailable(pSVar30,iVar4);
            if (iVar17 != 0) goto LAB_0017f5d6;
            pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017fecc + 0x17fbd0));
            AbyssEngine::String::operator=(aSStack_40,pSVar9);
            uVar14 = *puVar16;
            AbyssEngine::String::String(aSStack_f4,aSStack_40,false);
            AbyssEngine::String::String(aSStack_58,iVar4);
            AbyssEngine::String::String(aSStack_100,aSStack_58,false);
            uVar10 = AbyssEngine::String::String
                               (aSStack_10c,(char *)(DAT_0017fed0 + 0x17fc06),false);
            Status::replaceHash(aSStack_4c,uVar14,aSStack_f4,aSStack_100,uVar10);
            AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
            pcVar26 = *(code **)(DAT_0017fed4 + 0x17fc28);
            (*pcVar26)(aSStack_4c);
            (*pcVar26)(aSStack_10c);
            (*pcVar26)(aSStack_100);
            (*pcVar26)(aSStack_58);
            pSVar21 = aSStack_f4;
LAB_0017fc3a:
            (*pcVar26)(pSVar21);
            ChoiceWindow::set(*(ChoiceWindow **)(this + 8),aSStack_40,false);
            goto LAB_0017fdb0;
          case 1:
            *(undefined4 *)(this + 0x14) = 0;
            goto LAB_0017e8e6;
          case 2:
          case 3:
          case 4:
          case 8:
          case 9:
          case 10:
            fVar38 = 1.0;
            if (*(char *)(*(int *)(DAT_0017ea48 + 0x17e7ea) + 0x38) == '\0') {
              fVar38 = 2.0;
            }
            fVar37 = 1.0;
            if (iVar4 - 8U < 3) {
              fVar37 = fVar38;
            }
            puVar16 = *(undefined4 **)(DAT_0017ea4c + 0x17e7fe);
            uVar10 = Status::getCredits();
            uVar14 = Agent::getSellItemPrice(pAVar27);
            fVar38 = (float)VectorSignedToFloat(uVar14,(byte)(in_fpscr >> 0x16) & 3);
            fVar39 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
            fVar37 = fVar37 * fVar38;
            uVar5 = in_fpscr & 0xfffffff | (uint)(fVar37 < fVar39) << 0x1f |
                    (uint)(fVar37 == fVar39) << 0x1e;
            uVar6 = uVar5 | (uint)(NAN(fVar37) || NAN(fVar39)) << 0x1c;
            bVar2 = (byte)(uVar5 >> 0x18);
            if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar6 >> 0x1c) & 1)) {
              pCVar33 = *(ChoiceWindow **)(this + 8);
              uVar34 = *puVar16;
              pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017ea50 + 0x17e844));
              AbyssEngine::String::String(aSStack_184,pSVar9,false);
              uVar10 = Agent::getSellItemPrice(pAVar27);
              uVar14 = Status::getCredits();
              VectorSignedToFloat(uVar10,(byte)(uVar6 >> 0x16) & 3);
              VectorSignedToFloat(uVar14,(byte)(uVar6 >> 0x16) & 3);
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_190,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_19c,(char *)(DAT_0017ea58 + 0x17e8aa),false);
              Status::replaceHash(aSStack_4c,uVar34,aSStack_184,aSStack_190,uVar10);
              ChoiceWindow::set(pCVar33,aSStack_4c,false);
              pcVar26 = *(code **)(DAT_0017ea5c + 0x17e8ce);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_19c);
              (*pcVar26)(aSStack_190);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_184;
              goto LAB_0017e8e0;
            }
            switch(iVar4) {
            case 2:
            case 9:
              piVar8 = *(int **)(DAT_0017f25c + 0x17eece);
              pSVar9 = (String *)GameText::getText(*piVar8);
              AbyssEngine::String::operator=(aSStack_40,pSVar9);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_280,aSStack_40,false);
              iVar4 = Agent::getSellItemQuantity(pAVar27);
              AbyssEngine::String::String(aSStack_58,iVar4);
              AbyssEngine::String::String(aSStack_28c,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_298,(char *)(DAT_0017f260 + 0x17ef0c),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_280,aSStack_28c,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              pcVar26 = *(code **)(DAT_0017f264 + 0x17ef2e);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_298);
              (*pcVar26)(aSStack_28c);
              (*pcVar26)(aSStack_58);
              (*pcVar26)(aSStack_280);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_2a4,aSStack_40,false);
              iVar4 = *piVar8;
              Agent::getSellItemIndex(pAVar27);
              pSVar9 = (String *)GameText::getText(iVar4);
              AbyssEngine::String::String(aSStack_2b0,pSVar9,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_2bc,(char *)(DAT_0017f268 + 0x17ef72),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_2a4,aSStack_2b0,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_2bc);
              (*pcVar26)(aSStack_2b0);
              (*pcVar26)(aSStack_2a4);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_2c8,aSStack_40,false);
              uVar10 = Agent::getSellItemPrice(pAVar27);
              VectorSignedToFloat(uVar10,(byte)(uVar6 >> 0x16) & 3);
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_2d4,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_2e0,(char *)(DAT_0017f270 + 0x17efe6),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_2c8,aSStack_2d4,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_2e0);
              (*pcVar26)(aSStack_2d4);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_2c8;
              break;
            case 3:
              piVar8 = *(int **)(DAT_0017faf4 + 0x17f866);
              pSVar9 = (String *)GameText::getText(*piVar8);
              AbyssEngine::String::operator=(aSStack_40,pSVar9);
              uVar10 = *puVar16;
              AbyssEngine::String::String(aSStack_1f0,aSStack_40,false);
              iVar31 = *piVar8;
              iVar17 = **(int **)(DAT_0017faf8 + 0x17f890);
              iVar4 = Agent::getSellBlueprintIndex(pAVar27);
              Item::getIndex(*(Item **)(*(int *)(iVar17 + 4) + iVar4 * 4));
              pSVar9 = (String *)GameText::getText(iVar31);
              AbyssEngine::String::String(aSStack_1fc,pSVar9,false);
              uVar14 = AbyssEngine::String::String
                                 (aSStack_208,(char *)(DAT_0017fafc + 0x17f8bc),false);
              Status::replaceHash(aSStack_4c,uVar10,aSStack_1f0,aSStack_1fc,uVar14);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              pcVar26 = *(code **)(DAT_0017fb00 + 0x17f8de);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_208);
              (*pcVar26)(aSStack_1fc);
              (*pcVar26)(aSStack_1f0);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_214,aSStack_40,false);
              Agent::getSellItemPrice(pAVar27);
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_220,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_22c,(char *)(DAT_0017fb08 + 0x17f924),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_214,aSStack_220,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_22c);
              (*pcVar26)(aSStack_220);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_214;
              break;
            case 4:
              pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017fb0c + 0x17f95a));
              AbyssEngine::String::operator=(aSStack_40,pSVar9);
              this_05 = (Galaxy *)**(undefined4 **)(DAT_0017fb10 + 0x17f972);
              iVar4 = Agent::getSellSystemIndex(pAVar27);
              Galaxy::getSystem(this_05,iVar4);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_238,aSStack_40,false);
              SolarSystem::getName();
              AbyssEngine::String::String(aSStack_244,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_250,(char *)(DAT_0017fb14 + 0x17f9aa),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_238,aSStack_244,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              pcVar26 = *(code **)(DAT_0017fb18 + 0x17f9cc);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_250);
              (*pcVar26)(aSStack_244);
              (*pcVar26)(aSStack_58);
              (*pcVar26)(aSStack_238);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_25c,aSStack_40,false);
              Agent::getSellItemPrice(pAVar27);
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_268,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_274,(char *)(DAT_0017fb20 + 0x17fa16),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_25c,aSStack_268,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_274);
              (*pcVar26)(aSStack_268);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_25c;
              break;
            default:
              goto switchD_0017e7c2_default;
            case 8:
              pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017fb24 + 0x17fa4c));
              AbyssEngine::String::operator=(aSStack_40,pSVar9);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_1a8,aSStack_40,false);
              uVar10 = Agent::getSellItemPrice(pAVar27);
              VectorSignedToFloat(uVar10,(byte)(uVar6 >> 0x16) & 3);
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_1b4,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_1c0,(char *)(DAT_0017fb2c + 0x17faa6),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_1a8,aSStack_1b4,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              pcVar26 = *(code **)(DAT_0017fb30 + 0x17fac8);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_1c0);
              (*pcVar26)(aSStack_1b4);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_1a8;
              break;
            case 10:
              pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017febc + 0x17fb3a));
              AbyssEngine::String::operator=(aSStack_40,pSVar9);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_1cc,aSStack_40,false);
              uVar10 = Agent::getSellItemPrice(pAVar27);
              VectorSignedToFloat(uVar10,(byte)(uVar6 >> 0x16) & 3);
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_1d8,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_1e4,(char *)(DAT_0017fec4 + 0x17fb94),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_1cc,aSStack_1d8,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              pcVar26 = *(code **)(DAT_0017fec8 + 0x17fbb6);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_1e4);
              (*pcVar26)(aSStack_1d8);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_1cc;
            }
LAB_0017fda2:
            (*pcVar26)(pSVar21);
          default:
switchD_0017e7c2_default:
            ChoiceWindow::set(*(ChoiceWindow **)(this + 8),aSStack_40,true);
            pSVar35 = pSVar1;
LAB_0017fdb0:
            *pSVar35 = (SpaceLounge)0x1;
            AbyssEngine::String::~String(aSStack_40);
            break;
          case 6:
            puVar16 = *(undefined4 **)(DAT_0017f6ec + 0x17f3e0);
            iVar4 = Achievements::gotAllMedals((Achievements *)*puVar16);
            if (iVar4 == 0) {
              uVar5 = Status::getCredits();
              uVar40 = Agent::getCosts(pAVar27);
              puVar20 = (undefined4 *)((ulonglong)uVar40 >> 0x20);
              uVar6 = (uint)uVar40;
                    /* WARNING: Could not recover jumptable at 0x0017f404. Too many branches */
                    /* WARNING: Treating indirect jump as call */
              if (uVar6 < uVar5) {
                (*(code *)(pSVar1 + (uint)(uVar6 <= uVar5) + uVar6 * 0x20))();
                return;
              }
              if (uVar6 > uVar5 || uVar5 == uVar6) {
                puVar16[-1] = 0x17f403;
                puVar16[-2] = piVar36;
                puVar16[-3] = &stack0xfffffff8;
                puVar16[-4] = uVar5;
                puVar16[-5] = pSVar35;
                puVar16[-6] = extraout_r3;
                puVar16[-7] = puVar20;
              }
              pCVar33 = *(ChoiceWindow **)(this + 8);
              uVar14 = *puVar20;
              pSVar9 = (String *)GameText::getText(**(int **)(&UNK_0017f410 + uVar6));
              AbyssEngine::String::String(aSStack_2ec,pSVar9,false);
              Agent::getCosts(pAVar27);
              Status::getCredits();
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_2f8,aSStack_58,false);
              uVar10 = AbyssEngine::String::String(aSStack_304,&UNK_0017f45e + _UNK_0017f6fc,false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_2ec,aSStack_2f8,uVar10);
              ChoiceWindow::set(pCVar33,aSStack_4c,false);
              pcVar26 = *(code **)(&UNK_0017f482 + _UNK_0017f700);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_304);
              (*pcVar26)(aSStack_2f8);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_2ec;
              goto LAB_0017e8e0;
            }
            puVar20 = *(undefined4 **)(DAT_0017f71c + 0x17f582);
            iVar4 = Status::getWingmen();
            if (iVar4 == 0) {
              iVar4 = **(int **)(DAT_0017fed8 + 0x17fcba);
              Achievements::gotAllMedals((Achievements *)*puVar16);
              pSVar9 = (String *)GameText::getText(iVar4);
              AbyssEngine::String::operator=(aSStack_40,pSVar9);
              uVar14 = *puVar20;
              AbyssEngine::String::String(aSStack_310,aSStack_40,false);
              iVar4 = Agent::getWingmanFriendsCount(pAVar27);
              AbyssEngine::String::String(aSStack_58,iVar4 + 1);
              AbyssEngine::String::String(aSStack_31c,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_328,(char *)(DAT_0017fedc + 0x17fd08),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_310,aSStack_31c,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              pcVar26 = *(code **)(DAT_0017fee0 + 0x17fd2a);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_328);
              (*pcVar26)(aSStack_31c);
              (*pcVar26)(aSStack_58);
              (*pcVar26)(aSStack_310);
              uVar14 = *puVar20;
              AbyssEngine::String::String(aSStack_334,aSStack_40,false);
              Agent::getCosts(pAVar27);
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_340,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_34c,(char *)(DAT_0017fee8 + 0x17fd74),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_334,aSStack_340,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_34c);
              (*pcVar26)(aSStack_340);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_334;
              goto LAB_0017fda2;
            }
            pCVar33 = *(ChoiceWindow **)(this + 8);
            pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017f720 + 0x17f59a));
            ChoiceWindow::set(pCVar33,pSVar9,false);
            goto LAB_0017e8e2;
          case 7:
            puVar16 = *(undefined4 **)(DAT_0017f704 + 0x17f49e);
            iVar4 = Status::getCredits();
            iVar17 = Agent::getCosts(pAVar27);
            if (iVar17 <= iVar4) {
              pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017fae4 + 0x17f7e4));
              AbyssEngine::String::operator=(aSStack_40,pSVar9);
              uVar14 = *puVar16;
              AbyssEngine::String::String(aSStack_d0,aSStack_40,false);
              Agent::getCosts(pAVar27);
              Layout::formatCredits((int)aSStack_58);
              AbyssEngine::String::String(aSStack_dc,aSStack_58,false);
              uVar10 = AbyssEngine::String::String
                                 (aSStack_e8,(char *)(DAT_0017faec + 0x17f82a),false);
              Status::replaceHash(aSStack_4c,uVar14,aSStack_d0,aSStack_dc,uVar10);
              AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
              pcVar26 = *(code **)(DAT_0017faf0 + 0x17f84c);
              (*pcVar26)(aSStack_4c);
              (*pcVar26)(aSStack_e8);
              (*pcVar26)(aSStack_dc);
              (*pcVar26)(aSStack_58);
              pSVar21 = aSStack_d0;
              goto LAB_0017fda2;
            }
            pCVar33 = *(ChoiceWindow **)(this + 8);
            uVar14 = *puVar16;
            pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017f708 + 0x17f4bc));
            AbyssEngine::String::String(aSStack_ac,pSVar9,false);
            Agent::getCosts(pAVar27);
            Status::getCredits();
            Layout::formatCredits((int)aSStack_58);
            AbyssEngine::String::String(aSStack_b8,aSStack_58,false);
            uVar10 = AbyssEngine::String::String(aSStack_c4,(char *)(DAT_0017f710 + 0x17f506),false)
            ;
            Status::replaceHash(aSStack_4c,uVar14,aSStack_ac,aSStack_b8,uVar10);
            ChoiceWindow::set(pCVar33,aSStack_4c,false);
            pcVar26 = *(code **)(DAT_0017f714 + 0x17f52a);
            (*pcVar26)(aSStack_4c);
            (*pcVar26)(aSStack_c4);
            (*pcVar26)(aSStack_b8);
            (*pcVar26)(aSStack_58);
            pSVar21 = aSStack_ac;
LAB_0017e8e0:
            (*pcVar26)(pSVar21);
LAB_0017e8e2:
            *pSVar35 = (SpaceLounge)0x1;
LAB_0017e8e6:
            AbyssEngine::String::~String(aSStack_40);
            goto switchD_0017e340_default;
          }
          break;
        case 1:
          pSVar32 = *(ScrollTouchWindow **)(this + 0x60);
          Agent::getName();
          iVar4 = **(int **)(DAT_0017ea6c + 0x17e9d8);
          AbyssEngine::AERandom::nextInt(**(int **)(DAT_0017ea70 + 0x17e9da));
          pSVar9 = (String *)GameText::getText(iVar4);
          AbyssEngine::String::String(aSStack_364,pSVar9,false);
          ScrollTouchWindow::setText(pSVar32,aSStack_358,aSStack_364);
          AbyssEngine::String::~String(aSStack_364);
          AbyssEngine::String::~String(aSStack_358);
          iVar4 = DAT_0017ea74;
          *(undefined4 *)(this + 0x2c) = 0;
          *(undefined4 *)(this + 0x14) = 3;
          *(int *)(**(int **)(iVar4 + 0x17ea1e) + 0xe0) =
               *(int *)(**(int **)(iVar4 + 0x17ea1e) + 0xe0) + 1;
          break;
        case 2:
          pSVar32 = *(ScrollTouchWindow **)(this + 0x60);
          *(undefined4 *)(this + 0x14) = 2;
          Agent::getName();
          Globals::getAgentMissionText(aAStack_37c);
          ScrollTouchWindow::setText(pSVar32,aSStack_370,aAStack_37c);
          AbyssEngine::String::~String((String *)aAStack_37c);
          AbyssEngine::String::~String(aSStack_370);
          iVar4 = DAT_0017ea64;
          *(undefined4 *)(this + 0x2c) = 0;
          *(int *)(**(int **)(iVar4 + 0x17e936) + 0xe4) =
               *(int *)(**(int **)(iVar4 + 0x17e936) + 0xe4) + 1;
          break;
        case 3:
          iVar4 = Agent::getOffer(pAVar27);
          if ((iVar4 != 2) &&
             (iVar4 = Agent::getOffer(*(Agent **)
                                       (*(int *)(*(int *)(this + 0x24) + 4) +
                                       *(int *)(this + 0x20) * 4)), iVar4 != 3)) {
            pAVar27 = *(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4);
            pAVar27[0x25] = (Agent)0x1;
            Agent::getMission(pAVar27);
            iVar4 = Mission::getTargetStation();
            this_03 = (Station *)Status::getStation();
            iVar17 = Station::getIndex(this_03);
            if (iVar4 == iVar17) {
              pSVar32 = *(ScrollTouchWindow **)(this + 0x60);
              Agent::getName();
              pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017eec4 + 0x17ee38));
              AbyssEngine::String::String(aSStack_394,pSVar9,false);
              ScrollTouchWindow::setText(pSVar32,aSStack_388,aSStack_394);
              AbyssEngine::String::~String(aSStack_394);
              pSVar21 = aSStack_388;
            }
            else {
              this_04 = (SolarSystem *)Status::getSystem();
              Agent::getMission(*(Agent **)
                                 (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
              iVar4 = Mission::getTargetStation();
              iVar4 = SolarSystem::stationIsInSystem(this_04,iVar4);
              if (iVar4 == 0) {
                puVar25 = *(uint **)(DAT_0017f718 + 0x17f548);
                iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar25);
                pEVar28 = *(EVP_PKEY_CTX **)(iVar4 + 0x10);
                *(EVP_PKEY_CTX **)(this + 4) = pEVar28;
                if (pEVar28 == (EVP_PKEY_CTX *)0x0) {
                  pSVar7 = operator_new(0x1e8);
                  pMVar22 = (Mission *)
                            Agent::getMission(*(Agent **)
                                               (*(int *)(*(int *)(this + 0x24) + 4) +
                                               *(int *)(this + 0x20) * 4));
                  StarMap::StarMap(pSVar7,true,pMVar22,false,-1);
                  iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar25);
                  *(StarMap **)(iVar4 + 0x10) = pSVar7;
                  iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar25);
                  *(undefined4 *)(this + 4) = *(undefined4 *)(iVar4 + 0x10);
                }
                else {
                  Agent::getMission(*(Agent **)
                                     (*(int *)(*(int *)(this + 0x24) + 4) +
                                     *(int *)(this + 0x20) * 4));
                  StarMap::init(pEVar28);
                }
                this[0x34] = (SpaceLounge)0x1;
                break;
              }
              pSVar32 = *(ScrollTouchWindow **)(this + 0x60);
              Agent::getName();
              pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017f6e8 + 0x17f394));
              AbyssEngine::String::String(aSStack_3ac,pSVar9,false);
              ScrollTouchWindow::setText(pSVar32,aSStack_3a0,aSStack_3ac);
              AbyssEngine::String::~String(aSStack_3ac);
              pSVar21 = aSStack_3a0;
            }
            goto LAB_0017e9b0;
          }
          if (*(int *)(this + 0xc) == 0) {
            pLVar12 = operator_new(0x13c);
            ListItemWindow::ListItemWindow(pLVar12);
            *(ListItemWindow **)(this + 0xc) = pLVar12;
          }
          iVar4 = Agent::getOffer(*(Agent **)
                                   (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4)
                                 );
          if (iVar4 == 2) {
            pLVar12 = *(ListItemWindow **)(this + 0xc);
            this_01 = operator_new(0x48);
            iVar17 = **(int **)(DAT_0017ee80 + 0x17ead6);
            iVar4 = Agent::getSellItemIndex
                              (*(Agent **)
                                (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
            ::ListItem::ListItem(this_01,*(Item **)(*(int *)(iVar17 + 4) + iVar4 * 4));
LAB_0017f058:
            ListItemWindow::set(pLVar12,this_01,0,0,0,0,false);
          }
          else {
            puVar16 = *(undefined4 **)(DAT_0017ee84 + 0x17eb04);
            for (uVar5 = 0; puVar25 = (uint *)Status::getBluePrints((Status *)*puVar16),
                uVar5 < *puVar25; uVar5 = uVar5 + 1) {
              iVar4 = Status::getBluePrints((Status *)*puVar16);
              iVar4 = BluePrint::getIndex(*(BluePrint **)(*(int *)(iVar4 + 4) + uVar5 * 4));
              iVar17 = Agent::getSellBlueprintIndex
                                 (*(Agent **)
                                   (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4)
                                 );
              if (iVar4 == iVar17) {
                pLVar12 = *(ListItemWindow **)(this + 0xc);
                this_01 = operator_new(0x48);
                iVar17 = **(int **)(DAT_0017f274 + 0x17f02a);
                iVar4 = Status::getBluePrints((Status *)*puVar16);
                iVar4 = BluePrint::getIndex(*(BluePrint **)(*(int *)(iVar4 + 4) + uVar5 * 4));
                ::ListItem::ListItem(this_01,*(Item **)(*(int *)(iVar17 + 4) + iVar4 * 4));
                goto LAB_0017f058;
              }
            }
          }
          this[0x1c] = (SpaceLounge)0x1;
          break;
        case 4:
          pAVar27[0x24] = (Agent)0x1;
          pMVar22 = (Mission *)Agent::getMission(pAVar27);
          uVar10 = Mission::getDifficulty(pMVar22);
          pSVar32 = *(ScrollTouchWindow **)(this + 0x60);
          Agent::getName();
          VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
          pSVar9 = (String *)GameText::getText(**(int **)(DAT_0017ea68 + 0x17e976));
          AbyssEngine::String::String(aSStack_3c4,pSVar9,false);
          ScrollTouchWindow::setText(pSVar32,aSStack_3b8,aSStack_3c4);
          AbyssEngine::String::~String(aSStack_3c4);
          pSVar21 = aSStack_3b8;
LAB_0017e9b0:
          AbyssEngine::String::~String(pSVar21);
          *(undefined4 *)(this + 0x2c) = 0;
          *(undefined4 *)(this + 0x14) = 2;
        }
LAB_0017fdbc:
        iVar4 = Agent::isGenericAgent
                          (*(Agent **)
                            (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
        if (iVar4 != 0) {
          Agent::setEvent(*(Agent **)
                           (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4),1);
        }
      }
      goto switchD_0017e340_default;
    }
    if (param_1 != 0x10000) {
      if (param_1 == 0x2000) goto LAB_0017e4fc;
      if (param_1 == 0x1000) goto LAB_0017e4f2;
      goto switchD_0017e340_default;
    }
    break;
  default:
    goto switchD_0017e340_default;
  }
  piVar8 = *(int **)(DAT_0017e678 + 0x17e616);
  piVar24 = *(int **)(DAT_0017e67c + 0x17e618);
  iVar4 = *piVar8;
  AbyssEngine::AERandom::nextInt(*piVar24);
  pSVar9 = (String *)GameText::getText(iVar4);
  AbyssEngine::String::String(aSStack_40,pSVar9,false);
  pAVar27 = *(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4);
  uVar10 = Agent::getOffer(pAVar27);
  switch(uVar10) {
  case 0:
  case 5:
    piVar11 = *(int **)(DAT_0017ea34 + 0x17e686);
    iVar4 = Status::hardCoreMode();
    if (iVar4 != 0) {
      Agent::getMission(pAVar27);
      iVar4 = Mission::getReward();
      Agent::getMission(pAVar27);
      iVar17 = Mission::getBonus();
      iVar4 = __aeabi_idiv(iVar17 + iVar4,10);
      iVar17 = Status::getCredits();
      if (iVar17 < iVar4) {
        pSVar9 = (String *)GameText::getText(*piVar8);
        AbyssEngine::String::String(aSStack_4c,pSVar9,false);
        iVar4 = *piVar11;
        AbyssEngine::String::String(aSStack_64,aSStack_4c,false);
        Status::getCredits();
        Layout::formatCredits((int)auStack_7c);
        AbyssEngine::String::String(aSStack_70,auStack_7c,false);
        uVar10 = AbyssEngine::String::String(aSStack_88,(char *)(DAT_0017ea3c + 0x17e70e),false);
        Status::replaceHash(aSStack_58,iVar4,aSStack_64,aSStack_70,uVar10);
        AbyssEngine::String::operator=(aSStack_4c,aSStack_58);
        pcVar26 = *(code **)(DAT_0017ea40 + 0x17e730);
        (*pcVar26)(aSStack_58);
        (*pcVar26)(aSStack_88);
        (*pcVar26)(aSStack_70);
        (*pcVar26)(auStack_7c);
        (*pcVar26)(aSStack_64);
        ChoiceWindow::set(*(ChoiceWindow **)(this + 8),aSStack_4c,false);
        *pSVar35 = (SpaceLounge)0x1;
        *pSVar1 = (SpaceLounge)0x0;
        AbyssEngine::String::~String(aSStack_4c);
        AbyssEngine::String::~String(aSStack_40);
        goto LAB_0017e506;
      }
      Status::changeCredits(*piVar11);
    }
    iVar4 = Status::getFreelanceMission();
    if (iVar4 != 0) {
      Status::getFreelanceMission();
      iVar4 = Mission::isEmpty();
      if (iVar4 == 0) {
        Status::getFreelanceMission();
        iVar4 = Mission::getType();
        if (iVar4 == 0) {
LAB_0017f0c0:
          Status::getShip();
          puVar25 = (uint *)Ship::getCargo();
          if (puVar25 != (uint *)0x0) {
            for (uVar5 = 0; uVar5 < *puVar25; uVar5 = uVar5 + 1) {
              iVar4 = Item::isUnsaleable(*(Item **)(puVar25[1] + uVar5 * 4));
              if ((iVar4 != 0) &&
                 (iVar4 = Item::getIndex(*(Item **)(puVar25[1] + uVar5 * 4)), iVar4 == 0x74)) {
                pIVar18 = (Item *)Status::getShip();
                Ship::removeCargo(pIVar18);
                this[0x35] = (SpaceLounge)0x1;
                break;
              }
            }
          }
        }
        else {
          Status::getFreelanceMission();
          iVar4 = Mission::getType();
          if (iVar4 == 3) goto LAB_0017f0c0;
          Status::getFreelanceMission();
          iVar4 = Mission::getType();
          if (iVar4 == 5) goto LAB_0017f0c0;
          Status::getFreelanceMission();
          iVar4 = Mission::getType();
          if (iVar4 == 0xb) {
            Status::setPassengers((Status *)*piVar11,0);
          }
        }
        pMVar22 = (Mission *)Status::getFreelanceMission();
        pAVar23 = (Agent *)Mission::getAgent(pMVar22);
        iVar4 = Agent::isGenericAgent(pAVar23);
        if (iVar4 == 0) {
          pMVar22 = (Mission *)Status::getFreelanceMission();
          pAVar23 = (Agent *)Mission::getAgent(pMVar22);
          Agent::setOfferAccepted(pAVar23,false);
        }
        Status::setFreelanceMission((Mission *)*piVar11);
      }
    }
    Agent::getMission(pAVar27);
    iVar4 = Mission::getType();
    if (iVar4 == 0) {
      iVar4 = *(int *)(*(int *)(**(int **)(DAT_0017fef0 + 0x17fe68) + 4) + 0x1d0);
      Agent::getMission(pAVar27);
      Mission::getProductionGoodAmount();
      bVar3 = (bool)Item::makeItem(iVar4);
      Item::setUnsaleable(bVar3);
      pIVar18 = (Item *)Status::getShip();
      Ship::addCargo(pIVar18);
    }
    else {
      Agent::getMission(pAVar27);
      iVar4 = Mission::getType();
      if (iVar4 == 0xb) {
        pSVar29 = (Status *)*piVar11;
        Agent::getMission(pAVar27);
        iVar4 = Mission::getProductionGoodAmount();
        Status::setPassengers(pSVar29,iVar4);
      }
    }
    Agent::getMission(pAVar27);
    iVar4 = Mission::getType();
    if (iVar4 == 0xc) {
      pSVar9 = (String *)GameText::getText(*piVar8);
      AbyssEngine::String::operator=(aSStack_40,pSVar9);
    }
    else {
      AbyssEngine::String::String(aSStack_58,(char *)(DAT_00180248 + 0x17fefa),false);
      iVar4 = *piVar8;
      AbyssEngine::AERandom::nextInt(*piVar24);
      GameText::getText(iVar4);
      AbyssEngine::operator+(aSStack_4c,aSStack_58);
      AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
      AbyssEngine::String::~String(aSStack_4c);
      AbyssEngine::String::~String(aSStack_58);
    }
    Agent::getMission(pAVar27);
    iVar4 = Mission::isOutsideMission();
    if (iVar4 != 0) {
      if (pAVar27[0x24] == (Agent)0x0) {
        *(int *)(*piVar11 + 0xe8) = *(int *)(*piVar11 + 0xe8) + 1;
      }
      if (pAVar27[0x25] == (Agent)0x0) {
        *(int *)(*piVar11 + 0xec) = *(int *)(*piVar11 + 0xec) + 1;
      }
    }
    pMVar22 = (Mission *)*piVar11;
    Agent::getMission(pAVar27);
    Status::setFreelanceMission(pMVar22);
    pMVar22 = (Mission *)Status::getFreelanceMission();
    Mission::setAgent(pMVar22,pAVar27);
    init((EVP_PKEY_CTX *)this);
    break;
  default:
    goto switchD_0017e652_caseD_1;
  case 2:
    piVar8 = *(int **)(DAT_0017ee88 + 0x17eb42);
    iVar4 = *piVar8;
    Agent::getSellItemPrice(pAVar27);
    Status::changeCredits(iVar4);
    iVar17 = **(int **)(DAT_0017ee8c + 0x17eb5a);
    iVar4 = Agent::getSellItemIndex(pAVar27);
    iVar4 = *(int *)(*(int *)(iVar17 + 4) + iVar4 * 4);
    Agent::getSellItemQuantity(pAVar27);
    pIVar18 = (Item *)Item::makeItem(iVar4);
    iVar4 = Item::getType(pIVar18);
    if (iVar4 == 1) {
      pSVar30 = (Ship *)Status::getShip();
      puVar25 = (uint *)Ship::getEquipment(pSVar30);
      if (puVar25 == (uint *)0x0) goto LAB_0017ebdc;
      bVar3 = false;
      for (uVar5 = 0; uVar5 < *puVar25; uVar5 = uVar5 + 1) {
        pIVar19 = *(Item **)(puVar25[1] + uVar5 * 4);
        if (pIVar19 != (Item *)0x0) {
          iVar4 = Item::getIndex(pIVar19);
          iVar17 = Item::getIndex(pIVar18);
          if (iVar4 == iVar17) {
            pIVar19 = *(Item **)(puVar25[1] + uVar5 * 4);
            iVar4 = Item::getAmount();
            Item::changeAmount(pIVar19,iVar4);
            bVar3 = true;
          }
        }
      }
      if (!bVar3) goto LAB_0017ebdc;
    }
    else {
LAB_0017ebdc:
      pIVar18 = (Item *)Status::getShip();
      Ship::addCargo(pIVar18);
    }
    iVar4 = Agent::getSellItemIndex(pAVar27);
    if ((0x83 < iVar4) && (iVar4 = Agent::getSellItemIndex(pAVar27), iVar4 < 0x9a)) {
      iVar17 = *(int *)(*piVar8 + 0xac);
      iVar4 = Agent::getSellItemIndex(pAVar27);
      *(undefined1 *)(iVar4 + *(int *)(iVar17 + 4) + -0x84) = 1;
    }
    break;
  case 3:
    piVar8 = *(int **)(DAT_0017ee90 + 0x17ec20);
    iVar4 = *piVar8;
    Agent::getSellItemPrice(pAVar27);
    Status::changeCredits(iVar4);
    for (uVar5 = 0; puVar25 = (uint *)Status::getBluePrints((Status *)*piVar8), uVar5 < *puVar25;
        uVar5 = uVar5 + 1) {
      iVar4 = Status::getBluePrints((Status *)*piVar8);
      iVar4 = BluePrint::getIndex(*(BluePrint **)(*(int *)(iVar4 + 4) + uVar5 * 4));
      iVar17 = Agent::getSellBlueprintIndex(pAVar27);
      if (iVar4 == iVar17) {
        iVar4 = Status::getBluePrints((Status *)*piVar8);
        BluePrint::unlock(*(BluePrint **)(*(int *)(iVar4 + 4) + uVar5 * 4));
        break;
      }
    }
    puVar25 = (uint *)Status::getAgents((Status *)*piVar8);
    uVar5 = Agent::getIndex(pAVar27);
    if (uVar5 < *puVar25) {
      iVar4 = Agent::getIndex(pAVar27);
      Agent::setOfferAccepted(*(Agent **)(puVar25[1] + iVar4 * 4),true);
    }
    break;
  case 4:
    piVar8 = *(int **)(DAT_0017ee94 + 0x17ec6a);
    iVar4 = *piVar8;
    Agent::getSellItemPrice(pAVar27);
    Status::changeCredits(iVar4);
    pSVar29 = (Status *)*piVar8;
    iVar4 = Agent::getSellSystemIndex(pAVar27);
    Status::setSystemVisibility(pSVar29,iVar4,true);
    this[0x18] = (SpaceLounge)0x1;
    this[0x35] = (SpaceLounge)0x1;
    puVar25 = (uint *)Status::getAgents((Status *)*piVar8);
    uVar5 = Agent::getIndex(pAVar27);
    if (uVar5 < *puVar25) {
      iVar4 = Agent::getIndex(pAVar27);
      Agent::setOfferAccepted(*(Agent **)(puVar25[1] + iVar4 * 4),true);
    }
    goto switchD_0017e652_caseD_1;
  case 6:
    iVar4 = Agent::getWingmanFriendsCount(pAVar27);
    piVar8 = *(int **)(DAT_0017ee98 + 0x17ecce);
    pSVar29 = (Status *)*piVar8;
    *(int *)(pSVar29 + 0xd4) = iVar4 + *(int *)(pSVar29 + 0xd4) + 1;
    pAVar13 = (Array *)Agent::getWingmanNames(pAVar27);
    Status::setWingmen(pSVar29,pAVar13);
    uVar14 = Agent::getRace(pAVar27);
    uVar10 = DAT_0017ee9c;
    iVar4 = *piVar8;
    *(undefined4 *)(iVar4 + 0x2c) = uVar14;
    *(undefined4 *)(iVar4 + 0x30) = uVar10;
    pvVar15 = operator_new__(0x14);
    for (uVar5 = 0; uVar5 < 5; uVar5 = uVar5 + 1) {
      iVar4 = Agent::getImageParts(pAVar27);
      *(undefined4 *)((int)pvVar15 + uVar5 * 4) = *(undefined4 *)(iVar4 + uVar5 * 4);
    }
    puVar16 = *(undefined4 **)(DAT_0017eea0 + 0x17ed20);
    *(void **)(*piVar8 + 0x28) = pvVar15;
    iVar4 = Achievements::gotAllMedals((Achievements *)*puVar16);
    iVar17 = *piVar8;
    if (iVar4 == 0) {
      Agent::getCosts(pAVar27);
      Status::changeCredits(iVar17);
    }
    else {
      Agent::getCosts(pAVar27);
      Status::changeCredits(iVar17);
    }
    goto switchD_0017e652_caseD_1;
  case 7:
    piVar8 = *(int **)(DAT_0017eea4 + 0x17ed48);
    this_02 = (Standing *)Status::getStanding();
    iVar4 = Agent::getRace(pAVar27);
    Standing::rehabilitate(this_02,iVar4);
    iVar4 = *piVar8;
    Agent::getCosts(pAVar27);
    Status::changeCredits(iVar4);
    goto switchD_0017e652_caseD_1;
  case 8:
    iVar4 = **(int **)(DAT_0017eea8 + 0x17ed7a);
    if (*(char *)(*(int *)(DAT_0017eeac + 0x17ed7c) + 0x38) == '\0') {
      iVar17 = Agent::getSellItemPrice(pAVar27);
      VectorSignedToFloat(-iVar17,(byte)(in_fpscr >> 0x16) & 3);
    }
    else {
      iVar17 = Agent::getSellItemPrice(pAVar27);
      VectorSignedToFloat(-iVar17,(byte)(in_fpscr >> 0x16) & 3);
    }
    Status::changeCredits(iVar4);
    pSVar30 = (Ship *)Status::getShip();
    iVar4 = Agent::getSellModIndex(pAVar27);
    Ship::addMod(pSVar30,iVar4);
    break;
  case 9:
    iVar4 = **(int **)(DAT_0017eeb0 + 0x17eda4);
    if (*(char *)(*(int *)(DAT_0017eeb4 + 0x17eda6) + 0x38) == '\0') {
      iVar17 = Agent::getSellItemPrice(pAVar27);
      VectorSignedToFloat(-iVar17,(byte)(in_fpscr >> 0x16) & 3);
    }
    else {
      iVar17 = Agent::getSellItemPrice(pAVar27);
      VectorSignedToFloat(-iVar17,(byte)(in_fpscr >> 0x16) & 3);
    }
    Status::changeCredits(iVar4);
    iVar17 = **(int **)(DAT_0017f288 + 0x17f21a);
    iVar4 = Agent::getSellItemIndex(pAVar27);
    iVar4 = *(int *)(*(int *)(iVar17 + 4) + iVar4 * 4);
    Agent::getSellItemQuantity(pAVar27);
    pIVar18 = (Item *)Item::makeItem(iVar4);
    iVar4 = Item::getType(pIVar18);
    if (iVar4 == 1) {
      pSVar30 = (Ship *)Status::getShip();
      puVar25 = (uint *)Ship::getEquipment(pSVar30);
      if (puVar25 != (uint *)0x0) {
        bVar3 = false;
        for (uVar5 = 0; uVar5 < *puVar25; uVar5 = uVar5 + 1) {
          pIVar19 = *(Item **)(puVar25[1] + uVar5 * 4);
          if (pIVar19 != (Item *)0x0) {
            iVar4 = Item::getIndex(pIVar19);
            iVar17 = Item::getIndex(pIVar18);
            if (iVar4 == iVar17) {
              pIVar19 = *(Item **)(puVar25[1] + uVar5 * 4);
              iVar4 = Item::getAmount();
              Item::changeAmount(pIVar19,iVar4);
              bVar3 = true;
            }
          }
        }
        if (bVar3) break;
      }
    }
    pIVar18 = (Item *)Status::getShip();
    Ship::addCargo(pIVar18);
    break;
  case 10:
    piVar8 = *(int **)(DAT_0017eeb8 + 0x17edd0);
    iVar4 = *piVar8;
    if (*(char *)(*(int *)(DAT_0017eebc + 0x17edd2) + 0x38) == '\0') {
      iVar17 = Agent::getSellItemPrice(pAVar27);
      VectorSignedToFloat(-iVar17,(byte)(in_fpscr >> 0x16) & 3);
    }
    else {
      iVar17 = Agent::getSellItemPrice(pAVar27);
      VectorSignedToFloat(-iVar17,(byte)(in_fpscr >> 0x16) & 3);
    }
    Status::changeCredits(iVar4);
    piVar24 = *(int **)(DAT_0017f6e4 + 0x17f308);
    iVar17 = *piVar24;
    pSVar30 = *(Ship **)(*piVar8 + 0x14c);
    iVar4 = Agent::getSellItemIndex(pAVar27);
    Ship::makeShip(*(int *)(*(int *)(iVar17 + 4) + iVar4 * 4));
    Station::addShip(pSVar30);
    pSVar30 = (Ship *)Status::getStation();
    iVar17 = *piVar24;
    iVar4 = Agent::getSellItemIndex(pAVar27);
    Ship::makeShip(*(int *)(*(int *)(iVar17 + 4) + iVar4 * 4));
    Station::addShip(pSVar30);
  }
  this[0x35] = (SpaceLounge)0x1;
switchD_0017e652_caseD_1:
  iVar4 = Agent::getOffer(pAVar27);
  if (iVar4 != 8) {
    Agent::setOfferAccepted(pAVar27,true);
  }
  pSVar32 = *(ScrollTouchWindow **)(this + 0x60);
  Agent::getName();
  AbyssEngine::String::String(aSStack_a0,aSStack_40,false);
  ScrollTouchWindow::setText(pSVar32,auStack_94,aSStack_a0);
  pcVar26 = *(code **)(DAT_0018024c + 0x17ffca);
  (*pcVar26)(aSStack_a0);
  (*pcVar26)(auStack_94);
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x14) = 3;
  (*pcVar26)(aSStack_40);
  this[0x19] = (SpaceLounge)0x0;
switchD_0017e340_default:
LAB_0017e506:
  if (*piVar36 - local_34 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*piVar36 - local_34);
  }
  return;
}

```

## target disasm
```
  0017e2f0: push {r4,r5,r6,r7,lr}
  0017e2f2: add r7,sp,#0xc
  0017e2f4: push {r7,r8,r9,r10,r11}
  0017e2f8: vpush {d8}
  0017e2fc: sub.w sp,sp,#0x3b0
  0017e300: mov r8,r0
  0017e302: ldr r0,[0x0017e66c]
  0017e304: mov r4,r8
  0017e306: mov r5,r1
  0017e308: add r0,pc
  0017e30a: ldr.w r11,[r0,#0x0]
  0017e30e: ldr.w r0,[r11,#0x0]
  0017e312: str r0,[sp,#0x3ac]
  0017e314: ldrb.w r0,[r4,#0x1b]!
  0017e318: cbz r0,0x0017e336
  0017e31a: cmp.w r5,#0x1000
  0017e31e: beq.w 0x0017e4f2
  0017e322: cmp.w r5,#0x2000
  0017e326: beq.w 0x0017e4fc
  0017e32a: cmp.w r5,#0x10000
  0017e32e: itt eq
  0017e330: mov.eq r0,#0x0
  0017e332: strb.eq r0,[r4,#0x0]
  0017e334: b 0x0017e594
  0017e336: ldr.w r0,[r8,#0x14]
  0017e33a: cmp r0,#0x3
  0017e33c: bhi.w 0x0017e594
  0017e340: tbb [pc,r0]
  0017e348: cmp.w r5,#0x10000
  0017e34c: it ne
  0017e34e: cmp.ne.w r5,#0x20000
  0017e352: bne.w 0x0017e468
  0017e356: movs r2,#0x0
  0017e358: ldrd r0,r1,[r8,#0x20]
  0017e35c: str.w r2,[r8,#0x30]
  0017e360: ldr r1,[r1,#0x4]
  0017e362: ldr.w r5,[r1,r0,lsl #0x2]
  0017e366: mov r0,r5
  0017e368: blx 0x00071ae8
  0017e36c: cmp r0,#0x1
  0017e36e: bne.w 0x0017e482
  0017e372: movs r0,#0x2
  0017e374: str.w r0,[r8,#0x14]
  0017e378: b 0x0017e594
  0017e37a: movs r6,#0x0
  0017e37c: cmp.w r5,#0x1000
  0017e380: beq.w 0x0017e528
  0017e384: cmp.w r5,#0x2000
  0017e388: beq.w 0x0017e546
  0017e38c: cmp.w r5,#0x40000
  0017e390: beq.w 0x0017e506
  0017e394: cmp.w r5,#0x8000
  0017e398: beq.w 0x0017e528
  0017e39c: cmp.w r5,#0x10000
  0017e3a0: it ne
  0017e3a2: cmp.ne.w r5,#0x20000
  0017e3a6: bne.w 0x0017e540
  0017e3aa: mov r0,r8
  0017e3ac: blx 0x00077d28
  0017e3b0: b 0x0017e594
  0017e3b2: ldrd r0,r1,[r8,#0x20]
  0017e3b6: ldr r1,[r1,#0x4]
  0017e3b8: ldr.w r0,[r1,r0,lsl #0x2]
  0017e3bc: blx 0x00071bb4
  0017e3c0: cbz r0,0x0017e3da
  0017e3c2: ldrd r0,r1,[r8,#0x20]
  0017e3c6: ldr r1,[r1,#0x4]
  0017e3c8: ldr.w r0,[r1,r0,lsl #0x2]
  0017e3cc: blx 0x00071bb4
  0017e3d0: blx 0x00077d70
  0017e3d4: cbz r0,0x0017e3da
  0017e3d6: movs r0,#0x5
  0017e3d8: b 0x0017e438
  0017e3da: ldrd r0,r1,[r8,#0x20]
  0017e3de: ldr r1,[r1,#0x4]
  0017e3e0: ldr.w r0,[r1,r0,lsl #0x2]
  0017e3e4: blx 0x00071ae8
  0017e3e8: cmp r0,#0x2
  0017e3ea: beq 0x0017e436
  0017e3ec: ldrd r0,r1,[r8,#0x20]
  0017e3f0: ldr r1,[r1,#0x4]
  0017e3f2: ldr.w r0,[r1,r0,lsl #0x2]
  0017e3f6: blx 0x00071ae8
  0017e3fa: cmp r0,#0x3
  0017e3fc: beq 0x0017e436
  0017e3fe: ldrd r0,r1,[r8,#0x20]
  0017e402: ldr r1,[r1,#0x4]
  0017e404: ldr.w r0,[r1,r0,lsl #0x2]
  0017e408: blx 0x00071ae8
  0017e40c: cmp r0,#0x8
  0017e40e: beq 0x0017e436
  0017e410: ldrd r0,r1,[r8,#0x20]
  0017e414: ldr r1,[r1,#0x4]
  0017e416: ldr.w r0,[r1,r0,lsl #0x2]
  0017e41a: blx 0x00071ae8
  0017e41e: cmp r0,#0x9
  0017e420: beq 0x0017e436
  0017e422: ldrd r0,r1,[r8,#0x20]
  0017e426: ldr r1,[r1,#0x4]
  0017e428: ldr.w r0,[r1,r0,lsl #0x2]
  0017e42c: blx 0x00071ae8
  0017e430: cmp r0,#0xa
  0017e432: bne.w 0x0017e8ee
  0017e436: movs r0,#0x4
  0017e438: ldrb.w r1,[r8,#0x1c]
  0017e43c: cmp r1,#0x0
  0017e43e: beq 0x0017e4d4
  0017e440: cmp.w r5,#0x40000
  0017e444: bne.w 0x0017e594
  0017e448: ldr r0,[0x0017e670]
  0017e44a: movs r1,#0x0
  0017e44c: strb.w r1,[r8,#0x1c]
  0017e450: add r0,pc
  0017e452: ldr r0,[r0,#0x0]
  0017e454: ldr r0,[r0,#0x0]
  0017e456: blx 0x0007258c
  0017e45a: ldrb.w r2,[r8,#0xb1]
  0017e45e: mov.w r1,#0x1400000
  0017e462: blx 0x000707f8
  0017e466: b 0x0017e594
  0017e468: cmp.w r5,#0x40000
  0017e46c: bne.w 0x0017e594
  0017e470: ldr.w r1,[r8,#0x2c]
  0017e474: cmp r1,#0x3
  0017e476: blt 0x0017e564
  0017e478: sub.w r0,r1,#0x28
  0017e47c: str.w r0,[r8,#0x2c]
  0017e480: b 0x0017e594
  0017e482: ldr.w r0,[r8,#0x14]
  0017e486: subs r0,#0x1
  0017e488: clz r0,r0
  0017e48c: lsr.w r0,r0,#0x5
  0017e490: lsl.w r0,r0,#0x1
  0017e494: str.w r0,[r8,#0x14]
  0017e498: itt ne
  0017e49a: ldrb.ne.w r0,[r8,#0x18]
  0017e49e: cmp.ne r0,#0x0
  0017e4a0: beq.w 0x0017e594
  0017e4a4: ldr r0,[0x0017e674]
  0017e4a6: movs r1,#0x5
  0017e4a8: add r0,pc
  0017e4aa: ldr r4,[r0,#0x0]
  0017e4ac: ldr r0,[r4,#0x0]
  0017e4ae: blx 0x00075bd4
  0017e4b2: ldr r6,[r0,#0x10]
  0017e4b4: str.w r6,[r8,#0x4]
  0017e4b8: cmp r6,#0x0
  0017e4ba: beq.w 0x0017e5ca
  0017e4be: mov r0,r5
  0017e4c0: blx 0x00074b84
  0017e4c4: str r0,[sp,#0x0]
  0017e4c6: mov r0,r6
  0017e4c8: movs r1,#0x0
  0017e4ca: movs r2,#0x0
  0017e4cc: movs r3,#0x1
  0017e4ce: blx 0x00074554
  0017e4d2: b 0x0017e600
  0017e4d4: mov r10,r8
  0017e4d6: ldrb.w r1,[r10,#0x19]!
  0017e4da: cmp r1,#0x0
  0017e4dc: beq 0x0017e574
  0017e4de: cmp.w r5,#0x10000
  0017e4e2: beq.w 0x0017e60e
  0017e4e6: cmp.w r5,#0x2000
  0017e4ea: beq 0x0017e4fc
  0017e4ec: cmp.w r5,#0x1000
  0017e4f0: bne 0x0017e594
  0017e4f2: ldr.w r0,[r8,#0x8]
  0017e4f6: blx 0x00075304
  0017e4fa: b 0x0017e594
  0017e4fc: ldr.w r0,[r8,#0x8]
  0017e500: blx 0x00077d7c
  0017e504: b 0x0017e594
  0017e506: ldr r0,[sp,#0x3ac]
  0017e508: ldr.w r1,[r11,#0x0]
  0017e50c: subs r0,r1,r0
  0017e50e: itttt eq
  0017e510: mov.eq r0,r6
  0017e512: add.eq.w sp,sp,#0x3b0
  0017e516: vpop.eq {d8}
  0017e51a: add.eq sp,#0x4
  0017e51c: itt eq
  0017e51e: pop.eq.w {r8,r9,r10,r11}
  0017e522: pop.eq {r4,r5,r6,r7,pc}
  0017e524: blx 0x0006e824
  0017e528: ldrd r0,r1,[r8,#0x20]
  0017e52c: adds r0,#0x1
  0017e52e: str.w r0,[r8,#0x20]
  0017e532: ldr r1,[r1,#0x0]
  0017e534: cmp r0,r1
  0017e536: it cc
  0017e538: mov.cc r6,r0
  0017e53a: str.w r6,[r8,#0x20]
  0017e53e: b 0x0017e594
  0017e540: cmp.w r5,#0x4000
  0017e544: bne 0x0017e594
  0017e546: ldr.w r0,[r8,#0x20]
  0017e54a: cmp r0,#0x0
  0017e54c: sub.w r1,r0,#0x1
  0017e550: str.w r1,[r8,#0x20]
  0017e554: bgt 0x0017e594
  0017e556: ldr.w r0,[r8,#0x24]
  0017e55a: ldr r0,[r0,#0x0]
  0017e55c: subs r0,#0x1
  0017e55e: str.w r0,[r8,#0x20]
  0017e562: b 0x0017e594
  0017e564: cmp r0,#0x1
  0017e566: ittt eq
  0017e568: mov.eq r0,#0x0
  0017e56a: str.w.eq r0,[r8,#0x2c]
  0017e56e: str.w.eq r0,[r8,#0x14]
  0017e572: b 0x0017e594
  0017e574: sub.w r1,r5,#0x4000
  0017e578: ror r1,r1, ror #0xe
  0017e57c: cmp r1,#0x7
  0017e57e: bhi 0x0017e594
  0017e580: tbh [pc,r1]
  0017e594: movs r6,#0x1
  0017e596: b 0x0017e506
  0017e598: ldr.w r1,[r8,#0x30]
  0017e59c: cmp r1,#0x1
  0017e59e: it lt
  0017e5a0: mov.lt r1,r0
  0017e5a2: cmp.w r5,#0x20000
  0017e5a6: sub.w r2,r1,#0x1
  0017e5aa: str.w r2,[r8,#0x30]
  0017e5ae: it ne
  0017e5b0: cmp.ne.w r5,#0x10000
  0017e5b4: beq.w 0x0017e786
  0017e5b8: cmp.w r5,#0x8000
  0017e5bc: bne 0x0017e594
  0017e5be: cmp r1,r0
  0017e5c0: it ge
  0017e5c2: mov.ge r1,#0x0
  0017e5c4: str.w r1,[r8,#0x30]
  0017e5c8: b 0x0017e594
  0017e5ca: mov.w r0,#0x1e8
  0017e5ce: blx 0x0006eb24
  0017e5d2: mov r6,r0
  0017e5d4: mov r0,r5
  0017e5d6: blx 0x00074b84
  0017e5da: str r0,[sp,#0x0]
  0017e5dc: mov r0,r6
  0017e5de: movs r1,#0x0
  0017e5e0: movs r2,#0x0
  0017e5e2: movs r3,#0x1
  0017e5e4: blx 0x00075424
  0017e5e8: ldr r0,[r4,#0x0]
  0017e5ea: movs r1,#0x5
  0017e5ec: blx 0x00075bd4
  0017e5f0: str r6,[r0,#0x10]
  0017e5f2: movs r1,#0x5
  0017e5f4: ldr r0,[r4,#0x0]
  0017e5f6: blx 0x00075bd4
  0017e5fa: ldr r0,[r0,#0x10]
  0017e5fc: str.w r0,[r8,#0x4]
  0017e600: movs r0,#0x0
  0017e602: strb.w r0,[r8,#0x18]
  0017e606: movs r0,#0x1
  0017e608: strb.w r0,[r8,#0x34]
  0017e60c: b 0x0017e594
  0017e60e: ldr r0,[0x0017e678]
  0017e610: ldr r1,[0x0017e67c]
  0017e612: add r0,pc
  0017e614: add r1,pc
  0017e616: ldr r0,[r0,#0x0]
  0017e618: ldr r1,[r1,#0x0]
  0017e61a: str r0,[sp,#0x18]
  0017e61c: ldr r5,[r0,#0x0]
  0017e61e: ldr r0,[r1,#0x0]
  0017e620: str r1,[sp,#0x14]
  0017e622: movs r1,#0x3
  0017e624: blx 0x00071848
  0017e628: addw r1,r0,#0x352
  0017e62c: mov r0,r5
  0017e62e: blx 0x00072f70
  0017e632: mov r1,r0
  0017e634: add r0,sp,#0x3a0
  0017e636: movs r2,#0x0
  0017e638: blx 0x0006f028
  0017e63c: ldrd r0,r1,[r8,#0x20]
  0017e640: ldr r1,[r1,#0x4]
  0017e642: ldr.w r9,[r1,r0,lsl #0x2]
  0017e646: mov r0,r9
  0017e648: blx 0x00071ae8
  0017e64c: cmp r0,#0xa
  0017e64e: bhi.w 0x0017ff92
  0017e652: tbh [pc,r0]
  0017e680: ldr r0,[0x0017ea34]
  0017e682: add r0,pc
  0017e684: ldr r0,[r0,#0x0]
  0017e686: str r0,[sp,#0x10]
  0017e688: ldr r0,[r0,#0x0]
  0017e68a: blx 0x0007192c
  0017e68e: ldr r5,[sp,#0x10]
  0017e690: cmp r0,#0x0
  0017e692: beq.w 0x0017f072
  0017e696: mov r0,r9
  0017e698: blx 0x00071bb4
  0017e69c: blx 0x00071bc0
  0017e6a0: mov r6,r0
  0017e6a2: mov r0,r9
  0017e6a4: blx 0x00071bb4
  0017e6a8: blx 0x00074b78
  0017e6ac: add r0,r6
  0017e6ae: movs r1,#0xa
  0017e6b0: blx 0x0007198c
  0017e6b4: mov r6,r0
  0017e6b6: ldr r0,[r5,#0x0]
  0017e6b8: blx 0x000733d8
  0017e6bc: cmp r6,r0
  0017e6be: ble.w 0x0017f06a
  0017e6c2: ldr r0,[sp,#0x18]
  0017e6c4: ldr r0,[r0,#0x0]
  0017e6c6: movs r1,#0xcb
  0017e6c8: blx 0x00072f70
  0017e6cc: mov r1,r0
  0017e6ce: add r0,sp,#0x394
  0017e6d0: movs r2,#0x0
  0017e6d2: blx 0x0006f028
  0017e6d6: ldr.w r9,[r5,#0x0]
  0017e6da: add r0,sp,#0x37c
  0017e6dc: add r1,sp,#0x394
  0017e6de: movs r2,#0x0
  0017e6e0: blx 0x0006f028
  0017e6e4: mov r1,r5
  0017e6e6: ldr r0,[0x0017ea38]
  0017e6e8: add r0,pc
  0017e6ea: ldr r0,[r0,#0x0]
  0017e6ec: ldr r5,[r0,#0x0]
  0017e6ee: ldr r0,[r1,#0x0]
  0017e6f0: blx 0x000733d8
  0017e6f4: subs r2,r6,r0
  0017e6f6: add r0,sp,#0x364
  0017e6f8: mov r1,r5
  0017e6fa: blx 0x00074e54
  0017e6fe: add r0,sp,#0x370
  0017e700: add r1,sp,#0x364
  0017e702: movs r2,#0x0
  0017e704: blx 0x0006f028
  0017e708: ldr r1,[0x0017ea3c]
  0017e70a: add r1,pc
  0017e70c: add r0,sp,#0x358
  0017e70e: movs r2,#0x0
  0017e710: blx 0x0006ee18
  0017e714: str r0,[sp,#0x0]
  0017e716: add r0,sp,#0x388
  0017e718: add r2,sp,#0x37c
  0017e71a: add r3,sp,#0x370
  0017e71c: mov r1,r9
  0017e71e: blx 0x000739b4
  0017e722: add r0,sp,#0x394
  0017e724: add r1,sp,#0x388
  0017e726: blx 0x0006f2b0
  0017e72a: ldr r0,[0x0017ea40]
  0017e72c: add r0,pc
  0017e72e: ldr r5,[r0,#0x0]
  0017e730: add r0,sp,#0x388
  0017e732: blx r5
  0017e734: add r0,sp,#0x358
  0017e736: blx r5
  0017e738: add r0,sp,#0x370
  0017e73a: blx r5
  0017e73c: add r0,sp,#0x364
  0017e73e: blx r5
  0017e740: add r0,sp,#0x37c
  0017e742: blx r5
  0017e744: ldr.w r0,[r8,#0x8]
  0017e748: add r1,sp,#0x394
  0017e74a: movs r2,#0x0
  0017e74c: movs r6,#0x0
  0017e74e: blx 0x000746f8
  0017e752: movs r0,#0x1
  0017e754: strb r0,[r4,#0x0]
  0017e756: add r0,sp,#0x394
  0017e758: strb.w r6,[r10,#0x0]
  0017e75c: blx 0x0006ee30
  0017e760: add r0,sp,#0x3a0
  0017e762: blx 0x0006ee30
  0017e766: b 0x0017e506
  0017e768: ldr.w r1,[r8,#0x30]
  0017e76c: adds r1,#0x1
  0017e76e: cmp r1,r0
  0017e770: it ge
  0017e772: mov.ge r1,#0x0
  0017e774: cmp.w r5,#0x20000
  0017e778: str.w r1,[r8,#0x30]
  0017e77c: it ne
  0017e77e: cmp.ne.w r5,#0x10000
  0017e782: bne.w 0x0017e594
  0017e786: ldr.w r0,[r8,#0x30]
  0017e78a: cmp r0,#0x4
  0017e78c: bhi.w 0x0017fdbc
  0017e790: ldrd r1,r2,[r8,#0x20]
  0017e794: ldr r2,[r2,#0x4]
  0017e796: ldr.w r9,[r2,r1,lsl #0x2]
  0017e79a: tbh [pc,r0]
  0017e7a8: mov r0,r9
  0017e7aa: blx 0x00071ae8
  0017e7ae: ldr r1,[0x0017ea44]
  0017e7b0: mov r6,r0
  0017e7b2: add r0,sp,#0x3a0
  0017e7b4: movs r2,#0x0
  0017e7b6: add r1,pc
  0017e7b8: blx 0x0006ee18
  0017e7bc: cmp r6,#0xa
  0017e7be: bhi.w 0x0017fda4
  0017e7c2: tbh [pc,r6]
  0017e7dc: ldr r0,[0x0017ea48]
  0017e7de: vmov.f32 s16,0x3f800000
  0017e7e2: vmov.f32 s0,0x40000000
  0017e7e6: add r0,pc
  0017e7e8: ldr r0,[r0,#0x0]
  0017e7ea: ldrb.w r0,[r0,#0x38]
  0017e7ee: vmov.f32 s2,s16
  0017e7f2: ldr r1,[0x0017ea4c]
  0017e7f4: cmp r0,#0x0
  0017e7f6: sub.w r0,r6,#0x8
  0017e7fa: add r1,pc
  0017e7fc: it eq
  0017e7fe: vmov.eq.f32 s2,s0
  0017e802: cmp r0,#0x3
  0017e804: it cc
  0017e806: vmov.cc.f32 s16,s2
  0017e80a: ldr r0,[r1,#0x0]
  0017e80c: str r0,[sp,#0x18]
  0017e80e: ldr r0,[r0,#0x0]
  0017e810: blx 0x000733d8
  0017e814: mov r5,r0
  0017e816: mov r0,r9
  0017e818: blx 0x00074bc0
  0017e81c: vmov s0,r0
  0017e820: vmov s2,r5
  0017e824: vcvt.f32.s32 s0,s0
  0017e828: vcvt.f32.s32 s2,s2
  0017e82c: vmul.f32 s0,s16,s0
  0017e830: vcmpe.f32 s0,s2
  0017e834: vmrs apsr,fpscr
  0017e838: ble.w 0x0017ee60
  0017e83c: ldr r0,[0x0017ea50]
  0017e83e: ldr r1,[sp,#0x18]
  0017e840: add r0,pc
  0017e842: ldr.w r8,[r8,#0x8]
  0017e846: ldr r0,[r0,#0x0]
  0017e848: ldr.w r10,[r1,#0x0]
  0017e84c: ldr r0,[r0,#0x0]
  0017e84e: movs r1,#0xcb
  0017e850: blx 0x00072f70
  0017e854: mov r1,r0
  0017e856: add r0,sp,#0x25c
  0017e858: movs r2,#0x0
  0017e85a: blx 0x0006f028
  0017e85e: ldr r0,[0x0017ea54]
  0017e860: add r0,pc
  0017e862: ldr r0,[r0,#0x0]
  0017e864: ldr r6,[r0,#0x0]
  0017e866: mov r0,r9
  0017e868: blx 0x00074bc0
  0017e86c: mov r5,r0
  0017e86e: ldr r0,[sp,#0x18]
  0017e870: ldr r0,[r0,#0x0]
  0017e872: blx 0x000733d8
  0017e876: vmov s0,r5
  0017e87a: vmov s2,r0
  0017e87e: vcvt.f32.s32 s0,s0
  0017e882: vcvt.f32.s32 s2,s2
  0017e886: vnmls.f32 s2,s16,s0
  0017e88a: vcvt.s32.f32 s0,s2
  0017e88e: vmov r2,s0
  0017e892: add r0,sp,#0x388
  0017e894: mov r1,r6
  0017e896: blx 0x00074e54
  0017e89a: add r0,sp,#0x250
  0017e89c: add r1,sp,#0x388
  0017e89e: movs r2,#0x0
  0017e8a0: blx 0x0006f028
  0017e8a4: ldr r1,[0x0017ea58]
  0017e8a6: add r1,pc
  0017e8a8: add r0,sp,#0x244
  0017e8aa: movs r2,#0x0
  0017e8ac: blx 0x0006ee18
  0017e8b0: str r0,[sp,#0x0]
  0017e8b2: add r0,sp,#0x394
  0017e8b4: add r2,sp,#0x25c
  0017e8b6: add r3,sp,#0x250
  0017e8b8: mov r1,r10
  0017e8ba: blx 0x000739b4
  0017e8be: add r1,sp,#0x394
  0017e8c0: mov r0,r8
  0017e8c2: movs r2,#0x0
  0017e8c4: blx 0x000746f8
  0017e8c8: ldr r0,[0x0017ea5c]
  0017e8ca: add r0,pc
  0017e8cc: ldr r5,[r0,#0x0]
  0017e8ce: add r0,sp,#0x394
  0017e8d0: blx r5
  0017e8d2: add r0,sp,#0x244
  0017e8d4: blx r5
  0017e8d6: add r0,sp,#0x250
  0017e8d8: blx r5
  0017e8da: add r0,sp,#0x388
  0017e8dc: blx r5
  0017e8de: add r0,sp,#0x25c
  0017e8e0: blx r5
  0017e8e2: movs r0,#0x1
  0017e8e4: strb r0,[r4,#0x0]
  0017e8e6: add r0,sp,#0x3a0
  0017e8e8: blx 0x0006ee30
  0017e8ec: b 0x0017e594
  0017e8ee: movs r0,#0x3
  0017e8f0: b 0x0017e438
  0017e8f2: movs r0,#0x2
  0017e8f4: ldr.w r6,[r8,#0x60]
  0017e8f8: str.w r0,[r8,#0x14]
  0017e8fc: add r0,sp,#0x70
  0017e8fe: mov r1,r9
  0017e900: blx 0x00071c2c
  0017e904: ldr r0,[0x0017ea60]
  0017e906: add r0,pc
  0017e908: ldr r0,[r0,#0x0]
  0017e90a: ldr r1,[r0,#0x0]
  0017e90c: add r0,sp,#0x64
  0017e90e: mov r2,r9
  0017e910: blx 0x00077278
  0017e914: add r1,sp,#0x70
  0017e916: add r2,sp,#0x64
  0017e918: mov r0,r6
  0017e91a: blx 0x00075b2c
  0017e91e: add r0,sp,#0x64
  0017e920: blx 0x0006ee30
  0017e924: add r0,sp,#0x70
  0017e926: blx 0x0006ee30
  0017e92a: ldr r0,[0x0017ea64]
  0017e92c: movs r1,#0x0
  0017e92e: str.w r1,[r8,#0x2c]
  0017e932: add r0,pc
  0017e934: ldr r0,[r0,#0x0]
  0017e936: ldr r0,[r0,#0x0]
  0017e938: ldr.w r1,[r0,#0xe4]
  0017e93c: adds r1,#0x1
  0017e93e: str.w r1,[r0,#0xe4]
  0017e942: b.w 0x0017fdbc
  0017e946: movs r0,#0x1
  0017e948: strb.w r0,[r9,#0x24]
  0017e94c: mov r0,r9
  0017e94e: blx 0x00071bb4
  0017e952: blx 0x00073ea0
  0017e956: mov r5,r0
  0017e958: add r0,sp,#0x28
  0017e95a: mov r1,r9
  0017e95c: ldr.w r6,[r8,#0x60]
  0017e960: blx 0x00071c2c
  0017e964: vmov s2,r5
  0017e968: vmov.f32 s0,0x41200000
  0017e96c: vcvt.f32.s32 s2,s2
  0017e970: ldr r0,[0x0017ea68]
  0017e972: add r0,pc
  0017e974: ldr r0,[r0,#0x0]
  0017e976: vdiv.f32 s0,s2,s0
  0017e97a: vmov.f32 s2,0x40a00000
  0017e97e: vmul.f32 s0,s0,s2
  0017e982: vcvt.s32.f32 s0,s0
  0017e986: ldr r0,[r0,#0x0]
  0017e988: vmov r1,s0
  0017e98c: add.w r1,r1,#0x328
  0017e990: blx 0x00072f70
  0017e994: mov r1,r0
  0017e996: add r0,sp,#0x1c
  0017e998: movs r2,#0x0
  0017e99a: blx 0x0006f028
  0017e99e: add r1,sp,#0x28
  0017e9a0: add r2,sp,#0x1c
  0017e9a2: mov r0,r6
  0017e9a4: blx 0x00075b2c
  0017e9a8: add r0,sp,#0x1c
  0017e9aa: blx 0x0006ee30
  0017e9ae: add r0,sp,#0x28
  0017e9b0: blx 0x0006ee30
  0017e9b4: movs r0,#0x0
  0017e9b6: str.w r0,[r8,#0x2c]
  0017e9ba: movs r0,#0x2
  0017e9bc: str.w r0,[r8,#0x14]
  0017e9c0: b.w 0x0017fdbc
  0017e9c4: add r0,sp,#0x88
  0017e9c6: mov r1,r9
  0017e9c8: ldr.w r6,[r8,#0x60]
  0017e9cc: blx 0x00071c2c
  0017e9d0: ldr r0,[0x0017ea6c]
  0017e9d2: ldr r1,[0x0017ea70]
  0017e9d4: add r0,pc
  0017e9d6: add r1,pc
  0017e9d8: ldr r0,[r0,#0x0]
  0017e9da: ldr r1,[r1,#0x0]
  0017e9dc: ldr r5,[r0,#0x0]
  0017e9de: ldr r0,[r1,#0x0]
  0017e9e0: movs r1,#0x5
  0017e9e2: blx 0x00071848
  0017e9e6: addw r1,r0,#0x34d
  0017e9ea: mov r0,r5
  0017e9ec: blx 0x00072f70
  0017e9f0: mov r1,r0
  0017e9f2: add r0,sp,#0x7c
  0017e9f4: movs r2,#0x0
  0017e9f6: blx 0x0006f028
  0017e9fa: add r1,sp,#0x88
  0017e9fc: add r2,sp,#0x7c
  0017e9fe: mov r0,r6
  0017ea00: blx 0x00075b2c
  0017ea04: add r0,sp,#0x7c
  0017ea06: blx 0x0006ee30
  0017ea0a: add r0,sp,#0x88
  0017ea0c: blx 0x0006ee30
  0017ea10: ldr r0,[0x0017ea74]
  0017ea12: movs r1,#0x0
  0017ea14: str.w r1,[r8,#0x2c]
  0017ea18: movs r1,#0x3
  0017ea1a: add r0,pc
  0017ea1c: str.w r1,[r8,#0x14]
  0017ea20: ldr r0,[r0,#0x0]
  0017ea22: ldr r0,[r0,#0x0]
  0017ea24: ldr.w r1,[r0,#0xe0]
  0017ea28: adds r1,#0x1
  0017ea2a: str.w r1,[r0,#0xe0]
  0017ea2e: b.w 0x0017fdbc
  0017ea78: mov r0,r9
  0017ea7a: blx 0x00071ae8
  0017ea7e: cmp r0,#0x2
  0017ea80: beq 0x0017ea96
  0017ea82: ldrd r0,r1,[r8,#0x20]
  0017ea86: ldr r1,[r1,#0x4]
  0017ea88: ldr.w r0,[r1,r0,lsl #0x2]
  0017ea8c: blx 0x00071ae8
  0017ea90: cmp r0,#0x3
  0017ea92: bne.w 0x0017edf6
  0017ea96: ldr.w r0,[r8,#0xc]
  0017ea9a: cbnz r0,0x0017eaae
  0017ea9c: mov.w r0,#0x13c
  0017eaa0: blx 0x0006eb24
  0017eaa4: mov r5,r0
  0017eaa6: blx 0x00076f18
  0017eaaa: str.w r5,[r8,#0xc]
  0017eaae: ldrd r0,r1,[r8,#0x20]
  0017eab2: ldr r1,[r1,#0x4]
  0017eab4: ldr.w r0,[r1,r0,lsl #0x2]
  0017eab8: blx 0x00071ae8
  0017eabc: cmp r0,#0x2
  0017eabe: bne 0x0017eafc
  0017eac0: movs r0,#0x48
  0017eac2: ldr.w r5,[r8,#0xc]
  0017eac6: blx 0x0006eb24
  0017eaca: mov r6,r0
  0017eacc: ldr r0,[0x0017ee80]
  0017eace: ldrd r1,r2,[r8,#0x20]
  0017ead2: add r0,pc
  0017ead4: ldr r2,[r2,#0x4]
  0017ead6: ldr r0,[r0,#0x0]
  0017ead8: ldr r4,[r0,#0x0]
  0017eada: ldr.w r0,[r2,r1,lsl #0x2]
  0017eade: blx 0x00074bb4
  0017eae2: ldr r1,[r4,#0x4]
  0017eae4: ldr.w r1,[r1,r0,lsl #0x2]
  0017eae8: mov r0,r6
  0017eaea: blx 0x00076198
  0017eaee: movs r0,#0x0
  0017eaf0: mov r1,r6
  0017eaf2: strd r0,r0,[sp,#0x0]
  0017eaf6: str r0,[sp,#0x8]
  0017eaf8: mov r0,r5
  0017eafa: b 0x0017f058
  0017eafc: ldr r0,[0x0017ee84]
  0017eafe: movs r4,#0x0
  0017eb00: add r0,pc
  0017eb02: ldr r6,[r0,#0x0]
  0017eb04: b 0x0017eb2e
  0017eb06: ldr r0,[r6,#0x0]
  0017eb08: blx 0x000748a8
  0017eb0c: ldr r0,[r0,#0x4]
  0017eb0e: ldr.w r0,[r0,r4,lsl #0x2]
  0017eb12: blx 0x000733fc
  0017eb16: mov r5,r0
  0017eb18: ldrd r0,r1,[r8,#0x20]
  0017eb1c: ldr r1,[r1,#0x4]
  0017eb1e: ldr.w r0,[r1,r0,lsl #0x2]
  0017eb22: blx 0x00074b90
  0017eb26: cmp r5,r0
  0017eb28: beq.w 0x0017f018
  0017eb2c: adds r4,#0x1
  0017eb2e: ldr r0,[r6,#0x0]
  0017eb30: blx 0x000748a8
  0017eb34: ldr r0,[r0,#0x0]
  0017eb36: cmp r4,r0
  0017eb38: bcc 0x0017eb06
  0017eb3a: b 0x0017f060
  0017eb3c: ldr r0,[0x0017ee88]
  0017eb3e: add r0,pc
  0017eb40: ldr r0,[r0,#0x0]
  0017eb42: str r0,[sp,#0x18]
  0017eb44: ldr r6,[r0,#0x0]
  0017eb46: mov r0,r9
  0017eb48: blx 0x00074bc0
  0017eb4c: rsbs r1,r0
  0017eb4e: mov r0,r6
  0017eb50: blx 0x0007171c
  0017eb54: ldr r0,[0x0017ee8c]
  0017eb56: add r0,pc
  0017eb58: ldr r0,[r0,#0x0]
  0017eb5a: ldr r4,[r0,#0x0]
  0017eb5c: mov r0,r9
  0017eb5e: blx 0x00074bb4
  0017eb62: ldr r1,[r4,#0x4]
  0017eb64: ldr.w r6,[r1,r0,lsl #0x2]
  0017eb68: mov r0,r9
  0017eb6a: blx 0x00074bcc
  0017eb6e: mov r1,r0
  0017eb70: mov r0,r6
  0017eb72: blx 0x000718c0
  0017eb76: mov r4,r0
  0017eb78: blx 0x000718fc
  0017eb7c: ldr r5,[sp,#0x18]
  0017eb7e: cmp r0,#0x1
  0017eb80: bne 0x0017ebdc
  0017eb82: ldr r0,[r5,#0x0]
  0017eb84: blx 0x00071a58
  0017eb88: blx 0x0007333c
  0017eb8c: mov r6,r0
  0017eb8e: cbz r0,0x0017ebdc
  0017eb90: mov.w r10,#0x0
  0017eb94: movs r0,#0x0
  0017eb96: str r0,[sp,#0x14]
  0017eb98: b 0x0017ebce
  0017eb9a: ldr r0,[r6,#0x4]
  0017eb9c: ldr.w r0,[r0,r10,lsl #0x2]
  0017eba0: cbz r0,0x0017ebca
  0017eba2: blx 0x000718d8
  0017eba6: mov r5,r0
  0017eba8: mov r0,r4
  0017ebaa: blx 0x000718d8
  0017ebae: cmp r5,r0
  0017ebb0: bne 0x0017ebca
  0017ebb2: ldr r0,[r6,#0x4]
  0017ebb4: ldr.w r5,[r0,r10,lsl #0x2]
  0017ebb8: mov r0,r4
  0017ebba: blx 0x0007183c
  0017ebbe: mov r1,r0
  0017ebc0: mov r0,r5
  0017ebc2: blx 0x00071854
  0017ebc6: movs r0,#0x1
  0017ebc8: str r0,[sp,#0x14]
  0017ebca: add.w r10,r10,#0x1
  0017ebce: ldr r0,[r6,#0x0]
  0017ebd0: cmp r10,r0
  0017ebd2: bcc 0x0017eb9a
  0017ebd4: ldr r0,[sp,#0x14]
  0017ebd6: ldr r5,[sp,#0x18]
  0017ebd8: lsls r0,r0,#0x1f
  0017ebda: bne 0x0017ebe8
  0017ebdc: ldr r0,[r5,#0x0]
  0017ebde: blx 0x00071a58
  0017ebe2: mov r1,r4
  0017ebe4: blx 0x00072cdc
  0017ebe8: mov r0,r9
  0017ebea: blx 0x00074bb4
  0017ebee: cmp r0,#0x84
  0017ebf0: blt.w 0x0017ff8c
  0017ebf4: mov r0,r9
  0017ebf6: blx 0x00074bb4
  0017ebfa: cmp r0,#0x99
  0017ebfc: bgt.w 0x0017ff8c
  0017ec00: ldr r0,[r5,#0x0]
  0017ec02: ldr.w r4,[r0,#0xac]
  0017ec06: mov r0,r9
  0017ec08: blx 0x00074bb4
  0017ec0c: ldr r1,[r4,#0x4]
  0017ec0e: add r0,r1
  0017ec10: movs r1,#0x1
  0017ec12: strb.w r1,[r0,#-0x84]
  0017ec16: b.w 0x0017ff8c
  0017ec1a: ldr r0,[0x0017ee90]
  0017ec1c: add r0,pc
  0017ec1e: ldr r4,[r0,#0x0]
  0017ec20: ldr r5,[r4,#0x0]
  0017ec22: mov r0,r9
  0017ec24: blx 0x00074bc0
  0017ec28: rsbs r1,r0
  0017ec2a: mov r0,r5
  0017ec2c: blx 0x0007171c
  0017ec30: movs r6,#0x0
  0017ec32: b 0x0017ec54
  0017ec34: ldr r0,[r4,#0x0]
  0017ec36: blx 0x000748a8
  0017ec3a: ldr r0,[r0,#0x4]
  0017ec3c: ldr.w r0,[r0,r6,lsl #0x2]
  0017ec40: blx 0x000733fc
  0017ec44: mov r5,r0
  0017ec46: mov r0,r9
  0017ec48: blx 0x00074b90
  0017ec4c: cmp r5,r0
  0017ec4e: beq.w 0x0017f7a4
  0017ec52: adds r6,#0x1
  0017ec54: ldr r0,[r4,#0x0]
  0017ec56: blx 0x000748a8
  0017ec5a: ldr r0,[r0,#0x0]
  0017ec5c: cmp r6,r0
  0017ec5e: bcc 0x0017ec34
  0017ec60: b.w 0x0017f7b4
  0017ec64: ldr r0,[0x0017ee94]
  0017ec66: add r0,pc
  0017ec68: ldr r4,[r0,#0x0]
  0017ec6a: ldr r5,[r4,#0x0]
  0017ec6c: mov r0,r9
  0017ec6e: blx 0x00074bc0
  0017ec72: rsbs r1,r0
  0017ec74: mov r0,r5
  0017ec76: blx 0x0007171c
  0017ec7a: ldr r6,[r4,#0x0]
  0017ec7c: mov r0,r9
  0017ec7e: blx 0x00074b84
  0017ec82: mov r1,r0
  0017ec84: mov r0,r6
  0017ec86: movs r2,#0x1
  0017ec88: movs r5,#0x1
  0017ec8a: blx 0x0007174c
  0017ec8e: strb.w r5,[r8,#0x18]
  0017ec92: strb.w r5,[r8,#0x35]
  0017ec96: ldr r0,[r4,#0x0]
  0017ec98: blx 0x00071ab8
  0017ec9c: mov r6,r0
  0017ec9e: mov r0,r9
  0017eca0: blx 0x00074ba8
  0017eca4: ldr r1,[r6,#0x0]
  0017eca6: cmp r0,r1
  0017eca8: bcs.w 0x0017ff92
  0017ecac: mov r0,r9
  0017ecae: blx 0x00074ba8
  0017ecb2: ldr r1,[r6,#0x4]
  0017ecb4: ldr.w r0,[r1,r0,lsl #0x2]
  0017ecb8: movs r1,#0x1
  0017ecba: blx 0x00071b18
  0017ecbe: b.w 0x0017ff92
  0017ecc2: mov r0,r9
  0017ecc4: blx 0x00074bd8
  0017ecc8: ldr r1,[0x0017ee98]
  0017ecca: add r1,pc
  0017eccc: ldr r4,[r1,#0x0]
  0017ecce: ldr r6,[r4,#0x0]
  0017ecd0: ldr.w r1,[r6,#0xd4]
  0017ecd4: add r0,r1
  0017ecd6: adds r0,#0x1
  0017ecd8: str.w r0,[r6,#0xd4]
  0017ecdc: mov r0,r9
  0017ecde: blx 0x00077d88
  0017ece2: mov r1,r0
  0017ece4: mov r0,r6
  0017ece6: blx 0x000743f8
  0017ecea: mov r0,r9
  0017ecec: blx 0x00071bd8
  0017ecf0: ldr r1,[r4,#0x0]
  0017ecf2: ldr r2,[0x0017ee9c]
  0017ecf4: strd r0,r2,[r1,#0x2c]
  0017ecf8: movs r0,#0x14
  0017ecfa: blx 0x0006ec08
  0017ecfe: mov r6,r0
  0017ed00: movs r5,#0x0
  0017ed02: b 0x0017ed14
  0017ed04: mov r0,r9
  0017ed06: blx 0x00071c74
  0017ed0a: ldr.w r0,[r0,r5,lsl #0x2]
  0017ed0e: str.w r0,[r6,r5,lsl #0x2]
  0017ed12: adds r5,#0x1
  0017ed14: cmp r5,#0x5
  0017ed16: bcc 0x0017ed04
  0017ed18: ldr r0,[0x0017eea0]
  0017ed1a: ldr r1,[r4,#0x0]
  0017ed1c: add r0,pc
  0017ed1e: ldr r0,[r0,#0x0]
  0017ed20: str r6,[r1,#0x28]
  0017ed22: ldr r0,[r0,#0x0]
  0017ed24: blx 0x00075580
  0017ed28: ldr r6,[r4,#0x0]
  0017ed2a: cmp r0,#0x0
  0017ed2c: beq.w 0x0017f3be
  0017ed30: mov r0,r9
  0017ed32: blx 0x00071bfc
  0017ed36: mov r1,r0
  0017ed38: mov r0,r6
  0017ed3a: blx 0x0007171c
  0017ed3e: b.w 0x0017ff92
  0017ed42: ldr r0,[0x0017eea4]
  0017ed44: add r0,pc
  0017ed46: ldr r4,[r0,#0x0]
  0017ed48: ldr r0,[r4,#0x0]
  0017ed4a: blx 0x00071b84
  0017ed4e: mov r6,r0
  0017ed50: mov r0,r9
  0017ed52: blx 0x00071bd8
  0017ed56: mov r1,r0
  0017ed58: mov r0,r6
  0017ed5a: blx 0x00077d94
  0017ed5e: ldr r6,[r4,#0x0]
  0017ed60: mov r0,r9
  0017ed62: blx 0x00071bfc
  0017ed66: rsbs r1,r0
  0017ed68: mov r0,r6
  0017ed6a: blx 0x0007171c
  0017ed6e: b.w 0x0017ff92
  0017ed72: ldr r0,[0x0017eea8]
  0017ed74: ldr r1,[0x0017eeac]
  0017ed76: add r0,pc
  0017ed78: add r1,pc
  0017ed7a: ldr r4,[r0,#0x0]
  0017ed7c: ldr r0,[r1,#0x0]
  0017ed7e: ldr r6,[r4,#0x0]
  0017ed80: ldrb.w r0,[r0,#0x38]
  0017ed84: cmp r0,#0x0
  0017ed86: beq.w 0x0017f1b6
  0017ed8a: mov r0,r9
  0017ed8c: blx 0x00074bc0
  0017ed90: rsbs r0,r0
  0017ed92: vmov s0,r0
  0017ed96: vcvt.f32.s32 s0,s0
  0017ed9a: b 0x0017f1ca
  0017ed9c: ldr r0,[0x0017eeb0]
  0017ed9e: ldr r1,[0x0017eeb4]
  0017eda0: add r0,pc
  0017eda2: add r1,pc
  0017eda4: ldr r2,[r0,#0x0]
  0017eda6: ldr r0,[r1,#0x0]
  0017eda8: str r2,[sp,#0x18]
  0017edaa: ldr r6,[r2,#0x0]
  0017edac: ldrb.w r0,[r0,#0x38]
  0017edb0: cmp r0,#0x0
  0017edb2: beq.w 0x0017f1f2
  0017edb6: mov r0,r9
  0017edb8: blx 0x00074bc0
  0017edbc: rsbs r0,r0
  0017edbe: vmov s0,r0
  0017edc2: vcvt.f32.s32 s0,s0
  0017edc6: b 0x0017f206
  0017edc8: ldr r0,[0x0017eeb8]
  0017edca: ldr r1,[0x0017eebc]
  0017edcc: add r0,pc
  0017edce: add r1,pc
  0017edd0: ldr.w r10,[r0,#0x0]
  0017edd4: ldr r0,[r1,#0x0]
  0017edd6: ldr.w r6,[r10,#0x0]
  0017edda: ldrb.w r0,[r0,#0x38]
  0017edde: cmp r0,#0x0
  0017ede0: beq.w 0x0017f2e0
  0017ede4: mov r0,r9
  0017ede6: blx 0x00074bc0
  0017edea: rsbs r0,r0
  0017edec: vmov s0,r0
  0017edf0: vcvt.f32.s32 s0,s0
  0017edf4: b 0x0017f2f4
  0017edf6: ldrd r0,r1,[r8,#0x20]
  0017edfa: ldr r1,[r1,#0x4]
  0017edfc: ldr.w r0,[r1,r0,lsl #0x2]
  0017ee00: movs r1,#0x1
  0017ee02: strb.w r1,[r0,#0x25]
  0017ee06: blx 0x00071bb4
  0017ee0a: blx 0x00073738
  0017ee0e: mov r5,r0
  0017ee10: ldr r0,[0x0017eec0]
  0017ee12: add r0,pc
  0017ee14: ldr r4,[r0,#0x0]
  0017ee16: ldr r0,[r4,#0x0]
  0017ee18: blx 0x00071c14
  0017ee1c: blx 0x00071824
  0017ee20: cmp r5,r0
  0017ee22: bne.w 0x0017f35a
  0017ee26: add r0,sp,#0x58
  0017ee28: mov r1,r9
  0017ee2a: ldr.w r6,[r8,#0x60]
  0017ee2e: blx 0x00071c2c
  0017ee32: ldr r0,[0x0017eec4]
  0017ee34: add r0,pc
  0017ee36: ldr r0,[r0,#0x0]
  0017ee38: ldr r0,[r0,#0x0]
  0017ee3a: movw r1,#0x325
  0017ee3e: blx 0x00072f70
  0017ee42: mov r1,r0
  0017ee44: add r0,sp,#0x4c
  0017ee46: movs r2,#0x0
  0017ee48: blx 0x0006f028
  0017ee4c: add r1,sp,#0x58
  0017ee4e: add r2,sp,#0x4c
  0017ee50: mov r0,r6
  0017ee52: blx 0x00075b2c
  0017ee56: add r0,sp,#0x4c
  0017ee58: blx 0x0006ee30
  0017ee5c: add r0,sp,#0x58
  0017ee5e: b 0x0017e9b0
  0017ee60: subs r0,r6,#0x2
  0017ee62: cmp r0,#0x8
  0017ee64: bhi.w 0x0017fda4
  0017ee68: tbh [pc,r0]
  0017eec8: ldr r0,[0x0017f25c]
  0017eeca: add r0,pc
  0017eecc: ldr r5,[r0,#0x0]
  0017eece: ldr r0,[r5,#0x0]
  0017eed0: mov.w r1,#0x364
  0017eed4: blx 0x00072f70
  0017eed8: mov r1,r0
  0017eeda: add r0,sp,#0x3a0
  0017eedc: blx 0x0006f2b0
  0017eee0: ldr r0,[sp,#0x18]
  0017eee2: ldr r6,[r0,#0x0]
  0017eee4: add r0,sp,#0x160
  0017eee6: add r1,sp,#0x3a0
  0017eee8: movs r2,#0x0
  0017eeea: blx 0x0006f028
  0017eeee: mov r0,r9
  0017eef0: blx 0x00074bcc
  0017eef4: mov r1,r0
  0017eef6: add r0,sp,#0x388
  0017eef8: blx 0x0006f658
  0017eefc: add r0,sp,#0x154
  0017eefe: add r1,sp,#0x388
  0017ef00: movs r2,#0x0
  0017ef02: blx 0x0006f028
  0017ef06: ldr r1,[0x0017f260]
  0017ef08: add r1,pc
  0017ef0a: add r0,sp,#0x148
  0017ef0c: movs r2,#0x0
  0017ef0e: blx 0x0006ee18
  0017ef12: str r0,[sp,#0x0]
  0017ef14: add r0,sp,#0x394
  0017ef16: add r2,sp,#0x160
  0017ef18: add r3,sp,#0x154
  0017ef1a: mov r1,r6
  0017ef1c: blx 0x000739b4
  0017ef20: add r0,sp,#0x3a0
  0017ef22: add r1,sp,#0x394
  0017ef24: blx 0x0006f2b0
  0017ef28: ldr r0,[0x0017f264]
  0017ef2a: add r0,pc
  0017ef2c: ldr r4,[r0,#0x0]
  0017ef2e: add r0,sp,#0x394
  0017ef30: blx r4
  0017ef32: add r0,sp,#0x148
  0017ef34: blx r4
  0017ef36: add r0,sp,#0x154
  0017ef38: blx r4
  0017ef3a: add r0,sp,#0x388
  0017ef3c: blx r4
  0017ef3e: add r0,sp,#0x160
  0017ef40: blx r4
  0017ef42: ldr r0,[sp,#0x18]
  0017ef44: ldr r6,[r0,#0x0]
  0017ef46: add r0,sp,#0x13c
  0017ef48: add r1,sp,#0x3a0
  0017ef4a: movs r2,#0x0
  0017ef4c: blx 0x0006f028
  0017ef50: ldr r5,[r5,#0x0]
  0017ef52: mov r0,r9
  0017ef54: blx 0x00074bb4
  0017ef58: addw r1,r0,#0x4fa
  0017ef5c: mov r0,r5
  0017ef5e: blx 0x00072f70
  0017ef62: mov r1,r0
  0017ef64: add r0,sp,#0x130
  0017ef66: movs r2,#0x0
  0017ef68: blx 0x0006f028
  0017ef6c: ldr r1,[0x0017f268]
  0017ef6e: add r1,pc
  0017ef70: add r0,sp,#0x124
  0017ef72: movs r2,#0x0
  0017ef74: blx 0x0006ee18
  0017ef78: str r0,[sp,#0x0]
  0017ef7a: add r0,sp,#0x394
  0017ef7c: add r2,sp,#0x13c
  0017ef7e: add r3,sp,#0x130
  0017ef80: mov r1,r6
  0017ef82: blx 0x000739b4
  0017ef86: add r0,sp,#0x3a0
  0017ef88: add r1,sp,#0x394
  0017ef8a: blx 0x0006f2b0
  0017ef8e: add r0,sp,#0x394
  0017ef90: blx r4
  0017ef92: add r0,sp,#0x124
  0017ef94: blx r4
  0017ef96: add r0,sp,#0x130
  0017ef98: blx r4
  0017ef9a: add r0,sp,#0x13c
  0017ef9c: blx r4
  0017ef9e: ldr r0,[sp,#0x18]
  0017efa0: ldr r5,[r0,#0x0]
  0017efa2: add r0,sp,#0x118
  0017efa4: add r1,sp,#0x3a0
  0017efa6: movs r2,#0x0
  0017efa8: blx 0x0006f028
  0017efac: ldr r0,[0x0017f26c]
  0017efae: add r0,pc
  0017efb0: ldr r0,[r0,#0x0]
  0017efb2: ldr r6,[r0,#0x0]
  0017efb4: mov r0,r9
  0017efb6: blx 0x00074bc0
  0017efba: vmov s0,r0
  0017efbe: vcvt.f32.s32 s0,s0
  0017efc2: vmul.f32 s0,s16,s0
  0017efc6: vcvt.s32.f32 s0,s0
  0017efca: vmov r2,s0
  0017efce: add r0,sp,#0x388
  0017efd0: mov r1,r6
  0017efd2: blx 0x00074e54
  0017efd6: add r0,sp,#0x10c
  0017efd8: add r1,sp,#0x388
  0017efda: movs r2,#0x0
  0017efdc: blx 0x0006f028
  0017efe0: ldr r1,[0x0017f270]
  0017efe2: add r1,pc
  0017efe4: add r0,sp,#0x100
  0017efe6: movs r2,#0x0
  0017efe8: blx 0x0006ee18
  0017efec: str r0,[sp,#0x0]
  0017efee: add r0,sp,#0x394
  0017eff0: add r2,sp,#0x118
  0017eff2: add r3,sp,#0x10c
  0017eff4: mov r1,r5
  0017eff6: blx 0x000739b4
  0017effa: add r0,sp,#0x3a0
  0017effc: add r1,sp,#0x394
  0017effe: blx 0x0006f2b0
  0017f002: add r0,sp,#0x394
  0017f004: blx r4
  0017f006: add r0,sp,#0x100
  0017f008: blx r4
  0017f00a: add r0,sp,#0x10c
  0017f00c: blx r4
  0017f00e: add r0,sp,#0x388
  0017f010: blx r4
  0017f012: add r0,sp,#0x118
  0017f014: b.w 0x0017fda2
  0017f018: movs r0,#0x48
  0017f01a: ldr.w r9,[r8,#0xc]
  0017f01e: blx 0x0006eb24
  0017f022: mov r5,r0
  0017f024: ldr r0,[0x0017f274]
  0017f026: add r0,pc
  0017f028: ldr r0,[r0,#0x0]
  0017f02a: ldr.w r10,[r0,#0x0]
  0017f02e: ldr r0,[r6,#0x0]
  0017f030: blx 0x000748a8
  0017f034: ldr r0,[r0,#0x4]
  0017f036: ldr.w r0,[r0,r4,lsl #0x2]
  0017f03a: blx 0x000733fc
  0017f03e: ldr.w r1,[r10,#0x4]
  0017f042: ldr.w r1,[r1,r0,lsl #0x2]
  0017f046: mov r0,r5
  0017f048: blx 0x00076198
  0017f04c: movs r0,#0x0
  0017f04e: mov r1,r5
  0017f050: strd r0,r0,[sp,#0x0]
  0017f054: str r0,[sp,#0x8]
  0017f056: mov r0,r9
  0017f058: movs r2,#0x0
  0017f05a: movs r3,#0x0
  0017f05c: blx 0x000771d0
  0017f060: movs r0,#0x1
  0017f062: strb.w r0,[r8,#0x1c]
  0017f066: b.w 0x0017fdbc
  0017f06a: ldr r0,[r5,#0x0]
  0017f06c: rsbs r1,r6
  0017f06e: blx 0x0007171c
  0017f072: ldr r0,[r5,#0x0]
  0017f074: blx 0x00073444
  0017f078: cmp r0,#0x0
  0017f07a: beq.w 0x0017fe30
  0017f07e: ldr r0,[sp,#0x10]
  0017f080: ldr r0,[r0,#0x0]
  0017f082: blx 0x00073444
  0017f086: blx 0x00072868
  0017f08a: cmp r0,#0x0
  0017f08c: bne.w 0x0017fe30
  0017f090: ldr r0,[sp,#0x10]
  0017f092: ldr r0,[r0,#0x0]
  0017f094: blx 0x00073444
  0017f098: blx 0x00072874
  0017f09c: cbz r0,0x0017f0c0
  0017f09e: ldr r0,[sp,#0x10]
  0017f0a0: ldr r0,[r0,#0x0]
  0017f0a2: blx 0x00073444
  0017f0a6: blx 0x00072874
  0017f0aa: cmp r0,#0x3
  0017f0ac: beq 0x0017f0c0
  0017f0ae: ldr r0,[sp,#0x10]
  0017f0b0: ldr r0,[r0,#0x0]
  0017f0b2: blx 0x00073444
  0017f0b6: blx 0x00072874
  0017f0ba: cmp r0,#0x5
  0017f0bc: bne.w 0x0017fc4c
  0017f0c0: ldr r0,[sp,#0x10]
  0017f0c2: ldr r0,[r0,#0x0]
  0017f0c4: blx 0x00071a58
  0017f0c8: blx 0x00073750
  0017f0cc: mov r6,r0
  0017f0ce: cmp r0,#0x0
  0017f0d0: beq.w 0x0017fdfc
  0017f0d4: movs r4,#0x0
  0017f0d6: b 0x0017f0f6
  0017f0d8: ldr r0,[r6,#0x4]
  0017f0da: ldr.w r0,[r0,r4,lsl #0x2]
  0017f0de: blx 0x00074ad0
  0017f0e2: cbz r0,0x0017f0f4
  0017f0e4: ldr r0,[r6,#0x4]
  0017f0e6: ldr.w r0,[r0,r4,lsl #0x2]
  0017f0ea: blx 0x000718d8
  0017f0ee: cmp r0,#0x74
  0017f0f0: beq.w 0x0017fde4
  0017f0f4: adds r4,#0x1
  0017f0f6: ldr r0,[r6,#0x0]
  0017f0f8: cmp r4,r0
  0017f0fa: bcc 0x0017f0d8
  0017f0fc: b.w 0x0017fdfc
  0017f100: mov r0,r9
  0017f102: blx 0x00071bb4
  0017f106: blx 0x00072874
  0017f10a: cmp r0,#0x0
  0017f10c: beq.w 0x0017f5b2
  0017f110: mov r0,r9
  0017f112: blx 0x00071bb4
  0017f116: blx 0x00072874
  0017f11a: cmp r0,#0xb
  0017f11c: bne.w 0x0017f5d6
  0017f120: mov r0,r9
  0017f122: blx 0x00071bb4
  0017f126: blx 0x00072bb0
  0017f12a: mov r6,r0
  0017f12c: ldr r0,[0x0017f278]
  0017f12e: add r0,pc
  0017f130: ldr r5,[r0,#0x0]
  0017f132: ldr r0,[r5,#0x0]
  0017f134: blx 0x00071a58
  0017f138: blx 0x00072bd4
  0017f13c: cmp r6,r0
  0017f13e: ble.w 0x0017f5d6
  0017f142: ldr r0,[0x0017f27c]
  0017f144: add r0,pc
  0017f146: ldr r0,[r0,#0x0]
  0017f148: ldr r0,[r0,#0x0]
  0017f14a: mov.w r1,#0x152
  0017f14e: blx 0x00072f70
  0017f152: mov r1,r0
  0017f154: add r0,sp,#0x3a0
  0017f156: blx 0x0006f2b0
  0017f15a: ldr r5,[r5,#0x0]
  0017f15c: add r0,sp,#0x2c8
  0017f15e: add r1,sp,#0x3a0
  0017f160: movs r2,#0x0
  0017f162: blx 0x0006f028
  0017f166: add r0,sp,#0x388
  0017f168: mov r1,r6
  0017f16a: blx 0x0006f658
  0017f16e: add r0,sp,#0x2bc
  0017f170: add r1,sp,#0x388
  0017f172: movs r2,#0x0
  0017f174: blx 0x0006f028
  0017f178: ldr r1,[0x0017f280]
  0017f17a: add r1,pc
  0017f17c: add r0,sp,#0x2b0
  0017f17e: movs r2,#0x0
  0017f180: blx 0x0006ee18
  0017f184: str r0,[sp,#0x0]
  0017f186: add r0,sp,#0x394
  0017f188: add r2,sp,#0x2c8
  0017f18a: add r3,sp,#0x2bc
  0017f18c: mov r1,r5
  0017f18e: blx 0x000739b4
  0017f192: add r0,sp,#0x3a0
  0017f194: add r1,sp,#0x394
  0017f196: blx 0x0006f2b0
  0017f19a: ldr r0,[0x0017f284]
  0017f19c: add r0,pc
  0017f19e: ldr r5,[r0,#0x0]
  0017f1a0: add r0,sp,#0x394
  0017f1a2: blx r5
  0017f1a4: add r0,sp,#0x2b0
  0017f1a6: blx r5
  0017f1a8: add r0,sp,#0x2bc
  0017f1aa: blx r5
  0017f1ac: add r0,sp,#0x388
  0017f1ae: blx r5
  0017f1b0: add r0,sp,#0x2c8
  0017f1b2: b.w 0x0017fc3a
  0017f1b6: mov r0,r9
  0017f1b8: blx 0x00074bc0
  0017f1bc: rsbs r0,r0
  0017f1be: vmov s0,r0
  0017f1c2: vcvt.f32.s32 s0,s0
  0017f1c6: vadd.f32 s0,s0,s0
  0017f1ca: vcvt.s32.f32 s0,s0
  0017f1ce: vmov r1,s0
  0017f1d2: mov r0,r6
  0017f1d4: blx 0x0007171c
  0017f1d8: ldr r0,[r4,#0x0]
  0017f1da: blx 0x00071a58
  0017f1de: mov r6,r0
  0017f1e0: mov r0,r9
  0017f1e2: blx 0x00074bfc
  0017f1e6: mov r1,r0
  0017f1e8: mov r0,r6
  0017f1ea: blx 0x000748d8
  0017f1ee: b.w 0x0017ff8c
  0017f1f2: mov r0,r9
  0017f1f4: blx 0x00074bc0
  0017f1f8: rsbs r0,r0
  0017f1fa: vmov s0,r0
  0017f1fe: vcvt.f32.s32 s0,s0
  0017f202: vadd.f32 s0,s0,s0
  0017f206: vcvt.s32.f32 s0,s0
  0017f20a: vmov r1,s0
  0017f20e: mov r0,r6
  0017f210: blx 0x0007171c
  0017f214: ldr r0,[0x0017f288]
  0017f216: add r0,pc
  0017f218: ldr r0,[r0,#0x0]
  0017f21a: ldr r4,[r0,#0x0]
  0017f21c: mov r0,r9
  0017f21e: blx 0x00074bb4
  0017f222: ldr r1,[r4,#0x4]
  0017f224: ldr.w r5,[r1,r0,lsl #0x2]
  0017f228: mov r0,r9
  0017f22a: blx 0x00074bcc
  0017f22e: mov r1,r0
  0017f230: mov r0,r5
  0017f232: blx 0x000718c0
  0017f236: mov r4,r0
  0017f238: blx 0x000718fc
  0017f23c: cmp r0,#0x1
  0017f23e: bne 0x0017f2ce
  0017f240: ldr r0,[sp,#0x18]
  0017f242: ldr r0,[r0,#0x0]
  0017f244: blx 0x00071a58
  0017f248: blx 0x0007333c
  0017f24c: mov r6,r0
  0017f24e: cbz r0,0x0017f2ce
  0017f250: movs r0,#0x0
  0017f252: mov.w r10,#0x0
  0017f256: str r0,[sp,#0x14]
  0017f258: b 0x0017f2c0
  0017f28c: ldr r0,[r6,#0x4]
  0017f28e: ldr.w r0,[r0,r10,lsl #0x2]
  0017f292: cbz r0,0x0017f2bc
  0017f294: blx 0x000718d8
  0017f298: mov r5,r0
  0017f29a: mov r0,r4
  0017f29c: blx 0x000718d8
  0017f2a0: cmp r5,r0
  0017f2a2: bne 0x0017f2bc
  0017f2a4: ldr r0,[r6,#0x4]
  0017f2a6: ldr.w r5,[r0,r10,lsl #0x2]
  0017f2aa: mov r0,r4
  0017f2ac: blx 0x0007183c
  0017f2b0: mov r1,r0
  0017f2b2: mov r0,r5
  0017f2b4: blx 0x00071854
  0017f2b8: movs r0,#0x1
  0017f2ba: str r0,[sp,#0x14]
  0017f2bc: add.w r10,r10,#0x1
  0017f2c0: ldr r0,[r6,#0x0]
  0017f2c2: cmp r10,r0
  0017f2c4: bcc 0x0017f28c
  0017f2c6: ldr r0,[sp,#0x14]
  0017f2c8: lsls r0,r0,#0x1f
  0017f2ca: bne.w 0x0017ff8c
  0017f2ce: ldr r0,[sp,#0x18]
  0017f2d0: ldr r0,[r0,#0x0]
  0017f2d2: blx 0x00071a58
  0017f2d6: mov r1,r4
  0017f2d8: blx 0x00072cdc
  0017f2dc: b.w 0x0017ff8c
  0017f2e0: mov r0,r9
  0017f2e2: blx 0x00074bc0
  0017f2e6: rsbs r0,r0
  0017f2e8: vmov s0,r0
  0017f2ec: vcvt.f32.s32 s0,s0
  0017f2f0: vadd.f32 s0,s0,s0
  0017f2f4: vcvt.s32.f32 s0,s0
  0017f2f8: vmov r1,s0
  0017f2fc: mov r0,r6
  0017f2fe: blx 0x0007171c
  0017f302: ldr r0,[0x0017f6e4]
  0017f304: add r0,pc
  0017f306: ldr r5,[r0,#0x0]
  0017f308: ldr.w r0,[r10,#0x0]
  0017f30c: ldr r4,[r5,#0x0]
  0017f30e: ldr.w r6,[r0,#0x14c]
  0017f312: mov r0,r9
  0017f314: blx 0x00074bb4
  0017f318: ldr r1,[r4,#0x4]
  0017f31a: ldr.w r0,[r1,r0,lsl #0x2]
  0017f31e: mov.w r1,#0xffffffff
  0017f322: blx 0x000719d4
  0017f326: mov r1,r0
  0017f328: mov r0,r6
  0017f32a: blx 0x00075004
  0017f32e: ldr.w r0,[r10,#0x0]
  0017f332: blx 0x00071c14
  0017f336: ldr r4,[r5,#0x0]
  0017f338: mov r6,r0
  0017f33a: mov r0,r9
  0017f33c: blx 0x00074bb4
  0017f340: ldr r1,[r4,#0x4]
  0017f342: ldr.w r0,[r1,r0,lsl #0x2]
  0017f346: mov.w r1,#0xffffffff
  0017f34a: blx 0x000719d4
  0017f34e: mov r1,r0
  0017f350: mov r0,r6
  0017f352: blx 0x00075004
  0017f356: b.w 0x0017ff8c
  0017f35a: ldr r0,[r4,#0x0]
  0017f35c: blx 0x00071a10
  0017f360: mov r5,r0
  0017f362: ldrd r0,r1,[r8,#0x20]
  0017f366: ldr r1,[r1,#0x4]
  0017f368: ldr.w r0,[r1,r0,lsl #0x2]
  0017f36c: blx 0x00071bb4
  0017f370: blx 0x00073738
  0017f374: mov r1,r0
  0017f376: mov r0,r5
  0017f378: blx 0x00071a94
  0017f37c: cmp r0,#0x0
  0017f37e: beq.w 0x0017f540
  0017f382: add r0,sp,#0x40
  0017f384: mov r1,r9
  0017f386: ldr.w r6,[r8,#0x60]
  0017f38a: blx 0x00071c2c
  0017f38e: ldr r0,[0x0017f6e8]
  0017f390: add r0,pc
  0017f392: ldr r0,[r0,#0x0]
  0017f394: ldr r0,[r0,#0x0]
  0017f396: movw r1,#0x326
  0017f39a: blx 0x00072f70
  0017f39e: mov r1,r0
  0017f3a0: add r0,sp,#0x34
  0017f3a2: movs r2,#0x0
  0017f3a4: blx 0x0006f028
  0017f3a8: add r1,sp,#0x40
  0017f3aa: add r2,sp,#0x34
  0017f3ac: mov r0,r6
  0017f3ae: blx 0x00075b2c
  0017f3b2: add r0,sp,#0x34
  0017f3b4: blx 0x0006ee30
  0017f3b8: add r0,sp,#0x40
  0017f3ba: b.w 0x0017e9b0
  0017f3be: mov r0,r9
  0017f3c0: blx 0x00071bfc
  0017f3c4: rsbs r1,r0
  0017f3c6: mov r0,r6
  0017f3c8: blx 0x0007171c
  0017f3cc: b.w 0x0017ff92
  0017f3d0: movs r0,#0x0
  0017f3d2: str.w r0,[r8,#0x14]
  0017f3d6: b.w 0x0017e8e6
  0017f3da: ldr r0,[0x0017f6ec]
  0017f3dc: add r0,pc
  0017f3de: ldr r6,[r0,#0x0]
  0017f3e0: ldr r0,[r6,#0x0]
  0017f3e2: blx 0x00075580
  0017f3e6: cmp r0,#0x0
  0017f3e8: bne.w 0x0017f57c
  0017f3ec: ldr r0,[0x0017f6f0]
  0017f3ee: add r0,pc
  0017f3f0: ldr r0,[r0,#0x0]
  0017f3f2: str r0,[sp,#0x18]
  0017f3f4: ldr r0,[r0,#0x0]
  0017f3f6: blx 0x000733d8
  0017f3fa: mov r5,r0
  0017f3fc: mov r0,r9
  0017f498: ldr r0,[0x0017f704]
  0017f49a: add r0,pc
  0017f49c: ldr r6,[r0,#0x0]
  0017f49e: ldr r0,[r6,#0x0]
  0017f4a0: blx 0x000733d8
  0017f4a4: mov r5,r0
  0017f4a6: mov r0,r9
  0017f4a8: blx 0x00071bfc
  0017f4ac: cmp r5,r0
  0017f4ae: bge.w 0x0017f7de
  0017f4b2: ldr r0,[0x0017f708]
  0017f4b4: ldr.w r1,[r8,#0x8]
  0017f4b8: add r0,pc
  0017f4ba: ldr.w r10,[r6,#0x0]
  0017f4be: str r1,[sp,#0x18]
  0017f4c0: ldr r0,[r0,#0x0]
  0017f4c2: ldr r0,[r0,#0x0]
  0017f4c4: movs r1,#0xcb
  0017f4c6: blx 0x00072f70
  0017f4ca: mov r1,r0
  0017f4cc: add r0,sp,#0x334
  0017f4ce: movs r2,#0x0
  0017f4d0: blx 0x0006f028
  0017f4d4: ldr r0,[0x0017f70c]
  0017f4d6: add r0,pc
  0017f4d8: ldr r0,[r0,#0x0]
  0017f4da: ldr.w r8,[r0,#0x0]
  0017f4de: mov r0,r9
  0017f4e0: blx 0x00071bfc
  0017f4e4: mov r5,r0
  0017f4e6: ldr r0,[r6,#0x0]
  0017f4e8: blx 0x000733d8
  0017f4ec: subs r2,r5,r0
  0017f4ee: add r0,sp,#0x388
  0017f4f0: mov r1,r8
  0017f4f2: blx 0x00074e54
  0017f4f6: add r0,sp,#0x328
  0017f4f8: add r1,sp,#0x388
  0017f4fa: movs r2,#0x0
  0017f4fc: blx 0x0006f028
  0017f500: ldr r1,[0x0017f710]
  0017f502: add r1,pc
  0017f504: add r0,sp,#0x31c
  0017f506: movs r2,#0x0
  0017f508: blx 0x0006ee18
  0017f50c: str r0,[sp,#0x0]
  0017f50e: add r0,sp,#0x394
  0017f510: add r2,sp,#0x334
  0017f512: add r3,sp,#0x328
  0017f514: mov r1,r10
  0017f516: blx 0x000739b4
  0017f51a: ldr r0,[sp,#0x18]
  0017f51c: add r1,sp,#0x394
  0017f51e: movs r2,#0x0
  0017f520: blx 0x000746f8
  0017f524: ldr r0,[0x0017f714]
  0017f526: add r0,pc
  0017f528: ldr r5,[r0,#0x0]
  0017f52a: add r0,sp,#0x394
  0017f52c: blx r5
  0017f52e: add r0,sp,#0x31c
  0017f530: blx r5
  0017f532: add r0,sp,#0x328
  0017f534: blx r5
  0017f536: add r0,sp,#0x388
  0017f538: blx r5
  0017f53a: add r0,sp,#0x334
  0017f53c: b.w 0x0017e8e0
  0017f540: ldr r0,[0x0017f718]
  0017f542: movs r1,#0x5
  0017f544: add r0,pc
  0017f546: ldr r4,[r0,#0x0]
  0017f548: ldr r0,[r4,#0x0]
  0017f54a: blx 0x00075bd4
  0017f54e: ldr r5,[r0,#0x10]
  0017f550: str.w r5,[r8,#0x4]
  0017f554: cmp r5,#0x0
  0017f556: beq.w 0x0017fc6a
  0017f55a: ldrd r0,r1,[r8,#0x20]
  0017f55e: ldr r1,[r1,#0x4]
  0017f560: ldr.w r0,[r1,r0,lsl #0x2]
  0017f564: blx 0x00071bb4
  0017f568: mov r2,r0
  0017f56a: mov.w r0,#0xffffffff
  0017f56e: str r0,[sp,#0x0]
  0017f570: mov r0,r5
  0017f572: movs r1,#0x1
  0017f574: movs r3,#0x0
  0017f576: blx 0x00074554
  0017f57a: b 0x0017fcac
  0017f57c: ldr r0,[0x0017f71c]
  0017f57e: add r0,pc
  0017f580: ldr r0,[r0,#0x0]
  0017f582: str r0,[sp,#0x18]
  0017f584: ldr r0,[r0,#0x0]
  0017f586: blx 0x000741dc
  0017f58a: cmp r0,#0x0
  0017f58c: beq.w 0x0017fcb4
  0017f590: ldr r0,[0x0017f720]
  0017f592: ldr.w r5,[r8,#0x8]
  0017f596: add r0,pc
  0017f598: ldr r0,[r0,#0x0]
  0017f59a: ldr r0,[r0,#0x0]
  0017f59c: movw r1,#0x311
  0017f5a0: blx 0x00072f70
  0017f5a4: mov r1,r0
  0017f5a6: mov r0,r5
  0017f5a8: movs r2,#0x0
  0017f5aa: blx 0x000746f8
  0017f5ae: b.w 0x0017e8e2
  0017f5b2: mov r0,r9
  0017f5b4: blx 0x00071bb4
  0017f5b8: blx 0x00072bb0
  0017f5bc: mov r6,r0
  0017f5be: ldr r0,[0x0017f724]
  0017f5c0: add r0,pc
  0017f5c2: ldr r5,[r0,#0x0]
  0017f5c4: ldr r0,[r5,#0x0]
  0017f5c6: blx 0x00071a58
  0017f5ca: mov r1,r6
  0017f5cc: blx 0x00073318
  0017f5d0: cmp r0,#0x0
  0017f5d2: beq.w 0x0017fbca
  0017f5d6: mov r0,r9
  0017f5d8: blx 0x00071b9c
  0017f5dc: mov r1,r0
  0017f5de: ldr r0,[0x0017f728]
  0017f5e0: cmp r1,#0x0
  0017f5e2: add r0,pc
  0017f5e4: ldr r0,[r0,#0x0]
  0017f5e6: str r0,[sp,#0x18]
  0017f5e8: ldr r0,[r0,#0x0]
  0017f5ea: beq.w 0x0017f740
  0017f5ee: movw r1,#0x361
  0017f5f2: blx 0x00072f70
  0017f5f6: mov r1,r0
  0017f5f8: add r0,sp,#0x3a0
  0017f5fa: blx 0x0006f2b0
  0017f5fe: ldr r0,[0x0017f72c]
  0017f600: add r0,pc
  0017f602: ldr r5,[r0,#0x0]
  0017f604: ldr r0,[r5,#0x0]
  0017f606: str r0,[sp,#0x14]
  0017f608: add r0,sp,#0x2a4
  0017f60a: add r1,sp,#0x3a0
  0017f60c: movs r2,#0x0
  0017f60e: blx 0x0006f028
  0017f612: ldr r0,[0x0017f730]
  0017f614: add r0,pc
  0017f616: ldr r0,[r0,#0x0]
  0017f618: ldr r4,[r0,#0x0]
  0017f61a: mov r0,r9
  0017f61c: blx 0x00071bb4
  0017f620: blx 0x00071bc0
  0017f624: mov r6,r0
  0017f626: mov r0,r9
  0017f628: b 0xff2bd1f8
  0017f740: movw r1,#0x35f
  0017f744: blx 0x00072f70
  0017f748: mov r1,r0
  0017f74a: add r0,sp,#0x3a0
  0017f74c: blx 0x0006f2b0
  0017f750: ldr r0,[0x0017fadc]
  0017f752: add r0,pc
  0017f754: ldr r0,[r0,#0x0]
  0017f756: ldr r0,[r0,#0x0]
  0017f758: blx 0x00073444
  0017f75c: cmp r0,#0x0
  0017f75e: beq.w 0x0017fda4
  0017f762: blx 0x00072868
  0017f766: cmp r0,#0x0
  0017f768: bne.w 0x0017fda4
  0017f76c: ldr r1,[0x0017fae0]
  0017f76e: add r1,pc
  0017f770: add r0,sp,#0x388
  0017f772: movs r2,#0x0
  0017f774: blx 0x0006ee18
  0017f778: ldr r0,[sp,#0x18]
  0017f77a: ldr r0,[r0,#0x0]
  0017f77c: mov.w r1,#0x360
  0017f780: blx 0x00072f70
  0017f784: mov r2,r0
  0017f786: add r0,sp,#0x394
  0017f788: add r1,sp,#0x388
  0017f78a: blx 0x0006ef98
  0017f78e: add r0,sp,#0x3a0
  0017f790: add r1,sp,#0x394
  0017f792: blx 0x0006ef5c
  0017f796: add r0,sp,#0x394
  0017f798: blx 0x0006ee30
  0017f79c: add r0,sp,#0x388
  0017f79e: blx 0x0006ee30
  0017f7a2: b 0x0017fda4
  0017f7a4: ldr r0,[r4,#0x0]
  0017f7a6: blx 0x000748a8
  0017f7aa: ldr r0,[r0,#0x4]
  0017f7ac: ldr.w r0,[r0,r6,lsl #0x2]
  0017f7b0: blx 0x000738a0
  0017f7b4: ldr r0,[r4,#0x0]
  0017f7b6: blx 0x00071ab8
  0017f7ba: mov r6,r0
  0017f7bc: mov r0,r9
  0017f7be: blx 0x00074ba8
  0017f7c2: ldr r1,[r6,#0x0]
  0017f7c4: cmp r0,r1
  0017f7c6: bcs.w 0x0017ff8c
  0017f7ca: mov r0,r9
  0017f7cc: blx 0x00074ba8
  0017f7d0: ldr r1,[r6,#0x4]
  0017f7d2: ldr.w r0,[r1,r0,lsl #0x2]
  0017f7d6: movs r1,#0x1
  0017f7d8: blx 0x00071b18
  0017f7dc: b 0x0017ff8c
  0017f7de: ldr r0,[0x0017fae4]
  0017f7e0: add r0,pc
  0017f7e2: ldr r0,[r0,#0x0]
  0017f7e4: ldr r0,[r0,#0x0]
  0017f7e6: movw r1,#0x375
  0017f7ea: blx 0x00072f70
  0017f7ee: mov r1,r0
  0017f7f0: add r0,sp,#0x3a0
  0017f7f2: blx 0x0006f2b0
  0017f7f6: ldr r6,[r6,#0x0]
  0017f7f8: add r0,sp,#0x310
  0017f7fa: add r1,sp,#0x3a0
  0017f7fc: movs r2,#0x0
  0017f7fe: blx 0x0006f028
  0017f802: ldr r0,[0x0017fae8]
  0017f804: add r0,pc
  0017f806: ldr r0,[r0,#0x0]
  0017f808: ldr r5,[r0,#0x0]
  0017f80a: mov r0,r9
  0017f80c: blx 0x00071bfc
  0017f810: mov r2,r0
  0017f812: add r0,sp,#0x388
  0017f814: mov r1,r5
  0017f816: blx 0x00074e54
  0017f81a: add r0,sp,#0x304
  0017f81c: add r1,sp,#0x388
  0017f81e: movs r2,#0x0
  0017f820: blx 0x0006f028
  0017f824: ldr r1,[0x0017faec]
  0017f826: add r1,pc
  0017f828: add r0,sp,#0x2f8
  0017f82a: movs r2,#0x0
  0017f82c: blx 0x0006ee18
  0017f830: str r0,[sp,#0x0]
  0017f832: add r0,sp,#0x394
  0017f834: add r2,sp,#0x310
  0017f836: add r3,sp,#0x304
  0017f838: mov r1,r6
  0017f83a: blx 0x000739b4
  0017f83e: add r0,sp,#0x3a0
  0017f840: add r1,sp,#0x394
  0017f842: blx 0x0006f2b0
  0017f846: ldr r0,[0x0017faf0]
  0017f848: add r0,pc
  0017f84a: ldr r4,[r0,#0x0]
  0017f84c: add r0,sp,#0x394
  0017f84e: blx r4
  0017f850: add r0,sp,#0x2f8
  0017f852: blx r4
  0017f854: add r0,sp,#0x304
  0017f856: blx r4
  0017f858: add r0,sp,#0x388
  0017f85a: blx r4
  0017f85c: add r0,sp,#0x310
  0017f85e: b 0x0017fda2
  0017f860: ldr r0,[0x0017faf4]
  0017f862: add r0,pc
  0017f864: ldr r4,[r0,#0x0]
  0017f866: ldr r0,[r4,#0x0]
  0017f868: movw r1,#0x365
  0017f86c: blx 0x00072f70
  0017f870: mov r1,r0
  0017f872: add r0,sp,#0x3a0
  0017f874: blx 0x0006f2b0
  0017f878: ldr r0,[sp,#0x18]
  0017f87a: ldr r0,[r0,#0x0]
  0017f87c: str r0,[sp,#0x14]
  0017f87e: add r0,sp,#0x1f0
  0017f880: add r1,sp,#0x3a0
  0017f882: movs r2,#0x0
  0017f884: blx 0x0006f028
  0017f888: ldr r0,[0x0017faf8]
  0017f88a: ldr r6,[r4,#0x0]
  0017f88c: add r0,pc
  0017f88e: ldr r0,[r0,#0x0]
  0017f890: ldr r5,[r0,#0x0]
  0017f892: mov r0,r9
  0017f894: blx 0x00074b90
  0017f898: ldr r1,[r5,#0x4]
  0017f89a: ldr.w r0,[r1,r0,lsl #0x2]
  0017f89e: blx 0x000718d8
  0017f8a2: addw r1,r0,#0x4fa
  0017f8a6: mov r0,r6
  0017f8a8: blx 0x00072f70
  0017f8ac: mov r1,r0
  0017f8ae: add r0,sp,#0x1e4
  0017f8b0: movs r2,#0x0
  0017f8b2: blx 0x0006f028
  0017f8b6: ldr r1,[0x0017fafc]
  0017f8b8: add r1,pc
  0017f8ba: add r0,sp,#0x1d8
  0017f8bc: movs r2,#0x0
  0017f8be: blx 0x0006ee18
  0017f8c2: ldr r1,[sp,#0x14]
  0017f8c4: add r2,sp,#0x1f0
  0017f8c6: str r0,[sp,#0x0]
  0017f8c8: add r0,sp,#0x394
  0017f8ca: add r3,sp,#0x1e4
  0017f8cc: blx 0x000739b4
  0017f8d0: add r0,sp,#0x3a0
  0017f8d2: add r1,sp,#0x394
  0017f8d4: blx 0x0006f2b0
  0017f8d8: ldr r0,[0x0017fb00]
  0017f8da: add r0,pc
  0017f8dc: ldr r4,[r0,#0x0]
  0017f8de: add r0,sp,#0x394
  0017f8e0: blx r4
  0017f8e2: add r0,sp,#0x1d8
  0017f8e4: blx r4
  0017f8e6: add r0,sp,#0x1e4
  0017f8e8: blx r4
  0017f8ea: add r0,sp,#0x1f0
  0017f8ec: blx r4
  0017f8ee: ldr r0,[sp,#0x18]
  0017f8f0: ldr r6,[r0,#0x0]
  0017f8f2: add r0,sp,#0x1cc
  0017f8f4: add r1,sp,#0x3a0
  0017f8f6: movs r2,#0x0
  0017f8f8: blx 0x0006f028
  0017f8fc: ldr r0,[0x0017fb04]
  0017f8fe: add r0,pc
  0017f900: ldr r0,[r0,#0x0]
  0017f902: ldr r5,[r0,#0x0]
  0017f904: mov r0,r9
  0017f906: blx 0x00074bc0
  0017f90a: mov r2,r0
  0017f90c: add r0,sp,#0x388
  0017f90e: mov r1,r5
  0017f910: blx 0x00074e54
  0017f914: add r0,sp,#0x1c0
  0017f916: add r1,sp,#0x388
  0017f918: movs r2,#0x0
  0017f91a: blx 0x0006f028
  0017f91e: ldr r1,[0x0017fb08]
  0017f920: add r1,pc
  0017f922: add r0,sp,#0x1b4
  0017f924: movs r2,#0x0
  0017f926: blx 0x0006ee18
  0017f92a: str r0,[sp,#0x0]
  0017f92c: add r0,sp,#0x394
  0017f92e: add r2,sp,#0x1cc
  0017f930: add r3,sp,#0x1c0
  0017f932: mov r1,r6
  0017f934: blx 0x000739b4
  0017f938: add r0,sp,#0x3a0
  0017f93a: add r1,sp,#0x394
  0017f93c: blx 0x0006f2b0
  0017f940: add r0,sp,#0x394
  0017f942: blx r4
  0017f944: add r0,sp,#0x1b4
  0017f946: blx r4
  0017f948: add r0,sp,#0x1c0
  0017f94a: blx r4
  0017f94c: add r0,sp,#0x388
  0017f94e: blx r4
  0017f950: add r0,sp,#0x1cc
  0017f952: b 0x0017fda2
  0017f954: ldr r0,[0x0017fb0c]
  0017f956: add r0,pc
  0017f958: ldr r0,[r0,#0x0]
  0017f95a: ldr r0,[r0,#0x0]
  0017f95c: movw r1,#0x366
  0017f960: blx 0x00072f70
  0017f964: mov r1,r0
  0017f966: add r0,sp,#0x3a0
  0017f968: blx 0x0006f2b0
  0017f96c: ldr r0,[0x0017fb10]
  0017f96e: add r0,pc
  0017f970: ldr r0,[r0,#0x0]
  0017f972: ldr r5,[r0,#0x0]
  0017f974: mov r0,r9
  0017f976: blx 0x00074b84
  0017f97a: mov r1,r0
  0017f97c: mov r0,r5
  0017f97e: blx 0x00073684
  0017f982: mov r6,r0
  0017f984: ldr r0,[sp,#0x18]
  0017f986: ldr r5,[r0,#0x0]
  0017f988: add r0,sp,#0x1a8
  0017f98a: add r1,sp,#0x3a0
  0017f98c: movs r2,#0x0
  0017f98e: blx 0x0006f028
  0017f992: add r0,sp,#0x388
  0017f994: mov r1,r6
  0017f996: blx 0x00071cc8
  0017f99a: add r0,sp,#0x19c
  0017f99c: add r1,sp,#0x388
  0017f99e: movs r2,#0x0
  0017f9a0: blx 0x0006f028
  0017f9a4: ldr r1,[0x0017fb14]
  0017f9a6: add r1,pc
  0017f9a8: add r0,sp,#0x190
  0017f9aa: movs r2,#0x0
  0017f9ac: blx 0x0006ee18
  0017f9b0: str r0,[sp,#0x0]
  0017f9b2: add r0,sp,#0x394
  0017f9b4: add r2,sp,#0x1a8
  0017f9b6: add r3,sp,#0x19c
  0017f9b8: mov r1,r5
  0017f9ba: blx 0x000739b4
  0017f9be: add r0,sp,#0x3a0
  0017f9c0: add r1,sp,#0x394
  0017f9c2: blx 0x0006f2b0
  0017f9c6: ldr r0,[0x0017fb18]
  0017f9c8: add r0,pc
  0017f9ca: ldr r4,[r0,#0x0]
  0017f9cc: add r0,sp,#0x394
  0017f9ce: blx r4
  0017f9d0: add r0,sp,#0x190
  0017f9d2: blx r4
  0017f9d4: add r0,sp,#0x19c
  0017f9d6: blx r4
  0017f9d8: add r0,sp,#0x388
  0017f9da: blx r4
  0017f9dc: add r0,sp,#0x1a8
  0017f9de: blx r4
  0017f9e0: ldr r0,[sp,#0x18]
  0017f9e2: ldr r6,[r0,#0x0]
  0017f9e4: add r0,sp,#0x184
  0017f9e6: add r1,sp,#0x3a0
  0017f9e8: movs r2,#0x0
  0017f9ea: blx 0x0006f028
  0017f9ee: ldr r0,[0x0017fb1c]
  0017f9f0: add r0,pc
  0017f9f2: ldr r0,[r0,#0x0]
  0017f9f4: ldr r5,[r0,#0x0]
  0017f9f6: mov r0,r9
  0017f9f8: blx 0x00074bc0
  0017f9fc: mov r2,r0
  0017f9fe: add r0,sp,#0x388
  0017fa00: mov r1,r5
  0017fa02: blx 0x00074e54
  0017fa06: add r0,sp,#0x178
  0017fa08: add r1,sp,#0x388
  0017fa0a: movs r2,#0x0
  0017fa0c: blx 0x0006f028
  0017fa10: ldr r1,[0x0017fb20]
  0017fa12: add r1,pc
  0017fa14: add r0,sp,#0x16c
  0017fa16: movs r2,#0x0
  0017fa18: blx 0x0006ee18
  0017fa1c: str r0,[sp,#0x0]
  0017fa1e: add r0,sp,#0x394
  0017fa20: add r2,sp,#0x184
  0017fa22: add r3,sp,#0x178
  0017fa24: mov r1,r6
  0017fa26: blx 0x000739b4
  0017fa2a: add r0,sp,#0x3a0
  0017fa2c: add r1,sp,#0x394
  0017fa2e: blx 0x0006f2b0
  0017fa32: add r0,sp,#0x394
  0017fa34: blx r4
  0017fa36: add r0,sp,#0x16c
  0017fa38: blx r4
  0017fa3a: add r0,sp,#0x178
  0017fa3c: blx r4
  0017fa3e: add r0,sp,#0x388
  0017fa40: blx r4
  0017fa42: add r0,sp,#0x184
  0017fa44: b 0x0017fda2
  0017fa46: ldr r0,[0x0017fb24]
  0017fa48: add r0,pc
  0017fa4a: ldr r0,[r0,#0x0]
  0017fa4c: ldr r0,[r0,#0x0]
  0017fa4e: movw r1,#0x367
  0017fa52: blx 0x00072f70
  0017fa56: mov r1,r0
  0017fa58: add r0,sp,#0x3a0
  0017fa5a: blx 0x0006f2b0
  0017fa5e: ldr r0,[sp,#0x18]
  0017fa60: ldr r5,[r0,#0x0]
  0017fa62: add r0,sp,#0x238
  0017fa64: add r1,sp,#0x3a0
  0017fa66: movs r2,#0x0
  0017fa68: blx 0x0006f028
  0017fa6c: ldr r0,[0x0017fb28]
  0017fa6e: add r0,pc
  0017fa70: ldr r0,[r0,#0x0]
  0017fa72: ldr r6,[r0,#0x0]
  0017fa74: mov r0,r9
  0017fa76: blx 0x00074bc0
  0017fa7a: vmov s0,r0
  0017fa7e: vcvt.f32.s32 s0,s0
  0017fa82: vmul.f32 s0,s16,s0
  0017fa86: vcvt.s32.f32 s0,s0
  0017fa8a: vmov r2,s0
  0017fa8e: add r0,sp,#0x388
  0017fa90: mov r1,r6
  0017fa92: blx 0x00074e54
  0017fa96: add r0,sp,#0x22c
  0017fa98: add r1,sp,#0x388
  0017fa9a: movs r2,#0x0
  0017fa9c: blx 0x0006f028
  0017faa0: ldr r1,[0x0017fb2c]
  0017faa2: add r1,pc
  0017faa4: add r0,sp,#0x220
  0017faa6: movs r2,#0x0
  0017faa8: blx 0x0006ee18
  0017faac: str r0,[sp,#0x0]
  0017faae: add r0,sp,#0x394
  0017fab0: add r2,sp,#0x238
  0017fab2: add r3,sp,#0x22c
  0017fab4: mov r1,r5
  0017fab6: blx 0x000739b4
  0017faba: add r0,sp,#0x3a0
  0017fabc: add r1,sp,#0x394
  0017fabe: blx 0x0006f2b0
  0017fac2: ldr r0,[0x0017fb30]
  0017fac4: add r0,pc
  0017fac6: ldr r4,[r0,#0x0]
  0017fac8: add r0,sp,#0x394
  0017faca: blx r4
  0017facc: add r0,sp,#0x220
  0017face: blx r4
  0017fad0: add r0,sp,#0x22c
  0017fad2: blx r4
  0017fad4: add r0,sp,#0x388
  0017fad6: blx r4
  0017fad8: add r0,sp,#0x238
  0017fada: b 0x0017fda2
  0017fb34: ldr r0,[0x0017febc]
  0017fb36: add r0,pc
  0017fb38: ldr r0,[r0,#0x0]
  0017fb3a: ldr r0,[r0,#0x0]
  0017fb3c: movw r1,#0x369
  0017fb40: blx 0x00072f70
  0017fb44: mov r1,r0
  0017fb46: add r0,sp,#0x3a0
  0017fb48: blx 0x0006f2b0
  0017fb4c: ldr r0,[sp,#0x18]
  0017fb4e: ldr r5,[r0,#0x0]
  0017fb50: add r0,sp,#0x214
  0017fb52: add r1,sp,#0x3a0
  0017fb54: movs r2,#0x0
  0017fb56: blx 0x0006f028
  0017fb5a: ldr r0,[0x0017fec0]
  0017fb5c: add r0,pc
  0017fb5e: ldr r0,[r0,#0x0]
  0017fb60: ldr r6,[r0,#0x0]
  0017fb62: mov r0,r9
  0017fb64: blx 0x00074bc0
  0017fb68: vmov s0,r0
  0017fb6c: vcvt.f32.s32 s0,s0
  0017fb70: vmul.f32 s0,s16,s0
  0017fb74: vcvt.s32.f32 s0,s0
  0017fb78: vmov r2,s0
  0017fb7c: add r0,sp,#0x388
  0017fb7e: mov r1,r6
  0017fb80: blx 0x00074e54
  0017fb84: add r0,sp,#0x208
  0017fb86: add r1,sp,#0x388
  0017fb88: movs r2,#0x0
  0017fb8a: blx 0x0006f028
  0017fb8e: ldr r1,[0x0017fec4]
  0017fb90: add r1,pc
  0017fb92: add r0,sp,#0x1fc
  0017fb94: movs r2,#0x0
  0017fb96: blx 0x0006ee18
  0017fb9a: str r0,[sp,#0x0]
  0017fb9c: add r0,sp,#0x394
  0017fb9e: add r2,sp,#0x214
  0017fba0: add r3,sp,#0x208
  0017fba2: mov r1,r5
  0017fba4: blx 0x000739b4
  0017fba8: add r0,sp,#0x3a0
  0017fbaa: add r1,sp,#0x394
  0017fbac: blx 0x0006f2b0
  0017fbb0: ldr r0,[0x0017fec8]
  0017fbb2: add r0,pc
  0017fbb4: ldr r4,[r0,#0x0]
  0017fbb6: add r0,sp,#0x394
  0017fbb8: blx r4
  0017fbba: add r0,sp,#0x1fc
  0017fbbc: blx r4
  0017fbbe: add r0,sp,#0x208
  0017fbc0: blx r4
  0017fbc2: add r0,sp,#0x388
  0017fbc4: blx r4
  0017fbc6: add r0,sp,#0x214
  0017fbc8: b 0x0017fda2
  0017fbca: ldr r0,[0x0017fecc]
  0017fbcc: add r0,pc
  0017fbce: ldr r0,[r0,#0x0]
  0017fbd0: ldr r0,[r0,#0x0]
  0017fbd2: movw r1,#0x151
  0017fbd6: blx 0x00072f70
  0017fbda: mov r1,r0
  0017fbdc: add r0,sp,#0x3a0
  0017fbde: blx 0x0006f2b0
  0017fbe2: ldr r5,[r5,#0x0]
  0017fbe4: add r0,sp,#0x2ec
  0017fbe6: add r1,sp,#0x3a0
  0017fbe8: movs r2,#0x0
  0017fbea: blx 0x0006f028
  0017fbee: add r0,sp,#0x388
  0017fbf0: mov r1,r6
  0017fbf2: blx 0x0006f658
  0017fbf6: add r0,sp,#0x2e0
  0017fbf8: add r1,sp,#0x388
  0017fbfa: movs r2,#0x0
  0017fbfc: blx 0x0006f028
  0017fc00: ldr r1,[0x0017fed0]
  0017fc02: add r1,pc
  0017fc04: add r0,sp,#0x2d4
  0017fc06: movs r2,#0x0
  0017fc08: blx 0x0006ee18
  0017fc0c: str r0,[sp,#0x0]
  0017fc0e: add r0,sp,#0x394
  0017fc10: add r2,sp,#0x2ec
  0017fc12: add r3,sp,#0x2e0
  0017fc14: mov r1,r5
  0017fc16: blx 0x000739b4
  0017fc1a: add r0,sp,#0x3a0
  0017fc1c: add r1,sp,#0x394
  0017fc1e: blx 0x0006f2b0
  0017fc22: ldr r0,[0x0017fed4]
  0017fc24: add r0,pc
  0017fc26: ldr r5,[r0,#0x0]
  0017fc28: add r0,sp,#0x394
  0017fc2a: blx r5
  0017fc2c: add r0,sp,#0x2d4
  0017fc2e: blx r5
  0017fc30: add r0,sp,#0x2e0
  0017fc32: blx r5
  0017fc34: add r0,sp,#0x388
  0017fc36: blx r5
  0017fc38: add r0,sp,#0x2ec
  0017fc3a: blx r5
  0017fc3c: ldr.w r0,[r8,#0x8]
  0017fc40: add r1,sp,#0x3a0
  0017fc42: movs r2,#0x0
  0017fc44: blx 0x000746f8
  0017fc48: mov r10,r4
  0017fc4a: b 0x0017fdb0
  0017fc4c: ldr r0,[sp,#0x10]
  0017fc4e: ldr r0,[r0,#0x0]
  0017fc50: blx 0x00073444
  0017fc54: blx 0x00072874
  0017fc58: cmp r0,#0xb
  0017fc5a: bne.w 0x0017fdfc
  0017fc5e: ldr r0,[sp,#0x10]
  0017fc60: ldr r0,[r0,#0x0]
  0017fc62: movs r1,#0x0
  0017fc64: blx 0x00075148
  0017fc68: b 0x0017fdfc
  0017fc6a: mov.w r0,#0x1e8
  0017fc6e: blx 0x0006eb24
  0017fc72: mov r5,r0
  0017fc74: ldrd r0,r1,[r8,#0x20]
  0017fc78: ldr r1,[r1,#0x4]
  0017fc7a: ldr.w r0,[r1,r0,lsl #0x2]
  0017fc7e: blx 0x00071bb4
  0017fc82: mov r2,r0
  0017fc84: mov.w r0,#0xffffffff
  0017fc88: movs r1,#0x1
  0017fc8a: str r0,[sp,#0x0]
  0017fc8c: mov r0,r5
  0017fc8e: movs r3,#0x0
  0017fc90: blx 0x00075424
  0017fc94: ldr r0,[r4,#0x0]
  0017fc96: movs r1,#0x5
  0017fc98: blx 0x00075bd4
  0017fc9c: str r5,[r0,#0x10]
  0017fc9e: movs r1,#0x5
  0017fca0: ldr r0,[r4,#0x0]
  0017fca2: blx 0x00075bd4
  0017fca6: ldr r0,[r0,#0x10]
  0017fca8: str.w r0,[r8,#0x4]
  0017fcac: movs r0,#0x1
  0017fcae: strb.w r0,[r8,#0x34]
  0017fcb2: b 0x0017fdbc
  0017fcb4: ldr r0,[0x0017fed8]
  0017fcb6: add r0,pc
  0017fcb8: ldr r0,[r0,#0x0]
  0017fcba: ldr r5,[r0,#0x0]
  0017fcbc: ldr r0,[r6,#0x0]
  0017fcbe: blx 0x00075580
  0017fcc2: movw r1,#0x362
  0017fcc6: cmp r0,#0x0
  0017fcc8: it ne
  0017fcca: movw.ne r1,#0x363
  0017fcce: mov r0,r5
  0017fcd0: blx 0x00072f70
  0017fcd4: mov r1,r0
  0017fcd6: add r0,sp,#0x3a0
  0017fcd8: blx 0x0006f2b0
  0017fcdc: ldr r0,[sp,#0x18]
  0017fcde: ldr r4,[r0,#0x0]
  0017fce0: add r0,sp,#0xd0
  0017fce2: add r1,sp,#0x3a0
  0017fce4: movs r2,#0x0
  0017fce6: blx 0x0006f028
  0017fcea: mov r0,r9
  0017fcec: blx 0x00074bd8
  0017fcf0: adds r1,r0,#0x1
  0017fcf2: add r0,sp,#0x388
  0017fcf4: blx 0x0006f658
  0017fcf8: add r0,sp,#0xc4
  0017fcfa: add r1,sp,#0x388
  0017fcfc: movs r2,#0x0
  0017fcfe: blx 0x0006f028
  0017fd02: ldr r1,[0x0017fedc]
  0017fd04: add r1,pc
  0017fd06: add r0,sp,#0xb8
  0017fd08: movs r2,#0x0
  0017fd0a: blx 0x0006ee18
  0017fd0e: str r0,[sp,#0x0]
  0017fd10: add r0,sp,#0x394
  0017fd12: add r2,sp,#0xd0
  0017fd14: add r3,sp,#0xc4
  0017fd16: mov r1,r4
  0017fd18: blx 0x000739b4
  0017fd1c: add r0,sp,#0x3a0
  0017fd1e: add r1,sp,#0x394
  0017fd20: blx 0x0006f2b0
  0017fd24: ldr r0,[0x0017fee0]
  0017fd26: add r0,pc
  0017fd28: ldr r4,[r0,#0x0]
  0017fd2a: add r0,sp,#0x394
  0017fd2c: blx r4
  0017fd2e: add r0,sp,#0xb8
  0017fd30: blx r4
  0017fd32: add r0,sp,#0xc4
  0017fd34: blx r4
  0017fd36: add r0,sp,#0x388
  0017fd38: blx r4
  0017fd3a: add r0,sp,#0xd0
  0017fd3c: blx r4
  0017fd3e: ldr r0,[sp,#0x18]
  0017fd40: ldr r6,[r0,#0x0]
  0017fd42: add r0,sp,#0xac
  0017fd44: add r1,sp,#0x3a0
  0017fd46: movs r2,#0x0
  0017fd48: blx 0x0006f028
  0017fd4c: ldr r0,[0x0017fee4]
  0017fd4e: add r0,pc
  0017fd50: ldr r0,[r0,#0x0]
  0017fd52: ldr r5,[r0,#0x0]
  0017fd54: mov r0,r9
  0017fd56: blx 0x00071bfc
  0017fd5a: mov r2,r0
  0017fd5c: add r0,sp,#0x388
  0017fd5e: mov r1,r5
  0017fd60: blx 0x00074e54
  0017fd64: add r0,sp,#0xa0
  0017fd66: add r1,sp,#0x388
  0017fd68: movs r2,#0x0
  0017fd6a: blx 0x0006f028
  0017fd6e: ldr r1,[0x0017fee8]
  0017fd70: add r1,pc
  0017fd72: add r0,sp,#0x94
  0017fd74: movs r2,#0x0
  0017fd76: blx 0x0006ee18
  0017fd7a: str r0,[sp,#0x0]
  0017fd7c: add r0,sp,#0x394
  0017fd7e: add r2,sp,#0xac
  0017fd80: add r3,sp,#0xa0
  0017fd82: mov r1,r6
  0017fd84: blx 0x000739b4
  0017fd88: add r0,sp,#0x3a0
  0017fd8a: add r1,sp,#0x394
  0017fd8c: blx 0x0006f2b0
  0017fd90: add r0,sp,#0x394
  0017fd92: blx r4
  0017fd94: add r0,sp,#0x94
  0017fd96: blx r4
  0017fd98: add r0,sp,#0xa0
  0017fd9a: blx r4
  0017fd9c: add r0,sp,#0x388
  0017fd9e: blx r4
  0017fda0: add r0,sp,#0xac
  0017fda2: blx r4
  0017fda4: ldr.w r0,[r8,#0x8]
  0017fda8: add r1,sp,#0x3a0
  0017fdaa: movs r2,#0x1
  0017fdac: blx 0x000746f8
  0017fdb0: movs r0,#0x1
  0017fdb2: strb.w r0,[r10,#0x0]
  0017fdb6: add r0,sp,#0x3a0
  0017fdb8: blx 0x0006ee30
  0017fdbc: ldrd r0,r1,[r8,#0x20]
  0017fdc0: ldr r1,[r1,#0x4]
  0017fdc2: ldr.w r0,[r1,r0,lsl #0x2]
  0017fdc6: blx 0x00071b9c
  0017fdca: cmp r0,#0x0
  0017fdcc: beq.w 0x0017e594
  0017fdd0: ldrd r0,r1,[r8,#0x20]
  0017fdd4: ldr r1,[r1,#0x4]
  0017fdd6: ldr.w r0,[r1,r0,lsl #0x2]
  0017fdda: movs r1,#0x1
  0017fddc: blx 0x000749bc
  0017fde0: b.w 0x0017e594
  0017fde4: ldr r0,[sp,#0x10]
  0017fde6: ldr r0,[r0,#0x0]
  0017fde8: blx 0x00071a58
  0017fdec: ldr r1,[r6,#0x4]
  0017fdee: ldr.w r1,[r1,r4,lsl #0x2]
  0017fdf2: blx 0x000738e8
  0017fdf6: movs r0,#0x1
  0017fdf8: strb.w r0,[r8,#0x35]
  0017fdfc: ldr r0,[sp,#0x10]
  0017fdfe: ldr r0,[r0,#0x0]
  0017fe00: blx 0x00073444
  0017fe04: blx 0x00073438
  0017fe08: blx 0x00071b9c
  0017fe0c: cbnz r0,0x0017fe20
  0017fe0e: ldr r0,[sp,#0x10]
  0017fe10: ldr r0,[r0,#0x0]
  0017fe12: blx 0x00073444
  0017fe16: blx 0x00073438
  0017fe1a: movs r1,#0x0
  0017fe1c: blx 0x00071b18
  0017fe20: ldr r0,[0x0017feec]
  0017fe22: add r0,pc
  0017fe24: ldr r0,[r0,#0x0]
  0017fe26: ldr r1,[r0,#0x0]
  0017fe28: ldr r0,[sp,#0x10]
  0017fe2a: ldr r0,[r0,#0x0]
  0017fe2c: blx 0x00073f90
  0017fe30: mov r0,r9
  0017fe32: blx 0x00071bb4
  0017fe36: blx 0x00072874
  0017fe3a: cbz r0,0x0017fe62
  0017fe3c: mov r0,r9
  0017fe3e: blx 0x00071bb4
  0017fe42: blx 0x00072874
  0017fe46: cmp r0,#0xb
  0017fe48: bne 0x0017fe98
  0017fe4a: ldr r0,[sp,#0x10]
  0017fe4c: ldr r6,[r0,#0x0]
  0017fe4e: mov r0,r9
  0017fe50: blx 0x00071bb4
  0017fe54: blx 0x00072bb0
  0017fe58: mov r1,r0
  0017fe5a: mov r0,r6
  0017fe5c: blx 0x00075148
  0017fe60: b 0x0017fe98
  0017fe62: ldr r0,[0x0017fef0]
  0017fe64: add r0,pc
  0017fe66: ldr r0,[r0,#0x0]
  0017fe68: ldr r0,[r0,#0x0]
  0017fe6a: ldr r0,[r0,#0x4]
  0017fe6c: ldr.w r6,[r0,#0x1d0]
  0017fe70: mov r0,r9
  0017fe72: blx 0x00071bb4
  0017fe76: blx 0x00072bb0
  0017fe7a: mov r1,r0
  0017fe7c: mov r0,r6
  0017fe7e: blx 0x000718c0
  0017fe82: mov r6,r0
  0017fe84: movs r1,#0x1
  0017fe86: blx 0x00073330
  0017fe8a: ldr r0,[sp,#0x10]
  0017fe8c: ldr r0,[r0,#0x0]
  0017fe8e: blx 0x00071a58
  0017fe92: mov r1,r6
  0017fe94: blx 0x00072cdc
  0017fe98: mov r0,r9
  0017fe9a: blx 0x00071bb4
  0017fe9e: blx 0x00072874
  0017fea2: cmp r0,#0xc
  0017fea4: bne 0x0017fef4
  0017fea6: ldr r0,[sp,#0x18]
  0017fea8: ldr r0,[r0,#0x0]
  0017feaa: mov.w r1,#0x358
  0017feae: blx 0x00072f70
  0017feb2: mov r1,r0
  0017feb4: add r0,sp,#0x3a0
  0017feb6: blx 0x0006f2b0
  0017feba: b 0x0017ff36
  0017fef4: ldr r1,[0x00180248]
  0017fef6: add r1,pc
  0017fef8: add r0,sp,#0x388
  0017fefa: movs r2,#0x0
  0017fefc: blx 0x0006ee18
  0017ff00: ldr r0,[sp,#0x18]
  0017ff02: ldr r6,[r0,#0x0]
  0017ff04: ldr r0,[sp,#0x14]
  0017ff06: ldr r0,[r0,#0x0]
  0017ff08: movs r1,#0x3
  0017ff0a: blx 0x00071848
  0017ff0e: addw r1,r0,#0x355
  0017ff12: mov r0,r6
  0017ff14: blx 0x00072f70
  0017ff18: mov r2,r0
  0017ff1a: add r0,sp,#0x394
  0017ff1c: add r1,sp,#0x388
  0017ff1e: blx 0x0006ef98
  0017ff22: add r0,sp,#0x3a0
  0017ff24: add r1,sp,#0x394
  0017ff26: blx 0x0006ef5c
  0017ff2a: add r0,sp,#0x394
  0017ff2c: blx 0x0006ee30
  0017ff30: add r0,sp,#0x388
  0017ff32: blx 0x0006ee30
  0017ff36: mov r0,r9
  0017ff38: blx 0x00071bb4
  0017ff3c: blx 0x00077d70
  0017ff40: ldr r2,[sp,#0x10]
  0017ff42: cbz r0,0x0017ff68
  0017ff44: ldrb.w r0,[r9,#0x24]
  0017ff48: cbnz r0,0x0017ff56
  0017ff4a: ldr r0,[r2,#0x0]
  0017ff4c: ldr.w r1,[r0,#0xe8]
  0017ff50: adds r1,#0x1
  0017ff52: str.w r1,[r0,#0xe8]
  0017ff56: ldrb.w r0,[r9,#0x25]
  0017ff5a: cbnz r0,0x0017ff68
  0017ff5c: ldr r0,[r2,#0x0]
  0017ff5e: ldr.w r1,[r0,#0xec]
  0017ff62: adds r1,#0x1
  0017ff64: str.w r1,[r0,#0xec]
  0017ff68: ldr r6,[r2,#0x0]
  0017ff6a: mov r0,r9
  0017ff6c: blx 0x00071bb4
  0017ff70: mov r1,r0
  0017ff72: mov r0,r6
  0017ff74: blx 0x00073f90
  0017ff78: ldr r0,[sp,#0x10]
  0017ff7a: ldr r0,[r0,#0x0]
  0017ff7c: blx 0x00073444
  0017ff80: mov r1,r9
  0017ff82: blx 0x00074140
  0017ff86: mov r0,r8
  0017ff88: blx 0x000753dc
  0017ff8c: movs r0,#0x1
  0017ff8e: strb.w r0,[r8,#0x35]
  0017ff92: mov r0,r9
  0017ff94: blx 0x00071ae8
  0017ff98: cmp r0,#0x8
  0017ff9a: beq 0x0017ffa4
  0017ff9c: mov r0,r9
  0017ff9e: movs r1,#0x1
  0017ffa0: blx 0x00071b18
  0017ffa4: ldr.w r6,[r8,#0x60]
  0017ffa8: add r0,sp,#0x34c
  0017ffaa: mov r1,r9
  0017ffac: blx 0x00071c2c
  0017ffb0: add r0,sp,#0x340
  0017ffb2: add r1,sp,#0x3a0
  0017ffb4: movs r2,#0x0
  0017ffb6: blx 0x0006f028
  0017ffba: add r1,sp,#0x34c
  0017ffbc: add r2,sp,#0x340
  0017ffbe: mov r0,r6
  0017ffc0: blx 0x00075b2c
  0017ffc4: ldr r0,[0x0018024c]
  0017ffc6: add r0,pc
  0017ffc8: ldr r4,[r0,#0x0]
  0017ffca: add r0,sp,#0x340
  0017ffcc: blx r4
  0017ffce: add r0,sp,#0x34c
  0017ffd0: blx r4
  0017ffd2: movs r5,#0x0
  0017ffd4: movs r0,#0x3
  0017ffd6: str.w r5,[r8,#0x2c]
  0017ffda: str.w r0,[r8,#0x14]
  0017ffde: add r0,sp,#0x3a0
  0017ffe0: blx r4
  0017ffe2: strb.w r5,[r8,#0x19]
  0017ffe6: b.w 0x0017e594
```
