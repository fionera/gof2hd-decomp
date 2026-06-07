# HangarList::fillIngredientsList
elf 0x11e5a8 body 332
Sig: undefined __stdcall fillIngredientsList(BluePrint * param_1, bool param_2)

## decompile
```c

/* HangarList::fillIngredientsList(BluePrint*, bool) */

void HangarList::fillIngredientsList(BluePrint *param_1,bool param_2)

{
  uint *puVar1;
  uint *puVar2;
  Array<ListItem*> *this;
  ListItem *pLVar3;
  String *pSVar4;
  int iVar5;
  Item *this_00;
  uint uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  
  Status::getShip();
  puVar1 = (uint *)Ship::getCargo();
  iVar8 = **(int **)(DAT_0012e718 + 0x12e5cc);
  puVar2 = (uint *)BluePrint::getIngredientList((BluePrint *)(uint)param_2);
  uVar6 = *puVar2;
  this = operator_new(0xc);
  Array<ListItem*>::Array(this);
  ArraySetLength<ListItem*>(uVar6 + 1,(Array *)this);
  pLVar3 = operator_new(0x48);
  iVar7 = **(int **)(DAT_0012e71c + 0x12e5fe);
  BluePrint::getIndex((BluePrint *)(uint)param_2);
  pSVar4 = (String *)GameText::getText(iVar7);
  ::ListItem::ListItem(pLVar3,pSVar4);
  uVar6 = 0;
  **(undefined4 **)(this + 4) = pLVar3;
  iVar7 = 1;
  do {
    if (*puVar2 <= uVar6) {
      *(Array<ListItem*> **)(*(int *)(*(int *)param_1 + 4) + 0x10) = this;
      pLVar3 = operator_new(0x48);
      ::ListItem::ListItem(pLVar3,0);
      pLVar3[0x24] = (ListItem)0x0;
      (*(code *)(DAT_001ac3f4 + 0x1ac3f8))
                (pLVar3,*(undefined4 *)(*(int *)(*(int *)param_1 + 4) + 0x10));
      return;
    }
    if (puVar1 != (uint *)0x0) {
      for (uVar9 = 0; uVar9 < *puVar1; uVar9 = uVar9 + 1) {
        iVar5 = Item::getIndex(*(Item **)(puVar1[1] + uVar9 * 4));
        if (iVar5 == *(int *)(puVar2[1] + uVar6 * 4)) {
          pLVar3 = operator_new(0x48);
          ::ListItem::ListItem(pLVar3,*(Item **)(puVar1[1] + uVar9 * 4));
          *(ListItem **)(*(int *)(this + 4) + iVar7 * 4) = pLVar3;
          this_00 = *(Item **)(puVar1[1] + uVar9 * 4);
          goto LAB_0012e6aa;
        }
      }
    }
    pLVar3 = operator_new(0x48);
    ::ListItem::ListItem
              (pLVar3,*(Item **)(*(int *)(iVar8 + 4) + *(int *)(puVar2[1] + uVar6 * 4) * 4));
    *(ListItem **)(*(int *)(this + 4) + iVar7 * 4) = pLVar3;
    this_00 = *(Item **)(*(int *)(iVar8 + 4) + *(int *)(puVar2[1] + uVar6 * 4) * 4);
LAB_0012e6aa:
    Item::setBlueprintAmount(this_00,0);
    uVar6 = uVar6 + 1;
    iVar7 = iVar7 + 1;
  } while( true );
}

```

## target disasm
```
  0012e5a8: push {r4,r5,r6,r7,lr}
  0012e5aa: add r7,sp,#0xc
  0012e5ac: push {r5,r6,r7,r8,r9,r10,r11}
  0012e5b0: str r0,[sp,#0x4]
  0012e5b2: mov r11,r1
  0012e5b4: ldr r0,[0x0012e714]
  0012e5b6: add r0,pc
  0012e5b8: ldr r0,[r0,#0x0]
  0012e5ba: ldr r0,[r0,#0x0]
  0012e5bc: blx 0x00071a58
  0012e5c0: blx 0x00073750
  0012e5c4: mov r5,r0
  0012e5c6: ldr r0,[0x0012e718]
  0012e5c8: add r0,pc
  0012e5ca: ldr r0,[r0,#0x0]
  0012e5cc: ldr.w r8,[r0,#0x0]
  0012e5d0: mov r0,r11
  0012e5d2: blx 0x00073954
  0012e5d6: mov r9,r0
  0012e5d8: ldr r6,[r0,#0x0]
  0012e5da: movs r0,#0xc
  0012e5dc: blx 0x0006eb24
  0012e5e0: mov r4,r0
  0012e5e2: blx 0x00076144
  0012e5e6: adds r0,r6,#0x1
  0012e5e8: mov r1,r4
  0012e5ea: str r4,[sp,#0x8]
  0012e5ec: blx 0x00076168
  0012e5f0: movs r0,#0x48
  0012e5f2: blx 0x0006eb24
  0012e5f6: mov r10,r0
  0012e5f8: ldr r0,[0x0012e71c]
  0012e5fa: add r0,pc
  0012e5fc: ldr r0,[r0,#0x0]
  0012e5fe: ldr r6,[r0,#0x0]
  0012e600: mov r0,r11
  0012e602: blx 0x000733fc
  0012e606: addw r1,r0,#0x4fa
  0012e60a: mov r0,r6
  0012e60c: blx 0x00072f70
  0012e610: mov r1,r0
  0012e612: mov r0,r10
  0012e614: blx 0x00076174
  0012e618: ldr r0,[sp,#0x8]
  0012e61a: movs r4,#0x0
  0012e61c: ldr r0,[r0,#0x4]
  0012e61e: str.w r10,[r0,#0x0]
  0012e622: mov.w r10,#0x1
  0012e626: b 0x0012e6b6
  0012e628: cbz r5,0x0012e674
  0012e62a: mov.w r11,#0x0
  0012e62e: b 0x0012e634
  0012e630: add.w r11,r11,#0x1
  0012e634: ldr r0,[r5,#0x0]
  0012e636: cmp r11,r0
  0012e638: bcs 0x0012e674
  0012e63a: ldr r0,[r5,#0x4]
  0012e63c: ldr.w r0,[r0,r11,lsl #0x2]
  0012e640: blx 0x000718d8
  0012e644: ldr.w r1,[r9,#0x4]
  0012e648: ldr.w r1,[r1,r4,lsl #0x2]
  0012e64c: cmp r0,r1
  0012e64e: bne 0x0012e630
  0012e650: movs r0,#0x48
  0012e652: blx 0x0006eb24
  0012e656: mov r6,r0
  0012e658: ldr r0,[r5,#0x4]
  0012e65a: ldr.w r1,[r0,r11,lsl #0x2]
  0012e65e: mov r0,r6
  0012e660: blx 0x00076198
  0012e664: ldr r0,[sp,#0x8]
  0012e666: ldr r0,[r0,#0x4]
  0012e668: str.w r6,[r0,r10,lsl #0x2]
  0012e66c: ldr r0,[r5,#0x4]
  0012e66e: ldr.w r0,[r0,r11,lsl #0x2]
  0012e672: b 0x0012e6aa
  0012e674: movs r0,#0x48
  0012e676: blx 0x0006eb24
  0012e67a: mov r6,r0
  0012e67c: ldr.w r0,[r9,#0x4]
  0012e680: ldr.w r1,[r8,#0x4]
  0012e684: ldr.w r0,[r0,r4,lsl #0x2]
  0012e688: ldr.w r1,[r1,r0,lsl #0x2]
  0012e68c: mov r0,r6
  0012e68e: blx 0x00076198
  0012e692: ldr r0,[sp,#0x8]
  0012e694: ldr r0,[r0,#0x4]
  0012e696: str.w r6,[r0,r10,lsl #0x2]
  0012e69a: ldr.w r0,[r9,#0x4]
  0012e69e: ldr.w r1,[r8,#0x4]
  0012e6a2: ldr.w r0,[r0,r4,lsl #0x2]
  0012e6a6: ldr.w r0,[r1,r0,lsl #0x2]
  0012e6aa: movs r1,#0x0
  0012e6ac: blx 0x00076234
  0012e6b0: adds r4,#0x1
  0012e6b2: add.w r10,r10,#0x1
  0012e6b6: ldr.w r0,[r9,#0x0]
  0012e6ba: cmp r4,r0
  0012e6bc: bcc 0x0012e628
  0012e6be: ldr r6,[sp,#0x4]
  0012e6c0: ldr r1,[sp,#0x8]
  0012e6c2: ldr r0,[r6,#0x0]
  0012e6c4: ldr r0,[r0,#0x4]
  0012e6c6: str r1,[r0,#0x10]
  0012e6c8: movs r0,#0x48
  0012e6ca: blx 0x0006eb24
  0012e6ce: mov r5,r0
  0012e6d0: movs r1,#0x0
  0012e6d2: movs r4,#0x0
  0012e6d4: blx 0x000761a4
  0012e6d8: strb.w r4,[r5,#0x24]
  0012e6dc: ldr r0,[r6,#0x0]
  0012e6de: ldr r0,[r0,#0x4]
  0012e6e0: ldr r1,[r0,#0x10]
  0012e6e2: mov r0,r5
  0012e6e4: add sp,#0xc
  0012e6e6: pop.w {r8,r9,r10,r11}
  0012e6ea: pop.w {r4,r5,r6,r7,lr}
  0012e6ee: b.w 0x001ac3e8
  001ac3e8: bx pc
```
