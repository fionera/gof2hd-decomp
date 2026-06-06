# Ship::getSlotTypes
elf 0x175390 body 26
Sig: undefined __thiscall getSlotTypes(Ship * this)

## decompile
```c

/* Ship::getSlotTypes() */

int __thiscall Ship::getSlotTypes(Ship *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar3 = 0;
  while (iVar3 != 4) {
    iVar1 = iVar3 * 4;
    iVar3 = iVar3 + 1;
    if (0 < *(int *)(*(int *)(this + 0x68) + iVar1)) {
      iVar2 = iVar2 + 1;
    }
  }
  return iVar2;
}

```
## target disasm
```
  00185390: ldr r1,[r0,#0x68]
  00185392: movs r0,#0x0
  00185394: movs r2,#0x0
  00185396: cmp r2,#0x4
  00185398: it eq
  0018539a: bx.eq lr
  0018539c: ldr.w r3,[r1,r2,lsl #0x2]
  001853a0: adds r2,#0x1
  001853a2: cmp r3,#0x0
  001853a4: it gt
  001853a6: add.gt r0,#0x1
  001853a8: b 0x00185396
```
