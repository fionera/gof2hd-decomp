# Globals::getAgentMissionText
elf 0xe5ed4 body 7024
Sig: undefined __stdcall getAgentMissionText(Agent * param_1)

## decompile
```c

/* Globals::getAgentMissionText(Agent*) */

void Globals::getAgentMissionText(Agent *param_1)

{
  uint uVar1;
  byte bVar2;
  int iVar3;
  undefined4 uVar4;
  Ship *pSVar5;
  int iVar6;
  Station *this;
  Array *pAVar7;
  Mission *pMVar8;
  Standing *this_00;
  float fVar9;
  String *pSVar10;
  String *pSVar11;
  int extraout_r1;
  int extraout_r1_00;
  Agent *in_r2;
  code *pcVar12;
  Galaxy *this_01;
  undefined4 *puVar13;
  int *piVar14;
  undefined4 uVar15;
  int iVar16;
  int *piVar17;
  int *piVar18;
  undefined4 uVar19;
  int iVar20;
  uint in_fpscr;
  uint uVar21;
  float fVar22;
  String aSStack_550 [12];
  String aSStack_544 [12];
  String aSStack_538 [12];
  String aSStack_52c [12];
  String aSStack_520 [12];
  String aSStack_514 [12];
  String aSStack_508 [12];
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
  String aSStack_3ac [12];
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
  undefined1 auStack_2bc [12];
  String aSStack_2b0 [12];
  String aSStack_2a4 [12];
  String aSStack_298 [12];
  String aSStack_28c [12];
  String aSStack_280 [12];
  String aSStack_274 [12];
  String aSStack_268 [12];
  String aSStack_25c [12];
  String aSStack_250 [12];
  undefined1 auStack_244 [12];
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
  String aSStack_184 [12];
  String aSStack_178 [12];
  String aSStack_16c [12];
  String aSStack_160 [12];
  String aSStack_154 [12];
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
  String aSStack_4c [8];
  int iStack_44;
  String aSStack_40 [12];
  int iStack_34;
  
  piVar17 = *(int **)(DAT_000f6154 + 0xf5ef0);
  iStack_34 = *piVar17;
  if (in_r2 == (Agent *)0x0) {
    AbyssEngine::String::String((String *)param_1,(char *)(DAT_000f616c + 0xf6052),false);
    goto LAB_000f7c1e;
  }
  AbyssEngine::String::String(aSStack_40);
  iVar3 = Agent::isGenericAgent(in_r2);
  if (iVar3 == 0) {
    AbyssEngine::String::String(aSStack_4c);
    iVar3 = Agent::getEvent(in_r2);
    if (iVar3 < 1) {
      iVar3 = Agent::hasAcceptedOffer(in_r2);
      if (iVar3 == 0) {
        piVar18 = *(int **)(DAT_000f6178 + 0xf60b4);
        *(int *)(*piVar18 + 0xd0) = *(int *)(*piVar18 + 0xd0) + 1;
        iVar3 = Agent::getOffer(in_r2);
        if (iVar3 == 8) {
          pSVar5 = (Ship *)Status::getShip();
          iVar3 = Ship::getPrice(pSVar5);
          iVar6 = Agent::getModPricePercentage(in_r2);
          iVar3 = __aeabi_idiv(iVar3 * iVar6,100);
          Agent::setSellItemPrice(in_r2,iVar3);
          pSVar5 = (Ship *)Status::getShip();
          iVar3 = Agent::getSellModIndex(in_r2);
          iVar3 = Ship::hasModInstalled(pSVar5,iVar3);
          if (iVar3 != 0) {
            pSVar10 = (String *)GameText::getText(**(int **)(DAT_000f617c + 0xf6118));
            AbyssEngine::String::operator=(aSStack_40,pSVar10);
            *(int *)(*piVar18 + 0xd0) = *(int *)(*piVar18 + 0xd0) + -1;
            AbyssEngine::String::String((String *)param_1,aSStack_40,false);
            AbyssEngine::String::~String(aSStack_4c);
            goto LAB_000f7c16;
          }
          iVar3 = *(int *)(in_r2 + 0x28);
          if (iVar3 == -1) {
            iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f7028 + 0xf6f1e));
            iVar3 = iVar3 + 0x36a;
          }
          iVar6 = DAT_000f702c;
          *(int *)(in_r2 + 0x28) = iVar3;
          piVar14 = *(int **)(iVar6 + 0xf6f34);
          pSVar10 = (String *)GameText::getText(*piVar14);
          AbyssEngine::String::operator=(aSStack_40,pSVar10);
          AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f7030 + 0xf6f4a),false);
          iVar3 = *piVar14;
          Agent::getIndex(in_r2);
          GameText::getText(iVar3);
          AbyssEngine::operator+(aSStack_70,aSStack_238);
          AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
          AbyssEngine::String::~String(aSStack_70);
          AbyssEngine::String::~String(aSStack_238);
          AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f7034 + 0xf6f8e),false);
          GameText::getText(*piVar14);
          AbyssEngine::operator+(aSStack_70,aSStack_238);
          AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
          AbyssEngine::String::~String(aSStack_70);
          AbyssEngine::String::~String(aSStack_238);
          iVar3 = *piVar18;
          AbyssEngine::String::String(aSStack_3d0,aSStack_4c,false);
          if (*(char *)(*(int *)(DAT_000f703c + 0xf6fde) + 0x38) == '\0') {
            uVar4 = Agent::getSellItemPrice(in_r2);
            VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
          }
          else {
            uVar4 = Agent::getSellItemPrice(in_r2);
            VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
          }
          Layout::formatCredits((int)aSStack_238);
          AbyssEngine::String::String(aSStack_3dc,aSStack_238,false);
          uVar4 = AbyssEngine::String::String(aSStack_3e8,(char *)(DAT_000f7994 + 0xf78c4),false);
          Status::replaceHash(aSStack_70,iVar3,aSStack_3d0,aSStack_3dc,uVar4);
          AbyssEngine::String::operator=(aSStack_40,aSStack_70);
          pcVar12 = *(code **)(DAT_000f7998 + 0xf78ec);
          (*pcVar12)(aSStack_70);
          (*pcVar12)(aSStack_3e8);
          (*pcVar12)(aSStack_3dc);
          (*pcVar12)(aSStack_238);
          (*pcVar12)(aSStack_3d0);
        }
        else {
          iVar3 = Agent::getOffer(in_r2);
          if (iVar3 == 9) {
            iVar3 = *(int *)(in_r2 + 0x28);
            if (iVar3 == -1) {
              iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f6cf0 + 0xf6ac6));
              iVar3 = iVar3 + 0x36a;
            }
            iVar6 = DAT_000f6cf4;
            *(int *)(in_r2 + 0x28) = iVar3;
            piVar14 = *(int **)(iVar6 + 0xf6adc);
            pSVar10 = (String *)GameText::getText(*piVar14);
            AbyssEngine::String::operator=(aSStack_40,pSVar10);
            AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f6cf8 + 0xf6af6),false);
            iVar3 = *piVar14;
            Agent::getIndex(in_r2);
            GameText::getText(iVar3);
            AbyssEngine::operator+(aSStack_70,aSStack_238);
            AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
            AbyssEngine::String::~String(aSStack_70);
            AbyssEngine::String::~String(aSStack_238);
            iVar3 = *(int *)(in_r2 + 0x2c);
            if (iVar3 == -1) {
              iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f6cfc + 0xf6b44));
              iVar3 = iVar3 + 0x305;
            }
            iVar6 = DAT_000f6d00;
            *(int *)(in_r2 + 0x2c) = iVar3;
            AbyssEngine::String::String(aSStack_238,(char *)(iVar6 + 0xf6b5a),false);
            GameText::getText(*piVar14);
            AbyssEngine::operator+(aSStack_70,aSStack_238);
            AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
            AbyssEngine::String::~String(aSStack_70);
            AbyssEngine::String::~String(aSStack_238);
            iVar6 = *piVar18;
            AbyssEngine::String::String(aSStack_3f4,aSStack_40,false);
            iVar3 = Agent::getSellItemQuantity(in_r2);
            AbyssEngine::String::String(aSStack_238,iVar3);
            AbyssEngine::String::String(aSStack_400,aSStack_238,false);
            uVar4 = AbyssEngine::String::String(aSStack_40c,(char *)(DAT_000f6d04 + 0xf6bbe),false);
            Status::replaceHash(aSStack_70,iVar6,aSStack_3f4,aSStack_400,uVar4);
            AbyssEngine::String::operator=(aSStack_40,aSStack_70);
            pcVar12 = *(code **)(DAT_000f6d08 + 0xf6be6);
            (*pcVar12)(aSStack_70);
            (*pcVar12)(aSStack_40c);
            (*pcVar12)(aSStack_400);
            (*pcVar12)(aSStack_238);
            (*pcVar12)(aSStack_3f4);
            iVar6 = *piVar18;
            AbyssEngine::String::String(aSStack_418,aSStack_40,false);
            iVar3 = *piVar14;
            Agent::getSellItemIndex(in_r2);
            pSVar10 = (String *)GameText::getText(iVar3);
            AbyssEngine::String::String(aSStack_424,pSVar10,false);
            uVar4 = AbyssEngine::String::String(aSStack_430,(char *)(DAT_000f6d0c + 0xf6c30),false);
            Status::replaceHash(aSStack_70,iVar6,aSStack_418,aSStack_424,uVar4);
            AbyssEngine::String::operator=(aSStack_40,aSStack_70);
            (*pcVar12)(aSStack_70);
            (*pcVar12)(aSStack_430);
            (*pcVar12)(aSStack_424);
            (*pcVar12)(aSStack_418);
            iVar3 = *piVar18;
            AbyssEngine::String::String(aSStack_43c,aSStack_40,false);
            if (*(char *)(*(int *)(DAT_000f6d14 + 0xf6c82) + 0x38) == '\0') {
              uVar4 = Agent::getSellItemPrice(in_r2);
              VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
            }
            else {
              uVar4 = Agent::getSellItemPrice(in_r2);
              VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
            }
            Layout::formatCredits((int)aSStack_238);
            AbyssEngine::String::String(aSStack_448,aSStack_238,false);
            uVar4 = AbyssEngine::String::String(aSStack_454,(char *)(DAT_000f799c + 0xf7936),false);
            Status::replaceHash(aSStack_70,iVar3,aSStack_43c,aSStack_448,uVar4);
            AbyssEngine::String::operator=(aSStack_40,aSStack_70);
            (*pcVar12)(aSStack_70);
            (*pcVar12)(aSStack_454);
            (*pcVar12)(aSStack_448);
            (*pcVar12)(aSStack_238);
            pSVar11 = aSStack_43c;
          }
          else {
            iVar3 = Agent::getOffer(in_r2);
            if (iVar3 == 10) {
              iVar3 = *(int *)(in_r2 + 0x28);
              if (iVar3 == -1) {
                iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f7000 + 0xf6d3a));
                iVar3 = iVar3 + 0x36a;
              }
              iVar6 = DAT_000f7004;
              *(int *)(in_r2 + 0x28) = iVar3;
              piVar14 = *(int **)(iVar6 + 0xf6d50);
              pSVar10 = (String *)GameText::getText(*piVar14);
              AbyssEngine::String::operator=(aSStack_40,pSVar10);
              AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f7008 + 0xf6d6a),false);
              iVar3 = *piVar14;
              Agent::getIndex(in_r2);
              GameText::getText(iVar3);
              AbyssEngine::operator+(aSStack_70,aSStack_238);
              AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
              AbyssEngine::String::~String(aSStack_70);
              AbyssEngine::String::~String(aSStack_238);
              iVar3 = *(int *)(in_r2 + 0x2c);
              if (iVar3 == -1) {
                iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f700c + 0xf6db8));
                iVar3 = iVar3 + 0x305;
              }
              iVar6 = DAT_000f7010;
              *(int *)(in_r2 + 0x2c) = iVar3;
              AbyssEngine::String::String(aSStack_238,(char *)(iVar6 + 0xf6dce),false);
              GameText::getText(*piVar14);
              AbyssEngine::operator+(aSStack_70,aSStack_238);
              AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
              AbyssEngine::String::~String(aSStack_70);
              AbyssEngine::String::~String(aSStack_238);
              iVar3 = *piVar18;
              AbyssEngine::String::String(aSStack_460,aSStack_40,false);
              AbyssEngine::String::String(aSStack_238,1);
              AbyssEngine::String::String(aSStack_46c,aSStack_238,false);
              uVar4 = AbyssEngine::String::String
                                (aSStack_478,(char *)(DAT_000f7014 + 0xf6e2c),false);
              Status::replaceHash(aSStack_70,iVar3,aSStack_460,aSStack_46c,uVar4);
              AbyssEngine::String::operator=(aSStack_40,aSStack_70);
              pcVar12 = *(code **)(DAT_000f7018 + 0xf6e54);
              (*pcVar12)(aSStack_70);
              (*pcVar12)(aSStack_478);
              (*pcVar12)(aSStack_46c);
              (*pcVar12)(aSStack_238);
              (*pcVar12)(aSStack_460);
              iVar6 = *piVar18;
              AbyssEngine::String::String(aSStack_484,aSStack_40,false);
              iVar3 = *piVar14;
              Agent::getSellItemIndex(in_r2);
              pSVar10 = (String *)GameText::getText(iVar3);
              AbyssEngine::String::String(aSStack_490,pSVar10,false);
              uVar4 = AbyssEngine::String::String
                                (aSStack_49c,(char *)(DAT_000f701c + 0xf6e9e),false);
              Status::replaceHash(aSStack_70,iVar6,aSStack_484,aSStack_490,uVar4);
              AbyssEngine::String::operator=(aSStack_40,aSStack_70);
              (*pcVar12)(aSStack_70);
              (*pcVar12)(aSStack_49c);
              (*pcVar12)(aSStack_490);
              (*pcVar12)(aSStack_484);
              iVar3 = *piVar18;
              AbyssEngine::String::String(aSStack_4a8,aSStack_40,false);
              if (*(char *)(*(int *)(DAT_000f7024 + 0xf6ef0) + 0x38) == '\0') {
                uVar4 = Agent::getSellItemPrice(in_r2);
                VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
              }
              else {
                uVar4 = Agent::getSellItemPrice(in_r2);
                VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
              }
              Layout::formatCredits((int)aSStack_238);
              AbyssEngine::String::String(aSStack_4b4,aSStack_238,false);
              uVar4 = AbyssEngine::String::String
                                (aSStack_4c0,(char *)(DAT_000f7d70 + 0xf7b78),false);
              Status::replaceHash(aSStack_70,iVar3,aSStack_4a8,aSStack_4b4,uVar4);
              AbyssEngine::String::operator=(aSStack_40,aSStack_70);
              (*pcVar12)(aSStack_70);
              (*pcVar12)(aSStack_4c0);
              (*pcVar12)(aSStack_4b4);
              (*pcVar12)(aSStack_238);
              pSVar11 = aSStack_4a8;
            }
            else {
              iVar3 = Agent::getOffer(in_r2);
              iVar6 = *(int *)(in_r2 + 0x28);
              if (iVar3 == 4) {
                if (iVar6 == -1) {
                  iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f7970 + 0xf76fe));
                  iVar6 = iVar6 + 0x36a;
                }
                iVar3 = DAT_000f7974;
                *(int *)(in_r2 + 0x28) = iVar6;
                piVar14 = *(int **)(iVar3 + 0xf7714);
                pSVar10 = (String *)GameText::getText(*piVar14);
                AbyssEngine::String::operator=(aSStack_40,pSVar10);
                AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f7978 + 0xf772a),false);
                iVar3 = *piVar14;
                Agent::getIndex(in_r2);
                GameText::getText(iVar3);
                AbyssEngine::operator+(aSStack_70,aSStack_238);
                AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
                AbyssEngine::String::~String(aSStack_70);
                AbyssEngine::String::~String(aSStack_238);
                AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f797c + 0xf776e),false);
                GameText::getText(*piVar14);
                AbyssEngine::operator+(aSStack_70,aSStack_238);
                AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
                AbyssEngine::String::~String(aSStack_70);
                AbyssEngine::String::~String(aSStack_238);
                this_01 = (Galaxy *)**(undefined4 **)(DAT_000f7980 + 0xf77aa);
                iVar3 = Agent::getSellSystemIndex(in_r2);
                Galaxy::getSystem(this_01,iVar3);
                iVar3 = *piVar18;
                AbyssEngine::String::String(aSStack_4cc,aSStack_4c,false);
                SolarSystem::getName();
                AbyssEngine::String::String(aSStack_4d8,aSStack_238,false);
                uVar4 = AbyssEngine::String::String
                                  (aSStack_4e4,(char *)(DAT_000f7984 + 0xf77e4),false);
                Status::replaceHash(aSStack_70,iVar3,aSStack_4cc,aSStack_4d8,uVar4);
                AbyssEngine::String::operator=(aSStack_40,aSStack_70);
                pcVar12 = *(code **)(DAT_000f7988 + 0xf780c);
                (*pcVar12)(aSStack_70);
                (*pcVar12)(aSStack_4e4);
                (*pcVar12)(aSStack_4d8);
                (*pcVar12)(aSStack_238);
                (*pcVar12)(aSStack_4cc);
                iVar3 = *piVar18;
                AbyssEngine::String::String(aSStack_4f0,aSStack_4c,false);
                Agent::getSellItemPrice(in_r2);
                Layout::formatCredits((int)aSStack_238);
                AbyssEngine::String::String(aSStack_4fc,aSStack_238,false);
                uVar4 = AbyssEngine::String::String
                                  (aSStack_508,(char *)(DAT_000f7990 + 0xf785a),false);
                Status::replaceHash(aSStack_70,iVar3,aSStack_4f0,aSStack_4fc,uVar4);
                AbyssEngine::String::operator=(aSStack_40,aSStack_70);
                (*pcVar12)(aSStack_70);
                (*pcVar12)(aSStack_508);
                (*pcVar12)(aSStack_4fc);
                (*pcVar12)(aSStack_238);
                pSVar11 = aSStack_4f0;
              }
              else {
                if (iVar6 == -1) {
                  iVar6 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f7d4c + 0xf79aa));
                  iVar6 = iVar6 + 0x36a;
                }
                iVar3 = DAT_000f7d50;
                *(int *)(in_r2 + 0x28) = iVar6;
                piVar14 = *(int **)(iVar3 + 0xf79c0);
                pSVar10 = (String *)GameText::getText(*piVar14);
                AbyssEngine::String::operator=(aSStack_40,pSVar10);
                AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f7d54 + 0xf79d6),false);
                iVar3 = *piVar14;
                Agent::getIndex(in_r2);
                GameText::getText(iVar3);
                AbyssEngine::operator+(aSStack_70,aSStack_238);
                AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
                AbyssEngine::String::~String(aSStack_70);
                AbyssEngine::String::~String(aSStack_238);
                AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f7d58 + 0xf7a1a),false);
                GameText::getText(*piVar14);
                AbyssEngine::operator+(aSStack_70,aSStack_238);
                AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
                AbyssEngine::String::~String(aSStack_70);
                AbyssEngine::String::~String(aSStack_238);
                iVar20 = *piVar18;
                AbyssEngine::String::String(aSStack_514,aSStack_4c,false);
                iVar6 = *piVar14;
                iVar16 = **(int **)(DAT_000f7d5c + 0xf7a6c);
                iVar3 = Agent::getSellBlueprintIndex(in_r2);
                Item::getIndex(*(Item **)(*(int *)(iVar16 + 4) + iVar3 * 4));
                pSVar10 = (String *)GameText::getText(iVar6);
                AbyssEngine::String::String(aSStack_520,pSVar10,false);
                uVar4 = AbyssEngine::String::String
                                  (aSStack_52c,(char *)(DAT_000f7d60 + 0xf7a98),false);
                Status::replaceHash(aSStack_70,iVar20,aSStack_514,aSStack_520,uVar4);
                AbyssEngine::String::operator=(aSStack_40,aSStack_70);
                pcVar12 = *(code **)(DAT_000f7d64 + 0xf7ac4);
                (*pcVar12)(aSStack_70);
                (*pcVar12)(aSStack_52c);
                (*pcVar12)(aSStack_520);
                (*pcVar12)(aSStack_514);
                iVar3 = *piVar18;
                AbyssEngine::String::String(aSStack_538,aSStack_4c,false);
                Agent::getSellItemPrice(in_r2);
                Layout::formatCredits((int)aSStack_238);
                AbyssEngine::String::String(aSStack_544,aSStack_238,false);
                uVar4 = AbyssEngine::String::String
                                  (aSStack_550,(char *)(DAT_000f7d6c + 0xf7b0e),false);
                Status::replaceHash(aSStack_70,iVar3,aSStack_538,aSStack_544,uVar4);
                AbyssEngine::String::operator=(aSStack_40,aSStack_70);
                (*pcVar12)(aSStack_70);
                (*pcVar12)(aSStack_550);
                (*pcVar12)(aSStack_544);
                (*pcVar12)(aSStack_238);
                pSVar11 = aSStack_538;
              }
            }
          }
          (*pcVar12)(pSVar11);
        }
        AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f7d74 + 0xf7bb6),false);
        iVar3 = **(int **)(DAT_000f7d78 + 0xf7bc4);
        AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f7d7c + 0xf7bc6));
        GameText::getText(iVar3);
        AbyssEngine::operator+(aSStack_70,aSStack_238);
        AbyssEngine::String::operator+=(aSStack_40,aSStack_70);
        AbyssEngine::String::~String(aSStack_70);
        AbyssEngine::String::~String(aSStack_238);
      }
      else {
        pSVar10 = (String *)GameText::getText(**(int **)(DAT_000f6174 + 0xf6096));
        AbyssEngine::String::operator=(aSStack_40,pSVar10);
      }
    }
    else {
      pSVar10 = (String *)GameText::getText(**(int **)(DAT_000f6170 + 0xf6070));
      AbyssEngine::String::operator=(aSStack_40,pSVar10);
    }
    AbyssEngine::String::~String(aSStack_4c);
  }
  else {
    uVar4 = Agent::getOffer(in_r2);
    switch(uVar4) {
    case 0:
      pMVar8 = (Mission *)Agent::getMission(in_r2);
      if (pMVar8 == (Mission *)0x0) {
        piVar18 = *(int **)(DAT_000f6158 + 0xf5f46);
        *(undefined4 *)(in_r2 + 0x28) = 0x334;
        pSVar10 = (String *)GameText::getText(*piVar18);
        AbyssEngine::String::operator=(aSStack_40,pSVar10);
      }
      else {
        piVar18 = *(int **)(DAT_000f6158 + 0xf5f46);
        iVar3 = *piVar18;
        Mission::getType();
        pSVar10 = (String *)GameText::getText(iVar3);
        AbyssEngine::String::operator+=(aSStack_40,pSVar10);
        iVar3 = Mission::getType();
        if ((iVar3 == 5) || (iVar3 = Mission::getType(), iVar3 == 3)) {
          AbyssEngine::String::String(aSStack_70,(char *)(DAT_000f615c + 0xf5f84),false);
          GameText::getText(*piVar18);
          AbyssEngine::operator+(aSStack_4c,aSStack_70);
          AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
          AbyssEngine::String::~String(aSStack_4c);
          AbyssEngine::String::~String(aSStack_70);
        }
        iVar3 = Mission::getType();
        if (iVar3 == 0xf) {
          uVar15 = **(undefined4 **)(DAT_000f6160 + 0xf5fd4);
          AbyssEngine::String::String(aSStack_160,aSStack_40,false);
          iVar3 = *piVar18;
          Mission::getProductionGoodIndex(pMVar8);
          pSVar10 = (String *)GameText::getText(iVar3);
          AbyssEngine::String::String(aSStack_16c,pSVar10,false);
          uVar4 = AbyssEngine::String::String(aSStack_178,(char *)(DAT_000f6164 + 0xf6008),false);
          Status::replaceHash(aSStack_4c,uVar15,aSStack_160,aSStack_16c,uVar4);
          AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
          pcVar12 = *(code **)(DAT_000f6168 + 0xf6032);
          (*pcVar12)(aSStack_4c);
          (*pcVar12)(aSStack_178);
          (*pcVar12)(aSStack_16c);
          pSVar11 = aSStack_160;
        }
        else {
          uVar15 = **(undefined4 **)(DAT_000f7398 + 0xf7046);
          AbyssEngine::String::String(aSStack_184,aSStack_40,false);
          iVar3 = *piVar18;
          Mission::getProductionGoodIndex(pMVar8);
          pSVar10 = (String *)GameText::getText(iVar3);
          AbyssEngine::String::String(aSStack_190,pSVar10,false);
          uVar4 = AbyssEngine::String::String(aSStack_19c,(char *)(DAT_000f739c + 0xf7078),false);
          Status::replaceHash(aSStack_4c,uVar15,aSStack_184,aSStack_190,uVar4);
          AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
          pcVar12 = *(code **)(DAT_000f73a0 + 0xf70a0);
          (*pcVar12)(aSStack_4c);
          (*pcVar12)(aSStack_19c);
          (*pcVar12)(aSStack_190);
          pSVar11 = aSStack_184;
        }
        (*pcVar12)(pSVar11);
        puVar13 = *(undefined4 **)(DAT_000f73a4 + 0xf70b8);
        uVar15 = *puVar13;
        AbyssEngine::String::String(aSStack_1a8,aSStack_40,false);
        iVar3 = Mission::getProductionGoodAmount();
        AbyssEngine::String::String(aSStack_70,iVar3);
        AbyssEngine::String::String(aSStack_1b4,aSStack_70,false);
        uVar4 = AbyssEngine::String::String(aSStack_1c0,(char *)(DAT_000f73a8 + 0xf70ea),false);
        Status::replaceHash(aSStack_4c,uVar15,aSStack_1a8,aSStack_1b4,uVar4);
        AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
        pcVar12 = *(code **)(DAT_000f73ac + 0xf7112);
        (*pcVar12)(aSStack_4c);
        (*pcVar12)(aSStack_1c0);
        (*pcVar12)(aSStack_1b4);
        (*pcVar12)(aSStack_70);
        (*pcVar12)(aSStack_1a8);
        iVar3 = Mission::getType();
        if (iVar3 == 0xe) {
          uVar15 = *puVar13;
          AbyssEngine::String::String(aSStack_1cc,aSStack_40,false);
          Mission::getTargetSystemName();
          AbyssEngine::String::String(aSStack_1d8,aSStack_70,false);
          uVar4 = AbyssEngine::String::String(aSStack_1e4,(char *)(DAT_000f73b0 + 0xf7162),false);
          Status::replaceHash(aSStack_4c,uVar15,aSStack_1cc,aSStack_1d8,uVar4);
          AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
          (*pcVar12)(aSStack_4c);
          (*pcVar12)(aSStack_1e4);
          (*pcVar12)(aSStack_1d8);
          (*pcVar12)(aSStack_70);
          pSVar11 = aSStack_1cc;
        }
        else {
          uVar15 = *puVar13;
          AbyssEngine::String::String(aSStack_1f0,aSStack_40,false);
          Mission::getTargetStationName();
          AbyssEngine::String::String(aSStack_1fc,aSStack_70,false);
          uVar4 = AbyssEngine::String::String(aSStack_208,(char *)(DAT_000f73b4 + 0xf71c8),false);
          Status::replaceHash(aSStack_4c,uVar15,aSStack_1f0,aSStack_1fc,uVar4);
          AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
          (*pcVar12)(aSStack_4c);
          (*pcVar12)(aSStack_208);
          (*pcVar12)(aSStack_1fc);
          (*pcVar12)(aSStack_70);
          pSVar11 = aSStack_1f0;
        }
        (*pcVar12)(pSVar11);
        uVar15 = *puVar13;
        AbyssEngine::String::String(aSStack_214,aSStack_40,false);
        Mission::getTargetName();
        AbyssEngine::String::String(aSStack_220,aSStack_70,false);
        uVar4 = AbyssEngine::String::String(aSStack_22c,(char *)(DAT_000f73b8 + 0xf722e),false);
        Status::replaceHash(aSStack_4c,uVar15,aSStack_214,aSStack_220,uVar4);
        AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
        (*pcVar12)(aSStack_4c);
        (*pcVar12)(aSStack_22c);
        (*pcVar12)(aSStack_220);
        (*pcVar12)(aSStack_70);
        (*pcVar12)(aSStack_214);
        iVar3 = *(int *)(in_r2 + 0x28);
        if (iVar3 == -1) {
          iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f73bc + 0xf7276));
          iVar3 = iVar3 + 0x2fc;
        }
        iVar6 = DAT_000f73c0;
        *(int *)(in_r2 + 0x28) = iVar3;
        AbyssEngine::String::String(aSStack_70,(char *)(iVar6 + 0xf728c),false);
        GameText::getText(*piVar18);
        AbyssEngine::operator+(aSStack_4c,aSStack_70);
        AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
        AbyssEngine::String::~String(aSStack_4c);
        AbyssEngine::String::~String(aSStack_70);
        this_00 = (Standing *)Status::getStanding();
        iVar3 = Agent::getRace(in_r2);
        fVar9 = (float)Standing::getMissionBonus(this_00,iVar3);
        uVar1 = in_fpscr & 0xfffffff | (uint)(fVar9 < 0.0) << 0x1f | (uint)(fVar9 == 0.0) << 0x1e;
        uVar21 = uVar1 | (uint)NAN(fVar9) << 0x1c;
        bVar2 = (byte)(uVar1 >> 0x18);
        if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(uVar21 >> 0x1c) & 1)) {
          AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f76b0 + 0xf73da),false);
        }
        else {
          AbyssEngine::String::String(aSStack_238,(char *)(DAT_000f73c4 + 0xf72f4),false);
          uVar15 = *puVar13;
          pSVar10 = (String *)GameText::getText(*piVar18);
          AbyssEngine::String::String(aSStack_250,pSVar10,false);
          AbyssEngine::String::String(aSStack_268,(int)(fVar9 * DAT_000f73c8));
          AbyssEngine::String::String(aSStack_25c,aSStack_268,false);
          uVar4 = AbyssEngine::String::String(aSStack_274,(char *)(DAT_000f73cc + 0xf733a),false);
          Status::replaceHash(auStack_244,uVar15,aSStack_250,aSStack_25c,uVar4);
          AbyssEngine::operator+(aSStack_70,aSStack_238);
          AbyssEngine::String::String(aSStack_280,(char *)(DAT_000f73d0 + 0xf7360),false);
          AbyssEngine::operator+(aSStack_4c,aSStack_70);
          (*pcVar12)(aSStack_280);
          (*pcVar12)(aSStack_70);
          (*pcVar12)(auStack_244);
          (*pcVar12)(aSStack_274);
          (*pcVar12)(aSStack_25c);
          (*pcVar12)(aSStack_268);
          (*pcVar12)(aSStack_250);
          (*pcVar12)(aSStack_238);
        }
        Agent::getMission(in_r2);
        uVar4 = Mission::getReward();
        fVar22 = (float)VectorSignedToFloat(uVar4,(byte)(uVar21 >> 0x16) & 3);
        iVar6 = (int)(fVar9 * fVar22);
        __aeabi_idivmod(iVar6,0x32);
        pMVar8 = (Mission *)Agent::getMission(in_r2);
        iVar3 = extraout_r1 + iVar6;
        __aeabi_idivmod(iVar3,0x32);
        if (extraout_r1_00 != 0) {
          iVar3 = iVar6 - extraout_r1;
        }
        Mission::setBonus(pMVar8,iVar3);
        uVar15 = *puVar13;
        AbyssEngine::String::String(aSStack_28c,aSStack_40,false);
        Mission::getReward();
        Mission::getBonus();
        Layout::formatCredits((int)auStack_244);
        AbyssEngine::String::String(aSStack_238,auStack_244,false);
        AbyssEngine::operator+(aSStack_298,aSStack_238);
        uVar4 = AbyssEngine::String::String(aSStack_2a4,(char *)(DAT_000f76b8 + 0xf747e),false);
        Status::replaceHash(aSStack_70,uVar15,aSStack_28c,aSStack_298,uVar4);
        AbyssEngine::String::operator=(aSStack_40,aSStack_70);
        (*pcVar12)(aSStack_70);
        (*pcVar12)(aSStack_2a4);
        (*pcVar12)(aSStack_298);
        (*pcVar12)(aSStack_238);
        (*pcVar12)(auStack_244);
        (*pcVar12)(aSStack_28c);
        iVar3 = Mission::getStatusValue();
        if (iVar3 == -1) {
          pSVar10 = (String *)GameText::getText(*piVar18);
          AbyssEngine::String::operator=(aSStack_40,pSVar10);
          uVar15 = *puVar13;
          AbyssEngine::String::String(aSStack_2b0,aSStack_40,false);
          Mission::getTargetStationName();
          uVar4 = AbyssEngine::String::String(aSStack_2c8,(char *)(DAT_000f76bc + 0xf74f8),false);
          Status::replaceHash(aSStack_70,uVar15,aSStack_2b0,auStack_2bc,uVar4);
          AbyssEngine::String::operator=(aSStack_40,aSStack_70);
          (*pcVar12)(aSStack_70);
          (*pcVar12)(aSStack_2c8);
          (*pcVar12)(auStack_2bc);
          (*pcVar12)(aSStack_2b0);
        }
        AbyssEngine::String::~String(aSStack_4c);
      }
      break;
    case 1:
      iVar3 = *(int *)(in_r2 + 0x28);
      piVar18 = *(int **)(DAT_000f639c + 0xf618a);
      if (iVar3 == -1) {
        iVar3 = 0x338;
      }
      *(int *)(in_r2 + 0x28) = iVar3;
      pSVar10 = (String *)GameText::getText(*piVar18);
      AbyssEngine::String::operator=(aSStack_40,pSVar10);
      piVar14 = *(int **)(DAT_000f63a0 + 0xf61b0);
      iVar6 = *piVar14;
      Agent::getName();
      iVar3 = Agent::getRace(in_r2);
      puVar13 = *(undefined4 **)(DAT_000f63a4 + 0xf61d0);
      this = (Station *)Status::getStation();
      Station::getIndex(this);
      AbyssEngine::AERandom::setSeed(CONCAT44(iVar3 * iStack_44,iVar6));
      AbyssEngine::String::~String(aSStack_4c);
      AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f63a8 + 0xf61fa),false);
      iVar3 = AbyssEngine::String::IndexOf(aSStack_40);
      AbyssEngine::String::~String(aSStack_4c);
      if (-1 < iVar3) {
        uVar15 = *puVar13;
        AbyssEngine::String::String(aSStack_f4,aSStack_40,false);
        getRandomPlanetName();
        AbyssEngine::String::String(aSStack_100,aSStack_70,false);
        uVar4 = AbyssEngine::String::String(aSStack_10c,(char *)(DAT_000f63ac + 0xf624a),false);
        Status::replaceHash(aSStack_4c,uVar15,aSStack_f4,aSStack_100,uVar4);
        AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
        pcVar12 = *(code **)(DAT_000f63b0 + 0xf6278);
        (*pcVar12)(aSStack_4c);
        (*pcVar12)(aSStack_10c);
        (*pcVar12)(aSStack_100);
        (*pcVar12)(aSStack_70);
        (*pcVar12)(aSStack_f4);
      }
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_118,aSStack_40,false);
      Agent::getName();
      AbyssEngine::String::String(aSStack_124,aSStack_70,false);
      uVar4 = AbyssEngine::String::String(aSStack_130,(char *)(DAT_000f63b4 + 0xf62c6),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_118,aSStack_124,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      pcVar12 = *(code **)(DAT_000f63b8 + 0xf62f6);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_130);
      (*pcVar12)(aSStack_124);
      (*pcVar12)(aSStack_70);
      (*pcVar12)(aSStack_118);
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_13c,aSStack_40,false);
      iVar3 = *piVar18;
      AbyssEngine::AERandom::nextInt(*piVar14);
      pSVar10 = (String *)GameText::getText(iVar3);
      AbyssEngine::String::String(aSStack_148,pSVar10,false);
      uVar4 = AbyssEngine::String::String(aSStack_154,(char *)(DAT_000f63bc + 0xf6350),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_13c,aSStack_148,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_154);
      (*pcVar12)(aSStack_148);
      (*pcVar12)(aSStack_13c);
      AbyssEngine::AERandom::reset();
      break;
    case 2:
      iVar3 = *(int *)(in_r2 + 0x28);
      if (iVar3 == -1) {
        iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f6690 + 0xf63ce));
        iVar3 = iVar3 + 0x300;
      }
      iVar6 = DAT_000f6694;
      *(int *)(in_r2 + 0x28) = iVar3;
      piVar18 = *(int **)(iVar6 + 0xf63e4);
      pSVar10 = (String *)GameText::getText(*piVar18);
      AbyssEngine::String::operator+=(aSStack_40,pSVar10);
      iVar3 = *(int *)(in_r2 + 0x2c);
      if (iVar3 == -1) {
        iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f6698 + 0xf6406));
        iVar3 = iVar3 + 0x305;
      }
      iVar6 = DAT_000f669c;
      *(int *)(in_r2 + 0x2c) = iVar3;
      AbyssEngine::String::String(aSStack_70,(char *)(iVar6 + 0xf641c),false);
      GameText::getText(*piVar18);
      AbyssEngine::operator+(aSStack_4c,aSStack_70);
      AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
      AbyssEngine::String::~String(aSStack_4c);
      AbyssEngine::String::~String(aSStack_70);
      puVar13 = *(undefined4 **)(DAT_000f66a0 + 0xf645e);
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_2d4,aSStack_40,false);
      iVar3 = Agent::getSellItemQuantity(in_r2);
      AbyssEngine::String::String(aSStack_70,iVar3);
      AbyssEngine::String::String(aSStack_2e0,aSStack_70,false);
      uVar4 = AbyssEngine::String::String(aSStack_2ec,(char *)(DAT_000f66a4 + 0xf6492),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_2d4,aSStack_2e0,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      pcVar12 = *(code **)(DAT_000f66a8 + 0xf64ba);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_2ec);
      (*pcVar12)(aSStack_2e0);
      (*pcVar12)(aSStack_70);
      (*pcVar12)(aSStack_2d4);
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_2f8,aSStack_40,false);
      iVar3 = *piVar18;
      Agent::getSellItemIndex(in_r2);
      pSVar10 = (String *)GameText::getText(iVar3);
      AbyssEngine::String::String(aSStack_304,pSVar10,false);
      uVar4 = AbyssEngine::String::String(aSStack_310,(char *)(DAT_000f66ac + 0xf6506),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_2f8,aSStack_304,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_310);
      (*pcVar12)(aSStack_304);
      (*pcVar12)(aSStack_2f8);
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_31c,aSStack_40,false);
      Agent::getSellItemPrice(in_r2);
      Layout::formatCredits((int)aSStack_70);
      AbyssEngine::String::String(aSStack_328,aSStack_70,false);
      uVar4 = AbyssEngine::String::String(aSStack_334,(char *)(DAT_000f66b4 + 0xf657e),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_31c,aSStack_328,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_334);
      (*pcVar12)(aSStack_328);
      (*pcVar12)(aSStack_70);
      (*pcVar12)(aSStack_31c);
      iVar3 = Agent::getSellItemQuantity(in_r2);
      if (1 < iVar3) {
        AbyssEngine::String::String(aSStack_70,(char *)(DAT_000f66b8 + 0xf65ca),false);
        GameText::getText(*piVar18);
        AbyssEngine::operator+(aSStack_4c,aSStack_70);
        AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
        AbyssEngine::String::~String(aSStack_4c);
        AbyssEngine::String::~String(aSStack_70);
        uVar19 = *puVar13;
        AbyssEngine::String::String(aSStack_340,aSStack_40,false);
        uVar4 = Agent::getSellItemPrice(in_r2);
        uVar15 = Agent::getSellItemQuantity(in_r2);
        __aeabi_idiv(uVar4,uVar15);
        Layout::formatCredits((int)aSStack_70);
        AbyssEngine::String::String(aSStack_34c,aSStack_70,false);
        uVar4 = AbyssEngine::String::String(aSStack_358,(char *)(DAT_000f66bc + 0xf6650),false);
        Status::replaceHash(aSStack_4c,uVar19,aSStack_340,aSStack_34c,uVar4);
        AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
        (*pcVar12)(aSStack_4c);
        (*pcVar12)(aSStack_358);
        (*pcVar12)(aSStack_34c);
        (*pcVar12)(aSStack_70);
        (*pcVar12)(aSStack_340);
      }
      break;
    case 5:
      iVar3 = Agent::getMission(in_r2);
      if (iVar3 == 0) {
        pAVar7 = (Array *)Galaxy::getSystems((Galaxy *)**(undefined4 **)(DAT_000f6a58 + 0xf66ce));
        pMVar8 = (Mission *)
                 Generator::createMission
                           ((Generator *)**(undefined4 **)(DAT_000f6a5c + 0xf66dc),in_r2,pAVar7);
        Agent::setMission(in_r2,pMVar8);
      }
      iVar3 = *(int *)(in_r2 + 0x28);
      if (iVar3 == -1) {
        iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f6a60 + 0xf66fa));
        iVar3 = iVar3 + 0x309;
      }
      iVar6 = DAT_000f6a64;
      *(int *)(in_r2 + 0x28) = iVar3;
      piVar18 = *(int **)(iVar6 + 0xf6710);
      pSVar10 = (String *)GameText::getText(*piVar18);
      AbyssEngine::String::operator+=(aSStack_40,pSVar10);
      puVar13 = *(undefined4 **)(DAT_000f6a68 + 0xf6726);
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_88,aSStack_40,false);
      Agent::getMission(in_r2);
      iVar3 = Mission::getProductionGoodAmount();
      AbyssEngine::String::String(aSStack_70,iVar3);
      AbyssEngine::String::String(aSStack_94,aSStack_70,false);
      uVar4 = AbyssEngine::String::String(aSStack_a0,(char *)(DAT_000f6a6c + 0xf6762),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_88,aSStack_94,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      pcVar12 = *(code **)(DAT_000f6a70 + 0xf6790);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_a0);
      (*pcVar12)(aSStack_94);
      (*pcVar12)(aSStack_70);
      (*pcVar12)(aSStack_88);
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_ac,aSStack_40,false);
      iVar3 = *piVar18;
      pMVar8 = (Mission *)Agent::getMission(in_r2);
      Mission::getProductionGoodIndex(pMVar8);
      pSVar10 = (String *)GameText::getText(iVar3);
      AbyssEngine::String::String(aSStack_b8,pSVar10,false);
      uVar4 = AbyssEngine::String::String(aSStack_c4,(char *)(DAT_000f6a74 + 0xf67e8),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_ac,aSStack_b8,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_c4);
      (*pcVar12)(aSStack_b8);
      (*pcVar12)(aSStack_ac);
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_d0,aSStack_40,false);
      Agent::getMission(in_r2);
      Mission::getReward();
      Layout::formatCredits((int)aSStack_70);
      AbyssEngine::String::String(aSStack_dc,aSStack_70,false);
      uVar4 = AbyssEngine::String::String(aSStack_e8,(char *)(DAT_000f6a7c + 0xf686c),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_d0,aSStack_dc,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_e8);
      (*pcVar12)(aSStack_dc);
      (*pcVar12)(aSStack_70);
      (*pcVar12)(aSStack_d0);
      break;
    case 6:
      iVar3 = **(int **)(DAT_000f6a80 + 0xf68be);
      Achievements::gotAllMedals((Achievements *)**(undefined4 **)(DAT_000f6a84 + 0xf68c0));
      Agent::getWingmanFriendsCount(in_r2);
      pSVar10 = (String *)GameText::getText(iVar3);
      AbyssEngine::String::operator+=(aSStack_40,pSVar10);
      puVar13 = *(undefined4 **)(DAT_000f6a88 + 0xf68f6);
      uVar15 = *puVar13;
      AbyssEngine::String::String(aSStack_364,aSStack_40,false);
      Agent::getCosts(in_r2);
      Layout::formatCredits((int)aSStack_70);
      AbyssEngine::String::String(aSStack_370,aSStack_70,false);
      uVar4 = AbyssEngine::String::String(aSStack_37c,(char *)(DAT_000f6a90 + 0xf6930),false);
      Status::replaceHash(aSStack_4c,uVar15,aSStack_364,aSStack_370,uVar4);
      AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
      pcVar12 = *(code **)(DAT_000f6a94 + 0xf6958);
      (*pcVar12)(aSStack_4c);
      (*pcVar12)(aSStack_37c);
      (*pcVar12)(aSStack_370);
      (*pcVar12)(aSStack_70);
      (*pcVar12)(aSStack_364);
      iVar3 = Agent::getWingmanFriendsCount(in_r2);
      if (0 < iVar3) {
        uVar15 = *puVar13;
        AbyssEngine::String::String(aSStack_388,aSStack_40,false);
        Agent::getWingmanName((int)aSStack_70);
        AbyssEngine::String::String(aSStack_394,aSStack_70,false);
        uVar4 = AbyssEngine::String::String(aSStack_3a0,(char *)(DAT_000f6a98 + 0xf69a8),false);
        Status::replaceHash(aSStack_4c,uVar15,aSStack_388,aSStack_394,uVar4);
        AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
        (*pcVar12)(aSStack_4c);
        (*pcVar12)(aSStack_3a0);
        (*pcVar12)(aSStack_394);
        (*pcVar12)(aSStack_70);
        pSVar11 = aSStack_388;
LAB_000f75e0:
        (*pcVar12)(pSVar11);
      }
      break;
    case 7:
      Agent::getRace(in_r2);
      puVar13 = *(undefined4 **)(DAT_000f6a9c + 0xf69f2);
      iVar3 = Status::getStanding();
      iVar3 = Standing::getStanding(iVar3);
      iVar6 = Status::getStanding();
      iVar6 = Standing::isEnemy(iVar6);
      if (iVar6 != 0) {
        if (iVar3 < 0) {
          iVar3 = -iVar3;
        }
        VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
        pSVar10 = (String *)GameText::getText(**(int **)(DAT_000f76c0 + 0xf754e));
        AbyssEngine::String::operator=(aSStack_40,pSVar10);
        uVar15 = *puVar13;
        AbyssEngine::String::String(aSStack_58,aSStack_40,false);
        Layout::formatCredits((int)aSStack_70);
        AbyssEngine::String::String(aSStack_64,aSStack_70,false);
        uVar4 = AbyssEngine::String::String(aSStack_7c,(char *)(DAT_000f76c8 + 0xf7596),false);
        Status::replaceHash(aSStack_4c,uVar15,aSStack_58,aSStack_64,uVar4);
        AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
        pcVar12 = *(code **)(DAT_000f76cc + 0xf75c4);
        (*pcVar12)(aSStack_4c);
        (*pcVar12)(aSStack_7c);
        (*pcVar12)(aSStack_64);
        (*pcVar12)(aSStack_70);
        pSVar11 = aSStack_58;
        goto LAB_000f75e0;
      }
      pSVar10 = (String *)GameText::getText(**(int **)(DAT_000f6d18 + 0xf6ca8));
      AbyssEngine::String::operator=(aSStack_40,pSVar10);
      AbyssEngine::String::String((String *)param_1,aSStack_40,false);
      goto LAB_000f7c16;
    }
    iVar3 = Agent::getMission(in_r2);
    if (iVar3 != 0) {
      Agent::getMission(in_r2);
      iVar3 = Mission::getType();
      if (iVar3 == 0xc) {
        pSVar10 = (String *)GameText::getText(**(int **)(DAT_000f76d0 + 0xf7608));
        AbyssEngine::String::operator=(aSStack_40,pSVar10);
        uVar15 = **(undefined4 **)(DAT_000f76d4 + 0xf7622);
        AbyssEngine::String::String(aSStack_3ac,aSStack_40,false);
        Agent::getMission(in_r2);
        Mission::getReward();
        Agent::getMission(in_r2);
        Mission::getBonus();
        Layout::formatCredits((int)aSStack_70);
        AbyssEngine::String::String(aSStack_3b8,aSStack_70,false);
        uVar4 = AbyssEngine::String::String(aSStack_3c4,(char *)(DAT_000f76dc + 0xf766c),false);
        Status::replaceHash(aSStack_4c,uVar15,aSStack_3ac,aSStack_3b8,uVar4);
        AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
        pcVar12 = *(code **)(DAT_000f76e0 + 0xf7694);
        (*pcVar12)(aSStack_4c);
        (*pcVar12)(aSStack_3c4);
        (*pcVar12)(aSStack_3b8);
        (*pcVar12)(aSStack_70);
        (*pcVar12)(aSStack_3ac);
      }
    }
  }
  AbyssEngine::String::String((String *)param_1,aSStack_40,false);
LAB_000f7c16:
  AbyssEngine::String::~String(aSStack_40);
LAB_000f7c1e:
  if (*piVar17 == iStack_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000f5ed4: push {r4,r5,r6,r7,lr}
  000f5ed6: add r7,sp,#0xc
  000f5ed8: push {r7,r8,r9,r10,r11}
  000f5edc: vpush {d8}
  000f5ee0: sub.w sp,sp,#0x538
  000f5ee4: mov r11,r0
  000f5ee6: ldr r0,[0x000f6154]
  000f5ee8: mov r10,r2
  000f5eea: cmp r2,#0x0
  000f5eec: add r0,pc
  000f5eee: ldr.w r8,[r0,#0x0]
  000f5ef2: ldr.w r0,[r8,#0x0]
  000f5ef6: str.w r0,[r7,#-0x2c]
  000f5efa: beq.w 0x000f6048
  000f5efe: sub.w r0,r7,#0x38
  000f5f02: blx 0x0006efbc
  000f5f06: mov r0,r10
  000f5f08: blx 0x00071b9c
  000f5f0c: cmp r0,#0x0
  000f5f0e: beq.w 0x000f6058
  000f5f12: mov r0,r10
  000f5f14: blx 0x00071ae8
  000f5f18: cmp r0,#0x7
  000f5f1a: str.w r11,[sp,#0x14]
  000f5f1e: bhi.w 0x000f75e2
  000f5f22: tbh [pc,r0]
  000f5f36: mov r0,r10
  000f5f38: blx 0x00071bb4
  000f5f3c: mov r5,r0
  000f5f3e: ldr r0,[0x000f6158]
  000f5f40: cmp r5,#0x0
  000f5f42: add r0,pc
  000f5f44: beq.w 0x000f6ccc
  000f5f48: ldr.w r11,[r0,#0x0]
  000f5f4c: ldr.w r4,[r11,#0x0]
  000f5f50: mov r0,r5
  000f5f52: blx 0x00072874
  000f5f56: addw r1,r0,#0x312
  000f5f5a: mov r0,r4
  000f5f5c: blx 0x00072f70
  000f5f60: mov r1,r0
  000f5f62: sub.w r0,r7,#0x38
  000f5f66: blx 0x0006ef5c
  000f5f6a: mov r0,r5
  000f5f6c: blx 0x00072874
  000f5f70: cmp r0,#0x5
  000f5f72: beq 0x000f5f7e
  000f5f74: mov r0,r5
  000f5f76: blx 0x00072874
  000f5f7a: cmp r0,#0x3
  000f5f7c: bne 0x000f5fc2
  000f5f7e: ldr r1,[0x000f615c]
  000f5f80: add r1,pc
  000f5f82: sub.w r0,r7,#0x68
  000f5f86: movs r2,#0x0
  000f5f88: blx 0x0006ee18
  000f5f8c: ldr.w r0,[r11,#0x0]
  000f5f90: movw r1,#0x322
  000f5f94: blx 0x00072f70
  000f5f98: mov r2,r0
  000f5f9a: sub.w r0,r7,#0x44
  000f5f9e: sub.w r1,r7,#0x68
  000f5fa2: blx 0x0006ef98
  000f5fa6: sub.w r0,r7,#0x38
  000f5faa: sub.w r1,r7,#0x44
  000f5fae: blx 0x0006ef5c
  000f5fb2: sub.w r0,r7,#0x44
  000f5fb6: blx 0x0006ee30
  000f5fba: sub.w r0,r7,#0x68
  000f5fbe: blx 0x0006ee30
  000f5fc2: mov r0,r5
  000f5fc4: blx 0x00072874
  000f5fc8: cmp r0,#0xf
  000f5fca: bne.w 0x000f7040
  000f5fce: ldr r0,[0x000f6160]
  000f5fd0: add r0,pc
  000f5fd2: ldr r0,[r0,#0x0]
  000f5fd4: ldr r6,[r0,#0x0]
  000f5fd6: add.w r0,sp,#0x408
  000f5fda: sub.w r1,r7,#0x38
  000f5fde: movs r2,#0x0
  000f5fe0: blx 0x0006f028
  000f5fe4: ldr.w r4,[r11,#0x0]
  000f5fe8: mov r0,r5
  000f5fea: blx 0x000728a4
  000f5fee: addw r1,r0,#0x4fa
  000f5ff2: mov r0,r4
  000f5ff4: blx 0x00072f70
  000f5ff8: mov r1,r0
  000f5ffa: add r0,sp,#0x3fc
  000f5ffc: movs r2,#0x0
  000f5ffe: blx 0x0006f028
  000f6002: ldr r1,[0x000f6164]
  000f6004: add r1,pc
  000f6006: add r0,sp,#0x3f0
  000f6008: movs r2,#0x0
  000f600a: blx 0x0006ee18
  000f600e: str r0,[sp,#0x0]
  000f6010: sub.w r0,r7,#0x44
  000f6014: add.w r2,sp,#0x408
  000f6018: add r3,sp,#0x3fc
  000f601a: mov r1,r6
  000f601c: blx 0x000739b4
  000f6020: sub.w r0,r7,#0x38
  000f6024: sub.w r1,r7,#0x44
  000f6028: blx 0x0006f2b0
  000f602c: ldr r0,[0x000f6168]
  000f602e: add r0,pc
  000f6030: ldr r4,[r0,#0x0]
  000f6032: sub.w r0,r7,#0x44
  000f6036: blx r4
  000f6038: add r0,sp,#0x3f0
  000f603a: blx r4
  000f603c: add r0,sp,#0x3fc
  000f603e: blx r4
  000f6040: add.w r0,sp,#0x408
  000f6044: b.w 0x000f70b0
  000f6048: ldr r1,[0x000f616c]
  000f604a: mov r0,r11
  000f604c: movs r2,#0x0
  000f604e: add r1,pc
  000f6050: blx 0x0006ee18
  000f6054: b.w 0x000f7c1e
  000f6058: sub.w r0,r7,#0x44
  000f605c: blx 0x0006efbc
  000f6060: mov r0,r10
  000f6062: blx 0x00074b9c
  000f6066: cmp r0,#0x1
  000f6068: blt 0x000f6088
  000f606a: ldr r0,[0x000f6170]
  000f606c: add r0,pc
  000f606e: ldr r0,[r0,#0x0]
  000f6070: ldr r0,[r0,#0x0]
  000f6072: movw r1,#0x35a
  000f6076: blx 0x00072f70
  000f607a: mov r1,r0
  000f607c: sub.w r0,r7,#0x38
  000f6080: blx 0x0006f2b0
  000f6084: b.w 0x000f7c02
  000f6088: mov r0,r10
  000f608a: blx 0x00074bf0
  000f608e: cbz r0,0x000f60ae
  000f6090: ldr r0,[0x000f6174]
  000f6092: add r0,pc
  000f6094: ldr r0,[r0,#0x0]
  000f6096: ldr r0,[r0,#0x0]
  000f6098: movw r1,#0x35a
  000f609c: blx 0x00072f70
  000f60a0: mov r1,r0
  000f60a2: sub.w r0,r7,#0x38
  000f60a6: blx 0x0006f2b0
  000f60aa: b.w 0x000f7c02
  000f60ae: ldr r0,[0x000f6178]
  000f60b0: add r0,pc
  000f60b2: ldr.w r9,[r0,#0x0]
  000f60b6: ldr.w r0,[r9,#0x0]
  000f60ba: ldr.w r1,[r0,#0xd0]
  000f60be: adds r1,#0x1
  000f60c0: str.w r1,[r0,#0xd0]
  000f60c4: mov r0,r10
  000f60c6: blx 0x00071ae8
  000f60ca: cmp r0,#0x8
  000f60cc: bne.w 0x000f6aa8
  000f60d0: ldr.w r0,[r9,#0x0]
  000f60d4: blx 0x00071a58
  000f60d8: blx 0x00071b00
  000f60dc: mov r4,r0
  000f60de: mov r0,r10
  000f60e0: blx 0x00075e14
  000f60e4: muls r0,r4
  000f60e6: movs r1,#0x64
  000f60e8: blx 0x0007198c
  000f60ec: mov r1,r0
  000f60ee: mov r0,r10
  000f60f0: blx 0x00075e20
  000f60f4: ldr.w r0,[r9,#0x0]
  000f60f8: blx 0x00071a58
  000f60fc: mov r4,r0
  000f60fe: mov r0,r10
  000f6100: blx 0x00074bfc
  000f6104: mov r1,r0
  000f6106: mov r0,r4
  000f6108: blx 0x00075e2c
  000f610c: cmp r0,#0x0
  000f610e: beq.w 0x000f6f10
  000f6112: ldr r0,[0x000f617c]
  000f6114: add r0,pc
  000f6116: ldr r0,[r0,#0x0]
  000f6118: ldr r0,[r0,#0x0]
  000f611a: movw r1,#0x35a
  000f611e: blx 0x00072f70
  000f6122: mov r1,r0
  000f6124: sub.w r0,r7,#0x38
  000f6128: blx 0x0006f2b0
  000f612c: ldr.w r0,[r9,#0x0]
  000f6130: ldr.w r1,[r0,#0xd0]
  000f6134: subs r1,#0x1
  000f6136: str.w r1,[r0,#0xd0]
  000f613a: sub.w r1,r7,#0x38
  000f613e: mov r0,r11
  000f6140: movs r2,#0x0
  000f6142: blx 0x0006f028
  000f6146: sub.w r0,r7,#0x44
  000f614a: blx 0x0006ee30
  000f614e: b.w 0x000f7c16
  000f6180: ldr r0,[0x000f639c]
  000f6182: ldr.w r1,[r10,#0x28]
  000f6186: add r0,pc
  000f6188: ldr.w r9,[r0,#0x0]
  000f618c: adds r0,r1,#0x1
  000f618e: it eq
  000f6190: mov.eq.w r1,#0x338
  000f6194: str.w r1,[r10,#0x28]
  000f6198: ldr.w r0,[r9,#0x0]
  000f619c: blx 0x00072f70
  000f61a0: mov r1,r0
  000f61a2: sub.w r0,r7,#0x38
  000f61a6: blx 0x0006f2b0
  000f61aa: ldr r0,[0x000f63a0]
  000f61ac: add r0,pc
  000f61ae: ldr r0,[r0,#0x0]
  000f61b0: str r0,[sp,#0x10]
  000f61b2: ldr r6,[r0,#0x0]
  000f61b4: sub.w r0,r7,#0x44
  000f61b8: mov r1,r10
  000f61ba: blx 0x00071c2c
  000f61be: ldr.w r5,[r7,#-0x3c]
  000f61c2: mov r0,r10
  000f61c4: blx 0x00071bd8
  000f61c8: mov r4,r0
  000f61ca: ldr r0,[0x000f63a4]
  000f61cc: add r0,pc
  000f61ce: ldr.w r11,[r0,#0x0]
  000f61d2: ldr.w r0,[r11,#0x0]
  000f61d6: blx 0x00071c14
  000f61da: blx 0x00071824
  000f61de: mul r1,r4,r5
  000f61e2: adds r2,r0,r1
  000f61e4: mov r0,r6
  000f61e6: movs r3,#0x0
  000f61e8: blx 0x000739c0
  000f61ec: sub.w r0,r7,#0x44
  000f61f0: blx 0x0006ee30
  000f61f4: ldr r1,[0x000f63a8]
  000f61f6: add r1,pc
  000f61f8: sub.w r0,r7,#0x44
  000f61fc: movs r2,#0x0
  000f61fe: blx 0x0006ee18
  000f6202: sub.w r0,r7,#0x38
  000f6206: sub.w r1,r7,#0x44
  000f620a: blx 0x0006f3a0
  000f620e: mov r4,r0
  000f6210: sub.w r0,r7,#0x44
  000f6214: blx 0x0006ee30
  000f6218: cmp r4,#0x0
  000f621a: blt 0x000f6296
  000f621c: ldr.w r6,[r11,#0x0]
  000f6220: sub.w r0,r7,#0xec
  000f6224: sub.w r1,r7,#0x38
  000f6228: movs r2,#0x0
  000f622a: blx 0x0006f028
  000f622e: sub.w r0,r7,#0x68
  000f6232: blx 0x00075e38
  000f6236: sub.w r0,r7,#0xf8
  000f623a: sub.w r1,r7,#0x68
  000f623e: movs r2,#0x0
  000f6240: blx 0x0006f028
  000f6244: ldr r1,[0x000f63ac]
  000f6246: add r1,pc
  000f6248: addw r0,sp,#0x45c
  000f624c: movs r2,#0x0
  000f624e: blx 0x0006ee18
  000f6252: str r0,[sp,#0x0]
  000f6254: sub.w r0,r7,#0x44
  000f6258: sub.w r2,r7,#0xec
  000f625c: sub.w r3,r7,#0xf8
  000f6260: mov r1,r6
  000f6262: blx 0x000739b4
  000f6266: sub.w r0,r7,#0x38
  000f626a: sub.w r1,r7,#0x44
  000f626e: blx 0x0006f2b0
  000f6272: ldr r0,[0x000f63b0]
  000f6274: add r0,pc
  000f6276: ldr r4,[r0,#0x0]
  000f6278: sub.w r0,r7,#0x44
  000f627c: blx r4
  000f627e: addw r0,sp,#0x45c
  000f6282: blx r4
  000f6284: sub.w r0,r7,#0xf8
  000f6288: blx r4
  000f628a: sub.w r0,r7,#0x68
  000f628e: blx r4
  000f6290: sub.w r0,r7,#0xec
  000f6294: blx r4
  000f6296: ldr.w r6,[r11,#0x0]
  000f629a: add.w r0,sp,#0x450
  000f629e: sub.w r1,r7,#0x38
  000f62a2: movs r2,#0x0
  000f62a4: blx 0x0006f028
  000f62a8: sub.w r0,r7,#0x68
  000f62ac: mov r1,r10
  000f62ae: blx 0x00071c2c
  000f62b2: addw r0,sp,#0x444
  000f62b6: sub.w r1,r7,#0x68
  000f62ba: movs r2,#0x0
  000f62bc: blx 0x0006f028
  000f62c0: ldr r1,[0x000f63b4]
  000f62c2: add r1,pc
  000f62c4: add.w r0,sp,#0x438
  000f62c8: movs r2,#0x0
  000f62ca: blx 0x0006ee18
  000f62ce: str r0,[sp,#0x0]
  000f62d0: sub.w r0,r7,#0x44
  000f62d4: add.w r2,sp,#0x450
  000f62d8: addw r3,sp,#0x444
  000f62dc: mov r1,r6
  000f62de: blx 0x000739b4
  000f62e2: sub.w r0,r7,#0x38
  000f62e6: sub.w r1,r7,#0x44
  000f62ea: mov r4,r9
  000f62ec: blx 0x0006f2b0
  000f62f0: ldr r0,[0x000f63b8]
  000f62f2: add r0,pc
  000f62f4: ldr.w r9,[r0,#0x0]
  000f62f8: sub.w r0,r7,#0x44
  000f62fc: blx r9
  000f62fe: add.w r0,sp,#0x438
  000f6302: blx r9
  000f6304: addw r0,sp,#0x444
  000f6308: blx r9
  000f630a: sub.w r0,r7,#0x68
  000f630e: blx r9
  000f6310: add.w r0,sp,#0x450
  000f6314: blx r9
  000f6316: ldr.w r6,[r11,#0x0]
  000f631a: addw r0,sp,#0x42c
  000f631e: sub.w r1,r7,#0x38
  000f6322: movs r2,#0x0
  000f6324: blx 0x0006f028
  000f6328: ldr r5,[sp,#0x10]
  000f632a: ldr r4,[r4,#0x0]
  000f632c: ldr r0,[r5,#0x0]
  000f632e: movs r1,#0xa
  000f6330: blx 0x00071848
  000f6334: addw r1,r0,#0x594
  000f6338: mov r0,r4
  000f633a: blx 0x00072f70
  000f633e: mov r1,r0
  000f6340: add.w r0,sp,#0x420
  000f6344: movs r2,#0x0
  000f6346: blx 0x0006f028
  000f634a: ldr r1,[0x000f63bc]
  000f634c: add r1,pc
  000f634e: addw r0,sp,#0x414
  000f6352: movs r2,#0x0
  000f6354: blx 0x0006ee18
  000f6358: str r0,[sp,#0x0]
  000f635a: sub.w r0,r7,#0x44
  000f635e: addw r2,sp,#0x42c
  000f6362: add.w r3,sp,#0x420
  000f6366: mov r1,r6
  000f6368: blx 0x000739b4
  000f636c: sub.w r0,r7,#0x38
  000f6370: sub.w r1,r7,#0x44
  000f6374: blx 0x0006f2b0
  000f6378: sub.w r0,r7,#0x44
  000f637c: blx r9
  000f637e: addw r0,sp,#0x414
  000f6382: blx r9
  000f6384: add.w r0,sp,#0x420
  000f6388: blx r9
  000f638a: addw r0,sp,#0x42c
  000f638e: blx r9
  000f6390: ldr r0,[r5,#0x0]
  000f6392: blx 0x000718cc
  000f6396: b.w 0x000f75e2
  000f63c0: ldr.w r1,[r10,#0x28]
  000f63c4: adds r0,r1,#0x1
  000f63c6: bne 0x000f63da
  000f63c8: ldr r0,[0x000f6690]
  000f63ca: add r0,pc
  000f63cc: ldr r0,[r0,#0x0]
  000f63ce: ldr r0,[r0,#0x0]
  000f63d0: movs r1,#0x5
  000f63d2: blx 0x00071848
  000f63d6: add.w r1,r0,#0x300
  000f63da: ldr r0,[0x000f6694]
  000f63dc: str.w r1,[r10,#0x28]
  000f63e0: add r0,pc
  000f63e2: ldr.w r9,[r0,#0x0]
  000f63e6: ldr.w r0,[r9,#0x0]
  000f63ea: blx 0x00072f70
  000f63ee: mov r1,r0
  000f63f0: sub.w r0,r7,#0x38
  000f63f4: blx 0x0006ef5c
  000f63f8: ldr.w r4,[r10,#0x2c]
  000f63fc: adds r0,r4,#0x1
  000f63fe: bne 0x000f6412
  000f6400: ldr r0,[0x000f6698]
  000f6402: add r0,pc
  000f6404: ldr r0,[r0,#0x0]
  000f6406: ldr r0,[r0,#0x0]
  000f6408: movs r1,#0x2
  000f640a: blx 0x00071848
  000f640e: addw r4,r0,#0x305
  000f6412: ldr r1,[0x000f669c]
  000f6414: str.w r4,[r10,#0x2c]
  000f6418: add r1,pc
  000f641a: sub.w r0,r7,#0x68
  000f641e: movs r2,#0x0
  000f6420: blx 0x0006ee18
  000f6424: ldr.w r0,[r9,#0x0]
  000f6428: mov r1,r4
  000f642a: blx 0x00072f70
  000f642e: mov r2,r0
  000f6430: sub.w r0,r7,#0x44
  000f6434: sub.w r1,r7,#0x68
  000f6438: blx 0x0006ef98
  000f643c: sub.w r0,r7,#0x38
  000f6440: sub.w r1,r7,#0x44
  000f6444: blx 0x0006ef5c
  000f6448: sub.w r0,r7,#0x44
  000f644c: blx 0x0006ee30
  000f6450: sub.w r0,r7,#0x68
  000f6454: blx 0x0006ee30
  000f6458: ldr r0,[0x000f66a0]
  000f645a: add r0,pc
  000f645c: ldr.w r11,[r0,#0x0]
  000f6460: ldr.w r6,[r11,#0x0]
  000f6464: add r0,sp,#0x294
  000f6466: sub.w r1,r7,#0x38
  000f646a: movs r2,#0x0
  000f646c: blx 0x0006f028
  000f6470: mov r0,r10
  000f6472: blx 0x00074bcc
  000f6476: mov r1,r0
  000f6478: sub.w r0,r7,#0x68
  000f647c: blx 0x0006f658
  000f6480: add r0,sp,#0x288
  000f6482: sub.w r1,r7,#0x68
  000f6486: movs r2,#0x0
  000f6488: blx 0x0006f028
  000f648c: ldr r1,[0x000f66a4]
  000f648e: add r1,pc
  000f6490: add r0,sp,#0x27c
  000f6492: movs r2,#0x0
  000f6494: blx 0x0006ee18
  000f6498: str r0,[sp,#0x0]
  000f649a: sub.w r0,r7,#0x44
  000f649e: add r2,sp,#0x294
  000f64a0: add r3,sp,#0x288
  000f64a2: mov r1,r6
  000f64a4: blx 0x000739b4
  000f64a8: sub.w r0,r7,#0x38
  000f64ac: sub.w r1,r7,#0x44
  000f64b0: blx 0x0006f2b0
  000f64b4: ldr r0,[0x000f66a8]
  000f64b6: add r0,pc
  000f64b8: ldr r6,[r0,#0x0]
  000f64ba: sub.w r0,r7,#0x44
  000f64be: blx r6
  000f64c0: add r0,sp,#0x27c
  000f64c2: blx r6
  000f64c4: add r0,sp,#0x288
  000f64c6: blx r6
  000f64c8: sub.w r0,r7,#0x68
  000f64cc: blx r6
  000f64ce: add r0,sp,#0x294
  000f64d0: blx r6
  000f64d2: ldr.w r5,[r11,#0x0]
  000f64d6: add r0,sp,#0x270
  000f64d8: sub.w r1,r7,#0x38
  000f64dc: movs r2,#0x0
  000f64de: blx 0x0006f028
  000f64e2: ldr.w r4,[r9,#0x0]
  000f64e6: mov r0,r10
  000f64e8: blx 0x00074bb4
  000f64ec: addw r1,r0,#0x4fa
  000f64f0: mov r0,r4
  000f64f2: blx 0x00072f70
  000f64f6: mov r1,r0
  000f64f8: add r0,sp,#0x264
  000f64fa: movs r2,#0x0
  000f64fc: blx 0x0006f028
  000f6500: ldr r1,[0x000f66ac]
  000f6502: add r1,pc
  000f6504: add r0,sp,#0x258
  000f6506: movs r2,#0x0
  000f6508: blx 0x0006ee18
  000f650c: str r0,[sp,#0x0]
  000f650e: sub.w r0,r7,#0x44
  000f6512: add r2,sp,#0x270
  000f6514: add r3,sp,#0x264
  000f6516: mov r1,r5
  000f6518: blx 0x000739b4
  000f651c: sub.w r0,r7,#0x38
  000f6520: sub.w r1,r7,#0x44
  000f6524: blx 0x0006f2b0
  000f6528: sub.w r0,r7,#0x44
  000f652c: str.w r9,[sp,#0x10]
  000f6530: blx r6
  000f6532: add r0,sp,#0x258
  000f6534: blx r6
  000f6536: add r0,sp,#0x264
  000f6538: blx r6
  000f653a: add r0,sp,#0x270
  000f653c: blx r6
  000f653e: ldr.w r5,[r11,#0x0]
  000f6542: add r0,sp,#0x24c
  000f6544: sub.w r1,r7,#0x38
  000f6548: movs r2,#0x0
  000f654a: blx 0x0006f028
  000f654e: ldr r0,[0x000f66b0]
  000f6550: add r0,pc
  000f6552: ldr.w r9,[r0,#0x0]
  000f6556: ldr.w r4,[r9,#0x0]
  000f655a: mov r0,r10
  000f655c: blx 0x00074bc0
  000f6560: mov r2,r0
  000f6562: sub.w r0,r7,#0x68
  000f6566: mov r1,r4
  000f6568: blx 0x00074e54
  000f656c: add r0,sp,#0x240
  000f656e: sub.w r1,r7,#0x68
  000f6572: movs r2,#0x0
  000f6574: blx 0x0006f028
  000f6578: ldr r1,[0x000f66b4]
  000f657a: add r1,pc
  000f657c: add r0,sp,#0x234
  000f657e: movs r2,#0x0
  000f6580: blx 0x0006ee18
  000f6584: str r0,[sp,#0x0]
  000f6586: sub.w r0,r7,#0x44
  000f658a: add r2,sp,#0x24c
  000f658c: add r3,sp,#0x240
  000f658e: mov r1,r5
  000f6590: blx 0x000739b4
  000f6594: sub.w r0,r7,#0x38
  000f6598: sub.w r1,r7,#0x44
  000f659c: blx 0x0006f2b0
  000f65a0: sub.w r0,r7,#0x44
  000f65a4: blx r6
  000f65a6: add r0,sp,#0x234
  000f65a8: blx r6
  000f65aa: add r0,sp,#0x240
  000f65ac: blx r6
  000f65ae: sub.w r0,r7,#0x68
  000f65b2: blx r6
  000f65b4: add r0,sp,#0x24c
  000f65b6: blx r6
  000f65b8: mov r0,r10
  000f65ba: blx 0x00074bcc
  000f65be: cmp r0,#0x2
  000f65c0: blt.w 0x000f75e2
  000f65c4: ldr r1,[0x000f66b8]
  000f65c6: add r1,pc
  000f65c8: sub.w r0,r7,#0x68
  000f65cc: movs r2,#0x0
  000f65ce: blx 0x0006ee18
  000f65d2: ldr r0,[sp,#0x10]
  000f65d4: ldr r0,[r0,#0x0]
  000f65d6: movw r1,#0x307
  000f65da: blx 0x00072f70
  000f65de: mov r2,r0
  000f65e0: sub.w r0,r7,#0x44
  000f65e4: sub.w r1,r7,#0x68
  000f65e8: blx 0x0006ef98
  000f65ec: sub.w r0,r7,#0x38
  000f65f0: sub.w r1,r7,#0x44
  000f65f4: blx 0x0006ef5c
  000f65f8: sub.w r0,r7,#0x44
  000f65fc: blx 0x0006ee30
  000f6600: sub.w r0,r7,#0x68
  000f6604: blx 0x0006ee30
  000f6608: ldr.w r11,[r11,#0x0]
  000f660c: add r0,sp,#0x228
  000f660e: sub.w r1,r7,#0x38
  000f6612: movs r2,#0x0
  000f6614: blx 0x0006f028
  000f6618: ldr.w r4,[r9,#0x0]
  000f661c: mov r0,r10
  000f661e: blx 0x00074bc0
  000f6622: mov r5,r0
  000f6624: mov r0,r10
  000f6626: blx 0x00074bcc
  000f662a: mov r1,r0
  000f662c: mov r0,r5
  000f662e: blx 0x0007198c
  000f6632: mov r2,r0
  000f6634: sub.w r0,r7,#0x68
  000f6638: mov r1,r4
  000f663a: blx 0x00074e54
  000f663e: add r0,sp,#0x21c
  000f6640: sub.w r1,r7,#0x68
  000f6644: movs r2,#0x0
  000f6646: blx 0x0006f028
  000f664a: ldr r1,[0x000f66bc]
  000f664c: add r1,pc
  000f664e: add r0,sp,#0x210
  000f6650: movs r2,#0x0
  000f6652: blx 0x0006ee18
  000f6656: str r0,[sp,#0x0]
  000f6658: sub.w r0,r7,#0x44
  000f665c: add r2,sp,#0x228
  000f665e: add r3,sp,#0x21c
  000f6660: mov r1,r11
  000f6662: blx 0x000739b4
  000f6666: sub.w r0,r7,#0x38
  000f666a: sub.w r1,r7,#0x44
  000f666e: blx 0x0006f2b0
  000f6672: sub.w r0,r7,#0x44
  000f6676: blx r6
  000f6678: add r0,sp,#0x210
  000f667a: blx r6
  000f667c: add r0,sp,#0x21c
  000f667e: blx r6
  000f6680: sub.w r0,r7,#0x68
  000f6684: blx r6
  000f6686: add r0,sp,#0x228
  000f6688: blx r6
  000f668a: b.w 0x000f75e2
  000f66c0: mov r0,r10
  000f66c2: blx 0x00071bb4
  000f66c6: cbnz r0,0x000f66ec
  000f66c8: ldr r0,[0x000f6a58]
  000f66ca: add r0,pc
  000f66cc: ldr r0,[r0,#0x0]
  000f66ce: ldr r0,[r0,#0x0]
  000f66d0: blx 0x00071884
  000f66d4: mov r2,r0
  000f66d6: ldr r0,[0x000f6a5c]
  000f66d8: add r0,pc
  000f66da: ldr r0,[r0,#0x0]
  000f66dc: ldr r0,[r0,#0x0]
  000f66de: mov r1,r10
  000f66e0: blx 0x00071b6c
  000f66e4: mov r1,r0
  000f66e6: mov r0,r10
  000f66e8: blx 0x00071b78
  000f66ec: ldr.w r1,[r10,#0x28]
  000f66f0: adds r0,r1,#0x1
  000f66f2: bne 0x000f6706
  000f66f4: ldr r0,[0x000f6a60]
  000f66f6: add r0,pc
  000f66f8: ldr r0,[r0,#0x0]
  000f66fa: ldr r0,[r0,#0x0]
  000f66fc: movs r1,#0x2
  000f66fe: blx 0x00071848
  000f6702: addw r1,r0,#0x309
  000f6706: ldr r0,[0x000f6a64]
  000f6708: str.w r1,[r10,#0x28]
  000f670c: add r0,pc
  000f670e: ldr r4,[r0,#0x0]
  000f6710: ldr r0,[r4,#0x0]
  000f6712: blx 0x00072f70
  000f6716: mov r1,r0
  000f6718: sub.w r0,r7,#0x38
  000f671c: blx 0x0006ef5c
  000f6720: ldr r0,[0x000f6a68]
  000f6722: add r0,pc
  000f6724: ldr.w r9,[r0,#0x0]
  000f6728: ldr.w r6,[r9,#0x0]
  000f672c: sub.w r0,r7,#0x80
  000f6730: sub.w r1,r7,#0x38
  000f6734: movs r2,#0x0
  000f6736: blx 0x0006f028
  000f673a: mov r0,r10
  000f673c: blx 0x00071bb4
  000f6740: blx 0x00072bb0
  000f6744: mov r1,r0
  000f6746: sub.w r0,r7,#0x68
  000f674a: blx 0x0006f658
  000f674e: sub.w r0,r7,#0x8c
  000f6752: sub.w r1,r7,#0x68
  000f6756: movs r2,#0x0
  000f6758: blx 0x0006f028
  000f675c: ldr r1,[0x000f6a6c]
  000f675e: add r1,pc
  000f6760: sub.w r0,r7,#0x98
  000f6764: movs r2,#0x0
  000f6766: blx 0x0006ee18
  000f676a: str r0,[sp,#0x0]
  000f676c: sub.w r0,r7,#0x44
  000f6770: sub.w r2,r7,#0x80
  000f6774: sub.w r3,r7,#0x8c
  000f6778: mov r1,r6
  000f677a: blx 0x000739b4
  000f677e: sub.w r0,r7,#0x38
  000f6782: sub.w r1,r7,#0x44
  000f6786: blx 0x0006f2b0
  000f678a: ldr r0,[0x000f6a70]
  000f678c: add r0,pc
  000f678e: ldr r5,[r0,#0x0]
  000f6790: sub.w r0,r7,#0x44
  000f6794: blx r5
  000f6796: sub.w r0,r7,#0x98
  000f679a: blx r5
  000f679c: sub.w r0,r7,#0x8c
  000f67a0: blx r5
  000f67a2: sub.w r0,r7,#0x68
  000f67a6: blx r5
  000f67a8: sub.w r0,r7,#0x80
  000f67ac: blx r5
  000f67ae: ldr.w r6,[r9,#0x0]
  000f67b2: sub.w r0,r7,#0xa4
  000f67b6: sub.w r1,r7,#0x38
  000f67ba: movs r2,#0x0
  000f67bc: blx 0x0006f028
  000f67c0: ldr r4,[r4,#0x0]
  000f67c2: mov r0,r10
  000f67c4: blx 0x00071bb4
  000f67c8: blx 0x000728a4
  000f67cc: addw r1,r0,#0x4fa
  000f67d0: mov r0,r4
  000f67d2: blx 0x00072f70
  000f67d6: mov r1,r0
  000f67d8: sub.w r0,r7,#0xb0
  000f67dc: movs r2,#0x0
  000f67de: blx 0x0006f028
  000f67e2: ldr r1,[0x000f6a74]
  000f67e4: add r1,pc
  000f67e6: sub.w r0,r7,#0xbc
  000f67ea: movs r2,#0x0
  000f67ec: blx 0x0006ee18
  000f67f0: str r0,[sp,#0x0]
  000f67f2: sub.w r0,r7,#0x44
  000f67f6: sub.w r2,r7,#0xa4
  000f67fa: sub.w r3,r7,#0xb0
  000f67fe: mov r1,r6
  000f6800: blx 0x000739b4
  000f6804: sub.w r0,r7,#0x38
  000f6808: sub.w r1,r7,#0x44
  000f680c: blx 0x0006f2b0
  000f6810: sub.w r0,r7,#0x44
  000f6814: blx r5
  000f6816: sub.w r0,r7,#0xbc
  000f681a: blx r5
  000f681c: sub.w r0,r7,#0xb0
  000f6820: blx r5
  000f6822: sub.w r0,r7,#0xa4
  000f6826: blx r5
  000f6828: ldr.w r6,[r9,#0x0]
  000f682c: sub.w r0,r7,#0xc8
  000f6830: sub.w r1,r7,#0x38
  000f6834: movs r2,#0x0
  000f6836: blx 0x0006f028
  000f683a: ldr r0,[0x000f6a78]
  000f683c: add r0,pc
  000f683e: ldr r0,[r0,#0x0]
  000f6840: ldr r4,[r0,#0x0]
  000f6842: mov r0,r10
  000f6844: blx 0x00071bb4
  000f6848: blx 0x00071bc0
  000f684c: mov r2,r0
  000f684e: sub.w r0,r7,#0x68
  000f6852: mov r1,r4
  000f6854: blx 0x00074e54
  000f6858: sub.w r0,r7,#0xd4
  000f685c: sub.w r1,r7,#0x68
  000f6860: movs r2,#0x0
  000f6862: blx 0x0006f028
  000f6866: ldr r1,[0x000f6a7c]
  000f6868: add r1,pc
  000f686a: sub.w r0,r7,#0xe0
  000f686e: movs r2,#0x0
  000f6870: blx 0x0006ee18
  000f6874: str r0,[sp,#0x0]
  000f6876: sub.w r0,r7,#0x44
  000f687a: sub.w r2,r7,#0xc8
  000f687e: sub.w r3,r7,#0xd4
  000f6882: mov r1,r6
  000f6884: blx 0x000739b4
  000f6888: sub.w r0,r7,#0x38
  000f688c: sub.w r1,r7,#0x44
  000f6890: blx 0x0006f2b0
  000f6894: sub.w r0,r7,#0x44
  000f6898: blx r5
  000f689a: sub.w r0,r7,#0xe0
  000f689e: blx r5
  000f68a0: sub.w r0,r7,#0xd4
  000f68a4: blx r5
  000f68a6: sub.w r0,r7,#0x68
  000f68aa: blx r5
  000f68ac: sub.w r0,r7,#0xc8
  000f68b0: blx r5
  000f68b2: b.w 0x000f75e2
  000f68b6: ldr r0,[0x000f6a80]
  000f68b8: ldr r1,[0x000f6a84]
  000f68ba: add r0,pc
  000f68bc: add r1,pc
  000f68be: ldr r0,[r0,#0x0]
  000f68c0: ldr r1,[r1,#0x0]
  000f68c2: ldr r4,[r0,#0x0]
  000f68c4: ldr r0,[r1,#0x0]
  000f68c6: blx 0x00075580
  000f68ca: mov r5,r0
  000f68cc: mov r0,r10
  000f68ce: blx 0x00074bd8
  000f68d2: movw r1,#0x30b
  000f68d6: cmp r5,#0x0
  000f68d8: it ne
  000f68da: movw.ne r1,#0x30e
  000f68de: add r1,r0
  000f68e0: mov r0,r4
  000f68e2: blx 0x00072f70
  000f68e6: mov r1,r0
  000f68e8: sub.w r0,r7,#0x38
  000f68ec: blx 0x0006ef5c
  000f68f0: ldr r0,[0x000f6a88]
  000f68f2: add r0,pc
  000f68f4: ldr r5,[r0,#0x0]
  000f68f6: ldr r6,[r5,#0x0]
  000f68f8: add r0,sp,#0x204
  000f68fa: sub.w r1,r7,#0x38
  000f68fe: movs r2,#0x0
  000f6900: blx 0x0006f028
  000f6904: ldr r0,[0x000f6a8c]
  000f6906: add r0,pc
  000f6908: ldr r0,[r0,#0x0]
  000f690a: ldr r4,[r0,#0x0]
  000f690c: mov r0,r10
  000f690e: blx 0x00071bfc
  000f6912: mov r2,r0
  000f6914: sub.w r0,r7,#0x68
  000f6918: mov r1,r4
  000f691a: blx 0x00074e54
  000f691e: add r0,sp,#0x1f8
  000f6920: sub.w r1,r7,#0x68
  000f6924: movs r2,#0x0
  000f6926: blx 0x0006f028
  000f692a: ldr r1,[0x000f6a90]
  000f692c: add r1,pc
  000f692e: add r0,sp,#0x1ec
  000f6930: movs r2,#0x0
  000f6932: blx 0x0006ee18
  000f6936: str r0,[sp,#0x0]
  000f6938: sub.w r0,r7,#0x44
  000f693c: add r2,sp,#0x204
  000f693e: add r3,sp,#0x1f8
  000f6940: mov r1,r6
  000f6942: blx 0x000739b4
  000f6946: sub.w r0,r7,#0x38
  000f694a: sub.w r1,r7,#0x44
  000f694e: blx 0x0006f2b0
  000f6952: ldr r0,[0x000f6a94]
  000f6954: add r0,pc
  000f6956: ldr r4,[r0,#0x0]
  000f6958: sub.w r0,r7,#0x44
  000f695c: blx r4
  000f695e: add r0,sp,#0x1ec
  000f6960: blx r4
  000f6962: add r0,sp,#0x1f8
  000f6964: blx r4
  000f6966: sub.w r0,r7,#0x68
  000f696a: blx r4
  000f696c: add r0,sp,#0x204
  000f696e: blx r4
  000f6970: mov r0,r10
  000f6972: blx 0x00074bd8
  000f6976: cmp r0,#0x1
  000f6978: blt.w 0x000f75e2
  000f697c: ldr r6,[r5,#0x0]
  000f697e: add r0,sp,#0x1e0
  000f6980: sub.w r1,r7,#0x38
  000f6984: movs r2,#0x0
  000f6986: blx 0x0006f028
  000f698a: sub.w r0,r7,#0x68
  000f698e: mov r1,r10
  000f6990: movs r2,#0x1
  000f6992: blx 0x00075e44
  000f6996: add r0,sp,#0x1d4
  000f6998: sub.w r1,r7,#0x68
  000f699c: movs r2,#0x0
  000f699e: blx 0x0006f028
  000f69a2: ldr r1,[0x000f6a98]
  000f69a4: add r1,pc
  000f69a6: add r0,sp,#0x1c8
  000f69a8: movs r2,#0x0
  000f69aa: blx 0x0006ee18
  000f69ae: str r0,[sp,#0x0]
  000f69b0: sub.w r0,r7,#0x44
  000f69b4: add r2,sp,#0x1e0
  000f69b6: add r3,sp,#0x1d4
  000f69b8: mov r1,r6
  000f69ba: blx 0x000739b4
  000f69be: sub.w r0,r7,#0x38
  000f69c2: sub.w r1,r7,#0x44
  000f69c6: blx 0x0006f2b0
  000f69ca: sub.w r0,r7,#0x44
  000f69ce: blx r4
  000f69d0: add r0,sp,#0x1c8
  000f69d2: blx r4
  000f69d4: add r0,sp,#0x1d4
  000f69d6: blx r4
  000f69d8: sub.w r0,r7,#0x68
  000f69dc: blx r4
  000f69de: add r0,sp,#0x1e0
  000f69e0: b.w 0x000f75e0
  000f69e4: mov r0,r10
  000f69e6: blx 0x00071bd8
  000f69ea: mov r6,r0
  000f69ec: ldr r0,[0x000f6a9c]
  000f69ee: add r0,pc
  000f69f0: ldr r5,[r0,#0x0]
  000f69f2: ldr r0,[r5,#0x0]
  000f69f4: blx 0x00071b84
  000f69f8: ldr.w r9,[sp,#0x14]
  000f69fc: orr r1,r6,#0x1
  000f6a00: subs r1,#0x3
  000f6a02: clz r1,r1
  000f6a06: lsrs r1,r1,#0x5
  000f6a08: blx 0x00074fb0
  000f6a0c: mov r4,r0
  000f6a0e: ldr r0,[r5,#0x0]
  000f6a10: blx 0x00071b84
  000f6a14: mov r1,r6
  000f6a16: blx 0x00071b90
  000f6a1a: cmp r0,#0x0
  000f6a1c: beq.w 0x000f6ca2
  000f6a20: cmp r4,#0x0
  000f6a22: bpl 0x000f6a26
  000f6a24: rsbs r4,r4
  000f6a26: vmov s0,r4
  000f6a2a: vldr.32 s2,[pc,#0x74]
  000f6a2e: cmp r6,#0x2
  000f6a30: vcvt.f32.s32 s0,s0
  000f6a34: vdiv.f32 s0,s0,s2
  000f6a38: vldr.32 s2,[pc,#0x68]
  000f6a3c: vmul.f32 s0,s0,s2
  000f6a40: vcvt.s32.f32 s16,s0
  000f6a44: beq.w 0x000f7536
  000f6a48: cmp r6,#0x3
  000f6a4a: bne.w 0x000f753c
  000f6a4e: mov.w r1,#0x370
  000f6a52: b.w 0x000f7548
  000f6aa8: mov r0,r10
  000f6aaa: blx 0x00071ae8
  000f6aae: cmp r0,#0x9
  000f6ab0: bne.w 0x000f6d1c
  000f6ab4: ldr.w r1,[r10,#0x28]
  000f6ab8: str.w r11,[sp,#0x14]
  000f6abc: adds r0,r1,#0x1
  000f6abe: bne 0x000f6ad2
  000f6ac0: ldr r0,[0x000f6cf0]
  000f6ac2: add r0,pc
  000f6ac4: ldr r0,[r0,#0x0]
  000f6ac6: ldr r0,[r0,#0x0]
  000f6ac8: movs r1,#0x2
  000f6aca: blx 0x00071848
  000f6ace: addw r1,r0,#0x36a
  000f6ad2: ldr r0,[0x000f6cf4]
  000f6ad4: str.w r1,[r10,#0x28]
  000f6ad8: add r0,pc
  000f6ada: ldr.w r11,[r0,#0x0]
  000f6ade: ldr.w r0,[r11,#0x0]
  000f6ae2: blx 0x00072f70
  000f6ae6: mov r1,r0
  000f6ae8: sub.w r0,r7,#0x38
  000f6aec: blx 0x0006f2b0
  000f6af0: ldr r1,[0x000f6cf8]
  000f6af2: add r1,pc
  000f6af4: add r0,sp,#0x330
  000f6af6: movs r2,#0x0
  000f6af8: blx 0x0006ee18
  000f6afc: ldr.w r4,[r11,#0x0]
  000f6b00: mov r0,r10
  000f6b02: blx 0x00074ba8
  000f6b06: addw r1,r0,#0x376
  000f6b0a: mov r0,r4
  000f6b0c: blx 0x00072f70
  000f6b10: mov r2,r0
  000f6b12: sub.w r0,r7,#0x68
  000f6b16: add r1,sp,#0x330
  000f6b18: blx 0x0006ef98
  000f6b1c: sub.w r0,r7,#0x38
  000f6b20: sub.w r1,r7,#0x68
  000f6b24: blx 0x0006ef5c
  000f6b28: sub.w r0,r7,#0x68
  000f6b2c: blx 0x0006ee30
  000f6b30: add r0,sp,#0x330
  000f6b32: blx 0x0006ee30
  000f6b36: ldr.w r4,[r10,#0x2c]
  000f6b3a: adds r0,r4,#0x1
  000f6b3c: bne 0x000f6b50
  000f6b3e: ldr r0,[0x000f6cfc]
  000f6b40: add r0,pc
  000f6b42: ldr r0,[r0,#0x0]
  000f6b44: ldr r0,[r0,#0x0]
  000f6b46: movs r1,#0x2
  000f6b48: blx 0x00071848
  000f6b4c: addw r4,r0,#0x305
  000f6b50: ldr r1,[0x000f6d00]
  000f6b52: str.w r4,[r10,#0x2c]
  000f6b56: add r1,pc
  000f6b58: add r0,sp,#0x330
  000f6b5a: movs r2,#0x0
  000f6b5c: blx 0x0006ee18
  000f6b60: ldr.w r0,[r11,#0x0]
  000f6b64: mov r1,r4
  000f6b66: blx 0x00072f70
  000f6b6a: mov r2,r0
  000f6b6c: sub.w r0,r7,#0x68
  000f6b70: add r1,sp,#0x330
  000f6b72: blx 0x0006ef98
  000f6b76: sub.w r0,r7,#0x38
  000f6b7a: sub.w r1,r7,#0x68
  000f6b7e: blx 0x0006ef5c
  000f6b82: sub.w r0,r7,#0x68
  000f6b86: blx 0x0006ee30
  000f6b8a: add r0,sp,#0x330
  000f6b8c: blx 0x0006ee30
  000f6b90: ldr.w r6,[r9,#0x0]
  000f6b94: add r0,sp,#0x174
  000f6b96: sub.w r1,r7,#0x38
  000f6b9a: movs r2,#0x0
  000f6b9c: blx 0x0006f028
  000f6ba0: mov r0,r10
  000f6ba2: blx 0x00074bcc
  000f6ba6: mov r1,r0
  000f6ba8: add r0,sp,#0x330
  000f6baa: blx 0x0006f658
  000f6bae: add r0,sp,#0x168
  000f6bb0: add r1,sp,#0x330
  000f6bb2: movs r2,#0x0
  000f6bb4: blx 0x0006f028
  000f6bb8: ldr r1,[0x000f6d04]
  000f6bba: add r1,pc
  000f6bbc: add r0,sp,#0x15c
  000f6bbe: movs r2,#0x0
  000f6bc0: blx 0x0006ee18
  000f6bc4: str r0,[sp,#0x0]
  000f6bc6: sub.w r0,r7,#0x68
  000f6bca: add r2,sp,#0x174
  000f6bcc: add r3,sp,#0x168
  000f6bce: mov r1,r6
  000f6bd0: blx 0x000739b4
  000f6bd4: sub.w r0,r7,#0x38
  000f6bd8: sub.w r1,r7,#0x68
  000f6bdc: blx 0x0006f2b0
  000f6be0: ldr r0,[0x000f6d08]
  000f6be2: add r0,pc
  000f6be4: ldr r5,[r0,#0x0]
  000f6be6: sub.w r0,r7,#0x68
  000f6bea: blx r5
  000f6bec: add r0,sp,#0x15c
  000f6bee: blx r5
  000f6bf0: add r0,sp,#0x168
  000f6bf2: blx r5
  000f6bf4: add r0,sp,#0x330
  000f6bf6: blx r5
  000f6bf8: add r0,sp,#0x174
  000f6bfa: blx r5
  000f6bfc: ldr.w r6,[r9,#0x0]
  000f6c00: add r0,sp,#0x150
  000f6c02: sub.w r1,r7,#0x38
  000f6c06: movs r2,#0x0
  000f6c08: blx 0x0006f028
  000f6c0c: ldr.w r4,[r11,#0x0]
  000f6c10: mov r0,r10
  000f6c12: blx 0x00074bb4
  000f6c16: addw r1,r0,#0x4fa
  000f6c1a: mov r0,r4
  000f6c1c: blx 0x00072f70
  000f6c20: mov r1,r0
  000f6c22: add r0,sp,#0x144
  000f6c24: movs r2,#0x0
  000f6c26: blx 0x0006f028
  000f6c2a: ldr r1,[0x000f6d0c]
  000f6c2c: add r1,pc
  000f6c2e: add r0,sp,#0x138
  000f6c30: movs r2,#0x0
  000f6c32: blx 0x0006ee18
  000f6c36: ldr.w r11,[sp,#0x14]
  000f6c3a: str r0,[sp,#0x0]
  000f6c3c: sub.w r0,r7,#0x68
  000f6c40: add r2,sp,#0x150
  000f6c42: add r3,sp,#0x144
  000f6c44: mov r1,r6
  000f6c46: blx 0x000739b4
  000f6c4a: sub.w r0,r7,#0x38
  000f6c4e: sub.w r1,r7,#0x68
  000f6c52: blx 0x0006f2b0
  000f6c56: sub.w r0,r7,#0x68
  000f6c5a: blx r5
  000f6c5c: add r0,sp,#0x138
  000f6c5e: blx r5
  000f6c60: add r0,sp,#0x144
  000f6c62: blx r5
  000f6c64: add r0,sp,#0x150
  000f6c66: blx r5
  000f6c68: ldr.w r6,[r9,#0x0]
  000f6c6c: add r0,sp,#0x12c
  000f6c6e: sub.w r1,r7,#0x38
  000f6c72: movs r2,#0x0
  000f6c74: blx 0x0006f028
  000f6c78: ldr r0,[0x000f6d10]
  000f6c7a: ldr r1,[0x000f6d14]
  000f6c7c: add r0,pc
  000f6c7e: add r1,pc
  000f6c80: ldr r0,[r0,#0x0]
  000f6c82: ldr r1,[r1,#0x0]
  000f6c84: ldr r4,[r0,#0x0]
  000f6c86: ldrb.w r0,[r1,#0x38]
  000f6c8a: cmp r0,#0x0
  000f6c8c: beq.w 0x000f7904
  000f6c90: mov r0,r10
  000f6c92: blx 0x00074bc0
  000f6c96: vmov s0,r0
  000f6c9a: vcvt.f32.s32 s0,s0
  000f6c9e: b.w 0x000f7916
  000f6ca2: ldr r0,[0x000f6d18]
  000f6ca4: add r0,pc
  000f6ca6: ldr r0,[r0,#0x0]
  000f6ca8: ldr r0,[r0,#0x0]
  000f6caa: movw r1,#0x373
  000f6cae: blx 0x00072f70
  000f6cb2: mov r1,r0
  000f6cb4: sub.w r0,r7,#0x38
  000f6cb8: blx 0x0006f2b0
  000f6cbc: sub.w r1,r7,#0x38
  000f6cc0: mov r0,r9
  000f6cc2: movs r2,#0x0
  000f6cc4: blx 0x0006f028
  000f6cc8: b.w 0x000f7c16
  000f6ccc: ldr r0,[r0,#0x0]
  000f6cce: mov.w r1,#0x334
  000f6cd2: str.w r1,[r10,#0x28]
  000f6cd6: ldr r0,[r0,#0x0]
  000f6cd8: mov.w r1,#0x334
  000f6cdc: blx 0x00072f70
  000f6ce0: mov r1,r0
  000f6ce2: sub.w r0,r7,#0x38
  000f6ce6: blx 0x0006f2b0
  000f6cea: b.w 0x000f75e2
  000f6d1c: mov r0,r10
  000f6d1e: blx 0x00071ae8
  000f6d22: cmp r0,#0xa
  000f6d24: bne.w 0x000f76e4
  000f6d28: ldr.w r1,[r10,#0x28]
  000f6d2c: str.w r11,[sp,#0x14]
  000f6d30: adds r0,r1,#0x1
  000f6d32: bne 0x000f6d46
  000f6d34: ldr r0,[0x000f7000]
  000f6d36: add r0,pc
  000f6d38: ldr r0,[r0,#0x0]
  000f6d3a: ldr r0,[r0,#0x0]
  000f6d3c: movs r1,#0x2
  000f6d3e: blx 0x00071848
  000f6d42: addw r1,r0,#0x36a
  000f6d46: ldr r0,[0x000f7004]
  000f6d48: str.w r1,[r10,#0x28]
  000f6d4c: add r0,pc
  000f6d4e: ldr.w r11,[r0,#0x0]
  000f6d52: ldr.w r0,[r11,#0x0]
  000f6d56: blx 0x00072f70
  000f6d5a: mov r1,r0
  000f6d5c: sub.w r0,r7,#0x38
  000f6d60: blx 0x0006f2b0
  000f6d64: ldr r1,[0x000f7008]
  000f6d66: add r1,pc
  000f6d68: add r0,sp,#0x330
  000f6d6a: movs r2,#0x0
  000f6d6c: blx 0x0006ee18
  000f6d70: ldr.w r4,[r11,#0x0]
  000f6d74: mov r0,r10
  000f6d76: blx 0x00074ba8
  000f6d7a: addw r1,r0,#0x376
  000f6d7e: mov r0,r4
  000f6d80: blx 0x00072f70
  000f6d84: mov r2,r0
  000f6d86: sub.w r0,r7,#0x68
  000f6d8a: add r1,sp,#0x330
  000f6d8c: blx 0x0006ef98
  000f6d90: sub.w r0,r7,#0x38
  000f6d94: sub.w r1,r7,#0x68
  000f6d98: blx 0x0006ef5c
  000f6d9c: sub.w r0,r7,#0x68
  000f6da0: blx 0x0006ee30
  000f6da4: add r0,sp,#0x330
  000f6da6: blx 0x0006ee30
  000f6daa: ldr.w r4,[r10,#0x2c]
  000f6dae: adds r0,r4,#0x1
  000f6db0: bne 0x000f6dc4
  000f6db2: ldr r0,[0x000f700c]
  000f6db4: add r0,pc
  000f6db6: ldr r0,[r0,#0x0]
  000f6db8: ldr r0,[r0,#0x0]
  000f6dba: movs r1,#0x2
  000f6dbc: blx 0x00071848
  000f6dc0: addw r4,r0,#0x305
  000f6dc4: ldr r1,[0x000f7010]
  000f6dc6: str.w r4,[r10,#0x2c]
  000f6dca: add r1,pc
  000f6dcc: add r0,sp,#0x330
  000f6dce: movs r2,#0x0
  000f6dd0: blx 0x0006ee18
  000f6dd4: ldr.w r0,[r11,#0x0]
  000f6dd8: mov r1,r4
  000f6dda: blx 0x00072f70
  000f6dde: mov r2,r0
  000f6de0: sub.w r0,r7,#0x68
  000f6de4: add r1,sp,#0x330
  000f6de6: blx 0x0006ef98
  000f6dea: sub.w r0,r7,#0x38
  000f6dee: sub.w r1,r7,#0x68
  000f6df2: blx 0x0006ef5c
  000f6df6: sub.w r0,r7,#0x68
  000f6dfa: blx 0x0006ee30
  000f6dfe: add r0,sp,#0x330
  000f6e00: blx 0x0006ee30
  000f6e04: ldr.w r6,[r9,#0x0]
  000f6e08: add r0,sp,#0x108
  000f6e0a: sub.w r1,r7,#0x38
  000f6e0e: movs r2,#0x0
  000f6e10: blx 0x0006f028
  000f6e14: add r0,sp,#0x330
  000f6e16: movs r1,#0x1
  000f6e18: blx 0x0006f658
  000f6e1c: add r0,sp,#0xfc
  000f6e1e: add r1,sp,#0x330
  000f6e20: movs r2,#0x0
  000f6e22: blx 0x0006f028
  000f6e26: ldr r1,[0x000f7014]
  000f6e28: add r1,pc
  000f6e2a: add r0,sp,#0xf0
  000f6e2c: movs r2,#0x0
  000f6e2e: blx 0x0006ee18
  000f6e32: str r0,[sp,#0x0]
  000f6e34: sub.w r0,r7,#0x68
  000f6e38: add r2,sp,#0x108
  000f6e3a: add r3,sp,#0xfc
  000f6e3c: mov r1,r6
  000f6e3e: blx 0x000739b4
  000f6e42: sub.w r0,r7,#0x38
  000f6e46: sub.w r1,r7,#0x68
  000f6e4a: blx 0x0006f2b0
  000f6e4e: ldr r0,[0x000f7018]
  000f6e50: add r0,pc
  000f6e52: ldr r5,[r0,#0x0]
  000f6e54: sub.w r0,r7,#0x68
  000f6e58: blx r5
  000f6e5a: add r0,sp,#0xf0
  000f6e5c: blx r5
  000f6e5e: add r0,sp,#0xfc
  000f6e60: blx r5
  000f6e62: add r0,sp,#0x330
  000f6e64: blx r5
  000f6e66: add r0,sp,#0x108
  000f6e68: blx r5
  000f6e6a: ldr.w r6,[r9,#0x0]
  000f6e6e: add r0,sp,#0xe4
  000f6e70: sub.w r1,r7,#0x38
  000f6e74: movs r2,#0x0
  000f6e76: blx 0x0006f028
  000f6e7a: ldr.w r4,[r11,#0x0]
  000f6e7e: mov r0,r10
  000f6e80: blx 0x00074bb4
  000f6e84: addw r1,r0,#0x391
  000f6e88: mov r0,r4
  000f6e8a: blx 0x00072f70
  000f6e8e: mov r1,r0
  000f6e90: add r0,sp,#0xd8
  000f6e92: movs r2,#0x0
  000f6e94: blx 0x0006f028
  000f6e98: ldr r1,[0x000f701c]
  000f6e9a: add r1,pc
  000f6e9c: add r0,sp,#0xcc
  000f6e9e: movs r2,#0x0
  000f6ea0: blx 0x0006ee18
  000f6ea4: ldr.w r11,[sp,#0x14]
  000f6ea8: str r0,[sp,#0x0]
  000f6eaa: sub.w r0,r7,#0x68
  000f6eae: add r2,sp,#0xe4
  000f6eb0: add r3,sp,#0xd8
  000f6eb2: mov r1,r6
  000f6eb4: blx 0x000739b4
  000f6eb8: sub.w r0,r7,#0x38
  000f6ebc: sub.w r1,r7,#0x68
  000f6ec0: blx 0x0006f2b0
  000f6ec4: sub.w r0,r7,#0x68
  000f6ec8: blx r5
  000f6eca: add r0,sp,#0xcc
  000f6ecc: blx r5
  000f6ece: add r0,sp,#0xd8
  000f6ed0: blx r5
  000f6ed2: add r0,sp,#0xe4
  000f6ed4: blx r5
  000f6ed6: ldr.w r6,[r9,#0x0]
  000f6eda: add r0,sp,#0xc0
  000f6edc: sub.w r1,r7,#0x38
  000f6ee0: movs r2,#0x0
  000f6ee2: blx 0x0006f028
  000f6ee6: ldr r0,[0x000f7020]
  000f6ee8: ldr r1,[0x000f7024]
  000f6eea: add r0,pc
  000f6eec: add r1,pc
  000f6eee: ldr r0,[r0,#0x0]
  000f6ef0: ldr r1,[r1,#0x0]
  000f6ef2: ldr r4,[r0,#0x0]
  000f6ef4: ldrb.w r0,[r1,#0x38]
  000f6ef8: cmp r0,#0x0
  000f6efa: beq.w 0x000f7b46
  000f6efe: mov r0,r10
  000f6f00: blx 0x00074bc0
  000f6f04: vmov s0,r0
  000f6f08: vcvt.f32.s32 s0,s0
  000f6f0c: b.w 0x000f7b58
  000f6f10: ldr.w r1,[r10,#0x28]
  000f6f14: adds r0,r1,#0x1
  000f6f16: bne 0x000f6f2a
  000f6f18: ldr r0,[0x000f7028]
  000f6f1a: add r0,pc
  000f6f1c: ldr r0,[r0,#0x0]
  000f6f1e: ldr r0,[r0,#0x0]
  000f6f20: movs r1,#0x2
  000f6f22: blx 0x00071848
  000f6f26: addw r1,r0,#0x36a
  000f6f2a: ldr r0,[0x000f702c]
  000f6f2c: str.w r1,[r10,#0x28]
  000f6f30: add r0,pc
  000f6f32: ldr r5,[r0,#0x0]
  000f6f34: ldr r0,[r5,#0x0]
  000f6f36: blx 0x00072f70
  000f6f3a: mov r1,r0
  000f6f3c: sub.w r0,r7,#0x38
  000f6f40: blx 0x0006f2b0
  000f6f44: ldr r1,[0x000f7030]
  000f6f46: add r1,pc
  000f6f48: add r0,sp,#0x330
  000f6f4a: movs r2,#0x0
  000f6f4c: blx 0x0006ee18
  000f6f50: ldr r4,[r5,#0x0]
  000f6f52: mov r0,r10
  000f6f54: blx 0x00074ba8
  000f6f58: addw r1,r0,#0x376
  000f6f5c: mov r0,r4
  000f6f5e: blx 0x00072f70
  000f6f62: mov r2,r0
  000f6f64: sub.w r0,r7,#0x68
  000f6f68: add r1,sp,#0x330
  000f6f6a: blx 0x0006ef98
  000f6f6e: sub.w r0,r7,#0x38
  000f6f72: sub.w r1,r7,#0x68
  000f6f76: blx 0x0006ef5c
  000f6f7a: sub.w r0,r7,#0x68
  000f6f7e: blx 0x0006ee30
  000f6f82: add r0,sp,#0x330
  000f6f84: blx 0x0006ee30
  000f6f88: ldr r1,[0x000f7034]
  000f6f8a: add r1,pc
  000f6f8c: add r0,sp,#0x330
  000f6f8e: movs r2,#0x0
  000f6f90: blx 0x0006ee18
  000f6f94: ldr r0,[r5,#0x0]
  000f6f96: movw r1,#0x36f
  000f6f9a: blx 0x00072f70
  000f6f9e: mov r2,r0
  000f6fa0: sub.w r0,r7,#0x68
  000f6fa4: add r1,sp,#0x330
  000f6fa6: blx 0x0006ef98
  000f6faa: sub.w r0,r7,#0x38
  000f6fae: sub.w r1,r7,#0x68
  000f6fb2: blx 0x0006ef5c
  000f6fb6: sub.w r0,r7,#0x68
  000f6fba: blx 0x0006ee30
  000f6fbe: add r0,sp,#0x330
  000f6fc0: blx 0x0006ee30
  000f6fc4: ldr.w r6,[r9,#0x0]
  000f6fc8: add r0,sp,#0x198
  000f6fca: sub.w r1,r7,#0x44
  000f6fce: movs r2,#0x0
  000f6fd0: blx 0x0006f028
  000f6fd4: ldr r0,[0x000f7038]
  000f6fd6: ldr r1,[0x000f703c]
  000f6fd8: add r0,pc
  000f6fda: add r1,pc
  000f6fdc: ldr r0,[r0,#0x0]
  000f6fde: ldr r1,[r1,#0x0]
  000f6fe0: ldr r4,[r0,#0x0]
  000f6fe2: ldrb.w r0,[r1,#0x38]
  000f6fe6: cmp r0,#0x0
  000f6fe8: beq.w 0x000f7892
  000f6fec: mov r0,r10
  000f6fee: blx 0x00074bc0
  000f6ff2: vmov s0,r0
  000f6ff6: vcvt.f32.s32 s0,s0
  000f6ffa: b.w 0x000f78a4
  000f7040: ldr r0,[0x000f7398]
  000f7042: add r0,pc
  000f7044: ldr r0,[r0,#0x0]
  000f7046: ldr r6,[r0,#0x0]
  000f7048: add r0,sp,#0x3e4
  000f704a: sub.w r1,r7,#0x38
  000f704e: movs r2,#0x0
  000f7050: blx 0x0006f028
  000f7054: ldr.w r4,[r11,#0x0]
  000f7058: mov r0,r5
  000f705a: blx 0x000728a4
  000f705e: addw r1,r0,#0x32d
  000f7062: mov r0,r4
  000f7064: blx 0x00072f70
  000f7068: mov r1,r0
  000f706a: add r0,sp,#0x3d8
  000f706c: movs r2,#0x0
  000f706e: blx 0x0006f028
  000f7072: ldr r1,[0x000f739c]
  000f7074: add r1,pc
  000f7076: add r0,sp,#0x3cc
  000f7078: movs r2,#0x0
  000f707a: blx 0x0006ee18
  000f707e: str r0,[sp,#0x0]
  000f7080: sub.w r0,r7,#0x44
  000f7084: add r2,sp,#0x3e4
  000f7086: add r3,sp,#0x3d8
  000f7088: mov r1,r6
  000f708a: blx 0x000739b4
  000f708e: sub.w r0,r7,#0x38
  000f7092: sub.w r1,r7,#0x44
  000f7096: blx 0x0006f2b0
  000f709a: ldr r0,[0x000f73a0]
  000f709c: add r0,pc
  000f709e: ldr r4,[r0,#0x0]
  000f70a0: sub.w r0,r7,#0x44
  000f70a4: blx r4
  000f70a6: add r0,sp,#0x3cc
  000f70a8: blx r4
  000f70aa: add r0,sp,#0x3d8
  000f70ac: blx r4
  000f70ae: add r0,sp,#0x3e4
  000f70b0: blx r4
  000f70b2: ldr r0,[0x000f73a4]
  000f70b4: add r0,pc
  000f70b6: ldr r0,[r0,#0x0]
  000f70b8: str r0,[sp,#0x10]
  000f70ba: ldr r6,[r0,#0x0]
  000f70bc: add r0,sp,#0x3c0
  000f70be: sub.w r1,r7,#0x38
  000f70c2: movs r2,#0x0
  000f70c4: blx 0x0006f028
  000f70c8: mov r0,r5
  000f70ca: blx 0x00072bb0
  000f70ce: mov r1,r0
  000f70d0: sub.w r0,r7,#0x68
  000f70d4: blx 0x0006f658
  000f70d8: add r0,sp,#0x3b4
  000f70da: sub.w r1,r7,#0x68
  000f70de: movs r2,#0x0
  000f70e0: blx 0x0006f028
  000f70e4: ldr r1,[0x000f73a8]
  000f70e6: add r1,pc
  000f70e8: add r0,sp,#0x3a8
  000f70ea: movs r2,#0x0
  000f70ec: blx 0x0006ee18
  000f70f0: str r0,[sp,#0x0]
  000f70f2: sub.w r0,r7,#0x44
  000f70f6: add r2,sp,#0x3c0
  000f70f8: add r3,sp,#0x3b4
  000f70fa: mov r1,r6
  000f70fc: blx 0x000739b4
  000f7100: sub.w r0,r7,#0x38
  000f7104: sub.w r1,r7,#0x44
  000f7108: blx 0x0006f2b0
  000f710c: ldr r0,[0x000f73ac]
  000f710e: add r0,pc
  000f7110: ldr.w r9,[r0,#0x0]
  000f7114: sub.w r0,r7,#0x44
  000f7118: blx r9
  000f711a: add r0,sp,#0x3a8
  000f711c: blx r9
  000f711e: add r0,sp,#0x3b4
  000f7120: blx r9
  000f7122: sub.w r0,r7,#0x68
  000f7126: blx r9
  000f7128: add r0,sp,#0x3c0
  000f712a: blx r9
  000f712c: mov r0,r5
  000f712e: blx 0x00072874
  000f7132: cmp r0,#0xe
  000f7134: bne 0x000f719c
  000f7136: ldr r0,[sp,#0x10]
  000f7138: ldr r6,[r0,#0x0]
  000f713a: add r0,sp,#0x39c
  000f713c: sub.w r1,r7,#0x38
  000f7140: movs r2,#0x0
  000f7142: blx 0x0006f028
  000f7146: sub.w r0,r7,#0x68
  000f714a: mov r1,r5
  000f714c: blx 0x00074b60
  000f7150: add r0,sp,#0x390
  000f7152: sub.w r1,r7,#0x68
  000f7156: movs r2,#0x0
  000f7158: blx 0x0006f028
  000f715c: ldr r1,[0x000f73b0]
  000f715e: add r1,pc
  000f7160: add r0,sp,#0x384
  000f7162: movs r2,#0x0
  000f7164: blx 0x0006ee18
  000f7168: str r0,[sp,#0x0]
  000f716a: sub.w r0,r7,#0x44
  000f716e: add r2,sp,#0x39c
  000f7170: add r3,sp,#0x390
  000f7172: mov r1,r6
  000f7174: blx 0x000739b4
  000f7178: sub.w r0,r7,#0x38
  000f717c: sub.w r1,r7,#0x44
  000f7180: blx 0x0006f2b0
  000f7184: sub.w r0,r7,#0x44
  000f7188: blx r9
  000f718a: add r0,sp,#0x384
  000f718c: blx r9
  000f718e: add r0,sp,#0x390
  000f7190: blx r9
  000f7192: sub.w r0,r7,#0x68
  000f7196: blx r9
  000f7198: add r0,sp,#0x39c
  000f719a: b 0x000f7200
  000f719c: ldr r0,[sp,#0x10]
  000f719e: ldr r6,[r0,#0x0]
  000f71a0: add r0,sp,#0x378
  000f71a2: sub.w r1,r7,#0x38
  000f71a6: movs r2,#0x0
  000f71a8: blx 0x0006f028
  000f71ac: sub.w r0,r7,#0x68
  000f71b0: mov r1,r5
  000f71b2: blx 0x00074b54
  000f71b6: add r0,sp,#0x36c
  000f71b8: sub.w r1,r7,#0x68
  000f71bc: movs r2,#0x0
  000f71be: blx 0x0006f028
  000f71c2: ldr r1,[0x000f73b4]
  000f71c4: add r1,pc
  000f71c6: add r0,sp,#0x360
  000f71c8: movs r2,#0x0
  000f71ca: blx 0x0006ee18
  000f71ce: str r0,[sp,#0x0]
  000f71d0: sub.w r0,r7,#0x44
  000f71d4: add r2,sp,#0x378
  000f71d6: add r3,sp,#0x36c
  000f71d8: mov r1,r6
  000f71da: blx 0x000739b4
  000f71de: sub.w r0,r7,#0x38
  000f71e2: sub.w r1,r7,#0x44
  000f71e6: blx 0x0006f2b0
  000f71ea: sub.w r0,r7,#0x44
  000f71ee: blx r9
  000f71f0: add r0,sp,#0x360
  000f71f2: blx r9
  000f71f4: add r0,sp,#0x36c
  000f71f6: blx r9
  000f71f8: sub.w r0,r7,#0x68
  000f71fc: blx r9
  000f71fe: add r0,sp,#0x378
  000f7200: blx r9
  000f7202: ldr r0,[sp,#0x10]
  000f7204: ldr r6,[r0,#0x0]
  000f7206: add r0,sp,#0x354
  000f7208: sub.w r1,r7,#0x38
  000f720c: movs r2,#0x0
  000f720e: blx 0x0006f028
  000f7212: sub.w r0,r7,#0x68
  000f7216: mov r1,r5
  000f7218: blx 0x00074b48
  000f721c: add r0,sp,#0x348
  000f721e: sub.w r1,r7,#0x68
  000f7222: movs r2,#0x0
  000f7224: blx 0x0006f028
  000f7228: ldr r1,[0x000f73b8]
  000f722a: add r1,pc
  000f722c: add r0,sp,#0x33c
  000f722e: movs r2,#0x0
  000f7230: blx 0x0006ee18
  000f7234: str r0,[sp,#0x0]
  000f7236: sub.w r0,r7,#0x44
  000f723a: add r2,sp,#0x354
  000f723c: add r3,sp,#0x348
  000f723e: mov r1,r6
  000f7240: blx 0x000739b4
  000f7244: sub.w r0,r7,#0x38
  000f7248: sub.w r1,r7,#0x44
  000f724c: blx 0x0006f2b0
  000f7250: sub.w r0,r7,#0x44
  000f7254: blx r9
  000f7256: add r0,sp,#0x33c
  000f7258: blx r9
  000f725a: add r0,sp,#0x348
  000f725c: blx r9
  000f725e: sub.w r0,r7,#0x68
  000f7262: blx r9
  000f7264: add r0,sp,#0x354
  000f7266: blx r9
  000f7268: ldr.w r4,[r10,#0x28]
  000f726c: adds r0,r4,#0x1
  000f726e: bne 0x000f7282
  000f7270: ldr r0,[0x000f73bc]
  000f7272: add r0,pc
  000f7274: ldr r0,[r0,#0x0]
  000f7276: ldr r0,[r0,#0x0]
  000f7278: movs r1,#0x3
  000f727a: blx 0x00071848
  000f727e: add.w r4,r0,#0x2fc
  000f7282: ldr r1,[0x000f73c0]
  000f7284: str.w r4,[r10,#0x28]
  000f7288: add r1,pc
  000f728a: sub.w r0,r7,#0x68
  000f728e: movs r2,#0x0
  000f7290: blx 0x0006ee18
  000f7294: ldr.w r0,[r11,#0x0]
  000f7298: mov r1,r4
  000f729a: blx 0x00072f70
  000f729e: mov r2,r0
  000f72a0: sub.w r0,r7,#0x44
  000f72a4: sub.w r1,r7,#0x68
  000f72a8: blx 0x0006ef98
  000f72ac: sub.w r0,r7,#0x38
  000f72b0: sub.w r1,r7,#0x44
  000f72b4: blx 0x0006ef5c
  000f72b8: sub.w r0,r7,#0x44
  000f72bc: blx 0x0006ee30
  000f72c0: sub.w r0,r7,#0x68
  000f72c4: blx 0x0006ee30
  000f72c8: ldr r0,[sp,#0x10]
  000f72ca: ldr r0,[r0,#0x0]
  000f72cc: blx 0x00071b84
  000f72d0: mov r4,r0
  000f72d2: mov r0,r10
  000f72d4: blx 0x00071bd8
  000f72d8: mov r1,r0
  000f72da: mov r0,r4
  000f72dc: blx 0x00071c68
  000f72e0: vmov s16,r0
  000f72e4: vcmpe.f32 s16,#0
  000f72e8: vmrs apsr,fpscr
  000f72ec: ble 0x000f73d4
  000f72ee: ldr r1,[0x000f73c4]
  000f72f0: add r1,pc
  000f72f2: add r0,sp,#0x330
  000f72f4: movs r2,#0x0
  000f72f6: blx 0x0006ee18
  000f72fa: ldr r0,[sp,#0x10]
  000f72fc: ldr r6,[r0,#0x0]
  000f72fe: ldr.w r0,[r11,#0x0]
  000f7302: movw r1,#0x2ff
  000f7306: blx 0x00072f70
  000f730a: mov r1,r0
  000f730c: add r0,sp,#0x318
  000f730e: movs r2,#0x0
  000f7310: blx 0x0006f028
  000f7314: vldr.32 s0,[pc,#0xb0]
  000f7318: vmul.f32 s0,s16,s0
  000f731c: vcvt.s32.f32 s0,s0
  000f7320: vmov r1,s0
  000f7324: add r0,sp,#0x300
  000f7326: blx 0x0006f658
  000f732a: add r0,sp,#0x30c
  000f732c: add r1,sp,#0x300
  000f732e: movs r2,#0x0
  000f7330: blx 0x0006f028
  000f7334: ldr r1,[0x000f73cc]
  000f7336: add r1,pc
  000f7338: add r0,sp,#0x2f4
  000f733a: movs r2,#0x0
  000f733c: blx 0x0006ee18
  000f7340: str r0,[sp,#0x0]
  000f7342: add r0,sp,#0x324
  000f7344: add r2,sp,#0x318
  000f7346: add r3,sp,#0x30c
  000f7348: mov r1,r6
  000f734a: blx 0x000739b4
  000f734e: sub.w r0,r7,#0x68
  000f7352: add r1,sp,#0x330
  000f7354: add r2,sp,#0x324
  000f7356: blx 0x0006ef98
  000f735a: ldr r1,[0x000f73d0]
  000f735c: add r1,pc
  000f735e: add r0,sp,#0x2e8
  000f7360: movs r2,#0x0
  000f7362: blx 0x0006ee18
  000f7366: sub.w r0,r7,#0x44
  000f736a: sub.w r1,r7,#0x68
  000f736e: add r2,sp,#0x2e8
  000f7370: blx 0x0006ef98
  000f7374: add r0,sp,#0x2e8
  000f7376: blx r9
  000f7378: sub.w r0,r7,#0x68
  000f737c: blx r9
  000f737e: add r0,sp,#0x324
  000f7380: blx r9
  000f7382: add r0,sp,#0x2f4
  000f7384: blx r9
  000f7386: add r0,sp,#0x30c
  000f7388: blx r9
  000f738a: add r0,sp,#0x300
  000f738c: blx r9
  000f738e: add r0,sp,#0x318
  000f7390: blx r9
  000f7392: add r0,sp,#0x330
  000f7394: blx r9
  000f7396: b 0x000f73e2
  000f73d4: ldr r1,[0x000f76b0]
  000f73d6: add r1,pc
  000f73d8: sub.w r0,r7,#0x44
  000f73dc: movs r2,#0x0
  000f73de: blx 0x0006ee18
  000f73e2: mov r0,r10
  000f73e4: strd r11,r5,[sp,#0x8]
  000f73e8: blx 0x00071bb4
  000f73ec: blx 0x00071bc0
  000f73f0: vmov s0,r0
  000f73f4: movs r1,#0x32
  000f73f6: vcvt.f32.s32 s0,s0
  000f73fa: vmul.f32 s0,s16,s0
  000f73fe: vcvt.s32.f32 s0,s0
  000f7402: vmov r4,s0
  000f7406: mov r0,r4
  000f7408: blx 0x0006f514
  000f740c: mov r5,r1
  000f740e: mov r0,r10
  000f7410: blx 0x00071bb4
  000f7414: adds r6,r5,r4
  000f7416: mov r11,r0
  000f7418: movs r1,#0x32
  000f741a: mov r0,r6
  000f741c: blx 0x0006f514
  000f7420: cmp r1,#0x0
  000f7422: it ne
  000f7424: sub.ne r6,r4,r5
  000f7426: mov r0,r11
  000f7428: mov r1,r6
  000f742a: blx 0x00071cb0
  000f742e: ldr r0,[sp,#0x10]
  000f7430: ldr.w r11,[r0,#0x0]
  000f7434: add r0,sp,#0x2dc
  000f7436: sub.w r1,r7,#0x38
  000f743a: movs r2,#0x0
  000f743c: blx 0x0006f028
  000f7440: ldr r0,[0x000f76b4]
  000f7442: add r0,pc
  000f7444: ldr r0,[r0,#0x0]
  000f7446: ldr r4,[r0,#0x0]
  000f7448: ldr r6,[sp,#0xc]
  000f744a: mov r0,r6
  000f744c: blx 0x00071bc0
  000f7450: mov r5,r0
  000f7452: mov r0,r6
  000f7454: blx 0x00074b78
  000f7458: adds r2,r0,r5
  000f745a: add r0,sp,#0x324
  000f745c: mov r1,r4
  000f745e: blx 0x00074e54
  000f7462: add r0,sp,#0x330
  000f7464: add r1,sp,#0x324
  000f7466: movs r2,#0x0
  000f7468: blx 0x0006f028
  000f746c: add r0,sp,#0x2d0
  000f746e: add r1,sp,#0x330
  000f7470: sub.w r2,r7,#0x44
  000f7474: blx 0x0006ef98
  000f7478: ldr r1,[0x000f76b8]
  000f747a: add r1,pc
  000f747c: add r0,sp,#0x2c4
  000f747e: movs r2,#0x0
  000f7480: blx 0x0006ee18
  000f7484: str r0,[sp,#0x0]
  000f7486: sub.w r0,r7,#0x68
  000f748a: add r2,sp,#0x2dc
  000f748c: add r3,sp,#0x2d0
  000f748e: mov r1,r11
  000f7490: blx 0x000739b4
  000f7494: sub.w r0,r7,#0x38
  000f7498: sub.w r1,r7,#0x68
  000f749c: blx 0x0006f2b0
  000f74a0: sub.w r0,r7,#0x68
  000f74a4: blx r9
  000f74a6: add r0,sp,#0x2c4
  000f74a8: blx r9
  000f74aa: add r0,sp,#0x2d0
  000f74ac: blx r9
  000f74ae: add r0,sp,#0x330
  000f74b0: blx r9
  000f74b2: add r0,sp,#0x324
  000f74b4: blx r9
  000f74b6: add r0,sp,#0x2dc
  000f74b8: blx r9
  000f74ba: mov r0,r6
  000f74bc: blx 0x0007288c
  000f74c0: adds r0,#0x1
  000f74c2: bne 0x000f752c
  000f74c4: ldr r0,[sp,#0x8]
  000f74c6: ldr r0,[r0,#0x0]
  000f74c8: movw r1,#0x323
  000f74cc: blx 0x00072f70
  000f74d0: mov r1,r0
  000f74d2: sub.w r0,r7,#0x38
  000f74d6: blx 0x0006f2b0
  000f74da: ldr r0,[sp,#0x10]
  000f74dc: ldr r6,[r0,#0x0]
  000f74de: add r0,sp,#0x2b8
  000f74e0: sub.w r1,r7,#0x38
  000f74e4: movs r2,#0x0
  000f74e6: blx 0x0006f028
  000f74ea: ldr r1,[sp,#0xc]
  000f74ec: add r0,sp,#0x2ac
  000f74ee: blx 0x00074b54
  000f74f2: ldr r1,[0x000f76bc]
  000f74f4: add r1,pc
  000f74f6: add r0,sp,#0x2a0
  000f74f8: movs r2,#0x0
  000f74fa: blx 0x0006ee18
  000f74fe: str r0,[sp,#0x0]
  000f7500: sub.w r0,r7,#0x68
  000f7504: add r2,sp,#0x2b8
  000f7506: add r3,sp,#0x2ac
  000f7508: mov r1,r6
  000f750a: blx 0x000739b4
  000f750e: sub.w r0,r7,#0x38
  000f7512: sub.w r1,r7,#0x68
  000f7516: blx 0x0006f2b0
  000f751a: sub.w r0,r7,#0x68
  000f751e: blx r9
  000f7520: add r0,sp,#0x2a0
  000f7522: blx r9
  000f7524: add r0,sp,#0x2ac
  000f7526: blx r9
  000f7528: add r0,sp,#0x2b8
  000f752a: blx r9
  000f752c: sub.w r0,r7,#0x44
  000f7530: blx 0x0006ee30
  000f7534: b 0x000f75e2
  000f7536: movw r1,#0x36e
  000f753a: b 0x000f7548
  000f753c: movw r1,#0x372
  000f7540: cmp r6,#0x0
  000f7542: it eq
  000f7544: movw.eq r1,#0x371
  000f7548: ldr r0,[0x000f76c0]
  000f754a: add r0,pc
  000f754c: ldr r0,[r0,#0x0]
  000f754e: ldr r0,[r0,#0x0]
  000f7550: blx 0x00072f70
  000f7554: mov r1,r0
  000f7556: sub.w r0,r7,#0x38
  000f755a: blx 0x0006f2b0
  000f755e: ldr r6,[r5,#0x0]
  000f7560: sub.w r0,r7,#0x50
  000f7564: sub.w r1,r7,#0x38
  000f7568: movs r2,#0x0
  000f756a: blx 0x0006f028
  000f756e: ldr r0,[0x000f76c4]
  000f7570: vmov r2,s16
  000f7574: add r0,pc
  000f7576: ldr r0,[r0,#0x0]
  000f7578: ldr r1,[r0,#0x0]
  000f757a: sub.w r0,r7,#0x68
  000f757e: blx 0x00074e54
  000f7582: sub.w r0,r7,#0x5c
  000f7586: sub.w r1,r7,#0x68
  000f758a: movs r2,#0x0
  000f758c: blx 0x0006f028
  000f7590: ldr r1,[0x000f76c8]
  000f7592: add r1,pc
  000f7594: sub.w r0,r7,#0x74
  000f7598: movs r2,#0x0
  000f759a: blx 0x0006ee18
  000f759e: str r0,[sp,#0x0]
  000f75a0: sub.w r0,r7,#0x44
  000f75a4: sub.w r2,r7,#0x50
  000f75a8: sub.w r3,r7,#0x5c
  000f75ac: mov r1,r6
  000f75ae: blx 0x000739b4
  000f75b2: sub.w r0,r7,#0x38
  000f75b6: sub.w r1,r7,#0x44
  000f75ba: blx 0x0006f2b0
  000f75be: ldr r0,[0x000f76cc]
  000f75c0: add r0,pc
  000f75c2: ldr r4,[r0,#0x0]
  000f75c4: sub.w r0,r7,#0x44
  000f75c8: blx r4
  000f75ca: sub.w r0,r7,#0x74
  000f75ce: blx r4
  000f75d0: sub.w r0,r7,#0x5c
  000f75d4: blx r4
  000f75d6: sub.w r0,r7,#0x68
  000f75da: blx r4
  000f75dc: sub.w r0,r7,#0x50
  000f75e0: blx r4
  000f75e2: mov r0,r10
  000f75e4: blx 0x00071bb4
  000f75e8: ldr.w r11,[sp,#0x14]
  000f75ec: cmp r0,#0x0
  000f75ee: beq.w 0x000f7c0a
  000f75f2: mov r0,r10
  000f75f4: blx 0x00071bb4
  000f75f8: blx 0x00072874
  000f75fc: cmp r0,#0xc
  000f75fe: bne.w 0x000f7c0a
  000f7602: ldr r0,[0x000f76d0]
  000f7604: add r0,pc
  000f7606: ldr r0,[r0,#0x0]
  000f7608: ldr r0,[r0,#0x0]
  000f760a: movw r1,#0x31e
  000f760e: blx 0x00072f70
  000f7612: mov r1,r0
  000f7614: sub.w r0,r7,#0x38
  000f7618: blx 0x0006f2b0
  000f761c: ldr r0,[0x000f76d4]
  000f761e: add r0,pc
  000f7620: ldr r0,[r0,#0x0]
  000f7622: ldr r6,[r0,#0x0]
  000f7624: add r0,sp,#0x1bc
  000f7626: sub.w r1,r7,#0x38
  000f762a: movs r2,#0x0
  000f762c: blx 0x0006f028
  000f7630: ldr r0,[0x000f76d8]
  000f7632: add r0,pc
  000f7634: ldr r0,[r0,#0x0]
  000f7636: ldr r4,[r0,#0x0]
  000f7638: mov r0,r10
  000f763a: blx 0x00071bb4
  000f763e: blx 0x00071bc0
  000f7642: mov r5,r0
  000f7644: mov r0,r10
  000f7646: blx 0x00071bb4
  000f764a: blx 0x00074b78
  000f764e: adds r2,r0,r5
  000f7650: sub.w r0,r7,#0x68
  000f7654: mov r1,r4
  000f7656: blx 0x00074e54
  000f765a: add r0,sp,#0x1b0
  000f765c: sub.w r1,r7,#0x68
  000f7660: movs r2,#0x0
  000f7662: blx 0x0006f028
  000f7666: ldr r1,[0x000f76dc]
  000f7668: add r1,pc
  000f766a: add r0,sp,#0x1a4
  000f766c: movs r2,#0x0
  000f766e: blx 0x0006ee18
  000f7672: str r0,[sp,#0x0]
  000f7674: sub.w r0,r7,#0x44
  000f7678: add r2,sp,#0x1bc
  000f767a: add r3,sp,#0x1b0
  000f767c: mov r1,r6
  000f767e: blx 0x000739b4
  000f7682: sub.w r0,r7,#0x38
  000f7686: sub.w r1,r7,#0x44
  000f768a: blx 0x0006f2b0
  000f768e: ldr r0,[0x000f76e0]
  000f7690: add r0,pc
  000f7692: ldr r4,[r0,#0x0]
  000f7694: sub.w r0,r7,#0x44
  000f7698: blx r4
  000f769a: add r0,sp,#0x1a4
  000f769c: blx r4
  000f769e: add r0,sp,#0x1b0
  000f76a0: blx r4
  000f76a2: sub.w r0,r7,#0x68
  000f76a6: blx r4
  000f76a8: add r0,sp,#0x1bc
  000f76aa: blx r4
  000f76ac: b 0x000f7c0a
  000f76e4: mov r0,r10
  000f76e6: blx 0x00071ae8
  000f76ea: ldr.w r1,[r10,#0x28]
  000f76ee: cmp r0,#0x4
  000f76f0: bne.w 0x000f79a0
  000f76f4: adds r0,r1,#0x1
  000f76f6: bne 0x000f770a
  000f76f8: ldr r0,[0x000f7970]
  000f76fa: add r0,pc
  000f76fc: ldr r0,[r0,#0x0]
  000f76fe: ldr r0,[r0,#0x0]
  000f7700: movs r1,#0x2
  000f7702: blx 0x00071848
  000f7706: addw r1,r0,#0x36a
  000f770a: ldr r0,[0x000f7974]
  000f770c: str.w r1,[r10,#0x28]
  000f7710: add r0,pc
  000f7712: ldr r5,[r0,#0x0]
  000f7714: ldr r0,[r5,#0x0]
  000f7716: blx 0x00072f70
  000f771a: mov r1,r0
  000f771c: sub.w r0,r7,#0x38
  000f7720: blx 0x0006f2b0
  000f7724: ldr r1,[0x000f7978]
  000f7726: add r1,pc
  000f7728: add r0,sp,#0x330
  000f772a: movs r2,#0x0
  000f772c: blx 0x0006ee18
  000f7730: ldr r4,[r5,#0x0]
  000f7732: mov r0,r10
  000f7734: blx 0x00074ba8
  000f7738: addw r1,r0,#0x376
  000f773c: mov r0,r4
  000f773e: blx 0x00072f70
  000f7742: mov r2,r0
  000f7744: sub.w r0,r7,#0x68
  000f7748: add r1,sp,#0x330
  000f774a: blx 0x0006ef98
  000f774e: sub.w r0,r7,#0x38
  000f7752: sub.w r1,r7,#0x68
  000f7756: blx 0x0006ef5c
  000f775a: sub.w r0,r7,#0x68
  000f775e: blx 0x0006ee30
  000f7762: add r0,sp,#0x330
  000f7764: blx 0x0006ee30
  000f7768: ldr r1,[0x000f797c]
  000f776a: add r1,pc
  000f776c: add r0,sp,#0x330
  000f776e: movs r2,#0x0
  000f7770: blx 0x0006ee18
  000f7774: ldr r0,[r5,#0x0]
  000f7776: movw r1,#0x36d
  000f777a: blx 0x00072f70
  000f777e: mov r2,r0
  000f7780: sub.w r0,r7,#0x68
  000f7784: add r1,sp,#0x330
  000f7786: blx 0x0006ef98
  000f778a: sub.w r0,r7,#0x38
  000f778e: sub.w r1,r7,#0x68
  000f7792: blx 0x0006ef5c
  000f7796: sub.w r0,r7,#0x68
  000f779a: blx 0x0006ee30
  000f779e: add r0,sp,#0x330
  000f77a0: blx 0x0006ee30
  000f77a4: ldr r0,[0x000f7980]
  000f77a6: add r0,pc
  000f77a8: ldr r0,[r0,#0x0]
  000f77aa: ldr r4,[r0,#0x0]
  000f77ac: mov r0,r10
  000f77ae: blx 0x00074b84
  000f77b2: mov r1,r0
  000f77b4: mov r0,r4
  000f77b6: blx 0x00073684
  000f77ba: ldr.w r6,[r9,#0x0]
  000f77be: mov r4,r0
  000f77c0: add r0,sp,#0x9c
  000f77c2: sub.w r1,r7,#0x44
  000f77c6: movs r2,#0x0
  000f77c8: blx 0x0006f028
  000f77cc: add r0,sp,#0x330
  000f77ce: mov r1,r4
  000f77d0: blx 0x00071cc8
  000f77d4: add r0,sp,#0x90
  000f77d6: add r1,sp,#0x330
  000f77d8: movs r2,#0x0
  000f77da: blx 0x0006f028
  000f77de: ldr r1,[0x000f7984]
  000f77e0: add r1,pc
  000f77e2: add r0,sp,#0x84
  000f77e4: movs r2,#0x0
  000f77e6: blx 0x0006ee18
  000f77ea: str r0,[sp,#0x0]
  000f77ec: sub.w r0,r7,#0x68
  000f77f0: add r2,sp,#0x9c
  000f77f2: add r3,sp,#0x90
  000f77f4: mov r1,r6
  000f77f6: blx 0x000739b4
  000f77fa: sub.w r0,r7,#0x38
  000f77fe: sub.w r1,r7,#0x68
  000f7802: blx 0x0006f2b0
  000f7806: ldr r0,[0x000f7988]
  000f7808: add r0,pc
  000f780a: ldr r5,[r0,#0x0]
  000f780c: sub.w r0,r7,#0x68
  000f7810: blx r5
  000f7812: add r0,sp,#0x84
  000f7814: blx r5
  000f7816: add r0,sp,#0x90
  000f7818: blx r5
  000f781a: add r0,sp,#0x330
  000f781c: blx r5
  000f781e: add r0,sp,#0x9c
  000f7820: blx r5
  000f7822: ldr.w r6,[r9,#0x0]
  000f7826: add r0,sp,#0x78
  000f7828: sub.w r1,r7,#0x44
  000f782c: movs r2,#0x0
  000f782e: blx 0x0006f028
  000f7832: ldr r0,[0x000f798c]
  000f7834: add r0,pc
  000f7836: ldr r0,[r0,#0x0]
  000f7838: ldr r4,[r0,#0x0]
  000f783a: mov r0,r10
  000f783c: blx 0x00074bc0
  000f7840: mov r2,r0
  000f7842: add r0,sp,#0x330
  000f7844: mov r1,r4
  000f7846: blx 0x00074e54
  000f784a: add r0,sp,#0x6c
  000f784c: add r1,sp,#0x330
  000f784e: movs r2,#0x0
  000f7850: blx 0x0006f028
  000f7854: ldr r1,[0x000f7990]
  000f7856: add r1,pc
  000f7858: add r0,sp,#0x60
  000f785a: movs r2,#0x0
  000f785c: blx 0x0006ee18
  000f7860: str r0,[sp,#0x0]
  000f7862: sub.w r0,r7,#0x68
  000f7866: add r2,sp,#0x78
  000f7868: add r3,sp,#0x6c
  000f786a: mov r1,r6
  000f786c: blx 0x000739b4
  000f7870: sub.w r0,r7,#0x38
  000f7874: sub.w r1,r7,#0x68
  000f7878: blx 0x0006f2b0
  000f787c: sub.w r0,r7,#0x68
  000f7880: blx r5
  000f7882: add r0,sp,#0x60
  000f7884: blx r5
  000f7886: add r0,sp,#0x6c
  000f7888: blx r5
  000f788a: add r0,sp,#0x330
  000f788c: blx r5
  000f788e: add r0,sp,#0x78
  000f7890: b 0x000f7bae
  000f7892: mov r0,r10
  000f7894: blx 0x00074bc0
  000f7898: vmov s0,r0
  000f789c: vcvt.f32.s32 s0,s0
  000f78a0: vadd.f32 s0,s0,s0
  000f78a4: vcvt.s32.f32 s0,s0
  000f78a8: add r0,sp,#0x330
  000f78aa: mov r1,r4
  000f78ac: vmov r2,s0
  000f78b0: blx 0x00074e54
  000f78b4: add r0,sp,#0x18c
  000f78b6: add r1,sp,#0x330
  000f78b8: movs r2,#0x0
  000f78ba: blx 0x0006f028
  000f78be: ldr r1,[0x000f7994]
  000f78c0: add r1,pc
  000f78c2: add r0,sp,#0x180
  000f78c4: movs r2,#0x0
  000f78c6: blx 0x0006ee18
  000f78ca: str r0,[sp,#0x0]
  000f78cc: sub.w r0,r7,#0x68
  000f78d0: add r2,sp,#0x198
  000f78d2: add r3,sp,#0x18c
  000f78d4: mov r1,r6
  000f78d6: blx 0x000739b4
  000f78da: sub.w r0,r7,#0x38
  000f78de: sub.w r1,r7,#0x68
  000f78e2: blx 0x0006f2b0
  000f78e6: ldr r0,[0x000f7998]
  000f78e8: add r0,pc
  000f78ea: ldr r4,[r0,#0x0]
  000f78ec: sub.w r0,r7,#0x68
  000f78f0: blx r4
  000f78f2: add r0,sp,#0x180
  000f78f4: blx r4
  000f78f6: add r0,sp,#0x18c
  000f78f8: blx r4
  000f78fa: add r0,sp,#0x330
  000f78fc: blx r4
  000f78fe: add r0,sp,#0x198
  000f7900: blx r4
  000f7902: b 0x000f7bb0
  000f7904: mov r0,r10
  000f7906: blx 0x00074bc0
  000f790a: vmov s0,r0
  000f790e: vcvt.f32.s32 s0,s0
  000f7912: vadd.f32 s0,s0,s0
  000f7916: vcvt.s32.f32 s0,s0
  000f791a: add r0,sp,#0x330
  000f791c: mov r1,r4
  000f791e: vmov r2,s0
  000f7922: blx 0x00074e54
  000f7926: add r0,sp,#0x120
  000f7928: add r1,sp,#0x330
  000f792a: movs r2,#0x0
  000f792c: blx 0x0006f028
  000f7930: ldr r1,[0x000f799c]
  000f7932: add r1,pc
  000f7934: add r0,sp,#0x114
  000f7936: movs r2,#0x0
  000f7938: blx 0x0006ee18
  000f793c: str r0,[sp,#0x0]
  000f793e: sub.w r0,r7,#0x68
  000f7942: add r2,sp,#0x12c
  000f7944: add r3,sp,#0x120
  000f7946: mov r1,r6
  000f7948: blx 0x000739b4
  000f794c: sub.w r0,r7,#0x38
  000f7950: sub.w r1,r7,#0x68
  000f7954: blx 0x0006f2b0
  000f7958: sub.w r0,r7,#0x68
  000f795c: blx r5
  000f795e: add r0,sp,#0x114
  000f7960: blx r5
  000f7962: add r0,sp,#0x120
  000f7964: blx r5
  000f7966: add r0,sp,#0x330
  000f7968: blx r5
  000f796a: add r0,sp,#0x12c
  000f796c: b 0x000f7bae
  000f79a0: adds r0,r1,#0x1
  000f79a2: bne 0x000f79b6
  000f79a4: ldr r0,[0x000f7d4c]
  000f79a6: add r0,pc
  000f79a8: ldr r0,[r0,#0x0]
  000f79aa: ldr r0,[r0,#0x0]
  000f79ac: movs r1,#0x2
  000f79ae: blx 0x00071848
  000f79b2: addw r1,r0,#0x36a
  000f79b6: ldr r0,[0x000f7d50]
  000f79b8: str.w r1,[r10,#0x28]
  000f79bc: add r0,pc
  000f79be: ldr r5,[r0,#0x0]
  000f79c0: ldr r0,[r5,#0x0]
  000f79c2: blx 0x00072f70
  000f79c6: mov r1,r0
  000f79c8: sub.w r0,r7,#0x38
  000f79cc: blx 0x0006f2b0
  000f79d0: ldr r1,[0x000f7d54]
  000f79d2: add r1,pc
  000f79d4: add r0,sp,#0x330
  000f79d6: movs r2,#0x0
  000f79d8: blx 0x0006ee18
  000f79dc: ldr r4,[r5,#0x0]
  000f79de: mov r0,r10
  000f79e0: blx 0x00074ba8
  000f79e4: addw r1,r0,#0x376
  000f79e8: mov r0,r4
  000f79ea: blx 0x00072f70
  000f79ee: mov r2,r0
  000f79f0: sub.w r0,r7,#0x68
  000f79f4: add r1,sp,#0x330
  000f79f6: blx 0x0006ef98
  000f79fa: sub.w r0,r7,#0x38
  000f79fe: sub.w r1,r7,#0x68
  000f7a02: blx 0x0006ef5c
  000f7a06: sub.w r0,r7,#0x68
  000f7a0a: blx 0x0006ee30
  000f7a0e: add r0,sp,#0x330
  000f7a10: blx 0x0006ee30
  000f7a14: ldr r1,[0x000f7d58]
  000f7a16: add r1,pc
  000f7a18: add r0,sp,#0x330
  000f7a1a: movs r2,#0x0
  000f7a1c: blx 0x0006ee18
  000f7a20: ldr r0,[r5,#0x0]
  000f7a22: mov.w r1,#0x36c
  000f7a26: blx 0x00072f70
  000f7a2a: mov r2,r0
  000f7a2c: sub.w r0,r7,#0x68
  000f7a30: add r1,sp,#0x330
  000f7a32: blx 0x0006ef98
  000f7a36: sub.w r0,r7,#0x38
  000f7a3a: sub.w r1,r7,#0x68
  000f7a3e: str.w r11,[sp,#0x14]
  000f7a42: blx 0x0006ef5c
  000f7a46: sub.w r0,r7,#0x68
  000f7a4a: blx 0x0006ee30
  000f7a4e: add r0,sp,#0x330
  000f7a50: blx 0x0006ee30
  000f7a54: ldr.w r11,[r9,#0x0]
  000f7a58: add r0,sp,#0x54
  000f7a5a: sub.w r1,r7,#0x44
  000f7a5e: movs r2,#0x0
  000f7a60: blx 0x0006f028
  000f7a64: ldr r0,[0x000f7d5c]
  000f7a66: ldr r4,[r5,#0x0]
  000f7a68: add r0,pc
  000f7a6a: ldr r0,[r0,#0x0]
  000f7a6c: ldr r6,[r0,#0x0]
  000f7a6e: mov r0,r10
  000f7a70: blx 0x00074b90
  000f7a74: ldr r1,[r6,#0x4]
  000f7a76: ldr.w r0,[r1,r0,lsl #0x2]
  000f7a7a: blx 0x000718d8
  000f7a7e: addw r1,r0,#0x4fa
  000f7a82: mov r0,r4
  000f7a84: blx 0x00072f70
  000f7a88: mov r1,r0
  000f7a8a: add r0,sp,#0x48
  000f7a8c: movs r2,#0x0
  000f7a8e: blx 0x0006f028
  000f7a92: ldr r1,[0x000f7d60]
  000f7a94: add r1,pc
  000f7a96: add r0,sp,#0x3c
  000f7a98: movs r2,#0x0
  000f7a9a: blx 0x0006ee18
  000f7a9e: str r0,[sp,#0x0]
  000f7aa0: sub.w r0,r7,#0x68
  000f7aa4: add r2,sp,#0x54
  000f7aa6: add r3,sp,#0x48
  000f7aa8: mov r1,r11
  000f7aaa: blx 0x000739b4
  000f7aae: sub.w r0,r7,#0x38
  000f7ab2: sub.w r1,r7,#0x68
  000f7ab6: blx 0x0006f2b0
  000f7aba: ldr.w r11,[sp,#0x14]
  000f7abe: ldr r0,[0x000f7d64]
  000f7ac0: add r0,pc
  000f7ac2: ldr r5,[r0,#0x0]
  000f7ac4: sub.w r0,r7,#0x68
  000f7ac8: blx r5
  000f7aca: add r0,sp,#0x3c
  000f7acc: blx r5
  000f7ace: add r0,sp,#0x48
  000f7ad0: blx r5
  000f7ad2: add r0,sp,#0x54
  000f7ad4: blx r5
  000f7ad6: ldr.w r6,[r9,#0x0]
  000f7ada: add r0,sp,#0x30
  000f7adc: sub.w r1,r7,#0x44
  000f7ae0: movs r2,#0x0
  000f7ae2: blx 0x0006f028
  000f7ae6: ldr r0,[0x000f7d68]
  000f7ae8: add r0,pc
  000f7aea: ldr r0,[r0,#0x0]
  000f7aec: ldr r4,[r0,#0x0]
  000f7aee: mov r0,r10
  000f7af0: blx 0x00074bc0
  000f7af4: mov r2,r0
  000f7af6: add r0,sp,#0x330
  000f7af8: mov r1,r4
  000f7afa: blx 0x00074e54
  000f7afe: add r0,sp,#0x24
  000f7b00: add r1,sp,#0x330
  000f7b02: movs r2,#0x0
  000f7b04: blx 0x0006f028
  000f7b08: ldr r1,[0x000f7d6c]
  000f7b0a: add r1,pc
  000f7b0c: add r0,sp,#0x18
  000f7b0e: movs r2,#0x0
  000f7b10: blx 0x0006ee18
  000f7b14: str r0,[sp,#0x0]
  000f7b16: sub.w r0,r7,#0x68
  000f7b1a: add r2,sp,#0x30
  000f7b1c: add r3,sp,#0x24
  000f7b1e: mov r1,r6
  000f7b20: blx 0x000739b4
  000f7b24: sub.w r0,r7,#0x38
  000f7b28: sub.w r1,r7,#0x68
  000f7b2c: blx 0x0006f2b0
  000f7b30: sub.w r0,r7,#0x68
  000f7b34: blx r5
  000f7b36: add r0,sp,#0x18
  000f7b38: blx r5
  000f7b3a: add r0,sp,#0x24
  000f7b3c: blx r5
  000f7b3e: add r0,sp,#0x330
  000f7b40: blx r5
  000f7b42: add r0,sp,#0x30
  000f7b44: b 0x000f7bae
  000f7b46: mov r0,r10
  000f7b48: blx 0x00074bc0
  000f7b4c: vmov s0,r0
  000f7b50: vcvt.f32.s32 s0,s0
  000f7b54: vadd.f32 s0,s0,s0
  000f7b58: vcvt.s32.f32 s0,s0
  000f7b5c: add r0,sp,#0x330
  000f7b5e: mov r1,r4
  000f7b60: vmov r2,s0
  000f7b64: blx 0x00074e54
  000f7b68: add r0,sp,#0xb4
  000f7b6a: add r1,sp,#0x330
  000f7b6c: movs r2,#0x0
  000f7b6e: blx 0x0006f028
  000f7b72: ldr r1,[0x000f7d70]
  000f7b74: add r1,pc
  000f7b76: add r0,sp,#0xa8
  000f7b78: movs r2,#0x0
  000f7b7a: blx 0x0006ee18
  000f7b7e: str r0,[sp,#0x0]
  000f7b80: sub.w r0,r7,#0x68
  000f7b84: add r2,sp,#0xc0
  000f7b86: add r3,sp,#0xb4
  000f7b88: mov r1,r6
  000f7b8a: blx 0x000739b4
  000f7b8e: sub.w r0,r7,#0x38
  000f7b92: sub.w r1,r7,#0x68
  000f7b96: blx 0x0006f2b0
  000f7b9a: sub.w r0,r7,#0x68
  000f7b9e: blx r5
  000f7ba0: add r0,sp,#0xa8
  000f7ba2: blx r5
  000f7ba4: add r0,sp,#0xb4
  000f7ba6: blx r5
  000f7ba8: add r0,sp,#0x330
  000f7baa: blx r5
  000f7bac: add r0,sp,#0xc0
  000f7bae: blx r5
  000f7bb0: ldr r1,[0x000f7d74]
  000f7bb2: add r1,pc
  000f7bb4: add r0,sp,#0x330
  000f7bb6: movs r2,#0x0
  000f7bb8: blx 0x0006ee18
  000f7bbc: ldr r0,[0x000f7d78]
  000f7bbe: ldr r1,[0x000f7d7c]
  000f7bc0: add r0,pc
  000f7bc2: add r1,pc
  000f7bc4: ldr r0,[r0,#0x0]
  000f7bc6: ldr r1,[r1,#0x0]
  000f7bc8: ldr r4,[r0,#0x0]
  000f7bca: ldr r0,[r1,#0x0]
  000f7bcc: movs r1,#0x3
  000f7bce: blx 0x00071848
  000f7bd2: addw r1,r0,#0x349
  000f7bd6: mov r0,r4
  000f7bd8: blx 0x00072f70
  000f7bdc: mov r2,r0
  000f7bde: sub.w r0,r7,#0x68
  000f7be2: add r1,sp,#0x330
  000f7be4: blx 0x0006ef98
  000f7be8: sub.w r0,r7,#0x38
  000f7bec: sub.w r1,r7,#0x68
  000f7bf0: blx 0x0006ef5c
  000f7bf4: sub.w r0,r7,#0x68
  000f7bf8: blx 0x0006ee30
  000f7bfc: add r0,sp,#0x330
  000f7bfe: blx 0x0006ee30
  000f7c02: sub.w r0,r7,#0x44
  000f7c06: blx 0x0006ee30
  000f7c0a: sub.w r1,r7,#0x38
  000f7c0e: mov r0,r11
  000f7c10: movs r2,#0x0
  000f7c12: blx 0x0006f028
  000f7c16: sub.w r0,r7,#0x38
  000f7c1a: blx 0x0006ee30
  000f7c1e: ldr.w r0,[r7,#-0x2c]
  000f7c22: ldr.w r1,[r8,#0x0]
  000f7c26: subs r0,r1,r0
  000f7c28: itttt eq
  000f7c2a: add.eq.w sp,sp,#0x538
  000f7c2e: vpop.eq {d8}
  000f7c32: add.eq sp,#0x4
  000f7c34: pop.eq.w {r8,r9,r10,r11}
  000f7c38: it eq
  000f7c3a: pop.eq {r4,r5,r6,r7,pc}
  000f7c3c: blx 0x0006e824
```
