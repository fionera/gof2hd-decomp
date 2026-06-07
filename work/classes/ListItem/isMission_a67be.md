# ListItem::isMission
elf 0xa67be body 10
Sig: undefined __thiscall isMission(ListItem * this)

## decompile
```c

/* ListItem::isMission() */

bool __thiscall ListItem::isMission(ListItem *this)

{
  return *(int *)(this + 0x14) != 0;
}

```

## target disasm
```
  000b67be: ldr r0,[r0,#0x14]
  000b67c0: cmp r0,#0x0
  000b67c2: it ne
  000b67c4: mov.ne r0,#0x1
  000b67c6: bx lr
```
