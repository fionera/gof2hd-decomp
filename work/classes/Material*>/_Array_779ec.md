# Material*>::~Array
elf 0x779ec body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::Material*> * this)

## decompile
```c

/* Array<AbyssEngine::Material*>::~Array() */

Array<AbyssEngine::Material*> * __thiscall
Array<AbyssEngine::Material*>::~Array(Array<AbyssEngine::Material*> *this)

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
  000879ec: push {r4,r6,r7,lr}
  000879ee: add r7,sp,#0x8
  000879f0: mov r4,r0
  000879f2: ldr r0,[r0,#0x4]
  000879f4: cbz r0,0x000879fa
  000879f6: blx 0x0006ebfc
  000879fa: movs r0,#0x0
  000879fc: str r0,[r4,#0x4]
  000879fe: mov r0,r4
  00087a00: pop {r4,r6,r7,pc}
```
