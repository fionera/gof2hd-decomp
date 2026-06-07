# ListItem::isText
elf 0xa681c body 6
Sig: undefined __thiscall isText(ListItem * this)

## decompile
```c

/* ListItem::isText() */

ListItem __thiscall ListItem::isText(ListItem *this)

{
  return this[0x44];
}

```

## target disasm
```
  000b681c: ldrb.w r0,[r0,#0x44]
  000b6820: bx lr
```
