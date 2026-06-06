# Level::createRadioMessage
elf 0xc566c body 1420
Sig: undefined __stdcall createRadioMessage(int param_1, int param_2)

## decompile
```c

/* Level::createRadioMessage(int, int) */

void Level::createRadioMessage(int param_1,int param_2)

{
  int iVar1;
  void *pvVar2;
  Array<RadioMessage*> *this;
  Station *pSVar3;
  RadioMessage *pRVar4;
  uint *puVar5;
  undefined4 uVar6;
  uint in_r2;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  int local_28;
  
  if ((*(int *)(param_1 + 0xf0) == 0) || (*(int *)(*(int *)(param_1 + 0xf0) + 0x18) == 0)) {
    return;
  }
  piVar13 = *(int **)(DAT_000d59dc + 0xd568a);
  Status::getMission();
  iVar1 = Mission::isEmpty();
  if (iVar1 == 0) {
    return;
  }
  if (*(Array<RadioMessage*> **)(param_1 + 0x114) != (Array<RadioMessage*> *)0x0) {
    pvVar2 = (void *)Array<RadioMessage*>::~Array(*(Array<RadioMessage*> **)(param_1 + 0x114));
    operator_delete(pvVar2);
    *(undefined4 *)(param_1 + 0x114) = 0;
  }
  this = operator_new(0xc);
  Array<RadioMessage*>::Array(this);
  *(Array<RadioMessage*> **)(param_1 + 0x114) = this;
  iVar7 = DAT_000d59f0;
  iVar1 = DAT_000d59ec;
  if (in_r2 == 0) {
    local_28 = 0x40;
  }
  else if (in_r2 == 2) {
    local_28 = 0x41;
  }
  else if (in_r2 == 3) {
    local_28 = 0x15;
  }
  else {
    local_28 = 0x3f;
    if (in_r2 == 8) {
      local_28 = 9;
    }
  }
  iVar11 = 0;
  uVar10 = 0x1ba;
  switch(param_2) {
  case 0:
    puVar5 = *(uint **)(*piVar13 + 0x90);
    if (puVar5 != (uint *)0x0) {
      for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
        iVar7 = *(int *)(puVar5[1] + uVar10 * 4);
        pSVar3 = (Station *)Status::getStation();
        iVar1 = Station::getIndex(pSVar3);
        if (iVar7 == iVar1) goto LAB_000d5c10;
        puVar5 = *(uint **)(*piVar13 + 0x90);
      }
    }
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d59e0 + 0xd5776));
    uVar10 = iVar1 + 0x1aa;
    break;
  case 1:
    puVar5 = *(uint **)(*piVar13 + 0x90);
    if (puVar5 != (uint *)0x0) {
      for (uVar10 = 0; uVar10 < *puVar5; uVar10 = uVar10 + 1) {
        iVar7 = *(int *)(puVar5[1] + uVar10 * 4);
        pSVar3 = (Station *)Status::getStation();
        iVar1 = Station::getIndex(pSVar3);
        if (iVar7 == iVar1) goto LAB_000d5c10;
        puVar5 = *(uint **)(*piVar13 + 0x90);
      }
    }
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d59e8 + 0xd5848));
    uVar10 = iVar1 + 0x1ad;
    break;
  default:
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d5ca0 + 0xd5c04));
    uVar10 = iVar1 + 0x1bd;
    break;
  case 3:
    *(undefined1 *)(param_1 + 0x1b0) = 1;
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(iVar1 + 0xd5862));
    uVar10 = iVar1 + 0x1b3;
    break;
  case 4:
    *(undefined1 *)(param_1 + 0x68) = 1;
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(iVar7 + 0xd587c));
    uVar10 = iVar1 + 0x1b6;
    break;
  case 5:
    uVar10 = 0x1bb;
    goto LAB_000d5b66;
  case 6:
    uVar10 = 0x1bc;
    goto LAB_000d5b66;
  case 7:
    goto switchD_000d56fe_caseD_7;
  case 8:
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d59f4 + 0xd589c));
    iVar7 = 0;
    for (iVar11 = 0; iVar11 < iVar1; iVar11 = iVar11 + 1) {
      iVar7 = iVar7 + *(int *)(DAT_000d59f8 + 0xd58ac + iVar11 * 4);
    }
    iVar1 = *(int *)(DAT_000d59f8 + 0xd58ac + iVar1 * 4);
    iVar7 = DAT_000d59fc + 0xd58c4 + iVar7 * 8;
    for (iVar11 = 0; iVar11 < iVar1; iVar11 = iVar11 + 1) {
      iVar12 = iVar11 + -1;
      iVar8 = *(int *)(iVar7 + iVar11 * 8);
      if (iVar11 == 0) {
        iVar12 = 5000;
      }
      pRVar4 = operator_new(0x28);
      iVar9 = 6;
      if (iVar11 == 0) {
        iVar9 = 5;
      }
      RadioMessage::RadioMessage(pRVar4,*(int *)(iVar7 + iVar11 * 8 + 4),iVar8,iVar9,iVar12);
      ArrayAdd<RadioMessage*>(pRVar4,*(Array **)(param_1 + 0x114));
    }
    goto LAB_000d5b8e;
  case 9:
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d5c90 + 0xd5920));
    uVar10 = iVar1 + 0x1c1;
    break;
  case 10:
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d5c94 + 0xd5934));
    uVar10 = iVar1 + 0x1c3;
    break;
  case 0xb:
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d5c98 + 0xd5948));
    uVar10 = iVar1 + 0x1c5;
    break;
  case 0xc:
    iVar11 = 0;
    uVar10 = 0x1c5;
    goto switchD_000d56fe_caseD_7;
  case 0xd:
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d5c9c + 0xd5966));
    uVar10 = iVar1 + 0x1c7;
    break;
  case 0xe:
    piVar13 = *(int **)(*piVar13 + 0x90);
    if (piVar13 != (int *)0x0) {
      iVar1 = 0;
      uVar10 = 0x88f;
      while (*piVar13 != iVar1) {
        iVar7 = iVar1 * 4;
        iVar1 = iVar1 + 1;
        if (-1 < *(int *)(piVar13[1] + iVar7)) {
          uVar10 = uVar10 - 2;
        }
      }
      break;
    }
    iVar11 = 0;
    uVar10 = 0x88f;
    goto switchD_000d56fe_caseD_7;
  case 0xf:
    piVar13 = *(int **)(*piVar13 + 0x90);
    if (piVar13 != (int *)0x0) {
      iVar1 = 0;
      uVar10 = 0x88e;
      while (*piVar13 != iVar1) {
        iVar7 = iVar1 * 4;
        iVar1 = iVar1 + 1;
        if (-1 < *(int *)(piVar13[1] + iVar7)) {
          uVar10 = uVar10 - 2;
        }
      }
      if (uVar10 - 0x889 < 5) goto LAB_000d5b66;
    }
LAB_000d5c16:
    pvVar2 = (void *)Array<RadioMessage*>::~Array(this);
    operator_delete(pvVar2);
    goto LAB_000d5c1e;
  case 0x10:
    iVar1 = 0;
    for (iVar7 = 0; iVar7 < (int)in_r2; iVar7 = iVar7 + 1) {
      iVar1 = iVar1 + *(int *)(DAT_000d5c74 + 0xd59b2 + iVar7 * 4);
    }
    uVar10 = iVar1 << 1;
    iVar1 = *(int *)(DAT_000d5c74 + 0xd59b2 + in_r2 * 4);
    iVar7 = DAT_000d5c78 + 0xd59d4;
    for (iVar11 = 0; iVar11 < iVar1; iVar11 = iVar11 + 1) {
      iVar12 = iVar11 + -1;
      if (iVar11 == 0) {
        iVar12 = 5000;
      }
      pRVar4 = operator_new(0x28);
      iVar9 = 6;
      iVar8 = *(int *)(iVar7 + uVar10 * 4);
      if ((0x111U >> (uVar10 & 0xff) & 1) != 0) {
        iVar8 = iVar8 + in_r2;
      }
      if (iVar11 == 0) {
        iVar9 = 5;
      }
      RadioMessage::RadioMessage(pRVar4,*(int *)(iVar7 + uVar10 * 4 + 4),iVar8,iVar9,iVar12);
      ArrayAdd<RadioMessage*>(pRVar4,*(Array **)(param_1 + 0x114));
      uVar10 = uVar10 + 2;
    }
    if ((in_r2 | 4) == 6) {
      iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d5c7c + 0xd5a70));
      uVar10 = iVar1 + 0xc4a;
      local_28 = in_r2 + 10000;
      break;
    }
    goto LAB_000d5b8e;
  case 0x11:
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d5c80 + 0xd5a8c));
    uVar10 = iVar1 + 0xc4f;
    local_28 = 0;
    iVar11 = *(int *)(*(int *)(*(int *)(param_1 + 0xf0) + 0x10) + 8) + 6000;
    goto switchD_000d56fe_caseD_7;
  case 0x12:
    iVar1 = 0;
    for (iVar7 = 0; iVar7 < (int)in_r2; iVar7 = iVar7 + 1) {
      iVar1 = iVar1 + *(int *)(DAT_000d5c84 + 0xd5ab6 + iVar7 * 4);
    }
    uVar10 = iVar1 << 1;
    iVar1 = *(int *)(DAT_000d5c84 + 0xd5ab6 + in_r2 * 4);
    iVar7 = DAT_000d5c88 + 0xd5ad8;
    for (iVar11 = 0; iVar11 < iVar1; iVar11 = iVar11 + 1) {
      iVar12 = iVar11 + -1;
      if (iVar11 == 0) {
        iVar12 = 5000;
      }
      pRVar4 = operator_new(0x28);
      iVar9 = 6;
      iVar8 = *(int *)(iVar7 + uVar10 * 4);
      if ((uVar10 | 4) == 6) {
        iVar8 = iVar8 + in_r2;
      }
      if (iVar11 == 0) {
        iVar9 = 5;
      }
      RadioMessage::RadioMessage(pRVar4,*(int *)(iVar7 + uVar10 * 4 + 4),iVar8,iVar9,iVar12);
      ArrayAdd<RadioMessage*>(pRVar4,*(Array **)(param_1 + 0x114));
      uVar10 = uVar10 + 2;
    }
    if (in_r2 < 2) {
      iVar1 = *(int *)(param_1 + 0xf0);
      uVar6 = *(undefined4 *)(param_1 + 0x114);
      goto LAB_000d5b96;
    }
    iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000d5c8c + 0xd5c3e));
    uVar10 = iVar1 + 0xc45;
    local_28 = 0;
    iVar11 = 0;
    goto switchD_000d56fe_caseD_7;
  case 0x13:
    pRVar4 = operator_new(0x28);
    piVar13 = *(int **)(DAT_000d59e4 + 0xd57be);
    iVar1 = AbyssEngine::AERandom::nextInt(*piVar13);
    RadioMessage::RadioMessage(pRVar4,iVar1 + 0xaf4,0,5,0x5dc);
    ArrayAdd<RadioMessage*>(pRVar4,*(Array **)(param_1 + 0x114));
    pRVar4 = operator_new(0x28);
    iVar1 = AbyssEngine::AERandom::nextInt(*piVar13);
    RadioMessage::RadioMessage(pRVar4,iVar1 + 0xafa,0,6,0);
    goto LAB_000d5b84;
  case 0x15:
    uVar10 = 0xc54;
    goto LAB_000d5b66;
  case 0x16:
    uVar10 = 0xc55;
    goto LAB_000d5b66;
  case 0x17:
    uVar10 = 0xc56;
    goto LAB_000d5b66;
  case 0x18:
    uVar10 = 0xc57;
    goto LAB_000d5b66;
  case 0x19:
    uVar10 = 0xc58;
    goto LAB_000d5b66;
  case 0x1a:
    uVar10 = 0xc59;
LAB_000d5b66:
    local_28 = 0;
    break;
  case 0x1b:
    pRVar4 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar4,in_r2 * 2 + 0xc60,6,5,0x5dc);
    ArrayAdd<RadioMessage*>(pRVar4,*(Array **)(param_1 + 0x114));
    pRVar4 = operator_new(0x28);
    RadioMessage::RadioMessage(pRVar4,in_r2 * 2 + 0xc61,0,6,0);
    goto LAB_000d5b84;
  case 0x1c:
    iVar11 = 0;
    local_28 = 0x26;
    uVar10 = in_r2;
    goto switchD_000d56fe_caseD_7;
  }
  iVar11 = 0;
switchD_000d56fe_caseD_7:
  pRVar4 = operator_new(0x28);
  RadioMessage::RadioMessage(pRVar4,uVar10,local_28,5,iVar11);
LAB_000d5b84:
  ArrayAdd<RadioMessage*>(pRVar4,*(Array **)(param_1 + 0x114));
LAB_000d5b8e:
  iVar1 = *(int *)(param_1 + 0xf0);
  uVar6 = *(undefined4 *)(param_1 + 0x114);
LAB_000d5b96:
  (*(code *)(DAT_001ac004 + 0x1ac008))(*(undefined4 *)(iVar1 + 0x18),uVar6);
  return;
LAB_000d5c10:
  this = *(Array<RadioMessage*> **)(param_1 + 0x114);
  if (this != (Array<RadioMessage*> *)0x0) goto LAB_000d5c16;
LAB_000d5c1e:
  iVar1 = *(int *)(param_1 + 0xf0);
  *(undefined4 *)(param_1 + 0x114) = 0;
  uVar6 = 0;
  goto LAB_000d5b96;
}

```
## target disasm
```
  000d566c: push {r4,r5,r6,r7,lr}
  000d566e: add r7,sp,#0xc
  000d5670: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000d5674: mov r9,r0
  000d5676: ldr.w r0,[r0,#0xf0]
  000d567a: mov r10,r2
  000d567c: mov r4,r1
  000d567e: cbz r0,0x000d56d4
  000d5680: ldr r0,[r0,#0x18]
  000d5682: cbz r0,0x000d56d4
  000d5684: ldr r0,[0x000d59dc]
  000d5686: add r0,pc
  000d5688: ldr.w r11,[r0,#0x0]
  000d568c: ldr.w r0,[r11,#0x0]
  000d5690: blx 0x00072850
  000d5694: blx 0x00072868
  000d5698: cbz r0,0x000d56d4
  000d569a: ldr.w r0,[r9,#0x114]
  000d569e: cbz r0,0x000d56ae
  000d56a0: blx 0x00073bac
  000d56a4: blx 0x0006eb48
  000d56a8: movs r0,#0x0
  000d56aa: str.w r0,[r9,#0x114]
  000d56ae: movs r0,#0xc
  000d56b0: blx 0x0006eb24
  000d56b4: mov r5,r0
  000d56b6: blx 0x00074380
  000d56ba: cmp.w r10,#0x0
  000d56be: str.w r5,[r9,#0x114]
  000d56c2: beq 0x000d56dc
  000d56c4: cmp.w r10,#0x2
  000d56c8: beq 0x000d56e0
  000d56ca: cmp.w r10,#0x3
  000d56ce: bne 0x000d56e4
  000d56d0: movs r0,#0x15
  000d56d2: b 0x000d56ee
  000d56d4: add sp,#0x14
  000d56d6: pop.w {r8,r9,r10,r11}
  000d56da: pop {r4,r5,r6,r7,pc}
  000d56dc: movs r0,#0x40
  000d56de: b 0x000d56ee
  000d56e0: movs r0,#0x41
  000d56e2: b 0x000d56ee
  000d56e4: movs r0,#0x3f
  000d56e6: cmp.w r10,#0x8
  000d56ea: it eq
  000d56ec: mov.eq r0,#0x9
  000d56ee: cmp r4,#0x1c
  000d56f0: str r0,[sp,#0x10]
  000d56f2: bhi.w 0x000d5bfc
  000d56f6: mov.w r8,#0x0
  000d56fa: mov.w r6,#0x1ba
  000d56fe: tbh [pc,r4]
  000d573c: ldr.w r0,[r11,#0x0]
  000d5740: ldr.w r1,[r0,#0x90]
  000d5744: cbz r1,0x000d576e
  000d5746: movs r4,#0x0
  000d5748: b 0x000d5768
  000d574a: ldr r1,[r1,#0x4]
  000d574c: ldr.w r5,[r1,r4,lsl #0x2]
  000d5750: blx 0x00071c14
  000d5754: blx 0x00071824
  000d5758: cmp r5,r0
  000d575a: beq.w 0x000d5c10
  000d575e: ldr.w r0,[r11,#0x0]
  000d5762: adds r4,#0x1
  000d5764: ldr.w r1,[r0,#0x90]
  000d5768: ldr r2,[r1,#0x0]
  000d576a: cmp r4,r2
  000d576c: bcc 0x000d574a
  000d576e: ldr r0,[0x000d59e0]
  000d5770: movs r1,#0x3
  000d5772: add r0,pc
  000d5774: ldr r0,[r0,#0x0]
  000d5776: ldr r0,[r0,#0x0]
  000d5778: blx 0x00071848
  000d577c: add.w r6,r0,#0x1aa
  000d5780: b 0x000d5b6a
  000d5782: ldr.w r0,[r11,#0x0]
  000d5786: ldr.w r0,[r0,#0x90]
  000d578a: cmp r0,#0x0
  000d578c: beq.w 0x000d5c2c
  000d5790: ldr r2,[r0,#0x0]
  000d5792: movs r1,#0x0
  000d5794: movw r6,#0x88f
  000d5798: b 0x000d57aa
  000d579a: ldr r3,[r0,#0x4]
  000d579c: ldr.w r3,[r3,r1,lsl #0x2]
  000d57a0: adds r1,#0x1
  000d57a2: cmp.w r3,#0xffffffff
  000d57a6: it gt
  000d57a8: sub.gt r6,#0x2
  000d57aa: cmp r2,r1
  000d57ac: bne 0x000d579a
  000d57ae: b 0x000d5b6a
  000d57b0: movs r0,#0x28
  000d57b2: blx 0x0006eb24
  000d57b6: mov r5,r0
  000d57b8: ldr r0,[0x000d59e4]
  000d57ba: add r0,pc
  000d57bc: ldr r4,[r0,#0x0]
  000d57be: ldr r0,[r4,#0x0]
  000d57c0: movs r1,#0x4
  000d57c2: blx 0x00071848
  000d57c6: movw r1,#0x5dc
  000d57ca: movs r2,#0x0
  000d57cc: str r1,[sp,#0x0]
  000d57ce: addw r1,r0,#0xaf4
  000d57d2: mov r0,r5
  000d57d4: movs r3,#0x5
  000d57d6: blx 0x00074398
  000d57da: ldr.w r1,[r9,#0x114]
  000d57de: mov r0,r5
  000d57e0: blx 0x0007447c
  000d57e4: movs r0,#0x28
  000d57e6: blx 0x0006eb24
  000d57ea: mov r5,r0
  000d57ec: ldr r0,[r4,#0x0]
  000d57ee: movs r1,#0x4
  000d57f0: blx 0x00071848
  000d57f4: movs r1,#0x0
  000d57f6: movs r2,#0x0
  000d57f8: str r1,[sp,#0x0]
  000d57fa: addw r1,r0,#0xafa
  000d57fe: mov r0,r5
  000d5800: movs r3,#0x6
  000d5802: blx 0x00074398
  000d5806: b 0x000d5b84
  000d5808: movw r6,#0xc56
  000d580c: b 0x000d5b66
  000d580e: ldr.w r0,[r11,#0x0]
  000d5812: ldr.w r1,[r0,#0x90]
  000d5816: cbz r1,0x000d5840
  000d5818: movs r4,#0x0
  000d581a: b 0x000d583a
  000d581c: ldr r1,[r1,#0x4]
  000d581e: ldr.w r5,[r1,r4,lsl #0x2]
  000d5822: blx 0x00071c14
  000d5826: blx 0x00071824
  000d582a: cmp r5,r0
  000d582c: beq.w 0x000d5c10
  000d5830: ldr.w r0,[r11,#0x0]
  000d5834: adds r4,#0x1
  000d5836: ldr.w r1,[r0,#0x90]
  000d583a: ldr r2,[r1,#0x0]
  000d583c: cmp r4,r2
  000d583e: bcc 0x000d581c
  000d5840: ldr r0,[0x000d59e8]
  000d5842: movs r1,#0x3
  000d5844: add r0,pc
  000d5846: ldr r0,[r0,#0x0]
  000d5848: ldr r0,[r0,#0x0]
  000d584a: blx 0x00071848
  000d584e: addw r6,r0,#0x1ad
  000d5852: b 0x000d5b6a
  000d5854: ldr r0,[0x000d59ec]
  000d5856: movs r1,#0x1
  000d5858: strb.w r1,[r9,#0x1b0]
  000d585c: movs r1,#0x3
  000d585e: add r0,pc
  000d5860: ldr r0,[r0,#0x0]
  000d5862: ldr r0,[r0,#0x0]
  000d5864: blx 0x00071848
  000d5868: addw r6,r0,#0x1b3
  000d586c: b 0x000d5b6a
  000d586e: ldr r0,[0x000d59f0]
  000d5870: movs r1,#0x1
  000d5872: strb.w r1,[r9,#0x68]
  000d5876: movs r1,#0x3
  000d5878: add r0,pc
  000d587a: ldr r0,[r0,#0x0]
  000d587c: ldr r0,[r0,#0x0]
  000d587e: blx 0x00071848
  000d5882: add.w r6,r0,#0x1b6
  000d5886: b 0x000d5b6a
  000d5888: movw r6,#0x1bb
  000d588c: b 0x000d5b66
  000d588e: mov.w r6,#0x1bc
  000d5892: b 0x000d5b66
  000d5894: ldr r0,[0x000d59f4]
  000d5896: movs r1,#0x8
  000d5898: add r0,pc
  000d589a: ldr r0,[r0,#0x0]
  000d589c: ldr r0,[r0,#0x0]
  000d589e: blx 0x00071848
  000d58a2: ldr r2,[0x000d59f8]
  000d58a4: movs r1,#0x0
  000d58a6: movs r3,#0x0
  000d58a8: add r2,pc
  000d58aa: b 0x000d58b4
  000d58ac: ldr.w r6,[r2,r3,lsl #0x2]
  000d58b0: adds r3,#0x1
  000d58b2: add r1,r6
  000d58b4: cmp r3,r0
  000d58b6: blt 0x000d58ac
  000d58b8: ldr r3,[0x000d59fc]
  000d58ba: movs r4,#0x0
  000d58bc: ldr.w r0,[r2,r0,lsl #0x2]
  000d58c0: add r3,pc
  000d58c2: add.w r1,r3,r1, lsl #0x3
  000d58c6: str r1,[sp,#0xc]
  000d58c8: str r0,[sp,#0x10]
  000d58ca: b 0x000d5910
  000d58cc: ldr r0,[sp,#0xc]
  000d58ce: sub.w r8,r4,#0x1
  000d58d2: cmp r4,#0x0
  000d58d4: mov r6,r8
  000d58d6: add.w r10,r0,r4, lsl #0x3
  000d58da: ldr.w r11,[r0,r4,lsl #0x3]
  000d58de: mov.w r0,#0x28
  000d58e2: it eq
  000d58e4: movw.eq r6,#0x1388
  000d58e8: blx 0x0006eb24
  000d58ec: ldr.w r1,[r10,#0x4]
  000d58f0: mov r5,r0
  000d58f2: movs r3,#0x6
  000d58f4: str r6,[sp,#0x0]
  000d58f6: cmp r4,#0x0
  000d58f8: it eq
  000d58fa: mov.eq r3,#0x5
  000d58fc: mov r2,r11
  000d58fe: blx 0x00074398
  000d5902: ldr.w r1,[r9,#0x114]
  000d5906: mov r0,r5
  000d5908: blx 0x0007447c
  000d590c: add.w r4,r8,#0x2
  000d5910: ldr r0,[sp,#0x10]
  000d5912: cmp r4,r0
  000d5914: blt 0x000d58cc
  000d5916: b 0x000d5b8e
  000d5918: ldr r0,[0x000d5c90]
  000d591a: movs r1,#0x2
  000d591c: add r0,pc
  000d591e: ldr r0,[r0,#0x0]
  000d5920: ldr r0,[r0,#0x0]
  000d5922: blx 0x00071848
  000d5926: addw r6,r0,#0x1c1
  000d592a: b 0x000d5b6a
  000d592c: ldr r0,[0x000d5c94]
  000d592e: movs r1,#0x2
  000d5930: add r0,pc
  000d5932: ldr r0,[r0,#0x0]
  000d5934: ldr r0,[r0,#0x0]
  000d5936: blx 0x00071848
  000d593a: addw r6,r0,#0x1c3
  000d593e: b 0x000d5b6a
  000d5940: ldr r0,[0x000d5c98]
  000d5942: movs r1,#0x2
  000d5944: add r0,pc
  000d5946: ldr r0,[r0,#0x0]
  000d5948: ldr r0,[r0,#0x0]
  000d594a: blx 0x00071848
  000d594e: addw r6,r0,#0x1c5
  000d5952: b 0x000d5b6a
  000d5954: mov.w r8,#0x0
  000d5958: movw r6,#0x1c5
  000d595c: b 0x000d5b6e
  000d595e: ldr r0,[0x000d5c9c]
  000d5960: movs r1,#0x2
  000d5962: add r0,pc
  000d5964: ldr r0,[r0,#0x0]
  000d5966: ldr r0,[r0,#0x0]
  000d5968: blx 0x00071848
  000d596c: addw r6,r0,#0x1c7
  000d5970: b 0x000d5b6a
  000d5972: ldr.w r0,[r11,#0x0]
  000d5976: ldr.w r0,[r0,#0x90]
  000d597a: cbz r0,0x000d59a4
  000d597c: ldr r2,[r0,#0x0]
  000d597e: movs r1,#0x0
  000d5980: movw r6,#0x88e
  000d5984: b 0x000d5996
  000d5986: ldr r3,[r0,#0x4]
  000d5988: ldr.w r3,[r3,r1,lsl #0x2]
  000d598c: adds r1,#0x1
  000d598e: cmp.w r3,#0xffffffff
  000d5992: it gt
  000d5994: sub.gt r6,#0x2
  000d5996: cmp r2,r1
  000d5998: bne 0x000d5986
  000d599a: subw r0,r6,#0x889
  000d599e: cmp r0,#0x5
  000d59a0: bcc.w 0x000d5b66
  000d59a4: mov r0,r5
  000d59a6: b 0x000d5c16
  000d59a8: ldr r1,[0x000d5c74]
  000d59aa: movs r0,#0x0
  000d59ac: movs r2,#0x0
  000d59ae: add r1,pc
  000d59b0: b 0x000d59ba
  000d59b2: ldr.w r3,[r1,r2,lsl #0x2]
  000d59b6: adds r2,#0x1
  000d59b8: add r0,r3
  000d59ba: cmp r2,r10
  000d59bc: blt 0x000d59b2
  000d59be: lsls r4,r0,#0x1
  000d59c0: ldr.w r0,[r1,r10,lsl #0x2]
  000d59c4: str.w r10,[sp,#0xc]
  000d59c8: str r0,[sp,#0x8]
  000d59ca: mov.w r10,#0x0
  000d59ce: ldr r0,[0x000d5c78]
  000d59d0: add r0,pc
  000d59d2: str r0,[sp,#0x4]
  000d59d4: ldr.w r11,[sp,#0x4]
  000d59d8: b 0x000d5a56
  000d5a00: sub.w r8,r10,#0x1
  000d5a04: add.w r0,r11,r4, lsl #0x2
  000d5a08: cmp.w r10,#0x0
  000d5a0c: mov r6,r8
  000d5a0e: it eq
  000d5a10: movw.eq r6,#0x1388
  000d5a14: str r0,[sp,#0x10]
  000d5a16: movs r0,#0x28
  000d5a18: blx 0x0006eb24
  000d5a1c: mov r5,r0
  000d5a1e: ldr r0,[sp,#0x10]
  000d5a20: movs r3,#0x6
  000d5a22: ldr.w r2,[r11,r4,lsl #0x2]
  000d5a26: str r6,[sp,#0x0]
  000d5a28: ldr r1,[r0,#0x4]
  000d5a2a: movw r0,#0x111
  000d5a2e: lsrs r0,r4
  000d5a30: lsls r0,r0,#0x1f
  000d5a32: ldr r0,[sp,#0xc]
  000d5a34: it ne
  000d5a36: add.ne r2,r0
  000d5a38: cmp.w r10,#0x0
  000d5a3c: it eq
  000d5a3e: mov.eq r3,#0x5
  000d5a40: mov r0,r5
  000d5a42: blx 0x00074398
  000d5a46: ldr.w r1,[r9,#0x114]
  000d5a4a: mov r0,r5
  000d5a4c: blx 0x0007447c
  000d5a50: add.w r10,r8,#0x2
  000d5a54: adds r4,#0x2
  000d5a56: ldr r0,[sp,#0x8]
  000d5a58: cmp r10,r0
  000d5a5a: blt 0x000d5a00
  000d5a5c: ldr r4,[sp,#0xc]
  000d5a5e: orr r0,r4,#0x4
  000d5a62: cmp r0,#0x6
  000d5a64: bne.w 0x000d5b8e
  000d5a68: ldr r0,[0x000d5c7c]
  000d5a6a: movs r1,#0x5
  000d5a6c: add r0,pc
  000d5a6e: ldr r0,[r0,#0x0]
  000d5a70: ldr r0,[r0,#0x0]
  000d5a72: blx 0x00071848
  000d5a76: movw r1,#0x2710
  000d5a7a: addw r6,r0,#0xc4a
  000d5a7e: add r1,r4
  000d5a80: str r1,[sp,#0x10]
  000d5a82: b 0x000d5b6a
  000d5a84: ldr r0,[0x000d5c80]
  000d5a86: movs r1,#0x5
  000d5a88: add r0,pc
  000d5a8a: ldr r0,[r0,#0x0]
  000d5a8c: ldr r0,[r0,#0x0]
  000d5a8e: blx 0x00071848
  000d5a92: ldr.w r1,[r9,#0xf0]
  000d5a96: addw r6,r0,#0xc4f
  000d5a9a: movs r0,#0x0
  000d5a9c: movw r2,#0x1770
  000d5aa0: str r0,[sp,#0x10]
  000d5aa2: ldr r1,[r1,#0x10]
  000d5aa4: ldr r1,[r1,#0x8]
  000d5aa6: add.w r8,r1,r2
  000d5aaa: b 0x000d5b6e
  000d5aac: ldr r1,[0x000d5c84]
  000d5aae: movs r0,#0x0
  000d5ab0: movs r2,#0x0
  000d5ab2: add r1,pc
  000d5ab4: b 0x000d5abe
  000d5ab6: ldr.w r3,[r1,r2,lsl #0x2]
  000d5aba: adds r2,#0x1
  000d5abc: add r0,r3
  000d5abe: cmp r2,r10
  000d5ac0: blt 0x000d5ab6
  000d5ac2: lsls r6,r0,#0x1
  000d5ac4: ldr.w r0,[r1,r10,lsl #0x2]
  000d5ac8: str.w r9,[sp,#0x8]
  000d5acc: str r0,[sp,#0xc]
  000d5ace: mov.w r9,#0x0
  000d5ad2: ldr r0,[0x000d5c88]
  000d5ad4: add r0,pc
  000d5ad6: str r0,[sp,#0x4]
  000d5ad8: ldr.w r11,[sp,#0x4]
  000d5adc: b 0x000d5b32
  000d5ade: sub.w r8,r9,#0x1
  000d5ae2: add.w r0,r11,r6, lsl #0x2
  000d5ae6: cmp.w r9,#0x0
  000d5aea: mov r4,r8
  000d5aec: it eq
  000d5aee: movw.eq r4,#0x1388
  000d5af2: str r0,[sp,#0x10]
  000d5af4: movs r0,#0x28
  000d5af6: blx 0x0006eb24
  000d5afa: mov r5,r0
  000d5afc: ldr r0,[sp,#0x10]
  000d5afe: movs r3,#0x6
  000d5b00: ldr.w r2,[r11,r6,lsl #0x2]
  000d5b04: str r4,[sp,#0x0]
  000d5b06: ldr r1,[r0,#0x4]
  000d5b08: orr r0,r6,#0x4
  000d5b0c: cmp r0,#0x6
  000d5b0e: it eq
  000d5b10: add.eq r2,r10
  000d5b12: cmp.w r9,#0x0
  000d5b16: it eq
  000d5b18: mov.eq r3,#0x5
  000d5b1a: mov r0,r5
  000d5b1c: blx 0x00074398
  000d5b20: ldr r0,[sp,#0x8]
  000d5b22: ldr.w r1,[r0,#0x114]
  000d5b26: mov r0,r5
  000d5b28: blx 0x0007447c
  000d5b2c: add.w r9,r8,#0x2
  000d5b30: adds r6,#0x2
  000d5b32: ldr r0,[sp,#0xc]
  000d5b34: cmp r9,r0
  000d5b36: blt 0x000d5ade
  000d5b38: cmp.w r10,#0x2
  000d5b3c: bcs 0x000d5c36
  000d5b3e: ldr r1,[sp,#0x8]
  000d5b40: ldr.w r0,[r1,#0xf0]
  000d5b44: ldr.w r1,[r1,#0x114]
  000d5b48: b 0x000d5b96
  000d5b4a: movw r6,#0xc54
  000d5b4e: b 0x000d5b66
  000d5b50: movw r6,#0xc55
  000d5b54: b 0x000d5b66
  000d5b56: movw r6,#0xc57
  000d5b5a: b 0x000d5b66
  000d5b5c: movw r6,#0xc58
  000d5b60: b 0x000d5b66
  000d5b62: movw r6,#0xc59
  000d5b66: movs r0,#0x0
  000d5b68: str r0,[sp,#0x10]
  000d5b6a: mov.w r8,#0x0
  000d5b6e: movs r0,#0x28
  000d5b70: blx 0x0006eb24
  000d5b74: mov r5,r0
  000d5b76: str.w r8,[sp,#0x0]
  000d5b7a: mov r1,r6
  000d5b7c: ldr r2,[sp,#0x10]
  000d5b7e: movs r3,#0x5
  000d5b80: blx 0x00074398
  000d5b84: ldr.w r1,[r9,#0x114]
  000d5b88: mov r0,r5
  000d5b8a: blx 0x0007447c
  000d5b8e: ldr.w r0,[r9,#0xf0]
  000d5b92: ldr.w r1,[r9,#0x114]
  000d5b96: ldr r0,[r0,#0x18]
  000d5b98: add sp,#0x14
  000d5b9a: pop.w {r8,r9,r10,r11}
  000d5b9e: pop.w {r4,r5,r6,r7,lr}
  000d5ba2: b.w 0x001abff8
  000d5ba6: movs r0,#0x28
  000d5ba8: blx 0x0006eb24
  000d5bac: mov r5,r0
  000d5bae: movw r0,#0x5dc
  000d5bb2: movs r2,#0x6
  000d5bb4: str r0,[sp,#0x0]
  000d5bb6: mov.w r0,#0xc60
  000d5bba: add.w r1,r0,r10, lsl #0x1
  000d5bbe: mov r0,r5
  000d5bc0: movs r3,#0x5
  000d5bc2: blx 0x00074398
  000d5bc6: ldr.w r1,[r9,#0x114]
  000d5bca: mov r0,r5
  000d5bcc: lsl.w r4,r10,#0x1
  000d5bd0: blx 0x0007447c
  000d5bd4: movs r0,#0x28
  000d5bd6: blx 0x0006eb24
  000d5bda: mov r5,r0
  000d5bdc: movs r0,#0x0
  000d5bde: addw r1,r4,#0xc61
  000d5be2: str r0,[sp,#0x0]
  000d5be4: mov r0,r5
  000d5be6: movs r2,#0x0
  000d5be8: movs r3,#0x6
  000d5bea: blx 0x00074398
  000d5bee: b 0x000d5b84
  000d5bf0: movs r0,#0x26
  000d5bf2: mov.w r8,#0x0
  000d5bf6: str r0,[sp,#0x10]
  000d5bf8: mov r6,r10
  000d5bfa: b 0x000d5b6e
  000d5bfc: ldr r0,[0x000d5ca0]
  000d5bfe: movs r1,#0x3
  000d5c00: add r0,pc
  000d5c02: ldr r0,[r0,#0x0]
  000d5c04: ldr r0,[r0,#0x0]
  000d5c06: blx 0x00071848
  000d5c0a: addw r6,r0,#0x1bd
  000d5c0e: b 0x000d5b6a
  000d5c10: ldr.w r0,[r9,#0x114]
  000d5c14: cbz r0,0x000d5c1e
  000d5c16: blx 0x00073bac
  000d5c1a: blx 0x0006eb48
  000d5c1e: ldr.w r0,[r9,#0xf0]
  000d5c22: movs r1,#0x0
  000d5c24: str.w r1,[r9,#0x114]
  000d5c28: movs r1,#0x0
  000d5c2a: b 0x000d5b96
  000d5c2c: mov.w r8,#0x0
  000d5c30: movw r6,#0x88f
  000d5c34: b 0x000d5b6e
  000d5c36: ldr r0,[0x000d5c8c]
  000d5c38: movs r1,#0x5
  000d5c3a: add r0,pc
  000d5c3c: ldr r0,[r0,#0x0]
  000d5c3e: ldr r0,[r0,#0x0]
  000d5c40: blx 0x00071848
  000d5c44: addw r6,r0,#0xc45
  000d5c48: movs r0,#0x0
  000d5c4a: str r0,[sp,#0x10]
  000d5c4c: mov.w r8,#0x0
  000d5c50: ldr.w r9,[sp,#0x8]
  000d5c54: b 0x000d5b6e
  001abff8: bx pc
```
