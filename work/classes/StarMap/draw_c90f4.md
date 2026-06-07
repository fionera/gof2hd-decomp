# StarMap::draw
elf 0xc90f4 body 2460
Sig: undefined __stdcall draw(void)

## decompile
```c

/* StarMap::draw() */

void StarMap::draw(void)

{
  StarMap *in_r0;
  int iVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  uint *puVar6;
  Vector *this;
  Vector *this_00;
  uint uVar7;
  undefined8 *puVar8;
  uint *puVar9;
  uint *puVar10;
  uint uVar11;
  int iVar12;
  String *pSVar13;
  undefined4 uVar14;
  StarMap SVar15;
  uint extraout_r2;
  uint extraout_r2_00;
  int *piVar16;
  int extraout_r3;
  int extraout_r3_00;
  int *piVar17;
  int iVar18;
  code *pcVar19;
  String *this_01;
  undefined4 *puVar20;
  String *pSVar21;
  uint uVar22;
  int *piVar23;
  int iVar24;
  bool bVar25;
  undefined8 uVar26;
  String aSStack_bc [12];
  String aSStack_b0 [12];
  String aSStack_a4 [12];
  String aSStack_98 [12];
  String aSStack_8c [12];
  String aSStack_80 [12];
  String aSStack_74 [12];
  String aSStack_68 [12];
  String aSStack_5c [12];
  undefined8 local_50;
  undefined4 local_48;
  int local_44;
  
  uVar14 = 0;
  piVar16 = *(int **)(DAT_000d93e0 + 0xd910c);
  local_44 = *piVar16;
  iVar1 = *(int *)(in_r0 + 4);
  SVar15 = in_r0[0x138];
  if (iVar1 == 0) {
    uVar14 = 0xff;
  }
  *(undefined4 *)(in_r0 + 0x1a4) = uVar14;
  bVar25 = SVar15 == (StarMap)0x0;
  if (bVar25) {
    SVar15 = in_r0[0x139];
  }
  if (bVar25 && SVar15 == (StarMap)0x0) {
    SVar15 = (StarMap)0x0;
  }
  else {
    fVar2 = (float)AbyssEngine::EaseInOut::GetValue();
    fVar3 = (float)AbyssEngine::EaseInOut::GetMinValue(*(EaseInOut **)(in_r0 + 0x184));
    fVar4 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar5 = (float)AbyssEngine::EaseInOut::GetMinValue(*(EaseInOut **)(in_r0 + 0x184));
    SVar15 = in_r0[0x138];
    fVar3 = (fVar2 - fVar3) / (fVar4 - fVar5);
    fVar2 = 1.0 - fVar3;
    if (SVar15 == (StarMap)0x0) {
      fVar2 = fVar3;
    }
    iVar1 = *(int *)(in_r0 + 4);
    *(int *)(in_r0 + 0x1a4) = (int)(fVar2 * DAT_000d93e4);
  }
  if ((iVar1 != 3 || SVar15 != (StarMap)0x0) || (in_r0[0x139] != (StarMap)0x0)) {
    puVar6 = *(uint **)(DAT_000d93e8 + 0xd91b8);
    AbyssEngine::PaintCanvas::SetColor((uchar)*puVar6,0xff,0xff,0xff);
    this = (Vector *)(in_r0 + 0x84);
    this_00 = (Vector *)(in_r0 + 0x78);
    puVar20 = *(undefined4 **)(DAT_000d93ec + 0xd91e2);
    for (uVar22 = 0; uVar22 < **(uint **)(in_r0 + 0x68); uVar22 = uVar22 + 1) {
      puVar9 = (uint *)Status::getSystemVisibilities((Status *)*puVar20);
      if (((((puVar9 != (uint *)0x0) && (uVar22 < *puVar9)) &&
           (*(char *)(puVar9[1] + uVar22) != '\0')) &&
          (((in_r0[0x118] == (StarMap)0x0 || (*(uint *)(in_r0 + 0x114) != uVar22)) ||
           (3999 < *(int *)(in_r0 + 0x11c))))) &&
         (puVar10 = (uint *)SolarSystem::getRoutes(), puVar10 != (uint *)0x0)) {
        puVar8 = *(undefined8 **)(*(int *)(*(int *)(in_r0 + 0x194) + 4) + uVar22 * 4);
        local_50 = *puVar8;
        local_48 = *(undefined4 *)(puVar8 + 1);
        AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_50);
        for (uVar11 = 0; uVar11 < *puVar10; uVar11 = uVar11 + 1) {
          uVar7 = *(uint *)(puVar10[1] + uVar11 * 4);
          if (((uVar7 < *puVar9) && (*(char *)(puVar9[1] + uVar7) != '\0')) &&
             ((uVar7 < **(uint **)(in_r0 + 0x194) &&
              (((in_r0[0x118] == (StarMap)0x0 || (*(uint *)(in_r0 + 0x114) != uVar7)) ||
               (3999 < *(int *)(in_r0 + 0x11c))))))) {
            puVar8 = *(undefined8 **)((*(uint **)(in_r0 + 0x194))[1] + uVar7 * 4);
            local_50 = *puVar8;
            local_48 = *(undefined4 *)(puVar8 + 1);
            AbyssEngine::AEMath::Vector::operator=(this,(Vector *)&local_50);
            if ((-1 < (int)((uint)(*(float *)(in_r0 + 0x80) < 0.0) << 0x1f)) ||
               (-1 < (int)((uint)(*(float *)(in_r0 + 0x8c) < 0.0) << 0x1f))) {
              AbyssEngine::PaintCanvas::DrawLine
                        ((PaintCanvas *)*puVar6,(int)*(float *)(in_r0 + 0x78),
                         (int)*(float *)(in_r0 + 0x7c),(int)*(float *)(in_r0 + 0x84),
                         (int)*(float *)(in_r0 + 0x88));
            }
          }
        }
      }
    }
    AbyssEngine::PaintCanvas::SetColor((uchar)*puVar6,0xff,0xff,0xff);
    iVar18 = *(int *)(in_r0 + 0x194);
    Status::getSystem();
    iVar1 = SolarSystem::getIndex();
    puVar8 = *(undefined8 **)(*(int *)(iVar18 + 4) + iVar1 * 4);
    local_50 = *puVar8;
    local_48 = *(undefined4 *)(puVar8 + 1);
    AbyssEngine::AEMath::Vector::operator=(this,(Vector *)&local_50);
    Status::getSystem();
    puVar9 = (uint *)SolarSystem::getRoutes();
    fVar2 = DAT_000d93f0;
    for (uVar22 = 0; uVar22 < **(uint **)(in_r0 + 0x54); uVar22 = uVar22 + 1) {
      puVar10 = (uint *)Status::getSystemVisibilities((Status *)*puVar20);
      if (((puVar10 != (uint *)0x0) && (uVar22 < **(uint **)(in_r0 + 0x194))) &&
         ((uVar22 < *puVar10 && (*(char *)(puVar10[1] + uVar22) != '\0')))) {
        puVar8 = *(undefined8 **)((*(uint **)(in_r0 + 0x194))[1] + uVar22 * 4);
        local_50 = *puVar8;
        local_48 = *(undefined4 *)(puVar8 + 1);
        AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_50);
        if ((int)((uint)(*(float *)(in_r0 + 0x80) < 0.0) << 0x1f) < 0) {
          bVar25 = (int)((uint)(*(float *)(in_r0 + 0x8c) < 0.0) << 0x1f) < 0;
        }
        else {
          bVar25 = false;
        }
        if (((puVar9 != (uint *)0x0) && (!bVar25)) && (in_r0[0xab] == (StarMap)0x0)) {
          uVar11 = 0;
          do {
            if (*puVar9 <= uVar11) goto LAB_000d94be;
            iVar1 = uVar11 * 4;
            uVar11 = uVar11 + 1;
          } while (*(uint *)(puVar9[1] + iVar1) != uVar22);
          uVar26 = Status::getPlayingTime();
          uVar26 = FUN_001aae54((int)uVar26,(int)((ulonglong)uVar26 >> 0x20),10,0);
          FUN_001aae54((int)uVar26,(int)((ulonglong)uVar26 >> 0x20),99,0);
          fVar3 = (float)__aeabi_l2f(0x65 - extraout_r2,-(uint)(0x65 < extraout_r2) - extraout_r3);
          fVar3 = fVar3 / fVar2;
          AbyssEngine::PaintCanvas::SetColor((uchar)*puVar6,0xff,0xff,0xff);
          AbyssEngine::PaintCanvas::DrawLine
                    ((PaintCanvas *)*puVar6,(int)*(float *)(in_r0 + 0x84),
                     (int)*(float *)(in_r0 + 0x88),
                     (int)(*(float *)(in_r0 + 0x78) +
                          fVar3 * (*(float *)(in_r0 + 0x84) - *(float *)(in_r0 + 0x78))),
                     (int)(*(float *)(in_r0 + 0x7c) +
                          fVar3 * (*(float *)(in_r0 + 0x88) - *(float *)(in_r0 + 0x7c))));
        }
LAB_000d94be:
        if ((in_r0[0x138] == (StarMap)0x0) && (in_r0[0x139] == (StarMap)0x0)) {
          drawOnScreenInfo(in_r0,uVar22,false);
        }
      }
    }
    if (((*(int *)(in_r0 + 4) == 0) && (in_r0[0x138] == (StarMap)0x0)) &&
       ((in_r0[0x139] == (StarMap)0x0 && (*(int *)(in_r0 + 0xa0) != 0)))) {
      uVar26 = Status::getPlayingTime();
      uVar26 = FUN_001aae54((int)uVar26,(int)((ulonglong)uVar26 >> 0x20),10,0);
      FUN_001aae54((int)uVar26,(int)((ulonglong)uVar26 >> 0x20),99,0);
      fVar3 = (float)__aeabi_l2f(0x65 - extraout_r2_00,
                                 -(uint)(0x65 < extraout_r2_00) - extraout_r3_00);
      uVar22 = *(uint *)(in_r0 + 0xe8);
      fVar3 = fVar3 / fVar2;
      if (fVar3 <= *(float *)(in_r0 + 0xec)) {
        puVar9 = *(uint **)(in_r0 + 0xa0);
      }
      else {
        puVar9 = *(uint **)(in_r0 + 0xa0);
        uVar11 = uVar22 + 1;
        *(uint *)(in_r0 + 0xe8) = uVar11;
        uVar22 = 0;
        if (uVar11 < *puVar9) {
          uVar22 = uVar11;
        }
        *(uint *)(in_r0 + 0xe8) = uVar22;
      }
      *(float *)(in_r0 + 0xec) = fVar3;
      if (uVar22 == *puVar9 - 1) {
        AbyssEngine::PaintCanvas::SetColor((uchar)*puVar6,0xff,0xff,'\0');
      }
      else {
        AbyssEngine::PaintCanvas::SetColor(*puVar6);
      }
      for (iVar1 = 0; uVar22 = *(uint *)(in_r0 + 0xe8), iVar1 < (int)uVar22; iVar1 = iVar1 + 1) {
        puVar8 = *(undefined8 **)
                  (*(int *)(*(int *)(in_r0 + 0x194) + 4) +
                  *(int *)(*(int *)(*(int *)(in_r0 + 0xa0) + 4) + iVar1 * 4) * 4);
        local_50 = *puVar8;
        local_48 = *(undefined4 *)(puVar8 + 1);
        AbyssEngine::AEMath::Vector::operator=(this,(Vector *)&local_50);
        puVar8 = *(undefined8 **)
                  (*(int *)(*(int *)(in_r0 + 0x194) + 4) +
                  *(int *)(*(int *)(*(int *)(in_r0 + 0xa0) + 4) + iVar1 * 4 + 4) * 4);
        local_50 = *puVar8;
        local_48 = *(undefined4 *)(puVar8 + 1);
        AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_50);
        AbyssEngine::PaintCanvas::DrawLine
                  ((PaintCanvas *)*puVar6,(int)*(float *)(in_r0 + 0x84),
                   (int)*(float *)(in_r0 + 0x88),(int)*(float *)(in_r0 + 0x78),
                   (int)*(float *)(in_r0 + 0x7c));
      }
      if (uVar22 < **(int **)(in_r0 + 0xa0) - 1U) {
        puVar8 = *(undefined8 **)
                  (*(int *)(*(int *)(in_r0 + 0x194) + 4) +
                  *(int *)((*(int **)(in_r0 + 0xa0))[1] + uVar22 * 4) * 4);
        local_50 = *puVar8;
        local_48 = *(undefined4 *)(puVar8 + 1);
        AbyssEngine::AEMath::Vector::operator=(this,(Vector *)&local_50);
        puVar8 = *(undefined8 **)
                  (*(int *)(*(int *)(in_r0 + 0x194) + 4) +
                  *(int *)(*(int *)(*(int *)(in_r0 + 0xa0) + 4) + *(int *)(in_r0 + 0xe8) * 4 + 4) *
                  4);
        local_50 = *puVar8;
        local_48 = *(undefined4 *)(puVar8 + 1);
        AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_50);
        AbyssEngine::PaintCanvas::DrawLine
                  ((PaintCanvas *)*puVar6,(int)*(float *)(in_r0 + 0x84),
                   (int)*(float *)(in_r0 + 0x88),
                   (int)(*(float *)(in_r0 + 0x78) +
                        fVar3 * (*(float *)(in_r0 + 0x84) - *(float *)(in_r0 + 0x78))),
                   (int)(*(float *)(in_r0 + 0x7c) +
                        fVar3 * (*(float *)(in_r0 + 0x88) - *(float *)(in_r0 + 0x7c))));
      }
    }
  }
  if ((-1 < *(int *)(in_r0 + 0x60)) &&
     ((in_r0[0x118] == (StarMap)0x0 || (3999 < *(int *)(in_r0 + 0x11c))))) {
    drawOnScreenInfo(in_r0,*(int *)(in_r0 + 0x60),false);
  }
  if ((*(int *)(in_r0 + 0xa4) != 0) && (*(int *)(in_r0 + 0x58) != 0)) {
    iVar1 = SolarSystem::hasNoOwner();
    if (iVar1 == 0) {
      puVar6 = *(uint **)(DAT_000d9968 + 0xd95ae);
      AbyssEngine::PaintCanvas::SetColor((uchar)*puVar6,0xff,0xff,0xff);
      piVar23 = *(int **)(DAT_000d996c + 0xd95c4);
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar6,*(int *)(in_r0 + 0x34),*(int *)(*piVar23 + 0x2c));
      iVar18 = *(int *)(*piVar23 + 0x2c);
      iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar6);
      AbyssEngine::PaintCanvas::SetColor((uchar)*puVar6,0xff,0x80,'\0');
      uVar22 = *puVar6;
      puVar20 = *(undefined4 **)(DAT_000d9970 + 0xd960a);
      pSVar21 = (String *)*puVar20;
      SolarSystem::getName();
      iVar1 = iVar1 + iVar18 * 2;
      AbyssEngine::PaintCanvas::DrawString
                (uVar22,pSVar21,(int)&local_50,iVar1,
                 (bool)((char)*(undefined4 *)(*piVar23 + 0x2c) +
                        (char)*(undefined4 *)(*piVar23 + 0xc) + '\x02'));
      AbyssEngine::String::~String((String *)&local_50);
      AbyssEngine::PaintCanvas::SetColor((uchar)*puVar6,0xff,0xff,0xff);
      piVar17 = *(int **)(DAT_000d9974 + 0xd966c);
      pSVar21 = (String *)*puVar20;
      uVar22 = *puVar6;
      iVar18 = *piVar17;
      SolarSystem::getRace();
      iVar18 = GameText::getText(iVar18);
      iVar12 = *piVar23;
      AbyssEngine::PaintCanvas::DrawString
                (uVar22,pSVar21,iVar18,iVar1,
                 (bool)((char)*(undefined4 *)(iVar12 + 0x2c) + (char)*(undefined4 *)(iVar12 + 0xc) +
                        (char)*(undefined4 *)(iVar12 + 4) + '\x02'));
      iVar18 = SolarSystem::getSecurityLevel
                         (*(SolarSystem **)
                           (*(int *)(*(int *)(in_r0 + 0x54) + 4) + *(int *)(in_r0 + 0x60) * 4));
      iVar12 = DAT_000d9978 + 0xd96e2 + iVar18 * 0xc;
      AbyssEngine::PaintCanvas::SetColor
                ((uchar)*puVar6,*(uchar *)(DAT_000d9978 + 0xd96e2 + iVar18 * 0xc),
                 *(uchar *)(iVar12 + 4),*(uchar *)(iVar12 + 8));
      pSVar21 = (String *)*puVar20;
      uVar22 = *puVar6;
      iVar18 = GameText::getText(*piVar17);
      iVar12 = *piVar23;
      AbyssEngine::PaintCanvas::DrawString
                (uVar22,pSVar21,iVar18,iVar1,
                 (bool)((char)*(undefined4 *)(iVar12 + 0x2c) + (char)*(undefined4 *)(iVar12 + 0xc) +
                        (char)*(undefined4 *)(iVar12 + 4) * '\x02' + '\x02'));
    }
    for (uVar22 = 0; uVar11 = *(uint *)(in_r0 + 100), uVar22 < **(uint **)(in_r0 + 0x58);
        uVar22 = uVar22 + 1) {
      if (uVar22 != uVar11) {
        drawOnScreenInfo(in_r0,uVar22,true);
      }
    }
    if (-1 < (int)uVar11) {
      drawOnScreenInfo(in_r0,uVar11,true);
    }
  }
  if (in_r0[0x108] != (StarMap)0x0) {
    drawKey(in_r0);
  }
  piVar17 = *(int **)(DAT_000d9b1c + 0xd9766);
  piVar23 = *(int **)(DAT_000d9b20 + 0xd9768);
  iVar1 = *piVar17;
  pSVar21 = (String *)GameText::getText(*piVar23);
  this_01 = aSStack_5c;
  AbyssEngine::String::String(this_01,pSVar21,false);
  Layout::drawHeader(iVar1,this_01);
  AbyssEngine::String::~String(aSStack_5c);
  Layout::drawEmptyFooter(SUB41(*piVar17,0));
  Status::getShip();
  iVar1 = Ship::hasJumpDriveIntegrated();
  if ((iVar1 != 0) || (in_r0[0xab] != (StarMap)0x0)) {
    iVar1 = *(int *)(in_r0 + 0x1d0);
    if (0 < iVar1) {
      this_01 = *(String **)(in_r0 + 0x60);
    }
    pSVar13 = (String *)(iVar1 + -1);
    if (iVar1 >= 1) {
      pSVar13 = this_01;
    }
    if ((int)pSVar13 < 0 == (iVar1 < 1 && SBORROW4(iVar1,1))) {
      Status::getSystem();
      pSVar13 = (String *)SolarSystem::getIndex();
      if (this_01 != pSVar13) {
        pSVar21 = (String *)GameText::getText(*piVar23);
        AbyssEngine::String::String(aSStack_74,(char *)(DAT_000d9b30 + 0xd981c),false);
        AbyssEngine::operator+(aSStack_68,pSVar21);
        AbyssEngine::String::String(aSStack_a4,*(int *)(in_r0 + 0x1d0));
        AbyssEngine::String::String(aSStack_b0,(char *)(DAT_000d9b34 + 0xd983a),false);
        AbyssEngine::operator+(aSStack_98,aSStack_a4);
        AbyssEngine::String::String(aSStack_bc,*(int *)(in_r0 + 0x1d8));
        AbyssEngine::operator+(aSStack_8c,aSStack_98);
        AbyssEngine::String::String(aSStack_80,aSStack_8c,false);
        AbyssEngine::operator+((String *)&local_50,aSStack_68);
        pcVar19 = *(code **)(DAT_000d9b38 + 0xd9878);
        (*pcVar19)(aSStack_80);
        (*pcVar19)(aSStack_8c);
        (*pcVar19)(aSStack_bc);
        (*pcVar19)(aSStack_98);
        (*pcVar19)(aSStack_b0);
        (*pcVar19)(aSStack_a4);
        (*pcVar19)(aSStack_68);
        (*pcVar19)(aSStack_74);
        if (*(int *)(in_r0 + 0x1d8) < *(int *)(in_r0 + 0x1d0)) {
          AbyssEngine::PaintCanvas::SetColor(**(uint **)(DAT_000d9b3c + 0xd98ac));
        }
        puVar6 = *(uint **)(DAT_000d9b44 + 0xd98bc);
        pSVar21 = (String *)**(undefined4 **)(DAT_000d9b40 + 0xd98ba);
        uVar22 = *puVar6;
        iVar12 = *(int *)(*piVar17 + 0x2dc);
        iVar24 = *(int *)(*piVar17 + 0x2e4);
        iVar1 = AbyssEngine::PaintCanvas::GetTextWidth(uVar22,pSVar21);
        iVar18 = *piVar17;
        AbyssEngine::PaintCanvas::DrawString
                  (uVar22,pSVar21,(int)&local_50,(iVar12 + iVar24 / 2) - iVar1 / 2,
                   (bool)(((char)*(undefined4 *)(iVar18 + 0x2e0) +
                          (char)*(undefined4 *)(iVar18 + 0x2e8)) -
                         (char)*(undefined4 *)(iVar18 + 0x14)));
        AbyssEngine::PaintCanvas::SetColor(*puVar6);
        AbyssEngine::String::~String((String *)&local_50);
      }
    }
  }
  TouchButton::draw();
  if (in_r0[0xa9] != (StarMap)0x0) {
    ChoiceWindow::draw();
  }
  if (*piVar16 != local_44) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000d90f4: push {r4,r5,r6,r7,lr}
  000d90f6: add r7,sp,#0xc
  000d90f8: push {r7,r8,r9,r10,r11}
  000d90fc: vpush {d8,d9,d10}
  000d9100: sub sp,#0x98
  000d9102: mov r4,r0
  000d9104: ldr r0,[0x000d93e0]
  000d9106: movs r1,#0x0
  000d9108: add r0,pc
  000d910a: ldr r3,[r0,#0x0]
  000d910c: ldr r0,[r3,#0x0]
  000d910e: str r0,[sp,#0x94]
  000d9110: ldr r0,[r4,#0x4]
  000d9112: ldrb.w r2,[r4,#0x138]
  000d9116: cmp r0,#0x0
  000d9118: it eq
  000d911a: mov.eq r1,#0xff
  000d911c: str.w r1,[r4,#0x1a4]
  000d9120: cmp r2,#0x0
  000d9122: str r3,[sp,#0xc]
  000d9124: itt eq
  000d9126: ldrb.eq.w r2,[r4,#0x139]
  000d912a: cmp.eq r2,#0x0
  000d912c: beq.w 0x000d9924
  000d9130: ldr.w r0,[r4,#0x184]
  000d9134: blx 0x0007468c
  000d9138: ldr.w r1,[r4,#0x184]
  000d913c: vmov s16,r0
  000d9140: mov r0,r1
  000d9142: blx 0x00074698
  000d9146: ldr.w r1,[r4,#0x184]
  000d914a: vmov s18,r0
  000d914e: mov r0,r1
  000d9150: blx 0x000746a4
  000d9154: ldr.w r1,[r4,#0x184]
  000d9158: vmov s20,r0
  000d915c: mov r0,r1
  000d915e: blx 0x00074698
  000d9162: vmov s0,r0
  000d9166: ldrb.w r2,[r4,#0x138]
  000d916a: vsub.f32 s2,s16,s18
  000d916e: vsub.f32 s0,s20,s0
  000d9172: cmp r2,#0x0
  000d9174: vdiv.f32 s0,s2,s0
  000d9178: vmov.f32 s2,0x3f800000
  000d917c: vsub.f32 s2,s2,s0
  000d9180: it eq
  000d9182: vmov.eq.f32 s2,s0
  000d9186: vldr.32 s0,[pc,#0x25c]
  000d918a: vmul.f32 s0,s2,s0
  000d918e: vcvt.s32.f32 s2,s0
  000d9192: ldr r0,[r4,#0x4]
  000d9194: vcvt.s32.f32 s0,s0
  000d9198: vstr.32 s2,[r4,#0x1a4]
  000d919c: vmov r1,s0
  000d91a0: cmp r0,#0x3
  000d91a2: it eq
  000d91a4: cmp.eq r2,#0x0
  000d91a6: beq.w 0x000d9552
  000d91aa: ldr r0,[0x000d93e8]
  000d91ac: cmp r1,#0x22
  000d91ae: it ge
  000d91b0: mov.ge r1,#0x22
  000d91b2: movs r2,#0xff
  000d91b4: add r0,pc
  000d91b6: uxtb r1,r1
  000d91b8: str r1,[sp,#0x0]
  000d91ba: movs r1,#0xff
  000d91bc: ldr r0,[r0,#0x0]
  000d91be: movs r3,#0xff
  000d91c0: str r0,[sp,#0x14]
  000d91c2: ldr r0,[r0,#0x0]
  000d91c4: blx 0x0006ff28
  000d91c8: add.w r0,r4,#0x84
  000d91cc: str r0,[sp,#0x18]
  000d91ce: add.w r0,r4,#0x78
  000d91d2: str r0,[sp,#0x10]
  000d91d4: ldr r0,[0x000d93ec]
  000d91d6: add.w r8,sp,#0x88
  000d91da: mov.w r11,#0x0
  000d91de: add r0,pc
  000d91e0: ldr.w r9,[r0,#0x0]
  000d91e4: b 0x000d9308
  000d91e6: ldr.w r0,[r9,#0x0]
  000d91ea: blx 0x00071aa0
  000d91ee: mov r6,r0
  000d91f0: cmp r0,#0x0
  000d91f2: beq.w 0x000d9304
  000d91f6: ldr r0,[r6,#0x0]
  000d91f8: cmp r0,r11
  000d91fa: bls.w 0x000d9304
  000d91fe: ldr r0,[r6,#0x4]
  000d9200: ldrb.w r0,[r0,r11]
  000d9204: cmp r0,#0x0
  000d9206: beq 0x000d9304
  000d9208: ldrb.w r0,[r4,#0x118]
  000d920c: cbz r0,0x000d9220
  000d920e: ldr.w r0,[r4,#0x114]
  000d9212: cmp r0,r11
  000d9214: bne 0x000d9220
  000d9216: ldr.w r0,[r4,#0x11c]
  000d921a: cmp.w r0,#0xfa0
  000d921e: blt 0x000d9304
  000d9220: ldr r0,[r4,#0x54]
  000d9222: ldr r0,[r0,#0x4]
  000d9224: ldr.w r0,[r0,r11,lsl #0x2]
  000d9228: blx 0x00071aac
  000d922c: mov r5,r0
  000d922e: cmp r0,#0x0
  000d9230: beq 0x000d9304
  000d9232: ldr.w r0,[r4,#0x194]
  000d9236: mov r1,r8
  000d9238: ldr r0,[r0,#0x4]
  000d923a: ldr.w r0,[r0,r11,lsl #0x2]
  000d923e: vldr.64 d16,[r0]
  000d9242: ldr r0,[r0,#0x8]
  000d9244: str r0,[sp,#0x90]
  000d9246: ldr r0,[sp,#0x10]
  000d9248: vstr.64 d16,[sp,#0x88]
  000d924c: blx 0x0006eb3c
  000d9250: mov.w r10,#0x0
  000d9254: b 0x000d92fe
  000d9256: ldr r0,[r5,#0x4]
  000d9258: ldr r1,[r6,#0x0]
  000d925a: ldr.w r0,[r0,r10,lsl #0x2]
  000d925e: cmp r1,r0
  000d9260: bls 0x000d92fa
  000d9262: ldr r1,[r6,#0x4]
  000d9264: ldrb r1,[r1,r0]
  000d9266: cmp r1,#0x0
  000d9268: beq 0x000d92fa
  000d926a: ldr.w r1,[r4,#0x194]
  000d926e: ldr r2,[r1,#0x0]
  000d9270: cmp r0,r2
  000d9272: bcs 0x000d92fa
  000d9274: ldrb.w r2,[r4,#0x118]
  000d9278: cbz r2,0x000d928c
  000d927a: ldr.w r2,[r4,#0x114]
  000d927e: cmp r2,r0
  000d9280: bne 0x000d928c
  000d9282: ldr.w r2,[r4,#0x11c]
  000d9286: cmp.w r2,#0xfa0
  000d928a: blt 0x000d92fa
  000d928c: ldr r1,[r1,#0x4]
  000d928e: ldr.w r0,[r1,r0,lsl #0x2]
  000d9292: mov r1,r8
  000d9294: vldr.64 d16,[r0]
  000d9298: ldr r0,[r0,#0x8]
  000d929a: str r0,[sp,#0x90]
  000d929c: ldr r0,[sp,#0x18]
  000d929e: vstr.64 d16,[sp,#0x88]
  000d92a2: blx 0x0006eb3c
  000d92a6: vldr.32 s0,[r4,#0x80]
  000d92aa: vcmpe.f32 s0,#0
  000d92ae: vmrs apsr,fpscr
  000d92b2: bpl 0x000d92c2
  000d92b4: vldr.32 s0,[r4,#0x8c]
  000d92b8: vcmpe.f32 s0,#0
  000d92bc: vmrs apsr,fpscr
  000d92c0: bmi 0x000d92fa
  000d92c2: ldr r0,[sp,#0x14]
  000d92c4: vldr.32 s0,[r4,#0x78]
  000d92c8: vldr.32 s2,[r4,#0x7c]
  000d92cc: vldr.32 s4,[r4,#0x84]
  000d92d0: vldr.32 s6,[r4,#0x88]
  000d92d4: ldr r0,[r0,#0x0]
  000d92d6: vcvt.s32.f32 s6,s6
  000d92da: vcvt.s32.f32 s0,s0
  000d92de: vcvt.s32.f32 s2,s2
  000d92e2: vcvt.s32.f32 s4,s4
  000d92e6: vstr.32 s6,[sp]
  000d92ea: vmov r1,s0
  000d92ee: vmov r2,s2
  000d92f2: vmov r3,s4
  000d92f6: blx 0x000746b0
  000d92fa: add.w r10,r10,#0x1
  000d92fe: ldr r0,[r5,#0x0]
  000d9300: cmp r10,r0
  000d9302: bcc 0x000d9256
  000d9304: add.w r11,r11,#0x1
  000d9308: ldr r0,[r4,#0x68]
  000d930a: ldr r0,[r0,#0x0]
  000d930c: cmp r11,r0
  000d930e: bcc.w 0x000d91e6
  000d9312: ldr r0,[sp,#0x14]
  000d9314: movs r2,#0xff
  000d9316: ldrb.w r1,[r4,#0x1a4]
  000d931a: movs r3,#0xff
  000d931c: str r1,[sp,#0x0]
  000d931e: movs r1,#0xff
  000d9320: ldr r0,[r0,#0x0]
  000d9322: blx 0x0006ff28
  000d9326: ldr.w r0,[r9,#0x0]
  000d932a: ldr.w r5,[r4,#0x194]
  000d932e: blx 0x00071a10
  000d9332: blx 0x00071a7c
  000d9336: ldr r1,[r5,#0x4]
  000d9338: add.w r8,sp,#0x88
  000d933c: ldr.w r0,[r1,r0,lsl #0x2]
  000d9340: mov r1,r8
  000d9342: vldr.64 d16,[r0]
  000d9346: ldr r0,[r0,#0x8]
  000d9348: str r0,[sp,#0x90]
  000d934a: ldr r0,[sp,#0x18]
  000d934c: vstr.64 d16,[sp,#0x88]
  000d9350: blx 0x0006eb3c
  000d9354: ldr.w r0,[r9,#0x0]
  000d9358: blx 0x00071a10
  000d935c: blx 0x00071aac
  000d9360: ldr.w r11,[sp,#0x10]
  000d9364: mov r5,r0
  000d9366: vldr.32 s18,[pc,#0x88]
  000d936a: mov.w r10,#0x0
  000d936e: vldr.32 s16,[pc,#0x74]
  000d9372: movs r6,#0x0
  000d9374: b 0x000d94d6
  000d9376: ldr.w r0,[r9,#0x0]
  000d937a: blx 0x00071aa0
  000d937e: cmp r0,#0x0
  000d9380: beq.w 0x000d94d4
  000d9384: ldr.w r1,[r4,#0x194]
  000d9388: ldr r2,[r1,#0x0]
  000d938a: cmp r6,r2
  000d938c: bcs.w 0x000d94d4
  000d9390: ldr r2,[r0,#0x0]
  000d9392: cmp r2,r6
  000d9394: bls.w 0x000d94d4
  000d9398: ldr r0,[r0,#0x4]
  000d939a: ldrb r0,[r0,r6]
  000d939c: cmp r0,#0x0
  000d939e: beq.w 0x000d94d4
  000d93a2: ldr r0,[r1,#0x4]
  000d93a4: mov r1,r8
  000d93a6: ldr.w r0,[r0,r6,lsl #0x2]
  000d93aa: vldr.64 d16,[r0]
  000d93ae: ldr r0,[r0,#0x8]
  000d93b0: str r0,[sp,#0x90]
  000d93b2: mov r0,r11
  000d93b4: vstr.64 d16,[sp,#0x88]
  000d93b8: blx 0x0006eb3c
  000d93bc: vldr.32 s0,[r4,#0x80]
  000d93c0: vcmpe.f32 s0,#0
  000d93c4: vmrs apsr,fpscr
  000d93c8: bpl 0x000d93f4
  000d93ca: vldr.32 s0,[r4,#0x8c]
  000d93ce: movs r0,#0x0
  000d93d0: vcmpe.f32 s0,#0
  000d93d4: vmrs apsr,fpscr
  000d93d8: it mi
  000d93da: mov.mi r0,#0x1
  000d93dc: b 0x000d93f6
  000d93f4: movs r0,#0x0
  000d93f6: cmp r5,#0x0
  000d93f8: beq 0x000d94be
  000d93fa: cmp r0,#0x0
  000d93fc: bne 0x000d94be
  000d93fe: ldrb.w r0,[r4,#0xab]
  000d9402: cmp r0,#0x0
  000d9404: bne 0x000d94be
  000d9406: ldr r0,[r5,#0x0]
  000d9408: movs r1,#0x0
  000d940a: cmp r1,r0
  000d940c: bcs 0x000d94be
  000d940e: ldr r2,[r5,#0x4]
  000d9410: ldr.w r2,[r2,r1,lsl #0x2]
  000d9414: adds r1,#0x1
  000d9416: cmp r2,r6
  000d9418: bne 0x000d940a
  000d941a: ldr.w r0,[r9,#0x0]
  000d941e: blx 0x00071710
  000d9422: movs r2,#0xa
  000d9424: movs r3,#0x0
  000d9426: bl 0x001aae54
  000d942a: movs r2,#0x63
  000d942c: movs r3,#0x0
  000d942e: bl 0x001aae54
  000d9432: rsbs.w r0,r2,#0x65
  000d9436: sbc.w r1,r10,r3
  000d943a: blx 0x0006f7d8
  000d943e: vmov s0,r0
  000d9442: ldr.w r1,[r4,#0x1a4]
  000d9446: movs r3,#0xff
  000d9448: vdiv.f32 s20,s0,s18
  000d944c: vmul.f32 s0,s20,s16
  000d9450: vcvt.s32.f32 s0,s0
  000d9454: ldr.w r10,[sp,#0x14]
  000d9458: ldr.w r0,[r10,#0x0]
  000d945c: vmov r2,s0
  000d9460: cmp r1,r2
  000d9462: it lt
  000d9464: mov.lt r2,r1
  000d9466: uxtb r1,r2
  000d9468: movs r2,#0xff
  000d946a: str r1,[sp,#0x0]
  000d946c: movs r1,#0xff
  000d946e: blx 0x0006ff28
  000d9472: vldr.32 s0,[r4,#0x78]
  000d9476: vldr.32 s4,[r4,#0x84]
  000d947a: vldr.32 s2,[r4,#0x7c]
  000d947e: vldr.32 s6,[r4,#0x88]
  000d9482: vsub.f32 s10,s4,s0
  000d9486: ldr.w r0,[r10,#0x0]
  000d948a: mov.w r10,#0x0
  000d948e: vsub.f32 s8,s6,s2
  000d9492: vmla.f32 s0,s20,s10
  000d9496: vmla.f32 s2,s20,s8
  000d949a: vcvt.s32.f32 s2,s2
  000d949e: vcvt.s32.f32 s4,s4
  000d94a2: vcvt.s32.f32 s6,s6
  000d94a6: vcvt.s32.f32 s0,s0
  000d94aa: vstr.32 s2,[sp]
  000d94ae: vmov r1,s4
  000d94b2: vmov r2,s6
  000d94b6: vmov r3,s0
  000d94ba: blx 0x000746b0
  000d94be: ldrb.w r0,[r4,#0x138]
  000d94c2: cbnz r0,0x000d94d4
  000d94c4: ldrb.w r0,[r4,#0x139]
  000d94c8: cbnz r0,0x000d94d4
  000d94ca: mov r0,r4
  000d94cc: mov r1,r6
  000d94ce: movs r2,#0x0
  000d94d0: blx 0x000746bc
  000d94d4: adds r6,#0x1
  000d94d6: ldr r0,[r4,#0x54]
  000d94d8: ldr r0,[r0,#0x0]
  000d94da: cmp r6,r0
  000d94dc: bcc.w 0x000d9376
  000d94e0: ldr r0,[r4,#0x4]
  000d94e2: cbnz r0,0x000d955c
  000d94e4: ldrb.w r0,[r4,#0x138]
  000d94e8: cbnz r0,0x000d955c
  000d94ea: ldrb.w r0,[r4,#0x139]
  000d94ee: cbnz r0,0x000d955c
  000d94f0: ldr.w r0,[r4,#0xa0]
  000d94f4: cbz r0,0x000d955c
  000d94f6: ldr.w r0,[r9,#0x0]
  000d94fa: blx 0x00071710
  000d94fe: movs r2,#0xa
  000d9500: movs r3,#0x0
  000d9502: movs r5,#0x0
  000d9504: bl 0x001aae54
  000d9508: movs r2,#0x63
  000d950a: movs r3,#0x0
  000d950c: bl 0x001aae54
  000d9510: rsbs.w r0,r2,#0x65
  000d9514: sbc.w r1,r5,r3
  000d9518: blx 0x0006f7d8
  000d951c: vmov s0,r0
  000d9520: ldr.w r0,[r4,#0xe8]
  000d9524: vdiv.f32 s18,s0,s18
  000d9528: vldr.32 s0,[r4,#0xec]
  000d952c: vcmpe.f32 s18,s0
  000d9530: vmrs apsr,fpscr
  000d9534: ble.w 0x000d9928
  000d9538: ldr.w r1,[r4,#0xa0]
  000d953c: adds r0,#0x1
  000d953e: str.w r0,[r4,#0xe8]
  000d9542: ldr r2,[r1,#0x0]
  000d9544: cmp r0,r2
  000d9546: it cc
  000d9548: mov.cc r5,r0
  000d954a: mov r0,r5
  000d954c: str.w r5,[r4,#0xe8]
  000d9550: b 0x000d992c
  000d9552: ldrb.w r0,[r4,#0x139]
  000d9556: cmp r0,#0x0
  000d9558: bne.w 0x000d91aa
  000d955c: ldr r1,[r4,#0x60]
  000d955e: cmp r1,#0x0
  000d9560: blt 0x000d957a
  000d9562: ldrb.w r0,[r4,#0x118]
  000d9566: cbz r0,0x000d9572
  000d9568: ldr.w r0,[r4,#0x11c]
  000d956c: cmp.w r0,#0xfa0
  000d9570: blt 0x000d957a
  000d9572: mov r0,r4
  000d9574: movs r2,#0x0
  000d9576: blx 0x000746bc
  000d957a: ldr.w r0,[r4,#0xa4]
  000d957e: cmp r0,#0x0
  000d9580: beq.w 0x000d9752
  000d9584: ldr r0,[r4,#0x58]
  000d9586: cmp r0,#0x0
  000d9588: beq.w 0x000d9752
  000d958c: ldr r0,[r4,#0x54]
  000d958e: ldr r1,[r4,#0x60]
  000d9590: ldr r0,[r0,#0x4]
  000d9592: ldr.w r0,[r0,r1,lsl #0x2]
  000d9596: blx 0x00074680
  000d959a: cmp r0,#0x0
  000d959c: bne.w 0x000d9726
  000d95a0: ldr r0,[0x000d9968]
  000d95a2: movs r2,#0xff
  000d95a4: ldrb.w r1,[r4,#0x1a4]
  000d95a8: movs r3,#0xff
  000d95aa: add r0,pc
  000d95ac: eor r1,r1,#0xff
  000d95b0: str r1,[sp,#0x0]
  000d95b2: ldr r6,[r0,#0x0]
  000d95b4: movs r1,#0xff
  000d95b6: ldr r0,[r6,#0x0]
  000d95b8: blx 0x0006ff28
  000d95bc: ldr r0,[0x000d996c]
  000d95be: ldr r1,[r4,#0x34]
  000d95c0: add r0,pc
  000d95c2: ldr.w r11,[r0,#0x0]
  000d95c6: ldr r0,[r6,#0x0]
  000d95c8: ldr.w r2,[r11,#0x0]
  000d95cc: ldr r3,[r2,#0xc]
  000d95ce: ldr r2,[r2,#0x2c]
  000d95d0: add r3,r2
  000d95d2: blx 0x00072dc0
  000d95d6: ldr.w r2,[r11,#0x0]
  000d95da: ldr r1,[r4,#0x34]
  000d95dc: ldr r0,[r6,#0x0]
  000d95de: ldr r2,[r2,#0x2c]
  000d95e0: str r2,[sp,#0x18]
  000d95e2: blx 0x00072d84
  000d95e6: ldrb.w r1,[r4,#0x1a4]
  000d95ea: mov r8,r0
  000d95ec: ldr r0,[r6,#0x0]
  000d95ee: movs r2,#0x80
  000d95f0: eor r1,r1,#0xff
  000d95f4: str r1,[sp,#0x0]
  000d95f6: movs r1,#0xff
  000d95f8: movs r3,#0x0
  000d95fa: blx 0x0006ff28
  000d95fe: ldr r0,[0x000d9970]
  000d9600: add.w r9,sp,#0x88
  000d9604: ldr r1,[r4,#0x60]
  000d9606: add r0,pc
  000d9608: ldr r5,[r6,#0x0]
  000d960a: ldr r0,[r0,#0x0]
  000d960c: str r0,[sp,#0x10]
  000d960e: ldr.w r10,[r0,#0x0]
  000d9612: ldr r0,[r4,#0x54]
  000d9614: ldr r0,[r0,#0x4]
  000d9616: ldr.w r1,[r0,r1,lsl #0x2]
  000d961a: mov r0,r9
  000d961c: blx 0x00071cc8
  000d9620: ldr r1,[sp,#0x18]
  000d9622: ldr.w r0,[r11,#0x0]
  000d9626: str.w r11,[sp,#0x14]
  000d962a: add.w r3,r8,r1, lsl #0x1
  000d962e: ldr r1,[r0,#0xc]
  000d9630: ldr r0,[r0,#0x2c]
  000d9632: add r0,r1
  000d9634: mov r1,r10
  000d9636: adds r0,#0x2
  000d9638: str r0,[sp,#0x0]
  000d963a: movs r0,#0x0
  000d963c: mov r2,r9
  000d963e: str r0,[sp,#0x4]
  000d9640: mov r0,r5
  000d9642: str r3,[sp,#0x18]
  000d9644: blx 0x0006fe14
  000d9648: add r0,sp,#0x88
  000d964a: blx 0x0006ee30
  000d964e: ldrb.w r1,[r4,#0x1a4]
  000d9652: movs r2,#0xff
  000d9654: ldr r0,[r6,#0x0]
  000d9656: movs r3,#0xff
  000d9658: eor r1,r1,#0xff
  000d965c: str r1,[sp,#0x0]
  000d965e: movs r1,#0xff
  000d9660: blx 0x0006ff28
  000d9664: ldr r0,[0x000d9974]
  000d9666: ldr r1,[r4,#0x54]
  000d9668: add r0,pc
  000d966a: ldr r2,[r4,#0x60]
  000d966c: ldr.w r8,[sp,#0x10]
  000d9670: ldr r3,[r0,#0x0]
  000d9672: ldr r0,[r1,#0x4]
  000d9674: str r3,[sp,#0x8]
  000d9676: ldr.w r9,[r8,#0x0]
  000d967a: ldr.w r0,[r0,r2,lsl #0x2]
  000d967e: ldr.w r10,[r6,#0x0]
  000d9682: ldr r5,[r3,#0x0]
  000d9684: blx 0x00071950
  000d9688: add.w r1,r0,#0x196
  000d968c: mov r0,r5
  000d968e: blx 0x00072f70
  000d9692: ldr.w r11,[sp,#0x14]
  000d9696: mov r2,r0
  000d9698: ldr.w r0,[r11,#0x0]
  000d969c: ldr r1,[r0,#0x4]
  000d969e: ldr r3,[r0,#0xc]
  000d96a0: ldr r0,[r0,#0x2c]
  000d96a2: add r0,r3
  000d96a4: add r0,r1
  000d96a6: mov r1,r9
  000d96a8: adds r0,#0x2
  000d96aa: str r0,[sp,#0x0]
  000d96ac: movs r0,#0x0
  000d96ae: str r0,[sp,#0x4]
  000d96b0: mov r0,r10
  000d96b2: ldr.w r10,[sp,#0x18]
  000d96b6: mov r3,r10
  000d96b8: blx 0x0006fe14
  000d96bc: ldr r0,[r4,#0x54]
  000d96be: ldr r1,[r4,#0x60]
  000d96c0: ldr r0,[r0,#0x4]
  000d96c2: ldr.w r0,[r0,r1,lsl #0x2]
  000d96c6: blx 0x00073ef4
  000d96ca: mov r5,r0
  000d96cc: ldrb.w r1,[r4,#0x1a4]
  000d96d0: ldr r0,[r6,#0x0]
  000d96d2: add.w r3,r5,r5, lsl #0x1
  000d96d6: ldr r2,[0x000d9978]
  000d96d8: eor r1,r1,#0xff
  000d96dc: str r1,[sp,#0x0]
  000d96de: add r2,pc
  000d96e0: ldrb.w r1,[r2,r3,lsl #0x2]
  000d96e4: add.w r3,r2,r3, lsl #0x2
  000d96e8: ldrb r2,[r3,#0x4]
  000d96ea: ldrb r3,[r3,#0x8]
  000d96ec: blx 0x0006ff28
  000d96f0: ldr r0,[sp,#0x8]
  000d96f2: add.w r1,r5,#0x192
  000d96f6: ldr.w r9,[r8,#0x0]
  000d96fa: ldr r6,[r6,#0x0]
  000d96fc: ldr r0,[r0,#0x0]
  000d96fe: blx 0x00072f70
  000d9702: mov r2,r0
  000d9704: ldr.w r0,[r11,#0x0]
  000d9708: ldr r1,[r0,#0x4]
  000d970a: ldr r3,[r0,#0xc]
  000d970c: ldr r0,[r0,#0x2c]
  000d970e: add r0,r3
  000d9710: mov r3,r10
  000d9712: add.w r0,r0,r1, lsl #0x1
  000d9716: mov r1,r9
  000d9718: adds r0,#0x2
  000d971a: str r0,[sp,#0x0]
  000d971c: movs r0,#0x0
  000d971e: str r0,[sp,#0x4]
  000d9720: mov r0,r6
  000d9722: blx 0x0006fe14
  000d9726: movs r5,#0x0
  000d9728: b 0x000d973a
  000d972a: cmp r5,r1
  000d972c: beq 0x000d9738
  000d972e: mov r0,r4
  000d9730: mov r1,r5
  000d9732: movs r2,#0x1
  000d9734: blx 0x000746bc
  000d9738: adds r5,#0x1
  000d973a: ldr r0,[r4,#0x58]
  000d973c: ldr r1,[r4,#0x64]
  000d973e: ldr r0,[r0,#0x0]
  000d9740: cmp r5,r0
  000d9742: bcc 0x000d972a
  000d9744: cmp.w r1,#0xffffffff
  000d9748: ittt gt
  000d974a: mov.gt r0,r4
  000d974c: mov.gt r2,#0x1
  000d974e: blx.gt 0x000746bc
  000d9752: ldrb.w r0,[r4,#0x108]
  000d9756: cbz r0,0x000d975e
  000d9758: mov r0,r4
  000d975a: blx 0x000746c8
  000d975e: ldr r0,[0x000d9b1c]
  000d9760: ldr r1,[0x000d9b20]
  000d9762: add r0,pc
  000d9764: add r1,pc
  000d9766: ldr.w r8,[r0,#0x0]
  000d976a: ldr.w r9,[r1,#0x0]
  000d976e: movs r1,#0xb1
  000d9770: ldr.w r5,[r8,#0x0]
  000d9774: ldr.w r0,[r9,#0x0]
  000d9778: blx 0x00072f70
  000d977c: add r6,sp,#0x7c
  000d977e: mov r1,r0
  000d9780: movs r2,#0x0
  000d9782: mov r0,r6
  000d9784: blx 0x0006f028
  000d9788: mov r0,r5
  000d978a: mov r1,r6
  000d978c: blx 0x000746d4
  000d9790: add r0,sp,#0x7c
  000d9792: blx 0x0006ee30
  000d9796: ldr.w r0,[r8,#0x0]
  000d979a: movs r1,#0x1
  000d979c: blx 0x00071d58
  000d97a0: ldr r0,[0x000d9b2c]
  000d97a2: add r0,pc
  000d97a4: ldr r5,[r0,#0x0]
  000d97a6: ldr r0,[r5,#0x0]
  000d97a8: blx 0x00071a58
  000d97ac: blx 0x000719bc
  000d97b0: cbnz r0,0x000d97b8
  000d97b2: ldrb.w r0,[r4,#0xab]
  000d97b6: cbz r0,0x000d97c6
  000d97b8: ldr.w r0,[r4,#0x1d0]
  000d97bc: cmp r0,#0x1
  000d97be: itt ge
  000d97c0: ldr.ge r6,[r4,#0x60]
  000d97c2: cmp.ge r6,#0x0
  000d97c4: bge 0x000d97f6
  000d97c6: ldr r0,[r4,#0x4c]
  000d97c8: blx 0x000746e0
  000d97cc: ldrb.w r0,[r4,#0xa9]
  000d97d0: cbz r0,0x000d97d8
  000d97d2: ldr r0,[r4,#0x5c]
  000d97d4: blx 0x000746ec
  000d97d8: ldr r0,[sp,#0x94]
  000d97da: ldr r1,[sp,#0xc]
  000d97dc: ldr r1,[r1,#0x0]
  000d97de: subs r0,r1,r0
  000d97e0: itttt eq
  000d97e2: add.eq sp,#0x98
  000d97e4: vpop.eq {d8,d9,d10}
  000d97e8: add.eq sp,#0x4
  000d97ea: pop.eq.w {r8,r9,r10,r11}
  000d97ee: it eq
  000d97f0: pop.eq {r4,r5,r6,r7,pc}
  000d97f2: blx 0x0006e824
  000d97f6: ldr r0,[r5,#0x0]
  000d97f8: blx 0x00071a10
  000d97fc: blx 0x00071a7c
  000d9800: cmp r6,r0
  000d9802: beq 0x000d97c6
  000d9804: ldr.w r0,[r9,#0x0]
  000d9808: movw r1,#0x242
  000d980c: blx 0x00072f70
  000d9810: ldr r1,[0x000d9b30]
  000d9812: add r6,sp,#0x64
  000d9814: mov r5,r0
  000d9816: movs r2,#0x0
  000d9818: add r1,pc
  000d981a: mov r0,r6
  000d981c: blx 0x0006ee18
  000d9820: add r0,sp,#0x70
  000d9822: mov r1,r5
  000d9824: mov r2,r6
  000d9826: blx 0x0006ef98
  000d982a: ldr.w r1,[r4,#0x1d0]
  000d982e: add r0,sp,#0x34
  000d9830: blx 0x0006f658
  000d9834: ldr r1,[0x000d9b34]
  000d9836: add r1,pc
  000d9838: add r0,sp,#0x28
  000d983a: movs r2,#0x0
  000d983c: blx 0x0006ee18
  000d9840: add r0,sp,#0x40
  000d9842: add r1,sp,#0x34
  000d9844: add r2,sp,#0x28
  000d9846: blx 0x0006ef98
  000d984a: ldr.w r1,[r4,#0x1d8]
  000d984e: add r0,sp,#0x1c
  000d9850: blx 0x0006f658
  000d9854: add r0,sp,#0x4c
  000d9856: add r1,sp,#0x40
  000d9858: add r2,sp,#0x1c
  000d985a: blx 0x0006ef98
  000d985e: add r0,sp,#0x58
  000d9860: add r1,sp,#0x4c
  000d9862: movs r2,#0x0
  000d9864: blx 0x0006f028
  000d9868: add r0,sp,#0x88
  000d986a: add r1,sp,#0x70
  000d986c: add r2,sp,#0x58
  000d986e: blx 0x0006ef98
  000d9872: ldr r0,[0x000d9b38]
  000d9874: add r0,pc
  000d9876: ldr r5,[r0,#0x0]
  000d9878: add r0,sp,#0x58
  000d987a: blx r5
  000d987c: add r0,sp,#0x4c
  000d987e: blx r5
  000d9880: add r0,sp,#0x1c
  000d9882: blx r5
  000d9884: add r0,sp,#0x40
  000d9886: blx r5
  000d9888: add r0,sp,#0x28
  000d988a: blx r5
  000d988c: add r0,sp,#0x34
  000d988e: blx r5
  000d9890: add r0,sp,#0x70
  000d9892: blx r5
  000d9894: add r0,sp,#0x64
  000d9896: blx r5
  000d9898: ldr.w r0,[r4,#0x1d0]
  000d989c: ldr.w r1,[r4,#0x1d8]
  000d98a0: cmp r0,r1
  000d98a2: ble 0x000d98b2
  000d98a4: ldr r0,[0x000d9b3c]
  000d98a6: ldr r1,[0x000d9b28]
  000d98a8: add r0,pc
  000d98aa: ldr r0,[r0,#0x0]
  000d98ac: ldr r0,[r0,#0x0]
  000d98ae: blx 0x0006fac0
  000d98b2: ldr r0,[0x000d9b40]
  000d98b4: ldr r1,[0x000d9b44]
  000d98b6: add r0,pc
  000d98b8: add r1,pc
  000d98ba: ldr r0,[r0,#0x0]
  000d98bc: ldr.w r9,[r1,#0x0]
  000d98c0: ldr.w r1,[r8,#0x0]
  000d98c4: ldr r5,[r0,#0x0]
  000d98c6: ldr.w r6,[r9,#0x0]
  000d98ca: ldr.w r10,[r1,#0x2dc]
  000d98ce: ldr.w r11,[r1,#0x2e4]
  000d98d2: add r2,sp,#0x88
  000d98d4: mov r0,r6
  000d98d6: mov r1,r5
  000d98d8: blx 0x0006faa8
  000d98dc: ldr.w r1,[r8,#0x0]
  000d98e0: ldr r2,[r1,#0x14]
  000d98e2: ldr.w r3,[r1,#0x2e8]
  000d98e6: ldr.w r1,[r1,#0x2e0]
  000d98ea: add r1,r3
  000d98ec: mov.w r12,#0x0
  000d98f0: subs r1,r1,r2
  000d98f2: strd r1,r12,[sp,#0x0]
  000d98f6: add.w r1,r11,r11, lsr #0x1f
  000d98fa: add.w r0,r0,r0, lsr #0x1f
  000d98fe: add r2,sp,#0x88
  000d9900: add.w r1,r10,r1, asr #0x1
  000d9904: sub.w r3,r1,r0, asr #0x1
  000d9908: mov r0,r6
  000d990a: mov r1,r5
  000d990c: blx 0x0006fe14
  000d9910: ldr.w r0,[r9,#0x0]
  000d9914: mov.w r1,#0xffffffff
  000d9918: blx 0x0006fac0
  000d991c: add r0,sp,#0x88
  000d991e: blx 0x0006ee30
  000d9922: b 0x000d97c6
  000d9924: movs r2,#0x0
  000d9926: b 0x000d91a0
  000d9928: ldr.w r1,[r4,#0xa0]
  000d992c: vstr.32 s18,[r4,#0xec]
  000d9930: ldr r1,[r1,#0x0]
  000d9932: subs r1,#0x1
  000d9934: cmp r0,r1
  000d9936: bne 0x000d9958
  000d9938: vmul.f32 s0,s18,s16
  000d993c: movs r2,#0xff
  000d993e: movs r3,#0x0
  000d9940: vcvt.s32.f32 s0,s0
  000d9944: ldr r0,[sp,#0x14]
  000d9946: ldr r0,[r0,#0x0]
  000d9948: vmov r1,s0
  000d994c: uxtb r1,r1
  000d994e: str r1,[sp,#0x0]
  000d9950: movs r1,#0xff
  000d9952: blx 0x0006ff28
  000d9956: b 0x000d9962
  000d9958: ldr r0,[sp,#0x14]
  000d995a: ldr r1,[0x000d9b24]
  000d995c: ldr r0,[r0,#0x0]
  000d995e: blx 0x0006fac0
  000d9962: add r5,sp,#0x88
  000d9964: movs r6,#0x0
  000d9966: b 0x000d9a08
  000d997c: ldr.w r0,[r4,#0xa0]
  000d9980: ldr.w r1,[r4,#0x194]
  000d9984: ldr r0,[r0,#0x4]
  000d9986: ldr r1,[r1,#0x4]
  000d9988: ldr.w r0,[r0,r6,lsl #0x2]
  000d998c: ldr.w r0,[r1,r0,lsl #0x2]
  000d9990: mov r1,r5
  000d9992: vldr.64 d16,[r0]
  000d9996: ldr r0,[r0,#0x8]
  000d9998: str r0,[sp,#0x90]
  000d999a: ldr r0,[sp,#0x18]
  000d999c: vstr.64 d16,[sp,#0x88]
  000d99a0: blx 0x0006eb3c
  000d99a4: ldr.w r0,[r4,#0xa0]
  000d99a8: ldr.w r1,[r4,#0x194]
  000d99ac: ldr r0,[r0,#0x4]
  000d99ae: ldr r1,[r1,#0x4]
  000d99b0: add.w r0,r0,r6, lsl #0x2
  000d99b4: ldr r0,[r0,#0x4]
  000d99b6: ldr.w r0,[r1,r0,lsl #0x2]
  000d99ba: mov r1,r5
  000d99bc: vldr.64 d16,[r0]
  000d99c0: ldr r0,[r0,#0x8]
  000d99c2: str r0,[sp,#0x90]
  000d99c4: mov r0,r11
  000d99c6: vstr.64 d16,[sp,#0x88]
  000d99ca: blx 0x0006eb3c
  000d99ce: ldr r0,[sp,#0x14]
  000d99d0: vldr.32 s0,[r4,#0x78]
  000d99d4: vldr.32 s2,[r4,#0x7c]
  000d99d8: vldr.32 s4,[r4,#0x84]
  000d99dc: vldr.32 s6,[r4,#0x88]
  000d99e0: ldr r0,[r0,#0x0]
  000d99e2: vcvt.s32.f32 s2,s2
  000d99e6: vcvt.s32.f32 s4,s4
  000d99ea: vcvt.s32.f32 s6,s6
  000d99ee: vcvt.s32.f32 s0,s0
  000d99f2: vstr.32 s2,[sp]
  000d99f6: vmov r1,s4
  000d99fa: vmov r2,s6
  000d99fe: vmov r3,s0
  000d9a02: blx 0x000746b0
  000d9a06: adds r6,#0x1
  000d9a08: ldr.w r0,[r4,#0xe8]
  000d9a0c: cmp r6,r0
  000d9a0e: blt 0x000d997c
  000d9a10: ldr.w r1,[r4,#0xa0]
  000d9a14: ldr r2,[r1,#0x0]
  000d9a16: subs r2,#0x1
  000d9a18: cmp r0,r2
  000d9a1a: bcs.w 0x000d955c
  000d9a1e: ldr r1,[r1,#0x4]
  000d9a20: ldr.w r2,[r4,#0x194]
  000d9a24: ldr.w r0,[r1,r0,lsl #0x2]
  000d9a28: ldr r1,[r2,#0x4]
  000d9a2a: ldr.w r0,[r1,r0,lsl #0x2]
  000d9a2e: add r1,sp,#0x88
  000d9a30: vldr.64 d16,[r0]
  000d9a34: ldr r0,[r0,#0x8]
  000d9a36: str r0,[sp,#0x90]
  000d9a38: ldr r0,[sp,#0x18]
  000d9a3a: vstr.64 d16,[sp,#0x88]
  000d9a3e: blx 0x0006eb3c
  000d9a42: ldr.w r0,[r4,#0xa0]
  000d9a46: ldr.w r1,[r4,#0xe8]
  000d9a4a: ldr.w r2,[r4,#0x194]
  000d9a4e: ldr r0,[r0,#0x4]
  000d9a50: add.w r0,r0,r1, lsl #0x2
  000d9a54: ldr r2,[r2,#0x4]
  000d9a56: add r1,sp,#0x88
  000d9a58: ldr r0,[r0,#0x4]
  000d9a5a: ldr.w r0,[r2,r0,lsl #0x2]
  000d9a5e: vldr.64 d16,[r0]
  000d9a62: ldr r0,[r0,#0x8]
  000d9a64: str r0,[sp,#0x90]
  000d9a66: mov r0,r11
  000d9a68: vstr.64 d16,[sp,#0x88]
  000d9a6c: blx 0x0006eb3c
  000d9a70: vldr.32 s0,[r4,#0x78]
  000d9a74: vldr.32 s4,[r4,#0x84]
  000d9a78: vldr.32 s2,[r4,#0x7c]
  000d9a7c: vldr.32 s6,[r4,#0x88]
  000d9a80: vsub.f32 s10,s4,s0
  000d9a84: ldr r0,[sp,#0x14]
  000d9a86: ldr r0,[r0,#0x0]
  000d9a88: vsub.f32 s8,s6,s2
  000d9a8c: vmla.f32 s0,s18,s10
  000d9a90: vmla.f32 s2,s18,s8
  000d9a94: vcvt.s32.f32 s2,s2
  000d9a98: vcvt.s32.f32 s4,s4
  000d9a9c: vcvt.s32.f32 s6,s6
  000d9aa0: vcvt.s32.f32 s0,s0
  000d9aa4: vstr.32 s2,[sp]
  000d9aa8: vmov r1,s4
  000d9aac: vmov r2,s6
  000d9ab0: vmov r3,s0
  000d9ab4: blx 0x000746b0
  000d9ab8: b 0x000d955c
```
