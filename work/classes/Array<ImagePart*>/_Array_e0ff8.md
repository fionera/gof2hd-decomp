# Array<ImagePart*>::~Array
elf 0xe0ff8 body 22
Sig: undefined __thiscall ~Array(Array<ImagePart*> * this)

## decompile
```c

/* Array<ImagePart*>::~Array() */

Array<ImagePart*> * __thiscall Array<ImagePart*>::~Array(Array<ImagePart*> *this)

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
  000f0ff8: push {r4,r6,r7,lr}
  000f0ffa: add r7,sp,#0x8
  000f0ffc: mov r4,r0
  000f0ffe: ldr r0,[r0,#0x4]
  000f1000: cbz r0,0x000f1006
  000f1002: blx 0x0006ebfc
  000f1006: movs r0,#0x0
  000f1008: str r0,[r4,#0x4]
  000f100a: mov r0,r4
  000f100c: pop {r4,r6,r7,pc}
```
