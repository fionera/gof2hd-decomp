# Vector*>::~Array
elf 0x997d0 body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::AEMath::Vector*> * this)

## decompile
```c

/* Array<AbyssEngine::AEMath::Vector*>::~Array() */

Array<AbyssEngine::AEMath::Vector*> * __thiscall
Array<AbyssEngine::AEMath::Vector*>::~Array(Array<AbyssEngine::AEMath::Vector*> *this)

{
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  000a97d0: push {r4,r6,r7,lr}
  000a97d2: add r7,sp,#0x8
  000a97d4: mov r4,r0
  000a97d6: ldr r0,[r0,#0x4]
  000a97d8: cbz r0,0x000a97de
  000a97da: blx 0x0006ebfc
  000a97de: movs r0,#0x0
  000a97e0: str r0,[r4,#0x4]
  000a97e2: mov r0,r4
  000a97e4: pop {r4,r6,r7,pc}
```
