# HangarWindow::refreshCargoAvailabilityForBlueprints
elf 0x14900c body 184
Sig: undefined __thiscall refreshCargoAvailabilityForBlueprints(HangarWindow * this)

## decompile
```c

/* HangarWindow::refreshCargoAvailabilityForBlueprints() */

void __thiscall HangarWindow::refreshCargoAvailabilityForBlueprints(HangarWindow *this)

{
  int iVar1;
  uint *puVar2;
  uint *puVar3;
  int iVar4;
  uint *puVar5;
  uint uVar6;
  ListItem *this_00;
  BluePrint *this_01;
  uint uVar7;
  uint uVar8;
  
  iVar1 = HangarList::getItems(*(HangarList **)(this + 0x14));
  puVar5 = *(uint **)(*(int *)(iVar1 + 4) + 8);
  if (puVar5 != (uint *)0x0) {
    for (uVar6 = 0; uVar6 < *puVar5; uVar6 = uVar6 + 1) {
      this_00 = *(ListItem **)(puVar5[1] + uVar6 * 4);
      this_00[0x45] = (ListItem)0x0;
      if ((this_00 != (ListItem *)0x0) && (iVar1 = ::ListItem::isBluePrint(this_00), iVar1 != 0)) {
        this_01 = *(BluePrint **)(this_00 + 8);
        Status::getShip();
        puVar2 = (uint *)Ship::getCargo();
        puVar3 = (uint *)BluePrint::getIngredientList(this_01);
        if (puVar2 != (uint *)0x0) {
          iVar1 = *(int *)this_01;
          for (uVar7 = 0; uVar7 < *puVar3; uVar7 = uVar7 + 1) {
            if (0 < *(int *)(*(int *)(iVar1 + 4) + uVar7 * 4)) {
              uVar8 = 0;
              while (uVar8 < *puVar2) {
                iVar4 = Item::getIndex(*(Item **)(puVar2[1] + uVar8 * 4));
                uVar8 = uVar8 + 1;
                if (iVar4 == *(int *)(puVar3[1] + uVar7 * 4)) {
                  this_00[0x45] = (ListItem)0x1;
                }
              }
            }
          }
        }
      }
    }
  }
  return;
}

```

## target disasm
```
  0015900c: push {r4,r5,r6,r7,lr}
  0015900e: add r7,sp,#0xc
  00159010: push {r5,r6,r7,r8,r9,r10,r11}
  00159014: ldr r0,[r0,#0x14]
  00159016: blx 0x00076fcc
  0015901a: ldr r0,[r0,#0x4]
  0015901c: ldr r1,[r0,#0x8]
  0015901e: cmp r1,#0x0
  00159020: beq 0x001590be
  00159022: ldr r0,[0x001590c4]
  00159024: mov.w r11,#0x1
  00159028: movs r2,#0x0
  0015902a: str r1,[sp,#0x4]
  0015902c: add r0,pc
  0015902e: ldr r0,[r0,#0x0]
  00159030: str r0,[sp,#0x0]
  00159032: b 0x001590b8
  00159034: ldr r0,[r1,#0x4]
  00159036: str r2,[sp,#0x8]
  00159038: ldr.w r4,[r0,r2,lsl #0x2]
  0015903c: movs r0,#0x0
  0015903e: cmp r4,#0x0
  00159040: strb.w r0,[r4,#0x45]
  00159044: beq 0x001590b2
  00159046: mov r0,r4
  00159048: blx 0x000768f4
  0015904c: cbz r0,0x001590b2
  0015904e: ldr r0,[sp,#0x0]
  00159050: ldr.w r8,[r4,#0x8]
  00159054: ldr r0,[r0,#0x0]
  00159056: blx 0x00071a58
  0015905a: blx 0x00073750
  0015905e: mov r5,r0
  00159060: mov r0,r8
  00159062: blx 0x00073954
  00159066: mov r6,r0
  00159068: cbz r5,0x001590b2
  0015906a: ldr.w r8,[r8,#0x0]
  0015906e: mov.w r9,#0x0
  00159072: b 0x001590ac
  00159074: ldr.w r0,[r8,#0x4]
  00159078: ldr.w r0,[r0,r9,lsl #0x2]
  0015907c: cmp r0,#0x1
  0015907e: blt 0x001590a8
  00159080: mov.w r10,#0x0
  00159084: b 0x001590a2
  00159086: ldr r0,[r5,#0x4]
  00159088: ldr.w r0,[r0,r10,lsl #0x2]
  0015908c: blx 0x000718d8
  00159090: ldr r1,[r6,#0x4]
  00159092: add.w r10,r10,#0x1
  00159096: ldr.w r1,[r1,r9,lsl #0x2]
  0015909a: cmp r0,r1
  0015909c: it eq
  0015909e: strb.eq.w r11,[r4,#0x45]
  001590a2: ldr r0,[r5,#0x0]
  001590a4: cmp r10,r0
  001590a6: bcc 0x00159086
  001590a8: add.w r9,r9,#0x1
  001590ac: ldr r0,[r6,#0x0]
  001590ae: cmp r9,r0
  001590b0: bcc 0x00159074
  001590b2: ldr r2,[sp,#0x8]
  001590b4: ldr r1,[sp,#0x4]
  001590b6: adds r2,#0x1
  001590b8: ldr r0,[r1,#0x0]
  001590ba: cmp r2,r0
  001590bc: bcc 0x00159034
  001590be: pop.w {r1,r2,r3,r8,r9,r10,r11}
  001590c2: pop {r4,r5,r6,r7,pc}
```
