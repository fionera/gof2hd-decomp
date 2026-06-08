# Array<Array<Gun*>*>::~Array
elf 0xa299a body 22
Sig: undefined __thiscall ~Array(Array<Array<Gun*>*> * this)

## decompile
```c

/* Array<Array<Gun*>*>::~Array() */

Array<Array<Gun*>*> * __thiscall Array<Array<Gun*>*>::~Array(Array<Array<Gun*>*> *this)

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
  000b299a: push {r4,r6,r7,lr}
  000b299c: add r7,sp,#0x8
  000b299e: mov r4,r0
  000b29a0: ldr r0,[r0,#0x4]
  000b29a2: cbz r0,0x000b29a8
  000b29a4: blx 0x0006ebfc
  000b29a8: movs r0,#0x0
  000b29aa: str r0,[r4,#0x4]
  000b29ac: mov r0,r4
  000b29ae: pop {r4,r6,r7,pc}
```
