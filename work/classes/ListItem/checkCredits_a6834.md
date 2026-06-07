# ListItem::checkCredits
elf 0xa6834 body 54
Sig: undefined __thiscall checkCredits(ListItem * this)

## decompile
```c

/* ListItem::checkCredits() */

bool __thiscall ListItem::checkCredits(ListItem *this)

{
  int iVar1;
  int iVar2;
  
  if (*(Ship **)(this + 0xc) == (Ship *)0x0) {
    if (*(Item **)(this + 0x10) == (Item *)0x0) {
      return false;
    }
    iVar1 = Item::getSinglePrice(*(Item **)(this + 0x10));
  }
  else {
    iVar1 = Ship::getPrice(*(Ship **)(this + 0xc));
  }
  iVar2 = Status::getCredits();
  return iVar1 <= iVar2;
}

```

## target disasm
```
  000b6834: push {r4,r6,r7,lr}
  000b6836: add r7,sp,#0x8
  000b6838: mov r1,r0
  000b683a: ldr r0,[r0,#0xc]
  000b683c: cbz r0,0x000b6844
  000b683e: blx 0x00071b00
  000b6842: b 0x000b684c
  000b6844: ldr r0,[r1,#0x10]
  000b6846: cbz r0,0x000b6866
  000b6848: blx 0x00071944
  000b684c: mov r4,r0
  000b684e: ldr r0,[0x000b686c]
  000b6850: add r0,pc
  000b6852: ldr r0,[r0,#0x0]
  000b6854: ldr r0,[r0,#0x0]
  000b6856: blx 0x000733d8
  000b685a: mov r1,r0
  000b685c: movs r0,#0x0
  000b685e: cmp r4,r1
  000b6860: it le
  000b6862: mov.le r0,#0x1
  000b6864: pop {r4,r6,r7,pc}
  000b6866: movs r0,#0x0
  000b6868: pop {r4,r6,r7,pc}
```
