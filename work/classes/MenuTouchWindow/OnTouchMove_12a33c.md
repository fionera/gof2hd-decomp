# MenuTouchWindow::OnTouchMove
elf 0x12a33c body 1442
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2, void * param_3)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* MenuTouchWindow::OnTouchMove(int, int, void*) */

undefined4 MenuTouchWindow::OnTouchMove(int param_1,int param_2,void *param_3)

{
  char *pcVar1;
  int *piVar2;
  uint *puVar3;
  Globals *pGVar4;
  int iVar5;
  int iVar6;
  uint *puVar7;
  int *piVar8;
  uint uVar9;
  int in_r3;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  undefined4 *puVar14;
  code *pcVar15;
  uint uVar16;
  int iVar17;
  int iVar18;
  int *piVar19;
  undefined4 unaff_lr;
  float fVar20;
  
  if (*(char *)(param_1 + 0x170) != '\0') {
    ChoiceWindow::OnTouchMove(*(int *)(param_1 + 0x104),param_2);
    return 0;
  }
  piVar8 = *(int **)(DAT_0013a664 + 0x13a368);
  pcVar1 = (char *)*piVar8;
  if (*pcVar1 != '\0') goto LAB_0013aa7e;
  iVar6 = *(int *)(param_1 + 0x16c) + -1;
  switch(iVar6) {
  case 0:
  case 1:
    if ((*(int *)(pcVar1 + 0xc) < (int)param_3) &&
       ((int)param_3 < **(int **)(DAT_0013a668 + 0x13a3b4) - *(int *)(pcVar1 + 0x10))) {
      iVar6 = (int)param_3 - *(int *)(param_1 + 0x20c);
      *(void **)(param_1 + 0x20c) = param_3;
      *(int *)(param_1 + 0x214) = iVar6;
      *(undefined4 *)(param_1 + 0x218) = 0x3f800000;
      *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + iVar6;
    }
    iVar6 = *(int *)(param_1 + 0xc4);
LAB_0013a3de:
    TouchButton::OnTouchMove(iVar6,param_2);
    break;
  case 2:
    if (**(char **)(DAT_0013a66c + 0x13a3ee) == '\0') {
      for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 0xac); uVar11 = uVar11 + 1) {
        TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 0xac))[1] + uVar11 * 4),param_2);
      }
    }
    else {
      TouchButton::OnTouchMove(*(int *)(param_1 + 0xe4),param_2);
      *(undefined2 *)(param_1 + 0x108) = 0;
      iVar6 = *piVar8;
      iVar13 = *(int *)(iVar6 + 0x28);
      iVar5 = iVar13 + *(int *)(param_1 + 0x19c);
      iVar17 = *(int *)(param_1 + 0x158) + iVar5;
      if ((((iVar5 < param_2) && (param_2 < iVar17)) &&
          (*(int *)(iVar6 + 0xc) + iVar13 < (int)param_3)) &&
         ((int)param_3 < *(int *)(iVar6 + 0xc) + *(int *)(iVar6 + 0x20) + *(int *)(param_1 + 0x154))
         ) {
        *(undefined1 *)(param_1 + 0x108) = 1;
      }
      if (((iVar17 < param_2) &&
          (param_2 < (*(int *)(param_1 + 0x19c) - iVar13) + *(int *)(param_1 + 0x1a0))) &&
         ((iVar13 + *(int *)(iVar6 + 0xc) < (int)param_3 &&
          ((int)param_3 < *(int *)(iVar6 + 0x20) + *(int *)(iVar6 + 0xc) + *(int *)(param_1 + 0x154)
          )))) {
        *(undefined1 *)(param_1 + 0x109) = 1;
      }
      pcVar15 = *(code **)(DAT_0013a670 + 0x13a476);
      (*pcVar15)(*(undefined4 *)(param_1 + 0xcc),param_2,param_3);
      (*pcVar15)(*(undefined4 *)(param_1 + 0xd0),param_2,param_3);
      TouchSlider::OnTouchMove(**(int **)(*(int *)(param_1 + 0xec) + 4),param_2);
      (*pcVar15)(*(undefined4 *)(param_1 + 0xd4),param_2,param_3);
      (*pcVar15)(*(undefined4 *)(param_1 + 0xd8),param_2,param_3);
      (*pcVar15)(*(undefined4 *)(param_1 + 0xdc),param_2,param_3);
      piVar2 = *(int **)(DAT_0013a674 + 0x13a4b8);
      iVar6 = FModSound::tryToStopMusicForBGMusic();
      if (iVar6 == 0) {
        iVar6 = *piVar2;
        fVar20 = (float)TouchSlider::getValue
                                  (*(TouchSlider **)(*(int *)(*(int *)(param_1 + 0xec) + 4) + 4));
        FModSound::setVolume(iVar6,fVar20);
      }
      iVar6 = *piVar2;
      fVar20 = (float)TouchSlider::getValue
                                (*(TouchSlider **)(*(int *)(*(int *)(param_1 + 0xec) + 4) + 8));
      FModSound::setVolume(iVar6,fVar20);
      iVar6 = *piVar2;
      fVar20 = (float)TouchSlider::getValue
                                (*(TouchSlider **)(*(int *)(*(int *)(param_1 + 0xec) + 4) + 0xc));
      FModSound::setVolume(iVar6,fVar20);
      for (uVar11 = 1; uVar11 < **(uint **)(param_1 + 0xec); uVar11 = uVar11 + 1) {
        TouchSlider::OnTouchMove(*(int *)((*(uint **)(param_1 + 0xec))[1] + uVar11 * 4),param_2);
      }
      if ((**(char **)(DAT_0013a678 + 0x13a52c) != '\0') &&
         (iVar6 = *(int *)(param_1 + 0xe8), iVar6 != 0)) goto LAB_0013a3de;
    }
    break;
  case 3:
    ScrollTouchWindow::OnTouchMove(*(int *)(param_1 + 0xf0),param_2);
    for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 0xc0); uVar11 = uVar11 + 1) {
      puVar7 = *(uint **)((*(uint **)(param_1 + 0xc0))[1] + uVar11 * 4);
      uVar9 = *puVar7;
      if (((-(puVar7[1] - (uint)(uVar9 < 0x6a)) < (uint)(uVar9 - 0x6a < 5)) &&
          ((1 << (uVar9 - 0x6a & 0xff) & 0x19U) != 0)) || (puVar7[1] == 0 && uVar9 == 0x16)) {
        TouchButton::OnTouchMove((int)puVar7,param_2);
      }
    }
    break;
  default:
    for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 4); uVar11 = uVar11 + 1) {
      TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 4))[1] + uVar11 * 4),param_2);
    }
    uVar11 = 0;
    while( true ) {
      puVar7 = *(uint **)(param_1 + 0xc0);
      uVar9 = *puVar7;
      if (uVar9 <= uVar11) break;
      piVar2 = *(int **)(puVar7[1] + uVar11 * 4);
      if (*piVar2 - 0x17U < 2) {
        TouchButton::OnTouchBegin((int)piVar2,param_2);
      }
      uVar11 = uVar11 + 1;
    }
    for (uVar11 = 0; uVar11 < uVar9; uVar11 = uVar11 + 1) {
      puVar3 = *(uint **)(puVar7[1] + uVar11 * 4);
      uVar12 = *puVar3;
      uVar10 = puVar3[1];
      if (uVar12 == 5 && uVar10 == 0) {
LAB_0013a60a:
        TouchButton::OnTouchMove((int)puVar3,param_2);
        puVar7 = *(uint **)(param_1 + 0xc0);
        uVar9 = *puVar7;
      }
      else {
        uVar16 = 0;
        if (uVar12 != 0x11 || uVar10 != 0) {
          uVar16 = uVar12 ^ 100;
        }
        if ((uVar10 == 0 && (uVar12 == 0x11 || uVar16 == 0)) || (uVar10 == 0 && uVar12 == 0x35))
        goto LAB_0013a60a;
      }
    }
    break;
  case 5:
  case 0xc:
    break;
  case 6:
    pcVar15 = *(code **)(DAT_0013a67c + 0x13a632);
    (*pcVar15)(*(undefined4 *)(param_1 + 0xd4),param_2,param_3);
    (*pcVar15)(*(undefined4 *)(param_1 + 0xd8),param_2,param_3);
    (*pcVar15)(*(undefined4 *)(param_1 + 0xdc),param_2,param_3);
    piVar2 = *(int **)(DAT_0013a680 + 0x13a650);
    iVar6 = FModSound::tryToStopMusicForBGMusic();
    if (iVar6 == 0) {
      iVar6 = *piVar2;
      fVar20 = (float)TouchSlider::getValue
                                (*(TouchSlider **)(*(int *)(*(int *)(param_1 + 0xec) + 4) + 4));
      FModSound::setVolume(iVar6,fVar20);
    }
    piVar19 = (int *)(param_1 + 0xec);
    iVar6 = *piVar2;
    fVar20 = (float)TouchSlider::getValue(*(TouchSlider **)(*(int *)(*piVar19 + 4) + 8));
    FModSound::setVolume(iVar6,fVar20);
    iVar6 = *piVar2;
    fVar20 = (float)TouchSlider::getValue(*(TouchSlider **)(*(int *)(*piVar19 + 4) + 0xc));
    FModSound::setVolume(iVar6,fVar20);
    uVar11 = 1;
    while( true ) {
      uVar9 = *(uint *)*piVar19;
      if (uVar9 <= uVar11) break;
      if (uVar11 == 5) {
        uVar9 = (uint)*(byte *)(*piVar8 + 0x284);
      }
      if (uVar11 != 5 || uVar9 != 0) {
        TouchSlider::OnTouchMove(*(int *)(((uint *)*piVar19)[1] + uVar11 * 4),param_2);
      }
      uVar11 = uVar11 + 1;
    }
    break;
  case 7:
    *(undefined2 *)(param_1 + 0x108) = 0;
    iVar6 = *(int *)(pcVar1 + 0x28);
    iVar17 = iVar6 + *(int *)(param_1 + 0x19c);
    iVar13 = *(int *)(param_1 + 0x158) + iVar17;
    if (((iVar17 < param_2) && (param_2 < iVar13)) &&
       ((*(int *)(pcVar1 + 0xc) + iVar6 < (int)param_3 &&
        ((int)param_3 < *(int *)(pcVar1 + 0xc) + *(int *)(pcVar1 + 0x20) + *(int *)(param_1 + 0x154)
        )))) {
      *(undefined1 *)(param_1 + 0x108) = 1;
    }
    if ((((iVar13 < param_2) &&
         (param_2 < (*(int *)(param_1 + 0x19c) - iVar6) + *(int *)(param_1 + 0x1a0))) &&
        (iVar6 + *(int *)(pcVar1 + 0xc) < (int)param_3)) &&
       ((int)param_3 < *(int *)(pcVar1 + 0x20) + *(int *)(pcVar1 + 0xc) + *(int *)(param_1 + 0x154))
       ) {
      *(undefined1 *)(param_1 + 0x109) = 1;
    }
    TouchButton::OnTouchMove(*(int *)(param_1 + 0xcc),param_2);
    TouchButton::OnTouchMove(*(int *)(param_1 + 0xd0),param_2);
    TouchSlider::OnTouchMove(**(int **)(*(int *)(param_1 + 0xec) + 4),param_2);
    break;
  case 8:
    MissionsWindow::OnTouchMove(*(int *)(param_1 + 0xfc),param_2);
    break;
  case 10:
    if (*(int *)(param_1 + 0x16c) + -0x11 < 0) {
      param_2 = param_2 + -0x8dd;
    }
    if (!SBORROW4(iVar6,0x10)) {
      *(undefined4 *)pcVar1 = unaff_lr;
      *(undefined1 **)(pcVar1 + -4) = &stack0xffffff90;
      *(void **)(pcVar1 + -8) = param_3;
    }
    if ((pcVar1 != (char *)0x0) && (**(char **)(&UNK_0013a734 + _UNK_0013aa9c) == '\0')) {
      TouchButton::OnTouchMove(*(int *)(param_1 + 0x14),param_2);
      TouchButton::OnTouchMove(*(int *)(param_1 + 0x18),param_2);
      if (in_r3 != 0) {
        if (*(int *)(param_1 + 8) == in_r3) {
          puVar14 = *(undefined4 **)(&UNK_0013a9f4 + _UNK_0013aaa4);
          pGVar4 = (Globals *)**(undefined4 **)(&UNK_0013a9f2 + _UNK_0013aaa0);
          iVar6 = *(int *)(*(int *)(&UNK_0013a9f8 + _UNK_0013aaa8) + 0x54);
          pcVar15 = *(code **)(&UNK_0013a9fe + _UNK_0013aaac);
          iVar18 = *(int *)(param_1 + 0x90);
          iVar13 = (*pcVar15)(*puVar14,*(undefined4 *)(param_1 + 0x1c));
          iVar17 = (*pcVar15)(*puVar14,*(undefined4 *)(param_1 + 0x24));
          iVar5 = (*pcVar15)(*puVar14,*(undefined4 *)(param_1 + 0x20));
          Globals::setCoordsSteer
                    (pGVar4,(int)param_3 + (iVar6 - iVar18),iVar13,iVar17,iVar5,
                     (ushort *)(param_1 + 0x2e),(ushort *)(param_1 + 0x30),
                     (ushort *)(param_1 + 0x40),(ushort *)(param_1 + 0x42),
                     (ushort *)(param_1 + 0x3c),(ushort *)(param_1 + 0x3e),
                     (ushort *)(param_1 + 0x34),(ushort *)(param_1 + 0x32),
                     (ushort *)(param_1 + 0x48),(ushort *)(param_1 + 0x4a));
          *(void **)(param_1 + 0x90) = param_3;
        }
        else if (*(int *)(param_1 + 0xc) == in_r3) {
          iVar17 = *(int *)(param_1 + 0x94);
          pGVar4 = (Globals *)**(undefined4 **)(&UNK_0013a778 + _UNK_0013aab0);
          iVar6 = *(int *)(*(int *)(&UNK_0013a780 + _UNK_0013aab8) + 0x58);
          iVar13 = AbyssEngine::PaintCanvas::GetImage2DWidth
                             (**(uint **)(&UNK_0013a77a + _UNK_0013aab4));
          Globals::setCoordsFire
                    (pGVar4,(int)param_3 + (iVar6 - iVar17),iVar13,*(uint *)(param_1 + 0x78),
                     *(uint *)(param_1 + 0x7c),(uint *)(param_1 + 0x74),(ushort *)(param_1 + 0x60),
                     (ushort *)(param_1 + 0x62),(ushort *)(param_1 + 100),(ushort *)(param_1 + 0x66)
                     ,(ushort *)(param_1 + 0x68),(ushort *)(param_1 + 0x6a),
                     (ushort *)(param_1 + 0x6c),(ushort *)(param_1 + 0x6e),
                     (ushort *)(param_1 + 0x70),(ushort *)(param_1 + 0x72),
                     (ushort *)(param_1 + 0x4c),(ushort *)(param_1 + 0x4e));
          *(void **)(param_1 + 0x94) = param_3;
        }
      }
    }
    break;
  case 0xb:
    for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 0xb4); uVar11 = uVar11 + 1) {
      TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 0xb4))[1] + uVar11 * 4),param_2);
    }
    break;
  case 0xd:
    for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 0xb0); uVar11 = uVar11 + 1) {
      TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 0xb0))[1] + uVar11 * 4),param_2);
    }
    break;
  case 0xe:
    goto LAB_0013a880;
  case 0xf:
    for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 0xc0); uVar11 = uVar11 + 1) {
      puVar7 = *(uint **)((*(uint **)(param_1 + 0xc0))[1] + uVar11 * 4);
      if (-(puVar7[1] - (uint)(*puVar7 < 0x65)) < (uint)(*puVar7 - 0x65 < 5)) {
        TouchButton::OnTouchMove((int)puVar7,param_2);
      }
    }
LAB_0013a880:
    ScrollTouchWindow::OnTouchMove(*(int *)(param_1 + 0xf4),param_2);
    for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 0xc0); uVar11 = uVar11 + 1) {
      puVar7 = *(uint **)((*(uint **)(param_1 + 0xc0))[1] + uVar11 * 4);
      if ((*puVar7 | 8) == 0x3c && puVar7[1] == 0) {
        TouchButton::OnTouchMove((int)puVar7,param_2);
      }
    }
    for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 0xf8); uVar11 = uVar11 + 1) {
      TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 0xf8))[1] + uVar11 * 4),param_2);
    }
    iVar6 = *piVar8;
    if (((*(int *)(iVar6 + 0xc) < (int)param_3) &&
        ((int)param_3 < **(int **)(DAT_0013aa90 + 0x13a8ea) - *(int *)(iVar6 + 0x10))) &&
       (iVar13 = *(int *)(iVar6 + 0x28),
       iVar6 = AbyssEngine::PaintCanvas::GetImage2DWidth(**(uint **)(DAT_0013aa94 + 0x13a8fe)),
       param_2 < iVar6 + iVar13)) {
      iVar6 = (int)param_3 - *(int *)(param_1 + 0x20c);
      *(void **)(param_1 + 0x20c) = param_3;
      *(int *)(param_1 + 0x214) = iVar6;
      *(undefined4 *)(param_1 + 0x218) = 0x3f800000;
      *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + iVar6;
    }
    break;
  case 0x10:
    for (uVar11 = 0; uVar11 < **(uint **)(param_1 + 0xb8); uVar11 = uVar11 + 1) {
      TouchButton::OnTouchMove(*(int *)((*(uint **)(param_1 + 0xb8))[1] + uVar11 * 4),param_2);
    }
  }
  pcVar1 = (char *)*piVar8;
LAB_0013aa7e:
  Layout::OnTouchMove((int)pcVar1,param_2);
  return 0;
}

```

## target disasm
```
  0013a33c: push {r4,r5,r6,r7,lr}
  0013a33e: add r7,sp,#0xc
  0013a340: push {r8,r9,r10,r11}
  0013a344: sub sp,#0x4c
  0013a346: mov r9,r0
  0013a348: ldrb.w r0,[r0,#0x170]
  0013a34c: mov r5,r3
  0013a34e: mov r11,r2
  0013a350: mov r8,r1
  0013a352: cbz r0,0x0013a362
  0013a354: ldr.w r0,[r9,#0x104]
  0013a358: mov r1,r8
  0013a35a: mov r2,r11
  0013a35c: blx 0x0007474c
  0013a360: b 0x0013aa86
  0013a362: ldr r0,[0x0013a664]
  0013a364: add r0,pc
  0013a366: ldr r2,[r0,#0x0]
  0013a368: ldr r0,[r2,#0x0]
  0013a36a: ldrb r1,[r0,#0x0]
  0013a36c: cmp r1,#0x0
  0013a36e: bne.w 0x0013aa7e
  0013a372: ldr.w r1,[r9,#0x16c]
  0013a376: str r2,[sp,#0x48]
  0013a378: subs r1,#0x1
  0013a37a: cmp r1,#0x10
  0013a37c: bhi.w 0x0013a59a
  0013a380: tbh [pc,r1]
  0013a3a6: ldr r1,[r0,#0xc]
  0013a3a8: cmp r1,r11
  0013a3aa: bge 0x0013a3da
  0013a3ac: ldr r1,[0x0013a668]
  0013a3ae: ldr r0,[r0,#0x10]
  0013a3b0: add r1,pc
  0013a3b2: ldr r1,[r1,#0x0]
  0013a3b4: ldr r1,[r1,#0x0]
  0013a3b6: subs r0,r1,r0
  0013a3b8: cmp r0,r11
  0013a3ba: ble 0x0013a3da
  0013a3bc: ldr.w r1,[r9,#0x20c]
  0013a3c0: mov.w r2,#0x3f800000
  0013a3c4: ldr.w r0,[r9,#0x194]
  0013a3c8: sub.w r1,r11,r1
  0013a3cc: str.w r11,[r9,#0x20c]
  0013a3d0: strd r1,r2,[r9,#0x214]
  0013a3d4: add r0,r1
  0013a3d6: str.w r0,[r9,#0x194]
  0013a3da: ldr.w r0,[r9,#0xc4]
  0013a3de: mov r1,r8
  0013a3e0: mov r2,r11
  0013a3e2: blx 0x00074764
  0013a3e6: b 0x0013aa7a
  0013a3e8: ldr r0,[0x0013a66c]
  0013a3ea: add r0,pc
  0013a3ec: ldr r0,[r0,#0x0]
  0013a3ee: ldrb r0,[r0,#0x0]
  0013a3f0: cmp r0,#0x0
  0013a3f2: beq.w 0x0013a9c8
  0013a3f6: ldr.w r0,[r9,#0xe4]
  0013a3fa: mov r1,r8
  0013a3fc: mov r2,r11
  0013a3fe: blx 0x00074764
  0013a402: movs r0,#0x0
  0013a404: strh.w r0,[r9,#0x108]
  0013a408: ldr r0,[sp,#0x48]
  0013a40a: ldr.w r2,[r9,#0x19c]
  0013a40e: ldr.w r3,[r9,#0x158]
  0013a412: ldr r0,[r0,#0x0]
  0013a414: ldr r1,[r0,#0x28]
  0013a416: adds r5,r1,r2
  0013a418: add r3,r5
  0013a41a: cmp r5,r8
  0013a41c: bge 0x0013a43e
  0013a41e: cmp r3,r8
  0013a420: ble 0x0013a43e
  0013a422: ldr r5,[r0,#0xc]
  0013a424: adds r4,r5,r1
  0013a426: cmp r4,r11
  0013a428: bge 0x0013a43e
  0013a42a: ldr r4,[r0,#0x20]
  0013a42c: ldr.w r12,[r9,#0x154]
  0013a430: add r5,r4
  0013a432: add r5,r12
  0013a434: cmp r5,r11
  0013a436: itt gt
  0013a438: mov.gt r5,#0x1
  0013a43a: strb.gt.w r5,[r9,#0x108]
  0013a43e: cmp r3,r8
  0013a440: bge 0x0013a46a
  0013a442: ldr.w r3,[r9,#0x1a0]
  0013a446: subs r2,r2,r1
  0013a448: add r2,r3
  0013a44a: cmp r2,r8
  0013a44c: ble 0x0013a46a
  0013a44e: ldr r2,[r0,#0xc]
  0013a450: add r1,r2
  0013a452: cmp r1,r11
  0013a454: bge 0x0013a46a
  0013a456: ldr r0,[r0,#0x20]
  0013a458: ldr.w r1,[r9,#0x154]
  0013a45c: add r0,r2
  0013a45e: add r0,r1
  0013a460: cmp r0,r11
  0013a462: itt gt
  0013a464: mov.gt r0,#0x1
  0013a466: strb.gt.w r0,[r9,#0x109]
  0013a46a: ldr r1,[0x0013a670]
  0013a46c: mov r2,r11
  0013a46e: ldr.w r0,[r9,#0xcc]
  0013a472: add r1,pc
  0013a474: ldr r5,[r1,#0x0]
  0013a476: mov r1,r8
  0013a478: blx r5
  0013a47a: ldr.w r0,[r9,#0xd0]
  0013a47e: mov r1,r8
  0013a480: mov r2,r11
  0013a482: blx r5
  0013a484: ldr.w r0,[r9,#0xec]
  0013a488: mov r1,r8
  0013a48a: mov r2,r11
  0013a48c: ldr r0,[r0,#0x4]
  0013a48e: ldr r0,[r0,#0x0]
  0013a490: blx 0x00076630
  0013a494: ldr.w r0,[r9,#0xd4]
  0013a498: mov r1,r8
  0013a49a: mov r2,r11
  0013a49c: blx r5
  0013a49e: ldr.w r0,[r9,#0xd8]
  0013a4a2: mov r1,r8
  0013a4a4: mov r2,r11
  0013a4a6: blx r5
  0013a4a8: ldr.w r0,[r9,#0xdc]
  0013a4ac: mov r1,r8
  0013a4ae: mov r2,r11
  0013a4b0: blx r5
  0013a4b2: ldr r0,[0x0013a674]
  0013a4b4: add r0,pc
  0013a4b6: ldr r5,[r0,#0x0]
  0013a4b8: ldr r0,[r5,#0x0]
  0013a4ba: blx 0x000756c4
  0013a4be: cbnz r0,0x0013a4d8
  0013a4c0: ldr.w r0,[r9,#0xec]
  0013a4c4: ldr r4,[r5,#0x0]
  0013a4c6: ldr r0,[r0,#0x4]
  0013a4c8: ldr r0,[r0,#0x4]
  0013a4ca: blx 0x00072dd8
  0013a4ce: mov r2,r0
  0013a4d0: mov r0,r4
  0013a4d2: movs r1,#0x1
  0013a4d4: blx 0x00074920
  0013a4d8: ldr.w r0,[r9,#0xec]
  0013a4dc: ldr r4,[r5,#0x0]
  0013a4de: ldr r0,[r0,#0x4]
  0013a4e0: ldr r0,[r0,#0x8]
  0013a4e2: blx 0x00072dd8
  0013a4e6: mov r2,r0
  0013a4e8: mov r0,r4
  0013a4ea: movs r1,#0x2
  0013a4ec: blx 0x00074920
  0013a4f0: ldr.w r0,[r9,#0xec]
  0013a4f4: ldr r5,[r5,#0x0]
  0013a4f6: ldr r0,[r0,#0x4]
  0013a4f8: ldr r0,[r0,#0xc]
  0013a4fa: blx 0x00072dd8
  0013a4fe: mov r2,r0
  0013a500: mov r0,r5
  0013a502: movs r1,#0x3
  0013a504: blx 0x00074920
  0013a508: movs r4,#0x1
  0013a50a: b 0x0013a51c
  0013a50c: ldr r0,[r0,#0x4]
  0013a50e: mov r1,r8
  0013a510: mov r2,r11
  0013a512: ldr.w r0,[r0,r4,lsl #0x2]
  0013a516: blx 0x00076630
  0013a51a: adds r4,#0x1
  0013a51c: ldr.w r0,[r9,#0xec]
  0013a520: ldr r1,[r0,#0x0]
  0013a522: cmp r4,r1
  0013a524: bcc 0x0013a50c
  0013a526: ldr r0,[0x0013a678]
  0013a528: add r0,pc
  0013a52a: ldr r0,[r0,#0x0]
  0013a52c: ldrb r0,[r0,#0x0]
  0013a52e: cmp r0,#0x0
  0013a530: beq.w 0x0013aa7a
  0013a534: ldr.w r0,[r9,#0xe8]
  0013a538: cmp r0,#0x0
  0013a53a: bne.w 0x0013a3de
  0013a53e: b 0x0013aa7a
  0013a540: ldr.w r0,[r9,#0xf0]
  0013a544: mov r1,r8
  0013a546: mov r2,r11
  0013a548: blx 0x00075bbc
  0013a54c: mov.w r10,#0x0
  0013a550: movs r5,#0x0
  0013a552: b 0x0013a58e
  0013a554: ldr r0,[r0,#0x4]
  0013a556: ldr.w r0,[r0,r5,lsl #0x2]
  0013a55a: ldrd r2,r1,[r0,#0x0]
  0013a55e: subs.w r3,r2,#0x6a
  0013a562: sbc r4,r1,#0x0
  0013a566: rsbs.w r6,r3,#0x4
  0013a56a: sbcs.w r6,r10,r4
  0013a56e: bcc 0x0013a57c
  0013a570: movs r6,#0x1
  0013a572: lsl.w r3,r6,r3
  0013a576: tst r3,#0x19
  0013a57a: bne 0x0013a584
  0013a57c: eor r2,r2,#0x16
  0013a580: orrs r1,r2
  0013a582: bne 0x0013a58c
  0013a584: mov r1,r8
  0013a586: mov r2,r11
  0013a588: blx 0x00074764
  0013a58c: adds r5,#0x1
  0013a58e: ldr.w r0,[r9,#0xc0]
  0013a592: ldr r1,[r0,#0x0]
  0013a594: cmp r5,r1
  0013a596: bcc 0x0013a554
  0013a598: b 0x0013aa7a
  0013a59a: movs r4,#0x0
  0013a59c: b 0x0013a5ae
  0013a59e: ldr r0,[r0,#0x4]
  0013a5a0: mov r1,r8
  0013a5a2: mov r2,r11
  0013a5a4: ldr.w r0,[r0,r4,lsl #0x2]
  0013a5a8: blx 0x00074764
  0013a5ac: adds r4,#0x1
  0013a5ae: ldr.w r0,[r9,#0x4]
  0013a5b2: ldr r1,[r0,#0x0]
  0013a5b4: cmp r4,r1
  0013a5b6: bcc 0x0013a59e
  0013a5b8: movs r4,#0x0
  0013a5ba: b 0x0013a5d4
  0013a5bc: ldr r0,[r1,#0x4]
  0013a5be: ldr.w r0,[r0,r4,lsl #0x2]
  0013a5c2: ldr r1,[r0,#0x0]
  0013a5c4: subs r1,#0x17
  0013a5c6: cmp r1,#0x1
  0013a5c8: bhi 0x0013a5d2
  0013a5ca: mov r1,r8
  0013a5cc: mov r2,r11
  0013a5ce: blx 0x00074734
  0013a5d2: adds r4,#0x1
  0013a5d4: ldr.w r1,[r9,#0xc0]
  0013a5d8: ldr r2,[r1,#0x0]
  0013a5da: cmp r4,r2
  0013a5dc: bcc 0x0013a5bc
  0013a5de: movs r5,#0x0
  0013a5e0: cmp r5,r2
  0013a5e2: bcs.w 0x0013aa7a
  0013a5e6: ldr r0,[r1,#0x4]
  0013a5e8: ldr.w r0,[r0,r5,lsl #0x2]
  0013a5ec: ldrd r4,r3,[r0,#0x0]
  0013a5f0: eor r6,r4,#0x5
  0013a5f4: orrs r6,r3
  0013a5f6: beq 0x0013a60a
  0013a5f8: eor r6,r4,#0x11
  0013a5fc: orrs r6,r3
  0013a5fe: itt ne
  0013a600: eor.ne r6,r4,#0x64
  0013a604: orrs.ne.w r6,r6,r3
  0013a608: bne 0x0013a61c
  0013a60a: mov r1,r8
  0013a60c: mov r2,r11
  0013a60e: blx 0x00074764
  0013a612: ldr.w r1,[r9,#0xc0]
  0013a616: ldr r2,[r1,#0x0]
  0013a618: adds r5,#0x1
  0013a61a: b 0x0013a5e0
  0013a61c: eor r6,r4,#0x35
  0013a620: orrs r3,r6
  0013a622: beq 0x0013a60a
  0013a624: b 0x0013a618
  0013a626: ldr r1,[0x0013a67c]
  0013a628: mov r2,r11
  0013a62a: ldr.w r0,[r9,#0xd4]
  0013a62e: add r1,pc
  0013a630: ldr r4,[r1,#0x0]
  0013a632: mov r1,r8
  0013a634: blx r4
  0013a636: ldr.w r0,[r9,#0xd8]
  0013a63a: mov r1,r8
  0013a63c: mov r2,r11
  0013a63e: blx r4
  0013a640: ldr.w r0,[r9,#0xdc]
  0013a644: mov r1,r8
  0013a646: mov r2,r11
  0013a648: blx r4
  0013a64a: ldr r0,[0x0013a680]
  0013a64c: add r0,pc
  0013a64e: ldr r4,[r0,#0x0]
  0013a650: ldr r0,[r4,#0x0]
  0013a652: blx 0x000756c4
  0013a656: cmp r0,#0x0
  0013a658: beq.w 0x0013a950
  0013a65c: add.w r9,r9,#0xec
  0013a660: b 0x0013a968
  0013a684: movs r1,#0x0
  0013a686: ldr.w r3,[r9,#0x158]
  0013a68a: ldr.w r2,[r9,#0x19c]
  0013a68e: strh.w r1,[r9,#0x108]
  0013a692: ldr r1,[r0,#0x28]
  0013a694: adds r5,r1,r2
  0013a696: add r3,r5
  0013a698: cmp r5,r8
  0013a69a: bge 0x0013a6bc
  0013a69c: cmp r3,r8
  0013a69e: ble 0x0013a6bc
  0013a6a0: ldr r5,[r0,#0xc]
  0013a6a2: adds r4,r5,r1
  0013a6a4: cmp r4,r11
  0013a6a6: bge 0x0013a6bc
  0013a6a8: ldr r4,[r0,#0x20]
  0013a6aa: ldr.w r12,[r9,#0x154]
  0013a6ae: add r5,r4
  0013a6b0: add r5,r12
  0013a6b2: cmp r5,r11
  0013a6b4: itt gt
  0013a6b6: mov.gt r5,#0x1
  0013a6b8: strb.gt.w r5,[r9,#0x108]
  0013a6bc: cmp r3,r8
  0013a6be: bge 0x0013a6e8
  0013a6c0: ldr.w r3,[r9,#0x1a0]
  0013a6c4: subs r2,r2,r1
  0013a6c6: add r2,r3
  0013a6c8: cmp r2,r8
  0013a6ca: ble 0x0013a6e8
  0013a6cc: ldr r2,[r0,#0xc]
  0013a6ce: add r1,r2
  0013a6d0: cmp r1,r11
  0013a6d2: bge 0x0013a6e8
  0013a6d4: ldr r0,[r0,#0x20]
  0013a6d6: ldr.w r1,[r9,#0x154]
  0013a6da: add r0,r2
  0013a6dc: add r0,r1
  0013a6de: cmp r0,r11
  0013a6e0: itt gt
  0013a6e2: mov.gt r0,#0x1
  0013a6e4: strb.gt.w r0,[r9,#0x109]
  0013a6e8: ldr.w r0,[r9,#0xcc]
  0013a6ec: mov r1,r8
  0013a6ee: mov r2,r11
  0013a6f0: blx 0x00074764
  0013a6f4: ldr.w r0,[r9,#0xd0]
  0013a6f8: mov r1,r8
  0013a6fa: mov r2,r11
  0013a6fc: blx 0x00074764
  0013a700: ldr.w r0,[r9,#0xec]
  0013a704: mov r1,r8
  0013a706: mov r2,r11
  0013a708: ldr r0,[r0,#0x4]
  0013a70a: ldr r0,[r0,#0x0]
  0013a70c: blx 0x00076630
  0013a710: b 0x0013aa7a
  0013a712: ldr.w r0,[r9,#0xfc]
  0013a716: mov r1,r8
  0013a718: mov r2,r11
  0013a71a: blx 0x0007510c
  0013a71e: b 0x0013aa7a
  0013a720: ldrbtmi r4,[r8],#-0x8dd
  0013a724: stmdavc r0,{r11,sp,lr}
  0013a804: movs r4,#0x0
  0013a806: b 0x0013a818
  0013a808: ldr r0,[r0,#0x4]
  0013a80a: mov r1,r8
  0013a80c: mov r2,r11
  0013a80e: ldr.w r0,[r0,r4,lsl #0x2]
  0013a812: blx 0x00074764
  0013a816: adds r4,#0x1
  0013a818: ldr.w r0,[r9,#0xb4]
  0013a81c: ldr r1,[r0,#0x0]
  0013a81e: cmp r4,r1
  0013a820: bcc 0x0013a808
  0013a822: b 0x0013aa7a
  0013a824: movs r4,#0x0
  0013a826: b 0x0013a838
  0013a828: ldr r0,[r0,#0x4]
  0013a82a: mov r1,r8
  0013a82c: mov r2,r11
  0013a82e: ldr.w r0,[r0,r4,lsl #0x2]
  0013a832: blx 0x00074764
  0013a836: adds r4,#0x1
  0013a838: ldr.w r0,[r9,#0xb0]
  0013a83c: ldr r1,[r0,#0x0]
  0013a83e: cmp r4,r1
  0013a840: bcc 0x0013a828
  0013a842: b 0x0013aa7a
  0013a844: add.w r5,r9,#0xc0
  0013a848: b 0x0013a880
  0013a84a: add.w r5,r9,#0xc0
  0013a84e: movs r6,#0x0
  0013a850: movs r4,#0x0
  0013a852: b 0x0013a878
  0013a854: ldr r0,[r0,#0x4]
  0013a856: ldr.w r0,[r0,r4,lsl #0x2]
  0013a85a: ldrd r1,r2,[r0,#0x0]
  0013a85e: subs r1,#0x65
  0013a860: sbc r2,r2,#0x0
  0013a864: rsbs.w r1,r1,#0x4
  0013a868: sbcs.w r1,r6,r2
  0013a86c: bcc 0x0013a876
  0013a86e: mov r1,r8
  0013a870: mov r2,r11
  0013a872: blx 0x00074764
  0013a876: adds r4,#0x1
  0013a878: ldr r0,[r5,#0x0]
  0013a87a: ldr r1,[r0,#0x0]
  0013a87c: cmp r4,r1
  0013a87e: bcc 0x0013a854
  0013a880: ldr.w r0,[r9,#0xf4]
  0013a884: mov r1,r8
  0013a886: mov r2,r11
  0013a888: blx 0x00075bbc
  0013a88c: movs r4,#0x0
  0013a88e: b 0x0013a8b0
  0013a890: ldr r0,[r0,#0x4]
  0013a892: ldr.w r0,[r0,r4,lsl #0x2]
  0013a896: ldrd r1,r2,[r0,#0x0]
  0013a89a: orr r1,r1,#0x8
  0013a89e: eor r1,r1,#0x3c
  0013a8a2: orrs r1,r2
  0013a8a4: ittt eq
  0013a8a6: mov.eq r1,r8
  0013a8a8: mov.eq r2,r11
  0013a8aa: blx.eq 0x00074764
  0013a8ae: adds r4,#0x1
  0013a8b0: ldr r0,[r5,#0x0]
  0013a8b2: ldr r1,[r0,#0x0]
  0013a8b4: cmp r4,r1
  0013a8b6: bcc 0x0013a890
  0013a8b8: movs r4,#0x0
  0013a8ba: b 0x0013a8cc
  0013a8bc: ldr r0,[r0,#0x4]
  0013a8be: mov r1,r8
  0013a8c0: mov r2,r11
  0013a8c2: ldr.w r0,[r0,r4,lsl #0x2]
  0013a8c6: blx 0x00074764
  0013a8ca: adds r4,#0x1
  0013a8cc: ldr.w r0,[r9,#0xf8]
  0013a8d0: ldr r1,[r0,#0x0]
  0013a8d2: cmp r4,r1
  0013a8d4: bcc 0x0013a8bc
  0013a8d6: ldr r0,[sp,#0x48]
  0013a8d8: ldr r0,[r0,#0x0]
  0013a8da: ldr r1,[r0,#0xc]
  0013a8dc: cmp r1,r11
  0013a8de: bge.w 0x0013aa7a
  0013a8e2: ldr r1,[0x0013aa90]
  0013a8e4: ldr r2,[r0,#0x10]
  0013a8e6: add r1,pc
  0013a8e8: ldr r1,[r1,#0x0]
  0013a8ea: ldr r1,[r1,#0x0]
  0013a8ec: subs r1,r1,r2
  0013a8ee: cmp r1,r11
  0013a8f0: ble.w 0x0013aa7a
  0013a8f4: ldr r1,[0x0013aa94]
  0013a8f6: ldr.w r2,[r9,#0x134]
  0013a8fa: add r1,pc
  0013a8fc: ldr r4,[r0,#0x28]
  0013a8fe: ldr r3,[r1,#0x0]
  0013a900: ldr r1,[r2,#0x0]
  0013a902: ldr r0,[r3,#0x0]
  0013a904: blx 0x00072d84
  0013a908: add r0,r4
  0013a90a: cmp r0,r8
  0013a90c: ble.w 0x0013aa7a
  0013a910: ldr.w r1,[r9,#0x20c]
  0013a914: mov.w r2,#0x3f800000
  0013a918: ldr.w r0,[r9,#0x194]
  0013a91c: sub.w r1,r11,r1
  0013a920: str.w r11,[r9,#0x20c]
  0013a924: strd r1,r2,[r9,#0x214]
  0013a928: add r0,r1
  0013a92a: str.w r0,[r9,#0x194]
  0013a92e: b 0x0013aa7a
  0013a930: movs r4,#0x0
  0013a932: b 0x0013a944
  0013a934: ldr r0,[r0,#0x4]
  0013a936: mov r1,r8
  0013a938: mov r2,r11
  0013a93a: ldr.w r0,[r0,r4,lsl #0x2]
  0013a93e: blx 0x00074764
  0013a942: adds r4,#0x1
  0013a944: ldr.w r0,[r9,#0xb8]
  0013a948: ldr r1,[r0,#0x0]
  0013a94a: cmp r4,r1
  0013a94c: bcc 0x0013a934
  0013a94e: b 0x0013aa7a
  0013a950: ldr.w r0,[r9,#0xec]!
  0013a954: ldr r5,[r4,#0x0]
  0013a956: ldr r0,[r0,#0x4]
  0013a958: ldr r0,[r0,#0x4]
  0013a95a: blx 0x00072dd8
  0013a95e: mov r2,r0
  0013a960: mov r0,r5
  0013a962: movs r1,#0x1
  0013a964: blx 0x00074920
  0013a968: ldr.w r0,[r9,#0x0]
  0013a96c: ldr r5,[r4,#0x0]
  0013a96e: ldr r0,[r0,#0x4]
  0013a970: ldr r0,[r0,#0x8]
  0013a972: blx 0x00072dd8
  0013a976: mov r2,r0
  0013a978: mov r0,r5
  0013a97a: movs r1,#0x2
  0013a97c: blx 0x00074920
  0013a980: ldr.w r0,[r9,#0x0]
  0013a984: ldr r5,[r4,#0x0]
  0013a986: ldr r0,[r0,#0x4]
  0013a988: ldr r0,[r0,#0xc]
  0013a98a: blx 0x00072dd8
  0013a98e: mov r2,r0
  0013a990: mov r0,r5
  0013a992: movs r1,#0x3
  0013a994: blx 0x00074920
  0013a998: movs r5,#0x1
  0013a99a: b 0x0013a9bc
  0013a99c: cmp r5,#0x5
  0013a99e: itttt eq
  0013a9a0: ldr.eq r1,[sp,#0x48]
  0013a9a2: ldr.eq r1,[r1,#0x0]
  0013a9a4: ldrb.eq.w r1,[r1,#0x284]
  0013a9a8: cmp.eq r1,#0x0
  0013a9aa: beq 0x0013a9ba
  0013a9ac: ldr r0,[r0,#0x4]
  0013a9ae: mov r1,r8
  0013a9b0: mov r2,r11
  0013a9b2: ldr.w r0,[r0,r5,lsl #0x2]
  0013a9b6: blx 0x00076630
  0013a9ba: adds r5,#0x1
  0013a9bc: ldr.w r0,[r9,#0x0]
  0013a9c0: ldr r1,[r0,#0x0]
  0013a9c2: cmp r5,r1
  0013a9c4: bcc 0x0013a99c
  0013a9c6: b 0x0013aa7a
  0013a9c8: movs r4,#0x0
  0013a9ca: b 0x0013a9dc
  0013a9cc: ldr r0,[r0,#0x4]
  0013a9ce: mov r1,r8
  0013a9d0: mov r2,r11
  0013a9d2: ldr.w r0,[r0,r4,lsl #0x2]
  0013a9d6: blx 0x00074764
  0013a9da: adds r4,#0x1
  0013a9dc: ldr.w r0,[r9,#0xac]
  0013a9e0: ldr r1,[r0,#0x0]
  0013a9e2: cmp r4,r1
  0013a9e4: bcc 0x0013a9cc
  0013a9e6: b 0x0013aa7a
  0013aa7a: ldr r0,[sp,#0x48]
  0013aa7c: ldr r0,[r0,#0x0]
  0013aa7e: mov r1,r8
  0013aa80: mov r2,r11
  0013aa82: blx 0x00074758
  0013aa86: movs r0,#0x0
  0013aa88: add sp,#0x4c
  0013aa8a: pop.w {r8,r9,r10,r11}
  0013aa8e: pop {r4,r5,r6,r7,pc}
```
