# ModStation::OnInitialize
elf 0xd5708 body 5868
Sig: undefined __stdcall OnInitialize(void)

## decompile
```c

/* ModStation::OnInitialize() */

void ModStation::OnInitialize(void)

{
  char cVar1;
  longlong lVar2;
  bool bVar3;
  int in_r0;
  CutScene *this;
  int iVar4;
  Station *pSVar5;
  int iVar6;
  Generator *pGVar7;
  undefined4 uVar8;
  DialogueWindow *pDVar9;
  String *pSVar10;
  Agent *this_00;
  Vector *pVVar11;
  void *pvVar12;
  TouchButton *pTVar13;
  Array *pAVar14;
  String *pSVar15;
  NewsTicker *this_01;
  ChoiceWindow *pCVar16;
  uint *puVar17;
  uint *puVar18;
  float *pfVar19;
  EaseInOut *pEVar20;
  Matrix *pMVar21;
  EaseInOutMatrix *pEVar22;
  Item *pIVar23;
  Ship *pSVar24;
  Mission *this_02;
  int iVar25;
  int iVar26;
  undefined4 *puVar27;
  int iVar28;
  PaintCanvas *this_03;
  int iVar29;
  code *pcVar30;
  int *piVar31;
  int iVar32;
  uint uVar33;
  int *piVar34;
  int *piVar35;
  char *pcVar36;
  int *piVar37;
  int iVar38;
  bool bVar39;
  uint in_fpscr;
  float in_s0;
  float extraout_s0;
  float fVar40;
  float fVar41;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  undefined8 uVar42;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s1_07;
  float extraout_s2;
  float extraout_s2_00;
  float fVar43;
  undefined4 uVar44;
  undefined4 uVar45;
  undefined4 uVar46;
  longlong lVar47;
  undefined4 local_134;
  undefined4 local_130;
  undefined4 local_12c;
  float local_f8;
  float local_f4;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 uStack_dc;
  undefined4 local_d8;
  undefined4 uStack_d4;
  undefined8 local_d0;
  undefined8 local_c8;
  undefined4 local_c0;
  String aSStack_b8 [12];
  float local_ac;
  float local_a8;
  String aSStack_a0 [12];
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  undefined1 auStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  int local_4c;
  
  piVar34 = *(int **)(DAT_000e5a3c + 0xe5720);
  local_4c = *piVar34;
  *(undefined4 *)(in_r0 + 0x9c) = 0;
  if (*(int *)(in_r0 + 0x14) == 0) {
    this = operator_new(0xa0);
    CutScene::CutScene(this,0x17);
    *(CutScene **)(in_r0 + 0x14) = this;
    in_s0 = (float)CutScene::initialize();
  }
  iVar29 = DAT_000e5e44;
  iVar6 = DAT_000e5e40;
  iVar4 = *(int *)(in_r0 + 0xc);
  if (iVar4 == 0x14) {
    if (((*(char *)(in_r0 + 0xb1) == '\0') && (*(char *)(in_r0 + 0xb0) == '\0')) &&
       (*(char *)(in_r0 + 0x101) != '\0')) {
      iVar6 = Status::getCurrentCampaignMission();
      if (iVar6 == 0x4d) {
        pSVar5 = (Station *)Status::getStation();
        iVar6 = Station::getIndex(pSVar5);
        if (iVar6 == 0x65) goto LAB_000e5f72;
      }
      autosave();
    }
LAB_000e5f72:
    piVar31 = *(int **)(DAT_000e617c + 0xe5f78);
    iVar6 = Status::getCurrentCampaignMission();
    if (iVar6 == 1) {
      pSVar24 = (Ship *)*piVar31;
      Ship::makeShip(**(int **)(**(int **)(DAT_000e6180 + 0xe5f94) + 4));
      Status::setShip(pSVar24);
      pcVar30 = *(code **)(DAT_000e6184 + 0xe5fb0);
      iVar6 = (*pcVar30)(*piVar31);
      Ship::setRace(iVar6);
      iVar6 = Item::makeItem();
      Item::setUnsaleable(SUB41(iVar6,0));
      pIVar23 = (Item *)(*pcVar30)(*piVar31);
      Ship::setEquipment(pIVar23,iVar6);
      iVar6 = Item::makeItem();
      pIVar23 = (Item *)(*pcVar30)(*piVar31);
      Ship::setEquipment(pIVar23,iVar6);
      Item::setUnsaleable(SUB41(iVar6,0));
      iVar29 = *(int *)(in_r0 + 0x14);
      (*pcVar30)(*piVar31);
      iVar6 = Ship::getIndex();
      CutScene::replacePlayerShip(iVar29,iVar6);
    }
    iVar6 = Status::getCurrentCampaignMission();
    if (iVar6 < 5) {
      TouchButton::setHalfTransparent(SUB41(**(undefined4 **)(*(int *)(in_r0 + 0x8c) + 4),0));
      bVar39 = false;
LAB_000e619a:
      TouchButton::setHalfTransparent
                (SUB41(*(undefined4 *)(*(int *)(*(int *)(in_r0 + 0x8c) + 4) + 8),0));
      TouchButton::setHalfTransparent
                (SUB41(*(undefined4 *)(*(int *)(*(int *)(in_r0 + 0x8c) + 4) + 0xc),0));
    }
    else {
      if ((iVar6 < 9) || (iVar6 == 0xf)) {
        bVar39 = iVar6 == 0xf;
        goto LAB_000e619a;
      }
      bVar39 = false;
    }
    if ((iVar6 < 0xc) || (bVar39)) {
LAB_000e61d8:
      TouchButton::setHalfTransparent
                (SUB41(*(undefined4 *)(*(int *)(*(int *)(in_r0 + 0x8c) + 4) + 4),0));
    }
    else {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if (iVar6 == 100) goto LAB_000e61d8;
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if (iVar6 == 0x65) goto LAB_000e61d8;
    }
    puVar17 = *(uint **)(DAT_000e6394 + 0xe61ec);
    uVar33 = *puVar17;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraGetLocal(uVar33);
    AbyssEngine::AEMath::MatrixGetDir((Matrix *)&local_ac);
    local_a8 = local_a8 + DAT_000e6398;
    local_ac = local_ac + DAT_000e639c;
    AbyssEngine::AEMath::Vector::operator=((Vector *)(in_r0 + 0x11c),(Vector *)&local_ac);
    resetLight();
    if (*(char *)(in_r0 + 0x101) != '\0') {
      enterStation();
    }
    iVar6 = Status::getCurrentCampaignMission();
    if (iVar6 == 0x4d) {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if (iVar6 == 100) {
        Status::getStation();
        puVar18 = (uint *)Station::getShips();
        if (puVar18 != (uint *)0x0) {
          for (uVar33 = 0; uVar33 < *puVar18; uVar33 = uVar33 + 1) {
            iVar6 = Ship::getIndex();
            if (iVar6 == 0x25) {
              Ship::setPrice(*(int *)(puVar18[1] + uVar33 * 4));
            }
          }
        }
      }
    }
    pSVar5 = (Station *)Status::getStation();
    iVar6 = Station::getIndex(pSVar5);
    if (iVar6 == 0x65) {
      iVar6 = 8;
      fVar40 = extraout_s1;
    }
    else {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if (iVar6 == 100) {
        iVar6 = 7;
        fVar40 = extraout_s1_00;
      }
      else {
        Status::getSystem();
        iVar6 = SolarSystem::getRace();
        fVar40 = extraout_s1_01;
      }
    }
    local_c8 = DAT_000e6fd0;
    local_d0 = DAT_000e6fc8;
    iVar29 = DAT_000e63a0;
    uStack_dc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    local_d8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_d4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar27 = (undefined4 *)((uint)&local_f8 | 4);
    *puVar27 = 0;
    puVar27[1] = uStack_dc;
    puVar27[2] = local_d8;
    puVar27[3] = uStack_d4;
    pfVar19 = (float *)(iVar29 + 0xe62ee + iVar6 * 0xc);
    fVar41 = *pfVar19;
    local_e0 = 0;
    uVar8 = VectorSignedToFloat(pfVar19[1],(byte)(in_fpscr >> 0x16) & 3);
    uVar44 = VectorSignedToFloat(fVar41,(byte)(in_fpscr >> 0x16) & 3);
    uVar45 = VectorSignedToFloat(pfVar19[2],(byte)(in_fpscr >> 0x16) & 3);
    local_f8 = 1.0;
    local_e4 = 0x3f800000;
    local_c0 = 0x3f800000;
    *(undefined4 *)(in_r0 + 0x130) = uVar44;
    *(undefined4 *)(in_r0 + 0x134) = uVar8;
    *(undefined4 *)(in_r0 + 0x138) = uVar45;
    if (*(int *)(in_r0 + 0x13c) == 0) {
      pEVar20 = operator_new(0x10);
      uVar42 = AbyssEngine::EaseInOut::EaseInOut(pEVar20,extraout_s0_00,extraout_s1_02);
      *(EaseInOut **)(in_r0 + 0x13c) = pEVar20;
    }
    else {
      uVar42 = AbyssEngine::EaseInOut::SetRange(fVar41,fVar40);
    }
    if (*(int *)(in_r0 + 0x140) == 0) {
      pEVar20 = operator_new(0x10);
      uVar42 = AbyssEngine::EaseInOut::EaseInOut(pEVar20,extraout_s0_01,extraout_s1_03);
      *(EaseInOut **)(in_r0 + 0x140) = pEVar20;
    }
    else {
      uVar42 = AbyssEngine::EaseInOut::SetRange((float)uVar42,(float)((ulonglong)uVar42 >> 0x20));
    }
    if (*(int *)(in_r0 + 0x144) == 0) {
      pEVar20 = operator_new(0x10);
      AbyssEngine::EaseInOut::EaseInOut(pEVar20,extraout_s0_03,extraout_s1_05);
      *(EaseInOut **)(in_r0 + 0x144) = pEVar20;
      fVar40 = extraout_s0_04;
      fVar41 = extraout_s1_06;
      fVar43 = extraout_s2_00;
    }
    else {
      AbyssEngine::EaseInOut::SetRange((float)uVar42,(float)((ulonglong)uVar42 >> 0x20));
      fVar40 = extraout_s0_02;
      fVar41 = extraout_s1_04;
      fVar43 = extraout_s2;
    }
    if (*(int *)(in_r0 + 0x14) != 0) {
      AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_134,fVar40,fVar41,fVar43);
      pcVar36 = *(char **)(DAT_000e6750 + 0xe63ec);
      iVar4 = DAT_000e6758 + 0xe63fc;
      iVar25 = DAT_000e675c + 0xe63fe;
      uVar46 = *(undefined4 *)(DAT_000e6754 + 0xe63f0 + iVar6 * 4);
      iVar29 = iVar4;
      if (*pcVar36 == '\0') {
        iVar29 = iVar25;
      }
      AbyssEngine::AEMath::MatrixSetRotation
                (&local_134,&local_f8,uVar46,*(undefined4 *)(iVar29 + iVar6 * 4),DAT_000e6760,2);
      uVar33 = *puVar17;
      pMVar21 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetCurrent();
      AbyssEngine::PaintCanvas::CameraSetLocal(uVar33,pMVar21);
      local_134 = uVar44;
      local_130 = uVar8;
      local_12c = uVar45;
      AbyssEngine::AEMath::Vector::operator=
                ((Vector *)(*(int *)(in_r0 + 0x14) + 8),(Vector *)&local_134);
      if (*pcVar36 == '\0') {
        iVar4 = iVar25;
      }
      local_130 = *(undefined4 *)(iVar4 + iVar6 * 4);
      local_12c = DAT_000e6760;
      local_134 = uVar46;
      AbyssEngine::AEMath::Vector::operator=
                ((Vector *)(*(int *)(in_r0 + 0x14) + 0x14),(Vector *)&local_134);
      AEGeometry::setMatrix(*(Matrix **)(*(int *)(in_r0 + 0x14) + 0x20));
      puVar27 = *(undefined4 **)(DAT_000e6764 + 0xe648a);
      pcVar30 = *(code **)(DAT_000e6768 + 0xe648c);
      iVar6 = (*pcVar30)(*puVar27,0x14);
      *(bool *)(in_r0 + 0x12d) = iVar6 < 10;
      iVar6 = (*pcVar30)(*puVar27,0x14);
      *(bool *)(in_r0 + 0x12e) = 10 < iVar6;
      iVar6 = (*pcVar30)(*puVar27,0x14);
      cVar1 = *(char *)(in_r0 + 0x12e);
      *(bool *)(in_r0 + 0x12f) = iVar6 < 10;
      iVar6 = (*pcVar30)(*puVar27,0x96);
      if (cVar1 != '\0') {
        iVar6 = -iVar6;
      }
      fVar40 = (float)VectorSignedToFloat(iVar6,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(*(int *)(in_r0 + 0x14) + 8) = *(float *)(*(int *)(in_r0 + 0x14) + 8) + fVar40;
      cVar1 = *(char *)(in_r0 + 0x12d);
      iVar6 = (*pcVar30)(*puVar27,0x96);
      if (cVar1 != '\0') {
        iVar6 = -iVar6;
      }
      fVar40 = (float)VectorSignedToFloat(iVar6,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(*(int *)(in_r0 + 0x14) + 0xc) = *(float *)(*(int *)(in_r0 + 0x14) + 0xc) + fVar40;
      cVar1 = *(char *)(in_r0 + 0x12f);
      iVar6 = (*pcVar30)(*puVar27,0x96);
      if (cVar1 != '\0') {
        iVar6 = -iVar6;
      }
      fVar40 = (float)VectorSignedToFloat(iVar6,(byte)(in_fpscr >> 0x16) & 3);
      fVar41 = *(float *)(*(int *)(in_r0 + 0x14) + 0x10);
      fVar40 = fVar41 + fVar40;
      *(float *)(*(int *)(in_r0 + 0x14) + 0x10) = fVar40;
      AbyssEngine::AEMath::MatrixSetTranslation((Matrix *)&local_134,fVar40,extraout_s1_07,fVar41);
      if (*(int *)(in_r0 + 0x20) == 0) {
        pEVar22 = operator_new(0xf4);
        AbyssEngine::EaseInOutMatrix::EaseInOutMatrix(pEVar22);
        *(EaseInOutMatrix **)(in_r0 + 0x20) = pEVar22;
      }
      else {
        AbyssEngine::EaseInOutMatrix::SetRange(*(int *)(in_r0 + 0x20));
      }
    }
    if ((((*(char *)(in_r0 + 0x69) == '\0') && (*(char *)(in_r0 + 0x6a) == '\0')) &&
        (*(char *)(in_r0 + 99) == '\0')) &&
       (*(char *)(**(int **)(DAT_000e676c + 0xe666c) + 0x2ec) == '\0')) {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if (iVar6 != 0x65) {
        Status::getShip();
        iVar6 = Ship::hasJumpDrive();
        iVar29 = Status::getShip();
        iVar29 = Ship::hasCargo(iVar29,0x55);
        Status::getSystem();
        iVar4 = SolarSystem::getRoutes();
        if (((iVar4 == 0) &&
            (iVar4 = Status::getCurrentCampaignMission(), iVar6 == 0 && iVar29 == 0)) &&
           (0x10 < iVar4)) {
          pCVar16 = *(ChoiceWindow **)(in_r0 + 0x70);
          pSVar15 = (String *)GameText::getText(**(int **)(DAT_000e6770 + 0xe66d2));
          ChoiceWindow::set(pCVar16,pSVar15,true);
          *(undefined1 *)(in_r0 + 0x6d) = 1;
          *(undefined1 *)(in_r0 + 99) = 1;
        }
      }
    }
    uVar8 = 1;
    *(undefined1 *)(*piVar31 + 0xf8) = 1;
    *(undefined1 *)(in_r0 + 0x118) = 0;
    *(undefined4 *)(in_r0 + 0x128) = 0;
    *(undefined1 *)(in_r0 + 0xb3) = 0;
    *(undefined4 *)(in_r0 + 0x2c) = 0;
    *(undefined1 *)(in_r0 + 0x148) = 0;
  }
  else if (iVar4 == 0x28) {
    piVar31 = *(int **)(DAT_000e5a4c + 0xe585c);
    iVar6 = Status::inAlienOrbit();
    if (iVar6 == 0) {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if ((iVar6 == 0x78) &&
         ((iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x62 ||
          (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 100)))) {
        *(undefined1 *)(in_r0 + 0xd8) = 1;
      }
    }
    iVar6 = Status::inAlienOrbit();
    if (iVar6 == 0) {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if ((iVar6 == 0x3a) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x8a)) {
        *(undefined1 *)(in_r0 + 0xd8) = 1;
      }
    }
    iVar6 = Status::inAlienOrbit();
    if (iVar6 == 0) {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if ((iVar6 == 0x7e) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x78)) {
        *(undefined1 *)(in_r0 + 0xd8) = 1;
      }
    }
    iVar6 = Status::inAlienOrbit();
    if (iVar6 == 0) {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if ((iVar6 == 0x4e) && (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x8d)) {
        *(undefined1 *)(in_r0 + 0xd8) = 1;
      }
    }
    iVar6 = Status::inSupernovaSystem();
    if (iVar6 != 0) {
      *(undefined1 *)(in_r0 + 0xd8) = 1;
    }
    if (**(char **)(DAT_000e5a50 + 0xe5930) != '\0') {
      *(undefined2 *)(in_r0 + 0xd8) = 0x101;
    }
    if (*(char *)(in_r0 + 0xb0) == '\0') {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if (iVar6 == 4) {
LAB_000e604e:
        iVar6 = Status::getCurrentCampaignMission();
        if ((iVar6 != 0x30) && (iVar6 = Status::inBlackMarketSystem(), iVar6 == 0)) {
          pSVar5 = (Station *)Status::getStation();
          iVar6 = Station::getIndex(pSVar5);
          if (iVar6 != 0x6c) {
            pSVar5 = (Station *)Status::getStation();
            iVar6 = Station::getIndex(pSVar5);
            if (iVar6 != 100) {
              pSVar5 = (Station *)Status::getStation();
              iVar6 = Station::getIndex(pSVar5);
              if ((((iVar6 != 0x65) && (*(char *)(in_r0 + 0x6a) == '\0')) &&
                  (*(char *)(in_r0 + 99) == '\0')) &&
                 ((*(char *)(in_r0 + 0xd8) == '\0' && (*(char *)(*piVar31 + 0x108) == '\0')))) {
                Status::getStation();
                iVar6 = Station::hasAttackedFriends();
                if (iVar6 == 0) {
                  iVar6 = Status::getStanding();
                  Status::getSystem();
                  SolarSystem::getRace();
                  iVar6 = Standing::isEnemy(iVar6);
                  if (iVar6 == 0) goto LAB_000e6870;
                }
                AbyssEngine::String::String((String *)&local_f8);
                iVar6 = Status::getStanding();
                Status::getSystem();
                SolarSystem::getRace();
                iVar6 = Standing::isEnemy(iVar6);
                if (iVar6 == 0) {
                  iVar6 = Status::getLevel();
                  *(int *)(in_r0 + 0xd4) = iVar6 * 0x96 + 1000;
                  iVar6 = Status::hardCoreMode();
                  if (iVar6 != 0) {
                    *(int *)(in_r0 + 0xd4) = *(int *)(in_r0 + 0xd4) * 10;
                  }
                  pSVar15 = (String *)GameText::getText(**(int **)(DAT_000e6774 + 0xe673e));
                  AbyssEngine::String::operator=((String *)&local_f8,pSVar15);
                }
                else {
                  iVar6 = Status::getStanding();
                  Status::getSystem();
                  SolarSystem::getRace();
                  iVar29 = Standing::getStanding(iVar6);
                  if (iVar29 < 1) {
                    iVar6 = Standing::getStanding(iVar6);
                    iVar6 = -iVar6;
                  }
                  else {
                    iVar6 = Standing::getStanding(iVar6);
                  }
                  fVar40 = (float)VectorSignedToFloat(iVar6,(byte)(in_fpscr >> 0x16) & 3);
                  piVar35 = *(int **)(DAT_000e6b1c + 0xe67ae);
                  *(int *)(in_r0 + 0xd4) = (int)((fVar40 / DAT_000e6778) * DAT_000e677c);
                  iVar6 = AbyssEngine::AERandom::nextInt(*piVar35);
                  *(int *)(in_r0 + 0xd4) = iVar6 + *(int *)(in_r0 + 0xd4) + -100;
                  iVar6 = Status::hardCoreMode();
                  if (iVar6 != 0) {
                    *(int *)(in_r0 + 0xd4) = *(int *)(in_r0 + 0xd4) * 10;
                  }
                  pSVar15 = (String *)GameText::getText(**(int **)(DAT_000e6b20 + 0xe67e6));
                  AbyssEngine::String::operator=((String *)&local_f8,pSVar15);
                }
                iVar6 = *piVar31;
                AbyssEngine::String::String(aSStack_64,(String *)&local_f8,false);
                Layout::formatCredits((int)auStack_70);
                uVar8 = AbyssEngine::String::String
                                  (aSStack_7c,(char *)(DAT_000e6b28 + 0xe681c),false);
                Status::replaceHash(&local_134,iVar6,aSStack_64,auStack_70,uVar8);
                AbyssEngine::String::operator=((String *)&local_f8,(String *)&local_134);
                pcVar30 = *(code **)(DAT_000e6b2c + 0xe683e);
                (*pcVar30)(&local_134);
                (*pcVar30)(aSStack_7c);
                (*pcVar30)(auStack_70);
                (*pcVar30)(aSStack_64);
                ChoiceWindow::set(*(ChoiceWindow **)(in_r0 + 0x70),(String *)&local_f8,true);
                *(undefined1 *)(in_r0 + 99) = 1;
                *(undefined1 *)(in_r0 + 0x6b) = 1;
                *(undefined1 *)(in_r0 + 0x101) = 0;
                AbyssEngine::String::~String((String *)&local_f8);
              }
            }
          }
        }
      }
      else {
        pSVar5 = (Station *)Status::getStation();
        iVar6 = Station::getIndex(pSVar5);
        if (iVar6 == 0x58) goto LAB_000e604e;
        Status::getStation();
        iVar6 = Station::stationHasPirateBase();
        if (iVar6 == 0) goto LAB_000e604e;
        if (*(int *)(in_r0 + 0x84) == 0) {
          pDVar9 = operator_new(0x74);
          piVar35 = *(int **)(DAT_000e5a54 + 0xe5996);
          pSVar15 = (String *)GameText::getText(*piVar35);
          pSVar10 = (String *)GameText::getText(*piVar35);
          DialogueWindow::DialogueWindow(pDVar9,pSVar15,pSVar10,(int *)(DAT_000e5a58 + 0xe59b4));
          *(DialogueWindow **)(in_r0 + 0x84) = pDVar9;
          this_00 = operator_new(0x98);
          AbyssEngine::String::String(aSStack_58,(char *)(DAT_000e5a5c + 0xe59cc),false);
          Agent::Agent(this_00,0,aSStack_58,0,0,2,1,0,0,0,0);
          AbyssEngine::String::~String(aSStack_58);
          iVar6 = **(int **)(DAT_000e5a60 + 0xe5a04);
          pVVar11 = (Vector *)
                    Globals::getDialogueSoundId
                              ((Globals *)**(undefined4 **)(DAT_000e5a64 + 0xe5a06),0x1b2,this_00);
          FModSound::play(iVar6,pVVar11,(Vector *)0x0,extraout_s0);
          pvVar12 = (void *)Agent::~Agent(this_00);
          operator_delete(pvVar12);
          *(undefined1 *)(in_r0 + 0x101) = 0;
          *(undefined1 *)(in_r0 + 0x69) = 1;
        }
      }
LAB_000e6870:
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      Status::getCampaignMission();
      iVar29 = Status::getCurrentCampaignMission();
      if ((iVar29 == 0x14) && (iVar29 = Mission::getTargetStation(), iVar29 == iVar6)) {
        Status::getStation();
        puVar17 = (uint *)Station::getItems();
        if (puVar17 != (uint *)0x0) {
          for (uVar33 = 0; uVar33 < *puVar17; uVar33 = uVar33 + 1) {
            iVar29 = Item::getIndex(*(Item **)(puVar17[1] + uVar33 * 4));
            if (iVar29 == 0x29) {
              Item::setPrice(*(int *)(puVar17[1] + uVar33 * 4));
            }
          }
        }
        pIVar23 = (Item *)Status::getStation();
        Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000e6b30 + 0xe68e8) + 4) + 0xa4),10);
        Station::addItem(pIVar23);
      }
      iVar29 = Status::getCurrentCampaignMission();
      if ((iVar29 == 0x1b) && (iVar29 = Mission::getTargetStation(), iVar29 == iVar6)) {
        iVar29 = Status::getShip();
        Ship::removeCargo(iVar29);
      }
      iVar29 = Status::getCurrentCampaignMission();
      if (iVar29 == 0x2b) {
        if (iVar6 == 10) {
LAB_000e6946:
          *(undefined1 *)(in_r0 + 0xd9) = 1;
        }
      }
      else {
        iVar29 = Status::getCurrentCampaignMission();
        if ((iVar6 == 10) && (iVar29 == 0x2c)) goto LAB_000e6946;
      }
      iVar29 = Status::gameWon();
      if ((iVar6 == 10) && (iVar29 != 0)) {
        iVar6 = Status::getShip();
        iVar6 = Ship::hasEquipment(iVar6,0x55);
        if (iVar6 == 0) {
          iVar6 = Status::getShip();
          iVar6 = Ship::hasCargo(iVar6,0x55);
          if (iVar6 == 0) {
            iVar6 = Status::getStation();
            iVar6 = Station::hasItem(iVar6);
            if (iVar6 == 0) {
              iVar6 = Status::getStation();
              iVar6 = Station::hasItem(iVar6);
              if (iVar6 == 0) {
                pIVar23 = (Item *)Status::getStation();
                Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000e6b34 + 0xe69b0) + 4) + 0x290));
                Station::addItem(pIVar23);
              }
            }
          }
        }
        bVar39 = false;
LAB_000e69dc:
        iVar6 = Status::getShip();
        iVar6 = Ship::getCargo(iVar6);
        if (iVar6 == 0) {
          iVar6 = 0;
        }
        else {
          iVar6 = Item::getAmount();
        }
        iVar29 = Status::getStation();
        iVar29 = Station::hasItem(iVar29);
        if ((iVar6 < 6) && (iVar29 == 0)) {
          pIVar23 = (Item *)Status::getStation();
          Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000e6b38 + 0xe6a1c) + 4) + 0x1e8));
          Station::addItem(pIVar23);
        }
      }
      else {
        bVar39 = iVar6 == 100;
        if ((iVar6 - 100U < 2) || (iVar6 == 10)) goto LAB_000e69dc;
      }
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if ((iVar6 == 10) && (iVar6 = Achievements::gotAllGoldMedals(), iVar6 != 0)) {
        Status::getStation();
        puVar17 = (uint *)Station::getShips();
        if (puVar17 == (uint *)0x0) {
LAB_000e6a78:
          pGVar7 = operator_new(1);
          Generator::Generator(pGVar7);
          pAVar14 = (Array *)Status::getStation();
          Status::getStation();
          bVar3 = (bool)Generator::getShipBuyList((Station *)pGVar7);
          Station::setShips(pAVar14,bVar3);
          pvVar12 = (void *)Generator::~Generator(pGVar7);
          operator_delete(pvVar12);
        }
        else {
          uVar33 = 0;
          do {
            if (*puVar17 <= uVar33) goto LAB_000e6a78;
            iVar6 = Ship::getIndex();
            uVar33 = uVar33 + 1;
          } while (iVar6 != 8);
        }
      }
      if ((bVar39) &&
         ((((iVar6 = Status::dlc1Won(), iVar6 != 0 ||
            (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x50)) ||
           (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x51)) ||
          (((iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x52 ||
            (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x53)) ||
           (iVar6 = Status::getCurrentCampaignMission(), iVar6 == 0x54)))))) {
        pSVar24 = (Ship *)Status::getStation();
        iVar6 = Station::hasShip((int)pSVar24);
        if (iVar6 != 0) {
          puVar17 = (uint *)Station::getShips();
          uVar33 = 0;
          while ((uVar33 < *puVar17 && (iVar6 = Ship::getIndex(), iVar6 != 0x25))) {
            uVar33 = uVar33 + 1;
          }
          Station::removeShip(pSVar24);
        }
        iVar6 = Station::hasShip((int)pSVar24);
        if (iVar6 == 0) {
          Ship::makeShip(*(int *)(*(int *)(**(int **)(DAT_000e6f1c + 0xe6b7a) + 4) + 0x98));
          Station::addShip(pSVar24);
        }
        iVar6 = Station::hasShip((int)pSVar24);
        if (iVar6 == 0) {
          Ship::makeShip(*(int *)(*(int *)(**(int **)(DAT_000e6f20 + 0xe6ba2) + 4) + 0xa0));
          Station::addShip(pSVar24);
        }
      }
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      bVar39 = iVar6 == 0x6c;
      if (bVar39) {
        iVar6 = *(int *)(*piVar31 + 0x114);
      }
      if (bVar39 && iVar6 == 1) {
        if (*(DialogueWindow **)(in_r0 + 0x84) != (DialogueWindow *)0x0) {
          pvVar12 = (void *)DialogueWindow::~DialogueWindow(*(DialogueWindow **)(in_r0 + 0x84));
          operator_delete(pvVar12);
        }
        *(undefined4 *)(in_r0 + 0x84) = 0;
        this_02 = operator_new(0x78);
        pSVar15 = (String *)GameText::getText(**(int **)(DAT_000e6fc4 + 0xe6c54));
        AbyssEngine::String::String(aSStack_88,pSVar15,false);
        Mission::Mission(this_02,0xffffffff,aSStack_88,DAT_000e6fd8 + 0xe6c84,4,0,0x6c,0);
        AbyssEngine::String::~String(aSStack_88);
        Mission::setWon(this_02,true);
        *(undefined4 *)(*piVar31 + 0x114) = 2;
        pDVar9 = operator_new(0x74);
        DialogueWindow::DialogueWindow(pDVar9,this_02,(Level *)0x0,0);
        *(undefined1 *)(in_r0 + 0x69) = 1;
        *(DialogueWindow **)(in_r0 + 0x84) = pDVar9;
      }
      else {
        pSVar5 = (Station *)Status::getStation();
        iVar6 = Station::getIndex(pSVar5);
        if ((iVar6 == 0x6c) && (*(int *)(*piVar31 + 0x114) == 2)) {
          iVar6 = Status::getCredits();
          if (iVar6 < DAT_000e6f24) {
LAB_000e6cc0:
            pSVar15 = *(String **)(in_r0 + 0x70);
            GameText::getText(**(int **)(DAT_000e6fdc + 0xe6cce));
            ChoiceWindow::set(pSVar15);
          }
          else {
            iVar6 = Status::getShip();
            iVar6 = Ship::hasCargo(iVar6,0x6d);
            if (iVar6 == 0) goto LAB_000e6cc0;
            pCVar16 = *(ChoiceWindow **)(in_r0 + 0x70);
            pSVar15 = (String *)GameText::getText(**(int **)(DAT_000e6f28 + 0xe6c1a));
            ChoiceWindow::set(pCVar16,pSVar15,true);
            *(undefined1 *)(in_r0 + 0x6c) = 1;
          }
          *(undefined1 *)(in_r0 + 99) = 1;
        }
      }
      iVar6 = Status::getFreelanceMission();
      if ((iVar6 != 0) && (iVar6 = Mission::getType(), iVar6 == 0xe)) {
        iVar6 = Mission::getTargetStation();
        pSVar5 = (Station *)Status::getStation();
        iVar29 = Station::getIndex(pSVar5);
        if (iVar6 == iVar29) {
          iVar6 = Status::getStation();
          iVar6 = Station::hasItem(iVar6);
          if (iVar6 == 0) {
            Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000e6fe0 + 0xe6d28) + 4) + 0x1cc));
            pIVar23 = (Item *)Status::getStation();
            Station::addItem(pIVar23);
          }
        }
      }
      uVar33 = Status::getCurrentCampaignMission();
      bVar39 = uVar33 == 0xa2;
      iVar6 = *(int *)(DAT_000e6fe4 + 0xe6d54);
      if (bVar39) {
        uVar33 = (uint)*(byte *)(iVar6 + 0x2b);
      }
      if (bVar39 && uVar33 == 0) {
        pSVar15 = *(String **)(in_r0 + 0x70);
        GameText::getText(**(int **)(DAT_000e6fe8 + 0xe6d6c));
        ChoiceWindow::set(pSVar15);
        *(undefined1 *)(in_r0 + 99) = 1;
        *(undefined1 *)(iVar6 + 0x2b) = 1;
      }
      iVar29 = Status::gameWon();
      iVar4 = *(int *)(DAT_000e6fec + 0xe6d94);
      if ((iVar29 != 0) && (*(char *)(iVar4 + 0x35) == '\0' && *(char *)(iVar6 + 0x38) == '\0')) {
        pSVar15 = *(String **)(in_r0 + 0x70);
        GameText::getText(**(int **)(DAT_000e6ff0 + 0xe6db0));
        ChoiceWindow::set(pSVar15);
        *(undefined1 *)(in_r0 + 99) = 1;
        *(undefined1 *)(iVar6 + 0x38) = 1;
      }
      iVar29 = Status::dlc1Won();
      if ((iVar29 != 0) && (*(char *)(iVar4 + 0x37) == '\0' && *(char *)(iVar6 + 0x39) == '\0')) {
        pSVar15 = *(String **)(in_r0 + 0x70);
        GameText::getText(**(int **)(DAT_000e6ff4 + 0xe6dec));
        ChoiceWindow::set(pSVar15);
        *(undefined1 *)(in_r0 + 99) = 1;
        *(undefined1 *)(iVar6 + 0x39) = 1;
      }
      iVar6 = Status::activateNewWanted();
      if ((0 < iVar6) && (*(char *)(in_r0 + 99) == '\0')) {
        if (iVar6 == 1) {
          pSVar15 = *(String **)(in_r0 + 0x70);
          GameText::getText(**(int **)(DAT_000e6ff8 + 0xe6e28));
          ChoiceWindow::set(pSVar15);
        }
        else {
          pSVar15 = (String *)GameText::getText(**(int **)(DAT_000e6ff8 + 0xe6e28));
          AbyssEngine::String::String((String *)&local_f8,pSVar15,false);
          iVar29 = *piVar31;
          AbyssEngine::String::String(aSStack_94,(String *)&local_f8,false);
          AbyssEngine::String::String((String *)&local_ac,iVar6);
          AbyssEngine::String::String(aSStack_a0,(String *)&local_ac,false);
          uVar8 = AbyssEngine::String::String(aSStack_b8,(char *)(DAT_000e6ffc + 0xe6e80),false);
          Status::replaceHash(&local_134,iVar29,aSStack_94,aSStack_a0,uVar8);
          AbyssEngine::String::operator=((String *)&local_f8,(String *)&local_134);
          pcVar30 = *(code **)(DAT_000e7000 + 0xe6ea2);
          (*pcVar30)(&local_134);
          (*pcVar30)(aSStack_b8);
          (*pcVar30)(aSStack_a0);
          (*pcVar30)(&local_ac);
          (*pcVar30)(aSStack_94);
          ChoiceWindow::set(*(String **)(in_r0 + 0x70));
          AbyssEngine::String::~String((String *)&local_f8);
        }
        *(char *)(in_r0 + 99) = '\x01';
      }
    }
    uVar8 = 0x14;
  }
  else if (iVar4 == 0x3c) {
    *(undefined1 *)(in_r0 + 0x100) = 0;
    *(undefined4 *)(in_r0 + 0xe8) = 0;
    *(undefined4 *)(in_r0 + 0xe0) = 0;
    *(undefined4 *)(in_r0 + 0xf0) = 0;
    *(undefined4 *)(in_r0 + 0xf4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    *(undefined4 *)(in_r0 + 0xf8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    *(undefined4 *)(in_r0 + 0xfc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    Status::getSystem();
    iVar6 = SolarSystem::getRace();
    uVar8 = 0x10e;
    if (iVar6 == 2) {
      uVar8 = 0xffffff38;
    }
    *(undefined4 *)(in_r0 + 0xe4) = uVar8;
    if (*(int *)(in_r0 + 0x14) != 0) {
      fVar40 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(*(int *)(in_r0 + 0x14) + 4) = fVar40 / DAT_000e5e5c;
    }
    *(undefined4 *)(in_r0 + 0xec) = 0;
    *(undefined1 *)(in_r0 + 0xb0) = *(undefined1 *)(in_r0 + 0xb3);
    this_01 = operator_new(0x30);
    puVar17 = *(uint **)(DAT_000e5e68 + 0xe5d90);
    iVar25 = **(int **)(DAT_000e5e64 + 0xe5d8c);
    iVar38 = *(int *)(**(int **)(DAT_000e5e60 + 0xe5d86) + 0x10);
    iVar26 = *(int *)(in_r0 + 0xa8);
    iVar6 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar17);
    iVar32 = *(int *)(in_r0 + 0xa8);
    iVar28 = **(int **)(DAT_000e5e70 + 0xe5dbc);
    Status::getSystem();
    iVar29 = SolarSystem::getRace();
    iVar4 = Status::getCurrentCampaignMission();
    NewsTicker::NewsTicker(this_01,iVar26,(iVar25 - iVar38) - iVar6,iVar28 - iVar32,iVar29,iVar4);
    *(undefined4 *)(in_r0 + 0x30) = 0;
    *(undefined4 *)(in_r0 + 0x34) = 0;
    *(NewsTicker **)(in_r0 + 0x1c) = this_01;
    *(undefined2 *)(in_r0 + 0xb1) = 0;
    *(undefined4 *)(in_r0 + 0xd0) = 0;
    *(undefined4 *)(in_r0 + 0xd4) = 0;
    *(undefined4 *)(in_r0 + 0xd8) = 0;
    pCVar16 = operator_new(0x5c);
    ChoiceWindow::ChoiceWindow(pCVar16);
    *(ChoiceWindow **)(in_r0 + 0x70) = pCVar16;
    this_03 = (PaintCanvas *)*puVar17;
    Status::getSystem();
    iVar6 = SolarSystem::getRace();
    AbyssEngine::PaintCanvas::Image2DCreate
              (this_03,*(ushort *)(DAT_000e5e74 + 0xe5e2e + iVar6 * 4),(uint *)(in_r0 + 0x4c));
    uVar8 = 0x28;
  }
  else if (iVar4 == 0x50) {
    *(undefined4 *)(in_r0 + 0x62) = 0;
    *(undefined1 *)(in_r0 + 0x6e) = 0;
    *(undefined2 *)(in_r0 + 0x6c) = 0;
    piVar31 = *(int **)(iVar29 + 0xe5a7c);
    *(undefined4 *)(in_r0 + 0x68) = 0;
    *(undefined2 *)(in_r0 + 0x66) = 0;
    *(undefined2 *)(in_r0 + 0x60) = 0x100;
    *(undefined1 *)(in_r0 + 0xdc) = 0;
    *(undefined2 *)(in_r0 + 0xde) = 0;
    *(undefined1 *)(in_r0 + 300) = 0;
    *(undefined1 *)(in_r0 + 0x18) = 0;
    *(undefined1 *)(in_r0 + 99) = 0;
    iVar4 = *piVar31;
    piVar37 = *(int **)(iVar6 + 0xe5a7a);
    iVar26 = *(int *)(iVar4 + 0xcc);
    *(int *)(in_r0 + 0xa8) = iVar26;
    iVar29 = *piVar37;
    iVar25 = *(int *)(iVar4 + 0xc);
    iVar28 = *(int *)(iVar4 + 0x10);
    iVar32 = *(int *)(iVar4 + 0x28);
    iVar4 = *(int *)(iVar4 + 0x4c);
    *(int *)(in_r0 + 0xa4) = iVar25;
    iVar6 = DAT_000e5e48;
    *(undefined4 *)(in_r0 + 0xa0) = 0;
    *(int *)(in_r0 + 0xac) = (iVar29 - iVar25) - iVar28;
    pTVar13 = operator_new(200);
    AbyssEngine::String::String((String *)&local_f8,(char *)(iVar6 + 0xe5ae0),false);
    piVar35 = *(int **)(DAT_000e5e4c + 0xe5af4);
    TouchButton::TouchButton(pTVar13,(String *)&local_f8,0xc,*piVar35,*piVar37,'\"');
    *(TouchButton **)(in_r0 + 0x90) = pTVar13;
    AbyssEngine::String::~String((String *)&local_f8);
    pTVar13 = operator_new(200);
    Status::getCredits();
    Layout::formatCredits((int)&local_f8);
    iVar29 = *piVar35;
    iVar6 = TouchButton::getWidth();
    TouchButton::TouchButton(pTVar13,(String *)&local_f8,0xb,iVar29 - iVar6,*piVar37,'\"');
    *(TouchButton **)(in_r0 + 0x94) = pTVar13;
    AbyssEngine::String::~String((String *)&local_f8);
    pAVar14 = operator_new(0xc);
    Array<TouchButton*>::Array();
    *(Array **)(in_r0 + 0x8c) = pAVar14;
    ArraySetLength<TouchButton*>(5,pAVar14);
    iVar6 = *piVar31;
    iVar28 = *piVar37;
    iVar29 = *(int *)(iVar6 + 0x10);
    iVar25 = *(int *)(iVar6 + 0x24);
    iVar38 = *(int *)(iVar6 + 0x30);
    iVar6 = *(int *)(iVar6 + 0x34);
    piVar35 = *(int **)(DAT_000e5e54 + 0xe5b94);
    pTVar13 = operator_new(200);
    pSVar15 = (String *)GameText::getText(*piVar35);
    iVar4 = (iVar26 - iVar32) - iVar4;
    iVar29 = ((iVar28 - iVar29) - iVar25) + iVar38 * -5 + iVar6 * -4;
    TouchButton::TouchButton(pTVar13,pSVar15,0,*(int *)(*piVar31 + 0x28),iVar29,iVar4,'\x11','\x01')
    ;
    **(undefined4 **)(*(int *)(in_r0 + 0x8c) + 4) = pTVar13;
    pTVar13 = operator_new(200);
    pSVar15 = (String *)GameText::getText(*piVar35);
    iVar6 = *piVar31;
    TouchButton::TouchButton
              (pTVar13,pSVar15,0,*(int *)(iVar6 + 0x28),
               *(int *)(iVar6 + 0x34) + *(int *)(iVar6 + 0x30) + iVar29,iVar4,'\x11','\x01');
    *(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x8c) + 4) + 4) = pTVar13;
    pTVar13 = operator_new(200);
    pSVar15 = (String *)GameText::getText(*piVar35);
    iVar6 = *piVar31;
    TouchButton::TouchButton
              (pTVar13,pSVar15,0,*(int *)(iVar6 + 0x28),
               iVar29 + (*(int *)(iVar6 + 0x34) + *(int *)(iVar6 + 0x30)) * 2,iVar4,'\x11','\x01');
    *(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x8c) + 4) + 8) = pTVar13;
    pTVar13 = operator_new(200);
    pSVar15 = (String *)GameText::getText(*piVar35);
    iVar6 = *piVar31;
    TouchButton::TouchButton
              (pTVar13,pSVar15,0,*(int *)(iVar6 + 0x28),
               (*(int *)(iVar6 + 0x34) + *(int *)(iVar6 + 0x30)) * 3 + iVar29,iVar4,'\x11','\x01');
    *(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x8c) + 4) + 0xc) = pTVar13;
    pTVar13 = operator_new(200);
    pSVar15 = (String *)GameText::getText(*piVar35);
    iVar6 = *piVar31;
    TouchButton::TouchButton
              (pTVar13,pSVar15,0,*(int *)(iVar6 + 0x28),
               iVar29 + (*(int *)(iVar6 + 0x34) + *(int *)(iVar6 + 0x30)) * 4,iVar4,'\x11','\x01');
    *(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x8c) + 4) + 0x10) = pTVar13;
    uVar8 = 0x3c;
  }
  else {
    if (iVar4 != 100) {
      piVar31 = *(int **)(DAT_000e6160 + 0xe5e84);
      FModSound::play(*piVar31,(Vector *)0x7a,(Vector *)0x0,in_s0);
      FModSound::enableReverb(*piVar31);
      FModSound::setDownPitch(SUB41(*piVar31,0));
      piVar31 = *(int **)(DAT_000e6164 + 0xe5ea4);
      if (*piVar31 != -1) {
        Globals::playMusicAndFadeOutCurrent(**(int **)(DAT_000e6168 + 0xe5eb0));
      }
      *piVar31 = -1;
      *(undefined1 *)(in_r0 + 0x24) = 1;
      *(undefined4 *)(in_r0 + 0xc) = 100;
      puVar17 = *(uint **)(in_r0 + 0x8c);
      if (puVar17 != (uint *)0x0) {
        iVar6 = *(int *)(DAT_000e616c + 0xe5eda);
        iVar29 = *(int *)(DAT_000e6170 + 0xe5ee0);
        for (uVar33 = 0; uVar33 < *puVar17; uVar33 = uVar33 + 1) {
          if (uVar33 < 10) {
            TouchButton::getPosition();
            *(int *)(iVar6 + uVar33 * 4) = (int)local_f8;
            TouchButton::getPosition();
            *(int *)(iVar29 + uVar33 * 4) = (int)local_f4;
            puVar17 = *(uint **)(in_r0 + 0x8c);
          }
        }
        **(uint **)(DAT_000e6174 + 0xe5f34) = *puVar17;
      }
      uVar8 = 0;
      goto LAB_000e6ed0;
    }
    puVar27 = *(undefined4 **)(DAT_000e5a40 + 0xe5770);
    Globals::startNewSoundResourceList();
    pcVar30 = *(code **)(DAT_000e5a44 + 0xe577e);
    (*pcVar30)(*puVar27,0x5f);
    (*pcVar30)(*puVar27,0x7a);
    (*pcVar30)(*puVar27,0x6c);
    (*pcVar30)(*puVar27,0x60);
    (*pcVar30)(*puVar27,0x61);
    (*pcVar30)(*puVar27,0x62);
    (*pcVar30)(*puVar27,99);
    (*pcVar30)(*puVar27,0x65);
    (*pcVar30)(*puVar27,100);
    (*pcVar30)(*puVar27,0x66);
    (*pcVar30)(*puVar27,0x68);
    (*pcVar30)(*puVar27,0x69);
    (*pcVar30)(*puVar27,0x6a);
    (*pcVar30)(*puVar27,0x6b);
    (*pcVar30)(*puVar27,0x67);
    (*pcVar30)(*puVar27,0x7e);
    iVar6 = DAT_000e5a48;
    *(undefined4 *)(in_r0 + 0xcc) = 0x32;
    piVar31 = *(int **)(iVar6 + 0xe57e8);
    *(undefined1 *)(in_r0 + 0x101) = 1;
    Status::getStation();
    Station::getName();
    AbyssEngine::String::operator=((String *)(in_r0 + 0x38),(String *)&local_f8);
    AbyssEngine::String::~String((String *)&local_f8);
    lVar2 = *(longlong *)(*piVar31 + 0x70);
    lVar47 = Status::getPlayingTime();
    lVar47 = lVar47 - lVar2;
    if ((int)(uint)((uint)lVar47 < 0x7531) <= (int)((ulonglong)lVar47 >> 0x20)) {
      pSVar5 = (Station *)Status::getStation();
      iVar6 = Station::getIndex(pSVar5);
      if (iVar6 != 0x6c) {
        pGVar7 = operator_new(1);
        Generator::Generator(pGVar7);
        Status::getStation();
        Generator::computerTradeGoods((Station *)pGVar7);
      }
    }
    uVar8 = 0x50;
  }
  *(undefined4 *)(in_r0 + 0xc) = uVar8;
LAB_000e6ed0:
  if (*piVar34 != local_4c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar8);
  }
  return;
}

```

## target disasm
```
  000e5708: push {r4,r5,r6,r7,lr}
  000e570a: add r7,sp,#0xc
  000e570c: push {r7,r8,r9,r10,r11}
  000e5710: vpush {d8,d9,d10,d11}
  000e5714: sub sp,#0x178
  000e5716: mov r8,r0
  000e5718: ldr r0,[0x000e5a3c]
  000e571a: movs r1,#0x0
  000e571c: add r0,pc
  000e571e: ldr.w r10,[r0,#0x0]
  000e5722: ldr.w r0,[r10,#0x0]
  000e5726: str r0,[sp,#0x174]
  000e5728: ldr.w r0,[r8,#0x14]
  000e572c: str.w r1,[r8,#0x9c]
  000e5730: cbnz r0,0x000e574a
  000e5732: movs r0,#0xa0
  000e5734: blx 0x0006eb24
  000e5738: mov r5,r0
  000e573a: movs r1,#0x17
  000e573c: blx 0x00074ef0
  000e5740: mov r0,r5
  000e5742: str.w r5,[r8,#0x14]
  000e5746: blx 0x00074efc
  000e574a: ldr.w r0,[r8,#0xc]
  000e574e: cmp r0,#0x14
  000e5750: beq.w 0x000e5f3c
  000e5754: cmp r0,#0x28
  000e5756: beq 0x000e5856
  000e5758: cmp r0,#0x3c
  000e575a: beq.w 0x000e5d0c
  000e575e: cmp r0,#0x50
  000e5760: beq.w 0x000e5a68
  000e5764: cmp r0,#0x64
  000e5766: bne.w 0x000e5e78
  000e576a: ldr r0,[0x000e5a40]
  000e576c: add r0,pc
  000e576e: ldr r5,[r0,#0x0]
  000e5770: ldr r0,[r5,#0x0]
  000e5772: blx 0x00074f08
  000e5776: ldr r0,[0x000e5a44]
  000e5778: movs r1,#0x5f
  000e577a: add r0,pc
  000e577c: ldr r6,[r0,#0x0]
  000e577e: ldr r0,[r5,#0x0]
  000e5780: blx r6
  000e5782: ldr r0,[r5,#0x0]
  000e5784: movs r1,#0x7a
  000e5786: blx r6
  000e5788: ldr r0,[r5,#0x0]
  000e578a: movs r1,#0x6c
  000e578c: blx r6
  000e578e: ldr r0,[r5,#0x0]
  000e5790: movs r1,#0x60
  000e5792: blx r6
  000e5794: ldr r0,[r5,#0x0]
  000e5796: movs r1,#0x61
  000e5798: blx r6
  000e579a: ldr r0,[r5,#0x0]
  000e579c: movs r1,#0x62
  000e579e: blx r6
  000e57a0: ldr r0,[r5,#0x0]
  000e57a2: movs r1,#0x63
  000e57a4: blx r6
  000e57a6: ldr r0,[r5,#0x0]
  000e57a8: movs r1,#0x65
  000e57aa: blx r6
  000e57ac: ldr r0,[r5,#0x0]
  000e57ae: movs r1,#0x64
  000e57b0: blx r6
  000e57b2: ldr r0,[r5,#0x0]
  000e57b4: movs r1,#0x66
  000e57b6: blx r6
  000e57b8: ldr r0,[r5,#0x0]
  000e57ba: movs r1,#0x68
  000e57bc: blx r6
  000e57be: ldr r0,[r5,#0x0]
  000e57c0: movs r1,#0x69
  000e57c2: blx r6
  000e57c4: ldr r0,[r5,#0x0]
  000e57c6: movs r1,#0x6a
  000e57c8: blx r6
  000e57ca: ldr r0,[r5,#0x0]
  000e57cc: movs r1,#0x6b
  000e57ce: blx r6
  000e57d0: ldr r0,[r5,#0x0]
  000e57d2: movs r1,#0x67
  000e57d4: blx r6
  000e57d6: ldr r0,[r5,#0x0]
  000e57d8: movs r1,#0x7e
  000e57da: blx r6
  000e57dc: ldr r0,[0x000e5a48]
  000e57de: movs r1,#0x32
  000e57e0: str.w r1,[r8,#0xcc]
  000e57e4: add r0,pc
  000e57e6: ldr r6,[r0,#0x0]
  000e57e8: movs r0,#0x1
  000e57ea: strb.w r0,[r8,#0x101]
  000e57ee: ldr r0,[r6,#0x0]
  000e57f0: blx 0x00071c14
  000e57f4: add r5,sp,#0xc8
  000e57f6: mov r1,r0
  000e57f8: mov r0,r5
  000e57fa: blx 0x000736a8
  000e57fe: add.w r0,r8,#0x38
  000e5802: mov r1,r5
  000e5804: blx 0x0006f2b0
  000e5808: add r0,sp,#0xc8
  000e580a: blx 0x0006ee30
  000e580e: ldr r0,[r6,#0x0]
  000e5810: ldrd r4,r5,[r0,#0x70]
  000e5814: blx 0x00071710
  000e5818: subs r0,r0,r4
  000e581a: sbcs r1,r5
  000e581c: movw r2,#0x7531
  000e5820: subs r0,r0,r2
  000e5822: sbcs r0,r1,#0x0
  000e5826: blt 0x000e5850
  000e5828: ldr r0,[r6,#0x0]
  000e582a: blx 0x00071c14
  000e582e: blx 0x00071824
  000e5832: cmp r0,#0x6c
  000e5834: beq 0x000e5850
  000e5836: movs r0,#0x1
  000e5838: blx 0x0006eb24
  000e583c: mov r5,r0
  000e583e: blx 0x000736cc
  000e5842: ldr r0,[r6,#0x0]
  000e5844: blx 0x00071c14
  000e5848: mov r1,r0
  000e584a: mov r0,r5
  000e584c: blx 0x00074f14
  000e5850: movs r0,#0x50
  000e5852: b.w 0x000e6ecc
  000e5856: ldr r0,[0x000e5a4c]
  000e5858: add r0,pc
  000e585a: ldr.w r9,[r0,#0x0]
  000e585e: ldr.w r0,[r9,#0x0]
  000e5862: blx 0x000723d0
  000e5866: cbnz r0,0x000e5896
  000e5868: ldr.w r0,[r9,#0x0]
  000e586c: blx 0x00071c14
  000e5870: blx 0x00071824
  000e5874: cmp r0,#0x78
  000e5876: bne 0x000e5896
  000e5878: ldr.w r0,[r9,#0x0]
  000e587c: blx 0x00071770
  000e5880: cmp r0,#0x62
  000e5882: beq 0x000e5890
  000e5884: ldr.w r0,[r9,#0x0]
  000e5888: blx 0x00071770
  000e588c: cmp r0,#0x64
  000e588e: bne 0x000e5896
  000e5890: movs r0,#0x1
  000e5892: strb.w r0,[r8,#0xd8]
  000e5896: ldr.w r0,[r9,#0x0]
  000e589a: blx 0x000723d0
  000e589e: cbnz r0,0x000e58c2
  000e58a0: ldr.w r0,[r9,#0x0]
  000e58a4: blx 0x00071c14
  000e58a8: blx 0x00071824
  000e58ac: cmp r0,#0x3a
  000e58ae: bne 0x000e58c2
  000e58b0: ldr.w r0,[r9,#0x0]
  000e58b4: blx 0x00071770
  000e58b8: cmp r0,#0x8a
  000e58ba: itt eq
  000e58bc: mov.eq r0,#0x1
  000e58be: strb.eq.w r0,[r8,#0xd8]
  000e58c2: ldr.w r0,[r9,#0x0]
  000e58c6: blx 0x000723d0
  000e58ca: cbnz r0,0x000e58ee
  000e58cc: ldr.w r0,[r9,#0x0]
  000e58d0: blx 0x00071c14
  000e58d4: blx 0x00071824
  000e58d8: cmp r0,#0x7e
  000e58da: bne 0x000e58ee
  000e58dc: ldr.w r0,[r9,#0x0]
  000e58e0: blx 0x00071770
  000e58e4: cmp r0,#0x78
  000e58e6: itt eq
  000e58e8: mov.eq r0,#0x1
  000e58ea: strb.eq.w r0,[r8,#0xd8]
  000e58ee: ldr.w r0,[r9,#0x0]
  000e58f2: blx 0x000723d0
  000e58f6: cbnz r0,0x000e591a
  000e58f8: ldr.w r0,[r9,#0x0]
  000e58fc: blx 0x00071c14
  000e5900: blx 0x00071824
  000e5904: cmp r0,#0x4e
  000e5906: bne 0x000e591a
  000e5908: ldr.w r0,[r9,#0x0]
  000e590c: blx 0x00071770
  000e5910: cmp r0,#0x8d
  000e5912: itt eq
  000e5914: mov.eq r0,#0x1
  000e5916: strb.eq.w r0,[r8,#0xd8]
  000e591a: ldr.w r0,[r9,#0x0]
  000e591e: blx 0x00071878
  000e5922: cbz r0,0x000e592a
  000e5924: movs r0,#0x1
  000e5926: strb.w r0,[r8,#0xd8]
  000e592a: ldr r0,[0x000e5a50]
  000e592c: add r0,pc
  000e592e: ldr r0,[r0,#0x0]
  000e5930: ldrb r0,[r0,#0x0]
  000e5932: cbz r0,0x000e593c
  000e5934: movw r0,#0x101
  000e5938: strh.w r0,[r8,#0xd8]
  000e593c: ldrb.w r0,[r8,#0xb0]
  000e5940: cmp r0,#0x0
  000e5942: bne.w 0x000e6eca
  000e5946: ldr.w r0,[r9,#0x0]
  000e594a: blx 0x00071c14
  000e594e: blx 0x00071824
  000e5952: cmp r0,#0x4
  000e5954: beq.w 0x000e604e
  000e5958: ldr.w r0,[r9,#0x0]
  000e595c: blx 0x00071c14
  000e5960: blx 0x00071824
  000e5964: cmp r0,#0x58
  000e5966: beq.w 0x000e604e
  000e596a: ldr.w r0,[r9,#0x0]
  000e596e: blx 0x00071c14
  000e5972: blx 0x00073f00
  000e5976: cmp r0,#0x0
  000e5978: beq.w 0x000e604e
  000e597c: ldr.w r0,[r8,#0x84]
  000e5980: mov r11,r10
  000e5982: cmp r0,#0x0
  000e5984: bne.w 0x000e6870
  000e5988: movs r0,#0x74
  000e598a: blx 0x0006eb24
  000e598e: mov r5,r0
  000e5990: ldr r0,[0x000e5a54]
  000e5992: add r0,pc
  000e5994: ldr r4,[r0,#0x0]
  000e5996: ldr r0,[r4,#0x0]
  000e5998: mov.w r1,#0x1b2
  000e599c: blx 0x00072f70
  000e59a0: mov r6,r0
  000e59a2: ldr r0,[r4,#0x0]
  000e59a4: movw r1,#0x649
  000e59a8: blx 0x00072f70
  000e59ac: ldr r3,[0x000e5a58]
  000e59ae: mov r2,r0
  000e59b0: add r3,pc
  000e59b2: mov r0,r5
  000e59b4: mov r1,r6
  000e59b6: blx 0x00074f20
  000e59ba: movs r0,#0x98
  000e59bc: str.w r5,[r8,#0x84]
  000e59c0: blx 0x0006eb24
  000e59c4: ldr r1,[0x000e5a5c]
  000e59c6: mov r5,r0
  000e59c8: add r1,pc
  000e59ca: add r0,sp,#0x168
  000e59cc: movs r2,#0x0
  000e59ce: movs r6,#0x0
  000e59d0: blx 0x0006ee18
  000e59d4: movs r0,#0x2
  000e59d6: add r2,sp,#0x168
  000e59d8: strd r6,r0,[sp,#0x0]
  000e59dc: mov.w r10,#0x1
  000e59e0: mov r0,r5
  000e59e2: movs r1,#0x0
  000e59e4: movs r3,#0x0
  000e59e6: strd r10,r6,[sp,#0x8]
  000e59ea: strd r6,r6,[sp,#0x10]
  000e59ee: str r6,[sp,#0x18]
  000e59f0: blx 0x00071b30
  000e59f4: add r0,sp,#0x168
  000e59f6: blx 0x0006ee30
  000e59fa: ldr r0,[0x000e5a60]
  000e59fc: mov r2,r5
  000e59fe: ldr r1,[0x000e5a64]
  000e5a00: add r0,pc
  000e5a02: add r1,pc
  000e5a04: ldr r0,[r0,#0x0]
  000e5a06: ldr r1,[r1,#0x0]
  000e5a08: ldr r6,[r0,#0x0]
  000e5a0a: ldr r0,[r1,#0x0]
  000e5a0c: mov.w r1,#0x1b2
  000e5a10: blx 0x00074f2c
  000e5a14: mov r1,r0
  000e5a16: movs r4,#0x0
  000e5a18: mov r0,r6
  000e5a1a: movs r2,#0x0
  000e5a1c: movs r3,#0x0
  000e5a1e: str r4,[sp,#0x0]
  000e5a20: blx 0x00071548
  000e5a24: mov r0,r5
  000e5a26: blx 0x00073450
  000e5a2a: blx 0x0006eb48
  000e5a2e: strb.w r4,[r8,#0x101]
  000e5a32: strb.w r10,[r8,#0x69]
  000e5a36: b.w 0x000e6870
  000e5a68: str.w r10,[sp,#0x88]
  000e5a6c: movs r2,#0x0
  000e5a6e: ldr r0,[0x000e5e40]
  000e5a70: ldr r1,[0x000e5e44]
  000e5a72: str.w r2,[r8,#0x62]
  000e5a76: add r0,pc
  000e5a78: add r1,pc
  000e5a7a: strb.w r2,[r8,#0x6e]
  000e5a7e: strh.w r2,[r8,#0x6c]
  000e5a82: ldr r3,[r1,#0x0]
  000e5a84: mov.w r1,#0x100
  000e5a88: str.w r2,[r8,#0x68]
  000e5a8c: strh.w r2,[r8,#0x66]
  000e5a90: strh.w r1,[r8,#0x60]
  000e5a94: mov r9,r3
  000e5a96: strb.w r2,[r8,#0xdc]
  000e5a9a: strh.w r2,[r8,#0xde]
  000e5a9e: strb.w r2,[r8,#0x12c]
  000e5aa2: strb.w r2,[r8,#0x18]
  000e5aa6: strb.w r2,[r8,#0x63]
  000e5aaa: ldr r1,[r3,#0x0]
  000e5aac: ldr.w r11,[r0,#0x0]
  000e5ab0: ldr.w r4,[r1,#0xcc]
  000e5ab4: str.w r4,[r8,#0xa8]
  000e5ab8: ldr.w r0,[r11,#0x0]
  000e5abc: ldrd r3,r5,[r1,#0xc]
  000e5ac0: ldr r6,[r1,#0x28]
  000e5ac2: ldr r1,[r1,#0x4c]
  000e5ac4: subs r0,r0,r3
  000e5ac6: str r6,[sp,#0x80]
  000e5ac8: subs r0,r0,r5
  000e5aca: str r1,[sp,#0x7c]
  000e5acc: str.w r3,[r8,#0xa4]
  000e5ad0: ldr r6,[0x000e5e48]
  000e5ad2: str.w r2,[r8,#0xa0]
  000e5ad6: str.w r0,[r8,#0xac]
  000e5ada: movs r0,#0xc8
  000e5adc: add r6,pc
  000e5ade: blx 0x0006eb24
  000e5ae2: mov r5,r0
  000e5ae4: add r0,sp,#0xc8
  000e5ae6: mov r1,r6
  000e5ae8: movs r2,#0x0
  000e5aea: blx 0x0006ee18
  000e5aee: ldr r0,[0x000e5e4c]
  000e5af0: add r0,pc
  000e5af2: ldr.w r10,[r0,#0x0]
  000e5af6: ldr.w r0,[r11,#0x0]
  000e5afa: ldr.w r3,[r10,#0x0]
  000e5afe: movs r1,#0x22
  000e5b00: movs r2,#0xc
  000e5b02: strd r0,r1,[sp,#0x0]
  000e5b06: add r1,sp,#0xc8
  000e5b08: mov r0,r5
  000e5b0a: blx 0x00074578
  000e5b0e: add r0,sp,#0xc8
  000e5b10: str r4,[sp,#0x78]
  000e5b12: str.w r5,[r8,#0x90]
  000e5b16: blx 0x0006ee30
  000e5b1a: movs r0,#0xc8
  000e5b1c: blx 0x0006eb24
  000e5b20: mov r5,r0
  000e5b22: ldr r0,[0x000e5e50]
  000e5b24: ldr.w r6,[r9,#0x0]
  000e5b28: add r0,pc
  000e5b2a: ldr r0,[r0,#0x0]
  000e5b2c: ldr r0,[r0,#0x0]
  000e5b2e: blx 0x000733d8
  000e5b32: mov r2,r0
  000e5b34: add r0,sp,#0xc8
  000e5b36: mov r1,r6
  000e5b38: blx 0x00074e54
  000e5b3c: ldr.w r4,[r10,#0x0]
  000e5b40: ldr.w r0,[r8,#0x90]
  000e5b44: blx 0x00074da0
  000e5b48: ldr.w r1,[r11,#0x0]
  000e5b4c: movs r2,#0x22
  000e5b4e: subs r3,r4,r0
  000e5b50: strd r1,r2,[sp,#0x0]
  000e5b54: add r1,sp,#0xc8
  000e5b56: mov r0,r5
  000e5b58: movs r2,#0xb
  000e5b5a: blx 0x00074578
  000e5b5e: add r0,sp,#0xc8
  000e5b60: str.w r5,[r8,#0x94]
  000e5b64: blx 0x0006ee30
  000e5b68: movs r0,#0xc
  000e5b6a: blx 0x0006eb24
  000e5b6e: mov r5,r0
  000e5b70: blx 0x00074f38
  000e5b74: movs r0,#0x5
  000e5b76: mov r1,r5
  000e5b78: str.w r8,[sp,#0x70]
  000e5b7c: str.w r5,[r8,#0x8c]
  000e5b80: blx 0x00074f44
  000e5b84: ldr.w r0,[r9,#0x0]
  000e5b88: ldr r1,[0x000e5e54]
  000e5b8a: ldr.w r10,[r11,#0x0]
  000e5b8e: ldr r6,[r0,#0x10]
  000e5b90: add r1,pc
  000e5b92: ldr.w r8,[r0,#0x24]
  000e5b96: ldrd r11,r0,[r0,#0x30]
  000e5b9a: ldr r4,[r1,#0x0]
  000e5b9c: str r0,[sp,#0x74]
  000e5b9e: movs r0,#0xc8
  000e5ba0: blx 0x0006eb24
  000e5ba4: mov r5,r0
  000e5ba6: ldr r0,[r4,#0x0]
  000e5ba8: str r4,[sp,#0x84]
  000e5baa: movs r1,#0xa7
  000e5bac: blx 0x00072f70
  000e5bb0: mov r1,r0
  000e5bb2: ldr r0,[sp,#0x78]
  000e5bb4: ldr r2,[sp,#0x80]
  000e5bb6: subs r0,r0,r2
  000e5bb8: ldr r2,[sp,#0x7c]
  000e5bba: subs r4,r0,r2
  000e5bbc: sub.w r0,r10,r6
  000e5bc0: add.w r2,r11,r11, lsl #0x2
  000e5bc4: sub.w r0,r0,r8
  000e5bc8: rsbs r2,r2
  000e5bca: add r0,r2
  000e5bcc: ldr r2,[sp,#0x74]
  000e5bce: sub.w r10,r0,r2, lsl #0x2
  000e5bd2: ldr.w r0,[r9,#0x0]
  000e5bd6: ldr r3,[r0,#0x28]
  000e5bd8: movs r0,#0x1
  000e5bda: movs r2,#0x11
  000e5bdc: str r4,[sp,#0x80]
  000e5bde: strd r10,r4,[sp,#0x0]
  000e5be2: strd r2,r0,[sp,#0x8]
  000e5be6: mov r0,r5
  000e5be8: movs r2,#0x0
  000e5bea: blx 0x00074f50
  000e5bee: ldr.w r8,[sp,#0x70]
  000e5bf2: ldr.w r0,[r8,#0x8c]
  000e5bf6: ldr r0,[r0,#0x4]
  000e5bf8: str r5,[r0,#0x0]
  000e5bfa: movs r0,#0xc8
  000e5bfc: blx 0x0006eb24
  000e5c00: mov r5,r0
  000e5c02: ldr r0,[sp,#0x84]
  000e5c04: ldr r0,[r0,#0x0]
  000e5c06: mov.w r1,#0x18e
  000e5c0a: blx 0x00072f70
  000e5c0e: mov r1,r0
  000e5c10: ldr.w r0,[r9,#0x0]
  000e5c14: mov r11,r9
  000e5c16: ldr r3,[r0,#0x28]
  000e5c18: ldrd r2,r0,[r0,#0x30]
  000e5c1c: add r2,r10
  000e5c1e: movs r6,#0x11
  000e5c20: add r0,r2
  000e5c22: str r0,[sp,#0x0]
  000e5c24: ldr r0,[sp,#0x80]
  000e5c26: movs r4,#0x1
  000e5c28: strd r0,r6,[sp,#0x4]
  000e5c2c: mov r0,r5
  000e5c2e: movs r2,#0x0
  000e5c30: str r4,[sp,#0xc]
  000e5c32: blx 0x00074f50
  000e5c36: ldr.w r0,[r8,#0x8c]
  000e5c3a: ldr r0,[r0,#0x4]
  000e5c3c: str r5,[r0,#0x4]
  000e5c3e: movs r0,#0xc8
  000e5c40: blx 0x0006eb24
  000e5c44: mov r5,r0
  000e5c46: ldr r0,[sp,#0x84]
  000e5c48: ldr r0,[r0,#0x0]
  000e5c4a: movs r1,#0xb1
  000e5c4c: blx 0x00072f70
  000e5c50: mov r1,r0
  000e5c52: ldr.w r9,[sp,#0x88]
  000e5c56: ldr.w r0,[r11,#0x0]
  000e5c5a: ldr r3,[r0,#0x28]
  000e5c5c: ldrd r2,r0,[r0,#0x30]
  000e5c60: add r0,r2
  000e5c62: movs r6,#0x1
  000e5c64: movs r4,#0x11
  000e5c66: add r2,sp,#0x4
  000e5c68: add.w r0,r10,r0, lsl #0x1
  000e5c6c: str r0,[sp,#0x0]
  000e5c6e: ldr r0,[sp,#0x80]
  000e5c70: stmia r2!,{r0,r4,r6}
  000e5c72: mov r0,r5
  000e5c74: movs r2,#0x0
  000e5c76: blx 0x00074f50
  000e5c7a: ldr.w r0,[r8,#0x8c]
  000e5c7e: ldr r0,[r0,#0x4]
  000e5c80: str r5,[r0,#0x8]
  000e5c82: movs r0,#0xc8
  000e5c84: blx 0x0006eb24
  000e5c88: mov r5,r0
  000e5c8a: ldr r0,[sp,#0x84]
  000e5c8c: ldr r0,[r0,#0x0]
  000e5c8e: movs r1,#0x81
  000e5c90: blx 0x00072f70
  000e5c94: mov r1,r0
  000e5c96: ldr.w r0,[r11,#0x0]
  000e5c9a: ldr r3,[r0,#0x28]
  000e5c9c: ldrd r2,r0,[r0,#0x30]
  000e5ca0: add r0,r2
  000e5ca2: movs r6,#0x1
  000e5ca4: movs r4,#0x11
  000e5ca6: add r2,sp,#0x4
  000e5ca8: add.w r0,r0,r0, lsl #0x1
  000e5cac: add r0,r10
  000e5cae: str r0,[sp,#0x0]
  000e5cb0: ldr r0,[sp,#0x80]
  000e5cb2: stmia r2!,{r0,r4,r6}
  000e5cb4: mov r0,r5
  000e5cb6: movs r2,#0x0
  000e5cb8: blx 0x00074f50
  000e5cbc: ldr.w r0,[r8,#0x8c]
  000e5cc0: ldr r0,[r0,#0x4]
  000e5cc2: str r5,[r0,#0xc]
  000e5cc4: movs r0,#0xc8
  000e5cc6: blx 0x0006eb24
  000e5cca: mov r5,r0
  000e5ccc: ldr r0,[sp,#0x84]
  000e5cce: ldr r0,[r0,#0x0]
  000e5cd0: movs r1,#0xa9
  000e5cd2: blx 0x00072f70
  000e5cd6: mov r1,r0
  000e5cd8: ldr.w r0,[r11,#0x0]
  000e5cdc: ldr r3,[r0,#0x28]
  000e5cde: ldrd r2,r0,[r0,#0x30]
  000e5ce2: add r0,r2
  000e5ce4: movs r6,#0x1
  000e5ce6: movs r4,#0x11
  000e5ce8: add r2,sp,#0x4
  000e5cea: add.w r0,r10,r0, lsl #0x2
  000e5cee: str r0,[sp,#0x0]
  000e5cf0: ldr r0,[sp,#0x80]
  000e5cf2: stmia r2!,{r0,r4,r6}
  000e5cf4: mov r0,r5
  000e5cf6: movs r2,#0x0
  000e5cf8: blx 0x00074f50
  000e5cfc: ldr.w r0,[r8,#0x8c]
  000e5d00: mov r10,r9
  000e5d02: ldr r0,[r0,#0x4]
  000e5d04: str r5,[r0,#0x10]
  000e5d06: movs r0,#0x3c
  000e5d08: b.w 0x000e6ecc
  000e5d0c: str.w r10,[sp,#0x88]
  000e5d10: vmov.i32 q8,#0x0
  000e5d14: ldr r0,[0x000e5e58]
  000e5d16: movs r4,#0x0
  000e5d18: strb.w r4,[r8,#0x100]
  000e5d1c: add r0,pc
  000e5d1e: str.w r4,[r8,#0xe8]
  000e5d22: str.w r4,[r8,#0xe0]
  000e5d26: ldr.w r10,[r0,#0x0]
  000e5d2a: add.w r0,r8,#0xf0
  000e5d2e: vst1.64 {d16,d17},[r0]
  000e5d32: ldr.w r0,[r10,#0x0]
  000e5d36: blx 0x00071a10
  000e5d3a: blx 0x00071950
  000e5d3e: ldr.w r1,[r8,#0x14]
  000e5d42: mov.w r2,#0x10e
  000e5d46: cmp r0,#0x2
  000e5d48: it eq
  000e5d4a: mvn.eq r2,#0xc7
  000e5d4e: cmp r1,#0x0
  000e5d50: str.w r2,[r8,#0xe4]
  000e5d54: beq 0x000e5d6a
  000e5d56: vmov s0,r2
  000e5d5a: vldr.32 s2,[pc,#0x100]
  000e5d5e: vcvt.f32.s32 s0,s0
  000e5d62: vdiv.f32 s0,s0,s2
  000e5d66: vstr.32 s0,[r1,#0x4]
  000e5d6a: ldrb.w r0,[r8,#0xb3]
  000e5d6e: str.w r4,[r8,#0xec]
  000e5d72: strb.w r0,[r8,#0xb0]
  000e5d76: movs r0,#0x30
  000e5d78: blx 0x0006eb24
  000e5d7c: str r0,[sp,#0x84]
  000e5d7e: ldr r0,[0x000e5e60]
  000e5d80: ldr r1,[0x000e5e64]
  000e5d82: add r0,pc
  000e5d84: ldr r2,[0x000e5e68]
  000e5d86: ldr r3,[0x000e5e6c]
  000e5d88: add r1,pc
  000e5d8a: ldr r0,[r0,#0x0]
  000e5d8c: add r2,pc
  000e5d8e: add r3,pc
  000e5d90: ldr r1,[r1,#0x0]
  000e5d92: ldr r6,[r2,#0x0]
  000e5d94: ldr r0,[r0,#0x0]
  000e5d96: ldr r2,[r3,#0x0]
  000e5d98: ldr r1,[r1,#0x0]
  000e5d9a: str r1,[sp,#0x7c]
  000e5d9c: ldr.w r11,[r0,#0x10]
  000e5da0: ldr r0,[r6,#0x0]
  000e5da2: ldr r1,[r2,#0x0]
  000e5da4: ldr.w r3,[r8,#0xa8]
  000e5da8: str r3,[sp,#0x80]
  000e5daa: str r6,[sp,#0x78]
  000e5dac: blx 0x0006fe20
  000e5db0: mov r6,r0
  000e5db2: ldr r0,[0x000e5e70]
  000e5db4: ldr.w r9,[r8,#0xa8]
  000e5db8: add r0,pc
  000e5dba: ldr r0,[r0,#0x0]
  000e5dbc: ldr r4,[r0,#0x0]
  000e5dbe: ldr.w r0,[r10,#0x0]
  000e5dc2: blx 0x00071a10
  000e5dc6: blx 0x00071950
  000e5dca: mov r5,r0
  000e5dcc: ldr.w r0,[r10,#0x0]
  000e5dd0: blx 0x00071770
  000e5dd4: ldr r1,[sp,#0x7c]
  000e5dd6: sub.w r3,r4,r9
  000e5dda: sub.w r1,r1,r11
  000e5dde: subs r2,r1,r6
  000e5de0: strd r5,r0,[sp,#0x0]
  000e5de4: ldrd r1,r0,[sp,#0x80]
  000e5de8: blx 0x00074f5c
  000e5dec: movs r0,#0x0
  000e5dee: strd r0,r0,[r8,#0x30]
  000e5df2: ldr r1,[sp,#0x84]
  000e5df4: str.w r1,[r8,#0x1c]
  000e5df8: strh.w r0,[r8,#0xb1]
  000e5dfc: strd r0,r0,[r8,#0xd0]
  000e5e00: str.w r0,[r8,#0xd8]
  000e5e04: movs r0,#0x5c
  000e5e06: blx 0x0006eb24
  000e5e0a: mov r5,r0
  000e5e0c: blx 0x00074584
  000e5e10: ldr r0,[sp,#0x78]
  000e5e12: str.w r5,[r8,#0x70]
  000e5e16: ldr r5,[r0,#0x0]
  000e5e18: ldr.w r0,[r10,#0x0]
  000e5e1c: blx 0x00071a10
  000e5e20: blx 0x00071950
  000e5e24: ldr r1,[0x000e5e74]
  000e5e26: add.w r2,r8,#0x4c
  000e5e2a: add r1,pc
  000e5e2c: ldrh.w r1,[r1,r0,lsl #0x2]
  000e5e30: mov r0,r5
  000e5e32: blx 0x00071cf8
  000e5e36: movs r0,#0x28
  000e5e38: ldr.w r10,[sp,#0x88]
  000e5e3c: b.w 0x000e6ecc
  000e5e78: ldr r0,[0x000e6160]
  000e5e7a: movs r1,#0x0
  000e5e7c: str r1,[sp,#0x0]
  000e5e7e: movs r1,#0x7a
  000e5e80: add r0,pc
  000e5e82: movs r2,#0x0
  000e5e84: movs r3,#0x0
  000e5e86: ldr r4,[r0,#0x0]
  000e5e88: ldr r0,[r4,#0x0]
  000e5e8a: blx 0x00071548
  000e5e8e: ldr r0,[r4,#0x0]
  000e5e90: movs r1,#0x0
  000e5e92: blx 0x00074f68
  000e5e96: ldr r0,[r4,#0x0]
  000e5e98: movs r1,#0x0
  000e5e9a: blx 0x00074f74
  000e5e9e: ldr r0,[0x000e6164]
  000e5ea0: add r0,pc
  000e5ea2: ldr r4,[r0,#0x0]
  000e5ea4: ldr r1,[r4,#0x0]
  000e5ea6: adds r0,r1,#0x1
  000e5ea8: beq 0x000e5eb6
  000e5eaa: ldr r0,[0x000e6168]
  000e5eac: add r0,pc
  000e5eae: ldr r0,[r0,#0x0]
  000e5eb0: ldr r0,[r0,#0x0]
  000e5eb2: blx 0x00074f80
  000e5eb6: mov.w r0,#0xffffffff
  000e5eba: str r0,[r4,#0x0]
  000e5ebc: movs r0,#0x1
  000e5ebe: strb.w r0,[r8,#0x24]
  000e5ec2: movs r0,#0x64
  000e5ec4: str.w r0,[r8,#0xc]
  000e5ec8: ldr.w r0,[r8,#0x8c]
  000e5ecc: cbz r0,0x000e5f36
  000e5ece: ldr r1,[0x000e616c]
  000e5ed0: add.w r9,sp,#0xc8
  000e5ed4: movs r6,#0x0
  000e5ed6: add r1,pc
  000e5ed8: ldr r4,[r1,#0x0]
  000e5eda: ldr r1,[0x000e6170]
  000e5edc: add r1,pc
  000e5ede: ldr r5,[r1,#0x0]
  000e5ee0: b 0x000e5f28
  000e5ee2: cmp r6,#0x9
  000e5ee4: bhi 0x000e5f26
  000e5ee6: ldr r0,[r0,#0x4]
  000e5ee8: ldr.w r1,[r0,r6,lsl #0x2]
  000e5eec: mov r0,r9
  000e5eee: blx 0x00074dd0
  000e5ef2: vldr.32 s0,[sp,#0xc8]
  000e5ef6: vcvt.s32.f32 s0,s0
  000e5efa: vmov r0,s0
  000e5efe: str.w r0,[r4,r6,lsl #0x2]
  000e5f02: ldr.w r0,[r8,#0x8c]
  000e5f06: ldr r0,[r0,#0x4]
  000e5f08: ldr.w r1,[r0,r6,lsl #0x2]
  000e5f0c: mov r0,r9
  000e5f0e: blx 0x00074dd0
  000e5f12: vldr.32 s0,[sp,#0xcc]
  000e5f16: vcvt.s32.f32 s0,s0
  000e5f1a: vmov r0,s0
  000e5f1e: str.w r0,[r5,r6,lsl #0x2]
  000e5f22: ldr.w r0,[r8,#0x8c]
  000e5f26: adds r6,#0x1
  000e5f28: ldr r1,[r0,#0x0]
  000e5f2a: cmp r6,r1
  000e5f2c: bcc 0x000e5ee2
  000e5f2e: ldr r0,[0x000e6174]
  000e5f30: add r0,pc
  000e5f32: ldr r0,[r0,#0x0]
  000e5f34: str r1,[r0,#0x0]
  000e5f36: movs r0,#0x0
  000e5f38: b.w 0x000e6ed0
  000e5f3c: ldrb.w r0,[r8,#0xb1]
  000e5f40: cbnz r0,0x000e5f72
  000e5f42: ldrb.w r0,[r8,#0xb0]
  000e5f46: cbnz r0,0x000e5f72
  000e5f48: ldrb.w r0,[r8,#0x101]
  000e5f4c: cbz r0,0x000e5f72
  000e5f4e: ldr r0,[0x000e6178]
  000e5f50: add r0,pc
  000e5f52: ldr r4,[r0,#0x0]
  000e5f54: ldr r0,[r4,#0x0]
  000e5f56: blx 0x00071770
  000e5f5a: cmp r0,#0x4d
  000e5f5c: bne 0x000e5f6c
  000e5f5e: ldr r0,[r4,#0x0]
  000e5f60: blx 0x00071c14
  000e5f64: blx 0x00071824
  000e5f68: cmp r0,#0x65
  000e5f6a: beq 0x000e5f72
  000e5f6c: mov r0,r8
  000e5f6e: blx 0x00074f8c
  000e5f72: ldr r0,[0x000e617c]
  000e5f74: add r0,pc
  000e5f76: ldr.w r9,[r0,#0x0]
  000e5f7a: ldr.w r0,[r9,#0x0]
  000e5f7e: blx 0x00071770
  000e5f82: cmp r0,#0x1
  000e5f84: bne 0x000e601c
  000e5f86: ldr r0,[0x000e6180]
  000e5f88: mov.w r1,#0xffffffff
  000e5f8c: ldr.w r5,[r9,#0x0]
  000e5f90: add r0,pc
  000e5f92: ldr r0,[r0,#0x0]
  000e5f94: ldr r0,[r0,#0x0]
  000e5f96: ldr r0,[r0,#0x4]
  000e5f98: ldr r0,[r0,#0x0]
  000e5f9a: blx 0x000719d4
  000e5f9e: mov r1,r0
  000e5fa0: mov r0,r5
  000e5fa2: blx 0x00073870
  000e5fa6: ldr r1,[0x000e6184]
  000e5fa8: ldr.w r0,[r9,#0x0]
  000e5fac: add r1,pc
  000e5fae: ldr r6,[r1,#0x0]
  000e5fb0: blx r6
  000e5fb2: movs r1,#0x8
  000e5fb4: blx 0x000719ec
  000e5fb8: ldr r0,[0x000e6188]
  000e5fba: add r0,pc
  000e5fbc: ldr r4,[r0,#0x0]
  000e5fbe: ldr r0,[r4,#0x0]
  000e5fc0: ldr r0,[r0,#0x4]
  000e5fc2: ldr.w r0,[r0,#0x168]
  000e5fc6: blx 0x000718a8
  000e5fca: movs r1,#0x1
  000e5fcc: mov r5,r0
  000e5fce: blx 0x00073330
  000e5fd2: ldr.w r0,[r9,#0x0]
  000e5fd6: blx r6
  000e5fd8: mov r1,r5
  000e5fda: movs r2,#0x0
  000e5fdc: blx 0x0007387c
  000e5fe0: ldr r0,[r4,#0x0]
  000e5fe2: ldr r0,[r0,#0x4]
  000e5fe4: ldr.w r0,[r0,#0x144]
  000e5fe8: blx 0x000718a8
  000e5fec: mov r5,r0
  000e5fee: ldr.w r0,[r9,#0x0]
  000e5ff2: blx r6
  000e5ff4: mov r1,r5
  000e5ff6: movs r2,#0x1
  000e5ff8: blx 0x0007387c
  000e5ffc: mov r0,r5
  000e5ffe: movs r1,#0x1
  000e6000: blx 0x00073330
  000e6004: ldr.w r0,[r9,#0x0]
  000e6008: ldr.w r5,[r8,#0x14]
  000e600c: blx r6
  000e600e: blx 0x000719c8
  000e6012: mov r1,r0
  000e6014: mov r0,r5
  000e6016: movs r2,#0x3
  000e6018: blx 0x00074f98
  000e601c: ldr.w r0,[r9,#0x0]
  000e6020: blx 0x00071770
  000e6024: mov r5,r0
  000e6026: cmp r0,#0x4
  000e6028: bgt 0x000e603e
  000e602a: mov r6,r8
  000e602c: movs r1,#0x1
  000e602e: ldr.w r0,[r6,#0x8c]!
  000e6032: ldr r0,[r0,#0x4]
  000e6034: ldr r0,[r0,#0x0]
  000e6036: blx 0x00074fa4
  000e603a: movs r4,#0x0
  000e603c: b 0x000e619a
  000e603e: cmp r5,#0x9
  000e6040: blt.w 0x000e618c
  000e6044: cmp r5,#0xf
  000e6046: beq.w 0x000e618c
  000e604a: movs r4,#0x0
  000e604c: b 0x000e61b2
  000e604e: ldr.w r0,[r9,#0x0]
  000e6052: mov r11,r10
  000e6054: blx 0x00071770
  000e6058: cmp r0,#0x30
  000e605a: beq.w 0x000e6870
  000e605e: ldr.w r0,[r9,#0x0]
  000e6062: blx 0x00071998
  000e6066: cmp r0,#0x0
  000e6068: bne.w 0x000e6870
  000e606c: ldr.w r0,[r9,#0x0]
  000e6070: blx 0x00071c14
  000e6074: blx 0x00071824
  000e6078: cmp r0,#0x6c
  000e607a: beq.w 0x000e6870
  000e607e: ldr.w r0,[r9,#0x0]
  000e6082: blx 0x00071c14
  000e6086: blx 0x00071824
  000e608a: cmp r0,#0x64
  000e608c: beq.w 0x000e6870
  000e6090: ldr.w r0,[r9,#0x0]
  000e6094: blx 0x00071c14
  000e6098: blx 0x00071824
  000e609c: cmp r0,#0x65
  000e609e: beq.w 0x000e6870
  000e60a2: ldrb.w r0,[r8,#0x6a]
  000e60a6: cmp r0,#0x0
  000e60a8: bne.w 0x000e6870
  000e60ac: ldrb.w r0,[r8,#0x63]
  000e60b0: cmp r0,#0x0
  000e60b2: bne.w 0x000e6870
  000e60b6: ldrb.w r0,[r8,#0xd8]
  000e60ba: cmp r0,#0x0
  000e60bc: bne.w 0x000e6870
  000e60c0: ldr.w r0,[r9,#0x0]
  000e60c4: ldrb.w r1,[r0,#0x108]
  000e60c8: cmp r1,#0x0
  000e60ca: bne.w 0x000e6870
  000e60ce: blx 0x00071c14
  000e60d2: blx 0x00073f24
  000e60d6: cbnz r0,0x000e60fc
  000e60d8: ldr.w r0,[r9,#0x0]
  000e60dc: blx 0x00071b84
  000e60e0: mov r5,r0
  000e60e2: ldr.w r0,[r9,#0x0]
  000e60e6: blx 0x00071a10
  000e60ea: blx 0x00071950
  000e60ee: mov r1,r0
  000e60f0: mov r0,r5
  000e60f2: blx 0x00071b90
  000e60f6: cmp r0,#0x0
  000e60f8: beq.w 0x000e6870
  000e60fc: add r0,sp,#0xc8
  000e60fe: blx 0x0006efbc
  000e6102: ldr.w r0,[r9,#0x0]
  000e6106: blx 0x00071b84
  000e610a: mov r5,r0
  000e610c: ldr.w r0,[r9,#0x0]
  000e6110: blx 0x00071a10
  000e6114: blx 0x00071950
  000e6118: mov r1,r0
  000e611a: mov r0,r5
  000e611c: blx 0x00071b90
  000e6120: mov r1,r0
  000e6122: ldr.w r0,[r9,#0x0]
  000e6126: cmp r1,#0x0
  000e6128: beq.w 0x000e670e
  000e612c: blx 0x00071b84
  000e6130: mov r5,r0
  000e6132: ldr.w r0,[r9,#0x0]
  000e6136: blx 0x00071a10
  000e613a: blx 0x00071950
  000e613e: movs r6,#0x0
  000e6140: cmp r0,#0x1
  000e6142: it hi
  000e6144: mov.hi r6,#0x1
  000e6146: mov r0,r5
  000e6148: mov r1,r6
  000e614a: blx 0x00074fb0
  000e614e: cmp r0,#0x1
  000e6150: blt.w 0x000e6780
  000e6154: mov r0,r5
  000e6156: mov r1,r6
  000e6158: blx 0x00074fb0
  000e615c: b 0x000e678a
  000e618c: sub.w r0,r5,#0xf
  000e6190: add.w r6,r8,#0x8c
  000e6194: clz r0,r0
  000e6198: lsrs r4,r0,#0x5
  000e619a: ldr r0,[r6,#0x0]
  000e619c: movs r1,#0x1
  000e619e: ldr r0,[r0,#0x4]
  000e61a0: ldr r0,[r0,#0x8]
  000e61a2: blx 0x00074fa4
  000e61a6: ldr r0,[r6,#0x0]
  000e61a8: movs r1,#0x1
  000e61aa: ldr r0,[r0,#0x4]
  000e61ac: ldr r0,[r0,#0xc]
  000e61ae: blx 0x00074fa4
  000e61b2: cmp r5,#0xc
  000e61b4: blt 0x000e61d8
  000e61b6: cbnz r4,0x000e61d8
  000e61b8: ldr.w r0,[r9,#0x0]
  000e61bc: blx 0x00071c14
  000e61c0: blx 0x00071824
  000e61c4: cmp r0,#0x64
  000e61c6: beq 0x000e61d8
  000e61c8: ldr.w r0,[r9,#0x0]
  000e61cc: blx 0x00071c14
  000e61d0: blx 0x00071824
  000e61d4: cmp r0,#0x65
  000e61d6: bne 0x000e61e6
  000e61d8: ldr.w r0,[r8,#0x8c]
  000e61dc: movs r1,#0x1
  000e61de: ldr r0,[r0,#0x4]
  000e61e0: ldr r0,[r0,#0x4]
  000e61e2: blx 0x00074fa4
  000e61e6: ldr r0,[0x000e6394]
  000e61e8: add r0,pc
  000e61ea: ldr r6,[r0,#0x0]
  000e61ec: ldr r5,[r6,#0x0]
  000e61ee: mov r0,r5
  000e61f0: blx 0x000717f4
  000e61f4: mov r1,r0
  000e61f6: mov r0,r5
  000e61f8: blx 0x0006ff1c
  000e61fc: add r5,sp,#0x114
  000e61fe: mov r1,r0
  000e6200: mov r0,r5
  000e6202: blx 0x0006f4c0
  000e6206: vldr.32 s0,[pc,#0x190]
  000e620a: add.w r0,r8,#0x11c
  000e620e: vldr.32 s4,[sp,#0x118]
  000e6212: mov r1,r5
  000e6214: vldr.32 s2,[sp,#0x114]
  000e6218: vldr.32 s6,[pc,#0x180]
  000e621c: vadd.f32 s0,s4,s0
  000e6220: vadd.f32 s2,s2,s6
  000e6224: vstr.32 s0,[sp,#0x118]
  000e6228: vstr.32 s2,[sp,#0x114]
  000e622c: blx 0x0006eb3c
  000e6230: mov r0,r8
  000e6232: blx 0x00074fbc
  000e6236: ldrb.w r0,[r8,#0x101]
  000e623a: cbz r0,0x000e6240
  000e623c: blx 0x00074fc8
  000e6240: ldr.w r0,[r9,#0x0]
  000e6244: blx 0x00071770
  000e6248: cmp r0,#0x4d
  000e624a: bne 0x000e6292
  000e624c: ldr.w r0,[r9,#0x0]
  000e6250: blx 0x00071c14
  000e6254: blx 0x00071824
  000e6258: cmp r0,#0x64
  000e625a: bne 0x000e6292
  000e625c: ldr.w r0,[r9,#0x0]
  000e6260: blx 0x00071c14
  000e6264: blx 0x00073708
  000e6268: mov r5,r0
  000e626a: cbz r0,0x000e6292
  000e626c: movs r4,#0x0
  000e626e: b 0x000e628c
  000e6270: ldr r0,[r5,#0x4]
  000e6272: ldr.w r0,[r0,r4,lsl #0x2]
  000e6276: blx 0x000719c8
  000e627a: cmp r0,#0x25
  000e627c: bne 0x000e628a
  000e627e: ldr r0,[r5,#0x4]
  000e6280: movs r1,#0x0
  000e6282: ldr.w r0,[r0,r4,lsl #0x2]
  000e6286: blx 0x00074fd4
  000e628a: adds r4,#0x1
  000e628c: ldr r0,[r5,#0x0]
  000e628e: cmp r4,r0
  000e6290: bcc 0x000e6270
  000e6292: ldr.w r0,[r9,#0x0]
  000e6296: blx 0x00071c14
  000e629a: blx 0x00071824
  000e629e: cmp r0,#0x65
  000e62a0: str r6,[sp,#0x84]
  000e62a2: bne 0x000e62aa
  000e62a4: mov.w r11,#0x8
  000e62a8: b 0x000e62ce
  000e62aa: ldr.w r0,[r9,#0x0]
  000e62ae: blx 0x00071c14
  000e62b2: blx 0x00071824
  000e62b6: cmp r0,#0x64
  000e62b8: bne 0x000e62c0
  000e62ba: mov.w r11,#0x7
  000e62be: b 0x000e62ce
  000e62c0: ldr.w r0,[r9,#0x0]
  000e62c4: blx 0x00071a10
  000e62c8: blx 0x00071950
  000e62cc: mov r11,r0
  000e62ce: adr.w r1,0xe6fc8
  000e62d2: vmov.i32 q8,#0x0
  000e62d6: add r2,sp,#0xc8
  000e62d8: vld1.64 {d18,d19},[r1]
  000e62dc: orr r1,r2,#0x4
  000e62e0: ldr r0,[0x000e63a0]
  000e62e2: vst1.32 {d16,d17},[r1]
  000e62e6: add.w r1,r11,r11, lsl #0x1
  000e62ea: add r0,pc
  000e62ec: add.w r0,r0,r1, lsl #0x2
  000e62f0: add.w r1,r2,#0x18
  000e62f4: vldr.32 s0,[r0]
  000e62f8: vldr.32 s2,[r0,#0x4]
  000e62fc: vldr.32 s4,[r0,#0x8]
  000e6300: add.w r0,r2,#0x28
  000e6304: vst1.64 {d16,d17},[r1]
  000e6308: vcvt.f32.s32 s16,s2
  000e630c: vst1.64 {d18,d19},[r0]
  000e6310: vcvt.f32.s32 s18,s0
  000e6314: vcvt.f32.s32 s20,s4
  000e6318: mov.w r0,#0x3f800000
  000e631c: str r0,[sp,#0xc8]
  000e631e: str r0,[sp,#0xdc]
  000e6320: str r0,[sp,#0x100]
  000e6322: ldr.w r0,[r8,#0x13c]
  000e6326: cmp r0,#0x0
  000e6328: vstr.32 s18,[r8,#0x130]
  000e632c: vstr.32 s16,[r8,#0x134]
  000e6330: vstr.32 s20,[r8,#0x138]
  000e6334: beq 0x000e6342
  000e6336: vmov r1,s18
  000e633a: mov r2,r1
  000e633c: blx 0x0006f4fc
  000e6340: b 0x000e6358
  000e6342: movs r0,#0x10
  000e6344: blx 0x0006eb24
  000e6348: vmov r1,s18
  000e634c: mov r6,r0
  000e634e: mov r2,r1
  000e6350: blx 0x00074ecc
  000e6354: str.w r6,[r8,#0x13c]
  000e6358: ldr.w r0,[r8,#0x140]
  000e635c: cbz r0,0x000e636a
  000e635e: ldr.w r1,[r8,#0x134]
  000e6362: mov r2,r1
  000e6364: blx 0x0006f4fc
  000e6368: b 0x000e6380
  000e636a: movs r0,#0x10
  000e636c: blx 0x0006eb24
  000e6370: ldr.w r1,[r8,#0x134]
  000e6374: mov r6,r0
  000e6376: mov r2,r1
  000e6378: blx 0x00074ecc
  000e637c: str.w r6,[r8,#0x140]
  000e6380: ldr.w r0,[r8,#0x144]
  000e6384: cbz r0,0x000e63a4
  000e6386: ldr.w r1,[r8,#0x138]
  000e638a: mov r2,r1
  000e638c: blx 0x0006f4fc
  000e6390: b 0x000e63ba
  000e63a4: movs r0,#0x10
  000e63a6: blx 0x0006eb24
  000e63aa: ldr.w r1,[r8,#0x138]
  000e63ae: mov r6,r0
  000e63b0: mov r2,r1
  000e63b2: blx 0x00074ecc
  000e63b6: str.w r6,[r8,#0x144]
  000e63ba: ldr.w r0,[r8,#0x14]
  000e63be: cmp r0,#0x0
  000e63c0: beq.w 0x000e664e
  000e63c4: vmov r2,s18
  000e63c8: str.w r9,[sp,#0x80]
  000e63cc: vmov r3,s16
  000e63d0: add.w r9,sp,#0xc8
  000e63d4: add r0,sp,#0x8c
  000e63d6: str.w r10,[sp,#0x88]
  000e63da: mov r1,r9
  000e63dc: vstr.32 s20,[sp]
  000e63e0: blx 0x0006f820
  000e63e4: ldr r0,[0x000e6750]
  000e63e6: ldr r1,[0x000e6754]
  000e63e8: add r0,pc
  000e63ea: ldr r4,[0x000e6758]
  000e63ec: add r1,pc
  000e63ee: ldr r5,[0x000e675c]
  000e63f0: add.w r1,r1,r11, lsl #0x2
  000e63f4: ldr.w r10,[r0,#0x0]
  000e63f8: add r4,pc
  000e63fa: add r5,pc
  000e63fc: vldr.32 s22,[r1]
  000e6400: movs r1,#0x2
  000e6402: ldrb.w r0,[r10,#0x0]
  000e6406: vmov r2,s22
  000e640a: cmp r0,#0x0
  000e640c: mov r0,r4
  000e640e: it eq
  000e6410: mov.eq r0,r5
  000e6412: str r1,[sp,#0x4]
  000e6414: ldr.w r3,[r0,r11,lsl #0x2]
  000e6418: mov r1,r9
  000e641a: ldr r0,[0x000e6760]
  000e641c: str r0,[sp,#0x0]
  000e641e: add r0,sp,#0x8c
  000e6420: blx 0x00073060
  000e6424: ldr r0,[sp,#0x84]
  000e6426: ldr r6,[r0,#0x0]
  000e6428: mov r0,r6
  000e642a: blx 0x000717f4
  000e642e: mov r1,r0
  000e6430: mov r0,r6
  000e6432: mov r2,r9
  000e6434: blx 0x000720a0
  000e6438: vstr.32 s16,[sp,#0x90]
  000e643c: add r1,sp,#0x8c
  000e643e: vstr.32 s18,[sp,#0x8c]
  000e6442: vstr.32 s20,[sp,#0x94]
  000e6446: ldr.w r0,[r8,#0x14]
  000e644a: adds r0,#0x8
  000e644c: blx 0x0006eb3c
  000e6450: ldrb.w r0,[r10,#0x0]
  000e6454: ldr r1,[0x000e6760]
  000e6456: cmp r0,#0x0
  000e6458: it eq
  000e645a: mov.eq r4,r5
  000e645c: ldr.w r0,[r4,r11,lsl #0x2]
  000e6460: vstr.32 s22,[sp,#0x8c]
  000e6464: strd r0,r1,[sp,#0x90]
  000e6468: add r1,sp,#0x8c
  000e646a: ldr.w r0,[r8,#0x14]
  000e646e: adds r0,#0x14
  000e6470: blx 0x0006eb3c
  000e6474: ldr.w r0,[r8,#0x14]
  000e6478: mov r1,r9
  000e647a: mov r10,r9
  000e647c: ldr r0,[r0,#0x20]
  000e647e: blx 0x000721e4
  000e6482: ldr r0,[0x000e6764]
  000e6484: ldr r1,[0x000e6768]
  000e6486: add r0,pc
  000e6488: add r1,pc
  000e648a: ldr r6,[r0,#0x0]
  000e648c: ldr r5,[r1,#0x0]
  000e648e: movs r1,#0x14
  000e6490: ldr r0,[r6,#0x0]
  000e6492: blx r5
  000e6494: cmp r0,#0xa
  000e6496: mov.w r0,#0x0
  000e649a: it lt
  000e649c: mov.lt r0,#0x1
  000e649e: movs r1,#0x14
  000e64a0: strb.w r0,[r8,#0x12d]
  000e64a4: movs r4,#0x0
  000e64a6: ldr r0,[r6,#0x0]
  000e64a8: blx r5
  000e64aa: cmp r0,#0xa
  000e64ac: mov.w r0,#0x0
  000e64b0: it gt
  000e64b2: mov.gt r0,#0x1
  000e64b4: movs r1,#0x14
  000e64b6: strb.w r0,[r8,#0x12e]
  000e64ba: ldr r0,[r6,#0x0]
  000e64bc: blx r5
  000e64be: ldrb.w r9,[r8,#0x12e]
  000e64c2: cmp r0,#0xa
  000e64c4: it lt
  000e64c6: mov.lt r4,#0x1
  000e64c8: movs r1,#0x96
  000e64ca: strb.w r4,[r8,#0x12f]
  000e64ce: ldr r0,[r6,#0x0]
  000e64d0: blx r5
  000e64d2: cmp.w r9,#0x0
  000e64d6: it ne
  000e64d8: rsb.ne r0,r0
  000e64da: vmov s0,r0
  000e64de: movs r1,#0x96
  000e64e0: vcvt.f32.s32 s0,s0
  000e64e4: ldr.w r0,[r8,#0x14]
  000e64e8: vldr.32 s2,[r0,#0x8]
  000e64ec: vadd.f32 s0,s2,s0
  000e64f0: vstr.32 s0,[r0,#0x8]
  000e64f4: ldr r0,[r6,#0x0]
  000e64f6: ldrb.w r4,[r8,#0x12d]
  000e64fa: blx r5
  000e64fc: cmp r4,#0x0
  000e64fe: it ne
  000e6500: rsb.ne r0,r0
  000e6502: vmov s0,r0
  000e6506: movs r1,#0x96
  000e6508: vcvt.f32.s32 s0,s0
  000e650c: ldr.w r0,[r8,#0x14]
  000e6510: vldr.32 s2,[r0,#0xc]
  000e6514: vadd.f32 s0,s2,s0
  000e6518: vstr.32 s0,[r0,#0xc]
  000e651c: ldr r0,[r6,#0x0]
  000e651e: ldrb.w r4,[r8,#0x12f]
  000e6522: blx r5
  000e6524: cmp r4,#0x0
  000e6526: it ne
  000e6528: rsb.ne r0,r0
  000e652a: vmov s0,r0
  000e652e: mov r1,r10
  000e6530: vcvt.f32.s32 s0,s0
  000e6534: ldr.w r0,[r8,#0x14]
  000e6538: ldrd r2,r3,[r0,#0x8]
  000e653c: vldr.32 s2,[r0,#0x10]
  000e6540: vadd.f32 s0,s2,s0
  000e6544: vstr.32 s0,[r0,#0x10]
  000e6548: add r0,sp,#0x8c
  000e654a: vstr.32 s0,[sp]
  000e654e: blx 0x0006f820
  000e6552: ldr.w r6,[r8,#0x20]
  000e6556: cbz r6,0x000e65c4
  000e6558: ldr r0,[sp,#0xc8]
  000e655a: add r3,sp,#0xf8
  000e655c: str r0,[sp,#0x84]
  000e655e: ldr r0,[sp,#0xcc]
  000e6560: str r0,[sp,#0x7c]
  000e6562: ldr r0,[sp,#0xd0]
  000e6564: str r0,[sp,#0x78]
  000e6566: ldr r0,[sp,#0xd4]
  000e6568: str r0,[sp,#0x74]
  000e656a: ldrd r5,r4,[sp,#0xd8]
  000e656e: ldrd r12,lr,[sp,#0xe0]
  000e6572: ldrd r10,r11,[sp,#0xe8]
  000e6576: ldrd r9,r0,[sp,#0xf0]
  000e657a: ldmia r3,{r1,r2,r3}
  000e657c: str r1,[sp,#0x60]
  000e657e: str r2,[sp,#0x64]
  000e6580: str r3,[sp,#0x68]
  000e6582: strd r5,r4,[sp,#0x40]
  000e6586: strd r12,lr,[sp,#0x48]
  000e658a: strd r10,r11,[sp,#0x50]
  000e658e: strd r9,r0,[sp,#0x58]
  000e6592: str r0,[sp,#0x20]
  000e6594: str r1,[sp,#0x24]
  000e6596: str r2,[sp,#0x28]
  000e6598: str r3,[sp,#0x2c]
  000e659a: ldr r1,[sp,#0x84]
  000e659c: ldr r2,[sp,#0x7c]
  000e659e: ldr r3,[sp,#0x78]
  000e65a0: ldr r0,[sp,#0x74]
  000e65a2: str r1,[sp,#0x30]
  000e65a4: str r2,[sp,#0x34]
  000e65a6: str r3,[sp,#0x38]
  000e65a8: str r0,[sp,#0x3c]
  000e65aa: strd r0,r5,[sp,#0x0]
  000e65ae: add r0,sp,#0x8
  000e65b0: stm r0,{r4,r12,lr}
  000e65b4: mov r0,r6
  000e65b6: strd r10,r11,[sp,#0x14]
  000e65ba: str.w r9,[sp,#0x1c]
  000e65be: blx 0x0006f130
  000e65c2: b 0x000e6646
  000e65c4: movs r0,#0xf4
  000e65c6: blx 0x0006eb24
  000e65ca: str r0,[sp,#0x84]
  000e65cc: add r3,sp,#0xf8
  000e65ce: ldr r0,[sp,#0xc8]
  000e65d0: str r0,[sp,#0x7c]
  000e65d2: ldr r0,[sp,#0xcc]
  000e65d4: str r0,[sp,#0x78]
  000e65d6: ldr r0,[sp,#0xd0]
  000e65d8: str r0,[sp,#0x74]
  000e65da: ldrd r9,r6,[sp,#0xd4]
  000e65de: ldrd r4,r5,[sp,#0xdc]
  000e65e2: ldr r0,[sp,#0xe4]
  000e65e4: str r0,[sp,#0x70]
  000e65e6: ldrd lr,r11,[sp,#0xe8]
  000e65ea: ldrd r10,r0,[sp,#0xf0]
  000e65ee: ldmia r3,{r1,r2,r3}
  000e65f0: add.w r12,sp,#0x60
  000e65f4: stm.w r12!,{r1,r2,r3}
  000e65f8: movw r12,#0xbb8
  000e65fc: str.w r12,[sp,#0x6c]
  000e6600: strd r6,r4,[sp,#0x40]
  000e6604: ldr.w r12,[sp,#0x70]
  000e6608: str r5,[sp,#0x48]
  000e660a: strd r12,lr,[sp,#0x4c]
  000e660e: strd r11,r10,[sp,#0x54]
  000e6612: str r1,[sp,#0x24]
  000e6614: str r2,[sp,#0x28]
  000e6616: str r3,[sp,#0x2c]
  000e6618: ldr r1,[sp,#0x7c]
  000e661a: ldr r2,[sp,#0x78]
  000e661c: ldr r3,[sp,#0x74]
  000e661e: str r0,[sp,#0x5c]
  000e6620: str r0,[sp,#0x20]
  000e6622: str r1,[sp,#0x30]
  000e6624: str r2,[sp,#0x34]
  000e6626: strd r3,r9,[sp,#0x38]
  000e662a: strd r9,r6,[sp,#0x0]
  000e662e: strd r4,r5,[sp,#0x8]
  000e6632: ldr r5,[sp,#0x84]
  000e6634: strd r12,lr,[sp,#0x10]
  000e6638: strd r11,r10,[sp,#0x18]
  000e663c: mov r0,r5
  000e663e: blx 0x00072b74
  000e6642: str.w r5,[r8,#0x20]
  000e6646: ldr.w r10,[sp,#0x88]
  000e664a: ldr.w r9,[sp,#0x80]
  000e664e: ldrb.w r0,[r8,#0x69]
  000e6652: cmp r0,#0x0
  000e6654: bne 0x000e66ec
  000e6656: ldrb.w r0,[r8,#0x6a]
  000e665a: cmp r0,#0x0
  000e665c: bne 0x000e66ec
  000e665e: ldrb.w r0,[r8,#0x63]
  000e6662: cmp r0,#0x0
  000e6664: bne 0x000e66ec
  000e6666: ldr r0,[0x000e676c]
  000e6668: add r0,pc
  000e666a: ldr r0,[r0,#0x0]
  000e666c: ldr r0,[r0,#0x0]
  000e666e: ldrb.w r0,[r0,#0x2ec]
  000e6672: cbnz r0,0x000e66ec
  000e6674: ldr.w r0,[r9,#0x0]
  000e6678: blx 0x00071c14
  000e667c: blx 0x00071824
  000e6680: cmp r0,#0x65
  000e6682: beq 0x000e66ec
  000e6684: ldr.w r0,[r9,#0x0]
  000e6688: blx 0x00071a58
  000e668c: blx 0x00071a64
  000e6690: mov r5,r0
  000e6692: ldr.w r0,[r9,#0x0]
  000e6696: blx 0x00071a58
  000e669a: movs r1,#0x55
  000e669c: movs r2,#0x1
  000e669e: blx 0x00071a70
  000e66a2: mov r6,r0
  000e66a4: ldr.w r0,[r9,#0x0]
  000e66a8: blx 0x00071a10
  000e66ac: blx 0x00071aac
  000e66b0: cbnz r0,0x000e66ec
  000e66b2: ldr.w r0,[r9,#0x0]
  000e66b6: orr.w r4,r5,r6
  000e66ba: blx 0x00071770
  000e66be: cbnz r4,0x000e66ec
  000e66c0: cmp r0,#0x11
  000e66c2: blt 0x000e66ec
  000e66c4: ldr r0,[0x000e6770]
  000e66c6: movw r1,#0x14f
  000e66ca: ldr.w r5,[r8,#0x70]
  000e66ce: add r0,pc
  000e66d0: ldr r0,[r0,#0x0]
  000e66d2: ldr r0,[r0,#0x0]
  000e66d4: blx 0x00072f70
  000e66d8: mov r1,r0
  000e66da: mov r0,r5
  000e66dc: movs r2,#0x1
  000e66de: movs r4,#0x1
  000e66e0: blx 0x000746f8
  000e66e4: strb.w r4,[r8,#0x6d]
  000e66e8: strb.w r4,[r8,#0x63]
  000e66ec: ldr.w r1,[r9,#0x0]
  000e66f0: movs r0,#0x1
  000e66f2: strb.w r0,[r1,#0xf8]
  000e66f6: movs r1,#0x0
  000e66f8: strb.w r1,[r8,#0x118]
  000e66fc: str.w r1,[r8,#0x128]
  000e6700: strb.w r1,[r8,#0xb3]
  000e6704: str.w r1,[r8,#0x2c]
  000e6708: strb.w r1,[r8,#0x148]
  000e670c: b 0x000e6ecc
  000e670e: blx 0x00071c5c
  000e6712: movs r1,#0x96
  000e6714: muls r0,r1
  000e6716: add.w r0,r0,#0x3e8
  000e671a: str.w r0,[r8,#0xd4]
  000e671e: ldr.w r0,[r9,#0x0]
  000e6722: blx 0x0007192c
  000e6726: cbz r0,0x000e6738
  000e6728: ldr.w r0,[r8,#0xd4]
  000e672c: mov r1,r8
  000e672e: add.w r0,r0,r0, lsl #0x2
  000e6732: lsls r0,r0,#0x1
  000e6734: str.w r0,[r8,#0xd4]
  000e6738: ldr r0,[0x000e6774]
  000e673a: add r0,pc
  000e673c: ldr r0,[r0,#0x0]
  000e673e: ldr r0,[r0,#0x0]
  000e6740: movs r1,#0xce
  000e6742: blx 0x00072f70
  000e6746: mov r1,r0
  000e6748: add r0,sp,#0xc8
  000e674a: blx 0x0006f2b0
  000e674e: b 0x000e67f6
  000e6780: mov r0,r5
  000e6782: mov r1,r6
  000e6784: blx 0x00074fb0
  000e6788: rsbs r0,r0
  000e678a: vmov s0,r0
  000e678e: vldr.32 s2,[pc,#-0x18]
  000e6792: mov r4,r8
  000e6794: vcvt.f32.s32 s0,s0
  000e6798: vdiv.f32 s0,s0,s2
  000e679c: vldr.32 s2,[pc,#-0x24]
  000e67a0: vmul.f32 s0,s0,s2
  000e67a4: vcvt.s32.f32 s0,s0
  000e67a8: ldr r0,[0x000e6b1c]
  000e67aa: add r0,pc
  000e67ac: ldr r0,[r0,#0x0]
  000e67ae: vstr.32 s0,[r8,#0xd4]
  000e67b2: ldr r0,[r0,#0x0]
  000e67b4: movs r1,#0xc8
  000e67b6: blx 0x00071848
  000e67ba: ldr.w r1,[r4,#0xd4]
  000e67be: add r0,r1
  000e67c0: subs r0,#0x64
  000e67c2: str.w r0,[r4,#0xd4]
  000e67c6: ldr.w r0,[r9,#0x0]
  000e67ca: blx 0x0007192c
  000e67ce: cbz r0,0x000e67e0
  000e67d0: ldr.w r0,[r8,#0xd4]
  000e67d4: mov r1,r8
  000e67d6: add.w r0,r0,r0, lsl #0x2
  000e67da: lsls r0,r0,#0x1
  000e67dc: str.w r0,[r8,#0xd4]
  000e67e0: ldr r0,[0x000e6b20]
  000e67e2: add r0,pc
  000e67e4: ldr r0,[r0,#0x0]
  000e67e6: ldr r0,[r0,#0x0]
  000e67e8: movs r1,#0xcd
  000e67ea: blx 0x00072f70
  000e67ee: mov r1,r0
  000e67f0: add r0,sp,#0xc8
  000e67f2: blx 0x0006f2b0
  000e67f6: ldr.w r5,[r9,#0x0]
  000e67fa: add r0,sp,#0x15c
  000e67fc: add r1,sp,#0xc8
  000e67fe: movs r2,#0x0
  000e6800: blx 0x0006f028
  000e6804: ldr r0,[0x000e6b24]
  000e6806: ldr.w r2,[r8,#0xd4]
  000e680a: add r0,pc
  000e680c: ldr r0,[r0,#0x0]
  000e680e: ldr r1,[r0,#0x0]
  000e6810: add r0,sp,#0x150
  000e6812: blx 0x00074e54
  000e6816: ldr r1,[0x000e6b28]
  000e6818: add r1,pc
  000e681a: add r0,sp,#0x144
  000e681c: movs r2,#0x0
  000e681e: blx 0x0006ee18
  000e6822: str r0,[sp,#0x0]
  000e6824: add r0,sp,#0x8c
  000e6826: add r2,sp,#0x15c
  000e6828: add r3,sp,#0x150
  000e682a: mov r1,r5
  000e682c: blx 0x000739b4
  000e6830: add r0,sp,#0xc8
  000e6832: add r1,sp,#0x8c
  000e6834: blx 0x0006f2b0
  000e6838: ldr r0,[0x000e6b2c]
  000e683a: add r0,pc
  000e683c: ldr r4,[r0,#0x0]
  000e683e: add r0,sp,#0x8c
  000e6840: blx r4
  000e6842: add r0,sp,#0x144
  000e6844: blx r4
  000e6846: add r0,sp,#0x150
  000e6848: blx r4
  000e684a: add r0,sp,#0x15c
  000e684c: blx r4
  000e684e: ldr.w r0,[r8,#0x70]
  000e6852: add r1,sp,#0xc8
  000e6854: movs r2,#0x1
  000e6856: movs r4,#0x1
  000e6858: blx 0x000746f8
  000e685c: movs r0,#0x0
  000e685e: strb.w r4,[r8,#0x63]
  000e6862: strb.w r4,[r8,#0x6b]
  000e6866: strb.w r0,[r8,#0x101]
  000e686a: add r0,sp,#0xc8
  000e686c: blx 0x0006ee30
  000e6870: ldr.w r0,[r9,#0x0]
  000e6874: blx 0x00071c14
  000e6878: blx 0x00071824
  000e687c: mov r5,r0
  000e687e: ldr.w r0,[r9,#0x0]
  000e6882: blx 0x0007285c
  000e6886: mov r10,r0
  000e6888: ldr.w r0,[r9,#0x0]
  000e688c: blx 0x00071770
  000e6890: cmp r0,#0x14
  000e6892: bne 0x000e68fc
  000e6894: mov r0,r10
  000e6896: blx 0x00073738
  000e689a: cmp r0,r5
  000e689c: bne 0x000e68fc
  000e689e: ldr.w r0,[r9,#0x0]
  000e68a2: blx 0x00071c14
  000e68a6: blx 0x00071830
  000e68aa: mov r6,r0
  000e68ac: cbz r0,0x000e68d4
  000e68ae: movs r4,#0x0
  000e68b0: b 0x000e68ce
  000e68b2: ldr r0,[r6,#0x4]
  000e68b4: ldr.w r0,[r0,r4,lsl #0x2]
  000e68b8: blx 0x000718d8
  000e68bc: cmp r0,#0x29
  000e68be: bne 0x000e68cc
  000e68c0: ldr r0,[r6,#0x4]
  000e68c2: movs r1,#0x0
  000e68c4: ldr.w r0,[r0,r4,lsl #0x2]
  000e68c8: blx 0x00073864
  000e68cc: adds r4,#0x1
  000e68ce: ldr r0,[r6,#0x0]
  000e68d0: cmp r4,r0
  000e68d2: bcc 0x000e68b2
  000e68d4: ldr.w r0,[r9,#0x0]
  000e68d8: blx 0x00071c14
  000e68dc: mov r6,r0
  000e68de: ldr r0,[0x000e6b30]
  000e68e0: movs r1,#0xa
  000e68e2: movs r2,#0x0
  000e68e4: add r0,pc
  000e68e6: ldr r0,[r0,#0x0]
  000e68e8: ldr r0,[r0,#0x0]
  000e68ea: ldr r0,[r0,#0x4]
  000e68ec: ldr.w r0,[r0,#0xa4]
  000e68f0: blx 0x0007486c
  000e68f4: mov r1,r0
  000e68f6: mov r0,r6
  000e68f8: blx 0x00074fe0
  000e68fc: ldr.w r0,[r9,#0x0]
  000e6900: blx 0x00071770
  000e6904: cmp r0,#0x1b
  000e6906: bne 0x000e6920
  000e6908: mov r0,r10
  000e690a: blx 0x00073738
  000e690e: cmp r0,r5
  000e6910: bne 0x000e6920
  000e6912: ldr.w r0,[r9,#0x0]
  000e6916: blx 0x00071a58
  000e691a: movs r1,#0x83
  000e691c: blx 0x00073894
  000e6920: ldr.w r0,[r9,#0x0]
  000e6924: blx 0x00071770
  000e6928: cmp r0,#0x2b
  000e692a: bne 0x000e6934
  000e692c: cmp r5,#0xa
  000e692e: mov r10,r11
  000e6930: beq 0x000e6946
  000e6932: b 0x000e694c
  000e6934: ldr.w r0,[r9,#0x0]
  000e6938: blx 0x00071770
  000e693c: cmp r5,#0xa
  000e693e: mov r10,r11
  000e6940: bne 0x000e694c
  000e6942: cmp r0,#0x2c
  000e6944: bne 0x000e694c
  000e6946: movs r0,#0x1
  000e6948: strb.w r0,[r8,#0xd9]
  000e694c: ldr.w r0,[r9,#0x0]
  000e6950: blx 0x00073c48
  000e6954: cmp r5,#0xa
  000e6956: bne 0x000e69c8
  000e6958: cbz r0,0x000e69c8
  000e695a: ldr.w r0,[r9,#0x0]
  000e695e: blx 0x00071a58
  000e6962: movs r1,#0x55
  000e6964: movs r2,#0x1
  000e6966: blx 0x00073978
  000e696a: cbnz r0,0x000e69c4
  000e696c: ldr.w r0,[r9,#0x0]
  000e6970: blx 0x00071a58
  000e6974: movs r1,#0x55
  000e6976: movs r2,#0x1
  000e6978: blx 0x00071a70
  000e697c: cbnz r0,0x000e69c4
  000e697e: ldr.w r0,[r9,#0x0]
  000e6982: blx 0x00071c14
  000e6986: movs r1,#0x55
  000e6988: blx 0x00074fec
  000e698c: cbnz r0,0x000e69c4
  000e698e: ldr.w r0,[r9,#0x0]
  000e6992: blx 0x00071c14
  000e6996: movs r1,#0xa4
  000e6998: blx 0x00074fec
  000e699c: cbnz r0,0x000e69c4
  000e699e: ldr.w r0,[r9,#0x0]
  000e69a2: blx 0x00071c14
  000e69a6: mov r5,r0
  000e69a8: ldr r0,[0x000e6b34]
  000e69aa: movs r1,#0x32
  000e69ac: add r0,pc
  000e69ae: ldr r0,[r0,#0x0]
  000e69b0: ldr r0,[r0,#0x0]
  000e69b2: ldr r0,[r0,#0x4]
  000e69b4: ldr.w r0,[r0,#0x290]
  000e69b8: blx 0x000718c0
  000e69bc: mov r1,r0
  000e69be: mov r0,r5
  000e69c0: blx 0x00074fe0
  000e69c4: movs r4,#0x0
  000e69c6: b 0x000e69dc
  000e69c8: sub.w r0,r5,#0x64
  000e69cc: clz r1,r0
  000e69d0: cmp r0,#0x2
  000e69d2: lsr.w r4,r1,#0x5
  000e69d6: bcc 0x000e69dc
  000e69d8: cmp r5,#0xa
  000e69da: bne 0x000e6a30
  000e69dc: ldr.w r0,[r9,#0x0]
  000e69e0: blx 0x00071a58
  000e69e4: movs r1,#0x7a
  000e69e6: blx 0x00072424
  000e69ea: cbz r0,0x000e69f4
  000e69ec: blx 0x0007183c
  000e69f0: mov r5,r0
  000e69f2: b 0x000e69f6
  000e69f4: movs r5,#0x0
  000e69f6: ldr.w r0,[r9,#0x0]
  000e69fa: blx 0x00071c14
  000e69fe: movs r1,#0x7a
  000e6a00: blx 0x00074fec
  000e6a04: cmp r5,#0x5
  000e6a06: bgt 0x000e6a30
  000e6a08: cbnz r0,0x000e6a30
  000e6a0a: ldr.w r0,[r9,#0x0]
  000e6a0e: blx 0x00071c14
  000e6a12: mov r5,r0
  000e6a14: ldr r0,[0x000e6b38]
  000e6a16: movs r1,#0xa
  000e6a18: add r0,pc
  000e6a1a: ldr r0,[r0,#0x0]
  000e6a1c: ldr r0,[r0,#0x0]
  000e6a1e: ldr r0,[r0,#0x4]
  000e6a20: ldr.w r0,[r0,#0x1e8]
  000e6a24: blx 0x000718c0
  000e6a28: mov r1,r0
  000e6a2a: mov r0,r5
  000e6a2c: blx 0x00074fe0
  000e6a30: ldr.w r0,[r9,#0x0]
  000e6a34: blx 0x00071c14
  000e6a38: blx 0x00071824
  000e6a3c: cmp r0,#0xa
  000e6a3e: bne 0x000e6ab2
  000e6a40: ldr r0,[0x000e6b3c]
  000e6a42: add r0,pc
  000e6a44: ldr r0,[r0,#0x0]
  000e6a46: ldr r0,[r0,#0x0]
  000e6a48: blx 0x00071a1c
  000e6a4c: cbz r0,0x000e6ab2
  000e6a4e: ldr.w r0,[r9,#0x0]
  000e6a52: blx 0x00071c14
  000e6a56: blx 0x00073708
  000e6a5a: mov r5,r0
  000e6a5c: cbz r0,0x000e6a78
  000e6a5e: movs r6,#0x0
  000e6a60: ldr r0,[r5,#0x0]
  000e6a62: cmp r6,r0
  000e6a64: bcs 0x000e6a78
  000e6a66: ldr r0,[r5,#0x4]
  000e6a68: ldr.w r0,[r0,r6,lsl #0x2]
  000e6a6c: blx 0x000719c8
  000e6a70: adds r6,#0x1
  000e6a72: cmp r0,#0x8
  000e6a74: bne 0x000e6a60
  000e6a76: b 0x000e6ab2
  000e6a78: movs r0,#0x1
  000e6a7a: blx 0x0006eb24
  000e6a7e: mov r5,r0
  000e6a80: blx 0x000736cc
  000e6a84: ldr.w r0,[r9,#0x0]
  000e6a88: blx 0x00071c14
  000e6a8c: mov r6,r0
  000e6a8e: ldr.w r0,[r9,#0x0]
  000e6a92: blx 0x00071c14
  000e6a96: mov r1,r0
  000e6a98: mov r0,r5
  000e6a9a: blx 0x000736f0
  000e6a9e: mov r1,r0
  000e6aa0: mov r0,r6
  000e6aa2: movs r2,#0x0
  000e6aa4: blx 0x000736fc
  000e6aa8: mov r0,r5
  000e6aaa: blx 0x000740d4
  000e6aae: blx 0x0006eb48
  000e6ab2: cmp r4,#0x0
  000e6ab4: beq.w 0x000e6bb6
  000e6ab8: ldr.w r0,[r9,#0x0]
  000e6abc: blx 0x000719a4
  000e6ac0: cbnz r0,0x000e6afe
  000e6ac2: ldr.w r0,[r9,#0x0]
  000e6ac6: blx 0x00071770
  000e6aca: cmp r0,#0x50
  000e6acc: beq 0x000e6afe
  000e6ace: ldr.w r0,[r9,#0x0]
  000e6ad2: blx 0x00071770
  000e6ad6: cmp r0,#0x51
  000e6ad8: beq 0x000e6afe
  000e6ada: ldr.w r0,[r9,#0x0]
  000e6ade: blx 0x00071770
  000e6ae2: cmp r0,#0x52
  000e6ae4: beq 0x000e6afe
  000e6ae6: ldr.w r0,[r9,#0x0]
  000e6aea: blx 0x00071770
  000e6aee: cmp r0,#0x53
  000e6af0: beq 0x000e6afe
  000e6af2: ldr.w r0,[r9,#0x0]
  000e6af6: blx 0x00071770
  000e6afa: cmp r0,#0x54
  000e6afc: bne 0x000e6bb6
  000e6afe: ldr.w r0,[r9,#0x0]
  000e6b02: blx 0x00071c14
  000e6b06: movs r1,#0x25
  000e6b08: mov r5,r0
  000e6b0a: blx 0x00071af4
  000e6b0e: cbz r0,0x000e6b66
  000e6b10: mov r0,r5
  000e6b12: blx 0x00073708
  000e6b16: mov r6,r0
  000e6b18: movs r4,#0x0
  000e6b1a: b 0x000e6b50
  000e6b40: ldr r0,[r6,#0x4]
  000e6b42: ldr.w r0,[r0,r4,lsl #0x2]
  000e6b46: blx 0x000719c8
  000e6b4a: cmp r0,#0x25
  000e6b4c: beq 0x000e6b5a
  000e6b4e: adds r4,#0x1
  000e6b50: ldr r0,[r6,#0x0]
  000e6b52: cmp r4,r0
  000e6b54: bcc 0x000e6b40
  000e6b56: movs r1,#0x0
  000e6b58: b 0x000e6b60
  000e6b5a: ldr r0,[r6,#0x4]
  000e6b5c: ldr.w r1,[r0,r4,lsl #0x2]
  000e6b60: mov r0,r5
  000e6b62: blx 0x00074ff8
  000e6b66: mov r0,r5
  000e6b68: movs r1,#0x26
  000e6b6a: blx 0x00071af4
  000e6b6e: cbnz r0,0x000e6b8e
  000e6b70: ldr r0,[0x000e6f1c]
  000e6b72: mov.w r1,#0xffffffff
  000e6b76: add r0,pc
  000e6b78: ldr r0,[r0,#0x0]
  000e6b7a: ldr r0,[r0,#0x0]
  000e6b7c: ldr r0,[r0,#0x4]
  000e6b7e: ldr.w r0,[r0,#0x98]
  000e6b82: blx 0x000719d4
  000e6b86: mov r1,r0
  000e6b88: mov r0,r5
  000e6b8a: blx 0x00075004
  000e6b8e: mov r0,r5
  000e6b90: movs r1,#0x28
  000e6b92: blx 0x00071af4
  000e6b96: cbnz r0,0x000e6bb6
  000e6b98: ldr r0,[0x000e6f20]
  000e6b9a: mov.w r1,#0xffffffff
  000e6b9e: add r0,pc
  000e6ba0: ldr r0,[r0,#0x0]
  000e6ba2: ldr r0,[r0,#0x0]
  000e6ba4: ldr r0,[r0,#0x4]
  000e6ba6: ldr.w r0,[r0,#0xa0]
  000e6baa: blx 0x000719d4
  000e6bae: mov r1,r0
  000e6bb0: mov r0,r5
  000e6bb2: blx 0x00075004
  000e6bb6: ldr.w r0,[r9,#0x0]
  000e6bba: blx 0x00071c14
  000e6bbe: blx 0x00071824
  000e6bc2: mov r1,r0
  000e6bc4: ldr.w r0,[r9,#0x0]
  000e6bc8: cmp r1,#0x6c
  000e6bca: itt eq
  000e6bcc: ldr.eq.w r1,[r0,#0x114]
  000e6bd0: cmp.eq r1,#0x1
  000e6bd2: beq 0x000e6c32
  000e6bd4: blx 0x00071c14
  000e6bd8: blx 0x00071824
  000e6bdc: cmp r0,#0x6c
  000e6bde: bne.w 0x000e6ce2
  000e6be2: ldr.w r0,[r9,#0x0]
  000e6be6: ldr.w r1,[r0,#0x114]
  000e6bea: cmp r1,#0x2
  000e6bec: bne 0x000e6ce2
  000e6bee: blx 0x000733d8
  000e6bf2: ldr r1,[0x000e6f24]
  000e6bf4: cmp r0,r1
  000e6bf6: blt 0x000e6cc0
  000e6bf8: ldr.w r0,[r9,#0x0]
  000e6bfc: blx 0x00071a58
  000e6c00: movs r1,#0x6d
  000e6c02: movs r2,#0x32
  000e6c04: blx 0x00071a70
  000e6c08: cmp r0,#0x0
  000e6c0a: beq 0x000e6cc0
  000e6c0c: ldr r0,[0x000e6f28]
  000e6c0e: movw r1,#0x1dd
  000e6c12: ldr.w r5,[r8,#0x70]
  000e6c16: add r0,pc
  000e6c18: ldr r0,[r0,#0x0]
  000e6c1a: ldr r0,[r0,#0x0]
  000e6c1c: blx 0x00072f70
  000e6c20: mov r1,r0
  000e6c22: mov r0,r5
  000e6c24: movs r2,#0x1
  000e6c26: movs r4,#0x1
  000e6c28: blx 0x000746f8
  000e6c2c: strb.w r4,[r8,#0x6c]
  000e6c30: b 0x000e6cdc
  000e6c32: ldr.w r0,[r8,#0x84]
  000e6c36: cbz r0,0x000e6c40
  000e6c38: blx 0x00075010
  000e6c3c: blx 0x0006eb48
  000e6c40: movs r0,#0x0
  000e6c42: str.w r0,[r8,#0x84]
  000e6c46: movs r0,#0x78
  000e6c48: blx 0x0006eb24
  000e6c4c: mov r5,r0
  000e6c4e: ldr r0,[0x000e6fc4]
  000e6c50: add r0,pc
  000e6c52: ldr r0,[r0,#0x0]
  000e6c54: ldr r0,[r0,#0x0]
  000e6c56: movw r1,#0x641
  000e6c5a: blx 0x00072f70
  000e6c5e: mov r1,r0
  000e6c60: add r0,sp,#0x138
  000e6c62: movs r2,#0x0
  000e6c64: movs r4,#0x0
  000e6c66: blx 0x0006f028
  000e6c6a: ldr r0,[0x000e6fd8]
  000e6c6c: add r0,pc
  000e6c6e: movs r2,#0x4
  000e6c70: add.w r3,r0,#0x14
  000e6c74: strd r2,r4,[sp,#0x0]
  000e6c78: movs r1,#0x6c
  000e6c7a: add r2,sp,#0x138
  000e6c7c: strd r1,r4,[sp,#0x8]
  000e6c80: mov r0,r5
  000e6c82: mov.w r1,#0xffffffff
  000e6c86: blx 0x00071c80
  000e6c8a: add r0,sp,#0x138
  000e6c8c: blx 0x0006ee30
  000e6c90: mov r0,r5
  000e6c92: movs r1,#0x1
  000e6c94: movs r4,#0x1
  000e6c96: blx 0x00073984
  000e6c9a: ldr.w r0,[r9,#0x0]
  000e6c9e: movs r1,#0x2
  000e6ca0: str.w r1,[r0,#0x114]
  000e6ca4: movs r0,#0x74
  000e6ca6: blx 0x0006eb24
  000e6caa: mov r6,r0
  000e6cac: mov r1,r5
  000e6cae: movs r2,#0x0
  000e6cb0: movs r3,#0x0
  000e6cb2: blx 0x0007501c
  000e6cb6: strb.w r4,[r8,#0x69]
  000e6cba: str.w r6,[r8,#0x84]
  000e6cbe: b 0x000e6ce2
  000e6cc0: ldr r0,[0x000e6fdc]
  000e6cc2: mov.w r1,#0x1dc
  000e6cc6: ldr.w r5,[r8,#0x70]
  000e6cca: add r0,pc
  000e6ccc: ldr r0,[r0,#0x0]
  000e6cce: ldr r0,[r0,#0x0]
  000e6cd0: blx 0x00072f70
  000e6cd4: mov r1,r0
  000e6cd6: mov r0,r5
  000e6cd8: blx 0x00074794
  000e6cdc: movs r0,#0x1
  000e6cde: strb.w r0,[r8,#0x63]
  000e6ce2: ldr.w r0,[r9,#0x0]
  000e6ce6: blx 0x00073444
  000e6cea: mov r5,r0
  000e6cec: cbz r0,0x000e6d44
  000e6cee: mov r0,r5
  000e6cf0: blx 0x00072874
  000e6cf4: cmp r0,#0xe
  000e6cf6: bne 0x000e6d44
  000e6cf8: mov r0,r5
  000e6cfa: blx 0x00073738
  000e6cfe: mov r5,r0
  000e6d00: ldr.w r0,[r9,#0x0]
  000e6d04: blx 0x00071c14
  000e6d08: blx 0x00071824
  000e6d0c: cmp r5,r0
  000e6d0e: bne 0x000e6d44
  000e6d10: ldr.w r0,[r9,#0x0]
  000e6d14: blx 0x00071c14
  000e6d18: movs r1,#0x73
  000e6d1a: blx 0x00074fec
  000e6d1e: cbnz r0,0x000e6d44
  000e6d20: ldr r0,[0x000e6fe0]
  000e6d22: movs r1,#0x1
  000e6d24: add r0,pc
  000e6d26: ldr r0,[r0,#0x0]
  000e6d28: ldr r0,[r0,#0x0]
  000e6d2a: ldr r0,[r0,#0x4]
  000e6d2c: ldr.w r0,[r0,#0x1cc]
  000e6d30: blx 0x000718c0
  000e6d34: mov r5,r0
  000e6d36: ldr.w r0,[r9,#0x0]
  000e6d3a: blx 0x00071c14
  000e6d3e: mov r1,r5
  000e6d40: blx 0x00074fe0
  000e6d44: ldr.w r0,[r9,#0x0]
  000e6d48: blx 0x00071770
  000e6d4c: ldr r1,[0x000e6fe4]
  000e6d4e: cmp r0,#0xa2
  000e6d50: add r1,pc
  000e6d52: ldr r6,[r1,#0x0]
  000e6d54: itt eq
  000e6d56: ldrb.eq.w r0,[r6,#0x2b]
  000e6d5a: cmp.eq r0,#0x0
  000e6d5c: bne 0x000e6d84
  000e6d5e: ldr r0,[0x000e6fe8]
  000e6d60: movw r1,#0x265
  000e6d64: ldr.w r5,[r8,#0x70]
  000e6d68: add r0,pc
  000e6d6a: ldr r0,[r0,#0x0]
  000e6d6c: ldr r0,[r0,#0x0]
  000e6d6e: blx 0x00072f70
  000e6d72: mov r1,r0
  000e6d74: mov r0,r5
  000e6d76: blx 0x00074794
  000e6d7a: movs r0,#0x1
  000e6d7c: strb.w r0,[r8,#0x63]
  000e6d80: strb.w r0,[r6,#0x2b]
  000e6d84: ldr.w r0,[r9,#0x0]
  000e6d88: blx 0x00073c48
  000e6d8c: ldr r1,[0x000e6fec]
  000e6d8e: cmp r0,#0x0
  000e6d90: add r1,pc
  000e6d92: ldr r4,[r1,#0x0]
  000e6d94: beq 0x000e6dc8
  000e6d96: ldrb.w r0,[r4,#0x35]
  000e6d9a: ldrb.w r1,[r6,#0x38]
  000e6d9e: orrs r0,r1
  000e6da0: lsls r0,r0,#0x18
  000e6da2: bne 0x000e6dc8
  000e6da4: ldr r0,[0x000e6ff0]
  000e6da6: movs r1,#0x6e
  000e6da8: ldr.w r5,[r8,#0x70]
  000e6dac: add r0,pc
  000e6dae: ldr r0,[r0,#0x0]
  000e6db0: ldr r0,[r0,#0x0]
  000e6db2: blx 0x00072f70
  000e6db6: mov r1,r0
  000e6db8: mov r0,r5
  000e6dba: blx 0x00074794
  000e6dbe: movs r0,#0x1
  000e6dc0: strb.w r0,[r8,#0x63]
  000e6dc4: strb.w r0,[r6,#0x38]
  000e6dc8: ldr.w r0,[r9,#0x0]
  000e6dcc: blx 0x000719a4
  000e6dd0: cbz r0,0x000e6e08
  000e6dd2: ldrb.w r0,[r4,#0x37]
  000e6dd6: ldrb.w r1,[r6,#0x39]
  000e6dda: orrs r0,r1
  000e6ddc: lsls r0,r0,#0x18
  000e6dde: bne 0x000e6e08
  000e6de0: ldr r0,[0x000e6ff4]
  000e6de2: movs r1,#0x6f
  000e6de4: ldr.w r5,[r8,#0x70]
  000e6de8: add r0,pc
  000e6dea: ldr r0,[r0,#0x0]
  000e6dec: ldr r0,[r0,#0x0]
  000e6dee: blx 0x00072f70
  000e6df2: mov r1,r0
  000e6df4: mov r0,r5
  000e6df6: blx 0x00074794
  000e6dfa: movs r0,#0x1
  000e6dfc: mov r5,r8
  000e6dfe: strb.w r0,[r5,#0x63]!
  000e6e02: strb.w r0,[r6,#0x39]
  000e6e06: b 0x000e6e0c
  000e6e08: add.w r5,r8,#0x63
  000e6e0c: ldr.w r0,[r9,#0x0]
  000e6e10: blx 0x00075028
  000e6e14: mov r6,r0
  000e6e16: cmp r0,#0x1
  000e6e18: blt 0x000e6eca
  000e6e1a: ldrb r0,[r5,#0x0]
  000e6e1c: cmp r0,#0x0
  000e6e1e: bne 0x000e6eca
  000e6e20: ldr r0,[0x000e6ff8]
  000e6e22: cmp r6,#0x1
  000e6e24: add r0,pc
  000e6e26: ldr r0,[r0,#0x0]
  000e6e28: bne 0x000e6e42
  000e6e2a: ldr r0,[r0,#0x0]
  000e6e2c: movw r1,#0xc9e
  000e6e30: ldr.w r6,[r8,#0x70]
  000e6e34: blx 0x00072f70
  000e6e38: mov r1,r0
  000e6e3a: mov r0,r6
  000e6e3c: blx 0x00074794
  000e6e40: b 0x000e6ec6
  000e6e42: ldr r0,[r0,#0x0]
  000e6e44: movw r1,#0xc9f
  000e6e48: blx 0x00072f70
  000e6e4c: add.w r11,sp,#0xc8
  000e6e50: mov r1,r0
  000e6e52: movs r2,#0x0
  000e6e54: mov r0,r11
  000e6e56: blx 0x0006f028
  000e6e5a: ldr.w r4,[r9,#0x0]
  000e6e5e: add r0,sp,#0x12c
  000e6e60: mov r1,r11
  000e6e62: movs r2,#0x0
  000e6e64: blx 0x0006f028
  000e6e68: add r0,sp,#0x114
  000e6e6a: mov r1,r6
  000e6e6c: blx 0x0006f658
  000e6e70: add r0,sp,#0x120
  000e6e72: add r1,sp,#0x114
  000e6e74: movs r2,#0x0
  000e6e76: blx 0x0006f028
  000e6e7a: ldr r1,[0x000e6ffc]
  000e6e7c: add r1,pc
  000e6e7e: add r0,sp,#0x108
  000e6e80: movs r2,#0x0
  000e6e82: blx 0x0006ee18
  000e6e86: str r0,[sp,#0x0]
  000e6e88: add r0,sp,#0x8c
  000e6e8a: add r2,sp,#0x12c
  000e6e8c: add r3,sp,#0x120
  000e6e8e: mov r1,r4
  000e6e90: blx 0x000739b4
  000e6e94: add r0,sp,#0xc8
  000e6e96: add r1,sp,#0x8c
  000e6e98: blx 0x0006f2b0
  000e6e9c: ldr r0,[0x000e7000]
  000e6e9e: add r0,pc
  000e6ea0: ldr r4,[r0,#0x0]
  000e6ea2: add r0,sp,#0x8c
  000e6ea4: blx r4
  000e6ea6: add r0,sp,#0x108
  000e6ea8: blx r4
  000e6eaa: add r0,sp,#0x120
  000e6eac: blx r4
  000e6eae: add r0,sp,#0x114
  000e6eb0: blx r4
  000e6eb2: add r0,sp,#0x12c
  000e6eb4: blx r4
  000e6eb6: ldr.w r0,[r8,#0x70]
  000e6eba: add r1,sp,#0xc8
  000e6ebc: blx 0x00074794
  000e6ec0: add r0,sp,#0xc8
  000e6ec2: blx 0x0006ee30
  000e6ec6: movs r0,#0x1
  000e6ec8: strb r0,[r5,#0x0]
  000e6eca: movs r0,#0x14
  000e6ecc: str.w r0,[r8,#0xc]
  000e6ed0: ldr r1,[sp,#0x174]
  000e6ed2: ldr.w r2,[r10,#0x0]
  000e6ed6: subs r1,r2,r1
  000e6ed8: itttt eq
  000e6eda: add.eq sp,#0x178
  000e6edc: vpop.eq {d8,d9,d10,d11}
  000e6ee0: add.eq sp,#0x4
  000e6ee2: pop.eq.w {r8,r9,r10,r11}
  000e6ee6: it eq
  000e6ee8: pop.eq {r4,r5,r6,r7,pc}
  000e6eea: blx 0x0006e824
```
