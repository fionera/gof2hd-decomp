# ListItem::isCargo
elf 0xa6796 body 10
Sig: undefined __thiscall isCargo(ListItem * this)

## decompile
```c

/* ListItem::isCargo() */

bool __thiscall ListItem::isCargo(ListItem *this)

{
  return *(int *)(this + 0x10) != 0;
}

```

## target disasm
```
  000b6796: ldr r0,[r0,#0x10]
  000b6798: cmp r0,#0x0
  000b679a: it ne
  000b679c: mov.ne r0,#0x1
  000b679e: bx lr
```
