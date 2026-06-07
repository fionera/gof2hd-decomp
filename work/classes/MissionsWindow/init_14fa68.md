# MissionsWindow::init
elf 0x14fa68 body 2070
Sig: int __stdcall init(EVP_PKEY_CTX * ctx)

## decompile
```c

/* MissionsWindow::init() */

int MissionsWindow::init(EVP_PKEY_CTX *ctx)

{
  undefined1 *puVar1;
  uint uVar2;
  Array *pAVar3;
  TouchButton *pTVar4;
  String *pSVar5;
  int iVar6;
  void *pvVar7;
  ScrollTouchWindow *pSVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  Mission *pMVar13;
  undefined4 uVar14;
  Agent *this;
  int *piVar15;
  ChoiceWindow *this_00;
  WantedWindow *this_01;
  float *pfVar16;
  undefined4 *puVar17;
  int *piVar18;
  int *piVar19;
  int *piVar20;
  int iVar21;
  int iVar22;
  int iVar23;
  code *pcVar24;
  int iVar25;
  int iVar26;
  ImageFactory *this_02;
  int iVar27;
  undefined4 uVar28;
  int iVar29;
  int iVar30;
  int iVar31;
  bool bVar32;
  float fVar33;
  undefined8 uVar34;
  String aSStack_124 [12];
  String aSStack_118 [12];
  String aSStack_10c [12];
  undefined1 auStack_100 [12];
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
  undefined1 auStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  undefined1 auStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  iVar6 = DAT_0015fda0;
  piVar19 = *(int **)(DAT_0015fd84 + 0x15fa7e);
  local_28 = *piVar19;
  if (**(char **)(DAT_0015fd80 + 0x15fa7c) == '\0') {
    uVar14 = **(undefined4 **)(DAT_0015fd90 + 0x15faaa);
    uVar28 = **(undefined4 **)(DAT_0015fd94 + 0x15faac);
    *(undefined4 *)(ctx + 0x30) = 0;
    *(undefined4 *)(ctx + 0x34) = 0;
    *(undefined4 *)(ctx + 0x38) = uVar28;
    *(undefined4 *)(ctx + 0x3c) = uVar14;
  }
  else {
    if (**(char **)(DAT_0015fd88 + 0x15fa92) == '\0') {
      pfVar16 = (float *)&DAT_001603e8;
      bVar32 = **(char **)(DAT_0015fd98 + 0x15fac6) == '\0';
      if (bVar32) {
        pfVar16 = (float *)&DAT_001603ec;
      }
      uVar2 = 0x514;
      fVar33 = *pfVar16;
      if (bVar32) {
        uVar2 = 0x28a;
      }
    }
    else {
      uVar2 = 0x392;
      fVar33 = DAT_0015fd8c;
    }
    puVar17 = (undefined4 *)(DAT_0015fd9c + 0x15fae8);
    *(uint *)(ctx + 0x38) = uVar2;
    piVar18 = (int *)*puVar17;
    piVar20 = *(int **)(iVar6 + 0x15faec);
    *(int *)(ctx + 0x3c) = (int)fVar33;
    *(uint *)(ctx + 0x30) = (*piVar18 >> 1) - (uVar2 >> 1);
    *(int *)(ctx + 0x34) = (*piVar20 >> 1) - ((int)fVar33 >> 1);
  }
  pAVar3 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(Array **)(ctx + 0x14) = pAVar3;
  ArraySetLength<TouchButton*>(2,pAVar3);
  pTVar4 = operator_new(200);
  piVar18 = *(int **)(DAT_0015fda4 + 0x15fb32);
  pSVar5 = (String *)GameText::getText(*piVar18);
  iVar21 = *(int *)(ctx + 0x30);
  iVar25 = *(int *)(ctx + 0x38);
  piVar20 = *(int **)(DAT_0015fda8 + 0x15fb48);
  iVar6 = Layout::getHelpButtonOffset();
  TouchButton::TouchButton(pTVar4,pSVar5,3,(iVar25 + iVar21) - iVar6,*(int *)(ctx + 0x34),'\x12');
  *(TouchButton **)(*(int *)(*(int *)(ctx + 0x14) + 4) + 4) = pTVar4;
  pTVar4 = operator_new(200);
  pSVar5 = (String *)GameText::getText(*piVar18);
  iVar30 = *(int *)(ctx + 0x30);
  iVar25 = *(int *)(ctx + 0x38);
  iVar6 = Layout::getHelpButtonOffset();
  iVar21 = TouchButton::getWidth();
  TouchButton::TouchButton
            (pTVar4,pSVar5,3,(((iVar25 + iVar30) - iVar6) - iVar21) + *(int *)(*piVar20 + 0x38),
             *(int *)(ctx + 0x34),'\x12');
  **(undefined4 **)(*(int *)(ctx + 0x14) + 4) = pTVar4;
  TouchButton::setAlwaysPressed((TouchButton *)**(undefined4 **)(*(int *)(ctx + 0x14) + 4),true);
  Layout::setWindowDimensions
            ((Layout *)*piVar20,*(int *)(ctx + 0x30),*(int *)(ctx + 0x34),*(int *)(ctx + 0x38),
             *(int *)(ctx + 0x3c));
  if (*(Array **)(ctx + 0x18) != (Array *)0x0) {
    ArrayReleaseClasses<ImagePart*>(*(Array **)(ctx + 0x18));
    if (*(Array<ImagePart*> **)(ctx + 0x18) != (Array<ImagePart*> *)0x0) {
      pvVar7 = (void *)Array<ImagePart*>::~Array(*(Array<ImagePart*> **)(ctx + 0x18));
      operator_delete(pvVar7);
    }
  }
  *(undefined4 *)(ctx + 0x18) = 0;
  if (*(ScrollTouchWindow **)ctx != (ScrollTouchWindow *)0x0) {
    pvVar7 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)ctx);
    operator_delete(pvVar7);
  }
  *(undefined4 *)ctx = 0;
  if (*(ScrollTouchWindow **)(ctx + 4) != (ScrollTouchWindow *)0x0) {
    pvVar7 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(ctx + 4));
    operator_delete(pvVar7);
  }
  *(undefined4 *)(ctx + 4) = 0;
  if (*(ChoiceWindow **)(ctx + 0xc) != (ChoiceWindow *)0x0) {
    pvVar7 = (void *)ChoiceWindow::~ChoiceWindow(*(ChoiceWindow **)(ctx + 0xc));
    operator_delete(pvVar7);
  }
  *(undefined4 *)(ctx + 0xc) = 0;
  if (*(TouchButton **)(ctx + 0x24) != (TouchButton *)0x0) {
    pvVar7 = (void *)TouchButton::~TouchButton(*(TouchButton **)(ctx + 0x24));
    operator_delete(pvVar7);
  }
  *(undefined4 *)(ctx + 0x24) = 0;
  if (*(TouchButton **)(ctx + 0x28) != (TouchButton *)0x0) {
    pvVar7 = (void *)TouchButton::~TouchButton(*(TouchButton **)(ctx + 0x28));
    operator_delete(pvVar7);
  }
  *(undefined4 *)(ctx + 0x28) = 0;
  if (*(TouchButton **)(ctx + 0x2c) != (TouchButton *)0x0) {
    pvVar7 = (void *)TouchButton::~TouchButton(*(TouchButton **)(ctx + 0x2c));
    operator_delete(pvVar7);
  }
  *(undefined4 *)(ctx + 8) = 0;
  *(undefined4 *)(ctx + 0x2c) = 0;
  *(undefined4 *)(ctx + 0x20) = 0;
  iVar22 = *piVar20;
  iVar26 = *(int *)(ctx + 0x34);
  iVar6 = *(int *)(iVar22 + 0xc);
  iVar21 = *(int *)(iVar22 + 0x20);
  iVar25 = *(int *)(iVar22 + 0x2c);
  iVar30 = *(int *)(iVar22 + 0x5c);
  pSVar8 = operator_new(0x24);
  iVar9 = *(int *)(iVar22 + 0x10);
  iVar10 = *(int *)(iVar22 + 0x24);
  iVar27 = *(int *)(iVar22 + 0x28);
  iVar31 = *(int *)(iVar22 + 0x2c);
  iVar22 = *(int *)(ctx + 0x3c);
  iVar23 = *(int *)(ctx + 0x30);
  iVar29 = *(int *)(ctx + 0x38);
  puVar17 = *(undefined4 **)(DAT_0015fdac + 0x15fc9c);
  iVar11 = Status::gameWon();
  iVar12 = 0;
  iVar25 = iVar6 + iVar26 + iVar21 + iVar30 + iVar25;
  if (iVar11 == 0) {
    iVar12 = *(int *)(*piVar20 + 0x30);
  }
  ScrollTouchWindow::ScrollTouchWindow
            (pSVar8,iVar27 + iVar23,iVar25,(iVar29 >> 1) - (iVar31 + iVar27),
             (((((iVar26 - iVar25) + iVar22) - iVar9) - iVar10) - iVar12) +
             *(int *)(*piVar20 + 0x2c) * -2,false);
  *(ScrollTouchWindow **)ctx = pSVar8;
  iVar6 = Status::gameWon();
  if ((iVar6 == 0) ||
     (*(char *)(*(int *)(DAT_0015fdb0 + 0x15fd16) + 0x37) != '\0' ||
      *(char *)(*(int *)(DAT_0015fdb0 + 0x15fd16) + 0x35) != '\0')) {
    AbyssEngine::String::String(aSStack_34,(char *)(DAT_0015ffec + 0x15fdc6),false);
    iVar6 = Status::getCurrentCampaignMission();
    if (iVar6 < 0xa4) {
      iVar6 = *piVar18;
      Status::getCurrentCampaignMission();
      pSVar5 = (String *)GameText::getText(iVar6);
      AbyssEngine::String::operator=(aSStack_34,pSVar5);
    }
    Status::getCampaignMission();
    iVar6 = Mission::getType();
    if (iVar6 == 0xa7) {
LAB_0015fe16:
      uVar28 = *puVar17;
      AbyssEngine::String::String(aSStack_4c,aSStack_34,false);
      Status::getCampaignMission();
      iVar6 = Mission::getProductionGoodAmount();
      Status::getCampaignMission();
      iVar21 = Mission::getStatusValue();
      AbyssEngine::String::String(aSStack_58,iVar6 - iVar21);
      uVar14 = AbyssEngine::String::String(aSStack_64,(char *)(DAT_0015fff4 + 0x15fe4c),false);
      Status::replaceHash(auStack_40,uVar28,aSStack_4c,aSStack_58,uVar14);
      AbyssEngine::String::operator=(aSStack_34,auStack_40);
      pcVar24 = *(code **)(DAT_0015fff8 + 0x15fe6e);
      (*pcVar24)(auStack_40);
      (*pcVar24)(aSStack_64);
      (*pcVar24)(aSStack_58);
      puVar1 = &stack0x000000f8;
    }
    else {
      Status::getCampaignMission();
      iVar6 = Mission::getType();
      if (iVar6 == 0xae) goto LAB_0015fe16;
      uVar28 = *puVar17;
      AbyssEngine::String::String(aSStack_70,aSStack_34,false);
      Status::getCampaignMission();
      Mission::getTargetStationName();
      uVar14 = AbyssEngine::String::String(aSStack_88,(char *)(DAT_00160000 + 0x15fee0),false);
      Status::replaceHash(auStack_40,uVar28,aSStack_70,auStack_7c,uVar14);
      AbyssEngine::String::operator=(aSStack_34,auStack_40);
      pcVar24 = *(code **)(DAT_00160004 + 0x15ff02);
      (*pcVar24)(auStack_40);
      (*pcVar24)(aSStack_88);
      (*pcVar24)(auStack_7c);
      puVar1 = &stack0x000000d4;
    }
    (*pcVar24)(puVar1 + -0x144);
    pSVar8 = *(ScrollTouchWindow **)ctx;
    AbyssEngine::String::String(aSStack_94,(char *)(DAT_00160008 + 0x15ff1c),false);
    AbyssEngine::String::String(aSStack_a0,aSStack_34,false);
    ScrollTouchWindow::setText(pSVar8,aSStack_94,aSStack_a0);
    pcVar24 = *(code **)(DAT_0016000c + 0x15ff3c);
    (*pcVar24)(aSStack_a0);
    (*pcVar24)(aSStack_94);
    (*pcVar24)(aSStack_34);
    goto LAB_0015ff48;
  }
  iVar6 = Achievements::gotAllGoldMedals();
  if (iVar6 == 0) {
LAB_0015fd44:
    pSVar8 = *(ScrollTouchWindow **)ctx;
    AbyssEngine::String::String(aSStack_c4,(char *)(DAT_0015fdb8 + 0x15fd52),false);
    pSVar5 = (String *)GameText::getText(*piVar18);
    AbyssEngine::String::String(aSStack_d0,pSVar5,false);
    ScrollTouchWindow::setText(pSVar8,aSStack_c4,aSStack_d0);
    AbyssEngine::String::~String(aSStack_d0);
    puVar1 = &stack0x00000080;
  }
  else {
    Status::getShip();
    iVar6 = Ship::getIndex();
    if (iVar6 == 8) goto LAB_0015fd44;
    pSVar8 = *(ScrollTouchWindow **)ctx;
    AbyssEngine::String::String(aSStack_ac,(char *)(DAT_0015fffc + 0x15fe8c),false);
    pSVar5 = (String *)GameText::getText(*piVar18);
    AbyssEngine::String::String(aSStack_b8,pSVar5,false);
    ScrollTouchWindow::setText(pSVar8,aSStack_ac,aSStack_b8);
    AbyssEngine::String::~String(aSStack_b8);
    puVar1 = &stack0x00000098;
  }
  AbyssEngine::String::~String((String *)(puVar1 + -0x144));
LAB_0015ff48:
  Status::getFreelanceMission();
  iVar21 = Mission::isEmpty();
  pSVar8 = operator_new(0x24);
  iVar10 = *piVar20;
  iVar6 = *(int *)(ctx + 0x38) >> 1;
  iVar30 = *(int *)(iVar10 + 0x2c);
  iVar9 = *(int *)(ctx + 0x30) + iVar6 + iVar30;
  if (iVar21 == 0) {
    iVar21 = *(int *)(iVar10 + 0x2d8) + iVar25 + iVar30;
    ScrollTouchWindow::ScrollTouchWindow
              (pSVar8,iVar9,iVar21,(iVar6 - iVar30) - *(int *)(iVar10 + 0x28),
               (((((*(int *)(ctx + 0x34) - iVar21) + *(int *)(ctx + 0x3c)) - *(int *)(iVar10 + 0x10)
                 ) - *(int *)(iVar10 + 0x24)) - *(int *)(iVar10 + 0x4c)) - *(int *)(iVar10 + 0x30),
               false);
    *(ScrollTouchWindow **)(ctx + 4) = pSVar8;
    pMVar13 = (Mission *)Status::getFreelanceMission();
    Mission::getAgent(pMVar13);
    Globals::getAgentMissionText((Agent *)aSStack_34);
    uVar28 = *puVar17;
    AbyssEngine::String::String(aSStack_f4,(Agent *)aSStack_34,false);
    Status::getFreelanceMission();
    Mission::getReward();
    Status::getFreelanceMission();
    Mission::getBonus();
    Layout::formatCredits((int)auStack_100);
    uVar14 = AbyssEngine::String::String(aSStack_10c,(char *)(DAT_001603f8 + 0x1600bc),false);
    Status::replaceHash(auStack_40,uVar28,aSStack_f4,auStack_100,uVar14);
    AbyssEngine::String::operator=(aSStack_34,auStack_40);
    pcVar24 = *(code **)(DAT_001603fc + 0x1600de);
    (*pcVar24)(auStack_40);
    (*pcVar24)(aSStack_10c);
    (*pcVar24)(auStack_100);
    (*pcVar24)(aSStack_f4);
    pSVar8 = *(ScrollTouchWindow **)(ctx + 4);
    AbyssEngine::String::String(aSStack_118,(char *)(DAT_00160400 + 0x1600f8),false);
    AbyssEngine::String::String(aSStack_124,aSStack_34,false);
    ScrollTouchWindow::setText(pSVar8,aSStack_118,aSStack_124);
    AbyssEngine::String::~String(aSStack_124);
    AbyssEngine::String::~String(aSStack_118);
    this_02 = (ImageFactory *)**(undefined4 **)(DAT_00160404 + 0x160124);
    pMVar13 = (Mission *)Status::getFreelanceMission();
    this = (Agent *)Mission::getAgent(pMVar13);
    piVar15 = (int *)Agent::getImageParts(this);
    uVar14 = ImageFactory::loadChar(this_02,piVar15);
    *(undefined4 *)(ctx + 0x18) = uVar14;
    puVar1 = &stack0x00000110;
  }
  else {
    ScrollTouchWindow::ScrollTouchWindow
              (pSVar8,iVar9,iVar25,(iVar6 - iVar30) - *(int *)(iVar10 + 0x28),
               ((*(int *)(ctx + 0x3c) + (*(int *)(ctx + 0x34) - (iVar25 + iVar30 * 2))) -
               *(int *)(iVar10 + 0x10)) - *(int *)(iVar10 + 0x24),false);
    iVar6 = DAT_00160010;
    *(ScrollTouchWindow **)(ctx + 4) = pSVar8;
    AbyssEngine::String::String(aSStack_dc,(char *)(iVar6 + 0x15ffbe),false);
    pSVar5 = (String *)GameText::getText(*piVar18);
    AbyssEngine::String::String(aSStack_e8,pSVar5,false);
    ScrollTouchWindow::setText(pSVar8,aSStack_dc,aSStack_e8);
    AbyssEngine::String::~String(aSStack_e8);
    puVar1 = &stack0x00000068;
  }
  AbyssEngine::String::~String((String *)(puVar1 + -0x144));
  iVar21 = *(int *)(ctx + 0x38);
  iVar25 = *(int *)(*piVar20 + 0x28);
  iVar6 = Status::inAlienOrbit();
  if (iVar6 == 0) {
    iVar25 = ((iVar21 >> 1) - (iVar21 >> 0x1f) >> 1) - iVar25;
    iVar6 = Status::gameWon();
    if (iVar6 == 0) {
      pTVar4 = operator_new(200);
      pSVar5 = (String *)GameText::getText(*piVar18);
      iVar6 = *piVar20;
      TouchButton::TouchButton
                (pTVar4,pSVar5,0,*(int *)(iVar6 + 0x28) + *(int *)(ctx + 0x30),
                 (((*(int *)(ctx + 0x34) + *(int *)(ctx + 0x3c)) - *(int *)(iVar6 + 0x10)) -
                 *(int *)(iVar6 + 0x24)) - *(int *)(iVar6 + 0x2c),iVar25,'!','\x04');
      *(TouchButton **)(ctx + 0x24) = pTVar4;
    }
    Status::getFreelanceMission();
    iVar6 = Mission::isEmpty();
    if (iVar6 == 0) {
      pTVar4 = operator_new(200);
      pSVar5 = (String *)GameText::getText(*piVar18);
      iVar6 = *piVar20;
      TouchButton::TouchButton
                (pTVar4,pSVar5,0,
                 *(int *)(ctx + 0x30) + (*(int *)(ctx + 0x38) >> 1) + *(int *)(iVar6 + 0x2c),
                 (((*(int *)(ctx + 0x34) - *(int *)(iVar6 + 0x2c)) + *(int *)(ctx + 0x3c)) -
                 *(int *)(iVar6 + 0x10)) - *(int *)(iVar6 + 0x24),iVar25,'!','\x04');
      iVar6 = DAT_00160408;
      *(TouchButton **)(ctx + 0x28) = pTVar4;
      iVar6 = AbyssEngine::ApplicationManager::GetCurrentApplicationModule
                        ((ApplicationManager *)**(undefined4 **)(iVar6 + 0x160224));
      if (iVar6 == 5) {
        pTVar4 = operator_new(200);
        pSVar5 = (String *)GameText::getText(*piVar18);
        iVar6 = *piVar20;
        TouchButton::TouchButton
                  (pTVar4,pSVar5,0,
                   *(int *)(ctx + 0x30) + iVar25 + (*(int *)(ctx + 0x38) >> 1) +
                   *(int *)(iVar6 + 0x2c) * 2,
                   (((*(int *)(ctx + 0x34) - *(int *)(iVar6 + 0x2c)) + *(int *)(ctx + 0x3c)) -
                   *(int *)(iVar6 + 0x10)) - *(int *)(iVar6 + 0x24),iVar25,'!','\x04');
        iVar6 = DAT_001603f0;
        *(TouchButton **)(ctx + 0x2c) = pTVar4;
        TouchButton::setTextColor(pTVar4,iVar6);
      }
      this_00 = operator_new(0x5c);
      ChoiceWindow::ChoiceWindow(this_00);
      *(ChoiceWindow **)(ctx + 0xc) = this_00;
    }
  }
  *(undefined4 *)(ctx + 0x40) = 0;
  ctx[0x23] = (EVP_PKEY_CTX)0x0;
  uVar34 = Status::wantedBoardAccessible();
  if ((int)uVar34 != 0) {
    if (*(WantedWindow **)(ctx + 0x10) == (WantedWindow *)0x0) {
      this_01 = operator_new(0xb4);
      WantedWindow::WantedWindow(this_01);
      *(WantedWindow **)(ctx + 0x10) = this_01;
    }
    else {
      WantedWindow::init(*(WantedWindow **)(ctx + 0x10),(EVP_PKEY_CTX *)((ulonglong)uVar34 >> 0x20))
      ;
    }
  }
  if (*piVar19 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}

```

## target disasm
```
  0015fa68: push {r4,r5,r6,r7,lr}
  0015fa6a: add r7,sp,#0xc
  0015fa6c: push {r8,r9,r10,r11}
  0015fa70: sub sp,#0x144
  0015fa72: mov r6,r0
  0015fa74: ldr r0,[0x0015fd80]
  0015fa76: ldr r1,[0x0015fd84]
  0015fa78: add r0,pc
  0015fa7a: add r1,pc
  0015fa7c: ldr r0,[r0,#0x0]
  0015fa7e: ldr r2,[r1,#0x0]
  0015fa80: ldrb r0,[r0,#0x0]
  0015fa82: ldr r1,[r2,#0x0]
  0015fa84: cmp r0,#0x0
  0015fa86: str r1,[sp,#0x140]
  0015fa88: str r2,[sp,#0x14]
  0015fa8a: beq 0x0015faa0
  0015fa8c: ldr r0,[0x0015fd88]
  0015fa8e: add r0,pc
  0015fa90: ldr r0,[r0,#0x0]
  0015fa92: ldrb r0,[r0,#0x0]
  0015fa94: cbz r0,0x0015fabc
  0015fa96: vldr.32 s0,[pc,#0x2f4]
  0015fa9a: movw r0,#0x392
  0015fa9e: b 0x0015fadc
  0015faa0: ldr r0,[0x0015fd90]
  0015faa2: movs r2,#0x0
  0015faa4: ldr r1,[0x0015fd94]
  0015faa6: add r0,pc
  0015faa8: add r1,pc
  0015faaa: ldr r0,[r0,#0x0]
  0015faac: ldr r1,[r1,#0x0]
  0015faae: ldr r0,[r0,#0x0]
  0015fab0: ldr r1,[r1,#0x0]
  0015fab2: strd r2,r2,[r6,#0x30]
  0015fab6: strd r1,r0,[r6,#0x38]
  0015faba: b 0x0015fb0e
  0015fabc: ldr r0,[0x0015fd98]
  0015fabe: adr.w r1,0x1603e8
  0015fac2: add r0,pc
  0015fac4: ldr r0,[r0,#0x0]
  0015fac6: ldrb r0,[r0,#0x0]
  0015fac8: cmp r0,#0x0
  0015faca: it eq
  0015facc: add.eq r1,#0x4
  0015face: movw r0,#0x514
  0015fad2: vldr.32 s0,[r1]
  0015fad6: it eq
  0015fad8: movw.eq r0,#0x28a
  0015fadc: vcvt.s32.f32 s2,s0
  0015fae0: ldr r1,[0x0015fd9c]
  0015fae2: ldr r2,[0x0015fda0]
  0015fae4: add r1,pc
  0015fae6: str r0,[r6,#0x38]
  0015fae8: add r2,pc
  0015faea: ldr r1,[r1,#0x0]
  0015faec: ldr r2,[r2,#0x0]
  0015faee: vstr.32 s2,[r6,#0x3c]
  0015faf2: ldr r1,[r1,#0x0]
  0015faf4: asrs r1,r1,#0x1
  0015faf6: sub.w r0,r1,r0, lsr #0x1
  0015fafa: str r0,[r6,#0x30]
  0015fafc: ldr r0,[r2,#0x0]
  0015fafe: vcvt.s32.f32 s0,s0
  0015fb02: asrs r0,r0,#0x1
  0015fb04: vmov r1,s0
  0015fb08: sub.w r0,r0,r1, asr #0x1
  0015fb0c: str r0,[r6,#0x34]
  0015fb0e: movs r0,#0xc
  0015fb10: blx 0x0006eb24
  0015fb14: mov r5,r0
  0015fb16: blx 0x00074f38
  0015fb1a: movs r0,#0x2
  0015fb1c: mov r1,r5
  0015fb1e: str r5,[r6,#0x14]
  0015fb20: blx 0x00074f44
  0015fb24: movs r0,#0xc8
  0015fb26: blx 0x0006eb24
  0015fb2a: mov r9,r0
  0015fb2c: ldr r0,[0x0015fda4]
  0015fb2e: add r0,pc
  0015fb30: ldr r0,[r0,#0x0]
  0015fb32: str r0,[sp,#0x3c]
  0015fb34: ldr r0,[r0,#0x0]
  0015fb36: movw r1,#0xc93
  0015fb3a: blx 0x00072f70
  0015fb3e: mov r8,r0
  0015fb40: ldr r0,[0x0015fda8]
  0015fb42: ldr r4,[r6,#0x30]
  0015fb44: add r0,pc
  0015fb46: ldr r5,[r6,#0x38]
  0015fb48: ldr.w r11,[r0,#0x0]
  0015fb4c: ldr.w r0,[r11,#0x0]
  0015fb50: blx 0x00075ac0
  0015fb54: ldr r1,[r6,#0x34]
  0015fb56: movs r2,#0x12
  0015fb58: strd r1,r2,[sp,#0x0]
  0015fb5c: adds r1,r5,r4
  0015fb5e: subs r3,r1,r0
  0015fb60: mov r0,r9
  0015fb62: mov r1,r8
  0015fb64: movs r2,#0x3
  0015fb66: blx 0x00074578
  0015fb6a: ldr r0,[r6,#0x14]
  0015fb6c: ldr r0,[r0,#0x4]
  0015fb6e: str.w r9,[r0,#0x4]
  0015fb72: movs r0,#0xc8
  0015fb74: blx 0x0006eb24
  0015fb78: mov r9,r0
  0015fb7a: ldr r0,[sp,#0x3c]
  0015fb7c: ldr r0,[r0,#0x0]
  0015fb7e: movs r1,#0x81
  0015fb80: blx 0x00072f70
  0015fb84: mov r8,r0
  0015fb86: ldr.w r10,[r6,#0x30]
  0015fb8a: ldr r4,[r6,#0x38]
  0015fb8c: ldr.w r0,[r11,#0x0]
  0015fb90: blx 0x00075ac0
  0015fb94: mov r5,r0
  0015fb96: ldr r0,[r6,#0x14]
  0015fb98: ldr r0,[r0,#0x4]
  0015fb9a: ldr r0,[r0,#0x4]
  0015fb9c: blx 0x00074da0
  0015fba0: ldr.w r1,[r11,#0x0]
  0015fba4: ldr r2,[r6,#0x34]
  0015fba6: ldr r1,[r1,#0x38]
  0015fba8: movs r3,#0x12
  0015fbaa: strd r2,r3,[sp,#0x0]
  0015fbae: add.w r2,r4,r10
  0015fbb2: subs r2,r2,r5
  0015fbb4: subs r0,r2,r0
  0015fbb6: movs r2,#0x3
  0015fbb8: adds r3,r0,r1
  0015fbba: mov r0,r9
  0015fbbc: mov r1,r8
  0015fbbe: blx 0x00074578
  0015fbc2: ldr r0,[r6,#0x14]
  0015fbc4: movs r1,#0x1
  0015fbc6: ldr r0,[r0,#0x4]
  0015fbc8: str.w r9,[r0,#0x0]
  0015fbcc: ldr r0,[r6,#0x14]
  0015fbce: ldr r0,[r0,#0x4]
  0015fbd0: ldr r0,[r0,#0x0]
  0015fbd2: blx 0x00075acc
  0015fbd6: add.w r5,r6,#0x30
  0015fbda: ldmia r5,{r1,r2,r3,r5}
  0015fbdc: ldr.w r0,[r11,#0x0]
  0015fbe0: str r5,[sp,#0x0]
  0015fbe2: blx 0x00075ad8
  0015fbe6: ldr r0,[r6,#0x18]
  0015fbe8: cbz r0,0x0015fbfa
  0015fbea: blx 0x00075ae4
  0015fbee: ldr r0,[r6,#0x18]
  0015fbf0: cbz r0,0x0015fbfa
  0015fbf2: blx 0x00075af0
  0015fbf6: blx 0x0006eb48
  0015fbfa: ldr r0,[r6,#0x0]
  0015fbfc: movs r4,#0x0
  0015fbfe: str r4,[r6,#0x18]
  0015fc00: cbz r0,0x0015fc0a
  0015fc02: blx 0x00075afc
  0015fc06: blx 0x0006eb48
  0015fc0a: ldr r0,[r6,#0x4]
  0015fc0c: str r4,[r6,#0x0]
  0015fc0e: cbz r0,0x0015fc18
  0015fc10: blx 0x00075afc
  0015fc14: blx 0x0006eb48
  0015fc18: ldr r0,[r6,#0xc]
  0015fc1a: movs r4,#0x0
  0015fc1c: str r4,[r6,#0x4]
  0015fc1e: cbz r0,0x0015fc28
  0015fc20: blx 0x00074d88
  0015fc24: blx 0x0006eb48
  0015fc28: ldr r0,[r6,#0x24]
  0015fc2a: str r4,[r6,#0xc]
  0015fc2c: cbz r0,0x0015fc36
  0015fc2e: blx 0x00074d7c
  0015fc32: blx 0x0006eb48
  0015fc36: ldr r0,[r6,#0x28]
  0015fc38: movs r4,#0x0
  0015fc3a: str r4,[r6,#0x24]
  0015fc3c: cbz r0,0x0015fc46
  0015fc3e: blx 0x00074d7c
  0015fc42: blx 0x0006eb48
  0015fc46: ldr r0,[r6,#0x2c]
  0015fc48: str r4,[r6,#0x28]
  0015fc4a: cbz r0,0x0015fc54
  0015fc4c: blx 0x00074d7c
  0015fc50: blx 0x0006eb48
  0015fc54: movs r0,#0x0
  0015fc56: str.w r11,[sp,#0x40]
  0015fc5a: str r0,[r6,#0x8]
  0015fc5c: str r0,[r6,#0x2c]
  0015fc5e: str r0,[r6,#0x20]
  0015fc60: ldr.w r4,[r11,#0x0]
  0015fc64: ldr r5,[r6,#0x34]
  0015fc66: ldr r0,[r4,#0xc]
  0015fc68: str r0,[sp,#0x20]
  0015fc6a: ldr r0,[r4,#0x20]
  0015fc6c: str r0,[sp,#0x1c]
  0015fc6e: ldr r0,[r4,#0x2c]
  0015fc70: str r0,[sp,#0x24]
  0015fc72: ldr r0,[r4,#0x5c]
  0015fc74: str r0,[sp,#0x18]
  0015fc76: movs r0,#0x24
  0015fc78: blx 0x0006eb24
  0015fc7c: mov r11,r0
  0015fc7e: ldr r0,[r4,#0x10]
  0015fc80: str r0,[sp,#0x30]
  0015fc82: ldr r0,[r4,#0x24]
  0015fc84: str r0,[sp,#0x2c]
  0015fc86: ldrd r8,r10,[r4,#0x28]
  0015fc8a: ldr r0,[r6,#0x3c]
  0015fc8c: str r6,[sp,#0x34]
  0015fc8e: str r0,[sp,#0x28]
  0015fc90: ldr r4,[r6,#0x30]
  0015fc92: ldr.w r9,[r6,#0x38]
  0015fc96: ldr r0,[0x0015fdac]
  0015fc98: add r0,pc
  0015fc9a: ldr r0,[r0,#0x0]
  0015fc9c: str r0,[sp,#0x38]
  0015fc9e: ldr r0,[r0,#0x0]
  0015fca0: blx 0x00073c48
  0015fca4: ldr r2,[sp,#0x20]
  0015fca6: add.w r1,r8,r4
  0015fcaa: ldr r3,[sp,#0x1c]
  0015fcac: cmp r0,#0x0
  0015fcae: add r2,r5
  0015fcb0: mov.w r0,#0x0
  0015fcb4: add r2,r3
  0015fcb6: ldr r3,[sp,#0x18]
  0015fcb8: mov.w r12,#0x0
  0015fcbc: add r2,r3
  0015fcbe: ldr r3,[sp,#0x24]
  0015fcc0: add.w r4,r2,r3
  0015fcc4: add.w r2,r10,r8
  0015fcc8: rsb r3,r2,r9, asr #0x1
  0015fccc: ldr r2,[sp,#0x40]
  0015fcce: ldr r6,[r2,#0x0]
  0015fcd0: it eq
  0015fcd2: ldr.eq r0,[r6,#0x30]
  0015fcd4: ldr r6,[r6,#0x2c]
  0015fcd6: ldr r2,[sp,#0x28]
  0015fcd8: subs r5,r5,r4
  0015fcda: add r5,r2
  0015fcdc: ldr r2,[sp,#0x30]
  0015fcde: subs r5,r5,r2
  0015fce0: ldr r2,[sp,#0x2c]
  0015fce2: subs r5,r5,r2
  0015fce4: mov r2,r4
  0015fce6: subs r0,r5,r0
  0015fce8: sub.w r0,r0,r6, lsl #0x1
  0015fcec: strd r0,r12,[sp,#0x0]
  0015fcf0: mov r0,r11
  0015fcf2: str r4,[sp,#0x30]
  0015fcf4: blx 0x00075b20
  0015fcf8: ldr r0,[sp,#0x34]
  0015fcfa: ldr.w r9,[sp,#0x38]
  0015fcfe: str.w r11,[r0,#0x0]
  0015fd02: mov r11,r0
  0015fd04: ldr.w r0,[r9,#0x0]
  0015fd08: blx 0x00073c48
  0015fd0c: cmp r0,#0x0
  0015fd0e: beq 0x0015fdbc
  0015fd10: ldr r0,[0x0015fdb0]
  0015fd12: add r0,pc
  0015fd14: ldr r0,[r0,#0x0]
  0015fd16: ldrb.w r1,[r0,#0x35]
  0015fd1a: ldrb.w r0,[r0,#0x37]
  0015fd1e: orrs r0,r1
  0015fd20: lsls r0,r0,#0x18
  0015fd22: bne 0x0015fdbc
  0015fd24: ldr r0,[0x0015fdb4]
  0015fd26: add r0,pc
  0015fd28: ldr r0,[r0,#0x0]
  0015fd2a: ldr r0,[r0,#0x0]
  0015fd2c: blx 0x00071a1c
  0015fd30: cbz r0,0x0015fd44
  0015fd32: ldr.w r0,[r9,#0x0]
  0015fd36: blx 0x00071a58
  0015fd3a: blx 0x000719c8
  0015fd3e: cmp r0,#0x8
  0015fd40: bne.w 0x0015fe7e
  0015fd44: ldr.w r6,[r11,#0x0]
  0015fd48: add r0,sp,#0xa4
  0015fd4a: ldr r1,[0x0015fdb8]
  0015fd4c: movs r2,#0x0
  0015fd4e: add r1,pc
  0015fd50: blx 0x0006ee18
  0015fd54: ldr r0,[sp,#0x3c]
  0015fd56: ldr r0,[r0,#0x0]
  0015fd58: movw r1,#0x29e
  0015fd5c: blx 0x00072f70
  0015fd60: mov r1,r0
  0015fd62: add r0,sp,#0x98
  0015fd64: movs r2,#0x0
  0015fd66: blx 0x0006f028
  0015fd6a: add r1,sp,#0xa4
  0015fd6c: add r2,sp,#0x98
  0015fd6e: mov r0,r6
  0015fd70: blx 0x00075b2c
  0015fd74: add r0,sp,#0x98
  0015fd76: blx 0x0006ee30
  0015fd7a: add r0,sp,#0xa4
  0015fd7c: b 0x0015feb6
  0015fdbc: ldr r1,[0x0015ffec]
  0015fdbe: add r0,sp,#0x134
  0015fdc0: movs r2,#0x0
  0015fdc2: add r1,pc
  0015fdc4: blx 0x0006ee18
  0015fdc8: ldr.w r0,[r9,#0x0]
  0015fdcc: blx 0x00071770
  0015fdd0: cmp r0,#0xa3
  0015fdd2: bgt 0x0015fdf6
  0015fdd4: ldr r0,[sp,#0x3c]
  0015fdd6: ldr r5,[r0,#0x0]
  0015fdd8: ldr.w r0,[r9,#0x0]
  0015fddc: blx 0x00071770
  0015fde0: ldr r1,[0x0015fff0]
  0015fde2: add r1,pc
  0015fde4: ldr.w r1,[r1,r0,lsl #0x2]
  0015fde8: mov r0,r5
  0015fdea: blx 0x00072f70
  0015fdee: mov r1,r0
  0015fdf0: add r0,sp,#0x134
  0015fdf2: blx 0x0006f2b0
  0015fdf6: ldr.w r0,[r9,#0x0]
  0015fdfa: blx 0x0007285c
  0015fdfe: blx 0x00072874
  0015fe02: cmp r0,#0xa7
  0015fe04: beq 0x0015fe16
  0015fe06: ldr.w r0,[r9,#0x0]
  0015fe0a: blx 0x0007285c
  0015fe0e: blx 0x00072874
  0015fe12: cmp r0,#0xae
  0015fe14: bne 0x0015febc
  0015fe16: ldr.w r6,[r9,#0x0]
  0015fe1a: add r0,sp,#0x11c
  0015fe1c: add r1,sp,#0x134
  0015fe1e: movs r2,#0x0
  0015fe20: blx 0x0006f028
  0015fe24: ldr.w r0,[r9,#0x0]
  0015fe28: blx 0x0007285c
  0015fe2c: blx 0x00072bb0
  0015fe30: mov r5,r0
  0015fe32: ldr.w r0,[r9,#0x0]
  0015fe36: blx 0x0007285c
  0015fe3a: blx 0x0007288c
  0015fe3e: subs r1,r5,r0
  0015fe40: add r0,sp,#0x110
  0015fe42: blx 0x0006f658
  0015fe46: ldr r1,[0x0015fff4]
  0015fe48: add r1,pc
  0015fe4a: add r0,sp,#0x104
  0015fe4c: movs r2,#0x0
  0015fe4e: blx 0x0006ee18
  0015fe52: str r0,[sp,#0x0]
  0015fe54: add r0,sp,#0x128
  0015fe56: add r2,sp,#0x11c
  0015fe58: add r3,sp,#0x110
  0015fe5a: mov r1,r6
  0015fe5c: blx 0x000739b4
  0015fe60: add r0,sp,#0x134
  0015fe62: add r1,sp,#0x128
  0015fe64: blx 0x0006f2b0
  0015fe68: ldr r0,[0x0015fff8]
  0015fe6a: add r0,pc
  0015fe6c: ldr r4,[r0,#0x0]
  0015fe6e: add r0,sp,#0x128
  0015fe70: blx r4
  0015fe72: add r0,sp,#0x104
  0015fe74: blx r4
  0015fe76: add r0,sp,#0x110
  0015fe78: blx r4
  0015fe7a: add r0,sp,#0x11c
  0015fe7c: b 0x0015ff10
  0015fe7e: ldr.w r6,[r11,#0x0]
  0015fe82: add r0,sp,#0xbc
  0015fe84: ldr r1,[0x0015fffc]
  0015fe86: movs r2,#0x0
  0015fe88: add r1,pc
  0015fe8a: blx 0x0006ee18
  0015fe8e: ldr r0,[sp,#0x3c]
  0015fe90: ldr r0,[r0,#0x0]
  0015fe92: movw r1,#0x28a
  0015fe96: blx 0x00072f70
  0015fe9a: mov r1,r0
  0015fe9c: add r0,sp,#0xb0
  0015fe9e: movs r2,#0x0
  0015fea0: blx 0x0006f028
  0015fea4: add r1,sp,#0xbc
  0015fea6: add r2,sp,#0xb0
  0015fea8: mov r0,r6
  0015feaa: blx 0x00075b2c
  0015feae: add r0,sp,#0xb0
  0015feb0: blx 0x0006ee30
  0015feb4: add r0,sp,#0xbc
  0015feb6: blx 0x0006ee30
  0015feba: b 0x0015ff48
  0015febc: ldr.w r6,[r9,#0x0]
  0015fec0: add r0,sp,#0xf8
  0015fec2: add r1,sp,#0x134
  0015fec4: movs r2,#0x0
  0015fec6: blx 0x0006f028
  0015feca: ldr.w r0,[r9,#0x0]
  0015fece: blx 0x0007285c
  0015fed2: mov r1,r0
  0015fed4: add r0,sp,#0xec
  0015fed6: blx 0x00074b54
  0015feda: ldr r1,[0x00160000]
  0015fedc: add r1,pc
  0015fede: add r0,sp,#0xe0
  0015fee0: movs r2,#0x0
  0015fee2: blx 0x0006ee18
  0015fee6: str r0,[sp,#0x0]
  0015fee8: add r0,sp,#0x128
  0015feea: add r2,sp,#0xf8
  0015feec: add r3,sp,#0xec
  0015feee: mov r1,r6
  0015fef0: blx 0x000739b4
  0015fef4: add r0,sp,#0x134
  0015fef6: add r1,sp,#0x128
  0015fef8: blx 0x0006f2b0
  0015fefc: ldr r0,[0x00160004]
  0015fefe: add r0,pc
  0015ff00: ldr r4,[r0,#0x0]
  0015ff02: add r0,sp,#0x128
  0015ff04: blx r4
  0015ff06: add r0,sp,#0xe0
  0015ff08: blx r4
  0015ff0a: add r0,sp,#0xec
  0015ff0c: blx r4
  0015ff0e: add r0,sp,#0xf8
  0015ff10: blx r4
  0015ff12: ldr.w r6,[r11,#0x0]
  0015ff16: ldr r1,[0x00160008]
  0015ff18: add r1,pc
  0015ff1a: add r0,sp,#0xd4
  0015ff1c: movs r2,#0x0
  0015ff1e: blx 0x0006ee18
  0015ff22: add r0,sp,#0xc8
  0015ff24: add r1,sp,#0x134
  0015ff26: movs r2,#0x0
  0015ff28: blx 0x0006f028
  0015ff2c: add r1,sp,#0xd4
  0015ff2e: add r2,sp,#0xc8
  0015ff30: mov r0,r6
  0015ff32: blx 0x00075b2c
  0015ff36: ldr r0,[0x0016000c]
  0015ff38: add r0,pc
  0015ff3a: ldr r4,[r0,#0x0]
  0015ff3c: add r0,sp,#0xc8
  0015ff3e: blx r4
  0015ff40: add r0,sp,#0xd4
  0015ff42: blx r4
  0015ff44: add r0,sp,#0x134
  0015ff46: blx r4
  0015ff48: ldr.w r0,[r9,#0x0]
  0015ff4c: blx 0x00073444
  0015ff50: blx 0x00072868
  0015ff54: mov r5,r0
  0015ff56: movs r0,#0x24
  0015ff58: blx 0x0006eb24
  0015ff5c: mov r8,r0
  0015ff5e: ldr r0,[sp,#0x40]
  0015ff60: ldr.w r3,[r11,#0x38]
  0015ff64: cmp r5,#0x0
  0015ff66: ldr.w r1,[r11,#0x30]
  0015ff6a: ldr r2,[r0,#0x0]
  0015ff6c: add.w r1,r1,r3, asr #0x1
  0015ff70: asr.w r3,r3, asr #0x1
  0015ff74: ldr r0,[r2,#0x2c]
  0015ff76: add r1,r0
  0015ff78: beq 0x00160014
  0015ff7a: ldr r5,[r2,#0x10]
  0015ff7c: ldrd r9,r12,[r2,#0x24]
  0015ff80: ldr.w r6,[r11,#0x34]
  0015ff84: ldr.w r2,[r11,#0x3c]
  0015ff88: ldr.w r10,[sp,#0x30]
  0015ff8c: mov.w lr,#0x0
  0015ff90: add.w r4,r10,r0, lsl #0x1
  0015ff94: subs r0,r3,r0
  0015ff96: subs r4,r6,r4
  0015ff98: sub.w r3,r0,r12
  0015ff9c: add r2,r4
  0015ff9e: mov r0,r8
  0015ffa0: subs r2,r2,r5
  0015ffa2: sub.w r2,r2,r9
  0015ffa6: strd r2,lr,[sp,#0x0]
  0015ffaa: mov r2,r10
  0015ffac: blx 0x00075b20
  0015ffb0: ldr r1,[0x00160010]
  0015ffb2: add r0,sp,#0x8c
  0015ffb4: movs r2,#0x0
  0015ffb6: str.w r8,[r11,#0x4]
  0015ffba: add r1,pc
  0015ffbc: blx 0x0006ee18
  0015ffc0: ldr r0,[sp,#0x3c]
  0015ffc2: ldr r0,[r0,#0x0]
  0015ffc4: movs r1,#0xae
  0015ffc6: blx 0x00072f70
  0015ffca: mov r1,r0
  0015ffcc: add r0,sp,#0x80
  0015ffce: movs r2,#0x0
  0015ffd0: blx 0x0006f028
  0015ffd4: add r1,sp,#0x8c
  0015ffd6: add r2,sp,#0x80
  0015ffd8: mov r0,r8
  0015ffda: blx 0x00075b2c
  0015ffde: ldr r6,[sp,#0x38]
  0015ffe0: add r0,sp,#0x80
  0015ffe2: blx 0x0006ee30
  0015ffe6: add r0,sp,#0x8c
  0015ffe8: b 0x00160142
  00160014: ldr.w r10,[r2,#0x10]
  00160018: ldrd r9,r12,[r2,#0x24]
  0016001c: ldr.w lr,[r2,#0x30]
  00160020: ldr.w r4,[r11,#0x34]
  00160024: ldr.w r5,[r11,#0x3c]
  00160028: ldr.w r11,[r2,#0x4c]
  0016002c: ldr.w r2,[r2,#0x2d8]
  00160030: movs r6,#0x0
  00160032: str r6,[sp,#0x4]
  00160034: ldr r6,[sp,#0x30]
  00160036: add r6,r0
  00160038: subs r0,r3,r0
  0016003a: add r2,r6
  0016003c: sub.w r3,r0,r12
  00160040: subs r4,r4,r2
  00160042: mov r0,r8
  00160044: add r4,r5
  00160046: sub.w r4,r4,r10
  0016004a: sub.w r4,r4,r9
  0016004e: sub.w r4,r4,r11
  00160052: sub.w r6,r4,lr
  00160056: str r6,[sp,#0x0]
  00160058: blx 0x00075b20
  0016005c: ldr r0,[0x001603f4]
  0016005e: ldr.w r11,[sp,#0x34]
  00160062: add r0,pc
  00160064: ldr r6,[sp,#0x38]
  00160066: ldr r0,[r0,#0x0]
  00160068: str.w r8,[r11,#0x4]
  0016006c: ldr r5,[r0,#0x0]
  0016006e: ldr r0,[r6,#0x0]
  00160070: blx 0x00073444
  00160074: blx 0x00073438
  00160078: add r4,sp,#0x134
  0016007a: mov r2,r0
  0016007c: mov r1,r5
  0016007e: mov r0,r4
  00160080: blx 0x00077278
  00160084: ldr.w r8,[r6,#0x0]
  00160088: add r0,sp,#0x74
  0016008a: mov r1,r4
  0016008c: movs r2,#0x0
  0016008e: blx 0x0006f028
  00160092: ldr r0,[sp,#0x40]
  00160094: ldr r4,[r0,#0x0]
  00160096: ldr r0,[r6,#0x0]
  00160098: blx 0x00073444
  0016009c: blx 0x00071bc0
  001600a0: mov r5,r0
  001600a2: ldr r0,[r6,#0x0]
  001600a4: blx 0x00073444
  001600a8: blx 0x00074b78
  001600ac: adds r2,r0,r5
  001600ae: add r0,sp,#0x68
  001600b0: mov r1,r4
  001600b2: blx 0x00074e54
  001600b6: ldr r1,[0x001603f8]
  001600b8: add r1,pc
  001600ba: add r0,sp,#0x5c
  001600bc: movs r2,#0x0
  001600be: blx 0x0006ee18
  001600c2: str r0,[sp,#0x0]
  001600c4: add r0,sp,#0x128
  001600c6: add r2,sp,#0x74
  001600c8: add r3,sp,#0x68
  001600ca: mov r1,r8
  001600cc: blx 0x000739b4
  001600d0: add r0,sp,#0x134
  001600d2: add r1,sp,#0x128
  001600d4: blx 0x0006f2b0
  001600d8: ldr r0,[0x001603fc]
  001600da: add r0,pc
  001600dc: ldr r4,[r0,#0x0]
  001600de: add r0,sp,#0x128
  001600e0: blx r4
  001600e2: add r0,sp,#0x5c
  001600e4: blx r4
  001600e6: add r0,sp,#0x68
  001600e8: blx r4
  001600ea: add r0,sp,#0x74
  001600ec: blx r4
  001600ee: ldr.w r5,[r11,#0x4]
  001600f2: ldr r1,[0x00160400]
  001600f4: add r1,pc
  001600f6: add r0,sp,#0x50
  001600f8: movs r2,#0x0
  001600fa: blx 0x0006ee18
  001600fe: add r0,sp,#0x44
  00160100: add r1,sp,#0x134
  00160102: movs r2,#0x0
  00160104: blx 0x0006f028
  00160108: add r1,sp,#0x50
  0016010a: add r2,sp,#0x44
  0016010c: mov r0,r5
  0016010e: blx 0x00075b2c
  00160112: add r0,sp,#0x44
  00160114: blx 0x0006ee30
  00160118: add r0,sp,#0x50
  0016011a: blx 0x0006ee30
  0016011e: ldr r0,[0x00160404]
  00160120: add r0,pc
  00160122: ldr r0,[r0,#0x0]
  00160124: ldr r5,[r0,#0x0]
  00160126: ldr r0,[r6,#0x0]
  00160128: blx 0x00073444
  0016012c: blx 0x00073438
  00160130: blx 0x00071c74
  00160134: mov r1,r0
  00160136: mov r0,r5
  00160138: blx 0x00075b14
  0016013c: str.w r0,[r11,#0x18]
  00160140: add r0,sp,#0x134
  00160142: blx 0x0006ee30
  00160146: ldr r0,[sp,#0x40]
  00160148: ldr.w r4,[r11,#0x38]
  0016014c: ldr r0,[r0,#0x0]
  0016014e: ldr r5,[r0,#0x28]
  00160150: ldr r0,[r6,#0x0]
  00160152: blx 0x000723d0
  00160156: cmp r0,#0x0
  00160158: bne.w 0x001602a2
  0016015c: asrs r0,r4,#0x1
  0016015e: add.w r0,r0,r4, lsr #0x1f
  00160162: rsb r8,r5,r0, asr #0x1
  00160166: ldr r0,[r6,#0x0]
  00160168: blx 0x00073c48
  0016016c: cbnz r0,0x001601be
  0016016e: movs r0,#0xc8
  00160170: blx 0x0006eb24
  00160174: mov r9,r0
  00160176: ldr r0,[sp,#0x3c]
  00160178: ldr r0,[r0,#0x0]
  0016017a: mov.w r1,#0x1a8
  0016017e: blx 0x00072f70
  00160182: mov r12,r0
  00160184: ldr r0,[sp,#0x40]
  00160186: ldr r0,[r0,#0x0]
  00160188: ldrd r2,r3,[r11,#0x30]
  0016018c: ldr.w r4,[r11,#0x3c]
  00160190: ldr r6,[r0,#0x10]
  00160192: ldrd r1,r5,[r0,#0x24]
  00160196: ldr r0,[r0,#0x2c]
  00160198: add r3,r4
  0016019a: mov.w lr,#0x4
  0016019e: subs r3,r3,r6
  001601a0: mov.w r10,#0x21
  001601a4: subs r1,r3,r1
  001601a6: adds r3,r5,r2
  001601a8: subs r0,r1,r0
  001601aa: mov r1,r12
  001601ac: stm sp,{r0,r8,r10,lr}
  001601b0: mov r0,r9
  001601b2: movs r2,#0x0
  001601b4: blx 0x00074f50
  001601b8: ldr r6,[sp,#0x38]
  001601ba: str.w r9,[r11,#0x24]
  001601be: ldr r0,[r6,#0x0]
  001601c0: blx 0x00073444
  001601c4: blx 0x00072868
  001601c8: cmp r0,#0x0
  001601ca: bne 0x001602a2
  001601cc: movs r0,#0xc8
  001601ce: blx 0x0006eb24
  001601d2: mov r9,r0
  001601d4: ldr r0,[sp,#0x3c]
  001601d6: ldr r0,[r0,#0x0]
  001601d8: mov.w r1,#0x1a8
  001601dc: blx 0x00072f70
  001601e0: mov r12,r0
  001601e2: ldr r0,[sp,#0x40]
  001601e4: add.w r6,r11,#0x30
  001601e8: ldr r0,[r0,#0x0]
  001601ea: ldmia r6,{r2,r3,r6}
  001601ec: ldr r1,[r0,#0x10]
  001601ee: ldr r5,[r0,#0x24]
  001601f0: ldr r0,[r0,#0x2c]
  001601f2: ldr.w r4,[r11,#0x3c]
  001601f6: subs r3,r3,r0
  001601f8: mov.w lr,#0x4
  001601fc: add r3,r4
  001601fe: mov.w r10,#0x21
  00160202: subs r1,r3,r1
  00160204: subs r1,r1,r5
  00160206: stm sp,{r1,r8,r10,lr}
  0016020a: add.w r1,r2,r6, asr #0x1
  0016020e: adds r3,r1,r0
  00160210: mov r0,r9
  00160212: mov r1,r12
  00160214: movs r2,#0x0
  00160216: blx 0x00074f50
  0016021a: ldr r0,[0x00160408]
  0016021c: str.w r9,[r11,#0x28]
  00160220: add r0,pc
  00160222: ldr r0,[r0,#0x0]
  00160224: ldr r0,[r0,#0x0]
  00160226: blx 0x0006e848
  0016022a: cmp r0,#0x5
  0016022c: bne 0x00160290
  0016022e: movs r0,#0xc8
  00160230: blx 0x0006eb24
  00160234: mov r5,r0
  00160236: ldr r0,[sp,#0x3c]
  00160238: ldr r0,[r0,#0x0]
  0016023a: movw r1,#0x1a7
  0016023e: blx 0x00072f70
  00160242: mov r12,r0
  00160244: ldr r0,[sp,#0x40]
  00160246: ldr r0,[r0,#0x0]
  00160248: ldrd lr,r3,[r11,#0x30]
  0016024c: ldrd r6,r4,[r11,#0x38]
  00160250: ldr r1,[r0,#0x10]
  00160252: ldr r2,[r0,#0x24]
  00160254: ldr r0,[r0,#0x2c]
  00160256: subs r3,r3,r0
  00160258: mov.w r10,#0x21
  0016025c: add r3,r4
  0016025e: mov.w r9,#0x4
  00160262: subs r1,r3,r1
  00160264: subs r1,r1,r2
  00160266: movs r2,#0x0
  00160268: stm sp,{r1,r8,r10}
  0016026c: add.w r1,lr,r8
  00160270: add.w r1,r1,r6, asr #0x1
  00160274: add.w r3,r1,r0, lsl #0x1
  00160278: mov r0,r5
  0016027a: mov r1,r12
  0016027c: str.w r9,[sp,#0xc]
  00160280: blx 0x00074f50
  00160284: ldr r1,[0x001603f0]
  00160286: mov r0,r5
  00160288: str.w r5,[r11,#0x2c]
  0016028c: blx 0x00077284
  00160290: movs r0,#0x5c
  00160292: blx 0x0006eb24
  00160296: mov r5,r0
  00160298: blx 0x00074584
  0016029c: ldr r6,[sp,#0x38]
  0016029e: str.w r5,[r11,#0xc]
  001602a2: movs r0,#0x0
  001602a4: str.w r0,[r11,#0x40]
  001602a8: strb.w r0,[r11,#0x23]
  001602ac: ldr r0,[r6,#0x0]
  001602ae: blx 0x00077290
  001602b2: cbz r0,0x001602d0
  001602b4: ldr.w r0,[r11,#0x10]
  001602b8: cbz r0,0x001602c0
  001602ba: blx 0x00075a9c
  001602be: b 0x001602d0
  001602c0: movs r0,#0xb4
  001602c2: blx 0x0006eb24
  001602c6: mov r5,r0
  001602c8: blx 0x0007729c
  001602cc: str.w r5,[r11,#0x10]
  001602d0: ldr r0,[sp,#0x140]
  001602d2: ldr r1,[sp,#0x14]
  001602d4: ldr r1,[r1,#0x0]
  001602d6: subs r0,r1,r0
  001602d8: ittt eq
  001602da: add.eq sp,#0x144
  001602dc: pop.eq.w {r8,r9,r10,r11}
  001602e0: pop.eq {r4,r5,r6,r7,pc}
  001602e2: blx 0x0006e824
```
