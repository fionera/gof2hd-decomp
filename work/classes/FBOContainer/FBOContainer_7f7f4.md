# FBOContainer::FBOContainer
elf 0x7f7f4 body 64
Sig: undefined __thiscall FBOContainer(FBOContainer * this, Engine * param_1, String param_2)

## decompile
```c

/* AbyssEngine::FBOContainer::FBOContainer(AbyssEngine::Engine*, AbyssEngine::String) */

FBOContainer * __thiscall
AbyssEngine::FBOContainer::FBOContainer(FBOContainer *this,undefined4 param_1,String *param_3)

{
  String::String((String *)(this + 0x1c));
  this[0x18] = (FBOContainer)0x0;
  this[0x28] = (FBOContainer)0x0;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x10) = 0;
  String::operator=((String *)(this + 0x1c),param_3);
  *(undefined4 *)(this + 0x14) = param_1;
  return this;
}

```

## target disasm
```
  0008f7f4: push {r4,r5,r6,r7,lr}
  0008f7f6: add r7,sp,#0xc
  0008f7f8: push.w r8
  0008f7fc: add.w r4,r0,#0x1c
  0008f800: mov r5,r0
  0008f802: mov r6,r2
  0008f804: mov r8,r1
  0008f806: mov r0,r4
  0008f808: blx 0x0006efbc
  0008f80c: vmov.i32 q8,#0x0
  0008f810: movs r0,#0x0
  0008f812: mov r1,r5
  0008f814: strb r0,[r5,#0x18]
  0008f816: strb.w r0,[r5,#0x28]
  0008f81a: vst1.32 {d16,d17},[r1]!
  0008f81e: str r0,[r1,#0x0]
  0008f820: mov r0,r4
  0008f822: mov r1,r6
  0008f824: blx 0x0006f2b0
  0008f828: str.w r8,[r5,#0x14]
  0008f82c: mov r0,r5
  0008f82e: pop.w r8
  0008f832: pop {r4,r5,r6,r7,pc}
```
