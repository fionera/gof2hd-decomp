# Item::combineDuplicates
elf 0xe0504 body 130
Sig: undefined __stdcall combineDuplicates(Array * param_1)

## decompile
```c

/* Item::combineDuplicates(Array<Item*>*) */

void Item::combineDuplicates(Array *param_1)

{
  int *piVar1;
  Item *pIVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int *piVar6;
  uint uVar7;
  
  if (param_1 != (Array *)0x0) {
    uVar3 = *(uint *)param_1;
    uVar7 = 0;
    while (uVar4 = uVar7, uVar4 != uVar3) {
      for (uVar5 = uVar4 + 1; uVar7 = uVar4 + 1, uVar3 != uVar5; uVar5 = uVar5 + 1) {
        piVar6 = *(int **)(*(int *)(param_1 + 4) + uVar5 * 4);
        piVar1 = *(int **)(*(int *)(param_1 + 4) + uVar4 * 4);
        if (*piVar1 == *piVar6) {
          piVar1[0xd] = piVar6[0xd] + piVar1[0xd];
          piVar6[0xd] = 0;
          piVar1[0xe] = piVar6[0xe] + piVar1[0xe];
          piVar6[0xe] = 0;
        }
      }
    }
    for (uVar7 = 0; uVar7 < uVar3; uVar7 = uVar7 + 1) {
      pIVar2 = *(Item **)(*(int *)(param_1 + 4) + uVar7 * 4);
      if ((*(int *)(pIVar2 + 0x34) == 0) && (*(int *)(pIVar2 + 0x38) == 0)) {
        ArrayRemove<Item*>(pIVar2,param_1);
        uVar3 = *(uint *)param_1;
      }
    }
  }
  return;
}

```
## target disasm
```
  000f0504: push {r4,r5,r6,r7,lr}
  000f0506: add r7,sp,#0xc
  000f0508: push.w r8
  000f050c: mov r8,r0
  000f050e: cbz r0,0x000f0580
  000f0510: ldr.w r1,[r8,#0x0]
  000f0514: mov.w lr,#0x0
  000f0518: movs r2,#0x0
  000f051a: b 0x000f0558
  000f051c: add.w r12,r2,#0x1
  000f0520: mov r3,r12
  000f0522: b 0x000f0552
  000f0524: ldr.w r0,[r8,#0x4]
  000f0528: ldr.w r5,[r0,r3,lsl #0x2]
  000f052c: ldr.w r0,[r0,r2,lsl #0x2]
  000f0530: ldr r6,[r5,#0x0]
  000f0532: ldr r4,[r0,#0x0]
  000f0534: cmp r4,r6
  000f0536: bne 0x000f0550
  000f0538: ldr r4,[r5,#0x34]
  000f053a: ldr r6,[r0,#0x34]
  000f053c: add r4,r6
  000f053e: str r4,[r0,#0x34]
  000f0540: ldr r4,[r5,#0x38]
  000f0542: str.w lr,[r5,#0x34]
  000f0546: ldr r6,[r0,#0x38]
  000f0548: add r4,r6
  000f054a: str r4,[r0,#0x38]
  000f054c: str.w lr,[r5,#0x38]
  000f0550: adds r3,#0x1
  000f0552: cmp r1,r3
  000f0554: bne 0x000f0524
  000f0556: mov r2,r12
  000f0558: cmp r2,r1
  000f055a: bne 0x000f051c
  000f055c: movs r5,#0x0
  000f055e: b 0x000f057c
  000f0560: ldr.w r0,[r8,#0x4]
  000f0564: ldr.w r0,[r0,r5,lsl #0x2]
  000f0568: ldr r2,[r0,#0x34]
  000f056a: cbnz r2,0x000f057a
  000f056c: ldr r2,[r0,#0x38]
  000f056e: cbnz r2,0x000f057a
  000f0570: mov r1,r8
  000f0572: blx 0x00075a3c
  000f0576: ldr.w r1,[r8,#0x0]
  000f057a: adds r5,#0x1
  000f057c: cmp r5,r1
  000f057e: bcc 0x000f0560
  000f0580: pop.w r8
  000f0584: pop {r4,r5,r6,r7,pc}
```
