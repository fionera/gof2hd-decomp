# ListItem::isItem
elf 0xa678c body 10
Sig: undefined __thiscall isItem(ListItem * this)

## decompile
```c

/* ListItem::isItem() */

bool __thiscall ListItem::isItem(ListItem *this)

{
  return *(int *)(this + 0x10) != 0;
}

```

## target disasm
```
  000b678c: ldr r0,[r0,#0x10]
  000b678e: cmp r0,#0x0
  000b6790: it ne
  000b6792: mov.ne r0,#0x1
  000b6794: bx lr
```
