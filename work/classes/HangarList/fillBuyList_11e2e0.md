# HangarList::fillBuyList
elf 0x11e2e0 body 656
Sig: undefined __thiscall fillBuyList(HangarList * this, ListItem * param_1)

## decompile
```c

/* HangarList::fillBuyList(ListItem*) */

void __thiscall HangarList::fillBuyList(HangarList *this,ListItem *param_1)

{
  Array<ListItem*> *pAVar1;
  void *pvVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  ListItem *pLVar7;
  String *pSVar8;
  uint *puVar9;
  int *piVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  
  piVar10 = (int *)(*(int *)(*(int *)this + 4) + 0xc);
  pAVar1 = (Array<ListItem*> *)*piVar10;
  if (pAVar1 != (Array<ListItem*> *)0x0) {
    if (*(int *)pAVar1 != 0) {
      ArrayReleaseClasses<ListItem*>((Array *)pAVar1);
      piVar10 = (int *)(*(int *)(*(int *)this + 4) + 0xc);
      pAVar1 = (Array<ListItem*> *)*piVar10;
      if (pAVar1 == (Array<ListItem*> *)0x0) goto LAB_0012e318;
    }
    pvVar2 = (void *)Array<ListItem*>::~Array(pAVar1);
    operator_delete(pvVar2);
    piVar10 = (int *)(*(int *)(*(int *)this + 4) + 0xc);
  }
LAB_0012e318:
  *piVar10 = 0;
  uVar3 = ::ListItem::isShip(param_1);
  iVar13 = *(int *)(param_1 + 0x28);
  if (uVar3 == 0) {
    iVar11 = ::ListItem::isSlot(param_1);
    if (iVar11 == 0) {
      iVar13 = Item::getType(*(Item **)(param_1 + 0x10));
    }
    Status::getShip();
    puVar9 = (uint *)Ship::getCargo();
    if (puVar9 != (uint *)0x0) {
      iVar11 = 0;
      for (uVar4 = 0; uVar4 < *puVar9; uVar4 = uVar4 + 1) {
        iVar5 = Item::getType(*(Item **)(puVar9[1] + uVar4 * 4));
        if (iVar5 == iVar13) {
          iVar11 = iVar11 + 1;
        }
      }
      goto LAB_0012e38c;
    }
  }
  else {
    Status::getStation();
    piVar10 = (int *)Station::getShips();
    if (piVar10 != (int *)0x0) {
      iVar11 = *piVar10;
      goto LAB_0012e38c;
    }
  }
  iVar11 = 0;
LAB_0012e38c:
  uVar4 = ::ListItem::isSlot(param_1);
  pAVar1 = operator_new(0xc);
  Array<ListItem*>::Array(pAVar1);
  iVar5 = 3;
  uVar4 = uVar3 | uVar4;
  if (iVar11 == 0) {
    iVar5 = 4;
  }
  uVar6 = iVar5 + iVar11 + (uVar4 ^ 1);
  if (uVar4 == 0) {
    uVar6 = uVar6 + 2;
  }
  ArraySetLength<ListItem*>(uVar6,(Array *)pAVar1);
  pLVar7 = operator_new(0x48);
  piVar10 = *(int **)(DAT_0012e5a0 + 0x12e3d2);
  pSVar8 = (String *)GameText::getText(*piVar10);
  ::ListItem::ListItem(pLVar7,pSVar8);
  **(undefined4 **)(pAVar1 + 4) = pLVar7;
  pLVar7 = operator_new(0x48);
  ::ListItem::ListItem(pLVar7,param_1);
  *(ListItem **)(*(int *)(pAVar1 + 4) + 4) = pLVar7;
  if (uVar4 == 0) {
    pLVar7 = operator_new(0x48);
    pSVar8 = (String *)GameText::getText(*piVar10);
    ::ListItem::ListItem(pLVar7,pSVar8);
    *(ListItem **)(*(int *)(pAVar1 + 4) + 8) = pLVar7;
    pLVar7 = operator_new(0x48);
    pSVar8 = (String *)GameText::getText(*piVar10);
    ::ListItem::ListItem(pLVar7,pSVar8,true,0);
    *(ListItem **)(*(int *)(pAVar1 + 4) + 0xc) = pLVar7;
    pLVar7 = operator_new(0x48);
    pSVar8 = (String *)GameText::getText(*piVar10);
    ::ListItem::ListItem(pLVar7,pSVar8,true,1);
    iVar5 = 5;
    *(ListItem **)(*(int *)(pAVar1 + 4) + 0x10) = pLVar7;
  }
  else {
    iVar5 = 2;
  }
  pLVar7 = operator_new(0x48);
  if (uVar3 == 0) {
    pSVar8 = (String *)GameText::getText(*piVar10);
  }
  else {
    pSVar8 = (String *)GameText::getText(*piVar10);
  }
  ::ListItem::ListItem(pLVar7,pSVar8);
  iVar12 = iVar5 + 1;
  *(ListItem **)(*(int *)(pAVar1 + 4) + iVar5 * 4) = pLVar7;
  if (iVar11 < 1) {
    pLVar7 = operator_new(0x48);
    ::ListItem::ListItem(pLVar7,iVar12);
    *(ListItem **)(*(int *)(pAVar1 + 4) + iVar12 * 4) = pLVar7;
  }
  else if (uVar3 == 0) {
    Status::getShip();
    puVar9 = (uint *)Ship::getCargo();
    for (uVar3 = 0; uVar3 < *puVar9; uVar3 = uVar3 + 1) {
      iVar11 = Item::getType(*(Item **)(puVar9[1] + uVar3 * 4));
      if (iVar11 == iVar13) {
        pLVar7 = operator_new(0x48);
        ::ListItem::ListItem(pLVar7,*(Item **)(puVar9[1] + uVar3 * 4));
        *(ListItem **)(*(int *)(pAVar1 + 4) + iVar12 * 4) = pLVar7;
        iVar12 = iVar12 + 1;
      }
    }
  }
  else {
    Status::getStation();
    puVar9 = (uint *)Station::getShips();
    for (uVar3 = 0; uVar3 < *puVar9; uVar3 = uVar3 + 1) {
      pLVar7 = operator_new(0x48);
      ::ListItem::ListItem(pLVar7,*(Ship **)(puVar9[1] + uVar3 * 4));
      *(ListItem **)(*(int *)(pAVar1 + 4) + iVar5 * 4 + 4 + uVar3 * 4) = pLVar7;
    }
  }
  *(Array<ListItem*> **)(*(int *)(*(int *)this + 4) + 0xc) = pAVar1;
  return;
}

```

## target disasm
```
  0012e2e0: push {r4,r5,r6,r7,lr}
  0012e2e2: add r7,sp,#0xc
  0012e2e4: push {r5,r6,r7,r8,r9,r10,r11}
  0012e2e8: mov r4,r0
  0012e2ea: ldr r0,[r0,#0x0]
  0012e2ec: mov r9,r1
  0012e2ee: ldr r1,[r0,#0x4]
  0012e2f0: ldr.w r0,[r1,#0xc]!
  0012e2f4: cbz r0,0x0012e318
  0012e2f6: ldr r1,[r0,#0x0]
  0012e2f8: cbz r1,0x0012e308
  0012e2fa: blx 0x00076108
  0012e2fe: ldr r0,[r4,#0x0]
  0012e300: ldr r1,[r0,#0x4]
  0012e302: ldr.w r0,[r1,#0xc]!
  0012e306: cbz r0,0x0012e318
  0012e308: blx 0x00076114
  0012e30c: blx 0x0006eb48
  0012e310: ldr r0,[r4,#0x0]
  0012e312: ldr r0,[r0,#0x4]
  0012e314: add.w r1,r0,#0xc
  0012e318: movs r0,#0x0
  0012e31a: str r0,[r1,#0x0]
  0012e31c: mov r0,r9
  0012e31e: blx 0x00076204
  0012e322: ldr.w r10,[r9,#0x28]
  0012e326: mov r8,r0
  0012e328: cmp r0,#0x0
  0012e32a: str r4,[sp,#0x4]
  0012e32c: beq 0x0012e344
  0012e32e: ldr r0,[0x0012e59c]
  0012e330: add r0,pc
  0012e332: ldr r0,[r0,#0x0]
  0012e334: ldr r0,[r0,#0x0]
  0012e336: blx 0x00071c14
  0012e33a: blx 0x00073708
  0012e33e: cbz r0,0x0012e38a
  0012e340: ldr r6,[r0,#0x0]
  0012e342: b 0x0012e38c
  0012e344: mov r0,r9
  0012e346: blx 0x00076210
  0012e34a: cbnz r0,0x0012e356
  0012e34c: ldr.w r0,[r9,#0x10]
  0012e350: blx 0x000718fc
  0012e354: mov r10,r0
  0012e356: ldr r0,[0x0012e598]
  0012e358: add r0,pc
  0012e35a: ldr r0,[r0,#0x0]
  0012e35c: ldr r0,[r0,#0x0]
  0012e35e: blx 0x00071a58
  0012e362: blx 0x00073750
  0012e366: mov r5,r0
  0012e368: cbz r0,0x0012e38a
  0012e36a: movs r4,#0x0
  0012e36c: movs r6,#0x0
  0012e36e: b 0x0012e382
  0012e370: ldr r0,[r5,#0x4]
  0012e372: ldr.w r0,[r0,r4,lsl #0x2]
  0012e376: blx 0x000718fc
  0012e37a: cmp r0,r10
  0012e37c: it eq
  0012e37e: add.eq r6,#0x1
  0012e380: adds r4,#0x1
  0012e382: ldr r0,[r5,#0x0]
  0012e384: cmp r4,r0
  0012e386: bcc 0x0012e370
  0012e388: b 0x0012e38c
  0012e38a: movs r6,#0x0
  0012e38c: mov r0,r9
  0012e38e: blx 0x00076210
  0012e392: mov r5,r0
  0012e394: movs r0,#0xc
  0012e396: blx 0x0006eb24
  0012e39a: mov r11,r0
  0012e39c: blx 0x00076144
  0012e3a0: movs r0,#0x3
  0012e3a2: orr.w r4,r8,r5
  0012e3a6: str.w r8,[sp,#0x8]
  0012e3aa: cmp r6,#0x0
  0012e3ac: it eq
  0012e3ae: mov.eq r0,#0x4
  0012e3b0: eor r1,r4,#0x1
  0012e3b4: add r0,r6
  0012e3b6: cmp r4,#0x0
  0012e3b8: add r0,r1
  0012e3ba: mov r1,r11
  0012e3bc: it eq
  0012e3be: add.eq r0,#0x2
  0012e3c0: blx 0x00076168
  0012e3c4: movs r0,#0x48
  0012e3c6: blx 0x0006eb24
  0012e3ca: mov r5,r0
  0012e3cc: ldr r0,[0x0012e5a0]
  0012e3ce: add r0,pc
  0012e3d0: ldr.w r8,[r0,#0x0]
  0012e3d4: ldr.w r0,[r8,#0x0]
  0012e3d8: movw r1,#0x115
  0012e3dc: blx 0x00072f70
  0012e3e0: mov r1,r0
  0012e3e2: mov r0,r5
  0012e3e4: blx 0x00076174
  0012e3e8: ldr.w r0,[r11,#0x4]
  0012e3ec: str r5,[r0,#0x0]
  0012e3ee: movs r0,#0x48
  0012e3f0: blx 0x0006eb24
  0012e3f4: mov r5,r0
  0012e3f6: mov r1,r9
  0012e3f8: blx 0x0007621c
  0012e3fc: ldr.w r0,[r11,#0x4]
  0012e400: cmp r4,#0x0
  0012e402: str r5,[r0,#0x4]
  0012e404: beq 0x0012e40c
  0012e406: mov.w r9,#0x2
  0012e40a: b 0x0012e47e
  0012e40c: movs r0,#0x48
  0012e40e: blx 0x0006eb24
  0012e412: mov r5,r0
  0012e414: ldr.w r0,[r8,#0x0]
  0012e418: mov.w r1,#0x116
  0012e41c: blx 0x00072f70
  0012e420: mov r1,r0
  0012e422: mov r0,r5
  0012e424: blx 0x00076174
  0012e428: ldr.w r0,[r11,#0x4]
  0012e42c: str r5,[r0,#0x8]
  0012e42e: movs r0,#0x48
  0012e430: blx 0x0006eb24
  0012e434: mov r5,r0
  0012e436: ldr.w r0,[r8,#0x0]
  0012e43a: movw r1,#0x117
  0012e43e: blx 0x00072f70
  0012e442: mov r1,r0
  0012e444: mov r0,r5
  0012e446: movs r2,#0x1
  0012e448: movs r3,#0x0
  0012e44a: blx 0x00076228
  0012e44e: ldr.w r0,[r11,#0x4]
  0012e452: str r5,[r0,#0xc]
  0012e454: movs r0,#0x48
  0012e456: blx 0x0006eb24
  0012e45a: mov r5,r0
  0012e45c: ldr.w r0,[r8,#0x0]
  0012e460: mov.w r1,#0x11a
  0012e464: blx 0x00072f70
  0012e468: mov r1,r0
  0012e46a: mov r0,r5
  0012e46c: movs r2,#0x1
  0012e46e: movs r3,#0x1
  0012e470: blx 0x00076228
  0012e474: ldr.w r0,[r11,#0x4]
  0012e478: mov.w r9,#0x5
  0012e47c: str r5,[r0,#0x10]
  0012e47e: movs r0,#0x48
  0012e480: blx 0x0006eb24
  0012e484: mov r5,r0
  0012e486: ldr r1,[sp,#0x8]
  0012e488: ldr.w r0,[r8,#0x0]
  0012e48c: cbz r1,0x0012e49a
  0012e48e: movw r1,#0x11d
  0012e492: blx 0x00072f70
  0012e496: mov r1,r0
  0012e498: b 0x0012e4a4
  0012e49a: mov.w r1,#0x11c
  0012e49e: blx 0x00072f70
  0012e4a2: mov r1,r0
  0012e4a4: mov r0,r5
  0012e4a6: blx 0x00076174
  0012e4aa: ldr.w r0,[r11,#0x4]
  0012e4ae: add.w r8,r9,#0x1
  0012e4b2: cmp r6,#0x1
  0012e4b4: str.w r5,[r0,r9,lsl #0x2]
  0012e4b8: blt 0x0012e504
  0012e4ba: ldr r0,[0x0012e5a4]
  0012e4bc: ldr r1,[sp,#0x8]
  0012e4be: add r0,pc
  0012e4c0: cmp r1,#0x0
  0012e4c2: ldr r0,[r0,#0x0]
  0012e4c4: ldr r0,[r0,#0x0]
  0012e4c6: beq 0x0012e51c
  0012e4c8: movs r1,#0x4
  0012e4ca: add.w r8,r1,r9, lsl #0x2
  0012e4ce: blx 0x00071c14
  0012e4d2: blx 0x00073708
  0012e4d6: mov r4,r0
  0012e4d8: movs r6,#0x0
  0012e4da: b 0x0012e4fc
  0012e4dc: movs r0,#0x48
  0012e4de: blx 0x0006eb24
  0012e4e2: mov r5,r0
  0012e4e4: ldr r0,[r4,#0x4]
  0012e4e6: ldr.w r1,[r0,r6,lsl #0x2]
  0012e4ea: mov r0,r5
  0012e4ec: blx 0x00076180
  0012e4f0: ldr.w r0,[r11,#0x4]
  0012e4f4: add r0,r8
  0012e4f6: str.w r5,[r0,r6,lsl #0x2]
  0012e4fa: adds r6,#0x1
  0012e4fc: ldr r0,[r4,#0x0]
  0012e4fe: cmp r6,r0
  0012e500: bcc 0x0012e4dc
  0012e502: b 0x0012e560
  0012e504: movs r0,#0x48
  0012e506: blx 0x0006eb24
  0012e50a: mov r5,r0
  0012e50c: mov r1,r8
  0012e50e: blx 0x000761a4
  0012e512: ldr.w r0,[r11,#0x4]
  0012e516: str.w r5,[r0,r8,lsl #0x2]
  0012e51a: b 0x0012e560
  0012e51c: blx 0x00071a58
  0012e520: blx 0x00073750
  0012e524: mov r5,r0
  0012e526: movs r4,#0x0
  0012e528: b 0x0012e55a
  0012e52a: ldr r0,[r5,#0x4]
  0012e52c: ldr.w r0,[r0,r4,lsl #0x2]
  0012e530: blx 0x000718fc
  0012e534: cmp r0,r10
  0012e536: bne 0x0012e558
  0012e538: movs r0,#0x48
  0012e53a: blx 0x0006eb24
  0012e53e: mov r6,r0
  0012e540: ldr r0,[r5,#0x4]
  0012e542: ldr.w r1,[r0,r4,lsl #0x2]
  0012e546: mov r0,r6
  0012e548: blx 0x00076198
  0012e54c: ldr.w r0,[r11,#0x4]
  0012e550: str.w r6,[r0,r8,lsl #0x2]
  0012e554: add.w r8,r8,#0x1
  0012e558: adds r4,#0x1
  0012e55a: ldr r0,[r5,#0x0]
  0012e55c: cmp r4,r0
  0012e55e: bcc 0x0012e52a
  0012e560: ldr r0,[sp,#0x4]
  0012e562: ldr r0,[r0,#0x0]
  0012e564: ldr r0,[r0,#0x4]
  0012e566: str.w r11,[r0,#0xc]
  0012e56a: pop.w {r1,r2,r3,r8,r9,r10,r11}
  0012e56e: pop {r4,r5,r6,r7,pc}
```
