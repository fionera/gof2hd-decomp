# Array<Agent*>::~Array
elf 0xa6bb4 body 22
Sig: undefined __thiscall ~Array(Array<Agent*> * this)

## decompile
```c

/* Array<Agent*>::~Array() */

Array<Agent*> * __thiscall Array<Agent*>::~Array(Array<Agent*> *this)

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
  000b6bb4: push {r4,r6,r7,lr}
  000b6bb6: add r7,sp,#0x8
  000b6bb8: mov r4,r0
  000b6bba: ldr r0,[r0,#0x4]
  000b6bbc: cbz r0,0x000b6bc2
  000b6bbe: blx 0x0006ebfc
  000b6bc2: movs r0,#0x0
  000b6bc4: str r0,[r4,#0x4]
  000b6bc6: mov r0,r4
  000b6bc8: pop {r4,r6,r7,pc}
```
