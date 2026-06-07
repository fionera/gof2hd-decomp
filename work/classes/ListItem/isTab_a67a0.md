# ListItem::isTab
elf 0xa67a0 body 30
Sig: undefined __thiscall isTab(ListItem * this)

## decompile
```c

/* ListItem::isTab() */

bool __thiscall ListItem::isTab(ListItem *this)

{
  if ((this[0x24] == (ListItem)0x0) && (*(int *)(*(int *)(this + 0x1c) + 8) != 0)) {
    return this[0x38] == (ListItem)0x0;
  }
  return false;
}

```

## target disasm
```
  000b67a0: ldrb.w r1,[r0,#0x24]
  000b67a4: cbz r1,0x000b67aa
  000b67a6: movs r0,#0x0
  000b67a8: bx lr
  000b67aa: ldr r1,[r0,#0x1c]
  000b67ac: ldr r1,[r1,#0x8]
  000b67ae: cmp r1,#0x0
  000b67b0: beq 0x000b67a6
  000b67b2: ldrb.w r0,[r0,#0x38]
  000b67b6: clz r0,r0
  000b67ba: lsrs r0,r0,#0x5
  000b67bc: bx lr
```
