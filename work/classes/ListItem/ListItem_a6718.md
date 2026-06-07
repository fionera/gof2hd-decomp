# ListItem::ListItem
elf 0xa6718 body 24
Sig: undefined __thiscall ListItem(ListItem * this, Mission * param_1)

## decompile
```c

/* ListItem::ListItem(Mission*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,Mission *param_1)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  this[0x24] = (ListItem)0x1;
  *(Mission **)(this + 0x14) = param_1;
  return this;
}

```

## target disasm
```
  000b6718: push {r4,r5,r7,lr}
  000b671a: add r7,sp,#0x8
  000b671c: mov r4,r1
  000b671e: mov r5,r0
  000b6720: blx 0x000733cc
  000b6724: movs r0,#0x1
  000b6726: strb.w r0,[r5,#0x24]
  000b672a: mov r0,r5
  000b672c: str r4,[r5,#0x14]
  000b672e: pop {r4,r5,r7,pc}
```
