# ListItem::isTextButton
elf 0xa67d6 body 6
Sig: undefined __thiscall isTextButton(ListItem * this)

## decompile
```c

/* ListItem::isTextButton() */

ListItem __thiscall ListItem::isTextButton(ListItem *this)

{
  return this[0x38];
}

```

## target disasm
```
  000b67d6: ldrb.w r0,[r0,#0x38]
  000b67da: bx lr
```
