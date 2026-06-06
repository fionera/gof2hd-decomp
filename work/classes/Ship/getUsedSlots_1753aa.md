# Ship::getUsedSlots
elf 0x1753aa body 54
Sig: undefined __thiscall getUsedSlots(Ship * this, int param_1)

## decompile
```c

/* Ship::getUsedSlots(int) */

int __thiscall Ship::getUsedSlots(Ship *this,int param_1)

{
  Item *this_00;
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = 0;
  for (uVar2 = 0; uVar2 < **(uint **)(this + 0x6c); uVar2 = uVar2 + 1) {
    this_00 = *(Item **)((*(uint **)(this + 0x6c))[1] + uVar2 * 4);
    if ((this_00 != (Item *)0x0) && (iVar1 = Item::getType(this_00), iVar1 == param_1)) {
      iVar3 = iVar3 + 1;
    }
  }
  return iVar3;
}

```
## target disasm
```
  001853aa: push {r4,r5,r6,r7,lr}
  001853ac: add r7,sp,#0xc
  001853ae: push.w r8
  001853b2: mov r8,r1
  001853b4: mov r6,r0
  001853b6: movs r5,#0x0
  001853b8: movs r4,#0x0
  001853ba: b 0x001853d0
  001853bc: ldr r0,[r0,#0x4]
  001853be: ldr.w r0,[r0,r4,lsl #0x2]
  001853c2: cbz r0,0x001853ce
  001853c4: blx 0x000718fc
  001853c8: cmp r0,r8
  001853ca: it eq
  001853cc: add.eq r5,#0x1
  001853ce: adds r4,#0x1
  001853d0: ldr r0,[r6,#0x6c]
  001853d2: ldr r1,[r0,#0x0]
  001853d4: cmp r4,r1
  001853d6: bcc 0x001853bc
  001853d8: mov r0,r5
  001853da: pop.w r8
  001853de: pop {r4,r5,r6,r7,pc}
```
