# Ship::getSlotPos
elf 0x17515a body 84
Sig: undefined __thiscall getSlotPos(Ship * this, Item * param_1)

## decompile
```c

/* Ship::getSlotPos(Item*) */

uint __thiscall Ship::getSlotPos(Ship *this,Item *param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  if (param_1 == (Item *)0x0) {
    uVar2 = 0xffffffff;
  }
  else {
    for (uVar2 = 0; uVar2 < **(uint **)(this + 0x6c); uVar2 = uVar2 + 1) {
      if (*(Item **)((*(uint **)(this + 0x6c))[1] + uVar2 * 4) == param_1) goto LAB_00185186;
    }
    uVar2 = 0xffffffff;
LAB_00185186:
    for (iVar3 = 0; iVar1 = Item::getType(param_1), iVar3 < iVar1; iVar3 = iVar3 + 1) {
      uVar2 = uVar2 - *(int *)(*(int *)(this + 0x68) + iVar3 * 4);
    }
  }
  return uVar2;
}

```
## target disasm
```
  0018515a: push {r4,r5,r6,r7,lr}
  0018515c: add r7,sp,#0xc
  0018515e: push.w r8
  00185162: mov r5,r1
  00185164: mov r8,r0
  00185166: cbz r1,0x001851a2
  00185168: ldr.w r0,[r8,#0x6c]
  0018516c: movs r4,#0x0
  0018516e: ldr r1,[r0,#0x0]
  00185170: b 0x0018517e
  00185172: ldr r2,[r0,#0x4]
  00185174: ldr.w r2,[r2,r4,lsl #0x2]
  00185178: cmp r2,r5
  0018517a: beq 0x00185186
  0018517c: adds r4,#0x1
  0018517e: cmp r4,r1
  00185180: bcc 0x00185172
  00185182: mov.w r4,#0xffffffff
  00185186: movs r6,#0x0
  00185188: b 0x00185196
  0018518a: ldr.w r0,[r8,#0x68]
  0018518e: ldr.w r0,[r0,r6,lsl #0x2]
  00185192: adds r6,#0x1
  00185194: subs r4,r4,r0
  00185196: mov r0,r5
  00185198: blx 0x000718fc
  0018519c: cmp r6,r0
  0018519e: blt 0x0018518a
  001851a0: b 0x001851a6
  001851a2: mov.w r4,#0xffffffff
  001851a6: mov r0,r4
  001851a8: pop.w r8
  001851ac: pop {r4,r5,r6,r7,pc}
```
