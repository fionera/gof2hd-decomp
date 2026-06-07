# SpaceLounge::drawLounge
elf 0x170d38 body 1768
Sig: undefined __thiscall drawLounge(SpaceLounge * this)

## decompile
```c

/* SpaceLounge::drawLounge() */

void __thiscall SpaceLounge::drawLounge(SpaceLounge *this)

{
  char cVar1;
  int *piVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  String *pSVar6;
  int iVar7;
  int iVar8;
  float *pfVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int iVar13;
  uint uVar14;
  undefined4 uVar15;
  int iVar16;
  undefined4 uVar17;
  undefined4 *puVar18;
  Agent *this_00;
  undefined4 uVar19;
  uint *puVar20;
  undefined4 uVar21;
  uint in_fpscr;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  String aSStack_98 [12];
  String aSStack_8c [12];
  String aSStack_80 [12];
  String aSStack_74 [12];
  String aSStack_68 [12];
  String aSStack_5c [12];
  String aSStack_50 [12];
  int local_44;
  
  piVar12 = *(int **)(DAT_00181108 + 0x180d4e);
  local_44 = *piVar12;
  if (*(int *)(this + 0x14) == 0) {
    iVar13 = *(int *)(this + 0x88);
    if (-1 < iVar13) {
      puVar20 = *(uint **)(DAT_00181124 + 0x180e98);
      piVar2 = *(int **)(DAT_00181128 + 0x180e9e);
      pfVar9 = *(float **)(*(int *)(*(int *)(this + 0x40) + 4) + (iVar13 << 3 | 4U));
      this_00 = *(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + iVar13 * 4);
      iVar8 = *(int *)(*piVar2 + 0x94);
      fVar23 = *pfVar9;
      fVar24 = pfVar9[1];
      fVar25 = **(float **)(*(int *)(*(int *)(this + 0x40) + 4) + iVar13 * 8);
      AbyssEngine::PaintCanvas::SetColor(*puVar20);
      iVar10 = iVar8 * 2;
      fVar22 = (float)VectorSignedToFloat(iVar10,(byte)(in_fpscr >> 0x16) & 3);
      iVar5 = (int)(fVar25 + (fVar23 - fVar25) * 0.5);
      iVar16 = (int)(fVar24 - fVar22);
      iVar13 = Agent::isKnown(this_00);
      if ((iVar13 == 0) && (iVar13 = Agent::isStoryAgent(this_00), iVar13 == 0)) {
        iVar13 = **(int **)(DAT_00181130 + 0x18107a);
        Agent::getRace(this_00);
        pSVar6 = (String *)GameText::getText(iVar13);
        AbyssEngine::String::String(aSStack_50,pSVar6,false);
      }
      else {
        Agent::getName();
      }
      iVar13 = Agent::getMission(this_00);
      if (iVar13 == 0) {
        iVar13 = Agent::getOffer(this_00);
        if (iVar13 == 6) {
          pSVar6 = (String *)GameText::getText(**(int **)(DAT_00181138 + 0x1810d6));
          AbyssEngine::String::String(aSStack_5c,pSVar6,false);
        }
        else {
          iVar13 = Agent::getOffer(this_00);
          if (iVar13 == 2) {
            pSVar6 = (String *)GameText::getText(**(int **)(DAT_001814ac + 0x18114c));
            AbyssEngine::String::String(aSStack_5c,pSVar6,false);
          }
          else {
            iVar13 = Agent::getOffer(this_00);
            if (iVar13 == 7) {
              pSVar6 = (String *)GameText::getText(**(int **)(DAT_001814b0 + 0x1811da));
              AbyssEngine::String::String(aSStack_5c,pSVar6,false);
            }
            else {
              AbyssEngine::String::String(aSStack_5c,(char *)(DAT_001814b4 + 0x181256),false);
            }
          }
        }
      }
      else {
        iVar13 = **(int **)(DAT_00181134 + 0x1810a4);
        Agent::getMission(this_00);
        Mission::getType();
        pSVar6 = (String *)GameText::getText(iVar13);
        AbyssEngine::String::String(aSStack_5c,pSVar6,false);
      }
      puVar18 = *(undefined4 **)(DAT_001814b8 + 0x181262);
      iVar13 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar20,(String *)*puVar18);
      iVar7 = Agent::isKnown(this_00);
      if (iVar7 != 0) {
        pSVar6 = (String *)*puVar18;
        uVar4 = *puVar20;
        AbyssEngine::String::String(aSStack_74,(char *)(DAT_001814bc + 0x181288),false);
        AbyssEngine::operator+(aSStack_68,aSStack_5c);
        iVar7 = AbyssEngine::PaintCanvas::GetTextWidth(uVar4,pSVar6);
        AbyssEngine::String::~String(aSStack_68);
        AbyssEngine::String::~String(aSStack_74);
        iVar13 = iVar13 + iVar7;
      }
      iVar11 = *piVar2;
      iVar7 = *(int *)(iVar11 + 0x28);
      if (iVar5 - iVar8 < iVar7) {
        iVar5 = iVar7 + iVar8;
      }
      else {
        iVar7 = **(int **)(DAT_001814c0 + 0x1812d0) - iVar7;
        if (iVar7 < iVar8 + iVar5 + iVar13) {
          iVar5 = iVar7 - (iVar13 + iVar10);
        }
      }
      uVar15 = *(undefined4 *)(iVar11 + 0x30);
      uVar3 = AbyssEngine::String::String(aSStack_80,(char *)(DAT_001814c4 + 0x1812ee),false);
      iVar7 = iVar16 - iVar8;
      Layout::drawBox(iVar11,2,iVar5 - iVar8,iVar7,iVar10 + iVar13,uVar15,uVar3);
      AbyssEngine::String::~String(aSStack_80);
      AbyssEngine::PaintCanvas::SetColor(*puVar20);
      AbyssEngine::PaintCanvas::DrawRectangle
                ((PaintCanvas *)*puVar20,iVar5 - iVar8,iVar7,iVar10 + iVar13,
                 *(int *)(*piVar2 + 0x30));
      AbyssEngine::PaintCanvas::SetColor(*puVar20);
      pSVar6 = (String *)*puVar18;
      uVar4 = *puVar20;
      iVar13 = Agent::isKnown(this_00);
      if (iVar13 == 0) {
        AbyssEngine::String::String(aSStack_74,(char *)(DAT_001814cc + 0x18137a),false);
      }
      else {
        AbyssEngine::String::String(aSStack_74,(char *)(DAT_001814c8 + 0x18136c),false);
      }
      AbyssEngine::operator+(aSStack_68,aSStack_50);
      cVar1 = (char)iVar16;
      AbyssEngine::PaintCanvas::DrawString
                (uVar4,pSVar6,(int)aSStack_68,iVar5,
                 (bool)((char)*(undefined4 *)(*piVar2 + 0x2c0) + cVar1));
      AbyssEngine::String::~String(aSStack_68);
      AbyssEngine::String::~String(aSStack_74);
      iVar13 = Agent::isKnown(this_00);
      if (iVar13 != 0) {
        AbyssEngine::PaintCanvas::SetColor(*puVar20);
        pSVar6 = (String *)*puVar18;
        uVar4 = *puVar20;
        AbyssEngine::String::String(aSStack_74,(char *)(DAT_001814d0 + 0x1813d2),false);
        AbyssEngine::operator+(aSStack_68,aSStack_50);
        iVar13 = AbyssEngine::PaintCanvas::GetTextWidth(uVar4,pSVar6);
        AbyssEngine::PaintCanvas::DrawString
                  (uVar4,pSVar6,(int)aSStack_5c,iVar13 + iVar5,
                   (bool)((char)*(undefined4 *)(*piVar2 + 0x2c0) + cVar1));
        AbyssEngine::String::~String(aSStack_68);
        AbyssEngine::String::~String(aSStack_74);
      }
      AbyssEngine::String::~String(aSStack_5c);
      AbyssEngine::String::~String(aSStack_50);
    }
    goto LAB_00181424;
  }
  puVar20 = *(uint **)(DAT_0018110c + 0x180d66);
  AbyssEngine::PaintCanvas::SetColor(*puVar20);
  piVar2 = *(int **)(DAT_00181110 + 0x180d78);
  iVar13 = *piVar2;
  uVar21 = *(undefined4 *)(this + 0x70);
  uVar17 = *(undefined4 *)(this + 0x74);
  uVar15 = *(undefined4 *)(iVar13 + 0x68);
  uVar19 = *(undefined4 *)(iVar13 + 0x6c);
  uVar3 = AbyssEngine::String::String(aSStack_8c,(char *)(DAT_00181114 + 0x180d8c),false);
  Layout::drawBox(iVar13,2,uVar21,uVar17,uVar15,uVar19,uVar3);
  AbyssEngine::String::~String(aSStack_8c);
  AbyssEngine::PaintCanvas::SetColor(*puVar20);
  AbyssEngine::PaintCanvas::DrawRectangle
            ((PaintCanvas *)*puVar20,*(int *)(this + 0x70),*(int *)(this + 0x74),
             *(int *)(*piVar2 + 0x68),*(int *)(*piVar2 + 0x6c));
  AbyssEngine::PaintCanvas::SetColor(*puVar20);
  puVar18 = *(undefined4 **)(DAT_0018111c + 0x180de0);
  ImageFactory::drawChar
            ((ImageFactory *)*puVar18,
             *(Array **)(*(int *)(*(int *)(this + 0x38) + 4) + *(int *)(this + 0x20) * 4),
             *(int *)(*piVar2 + 0x4c) + *(int *)(this + 0x70),
             *(int *)(*piVar2 + 0x4c) + *(int *)(this + 0x74),false);
  ScrollTouchWindow::draw(*(ScrollTouchWindow **)(this + 0x60));
  if ((*(uint *)(this + 0x14) & 0xfffffffe) != 2) goto LAB_00181424;
  TouchButton::setTextColor
            ((TouchButton *)**(undefined4 **)(*(int *)(this + 0x5c) + 4),DAT_00181120);
  uVar4 = Agent::getOffer(*(Agent **)
                           (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
  if (*(int *)(this + 0x14) == 2) {
    TouchButton::setPosition
              ((TouchButton *)**(undefined4 **)(*(int *)(this + 0x5c) + 4),*(int *)(this + 0x84),
               *(int *)(this + 0x80));
    TouchButton::setPosition
              (*(TouchButton **)(*(int *)(*(int *)(this + 0x5c) + 4) + 4),
               *(int *)(this + 0x6c) + *(int *)(this + 0x84),*(int *)(this + 0x80),'\x12');
    *(undefined4 *)(this + 0x68) = 0;
    if (uVar4 < 0xb) {
      if ((1 << (uVar4 & 0xff) & 0x60cU) != 0) {
        *(undefined4 *)(this + 0x68) = 3;
        goto LAB_00180f36;
      }
      if (uVar4 == 0) {
        iVar13 = Agent::getMission(*(Agent **)
                                    (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4
                                    ));
        if (iVar13 != 0) {
          Agent::getMission(*(Agent **)
                             (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
          iVar13 = Mission::isOutsideMission();
          if (iVar13 != 0) {
            Agent::getMission(*(Agent **)
                               (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
            iVar13 = Mission::getType();
            if (iVar13 != 0xc) {
              uVar3 = 1;
              if (this[0x36] == (SpaceLounge)0x0) {
                uVar3 = 4;
              }
              *(undefined4 *)(this + 0x68) = uVar3;
              if (this[0x36] != (SpaceLounge)0x0) {
                iVar13 = *(int *)(this + 0x5c);
                goto LAB_00180f1a;
              }
              goto LAB_00180f36;
            }
          }
        }
      }
      else if (uVar4 == 1) {
        iVar13 = *(int *)(this + 0x5c);
        iVar8 = *(int *)(this + 0x7c);
        iVar5 = *(int *)(this + 0x84);
        *(undefined4 *)(this + 0x68) = 1;
        goto LAB_00180f2e;
      }
    }
    *(undefined4 *)(this + 0x68) = 2;
    TouchButton::setPosition
              ((TouchButton *)**(undefined4 **)(*(int *)(this + 0x5c) + 4),*(int *)(this + 0x84),
               *(int *)(this + 0x7c));
    TouchButton::setPosition
              (*(TouchButton **)(*(int *)(*(int *)(this + 0x5c) + 4) + 4),
               *(int *)(this + 0x6c) + *(int *)(this + 0x84),*(int *)(this + 0x7c),'\x12');
  }
  else {
    iVar13 = *(int *)(this + 0x5c);
    *(undefined4 *)(this + 0x68) = 1;
LAB_00180f1a:
    TouchButton::setTextColor((TouchButton *)**(undefined4 **)(iVar13 + 4),-1);
    iVar13 = *(int *)(this + 0x5c);
    iVar8 = *(int *)(this + 0x7c);
    iVar5 = *(int *)(this + 0x84);
LAB_00180f2e:
    TouchButton::setPosition((TouchButton *)**(undefined4 **)(iVar13 + 4),iVar5,iVar8);
  }
LAB_00180f36:
  for (uVar14 = 0; uVar14 < **(uint **)(this + 0x5c); uVar14 = uVar14 + 1) {
    TouchButton::setVisible(*(TouchButton **)((*(uint **)(this + 0x5c))[1] + uVar14 * 4),false);
  }
  iVar16 = *piVar2;
  iVar8 = *(int *)(this + 0x68);
  iVar10 = *(int *)(iVar16 + 0x4c);
  iVar5 = *(int *)(iVar16 + 0x2d8);
  iVar13 = iVar5;
  if ((2 < iVar8) &&
     (iVar13 = *(int *)(iVar16 + 0x30) * (iVar8 + -1) + (iVar8 + -2) * *(int *)(iVar16 + 0x34),
     iVar13 < iVar5)) {
    iVar13 = iVar5;
  }
  uVar15 = *(undefined4 *)(this + 0x70);
  uVar17 = *(undefined4 *)(this + 0x78);
  uVar19 = *(undefined4 *)(iVar16 + 0x68);
  uVar3 = AbyssEngine::String::String(aSStack_98,(char *)(DAT_0018112c + 0x180f92),false);
  iVar13 = iVar10 * 2 + iVar13;
  Layout::drawBox(iVar16,2,uVar15,uVar17,uVar19,iVar13,uVar3);
  AbyssEngine::String::~String(aSStack_98);
  AbyssEngine::PaintCanvas::SetColor(*puVar20);
  AbyssEngine::PaintCanvas::DrawRectangle
            ((PaintCanvas *)*puVar20,*(int *)(this + 0x70),*(int *)(this + 0x78),
             *(int *)(*piVar2 + 0x68),iVar13);
  AbyssEngine::PaintCanvas::SetColor(*puVar20);
  ImageFactory::drawChar
            ((ImageFactory *)*puVar18,*(Array **)(this + 0x3c),
             *(int *)(*piVar2 + 0x4c) + *(int *)(this + 0x70),
             *(int *)(this + 0x78) + *(int *)(*piVar2 + 0x4c),true);
  if (((uVar4 == 1) || (this[0x36] != (SpaceLounge)0x0)) || (*(int *)(this + 0x14) == 3)) {
    TouchButton::setVisible((TouchButton *)**(undefined4 **)(*(int *)(this + 0x5c) + 4),true);
  }
  else {
    TouchButton::setVisible((TouchButton *)**(undefined4 **)(*(int *)(this + 0x5c) + 4),true);
    TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)(this + 0x5c) + 4) + 4),true);
    TouchButton::draw();
    TouchButton::draw();
    if (10 < uVar4) goto LAB_00181424;
    if ((1 << (uVar4 & 0xff) & 0x60cU) == 0) {
      if (uVar4 != 0) goto LAB_00181424;
      Agent::getMission(*(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4)
                       );
      iVar13 = Mission::isOutsideMission();
      if (iVar13 == 0) goto LAB_00181424;
      Agent::getMission(*(Agent **)(*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4)
                       );
      iVar13 = Mission::getType();
      if (iVar13 == 0xc) goto LAB_00181424;
      TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)(this + 0x5c) + 4) + 0xc),true);
      TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)(this + 0x5c) + 4) + 0x10),true);
      TouchButton::draw();
    }
    else {
      TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)(this + 0x5c) + 4) + 8),true);
    }
  }
  TouchButton::draw();
LAB_00181424:
  if (*piVar12 == local_44) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00180d38: push {r4,r5,r6,r7,lr}
  00180d3a: add r7,sp,#0xc
  00180d3c: push {r7,r8,r9,r10,r11}
  00180d40: vpush {d8,d9,d10}
  00180d44: sub sp,#0x78
  00180d46: mov r4,r0
  00180d48: ldr r0,[0x00181108]
  00180d4a: add r0,pc
  00180d4c: ldr r5,[r0,#0x0]
  00180d4e: ldr r0,[r5,#0x0]
  00180d50: str r0,[sp,#0x74]
  00180d52: ldr r0,[r4,#0x14]
  00180d54: cmp r0,#0x0
  00180d56: beq.w 0x00180e82
  00180d5a: str r5,[sp,#0x1c]
  00180d5c: mov.w r1,#0xffffffff
  00180d60: ldr r0,[0x0018110c]
  00180d62: add r0,pc
  00180d64: ldr.w r10,[r0,#0x0]
  00180d68: ldr.w r0,[r10,#0x0]
  00180d6c: blx 0x0006fac0
  00180d70: ldr r0,[0x00181110]
  00180d72: movs r2,#0x0
  00180d74: add r0,pc
  00180d76: ldr r0,[r0,#0x0]
  00180d78: str r0,[sp,#0x18]
  00180d7a: ldr r5,[r0,#0x0]
  00180d7c: add r0,sp,#0x2c
  00180d7e: ldrd r11,r8,[r4,#0x70]
  00180d82: ldrd r6,r9,[r5,#0x68]
  00180d86: ldr r1,[0x00181114]
  00180d88: add r1,pc
  00180d8a: blx 0x0006ee18
  00180d8e: strd r6,r9,[sp,#0x0]
  00180d92: movs r1,#0x2
  00180d94: str r0,[sp,#0x8]
  00180d96: mov r0,r5
  00180d98: mov r2,r11
  00180d9a: mov r3,r8
  00180d9c: blx 0x0007462c
  00180da0: add r0,sp,#0x2c
  00180da2: blx 0x0006ee30
  00180da6: ldr.w r0,[r10,#0x0]
  00180daa: ldr r1,[0x00181118]
  00180dac: blx 0x0006fac0
  00180db0: ldr.w r9,[sp,#0x18]
  00180db4: ldr.w r0,[r9,#0x0]
  00180db8: ldrd r1,r2,[r4,#0x70]
  00180dbc: ldrd r3,r6,[r0,#0x68]
  00180dc0: ldr.w r0,[r10,#0x0]
  00180dc4: str r6,[sp,#0x0]
  00180dc6: blx 0x00074e18
  00180dca: ldr.w r0,[r10,#0x0]
  00180dce: mov.w r1,#0xffffffff
  00180dd2: blx 0x0006fac0
  00180dd6: ldr r0,[0x0018111c]
  00180dd8: ldr.w r1,[r9,#0x0]
  00180ddc: add r0,pc
  00180dde: ldr r2,[r4,#0x38]
  00180de0: ldr.w r8,[r0,#0x0]
  00180de4: ldr r3,[r1,#0x4c]
  00180de6: ldr r1,[r4,#0x20]
  00180de8: ldr.w r0,[r8,#0x0]
  00180dec: ldrd r6,r5,[r4,#0x70]
  00180df0: ldr r2,[r2,#0x4]
  00180df2: ldr.w r1,[r2,r1,lsl #0x2]
  00180df6: movs r2,#0x0
  00180df8: str r2,[sp,#0x0]
  00180dfa: adds r2,r3,r6
  00180dfc: add r3,r5
  00180dfe: blx 0x00075b74
  00180e02: ldr r0,[r4,#0x60]
  00180e04: blx 0x00075b80
  00180e08: ldr r0,[r4,#0x14]
  00180e0a: ldr r5,[sp,#0x1c]
  00180e0c: bic r0,r0,#0x1
  00180e10: cmp r0,#0x2
  00180e12: bne.w 0x00181424
  00180e16: ldr r0,[r4,#0x5c]
  00180e18: mov r6,r9
  00180e1a: ldr r1,[0x00181120]
  00180e1c: str.w r8,[sp,#0xc]
  00180e20: ldr r0,[r0,#0x4]
  00180e22: ldr r0,[r0,#0x0]
  00180e24: blx 0x00077284
  00180e28: ldrd r0,r1,[r4,#0x20]
  00180e2c: ldr r1,[r1,#0x4]
  00180e2e: ldr.w r0,[r1,r0,lsl #0x2]
  00180e32: blx 0x00071ae8
  00180e36: mov r8,r0
  00180e38: ldr r0,[r4,#0x14]
  00180e3a: cmp r0,#0x2
  00180e3c: bne 0x00180f14
  00180e3e: ldr r0,[r4,#0x5c]
  00180e40: ldrd r2,r1,[r4,#0x80]
  00180e44: ldr r0,[r0,#0x4]
  00180e46: ldr r0,[r0,#0x0]
  00180e48: blx 0x00076498
  00180e4c: ldr r0,[r4,#0x5c]
  00180e4e: ldr r1,[r4,#0x6c]
  00180e50: ldrd r2,r3,[r4,#0x80]
  00180e54: ldr r0,[r0,#0x4]
  00180e56: add r1,r3
  00180e58: movs r3,#0x12
  00180e5a: ldr r0,[r0,#0x4]
  00180e5c: blx 0x00076d80
  00180e60: movs r0,#0x0
  00180e62: cmp.w r8,#0xa
  00180e66: str r0,[r4,#0x68]
  00180e68: bhi.w 0x0018119e
  00180e6c: movs r0,#0x1
  00180e6e: movw r1,#0x60c
  00180e72: lsl.w r0,r0,r8
  00180e76: tst r0,r1
  00180e78: beq.w 0x001810ec
  00180e7c: movs r0,#0x3
  00180e7e: str r0,[r4,#0x68]
  00180e80: b 0x00180f36
  00180e82: ldr.w r0,[r4,#0x88]
  00180e86: cmp r0,#0x0
  00180e88: blt.w 0x00181424
  00180e8c: str r5,[sp,#0x1c]
  00180e8e: ldr r1,[0x00181124]
  00180e90: ldr r2,[r4,#0x24]
  00180e92: ldr r3,[r4,#0x40]
  00180e94: add r1,pc
  00180e96: ldr r4,[0x00181128]
  00180e98: ldr r5,[r1,#0x0]
  00180e9a: add r4,pc
  00180e9c: ldr r1,[r2,#0x4]
  00180e9e: movs r2,#0x4
  00180ea0: ldr r3,[r3,#0x4]
  00180ea2: orr.w r2,r2,r0, lsl #0x3
  00180ea6: ldr r6,[r4,#0x0]
  00180ea8: str r6,[sp,#0x14]
  00180eaa: ldr r2,[r3,r2]
  00180eac: ldr.w r3,[r3,r0,lsl #0x3]
  00180eb0: ldr.w r8,[r1,r0,lsl #0x2]
  00180eb4: ldr r6,[r6,#0x0]
  00180eb6: ldr r0,[r5,#0x0]
  00180eb8: mov.w r1,#0xffffffff
  00180ebc: str r5,[sp,#0x18]
  00180ebe: ldr.w r9,[r6,#0x94]
  00180ec2: vldr.32 s16,[r2]
  00180ec6: vldr.32 s18,[r2,#0x4]
  00180eca: vldr.32 s20,[r3]
  00180ece: blx 0x0006fac0
  00180ed2: lsl.w r10,r9,#0x1
  00180ed6: vmov.f32 s0,0x3f000000
  00180eda: vmov s4,r10
  00180ede: mov r0,r8
  00180ee0: vsub.f32 s2,s16,s20
  00180ee4: vcvt.f32.s32 s4,s4
  00180ee8: vmla.f32 s20,s2,s0
  00180eec: vsub.f32 s0,s18,s4
  00180ef0: vcvt.s32.f32 s18,s20
  00180ef4: vcvt.s32.f32 s16,s0
  00180ef8: blx 0x00077d1c
  00180efc: cbnz r0,0x00180f0a
  00180efe: mov r0,r8
  00180f00: blx 0x00071ba8
  00180f04: cmp r0,#0x0
  00180f06: beq.w 0x00181074
  00180f0a: add r0,sp,#0x68
  00180f0c: mov r1,r8
  00180f0e: blx 0x00071c2c
  00180f12: b 0x00181096
  00180f14: ldr r0,[r4,#0x5c]
  00180f16: movs r1,#0x1
  00180f18: str r1,[r4,#0x68]
  00180f1a: ldr r0,[r0,#0x4]
  00180f1c: mov.w r1,#0xffffffff
  00180f20: ldr r0,[r0,#0x0]
  00180f22: blx 0x00077284
  00180f26: ldr r0,[r4,#0x5c]
  00180f28: ldr r2,[r4,#0x7c]
  00180f2a: ldr.w r1,[r4,#0x84]
  00180f2e: ldr r0,[r0,#0x4]
  00180f30: ldr r0,[r0,#0x0]
  00180f32: blx 0x00076498
  00180f36: movs r5,#0x0
  00180f38: b 0x00180f48
  00180f3a: ldr r0,[r0,#0x4]
  00180f3c: movs r1,#0x0
  00180f3e: ldr.w r0,[r0,r5,lsl #0x2]
  00180f42: blx 0x00074e48
  00180f46: adds r5,#0x1
  00180f48: ldr r0,[r4,#0x5c]
  00180f4a: ldr r1,[r0,#0x0]
  00180f4c: cmp r5,r1
  00180f4e: bcc 0x00180f3a
  00180f50: strd r8,r10,[sp,#0x10]
  00180f54: ldr r6,[r6,#0x0]
  00180f56: ldr r1,[r4,#0x68]
  00180f58: ldr r2,[r6,#0x4c]
  00180f5a: cmp r1,#0x2
  00180f5c: ldr.w r0,[r6,#0x2d8]
  00180f60: lsl.w r8,r2,#0x1
  00180f64: ble 0x00180f7c
  00180f66: subs r5,r1,#0x2
  00180f68: ldrd r2,r3,[r6,#0x30]
  00180f6c: subs r1,#0x1
  00180f6e: muls r3,r5
  00180f70: mla r11,r2,r1,r3
  00180f74: cmp r0,r11
  00180f76: it gt
  00180f78: mov.gt r11,r0
  00180f7a: b 0x00180f7e
  00180f7c: mov r11,r0
  00180f7e: ldr r5,[r4,#0x70]
  00180f80: add r0,sp,#0x20
  00180f82: ldr.w r9,[r4,#0x78]
  00180f86: movs r2,#0x0
  00180f88: ldr.w r10,[r6,#0x68]
  00180f8c: ldr r1,[0x0018112c]
  00180f8e: add r1,pc
  00180f90: blx 0x0006ee18
  00180f94: add r8,r11
  00180f96: strd r10,r8,[sp,#0x0]
  00180f9a: movs r1,#0x2
  00180f9c: str r0,[sp,#0x8]
  00180f9e: mov r0,r6
  00180fa0: mov r2,r5
  00180fa2: mov r3,r9
  00180fa4: blx 0x0007462c
  00180fa8: add r0,sp,#0x20
  00180faa: blx 0x0006ee30
  00180fae: ldr r5,[sp,#0x14]
  00180fb0: ldr r1,[0x00181118]
  00180fb2: ldr r0,[r5,#0x0]
  00180fb4: blx 0x0006fac0
  00180fb8: ldr r6,[sp,#0x18]
  00180fba: ldr r1,[r4,#0x70]
  00180fbc: ldr r2,[r4,#0x78]
  00180fbe: ldr r3,[r6,#0x0]
  00180fc0: ldr r0,[r5,#0x0]
  00180fc2: ldr r3,[r3,#0x68]
  00180fc4: str.w r8,[sp,#0x0]
  00180fc8: blx 0x00074e18
  00180fcc: ldr r0,[r5,#0x0]
  00180fce: mov.w r1,#0xffffffff
  00180fd2: blx 0x0006fac0
  00180fd6: ldr r2,[r6,#0x0]
  00180fd8: ldr r0,[sp,#0xc]
  00180fda: ldr r3,[r4,#0x70]
  00180fdc: ldr r5,[r2,#0x4c]
  00180fde: movs r2,#0x1
  00180fe0: ldr r6,[r4,#0x78]
  00180fe2: ldr r1,[r4,#0x3c]
  00180fe4: ldr r0,[r0,#0x0]
  00180fe6: str r2,[sp,#0x0]
  00180fe8: adds r2,r5,r3
  00180fea: adds r3,r6,r5
  00180fec: blx 0x00075b74
  00180ff0: ldr r6,[sp,#0x10]
  00180ff2: ldr r5,[sp,#0x1c]
  00180ff4: cmp r6,#0x1
  00180ff6: beq 0x00181004
  00180ff8: ldrb.w r0,[r4,#0x36]
  00180ffc: cbnz r0,0x00181004
  00180ffe: ldr r0,[r4,#0x14]
  00181000: cmp r0,#0x3
  00181002: bne 0x0018101c
  00181004: ldr r0,[r4,#0x5c]
  00181006: movs r1,#0x1
  00181008: ldr r0,[r0,#0x4]
  0018100a: ldr r0,[r0,#0x0]
  0018100c: blx 0x00074e48
  00181010: ldr r0,[r4,#0x5c]
  00181012: ldr r0,[r0,#0x4]
  00181014: ldr r0,[r0,#0x0]
  00181016: blx 0x000746e0
  0018101a: b 0x00181424
  0018101c: ldr r0,[r4,#0x5c]
  0018101e: movs r1,#0x1
  00181020: mov.w r8,#0x1
  00181024: ldr r0,[r0,#0x4]
  00181026: ldr r0,[r0,#0x0]
  00181028: blx 0x00074e48
  0018102c: ldr r0,[r4,#0x5c]
  0018102e: movs r1,#0x1
  00181030: ldr r0,[r0,#0x4]
  00181032: ldr r0,[r0,#0x4]
  00181034: blx 0x00074e48
  00181038: ldr r0,[r4,#0x5c]
  0018103a: ldr r0,[r0,#0x4]
  0018103c: ldr r0,[r0,#0x0]
  0018103e: blx 0x000746e0
  00181042: ldr r0,[r4,#0x5c]
  00181044: ldr r0,[r0,#0x4]
  00181046: ldr r0,[r0,#0x4]
  00181048: blx 0x000746e0
  0018104c: cmp r6,#0xa
  0018104e: bhi.w 0x00181424
  00181052: lsl.w r0,r8,r6
  00181056: movw r1,#0x60c
  0018105a: tst r0,r1
  0018105c: beq.w 0x001811f0
  00181060: ldr r0,[r4,#0x5c]
  00181062: movs r1,#0x1
  00181064: ldr r0,[r0,#0x4]
  00181066: ldr r0,[r0,#0x8]
  00181068: blx 0x00074e48
  0018106c: ldr r0,[r4,#0x5c]
  0018106e: ldr r0,[r0,#0x4]
  00181070: ldr r0,[r0,#0x8]
  00181072: b 0x00181016
  00181074: ldr r0,[0x00181130]
  00181076: add r0,pc
  00181078: ldr r0,[r0,#0x0]
  0018107a: ldr r4,[r0,#0x0]
  0018107c: mov r0,r8
  0018107e: blx 0x00071bd8
  00181082: add.w r1,r0,#0x196
  00181086: mov r0,r4
  00181088: blx 0x00072f70
  0018108c: mov r1,r0
  0018108e: add r0,sp,#0x68
  00181090: movs r2,#0x0
  00181092: blx 0x0006f028
  00181096: mov r0,r8
  00181098: blx 0x00071bb4
  0018109c: cbz r0,0x001810c6
  0018109e: ldr r0,[0x00181134]
  001810a0: add r0,pc
  001810a2: ldr r0,[r0,#0x0]
  001810a4: ldr r4,[r0,#0x0]
  001810a6: mov r0,r8
  001810a8: blx 0x00071bb4
  001810ac: blx 0x00072874
  001810b0: add.w r1,r0,#0x162
  001810b4: mov r0,r4
  001810b6: blx 0x00072f70
  001810ba: mov r1,r0
  001810bc: add r0,sp,#0x5c
  001810be: movs r2,#0x0
  001810c0: blx 0x0006f028
  001810c4: b 0x0018125c
  001810c6: mov r0,r8
  001810c8: blx 0x00071ae8
  001810cc: cmp r0,#0x6
  001810ce: bne 0x0018113c
  001810d0: ldr r0,[0x00181138]
  001810d2: add r0,pc
  001810d4: ldr r0,[r0,#0x0]
  001810d6: ldr r0,[r0,#0x0]
  001810d8: mov.w r1,#0x132
  001810dc: blx 0x00072f70
  001810e0: mov r1,r0
  001810e2: add r0,sp,#0x5c
  001810e4: movs r2,#0x0
  001810e6: blx 0x0006f028
  001810ea: b 0x0018125c
  001810ec: cmp.w r8,#0x0
  001810f0: beq 0x00181162
  001810f2: cmp.w r8,#0x1
  001810f6: bne 0x0018119e
  001810f8: movs r3,#0x1
  001810fa: ldr r0,[r4,#0x5c]
  001810fc: ldr r2,[r4,#0x7c]
  001810fe: ldr.w r1,[r4,#0x84]
  00181102: str r3,[r4,#0x68]
  00181104: b 0x00180f2e
  0018113c: mov r0,r8
  0018113e: blx 0x00071ae8
  00181142: cmp r0,#0x2
  00181144: bne 0x001811ca
  00181146: ldr r0,[0x001814ac]
  00181148: add r0,pc
  0018114a: ldr r0,[r0,#0x0]
  0018114c: ldr r0,[r0,#0x0]
  0018114e: movw r1,#0x131
  00181152: blx 0x00072f70
  00181156: mov r1,r0
  00181158: add r0,sp,#0x5c
  0018115a: movs r2,#0x0
  0018115c: blx 0x0006f028
  00181160: b 0x0018125c
  00181162: ldrd r0,r1,[r4,#0x20]
  00181166: ldr r1,[r1,#0x4]
  00181168: ldr.w r0,[r1,r0,lsl #0x2]
  0018116c: blx 0x00071bb4
  00181170: cbz r0,0x0018119e
  00181172: ldrd r0,r1,[r4,#0x20]
  00181176: ldr r1,[r1,#0x4]
  00181178: ldr.w r0,[r1,r0,lsl #0x2]
  0018117c: blx 0x00071bb4
  00181180: blx 0x00077d70
  00181184: cbz r0,0x0018119e
  00181186: ldrd r0,r1,[r4,#0x20]
  0018118a: ldr r1,[r1,#0x4]
  0018118c: ldr.w r0,[r1,r0,lsl #0x2]
  00181190: blx 0x00071bb4
  00181194: blx 0x00072874
  00181198: cmp r0,#0xc
  0018119a: bne.w 0x00181440
  0018119e: ldr r0,[r4,#0x5c]
  001811a0: movs r3,#0x2
  001811a2: ldr r2,[r4,#0x7c]
  001811a4: ldr.w r1,[r4,#0x84]
  001811a8: str r3,[r4,#0x68]
  001811aa: ldr r0,[r0,#0x4]
  001811ac: ldr r0,[r0,#0x0]
  001811ae: blx 0x00076498
  001811b2: ldr r0,[r4,#0x5c]
  001811b4: ldr r1,[r4,#0x6c]
  001811b6: ldr.w r3,[r4,#0x84]
  001811ba: ldr r0,[r0,#0x4]
  001811bc: ldr r2,[r4,#0x7c]
  001811be: add r1,r3
  001811c0: movs r3,#0x12
  001811c2: ldr r0,[r0,#0x4]
  001811c4: blx 0x00076d80
  001811c8: b 0x00180f36
  001811ca: mov r0,r8
  001811cc: blx 0x00071ae8
  001811d0: cmp r0,#0x7
  001811d2: bne 0x00181250
  001811d4: ldr r0,[0x001814b0]
  001811d6: add r0,pc
  001811d8: ldr r0,[r0,#0x0]
  001811da: ldr r0,[r0,#0x0]
  001811dc: mov.w r1,#0x374
  001811e0: blx 0x00072f70
  001811e4: mov r1,r0
  001811e6: add r0,sp,#0x5c
  001811e8: movs r2,#0x0
  001811ea: blx 0x0006f028
  001811ee: b 0x0018125c
  001811f0: cmp r6,#0x0
  001811f2: bne.w 0x00181424
  001811f6: ldrd r0,r1,[r4,#0x20]
  001811fa: ldr r1,[r1,#0x4]
  001811fc: ldr.w r0,[r1,r0,lsl #0x2]
  00181200: blx 0x00071bb4
  00181204: blx 0x00077d70
  00181208: cmp r0,#0x0
  0018120a: beq.w 0x00181424
  0018120e: ldrd r0,r1,[r4,#0x20]
  00181212: ldr r1,[r1,#0x4]
  00181214: ldr.w r0,[r1,r0,lsl #0x2]
  00181218: blx 0x00071bb4
  0018121c: blx 0x00072874
  00181220: cmp r0,#0xc
  00181222: beq.w 0x00181424
  00181226: ldr r0,[r4,#0x5c]
  00181228: movs r1,#0x1
  0018122a: ldr r0,[r0,#0x4]
  0018122c: ldr r0,[r0,#0xc]
  0018122e: blx 0x00074e48
  00181232: ldr r0,[r4,#0x5c]
  00181234: movs r1,#0x1
  00181236: ldr r0,[r0,#0x4]
  00181238: ldr r0,[r0,#0x10]
  0018123a: blx 0x00074e48
  0018123e: ldr r0,[r4,#0x5c]
  00181240: ldr r0,[r0,#0x4]
  00181242: ldr r0,[r0,#0xc]
  00181244: blx 0x000746e0
  00181248: ldr r0,[r4,#0x5c]
  0018124a: ldr r0,[r0,#0x4]
  0018124c: ldr r0,[r0,#0x10]
  0018124e: b 0x00181016
  00181250: ldr r1,[0x001814b4]
  00181252: add r1,pc
  00181254: add r0,sp,#0x5c
  00181256: movs r2,#0x0
  00181258: blx 0x0006ee18
  0018125c: ldr r0,[0x001814b8]
  0018125e: add r0,pc
  00181260: ldr r0,[r0,#0x0]
  00181262: str r0,[sp,#0x10]
  00181264: ldr r1,[r0,#0x0]
  00181266: ldr r0,[sp,#0x18]
  00181268: ldr r0,[r0,#0x0]
  0018126a: add r2,sp,#0x68
  0018126c: blx 0x0006faa8
  00181270: mov r6,r0
  00181272: mov r0,r8
  00181274: blx 0x00077d1c
  00181278: cbz r0,0x001812b2
  0018127a: ldr r0,[sp,#0x10]
  0018127c: ldr r4,[r0,#0x0]
  0018127e: ldr r0,[sp,#0x18]
  00181280: ldr r5,[r0,#0x0]
  00181282: ldr r1,[0x001814bc]
  00181284: add r1,pc
  00181286: add r0,sp,#0x44
  00181288: movs r2,#0x0
  0018128a: blx 0x0006ee18
  0018128e: add r0,sp,#0x50
  00181290: add r1,sp,#0x5c
  00181292: add r2,sp,#0x44
  00181294: blx 0x0006ef98
  00181298: add r2,sp,#0x50
  0018129a: mov r0,r5
  0018129c: mov r1,r4
  0018129e: blx 0x0006faa8
  001812a2: mov r4,r0
  001812a4: add r0,sp,#0x50
  001812a6: blx 0x0006ee30
  001812aa: add r0,sp,#0x44
  001812ac: blx 0x0006ee30
  001812b0: add r6,r4
  001812b2: vmov r5,s18
  001812b6: ldr r0,[sp,#0x14]
  001812b8: ldr r4,[r0,#0x0]
  001812ba: ldr r0,[r4,#0x28]
  001812bc: sub.w r1,r5,r9
  001812c0: cmp r1,r0
  001812c2: bge 0x001812ca
  001812c4: add.w r5,r0,r9
  001812c8: b 0x001812e4
  001812ca: ldr r1,[0x001814c0]
  001812cc: add r1,pc
  001812ce: ldr r1,[r1,#0x0]
  001812d0: ldr r1,[r1,#0x0]
  001812d2: subs r0,r1,r0
  001812d4: add.w r1,r9,r5
  001812d8: add r1,r6
  001812da: cmp r1,r0
  001812dc: itt gt
  001812de: add.gt.w r1,r6,r10
  001812e2: sub.gt r5,r0,r1
  001812e4: ldr r1,[0x001814c4]
  001812e6: ldr.w r11,[r4,#0x30]
  001812ea: add r1,pc
  001812ec: add r0,sp,#0x38
  001812ee: movs r2,#0x0
  001812f0: blx 0x0006ee18
  001812f4: vmov r2,s16
  001812f8: mov r3,r5
  001812fa: add r10,r6
  001812fc: mov r1,r9
  001812fe: subs r6,r3,r1
  00181300: str.w r8,[sp,#0xc]
  00181304: sub.w r5,r2,r9
  00181308: mov r9,r3
  0018130a: mov r8,r2
  0018130c: strd r10,r11,[sp,#0x0]
  00181310: movs r1,#0x2
  00181312: str r0,[sp,#0x8]
  00181314: mov r0,r4
  00181316: mov r2,r6
  00181318: mov r3,r5
  0018131a: blx 0x0007462c
  0018131e: add r0,sp,#0x38
  00181320: blx 0x0006ee30
  00181324: ldr r0,[sp,#0x18]
  00181326: ldr r1,[0x001814a4]
  00181328: ldr r0,[r0,#0x0]
  0018132a: blx 0x0006fac0
  0018132e: mov r11,r8
  00181330: ldr.w r8,[sp,#0xc]
  00181334: ldr r0,[sp,#0x14]
  00181336: ldr r1,[r0,#0x0]
  00181338: ldr r0,[sp,#0x18]
  0018133a: ldr r1,[r1,#0x30]
  0018133c: ldr r0,[r0,#0x0]
  0018133e: str r1,[sp,#0x0]
  00181340: mov r1,r6
  00181342: mov r2,r5
  00181344: mov r3,r10
  00181346: blx 0x00074e18
  0018134a: ldr r0,[sp,#0x18]
  0018134c: ldr r0,[r0,#0x0]
  0018134e: mov.w r1,#0xffffffff
  00181352: blx 0x0006fac0
  00181356: ldr r0,[sp,#0x10]
  00181358: ldr r6,[r0,#0x0]
  0018135a: ldr r0,[sp,#0x18]
  0018135c: ldr r4,[r0,#0x0]
  0018135e: mov r0,r8
  00181360: blx 0x00077d1c
  00181364: cbz r0,0x00181374
  00181366: ldr r1,[0x001814c8]
  00181368: add r1,pc
  0018136a: add r0,sp,#0x44
  0018136c: movs r2,#0x0
  0018136e: blx 0x0006ee18
  00181372: b 0x00181380
  00181374: ldr r1,[0x001814cc]
  00181376: add r1,pc
  00181378: add r0,sp,#0x44
  0018137a: movs r2,#0x0
  0018137c: blx 0x0006ee18
  00181380: add r0,sp,#0x50
  00181382: add r1,sp,#0x68
  00181384: add r2,sp,#0x44
  00181386: blx 0x0006ef98
  0018138a: ldr r0,[sp,#0x14]
  0018138c: ldr r0,[r0,#0x0]
  0018138e: ldr.w r0,[r0,#0x2c0]
  00181392: movs r1,#0x0
  00181394: add r0,r11
  00181396: add r2,sp,#0x50
  00181398: strd r0,r1,[sp,#0x0]
  0018139c: mov r0,r4
  0018139e: mov r1,r6
  001813a0: mov r3,r9
  001813a2: blx 0x0006fe14
  001813a6: add r0,sp,#0x50
  001813a8: blx 0x0006ee30
  001813ac: add r0,sp,#0x44
  001813ae: blx 0x0006ee30
  001813b2: mov r0,r8
  001813b4: blx 0x00077d1c
  001813b8: cbz r0,0x00181416
  001813ba: ldr r0,[sp,#0x18]
  001813bc: ldr r1,[0x001814a8]
  001813be: ldr r0,[r0,#0x0]
  001813c0: blx 0x0006fac0
  001813c4: ldr r0,[sp,#0x10]
  001813c6: ldr r4,[r0,#0x0]
  001813c8: ldr r0,[sp,#0x18]
  001813ca: ldr r6,[r0,#0x0]
  001813cc: ldr r1,[0x001814d0]
  001813ce: add r1,pc
  001813d0: add r0,sp,#0x44
  001813d2: movs r2,#0x0
  001813d4: blx 0x0006ee18
  001813d8: add r0,sp,#0x50
  001813da: add r1,sp,#0x68
  001813dc: add r2,sp,#0x44
  001813de: blx 0x0006ef98
  001813e2: add r2,sp,#0x50
  001813e4: mov r0,r6
  001813e6: mov r1,r4
  001813e8: blx 0x0006faa8
  001813ec: ldr r1,[sp,#0x14]
  001813ee: ldr r1,[r1,#0x0]
  001813f0: ldr.w r1,[r1,#0x2c0]
  001813f4: movs r2,#0x0
  001813f6: add r1,r11
  001813f8: strd r1,r2,[sp,#0x0]
  001813fc: add.w r3,r0,r9
  00181400: add r2,sp,#0x5c
  00181402: mov r0,r6
  00181404: mov r1,r4
  00181406: blx 0x0006fe14
  0018140a: add r0,sp,#0x50
  0018140c: blx 0x0006ee30
  00181410: add r0,sp,#0x44
  00181412: blx 0x0006ee30
  00181416: add r0,sp,#0x5c
  00181418: blx 0x0006ee30
  0018141c: add r0,sp,#0x68
  0018141e: blx 0x0006ee30
  00181422: ldr r5,[sp,#0x1c]
  00181424: ldr r0,[sp,#0x74]
  00181426: ldr r1,[r5,#0x0]
  00181428: subs r0,r1,r0
  0018142a: itttt eq
  0018142c: add.eq sp,#0x78
  0018142e: vpop.eq {d8,d9,d10}
  00181432: add.eq sp,#0x4
  00181434: pop.eq.w {r8,r9,r10,r11}
  00181438: it eq
  0018143a: pop.eq {r4,r5,r6,r7,pc}
  0018143c: blx 0x0006e824
  00181440: ldrb.w r0,[r4,#0x36]
  00181444: movs r1,#0x1
  00181446: cmp r0,#0x0
  00181448: it eq
  0018144a: mov.eq r1,#0x4
  0018144c: str r1,[r4,#0x68]
  0018144e: beq.w 0x00180f36
  00181452: ldr r0,[r4,#0x5c]
  00181454: b 0x00180f1a
```
