# Resource*>::~Array
elf 0x77a44 body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::Resource*> * this)

## decompile
```c

/* Array<AbyssEngine::Resource*>::~Array() */

Array<AbyssEngine::Resource*> * __thiscall
Array<AbyssEngine::Resource*>::~Array(Array<AbyssEngine::Resource*> *this)

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
  00087a44: push {r4,r6,r7,lr}
  00087a46: add r7,sp,#0x8
  00087a48: mov r4,r0
  00087a4a: ldr r0,[r0,#0x4]
  00087a4c: cbz r0,0x00087a52
  00087a4e: blx 0x0006ebfc
  00087a52: movs r0,#0x0
  00087a54: str r0,[r4,#0x4]
  00087a56: mov r0,r4
  00087a58: pop {r4,r6,r7,pc}
```
