# MGame::dockEvent
elf 0x17f920 body 1140
Sig: undefined __stdcall dockEvent(int param_1, int param_2)

## decompile
```c

/* MGame::dockEvent(int, int) */

void MGame::dockEvent(int param_1,int param_2)

{
  undefined1 uVar1;
  int iVar2;
  String *pSVar3;
  Station *this;
  int iVar4;
  StarMap *this_00;
  Engine *this_01;
  undefined4 uVar5;
  int *piVar6;
  code *pcVar7;
  ChoiceWindow *pCVar8;
  int *piVar9;
  String aSStack_94 [12];
  undefined1 auStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  int local_28;
  
  piVar9 = *(int **)(DAT_0018fce8 + 0x18f932);
  local_28 = *piVar9;
  uVar5 = *(undefined4 *)(param_1 + 0x78);
  PlayerEgo::getPosition();
  uVar1 = Level::collideStream(uVar5,local_34,uStack_30,uStack_2c);
  uVar5 = *(undefined4 *)(param_1 + 0x78);
  *(undefined1 *)(param_1 + 0xcb) = uVar1;
  PlayerEgo::getPosition();
  uVar1 = Level::collideStation(uVar5,local_40,uStack_3c,uStack_38);
  iVar2 = DAT_0018fcec;
  *(undefined1 *)(param_1 + 0xcc) = uVar1;
  piVar6 = *(int **)(iVar2 + 0x18f970);
  Status::getMission();
  iVar2 = Mission::isEmpty();
  if (iVar2 == 0) {
    Status::getMission();
    iVar2 = Mission::getType();
    if (iVar2 == 0xb) goto LAB_0018f9c2;
    Status::getMission();
    iVar2 = Mission::getType();
    if (iVar2 == 0) goto LAB_0018f9c2;
    Status::getMission();
    iVar2 = Mission::getType();
    if (iVar2 == 0xbd) goto LAB_0018f9c2;
    Status::getMission();
    iVar2 = Mission::getType();
    if (iVar2 == 0xab) goto LAB_0018f9c2;
    Status::getMission();
    iVar2 = Mission::getType();
    if (iVar2 == 0xac) goto LAB_0018f9c2;
    if (((*(char *)(param_1 + 0xcc) == '\0') && (*(char *)(param_1 + 0xcb) == '\0')) ||
       (iVar2 = PlayerEgo::isAutoPilot(), iVar2 == 0)) goto LAB_0018fd1a;
LAB_0018fd08:
    Hud::hudEvent(*(int *)(param_1 + 0x74),(PlayerEgo *)0x15,*(int *)(param_1 + 0x58));
  }
  else {
LAB_0018f9c2:
    if (*(char *)(param_1 + 0xcb) != '\0') {
      iVar2 = PlayerEgo::goingToStream(*(PlayerEgo **)(param_1 + 0x58));
      if (((iVar2 != 0) && (iVar2 = PlayerEgo::isDockingToStream(), iVar2 == 0)) &&
         (iVar2 = PlayerEgo::isDockedToStream(*(PlayerEgo **)(param_1 + 0x58)), iVar2 == 0)) {
        PlayerEgo::dockToStream(SUB41(*(undefined4 *)(param_1 + 0x58),0));
        *(undefined1 *)(param_1 + 0x15c) = 0;
        *(undefined1 *)(param_1 + 0x111) = 1;
        goto LAB_0018fd1a;
      }
      if (*(char *)(param_1 + 0xcb) != '\0') {
        uVar5 = Player::getHitpoints();
        *(undefined4 *)(*piVar6 + 100) = uVar5;
        uVar5 = Player::getShieldHP((Player *)**(undefined4 **)(param_1 + 0x58));
        *(undefined4 *)(*piVar6 + 0x5c) = uVar5;
        uVar5 = Player::getArmorHP();
        *(undefined4 *)(*piVar6 + 0x60) = uVar5;
        uVar5 = Player::getGammaHP();
        *(undefined4 *)(*piVar6 + 0x68) = uVar5;
        uVar5 = PlayerEgo::getCurrentSecondaryWeaponIndex();
        *(undefined4 *)(*piVar6 + 0xf4) = uVar5;
        iVar2 = PlayerEgo::isAutoPilot();
        piVar6 = *(int **)(DAT_0018fcf0 + 0x18fa3a);
        if ((*piVar6 == 0) || (iVar2 == 0)) {
          if (*(char *)(param_1 + 199) != '\0') {
            *(undefined4 *)(param_1 + 0x30) = 0;
            *(undefined4 *)(param_1 + 0x34) = 0;
            goto LAB_0018fd1a;
          }
          PlayerEgo::isAutoPilot();
          iVar2 = PlayerEgo::goingToStream(*(PlayerEgo **)(param_1 + 0x58));
          if (iVar2 == 0) goto LAB_0018fd1a;
          if (*piVar6 == 0) {
            *(undefined1 *)(param_1 + 0x15c) = 0;
            *(undefined1 *)(param_1 + 0x111) = 1;
            if (*(int *)(param_1 + 0x90) == 0) {
              this_00 = operator_new(0x1e8);
              StarMap::StarMap(this_00,false,(Mission *)0x0,false,-1);
              *(StarMap **)(param_1 + 0x90) = this_00;
            }
            this_01 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
            AbyssEngine::Engine::SetPostEffect(this_01,DAT_0018fe0c,false);
            StarMap::initLights(*(StarMap **)(param_1 + 0x90));
            StarMap::setJumpMapMode(SUB41(*(undefined4 *)(param_1 + 0x90),0),true);
            PlayerEgo::setAutoPilot(*(KIPlayer **)(param_1 + 0x58));
            *(undefined1 *)(param_1 + 0x5d) = 1;
            *(undefined1 *)(param_1 + 199) = 1;
            pauseSounds();
            *(undefined1 *)(param_1 + 0x111) = 1;
            goto LAB_0018fd1a;
          }
          if (*(char *)(param_1 + 0xc5) != '\0') goto LAB_0018fd1a;
          *(undefined1 *)(param_1 + 0x15c) = 0;
          pCVar8 = *(ChoiceWindow **)(param_1 + 0x94);
          *(undefined1 *)(param_1 + 0x111) = 1;
          if (pCVar8 == (ChoiceWindow *)0x0) {
            pCVar8 = operator_new(0x5c);
            ChoiceWindow::ChoiceWindow(pCVar8);
            *(ChoiceWindow **)(param_1 + 0x94) = pCVar8;
          }
          piVar6 = *(int **)(DAT_0018fe20 + 0x18fc74);
          pSVar3 = (String *)GameText::getText(*piVar6);
          AbyssEngine::String::String(aSStack_7c,(char *)(DAT_0018fe24 + 0x18fc8c),false);
          AbyssEngine::operator+(aSStack_70,pSVar3);
          Station::getName();
          AbyssEngine::operator+(aSStack_64,aSStack_70);
          AbyssEngine::String::String(aSStack_94,(char *)(DAT_0018fe28 + 0x18fcb2),false);
          AbyssEngine::operator+(aSStack_58,aSStack_64);
          GameText::getText(*piVar6);
          AbyssEngine::operator+(aSStack_4c,aSStack_58);
          ChoiceWindow::set(pCVar8,aSStack_4c,true);
        }
        else {
          if (*(char *)(param_1 + 0xc5) != '\0') goto LAB_0018fd1a;
          *(undefined1 *)(param_1 + 0x15c) = 0;
          pCVar8 = *(ChoiceWindow **)(param_1 + 0x94);
          *(undefined1 *)(param_1 + 0x111) = 1;
          if (pCVar8 == (ChoiceWindow *)0x0) {
            pCVar8 = operator_new(0x5c);
            ChoiceWindow::ChoiceWindow(pCVar8);
            *(ChoiceWindow **)(param_1 + 0x94) = pCVar8;
          }
          piVar6 = *(int **)(DAT_0018fe10 + 0x18fa82);
          pSVar3 = (String *)GameText::getText(*piVar6);
          AbyssEngine::String::String(aSStack_7c,(char *)(DAT_0018fe14 + 0x18fa9a),false);
          AbyssEngine::operator+(aSStack_70,pSVar3);
          Station::getName();
          AbyssEngine::operator+(aSStack_64,aSStack_70);
          AbyssEngine::String::String(aSStack_94,(char *)(DAT_0018fe18 + 0x18fac0),false);
          AbyssEngine::operator+(aSStack_58,aSStack_64);
          GameText::getText(*piVar6);
          AbyssEngine::operator+(aSStack_4c,aSStack_58);
          ChoiceWindow::set(pCVar8,aSStack_4c,true);
        }
        pcVar7 = *(code **)(DAT_0018fe1c + 0x18faf8);
        (*pcVar7)(aSStack_4c);
        (*pcVar7)(aSStack_58);
        (*pcVar7)(aSStack_94);
        (*pcVar7)(aSStack_64);
        (*pcVar7)(auStack_88);
        (*pcVar7)(aSStack_70);
        (*pcVar7)(aSStack_7c);
        ChoiceWindow::left();
        *(undefined1 *)(param_1 + 0x5d) = 1;
        *(undefined1 *)(param_1 + 0xc5) = 1;
        pauseSounds();
        PlayerEgo::setAutoPilot(*(KIPlayer **)(param_1 + 0x58));
        goto LAB_0018fd1a;
      }
    }
    if (*(char *)(param_1 + 0xcc) == '\0') {
      iVar2 = PlayerEgo::getAutoPilotTarget(*(PlayerEgo **)(param_1 + 0x58));
      iVar4 = Level::getLandmarks();
      if ((iVar2 != **(int **)(iVar4 + 4)) ||
         (iVar2 = PlayerEgo::collidesWithStation(*(PlayerEgo **)(param_1 + 0x58)), iVar2 == 0))
      goto LAB_0018fd1a;
    }
    iVar2 = Status::getCurrentCampaignMission();
    if ((0x30 < iVar2) && (iVar2 = Status::getCurrentCampaignMission(), iVar2 < 0x37)) {
      this = (Station *)Status::getStation();
      iVar2 = Station::getIndex(this);
      if (iVar2 != 0x4a) goto LAB_0018fd08;
    }
    iVar2 = PlayerEgo::goingToStation(*(PlayerEgo **)(param_1 + 0x58));
    if (((iVar2 != 0) && (iVar2 = Status::inAlienOrbit(), iVar2 == 0)) &&
       (iVar2 = Status::inEmptyOrbit((Status *)*piVar6), iVar2 == 0)) {
      Achievements::checkForNewMedal
                ((Achievements *)**(undefined4 **)(DAT_0018fe2c + 0x18fb90),
                 *(PlayerEgo **)(param_1 + 0x58));
      **(undefined4 **)(DAT_0018fe30 + 0x18fb9c) = 0;
      uVar5 = Player::getHitpoints();
      *(undefined4 *)(*piVar6 + 100) = uVar5;
      uVar5 = Player::getShieldHP((Player *)**(undefined4 **)(param_1 + 0x58));
      *(undefined4 *)(*piVar6 + 0x5c) = uVar5;
      uVar5 = Player::getArmorHP();
      *(undefined4 *)(*piVar6 + 0x60) = uVar5;
      uVar5 = Player::getGammaHP();
      *(undefined4 *)(*piVar6 + 0x68) = uVar5;
      AbyssEngine::ApplicationManager::SetCurrentApplicationModule(*(uint *)(param_1 + 8));
      *(undefined1 *)(param_1 + 0x54) = 0;
    }
  }
LAB_0018fd1a:
  iVar2 = *piVar9 - local_28;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  0018f920: push {r4,r5,r6,r7,lr}
  0018f922: add r7,sp,#0xc
  0018f924: push {r8,r9,r10,r11}
  0018f928: sub sp,#0x74
  0018f92a: mov r4,r0
  0018f92c: ldr r0,[0x0018fce8]
  0018f92e: add r0,pc
  0018f930: ldr.w r9,[r0,#0x0]
  0018f934: ldr.w r0,[r9,#0x0]
  0018f938: str r0,[sp,#0x70]
  0018f93a: add r0,sp,#0x64
  0018f93c: ldr r1,[r4,#0x58]
  0018f93e: ldr r5,[r4,#0x78]
  0018f940: blx 0x0007264c
  0018f944: add r3,sp,#0x64
  0018f946: mov r0,r5
  0018f948: ldmia r3,{r1,r2,r3}
  0018f94a: blx 0x00078604
  0018f94e: ldr r1,[r4,#0x58]
  0018f950: ldr r5,[r4,#0x78]
  0018f952: strb.w r0,[r4,#0xcb]
  0018f956: add r0,sp,#0x58
  0018f958: blx 0x0007264c
  0018f95c: add r3,sp,#0x58
  0018f95e: mov r0,r5
  0018f960: ldmia r3,{r1,r2,r3}
  0018f962: blx 0x00078610
  0018f966: ldr r1,[0x0018fcec]
  0018f968: strb.w r0,[r4,#0xcc]
  0018f96c: add r1,pc
  0018f96e: ldr r6,[r1,#0x0]
  0018f970: ldr r0,[r6,#0x0]
  0018f972: blx 0x00072850
  0018f976: blx 0x00072868
  0018f97a: cbnz r0,0x0018f9c2
  0018f97c: ldr r0,[r6,#0x0]
  0018f97e: blx 0x00072850
  0018f982: blx 0x00072874
  0018f986: cmp r0,#0xb
  0018f988: beq 0x0018f9c2
  0018f98a: ldr r0,[r6,#0x0]
  0018f98c: blx 0x00072850
  0018f990: blx 0x00072874
  0018f994: cbz r0,0x0018f9c2
  0018f996: ldr r0,[r6,#0x0]
  0018f998: blx 0x00072850
  0018f99c: blx 0x00072874
  0018f9a0: cmp r0,#0xbd
  0018f9a2: beq 0x0018f9c2
  0018f9a4: ldr r0,[r6,#0x0]
  0018f9a6: blx 0x00072850
  0018f9aa: blx 0x00072874
  0018f9ae: cmp r0,#0xab
  0018f9b0: beq 0x0018f9c2
  0018f9b2: ldr r0,[r6,#0x0]
  0018f9b4: blx 0x00072850
  0018f9b8: blx 0x00072874
  0018f9bc: cmp r0,#0xac
  0018f9be: bne.w 0x0018fcf4
  0018f9c2: ldrb.w r0,[r4,#0xcb]
  0018f9c6: cmp r0,#0x0
  0018f9c8: beq.w 0x0018fb36
  0018f9cc: ldr r0,[r4,#0x58]
  0018f9ce: blx 0x00072598
  0018f9d2: cbz r0,0x0018f9e8
  0018f9d4: ldr r0,[r4,#0x58]
  0018f9d6: blx 0x00077b84
  0018f9da: cbnz r0,0x0018f9e8
  0018f9dc: ldr r0,[r4,#0x58]
  0018f9de: blx 0x0007861c
  0018f9e2: cmp r0,#0x0
  0018f9e4: beq.w 0x0018fc10
  0018f9e8: ldrb.w r0,[r4,#0xcb]
  0018f9ec: cmp r0,#0x0
  0018f9ee: beq.w 0x0018fb36
  0018f9f2: ldr r0,[r4,#0x58]
  0018f9f4: ldr r0,[r0,#0x0]
  0018f9f6: blx 0x000724f0
  0018f9fa: ldr r1,[r6,#0x0]
  0018f9fc: str r0,[r1,#0x64]
  0018f9fe: ldr r0,[r4,#0x58]
  0018fa00: ldr r0,[r0,#0x0]
  0018fa02: blx 0x000726c4
  0018fa06: ldr r1,[r6,#0x0]
  0018fa08: str r0,[r1,#0x5c]
  0018fa0a: ldr r0,[r4,#0x58]
  0018fa0c: ldr r0,[r0,#0x0]
  0018fa0e: blx 0x00072928
  0018fa12: ldr r1,[r6,#0x0]
  0018fa14: str r0,[r1,#0x60]
  0018fa16: ldr r0,[r4,#0x58]
  0018fa18: ldr r0,[r0,#0x0]
  0018fa1a: blx 0x00072814
  0018fa1e: ldr r1,[r6,#0x0]
  0018fa20: str r0,[r1,#0x68]
  0018fa22: ldr r0,[r4,#0x58]
  0018fa24: blx 0x00076d20
  0018fa28: ldr r1,[r6,#0x0]
  0018fa2a: str.w r0,[r1,#0xf4]
  0018fa2e: ldr r0,[r4,#0x58]
  0018fa30: blx 0x0007681c
  0018fa34: ldr r1,[0x0018fcf0]
  0018fa36: add r1,pc
  0018fa38: ldr r6,[r1,#0x0]
  0018fa3a: ldr r1,[r6,#0x0]
  0018fa3c: cmp r1,#0x0
  0018fa3e: beq.w 0x0018fc00
  0018fa42: cmp r0,#0x0
  0018fa44: beq.w 0x0018fc00
  0018fa48: ldrb.w r0,[r4,#0xc5]
  0018fa4c: movs r5,#0x0
  0018fa4e: cmp r0,#0x0
  0018fa50: bne.w 0x0018fd1a
  0018fa54: strb.w r5,[r4,#0x15c]
  0018fa58: movs r0,#0x1
  0018fa5a: ldr.w r8,[r4,#0x94]
  0018fa5e: strb.w r0,[r4,#0x111]
  0018fa62: cmp.w r8,#0x0
  0018fa66: bne 0x0018fa78
  0018fa68: movs r0,#0x5c
  0018fa6a: blx 0x0006eb24
  0018fa6e: mov r8,r0
  0018fa70: blx 0x00074584
  0018fa74: str.w r8,[r4,#0x94]
  0018fa78: ldr r0,[0x0018fe10]
  0018fa7a: movw r1,#0x23e
  0018fa7e: add r0,pc
  0018fa80: ldr.w r11,[r0,#0x0]
  0018fa84: ldr.w r0,[r11,#0x0]
  0018fa88: blx 0x00072f70
  0018fa8c: ldr r1,[0x0018fe14]
  0018fa8e: add.w r10,sp,#0x1c
  0018fa92: mov r5,r0
  0018fa94: movs r2,#0x0
  0018fa96: add r1,pc
  0018fa98: mov r0,r10
  0018fa9a: blx 0x0006ee18
  0018fa9e: add r0,sp,#0x28
  0018faa0: mov r1,r5
  0018faa2: mov r2,r10
  0018faa4: blx 0x0006ef98
  0018faa8: ldr r1,[r6,#0x0]
  0018faaa: add r0,sp,#0x10
  0018faac: blx 0x000736a8
  0018fab0: add r0,sp,#0x34
  0018fab2: add r1,sp,#0x28
  0018fab4: add r2,sp,#0x10
  0018fab6: blx 0x0006ef98
  0018faba: ldr r1,[0x0018fe18]
  0018fabc: add r1,pc
  0018fabe: add r0,sp,#0x4
  0018fac0: movs r2,#0x0
  0018fac2: blx 0x0006ee18
  0018fac6: add r0,sp,#0x40
  0018fac8: add r1,sp,#0x34
  0018faca: add r2,sp,#0x4
  0018facc: blx 0x0006ef98
  0018fad0: ldr.w r0,[r11,#0x0]
  0018fad4: movw r1,#0x1a5
  0018fad8: blx 0x00072f70
  0018fadc: mov r2,r0
  0018fade: add r0,sp,#0x4c
  0018fae0: add r1,sp,#0x40
  0018fae2: blx 0x0006ef98
  0018fae6: add r1,sp,#0x4c
  0018fae8: mov r0,r8
  0018faea: movs r2,#0x1
  0018faec: movs r5,#0x1
  0018faee: blx 0x000746f8
  0018faf2: ldr r0,[0x0018fe1c]
  0018faf4: add r0,pc
  0018faf6: ldr r6,[r0,#0x0]
  0018faf8: add r0,sp,#0x4c
  0018fafa: blx r6
  0018fafc: add r0,sp,#0x40
  0018fafe: blx r6
  0018fb00: add r0,sp,#0x4
  0018fb02: blx r6
  0018fb04: add r0,sp,#0x34
  0018fb06: blx r6
  0018fb08: add r0,sp,#0x10
  0018fb0a: blx r6
  0018fb0c: add r0,sp,#0x28
  0018fb0e: blx r6
  0018fb10: add r0,sp,#0x1c
  0018fb12: blx r6
  0018fb14: ldr.w r0,[r4,#0x94]
  0018fb18: blx 0x00075304
  0018fb1c: mov r0,r4
  0018fb1e: strb.w r5,[r4,#0x5d]
  0018fb22: strb.w r5,[r4,#0xc5]
  0018fb26: blx 0x0007822c
  0018fb2a: ldr r0,[r4,#0x58]
  0018fb2c: movs r1,#0x0
  0018fb2e: movs r5,#0x0
  0018fb30: blx 0x000728d4
  0018fb34: b 0x0018fd1a
  0018fb36: ldrb.w r0,[r4,#0xcc]
  0018fb3a: cmp r0,#0x0
  0018fb3c: beq 0x0018fbdc
  0018fb3e: ldr r0,[r6,#0x0]
  0018fb40: blx 0x00071770
  0018fb44: cmp r0,#0x31
  0018fb46: blt 0x0018fb62
  0018fb48: ldr r0,[r6,#0x0]
  0018fb4a: blx 0x00071770
  0018fb4e: cmp r0,#0x36
  0018fb50: bgt 0x0018fb62
  0018fb52: ldr r0,[r6,#0x0]
  0018fb54: blx 0x00071c14
  0018fb58: blx 0x00071824
  0018fb5c: cmp r0,#0x4a
  0018fb5e: bne.w 0x0018fd08
  0018fb62: ldr r0,[r4,#0x58]
  0018fb64: blx 0x000725a4
  0018fb68: cmp r0,#0x0
  0018fb6a: beq.w 0x0018fd18
  0018fb6e: ldr r0,[r6,#0x0]
  0018fb70: blx 0x000723d0
  0018fb74: cmp r0,#0x0
  0018fb76: bne.w 0x0018fd18
  0018fb7a: ldr r0,[r6,#0x0]
  0018fb7c: blx 0x00073d20
  0018fb80: movs r5,#0x0
  0018fb82: cmp r0,#0x0
  0018fb84: bne.w 0x0018fd1a
  0018fb88: ldr r0,[0x0018fe2c]
  0018fb8a: ldr r1,[r4,#0x58]
  0018fb8c: add r0,pc
  0018fb8e: ldr r0,[r0,#0x0]
  0018fb90: ldr r0,[r0,#0x0]
  0018fb92: blx 0x00078628
  0018fb96: ldr r0,[0x0018fe30]
  0018fb98: add r0,pc
  0018fb9a: ldr r0,[r0,#0x0]
  0018fb9c: str r5,[r0,#0x0]
  0018fb9e: ldr r0,[r4,#0x58]
  0018fba0: ldr r0,[r0,#0x0]
  0018fba2: blx 0x000724f0
  0018fba6: ldr r1,[r6,#0x0]
  0018fba8: str r0,[r1,#0x64]
  0018fbaa: ldr r0,[r4,#0x58]
  0018fbac: ldr r0,[r0,#0x0]
  0018fbae: blx 0x000726c4
  0018fbb2: ldr r1,[r6,#0x0]
  0018fbb4: str r0,[r1,#0x5c]
  0018fbb6: ldr r0,[r4,#0x58]
  0018fbb8: ldr r0,[r0,#0x0]
  0018fbba: blx 0x00072928
  0018fbbe: ldr r1,[r6,#0x0]
  0018fbc0: str r0,[r1,#0x60]
  0018fbc2: ldr r0,[r4,#0x58]
  0018fbc4: ldr r0,[r0,#0x0]
  0018fbc6: blx 0x00072814
  0018fbca: ldr r1,[r6,#0x0]
  0018fbcc: str r0,[r1,#0x68]
  0018fbce: movs r1,#0x5
  0018fbd0: ldr r0,[r4,#0x8]
  0018fbd2: blx 0x00071d64
  0018fbd6: strb.w r5,[r4,#0x54]
  0018fbda: b 0x0018fc0c
  0018fbdc: ldr r0,[r4,#0x58]
  0018fbde: blx 0x000767f8
  0018fbe2: mov r5,r0
  0018fbe4: ldr r0,[r4,#0x78]
  0018fbe6: blx 0x000725b0
  0018fbea: ldr r0,[r0,#0x4]
  0018fbec: ldr r0,[r0,#0x0]
  0018fbee: cmp r5,r0
  0018fbf0: bne.w 0x0018fd18
  0018fbf4: ldr r0,[r4,#0x58]
  0018fbf6: blx 0x00078634
  0018fbfa: cmp r0,#0x0
  0018fbfc: bne 0x0018fb3e
  0018fbfe: b 0x0018fd18
  0018fc00: ldrb.w r0,[r4,#0xc7]
  0018fc04: cbz r0,0x0018fc26
  0018fc06: movs r0,#0x0
  0018fc08: strd r0,r0,[r4,#0x30]
  0018fc0c: movs r5,#0x1
  0018fc0e: b 0x0018fd1a
  0018fc10: ldr r0,[r4,#0x58]
  0018fc12: movs r1,#0x1
  0018fc14: movs r6,#0x1
  0018fc16: blx 0x000783a0
  0018fc1a: movs r5,#0x0
  0018fc1c: strb.w r5,[r4,#0x15c]
  0018fc20: strb.w r6,[r4,#0x111]
  0018fc24: b 0x0018fd1a
  0018fc26: ldr r0,[r4,#0x58]
  0018fc28: blx 0x0007681c
  0018fc2c: ldr r0,[r4,#0x58]
  0018fc2e: blx 0x00072598
  0018fc32: cmp r0,#0x0
  0018fc34: beq 0x0018fd18
  0018fc36: ldr r0,[r6,#0x0]
  0018fc38: cmp r0,#0x0
  0018fc3a: beq 0x0018fd32
  0018fc3c: ldrb.w r0,[r4,#0xc5]
  0018fc40: movs r5,#0x0
  0018fc42: cmp r0,#0x0
  0018fc44: bne 0x0018fd1a
  0018fc46: strb.w r5,[r4,#0x15c]
  0018fc4a: movs r0,#0x1
  0018fc4c: ldr.w r8,[r4,#0x94]
  0018fc50: strb.w r0,[r4,#0x111]
  0018fc54: cmp.w r8,#0x0
  0018fc58: bne 0x0018fc6a
  0018fc5a: movs r0,#0x5c
  0018fc5c: blx 0x0006eb24
  0018fc60: mov r8,r0
  0018fc62: blx 0x00074584
  0018fc66: str.w r8,[r4,#0x94]
  0018fc6a: ldr r0,[0x0018fe20]
  0018fc6c: movw r1,#0x23e
  0018fc70: add r0,pc
  0018fc72: ldr.w r11,[r0,#0x0]
  0018fc76: ldr.w r0,[r11,#0x0]
  0018fc7a: blx 0x00072f70
  0018fc7e: ldr r1,[0x0018fe24]
  0018fc80: add.w r10,sp,#0x1c
  0018fc84: mov r5,r0
  0018fc86: movs r2,#0x0
  0018fc88: add r1,pc
  0018fc8a: mov r0,r10
  0018fc8c: blx 0x0006ee18
  0018fc90: add r0,sp,#0x28
  0018fc92: mov r1,r5
  0018fc94: mov r2,r10
  0018fc96: blx 0x0006ef98
  0018fc9a: ldr r1,[r6,#0x0]
  0018fc9c: add r0,sp,#0x10
  0018fc9e: blx 0x000736a8
  0018fca2: add r0,sp,#0x34
  0018fca4: add r1,sp,#0x28
  0018fca6: add r2,sp,#0x10
  0018fca8: blx 0x0006ef98
  0018fcac: ldr r1,[0x0018fe28]
  0018fcae: add r1,pc
  0018fcb0: add r0,sp,#0x4
  0018fcb2: movs r2,#0x0
  0018fcb4: blx 0x0006ee18
  0018fcb8: add r0,sp,#0x40
  0018fcba: add r1,sp,#0x34
  0018fcbc: add r2,sp,#0x4
  0018fcbe: blx 0x0006ef98
  0018fcc2: ldr.w r0,[r11,#0x0]
  0018fcc6: movw r1,#0x1a5
  0018fcca: blx 0x00072f70
  0018fcce: mov r2,r0
  0018fcd0: add r0,sp,#0x4c
  0018fcd2: add r1,sp,#0x40
  0018fcd4: blx 0x0006ef98
  0018fcd8: add r1,sp,#0x4c
  0018fcda: mov r0,r8
  0018fcdc: movs r2,#0x1
  0018fcde: movs r5,#0x1
  0018fce0: blx 0x000746f8
  0018fce4: b 0x0018faf2
  0018fcf4: ldrb.w r0,[r4,#0xcc]
  0018fcf8: cbnz r0,0x0018fd00
  0018fcfa: ldrb.w r0,[r4,#0xcb]
  0018fcfe: cbz r0,0x0018fd18
  0018fd00: ldr r0,[r4,#0x58]
  0018fd02: blx 0x0007681c
  0018fd06: cbz r0,0x0018fd18
  0018fd08: ldr r2,[r4,#0x58]
  0018fd0a: movs r1,#0x15
  0018fd0c: ldr r0,[r4,#0x74]
  0018fd0e: movs r3,#0x0
  0018fd10: movs r5,#0x0
  0018fd12: blx 0x00072418
  0018fd16: b 0x0018fd1a
  0018fd18: movs r5,#0x0
  0018fd1a: ldr r0,[sp,#0x70]
  0018fd1c: ldr.w r1,[r9,#0x0]
  0018fd20: subs r0,r1,r0
  0018fd22: itttt eq
  0018fd24: mov.eq r0,r5
  0018fd26: add.eq sp,#0x74
  0018fd28: pop.eq.w {r8,r9,r10,r11}
  0018fd2c: pop.eq {r4,r5,r6,r7,pc}
  0018fd2e: blx 0x0006e824
  0018fd32: movs r0,#0x0
  0018fd34: movs r5,#0x1
  0018fd36: strb.w r0,[r4,#0x15c]
  0018fd3a: ldr.w r0,[r4,#0x90]
  0018fd3e: strb.w r5,[r4,#0x111]
  0018fd42: cbnz r0,0x0018fd64
  0018fd44: mov.w r0,#0x1e8
  0018fd48: blx 0x0006eb24
  0018fd4c: mov r6,r0
  0018fd4e: mov.w r0,#0xffffffff
  0018fd52: movs r1,#0x0
  0018fd54: str r0,[sp,#0x0]
  0018fd56: mov r0,r6
  0018fd58: movs r2,#0x0
  0018fd5a: movs r3,#0x0
  0018fd5c: blx 0x00075424
  0018fd60: str.w r6,[r4,#0x90]
  0018fd64: ldr r0,[r4,#0x8]
  0018fd66: blx 0x0007258c
  0018fd6a: ldr r1,[0x0018fe0c]
  0018fd6c: movs r2,#0x0
  0018fd6e: blx 0x000707f8
  0018fd72: ldr.w r0,[r4,#0x90]
  0018fd76: blx 0x00078298
  0018fd7a: ldr.w r0,[r4,#0x90]
  0018fd7e: movs r1,#0x1
  0018fd80: movs r2,#0x0
  0018fd82: blx 0x00075430
  0018fd86: ldr r0,[r4,#0x58]
  0018fd88: movs r1,#0x0
  0018fd8a: blx 0x000728d4
  0018fd8e: mov r0,r4
  0018fd90: strb.w r5,[r4,#0x5d]
  0018fd94: strb.w r5,[r4,#0xc7]
  0018fd98: blx 0x0007822c
  0018fd9c: strb.w r5,[r4,#0x111]
  0018fda0: b 0x0018fd1a
```
