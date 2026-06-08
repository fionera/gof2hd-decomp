# Array<Wanted*>::~Array
elf 0xa8ede body 22
Sig: undefined __thiscall ~Array(Array<Wanted*> * this)

## decompile
```c

/* Array<Wanted*>::~Array() */

Array<Wanted*> * __thiscall Array<Wanted*>::~Array(Array<Wanted*> *this)

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
  000b8ede: push {r4,r6,r7,lr}
  000b8ee0: add r7,sp,#0x8
  000b8ee2: mov r4,r0
  000b8ee4: ldr r0,[r0,#0x4]
  000b8ee6: cbz r0,0x000b8eec
  000b8ee8: blx 0x0006ebfc
  000b8eec: movs r0,#0x0
  000b8eee: str r0,[r4,#0x4]
  000b8ef0: mov r0,r4
  000b8ef2: pop {r4,r6,r7,pc}
```
