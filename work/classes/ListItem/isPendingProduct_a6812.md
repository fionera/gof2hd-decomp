# ListItem::isPendingProduct
elf 0xa6812 body 10
Sig: undefined __thiscall isPendingProduct(ListItem * this)

## decompile
```c

/* ListItem::isPendingProduct() */

bool __thiscall ListItem::isPendingProduct(ListItem *this)

{
  return *(int *)(this + 0x18) != 0;
}

```

## target disasm
```
  000b6812: ldr r0,[r0,#0x18]
  000b6814: cmp r0,#0x0
  000b6816: it ne
  000b6818: mov.ne r0,#0x1
  000b681a: bx lr
```
