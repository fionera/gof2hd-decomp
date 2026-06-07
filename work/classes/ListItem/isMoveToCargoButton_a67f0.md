# ListItem::isMoveToCargoButton
elf 0xa67f0 body 24
Sig: undefined __thiscall isMoveToCargoButton(ListItem * this)

## decompile
```c

/* ListItem::isMoveToCargoButton() */

bool __thiscall ListItem::isMoveToCargoButton(ListItem *this)

{
  bool bVar1;
  
  if (this[0x38] == (ListItem)0x0) {
    bVar1 = false;
  }
  else {
    bVar1 = *(int *)(this + 0x30) == 1;
  }
  return bVar1;
}

```

## target disasm
```
  000b67f0: ldrb.w r1,[r0,#0x38]
  000b67f4: cmp r1,#0x0
  000b67f6: iteee eq
  000b67f8: mov.eq r0,#0x0
  000b67fa: ldr.ne r0,[r0,#0x30]
  000b67fc: sub.ne r0,#0x1
  000b67fe: clz.ne r0,r0
  000b6802: it ne
  000b6804: lsr.ne r0,r0,#0x5
  000b6806: bx lr
```
