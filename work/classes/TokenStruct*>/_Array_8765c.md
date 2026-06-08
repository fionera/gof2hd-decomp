# TokenStruct*>::~Array
elf 0x8765c body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::TokenStruct*> * this)

## decompile
```c

/* Array<AbyssEngine::TokenStruct*>::~Array() */

Array<AbyssEngine::TokenStruct*> * __thiscall
Array<AbyssEngine::TokenStruct*>::~Array(Array<AbyssEngine::TokenStruct*> *this)

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
  0009765c: push {r4,r6,r7,lr}
  0009765e: add r7,sp,#0x8
  00097660: mov r4,r0
  00097662: ldr r0,[r0,#0x4]
  00097664: cbz r0,0x0009766a
  00097666: blx 0x0006ebfc
  0009766a: movs r0,#0x0
  0009766c: str r0,[r4,#0x4]
  0009766e: mov r0,r4
  00097670: pop {r4,r6,r7,pc}
```
