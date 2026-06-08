# Matrix>::~Array
elf 0x779aa body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::AEMath::Matrix> * this)

## decompile
```c

/* Array<AbyssEngine::AEMath::Matrix>::~Array() */

Array<AbyssEngine::AEMath::Matrix> * __thiscall
Array<AbyssEngine::AEMath::Matrix>::~Array(Array<AbyssEngine::AEMath::Matrix> *this)

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
  000879aa: push {r4,r6,r7,lr}
  000879ac: add r7,sp,#0x8
  000879ae: mov r4,r0
  000879b0: ldr r0,[r0,#0x4]
  000879b2: cbz r0,0x000879b8
  000879b4: blx 0x0006ebfc
  000879b8: movs r0,#0x0
  000879ba: str r0,[r4,#0x4]
  000879bc: mov r0,r4
  000879be: pop {r4,r6,r7,pc}
```
