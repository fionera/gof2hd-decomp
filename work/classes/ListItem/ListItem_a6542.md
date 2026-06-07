# ListItem::ListItem
elf 0xa6542 body 24
Sig: undefined __thiscall ListItem(ListItem * this, PendingProduct * param_1)

## decompile
```c

/* ListItem::ListItem(PendingProduct*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,PendingProduct *param_1)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  this[0x24] = (ListItem)0x1;
  *(PendingProduct **)(this + 0x18) = param_1;
  return this;
}

```

## target disasm
```
  000b6542: push {r4,r5,r7,lr}
  000b6544: add r7,sp,#0x8
  000b6546: mov r4,r1
  000b6548: mov r5,r0
  000b654a: blx 0x000733cc
  000b654e: movs r0,#0x1
  000b6550: strb.w r0,[r5,#0x24]
  000b6554: mov r0,r5
  000b6556: str r4,[r5,#0x18]
  000b6558: pop {r4,r5,r7,pc}
```
