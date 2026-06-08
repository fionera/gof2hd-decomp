# Array<Ship*>::~Array
elf 0xa6b38 body 22
Sig: undefined __thiscall ~Array(Array<Ship*> * this)

## decompile
```c

/* Array<Ship*>::~Array() */

Array<Ship*> * __thiscall Array<Ship*>::~Array(Array<Ship*> *this)

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
  000b6b38: push {r4,r6,r7,lr}
  000b6b3a: add r7,sp,#0x8
  000b6b3c: mov r4,r0
  000b6b3e: ldr r0,[r0,#0x4]
  000b6b40: cbz r0,0x000b6b46
  000b6b42: blx 0x0006ebfc
  000b6b46: movs r0,#0x0
  000b6b48: str r0,[r4,#0x4]
  000b6b4a: mov r0,r4
  000b6b4c: pop {r4,r6,r7,pc}
```
