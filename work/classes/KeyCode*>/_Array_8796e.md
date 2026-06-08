# KeyCode*>::~Array
elf 0x8796e body 22
Sig: undefined __thiscall ~Array(Array<AbyssEngine::KeyCode*> * this)

## decompile
```c

/* Array<AbyssEngine::KeyCode*>::~Array() */

Array<AbyssEngine::KeyCode*> * __thiscall
Array<AbyssEngine::KeyCode*>::~Array(Array<AbyssEngine::KeyCode*> *this)

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
  0009796e: push {r4,r6,r7,lr}
  00097970: add r7,sp,#0x8
  00097972: mov r4,r0
  00097974: ldr r0,[r0,#0x4]
  00097976: cbz r0,0x0009797c
  00097978: blx 0x0006ebfc
  0009797c: movs r0,#0x0
  0009797e: str r0,[r4,#0x4]
  00097980: mov r0,r4
  00097982: pop {r4,r6,r7,pc}
```
