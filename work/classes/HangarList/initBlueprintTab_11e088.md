# HangarList::initBlueprintTab
elf 0x11e088 body 394
Sig: undefined __thiscall initBlueprintTab(HangarList * this, Array * param_1)

## decompile
```c

/* HangarList::initBlueprintTab(Array<BluePrint*>*) */

void __thiscall HangarList::initBlueprintTab(HangarList *this,Array *param_1)

{
  bool bVar1;
  Array<ListItem*> *pAVar2;
  void *pvVar3;
  uint *puVar4;
  ListItem *pLVar5;
  String *pSVar6;
  int *piVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  
  piVar7 = (int *)(*(int *)(*(int *)this + 4) + 8);
  pAVar2 = (Array<ListItem*> *)*piVar7;
  if (pAVar2 != (Array<ListItem*> *)0x0) {
    if (*(int *)pAVar2 != 0) {
      ArrayReleaseClasses<ListItem*>((Array *)pAVar2);
      piVar7 = (int *)(*(int *)(*(int *)this + 4) + 8);
      pAVar2 = (Array<ListItem*> *)*piVar7;
      if (pAVar2 == (Array<ListItem*> *)0x0) goto LAB_0012e0c4;
    }
    pvVar3 = (void *)Array<ListItem*>::~Array(pAVar2);
    operator_delete(pvVar3);
    piVar7 = (int *)(*(int *)(*(int *)this + 4) + 8);
  }
LAB_0012e0c4:
  *piVar7 = 0;
  pAVar2 = operator_new(0xc);
  Array<ListItem*>::Array(pAVar2);
  uVar10 = 1;
  for (uVar9 = 0; uVar9 < *(uint *)param_1; uVar9 = uVar9 + 1) {
    iVar12 = BluePrint::isUnlocked(*(BluePrint **)(*(int *)(param_1 + 4) + uVar9 * 4));
    uVar10 = uVar10 + iVar12;
  }
  puVar4 = (uint *)Status::getPendingProducts((Status *)**(undefined4 **)(DAT_0012e22c + 0x12e0f4));
  uVar9 = uVar10;
  if (puVar4 == (uint *)0x0) {
    bVar1 = true;
  }
  else {
    uVar8 = 0;
    while (*puVar4 != uVar8) {
      iVar12 = uVar8 * 4;
      uVar8 = uVar8 + 1;
      if (*(int *)(puVar4[1] + iVar12) != 0) {
        uVar9 = uVar9 + 1;
      }
    }
    bVar1 = uVar9 <= uVar10;
    if (uVar10 < uVar9) {
      uVar9 = uVar9 + 1;
    }
  }
  ArraySetLength<ListItem*>(uVar9,(Array *)pAVar2);
  pLVar5 = operator_new(0x48);
  piVar7 = *(int **)(DAT_0012e230 + 0x12e146);
  pSVar6 = (String *)GameText::getText(*piVar7);
  ::ListItem::ListItem(pLVar5,pSVar6);
  iVar12 = 1;
  **(undefined4 **)(pAVar2 + 4) = pLVar5;
  for (uVar9 = 0; uVar9 < *(uint *)param_1; uVar9 = uVar9 + 1) {
    iVar11 = BluePrint::isUnlocked(*(BluePrint **)(*(int *)(param_1 + 4) + uVar9 * 4));
    if (iVar11 != 0) {
      pLVar5 = operator_new(0x48);
      ::ListItem::ListItem(pLVar5,*(BluePrint **)(*(int *)(param_1 + 4) + uVar9 * 4));
      *(ListItem **)(*(int *)(pAVar2 + 4) + iVar12 * 4) = pLVar5;
      iVar12 = iVar12 + 1;
    }
  }
  if (!bVar1) {
    pLVar5 = operator_new(0x48);
    pSVar6 = (String *)GameText::getText(*piVar7);
    ::ListItem::ListItem(pLVar5,pSVar6);
    iVar11 = iVar12 + 1;
    *(ListItem **)(*(int *)(pAVar2 + 4) + iVar12 * 4) = pLVar5;
    for (uVar9 = 0; uVar9 < *puVar4; uVar9 = uVar9 + 1) {
      if (*(int *)(puVar4[1] + uVar9 * 4) != 0) {
        pLVar5 = operator_new(0x48);
        ::ListItem::ListItem(pLVar5,*(PendingProduct **)(puVar4[1] + uVar9 * 4));
        *(ListItem **)(*(int *)(pAVar2 + 4) + iVar11 * 4) = pLVar5;
        iVar11 = iVar11 + 1;
      }
    }
  }
  *(Array<ListItem*> **)(*(int *)(*(int *)this + 4) + 8) = pAVar2;
  return;
}

```

## target disasm
```
  0012e088: push {r4,r5,r6,r7,lr}
  0012e08a: add r7,sp,#0xc
  0012e08c: push {r5,r6,r7,r8,r9,r10,r11}
  0012e090: mov r8,r0
  0012e092: ldr r0,[r0,#0x0]
  0012e094: mov r6,r1
  0012e096: ldr r1,[r0,#0x4]
  0012e098: ldr.w r0,[r1,#0x8]!
  0012e09c: cbz r0,0x0012e0c4
  0012e09e: ldr r1,[r0,#0x0]
  0012e0a0: cbz r1,0x0012e0b2
  0012e0a2: blx 0x00076108
  0012e0a6: ldr.w r0,[r8,#0x0]
  0012e0aa: ldr r1,[r0,#0x4]
  0012e0ac: ldr.w r0,[r1,#0x8]!
  0012e0b0: cbz r0,0x0012e0c4
  0012e0b2: blx 0x00076114
  0012e0b6: blx 0x0006eb48
  0012e0ba: ldr.w r0,[r8,#0x0]
  0012e0be: ldr r0,[r0,#0x4]
  0012e0c0: add.w r1,r0,#0x8
  0012e0c4: movs r0,#0x0
  0012e0c6: str r0,[r1,#0x0]
  0012e0c8: movs r0,#0xc
  0012e0ca: blx 0x0006eb24
  0012e0ce: mov r9,r0
  0012e0d0: blx 0x00076144
  0012e0d4: movs r4,#0x0
  0012e0d6: movs r5,#0x1
  0012e0d8: b 0x0012e0e8
  0012e0da: ldr r0,[r6,#0x4]
  0012e0dc: ldr.w r0,[r0,r4,lsl #0x2]
  0012e0e0: blx 0x000739a8
  0012e0e4: add r5,r0
  0012e0e6: adds r4,#0x1
  0012e0e8: ldr r0,[r6,#0x0]
  0012e0ea: cmp r4,r0
  0012e0ec: bcc 0x0012e0da
  0012e0ee: ldr r0,[0x0012e22c]
  0012e0f0: add r0,pc
  0012e0f2: ldr r0,[r0,#0x0]
  0012e0f4: ldr r0,[r0,#0x0]
  0012e0f6: blx 0x00074668
  0012e0fa: mov r10,r0
  0012e0fc: cbz r0,0x0012e12a
  0012e0fe: ldr.w r1,[r10,#0x0]
  0012e102: movs r2,#0x0
  0012e104: mov r0,r5
  0012e106: b 0x0012e118
  0012e108: ldr.w r3,[r10,#0x4]
  0012e10c: ldr.w r3,[r3,r2,lsl #0x2]
  0012e110: adds r2,#0x1
  0012e112: cmp r3,#0x0
  0012e114: it ne
  0012e116: add.ne r0,#0x1
  0012e118: cmp r1,r2
  0012e11a: bne 0x0012e108
  0012e11c: movs r4,#0x0
  0012e11e: cmp r0,r5
  0012e120: ite ls
  0012e122: mov.ls r4,#0x1
  0012e124: add.hi r0,#0x1
  0012e126: mov r5,r0
  0012e128: b 0x0012e12c
  0012e12a: movs r4,#0x1
  0012e12c: mov r0,r5
  0012e12e: mov r1,r9
  0012e130: str.w r8,[sp,#0x8]
  0012e134: blx 0x00076168
  0012e138: movs r0,#0x48
  0012e13a: blx 0x0006eb24
  0012e13e: mov r5,r0
  0012e140: ldr r0,[0x0012e230]
  0012e142: add r0,pc
  0012e144: ldr r0,[r0,#0x0]
  0012e146: str r0,[sp,#0x4]
  0012e148: ldr r0,[r0,#0x0]
  0012e14a: movw r1,#0x111
  0012e14e: blx 0x00072f70
  0012e152: mov r1,r0
  0012e154: mov r0,r5
  0012e156: blx 0x00076174
  0012e15a: ldr.w r0,[r9,#0x4]
  0012e15e: mov.w r11,#0x0
  0012e162: mov.w r8,#0x1
  0012e166: str r5,[r0,#0x0]
  0012e168: b 0x0012e19a
  0012e16a: ldr r0,[r6,#0x4]
  0012e16c: ldr.w r0,[r0,r11,lsl #0x2]
  0012e170: blx 0x000739a8
  0012e174: cbz r0,0x0012e196
  0012e176: movs r0,#0x48
  0012e178: blx 0x0006eb24
  0012e17c: mov r5,r0
  0012e17e: ldr r0,[r6,#0x4]
  0012e180: ldr.w r1,[r0,r11,lsl #0x2]
  0012e184: mov r0,r5
  0012e186: blx 0x000761b0
  0012e18a: ldr.w r0,[r9,#0x4]
  0012e18e: str.w r5,[r0,r8,lsl #0x2]
  0012e192: add.w r8,r8,#0x1
  0012e196: add.w r11,r11,#0x1
  0012e19a: ldr r0,[r6,#0x0]
  0012e19c: cmp r11,r0
  0012e19e: bcc 0x0012e16a
  0012e1a0: cbnz r4,0x0012e202
  0012e1a2: movs r0,#0x48
  0012e1a4: blx 0x0006eb24
  0012e1a8: mov r5,r0
  0012e1aa: ldr r0,[sp,#0x4]
  0012e1ac: ldr r0,[r0,#0x0]
  0012e1ae: mov.w r1,#0x112
  0012e1b2: blx 0x00072f70
  0012e1b6: mov r1,r0
  0012e1b8: mov r0,r5
  0012e1ba: blx 0x00076174
  0012e1be: ldr.w r0,[r9,#0x4]
  0012e1c2: add.w r6,r8,#0x1
  0012e1c6: movs r4,#0x0
  0012e1c8: str.w r5,[r0,r8,lsl #0x2]
  0012e1cc: b 0x0012e1fa
  0012e1ce: ldr.w r0,[r10,#0x4]
  0012e1d2: ldr.w r0,[r0,r4,lsl #0x2]
  0012e1d6: cbz r0,0x0012e1f8
  0012e1d8: movs r0,#0x48
  0012e1da: blx 0x0006eb24
  0012e1de: mov r5,r0
  0012e1e0: ldr.w r0,[r10,#0x4]
  0012e1e4: ldr.w r1,[r0,r4,lsl #0x2]
  0012e1e8: mov r0,r5
  0012e1ea: blx 0x000761bc
  0012e1ee: ldr.w r0,[r9,#0x4]
  0012e1f2: str.w r5,[r0,r6,lsl #0x2]
  0012e1f6: adds r6,#0x1
  0012e1f8: adds r4,#0x1
  0012e1fa: ldr.w r0,[r10,#0x0]
  0012e1fe: cmp r4,r0
  0012e200: bcc 0x0012e1ce
  0012e202: ldr r0,[sp,#0x8]
  0012e204: ldr r0,[r0,#0x0]
  0012e206: ldr r0,[r0,#0x4]
  0012e208: str.w r9,[r0,#0x8]
  0012e20c: pop.w {r1,r2,r3,r8,r9,r10,r11}
  0012e210: pop {r4,r5,r6,r7,pc}
```
