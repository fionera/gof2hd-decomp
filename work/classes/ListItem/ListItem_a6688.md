# ListItem::ListItem
elf 0xa6688 body 24
Sig: undefined __thiscall ListItem(ListItem * this, int param_1)

## decompile
```c

/* ListItem::ListItem(int) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,int param_1)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  this[0x24] = (ListItem)0x1;
  *(int *)(this + 0x28) = param_1;
  return this;
}

```

## target disasm
```
  000b6688: push {r4,r5,r7,lr}
  000b668a: add r7,sp,#0x8
  000b668c: mov r4,r1
  000b668e: mov r5,r0
  000b6690: blx 0x000733cc
  000b6694: movs r0,#0x1
  000b6696: strb.w r0,[r5,#0x24]
  000b669a: mov r0,r5
  000b669c: str r4,[r5,#0x28]
  000b669e: pop {r4,r5,r7,pc}
```
