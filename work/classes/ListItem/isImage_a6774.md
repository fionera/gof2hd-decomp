# ListItem::isImage
elf 0xa6774 body 14
Sig: undefined __thiscall isImage(ListItem * this)

## decompile
```c

/* ListItem::isImage() */

bool __thiscall ListItem::isImage(ListItem *this)

{
  return *(uint *)(this + 0x34) < 0x80000000;
}

```

## target disasm
```
  000b6774: ldr r1,[r0,#0x34]
  000b6776: movs r0,#0x0
  000b6778: cmp.w r1,#0xffffffff
  000b677c: it gt
  000b677e: mov.gt r0,#0x1
  000b6780: bx lr
```
