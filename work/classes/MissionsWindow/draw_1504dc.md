# MissionsWindow::draw
elf 0x1504dc body 1148
Sig: undefined __thiscall draw(MissionsWindow * this)

## decompile
```c

/* MissionsWindow::draw() */

void __thiscall MissionsWindow::draw(MissionsWindow *this)

{
  String *pSVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  Mission *this_00;
  undefined4 *puVar5;
  Agent *this_01;
  int iVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  uint *puVar15;
  int iVar16;
  int iVar17;
  int *piVar18;
  undefined4 uVar19;
  int iVar20;
  int *piVar21;
  int iVar22;
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar8 = *(int **)(DAT_00160598 + 0x1604ee);
  local_28 = *piVar8;
  if (*(int *)(this + 0x40) == 1) {
    if (*piVar8 == local_28) {
      (*(code *)(DAT_001ac504 + 0x1ac508))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*(undefined4 *)(this + 0x10));
  }
  if (this[0x22] != (MissionsWindow)0x0) {
    if (*piVar8 == local_28) {
      (*(code *)(DAT_001ac274 + 0x1ac278))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*(undefined4 *)(this + 8));
  }
  puVar15 = *(uint **)(DAT_0016059c + 0x160542);
  AbyssEngine::PaintCanvas::SetColor(*puVar15);
  piVar21 = *(int **)(DAT_001605a0 + 0x160554);
  piVar18 = *(int **)(DAT_001605a4 + 0x160556);
  iVar9 = *piVar21;
  pSVar1 = (String *)GameText::getText(*piVar18);
  AbyssEngine::String::String(aSStack_34,pSVar1,false);
  Layout::drawHeader(iVar9,aSStack_34);
  AbyssEngine::String::~String(aSStack_34);
  uVar2 = *(undefined4 *)(DAT_001605a8 + 0x16058a);
  iVar9 = Status::wantedBoardAccessible();
  if (iVar9 != 0) {
    for (uVar10 = 0; uVar10 < **(uint **)(this + 0x14); uVar10 = uVar10 + 1) {
      TouchButton::draw();
    }
  }
  iVar11 = *piVar21;
  iVar9 = *(int *)(this + 0x30);
  iVar3 = *(int *)(this + 0x34);
  iVar6 = *(int *)(iVar11 + 0xc);
  iVar7 = *(int *)(iVar11 + 0x20);
  iVar12 = *(int *)(this + 0x38);
  iVar13 = *(int *)(iVar11 + 0x28);
  iVar16 = *(int *)(iVar11 + 0x2c);
  uVar19 = *(undefined4 *)(iVar11 + 0x5c);
  pSVar1 = (String *)GameText::getText(*piVar18);
  uVar4 = AbyssEngine::String::String(aSStack_40,pSVar1,false);
  Layout::drawBox(iVar11,1,iVar13 + iVar9,iVar3 + iVar6 + iVar7,(iVar12 >> 1) - (iVar16 + iVar13),
                  uVar19,uVar4,puVar15,uVar2);
  AbyssEngine::String::~String(aSStack_40);
  iVar9 = *(int *)(this + 0x30);
  iVar3 = *(int *)(this + 0x34);
  iVar6 = *(int *)(this + 0x38);
  iVar7 = *(int *)(this + 0x3c);
  iVar13 = *piVar21;
  iVar20 = *(int *)(iVar13 + 0xc);
  iVar11 = *(int *)(iVar13 + 0x10);
  iVar22 = *(int *)(iVar13 + 0x20);
  iVar12 = *(int *)(iVar13 + 0x24);
  iVar17 = *(int *)(iVar13 + 0x28);
  iVar16 = *(int *)(iVar13 + 0x2c);
  iVar14 = *(int *)(iVar13 + 0x5c);
  uVar2 = AbyssEngine::String::String(aSStack_4c,(char *)(DAT_00160998 + 0x16065e),false);
  Layout::drawBox(iVar13,5,iVar17 + iVar9,iVar3 + iVar20 + iVar22 + iVar14 + iVar16,
                  (iVar6 >> 1) - (iVar16 + iVar17),
                  ((iVar7 - (iVar22 + iVar20 + iVar14 + iVar16 * 2)) - iVar11) - iVar12,uVar2);
  AbyssEngine::String::~String(aSStack_4c);
  ScrollTouchWindow::draw(*(ScrollTouchWindow **)this);
  if (*(int *)(this + 0x24) != 0) {
    TouchButton::draw();
  }
  iVar9 = *(int *)(this + 0x30);
  iVar11 = *piVar21;
  iVar3 = *(int *)(this + 0x34);
  iVar6 = *(int *)(iVar11 + 0xc);
  iVar7 = *(int *)(iVar11 + 0x20);
  iVar12 = *(int *)(this + 0x38);
  iVar13 = *(int *)(iVar11 + 0x28);
  iVar16 = *(int *)(iVar11 + 0x2c);
  uVar4 = *(undefined4 *)(iVar11 + 0x5c);
  pSVar1 = (String *)GameText::getText(*piVar18);
  uVar2 = AbyssEngine::String::String(aSStack_58,pSVar1,false);
  Layout::drawBox(iVar11,1,iVar9 + (iVar12 >> 1) + iVar16,iVar3 + iVar6 + iVar7,
                  ((iVar12 >> 1) - iVar16) - iVar13,uVar4,uVar2);
  AbyssEngine::String::~String(aSStack_58);
  iVar9 = *(int *)(this + 0x30);
  iVar3 = *(int *)(this + 0x34);
  iVar13 = *piVar21;
  iVar22 = *(int *)(this + 0x38);
  iVar6 = *(int *)(this + 0x3c);
  iVar14 = *(int *)(iVar13 + 0xc);
  iVar7 = *(int *)(iVar13 + 0x10);
  iVar20 = *(int *)(iVar13 + 0x20);
  iVar11 = *(int *)(iVar13 + 0x24);
  iVar16 = *(int *)(iVar13 + 0x2c);
  iVar12 = *(int *)(iVar13 + 0x28);
  iVar17 = *(int *)(iVar13 + 0x5c);
  uVar2 = AbyssEngine::String::String(aSStack_64,(char *)(DAT_0016099c + 0x160752),false);
  Layout::drawBox(iVar13,5,iVar9 + (iVar22 >> 1) + iVar16,iVar3 + iVar16 + iVar14 + iVar20 + iVar17,
                  ((iVar22 >> 1) - iVar16) - iVar12,
                  ((iVar6 - (iVar14 + iVar16 * 2 + iVar20 + iVar17)) - iVar7) - iVar11,uVar2);
  AbyssEngine::String::~String(aSStack_64);
  this_00 = (Mission *)Status::getFreelanceMission();
  if (((this_00 != (Mission *)0x0) && (iVar9 = Mission::isEmpty(), iVar9 == 0)) &&
     (*(Array **)(this + 0x18) != (Array *)0x0)) {
    iVar9 = *piVar21;
    ImageFactory::drawChar
              ((ImageFactory *)**(undefined4 **)(DAT_001609a0 + 0x1607c6),*(Array **)(this + 0x18),
               *(int *)(this + 0x30) + (*(int *)(this + 0x38) >> 1) + *(int *)(iVar9 + 0x2c),
               *(int *)(iVar9 + 0x2c) + *(int *)(this + 0x34) + *(int *)(iVar9 + 0xc) +
               *(int *)(iVar9 + 0x20) + *(int *)(iVar9 + 0x5c),false);
    puVar5 = *(undefined4 **)(DAT_001609a4 + 0x1607fc);
    pSVar1 = (String *)*puVar5;
    uVar10 = *puVar15;
    Mission::getAgent(this_00);
    Agent::getName();
    iVar9 = *piVar21;
    AbyssEngine::PaintCanvas::DrawString
              (uVar10,pSVar1,(int)aSStack_70,
               *(int *)(this + 0x30) + (*(int *)(this + 0x38) >> 1) + *(int *)(iVar9 + 0x2d4) +
               *(int *)(iVar9 + 0x2c) * 2,
               (bool)((char)*(undefined4 *)(iVar9 + 0x5c) +
                     (char)*(undefined4 *)(iVar9 + 0xc) +
                     (char)*(undefined4 *)(this + 0x34) + (char)*(int *)(iVar9 + 0x2c) +
                     (char)*(undefined4 *)(iVar9 + 0x20)));
    AbyssEngine::String::~String(aSStack_70);
    pSVar1 = (String *)*puVar5;
    uVar10 = *puVar15;
    Mission::getAgent(this_00);
    Agent::getStationName();
    iVar9 = *piVar21;
    AbyssEngine::PaintCanvas::DrawString
              (uVar10,pSVar1,(int)aSStack_70,
               *(int *)(this + 0x30) + (*(int *)(this + 0x38) >> 1) + *(int *)(iVar9 + 0x2d4) +
               *(int *)(iVar9 + 0x2c) * 2,
               (bool)((char)*(undefined4 *)(iVar9 + 0x20) +
                      (char)*(undefined4 *)(this + 0x34) + (char)*(int *)(iVar9 + 0x2c) +
                      (char)*(undefined4 *)(iVar9 + 0xc) + (char)*(undefined4 *)(iVar9 + 0x5c) +
                     (char)*(undefined4 *)(iVar9 + 4)));
    AbyssEngine::String::~String(aSStack_70);
    pSVar1 = (String *)*puVar5;
    uVar10 = *puVar15;
    iVar9 = *piVar18;
    this_01 = (Agent *)Mission::getAgent(this_00);
    Agent::getMission(this_01);
    Mission::getType();
    iVar9 = GameText::getText(iVar9);
    iVar3 = *piVar21;
    AbyssEngine::PaintCanvas::DrawString
              (uVar10,pSVar1,iVar9,
               *(int *)(this + 0x30) + (*(int *)(this + 0x38) >> 1) + *(int *)(iVar3 + 0x2d4) +
               *(int *)(iVar3 + 0x2c) * 2,
               (bool)((char)*(undefined4 *)(iVar3 + 0x5c) +
                      (char)*(undefined4 *)(this + 0x34) + (char)*(int *)(iVar3 + 0x2c) +
                      (char)*(undefined4 *)(iVar3 + 0xc) + (char)*(undefined4 *)(iVar3 + 0x20) +
                     (char)*(undefined4 *)(iVar3 + 4) * '\x02'));
  }
  ScrollTouchWindow::draw(*(ScrollTouchWindow **)(this + 4));
  if (*(int *)(this + 0x2c) != 0) {
    TouchButton::draw();
  }
  if (*(int *)(this + 0x28) != 0) {
    TouchButton::draw();
  }
  Layout::drawFooter((Layout *)*piVar21);
  if ((this[0x21] != (MissionsWindow)0x0) || (this[0x20] != (MissionsWindow)0x0)) {
    ChoiceWindow::draw();
  }
  if (*piVar8 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001604dc: push {r4,r5,r6,r7,lr}
  001604de: add r7,sp,#0xc
  001604e0: push {r8,r9,r10,r11}
  001604e4: sub sp,#0x84
  001604e6: mov r4,r0
  001604e8: ldr r0,[0x00160598]
  001604ea: add r0,pc
  001604ec: ldr r2,[r0,#0x0]
  001604ee: ldr r0,[r2,#0x0]
  001604f0: str r0,[sp,#0x80]
  001604f2: ldr r0,[r4,#0x40]
  001604f4: cmp r0,#0x1
  001604f6: bne 0x00160514
  001604f8: ldr r0,[r4,#0x10]
  001604fa: ldr r1,[sp,#0x80]
  001604fc: ldr r2,[r2,#0x0]
  001604fe: subs r1,r2,r1
  00160500: itttt eq
  00160502: add.eq sp,#0x84
  00160504: pop.eq.w {r8,r9,r10,r11}
  00160508: pop.eq.w {r4,r5,r6,r7,lr}
  0016050c: b.eq.w 0x001ac4f8
  00160510: blx 0x0006e824
  00160514: ldrb.w r0,[r4,#0x22]
  00160518: cbz r0,0x00160536
  0016051a: ldr r0,[r4,#0x8]
  0016051c: ldr r1,[sp,#0x80]
  0016051e: ldr r2,[r2,#0x0]
  00160520: subs r1,r2,r1
  00160522: itttt eq
  00160524: add.eq sp,#0x84
  00160526: pop.eq.w {r8,r9,r10,r11}
  0016052a: pop.eq.w {r4,r5,r6,r7,lr}
  0016052e: b.eq.w 0x001ac268
  00160532: blx 0x0006e824
  00160536: str r2,[sp,#0x14]
  00160538: mov.w r1,#0xffffffff
  0016053c: ldr r0,[0x0016059c]
  0016053e: add r0,pc
  00160540: ldr.w r9,[r0,#0x0]
  00160544: ldr.w r0,[r9,#0x0]
  00160548: blx 0x0006fac0
  0016054c: ldr r0,[0x001605a0]
  0016054e: ldr r1,[0x001605a4]
  00160550: add r0,pc
  00160552: add r1,pc
  00160554: ldr.w r11,[r0,#0x0]
  00160558: ldr.w r10,[r1,#0x0]
  0016055c: movs r1,#0x81
  0016055e: ldr.w r5,[r11,#0x0]
  00160562: ldr.w r0,[r10,#0x0]
  00160566: blx 0x00072f70
  0016056a: add r6,sp,#0x74
  0016056c: mov r1,r0
  0016056e: movs r2,#0x0
  00160570: mov r0,r6
  00160572: blx 0x0006f028
  00160576: mov r0,r5
  00160578: mov r1,r6
  0016057a: blx 0x000746d4
  0016057e: add r0,sp,#0x74
  00160580: blx 0x0006ee30
  00160584: ldr r0,[0x001605a8]
  00160586: add r0,pc
  00160588: ldr r0,[r0,#0x0]
  0016058a: str r0,[sp,#0x10]
  0016058c: ldr r0,[r0,#0x0]
  0016058e: blx 0x00077290
  00160592: cbz r0,0x001605c0
  00160594: movs r5,#0x0
  00160596: b 0x001605b8
  001605ac: ldr r0,[r0,#0x4]
  001605ae: ldr.w r0,[r0,r5,lsl #0x2]
  001605b2: blx 0x000746e0
  001605b6: adds r5,#0x1
  001605b8: ldr r0,[r4,#0x14]
  001605ba: ldr r1,[r0,#0x0]
  001605bc: cmp r5,r1
  001605be: bcc 0x001605ac
  001605c0: ldr.w r5,[r11,#0x0]
  001605c4: ldr r0,[r4,#0x30]
  001605c6: str.w r9,[sp,#0xc]
  001605ca: str r0,[sp,#0x2c]
  001605cc: ldr r0,[r4,#0x34]
  001605ce: str r0,[sp,#0x28]
  001605d0: ldr.w r0,[r10,#0x0]
  001605d4: ldr r1,[r5,#0xc]
  001605d6: str.w r10,[sp,#0x30]
  001605da: str r1,[sp,#0x24]
  001605dc: ldr r1,[r5,#0x20]
  001605de: str r1,[sp,#0x20]
  001605e0: movw r1,#0x22b
  001605e4: ldr r6,[r4,#0x38]
  001605e6: ldrd r8,r9,[r5,#0x28]
  001605ea: ldr.w r10,[r5,#0x5c]
  001605ee: blx 0x00072f70
  001605f2: mov r1,r0
  001605f4: add r0,sp,#0x68
  001605f6: movs r2,#0x0
  001605f8: blx 0x0006f028
  001605fc: add.w r1,r9,r8
  00160600: rsb r1,r1,r6, asr #0x1
  00160604: strd r1,r10,[sp,#0x0]
  00160608: str r0,[sp,#0x8]
  0016060a: ldr r0,[sp,#0x2c]
  0016060c: add.w r2,r8,r0
  00160610: ldrd r1,r0,[sp,#0x24]
  00160614: add r0,r1
  00160616: ldr r1,[sp,#0x20]
  00160618: adds r3,r0,r1
  0016061a: mov r0,r5
  0016061c: movs r1,#0x1
  0016061e: blx 0x0007462c
  00160622: add r0,sp,#0x68
  00160624: blx 0x0006ee30
  00160628: ldr r0,[r4,#0x30]
  0016062a: movs r2,#0x0
  0016062c: str.w r11,[sp,#0x34]
  00160630: str r0,[sp,#0x2c]
  00160632: ldr r0,[r4,#0x34]
  00160634: str r0,[sp,#0x28]
  00160636: ldr r0,[r4,#0x38]
  00160638: str r0,[sp,#0x18]
  0016063a: ldr r0,[r4,#0x3c]
  0016063c: ldr.w r5,[r11,#0x0]
  00160640: str r0,[sp,#0x24]
  00160642: ldrd r10,r0,[r5,#0xc]
  00160646: str r0,[sp,#0x20]
  00160648: ldrd r11,r0,[r5,#0x20]
  0016064c: str r0,[sp,#0x1c]
  0016064e: add r0,sp,#0x5c
  00160650: ldrd r9,r6,[r5,#0x28]
  00160654: ldr.w r8,[r5,#0x5c]
  00160658: ldr r1,[0x00160998]
  0016065a: add r1,pc
  0016065c: blx 0x0006ee18
  00160660: add.w r1,r6,r9
  00160664: ldr r2,[sp,#0x18]
  00160666: ldr r3,[sp,#0x24]
  00160668: rsb r1,r1,r2, asr #0x1
  0016066c: add.w r2,r11,r10
  00160670: add r2,r8
  00160672: add.w r2,r2,r6, lsl #0x1
  00160676: subs r2,r3,r2
  00160678: ldr r3,[sp,#0x20]
  0016067a: subs r2,r2,r3
  0016067c: ldr r3,[sp,#0x1c]
  0016067e: subs r2,r2,r3
  00160680: strd r1,r2,[sp,#0x0]
  00160684: movs r1,#0x5
  00160686: str r0,[sp,#0x8]
  00160688: ldr r0,[sp,#0x2c]
  0016068a: add.w r2,r9,r0
  0016068e: ldr r0,[sp,#0x28]
  00160690: add r0,r10
  00160692: add r0,r11
  00160694: add r0,r8
  00160696: adds r3,r0,r6
  00160698: mov r0,r5
  0016069a: blx 0x0007462c
  0016069e: add r0,sp,#0x5c
  001606a0: blx 0x0006ee30
  001606a4: ldr r0,[r4,#0x0]
  001606a6: blx 0x00075b80
  001606aa: ldr r0,[r4,#0x24]
  001606ac: ldr.w r8,[sp,#0x30]
  001606b0: cmp r0,#0x0
  001606b2: it ne
  001606b4: blx.ne 0x000746e0
  001606b8: ldr.w r11,[sp,#0x34]
  001606bc: ldr r0,[r4,#0x30]
  001606be: str r0,[sp,#0x28]
  001606c0: ldr.w r5,[r11,#0x0]
  001606c4: ldr r0,[r4,#0x34]
  001606c6: str r0,[sp,#0x2c]
  001606c8: ldr r1,[r5,#0xc]
  001606ca: ldr.w r0,[r8,#0x0]
  001606ce: str r1,[sp,#0x24]
  001606d0: ldr r1,[r5,#0x20]
  001606d2: str r1,[sp,#0x20]
  001606d4: mov.w r1,#0x22c
  001606d8: ldr r6,[r4,#0x38]
  001606da: ldrd r8,r9,[r5,#0x28]
  001606de: ldr.w r10,[r5,#0x5c]
  001606e2: blx 0x00072f70
  001606e6: mov r1,r0
  001606e8: add r0,sp,#0x50
  001606ea: movs r2,#0x0
  001606ec: blx 0x0006f028
  001606f0: rsb r1,r9,r6, asr #0x1
  001606f4: sub.w r1,r1,r8
  001606f8: strd r1,r10,[sp,#0x0]
  001606fc: str r0,[sp,#0x8]
  001606fe: ldr r0,[sp,#0x28]
  00160700: ldr r1,[sp,#0x24]
  00160702: add.w r0,r0,r6, asr #0x1
  00160706: add.w r2,r0,r9
  0016070a: ldr r0,[sp,#0x2c]
  0016070c: add r0,r1
  0016070e: ldr r1,[sp,#0x20]
  00160710: adds r3,r0,r1
  00160712: mov r0,r5
  00160714: movs r1,#0x1
  00160716: blx 0x0007462c
  0016071a: add r0,sp,#0x50
  0016071c: blx 0x0006ee30
  00160720: ldr r0,[r4,#0x30]
  00160722: movs r2,#0x0
  00160724: str r0,[sp,#0x28]
  00160726: ldr r0,[r4,#0x34]
  00160728: str r0,[sp,#0x2c]
  0016072a: ldr.w r5,[r11,#0x0]
  0016072e: ldrd r11,r0,[r4,#0x38]
  00160732: str r0,[sp,#0x24]
  00160734: ldrd r8,r0,[r5,#0xc]
  00160738: str r0,[sp,#0x20]
  0016073a: ldrd r10,r0,[r5,#0x20]
  0016073e: ldr r6,[r5,#0x2c]
  00160740: str r0,[sp,#0x1c]
  00160742: ldr r0,[r5,#0x28]
  00160744: str r0,[sp,#0x18]
  00160746: add r0,sp,#0x44
  00160748: ldr.w r9,[r5,#0x5c]
  0016074c: ldr r1,[0x0016099c]
  0016074e: add r1,pc
  00160750: blx 0x0006ee18
  00160754: ldr r2,[sp,#0x18]
  00160756: rsb r1,r6,r11, asr #0x1
  0016075a: ldr r3,[sp,#0x24]
  0016075c: subs r1,r1,r2
  0016075e: add.w r2,r8,r6, lsl #0x1
  00160762: add r2,r10
  00160764: add r2,r9
  00160766: subs r2,r3,r2
  00160768: ldr r3,[sp,#0x20]
  0016076a: subs r2,r2,r3
  0016076c: ldr r3,[sp,#0x1c]
  0016076e: subs r2,r2,r3
  00160770: strd r1,r2,[sp,#0x0]
  00160774: movs r1,#0x5
  00160776: str r0,[sp,#0x8]
  00160778: ldr r0,[sp,#0x28]
  0016077a: add.w r0,r0,r11, asr #0x1
  0016077e: adds r2,r0,r6
  00160780: ldr r0,[sp,#0x2c]
  00160782: add r0,r6
  00160784: add r0,r8
  00160786: add r0,r10
  00160788: add.w r3,r0,r9
  0016078c: mov r0,r5
  0016078e: blx 0x0007462c
  00160792: add r0,sp,#0x44
  00160794: blx 0x0006ee30
  00160798: ldr r0,[sp,#0x10]
  0016079a: ldr r0,[r0,#0x0]
  0016079c: blx 0x00073444
  001607a0: mov r5,r0
  001607a2: cmp r0,#0x0
  001607a4: beq.w 0x00160922
  001607a8: mov r0,r5
  001607aa: blx 0x00072868
  001607ae: cmp r0,#0x0
  001607b0: bne.w 0x00160922
  001607b4: ldr r1,[r4,#0x18]
  001607b6: cmp r1,#0x0
  001607b8: beq.w 0x00160922
  001607bc: ldr r0,[0x001609a0]
  001607be: ldr.w r11,[sp,#0x34]
  001607c2: add r0,pc
  001607c4: ldr.w r2,[r11,#0x0]
  001607c8: ldr r0,[r0,#0x0]
  001607ca: ldr.w r12,[r2,#0xc]
  001607ce: ldr r0,[r0,#0x0]
  001607d0: ldr.w lr,[r2,#0x20]
  001607d4: ldr r3,[r2,#0x2c]
  001607d6: ldr.w r8,[r2,#0x5c]
  001607da: movs r2,#0x0
  001607dc: ldrd r10,r9,[r4,#0x30]
  001607e0: ldr r6,[r4,#0x38]
  001607e2: str r2,[sp,#0x0]
  001607e4: add.w r2,r10,r6, asr #0x1
  001607e8: add r2,r3
  001607ea: add r3,r9
  001607ec: add r3,r12
  001607ee: add r3,lr
  001607f0: add r3,r8
  001607f2: blx 0x00075b74
  001607f6: ldr r0,[0x001609a4]
  001607f8: add r0,pc
  001607fa: ldr r0,[r0,#0x0]
  001607fc: str r0,[sp,#0x2c]
  001607fe: ldr.w r8,[r0,#0x0]
  00160802: ldr r0,[sp,#0xc]
  00160804: ldr.w r9,[r0,#0x0]
  00160808: mov r0,r5
  0016080a: blx 0x00073438
  0016080e: mov r1,r0
  00160810: add r0,sp,#0x38
  00160812: blx 0x00071c2c
  00160816: ldr.w r0,[r11,#0x0]
  0016081a: ldrd r12,r2,[r4,#0x30]
  0016081e: ldr.w lr,[r4,#0x38]
  00160822: ldr.w r10,[r0,#0x2d4]
  00160826: ldr r1,[r0,#0xc]
  00160828: ldr r3,[r0,#0x20]
  0016082a: ldr r6,[r0,#0x2c]
  0016082c: ldr r0,[r0,#0x5c]
  0016082e: add r2,r6
  00160830: add r1,r2
  00160832: add r2,sp,#0x38
  00160834: add r1,r3
  00160836: add r0,r1
  00160838: str r0,[sp,#0x0]
  0016083a: movs r0,#0x0
  0016083c: mov r1,r8
  0016083e: str r0,[sp,#0x4]
  00160840: add.w r0,r12,lr, asr #0x1
  00160844: add r0,r10
  00160846: add.w r3,r0,r6, lsl #0x1
  0016084a: mov r0,r9
  0016084c: blx 0x0006fe14
  00160850: add r0,sp,#0x38
  00160852: blx 0x0006ee30
  00160856: ldr r0,[sp,#0x2c]
  00160858: ldr.w r8,[r0,#0x0]
  0016085c: ldr r0,[sp,#0xc]
  0016085e: ldr.w r9,[r0,#0x0]
  00160862: mov r0,r5
  00160864: blx 0x00073438
  00160868: mov r1,r0
  0016086a: add r0,sp,#0x38
  0016086c: blx 0x00074c14
  00160870: ldr.w r0,[r11,#0x0]
  00160874: ldrd r12,r2,[r4,#0x30]
  00160878: ldr.w lr,[r4,#0x38]
  0016087c: ldr.w r10,[r0,#0x2d4]
  00160880: ldr.w r11,[r0,#0x5c]
  00160884: ldr r3,[r0,#0x4]
  00160886: ldr r6,[r0,#0xc]
  00160888: ldr r1,[r0,#0x20]
  0016088a: ldr r0,[r0,#0x2c]
  0016088c: add r2,r0
  0016088e: add r2,r6
  00160890: add r1,r2
  00160892: add r2,sp,#0x38
  00160894: add r1,r11
  00160896: add r1,r3
  00160898: str r1,[sp,#0x0]
  0016089a: movs r1,#0x0
  0016089c: str r1,[sp,#0x4]
  0016089e: add.w r1,r12,lr, asr #0x1
  001608a2: add r1,r10
  001608a4: add.w r3,r1,r0, lsl #0x1
  001608a8: mov r0,r9
  001608aa: mov r1,r8
  001608ac: blx 0x0006fe14
  001608b0: add r0,sp,#0x38
  001608b2: blx 0x0006ee30
  001608b6: ldr r0,[sp,#0x2c]
  001608b8: ldr r0,[r0,#0x0]
  001608ba: str r0,[sp,#0x2c]
  001608bc: ldr r0,[sp,#0xc]
  001608be: ldr.w r8,[r0,#0x0]
  001608c2: ldr r0,[sp,#0x30]
  001608c4: ldr r6,[r0,#0x0]
  001608c6: mov r0,r5
  001608c8: blx 0x00073438
  001608cc: blx 0x00071bb4
  001608d0: blx 0x00072874
  001608d4: add.w r1,r0,#0x162
  001608d8: mov r0,r6
  001608da: blx 0x00072f70
  001608de: mov r12,r0
  001608e0: ldr r0,[sp,#0x34]
  001608e2: mov.w r11,#0x0
  001608e6: ldr r0,[r0,#0x0]
  001608e8: ldrd lr,r3,[r4,#0x30]
  001608ec: ldr.w r10,[r4,#0x38]
  001608f0: ldr.w r9,[r0,#0x2d4]
  001608f4: ldr r2,[r0,#0x5c]
  001608f6: ldr r1,[r0,#0x4]
  001608f8: ldr r5,[r0,#0xc]
  001608fa: ldr r6,[r0,#0x20]
  001608fc: ldr r0,[r0,#0x2c]
  001608fe: add r3,r0
  00160900: add r3,r5
  00160902: add r3,r6
  00160904: add r2,r3
  00160906: add.w r1,r2,r1, lsl #0x1
  0016090a: strd r1,r11,[sp,#0x0]
  0016090e: add.w r1,lr,r10, asr #0x1
  00160912: mov r2,r12
  00160914: add r1,r9
  00160916: add.w r3,r1,r0, lsl #0x1
  0016091a: ldr r1,[sp,#0x2c]
  0016091c: mov r0,r8
  0016091e: blx 0x0006fe14
  00160922: ldr r0,[r4,#0x4]
  00160924: blx 0x00075b80
  00160928: ldr r0,[r4,#0x2c]
  0016092a: ldr r5,[sp,#0x34]
  0016092c: cmp r0,#0x0
  0016092e: it ne
  00160930: blx.ne 0x000746e0
  00160934: ldr r0,[r4,#0x28]
  00160936: cbz r0,0x0016093c
  00160938: blx 0x000746e0
  0016093c: ldr r0,[r5,#0x0]
  0016093e: blx 0x00075b8c
  00160942: ldrb.w r0,[r4,#0x21]
  00160946: cbnz r0,0x0016094e
  00160948: ldrb.w r0,[r4,#0x20]
  0016094c: cbz r0,0x00160954
  0016094e: ldr r0,[r4,#0xc]
  00160950: blx 0x000746ec
  00160954: ldr r0,[sp,#0x80]
  00160956: ldr r1,[sp,#0x14]
  00160958: ldr r1,[r1,#0x0]
  0016095a: subs r0,r1,r0
  0016095c: ittt eq
  0016095e: add.eq sp,#0x84
  00160960: pop.eq.w {r8,r9,r10,r11}
  00160964: pop.eq {r4,r5,r6,r7,pc}
  00160966: blx 0x0006e824
  001ac4f8: bx pc
```
