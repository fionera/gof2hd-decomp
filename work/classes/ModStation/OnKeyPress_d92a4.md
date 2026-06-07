# ModStation::OnKeyPress
elf 0xd92a4 body 2234
Sig: undefined __stdcall OnKeyPress(longlong param_1, longlong param_2)

## decompile
```c

/* ModStation::OnKeyPress(long long, long long) */

void ModStation::OnKeyPress(longlong param_1,longlong param_2)

{
  bool bVar1;
  undefined1 uVar2;
  StatusWindow *this;
  undefined4 *puVar3;
  String *pSVar4;
  Station *pSVar5;
  DialogueWindow *pDVar6;
  String *pSVar7;
  Agent *this_00;
  Globals *this_01;
  Vector *pVVar8;
  void *pvVar9;
  MissionsWindow *this_02;
  SpaceLounge *this_03;
  String *this_04;
  HangarWindow *pHVar10;
  uint *puVar11;
  Engine *this_05;
  StarMap *this_06;
  FModSound *this_07;
  char cVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  undefined4 uVar16;
  int *piVar17;
  int iVar18;
  uint uVar19;
  int *piVar20;
  bool bVar21;
  float fVar22;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  float local_40;
  float local_3c;
  String aSStack_34 [12];
  int local_28;
  
  iVar18 = (int)param_1;
  piVar20 = *(int **)(DAT_000e9614 + 0xe92b6);
  local_28 = *piVar20;
  if (*(char *)(iVar18 + 0x24) == '\0') goto switchD_000e93b6_default;
  Status::checkForLevelUp();
  iVar15 = *(int *)(iVar18 + 0x34);
  bVar21 = 999 < *(uint *)(iVar18 + 0x30);
  if ((int)(-(uint)bVar21 - iVar15) < 0 == (SBORROW4(0,iVar15) != SBORROW4(-iVar15,(uint)bVar21))) {
    if (param_2 == 0x10000) {
      *(uint *)(iVar18 + 0x30) = 0x3e9;
      *(undefined4 *)(iVar18 + 0x34) = 0;
    }
    goto switchD_000e93b6_default;
  }
  if (((((*(char *)(iVar18 + 99) != '\0') || (*(char *)(iVar18 + 0x69) != '\0')) ||
       (*(char *)(iVar18 + 0x65) != '\0')) ||
      ((*(char *)(iVar18 + 0x66) != '\0' || (*(char *)(iVar18 + 0x67) != '\0')))) ||
     ((*(char *)(iVar18 + 100) != '\0' || (*(char *)(iVar18 + 0x68) != '\0'))))
  goto switchD_000e93b6_default;
  if (*(char *)(iVar18 + 0x6a) != '\0') {
    if (param_2 == 0x10000) {
      if (*piVar20 - local_28 == 0) {
        (*(code *)(DAT_001ac124 + 0x1ac128))();
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail(*piVar20 - local_28);
    }
    goto switchD_000e93b6_default;
  }
  cVar12 = *(char *)(iVar18 + 0x62);
  if (*(byte *)(iVar18 + 0x61) == 0) {
LAB_000e94b4:
    bVar21 = param_2 == 0x40000;
    if (param_2 == 0x40000) {
      bVar21 = cVar12 == '\0';
    }
    bVar1 = cVar12 == '\0';
    if (bVar21) {
      if (*piVar20 - local_28 == 0) {
        (*(code *)(DAT_001ac154 + 0x1ac158))();
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail(*piVar20 - local_28);
    }
  }
  else {
    if (cVar12 == '\0') {
      cVar12 = '\0';
      if (param_2 == 0x4000) {
        iVar14 = *(int *)(iVar18 + 0x48);
        *(undefined4 *)(iVar14 + *(int *)(iVar18 + 0x44) * 4) = 0;
        iVar15 = 4;
        if (0 < *(int *)(iVar18 + 0x44)) {
          iVar15 = *(int *)(iVar18 + 0x44) + -1;
        }
        *(int *)(iVar18 + 0x44) = iVar15;
        *(undefined4 *)(iVar14 + iVar15 * 4) = 1;
        goto switchD_000e93b6_default;
      }
      if (param_2 == 0x8000) {
        iVar15 = *(int *)(iVar18 + 0x48);
        iVar14 = 0;
        *(undefined4 *)(iVar15 + *(int *)(iVar18 + 0x44) * 4) = 0;
        if (*(int *)(iVar18 + 0x44) < 4) {
          iVar14 = *(int *)(iVar18 + 0x44) + 1;
        }
        *(int *)(iVar18 + 0x44) = iVar14;
        *(undefined4 *)(iVar15 + iVar14 * 4) = 1;
        goto switchD_000e93b6_default;
      }
      if (param_2 == 0x20000) goto switchD_000e93b6_default;
      if (param_2 != 0x10000) goto LAB_000e94b4;
      switch(*(undefined4 *)(iVar18 + 0x44)) {
      case 0:
        iVar15 = Status::getCurrentCampaignMission();
        if ((((iVar15 < 5) || (iVar15 = Status::getCurrentCampaignMission(), iVar15 == 0x30)) ||
            (iVar15 = Status::getCurrentCampaignMission(), iVar15 == 0x31)) ||
           (iVar15 = Status::getCurrentCampaignMission(), iVar15 == 0x38)) {
LAB_000e9742:
          pSVar4 = *(String **)(iVar18 + 0x70);
          puVar3 = (undefined4 *)(DAT_000e99e8 + 0xe9750);
LAB_000e974e:
          GameText::getText(*(int *)*puVar3);
          ChoiceWindow::set(pSVar4);
          *(undefined1 *)(iVar18 + 99) = 1;
          goto switchD_000e93b6_default;
        }
        pSVar5 = (Station *)Status::getStation();
        iVar15 = Station::getIndex(pSVar5);
        if (iVar15 == 4) {
          Status::getStation();
          iVar15 = Station::stationHasPirateBase();
          if (iVar15 != 0) {
            if (*(int *)(iVar18 + 0x84) != 0) goto switchD_000e93b6_default;
            pDVar6 = operator_new(0x74);
            piVar17 = *(int **)(DAT_000e961c + 0xe9438);
            pSVar4 = (String *)GameText::getText(*piVar17);
            pSVar7 = (String *)GameText::getText(*piVar17);
            DialogueWindow::DialogueWindow(pDVar6,pSVar4,pSVar7,(int *)(DAT_000e9620 + 0xe9456));
            *(undefined1 *)(iVar18 + 0x69) = 1;
            *(DialogueWindow **)(iVar18 + 0x84) = pDVar6;
            this_00 = operator_new(0x98);
            AbyssEngine::String::String(aSStack_34,(char *)(DAT_000e9624 + 0xe9474),false);
            Agent::Agent(this_00,0,aSStack_34,0,0,2,1,0,0,0,0);
            AbyssEngine::String::~String(aSStack_34);
            iVar15 = **(int **)(DAT_000e9628 + 0xe94a6);
            this_01 = (Globals *)**(undefined4 **)(DAT_000e962c + 0xe94a8);
            iVar18 = 0x1b0;
LAB_000e9714:
            pVVar8 = (Vector *)Globals::getDialogueSoundId(this_01,iVar18,this_00);
            FModSound::play(iVar15,pVVar8,(Vector *)0x0,extraout_s0);
            pvVar9 = (void *)Agent::~Agent(this_00);
            operator_delete(pvVar9);
            goto switchD_000e93b6_default;
          }
        }
        if (*(int *)(iVar18 + 0x78) == 0) {
          pHVar10 = operator_new(0x134);
          HangarWindow::HangarWindow(pHVar10);
          *(HangarWindow **)(iVar18 + 0x78) = pHVar10;
          HangarWindow::initialize();
        }
        if ((*(int *)(iVar18 + 0x74) == 0) ||
           (iVar15 = SpaceLounge::hangarNeedsUpdate(), iVar15 == 0)) {
          if ((*(int *)(iVar18 + 0x80) != 0) &&
             (iVar15 = MissionsWindow::hangarNeedsUpdate(), iVar15 != 0)) {
            if (*(HangarWindow **)(iVar18 + 0x78) != (HangarWindow *)0x0) {
              pvVar9 = (void *)HangarWindow::~HangarWindow(*(HangarWindow **)(iVar18 + 0x78));
              operator_delete(pvVar9);
            }
            *(undefined4 *)(iVar18 + 0x78) = 0;
            pHVar10 = operator_new(0x134);
            HangarWindow::HangarWindow(pHVar10);
            *(HangarWindow **)(iVar18 + 0x78) = pHVar10;
            HangarWindow::initialize();
            MissionsWindow::setHangarUpdate(SUB41(*(undefined4 *)(iVar18 + 0x80),0));
          }
        }
        else {
          if (*(HangarWindow **)(iVar18 + 0x78) != (HangarWindow *)0x0) {
            pvVar9 = (void *)HangarWindow::~HangarWindow(*(HangarWindow **)(iVar18 + 0x78));
            operator_delete(pvVar9);
          }
          *(undefined4 *)(iVar18 + 0x78) = 0;
          pHVar10 = operator_new(0x134);
          HangarWindow::HangarWindow(pHVar10);
          *(HangarWindow **)(iVar18 + 0x78) = pHVar10;
          HangarWindow::initialize();
          SpaceLounge::setHangarUpdate(SUB41(*(undefined4 *)(iVar18 + 0x74),0));
        }
        iVar14 = *(int *)(DAT_000e9c3c + 0xe9956);
        iVar15 = *(int *)(DAT_000e9c40 + 0xe995e);
        for (uVar19 = 0; uVar13 = **(uint **)(*(int *)(iVar18 + 0x78) + 4), uVar19 < uVar13;
            uVar19 = uVar19 + 1) {
          if (uVar19 < 10) {
            TouchButton::getPosition();
            *(int *)(iVar14 + uVar19 * 4) = (int)local_40;
            TouchButton::getPosition();
            *(int *)(iVar15 + uVar19 * 4) = (int)local_3c;
          }
        }
        **(uint **)(DAT_000e9c44 + 0xe99b8) = uVar13;
        *(undefined4 *)(iVar18 + 0xd0) = 0;
        Status::getShip();
        puVar11 = (uint *)Ship::getCargo();
        fVar22 = extraout_s0_02;
        if (puVar11 != (uint *)0x0) {
          for (uVar19 = 0; uVar19 < *puVar11; uVar19 = uVar19 + 1) {
            iVar15 = Item::getIndex(*(Item **)(puVar11[1] + uVar19 * 4));
            fVar22 = extraout_s0_03;
            if ((0x83 < iVar15) &&
               (iVar15 = Item::getIndex(*(Item **)(puVar11[1] + uVar19 * 4)),
               fVar22 = extraout_s0_04, iVar15 < 0x9a)) {
              iVar15 = Item::getAmount();
              *(int *)(iVar18 + 0xd0) = iVar15 + *(int *)(iVar18 + 0xd0);
              fVar22 = extraout_s0_05;
            }
          }
        }
        piVar17 = *(int **)(DAT_000e9c48 + 0xe9a54);
        FModSound::setParamValue((FModSound *)*piVar17,0,*(int *)*piVar17,fVar22);
        fVar22 = (float)FModSound::stop(*piVar17);
        FModSound::play(*piVar17,(Vector *)0x5f,(Vector *)0x0,fVar22);
        this_05 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
        uVar2 = AbyssEngine::Engine::IsPostEffectActivated(this_05);
        iVar15 = DAT_000e9c50;
        *(undefined1 *)(iVar18 + 300) = uVar2;
        *(undefined1 *)(iVar18 + 0x66) = 1;
        iVar18 = DAT_000e9c58;
        if (*(char *)(*(int *)(iVar15 + 0xe9a8e) + 8) != '\0') goto switchD_000e93b6_default;
        puVar3 = (undefined4 *)(DAT_000e9c54 + 0xe9aa2);
        *(undefined1 *)(*(int *)(iVar15 + 0xe9a8e) + 8) = 1;
        uVar16 = *(undefined4 *)*puVar3;
        pSVar4 = (String *)GameText::getText(**(int **)(iVar18 + 0xe9aa6));
        AbyssEngine::String::String(aSStack_4c,pSVar4,false);
        Layout::initHelpWindow(uVar16,aSStack_4c);
        this_04 = aSStack_4c;
        break;
      case 1:
        iVar15 = Status::getCurrentCampaignMission();
        if ((iVar15 < 0xd) || (iVar15 = Status::getCurrentCampaignMission(), iVar15 == 0x31))
        goto LAB_000e9742;
        pSVar5 = (Station *)Status::getStation();
        iVar15 = Station::getIndex(pSVar5);
        if (iVar15 == 0x58) {
          Status::getStation();
          iVar15 = Station::stationHasPirateBase();
          if (iVar15 != 0) {
            if (*(int *)(iVar18 + 0x84) != 0) goto switchD_000e93b6_default;
            pDVar6 = operator_new(0x74);
            piVar17 = *(int **)(DAT_000e99d4 + 0xe969c);
            pSVar4 = (String *)GameText::getText(*piVar17);
            pSVar7 = (String *)GameText::getText(*piVar17);
            DialogueWindow::DialogueWindow(pDVar6,pSVar4,pSVar7,(int *)(DAT_000e99d8 + 0xe96ba));
            *(undefined1 *)(iVar18 + 0x69) = 1;
            *(DialogueWindow **)(iVar18 + 0x84) = pDVar6;
            this_00 = operator_new(0x98);
            AbyssEngine::String::String(aSStack_58,(char *)(DAT_000e99dc + 0xe96d8),false);
            Agent::Agent(this_00,0,aSStack_58,0,0,0,1,0,0,0,0);
            AbyssEngine::String::~String(aSStack_58);
            iVar15 = **(int **)(DAT_000e99e0 + 0xe9708);
            this_01 = (Globals *)**(undefined4 **)(DAT_000e99e4 + 0xe970a);
            iVar18 = 0x1b1;
            goto LAB_000e9714;
          }
        }
        if (*(EVP_PKEY_CTX **)(iVar18 + 0x74) == (EVP_PKEY_CTX *)0x0) {
          this_03 = operator_new(0x10c);
          fVar22 = (float)SpaceLounge::SpaceLounge(this_03);
          *(SpaceLounge **)(iVar18 + 0x74) = this_03;
        }
        else {
          SpaceLounge::init(*(EVP_PKEY_CTX **)(iVar18 + 0x74));
          fVar22 = extraout_s0_01;
        }
        piVar17 = *(int **)(DAT_000e9a00 + 0xe9806);
        FModSound::setParamValue((FModSound *)*piVar17,0,*(int *)*piVar17,fVar22);
        fVar22 = (float)FModSound::stop(*piVar17);
        FModSound::play(*piVar17,(Vector *)0x6c,(Vector *)0x0,fVar22);
        iVar15 = DAT_000e9a04;
        *(undefined1 *)(iVar18 + 0x65) = 1;
        iVar18 = DAT_000e9a0c;
        if (*(char *)(*(int *)(iVar15 + 0xe9830) + 0xd) != '\0') goto switchD_000e93b6_default;
        puVar3 = (undefined4 *)(DAT_000e9a08 + 0xe9840);
        *(undefined1 *)(*(int *)(iVar15 + 0xe9830) + 0xd) = 1;
        uVar16 = *(undefined4 *)*puVar3;
        pSVar4 = (String *)GameText::getText(**(int **)(iVar18 + 0xe9844));
        AbyssEngine::String::String(aSStack_64,pSVar4,false);
        Layout::initHelpWindow(uVar16,aSStack_64);
        this_04 = aSStack_64;
        break;
      case 2:
        iVar15 = Status::getCurrentCampaignMission();
        if (((iVar15 < 9) || (iVar15 = Status::getCurrentCampaignMission(), iVar15 == 0x30)) ||
           (iVar15 = Status::getCurrentCampaignMission(), iVar15 == 0x31)) goto LAB_000e9742;
        Status::getShip();
        iVar15 = Ship::getCurrentLoad();
        Status::getShip();
        iVar14 = Ship::getMaxLoad();
        if (iVar14 < iVar15) {
          pSVar4 = *(String **)(iVar18 + 0x70);
          puVar3 = (undefined4 *)((int)&DAT_000e9614 + DAT_000e9648);
          goto LAB_000e974e;
        }
        iVar15 = Status::getCurrentCampaignMission();
        if (iVar15 == 0x4d) {
          Status::getShip();
          iVar15 = Ship::getIndex();
          if (iVar15 != 0x25) {
            pSVar4 = *(String **)(iVar18 + 0x70);
            puVar3 = (undefined4 *)(&UNK_000e9bda + DAT_000e9c5c);
            goto LAB_000e974e;
          }
        }
        if (*(EVP_PKEY_CTX **)(iVar18 + 0x10) == (EVP_PKEY_CTX *)0x0) {
          this_06 = operator_new(0x1e8);
          StarMap::StarMap(this_06,false,(Mission *)0x0,false,-1);
          *(StarMap **)(iVar18 + 0x10) = this_06;
        }
        else {
          StarMap::init(*(EVP_PKEY_CTX **)(iVar18 + 0x10));
        }
        iVar15 = Status::getShip();
        iVar15 = Ship::hasEquipment(iVar15,0x55);
        if (iVar15 == 0) {
          Status::getShip();
          iVar15 = Ship::hasJumpDriveIntegrated();
          if (iVar15 != 0) goto LAB_000e9b0c;
        }
        else {
LAB_000e9b0c:
          StarMap::setJumpMapMode(SUB41(*(undefined4 *)(iVar18 + 0x10),0),false);
        }
        piVar17 = *(int **)(DAT_000e9c60 + 0xe9b20);
        *(undefined1 *)(iVar18 + 0x67) = 1;
        fVar22 = (float)FModSound::stop(*piVar17);
        this_07 = (FModSound *)*piVar17;
        FModSound::setParamValue(this_07,0,*(int *)this_07,fVar22);
        iVar18 = *(int *)(DAT_000e9c64 + 0xe9b3e);
        if ((*(char *)(iVar18 + 0xe) == '\0') &&
           (iVar15 = Status::getCurrentCampaignMission(), 0xf < iVar15)) {
          uVar16 = **(undefined4 **)(DAT_000e9c68 + 0xe9b56);
          iVar15 = **(int **)(DAT_000e9c6c + 0xe9b58);
          *(undefined1 *)(iVar18 + 0xe) = 1;
          pSVar4 = (String *)GameText::getText(iVar15);
          AbyssEngine::String::String(aSStack_70,pSVar4,false);
          Layout::initHelpWindow(uVar16,aSStack_70);
          this_04 = aSStack_70;
        }
        else {
          if ((*(char *)(iVar18 + 0xf) != '\0') ||
             (iVar15 = Status::getCurrentCampaignMission(), 0xf < iVar15))
          goto switchD_000e93b6_default;
          uVar16 = **(undefined4 **)(DAT_000e9c70 + 0xe9ba0);
          iVar15 = **(int **)(DAT_000e9c74 + 0xe9ba2);
          *(undefined1 *)(iVar18 + 0xf) = 1;
          pSVar4 = (String *)GameText::getText(iVar15);
          AbyssEngine::String::String(aSStack_7c,pSVar4,false);
          Layout::initHelpWindow(uVar16,aSStack_7c);
          this_04 = aSStack_7c;
        }
        break;
      case 3:
        iVar15 = Status::getCurrentCampaignMission();
        if (iVar15 < 9) goto LAB_000e9742;
        if (*(EVP_PKEY_CTX **)(iVar18 + 0x80) == (EVP_PKEY_CTX *)0x0) {
          this_02 = operator_new(0x44);
          fVar22 = (float)MissionsWindow::MissionsWindow(this_02);
          *(MissionsWindow **)(iVar18 + 0x80) = this_02;
        }
        else {
          MissionsWindow::init(*(EVP_PKEY_CTX **)(iVar18 + 0x80));
          fVar22 = extraout_s0_00;
        }
        iVar15 = DAT_000e99ec;
        *(undefined1 *)(iVar18 + 100) = 1;
        FModSound::setParamValue
                  ((FModSound *)**(undefined4 **)(iVar15 + 0xe979c),0,
                   *(int *)**(undefined4 **)(iVar15 + 0xe979c),fVar22);
        iVar18 = DAT_000e99fc;
        if (*(char *)(*(int *)(DAT_000e99f4 + 0xe97ac) + 0x13) != '\0')
        goto switchD_000e93b6_default;
        puVar3 = (undefined4 *)(DAT_000e99f8 + 0xe97bc);
        *(undefined1 *)(*(int *)(DAT_000e99f4 + 0xe97ac) + 0x13) = 1;
        uVar16 = *(undefined4 *)*puVar3;
        pSVar4 = (String *)GameText::getText(**(int **)(iVar18 + 0xe97c0));
        AbyssEngine::String::String(aSStack_88,pSVar4,false);
        Layout::initHelpWindow(uVar16,aSStack_88);
        this_04 = aSStack_88;
        break;
      case 4:
        if (*(int *)(iVar18 + 0x7c) == 0) {
          this = operator_new(0x7c);
          StatusWindow::StatusWindow(this);
          *(StatusWindow **)(iVar18 + 0x7c) = this;
        }
        fVar22 = (float)StatusWindow::reInit();
        iVar15 = DAT_000e9630;
        *(undefined1 *)(iVar18 + 0x68) = 1;
        FModSound::setParamValue
                  ((FModSound *)**(undefined4 **)(iVar15 + 0xe956c),0,
                   *(int *)**(undefined4 **)(iVar15 + 0xe956c),fVar22);
        iVar18 = DAT_000e9644;
        if (**(char **)(DAT_000e9638 + 0xe957e) != '\0' ||
            *(char *)(*(int *)(DAT_000e963c + 0xe9580) + 0x18) != '\0')
        goto switchD_000e93b6_default;
        puVar3 = (undefined4 *)(DAT_000e9640 + 0xe9594);
        *(undefined1 *)(*(int *)(DAT_000e963c + 0xe9580) + 0x18) = 1;
        uVar16 = *(undefined4 *)*puVar3;
        pSVar4 = (String *)GameText::getText(**(int **)(iVar18 + 0xe9598));
        AbyssEngine::String::String(aSStack_94,pSVar4,false);
        Layout::initHelpWindow(uVar16,aSStack_94);
        this_04 = aSStack_94;
        break;
      default:
        goto switchD_000e93b6_default;
      }
      AbyssEngine::String::~String(this_04);
      goto switchD_000e93b6_default;
    }
    bVar1 = false;
  }
  if ((param_2 == 0x20000) && (bVar1)) {
    *(byte *)(iVar18 + 0x61) = *(byte *)(iVar18 + 0x61) ^ 1;
  }
switchD_000e93b6_default:
  if (*piVar20 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000e92a4: push {r4,r5,r6,r7,lr}
  000e92a6: add r7,sp,#0xc
  000e92a8: push {r8,r9,r10,r11}
  000e92ac: sub sp,#0x8c
  000e92ae: mov r10,r0
  000e92b0: ldr r0,[0x000e9614]
  000e92b2: add r0,pc
  000e92b4: ldr.w r8,[r0,#0x0]
  000e92b8: ldr.w r0,[r8,#0x0]
  000e92bc: str r0,[sp,#0x88]
  000e92be: ldrb.w r0,[r10,#0x24]
  000e92c2: cbz r0,0x000e932c
  000e92c4: ldr r0,[0x000e9618]
  000e92c6: mov r5,r3
  000e92c8: mov r6,r2
  000e92ca: add r0,pc
  000e92cc: ldr.w r9,[r0,#0x0]
  000e92d0: ldr.w r0,[r9,#0x0]
  000e92d4: blx 0x00075274
  000e92d8: mov r0,r10
  000e92da: movw r4,#0x3e7
  000e92de: ldr.w r1,[r0,#0x30]!
  000e92e2: movs r2,#0x0
  000e92e4: ldr r3,[r0,#0x4]
  000e92e6: subs r1,r4,r1
  000e92e8: sbcs.w r1,r2,r3
  000e92ec: blt 0x000e9302
  000e92ee: eor r1,r6,#0x10000
  000e92f2: orrs r1,r5
  000e92f4: ittt eq
  000e92f6: mov.eq r1,#0x0
  000e92f8: movw.eq r2,#0x3e9
  000e92fc: strd.eq r2,r1,[r0,#0x0]
  000e9300: b 0x000e932c
  000e9302: ldrb.w r0,[r10,#0x63]
  000e9306: cbnz r0,0x000e932c
  000e9308: ldrb.w r0,[r10,#0x69]
  000e930c: cbnz r0,0x000e932c
  000e930e: ldrb.w r0,[r10,#0x65]
  000e9312: cbnz r0,0x000e932c
  000e9314: ldrb.w r0,[r10,#0x66]
  000e9318: cbnz r0,0x000e932c
  000e931a: ldrb.w r0,[r10,#0x67]
  000e931e: cbnz r0,0x000e932c
  000e9320: ldrb.w r0,[r10,#0x64]
  000e9324: cbnz r0,0x000e932c
  000e9326: ldrb.w r0,[r10,#0x68]
  000e932a: cbz r0,0x000e9342
  000e932c: ldr r0,[sp,#0x88]
  000e932e: ldr.w r1,[r8,#0x0]
  000e9332: subs r0,r1,r0
  000e9334: ittt eq
  000e9336: add.eq sp,#0x8c
  000e9338: pop.eq.w {r8,r9,r10,r11}
  000e933c: pop.eq {r4,r5,r6,r7,pc}
  000e933e: blx 0x0006e824
  000e9342: ldrb.w r0,[r10,#0x6a]
  000e9346: cbz r0,0x000e9370
  000e9348: eor r0,r6,#0x10000
  000e934c: orrs r0,r5
  000e934e: bne 0x000e932c
  000e9350: ldr r0,[sp,#0x88]
  000e9352: ldr.w r1,[r8,#0x0]
  000e9356: subs r0,r1,r0
  000e9358: itttt eq
  000e935a: mov.eq r0,r10
  000e935c: add.eq sp,#0x8c
  000e935e: pop.eq.w {r8,r9,r10,r11}
  000e9362: pop.eq.w {r4,r5,r6,r7,lr}
  000e9366: it eq
  000e9368: b.eq.w 0x001ac118
  000e936c: blx 0x0006e824
  000e9370: ldrb.w r0,[r10,#0x61]
  000e9374: ldrb.w r1,[r10,#0x62]
  000e9378: cmp r0,#0x0
  000e937a: beq.w 0x000e94b4
  000e937e: cbz r1,0x000e9384
  000e9380: movs r2,#0x0
  000e9382: b 0x000e94c8
  000e9384: eor r1,r6,#0x4000
  000e9388: orrs r1,r5
  000e938a: mov.w r1,#0x0
  000e938e: beq.w 0x000e9522
  000e9392: eor r2,r6,#0x8000
  000e9396: orrs r2,r5
  000e9398: beq.w 0x000e9502
  000e939c: eor r2,r6,#0x20000
  000e93a0: orrs r2,r5
  000e93a2: beq 0x000e932c
  000e93a4: eor r2,r6,#0x10000
  000e93a8: orrs r2,r5
  000e93aa: bne.w 0x000e94b4
  000e93ae: ldr.w r0,[r10,#0x44]
  000e93b2: cmp r0,#0x4
  000e93b4: bhi 0x000e932c
  000e93b6: tbh [pc,r0]
  000e93c4: ldr.w r0,[r9,#0x0]
  000e93c8: blx 0x00071770
  000e93cc: cmp r0,#0x5
  000e93ce: blt.w 0x000e9742
  000e93d2: ldr.w r0,[r9,#0x0]
  000e93d6: blx 0x00071770
  000e93da: cmp r0,#0x30
  000e93dc: beq.w 0x000e9742
  000e93e0: ldr.w r0,[r9,#0x0]
  000e93e4: blx 0x00071770
  000e93e8: cmp r0,#0x31
  000e93ea: beq.w 0x000e9742
  000e93ee: ldr.w r0,[r9,#0x0]
  000e93f2: blx 0x00071770
  000e93f6: cmp r0,#0x38
  000e93f8: beq.w 0x000e9742
  000e93fc: ldr.w r0,[r9,#0x0]
  000e9400: blx 0x00071c14
  000e9404: blx 0x00071824
  000e9408: cmp r0,#0x4
  000e940a: bne.w 0x000e98a8
  000e940e: ldr.w r0,[r9,#0x0]
  000e9412: blx 0x00071c14
  000e9416: blx 0x00073f00
  000e941a: cmp r0,#0x0
  000e941c: beq.w 0x000e98a8
  000e9420: ldr.w r0,[r10,#0x84]
  000e9424: cmp r0,#0x0
  000e9426: bne.w 0x000e932c
  000e942a: movs r0,#0x74
  000e942c: blx 0x0006eb24
  000e9430: mov r9,r0
  000e9432: ldr r0,[0x000e961c]
  000e9434: add r0,pc
  000e9436: ldr r5,[r0,#0x0]
  000e9438: ldr r0,[r5,#0x0]
  000e943a: mov.w r1,#0x1b0
  000e943e: blx 0x00072f70
  000e9442: mov r6,r0
  000e9444: ldr r0,[r5,#0x0]
  000e9446: movw r1,#0x649
  000e944a: blx 0x00072f70
  000e944e: ldr r3,[0x000e9620]
  000e9450: mov r2,r0
  000e9452: add r3,pc
  000e9454: mov r0,r9
  000e9456: mov r1,r6
  000e9458: blx 0x00074f20
  000e945c: movs r6,#0x1
  000e945e: movs r0,#0x98
  000e9460: strb.w r6,[r10,#0x69]
  000e9464: str.w r9,[r10,#0x84]
  000e9468: blx 0x0006eb24
  000e946c: ldr r1,[0x000e9624]
  000e946e: mov r5,r0
  000e9470: add r1,pc
  000e9472: add r0,sp,#0x7c
  000e9474: movs r2,#0x0
  000e9476: movs r4,#0x0
  000e9478: blx 0x0006ee18
  000e947c: movs r0,#0x2
  000e947e: add r2,sp,#0x7c
  000e9480: strd r4,r0,[sp,#0x0]
  000e9484: mov r0,r5
  000e9486: movs r1,#0x0
  000e9488: movs r3,#0x0
  000e948a: strd r6,r4,[sp,#0x8]
  000e948e: strd r4,r4,[sp,#0x10]
  000e9492: str r4,[sp,#0x18]
  000e9494: blx 0x00071b30
  000e9498: add r0,sp,#0x7c
  000e949a: blx 0x0006ee30
  000e949e: ldr r0,[0x000e9628]
  000e94a0: ldr r1,[0x000e962c]
  000e94a2: add r0,pc
  000e94a4: add r1,pc
  000e94a6: ldr r0,[r0,#0x0]
  000e94a8: ldr r1,[r1,#0x0]
  000e94aa: ldr r4,[r0,#0x0]
  000e94ac: ldr r0,[r1,#0x0]
  000e94ae: mov.w r1,#0x1b0
  000e94b2: b 0x000e9714
  000e94b4: eor r2,r6,#0x40000
  000e94b8: orrs r2,r5
  000e94ba: clz r2,r1
  000e94be: it eq
  000e94c0: cmp.eq r1,#0x0
  000e94c2: lsr.w r2,r2,#0x5
  000e94c6: beq 0x000e94e2
  000e94c8: eor r1,r6,#0x20000
  000e94cc: orrs r1,r5
  000e94ce: bne.w 0x000e932c
  000e94d2: cmp r2,#0x0
  000e94d4: beq.w 0x000e932c
  000e94d8: eor r0,r0,#0x1
  000e94dc: strb.w r0,[r10,#0x61]
  000e94e0: b 0x000e932c
  000e94e2: ldr r0,[sp,#0x88]
  000e94e4: ldr.w r1,[r8,#0x0]
  000e94e8: subs r0,r1,r0
  000e94ea: itttt eq
  000e94ec: mov.eq r0,r10
  000e94ee: add.eq sp,#0x8c
  000e94f0: pop.eq.w {r8,r9,r10,r11}
  000e94f4: pop.eq.w {r4,r5,r6,r7,lr}
  000e94f8: it eq
  000e94fa: b.eq.w 0x001ac148
  000e94fe: blx 0x0006e824
  000e9502: ldrd r0,r1,[r10,#0x44]
  000e9506: movs r2,#0x0
  000e9508: str.w r2,[r1,r0,lsl #0x2]
  000e950c: ldr.w r0,[r10,#0x44]
  000e9510: cmp r0,#0x4
  000e9512: it lt
  000e9514: add.lt r2,r0,#0x1
  000e9516: movs r0,#0x1
  000e9518: str.w r2,[r10,#0x44]
  000e951c: str.w r0,[r1,r2,lsl #0x2]
  000e9520: b 0x000e932c
  000e9522: ldrd r0,r2,[r10,#0x44]
  000e9526: str.w r1,[r2,r0,lsl #0x2]
  000e952a: movs r1,#0x4
  000e952c: ldr.w r0,[r10,#0x44]
  000e9530: cmp r0,#0x0
  000e9532: it gt
  000e9534: sub.gt r1,r0,#0x1
  000e9536: movs r0,#0x1
  000e9538: str.w r1,[r10,#0x44]
  000e953c: str.w r0,[r2,r1,lsl #0x2]
  000e9540: b 0x000e932c
  000e9542: ldr.w r5,[r10,#0x7c]
  000e9546: cbnz r5,0x000e9558
  000e9548: movs r0,#0x7c
  000e954a: blx 0x0006eb24
  000e954e: mov r5,r0
  000e9550: blx 0x000753b8
  000e9554: str.w r5,[r10,#0x7c]
  000e9558: mov r0,r5
  000e955a: blx 0x000753c4
  000e955e: ldr r0,[0x000e9630]
  000e9560: movs r5,#0x1
  000e9562: strb.w r5,[r10,#0x68]
  000e9566: movs r1,#0x0
  000e9568: add r0,pc
  000e956a: ldr r3,[0x000e9634]
  000e956c: ldr r0,[r0,#0x0]
  000e956e: ldr r0,[r0,#0x0]
  000e9570: ldr r2,[r0,#0x0]
  000e9572: blx 0x0007159c
  000e9576: ldr r0,[0x000e9638]
  000e9578: ldr r1,[0x000e963c]
  000e957a: add r0,pc
  000e957c: add r1,pc
  000e957e: ldr r0,[r0,#0x0]
  000e9580: ldr r1,[r1,#0x0]
  000e9582: ldrb r0,[r0,#0x0]
  000e9584: ldrb r2,[r1,#0x18]
  000e9586: orrs r0,r2
  000e9588: bne.w 0x000e932c
  000e958c: ldr r0,[0x000e9640]
  000e958e: ldr r2,[0x000e9644]
  000e9590: add r0,pc
  000e9592: strb r5,[r1,#0x18]
  000e9594: add r2,pc
  000e9596: mov.w r1,#0x280
  000e959a: ldr r0,[r0,#0x0]
  000e959c: ldr r2,[r2,#0x0]
  000e959e: ldr r4,[r0,#0x0]
  000e95a0: ldr r0,[r2,#0x0]
  000e95a2: blx 0x00072f70
  000e95a6: add r5,sp,#0x1c
  000e95a8: mov r1,r0
  000e95aa: movs r2,#0x0
  000e95ac: mov r0,r5
  000e95ae: blx 0x0006f028
  000e95b2: mov r0,r4
  000e95b4: mov r1,r5
  000e95b6: blx 0x000747c4
  000e95ba: add r0,sp,#0x1c
  000e95bc: b 0x000e9868
  000e95be: ldr.w r0,[r9,#0x0]
  000e95c2: blx 0x00071770
  000e95c6: cmp r0,#0x9
  000e95c8: blt.w 0x000e9742
  000e95cc: ldr.w r0,[r9,#0x0]
  000e95d0: blx 0x00071770
  000e95d4: cmp r0,#0x30
  000e95d6: beq.w 0x000e9742
  000e95da: ldr.w r0,[r9,#0x0]
  000e95de: blx 0x00071770
  000e95e2: cmp r0,#0x31
  000e95e4: beq.w 0x000e9742
  000e95e8: ldr.w r0,[r9,#0x0]
  000e95ec: blx 0x00071a58
  000e95f0: blx 0x00072bf8
  000e95f4: mov r5,r0
  000e95f6: ldr.w r0,[r9,#0x0]
  000e95fa: blx 0x00071a58
  000e95fe: blx 0x00072c04
  000e9602: cmp r5,r0
  000e9604: ble.w 0x000e986e
  000e9608: ldr r0,[0x000e9648]
  000e960a: movs r1,#0xcc
  000e960c: ldr.w r5,[r10,#0x70]
  000e9610: add r0,pc
  000e9612: b 0x000e974e
  000e964c: ldr.w r0,[r9,#0x0]
  000e9650: blx 0x00071770
  000e9654: cmp r0,#0xd
  000e9656: blt 0x000e9742
  000e9658: ldr.w r0,[r9,#0x0]
  000e965c: blx 0x00071770
  000e9660: cmp r0,#0x31
  000e9662: beq 0x000e9742
  000e9664: ldr.w r0,[r9,#0x0]
  000e9668: blx 0x00071c14
  000e966c: blx 0x00071824
  000e9670: cmp r0,#0x58
  000e9672: bne 0x000e9772
  000e9674: ldr.w r0,[r9,#0x0]
  000e9678: blx 0x00071c14
  000e967c: blx 0x00073f00
  000e9680: cmp r0,#0x0
  000e9682: beq 0x000e9772
  000e9684: ldr.w r0,[r10,#0x84]
  000e9688: cmp r0,#0x0
  000e968a: bne.w 0x000e932c
  000e968e: movs r0,#0x74
  000e9690: blx 0x0006eb24
  000e9694: mov r9,r0
  000e9696: ldr r0,[0x000e99d4]
  000e9698: add r0,pc
  000e969a: ldr r5,[r0,#0x0]
  000e969c: ldr r0,[r5,#0x0]
  000e969e: movw r1,#0x1b1
  000e96a2: blx 0x00072f70
  000e96a6: mov r6,r0
  000e96a8: ldr r0,[r5,#0x0]
  000e96aa: movw r1,#0x63d
  000e96ae: blx 0x00072f70
  000e96b2: ldr r3,[0x000e99d8]
  000e96b4: mov r2,r0
  000e96b6: add r3,pc
  000e96b8: mov r0,r9
  000e96ba: mov r1,r6
  000e96bc: blx 0x00074f20
  000e96c0: movs r6,#0x1
  000e96c2: movs r0,#0x98
  000e96c4: strb.w r6,[r10,#0x69]
  000e96c8: str.w r9,[r10,#0x84]
  000e96cc: blx 0x0006eb24
  000e96d0: ldr r1,[0x000e99dc]
  000e96d2: mov r5,r0
  000e96d4: add r1,pc
  000e96d6: add r0,sp,#0x58
  000e96d8: movs r2,#0x0
  000e96da: movs r4,#0x0
  000e96dc: blx 0x0006ee18
  000e96e0: add r2,sp,#0x58
  000e96e2: mov r0,r5
  000e96e4: movs r1,#0x0
  000e96e6: movs r3,#0x0
  000e96e8: strd r4,r4,[sp,#0x0]
  000e96ec: strd r6,r4,[sp,#0x8]
  000e96f0: strd r4,r4,[sp,#0x10]
  000e96f4: str r4,[sp,#0x18]
  000e96f6: blx 0x00071b30
  000e96fa: add r0,sp,#0x58
  000e96fc: blx 0x0006ee30
  000e9700: ldr r0,[0x000e99e0]
  000e9702: ldr r1,[0x000e99e4]
  000e9704: add r0,pc
  000e9706: add r1,pc
  000e9708: ldr r0,[r0,#0x0]
  000e970a: ldr r1,[r1,#0x0]
  000e970c: ldr r4,[r0,#0x0]
  000e970e: ldr r0,[r1,#0x0]
  000e9710: movw r1,#0x1b1
  000e9714: mov r2,r5
  000e9716: blx 0x00074f2c
  000e971a: mov r1,r0
  000e971c: movs r0,#0x0
  000e971e: str r0,[sp,#0x0]
  000e9720: mov r0,r4
  000e9722: movs r2,#0x0
  000e9724: movs r3,#0x0
  000e9726: blx 0x00071548
  000e972a: mov r0,r5
  000e972c: blx 0x00073450
  000e9730: blx 0x0006eb48
  000e9734: b 0x000e932c
  000e9736: ldr.w r0,[r9,#0x0]
  000e973a: blx 0x00071770
  000e973e: cmp r0,#0x8
  000e9740: bgt 0x000e9766
  000e9742: ldr r0,[0x000e99e8]
  000e9744: mov.w r1,#0x210
  000e9748: ldr.w r5,[r10,#0x70]
  000e974c: add r0,pc
  000e974e: ldr r0,[r0,#0x0]
  000e9750: ldr r0,[r0,#0x0]
  000e9752: blx 0x00072f70
  000e9756: mov r1,r0
  000e9758: mov r0,r5
  000e975a: blx 0x00074794
  000e975e: movs r0,#0x1
  000e9760: strb.w r0,[r10,#0x63]
  000e9764: b 0x000e932c
  000e9766: ldr.w r0,[r10,#0x80]
  000e976a: cbz r0,0x000e977e
  000e976c: blx 0x000753d0
  000e9770: b 0x000e978e
  000e9772: ldr.w r0,[r10,#0x74]
  000e9776: cbz r0,0x000e97e6
  000e9778: blx 0x000753dc
  000e977c: b 0x000e97f8
  000e977e: movs r0,#0x44
  000e9780: blx 0x0006eb24
  000e9784: mov r5,r0
  000e9786: blx 0x000753e8
  000e978a: str.w r5,[r10,#0x80]
  000e978e: ldr r0,[0x000e99ec]
  000e9790: movs r5,#0x1
  000e9792: strb.w r5,[r10,#0x64]
  000e9796: movs r1,#0x0
  000e9798: add r0,pc
  000e979a: ldr r3,[0x000e99f0]
  000e979c: ldr r0,[r0,#0x0]
  000e979e: ldr r0,[r0,#0x0]
  000e97a0: ldr r2,[r0,#0x0]
  000e97a2: blx 0x0007159c
  000e97a6: ldr r0,[0x000e99f4]
  000e97a8: add r0,pc
  000e97aa: ldr r1,[r0,#0x0]
  000e97ac: ldrb r0,[r1,#0x13]
  000e97ae: cmp r0,#0x0
  000e97b0: bne.w 0x000e932c
  000e97b4: ldr r0,[0x000e99f8]
  000e97b6: ldr r2,[0x000e99fc]
  000e97b8: add r0,pc
  000e97ba: strb r5,[r1,#0x13]
  000e97bc: add r2,pc
  000e97be: movw r1,#0x27b
  000e97c2: ldr r0,[r0,#0x0]
  000e97c4: ldr r2,[r2,#0x0]
  000e97c6: ldr r4,[r0,#0x0]
  000e97c8: ldr r0,[r2,#0x0]
  000e97ca: blx 0x00072f70
  000e97ce: add r5,sp,#0x28
  000e97d0: mov r1,r0
  000e97d2: movs r2,#0x0
  000e97d4: mov r0,r5
  000e97d6: blx 0x0006f028
  000e97da: mov r0,r4
  000e97dc: mov r1,r5
  000e97de: blx 0x000747c4
  000e97e2: add r0,sp,#0x28
  000e97e4: b 0x000e9868
  000e97e6: mov.w r0,#0x10c
  000e97ea: blx 0x0006eb24
  000e97ee: mov r5,r0
  000e97f0: blx 0x000753f4
  000e97f4: str.w r5,[r10,#0x74]
  000e97f8: ldr r0,[0x000e9a00]
  000e97fa: movs r1,#0x0
  000e97fc: mov.w r3,#0x40000000
  000e9800: movs r5,#0x0
  000e9802: add r0,pc
  000e9804: ldr r4,[r0,#0x0]
  000e9806: ldr r0,[r4,#0x0]
  000e9808: ldr r2,[r0,#0x0]
  000e980a: blx 0x0007159c
  000e980e: ldr r0,[r4,#0x0]
  000e9810: movs r1,#0x7a
  000e9812: blx 0x000724a8
  000e9816: ldr r0,[r4,#0x0]
  000e9818: movs r1,#0x6c
  000e981a: movs r2,#0x0
  000e981c: movs r3,#0x0
  000e981e: str r5,[sp,#0x0]
  000e9820: blx 0x00071548
  000e9824: ldr r0,[0x000e9a04]
  000e9826: movs r2,#0x1
  000e9828: strb.w r2,[r10,#0x65]
  000e982c: add r0,pc
  000e982e: ldr r1,[r0,#0x0]
  000e9830: ldrb r0,[r1,#0xd]
  000e9832: cmp r0,#0x0
  000e9834: bne.w 0x000e932c
  000e9838: ldr r0,[0x000e9a08]
  000e983a: ldr r3,[0x000e9a0c]
  000e983c: add r0,pc
  000e983e: strb r2,[r1,#0xd]
  000e9840: add r3,pc
  000e9842: movw r1,#0x273
  000e9846: ldr r0,[r0,#0x0]
  000e9848: ldr r3,[r3,#0x0]
  000e984a: ldr r4,[r0,#0x0]
  000e984c: ldr r0,[r3,#0x0]
  000e984e: blx 0x00072f70
  000e9852: add r5,sp,#0x4c
  000e9854: mov r1,r0
  000e9856: movs r2,#0x0
  000e9858: mov r0,r5
  000e985a: blx 0x0006f028
  000e985e: mov r0,r4
  000e9860: mov r1,r5
  000e9862: blx 0x000747c4
  000e9866: add r0,sp,#0x4c
  000e9868: blx 0x0006ee30
  000e986c: b 0x000e932c
  000e986e: ldr.w r0,[r9,#0x0]
  000e9872: blx 0x00071770
  000e9876: cmp r0,#0x4d
  000e9878: bne 0x000e988c
  000e987a: ldr.w r0,[r9,#0x0]
  000e987e: blx 0x00071a58
  000e9882: blx 0x000719c8
  000e9886: cmp r0,#0x25
  000e9888: bne.w 0x000e9bcc
  000e988c: ldr.w r0,[r10,#0x10]
  000e9890: cmp r0,#0x0
  000e9892: beq.w 0x000e9acc
  000e9896: mov.w r1,#0xffffffff
  000e989a: movs r2,#0x0
  000e989c: str r1,[sp,#0x0]
  000e989e: movs r1,#0x0
  000e98a0: movs r3,#0x0
  000e98a2: blx 0x00074554
  000e98a6: b 0x000e9aec
  000e98a8: ldr.w r0,[r10,#0x78]
  000e98ac: cbnz r0,0x000e98c6
  000e98ae: mov.w r0,#0x134
  000e98b2: blx 0x0006eb24
  000e98b6: mov r5,r0
  000e98b8: blx 0x00075250
  000e98bc: mov r0,r5
  000e98be: str.w r5,[r10,#0x78]
  000e98c2: blx 0x0007513c
  000e98c6: ldr.w r0,[r10,#0x74]
  000e98ca: cbz r0,0x000e990a
  000e98cc: blx 0x00075208
  000e98d0: cbz r0,0x000e990a
  000e98d2: ldr.w r0,[r10,#0x78]
  000e98d6: cbz r0,0x000e98e0
  000e98d8: blx 0x00075214
  000e98dc: blx 0x0006eb48
  000e98e0: movs r0,#0x0
  000e98e2: str.w r0,[r10,#0x78]
  000e98e6: mov.w r0,#0x134
  000e98ea: blx 0x0006eb24
  000e98ee: mov r5,r0
  000e98f0: blx 0x00075250
  000e98f4: mov r0,r5
  000e98f6: str.w r5,[r10,#0x78]
  000e98fa: blx 0x0007513c
  000e98fe: ldr.w r0,[r10,#0x74]
  000e9902: movs r1,#0x0
  000e9904: blx 0x00075400
  000e9908: b 0x000e994c
  000e990a: ldr.w r0,[r10,#0x80]
  000e990e: cbz r0,0x000e994c
  000e9910: blx 0x0007540c
  000e9914: cbz r0,0x000e994c
  000e9916: ldr.w r0,[r10,#0x78]
  000e991a: cbz r0,0x000e9924
  000e991c: blx 0x00075214
  000e9920: blx 0x0006eb48
  000e9924: movs r0,#0x0
  000e9926: str.w r0,[r10,#0x78]
  000e992a: mov.w r0,#0x134
  000e992e: blx 0x0006eb24
  000e9932: mov r5,r0
  000e9934: blx 0x00075250
  000e9938: mov r0,r5
  000e993a: str.w r5,[r10,#0x78]
  000e993e: blx 0x0007513c
  000e9942: ldr.w r0,[r10,#0x80]
  000e9946: movs r1,#0x0
  000e9948: blx 0x00075418
  000e994c: ldr r0,[0x000e9c3c]
  000e994e: add r4,sp,#0x70
  000e9950: movs r6,#0x0
  000e9952: add r0,pc
  000e9954: ldr.w r11,[r0,#0x0]
  000e9958: ldr r0,[0x000e9c40]
  000e995a: add r0,pc
  000e995c: ldr r5,[r0,#0x0]
  000e995e: b 0x000e99a4
  000e9960: cmp r6,#0x9
  000e9962: bhi 0x000e99a2
  000e9964: ldr r0,[r0,#0x4]
  000e9966: ldr.w r1,[r0,r6,lsl #0x2]
  000e996a: mov r0,r4
  000e996c: blx 0x00074dd0
  000e9970: vldr.32 s0,[sp,#0x70]
  000e9974: vcvt.s32.f32 s0,s0
  000e9978: vmov r0,s0
  000e997c: str.w r0,[r11,r6,lsl #0x2]
  000e9980: ldr.w r0,[r10,#0x78]
  000e9984: ldr r0,[r0,#0x4]
  000e9986: ldr r0,[r0,#0x4]
  000e9988: ldr.w r1,[r0,r6,lsl #0x2]
  000e998c: mov r0,r4
  000e998e: blx 0x00074dd0
  000e9992: vldr.32 s0,[sp,#0x74]
  000e9996: vcvt.s32.f32 s0,s0
  000e999a: vmov r0,s0
  000e999e: str.w r0,[r5,r6,lsl #0x2]
  000e99a2: adds r6,#0x1
  000e99a4: ldr.w r0,[r10,#0x78]
  000e99a8: ldr r0,[r0,#0x4]
  000e99aa: ldr r1,[r0,#0x0]
  000e99ac: cmp r6,r1
  000e99ae: bcc 0x000e9960
  000e99b0: ldr r0,[0x000e9c44]
  000e99b2: movs r4,#0x0
  000e99b4: add r0,pc
  000e99b6: ldr r0,[r0,#0x0]
  000e99b8: str r1,[r0,#0x0]
  000e99ba: str.w r4,[r10,#0xd0]
  000e99be: ldr.w r0,[r9,#0x0]
  000e99c2: blx 0x00071a58
  000e99c6: blx 0x00073750
  000e99ca: mov r5,r0
  000e99cc: cbz r0,0x000e9a48
  000e99ce: movs r6,#0x0
  000e99d0: b 0x000e9a42
  000e9a10: ldr r0,[r5,#0x4]
  000e9a12: ldr.w r0,[r0,r6,lsl #0x2]
  000e9a16: blx 0x000718d8
  000e9a1a: cmp r0,#0x84
  000e9a1c: blt 0x000e9a40
  000e9a1e: ldr r0,[r5,#0x4]
  000e9a20: ldr.w r0,[r0,r6,lsl #0x2]
  000e9a24: blx 0x000718d8
  000e9a28: cmp r0,#0x99
  000e9a2a: bgt 0x000e9a40
  000e9a2c: ldr r0,[r5,#0x4]
  000e9a2e: ldr.w r0,[r0,r6,lsl #0x2]
  000e9a32: blx 0x0007183c
  000e9a36: ldr.w r1,[r10,#0xd0]
  000e9a3a: add r0,r1
  000e9a3c: str.w r0,[r10,#0xd0]
  000e9a40: adds r6,#0x1
  000e9a42: ldr r0,[r5,#0x0]
  000e9a44: cmp r6,r0
  000e9a46: bcc 0x000e9a10
  000e9a48: ldr r0,[0x000e9c48]
  000e9a4a: movs r1,#0x0
  000e9a4c: mov.w r3,#0x3f800000
  000e9a50: add r0,pc
  000e9a52: ldr r5,[r0,#0x0]
  000e9a54: ldr r0,[r5,#0x0]
  000e9a56: ldr r2,[r0,#0x0]
  000e9a58: blx 0x0007159c
  000e9a5c: ldr r0,[r5,#0x0]
  000e9a5e: movs r1,#0x7a
  000e9a60: blx 0x000724a8
  000e9a64: ldr r0,[r5,#0x0]
  000e9a66: movs r1,#0x5f
  000e9a68: movs r2,#0x0
  000e9a6a: movs r3,#0x0
  000e9a6c: str r4,[sp,#0x0]
  000e9a6e: blx 0x00071548
  000e9a72: ldr r0,[0x000e9c4c]
  000e9a74: add r0,pc
  000e9a76: ldr r0,[r0,#0x0]
  000e9a78: ldr r0,[r0,#0x0]
  000e9a7a: blx 0x0007258c
  000e9a7e: blx 0x0006ee78
  000e9a82: ldr r1,[0x000e9c50]
  000e9a84: movs r2,#0x1
  000e9a86: strb.w r0,[r10,#0x12c]
  000e9a8a: add r1,pc
  000e9a8c: strb.w r2,[r10,#0x66]
  000e9a90: ldr r1,[r1,#0x0]
  000e9a92: ldrb r0,[r1,#0x8]
  000e9a94: cmp r0,#0x0
  000e9a96: bne.w 0x000e932c
  000e9a9a: ldr r0,[0x000e9c54]
  000e9a9c: ldr r3,[0x000e9c58]
  000e9a9e: add r0,pc
  000e9aa0: strb r2,[r1,#0x8]
  000e9aa2: add r3,pc
  000e9aa4: movw r1,#0x26e
  000e9aa8: ldr r0,[r0,#0x0]
  000e9aaa: ldr r3,[r3,#0x0]
  000e9aac: ldr r4,[r0,#0x0]
  000e9aae: ldr r0,[r3,#0x0]
  000e9ab0: blx 0x00072f70
  000e9ab4: add r5,sp,#0x64
  000e9ab6: mov r1,r0
  000e9ab8: movs r2,#0x0
  000e9aba: mov r0,r5
  000e9abc: blx 0x0006f028
  000e9ac0: mov r0,r4
  000e9ac2: mov r1,r5
  000e9ac4: blx 0x000747c4
  000e9ac8: add r0,sp,#0x64
  000e9aca: b 0x000e9868
  000e9acc: mov.w r0,#0x1e8
  000e9ad0: blx 0x0006eb24
  000e9ad4: mov r5,r0
  000e9ad6: mov.w r0,#0xffffffff
  000e9ada: movs r1,#0x0
  000e9adc: str r0,[sp,#0x0]
  000e9ade: mov r0,r5
  000e9ae0: movs r2,#0x0
  000e9ae2: movs r3,#0x0
  000e9ae4: blx 0x00075424
  000e9ae8: str.w r5,[r10,#0x10]
  000e9aec: ldr.w r0,[r9,#0x0]
  000e9af0: blx 0x00071a58
  000e9af4: movs r1,#0x55
  000e9af6: movs r2,#0x1
  000e9af8: blx 0x00073978
  000e9afc: cbnz r0,0x000e9b0c
  000e9afe: ldr.w r0,[r9,#0x0]
  000e9b02: blx 0x00071a58
  000e9b06: blx 0x000719bc
  000e9b0a: cbz r0,0x000e9b18
  000e9b0c: ldr.w r0,[r10,#0x10]
  000e9b10: movs r1,#0x0
  000e9b12: movs r2,#0x1
  000e9b14: blx 0x00075430
  000e9b18: ldr r0,[0x000e9c60]
  000e9b1a: movs r1,#0x7a
  000e9b1c: add r0,pc
  000e9b1e: ldr r4,[r0,#0x0]
  000e9b20: movs r0,#0x1
  000e9b22: strb.w r0,[r10,#0x67]
  000e9b26: ldr r0,[r4,#0x0]
  000e9b28: blx 0x000724a8
  000e9b2c: ldr r0,[r4,#0x0]
  000e9b2e: movs r1,#0x0
  000e9b30: ldr r3,[0x000e9c38]
  000e9b32: ldr r2,[r0,#0x0]
  000e9b34: blx 0x0007159c
  000e9b38: ldr r0,[0x000e9c64]
  000e9b3a: add r0,pc
  000e9b3c: ldr r5,[r0,#0x0]
  000e9b3e: ldrb r0,[r5,#0xe]
  000e9b40: cbnz r0,0x000e9b82
  000e9b42: ldr.w r0,[r9,#0x0]
  000e9b46: blx 0x00071770
  000e9b4a: cmp r0,#0x10
  000e9b4c: blt 0x000e9b82
  000e9b4e: ldr r0,[0x000e9c68]
  000e9b50: ldr r1,[0x000e9c6c]
  000e9b52: add r0,pc
  000e9b54: add r1,pc
  000e9b56: ldr r0,[r0,#0x0]
  000e9b58: ldr r1,[r1,#0x0]
  000e9b5a: ldr r4,[r0,#0x0]
  000e9b5c: ldr r0,[r1,#0x0]
  000e9b5e: movs r1,#0x1
  000e9b60: strb r1,[r5,#0xe]
  000e9b62: mov.w r1,#0x274
  000e9b66: blx 0x00072f70
  000e9b6a: add r5,sp,#0x40
  000e9b6c: mov r1,r0
  000e9b6e: movs r2,#0x0
  000e9b70: mov r0,r5
  000e9b72: blx 0x0006f028
  000e9b76: mov r0,r4
  000e9b78: mov r1,r5
  000e9b7a: blx 0x000747c4
  000e9b7e: add r0,sp,#0x40
  000e9b80: b 0x000e9868
  000e9b82: ldrb r0,[r5,#0xf]
  000e9b84: cmp r0,#0x0
  000e9b86: bne.w 0x000e932c
  000e9b8a: ldr.w r0,[r9,#0x0]
  000e9b8e: blx 0x00071770
  000e9b92: cmp r0,#0xf
  000e9b94: bgt.w 0x000e932c
  000e9b98: ldr r0,[0x000e9c70]
  000e9b9a: ldr r1,[0x000e9c74]
  000e9b9c: add r0,pc
  000e9b9e: add r1,pc
  000e9ba0: ldr r0,[r0,#0x0]
  000e9ba2: ldr r1,[r1,#0x0]
  000e9ba4: ldr r4,[r0,#0x0]
  000e9ba6: ldr r0,[r1,#0x0]
  000e9ba8: movs r1,#0x1
  000e9baa: strb r1,[r5,#0xf]
  000e9bac: movw r1,#0x277
  000e9bb0: blx 0x00072f70
  000e9bb4: add r5,sp,#0x34
  000e9bb6: mov r1,r0
  000e9bb8: movs r2,#0x0
  000e9bba: mov r0,r5
  000e9bbc: blx 0x0006f028
  000e9bc0: mov r0,r4
  000e9bc2: mov r1,r5
  000e9bc4: blx 0x000747c4
  000e9bc8: add r0,sp,#0x34
  000e9bca: b 0x000e9868
  000e9bcc: ldr r0,[0x000e9c5c]
  000e9bce: mov.w r1,#0x146
  000e9bd2: ldr.w r5,[r10,#0x70]
  000e9bd6: add r0,pc
  000e9bd8: b 0x000e974e
  001ac118: bx pc
  001ac148: bx pc
```
