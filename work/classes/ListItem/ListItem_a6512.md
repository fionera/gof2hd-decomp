# ListItem::ListItem
elf 0xa6512 body 24
Sig: undefined __thiscall ListItem(ListItem * this, Ship * param_1)

## decompile
```c

/* ListItem::ListItem(Ship*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,Ship *param_1)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  this[0x24] = (ListItem)0x1;
  *(Ship **)(this + 0xc) = param_1;
  return this;
}

```

## target disasm
```
  000b6512: push {r4,r5,r7,lr}
  000b6514: add r7,sp,#0x8
  000b6516: mov r4,r1
  000b6518: mov r5,r0
  000b651a: blx 0x000733cc
  000b651e: movs r0,#0x1
  000b6520: strb.w r0,[r5,#0x24]
  000b6524: mov r0,r5
  000b6526: str r4,[r5,#0xc]
  000b6528: pop {r4,r5,r7,pc}
```
