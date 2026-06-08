# PendingProduct::PendingProduct
elf 0x12d448 body 42
Sig: undefined __thiscall PendingProduct(PendingProduct * this, int param_1, String param_2, int param_3, int param_4)

## decompile
```c

/* PendingProduct::PendingProduct(int, AbyssEngine::String, int, int) */

PendingProduct * __thiscall
PendingProduct::PendingProduct
          (PendingProduct *this,undefined4 param_1,String *param_3,undefined4 param_4,
          undefined4 param_5)

{
  String *this_00;
  
  this_00 = (String *)AbyssEngine::String::String((String *)this);
  *(undefined4 *)(this_00 + 0x14) = param_1;
  AbyssEngine::String::operator=(this_00,param_3);
  *(undefined4 *)(this + 0xc) = param_4;
  *(undefined4 *)(this + 0x10) = param_5;
  return this;
}

```

## target disasm
```
  0013d448: push {r4,r5,r6,r7,lr}
  0013d44a: add r7,sp,#0xc
  0013d44c: push.w r8
  0013d450: mov r8,r3
  0013d452: mov r6,r2
  0013d454: mov r5,r1
  0013d456: mov r4,r0
  0013d458: blx 0x0006efbc
  0013d45c: str r5,[r0,#0x14]
  0013d45e: mov r1,r6
  0013d460: blx 0x0006f2b0
  0013d464: ldr r0,[r7,#0x8]
  0013d466: strd r8,r0,[r4,#0xc]
  0013d46a: mov r0,r4
  0013d46c: pop.w r8
  0013d470: pop {r4,r5,r6,r7,pc}
```
