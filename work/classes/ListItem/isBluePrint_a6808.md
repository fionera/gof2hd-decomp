# ListItem::isBluePrint
elf 0xa6808 body 10
Sig: undefined __thiscall isBluePrint(ListItem * this)

## decompile
```c

/* ListItem::isBluePrint() */

bool __thiscall ListItem::isBluePrint(ListItem *this)

{
  return *(int *)(this + 8) != 0;
}

```

## target disasm
```
  000b6808: ldr r0,[r0,#0x8]
  000b680a: cmp r0,#0x0
  000b680c: it ne
  000b680e: mov.ne r0,#0x1
  000b6810: bx lr
```
