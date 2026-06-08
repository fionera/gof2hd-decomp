# String*>*>::~Array
elf 0x125ac0 body 22
Sig: undefined __thiscall ~Array(Array<Array<AbyssEngine::String*>*> * this)

## decompile
```c

/* Array<Array<AbyssEngine::String*>*>::~Array() */

Array<Array<AbyssEngine::String*>*> * __thiscall
Array<Array<AbyssEngine::String*>*>::~Array(Array<Array<AbyssEngine::String*>*> *this)

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
  00135ac0: push {r4,r6,r7,lr}
  00135ac2: add r7,sp,#0x8
  00135ac4: mov r4,r0
  00135ac6: ldr r0,[r0,#0x4]
  00135ac8: cbz r0,0x00135ace
  00135aca: blx 0x0006ebfc
  00135ace: movs r0,#0x0
  00135ad0: str r0,[r4,#0x4]
  00135ad2: mov r0,r4
  00135ad4: pop {r4,r6,r7,pc}
```
