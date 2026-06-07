# ListItem::ListItem
elf 0xa65bc body 50
Sig: undefined __thiscall ListItem(ListItem * this, String * param_1)

## decompile
```c

/* ListItem::ListItem(AbyssEngine::String*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,String *param_1)

{
  String *this_00;
  
  init(this,(EVP_PKEY_CTX *)param_1);
  this_00 = operator_new(0xc);
  AbyssEngine::String::String(this_00,param_1,false);
  this[0x24] = (ListItem)0x0;
  *(String **)(this + 0x1c) = this_00;
  return this;
}

```

## target disasm
```
  000b65bc: push {r4,r5,r6,r7,lr}
  000b65be: add r7,sp,#0xc
  000b65c0: push.w r8
  000b65c4: mov r6,r1
  000b65c6: mov r4,r0
  000b65c8: blx 0x000733cc
  000b65cc: movs r0,#0xc
  000b65ce: blx 0x0006eb24
  000b65d2: mov r5,r0
  000b65d4: mov r1,r6
  000b65d6: movs r2,#0x0
  000b65d8: mov.w r8,#0x0
  000b65dc: blx 0x0006f028
  000b65e0: strb.w r8,[r4,#0x24]
  000b65e4: mov r0,r4
  000b65e6: str r5,[r4,#0x1c]
  000b65e8: pop.w r8
  000b65ec: pop {r4,r5,r6,r7,pc}
```
