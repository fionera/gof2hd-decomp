# Array<Item*>::~Array
elf 0xa6b9e body 22
Sig: undefined __thiscall ~Array(Array<Item*> * this)

## decompile
```c

/* Array<Item*>::~Array() */

Array<Item*> * __thiscall Array<Item*>::~Array(Array<Item*> *this)

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
  000b6b9e: push {r4,r6,r7,lr}
  000b6ba0: add r7,sp,#0x8
  000b6ba2: mov r4,r0
  000b6ba4: ldr r0,[r0,#0x4]
  000b6ba6: cbz r0,0x000b6bac
  000b6ba8: blx 0x0006ebfc
  000b6bac: movs r0,#0x0
  000b6bae: str r0,[r4,#0x4]
  000b6bb0: mov r0,r4
  000b6bb2: pop {r4,r6,r7,pc}
```
