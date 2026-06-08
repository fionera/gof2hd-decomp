# Array<float>::~Array
elf 0xa72e0 body 22
Sig: undefined __thiscall ~Array(Array<float> * this)

## decompile
```c

/* Array<float>::~Array() */

Array<float> * __thiscall Array<float>::~Array(Array<float> *this)

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
  000b72e0: push {r4,r6,r7,lr}
  000b72e2: add r7,sp,#0x8
  000b72e4: mov r4,r0
  000b72e6: ldr r0,[r0,#0x4]
  000b72e8: cbz r0,0x000b72ee
  000b72ea: blx 0x0006ebfc
  000b72ee: movs r0,#0x0
  000b72f0: str r0,[r4,#0x4]
  000b72f2: mov r0,r4
  000b72f4: pop {r4,r6,r7,pc}
```
