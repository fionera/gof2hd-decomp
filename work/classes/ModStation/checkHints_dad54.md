# ModStation::checkHints
elf 0xdad54 body 2194
Sig: undefined __stdcall checkHints(void)

## decompile
```c

/* ModStation::checkHints() */

void ModStation::checkHints(void)

{
  int in_r0;
  int iVar1;
  ChoiceWindow *this;
  String *pSVar2;
  String *pSVar3;
  String *pSVar4;
  String *pSVar5;
  String *pSVar6;
  undefined4 uVar7;
  void *pvVar8;
  DialogueWindow *pDVar9;
  Agent *this_00;
  Vector *pVVar10;
  undefined4 *puVar11;
  int *piVar12;
  int *piVar13;
  int iVar14;
  code *pcVar15;
  undefined4 uVar16;
  undefined4 *puVar17;
  int *piVar18;
  float fVar19;
  float extraout_s0;
  String aSStack_16c [12];
  String aSStack_160 [12];
  undefined1 auStack_154 [12];
  String aSStack_148 [12];
  String aSStack_13c [12];
  String aSStack_130 [12];
  String aSStack_124 [12];
  String aSStack_118 [12];
  undefined1 auStack_10c [12];
  String aSStack_100 [12];
  String aSStack_f4 [12];
  String aSStack_e8 [12];
  String aSStack_dc [12];
  String aSStack_d0 [12];
  undefined1 auStack_c4 [12];
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
  
  piVar18 = *(int **)(DAT_000eaf3c + 0xead66);
  local_28 = *piVar18;
  if (*(char *)(in_r0 + 0x60) != '\0') goto LAB_000eb4e0;
  if (((*(char *)(in_r0 + 0x6a) == '\0') &&
      (iVar14 = *(int *)(DAT_000eaf40 + 0xead86),
      *(char *)(in_r0 + 99) == '\0' && *(char *)(iVar14 + 0x34) == '\0')) &&
     (iVar1 = Status::getCurrentCampaignMission(), 0x12 < iVar1)) {
    puVar17 = *(undefined4 **)(DAT_000eaf48 + 0xeadac);
    this = *(ChoiceWindow **)(in_r0 + 0x70);
    pcVar15 = *(code **)(DAT_000eaf4c + 0xeadae);
    pSVar2 = (String *)(*pcVar15)(*puVar17,0x3e);
    pSVar3 = (String *)(*pcVar15)(*puVar17,0x49);
    pSVar4 = (String *)(*pcVar15)(*puVar17,0x7e);
    pSVar5 = (String *)(*pcVar15)(*puVar17,0x7f);
    pSVar6 = (String *)(*pcVar15)(*puVar17,0x20c);
    ChoiceWindow::set(this,pSVar2,pSVar3,true,pSVar4,pSVar5,pSVar6,-1,-1);
    *(undefined1 *)(iVar14 + 0x34) = 1;
    *(undefined1 *)(in_r0 + 99) = 1;
    *(undefined1 *)(in_r0 + 0xdc) = 1;
  }
  if (*(char *)(in_r0 + 0x69) == '\0') {
    if (*(char *)(in_r0 + 0x6a) != '\0') goto LAB_000eb4e0;
    iVar14 = *(int *)(DAT_000eaf94 + 0xeaf80);
    if (*(char *)(in_r0 + 99) == '\0' && *(char *)(iVar14 + 0x33) == '\0') {
      puVar17 = *(undefined4 **)(DAT_000eafb4 + 0xeaf9e);
      iVar1 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)*puVar17 + 4) + 0x18));
      if (iVar1 != 0) {
        piVar12 = *(int **)(DAT_000eafe8 + 0xeafc2);
        pSVar2 = (String *)GameText::getText(*piVar12);
        AbyssEngine::String::String(aSStack_34,pSVar2,false);
        uVar16 = *puVar17;
        AbyssEngine::String::String(aSStack_4c,aSStack_34,false);
        iVar1 = *piVar12;
        Wanted::getShip(*(Wanted **)(*(int *)(*(int *)*puVar17 + 4) + 0x18));
        pSVar2 = (String *)GameText::getText(iVar1);
        AbyssEngine::String::String(aSStack_58,pSVar2,false);
        uVar7 = AbyssEngine::String::String(aSStack_64,(char *)(DAT_000eb01c + 0xeb014),false);
        Status::replaceHash(auStack_40,uVar16,aSStack_4c,aSStack_58,uVar7);
        AbyssEngine::String::operator=(aSStack_34,auStack_40);
        pcVar15 = *(code **)(DAT_000eb424 + 0xeb03c);
        (*pcVar15)(auStack_40);
        (*pcVar15)(aSStack_64);
        (*pcVar15)(aSStack_58);
        (*pcVar15)(aSStack_4c);
        uVar16 = *puVar17;
        AbyssEngine::String::String(aSStack_70,aSStack_34,false);
        Wanted::getName();
        uVar7 = AbyssEngine::String::String(aSStack_88,(char *)(DAT_000eb428 + 0xeb06e),false);
        Status::replaceHash(auStack_40,uVar16,aSStack_70,auStack_7c,uVar7);
        AbyssEngine::String::operator=(aSStack_34,auStack_40);
        (*pcVar15)(auStack_40);
        (*pcVar15)(aSStack_88);
        (*pcVar15)(auStack_7c);
        (*pcVar15)(aSStack_70);
        ChoiceWindow::set(*(String **)(in_r0 + 0x70));
        *(undefined1 *)(in_r0 + 99) = 1;
        *(undefined1 *)(iVar14 + 0x33) = 1;
        AbyssEngine::String::~String(aSStack_34);
      }
    }
    if (*(char *)(in_r0 + 0x69) == '\0') {
      if (*(char *)(in_r0 + 0x6a) != '\0') goto LAB_000eb4e0;
      if (*(char *)(in_r0 + 99) == '\0' && *(char *)(iVar14 + 0x34) == '\0') {
        puVar17 = *(undefined4 **)(DAT_000eb42c + 0xeb0da);
        iVar1 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)*puVar17 + 4) + 0x30));
        if (iVar1 != 0) {
          piVar12 = *(int **)(DAT_000eb430 + 0xeb0f4);
          pSVar2 = (String *)GameText::getText(*piVar12);
          AbyssEngine::String::String(aSStack_34,pSVar2,false);
          uVar16 = *puVar17;
          AbyssEngine::String::String(aSStack_94,aSStack_34,false);
          iVar1 = *piVar12;
          Wanted::getShip(*(Wanted **)(*(int *)(*(int *)*puVar17 + 4) + 0x30));
          pSVar2 = (String *)GameText::getText(iVar1);
          AbyssEngine::String::String(aSStack_a0,pSVar2,false);
          uVar7 = AbyssEngine::String::String(aSStack_ac,(char *)(DAT_000eb434 + 0xeb13e),false);
          Status::replaceHash(auStack_40,uVar16,aSStack_94,aSStack_a0,uVar7);
          AbyssEngine::String::operator=(aSStack_34,auStack_40);
          pcVar15 = *(code **)(DAT_000eb438 + 0xeb160);
          (*pcVar15)(auStack_40);
          (*pcVar15)(aSStack_ac);
          (*pcVar15)(aSStack_a0);
          (*pcVar15)(aSStack_94);
          uVar16 = *puVar17;
          AbyssEngine::String::String(aSStack_b8,aSStack_34,false);
          Wanted::getName();
          uVar7 = AbyssEngine::String::String(aSStack_d0,(char *)(DAT_000eb43c + 0xeb192),false);
          Status::replaceHash(auStack_40,uVar16,aSStack_b8,auStack_c4,uVar7);
          AbyssEngine::String::operator=(aSStack_34,auStack_40);
          (*pcVar15)(auStack_40);
          (*pcVar15)(aSStack_d0);
          (*pcVar15)(auStack_c4);
          (*pcVar15)(aSStack_b8);
          ChoiceWindow::set(*(String **)(in_r0 + 0x70));
          *(undefined1 *)(in_r0 + 99) = 1;
          *(undefined1 *)(iVar14 + 0x34) = 1;
          AbyssEngine::String::~String(aSStack_34);
        }
      }
      if (*(char *)(in_r0 + 0x69) == '\0') {
        if (*(char *)(in_r0 + 0x6a) != '\0') goto LAB_000eb4e0;
        if (*(char *)(in_r0 + 99) == '\0' && *(char *)(iVar14 + 0x35) == '\0') {
          puVar17 = *(undefined4 **)(DAT_000eb440 + 0xeb1fe);
          iVar1 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)*puVar17 + 4) + 0x48));
          if (iVar1 != 0) {
            piVar12 = *(int **)(DAT_000eb444 + 0xeb218);
            pSVar2 = (String *)GameText::getText(*piVar12);
            AbyssEngine::String::String(aSStack_34,pSVar2,false);
            uVar16 = *puVar17;
            AbyssEngine::String::String(aSStack_dc,aSStack_34,false);
            iVar1 = *piVar12;
            Wanted::getShip(*(Wanted **)(*(int *)(*(int *)*puVar17 + 4) + 0x48));
            pSVar2 = (String *)GameText::getText(iVar1);
            AbyssEngine::String::String(aSStack_e8,pSVar2,false);
            uVar7 = AbyssEngine::String::String(aSStack_f4,(char *)(DAT_000eb448 + 0xeb262),false);
            Status::replaceHash(auStack_40,uVar16,aSStack_dc,aSStack_e8,uVar7);
            AbyssEngine::String::operator=(aSStack_34,auStack_40);
            pcVar15 = *(code **)(DAT_000eb44c + 0xeb284);
            (*pcVar15)(auStack_40);
            (*pcVar15)(aSStack_f4);
            (*pcVar15)(aSStack_e8);
            (*pcVar15)(aSStack_dc);
            uVar16 = *puVar17;
            AbyssEngine::String::String(aSStack_100,aSStack_34,false);
            Wanted::getName();
            uVar7 = AbyssEngine::String::String(aSStack_118,(char *)(DAT_000eb450 + 0xeb2b6),false);
            Status::replaceHash(auStack_40,uVar16,aSStack_100,auStack_10c,uVar7);
            AbyssEngine::String::operator=(aSStack_34,auStack_40);
            (*pcVar15)(auStack_40);
            (*pcVar15)(aSStack_118);
            (*pcVar15)(auStack_10c);
            (*pcVar15)(aSStack_100);
            ChoiceWindow::set(*(String **)(in_r0 + 0x70));
            *(undefined1 *)(in_r0 + 99) = 1;
            *(undefined1 *)(iVar14 + 0x35) = 1;
            AbyssEngine::String::~String(aSStack_34);
          }
        }
        if (*(char *)(in_r0 + 0x69) == '\0') {
          if (*(char *)(in_r0 + 0x6a) != '\0') goto LAB_000eb4e0;
          if (*(char *)(in_r0 + 99) == '\0' && *(char *)(iVar14 + 0x36) == '\0') {
            puVar17 = *(undefined4 **)(DAT_000eb454 + 0xeb322);
            iVar1 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)*puVar17 + 4) + 0x60));
            if (iVar1 != 0) {
              piVar12 = *(int **)(DAT_000eb458 + 0xeb33e);
              pSVar2 = (String *)GameText::getText(*piVar12);
              AbyssEngine::String::String(aSStack_34,pSVar2,false);
              uVar16 = *puVar17;
              AbyssEngine::String::String(aSStack_124,aSStack_34,false);
              iVar1 = *piVar12;
              Wanted::getShip(*(Wanted **)(*(int *)(*(int *)*puVar17 + 4) + 0x60));
              pSVar2 = (String *)GameText::getText(iVar1);
              AbyssEngine::String::String(aSStack_130,pSVar2,false);
              uVar7 = AbyssEngine::String::String
                                (aSStack_13c,(char *)(DAT_000eb45c + 0xeb388),false);
              Status::replaceHash(auStack_40,uVar16,aSStack_124,aSStack_130,uVar7);
              AbyssEngine::String::operator=(aSStack_34,auStack_40);
              pcVar15 = *(code **)(DAT_000eb460 + 0xeb3aa);
              (*pcVar15)(auStack_40);
              (*pcVar15)(aSStack_13c);
              (*pcVar15)(aSStack_130);
              (*pcVar15)(aSStack_124);
              uVar16 = *puVar17;
              AbyssEngine::String::String(aSStack_148,aSStack_34,false);
              Wanted::getName();
              uVar7 = AbyssEngine::String::String
                                (aSStack_160,(char *)(DAT_000eb464 + 0xeb3dc),false);
              Status::replaceHash(auStack_40,uVar16,aSStack_148,auStack_154,uVar7);
              AbyssEngine::String::operator=(aSStack_34,auStack_40);
              (*pcVar15)(auStack_40);
              (*pcVar15)(aSStack_160);
              (*pcVar15)(auStack_154);
              (*pcVar15)(aSStack_148);
              ChoiceWindow::set(*(String **)(in_r0 + 0x70));
              *(undefined1 *)(in_r0 + 99) = 1;
              *(undefined1 *)(iVar14 + 0x36) = 1;
              AbyssEngine::String::~String(aSStack_34);
            }
          }
        }
      }
    }
  }
  if (*(char *)(in_r0 + 0x6a) == '\0') {
    iVar14 = *(int *)(DAT_000eaf50 + 0xeae2c);
    if ((*(char *)(in_r0 + 99) == '\0' && *(char *)(iVar14 + 0x1a) == '\0') &&
       (iVar1 = Achievements::gotAllMedals
                          ((Achievements *)**(undefined4 **)(DAT_000eaf54 + 0xeae3c)), iVar1 != 0))
    {
      pSVar2 = *(String **)(in_r0 + 0x70);
      GameText::getText(**(int **)(DAT_000eaf58 + 0xeae50));
      ChoiceWindow::set(pSVar2);
      *(undefined1 *)(iVar14 + 0x1a) = 1;
      *(undefined1 *)(in_r0 + 99) = 1;
    }
    if (*(char *)(in_r0 + 0x6a) == '\0') {
      if ((*(char *)(in_r0 + 99) == '\0' && *(char *)(iVar14 + 0x1b) == '\0') &&
         (iVar1 = Achievements::gotAllGoldMedals(), iVar1 != 0)) {
        pSVar2 = *(String **)(in_r0 + 0x70);
        GameText::getText(**(int **)(DAT_000eaf60 + 0xeae94));
        ChoiceWindow::set(pSVar2);
        *(undefined1 *)(iVar14 + 0x1b) = 1;
        *(undefined1 *)(in_r0 + 99) = 1;
      }
      if (*(char *)(in_r0 + 0x6a) == '\0') {
        if (*(char *)(in_r0 + 99) == '\0') {
          puVar17 = *(undefined4 **)(DAT_000eb7c8 + 0xeb470);
          iVar1 = Status::isBlueprintUnlocked((Status *)*puVar17,0xe8);
          if (iVar1 == 0) {
            puVar11 = *(undefined4 **)(DAT_000eb7e0 + 0xeb482);
            iVar1 = Achievements::gotAllGoldMedals();
            if ((iVar1 != 0) &&
               (iVar1 = Achievements::gotAllSupernovaMedals((Achievements *)*puVar11), iVar1 != 0))
            {
              pSVar2 = *(String **)(in_r0 + 0x70);
              GameText::getText(**(int **)(DAT_000eb7e4 + 0xeb4a0));
              ChoiceWindow::set(pSVar2);
              Status::unlockBluePrint((Status *)*puVar17,0xe8);
              autosave();
              *(undefined1 *)(in_r0 + 99) = 1;
            }
          }
          if (*(char *)(in_r0 + 0x6a) != '\0') goto LAB_000eb4ce;
        }
        if ((*(char *)(in_r0 + 99) == '\0' && *(char *)(iVar14 + 0x3a) == '\0') &&
           (iVar1 = Status::getCurrentCampaignMission(), 0xa1 < iVar1)) {
          puVar17 = *(undefined4 **)(DAT_000eaf68 + 0xeaee4);
          iVar1 = Achievements::gotAllGoldMedals();
          if (((iVar1 != 0) &&
              ((iVar1 = Achievements::gotAllSupernovaMedals((Achievements *)*puVar17), iVar1 != 0 &&
               (iVar1 = Status::hardCoreMode(), iVar1 == 0)))) ||
             (iVar1 = Status::hardCoreMode(), iVar1 != 0)) {
            iVar1 = DAT_000eaf6c;
            *(undefined1 *)(iVar14 + 0x3a) = 1;
            pSVar2 = *(String **)(in_r0 + 0x70);
            GameText::getText(**(int **)(iVar1 + 0xeaf20));
            ChoiceWindow::set(pSVar2);
            autosave();
            *(undefined1 *)(in_r0 + 99) = 1;
          }
        }
      }
    }
  }
LAB_000eb4ce:
  if (((*(char *)(in_r0 + 0x69) == '\0') && (*(char *)(in_r0 + 0x6a) == '\0')) &&
     (*(char *)(in_r0 + 99) == '\0')) {
    piVar12 = *(int **)(DAT_000eb7e8 + 0xeb4fc);
    if (((*(int *)(*piVar12 + 0x24) == 0) || (*(char *)(in_r0 + 0xdb) != '\0')) ||
       ((0 < *(int *)(*piVar12 + 0x30) ||
        (*(char *)(**(int **)(DAT_000eb7ec + 0xeb514) + 0x2ec) != '\0')))) {
      if ((*(char *)(*piVar12 + 0xf9) != '\0') &&
         (*(char *)(**(int **)(DAT_000eb800 + 0xeb52e) + 0x2ec) == '\0')) {
        if (*(DialogueWindow **)(in_r0 + 0x84) != (DialogueWindow *)0x0) {
          pvVar8 = (void *)DialogueWindow::~DialogueWindow(*(DialogueWindow **)(in_r0 + 0x84));
          operator_delete(pvVar8);
          *(undefined4 *)(in_r0 + 0x84) = 0;
        }
        pDVar9 = operator_new(0x74);
        piVar13 = *(int **)(DAT_000eb804 + 0xeb55a);
        pSVar2 = (String *)GameText::getText(*piVar13);
        pSVar3 = (String *)GameText::getText(*piVar13);
        fVar19 = (float)DialogueWindow::DialogueWindow
                                  (pDVar9,pSVar2,pSVar3,(int *)(DAT_000eb808 + 0xeb578));
        iVar14 = DAT_000eb80c;
        *(DialogueWindow **)(in_r0 + 0x84) = pDVar9;
        FModSound::play(**(int **)(iVar14 + 0xeb58e),(Vector *)0x24d,(Vector *)0x0,fVar19);
        *(undefined1 *)(in_r0 + 0x69) = 1;
        iVar14 = *piVar12;
        *(undefined1 *)(iVar14 + 0xf9) = 0;
        Status::changeCredits(iVar14);
      }
    }
    else {
      if (*(DialogueWindow **)(in_r0 + 0x84) != (DialogueWindow *)0x0) {
        pvVar8 = (void *)DialogueWindow::~DialogueWindow(*(DialogueWindow **)(in_r0 + 0x84));
        operator_delete(pvVar8);
        *(undefined4 *)(in_r0 + 0x84) = 0;
      }
      pDVar9 = operator_new(0x74);
      pSVar2 = (String *)GameText::getText(**(int **)(DAT_000eb7f0 + 0xeb5d2));
      DialogueWindow::DialogueWindow
                (pDVar9,pSVar2,(String *)**(undefined4 **)(*(int *)(*piVar12 + 0x24) + 4),
                 *(int **)(*piVar12 + 0x28));
      *(DialogueWindow **)(in_r0 + 0x84) = pDVar9;
      this_00 = operator_new(0x98);
      AbyssEngine::String::String(aSStack_16c,(char *)(DAT_000eb7f4 + 0xeb600),false);
      Agent::Agent(this_00,0,aSStack_16c,0,0,*(undefined4 *)(*piVar12 + 0x2c),1,0,0,0,0);
      AbyssEngine::String::~String(aSStack_16c);
      iVar14 = **(int **)(DAT_000eb7f8 + 0xeb63a);
      pVVar10 = (Vector *)
                Globals::getDialogueSoundId
                          ((Globals *)**(undefined4 **)(DAT_000eb7fc + 0xeb63c),0x139,this_00);
      FModSound::play(iVar14,pVVar10,(Vector *)0x0,extraout_s0);
      pvVar8 = (void *)Agent::~Agent(this_00);
      operator_delete(pvVar8);
      *(undefined1 *)(in_r0 + 0xdb) = 1;
      *(undefined1 *)(in_r0 + 0x69) = 1;
      Status::setWingmen((Status *)*piVar12,(Array *)0x0);
    }
  }
LAB_000eb4e0:
  if (*piVar18 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000ead54: push {r4,r5,r6,r7,lr}
  000ead56: add r7,sp,#0xc
  000ead58: push {r8,r9,r10,r11}
  000ead5c: sub sp,#0x174
  000ead5e: mov r4,r0
  000ead60: ldr r0,[0x000eaf3c]
  000ead62: add r0,pc
  000ead64: ldr.w r11,[r0,#0x0]
  000ead68: ldr.w r0,[r11,#0x0]
  000ead6c: str r0,[sp,#0x170]
  000ead6e: ldrb.w r0,[r4,#0x60]
  000ead72: cmp r0,#0x0
  000ead74: bne.w 0x000eb4e0
  000ead78: ldrb.w r0,[r4,#0x6a]
  000ead7c: cmp r0,#0x0
  000ead7e: bne 0x000eae12
  000ead80: ldr r0,[0x000eaf40]
  000ead82: add r0,pc
  000ead84: ldr.w r8,[r0,#0x0]
  000ead88: ldrb.w r0,[r4,#0x63]
  000ead8c: ldrb.w r1,[r8,#0x34]
  000ead90: orrs r0,r1
  000ead92: bne 0x000eae12
  000ead94: ldr r0,[0x000eaf44]
  000ead96: add r0,pc
  000ead98: ldr r0,[r0,#0x0]
  000ead9a: ldr r0,[r0,#0x0]
  000ead9c: blx 0x00071770
  000eada0: cmp r0,#0x13
  000eada2: blt 0x000eae12
  000eada4: ldr r0,[0x000eaf48]
  000eada6: ldr r1,[0x000eaf4c]
  000eada8: add r0,pc
  000eadaa: add r1,pc
  000eadac: ldr.w r10,[r0,#0x0]
  000eadb0: ldr r0,[r4,#0x70]
  000eadb2: str r0,[sp,#0x28]
  000eadb4: ldr.w r9,[r1,#0x0]
  000eadb8: movs r1,#0x3e
  000eadba: ldr.w r0,[r10,#0x0]
  000eadbe: blx r9
  000eadc0: str r0,[sp,#0x24]
  000eadc2: movs r1,#0x49
  000eadc4: ldr.w r0,[r10,#0x0]
  000eadc8: blx r9
  000eadca: str r0,[sp,#0x20]
  000eadcc: movs r1,#0x7e
  000eadce: ldr.w r0,[r10,#0x0]
  000eadd2: blx r9
  000eadd4: mov r6,r0
  000eadd6: ldr.w r0,[r10,#0x0]
  000eadda: movs r1,#0x7f
  000eaddc: blx r9
  000eadde: mov r5,r0
  000eade0: ldr.w r0,[r10,#0x0]
  000eade4: mov.w r1,#0x20c
  000eade8: blx r9
  000eadea: mov.w r1,#0xffffffff
  000eadee: strd r6,r5,[sp,#0x0]
  000eadf2: strd r0,r1,[sp,#0x8]
  000eadf6: movs r3,#0x1
  000eadf8: str r1,[sp,#0x10]
  000eadfa: movs r5,#0x1
  000eadfc: ldr r0,[sp,#0x28]
  000eadfe: ldrd r2,r1,[sp,#0x20]
  000eae02: blx 0x00075574
  000eae06: strb.w r5,[r8,#0x34]
  000eae0a: strb.w r5,[r4,#0x63]
  000eae0e: strb.w r5,[r4,#0xdc]
  000eae12: ldrb.w r0,[r4,#0x69]
  000eae16: cmp r0,#0x0
  000eae18: beq.w 0x000eaf70
  000eae1c: ldrb.w r0,[r4,#0x6a]
  000eae20: cmp r0,#0x0
  000eae22: bne.w 0x000eb4ce
  000eae26: ldr r0,[0x000eaf50]
  000eae28: add r0,pc
  000eae2a: ldr r6,[r0,#0x0]
  000eae2c: ldrb.w r0,[r4,#0x63]
  000eae30: ldrb r1,[r6,#0x1a]
  000eae32: orrs r0,r1
  000eae34: bne 0x000eae66
  000eae36: ldr r0,[0x000eaf54]
  000eae38: add r0,pc
  000eae3a: ldr r0,[r0,#0x0]
  000eae3c: ldr r0,[r0,#0x0]
  000eae3e: blx 0x00075580
  000eae42: cbz r0,0x000eae66
  000eae44: ldr r0,[0x000eaf58]
  000eae46: movw r1,#0x289
  000eae4a: ldr r5,[r4,#0x70]
  000eae4c: add r0,pc
  000eae4e: ldr r0,[r0,#0x0]
  000eae50: ldr r0,[r0,#0x0]
  000eae52: blx 0x00072f70
  000eae56: mov r1,r0
  000eae58: mov r0,r5
  000eae5a: blx 0x00074794
  000eae5e: movs r0,#0x1
  000eae60: strb r0,[r6,#0x1a]
  000eae62: strb.w r0,[r4,#0x63]
  000eae66: ldrb.w r0,[r4,#0x6a]
  000eae6a: cmp r0,#0x0
  000eae6c: bne.w 0x000eb4ce
  000eae70: ldrb.w r0,[r4,#0x63]
  000eae74: ldrb r1,[r6,#0x1b]
  000eae76: orrs r0,r1
  000eae78: bne 0x000eaeaa
  000eae7a: ldr r0,[0x000eaf5c]
  000eae7c: add r0,pc
  000eae7e: ldr r0,[r0,#0x0]
  000eae80: ldr r0,[r0,#0x0]
  000eae82: blx 0x00071a1c
  000eae86: cbz r0,0x000eaeaa
  000eae88: ldr r0,[0x000eaf60]
  000eae8a: movw r1,#0x28a
  000eae8e: ldr r5,[r4,#0x70]
  000eae90: add r0,pc
  000eae92: ldr r0,[r0,#0x0]
  000eae94: ldr r0,[r0,#0x0]
  000eae96: blx 0x00072f70
  000eae9a: mov r1,r0
  000eae9c: mov r0,r5
  000eae9e: blx 0x00074794
  000eaea2: movs r0,#0x1
  000eaea4: strb r0,[r6,#0x1b]
  000eaea6: strb.w r0,[r4,#0x63]
  000eaeaa: ldrb.w r0,[r4,#0x6a]
  000eaeae: cmp r0,#0x0
  000eaeb0: bne.w 0x000eb4ce
  000eaeb4: ldrb.w r0,[r4,#0x63]
  000eaeb8: cmp r0,#0x0
  000eaeba: beq.w 0x000eb468
  000eaebe: ldrb.w r0,[r4,#0x63]
  000eaec2: ldrb.w r1,[r6,#0x3a]
  000eaec6: orrs r0,r1
  000eaec8: bne.w 0x000eb4ce
  000eaecc: ldr r0,[0x000eaf64]
  000eaece: add r0,pc
  000eaed0: ldr r5,[r0,#0x0]
  000eaed2: ldr r0,[r5,#0x0]
  000eaed4: blx 0x00071770
  000eaed8: cmp r0,#0xa2
  000eaeda: blt.w 0x000eb4ce
  000eaede: ldr r0,[0x000eaf68]
  000eaee0: add r0,pc
  000eaee2: ldr.w r8,[r0,#0x0]
  000eaee6: ldr.w r0,[r8,#0x0]
  000eaeea: blx 0x00071a1c
  000eaeee: cbz r0,0x000eaf02
  000eaef0: ldr.w r0,[r8,#0x0]
  000eaef4: blx 0x00071a40
  000eaef8: cbz r0,0x000eaf02
  000eaefa: ldr r0,[r5,#0x0]
  000eaefc: blx 0x0007192c
  000eaf00: cbz r0,0x000eaf0e
  000eaf02: ldr r0,[r5,#0x0]
  000eaf04: blx 0x0007192c
  000eaf08: cmp r0,#0x0
  000eaf0a: beq.w 0x000eb4ce
  000eaf0e: ldr r0,[0x000eaf6c]
  000eaf10: mov.w r8,#0x1
  000eaf14: strb.w r8,[r6,#0x3a]
  000eaf18: movw r1,#0xca1
  000eaf1c: add r0,pc
  000eaf1e: ldr r5,[r4,#0x70]
  000eaf20: ldr r0,[r0,#0x0]
  000eaf22: ldr r0,[r0,#0x0]
  000eaf24: blx 0x00072f70
  000eaf28: mov r1,r0
  000eaf2a: mov r0,r5
  000eaf2c: blx 0x00074794
  000eaf30: mov r0,r4
  000eaf32: blx 0x00074f8c
  000eaf36: strb.w r8,[r4,#0x63]
  000eaf3a: b 0x000eb4ce
  000eaf70: ldrb.w r0,[r4,#0x6a]
  000eaf74: cmp r0,#0x0
  000eaf76: bne.w 0x000eb4e0
  000eaf7a: ldr r0,[0x000eaf94]
  000eaf7c: add r0,pc
  000eaf7e: ldr.w r8,[r0,#0x0]
  000eaf82: ldrb.w r0,[r4,#0x63]
  000eaf86: ldrb.w r1,[r8,#0x33]
  000eaf8a: orrs r0,r1
  000eaf8c: bne.w 0x000eb0b2
  000eaf90: b 0x000eaf98
  000eaf98: ldr r0,[0x000eafb4]
  000eaf9a: add r0,pc
  000eaf9c: ldr r5,[r0,#0x0]
  000eaf9e: ldr r0,[r5,#0x0]
  000eafa0: ldr r0,[r0,#0x0]
  000eafa2: ldr r0,[r0,#0x4]
  000eafa4: ldr r0,[r0,#0x18]
  000eafa6: blx 0x00071a04
  000eafaa: cmp r0,#0x0
  000eafac: beq.w 0x000eb0b2
  000eafb0: b 0x000eafb8
  000eafb8: ldr r0,[0x000eafe8]
  000eafba: mov.w r1,#0xca0
  000eafbe: add r0,pc
  000eafc0: ldr r6,[r0,#0x0]
  000eafc2: ldr r0,[r6,#0x0]
  000eafc4: blx 0x00072f70
  000eafc8: add.w r10,sp,#0x164
  000eafcc: mov r1,r0
  000eafce: movs r2,#0x0
  000eafd0: mov r0,r10
  000eafd2: blx 0x0006f028
  000eafd6: ldr.w r9,[r5,#0x0]
  000eafda: add r0,sp,#0x14c
  000eafdc: mov r1,r10
  000eafde: movs r2,#0x0
  000eafe0: blx 0x0006f028
  000eafe4: b 0x000eafec
  000eafec: ldr r0,[r5,#0x0]
  000eafee: ldr r6,[r6,#0x0]
  000eaff0: ldr r0,[r0,#0x0]
  000eaff2: ldr r0,[r0,#0x4]
  000eaff4: ldr r0,[r0,#0x18]
  000eaff6: blx 0x00074044
  000eaffa: addw r1,r0,#0x391
  000eaffe: mov r0,r6
  000eb000: blx 0x00072f70
  000eb004: mov r1,r0
  000eb006: add r0,sp,#0x140
  000eb008: movs r2,#0x0
  000eb00a: blx 0x0006f028
  000eb00e: ldr r1,[0x000eb01c]
  000eb010: add r1,pc
  000eb012: add r0,sp,#0x134
  000eb014: movs r2,#0x0
  000eb016: blx 0x0006ee18
  000eb01a: b 0x000eb020
  000eb020: str r0,[sp,#0x0]
  000eb022: add r0,sp,#0x158
  000eb024: add r2,sp,#0x14c
  000eb026: add r3,sp,#0x140
  000eb028: mov r1,r9
  000eb02a: blx 0x000739b4
  000eb02e: add r0,sp,#0x164
  000eb030: add r1,sp,#0x158
  000eb032: blx 0x0006f2b0
  000eb036: ldr r0,[0x000eb424]
  000eb038: add r0,pc
  000eb03a: ldr r6,[r0,#0x0]
  000eb03c: add r0,sp,#0x158
  000eb03e: blx r6
  000eb040: add r0,sp,#0x134
  000eb042: blx r6
  000eb044: add r0,sp,#0x140
  000eb046: blx r6
  000eb048: add r0,sp,#0x14c
  000eb04a: blx r6
  000eb04c: ldr.w r9,[r5,#0x0]
  000eb050: add r0,sp,#0x128
  000eb052: add r1,sp,#0x164
  000eb054: movs r2,#0x0
  000eb056: blx 0x0006f028
  000eb05a: ldr r0,[r5,#0x0]
  000eb05c: ldr r0,[r0,#0x0]
  000eb05e: ldr r0,[r0,#0x4]
  000eb060: ldr r1,[r0,#0x18]
  000eb062: add r0,sp,#0x11c
  000eb064: blx 0x0007405c
  000eb068: ldr r1,[0x000eb428]
  000eb06a: add r1,pc
  000eb06c: add r0,sp,#0x110
  000eb06e: movs r2,#0x0
  000eb070: blx 0x0006ee18
  000eb074: str r0,[sp,#0x0]
  000eb076: add r0,sp,#0x158
  000eb078: add r2,sp,#0x128
  000eb07a: add r3,sp,#0x11c
  000eb07c: mov r1,r9
  000eb07e: blx 0x000739b4
  000eb082: add r0,sp,#0x164
  000eb084: add r1,sp,#0x158
  000eb086: blx 0x0006f2b0
  000eb08a: add r0,sp,#0x158
  000eb08c: blx r6
  000eb08e: add r0,sp,#0x110
  000eb090: blx r6
  000eb092: add r0,sp,#0x11c
  000eb094: blx r6
  000eb096: add r0,sp,#0x128
  000eb098: blx r6
  000eb09a: ldr r0,[r4,#0x70]
  000eb09c: add r1,sp,#0x164
  000eb09e: blx 0x00074794
  000eb0a2: movs r0,#0x1
  000eb0a4: strb.w r0,[r4,#0x63]
  000eb0a8: strb.w r0,[r8,#0x33]
  000eb0ac: add r0,sp,#0x164
  000eb0ae: blx 0x0006ee30
  000eb0b2: ldrb.w r0,[r4,#0x69]
  000eb0b6: cmp r0,#0x0
  000eb0b8: bne.w 0x000eae1c
  000eb0bc: ldrb.w r0,[r4,#0x6a]
  000eb0c0: cmp r0,#0x0
  000eb0c2: bne.w 0x000eb4e0
  000eb0c6: ldrb.w r0,[r4,#0x63]
  000eb0ca: ldrb.w r1,[r8,#0x34]
  000eb0ce: orrs r0,r1
  000eb0d0: bne.w 0x000eb1d6
  000eb0d4: ldr r0,[0x000eb42c]
  000eb0d6: add r0,pc
  000eb0d8: ldr r5,[r0,#0x0]
  000eb0da: ldr r0,[r5,#0x0]
  000eb0dc: ldr r0,[r0,#0x0]
  000eb0de: ldr r0,[r0,#0x4]
  000eb0e0: ldr r0,[r0,#0x30]
  000eb0e2: blx 0x00071a04
  000eb0e6: cmp r0,#0x0
  000eb0e8: beq 0x000eb1d6
  000eb0ea: ldr r0,[0x000eb430]
  000eb0ec: mov.w r1,#0xca0
  000eb0f0: add r0,pc
  000eb0f2: ldr r6,[r0,#0x0]
  000eb0f4: ldr r0,[r6,#0x0]
  000eb0f6: blx 0x00072f70
  000eb0fa: add.w r10,sp,#0x164
  000eb0fe: mov r1,r0
  000eb100: movs r2,#0x0
  000eb102: mov r0,r10
  000eb104: blx 0x0006f028
  000eb108: ldr.w r9,[r5,#0x0]
  000eb10c: add r0,sp,#0x104
  000eb10e: mov r1,r10
  000eb110: movs r2,#0x0
  000eb112: blx 0x0006f028
  000eb116: ldr r0,[r5,#0x0]
  000eb118: ldr r6,[r6,#0x0]
  000eb11a: ldr r0,[r0,#0x0]
  000eb11c: ldr r0,[r0,#0x4]
  000eb11e: ldr r0,[r0,#0x30]
  000eb120: blx 0x00074044
  000eb124: addw r1,r0,#0x391
  000eb128: mov r0,r6
  000eb12a: blx 0x00072f70
  000eb12e: mov r1,r0
  000eb130: add r0,sp,#0xf8
  000eb132: movs r2,#0x0
  000eb134: blx 0x0006f028
  000eb138: ldr r1,[0x000eb434]
  000eb13a: add r1,pc
  000eb13c: add r0,sp,#0xec
  000eb13e: movs r2,#0x0
  000eb140: blx 0x0006ee18
  000eb144: str r0,[sp,#0x0]
  000eb146: add r0,sp,#0x158
  000eb148: add r2,sp,#0x104
  000eb14a: add r3,sp,#0xf8
  000eb14c: mov r1,r9
  000eb14e: blx 0x000739b4
  000eb152: add r0,sp,#0x164
  000eb154: add r1,sp,#0x158
  000eb156: blx 0x0006f2b0
  000eb15a: ldr r0,[0x000eb438]
  000eb15c: add r0,pc
  000eb15e: ldr r6,[r0,#0x0]
  000eb160: add r0,sp,#0x158
  000eb162: blx r6
  000eb164: add r0,sp,#0xec
  000eb166: blx r6
  000eb168: add r0,sp,#0xf8
  000eb16a: blx r6
  000eb16c: add r0,sp,#0x104
  000eb16e: blx r6
  000eb170: ldr.w r9,[r5,#0x0]
  000eb174: add r0,sp,#0xe0
  000eb176: add r1,sp,#0x164
  000eb178: movs r2,#0x0
  000eb17a: blx 0x0006f028
  000eb17e: ldr r0,[r5,#0x0]
  000eb180: ldr r0,[r0,#0x0]
  000eb182: ldr r0,[r0,#0x4]
  000eb184: ldr r1,[r0,#0x30]
  000eb186: add r0,sp,#0xd4
  000eb188: blx 0x0007405c
  000eb18c: ldr r1,[0x000eb43c]
  000eb18e: add r1,pc
  000eb190: add r0,sp,#0xc8
  000eb192: movs r2,#0x0
  000eb194: blx 0x0006ee18
  000eb198: str r0,[sp,#0x0]
  000eb19a: add r0,sp,#0x158
  000eb19c: add r2,sp,#0xe0
  000eb19e: add r3,sp,#0xd4
  000eb1a0: mov r1,r9
  000eb1a2: blx 0x000739b4
  000eb1a6: add r0,sp,#0x164
  000eb1a8: add r1,sp,#0x158
  000eb1aa: blx 0x0006f2b0
  000eb1ae: add r0,sp,#0x158
  000eb1b0: blx r6
  000eb1b2: add r0,sp,#0xc8
  000eb1b4: blx r6
  000eb1b6: add r0,sp,#0xd4
  000eb1b8: blx r6
  000eb1ba: add r0,sp,#0xe0
  000eb1bc: blx r6
  000eb1be: ldr r0,[r4,#0x70]
  000eb1c0: add r1,sp,#0x164
  000eb1c2: blx 0x00074794
  000eb1c6: movs r0,#0x1
  000eb1c8: strb.w r0,[r4,#0x63]
  000eb1cc: strb.w r0,[r8,#0x34]
  000eb1d0: add r0,sp,#0x164
  000eb1d2: blx 0x0006ee30
  000eb1d6: ldrb.w r0,[r4,#0x69]
  000eb1da: cmp r0,#0x0
  000eb1dc: bne.w 0x000eae1c
  000eb1e0: ldrb.w r0,[r4,#0x6a]
  000eb1e4: cmp r0,#0x0
  000eb1e6: bne.w 0x000eb4e0
  000eb1ea: ldrb.w r0,[r4,#0x63]
  000eb1ee: ldrb.w r1,[r8,#0x35]
  000eb1f2: orrs r0,r1
  000eb1f4: bne.w 0x000eb2fa
  000eb1f8: ldr r0,[0x000eb440]
  000eb1fa: add r0,pc
  000eb1fc: ldr r5,[r0,#0x0]
  000eb1fe: ldr r0,[r5,#0x0]
  000eb200: ldr r0,[r0,#0x0]
  000eb202: ldr r0,[r0,#0x4]
  000eb204: ldr r0,[r0,#0x48]
  000eb206: blx 0x00071a04
  000eb20a: cmp r0,#0x0
  000eb20c: beq 0x000eb2fa
  000eb20e: ldr r0,[0x000eb444]
  000eb210: mov.w r1,#0xca0
  000eb214: add r0,pc
  000eb216: ldr r6,[r0,#0x0]
  000eb218: ldr r0,[r6,#0x0]
  000eb21a: blx 0x00072f70
  000eb21e: add.w r10,sp,#0x164
  000eb222: mov r1,r0
  000eb224: movs r2,#0x0
  000eb226: mov r0,r10
  000eb228: blx 0x0006f028
  000eb22c: ldr.w r9,[r5,#0x0]
  000eb230: add r0,sp,#0xbc
  000eb232: mov r1,r10
  000eb234: movs r2,#0x0
  000eb236: blx 0x0006f028
  000eb23a: ldr r0,[r5,#0x0]
  000eb23c: ldr r6,[r6,#0x0]
  000eb23e: ldr r0,[r0,#0x0]
  000eb240: ldr r0,[r0,#0x4]
  000eb242: ldr r0,[r0,#0x48]
  000eb244: blx 0x00074044
  000eb248: addw r1,r0,#0x391
  000eb24c: mov r0,r6
  000eb24e: blx 0x00072f70
  000eb252: mov r1,r0
  000eb254: add r0,sp,#0xb0
  000eb256: movs r2,#0x0
  000eb258: blx 0x0006f028
  000eb25c: ldr r1,[0x000eb448]
  000eb25e: add r1,pc
  000eb260: add r0,sp,#0xa4
  000eb262: movs r2,#0x0
  000eb264: blx 0x0006ee18
  000eb268: str r0,[sp,#0x0]
  000eb26a: add r0,sp,#0x158
  000eb26c: add r2,sp,#0xbc
  000eb26e: add r3,sp,#0xb0
  000eb270: mov r1,r9
  000eb272: blx 0x000739b4
  000eb276: add r0,sp,#0x164
  000eb278: add r1,sp,#0x158
  000eb27a: blx 0x0006f2b0
  000eb27e: ldr r0,[0x000eb44c]
  000eb280: add r0,pc
  000eb282: ldr r6,[r0,#0x0]
  000eb284: add r0,sp,#0x158
  000eb286: blx r6
  000eb288: add r0,sp,#0xa4
  000eb28a: blx r6
  000eb28c: add r0,sp,#0xb0
  000eb28e: blx r6
  000eb290: add r0,sp,#0xbc
  000eb292: blx r6
  000eb294: ldr.w r9,[r5,#0x0]
  000eb298: add r0,sp,#0x98
  000eb29a: add r1,sp,#0x164
  000eb29c: movs r2,#0x0
  000eb29e: blx 0x0006f028
  000eb2a2: ldr r0,[r5,#0x0]
  000eb2a4: ldr r0,[r0,#0x0]
  000eb2a6: ldr r0,[r0,#0x4]
  000eb2a8: ldr r1,[r0,#0x48]
  000eb2aa: add r0,sp,#0x8c
  000eb2ac: blx 0x0007405c
  000eb2b0: ldr r1,[0x000eb450]
  000eb2b2: add r1,pc
  000eb2b4: add r0,sp,#0x80
  000eb2b6: movs r2,#0x0
  000eb2b8: blx 0x0006ee18
  000eb2bc: str r0,[sp,#0x0]
  000eb2be: add r0,sp,#0x158
  000eb2c0: add r2,sp,#0x98
  000eb2c2: add r3,sp,#0x8c
  000eb2c4: mov r1,r9
  000eb2c6: blx 0x000739b4
  000eb2ca: add r0,sp,#0x164
  000eb2cc: add r1,sp,#0x158
  000eb2ce: blx 0x0006f2b0
  000eb2d2: add r0,sp,#0x158
  000eb2d4: blx r6
  000eb2d6: add r0,sp,#0x80
  000eb2d8: blx r6
  000eb2da: add r0,sp,#0x8c
  000eb2dc: blx r6
  000eb2de: add r0,sp,#0x98
  000eb2e0: blx r6
  000eb2e2: ldr r0,[r4,#0x70]
  000eb2e4: add r1,sp,#0x164
  000eb2e6: blx 0x00074794
  000eb2ea: movs r0,#0x1
  000eb2ec: strb.w r0,[r4,#0x63]
  000eb2f0: strb.w r0,[r8,#0x35]
  000eb2f4: add r0,sp,#0x164
  000eb2f6: blx 0x0006ee30
  000eb2fa: ldrb.w r0,[r4,#0x69]
  000eb2fe: cmp r0,#0x0
  000eb300: bne.w 0x000eae1c
  000eb304: ldrb.w r0,[r4,#0x6a]
  000eb308: cmp r0,#0x0
  000eb30a: bne.w 0x000eb4e0
  000eb30e: ldrb.w r0,[r4,#0x63]
  000eb312: ldrb.w r1,[r8,#0x36]
  000eb316: orrs r0,r1
  000eb318: bne.w 0x000eae1c
  000eb31c: ldr r0,[0x000eb454]
  000eb31e: add r0,pc
  000eb320: ldr r5,[r0,#0x0]
  000eb322: ldr r0,[r5,#0x0]
  000eb324: ldr r0,[r0,#0x0]
  000eb326: ldr r0,[r0,#0x4]
  000eb328: ldr r0,[r0,#0x60]
  000eb32a: blx 0x00071a04
  000eb32e: cmp r0,#0x0
  000eb330: beq.w 0x000eae1c
  000eb334: ldr r0,[0x000eb458]
  000eb336: mov.w r1,#0xca0
  000eb33a: add r0,pc
  000eb33c: ldr r6,[r0,#0x0]
  000eb33e: ldr r0,[r6,#0x0]
  000eb340: blx 0x00072f70
  000eb344: add.w r10,sp,#0x164
  000eb348: mov r1,r0
  000eb34a: movs r2,#0x0
  000eb34c: mov r0,r10
  000eb34e: blx 0x0006f028
  000eb352: ldr.w r9,[r5,#0x0]
  000eb356: add r0,sp,#0x74
  000eb358: mov r1,r10
  000eb35a: movs r2,#0x0
  000eb35c: blx 0x0006f028
  000eb360: ldr r0,[r5,#0x0]
  000eb362: ldr r6,[r6,#0x0]
  000eb364: ldr r0,[r0,#0x0]
  000eb366: ldr r0,[r0,#0x4]
  000eb368: ldr r0,[r0,#0x60]
  000eb36a: blx 0x00074044
  000eb36e: addw r1,r0,#0x391
  000eb372: mov r0,r6
  000eb374: blx 0x00072f70
  000eb378: mov r1,r0
  000eb37a: add r0,sp,#0x68
  000eb37c: movs r2,#0x0
  000eb37e: blx 0x0006f028
  000eb382: ldr r1,[0x000eb45c]
  000eb384: add r1,pc
  000eb386: add r0,sp,#0x5c
  000eb388: movs r2,#0x0
  000eb38a: blx 0x0006ee18
  000eb38e: str r0,[sp,#0x0]
  000eb390: add r0,sp,#0x158
  000eb392: add r2,sp,#0x74
  000eb394: add r3,sp,#0x68
  000eb396: mov r1,r9
  000eb398: blx 0x000739b4
  000eb39c: add r0,sp,#0x164
  000eb39e: add r1,sp,#0x158
  000eb3a0: blx 0x0006f2b0
  000eb3a4: ldr r0,[0x000eb460]
  000eb3a6: add r0,pc
  000eb3a8: ldr r6,[r0,#0x0]
  000eb3aa: add r0,sp,#0x158
  000eb3ac: blx r6
  000eb3ae: add r0,sp,#0x5c
  000eb3b0: blx r6
  000eb3b2: add r0,sp,#0x68
  000eb3b4: blx r6
  000eb3b6: add r0,sp,#0x74
  000eb3b8: blx r6
  000eb3ba: ldr.w r9,[r5,#0x0]
  000eb3be: add r0,sp,#0x50
  000eb3c0: add r1,sp,#0x164
  000eb3c2: movs r2,#0x0
  000eb3c4: blx 0x0006f028
  000eb3c8: ldr r0,[r5,#0x0]
  000eb3ca: ldr r0,[r0,#0x0]
  000eb3cc: ldr r0,[r0,#0x4]
  000eb3ce: ldr r1,[r0,#0x60]
  000eb3d0: add r0,sp,#0x44
  000eb3d2: blx 0x0007405c
  000eb3d6: ldr r1,[0x000eb464]
  000eb3d8: add r1,pc
  000eb3da: add r0,sp,#0x38
  000eb3dc: movs r2,#0x0
  000eb3de: blx 0x0006ee18
  000eb3e2: str r0,[sp,#0x0]
  000eb3e4: add r0,sp,#0x158
  000eb3e6: add r2,sp,#0x50
  000eb3e8: add r3,sp,#0x44
  000eb3ea: mov r1,r9
  000eb3ec: blx 0x000739b4
  000eb3f0: add r0,sp,#0x164
  000eb3f2: add r1,sp,#0x158
  000eb3f4: blx 0x0006f2b0
  000eb3f8: add r0,sp,#0x158
  000eb3fa: blx r6
  000eb3fc: add r0,sp,#0x38
  000eb3fe: blx r6
  000eb400: add r0,sp,#0x44
  000eb402: blx r6
  000eb404: add r0,sp,#0x50
  000eb406: blx r6
  000eb408: ldr r0,[r4,#0x70]
  000eb40a: add r1,sp,#0x164
  000eb40c: blx 0x00074794
  000eb410: movs r0,#0x1
  000eb412: strb.w r0,[r4,#0x63]
  000eb416: strb.w r0,[r8,#0x36]
  000eb41a: add r0,sp,#0x164
  000eb41c: blx 0x0006ee30
  000eb420: b 0x000eae1c
  000eb468: ldr r0,[0x000eb7c8]
  000eb46a: movs r1,#0xe8
  000eb46c: add r0,pc
  000eb46e: ldr.w r9,[r0,#0x0]
  000eb472: ldr.w r0,[r9,#0x0]
  000eb476: blx 0x0007558c
  000eb47a: cbnz r0,0x000eb4c4
  000eb47c: ldr r0,[0x000eb7e0]
  000eb47e: add r0,pc
  000eb480: ldr r5,[r0,#0x0]
  000eb482: ldr r0,[r5,#0x0]
  000eb484: blx 0x00071a1c
  000eb488: cbz r0,0x000eb4c4
  000eb48a: ldr r0,[r5,#0x0]
  000eb48c: blx 0x00071a40
  000eb490: cbz r0,0x000eb4c4
  000eb492: ldr r0,[0x000eb7e4]
  000eb494: movw r1,#0x28b
  000eb498: ldr.w r8,[r4,#0x70]
  000eb49c: add r0,pc
  000eb49e: ldr r0,[r0,#0x0]
  000eb4a0: ldr r0,[r0,#0x0]
  000eb4a2: blx 0x00072f70
  000eb4a6: mov r1,r0
  000eb4a8: mov r0,r8
  000eb4aa: blx 0x00074794
  000eb4ae: ldr.w r0,[r9,#0x0]
  000eb4b2: movs r1,#0xe8
  000eb4b4: blx 0x00072730
  000eb4b8: mov r0,r4
  000eb4ba: blx 0x00074f8c
  000eb4be: movs r0,#0x1
  000eb4c0: strb.w r0,[r4,#0x63]
  000eb4c4: ldrb.w r0,[r4,#0x6a]
  000eb4c8: cmp r0,#0x0
  000eb4ca: beq.w 0x000eaebe
  000eb4ce: ldrb.w r0,[r4,#0x69]
  000eb4d2: cbnz r0,0x000eb4e0
  000eb4d4: ldrb.w r0,[r4,#0x6a]
  000eb4d8: cbnz r0,0x000eb4e0
  000eb4da: ldrb.w r0,[r4,#0x63]
  000eb4de: cbz r0,0x000eb4f6
  000eb4e0: ldr r0,[sp,#0x170]
  000eb4e2: ldr.w r1,[r11,#0x0]
  000eb4e6: subs r0,r1,r0
  000eb4e8: ittt eq
  000eb4ea: add.eq sp,#0x174
  000eb4ec: pop.eq.w {r8,r9,r10,r11}
  000eb4f0: pop.eq {r4,r5,r6,r7,pc}
  000eb4f2: blx 0x0006e824
  000eb4f6: ldr r0,[0x000eb7e8]
  000eb4f8: add r0,pc
  000eb4fa: ldr r5,[r0,#0x0]
  000eb4fc: ldr r0,[r5,#0x0]
  000eb4fe: ldr r1,[r0,#0x24]
  000eb500: cbz r1,0x000eb51e
  000eb502: ldrb.w r1,[r4,#0xdb]
  000eb506: cbnz r1,0x000eb51e
  000eb508: ldr r0,[r0,#0x30]
  000eb50a: cmp r0,#0x0
  000eb50c: bgt 0x000eb51e
  000eb50e: ldr r0,[0x000eb7ec]
  000eb510: add r0,pc
  000eb512: ldr r0,[r0,#0x0]
  000eb514: ldr r0,[r0,#0x0]
  000eb516: ldrb.w r0,[r0,#0x2ec]
  000eb51a: cmp r0,#0x0
  000eb51c: beq 0x000eb5b0
  000eb51e: ldr r0,[r5,#0x0]
  000eb520: ldrb.w r0,[r0,#0xf9]
  000eb524: cmp r0,#0x0
  000eb526: beq 0x000eb4e0
  000eb528: ldr r0,[0x000eb800]
  000eb52a: add r0,pc
  000eb52c: ldr r0,[r0,#0x0]
  000eb52e: ldr r0,[r0,#0x0]
  000eb530: ldrb.w r0,[r0,#0x2ec]
  000eb534: cmp r0,#0x0
  000eb536: bne 0x000eb4e0
  000eb538: ldr.w r0,[r4,#0x84]
  000eb53c: cbz r0,0x000eb54c
  000eb53e: blx 0x00075010
  000eb542: blx 0x0006eb48
  000eb546: movs r0,#0x0
  000eb548: str.w r0,[r4,#0x84]
  000eb54c: movs r0,#0x74
  000eb54e: blx 0x0006eb24
  000eb552: mov r8,r0
  000eb554: ldr r0,[0x000eb804]
  000eb556: add r0,pc
  000eb558: ldr r6,[r0,#0x0]
  000eb55a: ldr r0,[r6,#0x0]
  000eb55c: mov.w r1,#0x1ba
  000eb560: blx 0x00072f70
  000eb564: mov r9,r0
  000eb566: ldr r0,[r6,#0x0]
  000eb568: movw r1,#0x67e
  000eb56c: blx 0x00072f70
  000eb570: ldr r3,[0x000eb808]
  000eb572: mov r2,r0
  000eb574: add r3,pc
  000eb576: mov r0,r8
  000eb578: mov r1,r9
  000eb57a: blx 0x00074f20
  000eb57e: ldr r0,[0x000eb80c]
  000eb580: movs r6,#0x0
  000eb582: str.w r8,[r4,#0x84]
  000eb586: movw r1,#0x24d
  000eb58a: add r0,pc
  000eb58c: movs r2,#0x0
  000eb58e: movs r3,#0x0
  000eb590: str r6,[sp,#0x0]
  000eb592: ldr r0,[r0,#0x0]
  000eb594: ldr r0,[r0,#0x0]
  000eb596: blx 0x00071548
  000eb59a: movs r0,#0x1
  000eb59c: movw r1,#0x4e20
  000eb5a0: strb.w r0,[r4,#0x69]
  000eb5a4: ldr r0,[r5,#0x0]
  000eb5a6: strb.w r6,[r0,#0xf9]
  000eb5aa: blx 0x0007171c
  000eb5ae: b 0x000eb4e0
  000eb5b0: ldr.w r0,[r4,#0x84]
  000eb5b4: cbz r0,0x000eb5c4
  000eb5b6: blx 0x00075010
  000eb5ba: blx 0x0006eb48
  000eb5be: movs r0,#0x0
  000eb5c0: str.w r0,[r4,#0x84]
  000eb5c4: movs r0,#0x74
  000eb5c6: blx 0x0006eb24
  000eb5ca: mov r6,r0
  000eb5cc: ldr r0,[0x000eb7f0]
  000eb5ce: add r0,pc
  000eb5d0: ldr r0,[r0,#0x0]
  000eb5d2: ldr r0,[r0,#0x0]
  000eb5d4: movw r1,#0x139
  000eb5d8: blx 0x00072f70
  000eb5dc: mov r1,r0
  000eb5de: ldr r0,[r5,#0x0]
  000eb5e0: ldrd r2,r3,[r0,#0x24]
  000eb5e4: ldr r0,[r2,#0x4]
  000eb5e6: ldr r2,[r0,#0x0]
  000eb5e8: mov r0,r6
  000eb5ea: blx 0x00074f20
  000eb5ee: movs r0,#0x98
  000eb5f0: str.w r6,[r4,#0x84]
  000eb5f4: blx 0x0006eb24
  000eb5f8: ldr r1,[0x000eb7f4]
  000eb5fa: mov r8,r0
  000eb5fc: add r1,pc
  000eb5fe: add r0,sp,#0x2c
  000eb600: movs r2,#0x0
  000eb602: movs r6,#0x0
  000eb604: blx 0x0006ee18
  000eb608: ldr r0,[r5,#0x0]
  000eb60a: ldr r0,[r0,#0x2c]
  000eb60c: add r2,sp,#0x2c
  000eb60e: strd r6,r0,[sp,#0x0]
  000eb612: mov.w r9,#0x1
  000eb616: mov r0,r8
  000eb618: movs r1,#0x0
  000eb61a: movs r3,#0x0
  000eb61c: strd r9,r6,[sp,#0x8]
  000eb620: strd r6,r6,[sp,#0x10]
  000eb624: str r6,[sp,#0x18]
  000eb626: blx 0x00071b30
  000eb62a: add r0,sp,#0x2c
  000eb62c: blx 0x0006ee30
  000eb630: ldr r0,[0x000eb7f8]
  000eb632: mov r2,r8
  000eb634: ldr r1,[0x000eb7fc]
  000eb636: add r0,pc
  000eb638: add r1,pc
  000eb63a: ldr r0,[r0,#0x0]
  000eb63c: ldr r1,[r1,#0x0]
  000eb63e: ldr r6,[r0,#0x0]
  000eb640: ldr r0,[r1,#0x0]
  000eb642: movw r1,#0x139
  000eb646: blx 0x00074f2c
  000eb64a: mov r1,r0
  000eb64c: movs r0,#0x0
  000eb64e: str r0,[sp,#0x0]
  000eb650: mov r0,r6
  000eb652: movs r2,#0x0
  000eb654: movs r3,#0x0
  000eb656: blx 0x00071548
  000eb65a: mov r0,r8
  000eb65c: blx 0x00073450
  000eb660: blx 0x0006eb48
  000eb664: strb.w r9,[r4,#0xdb]
  000eb668: movs r1,#0x0
  000eb66a: strb.w r9,[r4,#0x69]
  000eb66e: ldr r0,[r5,#0x0]
  000eb670: blx 0x000743f8
  000eb674: b 0x000eb4e0
```
