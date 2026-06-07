# SystemPathFinder::getSystemPath
elf 0x11b7a8 body 334
Sig: undefined __thiscall getSystemPath(SystemPathFinder * this, Array * param_1, int param_2, int param_3)

## decompile
```c

/* SystemPathFinder::getSystemPath(Array<SolarSystem*>*, int, int) */

Array * __thiscall
SystemPathFinder::getSystemPath(SystemPathFinder *this,Array *param_1,int param_2,int param_3)

{
  Array<Node*> *this_00;
  Node *this_01;
  uint uVar1;
  uint *puVar2;
  uint *puVar3;
  uint uVar4;
  int iVar5;
  Array *this_02;
  Array *pAVar6;
  int *piVar7;
  void *pvVar8;
  uint uVar9;
  undefined4 *puVar10;
  uint uVar11;
  
  this_00 = operator_new(0xc);
  Array<Node*>::Array(this_00);
  ArraySetLength<Node*>(*(uint *)param_1,(Array *)this_00);
  for (uVar9 = 0; uVar1 = *(uint *)param_1, uVar9 < uVar1; uVar9 = uVar9 + 1) {
    this_01 = operator_new(0xc);
    Node::Node(this_01,uVar9);
    *(Node **)(*(int *)(this_00 + 4) + uVar9 * 4) = this_01;
  }
  puVar10 = *(undefined4 **)(DAT_0012b90c + 0x12b800);
  for (uVar9 = 0; uVar9 < uVar1; uVar9 = uVar9 + 1) {
    puVar2 = (uint *)SolarSystem::getRoutes();
    if (puVar2 != (uint *)0x0) {
      for (uVar1 = 0; uVar1 < *puVar2; uVar1 = uVar1 + 1) {
        puVar3 = (uint *)Status::getSystemVisibilities((Status *)*puVar10);
        if (((puVar3 != (uint *)0x0) &&
            (uVar11 = *puVar3, uVar4 = SolarSystem::getIndex(), uVar4 < uVar11)) &&
           (iVar5 = SolarSystem::getIndex(), *(char *)(puVar3[1] + iVar5) != '\0')) {
          ArrayAdd<Node*>(*(Node **)(*(int *)(this_00 + 4) + *(int *)(puVar2[1] + uVar1 * 4) * 4),
                          (Array *)**(undefined4 **)(*(int *)(this_00 + 4) + uVar9 * 4));
        }
      }
    }
    uVar1 = *(uint *)param_1;
  }
  this_02 = (Array *)search(this,*(Node **)(*(int *)(this_00 + 4) + param_2 * 4),
                            *(Node **)(*(int *)(this_00 + 4) + param_3 * 4));
  if (this_02 == (Array *)0x0) {
    pAVar6 = (Array *)0x0;
  }
  else {
    if (*(int *)this_02 == 0) {
      pAVar6 = (Array *)0x0;
    }
    else {
      pAVar6 = operator_new(0xc);
      Array<int>::Array();
      ArraySetLength<int>(*(int *)this_02 + 1,pAVar6);
      piVar7 = *(int **)(pAVar6 + 4);
      *piVar7 = param_2;
      uVar9 = 0;
      while (uVar9 + 1 < *(uint *)pAVar6) {
        piVar7[uVar9 + 1] = *(int *)(*(int *)(*(int *)(this_02 + 4) + uVar9 * 4) + 8);
        uVar9 = uVar9 + 1;
      }
    }
    ArrayReleaseClasses<Node*>(this_02);
    pvVar8 = (void *)Array<Node*>::~Array((Array<Node*> *)this_02);
    operator_delete(pvVar8);
  }
  return pAVar6;
}

```

## target disasm
```
  0012b7a8: push {r4,r5,r6,r7,lr}
  0012b7aa: add r7,sp,#0xc
  0012b7ac: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0012b7b0: mov r9,r0
  0012b7b2: movs r0,#0xc
  0012b7b4: str r3,[sp,#0xc]
  0012b7b6: mov r8,r2
  0012b7b8: mov r4,r1
  0012b7ba: blx 0x0006eb24
  0012b7be: mov r10,r0
  0012b7c0: blx 0x00075ee0
  0012b7c4: ldr r0,[r4,#0x0]
  0012b7c6: mov r1,r10
  0012b7c8: blx 0x00075ef8
  0012b7cc: movs r6,#0x0
  0012b7ce: b 0x0012b7e8
  0012b7d0: movs r0,#0xc
  0012b7d2: blx 0x0006eb24
  0012b7d6: mov r5,r0
  0012b7d8: mov r1,r6
  0012b7da: blx 0x00075f04
  0012b7de: ldr.w r0,[r10,#0x4]
  0012b7e2: str.w r5,[r0,r6,lsl #0x2]
  0012b7e6: adds r6,#0x1
  0012b7e8: ldr r0,[r4,#0x0]
  0012b7ea: cmp r6,r0
  0012b7ec: bcc 0x0012b7d0
  0012b7ee: strd r9,r8,[sp,#0x4]
  0012b7f2: mov.w r8,#0x0
  0012b7f6: ldr r1,[0x0012b90c]
  0012b7f8: str.w r10,[sp,#0x10]
  0012b7fc: add r1,pc
  0012b7fe: ldr.w r9,[r1,#0x0]
  0012b802: b 0x0012b87e
  0012b804: ldr r0,[r4,#0x4]
  0012b806: ldr.w r0,[r0,r8,lsl #0x2]
  0012b80a: blx 0x00071aac
  0012b80e: mov r6,r0
  0012b810: cbz r0,0x0012b874
  0012b812: mov.w r10,#0x0
  0012b816: b 0x0012b86e
  0012b818: ldr.w r0,[r9,#0x0]
  0012b81c: blx 0x00071aa0
  0012b820: mov r5,r0
  0012b822: cbz r0,0x0012b86a
  0012b824: ldr r0,[r6,#0x4]
  0012b826: ldr r1,[r4,#0x4]
  0012b828: ldr.w r11,[r5,#0x0]
  0012b82c: ldr.w r0,[r0,r10,lsl #0x2]
  0012b830: ldr.w r0,[r1,r0,lsl #0x2]
  0012b834: blx 0x00071a7c
  0012b838: cmp r11,r0
  0012b83a: bls 0x0012b86a
  0012b83c: ldr r0,[r6,#0x4]
  0012b83e: ldr r1,[r4,#0x4]
  0012b840: ldr.w r0,[r0,r10,lsl #0x2]
  0012b844: ldr.w r0,[r1,r0,lsl #0x2]
  0012b848: blx 0x00071a7c
  0012b84c: ldr r1,[r5,#0x4]
  0012b84e: ldrb r0,[r1,r0]
  0012b850: cbz r0,0x0012b86a
  0012b852: ldr r1,[sp,#0x10]
  0012b854: ldr r0,[r6,#0x4]
  0012b856: ldr r1,[r1,#0x4]
  0012b858: ldr.w r0,[r0,r10,lsl #0x2]
  0012b85c: ldr.w r2,[r1,r8,lsl #0x2]
  0012b860: ldr.w r0,[r1,r0,lsl #0x2]
  0012b864: ldr r1,[r2,#0x0]
  0012b866: blx 0x00075f10
  0012b86a: add.w r10,r10,#0x1
  0012b86e: ldr r0,[r6,#0x0]
  0012b870: cmp r10,r0
  0012b872: bcc 0x0012b818
  0012b874: ldr r0,[r4,#0x0]
  0012b876: add.w r8,r8,#0x1
  0012b87a: ldr.w r10,[sp,#0x10]
  0012b87e: cmp r8,r0
  0012b880: bcc 0x0012b804
  0012b882: ldr.w r0,[r10,#0x4]
  0012b886: ldr r1,[sp,#0xc]
  0012b888: ldr r6,[sp,#0x8]
  0012b88a: ldr.w r2,[r0,r1,lsl #0x2]
  0012b88e: ldr.w r1,[r0,r6,lsl #0x2]
  0012b892: ldr r0,[sp,#0x4]
  0012b894: blx 0x00075f1c
  0012b898: mov r4,r0
  0012b89a: cbz r0,0x0012b8d6
  0012b89c: ldr r0,[r4,#0x0]
  0012b89e: cbz r0,0x0012b8da
  0012b8a0: movs r0,#0xc
  0012b8a2: blx 0x0006eb24
  0012b8a6: mov r5,r0
  0012b8a8: blx 0x000701f8
  0012b8ac: ldr r0,[r4,#0x0]
  0012b8ae: mov r1,r5
  0012b8b0: adds r0,#0x1
  0012b8b2: blx 0x00071a4c
  0012b8b6: ldr r0,[r5,#0x4]
  0012b8b8: movs r1,#0x0
  0012b8ba: stmia r0!,{r6}
  0012b8bc: b 0x0012b8cc
  0012b8be: ldr r3,[r4,#0x4]
  0012b8c0: ldr.w r3,[r3,r1,lsl #0x2]
  0012b8c4: ldr r3,[r3,#0x8]
  0012b8c6: str.w r3,[r0,r1,lsl #0x2]
  0012b8ca: mov r1,r2
  0012b8cc: ldr r3,[r5,#0x0]
  0012b8ce: adds r2,r1,#0x1
  0012b8d0: cmp r2,r3
  0012b8d2: bcc 0x0012b8be
  0012b8d4: b 0x0012b8dc
  0012b8d6: movs r5,#0x0
  0012b8d8: b 0x0012b8ec
  0012b8da: movs r5,#0x0
  0012b8dc: mov r0,r4
  0012b8de: blx 0x00075f28
  0012b8e2: mov r0,r4
  0012b8e4: blx 0x00075f34
  0012b8e8: blx 0x0006eb48
  0012b8ec: mov r0,r5
  0012b8ee: add sp,#0x14
  0012b8f0: pop.w {r8,r9,r10,r11}
  0012b8f4: pop {r4,r5,r6,r7,pc}
```
