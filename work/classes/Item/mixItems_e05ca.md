# Item::mixItems
elf 0xe05ca body 456
Sig: undefined __stdcall mixItems(Array * param_1, Array * param_2)

## decompile
```c

/* Item::mixItems(Array<Item*>*, Array<Item*>*) */

Array<Item*> * Item::mixItems(Array *param_1,Array *param_2)

{
  bool bVar1;
  Array<Item*> *this;
  undefined4 uVar2;
  Array<Item*> *this_00;
  void *pvVar3;
  bool bVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int *piVar8;
  uint uVar9;
  int *piVar10;
  
  if (param_1 == (Array *)0x0) {
    uVar9 = 0;
  }
  else {
    uVar9 = *(uint *)param_1;
  }
  if (param_2 == (Array *)0x0) {
    iVar5 = 0;
  }
  else {
    iVar5 = *(int *)param_2;
  }
  this = operator_new(0xc);
  Array<Item*>::Array(this);
  ArraySetLength<Item*>(iVar5 + uVar9,(Array *)this);
  if (((int)uVar9 < 1) || (iVar5 != 0)) {
    if ((uVar9 == 0) && (0 < iVar5)) {
      for (uVar9 = 0; uVar9 < *(uint *)param_2; uVar9 = uVar9 + 1) {
        uVar2 = makeItem(*(int *)(*(int *)(param_2 + 4) + uVar9 * 4));
        *(undefined4 *)(*(int *)(this + 4) + uVar9 * 4) = uVar2;
        *(undefined4 *)(*(int *)(*(int *)(this + 4) + uVar9 * 4) + 0x38) =
             *(undefined4 *)(*(int *)(*(int *)(param_2 + 4) + uVar9 * 4) + 0x34);
      }
    }
    else if (iVar5 == 0 && uVar9 == 0) {
      this = (Array<Item*> *)0x0;
    }
    else {
      for (uVar6 = 0; uVar6 < *(uint *)param_1; uVar6 = uVar6 + 1) {
        uVar2 = makeItem(*(int *)(*(int *)(param_1 + 4) + uVar6 * 4));
        *(undefined4 *)(*(int *)(this + 4) + uVar6 * 4) = uVar2;
      }
      for (uVar6 = 0; uVar6 < *(uint *)param_2; uVar6 = uVar6 + 1) {
        for (uVar7 = 0; uVar7 < *(uint *)this; uVar7 = uVar7 + 1) {
          piVar10 = *(int **)(*(int *)(this + 4) + uVar7 * 4);
          piVar8 = *(int **)(*(int *)(param_2 + 4) + uVar6 * 4);
          if (piVar10 == (int *)0x0) {
            uVar2 = makeItem((int)piVar8);
            iVar5 = *(int *)(this + 4);
            uVar9 = uVar9 + 1;
LAB_000f06e0:
            *(undefined4 *)(iVar5 + uVar7 * 4) = uVar2;
            *(undefined4 *)(*(int *)(*(int *)(this + 4) + uVar7 * 4) + 0x38) =
                 *(undefined4 *)(*(int *)(*(int *)(param_2 + 4) + uVar6 * 4) + 0x34);
            break;
          }
          if (*piVar8 == *piVar10) {
            uVar2 = makeItem((int)piVar8);
            iVar5 = *(int *)(this + 4);
            goto LAB_000f06e0;
          }
        }
      }
      this_00 = operator_new(0xc);
      Array<Item*>::Array(this_00);
      ArraySetLength<Item*>(uVar9,(Array *)this_00);
      for (iVar5 = 0; iVar5 < (int)uVar9; iVar5 = iVar5 + 1) {
        *(undefined4 *)(*(int *)(this_00 + 4) + iVar5 * 4) =
             *(undefined4 *)(*(int *)(this + 4) + iVar5 * 4);
      }
      pvVar3 = (void *)Array<Item*>::~Array(this);
      operator_delete(pvVar3);
      uVar6 = *(uint *)this_00;
      bVar4 = true;
      uVar9 = 1;
      do {
        for (; uVar9 < uVar6; uVar9 = uVar9 + 1) {
          iVar5 = *(int *)(this_00 + 4);
          piVar10 = *(int **)(iVar5 + uVar9 * 4);
          piVar8 = *(int **)(iVar5 + (uVar9 - 1) * 4);
          if (*piVar10 < *piVar8) {
            *(int **)(iVar5 + (uVar9 - 1) * 4) = piVar10;
            bVar4 = false;
            *(int **)(*(int *)(this_00 + 4) + uVar9 * 4) = piVar8;
          }
        }
        bVar1 = !bVar4;
        uVar9 = 1;
        bVar4 = true;
      } while (bVar1);
      combineDuplicates((Array *)this_00);
      this = this_00;
    }
  }
  else {
    for (uVar9 = 0; uVar9 < *(uint *)param_1; uVar9 = uVar9 + 1) {
      uVar2 = makeItem(*(int *)(*(int *)(param_1 + 4) + uVar9 * 4));
      *(undefined4 *)(*(int *)(this + 4) + uVar9 * 4) = uVar2;
    }
  }
  return this;
}

```
## target disasm
```
  000f05ca: push {r4,r5,r6,r7,lr}
  000f05cc: add r7,sp,#0xc
  000f05ce: push {r8,r9,r10}
  000f05d2: mov r9,r1
  000f05d4: mov r5,r0
  000f05d6: cmp r0,#0x0
  000f05d8: ite ne
  000f05da: ldr.ne.w r8,[r5,#0x0]
  000f05de: mov.eq.w r8,#0x0
  000f05e2: cmp.w r9,#0x0
  000f05e6: ite ne
  000f05e8: ldr.ne.w r6,[r9,#0x0]
  000f05ec: mov.eq r6,#0x0
  000f05ee: movs r0,#0xc
  000f05f0: blx 0x0006eb24
  000f05f4: mov r10,r0
  000f05f6: blx 0x00071860
  000f05fa: add.w r0,r6,r8
  000f05fe: mov r1,r10
  000f0600: blx 0x0007186c
  000f0604: cmp.w r8,#0x1
  000f0608: blt 0x000f062e
  000f060a: cbnz r6,0x000f062e
  000f060c: movs r6,#0x0
  000f060e: b 0x000f0626
  000f0610: ldr r0,[r5,#0x4]
  000f0612: ldr.w r0,[r0,r6,lsl #0x2]
  000f0616: ldr r1,[r0,#0x34]
  000f0618: blx 0x000718c0
  000f061c: ldr.w r1,[r10,#0x4]
  000f0620: str.w r0,[r1,r6,lsl #0x2]
  000f0624: adds r6,#0x1
  000f0626: ldr r0,[r5,#0x0]
  000f0628: cmp r6,r0
  000f062a: bcc 0x000f0610
  000f062c: b 0x000f078a
  000f062e: cmp.w r8,#0x0
  000f0632: bne 0x000f0672
  000f0634: cmp r6,#0x1
  000f0636: blt 0x000f0672
  000f0638: movs r5,#0x0
  000f063a: b 0x000f0668
  000f063c: ldr.w r0,[r9,#0x4]
  000f0640: movs r1,#0x0
  000f0642: ldr.w r0,[r0,r5,lsl #0x2]
  000f0646: blx 0x000718c0
  000f064a: ldr.w r1,[r10,#0x4]
  000f064e: str.w r0,[r1,r5,lsl #0x2]
  000f0652: ldr.w r0,[r9,#0x4]
  000f0656: ldr.w r1,[r10,#0x4]
  000f065a: ldr.w r0,[r0,r5,lsl #0x2]
  000f065e: ldr.w r1,[r1,r5,lsl #0x2]
  000f0662: adds r5,#0x1
  000f0664: ldr r0,[r0,#0x34]
  000f0666: str r0,[r1,#0x38]
  000f0668: ldr.w r0,[r9,#0x0]
  000f066c: cmp r5,r0
  000f066e: bcc 0x000f063c
  000f0670: b 0x000f078a
  000f0672: orrs.w r0,r6,r8
  000f0676: beq.w 0x000f0786
  000f067a: movs r6,#0x0
  000f067c: b 0x000f0694
  000f067e: ldr r0,[r5,#0x4]
  000f0680: ldr.w r0,[r0,r6,lsl #0x2]
  000f0684: ldr r1,[r0,#0x34]
  000f0686: blx 0x000718c0
  000f068a: ldr.w r1,[r10,#0x4]
  000f068e: str.w r0,[r1,r6,lsl #0x2]
  000f0692: adds r6,#0x1
  000f0694: ldr r0,[r5,#0x0]
  000f0696: cmp r6,r0
  000f0698: bcc 0x000f067e
  000f069a: movs r5,#0x0
  000f069c: b 0x000f06fa
  000f069e: ldr.w r1,[r10,#0x0]
  000f06a2: movs r6,#0x0
  000f06a4: b 0x000f06a8
  000f06a6: adds r6,#0x1
  000f06a8: cmp r6,r1
  000f06aa: bcs 0x000f06f8
  000f06ac: ldr.w r0,[r10,#0x4]
  000f06b0: ldr.w r3,[r9,#0x4]
  000f06b4: ldr.w r2,[r0,r6,lsl #0x2]
  000f06b8: ldr.w r0,[r3,r5,lsl #0x2]
  000f06bc: cbz r2,0x000f06d2
  000f06be: ldr r3,[r2,#0x0]
  000f06c0: ldr r4,[r0,#0x0]
  000f06c2: cmp r4,r3
  000f06c4: bne 0x000f06a6
  000f06c6: ldr r1,[r2,#0x34]
  000f06c8: blx 0x000718c0
  000f06cc: ldr.w r1,[r10,#0x4]
  000f06d0: b 0x000f06e0
  000f06d2: movs r1,#0x0
  000f06d4: blx 0x000718c0
  000f06d8: ldr.w r1,[r10,#0x4]
  000f06dc: add.w r8,r8,#0x1
  000f06e0: str.w r0,[r1,r6,lsl #0x2]
  000f06e4: ldr.w r0,[r9,#0x4]
  000f06e8: ldr.w r1,[r10,#0x4]
  000f06ec: ldr.w r0,[r0,r5,lsl #0x2]
  000f06f0: ldr.w r1,[r1,r6,lsl #0x2]
  000f06f4: ldr r0,[r0,#0x34]
  000f06f6: str r0,[r1,#0x38]
  000f06f8: adds r5,#0x1
  000f06fa: ldr.w r0,[r9,#0x0]
  000f06fe: cmp r5,r0
  000f0700: bcc 0x000f069e
  000f0702: movs r0,#0xc
  000f0704: blx 0x0006eb24
  000f0708: mov r9,r0
  000f070a: blx 0x00071860
  000f070e: mov r0,r8
  000f0710: mov r1,r9
  000f0712: blx 0x0007186c
  000f0716: movs r2,#0x0
  000f0718: b 0x000f072c
  000f071a: ldr.w r0,[r10,#0x4]
  000f071e: ldr.w r1,[r9,#0x4]
  000f0722: ldr.w r0,[r0,r2,lsl #0x2]
  000f0726: str.w r0,[r1,r2,lsl #0x2]
  000f072a: adds r2,#0x1
  000f072c: cmp r2,r8
  000f072e: blt 0x000f071a
  000f0730: mov r0,r10
  000f0732: blx 0x0007342c
  000f0736: blx 0x0006eb48
  000f073a: ldr.w r12,[r9,#0x0]
  000f073e: movs r1,#0x1
  000f0740: movs r2,#0x1
  000f0742: cmp r2,r12
  000f0744: bcs 0x000f0770
  000f0746: ldr.w r3,[r9,#0x4]
  000f074a: subs r5,r2,#0x1
  000f074c: ldr.w lr,[r3,r2,lsl #0x2]
  000f0750: ldr.w r6,[r3,r5,lsl #0x2]
  000f0754: ldr.w r0,[lr,#0x0]
  000f0758: ldr r4,[r6,#0x0]
  000f075a: cmp r4,r0
  000f075c: ble 0x000f076c
  000f075e: str.w lr,[r3,r5,lsl #0x2]
  000f0762: movs r1,#0x0
  000f0764: ldr.w r0,[r9,#0x4]
  000f0768: str.w r6,[r0,r2,lsl #0x2]
  000f076c: adds r2,#0x1
  000f076e: b 0x000f0742
  000f0770: lsls r1,r1,#0x1f
  000f0772: mov.w r2,#0x1
  000f0776: mov.w r1,#0x1
  000f077a: beq 0x000f0742
  000f077c: mov r0,r9
  000f077e: blx 0x00075a48
  000f0782: mov r10,r9
  000f0784: b 0x000f078a
  000f0786: mov.w r10,#0x0
  000f078a: mov r0,r10
  000f078c: pop.w {r8,r9,r10}
  000f0790: pop {r4,r5,r6,r7,pc}
```
