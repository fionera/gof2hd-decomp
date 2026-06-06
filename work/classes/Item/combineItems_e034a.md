# Item::combineItems
elf 0xe034a body 312
Sig: undefined __stdcall combineItems(Array * param_1, Array * param_2)

## decompile
```c

/* Item::combineItems(Array<Item*>*, Array<Item*>*) */

Array * Item::combineItems(Array *param_1,Array *param_2)

{
  Array<Item*> *this;
  int iVar1;
  uint uVar2;
  Array<Item*> *this_00;
  Array *this_01;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  uint uVar9;
  
  this_01 = param_2;
  if ((param_1 != (Array *)0x0) && (this_01 = param_1, param_2 != (Array *)0x0)) {
    this = operator_new(0xc);
    Array<Item*>::Array(this);
    ArraySetLength<Item*>(*(uint *)param_2,(Array *)this);
    iVar1 = *(int *)param_2;
    for (iVar3 = 0; iVar1 != iVar3; iVar3 = iVar3 + 1) {
      *(undefined4 *)(*(int *)(this + 4) + iVar3 * 4) =
           *(undefined4 *)(*(int *)(param_2 + 4) + iVar3 * 4);
    }
    uVar4 = *(uint *)this;
    iVar3 = *(int *)param_1;
    uVar9 = uVar4;
    for (iVar1 = 0; iVar1 != iVar3; iVar1 = iVar1 + 1) {
      for (uVar2 = 0; uVar4 != uVar2; uVar2 = uVar2 + 1) {
        iVar5 = *(int *)(this + 4);
        piVar7 = *(int **)(iVar5 + uVar2 * 4);
        if ((piVar7 != (int *)0x0) &&
           (piVar8 = *(int **)(*(int *)(param_1 + 4) + iVar1 * 4), *piVar8 == *piVar7)) {
          uVar9 = uVar9 - 1;
          piVar8[0xd] = piVar7[0xd] + piVar8[0xd];
          *(undefined4 *)(iVar5 + uVar2 * 4) = 0;
        }
      }
    }
    if (0 < (int)uVar9) {
      this_00 = operator_new(0xc);
      Array<Item*>::Array(this_00);
      ArraySetLength<Item*>(uVar9,(Array *)this_00);
      iVar1 = *(int *)this;
      iVar3 = 0;
      for (iVar5 = 0; iVar1 != iVar5; iVar5 = iVar5 + 1) {
        iVar6 = *(int *)(*(int *)(this + 4) + iVar5 * 4);
        if (iVar6 != 0) {
          *(int *)(*(int *)(this_00 + 4) + iVar3 * 4) = iVar6;
          iVar3 = iVar3 + 1;
        }
      }
      this_01 = operator_new(0xc);
      Array<Item*>::Array((Array<Item*> *)this_01);
      ArraySetLength<Item*>(*(int *)param_1 + *(int *)this_00,this_01);
      iVar1 = *(int *)param_1;
      for (iVar3 = 0; iVar1 != iVar3; iVar3 = iVar3 + 1) {
        *(undefined4 *)(*(int *)(this_01 + 4) + iVar3 * 4) =
             *(undefined4 *)(*(int *)(param_1 + 4) + iVar3 * 4);
      }
      iVar3 = *(int *)this_00;
      for (iVar5 = 0; iVar3 != iVar5; iVar5 = iVar5 + 1) {
        *(undefined4 *)(*(int *)(this_01 + 4) + iVar1 * 4 + iVar5 * 4) =
             *(undefined4 *)(*(int *)(this_00 + 4) + iVar5 * 4);
      }
    }
  }
  return this_01;
}

```
## target disasm
```
  000f034a: push {r4,r5,r6,r7,lr}
  000f034c: add r7,sp,#0xc
  000f034e: push {r7,r8,r9,r10,r11}
  000f0352: mov r6,r1
  000f0354: mov r9,r0
  000f0356: cmp r0,#0x0
  000f0358: beq.w 0x000f047a
  000f035c: cmp r6,#0x0
  000f035e: beq.w 0x000f0478
  000f0362: movs r0,#0xc
  000f0364: blx 0x0006eb24
  000f0368: mov r10,r0
  000f036a: blx 0x00071860
  000f036e: ldr r0,[r6,#0x0]
  000f0370: mov r1,r10
  000f0372: blx 0x0007186c
  000f0376: ldr r0,[r6,#0x0]
  000f0378: movs r1,#0x0
  000f037a: b 0x000f038c
  000f037c: ldr r2,[r6,#0x4]
  000f037e: ldr.w r3,[r10,#0x4]
  000f0382: ldr.w r2,[r2,r1,lsl #0x2]
  000f0386: str.w r2,[r3,r1,lsl #0x2]
  000f038a: adds r1,#0x1
  000f038c: cmp r0,r1
  000f038e: bne 0x000f037c
  000f0390: ldr.w r1,[r10,#0x0]
  000f0394: mov.w lr,#0x0
  000f0398: ldr.w r12,[r9,#0x0]
  000f039c: mov.w r8,#0x0
  000f03a0: mov r11,r1
  000f03a2: b 0x000f03dc
  000f03a4: movs r0,#0x0
  000f03a6: b 0x000f03d4
  000f03a8: ldr.w r2,[r10,#0x4]
  000f03ac: ldr.w r4,[r2,r0,lsl #0x2]
  000f03b0: cbz r4,0x000f03d2
  000f03b2: ldr.w r5,[r9,#0x4]
  000f03b6: ldr r6,[r4,#0x0]
  000f03b8: ldr.w r5,[r5,r8,lsl #0x2]
  000f03bc: ldr r3,[r5,#0x0]
  000f03be: cmp r3,r6
  000f03c0: bne 0x000f03d2
  000f03c2: ldr r3,[r4,#0x34]
  000f03c4: sub.w r11,r11,#0x1
  000f03c8: ldr r4,[r5,#0x34]
  000f03ca: add r3,r4
  000f03cc: str r3,[r5,#0x34]
  000f03ce: str.w lr,[r2,r0,lsl #0x2]
  000f03d2: adds r0,#0x1
  000f03d4: cmp r1,r0
  000f03d6: bne 0x000f03a8
  000f03d8: add.w r8,r8,#0x1
  000f03dc: cmp r8,r12
  000f03de: bne 0x000f03a4
  000f03e0: cmp.w r11,#0x0
  000f03e4: ble 0x000f0478
  000f03e6: movs r0,#0xc
  000f03e8: blx 0x0006eb24
  000f03ec: mov r8,r0
  000f03ee: blx 0x00071860
  000f03f2: mov r0,r11
  000f03f4: mov r1,r8
  000f03f6: blx 0x0007186c
  000f03fa: ldr.w r0,[r10,#0x0]
  000f03fe: movs r1,#0x0
  000f0400: movs r2,#0x0
  000f0402: b 0x000f041a
  000f0404: ldr.w r3,[r10,#0x4]
  000f0408: ldr.w r3,[r3,r2,lsl #0x2]
  000f040c: cbz r3,0x000f0418
  000f040e: ldr.w r6,[r8,#0x4]
  000f0412: str.w r3,[r6,r1,lsl #0x2]
  000f0416: adds r1,#0x1
  000f0418: adds r2,#0x1
  000f041a: cmp r0,r2
  000f041c: bne 0x000f0404
  000f041e: movs r0,#0xc
  000f0420: blx 0x0006eb24
  000f0424: mov r6,r0
  000f0426: blx 0x00071860
  000f042a: ldr.w r0,[r9,#0x0]
  000f042e: ldr.w r1,[r8,#0x0]
  000f0432: add r0,r1
  000f0434: mov r1,r6
  000f0436: blx 0x0007186c
  000f043a: ldr.w r0,[r9,#0x0]
  000f043e: movs r1,#0x0
  000f0440: b 0x000f0452
  000f0442: ldr.w r2,[r9,#0x4]
  000f0446: ldr r3,[r6,#0x4]
  000f0448: ldr.w r2,[r2,r1,lsl #0x2]
  000f044c: str.w r2,[r3,r1,lsl #0x2]
  000f0450: adds r1,#0x1
  000f0452: cmp r0,r1
  000f0454: bne 0x000f0442
  000f0456: ldr.w r1,[r8,#0x0]
  000f045a: lsls r0,r0,#0x2
  000f045c: movs r2,#0x0
  000f045e: b 0x000f0472
  000f0460: ldr.w r3,[r8,#0x4]
  000f0464: ldr r5,[r6,#0x4]
  000f0466: ldr.w r3,[r3,r2,lsl #0x2]
  000f046a: add r5,r0
  000f046c: str.w r3,[r5,r2,lsl #0x2]
  000f0470: adds r2,#0x1
  000f0472: cmp r1,r2
  000f0474: bne 0x000f0460
  000f0476: b 0x000f047a
  000f0478: mov r6,r9
  000f047a: mov r0,r6
  000f047c: pop.w {r3,r8,r9,r10,r11}
  000f0480: pop {r4,r5,r6,r7,pc}
```
