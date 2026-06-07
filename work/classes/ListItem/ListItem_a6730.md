# ListItem::ListItem
elf 0xa6730 body 24
Sig: undefined __thiscall ListItem(ListItem * this, Array * param_1)

## decompile
```c

/* ListItem::ListItem(Array<AbyssEngine::String*>*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,Array *param_1)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  this[0x24] = (ListItem)0x0;
  *(Array **)this = param_1;
  return this;
}

```

## target disasm
```
  000b6730: push {r4,r5,r7,lr}
  000b6732: add r7,sp,#0x8
  000b6734: mov r4,r1
  000b6736: mov r5,r0
  000b6738: blx 0x000733cc
  000b673c: movs r0,#0x0
  000b673e: strb.w r0,[r5,#0x24]
  000b6742: mov r0,r5
  000b6744: str r4,[r5,#0x0]
  000b6746: pop {r4,r5,r7,pc}
```
