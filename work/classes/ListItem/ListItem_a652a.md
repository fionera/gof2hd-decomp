# ListItem::ListItem
elf 0xa652a body 24
Sig: undefined __thiscall ListItem(ListItem * this, Item * param_1)

## decompile
```c

/* ListItem::ListItem(Item*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,Item *param_1)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  this[0x24] = (ListItem)0x1;
  *(Item **)(this + 0x10) = param_1;
  return this;
}

```

## target disasm
```
  000b652a: push {r4,r5,r7,lr}
  000b652c: add r7,sp,#0x8
  000b652e: mov r4,r1
  000b6530: mov r5,r0
  000b6532: blx 0x000733cc
  000b6536: movs r0,#0x1
  000b6538: strb.w r0,[r5,#0x24]
  000b653c: mov r0,r5
  000b653e: str r4,[r5,#0x10]
  000b6540: pop {r4,r5,r7,pc}
```
