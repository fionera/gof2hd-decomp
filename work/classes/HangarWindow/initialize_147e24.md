# HangarWindow::initialize
elf 0x147e24 body 2838
Sig: undefined __stdcall initialize(void)

## decompile
```c

/* HangarWindow::initialize() */

void HangarWindow::initialize(void)

{
  short sVar1;
  HangarWindow *in_r0;
  Station *pSVar2;
  int iVar3;
  Status *this;
  HangarList *pHVar4;
  Array *pAVar5;
  Array *pAVar6;
  undefined4 uVar7;
  EVP_PKEY_CTX *ctx;
  TouchButton *pTVar8;
  int *piVar9;
  String *pSVar10;
  int *piVar11;
  HangarWindow *pHVar12;
  void *pvVar13;
  PaintCanvas *pPVar14;
  int *piVar15;
  Ship *pSVar16;
  uint *puVar17;
  uint uVar18;
  ListItemWindow *this_00;
  ChoiceWindow *pCVar19;
  HangarWindow HVar20;
  ushort uVar21;
  int *piVar22;
  uint *puVar23;
  char *pcVar24;
  int iVar25;
  uint uVar26;
  int *piVar27;
  int iVar28;
  int iVar29;
  uint *puVar30;
  code *pcVar31;
  uint uVar32;
  Item *this_01;
  undefined4 *puVar33;
  int *piVar34;
  int iVar35;
  uint in_fpscr;
  float fVar36;
  float fVar37;
  uint local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  float local_34;
  float local_30;
  int local_28;
  
  piVar34 = *(int **)(DAT_0015815c + 0x157e38);
  piVar27 = *(int **)(DAT_00158160 + 0x157e3a);
  local_28 = *piVar34;
  pSVar2 = (Station *)Status::getStation();
  iVar3 = Station::getIndex(pSVar2);
  this = (Status *)*piVar27;
  if (iVar3 == 0x6c) {
    HVar20 = (HangarWindow)(*(int *)(this + 0x114) == 3);
  }
  else {
    HVar20 = (HangarWindow)0x0;
  }
  in_r0[0x11d] = HVar20;
  Status::calcCargoPrices(this);
  pHVar4 = operator_new(0xc);
  HangarList::HangarList(pHVar4);
  *(HangarList **)(in_r0 + 0x14) = pHVar4;
  Status::getShip();
  pAVar5 = (Array *)Ship::getCargo();
  Status::getStation();
  pAVar6 = (Array *)Station::getItems();
  uVar7 = Item::mixItems(pAVar5,pAVar6);
  pHVar4 = *(HangarList **)(in_r0 + 0x14);
  *(undefined4 *)(in_r0 + 0x10) = uVar7;
  ctx = (EVP_PKEY_CTX *)Status::getShip();
  Status::getStation();
  Station::getShips();
  Status::getBluePrints((Status *)*piVar27);
  HangarList::init(pHVar4,ctx);
  pAVar5 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(Array **)(in_r0 + 4) = pAVar5;
  ArraySetLength<TouchButton*>(3,pAVar5);
  pTVar8 = operator_new(200);
  piVar9 = *(int **)(DAT_00158164 + 0x157ef8);
  pSVar10 = (String *)GameText::getText(*piVar9);
  piVar11 = *(int **)(DAT_00158168 + 0x157f0e);
  piVar22 = *(int **)(DAT_0015816c + 0x157f10);
  iVar28 = *piVar11;
  iVar3 = Layout::getHelpButtonOffset();
  TouchButton::TouchButton(pTVar8,pSVar10,3,iVar28 - iVar3,0,'\x12');
  *(TouchButton **)(*(int *)(*(int *)(in_r0 + 4) + 4) + 8) = pTVar8;
  pTVar8 = operator_new(200);
  pSVar10 = (String *)GameText::getText(*piVar9);
  iVar29 = *piVar11;
  iVar3 = Layout::getHelpButtonOffset();
  iVar28 = TouchButton::getWidth();
  TouchButton::TouchButton
            (pTVar8,pSVar10,3,((iVar29 - iVar3) - iVar28) + *(int *)(*piVar22 + 0x38),0,'\x12');
  *(TouchButton **)(*(int *)(*(int *)(in_r0 + 4) + 4) + 4) = pTVar8;
  pTVar8 = operator_new(200);
  pSVar10 = (String *)GameText::getText(*piVar9);
  iVar35 = *piVar11;
  iVar3 = Layout::getHelpButtonOffset();
  iVar28 = TouchButton::getWidth();
  iVar29 = TouchButton::getWidth();
  TouchButton::TouchButton
            (pTVar8,pSVar10,3,(((iVar35 - iVar3) - iVar28) - iVar29) + *(int *)(*piVar22 + 0x38) * 2
             ,0,'\x12');
  pHVar12 = *(HangarWindow **)(DAT_00158170 + 0x15800c);
  **(undefined4 **)(*(int *)(in_r0 + 4) + 4) = pTVar8;
  in_r0[0x11f] = *pHVar12;
  pvVar13 = operator_new__(0x18);
  iVar3 = 0;
  *(void **)(in_r0 + 0x30) = pvVar13;
  for (iVar28 = 0; iVar28 != 6; iVar28 = iVar28 + 1) {
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)**(undefined4 **)(DAT_00158174 + 0x15803a),(short)iVar28 + 0x232a,
               (uint *)(*(int *)(in_r0 + 0x30) + iVar3));
    iVar3 = iVar3 + 4;
  }
  iVar3 = *(int *)(DAT_00158178 + 0x15805a);
  iVar28 = *(int *)(DAT_0015817c + 0x158064);
  for (uVar32 = 0; uVar32 < **(uint **)(in_r0 + 4); uVar32 = uVar32 + 1) {
    if (uVar32 < 10) {
      TouchButton::getPosition();
      *(int *)(iVar3 + uVar32 * 4) = (int)local_34;
      TouchButton::getPosition();
      *(int *)(iVar28 + uVar32 * 4) = (int)local_30;
    }
  }
  puVar30 = *(uint **)(DAT_00158180 + 0x1580ba);
  pPVar14 = (PaintCanvas *)*puVar30;
  **(uint **)(DAT_00158184 + 0x1580bc) = **(uint **)(in_r0 + 4);
  AbyssEngine::PaintCanvas::Image2DCreate(pPVar14,0x52e,(uint *)(in_r0 + 0xe8));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar30,0x544,(uint *)(in_r0 + 0xec));
  pAVar5 = operator_new(0xc);
  Array<TouchButton*>::Array();
  pHVar12 = in_r0 + 0x24;
  *(Array **)pHVar12 = pAVar5;
  ArraySetLength<TouchButton*>(0x18,pAVar5);
  local_38 = 0xffffffff;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar30,0x470,&local_38);
  pTVar8 = operator_new(200);
  TouchButton::TouchButton(pTVar8,local_38,7,0,0,*(int *)(*piVar22 + 0x60),'\x11','\x04');
  **(undefined4 **)(*(int *)pHVar12 + 4) = pTVar8;
  iVar3 = Status::getCurrentCampaignMission();
  if (iVar3 == 0x4d) {
    pSVar2 = (Station *)Status::getStation();
    Station::getIndex(pSVar2);
  }
  pTVar8 = operator_new(200);
  pSVar10 = (String *)GameText::getText(*piVar9);
  TouchButton::TouchButton(pTVar8,pSVar10,7,0,0,'\x11');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 4) = pTVar8;
  pTVar8 = operator_new(200);
  pSVar10 = (String *)GameText::getText(*piVar9);
  TouchButton::TouchButton(pTVar8,pSVar10,7,0,0,'\x11');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 8) = pTVar8;
  local_3c = 0xffffffff;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar30,0x533,&local_3c);
  pTVar8 = operator_new(200);
  TouchButton::TouchButton(pTVar8,local_3c,7,0,0,*(int *)(*piVar22 + 100),'\x11','\x04');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0xc) = pTVar8;
  local_40 = 0xffffffff;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar30,0x532,&local_40);
  pTVar8 = operator_new(200);
  TouchButton::TouchButton(pTVar8,local_40,7,0,0,*(int *)(*piVar22 + 100),'\x11','\x04');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x10) = pTVar8;
  pTVar8 = operator_new(200);
  pSVar10 = (String *)GameText::getText(*piVar9);
  TouchButton::TouchButton(pTVar8,pSVar10,7,0,0,*(int *)(in_r0 + 0x110),'\x11','\x01');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x14) = pTVar8;
  pTVar8 = operator_new(200);
  pSVar10 = (String *)GameText::getText(*piVar9);
  TouchButton::TouchButton(pTVar8,pSVar10,7,0,0,*(int *)(in_r0 + 0x110),'\x11','\x01');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x18) = pTVar8;
  pTVar8 = operator_new(200);
  pSVar10 = (String *)GameText::getText(*piVar9);
  TouchButton::TouchButton(pTVar8,pSVar10,7,0,0,'\x11');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x1c) = pTVar8;
  pTVar8 = operator_new(200);
  AbyssEngine::String::String((String *)&local_34,(char *)(DAT_00158708 + 0x158336),false);
  TouchButton::TouchButton(pTVar8,(String *)&local_34,8,0,0,*(int *)(*piVar22 + 0x50),'\x11','\x04')
  ;
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x20) = pTVar8;
  AbyssEngine::String::~String((String *)&local_34);
  pTVar8 = operator_new(200);
  AbyssEngine::String::String((String *)&local_34,(char *)(DAT_0015870c + 0x158378),false);
  TouchButton::TouchButton(pTVar8,(String *)&local_34,9,0,0,*(int *)(*piVar22 + 0x50),'\x11','\x04')
  ;
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x24) = pTVar8;
  AbyssEngine::String::~String((String *)&local_34);
  pTVar8 = operator_new(200);
  pSVar10 = (String *)GameText::getText(*piVar9);
  TouchButton::TouchButton(pTVar8,pSVar10,7,0,0,*(int *)(*piVar22 + 0x50),'\x11','\x04');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x28) = pTVar8;
  pTVar8 = operator_new(200);
  Status::getCredits();
  Layout::formatCredits((int)&local_34);
  piVar15 = *(int **)(DAT_00158710 + 0x15840a);
  iVar28 = *piVar15;
  iVar29 = *piVar11;
  iVar3 = Layout::getFooterTransitionWidth();
  TouchButton::TouchButton(pTVar8,(String *)&local_34,0xb,iVar29,iVar28,iVar3,'\"','\x04');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x2c) = pTVar8;
  AbyssEngine::String::~String((String *)&local_34);
  iVar28 = 0xc;
  iVar3 = 0;
  while( true ) {
    HVar20 = in_r0[0x11f];
    pTVar8 = operator_new(200);
    if (4 < iVar28 - 0xcU) break;
    if (HVar20 == (HangarWindow)0x0) {
      AbyssEngine::String::String((String *)&local_34,(char *)(DAT_00158714 + 0x158456),false);
      TouchButton::TouchButton
                (pTVar8,(String *)&local_34,0,0,0,*(int *)(*piVar22 + 0x264),'\x11','\x01');
      *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + iVar28 * 4) = pTVar8;
      AbyssEngine::String::~String((String *)&local_34);
      iVar29 = iVar3 + 0xc;
    }
    else {
      AbyssEngine::String::String((String *)&local_34,(char *)(DAT_00158714 + 0x158456),false);
      TouchButton::TouchButton(pTVar8,(String *)&local_34,10,0,0,'\x01');
      *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + iVar28 * 4) = pTVar8;
      AbyssEngine::String::~String((String *)&local_34);
      iVar29 = iVar28;
    }
    TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + iVar29 * 4),false);
    iVar28 = iVar28 + 1;
    iVar3 = iVar3 + 1;
  }
  if (HVar20 == (HangarWindow)0x0) {
    AbyssEngine::String::String((String *)&local_34,(char *)(DAT_00158718 + 0x1584fa),false);
    TouchButton::TouchButton
              (pTVar8,(String *)&local_34,0,0,0,*(int *)(*piVar22 + 0x264),'\x11','\x01');
  }
  else {
    AbyssEngine::String::String((String *)&local_34,(char *)(DAT_00158718 + 0x1584fa),false);
    TouchButton::TouchButton(pTVar8,(String *)&local_34,10,0,0,'\x01');
  }
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x44) = pTVar8;
  AbyssEngine::String::~String((String *)&local_34);
  TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x44),false);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar30,0x233e,(uint *)(in_r0 + 0x34));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar30,0x233f,(uint *)(in_r0 + 0x38));
  pTVar8 = operator_new(200);
  AbyssEngine::String::String((String *)&local_34,(char *)(DAT_0015871c + 0x15858a),false);
  TouchButton::TouchButton(pTVar8,(String *)&local_34,7,0,0,'\x11');
  *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + 0x5c) = pTVar8;
  AbyssEngine::String::~String((String *)&local_34);
  local_34 = -NAN;
  local_44 = 0xffffffff;
  for (iVar3 = 0x12; iVar3 - 0x12U < 5; iVar3 = iVar3 + 1) {
    if (iVar3 == 0x12) {
      AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar30,0x233c,(uint *)&local_34);
      pPVar14 = (PaintCanvas *)*puVar30;
      uVar21 = 0x233d;
    }
    else {
      sVar1 = (short)(iVar3 - 0x12U);
      AbyssEngine::PaintCanvas::Image2DCreate
                ((PaintCanvas *)*puVar30,sVar1 * 2 + 0x2330,(uint *)&local_34);
      uVar21 = sVar1 * 2 + 0x2331;
      pPVar14 = (PaintCanvas *)*puVar30;
    }
    AbyssEngine::PaintCanvas::Image2DCreate(pPVar14,uVar21,&local_44);
    pTVar8 = operator_new(200);
    TouchButton::TouchButton(pTVar8,(uint)local_34,local_44,0x13,0,0,'\x01');
    *(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + iVar3 * 4) = pTVar8;
    TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar12 + 4) + iVar3 * 4),false);
  }
  uVar7 = TouchButton::getWidth();
  *(undefined4 *)(in_r0 + 0x120) = uVar7;
  iVar28 = TouchButton::getHeight(*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x30));
  iVar3 = DAT_001589f8;
  fVar36 = (float)VectorSignedToFloat(-iVar28,(byte)(in_fpscr >> 0x16) & 3);
  fVar37 = (float)VectorSignedToFloat(-*(int *)(in_r0 + 0x120),(byte)(in_fpscr >> 0x16) & 3);
  fVar36 = fVar36 * DAT_00158720;
  fVar37 = fVar37 * DAT_00158720;
  *(int *)(in_r0 + 0x124) = iVar28;
  *(int *)(in_r0 + 0x128) = (int)fVar37;
  *(int *)(in_r0 + 300) = (int)fVar36;
  pcVar31 = *(code **)(iVar3 + 0x158696);
  (*pcVar31)(*puVar30,0x475,in_r0 + 0x78);
  (*pcVar31)(*puVar30,0x476,in_r0 + 0x7c);
  (*pcVar31)(*puVar30,0x477,in_r0 + 0x74);
  uVar7 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar30);
  *(undefined4 *)(in_r0 + 0xdc) = uVar7;
  uVar7 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar30);
  *(undefined4 *)(in_r0 + 0xe0) = uVar7;
  if (((*(int *)(in_r0 + 0x10) != 0) && (iVar3 = Status::inBlackMarketSystem(), iVar3 == 0)) &&
     (in_r0[0x11d] == (HangarWindow)0x0)) {
    pSVar16 = (Ship *)Status::getShip();
    puVar17 = (uint *)Ship::getEquipment(pSVar16);
    uVar32 = 0;
    while( true ) {
      uVar18 = **(uint **)(in_r0 + 0x10);
      if (puVar17 == (uint *)0x0) {
        uVar26 = 0;
      }
      else {
        uVar26 = *puVar17;
      }
      if (uVar26 + uVar18 <= uVar32) break;
      puVar23 = *(uint **)(in_r0 + 0x10);
      if (uVar18 <= uVar32) {
        puVar23 = puVar17;
      }
      if (uVar18 > uVar32) {
        uVar18 = 0;
      }
      this_01 = *(Item **)(puVar23[1] + uVar18 * -4 + uVar32 * 4);
      if (this_01 != (Item *)0x0) {
        iVar3 = Item::getSinglePrice(this_01);
        iVar28 = Item::getIndex(this_01);
        iVar29 = *piVar27;
        iVar35 = *(int *)(*(int *)(iVar29 + 0x40) + 4);
        iVar25 = *(int *)(iVar35 + iVar28 * 4);
        if ((iVar25 < iVar3) || (iVar25 == 0)) {
          *(int *)(iVar35 + iVar28 * 4) = iVar3;
          Status::getSystem();
          uVar7 = SolarSystem::getIndex();
          iVar29 = *piVar27;
          *(undefined4 *)(*(int *)(*(int *)(iVar29 + 0x48) + 4) + iVar28 * 4) = uVar7;
        }
        iVar29 = *(int *)(*(int *)(iVar29 + 0x3c) + 4);
        iVar35 = *(int *)(iVar29 + iVar28 * 4);
        if ((iVar3 < iVar35) || (iVar35 == 0)) {
          *(int *)(iVar29 + iVar28 * 4) = iVar3;
          Status::getSystem();
          uVar7 = SolarSystem::getIndex();
          *(undefined4 *)(*(int *)(*(int *)(*piVar27 + 0x44) + 4) + iVar28 * 4) = uVar7;
        }
      }
      uVar32 = uVar32 + 1;
    }
  }
  pSVar16 = (Ship *)Status::getShip();
  Ship::adjustPrice(pSVar16);
  this_00 = operator_new(0x13c);
  ListItemWindow::ListItemWindow(this_00);
  *(ListItemWindow **)(in_r0 + 0x18) = this_00;
  pCVar19 = operator_new(0x5c);
  ChoiceWindow::ChoiceWindow(pCVar19);
  iVar29 = 0;
  *(undefined4 *)(in_r0 + 0x58) = 0;
  *(ChoiceWindow **)(in_r0 + 0x1c) = pCVar19;
  pCVar19 = operator_new(0x5c);
  ChoiceWindow::ChoiceWindow(pCVar19);
  in_r0[0x3c] = (HangarWindow)0x0;
  *(ChoiceWindow **)(in_r0 + 0x20) = pCVar19;
  in_r0[0x11c] = (HangarWindow)0x0;
  *(undefined4 *)(in_r0 + 0x6c) = 0;
  *(undefined4 *)(in_r0 + 0x70) = 0;
  *(undefined4 *)(in_r0 + 0x28) = 0;
  *(undefined4 *)(in_r0 + 0x2c) = 0;
  in_r0[0x130] = (HangarWindow)0x0;
  *(undefined2 *)(in_r0 + 0xd1) = 0;
  in_r0[0xb1] = (HangarWindow)0x0;
  *(undefined4 *)(in_r0 + 0xad) = 0;
  iVar28 = *piVar11;
  iVar3 = *piVar15;
  iVar35 = iVar28 + -10;
  *(undefined4 *)(in_r0 + 0x40) = 0x10;
  *(undefined4 *)(in_r0 + 0x44) = 5;
  *(undefined4 *)(in_r0 + 0x48) = 5;
  *(int *)(in_r0 + 0x4c) = iVar35;
  *(int *)(in_r0 + 0x50) = iVar3 + -10;
  piVar27 = operator_new__(0xc);
  iVar3 = DAT_001589fc;
  *(int **)(in_r0 + 0x54) = piVar27;
  puVar33 = *(undefined4 **)(iVar3 + 0x158848);
  iVar3 = __aeabi_idiv(iVar35,3);
  iVar3 = iVar3 + -2;
  *piVar27 = iVar3;
  piVar27[1] = iVar3;
  piVar27[2] = iVar28 + -0xe + iVar3 * -2;
  HangarList::setCurrentTab(*(int *)(in_r0 + 0x14),SUB41(*puVar33,0));
  refreshCurrentContentHeight(in_r0);
  Status::getShip();
  uVar7 = Ship::getCurrentLoad();
  iVar3 = DAT_00158a00;
  *(undefined4 *)(in_r0 + 0xa8) = uVar7;
  pcVar24 = *(char **)(iVar3 + 0x15888c);
  iVar3 = *piVar22;
  *(int *)(in_r0 + 0xd8) =
       (((*piVar15 - *(int *)(iVar3 + 0x10)) - *(int *)(iVar3 + 0xc)) - *(int *)(iVar3 + 0x20)) -
       *(int *)(iVar3 + 0x24);
  if ((*pcVar24 != '\0') && (**(char **)(DAT_00158a04 + 0x1588b2) == '\0')) {
    AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar30,0x6a4,(uint *)(in_r0 + 0xf0));
    fVar36 = (float)VectorSignedToFloat(*piVar11,(byte)(in_fpscr >> 0x16) & 3);
    iVar29 = (int)(fVar36 * DAT_001589f4);
  }
  *(undefined4 *)(in_r0 + 0x68) = 0;
  *(int *)(in_r0 + 0xf4) = iVar29;
  *(undefined2 *)(in_r0 + 0x88) = 0;
  in_r0[0xac] = (HangarWindow)0x0;
  *(undefined2 *)(in_r0 + 0x90) = 0;
  in_r0[0x92] = (HangarWindow)0x0;
  *(undefined4 *)(in_r0 + 0xc1) = 0;
  *(undefined4 *)(in_r0 + 0xc5) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(in_r0 + 0xc9) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(in_r0 + 0xcd) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(in_r0 + 0xe4) = 0;
  *in_r0 = (HangarWindow)0x0;
  in_r0[0xc] = (HangarWindow)0x1;
  in_r0[0xf8] = (HangarWindow)0x0;
  *(undefined4 *)(in_r0 + 0xfc) = 0xffffffff;
  *(undefined4 *)(in_r0 + 0xb4) = 0;
  *(undefined4 *)(in_r0 + 0xb8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(in_r0 + 0xbc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(in_r0 + 0xc0) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar3 = Status::getCurrentCampaignMission();
  if ((0xd < iVar3) && (iVar3 = *(int *)(DAT_00158a08 + 0x15893e), *(char *)(iVar3 + 0x4e) == '\0'))
  {
    pSVar10 = *(String **)(in_r0 + 0x20);
    GameText::getText(*piVar9);
    ChoiceWindow::set(pSVar10);
    iVar28 = DAT_00158a0c;
    *(undefined1 *)(iVar3 + 0x4e) = 1;
    RecordHandler::saveOptions((RecordHandler *)**(undefined4 **)(iVar28 + 0x158966));
    in_r0[0x3c] = (HangarWindow)0x1;
  }
  if (*piVar34 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00157e24: push {r4,r5,r6,r7,lr}
  00157e26: add r7,sp,#0xc
  00157e28: push {r8,r9,r10,r11}
  00157e2c: sub sp,#0x54
  00157e2e: mov r11,r0
  00157e30: ldr r0,[0x0015815c]
  00157e32: ldr r1,[0x00158160]
  00157e34: add r0,pc
  00157e36: add r1,pc
  00157e38: ldr.w r10,[r0,#0x0]
  00157e3c: ldr r4,[r1,#0x0]
  00157e3e: ldr.w r1,[r10,#0x0]
  00157e42: ldr r0,[r4,#0x0]
  00157e44: str r1,[sp,#0x50]
  00157e46: blx 0x00071c14
  00157e4a: blx 0x00071824
  00157e4e: mov r1,r0
  00157e50: ldr r0,[r4,#0x0]
  00157e52: cmp r1,#0x6c
  00157e54: bne 0x00157e64
  00157e56: ldr.w r1,[r0,#0x114]
  00157e5a: subs r1,#0x3
  00157e5c: clz r1,r1
  00157e60: lsrs r1,r1,#0x5
  00157e62: b 0x00157e66
  00157e64: movs r1,#0x0
  00157e66: strb.w r1,[r11,#0x11d]
  00157e6a: blx 0x00076ed0
  00157e6e: movs r0,#0xc
  00157e70: blx 0x0006eb24
  00157e74: mov r5,r0
  00157e76: blx 0x00076edc
  00157e7a: str.w r5,[r11,#0x14]
  00157e7e: ldr r0,[r4,#0x0]
  00157e80: blx 0x00071a58
  00157e84: blx 0x00073750
  00157e88: mov r5,r0
  00157e8a: ldr r0,[r4,#0x0]
  00157e8c: blx 0x00071c14
  00157e90: blx 0x00071830
  00157e94: mov r1,r0
  00157e96: mov r0,r5
  00157e98: blx 0x00076ee8
  00157e9c: ldr.w r8,[r11,#0x14]
  00157ea0: str.w r0,[r11,#0x10]
  00157ea4: ldr r0,[r4,#0x0]
  00157ea6: blx 0x00071a58
  00157eaa: mov r6,r0
  00157eac: ldr r0,[r4,#0x0]
  00157eae: ldr.w r9,[r11,#0x10]
  00157eb2: blx 0x00071c14
  00157eb6: blx 0x00073708
  00157eba: mov r5,r0
  00157ebc: ldr r0,[r4,#0x0]
  00157ebe: str r4,[sp,#0x20]
  00157ec0: blx 0x000748a8
  00157ec4: str r0,[sp,#0x0]
  00157ec6: mov r0,r8
  00157ec8: mov r1,r6
  00157eca: mov r2,r9
  00157ecc: mov r3,r5
  00157ece: blx 0x00076ef4
  00157ed2: movs r0,#0xc
  00157ed4: blx 0x0006eb24
  00157ed8: mov r5,r0
  00157eda: blx 0x00074f38
  00157ede: movs r0,#0x3
  00157ee0: mov r1,r5
  00157ee2: str.w r5,[r11,#0x4]
  00157ee6: blx 0x00074f44
  00157eea: movs r0,#0xc8
  00157eec: blx 0x0006eb24
  00157ef0: mov r5,r0
  00157ef2: ldr r0,[0x00158164]
  00157ef4: add r0,pc
  00157ef6: ldr r0,[r0,#0x0]
  00157ef8: str r0,[sp,#0x24]
  00157efa: ldr r0,[r0,#0x0]
  00157efc: mov.w r1,#0x110
  00157f00: blx 0x00072f70
  00157f04: mov r6,r0
  00157f06: ldr r0,[0x00158168]
  00157f08: ldr r1,[0x0015816c]
  00157f0a: add r0,pc
  00157f0c: add r1,pc
  00157f0e: ldr r0,[r0,#0x0]
  00157f10: ldr r1,[r1,#0x0]
  00157f12: str r0,[sp,#0x28]
  00157f14: ldr r4,[r0,#0x0]
  00157f16: ldr r0,[r1,#0x0]
  00157f18: str r1,[sp,#0x2c]
  00157f1a: blx 0x00075ac0
  00157f1e: subs r3,r4,r0
  00157f20: movs r1,#0x12
  00157f22: movs r2,#0x0
  00157f24: mov r0,r5
  00157f26: strd r2,r1,[sp,#0x0]
  00157f2a: mov r1,r6
  00157f2c: movs r2,#0x3
  00157f2e: blx 0x00074578
  00157f32: ldr.w r0,[r11,#0x4]
  00157f36: str.w r10,[sp,#0x18]
  00157f3a: ldr r0,[r0,#0x4]
  00157f3c: str r5,[r0,#0x8]
  00157f3e: movs r0,#0xc8
  00157f40: blx 0x0006eb24
  00157f44: ldr.w r10,[sp,#0x24]
  00157f48: mov r5,r0
  00157f4a: ldrb.w r2,[r11,#0x11d]
  00157f4e: movs r1,#0xba
  00157f50: ldr.w r0,[r10,#0x0]
  00157f54: cmp r2,#0x0
  00157f56: it eq
  00157f58: mov.eq r1,#0xb9
  00157f5a: blx 0x00072f70
  00157f5e: mov r8,r0
  00157f60: ldr r0,[sp,#0x28]
  00157f62: ldr r4,[r0,#0x0]
  00157f64: ldr r0,[sp,#0x2c]
  00157f66: ldr r0,[r0,#0x0]
  00157f68: blx 0x00075ac0
  00157f6c: mov r6,r0
  00157f6e: ldr.w r0,[r11,#0x4]
  00157f72: ldr r0,[r0,#0x4]
  00157f74: ldr r0,[r0,#0x8]
  00157f76: blx 0x00074da0
  00157f7a: ldr r1,[sp,#0x2c]
  00157f7c: ldr r1,[r1,#0x0]
  00157f7e: ldr r1,[r1,#0x38]
  00157f80: movs r2,#0x12
  00157f82: movs r3,#0x0
  00157f84: strd r3,r2,[sp,#0x0]
  00157f88: subs r2,r4,r6
  00157f8a: subs r0,r2,r0
  00157f8c: movs r2,#0x3
  00157f8e: adds r3,r0,r1
  00157f90: mov r0,r5
  00157f92: mov r1,r8
  00157f94: blx 0x00074578
  00157f98: ldr.w r0,[r11,#0x4]
  00157f9c: ldr r0,[r0,#0x4]
  00157f9e: str r5,[r0,#0x4]
  00157fa0: movs r0,#0xc8
  00157fa2: blx 0x0006eb24
  00157fa6: mov r9,r0
  00157fa8: ldr.w r0,[r10,#0x0]
  00157fac: movs r1,#0xb7
  00157fae: blx 0x00072f70
  00157fb2: mov r8,r0
  00157fb4: ldr r0,[sp,#0x28]
  00157fb6: ldr.w r10,[r0,#0x0]
  00157fba: ldr r0,[sp,#0x2c]
  00157fbc: ldr r0,[r0,#0x0]
  00157fbe: blx 0x00075ac0
  00157fc2: mov r6,r0
  00157fc4: ldr.w r0,[r11,#0x4]
  00157fc8: ldr r0,[r0,#0x4]
  00157fca: ldr r0,[r0,#0x8]
  00157fcc: blx 0x00074da0
  00157fd0: mov r5,r0
  00157fd2: ldr.w r0,[r11,#0x4]
  00157fd6: ldr r0,[r0,#0x4]
  00157fd8: ldr r0,[r0,#0x4]
  00157fda: blx 0x00074da0
  00157fde: ldr r1,[sp,#0x2c]
  00157fe0: ldr r1,[r1,#0x0]
  00157fe2: ldr r1,[r1,#0x38]
  00157fe4: movs r2,#0x12
  00157fe6: movs r3,#0x0
  00157fe8: strd r3,r2,[sp,#0x0]
  00157fec: sub.w r2,r10,r6
  00157ff0: subs r2,r2,r5
  00157ff2: subs r0,r2,r0
  00157ff4: movs r2,#0x3
  00157ff6: add.w r3,r0,r1, lsl #0x1
  00157ffa: mov r0,r9
  00157ffc: mov r1,r8
  00157ffe: blx 0x00074578
  00158002: ldr r0,[0x00158170]
  00158004: ldr.w r1,[r11,#0x4]
  00158008: add r0,pc
  0015800a: ldr r1,[r1,#0x4]
  0015800c: ldr r0,[r0,#0x0]
  0015800e: str.w r9,[r1,#0x0]
  00158012: ldrb r0,[r0,#0x0]
  00158014: strb.w r0,[r11,#0x11f]
  00158018: movs r0,#0x18
  0015801a: blx 0x0006ec08
  0015801e: ldr.w r10,[sp,#0x2c]
  00158022: movs r4,#0x0
  00158024: movw r5,#0x232a
  00158028: movs r6,#0x0
  0015802a: str.w r0,[r11,#0x30]
  0015802e: b 0x0015804a
  00158030: ldr r0,[0x00158174]
  00158032: ldr.w r1,[r11,#0x30]
  00158036: add r0,pc
  00158038: adds r2,r1,r4
  0015803a: adds r1,r6,r5
  0015803c: ldr r0,[r0,#0x0]
  0015803e: uxth r1,r1
  00158040: ldr r0,[r0,#0x0]
  00158042: blx 0x00071cf8
  00158046: adds r4,#0x4
  00158048: adds r6,#0x1
  0015804a: cmp r6,#0x6
  0015804c: bne 0x00158030
  0015804e: ldr r0,[0x00158178]
  00158050: add.w r8,sp,#0x44
  00158054: movs r6,#0x0
  00158056: add r0,pc
  00158058: ldr r4,[r0,#0x0]
  0015805a: ldr r0,[0x0015817c]
  0015805c: ldr.w r9,[sp,#0x24]
  00158060: add r0,pc
  00158062: ldr r5,[r0,#0x0]
  00158064: b 0x001580a8
  00158066: cmp r6,#0x9
  00158068: bhi 0x001580a6
  0015806a: ldr r0,[r0,#0x4]
  0015806c: ldr.w r1,[r0,r6,lsl #0x2]
  00158070: mov r0,r8
  00158072: blx 0x00074dd0
  00158076: vldr.32 s0,[sp,#0x44]
  0015807a: vcvt.s32.f32 s0,s0
  0015807e: vmov r0,s0
  00158082: str.w r0,[r4,r6,lsl #0x2]
  00158086: ldr.w r0,[r11,#0x4]
  0015808a: ldr r0,[r0,#0x4]
  0015808c: ldr.w r1,[r0,r6,lsl #0x2]
  00158090: mov r0,r8
  00158092: blx 0x00074dd0
  00158096: vldr.32 s0,[sp,#0x48]
  0015809a: vcvt.s32.f32 s0,s0
  0015809e: vmov r0,s0
  001580a2: str.w r0,[r5,r6,lsl #0x2]
  001580a6: adds r6,#0x1
  001580a8: ldr.w r0,[r11,#0x4]
  001580ac: ldr r1,[r0,#0x0]
  001580ae: cmp r6,r1
  001580b0: bcc 0x00158066
  001580b2: ldr r0,[0x00158180]
  001580b4: ldr r2,[0x00158184]
  001580b6: add r0,pc
  001580b8: add r2,pc
  001580ba: ldr r4,[r0,#0x0]
  001580bc: ldr r2,[r2,#0x0]
  001580be: ldr r0,[r4,#0x0]
  001580c0: str r1,[r2,#0x0]
  001580c2: add.w r2,r11,#0xe8
  001580c6: movw r1,#0x52e
  001580ca: blx 0x00071cf8
  001580ce: ldr r0,[r4,#0x0]
  001580d0: add.w r2,r11,#0xec
  001580d4: movw r1,#0x544
  001580d8: blx 0x00071cf8
  001580dc: movs r0,#0xc
  001580de: blx 0x0006eb24
  001580e2: mov r5,r0
  001580e4: blx 0x00074f38
  001580e8: mov r8,r11
  001580ea: movs r0,#0x18
  001580ec: mov r1,r5
  001580ee: str.w r5,[r8,#0x24]!
  001580f2: blx 0x00074f44
  001580f6: ldr r0,[r4,#0x0]
  001580f8: mov.w r1,#0xffffffff
  001580fc: add r2,sp,#0x40
  001580fe: str r1,[sp,#0x40]
  00158100: mov.w r1,#0x470
  00158104: blx 0x00071cf8
  00158108: movs r0,#0xc8
  0015810a: blx 0x0006eb24
  0015810e: mov r5,r0
  00158110: ldr.w r0,[r10,#0x0]
  00158114: ldr r1,[sp,#0x40]
  00158116: ldr r0,[r0,#0x60]
  00158118: movs r6,#0x0
  0015811a: movs r2,#0x4
  0015811c: movs r3,#0x11
  0015811e: strd r6,r0,[sp,#0x0]
  00158122: strd r3,r2,[sp,#0x8]
  00158126: mov r0,r5
  00158128: movs r2,#0x7
  0015812a: movs r3,#0x0
  0015812c: blx 0x00074db8
  00158130: ldr r6,[sp,#0x20]
  00158132: ldr.w r0,[r8,#0x0]
  00158136: str.w r8,[sp,#0x30]
  0015813a: ldr r0,[r0,#0x4]
  0015813c: str r5,[r0,#0x0]
  0015813e: ldr r0,[r6,#0x0]
  00158140: blx 0x00071770
  00158144: cmp r0,#0x4d
  00158146: bne 0x00158188
  00158148: ldr r0,[r6,#0x0]
  0015814a: blx 0x00071c14
  0015814e: blx 0x00071824
  00158152: subs r0,#0x64
  00158154: clz r0,r0
  00158158: lsrs r6,r0,#0x5
  0015815a: b 0x0015818a
  00158188: movs r6,#0x0
  0015818a: movs r0,#0xc8
  0015818c: blx 0x0006eb24
  00158190: mov r5,r0
  00158192: ldrb.w r0,[r11,#0x11d]
  00158196: movw r1,#0x12d
  0015819a: cmp r0,#0x0
  0015819c: it ne
  0015819e: mov.ne.w r1,#0x14c
  001581a2: cmp r6,#0x0
  001581a4: it ne
  001581a6: mov.ne.w r1,#0x14c
  001581aa: ldr.w r0,[r9,#0x0]
  001581ae: blx 0x00072f70
  001581b2: mov r1,r0
  001581b4: movs r0,#0x11
  001581b6: movs r2,#0x0
  001581b8: strd r2,r0,[sp,#0x0]
  001581bc: mov r0,r5
  001581be: movs r2,#0x7
  001581c0: movs r3,#0x0
  001581c2: blx 0x00074578
  001581c6: ldr r0,[sp,#0x30]
  001581c8: ldr r0,[r0,#0x0]
  001581ca: ldr r0,[r0,#0x4]
  001581cc: str r5,[r0,#0x4]
  001581ce: movs r0,#0xc8
  001581d0: blx 0x0006eb24
  001581d4: mov r5,r0
  001581d6: ldr.w r0,[r9,#0x0]
  001581da: mov.w r1,#0x12e
  001581de: blx 0x00072f70
  001581e2: mov r1,r0
  001581e4: movs r0,#0x11
  001581e6: movs r6,#0x0
  001581e8: strd r6,r0,[sp,#0x0]
  001581ec: mov r0,r5
  001581ee: movs r2,#0x7
  001581f0: movs r3,#0x0
  001581f2: blx 0x00074578
  001581f6: ldr.w r8,[sp,#0x30]
  001581fa: add r2,sp,#0x3c
  001581fc: mov.w r10,#0xffffffff
  00158200: movw r1,#0x533
  00158204: ldr.w r0,[r8,#0x0]
  00158208: ldr r0,[r0,#0x4]
  0015820a: str r5,[r0,#0x8]
  0015820c: ldr r0,[r4,#0x0]
  0015820e: str.w r10,[sp,#0x3c]
  00158212: blx 0x00071cf8
  00158216: movs r0,#0xc8
  00158218: blx 0x0006eb24
  0015821c: ldr.w r9,[sp,#0x2c]
  00158220: mov r5,r0
  00158222: ldr r1,[sp,#0x3c]
  00158224: ldr.w r0,[r9,#0x0]
  00158228: ldr r0,[r0,#0x64]
  0015822a: str r6,[sp,#0x0]
  0015822c: movs r6,#0x4
  0015822e: str r0,[sp,#0x4]
  00158230: movs r0,#0x11
  00158232: strd r0,r6,[sp,#0x8]
  00158236: mov r0,r5
  00158238: movs r2,#0x7
  0015823a: movs r3,#0x0
  0015823c: blx 0x00074db8
  00158240: ldr.w r0,[r8,#0x0]
  00158244: add r2,sp,#0x38
  00158246: movw r1,#0x532
  0015824a: str r4,[sp,#0x1c]
  0015824c: ldr r0,[r0,#0x4]
  0015824e: str r5,[r0,#0xc]
  00158250: ldr r0,[r4,#0x0]
  00158252: str.w r10,[sp,#0x38]
  00158256: blx 0x00071cf8
  0015825a: movs r0,#0xc8
  0015825c: blx 0x0006eb24
  00158260: mov r5,r0
  00158262: ldr.w r0,[r9,#0x0]
  00158266: ldr r1,[sp,#0x38]
  00158268: ldr r0,[r0,#0x64]
  0015826a: movs r3,#0x0
  0015826c: movs r2,#0x11
  0015826e: strd r3,r0,[sp,#0x0]
  00158272: mov r0,r5
  00158274: strd r2,r6,[sp,#0x8]
  00158278: movs r2,#0x7
  0015827a: movs r3,#0x0
  0015827c: blx 0x00074db8
  00158280: ldr.w r0,[r8,#0x0]
  00158284: ldr r0,[r0,#0x4]
  00158286: str r5,[r0,#0x10]
  00158288: movs r0,#0xc8
  0015828a: blx 0x0006eb24
  0015828e: ldr r4,[sp,#0x24]
  00158290: mov r5,r0
  00158292: ldr r0,[r4,#0x0]
  00158294: movw r1,#0x117
  00158298: blx 0x00072f70
  0015829c: mov r1,r0
  0015829e: ldr.w r0,[r11,#0x110]
  001582a2: movs r6,#0x0
  001582a4: movs r2,#0x1
  001582a6: movs r3,#0x11
  001582a8: strd r6,r0,[sp,#0x0]
  001582ac: strd r3,r2,[sp,#0x8]
  001582b0: mov r0,r5
  001582b2: movs r2,#0x7
  001582b4: movs r3,#0x0
  001582b6: blx 0x00074f50
  001582ba: ldr r0,[sp,#0x30]
  001582bc: ldr r0,[r0,#0x0]
  001582be: ldr r0,[r0,#0x4]
  001582c0: str r5,[r0,#0x14]
  001582c2: movs r0,#0xc8
  001582c4: blx 0x0006eb24
  001582c8: mov r5,r0
  001582ca: ldr r0,[r4,#0x0]
  001582cc: mov.w r1,#0x11a
  001582d0: blx 0x00072f70
  001582d4: mov r1,r0
  001582d6: ldr.w r0,[r11,#0x110]
  001582da: movs r6,#0x0
  001582dc: movs r2,#0x1
  001582de: movs r3,#0x11
  001582e0: strd r6,r0,[sp,#0x0]
  001582e4: strd r3,r2,[sp,#0x8]
  001582e8: mov r0,r5
  001582ea: movs r2,#0x7
  001582ec: movs r3,#0x0
  001582ee: blx 0x00074f50
  001582f2: ldr r0,[sp,#0x30]
  001582f4: ldr r0,[r0,#0x0]
  001582f6: ldr r0,[r0,#0x4]
  001582f8: str r5,[r0,#0x18]
  001582fa: movs r0,#0xc8
  001582fc: blx 0x0006eb24
  00158300: mov r5,r0
  00158302: ldr r0,[r4,#0x0]
  00158304: movw r1,#0x11b
  00158308: blx 0x00072f70
  0015830c: mov r1,r0
  0015830e: movs r0,#0x11
  00158310: movs r2,#0x0
  00158312: strd r2,r0,[sp,#0x0]
  00158316: mov r0,r5
  00158318: movs r2,#0x7
  0015831a: movs r3,#0x0
  0015831c: blx 0x00074578
  00158320: ldr r0,[sp,#0x30]
  00158322: ldr r0,[r0,#0x0]
  00158324: ldr r0,[r0,#0x4]
  00158326: str r5,[r0,#0x1c]
  00158328: movs r0,#0xc8
  0015832a: blx 0x0006eb24
  0015832e: ldr r1,[0x00158708]
  00158330: mov r5,r0
  00158332: add r1,pc
  00158334: add r0,sp,#0x44
  00158336: movs r2,#0x0
  00158338: movs r6,#0x0
  0015833a: blx 0x0006ee18
  0015833e: ldr.w r0,[r9,#0x0]
  00158342: ldr r0,[r0,#0x50]
  00158344: movs r1,#0x4
  00158346: movs r2,#0x11
  00158348: strd r6,r0,[sp,#0x0]
  0015834c: mov r0,r5
  0015834e: strd r2,r1,[sp,#0x8]
  00158352: add r1,sp,#0x44
  00158354: movs r2,#0x8
  00158356: movs r3,#0x0
  00158358: blx 0x00074f50
  0015835c: ldr r0,[sp,#0x30]
  0015835e: ldr r0,[r0,#0x0]
  00158360: ldr r0,[r0,#0x4]
  00158362: str r5,[r0,#0x20]
  00158364: add r0,sp,#0x44
  00158366: blx 0x0006ee30
  0015836a: movs r0,#0xc8
  0015836c: blx 0x0006eb24
  00158370: ldr r1,[0x0015870c]
  00158372: mov r5,r0
  00158374: add r1,pc
  00158376: add r0,sp,#0x44
  00158378: movs r2,#0x0
  0015837a: movs r6,#0x0
  0015837c: blx 0x0006ee18
  00158380: ldr.w r0,[r9,#0x0]
  00158384: ldr r0,[r0,#0x50]
  00158386: movs r1,#0x4
  00158388: movs r2,#0x11
  0015838a: strd r6,r0,[sp,#0x0]
  0015838e: mov r0,r5
  00158390: strd r2,r1,[sp,#0x8]
  00158394: add r1,sp,#0x44
  00158396: movs r2,#0x9
  00158398: movs r3,#0x0
  0015839a: blx 0x00074f50
  0015839e: ldr r0,[sp,#0x30]
  001583a0: ldr r0,[r0,#0x0]
  001583a2: ldr r0,[r0,#0x4]
  001583a4: str r5,[r0,#0x24]
  001583a6: add r0,sp,#0x44
  001583a8: blx 0x0006ee30
  001583ac: movs r0,#0xc8
  001583ae: blx 0x0006eb24
  001583b2: mov r5,r0
  001583b4: ldr r0,[r4,#0x0]
  001583b6: mov.w r1,#0x14a
  001583ba: blx 0x00072f70
  001583be: mov r1,r0
  001583c0: ldr.w r0,[r9,#0x0]
  001583c4: ldr r0,[r0,#0x50]
  001583c6: movs r6,#0x0
  001583c8: movs r2,#0x4
  001583ca: movs r3,#0x11
  001583cc: strd r6,r0,[sp,#0x0]
  001583d0: strd r3,r2,[sp,#0x8]
  001583d4: mov r0,r5
  001583d6: movs r2,#0x7
  001583d8: movs r3,#0x0
  001583da: blx 0x00074f50
  001583de: ldr r4,[sp,#0x30]
  001583e0: ldr r0,[r4,#0x0]
  001583e2: ldr r0,[r0,#0x4]
  001583e4: str r5,[r0,#0x28]
  001583e6: movs r0,#0xc8
  001583e8: blx 0x0006eb24
  001583ec: mov r8,r0
  001583ee: ldr r0,[sp,#0x20]
  001583f0: ldr.w r6,[r9,#0x0]
  001583f4: ldr r0,[r0,#0x0]
  001583f6: blx 0x000733d8
  001583fa: mov r2,r0
  001583fc: add r0,sp,#0x44
  001583fe: mov r1,r6
  00158400: blx 0x00074e54
  00158404: ldr r0,[0x00158710]
  00158406: add r0,pc
  00158408: ldr r0,[r0,#0x0]
  0015840a: str r0,[sp,#0x14]
  0015840c: ldr r5,[r0,#0x0]
  0015840e: ldr r0,[sp,#0x28]
  00158410: ldr r6,[r0,#0x0]
  00158412: ldr.w r0,[r9,#0x0]
  00158416: blx 0x00076f00
  0015841a: movs r1,#0x4
  0015841c: movs r2,#0x22
  0015841e: strd r5,r0,[sp,#0x0]
  00158422: mov r0,r8
  00158424: strd r2,r1,[sp,#0x8]
  00158428: add r1,sp,#0x44
  0015842a: movs r2,#0xb
  0015842c: mov r3,r6
  0015842e: blx 0x00074f50
  00158432: ldr r0,[r4,#0x0]
  00158434: add r6,sp,#0x44
  00158436: ldr r0,[r0,#0x4]
  00158438: str.w r8,[r0,#0x2c]
  0015843c: mov r0,r6
  0015843e: blx 0x0006ee30
  00158442: mov.w r10,#0xc
  00158446: mov.w r8,#0x0
  0015844a: b 0x001584da
  0015844c: ldr r1,[0x00158714]
  0015844e: cmp.w r9,#0x0
  00158452: add r1,pc
  00158454: beq 0x00158486
  00158456: mov r0,r6
  00158458: movs r2,#0x0
  0015845a: blx 0x0006ee18
  0015845e: ldr r4,[sp,#0x30]
  00158460: movs r0,#0x0
  00158462: mov r1,r6
  00158464: str r0,[sp,#0x0]
  00158466: movs r0,#0x1
  00158468: str r0,[sp,#0x4]
  0015846a: mov r0,r5
  0015846c: movs r2,#0xa
  0015846e: movs r3,#0x0
  00158470: blx 0x00074578
  00158474: ldr r0,[r4,#0x0]
  00158476: ldr r0,[r0,#0x4]
  00158478: str.w r5,[r0,r10,lsl #0x2]
  0015847c: mov r0,r6
  0015847e: blx 0x0006ee30
  00158482: mov r0,r10
  00158484: b 0x001584c4
  00158486: mov r0,r6
  00158488: movs r2,#0x0
  0015848a: blx 0x0006ee18
  0015848e: ldr r4,[sp,#0x30]
  00158490: ldr r0,[sp,#0x2c]
  00158492: ldr r0,[r0,#0x0]
  00158494: ldr.w r0,[r0,#0x264]
  00158498: movs r1,#0x0
  0015849a: movs r2,#0x0
  0015849c: strd r1,r0,[sp,#0x0]
  001584a0: movs r0,#0x11
  001584a2: str r0,[sp,#0x8]
  001584a4: movs r0,#0x1
  001584a6: str r0,[sp,#0xc]
  001584a8: mov r0,r5
  001584aa: mov r1,r6
  001584ac: movs r3,#0x0
  001584ae: blx 0x00074f50
  001584b2: ldr r0,[r4,#0x0]
  001584b4: ldr r0,[r0,#0x4]
  001584b6: str.w r5,[r0,r10,lsl #0x2]
  001584ba: mov r0,r6
  001584bc: blx 0x0006ee30
  001584c0: add.w r0,r8,#0xc
  001584c4: ldr r1,[r4,#0x0]
  001584c6: ldr r1,[r1,#0x4]
  001584c8: ldr.w r0,[r1,r0,lsl #0x2]
  001584cc: movs r1,#0x0
  001584ce: blx 0x00074e48
  001584d2: add.w r10,r10,#0x1
  001584d6: add.w r8,r8,#0x1
  001584da: movs r0,#0xc8
  001584dc: ldrb.w r9,[r11,#0x11f]
  001584e0: sub.w r4,r10,#0xc
  001584e4: blx 0x0006eb24
  001584e8: mov r5,r0
  001584ea: cmp r4,#0x5
  001584ec: bcc 0x0015844c
  001584ee: ldr r4,[sp,#0x1c]
  001584f0: cmp.w r9,#0x0
  001584f4: ldr r1,[0x00158718]
  001584f6: add r1,pc
  001584f8: beq 0x00158518
  001584fa: add r0,sp,#0x44
  001584fc: movs r2,#0x0
  001584fe: movs r6,#0x0
  00158500: blx 0x0006ee18
  00158504: movs r0,#0x1
  00158506: add r1,sp,#0x44
  00158508: strd r6,r0,[sp,#0x0]
  0015850c: mov r0,r5
  0015850e: movs r2,#0xa
  00158510: movs r3,#0x0
  00158512: blx 0x00074578
  00158516: b 0x00158542
  00158518: add r0,sp,#0x44
  0015851a: movs r2,#0x0
  0015851c: movs r6,#0x0
  0015851e: blx 0x0006ee18
  00158522: ldr r0,[sp,#0x2c]
  00158524: ldr r0,[r0,#0x0]
  00158526: ldr.w r0,[r0,#0x264]
  0015852a: movs r1,#0x1
  0015852c: movs r2,#0x11
  0015852e: strd r6,r0,[sp,#0x0]
  00158532: mov r0,r5
  00158534: strd r2,r1,[sp,#0x8]
  00158538: add r1,sp,#0x44
  0015853a: movs r2,#0x0
  0015853c: movs r3,#0x0
  0015853e: blx 0x00074f50
  00158542: ldr r0,[sp,#0x30]
  00158544: ldr r0,[r0,#0x0]
  00158546: ldr r0,[r0,#0x4]
  00158548: str r5,[r0,#0x44]
  0015854a: add r0,sp,#0x44
  0015854c: blx 0x0006ee30
  00158550: ldr.w r0,[r11,#0x24]
  00158554: movs r1,#0x0
  00158556: movs r6,#0x0
  00158558: ldr r0,[r0,#0x4]
  0015855a: ldr r0,[r0,#0x44]
  0015855c: blx 0x00074e48
  00158560: ldr r0,[r4,#0x0]
  00158562: add.w r2,r11,#0x34
  00158566: movw r1,#0x233e
  0015856a: blx 0x00071cf8
  0015856e: ldr r0,[r4,#0x0]
  00158570: add.w r2,r11,#0x38
  00158574: movw r1,#0x233f
  00158578: blx 0x00071cf8
  0015857c: movs r0,#0xc8
  0015857e: blx 0x0006eb24
  00158582: ldr r1,[0x0015871c]
  00158584: mov r5,r0
  00158586: add r1,pc
  00158588: add r0,sp,#0x44
  0015858a: movs r2,#0x0
  0015858c: blx 0x0006ee18
  00158590: movs r0,#0x11
  00158592: add r1,sp,#0x44
  00158594: strd r6,r0,[sp,#0x0]
  00158598: mov r0,r5
  0015859a: movs r2,#0x7
  0015859c: movs r3,#0x0
  0015859e: blx 0x00074578
  001585a2: ldr r0,[sp,#0x30]
  001585a4: ldr r0,[r0,#0x0]
  001585a6: ldr r0,[r0,#0x4]
  001585a8: str r5,[r0,#0x5c]
  001585aa: add r0,sp,#0x44
  001585ac: blx 0x0006ee30
  001585b0: add.w r9,sp,#0x34
  001585b4: mov.w r0,#0xffffffff
  001585b8: movs r6,#0x12
  001585ba: mov.w r10,#0x1
  001585be: mov.w r8,#0x0
  001585c2: str r0,[sp,#0x44]
  001585c4: str r0,[sp,#0x34]
  001585c6: b 0x00158636
  001585c8: ldr r0,[r4,#0x0]
  001585ca: cmp r6,#0x12
  001585cc: bne 0x001585e0
  001585ce: add r2,sp,#0x44
  001585d0: movw r1,#0x233c
  001585d4: blx 0x00071cf8
  001585d8: ldr r0,[r4,#0x0]
  001585da: movw r1,#0x233d
  001585de: b 0x001585fc
  001585e0: movw r1,#0x2330
  001585e4: add r2,sp,#0x44
  001585e6: add.w r1,r1,r5, lsl #0x1
  001585ea: uxth r1,r1
  001585ec: blx 0x00071cf8
  001585f0: movw r0,#0x2331
  001585f4: add.w r0,r0,r5, lsl #0x1
  001585f8: uxth r1,r0
  001585fa: ldr r0,[r4,#0x0]
  001585fc: mov r2,r9
  001585fe: blx 0x00071cf8
  00158602: movs r0,#0xc8
  00158604: blx 0x0006eb24
  00158608: ldr r1,[sp,#0x44]
  0015860a: mov r5,r0
  0015860c: ldr r2,[sp,#0x34]
  0015860e: movs r3,#0x13
  00158610: strd r8,r8,[sp,#0x0]
  00158614: str.w r10,[sp,#0x8]
  00158618: blx 0x000764d4
  0015861c: ldr r1,[sp,#0x30]
  0015861e: ldr r0,[r1,#0x0]
  00158620: ldr r0,[r0,#0x4]
  00158622: str.w r5,[r0,r6,lsl #0x2]
  00158626: ldr r0,[r1,#0x0]
  00158628: movs r1,#0x0
  0015862a: ldr r0,[r0,#0x4]
  0015862c: ldr.w r0,[r0,r6,lsl #0x2]
  00158630: blx 0x00074e48
  00158634: adds r6,#0x1
  00158636: sub.w r5,r6,#0x12
  0015863a: cmp r5,#0x5
  0015863c: bcc 0x001585c8
  0015863e: ldr.w r0,[r11,#0x24]
  00158642: ldr r0,[r0,#0x4]
  00158644: ldr r0,[r0,#0x30]
  00158646: blx 0x00074da0
  0015864a: ldr.w r1,[r11,#0x24]
  0015864e: str.w r0,[r11,#0x120]
  00158652: ldr r0,[r1,#0x4]
  00158654: ldr r0,[r0,#0x30]
  00158656: blx 0x00076f0c
  0015865a: ldr.w r1,[r11,#0x120]
  0015865e: rsbs r2,r0
  00158660: vldr.32 s4,[pc,#0xbc]
  00158664: mov r5,r4
  00158666: rsbs r1,r1
  00158668: vmov s0,r2
  0015866c: add.w r2,r11,#0x78
  00158670: vmov s2,r1
  00158674: vcvt.f32.s32 s0,s0
  00158678: vcvt.f32.s32 s2,s2
  0015867c: vmul.f32 s0,s0,s4
  00158680: vmul.f32 s2,s2,s4
  00158684: vcvt.s32.f32 s0,s0
  00158688: vcvt.s32.f32 s2,s2
  0015868c: ldr r1,[0x001589f8]
  0015868e: str.w r0,[r11,#0x124]
  00158692: add r1,pc
  00158694: vstr.32 s2,[r11,#0x128]
  00158698: vstr.32 s0,[r11,#0x12c]
  0015869c: ldr r0,[r4,#0x0]
  0015869e: ldr r4,[r1,#0x0]
  001586a0: movw r1,#0x475
  001586a4: blx r4
  001586a6: ldr r0,[r5,#0x0]
  001586a8: add.w r2,r11,#0x7c
  001586ac: movw r1,#0x476
  001586b0: blx r4
  001586b2: ldr r0,[r5,#0x0]
  001586b4: add.w r2,r11,#0x74
  001586b8: movw r1,#0x477
  001586bc: blx r4
  001586be: ldr.w r1,[r11,#0x7c]
  001586c2: ldr r0,[r5,#0x0]
  001586c4: blx 0x00072d84
  001586c8: ldr.w r1,[r11,#0x7c]
  001586cc: str.w r0,[r11,#0xdc]
  001586d0: ldr r0,[r5,#0x0]
  001586d2: blx 0x00072d90
  001586d6: ldr.w r1,[r11,#0x10]
  001586da: ldr r4,[sp,#0x20]
  001586dc: cmp r1,#0x0
  001586de: str.w r0,[r11,#0xe0]
  001586e2: beq 0x001587b0
  001586e4: ldr r0,[r4,#0x0]
  001586e6: blx 0x00071998
  001586ea: cmp r0,#0x0
  001586ec: bne 0x001587b0
  001586ee: ldrb.w r0,[r11,#0x11d]
  001586f2: cmp r0,#0x0
  001586f4: bne 0x001587b0
  001586f6: ldr r0,[r4,#0x0]
  001586f8: blx 0x00071a58
  001586fc: blx 0x0007333c
  00158700: mov r8,r0
  00158702: movs r5,#0x0
  00158704: b 0x00158798
  00158724: cmp r5,r0
  00158726: it cs
  00158728: mov.cs r1,r8
  0015872a: ldr r1,[r1,#0x4]
  0015872c: it cc
  0015872e: mov.cc r0,#0x0
  00158730: sub.w r0,r1,r0, lsl #0x2
  00158734: ldr.w r6,[r0,r5,lsl #0x2]
  00158738: cbz r6,0x00158796
  0015873a: mov r0,r6
  0015873c: blx 0x00071944
  00158740: mov r9,r0
  00158742: mov r0,r6
  00158744: blx 0x000718d8
  00158748: mov r6,r0
  0015874a: ldr r0,[r4,#0x0]
  0015874c: ldr r1,[r0,#0x40]
  0015874e: ldr r1,[r1,#0x4]
  00158750: ldr.w r2,[r1,r6,lsl #0x2]
  00158754: cmp r9,r2
  00158756: bgt 0x0015875a
  00158758: cbnz r2,0x00158772
  0015875a: str.w r9,[r1,r6,lsl #0x2]
  0015875e: blx 0x00071a10
  00158762: blx 0x00071a7c
  00158766: mov r1,r0
  00158768: ldr r0,[r4,#0x0]
  0015876a: ldr r2,[r0,#0x48]
  0015876c: ldr r2,[r2,#0x4]
  0015876e: str.w r1,[r2,r6,lsl #0x2]
  00158772: ldr r1,[r0,#0x3c]
  00158774: ldr r1,[r1,#0x4]
  00158776: ldr.w r2,[r1,r6,lsl #0x2]
  0015877a: cmp r9,r2
  0015877c: blt 0x00158780
  0015877e: cbnz r2,0x00158796
  00158780: str.w r9,[r1,r6,lsl #0x2]
  00158784: blx 0x00071a10
  00158788: blx 0x00071a7c
  0015878c: ldr r1,[r4,#0x0]
  0015878e: ldr r1,[r1,#0x44]
  00158790: ldr r1,[r1,#0x4]
  00158792: str.w r0,[r1,r6,lsl #0x2]
  00158796: adds r5,#0x1
  00158798: ldr.w r1,[r11,#0x10]
  0015879c: cmp.w r8,#0x0
  001587a0: ldr r0,[r1,#0x0]
  001587a2: ite ne
  001587a4: ldr.ne.w r2,[r8,#0x0]
  001587a8: mov.eq r2,#0x0
  001587aa: add r2,r0
  001587ac: cmp r5,r2
  001587ae: bcc 0x00158724
  001587b0: ldr r0,[r4,#0x0]
  001587b2: blx 0x00071a58
  001587b6: blx 0x000719f8
  001587ba: mov.w r0,#0x13c
  001587be: blx 0x0006eb24
  001587c2: mov r5,r0
  001587c4: blx 0x00076f18
  001587c8: movs r0,#0x5c
  001587ca: str.w r5,[r11,#0x18]
  001587ce: blx 0x0006eb24
  001587d2: mov r6,r0
  001587d4: blx 0x00074584
  001587d8: movs r5,#0x0
  001587da: movs r0,#0x5c
  001587dc: str.w r5,[r11,#0x58]
  001587e0: str.w r6,[r11,#0x1c]
  001587e4: blx 0x0006eb24
  001587e8: mov r6,r0
  001587ea: blx 0x00074584
  001587ee: strb.w r5,[r11,#0x3c]
  001587f2: movs r0,#0x10
  001587f4: str.w r6,[r11,#0x20]
  001587f8: movs r1,#0x5
  001587fa: strb.w r5,[r11,#0x11c]
  001587fe: strd r5,r5,[r11,#0x6c]
  00158802: strd r5,r5,[r11,#0x28]
  00158806: ldr r2,[sp,#0x28]
  00158808: strb.w r5,[r11,#0x130]
  0015880c: strh.w r5,[r11,#0xd1]
  00158810: strb.w r5,[r11,#0xb1]
  00158814: str.w r5,[r11,#0xad]
  00158818: ldr.w r10,[sp,#0x14]
  0015881c: ldr r4,[r2,#0x0]
  0015881e: ldr.w r2,[r10,#0x0]
  00158822: sub.w r8,r4,#0xa
  00158826: strd r0,r1,[r11,#0x40]
  0015882a: movs r0,#0xc
  0015882c: subs r2,#0xa
  0015882e: strd r1,r8,[r11,#0x48]
  00158832: str.w r2,[r11,#0x50]
  00158836: blx 0x0006ec08
  0015883a: mov r6,r0
  0015883c: ldr r0,[0x001589fc]
  0015883e: movs r1,#0x3
  00158840: str.w r6,[r11,#0x54]
  00158844: add r0,pc
  00158846: ldr.w r9,[r0,#0x0]
  0015884a: mov r0,r8
  0015884c: blx 0x0007198c
  00158850: subs r0,#0x2
  00158852: sub.w r1,r4,#0xe
  00158856: subs r1,r1,r0
  00158858: movs r2,#0x0
  0015885a: subs r1,r1,r0
  0015885c: strd r0,r0,[r6,#0x0]
  00158860: str r1,[r6,#0x8]
  00158862: ldr.w r0,[r11,#0x14]
  00158866: ldr.w r1,[r9,#0x0]
  0015886a: blx 0x00076f24
  0015886e: mov r0,r11
  00158870: blx 0x00076f30
  00158874: ldr r0,[sp,#0x20]
  00158876: ldr r0,[r0,#0x0]
  00158878: blx 0x00071a58
  0015887c: blx 0x00072bf8
  00158880: ldr r1,[0x00158a00]
  00158882: str.w r0,[r11,#0xa8]
  00158886: ldr r0,[sp,#0x2c]
  00158888: add r1,pc
  0015888a: ldr.w r2,[r10,#0x0]
  0015888e: ldr r1,[r1,#0x0]
  00158890: ldr r0,[r0,#0x0]
  00158892: ldrd r3,r6,[r0,#0xc]
  00158896: subs r2,r2,r6
  00158898: ldrd r4,r0,[r0,#0x20]
  0015889c: subs r2,r2,r3
  0015889e: subs r2,r2,r4
  001588a0: subs r0,r2,r0
  001588a2: str.w r0,[r11,#0xd8]
  001588a6: ldrb r0,[r1,#0x0]
  001588a8: cbz r0,0x001588e2
  001588aa: ldr r0,[0x00158a04]
  001588ac: ldr r6,[sp,#0x18]
  001588ae: add r0,pc
  001588b0: ldr r0,[r0,#0x0]
  001588b2: ldrb r0,[r0,#0x0]
  001588b4: cbnz r0,0x001588e4
  001588b6: ldr r0,[sp,#0x1c]
  001588b8: add.w r2,r11,#0xf0
  001588bc: movw r1,#0x6a4
  001588c0: ldr r0,[r0,#0x0]
  001588c2: blx 0x00071cf8
  001588c6: ldr r0,[sp,#0x28]
  001588c8: vldr.32 s2,[pc,#0x128]
  001588cc: vldr.32 s0,[r0]
  001588d0: vcvt.f32.s32 s0,s0
  001588d4: vmul.f32 s0,s0,s2
  001588d8: vcvt.s32.f32 s0,s0
  001588dc: vmov r5,s0
  001588e0: b 0x001588e4
  001588e2: ldr r6,[sp,#0x18]
  001588e4: vmov.i32 q8,#0x0
  001588e8: add.w r1,r11,#0xc1
  001588ec: movs r0,#0x0
  001588ee: str.w r0,[r11,#0x68]
  001588f2: str.w r5,[r11,#0xf4]
  001588f6: strh.w r0,[r11,#0x88]
  001588fa: strb.w r0,[r11,#0xac]
  001588fe: strh.w r0,[r11,#0x90]
  00158902: strb.w r0,[r11,#0x92]
  00158906: vst1.8 {d16,d17},[r1]
  0015890a: movs r1,#0x1
  0015890c: str.w r0,[r11,#0xe4]
  00158910: strb.w r0,[r11,#0x0]
  00158914: strb.w r1,[r11,#0xc]
  00158918: strb.w r0,[r11,#0xf8]
  0015891c: mov.w r0,#0xffffffff
  00158920: str.w r0,[r11,#0xfc]
  00158924: add.w r0,r11,#0xb4
  00158928: vst1.32 {d16,d17},[r0]
  0015892c: ldr r0,[sp,#0x20]
  0015892e: ldr r0,[r0,#0x0]
  00158930: blx 0x00071770
  00158934: cmp r0,#0xe
  00158936: blt 0x00158970
  00158938: ldr r0,[0x00158a08]
  0015893a: add r0,pc
  0015893c: ldr r4,[r0,#0x0]
  0015893e: ldrb.w r0,[r4,#0x4e]
  00158942: cbnz r0,0x00158970
  00158944: ldr r0,[sp,#0x24]
  00158946: movs r1,#0x6d
  00158948: ldr.w r5,[r11,#0x20]
  0015894c: ldr r0,[r0,#0x0]
  0015894e: blx 0x00072f70
  00158952: mov r1,r0
  00158954: mov r0,r5
  00158956: blx 0x00074794
  0015895a: ldr r0,[0x00158a0c]
  0015895c: movs r5,#0x1
  0015895e: strb.w r5,[r4,#0x4e]
  00158962: add r0,pc
  00158964: ldr r0,[r0,#0x0]
  00158966: ldr r0,[r0,#0x0]
  00158968: blx 0x00071788
  0015896c: strb.w r5,[r11,#0x3c]
  00158970: ldr r0,[sp,#0x50]
  00158972: ldr r1,[r6,#0x0]
  00158974: subs r0,r1,r0
  00158976: ittt eq
  00158978: add.eq sp,#0x54
  0015897a: pop.eq.w {r8,r9,r10,r11}
  0015897e: pop.eq {r4,r5,r6,r7,pc}
  00158980: blx 0x0006e824
```
