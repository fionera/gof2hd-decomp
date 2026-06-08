# Array<SpacePoint*>::~Array
elf 0xa5b00 body 22
Sig: undefined __thiscall ~Array(Array<SpacePoint*> * this)

## decompile
```c

/* Array<SpacePoint*>::~Array() */

Array<SpacePoint*> * __thiscall Array<SpacePoint*>::~Array(Array<SpacePoint*> *this)

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
  000b5b00: push {r4,r6,r7,lr}
  000b5b02: add r7,sp,#0x8
  000b5b04: mov r4,r0
  000b5b06: ldr r0,[r0,#0x4]
  000b5b08: cbz r0,0x000b5b0e
  000b5b0a: blx 0x0006ebfc
  000b5b0e: movs r0,#0x0
  000b5b10: str r0,[r4,#0x4]
  000b5b12: mov r0,r4
  000b5b14: pop {r4,r6,r7,pc}
```
