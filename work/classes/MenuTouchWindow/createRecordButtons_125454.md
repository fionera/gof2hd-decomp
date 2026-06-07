# MenuTouchWindow::createRecordButtons
elf 0x125454 body 1386
Sig: undefined __stdcall createRecordButtons(bool param_1)

## decompile
```c

/* MenuTouchWindow::createRecordButtons(bool) */

void MenuTouchWindow::createRecordButtons(bool param_1)

{
  char cVar1;
  uint uVar2;
  int *piVar3;
  Array *pAVar4;
  Array<AbyssEngine::String*> *this;
  void *pvVar5;
  Array<Array<AbyssEngine::String*>*> *this_00;
  undefined4 *puVar6;
  int *piVar7;
  char *pcVar8;
  String *pSVar9;
  String *pSVar10;
  TouchButton *pTVar11;
  String *pSVar12;
  undefined4 extraout_r1;
  int iVar13;
  uint uVar14;
  int *piVar15;
  code *pcVar16;
  int iVar17;
  uint *puVar18;
  int *piVar19;
  float fVar20;
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  int local_3c;
  
  uVar2 = (uint)param_1;
  piVar3 = *(int **)(DAT_00135810 + 0x13546c);
  local_3c = *piVar3;
  pAVar4 = *(Array **)(uVar2 + 0x100);
  if (pAVar4 != (Array *)0x0) {
    iVar17 = 0;
    for (uVar14 = 0; uVar14 < *(uint *)pAVar4; uVar14 = uVar14 + 1) {
      if (*(Array **)(*(int *)(pAVar4 + 4) + iVar17) != (Array *)0x0) {
        ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(*(int *)(pAVar4 + 4) + iVar17));
        iVar13 = *(int *)(*(int *)(uVar2 + 0x100) + 4);
        this = *(Array<AbyssEngine::String*> **)(iVar13 + iVar17);
        if (this == (Array<AbyssEngine::String*> *)0x0) {
          puVar6 = (undefined4 *)(iVar13 + uVar14 * 4);
        }
        else {
          pvVar5 = (void *)Array<AbyssEngine::String*>::~Array(this);
          operator_delete(pvVar5);
          puVar6 = (undefined4 *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17);
        }
        *puVar6 = 0;
        pAVar4 = *(Array **)(uVar2 + 0x100);
      }
      iVar17 = iVar17 + 4;
    }
    ArrayReleaseClasses<Array<AbyssEngine::String*>*>(pAVar4);
    if (*(Array<Array<AbyssEngine::String*>*> **)(uVar2 + 0x100) !=
        (Array<Array<AbyssEngine::String*>*> *)0x0) {
      pvVar5 = (void *)Array<Array<AbyssEngine::String*>*>::~Array
                                 (*(Array<Array<AbyssEngine::String*>*> **)(uVar2 + 0x100));
      operator_delete(pvVar5);
    }
    *(undefined4 *)(uVar2 + 0x100) = 0;
  }
  this_00 = operator_new(0xc);
  Array<Array<AbyssEngine::String*>*>::Array(this_00);
  iVar17 = DAT_00135814;
  *(Array<Array<AbyssEngine::String*>*> **)(uVar2 + 0x100) = this_00;
  puVar18 = *(uint **)(iVar17 + 0x1354ec);
  ArraySetLength<Array<AbyssEngine::String*>*>(*puVar18,(Array *)this_00);
  iVar17 = 0;
  puVar6 = *(undefined4 **)(DAT_00135818 + 0x135504);
  piVar7 = *(int **)(DAT_0013581c + 0x135516);
  pcVar8 = (char *)(DAT_00135820 + 0x13551e);
  while( true ) {
    if ((int)*puVar18 <= iVar17) break;
    pvVar5 = operator_new(0xc);
    Array<AbyssEngine::String*>::Array();
    *(void **)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) = pvVar5;
    ArraySetLength<AbyssEngine::String*>
              (6,*(Array **)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4));
    AbyssEngine::String::String(aSStack_48);
    if ((*(int *)(uVar2 + 0xbc) == 0) ||
       (*(int *)(*(int *)(*(int *)(uVar2 + 0xbc) + 4) + iVar17 * 4) == 0)) {
      AbyssEngine::String::String(aSStack_54);
      Globals::longToTimeStringNoSeconds(CONCAT44(extraout_r1,*puVar6),(String *)0x0);
      pSVar9 = operator_new(0xc);
      AbyssEngine::String::String(pSVar9,aSStack_54,false);
      **(undefined4 **)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) = pSVar9;
      pSVar9 = operator_new(0xc);
      pSVar12 = (String *)GameText::getText(*piVar7);
      AbyssEngine::String::String(pSVar9,pSVar12,false);
      *(String **)(*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 4) =
           pSVar9;
      pSVar9 = operator_new(0xc);
      if (iVar17 == 0) {
        pSVar12 = (String *)GameText::getText(*piVar7);
        AbyssEngine::String::String(pSVar9,pSVar12,false);
      }
      else {
        AbyssEngine::String::String(pSVar9,pcVar8,false);
      }
      *(String **)(*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 8) =
           pSVar9;
      pSVar9 = operator_new(0xc);
      AbyssEngine::String::String(pSVar9,pcVar8,false);
      *(String **)(*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 0xc)
           = pSVar9;
      pSVar9 = operator_new(0xc);
      AbyssEngine::String::String(pSVar9,pcVar8,false);
      *(String **)
       (*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 0x10) = pSVar9;
      pSVar9 = operator_new(0xc);
      AbyssEngine::String::String(pSVar9,pcVar8,false);
    }
    else {
      AbyssEngine::String::String(aSStack_54);
      iVar13 = *(int *)(*(int *)(*(int *)(uVar2 + 0xbc) + 4) + iVar17 * 4);
      Globals::longToTimeStringNoSeconds(CONCAT44(iVar13,*puVar6),*(String **)(iVar13 + 0x10));
      pSVar9 = operator_new(0xc);
      AbyssEngine::String::String(pSVar9,aSStack_54,false);
      **(undefined4 **)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) = pSVar9;
      pSVar9 = operator_new(0xc);
      AbyssEngine::String::String
                (pSVar9,(String *)
                        (*(int *)(*(int *)(*(int *)(uVar2 + 0xbc) + 4) + iVar17 * 4) + 0x194),false)
      ;
      *(String **)(*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 4) =
           pSVar9;
      pSVar9 = operator_new(0xc);
      if (iVar17 == 0) {
        pSVar12 = (String *)GameText::getText(*piVar7);
        AbyssEngine::String::String(pSVar9,pSVar12,false);
      }
      else {
        AbyssEngine::String::String(pSVar9,(char *)(DAT_00135824 + 0x1355de),false);
      }
      *(String **)(*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 8) =
           pSVar9;
      pvVar5 = operator_new(0xc);
      Layout::formatCredits((int)pvVar5);
      *(void **)(*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 0xc) =
           pvVar5;
      pSVar12 = operator_new(0xc);
      pSVar10 = (String *)GameText::getText(*piVar7);
      AbyssEngine::String::String(aSStack_6c,(char *)(DAT_00135a64 + 0x135740),false);
      AbyssEngine::operator+(aSStack_60,pSVar10);
      AbyssEngine::String::String
                (aSStack_78,
                 *(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0xbc) + 4) + iVar17 * 4) + 0x20));
      AbyssEngine::operator+(pSVar12,aSStack_60);
      pcVar16 = *(code **)(DAT_00135a68 + 0x135782);
      *(String **)
       (*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 0x10) = pSVar12
      ;
      (*pcVar16)(aSStack_78);
      (*pcVar16)(aSStack_60);
      (*pcVar16)(aSStack_6c);
      pSVar9 = operator_new(0xc);
      fVar20 = *(float *)(*(int *)(*(int *)(*(int *)(uVar2 + 0xbc) + 4) + iVar17 * 4) + 0x11c);
      if (fVar20 <= 0.0) {
        pSVar12 = (String *)GameText::getText(*piVar7);
      }
      else if (fVar20 <= 0.5) {
        pSVar12 = (String *)GameText::getText(*piVar7);
      }
      else if (fVar20 <= 1.0) {
        pSVar12 = (String *)GameText::getText(*piVar7);
      }
      else {
        pSVar12 = (String *)GameText::getText(*piVar7);
      }
      AbyssEngine::String::String(pSVar9,pSVar12,false);
    }
    *(String **)(*(int *)(*(int *)(*(int *)(*(int *)(uVar2 + 0x100) + 4) + iVar17 * 4) + 4) + 0x14)
         = pSVar9;
    AbyssEngine::String::~String(aSStack_54);
    AbyssEngine::String::~String(aSStack_48);
    iVar17 = iVar17 + 1;
  }
  if (*(TouchButton **)(uVar2 + 0xc4) != (TouchButton *)0x0) {
    pvVar5 = (void *)TouchButton::~TouchButton(*(TouchButton **)(uVar2 + 0xc4));
    operator_delete(pvVar5);
    *(undefined4 *)(uVar2 + 0xc4) = 0;
  }
  piVar19 = *(int **)(DAT_00135a50 + 0x13587c);
  cVar1 = **(char **)(DAT_00135a54 + 0x13587e);
  iVar17 = *(int *)(*piVar19 + 0x108);
  pTVar11 = operator_new(200);
  if (cVar1 == '\0') {
    pSVar12 = (String *)GameText::getText(*piVar7);
    iVar13 = *piVar19;
    TouchButton::TouchButton
              (pTVar11,pSVar12,0,
               (**(int **)(DAT_00135a6c + 0x1358ee) - *(int *)(uVar2 + 0x198)) -
               *(int *)(iVar13 + 0x28),*(int *)(iVar13 + 0x20) + *(int *)(iVar13 + 0xc) + iVar17,
               *(int *)(iVar13 + 0x2a4),'\x12','\x04');
  }
  else {
    pSVar12 = (String *)GameText::getText(*piVar7);
    iVar13 = *piVar19;
    TouchButton::TouchButton
              (pTVar11,pSVar12,7,
               (**(int **)(DAT_00135a58 + 0x1358b8) - *(int *)(uVar2 + 0x198)) -
               *(int *)(iVar13 + 0x28),*(int *)(iVar13 + 0x20) + *(int *)(iVar13 + 0xc) + iVar17,
               '\x12');
  }
  *(TouchButton **)(uVar2 + 0xc4) = pTVar11;
  iVar13 = *piVar19;
  piVar15 = *(int **)(DAT_00135a5c + 0x135954);
  TouchButton::setPosition
            (pTVar11,(*piVar15 - *(int *)(uVar2 + 0x198)) - *(int *)(iVar13 + 0x28),
             (*(int *)(iVar13 + 0x70) + *(int *)(uVar2 + 0x1b4)) * *(int *)(uVar2 + 0x18c) +
             *(int *)(iVar13 + 0xc) + iVar17 + *(int *)(iVar13 + 0x20) + *(int *)(uVar2 + 0x194));
  if (*(TouchButton **)(uVar2 + 200) != (TouchButton *)0x0) {
    pvVar5 = (void *)TouchButton::~TouchButton(*(TouchButton **)(uVar2 + 200));
    operator_delete(pvVar5);
    *(undefined4 *)(uVar2 + 200) = 0;
  }
  pTVar11 = operator_new(200);
  pSVar12 = (String *)GameText::getText(*piVar7);
  iVar13 = *piVar19;
  TouchButton::TouchButton
            (pTVar11,pSVar12,7,(*piVar15 - *(int *)(uVar2 + 0x198)) - *(int *)(iVar13 + 0x28),
             *(int *)(iVar13 + 0xc) + iVar17 + *(int *)(iVar13 + 0x20),'\x12');
  *(TouchButton **)(uVar2 + 200) = pTVar11;
  if (*piVar3 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00135454: push {r4,r5,r6,r7,lr}
  00135456: add r7,sp,#0xc
  00135458: push {r7,r8,r9,r10,r11}
  0013545c: vpush {d8,d9}
  00135460: sub sp,#0x68
  00135462: str r1,[sp,#0x14]
  00135464: mov r9,r0
  00135466: ldr r0,[0x00135810]
  00135468: add r0,pc
  0013546a: ldr r0,[r0,#0x0]
  0013546c: str r0,[sp,#0x10]
  0013546e: ldr r0,[r0,#0x0]
  00135470: str r0,[sp,#0x64]
  00135472: ldr.w r0,[r9,#0x100]
  00135476: cbz r0,0x001354d4
  00135478: movs r5,#0x0
  0013547a: movs r6,#0x0
  0013547c: movs r4,#0x0
  0013547e: b 0x001354b6
  00135480: ldr r1,[r0,#0x4]
  00135482: ldr r1,[r1,r6]
  00135484: cbz r1,0x001354b2
  00135486: mov r0,r1
  00135488: blx 0x0006facc
  0013548c: ldr.w r0,[r9,#0x100]
  00135490: ldr r1,[r0,#0x4]
  00135492: ldr r0,[r1,r6]
  00135494: cbz r0,0x001354a8
  00135496: blx 0x0006f64c
  0013549a: blx 0x0006eb48
  0013549e: ldr.w r0,[r9,#0x100]
  001354a2: ldr r0,[r0,#0x4]
  001354a4: add r0,r6
  001354a6: b 0x001354ac
  001354a8: add.w r0,r1,r4, lsl #0x2
  001354ac: str r5,[r0,#0x0]
  001354ae: ldr.w r0,[r9,#0x100]
  001354b2: adds r6,#0x4
  001354b4: adds r4,#0x1
  001354b6: ldr r1,[r0,#0x0]
  001354b8: cmp r4,r1
  001354ba: bcc 0x00135480
  001354bc: blx 0x00076570
  001354c0: ldr.w r0,[r9,#0x100]
  001354c4: cbz r0,0x001354ce
  001354c6: blx 0x0007657c
  001354ca: blx 0x0006eb48
  001354ce: movs r0,#0x0
  001354d0: str.w r0,[r9,#0x100]
  001354d4: movs r0,#0xc
  001354d6: blx 0x0006eb24
  001354da: mov r5,r0
  001354dc: blx 0x00076588
  001354e0: ldr r0,[0x00135814]
  001354e2: mov r1,r5
  001354e4: str.w r5,[r9,#0x100]
  001354e8: add r0,pc
  001354ea: ldr r6,[r0,#0x0]
  001354ec: ldr r0,[r6,#0x0]
  001354ee: blx 0x00076594
  001354f2: ldr r0,[0x00135818]
  001354f4: vmov.f32 s16,0x3f000000
  001354f8: vmov.f32 s18,0x3f800000
  001354fc: add.w r8,sp,#0x58
  00135500: add r0,pc
  00135502: add.w r11,sp,#0x4c
  00135506: add r4,sp,#0x34
  00135508: mov.w r10,#0x0
  0013550c: ldr r0,[r0,#0x0]
  0013550e: str r0,[sp,#0x1c]
  00135510: ldr r0,[0x0013581c]
  00135512: add r0,pc
  00135514: ldr r0,[r0,#0x0]
  00135516: str r0,[sp,#0x20]
  00135518: ldr r0,[0x00135820]
  0013551a: add r0,pc
  0013551c: str r0,[sp,#0x24]
  0013551e: b 0x00135858
  00135520: movs r0,#0xc
  00135522: blx 0x0006eb24
  00135526: mov r5,r0
  00135528: blx 0x0006f628
  0013552c: ldr.w r0,[r9,#0x100]
  00135530: ldr r0,[r0,#0x4]
  00135532: str.w r5,[r0,r10,lsl #0x2]
  00135536: ldr.w r0,[r9,#0x100]
  0013553a: ldr r0,[r0,#0x4]
  0013553c: ldr.w r1,[r0,r10,lsl #0x2]
  00135540: movs r0,#0x6
  00135542: blx 0x0006fe08
  00135546: mov r0,r8
  00135548: blx 0x0006efbc
  0013554c: ldr.w r0,[r9,#0xbc]
  00135550: cmp r0,#0x0
  00135552: beq 0x001355e6
  00135554: ldr r0,[r0,#0x4]
  00135556: ldr.w r0,[r0,r10,lsl #0x2]
  0013555a: cmp r0,#0x0
  0013555c: beq 0x001355e6
  0013555e: mov r0,r11
  00135560: blx 0x0006efbc
  00135564: ldr.w r0,[r9,#0xbc]
  00135568: str r6,[sp,#0x18]
  0013556a: ldr r0,[r0,#0x4]
  0013556c: ldr.w r1,[r0,r10,lsl #0x2]
  00135570: ldr r0,[sp,#0x1c]
  00135572: ldrd r2,r3,[r1,#0x10]
  00135576: ldr r0,[r0,#0x0]
  00135578: str.w r11,[sp,#0x0]
  0013557c: blx 0x000765a0
  00135580: movs r0,#0xc
  00135582: blx 0x0006eb24
  00135586: mov r5,r0
  00135588: add r1,sp,#0x4c
  0013558a: movs r2,#0x0
  0013558c: blx 0x0006f028
  00135590: ldr.w r0,[r9,#0x100]
  00135594: ldr r0,[r0,#0x4]
  00135596: ldr.w r0,[r0,r10,lsl #0x2]
  0013559a: ldr r0,[r0,#0x4]
  0013559c: str r5,[r0,#0x0]
  0013559e: movs r0,#0xc
  001355a0: blx 0x0006eb24
  001355a4: mov r5,r0
  001355a6: ldr.w r0,[r9,#0xbc]
  001355aa: ldr r0,[r0,#0x4]
  001355ac: ldr.w r0,[r0,r10,lsl #0x2]
  001355b0: add.w r1,r0,#0x194
  001355b4: mov r0,r5
  001355b6: movs r2,#0x0
  001355b8: blx 0x0006f028
  001355bc: ldr.w r0,[r9,#0x100]
  001355c0: ldr r0,[r0,#0x4]
  001355c2: ldr.w r0,[r0,r10,lsl #0x2]
  001355c6: ldr r0,[r0,#0x4]
  001355c8: str r5,[r0,#0x4]
  001355ca: movs r0,#0xc
  001355cc: blx 0x0006eb24
  001355d0: mov r5,r0
  001355d2: cmp.w r10,#0x0
  001355d6: beq 0x001356d0
  001355d8: ldr r1,[0x00135824]
  001355da: add r1,pc
  001355dc: mov r0,r5
  001355de: movs r2,#0x0
  001355e0: blx 0x0006ee18
  001355e4: b 0x001356e6
  001355e6: mov r0,r11
  001355e8: blx 0x0006efbc
  001355ec: ldr r0,[sp,#0x1c]
  001355ee: ldr r0,[r0,#0x0]
  001355f0: movs r2,#0x0
  001355f2: movs r3,#0x0
  001355f4: str.w r11,[sp,#0x0]
  001355f8: blx 0x000765a0
  001355fc: movs r0,#0xc
  001355fe: blx 0x0006eb24
  00135602: mov r5,r0
  00135604: mov r1,r11
  00135606: movs r2,#0x0
  00135608: blx 0x0006f028
  0013560c: ldr.w r0,[r9,#0x100]
  00135610: ldr r0,[r0,#0x4]
  00135612: ldr.w r0,[r0,r10,lsl #0x2]
  00135616: ldr r0,[r0,#0x4]
  00135618: str r5,[r0,#0x0]
  0013561a: movs r0,#0xc
  0013561c: blx 0x0006eb24
  00135620: mov r5,r0
  00135622: ldr r0,[sp,#0x20]
  00135624: ldr r0,[r0,#0x0]
  00135626: movs r1,#0xae
  00135628: blx 0x00072f70
  0013562c: mov r1,r0
  0013562e: mov r0,r5
  00135630: movs r2,#0x0
  00135632: blx 0x0006f028
  00135636: ldr.w r0,[r9,#0x100]
  0013563a: ldr r0,[r0,#0x4]
  0013563c: ldr.w r0,[r0,r10,lsl #0x2]
  00135640: ldr r0,[r0,#0x4]
  00135642: str r5,[r0,#0x4]
  00135644: movs r0,#0xc
  00135646: blx 0x0006eb24
  0013564a: mov r5,r0
  0013564c: cmp.w r10,#0x0
  00135650: beq 0x0013565e
  00135652: ldr r1,[sp,#0x24]
  00135654: mov r0,r5
  00135656: movs r2,#0x0
  00135658: blx 0x0006ee18
  0013565c: b 0x00135674
  0013565e: ldr r0,[sp,#0x20]
  00135660: ldr r0,[r0,#0x0]
  00135662: mov.w r1,#0x1e6
  00135666: blx 0x00072f70
  0013566a: mov r1,r0
  0013566c: mov r0,r5
  0013566e: movs r2,#0x0
  00135670: blx 0x0006f028
  00135674: ldr.w r0,[r9,#0x100]
  00135678: ldr r0,[r0,#0x4]
  0013567a: ldr.w r0,[r0,r10,lsl #0x2]
  0013567e: ldr r0,[r0,#0x4]
  00135680: str r5,[r0,#0x8]
  00135682: movs r0,#0xc
  00135684: blx 0x0006eb24
  00135688: mov r5,r0
  0013568a: ldr r1,[sp,#0x24]
  0013568c: movs r2,#0x0
  0013568e: blx 0x0006ee18
  00135692: ldr.w r0,[r9,#0x100]
  00135696: ldr r0,[r0,#0x4]
  00135698: ldr.w r0,[r0,r10,lsl #0x2]
  0013569c: ldr r0,[r0,#0x4]
  0013569e: str r5,[r0,#0xc]
  001356a0: movs r0,#0xc
  001356a2: blx 0x0006eb24
  001356a6: mov r5,r0
  001356a8: ldr r1,[sp,#0x24]
  001356aa: movs r2,#0x0
  001356ac: blx 0x0006ee18
  001356b0: ldr.w r0,[r9,#0x100]
  001356b4: ldr r0,[r0,#0x4]
  001356b6: ldr.w r0,[r0,r10,lsl #0x2]
  001356ba: ldr r0,[r0,#0x4]
  001356bc: str r5,[r0,#0x10]
  001356be: movs r0,#0xc
  001356c0: blx 0x0006eb24
  001356c4: mov r5,r0
  001356c6: ldr r1,[sp,#0x24]
  001356c8: movs r2,#0x0
  001356ca: blx 0x0006ee18
  001356ce: b 0x0013583a
  001356d0: ldr r0,[sp,#0x20]
  001356d2: ldr r0,[r0,#0x0]
  001356d4: mov.w r1,#0x1e6
  001356d8: blx 0x00072f70
  001356dc: mov r1,r0
  001356de: mov r0,r5
  001356e0: movs r2,#0x0
  001356e2: blx 0x0006f028
  001356e6: ldr.w r0,[r9,#0x100]
  001356ea: ldr r0,[r0,#0x4]
  001356ec: ldr.w r0,[r0,r10,lsl #0x2]
  001356f0: ldr r0,[r0,#0x4]
  001356f2: str r5,[r0,#0x8]
  001356f4: movs r0,#0xc
  001356f6: blx 0x0006eb24
  001356fa: mov r5,r0
  001356fc: ldr.w r0,[r9,#0xbc]
  00135700: ldr r1,[0x00135a60]
  00135702: add r1,pc
  00135704: ldr r0,[r0,#0x4]
  00135706: ldr r1,[r1,#0x0]
  00135708: ldr.w r0,[r0,r10,lsl #0x2]
  0013570c: ldr r1,[r1,#0x0]
  0013570e: ldr r2,[r0,#0x8]
  00135710: mov r0,r5
  00135712: blx 0x00074e54
  00135716: ldr.w r0,[r9,#0x100]
  0013571a: ldr r0,[r0,#0x4]
  0013571c: ldr.w r0,[r0,r10,lsl #0x2]
  00135720: ldr r0,[r0,#0x4]
  00135722: str r5,[r0,#0xc]
  00135724: movs r0,#0xc
  00135726: blx 0x0006eb24
  0013572a: mov r8,r0
  0013572c: ldr r0,[sp,#0x20]
  0013572e: ldr r0,[r0,#0x0]
  00135730: movw r1,#0x141
  00135734: blx 0x00072f70
  00135738: ldr r1,[0x00135a64]
  0013573a: mov r5,r0
  0013573c: add r1,pc
  0013573e: mov r0,r4
  00135740: movs r2,#0x0
  00135742: mov r11,r4
  00135744: blx 0x0006ee18
  00135748: add r0,sp,#0x40
  0013574a: mov r1,r5
  0013574c: mov r2,r11
  0013574e: blx 0x0006ef98
  00135752: ldr.w r0,[r9,#0xbc]
  00135756: ldr r0,[r0,#0x4]
  00135758: ldr.w r0,[r0,r10,lsl #0x2]
  0013575c: ldr r1,[r0,#0x20]
  0013575e: add r6,sp,#0x28
  00135760: mov r0,r6
  00135762: blx 0x0006f658
  00135766: add r5,sp,#0x40
  00135768: mov r0,r8
  0013576a: mov r2,r6
  0013576c: mov r1,r5
  0013576e: blx 0x0006ef98
  00135772: ldr.w r0,[r9,#0x100]
  00135776: ldr r0,[r0,#0x4]
  00135778: ldr.w r0,[r0,r10,lsl #0x2]
  0013577c: ldr r1,[0x00135a68]
  0013577e: add r1,pc
  00135780: ldr r2,[r0,#0x4]
  00135782: mov r0,r6
  00135784: mov r6,r5
  00135786: ldr r5,[r1,#0x0]
  00135788: str.w r8,[r2,#0x10]
  0013578c: blx r5
  0013578e: mov r0,r6
  00135790: blx r5
  00135792: mov r0,r11
  00135794: blx r5
  00135796: movs r0,#0xc
  00135798: blx 0x0006eb24
  0013579c: mov r5,r0
  0013579e: ldr.w r0,[r9,#0xbc]
  001357a2: ldr r0,[r0,#0x4]
  001357a4: ldr.w r0,[r0,r10,lsl #0x2]
  001357a8: vldr.32 s0,[r0,#0x11c]
  001357ac: vcmpe.f32 s0,#0
  001357b0: vmrs apsr,fpscr
  001357b4: bls 0x001357e2
  001357b6: ldr r0,[sp,#0x20]
  001357b8: vcmpe.f32 s0,s16
  001357bc: ldr r6,[sp,#0x18]
  001357be: add.w r8,sp,#0x58
  001357c2: vmrs apsr,fpscr
  001357c6: bls 0x001357fc
  001357c8: ldr r0,[r0,#0x0]
  001357ca: vcmpe.f32 s0,s18
  001357ce: add.w r11,sp,#0x4c
  001357d2: vmrs apsr,fpscr
  001357d6: bls 0x00135828
  001357d8: movs r1,#0x19
  001357da: blx 0x00072f70
  001357de: mov r1,r0
  001357e0: b 0x00135832
  001357e2: ldr r0,[sp,#0x20]
  001357e4: ldr r0,[r0,#0x0]
  001357e6: movw r1,#0x207
  001357ea: blx 0x00072f70
  001357ee: mov r1,r0
  001357f0: ldr r6,[sp,#0x18]
  001357f2: add.w r8,sp,#0x58
  001357f6: add.w r11,sp,#0x4c
  001357fa: b 0x00135832
  001357fc: ldr r0,[r0,#0x0]
  001357fe: movw r1,#0x207
  00135802: blx 0x00072f70
  00135806: mov r1,r0
  00135808: add.w r11,sp,#0x4c
  0013580c: b 0x00135832
  00135828: movw r1,#0x207
  0013582c: blx 0x00072f70
  00135830: mov r1,r0
  00135832: mov r0,r5
  00135834: movs r2,#0x0
  00135836: blx 0x0006f028
  0013583a: ldr.w r0,[r9,#0x100]
  0013583e: ldr r0,[r0,#0x4]
  00135840: ldr.w r0,[r0,r10,lsl #0x2]
  00135844: ldr r0,[r0,#0x4]
  00135846: str r5,[r0,#0x14]
  00135848: mov r0,r11
  0013584a: blx 0x0006ee30
  0013584e: mov r0,r8
  00135850: blx 0x0006ee30
  00135854: add.w r10,r10,#0x1
  00135858: ldr r0,[r6,#0x0]
  0013585a: cmp r10,r0
  0013585c: blt.w 0x00135520
  00135860: ldr.w r0,[r9,#0xc4]
  00135864: cbz r0,0x00135874
  00135866: blx 0x00074d7c
  0013586a: blx 0x0006eb48
  0013586e: movs r0,#0x0
  00135870: str.w r0,[r9,#0xc4]
  00135874: ldr r0,[0x00135a50]
  00135876: ldr r1,[0x00135a54]
  00135878: add r0,pc
  0013587a: add r1,pc
  0013587c: ldr.w r11,[r0,#0x0]
  00135880: ldr r0,[r1,#0x0]
  00135882: ldr.w r1,[r11,#0x0]
  00135886: ldrb r4,[r0,#0x0]
  00135888: movs r0,#0xc8
  0013588a: ldr.w r5,[r1,#0x108]
  0013588e: blx 0x0006eb24
  00135892: mov r8,r0
  00135894: ldr r0,[sp,#0x20]
  00135896: ldr r2,[sp,#0x14]
  00135898: movw r1,#0x1f9
  0013589c: ldr r0,[r0,#0x0]
  0013589e: cmp r2,#0x0
  001358a0: it ne
  001358a2: mov.ne.w r1,#0x1fa
  001358a6: cbz r4,0x001358de
  001358a8: blx 0x00072f70
  001358ac: mov r1,r0
  001358ae: ldr r0,[0x00135a58]
  001358b0: ldr.w r2,[r11,#0x0]
  001358b4: add r0,pc
  001358b6: ldr.w r3,[r9,#0x198]
  001358ba: ldr r0,[r0,#0x0]
  001358bc: ldr r4,[r2,#0xc]
  001358be: ldr r6,[r2,#0x20]
  001358c0: ldr r0,[r0,#0x0]
  001358c2: ldr r2,[r2,#0x28]
  001358c4: subs r0,r0,r3
  001358c6: add r4,r5
  001358c8: subs r3,r0,r2
  001358ca: mov.w r12,#0x12
  001358ce: mov r0,r8
  001358d0: movs r2,#0x7
  001358d2: add r6,r4
  001358d4: strd r6,r12,[sp,#0x0]
  001358d8: blx 0x00074578
  001358dc: b 0x00135920
  001358de: blx 0x00072f70
  001358e2: mov r12,r0
  001358e4: ldr r0,[0x00135a6c]
  001358e6: ldr.w r2,[r11,#0x0]
  001358ea: add r0,pc
  001358ec: ldr.w r3,[r9,#0x198]
  001358f0: ldr r0,[r0,#0x0]
  001358f2: ldr r1,[r2,#0x28]
  001358f4: ldr r6,[r2,#0xc]
  001358f6: ldr r0,[r0,#0x0]
  001358f8: ldr r4,[r2,#0x20]
  001358fa: ldr.w r2,[r2,#0x2a4]
  001358fe: subs r0,r0,r3
  00135900: add r6,r5
  00135902: subs r3,r0,r1
  00135904: add r4,r6
  00135906: strd r4,r2,[sp,#0x0]
  0013590a: mov.w lr,#0x4
  0013590e: mov.w r10,#0x12
  00135912: mov r0,r8
  00135914: mov r1,r12
  00135916: movs r2,#0x0
  00135918: strd r10,lr,[sp,#0x8]
  0013591c: blx 0x00074f50
  00135920: add.w r0,r9,#0x198
  00135924: str.w r8,[r9,#0xc4]
  00135928: str r0,[sp,#0x24]
  0013592a: mov r10,r5
  0013592c: ldr.w r0,[r11,#0x0]
  00135930: ldr.w r6,[r9,#0x18c]
  00135934: ldr r1,[r0,#0xc]
  00135936: ldr r2,[r0,#0x20]
  00135938: add r1,r10
  0013593a: ldr.w r12,[r0,#0x28]
  0013593e: ldr r0,[r0,#0x70]
  00135940: add r1,r2
  00135942: ldrd r4,r5,[r9,#0x194]
  00135946: ldr.w r3,[r9,#0x1b4]
  0013594a: ldr r2,[0x00135a5c]
  0013594c: add r1,r4
  0013594e: add r0,r3
  00135950: add r2,pc
  00135952: ldr r4,[r2,#0x0]
  00135954: mla r2,r0,r6,r1
  00135958: ldr r3,[r4,#0x0]
  0013595a: subs r0,r3,r5
  0013595c: sub.w r1,r0,r12
  00135960: mov r0,r8
  00135962: blx 0x00076498
  00135966: ldr.w r0,[r9,#0xc8]
  0013596a: cbz r0,0x0013597a
  0013596c: blx 0x00074d7c
  00135970: blx 0x0006eb48
  00135974: movs r0,#0x0
  00135976: str.w r0,[r9,#0xc8]
  0013597a: movs r0,#0xc8
  0013597c: blx 0x0006eb24
  00135980: mov r5,r0
  00135982: ldr r0,[sp,#0x20]
  00135984: ldr r0,[r0,#0x0]
  00135986: movs r1,#0x41
  00135988: blx 0x00072f70
  0013598c: mov r1,r0
  0013598e: ldr.w r0,[r11,#0x0]
  00135992: ldr r2,[sp,#0x24]
  00135994: ldr r3,[r4,#0x0]
  00135996: ldr r6,[r0,#0xc]
  00135998: ldr r4,[r0,#0x20]
  0013599a: ldr r0,[r0,#0x28]
  0013599c: ldr r2,[r2,#0x0]
  0013599e: subs r2,r3,r2
  001359a0: add r6,r10
  001359a2: subs r3,r2,r0
  001359a4: mov.w r12,#0x12
  001359a8: mov r0,r5
  001359aa: movs r2,#0x7
  001359ac: add r6,r4
  001359ae: strd r6,r12,[sp,#0x0]
  001359b2: blx 0x00074578
  001359b6: str.w r5,[r9,#0xc8]
  001359ba: ldr r0,[sp,#0x64]
  001359bc: ldr r1,[sp,#0x10]
  001359be: ldr r1,[r1,#0x0]
  001359c0: subs r0,r1,r0
  001359c2: itttt eq
  001359c4: add.eq sp,#0x68
  001359c6: vpop.eq {d8,d9}
  001359ca: add.eq sp,#0x4
  001359cc: pop.eq.w {r8,r9,r10,r11}
  001359d0: it eq
  001359d2: pop.eq {r4,r5,r6,r7,pc}
  001359d4: blx 0x0006e824
```
