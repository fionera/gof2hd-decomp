# StarMap::drawOnScreenInfo
elf 0xc8354 body 3336
Sig: undefined __thiscall drawOnScreenInfo(StarMap * this, int param_1, bool param_2)

## decompile
```c

/* StarMap::drawOnScreenInfo(int, bool) */

void __thiscall StarMap::drawOnScreenInfo(StarMap *this,int param_1,bool param_2)

{
  byte bVar1;
  uchar uVar2;
  int iVar3;
  undefined8 *puVar4;
  uint *puVar5;
  Mission *this_00;
  uint *puVar6;
  uint uVar7;
  Agent *pAVar8;
  undefined4 *puVar9;
  Station *this_01;
  int iVar10;
  StarMap *pSVar11;
  int *piVar12;
  int iVar13;
  String *pSVar14;
  uchar uVar15;
  uchar uVar16;
  int *piVar17;
  code *pcVar18;
  int *piVar19;
  uint uVar20;
  SolarSystem *pSVar21;
  char cVar22;
  String *pSVar23;
  char *pcVar24;
  bool bVar25;
  uint in_fpscr;
  uint uVar26;
  float fVar27;
  int iVar28;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  undefined4 extraout_s1;
  undefined4 extraout_s1_00;
  undefined4 extraout_s1_01;
  undefined4 extraout_s1_02;
  undefined4 extraout_s1_03;
  undefined4 uVar29;
  undefined4 extraout_s1_04;
  undefined4 extraout_s1_05;
  float fVar30;
  float extraout_s2;
  float extraout_s2_00;
  float fVar31;
  undefined8 uVar32;
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  undefined8 local_48;
  undefined4 local_40;
  int local_3c;
  
  piVar12 = *(int **)(DAT_000d86f4 + 0xd836e);
  local_3c = *piVar12;
  if (param_2) {
    iVar3 = *(int *)(this + 0x198);
  }
  else {
    iVar3 = *(int *)(this + 0x194);
  }
  puVar4 = *(undefined8 **)(*(int *)(iVar3 + 4) + param_1 * 4);
  local_48 = *puVar4;
  local_40 = *(undefined4 *)(puVar4 + 1);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x78),(Vector *)&local_48);
  fVar30 = *(float *)(this + 0x78);
  in_fpscr = in_fpscr & 0xfffffff;
  fVar27 = DAT_000d86f8;
  uVar29 = extraout_s1;
  if (((fVar30 < DAT_000d86f8) ||
      (fVar31 = (float)VectorSignedToFloat(**(int **)(DAT_000d86fc + 0xd83b6) + 0x32,
                                           (byte)(in_fpscr >> 0x16) & 3), fVar31 < fVar30)) ||
     (fVar30 = *(float *)(this + 0x7c), fVar30 < DAT_000d86f8)) goto LAB_000d9078;
  fVar27 = (float)VectorSignedToFloat(**(int **)(DAT_000d8700 + 0xd83e4) + 0x32,
                                      (byte)(in_fpscr >> 0x16) & 3);
  uVar20 = in_fpscr | (uint)(fVar30 < fVar27) << 0x1f | (uint)(fVar30 == fVar27) << 0x1e;
  uVar26 = uVar20 | (uint)(NAN(fVar30) || NAN(fVar27)) << 0x1c;
  bVar1 = (byte)(uVar20 >> 0x18);
  if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(uVar26 >> 0x1c) & 1)) goto LAB_000d9078;
  puVar5 = *(uint **)(DAT_000d8704 + 0xd840a);
  AbyssEngine::PaintCanvas::SetColor(*puVar5);
  iVar3 = *(int *)(this + 0xfc);
  for (iVar13 = 0; iVar13 != 5; iVar13 = iVar13 + 1) {
    *(undefined4 *)(iVar3 + iVar13 * 4) = 0xffffffff;
  }
  if (param_2) {
    iVar3 = Station::isDiscovered(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
  }
  else {
    iVar3 = SolarSystem::isFullyDiscovered
                      (*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4));
  }
  if (iVar3 != 0) {
    **(undefined4 **)(this + 0xfc) = *(undefined4 *)(this + 0x30);
  }
  piVar17 = *(int **)(DAT_000d8708 + 0xd845a);
  iVar3 = Status::getCampaignMission();
  this_00 = (Mission *)Status::getFreelanceMission();
  iVar13 = Status::getCurrentCampaignMission();
  if (iVar13 == 0x34) {
    if (param_2) {
      iVar13 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
      if (iVar13 == 0x4a) {
LAB_000d84a2:
        *(undefined4 *)(*(int *)(this + 0xfc) + 4) = *(undefined4 *)(this + 0x24);
      }
    }
    else {
      iVar13 = SolarSystem::getStationEnumIndex
                         (*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4),0x4a)
      ;
      if (-1 < iVar13) goto LAB_000d84a2;
    }
  }
  iVar13 = Status::getCurrentCampaignMission();
  if (((iVar13 == 0x74 && param_2) &&
      (iVar13 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4)),
      0x59 < iVar13)) &&
     (iVar13 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4)),
     iVar13 < 0x5f)) {
    Status::getCampaignMission();
    uVar20 = Mission::getStatusValue();
    iVar13 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
    if ((1 << (iVar13 - 0x5aU & 0xff) & uVar20) != 0) goto LAB_000d84c0;
    goto LAB_000d8600;
  }
LAB_000d84c0:
  iVar13 = Status::getCurrentCampaignMission();
  if (iVar13 == 0x78) {
    if (param_2) {
      iVar13 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
      if (iVar13 != 0x5d) goto LAB_000d8548;
    }
    else {
      iVar13 = SolarSystem::getStationEnumIndex
                         (*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4),0x5d)
      ;
      if (iVar13 < 0) goto LAB_000d8548;
    }
LAB_000d8600:
    *(undefined4 *)(*(int *)(this + 0xfc) + 4) = *(undefined4 *)(this + 0x24);
  }
  else {
LAB_000d8548:
    iVar13 = Status::getCurrentCampaignMission();
    if (iVar13 == 0x7d && param_2) {
      iVar13 = *piVar17;
      Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
      iVar13 = Status::isFreighterMissionStation(iVar13);
      if (iVar13 != 0) {
        Status::getCampaignMission();
        uVar20 = Mission::getStatusValue();
        iVar13 = *piVar17;
        Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
        uVar7 = Status::getFreighterMissionStationBit(iVar13);
        if ((1 << (uVar7 & 0xff) & uVar20) == 0) goto LAB_000d8600;
      }
    }
    iVar13 = Status::getCurrentCampaignMission();
    if (iVar13 == 0x7d && !param_2) {
      iVar13 = *piVar17;
      SolarSystem::getWarpGateIndex
                (*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4));
      iVar13 = Status::isFreighterMissionStation(iVar13);
      if (iVar13 != 0) {
        Status::getCampaignMission();
        uVar20 = Mission::getStatusValue();
        iVar13 = *piVar17;
        SolarSystem::getWarpGateIndex
                  (*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4));
        uVar7 = Status::getFreighterMissionStationBit(iVar13);
        if ((1 << (uVar7 & 0xff) & uVar20) == 0) goto LAB_000d8600;
      }
    }
  }
  if (iVar3 != 0) {
    iVar3 = Mission::getType();
    if ((iVar3 == 0xa3) && (puVar6 = *(uint **)(*piVar17 + 0x90), puVar6 != (uint *)0x0)) {
      for (uVar20 = 0; uVar20 < *puVar6; uVar20 = uVar20 + 1) {
        if (param_2) {
          iVar13 = *(int *)(puVar6[1] + uVar20 * 4);
          iVar3 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4)
                                   );
          if (iVar13 == iVar3) {
LAB_000d86ee:
            bVar25 = true;
            goto LAB_000d8676;
          }
        }
        else {
          uVar7 = SolarSystem::getStationEnumIndex
                            (*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4),
                             *(int *)(puVar6[1] + uVar20 * 4));
          if (uVar7 < 0x80000000) goto LAB_000d86ee;
        }
        puVar6 = *(uint **)(*piVar17 + 0x90);
      }
    }
    bVar25 = false;
LAB_000d8676:
    iVar3 = Mission::isEmpty();
    if (((((iVar3 == 0) && (iVar3 = Status::getCurrentCampaignMission(), iVar3 != 0x34)) &&
         (iVar3 = Status::getCurrentCampaignMission(), iVar3 != 0x78)) &&
        ((iVar3 = Status::getCurrentCampaignMission(), iVar3 != 0x80 &&
         (iVar3 = Status::getCurrentCampaignMission(), iVar3 != 0x82)))) &&
       ((iVar3 = Status::getCurrentCampaignMission(), iVar3 < 0x94 ||
        (iVar3 = Status::getCurrentCampaignMission(), 0x97 < iVar3)))) {
      iVar3 = Status::getCurrentCampaignMission();
      if (iVar3 == 0x3b) {
        if (param_2) {
          iVar3 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4)
                                   );
          if (iVar3 != 0x65) goto LAB_000d8716;
        }
        else {
          iVar3 = SolarSystem::getIndex();
          if (iVar3 != 0x17) goto LAB_000d8754;
        }
      }
      else {
        if (param_2) {
LAB_000d8716:
          iVar3 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4)
                                   );
          iVar13 = Mission::getTargetStation();
          if (iVar3 != iVar13 && !bVar25) {
LAB_000d8788:
            iVar3 = Status::getCurrentCampaignMission();
            if (0x20 < iVar3) {
              if (param_2) {
                iVar3 = Station::getIndex(*(Station **)
                                           (*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
                if (iVar3 == *(int *)(*piVar17 + 0x80)) {
LAB_000d87ca:
                  iVar3 = Mission::getTargetStation();
                  if (iVar3 == -1) goto LAB_000d877c;
                }
              }
              else {
                iVar3 = SolarSystem::getStationEnumIndex
                                  (*(SolarSystem **)
                                    (*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4),
                                   *(int *)(*piVar17 + 0x80));
                if (-1 < iVar3) goto LAB_000d87ca;
              }
            }
            goto LAB_000d87d4;
          }
        }
        else {
LAB_000d8754:
          pSVar21 = *(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4);
          iVar3 = Mission::getTargetStation();
          uVar20 = SolarSystem::getStationEnumIndex(pSVar21,iVar3);
          if (!(bool)(bVar25 | uVar20 < 0x80000000)) goto LAB_000d8788;
        }
LAB_000d877c:
        *(undefined4 *)(*(int *)(this + 0xfc) + 4) = *(undefined4 *)(this + 0x24);
      }
    }
  }
LAB_000d87d4:
  if ((this_00 == (Mission *)0x0) || (iVar3 = Mission::isEmpty(), iVar3 != 0)) {
LAB_000d8868:
    if (param_2) {
LAB_000d886e:
      if (-1 < *(int *)(this + 0x60)) {
        iVar3 = SolarSystem::getWarpGateIndex
                          (*(SolarSystem **)
                            (*(int *)(*(int *)(this + 0x54) + 4) + *(int *)(this + 0x60) * 4));
        uVar32 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4))
        ;
        bVar25 = iVar3 == (int)uVar32;
        if (bVar25) {
          uVar32 = CONCAT44(*(undefined4 *)(this + 0xfc),*(undefined4 *)(this + 0x2c));
        }
        if (bVar25) {
          *(int *)((int)((ulonglong)uVar32 >> 0x20) + 0xc) = (int)uVar32;
        }
      }
    }
  }
  else {
    if (param_2) {
      iVar3 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
      iVar13 = Mission::getType();
      if (iVar13 == 0xe) {
        pAVar8 = (Agent *)Mission::getAgent(this_00);
        iVar13 = Agent::getStation(pAVar8);
      }
      else {
        iVar13 = Mission::getTargetStation();
      }
      if (iVar3 == iVar13) goto LAB_000d885e;
      goto LAB_000d886e;
    }
    pSVar21 = *(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4);
    iVar3 = Mission::getType();
    if (iVar3 == 0xe) {
      iVar3 = Status::getShip();
      iVar3 = Ship::hasCargo(iVar3,0x73);
      if (iVar3 == 0) goto LAB_000d8840;
      pAVar8 = (Agent *)Mission::getAgent(this_00);
      iVar3 = Agent::getStation(pAVar8);
    }
    else {
LAB_000d8840:
      iVar3 = Mission::getTargetStation();
    }
    iVar3 = SolarSystem::getStationEnumIndex(pSVar21,iVar3);
    if (-1 < iVar3) {
LAB_000d885e:
      *(undefined4 *)(*(int *)(this + 0xfc) + 8) = *(undefined4 *)(this + 0x28);
      goto LAB_000d8868;
    }
  }
  puVar6 = (uint *)Status::getPendingProducts((Status *)*piVar17);
  if (puVar6 != (uint *)0x0) {
    for (uVar20 = 0; uVar20 < *puVar6; uVar20 = uVar20 + 1) {
      iVar3 = *(int *)(puVar6[1] + uVar20 * 4);
      if (iVar3 != 0) {
        if (param_2) {
          iVar13 = *(int *)(iVar3 + 0xc);
          iVar3 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4)
                                   );
          if (iVar13 == iVar3) {
LAB_000d88f2:
            *(undefined4 *)(*(int *)(this + 0xfc) + 0x10) = *(undefined4 *)(this + 0x20);
            break;
          }
        }
        else {
          iVar3 = SolarSystem::getStationEnumIndex
                            (*(SolarSystem **)(*(int *)(*(int *)(this + 0x54) + 4) + param_1 * 4),
                             *(int *)(iVar3 + 0xc));
          if (-1 < iVar3) goto LAB_000d88f2;
        }
      }
    }
  }
  fVar27 = (float)VectorSignedToFloat(*(int *)(this + 0x1a8) >> 1,(byte)(uVar26 >> 0x16) & 3);
  fVar30 = *(float *)(this + 0x78);
  puVar9 = *(undefined4 **)(DAT_000d8c74 + 0xd8922);
  pSVar23 = (String *)*puVar9;
  uVar20 = *puVar5;
  iVar3 = (int)(*(float *)(this + 0x7c) + fVar27 + -3.0);
  bVar25 = SUB41(iVar3,0);
  if (param_2) {
    Station::getName();
    iVar3 = AbyssEngine::PaintCanvas::GetTextWidth(uVar20,pSVar23);
    AbyssEngine::String::~String((String *)&local_48);
    fVar27 = (float)VectorSignedToFloat(iVar3 / 2,(byte)(uVar26 >> 0x16) & 3);
    this_01 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(this_01);
    iVar28 = (int)(fVar30 - fVar27);
    iVar13 = Station::getIndex(*(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + param_1 * 4));
    if (iVar3 == iVar13) {
      iVar3 = *(int *)(this + 0x1a4);
      uVar20 = *puVar5;
      Layout::getPulseValue((Layout *)**(undefined4 **)(DAT_000d8c78 + 0xd89aa),extraout_s0);
      VectorSignedToFloat(0xff - iVar3,(byte)(uVar26 >> 0x16) & 3);
      AbyssEngine::PaintCanvas::SetColor((uchar)uVar20,0xff,0xff,0xff);
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar5,*(int *)(this + 0x48),(int)*(float *)(this + 0x78),
                 (uchar)(int)*(float *)(this + 0x7c),'\x11');
    }
    if (*(int *)(this + 100) == param_1) {
      AbyssEngine::PaintCanvas::SetColor((uchar)*puVar5,0xff,0x80,'\0');
      uVar20 = *puVar5;
      pSVar23 = (String *)*puVar9;
      Station::getName();
      AbyssEngine::PaintCanvas::DrawString(uVar20,pSVar23,(int)&local_48,iVar28,bVar25);
      AbyssEngine::String::~String((String *)&local_48);
      AbyssEngine::PaintCanvas::SetColor((uchar)*puVar5,0xff,0xff,0xff);
      iVar3 = Station::getTecLevel();
      if (0 < iVar3) {
        uVar20 = *puVar5;
        pSVar14 = (String *)*puVar9;
        pSVar23 = (String *)GameText::getText(**(int **)(DAT_000d8c84 + 0xd8bb4));
        AbyssEngine::String::String(aSStack_60,(char *)(DAT_000d8c88 + 0xd8bd0),false);
        AbyssEngine::operator+(aSStack_54,pSVar23);
        iVar3 = Station::getTecLevel();
        AbyssEngine::String::String(aSStack_78,iVar3);
        AbyssEngine::String::String(aSStack_6c,aSStack_78,false);
        AbyssEngine::operator+((String *)&local_48,aSStack_54);
        AbyssEngine::PaintCanvas::DrawString
                  (uVar20,pSVar14,(int)&local_48,iVar28,
                   (bool)((char)*(undefined4 *)(**(int **)(DAT_000d8c8c + 0xd8c0e) + 4) + bVar25));
        pcVar18 = *(code **)(DAT_000d8c90 + 0xd8c2e);
        (*pcVar18)(&local_48);
        (*pcVar18)(aSStack_6c);
        (*pcVar18)(aSStack_78);
        (*pcVar18)(aSStack_54);
        (*pcVar18)(aSStack_60);
      }
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar5,*(int *)(this + 0x40),(int)*(float *)(this + 0x78),
                 (uchar)(int)*(float *)(this + 0x7c),'\x11');
      uVar29 = extraout_s1_00;
    }
    else {
      AbyssEngine::PaintCanvas::SetColor((uchar)*puVar5,0xff,0xff,0xff);
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar5,*(int *)(this + 0x44),(int)*(float *)(this + 0x78),
                 (uchar)(int)*(float *)(this + 0x7c),'\x11');
      if (-1 < *(int *)(this + 100)) {
        AbyssEngine::PaintCanvas::SetColor((uchar)*puVar5,0xff,0xff,0xff);
      }
      uVar20 = *puVar5;
      pSVar23 = (String *)*puVar9;
      Station::getName();
      AbyssEngine::PaintCanvas::DrawString(uVar20,pSVar23,(int)&local_48,iVar28,bVar25);
      AbyssEngine::String::~String((String *)&local_48);
      uVar29 = extraout_s1_01;
    }
  }
  else {
    SolarSystem::getName();
    iVar13 = AbyssEngine::PaintCanvas::GetTextWidth(uVar20,pSVar23);
    AbyssEngine::String::~String((String *)&local_48);
    fVar27 = (float)VectorSignedToFloat(iVar13 / 2,(byte)(uVar26 >> 0x16) & 3);
    iVar13 = (int)(fVar30 - fVar27);
    if (this[0xa8] != (StarMap)0x0) {
      SolarSystem::getIndex();
    }
    Status::getSystem();
    iVar28 = SolarSystem::getIndex();
    iVar10 = SolarSystem::getIndex();
    if (iVar28 == iVar10) {
      VectorSignedToFloat(*(float *)(this + 0x1a4),(byte)(uVar26 >> 0x16) & 3);
      uVar20 = *puVar5;
      Layout::getPulseValue
                ((Layout *)**(undefined4 **)(DAT_000d8c80 + 0xd8a96),*(float *)(this + 0x1a4));
      AbyssEngine::PaintCanvas::SetColor((uchar)uVar20,0xff,0xff,0xff);
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar5,*(int *)(this + 0x48),(int)*(float *)(this + 0x78),
                 (uchar)(int)*(float *)(this + 0x7c),'\x11');
    }
    AbyssEngine::PaintCanvas::SetColor((uchar)*puVar5,0xff,0xff,0xff);
    if (*(int *)(this + 0x60) == param_1) {
      uVar15 = 0x80;
      uVar2 = (uchar)*puVar5;
      uVar16 = '\0';
LAB_000d8cae:
      AbyssEngine::PaintCanvas::SetColor(uVar2,0xff,uVar15,uVar16);
    }
    else if (-1 < *(int *)(this + 0x60)) {
      uVar15 = 0xff;
      uVar16 = 0xff;
      uVar2 = (uchar)*puVar5;
      goto LAB_000d8cae;
    }
    uVar20 = *puVar5;
    pSVar23 = (String *)*puVar9;
    SolarSystem::getName();
    AbyssEngine::PaintCanvas::DrawString(uVar20,pSVar23,(int)&local_48,iVar13,bVar25);
    AbyssEngine::String::~String((String *)&local_48);
    AbyssEngine::PaintCanvas::SetColor((uchar)*puVar5,0xff,0xff,0xff);
    iVar28 = SolarSystem::hasNoOwner();
    if (iVar28 == 0) {
      iVar28 = SolarSystem::getRace();
      if (iVar28 == 2) {
        pSVar11 = this + 300;
      }
      else if (iVar28 == 1) {
        pSVar11 = this + 0x128;
      }
      else if (iVar28 == 0) {
        pSVar11 = this + 0x124;
      }
      else {
        pSVar11 = this + 0x130;
      }
      AbyssEngine::PaintCanvas::DrawImage2D(*puVar5,*(int *)pSVar11,iVar13,bVar25 + 0xfd,'\x11');
    }
    if ((param_1 == 0x1a) && (*(int *)(*piVar17 + 0x114) == 3)) {
      fVar27 = (float)VectorSignedToFloat(*(undefined4 *)(**(int **)(DAT_000d90dc + 0xd8e26) + 0x2c)
                                          ,(byte)(uVar26 >> 0x16) & 3);
      fVar30 = (float)VectorSignedToFloat(iVar3,(byte)(uVar26 >> 0x16) & 3);
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar5,*(int *)(this + 0x134),iVar13,(uchar)(int)(fVar30 + fVar27 * 0.5),'\x11');
    }
    iVar3 = *(int *)(this + 0x60);
    AbyssEngine::PaintCanvas::SetColor((uchar)*puVar5,0xff,0xff,0xff);
    if (iVar3 == param_1) {
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar5,*(int *)(this + 0x40),(int)*(float *)(this + 0x78),
                 (uchar)(int)*(float *)(this + 0x7c),'\x11');
      piVar19 = *(int **)(DAT_000d90e0 + 0xd8ea8);
      cVar22 = (char)*(undefined4 *)(*piVar19 + 4);
      iVar3 = SolarSystem::hasNoOwner();
      if (iVar3 == 0) {
        uVar20 = *puVar5;
        iVar3 = **(int **)(DAT_000d90e4 + 0xd8ece);
        pSVar23 = (String *)*puVar9;
        SolarSystem::getRace();
        iVar3 = GameText::getText(iVar3);
        AbyssEngine::PaintCanvas::DrawString
                  (uVar20,pSVar23,iVar3,iVar13,(bool)((char)*(undefined4 *)(*piVar19 + 4) + bVar25))
        ;
        cVar22 = (char)(*(int *)(*piVar19 + 4) << 1);
      }
      iVar3 = SolarSystem::getSecurityLevel
                        (*(SolarSystem **)
                          (*(int *)(*(int *)(this + 0x54) + 4) + *(int *)(this + 0x60) * 4));
      if ((param_1 == 0x1a) && (1 < *(int *)(*piVar17 + 0x114))) {
        iVar3 = 3;
      }
      iVar28 = DAT_000d90e8 + 0xd8f44 + iVar3 * 0xc;
      AbyssEngine::PaintCanvas::SetColor
                ((uchar)*puVar5,*(uchar *)(DAT_000d90e8 + 0xd8f44 + iVar3 * 0xc),
                 *(uchar *)(iVar28 + 4),*(uchar *)(iVar28 + 8));
      uVar20 = *puVar5;
      pSVar23 = (String *)*puVar9;
      iVar3 = GameText::getText(**(int **)(DAT_000d90ec + 0xd8f64));
      AbyssEngine::PaintCanvas::DrawString(uVar20,pSVar23,iVar3,iVar13,(bool)(bVar25 + cVar22));
      uVar2 = (uchar)*puVar5;
    }
    else {
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar5,*(int *)(this + 0x44),(int)*(float *)(this + 0x78),
                 (uchar)(int)*(float *)(this + 0x7c),'\x11');
      uVar29 = extraout_s1_02;
      if (*(int *)(this + 0x60) < 0) goto LAB_000d8fe0;
      uVar2 = (uchar)*puVar5;
    }
    AbyssEngine::PaintCanvas::SetColor(uVar2,0xff,0xff,0xff);
    uVar29 = extraout_s1_03;
  }
LAB_000d8fe0:
  iVar3 = 0;
  fVar27 = (float)VectorSignedToFloat(*(int *)(this + 0x1a8) >> 1,(byte)(uVar26 >> 0x16) & 3);
  fVar30 = (float)(int)((*(float *)(this + 0x7c) - fVar27) + 10.0);
  fVar31 = (float)(int)(*(float *)(this + 0x78) + fVar27 + -7.0);
  pcVar24 = *(char **)(DAT_000d90f0 + 0xd901e);
  fVar27 = fVar31;
  for (; iVar3 != 5; iVar3 = iVar3 + 1) {
    iVar13 = *(int *)(*(int *)(this + 0xfc) + iVar3 * 4);
    if (iVar13 != -1) {
      if (iVar3 == 0) {
        iVar28 = 0x12;
        if (*pcVar24 != '\0') {
          iVar28 = 0xc;
        }
        AbyssEngine::PaintCanvas::DrawImage2D(*puVar5,iVar13,(int)fVar31 - iVar28);
        fVar27 = extraout_s0_01;
        fVar30 = extraout_s2_00;
        uVar29 = extraout_s1_05;
      }
      else {
        AbyssEngine::PaintCanvas::DrawImage2D(*puVar5,iVar13,(int)fVar31);
        fVar27 = extraout_s0_00;
        fVar30 = extraout_s2;
        uVar29 = extraout_s1_04;
      }
    }
  }
LAB_000d9078:
  if (*piVar12 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(fVar27,uVar29,fVar30);
  }
  return;
}

```

## target disasm
```
  000d8354: push {r4,r5,r6,r7,lr}
  000d8356: add r7,sp,#0xc
  000d8358: push {r7,r8,r9,r10,r11}
  000d835c: vpush {d8,d9}
  000d8360: sub sp,#0x68
  000d8362: mov r9,r0
  000d8364: ldr r0,[0x000d86f4]
  000d8366: mov r10,r1
  000d8368: cmp r2,#0x0
  000d836a: add r0,pc
  000d836c: mov r11,r2
  000d836e: ldr r1,[r0,#0x0]
  000d8370: ldr r0,[r1,#0x0]
  000d8372: str r0,[sp,#0x64]
  000d8374: str r1,[sp,#0x24]
  000d8376: add r1,sp,#0x58
  000d8378: ite eq
  000d837a: ldr.eq.w r0,[r9,#0x194]
  000d837e: ldr.ne.w r0,[r9,#0x198]
  000d8382: ldr r0,[r0,#0x4]
  000d8384: ldr.w r0,[r0,r10,lsl #0x2]
  000d8388: vldr.64 d16,[r0]
  000d838c: ldr r0,[r0,#0x8]
  000d838e: str r0,[sp,#0x60]
  000d8390: add.w r0,r9,#0x78
  000d8394: vstr.64 d16,[sp,#0x58]
  000d8398: blx 0x0006eb3c
  000d839c: vldr.32 s0,[pc,#0x358]
  000d83a0: vldr.32 s2,[r9,#0x78]
  000d83a4: vcmpe.f32 s2,s0
  000d83a8: vmrs apsr,fpscr
  000d83ac: bmi.w 0x000d9078
  000d83b0: ldr r0,[0x000d86fc]
  000d83b2: add r0,pc
  000d83b4: ldr r0,[r0,#0x0]
  000d83b6: ldr r0,[r0,#0x0]
  000d83b8: adds r0,#0x32
  000d83ba: vmov s4,r0
  000d83be: vcvt.f32.s32 s4,s4
  000d83c2: vcmpe.f32 s2,s4
  000d83c6: vmrs apsr,fpscr
  000d83ca: bgt.w 0x000d9078
  000d83ce: vldr.32 s2,[r9,#0x7c]
  000d83d2: vcmpe.f32 s2,s0
  000d83d6: vmrs apsr,fpscr
  000d83da: bmi.w 0x000d9078
  000d83de: ldr r0,[0x000d8700]
  000d83e0: add r0,pc
  000d83e2: ldr r0,[r0,#0x0]
  000d83e4: ldr r0,[r0,#0x0]
  000d83e6: adds r0,#0x32
  000d83e8: vmov s0,r0
  000d83ec: vcvt.f32.s32 s0,s0
  000d83f0: vcmpe.f32 s2,s0
  000d83f4: vmrs apsr,fpscr
  000d83f8: bgt.w 0x000d9078
  000d83fc: ldr r0,[0x000d8704]
  000d83fe: mov.w r1,#0xffffffff
  000d8402: mov.w r4,#0xffffffff
  000d8406: add r0,pc
  000d8408: ldr r0,[r0,#0x0]
  000d840a: str r0,[sp,#0x1c]
  000d840c: ldr r0,[r0,#0x0]
  000d840e: blx 0x0006fac0
  000d8412: ldr.w r0,[r9,#0xfc]
  000d8416: movs r1,#0x0
  000d8418: b 0x000d8420
  000d841a: str.w r4,[r0,r1,lsl #0x2]
  000d841e: adds r1,#0x1
  000d8420: cmp r1,#0x5
  000d8422: bne 0x000d841a
  000d8424: cmp.w r11,#0x0
  000d8428: beq 0x000d843a
  000d842a: ldr.w r0,[r9,#0x58]
  000d842e: ldr r0,[r0,#0x4]
  000d8430: ldr.w r0,[r0,r10,lsl #0x2]
  000d8434: blx 0x0007348c
  000d8438: b 0x000d8448
  000d843a: ldr.w r0,[r9,#0x54]
  000d843e: ldr r0,[r0,#0x4]
  000d8440: ldr.w r0,[r0,r10,lsl #0x2]
  000d8444: blx 0x00074638
  000d8448: cbz r0,0x000d8454
  000d844a: ldr.w r1,[r9,#0xfc]
  000d844e: ldr.w r0,[r9,#0x30]
  000d8452: str r0,[r1,#0x0]
  000d8454: ldr r0,[0x000d8708]
  000d8456: add r0,pc
  000d8458: ldr r4,[r0,#0x0]
  000d845a: ldr r0,[r4,#0x0]
  000d845c: blx 0x0007285c
  000d8460: mov r6,r0
  000d8462: ldr r0,[r4,#0x0]
  000d8464: blx 0x00073444
  000d8468: str r0,[sp,#0x14]
  000d846a: ldr r0,[r4,#0x0]
  000d846c: blx 0x00071770
  000d8470: cmp r0,#0x34
  000d8472: bne 0x000d84ac
  000d8474: cmp.w r11,#0x0
  000d8478: beq 0x000d848e
  000d847a: ldr.w r0,[r9,#0x58]
  000d847e: ldr r0,[r0,#0x4]
  000d8480: ldr.w r0,[r0,r10,lsl #0x2]
  000d8484: blx 0x00071824
  000d8488: cmp r0,#0x4a
  000d848a: beq 0x000d84a2
  000d848c: b 0x000d84ac
  000d848e: ldr.w r0,[r9,#0x54]
  000d8492: movs r1,#0x4a
  000d8494: ldr r0,[r0,#0x4]
  000d8496: ldr.w r0,[r0,r10,lsl #0x2]
  000d849a: blx 0x00074644
  000d849e: cmp r0,#0x0
  000d84a0: blt 0x000d84ac
  000d84a2: ldr.w r1,[r9,#0xfc]
  000d84a6: ldr.w r0,[r9,#0x24]
  000d84aa: str r0,[r1,#0x4]
  000d84ac: ldr r0,[r4,#0x0]
  000d84ae: blx 0x00071770
  000d84b2: eor r5,r11,#0x1
  000d84b6: cmp r0,#0x74
  000d84b8: str r6,[sp,#0x18]
  000d84ba: it eq
  000d84bc: cmp.eq r5,#0x0
  000d84be: beq 0x000d84e4
  000d84c0: ldr r0,[r4,#0x0]
  000d84c2: blx 0x00071770
  000d84c6: cmp r0,#0x78
  000d84c8: bne 0x000d8548
  000d84ca: cmp.w r11,#0x0
  000d84ce: beq 0x000d8534
  000d84d0: ldr.w r0,[r9,#0x58]
  000d84d4: ldr r0,[r0,#0x4]
  000d84d6: ldr.w r0,[r0,r10,lsl #0x2]
  000d84da: blx 0x00071824
  000d84de: cmp r0,#0x5d
  000d84e0: bne 0x000d8548
  000d84e2: b 0x000d8600
  000d84e4: ldr.w r0,[r9,#0x58]
  000d84e8: ldr r0,[r0,#0x4]
  000d84ea: ldr.w r0,[r0,r10,lsl #0x2]
  000d84ee: blx 0x00071824
  000d84f2: cmp r0,#0x5a
  000d84f4: blt 0x000d84c0
  000d84f6: ldr.w r0,[r9,#0x58]
  000d84fa: ldr r0,[r0,#0x4]
  000d84fc: ldr.w r0,[r0,r10,lsl #0x2]
  000d8500: blx 0x00071824
  000d8504: cmp r0,#0x5e
  000d8506: bgt 0x000d84c0
  000d8508: ldr r0,[r4,#0x0]
  000d850a: blx 0x0007285c
  000d850e: blx 0x0007288c
  000d8512: mov r8,r0
  000d8514: ldr.w r0,[r9,#0x58]
  000d8518: ldr r0,[r0,#0x4]
  000d851a: ldr.w r0,[r0,r10,lsl #0x2]
  000d851e: blx 0x00071824
  000d8522: subs r0,#0x5a
  000d8524: ldr r6,[sp,#0x18]
  000d8526: movs r1,#0x1
  000d8528: lsl.w r0,r1,r0
  000d852c: tst.w r0,r8
  000d8530: bne 0x000d84c0
  000d8532: b 0x000d8600
  000d8534: ldr.w r0,[r9,#0x54]
  000d8538: movs r1,#0x5d
  000d853a: ldr r0,[r0,#0x4]
  000d853c: ldr.w r0,[r0,r10,lsl #0x2]
  000d8540: blx 0x00074644
  000d8544: cmp r0,#0x0
  000d8546: bge 0x000d8600
  000d8548: ldr r0,[r4,#0x0]
  000d854a: blx 0x00071770
  000d854e: subs r0,#0x7d
  000d8550: it ne
  000d8552: mov.ne r0,#0x1
  000d8554: orrs r0,r5
  000d8556: bne 0x000d85a4
  000d8558: ldr.w r0,[r9,#0x58]
  000d855c: ldr r5,[r4,#0x0]
  000d855e: ldr r0,[r0,#0x4]
  000d8560: ldr.w r0,[r0,r10,lsl #0x2]
  000d8564: blx 0x00071824
  000d8568: mov r1,r0
  000d856a: mov r0,r5
  000d856c: blx 0x00074650
  000d8570: cbz r0,0x000d85a4
  000d8572: ldr r0,[r4,#0x0]
  000d8574: blx 0x0007285c
  000d8578: blx 0x0007288c
  000d857c: mov r8,r0
  000d857e: ldr.w r0,[r9,#0x58]
  000d8582: ldr r5,[r4,#0x0]
  000d8584: ldr r0,[r0,#0x4]
  000d8586: ldr.w r0,[r0,r10,lsl #0x2]
  000d858a: blx 0x00071824
  000d858e: mov r1,r0
  000d8590: mov r0,r5
  000d8592: blx 0x0007465c
  000d8596: ldr r6,[sp,#0x18]
  000d8598: movs r1,#0x1
  000d859a: lsl.w r0,r1,r0
  000d859e: tst.w r0,r8
  000d85a2: beq 0x000d8600
  000d85a4: ldr r0,[r4,#0x0]
  000d85a6: blx 0x00071770
  000d85aa: cmp r0,#0x7d
  000d85ac: it eq
  000d85ae: cmp.eq.w r11,#0x0
  000d85b2: bne 0x000d860a
  000d85b4: ldr.w r0,[r9,#0x54]
  000d85b8: ldr r5,[r4,#0x0]
  000d85ba: ldr r0,[r0,#0x4]
  000d85bc: ldr.w r0,[r0,r10,lsl #0x2]
  000d85c0: blx 0x000737f8
  000d85c4: mov r1,r0
  000d85c6: mov r0,r5
  000d85c8: blx 0x00074650
  000d85cc: cbz r0,0x000d860a
  000d85ce: ldr r0,[r4,#0x0]
  000d85d0: blx 0x0007285c
  000d85d4: blx 0x0007288c
  000d85d8: mov r8,r0
  000d85da: ldr.w r0,[r9,#0x54]
  000d85de: ldr r5,[r4,#0x0]
  000d85e0: ldr r0,[r0,#0x4]
  000d85e2: ldr.w r0,[r0,r10,lsl #0x2]
  000d85e6: blx 0x000737f8
  000d85ea: mov r1,r0
  000d85ec: mov r0,r5
  000d85ee: blx 0x0007465c
  000d85f2: ldr r6,[sp,#0x18]
  000d85f4: movs r1,#0x1
  000d85f6: lsl.w r0,r1,r0
  000d85fa: tst.w r0,r8
  000d85fe: bne 0x000d860a
  000d8600: ldr.w r1,[r9,#0xfc]
  000d8604: ldr.w r0,[r9,#0x24]
  000d8608: str r0,[r1,#0x4]
  000d860a: cmp r6,#0x0
  000d860c: str r4,[sp,#0x20]
  000d860e: beq.w 0x000d87d4
  000d8612: ldr r6,[sp,#0x18]
  000d8614: mov r0,r6
  000d8616: blx 0x00072874
  000d861a: cmp r0,#0xa3
  000d861c: bne 0x000d8674
  000d861e: ldr r0,[r4,#0x0]
  000d8620: ldr.w r0,[r0,#0x90]
  000d8624: cbz r0,0x000d8674
  000d8626: movs r5,#0x0
  000d8628: b 0x000d866e
  000d862a: cmp.w r11,#0x0
  000d862e: beq 0x000d864c
  000d8630: ldr.w r1,[r9,#0x58]
  000d8634: ldr r0,[r0,#0x4]
  000d8636: ldr r1,[r1,#0x4]
  000d8638: ldr.w r4,[r0,r5,lsl #0x2]
  000d863c: ldr.w r0,[r1,r10,lsl #0x2]
  000d8640: blx 0x00071824
  000d8644: cmp r4,r0
  000d8646: ldr r4,[sp,#0x20]
  000d8648: bne 0x000d8666
  000d864a: b 0x000d86ee
  000d864c: ldr.w r1,[r9,#0x54]
  000d8650: ldr r0,[r0,#0x4]
  000d8652: ldr r2,[r1,#0x4]
  000d8654: ldr.w r1,[r0,r5,lsl #0x2]
  000d8658: ldr.w r0,[r2,r10,lsl #0x2]
  000d865c: blx 0x00074644
  000d8660: cmp.w r0,#0xffffffff
  000d8664: bgt 0x000d86ee
  000d8666: ldr r0,[r4,#0x0]
  000d8668: adds r5,#0x1
  000d866a: ldr.w r0,[r0,#0x90]
  000d866e: ldr r1,[r0,#0x0]
  000d8670: cmp r5,r1
  000d8672: bcc 0x000d862a
  000d8674: movs r5,#0x0
  000d8676: mov r0,r6
  000d8678: blx 0x00072868
  000d867c: cmp r0,#0x0
  000d867e: bne.w 0x000d87d4
  000d8682: ldr r0,[r4,#0x0]
  000d8684: blx 0x00071770
  000d8688: cmp r0,#0x34
  000d868a: beq.w 0x000d87d4
  000d868e: ldr r0,[r4,#0x0]
  000d8690: blx 0x00071770
  000d8694: cmp r0,#0x78
  000d8696: beq.w 0x000d87d4
  000d869a: ldr r0,[r4,#0x0]
  000d869c: blx 0x00071770
  000d86a0: cmp r0,#0x80
  000d86a2: beq.w 0x000d87d4
  000d86a6: ldr r0,[r4,#0x0]
  000d86a8: blx 0x00071770
  000d86ac: cmp r0,#0x82
  000d86ae: beq.w 0x000d87d4
  000d86b2: ldr r0,[r4,#0x0]
  000d86b4: blx 0x00071770
  000d86b8: cmp r0,#0x94
  000d86ba: blt 0x000d86c8
  000d86bc: ldr r0,[r4,#0x0]
  000d86be: blx 0x00071770
  000d86c2: cmp r0,#0x98
  000d86c4: blt.w 0x000d87d4
  000d86c8: ldr r0,[r4,#0x0]
  000d86ca: blx 0x00071770
  000d86ce: cmp r0,#0x3b
  000d86d0: bne 0x000d870c
  000d86d2: mov r8,r9
  000d86d4: cmp.w r11,#0x0
  000d86d8: beq 0x000d873c
  000d86da: ldr.w r0,[r8,#0x58]!
  000d86de: ldr r0,[r0,#0x4]
  000d86e0: ldr.w r0,[r0,r10,lsl #0x2]
  000d86e4: blx 0x00071824
  000d86e8: cmp r0,#0x65
  000d86ea: bne 0x000d8716
  000d86ec: b 0x000d87d4
  000d86ee: movs r5,#0x1
  000d86f0: b 0x000d8676
  000d870c: cmp.w r11,#0x0
  000d8710: beq 0x000d8750
  000d8712: add.w r8,r9,#0x58
  000d8716: ldr.w r0,[r8,#0x0]
  000d871a: ldr r0,[r0,#0x4]
  000d871c: ldr.w r0,[r0,r10,lsl #0x2]
  000d8720: blx 0x00071824
  000d8724: mov r8,r0
  000d8726: ldr r0,[sp,#0x18]
  000d8728: blx 0x00073738
  000d872c: sub.w r0,r8,r0
  000d8730: clz r0,r0
  000d8734: lsrs r0,r0,#0x5
  000d8736: orrs r0,r5
  000d8738: bne 0x000d877c
  000d873a: b 0x000d8788
  000d873c: ldr.w r0,[r8,#0x54]!
  000d8740: ldr r0,[r0,#0x4]
  000d8742: ldr.w r0,[r0,r10,lsl #0x2]
  000d8746: blx 0x00071a7c
  000d874a: cmp r0,#0x17
  000d874c: bne 0x000d8754
  000d874e: b 0x000d87d4
  000d8750: add.w r8,r9,#0x54
  000d8754: ldr.w r0,[r8,#0x0]
  000d8758: ldr r0,[r0,#0x4]
  000d875a: ldr.w r8,[r0,r10,lsl #0x2]
  000d875e: ldr r0,[sp,#0x18]
  000d8760: blx 0x00073738
  000d8764: mov r1,r0
  000d8766: mov r0,r8
  000d8768: blx 0x00074644
  000d876c: movs r1,#0x0
  000d876e: cmp.w r0,#0xffffffff
  000d8772: it gt
  000d8774: mov.gt r1,#0x1
  000d8776: orrs.w r0,r5,r1
  000d877a: beq 0x000d8788
  000d877c: ldr.w r1,[r9,#0xfc]
  000d8780: ldr.w r0,[r9,#0x24]
  000d8784: str r0,[r1,#0x4]
  000d8786: b 0x000d87d4
  000d8788: ldr r0,[r4,#0x0]
  000d878a: blx 0x00071770
  000d878e: cmp r0,#0x21
  000d8790: blt 0x000d87d4
  000d8792: cmp.w r11,#0x0
  000d8796: beq 0x000d87b2
  000d8798: ldr.w r0,[r9,#0x58]
  000d879c: ldr r0,[r0,#0x4]
  000d879e: ldr.w r0,[r0,r10,lsl #0x2]
  000d87a2: blx 0x00071824
  000d87a6: ldr r1,[r4,#0x0]
  000d87a8: ldr.w r1,[r1,#0x80]
  000d87ac: cmp r0,r1
  000d87ae: beq 0x000d87ca
  000d87b0: b 0x000d87d4
  000d87b2: ldr.w r0,[r9,#0x54]
  000d87b6: ldr r1,[r4,#0x0]
  000d87b8: ldr r0,[r0,#0x4]
  000d87ba: ldr.w r1,[r1,#0x80]
  000d87be: ldr.w r0,[r0,r10,lsl #0x2]
  000d87c2: blx 0x00074644
  000d87c6: cmp r0,#0x0
  000d87c8: blt 0x000d87d4
  000d87ca: ldr r0,[sp,#0x18]
  000d87cc: blx 0x00073738
  000d87d0: adds r0,#0x1
  000d87d2: beq 0x000d877c
  000d87d4: ldr r6,[sp,#0x14]
  000d87d6: cmp r6,#0x0
  000d87d8: beq 0x000d8868
  000d87da: mov r0,r6
  000d87dc: blx 0x00072868
  000d87e0: cmp r0,#0x0
  000d87e2: bne 0x000d8868
  000d87e4: cmp.w r11,#0x0
  000d87e8: beq 0x000d8810
  000d87ea: ldr.w r0,[r9,#0x58]
  000d87ee: ldr r0,[r0,#0x4]
  000d87f0: ldr.w r0,[r0,r10,lsl #0x2]
  000d87f4: blx 0x00071824
  000d87f8: mov r5,r0
  000d87fa: mov r0,r6
  000d87fc: blx 0x00072874
  000d8800: cmp r0,#0xe
  000d8802: bne 0x000d8854
  000d8804: mov r0,r6
  000d8806: blx 0x00073438
  000d880a: blx 0x00071ac4
  000d880e: b 0x000d885a
  000d8810: ldr.w r0,[r9,#0x54]
  000d8814: ldr r0,[r0,#0x4]
  000d8816: ldr.w r5,[r0,r10,lsl #0x2]
  000d881a: mov r0,r6
  000d881c: blx 0x00072874
  000d8820: cmp r0,#0xe
  000d8822: bne 0x000d8840
  000d8824: ldr r0,[r4,#0x0]
  000d8826: blx 0x00071a58
  000d882a: movs r1,#0x73
  000d882c: movs r2,#0x1
  000d882e: blx 0x00071a70
  000d8832: cbz r0,0x000d8840
  000d8834: mov r0,r6
  000d8836: blx 0x00073438
  000d883a: blx 0x00071ac4
  000d883e: b 0x000d8846
  000d8840: mov r0,r6
  000d8842: blx 0x00073738
  000d8846: mov r1,r0
  000d8848: mov r0,r5
  000d884a: blx 0x00074644
  000d884e: cmp r0,#0x0
  000d8850: bge 0x000d885e
  000d8852: b 0x000d88a2
  000d8854: mov r0,r6
  000d8856: blx 0x00073738
  000d885a: cmp r5,r0
  000d885c: bne 0x000d886e
  000d885e: ldr.w r1,[r9,#0xfc]
  000d8862: ldr.w r0,[r9,#0x28]
  000d8866: str r0,[r1,#0x8]
  000d8868: cmp.w r11,#0x0
  000d886c: beq 0x000d88a2
  000d886e: ldr.w r0,[r9,#0x60]
  000d8872: cmp r0,#0x0
  000d8874: blt 0x000d88a2
  000d8876: ldr.w r1,[r9,#0x54]
  000d887a: ldr r1,[r1,#0x4]
  000d887c: ldr.w r0,[r1,r0,lsl #0x2]
  000d8880: blx 0x000737f8
  000d8884: mov r5,r0
  000d8886: ldr.w r0,[r9,#0x58]
  000d888a: ldr r0,[r0,#0x4]
  000d888c: ldr.w r0,[r0,r10,lsl #0x2]
  000d8890: blx 0x00071824
  000d8894: cmp r5,r0
  000d8896: ittt eq
  000d8898: ldr.eq.w r0,[r9,#0x2c]
  000d889c: ldr.eq.w r1,[r9,#0xfc]
  000d88a0: str.eq r0,[r1,#0xc]
  000d88a2: ldr r0,[r4,#0x0]
  000d88a4: blx 0x00074668
  000d88a8: mov r5,r0
  000d88aa: cbz r0,0x000d88fc
  000d88ac: movs r4,#0x0
  000d88ae: b 0x000d88ea
  000d88b0: ldr r0,[r5,#0x4]
  000d88b2: ldr.w r0,[r0,r4,lsl #0x2]
  000d88b6: cbz r0,0x000d88e8
  000d88b8: cmp.w r11,#0x0
  000d88bc: beq 0x000d88d4
  000d88be: ldr.w r1,[r9,#0x58]
  000d88c2: ldr r6,[r0,#0xc]
  000d88c4: ldr r1,[r1,#0x4]
  000d88c6: ldr.w r0,[r1,r10,lsl #0x2]
  000d88ca: blx 0x00071824
  000d88ce: cmp r6,r0
  000d88d0: bne 0x000d88e8
  000d88d2: b 0x000d88f2
  000d88d4: ldr.w r1,[r9,#0x54]
  000d88d8: ldr r2,[r1,#0x4]
  000d88da: ldr r1,[r0,#0xc]
  000d88dc: ldr.w r0,[r2,r10,lsl #0x2]
  000d88e0: blx 0x00074644
  000d88e4: cmp r0,#0x0
  000d88e6: bge 0x000d88f2
  000d88e8: adds r4,#0x1
  000d88ea: ldr r0,[r5,#0x0]
  000d88ec: cmp r4,r0
  000d88ee: bcc 0x000d88b0
  000d88f0: b 0x000d88fc
  000d88f2: ldr.w r1,[r9,#0xfc]
  000d88f6: ldr.w r0,[r9,#0x20]
  000d88fa: str r0,[r1,#0x10]
  000d88fc: ldr.w r0,[r9,#0x1a8]
  000d8900: vmov.f32 s4,0xc0400000
  000d8904: cmp.w r11,#0x0
  000d8908: asr.w r0,r0, asr #0x1
  000d890c: vmov s0,r0
  000d8910: vcvt.f32.s32 s0,s0
  000d8914: vldr.32 s2,[r9,#0x7c]
  000d8918: vldr.32 s18,[r9,#0x78]
  000d891c: ldr r0,[0x000d8c74]
  000d891e: add r0,pc
  000d8920: ldr r0,[r0,#0x0]
  000d8922: vadd.f32 s0,s2,s0
  000d8926: str r0,[sp,#0x14]
  000d8928: ldr.w r8,[r0,#0x0]
  000d892c: ldr r0,[sp,#0x1c]
  000d892e: ldr r5,[r0,#0x0]
  000d8930: vadd.f32 s0,s0,s4
  000d8934: vcvt.s32.f32 s16,s0
  000d8938: vmov r0,s16
  000d893c: str r0,[sp,#0x18]
  000d893e: beq 0x000d8a16
  000d8940: ldr.w r0,[r9,#0x58]
  000d8944: add r6,sp,#0x58
  000d8946: ldr r0,[r0,#0x4]
  000d8948: ldr.w r1,[r0,r10,lsl #0x2]
  000d894c: mov r0,r6
  000d894e: blx 0x000736a8
  000d8952: mov r0,r5
  000d8954: mov r1,r8
  000d8956: mov r2,r6
  000d8958: blx 0x0006faa8
  000d895c: mov r5,r0
  000d895e: add r0,sp,#0x58
  000d8960: blx 0x0006ee30
  000d8964: add.w r0,r5,r5, lsr #0x1f
  000d8968: asrs r0,r0,#0x1
  000d896a: vmov s0,r0
  000d896e: vcvt.f32.s32 s16,s0
  000d8972: ldr r0,[sp,#0x20]
  000d8974: ldr r0,[r0,#0x0]
  000d8976: blx 0x00071c14
  000d897a: vsub.f32 s16,s18,s16
  000d897e: blx 0x00071824
  000d8982: mov r5,r0
  000d8984: ldr.w r0,[r9,#0x58]
  000d8988: ldr r0,[r0,#0x4]
  000d898a: ldr.w r0,[r0,r10,lsl #0x2]
  000d898e: vcvt.s32.f32 s0,s16
  000d8992: vmov r4,s0
  000d8996: blx 0x00071824
  000d899a: cmp r5,r0
  000d899c: bne.w 0x000d8b22
  000d89a0: ldr r0,[0x000d8c78]
  000d89a2: ldr.w r11,[sp,#0x1c]
  000d89a6: add r0,pc
  000d89a8: ldr r1,[0x000d8c7c]
  000d89aa: ldr.w r6,[r9,#0x1a4]
  000d89ae: ldr r0,[r0,#0x0]
  000d89b0: ldr.w r5,[r11,#0x0]
  000d89b4: ldr r0,[r0,#0x0]
  000d89b6: blx 0x00074674
  000d89ba: rsb.w r1,r6,#0xff
  000d89be: vmov s2,r0
  000d89c2: movs r2,#0xff
  000d89c4: movs r3,#0xff
  000d89c6: vmov s0,r1
  000d89ca: movs r1,#0xff
  000d89cc: vcvt.f32.s32 s0,s0
  000d89d0: vmul.f32 s0,s2,s0
  000d89d4: vcvt.u32.f32 s0,s0
  000d89d8: vmov r0,s0
  000d89dc: str r0,[sp,#0x0]
  000d89de: mov r0,r5
  000d89e0: blx 0x0006ff28
  000d89e4: vldr.32 s0,[r9,#0x78]
  000d89e8: movs r2,#0x44
  000d89ea: vldr.32 s2,[r9,#0x7c]
  000d89ee: movs r3,#0x11
  000d89f0: ldr.w r1,[r9,#0x48]
  000d89f4: ldr.w r0,[r11,#0x0]
  000d89f8: vcvt.s32.f32 s0,s0
  000d89fc: vcvt.s32.f32 s2,s2
  000d8a00: strd r3,r2,[sp,#0x0]
  000d8a04: vmov r2,s0
  000d8a08: vmov r3,s2
  000d8a0c: blx 0x00071d70
  000d8a10: add.w r5,r9,#0x1a4
  000d8a14: b 0x000d8b2a
  000d8a16: ldr.w r0,[r9,#0x54]
  000d8a1a: add r6,sp,#0x58
  000d8a1c: ldr r0,[r0,#0x4]
  000d8a1e: ldr.w r1,[r0,r10,lsl #0x2]
  000d8a22: mov r0,r6
  000d8a24: blx 0x00071cc8
  000d8a28: mov r0,r5
  000d8a2a: mov r1,r8
  000d8a2c: mov r2,r6
  000d8a2e: blx 0x0006faa8
  000d8a32: mov r5,r0
  000d8a34: add r0,sp,#0x58
  000d8a36: blx 0x0006ee30
  000d8a3a: add.w r0,r5,r5, lsr #0x1f
  000d8a3e: asrs r0,r0,#0x1
  000d8a40: vmov s0,r0
  000d8a44: vcvt.f32.s32 s0,s0
  000d8a48: ldrb.w r0,[r9,#0xa8]
  000d8a4c: cmp r0,#0x0
  000d8a4e: vsub.f32 s0,s18,s0
  000d8a52: vcvt.s32.f32 s18,s0
  000d8a56: ldrd r11,r4,[sp,#0x1c]
  000d8a5a: beq 0x000d8a6a
  000d8a5c: ldr.w r0,[r9,#0x54]
  000d8a60: ldr r0,[r0,#0x4]
  000d8a62: ldr.w r0,[r0,r10,lsl #0x2]
  000d8a66: blx 0x00071a7c
  000d8a6a: ldr r0,[r4,#0x0]
  000d8a6c: vmov r8,s18
  000d8a70: blx 0x00071a10
  000d8a74: blx 0x00071a7c
  000d8a78: mov r5,r0
  000d8a7a: ldr.w r0,[r9,#0x54]
  000d8a7e: ldr r0,[r0,#0x4]
  000d8a80: ldr.w r0,[r0,r10,lsl #0x2]
  000d8a84: blx 0x00071a7c
  000d8a88: cmp r5,r0
  000d8a8a: bne 0x000d8af0
  000d8a8c: ldr r0,[0x000d8c80]
  000d8a8e: vldr.32 s0,[r9,#0x1a4]
  000d8a92: add r0,pc
  000d8a94: ldr r1,[0x000d8c7c]
  000d8a96: vcvt.f32.s32 s18,s0
  000d8a9a: ldr.w r5,[r11,#0x0]
  000d8a9e: ldr r0,[r0,#0x0]
  000d8aa0: ldr r0,[r0,#0x0]
  000d8aa2: blx 0x00074674
  000d8aa6: vmov s0,r0
  000d8aaa: movs r1,#0xff
  000d8aac: movs r2,#0xff
  000d8aae: movs r3,#0xff
  000d8ab0: vmul.f32 s0,s0,s18
  000d8ab4: vcvt.u32.f32 s0,s0
  000d8ab8: vmov r0,s0
  000d8abc: str r0,[sp,#0x0]
  000d8abe: mov r0,r5
  000d8ac0: blx 0x0006ff28
  000d8ac4: vldr.32 s0,[r9,#0x78]
  000d8ac8: movs r2,#0x44
  000d8aca: vldr.32 s2,[r9,#0x7c]
  000d8ace: movs r3,#0x11
  000d8ad0: ldr.w r1,[r9,#0x48]
  000d8ad4: ldr.w r0,[r11,#0x0]
  000d8ad8: vcvt.s32.f32 s0,s0
  000d8adc: vcvt.s32.f32 s2,s2
  000d8ae0: strd r3,r2,[sp,#0x0]
  000d8ae4: vmov r2,s0
  000d8ae8: vmov r3,s2
  000d8aec: blx 0x00071d70
  000d8af0: ldr.w r0,[r11,#0x0]
  000d8af4: movs r2,#0xff
  000d8af6: ldrb.w r1,[r9,#0x1a4]
  000d8afa: movs r3,#0xff
  000d8afc: str r1,[sp,#0x0]
  000d8afe: movs r1,#0xff
  000d8b00: add.w r4,r9,#0x1a4
  000d8b04: blx 0x0006ff28
  000d8b08: ldr.w r0,[r9,#0x60]
  000d8b0c: cmp r0,r10
  000d8b0e: bne.w 0x000d8c94
  000d8b12: ldrb r1,[r4,#0x0]
  000d8b14: movs r2,#0x80
  000d8b16: str r1,[sp,#0x0]
  000d8b18: movs r1,#0xff
  000d8b1a: ldr.w r0,[r11,#0x0]
  000d8b1e: movs r3,#0x0
  000d8b20: b 0x000d8cae
  000d8b22: add.w r5,r9,#0x1a4
  000d8b26: ldr.w r11,[sp,#0x1c]
  000d8b2a: ldrb.w r1,[r9,#0x1a4]
  000d8b2e: ldr.w r2,[r9,#0x64]
  000d8b32: ldr.w r0,[r11,#0x0]
  000d8b36: mvns r1,r1
  000d8b38: cmp r2,r10
  000d8b3a: uxtb r1,r1
  000d8b3c: bne.w 0x000d8d4e
  000d8b40: str r1,[sp,#0x0]
  000d8b42: movs r1,#0xff
  000d8b44: movs r2,#0x80
  000d8b46: movs r3,#0x0
  000d8b48: blx 0x0006ff28
  000d8b4c: ldr r0,[sp,#0x14]
  000d8b4e: add.w r8,sp,#0x58
  000d8b52: ldr.w r6,[r11,#0x0]
  000d8b56: ldr r5,[r0,#0x0]
  000d8b58: ldr.w r0,[r9,#0x58]
  000d8b5c: ldr r0,[r0,#0x4]
  000d8b5e: ldr.w r1,[r0,r10,lsl #0x2]
  000d8b62: mov r0,r8
  000d8b64: blx 0x000736a8
  000d8b68: ldr r0,[sp,#0x18]
  000d8b6a: mov r1,r5
  000d8b6c: str r0,[sp,#0x0]
  000d8b6e: movs r0,#0x0
  000d8b70: str r0,[sp,#0x4]
  000d8b72: mov r0,r6
  000d8b74: mov r2,r8
  000d8b76: mov r3,r4
  000d8b78: blx 0x0006fe14
  000d8b7c: add r0,sp,#0x58
  000d8b7e: blx 0x0006ee30
  000d8b82: ldrb.w r1,[r9,#0x1a4]
  000d8b86: movs r2,#0xff
  000d8b88: ldr.w r0,[r11,#0x0]
  000d8b8c: movs r3,#0xff
  000d8b8e: eor r1,r1,#0xff
  000d8b92: str r1,[sp,#0x0]
  000d8b94: movs r1,#0xff
  000d8b96: blx 0x0006ff28
  000d8b9a: ldr.w r0,[r9,#0x58]
  000d8b9e: ldr r0,[r0,#0x4]
  000d8ba0: ldr.w r0,[r0,r10,lsl #0x2]
  000d8ba4: blx 0x00071890
  000d8ba8: cmp r0,#0x1
  000d8baa: blt 0x000d8c46
  000d8bac: ldr r0,[0x000d8c84]
  000d8bae: ldr r1,[sp,#0x14]
  000d8bb0: add r0,pc
  000d8bb2: ldr.w r8,[r11,#0x0]
  000d8bb6: ldr r0,[r0,#0x0]
  000d8bb8: ldr r1,[r1,#0x0]
  000d8bba: str r1,[sp,#0x20]
  000d8bbc: movs r1,#0x85
  000d8bbe: ldr r0,[r0,#0x0]
  000d8bc0: blx 0x00072f70
  000d8bc4: ldr r1,[0x000d8c88]
  000d8bc6: add r6,sp,#0x40
  000d8bc8: mov r5,r0
  000d8bca: movs r2,#0x0
  000d8bcc: add r1,pc
  000d8bce: mov r0,r6
  000d8bd0: blx 0x0006ee18
  000d8bd4: add r0,sp,#0x4c
  000d8bd6: mov r1,r5
  000d8bd8: mov r2,r6
  000d8bda: blx 0x0006ef98
  000d8bde: ldr.w r0,[r9,#0x58]
  000d8be2: ldr r0,[r0,#0x4]
  000d8be4: ldr.w r0,[r0,r10,lsl #0x2]
  000d8be8: blx 0x00071890
  000d8bec: mov r1,r0
  000d8bee: add r0,sp,#0x28
  000d8bf0: blx 0x0006f658
  000d8bf4: add r0,sp,#0x34
  000d8bf6: add r1,sp,#0x28
  000d8bf8: movs r2,#0x0
  000d8bfa: blx 0x0006f028
  000d8bfe: add r0,sp,#0x58
  000d8c00: add r1,sp,#0x4c
  000d8c02: add r2,sp,#0x34
  000d8c04: blx 0x0006ef98
  000d8c08: ldr r0,[0x000d8c8c]
  000d8c0a: add r0,pc
  000d8c0c: ldr r0,[r0,#0x0]
  000d8c0e: ldr r0,[r0,#0x0]
  000d8c10: ldr r0,[r0,#0x4]
  000d8c12: ldr r2,[sp,#0x18]
  000d8c14: movs r1,#0x0
  000d8c16: mov r3,r4
  000d8c18: add r0,r2
  000d8c1a: strd r0,r1,[sp,#0x0]
  000d8c1e: ldr r1,[sp,#0x20]
  000d8c20: add r2,sp,#0x58
  000d8c22: mov r0,r8
  000d8c24: blx 0x0006fe14
  000d8c28: ldr r0,[0x000d8c90]
  000d8c2a: add r0,pc
  000d8c2c: ldr r4,[r0,#0x0]
  000d8c2e: add r0,sp,#0x58
  000d8c30: blx r4
  000d8c32: add r0,sp,#0x34
  000d8c34: blx r4
  000d8c36: add r0,sp,#0x28
  000d8c38: blx r4
  000d8c3a: add r0,sp,#0x4c
  000d8c3c: blx r4
  000d8c3e: add r0,sp,#0x40
  000d8c40: blx r4
  000d8c42: ldr.w r11,[sp,#0x1c]
  000d8c46: vldr.32 s0,[r9,#0x78]
  000d8c4a: movs r2,#0x44
  000d8c4c: vldr.32 s2,[r9,#0x7c]
  000d8c50: movs r3,#0x11
  000d8c52: ldr.w r1,[r9,#0x40]
  000d8c56: vcvt.s32.f32 s0,s0
  000d8c5a: ldr.w r0,[r11,#0x0]
  000d8c5e: vcvt.s32.f32 s2,s2
  000d8c62: strd r3,r2,[sp,#0x0]
  000d8c66: vmov r2,s0
  000d8c6a: vmov r3,s2
  000d8c6e: blx 0x00071d70
  000d8c72: b 0x000d8fe0
  000d8c94: cmp r0,#0x0
  000d8c96: blt 0x000d8cb2
  000d8c98: ldr r1,[r4,#0x0]
  000d8c9a: movs r2,#0xff
  000d8c9c: movs r3,#0xff
  000d8c9e: cmp r1,#0x40
  000d8ca0: it ge
  000d8ca2: mov.ge r1,#0x40
  000d8ca4: ldr.w r0,[r11,#0x0]
  000d8ca8: uxtb r1,r1
  000d8caa: str r1,[sp,#0x0]
  000d8cac: movs r1,#0xff
  000d8cae: blx 0x0006ff28
  000d8cb2: ldr r0,[sp,#0x14]
  000d8cb4: str r4,[sp,#0x10]
  000d8cb6: add r4,sp,#0x58
  000d8cb8: ldr.w r6,[r11,#0x0]
  000d8cbc: ldr r5,[r0,#0x0]
  000d8cbe: ldr.w r0,[r9,#0x54]
  000d8cc2: ldr r0,[r0,#0x4]
  000d8cc4: ldr.w r1,[r0,r10,lsl #0x2]
  000d8cc8: mov r0,r4
  000d8cca: blx 0x00071cc8
  000d8cce: movs r0,#0x0
  000d8cd0: ldr r1,[sp,#0x18]
  000d8cd2: strd r1,r0,[sp,#0x0]
  000d8cd6: mov r0,r6
  000d8cd8: mov r1,r5
  000d8cda: mov r2,r4
  000d8cdc: mov r3,r8
  000d8cde: blx 0x0006fe14
  000d8ce2: add r0,sp,#0x58
  000d8ce4: blx 0x0006ee30
  000d8ce8: ldr.w r2,[r9,#0x1a4]
  000d8cec: ldr.w r1,[r9,#0x60]
  000d8cf0: cmp r2,#0x40
  000d8cf2: mov r3,r2
  000d8cf4: it ge
  000d8cf6: mov.ge r3,#0x40
  000d8cf8: cmp r1,r10
  000d8cfa: it eq
  000d8cfc: mov.eq r3,r2
  000d8cfe: ldr.w r0,[r11,#0x0]
  000d8d02: cmp r1,#0x0
  000d8d04: it lt
  000d8d06: mov.lt r3,r2
  000d8d08: uxtb r1,r3
  000d8d0a: movs r2,#0xff
  000d8d0c: str r1,[sp,#0x0]
  000d8d0e: movs r1,#0xff
  000d8d10: movs r3,#0xff
  000d8d12: blx 0x0006ff28
  000d8d16: ldr.w r0,[r9,#0x54]
  000d8d1a: ldr r0,[r0,#0x4]
  000d8d1c: ldr.w r0,[r0,r10,lsl #0x2]
  000d8d20: blx 0x00074680
  000d8d24: ldr r4,[sp,#0x20]
  000d8d26: cmp r0,#0x0
  000d8d28: bne 0x000d8e0a
  000d8d2a: ldr.w r0,[r9,#0x54]
  000d8d2e: ldr r0,[r0,#0x4]
  000d8d30: ldr.w r0,[r0,r10,lsl #0x2]
  000d8d34: blx 0x00071950
  000d8d38: cmp r0,#0x2
  000d8d3a: beq 0x000d8de8
  000d8d3c: cmp r0,#0x1
  000d8d3e: beq 0x000d8dee
  000d8d40: cmp r0,#0x0
  000d8d42: ite ne
  000d8d44: add.ne.w r0,r9,#0x130
  000d8d48: add.eq.w r0,r9,#0x124
  000d8d4c: b 0x000d8df2
  000d8d4e: str r1,[sp,#0x0]
  000d8d50: movs r1,#0xff
  000d8d52: movs r2,#0xff
  000d8d54: movs r3,#0xff
  000d8d56: blx 0x0006ff28
  000d8d5a: vldr.32 s0,[r9,#0x78]
  000d8d5e: movs r2,#0x44
  000d8d60: vldr.32 s2,[r9,#0x7c]
  000d8d64: movs r3,#0x11
  000d8d66: ldr.w r1,[r9,#0x44]
  000d8d6a: vcvt.s32.f32 s0,s0
  000d8d6e: ldr.w r0,[r11,#0x0]
  000d8d72: vcvt.s32.f32 s2,s2
  000d8d76: strd r3,r2,[sp,#0x0]
  000d8d7a: vmov r2,s0
  000d8d7e: vmov r3,s2
  000d8d82: blx 0x00071d70
  000d8d86: ldr.w r0,[r9,#0x64]
  000d8d8a: cmp r0,#0x0
  000d8d8c: blt 0x000d8dac
  000d8d8e: ldr r0,[r5,#0x0]
  000d8d90: movs r2,#0xff
  000d8d92: movs r3,#0xff
  000d8d94: rsb.w r1,r0,#0xff
  000d8d98: cmp r1,#0x40
  000d8d9a: it ge
  000d8d9c: mov.ge r1,#0x40
  000d8d9e: ldr.w r0,[r11,#0x0]
  000d8da2: uxtb r1,r1
  000d8da4: str r1,[sp,#0x0]
  000d8da6: movs r1,#0xff
  000d8da8: blx 0x0006ff28
  000d8dac: ldr r0,[sp,#0x14]
  000d8dae: add.w r8,sp,#0x58
  000d8db2: ldr.w r6,[r11,#0x0]
  000d8db6: ldr r5,[r0,#0x0]
  000d8db8: ldr.w r0,[r9,#0x58]
  000d8dbc: ldr r0,[r0,#0x4]
  000d8dbe: ldr.w r1,[r0,r10,lsl #0x2]
  000d8dc2: mov r0,r8
  000d8dc4: blx 0x000736a8
  000d8dc8: movs r0,#0x0
  000d8dca: ldr r1,[sp,#0x18]
  000d8dcc: strd r1,r0,[sp,#0x0]
  000d8dd0: mov r0,r6
  000d8dd2: mov r1,r5
  000d8dd4: mov r2,r8
  000d8dd6: mov r3,r4
  000d8dd8: blx 0x0006fe14
  000d8ddc: add r0,sp,#0x58
  000d8dde: blx 0x0006ee30
  000d8de2: ldr.w r11,[sp,#0x1c]
  000d8de6: b 0x000d8fe0
  000d8de8: add.w r0,r9,#0x12c
  000d8dec: b 0x000d8df2
  000d8dee: add.w r0,r9,#0x128
  000d8df2: movs r2,#0x12
  000d8df4: movs r3,#0x11
  000d8df6: ldr r1,[r0,#0x0]
  000d8df8: ldr.w r0,[r11,#0x0]
  000d8dfc: strd r3,r2,[sp,#0x0]
  000d8e00: ldr r2,[sp,#0x18]
  000d8e02: subs r3,r2,#0x3
  000d8e04: mov r2,r8
  000d8e06: blx 0x00071d70
  000d8e0a: cmp.w r10,#0x1a
  000d8e0e: bne 0x000d8e54
  000d8e10: ldr r0,[r4,#0x0]
  000d8e12: ldr.w r0,[r0,#0x114]
  000d8e16: cmp r0,#0x3
  000d8e18: bne 0x000d8e54
  000d8e1a: ldr r0,[0x000d90dc]
  000d8e1c: vmov.f32 s2,0x3f000000
  000d8e20: movs r2,#0x12
  000d8e22: add r0,pc
  000d8e24: ldr r0,[r0,#0x0]
  000d8e26: ldr r0,[r0,#0x0]
  000d8e28: vldr.32 s0,[r0,#0x2c]
  000d8e2c: vcvt.f32.s32 s0,s0
  000d8e30: vcvt.f32.s32 s4,s16
  000d8e34: ldr.w r1,[r9,#0x134]
  000d8e38: str r2,[sp,#0x4]
  000d8e3a: movs r2,#0x11
  000d8e3c: ldr.w r0,[r11,#0x0]
  000d8e40: vmla.f32 s4,s0,s2
  000d8e44: vcvt.s32.f32 s0,s4
  000d8e48: str r2,[sp,#0x0]
  000d8e4a: mov r2,r8
  000d8e4c: vmov r3,s0
  000d8e50: blx 0x00071d70
  000d8e54: ldr.w r0,[r11,#0x0]
  000d8e58: movs r2,#0xff
  000d8e5a: ldrb.w r1,[r9,#0x1a4]
  000d8e5e: movs r3,#0xff
  000d8e60: ldr.w r4,[r9,#0x60]
  000d8e64: str r1,[sp,#0x0]
  000d8e66: movs r1,#0xff
  000d8e68: blx 0x0006ff28
  000d8e6c: ldr.w r0,[r11,#0x0]
  000d8e70: cmp r4,r10
  000d8e72: bne.w 0x000d8f94
  000d8e76: vldr.32 s0,[r9,#0x78]
  000d8e7a: movs r2,#0x44
  000d8e7c: vldr.32 s2,[r9,#0x7c]
  000d8e80: movs r3,#0x11
  000d8e82: str.w r8,[sp,#0xc]
  000d8e86: ldr.w r1,[r9,#0x40]
  000d8e8a: vcvt.s32.f32 s0,s0
  000d8e8e: vcvt.s32.f32 s2,s2
  000d8e92: strd r3,r2,[sp,#0x0]
  000d8e96: vmov r2,s0
  000d8e9a: vmov r3,s2
  000d8e9e: blx 0x00071d70
  000d8ea2: ldr r0,[0x000d90e0]
  000d8ea4: add r0,pc
  000d8ea6: ldr r4,[r0,#0x0]
  000d8ea8: ldr.w r0,[r9,#0x54]
  000d8eac: ldr r1,[r4,#0x0]
  000d8eae: ldr r0,[r0,#0x4]
  000d8eb0: ldr.w r8,[r1,#0x4]
  000d8eb4: ldr.w r0,[r0,r10,lsl #0x2]
  000d8eb8: blx 0x00074680
  000d8ebc: cbnz r0,0x000d8f0e
  000d8ebe: ldr.w r0,[r9,#0x54]
  000d8ec2: ldr.w r1,[r9,#0x60]
  000d8ec6: ldr r2,[0x000d90e4]
  000d8ec8: ldr r0,[r0,#0x4]
  000d8eca: add r2,pc
  000d8ecc: ldr.w r6,[r11,#0x0]
  000d8ed0: ldr.w r0,[r0,r1,lsl #0x2]
  000d8ed4: ldr r2,[r2,#0x0]
  000d8ed6: ldr r1,[sp,#0x14]
  000d8ed8: ldr r5,[r2,#0x0]
  000d8eda: ldr.w r8,[r1,#0x0]
  000d8ede: blx 0x00071950
  000d8ee2: add.w r1,r0,#0x196
  000d8ee6: mov r0,r5
  000d8ee8: blx 0x00072f70
  000d8eec: mov r2,r0
  000d8eee: ldr r0,[r4,#0x0]
  000d8ef0: ldr r3,[sp,#0x18]
  000d8ef2: movs r1,#0x0
  000d8ef4: ldr r0,[r0,#0x4]
  000d8ef6: add r0,r3
  000d8ef8: strd r0,r1,[sp,#0x0]
  000d8efc: ldr r3,[sp,#0xc]
  000d8efe: mov r0,r6
  000d8f00: mov r1,r8
  000d8f02: blx 0x0006fe14
  000d8f06: ldr r0,[r4,#0x0]
  000d8f08: ldr r0,[r0,#0x4]
  000d8f0a: lsl.w r8,r0,#0x1
  000d8f0e: ldr.w r0,[r9,#0x54]
  000d8f12: ldr.w r1,[r9,#0x60]
  000d8f16: ldr r0,[r0,#0x4]
  000d8f18: ldr.w r0,[r0,r1,lsl #0x2]
  000d8f1c: blx 0x00073ef4
  000d8f20: mov r5,r0
  000d8f22: cmp.w r10,#0x1a
  000d8f26: bne 0x000d8f36
  000d8f28: ldr r0,[sp,#0x20]
  000d8f2a: ldr r0,[r0,#0x0]
  000d8f2c: ldr.w r0,[r0,#0x114]
  000d8f30: cmp r0,#0x1
  000d8f32: it gt
  000d8f34: mov.gt r5,#0x3
  000d8f36: ldr r0,[0x000d90e8]
  000d8f38: add.w r2,r5,r5, lsl #0x1
  000d8f3c: ldr.w r10,[sp,#0x10]
  000d8f40: add r0,pc
  000d8f42: add.w r3,r0,r2, lsl #0x2
  000d8f46: ldrb.w r1,[r0,r2,lsl #0x2]
  000d8f4a: ldrb r2,[r3,#0x4]
  000d8f4c: ldr.w r0,[r11,#0x0]
  000d8f50: ldrb r3,[r3,#0x8]
  000d8f52: ldrb.w r6,[r10,#0x0]
  000d8f56: str r6,[sp,#0x0]
  000d8f58: blx 0x0006ff28
  000d8f5c: ldr r0,[0x000d90ec]
  000d8f5e: ldr r1,[sp,#0x14]
  000d8f60: add r0,pc
  000d8f62: ldr.w r6,[r11,#0x0]
  000d8f66: ldr r0,[r0,#0x0]
  000d8f68: ldr r4,[r1,#0x0]
  000d8f6a: add.w r1,r5,#0x192
  000d8f6e: ldr r0,[r0,#0x0]
  000d8f70: blx 0x00072f70
  000d8f74: ldr r1,[sp,#0x18]
  000d8f76: mov r2,r0
  000d8f78: movs r0,#0x0
  000d8f7a: add r1,r8
  000d8f7c: strd r1,r0,[sp,#0x0]
  000d8f80: ldr r3,[sp,#0xc]
  000d8f82: mov r0,r6
  000d8f84: mov r1,r4
  000d8f86: blx 0x0006fe14
  000d8f8a: ldr.w r0,[r11,#0x0]
  000d8f8e: ldrb.w r1,[r10,#0x0]
  000d8f92: b 0x000d8fd4
  000d8f94: vldr.32 s0,[r9,#0x78]
  000d8f98: movs r2,#0x44
  000d8f9a: vldr.32 s2,[r9,#0x7c]
  000d8f9e: movs r3,#0x11
  000d8fa0: ldr.w r1,[r9,#0x44]
  000d8fa4: vcvt.s32.f32 s0,s0
  000d8fa8: vcvt.s32.f32 s2,s2
  000d8fac: strd r3,r2,[sp,#0x0]
  000d8fb0: vmov r2,s0
  000d8fb4: vmov r3,s2
  000d8fb8: blx 0x00071d70
  000d8fbc: ldr.w r0,[r9,#0x60]
  000d8fc0: cmp r0,#0x0
  000d8fc2: blt 0x000d8fe0
  000d8fc4: ldr r0,[sp,#0x10]
  000d8fc6: ldr r1,[r0,#0x0]
  000d8fc8: cmp r1,#0x40
  000d8fca: it ge
  000d8fcc: mov.ge r1,#0x40
  000d8fce: ldr.w r0,[r11,#0x0]
  000d8fd2: uxtb r1,r1
  000d8fd4: str r1,[sp,#0x0]
  000d8fd6: movs r1,#0xff
  000d8fd8: movs r2,#0xff
  000d8fda: movs r3,#0xff
  000d8fdc: blx 0x0006ff28
  000d8fe0: ldr.w r0,[r9,#0x1a8]
  000d8fe4: vmov.f32 s6,0x41200000
  000d8fe8: vmov.f32 s8,0xc0e00000
  000d8fec: movs r4,#0x0
  000d8fee: asrs r0,r0,#0x1
  000d8ff0: vmov s0,r0
  000d8ff4: vcvt.f32.s32 s0,s0
  000d8ff8: vldr.32 s4,[r9,#0x7c]
  000d8ffc: vldr.32 s2,[r9,#0x78]
  000d9000: vsub.f32 s4,s4,s0
  000d9004: vadd.f32 s0,s2,s0
  000d9008: vadd.f32 s2,s4,s6
  000d900c: vadd.f32 s0,s0,s8
  000d9010: vcvt.s32.f32 s2,s2
  000d9014: vcvt.s32.f32 s0,s0
  000d9018: ldr r0,[0x000d90f0]
  000d901a: add r0,pc
  000d901c: ldr.w r8,[r0,#0x0]
  000d9020: vmov r5,s2
  000d9024: vmov r6,s0
  000d9028: b 0x000d9074
  000d902a: ldr.w r0,[r9,#0xfc]
  000d902e: ldr.w r1,[r0,r4,lsl #0x2]
  000d9032: adds r0,r1,#0x1
  000d9034: beq 0x000d9072
  000d9036: ldr.w r0,[r11,#0x0]
  000d903a: cbz r4,0x000d904c
  000d903c: mov r2,r6
  000d903e: mov r3,r5
  000d9040: blx 0x00072dc0
  000d9044: ldr.w r0,[r9,#0x1ac]
  000d9048: add r5,r0
  000d904a: b 0x000d9072
  000d904c: ldrb.w r3,[r8,#0x0]
  000d9050: ldr.w r2,[r9,#0x1a8]
  000d9054: cmp r3,#0x0
  000d9056: mvn r3,#0x22
  000d905a: add r2,r5
  000d905c: it ne
  000d905e: mvn.ne r3,#0x17
  000d9062: add r3,r2
  000d9064: mov.w r2,#0x12
  000d9068: it ne
  000d906a: mov.ne r2,#0xc
  000d906c: subs r2,r6,r2
  000d906e: blx 0x00072dc0
  000d9072: adds r4,#0x1
  000d9074: cmp r4,#0x5
  000d9076: bne 0x000d902a
  000d9078: ldr r0,[sp,#0x64]
  000d907a: ldr r1,[sp,#0x24]
  000d907c: ldr r1,[r1,#0x0]
  000d907e: subs r0,r1,r0
  000d9080: itttt eq
  000d9082: add.eq sp,#0x68
  000d9084: vpop.eq {d8,d9}
  000d9088: add.eq sp,#0x4
  000d908a: pop.eq.w {r8,r9,r10,r11}
  000d908e: it eq
  000d9090: pop.eq {r4,r5,r6,r7,pc}
  000d9092: blx 0x0006e824
```
