# ListItem::ListItem
elf 0xa6572 body 60
Sig: undefined __thiscall ListItem(ListItem * this, String * param_1, bool param_2, int param_3)

## decompile
```c

/* ListItem::ListItem(AbyssEngine::String*, bool, int) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,String *param_1,bool param_2,int param_3)

{
  String *this_00;
  
  init(this,(EVP_PKEY_CTX *)param_1);
  this_00 = operator_new(0xc);
  AbyssEngine::String::String(this_00,param_1,false);
  this[0x38] = (ListItem)0x1;
  *(String **)(this + 0x1c) = this_00;
  *(int *)(this + 0x30) = param_3;
  this[0x24] = (ListItem)param_2;
  return this;
}

```

## target disasm
```
  000b6572: push {r4,r5,r6,r7,lr}
  000b6574: add r7,sp,#0xc
  000b6576: push {r8,r9,r11}
  000b657a: mov r8,r3
  000b657c: mov r9,r2
  000b657e: mov r5,r1
  000b6580: mov r6,r0
  000b6582: blx 0x000733cc
  000b6586: movs r0,#0xc
  000b6588: blx 0x0006eb24
  000b658c: mov r4,r0
  000b658e: mov r1,r5
  000b6590: movs r2,#0x0
  000b6592: blx 0x0006f028
  000b6596: movs r0,#0x1
  000b6598: strb.w r0,[r6,#0x38]
  000b659c: mov r0,r6
  000b659e: str r4,[r6,#0x1c]
  000b65a0: str.w r8,[r6,#0x30]
  000b65a4: strb.w r9,[r6,#0x24]
  000b65a8: pop.w {r8,r9,r11}
  000b65ac: pop {r4,r5,r6,r7,pc}
```
