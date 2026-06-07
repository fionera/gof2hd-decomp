# MenuTouchWindow::OnTouchBegin
elf 0x129bd4 body 1778
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2, void * param_3)

## decompile
```c

/* MenuTouchWindow::OnTouchBegin(int, int, void*) */

undefined4 MenuTouchWindow::OnTouchBegin(int param_1,int param_2,void *param_3)

{
  undefined1 uVar1;
  int *piVar2;
  uint *puVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  uint *puVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  int in_r3;
  int iVar12;
  uint uVar13;
  int iVar14;
  uint uVar15;
  uint uVar16;
  code *pcVar17;
  int iVar18;
  char *pcVar19;
  uint uVar20;
  float in_s0;
  float fVar21;
  
  if (*(char *)(param_1 + 0x170) != '\0') {
    ChoiceWindow::OnTouchBegin(*(int *)(param_1 + 0x104),param_2);
    return 0;
  }
  piVar5 = *(int **)(DAT_00139f54 + 0x139bfe);
  pcVar19 = (char *)*piVar5;
  if (*pcVar19 != '\0') {
    Layout::OnTouchBegin((int)pcVar19,param_2);
    return 0;
  }
  switch(*(undefined4 *)(param_1 + 0x16c)) {
  case 1:
  case 2:
    *(void **)(param_1 + 0x220) = param_3;
    *(void **)(param_1 + 0x20c) = param_3;
    *(undefined4 *)(param_1 + 0x214) = 0;
    *(undefined1 *)(param_1 + 0x224) = 1;
    iVar11 = *(int *)(param_1 + 0x18c);
    iVar18 = *(int *)(pcVar19 + 0xc);
    iVar4 = iVar11;
    if ((iVar18 < (int)param_3) &&
       ((int)param_3 < **(int **)(DAT_00139f58 + 0x139c6e) - *(int *)(pcVar19 + 0x10))) {
      iVar10 = *(int *)(pcVar19 + 0x70);
      iVar6 = *(int *)(param_1 + 0x1b4);
      iVar14 = *(int *)(pcVar19 + 0x20);
      iVar8 = *(int *)(param_1 + 0x194);
      iVar12 = __aeabi_idiv((int)param_3 + ((-iVar14 - iVar18) - iVar8));
      if (iVar12 < **(int **)(DAT_00139f5c + 0x139c9c)) {
        piVar2 = *(int **)(DAT_00139f60 + 0x139cb0);
        *(int *)(param_1 + 0x18c) = iVar12;
        fVar21 = (float)TouchButton::setPosition
                                  (*(TouchButton **)(param_1 + 0xc4),
                                   (*piVar2 - *(int *)(param_1 + 0x198)) - *(int *)(pcVar19 + 0x28),
                                   iVar12 * (iVar6 + iVar10) + iVar14 + iVar18 + iVar8 +
                                   *(int *)(pcVar19 + 0x108));
        FModSound::play(**(int **)(DAT_00139f64 + 0x139ce4),(Vector *)0x7c,(Vector *)0x0,fVar21);
        iVar4 = *(int *)(param_1 + 0x18c);
      }
    }
    if (iVar11 != iVar4) break;
    iVar4 = *(int *)(param_1 + 0xc4);
    goto LAB_00139cfc;
  case 3:
    if (**(char **)(DAT_00139f68 + 0x139d0c) == '\0') {
      for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 0xac); uVar16 = uVar16 + 1) {
        TouchButton::OnTouchBegin(*(int *)((*(uint **)(param_1 + 0xac))[1] + uVar16 * 4),param_2);
      }
      break;
    }
    fVar21 = (float)TouchButton::OnTouchBegin(*(int *)(param_1 + 0xe4),param_2);
    *(undefined2 *)(param_1 + 0x108) = 0;
    iVar4 = DAT_00139f6c;
    iVar10 = *(int *)(param_1 + 0x19c);
    iVar11 = *piVar5;
    iVar18 = *(int *)(iVar11 + 0x28);
    iVar12 = iVar18 + iVar10;
    if (((iVar12 < param_2) && (param_2 < *(int *)(param_1 + 0x158) + iVar12)) &&
       ((*(int *)(iVar11 + 0xc) + iVar18 < (int)param_3 &&
        ((int)param_3 < *(int *)(iVar11 + 0xc) + *(int *)(iVar11 + 0x20) + *(int *)(param_1 + 0x154)
        )))) {
      *(undefined1 *)(param_1 + 0x108) = 1;
      fVar21 = (float)FModSound::play(**(int **)(iVar4 + 0x139d64),(Vector *)0x7c,(Vector *)0x0,
                                      fVar21);
      iVar11 = *piVar5;
      iVar10 = *(int *)(param_1 + 0x19c);
      iVar18 = *(int *)(iVar11 + 0x28);
      iVar12 = iVar18 + iVar10;
    }
    iVar4 = DAT_00139f70;
    if (((iVar12 + *(int *)(param_1 + 0x158) < param_2) &&
        (param_2 < (iVar10 - iVar18) + *(int *)(param_1 + 0x1a0))) &&
       ((iVar18 + *(int *)(iVar11 + 0xc) < (int)param_3 &&
        ((int)param_3 < *(int *)(iVar11 + 0x20) + *(int *)(iVar11 + 0xc) + *(int *)(param_1 + 0x154)
        )))) {
      *(undefined1 *)(param_1 + 0x109) = 1;
      FModSound::play(**(int **)(iVar4 + 0x139db6),(Vector *)0x7c,(Vector *)0x0,fVar21);
    }
    pcVar17 = *(code **)(DAT_00139f74 + 0x139dd0);
    (*pcVar17)(*(undefined4 *)(param_1 + 0xcc),param_2,param_3);
    (*pcVar17)(*(undefined4 *)(param_1 + 0xd0),param_2,param_3);
    TouchSlider::OnTouchBegin(**(int **)(*(int *)(param_1 + 0xec) + 4),param_2);
    (*pcVar17)(*(undefined4 *)(param_1 + 0xd4),param_2,param_3);
    (*pcVar17)(*(undefined4 *)(param_1 + 0xd8),param_2,param_3);
    (*pcVar17)(*(undefined4 *)(param_1 + 0xdc),param_2,param_3);
    for (uVar16 = 1; uVar16 < **(uint **)(param_1 + 0xec); uVar16 = uVar16 + 1) {
      TouchSlider::OnTouchBegin(*(int *)((*(uint **)(param_1 + 0xec))[1] + uVar16 * 4),param_2);
    }
    if ((**(char **)(DAT_00139f78 + 0x139e30) == '\0') ||
       (iVar4 = *(int *)(param_1 + 0xe8), iVar4 == 0)) break;
LAB_00139cfc:
    TouchButton::OnTouchBegin(iVar4,param_2);
    break;
  case 4:
    ScrollTouchWindow::OnTouchBegin(*(int *)(param_1 + 0xf0),param_2);
    for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 0xc0); uVar16 = uVar16 + 1) {
      puVar7 = *(uint **)((*(uint **)(param_1 + 0xc0))[1] + uVar16 * 4);
      uVar9 = *puVar7;
      if (((-(puVar7[1] - (uint)(uVar9 < 0x6a)) < (uint)(uVar9 - 0x6a < 5)) &&
          ((1 << (uVar9 - 0x6a & 0xff) & 0x19U) != 0)) || (puVar7[1] == 0 && uVar9 == 0x16)) {
        TouchButton::OnTouchBegin((int)puVar7,param_2);
      }
    }
    break;
  default:
    for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 4); uVar16 = uVar16 + 1) {
      TouchButton::OnTouchBegin(*(int *)((*(uint **)(param_1 + 4))[1] + uVar16 * 4),param_2);
    }
    uVar16 = 0;
    while( true ) {
      puVar7 = *(uint **)(param_1 + 0xc0);
      uVar9 = *puVar7;
      if (uVar9 <= uVar16) break;
      piVar2 = *(int **)(puVar7[1] + uVar16 * 4);
      if (*piVar2 - 0x17U < 2) {
        TouchButton::OnTouchBegin((int)piVar2,param_2);
      }
      uVar16 = uVar16 + 1;
    }
    for (uVar16 = 0; uVar16 < uVar9; uVar16 = uVar16 + 1) {
      puVar3 = *(uint **)(puVar7[1] + uVar16 * 4);
      uVar20 = *puVar3;
      uVar13 = puVar3[1];
      if (uVar20 == 5 && uVar13 == 0) {
LAB_00139f0e:
        TouchButton::OnTouchBegin((int)puVar3,param_2);
        puVar7 = *(uint **)(param_1 + 0xc0);
        uVar9 = *puVar7;
      }
      else {
        uVar15 = 0;
        if (uVar20 != 0x11 || uVar13 != 0) {
          uVar15 = uVar20 ^ 100;
        }
        if ((uVar13 == 0 && (uVar20 == 0x11 || uVar15 == 0)) || (uVar13 == 0 && uVar20 == 0x35))
        goto LAB_00139f0e;
      }
    }
    break;
  case 6:
  case 0xd:
    break;
  case 7:
    pcVar17 = *(code **)(DAT_00139f7c + 0x139f36);
    (*pcVar17)(*(undefined4 *)(param_1 + 0xd4),param_2,param_3);
    (*pcVar17)(*(undefined4 *)(param_1 + 0xd8),param_2,param_3);
    (*pcVar17)(*(undefined4 *)(param_1 + 0xdc),param_2,param_3);
    uVar16 = 1;
    while( true ) {
      uVar9 = **(uint **)(param_1 + 0xec);
      if (uVar9 <= uVar16) break;
      if (uVar16 == 5) {
        uVar9 = (uint)*(byte *)(*piVar5 + 0x284);
      }
      if (uVar16 != 5 || uVar9 != 0) {
        TouchSlider::OnTouchBegin(*(int *)((*(uint **)(param_1 + 0xec))[1] + uVar16 * 4),param_2);
      }
      uVar16 = uVar16 + 1;
    }
    break;
  case 8:
    *(undefined2 *)(param_1 + 0x108) = 0;
    iVar4 = DAT_0013a334;
    iVar18 = *(int *)(param_1 + 0x19c);
    iVar11 = *(int *)(pcVar19 + 0x28);
    iVar10 = iVar11 + iVar18;
    if ((((iVar10 < param_2) && (param_2 < *(int *)(param_1 + 0x158) + iVar10)) &&
        (*(int *)(pcVar19 + 0xc) + iVar11 < (int)param_3)) &&
       ((int)param_3 <
        *(int *)(pcVar19 + 0xc) + *(int *)(pcVar19 + 0x20) + *(int *)(param_1 + 0x154))) {
      *(undefined1 *)(param_1 + 0x108) = 1;
      in_s0 = (float)FModSound::play(**(int **)(iVar4 + 0x139fec),(Vector *)0x7c,(Vector *)0x0,in_s0
                                    );
      iVar18 = *(int *)(param_1 + 0x19c);
      pcVar19 = (char *)*piVar5;
      iVar11 = *(int *)(pcVar19 + 0x28);
      iVar10 = iVar11 + iVar18;
    }
    iVar4 = DAT_0013a338;
    if (((iVar10 + *(int *)(param_1 + 0x158) < param_2) &&
        (param_2 < (iVar18 - iVar11) + *(int *)(param_1 + 0x1a0))) &&
       ((iVar11 + *(int *)(pcVar19 + 0xc) < (int)param_3 &&
        ((int)param_3 <
         *(int *)(pcVar19 + 0x20) + *(int *)(pcVar19 + 0xc) + *(int *)(param_1 + 0x154))))) {
      *(undefined1 *)(param_1 + 0x109) = 1;
      FModSound::play(**(int **)(iVar4 + 0x13a040),(Vector *)0x7c,(Vector *)0x0,in_s0);
    }
    TouchButton::OnTouchBegin(*(int *)(param_1 + 0xcc),param_2);
    TouchButton::OnTouchBegin(*(int *)(param_1 + 0xd0),param_2);
    TouchSlider::OnTouchBegin(**(int **)(*(int *)(param_1 + 0xec) + 4),param_2);
    break;
  case 9:
    MissionsWindow::OnTouchBegin(*(int *)(param_1 + 0xfc),param_2);
    break;
  case 0xb:
    if ((**(char **)(DAT_0013a320 + 0x13a08c) != '\0') &&
       (**(char **)(DAT_0013a324 + 0x13a09a) == '\0')) {
      TouchButton::OnTouchBegin(*(int *)(param_1 + 0x14),param_2);
      TouchButton::OnTouchBegin(*(int *)(param_1 + 0x18),param_2);
      if ((in_r3 == 0) ||
         ((((*(int *)(param_1 + 8) != 0 || (0xd1 < param_2)) || (*(int *)(param_1 + 0xc) == in_r3))
          || ((iVar4 = *(int *)(*(int *)(DAT_0013a328 + 0x13a0de) + 0x54),
              (int)param_3 <= iVar4 + -0x14 || (iVar4 + 300 <= (int)param_3)))))) {
        if ((*(int *)(param_1 + 8) == in_r3 || in_r3 == 0) ||
           (((*(int *)(param_1 + 0xc) != 0 ||
             (param_2 <= **(int **)(DAT_0013a32c + 0x13a2e6) + -0xdc)) ||
            ((iVar4 = *(int *)(*(int *)(DAT_0013a330 + 0x13a2f4) + 0x58),
             (int)param_3 <= iVar4 + -0x14 || (iVar4 + 0xe6 <= (int)param_3)))))) {
          *(undefined2 *)(param_1 + 0x98) = 0;
        }
        else {
          *(int *)(param_1 + 0xc) = in_r3;
          *(undefined2 *)(param_1 + 0x98) = 0x100;
          *(void **)(param_1 + 0x94) = param_3;
        }
      }
      else {
        *(int *)(param_1 + 8) = in_r3;
        *(undefined2 *)(param_1 + 0x98) = 1;
        *(void **)(param_1 + 0x90) = param_3;
      }
    }
    break;
  case 0xc:
    for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 0xb4); uVar16 = uVar16 + 1) {
      TouchButton::OnTouchBegin(*(int *)((*(uint **)(param_1 + 0xb4))[1] + uVar16 * 4),param_2);
    }
    break;
  case 0xe:
    for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 0xb0); uVar16 = uVar16 + 1) {
      TouchButton::OnTouchBegin(*(int *)((*(uint **)(param_1 + 0xb0))[1] + uVar16 * 4),param_2);
    }
    break;
  case 0xf:
    goto LAB_0013a182;
  case 0x10:
    for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 0xc0); uVar16 = uVar16 + 1) {
      puVar7 = *(uint **)((*(uint **)(param_1 + 0xc0))[1] + uVar16 * 4);
      if (-(puVar7[1] - (uint)(*puVar7 < 0x65)) < (uint)(*puVar7 - 0x65 < 5)) {
        TouchButton::OnTouchBegin((int)puVar7,param_2);
      }
    }
LAB_0013a182:
    ScrollTouchWindow::OnTouchBegin(*(int *)(param_1 + 0xf4),param_2);
    for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 0xc0); uVar16 = uVar16 + 1) {
      puVar7 = *(uint **)((*(uint **)(param_1 + 0xc0))[1] + uVar16 * 4);
      if ((*puVar7 | 8) == 0x3c && puVar7[1] == 0) {
        TouchButton::OnTouchBegin((int)puVar7,param_2);
      }
    }
    for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 0xf8); uVar16 = uVar16 + 1) {
      TouchButton::OnTouchBegin(*(int *)((*(uint **)(param_1 + 0xf8))[1] + uVar16 * 4),param_2);
    }
    puVar7 = *(uint **)(DAT_0013a31c + 0x13a1e8);
    iVar18 = **(int **)(DAT_0013a318 + 0x13a1e2);
    iVar11 = *(int *)(*piVar5 + 0x28);
    iVar4 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    if (((iVar18 - iVar11) - iVar4 < param_2) &&
       (iVar11 = *(int *)(*piVar5 + 0xc), iVar18 = *(int *)(*piVar5 + 0x20),
       iVar4 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar7),
       (int)param_3 < iVar4 + iVar18 + iVar11)) {
      uVar1 = 1;
    }
    else {
      uVar1 = 0;
    }
    *(undefined1 *)(param_1 + 0x1d9) = uVar1;
    iVar11 = *(int *)(*piVar5 + 0x28);
    iVar4 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    if (param_2 < iVar4 + iVar11) {
      *(void **)(param_1 + 0x220) = param_3;
      *(void **)(param_1 + 0x20c) = param_3;
      *(undefined4 *)(param_1 + 0x214) = 0;
      *(undefined1 *)(param_1 + 0x224) = 1;
    }
    break;
  case 0x11:
    for (uVar16 = 0; uVar16 < **(uint **)(param_1 + 0xb8); uVar16 = uVar16 + 1) {
      TouchButton::OnTouchBegin(*(int *)((*(uint **)(param_1 + 0xb8))[1] + uVar16 * 4),param_2);
    }
  }
  iVar4 = Layout::OnTouchBegin(*piVar5,param_2);
  if ((iVar4 != 0) && (*(int *)(param_1 + 0x16c) == 0xd)) {
    *(undefined1 *)(param_1 + 1) = 1;
  }
  return 0;
}

```

## target disasm
```
  00139bd4: push {r4,r5,r6,r7,lr}
  00139bd6: add r7,sp,#0xc
  00139bd8: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00139bdc: mov r10,r0
  00139bde: ldrb.w r0,[r0,#0x170]
  00139be2: mov r5,r3
  00139be4: mov r9,r2
  00139be6: mov r8,r1
  00139be8: cbz r0,0x00139bf8
  00139bea: ldr.w r0,[r10,#0x104]
  00139bee: mov r1,r8
  00139bf0: mov r2,r9
  00139bf2: blx 0x0007471c
  00139bf6: b 0x0013a2d2
  00139bf8: ldr r0,[0x00139f54]
  00139bfa: add r0,pc
  00139bfc: ldr r1,[r0,#0x0]
  00139bfe: ldr r6,[r1,#0x0]
  00139c00: ldrb r0,[r6,#0x0]
  00139c02: cbz r0,0x00139c10
  00139c04: mov r0,r6
  00139c06: mov r1,r8
  00139c08: mov r2,r9
  00139c0a: blx 0x00074728
  00139c0e: b 0x0013a2d2
  00139c10: ldr.w r0,[r10,#0x16c]
  00139c14: str r1,[sp,#0x10]
  00139c16: subs r0,#0x1
  00139c18: cmp r0,#0x10
  00139c1a: bhi.w 0x00139e9e
  00139c1e: tbh [pc,r0]
  00139c44: movs r0,#0x0
  00139c46: str.w r9,[r10,#0x220]
  00139c4a: str.w r9,[r10,#0x20c]
  00139c4e: str.w r0,[r10,#0x214]
  00139c52: movs r0,#0x1
  00139c54: strb.w r0,[r10,#0x224]
  00139c58: ldr.w r5,[r10,#0x18c]
  00139c5c: ldr.w r11,[r6,#0xc]
  00139c60: cmp r11,r9
  00139c62: mov r1,r5
  00139c64: bge 0x00139cf2
  00139c66: ldr r0,[0x00139f58]
  00139c68: ldr r1,[r6,#0x10]
  00139c6a: add r0,pc
  00139c6c: ldr r0,[r0,#0x0]
  00139c6e: ldr r0,[r0,#0x0]
  00139c70: subs r0,r0,r1
  00139c72: mov r1,r5
  00139c74: cmp r0,r9
  00139c76: ble 0x00139cf2
  00139c78: ldr r0,[r6,#0x70]
  00139c7a: ldr.w r1,[r10,#0x1b4]
  00139c7e: ldr r4,[r6,#0x20]
  00139c80: add r1,r0
  00139c82: sub.w r0,r9,r11
  00139c86: ldr.w r2,[r10,#0x194]
  00139c8a: subs r0,r0,r4
  00139c8c: str r2,[sp,#0xc]
  00139c8e: subs r0,r0,r2
  00139c90: str r1,[sp,#0x8]
  00139c92: blx 0x0007198c
  00139c96: ldr r1,[0x00139f5c]
  00139c98: add r1,pc
  00139c9a: ldr r1,[r1,#0x0]
  00139c9c: ldr r1,[r1,#0x0]
  00139c9e: cmp r0,r1
  00139ca0: mov r1,r5
  00139ca2: bge 0x00139cf2
  00139ca4: ldr r1,[0x00139f60]
  00139ca6: add.w r2,r4,r11
  00139caa: ldr r3,[sp,#0xc]
  00139cac: add r1,pc
  00139cae: ldr.w r12,[r10,#0xc4]
  00139cb2: add r2,r3
  00139cb4: ldr r3,[sp,#0x8]
  00139cb6: ldr r1,[r1,#0x0]
  00139cb8: mla r2,r0,r3,r2
  00139cbc: ldr.w r3,[r10,#0x198]
  00139cc0: str.w r0,[r10,#0x18c]
  00139cc4: ldr r1,[r1,#0x0]
  00139cc6: ldr r0,[r6,#0x28]
  00139cc8: ldr.w r6,[r6,#0x108]
  00139ccc: subs r1,r1,r3
  00139cce: subs r1,r1,r0
  00139cd0: mov r0,r12
  00139cd2: add r2,r6
  00139cd4: blx 0x00076498
  00139cd8: ldr r0,[0x00139f64]
  00139cda: movs r1,#0x0
  00139cdc: str r1,[sp,#0x0]
  00139cde: movs r1,#0x7c
  00139ce0: add r0,pc
  00139ce2: movs r2,#0x0
  00139ce4: movs r3,#0x0
  00139ce6: ldr r0,[r0,#0x0]
  00139ce8: ldr r0,[r0,#0x0]
  00139cea: blx 0x00071548
  00139cee: ldr.w r1,[r10,#0x18c]
  00139cf2: cmp r5,r1
  00139cf4: bne.w 0x0013a2b6
  00139cf8: ldr.w r0,[r10,#0xc4]
  00139cfc: mov r1,r8
  00139cfe: mov r2,r9
  00139d00: blx 0x00074734
  00139d04: b 0x0013a2b6
  00139d06: ldr r0,[0x00139f68]
  00139d08: add r0,pc
  00139d0a: ldr r0,[r0,#0x0]
  00139d0c: ldrb r0,[r0,#0x0]
  00139d0e: cmp r0,#0x0
  00139d10: beq.w 0x0013a298
  00139d14: ldr.w r0,[r10,#0xe4]
  00139d18: mov r1,r8
  00139d1a: mov r2,r9
  00139d1c: blx 0x00074734
  00139d20: ldr r4,[sp,#0x10]
  00139d22: movs r0,#0x0
  00139d24: strh.w r0,[r10,#0x108]
  00139d28: ldr.w r2,[r10,#0x19c]
  00139d2c: ldr r0,[r4,#0x0]
  00139d2e: ldr r1,[r0,#0x28]
  00139d30: adds r3,r1,r2
  00139d32: cmp r3,r8
  00139d34: bge 0x00139d7c
  00139d36: ldr.w r6,[r10,#0x158]
  00139d3a: add r6,r3
  00139d3c: cmp r6,r8
  00139d3e: ble 0x00139d7c
  00139d40: ldr r6,[r0,#0xc]
  00139d42: adds r5,r6,r1
  00139d44: cmp r5,r9
  00139d46: bge 0x00139d7c
  00139d48: ldr r5,[r0,#0x20]
  00139d4a: ldr.w r12,[r10,#0x154]
  00139d4e: add r6,r5
  00139d50: add r6,r12
  00139d52: cmp r6,r9
  00139d54: ble 0x00139d7c
  00139d56: ldr r0,[0x00139f6c]
  00139d58: movs r1,#0x1
  00139d5a: strb.w r1,[r10,#0x108]
  00139d5e: movs r1,#0x0
  00139d60: add r0,pc
  00139d62: str r1,[sp,#0x0]
  00139d64: movs r1,#0x7c
  00139d66: movs r2,#0x0
  00139d68: ldr r0,[r0,#0x0]
  00139d6a: movs r3,#0x0
  00139d6c: ldr r0,[r0,#0x0]
  00139d6e: blx 0x00071548
  00139d72: ldr r0,[r4,#0x0]
  00139d74: ldr.w r2,[r10,#0x19c]
  00139d78: ldr r1,[r0,#0x28]
  00139d7a: adds r3,r1,r2
  00139d7c: ldr.w r6,[r10,#0x158]
  00139d80: add r3,r6
  00139d82: cmp r3,r8
  00139d84: bge 0x00139dc4
  00139d86: ldr.w r3,[r10,#0x1a0]
  00139d8a: subs r2,r2,r1
  00139d8c: add r2,r3
  00139d8e: cmp r2,r8
  00139d90: ble 0x00139dc4
  00139d92: ldr r2,[r0,#0xc]
  00139d94: add r1,r2
  00139d96: cmp r1,r9
  00139d98: bge 0x00139dc4
  00139d9a: ldr r0,[r0,#0x20]
  00139d9c: ldr.w r1,[r10,#0x154]
  00139da0: add r0,r2
  00139da2: add r0,r1
  00139da4: cmp r0,r9
  00139da6: ble 0x00139dc4
  00139da8: ldr r0,[0x00139f70]
  00139daa: movs r1,#0x1
  00139dac: strb.w r1,[r10,#0x109]
  00139db0: movs r1,#0x0
  00139db2: add r0,pc
  00139db4: str r1,[sp,#0x0]
  00139db6: movs r1,#0x7c
  00139db8: movs r2,#0x0
  00139dba: ldr r0,[r0,#0x0]
  00139dbc: movs r3,#0x0
  00139dbe: ldr r0,[r0,#0x0]
  00139dc0: blx 0x00071548
  00139dc4: ldr r1,[0x00139f74]
  00139dc6: mov r2,r9
  00139dc8: ldr.w r0,[r10,#0xcc]
  00139dcc: add r1,pc
  00139dce: ldr r5,[r1,#0x0]
  00139dd0: mov r1,r8
  00139dd2: blx r5
  00139dd4: ldr.w r0,[r10,#0xd0]
  00139dd8: mov r1,r8
  00139dda: mov r2,r9
  00139ddc: blx r5
  00139dde: ldr.w r0,[r10,#0xec]
  00139de2: mov r1,r8
  00139de4: mov r2,r9
  00139de6: ldr r0,[r0,#0x4]
  00139de8: ldr r0,[r0,#0x0]
  00139dea: blx 0x00076624
  00139dee: ldr.w r0,[r10,#0xd4]
  00139df2: mov r1,r8
  00139df4: mov r2,r9
  00139df6: blx r5
  00139df8: ldr.w r0,[r10,#0xd8]
  00139dfc: mov r1,r8
  00139dfe: mov r2,r9
  00139e00: blx r5
  00139e02: ldr.w r0,[r10,#0xdc]
  00139e06: mov r1,r8
  00139e08: mov r2,r9
  00139e0a: blx r5
  00139e0c: movs r5,#0x1
  00139e0e: b 0x00139e20
  00139e10: ldr r0,[r0,#0x4]
  00139e12: mov r1,r8
  00139e14: mov r2,r9
  00139e16: ldr.w r0,[r0,r5,lsl #0x2]
  00139e1a: blx 0x00076624
  00139e1e: adds r5,#0x1
  00139e20: ldr.w r0,[r10,#0xec]
  00139e24: ldr r1,[r0,#0x0]
  00139e26: cmp r5,r1
  00139e28: bcc 0x00139e10
  00139e2a: ldr r0,[0x00139f78]
  00139e2c: add r0,pc
  00139e2e: ldr r0,[r0,#0x0]
  00139e30: ldrb r0,[r0,#0x0]
  00139e32: cmp r0,#0x0
  00139e34: beq.w 0x0013a2b6
  00139e38: ldr.w r0,[r10,#0xe8]
  00139e3c: cmp r0,#0x0
  00139e3e: bne.w 0x00139cfc
  00139e42: b 0x0013a2b6
  00139e44: ldr.w r0,[r10,#0xf0]
  00139e48: mov r1,r8
  00139e4a: mov r2,r9
  00139e4c: blx 0x00075ba4
  00139e50: mov.w r11,#0x0
  00139e54: movs r6,#0x0
  00139e56: b 0x00139e92
  00139e58: ldr r0,[r0,#0x4]
  00139e5a: ldr.w r0,[r0,r6,lsl #0x2]
  00139e5e: ldrd r2,r1,[r0,#0x0]
  00139e62: subs.w r3,r2,#0x6a
  00139e66: sbc r5,r1,#0x0
  00139e6a: rsbs.w r4,r3,#0x4
  00139e6e: sbcs.w r5,r11,r5
  00139e72: bcc 0x00139e80
  00139e74: movs r5,#0x1
  00139e76: lsl.w r3,r5,r3
  00139e7a: tst r3,#0x19
  00139e7e: bne 0x00139e88
  00139e80: eor r2,r2,#0x16
  00139e84: orrs r1,r2
  00139e86: bne 0x00139e90
  00139e88: mov r1,r8
  00139e8a: mov r2,r9
  00139e8c: blx 0x00074734
  00139e90: adds r6,#0x1
  00139e92: ldr.w r0,[r10,#0xc0]
  00139e96: ldr r1,[r0,#0x0]
  00139e98: cmp r6,r1
  00139e9a: bcc 0x00139e58
  00139e9c: b 0x0013a2b6
  00139e9e: movs r5,#0x0
  00139ea0: b 0x00139eb2
  00139ea2: ldr r0,[r0,#0x4]
  00139ea4: mov r1,r8
  00139ea6: mov r2,r9
  00139ea8: ldr.w r0,[r0,r5,lsl #0x2]
  00139eac: blx 0x00074734
  00139eb0: adds r5,#0x1
  00139eb2: ldr.w r0,[r10,#0x4]
  00139eb6: ldr r1,[r0,#0x0]
  00139eb8: cmp r5,r1
  00139eba: bcc 0x00139ea2
  00139ebc: movs r5,#0x0
  00139ebe: b 0x00139ed8
  00139ec0: ldr r0,[r1,#0x4]
  00139ec2: ldr.w r0,[r0,r5,lsl #0x2]
  00139ec6: ldr r1,[r0,#0x0]
  00139ec8: subs r1,#0x17
  00139eca: cmp r1,#0x1
  00139ecc: bhi 0x00139ed6
  00139ece: mov r1,r8
  00139ed0: mov r2,r9
  00139ed2: blx 0x00074734
  00139ed6: adds r5,#0x1
  00139ed8: ldr.w r1,[r10,#0xc0]
  00139edc: ldr r2,[r1,#0x0]
  00139ede: cmp r5,r2
  00139ee0: bcc 0x00139ec0
  00139ee2: movs r5,#0x0
  00139ee4: cmp r5,r2
  00139ee6: bcs.w 0x0013a2b6
  00139eea: ldr r0,[r1,#0x4]
  00139eec: ldr.w r0,[r0,r5,lsl #0x2]
  00139ef0: ldrd r6,r3,[r0,#0x0]
  00139ef4: eor r4,r6,#0x5
  00139ef8: orrs r4,r3
  00139efa: beq 0x00139f0e
  00139efc: eor r4,r6,#0x11
  00139f00: orrs r4,r3
  00139f02: itt ne
  00139f04: eor.ne r4,r6,#0x64
  00139f08: orrs.ne.w r4,r4,r3
  00139f0c: bne 0x00139f20
  00139f0e: mov r1,r8
  00139f10: mov r2,r9
  00139f12: blx 0x00074734
  00139f16: ldr.w r1,[r10,#0xc0]
  00139f1a: ldr r2,[r1,#0x0]
  00139f1c: adds r5,#0x1
  00139f1e: b 0x00139ee4
  00139f20: eor r6,r6,#0x35
  00139f24: orrs r3,r6
  00139f26: beq 0x00139f0e
  00139f28: b 0x00139f1c
  00139f2a: ldr r1,[0x00139f7c]
  00139f2c: mov r2,r9
  00139f2e: ldr.w r0,[r10,#0xd4]
  00139f32: add r1,pc
  00139f34: ldr r5,[r1,#0x0]
  00139f36: mov r1,r8
  00139f38: blx r5
  00139f3a: ldr.w r0,[r10,#0xd8]
  00139f3e: mov r1,r8
  00139f40: mov r2,r9
  00139f42: blx r5
  00139f44: ldr.w r0,[r10,#0xdc]
  00139f48: mov r1,r8
  00139f4a: mov r2,r9
  00139f4c: blx r5
  00139f4e: movs r5,#0x1
  00139f50: b 0x00139fa0
  00139f80: cmp r5,#0x5
  00139f82: itttt eq
  00139f84: ldr.eq r1,[sp,#0x10]
  00139f86: ldr.eq r1,[r1,#0x0]
  00139f88: ldrb.eq.w r1,[r1,#0x284]
  00139f8c: cmp.eq r1,#0x0
  00139f8e: beq 0x00139f9e
  00139f90: ldr r0,[r0,#0x4]
  00139f92: mov r1,r8
  00139f94: mov r2,r9
  00139f96: ldr.w r0,[r0,r5,lsl #0x2]
  00139f9a: blx 0x00076624
  00139f9e: adds r5,#0x1
  00139fa0: ldr.w r0,[r10,#0xec]
  00139fa4: ldr r1,[r0,#0x0]
  00139fa6: cmp r5,r1
  00139fa8: bcc 0x00139f80
  00139faa: b 0x0013a2b6
  00139fac: movs r0,#0x0
  00139fae: strh.w r0,[r10,#0x108]
  00139fb2: ldr.w r1,[r10,#0x19c]
  00139fb6: ldr r0,[r6,#0x28]
  00139fb8: adds r2,r0,r1
  00139fba: cmp r2,r8
  00139fbc: bge 0x0013a006
  00139fbe: ldr.w r3,[r10,#0x158]
  00139fc2: add r3,r2
  00139fc4: cmp r3,r8
  00139fc6: ble 0x0013a006
  00139fc8: ldr r3,[r6,#0xc]
  00139fca: adds r5,r3,r0
  00139fcc: cmp r5,r9
  00139fce: bge 0x0013a006
  00139fd0: ldr r5,[r6,#0x20]
  00139fd2: ldr.w r12,[r10,#0x154]
  00139fd6: add r3,r5
  00139fd8: add r3,r12
  00139fda: cmp r3,r9
  00139fdc: ble 0x0013a006
  00139fde: ldr r0,[0x0013a334]
  00139fe0: movs r1,#0x1
  00139fe2: strb.w r1,[r10,#0x108]
  00139fe6: movs r1,#0x0
  00139fe8: add r0,pc
  00139fea: str r1,[sp,#0x0]
  00139fec: movs r1,#0x7c
  00139fee: movs r2,#0x0
  00139ff0: ldr r0,[r0,#0x0]
  00139ff2: movs r3,#0x0
  00139ff4: ldr r0,[r0,#0x0]
  00139ff6: blx 0x00071548
  00139ffa: ldr r0,[sp,#0x10]
  00139ffc: ldr.w r1,[r10,#0x19c]
  0013a000: ldr r6,[r0,#0x0]
  0013a002: ldr r0,[r6,#0x28]
  0013a004: adds r2,r0,r1
  0013a006: ldr.w r3,[r10,#0x158]
  0013a00a: add r2,r3
  0013a00c: cmp r2,r8
  0013a00e: bge 0x0013a04e
  0013a010: ldr.w r2,[r10,#0x1a0]
  0013a014: subs r1,r1,r0
  0013a016: add r1,r2
  0013a018: cmp r1,r8
  0013a01a: ble 0x0013a04e
  0013a01c: ldr r1,[r6,#0xc]
  0013a01e: add r0,r1
  0013a020: cmp r0,r9
  0013a022: bge 0x0013a04e
  0013a024: ldr r0,[r6,#0x20]
  0013a026: ldr.w r2,[r10,#0x154]
  0013a02a: add r0,r1
  0013a02c: add r0,r2
  0013a02e: cmp r0,r9
  0013a030: ble 0x0013a04e
  0013a032: ldr r0,[0x0013a338]
  0013a034: movs r1,#0x1
  0013a036: strb.w r1,[r10,#0x109]
  0013a03a: movs r1,#0x0
  0013a03c: add r0,pc
  0013a03e: str r1,[sp,#0x0]
  0013a040: movs r1,#0x7c
  0013a042: movs r2,#0x0
  0013a044: ldr r0,[r0,#0x0]
  0013a046: movs r3,#0x0
  0013a048: ldr r0,[r0,#0x0]
  0013a04a: blx 0x00071548
  0013a04e: ldr.w r0,[r10,#0xcc]
  0013a052: mov r1,r8
  0013a054: mov r2,r9
  0013a056: blx 0x00074734
  0013a05a: ldr.w r0,[r10,#0xd0]
  0013a05e: mov r1,r8
  0013a060: mov r2,r9
  0013a062: blx 0x00074734
  0013a066: ldr.w r0,[r10,#0xec]
  0013a06a: mov r1,r8
  0013a06c: mov r2,r9
  0013a06e: ldr r0,[r0,#0x4]
  0013a070: ldr r0,[r0,#0x0]
  0013a072: blx 0x00076624
  0013a076: b 0x0013a2b6
  0013a078: ldr.w r0,[r10,#0xfc]
  0013a07c: mov r1,r8
  0013a07e: mov r2,r9
  0013a080: blx 0x000750ac
  0013a084: b 0x0013a2b6
  0013a086: ldr r0,[0x0013a320]
  0013a088: add r0,pc
  0013a08a: ldr r0,[r0,#0x0]
  0013a08c: ldrb r0,[r0,#0x0]
  0013a08e: cmp r0,#0x0
  0013a090: beq.w 0x0013a2b6
  0013a094: ldr r0,[0x0013a324]
  0013a096: add r0,pc
  0013a098: ldr r0,[r0,#0x0]
  0013a09a: ldrb r0,[r0,#0x0]
  0013a09c: cmp r0,#0x0
  0013a09e: bne.w 0x0013a2b6
  0013a0a2: ldr.w r0,[r10,#0x14]
  0013a0a6: mov r1,r8
  0013a0a8: mov r2,r9
  0013a0aa: blx 0x00074734
  0013a0ae: ldr.w r0,[r10,#0x18]
  0013a0b2: mov r1,r8
  0013a0b4: mov r2,r9
  0013a0b6: blx 0x00074734
  0013a0ba: ldrd r1,r0,[r10,#0x8]
  0013a0be: cmp r5,#0x0
  0013a0c0: beq.w 0x0013a288
  0013a0c4: cmp r1,#0x0
  0013a0c6: bne.w 0x0013a288
  0013a0ca: cmp.w r8,#0xd1
  0013a0ce: bgt.w 0x0013a288
  0013a0d2: cmp r0,r5
  0013a0d4: beq.w 0x0013a288
  0013a0d8: ldr r2,[0x0013a328]
  0013a0da: add r2,pc
  0013a0dc: ldr r2,[r2,#0x0]
  0013a0de: ldr r2,[r2,#0x54]
  0013a0e0: sub.w r3,r2,#0x14
  0013a0e4: cmp r3,r9
  0013a0e6: bge.w 0x0013a288
  0013a0ea: add.w r2,r2,#0x12c
  0013a0ee: cmp r2,r9
  0013a0f0: ble.w 0x0013a288
  0013a0f4: movs r0,#0x1
  0013a0f6: str.w r5,[r10,#0x8]
  0013a0fa: strh.w r0,[r10,#0x98]
  0013a0fe: str.w r9,[r10,#0x90]
  0013a102: b 0x0013a2b6
  0013a104: movs r5,#0x0
  0013a106: b 0x0013a118
  0013a108: ldr r0,[r0,#0x4]
  0013a10a: mov r1,r8
  0013a10c: mov r2,r9
  0013a10e: ldr.w r0,[r0,r5,lsl #0x2]
  0013a112: blx 0x00074734
  0013a116: adds r5,#0x1
  0013a118: ldr.w r0,[r10,#0xb4]
  0013a11c: ldr r1,[r0,#0x0]
  0013a11e: cmp r5,r1
  0013a120: bcc 0x0013a108
  0013a122: b 0x0013a2b6
  0013a124: movs r5,#0x0
  0013a126: b 0x0013a138
  0013a128: ldr r0,[r0,#0x4]
  0013a12a: mov r1,r8
  0013a12c: mov r2,r9
  0013a12e: ldr.w r0,[r0,r5,lsl #0x2]
  0013a132: blx 0x00074734
  0013a136: adds r5,#0x1
  0013a138: ldr.w r0,[r10,#0xb0]
  0013a13c: ldr r1,[r0,#0x0]
  0013a13e: cmp r5,r1
  0013a140: bcc 0x0013a128
  0013a142: b 0x0013a2b6
  0013a144: add.w r5,r10,#0xc0
  0013a148: b 0x0013a182
  0013a14a: add.w r5,r10,#0xc0
  0013a14e: mov.w r11,#0x0
  0013a152: movs r6,#0x0
  0013a154: b 0x0013a17a
  0013a156: ldr r0,[r0,#0x4]
  0013a158: ldr.w r0,[r0,r6,lsl #0x2]
  0013a15c: ldrd r1,r2,[r0,#0x0]
  0013a160: subs r1,#0x65
  0013a162: sbc r2,r2,#0x0
  0013a166: rsbs.w r1,r1,#0x4
  0013a16a: sbcs.w r1,r11,r2
  0013a16e: bcc 0x0013a178
  0013a170: mov r1,r8
  0013a172: mov r2,r9
  0013a174: blx 0x00074734
  0013a178: adds r6,#0x1
  0013a17a: ldr r0,[r5,#0x0]
  0013a17c: ldr r1,[r0,#0x0]
  0013a17e: cmp r6,r1
  0013a180: bcc 0x0013a156
  0013a182: ldr.w r0,[r10,#0xf4]
  0013a186: mov r1,r8
  0013a188: mov r2,r9
  0013a18a: blx 0x00075ba4
  0013a18e: movs r6,#0x0
  0013a190: b 0x0013a1b2
  0013a192: ldr r0,[r0,#0x4]
  0013a194: ldr.w r0,[r0,r6,lsl #0x2]
  0013a198: ldrd r1,r2,[r0,#0x0]
  0013a19c: orr r1,r1,#0x8
  0013a1a0: eor r1,r1,#0x3c
  0013a1a4: orrs r1,r2
  0013a1a6: ittt eq
  0013a1a8: mov.eq r1,r8
  0013a1aa: mov.eq r2,r9
  0013a1ac: blx.eq 0x00074734
  0013a1b0: adds r6,#0x1
  0013a1b2: ldr r0,[r5,#0x0]
  0013a1b4: ldr r1,[r0,#0x0]
  0013a1b6: cmp r6,r1
  0013a1b8: bcc 0x0013a192
  0013a1ba: movs r5,#0x0
  0013a1bc: ldr r4,[sp,#0x10]
  0013a1be: b 0x0013a1d0
  0013a1c0: ldr r0,[r0,#0x4]
  0013a1c2: mov r1,r8
  0013a1c4: mov r2,r9
  0013a1c6: ldr.w r0,[r0,r5,lsl #0x2]
  0013a1ca: blx 0x00074734
  0013a1ce: adds r5,#0x1
  0013a1d0: ldr.w r0,[r10,#0xf8]
  0013a1d4: ldr r1,[r0,#0x0]
  0013a1d6: cmp r5,r1
  0013a1d8: bcc 0x0013a1c0
  0013a1da: ldr r0,[0x0013a318]
  0013a1dc: ldr r1,[0x0013a31c]
  0013a1de: add r0,pc
  0013a1e0: ldr.w r3,[r10,#0x138]
  0013a1e4: add r1,pc
  0013a1e6: ldr.w r6,[r10,#0x1e0]
  0013a1ea: ldr r0,[r0,#0x0]
  0013a1ec: ldr.w r11,[r1,#0x0]
  0013a1f0: ldr r2,[r4,#0x0]
  0013a1f2: ldr.w r1,[r3,r6,lsl #0x2]
  0013a1f6: ldr r5,[r0,#0x0]
  0013a1f8: ldr.w r0,[r11,#0x0]
  0013a1fc: ldr r2,[r2,#0x28]
  0013a1fe: subs r5,r5,r2
  0013a200: blx 0x00072d84
  0013a204: subs r0,r5,r0
  0013a206: cmp r0,r8
  0013a208: bge 0x0013a232
  0013a20a: ldr.w r0,[r10,#0x138]
  0013a20e: ldr.w r1,[r10,#0x1e0]
  0013a212: ldr r2,[sp,#0x10]
  0013a214: ldr.w r1,[r0,r1,lsl #0x2]
  0013a218: ldr r2,[r2,#0x0]
  0013a21a: ldr.w r0,[r11,#0x0]
  0013a21e: ldr r5,[r2,#0xc]
  0013a220: ldr r6,[r2,#0x20]
  0013a222: blx 0x00072d90
  0013a226: adds r1,r6,r5
  0013a228: add r0,r1
  0013a22a: cmp r0,r9
  0013a22c: ble 0x0013a232
  0013a22e: movs r0,#0x1
  0013a230: b 0x0013a234
  0013a232: movs r0,#0x0
  0013a234: ldr.w r1,[r10,#0x134]
  0013a238: strb.w r0,[r10,#0x1d9]
  0013a23c: ldr r0,[sp,#0x10]
  0013a23e: ldr r1,[r1,#0x0]
  0013a240: ldr r2,[r0,#0x0]
  0013a242: ldr.w r0,[r11,#0x0]
  0013a246: ldr r5,[r2,#0x28]
  0013a248: blx 0x00072d84
  0013a24c: add r0,r5
  0013a24e: cmp r0,r8
  0013a250: ble 0x0013a2b6
  0013a252: movs r0,#0x0
  0013a254: str.w r9,[r10,#0x220]
  0013a258: str.w r9,[r10,#0x20c]
  0013a25c: str.w r0,[r10,#0x214]
  0013a260: movs r0,#0x1
  0013a262: strb.w r0,[r10,#0x224]
  0013a266: b 0x0013a2b6
  0013a268: movs r5,#0x0
  0013a26a: b 0x0013a27c
  0013a26c: ldr r0,[r0,#0x4]
  0013a26e: mov r1,r8
  0013a270: mov r2,r9
  0013a272: ldr.w r0,[r0,r5,lsl #0x2]
  0013a276: blx 0x00074734
  0013a27a: adds r5,#0x1
  0013a27c: ldr.w r0,[r10,#0xb8]
  0013a280: ldr r1,[r0,#0x0]
  0013a282: cmp r5,r1
  0013a284: bcc 0x0013a26c
  0013a286: b 0x0013a2b6
  0013a288: cmp r1,r5
  0013a28a: it ne
  0013a28c: cmp.ne r5,#0x0
  0013a28e: bne 0x0013a2dc
  0013a290: movs r0,#0x0
  0013a292: strh.w r0,[r10,#0x98]
  0013a296: b 0x0013a2b6
  0013a298: movs r5,#0x0
  0013a29a: b 0x0013a2ac
  0013a29c: ldr r0,[r0,#0x4]
  0013a29e: mov r1,r8
  0013a2a0: mov r2,r9
  0013a2a2: ldr.w r0,[r0,r5,lsl #0x2]
  0013a2a6: blx 0x00074734
  0013a2aa: adds r5,#0x1
  0013a2ac: ldr.w r0,[r10,#0xac]
  0013a2b0: ldr r1,[r0,#0x0]
  0013a2b2: cmp r5,r1
  0013a2b4: bcc 0x0013a29c
  0013a2b6: ldr r0,[sp,#0x10]
  0013a2b8: mov r1,r8
  0013a2ba: mov r2,r9
  0013a2bc: ldr r0,[r0,#0x0]
  0013a2be: blx 0x00074728
  0013a2c2: cbz r0,0x0013a2d2
  0013a2c4: ldr.w r0,[r10,#0x16c]
  0013a2c8: cmp r0,#0xd
  0013a2ca: itt eq
  0013a2cc: mov.eq r0,#0x1
  0013a2ce: strb.eq.w r0,[r10,#0x1]
  0013a2d2: movs r0,#0x0
  0013a2d4: add sp,#0x14
  0013a2d6: pop.w {r8,r9,r10,r11}
  0013a2da: pop {r4,r5,r6,r7,pc}
  0013a2dc: cmp r0,#0x0
  0013a2de: bne 0x0013a290
  0013a2e0: ldr r0,[0x0013a32c]
  0013a2e2: add r0,pc
  0013a2e4: ldr r0,[r0,#0x0]
  0013a2e6: ldr r0,[r0,#0x0]
  0013a2e8: subs r0,#0xdc
  0013a2ea: cmp r0,r8
  0013a2ec: bge 0x0013a290
  0013a2ee: ldr r0,[0x0013a330]
  0013a2f0: add r0,pc
  0013a2f2: ldr r0,[r0,#0x0]
  0013a2f4: ldr r0,[r0,#0x58]
  0013a2f6: sub.w r1,r0,#0x14
  0013a2fa: cmp r1,r9
  0013a2fc: bge 0x0013a290
  0013a2fe: adds r0,#0xe6
  0013a300: cmp r0,r9
  0013a302: ble 0x0013a290
  0013a304: mov.w r0,#0x100
  0013a308: str.w r5,[r10,#0xc]
  0013a30c: strh.w r0,[r10,#0x98]
  0013a310: str.w r9,[r10,#0x94]
  0013a314: b 0x0013a2b6
```
