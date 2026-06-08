# AESoundInterface*>::Array
elf 0x7fa8e body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::AESoundInterface*> * this)

## decompile
```c

/* Array<AbyssEngine::AESoundInterface*>::Array() */

Array<AbyssEngine::AESoundInterface*> * __thiscall
Array<AbyssEngine::AESoundInterface*>::Array(Array<AbyssEngine::AESoundInterface*> *this)

{
  undefined4 *puVar1;
  
  puVar1 = operator_new__(4);
  *puVar1 = 0;
  *(undefined4 *)this = 0;
  *(undefined4 **)(this + 4) = puVar1;
  *(undefined4 *)(this + 8) = 1;
  return this;
}

```

## target disasm
```
  0008fa8e: push {r4,r6,r7,lr}
  0008fa90: add r7,sp,#0x8
  0008fa92: mov r4,r0
  0008fa94: movs r0,#0x4
  0008fa96: blx 0x0006ec08
  0008fa9a: movs r2,#0x0
  0008fa9c: movs r1,#0x1
  0008fa9e: str r2,[r0,#0x0]
  0008faa0: strd r2,r0,[r4,#0x0]
  0008faa4: mov r0,r4
  0008faa6: str r1,[r4,#0x8]
  0008faa8: pop {r4,r6,r7,pc}
```
