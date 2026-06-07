# ListItem::ListItem
elf 0xa66a0 body 68
Sig: undefined __thiscall ListItem(ListItem * this, String * param_1, String * param_2)

## decompile
```c

/* ListItem::ListItem(AbyssEngine::String*, AbyssEngine::String*) */

ListItem * __thiscall ListItem::ListItem(ListItem *this,String *param_1,String *param_2)

{
  String *pSVar1;
  
  init(this,(EVP_PKEY_CTX *)param_1);
  pSVar1 = operator_new(0xc);
  AbyssEngine::String::String(pSVar1,param_1,false);
  *(String **)(this + 0x1c) = pSVar1;
  pSVar1 = operator_new(0xc);
  AbyssEngine::String::String(pSVar1,param_2,false);
  this[0x24] = (ListItem)0x0;
  *(String **)(this + 0x20) = pSVar1;
  return this;
}

```

## target disasm
```
  000b66a0: push {r4,r5,r6,r7,lr}
  000b66a2: add r7,sp,#0xc
  000b66a4: push.w r8
  000b66a8: mov r8,r2
  000b66aa: mov r5,r1
  000b66ac: mov r4,r0
  000b66ae: blx 0x000733cc
  000b66b2: movs r0,#0xc
  000b66b4: blx 0x0006eb24
  000b66b8: mov r6,r0
  000b66ba: mov r1,r5
  000b66bc: movs r2,#0x0
  000b66be: blx 0x0006f028
  000b66c2: movs r0,#0xc
  000b66c4: str r6,[r4,#0x1c]
  000b66c6: blx 0x0006eb24
  000b66ca: mov r6,r0
  000b66cc: mov r1,r8
  000b66ce: movs r2,#0x0
  000b66d0: movs r5,#0x0
  000b66d2: blx 0x0006f028
  000b66d6: strb.w r5,[r4,#0x24]
  000b66da: mov r0,r4
  000b66dc: str r6,[r4,#0x20]
  000b66de: pop.w r8
  000b66e2: pop {r4,r5,r6,r7,pc}
```
