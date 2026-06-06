# Item::isInList
elf 0xe026e body 50
Sig: undefined __stdcall isInList(int param_1, int param_2, Array * param_3)

## decompile
```c

/* Item::isInList(int, int, Array<Item*>*) */

undefined4 Item::isInList(int param_1,int param_2,Array *param_3)

{
  uint uVar1;
  int *piVar2;
  
  if (param_3 != (Array *)0x0) {
    for (uVar1 = 0; uVar1 < *(uint *)param_3; uVar1 = uVar1 + 1) {
      piVar2 = *(int **)(*(int *)(param_3 + 4) + uVar1 * 4);
      if ((*piVar2 == param_1) && (param_2 <= piVar2[0xd])) {
        return 1;
      }
    }
  }
  return 0;
}

```
## target disasm
```
  000f026e: push {r4,r6,r7,lr}
  000f0270: add r7,sp,#0x8
  000f0272: cbz r2,0x000f0298
  000f0274: ldr.w r12,[r2,#0x0]
  000f0278: movs r4,#0x0
  000f027a: b 0x000f0294
  000f027c: ldr r3,[r2,#0x4]
  000f027e: ldr.w lr,[r3,r4,lsl #0x2]
  000f0282: ldr.w r3,[lr,#0x0]
  000f0286: cmp r3,r0
  000f0288: bne 0x000f0292
  000f028a: ldr.w r3,[lr,#0x34]
  000f028e: cmp r3,r1
  000f0290: bge 0x000f029c
  000f0292: adds r4,#0x1
  000f0294: cmp r4,r12
  000f0296: bcc 0x000f027c
  000f0298: movs r0,#0x0
  000f029a: pop {r4,r6,r7,pc}
  000f029c: movs r0,#0x1
  000f029e: pop {r4,r6,r7,pc}
```
