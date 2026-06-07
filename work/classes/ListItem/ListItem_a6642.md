# ListItem::ListItem
elf 0xa6642 body 56
Sig: undefined __thiscall ListItem(ListItem * this, String * param_1, bool param_2)

## decompile
```c

/* ListItem::ListItem(AbyssEngine::String*, bool) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,String *param_1,bool param_2)

{
  String *this_00;
  
  init(this,(EVP_PKEY_CTX *)param_1);
  this_00 = operator_new(0xc);
  AbyssEngine::String::String(this_00,param_1,false);
  this[0x44] = (ListItem)param_2;
  *(String **)(this + 0x1c) = this_00;
  this[0x24] = (ListItem)0x0;
  return this;
}

```

## target disasm
```
  000b6642: push {r4,r5,r6,r7,lr}
  000b6644: add r7,sp,#0xc
  000b6646: push {r8,r9,r11}
  000b664a: mov r8,r2
  000b664c: mov r5,r1
  000b664e: mov r4,r0
  000b6650: blx 0x000733cc
  000b6654: movs r0,#0xc
  000b6656: blx 0x0006eb24
  000b665a: mov r6,r0
  000b665c: mov r1,r5
  000b665e: movs r2,#0x0
  000b6660: mov.w r9,#0x0
  000b6664: blx 0x0006f028
  000b6668: strb.w r8,[r4,#0x44]
  000b666c: mov r0,r4
  000b666e: str r6,[r4,#0x1c]
  000b6670: strb.w r9,[r4,#0x24]
  000b6674: pop.w {r8,r9,r11}
  000b6678: pop {r4,r5,r6,r7,pc}
```
