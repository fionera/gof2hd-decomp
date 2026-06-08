# Camera*>::Array
elf 0x778ec body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::Camera*> * this)

## decompile
```c

/* Array<AbyssEngine::Camera*>::Array() */

Array<AbyssEngine::Camera*> * __thiscall
Array<AbyssEngine::Camera*>::Array(Array<AbyssEngine::Camera*> *this)

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
  000878ec: push {r4,r6,r7,lr}
  000878ee: add r7,sp,#0x8
  000878f0: mov r4,r0
  000878f2: movs r0,#0x4
  000878f4: blx 0x0006ec08
  000878f8: movs r2,#0x0
  000878fa: movs r1,#0x1
  000878fc: str r2,[r0,#0x0]
  000878fe: strd r2,r0,[r4,#0x0]
  00087902: mov r0,r4
  00087904: str r1,[r4,#0x8]
  00087906: pop {r4,r6,r7,pc}
```
