# StarMap::OnTouchEnd
elf 0xcae90 body 2668
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* StarMap::OnTouchEnd(int, int) */

void StarMap::OnTouchEnd(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  Station *pSVar4;
  int iVar5;
  PaintCanvas *this;
  SolarSystem *pSVar6;
  Ship *pSVar7;
  uint *puVar8;
  String *pSVar9;
  code *pcVar10;
  undefined4 uVar11;
  uint uVar12;
  int in_r2;
  undefined1 *puVar13;
  undefined4 *puVar14;
  ChoiceWindow *pCVar15;
  undefined4 *puVar16;
  undefined4 uVar17;
  int *piVar18;
  code *pcVar19;
  code *pcVar20;
  int *piVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float fVar25;
  float fVar26;
  float fVar27;
  String aSStack_94 [12];
  String aSStack_88 [12];
  undefined1 auStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  Vector aVStack_40 [12];
  int local_34;
  
  piVar18 = *(int **)(DAT_000db204 + 0xdaeaa);
  local_34 = *piVar18;
  if (*(char *)(param_1 + 0xa9) != '\0') {
    iVar2 = ChoiceWindow::OnTouchEnd(*(int *)(param_1 + 0x5c),param_2);
    if (iVar2 == 1) {
      *(undefined1 *)(param_1 + 0xa9) = 0;
      *(undefined1 *)(param_1 + 0x120) = 0;
      goto LAB_000db932;
    }
    if (iVar2 != 0) goto LAB_000db932;
    *(undefined1 *)(param_1 + 0xa9) = 0;
    if (*(char *)(param_1 + 0x1e4) != '\0') {
LAB_000daedc:
      *(undefined1 *)(param_1 + 0x1e4) = 0;
      goto LAB_000db932;
    }
    if (*(int *)(param_1 + 4) == 3) {
      iVar2 = Station::getIndex(*(Station **)
                                 (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                 *(int *)(param_1 + 100) * 4));
      pSVar4 = (Station *)Status::getStation();
      iVar5 = Station::getIndex(pSVar4);
      if (iVar2 == iVar5) goto LAB_000daedc;
    }
    if ((((*(char *)(param_1 + 0x120) == '\0') || (*(int *)(param_1 + 4) != 0)) ||
        (*(char *)(param_1 + 0xa8) != '\0')) || (*(char *)(param_1 + 0x118) != '\0')) {
      if ((*(char *)(param_1 + 0xa8) != '\0') || (*(int *)(param_1 + 4) != 3)) goto LAB_000db932;
      cVar1 = *(char *)(param_1 + 0xab);
      if ((cVar1 == '\0') || (*(int *)(param_1 + 0x1d0) <= *(int *)(param_1 + 0x1d8))) {
        if (*(char *)(param_1 + 0xaa) == '\0') {
          depart((StarMap *)param_1,true);
          goto LAB_000db932;
        }
        iVar2 = *(int *)(param_1 + 100);
        iVar5 = *(int *)(*(int *)(param_1 + 0x58) + 4);
        **(undefined4 **)(DAT_000db23c + 0xdb19e) = *(undefined4 *)(iVar5 + iVar2 * 4);
        if (cVar1 == '\0') {
          **(undefined1 **)(DAT_000db380 + 0xdb33c) = 0;
        }
        else {
          iVar2 = Station::getSystem(*(Station **)(iVar5 + iVar2 * 4));
          Status::getSystem();
          iVar5 = SolarSystem::getIndex();
          *(bool *)*(undefined4 *)(DAT_000db244 + 0xdb1ce) = iVar2 != iVar5;
          if (iVar2 != iVar5) {
            **(undefined4 **)(DAT_000db248 + 0xdb1de) = *(undefined4 *)(param_1 + 0x1d0);
          }
        }
        uVar12 = *(uint *)(param_1 + 0x74);
        puVar14 = (undefined4 *)(DAT_000db384 + 0xdb348);
        *(undefined1 *)param_1 = 1;
        this = *(PaintCanvas **)*puVar14;
      }
      else {
        if (*(int *)(param_1 + 0x1d0) != 1) goto LAB_000db932;
        if (*(char *)(param_1 + 0xaa) == '\0') {
          depart((StarMap *)param_1,false);
          goto LAB_000db932;
        }
        puVar14 = *(undefined4 **)(DAT_000db224 + 0xdb056);
        puVar16 = *(undefined4 **)(DAT_000db228 + 0xdb058);
        uVar11 = *(undefined4 *)
                  (*(int *)(*(int *)(param_1 + 0x58) + 4) + *(int *)(param_1 + 100) * 4);
        **(undefined1 **)(DAT_000db220 + 0xdb04e) = 0;
        *puVar16 = uVar11;
        uVar12 = *(uint *)(param_1 + 0x74);
        *(undefined1 *)param_1 = 1;
        this = (PaintCanvas *)*puVar14;
      }
    }
    else {
      puVar14 = (undefined4 *)(DAT_000db378 + 0xdb30e);
      puVar13 = *(undefined1 **)(DAT_000db374 + 0xdb30a);
      puVar16 = *(undefined4 **)(DAT_000db37c + 0xdb314);
      **(undefined4 **)(DAT_000db370 + 0xdb306) =
           *(undefined4 *)(**(int **)(DAT_000db36c + 0xdb300) + 0x78);
      *(undefined1 *)(param_1 + 0x120) = 0;
      *(undefined1 *)param_1 = 1;
      puVar14 = (undefined4 *)*puVar14;
      *puVar13 = 1;
      *puVar16 = *(undefined4 *)(param_1 + 0x1d0);
      this = (PaintCanvas *)*puVar14;
      uVar12 = *(uint *)(param_1 + 0x74);
    }
    AbyssEngine::PaintCanvas::CameraSetCurrent(this,uVar12);
    goto LAB_000db932;
  }
  if ((*(char *)(param_1 + 0x138) != '\0') || (*(char *)(param_1 + 0x139) != '\0'))
  goto LAB_000db932;
  puVar14 = *(undefined4 **)(DAT_000db208 + 0xdaefe);
  if ((*(Layout *)*puVar14 == (Layout)0x0) &&
     (iVar5 = Layout::OnTouchEnd((Layout *)*puVar14,param_2,in_r2), iVar2 = DAT_000db20c, iVar5 != 0
     )) {
    if ((*(int *)(param_1 + 4) == 3) && (*(char *)(param_1 + 0xf4) != '\0')) {
      *(undefined1 *)(param_1 + 0x139) = 1;
      *(undefined4 *)(param_1 + 0x168) = 0;
      *(undefined4 *)(param_1 + 0x16c) = 0;
      *(undefined4 *)(param_1 + 0x170) = 0;
      uVar17 = *(undefined4 *)(param_1 + 0x17c);
      pcVar10 = *(code **)(iVar2 + 0xdaf3a);
      uVar11 = (*pcVar10)(uVar17);
      pcVar20 = *(code **)(DAT_000db210 + 0xdaf58);
      uVar3 = (*pcVar20)(*(undefined4 *)(param_1 + 0x17c));
      pcVar19 = *(code **)(DAT_000db214 + 0xdaf66);
      (*pcVar19)(uVar17,uVar11,uVar3);
      uVar17 = *(undefined4 *)(param_1 + 0x180);
      uVar11 = (*pcVar10)(uVar17);
      uVar3 = (*pcVar20)(*(undefined4 *)(param_1 + 0x180));
      (*pcVar19)(uVar17,uVar11,uVar3);
      uVar17 = *(undefined4 *)(param_1 + 0x184);
      uVar11 = (*pcVar10)(uVar17);
      uVar3 = (*pcVar20)(*(undefined4 *)(param_1 + 0x184));
      fVar22 = (float)(*pcVar19)(uVar17,uVar11,uVar3);
      FModSound::play(**(int **)(DAT_000db218 + 0xdafac),(Vector *)0x6b,(Vector *)0x0,fVar22);
    }
    else {
      AbyssEngine::PaintCanvas::CameraSetCurrent
                ((PaintCanvas *)**(undefined4 **)(DAT_000db24c + 0xdb1ea),*(uint *)(param_1 + 0x74))
      ;
      FModSound::stop(**(int **)(DAT_000db250 + 0xdb1f8));
    }
    goto LAB_000db932;
  }
  if ((*(char *)(param_1 + 0xa8) != '\0') && (*(char *)(param_1 + 0x13a) != '\0'))
  goto LAB_000db932;
  iVar2 = TouchButton::OnTouchEnd(*(int *)(param_1 + 0x4c),param_2);
  if (iVar2 != 0) {
    *(byte *)(param_1 + 0x108) = *(byte *)(param_1 + 0x108) ^ 1;
  }
  fVar22 = DAT_000db22c;
  if (*(char *)(param_1 + 0x174) != '\0') {
    fVar23 = *(float *)(param_1 + 0x150);
    fVar25 = *(float *)(param_1 + 0x154);
    fVar24 = -fVar23;
    if (0.0 < fVar23) {
      fVar24 = fVar23;
    }
    fVar27 = -fVar25;
    if (0.0 < fVar25) {
      fVar27 = fVar25;
    }
    fVar26 = DAT_000db22c;
    if (3.0 < fVar24) {
      fVar26 = fVar23;
    }
    fVar24 = DAT_000db22c;
    if (3.0 < fVar27) {
      fVar24 = fVar25;
    }
    *(undefined4 *)(param_1 + 0x168) = DAT_000db230;
    *(undefined1 *)(param_1 + 0x174) = 0;
    *(float *)(param_1 + 0x16c) = fVar26;
    *(float *)(param_1 + 0x170) = fVar24;
    if (*(int *)(param_1 + 4) == 0) {
      if (-1 < *(int *)(param_1 + 0x60)) {
        if (((*(char *)(param_1 + 0x118) == '\0') && (*(char *)(param_1 + 0xa8) == '\0')) &&
           (*(int *)(param_1 + 0x19c) == *(int *)(param_1 + 0x60))) {
          if (*(char *)(param_1 + 0xab) == '\0') {
            pSVar6 = (SolarSystem *)Status::getSystem();
            iVar2 = SolarSystem::getIndex();
            iVar2 = SolarSystem::systemIsInSystemRoutes(pSVar6,iVar2);
            if (iVar2 == 0) {
              pCVar15 = *(ChoiceWindow **)(param_1 + 0x5c);
              puVar14 = (undefined4 *)(DAT_000db9e8 + 0xdb7d8);
LAB_000db7d6:
              pSVar9 = (String *)GameText::getText(*(int *)*puVar14);
              ChoiceWindow::set(pCVar15,pSVar9,false);
              *(undefined1 *)(param_1 + 0xa9) = 1;
              goto LAB_000db932;
            }
            fVar22 = extraout_s0_01;
            if (*(char *)(param_1 + 0xab) != '\0') goto LAB_000db27a;
          }
          else {
LAB_000db27a:
            pSVar7 = (Ship *)Status::getShip();
            iVar2 = Ship::hasVolatileGoods(pSVar7);
            fVar22 = extraout_s0;
            if (iVar2 != 0) {
              pSVar6 = (SolarSystem *)Status::getSystem();
              iVar2 = SolarSystem::getIndex();
              iVar2 = SolarSystem::systemIsInSystemRoutes(pSVar6,iVar2);
              fVar22 = extraout_s0_00;
              if (iVar2 == 0) {
                pCVar15 = *(ChoiceWindow **)(param_1 + 0x5c);
                puVar14 = (undefined4 *)(DAT_000db364 + 0xdb2c0);
                goto LAB_000db7d6;
              }
            }
          }
          FModSound::play(**(int **)(DAT_000db9ec + 0xdb706),(Vector *)0x6a,(Vector *)0x0,fVar22);
          initStarSystem((StarMap *)param_1);
          *(undefined1 *)(param_1 + 0x138) = 1;
          AEGeometry::getPosition();
          AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x78),aVStack_40);
          puVar8 = *(uint **)(DAT_000db9f0 + 0xdb74a);
          *(float *)(param_1 + 0x80) = *(float *)(param_1 + 0x80) + DAT_000db9e0;
          uVar12 = *puVar8;
          AbyssEngine::PaintCanvas::CameraGetCurrent();
          AbyssEngine::PaintCanvas::CameraGetLocal(uVar12);
          AbyssEngine::AEMath::MatrixGetPosition((Matrix *)aVStack_40);
          AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x84),aVStack_40);
          pcVar10 = *(code **)(DAT_000db9f4 + 0xdb77a);
          (*pcVar10)(*(undefined4 *)(param_1 + 0x17c),*(undefined4 *)(param_1 + 0x84),
                     *(undefined4 *)(param_1 + 0x78));
          (*pcVar10)(*(undefined4 *)(param_1 + 0x180),*(undefined4 *)(param_1 + 0x88),
                     *(undefined4 *)(param_1 + 0x7c));
          (*pcVar10)(*(undefined4 *)(param_1 + 0x184),*(undefined4 *)(param_1 + 0x8c),
                     *(undefined4 *)(param_1 + 0x80));
          *(undefined4 *)(param_1 + 0x168) = 0;
          *(undefined4 *)(param_1 + 0x16c) = 0;
          *(undefined4 *)(param_1 + 0x170) = 0;
          goto LAB_000db932;
        }
        *(undefined4 *)(param_1 + 0x19c) = 0xffffffff;
        *(undefined1 *)(param_1 + 0x13a) = 1;
      }
    }
    else {
      iVar2 = *(int *)(param_1 + 100);
      if (-1 < iVar2) {
        if (*(int *)(param_1 + 0x1a0) == iVar2) {
          if (*(char *)(param_1 + 0xa8) == '\0') {
            iVar2 = Station::getIndex(*(Station **)
                                       (*(int *)(*(int *)(param_1 + 0x58) + 4) + iVar2 * 4));
            pSVar4 = (Station *)Status::getStation();
            iVar5 = Station::getIndex(pSVar4);
            pCVar15 = *(ChoiceWindow **)(param_1 + 0x5c);
            if (iVar2 == iVar5) {
              if (pCVar15 == (ChoiceWindow *)0x0) {
                pCVar15 = operator_new(0x5c);
                ChoiceWindow::ChoiceWindow(pCVar15);
                *(ChoiceWindow **)(param_1 + 0x5c) = pCVar15;
              }
              iVar2 = **(int **)(DAT_000db238 + 0xdb176);
LAB_000db178:
              GameText::getText(iVar2);
              ChoiceWindow::set(pCVar15);
            }
            else {
              if (pCVar15 == (ChoiceWindow *)0x0) {
                pCVar15 = operator_new(0x5c);
                ChoiceWindow::ChoiceWindow(pCVar15);
                *(ChoiceWindow **)(param_1 + 0x5c) = pCVar15;
              }
              iVar2 = Status::getCurrentCampaignMission();
              if (iVar2 == 0x18) {
                iVar2 = Station::getIndex(*(Station **)
                                           (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                           *(int *)(param_1 + 100) * 4));
                Status::getCampaignMission();
                iVar5 = Mission::getTargetStation();
                if (iVar2 != iVar5) goto LAB_000db3ec;
                iVar2 = Status::getShip();
                iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
                if (iVar2 != 0) {
                  iVar2 = Status::getShip();
                  iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
                  if (iVar2 != 0) goto LAB_000db3ec;
                }
                pSVar9 = *(String **)(param_1 + 0x5c);
                puVar16 = (undefined4 *)(DAT_000db9f8 + 0xdb7c0);
              }
              else {
LAB_000db3ec:
                iVar2 = Status::getCurrentCampaignMission();
                if (iVar2 == 0x87) {
                  iVar2 = Station::getIndex(*(Station **)
                                             (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                             *(int *)(param_1 + 100) * 4));
                  Status::getCampaignMission();
                  iVar5 = Mission::getTargetStation();
                  if (iVar2 == iVar5) {
                    iVar2 = Status::getShip();
                    iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
                    if (iVar2 == 0) {
                      pSVar9 = *(String **)(param_1 + 0x5c);
                      puVar16 = (undefined4 *)(DAT_000db9fc + 0xdb7fe);
                      goto LAB_000db87a;
                    }
                  }
                }
                iVar2 = Status::getCurrentCampaignMission();
                if (iVar2 != 0x5b) {
LAB_000db46a:
                  iVar2 = Status::getCurrentCampaignMission();
                  if (iVar2 == 0x5e) {
                    iVar2 = Station::getIndex(*(Station **)
                                               (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                               *(int *)(param_1 + 100) * 4));
                    Status::getCampaignMission();
                    iVar5 = Mission::getTargetStation();
                    if (iVar2 == iVar5) {
                      Status::getShip();
                      iVar2 = Ship::getMaxPassengers();
                      if (iVar2 == 0) goto LAB_000db7a8;
                    }
                  }
                  iVar2 = Status::getCurrentCampaignMission();
                  if (iVar2 == 0x69) {
                    iVar2 = Station::getIndex(*(Station **)
                                               (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                               *(int *)(param_1 + 100) * 4));
                    Status::getCampaignMission();
                    iVar5 = Mission::getTargetStation();
                    if (iVar2 != iVar5) goto LAB_000db4ec;
                    iVar2 = Status::getShip();
                    iVar2 = Ship::hasEquipment(iVar2,0xce);
                    if (iVar2 != 0) goto LAB_000db4ec;
                    pSVar9 = *(String **)(param_1 + 0x5c);
                    GameText::getText(**(int **)(DAT_000dba04 + 0xdb80a));
                    ChoiceWindow::set(pSVar9);
                    *(undefined1 *)(param_1 + 0x1e4) = 1;
                    goto LAB_000db890;
                  }
LAB_000db4ec:
                  iVar2 = Status::getCurrentCampaignMission();
                  if (iVar2 == 0x8b) {
                    iVar2 = Station::getIndex(*(Station **)
                                               (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                               *(int *)(param_1 + 100) * 4));
                    Status::getCampaignMission();
                    iVar5 = Mission::getTargetStation();
                    if (iVar2 == iVar5) {
                      Status::getShip();
                      iVar2 = Ship::getIndex();
                      if (iVar2 != 0x2a) {
                        Status::getShip();
                        iVar2 = Ship::getIndex();
                        if (*(int *)(DAT_000dba08 + 0xdb830 + iVar2 * 4) == 1) {
                          iVar2 = Status::getShip();
                          iVar2 = Ship::hasEquipment(iVar2,0xbe);
                          if (iVar2 != 0) goto LAB_000db52a;
                        }
                        pSVar9 = *(String **)(param_1 + 0x5c);
                        puVar16 = (undefined4 *)(DAT_000dba0c + 0xdb858);
                        goto LAB_000db87a;
                      }
                    }
                  }
LAB_000db52a:
                  iVar2 = Status::getCurrentCampaignMission();
                  if (iVar2 == 0x8e) {
                    iVar2 = Station::getIndex(*(Station **)
                                               (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                               *(int *)(param_1 + 100) * 4));
                    Status::getCampaignMission();
                    iVar5 = Mission::getTargetStation();
                    if (iVar2 == iVar5) {
                      iVar2 = Status::getShip();
                      iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
                      if (iVar2 != 0) {
                        iVar2 = Status::getShip();
                        iVar2 = Ship::hasEquipment(iVar2,0xc5);
                        if (iVar2 != 0) {
                          iVar2 = Status::getShip();
                          iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
                          if (iVar2 != 0) goto LAB_000db594;
                        }
                      }
                      pSVar9 = *(String **)(param_1 + 0x5c);
                      puVar16 = (undefined4 *)(DAT_000dba10 + 0xdb7cc);
                      goto LAB_000db87a;
                    }
                  }
LAB_000db594:
                  iVar2 = Status::getCurrentCampaignMission();
                  if (iVar2 == 0x8e) {
                    iVar2 = Station::getIndex(*(Station **)
                                               (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                               *(int *)(param_1 + 100) * 4));
                    Status::getCampaignMission();
                    iVar5 = Mission::getTargetStation();
                    if (iVar2 == iVar5) {
                      Status::getShip();
                      iVar2 = Ship::getFreeSpace();
                      if (iVar2 == 0) {
                        pSVar9 = *(String **)(param_1 + 0x5c);
                        puVar16 = (undefined4 *)(DAT_000dba14 + 0xdb87c);
                        goto LAB_000db87a;
                      }
                    }
                  }
                  if (*(char *)(param_1 + 0xab) != '\0') {
                    if (*(int *)(param_1 + 0x1d8) < *(int *)(param_1 + 0x1d0)) {
                      pSVar7 = (Ship *)Status::getShip();
                      iVar2 = Ship::hasVolatileGoods(pSVar7);
                      if (iVar2 != 0) {
                        if (*(char *)(param_1 + 0xab) == '\0') goto LAB_000db622;
                        goto LAB_000db5fc;
                      }
                      pCVar15 = *(ChoiceWindow **)(param_1 + 0x5c);
                      iVar2 = **(int **)(DAT_000dba18 + 0xdb862);
                      if (*(int *)(param_1 + 0x1d0) != 1) goto LAB_000db178;
LAB_000db968:
                      pSVar9 = (String *)GameText::getText(iVar2);
                      ChoiceWindow::set(pCVar15,pSVar9,true);
                      goto LAB_000db890;
                    }
LAB_000db5fc:
                    if ((*(char *)(param_1 + 0x1d4) != '\0') &&
                       (*(int *)(param_1 + 0x1d8) < *(int *)(param_1 + 0x1d0) * 2)) {
                      pSVar7 = (Ship *)Status::getShip();
                      iVar2 = Ship::hasVolatileGoods(pSVar7);
                      if (iVar2 == 0) {
                        pCVar15 = *(ChoiceWindow **)(param_1 + 0x5c);
                        iVar2 = **(int **)(DAT_000dba1c + 0xdb966);
                        goto LAB_000db968;
                      }
                    }
                  }
LAB_000db622:
                  pCVar15 = *(ChoiceWindow **)(param_1 + 0x5c);
                  piVar21 = *(int **)(DAT_000db9d4 + 0xdb630);
                  pSVar9 = (String *)GameText::getText(*piVar21);
                  AbyssEngine::String::String(aSStack_70,(char *)(DAT_000db9d8 + 0xdb646),false);
                  AbyssEngine::operator+(aSStack_64,pSVar9);
                  Station::getName();
                  AbyssEngine::operator+(aSStack_58,aSStack_64);
                  AbyssEngine::String::String(aSStack_88,(char *)(DAT_000dba20 + 0xdb674),false);
                  AbyssEngine::operator+(aSStack_4c,aSStack_58);
                  GameText::getText(*piVar21);
                  AbyssEngine::operator+(aVStack_40,aSStack_4c);
                  ChoiceWindow::set(pCVar15,aVStack_40,true);
                  pcVar10 = *(code **)(DAT_000dba24 + 0xdb6aa);
                  (*pcVar10)(aVStack_40);
                  (*pcVar10)(aSStack_4c);
                  (*pcVar10)(aSStack_88);
                  (*pcVar10)(aSStack_58);
                  (*pcVar10)(auStack_7c);
                  (*pcVar10)(aSStack_64);
                  (*pcVar10)(aSStack_70);
                  goto LAB_000db890;
                }
                iVar2 = Station::getIndex(*(Station **)
                                           (*(int *)(*(int *)(param_1 + 0x58) + 4) +
                                           *(int *)(param_1 + 100) * 4));
                Status::getCampaignMission();
                iVar5 = Mission::getTargetStation();
                if (iVar2 != iVar5) goto LAB_000db46a;
                Status::getShip();
                iVar2 = Ship::getMaxPassengers();
                if (9 < iVar2) goto LAB_000db46a;
LAB_000db7a8:
                pSVar9 = *(String **)(param_1 + 0x5c);
                puVar16 = (undefined4 *)(DAT_000dba00 + 0xdb7b4);
              }
LAB_000db87a:
              GameText::getText(*(int *)*puVar16);
              ChoiceWindow::set(pSVar9);
              *(undefined1 *)(param_1 + 0x1e4) = 1;
            }
LAB_000db890:
            *(undefined1 *)(param_1 + 0xa9) = 1;
          }
        }
        else {
          FModSound::play(**(int **)(DAT_000db368 + 0xdb2dc),(Vector *)0x69,(Vector *)0x0,fVar24);
          *(undefined1 *)(param_1 + 0x13b) = 1;
        }
      }
      fVar23 = *(float *)(param_1 + 0x150);
      fVar25 = *(float *)(param_1 + 0x154);
      fVar24 = -fVar23;
      if (0.0 < fVar23) {
        fVar24 = fVar23;
      }
      fVar27 = -fVar25;
      if (0.0 < fVar25) {
        fVar27 = fVar25;
      }
      fVar26 = fVar22;
      if (DAT_000db9dc < fVar24) {
        fVar26 = fVar23;
      }
      if (DAT_000db9dc < fVar27) {
        fVar22 = fVar25;
      }
      *(float *)(param_1 + 0x16c) = fVar26;
      *(float *)(param_1 + 0x170) = fVar22;
    }
  }
  iVar2 = Layout::helpPressed((Layout *)*puVar14);
  if (iVar2 != 0) {
    uVar11 = *puVar14;
    pSVar9 = (String *)GameText::getText(**(int **)(DAT_000dba28 + 0xdb904));
    AbyssEngine::String::String(aSStack_94,pSVar9,false);
    Layout::initHelpWindow(uVar11,aSStack_94);
    AbyssEngine::String::~String(aSStack_94);
  }
LAB_000db932:
  iVar2 = *piVar18 - local_34;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  000dae90: push {r4,r5,r6,r7,lr}
  000dae92: add r7,sp,#0xc
  000dae94: push {r7,r8,r9,r10,r11}
  000dae98: vpush {d8}
  000dae9c: sub sp,#0x70
  000dae9e: mov r4,r0
  000daea0: ldr r0,[0x000db204]
  000daea2: mov r5,r2
  000daea4: mov r6,r1
  000daea6: add r0,pc
  000daea8: ldr.w r9,[r0,#0x0]
  000daeac: ldr.w r0,[r9,#0x0]
  000daeb0: str r0,[sp,#0x6c]
  000daeb2: ldrb.w r0,[r4,#0xa9]
  000daeb6: cbz r0,0x000daee4
  000daeb8: ldr r0,[r4,#0x5c]
  000daeba: mov r1,r6
  000daebc: mov r2,r5
  000daebe: blx 0x00074770
  000daec2: movs r5,#0x0
  000daec4: cmp r0,#0x1
  000daec6: beq 0x000dafb8
  000daec8: cmp r0,#0x0
  000daeca: bne.w 0x000db932
  000daece: ldrb.w r0,[r4,#0x1e4]
  000daed2: movs r5,#0x0
  000daed4: strb.w r5,[r4,#0xa9]
  000daed8: cmp r0,#0x0
  000daeda: beq 0x000dafc4
  000daedc: strb.w r5,[r4,#0x1e4]
  000daee0: b.w 0x000db932
  000daee4: ldrb.w r0,[r4,#0x138]
  000daee8: cmp r0,#0x0
  000daeea: bne.w 0x000db930
  000daeee: ldrb.w r0,[r4,#0x139]
  000daef2: cmp r0,#0x0
  000daef4: bne.w 0x000db930
  000daef8: ldr r0,[0x000db208]
  000daefa: add r0,pc
  000daefc: ldr.w r10,[r0,#0x0]
  000daf00: ldr.w r0,[r10,#0x0]
  000daf04: ldrb r1,[r0,#0x0]
  000daf06: cmp r1,#0x0
  000daf08: bne.w 0x000db070
  000daf0c: mov r1,r6
  000daf0e: mov r2,r5
  000daf10: blx 0x0007477c
  000daf14: cmp r0,#0x0
  000daf16: beq.w 0x000db070
  000daf1a: ldr r0,[r4,#0x4]
  000daf1c: cmp r0,#0x3
  000daf1e: bne.w 0x000db1e2
  000daf22: ldrb.w r0,[r4,#0xf4]
  000daf26: cmp r0,#0x0
  000daf28: beq.w 0x000db1e2
  000daf2c: ldr r0,[0x000db20c]
  000daf2e: movs r1,#0x1
  000daf30: movs r5,#0x0
  000daf32: strb.w r1,[r4,#0x139]
  000daf36: add r0,pc
  000daf38: strd r5,r5,[r4,#0x168]
  000daf3c: str.w r5,[r4,#0x170]
  000daf40: ldr.w r8,[r4,#0x17c]
  000daf44: ldr r1,[r0,#0x0]
  000daf46: str r1,[sp,#0x8]
  000daf48: mov r0,r8
  000daf4a: blx r1
  000daf4c: ldr r1,[0x000db210]
  000daf4e: mov r6,r0
  000daf50: ldr.w r0,[r4,#0x17c]
  000daf54: add r1,pc
  000daf56: ldr.w r11,[r1,#0x0]
  000daf5a: blx r11
  000daf5c: mov r2,r0
  000daf5e: ldr r0,[0x000db214]
  000daf60: mov r1,r6
  000daf62: add r0,pc
  000daf64: ldr.w r10,[r0,#0x0]
  000daf68: mov r0,r8
  000daf6a: blx r10
  000daf6c: ldr.w r6,[r4,#0x180]
  000daf70: ldr.w r8,[sp,#0x8]
  000daf74: mov r0,r6
  000daf76: blx r8
  000daf78: str r0,[sp,#0x4]
  000daf7a: ldr.w r0,[r4,#0x180]
  000daf7e: blx r11
  000daf80: ldr r1,[sp,#0x4]
  000daf82: mov r2,r0
  000daf84: mov r0,r6
  000daf86: blx r10
  000daf88: ldr.w r6,[r4,#0x184]
  000daf8c: mov r0,r6
  000daf8e: blx r8
  000daf90: mov r8,r0
  000daf92: ldr.w r0,[r4,#0x184]
  000daf96: blx r11
  000daf98: mov r2,r0
  000daf9a: mov r0,r6
  000daf9c: mov r1,r8
  000daf9e: blx r10
  000dafa0: ldr r0,[0x000db218]
  000dafa2: movs r1,#0x6b
  000dafa4: movs r2,#0x0
  000dafa6: movs r3,#0x0
  000dafa8: add r0,pc
  000dafaa: str r5,[sp,#0x0]
  000dafac: ldr r0,[r0,#0x0]
  000dafae: ldr r0,[r0,#0x0]
  000dafb0: blx 0x00071548
  000dafb4: b.w 0x000db932
  000dafb8: strb.w r5,[r4,#0xa9]
  000dafbc: strb.w r5,[r4,#0x120]
  000dafc0: b.w 0x000db932
  000dafc4: ldr r0,[r4,#0x4]
  000dafc6: cmp r0,#0x3
  000dafc8: bne 0x000daff0
  000dafca: ldr r0,[r4,#0x58]
  000dafcc: ldr r1,[r4,#0x64]
  000dafce: ldr r0,[r0,#0x4]
  000dafd0: ldr.w r0,[r0,r1,lsl #0x2]
  000dafd4: blx 0x00071824
  000dafd8: mov r6,r0
  000dafda: ldr r0,[0x000db21c]
  000dafdc: add r0,pc
  000dafde: ldr r0,[r0,#0x0]
  000dafe0: ldr r0,[r0,#0x0]
  000dafe2: blx 0x00071c14
  000dafe6: blx 0x00071824
  000dafea: cmp r6,r0
  000dafec: beq.w 0x000daedc
  000daff0: ldrb.w r0,[r4,#0x120]
  000daff4: cbz r0,0x000db00a
  000daff6: ldr r0,[r4,#0x4]
  000daff8: cbnz r0,0x000db00a
  000daffa: ldrb.w r0,[r4,#0xa8]
  000daffe: cbnz r0,0x000db00a
  000db000: ldrb.w r0,[r4,#0x118]
  000db004: cmp r0,#0x0
  000db006: beq.w 0x000db2f8
  000db00a: ldrb.w r0,[r4,#0xa8]
  000db00e: cmp r0,#0x0
  000db010: bne.w 0x000db930
  000db014: ldr r0,[r4,#0x4]
  000db016: cmp r0,#0x3
  000db018: bne.w 0x000db930
  000db01c: ldrb.w r0,[r4,#0xab]
  000db020: cmp r0,#0x0
  000db022: beq.w 0x000db186
  000db026: ldr.w r1,[r4,#0x1d0]
  000db02a: ldr.w r2,[r4,#0x1d8]
  000db02e: cmp r2,r1
  000db030: bge.w 0x000db186
  000db034: cmp r1,#0x1
  000db036: bne.w 0x000db930
  000db03a: ldrb.w r0,[r4,#0xaa]
  000db03e: cmp r0,#0x0
  000db040: beq.w 0x000db352
  000db044: ldr r0,[0x000db220]
  000db046: ldr r1,[r4,#0x58]
  000db048: ldr r2,[r4,#0x64]
  000db04a: add r0,pc
  000db04c: ldr r3,[0x000db224]
  000db04e: ldr r5,[0x000db228]
  000db050: ldr r1,[r1,#0x4]
  000db052: add r3,pc
  000db054: add r5,pc
  000db056: ldr r0,[r0,#0x0]
  000db058: ldr r3,[r3,#0x0]
  000db05a: ldr r6,[r5,#0x0]
  000db05c: movs r5,#0x1
  000db05e: ldr.w r1,[r1,r2,lsl #0x2]
  000db062: movs r2,#0x0
  000db064: strb r2,[r0,#0x0]
  000db066: str r1,[r6,#0x0]
  000db068: ldr r1,[r4,#0x74]
  000db06a: strb r5,[r4,#0x0]
  000db06c: ldr r0,[r3,#0x0]
  000db06e: b 0x000db34c
  000db070: ldrb.w r0,[r4,#0xa8]
  000db074: cbz r0,0x000db080
  000db076: ldrb.w r0,[r4,#0x13a]
  000db07a: cmp r0,#0x0
  000db07c: bne.w 0x000db930
  000db080: ldr r0,[r4,#0x4c]
  000db082: mov r1,r6
  000db084: mov r2,r5
  000db086: blx 0x00074788
  000db08a: cbz r0,0x000db098
  000db08c: ldrb.w r0,[r4,#0x108]
  000db090: eor r0,r0,#0x1
  000db094: strb.w r0,[r4,#0x108]
  000db098: ldrb.w r0,[r4,#0x174]
  000db09c: cmp r0,#0x0
  000db09e: beq.w 0x000db8ee
  000db0a2: vldr.32 s0,[r4,#0x150]
  000db0a6: vmov.f32 s8,0x40400000
  000db0aa: vldr.32 s2,[r4,#0x154]
  000db0ae: movs r1,#0x0
  000db0b0: vcmpe.f32 s0,#0
  000db0b4: vldr.32 s16,[pc,#0x174]
  000db0b8: vmrs apsr,fpscr
  000db0bc: vneg.f32 s4,s0
  000db0c0: vcmpe.f32 s2,#0
  000db0c4: ldr r0,[0x000db230]
  000db0c6: vneg.f32 s6,s2
  000db0ca: it gt
  000db0cc: vmov.gt.f32 s4,s0
  000db0d0: vmrs apsr,fpscr
  000db0d4: vcmpe.f32 s4,s8
  000db0d8: vmov.f32 s4,s16
  000db0dc: it gt
  000db0de: vmov.gt.f32 s6,s2
  000db0e2: vmrs apsr,fpscr
  000db0e6: vcmpe.f32 s6,s8
  000db0ea: it gt
  000db0ec: vmov.gt.f32 s4,s0
  000db0f0: vmrs apsr,fpscr
  000db0f4: vmov.f32 s0,s16
  000db0f8: it gt
  000db0fa: vmov.gt.f32 s0,s2
  000db0fe: str.w r0,[r4,#0x168]
  000db102: ldr r0,[r4,#0x4]
  000db104: strb.w r1,[r4,#0x174]
  000db108: cmp r0,#0x0
  000db10a: vstr.32 s4,[r4,#0x16c]
  000db10e: vstr.32 s0,[r4,#0x170]
  000db112: beq.w 0x000db254
  000db116: ldr r0,[r4,#0x64]
  000db118: cmp r0,#0x0
  000db11a: blt.w 0x000db896
  000db11e: ldr.w r1,[r4,#0x1a0]
  000db122: cmp r1,r0
  000db124: bne.w 0x000db2d0
  000db128: ldrb.w r1,[r4,#0xa8]
  000db12c: cmp r1,#0x0
  000db12e: bne.w 0x000db896
  000db132: ldr r1,[r4,#0x58]
  000db134: ldr r1,[r1,#0x4]
  000db136: ldr.w r0,[r1,r0,lsl #0x2]
  000db13a: blx 0x00071824
  000db13e: mov r6,r0
  000db140: ldr r0,[0x000db234]
  000db142: add r0,pc
  000db144: ldr.w r8,[r0,#0x0]
  000db148: ldr.w r0,[r8,#0x0]
  000db14c: blx 0x00071c14
  000db150: blx 0x00071824
  000db154: ldr r5,[r4,#0x5c]
  000db156: cmp r6,r0
  000db158: bne.w 0x000db388
  000db15c: cbnz r5,0x000db16c
  000db15e: movs r0,#0x5c
  000db160: blx 0x0006eb24
  000db164: mov r5,r0
  000db166: blx 0x00074584
  000db16a: str r5,[r4,#0x5c]
  000db16c: ldr r0,[0x000db238]
  000db16e: movw r1,#0x1a3
  000db172: add r0,pc
  000db174: ldr r0,[r0,#0x0]
  000db176: ldr r0,[r0,#0x0]
  000db178: blx 0x00072f70
  000db17c: mov r1,r0
  000db17e: mov r0,r5
  000db180: blx 0x00074794
  000db184: b 0x000db890
  000db186: ldrb.w r1,[r4,#0xaa]
  000db18a: cmp r1,#0x0
  000db18c: beq.w 0x000db2ee
  000db190: ldr r2,[r4,#0x58]
  000db192: cmp r0,#0x0
  000db194: ldr r1,[r4,#0x64]
  000db196: ldr r3,[0x000db23c]
  000db198: ldr r2,[r2,#0x4]
  000db19a: add r3,pc
  000db19c: ldr r3,[r3,#0x0]
  000db19e: ldr.w r6,[r2,r1,lsl #0x2]
  000db1a2: str r6,[r3,#0x0]
  000db1a4: beq.w 0x000db334
  000db1a8: ldr.w r0,[r2,r1,lsl #0x2]
  000db1ac: blx 0x0007189c
  000db1b0: mov r5,r0
  000db1b2: ldr r0,[0x000db240]
  000db1b4: add r0,pc
  000db1b6: ldr r0,[r0,#0x0]
  000db1b8: ldr r0,[r0,#0x0]
  000db1ba: blx 0x00071a10
  000db1be: blx 0x00071a7c
  000db1c2: ldr r1,[0x000db244]
  000db1c4: subs r0,r5,r0
  000db1c6: it ne
  000db1c8: mov.ne r0,#0x1
  000db1ca: add r1,pc
  000db1cc: ldr r1,[r1,#0x0]
  000db1ce: strb r0,[r1,#0x0]
  000db1d0: beq.w 0x000db33e
  000db1d4: ldr r0,[0x000db248]
  000db1d6: ldr.w r1,[r4,#0x1d0]
  000db1da: add r0,pc
  000db1dc: ldr r0,[r0,#0x0]
  000db1de: str r1,[r0,#0x0]
  000db1e0: b 0x000db33e
  000db1e2: ldr r0,[0x000db24c]
  000db1e4: ldr r1,[r4,#0x74]
  000db1e6: add r0,pc
  000db1e8: ldr r0,[r0,#0x0]
  000db1ea: ldr r0,[r0,#0x0]
  000db1ec: blx 0x0006fd9c
  000db1f0: ldr r0,[0x000db250]
  000db1f2: movs r1,#0x66
  000db1f4: add r0,pc
  000db1f6: ldr r0,[r0,#0x0]
  000db1f8: ldr r0,[r0,#0x0]
  000db1fa: blx 0x000724a8
  000db1fe: movs r5,#0x1
  000db200: b 0x000db932
  000db254: ldr r0,[r4,#0x60]
  000db256: cmp r0,#0x0
  000db258: blt.w 0x000db8ee
  000db25c: ldrb.w r1,[r4,#0x118]
  000db260: cbnz r1,0x000db2c0
  000db262: ldrb.w r1,[r4,#0xa8]
  000db266: cbnz r1,0x000db2c0
  000db268: ldr.w r1,[r4,#0x19c]
  000db26c: cmp r1,r0
  000db26e: bne 0x000db2c0
  000db270: ldrb.w r0,[r4,#0xab]
  000db274: cmp r0,#0x0
  000db276: beq.w 0x000db6c8
  000db27a: ldr r0,[0x000db360]
  000db27c: add r0,pc
  000db27e: ldr r5,[r0,#0x0]
  000db280: ldr r0,[r5,#0x0]
  000db282: blx 0x00071a58
  000db286: blx 0x000722e0
  000db28a: cmp r0,#0x0
  000db28c: beq.w 0x000db6fa
  000db290: ldr r0,[r5,#0x0]
  000db292: blx 0x00071a10
  000db296: mov r5,r0
  000db298: ldr r0,[r4,#0x54]
  000db29a: ldr r1,[r4,#0x60]
  000db29c: ldr r0,[r0,#0x4]
  000db29e: ldr.w r0,[r0,r1,lsl #0x2]
  000db2a2: blx 0x00071a7c
  000db2a6: mov r1,r0
  000db2a8: mov r0,r5
  000db2aa: blx 0x000747a0
  000db2ae: cmp r0,#0x0
  000db2b0: bne.w 0x000db6fa
  000db2b4: ldr r0,[0x000db364]
  000db2b6: mov.w r1,#0x264
  000db2ba: ldr r6,[r4,#0x5c]
  000db2bc: add r0,pc
  000db2be: b 0x000db7d6
  000db2c0: mov.w r0,#0xffffffff
  000db2c4: str.w r0,[r4,#0x19c]
  000db2c8: movs r0,#0x1
  000db2ca: strb.w r0,[r4,#0x13a]
  000db2ce: b 0x000db8ee
  000db2d0: ldr r0,[0x000db368]
  000db2d2: movs r1,#0x0
  000db2d4: str r1,[sp,#0x0]
  000db2d6: movs r1,#0x69
  000db2d8: add r0,pc
  000db2da: movs r2,#0x0
  000db2dc: movs r3,#0x0
  000db2de: ldr r0,[r0,#0x0]
  000db2e0: ldr r0,[r0,#0x0]
  000db2e2: blx 0x00071548
  000db2e6: movs r0,#0x1
  000db2e8: strb.w r0,[r4,#0x13b]
  000db2ec: b 0x000db896
  000db2ee: mov r0,r4
  000db2f0: movs r1,#0x1
  000db2f2: blx 0x000747ac
  000db2f6: b 0x000db930
  000db2f8: ldr r0,[0x000db36c]
  000db2fa: ldr r1,[0x000db370]
  000db2fc: add r0,pc
  000db2fe: ldr r2,[0x000db374]
  000db300: ldr r3,[0x000db378]
  000db302: add r1,pc
  000db304: ldr r0,[r0,#0x0]
  000db306: add r2,pc
  000db308: ldr r5,[0x000db37c]
  000db30a: add r3,pc
  000db30c: ldr r1,[r1,#0x0]
  000db30e: ldr r0,[r0,#0x0]
  000db310: add r5,pc
  000db312: ldr r2,[r2,#0x0]
  000db314: ldr r6,[r5,#0x0]
  000db316: movs r5,#0x0
  000db318: ldr r0,[r0,#0x78]
  000db31a: str r0,[r1,#0x0]
  000db31c: strb.w r5,[r4,#0x120]
  000db320: movs r5,#0x1
  000db322: strb r5,[r4,#0x0]
  000db324: ldr r3,[r3,#0x0]
  000db326: strb r5,[r2,#0x0]
  000db328: ldr.w r1,[r4,#0x1d0]
  000db32c: str r1,[r6,#0x0]
  000db32e: ldr r0,[r3,#0x0]
  000db330: ldr r1,[r4,#0x74]
  000db332: b 0x000db34c
  000db334: ldr r0,[0x000db380]
  000db336: movs r1,#0x0
  000db338: add r0,pc
  000db33a: ldr r0,[r0,#0x0]
  000db33c: strb r1,[r0,#0x0]
  000db33e: ldr r0,[0x000db384]
  000db340: movs r5,#0x1
  000db342: ldr r1,[r4,#0x74]
  000db344: add r0,pc
  000db346: strb r5,[r4,#0x0]
  000db348: ldr r0,[r0,#0x0]
  000db34a: ldr r0,[r0,#0x0]
  000db34c: blx 0x0006fd9c
  000db350: b 0x000db932
  000db352: mov r0,r4
  000db354: movs r1,#0x0
  000db356: movs r5,#0x0
  000db358: blx 0x000747ac
  000db35c: b 0x000db932
  000db388: cbnz r5,0x000db398
  000db38a: movs r0,#0x5c
  000db38c: blx 0x0006eb24
  000db390: mov r5,r0
  000db392: blx 0x00074584
  000db396: str r5,[r4,#0x5c]
  000db398: ldr.w r0,[r8,#0x0]
  000db39c: blx 0x00071770
  000db3a0: cmp r0,#0x18
  000db3a2: bne 0x000db3ec
  000db3a4: ldr r0,[r4,#0x58]
  000db3a6: ldr r1,[r4,#0x64]
  000db3a8: ldr r0,[r0,#0x4]
  000db3aa: ldr.w r0,[r0,r1,lsl #0x2]
  000db3ae: blx 0x00071824
  000db3b2: mov r5,r0
  000db3b4: ldr.w r0,[r8,#0x0]
  000db3b8: blx 0x0007285c
  000db3bc: blx 0x00073738
  000db3c0: cmp r5,r0
  000db3c2: bne 0x000db3ec
  000db3c4: ldr.w r0,[r8,#0x0]
  000db3c8: blx 0x00071a58
  000db3cc: movs r1,#0xd
  000db3ce: blx 0x0007228c
  000db3d2: cmp r0,#0x0
  000db3d4: beq.w 0x000db7b4
  000db3d8: ldr.w r0,[r8,#0x0]
  000db3dc: blx 0x00071a58
  000db3e0: movs r1,#0x11
  000db3e2: blx 0x0007228c
  000db3e6: cmp r0,#0x0
  000db3e8: beq.w 0x000db7b4
  000db3ec: ldr.w r0,[r8,#0x0]
  000db3f0: blx 0x00071770
  000db3f4: cmp r0,#0x87
  000db3f6: bne 0x000db42c
  000db3f8: ldr r0,[r4,#0x58]
  000db3fa: ldr r1,[r4,#0x64]
  000db3fc: ldr r0,[r0,#0x4]
  000db3fe: ldr.w r0,[r0,r1,lsl #0x2]
  000db402: blx 0x00071824
  000db406: mov r5,r0
  000db408: ldr.w r0,[r8,#0x0]
  000db40c: blx 0x0007285c
  000db410: blx 0x00073738
  000db414: cmp r5,r0
  000db416: bne 0x000db42c
  000db418: ldr.w r0,[r8,#0x0]
  000db41c: blx 0x00071a58
  000db420: movs r1,#0x13
  000db422: blx 0x0007228c
  000db426: cmp r0,#0x0
  000db428: beq.w 0x000db7f2
  000db42c: ldr.w r0,[r8,#0x0]
  000db430: blx 0x00071770
  000db434: cmp r0,#0x5b
  000db436: bne 0x000db46a
  000db438: ldr r0,[r4,#0x58]
  000db43a: ldr r1,[r4,#0x64]
  000db43c: ldr r0,[r0,#0x4]
  000db43e: ldr.w r0,[r0,r1,lsl #0x2]
  000db442: blx 0x00071824
  000db446: mov r5,r0
  000db448: ldr.w r0,[r8,#0x0]
  000db44c: blx 0x0007285c
  000db450: blx 0x00073738
  000db454: cmp r5,r0
  000db456: bne 0x000db46a
  000db458: ldr.w r0,[r8,#0x0]
  000db45c: blx 0x00071a58
  000db460: blx 0x00072bd4
  000db464: cmp r0,#0x9
  000db466: ble.w 0x000db7a8
  000db46a: ldr.w r0,[r8,#0x0]
  000db46e: blx 0x00071770
  000db472: cmp r0,#0x5e
  000db474: bne 0x000db4a8
  000db476: ldr r0,[r4,#0x58]
  000db478: ldr r1,[r4,#0x64]
  000db47a: ldr r0,[r0,#0x4]
  000db47c: ldr.w r0,[r0,r1,lsl #0x2]
  000db480: blx 0x00071824
  000db484: mov r5,r0
  000db486: ldr.w r0,[r8,#0x0]
  000db48a: blx 0x0007285c
  000db48e: blx 0x00073738
  000db492: cmp r5,r0
  000db494: bne 0x000db4a8
  000db496: ldr.w r0,[r8,#0x0]
  000db49a: blx 0x00071a58
  000db49e: blx 0x00072bd4
  000db4a2: cmp r0,#0x0
  000db4a4: beq.w 0x000db7a8
  000db4a8: ldr.w r0,[r8,#0x0]
  000db4ac: blx 0x00071770
  000db4b0: cmp r0,#0x69
  000db4b2: bne 0x000db4ec
  000db4b4: ldr r0,[r4,#0x58]
  000db4b6: ldr r1,[r4,#0x64]
  000db4b8: ldr r0,[r0,#0x4]
  000db4ba: ldr.w r0,[r0,r1,lsl #0x2]
  000db4be: blx 0x00071824
  000db4c2: mov r5,r0
  000db4c4: ldr.w r0,[r8,#0x0]
  000db4c8: blx 0x0007285c
  000db4cc: blx 0x00073738
  000db4d0: cmp r5,r0
  000db4d2: bne 0x000db4ec
  000db4d4: ldr.w r0,[r8,#0x0]
  000db4d8: blx 0x00071a58
  000db4dc: movs r1,#0xce
  000db4de: movs r2,#0x1
  000db4e0: movs r6,#0x1
  000db4e2: blx 0x00073978
  000db4e6: cmp r0,#0x0
  000db4e8: beq.w 0x000db7fe
  000db4ec: ldr.w r0,[r8,#0x0]
  000db4f0: blx 0x00071770
  000db4f4: cmp r0,#0x8b
  000db4f6: bne 0x000db52a
  000db4f8: ldr r0,[r4,#0x58]
  000db4fa: ldr r1,[r4,#0x64]
  000db4fc: ldr r0,[r0,#0x4]
  000db4fe: ldr.w r0,[r0,r1,lsl #0x2]
  000db502: blx 0x00071824
  000db506: mov r5,r0
  000db508: ldr.w r0,[r8,#0x0]
  000db50c: blx 0x0007285c
  000db510: blx 0x00073738
  000db514: cmp r5,r0
  000db516: bne 0x000db52a
  000db518: ldr.w r0,[r8,#0x0]
  000db51c: blx 0x00071a58
  000db520: blx 0x000719c8
  000db524: cmp r0,#0x2a
  000db526: bne.w 0x000db81e
  000db52a: ldr.w r0,[r8,#0x0]
  000db52e: blx 0x00071770
  000db532: cmp r0,#0x8e
  000db534: bne 0x000db594
  000db536: ldr r0,[r4,#0x58]
  000db538: ldr r1,[r4,#0x64]
  000db53a: ldr r0,[r0,#0x4]
  000db53c: ldr.w r0,[r0,r1,lsl #0x2]
  000db540: blx 0x00071824
  000db544: mov r5,r0
  000db546: ldr.w r0,[r8,#0x0]
  000db54a: blx 0x0007285c
  000db54e: blx 0x00073738
  000db552: cmp r5,r0
  000db554: bne 0x000db594
  000db556: ldr.w r0,[r8,#0x0]
  000db55a: blx 0x00071a58
  000db55e: movs r1,#0x21
  000db560: blx 0x0007228c
  000db564: cmp r0,#0x0
  000db566: beq.w 0x000db7c0
  000db56a: ldr.w r0,[r8,#0x0]
  000db56e: blx 0x00071a58
  000db572: movs r1,#0xc5
  000db574: movs r2,#0xf
  000db576: blx 0x00073978
  000db57a: cmp r0,#0x0
  000db57c: beq.w 0x000db7c0
  000db580: ldr.w r0,[r8,#0x0]
  000db584: blx 0x00071a58
  000db588: movs r1,#0x23
  000db58a: blx 0x0007228c
  000db58e: cmp r0,#0x0
  000db590: beq.w 0x000db7c0
  000db594: ldr.w r0,[r8,#0x0]
  000db598: blx 0x00071770
  000db59c: cmp r0,#0x8e
  000db59e: bne 0x000db5d2
  000db5a0: ldr r0,[r4,#0x58]
  000db5a2: ldr r1,[r4,#0x64]
  000db5a4: ldr r0,[r0,#0x4]
  000db5a6: ldr.w r0,[r0,r1,lsl #0x2]
  000db5aa: blx 0x00071824
  000db5ae: mov r5,r0
  000db5b0: ldr.w r0,[r8,#0x0]
  000db5b4: blx 0x0007285c
  000db5b8: blx 0x00073738
  000db5bc: cmp r5,r0
  000db5be: bne 0x000db5d2
  000db5c0: ldr.w r0,[r8,#0x0]
  000db5c4: blx 0x00071a58
  000db5c8: blx 0x000722ec
  000db5cc: cmp r0,#0x0
  000db5ce: beq.w 0x000db870
  000db5d2: ldrb.w r0,[r4,#0xab]
  000db5d6: cbz r0,0x000db622
  000db5d8: ldr.w r0,[r4,#0x1d0]
  000db5dc: ldr.w r1,[r4,#0x1d8]
  000db5e0: cmp r1,r0
  000db5e2: bge 0x000db5fc
  000db5e4: ldr.w r0,[r8,#0x0]
  000db5e8: blx 0x00071a58
  000db5ec: blx 0x000722e0
  000db5f0: cmp r0,#0x0
  000db5f2: beq.w 0x000db858
  000db5f6: ldrb.w r0,[r4,#0xab]
  000db5fa: cbz r0,0x000db622
  000db5fc: ldrb.w r0,[r4,#0x1d4]
  000db600: cbz r0,0x000db622
  000db602: ldr.w r0,[r4,#0x1d0]
  000db606: ldr.w r1,[r4,#0x1d8]
  000db60a: cmp.w r1,r0, lsl #0x1
  000db60e: bge 0x000db622
  000db610: ldr.w r0,[r8,#0x0]
  000db614: blx 0x00071a58
  000db618: blx 0x000722e0
  000db61c: cmp r0,#0x0
  000db61e: beq.w 0x000db95a
  000db622: ldr r0,[0x000db9d4]
  000db624: movw r1,#0x23e
  000db628: ldr.w r8,[r4,#0x5c]
  000db62c: add r0,pc
  000db62e: ldr.w r11,[r0,#0x0]
  000db632: ldr.w r0,[r11,#0x0]
  000db636: blx 0x00072f70
  000db63a: ldr r1,[0x000db9d8]
  000db63c: add r5,sp,#0x30
  000db63e: mov r6,r0
  000db640: movs r2,#0x0
  000db642: add r1,pc
  000db644: mov r0,r5
  000db646: blx 0x0006ee18
  000db64a: add r0,sp,#0x3c
  000db64c: mov r1,r6
  000db64e: mov r2,r5
  000db650: blx 0x0006ef98
  000db654: ldr r0,[r4,#0x58]
  000db656: ldr r1,[r4,#0x64]
  000db658: ldr r0,[r0,#0x4]
  000db65a: ldr.w r1,[r0,r1,lsl #0x2]
  000db65e: add r0,sp,#0x24
  000db660: blx 0x000736a8
  000db664: add r0,sp,#0x48
  000db666: add r1,sp,#0x3c
  000db668: add r2,sp,#0x24
  000db66a: blx 0x0006ef98
  000db66e: ldr r1,[0x000dba20]
  000db670: add r1,pc
  000db672: add r0,sp,#0x18
  000db674: movs r2,#0x0
  000db676: blx 0x0006ee18
  000db67a: add r0,sp,#0x54
  000db67c: add r1,sp,#0x48
  000db67e: add r2,sp,#0x18
  000db680: blx 0x0006ef98
  000db684: ldr.w r0,[r11,#0x0]
  000db688: movw r1,#0x1a5
  000db68c: blx 0x00072f70
  000db690: mov r2,r0
  000db692: add r0,sp,#0x60
  000db694: add r1,sp,#0x54
  000db696: blx 0x0006ef98
  000db69a: add r1,sp,#0x60
  000db69c: mov r0,r8
  000db69e: movs r2,#0x1
  000db6a0: blx 0x000746f8
  000db6a4: ldr r0,[0x000dba24]
  000db6a6: add r0,pc
  000db6a8: ldr r5,[r0,#0x0]
  000db6aa: add r0,sp,#0x60
  000db6ac: blx r5
  000db6ae: add r0,sp,#0x54
  000db6b0: blx r5
  000db6b2: add r0,sp,#0x18
  000db6b4: blx r5
  000db6b6: add r0,sp,#0x48
  000db6b8: blx r5
  000db6ba: add r0,sp,#0x24
  000db6bc: blx r5
  000db6be: add r0,sp,#0x3c
  000db6c0: blx r5
  000db6c2: add r0,sp,#0x30
  000db6c4: blx r5
  000db6c6: b 0x000db890
  000db6c8: ldr r0,[0x000db9e4]
  000db6ca: add r0,pc
  000db6cc: ldr r0,[r0,#0x0]
  000db6ce: ldr r0,[r0,#0x0]
  000db6d0: blx 0x00071a10
  000db6d4: mov r5,r0
  000db6d6: ldr r0,[r4,#0x54]
  000db6d8: ldr r1,[r4,#0x60]
  000db6da: ldr r0,[r0,#0x4]
  000db6dc: ldr.w r0,[r0,r1,lsl #0x2]
  000db6e0: blx 0x00071a7c
  000db6e4: mov r1,r0
  000db6e6: mov r0,r5
  000db6e8: blx 0x000747a0
  000db6ec: cmp r0,#0x0
  000db6ee: beq 0x000db7cc
  000db6f0: ldrb.w r0,[r4,#0xab]
  000db6f4: cmp r0,#0x0
  000db6f6: bne.w 0x000db27a
  000db6fa: ldr r0,[0x000db9ec]
  000db6fc: movs r5,#0x0
  000db6fe: movs r1,#0x6a
  000db700: movs r2,#0x0
  000db702: add r0,pc
  000db704: movs r3,#0x0
  000db706: str r5,[sp,#0x0]
  000db708: ldr r0,[r0,#0x0]
  000db70a: ldr r0,[r0,#0x0]
  000db70c: blx 0x00071548
  000db710: mov r0,r4
  000db712: blx 0x0007456c
  000db716: movs r0,#0x1
  000db718: add r6,sp,#0x60
  000db71a: strb.w r0,[r4,#0x138]
  000db71e: ldr r1,[r4,#0x68]
  000db720: ldr r0,[r4,#0x60]
  000db722: ldr r1,[r1,#0x4]
  000db724: ldr.w r1,[r1,r0,lsl #0x2]
  000db728: mov r0,r6
  000db72a: blx 0x000720b8
  000db72e: add.w r0,r4,#0x78
  000db732: mov r1,r6
  000db734: blx 0x0006eb3c
  000db738: vldr.32 s2,[r4,#0x80]
  000db73c: vldr.32 s0,[pc,#0x2a0]
  000db740: ldr r0,[0x000db9f0]
  000db742: vadd.f32 s0,s2,s0
  000db746: add r0,pc
  000db748: ldr r0,[r0,#0x0]
  000db74a: vstr.32 s0,[r4,#0x80]
  000db74e: ldr r6,[r0,#0x0]
  000db750: mov r0,r6
  000db752: blx 0x000717f4
  000db756: mov r1,r0
  000db758: mov r0,r6
  000db75a: blx 0x0006ff1c
  000db75e: add r6,sp,#0x60
  000db760: mov r1,r0
  000db762: mov r0,r6
  000db764: blx 0x0006f16c
  000db768: add.w r0,r4,#0x84
  000db76c: mov r1,r6
  000db76e: blx 0x0006eb3c
  000db772: ldr r3,[0x000db9f4]
  000db774: ldr r2,[r4,#0x78]
  000db776: add r3,pc
  000db778: ldr.w r1,[r4,#0x84]
  000db77c: ldr.w r0,[r4,#0x17c]
  000db780: ldr r6,[r3,#0x0]
  000db782: blx r6
  000db784: ldr r2,[r4,#0x7c]
  000db786: ldr.w r1,[r4,#0x88]
  000db78a: ldr.w r0,[r4,#0x180]
  000db78e: blx r6
  000db790: ldr.w r2,[r4,#0x80]
  000db794: ldr.w r1,[r4,#0x8c]
  000db798: ldr.w r0,[r4,#0x184]
  000db79c: blx r6
  000db79e: strd r5,r5,[r4,#0x168]
  000db7a2: str.w r5,[r4,#0x170]
  000db7a6: b 0x000db932
  000db7a8: ldr r0,[0x000dba00]
  000db7aa: movw r1,#0xc8e
  000db7ae: ldr r5,[r4,#0x5c]
  000db7b0: add r0,pc
  000db7b2: b 0x000db87a
  000db7b4: ldr r0,[0x000db9f8]
  000db7b6: mov.w r1,#0x214
  000db7ba: ldr r5,[r4,#0x5c]
  000db7bc: add r0,pc
  000db7be: b 0x000db87a
  000db7c0: ldr r0,[0x000dba10]
  000db7c2: mov.w r1,#0xc90
  000db7c6: ldr r5,[r4,#0x5c]
  000db7c8: add r0,pc
  000db7ca: b 0x000db87a
  000db7cc: ldr r0,[0x000db9e8]
  000db7ce: mov.w r1,#0x1a4
  000db7d2: ldr r6,[r4,#0x5c]
  000db7d4: add r0,pc
  000db7d6: ldr r0,[r0,#0x0]
  000db7d8: ldr r0,[r0,#0x0]
  000db7da: blx 0x00072f70
  000db7de: mov r1,r0
  000db7e0: mov r0,r6
  000db7e2: movs r2,#0x0
  000db7e4: movs r5,#0x0
  000db7e6: blx 0x000746f8
  000db7ea: movs r0,#0x1
  000db7ec: strb.w r0,[r4,#0xa9]
  000db7f0: b 0x000db932
  000db7f2: ldr r0,[0x000db9fc]
  000db7f4: movw r1,#0xc8d
  000db7f8: ldr r5,[r4,#0x5c]
  000db7fa: add r0,pc
  000db7fc: b 0x000db87a
  000db7fe: ldr r0,[0x000dba04]
  000db800: movw r1,#0xc91
  000db804: ldr r5,[r4,#0x5c]
  000db806: add r0,pc
  000db808: ldr r0,[r0,#0x0]
  000db80a: ldr r0,[r0,#0x0]
  000db80c: blx 0x00072f70
  000db810: mov r1,r0
  000db812: mov r0,r5
  000db814: blx 0x00074794
  000db818: strb.w r6,[r4,#0x1e4]
  000db81c: b 0x000db890
  000db81e: ldr.w r0,[r8,#0x0]
  000db822: blx 0x00071a58
  000db826: blx 0x000719c8
  000db82a: ldr r1,[0x000dba08]
  000db82c: add r1,pc
  000db82e: ldr.w r0,[r1,r0,lsl #0x2]
  000db832: cmp r0,#0x1
  000db834: bne 0x000db84c
  000db836: ldr.w r0,[r8,#0x0]
  000db83a: blx 0x00071a58
  000db83e: movs r1,#0xbe
  000db840: movs r2,#0x1
  000db842: blx 0x00073978
  000db846: cmp r0,#0x0
  000db848: bne.w 0x000db52a
  000db84c: ldr r0,[0x000dba0c]
  000db84e: movw r1,#0xc8f
  000db852: ldr r5,[r4,#0x5c]
  000db854: add r0,pc
  000db856: b 0x000db87a
  000db858: ldr r0,[0x000dba18]
  000db85a: ldr.w r1,[r4,#0x1d0]
  000db85e: add r0,pc
  000db860: ldr r5,[r4,#0x5c]
  000db862: cmp r1,#0x1
  000db864: ldr r0,[r0,#0x0]
  000db866: ldr r0,[r0,#0x0]
  000db868: bne 0x000db952
  000db86a: movw r1,#0x246
  000db86e: b 0x000db968
  000db870: ldr r0,[0x000dba14]
  000db872: movw r1,#0xc92
  000db876: ldr r5,[r4,#0x5c]
  000db878: add r0,pc
  000db87a: ldr r0,[r0,#0x0]
  000db87c: ldr r0,[r0,#0x0]
  000db87e: blx 0x00072f70
  000db882: mov r1,r0
  000db884: mov r0,r5
  000db886: blx 0x00074794
  000db88a: movs r0,#0x1
  000db88c: strb.w r0,[r4,#0x1e4]
  000db890: movs r0,#0x1
  000db892: strb.w r0,[r4,#0xa9]
  000db896: vldr.32 s0,[r4,#0x150]
  000db89a: vldr.32 s2,[r4,#0x154]
  000db89e: vcmpe.f32 s0,#0
  000db8a2: vldr.32 s8,[pc,#0x138]
  000db8a6: vmrs apsr,fpscr
  000db8aa: vneg.f32 s4,s0
  000db8ae: vcmpe.f32 s2,#0
  000db8b2: vneg.f32 s6,s2
  000db8b6: it gt
  000db8b8: vmov.gt.f32 s4,s0
  000db8bc: vmrs apsr,fpscr
  000db8c0: vcmpe.f32 s4,s8
  000db8c4: vmov.f32 s4,s16
  000db8c8: it gt
  000db8ca: vmov.gt.f32 s6,s2
  000db8ce: vmrs apsr,fpscr
  000db8d2: vcmpe.f32 s6,s8
  000db8d6: it gt
  000db8d8: vmov.gt.f32 s4,s0
  000db8dc: vmrs apsr,fpscr
  000db8e0: it gt
  000db8e2: vmov.gt.f32 s16,s2
  000db8e6: vstr.32 s4,[r4,#0x16c]
  000db8ea: vstr.32 s16,[r4,#0x170]
  000db8ee: ldr.w r0,[r10,#0x0]
  000db8f2: blx 0x000747b8
  000db8f6: cbz r0,0x000db930
  000db8f8: ldr r0,[0x000dba28]
  000db8fa: movw r1,#0x277
  000db8fe: ldr r2,[r4,#0x4]
  000db900: add r0,pc
  000db902: ldr.w r4,[r10,#0x0]
  000db906: cmp r2,#0x0
  000db908: ldr r0,[r0,#0x0]
  000db90a: ldr r0,[r0,#0x0]
  000db90c: it eq
  000db90e: mov.eq.w r1,#0x274
  000db912: blx 0x00072f70
  000db916: add r5,sp,#0xc
  000db918: mov r1,r0
  000db91a: movs r2,#0x0
  000db91c: mov r0,r5
  000db91e: blx 0x0006f028
  000db922: mov r0,r4
  000db924: mov r1,r5
  000db926: blx 0x000747c4
  000db92a: add r0,sp,#0xc
  000db92c: blx 0x0006ee30
  000db930: movs r5,#0x0
  000db932: ldr r0,[sp,#0x6c]
  000db934: ldr.w r1,[r9,#0x0]
  000db938: subs r0,r1,r0
  000db93a: itttt eq
  000db93c: mov.eq r0,r5
  000db93e: add.eq sp,#0x70
  000db940: vpop.eq {d8}
  000db944: add.eq sp,#0x4
  000db946: itt eq
  000db948: pop.eq.w {r8,r9,r10,r11}
  000db94c: pop.eq {r4,r5,r6,r7,pc}
  000db94e: blx 0x0006e824
  000db952: movw r1,#0x243
  000db956: b.w 0x000db178
  000db95a: ldr r0,[0x000dba1c]
  000db95c: movw r1,#0x245
  000db960: ldr r5,[r4,#0x5c]
  000db962: add r0,pc
  000db964: ldr r0,[r0,#0x0]
  000db966: ldr r0,[r0,#0x0]
  000db968: blx 0x00072f70
  000db96c: mov r1,r0
  000db96e: mov r0,r5
  000db970: movs r2,#0x1
  000db972: blx 0x000746f8
  000db976: b 0x000db890
```
