# ListItem::checkSort
elf 0xa68b0 body 48
Sig: undefined __thiscall checkSort(ListItem * this)

## decompile
```c

/* ListItem::checkSort() */

undefined4 __thiscall ListItem::checkSort(ListItem *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  if (*(int *)(this + 0x10) == 0) {
    return 0;
  }
  uVar1 = Status::getShip();
  uVar2 = Item::getSort(*(Item **)(this + 0x10));
  uVar1 = (*(code *)(DAT_001abe34 + 0x1abe38))(uVar1,uVar2);
  return uVar1;
}

```

## target disasm
```
  000b68b0: push {r4,r5,r7,lr}
  000b68b2: add r7,sp,#0x8
  000b68b4: mov r4,r0
  000b68b6: ldr r0,[r0,#0x10]
  000b68b8: cbz r0,0x000b68da
  000b68ba: ldr r0,[0x000b68e0]
  000b68bc: add r0,pc
  000b68be: ldr r0,[r0,#0x0]
  000b68c0: ldr r0,[r0,#0x0]
  000b68c2: blx 0x00071a58
  000b68c6: mov r5,r0
  000b68c8: ldr r0,[r4,#0x10]
  000b68ca: blx 0x00071908
  000b68ce: mov r1,r0
  000b68d0: mov r0,r5
  000b68d2: pop.w {r4,r5,r7,lr}
  000b68d6: b.w 0x001abe28
  000b68da: movs r0,#0x0
  000b68dc: pop {r4,r5,r7,pc}
  001abe28: bx pc
```
