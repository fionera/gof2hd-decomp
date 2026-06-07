# ListItem::ListItem
elf 0xa63e0 body 58
Sig: undefined __thiscall ListItem(ListItem * this, int param_1, int param_2, String * param_3)

## decompile
```c

/* ListItem::ListItem(int, int, AbyssEngine::String*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,int param_1,int param_2,String *param_3)

{
  String *this_00;
  
  init(this,(EVP_PKEY_CTX *)param_1);
  *(int *)(this + 0x34) = param_1;
  this_00 = operator_new(0xc);
  AbyssEngine::String::String(this_00,param_3,false);
  *(int *)(this + 0x28) = param_2;
  *(String **)(this + 0x1c) = this_00;
  this[0x24] = (ListItem)0x1;
  return this;
}

```

## target disasm
```
  000b63e0: push {r4,r5,r6,r7,lr}
  000b63e2: add r7,sp,#0xc
  000b63e4: push.w r8
  000b63e8: mov r5,r3
  000b63ea: mov r8,r2
  000b63ec: mov r6,r1
  000b63ee: mov r4,r0
  000b63f0: blx 0x000733cc
  000b63f4: movs r0,#0xc
  000b63f6: str r6,[r4,#0x34]
  000b63f8: blx 0x0006eb24
  000b63fc: mov r6,r0
  000b63fe: mov r1,r5
  000b6400: movs r2,#0x0
  000b6402: blx 0x0006f028
  000b6406: movs r0,#0x1
  000b6408: str.w r8,[r4,#0x28]
  000b640c: str r6,[r4,#0x1c]
  000b640e: strb.w r0,[r4,#0x24]
  000b6412: mov r0,r4
  000b6414: pop.w r8
  000b6418: pop {r4,r5,r6,r7,pc}
```
