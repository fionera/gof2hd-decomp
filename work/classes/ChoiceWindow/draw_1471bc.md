# ChoiceWindow::draw
elf 0x1471bc body 680
Sig: undefined __stdcall draw(void)

## decompile
```c

/* ChoiceWindow::draw() */

void ChoiceWindow::draw(void)

{
  int *in_r0;
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  String *pSVar7;
  int iVar8;
  uint *puVar9;
  undefined4 *puVar10;
  int iVar11;
  String aSStack_58 [12];
  String aSStack_4c [12];
  float local_40;
  float local_3c;
  String aSStack_34 [12];
  int local_28;
  
  piVar1 = *(int **)(DAT_0015748c + 0x1571d0);
  puVar10 = *(undefined4 **)(DAT_00157490 + 0x1571d2);
  local_28 = *piVar1;
  Layout::drawMask();
  iVar6 = *in_r0;
  iVar11 = in_r0[1];
  iVar4 = in_r0[2];
  iVar8 = in_r0[3];
  uVar3 = *puVar10;
  uVar2 = AbyssEngine::String::String(aSStack_34,(String *)(in_r0 + 8),false);
  Layout::drawBox(uVar3,7,iVar6,iVar11,iVar4,iVar8,uVar2);
  AbyssEngine::String::~String(aSStack_34);
  puVar9 = *(uint **)(DAT_00157494 + 0x15721e);
  AbyssEngine::PaintCanvas::SetColor(*puVar9);
  if (in_r0[0xd] != -1) {
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar9,in_r0[0xd],*in_r0 + (in_r0[2] >> 1),
               (char)in_r0[1] + (char)in_r0[0x13] + '\x01','\x11');
    AbyssEngine::PaintCanvas::SetColor(*puVar9);
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar9,in_r0[0xe],*in_r0 + (in_r0[2] >> 1),(char)in_r0[1] + (char)in_r0[0x13],'\x11'
              );
    iVar4 = Status::hardCoreMode();
    if ((iVar4 == 0) &&
       (iVar4 = Achievements::isEliteMedal
                          ((Achievements *)**(undefined4 **)(DAT_001574a4 + 0x1572aa),in_r0[0xc]),
       iVar4 == 0)) {
      AbyssEngine::String::String(aSStack_4c,(char *)(DAT_001574a8 + 0x1572be),false);
      Layout::formatCredits((int)aSStack_58);
      AbyssEngine::operator+((String *)&local_40,aSStack_4c);
      AbyssEngine::String::~String(aSStack_58);
      AbyssEngine::String::~String(aSStack_4c);
      iVar6 = *in_r0;
      iVar8 = in_r0[2];
      uVar5 = *puVar9;
      pSVar7 = (String *)**(undefined4 **)(DAT_001574b0 + 0x1572f4);
      iVar4 = AbyssEngine::PaintCanvas::GetTextWidth(uVar5,pSVar7);
      AbyssEngine::PaintCanvas::DrawString
                (uVar5,pSVar7,(int)&local_40,(iVar6 + (iVar8 >> 1)) - iVar4 / 2,
                 (bool)(((char)in_r0[1] + (char)in_r0[3]) - (char)in_r0[0x14]));
      AbyssEngine::String::~String((String *)&local_40);
    }
    AbyssEngine::PaintCanvas::SetColor(*puVar9);
    uVar5 = *puVar9;
    iVar6 = *in_r0;
    iVar8 = in_r0[2];
    pSVar7 = (String *)**(undefined4 **)(DAT_001574b4 + 0x15734e);
    iVar4 = AbyssEngine::PaintCanvas::GetTextWidth(uVar5,pSVar7);
    AbyssEngine::PaintCanvas::DrawString
              (uVar5,pSVar7,(int)(in_r0 + 0xf),(iVar6 + (iVar8 >> 1)) - iVar4 / 2,
               (bool)((char)in_r0[1] + (char)in_r0[0x15]));
  }
  ScrollTouchWindow::draw((ScrollTouchWindow *)in_r0[7]);
  if (*(char *)((int)in_r0 + 0x59) != '\0') {
    if (in_r0[4] != 0) {
      TouchButton::draw();
    }
    if ((in_r0[5] != 0) && (TouchButton::draw(), in_r0[5] != 0)) {
      TouchButton::getPosition();
      *(int *)(*(int *)(DAT_001574b8 + 0x1573bc) + 8) = (int)local_40;
      TouchButton::getPosition();
      *(int *)(*(int *)(DAT_001574bc + 0x1573d6) + 8) = (int)local_3c;
    }
    if (in_r0[4] != 0) {
      TouchButton::getPosition();
      iVar4 = *(int *)(DAT_001574c0 + 0x1573f2);
      *(int *)(iVar4 + 0xc) = (int)local_40;
      TouchButton::getPosition();
      iVar6 = *(int *)(DAT_001574c4 + 0x15740c);
      *(int *)(iVar6 + 0xc) = (int)local_3c;
      if (in_r0[5] == 0) {
        TouchButton::getPosition();
        *(int *)(iVar4 + 8) = (int)local_40;
        TouchButton::getPosition();
        *(int *)(iVar6 + 8) = (int)local_3c;
      }
    }
    **(undefined4 **)(DAT_001574c8 + 0x157444) = 1;
    if (in_r0[6] != 0) {
      TouchButton::draw();
    }
  }
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001571bc: push {r4,r5,r6,r7,lr}
  001571be: add r7,sp,#0xc
  001571c0: push {r8,r9,r10,r11}
  001571c4: sub sp,#0x44
  001571c6: mov r4,r0
  001571c8: ldr r0,[0x0015748c]
  001571ca: ldr r1,[0x00157490]
  001571cc: add r0,pc
  001571ce: add r1,pc
  001571d0: ldr r0,[r0,#0x0]
  001571d2: ldr.w r10,[r1,#0x0]
  001571d6: str r0,[sp,#0xc]
  001571d8: ldr r1,[r0,#0x0]
  001571da: ldr.w r0,[r10,#0x0]
  001571de: str r1,[sp,#0x40]
  001571e0: blx 0x0007696c
  001571e4: add.w r1,r4,#0x20
  001571e8: add r0,sp,#0x34
  001571ea: movs r2,#0x0
  001571ec: ldrd r8,r11,[r4,#0x0]
  001571f0: ldrd r6,r9,[r4,#0x8]
  001571f4: ldr.w r5,[r10,#0x0]
  001571f8: blx 0x0006f028
  001571fc: strd r6,r9,[sp,#0x0]
  00157200: movs r1,#0x7
  00157202: str r0,[sp,#0x8]
  00157204: mov r0,r5
  00157206: mov r2,r8
  00157208: mov r3,r11
  0015720a: blx 0x0007462c
  0015720e: add r0,sp,#0x34
  00157210: blx 0x0006ee30
  00157214: ldr r0,[0x00157494]
  00157216: mov.w r1,#0xffffffff
  0015721a: add r0,pc
  0015721c: ldr.w r9,[r0,#0x0]
  00157220: ldr.w r0,[r9,#0x0]
  00157224: blx 0x0006fac0
  00157228: ldr r1,[r4,#0x34]
  0015722a: adds r0,r1,#0x1
  0015722c: beq.w 0x00157386
  00157230: ldrd lr,r3,[r4,#0x0]
  00157234: mov.w r12,#0x44
  00157238: ldr r5,[r4,#0x4c]
  0015723a: movs r2,#0x11
  0015723c: ldr r6,[r4,#0x8]
  0015723e: add r3,r5
  00157240: ldr.w r0,[r9,#0x0]
  00157244: strd r2,r12,[sp,#0x0]
  00157248: adds r3,#0x1
  0015724a: add.w r2,lr,r6, asr #0x1
  0015724e: blx 0x00071d70
  00157252: ldrd r1,r2,[r4,#0x2c]
  00157256: cmp r2,#0x24
  00157258: ldr.w r0,[r9,#0x0]
  0015725c: blt 0x00157264
  0015725e: ldr r2,[0x00157498]
  00157260: add r2,pc
  00157262: b 0x00157268
  00157264: ldr r2,[0x0015749c]
  00157266: add r2,pc
  00157268: ldr.w r1,[r2,r1,lsl #0x2]
  0015726c: blx 0x0006fac0
  00157270: ldrd lr,r3,[r4,#0x0]
  00157274: mov.w r12,#0x44
  00157278: ldr r6,[r4,#0x8]
  0015727a: movs r2,#0x11
  0015727c: ldr r5,[r4,#0x4c]
  0015727e: ldr r1,[r4,#0x38]
  00157280: ldr.w r0,[r9,#0x0]
  00157284: add r3,r5
  00157286: strd r2,r12,[sp,#0x0]
  0015728a: add.w r2,lr,r6, asr #0x1
  0015728e: blx 0x00071d70
  00157292: ldr r0,[0x001574a0]
  00157294: add r0,pc
  00157296: ldr r0,[r0,#0x0]
  00157298: ldr r0,[r0,#0x0]
  0015729a: blx 0x0007192c
  0015729e: cmp r0,#0x0
  001572a0: bne 0x00157336
  001572a2: ldr r0,[0x001574a4]
  001572a4: ldr r1,[r4,#0x30]
  001572a6: add r0,pc
  001572a8: ldr r0,[r0,#0x0]
  001572aa: ldr r0,[r0,#0x0]
  001572ac: blx 0x000756ac
  001572b0: cmp r0,#0x0
  001572b2: bne 0x00157336
  001572b4: ldr r1,[0x001574a8]
  001572b6: add r0,sp,#0x1c
  001572b8: movs r2,#0x0
  001572ba: add r1,pc
  001572bc: blx 0x0006ee18
  001572c0: ldr r0,[0x001574ac]
  001572c2: ldr r2,[r4,#0x2c]
  001572c4: add r0,pc
  001572c6: ldr.w r1,[r10,#0x0]
  001572ca: ldr.w r2,[r0,r2,lsl #0x2]
  001572ce: add r0,sp,#0x10
  001572d0: blx 0x00074e54
  001572d4: add r0,sp,#0x28
  001572d6: add r1,sp,#0x1c
  001572d8: add r2,sp,#0x10
  001572da: blx 0x0006ef98
  001572de: add r0,sp,#0x10
  001572e0: blx 0x0006ee30
  001572e4: add r0,sp,#0x1c
  001572e6: blx 0x0006ee30
  001572ea: ldr r0,[0x001574b0]
  001572ec: ldr.w r10,[r4,#0x0]
  001572f0: add r0,pc
  001572f2: ldr.w r11,[r4,#0x8]
  001572f6: ldr.w r6,[r9,#0x0]
  001572fa: ldr r0,[r0,#0x0]
  001572fc: ldr.w r8,[r0,#0x0]
  00157300: add r2,sp,#0x28
  00157302: mov r0,r6
  00157304: mov r1,r8
  00157306: blx 0x0006faa8
  0015730a: ldr r1,[r4,#0x4]
  0015730c: ldr r2,[r4,#0xc]
  0015730e: ldr r3,[r4,#0x50]
  00157310: add r1,r2
  00157312: movs r5,#0x0
  00157314: subs r1,r1,r3
  00157316: add.w r0,r0,r0, lsr #0x1f
  0015731a: strd r1,r5,[sp,#0x0]
  0015731e: add.w r1,r10,r11, asr #0x1
  00157322: sub.w r3,r1,r0, asr #0x1
  00157326: add r2,sp,#0x28
  00157328: mov r0,r6
  0015732a: mov r1,r8
  0015732c: blx 0x0006fe14
  00157330: add r0,sp,#0x28
  00157332: blx 0x0006ee30
  00157336: ldr.w r0,[r9,#0x0]
  0015733a: ldr r1,[0x00157488]
  0015733c: blx 0x0006fac0
  00157340: ldr r0,[0x001574b4]
  00157342: add.w r5,r4,#0x3c
  00157346: ldr.w r6,[r9,#0x0]
  0015734a: add r0,pc
  0015734c: mov r2,r5
  0015734e: ldr.w r10,[r4,#0x0]
  00157352: ldr r0,[r0,#0x0]
  00157354: ldr.w r11,[r4,#0x8]
  00157358: ldr.w r8,[r0,#0x0]
  0015735c: mov r0,r6
  0015735e: mov r1,r8
  00157360: blx 0x0006faa8
  00157364: ldr r1,[r4,#0x4]
  00157366: movs r3,#0x0
  00157368: ldr r2,[r4,#0x54]
  0015736a: add.w r0,r0,r0, lsr #0x1f
  0015736e: add r1,r2
  00157370: strd r1,r3,[sp,#0x0]
  00157374: add.w r1,r10,r11, asr #0x1
  00157378: mov r2,r5
  0015737a: sub.w r3,r1,r0, asr #0x1
  0015737e: mov r0,r6
  00157380: mov r1,r8
  00157382: blx 0x0006fe14
  00157386: ldr r0,[r4,#0x1c]
  00157388: blx 0x00075b80
  0015738c: ldrb.w r0,[r4,#0x59]
  00157390: cmp r0,#0x0
  00157392: beq 0x0015744e
  00157394: ldr r0,[r4,#0x10]
  00157396: cbz r0,0x0015739c
  00157398: blx 0x000746e0
  0015739c: ldr r0,[r4,#0x14]
  0015739e: cbz r0,0x001573da
  001573a0: blx 0x000746e0
  001573a4: ldr r1,[r4,#0x14]
  001573a6: cbz r1,0x001573da
  001573a8: add r0,sp,#0x28
  001573aa: blx 0x00074dd0
  001573ae: vldr.32 s0,[sp,#0x28]
  001573b2: ldr r0,[0x001574b8]
  001573b4: vcvt.s32.f32 s0,s0
  001573b8: add r0,pc
  001573ba: ldr r0,[r0,#0x0]
  001573bc: vstr.32 s0,[r0,#0x8]
  001573c0: add r0,sp,#0x28
  001573c2: ldr r1,[r4,#0x14]
  001573c4: blx 0x00074dd0
  001573c8: vldr.32 s0,[sp,#0x2c]
  001573cc: ldr r0,[0x001574bc]
  001573ce: vcvt.s32.f32 s0,s0
  001573d2: add r0,pc
  001573d4: ldr r0,[r0,#0x0]
  001573d6: vstr.32 s0,[r0,#0x8]
  001573da: ldr r1,[r4,#0x10]
  001573dc: cbz r1,0x0015743c
  001573de: add r0,sp,#0x28
  001573e0: blx 0x00074dd0
  001573e4: vldr.32 s0,[sp,#0x28]
  001573e8: ldr r0,[0x001574c0]
  001573ea: vcvt.s32.f32 s0,s0
  001573ee: add r0,pc
  001573f0: ldr r5,[r0,#0x0]
  001573f2: add r0,sp,#0x28
  001573f4: vstr.32 s0,[r5,#0xc]
  001573f8: ldr r1,[r4,#0x10]
  001573fa: blx 0x00074dd0
  001573fe: vldr.32 s0,[sp,#0x2c]
  00157402: ldr r0,[0x001574c4]
  00157404: vcvt.s32.f32 s0,s0
  00157408: add r0,pc
  0015740a: ldr r6,[r0,#0x0]
  0015740c: vstr.32 s0,[r6,#0xc]
  00157410: ldr r0,[r4,#0x14]
  00157412: cbnz r0,0x0015743c
  00157414: ldr r1,[r4,#0x10]
  00157416: add r0,sp,#0x28
  00157418: blx 0x00074dd0
  0015741c: vldr.32 s0,[sp,#0x28]
  00157420: add r0,sp,#0x28
  00157422: vcvt.s32.f32 s0,s0
  00157426: vstr.32 s0,[r5,#0x8]
  0015742a: ldr r1,[r4,#0x10]
  0015742c: blx 0x00074dd0
  00157430: vldr.32 s0,[sp,#0x2c]
  00157434: vcvt.s32.f32 s0,s0
  00157438: vstr.32 s0,[r6,#0x8]
  0015743c: ldr r0,[0x001574c8]
  0015743e: movs r1,#0x1
  00157440: add r0,pc
  00157442: ldr r0,[r0,#0x0]
  00157444: str r1,[r0,#0x0]
  00157446: ldr r0,[r4,#0x18]
  00157448: cbz r0,0x0015744e
  0015744a: blx 0x000746e0
  0015744e: ldr r0,[sp,#0x40]
  00157450: ldr r1,[sp,#0xc]
  00157452: ldr r1,[r1,#0x0]
  00157454: subs r0,r1,r0
  00157456: ittt eq
  00157458: add.eq sp,#0x44
  0015745a: pop.eq.w {r8,r9,r10,r11}
  0015745e: pop.eq {r4,r5,r6,r7,pc}
  00157460: blx 0x0006e824
```
