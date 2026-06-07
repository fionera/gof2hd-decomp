# ListItem::isSelectable
elf 0xa676e body 6
Sig: undefined __thiscall isSelectable(ListItem * this)

## decompile
```c

/* ListItem::isSelectable() */

ListItem __thiscall ListItem::isSelectable(ListItem *this)

{
  return this[0x24];
}

```

## target disasm
```
  000b676e: ldrb.w r0,[r0,#0x24]
  000b6772: bx lr
```
