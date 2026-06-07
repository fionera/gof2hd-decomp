# ListItem::ListItem
elf 0xa66f4 body 36
Sig: undefined __thiscall ListItem(ListItem * this, int param_1, int param_2)

## decompile
```c

/* ListItem::ListItem(int, int) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,int param_1,int param_2)

{
  init(this,(EVP_PKEY_CTX *)param_1);
  *(int *)(this + 0x28) = param_2;
  *(int *)(this + 0x2c) = param_1;
  this[0x24] = (ListItem)0x1;
  return this;
}

```

## target disasm
```
  000b66f4: push {r4,r5,r6,r7,lr}
  000b66f6: add r7,sp,#0xc
  000b66f8: push.w r11
  000b66fc: mov r4,r2
  000b66fe: mov r5,r1
  000b6700: mov r6,r0
  000b6702: blx 0x000733cc
  000b6706: movs r0,#0x1
  000b6708: strd r4,r5,[r6,#0x28]
  000b670c: strb.w r0,[r6,#0x24]
  000b6710: mov r0,r6
  000b6712: pop.w r11
  000b6716: pop {r4,r5,r6,r7,pc}
```
