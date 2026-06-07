# ListItem::ListItem
elf 0xa64fa body 24
Sig: undefined __thiscall ListItem(ListItem * this, BluePrint * param_1)

## decompile
```c

/* ListItem::ListItem(BluePrint*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,BluePrint *param_1)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  this[0x24] = (ListItem)0x1;
  *(BluePrint **)(this + 8) = param_1;
  return this;
}

```

## target disasm
```
  000b64fa: push {r4,r5,r7,lr}
  000b64fc: add r7,sp,#0x8
  000b64fe: mov r4,r1
  000b6500: mov r5,r0
  000b6502: blx 0x000733cc
  000b6506: movs r0,#0x1
  000b6508: strb.w r0,[r5,#0x24]
  000b650c: mov r0,r5
  000b650e: str r4,[r5,#0x8]
  000b6510: pop {r4,r5,r7,pc}
```
