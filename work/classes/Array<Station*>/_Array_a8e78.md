# Array<Station*>::~Array
elf 0xa8e78 body 22
Sig: undefined __thiscall ~Array(Array<Station*> * this)

## decompile
```c

/* Array<Station*>::~Array() */

Array<Station*> * __thiscall Array<Station*>::~Array(Array<Station*> *this)

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
  000b8e78: push {r4,r6,r7,lr}
  000b8e7a: add r7,sp,#0x8
  000b8e7c: mov r4,r0
  000b8e7e: ldr r0,[r0,#0x4]
  000b8e80: cbz r0,0x000b8e86
  000b8e82: blx 0x0006ebfc
  000b8e86: movs r0,#0x0
  000b8e88: str r0,[r4,#0x4]
  000b8e8a: mov r0,r4
  000b8e8c: pop {r4,r6,r7,pc}
```
