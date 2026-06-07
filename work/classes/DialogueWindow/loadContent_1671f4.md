# DialogueWindow::loadContent
elf 0x1671f4 body 2772
Sig: undefined __thiscall loadContent(DialogueWindow * this)

## decompile
```c

/* DialogueWindow::loadContent() */

void __thiscall DialogueWindow::loadContent(DialogueWindow *this)

{
  String *pSVar1;
  undefined4 *puVar2;
  void *pvVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  Standing *this_00;
  String *pSVar7;
  DialogueWindow *pDVar8;
  String *this_01;
  Agent *pAVar9;
  Vector *pVVar10;
  int *piVar11;
  int iVar12;
  TouchButton *pTVar13;
  undefined4 uVar14;
  uint32_t *this_02;
  uint uVar15;
  code *pcVar16;
  ScrollTouchWindow *pSVar17;
  Globals *this_03;
  int *piVar18;
  AEString *pAVar19;
  int *piVar20;
  int *piVar21;
  bool bVar22;
  float fVar23;
  float extraout_s0;
  String aSStack_190 [12];
  String aSStack_184 [12];
  String aSStack_178 [12];
  String aSStack_16c [12];
  String aSStack_160 [12];
  String aSStack_154 [12];
  String aSStack_148 [12];
  undefined1 auStack_13c [12];
  String aSStack_130 [12];
  undefined1 auStack_124 [12];
  String aSStack_118 [12];
  String aSStack_10c [12];
  String aSStack_100 [12];
  String aSStack_f4 [12];
  String aSStack_e8 [12];
  String aSStack_dc [12];
  undefined1 auStack_d0 [12];
  String aSStack_c4 [12];
  undefined1 auStack_b8 [12];
  String aSStack_ac [12];
  String aSStack_a0 [12];
  undefined1 auStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar21 = *(int **)(DAT_001775a4 + 0x177208);
  piVar18 = *(int **)(DAT_001775a8 + 0x17720a);
  local_28 = *piVar21;
  pTVar13 = (TouchButton *)this->field_4;
  pSVar1 = (String *)GameText::getText(*piVar18);
  TouchButton::replaceTextKeepSize(pTVar13,pSVar1);
  puVar2 = (undefined4 *)(DAT_001775ac + 0x177230);
  this->m_bFlipImage = 0;
  this[1].field_0 = (int *)0x0;
  piVar20 = (int *)*puVar2;
  fVar23 = (float)FModSound::stop(*piVar20);
  this->field_64 = -1;
  TouchButton::setPressProgress((TouchButton *)this->field_4,fVar23);
  if ((Array *)this->field_C != (Array *)0x0) {
    ArrayReleaseClasses<ImagePart*>((Array *)this->field_C);
    if ((Array<ImagePart*> *)this->field_C != (Array<ImagePart*> *)0x0) {
      pvVar3 = (void *)Array<ImagePart*>::~Array((Array<ImagePart*> *)this->field_C);
      operator_delete(pvVar3);
    }
  }
  this->field_C = (int *)0x0;
  iVar4 = Mission::isCampaignMission((Mission *)this->field_4C);
  piVar11 = this->field_44;
  if (iVar4 == 0) {
    iVar4 = 0;
    if (piVar11 == (int *)0x2) {
      iVar4 = this->field_10;
    }
    if (piVar11 != (int *)0x2 || iVar4 != 0x2a) {
      bVar22 = ((uint)this->field_48 & 1) != 0;
      if (bVar22) {
        this->field_24 = (int *)(DAT_001775b4 + 0x1772d4);
        pSVar1 = (String *)GameText::getText(*piVar18);
        AbyssEngine::String::operator=((String *)&(this->m_pAgentName).size,pSVar1);
      }
      else {
        piVar11 = (int *)Mission::getClientImage((Mission *)this->field_4C);
        this->field_24 = piVar11;
        Mission::getClientName();
        AbyssEngine::String::operator=((String *)&(this->m_pAgentName).size,aSStack_34);
        AbyssEngine::String::~String(aSStack_34);
      }
      piVar11 = this->field_44;
      *(bool *)&this[1].field_4 = bVar22;
      if (piVar11 == (int *)0x1) {
        iVar4 = Mission::getAgent((Mission *)this->field_4C);
        if (iVar4 == 0) {
          iVar4 = 0;
        }
        else {
          pAVar9 = (Agent *)Mission::getAgent((Mission *)this->field_4C);
          iVar4 = Agent::isStoryAgent(pAVar9);
        }
        iVar12 = Mission::getType();
        if ((iVar12 == 5) || (iVar12 = Mission::getType(), iVar12 == 3)) {
          pSVar1 = (String *)GameText::getText(*piVar18);
          pAVar19 = &this->m_pText;
          AbyssEngine::String::operator=((String *)pAVar19,pSVar1);
          uVar14 = **(undefined4 **)(DAT_0017799c + 0x1776ba);
          AbyssEngine::String::String(aSStack_88,(String *)pAVar19,false);
          Mission::getTargetStationName();
          uVar5 = AbyssEngine::String::String(aSStack_a0,(char *)(DAT_001779a0 + 0x1776d0),false);
          Status::replaceHash(aSStack_34,uVar14,aSStack_88,auStack_94,uVar5);
          AbyssEngine::String::operator=((String *)pAVar19,aSStack_34);
          pcVar16 = *(code **)(DAT_001779a4 + 0x1776f4);
          (*pcVar16)(aSStack_34);
          (*pcVar16)(aSStack_a0);
          (*pcVar16)(auStack_94);
          (*pcVar16)(aSStack_88);
          iVar12 = 0x185;
        }
        else {
          iVar12 = Mission::getType();
          if (iVar12 == 0xc) {
            pSVar1 = (String *)GameText::getText(*piVar18);
            if (iVar4 == 0) {
              AbyssEngine::String::String(aSStack_40,(char *)(DAT_00177dc0 + 0x177b40),false);
            }
            else {
              AbyssEngine::String::String(aSStack_4c,(char *)(DAT_001779c4 + 0x177852),false);
              uVar14 = **(undefined4 **)(DAT_001779c8 + 0x17785a);
              pSVar7 = (String *)GameText::getText(*piVar18);
              AbyssEngine::String::String(aSStack_ac,pSVar7,false);
              Mission::getAgent((Mission *)this->field_4C);
              Agent::getStationName();
              uVar5 = AbyssEngine::String::String
                                (aSStack_c4,(char *)(DAT_001779cc + 0x177884),false);
              Status::replaceHash(aSStack_58,uVar14,aSStack_ac,auStack_b8,uVar5);
              AbyssEngine::operator+(aSStack_40,aSStack_4c);
            }
            AbyssEngine::operator+(aSStack_34,pSVar1);
            pAVar19 = &this->m_pText;
            AbyssEngine::String::operator=((String *)pAVar19,aSStack_34);
            AbyssEngine::String::~String(aSStack_34);
            AbyssEngine::String::~String(aSStack_40);
            if (iVar4 != 0) {
              pcVar16 = *(code **)(DAT_00177dc4 + 0x177b70);
              (*pcVar16)(aSStack_58);
              (*pcVar16)(aSStack_c4);
              (*pcVar16)(auStack_b8);
              (*pcVar16)(aSStack_ac);
              (*pcVar16)(aSStack_4c);
            }
            uVar14 = **(undefined4 **)(DAT_00177dc8 + 0x177b8e);
            AbyssEngine::String::String(aSStack_dc,(String *)pAVar19,false);
            AbyssEngine::String::String(aSStack_e8,this->field_58[9]);
            uVar5 = AbyssEngine::String::String(aSStack_f4,(char *)(DAT_00177dcc + 0x177ba6),false);
            Status::replaceHash(auStack_d0,uVar14,aSStack_dc,aSStack_e8,uVar5);
            AbyssEngine::String::String(aSStack_100,this->field_58[8]);
            uVar5 = AbyssEngine::String::String(aSStack_10c,(char *)(DAT_00177dd0 + 0x177bca),false)
            ;
            Status::replaceHash(aSStack_34,uVar14,auStack_d0,aSStack_100,uVar5);
            AbyssEngine::String::operator=((String *)pAVar19,aSStack_34);
            pcVar16 = *(code **)(DAT_00177dd4 + 0x177bee);
            (*pcVar16)(aSStack_34);
            (*pcVar16)(aSStack_10c);
            (*pcVar16)(aSStack_100);
            (*pcVar16)(auStack_d0);
            (*pcVar16)(aSStack_f4);
            (*pcVar16)(aSStack_e8);
            (*pcVar16)(aSStack_dc);
            iVar12 = 0x172;
          }
          else {
            iVar12 = Mission::getType();
            if (iVar12 == 0xb7) {
              iVar4 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_001779d0 + 0x1778e4));
              iVar12 = *(int *)(DAT_001779d4 + 0x1778f2 + iVar4 * 4);
              pSVar1 = (String *)GameText::getText(*piVar18);
              pAVar19 = &this->m_pText;
              AbyssEngine::String::operator=((String *)pAVar19,pSVar1);
              AbyssEngine::String::String(aSStack_58,(char *)(DAT_001779d8 + 0x177912),false);
              GameText::getText(*piVar18);
              AbyssEngine::operator+(aSStack_4c,aSStack_58);
              AbyssEngine::String::String(aSStack_64,(char *)(DAT_001779dc + 0x17792c),false);
              AbyssEngine::operator+(aSStack_40,aSStack_4c);
              AbyssEngine::String::String(aSStack_70,this->field_58[9]);
              AbyssEngine::operator+(aSStack_34,aSStack_40);
              pcVar16 = *(code **)(DAT_001779e0 + 0x177956);
              (*pcVar16)(aSStack_70);
              (*pcVar16)(aSStack_40);
              (*pcVar16)(aSStack_64);
              (*pcVar16)(aSStack_4c);
              (*pcVar16)(aSStack_58);
              AbyssEngine::operator+(aSStack_40,(String *)pAVar19);
              AbyssEngine::String::operator=((String *)pAVar19,aSStack_40);
              AbyssEngine::String::~String(aSStack_40);
              AbyssEngine::String::~String(aSStack_34);
            }
            else {
              iVar12 = GameText::getLanguage();
              if ((iVar12 == 1) &&
                 (iVar12 = Mission::getAgent((Mission *)this->field_4C), iVar12 != 0)) {
                piVar11 = this->field_44;
                pDVar8 = (DialogueWindow *)Mission::getAgent((Mission *)this->field_4C);
                iVar12 = pickGermanGenericTextBecauseWeSaved100EurosWithThat
                                   (pDVar8,(int)piVar11,(Agent *)pDVar8);
              }
              else {
                iVar12 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00177f78 + 0x177c3c));
                iVar12 = *(int *)(DAT_00177f7c + 0x177c48 + iVar12 * 4);
              }
              pSVar1 = (String *)GameText::getText(*piVar18);
              pAVar19 = &this->m_pText;
              AbyssEngine::String::operator=((String *)pAVar19,pSVar1);
              if (iVar4 == 0) {
                AbyssEngine::String::String(aSStack_40,(char *)(DAT_00177f80 + 0x177c68),false);
                GameText::getText(*piVar18);
                AbyssEngine::operator+(aSStack_34,aSStack_40);
                AbyssEngine::String::~String(aSStack_40);
              }
              else {
                AbyssEngine::String::String(aSStack_40,(char *)(DAT_00177f80 + 0x177c68),false);
                uVar14 = **(undefined4 **)(DAT_00177f84 + 0x177c76);
                pSVar1 = (String *)GameText::getText(*piVar18);
                AbyssEngine::String::String(aSStack_118,pSVar1,false);
                Mission::getAgent((Mission *)this->field_4C);
                Agent::getStationName();
                uVar5 = AbyssEngine::String::String
                                  (aSStack_130,(char *)(DAT_00177f88 + 0x177ca0),false);
                Status::replaceHash(aSStack_4c,uVar14,aSStack_118,auStack_124,uVar5);
                AbyssEngine::operator+(aSStack_34,aSStack_40);
                pcVar16 = *(code **)(DAT_00177f8c + 0x177cc4);
                (*pcVar16)(aSStack_4c);
                (*pcVar16)(aSStack_130);
                (*pcVar16)(auStack_124);
                (*pcVar16)(aSStack_118);
                (*pcVar16)(aSStack_40);
              }
              AbyssEngine::operator+(aSStack_40,(String *)pAVar19);
              AbyssEngine::String::operator=((String *)pAVar19,aSStack_40);
              AbyssEngine::String::~String(aSStack_40);
              AbyssEngine::String::~String(aSStack_34);
            }
          }
        }
        this_00 = (Standing *)Status::getStanding();
        iVar4 = Mission::getClientRace((Mission *)this->field_4C);
        Standing::applyMissionCompleted(this_00,iVar4);
        iVar4 = Mission::getTargetStation();
        if (iVar4 == 0x6c) {
          iVar4 = *piVar18;
          iVar12 = 0x1ca;
          goto LAB_001779fe;
        }
      }
      else if (piVar11 == (int *)0x0) {
        iVar4 = Mission::getType();
        if (iVar4 == 0xc) {
          iVar4 = *piVar18;
          iVar12 = 0x174;
LAB_001779fe:
          pSVar1 = (String *)GameText::getText(iVar4);
          this_01 = (String *)&this->m_pText;
        }
        else {
          iVar4 = Mission::getType();
          if (iVar4 == 7) {
            iVar4 = *piVar18;
            iVar12 = 0x17a;
            goto LAB_001779fe;
          }
          if ((this->field_4C == 0) || (iVar4 = Mission::getTargetStation(), iVar4 != 0x6c)) {
            iVar4 = GameText::getLanguage();
            if ((iVar4 == 1) && (iVar4 = Mission::getAgent((Mission *)this->field_4C), iVar4 != 0))
            {
              piVar11 = this->field_44;
              pDVar8 = (DialogueWindow *)Mission::getAgent((Mission *)this->field_4C);
              iVar12 = pickGermanGenericTextBecauseWeSaved100EurosWithThat
                                 (pDVar8,(int)piVar11,(Agent *)pDVar8);
            }
            else {
              iVar4 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_00177da0 + 0x1779ec));
              iVar12 = *(int *)(DAT_00177da4 + 0x1779f8 + iVar4 * 4);
            }
            iVar4 = *piVar18;
            goto LAB_001779fe;
          }
          if (*(int *)(**(int **)(DAT_001779bc + 0x177814) + 0x114) == 2) {
            iVar4 = DAT_001779c0 + 0x17782a;
          }
          else {
            iVar4 = DAT_00177f74 + 0x177ce2;
          }
          iVar12 = *(int *)(iVar4 + ((int)this->field_48 << 1 | 1U) * 4);
          pSVar1 = (String *)GameText::getText(*piVar18);
          AbyssEngine::String::operator=((String *)&this->m_pText,pSVar1);
          this->field_24 =
               *(int **)(DAT_00177f70 + 0x177d08 + *(int *)(iVar4 + (int)this->field_48 * 8) * 4);
          pSVar1 = (String *)GameText::getText(*piVar18);
          this_01 = (String *)&(this->m_pAgentName).size;
        }
        AbyssEngine::String::operator=(this_01,pSVar1);
      }
      else {
        iVar4 = Mission::getType();
        if (iVar4 == 0xc) {
          uVar14 = **(undefined4 **)(DAT_001775bc + 0x177354);
          pSVar1 = (String *)GameText::getText(*piVar18);
          AbyssEngine::String::String(aSStack_148,pSVar1,false);
          AbyssEngine::String::String(aSStack_154,this->field_58[9]);
          uVar5 = AbyssEngine::String::String(aSStack_160,(char *)(DAT_001775c0 + 0x177376),false);
          Status::replaceHash(auStack_13c,uVar14,aSStack_148,aSStack_154,uVar5);
          AbyssEngine::String::String(aSStack_16c,this->field_58[8]);
          uVar5 = AbyssEngine::String::String(aSStack_178,(char *)(DAT_001775c4 + 0x17739a),false);
          Status::replaceHash(aSStack_34,uVar14,auStack_13c,aSStack_16c,uVar5);
          AbyssEngine::String::operator=((String *)&this->m_pText,aSStack_34);
          pcVar16 = *(code **)(DAT_001775c8 + 0x1773c0);
          (*pcVar16)(aSStack_34);
          (*pcVar16)(aSStack_178);
          (*pcVar16)(aSStack_16c);
          (*pcVar16)(auStack_13c);
          (*pcVar16)(aSStack_160);
          (*pcVar16)(aSStack_154);
          (*pcVar16)(aSStack_148);
          iVar4 = -1;
          iVar12 = 0x173;
          goto LAB_00177a12;
        }
        iVar4 = GameText::getLanguage();
        if ((iVar4 == 1) && (iVar4 = Mission::getAgent((Mission *)this->field_4C), iVar4 != 0)) {
          piVar11 = this->field_44;
          pDVar8 = (DialogueWindow *)Mission::getAgent((Mission *)this->field_4C);
          iVar12 = pickGermanGenericTextBecauseWeSaved100EurosWithThat
                             (pDVar8,(int)piVar11,(Agent *)pDVar8);
        }
        else {
          iVar4 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_001779ac + 0x177748));
          iVar12 = *(int *)(DAT_001779b0 + 0x177754 + iVar4 * 4);
        }
        pSVar1 = (String *)GameText::getText(*piVar18);
        AbyssEngine::String::String(aSStack_4c,(char *)(DAT_001779b4 + 0x17776c),false);
        AbyssEngine::operator+(aSStack_40,pSVar1);
        GameText::getText(*piVar18);
        AbyssEngine::operator+(aSStack_34,aSStack_40);
        AbyssEngine::String::operator=((String *)&this->m_pText,aSStack_34);
        pcVar16 = *(code **)(DAT_001779b8 + 0x1777a2);
        (*pcVar16)(aSStack_34);
        (*pcVar16)(aSStack_40);
        (*pcVar16)(aSStack_4c);
      }
      iVar4 = -1;
      goto LAB_00177a12;
    }
LAB_001773ea:
    iVar4 = 0x10;
  }
  else {
    if (piVar11 == (int *)0x1) {
      piVar11 = this->field_48;
      iVar4 = *(int *)(this->field_60 + this->field_10 * 4);
      iVar12 = DAT_001775b8 + 0x17731c;
    }
    else {
      if (piVar11 != (int *)0x0) goto LAB_001773ea;
      piVar11 = this->field_48;
      iVar4 = *(int *)(this->field_5C + this->field_10 * 4);
      iVar12 = DAT_001775b0 + 0x177290;
    }
    iVar4 = *(int *)(iVar12 + (iVar4 + (int)piVar11 * 2) * 4);
  }
  this->field_24 = *(int **)(DAT_001775cc + 0x1773fa + iVar4 * 4);
  pSVar1 = (String *)GameText::getText(*piVar18);
  this_02 = &(this->m_pAgentName).size;
  AbyssEngine::String::operator=((String *)this_02,pSVar1);
  piVar11 = this->field_44;
  *(bool *)&this[1].field_4 = iVar4 == 0;
  if (piVar11 == (int *)0x1) {
    iVar12 = *(int *)(DAT_001775dc + 0x1774a4 +
                     (((int)this->field_48 << 1 | 1U) +
                     *(int *)(this->field_60 + this->field_10 * 4)) * 4);
    pSVar1 = (String *)GameText::getText(*piVar18);
    pAVar19 = &this->m_pText;
    AbyssEngine::String::operator=((String *)pAVar19,pSVar1);
    iVar6 = 0x6b6;
    if (iVar12 != 0x6b6) {
      iVar6 = 0x6c2;
    }
    if (iVar12 != 0x6b6 && iVar12 != iVar6) {
      if (iVar12 == 0x729) {
        fVar23 = (float)FModSound::stop(*piVar20);
        fVar23 = (float)FModSound::play(*piVar20,(Vector *)0x88,(Vector *)0x0,fVar23);
        FModSound::play(*piVar20,(Vector *)0xa2,(Vector *)0x0,fVar23);
        iVar12 = 0x729;
      }
      else {
        iVar6 = GameText::getLanguage();
        if (iVar12 == 0xade && iVar6 == 1) {
          this->field_24 = (int *)(DAT_00177dbc + 0x177b10);
          pSVar1 = (String *)GameText::getText(*piVar18);
          AbyssEngine::String::operator=((String *)this_02,pSVar1);
          *(bool *)&this[1].field_4 = iVar4 == 0;
          iVar12 = 0xade;
        }
        else if ((iVar12 - 0x891U < 2) && (iVar6 = Mission::getStatusValue(), iVar6 == 1)) {
          iVar12 = iVar12 + -2;
          pSVar1 = (String *)GameText::getText(*piVar18);
          AbyssEngine::String::operator=((String *)pAVar19,pSVar1);
        }
      }
    }
    else {
      Globals::replaceKeyBindingTokens(aSStack_34);
      AbyssEngine::String::operator=((String *)pAVar19,aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
    }
  }
  else if (piVar11 == (int *)0x0) {
    uVar15 = ((int)this->field_48 << 1 | 1U) + *(int *)(this->field_5C + this->field_10 * 4);
    iVar12 = *(int *)(DAT_001775d0 + 0x177440 + uVar15 * 4);
    pSVar1 = (String *)GameText::getText(*piVar18);
    AbyssEngine::String::operator=((String *)&this->m_pText,pSVar1);
    if (((uVar15 < 0x18) && ((1 << (uVar15 & 0xff) & DAT_001775d4) != 0)) || (uVar15 == 0x23)) {
      Globals::replaceKeyBindingTokens(aSStack_34);
      AbyssEngine::String::operator=((String *)&this->m_pText,aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
    }
  }
  else {
    uVar15 = this->field_10 - 0x26;
    if ((uVar15 < 4) && (uVar15 != 1)) {
      pSVar1 = (String *)GameText::getText(*piVar18);
      AbyssEngine::String::String(aSStack_34,pSVar1,false);
    }
    else {
      AbyssEngine::String::String(aSStack_34,(char *)(DAT_0017798c + 0x1775ee),false);
    }
    pSVar1 = (String *)GameText::getText(*piVar18);
    AbyssEngine::String::String(aSStack_70,(char *)(DAT_00177990 + 0x177608),false);
    AbyssEngine::operator+(aSStack_64,pSVar1);
    AbyssEngine::operator+(aSStack_58,aSStack_64);
    AbyssEngine::String::String(aSStack_7c,(char *)(DAT_00177994 + 0x177628),false);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    GameText::getText(*piVar18);
    AbyssEngine::operator+(aSStack_40,aSStack_4c);
    AbyssEngine::String::operator=((String *)&this->m_pText,aSStack_40);
    pcVar16 = *(code **)(DAT_00177998 + 0x17765e);
    (*pcVar16)(aSStack_40);
    (*pcVar16)(aSStack_4c);
    (*pcVar16)(aSStack_7c);
    (*pcVar16)(aSStack_58);
    (*pcVar16)(aSStack_64);
    (*pcVar16)(aSStack_70);
    (*pcVar16)(aSStack_34);
    iVar12 = -1;
  }
LAB_00177a12:
  pSVar17 = (ScrollTouchWindow *)this->field_40;
  AbyssEngine::String::String(aSStack_184,(char *)(DAT_00177da8 + 0x177a1e),false);
  AbyssEngine::String::String(aSStack_190,(String *)&this->m_pText,false);
  puVar2 = *(undefined4 **)(DAT_00177db0 + 0x177a38);
  if (iVar4 != 0x38) {
    puVar2 = *(undefined4 **)(DAT_00177dac + 0x177a36);
  }
  if (iVar4 == 0x13) {
    puVar2 = *(undefined4 **)(DAT_00177db0 + 0x177a38);
  }
  ScrollTouchWindow::setText(pSVar17,aSStack_184,aSStack_190,*puVar2);
  AbyssEngine::String::~String(aSStack_190);
  AbyssEngine::String::~String(aSStack_184);
  TouchButton::setVisible((TouchButton *)this->field_0,this->field_48 != (int *)0x0);
  pTVar13 = (TouchButton *)this->field_8;
  iVar4 = length(this);
  TouchButton::setVisible(pTVar13,1 < iVar4);
  piVar11 = (int *)ImageFactory::loadChar
                             ((ImageFactory *)**(undefined4 **)(DAT_00177db4 + 0x177a8c),
                              this->field_24);
  this->field_C = piVar11;
  iVar4 = isLastPage(this);
  if (iVar4 != 0) {
    pTVar13 = (TouchButton *)this->field_4;
    pSVar1 = (String *)GameText::getText(*piVar18);
    TouchButton::replaceTextKeepSize(pTVar13,pSVar1);
  }
  this_03 = (Globals *)**(undefined4 **)(DAT_00177db8 + 0x177ab4);
  if ((Mission *)this->field_4C == (Mission *)0x0) {
    pAVar9 = (Agent *)0x0;
  }
  else {
    pAVar9 = (Agent *)Mission::getAgent((Mission *)this->field_4C);
  }
  pVVar10 = (Vector *)Globals::getDialogueSoundId(this_03,iVar12,pAVar9);
  this->field_64 = (int)pVVar10;
  if (-1 < (int)pVVar10) {
    FModSound::play(*piVar20,pVVar10,(Vector *)0x0,extraout_s0);
    piVar18 = (int *)FModSound::getEventPauseLength(*piVar20);
    this[1].field_0 = piVar18;
  }
  if (*piVar21 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001771f4: push {r4,r5,r6,r7,lr}
  001771f6: add r7,sp,#0xc
  001771f8: push {r8,r9,r10,r11}
  001771fc: sub sp,#0x17c
  001771fe: mov r4,r0
  00177200: ldr r0,[0x001775a4]
  00177202: ldr r1,[0x001775a8]
  00177204: add r0,pc
  00177206: add r1,pc
  00177208: ldr.w r11,[r0,#0x0]
  0017720c: ldr r6,[r1,#0x0]
  0017720e: ldr.w r1,[r11,#0x0]
  00177212: ldr r0,[r6,#0x0]
  00177214: str r1,[sp,#0x178]
  00177216: movs r1,#0xb4
  00177218: ldr r5,[r4,#0x4]
  0017721a: blx 0x00072f70
  0017721e: mov r1,r0
  00177220: mov r0,r5
  00177222: blx 0x00077080
  00177226: ldr r0,[0x001775ac]
  00177228: movs r5,#0x0
  0017722a: ldr r1,[r4,#0x64]
  0017722c: add r0,pc
  0017722e: strd r5,r5,[r4,#0x68]
  00177232: ldr.w r8,[r0,#0x0]
  00177236: ldr.w r0,[r8,#0x0]
  0017723a: blx 0x000724a8
  0017723e: ldr r0,[r4,#0x4]
  00177240: mov.w r1,#0xffffffff
  00177244: str r1,[r4,#0x64]
  00177246: movs r1,#0x0
  00177248: blx 0x00077a70
  0017724c: ldr r0,[r4,#0xc]
  0017724e: cbz r0,0x00177260
  00177250: blx 0x00075ae4
  00177254: ldr r0,[r4,#0xc]
  00177256: cbz r0,0x00177260
  00177258: blx 0x00075af0
  0017725c: blx 0x0006eb48
  00177260: ldr r0,[r4,#0x4c]
  00177262: str r5,[r4,#0xc]
  00177264: blx 0x0007372c
  00177268: ldr r1,[r4,#0x44]
  0017726a: cmp r0,#0x0
  0017726c: str r6,[sp,#0xc]
  0017726e: beq 0x00177290
  00177270: cmp r1,#0x1
  00177272: str.w r8,[sp,#0x8]
  00177276: beq 0x0017730a
  00177278: cmp r1,#0x0
  0017727a: bne.w 0x001773ea
  0017727e: ldr r0,[r4,#0x10]
  00177280: mov r8,r11
  00177282: ldr r2,[r4,#0x5c]
  00177284: ldr r1,[r4,#0x48]
  00177286: ldr.w r0,[r2,r0,lsl #0x2]
  0017728a: ldr r2,[0x001775b0]
  0017728c: add r2,pc
  0017728e: b 0x0017731a
  00177290: cmp r1,#0x2
  00177292: itt eq
  00177294: ldr.eq r0,[r4,#0x10]
  00177296: cmp.eq r0,#0x2a
  00177298: beq.w 0x001773e6
  0017729c: ldrb.w r0,[r4,#0x48]
  001772a0: lsls r0,r0,#0x1f
  001772a2: bne 0x001772ca
  001772a4: ldr r0,[r4,#0x4c]
  001772a6: blx 0x0007393c
  001772aa: str r0,[r4,#0x24]
  001772ac: add r5,sp,#0x16c
  001772ae: ldr r1,[r4,#0x4c]
  001772b0: mov r0,r5
  001772b2: blx 0x00074b3c
  001772b6: add.w r0,r4,#0x34
  001772ba: mov r1,r5
  001772bc: blx 0x0006f2b0
  001772c0: add r0,sp,#0x16c
  001772c2: blx 0x0006ee30
  001772c6: movs r0,#0x0
  001772c8: b 0x001772e6
  001772ca: ldr r0,[0x001775b4]
  001772cc: movw r1,#0x63d
  001772d0: add r0,pc
  001772d2: str r0,[r4,#0x24]
  001772d4: ldr r0,[r6,#0x0]
  001772d6: blx 0x00072f70
  001772da: mov r1,r0
  001772dc: add.w r0,r4,#0x34
  001772e0: blx 0x0006f2b0
  001772e4: movs r0,#0x1
  001772e6: ldr r1,[r4,#0x44]
  001772e8: strb.w r0,[r4,#0x70]
  001772ec: cmp r1,#0x1
  001772ee: beq 0x00177324
  001772f0: cbnz r1,0x0017733e
  001772f2: ldr r0,[r4,#0x4c]
  001772f4: blx 0x00072874
  001772f8: cmp r0,#0xc
  001772fa: bne.w 0x00177558
  001772fe: ldr r0,[r6,#0x0]
  00177300: mov.w r9,#0x174
  00177304: mov.w r1,#0x174
  00177308: b 0x001779fe
  0017730a: ldr r0,[r4,#0x10]
  0017730c: mov r8,r11
  0017730e: ldr r2,[r4,#0x60]
  00177310: ldr r1,[r4,#0x48]
  00177312: ldr.w r0,[r2,r0,lsl #0x2]
  00177316: ldr r2,[0x001775b8]
  00177318: add r2,pc
  0017731a: add.w r0,r0,r1, lsl #0x1
  0017731e: ldr.w r10,[r2,r0,lsl #0x2]
  00177322: b 0x001773f0
  00177324: ldr r0,[r4,#0x4c]
  00177326: blx 0x00073438
  0017732a: cmp r0,#0x0
  0017732c: beq.w 0x00177680
  00177330: ldr r0,[r4,#0x4c]
  00177332: blx 0x00073438
  00177336: blx 0x00071ba8
  0017733a: mov r6,r0
  0017733c: b 0x00177682
  0017733e: ldr r0,[r4,#0x4c]
  00177340: blx 0x00072874
  00177344: cmp r0,#0xc
  00177346: bne.w 0x00177570
  0017734a: ldr r0,[0x001775bc]
  0017734c: movw r1,#0x173
  00177350: add r0,pc
  00177352: ldr r0,[r0,#0x0]
  00177354: ldr r5,[r0,#0x0]
  00177356: ldr r0,[r6,#0x0]
  00177358: blx 0x00072f70
  0017735c: mov r1,r0
  0017735e: add r0,sp,#0x58
  00177360: movs r2,#0x0
  00177362: blx 0x0006f028
  00177366: ldr r0,[r4,#0x58]
  00177368: ldr r1,[r0,#0x24]
  0017736a: add r0,sp,#0x4c
  0017736c: blx 0x0006f658
  00177370: ldr r1,[0x001775c0]
  00177372: add r1,pc
  00177374: add r0,sp,#0x40
  00177376: movs r2,#0x0
  00177378: blx 0x0006ee18
  0017737c: str r0,[sp,#0x0]
  0017737e: add r0,sp,#0x64
  00177380: add r2,sp,#0x58
  00177382: add r3,sp,#0x4c
  00177384: mov r1,r5
  00177386: blx 0x000739b4
  0017738a: ldr r0,[r4,#0x58]
  0017738c: ldr r1,[r0,#0x20]
  0017738e: add r0,sp,#0x34
  00177390: blx 0x0006f658
  00177394: ldr r1,[0x001775c4]
  00177396: add r1,pc
  00177398: add r0,sp,#0x28
  0017739a: movs r2,#0x0
  0017739c: blx 0x0006ee18
  001773a0: str r0,[sp,#0x0]
  001773a2: add r0,sp,#0x16c
  001773a4: add r2,sp,#0x64
  001773a6: add r3,sp,#0x34
  001773a8: mov r1,r5
  001773aa: blx 0x000739b4
  001773ae: add.w r6,r4,#0x28
  001773b2: add r1,sp,#0x16c
  001773b4: mov r0,r6
  001773b6: blx 0x0006f2b0
  001773ba: ldr r0,[0x001775c8]
  001773bc: add r0,pc
  001773be: ldr r5,[r0,#0x0]
  001773c0: add r0,sp,#0x16c
  001773c2: blx r5
  001773c4: add r0,sp,#0x28
  001773c6: blx r5
  001773c8: add r0,sp,#0x34
  001773ca: blx r5
  001773cc: add r0,sp,#0x64
  001773ce: blx r5
  001773d0: add r0,sp,#0x40
  001773d2: blx r5
  001773d4: add r0,sp,#0x4c
  001773d6: blx r5
  001773d8: add r0,sp,#0x58
  001773da: blx r5
  001773dc: mov.w r10,#0xffffffff
  001773e0: movw r9,#0x173
  001773e4: b 0x00177a12
  001773e6: str.w r8,[sp,#0x8]
  001773ea: mov r8,r11
  001773ec: mov.w r10,#0x10
  001773f0: ldr r0,[0x001775cc]
  001773f2: addw r1,r10,#0x63d
  001773f6: add r0,pc
  001773f8: ldr.w r0,[r0,r10,lsl #0x2]
  001773fc: str r0,[r4,#0x24]
  001773fe: ldr r0,[r6,#0x0]
  00177400: blx 0x00072f70
  00177404: add.w r5,r4,#0x34
  00177408: mov r1,r0
  0017740a: mov r0,r5
  0017740c: blx 0x0006f2b0
  00177410: clz r1,r10
  00177414: ldr r0,[r4,#0x44]
  00177416: lsr.w r11,r1,#0x5
  0017741a: cmp r0,#0x1
  0017741c: strb.w r11,[r4,#0x70]
  00177420: beq 0x0017748c
  00177422: cmp r0,#0x0
  00177424: bne 0x001774f2
  00177426: ldr r0,[r4,#0x10]
  00177428: mov.w r11,#0x1
  0017742c: ldr r2,[r4,#0x5c]
  0017742e: ldr r1,[r4,#0x48]
  00177430: ldr.w r2,[r2,r0,lsl #0x2]
  00177434: ldr r3,[0x001775d0]
  00177436: orr.w r1,r11,r1, lsl #0x1
  0017743a: ldr r0,[r6,#0x0]
  0017743c: add r3,pc
  0017743e: adds r5,r1,r2
  00177440: ldr.w r1,[r3,r5,lsl #0x2]
  00177444: mov r9,r1
  00177446: blx 0x00072f70
  0017744a: add.w r6,r4,#0x28
  0017744e: mov r1,r0
  00177450: mov r0,r6
  00177452: blx 0x0006f2b0
  00177456: cmp r5,#0x17
  00177458: bhi.w 0x0017759a
  0017745c: ldr r0,[0x001775d4]
  0017745e: lsl.w r1,r11,r5
  00177462: tst r1,r0
  00177464: beq.w 0x0017759a
  00177468: ldr r0,[0x001775d8]
  0017746a: add r5,sp,#0x16c
  0017746c: mov r2,r6
  0017746e: add r0,pc
  00177470: ldr r0,[r0,#0x0]
  00177472: ldr r1,[r0,#0x0]
  00177474: mov r0,r5
  00177476: blx 0x00077c08
  0017747a: mov r0,r6
  0017747c: mov r1,r5
  0017747e: blx 0x0006f2b0
  00177482: add r0,sp,#0x16c
  00177484: blx 0x0006ee30
  00177488: mov r11,r8
  0017748a: b 0x001774ec
  0017748c: ldr r0,[r4,#0x10]
  0017748e: ldr r2,[r4,#0x60]
  00177490: ldr r1,[r4,#0x48]
  00177492: ldr.w r2,[r2,r0,lsl #0x2]
  00177496: movs r0,#0x1
  00177498: ldr r3,[0x001775dc]
  0017749a: orr.w r1,r0,r1, lsl #0x1
  0017749e: ldr r0,[r6,#0x0]
  001774a0: add r3,pc
  001774a2: add r1,r2
  001774a4: ldr.w r9,[r3,r1,lsl #0x2]
  001774a8: mov r1,r9
  001774aa: blx 0x00072f70
  001774ae: add.w r6,r4,#0x28
  001774b2: mov r1,r0
  001774b4: mov r0,r6
  001774b6: blx 0x0006f2b0
  001774ba: movw r0,#0x6b6
  001774be: cmp r9,r0
  001774c0: itt ne
  001774c2: movw.ne r0,#0x6c2
  001774c6: cmp.ne r9,r0
  001774c8: bne 0x00177514
  001774ca: ldr r0,[0x001775e0]
  001774cc: add r5,sp,#0x16c
  001774ce: mov r2,r6
  001774d0: add r0,pc
  001774d2: ldr r0,[r0,#0x0]
  001774d4: ldr r1,[r0,#0x0]
  001774d6: mov r0,r5
  001774d8: blx 0x00077c08
  001774dc: mov r0,r6
  001774de: mov r1,r5
  001774e0: blx 0x0006f2b0
  001774e4: add r0,sp,#0x16c
  001774e6: mov r11,r8
  001774e8: blx 0x0006ee30
  001774ec: ldr.w r8,[sp,#0x8]
  001774f0: b 0x00177a12
  001774f2: ldr r0,[r4,#0x10]
  001774f4: subs r0,#0x26
  001774f6: cmp r0,#0x3
  001774f8: bhi 0x001775e4
  001774fa: cmp r0,#0x1
  001774fc: beq 0x001775e4
  001774fe: ldr r0,[r6,#0x0]
  00177500: movw r1,#0x20f
  00177504: blx 0x00072f70
  00177508: mov r1,r0
  0017750a: add r0,sp,#0x16c
  0017750c: movs r2,#0x0
  0017750e: blx 0x0006f028
  00177512: b 0x001775f0
  00177514: movw r0,#0x729
  00177518: cmp r9,r0
  0017751a: bne.w 0x001777b0
  0017751e: ldr.w r9,[sp,#0x8]
  00177522: ldr.w r0,[r9,#0x0]
  00177526: ldr r1,[r0,#0x0]
  00177528: blx 0x000724a8
  0017752c: ldr.w r0,[r9,#0x0]
  00177530: movs r5,#0x0
  00177532: movs r1,#0x88
  00177534: movs r2,#0x0
  00177536: movs r3,#0x0
  00177538: str r5,[sp,#0x0]
  0017753a: blx 0x00071548
  0017753e: ldr.w r0,[r9,#0x0]
  00177542: movs r1,#0xa2
  00177544: movs r2,#0x0
  00177546: movs r3,#0x0
  00177548: str r5,[sp,#0x0]
  0017754a: blx 0x00071548
  0017754e: mov r11,r8
  00177550: mov r8,r9
  00177552: movw r9,#0x729
  00177556: b 0x00177a12
  00177558: ldr r0,[r4,#0x4c]
  0017755a: blx 0x00072874
  0017755e: cmp r0,#0x7
  00177560: bne.w 0x001777fc
  00177564: ldr r0,[r6,#0x0]
  00177566: mov.w r9,#0x17a
  0017756a: mov.w r1,#0x17a
  0017756e: b 0x001779fe
  00177570: blx 0x0006f544
  00177574: cmp r0,#0x1
  00177576: bne.w 0x00177740
  0017757a: ldr r0,[r4,#0x4c]
  0017757c: blx 0x00073438
  00177580: cmp r0,#0x0
  00177582: beq.w 0x00177740
  00177586: ldr r0,[r4,#0x4c]
  00177588: ldr r5,[r4,#0x44]
  0017758a: blx 0x00073438
  0017758e: mov r1,r5
  00177590: mov r2,r0
  00177592: blx 0x00077c14
  00177596: mov r1,r0
  00177598: b 0x00177756
  0017759a: cmp r5,#0x23
  0017759c: beq.w 0x00177468
  001775a0: b 0x00177488
  001775e4: ldr r1,[0x0017798c]
  001775e6: add r0,sp,#0x16c
  001775e8: movs r2,#0x0
  001775ea: add r1,pc
  001775ec: blx 0x0006ee18
  001775f0: ldr r0,[r6,#0x0]
  001775f2: mov r11,r8
  001775f4: mov.w r1,#0x188
  001775f8: blx 0x00072f70
  001775fc: ldr.w r8,[sp,#0x8]
  00177600: ldr r1,[0x00177990]
  00177602: mov r5,r0
  00177604: add r1,pc
  00177606: add r0,sp,#0x130
  00177608: movs r2,#0x0
  0017760a: blx 0x0006ee18
  0017760e: add r0,sp,#0x13c
  00177610: add r2,sp,#0x130
  00177612: mov r1,r5
  00177614: blx 0x0006ef98
  00177618: add r0,sp,#0x148
  0017761a: add r1,sp,#0x13c
  0017761c: add r2,sp,#0x16c
  0017761e: blx 0x0006ef98
  00177622: ldr r1,[0x00177994]
  00177624: add r1,pc
  00177626: add r0,sp,#0x124
  00177628: movs r2,#0x0
  0017762a: blx 0x0006ee18
  0017762e: add r0,sp,#0x154
  00177630: add r1,sp,#0x148
  00177632: add r2,sp,#0x124
  00177634: blx 0x0006ef98
  00177638: ldr r0,[r6,#0x0]
  0017763a: movw r1,#0x13f
  0017763e: blx 0x00072f70
  00177642: mov r2,r0
  00177644: add r0,sp,#0x160
  00177646: add r1,sp,#0x154
  00177648: blx 0x0006ef98
  0017764c: add.w r6,r4,#0x28
  00177650: add r1,sp,#0x160
  00177652: mov r0,r6
  00177654: blx 0x0006f2b0
  00177658: ldr r0,[0x00177998]
  0017765a: add r0,pc
  0017765c: ldr r5,[r0,#0x0]
  0017765e: add r0,sp,#0x160
  00177660: blx r5
  00177662: add r0,sp,#0x154
  00177664: blx r5
  00177666: add r0,sp,#0x124
  00177668: blx r5
  0017766a: add r0,sp,#0x148
  0017766c: blx r5
  0017766e: add r0,sp,#0x13c
  00177670: blx r5
  00177672: add r0,sp,#0x130
  00177674: blx r5
  00177676: add r0,sp,#0x16c
  00177678: blx r5
  0017767a: mov.w r9,#0xffffffff
  0017767e: b 0x00177a12
  00177680: movs r6,#0x0
  00177682: ldr r0,[r4,#0x4c]
  00177684: blx 0x00072874
  00177688: cmp r0,#0x5
  0017768a: beq 0x00177698
  0017768c: ldr r0,[r4,#0x4c]
  0017768e: blx 0x00072874
  00177692: cmp r0,#0x3
  00177694: bne.w 0x0017782a
  00177698: ldr r0,[sp,#0xc]
  0017769a: movw r1,#0x185
  0017769e: ldr r0,[r0,#0x0]
  001776a0: blx 0x00072f70
  001776a4: add.w r5,r4,#0x28
  001776a8: mov r1,r0
  001776aa: mov r0,r5
  001776ac: blx 0x0006f2b0
  001776b0: ldr r0,[0x0017799c]
  001776b2: mov r1,r5
  001776b4: movs r2,#0x0
  001776b6: add r0,pc
  001776b8: ldr r0,[r0,#0x0]
  001776ba: ldr r6,[r0,#0x0]
  001776bc: add r0,sp,#0x118
  001776be: blx 0x0006f028
  001776c2: ldr r1,[r4,#0x4c]
  001776c4: add r0,sp,#0x10c
  001776c6: blx 0x00074b54
  001776ca: ldr r1,[0x001779a0]
  001776cc: add r1,pc
  001776ce: add r0,sp,#0x100
  001776d0: movs r2,#0x0
  001776d2: blx 0x0006ee18
  001776d6: str r0,[sp,#0x0]
  001776d8: add r0,sp,#0x16c
  001776da: add r2,sp,#0x118
  001776dc: add r3,sp,#0x10c
  001776de: mov r1,r6
  001776e0: blx 0x000739b4
  001776e4: add r1,sp,#0x16c
  001776e6: mov r0,r5
  001776e8: blx 0x0006f2b0
  001776ec: ldr r6,[sp,#0xc]
  001776ee: ldr r0,[0x001779a4]
  001776f0: add r0,pc
  001776f2: ldr r5,[r0,#0x0]
  001776f4: add r0,sp,#0x16c
  001776f6: blx r5
  001776f8: add r0,sp,#0x100
  001776fa: blx r5
  001776fc: add r0,sp,#0x10c
  001776fe: blx r5
  00177700: add r0,sp,#0x118
  00177702: blx r5
  00177704: movw r9,#0x185
  00177708: ldr r0,[0x001779a8]
  0017770a: add r0,pc
  0017770c: ldr r0,[r0,#0x0]
  0017770e: ldr r0,[r0,#0x0]
  00177710: blx 0x00071b84
  00177714: mov r5,r0
  00177716: ldr r0,[r4,#0x4c]
  00177718: blx 0x00074014
  0017771c: mov r1,r0
  0017771e: mov r0,r5
  00177720: blx 0x00077c20
  00177724: ldr r0,[r4,#0x4c]
  00177726: blx 0x00073738
  0017772a: cmp r0,#0x6c
  0017772c: bne 0x0017773a
  0017772e: ldr r0,[r6,#0x0]
  00177730: mov.w r9,#0x1ca
  00177734: mov.w r1,#0x1ca
  00177738: b 0x001779fe
  0017773a: add.w r6,r4,#0x28
  0017773e: b 0x00177a0e
  00177740: ldr r0,[0x001779ac]
  00177742: movs r1,#0x5
  00177744: add r0,pc
  00177746: ldr r0,[r0,#0x0]
  00177748: ldr r0,[r0,#0x0]
  0017774a: blx 0x00071848
  0017774e: ldr r1,[0x001779b0]
  00177750: add r1,pc
  00177752: ldr.w r1,[r1,r0,lsl #0x2]
  00177756: ldr r0,[r6,#0x0]
  00177758: mov r10,r1
  0017775a: blx 0x00072f70
  0017775e: ldr r1,[0x001779b4]
  00177760: add.w r9,sp,#0x154
  00177764: mov r5,r0
  00177766: movs r2,#0x0
  00177768: add r1,pc
  0017776a: mov r0,r9
  0017776c: blx 0x0006ee18
  00177770: add r0,sp,#0x160
  00177772: mov r1,r5
  00177774: mov r2,r9
  00177776: blx 0x0006ef98
  0017777a: ldr r0,[r6,#0x0]
  0017777c: mov.w r1,#0x188
  00177780: blx 0x00072f70
  00177784: mov r2,r0
  00177786: add r0,sp,#0x16c
  00177788: add r1,sp,#0x160
  0017778a: blx 0x0006ef98
  0017778e: add.w r6,r4,#0x28
  00177792: add r1,sp,#0x16c
  00177794: mov r0,r6
  00177796: blx 0x0006f2b0
  0017779a: ldr r0,[0x001779b8]
  0017779c: mov r9,r10
  0017779e: add r0,pc
  001777a0: ldr r5,[r0,#0x0]
  001777a2: add r0,sp,#0x16c
  001777a4: blx r5
  001777a6: add r0,sp,#0x160
  001777a8: blx r5
  001777aa: add r0,sp,#0x154
  001777ac: blx r5
  001777ae: b 0x00177a0e
  001777b0: blx 0x0006f544
  001777b4: movw r1,#0xade
  001777b8: cmp r9,r1
  001777ba: str.w r9,[sp,#0x4]
  001777be: it eq
  001777c0: cmp.eq r0,#0x1
  001777c2: beq.w 0x00177b06
  001777c6: ldr r0,[sp,#0x4]
  001777c8: subw r0,r0,#0x891
  001777cc: cmp r0,#0x1
  001777ce: bhi.w 0x00177b30
  001777d2: ldr r0,[r4,#0x4c]
  001777d4: blx 0x0007288c
  001777d8: cmp r0,#0x1
  001777da: mov r11,r8
  001777dc: bne.w 0x00177b32
  001777e0: ldr r0,[sp,#0xc]
  001777e2: ldr.w r9,[sp,#0x4]
  001777e6: sub.w r9,r9,#0x2
  001777ea: ldr r0,[r0,#0x0]
  001777ec: mov r1,r9
  001777ee: blx 0x00072f70
  001777f2: mov r1,r0
  001777f4: mov r0,r6
  001777f6: blx 0x0006f2b0
  001777fa: b 0x001774ec
  001777fc: ldr r0,[r4,#0x4c]
  001777fe: cmp r0,#0x0
  00177800: beq 0x001778a6
  00177802: blx 0x00073738
  00177806: cmp r0,#0x6c
  00177808: bne 0x001778a6
  0017780a: ldr r0,[0x001779bc]
  0017780c: movs r2,#0x1
  0017780e: ldr r1,[r4,#0x48]
  00177810: add r0,pc
  00177812: ldr r0,[r0,#0x0]
  00177814: ldr r0,[r0,#0x0]
  00177816: ldr.w r3,[r0,#0x114]
  0017781a: orr.w r0,r2,r1, lsl #0x1
  0017781e: cmp r3,#0x2
  00177820: bne.w 0x00177cdc
  00177824: ldr r1,[0x001779c0]
  00177826: add r1,pc
  00177828: b 0x00177ce0
  0017782a: ldr r0,[r4,#0x4c]
  0017782c: blx 0x00072874
  00177830: cmp r0,#0xc
  00177832: bne 0x001778d0
  00177834: ldr r5,[sp,#0xc]
  00177836: mov.w r1,#0x172
  0017783a: ldr r0,[r5,#0x0]
  0017783c: blx 0x00072f70
  00177840: mov r9,r0
  00177842: cmp r6,#0x0
  00177844: beq.w 0x00177b38
  00177848: ldr r1,[0x001779c4]
  0017784a: add r0,sp,#0x154
  0017784c: movs r2,#0x0
  0017784e: add r1,pc
  00177850: blx 0x0006ee18
  00177854: ldr r0,[0x001779c8]
  00177856: add r0,pc
  00177858: ldr r0,[r0,#0x0]
  0017785a: ldr r6,[r0,#0x0]
  0017785c: ldr r0,[r5,#0x0]
  0017785e: movw r1,#0x185
  00177862: blx 0x00072f70
  00177866: mov r1,r0
  00177868: add r0,sp,#0xf4
  0017786a: movs r2,#0x0
  0017786c: blx 0x0006f028
  00177870: ldr r0,[r4,#0x4c]
  00177872: blx 0x00073438
  00177876: mov r1,r0
  00177878: add r0,sp,#0xe8
  0017787a: blx 0x00074c14
  0017787e: ldr r1,[0x001779cc]
  00177880: add r1,pc
  00177882: add r0,sp,#0xdc
  00177884: movs r2,#0x0
  00177886: blx 0x0006ee18
  0017788a: str r0,[sp,#0x0]
  0017788c: add r0,sp,#0x148
  0017788e: add r2,sp,#0xf4
  00177890: add r3,sp,#0xe8
  00177892: mov r1,r6
  00177894: blx 0x000739b4
  00177898: add r0,sp,#0x160
  0017789a: add r1,sp,#0x154
  0017789c: add r2,sp,#0x148
  0017789e: blx 0x0006ef98
  001778a2: movs r6,#0x1
  001778a4: b 0x00177b46
  001778a6: blx 0x0006f544
  001778aa: cmp r0,#0x1
  001778ac: bne.w 0x001779e4
  001778b0: ldr r0,[r4,#0x4c]
  001778b2: blx 0x00073438
  001778b6: cmp r0,#0x0
  001778b8: beq.w 0x001779e4
  001778bc: ldr r0,[r4,#0x4c]
  001778be: ldr r5,[r4,#0x44]
  001778c0: blx 0x00073438
  001778c4: mov r1,r5
  001778c6: mov r2,r0
  001778c8: blx 0x00077c14
  001778cc: mov r9,r0
  001778ce: b 0x001779fa
  001778d0: ldr r0,[r4,#0x4c]
  001778d2: blx 0x00072874
  001778d6: cmp r0,#0xb7
  001778d8: bne.w 0x00177c10
  001778dc: ldr r0,[0x001779d0]
  001778de: movs r1,#0x5
  001778e0: add r0,pc
  001778e2: ldr r0,[r0,#0x0]
  001778e4: ldr r0,[r0,#0x0]
  001778e6: blx 0x00071848
  001778ea: ldr r1,[0x001779d4]
  001778ec: ldr r5,[sp,#0xc]
  001778ee: add r1,pc
  001778f0: ldr.w r9,[r1,r0,lsl #0x2]
  001778f4: ldr r0,[r5,#0x0]
  001778f6: mov r1,r9
  001778f8: blx 0x00072f70
  001778fc: add.w r6,r4,#0x28
  00177900: mov r1,r0
  00177902: mov r0,r6
  00177904: blx 0x0006f2b0
  00177908: ldr r1,[0x001779d8]
  0017790a: add r0,sp,#0x148
  0017790c: movs r2,#0x0
  0017790e: add r1,pc
  00177910: blx 0x0006ee18
  00177914: ldr r0,[r5,#0x0]
  00177916: movs r1,#0xb0
  00177918: blx 0x00072f70
  0017791c: mov r2,r0
  0017791e: add r0,sp,#0x154
  00177920: add r1,sp,#0x148
  00177922: blx 0x0006ef98
  00177926: ldr r1,[0x001779dc]
  00177928: add r1,pc
  0017792a: add r0,sp,#0x13c
  0017792c: movs r2,#0x0
  0017792e: blx 0x0006ee18
  00177932: add r0,sp,#0x160
  00177934: add r1,sp,#0x154
  00177936: add r2,sp,#0x13c
  00177938: blx 0x0006ef98
  0017793c: ldr r0,[r4,#0x58]
  0017793e: ldr r1,[r0,#0x24]
  00177940: add r0,sp,#0x130
  00177942: blx 0x0006f658
  00177946: add r0,sp,#0x16c
  00177948: add r1,sp,#0x160
  0017794a: add r2,sp,#0x130
  0017794c: blx 0x0006ef98
  00177950: ldr r0,[0x001779e0]
  00177952: add r0,pc
  00177954: ldr r5,[r0,#0x0]
  00177956: add r0,sp,#0x130
  00177958: blx r5
  0017795a: add r0,sp,#0x160
  0017795c: blx r5
  0017795e: add r0,sp,#0x13c
  00177960: blx r5
  00177962: add r0,sp,#0x154
  00177964: blx r5
  00177966: add r0,sp,#0x148
  00177968: blx r5
  0017796a: add r0,sp,#0x160
  0017796c: add r2,sp,#0x16c
  0017796e: mov r1,r6
  00177970: blx 0x0006ef98
  00177974: add r1,sp,#0x160
  00177976: mov r0,r6
  00177978: blx 0x0006f2b0
  0017797c: add r0,sp,#0x160
  0017797e: blx 0x0006ee30
  00177982: add r0,sp,#0x16c
  00177984: blx 0x0006ee30
  00177988: ldr r6,[sp,#0xc]
  0017798a: b 0x00177708
  001779e4: ldr r0,[0x00177da0]
  001779e6: movs r1,#0x5
  001779e8: add r0,pc
  001779ea: ldr r0,[r0,#0x0]
  001779ec: ldr r0,[r0,#0x0]
  001779ee: blx 0x00071848
  001779f2: ldr r1,[0x00177da4]
  001779f4: add r1,pc
  001779f6: ldr.w r9,[r1,r0,lsl #0x2]
  001779fa: ldr r0,[r6,#0x0]
  001779fc: mov r1,r9
  001779fe: blx 0x00072f70
  00177a02: add.w r6,r4,#0x28
  00177a06: mov r1,r0
  00177a08: mov r0,r6
  00177a0a: blx 0x0006f2b0
  00177a0e: mov.w r10,#0xffffffff
  00177a12: ldr r5,[r4,#0x40]
  00177a14: add r0,sp,#0x1c
  00177a16: ldr r1,[0x00177da8]
  00177a18: movs r2,#0x0
  00177a1a: add r1,pc
  00177a1c: blx 0x0006ee18
  00177a20: add r0,sp,#0x10
  00177a22: mov r1,r6
  00177a24: movs r2,#0x0
  00177a26: blx 0x0006f028
  00177a2a: ldr r0,[0x00177dac]
  00177a2c: cmp.w r10,#0x38
  00177a30: ldr r1,[0x00177db0]
  00177a32: add r0,pc
  00177a34: add r1,pc
  00177a36: ldr r1,[r1,#0x0]
  00177a38: mov r2,r1
  00177a3a: it ne
  00177a3c: ldr.ne r2,[r0,#0x0]
  00177a3e: cmp.w r10,#0x13
  00177a42: it eq
  00177a44: mov.eq r2,r1
  00177a46: ldr r3,[r2,#0x0]
  00177a48: add r1,sp,#0x1c
  00177a4a: add r2,sp,#0x10
  00177a4c: mov r0,r5
  00177a4e: blx 0x00077c2c
  00177a52: ldr r6,[sp,#0xc]
  00177a54: add r0,sp,#0x10
  00177a56: blx 0x0006ee30
  00177a5a: add r0,sp,#0x1c
  00177a5c: blx 0x0006ee30
  00177a60: ldr r1,[r4,#0x48]
  00177a62: ldr r0,[r4,#0x0]
  00177a64: cmp r1,#0x0
  00177a66: it ne
  00177a68: mov.ne r1,#0x1
  00177a6a: blx 0x00074e48
  00177a6e: mov r0,r4
  00177a70: ldr r5,[r4,#0x8]
  00177a72: blx 0x00077c38
  00177a76: movs r1,#0x0
  00177a78: cmp r0,#0x1
  00177a7a: it gt
  00177a7c: mov.gt r1,#0x1
  00177a7e: mov r0,r5
  00177a80: blx 0x00074e48
  00177a84: ldr r0,[0x00177db4]
  00177a86: ldr r1,[r4,#0x24]
  00177a88: add r0,pc
  00177a8a: ldr r0,[r0,#0x0]
  00177a8c: ldr r0,[r0,#0x0]
  00177a8e: blx 0x00075b14
  00177a92: str r0,[r4,#0xc]
  00177a94: mov r0,r4
  00177a96: blx 0x00077c44
  00177a9a: cbz r0,0x00177aae
  00177a9c: ldr r0,[r6,#0x0]
  00177a9e: movs r1,#0xb5
  00177aa0: ldr r5,[r4,#0x4]
  00177aa2: blx 0x00072f70
  00177aa6: mov r1,r0
  00177aa8: mov r0,r5
  00177aaa: blx 0x00077080
  00177aae: ldr r0,[0x00177db8]
  00177ab0: add r0,pc
  00177ab2: ldr r1,[r0,#0x0]
  00177ab4: ldr r0,[r4,#0x4c]
  00177ab6: ldr r5,[r1,#0x0]
  00177ab8: cbz r0,0x00177ac2
  00177aba: blx 0x00073438
  00177abe: mov r2,r0
  00177ac0: b 0x00177ac4
  00177ac2: movs r2,#0x0
  00177ac4: mov r0,r5
  00177ac6: mov r1,r9
  00177ac8: blx 0x00074f2c
  00177acc: cmp r0,#0x0
  00177ace: str r0,[r4,#0x64]
  00177ad0: blt 0x00177af0
  00177ad2: mov r1,r0
  00177ad4: ldr.w r0,[r8,#0x0]
  00177ad8: movs r2,#0x0
  00177ada: movs r3,#0x0
  00177adc: str r2,[sp,#0x0]
  00177ade: movs r2,#0x0
  00177ae0: blx 0x00071548
  00177ae4: ldr r1,[r4,#0x64]
  00177ae6: ldr.w r0,[r8,#0x0]
  00177aea: blx 0x00077c50
  00177aee: str r0,[r4,#0x6c]
  00177af0: ldr r0,[sp,#0x178]
  00177af2: ldr.w r1,[r11,#0x0]
  00177af6: subs r0,r1,r0
  00177af8: ittt eq
  00177afa: add.eq sp,#0x17c
  00177afc: pop.eq.w {r8,r9,r10,r11}
  00177b00: pop.eq {r4,r5,r6,r7,pc}
  00177b02: blx 0x0006e824
  00177b06: ldr r0,[0x00177dbc]
  00177b08: movw r1,#0x63d
  00177b0c: add r0,pc
  00177b0e: str r0,[r4,#0x24]
  00177b10: ldr r0,[sp,#0xc]
  00177b12: ldr r0,[r0,#0x0]
  00177b14: blx 0x00072f70
  00177b18: mov r1,r0
  00177b1a: mov r0,r5
  00177b1c: blx 0x0006f2b0
  00177b20: strb.w r11,[r4,#0x70]
  00177b24: mov r11,r8
  00177b26: ldr.w r8,[sp,#0x8]
  00177b2a: movw r9,#0xade
  00177b2e: b 0x00177a12
  00177b30: mov r11,r8
  00177b32: ldrd r9,r8,[sp,#0x4]
  00177b36: b 0x00177a12
  00177b38: ldr r1,[0x00177dc0]
  00177b3a: movs r6,#0x0
  00177b3c: add r1,pc
  00177b3e: add r0,sp,#0x160
  00177b40: movs r2,#0x0
  00177b42: blx 0x0006ee18
  00177b46: add r0,sp,#0x16c
  00177b48: add r2,sp,#0x160
  00177b4a: mov r1,r9
  00177b4c: blx 0x0006ef98
  00177b50: add.w r5,r4,#0x28
  00177b54: add r1,sp,#0x16c
  00177b56: mov r0,r5
  00177b58: blx 0x0006f2b0
  00177b5c: add r0,sp,#0x16c
  00177b5e: blx 0x0006ee30
  00177b62: add r0,sp,#0x160
  00177b64: blx 0x0006ee30
  00177b68: cbz r6,0x00177b84
  00177b6a: ldr r0,[0x00177dc4]
  00177b6c: add r0,pc
  00177b6e: ldr r6,[r0,#0x0]
  00177b70: add r0,sp,#0x148
  00177b72: blx r6
  00177b74: add r0,sp,#0xdc
  00177b76: blx r6
  00177b78: add r0,sp,#0xe8
  00177b7a: blx r6
  00177b7c: add r0,sp,#0xf4
  00177b7e: blx r6
  00177b80: add r0,sp,#0x154
  00177b82: blx r6
  00177b84: ldr r0,[0x00177dc8]
  00177b86: mov r1,r5
  00177b88: movs r2,#0x0
  00177b8a: add r0,pc
  00177b8c: ldr r0,[r0,#0x0]
  00177b8e: ldr r6,[r0,#0x0]
  00177b90: add r0,sp,#0xc4
  00177b92: blx 0x0006f028
  00177b96: ldr r0,[r4,#0x58]
  00177b98: ldr r1,[r0,#0x24]
  00177b9a: add r0,sp,#0xb8
  00177b9c: blx 0x0006f658
  00177ba0: ldr r1,[0x00177dcc]
  00177ba2: add r1,pc
  00177ba4: add r0,sp,#0xac
  00177ba6: movs r2,#0x0
  00177ba8: blx 0x0006ee18
  00177bac: str r0,[sp,#0x0]
  00177bae: add r0,sp,#0xd0
  00177bb0: add r2,sp,#0xc4
  00177bb2: add r3,sp,#0xb8
  00177bb4: mov r1,r6
  00177bb6: blx 0x000739b4
  00177bba: ldr r0,[r4,#0x58]
  00177bbc: ldr r1,[r0,#0x20]
  00177bbe: add r0,sp,#0xa0
  00177bc0: blx 0x0006f658
  00177bc4: ldr r1,[0x00177dd0]
  00177bc6: add r1,pc
  00177bc8: add r0,sp,#0x94
  00177bca: movs r2,#0x0
  00177bcc: blx 0x0006ee18
  00177bd0: str r0,[sp,#0x0]
  00177bd2: add r0,sp,#0x16c
  00177bd4: add r2,sp,#0xd0
  00177bd6: add r3,sp,#0xa0
  00177bd8: mov r1,r6
  00177bda: blx 0x000739b4
  00177bde: add r1,sp,#0x16c
  00177be0: mov r0,r5
  00177be2: blx 0x0006f2b0
  00177be6: ldr r6,[sp,#0xc]
  00177be8: ldr r0,[0x00177dd4]
  00177bea: add r0,pc
  00177bec: ldr r5,[r0,#0x0]
  00177bee: add r0,sp,#0x16c
  00177bf0: blx r5
  00177bf2: add r0,sp,#0x94
  00177bf4: blx r5
  00177bf6: add r0,sp,#0xa0
  00177bf8: blx r5
  00177bfa: add r0,sp,#0xd0
  00177bfc: blx r5
  00177bfe: add r0,sp,#0xac
  00177c00: blx r5
  00177c02: add r0,sp,#0xb8
  00177c04: blx r5
  00177c06: add r0,sp,#0xc4
  00177c08: blx r5
  00177c0a: mov.w r9,#0x172
  00177c0e: b 0x00177708
  00177c10: blx 0x0006f544
  00177c14: cmp r0,#0x1
  00177c16: bne 0x00177c34
  00177c18: ldr r0,[r4,#0x4c]
  00177c1a: blx 0x00073438
  00177c1e: cbz r0,0x00177c34
  00177c20: ldr r0,[r4,#0x4c]
  00177c22: ldr r5,[r4,#0x44]
  00177c24: blx 0x00073438
  00177c28: mov r1,r5
  00177c2a: mov r2,r0
  00177c2c: blx 0x00077c14
  00177c30: mov r1,r0
  00177c32: b 0x00177c4a
  00177c34: ldr r0,[0x00177f78]
  00177c36: movs r1,#0x5
  00177c38: add r0,pc
  00177c3a: ldr r0,[r0,#0x0]
  00177c3c: ldr r0,[r0,#0x0]
  00177c3e: blx 0x00071848
  00177c42: ldr r1,[0x00177f7c]
  00177c44: add r1,pc
  00177c46: ldr.w r1,[r1,r0,lsl #0x2]
  00177c4a: ldr r5,[sp,#0xc]
  00177c4c: mov r10,r1
  00177c4e: ldr r0,[r5,#0x0]
  00177c50: blx 0x00072f70
  00177c54: add.w r9,r4,#0x28
  00177c58: mov r1,r0
  00177c5a: mov r0,r9
  00177c5c: blx 0x0006f2b0
  00177c60: ldr r1,[0x00177f80]
  00177c62: cmp r6,#0x0
  00177c64: add r1,pc
  00177c66: beq 0x00177d1e
  00177c68: add r0,sp,#0x160
  00177c6a: movs r2,#0x0
  00177c6c: blx 0x0006ee18
  00177c70: ldr r0,[0x00177f84]
  00177c72: add r0,pc
  00177c74: ldr r0,[r0,#0x0]
  00177c76: ldr r6,[r0,#0x0]
  00177c78: ldr r0,[r5,#0x0]
  00177c7a: movw r1,#0x185
  00177c7e: blx 0x00072f70
  00177c82: mov r1,r0
  00177c84: add r0,sp,#0x88
  00177c86: movs r2,#0x0
  00177c88: blx 0x0006f028
  00177c8c: ldr r0,[r4,#0x4c]
  00177c8e: blx 0x00073438
  00177c92: mov r1,r0
  00177c94: add r0,sp,#0x7c
  00177c96: blx 0x00074c14
  00177c9a: ldr r1,[0x00177f88]
  00177c9c: add r1,pc
  00177c9e: add r0,sp,#0x70
  00177ca0: movs r2,#0x0
  00177ca2: blx 0x0006ee18
  00177ca6: str r0,[sp,#0x0]
  00177ca8: add r0,sp,#0x154
  00177caa: add r2,sp,#0x88
  00177cac: add r3,sp,#0x7c
  00177cae: mov r1,r6
  00177cb0: blx 0x000739b4
  00177cb4: add r0,sp,#0x16c
  00177cb6: add r1,sp,#0x160
  00177cb8: add r2,sp,#0x154
  00177cba: blx 0x0006ef98
  00177cbe: ldr r0,[0x00177f8c]
  00177cc0: add r0,pc
  00177cc2: ldr r6,[r0,#0x0]
  00177cc4: add r0,sp,#0x154
  00177cc6: blx r6
  00177cc8: add r0,sp,#0x70
  00177cca: blx r6
  00177ccc: add r0,sp,#0x7c
  00177cce: blx r6
  00177cd0: add r0,sp,#0x88
  00177cd2: blx r6
  00177cd4: add r0,sp,#0x160
  00177cd6: blx r6
  00177cd8: ldr r6,[sp,#0xc]
  00177cda: b 0x00177d40
  00177cdc: ldr r1,[0x00177f74]
  00177cde: add r1,pc
  00177ce0: ldr.w r9,[r1,r0,lsl #0x2]
  00177ce4: mov r10,r1
  00177ce6: ldr r0,[r6,#0x0]
  00177ce8: mov r1,r9
  00177cea: blx 0x00072f70
  00177cee: mov r5,r6
  00177cf0: add.w r6,r4,#0x28
  00177cf4: mov r1,r0
  00177cf6: mov r0,r6
  00177cf8: blx 0x0006f2b0
  00177cfc: ldr r0,[r4,#0x48]
  00177cfe: ldr r1,[0x00177f70]
  00177d00: ldr.w r2,[r10,r0,lsl #0x3]
  00177d04: add r1,pc
  00177d06: ldr.w r0,[r1,r2,lsl #0x2]
  00177d0a: addw r1,r2,#0x63d
  00177d0e: str r0,[r4,#0x24]
  00177d10: ldr r0,[r5,#0x0]
  00177d12: blx 0x00072f70
  00177d16: mov r1,r0
  00177d18: add.w r0,r4,#0x34
  00177d1c: b 0x00177a0a
  00177d1e: add r0,sp,#0x160
  00177d20: movs r2,#0x0
  00177d22: blx 0x0006ee18
  00177d26: ldr r6,[sp,#0xc]
  00177d28: ldr r0,[r6,#0x0]
  00177d2a: movs r1,#0xd8
  00177d2c: blx 0x00072f70
  00177d30: mov r2,r0
  00177d32: add r0,sp,#0x16c
  00177d34: add r1,sp,#0x160
  00177d36: blx 0x0006ef98
  00177d3a: add r0,sp,#0x160
  00177d3c: blx 0x0006ee30
  00177d40: add r0,sp,#0x160
  00177d42: add r2,sp,#0x16c
  00177d44: mov r1,r9
  00177d46: blx 0x0006ef98
  00177d4a: add r1,sp,#0x160
  00177d4c: mov r0,r9
  00177d4e: blx 0x0006f2b0
  00177d52: add r0,sp,#0x160
  00177d54: blx 0x0006ee30
  00177d58: add r0,sp,#0x16c
  00177d5a: blx 0x0006ee30
  00177d5e: mov r9,r10
  00177d60: b 0x00177708
```
