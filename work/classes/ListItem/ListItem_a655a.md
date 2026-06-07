# ListItem::ListItem
elf 0xa655a body 24
Sig: undefined __thiscall ListItem(ListItem * this, Agent * param_1)

## decompile
```c

/* ListItem::ListItem(Agent*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,Agent *param_1)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  this[0x24] = (ListItem)0x1;
  *(Agent **)(this + 4) = param_1;
  return this;
}

```

## target disasm
```
  000b655a: push {r4,r5,r7,lr}
  000b655c: add r7,sp,#0x8
  000b655e: mov r4,r1
  000b6560: mov r5,r0
  000b6562: blx 0x000733cc
  000b6566: movs r0,#0x1
  000b6568: strb.w r0,[r5,#0x24]
  000b656c: mov r0,r5
  000b656e: str r4,[r5,#0x4]
  000b6570: pop {r4,r5,r7,pc}
```
