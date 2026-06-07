# WantedWindow::draw
elf 0xe1838 body 1752
Sig: undefined __thiscall draw(WantedWindow * this)

## decompile
```c

/* WantedWindow::draw() */

void __thiscall WantedWindow::draw(WantedWindow *this)

{
  int iVar1;
  uint *puVar2;
  float fVar3;
  float fVar4;
  Layout *this_00;
  char *pcVar5;
  undefined4 *puVar6;
  uint uVar7;
  int *piVar8;
  String *pSVar9;
  undefined4 uVar10;
  int *piVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  String *pSVar17;
  code *pcVar18;
  int iVar19;
  int iVar20;
  int *piVar21;
  undefined4 uVar22;
  int iVar23;
  int iVar24;
  int iVar25;
  uint in_fpscr;
  int iVar26;
  int iVar27;
  float fVar28;
  bool bVar29;
  int local_a4;
  String aSStack_a0 [12];
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  int local_34;
  
  piVar11 = *(int **)(DAT_000f1ba8 + 0xf184e);
  local_34 = *piVar11;
  if (this[0x14] != (WantedWindow)0x0) {
    if (*piVar11 != local_34) {
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail(*(undefined4 *)(this + 4));
    }
    (*(code *)(DAT_001ac274 + 0x1ac278))();
    return;
  }
  piVar21 = *(int **)(DAT_000f1bac + 0xf188a);
  iVar1 = *piVar21;
  puVar2 = *(uint **)(DAT_000f1bb0 + 0xf18ac);
  AbyssEngine::PaintCanvas::EnableClip
            ((PaintCanvas *)*puVar2,*(int *)(this + 0x1c),
             *(int *)(this + 0x20) + *(int *)(iVar1 + 0xc) + *(int *)(iVar1 + 0x20) +
             *(int *)(iVar1 + 0x5c),*(int *)(this + 0x24),
             *(int *)(iVar1 + 0x2c) + *(int *)(this + 0xa8));
  fVar3 = (float)getRelativeScrollStartPos(this);
  iVar13 = *(int *)(this + 0xa8);
  fVar28 = (float)VectorSignedToFloat(iVar13,(byte)(in_fpscr >> 0x16) & 3);
  fVar4 = (float)getRelativeScrollHeight(this);
  iVar26 = (int)(fVar4 * fVar28);
  iVar27 = (int)(fVar3 * fVar28);
  iVar1 = iVar26;
  if (iVar26 < 1) {
    iVar1 = iVar27 + -1;
  }
  if (iVar1 < 0 == (iVar26 < 1 && SBORROW4(iVar27,1))) {
    this_00 = (Layout *)*piVar21;
    Layout::drawScrollBar
              (this_00,((*(int *)(this + 0x1c) + (*(int *)(this + 0x24) >> 1)) -
                       *(int *)(this_00 + 0x2c)) - *(int *)(this_00 + 0x48),
               *(int *)(this + 0x20) + *(int *)(this_00 + 0x2c) + *(int *)(this_00 + 0xc) +
               *(int *)(this_00 + 0x20) + *(int *)(this_00 + 0x5c),iVar13,iVar27,iVar26);
  }
  iVar1 = *piVar21;
  uVar12 = 0;
  local_a4 = *(int *)(this + 0x20) + *(int *)(iVar1 + 0xc) + *(int *)(iVar1 + 0x20) +
             *(int *)(iVar1 + 0x5c) + *(int *)(iVar1 + 0x2c) + *(int *)(this + 0x84);
  if (iVar26 < 1) {
    iVar13 = 0;
  }
  else {
    iVar13 = *(int *)(iVar1 + 0x2c) + *(int *)(iVar1 + 0x48);
  }
  pcVar5 = (char *)(DAT_000f1bb4 + 0xf1978);
  puVar6 = *(undefined4 **)(DAT_000f1bb8 + 0xf197e);
  for (; uVar12 < **(uint **)(this + 0x38); uVar12 = uVar12 + 1) {
    uVar7 = *(uint *)(this + 0x30);
    bVar29 = uVar12 != uVar7;
    if (bVar29) {
      uVar7 = *(uint *)(this + 0x34);
    }
    if (bVar29 && uVar12 != uVar7) {
      uVar10 = 3;
    }
    else {
      uVar10 = 4;
    }
    iVar26 = *(int *)(iVar1 + 0x28);
    iVar27 = *(int *)(iVar1 + 0x2c);
    uVar22 = *(undefined4 *)(iVar1 + 0x70);
    iVar15 = *(int *)(this + 0x1c);
    iVar14 = *(int *)(this + 0x24);
    AbyssEngine::String::String(aSStack_40,pcVar5,false);
    Layout::drawBox(iVar1,uVar10,iVar26 + iVar15,local_a4,(iVar14 >> 1) - (iVar13 + iVar26 + iVar27)
                    ,uVar22,aSStack_40);
    AbyssEngine::String::~String(aSStack_40);
    uVar7 = *puVar2;
    Wanted::isActive(*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + uVar12 * 4));
    AbyssEngine::PaintCanvas::SetColor(uVar7);
    pSVar9 = (String *)*puVar6;
    uVar7 = *puVar2;
    Wanted::getName();
    iVar15 = *(int *)(this + 0x1c);
    iVar1 = *piVar21;
    iVar14 = *(int *)(iVar1 + 0x28);
    iVar27 = *(int *)(iVar1 + 0x44);
    iVar26 = *(int *)(iVar1 + 0x70);
    iVar1 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar2);
    AbyssEngine::PaintCanvas::DrawString
              (uVar7,pSVar9,(int)aSStack_4c,iVar14 + iVar15 + iVar27,
               (bool)(((char)local_a4 + (char)(iVar26 / 2)) - (char)(iVar1 / 2)));
    AbyssEngine::String::~String(aSStack_4c);
    iVar1 = Status::getCurrentCampaignMission();
    if (((uVar12 == 0) && (iVar1 == 0x80)) ||
       (iVar1 = Status::getCurrentCampaignMission(), uVar12 == 1 && iVar1 == 0x82)) {
      iVar27 = *(int *)(this + 0x1c);
      iVar14 = *(int *)(this + 0xac);
      iVar15 = *(int *)(*piVar21 + 0x28);
      iVar1 = *(int *)(*piVar21 + 0x44);
      pSVar9 = (String *)*puVar6;
      uVar7 = *puVar2;
      Wanted::getName();
      AbyssEngine::String::String(aSStack_64,(char *)(DAT_000f1bc4 + 0xf1ad2),false);
      AbyssEngine::operator+(aSStack_4c,aSStack_58);
      iVar26 = AbyssEngine::PaintCanvas::GetTextWidth(uVar7,pSVar9);
      AbyssEngine::PaintCanvas::GetTextHeight(*puVar2);
      AbyssEngine::PaintCanvas::DrawImage2D(uVar7,iVar14,iVar27 + iVar15 + iVar1 + iVar26);
      pcVar18 = *(code **)(DAT_000f1bc8 + 990000);
      (*pcVar18)(aSStack_4c);
      (*pcVar18)(aSStack_64);
      (*pcVar18)(aSStack_58);
    }
    iVar1 = *piVar21;
    local_a4 = *(int *)(iVar1 + 0x34) + *(int *)(iVar1 + 0x70) + local_a4;
  }
  AbyssEngine::PaintCanvas::DisableClip();
  AbyssEngine::PaintCanvas::SetColor(*puVar2);
  iVar1 = *piVar21;
  piVar8 = *(int **)(DAT_000f1bcc + 0xf1b80);
  pSVar9 = (String *)GameText::getText(*piVar8);
  AbyssEngine::String::String(aSStack_70,pSVar9,false);
  Layout::drawHeader(iVar1,aSStack_70);
  AbyssEngine::String::~String(aSStack_70);
  for (uVar12 = 0; uVar12 < **(uint **)(this + 0xc); uVar12 = uVar12 + 1) {
    TouchButton::draw();
  }
  iVar1 = *(int *)(this + 0x1c);
  iVar13 = *(int *)(this + 0x20);
  iVar14 = *piVar21;
  iVar15 = *(int *)(this + 0x24);
  iVar26 = *(int *)(iVar14 + 0x20);
  iVar24 = *(int *)(iVar14 + 0xc);
  iVar27 = *(int *)(iVar14 + 0x28);
  iVar19 = *(int *)(iVar14 + 0x2c);
  uVar22 = *(undefined4 *)(iVar14 + 0x5c);
  pSVar9 = (String *)GameText::getText(*piVar8);
  uVar10 = AbyssEngine::String::String(aSStack_7c,pSVar9,false);
  Layout::drawBox(iVar14,1,iVar27 + iVar1,iVar13 + iVar24 + iVar26,(iVar15 >> 1) - (iVar19 + iVar27)
                  ,uVar22,uVar10);
  AbyssEngine::String::~String(aSStack_7c);
  iVar24 = *piVar21;
  iVar1 = *(int *)(this + 0x1c);
  iVar13 = *(int *)(this + 0x20);
  iVar26 = *(int *)(this + 0x24);
  iVar27 = *(int *)(this + 0x28);
  iVar25 = *(int *)(iVar24 + 0xc);
  iVar14 = *(int *)(iVar24 + 0x10);
  iVar16 = *(int *)(iVar24 + 0x20);
  iVar15 = *(int *)(iVar24 + 0x24);
  iVar23 = *(int *)(iVar24 + 0x28);
  iVar19 = *(int *)(iVar24 + 0x2c);
  iVar20 = *(int *)(iVar24 + 0x5c);
  uVar10 = AbyssEngine::String::String(aSStack_88,(char *)(DAT_000f1fa8 + 0xf1c86),false);
  Layout::drawBox(iVar24,5,iVar23 + iVar1,iVar13 + iVar25 + iVar16 + iVar20 + iVar19,
                  (iVar26 >> 1) - (iVar19 + iVar23),
                  ((iVar27 - (iVar16 + iVar25 + iVar20 + iVar19 * 2)) - iVar14) - iVar15,uVar10);
  AbyssEngine::String::~String(aSStack_88);
  iVar27 = *(int *)(this + 0x24);
  iVar14 = *piVar21;
  iVar1 = *(int *)(this + 0x1c);
  iVar13 = *(int *)(this + 0x20);
  iVar26 = *(int *)(iVar14 + 0x20);
  iVar24 = *(int *)(iVar14 + 0xc);
  iVar15 = *(int *)(iVar14 + 0x28);
  iVar19 = *(int *)(iVar14 + 0x2c);
  uVar22 = *(undefined4 *)(iVar14 + 0x5c);
  pSVar9 = (String *)GameText::getText(*piVar8);
  uVar10 = AbyssEngine::String::String(aSStack_94,pSVar9,false);
  Layout::drawBox(iVar14,1,iVar1 + (iVar27 >> 1) + iVar19,iVar13 + iVar24 + iVar26,
                  ((iVar27 >> 1) - iVar19) - iVar15,uVar22,uVar10);
  AbyssEngine::String::~String(aSStack_94);
  iVar24 = *piVar21;
  iVar1 = *(int *)(this + 0x1c);
  iVar13 = *(int *)(this + 0x20);
  iVar16 = *(int *)(this + 0x24);
  iVar26 = *(int *)(this + 0x28);
  iVar25 = *(int *)(iVar24 + 0xc);
  iVar27 = *(int *)(iVar24 + 0x10);
  iVar23 = *(int *)(iVar24 + 0x20);
  iVar14 = *(int *)(iVar24 + 0x24);
  iVar20 = *(int *)(iVar24 + 0x2c);
  iVar15 = *(int *)(iVar24 + 0x28);
  iVar19 = *(int *)(iVar24 + 0x5c);
  uVar10 = AbyssEngine::String::String(aSStack_a0,(char *)(DAT_000f1fac + 0xf1d68),false);
  Layout::drawBox(iVar24,5,iVar1 + (iVar16 >> 1) + iVar20,iVar13 + iVar20 + iVar25 + iVar23 + iVar19
                  ,((iVar16 >> 1) - iVar20) - iVar15,
                  ((iVar26 - (iVar25 + iVar20 * 2 + iVar23 + iVar19)) - iVar27) - iVar14,uVar10);
  AbyssEngine::String::~String(aSStack_a0);
  if (*(Array **)(this + 8) != (Array *)0x0) {
    iVar1 = *piVar21;
    iVar26 = *(int *)(this + 0x1c) + (*(int *)(this + 0x24) >> 1) + *(int *)(iVar1 + 0x2c);
    iVar13 = *(int *)(iVar1 + 0x5c) +
             *(int *)(this + 0x20) + *(int *)(iVar1 + 0x2c) + *(int *)(iVar1 + 0xc) +
             *(int *)(iVar1 + 0x20);
    ImageFactory::drawChar
              ((ImageFactory *)**(undefined4 **)(DAT_000f1fb0 + 0xf1dc8),*(Array **)(this + 8),
               iVar26,iVar13,false);
    iVar1 = *(int *)(*piVar21 + 0x2d4) + iVar26 + *(int *)(*piVar21 + 0x2c);
    bVar29 = SUB41(iVar13,0);
    AbyssEngine::PaintCanvas::DrawString(*puVar2,(String *)*puVar6,(int)(this + 0x54),iVar1,bVar29);
    pSVar17 = (String *)*puVar6;
    uVar12 = *puVar2;
    uVar10 = *(undefined4 *)(*piVar21 + 4);
    pSVar9 = (String *)GameText::getText(*piVar8);
    AbyssEngine::String::String(aSStack_64,(char *)(DAT_000f1fb4 + 0xf1e48),false);
    AbyssEngine::operator+(aSStack_58,pSVar9);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    bVar29 = (bool)(bVar29 + (char)uVar10 * '\x02');
    AbyssEngine::PaintCanvas::DrawString(uVar12,pSVar17,(int)aSStack_4c,iVar1,bVar29);
    pcVar18 = *(code **)(DAT_000f1fb8 + 0xf1e7e);
    (*pcVar18)(aSStack_4c);
    (*pcVar18)(aSStack_58);
    (*pcVar18)(aSStack_64);
    pSVar17 = (String *)*puVar6;
    uVar10 = *(undefined4 *)(*piVar21 + 4);
    uVar12 = *puVar2;
    pSVar9 = (String *)GameText::getText(*piVar8);
    AbyssEngine::String::String(aSStack_64,(char *)(DAT_000f1fbc + 0xf1eb6),false);
    AbyssEngine::operator+(aSStack_58,pSVar9);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    AbyssEngine::PaintCanvas::DrawString
              (uVar12,pSVar17,(int)aSStack_4c,iVar1,(bool)((char)uVar10 + bVar29));
    (*pcVar18)(aSStack_4c);
    (*pcVar18)(aSStack_58);
    (*pcVar18)(aSStack_64);
    ScrollTouchWindow::draw(*(ScrollTouchWindow **)(this + 0x2c));
  }
  if ((*(int *)(this + 0x18) != 0) &&
     (iVar1 = Wanted::isActive(*(Wanted **)
                                (*(int *)(*(int *)(this + 0x38) + 4) + *(int *)(this + 0x30) * 4)),
     iVar1 != 0)) {
    TouchButton::draw();
  }
  Layout::drawFooter((Layout *)*piVar21);
  if (*piVar11 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000f1838: push {r4,r5,r6,r7,lr}
  000f183a: add r7,sp,#0xc
  000f183c: push {r7,r8,r9,r10,r11}
  000f1840: vpush {d8}
  000f1844: sub sp,#0xb0
  000f1846: mov r8,r0
  000f1848: ldr r0,[0x000f1ba8]
  000f184a: add r0,pc
  000f184c: ldr r2,[r0,#0x0]
  000f184e: ldr r0,[r2,#0x0]
  000f1850: str r0,[sp,#0xac]
  000f1852: ldrb.w r0,[r8,#0x14]
  000f1856: cbz r0,0x000f187e
  000f1858: ldr.w r0,[r8,#0x4]
  000f185c: ldr r1,[sp,#0xac]
  000f185e: ldr r2,[r2,#0x0]
  000f1860: subs r1,r2,r1
  000f1862: itttt eq
  000f1864: add.eq sp,#0xb0
  000f1866: vpop.eq {d8}
  000f186a: add.eq sp,#0x4
  000f186c: pop.eq.w {r8,r9,r10,r11}
  000f1870: itt eq
  000f1872: pop.eq.w {r4,r5,r6,r7,lr}
  000f1876: b.eq.w 0x001ac268
  000f187a: blx 0x0006e824
  000f187e: str r2,[sp,#0x10]
  000f1880: add.w r3,r8,#0x1c
  000f1884: ldr r0,[0x000f1bac]
  000f1886: add r0,pc
  000f1888: ldr.w r9,[r0,#0x0]
  000f188c: ldr.w r0,[r9,#0x0]
  000f1890: ldr.w r12,[r0,#0xc]
  000f1894: ldr r6,[r0,#0x20]
  000f1896: ldr r5,[r0,#0x2c]
  000f1898: ldr r4,[r0,#0x5c]
  000f189a: ldmia r3,{r1,r2,r3}
  000f189c: ldr.w r0,[r8,#0xa8]
  000f18a0: add r2,r12
  000f18a2: add r5,r0
  000f18a4: ldr r0,[0x000f1bb0]
  000f18a6: add r2,r6
  000f18a8: add r0,pc
  000f18aa: add r2,r4
  000f18ac: ldr r0,[r0,#0x0]
  000f18ae: str r0,[sp,#0x30]
  000f18b0: ldr r0,[r0,#0x0]
  000f18b2: str r5,[sp,#0x0]
  000f18b4: blx 0x00075b38
  000f18b8: mov r0,r8
  000f18ba: blx 0x00075b44
  000f18be: ldr.w r5,[r8,#0xa8]
  000f18c2: mov r6,r0
  000f18c4: mov r0,r8
  000f18c6: vmov s0,r5
  000f18ca: vcvt.f32.s32 s16,s0
  000f18ce: blx 0x00075b50
  000f18d2: vmov s0,r0
  000f18d6: vmov s2,r6
  000f18da: vmul.f32 s0,s0,s16
  000f18de: vmul.f32 s2,s2,s16
  000f18e2: vcvt.s32.f32 s0,s0
  000f18e6: vcvt.s32.f32 s2,s2
  000f18ea: str.w r9,[sp,#0x38]
  000f18ee: vmov r11,s0
  000f18f2: vmov r1,s2
  000f18f6: cmp.w r11,#0x0
  000f18fa: it le
  000f18fc: cmp.le r1,#0x1
  000f18fe: blt 0x000f1940
  000f1900: ldr.w r0,[r9,#0x0]
  000f1904: mov r4,r8
  000f1906: ldrd r9,r12,[r8,#0x1c]
  000f190a: ldr.w r6,[r8,#0x24]
  000f190e: ldr r3,[r0,#0x2c]
  000f1910: ldr.w r8,[r0,#0xc]
  000f1914: ldr.w lr,[r0,#0x20]
  000f1918: ldr r2,[r0,#0x48]
  000f191a: ldr.w r10,[r0,#0x5c]
  000f191e: strd r1,r11,[sp,#0x0]
  000f1922: add.w r1,r9,r6, asr #0x1
  000f1926: subs r1,r1,r3
  000f1928: ldr.w r9,[sp,#0x38]
  000f192c: subs r1,r1,r2
  000f192e: add.w r2,r12,r3
  000f1932: add r2,r8
  000f1934: mov r3,r5
  000f1936: add r2,lr
  000f1938: mov r8,r4
  000f193a: add r2,r10
  000f193c: blx 0x00075b5c
  000f1940: ldr.w r6,[r9,#0x0]
  000f1944: cmp.w r11,#0x1
  000f1948: ldr.w r1,[r8,#0x20]
  000f194c: ldr.w r2,[r8,#0x84]
  000f1950: ldr r3,[r6,#0xc]
  000f1952: ldr r5,[r6,#0x20]
  000f1954: add r1,r3
  000f1956: ldr r4,[r6,#0x5c]
  000f1958: add r1,r5
  000f195a: ldr r0,[r6,#0x2c]
  000f195c: add r1,r4
  000f195e: mov.w r4,#0x0
  000f1962: add r1,r0
  000f1964: add r1,r2
  000f1966: str r1,[sp,#0x3c]
  000f1968: itte ge
  000f196a: ldr.ge r1,[r6,#0x48]
  000f196c: add.ge r0,r1
  000f196e: mov.lt r0,#0x0
  000f1970: str r0,[sp,#0x18]
  000f1972: ldr r0,[0x000f1bb4]
  000f1974: add r0,pc
  000f1976: str r0,[sp,#0x14]
  000f1978: ldr r0,[0x000f1bb8]
  000f197a: add r0,pc
  000f197c: ldr r0,[r0,#0x0]
  000f197e: str r0,[sp,#0x2c]
  000f1980: ldr r0,[0x000f1bbc]
  000f1982: add r0,pc
  000f1984: ldr r0,[r0,#0x0]
  000f1986: str r0,[sp,#0x20]
  000f1988: b 0x000f1b54
  000f198a: ldr.w r0,[r8,#0x30]
  000f198e: str r4,[sp,#0x34]
  000f1990: cmp r4,r0
  000f1992: itt ne
  000f1994: ldr.ne.w r0,[r8,#0x34]
  000f1998: cmp.ne r4,r0
  000f199a: bne 0x000f19a0
  000f199c: movs r0,#0x4
  000f199e: b 0x000f19a2
  000f19a0: movs r0,#0x3
  000f19a2: str r0,[sp,#0x28]
  000f19a4: add r0,sp,#0xa0
  000f19a6: ldrd r4,r5,[r6,#0x28]
  000f19aa: movs r2,#0x0
  000f19ac: ldr r1,[sp,#0x14]
  000f19ae: ldr.w r10,[r6,#0x70]
  000f19b2: ldr.w r11,[r8,#0x1c]
  000f19b6: ldr.w r9,[r8,#0x24]
  000f19ba: blx 0x0006ee18
  000f19be: ldr r0,[sp,#0x18]
  000f19c0: add.w r2,r4,r11
  000f19c4: add r0,r4
  000f19c6: add r0,r5
  000f19c8: add r5,sp,#0xa0
  000f19ca: rsb r0,r0,r9, asr #0x1
  000f19ce: strd r0,r10,[sp,#0x0]
  000f19d2: str r5,[sp,#0x8]
  000f19d4: mov r0,r6
  000f19d6: ldr r1,[sp,#0x28]
  000f19d8: ldr r3,[sp,#0x3c]
  000f19da: blx 0x0007462c
  000f19de: mov r0,r5
  000f19e0: blx 0x0006ee30
  000f19e4: ldr.w r0,[r8,#0x38]
  000f19e8: ldr r4,[sp,#0x34]
  000f19ea: ldr.w r9,[sp,#0x30]
  000f19ee: ldr r0,[r0,#0x4]
  000f19f0: ldr.w r5,[r9,#0x0]
  000f19f4: ldr.w r0,[r0,r4,lsl #0x2]
  000f19f8: blx 0x00073774
  000f19fc: ldr r1,[0x000f1bc0]
  000f19fe: cmp r0,#0x0
  000f1a00: mov r0,r5
  000f1a02: it ne
  000f1a04: mov.ne.w r1,#0xffffffff
  000f1a08: blx 0x0006fac0
  000f1a0c: ldr.w r10,[sp,#0x2c]
  000f1a10: ldr.w r0,[r10,#0x0]
  000f1a14: str r0,[sp,#0x28]
  000f1a16: ldr.w r0,[r9,#0x0]
  000f1a1a: str r0,[sp,#0x24]
  000f1a1c: ldr.w r0,[r8,#0x38]
  000f1a20: ldr r0,[r0,#0x4]
  000f1a22: ldr.w r1,[r0,r4,lsl #0x2]
  000f1a26: add r0,sp,#0x94
  000f1a28: blx 0x0007405c
  000f1a2c: ldr r0,[sp,#0x38]
  000f1a2e: ldr.w r11,[r8,#0x1c]
  000f1a32: ldr.w r1,[r10,#0x0]
  000f1a36: ldr r0,[r0,#0x0]
  000f1a38: ldr r6,[r0,#0x28]
  000f1a3a: ldr r5,[r0,#0x44]
  000f1a3c: ldr r4,[r0,#0x70]
  000f1a3e: ldr.w r0,[r9,#0x0]
  000f1a42: add.w r9,sp,#0x94
  000f1a46: blx 0x0006fe20
  000f1a4a: add.w r1,r4,r4, lsr #0x1f
  000f1a4e: ldr r2,[sp,#0x3c]
  000f1a50: add.w r0,r0,r0, lsr #0x1f
  000f1a54: add.w r1,r2,r1, asr #0x1
  000f1a58: mov r2,r9
  000f1a5a: sub.w r0,r1,r0, asr #0x1
  000f1a5e: str r0,[sp,#0x0]
  000f1a60: movs r0,#0x0
  000f1a62: str r0,[sp,#0x4]
  000f1a64: add.w r0,r6,r11
  000f1a68: adds r3,r0,r5
  000f1a6a: ldrd r0,r1,[sp,#0x24]
  000f1a6e: blx 0x0006fe14
  000f1a72: mov r0,r9
  000f1a74: blx 0x0006ee30
  000f1a78: ldr r0,[sp,#0x20]
  000f1a7a: ldr r0,[r0,#0x0]
  000f1a7c: blx 0x00071770
  000f1a80: ldr r4,[sp,#0x34]
  000f1a82: cbnz r4,0x000f1a88
  000f1a84: cmp r0,#0x80
  000f1a86: beq 0x000f1a98
  000f1a88: ldr r0,[sp,#0x20]
  000f1a8a: ldr r0,[r0,#0x0]
  000f1a8c: blx 0x00071770
  000f1a90: cmp r4,#0x1
  000f1a92: it eq
  000f1a94: cmp.eq r0,#0x82
  000f1a96: bne 0x000f1b3e
  000f1a98: ldr r0,[sp,#0x38]
  000f1a9a: add r6,sp,#0x88
  000f1a9c: ldr.w r1,[r8,#0x1c]
  000f1aa0: str r1,[sp,#0x24]
  000f1aa2: ldr.w r1,[r8,#0xac]
  000f1aa6: str r1,[sp,#0x28]
  000f1aa8: ldr r0,[r0,#0x0]
  000f1aaa: ldr r1,[sp,#0x2c]
  000f1aac: ldr.w r10,[r0,#0x28]
  000f1ab0: ldr r0,[r0,#0x44]
  000f1ab2: ldr r5,[r1,#0x0]
  000f1ab4: ldr r1,[sp,#0x30]
  000f1ab6: str r0,[sp,#0x1c]
  000f1ab8: ldr.w r0,[r8,#0x38]
  000f1abc: ldr.w r9,[r1,#0x0]
  000f1ac0: ldr r0,[r0,#0x4]
  000f1ac2: ldr.w r1,[r0,r4,lsl #0x2]
  000f1ac6: mov r0,r6
  000f1ac8: blx 0x0007405c
  000f1acc: ldr r1,[0x000f1bc4]
  000f1ace: add r1,pc
  000f1ad0: add.w r11,sp,#0x7c
  000f1ad4: movs r2,#0x0
  000f1ad6: mov r0,r11
  000f1ad8: blx 0x0006ee18
  000f1adc: add r4,sp,#0x94
  000f1ade: mov r1,r6
  000f1ae0: mov r2,r11
  000f1ae2: mov r0,r4
  000f1ae4: blx 0x0006ef98
  000f1ae8: mov r0,r9
  000f1aea: mov r1,r5
  000f1aec: mov r2,r4
  000f1aee: blx 0x0006faa8
  000f1af2: mov r5,r0
  000f1af4: ldr r0,[sp,#0x38]
  000f1af6: ldr r0,[r0,#0x0]
  000f1af8: ldr r6,[r0,#0x70]
  000f1afa: ldr r0,[sp,#0x2c]
  000f1afc: ldr r1,[r0,#0x0]
  000f1afe: ldr r0,[sp,#0x30]
  000f1b00: ldr r0,[r0,#0x0]
  000f1b02: blx 0x0006fe20
  000f1b06: ldr r1,[sp,#0x24]
  000f1b08: add.w r0,r0,r0, lsr #0x1f
  000f1b0c: ldr r2,[sp,#0x1c]
  000f1b0e: add r1,r10
  000f1b10: ldr r3,[sp,#0x3c]
  000f1b12: add r1,r2
  000f1b14: adds r2,r1,r5
  000f1b16: add.w r1,r6,r6, lsr #0x1f
  000f1b1a: add.w r1,r3,r1, asr #0x1
  000f1b1e: sub.w r3,r1,r0, asr #0x1
  000f1b22: ldr r1,[sp,#0x28]
  000f1b24: mov r0,r9
  000f1b26: blx 0x00072dc0
  000f1b2a: ldr r0,[0x000f1bc8]
  000f1b2c: add r0,pc
  000f1b2e: ldr r4,[r0,#0x0]
  000f1b30: add r0,sp,#0x94
  000f1b32: blx r4
  000f1b34: add r0,sp,#0x7c
  000f1b36: blx r4
  000f1b38: add r0,sp,#0x88
  000f1b3a: blx r4
  000f1b3c: ldr r4,[sp,#0x34]
  000f1b3e: ldr.w r9,[sp,#0x38]
  000f1b42: adds r4,#0x1
  000f1b44: ldr r2,[sp,#0x3c]
  000f1b46: ldr.w r6,[r9,#0x0]
  000f1b4a: ldr r1,[r6,#0x70]
  000f1b4c: ldr r0,[r6,#0x34]
  000f1b4e: add r1,r2
  000f1b50: add r0,r1
  000f1b52: str r0,[sp,#0x3c]
  000f1b54: ldr.w r0,[r8,#0x38]
  000f1b58: ldr r0,[r0,#0x0]
  000f1b5a: cmp r4,r0
  000f1b5c: bcc.w 0x000f198a
  000f1b60: ldr r4,[sp,#0x30]
  000f1b62: ldr r0,[r4,#0x0]
  000f1b64: blx 0x00075b68
  000f1b68: ldr r0,[r4,#0x0]
  000f1b6a: mov.w r1,#0xffffffff
  000f1b6e: blx 0x0006fac0
  000f1b72: ldr r0,[0x000f1bcc]
  000f1b74: movw r1,#0xc93
  000f1b78: ldr.w r5,[r9,#0x0]
  000f1b7c: add r0,pc
  000f1b7e: ldr r0,[r0,#0x0]
  000f1b80: str r0,[sp,#0x3c]
  000f1b82: ldr r0,[r0,#0x0]
  000f1b84: blx 0x00072f70
  000f1b88: add r6,sp,#0x70
  000f1b8a: mov r1,r0
  000f1b8c: movs r2,#0x0
  000f1b8e: mov r0,r6
  000f1b90: blx 0x0006f028
  000f1b94: mov r0,r5
  000f1b96: mov r1,r6
  000f1b98: blx 0x000746d4
  000f1b9c: add r0,sp,#0x70
  000f1b9e: blx 0x0006ee30
  000f1ba2: movs r4,#0x0
  000f1ba4: b 0x000f1bdc
  000f1bd0: ldr r0,[r0,#0x4]
  000f1bd2: ldr.w r0,[r0,r4,lsl #0x2]
  000f1bd6: blx 0x000746e0
  000f1bda: adds r4,#0x1
  000f1bdc: ldr.w r0,[r8,#0xc]
  000f1be0: ldr r1,[r0,#0x0]
  000f1be2: cmp r4,r1
  000f1be4: bcc 0x000f1bd0
  000f1be6: ldr.w r0,[r8,#0x1c]
  000f1bea: mov r11,r8
  000f1bec: str r0,[sp,#0x34]
  000f1bee: ldr.w r0,[r8,#0x20]
  000f1bf2: str r0,[sp,#0x28]
  000f1bf4: ldr r0,[sp,#0x3c]
  000f1bf6: ldr.w r5,[r9,#0x0]
  000f1bfa: ldr.w r6,[r8,#0x24]
  000f1bfe: ldr r0,[r0,#0x0]
  000f1c00: ldr r1,[r5,#0x20]
  000f1c02: str r1,[sp,#0x24]
  000f1c04: movw r1,#0xc95
  000f1c08: ldr.w r10,[r5,#0xc]
  000f1c0c: ldr r4,[r5,#0x28]
  000f1c0e: ldr.w r8,[r5,#0x2c]
  000f1c12: ldr.w r9,[r5,#0x5c]
  000f1c16: blx 0x00072f70
  000f1c1a: mov r1,r0
  000f1c1c: add r0,sp,#0x64
  000f1c1e: movs r2,#0x0
  000f1c20: blx 0x0006f028
  000f1c24: add.w r1,r8,r4
  000f1c28: rsb r1,r1,r6, asr #0x1
  000f1c2c: strd r1,r9,[sp,#0x0]
  000f1c30: str r0,[sp,#0x8]
  000f1c32: ldr r0,[sp,#0x34]
  000f1c34: ldr r1,[sp,#0x24]
  000f1c36: adds r2,r4,r0
  000f1c38: ldr r0,[sp,#0x28]
  000f1c3a: add r0,r10
  000f1c3c: adds r3,r0,r1
  000f1c3e: mov r0,r5
  000f1c40: movs r1,#0x1
  000f1c42: blx 0x0007462c
  000f1c46: add r0,sp,#0x64
  000f1c48: blx 0x0006ee30
  000f1c4c: ldr r0,[sp,#0x38]
  000f1c4e: movs r2,#0x0
  000f1c50: ldr r5,[r0,#0x0]
  000f1c52: ldr.w r0,[r11,#0x1c]
  000f1c56: str r0,[sp,#0x34]
  000f1c58: ldr.w r0,[r11,#0x20]
  000f1c5c: str r0,[sp,#0x28]
  000f1c5e: ldr.w r0,[r11,#0x24]
  000f1c62: str r0,[sp,#0x18]
  000f1c64: ldr.w r0,[r11,#0x28]
  000f1c68: str r0,[sp,#0x24]
  000f1c6a: ldrd r10,r0,[r5,#0xc]
  000f1c6e: str r0,[sp,#0x20]
  000f1c70: ldrd r6,r0,[r5,#0x20]
  000f1c74: str r0,[sp,#0x1c]
  000f1c76: add r0,sp,#0x58
  000f1c78: ldrd r9,r4,[r5,#0x28]
  000f1c7c: ldr.w r8,[r5,#0x5c]
  000f1c80: ldr r1,[0x000f1fa8]
  000f1c82: add r1,pc
  000f1c84: blx 0x0006ee18
  000f1c88: add.w r1,r4,r9
  000f1c8c: ldr r2,[sp,#0x18]
  000f1c8e: ldr r3,[sp,#0x24]
  000f1c90: rsb r1,r1,r2, asr #0x1
  000f1c94: add.w r2,r6,r10
  000f1c98: add r2,r8
  000f1c9a: add.w r2,r2,r4, lsl #0x1
  000f1c9e: subs r2,r3,r2
  000f1ca0: ldr r3,[sp,#0x20]
  000f1ca2: subs r2,r2,r3
  000f1ca4: ldr r3,[sp,#0x1c]
  000f1ca6: subs r2,r2,r3
  000f1ca8: strd r1,r2,[sp,#0x0]
  000f1cac: movs r1,#0x5
  000f1cae: str r0,[sp,#0x8]
  000f1cb0: ldr r0,[sp,#0x34]
  000f1cb2: add.w r2,r9,r0
  000f1cb6: ldr r0,[sp,#0x28]
  000f1cb8: add r0,r10
  000f1cba: add r0,r6
  000f1cbc: add r0,r8
  000f1cbe: adds r3,r0,r4
  000f1cc0: mov r0,r5
  000f1cc2: blx 0x0007462c
  000f1cc6: add r0,sp,#0x58
  000f1cc8: blx 0x0006ee30
  000f1ccc: ldr r0,[sp,#0x38]
  000f1cce: ldr.w r4,[r11,#0x24]
  000f1cd2: ldr r5,[r0,#0x0]
  000f1cd4: ldr.w r0,[r11,#0x1c]
  000f1cd8: str r0,[sp,#0x28]
  000f1cda: ldr.w r0,[r11,#0x20]
  000f1cde: str r0,[sp,#0x34]
  000f1ce0: ldr r0,[sp,#0x3c]
  000f1ce2: ldr r1,[r5,#0x20]
  000f1ce4: str r1,[sp,#0x24]
  000f1ce6: movw r1,#0xc96
  000f1cea: ldr r0,[r0,#0x0]
  000f1cec: ldr.w r10,[r5,#0xc]
  000f1cf0: ldrd r6,r8,[r5,#0x28]
  000f1cf4: ldr.w r9,[r5,#0x5c]
  000f1cf8: blx 0x00072f70
  000f1cfc: mov r1,r0
  000f1cfe: add r0,sp,#0x4c
  000f1d00: movs r2,#0x0
  000f1d02: blx 0x0006f028
  000f1d06: rsb r1,r8,r4, asr #0x1
  000f1d0a: subs r1,r1,r6
  000f1d0c: strd r1,r9,[sp,#0x0]
  000f1d10: str r0,[sp,#0x8]
  000f1d12: ldr r0,[sp,#0x28]
  000f1d14: ldr r1,[sp,#0x24]
  000f1d16: add.w r0,r0,r4, asr #0x1
  000f1d1a: add.w r2,r0,r8
  000f1d1e: ldr r0,[sp,#0x34]
  000f1d20: add r0,r10
  000f1d22: adds r3,r0,r1
  000f1d24: mov r0,r5
  000f1d26: movs r1,#0x1
  000f1d28: blx 0x0007462c
  000f1d2c: add r0,sp,#0x4c
  000f1d2e: blx 0x0006ee30
  000f1d32: ldr r0,[sp,#0x38]
  000f1d34: movs r2,#0x0
  000f1d36: ldr r5,[r0,#0x0]
  000f1d38: ldr.w r0,[r11,#0x1c]
  000f1d3c: str r0,[sp,#0x28]
  000f1d3e: ldr.w r0,[r11,#0x20]
  000f1d42: str r0,[sp,#0x34]
  000f1d44: ldrd r6,r0,[r11,#0x24]
  000f1d48: str r0,[sp,#0x24]
  000f1d4a: ldrd r10,r0,[r5,#0xc]
  000f1d4e: str r0,[sp,#0x20]
  000f1d50: ldrd r9,r0,[r5,#0x20]
  000f1d54: ldr.w r8,[r5,#0x2c]
  000f1d58: str r0,[sp,#0x1c]
  000f1d5a: ldr r0,[r5,#0x28]
  000f1d5c: str r0,[sp,#0x18]
  000f1d5e: add r0,sp,#0x40
  000f1d60: ldr r4,[r5,#0x5c]
  000f1d62: ldr r1,[0x000f1fac]
  000f1d64: add r1,pc
  000f1d66: blx 0x0006ee18
  000f1d6a: ldr r2,[sp,#0x18]
  000f1d6c: rsb r1,r8,r6, asr #0x1
  000f1d70: ldr r3,[sp,#0x24]
  000f1d72: subs r1,r1,r2
  000f1d74: add.w r2,r10,r8, lsl #0x1
  000f1d78: add r2,r9
  000f1d7a: add r2,r4
  000f1d7c: subs r2,r3,r2
  000f1d7e: ldr r3,[sp,#0x20]
  000f1d80: subs r2,r2,r3
  000f1d82: ldr r3,[sp,#0x1c]
  000f1d84: subs r2,r2,r3
  000f1d86: strd r1,r2,[sp,#0x0]
  000f1d8a: movs r1,#0x5
  000f1d8c: str r0,[sp,#0x8]
  000f1d8e: ldr r0,[sp,#0x28]
  000f1d90: add.w r0,r0,r6, asr #0x1
  000f1d94: add.w r2,r0,r8
  000f1d98: ldr r0,[sp,#0x34]
  000f1d9a: add r0,r8
  000f1d9c: add r0,r10
  000f1d9e: add r0,r9
  000f1da0: adds r3,r0,r4
  000f1da2: mov r0,r5
  000f1da4: blx 0x0007462c
  000f1da8: add r0,sp,#0x40
  000f1daa: blx 0x0006ee30
  000f1dae: ldr.w r1,[r11,#0x8]
  000f1db2: mov r5,r11
  000f1db4: ldr r6,[sp,#0x38]
  000f1db6: cmp r1,#0x0
  000f1db8: beq.w 0x000f1efc
  000f1dbc: ldr r0,[0x000f1fb0]
  000f1dbe: mov.w r8,#0x0
  000f1dc2: ldr r2,[r6,#0x0]
  000f1dc4: add r0,pc
  000f1dc6: ldr r0,[r0,#0x0]
  000f1dc8: ldr.w r12,[r2,#0xc]
  000f1dcc: ldr.w lr,[r2,#0x20]
  000f1dd0: ldr r0,[r0,#0x0]
  000f1dd2: ldr r4,[r2,#0x2c]
  000f1dd4: ldr.w r9,[r2,#0x5c]
  000f1dd8: ldrd r2,r10,[r5,#0x1c]
  000f1ddc: ldr r3,[r5,#0x24]
  000f1dde: str r5,[sp,#0x34]
  000f1de0: str.w r8,[sp,#0x0]
  000f1de4: add.w r5,r2,r3, asr #0x1
  000f1de8: add.w r3,r10,r4
  000f1dec: add r3,r12
  000f1dee: add.w r11,r5,r4
  000f1df2: add r3,lr
  000f1df4: add r9,r3
  000f1df6: mov r2,r11
  000f1df8: mov r3,r9
  000f1dfa: blx 0x00075b74
  000f1dfe: ldr r0,[r6,#0x0]
  000f1e00: ldr r5,[sp,#0x2c]
  000f1e02: ldr r4,[sp,#0x30]
  000f1e04: ldr.w r3,[r0,#0x2d4]
  000f1e08: ldr r2,[r0,#0x2c]
  000f1e0a: ldr r1,[r5,#0x0]
  000f1e0c: add r3,r11
  000f1e0e: ldr r0,[r4,#0x0]
  000f1e10: add r3,r2
  000f1e12: strd r9,r8,[sp,#0x0]
  000f1e16: ldr.w r10,[sp,#0x34]
  000f1e1a: str r3,[sp,#0x28]
  000f1e1c: add.w r2,r10,#0x54
  000f1e20: blx 0x0006fe14
  000f1e24: ldr r0,[r6,#0x0]
  000f1e26: movw r1,#0xc9a
  000f1e2a: ldr r6,[r5,#0x0]
  000f1e2c: ldr r4,[r4,#0x0]
  000f1e2e: ldr.w r8,[r0,#0x4]
  000f1e32: ldr r0,[sp,#0x3c]
  000f1e34: ldr r0,[r0,#0x0]
  000f1e36: blx 0x00072f70
  000f1e3a: ldr r1,[0x000f1fb4]
  000f1e3c: add.w r11,sp,#0x7c
  000f1e40: mov r5,r0
  000f1e42: movs r2,#0x0
  000f1e44: add r1,pc
  000f1e46: mov r0,r11
  000f1e48: blx 0x0006ee18
  000f1e4c: add r0,sp,#0x88
  000f1e4e: mov r1,r5
  000f1e50: mov r2,r11
  000f1e52: blx 0x0006ef98
  000f1e56: add.w r2,r10,#0x60
  000f1e5a: add r0,sp,#0x94
  000f1e5c: add r1,sp,#0x88
  000f1e5e: blx 0x0006ef98
  000f1e62: add.w r11,r9,r8, lsl #0x1
  000f1e66: movs r0,#0x0
  000f1e68: add r2,sp,#0x94
  000f1e6a: strd r11,r0,[sp,#0x0]
  000f1e6e: mov r0,r4
  000f1e70: ldr r3,[sp,#0x28]
  000f1e72: mov r1,r6
  000f1e74: blx 0x0006fe14
  000f1e78: ldr r0,[0x000f1fb8]
  000f1e7a: add r0,pc
  000f1e7c: ldr r6,[r0,#0x0]
  000f1e7e: add r0,sp,#0x94
  000f1e80: blx r6
  000f1e82: add r0,sp,#0x88
  000f1e84: blx r6
  000f1e86: add r0,sp,#0x7c
  000f1e88: blx r6
  000f1e8a: ldr r0,[sp,#0x38]
  000f1e8c: ldr r1,[sp,#0x2c]
  000f1e8e: ldr r0,[r0,#0x0]
  000f1e90: ldr.w r9,[r1,#0x0]
  000f1e94: ldr r1,[sp,#0x30]
  000f1e96: ldr r4,[r0,#0x4]
  000f1e98: ldr r0,[sp,#0x3c]
  000f1e9a: ldr.w r10,[r1,#0x0]
  000f1e9e: movw r1,#0xc99
  000f1ea2: ldr r0,[r0,#0x0]
  000f1ea4: blx 0x00072f70
  000f1ea8: ldr r1,[0x000f1fbc]
  000f1eaa: add.w r8,sp,#0x7c
  000f1eae: mov r5,r0
  000f1eb0: movs r2,#0x0
  000f1eb2: add r1,pc
  000f1eb4: mov r0,r8
  000f1eb6: blx 0x0006ee18
  000f1eba: add r0,sp,#0x88
  000f1ebc: mov r1,r5
  000f1ebe: mov r2,r8
  000f1ec0: blx 0x0006ef98
  000f1ec4: ldr r5,[sp,#0x34]
  000f1ec6: add r0,sp,#0x94
  000f1ec8: add r1,sp,#0x88
  000f1eca: add.w r2,r5,#0x78
  000f1ece: blx 0x0006ef98
  000f1ed2: add.w r0,r4,r11
  000f1ed6: movs r1,#0x0
  000f1ed8: add r2,sp,#0x94
  000f1eda: strd r0,r1,[sp,#0x0]
  000f1ede: mov r0,r10
  000f1ee0: ldr r3,[sp,#0x28]
  000f1ee2: mov r1,r9
  000f1ee4: blx 0x0006fe14
  000f1ee8: add r0,sp,#0x94
  000f1eea: blx r6
  000f1eec: add r0,sp,#0x88
  000f1eee: blx r6
  000f1ef0: add r0,sp,#0x7c
  000f1ef2: blx r6
  000f1ef4: ldr r0,[r5,#0x2c]
  000f1ef6: blx 0x00075b80
  000f1efa: ldr r6,[sp,#0x38]
  000f1efc: ldr r0,[r5,#0x18]
  000f1efe: cbz r0,0x000f1f16
  000f1f00: ldr r1,[r5,#0x38]
  000f1f02: ldr r0,[r5,#0x30]
  000f1f04: ldr r1,[r1,#0x4]
  000f1f06: ldr.w r0,[r1,r0,lsl #0x2]
  000f1f0a: blx 0x00073774
  000f1f0e: cbz r0,0x000f1f16
  000f1f10: ldr r0,[r5,#0x18]
  000f1f12: blx 0x000746e0
  000f1f16: ldr r0,[r6,#0x0]
  000f1f18: blx 0x00075b8c
  000f1f1c: ldr r0,[sp,#0xac]
  000f1f1e: ldr r1,[sp,#0x10]
  000f1f20: ldr r1,[r1,#0x0]
  000f1f22: subs r0,r1,r0
  000f1f24: itttt eq
  000f1f26: add.eq sp,#0xb0
  000f1f28: vpop.eq {d8}
  000f1f2c: add.eq sp,#0x4
  000f1f2e: pop.eq.w {r8,r9,r10,r11}
  000f1f32: it eq
  000f1f34: pop.eq {r4,r5,r6,r7,pc}
  000f1f36: blx 0x0006e824
```
