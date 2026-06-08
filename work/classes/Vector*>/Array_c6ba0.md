# Vector*>::Array
elf 0xc6ba0 body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::AEMath::Vector*> * this)

## decompile
```c

/* Array<AbyssEngine::AEMath::Vector*>::Array() */

Array<AbyssEngine::AEMath::Vector*> * __thiscall
Array<AbyssEngine::AEMath::Vector*>::Array(Array<AbyssEngine::AEMath::Vector*> *this)

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
  000d6ba0: push {r4,r6,r7,lr}
  000d6ba2: add r7,sp,#0x8
  000d6ba4: mov r4,r0
  000d6ba6: movs r0,#0x4
  000d6ba8: blx 0x0006ec08
  000d6bac: movs r2,#0x0
  000d6bae: movs r1,#0x1
  000d6bb0: str r2,[r0,#0x0]
  000d6bb2: strd r2,r0,[r4,#0x0]
  000d6bb6: mov r0,r4
  000d6bb8: str r1,[r4,#0x8]
  000d6bba: pop {r4,r6,r7,pc}
```
