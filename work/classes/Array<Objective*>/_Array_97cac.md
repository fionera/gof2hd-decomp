# Array<Objective*>::~Array
elf 0x97cac body 22
Sig: undefined __thiscall ~Array(Array<Objective*> * this)

## decompile
```c

/* Array<Objective*>::~Array() */

Array<Objective*> * __thiscall Array<Objective*>::~Array(Array<Objective*> *this)

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
  000a7cac: push {r4,r6,r7,lr}
  000a7cae: add r7,sp,#0x8
  000a7cb0: mov r4,r0
  000a7cb2: ldr r0,[r0,#0x4]
  000a7cb4: cbz r0,0x000a7cba
  000a7cb6: blx 0x0006ebfc
  000a7cba: movs r0,#0x0
  000a7cbc: str r0,[r4,#0x4]
  000a7cbe: mov r0,r4
  000a7cc0: pop {r4,r6,r7,pc}
```
