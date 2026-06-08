# Camera*>::~Array
elf 0x77a02 body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::Camera*> * this)

## decompile
```c

/* Array<AbyssEngine::Camera*>::~Array() */

Array<AbyssEngine::Camera*> * __thiscall
Array<AbyssEngine::Camera*>::~Array(Array<AbyssEngine::Camera*> *this)

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
  00087a02: push {r4,r6,r7,lr}
  00087a04: add r7,sp,#0x8
  00087a06: mov r4,r0
  00087a08: ldr r0,[r0,#0x4]
  00087a0a: cbz r0,0x00087a10
  00087a0c: blx 0x0006ebfc
  00087a10: movs r0,#0x0
  00087a12: str r0,[r4,#0x4]
  00087a14: mov r0,r4
  00087a16: pop {r4,r6,r7,pc}
```
