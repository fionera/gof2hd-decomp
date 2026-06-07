# HangarList::initShopTab
elf 0x11de38 body 554
Sig: undefined __thiscall initShopTab(HangarList * this, Array * param_1, Array * param_2)

## decompile
```c

/* HangarList::initShopTab(Array<Item*>*, Array<Ship*>*) */

void __thiscall HangarList::initShopTab(HangarList *this,Array *param_1,Array *param_2)

{
  Array<ListItem*> *pAVar1;
  void *pvVar2;
  int iVar3;
  int extraout_r0;
  ListItem *pLVar4;
  String *pSVar5;
  uint uVar6;
  int *piVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  int *piVar11;
  int local_40 [6];
  int local_28;
  
  piVar11 = *(int **)(DAT_0012e07c + 0x12de4e);
  local_28 = *piVar11;
  piVar7 = (int *)(*(int *)(*(int *)this + 4) + 4);
  pAVar1 = (Array<ListItem*> *)*piVar7;
  if (pAVar1 != (Array<ListItem*> *)0x0) {
    if (*(int *)pAVar1 != 0) {
      ArrayReleaseClasses<ListItem*>((Array *)pAVar1);
      piVar7 = (int *)(*(int *)(*(int *)this + 4) + 4);
      pAVar1 = (Array<ListItem*> *)*piVar7;
      if (pAVar1 == (Array<ListItem*> *)0x0) goto LAB_0012de80;
    }
    pvVar2 = (void *)Array<ListItem*>::~Array(pAVar1);
    operator_delete(pvVar2);
    piVar7 = (int *)(*(int *)(*(int *)this + 4) + 4);
  }
LAB_0012de80:
  *piVar7 = 0;
  if ((param_1 == (Array *)0x0) || (*(int *)param_1 == 0)) {
    iVar3 = 0;
    if (param_2 != (Array *)0x0) {
      iVar3 = *(int *)param_2;
    }
    if (param_2 == (Array *)0x0 || iVar3 == 0) goto LAB_0012de9a;
  }
  pAVar1 = operator_new(0xc);
  Array<ListItem*>::Array(pAVar1);
  local_40[0] = 0;
  local_40[1] = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  local_40[2] = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  local_40[3] = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  local_40[4] = 0;
  if (param_1 != (Array *)0x0) {
    for (uVar9 = 0; uVar9 < *(uint *)param_1; uVar9 = uVar9 + 1) {
      iVar3 = Item::getType(*(Item **)(*(int *)(param_1 + 4) + uVar9 * 4));
      local_40[iVar3] = local_40[iVar3] + 1;
    }
  }
  iVar3 = 0;
  iVar8 = 0;
  while (iVar8 != 5) {
    piVar7 = local_40 + iVar8;
    iVar8 = iVar8 + 1;
    if (0 < *piVar7) {
      iVar3 = iVar3 + 1;
    }
  }
  iVar8 = 5;
  if (param_2 != (Array *)0x0) {
    iVar8 = *(int *)param_2;
  }
  if (param_2 != (Array *)0x0 && iVar8 != 0) {
    iVar3 = iVar3 + iVar8 + 1;
  }
  if (param_1 == (Array *)0x0) {
    iVar8 = 0;
  }
  else {
    iVar8 = *(int *)param_1;
  }
  ArraySetLength<ListItem*>(iVar3 + iVar8,(Array *)pAVar1);
  iVar3 = extraout_r0;
  if (param_2 != (Array *)0x0) {
    iVar3 = *(int *)param_2;
  }
  if (param_2 != (Array *)0x0 && iVar3 != 0) {
    pLVar4 = operator_new(0x48);
    pSVar5 = (String *)GameText::getText(**(int **)(DAT_0012e080 + 0x12df54));
    ::ListItem::ListItem(pLVar4,pSVar5,-1);
    **(undefined4 **)(pAVar1 + 4) = pLVar4;
    for (uVar9 = 0; uVar9 < *(uint *)param_2; uVar9 = uVar9 + 1) {
      Ship::adjustPrice(*(Ship **)(*(int *)(param_2 + 4) + uVar9 * 4));
      pLVar4 = operator_new(0x48);
      ::ListItem::ListItem(pLVar4,*(Ship **)(*(int *)(param_2 + 4) + uVar9 * 4));
      *(ListItem **)(*(int *)(pAVar1 + 4) + uVar9 * 4 + 4) = pLVar4;
    }
    iVar3 = uVar9 + 1;
  }
  else {
    iVar3 = 0;
  }
  piVar7 = *(int **)(DAT_0012e084 + 0x12dfb8);
  for (uVar9 = 0; uVar9 < 5; uVar9 = uVar9 + 1) {
    if (0 < local_40[uVar9]) {
      pLVar4 = operator_new(0x48);
      pSVar5 = (String *)GameText::getText(*piVar7);
      ::ListItem::ListItem(pLVar4,pSVar5,uVar9);
      *(ListItem **)(*(int *)(pAVar1 + 4) + iVar3 * 4) = pLVar4;
      iVar3 = iVar3 + 1;
      if (param_1 != (Array *)0x0) {
        for (uVar10 = 0; uVar10 < *(uint *)param_1; uVar10 = uVar10 + 1) {
          uVar6 = Item::getType(*(Item **)(*(int *)(param_1 + 4) + uVar10 * 4));
          if (uVar6 == uVar9) {
            pLVar4 = operator_new(0x48);
            ::ListItem::ListItem(pLVar4,*(Item **)(*(int *)(param_1 + 4) + uVar10 * 4));
            *(ListItem **)(*(int *)(pAVar1 + 4) + iVar3 * 4) = pLVar4;
            iVar3 = iVar3 + 1;
          }
        }
      }
    }
  }
  *(Array<ListItem*> **)(*(int *)(*(int *)this + 4) + 4) = pAVar1;
LAB_0012de9a:
  if (*piVar11 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012de38: push {r4,r5,r6,r7,lr}
  0012de3a: add r7,sp,#0xc
  0012de3c: push {r8,r9,r10,r11}
  0012de40: sub sp,#0x2c
  0012de42: mov r4,r0
  0012de44: ldr r0,[0x0012e07c]
  0012de46: mov r5,r1
  0012de48: mov r8,r2
  0012de4a: add r0,pc
  0012de4c: ldr.w r10,[r0,#0x0]
  0012de50: ldr.w r0,[r10,#0x0]
  0012de54: str r0,[sp,#0x28]
  0012de56: ldr r0,[r4,#0x0]
  0012de58: ldr r1,[r0,#0x4]
  0012de5a: ldr.w r0,[r1,#0x4]!
  0012de5e: cbz r0,0x0012de80
  0012de60: ldr r1,[r0,#0x0]
  0012de62: cbz r1,0x0012de72
  0012de64: blx 0x00076108
  0012de68: ldr r0,[r4,#0x0]
  0012de6a: ldr r1,[r0,#0x4]
  0012de6c: ldr.w r0,[r1,#0x4]!
  0012de70: cbz r0,0x0012de80
  0012de72: blx 0x00076114
  0012de76: blx 0x0006eb48
  0012de7a: ldr r0,[r4,#0x0]
  0012de7c: ldr r0,[r0,#0x4]
  0012de7e: adds r1,r0,#0x4
  0012de80: movs r0,#0x0
  0012de82: cmp r5,#0x0
  0012de84: str r0,[r1,#0x0]
  0012de86: beq 0x0012de8c
  0012de88: ldr r0,[r5,#0x0]
  0012de8a: cbnz r0,0x0012deb0
  0012de8c: cmp.w r8,#0x0
  0012de90: itt ne
  0012de92: ldr.ne.w r0,[r8,#0x0]
  0012de96: cmp.ne r0,#0x0
  0012de98: bne 0x0012deb0
  0012de9a: ldr r0,[sp,#0x28]
  0012de9c: ldr.w r1,[r10,#0x0]
  0012dea0: subs r0,r1,r0
  0012dea2: ittt eq
  0012dea4: add.eq sp,#0x2c
  0012dea6: pop.eq.w {r8,r9,r10,r11}
  0012deaa: pop.eq {r4,r5,r6,r7,pc}
  0012deac: blx 0x0006e824
  0012deb0: movs r0,#0xc
  0012deb2: str r4,[sp,#0x8]
  0012deb4: blx 0x0006eb24
  0012deb8: mov r9,r0
  0012deba: blx 0x00076144
  0012debe: add.w r11,sp,#0x10
  0012dec2: vmov.i32 q8,#0x0
  0012dec6: cmp r5,#0x0
  0012dec8: mov.w r1,#0x0
  0012decc: mov r0,r11
  0012dece: vst1.64 {d16,d17},[r0]!
  0012ded2: str r1,[r0,#0x0]
  0012ded4: beq 0x0012def6
  0012ded6: movs r4,#0x0
  0012ded8: b 0x0012def0
  0012deda: ldr r0,[r5,#0x4]
  0012dedc: ldr.w r0,[r0,r4,lsl #0x2]
  0012dee0: blx 0x000718fc
  0012dee4: ldr.w r1,[r11,r0,lsl #0x2]
  0012dee8: adds r4,#0x1
  0012deea: adds r1,#0x1
  0012deec: str.w r1,[r11,r0,lsl #0x2]
  0012def0: ldr r0,[r5,#0x0]
  0012def2: cmp r4,r0
  0012def4: bcc 0x0012deda
  0012def6: movs r0,#0x0
  0012def8: movs r1,#0x0
  0012defa: b 0x0012df08
  0012defc: ldr.w r2,[r11,r1,lsl #0x2]
  0012df00: adds r1,#0x1
  0012df02: cmp r2,#0x0
  0012df04: it gt
  0012df06: add.gt r0,#0x1
  0012df08: cmp r1,#0x5
  0012df0a: bne 0x0012defc
  0012df0c: cmp.w r8,#0x0
  0012df10: itt ne
  0012df12: ldr.ne.w r1,[r8,#0x0]
  0012df16: cmp.ne r1,#0x0
  0012df18: beq 0x0012df1e
  0012df1a: add r0,r1
  0012df1c: adds r0,#0x1
  0012df1e: cmp r5,#0x0
  0012df20: ite ne
  0012df22: ldr.ne r1,[r5,#0x0]
  0012df24: mov.eq r1,#0x0
  0012df26: add r0,r1
  0012df28: mov r1,r9
  0012df2a: blx 0x00076168
  0012df2e: cmp.w r8,#0x0
  0012df32: itt ne
  0012df34: ldr.ne.w r0,[r8,#0x0]
  0012df38: cmp.ne r0,#0x0
  0012df3a: bne 0x0012df46
  0012df3c: str.w r10,[sp,#0x4]
  0012df40: mov.w r10,#0x0
  0012df44: b 0x0012dfb0
  0012df46: movs r0,#0x48
  0012df48: blx 0x0006eb24
  0012df4c: mov r6,r0
  0012df4e: ldr r0,[0x0012e080]
  0012df50: add r0,pc
  0012df52: ldr r0,[r0,#0x0]
  0012df54: ldr r0,[r0,#0x0]
  0012df56: movs r1,#0xad
  0012df58: blx 0x00072f70
  0012df5c: mov r1,r0
  0012df5e: mov r0,r6
  0012df60: mov.w r2,#0xffffffff
  0012df64: blx 0x0007618c
  0012df68: ldr.w r0,[r9,#0x4]
  0012df6c: movs r4,#0x0
  0012df6e: str r6,[r0,#0x0]
  0012df70: b 0x0012dfa0
  0012df72: ldr.w r0,[r8,#0x4]
  0012df76: ldr.w r0,[r0,r4,lsl #0x2]
  0012df7a: blx 0x000719f8
  0012df7e: movs r0,#0x48
  0012df80: blx 0x0006eb24
  0012df84: mov r6,r0
  0012df86: ldr.w r0,[r8,#0x4]
  0012df8a: ldr.w r1,[r0,r4,lsl #0x2]
  0012df8e: mov r0,r6
  0012df90: blx 0x00076180
  0012df94: ldr.w r0,[r9,#0x4]
  0012df98: add.w r0,r0,r4, lsl #0x2
  0012df9c: adds r4,#0x1
  0012df9e: str r6,[r0,#0x4]
  0012dfa0: ldr.w r0,[r8,#0x0]
  0012dfa4: cmp r4,r0
  0012dfa6: bcc 0x0012df72
  0012dfa8: str.w r10,[sp,#0x4]
  0012dfac: add.w r10,r4,#0x1
  0012dfb0: ldr r0,[0x0012e084]
  0012dfb2: movs r4,#0x0
  0012dfb4: add r0,pc
  0012dfb6: ldr.w r8,[r0,#0x0]
  0012dfba: str.w r8,[sp,#0xc]
  0012dfbe: b 0x0012e04e
  0012dfc0: ldr.w r0,[r11,r4,lsl #0x2]
  0012dfc4: cmp r0,#0x1
  0012dfc6: blt 0x0012e04c
  0012dfc8: movs r0,#0x48
  0012dfca: blx 0x0006eb24
  0012dfce: mov.w r1,#0x10e
  0012dfd2: mov r6,r0
  0012dfd4: cmp r4,#0x3
  0012dfd6: it eq
  0012dfd8: movw.eq r1,#0x10d
  0012dfdc: bic r0,r4,#0x80000000
  0012dfe0: cmp r0,#0x3
  0012dfe2: it cc
  0012dfe4: addw.cc r1,r4,#0x109
  0012dfe8: ldr.w r0,[r8,#0x0]
  0012dfec: blx 0x00072f70
  0012dff0: mov r1,r0
  0012dff2: mov r0,r6
  0012dff4: mov r2,r4
  0012dff6: blx 0x0007618c
  0012dffa: ldr.w r0,[r9,#0x4]
  0012dffe: cmp r5,#0x0
  0012e000: str.w r6,[r0,r10,lsl #0x2]
  0012e004: add.w r10,r10,#0x1
  0012e008: beq 0x0012e04c
  0012e00a: mov.w r8,#0x0
  0012e00e: b 0x0012e042
  0012e010: ldr r0,[r5,#0x4]
  0012e012: ldr.w r0,[r0,r8,lsl #0x2]
  0012e016: blx 0x000718fc
  0012e01a: cmp r0,r4
  0012e01c: bne 0x0012e03e
  0012e01e: movs r0,#0x48
  0012e020: blx 0x0006eb24
  0012e024: mov r6,r0
  0012e026: ldr r0,[r5,#0x4]
  0012e028: ldr.w r1,[r0,r8,lsl #0x2]
  0012e02c: mov r0,r6
  0012e02e: blx 0x00076198
  0012e032: ldr.w r0,[r9,#0x4]
  0012e036: str.w r6,[r0,r10,lsl #0x2]
  0012e03a: add.w r10,r10,#0x1
  0012e03e: add.w r8,r8,#0x1
  0012e042: ldr r0,[r5,#0x0]
  0012e044: cmp r8,r0
  0012e046: bcc 0x0012e010
  0012e048: ldr.w r8,[sp,#0xc]
  0012e04c: adds r4,#0x1
  0012e04e: cmp r4,#0x5
  0012e050: bcc 0x0012dfc0
  0012e052: ldr r0,[sp,#0x8]
  0012e054: ldr.w r10,[sp,#0x4]
  0012e058: ldr r0,[r0,#0x0]
  0012e05a: ldr r0,[r0,#0x4]
  0012e05c: str.w r9,[r0,#0x4]
  0012e060: b 0x0012de9a
```
