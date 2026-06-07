# ListItem::checkSlot
elf 0xa6870 body 58
Sig: undefined __thiscall checkSlot(ListItem * this)

## decompile
```c

/* ListItem::checkSlot() */

undefined4 __thiscall ListItem::checkSlot(ListItem *this)

{
  Ship *this_00;
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = 0;
  if (*(int *)(this + 0x10) != 0) {
    this_00 = (Ship *)Status::getShip();
    iVar1 = Item::getType(*(Item **)(this + 0x10));
    iVar1 = Ship::getFreeSlots(this_00,iVar1);
    if (0 < iVar1) {
      uVar2 = 1;
    }
  }
  return uVar2;
}

```

## target disasm
```
  000b6870: push {r4,r5,r6,r7,lr}
  000b6872: add r7,sp,#0xc
  000b6874: push.w r11
  000b6878: mov r5,r0
  000b687a: ldr r0,[r0,#0x10]
  000b687c: movs r4,#0x0
  000b687e: cbz r0,0x000b68a2
  000b6880: ldr r0,[0x000b68ac]
  000b6882: add r0,pc
  000b6884: ldr r0,[r0,#0x0]
  000b6886: ldr r0,[r0,#0x0]
  000b6888: blx 0x00071a58
  000b688c: mov r6,r0
  000b688e: ldr r0,[r5,#0x10]
  000b6890: blx 0x000718fc
  000b6894: mov r1,r0
  000b6896: mov r0,r6
  000b6898: blx 0x000733e4
  000b689c: cmp r0,#0x0
  000b689e: it gt
  000b68a0: mov.gt r4,#0x1
  000b68a2: mov r0,r4
  000b68a4: pop.w r11
  000b68a8: pop {r4,r5,r6,r7,pc}
```
