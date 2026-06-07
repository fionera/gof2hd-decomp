# ListItem::ListItem
elf 0xa65fc body 56
Sig: undefined __thiscall ListItem(ListItem * this, String * param_1, int param_2)

## decompile
```c

/* ListItem::ListItem(AbyssEngine::String*, int) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,String *param_1,int param_2)

{
  String *this_00;
  
  init(this,(EVP_PKEY_CTX *)param_1);
  this_00 = operator_new(0xc);
  AbyssEngine::String::String(this_00,param_1,false);
  *(int *)(this + 0x30) = param_2;
  *(String **)(this + 0x1c) = this_00;
  this[0x24] = (ListItem)0x0;
  return this;
}

```

## target disasm
```
  000b65fc: push {r4,r5,r6,r7,lr}
  000b65fe: add r7,sp,#0xc
  000b6600: push {r8,r9,r11}
  000b6604: mov r8,r2
  000b6606: mov r5,r1
  000b6608: mov r4,r0
  000b660a: blx 0x000733cc
  000b660e: movs r0,#0xc
  000b6610: blx 0x0006eb24
  000b6614: mov r6,r0
  000b6616: mov r1,r5
  000b6618: movs r2,#0x0
  000b661a: mov.w r9,#0x0
  000b661e: blx 0x0006f028
  000b6622: str.w r8,[r4,#0x30]
  000b6626: mov r0,r4
  000b6628: str r6,[r4,#0x1c]
  000b662a: strb.w r9,[r4,#0x24]
  000b662e: pop.w {r8,r9,r11}
  000b6632: pop {r4,r5,r6,r7,pc}
```
