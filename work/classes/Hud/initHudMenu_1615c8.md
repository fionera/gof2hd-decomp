# Hud::initHudMenu
elf 0x1615c8 body 2976
Sig: undefined __stdcall initHudMenu(int param_1, Level * param_2)

## decompile
```c

/* Hud::initHudMenu(int, Level*) */

void Hud::initHudMenu(int param_1,Level *param_2)

{
  int *piVar1;
  char cVar2;
  void *pvVar3;
  Ship *this;
  undefined4 uVar4;
  undefined4 *puVar5;
  int *piVar6;
  int iVar7;
  Item *this_00;
  char *pcVar8;
  char *pcVar9;
  int *piVar10;
  String *pSVar11;
  int *piVar12;
  int *piVar13;
  Station *this_01;
  PlayerEgo *pPVar14;
  Route *this_02;
  int iVar15;
  uint *puVar16;
  ushort uVar17;
  char *pcVar18;
  uint uVar19;
  int in_r2;
  int iVar20;
  float *pfVar21;
  uint uVar22;
  code *pcVar23;
  TouchButton *pTVar24;
  int *piVar25;
  int *piVar26;
  uint in_fpscr;
  float fVar27;
  float fVar28;
  int *local_ac;
  int *local_a8;
  int local_94;
  undefined8 local_90;
  undefined8 uStack_88;
  String aSStack_80 [12];
  String aSStack_74 [12];
  String aSStack_68 [12];
  String aSStack_5c [12];
  String aSStack_50 [12];
  float local_44;
  float local_40;
  int local_3c;
  undefined4 local_38;
  int local_34;
  
  piVar26 = *(int **)(DAT_0017176c + 0x1715ec);
  local_34 = *piVar26;
  piVar1 = (int *)(param_1 + 0x18);
  if ((Array *)*piVar1 != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>((Array *)*piVar1);
    if ((Array<TouchButton*> *)*piVar1 != (Array<TouchButton*> *)0x0) {
      pvVar3 = (void *)Array<TouchButton*>::~Array((Array<TouchButton*> *)*piVar1);
      operator_delete(pvVar3);
    }
    *piVar1 = 0;
  }
  pvVar3 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(Level **)(param_1 + 0x238) = param_2;
  *(void **)(param_1 + 0x18) = pvVar3;
  if (*(Array<Item*> **)(param_1 + 0x25c) != (Array<Item*> *)0x0) {
    pvVar3 = (void *)Array<Item*>::~Array(*(Array<Item*> **)(param_1 + 0x25c));
    operator_delete(pvVar3);
  }
  iVar7 = DAT_00171770;
  *(undefined4 *)(param_1 + 0x25c) = 0;
  piVar25 = *(int **)(iVar7 + 0x171644);
  this = (Ship *)Status::getShip();
  uVar4 = Ship::getEquipment(this,1);
  *(undefined4 *)(param_1 + 0x25c) = uVar4;
  updateSecondaryWeaponString((Hud *)param_1);
  iVar7 = DAT_00171778;
  puVar5 = (undefined4 *)(DAT_00171774 + 0x171666);
  *(undefined4 *)(param_1 + 0x4cc) = 0;
  piVar6 = (int *)*puVar5;
  pcVar18 = *(char **)(iVar7 + 0x17166c);
  iVar7 = *piVar6;
  cVar2 = *pcVar18;
  iVar20 = *(int *)(iVar7 + 0x1dc);
  if (cVar2 == '\0') {
    local_94 = *(int *)(param_1 + 0x3d8);
  }
  else {
    if (*(int *)(param_1 + 0x238) == 3) {
      fVar27 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(DAT_0017177c + 0x17168c) + 0x54),
                                          (byte)(in_fpscr >> 0x16) & 3);
    }
    else {
      fVar27 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(DAT_0017177c + 0x17168c) + 0x58),
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar28 = DAT_00171784;
      if (**(char **)(DAT_00171780 + 0x1716ac) == '\0') {
        pfVar21 = (float *)&DAT_001722d0;
        if (**(char **)(DAT_00171788 + 0x1716c4) == '\0') {
          pfVar21 = (float *)&DAT_001722d4;
        }
        fVar28 = *pfVar21;
      }
      fVar27 = fVar27 - fVar28;
    }
    in_fpscr = in_fpscr & 0xfffffff;
    fVar28 = DAT_0017178c;
    if (0.0 <= fVar27) {
      if (*(int *)(param_1 + 0x238) == 3) {
        fVar28 = (float)VectorSignedToFloat(*(undefined4 *)
                                             (*(int *)(DAT_00171790 + 0x1716f0) + 0x54),
                                            (byte)(in_fpscr >> 0x16) & 3);
      }
      else {
        fVar27 = (float)VectorSignedToFloat(*(undefined4 *)
                                             (*(int *)(DAT_00171794 + 0x171702) + 0x58),
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar28 = DAT_00171784;
        if (**(char **)(DAT_00171798 + 0x171704) == '\0') {
          pfVar21 = (float *)&DAT_001722d0;
          if (**(char **)(DAT_0017179c + 0x171722) == '\0') {
            pfVar21 = (float *)&DAT_001722d4;
          }
          fVar28 = *pfVar21;
        }
        fVar28 = fVar27 - fVar28;
      }
    }
    *(int *)(param_1 + 0x3c8) = (int)fVar28;
    local_94 = ((*(int *)(param_1 + 0x3cc) + (int)fVar28) - *(int *)(iVar7 + 0x30) / 2) + 1;
    *(int *)(param_1 + 0x3d8) = local_94;
  }
  switch(param_2) {
  case (Level *)0x0:
    puVar16 = *(uint **)(param_1 + 0x25c);
    if (puVar16 != (uint *)0x0) {
      uVar19 = 0;
      do {
        if (*puVar16 <= uVar19) goto LAB_00171816;
        iVar7 = uVar19 * 4;
        uVar19 = uVar19 + 1;
      } while (*(int *)(puVar16[1] + iVar7) == 0);
      pTVar24 = operator_new(200);
      pSVar11 = (String *)GameText::getText(**(int **)(DAT_00171b14 + 0x1717c8));
      TouchButton::TouchButton
                (pTVar24,pSVar11,0,*(int *)(param_1 + 0x3d4),local_94,*(int *)(param_1 + 0x3dc),
                 '\x11','\x04');
      *(undefined4 *)pTVar24 = 0x200;
      *(undefined4 *)(pTVar24 + 4) = 0;
      ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
      local_94 = *(int *)(*piVar6 + 0x30) + iVar20 + local_94;
    }
LAB_00171816:
    iVar7 = Status::getWingmen();
    if (((iVar7 != 0) && (iVar7 = Status::inSupernovaSystem(), iVar7 == 0)) &&
       (iVar7 = Status::getCurrentCampaignMission(), iVar7 != 0x9e)) {
      pTVar24 = operator_new(200);
      pSVar11 = (String *)GameText::getText(**(int **)(DAT_00171b18 + 0x171844));
      TouchButton::TouchButton
                (pTVar24,pSVar11,0,*(int *)(param_1 + 0x3d4),local_94,*(int *)(param_1 + 0x3dc),
                 '\x11','\x04');
      *(undefined4 *)pTVar24 = 0x400;
      *(undefined4 *)(pTVar24 + 4) = 0;
      ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
      local_94 = *(int *)(*piVar6 + 0x30) + iVar20 + local_94;
    }
    Status::getShip();
    iVar7 = Ship::hasCloak();
    if (iVar7 != 0) {
      iVar7 = Status::getShip();
      this_00 = (Item *)Ship::getFirstEquipmentOfSort(iVar7);
      pTVar24 = operator_new(200);
      iVar7 = **(int **)(DAT_00171b1c + 0x1718c0);
      Item::getIndex(this_00);
      pSVar11 = (String *)GameText::getText(iVar7);
      TouchButton::TouchButton
                (pTVar24,pSVar11,0,*(int *)(param_1 + 0x3d4),local_94,*(int *)(param_1 + 0x3dc),
                 '\x11','\x04');
      TouchButton::setPressProgressHighlight(pTVar24,false);
      pPVar14 = (PlayerEgo *)Level::getPlayer();
      iVar7 = PlayerEgo::isCloaked(pPVar14);
      if (iVar7 == 0) {
        pPVar14 = (PlayerEgo *)Level::getPlayer();
        iVar7 = PlayerEgo::isChargingCloak(pPVar14);
        if (iVar7 != 0) goto LAB_00171920;
        pPVar14 = (PlayerEgo *)Level::getPlayer();
        iVar7 = PlayerEgo::isRechargingCloak(pPVar14);
        if (iVar7 != 0) goto LAB_00171920;
      }
      else {
LAB_00171920:
        pPVar14 = (PlayerEgo *)Level::getPlayer();
        iVar7 = PlayerEgo::isRechargingCloak(pPVar14);
        if (iVar7 != 0) {
          pPVar14 = (PlayerEgo *)Level::getPlayer();
          fVar28 = (float)PlayerEgo::getCloakRechargeRate(pPVar14);
          TouchButton::setPressProgress(pTVar24,fVar28);
        }
        TouchButton::setHalfTransparent(SUB41(pTVar24,0));
      }
      *(undefined4 *)pTVar24 = 0x800;
      *(undefined4 *)(pTVar24 + 4) = 0;
      ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
      local_94 = *(int *)(*piVar6 + 0x30) + iVar20 + local_94;
    }
    Status::getShip();
    iVar7 = Ship::hasJumpDrive();
    if (iVar7 != 0) {
      pTVar24 = operator_new(200);
      pSVar11 = (String *)GameText::getText(**(int **)(DAT_00171b20 + 0x171986));
      TouchButton::TouchButton
                (pTVar24,pSVar11,0,*(int *)(param_1 + 0x3d4),local_94,*(int *)(param_1 + 0x3dc),
                 '\x11','\x04');
      Level::getPlayer();
      iVar7 = PlayerEgo::isChargingDrive();
      if (iVar7 == 0) {
        pPVar14 = (PlayerEgo *)Level::getPlayer();
        iVar7 = PlayerEgo::emergencySystemActive(pPVar14);
        if (iVar7 != 0) goto LAB_001719cc;
      }
      else {
LAB_001719cc:
        TouchButton::setHalfTransparent(SUB41(pTVar24,0));
      }
      *(undefined4 *)pTVar24 = 0x1000;
      *(undefined4 *)(pTVar24 + 4) = 0;
      ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
    }
    iVar7 = Status::getShip();
    iVar7 = Ship::getCargo(iVar7);
    if (iVar7 == 0) {
      uVar4 = 0;
    }
    else {
      uVar4 = Item::getAmount();
    }
    iVar7 = DAT_00171f80;
    *(undefined4 *)(param_1 + 0x27c) = uVar4;
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)**(undefined4 **)(iVar7 + 0x171d38),0x4f5,(uint *)(param_1 + 0x35c));
    break;
  case (Level *)0x1:
    piVar25 = *(int **)(DAT_00171b2c + 0x171afe);
    pcVar8 = (char *)(DAT_00171b30 + 0x171b06);
    pcVar9 = (char *)(DAT_00171b34 + 0x171b0c);
    for (uVar19 = 0; puVar16 = *(uint **)(param_1 + 0x25c), uVar19 < *puVar16; uVar19 = uVar19 + 1)
    {
      if (*(int *)(puVar16[1] + uVar19 * 4) != 0) {
        pTVar24 = operator_new(200);
        iVar7 = *piVar25;
        Item::getIndex(*(Item **)(puVar16[1] + uVar19 * 4));
        pSVar11 = (String *)GameText::getText(iVar7);
        AbyssEngine::String::String(aSStack_68,pcVar8,false);
        iVar7 = Item::getAmount();
        AbyssEngine::String::String(aSStack_74,iVar7);
        AbyssEngine::operator+(aSStack_5c,aSStack_68);
        AbyssEngine::String::String(aSStack_80,pcVar9,false);
        AbyssEngine::operator+(aSStack_50,aSStack_5c);
        AbyssEngine::String::String((String *)&local_90,aSStack_50,false);
        AbyssEngine::operator+((String *)&local_44,pSVar11);
        TouchButton::TouchButton
                  (pTVar24,(String *)&local_44,0,*(int *)(param_1 + 0x3d4),local_94,
                   *(int *)(param_1 + 0x3dc),'\x11','\x04');
        pcVar23 = *(code **)(DAT_00171f70 + 0x171bee);
        (*pcVar23)(&local_44);
        (*pcVar23)((String *)&local_90);
        (*pcVar23)(aSStack_50);
        (*pcVar23)(aSStack_80);
        (*pcVar23)(aSStack_5c);
        (*pcVar23)(aSStack_74);
        (*pcVar23)(aSStack_68);
        if ((uVar19 & 0x7fffffff) == 0) {
          uVar4 = 0x2000;
        }
        else if ((uVar19 & 0x7fffffff) == 1) {
          uVar4 = 0x4000;
        }
        else {
          uVar4 = 0x10000;
          if (uVar19 == 2) {
            uVar4 = 0x8000;
          }
        }
        *(undefined4 *)pTVar24 = uVar4;
        *(undefined4 *)(pTVar24 + 4) = 0;
        ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
        local_94 = *(int *)(*piVar6 + 0x30) + iVar20 + local_94;
      }
    }
    uVar17 = 0x4f6;
    puVar5 = (undefined4 *)(DAT_00171f74 + 0x171c74);
    goto LAB_0017209e;
  case (Level *)0x2:
    local_40 = 4.316e-43;
    local_44 = 4.30199e-43;
    uVar19 = 0;
    local_3c = 0x135;
    local_38 = 0x137;
    if (*(char *)(*piVar25 + 0xf8) == '\0') {
      local_38 = 0x136;
    }
    local_90 = DAT_001722c0;
    uStack_88 = DAT_001722c8;
    piVar25 = *(int **)(DAT_00171b24 + 0x171a48);
    for (; uVar19 < 4; uVar19 = uVar19 + 1) {
      pTVar24 = operator_new(200);
      pSVar11 = (String *)GameText::getText(*piVar25);
      TouchButton::TouchButton
                (pTVar24,pSVar11,0,*(int *)(param_1 + 0x3d4),local_94,*(int *)(param_1 + 0x3dc),
                 '\x11','\x04');
      iVar7 = *(int *)((int)&local_90 + uVar19 * 4);
      *(int *)pTVar24 = iVar7;
      *(int *)(pTVar24 + 4) = iVar7 >> 0x1f;
      ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
      local_94 = *(int *)(*piVar6 + 0x30) + iVar20 + local_94;
    }
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)**(undefined4 **)(DAT_00171b28 + 0x171ab6),0x4f3,
               (uint *)(param_1 + 0x35c));
    break;
  case (Level *)0x3:
    if (cVar2 != '\0') {
      *(int *)(param_1 + 0x4cc) = *(int *)(iVar7 + 0x28) - *(int *)(param_1 + 0x3c4);
    }
    iVar7 = Status::inAlienOrbit();
    if (iVar7 == 0) {
      pTVar24 = operator_new(200);
      piVar10 = *(int **)(DAT_00171f78 + 0x171c82);
      pSVar11 = (String *)GameText::getText(*piVar10);
      TouchButton::TouchButton
                (pTVar24,pSVar11,0,*(int *)(param_1 + 0x3d4),local_94,*(int *)(param_1 + 0x3dc),
                 '\x11','\x04');
      piVar12 = (int *)(param_1 + 0x3dc);
      piVar13 = (int *)(param_1 + 0x3d4);
      *(undefined4 *)pTVar24 = 0x1000000;
      *(undefined4 *)(pTVar24 + 4) = 0;
      ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
      local_94 = local_94 + iVar20 + *(int *)(*piVar6 + 0x30);
      iVar7 = Status::inEmptyOrbit((Status *)*piVar25);
      if (iVar7 == 0) {
        pTVar24 = operator_new(200);
        Status::getStation();
        Station::getName();
        AbyssEngine::String::String((String *)&local_90,aSStack_50,false);
        this_01 = (Station *)Status::getStation();
        iVar7 = Station::getIndex(this_01);
        if (iVar7 != 0x65) {
          AbyssEngine::String::String(aSStack_68,(char *)(DAT_00171f84 + 0x171d4a),false);
          GameText::getText(*piVar10);
          AbyssEngine::operator+(aSStack_5c,aSStack_68);
        }
        else {
          AbyssEngine::String::String(aSStack_5c,(char *)(DAT_00171f7c + 0x171d20),false);
        }
        AbyssEngine::operator+((String *)&local_44,(String *)&local_90);
        TouchButton::TouchButton
                  (pTVar24,(String *)&local_44,0,*piVar13,local_94,*piVar12,'\x11','\x04');
        AbyssEngine::String::~String((String *)&local_44);
        AbyssEngine::String::~String(aSStack_5c);
        if (iVar7 != 0x65) {
          AbyssEngine::String::~String(aSStack_68);
        }
        AbyssEngine::String::~String((String *)&local_90);
        AbyssEngine::String::~String(aSStack_50);
        *(undefined4 *)pTVar24 = 0x800000;
        *(undefined4 *)(pTVar24 + 4) = 0;
        ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
        local_94 = iVar20 + local_94 + *(int *)(*piVar6 + 0x30);
      }
      Status::getSystem();
      iVar7 = SolarSystem::currentOrbitHasWarpGate();
      if (iVar7 != 0) {
        pTVar24 = operator_new(200);
        pSVar11 = (String *)GameText::getText(*piVar10);
        TouchButton::TouchButton(pTVar24,pSVar11,0,*piVar13,local_94,*piVar12,'\x11','\x04');
        *(undefined4 *)pTVar24 = 0x400000;
        *(undefined4 *)(pTVar24 + 4) = 0;
        ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
        local_94 = iVar20 + local_94 + *(int *)(*piVar6 + 0x30);
      }
      pPVar14 = (PlayerEgo *)Level::getPlayer();
      iVar7 = PlayerEgo::getRoute(pPVar14);
      if (iVar7 != 0) {
        pPVar14 = (PlayerEgo *)Level::getPlayer();
        this_02 = (Route *)PlayerEgo::getRoute(pPVar14);
        iVar7 = Route::getLastWaypoint(this_02);
        if (*(char *)(iVar7 + 0x130) == '\0') {
          pTVar24 = operator_new(200);
          pSVar11 = (String *)GameText::getText(*piVar10);
          TouchButton::TouchButton(pTVar24,pSVar11,0,*piVar13,local_94,*piVar12,'\x11','\x04');
          *(undefined4 *)pTVar24 = 0x2000000;
          *(undefined4 *)(pTVar24 + 4) = 0;
          ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
          local_94 = iVar20 + local_94 + *(int *)(*piVar6 + 0x30);
        }
      }
      if (**(int **)(DAT_00171f88 + 0x171eb4) != 0) {
        pTVar24 = operator_new(200);
        pSVar11 = (String *)GameText::getText(*piVar10);
        AbyssEngine::String::String(aSStack_50,(char *)(DAT_00171f8c + 0x171ed6),false);
        AbyssEngine::operator+((String *)&local_90,pSVar11);
        Station::getName();
        AbyssEngine::String::String(aSStack_5c,aSStack_74,false);
        AbyssEngine::operator+((String *)&local_44,(String *)&local_90);
        TouchButton::TouchButton
                  (pTVar24,(String *)&local_44,0,*piVar13,local_94,*piVar12,'\x11','\x04');
        pcVar23 = *(code **)(DAT_00171f90 + 0x171f28);
        (*pcVar23)(&local_44);
        (*pcVar23)(aSStack_5c);
        (*pcVar23)(aSStack_74);
        (*pcVar23)(&local_90);
        (*pcVar23)(aSStack_50);
        *(undefined4 *)pTVar24 = 0x200000;
        *(undefined4 *)(pTVar24 + 4) = 0;
        ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
        local_94 = *(int *)(*piVar6 + 0x30) + iVar20 + local_94;
      }
    }
    local_a8 = (int *)(param_1 + 0x3dc);
    local_ac = (int *)(param_1 + 0x3d4);
    iVar7 = Level::getNumDockingTargets();
    for (uVar19 = 0; (int)uVar19 < iVar7; uVar19 = uVar19 + 1) {
      Level::getDockingTarget(in_r2);
      PlayerFixedObject::getName();
      iVar15 = local_3c;
      AbyssEngine::String::~String((String *)&local_44);
      if (iVar15 != 0) {
        pTVar24 = operator_new(200);
        Level::getDockingTarget(in_r2);
        PlayerFixedObject::getName();
        TouchButton::TouchButton
                  (pTVar24,(String *)&local_44,0,*local_ac,local_94,*local_a8,'\x11','\x04');
        AbyssEngine::String::~String((String *)&local_44);
        iVar15 = 0x4000000 << (uVar19 & 0xff);
        *(int *)pTVar24 = iVar15;
        *(int *)(pTVar24 + 4) = iVar15 >> 0x1f;
        ArrayAdd<TouchButton*>(pTVar24,(Array *)*piVar1);
        local_94 = *(int *)(*piVar6 + 0x30) + iVar20 + local_94;
      }
    }
    if (*pcVar18 == '\0') {
      puVar16 = (uint *)*piVar1;
      uVar19 = *puVar16;
      if (4 < uVar19) {
        for (uVar22 = 0; uVar22 < uVar19; uVar22 = uVar22 + 1) {
          pTVar24 = *(TouchButton **)(puVar16[1] + uVar22 * 4);
          TouchButton::getPosition();
          fVar28 = local_44;
          TouchButton::getPosition();
          fVar27 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar6 + 0x30),
                                              (byte)(in_fpscr >> 0x16) & 3);
          TouchButton::setPosition(pTVar24,(int)fVar28,(int)(local_90._4_4_ - fVar27));
          puVar16 = (uint *)*piVar1;
          uVar19 = *puVar16;
        }
      }
    }
    uVar17 = 0x4f4;
    puVar5 = (undefined4 *)(DAT_001722d8 + 0x1720a0);
LAB_0017209e:
    AbyssEngine::PaintCanvas::Image2DCreate
              (*(PaintCanvas **)*puVar5,uVar17,(uint *)(param_1 + 0x35c));
  }
  puVar16 = (uint *)*piVar1;
  uVar19 = *puVar16;
  if (*pcVar18 == '\0') {
    if (uVar19 < 5) {
      iVar7 = 0;
    }
    else {
      iVar7 = -*(int *)(*piVar6 + 0x30);
    }
    *(int *)(param_1 + 0x4d0) = iVar7;
    iVar20 = *(int *)(DAT_001722e4 + 0x172172);
    iVar7 = *(int *)(DAT_001722e8 + 0x172178);
    for (uVar22 = 0; uVar22 < uVar19; uVar22 = uVar22 + 1) {
      if (uVar22 < 10) {
        TouchButton::getPosition();
        *(int *)(iVar20 + uVar22 * 4) = (int)local_44;
        TouchButton::getPosition();
        *(int *)(iVar7 + uVar22 * 4) = (int)local_40;
        uVar19 = *(uint *)*piVar1;
      }
    }
  }
  else if (uVar19 != 0) {
    uVar22 = 0;
    iVar7 = (4 - uVar19) * (iVar20 + *(int *)(*piVar6 + 0x30));
    if (param_2 == (Level *)0x3) {
      iVar7 = iVar7 - *(int *)(*piVar6 + 0x30);
    }
    *(int *)(param_1 + 0x4d0) = iVar7;
    iVar20 = *(int *)(DAT_001722dc + 0x1720e6);
    iVar7 = *(int *)(DAT_001722e0 + 0x1720ec);
    for (; uVar22 < uVar19; uVar22 = uVar22 + 1) {
      TouchButton::translate
                (*(TouchButton **)(puVar16[1] + uVar22 * 4),*(int *)(param_1 + 0x4cc),
                 *(int *)(param_1 + 0x4d0));
      if (uVar22 < 10) {
        TouchButton::getPosition();
        *(int *)(iVar20 + uVar22 * 4) = (int)local_44;
        TouchButton::getPosition();
        *(int *)(iVar7 + uVar22 * 4) = (int)local_40;
      }
      puVar16 = (uint *)*piVar1;
      uVar19 = *puVar16;
    }
  }
  *(undefined1 *)(param_1 + 0x282) = 1;
  if (*piVar26 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001715c8: push {r4,r5,r6,r7,lr}
  001715ca: add r7,sp,#0xc
  001715cc: push {r7,r8,r9,r10,r11}
  001715d0: vpush {d8}
  001715d4: sub sp,#0xa0
  001715d6: mov r4,sp
  001715d8: bfc r4,#0x0,#0x4
  001715dc: mov sp,r4
  001715de: str r2,[sp,#0x30]
  001715e0: mov r11,r0
  001715e2: ldr r0,[0x0017176c]
  001715e4: mov r10,r11
  001715e6: mov r4,r1
  001715e8: add r0,pc
  001715ea: ldr.w r9,[r0,#0x0]
  001715ee: ldr.w r0,[r9,#0x0]
  001715f2: str r0,[sp,#0x9c]
  001715f4: ldr.w r0,[r10,#0x18]!
  001715f8: cbz r0,0x00171612
  001715fa: blx 0x00075328
  001715fe: ldr.w r0,[r10,#0x0]
  00171602: cbz r0,0x0017160c
  00171604: blx 0x00075334
  00171608: blx 0x0006eb48
  0017160c: movs r0,#0x0
  0017160e: str.w r0,[r10,#0x0]
  00171612: movs r0,#0xc
  00171614: blx 0x0006eb24
  00171618: mov r5,r0
  0017161a: blx 0x00074f38
  0017161e: str.w r4,[r11,#0x238]
  00171622: ldr.w r0,[r11,#0x25c]
  00171626: str r4,[sp,#0x2c]
  00171628: cmp r0,#0x0
  0017162a: str.w r5,[r11,#0x18]
  0017162e: beq 0x00171638
  00171630: blx 0x0007342c
  00171634: blx 0x0006eb48
  00171638: ldr r0,[0x00171770]
  0017163a: movs r4,#0x0
  0017163c: str.w r4,[r11,#0x25c]
  00171640: add r0,pc
  00171642: ldr.w r8,[r0,#0x0]
  00171646: ldr.w r0,[r8,#0x0]
  0017164a: blx 0x00071a58
  0017164e: movs r1,#0x1
  00171650: blx 0x00072118
  00171654: str.w r0,[r11,#0x25c]
  00171658: mov r0,r11
  0017165a: blx 0x00077a28
  0017165e: ldr r0,[0x00171774]
  00171660: ldr r1,[0x00171778]
  00171662: add r0,pc
  00171664: str.w r4,[r11,#0x4cc]
  00171668: add r1,pc
  0017166a: ldr r0,[r0,#0x0]
  0017166c: ldr r1,[r1,#0x0]
  0017166e: str r0,[sp,#0x38]
  00171670: ldr r0,[r0,#0x0]
  00171672: str r1,[sp,#0x20]
  00171674: ldrb r1,[r1,#0x0]
  00171676: ldr.w r2,[r0,#0x1dc]
  0017167a: cmp r1,#0x0
  0017167c: str r2,[sp,#0x34]
  0017167e: beq 0x00171698
  00171680: ldr.w r2,[r11,#0x238]
  00171684: ldr r3,[0x0017177c]
  00171686: cmp r2,#0x3
  00171688: add r3,pc
  0017168a: ldr r3,[r3,#0x0]
  0017168c: bne 0x001716a2
  0017168e: vldr.32 s0,[r3,#0x54]
  00171692: vcvt.f32.s32 s0,s0
  00171696: b 0x001716d4
  00171698: ldr.w r2,[r11,#0x3d8]
  0017169c: str r2,[sp,#0x3c]
  0017169e: ldr r4,[sp,#0x2c]
  001716a0: b 0x0017175a
  001716a2: ldr r4,[0x00171780]
  001716a4: vldr.32 s0,[r3,#0x58]
  001716a8: add r4,pc
  001716aa: vcvt.f32.s32 s0,s0
  001716ae: ldr r3,[r4,#0x0]
  001716b0: ldrb r3,[r3,#0x0]
  001716b2: cbz r3,0x001716ba
  001716b4: vldr.32 s2,[pc,#0xcc]
  001716b8: b 0x001716d0
  001716ba: ldr r3,[0x00171788]
  001716bc: adr.w r6,0x1722d0
  001716c0: add r3,pc
  001716c2: ldr r3,[r3,#0x0]
  001716c4: ldrb r3,[r3,#0x0]
  001716c6: cmp r3,#0x0
  001716c8: it eq
  001716ca: add.eq r6,#0x4
  001716cc: vldr.32 s2,[r6]
  001716d0: vsub.f32 s0,s0,s2
  001716d4: ldr r4,[sp,#0x2c]
  001716d6: vcmpe.f32 s0,#0
  001716da: vmrs apsr,fpscr
  001716de: bpl 0x001716e6
  001716e0: vldr.32 s0,[pc,#0xa8]
  001716e4: b 0x00171732
  001716e6: cmp r2,#0x3
  001716e8: bne 0x001716fa
  001716ea: ldr r2,[0x00171790]
  001716ec: add r2,pc
  001716ee: ldr r2,[r2,#0x0]
  001716f0: vldr.32 s0,[r2,#0x54]
  001716f4: vcvt.f32.s32 s0,s0
  001716f8: b 0x00171732
  001716fa: ldr r2,[0x00171794]
  001716fc: ldr r3,[0x00171798]
  001716fe: add r2,pc
  00171700: add r3,pc
  00171702: ldr r2,[r2,#0x0]
  00171704: vldr.32 s0,[r2,#0x58]
  00171708: ldr r2,[r3,#0x0]
  0017170a: vcvt.f32.s32 s0,s0
  0017170e: ldrb r2,[r2,#0x0]
  00171710: cbz r2,0x00171718
  00171712: vldr.32 s2,[pc,#0x70]
  00171716: b 0x0017172e
  00171718: ldr r2,[0x0017179c]
  0017171a: adr.w r3,0x1722d0
  0017171e: add r2,pc
  00171720: ldr r2,[r2,#0x0]
  00171722: ldrb r2,[r2,#0x0]
  00171724: cmp r2,#0x0
  00171726: it eq
  00171728: add.eq r3,#0x4
  0017172a: vldr.32 s2,[r3]
  0017172e: vsub.f32 s0,s0,s2
  00171732: vcvt.s32.f32 s2,s0
  00171736: vstr.32 s2,[r11,#0x3c8]
  0017173a: ldr r2,[r0,#0x30]
  0017173c: ldr.w r3,[r11,#0x3cc]
  00171740: vcvt.s32.f32 s0,s0
  00171744: add.w r2,r2,r2, lsr #0x1f
  00171748: vmov r6,s0
  0017174c: add r3,r6
  0017174e: sub.w r2,r3,r2, asr #0x1
  00171752: adds r2,#0x1
  00171754: str r2,[sp,#0x3c]
  00171756: str.w r2,[r11,#0x3d8]
  0017175a: cmp r4,#0x3
  0017175c: bhi.w 0x001720a8
  00171760: tbh [pc,r4]
  001717a0: ldr.w r0,[r11,#0x25c]
  001717a4: cbz r0,0x00171816
  001717a6: ldr r1,[r0,#0x0]
  001717a8: movs r2,#0x0
  001717aa: cmp r2,r1
  001717ac: bcs 0x00171816
  001717ae: ldr r3,[r0,#0x4]
  001717b0: ldr.w r3,[r3,r2,lsl #0x2]
  001717b4: adds r2,#0x1
  001717b6: cmp r3,#0x0
  001717b8: beq 0x001717aa
  001717ba: movs r0,#0xc8
  001717bc: blx 0x0006eb24
  001717c0: mov r5,r0
  001717c2: ldr r0,[0x00171b14]
  001717c4: add r0,pc
  001717c6: ldr r0,[r0,#0x0]
  001717c8: ldr r0,[r0,#0x0]
  001717ca: mov.w r1,#0x10a
  001717ce: blx 0x00072f70
  001717d2: mov r1,r0
  001717d4: ldr r4,[sp,#0x3c]
  001717d6: ldr.w r0,[r11,#0x3dc]
  001717da: ldr.w r3,[r11,#0x3d4]
  001717de: movs r2,#0x4
  001717e0: movs r6,#0x11
  001717e2: strd r4,r0,[sp,#0x0]
  001717e6: mov r0,r5
  001717e8: strd r6,r2,[sp,#0x8]
  001717ec: movs r2,#0x0
  001717ee: movs r6,#0x0
  001717f0: blx 0x00074f50
  001717f4: mov.w r0,#0x200
  001717f8: strd r0,r6,[r5,#0x0]
  001717fc: mov r0,r5
  001717fe: ldr.w r1,[r10,#0x0]
  00171802: blx 0x0007648c
  00171806: ldr r0,[sp,#0x38]
  00171808: ldr r1,[sp,#0x34]
  0017180a: ldr r0,[r0,#0x0]
  0017180c: add r1,r4
  0017180e: ldr r4,[sp,#0x2c]
  00171810: ldr r0,[r0,#0x30]
  00171812: add r0,r1
  00171814: str r0,[sp,#0x3c]
  00171816: ldr.w r0,[r8,#0x0]
  0017181a: blx 0x000741dc
  0017181e: cbz r0,0x00171892
  00171820: ldr.w r0,[r8,#0x0]
  00171824: blx 0x00071878
  00171828: cbnz r0,0x00171892
  0017182a: ldr.w r0,[r8,#0x0]
  0017182e: blx 0x00071770
  00171832: cmp r0,#0x9e
  00171834: beq 0x00171892
  00171836: movs r0,#0xc8
  00171838: blx 0x0006eb24
  0017183c: mov r5,r0
  0017183e: ldr r0,[0x00171b18]
  00171840: add r0,pc
  00171842: ldr r0,[r0,#0x0]
  00171844: ldr r0,[r0,#0x0]
  00171846: mov.w r1,#0x132
  0017184a: blx 0x00072f70
  0017184e: mov r1,r0
  00171850: ldr r4,[sp,#0x3c]
  00171852: ldr.w r0,[r11,#0x3dc]
  00171856: ldr.w r3,[r11,#0x3d4]
  0017185a: movs r2,#0x4
  0017185c: movs r6,#0x11
  0017185e: strd r4,r0,[sp,#0x0]
  00171862: mov r0,r5
  00171864: strd r6,r2,[sp,#0x8]
  00171868: movs r2,#0x0
  0017186a: movs r6,#0x0
  0017186c: blx 0x00074f50
  00171870: mov.w r0,#0x400
  00171874: strd r0,r6,[r5,#0x0]
  00171878: mov r0,r5
  0017187a: ldr.w r1,[r10,#0x0]
  0017187e: blx 0x0007648c
  00171882: ldr r0,[sp,#0x38]
  00171884: ldr r1,[sp,#0x34]
  00171886: ldr r0,[r0,#0x0]
  00171888: add r1,r4
  0017188a: ldr r4,[sp,#0x2c]
  0017188c: ldr r0,[r0,#0x30]
  0017188e: add r0,r1
  00171890: str r0,[sp,#0x3c]
  00171892: ldr.w r0,[r8,#0x0]
  00171896: blx 0x00071a58
  0017189a: blx 0x000779e0
  0017189e: cmp r0,#0x0
  001718a0: beq 0x0017196a
  001718a2: ldr.w r0,[r8,#0x0]
  001718a6: blx 0x00071a58
  001718aa: movs r1,#0x15
  001718ac: blx 0x0007228c
  001718b0: mov r6,r0
  001718b2: movs r0,#0xc8
  001718b4: blx 0x0006eb24
  001718b8: mov r5,r0
  001718ba: ldr r0,[0x00171b1c]
  001718bc: add r0,pc
  001718be: ldr r0,[r0,#0x0]
  001718c0: ldr r4,[r0,#0x0]
  001718c2: mov r0,r6
  001718c4: blx 0x000718d8
  001718c8: addw r1,r0,#0x4fa
  001718cc: mov r0,r4
  001718ce: blx 0x00072f70
  001718d2: mov r1,r0
  001718d4: ldr.w r0,[r11,#0x3dc]
  001718d8: ldr.w r3,[r11,#0x3d4]
  001718dc: ldr r4,[sp,#0x3c]
  001718de: movs r2,#0x4
  001718e0: movs r6,#0x11
  001718e2: strd r4,r0,[sp,#0x0]
  001718e6: strd r6,r2,[sp,#0x8]
  001718ea: mov r0,r5
  001718ec: movs r2,#0x0
  001718ee: blx 0x00074f50
  001718f2: mov r0,r5
  001718f4: movs r1,#0x0
  001718f6: blx 0x00077a34
  001718fa: ldr r0,[sp,#0x30]
  001718fc: blx 0x00072034
  00171900: blx 0x00077a40
  00171904: ldr r4,[sp,#0x2c]
  00171906: cbnz r0,0x00171920
  00171908: ldr r0,[sp,#0x30]
  0017190a: blx 0x00072034
  0017190e: blx 0x00077a4c
  00171912: cbnz r0,0x00171920
  00171914: ldr r0,[sp,#0x30]
  00171916: blx 0x00072034
  0017191a: blx 0x00077a58
  0017191e: cbz r0,0x00171946
  00171920: ldr r0,[sp,#0x30]
  00171922: blx 0x00072034
  00171926: blx 0x00077a58
  0017192a: cbz r0,0x0017193e
  0017192c: ldr r0,[sp,#0x30]
  0017192e: blx 0x00072034
  00171932: blx 0x00077a64
  00171936: mov r1,r0
  00171938: mov r0,r5
  0017193a: blx 0x00077a70
  0017193e: mov r0,r5
  00171940: movs r1,#0x1
  00171942: blx 0x00074fa4
  00171946: movs r0,#0x0
  00171948: mov.w r1,#0x800
  0017194c: strd r1,r0,[r5,#0x0]
  00171950: mov r0,r5
  00171952: ldr.w r1,[r10,#0x0]
  00171956: blx 0x0007648c
  0017195a: ldr r0,[sp,#0x38]
  0017195c: ldr r1,[sp,#0x34]
  0017195e: ldr r2,[sp,#0x3c]
  00171960: ldr r0,[r0,#0x0]
  00171962: add r1,r2
  00171964: ldr r0,[r0,#0x30]
  00171966: add r0,r1
  00171968: str r0,[sp,#0x3c]
  0017196a: ldr.w r0,[r8,#0x0]
  0017196e: blx 0x00071a58
  00171972: blx 0x00071a64
  00171976: cbz r0,0x001719e8
  00171978: movs r0,#0xc8
  0017197a: blx 0x0006eb24
  0017197e: mov r5,r0
  00171980: ldr r0,[0x00171b20]
  00171982: add r0,pc
  00171984: ldr r0,[r0,#0x0]
  00171986: ldr r0,[r0,#0x0]
  00171988: movw r1,#0x54f
  0017198c: blx 0x00072f70
  00171990: mov r1,r0
  00171992: ldr.w r0,[r11,#0x3dc]
  00171996: ldr.w r3,[r11,#0x3d4]
  0017199a: ldr r2,[sp,#0x3c]
  0017199c: mov.w r12,#0x4
  001719a0: strd r2,r0,[sp,#0x0]
  001719a4: movs r6,#0x11
  001719a6: mov r0,r5
  001719a8: movs r2,#0x0
  001719aa: strd r6,r12,[sp,#0x8]
  001719ae: blx 0x00074f50
  001719b2: ldr r6,[sp,#0x30]
  001719b4: mov r0,r6
  001719b6: blx 0x00072034
  001719ba: blx 0x00077a7c
  001719be: cbnz r0,0x001719cc
  001719c0: mov r0,r6
  001719c2: blx 0x00072034
  001719c6: blx 0x000743e0
  001719ca: cbz r0,0x001719d4
  001719cc: mov r0,r5
  001719ce: movs r1,#0x1
  001719d0: blx 0x00074fa4
  001719d4: movs r0,#0x0
  001719d6: mov.w r1,#0x1000
  001719da: strd r1,r0,[r5,#0x0]
  001719de: mov r0,r5
  001719e0: ldr.w r1,[r10,#0x0]
  001719e4: blx 0x0007648c
  001719e8: ldr.w r0,[r8,#0x0]
  001719ec: blx 0x00071a58
  001719f0: movs r1,#0x7a
  001719f2: blx 0x00072424
  001719f6: cmp r0,#0x0
  001719f8: beq.w 0x00171d28
  001719fc: blx 0x0007183c
  00171a00: b 0x00171d2a
  00171a02: mov.w r0,#0x134
  00171a06: movw r1,#0x135
  00171a0a: str r0,[sp,#0x90]
  00171a0c: movw r0,#0x133
  00171a10: str r0,[sp,#0x8c]
  00171a12: movs r6,#0x0
  00171a14: ldr.w r0,[r8,#0x0]
  00171a18: str r1,[sp,#0x94]
  00171a1a: adr.w r1,0x1722c0
  00171a1e: vld1.64 {d16,d17},[r1]
  00171a22: movw r1,#0x137
  00171a26: ldrb.w r0,[r0,#0xf8]
  00171a2a: str.w r9,[sp,#0x1c]
  00171a2e: add.w r9,sp,#0x8c
  00171a32: cmp r0,#0x0
  00171a34: add r0,sp,#0x40
  00171a36: it eq
  00171a38: mov.eq.w r1,#0x136
  00171a3c: str r1,[sp,#0x98]
  00171a3e: vst1.64 {d16,d17},[r0:128]
  00171a42: ldr r0,[0x00171b24]
  00171a44: add r0,pc
  00171a46: ldr r4,[r0,#0x0]
  00171a48: b 0x00171aa4
  00171a4a: movs r0,#0xc8
  00171a4c: blx 0x0006eb24
  00171a50: ldr.w r1,[r9,r6,lsl #0x2]
  00171a54: mov r5,r0
  00171a56: ldr r0,[r4,#0x0]
  00171a58: blx 0x00072f70
  00171a5c: mov r1,r0
  00171a5e: ldr.w r8,[sp,#0x3c]
  00171a62: ldr.w r0,[r11,#0x3dc]
  00171a66: ldr.w r3,[r11,#0x3d4]
  00171a6a: strd r8,r0,[sp,#0x0]
  00171a6e: movs r0,#0x11
  00171a70: str r0,[sp,#0x8]
  00171a72: movs r0,#0x4
  00171a74: str r0,[sp,#0xc]
  00171a76: mov r0,r5
  00171a78: movs r2,#0x0
  00171a7a: blx 0x00074f50
  00171a7e: add r0,sp,#0x40
  00171a80: ldr.w r0,[r0,r6,lsl #0x2]
  00171a84: asrs r1,r0,#0x1f
  00171a86: strd r0,r1,[r5,#0x0]
  00171a8a: mov r0,r5
  00171a8c: ldr.w r1,[r10,#0x0]
  00171a90: blx 0x0007648c
  00171a94: ldr r0,[sp,#0x38]
  00171a96: adds r6,#0x1
  00171a98: ldr r1,[sp,#0x34]
  00171a9a: ldr r0,[r0,#0x0]
  00171a9c: add r1,r8
  00171a9e: ldr r0,[r0,#0x30]
  00171aa0: add r0,r1
  00171aa2: str r0,[sp,#0x3c]
  00171aa4: cmp r6,#0x4
  00171aa6: bcc 0x00171a4a
  00171aa8: ldr r0,[0x00171b28]
  00171aaa: add.w r2,r11,#0x35c
  00171aae: movw r1,#0x4f3
  00171ab2: add r0,pc
  00171ab4: ldr r0,[r0,#0x0]
  00171ab6: ldr r0,[r0,#0x0]
  00171ab8: blx 0x00071cf8
  00171abc: ldr.w r9,[sp,#0x1c]
  00171ac0: b 0x001720a6
  00171ac2: cbz r1,0x00171ad0
  00171ac4: ldr.w r1,[r11,#0x3c4]
  00171ac8: ldr r0,[r0,#0x28]
  00171aca: subs r0,r0,r1
  00171acc: str.w r0,[r11,#0x4cc]
  00171ad0: ldr.w r0,[r8,#0x0]
  00171ad4: blx 0x000723d0
  00171ad8: cmp r0,#0x0
  00171ada: str.w r9,[sp,#0x1c]
  00171ade: beq.w 0x00171c74
  00171ae2: add.w r0,r11,#0x3dc
  00171ae6: str r0,[sp,#0x28]
  00171ae8: add.w r0,r11,#0x3d4
  00171aec: str r0,[sp,#0x24]
  00171aee: ldr.w r9,[sp,#0x30]
  00171af2: b 0x00171f62
  00171af4: ldr r0,[0x00171b2c]
  00171af6: mov.w r8,#0x0
  00171afa: add r0,pc
  00171afc: ldr r0,[r0,#0x0]
  00171afe: str r0,[sp,#0x30]
  00171b00: ldr r0,[0x00171b30]
  00171b02: add r0,pc
  00171b04: str r0,[sp,#0x28]
  00171b06: ldr r0,[0x00171b34]
  00171b08: add r0,pc
  00171b0a: str r0,[sp,#0x24]
  00171b0c: strd r10,r9,[sp,#0x18]
  00171b10: b 0x00171c5a
  00171b38: ldr r0,[r4,#0x4]
  00171b3a: ldr.w r0,[r0,r8,lsl #0x2]
  00171b3e: cmp r0,#0x0
  00171b40: beq.w 0x00171c56
  00171b44: movs r0,#0xc8
  00171b46: blx 0x0006eb24
  00171b4a: mov r10,r0
  00171b4c: ldr r0,[r4,#0x4]
  00171b4e: ldr r1,[sp,#0x30]
  00171b50: ldr.w r0,[r0,r8,lsl #0x2]
  00171b54: ldr r4,[r1,#0x0]
  00171b56: blx 0x000718d8
  00171b5a: addw r1,r0,#0x4fa
  00171b5e: mov r0,r4
  00171b60: blx 0x00072f70
  00171b64: mov r9,r0
  00171b66: ldr r1,[sp,#0x28]
  00171b68: add r0,sp,#0x68
  00171b6a: movs r2,#0x0
  00171b6c: blx 0x0006ee18
  00171b70: ldr.w r0,[r11,#0x25c]
  00171b74: ldr r0,[r0,#0x4]
  00171b76: ldr.w r0,[r0,r8,lsl #0x2]
  00171b7a: blx 0x0007183c
  00171b7e: mov r1,r0
  00171b80: add r4,sp,#0x5c
  00171b82: mov r0,r4
  00171b84: blx 0x0006f658
  00171b88: add r0,sp,#0x74
  00171b8a: add r1,sp,#0x68
  00171b8c: mov r2,r4
  00171b8e: blx 0x0006ef98
  00171b92: add r6,sp,#0x50
  00171b94: ldr r1,[sp,#0x24]
  00171b96: movs r2,#0x0
  00171b98: mov r4,r10
  00171b9a: mov r0,r6
  00171b9c: blx 0x0006ee18
  00171ba0: add.w r10,sp,#0x80
  00171ba4: add r1,sp,#0x74
  00171ba6: mov r2,r6
  00171ba8: mov r0,r10
  00171baa: blx 0x0006ef98
  00171bae: add r5,sp,#0x40
  00171bb0: mov r1,r10
  00171bb2: movs r2,#0x0
  00171bb4: mov r10,r4
  00171bb6: mov r0,r5
  00171bb8: blx 0x0006f028
  00171bbc: add r6,sp,#0x8c
  00171bbe: mov r1,r9
  00171bc0: mov r2,r5
  00171bc2: mov r0,r6
  00171bc4: blx 0x0006ef98
  00171bc8: ldr.w r0,[r11,#0x3dc]
  00171bcc: ldr.w r3,[r11,#0x3d4]
  00171bd0: ldr r1,[sp,#0x3c]
  00171bd2: movs r2,#0x0
  00171bd4: strd r1,r0,[sp,#0x0]
  00171bd8: movs r0,#0x11
  00171bda: str r0,[sp,#0x8]
  00171bdc: movs r0,#0x4
  00171bde: str r0,[sp,#0xc]
  00171be0: mov r0,r10
  00171be2: mov r1,r6
  00171be4: blx 0x00074f50
  00171be8: ldr r0,[0x00171f70]
  00171bea: add r0,pc
  00171bec: ldr r4,[r0,#0x0]
  00171bee: mov r0,r6
  00171bf0: blx r4
  00171bf2: mov r0,r5
  00171bf4: blx r4
  00171bf6: add r0,sp,#0x80
  00171bf8: blx r4
  00171bfa: add r0,sp,#0x50
  00171bfc: blx r4
  00171bfe: add r0,sp,#0x74
  00171c00: blx r4
  00171c02: add r0,sp,#0x5c
  00171c04: blx r4
  00171c06: add r0,sp,#0x68
  00171c08: blx r4
  00171c0a: bic r0,r8,#0x80000000
  00171c0e: cbz r0,0x00171c1e
  00171c10: ldrd r2,r9,[sp,#0x18]
  00171c14: cmp r0,#0x1
  00171c16: bne 0x00171c28
  00171c18: mov.w r0,#0x4000
  00171c1c: b 0x00171c36
  00171c1e: mov.w r0,#0x2000
  00171c22: ldrd r2,r9,[sp,#0x18]
  00171c26: b 0x00171c36
  00171c28: mov.w r0,#0x10000
  00171c2c: cmp.w r8,#0x2
  00171c30: it eq
  00171c32: mov.eq.w r0,#0x8000
  00171c36: movs r1,#0x0
  00171c38: strd r0,r1,[r10,#0x0]
  00171c3c: mov r0,r10
  00171c3e: ldr r1,[r2,#0x0]
  00171c40: mov r10,r2
  00171c42: blx 0x0007648c
  00171c46: ldr r0,[sp,#0x38]
  00171c48: ldr r1,[sp,#0x34]
  00171c4a: ldr r2,[sp,#0x3c]
  00171c4c: ldr r0,[r0,#0x0]
  00171c4e: add r1,r2
  00171c50: ldr r0,[r0,#0x30]
  00171c52: add r0,r1
  00171c54: str r0,[sp,#0x3c]
  00171c56: add.w r8,r8,#0x1
  00171c5a: ldr.w r4,[r11,#0x25c]
  00171c5e: ldr r0,[r4,#0x0]
  00171c60: cmp r8,r0
  00171c62: bcc.w 0x00171b38
  00171c66: ldr r0,[0x00171f74]
  00171c68: add.w r2,r11,#0x35c
  00171c6c: movw r1,#0x4f6
  00171c70: add r0,pc
  00171c72: b 0x0017209e
  00171c74: movs r0,#0xc8
  00171c76: blx 0x0006eb24
  00171c7a: mov r5,r0
  00171c7c: ldr r0,[0x00171f78]
  00171c7e: add r0,pc
  00171c80: ldr r0,[r0,#0x0]
  00171c82: str r0,[sp,#0x14]
  00171c84: ldr r0,[r0,#0x0]
  00171c86: movw r1,#0x225
  00171c8a: blx 0x00072f70
  00171c8e: mov r1,r0
  00171c90: ldr r4,[sp,#0x3c]
  00171c92: ldr.w r0,[r11,#0x3dc]
  00171c96: ldr.w r3,[r11,#0x3d4]
  00171c9a: movs r2,#0x4
  00171c9c: movs r6,#0x11
  00171c9e: strd r4,r0,[sp,#0x0]
  00171ca2: mov r0,r5
  00171ca4: strd r6,r2,[sp,#0x8]
  00171ca8: movs r2,#0x0
  00171caa: movs r6,#0x0
  00171cac: blx 0x00074f50
  00171cb0: add.w r0,r11,#0x3dc
  00171cb4: str r0,[sp,#0x28]
  00171cb6: add.w r0,r11,#0x3d4
  00171cba: str r0,[sp,#0x24]
  00171cbc: mov.w r0,#0x1000000
  00171cc0: strd r0,r6,[r5,#0x0]
  00171cc4: mov r0,r5
  00171cc6: ldr.w r1,[r10,#0x0]
  00171cca: blx 0x0007648c
  00171cce: ldr r0,[sp,#0x38]
  00171cd0: ldr r0,[r0,#0x0]
  00171cd2: ldr r1,[r0,#0x30]
  00171cd4: ldr r0,[sp,#0x34]
  00171cd6: adds r2,r4,r0
  00171cd8: ldr.w r0,[r8,#0x0]
  00171cdc: adds r4,r2,r1
  00171cde: blx 0x00073d20
  00171ce2: cmp r0,#0x0
  00171ce4: bne 0x00171dce
  00171ce6: movs r0,#0xc8
  00171ce8: blx 0x0006eb24
  00171cec: mov r5,r0
  00171cee: ldr.w r0,[r8,#0x0]
  00171cf2: blx 0x00071c14
  00171cf6: mov r1,r0
  00171cf8: add r0,sp,#0x80
  00171cfa: blx 0x000736a8
  00171cfe: add r0,sp,#0x40
  00171d00: add r1,sp,#0x80
  00171d02: movs r2,#0x0
  00171d04: blx 0x0006f028
  00171d08: ldr.w r0,[r8,#0x0]
  00171d0c: blx 0x00071c14
  00171d10: blx 0x00071824
  00171d14: cmp r0,#0x65
  00171d16: bne 0x00171d44
  00171d18: ldr r1,[0x00171f7c]
  00171d1a: movs r6,#0x0
  00171d1c: add r1,pc
  00171d1e: add r0,sp,#0x74
  00171d20: movs r2,#0x0
  00171d22: blx 0x0006ee18
  00171d26: b 0x00171d66
  00171d28: movs r0,#0x0
  00171d2a: ldr r1,[0x00171f80]
  00171d2c: add.w r2,r11,#0x35c
  00171d30: str.w r0,[r11,#0x27c]
  00171d34: add r1,pc
  00171d36: ldr r1,[r1,#0x0]
  00171d38: ldr r0,[r1,#0x0]
  00171d3a: movw r1,#0x4f5
  00171d3e: blx 0x00071cf8
  00171d42: b 0x001720a8
  00171d44: ldr r1,[0x00171f84]
  00171d46: add r1,pc
  00171d48: add r0,sp,#0x68
  00171d4a: movs r2,#0x0
  00171d4c: blx 0x0006ee18
  00171d50: ldr r0,[sp,#0x14]
  00171d52: ldr r0,[r0,#0x0]
  00171d54: movs r1,#0x88
  00171d56: blx 0x00072f70
  00171d5a: mov r2,r0
  00171d5c: add r0,sp,#0x74
  00171d5e: add r1,sp,#0x68
  00171d60: blx 0x0006ef98
  00171d64: movs r6,#0x1
  00171d66: add r0,sp,#0x8c
  00171d68: add r1,sp,#0x40
  00171d6a: add r2,sp,#0x74
  00171d6c: blx 0x0006ef98
  00171d70: ldr r0,[sp,#0x24]
  00171d72: ldr r3,[r0,#0x0]
  00171d74: ldr r0,[sp,#0x28]
  00171d76: ldr r0,[r0,#0x0]
  00171d78: movs r1,#0x4
  00171d7a: movs r2,#0x11
  00171d7c: strd r4,r0,[sp,#0x0]
  00171d80: mov r0,r5
  00171d82: strd r2,r1,[sp,#0x8]
  00171d86: add r1,sp,#0x8c
  00171d88: movs r2,#0x0
  00171d8a: blx 0x00074f50
  00171d8e: add r0,sp,#0x8c
  00171d90: blx 0x0006ee30
  00171d94: add r0,sp,#0x74
  00171d96: blx 0x0006ee30
  00171d9a: cbz r6,0x00171da2
  00171d9c: add r0,sp,#0x68
  00171d9e: blx 0x0006ee30
  00171da2: add r0,sp,#0x40
  00171da4: blx 0x0006ee30
  00171da8: add r0,sp,#0x80
  00171daa: blx 0x0006ee30
  00171dae: movs r0,#0x0
  00171db0: mov.w r1,#0x800000
  00171db4: strd r1,r0,[r5,#0x0]
  00171db8: mov r0,r5
  00171dba: ldr.w r1,[r10,#0x0]
  00171dbe: blx 0x0007648c
  00171dc2: ldr r0,[sp,#0x38]
  00171dc4: ldr r1,[sp,#0x34]
  00171dc6: ldr r0,[r0,#0x0]
  00171dc8: add r1,r4
  00171dca: ldr r0,[r0,#0x30]
  00171dcc: adds r4,r1,r0
  00171dce: ldr.w r0,[r8,#0x0]
  00171dd2: blx 0x00071a10
  00171dd6: blx 0x00073c18
  00171dda: cbz r0,0x00171e32
  00171ddc: movs r0,#0xc8
  00171dde: blx 0x0006eb24
  00171de2: mov r5,r0
  00171de4: ldr r0,[sp,#0x14]
  00171de6: ldr r0,[r0,#0x0]
  00171de8: movw r1,#0x223
  00171dec: blx 0x00072f70
  00171df0: mov r1,r0
  00171df2: ldr r0,[sp,#0x24]
  00171df4: ldr r3,[r0,#0x0]
  00171df6: ldr r0,[sp,#0x28]
  00171df8: ldr r0,[r0,#0x0]
  00171dfa: movs r2,#0x4
  00171dfc: movs r6,#0x11
  00171dfe: strd r4,r0,[sp,#0x0]
  00171e02: mov r0,r5
  00171e04: strd r6,r2,[sp,#0x8]
  00171e08: movs r2,#0x0
  00171e0a: movs r6,#0x0
  00171e0c: blx 0x00074f50
  00171e10: mov.w r0,#0x400000
  00171e14: strd r0,r6,[r5,#0x0]
  00171e18: mov r0,r5
  00171e1a: ldr.w r1,[r10,#0x0]
  00171e1e: blx 0x0007648c
  00171e22: ldr r0,[sp,#0x38]
  00171e24: ldr r1,[sp,#0x34]
  00171e26: ldr r0,[r0,#0x0]
  00171e28: add r1,r4
  00171e2a: ldr r0,[r0,#0x30]
  00171e2c: add.w r8,r1,r0
  00171e30: b 0x00171e34
  00171e32: mov r8,r4
  00171e34: ldr r4,[sp,#0x30]
  00171e36: mov r0,r4
  00171e38: blx 0x00072034
  00171e3c: blx 0x000768b8
  00171e40: cbz r0,0x00171eaa
  00171e42: mov r0,r4
  00171e44: blx 0x00072034
  00171e48: blx 0x000768b8
  00171e4c: blx 0x00071ee4
  00171e50: ldrb.w r0,[r0,#0x130]
  00171e54: cbnz r0,0x00171eaa
  00171e56: movs r0,#0xc8
  00171e58: blx 0x0006eb24
  00171e5c: mov r5,r0
  00171e5e: ldr r0,[sp,#0x14]
  00171e60: ldr r0,[r0,#0x0]
  00171e62: movw r1,#0x23d
  00171e66: blx 0x00072f70
  00171e6a: mov r1,r0
  00171e6c: ldr r0,[sp,#0x24]
  00171e6e: ldr r3,[r0,#0x0]
  00171e70: ldr r0,[sp,#0x28]
  00171e72: ldr r0,[r0,#0x0]
  00171e74: movs r2,#0x4
  00171e76: movs r6,#0x11
  00171e78: strd r8,r0,[sp,#0x0]
  00171e7c: mov r0,r5
  00171e7e: strd r6,r2,[sp,#0x8]
  00171e82: movs r2,#0x0
  00171e84: movs r6,#0x0
  00171e86: blx 0x00074f50
  00171e8a: mov.w r0,#0x2000000
  00171e8e: strd r0,r6,[r5,#0x0]
  00171e92: mov r0,r5
  00171e94: ldr.w r1,[r10,#0x0]
  00171e98: blx 0x0007648c
  00171e9c: ldr r0,[sp,#0x38]
  00171e9e: ldr r1,[sp,#0x34]
  00171ea0: ldr r0,[r0,#0x0]
  00171ea2: add r1,r8
  00171ea4: ldr r0,[r0,#0x30]
  00171ea6: add.w r8,r1,r0
  00171eaa: ldr r0,[0x00171f88]
  00171eac: ldr.w r9,[sp,#0x30]
  00171eb0: add r0,pc
  00171eb2: ldr r6,[r0,#0x0]
  00171eb4: ldr r0,[r6,#0x0]
  00171eb6: cmp r0,#0x0
  00171eb8: beq 0x00171f5e
  00171eba: movs r0,#0xc8
  00171ebc: blx 0x0006eb24
  00171ec0: mov r5,r0
  00171ec2: ldr r0,[sp,#0x14]
  00171ec4: ldr r0,[r0,#0x0]
  00171ec6: movw r1,#0x222
  00171eca: blx 0x00072f70
  00171ece: ldr r1,[0x00171f8c]
  00171ed0: mov r4,r0
  00171ed2: add r1,pc
  00171ed4: add r0,sp,#0x80
  00171ed6: movs r2,#0x0
  00171ed8: blx 0x0006ee18
  00171edc: add r0,sp,#0x40
  00171ede: add r2,sp,#0x80
  00171ee0: mov r1,r4
  00171ee2: blx 0x0006ef98
  00171ee6: ldr r1,[r6,#0x0]
  00171ee8: add r0,sp,#0x5c
  00171eea: blx 0x000736a8
  00171eee: add r0,sp,#0x74
  00171ef0: add r1,sp,#0x5c
  00171ef2: movs r2,#0x0
  00171ef4: blx 0x0006f028
  00171ef8: add r0,sp,#0x8c
  00171efa: add r1,sp,#0x40
  00171efc: add r2,sp,#0x74
  00171efe: blx 0x0006ef98
  00171f02: ldr r0,[sp,#0x24]
  00171f04: ldr r3,[r0,#0x0]
  00171f06: ldr r0,[sp,#0x28]
  00171f08: ldr r0,[r0,#0x0]
  00171f0a: movs r1,#0x4
  00171f0c: movs r2,#0x11
  00171f0e: strd r8,r0,[sp,#0x0]
  00171f12: mov r0,r5
  00171f14: strd r2,r1,[sp,#0x8]
  00171f18: add r1,sp,#0x8c
  00171f1a: movs r2,#0x0
  00171f1c: movs r4,#0x0
  00171f1e: blx 0x00074f50
  00171f22: ldr r0,[0x00171f90]
  00171f24: add r0,pc
  00171f26: ldr r6,[r0,#0x0]
  00171f28: add r0,sp,#0x8c
  00171f2a: blx r6
  00171f2c: add r0,sp,#0x74
  00171f2e: blx r6
  00171f30: add r0,sp,#0x5c
  00171f32: blx r6
  00171f34: add r0,sp,#0x40
  00171f36: blx r6
  00171f38: add r0,sp,#0x80
  00171f3a: blx r6
  00171f3c: mov.w r0,#0x200000
  00171f40: strd r0,r4,[r5,#0x0]
  00171f44: mov r0,r5
  00171f46: ldr.w r1,[r10,#0x0]
  00171f4a: blx 0x0007648c
  00171f4e: ldr r0,[sp,#0x38]
  00171f50: ldr r1,[sp,#0x34]
  00171f52: ldr r0,[r0,#0x0]
  00171f54: add r1,r8
  00171f56: ldr r0,[r0,#0x30]
  00171f58: add r0,r1
  00171f5a: str r0,[sp,#0x3c]
  00171f5c: b 0x00171f62
  00171f5e: str.w r8,[sp,#0x3c]
  00171f62: mov r0,r9
  00171f64: blx 0x00072a54
  00171f68: add r4,sp,#0x8c
  00171f6a: mov r8,r0
  00171f6c: movs r6,#0x0
  00171f6e: b 0x00172018
  00171f94: mov r0,r9
  00171f96: mov r1,r6
  00171f98: blx 0x00072a48
  00171f9c: mov r1,r0
  00171f9e: mov r0,r4
  00171fa0: blx 0x00077a88
  00171fa4: mov r0,r4
  00171fa6: ldr r5,[sp,#0x94]
  00171fa8: blx 0x0006ee30
  00171fac: cbz r5,0x00172016
  00171fae: movs r0,#0xc8
  00171fb0: blx 0x0006eb24
  00171fb4: mov r5,r0
  00171fb6: mov r0,r9
  00171fb8: mov r1,r6
  00171fba: blx 0x00072a48
  00171fbe: mov r1,r0
  00171fc0: mov r0,r4
  00171fc2: blx 0x00077a88
  00171fc6: ldr r0,[sp,#0x24]
  00171fc8: ldr r3,[r0,#0x0]
  00171fca: ldr r0,[sp,#0x28]
  00171fcc: ldr r0,[r0,#0x0]
  00171fce: ldr.w r9,[sp,#0x3c]
  00171fd2: mov r1,r4
  00171fd4: strd r9,r0,[sp,#0x0]
  00171fd8: movs r0,#0x11
  00171fda: str r0,[sp,#0x8]
  00171fdc: movs r0,#0x4
  00171fde: str r0,[sp,#0xc]
  00171fe0: mov r0,r5
  00171fe2: movs r2,#0x0
  00171fe4: blx 0x00074f50
  00171fe8: mov r0,r4
  00171fea: blx 0x0006ee30
  00171fee: mov.w r0,#0x4000000
  00171ff2: lsls r0,r6
  00171ff4: asrs r1,r0,#0x1f
  00171ff6: strd r0,r1,[r5,#0x0]
  00171ffa: mov r0,r5
  00171ffc: ldr.w r1,[r10,#0x0]
  00172000: blx 0x0007648c
  00172004: ldr r0,[sp,#0x38]
  00172006: ldr r1,[sp,#0x34]
  00172008: ldr r0,[r0,#0x0]
  0017200a: add r1,r9
  0017200c: ldr.w r9,[sp,#0x30]
  00172010: ldr r0,[r0,#0x30]
  00172012: add r0,r1
  00172014: str r0,[sp,#0x3c]
  00172016: adds r6,#0x1
  00172018: cmp r6,r8
  0017201a: blt 0x00171f94
  0017201c: ldr r0,[sp,#0x20]
  0017201e: ldr.w r9,[sp,#0x1c]
  00172022: ldrb r0,[r0,#0x0]
  00172024: cbnz r0,0x00172092
  00172026: ldr.w r0,[r10,#0x0]
  0017202a: ldr r1,[r0,#0x0]
  0017202c: cmp r1,#0x5
  0017202e: bcc 0x00172092
  00172030: add.w r8,sp,#0x8c
  00172034: add r5,sp,#0x40
  00172036: movs r4,#0x0
  00172038: b 0x0017208e
  0017203a: ldr r0,[r0,#0x4]
  0017203c: ldr.w r6,[r0,r4,lsl #0x2]
  00172040: mov r0,r8
  00172042: mov r1,r6
  00172044: blx 0x00074dd0
  00172048: ldr.w r0,[r10,#0x0]
  0017204c: vldr.32 s16,[sp,#0x8c]
  00172050: ldr r0,[r0,#0x4]
  00172052: ldr.w r1,[r0,r4,lsl #0x2]
  00172056: mov r0,r5
  00172058: blx 0x00074dd0
  0017205c: ldr r0,[sp,#0x38]
  0017205e: ldr r0,[r0,#0x0]
  00172060: vldr.32 s0,[r0,#0x30]
  00172064: mov r0,r6
  00172066: vcvt.f32.s32 s0,s0
  0017206a: vldr.32 s2,[sp,#0x44]
  0017206e: vsub.f32 s0,s2,s0
  00172072: vcvt.s32.f32 s0,s0
  00172076: vcvt.s32.f32 s2,s16
  0017207a: vmov r2,s0
  0017207e: vmov r1,s2
  00172082: blx 0x00076498
  00172086: ldr.w r0,[r10,#0x0]
  0017208a: adds r4,#0x1
  0017208c: ldr r1,[r0,#0x0]
  0017208e: cmp r4,r1
  00172090: bcc 0x0017203a
  00172092: ldr r0,[0x001722d8]
  00172094: add.w r2,r11,#0x35c
  00172098: movw r1,#0x4f4
  0017209c: add r0,pc
  0017209e: ldr r0,[r0,#0x0]
  001720a0: ldr r0,[r0,#0x0]
  001720a2: blx 0x00071cf8
  001720a6: ldr r4,[sp,#0x2c]
  001720a8: ldr r2,[sp,#0x20]
  001720aa: ldr.w r0,[r10,#0x0]
  001720ae: ldrb r2,[r2,#0x0]
  001720b0: ldr r1,[r0,#0x0]
  001720b2: cmp r2,#0x0
  001720b4: beq 0x00172152
  001720b6: cmp r1,#0x0
  001720b8: beq.w 0x001721c6
  001720bc: ldr r2,[sp,#0x38]
  001720be: rsb.w r6,r1,#0x4
  001720c2: ldr r3,[sp,#0x34]
  001720c4: cmp r4,#0x3
  001720c6: add.w r8,sp,#0x8c
  001720ca: mov.w r5,#0x0
  001720ce: ldr r2,[r2,#0x0]
  001720d0: ldr r2,[r2,#0x30]
  001720d2: add r3,r2
  001720d4: mul r3,r6,r3
  001720d8: it eq
  001720da: sub.eq r3,r3,r2
  001720dc: str.w r3,[r11,#0x4d0]
  001720e0: ldr r2,[0x001722dc]
  001720e2: add r2,pc
  001720e4: ldr r6,[r2,#0x0]
  001720e6: ldr r2,[0x001722e0]
  001720e8: add r2,pc
  001720ea: ldr r4,[r2,#0x0]
  001720ec: b 0x0017214c
  001720ee: ldr r0,[r0,#0x4]
  001720f0: ldr.w r1,[r11,#0x4cc]
  001720f4: ldr.w r2,[r11,#0x4d0]
  001720f8: ldr.w r0,[r0,r5,lsl #0x2]
  001720fc: blx 0x00076da4
  00172100: cmp r5,#0x9
  00172102: bhi 0x00172144
  00172104: ldr.w r0,[r10,#0x0]
  00172108: ldr r0,[r0,#0x4]
  0017210a: ldr.w r1,[r0,r5,lsl #0x2]
  0017210e: mov r0,r8
  00172110: blx 0x00074dd0
  00172114: vldr.32 s0,[sp,#0x8c]
  00172118: vcvt.s32.f32 s0,s0
  0017211c: vmov r0,s0
  00172120: str.w r0,[r6,r5,lsl #0x2]
  00172124: ldr.w r0,[r10,#0x0]
  00172128: ldr r0,[r0,#0x4]
  0017212a: ldr.w r1,[r0,r5,lsl #0x2]
  0017212e: mov r0,r8
  00172130: blx 0x00074dd0
  00172134: vldr.32 s0,[sp,#0x90]
  00172138: vcvt.s32.f32 s0,s0
  0017213c: vmov r0,s0
  00172140: str.w r0,[r4,r5,lsl #0x2]
  00172144: ldr.w r0,[r10,#0x0]
  00172148: adds r5,#0x1
  0017214a: ldr r1,[r0,#0x0]
  0017214c: cmp r5,r1
  0017214e: bcc 0x001720ee
  00172150: b 0x001721c6
  00172152: cmp r1,#0x5
  00172154: bcc 0x00172160
  00172156: ldr r2,[sp,#0x38]
  00172158: ldr r2,[r2,#0x0]
  0017215a: ldr r2,[r2,#0x30]
  0017215c: rsbs r2,r2
  0017215e: b 0x00172162
  00172160: movs r2,#0x0
  00172162: str.w r2,[r11,#0x4d0]
  00172166: add.w r8,sp,#0x8c
  0017216a: ldr r2,[0x001722e4]
  0017216c: movs r5,#0x0
  0017216e: add r2,pc
  00172170: ldr r6,[r2,#0x0]
  00172172: ldr r2,[0x001722e8]
  00172174: add r2,pc
  00172176: ldr r4,[r2,#0x0]
  00172178: b 0x001721c2
  0017217a: cmp r5,#0x9
  0017217c: bhi 0x001721c0
  0017217e: ldr r0,[r0,#0x4]
  00172180: ldr.w r1,[r0,r5,lsl #0x2]
  00172184: mov r0,r8
  00172186: blx 0x00074dd0
  0017218a: vldr.32 s0,[sp,#0x8c]
  0017218e: vcvt.s32.f32 s0,s0
  00172192: vmov r0,s0
  00172196: str.w r0,[r6,r5,lsl #0x2]
  0017219a: ldr.w r0,[r10,#0x0]
  0017219e: ldr r0,[r0,#0x4]
  001721a0: ldr.w r1,[r0,r5,lsl #0x2]
  001721a4: mov r0,r8
  001721a6: blx 0x00074dd0
  001721aa: vldr.32 s0,[sp,#0x90]
  001721ae: vcvt.s32.f32 s0,s0
  001721b2: vmov r0,s0
  001721b6: str.w r0,[r4,r5,lsl #0x2]
  001721ba: ldr.w r0,[r10,#0x0]
  001721be: ldr r1,[r0,#0x0]
  001721c0: adds r5,#0x1
  001721c2: cmp r5,r1
  001721c4: bcc 0x0017217a
  001721c6: movs r0,#0x1
  001721c8: strb.w r0,[r11,#0x282]
  001721cc: ldr r0,[sp,#0x9c]
  001721ce: ldr.w r1,[r9,#0x0]
  001721d2: subs r0,r1,r0
  001721d4: itttt eq
  001721d6: sub.eq.w r4,r7,#0x28
  001721da: mov.eq sp,r4
  001721dc: vpop.eq {d8}
  001721e0: add.eq sp,#0x4
  001721e2: itt eq
  001721e4: pop.eq.w {r8,r9,r10,r11}
  001721e8: pop.eq {r4,r5,r6,r7,pc}
  001721ea: blx 0x0006e824
```
