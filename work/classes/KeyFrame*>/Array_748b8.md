# KeyFrame*>::Array
elf 0x748b8 body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::KeyFrame*> * this)

## decompile
```c

/* Array<AbyssEngine::KeyFrame*>::Array() */

Array<AbyssEngine::KeyFrame*> * __thiscall
Array<AbyssEngine::KeyFrame*>::Array(Array<AbyssEngine::KeyFrame*> *this)

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
  000848b8: push {r4,r6,r7,lr}
  000848ba: add r7,sp,#0x8
  000848bc: mov r4,r0
  000848be: movs r0,#0x4
  000848c0: blx 0x0006ec08
  000848c4: movs r2,#0x0
  000848c6: movs r1,#0x1
  000848c8: str r2,[r0,#0x0]
  000848ca: strd r2,r0,[r4,#0x0]
  000848ce: mov r0,r4
  000848d0: str r1,[r4,#0x8]
  000848d2: pop {r4,r6,r7,pc}
```
