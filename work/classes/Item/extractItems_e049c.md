# Item::extractItems
elf 0xe049c body 90
Sig: undefined __stdcall extractItems(Array * param_1, bool param_2)

## decompile
```c

/* Item::extractItems(Array<Item*>*, bool) */

Array<Item*> * Item::extractItems(Array *param_1,bool param_2)

{
  Array<Item*> *this;
  int iVar1;
  Item *pIVar2;
  int iVar3;
  uint uVar4;
  
  if (param_1 == (Array *)0x0) {
    this = (Array<Item*> *)0x0;
  }
  else {
    this = operator_new(0xc);
    Array<Item*>::Array(this);
    for (uVar4 = 0; uVar4 < *(uint *)param_1; uVar4 = uVar4 + 1) {
      iVar1 = *(int *)(*(int *)(param_1 + 4) + uVar4 * 4);
      if (param_2) {
        iVar3 = *(int *)(iVar1 + 0x34);
      }
      else {
        iVar3 = *(int *)(iVar1 + 0x38);
      }
      if (0 < iVar3) {
        pIVar2 = (Item *)makeItem(iVar1);
        ArrayAdd<Item*>(pIVar2,(Array *)this);
      }
    }
    if (*(int *)this == 0) {
      this = (Array<Item*> *)0x0;
    }
  }
  return this;
}

```
## target disasm
```
  000f049c: push {r4,r5,r6,r7,lr}
  000f049e: add r7,sp,#0xc
  000f04a0: push.w r8
  000f04a4: mov r5,r1
  000f04a6: mov r6,r0
  000f04a8: cbz r0,0x000f04ea
  000f04aa: movs r0,#0xc
  000f04ac: blx 0x0006eb24
  000f04b0: mov r8,r0
  000f04b2: blx 0x00071860
  000f04b6: movs r4,#0x0
  000f04b8: b 0x000f04d8
  000f04ba: ldr r0,[r6,#0x4]
  000f04bc: cmp r5,#0x0
  000f04be: ldr.w r0,[r0,r4,lsl #0x2]
  000f04c2: ite eq
  000f04c4: ldr.eq r1,[r0,#0x38]
  000f04c6: ldr.ne r1,[r0,#0x34]
  000f04c8: cmp r1,#0x1
  000f04ca: blt 0x000f04d6
  000f04cc: blx 0x000718c0
  000f04d0: mov r1,r8
  000f04d2: blx 0x000718b4
  000f04d6: adds r4,#0x1
  000f04d8: ldr r0,[r6,#0x0]
  000f04da: cmp r4,r0
  000f04dc: bcc 0x000f04ba
  000f04de: ldr.w r0,[r8,#0x0]
  000f04e2: cmp r0,#0x0
  000f04e4: it eq
  000f04e6: mov.eq r8,r0
  000f04e8: b 0x000f04ee
  000f04ea: mov.w r8,#0x0
  000f04ee: mov r0,r8
  000f04f0: pop.w r8
  000f04f4: pop {r4,r5,r6,r7,pc}
```
