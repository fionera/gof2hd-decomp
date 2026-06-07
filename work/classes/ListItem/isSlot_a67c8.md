# ListItem::isSlot
elf 0xa67c8 body 14
Sig: undefined __thiscall isSlot(ListItem * this)

## decompile
```c

/* ListItem::isSlot() */

bool __thiscall ListItem::isSlot(ListItem *this)

{
  return *(uint *)(this + 0x28) < 0x80000000;
}

```

## target disasm
```
  000b67c8: ldr r1,[r0,#0x28]
  000b67ca: movs r0,#0x0
  000b67cc: cmp.w r1,#0xffffffff
  000b67d0: it gt
  000b67d2: mov.gt r0,#0x1
  000b67d4: bx lr
```
