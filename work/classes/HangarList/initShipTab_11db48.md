# HangarList::initShipTab
elf 0x11db48 body 626
Sig: undefined __thiscall initShipTab(HangarList * this, Ship * param_1)

## decompile
```c

/* HangarList::initShipTab(Ship*) */

void __thiscall HangarList::initShipTab(HangarList *this,Ship *param_1)

{
  bool bVar1;
  Array<ListItem*> *pAVar2;
  void *pvVar3;
  uint *puVar4;
  uint *puVar5;
  Array<Item*> *this_00;
  int iVar6;
  int iVar7;
  ListItem *pLVar8;
  String *pSVar9;
  Array<Item*> *this_01;
  uint uVar10;
  int *piVar11;
  uint uVar12;
  uint uVar13;
  
  piVar11 = *(int **)(*(int *)this + 4);
  pAVar2 = (Array<ListItem*> *)*piVar11;
  if (pAVar2 != (Array<ListItem*> *)0x0) {
    if (*(int *)pAVar2 != 0) {
      ArrayReleaseClasses<ListItem*>((Array *)pAVar2);
      piVar11 = *(int **)(*(int *)this + 4);
      pAVar2 = (Array<ListItem*> *)*piVar11;
      if (pAVar2 == (Array<ListItem*> *)0x0) goto LAB_0012db7e;
    }
    pvVar3 = (void *)Array<ListItem*>::~Array(pAVar2);
    operator_delete(pvVar3);
    piVar11 = *(int **)(*(int *)this + 4);
  }
LAB_0012db7e:
  *piVar11 = 0;
  pAVar2 = operator_new(0xc);
  Array<ListItem*>::Array(pAVar2);
  puVar4 = (uint *)Ship::getEquipment(param_1);
  puVar5 = (uint *)Ship::getCargo();
  if (puVar5 == (uint *)0x0) {
    iVar6 = 2;
    this_00 = (Array<Item*> *)0x0;
    bVar1 = true;
  }
  else {
    this_00 = operator_new(0xc);
    Array<Item*>::Array(this_00);
    for (uVar12 = 0; uVar12 < *puVar5; uVar12 = uVar12 + 1) {
      iVar6 = Item::getType(*(Item **)(puVar5[1] + uVar12 * 4));
      if (iVar6 != 4) {
        iVar6 = Item::getType(*(Item **)(puVar5[1] + uVar12 * 4));
        iVar6 = Ship::getSlots(param_1,iVar6);
        if (0 < iVar6) {
          ArrayAdd<Item*>(*(Item **)(puVar5[1] + uVar12 * 4),(Array *)this_00);
        }
      }
    }
    bVar1 = false;
    iVar6 = *(int *)this_00 + 2;
  }
  uVar12 = *puVar4;
  iVar7 = Ship::getSlotTypes(param_1);
  ArraySetLength<ListItem*>(iVar7 + iVar6 + uVar12,(Array *)pAVar2);
  pLVar8 = operator_new(0x48);
  piVar11 = *(int **)(DAT_0012dde4 + 0x12dc26);
  pSVar9 = (String *)GameText::getText(*piVar11);
  ::ListItem::ListItem(pLVar8,pSVar9);
  **(undefined4 **)(pAVar2 + 4) = pLVar8;
  pLVar8 = operator_new(0x48);
  ::ListItem::ListItem(pLVar8,param_1);
  uVar12 = 0;
  iVar6 = 2;
  *(ListItem **)(*(int *)(pAVar2 + 4) + 4) = pLVar8;
  do {
    if (3 < uVar12) {
      **(undefined4 **)(*(int *)this + 4) = pAVar2;
      return;
    }
    iVar7 = Ship::getSlots(param_1,uVar12);
    if (0 < iVar7) {
      pLVar8 = operator_new(0x48);
      pSVar9 = (String *)GameText::getText(*piVar11);
      ::ListItem::ListItem(pLVar8,pSVar9,uVar12);
      *(ListItem **)(*(int *)(pAVar2 + 4) + iVar6 * 4) = pLVar8;
      this_01 = (Array<Item*> *)Ship::getEquipment(param_1,uVar12);
      for (uVar13 = 0; iVar6 = iVar6 + 1, uVar13 < *(uint *)this_01; uVar13 = uVar13 + 1) {
        iVar7 = *(int *)(*(int *)(this_01 + 4) + uVar13 * 4);
        pLVar8 = operator_new(0x48);
        if (iVar7 == 0) {
          ::ListItem::ListItem(pLVar8,uVar12);
        }
        else {
          ::ListItem::ListItem(pLVar8,*(Item **)(*(int *)(this_01 + 4) + uVar13 * 4));
        }
        *(ListItem **)(*(int *)(pAVar2 + 4) + iVar6 * 4) = pLVar8;
        for (uVar10 = 0; uVar10 < *puVar4; uVar10 = uVar10 + 1) {
          iVar7 = *(int *)(*(int *)(pAVar2 + 4) + iVar6 * 4);
          if (*(int *)(puVar4[1] + uVar10 * 4) == *(int *)(iVar7 + 0x10)) {
            *(uint *)(iVar7 + 0x40) = uVar10;
            goto LAB_0012dd2a;
          }
        }
        iVar7 = *(int *)(*(int *)(pAVar2 + 4) + iVar6 * 4);
LAB_0012dd2a:
        *(uint *)(iVar7 + 0x3c) = uVar13;
      }
      pvVar3 = (void *)Array<Item*>::~Array(this_01);
      operator_delete(pvVar3);
      if (!bVar1) {
        for (uVar13 = 0; uVar13 < *(uint *)this_00; uVar13 = uVar13 + 1) {
          uVar10 = Item::getType(*(Item **)(*(int *)(this_00 + 4) + uVar13 * 4));
          if (uVar10 == uVar12) {
            pLVar8 = operator_new(0x48);
            ::ListItem::ListItem(pLVar8,*(Item **)(*(int *)(this_00 + 4) + uVar13 * 4));
            *(ListItem **)(*(int *)(pAVar2 + 4) + iVar6 * 4) = pLVar8;
            iVar6 = iVar6 + 1;
          }
        }
      }
    }
    uVar12 = uVar12 + 1;
  } while( true );
}

```

## target disasm
```
  0012db48: push {r4,r5,r6,r7,lr}
  0012db4a: add r7,sp,#0xc
  0012db4c: push {r8,r9,r10,r11}
  0012db50: sub sp,#0x24
  0012db52: mov r9,r0
  0012db54: ldr r0,[r0,#0x0]
  0012db56: mov r10,r1
  0012db58: ldr r1,[r0,#0x4]
  0012db5a: ldr r0,[r1,#0x0]
  0012db5c: cbz r0,0x0012db7e
  0012db5e: ldr r1,[r0,#0x0]
  0012db60: cbz r1,0x0012db70
  0012db62: blx 0x00076108
  0012db66: ldr.w r0,[r9,#0x0]
  0012db6a: ldr r1,[r0,#0x4]
  0012db6c: ldr r0,[r1,#0x0]
  0012db6e: cbz r0,0x0012db7e
  0012db70: blx 0x00076114
  0012db74: blx 0x0006eb48
  0012db78: ldr.w r0,[r9,#0x0]
  0012db7c: ldr r1,[r0,#0x4]
  0012db7e: movs r0,#0x0
  0012db80: str r0,[r1,#0x0]
  0012db82: movs r0,#0xc
  0012db84: blx 0x0006eb24
  0012db88: str r0,[sp,#0x20]
  0012db8a: blx 0x00076144
  0012db8e: mov r0,r10
  0012db90: blx 0x0007333c
  0012db94: str r0,[sp,#0x1c]
  0012db96: mov r0,r10
  0012db98: blx 0x00073750
  0012db9c: mov r8,r0
  0012db9e: cbz r0,0x0012dbfa
  0012dba0: movs r0,#0xc
  0012dba2: blx 0x0006eb24
  0012dba6: mov r11,r0
  0012dba8: blx 0x00071860
  0012dbac: movs r5,#0x0
  0012dbae: b 0x0012dbe8
  0012dbb0: ldr.w r0,[r8,#0x4]
  0012dbb4: ldr.w r0,[r0,r5,lsl #0x2]
  0012dbb8: blx 0x000718fc
  0012dbbc: cmp r0,#0x4
  0012dbbe: beq 0x0012dbe6
  0012dbc0: ldr.w r0,[r8,#0x4]
  0012dbc4: ldr.w r0,[r0,r5,lsl #0x2]
  0012dbc8: blx 0x000718fc
  0012dbcc: mov r1,r0
  0012dbce: mov r0,r10
  0012dbd0: blx 0x00076150
  0012dbd4: cmp r0,#0x1
  0012dbd6: blt 0x0012dbe6
  0012dbd8: ldr.w r0,[r8,#0x4]
  0012dbdc: mov r1,r11
  0012dbde: ldr.w r0,[r0,r5,lsl #0x2]
  0012dbe2: blx 0x000718b4
  0012dbe6: adds r5,#0x1
  0012dbe8: ldr.w r0,[r8,#0x0]
  0012dbec: cmp r5,r0
  0012dbee: bcc 0x0012dbb0
  0012dbf0: ldr.w r0,[r11,#0x0]
  0012dbf4: movs r1,#0x0
  0012dbf6: adds r0,#0x2
  0012dbf8: b 0x0012dc02
  0012dbfa: movs r0,#0x2
  0012dbfc: mov.w r11,#0x0
  0012dc00: movs r1,#0x1
  0012dc02: str r1,[sp,#0x8]
  0012dc04: ldr r1,[sp,#0x1c]
  0012dc06: ldr r1,[r1,#0x0]
  0012dc08: adds r5,r0,r1
  0012dc0a: mov r0,r10
  0012dc0c: blx 0x0007615c
  0012dc10: ldr r1,[sp,#0x20]
  0012dc12: add r0,r5
  0012dc14: blx 0x00076168
  0012dc18: movs r0,#0x48
  0012dc1a: blx 0x0006eb24
  0012dc1e: mov r5,r0
  0012dc20: ldr r0,[0x0012dde4]
  0012dc22: add r0,pc
  0012dc24: ldr r0,[r0,#0x0]
  0012dc26: str r0,[sp,#0xc]
  0012dc28: ldr r0,[r0,#0x0]
  0012dc2a: movs r1,#0xb7
  0012dc2c: blx 0x00072f70
  0012dc30: mov r1,r0
  0012dc32: mov r0,r5
  0012dc34: blx 0x00076174
  0012dc38: ldr r4,[sp,#0x20]
  0012dc3a: str.w r9,[sp,#0x4]
  0012dc3e: ldr r0,[r4,#0x4]
  0012dc40: str r5,[r0,#0x0]
  0012dc42: movs r0,#0x48
  0012dc44: blx 0x0006eb24
  0012dc48: mov r5,r0
  0012dc4a: mov r1,r10
  0012dc4c: str.w r10,[sp,#0x10]
  0012dc50: blx 0x00076180
  0012dc54: ldr r0,[r4,#0x4]
  0012dc56: mov.w r10,#0x0
  0012dc5a: ldr r6,[sp,#0x1c]
  0012dc5c: mov.w r9,#0x2
  0012dc60: mov r1,r4
  0012dc62: str r5,[r0,#0x4]
  0012dc64: cmp.w r10,#0x4
  0012dc68: bcs.w 0x0012ddaa
  0012dc6c: ldr r0,[sp,#0x10]
  0012dc6e: mov r1,r10
  0012dc70: blx 0x00076150
  0012dc74: cmp r0,#0x1
  0012dc76: blt 0x0012dd50
  0012dc78: bic r0,r10,#0x80000000
  0012dc7c: movw r5,#0x10d
  0012dc80: cmp r0,#0x3
  0012dc82: mov.w r0,#0x48
  0012dc86: it cc
  0012dc88: addw.cc r5,r10,#0x109
  0012dc8c: blx 0x0006eb24
  0012dc90: mov r8,r0
  0012dc92: ldr r0,[sp,#0xc]
  0012dc94: ldr r0,[r0,#0x0]
  0012dc96: mov r1,r5
  0012dc98: blx 0x00072f70
  0012dc9c: mov r1,r0
  0012dc9e: mov r0,r8
  0012dca0: mov r2,r10
  0012dca2: blx 0x0007618c
  0012dca6: ldr r0,[sp,#0x20]
  0012dca8: mov r1,r10
  0012dcaa: str.w r10,[sp,#0x14]
  0012dcae: ldr r0,[r0,#0x4]
  0012dcb0: str.w r8,[r0,r9,lsl #0x2]
  0012dcb4: ldr r0,[sp,#0x10]
  0012dcb6: blx 0x00072118
  0012dcba: mov r8,r0
  0012dcbc: mov.w r10,#0x0
  0012dcc0: str r0,[sp,#0x18]
  0012dcc2: b 0x0012dd32
  0012dcc4: ldr.w r0,[r8,#0x4]
  0012dcc8: mov r5,r11
  0012dcca: ldr.w r11,[r0,r10,lsl #0x2]
  0012dcce: movs r0,#0x48
  0012dcd0: blx 0x0006eb24
  0012dcd4: mov r4,r8
  0012dcd6: mov r8,r0
  0012dcd8: cmp.w r11,#0x0
  0012dcdc: beq 0x0012dcec
  0012dcde: ldr r0,[r4,#0x4]
  0012dce0: ldr.w r1,[r0,r10,lsl #0x2]
  0012dce4: mov r0,r8
  0012dce6: blx 0x00076198
  0012dcea: b 0x0012dcf4
  0012dcec: ldr r1,[sp,#0x14]
  0012dcee: mov r0,r8
  0012dcf0: blx 0x000761a4
  0012dcf4: ldr r4,[sp,#0x20]
  0012dcf6: mov r11,r5
  0012dcf8: ldr r0,[r4,#0x4]
  0012dcfa: str.w r8,[r0,r9,lsl #0x2]
  0012dcfe: movs r0,#0x0
  0012dd00: ldr r1,[r6,#0x0]
  0012dd02: ldr.w r8,[sp,#0x18]
  0012dd06: b 0x0012dd0a
  0012dd08: adds r0,#0x1
  0012dd0a: cmp r0,r1
  0012dd0c: bcs 0x0012dd24
  0012dd0e: ldr r2,[r4,#0x4]
  0012dd10: ldr r3,[r6,#0x4]
  0012dd12: ldr.w r2,[r2,r9,lsl #0x2]
  0012dd16: ldr.w r3,[r3,r0,lsl #0x2]
  0012dd1a: ldr r5,[r2,#0x10]
  0012dd1c: cmp r3,r5
  0012dd1e: bne 0x0012dd08
  0012dd20: str r0,[r2,#0x40]
  0012dd22: b 0x0012dd2a
  0012dd24: ldr r0,[r4,#0x4]
  0012dd26: ldr.w r2,[r0,r9,lsl #0x2]
  0012dd2a: str.w r10,[r2,#0x3c]
  0012dd2e: add.w r10,r10,#0x1
  0012dd32: ldr.w r0,[r8,#0x0]
  0012dd36: add.w r9,r9,#0x1
  0012dd3a: cmp r10,r0
  0012dd3c: bcc 0x0012dcc4
  0012dd3e: mov r0,r8
  0012dd40: blx 0x0007342c
  0012dd44: blx 0x0006eb48
  0012dd48: ldr r0,[sp,#0x8]
  0012dd4a: cbz r0,0x0012dd58
  0012dd4c: ldr.w r10,[sp,#0x14]
  0012dd50: ldr r1,[sp,#0x20]
  0012dd52: add.w r10,r10,#0x1
  0012dd56: b 0x0012dc64
  0012dd58: ldr r1,[sp,#0x20]
  0012dd5a: mov.w r8,#0x0
  0012dd5e: b 0x0012dd9c
  0012dd60: ldr.w r0,[r11,#0x4]
  0012dd64: ldr.w r0,[r0,r8,lsl #0x2]
  0012dd68: blx 0x000718fc
  0012dd6c: ldr r1,[sp,#0x14]
  0012dd6e: cmp r0,r1
  0012dd70: bne 0x0012dd96
  0012dd72: movs r0,#0x48
  0012dd74: blx 0x0006eb24
  0012dd78: mov r5,r0
  0012dd7a: ldr.w r0,[r11,#0x4]
  0012dd7e: ldr.w r1,[r0,r8,lsl #0x2]
  0012dd82: mov r0,r5
  0012dd84: blx 0x00076198
  0012dd88: ldr r1,[sp,#0x20]
  0012dd8a: ldr r0,[r1,#0x4]
  0012dd8c: str.w r5,[r0,r9,lsl #0x2]
  0012dd90: add.w r9,r9,#0x1
  0012dd94: b 0x0012dd98
  0012dd96: ldr r1,[sp,#0x20]
  0012dd98: add.w r8,r8,#0x1
  0012dd9c: ldr.w r0,[r11,#0x0]
  0012dda0: cmp r8,r0
  0012dda2: bcc 0x0012dd60
  0012dda4: ldr.w r10,[sp,#0x14]
  0012dda8: b 0x0012dd52
  0012ddaa: ldr r0,[sp,#0x4]
  0012ddac: ldr r0,[r0,#0x0]
  0012ddae: ldr r0,[r0,#0x4]
  0012ddb0: str r1,[r0,#0x0]
  0012ddb2: add sp,#0x24
  0012ddb4: pop.w {r8,r9,r10,r11}
  0012ddb8: pop {r4,r5,r6,r7,pc}
```
