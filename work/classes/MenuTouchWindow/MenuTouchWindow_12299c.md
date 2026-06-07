# MenuTouchWindow::MenuTouchWindow
elf 0x12299c body 8784
Sig: undefined __thiscall MenuTouchWindow(MenuTouchWindow * this, int param_1)

## decompile
```c

/* MenuTouchWindow::MenuTouchWindow(int) */

void __thiscall MenuTouchWindow::MenuTouchWindow(MenuTouchWindow *this,int param_1)

{
  undefined4 uVar1;
  bool bVar2;
  int iVar3;
  void *pvVar4;
  String *pSVar5;
  TouchButton *pTVar6;
  char *pcVar7;
  void *pvVar8;
  char *pcVar9;
  Array *pAVar10;
  uint *puVar11;
  ScrollTouchWindow *pSVar12;
  Globals *pGVar13;
  int iVar14;
  Array<TouchSlider*> *this_00;
  TouchSlider *pTVar15;
  String *pSVar16;
  ChoiceWindow *this_01;
  ushort uVar17;
  int *piVar18;
  MenuTouchWindow *pMVar19;
  int *piVar20;
  undefined4 *puVar21;
  float *pfVar22;
  int *piVar23;
  undefined4 uVar24;
  int iVar25;
  float *pfVar26;
  int iVar27;
  uint uVar28;
  uint *puVar29;
  int *piVar30;
  int *piVar31;
  undefined4 uVar32;
  uint uVar33;
  uint uVar34;
  code *pcVar35;
  char *pcVar36;
  code *pcVar37;
  int *piVar38;
  int iVar39;
  int iVar40;
  int iVar41;
  uint in_fpscr;
  float fVar42;
  float fVar43;
  float fVar44;
  float fVar45;
  double dVar46;
  double dVar47;
  double dVar48;
  double dVar49;
  int local_234;
  String aSStack_218 [12];
  String aSStack_20c [12];
  String aSStack_200 [12];
  String aSStack_1f4 [12];
  String aSStack_1e8 [12];
  String aSStack_1dc [12];
  String aSStack_1d0 [12];
  String aSStack_1c4 [12];
  String aSStack_1b8 [12];
  String aSStack_1ac [12];
  uint local_1a0;
  uint local_19c;
  String aSStack_198 [12];
  String aSStack_18c [12];
  String aSStack_180 [12];
  String aSStack_174 [12];
  String aSStack_168 [12];
  String aSStack_15c [12];
  String aSStack_150 [12];
  String aSStack_144 [12];
  String aSStack_138 [12];
  String aSStack_12c [4];
  float local_128;
  uint local_124;
  float local_120;
  float local_11c;
  undefined4 local_118;
  String aSStack_114 [12];
  String aSStack_108 [12];
  String aSStack_fc [12];
  String aSStack_f0 [12];
  String aSStack_e4 [12];
  String aSStack_d8 [12];
  String aSStack_cc [12];
  String aSStack_c0 [12];
  String aSStack_b4 [12];
  String aSStack_a8 [12];
  String aSStack_9c [12];
  String aSStack_90 [12];
  String aSStack_84 [12];
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  int local_3c;
  
  piVar38 = *(int **)(DAT_00132c34 + 0x1329b8);
  piVar18 = *(int **)(DAT_00132c38 + 0x1329ba);
  iVar3 = *piVar38;
  local_3c = *piVar18;
  uVar32 = *(undefined4 *)(iVar3 + 0x298);
  uVar24 = *(undefined4 *)(iVar3 + 0x29c);
  uVar1 = *(undefined4 *)(iVar3 + 0x2a0);
  pMVar19 = this + 0x1a8;
  *(undefined4 *)pMVar19 = *(undefined4 *)(iVar3 + 0x294);
  *(undefined4 *)(this + 0x1ac) = uVar32;
  *(undefined4 *)(this + 0x1b0) = uVar24;
  *(undefined4 *)(this + 0x1b4) = uVar1;
  *(undefined4 *)(this + 0x1b8) = *(undefined4 *)(iVar3 + 0x2a4);
  *(undefined4 *)(this + 0x1bc) = *(undefined4 *)(iVar3 + 0x2a8);
  *(undefined4 *)(this + 0x1c0) = *(undefined4 *)(iVar3 + 0x2ac);
  uVar32 = *(undefined4 *)(iVar3 + 0x2b0);
  *(int *)(this + 0x168) = param_1;
  pvVar4 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(void **)(this + 4) = pvVar4;
  pvVar4 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(void **)(this + 0xc0) = pvVar4;
  *(undefined4 *)(this + 0xe4) = 0;
  *(undefined4 *)(this + 0xe8) = 0;
  *(undefined4 *)(this + 0xb8) = 0;
  *(undefined4 *)(this + 0xb0) = 0;
  *(undefined4 *)(this + 0xb4) = 0;
  *this = (MenuTouchWindow)0x0;
  *(undefined4 *)(this + 0x120) = 0xffffffff;
  *(undefined4 *)(this + 0x18c) = 0;
  this[0x1c4] = (MenuTouchWindow)0x0;
  *(undefined4 *)(this + 0x134) = 0;
  *(undefined4 *)(this + 0x138) = 0;
  *(undefined4 *)(this + 0x1e0) = 0;
  *(undefined4 *)(this + 0xf0) = 0;
  *(undefined4 *)(this + 0xf4) = 0;
  *(undefined4 *)(this + 0xf8) = 0;
  *(undefined4 *)(this + 0x230) = 0;
  *(undefined4 *)(this + 0x234) = 0;
  this[0x238] = (MenuTouchWindow)0x0;
  *(undefined4 *)(this + 0x9c) = 0;
  *(undefined4 *)(this + 0xa0) = 0;
  *(undefined4 *)(this + 0xbc) = 0;
  loadPreviewRecords(this);
  if (param_1 == 0) {
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)**(undefined4 **)(DAT_00132c44 + 0x132b8e),0x1b5a,
               (uint *)(this + 0x120));
    piVar30 = *(int **)(DAT_00132c48 + 0x132b9c);
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_48,pSVar5,false);
    addButton(this,0,aSStack_48,0,*(undefined4 *)(this + 4),uVar32);
    AbyssEngine::String::~String(aSStack_48);
    iVar3 = **(int **)(DAT_00132c4c + 0x132bd2);
    if (iVar3 < 0) {
LAB_00132ea4:
      iVar25 = 1;
    }
    else {
      iVar25 = 1;
      if ((*(int *)(this + 0xbc) != 0) && (iVar3 < **(int **)(DAT_00132c50 + 0x132bec))) {
        if (*(int *)(*(int *)(*(int *)(this + 0xbc) + 4) + iVar3 * 4) == 0) goto LAB_00132ea4;
        pSVar5 = (String *)GameText::getText(*piVar30);
        AbyssEngine::String::String(aSStack_54,pSVar5,false);
        addButton(this,0xb,aSStack_54,1,*(undefined4 *)(this + 4),uVar32);
        AbyssEngine::String::~String(aSStack_54);
        iVar25 = 2;
      }
    }
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_60,pSVar5,false);
    addButton(this,1,aSStack_60,iVar25,*(undefined4 *)(this + 4),uVar32);
    AbyssEngine::String::~String(aSStack_60);
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_6c,pSVar5,false);
    addButton(this,3,aSStack_6c,iVar25 + 1,*(undefined4 *)(this + 4),uVar32);
    iVar3 = iVar25 + 2;
    AbyssEngine::String::~String(aSStack_6c);
    if (**(char **)(DAT_00132f88 + 0x132f0e) != '\0') {
      pSVar5 = (String *)GameText::getText(*piVar30);
      AbyssEngine::String::String(aSStack_78,pSVar5,false);
      addButton(this,0x19,aSStack_78,iVar3,*(undefined4 *)(this + 4),uVar32);
      AbyssEngine::String::~String(aSStack_78);
      iVar3 = iVar25 + 3;
    }
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_84,pSVar5,false);
    addButton(this,4,aSStack_84,iVar3,*(undefined4 *)(this + 4),uVar32);
    pSVar16 = aSStack_84;
LAB_00132fba:
    AbyssEngine::String::~String(pSVar16);
LAB_00132fbe:
    if (*(char *)(*piVar38 + 0x285) != '\0') {
      pTVar6 = operator_new(200);
      pSVar5 = (String *)GameText::getText(**(int **)(DAT_001333b8 + 0x132fd6));
      TouchButton::TouchButton
                (pTVar6,pSVar5,0,**(int **)(DAT_001333bc + 0x132fe8) - *(int *)(*piVar38 + 0x2c),
                 **(int **)(DAT_001333c0 + 0x132fee) - *(int *)(*piVar38 + 0x2c),'\"');
      *(undefined4 *)pTVar6 = 0x11;
      *(undefined4 *)(pTVar6 + 4) = 0;
      ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
    }
    bVar2 = true;
  }
  else {
    if (param_1 != 1) {
      if (param_1 == 2) {
        piVar30 = *(int **)(DAT_00132c3c + 0x132a88);
        pSVar5 = (String *)GameText::getText(*piVar30);
        AbyssEngine::String::String(aSStack_90,pSVar5,false);
        addButton(this,0,aSStack_90,0,*(undefined4 *)(this + 4),0);
        AbyssEngine::String::~String(aSStack_90);
        pSVar5 = (String *)GameText::getText(*piVar30);
        AbyssEngine::String::String(aSStack_9c,pSVar5,false);
        addButton(this,1,aSStack_9c,1,*(undefined4 *)(this + 4),0);
        AbyssEngine::String::~String(aSStack_9c);
        pSVar5 = (String *)GameText::getText(*piVar30);
        AbyssEngine::String::String(aSStack_a8,pSVar5,false);
        addButton(this,2,aSStack_a8,2,*(undefined4 *)(this + 4),0);
        AbyssEngine::String::~String(aSStack_a8);
        pSVar5 = (String *)GameText::getText(*piVar30);
        AbyssEngine::String::String(aSStack_b4,pSVar5,false);
        addButton(this,3,aSStack_b4,3,*(undefined4 *)(this + 4),0);
        AbyssEngine::String::~String(aSStack_b4);
        if (**(char **)(DAT_00132c40 + 0x132b48) == '\0') {
          uVar32 = 4;
        }
        else {
          pSVar5 = (String *)GameText::getText(*piVar30);
          AbyssEngine::String::String(aSStack_c0,pSVar5,false);
          addButton(this,0x19,aSStack_c0,4,*(undefined4 *)(this + 4),0);
          AbyssEngine::String::~String(aSStack_c0);
          uVar32 = 5;
        }
        pSVar5 = (String *)GameText::getText(*piVar30);
        AbyssEngine::String::String(aSStack_cc,pSVar5,false);
        addButton(this,4,aSStack_cc,uVar32,*(undefined4 *)(this + 4),0);
        pSVar16 = aSStack_cc;
        goto LAB_00132fba;
      }
      goto LAB_00132fbe;
    }
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)**(undefined4 **)(DAT_00132f74 + 0x132c5e),0x534,
               (uint *)(this + 0x120));
    piVar30 = *(int **)(DAT_00132f78 + 0x132c6c);
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_d8,pSVar5,false);
    addButton(this,3,aSStack_d8,0,*(undefined4 *)(this + 4),0);
    AbyssEngine::String::~String(aSStack_d8);
    iVar3 = Status::getCurrentCampaignMission();
    if ((iVar3 < 0x10) || (iVar3 = Status::inAlienOrbit(), iVar3 != 0)) {
LAB_00132cce:
      iVar3 = 1;
    }
    else {
      Status::getMission();
      iVar3 = Mission::getType();
      if (iVar3 == 0xb7) goto LAB_00132cce;
      pSVar5 = (String *)GameText::getText(*piVar30);
      AbyssEngine::String::String(aSStack_e4,pSVar5,false);
      addButton(this,10,aSStack_e4,1,*(undefined4 *)(this + 4),0);
      AbyssEngine::String::~String(aSStack_e4);
      iVar3 = 2;
    }
    iVar25 = Status::getCurrentCampaignMission();
    if (1 < iVar25) {
      Status::getMission();
      iVar25 = Mission::getType();
      if (iVar25 != 0xb7) {
        pSVar5 = (String *)GameText::getText(*piVar30);
        AbyssEngine::String::String(aSStack_f0,pSVar5,false);
        addButton(this,0xc,aSStack_f0,iVar3,*(undefined4 *)(this + 4),0);
        AbyssEngine::String::~String(aSStack_f0);
        iVar3 = iVar3 + 1;
      }
    }
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_fc,pSVar5,false);
    addButton(this,6,aSStack_fc,iVar3,*(undefined4 *)(this + 4),0);
    AbyssEngine::String::~String(aSStack_fc);
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_108,pSVar5,false);
    addButton(this,0x13,aSStack_108,iVar3 + 1,*(undefined4 *)(this + 4),0);
    AbyssEngine::String::~String(aSStack_108);
    iVar25 = Status::getCurrentCampaignMission();
    if ((iVar25 - 0x9aU < 5) && ((1 << (iVar25 - 0x9aU & 0xff) & 0x19U) != 0)) {
      pSVar5 = (String *)GameText::getText(*piVar30);
      AbyssEngine::String::String(aSStack_114,pSVar5,false);
      addButton(this,0x12,aSStack_114,iVar3 + 2,*(undefined4 *)(this + 4),0);
      AbyssEngine::String::~String(aSStack_114);
    }
    pTVar6 = operator_new(200);
    pSVar5 = (String *)GameText::getText(*piVar30);
    piVar31 = *(int **)(DAT_00132f80 + 0x132df0);
    piVar23 = *(int **)(DAT_00132f84 + 0x132df2);
    TouchButton::TouchButton
              (pTVar6,pSVar5,0,*piVar31 - *(int *)(*piVar38 + 0x2c),
               *piVar23 - *(int *)(*piVar38 + 0x2c),'\"');
    *(undefined4 *)pTVar6 = 0x14;
    *(undefined4 *)(pTVar6 + 4) = 0;
    ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
    pTVar6 = operator_new(200);
    pSVar5 = (String *)GameText::getText(*piVar30);
    iVar27 = *piVar31;
    iVar25 = *(int *)(*piVar38 + 0x2c);
    iVar3 = TouchButton::getWidth();
    bVar2 = false;
    TouchButton::TouchButton
              (pTVar6,pSVar5,0,(iVar27 + iVar25 * -2) - iVar3,*piVar23 - *(int *)(*piVar38 + 0x2c),
               '\"');
    *(undefined4 *)pTVar6 = 0x15;
    *(undefined4 *)(pTVar6 + 4) = 0;
    ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
    TouchButton::setPosition
              (pTVar6,*piVar31 - *(int *)(*piVar38 + 0x2c),*piVar23 - *(int *)(*piVar38 + 0x2c));
  }
  pTVar6 = operator_new(200);
  piVar30 = *(int **)(DAT_001333c4 + 0x13302a);
  pSVar5 = (String *)GameText::getText(*piVar30);
  piVar31 = *(int **)(DAT_001333c8 + 0x13303e);
  piVar23 = *(int **)(DAT_001333cc + 0x133040);
  TouchButton::TouchButton
            (pTVar6,pSVar5,0,*piVar31 - *(int *)(*piVar38 + 0x2c),
             *piVar23 - *(int *)(*piVar38 + 0x2c),'\"');
  *(undefined4 *)pTVar6 = 0x16;
  *(undefined4 *)(pTVar6 + 4) = 0;
  ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
  if (*(int *)(this + 4) == 0) {
    local_120 = 0.0;
    local_11c = 0.0;
    local_118 = 0;
    fVar43 = DAT_001333d0;
  }
  else {
    TouchButton::getPosition();
    fVar43 = local_120;
  }
  uVar24 = *(undefined4 *)pMVar19;
  pTVar6 = operator_new(200);
  AbyssEngine::String::String(aSStack_12c,(char *)(DAT_001333d4 + 0x1330b4),false);
  uVar32 = __aeabi_idiv(uVar24,3);
  fVar42 = (float)VectorSignedToFloat(uVar24,(byte)(in_fpscr >> 0x16) & 3);
  fVar44 = (float)VectorSignedToFloat(uVar32,(byte)(in_fpscr >> 0x16) & 3);
  TouchButton::TouchButton
            (pTVar6,aSStack_12c,0x11,(int)(fVar43 + fVar42 + fVar44),(int)local_11c,'D');
  AbyssEngine::String::~String(aSStack_12c);
  *(undefined4 *)pTVar6 = 0x17;
  *(undefined4 *)(pTVar6 + 4) = 0;
  ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
  fVar43 = local_120;
  uVar32 = *(undefined4 *)(this + 0x1a8);
  pTVar6 = operator_new(200);
  AbyssEngine::String::String(aSStack_12c,(char *)(DAT_001333d8 + 0x13312a),false);
  uVar32 = __aeabi_idiv(uVar32,3);
  fVar42 = (float)VectorSignedToFloat(uVar32,(byte)(in_fpscr >> 0x16) & 3);
  TouchButton::TouchButton(pTVar6,aSStack_12c,0x11,(int)(fVar43 - fVar42),(int)local_11c,'D');
  AbyssEngine::String::~String(aSStack_12c);
  *(undefined4 *)pTVar6 = 0x18;
  *(undefined4 *)(pTVar6 + 4) = 0;
  ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
  pTVar6 = operator_new(200);
  pSVar5 = (String *)GameText::getText(*piVar30);
  iVar25 = *(int *)(*piVar38 + 0x2c);
  iVar27 = *piVar31;
  iVar3 = TouchButton::getWidth();
  TouchButton::TouchButton
            (pTVar6,pSVar5,0,(iVar27 + iVar25 * -2) - iVar3,*piVar23 - *(int *)(*piVar38 + 0x2c),
             '\"');
  *(undefined4 *)pTVar6 = 0x6d;
  *(undefined4 *)(pTVar6 + 4) = 0;
  ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
  pTVar6 = operator_new(200);
  pSVar5 = (String *)GameText::getText(*piVar30);
  iVar27 = *piVar31;
  iVar39 = *(int *)(*piVar38 + 0x2c);
  iVar3 = TouchButton::getWidth();
  iVar25 = TouchButton::getWidth();
  TouchButton::TouchButton
            (pTVar6,pSVar5,0,((iVar39 * -3 + iVar27) - iVar3) - iVar25,
             *piVar23 - *(int *)(*piVar38 + 0x2c),'\"');
  *(undefined4 *)pTVar6 = 0x6e;
  *(undefined4 *)(pTVar6 + 4) = 0;
  ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
  pvVar4 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(void **)(this + 0xac) = pvVar4;
  pSVar5 = (String *)GameText::getText(*piVar30);
  AbyssEngine::String::String(aSStack_138,pSVar5,false);
  addButton(this,8,aSStack_138,0,*(undefined4 *)(this + 0xac),0);
  AbyssEngine::String::~String(aSStack_138);
  pSVar5 = (String *)GameText::getText(*piVar30);
  AbyssEngine::String::String(aSStack_144,pSVar5,false);
  addButton(this,9,aSStack_144,1,*(undefined4 *)(this + 0xac),0);
  AbyssEngine::String::~String(aSStack_144);
  if (bVar2) {
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_150,pSVar5,false);
    addButton(this,0x19,aSStack_150,2,*(undefined4 *)(this + 0xac),0);
    AbyssEngine::String::~String(aSStack_150);
    pvVar4 = operator_new(0xc);
    Array<TouchButton*>::Array();
    *(void **)(this + 0xb0) = pvVar4;
    uVar33 = 0;
    fVar43 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar38 + 0x34),
                                        (byte)(in_fpscr >> 0x16) & 3);
    pcVar7 = *(char **)(DAT_001333e0 + 0x133328);
    if (*pcVar7 == '\0') {
      fVar43 = fVar43 * DAT_001333dc;
    }
    for (; uVar33 < 10; uVar33 = uVar33 + 1) {
      iVar39 = *(int *)(*piVar38 + 0x30);
      iVar27 = *(int *)(*piVar38 + 0x29c);
      iVar40 = *piVar23 / 2 - *(int *)(this + 0x1ac);
      iVar14 = *piVar31;
      iVar41 = (int)fVar43 + 2 + iVar39;
      iVar3 = (int)(iVar14 + ((uint)(iVar14 >> 0x1f) >> 0x1e)) >> 2;
      iVar25 = iVar14 - iVar3;
      if ((uVar33 & 1) == 0) {
        iVar25 = iVar3;
      }
      fVar44 = (float)VectorSignedToFloat(*(int *)(this + 0x1a8),(byte)(in_fpscr >> 0x16) & 3);
      fVar42 = (float)VectorSignedToFloat(iVar25,(byte)(in_fpscr >> 0x16) & 3);
      iVar3 = (int)(fVar42 + fVar44 * -0.5);
      if (*pcVar7 == '\0') {
        local_234 = iVar41 * (uVar33 >> 1) + ((iVar40 + -0xc) - iVar27);
      }
      else {
        local_234 = iVar41 * uVar33 + ((iVar40 - iVar27) - iVar39);
        iVar3 = iVar14 / 2 - *(int *)(this + 0x1a8) / 2;
      }
      puVar29 = *(uint **)(DAT_001336d8 + 0x133412);
      uVar28 = **(uint **)(DAT_001336d0 + 0x13340e);
      uVar34 = *puVar29;
      iVar25 = AbyssEngine::PaintCanvas::FontGetSpacing(**(uint **)(DAT_001336d4 + 0x133410));
      iVar27 = *(int *)pMVar19;
      pTVar6 = operator_new(200);
      pSVar5 = (String *)GameText::getText(*piVar30);
      if ((uVar33 | 1) == 9) {
        uVar28 = uVar34;
      }
      TouchButton::TouchButton(pTVar6,pSVar5,0,iVar3,local_234,iVar27,'\x11','\x04',uVar28,iVar25);
      iVar3 = *(int *)(DAT_001336e0 + 0x133482 + uVar33 * 4);
      *(int *)pTVar6 = iVar3;
      *(int *)(pTVar6 + 4) = iVar3 >> 0x1f;
      if ((uVar33 | 1) == 9) {
        *(uint *)(pTVar6 + 8) = *puVar29;
      }
      ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xb0));
    }
  }
  puVar29 = *(uint **)(DAT_001336e4 + 0x1334be);
  pcVar35 = *(code **)(DAT_001336e8 + 0x1334c0);
  (*pcVar35)(*puVar29,0x515,this + 0x10c);
  (*pcVar35)(*puVar29,0x516,this + 0x110);
  (*pcVar35)(*puVar29,0x517,this + 0x114);
  (*pcVar35)(*puVar29,0x518,this + 0x118);
  pvVar4 = operator_new__(0x14);
  *(void **)(this + 0x134) = pvVar4;
  pvVar8 = operator_new__(0x14);
  *(void **)(this + 0x138) = pvVar8;
  (*pcVar35)(*puVar29,0xbba,pvVar4);
  (*pcVar35)(*puVar29,0xbbc,*(int *)(this + 0x134) + 4);
  (*pcVar35)(*puVar29,0xbc3,*(int *)(this + 0x134) + 8);
  (*pcVar35)(*puVar29,0xbc5,*(int *)(this + 0x134) + 0xc);
  (*pcVar35)(*puVar29,0xbc7,*(int *)(this + 0x134) + 0x10);
  (*pcVar35)(*puVar29,0xbbb,*(undefined4 *)(this + 0x138));
  pcVar7 = *(char **)(DAT_001336ec + 0x133564);
  if (*pcVar7 == '\0') {
    uVar17 = 0xbbd;
  }
  else {
    uVar17 = 0xbc6;
  }
  AbyssEngine::PaintCanvas::Image2DCreate
            ((PaintCanvas *)*puVar29,uVar17,(uint *)(*(int *)(this + 0x138) + 4));
  (*pcVar35)(*puVar29,0xbc4,*(int *)(this + 0x138) + 8);
  (*pcVar35)(*puVar29,0xbc6,*(int *)(this + 0x138) + 0xc);
  (*pcVar35)(*puVar29,0xbc8,*(int *)(this + 0x138) + 0x10);
  pcVar9 = *(char **)(DAT_001336f0 + 0x1335b0);
  if ((*pcVar7 != '\0') && (*pcVar9 == '\0')) {
    AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar29,0xc1d,(uint *)(this + 0x14c));
  }
  (*pcVar35)(*puVar29,3000,this + 0x13c);
  (*pcVar35)(*puVar29,0xbb9,this + 0x140);
  (*pcVar35)(*puVar29,0xbbe,this + 0x144);
  (*pcVar35)(*puVar29,0xbbf,this + 0x148);
  uVar24 = 0;
  this[0x1d9] = (MenuTouchWindow)0x0;
  uVar32 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar29);
  *(undefined4 *)(this + 0x1e4) = uVar32;
  uVar32 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar29);
  *(undefined4 *)(this + 0x1e8) = uVar32;
  iVar3 = *piVar38;
  *(int *)(this + 0x1ec) = *(int *)(iVar3 + 0x28) + *(int *)(iVar3 + 0x2c);
  *(int *)(this + 0x1f0) = *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc);
  if ((*pcVar7 == '\0') || (*pcVar9 != '\0')) {
    uVar24 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar29);
  }
  *(undefined4 *)(this + 500) = uVar24;
  uVar32 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar29);
  *(undefined4 *)(this + 0x1f8) = uVar32;
  uVar32 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar29);
  *(undefined4 *)(this + 0x1fc) = uVar32;
  iVar3 = *piVar38;
  iVar25 = *(int *)(this + 0x1e8) + *(int *)(iVar3 + 0x2c) * 2;
  *(int *)(this + 0x200) = iVar25;
  *(int *)(this + 0x204) =
       ((*piVar31 - iVar25) + *(int *)(iVar3 + 0x28) * -2) - *(int *)(this + 500);
  pAVar10 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(Array **)(this + 0xf8) = pAVar10;
  ArraySetLength<TouchButton*>(5,pAVar10);
  puVar11 = *(uint **)(this + 0xf8);
  *(undefined4 *)(this + 0x208) = *(undefined4 *)(*piVar38 + 0x2c);
  for (uVar33 = 0; uVar33 < *puVar11; uVar33 = uVar33 + 1) {
    pTVar6 = operator_new(200);
    TouchButton::TouchButton
              (pTVar6,*(uint *)(*(int *)(this + 0x134) + uVar33 * 4),0x10,*(int *)(this + 0x1ec),
               (*(int *)(this + 0x208) + *(int *)(this + 0x1e4)) * uVar33 + *(int *)(this + 0x1f0),
               '\x11');
    *(TouchButton **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar33 * 4) = pTVar6;
    puVar11 = *(uint **)(this + 0xf8);
    piVar20 = *(int **)(puVar11[1] + uVar33 * 4);
    *piVar20 = uVar33 + 0x36;
    piVar20[1] = 0;
  }
  iVar3 = *piVar38;
  iVar25 = (((((*piVar23 - *(int *)(iVar3 + 0xc)) - *(int *)(iVar3 + 0x20)) - *(int *)(this + 0x1fc)
             ) - *(int *)(iVar3 + 0x2c)) - *(int *)(iVar3 + 0x10)) - *(int *)(iVar3 + 0x24);
  if ((*pcVar7 != '\0') && (*pcVar9 == '\0')) {
    iVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar29);
    iVar25 = iVar25 + iVar3 * -2;
    iVar3 = *piVar38;
  }
  pSVar12 = operator_new(0x24);
  ScrollTouchWindow::ScrollTouchWindow
            (pSVar12,(*piVar31 - *(int *)(iVar3 + 0x28)) - *(int *)(this + 0x204),
             *(int *)(iVar3 + 0x2c) +
             *(int *)(iVar3 + 0xc) + *(int *)(iVar3 + 0x20) + *(int *)(this + 0x1fc),
             *(int *)(this + 0x204),iVar25,false);
  iVar3 = DAT_00133b5c;
  *(ScrollTouchWindow **)(this + 0xf4) = pSVar12;
  AbyssEngine::String::String(aSStack_15c,(char *)(iVar3 + 0x1337d8),false);
  pSVar5 = (String *)GameText::getText(*piVar30);
  AbyssEngine::String::String(aSStack_168,pSVar5,false);
  ScrollTouchWindow::setText(pSVar12,aSStack_15c,aSStack_168);
  AbyssEngine::String::~String(aSStack_168);
  AbyssEngine::String::~String(aSStack_15c);
  fVar43 = local_120;
  uVar24 = *(undefined4 *)(this + 0x1a8);
  TouchButton::getPosition();
  iVar3 = *(int *)(*piVar38 + 0x2c);
  pTVar6 = operator_new(200);
  pSVar5 = (String *)GameText::getText(*piVar30);
  uVar32 = __aeabi_idiv(uVar24,3);
  fVar42 = (float)VectorSignedToFloat(uVar24,(byte)(in_fpscr >> 0x16) & 3);
  fVar44 = (float)VectorSignedToFloat(iVar3 * 3,(byte)(in_fpscr >> 0x16) & 3);
  fVar45 = (float)VectorSignedToFloat(uVar32,(byte)(in_fpscr >> 0x16) & 3);
  TouchButton::TouchButton
            (pTVar6,pSVar5,0x11,(int)(fVar43 + fVar42 + fVar45),(int)(local_128 - fVar44),'\x14');
  *(undefined4 *)pTVar6 = 0x35;
  *(undefined4 *)(pTVar6 + 4) = 0;
  ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
  if (**(char **)(DAT_00133b60 + 0x1338a8) == '\0') {
    TouchButton::setVisible(pTVar6,false);
  }
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar29,0xbc2,(uint *)(this + 0x124));
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar29,0x233f,(uint *)(this + 0x128));
  if (*pcVar7 == '\0') {
    iVar25 = *piVar31;
    *(undefined4 *)(this + 0x19c) = 0;
    *(int *)(this + 0x1a0) = iVar25;
    iVar3 = *piVar38;
    iVar27 = ((((*piVar23 - *(int *)(iVar3 + 0xc)) - *(int *)(iVar3 + 0x10)) -
              *(int *)(iVar3 + 0x20)) - *(int *)(iVar3 + 0x24)) / 2;
    *(int *)(this + 0x154) = iVar27;
  }
  else {
    (*pcVar35)(*puVar29,0x4c1,this + 0x1c);
    (*pcVar35)(*puVar29,0x4b2,this + 0x20);
    (*pcVar35)(*puVar29,0x4b0,this + 0x24);
    (*pcVar35)(*puVar29,0x4b6,this + 0x28);
    iVar3 = *(int *)(DAT_00133b64 + 0x133922);
    puVar21 = *(undefined4 **)(DAT_00133b68 + 0x133926);
    iVar25 = *(int *)(iVar3 + 0x54);
    pGVar13 = (Globals *)*puVar21;
    pcVar37 = *(code **)(DAT_00133b6c + 0x13392a);
    iVar27 = (*pcVar37)(*puVar29,*(undefined4 *)(this + 0x1c));
    iVar39 = (*pcVar37)(*puVar29,*(undefined4 *)(this + 0x24));
    iVar14 = (*pcVar37)(*puVar29,*(undefined4 *)(this + 0x20));
    Globals::setCoordsSteer
              (pGVar13,iVar25,iVar27,iVar39,iVar14,(ushort *)(this + 0x2e),(ushort *)(this + 0x30),
               (ushort *)(this + 0x40),(ushort *)(this + 0x42),(ushort *)(this + 0x3c),
               (ushort *)(this + 0x3e),(ushort *)(this + 0x34),(ushort *)(this + 0x32),
               (ushort *)(this + 0x48),(ushort *)(this + 0x4a));
    (*pcVar35)(*puVar29,0x4c6,this + 0x78);
    (*pcVar35)(*puVar29,0x6aa,this + 0x7c);
    (*pcVar35)(*puVar29,0x4ba,this + 0x84);
    (*pcVar35)(*puVar29,0x4b4,this + 0x80);
    (*pcVar35)(*puVar29,0x4be,this + 0x88);
    (*pcVar35)(*puVar29,0x4bc,this + 0x8c);
    iVar3 = *(int *)(iVar3 + 0x58);
    pGVar13 = (Globals *)*puVar21;
    iVar25 = (*pcVar37)(*puVar29,*(undefined4 *)(this + 0x78));
    Globals::setCoordsFire
              (pGVar13,iVar3,iVar25,*(uint *)(this + 0x78),*(uint *)(this + 0x7c),
               (uint *)(this + 0x74),(ushort *)(this + 0x60),(ushort *)(this + 0x62),
               (ushort *)(this + 100),(ushort *)(this + 0x66),(ushort *)(this + 0x68),
               (ushort *)(this + 0x6a),(ushort *)(this + 0x6c),(ushort *)(this + 0x6e),
               (ushort *)(this + 0x70),(ushort *)(this + 0x72),(ushort *)(this + 0x4c),
               (ushort *)(this + 0x4e));
    (*pcVar35)(*puVar29,0x6a5,this + 0x50);
    (*pcVar35)(*puVar29,0x6a6,this + 0x54);
    (*pcVar35)(*puVar29,0x6a7,this + 0x58);
    (*pcVar35)(*puVar29,0x6a8,this + 0x5c);
    pTVar6 = operator_new(200);
    pSVar5 = (String *)GameText::getText(*piVar30);
    iVar3 = *piVar38;
    pcVar36 = *(char **)(DAT_00133b70 + 0x133b00);
    fVar43 = DAT_00133b74;
    if (*pcVar36 == '\0') {
      pfVar26 = (float *)&DAT_00133cac;
      if (**(char **)(DAT_00133ca8 + 0x133b80) == '\0') {
        pfVar26 = (float *)&DAT_00133cb0;
      }
      fVar43 = *pfVar26;
    }
    TouchButton::TouchButton
              (pTVar6,pSVar5,0,*piVar31 >> 1,
               *(int *)(iVar3 + 0x308) + *(int *)(iVar3 + 0x300) + *(int *)(iVar3 + 0x34) * -2 +
               *(int *)(iVar3 + 0x30) * -2,(int)fVar43,'D','\x04');
    *(TouchButton **)(this + 0x18) = pTVar6;
    pTVar6 = operator_new(200);
    pSVar5 = (String *)GameText::getText(*piVar30);
    iVar3 = *piVar38;
    fVar43 = DAT_00133b74;
    if (*pcVar36 == '\0') {
      pfVar26 = (float *)&DAT_00133cac;
      if (**(char **)(DAT_00133cb4 + 0x133bf2) == '\0') {
        pfVar26 = (float *)&DAT_00133cb0;
      }
      fVar43 = *pfVar26;
    }
    TouchButton::TouchButton
              (pTVar6,pSVar5,0,*piVar31 >> 1,
               ((*(int *)(iVar3 + 0x300) + *(int *)(iVar3 + 0x308)) - *(int *)(iVar3 + 0x34)) -
               *(int *)(iVar3 + 0x30),(int)fVar43,'D','\x04');
    *(TouchButton **)(this + 0x14) = pTVar6;
    pSVar12 = operator_new(0x24);
    iVar3 = *piVar38;
    ScrollTouchWindow::ScrollTouchWindow
              (pSVar12,(*piVar31 >> 1) - *(int *)(iVar3 + 0x304) / 2,
               *(int *)(iVar3 + 0x300) + *(int *)(iVar3 + 8),*(int *)(iVar3 + 0x304),
               *(int *)(iVar3 + 0x308),false);
    iVar3 = DAT_00133cb8;
    *(ScrollTouchWindow **)(this + 0x10) = pSVar12;
    AbyssEngine::String::String(aSStack_174,(char *)(iVar3 + 0x133c5c),false);
    pSVar5 = (String *)GameText::getText(*piVar30);
    AbyssEngine::String::String(aSStack_180,pSVar5,false);
    ScrollTouchWindow::setText(pSVar12,aSStack_174,aSStack_180);
    AbyssEngine::String::~String(aSStack_180);
    AbyssEngine::String::~String(aSStack_174);
    *(undefined4 *)(this + 8) = 0;
    *(undefined4 *)(this + 0xc) = 0;
    if (*pcVar36 == '\0') {
      pfVar26 = (float *)&DAT_0013402c;
      iVar25 = 1000;
      if (**(char **)(DAT_00134028 + 0x133cd0) == '\0') {
        pfVar26 = (float *)&DAT_00134030;
        iVar25 = 500;
      }
      fVar43 = *pfVar26;
      iVar3 = *piVar31 - iVar25;
    }
    else {
      iVar25 = 0x2bf;
      iVar3 = *piVar31 + -0x2bf;
      fVar43 = DAT_00133cbc;
    }
    *(int *)(this + 0x1a0) = iVar25;
    iVar27 = (int)fVar43;
    *(int *)(this + 0x19c) = iVar3 / 2;
    *(int *)(this + 0x154) = (int)fVar43;
    iVar3 = *piVar38;
  }
  iVar25 = iVar25 + *(int *)(iVar3 + 0x28) * -2;
  *(int *)(this + 0x150) = iVar25;
  iVar3 = 1 - *(int *)(iVar3 + 0x2c);
  *(int *)(this + 0x158) = iVar3 + iVar25 / 2;
  *(int *)(this + 0x15c) = iVar3 + iVar27 / 2;
  iVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar29);
  *(int *)(this + 0x160) = iVar3;
  *(int *)(this + 0x164) = iVar3 + 0xf;
  pTVar6 = operator_new(200);
  pSVar5 = (String *)GameText::getText(*piVar30);
  iVar3 = *piVar38;
  TouchButton::TouchButton
            (pTVar6,pSVar5,0,
             ((*(int *)(this + 0x1a0) + *(int *)(this + 0x19c)) - *(int *)(iVar3 + 0x28)) -
             *(int *)(this + 0x158) / 2,
             *(int *)(iVar3 + 0x2c) +
             *(int *)(this + 0x154) + *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
             *(int *)(this + 0x15c) / 2,*(int *)(this + 0x158) + -0x14,'D','\x04');
  *(TouchButton **)(this + 0xd0) = pTVar6;
  pTVar6 = operator_new(200);
  AbyssEngine::String::String(aSStack_12c,(char *)(DAT_00134034 + 0x133db6),false);
  iVar3 = *piVar38;
  TouchButton::TouchButton
            (pTVar6,aSStack_12c,0xd,
             *(int *)(this + 0x19c) + *(int *)(iVar3 + 0x28) + *(int *)(this + 0x158) + -10,
             *(int *)(iVar3 + 0x2c) +
             *(int *)(this + 0x154) + *(int *)(iVar3 + 0xc) + *(int *)(iVar3 + 0x20) +
             *(int *)(this + 0x15c) / 2,'B');
  *(TouchButton **)(this + 0xcc) = pTVar6;
  AbyssEngine::String::~String(aSStack_12c);
  pTVar6 = operator_new(200);
  AbyssEngine::String::String(aSStack_12c,(char *)(DAT_00134038 + 0x133e1a),false);
  iVar3 = *piVar38;
  TouchButton::TouchButton
            (pTVar6,aSStack_12c,0xd,
             (*(int *)(this + 0x19c) + *(int *)(this + 0x1a0) + -0x19) - *(int *)(iVar3 + 0x28),
             *(int *)(iVar3 + 0x2c) * 7 +
             *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) + *(int *)(iVar3 + 0x1c) * 3 +
             *(int *)(this + 0x154) + *(int *)(this + 0x164) * 3 + *(int *)(this + 0x164) / 2 +
             *(int *)(iVar3 + 0x2fc) * 2 + *(int *)(this + 0x15c) * 2,'B');
  *(TouchButton **)(this + 0xe8) = pTVar6;
  AbyssEngine::String::~String(aSStack_12c);
  pfVar26 = *(float **)(DAT_0013403c + 0x133e94);
  TouchButton::setHalfTransparent(SUB41(*(undefined4 *)(this + 0xd0),0));
  TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xcc),*(bool *)(pfVar26 + 4));
  TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xe8),*(bool *)(pfVar26 + 0x10));
  if (*pcVar7 != '\0') {
    pTVar6 = operator_new(200);
    pSVar5 = (String *)GameText::getText(*piVar30);
    iVar3 = *piVar38;
    TouchButton::TouchButton
              (pTVar6,pSVar5,0,
               ((*(int *)(this + 0x1a0) + *(int *)(this + 0x19c)) - *(int *)(iVar3 + 0x28)) -
               *(int *)(this + 0x158) / 2,
               *(int *)(iVar3 + 0x2c) +
               *(int *)(this + 0x154) + *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
               *(int *)(this + 0x15c) / 2,*(int *)(this + 0x158) + -0x14,'D','\x04');
    *(TouchButton **)(this + 0xe4) = pTVar6;
  }
  pvVar4 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(void **)(this + 0xb4) = pvVar4;
  pSVar5 = (String *)GameText::getText(*piVar30);
  AbyssEngine::String::String(aSStack_18c,pSVar5,false);
  addButton(this,0x10,aSStack_18c,0,*(undefined4 *)(this + 0xb4),0);
  AbyssEngine::String::~String(aSStack_18c);
  pSVar5 = (String *)GameText::getText(*piVar30);
  AbyssEngine::String::String(aSStack_198,pSVar5,false);
  addButton(this,0x32,aSStack_198,1,*(undefined4 *)(this + 0xb4),0);
  AbyssEngine::String::~String(aSStack_198);
  pvVar4 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(void **)(this + 0xb8) = pvVar4;
  local_1a0 = 0xffffffff;
  local_19c = 0xffffffff;
  iVar3 = DAT_00134040 + 0x133fc8;
  for (uVar33 = 0; uVar33 < 3; uVar33 = uVar33 + 1) {
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)*puVar29,*(ushort *)(iVar3 + uVar33 * 8),&local_19c);
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)*puVar29,*(ushort *)(iVar3 + uVar33 * 8 + 4),&local_1a0);
    iVar25 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar29);
    iVar27 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar29);
    if (uVar33 == 0) {
      iVar25 = (-iVar25 - iVar25 / 2) + *piVar31 / 2;
    }
    else {
      iVar25 = iVar25 / 2;
      if (uVar33 == 1) {
        iVar25 = -iVar25;
      }
      iVar25 = iVar25 + *piVar31 / 2;
    }
    pTVar6 = operator_new(200);
    TouchButton::TouchButton
              (pTVar6,local_19c,local_1a0,0x13,iVar25,*piVar23 / 2 - iVar27 / 2,'\x01');
    ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xb8));
  }
  iVar27 = *(int *)(*piVar38 + 0x2b4);
  iVar3 = *(int *)(*piVar38 + 0x2b8);
  this_00 = operator_new(0xc);
  Array<TouchSlider*>::Array(this_00);
  *(Array<TouchSlider*> **)(this + 0xec) = this_00;
  pTVar15 = operator_new(0x3c);
  iVar25 = *piVar38;
  pfVar22 = pfVar26 + 5;
  if (*(char *)((int)pfVar26 + 0x11) == '\0') {
    pfVar22 = pfVar26 + 6;
  }
  TouchSlider::TouchSlider
            (pTVar15,0,((*piVar31 + 10) - *(int *)(iVar25 + 0x28)) - *(int *)(this + 0x158),
             (*(int *)(iVar25 + 0xc) - iVar27) + *(int *)(iVar25 + 0x20) + *(int *)(this + 0x154) +
             (*(int *)(this + 0x15c) + *(int *)(iVar25 + 0x2c)) * 2,*pfVar22);
  ArrayAdd<TouchSlider*>(pTVar15,*(Array **)(this + 0xec));
  pTVar15 = operator_new(0x3c);
  iVar25 = *piVar38;
  TouchSlider::TouchSlider
            (pTVar15,1,*(int *)(iVar25 + 0x28) + *(int *)(this + 0x1c0),
             *(int *)(iVar25 + 0xc) + iVar3 + *(int *)(iVar25 + 0x20) + *(int *)(iVar25 + 0x1c) +
             *(int *)(iVar25 + 0x2c),*pfVar26);
  ArrayAdd<TouchSlider*>(pTVar15,*(Array **)(this + 0xec));
  pTVar15 = operator_new(0x3c);
  iVar25 = *piVar38;
  TouchSlider::TouchSlider
            (pTVar15,1,*(int *)(this + 0x1c0) + *(int *)(iVar25 + 0x28),
             *(int *)(iVar25 + 0x20) + *(int *)(iVar25 + 0xc) + iVar3 + *(int *)(iVar25 + 0x1c) +
             *(int *)(iVar25 + 0x2c) * 3 + *(int *)(this + 0x160),pfVar26[1]);
  ArrayAdd<TouchSlider*>(pTVar15,*(Array **)(this + 0xec));
  pTVar15 = operator_new(0x3c);
  iVar25 = *piVar38;
  TouchSlider::TouchSlider
            (pTVar15,1,*(int *)(this + 0x1c0) + *(int *)(iVar25 + 0x28),
             *(int *)(iVar25 + 0x20) + *(int *)(iVar25 + 0xc) + iVar3 + *(int *)(iVar25 + 0x1c) +
             *(int *)(iVar25 + 0x2c) * 5 + *(int *)(this + 0x160) * 2,pfVar26[2]);
  ArrayAdd<TouchSlider*>(pTVar15,*(Array **)(this + 0xec));
  pTVar15 = operator_new(0x3c);
  iVar27 = *piVar38;
  dVar47 = (double)VectorSignedToFloat(*(undefined4 *)(this + 0x160),(byte)(in_fpscr >> 0x16) & 3);
  dVar48 = (double)VectorSignedToFloat(*(int *)(iVar27 + 0xc) + *(int *)(iVar27 + 0x20) +
                                       *(int *)(iVar27 + 0x1c) * 3 + *(int *)(iVar27 + 0x2c) * 6,
                                       (byte)(in_fpscr >> 0x16) & 3);
  dVar46 = (double)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  iVar25 = (int)(longlong)(dVar48 + dVar47 * 2.5 + dVar46);
  TouchSlider::TouchSlider
            (pTVar15,2,*(int *)(this + 0x1c0) + *(int *)(iVar27 + 0x28),iVar25,(float)iVar25);
  ArrayAdd<TouchSlider*>(pTVar15,*(Array **)(this + 0xec));
  pTVar15 = operator_new(0x3c);
  iVar27 = *piVar38;
  dVar47 = (double)VectorSignedToFloat(*(undefined4 *)(this + 0x160),(byte)(in_fpscr >> 0x16) & 3);
  dVar49 = (double)VectorSignedToFloat(*(int *)(iVar27 + 0xc) + *(int *)(iVar27 + 0x20) +
                                       *(int *)(iVar27 + 0x1c) * 3 + *(int *)(iVar27 + 0x2c) * 8,
                                       (byte)(in_fpscr >> 0x16) & 3);
  dVar48 = (double)VectorSignedToFloat(*(undefined4 *)(this + 0x164),(byte)(in_fpscr >> 0x16) & 3);
  iVar25 = (int)(longlong)(dVar49 + dVar47 * 2.5 + dVar48 + dVar46);
  TouchSlider::TouchSlider
            (pTVar15,2,*(int *)(this + 0x1c0) + *(int *)(iVar27 + 0x28),iVar25,(float)iVar25);
  ArrayAdd<TouchSlider*>(pTVar15,*(Array **)(this + 0xec));
  pTVar15 = operator_new(0x3c);
  iVar25 = *piVar38;
  TouchSlider::TouchSlider
            (pTVar15,2,*(int *)(iVar25 + 0x28) + *(int *)(this + 0x1c0),
             *(int *)(iVar25 + 0xc) + iVar3 + *(int *)(iVar25 + 0x20) + *(int *)(iVar25 + 0x1c) +
             *(int *)(iVar25 + 0x2c),pfVar26[0xb]);
  ArrayAdd<TouchSlider*>(pTVar15,*(Array **)(this + 0xec));
  iVar39 = *piVar31;
  pTVar15 = *(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x18);
  iVar25 = TouchSlider::getWidth(pTVar15);
  iVar27 = *piVar38;
  TouchSlider::setPosition
            (pTVar15,iVar39 / 2 - iVar25 / 2,
             *(int *)(iVar27 + 0xc) + iVar3 + *(int *)(iVar27 + 0x20) + *(int *)(iVar27 + 0x1c) +
             *(int *)(iVar27 + 0x2c));
  pTVar6 = operator_new(200);
  AbyssEngine::String::String(aSStack_12c,(char *)(DAT_00134574 + 0x134372),false);
  iVar3 = *piVar38;
  TouchButton::TouchButton
            (pTVar6,aSStack_12c,0xd,*piVar31 - *(int *)(iVar3 + 0x28),
             *(int *)(iVar3 + 0x2c) +
             *(int *)(iVar3 + 0xc) + *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0x1c),'\x12');
  *(TouchButton **)(this + 0xd4) = pTVar6;
  AbyssEngine::String::~String(aSStack_12c);
  TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xd4),*(bool *)((int)pfVar26 + 0xd));
  TouchSlider::setHalfTransparent
            (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 4),
             *(char *)((int)pfVar26 + 0xd) == '\0');
  pTVar6 = operator_new(200);
  AbyssEngine::String::String(aSStack_12c,(char *)(DAT_00134578 + 0x1343e2),false);
  iVar3 = *piVar38;
  TouchButton::TouchButton
            (pTVar6,aSStack_12c,0xd,*piVar31 - *(int *)(iVar3 + 0x28),
             *(int *)(iVar3 + 0x2c) * 3 +
             *(int *)(this + 0x160) + *(int *)(iVar3 + 0xc) + *(int *)(iVar3 + 0x20) +
             *(int *)(iVar3 + 0x1c),'\x12');
  *(TouchButton **)(this + 0xd8) = pTVar6;
  AbyssEngine::String::~String(aSStack_12c);
  TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xd8),*(bool *)(pfVar26 + 3));
  TouchSlider::setHalfTransparent
            (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 8),
             *(char *)((int)pfVar26 + 0xe) == '\0');
  pTVar6 = operator_new(200);
  AbyssEngine::String::String(aSStack_12c,(char *)(DAT_0013457c + 0x134458),false);
  iVar3 = *piVar38;
  TouchButton::TouchButton
            (pTVar6,aSStack_12c,0xd,*piVar31 - *(int *)(iVar3 + 0x28),
             *(int *)(iVar3 + 0x2c) * 5 +
             *(int *)(iVar3 + 0xc) + *(int *)(iVar3 + 0x20) + *(int *)(this + 0x160) * 2 +
             *(int *)(iVar3 + 0x1c),'\x12');
  *(TouchButton **)(this + 0xdc) = pTVar6;
  AbyssEngine::String::~String(aSStack_12c);
  TouchButton::setAlwaysPressed(*(TouchButton **)(this + 0xdc),*(bool *)((int)pfVar26 + 0xe));
  TouchSlider::setHalfTransparent
            (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0xc),
             *(char *)((int)pfVar26 + 0xe) == '\0');
  TouchSlider::setFixedScale(*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x14),1);
  TouchSlider::setFixedScale(*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x18),1);
  if (*pcVar7 == '\0') {
    *(undefined4 *)(this + 0x198) = 0;
    pSVar12 = operator_new(0x24);
    iVar3 = *piVar38;
    iVar25 = *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc);
    ScrollTouchWindow::ScrollTouchWindow
              (pSVar12,*(int *)(iVar3 + 0x28),iVar25,*piVar31 + *(int *)(iVar3 + 0x28) * -2,
               ((*piVar23 - iVar25) - *(int *)(iVar3 + 0x10)) - *(int *)(iVar3 + 0x24),false);
  }
  else {
    iVar3 = *piVar38;
    pcVar36 = *(char **)(DAT_00134580 + 0x1344ee);
    fVar42 = (float)VectorSignedToFloat(*(int *)(iVar3 + 0x2c) * 3 +
                                        *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
                                        *(int *)(iVar3 + 0x1c) + *(int *)(this + 0x154) +
                                        *(int *)(this + 0x15c) * 2,(byte)(in_fpscr >> 0x16) & 3);
    fVar43 = DAT_00134584;
    if ((*pcVar36 == '\0') && (fVar43 = DAT_00134908, **(char **)(DAT_00134904 + 0x134596) == '\0'))
    {
      fVar43 = 25.0;
    }
    TouchSlider::setPosition
              ((TouchSlider *)**(undefined4 **)(*(int *)(this + 0xec) + 4),
               *(int *)(this + 0x19c) + *(int *)(iVar3 + 0x28) + 10,(int)(fVar42 - fVar43));
    fVar43 = DAT_0013490c;
    iVar3 = *piVar38;
    fVar44 = (float)VectorSignedToFloat(*(int *)(iVar3 + 0x2fc) +
                                        *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
                                        *(int *)(this + 0x154) + *(int *)(iVar3 + 0x2c) * 4 +
                                        (*(int *)(this + 0x15c) + *(int *)(iVar3 + 0x1c)) * 2,
                                        (byte)(in_fpscr >> 0x16) & 3);
    fVar42 = DAT_0013490c;
    if ((*pcVar36 == '\0') && (fVar42 = 28.0, **(char **)(DAT_00134910 + 0x134614) == '\0')) {
      fVar42 = 14.0;
    }
    TouchSlider::setPosition
              (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 4),
               *(int *)(iVar3 + 0x28) + *(int *)(this + 0x19c) + *(int *)(this + 0x1c0),
               (int)(fVar42 + fVar44));
    iVar3 = *piVar38;
    fVar44 = (float)VectorSignedToFloat(*(int *)(iVar3 + 0x2fc) +
                                        *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
                                        *(int *)(this + 0x154) + *(int *)(iVar3 + 0x2c) * 5 +
                                        *(int *)(this + 0x164) +
                                        *(int *)(this + 0x15c) * 2 + *(int *)(iVar3 + 0x1c) * 2,
                                        (byte)(in_fpscr >> 0x16) & 3);
    fVar42 = fVar43;
    if ((*pcVar36 == '\0') && (fVar42 = 28.0, **(char **)(DAT_00134914 + 0x13469a) == '\0')) {
      fVar42 = 14.0;
    }
    TouchSlider::setPosition
              (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 8),
               *(int *)(this + 0x1c0) + *(int *)(this + 0x19c) + *(int *)(iVar3 + 0x28),
               (int)(fVar42 + fVar44));
    iVar3 = *piVar38;
    fVar42 = (float)VectorSignedToFloat(*(int *)(iVar3 + 0x2fc) +
                                        *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
                                        *(int *)(this + 0x154) + *(int *)(iVar3 + 0x2c) * 6 +
                                        (*(int *)(this + 0x15c) + *(int *)(iVar3 + 0x1c) +
                                        *(int *)(this + 0x164)) * 2,(byte)(in_fpscr >> 0x16) & 3);
    if ((*pcVar36 == '\0') && (fVar43 = 28.0, **(char **)(DAT_00134918 + 0x13471a) == '\0')) {
      fVar43 = 14.0;
    }
    TouchSlider::setPosition
              (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0xc),
               *(int *)(this + 0x1c0) + *(int *)(this + 0x19c) + *(int *)(iVar3 + 0x28),
               (int)(fVar43 + fVar42));
    fVar43 = DAT_00134924;
    pTVar15 = *(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x10);
    if (**(char **)(DAT_0013491c + 0x13473e) == '\0') {
      iVar3 = *piVar38;
      fVar44 = (float)VectorSignedToFloat(*(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
                                          *(int *)(iVar3 + 0x1c) * 3 + *(int *)(this + 0x154) +
                                          *(int *)(iVar3 + 0x2c) * 6 + *(int *)(this + 0x164) * 3 +
                                          (*(int *)(this + 0x15c) + *(int *)(iVar3 + 0x2fc)) * 2,
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar42 = DAT_00134924;
      if ((*pcVar36 == '\0') && (fVar42 = 14.0, **(char **)(DAT_00134928 + 0x134828) == '\0')) {
        fVar42 = 7.0;
      }
      TouchSlider::setPosition
                (pTVar15,*(int *)(this + 0x1c0) + *(int *)(iVar3 + 0x28) + *(int *)(this + 0x19c),
                 (int)(fVar42 + fVar44));
      iVar3 = *piVar38;
      fVar42 = (float)VectorSignedToFloat(*(int *)(iVar3 + 0x2c) * 7 +
                                          *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
                                          *(int *)(iVar3 + 0x1c) * 3 + *(int *)(this + 0x154) +
                                          (*(int *)(this + 0x15c) + *(int *)(iVar3 + 0x2fc) +
                                          *(int *)(this + 0x164) * 2) * 2,
                                          (byte)(in_fpscr >> 0x16) & 3);
      if ((*pcVar36 == '\0') && (fVar43 = 14.0, **(char **)(DAT_0013492c + 0x1348ae) == '\0')) {
        fVar43 = 7.0;
      }
      TouchSlider::setPosition
                (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x14),
                 *(int *)(this + 0x1c0) + *(int *)(this + 0x19c) + *(int *)(iVar3 + 0x28),
                 (int)(fVar43 + fVar42));
    }
    else {
      TouchSlider::setPosition(pTVar15,DAT_00134920,DAT_00134920);
      iVar3 = *piVar38;
      fVar42 = (float)VectorSignedToFloat(*(int *)(iVar3 + 0x2c) * 7 +
                                          *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
                                          *(int *)(iVar3 + 0x1c) * 3 + *(int *)(this + 0x154) +
                                          (*(int *)(this + 0x15c) + *(int *)(iVar3 + 0x2fc) +
                                          *(int *)(this + 0x164) * 2) * 2,
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar43 = DAT_00134924;
      if ((*pcVar36 == '\0') && (fVar43 = 14.0, **(char **)(DAT_00134d0c + 0x13493e) == '\0')) {
        fVar43 = 7.0;
      }
      fVar43 = (float)TouchSlider::setPosition
                                (*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x14),
                                 *(int *)(iVar3 + 0x28) + *(int *)(this + 0x19c) +
                                 *(int *)(this + 0x1c0),(int)(fVar43 + fVar42));
      TouchSlider::setValue(*(TouchSlider **)(*(int *)(*(int *)(this + 0xec) + 4) + 0x14),fVar43);
    }
    AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar29,0x6a4,(uint *)(this + 0x11c));
    iVar25 = *piVar31;
    fVar43 = (float)VectorSignedToFloat(iVar25,(byte)(in_fpscr >> 0x16) & 3);
    iVar3 = (int)(fVar43 * DAT_00134d10);
    if (*pcVar9 != '\0') {
      iVar3 = 0x32;
    }
    *(int *)(this + 0x198) = iVar3;
    pSVar12 = operator_new(0x24);
    iVar3 = *piVar38;
    iVar27 = *(int *)(iVar3 + 0xc) + *(int *)(iVar3 + 0x20);
    ScrollTouchWindow::ScrollTouchWindow
              (pSVar12,*(int *)(iVar3 + 0x28) + (iVar25 >> 2),iVar27,
               (iVar25 >> 1) + *(int *)(iVar3 + 0x28) * -2,
               ((*piVar23 - iVar27) - *(int *)(iVar3 + 0x10)) - *(int *)(iVar3 + 0x24),false);
  }
  *(ScrollTouchWindow **)(this + 0xf0) = pSVar12;
  iVar3 = AbyssEngine::ApplicationManager::GetEngine();
  AbyssEngine::String::String(aSStack_12c,(String *)(iVar3 + 0x3c),false);
  if (local_124 < 3) {
    AbyssEngine::String::String(aSStack_1ac,(char *)(DAT_00134d1c + 0x134a30),false);
    AbyssEngine::String::operator=(aSStack_12c,aSStack_1ac);
    pSVar16 = aSStack_1ac;
  }
  else {
    AbyssEngine::String::String(aSStack_1b8,(char *)(DAT_00134d18 + 0x134a08),false);
    AbyssEngine::operator+(aSStack_1ac,aSStack_1b8);
    AbyssEngine::String::operator=(aSStack_12c,aSStack_1ac);
    AbyssEngine::String::~String(aSStack_1ac);
    pSVar16 = aSStack_1b8;
  }
  AbyssEngine::String::~String(pSVar16);
  AbyssEngine::String::String(aSStack_1e8,(char *)(DAT_00134d20 + 0x134a4a),false);
  AbyssEngine::operator+(aSStack_1dc,aSStack_1e8);
  AbyssEngine::String::String(aSStack_1f4,(char *)(DAT_00134d24 + 0x134a60),false);
  AbyssEngine::operator+(aSStack_1d0,aSStack_1dc);
  GameText::getText(*piVar30);
  AbyssEngine::operator+(aSStack_1c4,aSStack_1d0);
  AbyssEngine::String::String(aSStack_200,(char *)(DAT_00134d28 + 0x134a88),false);
  AbyssEngine::operator+(aSStack_1b8,aSStack_1c4);
  GameText::getText(*piVar30);
  AbyssEngine::operator+(aSStack_1ac,aSStack_1b8);
  pcVar35 = *(code **)(DAT_00134d2c + 0x134ab0);
  (*pcVar35)(aSStack_1b8);
  (*pcVar35)(aSStack_200);
  (*pcVar35)(aSStack_1c4);
  (*pcVar35)(aSStack_1d0);
  (*pcVar35)(aSStack_1f4);
  (*pcVar35)(aSStack_1dc);
  (*pcVar35)(aSStack_1e8);
  pSVar12 = *(ScrollTouchWindow **)(this + 0xf0);
  AbyssEngine::String::String(aSStack_20c,(char *)(DAT_00134d30 + 0x134ad6),false);
  AbyssEngine::String::String(aSStack_218,aSStack_1ac,false);
  ScrollTouchWindow::setText(pSVar12,aSStack_20c,aSStack_218);
  AbyssEngine::String::~String(aSStack_218);
  AbyssEngine::String::~String(aSStack_20c);
  ScrollTouchWindow::setTextCentered(SUB41(*(undefined4 *)(this + 0xf0),0));
  if ((*pcVar7 == '\0') || (*pcVar9 != '\0')) {
    pTVar6 = operator_new(200);
    pSVar5 = (String *)GameText::getText(*piVar30);
    TouchButton::TouchButton
              (pTVar6,pSVar5,0,*piVar31 - *(int *)(*piVar38 + 0x2c),
               *piVar23 - *(int *)(*piVar38 + 0x2c),'\"');
  }
  else {
    pTVar6 = operator_new(200);
    pSVar5 = (String *)GameText::getText(*piVar30);
    iVar3 = *piVar38;
    iVar25 = *(int *)(this + 0x200);
    uVar32 = *(undefined4 *)(this + 0x204);
    iVar27 = *(int *)(iVar3 + 0x10);
    iVar40 = *piVar23;
    iVar14 = *(int *)(iVar3 + 0x24);
    iVar39 = *(int *)(iVar3 + 0x28);
    iVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar29);
    fVar43 = (float)VectorSignedToFloat(uVar32,(byte)(in_fpscr >> 0x16) & 3);
    fVar42 = (float)VectorSignedToFloat(iVar25 + iVar39,(byte)(in_fpscr >> 0x16) & 3);
    TouchButton::TouchButton
              (pTVar6,pSVar5,7,(int)(fVar42 + fVar43 * 0.5),
               (((iVar40 + 5) - iVar27) - iVar14) - iVar3,'$');
  }
  *(undefined4 *)pTVar6 = 0x34;
  *(undefined4 *)(pTVar6 + 4) = 0;
  ArrayAdd<TouchButton*>(pTVar6,*(Array **)(this + 0xc0));
  this[0x170] = (MenuTouchWindow)0x0;
  *(undefined2 *)(this + 0x17f) = 0;
  this[0x181] = (MenuTouchWindow)0x0;
  this_01 = operator_new(0x5c);
  ChoiceWindow::ChoiceWindow(this_01);
  *(ChoiceWindow **)(this + 0x104) = this_01;
  iVar27 = AbyssEngine::ApplicationManager::GetApplicationData();
  iVar25 = DAT_00134f18;
  iVar3 = DAT_00134f10;
  if ((*(char *)((int)pfVar26 + 0x35) == '\0') && (*(char *)(iVar27 + 0x6c) != '\0')) {
    *(undefined1 *)((int)pfVar26 + 0x35) = 1;
    RecordHandler::saveOptions((RecordHandler *)**(undefined4 **)(iVar3 + 0x134c54));
    *(undefined1 *)(iVar27 + 0x6c) = 0;
    pSVar5 = *(String **)(this + 0x104);
    GameText::getText(*piVar30);
    ChoiceWindow::set(pSVar5);
  }
  else {
    if ((**(char **)(DAT_00134f14 + 0x134c0c) == '\0') || (*(char *)((int)pfVar26 + 0x41) != '\0'))
    goto LAB_00134c80;
    *(undefined1 *)((int)pfVar26 + 0x41) = 1;
    RecordHandler::saveOptions((RecordHandler *)**(undefined4 **)(iVar25 + 0x134c22));
    pSVar5 = *(String **)(this + 0x104);
    GameText::getText(*piVar30);
    ChoiceWindow::set(pSVar5);
  }
  this[0x17f] = (MenuTouchWindow)0x1;
  this[0x170] = (MenuTouchWindow)0x1;
LAB_00134c80:
  *(undefined4 *)(this + 0xfc) = 0;
  *(undefined4 *)(this + 0x100) = 0;
  *(undefined4 *)(this + 0x16c) = 0;
  *(undefined4 *)(this + 0x177) = 0;
  *(undefined4 *)(this + 0x173) = 0;
  *(undefined2 *)(this + 0x171) = 0;
  *(undefined4 *)(this + 0x17b) = 0;
  *(undefined4 *)(this + 0x18c) = 0xffffffff;
  *(undefined4 *)(this + 0xc4) = 0;
  *(undefined4 *)(this + 200) = 0;
  *(undefined2 *)(this + 0x108) = 0;
  *(undefined4 *)(this + 0x1a4) = 0x3f000000;
  *(undefined4 *)(this + 0x184) = 0xffffffff;
  *(undefined2 *)(this + 0x188) = 0;
  this[400] = (MenuTouchWindow)0x0;
  *(undefined4 *)(this + 0x194) = 0;
  this[0x1de] = (MenuTouchWindow)0x0;
  *(undefined4 *)(this + 0x1da) = 0;
  *(undefined4 *)(this + 0x215) = 0;
  *(undefined4 *)(this + 0x219) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x21d) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x221) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x20c) = 0;
  *(undefined4 *)(this + 0x210) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x214) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x218) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  AbyssEngine::String::~String(aSStack_1ac);
  AbyssEngine::String::~String(aSStack_12c);
  iVar3 = *piVar18 - local_3c;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  0013299c: push {r4,r5,r6,r7,lr}
  0013299e: add r7,sp,#0xc
  001329a0: push {r7,r8,r9,r10,r11}
  001329a4: vpush {d8,d9}
  001329a8: sub.w sp,sp,#0x250
  001329ac: mov r10,r0
  001329ae: ldr r0,[0x00132c34]
  001329b0: mov r5,r1
  001329b2: ldr r1,[0x00132c38]
  001329b4: add r0,pc
  001329b6: add r1,pc
  001329b8: ldr.w r11,[r0,#0x0]
  001329bc: ldr r1,[r1,#0x0]
  001329be: str r1,[sp,#0x3c]
  001329c0: ldr.w r0,[r11,#0x0]
  001329c4: ldr r1,[r1,#0x0]
  001329c6: str r1,[sp,#0x24c]
  001329c8: add.w r1,r0,#0x294
  001329cc: vld1.32 {d16,d17},[r1]
  001329d0: add.w r1,r10,#0x1a8
  001329d4: vst1.32 {d16,d17},[r1]
  001329d8: str r1,[sp,#0x48]
  001329da: ldr.w r1,[r0,#0x2a4]
  001329de: str.w r1,[r10,#0x1b8]
  001329e2: ldr.w r1,[r0,#0x2a8]
  001329e6: str.w r1,[r10,#0x1bc]
  001329ea: ldr.w r1,[r0,#0x2ac]
  001329ee: str.w r1,[r10,#0x1c0]
  001329f2: ldr.w r8,[r0,#0x2b0]
  001329f6: movs r0,#0xc
  001329f8: str.w r5,[r10,#0x168]
  001329fc: blx 0x0006eb24
  00132a00: mov r4,r0
  00132a02: blx 0x00074f38
  00132a06: movs r0,#0xc
  00132a08: str.w r4,[r10,#0x4]
  00132a0c: blx 0x0006eb24
  00132a10: mov r4,r0
  00132a12: blx 0x00074f38
  00132a16: movs r0,#0x0
  00132a18: mov.w r1,#0xffffffff
  00132a1c: str.w r4,[r10,#0xc0]
  00132a20: strd r0,r0,[r10,#0xe4]
  00132a24: str.w r0,[r10,#0xb8]
  00132a28: strd r0,r0,[r10,#0xb0]
  00132a2c: strb.w r0,[r10,#0x0]
  00132a30: str.w r1,[r10,#0x120]
  00132a34: str.w r0,[r10,#0x18c]
  00132a38: strb.w r0,[r10,#0x1c4]
  00132a3c: strd r0,r0,[r10,#0x134]
  00132a40: str.w r0,[r10,#0x1e0]
  00132a44: strd r0,r0,[r10,#0xf0]
  00132a48: str.w r0,[r10,#0xf8]
  00132a4c: strd r0,r0,[r10,#0x230]
  00132a50: strb.w r0,[r10,#0x238]
  00132a54: strd r0,r0,[r10,#0x9c]
  00132a58: str.w r0,[r10,#0xbc]
  00132a5c: mov r0,r10
  00132a5e: blx 0x00075034
  00132a62: add.w r2,r10,#0x120
  00132a66: cmp r5,#0x0
  00132a68: str.w r10,[sp,#0x6c]
  00132a6c: str.w r11,[sp,#0x64]
  00132a70: beq.w 0x00132b84
  00132a74: cmp r5,#0x1
  00132a76: beq.w 0x00132c54
  00132a7a: cmp r5,#0x2
  00132a7c: bne.w 0x00132fbe
  00132a80: ldr r0,[0x00132c3c]
  00132a82: movs r1,#0x1c
  00132a84: add r0,pc
  00132a86: ldr r6,[r0,#0x0]
  00132a88: ldr r0,[r6,#0x0]
  00132a8a: blx 0x00072f70
  00132a8e: add r4,sp,#0x1f8
  00132a90: mov r1,r0
  00132a92: movs r2,#0x0
  00132a94: movs r5,#0x0
  00132a96: mov r0,r4
  00132a98: blx 0x0006f028
  00132a9c: ldr.w r0,[r10,#0x4]
  00132aa0: strd r0,r5,[sp,#0x0]
  00132aa4: mov r0,r10
  00132aa6: movs r1,#0x0
  00132aa8: mov r2,r4
  00132aaa: movs r3,#0x0
  00132aac: blx 0x00076480
  00132ab0: add r0,sp,#0x1f8
  00132ab2: blx 0x0006ee30
  00132ab6: ldr r0,[r6,#0x0]
  00132ab8: movs r1,#0x1d
  00132aba: blx 0x00072f70
  00132abe: add r4,sp,#0x1ec
  00132ac0: mov r1,r0
  00132ac2: movs r2,#0x0
  00132ac4: mov r0,r4
  00132ac6: blx 0x0006f028
  00132aca: ldr.w r0,[r10,#0x4]
  00132ace: strd r0,r5,[sp,#0x0]
  00132ad2: mov r0,r10
  00132ad4: movs r1,#0x1
  00132ad6: mov r2,r4
  00132ad8: movs r3,#0x1
  00132ada: blx 0x00076480
  00132ade: add r0,sp,#0x1ec
  00132ae0: blx 0x0006ee30
  00132ae4: ldr r0,[r6,#0x0]
  00132ae6: movs r1,#0x1e
  00132ae8: blx 0x00072f70
  00132aec: add r4,sp,#0x1e0
  00132aee: mov r1,r0
  00132af0: movs r2,#0x0
  00132af2: movs r5,#0x0
  00132af4: mov r0,r4
  00132af6: blx 0x0006f028
  00132afa: ldr.w r0,[r10,#0x4]
  00132afe: strd r0,r5,[sp,#0x0]
  00132b02: mov r0,r10
  00132b04: movs r1,#0x2
  00132b06: mov r2,r4
  00132b08: movs r3,#0x2
  00132b0a: blx 0x00076480
  00132b0e: add r0,sp,#0x1e0
  00132b10: blx 0x0006ee30
  00132b14: ldr r0,[r6,#0x0]
  00132b16: movs r1,#0x1f
  00132b18: blx 0x00072f70
  00132b1c: add r4,sp,#0x1d4
  00132b1e: mov r1,r0
  00132b20: movs r2,#0x0
  00132b22: mov r0,r4
  00132b24: blx 0x0006f028
  00132b28: ldr.w r0,[r10,#0x4]
  00132b2c: strd r0,r5,[sp,#0x0]
  00132b30: mov r0,r10
  00132b32: movs r1,#0x3
  00132b34: mov r2,r4
  00132b36: movs r3,#0x3
  00132b38: blx 0x00076480
  00132b3c: add r0,sp,#0x1d4
  00132b3e: blx 0x0006ee30
  00132b42: ldr r0,[0x00132c40]
  00132b44: add r0,pc
  00132b46: ldr r0,[r0,#0x0]
  00132b48: ldrb r0,[r0,#0x0]
  00132b4a: cmp r0,#0x0
  00132b4c: beq.w 0x00132f8c
  00132b50: ldr r0,[r6,#0x0]
  00132b52: movs r1,#0x0
  00132b54: movs r5,#0x0
  00132b56: blx 0x00072f70
  00132b5a: add r4,sp,#0x1c8
  00132b5c: mov r1,r0
  00132b5e: movs r2,#0x0
  00132b60: mov r0,r4
  00132b62: blx 0x0006f028
  00132b66: ldr.w r0,[r10,#0x4]
  00132b6a: strd r0,r5,[sp,#0x0]
  00132b6e: mov r0,r10
  00132b70: movs r1,#0x19
  00132b72: mov r2,r4
  00132b74: movs r3,#0x4
  00132b76: blx 0x00076480
  00132b7a: add r0,sp,#0x1c8
  00132b7c: blx 0x0006ee30
  00132b80: movs r5,#0x5
  00132b82: b 0x00132f8e
  00132b84: ldr r0,[0x00132c44]
  00132b86: movw r1,#0x1b5a
  00132b8a: add r0,pc
  00132b8c: ldr r0,[r0,#0x0]
  00132b8e: ldr r0,[r0,#0x0]
  00132b90: blx 0x00071cf8
  00132b94: ldr r0,[0x00132c48]
  00132b96: movs r1,#0x1c
  00132b98: add r0,pc
  00132b9a: ldr.w r9,[r0,#0x0]
  00132b9e: ldr.w r0,[r9,#0x0]
  00132ba2: blx 0x00072f70
  00132ba6: add r4,sp,#0x240
  00132ba8: mov r1,r0
  00132baa: movs r2,#0x0
  00132bac: mov r0,r4
  00132bae: blx 0x0006f028
  00132bb2: ldr.w r0,[r10,#0x4]
  00132bb6: strd r0,r8,[sp,#0x0]
  00132bba: mov r0,r10
  00132bbc: movs r1,#0x0
  00132bbe: mov r2,r4
  00132bc0: movs r3,#0x0
  00132bc2: blx 0x00076480
  00132bc6: add r0,sp,#0x240
  00132bc8: blx 0x0006ee30
  00132bcc: ldr r0,[0x00132c4c]
  00132bce: add r0,pc
  00132bd0: ldr r0,[r0,#0x0]
  00132bd2: ldr r0,[r0,#0x0]
  00132bd4: cmp r0,#0x0
  00132bd6: blt.w 0x00132ea4
  00132bda: ldr.w r1,[r10,#0xbc]
  00132bde: movs r5,#0x1
  00132be0: cmp r1,#0x0
  00132be2: beq.w 0x00132ea6
  00132be6: ldr r2,[0x00132c50]
  00132be8: add r2,pc
  00132bea: ldr r2,[r2,#0x0]
  00132bec: ldr r2,[r2,#0x0]
  00132bee: cmp r0,r2
  00132bf0: bge.w 0x00132ea6
  00132bf4: ldr r1,[r1,#0x4]
  00132bf6: ldr.w r0,[r1,r0,lsl #0x2]
  00132bfa: cmp r0,#0x0
  00132bfc: beq.w 0x00132ea4
  00132c00: ldr.w r0,[r9,#0x0]
  00132c04: movs r1,#0x29
  00132c06: blx 0x00072f70
  00132c0a: add r4,sp,#0x234
  00132c0c: mov r1,r0
  00132c0e: movs r2,#0x0
  00132c10: mov r0,r4
  00132c12: blx 0x0006f028
  00132c16: ldr.w r0,[r10,#0x4]
  00132c1a: strd r0,r8,[sp,#0x0]
  00132c1e: mov r0,r10
  00132c20: movs r1,#0xb
  00132c22: mov r2,r4
  00132c24: movs r3,#0x1
  00132c26: blx 0x00076480
  00132c2a: add r0,sp,#0x234
  00132c2c: blx 0x0006ee30
  00132c30: movs r5,#0x2
  00132c32: b 0x00132ea6
  00132c54: ldr r0,[0x00132f74]
  00132c56: movw r1,#0x534
  00132c5a: add r0,pc
  00132c5c: ldr r0,[r0,#0x0]
  00132c5e: ldr r0,[r0,#0x0]
  00132c60: blx 0x00071cf8
  00132c64: ldr r0,[0x00132f78]
  00132c66: movs r1,#0x1f
  00132c68: add r0,pc
  00132c6a: ldr.w r8,[r0,#0x0]
  00132c6e: ldr.w r0,[r8,#0x0]
  00132c72: blx 0x00072f70
  00132c76: add r4,sp,#0x1b0
  00132c78: mov r1,r0
  00132c7a: movs r2,#0x0
  00132c7c: movs r5,#0x0
  00132c7e: mov r0,r4
  00132c80: blx 0x0006f028
  00132c84: ldr.w r0,[r10,#0x4]
  00132c88: strd r0,r5,[sp,#0x0]
  00132c8c: mov r0,r10
  00132c8e: movs r1,#0x3
  00132c90: mov r2,r4
  00132c92: movs r3,#0x0
  00132c94: blx 0x00076480
  00132c98: add r0,sp,#0x1b0
  00132c9a: blx 0x0006ee30
  00132c9e: ldr r0,[0x00132f7c]
  00132ca0: add r0,pc
  00132ca2: ldr.w r9,[r0,#0x0]
  00132ca6: ldr.w r0,[r9,#0x0]
  00132caa: blx 0x00071770
  00132cae: cmp r0,#0x10
  00132cb0: blt 0x00132cce
  00132cb2: ldr.w r0,[r9,#0x0]
  00132cb6: blx 0x000723d0
  00132cba: cbnz r0,0x00132cce
  00132cbc: ldr.w r0,[r9,#0x0]
  00132cc0: blx 0x00072850
  00132cc4: blx 0x00072874
  00132cc8: cmp r0,#0xb7
  00132cca: bne.w 0x001348ca
  00132cce: movs r5,#0x1
  00132cd0: ldr.w r0,[r9,#0x0]
  00132cd4: blx 0x00071770
  00132cd8: cmp r0,#0x2
  00132cda: blt 0x00132d20
  00132cdc: ldr.w r0,[r9,#0x0]
  00132ce0: blx 0x00072850
  00132ce4: blx 0x00072874
  00132ce8: cmp r0,#0xb7
  00132cea: beq 0x00132d20
  00132cec: ldr.w r0,[r8,#0x0]
  00132cf0: movs r1,#0xa6
  00132cf2: blx 0x00072f70
  00132cf6: add r4,sp,#0x198
  00132cf8: mov r1,r0
  00132cfa: movs r2,#0x0
  00132cfc: movs r6,#0x0
  00132cfe: mov r0,r4
  00132d00: blx 0x0006f028
  00132d04: ldr.w r0,[r10,#0x4]
  00132d08: strd r0,r6,[sp,#0x0]
  00132d0c: mov r0,r10
  00132d0e: movs r1,#0xc
  00132d10: mov r2,r4
  00132d12: mov r3,r5
  00132d14: blx 0x00076480
  00132d18: add r0,sp,#0x198
  00132d1a: blx 0x0006ee30
  00132d1e: adds r5,#0x1
  00132d20: ldr.w r0,[r8,#0x0]
  00132d24: movw r1,#0x20a
  00132d28: blx 0x00072f70
  00132d2c: add r4,sp,#0x18c
  00132d2e: mov r1,r0
  00132d30: movs r2,#0x0
  00132d32: movs r6,#0x0
  00132d34: mov r0,r4
  00132d36: blx 0x0006f028
  00132d3a: ldr.w r0,[r10,#0x4]
  00132d3e: strd r0,r6,[sp,#0x0]
  00132d42: mov r0,r10
  00132d44: movs r1,#0x6
  00132d46: mov r2,r4
  00132d48: mov r3,r5
  00132d4a: blx 0x00076480
  00132d4e: add r0,sp,#0x18c
  00132d50: blx 0x0006ee30
  00132d54: ldr.w r0,[r8,#0x0]
  00132d58: movs r1,#0x3b
  00132d5a: blx 0x00072f70
  00132d5e: add r4,sp,#0x180
  00132d60: mov r1,r0
  00132d62: movs r2,#0x0
  00132d64: mov r0,r4
  00132d66: blx 0x0006f028
  00132d6a: ldr.w r0,[r10,#0x4]
  00132d6e: adds r3,r5,#0x1
  00132d70: strd r0,r6,[sp,#0x0]
  00132d74: mov r0,r10
  00132d76: movs r1,#0x13
  00132d78: mov r2,r4
  00132d7a: blx 0x00076480
  00132d7e: add r0,sp,#0x180
  00132d80: blx 0x0006ee30
  00132d84: ldr.w r0,[r9,#0x0]
  00132d88: blx 0x00071770
  00132d8c: subs r0,#0x9a
  00132d8e: cmp r0,#0x4
  00132d90: bhi 0x00132dd4
  00132d92: movs r1,#0x1
  00132d94: lsl.w r0,r1,r0
  00132d98: tst r0,#0x19
  00132d9c: beq 0x00132dd4
  00132d9e: ldr.w r0,[r8,#0x0]
  00132da2: movw r1,#0x18b
  00132da6: adds r4,r5,#0x2
  00132da8: blx 0x00072f70
  00132dac: add r5,sp,#0x174
  00132dae: mov r1,r0
  00132db0: movs r2,#0x0
  00132db2: movs r6,#0x0
  00132db4: mov r0,r5
  00132db6: blx 0x0006f028
  00132dba: ldr.w r0,[r10,#0x4]
  00132dbe: strd r0,r6,[sp,#0x0]
  00132dc2: mov r0,r10
  00132dc4: movs r1,#0x12
  00132dc6: mov r2,r5
  00132dc8: mov r3,r4
  00132dca: blx 0x00076480
  00132dce: add r0,sp,#0x174
  00132dd0: blx 0x0006ee30
  00132dd4: movs r0,#0xc8
  00132dd6: blx 0x0006eb24
  00132dda: mov r5,r0
  00132ddc: ldr.w r0,[r8,#0x0]
  00132de0: movs r1,#0x3d
  00132de2: blx 0x00072f70
  00132de6: mov r1,r0
  00132de8: ldr r0,[0x00132f80]
  00132dea: ldr r2,[0x00132f84]
  00132dec: add r0,pc
  00132dee: add r2,pc
  00132df0: ldr.w r9,[r0,#0x0]
  00132df4: ldr r0,[sp,#0x64]
  00132df6: ldr r3,[r2,#0x0]
  00132df8: str r3,[sp,#0x68]
  00132dfa: ldr r0,[r0,#0x0]
  00132dfc: ldr.w r2,[r9,#0x0]
  00132e00: ldr r3,[r3,#0x0]
  00132e02: ldr r0,[r0,#0x2c]
  00132e04: movs r6,#0x22
  00132e06: subs r3,r3,r0
  00132e08: strd r3,r6,[sp,#0x0]
  00132e0c: subs r3,r2,r0
  00132e0e: mov r0,r5
  00132e10: movs r2,#0x0
  00132e12: movs r4,#0x0
  00132e14: blx 0x00074578
  00132e18: movs r0,#0x14
  00132e1a: strd r0,r4,[r5,#0x0]
  00132e1e: ldr r0,[sp,#0x6c]
  00132e20: ldr.w r1,[r0,#0xc0]
  00132e24: mov r0,r5
  00132e26: blx 0x0007648c
  00132e2a: movs r0,#0xc8
  00132e2c: blx 0x0006eb24
  00132e30: mov r6,r0
  00132e32: ldr.w r0,[r8,#0x0]
  00132e36: movs r1,#0x3c
  00132e38: blx 0x00072f70
  00132e3c: ldr.w r11,[sp,#0x64]
  00132e40: mov r8,r0
  00132e42: ldr.w r0,[r11,#0x0]
  00132e46: ldr.w r10,[r9,#0x0]
  00132e4a: ldr r4,[r0,#0x2c]
  00132e4c: mov r0,r5
  00132e4e: blx 0x00074da0
  00132e52: ldr.w r1,[r11,#0x0]
  00132e56: ldr r2,[sp,#0x68]
  00132e58: ldr r1,[r1,#0x2c]
  00132e5a: ldr r2,[r2,#0x0]
  00132e5c: subs r1,r2,r1
  00132e5e: movs r3,#0x22
  00132e60: strd r1,r3,[sp,#0x0]
  00132e64: sub.w r1,r10,r4, lsl #0x1
  00132e68: subs r3,r1,r0
  00132e6a: mov r0,r6
  00132e6c: mov r1,r8
  00132e6e: movs r2,#0x0
  00132e70: movs r5,#0x0
  00132e72: blx 0x00074578
  00132e76: ldr.w r10,[sp,#0x6c]
  00132e7a: movs r0,#0x15
  00132e7c: strd r0,r5,[r6,#0x0]
  00132e80: mov r0,r6
  00132e82: ldr.w r1,[r10,#0xc0]
  00132e86: blx 0x0007648c
  00132e8a: ldr.w r0,[r11,#0x0]
  00132e8e: ldr r2,[sp,#0x68]
  00132e90: ldr.w r1,[r9,#0x0]
  00132e94: ldr r0,[r0,#0x2c]
  00132e96: ldr r2,[r2,#0x0]
  00132e98: subs r1,r1,r0
  00132e9a: subs r2,r2,r0
  00132e9c: mov r0,r6
  00132e9e: blx 0x00076498
  00132ea2: b 0x0013301c
  00132ea4: movs r5,#0x1
  00132ea6: ldr.w r0,[r9,#0x0]
  00132eaa: movs r1,#0x1d
  00132eac: blx 0x00072f70
  00132eb0: add r4,sp,#0x228
  00132eb2: mov r1,r0
  00132eb4: movs r2,#0x0
  00132eb6: mov r0,r4
  00132eb8: blx 0x0006f028
  00132ebc: ldr.w r0,[r10,#0x4]
  00132ec0: strd r0,r8,[sp,#0x0]
  00132ec4: mov r0,r10
  00132ec6: movs r1,#0x1
  00132ec8: mov r2,r4
  00132eca: mov r3,r5
  00132ecc: blx 0x00076480
  00132ed0: add r0,sp,#0x228
  00132ed2: blx 0x0006ee30
  00132ed6: ldr.w r0,[r9,#0x0]
  00132eda: movs r1,#0x1f
  00132edc: blx 0x00072f70
  00132ee0: add r4,sp,#0x21c
  00132ee2: mov r1,r0
  00132ee4: movs r2,#0x0
  00132ee6: mov r0,r4
  00132ee8: blx 0x0006f028
  00132eec: ldr.w r0,[r10,#0x4]
  00132ef0: adds r3,r5,#0x1
  00132ef2: strd r0,r8,[sp,#0x0]
  00132ef6: mov r0,r10
  00132ef8: movs r1,#0x3
  00132efa: mov r2,r4
  00132efc: blx 0x00076480
  00132f00: add r0,sp,#0x21c
  00132f02: adds r6,r5,#0x2
  00132f04: blx 0x0006ee30
  00132f08: ldr r0,[0x00132f88]
  00132f0a: add r0,pc
  00132f0c: ldr r0,[r0,#0x0]
  00132f0e: ldrb r0,[r0,#0x0]
  00132f10: cbz r0,0x00132f44
  00132f12: ldr.w r0,[r9,#0x0]
  00132f16: movs r1,#0x0
  00132f18: blx 0x00072f70
  00132f1c: add r4,sp,#0x210
  00132f1e: mov r1,r0
  00132f20: movs r2,#0x0
  00132f22: mov r0,r4
  00132f24: blx 0x0006f028
  00132f28: ldr.w r0,[r10,#0x4]
  00132f2c: strd r0,r8,[sp,#0x0]
  00132f30: mov r0,r10
  00132f32: movs r1,#0x19
  00132f34: mov r2,r4
  00132f36: mov r3,r6
  00132f38: blx 0x00076480
  00132f3c: add r0,sp,#0x210
  00132f3e: blx 0x0006ee30
  00132f42: adds r6,r5,#0x3
  00132f44: ldr.w r0,[r9,#0x0]
  00132f48: movs r1,#0x2b
  00132f4a: blx 0x00072f70
  00132f4e: add r4,sp,#0x204
  00132f50: mov r1,r0
  00132f52: movs r2,#0x0
  00132f54: mov r0,r4
  00132f56: blx 0x0006f028
  00132f5a: ldr.w r0,[r10,#0x4]
  00132f5e: strd r0,r8,[sp,#0x0]
  00132f62: mov r0,r10
  00132f64: movs r1,#0x4
  00132f66: mov r2,r4
  00132f68: mov r3,r6
  00132f6a: blx 0x00076480
  00132f6e: add r0,sp,#0x204
  00132f70: b 0x00132fba
  00132f8c: movs r5,#0x4
  00132f8e: ldr r0,[r6,#0x0]
  00132f90: movs r1,#0x2b
  00132f92: blx 0x00072f70
  00132f96: add r4,sp,#0x1bc
  00132f98: mov r1,r0
  00132f9a: movs r2,#0x0
  00132f9c: movs r6,#0x0
  00132f9e: mov r0,r4
  00132fa0: blx 0x0006f028
  00132fa4: ldr.w r0,[r10,#0x4]
  00132fa8: strd r0,r6,[sp,#0x0]
  00132fac: mov r0,r10
  00132fae: movs r1,#0x4
  00132fb0: mov r2,r4
  00132fb2: mov r3,r5
  00132fb4: blx 0x00076480
  00132fb8: add r0,sp,#0x1bc
  00132fba: blx 0x0006ee30
  00132fbe: ldr.w r0,[r11,#0x0]
  00132fc2: ldrb.w r0,[r0,#0x285]
  00132fc6: cbz r0,0x0013301a
  00132fc8: movs r0,#0xc8
  00132fca: blx 0x0006eb24
  00132fce: mov r5,r0
  00132fd0: ldr r0,[0x001333b8]
  00132fd2: add r0,pc
  00132fd4: ldr r0,[r0,#0x0]
  00132fd6: ldr r0,[r0,#0x0]
  00132fd8: movs r1,#0x21
  00132fda: blx 0x00072f70
  00132fde: mov r1,r0
  00132fe0: ldr r0,[0x001333bc]
  00132fe2: ldr r2,[0x001333c0]
  00132fe4: add r0,pc
  00132fe6: ldr.w r3,[r11,#0x0]
  00132fea: add r2,pc
  00132fec: ldr r0,[r0,#0x0]
  00132fee: ldr r2,[r2,#0x0]
  00132ff0: ldr r3,[r3,#0x2c]
  00132ff2: ldr r0,[r0,#0x0]
  00132ff4: ldr r2,[r2,#0x0]
  00132ff6: subs r2,r2,r3
  00132ff8: movs r6,#0x22
  00132ffa: subs r3,r0,r3
  00132ffc: strd r2,r6,[sp,#0x0]
  00133000: mov r0,r5
  00133002: movs r2,#0x0
  00133004: movs r4,#0x0
  00133006: blx 0x00074578
  0013300a: movs r0,#0x11
  0013300c: strd r0,r4,[r5,#0x0]
  00133010: mov r0,r5
  00133012: ldr.w r1,[r10,#0xc0]
  00133016: blx 0x0007648c
  0013301a: movs r5,#0x1
  0013301c: movs r0,#0xc8
  0013301e: blx 0x0006eb24
  00133022: mov r8,r0
  00133024: ldr r0,[0x001333c4]
  00133026: add r0,pc
  00133028: ldr r0,[r0,#0x0]
  0013302a: str r0,[sp,#0x60]
  0013302c: ldr r0,[r0,#0x0]
  0013302e: movs r1,#0x3e
  00133030: blx 0x00072f70
  00133034: mov r1,r0
  00133036: ldr r0,[0x001333c8]
  00133038: ldr r2,[0x001333cc]
  0013303a: add r0,pc
  0013303c: add r2,pc
  0013303e: ldr r6,[r0,#0x0]
  00133040: ldr.w r0,[r11,#0x0]
  00133044: ldr r3,[r2,#0x0]
  00133046: str r3,[sp,#0x68]
  00133048: ldr r0,[r0,#0x2c]
  0013304a: str r6,[sp,#0x5c]
  0013304c: ldr r2,[r6,#0x0]
  0013304e: ldr r3,[r3,#0x0]
  00133050: movs r6,#0x22
  00133052: subs r3,r3,r0
  00133054: strd r3,r6,[sp,#0x0]
  00133058: subs r3,r2,r0
  0013305a: mov r0,r8
  0013305c: movs r2,#0x0
  0013305e: movs r4,#0x0
  00133060: blx 0x00074578
  00133064: movs r0,#0x16
  00133066: strd r0,r4,[r8,#0x0]
  0013306a: mov r0,r8
  0013306c: ldr.w r1,[r10,#0xc0]
  00133070: blx 0x0007648c
  00133074: ldr.w r0,[r10,#0x4]
  00133078: cbz r0,0x00133096
  0013307a: ldrd r1,r0,[r0,#0x0]
  0013307e: subs r1,#0x1
  00133080: cmp r1,#0x4
  00133082: it cs
  00133084: mov.cs r1,#0x4
  00133086: ldr.w r1,[r0,r1,lsl #0x2]
  0013308a: add r0,sp,#0x168
  0013308c: blx 0x00074dd0
  00133090: vldr.32 s16,[sp,#0x168]
  00133094: b 0x001330a2
  00133096: vldr.32 s16,[pc,#0x338]
  0013309a: movs r0,#0x0
  0013309c: strd r0,r0,[sp,#0x168]
  001330a0: str r0,[sp,#0x170]
  001330a2: ldr r0,[sp,#0x48]
  001330a4: ldr r4,[r0,#0x0]
  001330a6: movs r0,#0xc8
  001330a8: blx 0x0006eb24
  001330ac: ldr r1,[0x001333d4]
  001330ae: mov r6,r0
  001330b0: add r1,pc
  001330b2: add r0,sp,#0x15c
  001330b4: movs r2,#0x0
  001330b6: blx 0x0006ee18
  001330ba: mov r0,r4
  001330bc: movs r1,#0x3
  001330be: blx 0x0007198c
  001330c2: vmov s0,r4
  001330c6: vmov s2,r0
  001330ca: vcvt.f32.s32 s0,s0
  001330ce: vcvt.f32.s32 s2,s2
  001330d2: vldr.32 s4,[sp,#0x16c]
  001330d6: vadd.f32 s0,s16,s0
  001330da: vadd.f32 s0,s0,s2
  001330de: movs r0,#0x44
  001330e0: vcvt.s32.f32 s2,s4
  001330e4: add r1,sp,#0x15c
  001330e6: movs r2,#0x11
  001330e8: vcvt.s32.f32 s0,s0
  001330ec: str r0,[sp,#0x4]
  001330ee: mov r0,r6
  001330f0: vstr.32 s2,[sp]
  001330f4: vmov r3,s0
  001330f8: blx 0x00074578
  001330fc: add r0,sp,#0x15c
  001330fe: blx 0x0006ee30
  00133102: movs r0,#0x0
  00133104: movs r1,#0x17
  00133106: strd r1,r0,[r6,#0x0]
  0013310a: mov r0,r6
  0013310c: ldr.w r1,[r10,#0xc0]
  00133110: blx 0x0007648c
  00133114: movs r0,#0xc8
  00133116: ldr.w r4,[r10,#0x1a8]
  0013311a: vldr.32 s16,[sp,#0x168]
  0013311e: blx 0x0006eb24
  00133122: ldr r1,[0x001333d8]
  00133124: mov r6,r0
  00133126: add r1,pc
  00133128: add r0,sp,#0x15c
  0013312a: movs r2,#0x0
  0013312c: blx 0x0006ee18
  00133130: mov r0,r4
  00133132: movs r1,#0x3
  00133134: str r5,[sp,#0x58]
  00133136: blx 0x0007198c
  0013313a: vmov s0,r0
  0013313e: vcvt.f32.s32 s0,s0
  00133142: vldr.32 s2,[sp,#0x16c]
  00133146: vsub.f32 s0,s16,s0
  0013314a: movs r0,#0x44
  0013314c: vcvt.s32.f32 s2,s2
  00133150: add r1,sp,#0x15c
  00133152: movs r2,#0x11
  00133154: vcvt.s32.f32 s0,s0
  00133158: str r0,[sp,#0x4]
  0013315a: mov r0,r6
  0013315c: vstr.32 s2,[sp]
  00133160: vmov r3,s0
  00133164: blx 0x00074578
  00133168: add r0,sp,#0x15c
  0013316a: blx 0x0006ee30
  0013316e: movs r0,#0x0
  00133170: movs r1,#0x18
  00133172: strd r1,r0,[r6,#0x0]
  00133176: mov r0,r6
  00133178: ldr.w r1,[r10,#0xc0]
  0013317c: blx 0x0007648c
  00133180: movs r0,#0xc8
  00133182: blx 0x0006eb24
  00133186: mov r6,r0
  00133188: ldr r0,[sp,#0x60]
  0013318a: ldr r0,[r0,#0x0]
  0013318c: movw r1,#0x1388
  00133190: blx 0x00072f70
  00133194: mov r9,r0
  00133196: ldr.w r0,[r11,#0x0]
  0013319a: ldr r1,[sp,#0x5c]
  0013319c: ldr r4,[r0,#0x2c]
  0013319e: ldr r5,[r1,#0x0]
  001331a0: mov r0,r8
  001331a2: blx 0x00074da0
  001331a6: ldr.w r1,[r11,#0x0]
  001331aa: ldr r2,[sp,#0x68]
  001331ac: ldr r1,[r1,#0x2c]
  001331ae: ldr r2,[r2,#0x0]
  001331b0: subs r1,r2,r1
  001331b2: movs r3,#0x22
  001331b4: strd r1,r3,[sp,#0x0]
  001331b8: sub.w r1,r5,r4, lsl #0x1
  001331bc: subs r3,r1,r0
  001331be: mov r0,r6
  001331c0: mov r1,r9
  001331c2: movs r2,#0x0
  001331c4: movs r4,#0x0
  001331c6: blx 0x00074578
  001331ca: movs r0,#0x6d
  001331cc: strd r0,r4,[r6,#0x0]
  001331d0: mov r0,r6
  001331d2: ldr.w r1,[r10,#0xc0]
  001331d6: blx 0x0007648c
  001331da: movs r0,#0xc8
  001331dc: blx 0x0006eb24
  001331e0: mov r5,r0
  001331e2: ldr r0,[sp,#0x60]
  001331e4: ldr r0,[r0,#0x0]
  001331e6: movw r1,#0x1389
  001331ea: blx 0x00072f70
  001331ee: mov r9,r0
  001331f0: ldr r0,[sp,#0x64]
  001331f2: ldr r1,[sp,#0x5c]
  001331f4: ldr r0,[r0,#0x0]
  001331f6: ldr.w r10,[r1,#0x0]
  001331fa: ldr.w r11,[r0,#0x2c]
  001331fe: mov r0,r8
  00133200: blx 0x00074da0
  00133204: mov r4,r0
  00133206: mov r0,r6
  00133208: blx 0x00074da0
  0013320c: ldr r1,[sp,#0x64]
  0013320e: ldr r2,[sp,#0x68]
  00133210: ldr r1,[r1,#0x0]
  00133212: ldr r2,[r2,#0x0]
  00133214: ldr r1,[r1,#0x2c]
  00133216: subs r1,r2,r1
  00133218: movs r3,#0x22
  0013321a: strd r1,r3,[sp,#0x0]
  0013321e: sub.w r1,r11,r11, lsl #0x2
  00133222: add r1,r10
  00133224: movs r2,#0x0
  00133226: subs r1,r1,r4
  00133228: movs r4,#0x0
  0013322a: subs r3,r1,r0
  0013322c: mov r0,r5
  0013322e: mov r1,r9
  00133230: blx 0x00074578
  00133234: ldr.w r11,[sp,#0x6c]
  00133238: movs r0,#0x6e
  0013323a: strd r0,r4,[r5,#0x0]
  0013323e: mov r0,r5
  00133240: ldr.w r1,[r11,#0xc0]
  00133244: blx 0x0007648c
  00133248: movs r0,#0xc
  0013324a: blx 0x0006eb24
  0013324e: mov r4,r0
  00133250: blx 0x00074f38
  00133254: ldr r6,[sp,#0x60]
  00133256: movw r1,#0x1e9
  0013325a: str.w r4,[r11,#0xac]
  0013325e: ldr r0,[r6,#0x0]
  00133260: blx 0x00072f70
  00133264: add r4,sp,#0x150
  00133266: mov r1,r0
  00133268: movs r2,#0x0
  0013326a: movs r5,#0x0
  0013326c: mov r0,r4
  0013326e: blx 0x0006f028
  00133272: ldr.w r0,[r11,#0xac]
  00133276: strd r0,r5,[sp,#0x0]
  0013327a: mov r0,r11
  0013327c: movs r1,#0x8
  0013327e: mov r2,r4
  00133280: movs r3,#0x0
  00133282: blx 0x00076480
  00133286: ldr.w r9,[sp,#0x5c]
  0013328a: add r0,sp,#0x150
  0013328c: blx 0x0006ee30
  00133290: ldr r0,[r6,#0x0]
  00133292: mov.w r1,#0x1f2
  00133296: blx 0x00072f70
  0013329a: add r4,sp,#0x144
  0013329c: mov r1,r0
  0013329e: movs r2,#0x0
  001332a0: mov r0,r4
  001332a2: blx 0x0006f028
  001332a6: ldr.w r0,[r11,#0xac]
  001332aa: strd r0,r5,[sp,#0x0]
  001332ae: mov r0,r11
  001332b0: movs r1,#0x9
  001332b2: mov r2,r4
  001332b4: movs r3,#0x1
  001332b6: blx 0x00076480
  001332ba: add r0,sp,#0x144
  001332bc: blx 0x0006ee30
  001332c0: ldr r0,[sp,#0x58]
  001332c2: cmp r0,#0x0
  001332c4: beq.w 0x001334b2
  001332c8: ldr r0,[sp,#0x60]
  001332ca: movs r1,#0x0
  001332cc: movs r5,#0x0
  001332ce: ldr r0,[r0,#0x0]
  001332d0: blx 0x00072f70
  001332d4: add r4,sp,#0x138
  001332d6: mov r1,r0
  001332d8: movs r2,#0x0
  001332da: mov r0,r4
  001332dc: blx 0x0006f028
  001332e0: ldr.w r0,[r11,#0xac]
  001332e4: strd r0,r5,[sp,#0x0]
  001332e8: mov r0,r11
  001332ea: movs r1,#0x19
  001332ec: mov r2,r4
  001332ee: movs r3,#0x2
  001332f0: blx 0x00076480
  001332f4: add r0,sp,#0x138
  001332f6: blx 0x0006ee30
  001332fa: movs r0,#0xc
  001332fc: blx 0x0006eb24
  00133300: mov r4,r0
  00133302: blx 0x00074f38
  00133306: ldr r0,[sp,#0x64]
  00133308: vmov.f32 s16,0xbf000000
  0013330c: str.w r4,[r11,#0xb0]
  00133310: mov.w r8,#0x0
  00133314: vldr.32 s2,[pc,#0xc4]
  00133318: ldr r0,[r0,#0x0]
  0013331a: vldr.32 s0,[r0,#0x34]
  0013331e: ldr r0,[0x001333e0]
  00133320: vcvt.f32.s32 s0,s0
  00133324: add r0,pc
  00133326: ldr r0,[r0,#0x0]
  00133328: str r0,[sp,#0x44]
  0013332a: ldrb r0,[r0,#0x0]
  0013332c: vmul.f32 s2,s0,s2
  00133330: cmp r0,#0x0
  00133332: it eq
  00133334: vmov.eq.f32 s0,s2
  00133338: vcvt.s32.f32 s0,s0
  0013333c: vmov r0,s0
  00133340: adds r0,#0x2
  00133342: str r0,[sp,#0x40]
  00133344: b 0x001334aa
  00133346: ldr r0,[sp,#0x64]
  00133348: tst r8,#0x1
  0013334c: ldr r1,[r0,#0x0]
  0013334e: ldr r0,[sp,#0x68]
  00133350: ldr r6,[r0,#0x0]
  00133352: ldrd r0,r5,[r11,#0x1a8]
  00133356: ldr r3,[r1,#0x30]
  00133358: add.w r6,r6,r6, lsr #0x1f
  0013335c: ldr.w r2,[r1,#0x29c]
  00133360: rsb r6,r5,r6, asr #0x1
  00133364: ldr.w r5,[r9,#0x0]
  00133368: ldr r1,[sp,#0x40]
  0013336a: vmov s2,r0
  0013336e: asr.w r4,r5, asr #0x1f
  00133372: add.w r12,r1,r3
  00133376: add.w r4,r5,r4, lsr #0x1e
  0013337a: sub.w r1,r5,r4, asr #0x2
  0013337e: it eq
  00133380: asr.eq r1,r4,#0x2
  00133382: vmov s0,r1
  00133386: ldr r1,[sp,#0x44]
  00133388: ldrb r1,[r1,#0x0]
  0013338a: vcvt.f32.s32 s2,s2
  0013338e: vcvt.f32.s32 s0,s0
  00133392: cmp r1,#0x0
  00133394: vmla.f32 s0,s2,s16
  00133398: vcvt.s32.f32 s0,s0
  0013339c: beq 0x001333e4
  0013339e: subs r1,r6,r2
  001333a0: add.w r2,r5,r5, lsr #0x1f
  001333a4: subs r1,r1,r3
  001333a6: add.w r0,r0,r0, lsr #0x1f
  001333aa: mla r1,r12,r8,r1
  001333ae: str r1,[sp,#0x54]
  001333b0: asrs r1,r2,#0x1
  001333b2: sub.w r0,r1,r0, asr #0x1
  001333b6: b 0x001333f8
  001333e4: sub.w r0,r6,#0xc
  001333e8: lsr.w r1,r8,#0x1
  001333ec: subs r0,r0,r2
  001333ee: mla r0,r12,r1,r0
  001333f2: str r0,[sp,#0x54]
  001333f4: vmov r0,s0
  001333f8: str r0,[sp,#0x58]
  001333fa: orr r10,r8,#0x1
  001333fe: ldr r0,[0x001336d0]
  00133400: mov r9,r8
  00133402: ldr r1,[0x001336d4]
  00133404: cmp.w r10,#0x9
  00133408: ldr r2,[0x001336d8]
  0013340a: add r0,pc
  0013340c: add r1,pc
  0013340e: add r2,pc
  00133410: ldr r0,[r0,#0x0]
  00133412: ldr r1,[r1,#0x0]
  00133414: ldr r2,[r2,#0x0]
  00133416: ldr r5,[r0,#0x0]
  00133418: ldr r0,[r1,#0x0]
  0013341a: ldr.w r8,[r2,#0x0]
  0013341e: mov r1,r5
  00133420: str r2,[sp,#0x4c]
  00133422: it eq
  00133424: mov.eq r1,r8
  00133426: blx 0x000764a4
  0013342a: mov r4,r0
  0013342c: ldr r0,[sp,#0x48]
  0013342e: ldr.w r11,[r0,#0x0]
  00133432: movs r0,#0xc8
  00133434: blx 0x0006eb24
  00133438: mov r6,r0
  0013343a: ldr r0,[0x001336dc]
  0013343c: str.w r9,[sp,#0x50]
  00133440: add r0,pc
  00133442: ldr.w r1,[r0,r9,lsl #0x2]
  00133446: ldr r0,[sp,#0x60]
  00133448: ldr r0,[r0,#0x0]
  0013344a: blx 0x00072f70
  0013344e: mov r1,r0
  00133450: cmp.w r10,#0x9
  00133454: it eq
  00133456: mov.eq r5,r8
  00133458: ldr r0,[sp,#0x54]
  0013345a: movs r2,#0x0
  0013345c: strd r0,r11,[sp,#0x0]
  00133460: movs r0,#0x11
  00133462: str r0,[sp,#0x8]
  00133464: movs r0,#0x4
  00133466: strd r0,r5,[sp,#0xc]
  0013346a: mov r0,r6
  0013346c: ldr r3,[sp,#0x58]
  0013346e: str r4,[sp,#0x14]
  00133470: blx 0x000764b0
  00133474: ldr r0,[0x001336e0]
  00133476: cmp.w r10,#0x9
  0013347a: ldr.w r8,[sp,#0x50]
  0013347e: add r0,pc
  00133480: ldr.w r0,[r0,r8,lsl #0x2]
  00133484: asr.w r1,r0, asr #0x1f
  00133488: strd r0,r1,[r6,#0x0]
  0013348c: ittt eq
  0013348e: ldr.eq r0,[sp,#0x4c]
  00133490: ldr.eq r0,[r0,#0x0]
  00133492: str.eq r0,[r6,#0x8]
  00133494: ldr.w r11,[sp,#0x6c]
  00133498: mov r0,r6
  0013349a: ldr.w r1,[r11,#0xb0]
  0013349e: blx 0x0007648c
  001334a2: add.w r8,r8,#0x1
  001334a6: ldr.w r9,[sp,#0x5c]
  001334aa: cmp.w r8,#0xa
  001334ae: bcc.w 0x00133346
  001334b2: ldr r0,[0x001336e4]
  001334b4: add.w r2,r11,#0x10c
  001334b8: ldr r1,[0x001336e8]
  001334ba: add r0,pc
  001334bc: add r1,pc
  001334be: ldr r5,[r0,#0x0]
  001334c0: ldr.w r8,[r1,#0x0]
  001334c4: movw r1,#0x515
  001334c8: ldr r0,[r5,#0x0]
  001334ca: blx r8
  001334cc: ldr r0,[r5,#0x0]
  001334ce: add.w r2,r11,#0x110
  001334d2: movw r1,#0x516
  001334d6: blx r8
  001334d8: ldr r0,[r5,#0x0]
  001334da: add.w r2,r11,#0x114
  001334de: movw r1,#0x517
  001334e2: blx r8
  001334e4: ldr r0,[r5,#0x0]
  001334e6: add.w r2,r11,#0x118
  001334ea: mov.w r1,#0x518
  001334ee: blx r8
  001334f0: movs r0,#0x14
  001334f2: blx 0x0006ec08
  001334f6: mov r4,r0
  001334f8: str.w r0,[r11,#0x134]
  001334fc: movs r0,#0x14
  001334fe: blx 0x0006ec08
  00133502: str.w r0,[r11,#0x138]
  00133506: movw r1,#0xbba
  0013350a: ldr r0,[r5,#0x0]
  0013350c: mov r2,r4
  0013350e: blx r8
  00133510: ldr.w r1,[r11,#0x134]
  00133514: ldr r0,[r5,#0x0]
  00133516: adds r2,r1,#0x4
  00133518: movw r1,#0xbbc
  0013351c: blx r8
  0013351e: ldr.w r1,[r11,#0x134]
  00133522: ldr r0,[r5,#0x0]
  00133524: add.w r2,r1,#0x8
  00133528: movw r1,#0xbc3
  0013352c: blx r8
  0013352e: ldr.w r1,[r11,#0x134]
  00133532: ldr r0,[r5,#0x0]
  00133534: add.w r2,r1,#0xc
  00133538: movw r1,#0xbc5
  0013353c: blx r8
  0013353e: ldr.w r1,[r11,#0x134]
  00133542: ldr r0,[r5,#0x0]
  00133544: add.w r2,r1,#0x10
  00133548: movw r1,#0xbc7
  0013354c: blx r8
  0013354e: ldr.w r2,[r11,#0x138]
  00133552: movw r1,#0xbbb
  00133556: ldr r0,[r5,#0x0]
  00133558: blx r8
  0013355a: ldr r0,[0x001336ec]
  0013355c: ldr.w r1,[r11,#0x138]
  00133560: add r0,pc
  00133562: adds r2,r1,#0x4
  00133564: ldr r4,[r0,#0x0]
  00133566: ldr r0,[r5,#0x0]
  00133568: ldrb r3,[r4,#0x0]
  0013356a: cmp r3,#0x0
  0013356c: ite eq
  0013356e: movw.eq r1,#0xbbd
  00133572: movw.ne r1,#0xbc6
  00133576: blx 0x00071cf8
  0013357a: ldr.w r1,[r11,#0x138]
  0013357e: ldr r0,[r5,#0x0]
  00133580: add.w r2,r1,#0x8
  00133584: movw r1,#0xbc4
  00133588: blx r8
  0013358a: ldr.w r1,[r11,#0x138]
  0013358e: ldr r0,[r5,#0x0]
  00133590: add.w r2,r1,#0xc
  00133594: movw r1,#0xbc6
  00133598: blx r8
  0013359a: ldr.w r1,[r11,#0x138]
  0013359e: ldr r0,[r5,#0x0]
  001335a0: add.w r2,r1,#0x10
  001335a4: movw r1,#0xbc8
  001335a8: blx r8
  001335aa: ldr r0,[0x001336f0]
  001335ac: add r0,pc
  001335ae: ldr r0,[r0,#0x0]
  001335b0: str r0,[sp,#0x54]
  001335b2: ldrb r0,[r4,#0x0]
  001335b4: cbz r0,0x001335ca
  001335b6: ldr r0,[sp,#0x54]
  001335b8: ldrb r0,[r0,#0x0]
  001335ba: cbnz r0,0x001335ca
  001335bc: ldr r0,[r5,#0x0]
  001335be: add.w r2,r11,#0x14c
  001335c2: movw r1,#0xc1d
  001335c6: blx 0x00071cf8
  001335ca: add.w r10,r11,#0x13c
  001335ce: ldr r0,[r5,#0x0]
  001335d0: movw r1,#0xbb8
  001335d4: mov r2,r10
  001335d6: blx r8
  001335d8: ldr r0,[r5,#0x0]
  001335da: add.w r2,r11,#0x140
  001335de: movw r1,#0xbb9
  001335e2: blx r8
  001335e4: ldr r0,[r5,#0x0]
  001335e6: add.w r2,r11,#0x144
  001335ea: movw r1,#0xbbe
  001335ee: blx r8
  001335f0: ldr r0,[r5,#0x0]
  001335f2: add.w r2,r11,#0x148
  001335f6: movw r1,#0xbbf
  001335fa: blx r8
  001335fc: ldr.w r1,[r11,#0x134]
  00133600: movs r6,#0x0
  00133602: strb.w r6,[r11,#0x1d9]
  00133606: ldr r0,[r5,#0x0]
  00133608: ldr r1,[r1,#0x0]
  0013360a: blx 0x00072d90
  0013360e: ldr.w r1,[r11,#0x134]
  00133612: str.w r0,[r11,#0x1e4]
  00133616: ldr r0,[r5,#0x0]
  00133618: ldr r1,[r1,#0x0]
  0013361a: blx 0x00072d84
  0013361e: str.w r0,[r11,#0x1e8]
  00133622: ldr r0,[sp,#0x64]
  00133624: ldr r0,[r0,#0x0]
  00133626: ldrd r1,r2,[r0,#0x28]
  0013362a: add r1,r2
  0013362c: str.w r1,[r11,#0x1ec]
  00133630: ldr r1,[r0,#0xc]
  00133632: ldr r0,[r0,#0x20]
  00133634: add r0,r1
  00133636: str.w r0,[r11,#0x1f0]
  0013363a: ldrb r0,[r4,#0x0]
  0013363c: cbz r0,0x00133644
  0013363e: ldr r0,[sp,#0x54]
  00133640: ldrb r0,[r0,#0x0]
  00133642: cbz r0,0x00133650
  00133644: ldr.w r1,[r10,#0x0]
  00133648: ldr r0,[r5,#0x0]
  0013364a: blx 0x00072d84
  0013364e: mov r6,r0
  00133650: ldr.w r0,[r11,#0x138]
  00133654: str.w r6,[r11,#0x1f4]
  00133658: str r4,[sp,#0x50]
  0013365a: ldr r1,[r0,#0x0]
  0013365c: ldr r0,[r5,#0x0]
  0013365e: blx 0x00072d84
  00133662: ldr.w r1,[r11,#0x138]
  00133666: str.w r0,[r11,#0x1f8]
  0013366a: ldr r0,[r5,#0x0]
  0013366c: ldr r1,[r1,#0x0]
  0013366e: str r5,[sp,#0x58]
  00133670: blx 0x00072d90
  00133674: ldr r5,[sp,#0x64]
  00133676: str.w r0,[r11,#0x1fc]
  0013367a: ldr.w r2,[r11,#0x1e8]
  0013367e: ldr r0,[r5,#0x0]
  00133680: ldr.w r3,[r11,#0x1f4]
  00133684: ldr r1,[r0,#0x2c]
  00133686: add.w r1,r2,r1, lsl #0x1
  0013368a: str.w r1,[r11,#0x200]
  0013368e: ldr.w r2,[r9,#0x0]
  00133692: ldr r0,[r0,#0x28]
  00133694: subs r1,r2,r1
  00133696: sub.w r0,r1,r0, lsl #0x1
  0013369a: subs r0,r0,r3
  0013369c: str.w r0,[r11,#0x204]
  001336a0: movs r0,#0xc
  001336a2: blx 0x0006eb24
  001336a6: mov r4,r0
  001336a8: blx 0x00074f38
  001336ac: movs r0,#0x5
  001336ae: mov r1,r4
  001336b0: str.w r4,[r11,#0xf8]
  001336b4: blx 0x00074f44
  001336b8: ldr r0,[r5,#0x0]
  001336ba: mov.w r9,#0x0
  001336be: mov.w r10,#0x11
  001336c2: movs r4,#0x0
  001336c4: ldr r1,[r0,#0x2c]
  001336c6: ldr.w r0,[r11,#0xf8]
  001336ca: str.w r1,[r11,#0x208]
  001336ce: b 0x00133740
  001336f4: movs r0,#0xc8
  001336f6: blx 0x0006eb24
  001336fa: mov r5,r0
  001336fc: ldr.w r0,[r11,#0x134]
  00133700: ldr.w r1,[r11,#0x1e4]
  00133704: ldrd r3,r2,[r11,#0x1ec]
  00133708: ldr.w r6,[r11,#0x208]
  0013370c: add r6,r1
  0013370e: ldr.w r1,[r0,r4,lsl #0x2]
  00133712: mla r0,r6,r4,r2
  00133716: strd r0,r10,[sp,#0x0]
  0013371a: mov r0,r5
  0013371c: movs r2,#0x10
  0013371e: blx 0x00074dac
  00133722: ldr.w r0,[r11,#0xf8]
  00133726: add.w r2,r4,#0x36
  0013372a: ldr r0,[r0,#0x4]
  0013372c: str.w r5,[r0,r4,lsl #0x2]
  00133730: ldr.w r0,[r11,#0xf8]
  00133734: ldr r1,[r0,#0x4]
  00133736: ldr.w r1,[r1,r4,lsl #0x2]
  0013373a: adds r4,#0x1
  0013373c: strd r2,r9,[r1,#0x0]
  00133740: ldr r0,[r0,#0x0]
  00133742: cmp r4,r0
  00133744: bcc 0x001336f4
  00133746: ldr r0,[sp,#0x64]
  00133748: ldr r1,[sp,#0x50]
  0013374a: ldr r6,[r0,#0x0]
  0013374c: ldr r0,[sp,#0x68]
  0013374e: ldrb r1,[r1,#0x0]
  00133750: ldr r0,[r0,#0x0]
  00133752: cmp r1,#0x0
  00133754: ldrd r2,r3,[r6,#0xc]
  00133758: ldrd r5,r4,[r6,#0x20]
  0013375c: sub.w r0,r0,r2
  00133760: ldr.w r2,[r11,#0x1fc]
  00133764: sub.w r0,r0,r5
  00133768: ldr r5,[r6,#0x2c]
  0013376a: sub.w r0,r0,r2
  0013376e: sub.w r0,r0,r5
  00133772: sub.w r0,r0,r3
  00133776: sub.w r9,r0,r4
  0013377a: beq 0x00133796
  0013377c: ldr r0,[sp,#0x54]
  0013377e: ldrb r0,[r0,#0x0]
  00133780: cbnz r0,0x00133796
  00133782: ldr r0,[sp,#0x58]
  00133784: ldr.w r1,[r11,#0x14c]
  00133788: ldr r0,[r0,#0x0]
  0013378a: blx 0x00072d90
  0013378e: ldr r1,[sp,#0x64]
  00133790: sub.w r9,r9,r0, lsl #0x1
  00133794: ldr r6,[r1,#0x0]
  00133796: movs r0,#0x24
  00133798: blx 0x0006eb24
  0013379c: mov r10,r0
  0013379e: ldr r0,[r6,#0xc]
  001337a0: ldr r1,[r6,#0x20]
  001337a2: ldrd r4,r2,[r6,#0x28]
  001337a6: ldr r5,[sp,#0x5c]
  001337a8: ldr.w r6,[r11,#0x1fc]
  001337ac: ldr.w r3,[r11,#0x204]
  001337b0: ldr r5,[r5,#0x0]
  001337b2: add r0,r1
  001337b4: mov.w r12,#0x0
  001337b8: add r0,r6
  001337ba: strd r9,r12,[sp,#0x0]
  001337be: add r2,r0
  001337c0: subs r0,r5,r4
  001337c2: subs r1,r0,r3
  001337c4: mov r0,r10
  001337c6: blx 0x00075b20
  001337ca: ldr r1,[0x00133b5c]
  001337cc: add r0,sp,#0x12c
  001337ce: movs r2,#0x0
  001337d0: str.w r10,[r11,#0xf4]
  001337d4: add r1,pc
  001337d6: blx 0x0006ee18
  001337da: ldr r0,[sp,#0x60]
  001337dc: ldr.w r1,[r11,#0x1e0]
  001337e0: ldr r0,[r0,#0x0]
  001337e2: adds r1,#0x57
  001337e4: blx 0x00072f70
  001337e8: mov r1,r0
  001337ea: add r0,sp,#0x120
  001337ec: movs r2,#0x0
  001337ee: blx 0x0006f028
  001337f2: add r1,sp,#0x12c
  001337f4: add r2,sp,#0x120
  001337f6: mov r0,r10
  001337f8: blx 0x00075b2c
  001337fc: add r0,sp,#0x120
  001337fe: blx 0x0006ee30
  00133802: add r0,sp,#0x12c
  00133804: blx 0x0006ee30
  00133808: ldr.w r0,[r11,#0x4]
  0013380c: ldr.w r6,[r11,#0x1a8]
  00133810: vldr.32 s16,[sp,#0x168]
  00133814: ldr r0,[r0,#0x4]
  00133816: ldr r1,[r0,#0x0]
  00133818: add r0,sp,#0x15c
  0013381a: blx 0x00074dd0
  0013381e: ldr r0,[sp,#0x64]
  00133820: vldr.32 s18,[sp,#0x160]
  00133824: ldr r0,[r0,#0x0]
  00133826: ldr r4,[r0,#0x2c]
  00133828: movs r0,#0xc8
  0013382a: blx 0x0006eb24
  0013382e: mov r5,r0
  00133830: ldr r0,[sp,#0x60]
  00133832: ldr r0,[r0,#0x0]
  00133834: movs r1,#0x61
  00133836: blx 0x00072f70
  0013383a: ldr.w r10,[sp,#0x5c]
  0013383e: mov r9,r0
  00133840: mov r0,r6
  00133842: movs r1,#0x3
  00133844: blx 0x0007198c
  00133848: add.w r1,r4,r4, lsl #0x1
  0013384c: vmov s0,r6
  00133850: vmov s4,r0
  00133854: vmov s2,r1
  00133858: vcvt.f32.s32 s0,s0
  0013385c: vcvt.f32.s32 s2,s2
  00133860: vcvt.f32.s32 s4,s4
  00133864: vadd.f32 s0,s16,s0
  00133868: vsub.f32 s2,s18,s2
  0013386c: vadd.f32 s0,s0,s4
  00133870: movs r0,#0x14
  00133872: vcvt.s32.f32 s2,s2
  00133876: str r0,[sp,#0x4]
  00133878: mov r0,r5
  0013387a: mov r1,r9
  0013387c: movs r2,#0x11
  0013387e: vcvt.s32.f32 s0,s0
  00133882: vstr.32 s2,[sp]
  00133886: vmov r3,s0
  0013388a: blx 0x00074578
  0013388e: movs r0,#0x0
  00133890: movs r1,#0x35
  00133892: strd r1,r0,[r5,#0x0]
  00133896: mov r0,r5
  00133898: ldr.w r1,[r11,#0xc0]
  0013389c: mov r4,r10
  0013389e: blx 0x0007648c
  001338a2: ldr r0,[0x00133b60]
  001338a4: add r0,pc
  001338a6: ldr r0,[r0,#0x0]
  001338a8: ldrb r0,[r0,#0x0]
  001338aa: cbnz r0,0x001338b4
  001338ac: mov r0,r5
  001338ae: movs r1,#0x0
  001338b0: blx 0x00074e48
  001338b4: ldr.w r10,[sp,#0x58]
  001338b8: add.w r2,r11,#0x124
  001338bc: movw r1,#0xbc2
  001338c0: ldr.w r0,[r10,#0x0]
  001338c4: blx 0x00071cf8
  001338c8: ldr.w r0,[r10,#0x0]
  001338cc: add.w r2,r11,#0x128
  001338d0: movw r1,#0x233f
  001338d4: blx 0x00071cf8
  001338d8: ldr r0,[sp,#0x50]
  001338da: ldrb r0,[r0,#0x0]
  001338dc: cmp r0,#0x0
  001338de: beq.w 0x00133b2c
  001338e2: ldr.w r0,[r10,#0x0]
  001338e6: add.w r2,r11,#0x1c
  001338ea: movw r1,#0x4c1
  001338ee: blx r8
  001338f0: ldr.w r0,[r10,#0x0]
  001338f4: add.w r2,r11,#0x20
  001338f8: movw r1,#0x4b2
  001338fc: blx r8
  001338fe: ldr.w r0,[r10,#0x0]
  00133902: add.w r2,r11,#0x24
  00133906: mov.w r1,#0x4b0
  0013390a: blx r8
  0013390c: ldr.w r0,[r10,#0x0]
  00133910: add.w r2,r11,#0x28
  00133914: movw r1,#0x4b6
  00133918: blx r8
  0013391a: ldr r0,[0x00133b64]
  0013391c: ldr r1,[0x00133b68]
  0013391e: add r0,pc
  00133920: ldr r2,[0x00133b6c]
  00133922: add r1,pc
  00133924: ldr r0,[r0,#0x0]
  00133926: add r2,pc
  00133928: ldr r1,[r1,#0x0]
  0013392a: str r0,[sp,#0x4c]
  0013392c: ldr r0,[r0,#0x54]
  0013392e: str r0,[sp,#0x44]
  00133930: ldr r0,[r1,#0x0]
  00133932: str r1,[sp,#0x48]
  00133934: str r0,[sp,#0x40]
  00133936: ldr.w r9,[r2,#0x0]
  0013393a: ldr.w r1,[r11,#0x1c]
  0013393e: ldr.w r0,[r10,#0x0]
  00133942: blx r9
  00133944: str r0,[sp,#0x38]
  00133946: ldr.w r0,[r10,#0x0]
  0013394a: ldr.w r1,[r11,#0x24]
  0013394e: blx r9
  00133950: ldr r1,[sp,#0x6c]
  00133952: mov r11,r0
  00133954: ldr.w r0,[r10,#0x0]
  00133958: ldr r1,[r1,#0x20]
  0013395a: blx r9
  0013395c: ldr r2,[sp,#0x6c]
  0013395e: ldr r1,[sp,#0x6c]
  00133960: ldr r3,[sp,#0x6c]
  00133962: adds r2,#0x48
  00133964: adds r1,#0x4a
  00133966: adds r3,#0x32
  00133968: strd r3,r2,[sp,#0x20]
  0013396c: str r1,[sp,#0x28]
  0013396e: ldr r1,[sp,#0x6c]
  00133970: add.w r12,r1,#0x34
  00133974: ldr r1,[sp,#0x6c]
  00133976: add.w r2,r1,#0x3e
  0013397a: ldr r1,[sp,#0x6c]
  0013397c: add.w r3,r1,#0x3c
  00133980: ldr r1,[sp,#0x6c]
  00133982: add.w r5,r1,#0x42
  00133986: ldr r1,[sp,#0x6c]
  00133988: add.w r6,r1,#0x40
  0013398c: ldr r1,[sp,#0x6c]
  0013398e: add.w r4,r1,#0x30
  00133992: ldr r1,[sp,#0x6c]
  00133994: adds r1,#0x2e
  00133996: stm sp,{r0,r1,r4,r6}
  0013399a: strd r5,r3,[sp,#0x10]
  0013399e: mov r3,r11
  001339a0: strd r2,r12,[sp,#0x18]
  001339a4: ldrd r0,r1,[sp,#0x40]
  001339a8: ldr r2,[sp,#0x38]
  001339aa: blx 0x000764bc
  001339ae: ldr r1,[sp,#0x6c]
  001339b0: ldr.w r0,[r10,#0x0]
  001339b4: add.w r2,r1,#0x78
  001339b8: movw r1,#0x4c6
  001339bc: blx r8
  001339be: ldr r1,[sp,#0x6c]
  001339c0: ldr.w r0,[r10,#0x0]
  001339c4: add.w r2,r1,#0x7c
  001339c8: movw r1,#0x6aa
  001339cc: blx r8
  001339ce: ldr r1,[sp,#0x6c]
  001339d0: ldr.w r0,[r10,#0x0]
  001339d4: add.w r2,r1,#0x84
  001339d8: movw r1,#0x4ba
  001339dc: blx r8
  001339de: ldr r1,[sp,#0x6c]
  001339e0: ldr.w r0,[r10,#0x0]
  001339e4: add.w r2,r1,#0x80
  001339e8: movw r1,#0x4b4
  001339ec: blx r8
  001339ee: ldr r1,[sp,#0x6c]
  001339f0: ldr.w r0,[r10,#0x0]
  001339f4: add.w r2,r1,#0x88
  001339f8: movw r1,#0x4be
  001339fc: blx r8
  001339fe: ldr r1,[sp,#0x6c]
  00133a00: ldr.w r0,[r10,#0x0]
  00133a04: add.w r2,r1,#0x8c
  00133a08: movw r1,#0x4bc
  00133a0c: blx r8
  00133a0e: ldr r0,[sp,#0x4c]
  00133a10: ldr r0,[r0,#0x58]
  00133a12: str r0,[sp,#0x4c]
  00133a14: ldr r0,[sp,#0x48]
  00133a16: ldr.w r11,[r0,#0x0]
  00133a1a: ldr r0,[sp,#0x6c]
  00133a1c: ldr r1,[r0,#0x78]
  00133a1e: ldr.w r0,[r10,#0x0]
  00133a22: blx r9
  00133a24: mov r12,r0
  00133a26: ldr r0,[sp,#0x6c]
  00133a28: add.w r9,sp,#0x20
  00133a2c: ldr r3,[r0,#0x78]
  00133a2e: ldr r0,[sp,#0x6c]
  00133a30: ldr.w lr,[r0,#0x7c]
  00133a34: ldr r0,[sp,#0x6c]
  00133a36: add.w r1,r0,#0x4e
  00133a3a: ldr r0,[sp,#0x6c]
  00133a3c: add.w r4,r0,#0x4c
  00133a40: ldr r0,[sp,#0x6c]
  00133a42: add.w r5,r0,#0x72
  00133a46: ldr r0,[sp,#0x6c]
  00133a48: add.w r6,r0,#0x70
  00133a4c: ldr r0,[sp,#0x6c]
  00133a4e: add.w r2,r0,#0x6e
  00133a52: ldr r0,[sp,#0x6c]
  00133a54: adds r0,#0x6c
  00133a56: stm.w r9!,{r0,r2,r6}
  00133a5a: strd r5,r4,[sp,#0x2c]
  00133a5e: ldr r0,[sp,#0x6c]
  00133a60: str r1,[sp,#0x34]
  00133a62: add.w r9,r0,#0x6a
  00133a66: ldr r0,[sp,#0x6c]
  00133a68: add.w r1,r0,#0x68
  00133a6c: ldr r0,[sp,#0x6c]
  00133a6e: add.w r2,r0,#0x66
  00133a72: ldr r0,[sp,#0x6c]
  00133a74: add.w r4,r0,#0x64
  00133a78: ldr r0,[sp,#0x6c]
  00133a7a: add.w r5,r0,#0x62
  00133a7e: ldr r0,[sp,#0x6c]
  00133a80: add.w r6,r0,#0x60
  00133a84: ldr r0,[sp,#0x6c]
  00133a86: adds r0,#0x74
  00133a88: strd lr,r0,[sp,#0x0]
  00133a8c: mov r0,r11
  00133a8e: strd r6,r5,[sp,#0x8]
  00133a92: strd r4,r2,[sp,#0x10]
  00133a96: mov r2,r12
  00133a98: strd r1,r9,[sp,#0x18]
  00133a9c: ldr r1,[sp,#0x4c]
  00133a9e: ldr.w r11,[sp,#0x6c]
  00133aa2: ldr.w r9,[sp,#0x5c]
  00133aa6: blx 0x000764c8
  00133aaa: ldr.w r0,[r10,#0x0]
  00133aae: add.w r2,r11,#0x50
  00133ab2: movw r1,#0x6a5
  00133ab6: blx r8
  00133ab8: ldr.w r0,[r10,#0x0]
  00133abc: add.w r2,r11,#0x54
  00133ac0: movw r1,#0x6a6
  00133ac4: blx r8
  00133ac6: ldr.w r0,[r10,#0x0]
  00133aca: add.w r2,r11,#0x58
  00133ace: movw r1,#0x6a7
  00133ad2: blx r8
  00133ad4: ldr.w r0,[r10,#0x0]
  00133ad8: add.w r2,r11,#0x5c
  00133adc: mov.w r1,#0x6a8
  00133ae0: blx r8
  00133ae2: movs r0,#0xc8
  00133ae4: blx 0x0006eb24
  00133ae8: mov r5,r0
  00133aea: ldr r0,[sp,#0x60]
  00133aec: ldr r0,[r0,#0x0]
  00133aee: movw r1,#0x1f1
  00133af2: blx 0x00072f70
  00133af6: mov r12,r0
  00133af8: ldr r0,[0x00133b70]
  00133afa: ldr r1,[sp,#0x64]
  00133afc: add r0,pc
  00133afe: ldr r2,[r1,#0x0]
  00133b00: ldr.w r8,[r0,#0x0]
  00133b04: ldrd r0,r3,[r2,#0x30]
  00133b08: ldr.w r6,[r2,#0x300]
  00133b0c: ldr.w r2,[r2,#0x308]
  00133b10: ldr.w r4,[r9,#0x0]
  00133b14: add r2,r6
  00133b16: ldrb.w r1,[r8,#0x0]
  00133b1a: sub.w r2,r2,r3, lsl #0x1
  00133b1e: sub.w r0,r2,r0, lsl #0x1
  00133b22: asrs r3,r4,#0x1
  00133b24: cbz r1,0x00133b78
  00133b26: vldr.32 s0,[pc,#0x4c]
  00133b2a: b 0x00133b8c
  00133b2c: ldr r2,[r4,#0x0]
  00133b2e: movs r0,#0x0
  00133b30: strd r0,r2,[r11,#0x19c]
  00133b34: ldr r0,[sp,#0x64]
  00133b36: ldr r1,[sp,#0x68]
  00133b38: ldr r0,[r0,#0x0]
  00133b3a: ldr r1,[r1,#0x0]
  00133b3c: ldrd r3,r6,[r0,#0xc]
  00133b40: ldrd r5,r4,[r0,#0x20]
  00133b44: subs r1,r1,r3
  00133b46: subs r1,r1,r6
  00133b48: subs r1,r1,r5
  00133b4a: subs r1,r1,r4
  00133b4c: ldr r4,[sp,#0x60]
  00133b4e: add.w r1,r1,r1, lsr #0x1f
  00133b52: asrs r3,r1,#0x1
  00133b54: str.w r3,[r11,#0x154]
  00133b58: b 0x00133d06
  00133b78: ldr r1,[0x00133ca8]
  00133b7a: adr r2,[0x133cac]
  00133b7c: add r1,pc
  00133b7e: ldr r1,[r1,#0x0]
  00133b80: ldrb r1,[r1,#0x0]
  00133b82: cmp r1,#0x0
  00133b84: it eq
  00133b86: add.eq r2,#0x4
  00133b88: vldr.32 s0,[r2]
  00133b8c: vcvt.s32.f32 s0,s0
  00133b90: movs r1,#0x4
  00133b92: movs r2,#0x44
  00133b94: strd r2,r1,[sp,#0x8]
  00133b98: mov r1,r12
  00133b9a: str r0,[sp,#0x0]
  00133b9c: mov r0,r5
  00133b9e: movs r2,#0x0
  00133ba0: vstr.32 s0,[sp,#0x4]
  00133ba4: blx 0x00074f50
  00133ba8: ldr r4,[sp,#0x60]
  00133baa: movs r0,#0xc8
  00133bac: str.w r5,[r11,#0x18]
  00133bb0: blx 0x0006eb24
  00133bb4: mov r5,r0
  00133bb6: ldr r0,[r4,#0x0]
  00133bb8: mov.w r1,#0x1f0
  00133bbc: blx 0x00072f70
  00133bc0: mov r12,r0
  00133bc2: ldr r0,[sp,#0x64]
  00133bc4: ldr.w r6,[r9,#0x0]
  00133bc8: ldr r0,[r0,#0x0]
  00133bca: ldr.w r2,[r0,#0x300]
  00133bce: ldr.w r3,[r0,#0x308]
  00133bd2: ldrd r4,r0,[r0,#0x30]
  00133bd6: add r2,r3
  00133bd8: ldrb.w r1,[r8,#0x0]
  00133bdc: subs r0,r2,r0
  00133bde: asrs r3,r6,#0x1
  00133be0: subs r0,r0,r4
  00133be2: cbz r1,0x00133bea
  00133be4: vldr.32 s0,[pc,#-0x74]
  00133be8: b 0x00133bfe
  00133bea: ldr r1,[0x00133cb4]
  00133bec: adr r2,[0x133cac]
  00133bee: add r1,pc
  00133bf0: ldr r1,[r1,#0x0]
  00133bf2: ldrb r1,[r1,#0x0]
  00133bf4: cmp r1,#0x0
  00133bf6: it eq
  00133bf8: add.eq r2,#0x4
  00133bfa: vldr.32 s0,[r2]
  00133bfe: vcvt.s32.f32 s0,s0
  00133c02: movs r1,#0x4
  00133c04: movs r2,#0x44
  00133c06: movs r6,#0x0
  00133c08: strd r2,r1,[sp,#0x8]
  00133c0c: mov r1,r12
  00133c0e: str r0,[sp,#0x0]
  00133c10: mov r0,r5
  00133c12: movs r2,#0x0
  00133c14: vstr.32 s0,[sp,#0x4]
  00133c18: blx 0x00074f50
  00133c1c: movs r0,#0x24
  00133c1e: str.w r5,[r11,#0x14]
  00133c22: blx 0x0006eb24
  00133c26: mov r5,r0
  00133c28: ldr r0,[sp,#0x64]
  00133c2a: ldr.w r1,[r9,#0x0]
  00133c2e: ldr r0,[r0,#0x0]
  00133c30: add.w r4,r0,#0x300
  00133c34: ldmia r4,{r2,r3,r4}
  00133c36: ldr r0,[r0,#0x8]
  00133c38: add r2,r0
  00133c3a: add.w r0,r3,r3, lsr #0x1f
  00133c3e: asrs r1,r1,#0x1
  00133c40: strd r4,r6,[sp,#0x0]
  00133c44: sub.w r1,r1,r0, asr #0x1
  00133c48: mov r0,r5
  00133c4a: blx 0x00075b20
  00133c4e: ldr r1,[0x00133cb8]
  00133c50: add r0,sp,#0x114
  00133c52: movs r2,#0x0
  00133c54: str.w r5,[r11,#0x10]
  00133c58: add r1,pc
  00133c5a: blx 0x0006ee18
  00133c5e: ldr r4,[sp,#0x60]
  00133c60: ldr r0,[r4,#0x0]
  00133c62: movw r1,#0x1ef
  00133c66: blx 0x00072f70
  00133c6a: mov r1,r0
  00133c6c: add r0,sp,#0x108
  00133c6e: movs r2,#0x0
  00133c70: blx 0x0006f028
  00133c74: add r1,sp,#0x114
  00133c76: add r2,sp,#0x108
  00133c78: mov r0,r5
  00133c7a: blx 0x00075b2c
  00133c7e: add r0,sp,#0x108
  00133c80: blx 0x0006ee30
  00133c84: add r0,sp,#0x114
  00133c86: blx 0x0006ee30
  00133c8a: movs r0,#0x0
  00133c8c: strd r0,r0,[r11,#0x8]
  00133c90: ldrb.w r0,[r8,#0x0]
  00133c94: cbz r0,0x00133cc0
  00133c96: ldr.w r0,[r9,#0x0]
  00133c9a: movw r2,#0x2bf
  00133c9e: vldr.32 s0,[pc,#0x1c]
  00133ca2: subw r0,r0,#0x2bf
  00133ca6: b 0x00133ce4
  00133cc0: ldr r0,[0x00134028]
  00133cc2: adr r1,[0x13402c]
  00133cc4: ldr.w r3,[r9,#0x0]
  00133cc8: mov.w r2,#0x3e8
  00133ccc: add r0,pc
  00133cce: ldr r0,[r0,#0x0]
  00133cd0: ldrb r0,[r0,#0x0]
  00133cd2: cmp r0,#0x0
  00133cd4: it eq
  00133cd6: add.eq r1,#0x4
  00133cd8: it eq
  00133cda: mov.eq.w r2,#0x1f4
  00133cde: vldr.32 s0,[r1]
  00133ce2: subs r0,r3,r2
  00133ce4: vcvt.s32.f32 s2,s0
  00133ce8: str.w r2,[r11,#0x1a0]
  00133cec: vcvt.s32.f32 s0,s0
  00133cf0: add.w r0,r0,r0, lsr #0x1f
  00133cf4: asrs r0,r0,#0x1
  00133cf6: str.w r0,[r11,#0x19c]
  00133cfa: ldr r0,[sp,#0x64]
  00133cfc: vstr.32 s2,[r11,#0x154]
  00133d00: ldr r0,[r0,#0x0]
  00133d02: vmov r3,s0
  00133d06: ldr r1,[r0,#0x28]
  00133d08: add.w r3,r3,r3, lsr #0x1f
  00133d0c: sub.w r2,r2,r1, lsl #0x1
  00133d10: str.w r2,[r11,#0x150]
  00133d14: ldr r0,[r0,#0x2c]
  00133d16: add.w r2,r2,r2, lsr #0x1f
  00133d1a: ldr.w r1,[r11,#0x114]
  00133d1e: rsb.w r0,r0,#0x1
  00133d22: add.w r3,r0,r3, asr #0x1
  00133d26: add.w r0,r0,r2, asr #0x1
  00133d2a: strd r0,r3,[r11,#0x158]
  00133d2e: ldr.w r0,[r10,#0x0]
  00133d32: blx 0x00072d90
  00133d36: add.w r1,r0,#0xf
  00133d3a: strd r0,r1,[r11,#0x160]
  00133d3e: movs r0,#0xc8
  00133d40: blx 0x0006eb24
  00133d44: mov r8,r0
  00133d46: ldr r0,[r4,#0x0]
  00133d48: mov.w r1,#0x1ec
  00133d4c: blx 0x00072f70
  00133d50: mov r10,r0
  00133d52: ldr r0,[sp,#0x64]
  00133d54: add.w r6,r11,#0x154
  00133d58: ldr r0,[r0,#0x0]
  00133d5a: ldmia r6,{r2,r3,r6}
  00133d5c: ldrd lr,r9,[r11,#0x19c]
  00133d60: ldr r5,[r0,#0xc]
  00133d62: ldr r4,[r0,#0x20]
  00133d64: ldrd r1,r0,[r0,#0x28]
  00133d68: add r4,r5
  00133d6a: sub.w r12,r3,#0x14
  00133d6e: add r2,r4
  00133d70: mov.w r11,#0x44
  00133d74: add r0,r2
  00133d76: add.w r2,r6,r6, lsr #0x1f
  00133d7a: add.w r0,r0,r2, asr #0x1
  00133d7e: strd r0,r12,[sp,#0x0]
  00133d82: movs r0,#0x4
  00133d84: movs r2,#0x0
  00133d86: str r0,[sp,#0xc]
  00133d88: add.w r0,r9,lr
  00133d8c: subs r0,r0,r1
  00133d8e: add.w r1,r3,r3, lsr #0x1f
  00133d92: str.w r11,[sp,#0x8]
  00133d96: sub.w r3,r0,r1, asr #0x1
  00133d9a: mov r0,r8
  00133d9c: mov r1,r10
  00133d9e: ldr r6,[sp,#0x6c]
  00133da0: blx 0x00074f50
  00133da4: movs r0,#0xc8
  00133da6: str.w r8,[r6,#0xd0]
  00133daa: blx 0x0006eb24
  00133dae: ldr r1,[0x00134034]
  00133db0: mov r8,r0
  00133db2: add r1,pc
  00133db4: add r0,sp,#0x15c
  00133db6: movs r2,#0x0
  00133db8: blx 0x0006ee18
  00133dbc: ldr.w r10,[sp,#0x64]
  00133dc0: ldr.w r0,[r10,#0x0]
  00133dc4: ldrd r1,r12,[r6,#0x154]
  00133dc8: ldr.w r3,[r6,#0x15c]
  00133dcc: ldr.w r9,[r6,#0x19c]
  00133dd0: ldr r4,[r0,#0xc]
  00133dd2: ldr r5,[r0,#0x20]
  00133dd4: ldrd r2,r0,[r0,#0x28]
  00133dd8: add r4,r5
  00133dda: mov.w lr,#0x42
  00133dde: add r1,r4
  00133de0: add r0,r1
  00133de2: add.w r1,r3,r3, lsr #0x1f
  00133de6: add.w r0,r0,r1, asr #0x1
  00133dea: strd r0,lr,[sp,#0x0]
  00133dee: add.w r0,r9,r2
  00133df2: add r1,sp,#0x15c
  00133df4: add r0,r12
  00133df6: movs r2,#0xd
  00133df8: sub.w r3,r0,#0xa
  00133dfc: mov r0,r8
  00133dfe: blx 0x00074578
  00133e02: add r0,sp,#0x15c
  00133e04: str.w r8,[r6,#0xcc]
  00133e08: blx 0x0006ee30
  00133e0c: movs r0,#0xc8
  00133e0e: blx 0x0006eb24
  00133e12: str r0,[sp,#0x4c]
  00133e14: ldr r1,[0x00134038]
  00133e16: add r1,pc
  00133e18: add r0,sp,#0x15c
  00133e1a: movs r2,#0x0
  00133e1c: blx 0x0006ee18
  00133e20: ldr.w r0,[r10,#0x0]
  00133e24: ldr.w r10,[r6,#0x154]
  00133e28: ldr.w r11,[r6,#0x15c]
  00133e2c: ldr.w r3,[r6,#0x164]
  00133e30: ldrd r12,lr,[r6,#0x19c]
  00133e34: ldr r5,[r0,#0xc]
  00133e36: ldrd r8,r4,[r0,#0x1c]
  00133e3a: ldrd r9,r1,[r0,#0x28]
  00133e3e: ldr.w r0,[r0,#0x2fc]
  00133e42: movs r2,#0x42
  00133e44: str r2,[sp,#0x4]
  00133e46: add.w r2,r11,r1
  00133e4a: add r0,r2
  00133e4c: adds r2,r4,r5
  00133e4e: add.w r4,r8,r8, lsl #0x1
  00133e52: add.w r1,r1,r1, lsl #0x2
  00133e56: add r2,r4
  00133e58: add r0,r3
  00133e5a: add r2,r10
  00133e5c: ldr r4,[sp,#0x4c]
  00133e5e: add r1,r2
  00133e60: add.w r2,r3,r3, lsr #0x1f
  00133e64: add r1,r3
  00133e66: add.w r1,r1,r2, asr #0x1
  00133e6a: movs r2,#0xd
  00133e6c: add.w r0,r1,r0, lsl #0x1
  00133e70: str r0,[sp,#0x0]
  00133e72: add.w r0,r12,lr
  00133e76: add r1,sp,#0x15c
  00133e78: subs r0,#0x19
  00133e7a: sub.w r3,r0,r9
  00133e7e: mov r0,r4
  00133e80: blx 0x00074578
  00133e84: add r0,sp,#0x15c
  00133e86: str.w r4,[r6,#0xe8]
  00133e8a: blx 0x0006ee30
  00133e8e: ldr r0,[0x0013403c]
  00133e90: add r0,pc
  00133e92: ldr r4,[r0,#0x0]
  00133e94: ldr.w r0,[r6,#0xd0]
  00133e98: str r4,[sp,#0x4c]
  00133e9a: ldrb r1,[r4,#0x11]
  00133e9c: blx 0x00074fa4
  00133ea0: ldrb r1,[r4,#0x10]
  00133ea2: ldr.w r0,[r6,#0xcc]
  00133ea6: blx 0x00075acc
  00133eaa: ldrb.w r1,[r4,#0x40]
  00133eae: ldr.w r0,[r6,#0xe8]
  00133eb2: blx 0x00075acc
  00133eb6: ldr r0,[sp,#0x50]
  00133eb8: mov r5,r6
  00133eba: ldrb r0,[r0,#0x0]
  00133ebc: cbz r0,0x00133f2a
  00133ebe: movs r0,#0xc8
  00133ec0: blx 0x0006eb24
  00133ec4: mov r8,r0
  00133ec6: ldr r0,[sp,#0x60]
  00133ec8: ldr r0,[r0,#0x0]
  00133eca: mov.w r1,#0x1ee
  00133ece: blx 0x00072f70
  00133ed2: mov r10,r0
  00133ed4: ldr r0,[sp,#0x64]
  00133ed6: add.w r6,r5,#0x154
  00133eda: ldr r0,[r0,#0x0]
  00133edc: ldmia r6,{r2,r3,r6}
  00133ede: ldrd lr,r9,[r5,#0x19c]
  00133ee2: ldr r5,[r0,#0xc]
  00133ee4: ldr r4,[r0,#0x20]
  00133ee6: ldrd r1,r0,[r0,#0x28]
  00133eea: add r4,r5
  00133eec: sub.w r12,r3,#0x14
  00133ef0: add r2,r4
  00133ef2: mov.w r11,#0x44
  00133ef6: add r0,r2
  00133ef8: add.w r2,r6,r6, lsr #0x1f
  00133efc: add.w r0,r0,r2, asr #0x1
  00133f00: strd r0,r12,[sp,#0x0]
  00133f04: movs r0,#0x4
  00133f06: movs r2,#0x0
  00133f08: str r0,[sp,#0xc]
  00133f0a: add.w r0,r9,lr
  00133f0e: subs r0,r0,r1
  00133f10: add.w r1,r3,r3, lsr #0x1f
  00133f14: str.w r11,[sp,#0x8]
  00133f18: sub.w r3,r0,r1, asr #0x1
  00133f1c: mov r0,r8
  00133f1e: mov r1,r10
  00133f20: ldr r5,[sp,#0x6c]
  00133f22: blx 0x00074f50
  00133f26: str.w r8,[r5,#0xe4]
  00133f2a: movs r0,#0xc
  00133f2c: blx 0x0006eb24
  00133f30: mov r4,r0
  00133f32: blx 0x00074f38
  00133f36: ldr r6,[sp,#0x60]
  00133f38: movw r1,#0x207
  00133f3c: str.w r4,[r5,#0xb4]
  00133f40: ldr r0,[r6,#0x0]
  00133f42: blx 0x00072f70
  00133f46: add r4,sp,#0xfc
  00133f48: mov r1,r0
  00133f4a: movs r2,#0x0
  00133f4c: mov.w r8,#0x0
  00133f50: mov r0,r4
  00133f52: blx 0x0006f028
  00133f56: ldr.w r0,[r5,#0xb4]
  00133f5a: strd r0,r8,[sp,#0x0]
  00133f5e: mov r0,r5
  00133f60: movs r1,#0x10
  00133f62: mov r2,r4
  00133f64: movs r3,#0x0
  00133f66: blx 0x00076480
  00133f6a: ldr.w r10,[sp,#0x5c]
  00133f6e: add r0,sp,#0xfc
  00133f70: blx 0x0006ee30
  00133f74: ldr r0,[r6,#0x0]
  00133f76: movs r1,#0x19
  00133f78: blx 0x00072f70
  00133f7c: add r4,sp,#0xf0
  00133f7e: mov r1,r0
  00133f80: movs r2,#0x0
  00133f82: mov r0,r4
  00133f84: blx 0x0006f028
  00133f88: ldr.w r0,[r5,#0xb4]
  00133f8c: strd r0,r8,[sp,#0x0]
  00133f90: mov r0,r5
  00133f92: movs r1,#0x32
  00133f94: mov r2,r4
  00133f96: movs r3,#0x1
  00133f98: blx 0x00076480
  00133f9c: add r0,sp,#0xf0
  00133f9e: blx 0x0006ee30
  00133fa2: movs r0,#0xc
  00133fa4: blx 0x0006eb24
  00133fa8: mov r4,r0
  00133faa: blx 0x00074f38
  00133fae: mov.w r0,#0xffffffff
  00133fb2: str.w r4,[r5,#0xb8]
  00133fb6: strd r0,r0,[sp,#0xe8]
  00133fba: mov.w r9,#0x1
  00133fbe: ldr r0,[0x00134040]
  00133fc0: mov.w r8,#0x0
  00133fc4: add r0,pc
  00133fc6: mov r11,r0
  00133fc8: b 0x0013409a
  00133fca: ldr r5,[sp,#0x58]
  00133fcc: add r2,sp,#0xec
  00133fce: ldrh.w r1,[r11,r8,lsl #0x3]
  00133fd2: mov r4,r11
  00133fd4: ldr r0,[r5,#0x0]
  00133fd6: blx 0x00071cf8
  00133fda: add.w r0,r11,r8, lsl #0x3
  00133fde: add r2,sp,#0xe8
  00133fe0: ldrh r1,[r0,#0x4]
  00133fe2: ldr r0,[r5,#0x0]
  00133fe4: blx 0x00071cf8
  00133fe8: ldr r0,[r5,#0x0]
  00133fea: ldr r1,[sp,#0xec]
  00133fec: blx 0x00072d84
  00133ff0: mov r4,r0
  00133ff2: ldr r0,[r5,#0x0]
  00133ff4: ldr r1,[sp,#0xec]
  00133ff6: blx 0x00072d90
  00133ffa: mov r5,r0
  00133ffc: cmp.w r8,#0x0
  00134000: beq 0x00134044
  00134002: ldr.w r0,[r10,#0x0]
  00134006: add.w r1,r4,r4, lsr #0x1f
  0013400a: cmp.w r8,#0x1
  0013400e: mov.w r3,#0x0
  00134012: asr.w r2,r1, asr #0x1
  00134016: add.w r0,r0,r0, lsr #0x1f
  0013401a: it eq
  0013401c: sub.eq.w r2,r3,r1, asr #0x1
  00134020: add.w r6,r2,r0, asr #0x1
  00134024: b 0x0013405c
  00134044: ldr.w r0,[r10,#0x0]
  00134048: add.w r1,r4,r4, lsr #0x1f
  0013404c: movs r2,#0x0
  0013404e: sub.w r1,r2,r1, asr #0x1
  00134052: add.w r0,r0,r0, lsr #0x1f
  00134056: subs r1,r1,r4
  00134058: add.w r6,r1,r0, asr #0x1
  0013405c: movs r0,#0xc8
  0013405e: blx 0x0006eb24
  00134062: mov r4,r0
  00134064: ldrd r2,r1,[sp,#0xe8]
  00134068: ldr r0,[sp,#0x68]
  0013406a: ldr r0,[r0,#0x0]
  0013406c: add.w r0,r0,r0, lsr #0x1f
  00134070: add.w r3,r5,r5, lsr #0x1f
  00134074: asrs r0,r0,#0x1
  00134076: sub.w r0,r0,r3, asr #0x1
  0013407a: movs r3,#0x13
  0013407c: strd r6,r0,[sp,#0x0]
  00134080: mov r0,r4
  00134082: str.w r9,[sp,#0x8]
  00134086: blx 0x000764d4
  0013408a: ldr r0,[sp,#0x6c]
  0013408c: ldr.w r1,[r0,#0xb8]
  00134090: mov r0,r4
  00134092: blx 0x0007648c
  00134096: add.w r8,r8,#0x1
  0013409a: cmp.w r8,#0x3
  0013409e: bcc 0x00133fca
  001340a0: ldr.w r11,[sp,#0x64]
  001340a4: ldr.w r0,[r11,#0x0]
  001340a8: ldrd r8,r0,[r0,#0x2b4]
  001340ac: str r0,[sp,#0x48]
  001340ae: movs r0,#0xc
  001340b0: blx 0x0006eb24
  001340b4: mov r4,r0
  001340b6: blx 0x000764e0
  001340ba: ldr.w r10,[sp,#0x6c]
  001340be: movs r0,#0x3c
  001340c0: str.w r4,[r10,#0xec]
  001340c4: blx 0x0006eb24
  001340c8: ldrd lr,r12,[r10,#0x154]
  001340cc: mov r5,r0
  001340ce: ldr.w r9,[sp,#0x4c]
  001340d2: ldr r0,[sp,#0x5c]
  001340d4: ldr.w r3,[r11,#0x0]
  001340d8: add.w r1,r9,#0x14
  001340dc: ldrb.w r6,[r9,#0x11]
  001340e0: ldr.w r2,[r10,#0x15c]
  001340e4: ldr r4,[r0,#0x0]
  001340e6: cmp r6,#0x0
  001340e8: it eq
  001340ea: add.eq.w r1,r9,#0x18
  001340ee: ldr r6,[r3,#0x20]
  001340f0: vldr.32 s0,[r1]
  001340f4: ldr r1,[r3,#0xc]
  001340f6: ldrd r0,r3,[r3,#0x28]
  001340fa: sub.w r1,r1,r8
  001340fe: add r2,r3
  00134100: add r1,r6
  00134102: vstr.32 s0,[sp]
  00134106: add r1,lr
  00134108: add.w r3,r1,r2, lsl #0x1
  0013410c: add.w r1,r4,#0xa
  00134110: subs r0,r1,r0
  00134112: movs r1,#0x0
  00134114: sub.w r2,r0,r12
  00134118: mov r0,r5
  0013411a: blx 0x000764ec
  0013411e: ldr.w r1,[r10,#0xec]
  00134122: mov r0,r5
  00134124: blx 0x000764f8
  00134128: movs r0,#0x3c
  0013412a: blx 0x0006eb24
  0013412e: mov r5,r0
  00134130: ldr.w r0,[r11,#0x0]
  00134134: ldr.w r1,[r10,#0x1c0]
  00134138: vldr.32 s0,[r9]
  0013413c: ldr r3,[r0,#0xc]
  0013413e: ldrd r6,r4,[r0,#0x1c]
  00134142: ldrd r2,r0,[r0,#0x28]
  00134146: ldr.w r8,[sp,#0x48]
  0013414a: add r2,r1
  0013414c: vstr.32 s0,[sp]
  00134150: add.w r1,r3,r8
  00134154: add r1,r4
  00134156: add r1,r6
  00134158: adds r3,r1,r0
  0013415a: mov r0,r5
  0013415c: movs r1,#0x1
  0013415e: blx 0x000764ec
  00134162: ldr.w r1,[r10,#0xec]
  00134166: mov r0,r5
  00134168: blx 0x000764f8
  0013416c: movs r0,#0x3c
  0013416e: blx 0x0006eb24
  00134172: mov r5,r0
  00134174: ldr.w r0,[r11,#0x0]
  00134178: ldr.w r12,[r10,#0x160]
  0013417c: ldr.w r2,[r10,#0x1c0]
  00134180: vldr.32 s0,[r9,#0x4]
  00134184: ldrd r3,r6,[r0,#0x28]
  00134188: ldr r4,[r0,#0xc]
  0013418a: ldrd r1,r0,[r0,#0x1c]
  0013418e: add r2,r3
  00134190: add.w r3,r4,r8
  00134194: add r0,r3
  00134196: vstr.32 s0,[sp]
  0013419a: add r0,r1
  0013419c: add.w r1,r6,r6, lsl #0x1
  001341a0: add r0,r1
  001341a2: movs r1,#0x1
  001341a4: add.w r3,r0,r12
  001341a8: mov r0,r5
  001341aa: blx 0x000764ec
  001341ae: ldr.w r1,[r10,#0xec]
  001341b2: mov r0,r5
  001341b4: blx 0x000764f8
  001341b8: movs r0,#0x3c
  001341ba: blx 0x0006eb24
  001341be: mov r5,r0
  001341c0: ldr.w r0,[r11,#0x0]
  001341c4: ldr.w r12,[r10,#0x160]
  001341c8: ldr.w r2,[r10,#0x1c0]
  001341cc: vldr.32 s0,[r9,#0x8]
  001341d0: ldrd r3,r6,[r0,#0x28]
  001341d4: ldr r4,[r0,#0xc]
  001341d6: ldrd r1,r0,[r0,#0x1c]
  001341da: add r2,r3
  001341dc: add.w r3,r4,r8
  001341e0: add r0,r3
  001341e2: vstr.32 s0,[sp]
  001341e6: add r0,r1
  001341e8: add.w r1,r6,r6, lsl #0x2
  001341ec: add r0,r1
  001341ee: movs r1,#0x1
  001341f0: add.w r3,r0,r12, lsl #0x1
  001341f4: mov r0,r5
  001341f6: blx 0x000764ec
  001341fa: ldr.w r1,[r10,#0xec]
  001341fe: mov r0,r5
  00134200: blx 0x000764f8
  00134204: movs r0,#0x3c
  00134206: blx 0x0006eb24
  0013420a: mov r5,r0
  0013420c: ldr.w r0,[r11,#0x0]
  00134210: vldr.32 s0,[r10,#0x160]
  00134214: vmov.f64 d8,0x4004000000000000
  00134218: ldr r1,[r0,#0xc]
  0013421a: ldrd r2,r3,[r0,#0x1c]
  0013421e: ldrd r6,r0,[r0,#0x28]
  00134222: add r1,r3
  00134224: add.w r0,r0,r0, lsl #0x1
  00134228: vcvt.f64.s32 d16,s0
  0013422c: add.w r2,r2,r2, lsl #0x1
  00134230: add r1,r2
  00134232: add.w r0,r1,r0, lsl #0x1
  00134236: vmov s0,r0
  0013423a: vcvt.f64.s32 d17,s0
  0013423e: vmla.f64 d17,d16,d8
  00134242: vmov s0,r8
  00134246: vcvt.f64.s32 d9,s0
  0013424a: vadd.f64 d16,d17,d9
  0013424e: vcvt.s32.f64 s0,d16
  00134252: ldr.w r0,[r10,#0x1c0]
  00134256: vldr.32 s2,[r9,#0x24]
  0013425a: vmov r3,s0
  0013425e: adds r2,r0,r6
  00134260: mov r0,r5
  00134262: movs r1,#0x2
  00134264: vstr.32 s2,[sp]
  00134268: blx 0x000764ec
  0013426c: ldr.w r1,[r10,#0xec]
  00134270: mov r0,r5
  00134272: blx 0x000764f8
  00134276: movs r0,#0x3c
  00134278: blx 0x0006eb24
  0013427c: mov r5,r0
  0013427e: ldr.w r0,[r11,#0x0]
  00134282: vldr.32 s0,[r10,#0x160]
  00134286: vldr.32 s2,[r10,#0x164]
  0013428a: ldr r1,[r0,#0xc]
  0013428c: ldrd r2,r3,[r0,#0x1c]
  00134290: ldr r6,[r0,#0x28]
  00134292: vcvt.f64.s32 d16,s0
  00134296: ldr r0,[r0,#0x2c]
  00134298: add r1,r3
  0013429a: add.w r2,r2,r2, lsl #0x1
  0013429e: add r1,r2
  001342a0: add.w r0,r1,r0, lsl #0x3
  001342a4: vmov s0,r0
  001342a8: vcvt.f64.s32 d17,s0
  001342ac: vmla.f64 d17,d16,d8
  001342b0: vcvt.f64.s32 d16,s2
  001342b4: vadd.f64 d16,d17,d16
  001342b8: vadd.f64 d16,d16,d9
  001342bc: vcvt.s32.f64 s0,d16
  001342c0: ldr.w r0,[r10,#0x1c0]
  001342c4: vldr.32 s2,[r9,#0x28]
  001342c8: vmov r3,s0
  001342cc: adds r2,r0,r6
  001342ce: mov r0,r5
  001342d0: movs r1,#0x2
  001342d2: vstr.32 s2,[sp]
  001342d6: blx 0x000764ec
  001342da: ldr.w r1,[r10,#0xec]
  001342de: mov r0,r5
  001342e0: blx 0x000764f8
  001342e4: movs r0,#0x3c
  001342e6: blx 0x0006eb24
  001342ea: mov r5,r0
  001342ec: ldr.w r0,[r11,#0x0]
  001342f0: ldr.w r1,[r10,#0x1c0]
  001342f4: vldr.32 s0,[r9,#0x2c]
  001342f8: ldr r3,[r0,#0xc]
  001342fa: ldrd r6,r4,[r0,#0x1c]
  001342fe: ldrd r2,r0,[r0,#0x28]
  00134302: add r2,r1
  00134304: add.w r1,r3,r8
  00134308: add r1,r4
  0013430a: vstr.32 s0,[sp]
  0013430e: add r1,r6
  00134310: adds r3,r1,r0
  00134312: mov r0,r5
  00134314: movs r1,#0x2
  00134316: blx 0x000764ec
  0013431a: ldr.w r1,[r10,#0xec]
  0013431e: mov r0,r5
  00134320: blx 0x000764f8
  00134324: ldr.w r0,[r10,#0xec]
  00134328: ldr.w r9,[sp,#0x5c]
  0013432c: ldr r0,[r0,#0x4]
  0013432e: ldr.w r5,[r9,#0x0]
  00134332: ldr r4,[r0,#0x18]
  00134334: mov r0,r4
  00134336: blx 0x00076504
  0013433a: ldr.w r1,[r11,#0x0]
  0013433e: add.w r2,r5,r5, lsr #0x1f
  00134342: add.w r0,r0,r0, lsr #0x1f
  00134346: ldr r3,[r1,#0xc]
  00134348: ldrd r12,r5,[r1,#0x1c]
  0013434c: ldr r6,[r1,#0x2c]
  0013434e: asrs r1,r2,#0x1
  00134350: sub.w r1,r1,r0, asr #0x1
  00134354: add.w r0,r3,r8
  00134358: add r0,r5
  0013435a: add r0,r12
  0013435c: adds r2,r0,r6
  0013435e: mov r0,r4
  00134360: blx 0x00072db4
  00134364: movs r0,#0xc8
  00134366: blx 0x0006eb24
  0013436a: ldr r1,[0x00134574]
  0013436c: mov r5,r0
  0013436e: add r1,pc
  00134370: add r0,sp,#0x15c
  00134372: movs r2,#0x0
  00134374: blx 0x0006ee18
  00134378: ldr.w r0,[r11,#0x0]
  0013437c: ldr.w r1,[r9,#0x0]
  00134380: ldr r2,[r0,#0xc]
  00134382: ldrd r3,r6,[r0,#0x1c]
  00134386: ldrd r4,r0,[r0,#0x28]
  0013438a: add r2,r6
  0013438c: mov.w r12,#0x12
  00134390: add r2,r3
  00134392: subs r3,r1,r4
  00134394: add r0,r2
  00134396: add r1,sp,#0x15c
  00134398: strd r0,r12,[sp,#0x0]
  0013439c: mov r0,r5
  0013439e: movs r2,#0xd
  001343a0: blx 0x00074578
  001343a4: add r0,sp,#0x15c
  001343a6: str.w r5,[r10,#0xd4]
  001343aa: blx 0x0006ee30
  001343ae: ldr.w r9,[sp,#0x4c]
  001343b2: ldr.w r0,[r10,#0xd4]
  001343b6: ldrb.w r1,[r9,#0xd]
  001343ba: blx 0x00075acc
  001343be: ldr.w r0,[r10,#0xec]
  001343c2: ldrb.w r1,[r9,#0xd]
  001343c6: ldr r0,[r0,#0x4]
  001343c8: clz r1,r1
  001343cc: lsrs r1,r1,#0x5
  001343ce: ldr r0,[r0,#0x4]
  001343d0: blx 0x00076510
  001343d4: movs r0,#0xc8
  001343d6: blx 0x0006eb24
  001343da: ldr r1,[0x00134578]
  001343dc: mov r8,r0
  001343de: add r1,pc
  001343e0: add r0,sp,#0x15c
  001343e2: movs r2,#0x0
  001343e4: blx 0x0006ee18
  001343e8: ldr r2,[sp,#0x5c]
  001343ea: ldr.w r0,[r11,#0x0]
  001343ee: ldr.w r1,[r10,#0x160]
  001343f2: ldr r2,[r2,#0x0]
  001343f4: ldr r3,[r0,#0xc]
  001343f6: ldrd r6,r4,[r0,#0x1c]
  001343fa: ldrd r5,r0,[r0,#0x28]
  001343fe: add r3,r4
  00134400: add.w r0,r0,r0, lsl #0x1
  00134404: add r1,r3
  00134406: subs r3,r2,r5
  00134408: add r1,r6
  0013440a: mov.w r12,#0x12
  0013440e: add r0,r1
  00134410: add r1,sp,#0x15c
  00134412: strd r0,r12,[sp,#0x0]
  00134416: mov r0,r8
  00134418: movs r2,#0xd
  0013441a: blx 0x00074578
  0013441e: add r0,sp,#0x15c
  00134420: str.w r8,[r10,#0xd8]
  00134424: blx 0x0006ee30
  00134428: ldr.w r0,[r10,#0xd8]
  0013442c: ldrb.w r1,[r9,#0xc]
  00134430: blx 0x00075acc
  00134434: ldr.w r0,[r10,#0xec]
  00134438: ldrb.w r1,[r9,#0xe]
  0013443c: ldr r0,[r0,#0x4]
  0013443e: clz r1,r1
  00134442: lsrs r1,r1,#0x5
  00134444: ldr r0,[r0,#0x8]
  00134446: blx 0x00076510
  0013444a: movs r0,#0xc8
  0013444c: blx 0x0006eb24
  00134450: ldr r1,[0x0013457c]
  00134452: mov r8,r0
  00134454: add r1,pc
  00134456: add r0,sp,#0x15c
  00134458: movs r2,#0x0
  0013445a: blx 0x0006ee18
  0013445e: ldr r2,[sp,#0x5c]
  00134460: ldr.w r0,[r11,#0x0]
  00134464: ldr.w r1,[r10,#0x160]
  00134468: ldr r2,[r2,#0x0]
  0013446a: ldr r3,[r0,#0xc]
  0013446c: ldrd r6,r4,[r0,#0x1c]
  00134470: ldrd r5,r0,[r0,#0x28]
  00134474: add r3,r4
  00134476: add.w r0,r0,r0, lsl #0x2
  0013447a: add.w r1,r3,r1, lsl #0x1
  0013447e: subs r3,r2,r5
  00134480: add r1,r6
  00134482: mov.w r12,#0x12
  00134486: add r0,r1
  00134488: add r1,sp,#0x15c
  0013448a: strd r0,r12,[sp,#0x0]
  0013448e: mov r0,r8
  00134490: movs r2,#0xd
  00134492: blx 0x00074578
  00134496: add r0,sp,#0x15c
  00134498: str.w r8,[r10,#0xdc]
  0013449c: blx 0x0006ee30
  001344a0: ldr.w r0,[r10,#0xdc]
  001344a4: ldrb.w r1,[r9,#0xe]
  001344a8: blx 0x00075acc
  001344ac: ldr.w r0,[r10,#0xec]
  001344b0: ldrb.w r1,[r9,#0xe]
  001344b4: ldr r0,[r0,#0x4]
  001344b6: clz r1,r1
  001344ba: lsrs r1,r1,#0x5
  001344bc: ldr r0,[r0,#0xc]
  001344be: blx 0x00076510
  001344c2: ldr.w r0,[r10,#0xec]
  001344c6: movs r1,#0x1
  001344c8: ldr r0,[r0,#0x4]
  001344ca: ldr r0,[r0,#0x14]
  001344cc: blx 0x0007651c
  001344d0: ldr.w r0,[r10,#0xec]
  001344d4: movs r1,#0x1
  001344d6: ldr r0,[r0,#0x4]
  001344d8: ldr r0,[r0,#0x18]
  001344da: blx 0x0007651c
  001344de: ldr r0,[sp,#0x50]
  001344e0: ldrb r0,[r0,#0x0]
  001344e2: cbz r0,0x00134536
  001344e4: ldr.w r0,[r11,#0x0]
  001344e8: ldr r1,[0x00134580]
  001344ea: add r1,pc
  001344ec: ldr r2,[r0,#0xc]
  001344ee: ldrd r3,r6,[r0,#0x1c]
  001344f2: ldr r5,[r0,#0x28]
  001344f4: ldr r0,[r0,#0x2c]
  001344f6: ldr.w r8,[r1,#0x0]
  001344fa: adds r1,r6,r2
  001344fc: add r1,r3
  001344fe: ldr.w r3,[r10,#0x154]
  00134502: ldr.w r6,[r10,#0x15c]
  00134506: add.w r0,r0,r0, lsl #0x1
  0013450a: add r1,r3
  0013450c: ldr.w r2,[r10,#0xec]
  00134510: add r0,r1
  00134512: ldr.w r4,[r10,#0x19c]
  00134516: add.w r0,r0,r6, lsl #0x1
  0013451a: ldr r2,[r2,#0x4]
  0013451c: adds r1,r4,r5
  0013451e: vmov s0,r0
  00134522: adds r1,#0xa
  00134524: vcvt.f32.s32 s0,s0
  00134528: ldr r0,[r2,#0x0]
  0013452a: ldrb.w r2,[r8,#0x0]
  0013452e: cbz r2,0x00134588
  00134530: vldr.32 s2,[pc,#0x50]
  00134534: b 0x001345a0
  00134536: mov.w r9,#0x0
  0013453a: movs r0,#0x24
  0013453c: str.w r9,[r10,#0x198]
  00134540: blx 0x0006eb24
  00134544: ldr r1,[sp,#0x5c]
  00134546: mov r8,r0
  00134548: ldr.w r0,[r11,#0x0]
  0013454c: ldr r3,[r1,#0x0]
  0013454e: ldr r1,[sp,#0x68]
  00134550: ldr r6,[r1,#0x0]
  00134552: ldrd r2,r5,[r0,#0x20]
  00134556: ldr r1,[r0,#0x28]
  00134558: ldrd r4,r0,[r0,#0xc]
  0013455c: add r2,r4
  0013455e: sub.w r3,r3,r1, lsl #0x1
  00134562: subs r6,r6,r2
  00134564: subs r0,r6,r0
  00134566: subs r0,r0,r5
  00134568: strd r0,r9,[sp,#0x0]
  0013456c: mov r0,r8
  0013456e: blx 0x00075b20
  00134572: b 0x001349de
  00134588: ldr r2,[0x00134904]
  0013458a: vmov.f32 s4,0x41c80000
  0013458e: vldr.32 s2,[pc,#0x378]
  00134592: add r2,pc
  00134594: ldr r2,[r2,#0x0]
  00134596: ldrb r2,[r2,#0x0]
  00134598: cmp r2,#0x0
  0013459a: it eq
  0013459c: vmov.eq.f32 s2,s4
  001345a0: vsub.f32 s0,s0,s2
  001345a4: vcvt.s32.f32 s0,s0
  001345a8: vmov r2,s0
  001345ac: blx 0x00072db4
  001345b0: ldr.w r0,[r11,#0x0]
  001345b4: ldr.w r1,[r10,#0xec]
  001345b8: ldr.w r9,[r10,#0x154]
  001345bc: ldr.w r3,[r10,#0x15c]
  001345c0: ldr.w r12,[r10,#0x19c]
  001345c4: ldr r5,[r0,#0xc]
  001345c6: ldrd r4,r6,[r0,#0x1c]
  001345ca: ldrd lr,r2,[r0,#0x28]
  001345ce: add r3,r4
  001345d0: adds r4,r6,r5
  001345d2: add r4,r9
  001345d4: ldr.w r0,[r0,#0x2fc]
  001345d8: add.w r2,r4,r2, lsl #0x2
  001345dc: vldr.32 s16,[pc,#0x32c]
  001345e0: add r0,r2
  001345e2: ldr r1,[r1,#0x4]
  001345e4: add.w r0,r0,r3, lsl #0x1
  001345e8: vmov.f32 s2,s16
  001345ec: add.w r6,lr,r12
  001345f0: vmov s0,r0
  001345f4: vcvt.f32.s32 s0,s0
  001345f8: ldr.w r2,[r10,#0x1c0]
  001345fc: ldrb.w r3,[r8,#0x0]
  00134600: ldr r0,[r1,#0x4]
  00134602: adds r1,r6,r2
  00134604: cbnz r3,0x0013461e
  00134606: ldr r2,[0x00134910]
  00134608: vmov.f32 s4,0x41600000
  0013460c: vmov.f32 s2,0x41e00000
  00134610: add r2,pc
  00134612: ldr r2,[r2,#0x0]
  00134614: ldrb r2,[r2,#0x0]
  00134616: cmp r2,#0x0
  00134618: it eq
  0013461a: vmov.eq.f32 s2,s4
  0013461e: vadd.f32 s0,s2,s0
  00134622: vcvt.s32.f32 s0,s0
  00134626: vmov r2,s0
  0013462a: blx 0x00072db4
  0013462e: ldr.w r0,[r11,#0x0]
  00134632: vmov.f32 s2,s16
  00134636: ldr.w lr,[r0,#0xc]
  0013463a: ldrd r2,r3,[r0,#0x1c]
  0013463e: ldrd r12,r5,[r0,#0x28]
  00134642: ldr.w r1,[r10,#0x15c]
  00134646: add r2,r5
  00134648: ldr.w r6,[r10,#0x154]
  0013464c: add r1,r2
  0013464e: add.w r2,r3,lr
  00134652: add r2,r6
  00134654: add.w r6,r5,r5, lsl #0x1
  00134658: ldr.w r0,[r0,#0x2fc]
  0013465c: add r2,r6
  0013465e: ldr.w r9,[r10,#0x164]
  00134662: add r0,r2
  00134664: ldr.w r4,[r10,#0xec]
  00134668: add r0,r9
  0013466a: ldr.w r2,[r10,#0x19c]
  0013466e: add.w r0,r0,r1, lsl #0x1
  00134672: ldr r3,[r4,#0x4]
  00134674: add r2,r12
  00134676: vmov s0,r0
  0013467a: vcvt.f32.s32 s0,s0
  0013467e: ldr.w r1,[r10,#0x1c0]
  00134682: ldrb.w r6,[r8,#0x0]
  00134686: ldr r0,[r3,#0x8]
  00134688: add r1,r2
  0013468a: cbnz r6,0x001346a4
  0013468c: ldr r2,[0x00134914]
  0013468e: vmov.f32 s4,0x41600000
  00134692: vmov.f32 s2,0x41e00000
  00134696: add r2,pc
  00134698: ldr r2,[r2,#0x0]
  0013469a: ldrb r2,[r2,#0x0]
  0013469c: cmp r2,#0x0
  0013469e: it eq
  001346a0: vmov.eq.f32 s2,s4
  001346a4: vadd.f32 s0,s2,s0
  001346a8: vcvt.s32.f32 s0,s0
  001346ac: vmov r2,s0
  001346b0: blx 0x00072db4
  001346b4: ldr.w r0,[r11,#0x0]
  001346b8: ldr.w r9,[r10,#0xec]
  001346bc: ldr.w r12,[r10,#0x154]
  001346c0: ldr r5,[r0,#0xc]
  001346c2: ldr.w r3,[r10,#0x15c]
  001346c6: ldr.w r6,[r10,#0x164]
  001346ca: ldrd r4,r2,[r0,#0x1c]
  001346ce: ldrd lr,r1,[r0,#0x28]
  001346d2: add r2,r5
  001346d4: add.w r1,r1,r1, lsl #0x1
  001346d8: add r3,r4
  001346da: add r2,r12
  001346dc: ldr.w r0,[r0,#0x2fc]
  001346e0: add.w r1,r2,r1, lsl #0x1
  001346e4: add r3,r6
  001346e6: add r0,r1
  001346e8: ldr.w r2,[r10,#0x19c]
  001346ec: add.w r0,r0,r3, lsl #0x1
  001346f0: ldr.w r4,[r9,#0x4]
  001346f4: add r2,lr
  001346f6: vmov s0,r0
  001346fa: vcvt.f32.s32 s0,s0
  001346fe: ldr.w r1,[r10,#0x1c0]
  00134702: ldrb.w r3,[r8,#0x0]
  00134706: ldr r0,[r4,#0xc]
  00134708: add r1,r2
  0013470a: cbnz r3,0x00134724
  0013470c: ldr r2,[0x00134918]
  0013470e: vmov.f32 s2,0x41600000
  00134712: vmov.f32 s16,0x41e00000
  00134716: add r2,pc
  00134718: ldr r2,[r2,#0x0]
  0013471a: ldrb r2,[r2,#0x0]
  0013471c: cmp r2,#0x0
  0013471e: it eq
  00134720: vmov.eq.f32 s16,s2
  00134724: vadd.f32 s0,s16,s0
  00134728: vcvt.s32.f32 s0,s0
  0013472c: vmov r2,s0
  00134730: blx 0x00072db4
  00134734: ldr r0,[0x0013491c]
  00134736: ldr.w r1,[r10,#0xec]
  0013473a: add r0,pc
  0013473c: ldr.w r9,[sp,#0x4c]
  00134740: ldr r0,[r0,#0x0]
  00134742: ldr r1,[r1,#0x4]
  00134744: ldrb r2,[r0,#0x0]
  00134746: ldr r0,[r1,#0x10]
  00134748: cbz r2,0x001347bc
  0013474a: ldr r1,[0x00134920]
  0013474c: mov r2,r1
  0013474e: blx 0x00072db4
  00134752: ldr.w r0,[r11,#0x0]
  00134756: ldr.w r3,[r10,#0x15c]
  0013475a: ldr.w r12,[r10,#0xec]
  0013475e: ldr.w r5,[r0,#0x2fc]
  00134762: ldr.w r2,[r10,#0x154]
  00134766: add r3,r5
  00134768: ldr.w r6,[r10,#0x164]
  0013476c: ldr r5,[r0,#0xc]
  0013476e: ldrd r4,r1,[r0,#0x1c]
  00134772: ldr.w lr,[r0,#0x28]
  00134776: add r1,r5
  00134778: ldr r0,[r0,#0x2c]
  0013477a: add.w r3,r3,r6, lsl #0x1
  0013477e: add.w r4,r4,r4, lsl #0x1
  00134782: ldr.w r5,[r10,#0x19c]
  00134786: add r1,r4
  00134788: ldr.w r4,[r10,#0x1c0]
  0013478c: add r1,r2
  0013478e: rsb r0,r0,r0, lsl #0x3
  00134792: add r0,r1
  00134794: ldr.w r6,[r12,#0x4]
  00134798: add.w r0,r0,r3, lsl #0x1
  0013479c: add.w r1,lr,r5
  001347a0: add r1,r4
  001347a2: vmov s0,r0
  001347a6: vcvt.f32.s32 s0,s0
  001347aa: ldrb.w r2,[r8,#0x0]
  001347ae: ldr r0,[r6,#0x14]
  001347b0: cmp r2,#0x0
  001347b2: beq.w 0x00134930
  001347b6: vldr.32 s2,[pc,#0x16c]
  001347ba: b 0x00134948
  001347bc: ldr.w r1,[r11,#0x0]
  001347c0: ldr.w r3,[r10,#0x15c]
  001347c4: ldr.w lr,[r10,#0x154]
  001347c8: ldr.w r4,[r1,#0x2fc]
  001347cc: ldr.w r6,[r10,#0x164]
  001347d0: add r3,r4
  001347d2: ldr.w r12,[r10,#0x19c]
  001347d6: ldr r4,[r1,#0xc]
  001347d8: add r3,r6
  001347da: ldrd r5,r2,[r1,#0x1c]
  001347de: ldrd r9,r1,[r1,#0x28]
  001347e2: add r2,r4
  001347e4: add.w r1,r1,r1, lsl #0x1
  001347e8: vldr.32 s16,[pc,#0x138]
  001347ec: add.w r4,r5,r5, lsl #0x1
  001347f0: add r2,r4
  001347f2: vmov.f32 s2,s16
  001347f6: add r2,lr
  001347f8: add.w r1,r2,r1, lsl #0x1
  001347fc: add r1,r6
  001347fe: add.w r1,r1,r3, lsl #0x1
  00134802: add.w r3,r9,r12
  00134806: vmov s0,r1
  0013480a: vcvt.f32.s32 s0,s0
  0013480e: ldr.w r1,[r10,#0x1c0]
  00134812: ldrb.w r2,[r8,#0x0]
  00134816: add r1,r3
  00134818: cbnz r2,0x00134832
  0013481a: ldr r2,[0x00134928]
  0013481c: vmov.f32 s4,0x40e00000
  00134820: vmov.f32 s2,0x41600000
  00134824: add r2,pc
  00134826: ldr r2,[r2,#0x0]
  00134828: ldrb r2,[r2,#0x0]
  0013482a: cmp r2,#0x0
  0013482c: it eq
  0013482e: vmov.eq.f32 s2,s4
  00134832: vadd.f32 s0,s2,s0
  00134836: vcvt.s32.f32 s0,s0
  0013483a: vmov r2,s0
  0013483e: blx 0x00072db4
  00134842: ldr.w r0,[r11,#0x0]
  00134846: ldr.w r1,[r10,#0xec]
  0013484a: ldr.w r3,[r10,#0x15c]
  0013484e: ldr.w r5,[r0,#0x2fc]
  00134852: ldr.w r2,[r10,#0x154]
  00134856: add r3,r5
  00134858: ldr.w r6,[r10,#0x164]
  0013485c: ldr.w r12,[r1,#0x4]
  00134860: ldr r5,[r0,#0xc]
  00134862: ldrd r4,r1,[r0,#0x1c]
  00134866: add.w r3,r3,r6, lsl #0x1
  0013486a: add r1,r5
  0013486c: ldrd lr,r0,[r0,#0x28]
  00134870: rsb r0,r0,r0, lsl #0x3
  00134874: add.w r4,r4,r4, lsl #0x1
  00134878: add r1,r4
  0013487a: add r1,r2
  0013487c: ldr.w r2,[r10,#0x19c]
  00134880: add r0,r1
  00134882: add.w r0,r0,r3, lsl #0x1
  00134886: add r2,lr
  00134888: vmov s0,r0
  0013488c: vcvt.f32.s32 s0,s0
  00134890: ldr.w r1,[r10,#0x1c0]
  00134894: ldrb.w r3,[r8,#0x0]
  00134898: ldr.w r0,[r12,#0x14]
  0013489c: add r1,r2
  0013489e: cbnz r3,0x001348b8
  001348a0: ldr r2,[0x0013492c]
  001348a2: vmov.f32 s2,0x40e00000
  001348a6: vmov.f32 s16,0x41600000
  001348aa: add r2,pc
  001348ac: ldr r2,[r2,#0x0]
  001348ae: ldrb r2,[r2,#0x0]
  001348b0: cmp r2,#0x0
  001348b2: it eq
  001348b4: vmov.eq.f32 s16,s2
  001348b8: vadd.f32 s0,s16,s0
  001348bc: vcvt.s32.f32 s0,s0
  001348c0: vmov r2,s0
  001348c4: blx 0x00072db4
  001348c8: b 0x00134968
  001348ca: ldr.w r0,[r8,#0x0]
  001348ce: movs r1,#0x81
  001348d0: blx 0x00072f70
  001348d4: add r4,sp,#0x1a4
  001348d6: mov r1,r0
  001348d8: movs r2,#0x0
  001348da: movs r5,#0x0
  001348dc: mov r0,r4
  001348de: blx 0x0006f028
  001348e2: ldr.w r0,[r10,#0x4]
  001348e6: strd r0,r5,[sp,#0x0]
  001348ea: mov r0,r10
  001348ec: movs r1,#0xa
  001348ee: mov r2,r4
  001348f0: movs r3,#0x1
  001348f2: blx 0x00076480
  001348f6: add r0,sp,#0x1a4
  001348f8: blx 0x0006ee30
  001348fc: movs r5,#0x2
  001348fe: b.w 0x00132cd0
  00134930: ldr r2,[0x00134d0c]
  00134932: vmov.f32 s4,0x40e00000
  00134936: vmov.f32 s2,0x41600000
  0013493a: add r2,pc
  0013493c: ldr r2,[r2,#0x0]
  0013493e: ldrb r2,[r2,#0x0]
  00134940: cmp r2,#0x0
  00134942: it eq
  00134944: vmov.eq.f32 s2,s4
  00134948: vadd.f32 s0,s2,s0
  0013494c: vcvt.s32.f32 s0,s0
  00134950: vmov r2,s0
  00134954: blx 0x00072db4
  00134958: ldr.w r0,[r10,#0xec]
  0013495c: ldr.w r1,[r9,#0x44]
  00134960: ldr r0,[r0,#0x4]
  00134962: ldr r0,[r0,#0x14]
  00134964: blx 0x00072de4
  00134968: ldr r0,[sp,#0x58]
  0013496a: add.w r2,r10,#0x11c
  0013496e: movw r1,#0x6a4
  00134972: ldr r0,[r0,#0x0]
  00134974: blx 0x00071cf8
  00134978: ldr r0,[sp,#0x5c]
  0013497a: vldr.32 s2,[pc,#0x394]
  0013497e: ldr r4,[r0,#0x0]
  00134980: vmov s0,r4
  00134984: vcvt.f32.s32 s0,s0
  00134988: ldr r0,[sp,#0x54]
  0013498a: ldrb r0,[r0,#0x0]
  0013498c: cmp r0,#0x0
  0013498e: mov.w r0,#0x24
  00134992: vmul.f32 s0,s0,s2
  00134996: vcvt.s32.f32 s0,s0
  0013499a: vmov r1,s0
  0013499e: it ne
  001349a0: mov.ne r1,#0x32
  001349a2: str.w r1,[r10,#0x198]
  001349a6: blx 0x0006eb24
  001349aa: ldr r1,[sp,#0x68]
  001349ac: mov r8,r0
  001349ae: ldr.w r0,[r11,#0x0]
  001349b2: ldr r1,[r1,#0x0]
  001349b4: ldrd r2,r3,[r0,#0xc]
  001349b8: ldrd r6,r5,[r0,#0x20]
  001349bc: ldr r0,[r0,#0x28]
  001349be: add r2,r6
  001349c0: mov.w r12,#0x0
  001349c4: subs r1,r1,r2
  001349c6: subs r1,r1,r3
  001349c8: asrs r3,r4,#0x1
  001349ca: subs r1,r1,r5
  001349cc: strd r1,r12,[sp,#0x0]
  001349d0: add.w r1,r0,r4, asr #0x2
  001349d4: sub.w r3,r3,r0, lsl #0x1
  001349d8: mov r0,r8
  001349da: blx 0x00075b20
  001349de: ldr r4,[sp,#0x60]
  001349e0: ldr r0,[0x00134d14]
  001349e2: str.w r8,[r10,#0xf0]
  001349e6: add r0,pc
  001349e8: ldr r5,[r0,#0x0]
  001349ea: ldr r0,[r5,#0x0]
  001349ec: blx 0x0007258c
  001349f0: add.w r1,r0,#0x3c
  001349f4: add r0,sp,#0x15c
  001349f6: movs r2,#0x0
  001349f8: blx 0x0006f028
  001349fc: ldr r0,[sp,#0x164]
  001349fe: cmp r0,#0x3
  00134a00: bcc 0x00134a2a
  00134a02: ldr r1,[0x00134d18]
  00134a04: add r1,pc
  00134a06: add r0,sp,#0xd0
  00134a08: movs r2,#0x0
  00134a0a: blx 0x0006ee18
  00134a0e: add r0,sp,#0xdc
  00134a10: add r1,sp,#0xd0
  00134a12: add r2,sp,#0x15c
  00134a14: blx 0x0006ef98
  00134a18: add r0,sp,#0x15c
  00134a1a: add r1,sp,#0xdc
  00134a1c: blx 0x0006f2b0
  00134a20: add r0,sp,#0xdc
  00134a22: blx 0x0006ee30
  00134a26: add r0,sp,#0xd0
  00134a28: b 0x00134a40
  00134a2a: ldr r1,[0x00134d1c]
  00134a2c: add r1,pc
  00134a2e: add r0,sp,#0xdc
  00134a30: movs r2,#0x0
  00134a32: blx 0x0006ee18
  00134a36: add r0,sp,#0x15c
  00134a38: add r1,sp,#0xdc
  00134a3a: blx 0x0006f2b0
  00134a3e: add r0,sp,#0xdc
  00134a40: blx 0x0006ee30
  00134a44: ldr r1,[0x00134d20]
  00134a46: add r1,pc
  00134a48: add r0,sp,#0xa0
  00134a4a: movs r2,#0x0
  00134a4c: blx 0x0006ee18
  00134a50: add r0,sp,#0xac
  00134a52: add r1,sp,#0xa0
  00134a54: add r2,sp,#0x15c
  00134a56: blx 0x0006ef98
  00134a5a: ldr r1,[0x00134d24]
  00134a5c: add r1,pc
  00134a5e: add r0,sp,#0x94
  00134a60: movs r2,#0x0
  00134a62: blx 0x0006ee18
  00134a66: add r0,sp,#0xb8
  00134a68: add r1,sp,#0xac
  00134a6a: add r2,sp,#0x94
  00134a6c: blx 0x0006ef98
  00134a70: ldr r0,[r4,#0x0]
  00134a72: movs r1,#0x2d
  00134a74: blx 0x00072f70
  00134a78: mov r2,r0
  00134a7a: add r0,sp,#0xc4
  00134a7c: add r1,sp,#0xb8
  00134a7e: blx 0x0006ef98
  00134a82: ldr r1,[0x00134d28]
  00134a84: add r1,pc
  00134a86: add r0,sp,#0x88
  00134a88: movs r2,#0x0
  00134a8a: blx 0x0006ee18
  00134a8e: add r0,sp,#0xd0
  00134a90: add r1,sp,#0xc4
  00134a92: add r2,sp,#0x88
  00134a94: blx 0x0006ef98
  00134a98: ldr r0,[r4,#0x0]
  00134a9a: movs r1,#0x30
  00134a9c: blx 0x00072f70
  00134aa0: mov r2,r0
  00134aa2: add r0,sp,#0xdc
  00134aa4: add r1,sp,#0xd0
  00134aa6: blx 0x0006ef98
  00134aaa: ldr r0,[0x00134d2c]
  00134aac: add r0,pc
  00134aae: ldr r4,[r0,#0x0]
  00134ab0: add r0,sp,#0xd0
  00134ab2: blx r4
  00134ab4: add r0,sp,#0x88
  00134ab6: blx r4
  00134ab8: add r0,sp,#0xc4
  00134aba: blx r4
  00134abc: add r0,sp,#0xb8
  00134abe: blx r4
  00134ac0: add r0,sp,#0x94
  00134ac2: blx r4
  00134ac4: add r0,sp,#0xac
  00134ac6: blx r4
  00134ac8: add r0,sp,#0xa0
  00134aca: blx r4
  00134acc: ldr.w r4,[r10,#0xf0]
  00134ad0: ldr r1,[0x00134d30]
  00134ad2: add r1,pc
  00134ad4: add r0,sp,#0x7c
  00134ad6: movs r2,#0x0
  00134ad8: blx 0x0006ee18
  00134adc: add r0,sp,#0x70
  00134ade: add r1,sp,#0xdc
  00134ae0: movs r2,#0x0
  00134ae2: blx 0x0006f028
  00134ae6: add r1,sp,#0x7c
  00134ae8: add r2,sp,#0x70
  00134aea: mov r0,r4
  00134aec: blx 0x00075b2c
  00134af0: add r0,sp,#0x70
  00134af2: blx 0x0006ee30
  00134af6: add r0,sp,#0x7c
  00134af8: blx 0x0006ee30
  00134afc: ldr.w r0,[r10,#0xf0]
  00134b00: movs r1,#0x1
  00134b02: blx 0x00076528
  00134b06: ldr r0,[sp,#0x50]
  00134b08: str r5,[sp,#0x48]
  00134b0a: ldrb r0,[r0,#0x0]
  00134b0c: cmp r0,#0x0
  00134b0e: beq 0x00134b90
  00134b10: ldr r0,[sp,#0x54]
  00134b12: ldrb r0,[r0,#0x0]
  00134b14: cbnz r0,0x00134b90
  00134b16: movs r0,#0xc8
  00134b18: blx 0x0006eb24
  00134b1c: mov r5,r0
  00134b1e: ldr r0,[sp,#0x60]
  00134b20: ldr r0,[r0,#0x0]
  00134b22: movs r1,#0x62
  00134b24: blx 0x00072f70
  00134b28: str r0,[sp,#0x5c]
  00134b2a: ldr r1,[sp,#0x6c]
  00134b2c: ldr.w r0,[r11,#0x0]
  00134b30: mov r2,r1
  00134b32: ldr.w r1,[r1,#0x14c]
  00134b36: ldrd r4,r8,[r2,#0x200]
  00134b3a: ldr r2,[sp,#0x68]
  00134b3c: ldr r6,[r0,#0x10]
  00134b3e: ldr.w r11,[r2,#0x0]
  00134b42: ldrd r10,r9,[r0,#0x24]
  00134b46: ldr r0,[sp,#0x58]
  00134b48: ldr r0,[r0,#0x0]
  00134b4a: blx 0x00072d90
  00134b4e: add.w r1,r4,r9
  00134b52: vmov s2,r8
  00134b56: vmov.f32 s0,0x3f000000
  00134b5a: vmov s4,r1
  00134b5e: vcvt.f32.s32 s2,s2
  00134b62: vcvt.f32.s32 s4,s4
  00134b66: vmla.f32 s4,s2,s0
  00134b6a: movs r1,#0x24
  00134b6c: movs r2,#0x7
  00134b6e: str r1,[sp,#0x4]
  00134b70: vcvt.s32.f32 s0,s4
  00134b74: add.w r1,r11,#0x5
  00134b78: subs r1,r1,r6
  00134b7a: sub.w r1,r1,r10
  00134b7e: subs r0,r1,r0
  00134b80: ldr r1,[sp,#0x5c]
  00134b82: str r0,[sp,#0x0]
  00134b84: mov r0,r5
  00134b86: vmov r3,s0
  00134b8a: blx 0x00074578
  00134b8e: b 0x00134bc4
  00134b90: movs r0,#0xc8
  00134b92: blx 0x0006eb24
  00134b96: mov r5,r0
  00134b98: ldr r0,[sp,#0x60]
  00134b9a: ldr r0,[r0,#0x0]
  00134b9c: movs r1,#0x62
  00134b9e: blx 0x00072f70
  00134ba2: mov r1,r0
  00134ba4: ldr r2,[sp,#0x5c]
  00134ba6: ldr.w r0,[r11,#0x0]
  00134baa: ldr r3,[sp,#0x68]
  00134bac: ldr r2,[r2,#0x0]
  00134bae: ldr r0,[r0,#0x2c]
  00134bb0: ldr r3,[r3,#0x0]
  00134bb2: movs r6,#0x22
  00134bb4: subs r3,r3,r0
  00134bb6: strd r3,r6,[sp,#0x0]
  00134bba: subs r3,r2,r0
  00134bbc: mov r0,r5
  00134bbe: movs r2,#0x0
  00134bc0: blx 0x00074578
  00134bc4: ldr r6,[sp,#0x6c]
  00134bc6: movs r4,#0x0
  00134bc8: movs r0,#0x34
  00134bca: strd r0,r4,[r5,#0x0]
  00134bce: ldr.w r1,[r6,#0xc0]
  00134bd2: mov r0,r5
  00134bd4: blx 0x0007648c
  00134bd8: strb.w r4,[r6,#0x170]
  00134bdc: strh.w r4,[r6,#0x17f]
  00134be0: strb.w r4,[r6,#0x181]
  00134be4: movs r0,#0x5c
  00134be6: blx 0x0006eb24
  00134bea: mov r4,r0
  00134bec: blx 0x00074584
  00134bf0: ldr r5,[sp,#0x4c]
  00134bf2: ldr r0,[sp,#0x48]
  00134bf4: str.w r4,[r6,#0x104]
  00134bf8: ldr r0,[r0,#0x0]
  00134bfa: blx 0x00071704
  00134bfe: mov r4,r0
  00134c00: ldrb.w r0,[r5,#0x35]
  00134c04: cbz r0,0x00134c40
  00134c06: ldr r0,[0x00134f14]
  00134c08: add r0,pc
  00134c0a: ldr r0,[r0,#0x0]
  00134c0c: ldrb r0,[r0,#0x0]
  00134c0e: cbz r0,0x00134c80
  00134c10: ldrb.w r0,[r5,#0x41]
  00134c14: cbnz r0,0x00134c80
  00134c16: ldr r0,[0x00134f18]
  00134c18: movs r1,#0x1
  00134c1a: strb.w r1,[r5,#0x41]
  00134c1e: add r0,pc
  00134c20: ldr r0,[r0,#0x0]
  00134c22: ldr r0,[r0,#0x0]
  00134c24: blx 0x00071788
  00134c28: ldr r0,[sp,#0x60]
  00134c2a: ldr.w r4,[r6,#0x104]
  00134c2e: ldr r0,[r0,#0x0]
  00134c30: movs r1,#0x7b
  00134c32: blx 0x00072f70
  00134c36: mov r1,r0
  00134c38: mov r0,r4
  00134c3a: blx 0x00074794
  00134c3e: b 0x00134c76
  00134c40: ldrb.w r0,[r4,#0x6c]
  00134c44: cmp r0,#0x0
  00134c46: beq 0x00134c06
  00134c48: ldr r0,[0x00134f10]
  00134c4a: movs r1,#0x1
  00134c4c: strb.w r1,[r5,#0x35]
  00134c50: add r0,pc
  00134c52: ldr r0,[r0,#0x0]
  00134c54: ldr r0,[r0,#0x0]
  00134c56: blx 0x00071788
  00134c5a: movs r0,#0x0
  00134c5c: strb.w r0,[r4,#0x6c]
  00134c60: ldr r0,[sp,#0x60]
  00134c62: ldr.w r4,[r6,#0x104]
  00134c66: ldr r0,[r0,#0x0]
  00134c68: movs r1,#0x4a
  00134c6a: blx 0x00072f70
  00134c6e: mov r1,r0
  00134c70: mov r0,r4
  00134c72: blx 0x00074794
  00134c76: movs r0,#0x1
  00134c78: strb.w r0,[r6,#0x17f]
  00134c7c: strb.w r0,[r6,#0x170]
  00134c80: movs r0,#0x0
  00134c82: vmov.i32 q8,#0x0
  00134c86: mov.w r1,#0xffffffff
  00134c8a: mov.w r2,#0x3f000000
  00134c8e: strd r0,r0,[r6,#0xfc]
  00134c92: str.w r0,[r6,#0x16c]
  00134c96: str.w r0,[r6,#0x177]
  00134c9a: str.w r0,[r6,#0x173]
  00134c9e: strh.w r0,[r6,#0x171]
  00134ca2: str.w r0,[r6,#0x17b]
  00134ca6: str.w r1,[r6,#0x18c]
  00134caa: strd r0,r0,[r6,#0xc4]
  00134cae: strh.w r0,[r6,#0x108]
  00134cb2: str.w r2,[r6,#0x1a4]
  00134cb6: str.w r1,[r6,#0x184]
  00134cba: strh.w r0,[r6,#0x188]
  00134cbe: strb.w r0,[r6,#0x190]
  00134cc2: str.w r0,[r6,#0x194]
  00134cc6: strb.w r0,[r6,#0x1de]
  00134cca: str.w r0,[r6,#0x1da]
  00134cce: addw r0,r6,#0x215
  00134cd2: vst1.8 {d16,d17},[r0]
  00134cd6: add.w r0,r6,#0x20c
  00134cda: vst1.32 {d16,d17},[r0]
  00134cde: add r0,sp,#0xdc
  00134ce0: blx 0x0006ee30
  00134ce4: add r0,sp,#0x15c
  00134ce6: blx 0x0006ee30
  00134cea: ldr r0,[sp,#0x24c]
  00134cec: ldr r1,[sp,#0x3c]
  00134cee: ldr r1,[r1,#0x0]
  00134cf0: subs r0,r1,r0
  00134cf2: itttt eq
  00134cf4: mov.eq r0,r6
  00134cf6: add.eq.w sp,sp,#0x250
  00134cfa: vpop.eq {d8,d9}
  00134cfe: add.eq sp,#0x4
  00134d00: itt eq
  00134d02: pop.eq.w {r8,r9,r10,r11}
  00134d06: pop.eq {r4,r5,r6,r7,pc}
  00134d08: blx 0x0006e824
```
