# Image2D*>::Array
elf 0x778d0 body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::Image2D*> * this)

## decompile
```c

/* Array<AbyssEngine::Image2D*>::Array() */

Array<AbyssEngine::Image2D*> * __thiscall
Array<AbyssEngine::Image2D*>::Array(Array<AbyssEngine::Image2D*> *this)

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
  000878d0: push {r4,r6,r7,lr}
  000878d2: add r7,sp,#0x8
  000878d4: mov r4,r0
  000878d6: movs r0,#0x4
  000878d8: blx 0x0006ec08
  000878dc: movs r2,#0x0
  000878de: movs r1,#0x1
  000878e0: str r2,[r0,#0x0]
  000878e2: strd r2,r0,[r4,#0x0]
  000878e6: mov r0,r4
  000878e8: str r1,[r4,#0x8]
  000878ea: pop {r4,r6,r7,pc}
```
