# MGame::gameOverCheck
elf 0x180514 body 632
Sig: undefined __stdcall gameOverCheck(void)

## decompile
```c

/* MGame::gameOverCheck() */

void MGame::gameOverCheck(void)

{
  uint *puVar1;
  int iVar2;
  int in_r0;
  int iVar3;
  DialogueWindow *pDVar4;
  undefined4 *puVar5;
  String *pSVar6;
  uint uVar7;
  Objective *this;
  undefined4 uVar8;
  Level *pLVar9;
  uint *puVar10;
  int *piVar11;
  uint uVar12;
  int iVar13;
  Mission *pMVar14;
  float extraout_s0;
  float fVar15;
  float extraout_s0_00;
  float extraout_s0_01;
  
  iVar3 = PlayerEgo::getHitpoints();
  if (iVar3 < 1) {
    iVar3 = PlayerEgo::tryToStartEmergencySystem(*(PlayerEgo **)(in_r0 + 0x58));
    if (iVar3 != 0) {
      return;
    }
    iVar3 = PlayerEgo::isInWormhole();
    if (iVar3 == 0) {
      PlayerEgo::setTurretMode(SUB41(*(undefined4 *)(in_r0 + 0x58),0));
      LevelScript::resetCamera(*(Level **)(in_r0 + 0x7c));
      PlayerEgo::setFreeLookMode(*(PlayerEgo **)(in_r0 + 0x58),false);
      TargetFollowCamera::enableFirstPersonCam(*(TargetFollowCamera **)(in_r0 + 0xf4),false);
      PlayerEgo::hideShipForFirstPersonCameraView(SUB41(*(undefined4 *)(in_r0 + 0x58),0));
      *(undefined1 *)(in_r0 + 0x111) = 1;
      PlayerEgo::explode(*(PlayerEgo **)(in_r0 + 0x58));
      iVar13 = PlayerEgo::explosionEnded(*(PlayerEgo **)(in_r0 + 0x58));
      iVar3 = DAT_001907a0;
      if (iVar13 != 0) {
        *(undefined1 *)(in_r0 + 0x60) = 1;
        pSVar6 = (String *)GameText::getText(**(int **)(iVar3 + 0x19069c));
        AbyssEngine::String::operator=((String *)(in_r0 + 100),pSVar6);
      }
    }
    else {
      puVar5 = (undefined4 *)(DAT_0019079c + 0x190572);
      *(undefined1 *)(in_r0 + 0x60) = 1;
      pSVar6 = (String *)GameText::getText(*(int *)*puVar5);
      AbyssEngine::String::operator=((String *)(in_r0 + 100),pSVar6);
      *(undefined1 *)(in_r0 + 0x111) = 1;
    }
    if (*(char *)(in_r0 + 0x60) != '\0') {
      iVar3 = Status::getMission();
      if (iVar3 != 0) {
        pMVar14 = (Mission *)Status::getMission();
        iVar3 = Mission::isCampaignMission(pMVar14);
        if (iVar3 != 0) {
          iVar3 = Status::getCurrentCampaignMission();
          piVar11 = *(int **)(DAT_001907a8 + 0x1906de);
          if (iVar3 == *piVar11) {
            iVar13 = **(int **)(DAT_001907ac + 0x1906ea) + 1;
          }
          else {
            **(undefined4 **)(DAT_001907b0 + 0x190764) = 0;
            iVar13 = 1;
          }
          iVar2 = DAT_001907b4;
          *piVar11 = iVar3;
          **(int **)(iVar2 + 0x190770) = iVar13;
        }
      }
      uVar8 = RecordHandler::recordStoreRead
                        ((RecordHandler *)**(undefined4 **)(DAT_001907bc + 0x19077c),
                         **(int **)(DAT_001907b8 + 0x19077a));
      *(undefined4 *)(in_r0 + 0x1e8) = uVar8;
    }
  }
  iVar3 = Level::checkGameOver(*(int *)(in_r0 + 0x78));
  fVar15 = extraout_s0;
  if (iVar3 != 0) {
    if (*(DialogueWindow **)(in_r0 + 0x8c) == (DialogueWindow *)0x0) {
      pDVar4 = operator_new(0x74);
      DialogueWindow::DialogueWindow(pDVar4);
      pLVar9 = *(Level **)(in_r0 + 0x78);
      *(DialogueWindow **)(in_r0 + 0x8c) = pDVar4;
      if (pLVar9 != (Level *)0x0) goto LAB_001905a0;
    }
    else {
      iVar3 = DialogueWindow::hasLevel(*(DialogueWindow **)(in_r0 + 0x8c));
      if ((iVar3 == 0) && (pLVar9 = *(Level **)(in_r0 + 0x78), pLVar9 != (Level *)0x0)) {
        pDVar4 = *(DialogueWindow **)(in_r0 + 0x8c);
LAB_001905a0:
        DialogueWindow::setLevel(pDVar4,pLVar9);
      }
    }
    pMVar14 = *(Mission **)(in_r0 + 0x8c);
    iVar3 = Status::getMission();
    DialogueWindow::set(pMVar14,iVar3,2);
    *(undefined1 *)(in_r0 + 0x5e) = 1;
    fVar15 = (float)pauseSounds();
    *(undefined1 *)(in_r0 + 0x5d) = 1;
  }
  puVar1 = (uint *)(in_r0 + 0x48);
  if (*(int *)(in_r0 + 0x4c) < (int)(uint)(*puVar1 < 0x1389)) goto LAB_00190732;
  *puVar1 = 0;
  *(undefined4 *)(in_r0 + 0x4c) = 0;
  puVar10 = *(uint **)(in_r0 + 0x7c);
  uVar7 = *puVar10;
  if ((int)uVar7 < 1) goto LAB_00190732;
  uVar12 = puVar10[3];
  iVar3 = (int)uVar7 >> 0x1f;
  if (((int)((iVar3 - uVar12) - (uint)(uVar7 < puVar10[2])) < 0 ==
       (SBORROW4(iVar3,uVar12) != SBORROW4(iVar3 - uVar12,(uint)(uVar7 < puVar10[2])))) ||
     ((iVar3 = Status::getCurrentCampaignMission(), iVar3 != 0x2a &&
      ((this = *(Objective **)(*(int *)(in_r0 + 0x78) + 0x28), fVar15 = extraout_s0_00,
       this == (Objective *)0x0 ||
       (iVar3 = Objective::isSurvivalObjective(this), fVar15 = extraout_s0_01, iVar3 != 0))))))
  goto LAB_00190732;
  if (*(DialogueWindow **)(in_r0 + 0x8c) == (DialogueWindow *)0x0) {
    pDVar4 = operator_new(0x74);
    DialogueWindow::DialogueWindow(pDVar4);
    pLVar9 = *(Level **)(in_r0 + 0x78);
    *(DialogueWindow **)(in_r0 + 0x8c) = pDVar4;
    if (pLVar9 != (Level *)0x0) goto LAB_00190706;
  }
  else {
    iVar3 = DialogueWindow::hasLevel(*(DialogueWindow **)(in_r0 + 0x8c));
    if ((iVar3 == 0) && (pLVar9 = *(Level **)(in_r0 + 0x78), pLVar9 != (Level *)0x0)) {
      pDVar4 = *(DialogueWindow **)(in_r0 + 0x8c);
LAB_00190706:
      DialogueWindow::setLevel(pDVar4,pLVar9);
    }
  }
  pMVar14 = *(Mission **)(in_r0 + 0x8c);
  iVar3 = Status::getMission();
  DialogueWindow::set(pMVar14,iVar3,2);
  *(undefined2 *)(in_r0 + 0x5d) = 0x101;
  fVar15 = (float)pauseSounds();
LAB_00190732:
  iVar3 = DAT_001907c8;
  if (*(char *)(in_r0 + 0x60) != '\0') {
    *puVar1 = 0;
    *(undefined4 *)(in_r0 + 0x4c) = 0;
    *(undefined4 *)(in_r0 + 0x50) = 0;
    FModSound::play(**(int **)(iVar3 + 0x190746),(Vector *)0x25,(Vector *)0x0,fVar15);
  }
  return;
}

```

## target disasm
```
  00190514: push {r4,r5,r6,r7,lr}
  00190516: add r7,sp,#0xc
  00190518: push.w r8
  0019051c: sub sp,#0x8
  0019051e: mov r4,r0
  00190520: ldr r0,[r0,#0x58]
  00190522: blx 0x000724e4
  00190526: cmp r0,#0x0
  00190528: ble 0x0019054e
  0019052a: ldrd r0,r1,[r4,#0x78]
  0019052e: ldr r1,[r1,#0x8]
  00190530: blx 0x00078688
  00190534: cmp r0,#0x0
  00190536: beq 0x001905d2
  00190538: ldr.w r0,[r4,#0x8c]
  0019053c: cbz r0,0x0019058a
  0019053e: blx 0x00078658
  00190542: cbnz r0,0x001905a4
  00190544: ldr r1,[r4,#0x78]
  00190546: cbz r1,0x001905a4
  00190548: ldr.w r0,[r4,#0x8c]
  0019054c: b 0x001905a0
  0019054e: ldr r0,[r4,#0x58]
  00190550: blx 0x00078694
  00190554: cmp r0,#0x0
  00190556: bne.w 0x00190754
  0019055a: ldr r0,[r4,#0x58]
  0019055c: blx 0x00076c84
  00190560: cmp r0,#0x0
  00190562: beq 0x0019064c
  00190564: ldr r0,[0x0019079c]
  00190566: movs r6,#0x1
  00190568: mov r5,r4
  0019056a: movw r1,#0x13f
  0019056e: add r0,pc
  00190570: strb.w r6,[r5,#0x60]!
  00190574: ldr r0,[r0,#0x0]
  00190576: ldr r0,[r0,#0x0]
  00190578: blx 0x00072f70
  0019057c: mov r1,r0
  0019057e: adds r0,r5,#0x4
  00190580: blx 0x0006f2b0
  00190584: strb.w r6,[r5,#0xb1]
  00190588: b 0x001906ac
  0019058a: movs r0,#0x74
  0019058c: blx 0x0006eb24
  00190590: mov r5,r0
  00190592: blx 0x00075544
  00190596: ldr r1,[r4,#0x78]
  00190598: str.w r5,[r4,#0x8c]
  0019059c: cbz r1,0x001905a4
  0019059e: mov r0,r5
  001905a0: blx 0x00078664
  001905a4: ldr r0,[0x001907c0]
  001905a6: ldr.w r5,[r4,#0x8c]
  001905aa: add r0,pc
  001905ac: ldr r0,[r0,#0x0]
  001905ae: ldr r0,[r0,#0x0]
  001905b0: blx 0x00072850
  001905b4: mov r1,r0
  001905b6: mov r0,r5
  001905b8: movs r2,#0x2
  001905ba: mov.w r3,#0xffffffff
  001905be: blx 0x00075550
  001905c2: movs r5,#0x1
  001905c4: mov r0,r4
  001905c6: strb.w r5,[r4,#0x5e]
  001905ca: blx 0x0007822c
  001905ce: strb.w r5,[r4,#0x5d]
  001905d2: mov r6,r4
  001905d4: movw r2,#0x1389
  001905d8: ldr.w r0,[r6,#0x48]!
  001905dc: ldr r1,[r6,#0x4]
  001905de: subs r0,r0,r2
  001905e0: sbcs r0,r1,#0x0
  001905e4: blt.w 0x00190732
  001905e8: movs r0,#0x0
  001905ea: strd r0,r0,[r6,#0x0]
  001905ee: ldr r1,[r4,#0x7c]
  001905f0: ldr r0,[r1,#0x0]
  001905f2: cmp r0,#0x1
  001905f4: blt.w 0x00190732
  001905f8: ldrd r1,r2,[r1,#0x8]
  001905fc: asrs r3,r0,#0x1f
  001905fe: subs r0,r0,r1
  00190600: sbcs.w r0,r3,r2
  00190604: bge.w 0x00190732
  00190608: ldr r0,[0x001907c4]
  0019060a: add r0,pc
  0019060c: ldr.w r8,[r0,#0x0]
  00190610: ldr.w r0,[r8,#0x0]
  00190614: blx 0x00071770
  00190618: cmp r0,#0x2a
  0019061a: beq 0x00190630
  0019061c: ldr r0,[r4,#0x78]
  0019061e: ldr r0,[r0,#0x28]
  00190620: cmp r0,#0x0
  00190622: beq.w 0x00190732
  00190626: blx 0x000786a0
  0019062a: cmp r0,#0x0
  0019062c: bne.w 0x00190732
  00190630: ldr.w r0,[r4,#0x8c]
  00190634: cmp r0,#0x0
  00190636: beq 0x001906f0
  00190638: blx 0x00078658
  0019063c: cmp r0,#0x0
  0019063e: bne 0x0019070a
  00190640: ldr r1,[r4,#0x78]
  00190642: cmp r1,#0x0
  00190644: beq 0x0019070a
  00190646: ldr.w r0,[r4,#0x8c]
  0019064a: b 0x00190706
  0019064c: ldr r0,[r4,#0x58]
  0019064e: movs r1,#0x0
  00190650: blx 0x00072b08
  00190654: ldrd r1,r0,[r4,#0x78]
  00190658: blx 0x0007246c
  0019065c: ldr r0,[r4,#0x58]
  0019065e: movs r1,#0x0
  00190660: blx 0x00076b94
  00190664: ldr.w r0,[r4,#0xf4]
  00190668: movs r1,#0x0
  0019066a: blx 0x00076ba0
  0019066e: ldr r0,[r4,#0x58]
  00190670: movs r1,#0x0
  00190672: blx 0x00076bac
  00190676: ldr r0,[r4,#0x58]
  00190678: movs r6,#0x1
  0019067a: strb.w r6,[r4,#0x111]
  0019067e: blx 0x000786ac
  00190682: ldr r0,[r4,#0x58]
  00190684: blx 0x000786b8
  00190688: add.w r5,r4,#0x60
  0019068c: cbz r0,0x001906ac
  0019068e: ldr r0,[0x001907a0]
  00190690: movw r1,#0x13f
  00190694: strb.w r6,[r4,#0x60]
  00190698: add r0,pc
  0019069a: ldr r0,[r0,#0x0]
  0019069c: ldr r0,[r0,#0x0]
  0019069e: blx 0x00072f70
  001906a2: mov r1,r0
  001906a4: add.w r0,r4,#0x64
  001906a8: blx 0x0006f2b0
  001906ac: ldrb r0,[r5,#0x0]
  001906ae: cmp r0,#0x0
  001906b0: beq.w 0x0019052a
  001906b4: ldr r0,[0x001907a4]
  001906b6: add r0,pc
  001906b8: ldr r5,[r0,#0x0]
  001906ba: ldr r0,[r5,#0x0]
  001906bc: blx 0x00072850
  001906c0: cmp r0,#0x0
  001906c2: beq 0x00190772
  001906c4: ldr r0,[r5,#0x0]
  001906c6: blx 0x00072850
  001906ca: blx 0x0007372c
  001906ce: cmp r0,#0x0
  001906d0: beq 0x00190772
  001906d2: ldr r0,[r5,#0x0]
  001906d4: blx 0x00071770
  001906d8: ldr r1,[0x001907a8]
  001906da: add r1,pc
  001906dc: ldr r1,[r1,#0x0]
  001906de: ldr r2,[r1,#0x0]
  001906e0: cmp r0,r2
  001906e2: bne 0x0019075c
  001906e4: ldr r2,[0x001907ac]
  001906e6: add r2,pc
  001906e8: ldr r2,[r2,#0x0]
  001906ea: ldr r2,[r2,#0x0]
  001906ec: adds r2,#0x1
  001906ee: b 0x00190768
  001906f0: movs r0,#0x74
  001906f2: blx 0x0006eb24
  001906f6: mov r5,r0
  001906f8: blx 0x00075544
  001906fc: ldr r1,[r4,#0x78]
  001906fe: str.w r5,[r4,#0x8c]
  00190702: cbz r1,0x0019070a
  00190704: mov r0,r5
  00190706: blx 0x00078664
  0019070a: ldr.w r0,[r8,#0x0]
  0019070e: ldr.w r5,[r4,#0x8c]
  00190712: blx 0x00072850
  00190716: mov r1,r0
  00190718: mov r0,r5
  0019071a: movs r2,#0x2
  0019071c: mov.w r3,#0xffffffff
  00190720: blx 0x00075550
  00190724: movw r0,#0x101
  00190728: strh.w r0,[r4,#0x5d]
  0019072c: mov r0,r4
  0019072e: blx 0x0007822c
  00190732: ldrb.w r0,[r4,#0x60]
  00190736: cbz r0,0x00190754
  00190738: ldr r0,[0x001907c8]
  0019073a: movs r1,#0x0
  0019073c: strd r1,r1,[r6,#0x0]
  00190740: movs r2,#0x0
  00190742: add r0,pc
  00190744: str r1,[r4,#0x50]
  00190746: str r1,[sp,#0x0]
  00190748: movs r1,#0x25
  0019074a: ldr r0,[r0,#0x0]
  0019074c: movs r3,#0x0
  0019074e: ldr r0,[r0,#0x0]
  00190750: blx 0x00071548
  00190754: add sp,#0x8
  00190756: pop.w r8
  0019075a: pop {r4,r5,r6,r7,pc}
  0019075c: ldr r2,[0x001907b0]
  0019075e: movs r3,#0x0
  00190760: add r2,pc
  00190762: ldr r2,[r2,#0x0]
  00190764: str r3,[r2,#0x0]
  00190766: movs r2,#0x1
  00190768: ldr r3,[0x001907b4]
  0019076a: str r0,[r1,#0x0]
  0019076c: add r3,pc
  0019076e: ldr r3,[r3,#0x0]
  00190770: str r2,[r3,#0x0]
  00190772: ldr r0,[0x001907b8]
  00190774: ldr r1,[0x001907bc]
  00190776: add r0,pc
  00190778: add r1,pc
  0019077a: ldr r0,[r0,#0x0]
  0019077c: ldr r2,[r1,#0x0]
  0019077e: ldr r1,[r0,#0x0]
  00190780: ldr r0,[r2,#0x0]
  00190782: blx 0x0007483c
  00190786: str.w r0,[r4,#0x1e8]
  0019078a: b 0x0019052a
```
