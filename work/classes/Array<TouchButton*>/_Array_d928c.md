# Array<TouchButton*>::~Array
elf 0xd928c body 22
Sig: undefined __thiscall ~Array(Array<TouchButton*> * this)

## decompile
```c

/* Array<TouchButton*>::~Array() */

Array<TouchButton*> * __thiscall Array<TouchButton*>::~Array(Array<TouchButton*> *this)

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
  000e928c: push {r4,r6,r7,lr}
  000e928e: add r7,sp,#0x8
  000e9290: mov r4,r0
  000e9292: ldr r0,[r0,#0x4]
  000e9294: cbz r0,0x000e929a
  000e9296: blx 0x0006ebfc
  000e929a: movs r0,#0x0
  000e929c: str r0,[r4,#0x4]
  000e929e: mov r0,r4
  000e92a0: pop {r4,r6,r7,pc}
```
