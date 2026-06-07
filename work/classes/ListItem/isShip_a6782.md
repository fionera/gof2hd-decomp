# ListItem::isShip
elf 0xa6782 body 10
Sig: undefined __thiscall isShip(ListItem * this)

## decompile
```c

/* ListItem::isShip() */

bool __thiscall ListItem::isShip(ListItem *this)

{
  return *(int *)(this + 0xc) != 0;
}

```

## target disasm
```
  000b6782: ldr r0,[r0,#0xc]
  000b6784: cmp r0,#0x0
  000b6786: it ne
  000b6788: mov.ne r0,#0x1
  000b678a: bx lr
```
