# ListItem::isSellButton
elf 0xa67dc body 20
Sig: undefined __thiscall isSellButton(ListItem * this)

## decompile
```c

/* ListItem::isSellButton() */

bool __thiscall ListItem::isSellButton(ListItem *this)

{
  bool bVar1;
  
  if (this[0x38] == (ListItem)0x0) {
    bVar1 = false;
  }
  else {
    bVar1 = *(int *)(this + 0x30) == 0;
  }
  return bVar1;
}

```

## target disasm
```
  000b67dc: ldrb.w r1,[r0,#0x38]
  000b67e0: cmp r1,#0x0
  000b67e2: iteee eq
  000b67e4: mov.eq r0,#0x0
  000b67e6: ldr.ne r0,[r0,#0x30]
  000b67e8: clz.ne r0,r0
  000b67ec: lsr.ne r0,r0,#0x5
  000b67ee: bx lr
```
