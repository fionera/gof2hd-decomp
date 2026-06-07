# HangarWindow::showFreeCreditsWindow
elf 0x14e32c body 366
Sig: undefined __thiscall showFreeCreditsWindow(HangarWindow * this)

## decompile
```c

/* HangarWindow::showFreeCreditsWindow() */

void __thiscall HangarWindow::showFreeCreditsWindow(HangarWindow *this)

{
  int iVar1;
  int *piVar2;
  String *pSVar3;
  int iVar4;
  code *pcVar5;
  int iVar6;
  char *pcVar7;
  int *piVar8;
  uint uVar9;
  ChoiceWindow *pCVar10;
  undefined4 *puVar11;
  uint *puVar12;
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar8 = *(int **)(DAT_0015e4c8 + 0x15e340);
  local_28 = *piVar8;
  iVar1 = AbyssEngine::ApplicationManager::GetApplicationData();
  *(undefined1 *)(iVar1 + 0x4c) = 0;
  iVar1 = AbyssEngine::ApplicationManager::GetApplicationData();
  *(undefined1 *)(iVar1 + 0x3d) = 1;
  pCVar10 = *(ChoiceWindow **)(this + 0x20);
  pcVar7 = (char *)(DAT_0015e4d0 + 0x15e36e);
  AbyssEngine::String::String(aSStack_34,pcVar7,false);
  AbyssEngine::String::String(aSStack_40,pcVar7,false);
  AbyssEngine::String::String(aSStack_4c,(char *)(DAT_0015e4d4 + 0x15e382),false);
  AbyssEngine::String::String(aSStack_58,(char *)(DAT_0015e4d8 + 0x15e390),false);
  piVar2 = *(int **)(DAT_0015e4dc + 0x15e3a0);
  pSVar3 = (String *)GameText::getText(*piVar2);
  ChoiceWindow::set(pCVar10,aSStack_34,aSStack_40,false,aSStack_4c,aSStack_58,pSVar3,-1,-1);
  pcVar5 = *(code **)(DAT_0015e4e0 + 0x15e3ca);
  (*pcVar5)(aSStack_58);
  (*pcVar5)(aSStack_4c);
  (*pcVar5)(aSStack_40);
  (*pcVar5)(aSStack_34);
  pCVar10 = *(ChoiceWindow **)(this + 0x20);
  iVar1 = TouchButton::getHeight(*(TouchButton **)(*(int *)(*(int *)(this + 0x24) + 4) + 0x48));
  ChoiceWindow::setHeight(pCVar10,iVar1 * 5);
  iVar1 = 0;
  puVar11 = *(undefined4 **)(DAT_0015e4e4 + 0x15e406);
  puVar12 = *(uint **)(DAT_0015e4e8 + 0x15e40c);
  for (iVar6 = 5; iVar6 != 0; iVar6 = iVar6 + -1) {
    pSVar3 = (String *)*puVar11;
    uVar9 = *puVar12;
    GameText::getText(*piVar2);
    iVar4 = AbyssEngine::PaintCanvas::GetTextWidth(uVar9,pSVar3);
    if (iVar1 < iVar4) {
      iVar1 = iVar4;
    }
  }
  iVar6 = TouchButton::getWidth();
  ChoiceWindow::setWidth
            (*(ChoiceWindow **)(this + 0x20),
             *(int *)(**(int **)(DAT_0015e4ec + 0x15e462) + 0x2c) +
             iVar6 + iVar1 + *(int *)(**(int **)(DAT_0015e4ec + 0x15e462) + 0x28) * 4);
  this[0xb0] = (HangarWindow)0x1;
  this[0x3c] = (HangarWindow)0x1;
  *(undefined2 *)(this + 0xae) = 0;
  if (*piVar8 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0015e32c: push {r4,r5,r6,r7,lr}
  0015e32e: add r7,sp,#0xc
  0015e330: push {r8,r9,r10,r11}
  0015e334: sub sp,#0x54
  0015e336: mov r9,r0
  0015e338: ldr r0,[0x0015e4c8]
  0015e33a: ldr r1,[0x0015e4cc]
  0015e33c: add r0,pc
  0015e33e: add r1,pc
  0015e340: ldr r6,[r0,#0x0]
  0015e342: ldr r4,[r1,#0x0]
  0015e344: ldr r1,[r6,#0x0]
  0015e346: ldr r0,[r4,#0x0]
  0015e348: str r1,[sp,#0x50]
  0015e34a: blx 0x00071704
  0015e34e: movs r1,#0x0
  0015e350: strb.w r1,[r0,#0x4c]
  0015e354: ldr r0,[r4,#0x0]
  0015e356: blx 0x00071704
  0015e35a: movs r1,#0x1
  0015e35c: movs r2,#0x0
  0015e35e: strb.w r1,[r0,#0x3d]
  0015e362: add r0,sp,#0x44
  0015e364: ldr.w r8,[r9,#0x20]
  0015e368: ldr r5,[0x0015e4d0]
  0015e36a: add r5,pc
  0015e36c: mov r1,r5
  0015e36e: blx 0x0006ee18
  0015e372: add r0,sp,#0x38
  0015e374: mov r1,r5
  0015e376: movs r2,#0x0
  0015e378: blx 0x0006ee18
  0015e37c: ldr r1,[0x0015e4d4]
  0015e37e: add r1,pc
  0015e380: add r4,sp,#0x2c
  0015e382: movs r2,#0x0
  0015e384: mov r0,r4
  0015e386: blx 0x0006ee18
  0015e38a: ldr r1,[0x0015e4d8]
  0015e38c: add r1,pc
  0015e38e: add r5,sp,#0x20
  0015e390: movs r2,#0x0
  0015e392: mov r0,r5
  0015e394: blx 0x0006ee18
  0015e398: str r6,[sp,#0x18]
  0015e39a: ldr r0,[0x0015e4dc]
  0015e39c: add r0,pc
  0015e39e: ldr r0,[r0,#0x0]
  0015e3a0: str r0,[sp,#0x1c]
  0015e3a2: ldr r0,[r0,#0x0]
  0015e3a4: movs r1,#0xaa
  0015e3a6: blx 0x00072f70
  0015e3aa: mov.w r1,#0xffffffff
  0015e3ae: strd r4,r5,[sp,#0x0]
  0015e3b2: strd r0,r1,[sp,#0x8]
  0015e3b6: add r2,sp,#0x38
  0015e3b8: str r1,[sp,#0x10]
  0015e3ba: add r1,sp,#0x44
  0015e3bc: mov r0,r8
  0015e3be: movs r3,#0x0
  0015e3c0: blx 0x00075574
  0015e3c4: ldr r0,[0x0015e4e0]
  0015e3c6: add r0,pc
  0015e3c8: ldr r4,[r0,#0x0]
  0015e3ca: add r0,sp,#0x20
  0015e3cc: blx r4
  0015e3ce: add r0,sp,#0x2c
  0015e3d0: blx r4
  0015e3d2: add r0,sp,#0x38
  0015e3d4: blx r4
  0015e3d6: add r0,sp,#0x44
  0015e3d8: blx r4
  0015e3da: ldr.w r0,[r9,#0x24]
  0015e3de: ldr.w r5,[r9,#0x20]
  0015e3e2: str.w r9,[sp,#0x14]
  0015e3e6: ldr r0,[r0,#0x4]
  0015e3e8: ldr r0,[r0,#0x48]
  0015e3ea: blx 0x00076f0c
  0015e3ee: add.w r1,r0,r0, lsl #0x2
  0015e3f2: mov r0,r5
  0015e3f4: blx 0x00076d98
  0015e3f8: ldr r0,[0x0015e4e4]
  0015e3fa: mov.w r8,#0x0
  0015e3fe: ldr r1,[0x0015e4e8]
  0015e400: movs r4,#0x5
  0015e402: add r0,pc
  0015e404: mov.w r11,#0x0
  0015e408: add r1,pc
  0015e40a: ldr.w r9,[r0,#0x0]
  0015e40e: ldr.w r10,[r1,#0x0]
  0015e412: b 0x0015e448
  0015e414: ldr r0,[sp,#0x1c]
  0015e416: add.w r1,r11,#0x70
  0015e41a: ldr.w r5,[r9,#0x0]
  0015e41e: cmp.w r11,#0x0
  0015e422: ldr.w r6,[r10,#0x0]
  0015e426: ldr r0,[r0,#0x0]
  0015e428: it eq
  0015e42a: movw.eq r1,#0xd48
  0015e42e: blx 0x00072f70
  0015e432: mov r2,r0
  0015e434: mov r0,r6
  0015e436: mov r1,r5
  0015e438: blx 0x0006faa8
  0015e43c: cmp r0,r8
  0015e43e: it gt
  0015e440: mov.gt r8,r0
  0015e442: subs r4,#0x1
  0015e444: add.w r11,r11,#0x1
  0015e448: cmp r4,#0x0
  0015e44a: bne 0x0015e414
  0015e44c: ldr r4,[sp,#0x14]
  0015e44e: ldr r0,[r4,#0x24]
  0015e450: ldr r0,[r0,#0x4]
  0015e452: ldr r0,[r0,#0x48]
  0015e454: blx 0x00074da0
  0015e458: ldr r1,[0x0015e4ec]
  0015e45a: add.w r3,r0,r8
  0015e45e: add r1,pc
  0015e460: ldr r1,[r1,#0x0]
  0015e462: ldr r1,[r1,#0x0]
  0015e464: ldrd r2,r1,[r1,#0x28]
  0015e468: ldr r0,[r4,#0x20]
  0015e46a: add.w r2,r3,r2, lsl #0x2
  0015e46e: add r1,r2
  0015e470: blx 0x0007723c
  0015e474: movs r0,#0x1
  0015e476: strb.w r0,[r4,#0xb0]
  0015e47a: strb.w r0,[r4,#0x3c]
  0015e47e: movs r0,#0x0
  0015e480: strh.w r0,[r4,#0xae]
  0015e484: ldr r0,[sp,#0x50]
  0015e486: ldr r1,[sp,#0x18]
  0015e488: ldr r1,[r1,#0x0]
  0015e48a: subs r0,r1,r0
  0015e48c: ittt eq
  0015e48e: add.eq sp,#0x54
  0015e490: pop.eq.w {r8,r9,r10,r11}
  0015e494: pop.eq {r4,r5,r6,r7,pc}
  0015e496: blx 0x0006e824
```
